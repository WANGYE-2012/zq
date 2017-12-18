// WinService.cpp: implementation of the CWinService class.
//
//////////////////////////////////////////////////////////////////////

#include "WinService.h"
#include "Tools_Ansi.h"
#include "Tools_Win32.h"



char						CWinService::m_ServiceName[256];					//��Ӧ�ó�����
char						CWinService::m_ServiceDisplayName[256];				//��Ӧ�ó�����
char						CWinService::m_ServiceDescription[256];				//��Ӧ�ó�����
int							CWinService::m_InteractiveID=0;						//�����Ƿ��ܽ��н��潻��
WinService_WORKFUNC			CWinService::m_pWorkFunc=NULL;						//���������������߳�ѭ�����ô˻ص��������ڴ˻ص��������������Ĺ�����������ѭ������
WinService_DELAYFUNC		CWinService::m_pDelayFunc=NULL;						//��ʱ�����������߳�ѭ������ʱ��ÿ�ε����깤����������ô˺���������ʱ����Sleep��
WinService_INITFUNC			CWinService::m_pInitFunc=NULL;						//��ʼ����������������ǰ���ô˺�������Ӧ�ó����ʼ��
WinService_RELEASEFUNC		CWinService::m_pReleaseFunc=NULL;					//�ͷź���������ֹͣǰ���ô˺���������Ӧ�ó����ͷ�

																				//********�����Ƿ�����ʹ�õĲ���********
SERVICE_STATUS_HANDLE		CWinService::hWinService_MyServiceStatusHandle=NULL;//����״̬���


int							CWinService::m_WinService_RunningID=0;				//�����Ƿ���������
int							CWinService::m_WinService_PauseID=0;				//�����Ƿ���ͣ
int							CWinService::m_WinService_ReleaseOK=0;				//�����Ƿ�ɹ�ֹͣ

char						CWinService::m_ModuleName[MAX_PATH];				//Current module name with directory

int							CWinService::m_RunInServiceModeID=0;				//�Ƿ������ڷ���ģʽ��
int							CWinService::m_RunningID_RunMode=0;					//��Runģʽ�£�ϵͳ�Ƿ���������
int							CWinService::m_NeedStopID=0;						//�Ƿ���Ҫֹͣ����

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWinService::CWinService()
{

}

CWinService::~CWinService()
{

}


//	���� �� InServiceInstalled
//	���� �� �ж�һ�������Ƿ��Ѿ���װ�������÷�����ָ��
//	���� �� ServiceName	:��������
//	���� �� TRUE:���ڣ�FALSE:������
BOOL CWinService::IsServiceInstalled(char *ServiceName)
{
	SC_HANDLE		schSCManager=NULL;
	SC_HANDLE		schService=NULL;

	if(!ServiceName) return FALSE;

	schSCManager=OpenSCManager(	NULL,						//local machine 
								NULL,						//ServicesActive database 
								SC_MANAGER_ALL_ACCESS);		//full access rights 
	if(schSCManager)
		schService=OpenService(	schSCManager,				// SCManager database 
								ServiceName,				// name of service 
								READ_CONTROL);

	if(schService) CloseServiceHandle(schService);
	if(schSCManager) CloseServiceHandle(schSCManager);

	return schService?TRUE:FALSE;
}


//	���� �� GetServiceStatus
//	���� �� ��ȡ����ĵ�ǰ״ֵ̬
//	���� �� ServiceName : ��������
//	���� �� ����ĵ�ǰ״ֵ̬����ͷ�ļ����ж���,-1Ϊʧ��
DWORD CWinService::GetServiceStatus(char *ServiceName)
{
	SC_HANDLE		schSCManager=NULL;
	SC_HANDLE		schService=NULL;
	BOOL			bRlt=FALSE;
	SERVICE_STATUS	iStatus;

	if(!ServiceName) return FALSE;

	schSCManager=OpenSCManager(	NULL,						//local machine 
								NULL,						//ServicesActive database 
								SC_MANAGER_ALL_ACCESS);		//full access rights 
	if(schSCManager)
		schService=OpenService(	schSCManager,				// SCManager database 
								ServiceName,				// name of service 
								SERVICE_ALL_ACCESS);

	if(schService)
	{
		memset((char*)&iStatus,0,sizeof(iStatus));
		bRlt=QueryServiceStatus(schService,&iStatus);
	}

	if(schService) CloseServiceHandle(schService);
	if(schSCManager) CloseServiceHandle(schSCManager);

	return bRlt?iStatus.dwCurrentState:0xffffffff;
}


//	���� �� InstallService
//	���� �� ��װһ������
//	���� �� ServiceName	:��������
//			DisplayName	:��ʾ����
//			FileName	:���������ļ�������
//	���� �� TRUE:�ɹ���FALSE:ʧ��
BOOL CWinService::InstallService(char *ServiceName,char *DisplayName,char *FileName,char *Description,bool bInteractive)
{
	SC_HANDLE							schSCManager=NULL;
	SC_HANDLE							schService=NULL;
	SERVICE_DESCRIPTION					sdBuf;


	if(!ServiceName||!DisplayName||!FileName) return FALSE;
	if(IsServiceInstalled(ServiceName)) return TRUE;

	schSCManager=OpenSCManager(		NULL,										// local machine 
									NULL,										// ServicesActive database 
									SC_MANAGER_ALL_ACCESS);						// full access rights 

	if(schSCManager)
	{
		if(bInteractive)
			schService=CreateService(	schSCManager,								// SCManager database 
										ServiceName,								// name of service 
										DisplayName,								// service name to display 
										SERVICE_ALL_ACCESS,							// desired access 
										SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS,					// service type 
										SERVICE_AUTO_START,							// start type 
										SERVICE_ERROR_NORMAL,						// error control type 
										FileName,									// service's binary 
										NULL,										// no load ordering group 
										NULL,										// no tag identifier 
										NULL,										// no dependencies 
										NULL,										// LocalSystem account 
										NULL);										// no password 
		else 
			schService=CreateService(	schSCManager,								// SCManager database 
										ServiceName,								// name of service 
										DisplayName,								// service name to display 
										SERVICE_ALL_ACCESS,							// desired access 
										SERVICE_WIN32_OWN_PROCESS,					// service type 
										SERVICE_AUTO_START,							// start type 
										SERVICE_ERROR_NORMAL,						// error control type 
										FileName,									// service's binary 
										NULL,										// no load ordering group 
										NULL,										// no tag identifier 
										NULL,										// no dependencies 
										NULL,										// LocalSystem account 
										NULL);										// no password 
	}
	if(schService&&Description&&Description[0])
	{
		sdBuf.lpDescription = Description;
		ChangeServiceConfig2(		schService,									// handle to service
									SERVICE_CONFIG_DESCRIPTION,					// change: description
									&sdBuf);									// value: new description
	}

	if(schService) CloseServiceHandle(schService);
	if(schSCManager) CloseServiceHandle(schSCManager);


	return schService?TRUE:FALSE;
}


//	���� �� UninstallService
//	���� �� ж��һ������
//	���� �� ServiceName : ������
//	���� �� TRUE:�ɹ���FALSE:ʧ��
BOOL CWinService::UninstallService(char *ServiceName)
{
	SC_HANDLE		schSCManager=NULL;
	SC_HANDLE		schService=NULL;
	BOOL			bUninstallRlt=FALSE;

	if(!ServiceName) return FALSE;

	schSCManager=OpenSCManager(	NULL,						//local machine 
								NULL,						//ServicesActive database 
								SC_MANAGER_ALL_ACCESS);		//full access rights 
	if(schSCManager)
		schService=OpenService(	schSCManager,				// SCManager database 
								ServiceName,				// name of service 
								DELETE);					// only need DELETE access 

	if(schService)
		bUninstallRlt=DeleteService(schService);

	if(schService) CloseServiceHandle(schService);
	if(schSCManager) CloseServiceHandle(schSCManager);

	if(!schSCManager) return FALSE;
	else if(!schSCManager) return TRUE;
	else if(bUninstallRlt) return TRUE;
	else return FALSE;
}


//	���� �� MyStartService
//	���� �� ����һ������
//	���� �� ServiceName : ������
//	���� �� TRUE:�ɹ���FALSE:ʧ��
BOOL CWinService::MyStartService(char *ServiceName)
{
	SC_HANDLE		schSCManager=NULL;
	SC_HANDLE		schService=NULL;
	BOOL			bRlt=FALSE;

	if(!ServiceName) return FALSE;

	schSCManager=OpenSCManager(	NULL,						//local machine 
								NULL,						//ServicesActive database 
								SC_MANAGER_ALL_ACCESS);		//full access rights 
	if(schSCManager)
		schService=OpenService(	schSCManager,				// SCManager database 
								ServiceName,				// name of service 
								SERVICE_ALL_ACCESS);

	if(schService)
		bRlt=StartService(schService,0,NULL);

	if(schService) CloseServiceHandle(schService);
	if(schSCManager) CloseServiceHandle(schSCManager);

	return bRlt?TRUE:FALSE;
}


//	���� �� MyStopService
//	���� �� ֹͣһ������
//	���� �� ServiceName : ������
//	���� �� TRUE:�ɹ���FALSE:ʧ��
BOOL CWinService::MyStopService(char *ServiceName)
{
	SC_HANDLE		schSCManager=NULL;
	SC_HANDLE		schService=NULL;
	BOOL			bRlt=FALSE;
	SERVICE_STATUS	iStatus;

	if(!ServiceName) return FALSE;

	schSCManager=OpenSCManager(	NULL,						//local machine 
								NULL,						//ServicesActive database 
								SC_MANAGER_ALL_ACCESS);		//full access rights 
	if(schSCManager)
		schService=OpenService(	schSCManager,				// SCManager database 
								ServiceName,				// name of service 
								SERVICE_ALL_ACCESS);

	if(schService)
	{
		memset((char*)&iStatus,0,sizeof(iStatus));
		bRlt=ControlService(schService,SERVICE_CONTROL_STOP,&iStatus);
	}

	if(schService) CloseServiceHandle(schService);
	if(schSCManager) CloseServiceHandle(schSCManager);

	return bRlt?TRUE:FALSE;
}

//�Ƿ������ڷ���ģʽ��
BOOL CWinService::IsRunInServiceMode(void)										
{
	return m_RunInServiceModeID?TRUE:FALSE;
}


//	���� �� ServiceEntry
//	���� �� 1. ���Ƿ�����ں���
//			2. �˳���Ӧ���ڷ�������main()�е���
//			3. ��������main()�е��ô˺����󼴿��˳�
//			4. �������Ҫ�������߳�����У��̺߳�����ַ�ɲ������룬�ڴ˺����д����߳�
//	���� �� pWorkFunc : ���������������߳�ѭ�����ô˻ص��������ڴ˻ص��������������Ĺ�����������ѭ������
//			pDelayFunc : ��ʱ�����������߳�ѭ������ʱ��ÿ�ε����깤����������ô˺���������ʱ����Sleep��
//			pInitFunc :	��ʼ����������������ǰ���ô˺�������Ӧ�ó����ʼ��
//			pReleaseFunc : �ͷź���������ֹͣǰ���ô˺���������Ӧ�ó����ͷ�
//	���� �� TRUE:�ɹ���FALSE:ʧ��
BOOL CWinService::ServiceEntry(void)
{
	BOOL			bRlt;
	SERVICE_TABLE_ENTRY DispatchTable[2]={{m_ServiceName,WinService_ServiceMain},{NULL,NULL}}; 

	bRlt=StartServiceCtrlDispatcher(DispatchTable);

	return bRlt;
}

//	���� �� ����������WinService_ServiceMain
//	���� �� 1. ���Ƿ���������
//			2. ��������ʱ��SCM����ô˺������Ӷ���������
//			3. ServiceMain is called when the SCM wants to start the service. When it returns, the 
//			   service has stopped. It therefore waits on an event just before the end of the function, 
//			   and that event gets set when it is time to stop.
//			   It also returns on any error because the service cannot start if there is an eror.
//	���� �� argc��argv��ΪSCM�������Ĳ���
void CWinService::WinService_ServiceMain(DWORD argc,LPTSTR *argv)
{
	SERVICE_STATUS      iServiceStatus;
	DWORD				iCount=0;
	HANDLE				hSleepEvent=NULL;


																				//ע�����״̬��Ϣ���Ĵ�����
    hWinService_MyServiceStatusHandle=RegisterServiceCtrlHandler(m_ServiceName,WinService_MyServiceCtrlHandler); 
    if(!hWinService_MyServiceStatusHandle) return ;

	m_WinService_RunningID=1;														//�����Ƿ���������
	m_WinService_PauseID=1;														//��������ͣ
	m_WinService_ReleaseOK=0;														//����û��ֹͣ

																				//Ӧ�ó�����Ҫ��ʼ����������״̬����Ϊ��������״̬
	if(m_pInitFunc)
	{
		memset((char*)&iServiceStatus,0,sizeof(iServiceStatus));
		iServiceStatus.dwServiceType		= SERVICE_WIN32; 
		iServiceStatus.dwCurrentState		= SERVICE_START_PENDING; 
		iServiceStatus.dwControlsAccepted	= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
		SetServiceStatus(hWinService_MyServiceStatusHandle,&iServiceStatus);

		(*m_pInitFunc)(argc,argv);
	}
																				//������״̬����Ϊ����״̬
	memset((char*)&iServiceStatus,0,sizeof(iServiceStatus));
	iServiceStatus.dwServiceType		= SERVICE_WIN32; 
	iServiceStatus.dwCurrentState		= SERVICE_RUNNING; 
	iServiceStatus.dwControlsAccepted	= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
	SetServiceStatus(hWinService_MyServiceStatusHandle,&iServiceStatus);
	m_WinService_PauseID=0;														//����û����ͣ
	m_NeedStopID=0;


	if(!m_pDelayFunc)
		hSleepEvent=CreateEvent(NULL,FALSE,FALSE,NULL);

	while(m_WinService_RunningID)													//ִ�й����̣߳��˳�����ʾ����ֹͣ
	{
		if(!m_WinService_PauseID&&m_pWorkFunc)
		{
			(*m_pWorkFunc)(iCount);									//��������
			iCount++;
		}

		if(m_NeedStopID)
		{																		//�����ⲿ���ã�������Ҫֹͣ
			MyStopService(m_ServiceName);
			break;
		}

																				//������ʱ
		if(m_pDelayFunc)												//����Ӧ�ó��������ʱ����
			(*m_pDelayFunc)();
		else if(hSleepEvent)
			WaitForSingleObject(hSleepEvent,1000);
		else Sleep(1000);
	}

	if(hSleepEvent)
	{
		CloseHandle(hSleepEvent);
		hSleepEvent=NULL;
	}

																				//��Ҫ�ͷ�Ӧ�ó�����Դ
	if(m_pReleaseFunc)
		(*m_pReleaseFunc)();


	m_WinService_ReleaseOK=1;

}


//	���� �� WinService_MyServiceCtrlHandler
//	���� �� 1. ���Ƿ���״̬��Ϣ�ַ�����
//			2. ���û�ͨ���������ı䱾�����״̬ʱ��SCM���ô˺�������
//			3. �˺�������֪ͨ�����̣߳�Ȼ��֪ͨSCM���ķ���״̬��ʾ
//	���� �� Opcode: ����״̬��Ϣ��
void CWinService::WinService_MyServiceCtrlHandler(DWORD Opcode)
{ 
    switch(Opcode) 
    { 
        case SERVICE_CONTROL_PAUSE: 
		{
			m_WinService_PauseID=1;

			if(hWinService_MyServiceStatusHandle)
			{
				SERVICE_STATUS      iServiceStatus;

				memset((char*)&iServiceStatus,0,sizeof(iServiceStatus));
				iServiceStatus.dwServiceType		= SERVICE_WIN32; 
				iServiceStatus.dwCurrentState		= SERVICE_START_PENDING; 
				iServiceStatus.dwControlsAccepted	= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
				iServiceStatus.dwCurrentState		= SERVICE_PAUSED; 
				SetServiceStatus(hWinService_MyServiceStatusHandle,&iServiceStatus);
			}
            break; 
		}
        case SERVICE_CONTROL_CONTINUE: 
		{
			m_WinService_PauseID=0;

			if(hWinService_MyServiceStatusHandle)
			{
				SERVICE_STATUS      iServiceStatus;

				memset((char*)&iServiceStatus,0,sizeof(iServiceStatus));
				iServiceStatus.dwServiceType		= SERVICE_WIN32; 
				iServiceStatus.dwCurrentState		= SERVICE_START_PENDING; 
				iServiceStatus.dwControlsAccepted	= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
				iServiceStatus.dwCurrentState		= SERVICE_RUNNING; 
				SetServiceStatus(hWinService_MyServiceStatusHandle,&iServiceStatus);
			}
            break; 
		}
		case SERVICE_CONTROL_SHUTDOWN:
        case SERVICE_CONTROL_STOP: 
		{
			int		i;
			HANDLE	hSleepEvent=NULL;

			m_WinService_RunningID=0;

			if(hWinService_MyServiceStatusHandle)
			{
				SERVICE_STATUS      iServiceStatus;

				memset((char*)&iServiceStatus,0,sizeof(iServiceStatus));
				iServiceStatus.dwServiceType		= SERVICE_WIN32; 
				iServiceStatus.dwCurrentState		= SERVICE_START_PENDING; 
				iServiceStatus.dwControlsAccepted	= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
				iServiceStatus.dwCurrentState		= SERVICE_STOP_PENDING; 
				SetServiceStatus(hWinService_MyServiceStatusHandle,&iServiceStatus);
			}

			hSleepEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
			for(i=0;i<40;i++)
			{
				if(hSleepEvent)
					WaitForSingleObject(hSleepEvent,500);
				else 
					Sleep(500);

				if(m_WinService_ReleaseOK) break;
			}
			if(hSleepEvent)
			{
				CloseHandle(hSleepEvent); 
				hSleepEvent=NULL;
			}
			

			if(hWinService_MyServiceStatusHandle)
			{
				SERVICE_STATUS      iServiceStatus;

				memset((char*)&iServiceStatus,0,sizeof(iServiceStatus));
				iServiceStatus.dwServiceType		= SERVICE_WIN32; 
				iServiceStatus.dwCurrentState		= SERVICE_START_PENDING; 
				iServiceStatus.dwControlsAccepted	= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
				iServiceStatus.dwCurrentState		= SERVICE_STOPPED; 
				SetServiceStatus(hWinService_MyServiceStatusHandle,&iServiceStatus);
			}
            break; 
		}
        case SERVICE_CONTROL_INTERROGATE: 
            break; 
        default: 
			break;
    } 
} 






int CWinService::Start_WinService(int argc,
								  char *argv[],
								  char *pServiceName,
								  char *pDispName,
								  char *pDescription,
								  int InteractiveID,
								  WinService_WORKFUNC pWorkFunc,
								  WinService_INITFUNC pInitFunc,
								  WinService_RELEASEFUNC pReleaseFunc,
								  WinService_DELAYFUNC pDelayFunc)
{


	memset(m_ServiceName,0,sizeof(m_ServiceName));
	strncpy(m_ServiceName,pServiceName,sizeof(m_ServiceName)-1);
	memset(m_ServiceDisplayName,0,sizeof(m_ServiceDisplayName));
	strncpy(m_ServiceDisplayName,pDispName,sizeof(m_ServiceDisplayName)-1);
	memset(m_ServiceDescription,0,sizeof(m_ServiceDescription));
	strncpy(m_ServiceDescription,pDescription,sizeof(m_ServiceDescription)-1);
	m_InteractiveID=InteractiveID;

	m_pInitFunc=pInitFunc;									//��ʼ����������������ǰ���ô˺�������Ӧ�ó����ʼ��
	m_pReleaseFunc=pReleaseFunc;							//�ͷź���������ֹͣǰ���ô˺���������Ӧ�ó����ͷ�
	m_pWorkFunc=pWorkFunc;									//���������������߳�ѭ�����ô˻ص��������ڴ˻ص��������������Ĺ�����������ѭ������
	m_pDelayFunc=pDelayFunc;								//��ʱ�����������߳�ѭ������ʱ��ÿ�ε����깤����������ô˺���������ʱ����Sleep��





															//********����ȡִ�г����·��������********
	memset(m_ModuleName,0,sizeof(m_ModuleName));
	GetModuleFileName(NULL,m_ModuleName,sizeof(m_ModuleName)-1);




																				//��װ����
	if(argc>=2&&stricmp(argv[1],WinService_INSTALLONLYSERVICECMD)==0)
	{
		if(argc==2)
		{
			if(!IsServiceInstalled(m_ServiceName))
				InstallService(m_ServiceName,m_ServiceDisplayName,m_ModuleName,m_ServiceDescription);
		}
		else
		{
			char	iServiceName[256];
			memset(iServiceName,0,sizeof(iServiceName));
			strncpy(iServiceName,argv[2],sizeof(iServiceName)-1);
			if(!IsServiceInstalled(iServiceName))
				InstallService(iServiceName,iServiceName,iServiceName,iServiceName);
		}
		return 0;
	}

																				//��װ����������
	if(argc>=2&&stricmp(argv[1],WinService_INSTALLSERVICECMD)==0)	
	{
		if(argc==2)
		{
			if(!IsServiceInstalled(m_ServiceName))
				InstallService(m_ServiceName,m_ServiceDisplayName,m_ModuleName,m_ServiceDescription);
			if(IsServiceInstalled(m_ServiceName))
				MyStartService(m_ServiceName);
		}
		else
		{
			char	iServiceName[256];
			memset(iServiceName,0,sizeof(iServiceName));
			strncpy(iServiceName,argv[2],sizeof(iServiceName)-1);
			if(!IsServiceInstalled(iServiceName))
				InstallService(iServiceName,iServiceName,iServiceName,iServiceName);
			if(IsServiceInstalled(iServiceName))
				MyStartService(iServiceName);
		}
		return 0;
	}
	
																				//ж�ط���
	if(argc>=2&&stricmp(argv[1],WinService_UNINSTALLSERVICECMD)==0)
	{
		if(argc==2)
		{
			if(IsServiceInstalled(m_ServiceName))
				MyStopService(m_ServiceName);
			if(IsServiceInstalled(m_ServiceName))
				UninstallService(m_ServiceName);
		}
		else
		{
			char	iServiceName[256];
			memset(iServiceName,0,sizeof(iServiceName));
			strncpy(iServiceName,argv[2],sizeof(iServiceName)-1);

			if(IsServiceInstalled(iServiceName))
				MyStopService(iServiceName);
			if(IsServiceInstalled(iServiceName))
				UninstallService(iServiceName);
		}
		return 0;
	}

																				//��������
	if(argc>=2&&stricmp(argv[1],WinService_STARTSERVICECMD)==0)
	{
		if(argc==2)
			MyStartService(m_ServiceName);
		else
		{
			char	iServiceName[256];
			memset(iServiceName,0,sizeof(iServiceName));
			strncpy(iServiceName,argv[2],sizeof(iServiceName)-1);

			MyStartService(iServiceName);
		}
		return 0;
	}

																				//ֹͣ����
	if(argc>=2&&stricmp(argv[1],WinService_STOPSERVICECMD)==0)
	{
		if(argc==2)
			MyStopService(m_ServiceName);
		else
		{
			char	iServiceName[256];
			memset(iServiceName,0,sizeof(iServiceName));
			strncpy(iServiceName,argv[2],sizeof(iServiceName)-1);

			MyStopService(iServiceName);
		}
		return 0;
	}

																				//Runģʽ��ֱ������
	if(argc==2&&stricmp(argv[1],WinService_RUNCMD)==0)
	{
		char		*iargv[2]={"",""};
		HANDLE		hThread=NULL;
		DWORD		ThreadID;
		DWORD		ExitCode;
		char		InputBuf[256],ich;
		HANDLE		hSleepEvent;

		
		m_RunInServiceModeID=0;

		if(m_pInitFunc)
		{
			m_pInitFunc(argc,argv);
		}

		m_RunningID_RunMode=1;
		m_NeedStopID=0;
		hThread=CreateThread(NULL,65536,(LPTHREAD_START_ROUTINE)mainService_WorkerThread_RunMode,NULL,0,&ThreadID);

		printf("press 'exit' to exit......\n");

		memset(InputBuf,0,sizeof(InputBuf));

		hSleepEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
		while(!m_NeedStopID)
		{
			if(_kbhit())
			{
				ich=(char)_getche();
				if(ich==10||ich==13)
				{
					if(stricmp(InputBuf,"exit")==0)
						break;
					memset(InputBuf,0,sizeof(InputBuf));
					printf("\r\n");
				}
				else if(ich==8)
				{
					int Len=strlen(InputBuf);
					if(Len>0)
					{
						InputBuf[Len-1]=0;
						printf("\r%s",InputBuf);
					}
				}
				else
					InputBuf[strlen(InputBuf)]=ich;
			}
			else
				WaitForSingleObject(hSleepEvent,100);
		}
		CloseHandle(hSleepEvent); 

		m_RunningID_RunMode=0;
		WaitForSingleObject((HANDLE)hThread,8000);
		if(GetExitCodeThread((HANDLE)hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)hThread,0);


		if(m_pReleaseFunc)
			m_pReleaseFunc();


		return 0;
	}






																				//����ִ�з���ָ��������
	m_RunInServiceModeID=1;
	ServiceEntry();



	return 0;
}


//ֹͣ�������ڷ���ģʽ��RUNģʽ����Ч
void CWinService::Stop_WinService(void)
{
	m_NeedStopID=1;
}


//	Runģʽ�µĹ����߳�
DWORD CWinService::mainService_WorkerThread_RunMode(void *)						
{
	DWORD			TickCount=0;
	HANDLE			hSleepEvent=NULL;

	if(!m_pDelayFunc)
		hSleepEvent=CreateEvent(NULL,FALSE,FALSE,NULL);

	while(m_RunningID_RunMode)
	{
		if(m_pWorkFunc)
			m_pWorkFunc(TickCount);

		if(m_pDelayFunc)
			m_pDelayFunc();
		else if(hSleepEvent)
			WaitForSingleObject(hSleepEvent,1000);
		else 
			Sleep(1000);


		TickCount++;
	}

	if(hSleepEvent)
	{
		CloseHandle(hSleepEvent); 
		hSleepEvent=NULL;
	}

	return 0;
}
