// ConfigMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ConfigMgr.h"
#include "TcpDataConnect.h"
#include "FileOpr.h"
#include "Tools_Ansi.h"
#include "CommonPkg.h"
#include "TcpClientWorker.h"
#include <windows.h>

#include <string>
using namespace std;




#pragma unmanaged


//�ڲ�����
bool CTcpDataConnect::m_bStop=false;
Ceasymutex CTcpDataConnect::m_mutex;
void* CTcpDataConnect::m_pTcpClient=NULL;
char* CTcpDataConnect::m_pRcvBuf=NULL;
int CTcpDataConnect::m_RcvBufLen=0;
char* CTcpDataConnect::m_pSndBuf=NULL;
int CTcpDataConnect::m_SndBufLen=0;
int CTcpDataConnect::m_hMainWnd=NULL;
multimap<unsigned int,int> CTcpDataConnect::m_Cmd2hWnd;
time_t CTcpDataConnect::m_HeartbeatTime=0;
bool CTcpDataConnect::m_bConnected=false;
bool CTcpDataConnect::m_bDisConnected=false;


void CTcpDataConnect::Init()
{
    m_mutex.lock();

	createtcpclient();

	m_mutex.unlock();
}

void CTcpDataConnect::Release()
{
	m_mutex.lock();
	CFileOpr::getObj().writelocallog("TcpDataConnect","Release��m_pTcpClient=%x ",
							m_pTcpClient);

	if(m_pTcpClient)
	{
		delete (CTcpClientWorker*)m_pTcpClient;
		m_pTcpClient=NULL;
	}
	m_mutex.unlock();
}

//ϵͳ�˳�ǰ����Stop��ֹͣ�ַ����ٹرո�ģ�鴰�ڡ�
void CTcpDataConnect::Stop()
{
    m_mutex.lock();
    m_bStop=true;
    m_mutex.unlock();
}


//��tcp�õ����ݺ󣬵��ô˻ص��������д���
void CTcpDataConnect::RecvDataCBFunc(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
    if(m_bStop) return;

    m_mutex.lock();
    if(!m_pRcvBuf) m_pRcvBuf=(char*)malloc(TcpRecvBufSize_Quot);
    if(!m_pRcvBuf||m_RcvBufLen+DataLen>TcpRecvBufSize_Quot)
    {
        CFileOpr::getObj().writelocallog("TcpDataConnect","RecvDataCBFunc���������㣬m_pRcvBuf=%x m_RcvBufLen=%d DataLen=%d TcpRecvBufSize_Quot=%d localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
            m_pRcvBuf,m_RcvBufLen,DataLen,TcpRecvBufSize_Quot,LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
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
        PostMessage((HWND)m_hMainWnd,WM_USER_Win_Quot,WndUserCmd_DataArrival_Quot,0L);
    }

}

//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
void CTcpDataConnect::DisconnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
    CFileOpr::getObj().writelocallog("TcpDataConnect","DisconnCBFunc��localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
        LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);

	m_bConnected=false;
	m_bDisConnected=true;

	//֪ͨ���������ӶϿ�(lParam=0)
	if(!m_bStop&&m_hMainWnd!=NULL&&IsWindow((HWND)m_hMainWnd))
	{
		PostMessage((HWND)m_hMainWnd,WM_USER_Win_Quot,WndUserCmd_QuotConnStatus_Quot,0L);
	}
}

//��ʼ����connect()�󣬵��ô˺���֪ͨӦ�ò�
void CTcpDataConnect::StartConnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
    CFileOpr::getObj().writelocallog("TcpDataConnect","StartConnCBFunc��localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
        LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);

	//֪ͨ���������ӿ�ʼ(lParam=1)
	if(!m_bStop&&m_hMainWnd!=NULL&&IsWindow((HWND)m_hMainWnd))
	{
		PostMessage((HWND)m_hMainWnd,WM_USER_Win_Quot,WndUserCmd_QuotConnStatus_Quot,1L);
	}
}

//���ӳɹ��󣬵��ô˻ص��������д���
void CTcpDataConnect::ConnSuccCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
    CFileOpr::getObj().writelocallog("TcpDataConnect","StartConnCBFunc��localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
        LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
	m_bConnected=true;
	m_bDisConnected=false;
	//֪ͨ���������ӳɹ�(lParam=2)
	if(!m_bStop&&m_hMainWnd!=NULL&&IsWindow((HWND)m_hMainWnd))
	{
		PostMessage((HWND)m_hMainWnd,WM_USER_Win_Quot,WndUserCmd_QuotConnStatus_Quot,2L);
	}
}

//����ʧ�ܺ󣬵��ô˻ص��������д���
void CTcpDataConnect::ConnFailCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
    CFileOpr::getObj().writelocallog("TcpDataConnect","ConnFailCBFunc��localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
        LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);

	m_bConnected=false;

	//֪ͨ����������ʧ��(lParam=3)
	if(!m_bStop&&m_hMainWnd!=NULL&&IsWindow((HWND)m_hMainWnd))
	{
		PostMessage((HWND)m_hMainWnd,WM_USER_Win_Quot,WndUserCmd_QuotConnStatus_Quot,3L);
	}
}

//���ڵ��õĻص�������һ��ÿ������һ�Σ��������ڼ��������ʱ��
void CTcpDataConnect::OnTimeCBFunc(int UserData)
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
void CTcpDataConnect::createtcpclient(void)
{
	m_mutex.lock();
	if(!m_bStop&&!m_pTcpClient && CConfigManager::IsLoaded())
	{
		string svrIP,localIP;
		DWORD dwsvrIP,dwlocalIP;
		unsigned short svrPort,localPort;

		if(CConfigManager::GetQuotSvrAddr(svrIP,svrPort))
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
					 SocketBufSize_Quot,
                     SocketBufSize_Quot,
                     0);

				if(Debug_TcpDataConnect||m_pTcpClient)
					CFileOpr::getObj().writelocallog("TcpDataConnect","����TcpClient��localaddr=%s(%u):%u svraddr=%s(%u):%u, m_pTcpClient=%x",
											localIP.c_str(),dwlocalIP,localPort,svrIP.c_str(),dwsvrIP,svrPort,m_pTcpClient);
			}
		}
	}
	m_mutex.unlock();
}

//�ж��Ƿ�������״̬
bool CTcpDataConnect::IsConnected()
{
	return m_bConnected;
}

bool CTcpDataConnect::SendData(UINT nCMDID, void* pData, int nLen, UINT seq, unsigned int userdata1, unsigned int userdata2, unsigned int userdata3, unsigned int userdata4)
{
	if(m_bDisConnected) return false;
	if(m_bStop) return false;
	Stru_UniPkgHead head;
	head.moduleid = 0;
	head.cmdid = nCMDID;
	head.seq = seq;
	head.len = nLen;
	head.userdata1=userdata1;
	head.userdata2=userdata2;
	head.userdata3=userdata3;
	head.userdata4=userdata4;

	
	int nTotalLen = sizeof(Stru_UniPkgHead)+head.len+4;
	char* pBuf = new char[nTotalLen];
	auto_ptr<char> ptrBuf( pBuf );
	if ( NULL == pBuf )
	{
		return false;
	}
	Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)pBuf;
	PkgHead.Set(nLen,nCMDID,0,seq,0,userdata1,userdata2,userdata3,userdata4);

	if(pData)
		memcpy(pBuf+sizeof(Stru_UniPkgHead),pData,nLen);
	*(unsigned int*)(pBuf+sizeof(PkgHead) + nLen)=TailID_UniPkg;

	return CTcpDataConnect::Senddata(pBuf,nTotalLen);
}

//��������
bool CTcpDataConnect::Senddata(const char* pData,int DataLen)
{
	if(m_bDisConnected) return false;
	if(m_bStop) return false;

	bool brlt=false;
	m_mutex.lock();
	if(!m_pSndBuf) m_pSndBuf=(char*)malloc(TcpSendBufSize_Quot);
	if(!m_pSndBuf||m_SndBufLen+DataLen>TcpSendBufSize_Quot)
	{
		CFileOpr::getObj().writelocallog("TcpLayer","senddata���������㣬m_pSndBuf=%x m_SndBufLen=%d DataLen=%d TcpSendBufSize_Quot=%d m_pTcpClient=%x hSocket=%x ConnStatus=%d",
			m_pSndBuf,m_SndBufLen,DataLen,TcpSendBufSize_Quot,m_pTcpClient,
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
void CTcpDataConnect::senddata_internal(void)
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
void CTcpDataConnect::SetMainWndHandle(int hMainWnd)
{
    m_mutex.lock();
    m_hMainWnd=hMainWnd;
    m_mutex.unlock();
}

//ģ�鶩����Ϣ����ָ����ϢID��ģ�鴰�ڵľ��
void CTcpDataConnect::SubscribePkg(unsigned int CmdID,int hModuleWnd)
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
void CTcpDataConnect::UnsubscribePkg(unsigned int CmdID,int hModuleWnd)
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
void CTcpDataConnect::UnsubscribeAllPkg(int hModuleWnd)
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
//���յ����ݺ󣬻������̷߳���WM_USER_Win������߳��ٵ��ô˺������н����ͷַ���
//�����ڴ����߳��д������ݣ���Ƚϰ�ȫ
//****ע�⣺WndUserCmd_YourPkgArrival��Ϣ�Ľ��մ�����̲��ܱ���������MessageBox()�ĵ��õȡ�����������ᵼ��ϵͳ���������
//****(��Ϊ��������Ϣ��������롣�ڽ��մ��ڵ���Ϣ��������������£���������ڵ���Ϣ���л�����Ϣ���������̻߳ᱣ�浱ǰ�����������������ݣ�Ȼ���ٴε�����Ϣ�������������롣(�ƺ���winform�Ļ���)�������ᵼ��m_pRcvBuf��ͻ������
void CTcpDataConnect::ParseRecvPackageInWindowThread(void)
{
    if(m_bStop) return;

    m_mutex.lock();

    if(!m_pRcvBuf) m_pRcvBuf=(char*)malloc(TcpRecvBufSize_Quot);

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
				//****ע�⣺WndUserCmd_YourPkgArrival��Ϣ�Ľ��մ�����̲��ܱ���������MessageBox()�ĵ��õ�
                if(IsWindow((HWND)(itlower->second)))
                    SendMessage((HWND)(itlower->second),WM_USER_Win_Quot,WndUserCmd_YourPkgArrival_Quot,(LPARAM)(m_pRcvBuf+off));

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
