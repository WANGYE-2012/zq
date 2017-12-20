#pragma once

#include "Interface_SvrCommon.h"
#include "CommonStruct.h"
#include <map>
#include <set>


using namespace std;

#ifndef SVRTRADEDATA_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRTRADEDATA_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRTRADEDATA_EXPORTS
#define SVRTRADEDATA_API __declspec(dllexport)
#else
#define SVRTRADEDATA_API __declspec(dllimport)
#endif
#endif

#include "KernelStruct.h"
#include "BusinessData.hpp"
#include "Interface_DBOpr.h"



class SVRTRADEDATA_API CInterface_SvrTradeData : public CInterface_SvrCommon
{
public:
    static CInterface_SvrTradeData& getObj(void);

/***********************************������Լ������Ĳ���******************************************************************************/
public: //������״̬֪ͨ	
	virtual void SetIntrumentsStatus(const CThostFtdcInstrumentStatusField & nStatus) = 0;

	//�ж��Ƿ��ǽ�����
	//�ַ���Ϊ���ڸ�ʽ2013-08-09
	virtual bool IsTradingDay(const std::string & nsDay) = 0;

	//�жϽ������Ƿ����
	//�ַ���Ϊ���ڸ�ʽ2013-08-09
	virtual bool IsSettlemented(const std::string & nsDay) = 0;

	//��ȡһ�����ڵ���һ��������
	virtual bool GetLastTrading(const std::string & nsDay, std::string & nsLastDay) = 0;

	//���õ�ǰ������
	virtual void SetCurrentTradingDay(const std::string & nsCurrentTradingDay,bool nbSetByMySelf=false) = 0;
	
	//��ȡ��ǰ������
	virtual void GetCurrentTradingDay(std::string & nsCurrentTradingDay) = 0;

	//�������ݹ���ģ���ʼ��
	virtual void TradeDataInit(bool nbForce = false) = 0;

	//��������ÿ���˻�������CTP��״̬
	virtual void SetAccountStatus(const std::string & nsAccoutnID,
		const std::string & nsBrokerID,
		const EnumAccountStatus& neUserStatus) = 0;

	//��ȡÿ���˻���ctp״̬
	virtual void GetAccountStatus(const std::string & nsAccoutnID,
		const std::string & nsBrokerID,
		EnumAccountStatus& neCTPStatus,
		double & nStatusTime) = 0;

    //��ȡ����ί�н����˺ŵ�״̬
	virtual void GetAllAccountStatus(std::vector<sAccountStatus> & lStatus) = 0;

	//��ȡ�����û��ĳ�ʼ��״̬
	virtual void GetAlUserInitStatus(std::vector<sUserInitStatus> & lStatus) = 0;

	//��ȡ�û���ctp����״̬
	virtual EnumAccountStatus GetUserCTPStatus(int nUserID,double & nDifftime) = 0;

	//��ȡ�û��ĳ�ʼ��״̬
	virtual eUserInitStatus GetUserInitStatus(int nUserID) = 0;

	//��ȡ�û���ctp����״̬
	virtual EnumAccountStatus GetUserCTPStatus(const std::string& nsUser,double & nDifftime) = 0;

	//��ȡ�û��ĳ�ʼ��״̬
	virtual eUserInitStatus GetUserInitStatus(const std::string& nsUser) = 0;

	//��ȡ�������ݹ���ģ��״̬
	virtual eTradeDataStatus GetTraderDataStatus() = 0;

	//��ȡ�������ݹ���ģ��״̬
	virtual eExchangeStatus GetExchangeStatus() = 0;

	//��½�ͳ�ʼ��һ��ί�н����˺�
	virtual CF_ERROR LoginAdnInitTraderAccount(const BrokerAccountKey & nKey,int nAccountID) = 0;

public: //��Լ����

	///����ָ����Լ��Ϣ
	virtual void SetInstrumentInfo(PlatformStru_InstrumentInfo& outData,bool bLast) = 0;

	///���������Լ�б�
	virtual void AddMainInstrumentList(std::string instrument) = 0;

    //��ȡ������
	virtual void GetExchangeData(ExchangeTime & nData) = 0;

	///��ȡ��Լ�б�
	virtual int GetInstrumentList(std::set<std::string> &outData,
		std::string ExchangeID="") = 0;

	///��ȡ������Լ�б�
	virtual int GetMainInstrumentList(std::set<std::string> &outData) = 0;

	///���ú�Լ�ݲ��б�
	virtual void SetInstrumentVarietyMap(map<string, string>& inData) = 0;

	// ��ȡ��Լ�ݲ��б�
	virtual int GetInstrumentVarietyMap(map<string, string>& outData) = 0;

	///��ȡָ��Ʒ�ֵĺ�Լ�б�,���غ�Լ����
	virtual int GetInstrumentListByProductID(const std::string& ProductID,
		std::set<std::string> &outData) = 0;

	///��ȡָ����Լ��Ϣ
	virtual int GetInstrumentInfo(const std::string& InstrumentID, 
		PlatformStru_InstrumentInfo& outData) = 0;

	///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
	virtual char GetProductClassType(const std::string& InstrumentID) = 0;

	///��ȡ�����˵��Ⱥ�Լ����Ϻ�Լ�б�
	virtual bool GetCombInstrumentIDs_IncludeLeg(const std::string& LegInstrument,
		std::vector<std::string>& vecCombInstruments) = 0;

	///��ȡ��ԼƷ�ִ���
	virtual int GetProductID_InstrumentIDsByExchangeID(std::map<std::string, std::set<std::string> >& outData,
		std::string ExchangeID="") = 0;    

	///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
	virtual int GetInstrumentMultiple(const std::string& InstrumentID) = 0;

	//���̵��ã���ѯ��Լ���
	virtual void EndQryInstrument() = 0;

public: //�������

	//����ָ����Լ���飬���õ�������£��򷵻�true�����򷵻�false����ԭ����������ϣ�
	virtual CF_ERROR SetQuotInfo(PlatformStru_DepthMarketData& inData,bool bSettlementManul = false) = 0;



	///��ȡָ����Լ����
	virtual int GetQuotInfo(const std::string& InstrumentID,
		PlatformStru_DepthMarketData& outData)= 0;

	///��ȡָ����Լ������
	virtual int GetOldQuotInfo(const std::string& InstrumentID, 
		PlatformStru_DepthMarketData& outData)= 0;

	///��ȡָ����Լ���ּۣ�ʧ�ܷ�����Чֵ
	virtual double GetCurPrice(const std::string& InstrumentID)= 0;

	//��ȡ�����



/***********************************�漰ί�н����˺ŵĲ���******************************************************************************/
/*
//����
//��������
//�ʽ�
//�ֲ�
//�ֲ���ϸ
*/
public: //���ʲ���

	///����ĳ���˻���Լ��������
	virtual void SetCommissionRate( PlatformStru_InstrumentCommissionRate& outData) = 0;

	///��ȡĳ���˻���Լ��������, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
	virtual int GetCommissionRate(const string& nsBrokerID,
		const string& nsAccountID,
		const string& InstrumentID,
		PlatformStru_InstrumentCommissionRate& outData) = 0;


	///��ȡĳ���û���Լ��������, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
	//��Ϊһ���û���һ���˺ţ�����Ŀǰ���������ַ�ʽ�����ط���
	virtual int GetCommissionRate(const string& nsUserID,	
		const string& InstrumentID, 
		PlatformStru_InstrumentCommissionRate& outData) = 0 ;


	//���̵��ã���ĳһ���û���Լ�ı�֤���ʲ�ѯ��ɺ�
	virtual void EndUserQryCommission(const std::string& nsBrokerID,
		const std::string &nAccountID) = 0;

public: //��֤����

	///���ú�Լ��֤����
	virtual void SetMarginRate( PlatformStru_InstrumentMarginRate& outData) = 0;

	///��ȡ��Լ��֤����, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
	virtual int GetMarginRate(const string& nsBrokerID,
		const string& nsAccountID,
		const string& InstrumentID, 
		PlatformStru_InstrumentMarginRate& outData) = 0;


	///��ȡ��Լ��֤����, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
	virtual int GetMarginRate(const string& nsUserID,	
		const string& InstrumentID, 
		PlatformStru_InstrumentMarginRate& outData) = 0;

    //���̵��ã���ĳһ���û���Լ�ı�֤���ʲ�ѯ��ɺ�
	virtual void EndUserQryMargin(const std::string& nsBrokerID,
		const std::string &nAccountID) = 0;


public: //�ʽ�
	
	//���̵��ã���ĳһ�����ʺ��ʽ��ѯ���
	virtual void QryFundReach(const PlatformStru_TradingAccountInfo& inData) = 0;


public: //�ֲ�

	//��ѯ��ĳһ�����ʺŵĳֲ�
	virtual void QryPositionReach( PlatformStru_Position& PositionInfo, int ErrID, bool bIsLast) = 0;


	//���̵��ã���ĳһ���ֲֲ�ѯ��ɺ�
	virtual void EndUserQryPosition(const std::string& nsBrokerID,
		const std::string &nAccountID) = 0;

public: //�ֲ�

	//��ѯ��ĳһ�����ʺŵĳֲ�
	virtual void QryPositionDetailReach( PlatformStru_PositionDetail& PositionInfo, int ErrID, bool bIsLast) = 0;


	//���̵��ã���ĳһ���ֲ���ϸ��ѯ��ɺ�
	virtual void EndUserQryPositionDetail(const std::string& nsBrokerID,
		const std::string &nAccountID) = 0;


/***********************************�漰���˺ŵĲ���******************************************************************************/
/*
//�ֲ���ϸ
//�ֲ�
//��ϳֲ���ϸ
//��ϳֲ�
//�ɽ�
//�ɽ�����
//����
//Ԥ�񵥲���
//Ԥ�񳷵�����
*/

public://��������

	//��ѯ��������
	virtual CF_ERROR QryOrderReach(PlatformStru_OrderInfo& OrderInfo,
		const int ErrID,const bool bIsLast) = 0;

	//���̵��ã���ĳһ���˻��ı�����
	virtual void EndUserQryOrder(const std::string& nsBrokerID,
		const std::string &nAccountID) = 0;

	////���һ������
	virtual CF_ERROR PushOrder(PlatformStru_OrderInfo & nOrderInfo,
		bool nbNeedRiseEvent = true) = 0;

	// ����¼��Ӧ��
	virtual CF_ERROR PushRspOrderInsert(int nFrontID,
		int nSessionID,
		CThostFtdcInputOrderField *pInputOrder, 
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, 
		bool bIsLast)= 0;
	
	///����¼�����ر�
	virtual CF_ERROR PushErrRtnOrderInsert(int nFrontID,
		int nSessionID,
		CThostFtdcInputOrderField *pInputOrder,
		CThostFtdcRspInfoField *pRspInfo)= 0;

	///��������������Ӧ
	virtual CF_ERROR PushRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, 
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, 
		bool bIsLast)= 0;
	
	///������������ر�
	virtual CF_ERROR PushErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction,
		CThostFtdcRspInfoField *pRspInfo)= 0;
	


	//
	///��ȡָ������
	virtual CF_ERROR GetUserOrderByKey(const std::string & nUserID,
		const OrderKey& nOrderKey,
		PlatformStru_OrderInfo& outData) = 0;

	
	///��ȡȫ������
	virtual CF_ERROR GetUserOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////��ȡȫ���Ѵ����ı���
	virtual CF_ERROR GetUserTriggerOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////��ȡ�ѳɽ�����
	virtual CF_ERROR GetUserTradedOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////��ȡ�ѳ����ʹ��󱨵�
	virtual CF_ERROR GetUserCanceledOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////��ȡδ�ɽ�����
	virtual CF_ERROR GetUserWaitOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	///��ȡδ֪״̬�ı���
	virtual CF_ERROR GetUserUnkownOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;


	//
	///��ȡָ������
	virtual CF_ERROR GetAccountOrderByKey(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		const OrderKey& nOrderKey,
		PlatformStru_OrderInfo& outData) = 0;

	
	///��ȡȫ������
	virtual CF_ERROR GetAccountOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////��ȡȫ���Ѵ����ı���
	virtual CF_ERROR GetAccountTriggerOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////��ȡ�ѳɽ�����
	virtual CF_ERROR GetAccountTradedOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////��ȡ�ѳ����ʹ��󱨵�
	virtual CF_ERROR GetAccountCanceledOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////��ȡδ�ɽ�����
	virtual CF_ERROR GetAccountWaitOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	///��ȡδ֪״̬�ı���
	virtual CF_ERROR GetAccountUnkownOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;


public: //�ɽ�����

	//��ѯ�ĳɽ��ر�����
	virtual CF_ERROR QryTradeReach( PlatformStru_TradeInfo& TradeInfo,
		int ErrID,
		bool bIsLast) = 0;


	//���̵��ã���ĳһ���˻��ĳɽ���
	virtual void EndUserQryTrade(const std::string& nsBrokerID,
		const std::string &nAccountID) = 0;

	//�µĳɽ��ر�����
	virtual CF_ERROR PushTrade( PlatformStru_TradeInfo& TradeInfo) = 0;

	//��ȡȫ���ɽ��������������������
	virtual CF_ERROR  GetUserTradeInfo(const std::string & nUserID,
		std::vector<PlatformStru_TradeInfo>& outData) = 0;

	//���CTP�ɽ�����ı�������ǲ������ǵı������
	virtual bool CheckTradeOrderSysIDIsOwn(const std::string& nsUserID,
		const std::string& nsOrderSysID) = 0;

	//��ȡȫ���ɽ��������������������
	virtual CF_ERROR  GetAccountTradeInfo(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_TradeInfo>& outData) = 0;


	//��ȡ���˻������ɽ�����Seq
	virtual CF_ERROR  GetAccountOrderAndTraderGESeq(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		int nOrderSeq,
		int nTraderSeq,
		std::vector<PlatformStru_OrderInfo> & nvecOrder,
		std::vector<PlatformStru_TradeInfo>& nvecTrader) = 0;


	//��ȡ���˻������ɽ�����Seq
	virtual CF_ERROR  GetUserOrderAndTraderGESeq(const std::string & nInvestorID,
		int nOrderSeq,
		int nTraderSeq,
		std::vector<PlatformStru_OrderInfo> & nvecOrder,
		std::vector<PlatformStru_TradeInfo>& nvecTrader) = 0;


public://�ֲֻ��ܲ���

	//��ȡ�ֲֻ��ܣ�
	virtual CF_ERROR  GetUserPositions(const std::string & nUserID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData,
		bool bNeedZeroVolumePosition = false) = 0;


	//��ȡ�ֲܳ���
	virtual CF_ERROR GetUserPositionVolume(const std::string & nUserID,
		const std::string & nInstrumentID,
		int & nVolume) = 0;

	//��ȡ�ֲֻ��ܣ�
	virtual CF_ERROR  GetAccountPositions(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData,
		bool bNeedZeroVolumePosition = false) = 0;


public: //�ֲ���ϸ����

	//��ȡ�ֲ���ϸ
	//�ɹ�����CF_ERROR_SUCCESS
	virtual CF_ERROR  GetUserPositionDetail(const std::string & nUserID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetail>& outData) = 0;

	//��ȡ�ֲ���ϸ�����ض�Ӧ��key�ĳֲ���ϸ
	//�ɹ�����CF_ERROR_SUCCESS
	virtual CF_ERROR  GetUserPositionDetail(const std::string & nUserID,
		const PositionDetailKey & nKey,
		PlatformStru_PositionDetail& outData) = 0;


	//��ȡ�ֲ���ϸ
	//�ɹ�����CF_ERROR_SUCCESS
	virtual CF_ERROR  GetAccountPositionDetail(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetail>& outData) = 0;

	//��ȡ�ֲ���ϸ�����ض�Ӧ��key�ĳֲ���ϸ
	//�ɹ�����CF_ERROR_SUCCESS
	virtual CF_ERROR  GetAccountPositionDetail(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		const PositionDetailKey & nKey,
		PlatformStru_PositionDetail& outData) = 0;


public: //��ϳֲֻ��ܲ���
	//��ȡ��ϳֲ֣����������������
	virtual CF_ERROR  GetUserPositionComb(const std::string & nUserID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData) = 0;

public: //��ϳֲ���ϸ����

	//��ȡ��ϳֲ���ϸ�����������������
	virtual CF_ERROR  GetUserPositionCombDetail(const std::string & nUserID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetailComb>& outData) = 0;

public: //�ʽ����
	//��ȡ����Ա�ʽ���Ϣ
	virtual CF_ERROR  GetUserFundInfo(const std::string & nUserID,
		PlatformStru_TradingAccountInfo& outData) = 0;

	//��ȡ����Ա��ʼ�ʽ�
	virtual CF_ERROR  GetUserInitFund(const std::string & nUserID,
		double& outData) = 0;

	//��ȡί�н����ʺ��ʽ���Ϣ
	virtual CF_ERROR  GetAccountFundInfo(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		PlatformStru_TradingAccountInfo& outData) = 0;

public: //�ṩ������Ľӿڻ��߻�ȡ���ܲ���

	//��ȡ����ί�н����˺ŵ���������
	virtual CF_ERROR GetAllCommissionRate(
		std::vector<PlatformStru_InstrumentCommissionRate> & outData) = 0;

	//��ȡ����ί�н����˺ŵı�֤����
	virtual CF_ERROR GetAllMarginRate(
		std::vector<PlatformStru_InstrumentMarginRate> & outData) = 0;


	//��ȡ��ǰ�����յĺ�Լ
	virtual CF_ERROR  GetCurrentDayAlInstrumentInfo(
		std::vector<PlatformStru_InstrumentInfo> & outData) = 0;


	//��ȡ���еĽ���ۣ�ֻҪ��һ����Լ�н���ۣ��ͷ�����ȷ
	virtual CF_ERROR  GetAllSettlementPrice(
		std::map<std::string,PlatformStru_DepthMarketData> & outData) = 0;



	//��ȡ��ǰ�����յĽ����
	virtual CF_ERROR  GetCurrentDayAllSettlementPrice(
		std::map<std::string,PlatformStru_DepthMarketData> & outData) = 0;

	//��ȡ���еĳɽ�
	virtual CF_ERROR  GetAllTradeInfo(
		std::map<std::string,std::vector<PlatformStru_TradeInfo>> & outData) = 0;

	//��ȡ���еĳֲ���ϸ
	virtual CF_ERROR  GetAllPositionDetail(
		std::map<std::string,std::vector<PlatformStru_PositionDetail>> & outData) = 0;

	//��ȡ���еĳֲ�
	virtual CF_ERROR  GetAllPosition(
		std::map<std::string,std::vector<PlatformStru_Position>> & outData) = 0;

	//��ȡ���е��ʽ�
	virtual CF_ERROR  GetAllFundInfo(
		std::map<std::string,PlatformStru_TradingAccountInfo> & outData) = 0;

	//
	//��ʼ���㣬�������ʷ���㣬�贫������
	//�ɹ����أ�CF_ERROR_SUCCESS
	//ʧ�ܷ��أ�
	//         
	//
	virtual CF_ERROR  StartSettlement(eSettlementLoadOption nOption,
		const std::string & nsDay ,
		bool nbHistorySettlement) = 0;


	//��������
	virtual CF_ERROR  EndSettlement(const std::string & nsDay) = 0;

	//�����޸ĺ���������ʺͱ�֤���ʣ��������ɽ��ͳֲֺ��ʽ�ļ���
	virtual CF_ERROR  SetModifiedCommissionAndMargin( std::vector<PlatformStru_InstrumentCommissionRate> & inCommssion,
		 std::vector<PlatformStru_InstrumentMarginRate> & inMargin) = 0;

	//�����޸ĺ�ĳɽ����������ɽ��ͳֲֺ��ʽ�ļ���
	virtual CF_ERROR  SeUserNewTrades(const std::string & nUserName,
		const std::vector<PlatformStru_TradeInfo> & inTradeInfo) = 0;
	

public:
	//�µ��жϸ��û��ú�Լ���������ʺͱ�֤�����Ƿ�׼������
	virtual CF_ERROR CheckUserInstrumentCommissionAndMaginExist(const std::string & nUserName,
		const std::string & nIstrumentID) = 0;
	
	//�ж��Ƿ�ɿ���
	virtual CF_ERROR  CanOrderOpen(const PlatformStru_OrderInfo & nOrder) = 0;

	//�ж��Ƿ��ƽ��
	virtual CF_ERROR  CanOrderColseToday(const PlatformStru_OrderInfo & nOrder) = 0;

	//�ж��Ƿ��ƽ��
	virtual CF_ERROR  CanOrderColse(const PlatformStru_OrderInfo & nOrder) = 0;

public:
	//�����
	virtual CF_ERROR FundInOut(const sFundInOut& nFundInOut) = 0;

	//��ѯ��ʷ�����
	virtual CF_ERROR QryHistroyFundInOut(const sQryFundInOut & nQryFund,
		std::vector<sFundInOut> & nvecFund) = 0;


public:
	////���һ��CTP����
	virtual CF_ERROR QryCTPOrder(PlatformStru_OrderInfo & nOrderInfo,const int ErrID,const bool bIsLast) = 0 ;

	//���һ��CTP�ɽ�
	virtual CF_ERROR QryCTPTrader(PlatformStru_TradeInfo & nTraderInfo,const int ErrID,const bool bIsLast) = 0;

	////���һ��CTP����
	virtual CF_ERROR PushCTPOrder(PlatformStru_OrderInfo & nOrderInfo) = 0;

	//���һ��CTP�ɽ�
	virtual CF_ERROR PushCTPTrader(PlatformStru_TradeInfo & nTraderInfo) = 0;



	////��ӿͻ����µ�
	virtual CF_ERROR PushExecuteInOrder(PlatformStru_OrderInfo & nOrderInfo) = 0;

	//���һ���ͻ����µ��ķ��ؽ��
	virtual CF_ERROR PushExecuteOutOrder(PlatformStru_OrderInfo & nOraderInfo) = 0;

public:
	//��ȡ�������ݿ�Ľӿ�
	virtual IInterfaceDBOpr * GetInterfaceDBOpr() = 0;

	//�鿴�û��Ƿ�ȷ���˽��㵥
	virtual bool IsUserSettlementConfirmed(const std::string & nUserName,
		std::string & nsConfirmedDate,
		std::string & nsConfirmedTime) = 0;

	//�û�ȷ���˽��㵥
	virtual void SetUserSettlementConfirmed(const std::string & nsUserName,
		const std::string & nsConfirmedDate,
		const std::string & nsConfirmedTime) = 0 ;

public:
	//��ȡ��ʼ��ʱ��
	virtual CF_ERROR GetReinitTime(sReinitTime & nsReinitTime) = 0;

	//��ȡ���ݱ���ʱ��
	virtual CF_ERROR GetSaveTime(sSaveTime & nsReinitTime) = 0 ;

	//���ó�ʼ��ʱ��
	virtual CF_ERROR SetReinitTime(const sReinitTime & nsReinitTime) = 0;

	//��ȡ����ʱ��
	virtual CF_ERROR GetTradingTime(sTradingTime & nsTradingTime) = 0 ;


	//���ý���ʱ��
	virtual CF_ERROR SetTradingTime(const sTradingTime & nsTradingTime) = 0;

public: //��������
	virtual void GenerateOrderInfo(const PlatformStru_InputOrder& inputOrder,
		PlatformStru_OrderInfo & orderInfo) = 0;

	virtual void GenerateInputOrder(const PlatformStru_OrderInfo& orderInfo,
		PlatformStru_InputOrder & inputOrder) = 0;

public:
	//���̲�ѯ���ɹ�����
	virtual void QryFailed(const BrokerAccountKey & nBrokerAccount,eEventType nEvent) = 0;

	//��ȡ��Ӧ�û���Ӧʱ��εĳֲ���ϸ
	virtual CF_ERROR GetPositionDetail_MaxTime(const  std::vector<std::string> nvecAccount, 
		int LowerOpenTime,int UpperOpenTime,
		std::map<std::string, std::vector<PlatformStru_PositionDetail>>& mapDetail) = 0;



public:
	virtual bool PushStrategyStopOrRun(const std::string & nsName,bool nsStop) = 0;

	//virtual	bool PushTraderUseroffLine(const std::string & nUserName,bool bOffline) = 0;

	virtual bool PushStrategyInstanceStopOrRun(const std::string & nsUserName,
		const std::string & nsStategyName,bool nsStop) = 0;

// 	virtual bool GetStategyUserInstanceUpdateTime(const std::string & nsUserName,
// 		const std::string & nsStategyName,
// 		time_t & nsUpdateTime) = 0;

    virtual bool PushStrategyTimer(const std::string & nsName,bool nsStop) = 0 ;

    virtual bool ConnectRedisServer() = 0;
    virtual bool DisConnectRedisServer() = 0;
};







