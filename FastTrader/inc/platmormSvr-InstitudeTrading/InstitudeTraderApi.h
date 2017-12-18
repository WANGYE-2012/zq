#pragma once

#include "InstitudeApi.h"
#include "Module-Misc2\KernelStruct.h"
#include "TraderSpi.h"
//#include "TcpLayer.h"
#include "TcpDataConnect.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "CommonStruct.h"

class CTraderSpi;

class CInstitudeTraderApi : public CInstitudeApi
{
public:
	typedef struct {
		int							FrontID;
		int							SessionID;
		TThostFtdcBrokerIDType		BrokerID;
		TThostFtdcInvestorIDType	InvestorID;
		TThostFtdcOrderRefType		OrderRef;
		SYSTEMTIME					DateTime;
	} ORDERTIME;

public:
	///����TraderApi
	///@param pszFlowPath ����������Ϣ�ļ���Ŀ¼��Ĭ��Ϊ��ǰĿ¼
	///@return ��������UserApi
	//modify for udp marketdata
	static CInstitudeTraderApi* CreateInstitudeTraderApi(const char *pszFlowPath = "", const bool bIsUsingUdp=false);
	
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
		return m_UserLogin.mTradingDay;
	};
	
	///ע��ǰ�û������ַ
	///@param pszFrontAddress��ǰ�û������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	void RegisterFront(char *pszFrontAddress);
	
	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	void RegisterSpi(CTraderSpi* pSpi);
	
	///����˽������
	///@param nResumeType ˽�����ش���ʽ  
	///        THOST_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        THOST_TERT_RESUME:���ϴ��յ�������
	///        THOST_TERT_QUICK:ֻ���͵�¼��˽����������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ�˽���������ݡ�
	void SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType);
	
	///���Ĺ�������
	///@param nResumeType �������ش���ʽ  
	///        THOST_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        THOST_TERT_RESUME:���ϴ��յ�������
	///        THOST_TERT_QUICK:ֻ���͵�¼�󹫹���������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ������������ݡ�
	void SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType);

	///�ͻ�����֤����
	int ReqAuthenticate(CThostFtdcReqAuthenticateField* pReqAuthenticateField, int nRequestID);

	///�û���¼����
	int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID);
	

	///�ǳ�����
	int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID);

	///�û������������
	int ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate *pUserPasswordUpdate, int nRequestID);

	///�ʽ��˻������������
	int ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate *pTradingAccountPasswordUpdate, int nRequestID);

	///����¼������
	int ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nRequestID);

	///Ԥ��¼������
	int ReqParkedOrderInsert(PlatformStru_ParkedOrder *pParkedOrder, int nRequestID);

	///Ԥ�񳷵�¼������
	int ReqParkedOrderAction(PlatformStru_ParkedOrderAction *pParkedOrderAction, int nRequestID);

	///������������
	int ReqOrderAction(PlatformStru_InputOrderAction *pInputOrderAction, int nRequestID);

	///��ѯ��󱨵���������
	int ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume *pQueryMaxOrderVolume, int nRequestID);

	///Ͷ���߽�����ȷ��
	int ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID);

	///����ɾ��Ԥ��
	int ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder *pRemoveParkedOrder, int nRequestID);

	///����ɾ��Ԥ�񳷵�
	int ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction *pRemoveParkedOrderAction, int nRequestID);

	///�����ѯ����
	int ReqQryOrder(PlatformStru_QryOrder *pQryOrder, int nRequestID);

	///�����ѯ�ɽ�
	int ReqQryTrade(PlatformStru_QryTrade *pQryTrade, int nRequestID);

	///�����ѯͶ���ֲ߳�
	int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition *pQryInvestorPosition, int nRequestID);

	///�����ѯ�ʽ��˻�
	int ReqQryTradingAccount(PlatformStru_QryTradingAccount *pQryTradingAccount, int nRequestID);

	///�����ѯͶ����
	int ReqQryInvestor(PlatformStru_QryInvestor *pQryInvestor, int nRequestID);

	///�����ѯ���ױ���
	int ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID);

	///�����ѯ��Լ��֤����
	int ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID);

	///�����ѯ��Լ��������
	int ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID);

	///�����ѯ������
	int ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID);

	///�����ѯ��Լ
	int ReqQryInstrument(PlatformStru_QryInstrument *pQryInstrument, int nRequestID);

	///�����ѯ����
	int ReqQryDepthMarketData(PlatformStru_QryDepthMarketData *pQryDepthMarketData, int nRequestID);

	///�����ѯͶ���߽�����
	int ReqQrySettlementInfo(PlatformStru_QrySettlementInfo *pQrySettlementInfo, int nRequestID);

	///�����ѯת������
	int ReqQryTransferBank(PlatformStru_QryTransferBank *pQryTransferBank, int nRequestID);

	///�����ѯͶ���ֲ߳���ϸ
	int ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail *pQryInvestorPositionDetail, int nRequestID);

	///�����ѯ�ͻ�֪ͨ
	int ReqQryNotice(PlatformStru_QryNotice *pQryNotice, int nRequestID);

	///�����ѯ������Ϣȷ��
	int ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm *pQrySettlementInfoConfirm, int nRequestID);

	///�����ѯͶ���ֲ߳���ϸ
	int ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail *pQryInvestorPositionCombineDetail, int nRequestID);

	///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
	int ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField *pQryCFMMCTradingAccountKey, int nRequestID);

	///�����ѯ�ֵ��۵���Ϣ
	int ReqQryEWarrantOffset(CThostFtdcQryEWarrantOffsetField *pQryEWarrantOffset, int nRequestID);

	///�����ѯת����ˮ
	int ReqQryTransferSerial(CThostFtdcQryTransferSerialField *pQryTransferSerial, int nRequestID);

	///�����ѯ����ǩԼ��ϵ
	int ReqQryAccountregister(CThostFtdcQryAccountregisterField *pQryAccountregister, int nRequestID);

    ///�����ѯ����
    int ReqQryExchangeRate(PlatformStru_QryExchangeRate *pQryExchangeRate, int nRequestID);

	///�����ѯǩԼ����
	int ReqQryContractBank(CThostFtdcQryContractBankField *pQryContractBank, int nRequestID);

	///�����ѯԤ��
	int ReqQryParkedOrder(PlatformStru_QryParkedOrder *pQryParkedOrder, int nRequestID);

	///�����ѯԤ�񳷵�
	int ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction *pQryParkedOrderAction, int nRequestID);

	///�����ѯ����֪ͨ
	int ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID);

	///�����ѯ���͹�˾���ײ���
	int ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID);

	///�����ѯ���͹�˾�����㷨
	int ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID);

	///�ڻ����������ʽ�ת�ڻ�����
	int ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer *pReqTransfer, int nRequestID);

	///�ڻ������ڻ��ʽ�ת��������
	int ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer *pReqTransfer, int nRequestID);

	///�ڻ������ѯ�����������
	int ReqQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, int nRequestID);
	
	// �����û���Ϣ
	bool SubsUserMsg(int nUserID);
	// �˶��û���Ϣ
	bool UnsubsUserMsg(int nUserID);

	void OnFrontConnected();
	void OnFrontDisconnected();
	// �������ݣ������յ�tcp����ʱ����
	BOOL ParseData(char* pData, int nDataLen, DWORD nCmdId, DWORD nModuleId, 
			DWORD nSeq, DWORD nSubSeq, DWORD nUserData1, DWORD nUserData2, DWORD nUserData3, 
			DWORD nUserData4);

protected:
	CInstitudeTraderApi();
	virtual ~CInstitudeTraderApi();

	void LockMe() {
		::WaitForSingleObject(m_mutex, INFINITE);
	};
	void UnLockMe() {
		::ReleaseMutex(m_mutex);
	};

	void Convert(sTraderLoginRsp& loginRsp, CThostFtdcRspUserLoginField& login);
	BOOL WriteOrderTimeToFile(std::string& strFile, ORDERTIME& item);

protected:
	HANDLE							m_eventExit;
	HANDLE							m_mutex;
	CTraderSpi*						m_pSpi;
	CTcpDataConnect*				m_pTcpConnect;
	string							m_strSvrIP;
	WORD							m_wSvrPort;
	CThostFtdcReqUserLoginField		m_ReqUser;
	PlatformStru_UserPasswordUpdate m_UserPasswordUpdate;
	sTraderLoginRsp					m_UserLogin;
};
