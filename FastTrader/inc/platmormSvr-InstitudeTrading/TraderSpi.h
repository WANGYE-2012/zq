/***************************************************************************//**
*   @file          TraderSpi.hpp
*   @brief        综合交易平台(CTP)交易相关的回调对象
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/09/13
********************************************************************************/
#ifndef _TRADERSPI_HPP_
#define _TRADERSPI_HPP_


//#include "ctp\\ThostFtdcTraderApi.h"
#include "InstitudeTraderApi.h"
#include "ISvr/IPlatformSingleSvr.h"

class CInstitudeTraderApi;
class CPlatFormService;
class CPackageCache;
class CPlatFormParam;
class zqWriteLog;

//class CTraderSpi
//      :public CThostFtdcTraderSpi
class CTraderSpi
{
public:
    
    CTraderSpi(CPlatFormService& RefPlatFormService);
    ~CTraderSpi();
private:
    CTraderSpi(const CTraderSpi&);
    CTraderSpi& operator= (const CTraderSpi&);
    
    //CThostFtdcTraderApi& m_RefTraderApi;
    CInstitudeTraderApi& m_RefTraderApi;
    CPackageCache&       m_RefPackageCache;
    CPlatFormParam&      m_PlatformParam;
    CPlatFormService&    m_RefPlatFormService;
    zqWriteLog*           m_pWriteLog;


public:
    ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	void OnFrontConnected();
	
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	void OnFrontDisconnected(int nReason);
		
	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	void OnHeartBeatWarning(int nTimeLapse);
#ifdef _NEED_AUTHENTICATE
	///客户端认证响应
	void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
#endif
	///登录请求响应
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///登出请求响应
	void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///用户口令更新请求响应
	void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///资金账户口令更新请求响应
	void OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///报单录入请求响应
    ///根据“综合交易平台API开发FAQ”: 
    ///     CTP在检查委托发现错误时，会给发出委托的投资者发出OnRspInsertOrder，同时发出OnErrRtnOrder给相关的交易员，所以，作为投资者可以不关心OnErrRtnOrder
	void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///报单操作请求响应
	void OnRspOrderAction(PlatformStru_InputOrderAction *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///查询最大报单数量响应
	void OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///投资者结算结果确认响应
	void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    ///查询保证金监管系统经纪公司资金账户密钥响应
    void OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


	///预埋单录入请求响应
	void OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///预埋撤单录入请求响应
	void OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///删除预埋单响应
	void OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///删除预埋撤单响应
	void OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询预埋单响应
	void OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询预埋撤单响应
	void OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;


	///请求查询报单响应
	void OnRspQryOrder(PlatformStru_OrderInfo *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询成交响应
	void OnRspQryTrade(PlatformStru_TradeInfo *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询投资者持仓响应
	void OnRspQryInvestorPosition(PlatformStru_Position *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询资金账户响应
	void OnRspQryTradingAccount(PlatformStru_TradingAccountInfo *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询投资者响应
	void OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询交易编码响应
	void OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询合约保证金率响应
	void OnRspQryInstrumentMarginRate(PlatformStru_InstrumentMarginRate *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询合约手续费率响应
	void OnRspQryInstrumentCommissionRate(PlatformStru_InstrumentCommissionRate *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询交易所响应
	void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询合约响应
	void OnRspQryInstrument(PlatformStru_InstrumentInfo *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询行情响应
	void OnRspQryDepthMarketData(PlatformStru_DepthMarketData *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询投资者结算结果响应
	void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询转帐银行响应
	void OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询投资者持仓明细响应
	void OnRspQryInvestorPositionDetail(PlatformStru_PositionDetail *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询客户通知响应
	void OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询结算信息确认响应
	void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询投资者持仓明细响应
	void OnRspQryInvestorPositionCombineDetail(PlatformStru_PositionDetailComb *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询转帐流水响应
	void OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///错误应答
	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///报单通知
	void OnRtnOrder(PlatformStru_OrderInfo *pOrder) ;

	///成交通知
	void OnRtnTrade(PlatformStru_TradeInfo *pTrade) ;

	///报单录入错误回报
	void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) ;

	///报单操作错误回报
	void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) ;

	///合约交易状态通知
	void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) ;

	///交易通知
	void OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) ;

	///请求查询签约银行响应
	void OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询交易通知响应
	void OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询经纪公司交易参数响应
	void OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询经纪公司交易算法响应
	void OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///银行发起银行资金转期货通知
	void OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer) ;

	///银行发起期货资金转银行通知
	void OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer) ;

	///银行发起冲正银行转期货通知
	void OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal) ;

	///银行发起冲正期货转银行通知
	void OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal) ;

	///期货发起银行资金转期货通知
	void OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer) ;

	///期货发起期货资金转银行通知
	void OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer) ;

	///系统运行时期货端手工发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
	void OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal) ;

	///系统运行时期货端手工发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
	void OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal) ;

	///期货发起查询银行余额通知
	void OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount) ;

    ///请求查询银期签约关系响应
    void OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///期货发起银行资金转期货错误回报
	void OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) ;

	///期货发起期货资金转银行错误回报
	void OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) ;

	///系统运行时期货端手工发起冲正银行转期货错误回报
	void OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) ;

	///系统运行时期货端手工发起冲正期货转银行错误回报
	void OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) ;

	///期货发起查询银行余额错误回报
	void OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo) ;

	///期货发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
	void OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal) ;

	///期货发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
	void OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal) ;

	///期货发起银行资金转期货应答
	void OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///期货发起期货资金转银行应答
	void OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///期货发起查询银行余额应答
	void OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询汇率响应
	void OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
	void CopyInstrumentField( PlatformStru_InstrumentInfo& data, const CThostFtdcInstrumentField& instrumentField );
	void CopyTradeRecordField( PlatformStru_TradeInfo& data, const CThostFtdcTradeField& tradeField );
	void CopyPositionField( PlatformStru_Position&data, const CThostFtdcInvestorPositionField& positionField );
	void CopyPositionDetailField( PlatformStru_PositionDetail&data, const CThostFtdcInvestorPositionDetailField& positionField );
    void CopyPositionCombDetailField( PlatformStru_PositionDetailComb& data, const CThostFtdcInvestorPositionCombineDetailField& positionCombField);
	//void CopyPositionCombDetailField2( PlatformStru_PositionCombDetail2& New, const CThostFtdcInvestorPositionCombineDetailField& Old);
    void CopyOrderField(PlatformStru_OrderInfo& data,const CThostFtdcOrderField& orderField);
};


#endif


