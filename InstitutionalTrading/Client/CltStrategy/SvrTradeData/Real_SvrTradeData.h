#pragma once

#include "Interface_SvrTradeData.h"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Rates.h"
#include "Interface_AccountOp.h"
#include "Interface_UserOp.h"
#include "TradingDayManager.h"
#include <map>
#include <queue>
#include "BufferDataStore.h"
#include "rwlock.h"
#include "WriteLog.h"




//#ifdef _DEBUG
//#pragma comment(lib, "PlatformDataMgr4dllD.lib")
//#pragma comment(lib, "tools4dllD.lib")
//#pragma comment(lib, "wsock32.lib")
//#else
//#pragma comment(lib, "PlatformDataMgr4dll.lib")
//#pragma comment(lib, "tools4dll.lib")
//#pragma comment(lib, "wsock32.lib")
//#endif

// #ifdef _DEBUG
//     #pragma comment(lib, "PlatformDataMgr4clrD.lib")
//     //#pragma comment(lib, "tools4dllD.lib")
//     #pragma comment(lib, "wsock32.lib")
// #else
//     #pragma comment(lib, "PlatformDataMgr4clr.lib")
//     //#pragma comment(lib, "tools4dll.lib")
//     #pragma comment(lib, "wsock32.lib")
// #endif

typedef std::map<BrokerAccountKey,IInterfaceAccountOp*>            _MapAccountData;
//typedef std::map<BrokerAccountKey,eCommissionMarginReady>          _MapAccountRateReady; 
typedef std::map<std::string,IInterfaceUserOp*>                    _MapUserTradeData;

class CReal_SvrTradeData : public CInterface_SvrTradeData
{

public:
    static CReal_SvrTradeData& getObj(void);
	CReal_SvrTradeData();
	~CReal_SvrTradeData();

public: //������״̬֪ͨ	
	virtual void SetIntrumentsStatus(const CThostFtdcInstrumentStatusField & nStatus);


	//�ж��Ƿ��ǽ�����
	//�ַ���Ϊ���ڸ�ʽ2013-08-09
	virtual bool IsTradingDay(const std::string & nsDay);


	//�жϽ������Ƿ����
	//�ַ���Ϊ���ڸ�ʽ2013-08-09
	virtual bool IsSettlemented(const std::string & nsDay) ;

	//��ȡһ�����ڵ���һ��������
	virtual bool GetLastTrading(const std::string & nsDay, std::string & nsLastDay) ;

	//���õ�ǰ������
	virtual void SetCurrentTradingDay(const std::string & nsCurrentTradingDay,bool nbSetByMySelf=false) ;

	//��ȡ��ǰ������
	virtual void GetCurrentTradingDay(std::string & nsCurrentTradingDay) ;


	//����״̬��ʼ��1
	virtual void TradeDataInit(CMemCache* pCache,string szUser,string szBrokerCode,string szTradeAccount,bool nbForce = false) ;

	//��������ÿ���˻�������CTP��״̬
	virtual void SetAccountStatus(const std::string & nsAccoutnID,
		const std::string & nsBrokerID,
		const EnumAccountStatus& neUserStatu) ;

	//��ȡÿ���˻���ctp״̬
	virtual void GetAccountStatus(const std::string & nsAccoutnID,
		const std::string & nsBrokerID,
		EnumAccountStatus& neCTPStatus,
		double & nStatusTime);

	//��ȡ����ί�н����˺ŵ�״̬
	virtual void GetAllAccountStatus(std::vector<sAccountStatus> & lStatus);

	//��ȡ�����û��ĳ�ʼ��״̬
	virtual void GetAlUserInitStatus(std::vector<sUserInitStatus> & lStatus);

	//��ȡ�û���ctp����״̬
	virtual EnumAccountStatus GetUserCTPStatus(int nUserID,double & nDifftime) ;

	//��ȡ�û��ĳ�ʼ��״̬
	virtual eUserInitStatus GetUserInitStatus(int nUserID) ;


	//��ȡ�û���ctp����״̬
	virtual EnumAccountStatus GetUserCTPStatus(const std::string& nsUser,double & nDifftime) ;

	//��ȡ�û��ĳ�ʼ��״̬
	virtual eUserInitStatus GetUserInitStatus(const std::string& nsUser) ;

	//��ȡ�������ݹ���ģ��״̬
	virtual eTradeDataStatus GetTraderDataStatus() ;

	//��ȡ�������ݹ���ģ��״̬
	virtual eExchangeStatus GetExchangeStatus() ;

	//��½�ͳ�ʼ��һ��ί�н����˺�
	virtual CF_ERROR LoginAdnInitTraderAccount(const BrokerAccountKey & nKey,int nAccountID) ;

public: //��Լ����


	//��ȡ������
	virtual void GetExchangeData(ExchangeTime & nData) ;

	///����ָ����Լ��Ϣ
	virtual void SetInstrumentInfo(PlatformStru_InstrumentInfo& outData) ;

	///���������Լ�б�
	virtual void AddMainInstrumentList(std::string instrument) ;

	///��ȡ��Լ�б�
	virtual int GetInstrumentList(std::set<std::string> &outData,
		std::string ExchangeID="") ;

	///��ȡ������Լ�б�
	virtual int GetMainInstrumentList(std::set<std::string> &outData) ;

	///���ú�Լ�ݲ��б�
	virtual void SetInstrumentVarietyMap(map<string, string>& inData);

	// ��ȡ��Լ�ݲ��б�
	virtual int GetInstrumentVarietyMap(map<string, string>& outData) ;

	///��ȡָ��Ʒ�ֵĺ�Լ�б�,���غ�Լ����
	virtual int GetInstrumentListByProductID(const std::string& ProductID,
		std::set<std::string> &outData) ;

	///��ȡָ����Լ��Ϣ
	virtual int GetInstrumentInfo(const std::string& InstrumentID, 
		PlatformStru_InstrumentInfo& outData) ;

	///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
	virtual char GetProductClassType(const std::string& InstrumentID) ;

	///��ȡ�����˵��Ⱥ�Լ����Ϻ�Լ�б�
	virtual bool GetCombInstrumentIDs_IncludeLeg(const std::string& LegInstrument,
		std::vector<std::string>& vecCombInstruments);

	///��ȡ��ԼƷ�ִ���
	virtual int GetProductID(std::map<std::string, std::set<std::string> >& outData,
		std::string ExchangeID="");    

	///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
	virtual int GetInstrumentMultiple(const std::string& InstrumentID) ;

	//���̵��ã���ѯ��Լ���
	virtual void EndQryInstrument() ;

	//��ȡ��Լ�����Ƿ����
	eDataReady GetInstrumentDataReady();


public://�������
	//����ָ����Լ���飬���õ�������£��򷵻�true�����򷵻�false����ԭ����������ϣ�
	virtual CF_ERROR SetQuotInfo(PlatformStru_DepthMarketData& inData,bool bSettlementManul = false);

	///��ȡָ����Լ����
	virtual int GetQuotInfo(const std::string& InstrumentID,
		PlatformStru_DepthMarketData& outData);

	///��ȡָ����Լ������
	virtual int GetOldQuotInfo(const std::string& InstrumentID, 
		PlatformStru_DepthMarketData& outData);

	///��ȡָ����Լ���ּۣ�ʧ�ܷ�����Чֵ
	virtual double GetCurPrice(const std::string& InstrumentID);


/***********************************�漰ί�н����˺ŵĲ���******************************************************************************/
/*
//����
//��������
//�ʽ�
//�ֲ�
//�ֲ���ϸ
*/
public: //���ʲ���
	///��ȡĳ���˻���Լ��������, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
	virtual int GetCommissionRate(const string& nsBrokerID,
		const string& nsAccountID,
		const string& InstrumentID, 
		PlatformStru_InstrumentCommissionRate& outData) ;

	///��ȡĳ���û���Լ��������, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
	//��Ϊһ���û���һ���˺ţ�����Ŀǰ���������ַ�ʽ�����ط���
	virtual int GetCommissionRate(const string& nsUserID,	
		const string& InstrumentID, 
		PlatformStru_InstrumentCommissionRate& outData) ;


	///����ĳ���˻���Լ��������
	virtual void SetCommissionRate(PlatformStru_InstrumentCommissionRate& outData) ;

	//���̵��ã���ĳһ���û���Լ�ı�֤���ʲ�ѯ��ɺ�
	virtual void EndUserQryCommission(const std::string& nsBrokerID,
		const std::string &nAccountID,
        const std::string &nUserID) ;

public: //��֤����
	///��ȡ��Լ��֤����, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
	virtual int GetMarginRate(const string& nsBrokerID,
		const string& nsAccountID,
		const string& InstrumentID, 
		PlatformStru_InstrumentMarginRate& outData) ;


	///��ȡ��Լ��֤����, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
	virtual int GetMarginRate(const string& nsUserID,	
		const string& InstrumentID, 
		PlatformStru_InstrumentMarginRate& outData) ;

	///���ú�Լ��֤����
	virtual void SetMarginRate(PlatformStru_InstrumentMarginRate& outData) ;


	//���̵��ã���ĳһ���û���Լ�ı�֤���ʲ�ѯ��ɺ�
	virtual void EndUserQryMargin(const std::string& nsBrokerID,
		const std::string &nAccountID) ;

public: //�ʽ�

	//���̵��ã���ĳһ�����ʺ��ʽ��ѯ���
	virtual void QryFundReach(const PlatformStru_TradingAccountInfo& inData,const std::string & nUserID) ;


public: //�ֲ�

	//��ѯ��ĳһ�����ʺŵĳֲ�
	virtual void QryPositionReach( PlatformStru_Position& PositionInfo, int ErrID, bool bIsLast) ;


	//���̵��ã���ĳһ���ֲֲ�ѯ��ɺ�
	virtual void EndUserQryPosition(const std::string& nsBrokerID,
		const std::string &nAccountID) ;

public: //�ֲ�

	//��ѯ��ĳһ�����ʺŵĳֲ�
	virtual void QryPositionDetailReach( PlatformStru_PositionDetail& PositionInfo, int ErrID, bool bIsLast) ;


	//���̵��ã���ĳһ���ֲ���ϸ��ѯ��ɺ�
	virtual void EndUserQryPositionDetail(const std::string& nsBrokerID,
		const std::string &nAccountID) ;

/***********************************�漰���˺ŵĲ���******************************************************************************/
/*
//�ֲ���ϸ
//�ֲ�
//��ϳֲ���ϸ
//��ϳֲ�
//�ɽ�
//�ɽ�����
//����
*/

public://��������

	//��ѯ��������
	virtual CF_ERROR QryOrderReach(PlatformStru_OrderInfo& OrderInfo,
		const int ErrID,const bool bIsLast) ;

	//���̵��ã���ĳһ���˻��ı�����
	virtual void EndUserQryOrder(const std::string& nsBrokerID,
		const std::string &nAccountID);

	////���һ������
	virtual CF_ERROR PushOrder(PlatformStru_OrderInfo & nOrderInfo,
		bool nbNeedRiseEvent = true) ;
	
	// ����¼��Ӧ��
	virtual CF_ERROR PushRspOrderInsert(int nFrontID,
		int nSessionID,
		CThostFtdcInputOrderField *pInputOrder, 
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, 
		bool bIsLast);

	///����¼�����ر�
	virtual CF_ERROR PushErrRtnOrderInsert(int nFrontID,
		int nSessionID,
		CThostFtdcInputOrderField *pInputOrder,
		CThostFtdcRspInfoField *pRspInfo);

	///��������������Ӧ
	virtual CF_ERROR PushRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, 
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, 
		bool bIsLast);

	///������������ر�
	virtual CF_ERROR PushErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction,
		CThostFtdcRspInfoField *pRspInfo);


	///��ȡָ������
	virtual CF_ERROR GetUserOrderByKey(const std::string & nUserID,
		const OrderKey& nOrderKey,
		PlatformStru_OrderInfo& outData) ;

	///��ȡȫ������
	virtual CF_ERROR GetUserOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	/////��ȡȫ���Ѵ����ı���
	virtual CF_ERROR GetUserTriggerOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	/////��ȡ�ѳɽ�����
	virtual CF_ERROR GetUserTradedOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	/////��ȡ�ѳ����ʹ��󱨵�
	virtual CF_ERROR GetUserCanceledOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	/////��ȡδ�ɽ�����
	virtual CF_ERROR GetUserWaitOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	///��ȡδ֪״̬�ı���
	virtual CF_ERROR GetUserUnkownOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	//
	///��ȡָ������
	virtual CF_ERROR GetAccountOrderByKey(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		const OrderKey& nOrderKey,
		PlatformStru_OrderInfo& outData);


	///��ȡȫ������
	virtual CF_ERROR GetAccountOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	/////��ȡȫ���Ѵ����ı���
	virtual CF_ERROR GetAccountTriggerOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	/////��ȡ�ѳɽ�����
	virtual CF_ERROR GetAccountTradedOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	/////��ȡ�ѳ����ʹ��󱨵�
	virtual CF_ERROR GetAccountCanceledOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	/////��ȡδ�ɽ�����
	virtual CF_ERROR GetAccountWaitOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	///��ȡδ֪״̬�ı���
	virtual CF_ERROR GetAccountUnkownOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) ;
public: //�ɽ�����

	//��ѯ�ĳɽ��ر�����
	virtual CF_ERROR QryTradeReach( PlatformStru_TradeInfo& TradeInfo,
		int ErrID,bool bIsLast) ;


	//���̵��ã���ĳһ���˻��ĳɽ���
	virtual void EndUserQryTrade(const std::string& nsBrokerID,
		const std::string &nAccountID);

	//�µĳɽ��ر�����
	virtual CF_ERROR PushTrade( PlatformStru_TradeInfo& TradeInfo);

	//��ȡȫ���ɽ��������������������
	virtual CF_ERROR  GetUserTradeInfo(const std::string & nUserID,
		std::vector<PlatformStru_TradeInfo>& outData);

	//���CTP�ɽ�����ı�������ǲ������ǵı������
	virtual bool CheckTradeOrderSysIDIsOwn(const std::string& nsUserID,
		const std::string& nsOrderSysID) ;

	//��ȡȫ���ɽ��������������������
	virtual CF_ERROR  GetAccountTradeInfo(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_TradeInfo>& outData) ;



	//��ȡ���˻������ɽ�����Seq
	virtual CF_ERROR  GetAccountOrderAndTraderGESeq(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		int nOrderSeq,
		int nTraderSeq,
		std::vector<PlatformStru_OrderInfo> & nvecOrder,
		std::vector<PlatformStru_TradeInfo>& nvecTrader) ;


	//��ȡ���˻������ɽ�����Seq
	virtual CF_ERROR  GetUserOrderAndTraderGESeq(const std::string & nInvestorID,
		int nOrderSeq,
		int nTraderSeq,
		std::vector<PlatformStru_OrderInfo> & nvecOrder,
		std::vector<PlatformStru_TradeInfo>& nvecTrader) ;

public://�ֲֻ��ܲ���


	//��ȡ�ֲֻ��ܣ����������������
	virtual CF_ERROR  GetUserPositions(const std::string & nUserID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData,
		bool bNeedZeroVolumePosition = false) ;

	//��ȡ�ֲܳ���
	virtual CF_ERROR GetUserPositionVolume(const std::string & nUserID,
		const std::string & nInstrumentID,
		int & nVolume) ;
	//��ȡ�ֲֻ��ܣ�
	virtual CF_ERROR  GetAccountPositions(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData,
		bool bNeedZeroVolumePosition = false) ;
public: //�ֲ���ϸ����

	//��ȡ�ֲ���ϸ�����������������
	virtual CF_ERROR  GetUserPositionDetail(const std::string & nUserID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetail>& outData) ;

	//��ȡ�ֲ���ϸ�����ض�Ӧ��key������
	virtual CF_ERROR  GetUserPositionDetail(const std::string & nUserID,
		const PositionDetailKey & nKey,
		PlatformStru_PositionDetail& outData) ;

	//��ȡ�ֲ���ϸ
	//�ɹ�����CF_ERROR_SUCCESS
	virtual CF_ERROR  GetAccountPositionDetail(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetail>& outData) ;

	//��ȡ�ֲ���ϸ�����ض�Ӧ��key�ĳֲ���ϸ
	//�ɹ�����CF_ERROR_SUCCESS
	virtual CF_ERROR  GetAccountPositionDetail(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		const PositionDetailKey & nKey,
		PlatformStru_PositionDetail& outData) ;




public: //��ϳֲֻ��ܲ���
	//��ȡ��ϳֲ֣����������������
	virtual CF_ERROR  GetUserPositionComb(const std::string & nUserID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData) ;

public: //��ϳֲ���ϸ����

	//��ȡ��ϳֲ���ϸ�����������������
	virtual CF_ERROR  GetUserPositionCombDetail(const std::string & nUserID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetailComb>& outData);


	
public: //�ʽ����
	//��ȡ����Ա�ʽ���Ϣ
	virtual CF_ERROR  GetUserFundInfo(const std::string & nUserID,
		PlatformStru_TradingAccountInfo& outData) ;

	//��ȡ����Ա��ʼ�ʽ�
	virtual CF_ERROR  GetUserInitFund(const std::string & nUserID,
		double& outData) ;


	//��ȡί�н����ʺ��ʽ���Ϣ
	virtual CF_ERROR  GetAccountFundInfo(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		PlatformStru_TradingAccountInfo& outData) ;

public: //��ȡ���ܲ���


	//��ȡ��ǰ�����յĺ�Լ
	virtual CF_ERROR  GetCurrentDayAlInstrumentInfo(
		std::vector<PlatformStru_InstrumentInfo> & outData) ;

	//��ȡ����ί�н����˺ŵ���������
	virtual CF_ERROR GetAllCommissionRate(
		std::vector<PlatformStru_InstrumentCommissionRate> & outData) ;

	//��ȡ����ί�н����˺ŵı�֤����
	virtual CF_ERROR GetAllMarginRate(
		std::vector<PlatformStru_InstrumentMarginRate> & outData) ;


	//��ȡ���еĽ����
	virtual CF_ERROR  GetAllSettlementPrice(
		std::map<std::string,PlatformStru_DepthMarketData> & outData) ;

	//��ȡ��ǰ�����յĽ����
	virtual CF_ERROR  GetCurrentDayAllSettlementPrice(
		std::map<std::string,PlatformStru_DepthMarketData> & outData) ;

	//��ȡ���еĳɽ�
	virtual CF_ERROR  GetAllTradeInfo(
		std::map<std::string,std::vector<PlatformStru_TradeInfo>> & outData) ;

	//��ȡ���еĳֲ���ϸ
	virtual CF_ERROR  GetAllPositionDetail(
		std::map<std::string,std::vector<PlatformStru_PositionDetail>> & outData) ;

	//��ȡ���еĳֲ�
	virtual CF_ERROR  GetAllPosition(
		std::map<std::string,std::vector<PlatformStru_Position>> & outData) ;

	//��ȡ���е��ʽ�
	virtual CF_ERROR  GetAllFundInfo(
		std::map<std::string,PlatformStru_TradingAccountInfo> & outData) ;

	//��ʼ����
	virtual CF_ERROR  StartSettlement(eSettlementLoadOption nOption,
		const std::string & nsDay ,
		bool nbHistorySettlement) ;

	
	//��������
	virtual CF_ERROR  EndSettlement(const std::string & nsDay);

	//�����޸ĺ���������ʺͱ�֤���ʣ��������ɽ��ͳֲֺ��ʽ�ļ���
	virtual CF_ERROR  SetModifiedCommissionAndMargin( std::vector<PlatformStru_InstrumentCommissionRate> & inCommssion,
		 std::vector<PlatformStru_InstrumentMarginRate> & inMargin) ;

	//�����޸ĺ�ĳɽ����������ɽ��ͳֲֺ��ʽ�ļ���
	virtual CF_ERROR  SeUserNewTrades(const std::string & nUserName,
		const std::vector<PlatformStru_TradeInfo> & inTradeInfo) ;

public:
	virtual CF_ERROR CheckUserInstrumentCommissionAndMaginExist(const std::string & nUserName,
		const std::string & nIstrumentID) ;
	
	//�ж��Ƿ�ɿ���
	virtual CF_ERROR  CanOrderOpen(const PlatformStru_OrderInfo & nOrder);

	//�ж��Ƿ��ƽ��
	virtual CF_ERROR  CanOrderColseToday(const PlatformStru_OrderInfo & nOrder) ;

	//�ж��Ƿ��ƽ��
	virtual CF_ERROR  CanOrderColse(const PlatformStru_OrderInfo & nOrder) ;

public:	
	//�����
	virtual CF_ERROR FundInOut(const sFundInOut& nFundInOut) ;

	//��ѯ��ʷ�����
	virtual CF_ERROR QryHistroyFundInOut(const sQryFundInOut & nQryFund,
		std::vector<sFundInOut> & nvecFund) ;

    //�����û���ʼ�ʽ� ���Ա��ص��� 
    virtual CF_ERROR SetUserInitFund(const double dInitFund);

public:
	//��ѯ����
	virtual CF_ERROR ResponseToAsk(const eAskType& neDataType,
		const eAskOperationType& neChangeType,
		const int nllDataID,
		const char * npDataBuf);


public:
	////���һ��CTP����
	virtual CF_ERROR QryCTPOrder(PlatformStru_OrderInfo & nOrderInfo,const int ErrID,const bool bIsLast) ;

	//���һ��CTP�ɽ�
	virtual CF_ERROR QryCTPTrader(PlatformStru_TradeInfo & nTraderInfo,const int ErrID,const bool bIsLast) ;

	////���һ��CTP����
	virtual CF_ERROR PushCTPOrder(PlatformStru_OrderInfo & nOrderInfo) ;

	//���һ��CTP�ɽ�
	virtual CF_ERROR PushCTPTrader(PlatformStru_TradeInfo & nTraderInfo) ;


	////��ӿͻ����µ�
	virtual CF_ERROR PushExecuteInOrder(PlatformStru_OrderInfo & nOrderInfo) ;

	//���һ���ͻ����µ��ķ��ؽ��
	virtual CF_ERROR PushExecuteOutOrder(PlatformStru_OrderInfo & nOraderInfo) ;

public:
	//��ȡ�������ݿ�Ľӿ�
// 	virtual IInterfaceDBOpr * GetInterfaceDBOpr();

public:
	//�鿴�û��Ƿ�ȷ���˽��㵥
	virtual bool IsUserSettlementConfirmed(const std::string & nUserName,
		std::string & nsConfirmedDate,
		std::string & nsConfirmedTime) ;

	//�û�ȷ���˽��㵥
	//�û�ȷ���˽��㵥
	virtual void SetUserSettlementConfirmed(const std::string & nsUserName,
		const std::string & nsConfirmedDate,
		const std::string & nsConfirmedTime) ;
public:
	//��ȡ��ʼ��ʱ��
	virtual CF_ERROR GetReinitTime(sReinitTime & nsReinitTime) ;

	//��ȡ���ݱ���ʱ��
	virtual CF_ERROR GetSaveTime(sSaveTime & nsReinitTime) ;

	//���ó�ʼ��ʱ��
	virtual CF_ERROR SetReinitTime(const sReinitTime & nsReinitTime) ;

	//��ȡ����ʱ��
	virtual CF_ERROR GetTradingTime(sTradingTime & nsTradingTime) ;


	//���ý���ʱ��
	virtual CF_ERROR SetTradingTime(const sTradingTime & nsTradingTime) ;


public:
	//���̲�ѯ���ɹ�����
	virtual void QryFailed(const BrokerAccountKey & nBrokerAccount,eEventType nEvent) ;

	//��ȡ��Ӧ�û���Ӧʱ��εĳֲ���ϸ
	virtual CF_ERROR GetPositionDetail_MaxTime(const  std::vector<std::string> nvecAccount, 
		int LowerOpenTime,int UpperOpenTime,
		std::map<std::string, std::vector<PlatformStru_PositionDetail>>& mapDetail) ;




/******************************************************************************************************************/

public: //��������
	virtual void GenerateOrderInfo(const PlatformStru_InputOrder& inputOrder,PlatformStru_OrderInfo & orderInfo);
	virtual void GenerateInputOrder(const PlatformStru_OrderInfo& orderInfo,PlatformStru_InputOrder & inputOrder);
/******************************************************************************************************************/
public://�ڲ��ӿ�

	//����ճ����ݵĳֲ��Ƿ��й��ں�Լ
	bool CheckInitDataHasExpiredInstruementPosition();
	//�����Ͳ�ѯ���ʺͱ�֤����
	void SendQryCommissionAndMargin();

	void GenerateOrderInfoFromCTPInputOrder(const CThostFtdcInputOrderField& inputOrder,
		PlatformStru_OrderInfo & orderInfo);

	//��ȡ�����պ󣬽��еĽ��׳�ʼ������
	virtual void TradeDataInitAfterGetTradingDay() ;
	
	//û�л�ȡ�����պ󣬽��еĽ��׳�ʼ������
	virtual void TradeDataInitAfterNoTradingDay() ;

	//���ÿ���û��Ƿ������˶��������뵽����״̬
	virtual bool CheckAllUserFinishedInit();


	//���ÿ���û����������Ƿ�ͬ�����
	virtual bool CheckAllUserAccountSyn();

	//���ÿ���û�ctp��½���
	virtual bool CheckAllUserFinishedCTP(int & nLoginCount);

	//������ã�����һ������ݽ������ݿ��ʼ����������
	virtual CF_ERROR SettlementInitTradeDataWithTradingDB(eSettlementLoadOption nOption,const std::string & nsDay) ;


	virtual _MapUserTradeData* GetMapUserDataWithOption(eSettlementLoadOption nOption);
	//���ý������ݹ���ģ��״̬
	virtual void SetTraderDataStatus(eTradeDataStatus neStatus) ;

	////�������뵥���ɱ���
	//static void GenerateOrderInfo(const CThostFtdcInputOrderField& inputOrder,
	//	PlatformStru_OrderInfo & orderInfo);

	//�����ʶ�����ɸ������û��Ķ���
	virtual void SetRatesObjectToAccountRelationUserObject(const std::string & nsBrokerId,
        const std::string & nAccountID,
		const std::string & nUserID);

	//����Լ������ɸ������û��Ķ���
	virtual void DispathRateToAllUser(_MapUserTradeData & mapUserTradeData);


	//����Լ������ɸ������û��Ķ���
	virtual void SetPlatformObjectToAllUser(_MapUserTradeData & mapUserTradeData);

	//����Լ������ɸ��������˻��Ķ���
	virtual void SetPlatformObjectToAllAccount();


	//�����յ��Ӷ��󲢼���map
	virtual void CreateEmptyAccountOpAndAddMap(const string & nUserId,_MapUserTradeData & nmapUserData);

	//�����û��ĳ�ʼ���ݵ�����������
	virtual void DispathInitialDataToUserWithAccount(const std::string & nBrokerID, 
		const std::string & nAccountID,
		_MapUserTradeData & nmapUserTradeData,
        const std::string & nUserID);

	//�����û��ĳ�ʼ���ݵ�����������
	virtual void DispathInitialDataToUser(const std::string & nUserId, 
		 const std::string & nsInstruemntDay,
		_MapUserTradeData & nmapUserTradeData);


	//���ɽ���ĳ������û�����
	virtual void DispathFundInOut(const std::string & nsDate,_MapUserTradeData & nmapUserTradeData);

	//װ��ĳһ�������û��ĵ��ճ�����
	virtual CF_ERROR LoadInitData(PlatformStru_TradingAccountInfo& inData, const std::string & nsDate,const std::string & szUser);

	//�ӽ������ݿ�����װ���ճ��ĳֲ�
	virtual CF_ERROR LoadInitialPositionFromDB(const std::string & nUserName,
		const std::string & nsTime,
		vector<PlatformStru_Position> & nVecPosition);

	//�ӽ������ݿ�����װ���ճ��ĳֲ���ϸ
	virtual CF_ERROR LoadInitialPositionDetailFromDB(const std::string & nUserName,
		const std::string & nsTime,
		vector<PlatformStru_PositionDetail> & nVecPositionDetail);

	//�ӽ������ݿ�����װ���ճ����ʽ�
	virtual CF_ERROR LoadInitialFundFromDB(const std::string & nUserName,
		const std::string & nsTime,
		PlatformStru_TradingAccountInfo & nFundIndo);

	////�ӳ�ʼȨ������������ת�س�ʼ�ʽ�
	virtual CF_ERROR LoadInitialInitFundFromDB(const std::string & nUserName,		
		double & nFundIndo);
	

	//��ʼ���û�����
	virtual CF_ERROR InitUserMapWithEmpty(string szUser,string szBrokerCode,string szTradeAccount,_MapUserTradeData & nmapUserData);
	//ȫ����������
	virtual CF_ERROR ResetAll();

	//�����������
	virtual CF_ERROR SaveAllTradingData(bool nbInitFirst=false);

	//��ʱ�洢
	virtual CF_ERROR TimerSaveTradingData(bool bTimer = true);

	//����һ���¼��û���ص��˻�Ϊ����
	virtual CF_ERROR PushEventToAccountWithUser(eEventType nEve,
		const std::string & nUserName);


	//����һ���û��Ƿ��гֲ�
	virtual bool TestUserHasPosition(const std::string & nUserName);

	//����������һ������Ա����ί�н����˺�Ҳ�Ѳ������
	virtual CF_ERROR AddTraderUserWhenTrading(const std::string & nUserName,
		const std::string & nsAccountName,
		const std::string & nBrokerCode);
	
	//����ʱ�����е�δ�ɽ������ֳɽ�������˵ı���ȫ����Ϊ�ѳ���
	virtual CF_ERROR CancelAllUnkonwnOrder();

	//����һ���û��Ƿ��иóɽ�
	virtual bool TestUserTraderExist(const PlatformStru_TradeInfo & nTraderInfo);


	//�ӷ�������������һ������Ա����ί�н����˺�Ҳ�Ѳ������
	virtual CF_ERROR AddInitTraderUser(const std::string & nUserName,
		const std::string & nsAccountName,
		const std::string & nBrokerCode);

private:
	static CReal_SvrTradeData    *m_pObj;
//     IInterfaceDBOpr * mpDBOpr;   //���ݿ��м��ӿ�
	std::string   msCurrentTradingDay; //��-��-��  
	Ceasymutex mMutexTradingday;

	eTradeDataStatus meTraderDataStatus;
	eExchangeStatus  meExchangeStatus;


	bool mbForceInit; //��ʼ���Ĺ�����ǿ�Ʋ�ѯ��Լ�ͷ���
private: //���ݶ���	
	



	std::string msUserInitDay;  //�ճ���������һ���
	std::map<std::string,std::vector<PlatformStru_Position>>       mUserInitPosition;       //�ճ��ֲ�
	std::map<std::string,std::vector<PlatformStru_PositionDetail>> mUserInitPositionDeatil; //�ճ��ֲ���ϸ
	std::map<std::string,PlatformStru_TradingAccountInfo>          mUserIniFundInfo;        //�ճ��ʽ�
	std::map<std::string,double>                                   mUserInitInitFund;        //��ʼ�ʽ�
	Ceasymutex                                                     m_mutexInitData;     //�����ճ����ݵı���


	std::string msInstrumentDay;                              //��Լ�ͷ�����������һ���
	CPlatformDataMgr_Instruments *mpPlatformDataInstrument;   //��Լ
	eDataReady                   mbInstrumentsReady;          //��Լ����׼������


	CPlatformDataMgr_Quots       *mpPlatformDataQuots;                        //����
	std::map<std::string,PlatformStru_DepthMarketData> mmapSettlementQuot;    //��������
	Ceasymutex mMutextSettlementQuot;


	_MapAccountData              mMapAccountData;             //����ί�н����˺ŵ����ݣ����ʣ�״̬��
	rwlock_t                     mrwLockMapAccount;           //����ί�н����˺����ݶ������


	
	_MapUserTradeData            mMapUser2TradingData;        //���е����˻����н������ݶ���
	bool                         mbDirty;                     //���н������ݶ������ڽ�����޸Ķ�����
  	rwlock_t                     mrwLockMapUserTrading;       //�������˻����н������ݶ���Ķ�д�� 


	//_MapUserTradeData            mMapUser2TradedData;               //���е����˻��������
	//Ceasymutex                   m_mutexMapUserTraded;              //�������˻��������ݶ������ 

	//_MapUserTradeData            mMapUser2TradedDataWithCTP;         //���е����˻����������ݿ�������ݶ���
	//eDataReady                   meUserTradedDataWithCTPReady;       //���㽻������׼������
	//Ceasymutex                   m_mutexMapUserTraderWithCTP;        //�������˻��������ݶ������ 




	CTradingDayManager           mpTradingDayManager;                //�����չ���     



	//���ݿ�洢�Ķ���
	CBufferDataStore<PlatformStru_TradeInfo> mBufferDataStoreTrade;
	CBufferDataStore<PlatformStru_OrderInfo> mBufferDataStoreOrder;
	CBufferDataStore<PlatformStru_TradeInfo> mBufferDataStoreCTPTrade;
	CBufferDataStore<PlatformStru_OrderInfo> mBufferDataStoreCTPOrder;
	CBufferDataStore<PlatformStru_OrderInfo> mBufferDataStoreExecuteInOrder;
	CBufferDataStore<PlatformStru_OrderInfo> mBufferDataStoreExecuteOutOrder;


	//��־��¼
	CWriteLog *m_pWriteLog;


private:
//�û����ӷ���ͬ���Ĳ���
	volatile int mMasterSocket; //�������socket
public:
	void SetMasterSocket(int nVal) {mMasterSocket = nVal;}
	int GetMasterSocket() { return mMasterSocket;}
    bool SlaverInitAfterGetTradingDay(std::string szUser,std::string szBrokerCode,std::string szTradeAccount,CDataEntity* pDataEntity, CMemCache* pCmdCache, double dFund );
	void SetUserSynFinished(const std::string & nsUserName);
	bool GetUserSynFinished(const std::string & nsUserName);
	void SetAccountSynFinished(const BrokerAccountKey & nKey);

public:
// 	bool NeedCache() {return mbNeedCache;}
// 	bool SubscribeCacheChannel(){	return mSubCache.SubscibleRead(&CReal_SvrTradeData::AddCommandCallback);}
	bool CacheErrOrderInsert(const PlatformStru_OrderInfo & lOrder) {return mpCache->PushOrderInputError(lOrder);}
	bool CacheErrOrderAction(const CThostFtdcOrderActionField & lOrderAction) {return mpCache->PushOrderActionError(lOrderAction);}

	static void AddCommandCallback(const std::string & nsCommand);
	void AddCommand(const std::string & nsCommand);
	void DealCommand();
	
public:	
	bool PushStrategyStopOrRun(const std::string & nsName,bool nsStop) {return mpCache->PushStrategyStopOrRun(nsName,nsStop);};

	bool PushTraderUseroffLine(const std::string & nUserName,bool bOffline) {return mpCache->PushTraderUseroffLine(nUserName,bOffline);} ;

	bool PushStrategyInstanceStopOrRun(const std::string & nsUserName,
		const std::string & nsStategyName,
		bool nsStop) 
	{
			return mpCache->PushStrategyInstanceStopOrRun(nsUserName,nsStategyName,nsStop);
	}

	virtual bool GetStategyUserInstanceUpdateTime(const std::string & nsUserName,
		const std::string & nsStategyName,
		time_t & nsUpdateTime) ;
	
private:
 	CMemCache* mpCache;
	bool mbNeedCache;
	std::string mCacheHostIP;
	int mCacheHostPort;
//  	CMemCache mSubCache;
	Ceasymutex mCommandMutex;
	std::queue<std::string> mCommandList;
	Ceasymutex mUpdateTimeMapMutex;
	std::map<std::string ,time_t> mStrategyInstanceUpdateTimeMap;
	

};



