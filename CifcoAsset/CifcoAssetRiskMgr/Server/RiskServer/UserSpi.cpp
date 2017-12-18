#include "StdAfx.h"
#include "UserSpi.h"
#include "RiskMsgQueue.h"
#include "ConvertFromCtp.h"
#include "DumpFromCtp.h"
#include "Tools/WriteLog.h"
//#if 0
//#define LOG_INFO(fmt, ...) 
//#else
bool g_NeedSaveUserSpi=false;
#define LOG_INFO(fmt, ...) \
	do{\
	if(g_NeedSaveUserSpi&&m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("UserSpi", WriteLogLevel_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
//#endif
#define SAFE_ADD_MSG		if(CRiskMsgQueue::Get()) CRiskMsgQueue::Get()->AddMsg
#define SAFE_ADD_MSG_QUOT	if(CRiskMsgQueue_Quot::Get()) CRiskMsgQueue_Quot::Get()->AddMsg
#define SAFE_ADD_MSG_SYNC	if(CRiskMsgQueue_Sync::Get()) CRiskMsgQueue_Sync::Get()->AddMsg
#define CONVERTSTRUCTFROMCTP(TYPE)\
	TYPE##Field* np##TYPE=new TYPE##Field();\
	std::auto_ptr<TYPE##Field> atuo_p##TYPE(np##TYPE);\
	std::string str##TYPE;\
	DumpFromCtp::Dump##TYPE##Field(str##TYPE,*p##TYPE);\
	LOG_INFO("%s",str##TYPE.c_str());\
	ConvertFromCtp::Get##TYPE##Field(*p##TYPE,*np##TYPE);
#define SAVE_MAX(tag,no) if(tag&&tag->UniqSequenceNo>m_MaxNo.no) m_MaxNo.no=tag->UniqSequenceNo

UserSpi::UserSpi(MaxNo& maxno)
:m_MaxNo(maxno)
{
	m_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE,"UserSpi.log");
}

UserSpi::~UserSpi(void)
{
	SAFE_DELETE(m_pWriteLog);
}
///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�	
void UserSpi::OnFrontConnected()	
{	
	LOG_INFO("OnFrontConnected");
	SAFE_ADD_MSG(MSGID_FrontConnected,0,0);
}	


///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������	
///@param nReason ����ԭ��	
///        0x1001 �����ʧ��	
///        0x1002 ����дʧ��	
///        0x2001 ����������ʱ	
///        0x2002 ��������ʧ��	
///        0x2003 �յ�������	
void UserSpi::OnFrontDisconnected(int nReason)	
{	
	LOG_INFO("OnFrontDisconnected(nReason=%d)\n",nReason);
	SAFE_ADD_MSG(MSGID_FrontDisconnected,nReason);
}	


///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�	
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��	
void UserSpi::OnHeartBeatWarning(int nTimeLapse)	
{	
	LOG_INFO("OnHeartBeatWarning(%d)\n",nTimeLapse);
	SAFE_ADD_MSG(MSGID_HeartBeatWarning,nTimeLapse);
}	
///����Ӧ��			
void UserSpi::OnRspError(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			
	LOG_INFO("OnRspError");		
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspError,0,0,npRspInfo,nRequestID,bIsLast);		
}			


///ϯλ�ʽ�ر�			
void UserSpi::OnRtnBrokerDeposit(CShfeFtdcBrokerDepositField *pBrokerDeposit) 			
{			
	LOG_INFO("OnRtnBrokerDeposit");		
	CONVERTSTRUCTFROMCTP(BrokerDeposit);	
	SAFE_ADD_MSG(MSGID_RtnBrokerDeposit,sizeof(BrokerDepositField),npBrokerDeposit );		
}			


///Ͷ������ϢժҪ			
void UserSpi::OnRtnInvestorSumInfo(CShfeFtdcInvestorSumInfoField *pInvestorSumInfo) 			
{			
	LOG_INFO("OnRtnInvestorSumInfo");		
	CONVERTSTRUCTFROMCTP(InvestorSumInfo);	
	SAFE_ADD_MSG(MSGID_RtnInvestorSumInfo,sizeof(InvestorSumInfoField),npInvestorSumInfo );		
}			


///�ͻ��˽���������ͬ����ʼ			
void UserSpi::OnRtnClientSGDataSyncStart(CShfeFtdcSettlementSessionField *pSettlementSession) 			
{			
	LOG_INFO("OnRtnClientSGDataSyncStart");		
	CONVERTSTRUCTFROMCTP(SettlementSession);	
	SAFE_ADD_MSG(MSGID_RtnClientSGDataSyncStart,sizeof(SettlementSessionField),npSettlementSession );		
}			


///�ͻ��˽���������ͬ������			
void UserSpi::OnRtnClientSGDataSyncEnd(CShfeFtdcSettlementSessionField *pSettlementSession) 			
{			
	LOG_INFO("OnRtnClientSGDataSyncEnd");		
	CONVERTSTRUCTFROMCTP(SettlementSession);	
	SAFE_ADD_MSG(MSGID_RtnClientSGDataSyncEnd,sizeof(SettlementSessionField),npSettlementSession );		
}			


///��¼������Ӧ			
void UserSpi::OnRspRiskUserLogin(CShfeFtdcRspRiskUserLoginField *pRspRiskUserLogin, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			
	LOG_INFO("OnRspRiskUserLogin");		
	CONVERTSTRUCTFROMCTP(RspRiskUserLogin);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRiskUserLogin,sizeof(RspRiskUserLoginField),npRspRiskUserLogin,npRspInfo,nRequestID,bIsLast);		
}			


///			
void UserSpi::OnRspQryInvestorMarginRate(CShfeFtdcInvestorMarginRateField *pInvestorMarginRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryInvestorMarginRate");		
	CONVERTSTRUCTFROMCTP(InvestorMarginRate);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryInvestorMarginRate,sizeof(InvestorMarginRateField),npInvestorMarginRate,npRspInfo,nRequestID,bIsLast);		
}			


///��Ʒ��Ϣ			
void UserSpi::OnRtnProduct(CShfeFtdcProductField *pProduct) 			
{			
	LOG_INFO("OnRtnProduct");		
	CONVERTSTRUCTFROMCTP(Product);	
	SAFE_ADD_MSG(MSGID_RtnProduct,sizeof(ProductField),npProduct );		
}			


///��Լ��Ϣ			
void UserSpi::OnRtnInstrument(CShfeFtdcInstrumentField *pInstrument) 			
{			
	LOG_INFO("OnRtnInstrument");		
	CONVERTSTRUCTFROMCTP(Instrument);	
	SAFE_ADD_MSG(MSGID_RtnInstrument,sizeof(InstrumentField),npInstrument );		
}			


///��ѯ��������Ӧ��			
void UserSpi::OnRspQryOrderStat(CShfeFtdcOrderStatField *pOrderStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			
	LOG_INFO("OnRspQryOrderStat");	
	CONVERTSTRUCTFROMCTP(OrderStat);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryOrderStat,sizeof(OrderStatField),npOrderStat,npRspInfo,nRequestID,bIsLast);		
}			


///�������Ļ�����Ϣ			
void UserSpi::OnRtnExchange(CShfeFtdcExchangeField *pExchange) 			
{			
	LOG_INFO("OnRtnExchange");		
	CONVERTSTRUCTFROMCTP(Exchange);	
	SAFE_ADD_MSG(MSGID_RtnExchange,sizeof(ExchangeField),npExchange );		
}			


///Ͷ���ֲ߳�ͳ�Ʋ�ѯӦ��-ReqInvestorPosition, ftd version 6 deprecated			
void UserSpi::OnRspInvestorPositionStatic(CShfeFtdcInvestorPositionStaticField *pInvestorPositionStatic, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			
	LOG_INFO("OnRspInvestorPositionStatic");	
	CONVERTSTRUCTFROMCTP(InvestorPositionStatic);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspInvestorPositionStatic,sizeof(InvestorPositionStaticField),npInvestorPositionStatic,npRspInfo,nRequestID,bIsLast);		
}			


///Ͷ���߳ɽ�ͳ�Ʋ�ѯӦ��-ReqInvestorTrade, ftd version 6 deprecated			
void UserSpi::OnRspInvestorTradeStatic(CShfeFtdcInvestorTradeStaticField *pInvestorTradeStatic, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			
	LOG_INFO("OnRspInvestorTradeStatic");		
	CONVERTSTRUCTFROMCTP(InvestorTradeStatic);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspInvestorTradeStatic,sizeof(InvestorTradeStaticField),npInvestorTradeStatic,npRspInfo,nRequestID,bIsLast);		
}			


///����������֪ͨ			
void UserSpi::OnRtnRiskDepthMarketData(CShfeFtdcDepthMarketDataField *pDepthMarketData) 			
{			
	LOG_INFO("OnRtnRiskDepthMarketData");	
	CONVERTSTRUCTFROMCTP(DepthMarketData);	
	SAFE_ADD_MSG_QUOT(MSGID_RtnRiskDepthMarketData,sizeof(DepthMarketDataField),npDepthMarketData );		
}			


///ʱ��ͬ��			
void UserSpi::OnRtnTimeSync(CShfeFtdcCurrentTimeField *pCurrentTime) 			
{			
	LOG_INFO("OnRtnTimeSync");		
	CONVERTSTRUCTFROMCTP(CurrentTime);	
	SAFE_ADD_MSG(MSGID_RtnTimeSync,sizeof(CurrentTimeField),npCurrentTime );		
}			


///��Լ�ֱֲ���Ӧ��			
void UserSpi::OnRspInstPositionRate(CShfeFtdcRspInstPositionRateField *pRspInstPositionRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			
	LOG_INFO("OnRspInstPositionRate");	
	CONVERTSTRUCTFROMCTP(RspInstPositionRate);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspInstPositionRate,sizeof(RspInstPositionRateField),npRspInstPositionRate,npRspInfo,nRequestID,bIsLast);		
}			


///��Ʒ�ֱֲ���Ӧ��			
void UserSpi::OnRspProductPositionRate(CShfeFtdcRspProductPositionRateField *pRspProductPositionRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			
	LOG_INFO("OnRspProductPositionRate");		
	CONVERTSTRUCTFROMCTP(RspProductPositionRate);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspProductPositionRate,sizeof(RspProductPositionRateField),npRspProductPositionRate,npRspInfo,nRequestID,bIsLast);		
}			


///Ͷ������			
void UserSpi::OnRtnInvestorGroup(CShfeFtdcInvestorGroupField *pInvestorGroup) 			
{			
	LOG_INFO("OnRtnInvestorGroup");		
	CONVERTSTRUCTFROMCTP(InvestorGroup);	
	SAFE_ADD_MSG(MSGID_RtnInvestorGroup,sizeof(InvestorGroupField),npInvestorGroup );		
}			


///��������֤���ʣ�ɾ������RtnDelRtnExchangeMarginRate			
void UserSpi::OnRtnExchangeMarginRate(CShfeFtdcExchangeMarginRateField *pExchangeMarginRate) 			
{			
	LOG_INFO("OnRtnExchangeMarginRate");	
	CONVERTSTRUCTFROMCTP(ExchangeMarginRate);	
	SAFE_ADD_MSG(MSGID_RtnExchangeMarginRate,sizeof(ExchangeMarginRateField),npExchangeMarginRate );		
}			


///��������֤���ʵ�����ɾ������RtnDelRtnExchangeMarginRateAdjust			
void UserSpi::OnRtnExchangeMarginRateAdjust(CShfeFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust) 			
{			
	LOG_INFO("OnRtnExchangeMarginRateAdjust");		
	CONVERTSTRUCTFROMCTP(ExchangeMarginRateAdjust);	
	SAFE_ADD_MSG(MSGID_RtnExchangeMarginRateAdjust,sizeof(ExchangeMarginRateAdjustField),npExchangeMarginRateAdjust );		
}			


///��ѯ���ױ���HashֵӦ��			
void UserSpi::OnRspQryTradingCodeHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryTradingCodeHash");		
	CONVERTSTRUCTFROMCTP(InvestorHash);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryTradingCodeHash,sizeof(InvestorHashField),npInvestorHash,npRspInfo,nRequestID,bIsLast);		
}			


///��ѯ���ױ���Ӧ��			
void UserSpi::OnRspQryTradingCode(CShfeFtdcTradingCodeField *pTradingCode, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryTradingCode");		
	CONVERTSTRUCTFROMCTP(TradingCode);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryTradingCode,sizeof(TradingCodeField),npTradingCode,npRspInfo,nRequestID,bIsLast);		
}			


///���ױ���ر�			
void UserSpi::OnRtnTradingCode(CShfeFtdcTradingCodeField *pTradingCode) 			
{			
	LOG_INFO("OnRtnTradingCode");		
	CONVERTSTRUCTFROMCTP(TradingCode);	
	SAFE_ADD_MSG(MSGID_RtnTradingCode,sizeof(TradingCodeField),npTradingCode );		
}			


///ɾ�����ױ���ر�			
void UserSpi::OnRtnDelTradingCode(CShfeFtdcTradingCodeField *pTradingCode) 			
{			
	LOG_INFO("OnRtnDelTradingCode");		
	CONVERTSTRUCTFROMCTP(TradingCode);	
	SAFE_ADD_MSG(MSGID_RtnDelTradingCode,sizeof(TradingCodeField),npTradingCode );		
}			


///����ľ��͹�˾�û��¼��ر�����Ӧ��������ReqSubBrokerUserEvent			
void UserSpi::OnRtnSequencialBrokerUserEvent(CShfeFtdcSequencialBrokerUserEventField *pSequencialBrokerUserEvent) 			
{			
	LOG_INFO("OnRtnSequencialBrokerUserEvent");		
	CONVERTSTRUCTFROMCTP(SequencialBrokerUserEvent);	
	SAFE_ADD_MSG(MSGID_RtnSequencialBrokerUserEvent,sizeof(SequencialBrokerUserEventField),npSequencialBrokerUserEvent );		
}			


///����ĳɽ��ر�����Ӧ��������ReqSubscribeTrade			
void UserSpi::OnRtnSequencialTrade(CShfeFtdcSequencialTradeField *pSequencialTrade) 			
{
	SAVE_MAX(pSequencialTrade,MaxTradeNo);
	LOG_INFO("OnRtnSequencialTrade");		
	CONVERTSTRUCTFROMCTP(SequencialTrade);	
	SAFE_ADD_MSG(MSGID_RtnSequencialTrade,sizeof(SequencialTradeField),npSequencialTrade );		
}			


///����ı����ر�����Ӧ��������ReqSubscribeOrder			
void UserSpi::OnRtnSequencialOrder(CShfeFtdcSequencialOrderField *pSequencialOrder) 			
{
	SAVE_MAX(pSequencialOrder,MaxOrderNo);			
	LOG_INFO("OnRtnSequencialOrder");		
	CONVERTSTRUCTFROMCTP(SequencialOrder);
	SAFE_ADD_MSG(MSGID_RtnSequencialOrder,sizeof(SequencialOrderField),npSequencialOrder );		
}			


///��ر���¼��������Ӧ			
void UserSpi::OnRspRiskOrderInsert(CShfeFtdcInputOrderField *pInputOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRiskOrderInsert");		
	CONVERTSTRUCTFROMCTP(InputOrder);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRiskOrderInsert,sizeof(InputOrderField),npInputOrder,npRspInfo,nRequestID,bIsLast);		
}			


///��ر�������������Ӧ			
void UserSpi::OnRspRiskOrderAction(CShfeFtdcInputOrderActionField *pInputOrderAction, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRiskOrderAction");		
	CONVERTSTRUCTFROMCTP(InputOrderAction);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRiskOrderAction,sizeof(InputOrderActionField),npInputOrderAction,npRspInfo,nRequestID,bIsLast);		
}			


///����ĳֲֻر�����Ӧ��������ReqSubscribePosition			
void UserSpi::OnRtnSequencialPosition(CShfeFtdcSequencialPositionField *pSequencialPosition) 			
{
	SAVE_MAX(pSequencialPosition,MaxPositionNo);				
	LOG_INFO("OnRtnSequencialPosition");		
	CONVERTSTRUCTFROMCTP(SequencialPosition);	
	SAFE_ADD_MSG(MSGID_RtnSequencialPosition,sizeof(SequencialPositionField),npSequencialPosition );		
}			


///�ͻ�����֪ͨ��������Ӧ��			
void UserSpi::OnRspRiskNotifyCommand(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRiskNotifyCommand");	
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRiskNotifyCommand,0,0,npRspInfo,nRequestID,bIsLast);		
}			


///����ǿƽ��������Ӧ��			
void UserSpi::OnRspBatchForceCloseCalc(CShfeFtdcRspForceClosePositionField *pRspForceClosePosition, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspBatchForceCloseCalc");		
	CONVERTSTRUCTFROMCTP(RspForceClosePosition);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspBatchForceCloseCalc,sizeof(RspForceClosePositionField),npRspForceClosePosition,npRspInfo,nRequestID,bIsLast);		
}			


///����Ͷ����ǿƽ��������Ӧ��			
void UserSpi::OnRspForceCloseCalc(CShfeFtdcRspForceClosePositionField *pRspForceClosePosition, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspForceCloseCalc");	
	CONVERTSTRUCTFROMCTP(RspForceClosePosition);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspForceCloseCalc,sizeof(RspForceClosePositionField),npRspForceClosePosition,npRspInfo,nRequestID,bIsLast);		
}			


///���ֱֲ�֤��ָ���������Ӧ��			
void UserSpi::OnRspSetIndexNPPPara(CShfeFtdcIndexNPPField *pIndexNPP, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspSetIndexNPPPara");		
	CONVERTSTRUCTFROMCTP(IndexNPP);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspSetIndexNPPPara,sizeof(IndexNPPField),npIndexNPP,npRspInfo,nRequestID,bIsLast);		
}			


///���ֱֲ�֤��ָ��ر�����ɾ��ָ�꣬��ʹ��RtnDelIndexNPP			
void UserSpi::OnRtnIndexNPP(CShfeFtdcFullIndexNPPField *pFullIndexNPP) 			
{			
	LOG_INFO("OnRtnIndexNPP");		
	CONVERTSTRUCTFROMCTP(FullIndexNPP);	
	SAFE_ADD_MSG(MSGID_RtnIndexNPP,sizeof(FullIndexNPPField),npFullIndexNPP );		
}			


///ɾ�����ֱֲ�֤��ָ�����Ӧ��			
void UserSpi::OnRspRiskDelIndexNPPPara(CShfeFtdcIndexNPPField *pIndexNPP, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRiskDelIndexNPPPara");		
	CONVERTSTRUCTFROMCTP(IndexNPP);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRiskDelIndexNPPPara,sizeof(IndexNPPField),npIndexNPP,npRspInfo,nRequestID,bIsLast);		
}			


///ǿƽ���������ʽ��˻�Ӧ��			
void UserSpi::OnRspForceCloseAccount(CShfeFtdcInvestorRiskAccountField *pInvestorRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspForceCloseAccount");		
	CONVERTSTRUCTFROMCTP(InvestorRiskAccount);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspForceCloseAccount,sizeof(InvestorRiskAccountField),npInvestorRiskAccount,npRspInfo,nRequestID,bIsLast);		
}			


///��ѯ��½����Ӧ��			
void UserSpi::OnRspQryLogin(CShfeFtdcNormalRiskQueryField *pNormalRiskQuery, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryLogin");	
	CONVERTSTRUCTFROMCTP(NormalRiskQuery);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryLogin,sizeof(NormalRiskQueryField),npNormalRiskQuery,npRspInfo,nRequestID,bIsLast);		
}			


///��ѯ��ȫ�۸񲨶���ΧӦ��			
void UserSpi::OnRspQrySafePriceRange(CShfeFtdcRspSafePriceRangeField *pRspSafePriceRange, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQrySafePriceRange");		
	CONVERTSTRUCTFROMCTP(RspSafePriceRange);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQrySafePriceRange,sizeof(RspSafePriceRangeField),npRspSafePriceRange,npRspInfo,nRequestID,bIsLast);		
}			


///��ѯ��ȫ�۸񲨶���Χ�ʽ�Ӧ��			
void UserSpi::OnRspSafePriceAccount(CShfeFtdcInvestorRiskAccountField *pInvestorRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspSafePriceAccount");		
	CONVERTSTRUCTFROMCTP(InvestorRiskAccount);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspSafePriceAccount,sizeof(InvestorRiskAccountField),npInvestorRiskAccount,npRspInfo,nRequestID,bIsLast);		
}			


///��ѯ�۸񲨶���Ͷ���߷��յ�Ӱ��Ӧ��			
void UserSpi::OnRspQryPriceVaryEffect(CShfeFtdcPriceVaryParamField *pPriceVaryParam, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryPriceVaryEffect");	
	CONVERTSTRUCTFROMCTP(PriceVaryParam);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryPriceVaryEffect,sizeof(PriceVaryParamField),npPriceVaryParam,npRspInfo,nRequestID,bIsLast);		
}			


///��֯�ܹ�			
void UserSpi::OnRtnDepartment(CShfeFtdcDepartmentField *pDepartment) 			
{			
	LOG_INFO("OnRtnDepartment");		
	CONVERTSTRUCTFROMCTP(Department);	
	SAFE_ADD_MSG(MSGID_RtnDepartment,sizeof(DepartmentField),npDepartment );		
}			


///IndexNPPӦ��			
void UserSpi::OnRspIndexNPP(CShfeFtdcFullIndexNPPField *pFullIndexNPP, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspIndexNPP");		
	CONVERTSTRUCTFROMCTP(FullIndexNPP);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspIndexNPP,sizeof(FullIndexNPPField),npFullIndexNPP,npRspInfo,nRequestID,bIsLast);		
}			


///����ϵͳ����			
void UserSpi::OnRtnTradeParam(CShfeFtdcTradeParamField *pTradeParam) 			
{			
	LOG_INFO("OnRtnTradeParam");		
	CONVERTSTRUCTFROMCTP(TradeParam);	
	SAFE_ADD_MSG(MSGID_RtnTradeParam,sizeof(TradeParamField),npTradeParam );		
}			


///���Ԥ��¼��Ӧ��			
void UserSpi::OnRspRiskParkedOrderInsert(CShfeFtdcRiskParkedOrderField *pRiskParkedOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRiskParkedOrderInsert");		
	CONVERTSTRUCTFROMCTP(RiskParkedOrder);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRiskParkedOrderInsert,sizeof(RiskParkedOrderField),npRiskParkedOrder,npRspInfo,nRequestID,bIsLast);		
}			


///ɾ��Ԥ����Ӧ			
void UserSpi::OnRspRemoveRiskParkedOrder(CShfeFtdcRemoveRiskParkedOrderField *pRemoveRiskParkedOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRemoveRiskParkedOrder");		
	CONVERTSTRUCTFROMCTP(RemoveRiskParkedOrder);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRemoveRiskParkedOrder,sizeof(RemoveRiskParkedOrderField),npRemoveRiskParkedOrder,npRspInfo,nRequestID,bIsLast);		
}			


///����ķ��Ԥ�񵥻ر�			
void UserSpi::OnRtnSeqRiskParkedOrder(CShfeFtdcSeqRiskParkedOrderField *pSeqRiskParkedOrder) 			
{			
	LOG_INFO("OnRtnSeqRiskParkedOrder");		
	CONVERTSTRUCTFROMCTP(SeqRiskParkedOrder);	
	SAFE_ADD_MSG(MSGID_RtnSeqRiskParkedOrder,sizeof(SeqRiskParkedOrderField),npSeqRiskParkedOrder );		
}			


///����û������޸�Ӧ��			
void UserSpi::OnRspRiskUserPasswordUpd(CShfeFtdcUserPasswordUpdateField *pUserPasswordUpdate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRiskUserPasswordUpd");	
	CONVERTSTRUCTFROMCTP(UserPasswordUpdate);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRiskUserPasswordUpd,sizeof(UserPasswordUpdateField),npUserPasswordUpdate,npRspInfo,nRequestID,bIsLast);		
}			


///����ĳ����ر�����Ӧ��������ReqSubSeqDeposit			
void UserSpi::OnRtnSeqDeposit(CShfeFtdcSeqDepositField *pSeqDeposit) 			
{
	SAVE_MAX(pSeqDeposit,MaxDepositNo);				
	LOG_INFO("OnRtnSeqDeposit");		
	CONVERTSTRUCTFROMCTP(SeqDeposit);	
	SAFE_ADD_MSG(MSGID_RtnSeqDeposit,sizeof(SeqDepositField),npSeqDeposit );		
}			


///�������û��¼�Ӧ��			
void UserSpi::OnRspAddRiskUserEvent(CShfeFtdcRiskUserEventField *pRiskUserEvent, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspAddRiskUserEvent");		
	CONVERTSTRUCTFROMCTP(RiskUserEvent);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspAddRiskUserEvent,sizeof(RiskUserEventField),npRiskUserEvent,npRspInfo,nRequestID,bIsLast);		
}			


///����Ԥ���ʽ�Ӧ��			
void UserSpi::OnRspPredictRiskAccount(CShfeFtdcPredictRiskAccountField *pPredictRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspPredictRiskAccount");		
	CONVERTSTRUCTFROMCTP(PredictRiskAccount);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspPredictRiskAccount,sizeof(PredictRiskAccountField),npPredictRiskAccount,npRspInfo,nRequestID,bIsLast);		
}			


///����Ԥ��ֲ�Ӧ��			
void UserSpi::OnRspPredictRiskPosition(CShfeFtdcPredictRiskPositionField *pPredictRiskPosition, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspPredictRiskPosition");		
	CONVERTSTRUCTFROMCTP(PredictRiskPosition);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspPredictRiskPosition,sizeof(PredictRiskPositionField),npPredictRiskPosition,npRspInfo,nRequestID,bIsLast);		
}			


///Ͷ���߻�����Ϣ�ر���ֻͬ�����б仯���ݣ��������{ReqQryInvestorLinkMan,RspQryInvestorLinkMan}ʹ��			
void UserSpi::OnRtnRiskInvestor(CShfeFtdcRiskSyncInvestorField *pRiskSyncInvestor) 			
{			
	LOG_INFO("OnRtnRiskInvestor");		
	CONVERTSTRUCTFROMCTP(RiskSyncInvestor);	
	SAFE_ADD_MSG(MSGID_RtnRiskInvestor,sizeof(RiskSyncInvestorField),npRiskSyncInvestor );		
}			


///��ѯͶ���ߺ���ϵ����ϢӦ��			
void UserSpi::OnRspQryInvestorLinkMan(CShfeFtdcInvestorLinkManField *pInvestorLinkMan, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryInvestorLinkMan");		
	CONVERTSTRUCTFROMCTP(InvestorLinkMan);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryInvestorLinkMan,sizeof(InvestorLinkManField),npInvestorLinkMan,npRspInfo,nRequestID,bIsLast);		
}			


///ɾ����������֤����			
void UserSpi::OnRtnDelExchangeMarginRate(CShfeFtdcExchangeMarginRateField *pExchangeMarginRate) 			
{			
	LOG_INFO("OnRtnDelExchangeMarginRate");		
	CONVERTSTRUCTFROMCTP(ExchangeMarginRate);	
	SAFE_ADD_MSG(MSGID_RtnDelExchangeMarginRate,sizeof(ExchangeMarginRateField),npExchangeMarginRate );		
}			


///ɾ����������֤���ʵ���			
void UserSpi::OnRtnDelExchangeMarginRateAdjust(CShfeFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust) 			
{			
	LOG_INFO("OnRtnDelExchangeMarginRateAdjust");		
	CONVERTSTRUCTFROMCTP(ExchangeMarginRateAdjust);	
	SAFE_ADD_MSG(MSGID_RtnDelExchangeMarginRateAdjust,sizeof(ExchangeMarginRateAdjustField),npExchangeMarginRateAdjust );		
}			


///��ѯ��֯�ܹ�Ͷ���߶�Ӧ��ϵӦ��			
void UserSpi::OnRspQryInvestorDepartment(CShfeFtdcInvestorDepartmentFlatField *pInvestorDepartmentFlat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryInvestorDepartment");		
	CONVERTSTRUCTFROMCTP(InvestorDepartmentFlat);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryInvestorDepartment,sizeof(InvestorDepartmentFlatField),npInvestorDepartmentFlat,npRspInfo,nRequestID,bIsLast);		
}			


///ɾ�����ֱֲ�֤��ָ��ر�����RtnIndexNPP��Ӧ			
void UserSpi::OnRtnDelIndexNPP(CShfeFtdcFullIndexNPPField *pFullIndexNPP) 			
{			
	LOG_INFO("OnRtnDelIndexNPP");		
	CONVERTSTRUCTFROMCTP(FullIndexNPP);	
	SAFE_ADD_MSG(MSGID_RtnDelIndexNPP,sizeof(FullIndexNPPField),npFullIndexNPP );		
}			


///����û�Ȩ�޻ر�			
void UserSpi::OnRtnRiskUserFunction(CShfeFtdcRiskUserFunctionField *pRiskUserFunction) 			
{			
	LOG_INFO("OnRtnRiskUserFunction");		
	CONVERTSTRUCTFROMCTP(RiskUserFunction);	
	SAFE_ADD_MSG(MSGID_RtnRiskUserFunction,sizeof(RiskUserFunctionField),npRiskUserFunction );		
}			


///ɾ������û�Ȩ�޻ر�			
void UserSpi::OnRtnDelRiskUserFunction(CShfeFtdcRiskUserFunctionField *pRiskUserFunction) 			
{			
	LOG_INFO("OnRtnDelRiskUserFunction");		
	CONVERTSTRUCTFROMCTP(RiskUserFunction);	
	SAFE_ADD_MSG(MSGID_RtnDelRiskUserFunction,sizeof(RiskUserFunctionField),npRiskUserFunction );		
}			


///Ͷ���ߵ����ʽ�ر��������ʽ�ʹ��{ReqSubPreRiskAccount,RtnSeqPreRiskAccount}			
void UserSpi::OnRtnRiskSyncAccount(CShfeFtdcRiskSyncAccountField *pRiskSyncAccount) 			
{			
	LOG_INFO("OnRtnRiskSyncAccount");	
	CONVERTSTRUCTFROMCTP(RiskSyncAccount);	
	SAFE_ADD_MSG_SYNC(MSGID_RtnRiskSyncAccount,sizeof(RiskSyncAccountField),npRiskSyncAccount );		
}			


///����������ʽ�ر�����Ӧ��������ReqSubPreRiskAccount			
void UserSpi::OnRtnSeqPreRiskAccount(CShfeFtdcSeqPreRiskAccountField *pSeqPreRiskAccount) 			
{	
	SAVE_MAX(pSeqPreRiskAccount,MaxPreRiskAccountNo);
	LOG_INFO("OnRtnSeqPreRiskAccount");		
	CONVERTSTRUCTFROMCTP(SeqPreRiskAccount);	
	SAFE_ADD_MSG(MSGID_RtnSeqPreRiskAccount,sizeof(SeqPreRiskAccountField),npSeqPreRiskAccount );		
}			


///ģ���滻�ֶλر�			
void UserSpi::OnRtnNoticeToken(CShfeFtdcNoticeTokenField *pNoticeToken) 			
{			
	LOG_INFO("OnRtnNoticeToken");		
	CONVERTSTRUCTFROMCTP(NoticeToken);	
	SAFE_ADD_MSG(MSGID_RtnNoticeToken,sizeof(NoticeTokenField),npNoticeToken );		
}			


///֪ͨģ��ر�			
void UserSpi::OnRtnNoticePattern(CShfeFtdcNoticePatternField *pNoticePattern) 			
{			
	LOG_INFO("OnRtnNoticePattern");		
	CONVERTSTRUCTFROMCTP(NoticePattern);	
	SAFE_ADD_MSG(MSGID_RtnNoticePattern,sizeof(NoticePatternField),npNoticePattern );		
}			


///����֪ͨģ��Ӧ��			
void UserSpi::OnRspModNoticePattern(CShfeFtdcNoticePatternField *pNoticePattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspModNoticePattern");		
	CONVERTSTRUCTFROMCTP(NoticePattern);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspModNoticePattern,sizeof(NoticePatternField),npNoticePattern,npRspInfo,nRequestID,bIsLast);		
}			


///�仯����ر���Ŀǰֻ�������¼�			
void UserSpi::OnRtnVaryMarketData(CShfeFtdcVaryMarketDataField *pVaryMarketData) 			
{			
	LOG_INFO("OnRtnVaryMarketData");		
	CONVERTSTRUCTFROMCTP(VaryMarketData);	
	SAFE_ADD_MSG(MSGID_RtnVaryMarketData,sizeof(VaryMarketDataField),npVaryMarketData );		
}			


///��������֪ͨӦ��			
void UserSpi::OnRspAddRiskNotifyA(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspAddRiskNotifyA");		
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspAddRiskNotifyA,0,0,npRspInfo,nRequestID,bIsLast);		
}			


///����ҵ��֪ͨӦ��			
void UserSpi::OnRspAddBizNotice(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspAddBizNotice");		
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspAddBizNotice,0,0,npRspInfo,nRequestID,bIsLast);		
}			


///�����ҵ��֪ͨ�ر�			
void UserSpi::OnRtnSeqBizNotice(CShfeFtdcSeqBizNoticeField *pSeqBizNotice) 			
{			
	LOG_INFO("OnRtnSeqBizNotice");		
	CONVERTSTRUCTFROMCTP(SeqBizNotice);	
	SAFE_ADD_MSG(MSGID_RtnSeqBizNotice,sizeof(SeqBizNoticeField),npSeqBizNotice );		
}			


///��ѯϯλ�ʽ�Ӧ��			
void UserSpi::OnRspRiskQryBrokerDeposit(CShfeFtdcQueryBrokerDepositField *pQueryBrokerDeposit, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRiskQryBrokerDeposit");		
	CONVERTSTRUCTFROMCTP(QueryBrokerDeposit);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRiskQryBrokerDeposit,sizeof(QueryBrokerDepositField),npQueryBrokerDeposit,npRspInfo,nRequestID,bIsLast);		
}			


///��ز�����Ϣ�ر�			
void UserSpi::OnRtnRiskParamInfo(CShfeFtdcRiskParamInfoField *pRiskParamInfo) 			
{			
	LOG_INFO("OnRtnRiskParamInfo");		
	CONVERTSTRUCTFROMCTP(RiskParamInfo);	
	SAFE_ADD_MSG(MSGID_RtnRiskParamInfo,sizeof(RiskParamInfoField),npRiskParamInfo );		
}			


///�޸�Ͷ���߲���Ӧ��			
void UserSpi::OnRspModRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspModRiskInvestorParam");		
	CONVERTSTRUCTFROMCTP(RiskInvestorParam);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspModRiskInvestorParam,sizeof(RiskInvestorParamField),npRiskInvestorParam,npRspInfo,nRequestID,bIsLast);		
}			


///ɾ��Ͷ���߲���Ӧ��			
void UserSpi::OnRspRemRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRemRiskInvestorParam");		
	CONVERTSTRUCTFROMCTP(RiskInvestorParam);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRemRiskInvestorParam,sizeof(RiskInvestorParamField),npRiskInvestorParam,npRspInfo,nRequestID,bIsLast);		
}			


///Ͷ���߲����ر�			
void UserSpi::OnRtnRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam) 			
{			
	LOG_INFO("OnRtnRiskInvestorParam");		
	CONVERTSTRUCTFROMCTP(RiskInvestorParam);	
	SAFE_ADD_MSG(MSGID_RtnRiskInvestorParam,sizeof(RiskInvestorParamField),npRiskInvestorParam );		
}			


///ɾ��Ͷ���߲����ر�			
void UserSpi::OnRtnDelRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam) 			
{			
	LOG_INFO("OnRtnDelRiskInvestorParam");		
	CONVERTSTRUCTFROMCTP(RiskInvestorParam);	
	SAFE_ADD_MSG(MSGID_RtnDelRiskInvestorParam,sizeof(RiskInvestorParamField),npRiskInvestorParam );		
}			


///ǿ�Ʒ���û��ǳ�Ӧ��			
void UserSpi::OnRspForceRiskUserLogout(CShfeFtdcRiskLoginInfoField *pRiskLoginInfo, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspForceRiskUserLogout");		
	CONVERTSTRUCTFROMCTP(RiskLoginInfo);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspForceRiskUserLogout,sizeof(RiskLoginInfoField),npRiskLoginInfo,npRspInfo,nRequestID,bIsLast);		
}			


///ǿ�Ʒ���û��ǳ��ر�			
void UserSpi::OnRtnForceRiskUserLogout(CShfeFtdcRiskLoginInfoField *pRiskLoginInfo) 			
{			
	LOG_INFO("OnRtnForceRiskUserLogout");		
	CONVERTSTRUCTFROMCTP(RiskLoginInfo);	
	SAFE_ADD_MSG(MSGID_RtnForceRiskUserLogout,sizeof(RiskLoginInfoField),npRiskLoginInfo );		
}			


///���ӷ��֪ͨģ��Ӧ��			
void UserSpi::OnRspAddRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspAddRiskPattern");		
	CONVERTSTRUCTFROMCTP(RiskPattern);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspAddRiskPattern,sizeof(RiskPatternField),npRiskPattern,npRspInfo,nRequestID,bIsLast);		
}			


///�޸ķ��֪ͨģ��Ӧ��			
void UserSpi::OnRspModRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspModRiskPattern");		
	CONVERTSTRUCTFROMCTP(RiskPattern);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspModRiskPattern,sizeof(RiskPatternField),npRiskPattern,npRspInfo,nRequestID,bIsLast);		
}			


///ɾ�����֪ͨģ��Ӧ��			
void UserSpi::OnRspRemRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRemRiskPattern");		
	CONVERTSTRUCTFROMCTP(RiskPattern);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRemRiskPattern,sizeof(RiskPatternField),npRiskPattern,npRspInfo,nRequestID,bIsLast);		
}			


///���֪ͨģ��ر�			
void UserSpi::OnRtnRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern) 			
{			
	LOG_INFO("OnRtnRiskPattern");		
	CONVERTSTRUCTFROMCTP(RiskPattern);	
	SAFE_ADD_MSG(MSGID_RtnRiskPattern,sizeof(RiskPatternField),npRiskPattern );		
}			


///ɾ�����֪ͨģ��ر�			
void UserSpi::OnRtnDelRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern) 			
{			
	LOG_INFO("OnRtnDelRiskPattern");		
	CONVERTSTRUCTFROMCTP(RiskPattern);	
	SAFE_ADD_MSG(MSGID_RtnDelRiskPattern,sizeof(RiskPatternField),npRiskPattern );		
}			


///����Ͷ����֪ͨģ��Ӧ��			
void UserSpi::OnRspAddInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspAddInvestorPattern");		
	CONVERTSTRUCTFROMCTP(InvestorPattern);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspAddInvestorPattern,sizeof(InvestorPatternField),npInvestorPattern,npRspInfo,nRequestID,bIsLast);		
}			


///�޸�Ͷ����֪ͨģ��Ӧ��			
void UserSpi::OnRspModInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspModInvestorPattern");		
	CONVERTSTRUCTFROMCTP(InvestorPattern);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspModInvestorPattern,sizeof(InvestorPatternField),npInvestorPattern,npRspInfo,nRequestID,bIsLast);		
}			


///ɾ��Ͷ����֪ͨģ��Ӧ��			
void UserSpi::OnRspRemInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspRemInvestorPattern");		
	CONVERTSTRUCTFROMCTP(InvestorPattern);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspRemInvestorPattern,sizeof(InvestorPatternField),npInvestorPattern,npRspInfo,nRequestID,bIsLast);		
}			


///Ͷ����֪ͨģ��ر�			
void UserSpi::OnRtnInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern) 			
{			
	LOG_INFO("OnRtnInvestorPattern");		
	CONVERTSTRUCTFROMCTP(InvestorPattern);	
	SAFE_ADD_MSG(MSGID_RtnInvestorPattern,sizeof(InvestorPatternField),npInvestorPattern );		
}			


///ɾ��Ͷ����֪ͨģ��ر�			
void UserSpi::OnRtnDelInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern) 			
{			
	LOG_INFO("OnRtnDelInvestorPattern");		
	CONVERTSTRUCTFROMCTP(InvestorPattern);	
	SAFE_ADD_MSG(MSGID_RtnDelInvestorPattern,sizeof(InvestorPatternField),npInvestorPattern );		
}			


///����֪ͨģ���Զ��滻�ֶλر�			
void UserSpi::OnRtnRiskNotifyToken(CShfeFtdcRiskNotifyTokenField *pRiskNotifyToken) 			
{			
	LOG_INFO("OnRtnRiskNotifyToken");		
	CONVERTSTRUCTFROMCTP(RiskNotifyToken);	
	SAFE_ADD_MSG(MSGID_RtnRiskNotifyToken,sizeof(RiskNotifyTokenField),npRiskNotifyToken );		
}			


///����ķ���֪ͨ�ر��汾B����Ӧ��������ReqSubSeqRiskNotifyB			
void UserSpi::OnRtnSeqRiskNotifyB(CShfeFtdcSeqRiskNotifyBField *pSeqRiskNotifyB) 			
{			
	LOG_INFO("OnRtnSeqRiskNotifyB");		
	CONVERTSTRUCTFROMCTP(SeqRiskNotifyB);	
	SAFE_ADD_MSG(MSGID_RtnSeqRiskNotifyB,sizeof(SeqRiskNotifyBField),npSeqRiskNotifyB );		
}			


///��ѯ�ֲ�����Ӧ��			
void UserSpi::OnRspQryPositionStat(CShfeFtdcPositionStatField *pPositionStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryPositionStat");		
	CONVERTSTRUCTFROMCTP(PositionStat);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryPositionStat,sizeof(PositionStatField),npPositionStat,npRspInfo,nRequestID,bIsLast);		
}			


///��ѯ�ɽ�����Ӧ��			
void UserSpi::OnRspQryTradeStat(CShfeFtdcTradeStatField *pTradeStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryTradeStat");		
	CONVERTSTRUCTFROMCTP(TradeStat);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryTradeStat,sizeof(TradeStatField),npTradeStat,npRspInfo,nRequestID,bIsLast);		
}			


///Ͷ���߼���ϵ����Ϣ��ϣֵ��ѯӦ��			
void UserSpi::OnRspQryInvestorLinkManHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryInvestorLinkManHash");		
	CONVERTSTRUCTFROMCTP(InvestorHash);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryInvestorLinkManHash,sizeof(InvestorHashField),npInvestorHash,npRspInfo,nRequestID,bIsLast);		
}			


///��ѯͶ������֯�ܹ�HashֵӦ��			
void UserSpi::OnRspQryInvestorDepartmentHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryInvestorDepartmentHash");		
	CONVERTSTRUCTFROMCTP(InvestorHash);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryInvestorDepartmentHash,sizeof(InvestorHashField),npInvestorHash,npRspInfo,nRequestID,bIsLast);		
}			


///ѹ������Ӧ��			
void UserSpi::OnRspQryStressTest(CShfeFtdcInvestorRiskAccountField *pInvestorRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryStressTest");		
	CONVERTSTRUCTFROMCTP(InvestorRiskAccount);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryStressTest,sizeof(InvestorRiskAccountField),npInvestorRiskAccount,npRspInfo,nRequestID,bIsLast);		
}			


///��ѯ�ͱ�Ͷ����hashֵӦ��			
void UserSpi::OnRspQryLowMarginInvestorHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryLowMarginInvestorHash");		
	CONVERTSTRUCTFROMCTP(InvestorHash);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryLowMarginInvestorHash,sizeof(InvestorHashField),npInvestorHash,npRspInfo,nRequestID,bIsLast);		
}			


///��ѯ�ͱ�Ͷ����Ӧ��			
void UserSpi::OnRspQryLowMarginInvestor(CShfeFtdcBrokerInvestorField *pBrokerInvestor, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 			
{			

	LOG_INFO("OnRspQryLowMarginInvestor");		
	CONVERTSTRUCTFROMCTP(BrokerInvestor);
	CONVERTSTRUCTFROMCTP(RspInfo);	
	SAFE_ADD_MSG(MSGID_RspQryLowMarginInvestor,sizeof(BrokerInvestorField),npBrokerInvestor,npRspInfo,nRequestID,bIsLast);		
}			


///�ͱ�Ͷ���߻ر�			
void UserSpi::OnRtnLowMarginInvestor(CShfeFtdcBrokerInvestorField *pBrokerInvestor) 			
{			
	LOG_INFO("OnRtnLowMarginInvestor");		
	CONVERTSTRUCTFROMCTP(BrokerInvestor);	
	SAFE_ADD_MSG(MSGID_RtnLowMarginInvestor,sizeof(BrokerInvestorField),npBrokerInvestor );		
}			


///ɾ���ͱ�Ͷ���߻ر�			
void UserSpi::OnRtnDelLowMarginInvestor(CShfeFtdcBrokerInvestorField *pBrokerInvestor) 			
{			
	LOG_INFO("BrokerInvestor");		
	CONVERTSTRUCTFROMCTP(BrokerInvestor);	
	SAFE_ADD_MSG(MSGID_RtnDelLowMarginInvestor,sizeof(BrokerInvestorField),npBrokerInvestor );		
}			
