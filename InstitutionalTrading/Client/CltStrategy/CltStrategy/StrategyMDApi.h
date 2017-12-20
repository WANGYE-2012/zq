#pragma once

#include "TcpConnectSpi.h"
#include "Module-Misc2\KernelStruct.h"
#include "TcpDataConnect.h"
#include "BaseBusinessSpi.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "CommonStruct.h"


class CStrategyMdApi : public CTcpConnectSpi
{
public:
	static CStrategyMdApi *CreateStrategyMdApi();
    
	///ɾ���ӿڶ�����
	///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
	void Release();
	
	///��ʼ��
	///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
	void Init();
	
	///��ȡ��ǰ������
	///@retrun ��ȡ���Ľ�����
	///@remark ֻ�е�¼�ɹ���,���ܵõ���ȷ�Ľ�����
	const std::string GetTradingDay() {
		return "";
	};
	
	///ע��ǰ�û������ַ
	///@param pszFrontAddress��ǰ�û������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	void RegisterFront(char* pszFrontAddress);
	
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
	BOOL OnRecvData(char* pData, int nDataLen, DWORD nCmdId, DWORD nModuleId, 
			DWORD nSeq, DWORD nSubSeq, DWORD nUserData1, DWORD nUserData2, DWORD nUserData3, 
			DWORD nUserData4, char* pSrcPackage);

public:
	void SetBuzSpi(CBaseBusinessSpi* pBuzSpi) {
		LockMe();
		m_pBuzSpi = pBuzSpi;
		UnLockMe();
	};
	CBaseBusinessSpi* GetBuzSpi() {
		LockMe();
		CBaseBusinessSpi* pSpi = m_pBuzSpi;
		UnLockMe();
		return pSpi;
	};
	//void SubscribePkg(UINT nCmdID, HWND hWnd);
	//void UnsubscribePkg(UINT nCmdID, HWND hWnd);
	//void UnsubscribeAllPkg(HWND hWnd);

protected:
    CStrategyMdApi();
	virtual ~CStrategyMdApi();

	void LockMe() {
		::WaitForSingleObject(m_mutex, INFINITE);
	};
	void UnLockMe() {
		::ReleaseMutex(m_mutex);
	};

protected:
	HANDLE							m_mutex;
	CTcpDataConnect*				m_pTcpConnect;
	CBaseBusinessSpi*				m_pBuzSpi;
	string							m_strSvrIP;
	WORD							m_wSvrPort;

	std::map<UINT, std::set<HWND>>	m_mapSubs;
	std::map<std::string, PlatformStru_DepthMarketData>
									m_mapMD;
};
