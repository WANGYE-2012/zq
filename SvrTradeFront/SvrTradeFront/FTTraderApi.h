#pragma once

#include "FTTraderSpi.h"
#include "Module-Misc2\\KernelStruct.h"

// �������Spi��Ctp��Api������Ctp��Apiָ�����Spi
// ʵ��Ctp��Api��ʼ����Release����
// ���б�Ҫ��������ѯ�̣߳������ѯ����

//���涨�彻��������״̬
enum CTPCONNSTATUS
{    
	CTPCONNSTATUS_Disconnected   =      0       ,            //δ����
	CTPCONNSTATUS_Connecting     =      1       ,            //��������
	CTPCONNSTATUS_Logining       =      2       ,            //���ڵ�¼
	CTPCONNSTATUS_LoginFailure   =      3       ,            //��¼ʧ��
	CTPCONNSTATUS_TradeSettlementInfoConfirming     =   4 ,  //���ڽ��н��׽�����ȷ��
	CTPCONNSTATUS_Connected      =      6       ,            //���ӵ�¼�ɹ�
	CTPCONNSTATUS_Disconnecting  =      7       ,            //���ڶϿ�����
	CTPCONNSTATUS_UnInit         =      8                    //δ��ʼ��
};

class CFTTraderApi
{
public:
	CFTTraderApi(void);
	virtual ~CFTTraderApi(void);

	void Init(int nSocket);
	void Init(LPVOID pLinkEntity);
	void Release();


// ����ʵ��Э�����ᵽ�ķ�CThostFtdcTraderApi�ĺ���
public:
	///�û���¼����
	int ReqUserLogin(CThostFtdcReqUserLoginField *pData, int nRequestID);

	///�ǳ�����
	int ReqUserLogout(CThostFtdcUserLogoutField *pData, int nRequestID);

	///�û������������
	int ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate* pData, int nRequestID);

	///�ʽ��˻������������
	int ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate* pData, int nRequestID);

	///����¼������
	int ReqOrderInsert(PlatformStru_InputOrder* pData, int nRequestID);

	///Ԥ��¼������
	int ReqParkedOrderInsert(PlatformStru_ParkedOrder* pData, int nRequestID);

	///Ԥ�񳷵�¼������
	int ReqParkedOrderAction(PlatformStru_ParkedOrderAction* pData, int nRequestID);

	///������������
	int ReqOrderAction(PlatformStru_InputOrderAction* pData, int nRequestID);

	///��ѯ��󱨵���������
	int ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume* pData, int nRequestID);

	///Ͷ���߽�����ȷ��
	int ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pData, int nRequestID);

	///����ɾ��Ԥ��
	int ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder* pData, int nRequestID);

	///����ɾ��Ԥ�񳷵�
	int ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction* pData, int nRequestID);

	///ѯ��¼������
	int ReqForQuoteInsert(PlatformStru_InputForQuoteField* pData, int nRequestID);

	///ִ������¼������
	int ReqExecOrderInsert(PlatformStru_InputExecOrderField* pData, int nRequestID);

	///ִ�������������
	int ReqExecOrderAction(PlatformStru_InputExecOrderActionField* pData, int nRequestID);

	///�����ѯ����
	int ReqQryOrder(PlatformStru_QryOrder* pData, int nRequestID);

	///�����ѯ�ɽ�
	int ReqQryTrade(PlatformStru_QryTrade* pData, int nRequestID);

	///�����ѯͶ���ֲ߳�
	int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition* pData, int nRequestID);

	///�����ѯ�ʽ��˻�
	int ReqQryTradingAccount(PlatformStru_QryTradingAccount* pData, int nRequestID);

	///�����ѯͶ����
	int ReqQryInvestor(PlatformStru_QryInvestor* pData, int nRequestID);

	///�����ѯ��Լ��֤����
	int ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField* pData, int nRequestID);

	///�����ѯ��Լ��������
	int ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField* pData, int nRequestID);

	///�����ѯ������
	int ReqQryExchange(CThostFtdcQryExchangeField* pData, int nRequestID);

	///�����ѯ��Ʒ
	int ReqQryProduct(PlatformStru_QryProduct* pData, int nRequestID);

	///�����ѯ��Լ
	int ReqQryInstrument(PlatformStru_QryInstrument* pData, int nRequestID);

	///�����ѯͶ���߽�����
	int ReqQrySettlementInfo(PlatformStru_QrySettlementInfo* pData, int nRequestID);

	///�����ѯͶ���ֲ߳���ϸ
	int ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail* pData, int nRequestID);

	///�����ѯ�ͻ�֪ͨ
	int ReqQryNotice(PlatformStru_QryNotice* pData, int nRequestID);

	///�����ѯ������Ϣȷ��
	int ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm* pData, int nRequestID);

	///�����ѯͶ���ֲ߳���ϸ
	int ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail* pData, int nRequestID);

	///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
	int ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField* pData, int nRequestID);

	///�����ѯ��Ȩ���׳ɱ�
	int ReqQryOptionInstrTradeCost(CThostFtdcQryOptionInstrTradeCostField* pData, int nRequestID);

	///�����ѯ��Ȩ��Լ������
	int ReqQryOptionInstrCommRate(CThostFtdcQryOptionInstrCommRateField* pData, int nRequestID);

	///�����ѯִ������
	int ReqQryExecOrder(PlatformStru_QryExecOrderField* pData, int nRequestID);

	///�����ѯת����ˮ
	int ReqQryTransferSerial(PlatformStru_QryTransferSerial* pData, int nRequestID);

	///�����ѯ����ǩԼ��ϵ
	int ReqQryAccountregister(PlatformStru_QryAccountRegister* pData, int nRequestID);

	///�����ѯǩԼ����
	int ReqQryContractBank(PlatformStru_QryContractBank* pData, int nRequestID);

	///�����ѯԤ��
	int ReqQryParkedOrder(PlatformStru_QryParkedOrder* pData, int nRequestID);

	///�����ѯԤ�񳷵�
	int ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction* pData, int nRequestID);

	///�����ѯ����֪ͨ
	int ReqQryTradingNotice(CThostFtdcQryTradingNoticeField* pData, int nRequestID);

	///�����ѯ���͹�˾���ײ���
	int ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField* pData, int nRequestID);

	///�ڻ����������ʽ�ת�ڻ�����
	int ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer* pData, int nRequestID);

	///�ڻ������ڻ��ʽ�ת��������
	int ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer* pData, int nRequestID);

	///�ڻ������ѯ�����������
	int ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount* pData, int nRequestID);

private:
	CTPCONNSTATUS		     m_TradeStatus;	 //��ctp����������״̬
	CThostFtdcTraderApi*     m_pTradeApi;    //���׽ӿڶ������ۺϽ���ƽ̨�Ľӿ��ṩ
	CFTTraderSpi	         m_TradeSpi;     //���׽ӿڻص�����
};
