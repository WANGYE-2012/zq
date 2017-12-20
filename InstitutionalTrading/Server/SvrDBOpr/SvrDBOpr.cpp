// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include "SvrDBOpr.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "Interface_SvrDBOpr.h"
#include "ConfigManager.h"


////-----------------------------------------------------------------------------------
////	import Tools4dll��
////-----------------------------------------------------------------------------------
//#ifdef _DEBUG
//#pragma comment(lib, "Tools4dllD.lib")
//#else
//#pragma comment(lib, "Tools4dll.lib")
//#endif
//
//#pragma comment(lib, "SvrNotifyAndAsk.lib")




#define WRITELOGID_SvrDBOpr




//ȫ�ֻ�����
Ceasymutex			g_mutex;

//�̲߳���
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;
DWORD ThreadWorker(void *arg);





//ģ���ʼ�������������̲߳���SvrTcp���ĸ���Ȥ�����ݰ�
SVRDBOPR_API void InitFunc(void)
{
	//���������߳�
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);

	ConfigManager::Init();
	CInterface_SvrDBOpr::getObj().InitDB(	ConfigManager::Get()->GetDBUser(),
											ConfigManager::Get()->GetDBPwd(),
											ConfigManager::Get()->GetDBSID());

	CInterface_SvrDBOpr::getObj().Conncect();

	//���涩�ı��̸߳���Ȥ�����ݰ�
	//CInterface_SvrTcp::getObj().SubscribePkg(CMDID_HEARTBEAT,g_idThread);
}

//ģ��������ͷ���Դ���رչ����߳�
SVRDBOPR_API void ReleaseFunc(void)     
{
	if(g_hThread)
	{
		//����WM_QUIT֪ͨ�߳̽���
		PostThreadMessage(g_idThread,WM_QUIT,0,0);

		//�ȴ��߳��˳�
		DWORD ExitCode;
		WaitForSingleObject((HANDLE)g_hThread,8000);
		if(GetExitCodeThread((HANDLE)g_hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)g_hThread,0);
		CloseHandle(g_hThread);
		g_hThread=NULL;
		g_idThread=0;
	}

	ConfigManager::Release();
}

//�����߳�
DWORD ThreadWorker(void *arg)
{
	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{
	}
	return 0;
}

