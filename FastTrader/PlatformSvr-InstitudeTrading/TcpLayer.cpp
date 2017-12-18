// ConfigMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TcpLayer.h"
#include "FileOpr.h"
#include "Tools_Ansi.h"
#include "CommonPkg.h"
#include "TcpClientWorker.h"
#include <windows.h>

#include <string>
using namespace std;


//�ڲ�����
bool CTcpLayer::m_bStop=false;
Ceasymutex CTcpLayer::m_mutex;
void* CTcpLayer::m_pTcpClient=NULL;
char* CTcpLayer::m_pRcvBuf=NULL;
int CTcpLayer::m_RcvBufLen=0;
char* CTcpLayer::m_pSndBuf=NULL;
int CTcpLayer::m_SndBufLen=0;
set<CInstitudeApi*> CTcpLayer::m_setApi;
time_t CTcpLayer::m_HeartbeatTime=0;

string CTcpLayer::m_strSvrIP;
WORD CTcpLayer::m_wSvrPort = 0;

HANDLE CTcpLayer::m_hMainThread = NULL;
DWORD CTcpLayer::m_dwMainThreadID = 0;
UINT CTcpLayer::m_nRecvDataMSGID = ::RegisterWindowMessage("m_nRecvDataMSGID");

DWORD WINAPI CTcpLayer::Proc_RecvData(LPVOID lpParameter)
{
	m_mutex.lock();
	createtcpclient();
	m_mutex.unlock();

	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{
		if(Msg.message==m_nRecvDataMSGID) {
			ParseRecvPackage(Msg.wParam);
		}
	}
	return 0;
}

void CTcpLayer::Init()
{
	m_bStop = false;
	m_hMainThread = NULL;
	m_dwMainThreadID = 0;
	m_hMainThread = ::CreateThread(NULL, 65536, Proc_RecvData, NULL, 0, &m_dwMainThreadID);
}

void CTcpLayer::Release()
{
	m_mutex.lock();
	CTcpClientWorker* pTcpClnt = (CTcpClientWorker*)m_pTcpClient;
	CFileOpr::writelocallog("TcpLayer.log","Release��m_pTcpClient=%x ",
							m_pTcpClient);

	if(pTcpClnt)
	{
		delete pTcpClnt;
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
	::PostThreadMessage(m_dwMainThreadID, WM_QUIT, 0, NULL);
	::WaitForSingleObject(m_hMainThread, INFINITE);
	//::TerminateThread(m_dwMainThreadID, 0);
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

	//֪ͨת���߳������ݵ���
	if(!m_bStop) {
		::PostThreadMessage(m_dwMainThreadID, m_nRecvDataMSGID, scktDataRecv, NULL);
	}

}

//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
void CTcpLayer::DisconnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","DisconnCBFunc��localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);

	m_mutex.lock();
	if(!m_bStop) {
		::PostThreadMessage(m_dwMainThreadID, m_nRecvDataMSGID, scktDisConnect, NULL);
	}
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
	if(!m_bStop) {
		::PostThreadMessage(m_dwMainThreadID, m_nRecvDataMSGID, scktConnect, NULL);
	}
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

//����tcpclient
void CTcpLayer::createtcpclient(void)
{
	m_mutex.lock();
	if(!m_bStop&&!m_pTcpClient/* && CConfigManager::IsLoaded()*/)
	{
		string svrIP,localIP;
		DWORD dwsvrIP=0,dwlocalIP=0;
		unsigned short svrPort=0,localPort=0;

		svrIP = m_strSvrIP;
		svrPort = m_wSvrPort;
		dwsvrIP=CTools_Ansi::ConvertStrIP(svrIP.c_str());
		if(dwsvrIP!=0&&svrPort!=0)
		{
			//CConfigManager::GetLocalAddr(localIP,localPort);
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
	m_mutex.unlock();
}

bool CTcpLayer::SendData(UINT nCMDID, void* pData, int nLen, UINT seq, 
						 UINT userData1, UINT userData2, UINT userData3, UINT userData4)
{
	if(m_bStop) return false;
	Stru_UniPkgHead head;
	head.moduleid = 0;
	head.cmdid = nCMDID;
	head.seq = seq;
	head.len = nLen;
	head.userdata1 = userData1;
	head.userdata2 = userData2;
	head.userdata3 = userData3;
	head.userdata4 = userData4;
	
	int nTotalLen = sizeof(Stru_UniPkgHead)+head.len+4;
	char* pBuf = new char[nTotalLen];
	auto_ptr<char> ptrBuf( pBuf );
	if ( NULL == pBuf )
	{
		return false;
	}
	Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)pBuf;
	PkgHead.Set(nLen,nCMDID,0,seq, 0, userData1, userData2, userData3, userData4);

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
//ע��Api
void CTcpLayer::RegisterApi(CInstitudeApi* pApi)
{
	if(pApi==NULL)
		return;
	m_mutex.lock();
	m_setApi.insert(pApi);
	m_mutex.unlock();
}

//ע��Api
void CTcpLayer::UnRegisterApi(CInstitudeApi* pApi)
{
	m_mutex.lock();
	m_setApi.erase(pApi);
	m_mutex.unlock();
}

//ע������Api
void CTcpLayer::UnRegisterApiAll()
{
	m_mutex.lock();
	m_setApi.clear();
	m_mutex.unlock();
}

//�����߳��е��ã����ڽ������ַ��������ݰ���
//���յ����ݺ󣬻������̷߳���WM_COMMAND������߳��ٵ��ô˺������н����ͷַ���
//�����ڴ����߳��д������ݣ���Ƚϰ�ȫ
void CTcpLayer::ParseRecvPackage(int nMode)
{
	if(m_bStop) return;

	m_mutex.lock();

	if(!m_pRcvBuf) m_pRcvBuf=(char*)malloc(TcpRecvBufSize);

	switch(nMode) {
	case scktConnect:
	{
		set<CInstitudeApi*>::iterator it;
		for(it = m_setApi.begin(); it != m_setApi.end(); it++) {
			//�ַ����ݰ�������ģ�飬ע�⣬ģ����һ�����ܽ��г�ʱ�����������ᵼ��ϵͳ����
			//lParamΪconst char*ָ�룬ָ��Stru_UniPkgHead + PkgData
			if(*it != NULL) {
				(*it)->OnFrontConnected();
			}
		}
	}
		break;
	case scktDisConnect:
	{
		set<CInstitudeApi*>::iterator it;
		for(it = m_setApi.begin(); it != m_setApi.end(); it++) {
			//�ַ����ݰ�������ģ�飬ע�⣬ģ����һ�����ܽ��г�ʱ�����������ᵼ��ϵͳ����
			//lParamΪconst char*ָ�룬ָ��Stru_UniPkgHead + PkgData
			if(*it != NULL) {
				(*it)->OnFrontDisconnected();
			}
		}
	}
		break;
	case scktDataRecv:
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

				set<CInstitudeApi*>::iterator it;
				for(it = m_setApi.begin(); it != m_setApi.end(); it++) {
					//�ַ����ݰ�������ģ�飬ע�⣬ģ����һ�����ܽ��г�ʱ�����������ᵼ��ϵͳ����
					//lParamΪconst char*ָ�룬ָ��Stru_UniPkgHead + PkgData
					if(*it != NULL) {
						(*it)->ParseData(m_pRcvBuf+off+sizeof(Stru_UniPkgHead), 
								PkgHead.len, PkgHead.cmdid, PkgHead.moduleid, 
								PkgHead.seq, PkgHead.subseq, PkgHead.userdata1, PkgHead.userdata2, 
								PkgHead.userdata3, PkgHead.userdata4);
					}
				}

				off+=LengthUsedByPkg;
			}
			if(off>0)
			{
				memmove(m_pRcvBuf,m_pRcvBuf+off,m_RcvBufLen-off);
				m_RcvBufLen-=off;
			}
		}
		break;
	};

	m_mutex.unlock();
}
