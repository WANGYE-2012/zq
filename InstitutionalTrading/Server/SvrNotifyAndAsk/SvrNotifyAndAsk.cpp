// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#include "stdafx.h"
#include <windows.h>
#include "SvrNotifyAndAsk.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "CommonMacro.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "Interface_SvrNotifyAndAsk.h"
#include "CustomInterfaceSvrNotifyAndAsk.h"
#include "Tools_Win32.h"




#define WRITELOGID_SvrNotifyAndAsk



//ȫ�ֻ�����
Ceasymutex			g_mutex;
Ceasymutex			g_mutexEvent;
//�̲߳���
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;

//�������ݰ����߳�map, cmdid -> idThread��
//�յ����ݰ�ʱ����PostThreadMessage()�����ݰ����͸����ĵ��̡߳�
//wParam=WndCmd_YourPkgArrival2
//lParam=EventParamID��EventParam��pEventData�ǰ����ݣ�Stru_UniPkgHead + ���� + ��β(4)��userint=hSocket
 multimap<eEventType,unsigned int> g_mapDataType2Thread;

 //��ѯ�ӿ�map
multimap<eAskType, CInterface_SvrCommon* const> g_mapDataType2Interface;

DWORD ThreadWorker(void *arg);


//����һ�����յ������ݰ�
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);



//ģ���ʼ�������������̲߳���SvrTcp���ĸ���Ȥ�����ݰ�
SVRNOTIFYANDASK_API void InitFunc(void)
{
	//���������߳�
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);
}

//ģ��������ͷ���Դ���رչ����߳�
SVRNOTIFYANDASK_API void ReleaseFunc(void)
{
	if(g_hThread)
	{
		//�˶����ݰ�
		CInterface_SvrTcp::getObj().UnsubscribeAllPkg(g_idThread);

		//����WM_QUIT֪ͨ�߳̽���
		PostThreadMessage(g_idThread,WM_QUIT,0,0);

		//�ȴ��߳��˳�
		CTools_Win32::StopThread((HANDLE)g_hThread,8);
		g_hThread=NULL;
		g_idThread=0;
	}
}

//��ΪAllocTmpStackBuf������ջ�ռ�ֻ���ں����в����Զ��ͷţ�������DealPackage����
void DealPackage(unsigned int EventParamID,int& PkgLen,int& hSocket)
{
	AllocTmpStackBuf(TmpPkg,PkgLen);
	if(TmpPkg.m_pbuf&&
		CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,TmpPkg.m_pbuf,&PkgLen,PkgLen,&hSocket)&&
		Stru_UniPkgHead::IsValidPkg(TmpPkg.m_pbuf,PkgLen))
	{
		Stru_UniPkgHead& PkgHead=*((Stru_UniPkgHead*)TmpPkg.m_pbuf);
		void* pPkgData=(char*)TmpPkg.m_pbuf+sizeof(Stru_UniPkgHead);
		
		//�������ݰ��������������ݰ�
		ProcessOneUniPkg_InThread(hSocket,PkgHead,pPkgData);	
	}
}
//�����߳�
DWORD ThreadWorker(void *arg)
{
	MSG Msg;

	//���ı��̸߳���Ȥ�����ݰ�
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_NotifyAndAsk_BroadcastNotify,g_idThread);

	while(GetMessage(&Msg,NULL,0,0))
	{
		if(Msg.message==WM_COMMAND&&Msg.wParam==WndCmd_YourPkgArrival2)
		{
			//���ݰ��Ĵ���ID
			unsigned int EventParamID=(unsigned int)Msg.lParam;
			//���ݰ�����
			int PkgLen;
			//Socket���
			int hSocket;
			if(CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,NULL,&PkgLen,0,NULL))
			{
				DealPackage(EventParamID,PkgLen,hSocket);

				//�ͷ�EventParam
				CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
			}

		}		
		
	}
	return 0;
}

//����һ��SvrTcp���͹��������ݰ�
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	//-----------------------------------------------------------------------------------
	//	����������ݰ��������֣��������ݰ�
	//-----------------------------------------------------------------------------------
	switch(PkgHead.cmdid)
	{
		case CMDID_NotifyAndAsk_BroadcastNotify:
		{
			//�յ������������㲥������֪ͨ��Ϣ
			if(PkgHead.len==sizeof(Stru_NotifyEvent))
			{
				CCustomInterfaceSvrNotifyAndAsk::getObj().NotifyEvent2(*((const Stru_NotifyEvent*)pPkgData),hSocket);
			}
			break;
		}
	}
}