#pragma once


#include "CommonDataChangeEvent.h"
#include "CommonStruct.h"
#include "socketBase.h"
#include <map>
#include <vector>
#include "Interface_SvrNotifyAndAsk.h"
#include "popmsgwin.h"




class CCustomInterfaceSvrNotifyAndAsk : public CInterface_SvrNotifyAndAsk
{
public:
    static CCustomInterfaceSvrNotifyAndAsk& getObj(void);

public:
	//��֤�û�Ȩ��
	virtual bool Ask_ValidateUserPrivilege(const int& nUserID,const ePrivilege& nPrivilege ) ;

public:
	//��ѯ�������ʸ���ģ��ָ�������Ƿ����ִ�С�����CF_ERROR_SUCCESS��ʾ���ԡ�һƱ���ԭ��	
	virtual CF_ERROR Ask(const eAskType& neDataType,const eAskOperationType& eOp,int nID,const char * npDataBuf) ;
	//��ѯ������ѯ�ʸ���ģ��ĳ�����⡣����CF_ERROR_SUCCESS��ʾYes�������ʾNo��һƱͨ��ԭ��ͨ��ʱ��npRlt�������������͵�ָ�룬����Ӧ���߷������ݡ�����ע��ģ���ResponseToAsk2()
	virtual CF_ERROR Ask2(const eAskType& neDataType,const eAskOperationType& eOp,int nID,const char * npDataBuf,void* pRlt=NULL);

	//ע����ѯ�Ľӿں���
	virtual void RegistAsk(const eAskType& neDataType,CInterface_SvrCommon * const npInterface) ;
	virtual void UnregistAsk(const eAskType& neDataType, CInterface_SvrCommon * const npInterface);
	virtual void UnregistAsk(CInterface_SvrCommon * const npInterface);



public:
	//����һ��֪ͨ��ͬʱ�㲥������������
	virtual CF_ERROR NotifyEvent(const Stru_NotifyEvent& nDataChangeEvent) ;
	//����һ��֪ͨ���㲥������������ʱ�ų�ָ��������
	virtual CF_ERROR NotifyEvent2(const Stru_NotifyEvent& nDataChangeEvent,int ExcludeSocket);

	//ҵ��ģ�鶩���ڲ����ݱ仯֪ͨ����ָ���������ͺ��߳�ID���յ�֪ͨ��Post����Ӧ�߳���
	void SubscribeNotifyEvent(const eEventType& neEventType,const unsigned int & idThread) ;
	void UnsubscribeNotifyEvent(const eEventType& neEventType,const unsigned int& idThread) ;
	void UnsubscribeNotifyEvent(const unsigned int& idThread) ;

public:
	//���һ������
	virtual CF_ERROR PushErrorInfo(const sErrorInfo& nErrorInfo,bool bSendMgrClient = true,bool bShow = true) ;
	//��ȡ���еĴ���
	virtual CF_ERROR GetAllError(std::vector<sErrorInfo>& nVecError) ;
	//������д���
	virtual void ClearAllError();


private:
	
    CCustomInterfaceSvrNotifyAndAsk();
    virtual ~ CCustomInterfaceSvrNotifyAndAsk();
    static CCustomInterfaceSvrNotifyAndAsk* m_pObj;

	Ceasymutex mEventMutex;

	Ceasymutex mEventMutexError;
	std::vector<sErrorInfo> mvecError;

	CPopMsgWin * mpPopMsgWnd;
};







