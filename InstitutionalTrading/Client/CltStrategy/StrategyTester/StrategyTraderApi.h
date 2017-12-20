#pragma once

#include "TcpConnectSpi.h"
#include "Module-Misc2\KernelStruct.h"
#include "TcpDataConnect.h"
#include "BaseBusinessSpi.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "CommonStruct.h"


class CStrategyTraderApi : public CTcpConnectSpi
{
public:
	///����StrategyTraderApi
	static CStrategyTraderApi* CreateStrategyTraderApi();
	
	///ɾ���ӿڶ�����
	///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
	void Release();
	
	///��ʼ��
	///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
	void Init();
		
	///��ȡ��ǰ������
	///@retrun ��ȡ���Ľ�����
	///@remark ֻ�е�¼�ɹ���,���ܵõ���ȷ�Ľ�����
	const string GetTradingDay() {
		return m_strTradingDay;
	};
	
	///ע��ǰ�û������ַ
	///@param pszFrontAddress��ǰ�û������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	void RegisterFront(char *pszFrontAddress);
	
	/////ע��ص��ӿ�
	/////@param pSpi �����Իص��ӿ����ʵ��
	//void RegisterSpi(CTraderSpi* pSpi);
	
	///�û���¼����
	int ReqUserLogin(const string& strUserID, const string& strPasswd, int nRequestID);	

	///�ǳ�����
	int ReqUserLogout(int nRequestID);

	///�����ѯ��Լ
	int ReqQryInstrument(PlatformStru_QryInstrument *pQryInstrument, int nRequestID);

	///�����ѯ����
	int ReqQryDepthMarketData(PlatformStru_QryDepthMarketData *pQryDepthMarketData, int nRequestID);

	///�����ѯ��Լ��֤����
	int ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID);

	///�����ѯ��Լ��������
	int ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID);

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

protected:
	CStrategyTraderApi();
	virtual ~CStrategyTraderApi();

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
	string							m_strUserID;
	string							m_strPasswd;
	string							m_strTradingDay;

	std::map<std::string, PlatformStru_InstrumentInfo>
									m_mapInstrument;

	sTraderLoginRsp					m_UserLogin;

};
