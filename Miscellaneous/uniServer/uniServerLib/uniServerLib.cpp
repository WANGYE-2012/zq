#pragma warning(disable : 4996)
#pragma warning(disable : 4786)


#include <windows.h>


#include "TcpServerWorker.h"
#include "UdpSimpleWorker.h"
#include "TcpClientWorker.h"
#include "WinService.h"
#include "WriteLog.h"
#include "IniFile.h"
#include "Tools_Ansi.h"

#include <vector>
using namespace std;

#ifdef _DEBUG
#pragma comment(lib, "ToolsD.lib")											
#else 
#pragma comment(lib, "Tools.lib")											
#endif


typedef	void 	(*Init_uniServerCBFunc)(void);	
typedef	void 	(*Release_uniServerCBFunc)(void);
typedef bool	(*IsInitCompleted_uniServerCBFunc)(int& InitPercent,char* InitMsg,int InitMsgSize);
typedef void	(*NotifyEntireSystemInitCompleted_uniServerCBFunc)(void);


char g_MainPath[MAX_PATH];
CWriteLog *g_pWriteLog=NULL;

struct Stru_Function
{
    Stru_Function()
    {
        memset(this,0,sizeof(*this));

		Addr_TcpSvr.Port=-1;
		Addr_UdpSvr.Port=-1;
		for(int i=0;i<(int)Tools_Ansi_ArraySize(pTcpClient);i++) 
			SvrAddr_TcpClient[i].Port=-1;
    };

    void LoadDLL(char* pMainPath)
    {
        if(!pMainPath) return;

        char filename[512];
        sprintf(filename,"%s%s",pMainPath,DllName);
        hLib=LoadLibrary(filename);
        if(!hLib) return;

        InitFunc=(Init_uniServerCBFunc)GetProcAddress(hLib,"InitFunc");
        ReleaseFunc=(Release_uniServerCBFunc)GetProcAddress(hLib,"ReleaseFunc");
		IsInitCompletedFunc=(IsInitCompleted_uniServerCBFunc)GetProcAddress(hLib,"IsInitCompletedFunc");
		NotifyEntireSystemInitCompletedFunc=(NotifyEntireSystemInitCompleted_uniServerCBFunc)GetProcAddress(hLib,"NotifyEntireSystemInitCompletedFunc");

        TcpSvrRecvFunc=(TCPSVRRCVDATAHANDLER)GetProcAddress(hLib,"TcpSvrRecvFunc");
        TcpSvrAcceptFunc=(TCPSVRACCEPTHANDLER)GetProcAddress(hLib,"TcpSvrAcceptFunc");
        TcpSvrDisconnFunc=(TCPSVRDISCONNHANDLER)GetProcAddress(hLib,"TcpSvrDisconnFunc");
        TcpSvrOntimeFunc=(TCPSVRONTIME)GetProcAddress(hLib,"TcpSvrOntimeFunc");

        UdpRecvFunc=(UDPSIMPLERCVDATAHANDLER)GetProcAddress(hLib,"UdpRecvFunc");
        UdpOntimeFunc=(UDPSIMPLEONTIME)GetProcAddress(hLib,"UdpOntimeFunc");

		TcpClientRecvFunc=(TCPCLINTRCVDATAHANDLER)GetProcAddress(hLib,"TcpClientRecvFunc");
		TcpClientDisconnFunc=(TCPCLINTDISCONNHANDLER)GetProcAddress(hLib,"TcpClientDisconnFunc");
		TcpClientStartFunc=(TCPCLINTSTARTCONNECT)GetProcAddress(hLib,"TcpClientStartFunc");
		TcpClientConnSuccFunc=(TCPCLINTCONNSUCCESSHANDLER)GetProcAddress(hLib,"TcpClientConnSuccFunc");
		TcpClientConnFailFunc=(TCPCLINTCONNFAILUREHANDLER)GetProcAddress(hLib,"TcpClientConnFailFunc");
		TcpClientOntimeFunc=(TCPCLINTONTIME)GetProcAddress(hLib,"TcpClientOntimeFunc");

    };

    void ReleaseDLL(void)
    {
        if(hLib)
        {
            FreeLibrary(hLib);
            hLib=NULL;
        }

        InitFunc=NULL;
        ReleaseFunc=NULL;
		IsInitCompletedFunc=NULL;
		NotifyEntireSystemInitCompletedFunc=NULL;

        TcpSvrRecvFunc=NULL;
        TcpSvrAcceptFunc=NULL;
        TcpSvrDisconnFunc=NULL;
        TcpSvrOntimeFunc=NULL;

        UdpRecvFunc=NULL;
        UdpOntimeFunc=NULL;

		TcpClientRecvFunc=NULL;
		TcpClientDisconnFunc=NULL;
		TcpClientStartFunc=NULL;
		TcpClientConnSuccFunc=NULL;
		TcpClientConnFailFunc=NULL;
		TcpClientOntimeFunc=NULL;

    };

    void CreateSvr(void)
    {
        if(!hLib)
            return;

    	//����tcp server
        if(Addr_TcpSvr.Port!=(unsigned short)-1)
		{
			if(MaxListen_TcpSvr==0) MaxListen_TcpSvr=16;
			pTcpSvr=new CTcpServerWorker(Addr_TcpSvr,MaxListen_TcpSvr,TcpSvrRecvFunc,TcpSvrAcceptFunc,TcpSvrDisconnFunc,TcpSvrOntimeFunc,OutBufSize_TcpSvr,InBufSize_TcpSvr,bReuseAddr);
		}

	    //����udp server
        if(Addr_UdpSvr.Port!=(unsigned short)-1)
	        pUdpSvr=new CUdpSimpleWorker(Addr_UdpSvr,UdpRecvFunc,UdpOntimeFunc,OutBufSize_UdpSvr,InBufSize_UdpSvr);

		//����tcp client
		for(int i=0;i<(int)Tools_Ansi_ArraySize(pTcpClient);i++)
		{
			if(SvrAddr_TcpClient[i].Port!=(unsigned short)-1)
			{
				pTcpClient[i]=new CTcpClientWorker(
					LocalAddr_TcpClient[i],
					SvrAddr_TcpClient[i],
					TcpClientRecvFunc,
					TcpClientDisconnFunc,
					TcpClientStartFunc,
					TcpClientConnSuccFunc,
					TcpClientConnFailFunc,
					TcpClientOntimeFunc,
					IOBufSize_TcpClient,
					IOBufSize_TcpClient,
					ReconnectInterval_TcpClient);
			}
		}

	};

    void ReleaseSvr(void)
    {
        if(pTcpSvr)
        {
            delete pTcpSvr;
            pTcpSvr=NULL;
        }
        if(pUdpSvr)
        {
            delete pUdpSvr;
            pUdpSvr=NULL;
        }
		for(int i=0;i<(int)Tools_Ansi_ArraySize(pTcpClient);i++)
        {
			if(pTcpClient[i])
			{
				delete pTcpClient[i];
				pTcpClient[i]=NULL;
			}
        }
    };





    HINSTANCE hLib;
    CTcpServerWorker *pTcpSvr;
    CUdpSimpleWorker *pUdpSvr;
	CTcpClientWorker *pTcpClient[8];

    char DllName[256];

	//���Դ���һ��tcpserver����
    Stru_IPAddress Addr_TcpSvr;
    int OutBufSize_TcpSvr;
    int InBufSize_TcpSvr;
	int MaxListen_TcpSvr;
    bool bReuseAddr;

	//���Դ���һ��udpserver����
	Stru_IPAddress Addr_UdpSvr;
    int OutBufSize_UdpSvr;
    int InBufSize_UdpSvr;

	//���Դ������8��tcpclient����
	Stru_IPAddress SvrAddr_TcpClient[8];
	Stru_IPAddress LocalAddr_TcpClient[8];
    int IOBufSize_TcpClient;
	int ReconnectInterval_TcpClient;		//�����������Ϊ��λ��0Ϊ������



	int HeartbeatTimeout;		//δʹ��

    Init_uniServerCBFunc InitFunc;
    Release_uniServerCBFunc ReleaseFunc;
	IsInitCompleted_uniServerCBFunc IsInitCompletedFunc;
	NotifyEntireSystemInitCompleted_uniServerCBFunc NotifyEntireSystemInitCompletedFunc;

    TCPSVRRCVDATAHANDLER TcpSvrRecvFunc;
    TCPSVRACCEPTHANDLER TcpSvrAcceptFunc;
    TCPSVRDISCONNHANDLER TcpSvrDisconnFunc;
    TCPSVRONTIME TcpSvrOntimeFunc;

    UDPSIMPLERCVDATAHANDLER UdpRecvFunc;
    UDPSIMPLEONTIME UdpOntimeFunc;

	TCPCLINTRCVDATAHANDLER TcpClientRecvFunc;
	TCPCLINTDISCONNHANDLER TcpClientDisconnFunc;
	TCPCLINTSTARTCONNECT TcpClientStartFunc;
	TCPCLINTCONNSUCCESSHANDLER TcpClientConnSuccFunc;
	TCPCLINTCONNFAILUREHANDLER TcpClientConnFailFunc;
	TCPCLINTONTIME TcpClientOntimeFunc;

};

vector<Stru_Function> g_vecFunc;

void WorkFunc_uniServer(DWORD TickCount);
void InitFunc_uniServer(int argc,char *argv[]);
void ReleaseFunc_uniServer(void);
void DelayFunc_uniServer(void);

//����uniServer
int Start_uniServer(int argc,char* argv[])
{
	//��ȡ����·��
	CTools_Win32::GetMainPath(g_MainPath,sizeof(g_MainPath)-1);

    //��ȡ��������
    char ServiceName[128];
    strcpy(ServiceName,"uniServer");
    CIniFile *pIniFile=NULL;
    char Buf[512];
	sprintf(Buf,"%sfunction.ini",g_MainPath);
	pIniFile=new CIniFile(Buf);
	if(pIniFile)
    {
        pIniFile->GetIniKeyStr("System","ServiceName","uniServer",ServiceName,sizeof(ServiceName));
        delete pIniFile;
    }


	CWinService::Start_WinService(		argc,
										argv,
										ServiceName,
										ServiceName,
										ServiceName,
										0,
										WorkFunc_uniServer,
										InitFunc_uniServer,
										ReleaseFunc_uniServer,
										DelayFunc_uniServer);

	return 1;
}


//��RunAsynģʽ����uniServer
void Start_uniServer_InRunAsynMode(void)
{
    char args[2][256]={"","runasyn"};
    char* argv[2]={args[0],args[1]};
    Start_uniServer(2,argv);
}

//ֹͣuniServer��ֻ����RunAsynģʽ�²���Ҫ����ֹͣ���ڷ���ģʽ��Cmd����ģʽ�²���
void Stop_uniServer_InRunAsynMode(void)
{
    CWinService::Stop_WinService();
}


void WorkFunc_uniServer(DWORD TickCount)
{
}

void InitFunc_uniServer(int argc,char *argv[])
{
    char Buf[512];

	g_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE,"uniServer.log");

	//���������ļ�����
    CIniFile *pIniFile=NULL;
	sprintf(Buf,"%sfunction.ini",g_MainPath);
	pIniFile=new CIniFile(Buf);
	if(!pIniFile) return;

    //ȡ������ַ
    CTools_Ansi::PerformWSAStartup();
    DWORD dwLocalIP=0;
    char defLocalIP[256];
    if(!CTools_Ansi::GetLocalHostName_IP(NULL,0,&dwLocalIP,1))
        dwLocalIP=CTools_Ansi::ConvertStrIP("127.0.0.1");
    strncpy(defLocalIP,CTools_Ansi::ConvertdwIP(dwLocalIP),sizeof(defLocalIP)-1);
    defLocalIP[sizeof(defLocalIP)-1]=0;
    CTools_Ansi::PerformWSACleanup();

    //˳���ȡ���õķ���������
    int FuncOrd=0;
    while(true)
    {
        char NodeName[256]="Function";
        if(!pIniFile->HasIniRootNode(NodeName,FuncOrd))
            break;

        Stru_Function FuncParam;
        pIniFile->GetIniKeyStr(NodeName,"DllName","",FuncParam.DllName,sizeof(FuncParam.DllName),0,FuncOrd);

        FuncParam.Addr_TcpSvr.IP=pIniFile->GetIniKeyIP(NodeName,"ip_TcpSvr",defLocalIP,0,FuncOrd);
        FuncParam.Addr_TcpSvr.Port=(unsigned short)pIniFile->GetIniKeyVal(NodeName,"port_TcpSvr",-1,0,FuncOrd);
        FuncParam.OutBufSize_TcpSvr=pIniFile->GetIniKeyVal(NodeName,"OutBufSize_TcpSvr",655360,0,FuncOrd);
        FuncParam.InBufSize_TcpSvr=pIniFile->GetIniKeyVal(NodeName,"InBufSize_TcpSvr",655360,0,FuncOrd);
        FuncParam.MaxListen_TcpSvr=pIniFile->GetIniKeyVal(NodeName,"MaxListen_TcpSvr",16,0,FuncOrd);
        FuncParam.bReuseAddr=pIniFile->GetIniKeyVal(NodeName,"ReuseAddr_TcpSvr",0,0,FuncOrd)==1?true:false;

        FuncParam.Addr_UdpSvr.IP=pIniFile->GetIniKeyIP(NodeName,"ip_UdpSvr",defLocalIP,0,FuncOrd);
        FuncParam.Addr_UdpSvr.Port=(unsigned short)pIniFile->GetIniKeyVal(NodeName,"port_UdpSvr",-1,0,FuncOrd);
        FuncParam.OutBufSize_UdpSvr=pIniFile->GetIniKeyVal(NodeName,"OutBufSize_UdpSvr",655360,0,FuncOrd);
        FuncParam.InBufSize_UdpSvr=pIniFile->GetIniKeyVal(NodeName,"InBufSize_UdpSvr",655360,0,FuncOrd);


		////���������8��tcpclient����
		for(int keyord=0;keyord<(int)Tools_Ansi_ArraySize(FuncParam.pTcpClient);keyord++)
		{
			if(!pIniFile->HasIniKey(NodeName,"serverip_TcpClient",keyord,FuncOrd)||!pIniFile->HasIniKey(NodeName,"serverport_TcpClient",keyord,FuncOrd))
				break;
			FuncParam.SvrAddr_TcpClient[keyord].IP=pIniFile->GetIniKeyIP(NodeName,"serverip_TcpClient",0,keyord,FuncOrd);
			FuncParam.SvrAddr_TcpClient[keyord].Port=(unsigned short)pIniFile->GetIniKeyVal(NodeName,"serverport_TcpClient",-1,keyord,FuncOrd);
			FuncParam.LocalAddr_TcpClient[keyord].IP=pIniFile->GetIniKeyIP(NodeName,"localip_TcpClient",0,keyord,FuncOrd);
			FuncParam.LocalAddr_TcpClient[keyord].Port=(unsigned short)pIniFile->GetIniKeyVal(NodeName,"localport_TcpClient",0,keyord,FuncOrd);
		}
		FuncParam.IOBufSize_TcpClient=pIniFile->GetIniKeyVal(NodeName,"IOBufSize_TcpClient",655360,0,FuncOrd);
		FuncParam.ReconnectInterval_TcpClient=pIniFile->GetIniKeyVal(NodeName,"ReconnectInterval_TcpClient",0,0,FuncOrd);

        FuncParam.HeartbeatTimeout=pIniFile->GetIniKeyVal(NodeName,"heartbeattimeout",16,0,FuncOrd);

        g_vecFunc.push_back(FuncParam);

        FuncOrd++;
    };

    //�ͷ�pIniFile
	if(pIniFile)
	{
		delete pIniFile;
		pIniFile=NULL;
	}


    //˳�����Func
    for(int i=0,Cnt=g_vecFunc.size();i<Cnt;i++)
        g_vecFunc[i].LoadDLL(g_MainPath);

    //˳�����Func�ĳ�ʼ������
    for(int i=0,Cnt=g_vecFunc.size();i<Cnt;i++)
        if(g_vecFunc[i].hLib&&g_vecFunc[i].InitFunc!=NULL)
            g_vecFunc[i].InitFunc();

	//�ȴ�����ģ���ʼ����ɡ�ģ���ʼ��ʱ������Ҫ�����̡߳����̴߳�����ɺ�����ʼ������
	while(true)
	{
		int i,Cnt=g_vecFunc.size();
		for(i=0;i<Cnt;i++)
		{
			int InitPercent;
			char InitMsg[1024];
			if(g_vecFunc[i].hLib&&g_vecFunc[i].IsInitCompletedFunc!=NULL&&!g_vecFunc[i].IsInitCompletedFunc(InitPercent,InitMsg,sizeof(InitMsg)))
				break;
		}
		if(i>=(int)g_vecFunc.size())
			break;

		Sleep(1000);
	}

    //˳��Ϊ����ģ�鴴��Tcp����(��Ҫ�Ļ�)
    for(int i=0,Cnt=g_vecFunc.size();i<Cnt;i++)
        g_vecFunc[i].CreateSvr();

	//֪ͨ����ģ�飬��ʼ���Ѿ�ȫ�����
    for(int i=0,Cnt=g_vecFunc.size();i<Cnt;i++)
        if(g_vecFunc[i].hLib&&g_vecFunc[i].NotifyEntireSystemInitCompletedFunc!=NULL)
            g_vecFunc[i].NotifyEntireSystemInitCompletedFunc();
}

void ReleaseFunc_uniServer(void)
{
	int Cnt=(int)g_vecFunc.size();
    for(int i=Cnt-1;i>=0;i--)
    {
        g_vecFunc[i].ReleaseSvr();

        if(g_vecFunc[i].hLib&&g_vecFunc[i].ReleaseFunc!=NULL)
            g_vecFunc[i].ReleaseFunc();

        g_vecFunc[i].ReleaseDLL();
    }
	if(g_pWriteLog)
	{
		delete g_pWriteLog;
		g_pWriteLog=NULL;
	}
}

void DelayFunc_uniServer(void)
{
	Sleep(1000);
}



