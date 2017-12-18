// MasterPipeService.h: interface for the CMasterPipeService class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASTERPIPESERVICE_H__77B25F66_FC48_449E_9D40_B493533A6884__INCLUDED_)
#define AFX_MASTERPIPESERVICE_H__77B25F66_FC48_449E_9D40_B493533A6884__INCLUDED_


#include "AppPipeManager.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMasterPipeService  
{
protected:
	BOOL				m_bInitOK;
	BOOL				m_bHasSubscribeResponse;
	LPVOID				m_lpOwner;
	CAppPipeManager		m_appPipeManager;

public:
	static CMasterPipeService* GetOnlyOneInstance();
	static void ReleaseOnlyOneInstance();
	// �����ͻ�����Ϣ���̹߳���
	static DWORD WINAPI Proc_MasterPipe(LPVOID lpvParam); 
	// �������ݸ��ͻ��˵��̹߳���
	static DWORD WINAPI Proc_PushToAppPipe(LPVOID lpvParam); 
	// �Ѹ���Ԫ���ݺϲ�һ����Ч��Э����Ϣ
	// ����pDataBuff������������Ϣ�Ļ���
	// ����pAppendDataBuff����Ҫ������Э��ͷ������ݣ����ݳ���ΪdwAppendDataLen
	// ����ֵ��
	//     0����ʾ�ϲ�ʧ��
	//     >0����ʾ�ϲ��ɹ�����ֵΪ��Ϣ�ĳ���
	static DWORD MergePackage(BYTE* pDataBuff, DWORD dwDataBuffSize, ISessionIDType SessionID, 
				DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum, 
				BYTE* pAppendDataBuff=NULL, DWORD dwAppendDataLen=0);
	static BOOL MergeAndSend(string& strAppPipe, BYTE* pDataBuff, DWORD dwDataBuffSize, 
				ISessionIDType SessionID, DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum, 
				BYTE* pAppendDataBuff=NULL, DWORD dwAppendDataLen=0);

	static BOOL MergeAndSend_Accounts(string& strAppPipe, 
				ISessionIDType SessionID, DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum);
	static BOOL MergeAndSend_ContractIDInfos(string& strAppPipe, 
				ISessionIDType SessionID, DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum);
	static BOOL MergeAndSend_AllOrders(string& strAppPipe, 
				ISessionIDType SessionID, DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum);
	static BOOL MergeAndSend_AllPositions(string& strAppPipe, 
				ISessionIDType SessionID, DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum);
	static BOOL MergeAndSend_ExchangeDateTime(string& strAppPipe, string& strExchangeID, 
									  ISessionIDType SessionID, 
									  DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum);
	static BOOL MergeAndSend_DepthMarket(string& strAppPipe, string& strInstrumentID, 
									  ISessionIDType SessionID, 
									  DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum);

	static int RspCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);

public:
	BOOL IsInitOK() {
		return m_bInitOK;
	};
	BOOL HasSubscribeResponse() {
		return m_bHasSubscribeResponse;
	};

	CAppPipeManager& GetAppPipeManager() {
		return m_appPipeManager;
	};

	BOOL Init(LPVOID lpOwner=NULL);
	void SubscribeResponse();
	// ���ڴ����¼�Ĺ��̣������û����������
	BOOL Login(ILOGINDATA& tLogin);

	// �����������¼��Ϣ
	BOOL ParseLogin(HANDLE hMasterPipe, IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen); 

	// ���������������Ϣ
	BOOL ParseMsgType(IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen);
	// ����������A����Ϣ����������¼��Ϣ
	BOOL ParseMsgType_A(IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen);
	// ����������C����Ϣ
	BOOL ParseMsgType_C(IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen);
	// ����������E����Ϣ
	BOOL ParseMsgType_E(IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen);
	// ����������G����Ϣ
	BOOL ParseMsgType_G(IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen);

public:
	CMasterPipeService();
	virtual ~CMasterPipeService();

};

#endif // !defined(AFX_MASTERPIPESERVICE_H__77B25F66_FC48_449E_9D40_B493533A6884__INCLUDED_)
