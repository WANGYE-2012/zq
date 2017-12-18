// TcpServerWorker.cpp: implementation of the CTcpServerWorker class.
//
//////////////////////////////////////////////////////////////////////

#define FD_SETSIZE 10240		//fd_set�������С��ϵͳĬ��Ϊ64�������Ϊ10240��������windows.hǰ����


#include <windows.h>
#include "TcpServerWorker.h"
#include "tools_ansi.h"
#include "FileOpr.h"

#define LOGSEM 0

using namespace std;

#ifdef WIN32
#pragma unmanaged
#endif 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTcpServerWorker::CTcpServerWorker(  Stru_IPAddress BindAddr,int iMaxListen,
									 TCPSVRRCVDATAHANDLER pRecvFunc,TCPSVRACCEPTHANDLER pAcceptFunc,TCPSVRDISCONNHANDLER pDisconnFunc,
									 TCPSVRONTIME pOntimeFunc,
									 int SOSNDBUFSize,int SORCVBUFSize,
                                     bool bReuseAddr)
:CSocketBase(SOSNDBUFSize,SORCVBUFSize)
{
	DWORD threadId;

	m_Sem.lock();

    if(iMaxListen>1024) iMaxListen=1024;
	m_TmpRecvBufSize=65536*iMaxListen;
	m_pTmpRecvBuf=(char*)malloc(m_TmpRecvBufSize);

	m_hListenSocket=INVALID_SOCKET;
	m_BindAddr=BindAddr;
	m_BindIP_Real=0;
    m_bReuseAddr=bReuseAddr;
	m_MaxListen=iMaxListen;
	m_CreateListenSocketTime=0;
	m_ShutdownListenSocketTime=0;

	m_RecvFunc=pRecvFunc;
	m_AcceptFunc=pAcceptFunc;
	m_DisconnFunc=pDisconnFunc;
	m_OntimeFunc=pOntimeFunc;

    m_pRcvDataCBParam=(Stru_SocketRecvDataCallBackParam*)malloc(m_MaxListen*sizeof(Stru_SocketRecvDataCallBackParam));
    m_pAcceptCBParam=(Stru_SocketCallBackParam*)malloc(m_MaxListen*sizeof(Stru_SocketCallBackParam));
    m_pDisconnCBParam=(Stru_SocketCallBackParam*)malloc(m_MaxListen*sizeof(Stru_SocketCallBackParam));

	m_DataSocketMap.clear();

	//**************************************************
	//��鱾�������绷���Ƿ�ready�����û�У���ȴ���3���ӳ�ʱ
	//**************************************************
    time_t curtime=time(NULL);
	while(!m_LocalIP[0]&&time(NULL)-curtime<=180)
	{
		GetLocalMachineIP();
        if(m_LocalIP[0]) break;
        Sleep(1000);
	}


    CreateServerSocket_Internal();

	m_Sem.unlock();

	m_bSocketThreadNeedExit=false;
	m_hSocketThread=CreateThread(NULL,65536,(LPTHREAD_START_ROUTINE)SocketThreadWorker,this,0,&threadId);
    CFileOpr::getObj().writelocallog("threadid","TID:\t% 8u\tCTcpServerWorker::SocketThreadWorker",threadId);

	if(m_pWriteLog)
		m_pWriteLog->WriteLog_Fmt("CTcpServerWorker",WriteLogLevel_DEBUGINFO,"CTcpServerWorker(%s/%d,%d,...)",CTools_Ansi::ConvertdwIP(BindAddr.IP),BindAddr.Port,iMaxListen);
}

CTcpServerWorker::~CTcpServerWorker()
{

	m_bSocketThreadNeedExit=true;
	if(m_hSocketThread)
	{
		DWORD ExitCode;
		WaitForSingleObject((HANDLE)m_hSocketThread,8000);
		if(GetExitCodeThread((HANDLE)m_hSocketThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)m_hSocketThread,0);
		CloseHandle(m_hSocketThread);
		m_hSocketThread=NULL;
	}

	m_Sem.lock();

	OffLine_Internal();

	m_Sem.unlock();

	if(m_pTmpRecvBuf)
	{
		free(m_pTmpRecvBuf);
		m_pTmpRecvBuf=NULL;
	}
	if(m_pRcvDataCBParam)
	{
		free(m_pRcvDataCBParam);
		m_pRcvDataCBParam=NULL;
	}
	if(m_pAcceptCBParam)
	{
		free(m_pAcceptCBParam);
		m_pAcceptCBParam=NULL;
	}
	if(m_pDisconnCBParam)
	{
		free(m_pDisconnCBParam);
		m_pDisconnCBParam=NULL;
	}


	if(m_pWriteLog)
		m_pWriteLog->WriteLog_Fmt("CTcpServerWorker",WriteLogLevel_DEBUGINFO,"~CTcpServerWorker(), BindAddr=%s/%d\r\n\r\n\r\n",CTools_Ansi::ConvertdwIP(m_BindAddr.IP),m_BindAddr.Port);
}


/**************************************************************************************************//**
*   @brief					GetServerAddr  ��ȡ��������ַ
*   @param	N/A
*	@return					Tcp�󶨵ĵ�ַ
*******************************************************************************************************/
Stru_IPAddress CTcpServerWorker::GetServerAddr(void)
{
	return m_BindAddr;
}


/**************************************************************************************************//**
*   @brief					SendData	��������
*   @param	[in] pData		Ҫ���͵�����
*			[in] DataLen	Ҫ���͵����ݳ���
*			[in] RemoteAddr	���͵�Ŀ�ĵ�ַ����IP/Port��Ϊ0����������BindAddr������Socket����
*	@return					�ѷ��͵ĳ���
*******************************************************************************************************/
int CTcpServerWorker::SendData(char* pData,int DataLen,Stru_IPAddress RemoteAddr)
{
	int SendedLen=0,Len;
	std::map<SOCKET,Stru_IPAddress>::iterator it_Data;

	if(!pData) return 0;

	if(DataLen>CSocketBase::m_SOSNDBUFSize)
		DataLen=CSocketBase::m_SOSNDBUFSize;

	m_Sem.lock();

	for(it_Data=m_DataSocketMap.begin();it_Data!=m_DataSocketMap.end();it_Data++)
	{
		if(RemoteAddr.IP!=0&&
			RemoteAddr.Port!=0&&
			RemoteAddr!=it_Data->second)
			continue;

		Len=send(it_Data->first,pData,DataLen,0);
		if(SendedLen==0) Len=SendedLen;
	}

	m_Sem.unlock();

	return SendedLen;
}


/**************************************************************************************************//**
*   @brief					SendData	��������
*   @param	[in] pData		Ҫ���͵�����
*			[in] DataLen	Ҫ���͵����ݳ���
*			[in] hDataSocket���ڷ��͵�����socket���
*	@return					�ѷ��͵ĳ���
*******************************************************************************************************/
int CTcpServerWorker::SendData(char* pData,int DataLen,SOCKET hDataSocket)
{
	int SendedLen=0;
	std::map<SOCKET,Stru_IPAddress>::iterator it_Data;

	if(!pData) return 0;

	if(DataLen>CSocketBase::m_SOSNDBUFSize)
		DataLen=CSocketBase::m_SOSNDBUFSize;

	m_Sem.lock();

	it_Data=m_DataSocketMap.find(hDataSocket);
	if(it_Data!=m_DataSocketMap.end())
	{
		SendedLen=send(hDataSocket,pData,DataLen,0);
	}

	m_Sem.unlock();

	return SendedLen;
}


void CTcpServerWorker::OffLineOneDataSocket(SOCKET hDataSocket)
{
	m_Sem.lock();
	OffLineOneDataSocket_Internal(hDataSocket);
	m_Sem.unlock();
}

void CTcpServerWorker::OffLineOneDataSocket(Stru_IPAddress RemoteAddr)
{
	m_Sem.lock();
	OffLineOneDataSocket_Internal(RemoteAddr);
	m_Sem.unlock();
}

bool CTcpServerWorker::IsDataSocket(SOCKET hDataSocket)
{
    CeasymutexGuard guard(m_Sem);

	if(m_DataSocketMap.find(hDataSocket)==m_DataSocketMap.end()) return false;
    return true;
}

void CTcpServerWorker::GetAllDataSocket(vector<SOCKET>& AllDataSocket)
{
    CeasymutexGuard guard(m_Sem);

    AllDataSocket.clear();
    for(map<SOCKET,Stru_IPAddress>::const_iterator it=m_DataSocketMap.begin();it!=m_DataSocketMap.end();it++)
        AllDataSocket.push_back(it->first);
}

Stru_IPAddress CTcpServerWorker::GetBindAddr(void)
{
    return Stru_IPAddress(m_BindIP_Real,m_BindPort_Real);
}




void CTcpServerWorker::OffLine_Internal(void)
{
	std::map<SOCKET,Stru_IPAddress>::iterator it_Data;
	for(it_Data=m_DataSocketMap.begin();it_Data!=m_DataSocketMap.end();it_Data++)
	{
        for(int j=0;j<m_MaxListen;j++)
        {
            if(!m_pDisconnCBParam[j].bValid)
            {
                m_pDisconnCBParam[j].set(m_BindAddr,it_Data->second,it_Data->first);
                break;
            }
        }

		shutdown(it_Data->first,2);
		closesocket(it_Data->first);
	}
	m_DataSocketMap.clear();

	if(m_hListenSocket!=INVALID_SOCKET)
	{
		shutdown(m_hListenSocket,2);
		closesocket(m_hListenSocket);
		m_hListenSocket=INVALID_SOCKET;
		time(&m_ShutdownListenSocketTime);
	}

	if(m_pWriteLog)
		m_pWriteLog->WriteLog_Fmt("CTcpServerWorker",WriteLogLevel_DEBUGINFO,"OffLine_Internal()");
}


void CTcpServerWorker::OffLineOneDataSocket_Internal(SOCKET hDataSocket)
{
	std::map<SOCKET,Stru_IPAddress>::iterator it;

	it=m_DataSocketMap.find(hDataSocket);
	if(it!=m_DataSocketMap.end())
	{
        for(int j=0;j<m_MaxListen;j++)
        {
            if(!m_pDisconnCBParam[j].bValid)
            {
                m_pDisconnCBParam[j].set(m_BindAddr,it->second,it->first);
                break;
            }
        }

		if(m_pWriteLog)
			m_pWriteLog->WriteLog_Fmt("CTcpServerWorker",WriteLogLevel_DEBUGINFO,"OffLineOneDataSocket_Internal(%x),Addr=%s/%d",hDataSocket,CTools_Ansi::ConvertdwIP(it->second.IP),it->second.Port);

		shutdown(it->first,2);
		closesocket(it->first);
		
		m_DataSocketMap.erase(it);

	}
}


void CTcpServerWorker::OffLineOneDataSocket_Internal(Stru_IPAddress RemoteAddr)
{
	std::map<SOCKET,Stru_IPAddress>::iterator it,ittmp;

	for(it=m_DataSocketMap.begin();it!=m_DataSocketMap.end();)
	{
		if(it->second==RemoteAddr)
		{
            for(int j=0;j<m_MaxListen;j++)
            {
                if(!m_pDisconnCBParam[j].bValid)
                {
                    m_pDisconnCBParam[j].set(m_BindAddr,it->second,it->first);
                    break;
                }
            }

			if(m_pWriteLog)
				m_pWriteLog->WriteLog_Fmt("CTcpServerWorker",WriteLogLevel_DEBUGINFO,"OffLineOneDataSocket_Internal(%s/%d),hSocket=%x",CTools_Ansi::ConvertdwIP(it->second.IP),it->second.Port,it->first);

			shutdown(it->first,2);
			closesocket(it->first);

			ittmp=it;
			it++;
			m_DataSocketMap.erase(ittmp);
		}
		else 
		{
			it++;
		}
	}
}


/**************************************************************************************************//**
*   @brief		CreateServerSocket_Internal  ���tcp server �Ƿ�Ҫ����socket
*******************************************************************************************************/
void CTcpServerWorker::CreateServerSocket_Internal()
{
	SOCKADDR_IN		TmpAddr;
	int iRlt;

	if(m_BindAddr.IP!=0) m_BindIP_Real=m_BindAddr.IP;
	else if(m_LocalIP[0]!=0) m_BindIP_Real=m_LocalIP[0];
	else m_BindIP_Real=0;
    m_BindPort_Real=m_BindAddr.Port;

	if(m_BindIP_Real)
	{
		m_hListenSocket=CreateTcpSocket(); 
		if(m_pWriteLog)
			m_pWriteLog->WriteLog_Fmt("CTcpServerWorker",WriteLogLevel_DEBUGINFO,"CreateServerSocket_Internal - CreateTcpSocket()=%x",m_hListenSocket);

		if(m_hListenSocket!=INVALID_SOCKET)
		{
            //����SO_REUSEADDR, �������������ʱ��˿�δ�ͷŶ���ʧ��
            if(m_bReuseAddr)
            {
                int option=1;
                setsockopt ( m_hListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(option) );
            }

			TmpAddr.sin_family=AF_INET;
			TmpAddr.sin_addr.s_addr=m_BindIP_Real;
			TmpAddr.sin_port=htons(m_BindPort_Real);

			iRlt=bind(m_hListenSocket,(SOCKADDR*)&TmpAddr,sizeof(SOCKADDR_IN));
			if(m_pWriteLog)
				m_pWriteLog->WriteLog_Fmt("CTcpServerWorker",WriteLogLevel_DEBUGINFO,"CreateServerSocket_Internal - bind(%s/%d)=%d",CTools_Ansi::ConvertdwIP(m_BindIP_Real),m_BindAddr.Port,iRlt);

			if(iRlt!=SOCKET_ERROR)
			{
                //ȡ�����󶨵ĵ�ַ��������bind�˿�Ϊ0�����
                if(m_BindPort_Real==0)
                {
                    SOCKADDR_IN realaddr;
                    int realaddrlen=sizeof(SOCKADDR_IN);
                    int irlt2=getsockname(m_hListenSocket,(SOCKADDR*)&realaddr,&realaddrlen);
                    if(irlt2==0)
                        m_BindPort_Real=htons(realaddr.sin_port);
                }

                iRlt=listen(m_hListenSocket,m_MaxListen);
				if(m_pWriteLog)
					m_pWriteLog->WriteLog_Fmt("CTcpServerWorker",WriteLogLevel_DEBUGINFO,"CreateServerSocket_Internal - listen(%x,%d)=%d",m_hListenSocket,m_MaxListen,iRlt);
			}

			if(iRlt==SOCKET_ERROR)
			{
				shutdown(m_hListenSocket,2);
				closesocket(m_hListenSocket);
				m_hListenSocket=INVALID_SOCKET;
				if(m_pWriteLog)
					m_pWriteLog->WriteLog_Fmt("CTcpServerWorker",WriteLogLevel_DEBUGINFO,"bind/listen�����ͷ�hListenSocket");
			}
		}
		time(&m_CreateListenSocketTime);
	}
}


/**************************************************************************************************//**
*   @brief		GetSocketSet_Internal  ��ȡҪselect��socketset
*******************************************************************************************************/
void CTcpServerWorker::GetSocketSet_Internal(void* pReadSocketSet,void* pWriteSocketSet,void* pExceptSocketSet)
{
	fd_set& ReadSocketSet=*((fd_set*)pReadSocketSet);
	fd_set& WriteSocketSet=*((fd_set*)pWriteSocketSet);
	fd_set& ExceptSocketSet=*((fd_set*)pExceptSocketSet);

	memset(&ReadSocketSet,0,sizeof(ReadSocketSet));
	memset(&WriteSocketSet,0,sizeof(WriteSocketSet));
	memset(&ExceptSocketSet,0,sizeof(ExceptSocketSet));

	std::map<SOCKET,Stru_IPAddress>::iterator it_Data;

	if(m_hListenSocket!=INVALID_SOCKET)
	{
		ReadSocketSet.fd_array[ReadSocketSet.fd_count]=m_hListenSocket;
		ReadSocketSet.fd_count++;
		ExceptSocketSet.fd_array[ExceptSocketSet.fd_count]=m_hListenSocket;
		ExceptSocketSet.fd_count++;

		for(it_Data=m_DataSocketMap.begin();it_Data!=m_DataSocketMap.end();it_Data++)
		{
			if(it_Data->first!=INVALID_SOCKET)
			{
				ReadSocketSet.fd_array[ReadSocketSet.fd_count]=it_Data->first;
				ReadSocketSet.fd_count++;
				ExceptSocketSet.fd_array[ExceptSocketSet.fd_count]=it_Data->first;
				ExceptSocketSet.fd_count++;
			}
		}
	}
}

//**************************************************
//   @brief		�ͷ�һ���������ӵ�socket
//**************************************************
bool CTcpServerWorker::Check_TryReleaseOneDataSocket_Internal(SOCKET hDataSocket)
{
	std::map<SOCKET,Stru_IPAddress>::iterator it;

	it=m_DataSocketMap.find(hDataSocket);
	if(it!=m_DataSocketMap.end())
	{
		OffLineOneDataSocket_Internal(hDataSocket);
		return true;
	}
	else 
	{
		return false;
	}
}


//**************************************************
//   @brief		�ͷż���socket
//**************************************************
bool CTcpServerWorker::Check_TryReleaseOneListenSocket_Internal(SOCKET hListenSocket)
{
	if(hListenSocket==m_hListenSocket)
	{
		OffLine_Internal();
		return true;
	}
	else
	{
		return false;
	}
}


//**************************************************
//   @brief		����socket�������¼�
//**************************************************
void CTcpServerWorker::CheckSocketExcept_Internal(void* pExceptSocketSet)
{
	int i;
	SOCKET hSocket;

	fd_set& ExceptSocketSet=*((fd_set*)pExceptSocketSet);

	for(i=0;i<(int)ExceptSocketSet.fd_count;i++)
	{
		hSocket=ExceptSocketSet.fd_array[i];
		if(hSocket==INVALID_SOCKET)
			continue;
		
		//����ĳ������socket���쳣
		if(Check_TryReleaseOneDataSocket_Internal(hSocket))
			continue;

		//����ĳ������socket���쳣
		if(Check_TryReleaseOneListenSocket_Internal(hSocket))
			continue;
	}
}


//**************************************************
//   @brief		����Accept
//**************************************************
void CTcpServerWorker::TryAccept_Internal(void* pReadSocketSet)
{
	int i;
	SOCKET hSocket;
	SOCKET NewSocket;
	Stru_IPAddress RemoteAddr;

	if(m_hListenSocket==INVALID_SOCKET)
		return;

	fd_set& ReadSocketSet=*((fd_set*)pReadSocketSet);

	for(i=0;i<(int)ReadSocketSet.fd_count;i++)
	{
		hSocket=ReadSocketSet.fd_array[i];
		if(hSocket==m_hListenSocket)
		{
			//accept
			NewSocket=AcceptTcpSocket(hSocket,RemoteAddr.IP,RemoteAddr.Port);
			if(NewSocket!=INVALID_SOCKET)
			{
				m_DataSocketMap[NewSocket]=RemoteAddr;

                for(int j=0;j<m_MaxListen;j++)
                {
                    if(!m_pAcceptCBParam[j].bValid)
                    {
                        m_pAcceptCBParam[j].set(m_BindAddr,RemoteAddr,NewSocket);
                        break;
                    }
                }

				if(m_pWriteLog)
					m_pWriteLog->WriteLog_Fmt(	"TryAccept_Internal",
												WriteLogLevel_DEBUGINFO,
												"Acceptһ���������� (RemoteAddr=%s/%d)=%x",
												CTools_Ansi::ConvertdwIP(RemoteAddr.IP),RemoteAddr.Port,NewSocket);
			}
			break;
		}
	}
}


//**************************************************
//   @brief		����Read
//**************************************************
void CTcpServerWorker::TryRead_Internal(void* pReadSocketSet)
{
	int     i,ReadLen,ReadSize;
	SOCKET  hSocket;
	int	    ErrCode=0;

	if(!m_pTmpRecvBuf||!m_pRcvDataCBParam||!m_pAcceptCBParam||!m_pDisconnCBParam) return;

    int RecvBufLen=0;

	fd_set& ReadSocketSet=*((fd_set*)pReadSocketSet);

	for(i=0;i<(int)ReadSocketSet.fd_count;i++)
	{
		hSocket=ReadSocketSet.fd_array[i];
		if(hSocket==INVALID_SOCKET)
			continue;
		
		//�ҵ���Ӧ��TcpData
		std::map<SOCKET,Stru_IPAddress>::iterator it_Data;
		it_Data=m_DataSocketMap.find(hSocket);
		if(it_Data==m_DataSocketMap.end())
			continue;

		//��������
        ReadSize=m_TmpRecvBufSize-RecvBufLen;
        if(ReadSize<=0) continue;

		//���ReadSize>CSocketBase::m_SORCVBUFSize����Զ�˶Ͽ���recv������WSAENOBUFS����
		if(ReadSize>CSocketBase::m_SORCVBUFSize)
			ReadSize=CSocketBase::m_SORCVBUFSize;

        ReadLen=recv(hSocket,m_pTmpRecvBuf+RecvBufLen,ReadSize,0);
        if(ReadLen>0)
		{
            for(int j=0;j<m_MaxListen;j++)
            {
                if(!m_pRcvDataCBParam[j].bValid)
                {
                    m_pRcvDataCBParam[j].set(m_pTmpRecvBuf+RecvBufLen,ReadLen,m_BindAddr,it_Data->second,hSocket);
                    break;
                }
            }
            RecvBufLen+=ReadLen;
		}
		else if(ReadLen==0||
				ReadLen==SOCKET_ERROR&&
				((ErrCode=WSAGetLastError())==WSAECONNRESET||ErrCode==WSAENETDOWN||
				 ErrCode==WSAEFAULT||ErrCode==WSAENOTCONN||ErrCode==WSAENETRESET||
				 ErrCode==WSAENOTSOCK||ErrCode==WSAESHUTDOWN||ErrCode==WSAECONNABORTED||
				 ErrCode==WSAETIMEDOUT))
		{	//Զ�������������Ĺر���socket

			//��¼��־
			if(m_pWriteLog)
			{
				Stru_IPAddress& addr=it_Data->second;
				m_pWriteLog->WriteLog_Fmt(	"TryRead_Internal",
											WriteLogLevel_DEBUGINFO,
											"�������ӵ�Զ�������������Ĺرա��ͷŸ��������� (RemoteAddr=%s/%d hSocket=%x ReadLen=%d ErrCode=%d)",
											CTools_Ansi::ConvertdwIP(addr.IP),addr.Port,hSocket,ReadLen,ErrCode);
			}

			//��hDataSocket����
			OffLineOneDataSocket_Internal(hSocket);
		}
	}
}




DWORD CTcpServerWorker::SocketThreadWorker(void *arg)
{
	CTcpServerWorker *pTcpServer;
	fd_set			ReadSocketSet;
	fd_set			WriteSocketSet;
	fd_set			ExceptSocketSet;
    fd_set          BackupExceptSocketSet;
	timeval			WaitTime;
    HANDLE	        hSleepEvent=NULL;

	int				SocketCount;
	time_t			CurSecs;



	pTcpServer=(CTcpServerWorker*)arg;
	if(!pTcpServer) return 0;
	if(!pTcpServer->m_pTmpRecvBuf||!pTcpServer->m_pRcvDataCBParam||!pTcpServer->m_pAcceptCBParam||!pTcpServer->m_pDisconnCBParam) return 0;


	hSleepEvent=CreateEvent(NULL,FALSE,FALSE,NULL);

    for(int i=0;i<pTcpServer->m_MaxListen;i++)
    {
        pTcpServer->m_pRcvDataCBParam[i].reset();
        pTcpServer->m_pAcceptCBParam[i].reset();
        pTcpServer->m_pDisconnCBParam[i].reset();
    }


	//**************************************************
	//��ʼ���紦��ѭ��
	//**************************************************
	while(!pTcpServer->m_bSocketThreadNeedExit)
	{
		time(&CurSecs);


		pTcpServer->m_Sem.lock();


		//**************************************************
		//����Ҫselect��socket
		//**************************************************
		pTcpServer->GetSocketSet_Internal(&ReadSocketSet,&WriteSocketSet,&ExceptSocketSet);
		if(ReadSocketSet.fd_count==0&&WriteSocketSet.fd_count==0&&ExceptSocketSet.fd_count==0)
		{
			pTcpServer->m_Sem.unlock();
    		WaitForSingleObject(hSleepEvent,1000);
			continue;
		}

		pTcpServer->m_Sem.unlock();

        //��������socket���
        BackupExceptSocketSet=ExceptSocketSet;

		//**************************************************
		//ִ��select
		//**************************************************
		WaitTime.tv_sec=1;
		WaitTime.tv_usec=0;
		SocketCount=select(NULL,&ReadSocketSet,&WriteSocketSet,&ExceptSocketSet,&WaitTime);

		time(&CurSecs);

        if(SocketCount<0)
        {
            memset(&ExceptSocketSet,0,sizeof(ExceptSocketSet));

            int ErrNo=WSAGetLastError();
            if(ErrNo==WSAENOTSOCK)
            {
                //�зǷ��׽��֣���Ҫһ�������
	            fd_set			tmpExceptSocketSet;
                timeval			tmpWaitTime;
                tmpWaitTime.tv_sec=0;
                tmpWaitTime.tv_usec=0;
                for(int i=0;i<(int)BackupExceptSocketSet.fd_count;i++)
                {
                    tmpExceptSocketSet.fd_array[0]=BackupExceptSocketSet.fd_array[i];
                    tmpExceptSocketSet.fd_count=1;
                    if(select(NULL,NULL,NULL,&tmpExceptSocketSet,&tmpWaitTime)<0&&
                        WSAGetLastError()==WSAENOTSOCK)
                    {
                        //�ŵ�ExceptSocketSet��ȥ
				        ExceptSocketSet.fd_array[ExceptSocketSet.fd_count]=BackupExceptSocketSet.fd_array[i];
				        ExceptSocketSet.fd_count++;
                    }
                }
            }
        }

		pTcpServer->m_Sem.lock();

		//**************************************************
		//���������շ����������ӵ�
		//**************************************************
		if(SocketCount<0)
        {
            //�ر�ExceptSocketSet�е���Ч���
            pTcpServer->CheckSocketExcept_Internal(&ExceptSocketSet);
        }

		else if(SocketCount>0)
		{
			//**************************************************
			//����socket�������¼�
			//**************************************************
			pTcpServer->CheckSocketExcept_Internal(&ExceptSocketSet);

			//**************************************************
			//���ʹ������socket
			//**************************************************
			pTcpServer->TryAccept_Internal(&ReadSocketSet);

			//**************************************************
			//���ʹ�������socket
			//**************************************************
			pTcpServer->TryRead_Internal(&ReadSocketSet);

		}


		pTcpServer->m_Sem.unlock();

        for(int i=0;i<pTcpServer->m_MaxListen;i++)
        {
            if(pTcpServer->m_pRcvDataCBParam[i].bValid)
            {
                if(pTcpServer->m_RecvFunc)
                    pTcpServer->m_RecvFunc(pTcpServer->m_pRcvDataCBParam[i].pData,
                                            pTcpServer->m_pRcvDataCBParam[i].DataLen,
                                            pTcpServer->m_pRcvDataCBParam[i].LocalAddr,
                                            pTcpServer->m_pRcvDataCBParam[i].RemoteAddr,
                                            pTcpServer->m_pRcvDataCBParam[i].hSocket);
                pTcpServer->m_pRcvDataCBParam[i].reset();
            }
        }
        for(int i=0;i<pTcpServer->m_MaxListen;i++)
        {
            if(pTcpServer->m_pAcceptCBParam[i].bValid)
            {
                if(pTcpServer->m_AcceptFunc)
                    pTcpServer->m_AcceptFunc(pTcpServer->m_pAcceptCBParam[i].LocalAddr,
                                                pTcpServer->m_pAcceptCBParam[i].RemoteAddr,
                                                pTcpServer->m_pAcceptCBParam[i].hSocket);
                pTcpServer->m_pAcceptCBParam[i].reset();
            }
        }
        for(int i=0;i<pTcpServer->m_MaxListen;i++)
        {
            if(pTcpServer->m_pDisconnCBParam[i].bValid)
            {
                if(pTcpServer->m_DisconnFunc)
                    pTcpServer->m_DisconnFunc(pTcpServer->m_pDisconnCBParam[i].LocalAddr,
                                                pTcpServer->m_pDisconnCBParam[i].RemoteAddr,
                                                pTcpServer->m_pDisconnCBParam[i].hSocket);
                pTcpServer->m_pDisconnCBParam[i].reset();
            }
        }

		if(pTcpServer->m_OntimeFunc)
			pTcpServer->m_OntimeFunc();


	}	
	

    if(hSleepEvent)
    {
	    CloseHandle(hSleepEvent); 
        hSleepEvent=NULL;
    }
	
	return 0;
}



