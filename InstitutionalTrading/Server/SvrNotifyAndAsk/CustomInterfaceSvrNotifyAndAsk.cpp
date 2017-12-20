// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID

#include "stdafx.h"
#include "CustomInterfaceSvrNotifyAndAsk.h"
#include "SvrNotifyAndAsk.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "CommonMacro.h"
#include "Tools_Ansi.h"

#include "..\\SvrTcp\\Interface_SvrTcp.h"

CCustomInterfaceSvrNotifyAndAsk* CCustomInterfaceSvrNotifyAndAsk::m_pObj=NULL;


CCustomInterfaceSvrNotifyAndAsk::CCustomInterfaceSvrNotifyAndAsk()
{
	CPopMsgWin::Stru_Param lParam;
	lParam.bInServiceID = true;
	lParam.bCreateOKBtn = false;
	lParam.InitWid = 640;
	lParam.InitHei = 480;
	lParam.MaxMsgLen = 50000; //(true,false,true,false,true,true,true,true,100,100,640,480,50000);
    strncpy(lParam.szTitle,"�����潻��ϵͳ��������Ϣ",sizeof(lParam.szTitle)-1);
	mpPopMsgWnd = new CPopMsgWin(lParam);
}

CCustomInterfaceSvrNotifyAndAsk::~CCustomInterfaceSvrNotifyAndAsk()
{
	if(mpPopMsgWnd)
		delete mpPopMsgWnd;
}

CCustomInterfaceSvrNotifyAndAsk& CCustomInterfaceSvrNotifyAndAsk::getObj(void)
{
    g_mutex.lock();
    if(!m_pObj)
        m_pObj=new CCustomInterfaceSvrNotifyAndAsk();
    g_mutex.unlock();
    return *m_pObj;
}


//ע����ѯ�Ľӿں���
void CCustomInterfaceSvrNotifyAndAsk::RegistAsk(const eAskType& neDataType, CInterface_SvrCommon * const npInterface) 
{
	if(!npInterface) return;
	CeasymutexGuard guard(g_mutex);
	g_mapDataType2Interface.insert(std::make_pair(neDataType,npInterface));
}
void CCustomInterfaceSvrNotifyAndAsk::UnregistAsk(const eAskType& neDataType, CInterface_SvrCommon * const npInterface)
{
	CeasymutexGuard guard(g_mutex);
	CTools_Ansi::EraseKeyValueInMultimap<eAskType, CInterface_SvrCommon* const>(g_mapDataType2Interface,neDataType,npInterface);
}
void CCustomInterfaceSvrNotifyAndAsk::UnregistAsk(CInterface_SvrCommon * const npInterface)
{
	CeasymutexGuard guard(g_mutex);
	CTools_Ansi::EraseValueInMultimap<eAskType, CInterface_SvrCommon* const>(g_mapDataType2Interface,npInterface);
}


//ҵ��ģ�鶩���ڲ����ݱ仯֪ͨ����ָ���������ͺ��߳�ID���յ�֪ͨ��Post����Ӧ�߳���
void CCustomInterfaceSvrNotifyAndAsk::SubscribeNotifyEvent(const eEventType& neDataType,const unsigned int& idThread) 
{
	CeasymutexGuard guard(g_mutexEvent);
	if(!CTools_Ansi::IsKeyValueInMultimap<eEventType,unsigned int>(g_mapDataType2Thread,neDataType,idThread))
		g_mapDataType2Thread.insert(pair<eEventType,unsigned int>(neDataType,idThread));
}

//ģ���˶��ڲ����ݱ仯֪ͨ����ָ���������ͺ��߳�ID
void CCustomInterfaceSvrNotifyAndAsk::UnsubscribeNotifyEvent(const eEventType&  neDataType,const unsigned int & idThread) 
{
	CeasymutexGuard guard(g_mutexEvent);
	CTools_Ansi::EraseKeyValueInMultimap<eEventType,unsigned int>(g_mapDataType2Thread,neDataType,idThread);
}

//ģ���˶�ȫ���ڲ����ݱ仯֪ͨ����ָ���߳�ID
void CCustomInterfaceSvrNotifyAndAsk::UnsubscribeNotifyEvent(const unsigned int& idThread) 
{
	CeasymutexGuard guard(g_mutexEvent);
	CTools_Ansi::EraseValueInMultimap<eEventType,unsigned int>(g_mapDataType2Thread,idThread);
}

//����һ��֪ͨ��ͬʱ�㲥������������
CF_ERROR CCustomInterfaceSvrNotifyAndAsk::NotifyEvent(const Stru_NotifyEvent& nDataChangeEvent) 
{
	return NotifyEvent2(nDataChangeEvent,0);
}
//����һ��֪ͨ���㲥������������ʱ�ų�ָ��������
CF_ERROR CCustomInterfaceSvrNotifyAndAsk::NotifyEvent2(const Stru_NotifyEvent& nDataChangeEvent,int ExcludeSocket)
{
	CeasymutexGuard guard(g_mutexEvent);

	//��֪ͨ����������ģ��
	multimap<eEventType,unsigned int>::iterator it2lower=g_mapDataType2Thread.lower_bound(nDataChangeEvent.meEventType);
	multimap<eEventType,unsigned int>::iterator it2upper=g_mapDataType2Thread.upper_bound(nDataChangeEvent.meEventType);
	for(;it2lower!=it2upper;it2lower++)
	{
		unsigned int EventParamID;
		if(CInterface_SvrTcp::getObj().getEventParamObj().CreateEventParam(EventParamID,NULL,NULL,&nDataChangeEvent,sizeof(nDataChangeEvent),0))
		{
			int iCount = 0;
			while(!PostThreadMessage(it2lower->second,WM_COMMAND,WndCmd_NotifyEventArrival,(LPARAM)EventParamID))
			{		
				Sleep(10);
				NOTIFY_LOG("PostTheadMessage Faild: Event ID %d Retry Count %d,THread %d Error %d",EventParamID,iCount+1,it2lower->second,GetLastError());
				if((++iCount) == 4)
					break;							
			}
		}
	}

	//Ȼ��֪ͨ����������
	set<int> ServerSockets=CInterface_SvrTcp::getObj().GetAllServerSockets();
	for(set<int>::const_iterator it=ServerSockets.begin();it!=ServerSockets.end();it++)
	{
		if(*it==ExcludeSocket) continue;
		CInterface_SvrTcp::getObj().SendPkgData(*it,CMDID_NotifyAndAsk_BroadcastNotify,&nDataChangeEvent,sizeof(nDataChangeEvent),0);
	}

	return CF_ERROR_SUCCESS;
}

//��֤�û�Ȩ��
bool CCustomInterfaceSvrNotifyAndAsk::Ask_ValidateUserPrivilege(const int& nUserID,const ePrivilege& nPrivilege ) 
{
    CF_ERROR lRet = Ask(AskType_QueryPrivilege,AskOperationTypeQuery,nUserID,(const char*)&nPrivilege);
    if(lRet != CF_ERROR_SUCCESS)			
        return false;

    return true;
}

//��ѯ�������ʸ���ģ��ָ�������Ƿ����ִ�С�����CF_ERROR_SUCCESS��ʾ����
//һƱ���ԭ��	
CF_ERROR CCustomInterfaceSvrNotifyAndAsk::Ask(const eAskType& neDataType,
												const eAskOperationType& eOp,
												int nID,
												const char * npDataBuf)
{
	CeasymutexGuard guard(g_mutex);

	CF_ERROR lRet = CF_ERROR_SUCCESS;

	multimap<eAskType, CInterface_SvrCommon*const>::iterator itlower=g_mapDataType2Interface.lower_bound(neDataType);
	multimap<eAskType, CInterface_SvrCommon*const>::iterator itupper=g_mapDataType2Interface.upper_bound(neDataType);
	for(;itlower!=itupper;itlower++)
	{
		CInterface_SvrCommon * const lpInterface = itlower->second;
		if(lpInterface)
		{	
			lRet = lpInterface->ResponseToAsk(neDataType,eOp,nID,npDataBuf);
			if(lRet != CF_ERROR_SUCCESS)
				break;
		}
	}
	return lRet;
}

//��ѯ������ѯ�ʸ���ģ��ĳ�����⡣����CF_ERROR_SUCCESS��ʾYes�������ʾNo��ͨ��ʱ��npRlt�������������͵�ָ�룬����Ӧ���߷������ݡ�����ע��ģ���ResponseToAsk2()
//һƱͨ��ԭ��
CF_ERROR CCustomInterfaceSvrNotifyAndAsk::Ask2(const eAskType& neDataType,const eAskOperationType& eOp,int nID,const char * npDataBuf,void* pRlt)
{
	CeasymutexGuard guard(g_mutex);

	CF_ERROR lRet = CF_ERROR_FAILURE;

	multimap<eAskType, CInterface_SvrCommon*const>::iterator itlower=g_mapDataType2Interface.lower_bound(neDataType);
	multimap<eAskType, CInterface_SvrCommon*const>::iterator itupper=g_mapDataType2Interface.upper_bound(neDataType);
	for(;itlower!=itupper;itlower++)
	{
		CInterface_SvrCommon * const lpInterface = itlower->second;
		if(lpInterface)
		{	
			lRet = lpInterface->ResponseToAsk2(neDataType,eOp,nID,npDataBuf,pRlt);
			if(lRet == CF_ERROR_SUCCESS)
				break;
		}
	}
	return lRet;
}

//���һ������
CF_ERROR  CCustomInterfaceSvrNotifyAndAsk::PushErrorInfo(const sErrorInfo& nErrorInfo,bool bSendMgrClient,bool bShow ) 
{
	if(nErrorInfo.meErrorType == FirstLevelError ||
		nErrorInfo.meErrorType == SecondLevelError ||
		nErrorInfo.meErrorType == ThirdLevelError)
	{
		mEventMutexError.lock();
		mvecError.push_back(nErrorInfo);
		mEventMutexError.unlock();
	}

	NOTIFY_LOG("%d level Error Come: %s ",nErrorInfo.meErrorType+1,nErrorInfo.msError);
	if(bSendMgrClient)
	{
		Stru_NotifyEvent lEvent(EventTypeErrorCome,0);
		lEvent.AddBufer((void*)&nErrorInfo,sizeof(nErrorInfo));
		NotifyEvent(lEvent);
	}

	if(bShow)
	{
		 if(mpPopMsgWnd )
		 {
			 mpPopMsgWnd->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
				 CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
				 nErrorInfo.msError);
		 }
	}

	return CF_ERROR_SUCCESS;
}
//��ȡ���еĴ���
 CF_ERROR CCustomInterfaceSvrNotifyAndAsk::GetAllError(std::vector<sErrorInfo>& nVecError) 
 {	
	 mEventMutexError.lock();
	 nVecError.clear();
	 nVecError = mvecError;
	 mEventMutexError.unlock();
	 return CF_ERROR_SUCCESS;
 }

	 
void CCustomInterfaceSvrNotifyAndAsk::ClearAllError()
{
	mEventMutexError.lock();
	mvecError.clear();
	mEventMutexError.unlock();
}