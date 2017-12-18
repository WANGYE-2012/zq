#pragma once

#include "ctp/ThostFtdcMdApi.h"
#include "InstitudeApi.h"
#include "QuotSpi.h"
//#include "TcpLayer.h"
#include "TcpDataConnect.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "CommonStruct.h"

class CQuotSpi;

class CInstitudeMdApi : public CInstitudeApi
{
public:
	static CInstitudeMdApi *CreateInstitudeMdApi(const char *pszFlowPath = "", const bool bIsUsingUdp=false);
    
	///ɾ���ӿڶ�����
	///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
	void Release();
	
	///��ʼ��
	///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
	void Init();
	
	///�ȴ��ӿ��߳̽�������
	///@return �߳��˳�����
	int Join();
	
	///��ȡ��ǰ������
	///@retrun ��ȡ���Ľ�����
	///@remark ֻ�е�¼�ɹ���,���ܵõ���ȷ�Ľ�����
	const char* GetTradingDay() {
		return "";
	};
	
	///ע��ǰ�û������ַ
	///@param pszFrontAddress��ǰ�û������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	void RegisterFront(char* pszFrontAddress);
	
	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	void RegisterSpi(CQuotSpi* pSpi);
	
	///�������顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	int SubscribeMarketData(char* ppInstrumentID[], int nCount);

	///�˶����顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	int UnSubscribeMarketData(char* ppInstrumentID[], int nCount);

	///�û���¼����
	int ReqUserLogin(CThostFtdcReqUserLoginField* pReqUserLoginField, int nRequestID);
	
	///�ǳ�����
	int ReqUserLogout(CThostFtdcUserLogoutField* pUserLogout, int nRequestID);

	void OnFrontConnected();
	void OnFrontDisconnected();
	// �������ݣ������յ�tcp����ʱ����
	BOOL ParseData(char* pData, int nDataLen, DWORD nCmdId, DWORD nModuleId, 
			DWORD nSeq, DWORD nSubSeq, DWORD nUserData1, DWORD nUserData2, DWORD nUserData3, 
			DWORD nUserData4);

protected:
    CInstitudeMdApi();
	virtual ~CInstitudeMdApi();

	void LockMe() {
		::WaitForSingleObject(m_mutex, INFINITE);
	};
	void UnLockMe() {
		::ReleaseMutex(m_mutex);
	};


protected:
    CInstitudeMdApi(const CInstitudeMdApi&);
    CInstitudeMdApi& operator= (const CInstitudeMdApi&);

protected:
	HANDLE							m_eventExit;
	HANDLE							m_mutex;
	CQuotSpi*						m_pSpi;
	CTcpDataConnect*				m_pTcpConnect;
	string							m_strSvrIP;
	WORD							m_wSvrPort;
	CThostFtdcReqUserLoginField		m_ReqUser;
};
