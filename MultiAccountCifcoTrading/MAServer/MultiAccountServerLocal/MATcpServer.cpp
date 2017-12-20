#include "stdafx.h"
#include "MultiAccountServer.h"
#include <map>
#include <vector>
#include <queue>
#include "Tools_Ansi.h"
#include "Tools_Win32.h"
#include "TcpServerWorker.h"
#include "CommonPkg.h"
#include <string>
#include "MultiAccountCmd.h"
#include "MultiAccountStruct.h"
using namespace std;

extern CWriteLog* g_pWriteLog;
extern char* g_pMASem;
extern int g_nMASemCount;
extern std::map<SOCKET,Stru_DataSocket> g_TcpUsers;					//�Ѿ�����tcp���ӵ��û�
extern std::map<int, std::set<SOCKET>> g_OnlineUser;			//�����û�
extern std::set<SOCKET> g_OnlineUserSet;			//�����û������ڲ���ʹ��
extern queue<RequestData> g_msgQueue;							//��Ϣ����

void ProcessPackage( Stru_DataSocket& soketData );
bool VerifyRequest( Stru_UniPkgHead& head );
void SaveReqToMsgQueue( Stru_DataSocket& soketData, Stru_UniPkgHead& head );
void ProcessLicenceReq( SOCKET hSocket );
void ProcessAddrReq( SOCKET hSocket, const char* pBuf );

MULTIACCOUNTSERVER_API void UdpRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{

}

MULTIACCOUNTSERVER_API void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	if( pData == NULL ) return;

	std::map<SOCKET,Stru_DataSocket>::iterator it = g_TcpUsers.find(hSocket);
	if( it != g_TcpUsers.end() && it->second.pStreamCache != NULL )
	{
		it->second.pStreamCache->SaveDataTo(pData,DataLen);
	}

	ProcessPackage( it->second );
}

MULTIACCOUNTSERVER_API void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	Stru_DataSocket TmpDataSocket;
	TmpDataSocket.hScoket = hSocket;
	TmpDataSocket.pStreamCache = new CMemoryCache(655360);
	TmpDataSocket.RemoteAddr = RemoteAddr;
	time(&TmpDataSocket.HeartbeatTime);

	std::map<SOCKET, Stru_DataSocket>::iterator it = g_TcpUsers.find(hSocket);
	if( it != g_TcpUsers.end())
	{
		if(it->second.pStreamCache)
		{
			delete it->second.pStreamCache;
			it->second.pStreamCache = NULL;
		}

		g_TcpUsers.erase(it);
	}

	g_TcpUsers[hSocket]=TmpDataSocket;

	if(g_pWriteLog)
		g_pWriteLog->WriteLog_Fmt("��������",WriteLogLevel_DEBUGINFO,"%s/%d-%d",
		CTools_Ansi::ConvertdwIP(TmpDataSocket.RemoteAddr.IP),
		TmpDataSocket.RemoteAddr.Port, hSocket);
}

MULTIACCOUNTSERVER_API void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	std::map<SOCKET,Stru_DataSocket>::iterator it = g_TcpUsers.find(hSocket);
	if(it != g_TcpUsers.end())
	{
		if(g_pWriteLog)
			g_pWriteLog->WriteLog_Fmt("�Ͽ�����",WriteLogLevel_DEBUGINFO,"%s/%d-%d",
			CTools_Ansi::ConvertdwIP(it->second.RemoteAddr.IP),
			it->second.RemoteAddr.Port, hSocket);

		if(it->second.pStreamCache)
		{
			delete it->second.pStreamCache;
			it->second.pStreamCache = NULL;
		}

		SOCKET hSocket = it->first;
		g_TcpUsers.erase(it);

		//���������û�
		if(g_pMASem) 
		{
			CTools_Win32::TakeWinSem(g_pMASem);
			g_nMASemCount++;
		}

		std::map<int, std::set<SOCKET>>::iterator it_online = g_OnlineUser.begin();
		for ( ; it_online != g_OnlineUser.end(); it_online++ )
		{
			int nCount = it_online->second.erase(hSocket);
			if ( nCount != 0 )
			{
				if ( it_online->second.empty() )
				{
					g_OnlineUser.erase(it_online);
				}

				break;
			}
		}

		g_OnlineUserSet.erase(hSocket);

		if(g_pMASem) 
		{
			CTools_Win32::GiveWinSem(g_pMASem);
			g_nMASemCount--;
		}
	}
}

MULTIACCOUNTSERVER_API void TcpSvrOntimeFunc(void)
{
	if ( g_TcpUsers.empty() )
	{
		return;
	}

	time_t curTime;
	time(&curTime);

	//��鳬ʱ����
	std::map<SOCKET,Stru_DataSocket>::iterator it = g_TcpUsers.begin();
	std::vector<SOCKET> vTemp;
	vTemp.clear();
	for( ;it != g_TcpUsers.end(); ++it)
	{
		int nDeltaSecs = (int)(curTime-it->second.HeartbeatTime);
		if( nDeltaSecs > 30000 )	//30�볬ʱ
		{
			if(g_pWriteLog)
				g_pWriteLog->WriteLog_Fmt("���ӳ�ʱ���Ͽ�����",WriteLogLevel_DEBUGINFO,"%s/%d ��ʱʱ��%ds",
				CTools_Ansi::ConvertdwIP(it->second.RemoteAddr.IP),
				it->second.RemoteAddr.Port,
				nDeltaSecs);

			vTemp.push_back( it->first );
		}
	}

	for ( int i = 0; i < static_cast<int>(vTemp.size()); i++ )
	{
		CSocketBase::CloseSocket(vTemp[i]);
	}
}

void ProcessPackage( Stru_DataSocket& soketData )
{
	if ( NULL == soketData.pStreamCache )
	{
		return;
	}

	int nCacheLen = 0;
	while( (nCacheLen = soketData.pStreamCache->GetLen()) >= sizeof(Stru_UniPkgHead) + sizeof(TAIL))
	{
		char szHeadBuf[sizeof(Stru_UniPkgHead)];
		memset(szHeadBuf, 0, sizeof(Stru_UniPkgHead));
		soketData.pStreamCache->PeekFrom(szHeadBuf, sizeof(Stru_UniPkgHead));
		int nPackageHead = *(DWORD*)szHeadBuf;

		// ��鲢������ͷ����Ĳ���
		if( nPackageHead != HeadID_UniPkg)
		{
			int i = 0;
			for( ; i < sizeof(Stru_UniPkgHead) - sizeof(DWORD) + 1; i++)
			{
				if(*(DWORD*)(szHeadBuf + i) == HeadID_UniPkg )
				{
					break;
				}
			}

			soketData.pStreamCache->DeleteData(i);
			continue;
		}

		Stru_UniPkgHead pkgHead;
		soketData.pStreamCache->PeekFrom(&pkgHead, sizeof(Stru_UniPkgHead));
		//�ȴ�һ��������
		if ( nCacheLen < static_cast<int>(sizeof(Stru_UniPkgHead) + pkgHead.len + sizeof(TAIL)) )
		{
			return;
		}

		// ���Ϸ�������Ϣ������Ϣ���У���Work�̴߳���
		if ( VerifyRequest(pkgHead))
		{
			time(&soketData.HeartbeatTime);
			Stru_UniPkgHead head;
			soketData.pStreamCache->GetFrom(&head, sizeof(Stru_UniPkgHead));
			SaveReqToMsgQueue(soketData, head);
			if(g_pWriteLog)
				g_pWriteLog->WriteLog_Fmt("����ҵ������",WriteLogLevel_DEBUGINFO,"%s/%d moduleID=%d, CMDID=%d",
				CTools_Ansi::ConvertdwIP(soketData.RemoteAddr.IP),
				soketData.RemoteAddr.Port, head.moduleid, head.cmdid);

			continue;
		}

		//��顢����������
		if( pkgHead.cmdid == CMDID_HEARTBEAT 
			&& pkgHead.len == 0 )
		{
			Stru_Heartbeat heartBeatPkg;
			memset(&heartBeatPkg, 0, sizeof(heartBeatPkg));
			soketData.pStreamCache->GetFrom(&heartBeatPkg, sizeof(Stru_Heartbeat));
			if( heartBeatPkg.tail == TailID_UniPkg )
			{			
				//����������
				time(&soketData.HeartbeatTime);
				heartBeatPkg.head.cmdid = CMDID_HEARTBEATRSP;
				//heartBeatPkg.head.moduleid = MODULE_ID_DBCOMPARE_CLIENT;
				CSocketBase::SendDataTcp(soketData.hScoket, (char*)&heartBeatPkg, sizeof(Stru_Heartbeat));
			}
			continue;
		}

		//��鲢����������������һ��������������ǰ��ò�����������Ϊ��һ�������������
		if( nCacheLen >= (int)(sizeof(Stru_UniPkgHead) + pkgHead.len + sizeof(TAIL)) )
		{
			soketData.pStreamCache->DeleteData( sizeof(Stru_UniPkgHead) + pkgHead.len + sizeof(TAIL) );
			if(g_pWriteLog)
			{
				g_pWriteLog->WriteLog_Fmt("�������Ϸ�����",WriteLogLevel_DEBUGINFO,"");
			}
			continue;
		}

		break;
	}
}

void SaveReqToMsgQueue( Stru_DataSocket& soketData, Stru_UniPkgHead& head )
{
	BYTE* pBuf = NULL;
	if ( head.len != 0 )
	{
		pBuf = new BYTE[head.len];
		if ( NULL == pBuf )
		{
			soketData.pStreamCache->DeleteData(head.len+sizeof(TAIL));
			if(g_pWriteLog)
				g_pWriteLog->WriteLog_Fmt("�ڴ治��",WriteLogLevel_DEBUGINFO, "%d", head.cmdid);
			return;
		}

		memset( pBuf, 0, head.len );
		soketData.pStreamCache->GetFrom(pBuf, head.len);
	}

	TAIL tail;
	soketData.pStreamCache->GetFrom(&tail, sizeof(TAIL));
	if ( tail == TailID_UniPkg )
	{
		if(g_pMASem) 
		{
			CTools_Win32::TakeWinSem(g_pMASem);
			g_nMASemCount++;
		}

		g_msgQueue.push(RequestData(soketData.hScoket, head, pBuf));

		if(g_pMASem) 
		{
			CTools_Win32::GiveWinSem(g_pMASem);
			g_nMASemCount--;
		}
	}
	else
	{
		if(g_pWriteLog)
			g_pWriteLog->WriteLog_Fmt("��β��һ��",WriteLogLevel_DEBUGINFO, "%d", tail);

		if ( NULL != pBuf )
		{
			delete[] pBuf;
			pBuf = NULL;
		}
	}

	return;
}

bool VerifyRequest( Stru_UniPkgHead& head )
{
	switch(head.cmdid)
	{
	case Cmd_MA_Login_Req:
		return head.len == sizeof(LoginReq);
	case Cmd_MA_AddAccount_Req:
		return head.len == sizeof(AccountInfo);
	case Cmd_MA_DelAccount_Req:
		return head.len % sizeof(int) == 0;
	case Cmd_MA_ModifyPwd_Req:
		return head.len == sizeof(ModifyPwd);
	case Cmd_MA_ModifyAccount_Req:
		return head.len == sizeof(AccountInfo);
	case Cmd_MA_QryALLAccount_Req:
		return head.len == 0;
	case Cmd_MA_QryAccountByOwner_Req:
		return head.len == sizeof(int);

	case Cmd_MA_AddIDCard_Req:
		return head.len == sizeof(IDCardInfo);
	case Cmd_MA_DelIDCard_Req:
		return head.len == sizeof(int);
	case Cmd_MA_ModifyIDCard_Req:
		return head.len == sizeof(IDCardInfo);
	case Cmd_MA_QryIDCard_Req:
		return head.len == 0;

	case Cmd_MA_AddServerGroup_Req:
		return head.len == sizeof(ServerGroup);
	case Cmd_MA_DelServerGroup_Req:
		return head.len == sizeof(int);
	case Cmd_MA_ModifyServerGroup_Req:
		return head.len == sizeof(ServerGroup);
	case Cmd_MA_QryServerGroup_Req:
		return head.len == sizeof(int);
	case Cmd_MA_AddServerAddr_Req:
		return head.len == sizeof(ServerAddr);
	case Cmd_MA_DelServerAddr_Req:
		return head.len == sizeof(int);
	case Cmd_MA_ModifyServerAddr_Req:
		return head.len == sizeof(ServerAddr);
	case Cmd_MA_QryServerAddr_Req:
		return head.len == sizeof(int);

	case Cmd_MA_AddBroker_Req:
		return head.len == sizeof(BrokerInfo);
	case Cmd_MA_DelBroker_Req:
		return head.len == sizeof(int);
	case Cmd_MA_ModifyBroker_Req:
		return head.len == sizeof(BrokerInfo);
	case Cmd_MA_QryBroker_Req:
		return head.len == sizeof(int);
	case Cmd_MA_QryUsableBroker_Req:
		return head.len == sizeof(int);

	case Cmd_MA_AddTradeAccount_Req:
		return head.len == sizeof(TradeAccount);
	case Cmd_MA_DelTradeAccount_Req:
		return head.len == sizeof(TradeAccountKey);
	case Cmd_MA_ModifyTradeAccount_Req:
		return head.len == sizeof(TradeAccount);
	case Cmd_MA_QryTradeAccount_Req:
		return head.len == sizeof(int);
	case Cmd_MA_SaveTradeAccount_Req:
		return (head.len >= sizeof(int) && 
			(head.len-sizeof(int)) % sizeof(TradeAccount) == 0);

	case Cmd_MA_AddPermissionModule_Req:
		return head.len == sizeof(PermissionModule);
	case Cmd_MA_DelPermissionModule_Req:
		return head.len == sizeof(int);
	case Cmd_MA_ModifyPermissionModule_Req:
		return head.len == sizeof(PermissionModule);
	case Cmd_MA_QryPermissionModule_Req:
		return head.len == 0;

	case Cmd_MA_AddPermission_Req:
		return head.len == sizeof(Permission);
	case Cmd_MA_DelPermission_Req:
		return head.len == sizeof(int);
	case Cmd_MA_ModifyPermission_Req:
		return head.len == sizeof(Permission);
	case Cmd_MA_QryPermissionByOwner_Req:
		return head.len == sizeof(int);
	case Cmd_MA_QryPermissionByID_Req:
		return head.len == sizeof(int);

	case Cmd_MA_AddMessage_Req:
		return (head.len > sizeof(MessageInfo) &&
			(head.len-sizeof(MessageInfo)) % sizeof(TargetAccount) == 0);
	case Cmd_MA_QryMessage_Req:
		return head.len == sizeof(int);
	case Cmd_MA_QryTargetAccount_Req:
		return head.len == sizeof(int);
	case Cmd_MA_QryWaitForSendAccount_Req:
		return head.len == sizeof(int);
	case Cmd_MA_QryUnReadMessage_Req:
		return head.len == sizeof(int);

	default:
		return false;
	}
}