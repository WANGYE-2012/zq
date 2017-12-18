#include "StdAfx.h"
#include "TCPDataService.h"
#include "Tools_Ansi.h"
#include "FileOpr.h "

using namespace std;
#include "QuoteCommon.h"
#define TIMER_SERVER_ADDR_CHECK	1
#define TIMER_HEARTBEAT			2
UINT_PTR g_lTimeQuto;
#if 0
#define LOG_INFO(fmt, ...)  ;
#else
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","PlatformCTPQuot", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif

//////////////////////////////////////////////////////////////////////////
HWND CTCPDataService::m_HwndParent = NULL;
CTCPDataService* CTCPDataService::m_pInstance = NULL;
int g_QuotServerSeqID = 0;
//��ʱ���licence��������ַ�Ƿ����������ʱ����������
VOID CALLBACK CTCPDataService::OnTimer1(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if( m_pInstance == NULL)
		return;
/*
	if ( m_pInstance->m_step == REQ_STEP_INIT && m_pInstance->m_nNetStatus == enmNetConnectSuccess )
	{
		//��������ַ
		char szBuffer[256];
		strcpy(szBuffer, "LicenceVarify.dll");
		m_pInstance->SendData(CMDID_SERVER_ADDR_REQ, szBuffer, sizeof(szBuffer), 0);
		m_pInstance->m_step = REQ_STEP_REQ_SERVER_ADDR;
		return;
	}

	if ( m_pInstance->m_step == REQ_STEP_SERVER_ADDR_RECVED )
	{
		m_pInstance->Connect( CTools_Ansi::ConvertdwIP(m_pInstance->m_addr.TCPIP), m_pInstance->m_addr.TCPPort);
		m_pInstance->m_step = REQ_STEP_CONNECT_TO_SERVER;
		return;
	}

	if ( m_pInstance->m_step == REQ_STEP_CONNECT_TO_SERVER && m_pInstance->m_nNetStatus == enmNetConnectSuccess)
	{
		m_pInstance->SendData(CMDID_LICENCE_LIST_REQ, NULL, 0, 0);
		m_pInstance->m_step = REQ_STEP_REQ_LICENCE_LIST;
		return;
	}*/

	if ( m_pInstance->m_step == REQ_STEP_LICENCE_LIST_RECVED)
	{
		KillTimer(NULL, TIMER_SERVER_ADDR_CHECK);
	}
}
void CTCPDataService::CallbackOntimeFunc(int UserData)
{
	if( m_pInstance == NULL)
		return;

	if( m_pInstance->GetNetStatus() == enmNetConnectSuccess) 
	{
		m_pInstance->SendData(CMDID_HEARTBEAT, NULL, 0);
	}	
}/*
 VOID CALLBACK CTCPDataService::OnTimer2(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
 {
 if( m_pInstance == NULL)
 return;

 m_pInstance->OnTimer_HeartBeat(uMsg, idEvent, dwTime);
 }*/

//��tcp�õ����ݺ󣬵��ô˻ص��������д���
void CTCPDataService::CallbackReceiveData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	if( m_pInstance == NULL || NULL == m_pInstance->m_pStreamCache || NULL == pData )
		return;

	//Stru_UniPkgHead * lpHead = (Stru_UniPkgHead*)pData;
	//if(lpHead->len == 0 && lpHead->cmdid == Cmd_QuotEvent_Push)
	//{
	//	OFFER_INFO("A zero len package come");
	//	return;
	//}
	////�ϸ��ж�
	//if(DataLen != sizeof(Stru_UniPkgHead) +lpHead->len + sizeof(TAIL) && lpHead->cmdid == Cmd_QuotEvent_Push )
	//{
	//	OFFER_INFO("DataLen not match %d ,headlen %d ",DataLen,lpHead->len);
	//	return;
	//}
	m_pInstance->m_pStreamCache->SaveDataTo(pData, DataLen);

	int nCacheLen = 0;
	while( (nCacheLen = m_pInstance->m_pStreamCache->GetLen()) >= sizeof(Stru_UniPkgHead) + sizeof(TAIL))
	{
		char szHeadBuf[sizeof(Stru_UniPkgHead)];
		memset(szHeadBuf, 0, sizeof(Stru_UniPkgHead));
		m_pInstance->m_pStreamCache->PeekFrom(szHeadBuf, sizeof(Stru_UniPkgHead));
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

			m_pInstance->m_pStreamCache->DeleteData(i);
			continue;
		}

		Stru_UniPkgHead pkgHead;
		m_pInstance->m_pStreamCache->PeekFrom(&pkgHead, sizeof(Stru_UniPkgHead));
		//�ȴ�һ��������
		if ( nCacheLen < (int)(sizeof(Stru_UniPkgHead) + pkgHead.len + sizeof(TAIL)) )
		{
			return;
		}

		//��顢����������
		if( pkgHead.cmdid == CMDID_HEARTBEATRSP 
			&& pkgHead.len == 0 )
		{
			Stru_Heartbeat heartBeatPkg;
			memset(&heartBeatPkg, 0, sizeof(heartBeatPkg));
			m_pInstance->m_pStreamCache->GetFrom(&heartBeatPkg, sizeof(Stru_Heartbeat));

			TAIL tail;
			m_pInstance->m_pStreamCache->GetFrom(&tail, sizeof(tail));

	//		OFFER_INFO("CMDID_HEARTBEATRSP \n");
			continue;
		}


		// ���Ϸ�������Ϣ������Ϣ���У���Work�̴߳���
		if ( VerifyRequest(pkgHead))
		{
			Stru_UniPkgHead head;
			m_pInstance->m_pStreamCache->GetFrom(&head, sizeof(Stru_UniPkgHead));
			//����ҵ�񷵻�����			
			if( (pkgHead.cmdid == Cmd_SubscribeQuotEvent_Rsp || pkgHead.cmdid == Cmd_QuotEvent_Push)
				&& pkgHead.len ==sizeof(PlatformStru_DepthMarketData))
			{//��������
				 if ( pkgHead.userdata1 == CF_ERROR_SUCCESS )
				 {
					PlatformStru_DepthMarketData depthData;
					ZeroMemory(&depthData, sizeof(depthData));
				//	memcpy(&depthData, pData+sizeof(Stru_UniPkgHead), sizeof(depthData));

					m_pInstance->m_pStreamCache->GetFrom(&depthData, sizeof(PlatformStru_DepthMarketData));

					::SendMessage(m_HwndParent, MESSAGE_QutoInTime, (WPARAM)(&depthData), NULL);
				 }
				//if ((depthData.SettlementPrice<0.00001 || depthData.SettlementPrice>10000) && depthData.SettlementPrice != util::GetDoubleInvalidValue())
				//{
				//	OFFER_INFO("���̽��յ�����������XXXXXXXXXXX,Instrument===%s, LastPrice===%f, SettlementPrice===%f, OpenPrice===%f\n",
				//		depthData.InstrumentID, depthData.LastPrice,depthData.SettlementPrice,depthData.OpenPrice);

					//PUSH_LOG_ERROR(FirstLevelError,true,true,"���̽��յ�����������XXXXXXXXXXX,Instrument===%s, LastPrice===%f, SettlementPrice===%f, OpenPrice===%f\n",
					//	 depthData.InstrumentID, depthData.LastPrice,depthData.SettlementPrice,depthData.OpenPrice);
				//}			
			}
		
		
		
			TAIL tail;
			m_pInstance->m_pStreamCache->GetFrom(&tail, sizeof(tail));
			continue;
		}


		//��鲢����������������һ��������������ǰ��ò�����������Ϊ��һ�������������
		if( nCacheLen >= (int)(sizeof(Stru_UniPkgHead) + pkgHead.len + sizeof(TAIL)) )
		{
			m_pInstance->m_pStreamCache->DeleteData( sizeof(Stru_UniPkgHead) + pkgHead.len + sizeof(TAIL) );
			continue;
		}

		break;
	}
}

//��ʼ����connect()�󣬵��ô˺���֪ͨӦ�ò�
void CTCPDataService::CallbackStartConnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	if( m_pInstance !=NULL ) 
	{
		m_pInstance->SetNetStatus(enmNetConnectWaiting);
	}
}

//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
void CTCPDataService::CallbackDisconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	if( m_pInstance !=NULL ) 
	{
		std::string strIP = CTools_Ansi::ConvertdwIP(SvrAddr.IP);
	//	OFFER_INFO("�Ͽ������������IP[%s], Port[%d]\n", strIP.c_str(),  SvrAddr.Port);
	//	PUSH_LOG_ERROR(FirstLevelError,true,true,"�Ͽ������������IP[%s], Port[%d]\n", strIP.c_str(),  SvrAddr.Port);
		m_pInstance->SetNetStatus(enmNetDisconnect);
		int n = 0;
		::SendMessage(m_HwndParent, MESSAGE_CONNECTFAIL, (WPARAM)&n, NULL);
	}
}

//���ӳɹ��󣬵��ô˻ص��������д���
void CTCPDataService::CallbackConnectSuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	if( m_pInstance !=NULL ) 
	{
		std::string strIP = CTools_Ansi::ConvertdwIP(SvrAddr.IP);
	//	OFFER_INFO("��������������ɹ���IP[%s], Port[%d]\n", strIP.c_str(),  SvrAddr.Port);
	//	PUSH_LOG_ERROR(Logger,true,true,"��������������ɹ���IP[%s], Port[%d]\n", strIP.c_str(),  SvrAddr.Port);
		m_pInstance->SetNetStatus(enmNetConnectSuccess);
		m_pInstance->m_pStreamCache->Empty();//��ջ�������������������
		int n = 0;
		::SendMessage(m_HwndParent, MESSAGE_CONNECTSUCCESS, (WPARAM)&n, NULL);
	
	}
}

//����ʧ�ܺ󣬵��ô˻ص��������д���
void CTCPDataService::CallbackConnectFail(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	if( m_pInstance !=NULL ) 
	{
		std::string strIP = CTools_Ansi::ConvertdwIP(SvrAddr.IP);
//		OFFER_INFO("�������������ʧ�ܣ�IP[%s], Port[%d]\n", strIP.c_str(),  SvrAddr.Port);
	//	PUSH_LOG_ERROR(FirstLevelError,true,true,"�������������ʧ�ܣ�IP[%s], Port[%d]\n", strIP.c_str(),  SvrAddr.Port);
		m_pInstance->SetNetStatus(enmNetDisconnect);
		int n = 0;
		::SendMessage(m_HwndParent, MESSAGE_CONNECTFAIL, (WPARAM)&n, NULL);
	}
}

CTCPDataService* CTCPDataService::GetInstance()
{
	if( m_pInstance == NULL ) 
	{
		g_QuotServerSeqID = 0;
		m_pInstance = new CTCPDataService();
	//	g_lTimeQuto = SetTimer(NULL,NULL,300000,(TIMERPROC)(NULL)); //ÿ5���Ӷ���һ������	
		
	}

	return m_pInstance;
}

void CTCPDataService::DestroyInstance()
{
	if( m_pInstance != NULL) 
	{
		KillTimer(NULL, TIMER_HEARTBEAT);
		Sleep(5);
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

CTCPDataService::CTCPDataService(void) 
: m_pTcpClient(NULL)
, m_pStreamCache(NULL)
, m_nNetStatus(enmNetNothing)
, m_step(REQ_STEP_INIT)
{
	memset(&m_addr, 0, sizeof(m_addr));
	m_licenceMap.clear();
	m_pStreamCache = new CMemoryCache(5*1024*1024);
}

CTCPDataService::~CTCPDataService(void)
{
	if( m_pTcpClient != NULL ) 
	{
		delete m_pTcpClient;
		m_pTcpClient = NULL;
	}

	if ( NULL != m_pStreamCache )
	{
		delete m_pStreamCache;
		m_pStreamCache = NULL;
	}

	m_nNetStatus = enmNetNothing;
}

bool CTCPDataService::Connect(std::string strIP, int nPort)
{
	if( m_pTcpClient != NULL ) 
	{
		delete m_pTcpClient;
		m_pTcpClient = NULL;
	}

	if ( NULL != m_pStreamCache )
	{
		m_pStreamCache->Empty();
	}

	Stru_IPAddress tIPAddr, tServAddr;
	ZeroMemory(&tIPAddr, sizeof(tIPAddr));
	ZeroMemory(&tServAddr, sizeof(tServAddr));

	tServAddr.IP = CTools_Ansi::ConvertStrIP((char*)strIP.c_str());
	tServAddr.Port = nPort;
	m_pTcpClient = new CTcpClientWorker(tIPAddr, tServAddr, 
		CallbackReceiveData, CallbackDisconnect, CallbackStartConnect, 
		CallbackConnectSuccess, CallbackConnectFail, CallbackOntimeFunc);
	SetNetStatus(enmNetConnectWaiting);

	return true;
}

bool CTCPDataService::SendData(UINT nCMDID, void* pData, int nLen, UINT seq)
{
	if( m_pTcpClient != NULL && m_nNetStatus == enmNetConnectSuccess) 
	{
		if ( pData == NULL && nLen != 0)
		{
			return false;
		}

		Stru_UniPkgHead head;
		head.moduleid = 0;
		head.cmdid = nCMDID;
		head.seq = seq;
		head.len = nLen;

		int nTotalLen = sizeof(Stru_UniPkgHead)+head.len+sizeof(TAIL);
		char* pBuf = new char[nTotalLen];
		memset(pBuf, 0, sizeof(nTotalLen));
		auto_ptr<char> ptrBuf( pBuf );
		if ( NULL == pBuf )
		{
			return false;
		}

		memcpy(pBuf, &head, sizeof(Stru_UniPkgHead));
		memcpy(pBuf+sizeof(Stru_UniPkgHead), pData, nLen);
		TAIL tail = TailID_UniPkg;
		memcpy(pBuf+sizeof(Stru_UniPkgHead)+head.len, &tail, sizeof(TAIL));

		int nLen = m_pTcpClient->SendData(pBuf, nTotalLen);
		if(nLen >0 && pData!= NULL)
		{
			//	OFFER_INFO("���̷��Ͷ���������� %s\n", pBuf+sizeof(Stru_UniPkgHead));
			//PUSH_LOG_ERROR(Logger,false,true,"���Ͷ���������� %s\n", pBuf+sizeof(Stru_UniPkgHead));

		}
		return true;
	}
	else 
	{
		return false;
	}
}
/*
void CTCPDataService::OnTimer_HeartBeat(UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
if( m_nNetStatus == enmNetConnectSuccess) 
{
SendData(CMDID_HEARTBEAT, NULL, 0);
}
}
*/
int CTCPDataService::GetPackageHeadTailLen()
{
	return (int)sizeof(Stru_UniPkgHead)+(int)sizeof(DWORD);
}

int CTCPDataService::GetPackageHeadLen()
{
	return (int)sizeof(Stru_UniPkgHead);
}

bool CTCPDataService::VerifyRequest( Stru_UniPkgHead& head )
{
	/*	switch(head.cmdid)
	{
	case CMDID_LICENCE_LIST_RSP:
	return head.len == sizeof(Stru_Licence);
	case CMDID_SERVER_ADDR_RSP:
	return head.len == sizeof(Stru_ServerAddr);
	case Cmd_QuotEvent_Push:
	return head.len == sizeof(PlatformStru_DepthMarketData);
	case Cmd_ReInitQuotServerEvent_Rsp:
	return true;
	case Cmd_QuotServerTimeResetEvent_Rsp:
	return true;
	case Cmd_QryQuotServerTimeEvent_Rsp:
	return true;  
	case Cmd_SubscribeQuotEvent_Rsp:
	return true;
	default:
	return false;
	}*/
	return true;
}

bool CTCPDataService::LicenceRecved()
{
	return m_step >= REQ_STEP_LICENCE_LIST_RECVED;
}

bool CTCPDataService::GetLicence( std::string strAppID, std::string& strLicence )
{
	std::map<std::string, std::string>::iterator it = m_licenceMap.find(strAppID);
	if ( it != m_licenceMap.end() )
	{
		strLicence = it->second;
		return true;
	}
	else
	{
		return false;
	}
}

void CTCPDataService::SetMessWnd(HWND  HwndParent)
{
	m_HwndParent = HwndParent;
}