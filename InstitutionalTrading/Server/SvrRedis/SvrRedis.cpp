// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#include "stdafx.h"
#include <windows.h>
#include "MyXML.h"


//-----------------------------------------------------------------------------------
//	import Tools4dll��
//-----------------------------------------------------------------------------------
#ifdef _DEBUG
#pragma comment(lib, "Tools4dllD.lib")											
#else 
#pragma comment(lib, "Tools4dll.lib")											
#endif




#define WRITELOGID_SvrRedis



bool mbSlave = false;
DWORD g_RedisServerProcessId = 0;
bool CreateRedisSvrProcess()
{
    char Node[128];
    char Buf[512];
    char PathFilePath[512];

    //��ȡ����·��
    memset(PathFilePath,0,sizeof(PathFilePath));
    int i,j=sizeof(PathFilePath)-1-strlen("redis-server.exe");
    memset(PathFilePath,0,j);
    GetModuleFileName(NULL,PathFilePath,j-1);
    i=strlen(PathFilePath);
    while(i>0&&PathFilePath[i-1]!='\\'&&PathFilePath[i-1]!=':') 
        PathFilePath[--i]=0;
    if(PathFilePath[i-1]!='\\')
        strcat((char*)PathFilePath,"\\");


    strcat(PathFilePath,"redis-server.exe");

    //string sPath = "F:\\svn_minor\\Miscellaneous\\Redis-Server\\redis-server.exe";

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    si.wShowWindow=SW_SHOW;
    si.dwFlags=STARTF_USESHOWWINDOW;
    ZeroMemory( &pi, sizeof(pi) );

    bool bRet = CreateProcess(LPCSTR(PathFilePath),
        NULL,
        NULL, 
        NULL, 
        FALSE, 
        CREATE_NEW_CONSOLE , 
        NULL, 
        NULL, 
        &si,
        &pi);

    // Wait until child process exits.
    //WaitForSingleObject( pi.hProcess, INFINITE );

    g_RedisServerProcessId = pi.dwProcessId;

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

    return bRet;
}

bool KillRedisSvrProcess(DWORD ProcessId)
{
    HANDLE hProcess=OpenProcess(PROCESS_TERMINATE,FALSE,ProcessId);
    if(hProcess==NULL)
        return FALSE;
    if(!TerminateProcess(hProcess,0))
        return FALSE;

    g_RedisServerProcessId = 0;
    return TRUE;
}


//ģ���ʼ�������������̲߳���SvrTcp���ĸ���Ȥ�����ݰ�
SVRREDIS_API void InitFunc(void)
{
	//Redis��������ӷ�������¼ģ�鿪����������ӷ�������¼ģ�������˳���ر�
    CMyXML myxml("InstitutionalTrading.xml");

    //���ӷ�����������
    mbSlave = myxml.GetNodeContentbool2(false,"root","SlaveServer");
    if(mbSlave)
    {
        if(!CreateRedisSvrProcess())
            LOGIN_LOG("Redis Server Startup Failed!!!!");
    }
}

//ģ��������ͷ���Դ���رչ����߳�
SVRREDIS_API void ReleaseFunc(void)
{
    //if(mbSlave&&g_RedisServerProcessId>0)
    //{
    //    Sleep(3000);//�ȴ��ͻ��˶������˳�
    //    if(!KillRedisSvrProcess(g_RedisServerProcessId))
    //        LOGIN_LOG("Redis Server Shutdown Failed!!!!");

    //    Sleep(2000);
    //}
}