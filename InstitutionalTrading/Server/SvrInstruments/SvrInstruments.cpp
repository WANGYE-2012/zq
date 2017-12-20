// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include "SvrInstruments.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "CommonMacro.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "Interface_SvrInstruments.h"
#include <vector>
#include <map>
//-----------------------------------------------------------------------------------
//	import Tools4dll��
//-----------------------------------------------------------------------------------
#ifdef _DEBUG
#pragma comment(lib, "Tools4dllD.lib")											
#else 
#pragma comment(lib, "Tools4dll.lib")											
#endif

//-----------------------------------------------------------------------------------
//	����import��ģ����Ҫ���õ�����ģ��
//-----------------------------------------------------------------------------------
#pragma comment(lib, "SvrTcp.lib")



#define WRITELOGID_SvrTemplate




//ȫ�ֻ�����
Ceasymutex			g_mutex;

//�̲߳���
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;
DWORD ThreadWorker(void *arg);


//����һ�����յ������ݰ�
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);



//ģ���ʼ�������������̲߳���SvrTcp���ĸ���Ȥ�����ݰ�
SVRINSTRUMENTS_API void InitFunc(void)
{
	//���������߳�
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);
	
	//���涩�ı��̸߳���Ȥ�����ݰ�
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_QueryAllInstruments_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_PushAllInstruments_Req,g_idThread);
}

//ģ��������ͷ���Դ���رչ����߳�
SVRINSTRUMENTS_API void ReleaseFunc(void)
{
	if(g_hThread)
	{
		//�˶����ݰ�
		CInterface_SvrTcp::getObj().UnsubscribeAllPkg(g_idThread);

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
}

//�����߳�
DWORD ThreadWorker(void *arg)
{
	MSG Msg;
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
				//������ʱ�ռ䲢��ȡ���ݰ�
				AllocTmpStackBuf(TmpPkg,PkgLen,10*1024);
				if(TmpPkg.m_pbuf&&
					CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,TmpPkg.m_pbuf,&PkgLen,PkgLen,&hSocket)&&
					Stru_UniPkgHead::IsValidPkg(TmpPkg.m_pbuf,PkgLen))
				{
					Stru_UniPkgHead& PkgHead=*((Stru_UniPkgHead*)TmpPkg.m_pbuf);
					void* pPkgData=(char*)TmpPkg.m_pbuf+sizeof(Stru_UniPkgHead);

					//�������ݰ��������������ݰ�
					ProcessOneUniPkg_InThread(hSocket,PkgHead,pPkgData);
				}

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
		case CMDID_QueryAllInstruments_Req:
		{
			BREAK_ON_DIFF_LEN(PkgHead.len,0);

			std::vector<InstrumentField> lVecFiled;
			std::map<std::string,InstrumentField> lMapFiled;
			g_mutex.lock();
			CInterface_SvrInstruments::getObj().GetAllInstruments(lMapFiled);
			g_mutex.unlock();

			//����
			std::map<std::string,InstrumentField>::iterator lIter = lMapFiled.begin();
			while(lIter != lMapFiled.end())
			{
				lVecFiled.push_back(lIter->second);
				lIter++;
			}

			//���ͻش�
			if(lVecFiled.size() != 0)
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,CMDID_QueryAllInstruments_Rsp,
					&lVecFiled[0],sizeof(InstrumentField)*lVecFiled.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
			else
			{
				const char * lErrorString = FormatErrorCode(CF_ERROR_NO_INSTRUMENTS);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, CMDID_QueryAllInstruments_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_NO_INSTRUMENTS);		
			}

			break;
		}
		case CMDID_PushAllInstruments_Req:
		{
			if(PkgHead.len == 0)
				break;	

			BREAK_ON_DIFF_LEN(PkgHead.len % sizeof(InstrumentField),0);

			InstrumentField * p = (InstrumentField*) pPkgData;
			int n = PkgHead.len / sizeof(InstrumentField);
			for(int i = 0 ; i < n; i++)
			{
				CInterface_SvrInstruments::getObj().AppendInstrument( *(p + i*sizeof(InstrumentField) ) );
			}


			CInterface_SvrTcp::getObj().SendPkgData(hSocket,CMDID_PushAllInstruments_Req,
				0,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
			break;
		}
		default: break;
	}
}