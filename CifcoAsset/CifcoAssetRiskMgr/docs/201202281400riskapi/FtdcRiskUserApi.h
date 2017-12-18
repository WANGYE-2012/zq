/////////////////////////////////////////////////////////////////////////
///@system ��һ��������ϵͳ���ռ��ϵͳ
///@company �Ϻ��ڻ���Ϣ�������޹�˾
///@file FtdcRiskUserApi.h
///@brief �����˿ͻ��˽ӿ�
///@history 
///@version v{main:7,max:7,server:v5.5.3}
///20070227	zhangjie		�������ļ�
/////////////////////////////////////////////////////////////////////////

#if !defined(_FTDCRISKUSERAPI_H)
#define _FTDCRISKUSERAPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FtdcRiskUserApiStruct.h"

#if defined(ISLIB) && defined(WIN32)
#ifdef LIB_RISKUSER_API_EXPORT
#define RISKUSER_API_EXPORT __declspec(dllexport)
#else
#define RISKUSER_API_EXPORT __declspec(dllimport)
#pragma comment(lib, "riskuserapi.lib")
#endif
#else
#define RISKUSER_API_EXPORT 
#endif

class CShfeFtdcRiskUserSpi
{
public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected(){};
	
	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason){};
		
	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse){};
	

	///����Ӧ��
	virtual void OnRspError(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ϯλ�ʽ�ر�
	virtual void OnRtnBrokerDeposit(CShfeFtdcBrokerDepositField *pBrokerDeposit) {};

	///Ͷ������ϢժҪ
	virtual void OnRtnInvestorSumInfo(CShfeFtdcInvestorSumInfoField *pInvestorSumInfo) {};

	///�ͻ��˽���������ͬ����ʼ
	virtual void OnRtnClientSGDataSyncStart(CShfeFtdcSettlementSessionField *pSettlementSession) {};

	///�ͻ��˽���������ͬ������
	virtual void OnRtnClientSGDataSyncEnd(CShfeFtdcSettlementSessionField *pSettlementSession) {};

	///��¼������Ӧ
	virtual void OnRspRiskUserLogin(CShfeFtdcRspRiskUserLoginField *pRspRiskUserLogin, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///
	virtual void OnRspQryInvestorMarginRate(CShfeFtdcInvestorMarginRateField *pInvestorMarginRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��Ʒ��Ϣ
	virtual void OnRtnProduct(CShfeFtdcProductField *pProduct) {};

	///��Լ��Ϣ
	virtual void OnRtnInstrument(CShfeFtdcInstrumentField *pInstrument) {};

	///��ѯ��������Ӧ��
	virtual void OnRspQryOrderStat(CShfeFtdcOrderStatField *pOrderStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������Ļ�����Ϣ
	virtual void OnRtnExchange(CShfeFtdcExchangeField *pExchange) {};

	///Ͷ���ֲ߳�ͳ�Ʋ�ѯӦ��-ReqInvestorPosition, ftd version 6 deprecated
	virtual void OnRspInvestorPositionStatic(CShfeFtdcInvestorPositionStaticField *pInvestorPositionStatic, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ͷ���߳ɽ�ͳ�Ʋ�ѯӦ��-ReqInvestorTrade, ftd version 6 deprecated
	virtual void OnRspInvestorTradeStatic(CShfeFtdcInvestorTradeStaticField *pInvestorTradeStatic, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����������֪ͨ
	virtual void OnRtnRiskDepthMarketData(CShfeFtdcDepthMarketDataField *pDepthMarketData) {};

	///ʱ��ͬ��
	virtual void OnRtnTimeSync(CShfeFtdcCurrentTimeField *pCurrentTime) {};

	///��Լ�ֱֲ���Ӧ��
	virtual void OnRspInstPositionRate(CShfeFtdcRspInstPositionRateField *pRspInstPositionRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��Ʒ�ֱֲ���Ӧ��
	virtual void OnRspProductPositionRate(CShfeFtdcRspProductPositionRateField *pRspProductPositionRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ͷ������
	virtual void OnRtnInvestorGroup(CShfeFtdcInvestorGroupField *pInvestorGroup) {};

	///��������֤���ʣ�ɾ������RtnDelRtnExchangeMarginRate
	virtual void OnRtnExchangeMarginRate(CShfeFtdcExchangeMarginRateField *pExchangeMarginRate) {};

	///��������֤���ʵ�����ɾ������RtnDelRtnExchangeMarginRateAdjust
	virtual void OnRtnExchangeMarginRateAdjust(CShfeFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust) {};

	///��ѯ���ױ���HashֵӦ��
	virtual void OnRspQryTradingCodeHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��ѯ���ױ���Ӧ��
	virtual void OnRspQryTradingCode(CShfeFtdcTradingCodeField *pTradingCode, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���ױ���ر�
	virtual void OnRtnTradingCode(CShfeFtdcTradingCodeField *pTradingCode) {};

	///ɾ�����ױ���ر�
	virtual void OnRtnDelTradingCode(CShfeFtdcTradingCodeField *pTradingCode) {};

	///����ľ��͹�˾�û��¼��ر�����Ӧ��������ReqSubBrokerUserEvent
	virtual void OnRtnSequencialBrokerUserEvent(CShfeFtdcSequencialBrokerUserEventField *pSequencialBrokerUserEvent) {};

	///����ĳɽ��ر�����Ӧ��������ReqSubscribeTrade
	virtual void OnRtnSequencialTrade(CShfeFtdcSequencialTradeField *pSequencialTrade) {};

	///����ı����ر�����Ӧ��������ReqSubscribeOrder
	virtual void OnRtnSequencialOrder(CShfeFtdcSequencialOrderField *pSequencialOrder) {};

	///��ر���¼��������Ӧ
	virtual void OnRspRiskOrderInsert(CShfeFtdcInputOrderField *pInputOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��ر�������������Ӧ
	virtual void OnRspRiskOrderAction(CShfeFtdcInputOrderActionField *pInputOrderAction, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ĳֲֻر�����Ӧ��������ReqSubscribePosition
	virtual void OnRtnSequencialPosition(CShfeFtdcSequencialPositionField *pSequencialPosition) {};

	///�ͻ�����֪ͨ��������Ӧ��
	virtual void OnRspRiskNotifyCommand(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ǿƽ��������Ӧ��
	virtual void OnRspBatchForceCloseCalc(CShfeFtdcRspForceClosePositionField *pRspForceClosePosition, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����Ͷ����ǿƽ��������Ӧ��
	virtual void OnRspForceCloseCalc(CShfeFtdcRspForceClosePositionField *pRspForceClosePosition, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���ֱֲ�֤��ָ���������Ӧ��
	virtual void OnRspSetIndexNPPPara(CShfeFtdcIndexNPPField *pIndexNPP, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���ֱֲ�֤��ָ��ر�����ɾ��ָ�꣬��ʹ��RtnDelIndexNPP
	virtual void OnRtnIndexNPP(CShfeFtdcFullIndexNPPField *pFullIndexNPP) {};

	///ɾ�����ֱֲ�֤��ָ�����Ӧ��
	virtual void OnRspRiskDelIndexNPPPara(CShfeFtdcIndexNPPField *pIndexNPP, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ǿƽ���������ʽ��˻�Ӧ��
	virtual void OnRspForceCloseAccount(CShfeFtdcInvestorRiskAccountField *pInvestorRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��ѯ��½����Ӧ��
	virtual void OnRspQryLogin(CShfeFtdcNormalRiskQueryField *pNormalRiskQuery, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��ѯ��ȫ�۸񲨶���ΧӦ��
	virtual void OnRspQrySafePriceRange(CShfeFtdcRspSafePriceRangeField *pRspSafePriceRange, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��ѯ��ȫ�۸񲨶���Χ�ʽ�Ӧ��
	virtual void OnRspSafePriceAccount(CShfeFtdcInvestorRiskAccountField *pInvestorRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��ѯ�۸񲨶���Ͷ���߷��յ�Ӱ��Ӧ��
	virtual void OnRspQryPriceVaryEffect(CShfeFtdcPriceVaryParamField *pPriceVaryParam, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��֯�ܹ�
	virtual void OnRtnDepartment(CShfeFtdcDepartmentField *pDepartment) {};

	///IndexNPPӦ��
	virtual void OnRspIndexNPP(CShfeFtdcFullIndexNPPField *pFullIndexNPP, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ϵͳ����
	virtual void OnRtnTradeParam(CShfeFtdcTradeParamField *pTradeParam) {};

	///���Ԥ��¼��Ӧ��
	virtual void OnRspRiskParkedOrderInsert(CShfeFtdcRiskParkedOrderField *pRiskParkedOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ɾ��Ԥ����Ӧ
	virtual void OnRspRemoveRiskParkedOrder(CShfeFtdcRemoveRiskParkedOrderField *pRemoveRiskParkedOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ķ��Ԥ�񵥻ر�
	virtual void OnRtnSeqRiskParkedOrder(CShfeFtdcSeqRiskParkedOrderField *pSeqRiskParkedOrder) {};

	///����û������޸�Ӧ��
	virtual void OnRspRiskUserPasswordUpd(CShfeFtdcUserPasswordUpdateField *pUserPasswordUpdate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ĳ����ر�����Ӧ��������ReqSubSeqDeposit
	virtual void OnRtnSeqDeposit(CShfeFtdcSeqDepositField *pSeqDeposit) {};

	///�������û��¼�Ӧ��
	virtual void OnRspAddRiskUserEvent(CShfeFtdcRiskUserEventField *pRiskUserEvent, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����Ԥ���ʽ�Ӧ��
	virtual void OnRspPredictRiskAccount(CShfeFtdcPredictRiskAccountField *pPredictRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����Ԥ��ֲ�Ӧ��
	virtual void OnRspPredictRiskPosition(CShfeFtdcPredictRiskPositionField *pPredictRiskPosition, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ͷ���߻�����Ϣ�ر���ֻͬ�����б仯���ݣ��������{ReqQryInvestorLinkMan,RspQryInvestorLinkMan}ʹ��
	virtual void OnRtnRiskInvestor(CShfeFtdcRiskSyncInvestorField *pRiskSyncInvestor) {};

	///��ѯͶ���ߺ���ϵ����ϢӦ��
	virtual void OnRspQryInvestorLinkMan(CShfeFtdcInvestorLinkManField *pInvestorLinkMan, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ɾ����������֤����
	virtual void OnRtnDelExchangeMarginRate(CShfeFtdcExchangeMarginRateField *pExchangeMarginRate) {};

	///ɾ����������֤���ʵ���
	virtual void OnRtnDelExchangeMarginRateAdjust(CShfeFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust) {};

	///��ѯ��֯�ܹ�Ͷ���߶�Ӧ��ϵӦ��
	virtual void OnRspQryInvestorDepartment(CShfeFtdcInvestorDepartmentFlatField *pInvestorDepartmentFlat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ɾ�����ֱֲ�֤��ָ��ر�����RtnIndexNPP��Ӧ
	virtual void OnRtnDelIndexNPP(CShfeFtdcFullIndexNPPField *pFullIndexNPP) {};

	///����û�Ȩ�޻ر�
	virtual void OnRtnRiskUserFunction(CShfeFtdcRiskUserFunctionField *pRiskUserFunction) {};

	///ɾ������û�Ȩ�޻ر�
	virtual void OnRtnDelRiskUserFunction(CShfeFtdcRiskUserFunctionField *pRiskUserFunction) {};

	///Ͷ���ߵ����ʽ�ر��������ʽ�ʹ��{ReqSubPreRiskAccount,RtnSeqPreRiskAccount}
	virtual void OnRtnRiskSyncAccount(CShfeFtdcRiskSyncAccountField *pRiskSyncAccount) {};

	///����������ʽ�ر�����Ӧ��������ReqSubPreRiskAccount
	virtual void OnRtnSeqPreRiskAccount(CShfeFtdcSeqPreRiskAccountField *pSeqPreRiskAccount) {};

	///ģ���滻�ֶλر�
	virtual void OnRtnNoticeToken(CShfeFtdcNoticeTokenField *pNoticeToken) {};

	///֪ͨģ��ر�
	virtual void OnRtnNoticePattern(CShfeFtdcNoticePatternField *pNoticePattern) {};

	///����֪ͨģ��Ӧ��
	virtual void OnRspModNoticePattern(CShfeFtdcNoticePatternField *pNoticePattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�仯����ر���Ŀǰֻ�������¼�
	virtual void OnRtnVaryMarketData(CShfeFtdcVaryMarketDataField *pVaryMarketData) {};

	///��������֪ͨӦ��
	virtual void OnRspAddRiskNotifyA(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����ҵ��֪ͨӦ��
	virtual void OnRspAddBizNotice(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�����ҵ��֪ͨ�ر�
	virtual void OnRtnSeqBizNotice(CShfeFtdcSeqBizNoticeField *pSeqBizNotice) {};

	///��ѯϯλ�ʽ�Ӧ��
	virtual void OnRspRiskQryBrokerDeposit(CShfeFtdcQueryBrokerDepositField *pQueryBrokerDeposit, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��ز�����Ϣ�ر�
	virtual void OnRtnRiskParamInfo(CShfeFtdcRiskParamInfoField *pRiskParamInfo) {};

	///�޸�Ͷ���߲���Ӧ��
	virtual void OnRspModRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ɾ��Ͷ���߲���Ӧ��
	virtual void OnRspRemRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ͷ���߲����ر�
	virtual void OnRtnRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam) {};

	///ɾ��Ͷ���߲����ر�
	virtual void OnRtnDelRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam) {};

	///ǿ�Ʒ���û��ǳ�Ӧ��
	virtual void OnRspForceRiskUserLogout(CShfeFtdcRiskLoginInfoField *pRiskLoginInfo, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ǿ�Ʒ���û��ǳ��ر�
	virtual void OnRtnForceRiskUserLogout(CShfeFtdcRiskLoginInfoField *pRiskLoginInfo) {};

	///���ӷ��֪ͨģ��Ӧ��
	virtual void OnRspAddRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�޸ķ��֪ͨģ��Ӧ��
	virtual void OnRspModRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ɾ�����֪ͨģ��Ӧ��
	virtual void OnRspRemRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���֪ͨģ��ر�
	virtual void OnRtnRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern) {};

	///ɾ�����֪ͨģ��ر�
	virtual void OnRtnDelRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern) {};

	///����Ͷ����֪ͨģ��Ӧ��
	virtual void OnRspAddInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�޸�Ͷ����֪ͨģ��Ӧ��
	virtual void OnRspModInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ɾ��Ͷ����֪ͨģ��Ӧ��
	virtual void OnRspRemInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ͷ����֪ͨģ��ر�
	virtual void OnRtnInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern) {};

	///ɾ��Ͷ����֪ͨģ��ر�
	virtual void OnRtnDelInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern) {};

	///����֪ͨģ���Զ��滻�ֶλر�
	virtual void OnRtnRiskNotifyToken(CShfeFtdcRiskNotifyTokenField *pRiskNotifyToken) {};

	///����ķ���֪ͨ�ر��汾B����Ӧ��������ReqSubSeqRiskNotifyB
	virtual void OnRtnSeqRiskNotifyB(CShfeFtdcSeqRiskNotifyBField *pSeqRiskNotifyB) {};

	///��ѯ�ֲ�����Ӧ��
	virtual void OnRspQryPositionStat(CShfeFtdcPositionStatField *pPositionStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��ѯ�ɽ�����Ӧ��
	virtual void OnRspQryTradeStat(CShfeFtdcTradeStatField *pTradeStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ͷ���߼���ϵ����Ϣ��ϣֵ��ѯӦ��
	virtual void OnRspQryInvestorLinkManHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��ѯͶ������֯�ܹ�HashֵӦ��
	virtual void OnRspQryInvestorDepartmentHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ѹ������Ӧ��
	virtual void OnRspQryStressTest(CShfeFtdcInvestorRiskAccountField *pInvestorRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��ѯ�ͱ�Ͷ����hashֵӦ��
	virtual void OnRspQryLowMarginInvestorHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��ѯ�ͱ�Ͷ����Ӧ��
	virtual void OnRspQryLowMarginInvestor(CShfeFtdcBrokerInvestorField *pBrokerInvestor, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�ͱ�Ͷ���߻ر�
	virtual void OnRtnLowMarginInvestor(CShfeFtdcBrokerInvestorField *pBrokerInvestor) {};

	///ɾ���ͱ�Ͷ���߻ر�
	virtual void OnRtnDelLowMarginInvestor(CShfeFtdcBrokerInvestorField *pBrokerInvestor) {};
};

class RISKUSER_API_EXPORT CShfeFtdcRiskUserApi
{
public:
	///����RiskUserApi
	///@param pszFlowPath ����������Ϣ�ļ���Ŀ¼��Ĭ��Ϊ��ǰĿ¼
	///@return ��������UserApi
	static CShfeFtdcRiskUserApi *CreateFtdcRiskUserApi(const char *pszFlowPath = "");
	
	///ɾ���ӿڶ�����
	///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
	virtual void Release() = 0;
	
	///��ʼ��
	///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
	virtual void Init() = 0;
	
	///�ȴ��ӿ��߳̽�������
	///@return �߳��˳�����
	virtual int Join() = 0;
	
	///��ȡ��ǰ������
	///@retrun ��ȡ���Ľ�����
	///@remark ֻ�е�¼�ɹ���,���ܵõ���ȷ�Ľ�����
	virtual const char *GetTradingDay() = 0;
	
	///ע��ǰ�û������ַ
	///@param pszFrontAddress��ǰ�û������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	virtual void RegisterFront(char *pszFrontAddress) = 0;
	
	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	virtual void RegisterSpi(CShfeFtdcRiskUserSpi *pSpi) = 0;
	
	///�����г����顣
	///@param nTopicID �г���������  
	///@param nResumeType �г������ش���ʽ  
	///        TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        TERT_RESUME:���ϴ��յ�������
	///        TERT_QUICK:�ȴ��͵�ǰ�������,�ٴ��͵�¼���г����������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ�˽���������ݡ�
	//virtual void SubscribeMarketDataTopic(int nTopicID, TE_RESUME_TYPE nResumeType) = 0;
	
	///��¼����
		// changelog
		// v4.3֮ǰ ReqRiskUserLogin.Version=0
		//	Ͷ������Ϣ�ӿ�: RtnInvestorSumInfo,ReqInvestorRangeSumInfo,RspInvestorRangeSumInfo,ReqInvestorRangeInfo,RspInvestorRangeInfo,RtnRiskInvestorInfo
		//	Ͷ������֯�ܹ��ӿ�: RtnInvestorDepartment
		//	�ʽ�ӿ�: RtnInvestorAccount
		//
		// v4.3	ReqRiskUserLogin.Version=1
		//	Ͷ������Ϣ�ӿ�: RtnInvestorLinkManHash,ReqQryInvestorLinkMan,RspQryInvestorLinkMan
		//	Ͷ������֯�ܹ��ӿ�: RtnInvestorDepartmentHash,ReqQryInvestorDepartment,RspQryInvestorDepartment
		//
		// v5.0 ReqRiskUserLogin.Version=2
		//	�ʽ�ӿ�: �����ʽ�(ReqSubPreRiskAccount,RtnSeqPreRiskAccount),�����ʽ�RtnRiskSyncAccount
		//
		// v5.4.0.1 ����ʹ���ֶ�ReqRiskUserLogin.Version�����κ�ֵ�Գ���û��Ӱ��
		//	��¼�ɹ����Զ�����RtnInvestorLinkManHash��InvestorLinkMan��MD5ֵͨ���ӿ�ReqInvestorLinkManHash��ѯ
	virtual int ReqRiskUserLogin(CShfeFtdcReqRiskUserLoginField *pReqRiskUserLogin, int nRequestID) = 0;



	///��ѯͶ���߱�֤����
	virtual int ReqQryInvestorMarginRate(CShfeFtdcQryInvestorMarginRateField *pQryInvestorMarginRate, int nRequestID) = 0;

	///��ѯ������������
	virtual int ReqQryOrderStat(CShfeFtdcQryStatField *pQryStat, int nRequestID) = 0;

	///���ķ��ʵʱ����
	virtual int ReqSubRiskMarketData(CShfeFtdcSubMarketDataField *pSubMarketData, int SubMarketDataCnt, 
		int nRequestID) = 0;

	///�˶����ʵʱ����
	virtual int ReqUnSubRiskMarketData(CShfeFtdcSubMarketDataField *pSubMarketData, int nRequestID) = 0;

	///��Լ�ֱֲ�����ѯ
	virtual int ReqQryInstPositionRate(CShfeFtdcQryInstPositionRateField *pQryInstPositionRate, int QryInstPositionRateCnt, 
		int nRequestID) = 0;

	///��Ʒ�ֱֲ�����ѯ
	virtual int ReqQryProductPositionRate(CShfeFtdcQryProductPositionRateField *pQryProductPositionRate, int nRequestID) = 0;

	///��ѯ���ױ���Hashֵ����
		// InvestorIDBeg    ��ʼͶ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ���СͶ���ߴ���
		// InvestorIdEnd    ����Ͷ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ����Ͷ���ߴ���
		// ���InvestorIDBeg��InvestorIDEnd��Ϊ�գ�����������Զ��ֶ�����䷵�ع�ϣֵ������ֻ������������һ����ϣֵ
		// hashֵ���㷽����
	virtual int ReqQryTradingCodeHash(CShfeFtdcInvestorIDRangeField *pInvestorIDRange, int nRequestID) = 0;

	///��ѯ���ױ�������ʹ�÷�������ReqQryInvestorLinkMan
	virtual int ReqQryTradingCode(CShfeFtdcInvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt, 
		int nRequestID) = 0;

	///��������ĳɽ����ɸ����ѽ��յ�RtnSequencialTrade������UniqSequenceNo��ʵ������
	virtual int ReqSubscribeTrade(CShfeFtdcNotifySequenceField *pNotifySequence, int nRequestID) = 0;

	///��������ĳɽ����ɸ����ѽ��յ�RtnSequencialOrder������UniqSequenceNo��ʵ������
	virtual int ReqSubscribeOrder(CShfeFtdcNotifySequenceField *pNotifySequence, int nRequestID) = 0;

	///��������ľ��͹�˾�û��¼����ɸ����ѽ��յ�RtnSequencialBrokerUserEvent������UniqSequenceNo��ʵ������
	virtual int ReqSubBrokerUserEvent(CShfeFtdcNotifySequenceField *pNotifySequence, int nRequestID) = 0;

	///��ر���¼������
	virtual int ReqRiskOrderInsert(CShfeFtdcRiskForceCloseOrderField *pRiskForceCloseOrder, int nRequestID) = 0;

	///��ر�����������
	virtual int ReqRiskOrderAction(CShfeFtdcInputOrderActionField *pInputOrderAction, int nRequestID) = 0;

	///��������ĳֲ֣��ɸ����ѽ��յ�RtnSequencialPosition������UniqSequenceNo��ʵ������
	virtual int ReqSubscribePosition(CShfeFtdcNotifySequenceField *pNotifySequence, int nRequestID) = 0;

	///�ͻ�����֪ͨ������������
		// v4.2֮ǰ RiskNotifyCommand.ReserveΪ�գ�ֻ�ܷ���ϵͳ֪ͨ
		// v4.2 RiskNotifyCommand.Reserve=NoDup��Dup�����Է���ϵͳ���ʼ�֪ͨ��NoDup-�������ظ����͸��ͻ���ͬ�������֪ͨ��Dup-�����ظ����͸��ͻ���ͬ�������֪ͨ
		// v5.2 ����IsNormal��IsWarn�ֶ�
	virtual int ReqRiskNotifyCommand(CShfeFtdcRiskNotifyCommandField *pRiskNotifyCommand, int nRequestID) = 0;

	///����ǿƽ��������, version:{6,v541},{7:v551}
	virtual int ReqBatchForceCloseCalc(CShfeFtdcForceCloseStandardField *pForceCloseStandard, CShfeFtdcForceClosePositionRuleField *pForceClosePositionRule, int ForceClosePositionRuleCnt, 
		CShfeFtdcForceCloseListField *pForceCloseList, int ForceCloseListCnt, 
		int nRequestID) = 0;

	///����Ͷ����ǿƽ��������, version:{6,v541},{7:v551}
	virtual int ReqForceCloseCalc(CShfeFtdcForceCloseStandardField *pForceCloseStandard, CShfeFtdcForceClosePositionField *pForceClosePosition, int ForceClosePositionCnt, 
		CShfeFtdcForceCloseListField *pForceCloseList, int nRequestID) = 0;

	///���ֱֲ�֤��ָ������������󣻶�ӦӦ��RspSetIndexNPPPara����Ӧ�ر�RtnIndexNPP
	virtual int ReqSetIndexNPPPara(CShfeFtdcIndexNPPField *pIndexNPP, int nRequestID) = 0;

	///ɾ�����ֱֲ�֤��ָ��������󣻶�ӦӦ��RspRiskDelIndexNPPPara����Ӧ�ر�RtnDelIndexNPP
	virtual int ReqRiskDelIndexNPPPara(CShfeFtdcIndexNPPField *pIndexNPP, int nRequestID) = 0;

	///��ѯ��½����
	virtual int ReqQryLogin(CShfeFtdcNormalRiskQueryField *pNormalRiskQuery, int nRequestID) = 0;

	///��ѯ�۸񲨶���Ͷ���߷��յ�Ӱ��
	virtual int ReqQryPriceVaryEffect(CShfeFtdcQryPriceVaryEffectField *pQryPriceVaryEffect, int nRequestID) = 0;

	///Ȩ�淴������룬Ӧ��ӿ�RspQrySafePriceRange��RspSafePriceAccount
	virtual int ReqCalSafePriceRange(CShfeFtdcQrySafePriceRangeField *pQrySafePriceRange, CShfeFtdcPriceVaryParamField *pPriceVaryParam, int PriceVaryParamCnt, 
		CShfeFtdcBrokerInvestorField *pBrokerInvestor, int BrokerInvestorCnt, 
		int nRequestID) = 0;

	///���Ԥ��¼������
	virtual int ReqRiskParkedOrderInsert(CShfeFtdcRiskParkedOrderField *pRiskParkedOrder, int nRequestID) = 0;

	///����ɾ��Ԥ��
	virtual int ReqRemoveRiskParkedOrder(CShfeFtdcRemoveRiskParkedOrderField *pRemoveRiskParkedOrder, int nRequestID) = 0;

	///��������ķ��Ԥ��
	virtual int ReqSubRiskParkedOrder(CShfeFtdcNotifySequenceField *pNotifySequence, int nRequestID) = 0;

	///����û������޸�����
	virtual int ReqRiskUserPasswordUpd(CShfeFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID) = 0;

	///��������ĳ���𣬿ɸ����ѽ��յ�RtnSeqDeposit������UniqSequenceNo��ʵ������
	virtual int ReqSubSeqDeposit(CShfeFtdcRiskNtfSequenceField *pRiskNtfSequence, int nRequestID) = 0;

	///�������û��¼�����
	virtual int ReqAddRiskUserEvent(CShfeFtdcRiskUserEventField *pRiskUserEvent, int nRequestID) = 0;

	///����Ԥ������(InstrumentPrice��v5.1_2֮����Ч)
	virtual int ReqPredictRisk(CShfeFtdcPredictRiskParamField *pPredictRiskParam, CShfeFtdcBrokerInvestorField *pBrokerInvestor, int BrokerInvestorCnt, 
		CShfeFtdcProductLimitsField *pProductLimits, int ProductLimitsCnt, 
		CShfeFtdcInstrumentPriceField *pInstrumentPrice, int InstrumentPriceCnt, 
		int nRequestID) = 0;

	///��ѯͶ���ߺ���ϵ����Ϣ����
		// Ͷ������Ϣ�������ܴ��뾡������һ�β�ѯ����Ͷ���ߵ�Ͷ������Ϣ��
		// ��ȷʵ��Ҫ����Ͷ������Ϣ���ݣ���ǿ�ҽ����ڱ��ر������ݣ�Ȼ��ÿ��ֻ��ѯ�б仯��Ͷ������Ϣ���������£�
		// ��¼�ɹ���������ReqInvestorLinkManHash(���������ע��)��RspInvestorLinkManHash�ὫͶ������Ϣ��MD5ֵ�ֶ��Ͷ��������[InvestorIDBeg,InvestorIDEnd]���أ�
		// ��ÿ�����䣬�뱾����ͬ�����Ͷ������Ϣ��MD5ֵ���бȽϣ������һ�£�������������Ͷ������Ϣ��
		// ���齫������Ҫ�����Ͷ�����������һ�������͡�
		// ��һ��Ͷ������Ϣfield���µ�md5��˳��
		// md5.update((const char *)field.Address, strlen((const char *)field.Address));
		// md5.update((const char *)field.BrokerID, strlen((const char *)field.BrokerID));
		// md5.update((const char *)field.IdentifiedCardNo, strlen((const char *)field.IdentifiedCardNo));
		// md5.update((const char *)field.InvestorGroupID, strlen((const char *)field.InvestorGroupID));
		// md5.update((const char *)field.InvestorID, strlen((const char *)field.InvestorID));
		// md5.update((const char *)field.InvestorName, strlen((const char *)field.InvestorName));
		// md5.update((const char *)field.Telephone, strlen((const char *)field.Telephone));
		// md5.update((const char *)field.OrderManIdentifiedCardNo, strlen((const char *)field.OrderManIdentifiedCardNo));
		// md5.update((const char *)field.OrderManPersonName, strlen((const char *)field.OrderManPersonName));
		// md5.update((const char *)field.OrderManTelephone, strlen((const char *)field.OrderManTelephone));
		// md5.update((const char *)field.OrderManAddress, strlen((const char *)field.OrderManAddress));
		// md5.update((const char *)field.OrderManZipCode, strlen((const char *)field.OrderManZipCode));
		// md5.update((const char *)field.FundManIdentifiedCardNo, strlen((const char *)field.FundManIdentifiedCardNo));
		// md5.update((const char *)field.FundManPersonName, strlen((const char *)field.FundManPersonName));
		// md5.update((const char *)field.FundManTelephone, strlen((const char *)field.FundManTelephone));
		// md5.update((const char *)field.FundManAddress, strlen((const char *)field.FundManAddress));
		// md5.update((const char *)field.FundManZipCode, strlen((const char *)field.FundManZipCode));
		// md5.update((const char *)field.OpenManIdentifiedCardNo, strlen((const char *)field.OpenManIdentifiedCardNo));
		// md5.update((const char *)field.OpenManPersonName, strlen((const char *)field.OpenManPersonName));
		// md5.update((const char *)field.OpenManTelephone, strlen((const char *)field.OpenManTelephone));
		// md5.update((const char *)field.OpenManAddress, strlen((const char *)field.OpenManAddress));
		// md5.update((const char *)field.OpenManZipCode, strlen((const char *)field.OpenManZipCode));
		// md5.update((const char *)field.SettlementManIdentifiedCardNo, strlen((const char *)field.SettlementManIdentifiedCardNo));
		// md5.update((const char *)field.SettlementManPersonName, strlen((const char *)field.SettlementManPersonName));
		// md5.update((const char *)field.SettlementManTelephone, strlen((const char *)field.SettlementManTelephone));
		// md5.update((const char *)field.SettlementManAddress, strlen((const char *)field.SettlementManAddress));
		// md5.update((const char *)field.SettlementManZipCode, strlen((const char *)field.SettlementManZipCode));
		// md5.update((const char *)field.OpenDate, strlen((const char *)field.OpenDate));
		// md5.update((const char *)field.Mobile, strlen((const char *)field.Mobile));
		// md5.update((const char *)field.EMail, strlen((const char *)field.EMail));
		// if (field.IsActive == 0) md5.update(�ַ�0);
		// else md5.update(�ַ�1);
		// md5.update((const char *)field.PhoneCountryCode, strlen((const char *)field.PhoneCountryCode));
		// md5.update((const char *)field.PhoneAreaCode, strlen((const char *)field.PhoneAreaCode));
		// md5.update((const char *)field.OpenPhoneCountryCode, strlen((const char *)field.OpenPhoneCountryCode));
		// md5.update((const char *)field.OpenPhoneAreaCode, strlen((const char *)field.OpenPhoneAreaCode));
		// md5.update((const char *)field.OrderPhoneCountryCode, strlen((const char *)field.OrderPhoneCountryCode));
		// md5.update((const char *)field.OrderPhoneAreaCode, strlen((const char *)field.OrderPhoneAreaCode));
		// md5.update((const char *)field.FundPhoneCountryCode, strlen((const char *)field.FundPhoneCountryCode));
		// md5.update((const char *)field.FundPhoneAreaCode, strlen((const char *)field.FundPhoneAreaCode));
		// md5.update((const char *)field.SettlePhoneCountryCode, strlen((const char *)field.SettlePhoneCountryCode));
		// md5.update((const char *)field.SettlePhoneAreaCode, strlen((const char *)field.SettlePhoneAreaCode));
	virtual int ReqQryInvestorLinkMan(CShfeFtdcInvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt, 
		int nRequestID) = 0;
	///��ѯ��֯�ܹ�Ͷ���߶�Ӧ��ϵ����ʹ�÷�������ReqQryInvestorLinkMan
	virtual int ReqQryInvestorDepartment(CShfeFtdcInvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt, 
		int nRequestID) = 0;







	///��������������ʽ𣬿ɸ����ѽ��յ�RtnSeqPreRiskAccount������UniqSequenceNo��ʵ�������������ʽ�ʹ��RtnRiskSyncAccount
	virtual int ReqSubPreRiskAccount(CShfeFtdcRiskNtfSequenceField *pRiskNtfSequence, int nRequestID) = 0;





	///����֪ͨģ������
	virtual int ReqModNoticePattern(CShfeFtdcNoticePatternField *pNoticePattern, int nRequestID) = 0;

	///���ı仯��������Ŀǰֻ�������¼�
		// ����֮�����¼۱仯ʱ�ᷢ��RtnVaryMarketData�ر���һ��������⣺�ϴ����¼ۺ����¼۶�����PriceRange.Price1��Price2֮��ʱ��������RtnVaryMarketData�ر�
	virtual int ReqSubVaryMarketData(CShfeFtdcPriceRangeField *pPriceRange, int PriceRangeCnt, 
		int nRequestID) = 0;

	///�˶��仯��������Ŀǰֻ�˶����¼�
	virtual int ReqUnSubVaryMarketData(CShfeFtdcSubMarketDataField *pSubMarketData, int SubMarketDataCnt, 
		int nRequestID) = 0;

	///��������֪ͨ����
	virtual int ReqAddRiskNotifyA(CShfeFtdcRiskNotifyAField *pRiskNotifyA, int nRequestID) = 0;

	///����ҵ��֪ͨ����
	virtual int ReqAddBizNotice(CShfeFtdcBizNoticeField *pBizNotice, int nRequestID) = 0;

	///����������������
	virtual int ReqSubSeqData(CShfeFtdcRiskNtfSequenceField *pRiskNtfSequence, int nRequestID) = 0;

	///��ѯϯλ�ʽ�����
	virtual int ReqRiskQryBrokerDeposit(CShfeFtdcQueryBrokerDepositField *pQueryBrokerDeposit, int nRequestID) = 0;

	///�޸�Ͷ���߲�������
	virtual int ReqModRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam, int nRequestID) = 0;

	///ɾ��Ͷ���߲�������
	virtual int ReqRemRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam, int nRequestID) = 0;

	///ǿ�Ʒ���û��ǳ�����
	virtual int ReqForceRiskUserLogout(CShfeFtdcRiskLoginInfoField *pRiskLoginInfo, int nRequestID) = 0;

	///���ӷ��֪ͨģ������
	virtual int ReqAddRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, int nRequestID) = 0;

	///�޸ķ��֪ͨģ������
	virtual int ReqModRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, int nRequestID) = 0;

	///ɾ�����֪ͨģ������
	virtual int ReqRemRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, int nRequestID) = 0;

	///����Ͷ����֪ͨģ������
	virtual int ReqAddInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, int InvestorPatternCnt, 
		int nRequestID) = 0;

	///�޸�Ͷ����֪ͨģ������
	virtual int ReqModInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, int InvestorPatternCnt, 
		int nRequestID) = 0;

	///ɾ��Ͷ����֪ͨģ������
	virtual int ReqRemInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, int InvestorPatternCnt, 
		int nRequestID) = 0;

	///��������ķ���֪ͨ�汾B���ɸ����ѽ��յ�RtnSeqRiskNotifyB������UniqSequenceNo��ʵ������
	virtual int ReqSubSeqRiskNotifyB(CShfeFtdcRiskNtfSequenceField *pRiskNtfSequence, int nRequestID) = 0;

	///��ѯ�ֲ���������
	virtual int ReqQryPositionStat(CShfeFtdcQryStatField *pQryStat, int nRequestID) = 0;

	///��ѯ�ɽ���������
	virtual int ReqQryTradeStat(CShfeFtdcQryStatField *pQryStat, int nRequestID) = 0;

	///Ͷ���߼���ϵ����Ϣ��ϣֵ��ѯ����
		// InvestorIDBeg    ��ʼͶ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ���СͶ���ߴ���
		// InvestorIdEnd    ����Ͷ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ����Ͷ���ߴ���
		// ���InvestorIDBeg��InvestorIDEnd��Ϊ�գ�����������Զ��ֶ�����䷵�ع�ϣֵ������ֻ������������һ����ϣֵ
	virtual int ReqQryInvestorLinkManHash(CShfeFtdcInvestorIDRangeField *pInvestorIDRange, int nRequestID) = 0;

	///��ѯͶ������֯�ܹ�Hashֵ����
		// InvestorIDBeg    ��ʼͶ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ���СͶ���ߴ���
		// InvestorIdEnd    ����Ͷ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ����Ͷ���ߴ���
		// ���InvestorIDBeg��InvestorIDEnd��Ϊ�գ�����������Զ��ֶ�����䷵�ع�ϣֵ������ֻ������������һ����ϣֵ
	virtual int ReqQryInvestorDepartmentHash(CShfeFtdcInvestorIDRangeField *pInvestorIDRange, int nRequestID) = 0;

	///ѹ����������
	virtual int ReqQryStressTest(CShfeFtdcSTPriceField *pSTPrice, int STPriceCnt, 
		CShfeFtdcSTMarginRateField *pSTMarginRate, int STMarginRateCnt, 
		int nRequestID) = 0;

	///��ѯ�ͱ�Ͷ����hashֵ����
	virtual int ReqQryLowMarginInvestorHash(CShfeFtdcInvestorIDRangeField *pInvestorIDRange, int nRequestID) = 0;

	///��ѯ�ͱ�Ͷ��������
	virtual int ReqQryLowMarginInvestor(CShfeFtdcInvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt, 
		int nRequestID) = 0;



protected:
	~CShfeFtdcRiskUserApi(){};
};

#endif
