#pragma once

#include "TcpConnectSpi.h"
#include "Module-Misc2\KernelStruct.h"
#include "TcpDataConnect.h"
#include "BaseBusinessSpi.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "CommonStruct.h"
#include "..\CommonClasses\IndicatorDataEx.h"
#include "..\CommonClasses\StrategyDataEx.h"


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

	///����¼������
	int ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nRequestID);
	
	///������������
	int ReqOrderAction(PlatformStru_InputOrderAction *pInputOrderAction, int nRequestID);

	///�����ѯ����
	int ReqQryOrder(PlatformStru_QryOrder *pQryOrder, int nRequestID);

	///�����ѯ�ɽ�
	int ReqQryTrade(PlatformStru_QryTrade *pQryTrade, int nRequestID);

	///�����ѯͶ���ֲ߳�
	int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition *pQryInvestorPosition, int nRequestID);

	///�����ѯ�ʽ��˻�
	int ReqQryTradingAccount(PlatformStru_QryTradingAccount *pQryTradingAccount, int nRequestID);

	///�����ѯ��Լ
	int ReqQryInstrument(PlatformStru_QryInstrument *pQryInstrument, int nRequestID);

	///�����ѯ����
	int ReqQryDepthMarketData(PlatformStru_QryDepthMarketData *pQryDepthMarketData, int nRequestID);

	///�����ѯͶ���ֲ߳���ϸ
	int ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail *pQryInvestorPositionDetail, int nRequestID);

	///�����ѯͶ���ֲ߳���ϸ
	int ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail *pQryInvestorPositionCombineDetail, int nRequestID);
	
	// �����û���Ϣ
	bool SubsUserMsg(int nUserID);

	// �˶��û���Ϣ
	bool UnsubsUserMsg(int nUserID);

	int ReqUploadStart(UploadStart& start, int nRequestID);
	int ReqUploadEnd(UploadEnd& end, int nRequestID);

	// �����ϴ�ָ�������Ϣ
	int ReqUploadIndicatorBaseInfo(CIndicatorDataEx& indicatorData, int nRequestID);
	// �����ϴ�ָ��DLL��Դ����zip
	int ReqUploadIndicatorFile(const std::string& strIndicatorID, const std::string& strFile, 
			int nRequestID, bool bIsDll);

	// ��������ָ���б�
	int ReqDownloadIndicatorList(int nRequestID);
	// ��������ָ��ָ����Ϣ
	int ReqDownloadIndicatorInfo(const std::string& strIndicatorID, int nRequestID);
	// ��������ָ��DLL��Դ����zip��
	// strFile: �ļ����غ󱣴���ļ�����·����
	int ReqDownloadIndicatorFile(const std::string& strIndicatorID, 
			int nRequestID, bool bIsDll);

	// �����ϴ����Ի�����Ϣ
	int ReqUploadStategyBaseInfo(CStrategyDataEx& strategyData, int nRequestID);
	// �����ϴ�����DLL��Դ����zip
	int ReqUploadStategyFile(const std::string& strStragetyID, const std::string& strFile, 
			int nRequestID, bool bIsDll);

	// �������ز����б�
	int ReqDownloadStategyList(int nRequestID);
	// ��������ָ��������Ϣ
	int ReqDownloadStategyInfo(const std::string& strStrategyID, int nRequestID);
	// �������ز���DLL��Դ����zip��
	// strFile: �ļ����غ󱣴���ļ�����·����
	int ReqDownloadStategyFile(const std::string& strStragetyID, 
			int nRequestID, bool bIsDll);

	// �������ò��Ե����ý���״̬
	int ReqResetStrategyUseStatus(const std::string& strID, int nStatus, int nRequestID);

	// ����ɾ������
	int ReqDeleteStrategy(const std::string& strID, int nRequestID);

	// �������ص�ǰ����Ա���еĲ������з���
	int ReqDownloadStrategyInstanceList(int nRequestID);

	// �������һ���������з���
	int ReqAddStrategyInstance(std::string& strStrategyID, CStrategyDataEx& strategyData, 
			const std::set<std::string>& setInstrument, 
			bool bRunAfterLoad, bool bOfflinePause, bool bStopThenCancelOrder, 
			bool bUseTimer, int nTimerInterval, int nRequestID);
	// �������һ���������з���
	int ReqAddStrategyInstance(std::string& strStrategyID, SStrategyInstance& instance, 
			int nRequestID);
	// ��������һ���������з���
	int ReqResetStrategyInstance(std::string& strStrategyID, SStrategyInstance& instance, 
			int nRequestID);

	// ����ɾ��һ���������з���
	int ReqDeleteStrategyInstance(const std::string& strID, int nRequestID);

	// ����������ֹͣһ���������з���
	int ReqStrategyInstance_RunStop(const std::string& strID, bool bRun, int nRequestID);

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
