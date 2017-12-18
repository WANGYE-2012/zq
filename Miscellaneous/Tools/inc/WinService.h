// WinService.h: interface for the CWinService class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINSERVICE_H__6F877EE2_D1A7_4DF2_A3EC_DC31134732C8__INCLUDED_)
#define AFX_WINSERVICE_H__6F877EE2_D1A7_4DF2_A3EC_DC31134732C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4996)


#include <windows.h>
#include <winsvc.h>
#include <stdio.h>
#include <conio.h>

#ifdef WIN32
#pragma managed(push,off)
#endif 


typedef void    (*WinService_WORKFUNC)(DWORD);									//���������������߳�ѭ�����ô˻ص��������ڴ˻ص��������������Ĺ�����������ѭ������
typedef void	(*WinService_DELAYFUNC)(void);									//��ʱ�����������߳�ѭ������ʱ��ÿ�ε����깤����������ô˺���������ʱ����Sleep��
typedef void	(*WinService_INITFUNC)(int argc,char *argv[]);					//��ʼ����������������ǰ���ô˺�������Ӧ�ó����ʼ��
typedef void	(*WinService_RELEASEFUNC)(void);								//�ͷź���������ֹͣǰ���ô˺���������Ӧ�ó����ͷ�



#define			WinService_INSTALLONLYSERVICECMD	"installonly"				//��װ����������в���
#define			WinService_INSTALLSERVICECMD		"install"					//��װ����������в���
#define			WinService_UNINSTALLSERVICECMD		"uninstall"					//ж�ط���������в���
#define			WinService_STARTSERVICECMD			"start"						//��������������в���
#define			WinService_STOPSERVICECMD			"stop"						//ֹͣ����������в���
#define			WinService_RUNCMD					"run"						//���У���cmd������
#define         WinService_RUNASYNCMD               "runasyn"                   //���У��첽


class CWinService  
{
public:
	CWinService();
	virtual ~CWinService();


																				//********���溯������Ҫ���������ⲿ(main()��)����********
	static int Start_WinService(		int argc,
										char *argv[],
										char *pServiceName,
										char *pDispName=NULL,
										char *pDescription=NULL,
										int InteractiveID=0,
										WinService_WORKFUNC pWorkFunc=NULL,
										WinService_INITFUNC pInitFunc=NULL,
										WinService_RELEASEFUNC pReleaseFunc=NULL,
										WinService_DELAYFUNC pDelayFunc=NULL);
	static void Stop_WinService(void);											//ֹͣ�������ڷ���ģʽ��RUNģʽ����Ч




																				//********���澲̬�����������ⲿ��������********
	static BOOL IsServiceInstalled(char *ServiceName);							//�ж�һ�������Ƿ��Ѿ���װ�������÷�����ָ��	
	static DWORD GetServiceStatus(char *ServiceName);							//��ȡ����ĵ�ǰ״ֵ̬������ �� ����ĵ�ǰ״ֵ̬����ͷ�ļ����ж���,-1Ϊʧ��
	static BOOL InstallService(char *ServiceName,char *DisplayName,char *FileName,char *Description=NULL,bool bInteractive=false);//��װһ������
	static BOOL UninstallService(char *ServiceName);							//ж��һ������	
	static BOOL MyStartService(char *ServiceName);								//����һ������
	static BOOL MyStopService(char *ServiceName);								//ֹͣһ������
	static BOOL IsRunInServiceMode(void);										//�Ƿ������ڷ���ģʽ��





private:

																				
	static BOOL ServiceEntry(void);												//���Ƿ�����ں������ڷ����������main()�е���
	static void WINAPI WinService_ServiceMain(DWORD argc,LPTSTR *argv);			//��������������SCM����	
	static void WINAPI WinService_MyServiceCtrlHandler(DWORD Opcode);			//����״̬��Ϣ�ַ���������SCM����
	static DWORD mainService_WorkerThread_RunMode(void *arg);

public:
    enum eRunMode
    {
        NotRunning,
        RunInService,
        RunInCmdWindow,
        RunInAsyn
    };

private:

	static char							m_ServiceName[256];						//��Ӧ�ó�����
	static char							m_ServiceDisplayName[256];				//��Ӧ�ó�����
	static char							m_ServiceDescription[256];				//��Ӧ�ó�����
	static int							m_InteractiveID;						//�����Ƿ��ܽ��н��潻��
	static WinService_WORKFUNC			m_pWorkFunc;							//���������������߳�ѭ�����ô˻ص��������ڴ˻ص��������������Ĺ�����������ѭ������
	static WinService_DELAYFUNC			m_pDelayFunc;							//��ʱ�����������߳�ѭ������ʱ��ÿ�ε����깤����������ô˺���������ʱ����Sleep��
	static WinService_INITFUNC			m_pInitFunc;							//��ʼ����������������ǰ���ô˺�������Ӧ�ó����ʼ��
	static WinService_RELEASEFUNC		m_pReleaseFunc;							//�ͷź���������ֹͣǰ���ô˺���������Ӧ�ó����ͷ�

																				//********�����Ƿ�����ʹ�õĲ���********
	static SERVICE_STATUS_HANDLE		hWinService_MyServiceStatusHandle;		//����״̬���


	static int							m_WinService_RunningID;					//�����Ƿ���������
	static int							m_WinService_PauseID;					//�����Ƿ���ͣ
	static int							m_WinService_ReleaseOK;					//�����Ƿ�ɹ�ֹͣ

	static char							m_ModuleName[MAX_PATH];					//Current module name with directory

    static eRunMode                     m_RunMode;
	//static int							m_RunInServiceModeID;					//�Ƿ������ڷ���ģʽ��
	static int							m_RunningID_RunMode;					//��Runģʽ�£�ϵͳ�Ƿ���������
	static int							m_NeedStopID;							//�Ƿ���Ҫֹͣ����

    static HANDLE                       m_hThread_RunAsyn;                      //RunAsynģʽ�µ������߳�


																				//����������������������ʵ�����ǣ�Ŀ���ǽ�ֹ��������͸�ֵ
	CWinService(const CWinService&);											//Ϊ�˴���������
	CWinService& operator= (const CWinService&);								//��ֵ������

};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_WINSERVICE_H__6F877EE2_D1A7_4DF2_A3EC_DC31134732C8__INCLUDED_)
