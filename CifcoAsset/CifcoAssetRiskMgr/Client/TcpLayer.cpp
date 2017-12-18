// ConfigMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
//#include <windows.h>
#include "ConfigMgr.h"
#include "TcpLayer.h"
#include "FileOpr.h"
#include "Tools_Ansi.h"
#include "CommonPkg.h"
#include "RiskManageCmd.h"
#include "TcpClientWorker.h"
#include <windows.h>

#include <string>
using namespace std;



#ifdef _DEBUG
	#pragma comment(lib, "Tools4clrD.lib")
#else
	#pragma comment(lib, "Tools4clr.lib")
#endif

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")


#pragma unmanaged


//�ڲ�����
bool CTcpLayer::m_bStop=false;
Ceasymutex CTcpLayer::m_mutex;
void* CTcpLayer::m_pTcpClient=NULL;
char* CTcpLayer::m_pRcvBuf=NULL;
int CTcpLayer::m_RcvBufLen=0;
char* CTcpLayer::m_pSndBuf=NULL;
int CTcpLayer::m_SndBufLen=0;
int CTcpLayer::m_hMainWnd=NULL;
multimap<unsigned int,int> CTcpLayer::m_Cmd2hWnd;
time_t CTcpLayer::m_HeartbeatTime=0;


void CTcpLayer::Init()
{
	m_mutex.lock();

	createtcpclient();

	m_mutex.unlock();
}

void CTcpLayer::Release()
{
	m_mutex.lock();
	CFileOpr::writelocallog("TcpLayer.log","Release��m_pTcpClient=%x ",
							m_pTcpClient);

	if(m_pTcpClient)
	{
		delete m_pTcpClient;
		m_pTcpClient=NULL;
	}
	m_mutex.unlock();
}

//ϵͳ�˳�ǰ����Stop��ֹͣ�ַ����ٹرո�ģ�鴰�ڡ�
void CTcpLayer::Stop()
{
	m_mutex.lock();
	m_bStop=true;
	m_mutex.unlock();
}


//��tcp�õ����ݺ󣬵��ô˻ص��������д���
void CTcpLayer::RecvDataCBFunc(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	if(m_bStop) return;

	m_mutex.lock();
	if(!m_pRcvBuf) m_pRcvBuf=(char*)malloc(TcpRecvBufSize);
	if(!m_pRcvBuf||m_RcvBufLen+DataLen>TcpRecvBufSize)
	{
		CFileOpr::writelocallog("TcpLayer.log","RecvDataCBFunc���������㣬m_pRcvBuf=%x m_RcvBufLen=%d DataLen=%d TcpRecvBufSize=%d localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
								m_pRcvBuf,m_RcvBufLen,DataLen,TcpRecvBufSize,LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
	}
	else
	{
		memcpy(m_pRcvBuf+m_RcvBufLen,pData,DataLen);
		m_RcvBufLen+=DataLen;
	}
	m_mutex.unlock();

	//֪ͨ�����������ݵ���
	if(!m_bStop&&m_hMainWnd!=NULL&&IsWindow((HWND)m_hMainWnd))
	{
		PostMessage((HWND)m_hMainWnd,WM_COMMAND_Win,WndCmd_DataArrival,0L);
	}

}

//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
void CTcpLayer::DisconnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","DisconnCBFunc��localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);

	m_mutex.lock();
	m_mutex.unlock();
}

//��ʼ����connect()�󣬵��ô˺���֪ͨӦ�ò�
void CTcpLayer::StartConnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","StartConnCBFunc��localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
	m_mutex.lock();
	m_mutex.unlock();
}

//���ӳɹ��󣬵��ô˻ص��������д���
void CTcpLayer::ConnSuccCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","StartConnCBFunc��localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
	m_mutex.lock();
	m_mutex.unlock();
}

//����ʧ�ܺ󣬵��ô˻ص��������д���
void CTcpLayer::ConnFailCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","ConnFailCBFunc��localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);

	m_mutex.lock();
	m_mutex.unlock();
}

//���ڵ��õĻص�������һ��ÿ������һ�Σ��������ڼ��������ʱ��
void CTcpLayer::OnTimeCBFunc(void)
{
	m_mutex.lock();

	//������δ���͵�����
	if(!m_bStop)
		senddata_internal();

	time_t cursecs=time(NULL);
	if(cursecs-m_HeartbeatTime>=3&&
		m_pTcpClient&&
		((CTcpClientWorker*)m_pTcpClient)->getSocket()>0&&
		((CTcpClientWorker*)m_pTcpClient)->getConnStatus()==TcpClientConnStatus_Connected)
	{
		//ÿ3�뷢������
		char buf[sizeof(Stru_UniPkgHead)+4];
		Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)buf;
		PkgHead.Set(0,CMDID_HEARTBEAT,0,0);
		*(unsigned int*)(buf+sizeof(PkgHead))=TailID_UniPkg;

		Senddata(buf,sizeof(buf));
		m_HeartbeatTime=cursecs;
	}


	m_mutex.unlock();
}
//�ж��Ƿ�Ͽ�
bool CTcpLayer::IsDisconnected()
{
	return ((CTcpClientWorker*)m_pTcpClient)->getConnStatus()== TcpClientConnStatus_Disconnected;
}
//����tcpclient
void CTcpLayer::createtcpclient(void)
{
	m_mutex.lock();
	if(!m_bStop&&!m_pTcpClient && CConfigManager::IsLoaded())
	{
		string svrIP,localIP;
		DWORD dwsvrIP,dwlocalIP;
		unsigned short svrPort,localPort;

		if(CConfigManager::GetSvrAddr(svrIP,svrPort))
		{
			dwsvrIP=CTools_Ansi::ConvertStrIP(svrIP.c_str());
			if(dwsvrIP!=0&&svrPort!=0)
			{
				CConfigManager::GetLocalAddr(localIP,localPort);
				dwlocalIP=CTools_Ansi::ConvertStrIP(localIP.c_str());

				m_pTcpClient=new CTcpClientWorker(Stru_IPAddress(dwlocalIP,localPort),Stru_IPAddress(dwsvrIP,svrPort),
					 RecvDataCBFunc,
					 DisconnCBFunc,
					 StartConnCBFunc,
					 ConnSuccCBFunc,
					 ConnFailCBFunc,
					 OnTimeCBFunc,
					 SocketBufSize,SocketBufSize);

				if(Debug_TcpLayer||m_pTcpClient)
					CFileOpr::writelocallog("TcpLayer.log","����TcpClient��localaddr=%s(%u):%u svraddr=%s(%u):%u, m_pTcpClient=%x",
											localIP,dwlocalIP,localPort,svrIP,dwsvrIP,svrPort,m_pTcpClient);
			}
		}
	}
	m_mutex.unlock();
}

bool CTcpLayer::SendData(UINT nCMDID, void* pData, int nLen, UINT seq)
{
	if(m_bStop) return false;
	Stru_UniPkgHead head;
	head.moduleid = 0;
	head.cmdid = nCMDID;
	head.seq = seq;
	head.len = nLen;

	
	int nTotalLen = sizeof(Stru_UniPkgHead)+head.len+4;
	char* pBuf = new char[nTotalLen];
	auto_ptr<char> ptrBuf( pBuf );
	if ( NULL == pBuf )
	{
		return false;
	}
	Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)pBuf;
	PkgHead.Set(nLen,nCMDID,0,seq);

	if(pData)
		memcpy(pBuf+sizeof(Stru_UniPkgHead),pData,nLen);
	*(unsigned int*)(pBuf+sizeof(PkgHead) + nLen)=TailID_UniPkg;

	return CTcpLayer::Senddata(pBuf,nTotalLen);
}

//��������
bool CTcpLayer::Senddata(const char* pData,int DataLen)
{
	if(m_bStop) return false;

	bool brlt=false;
	m_mutex.lock();
	if(!m_pSndBuf) m_pSndBuf=(char*)malloc(TcpSendBufSize);
	if(!m_pSndBuf||m_SndBufLen+DataLen>TcpSendBufSize)
	{
		CFileOpr::writelocallog("TcpLayer.log","senddata���������㣬m_pSndBuf=%x m_SndBufLen=%d DataLen=%d TcpSendBufSize=%d m_pTcpClient=%x hSocket=%x ConnStatus=%d",
			m_pSndBuf,m_SndBufLen,DataLen,TcpSendBufSize,m_pTcpClient,
			m_pTcpClient?((CTcpClientWorker*)m_pTcpClient)->getSocket():-1,
			m_pTcpClient?((CTcpClientWorker*)m_pTcpClient)->getConnStatus():0);
		brlt=false;
	}
	else
	{
		memcpy(m_pSndBuf+m_SndBufLen,pData,DataLen);
		m_SndBufLen+=DataLen;
		brlt=true;

		senddata_internal();
	}
	m_mutex.unlock();
	return brlt;
}


//��������
void CTcpLayer::senddata_internal(void)
{
	if(m_pSndBuf&&
		!m_bStop&&m_SndBufLen>0&&
		m_pTcpClient&&
		((CTcpClientWorker*)m_pTcpClient)->getSocket()>0&&
		((CTcpClientWorker*)m_pTcpClient)->getConnStatus()==TcpClientConnStatus_Connected)
	{
		int sentlen=((CTcpClientWorker*)m_pTcpClient)->SendData(m_pSndBuf,m_SndBufLen);
		if(sentlen>0&&sentlen<=m_SndBufLen)
		{
			memmove(m_pSndBuf,m_pSndBuf+sentlen,m_SndBufLen-sentlen);
			m_SndBufLen-=sentlen;
		}
	}
}

//���������ھ��
void CTcpLayer::SetMainWndHandle(int hMainWnd)
{
	m_mutex.lock();
	m_hMainWnd=hMainWnd;
	m_mutex.unlock();
}

//ģ�鶩����Ϣ����ָ����ϢID��ģ�鴰�ڵľ��
void CTcpLayer::SubscribePkg(unsigned int CmdID,int hModuleWnd)
{
	m_mutex.lock();
	multimap<unsigned int,int>::iterator itlower=m_Cmd2hWnd.lower_bound(CmdID);
	multimap<unsigned int,int>::iterator itupper=m_Cmd2hWnd.upper_bound(CmdID);
	while(itlower!=itupper&&itlower->second!=hModuleWnd) itlower++;
	if(itlower==itupper)
		m_Cmd2hWnd.insert(pair<unsigned int,int>(CmdID,hModuleWnd));
	m_mutex.unlock();
}

//ģ���˶���Ϣ����ָ����ϢID��ģ�鴰�ڵľ��
void CTcpLayer::UnsubscribePkg(unsigned int CmdID,int hModuleWnd)
{
	m_mutex.lock();
	multimap<unsigned int,int>::iterator itlower=m_Cmd2hWnd.lower_bound(CmdID);
	multimap<unsigned int,int>::iterator itupper=m_Cmd2hWnd.upper_bound(CmdID);
	while(itlower!=itupper&&itlower->second!=hModuleWnd) itlower++;
	if(itlower!=itupper)
		m_Cmd2hWnd.erase(itlower);
	m_mutex.unlock();
}
//ģ���˶�ȫ����Ϣ����ָ����ϢID��ģ�鴰�ڵľ��
void CTcpLayer::UnsubscribeAllPkg(int hModuleWnd)
{
	m_mutex.lock();
	multimap<unsigned int,int>::iterator it=m_Cmd2hWnd.begin();
	while(it!=m_Cmd2hWnd.end())
	{
		if(it->second==hModuleWnd)
			it=m_Cmd2hWnd.erase(it);
		else it++;
	}
	m_mutex.unlock();
}


//�����߳��е��ã����ڽ������ַ��������ݰ���
//���յ����ݺ󣬻������̷߳���WM_COMMAND������߳��ٵ��ô˺������н����ͷַ���
//�����ڴ����߳��д������ݣ���Ƚϰ�ȫ
void CTcpLayer::ParseRecvPackageInWindowThread(void)
{
	if(m_bStop) return;

	m_mutex.lock();

	if(!m_pRcvBuf) m_pRcvBuf=(char*)malloc(TcpRecvBufSize);

	if(m_pRcvBuf)
	{
		//���������ܳ��ֵ���Ч����
		int off=0;
		while(off<m_RcvBufLen&&Stru_UniPkgHead::IsInvalidPkgHead(m_pRcvBuf+off,m_RcvBufLen-off)) off++;

		//ѭ���������յ�������
		int LengthUsedByPkg;
		while(Stru_UniPkgHead::IsValidPkg(m_pRcvBuf+off,m_RcvBufLen-off,&LengthUsedByPkg))
		{
			Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)(m_pRcvBuf+off);

			multimap<unsigned int,int>::iterator itlower=m_Cmd2hWnd.lower_bound(PkgHead.cmdid);
			multimap<unsigned int,int>::iterator itupper=m_Cmd2hWnd.upper_bound(PkgHead.cmdid);
			while(itlower!=itupper)
			{
				//�ַ����ݰ�������ģ�飬ע�⣬ģ����һ�����ܽ��г�ʱ�����������ᵼ��ϵͳ����
				//lParamΪconst char*ָ�룬ָ��Stru_UniPkgHead + PkgData
				if(IsWindow((HWND)(itlower->second)))
					SendMessage((HWND)(itlower->second),WM_COMMAND_Win,WndCmd_YourPkgArrival,(LPARAM)(m_pRcvBuf+off));

				itlower++;
			}

			off+=LengthUsedByPkg;
		}
		if(off>0)
		{
			memmove(m_pRcvBuf,m_pRcvBuf+off,m_RcvBufLen-off);
			m_RcvBufLen-=off;
		}
	}

	m_mutex.unlock();
}
