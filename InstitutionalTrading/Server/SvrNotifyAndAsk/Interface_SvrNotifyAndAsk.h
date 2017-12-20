#pragma once

//************************************************************************************************
//	֪ͨ����ѯ�ӿ�
//		1. ֪ͨ�ӿ�ΪNotifyEvent()���첽��ͬʱ�㲥��������������
//		2. ��ѯ�ӿ���Ask()��Ask2()��ͬ����ֻ���ʱ���������ģ�顣
//************************************************************************************************

#include "Interface_SvrCommon.h"
#include "CommonDataChangeEvent.h"
#include "CommonStruct.h"
#include "socketBase.h"
#include <map>
#include <vector>
#ifndef SVRNOTIFYANDASK_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRNOTIFYANDASK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRNOTIFYANDASK_EXPORTS
#define SVRNOTIFYANDASK_API __declspec(dllexport)
#else
#define SVRNOTIFYANDASK_API __declspec(dllimport)
#endif
#endif


class SVRNOTIFYANDASK_API CInterface_SvrNotifyAndAsk : public CInterface_SvrCommon
{
public:
    static CInterface_SvrNotifyAndAsk& getObj(void);

public:
	//��֤�û�Ȩ��
	virtual bool Ask_ValidateUserPrivilege(const int& nUserID,const ePrivilege& nPrivilege )  = 0;

public:
	//��ѯ�������ʸ���ģ��ָ�������Ƿ����ִ�С�����CF_ERROR_SUCCESS��ʾ���ԡ�һƱ���ԭ�򡣵���ע��ģ���ResponseToAsk()
	virtual CF_ERROR Ask(const eAskType& neDataType,const eAskOperationType& eOp,int nID,const char * npDataBuf) = 0;
	//��ѯ������ѯ�ʸ���ģ��ĳ�����⡣����CF_ERROR_SUCCESS��ʾYes�������ʾNo��ͨ��ʱ��npRlt�������������͵�ָ�룬����Ӧ���߷������ݡ�һƱͨ��ԭ�򡣵���ע��ģ���ResponseToAsk2()
	virtual CF_ERROR Ask2(const eAskType& neDataType,const eAskOperationType& eOp,int nID,const char * npDataBuf,void* pRlt=NULL) = 0;

	//ע����ѯ�Ľӿں���
	virtual void RegistAsk(const eAskType& neDataType, CInterface_SvrCommon * const npInterface)  = 0;
	virtual void UnregistAsk(const eAskType& neDataType, CInterface_SvrCommon * const npInterface)  = 0;
	virtual void UnregistAsk(CInterface_SvrCommon * const npInterface)  = 0;

	

public:
	//֪ͨһ���¼�
	virtual CF_ERROR NotifyEvent(const Stru_NotifyEvent& nDataChangeEvent) = 0;

	//ҵ��ģ�鶩���ڲ����ݱ仯֪ͨ����ָ���������ͺ��߳�ID���յ�֪ͨ��Post����Ӧ�߳���
	virtual void SubscribeNotifyEvent(const eEventType& neDataType,const unsigned int& idThread)  = 0;
	virtual void UnsubscribeNotifyEvent(const eEventType& neDataType,const unsigned int& idThread) = 0;
	virtual void UnsubscribeNotifyEvent(const unsigned int& idThread) = 0;

public:
	//���һ������
	virtual CF_ERROR PushErrorInfo(const sErrorInfo& nErrorInfo,bool bSendMgrClient = true,bool bShow = true) = 0;
	//��ȡ���еĴ���
	virtual CF_ERROR GetAllError(std::vector<sErrorInfo>& nVecError) = 0;
	//������д���
	virtual void ClearAllError() = 0;
};







