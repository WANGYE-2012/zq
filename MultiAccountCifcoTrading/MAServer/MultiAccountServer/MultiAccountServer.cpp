// MultiAccountServer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include "CommonPkg.h"
#include "Tools_Win32.h"
#include "MultiAccountServer.h"
#include "MD5/MD5Checksum.h"
#include "Worker.h"
#include "ConfigManager.h"

using namespace std;

#ifdef _DEBUG
#pragma comment(lib, "Tools4dllD.lib")											
#else 
#pragma comment(lib, "Tools4dll.lib")											
#endif

CWriteLog* g_pWriteLog = NULL;
char* g_pMASem = NULL;
int g_nMASemCount = 0;
std::map<SOCKET,Stru_DataSocket> g_TcpUsers;					//�Ѿ�����tcp���ӵ��û�
std::map<int, std::set<SOCKET>> g_OnlineUser;			//�����û�
std::set<SOCKET> g_OnlineUserSet;			//�����û������ڲ���ʹ��
queue<RequestData> g_msgQueue;							//��Ϣ����
Worker* g_pWorker = NULL;

string GetSysPath();

MULTIACCOUNTSERVER_API void InitFunc( void )
{
	g_pMASem = CTools_Win32::AllocWinSem();
	g_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE,"MultiAccountServer.log");

	//��ն���
	while( !g_msgQueue.empty() )
	{
		g_msgQueue.pop();
	}

	//����Worker�߳�
	g_pWorker = new Worker( ConfigManager::GetInstance()->GetDBUser().c_str(), 
		ConfigManager::GetInstance()->GetDBPwd().c_str(), 
		ConfigManager::GetInstance()->GetDBSID().c_str());
}

MULTIACCOUNTSERVER_API void ReleaseFunc( void )
{
	if ( g_pWorker != NULL )
	{
		delete g_pWorker;
		g_pWorker = NULL;
	}

	if(g_pWriteLog != NULL)
	{
		g_pWriteLog->WriteLog_Fmt("�رշ���", WriteLogLevel_DEBUGINFO, "Line=%d", __LINE__ );
		delete g_pWriteLog;
		g_pWriteLog = NULL;
	}

	if(g_pMASem) 
	{
		CTools_Win32::TakeWinSem(g_pMASem);
		g_nMASemCount++;
	}

	while( !g_msgQueue.empty() )
	{
		RequestData reqData = g_msgQueue.front();
		g_msgQueue.pop();
		if ( NULL != reqData.pData )
		{
			delete[] reqData.pData;
			reqData.pData = NULL;
		}
	}

	if(g_pMASem) 
	{
		CTools_Win32::GiveWinSem(g_pMASem);
		g_nMASemCount--;
	}

	if ( g_pMASem != NULL )
	{
		CTools_Win32::ReleaseWinSem(g_pMASem);
		g_pMASem = NULL;
		g_nMASemCount=0;
	}

	ConfigManager::DestroyConfigManager();
}

string GetSysPath()
{
	char szBuffer[MAX_PATH];
	memset( szBuffer, 0, sizeof(szBuffer));
	::GetModuleFileName(NULL, szBuffer, sizeof(szBuffer));
	string strSysPath(szBuffer);
	int nPos = strSysPath.rfind('\\');
	if ( -1 != nPos )
	{
		strSysPath = strSysPath.substr(0, nPos);
	}

	return strSysPath;
}

// This is the constructor of a class that has been exported.
// see MultiAccountServer.h for the class definition
CMultiAccountServer::CMultiAccountServer()
{
	return;
}
