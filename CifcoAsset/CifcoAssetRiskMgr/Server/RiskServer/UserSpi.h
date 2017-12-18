#pragma once
#include "FtdcRiskUserApi.h"
class CWriteLog;
class CRiskMsgQueue;
class MaxNo
{
public:
	MaxNo()
	{
		memset(this,0,sizeof(*this));
	}
	int MaxTradeNo;
	int MaxOrderNo;
	int MaxPositionNo;
	int MaxDepositNo;
	int	MaxPreRiskAccountNo;
};
class UserSpi :
	public CShfeFtdcRiskUserSpi
{
public:
	UserSpi(MaxNo& maxno);
	~UserSpi(void);
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	void OnFrontConnected();

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	void OnHeartBeatWarning(int nTimeLapse);


	///����Ӧ��
	void OnRspError(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///ϯλ�ʽ�ر�
	void OnRtnBrokerDeposit(CShfeFtdcBrokerDepositField *pBrokerDeposit) ;

	///Ͷ������ϢժҪ
	void OnRtnInvestorSumInfo(CShfeFtdcInvestorSumInfoField *pInvestorSumInfo) ;

	///�ͻ��˽���������ͬ����ʼ
	void OnRtnClientSGDataSyncStart(CShfeFtdcSettlementSessionField *pSettlementSession) ;

	///�ͻ��˽���������ͬ������
	void OnRtnClientSGDataSyncEnd(CShfeFtdcSettlementSessionField *pSettlementSession) ;

	///��¼������Ӧ
	void OnRspRiskUserLogin(CShfeFtdcRspRiskUserLoginField *pRspRiskUserLogin, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///
	void OnRspQryInvestorMarginRate(CShfeFtdcInvestorMarginRateField *pInvestorMarginRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��Ʒ��Ϣ
	void OnRtnProduct(CShfeFtdcProductField *pProduct) ;

	///��Լ��Ϣ
	void OnRtnInstrument(CShfeFtdcInstrumentField *pInstrument) ;

	///��ѯ��������Ӧ��
	void OnRspQryOrderStat(CShfeFtdcOrderStatField *pOrderStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�������Ļ�����Ϣ
	void OnRtnExchange(CShfeFtdcExchangeField *pExchange) ;

	///Ͷ���ֲ߳�ͳ�Ʋ�ѯӦ��-ReqInvestorPosition, ftd version 6 deprecated
	void OnRspInvestorPositionStatic(CShfeFtdcInvestorPositionStaticField *pInvestorPositionStatic, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///Ͷ���߳ɽ�ͳ�Ʋ�ѯӦ��-ReqInvestorTrade, ftd version 6 deprecated
	void OnRspInvestorTradeStatic(CShfeFtdcInvestorTradeStaticField *pInvestorTradeStatic, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///����������֪ͨ
	void OnRtnRiskDepthMarketData(CShfeFtdcDepthMarketDataField *pDepthMarketData) ;

	///ʱ��ͬ��
	void OnRtnTimeSync(CShfeFtdcCurrentTimeField *pCurrentTime) ;

	///��Լ�ֱֲ���Ӧ��
	void OnRspInstPositionRate(CShfeFtdcRspInstPositionRateField *pRspInstPositionRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��Ʒ�ֱֲ���Ӧ��
	void OnRspProductPositionRate(CShfeFtdcRspProductPositionRateField *pRspProductPositionRate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///Ͷ������
	void OnRtnInvestorGroup(CShfeFtdcInvestorGroupField *pInvestorGroup) ;

	///��������֤���ʣ�ɾ������RtnDelRtnExchangeMarginRate
	void OnRtnExchangeMarginRate(CShfeFtdcExchangeMarginRateField *pExchangeMarginRate) ;

	///��������֤���ʵ�����ɾ������RtnDelRtnExchangeMarginRateAdjust
	void OnRtnExchangeMarginRateAdjust(CShfeFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust) ;

	///��ѯ���ױ���HashֵӦ��
	void OnRspQryTradingCodeHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��ѯ���ױ���Ӧ��
	void OnRspQryTradingCode(CShfeFtdcTradingCodeField *pTradingCode, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///���ױ���ر�
	void OnRtnTradingCode(CShfeFtdcTradingCodeField *pTradingCode) ;

	///ɾ�����ױ���ر�
	void OnRtnDelTradingCode(CShfeFtdcTradingCodeField *pTradingCode) ;

	///����ľ��͹�˾�û��¼��ر�����Ӧ��������ReqSubBrokerUserEvent
	void OnRtnSequencialBrokerUserEvent(CShfeFtdcSequencialBrokerUserEventField *pSequencialBrokerUserEvent) ;

	///����ĳɽ��ر�����Ӧ��������ReqSubscribeTrade
	void OnRtnSequencialTrade(CShfeFtdcSequencialTradeField *pSequencialTrade) ;

	///����ı����ر�����Ӧ��������ReqSubscribeOrder
	void OnRtnSequencialOrder(CShfeFtdcSequencialOrderField *pSequencialOrder) ;

	///��ر���¼��������Ӧ
	void OnRspRiskOrderInsert(CShfeFtdcInputOrderField *pInputOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��ر�������������Ӧ
	void OnRspRiskOrderAction(CShfeFtdcInputOrderActionField *pInputOrderAction, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///����ĳֲֻر�����Ӧ��������ReqSubscribePosition
	void OnRtnSequencialPosition(CShfeFtdcSequencialPositionField *pSequencialPosition) ;

	///�ͻ�����֪ͨ��������Ӧ��
	void OnRspRiskNotifyCommand(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///����ǿƽ��������Ӧ��
	void OnRspBatchForceCloseCalc(CShfeFtdcRspForceClosePositionField *pRspForceClosePosition, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///����Ͷ����ǿƽ��������Ӧ��
	void OnRspForceCloseCalc(CShfeFtdcRspForceClosePositionField *pRspForceClosePosition, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///���ֱֲ�֤��ָ���������Ӧ��
	void OnRspSetIndexNPPPara(CShfeFtdcIndexNPPField *pIndexNPP, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///���ֱֲ�֤��ָ��ر�����ɾ��ָ�꣬��ʹ��RtnDelIndexNPP
	void OnRtnIndexNPP(CShfeFtdcFullIndexNPPField *pFullIndexNPP) ;

	///ɾ�����ֱֲ�֤��ָ�����Ӧ��
	void OnRspRiskDelIndexNPPPara(CShfeFtdcIndexNPPField *pIndexNPP, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///ǿƽ���������ʽ��˻�Ӧ��
	void OnRspForceCloseAccount(CShfeFtdcInvestorRiskAccountField *pInvestorRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��ѯ��½����Ӧ��
	void OnRspQryLogin(CShfeFtdcNormalRiskQueryField *pNormalRiskQuery, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��ѯ��ȫ�۸񲨶���ΧӦ��
	void OnRspQrySafePriceRange(CShfeFtdcRspSafePriceRangeField *pRspSafePriceRange, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��ѯ��ȫ�۸񲨶���Χ�ʽ�Ӧ��
	void OnRspSafePriceAccount(CShfeFtdcInvestorRiskAccountField *pInvestorRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��ѯ�۸񲨶���Ͷ���߷��յ�Ӱ��Ӧ��
	void OnRspQryPriceVaryEffect(CShfeFtdcPriceVaryParamField *pPriceVaryParam, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��֯�ܹ�
	void OnRtnDepartment(CShfeFtdcDepartmentField *pDepartment) ;

	///IndexNPPӦ��
	void OnRspIndexNPP(CShfeFtdcFullIndexNPPField *pFullIndexNPP, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///����ϵͳ����
	void OnRtnTradeParam(CShfeFtdcTradeParamField *pTradeParam) ;

	///���Ԥ��¼��Ӧ��
	void OnRspRiskParkedOrderInsert(CShfeFtdcRiskParkedOrderField *pRiskParkedOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///ɾ��Ԥ����Ӧ
	void OnRspRemoveRiskParkedOrder(CShfeFtdcRemoveRiskParkedOrderField *pRemoveRiskParkedOrder, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///����ķ��Ԥ�񵥻ر�
	void OnRtnSeqRiskParkedOrder(CShfeFtdcSeqRiskParkedOrderField *pSeqRiskParkedOrder) ;

	///����û������޸�Ӧ��
	void OnRspRiskUserPasswordUpd(CShfeFtdcUserPasswordUpdateField *pUserPasswordUpdate, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///����ĳ����ر�����Ӧ��������ReqSubSeqDeposit
	void OnRtnSeqDeposit(CShfeFtdcSeqDepositField *pSeqDeposit) ;

	///�������û��¼�Ӧ��
	void OnRspAddRiskUserEvent(CShfeFtdcRiskUserEventField *pRiskUserEvent, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///����Ԥ���ʽ�Ӧ��
	void OnRspPredictRiskAccount(CShfeFtdcPredictRiskAccountField *pPredictRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///����Ԥ��ֲ�Ӧ��
	void OnRspPredictRiskPosition(CShfeFtdcPredictRiskPositionField *pPredictRiskPosition, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///Ͷ���߻�����Ϣ�ر���ֻͬ�����б仯���ݣ��������{ReqQryInvestorLinkMan,RspQryInvestorLinkMan}ʹ��
	void OnRtnRiskInvestor(CShfeFtdcRiskSyncInvestorField *pRiskSyncInvestor) ;

	///��ѯͶ���ߺ���ϵ����ϢӦ��
	void OnRspQryInvestorLinkMan(CShfeFtdcInvestorLinkManField *pInvestorLinkMan, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///ɾ����������֤����
	void OnRtnDelExchangeMarginRate(CShfeFtdcExchangeMarginRateField *pExchangeMarginRate) ;

	///ɾ����������֤���ʵ���
	void OnRtnDelExchangeMarginRateAdjust(CShfeFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust) ;

	///��ѯ��֯�ܹ�Ͷ���߶�Ӧ��ϵӦ��
	void OnRspQryInvestorDepartment(CShfeFtdcInvestorDepartmentFlatField *pInvestorDepartmentFlat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///ɾ�����ֱֲ�֤��ָ��ر�����RtnIndexNPP��Ӧ
	void OnRtnDelIndexNPP(CShfeFtdcFullIndexNPPField *pFullIndexNPP) ;

	///����û�Ȩ�޻ر�
	void OnRtnRiskUserFunction(CShfeFtdcRiskUserFunctionField *pRiskUserFunction) ;

	///ɾ������û�Ȩ�޻ر�
	void OnRtnDelRiskUserFunction(CShfeFtdcRiskUserFunctionField *pRiskUserFunction) ;

	///Ͷ���ߵ����ʽ�ر��������ʽ�ʹ��{ReqSubPreRiskAccount,RtnSeqPreRiskAccount}
	void OnRtnRiskSyncAccount(CShfeFtdcRiskSyncAccountField *pRiskSyncAccount) ;

	///����������ʽ�ر�����Ӧ��������ReqSubPreRiskAccount
	void OnRtnSeqPreRiskAccount(CShfeFtdcSeqPreRiskAccountField *pSeqPreRiskAccount) ;

	///ģ���滻�ֶλر�
	void OnRtnNoticeToken(CShfeFtdcNoticeTokenField *pNoticeToken) ;

	///֪ͨģ��ر�
	void OnRtnNoticePattern(CShfeFtdcNoticePatternField *pNoticePattern) ;

	///����֪ͨģ��Ӧ��
	void OnRspModNoticePattern(CShfeFtdcNoticePatternField *pNoticePattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�仯����ر���Ŀǰֻ�������¼�
	void OnRtnVaryMarketData(CShfeFtdcVaryMarketDataField *pVaryMarketData) ;

	///��������֪ͨӦ��
	void OnRspAddRiskNotifyA(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///����ҵ��֪ͨӦ��
	void OnRspAddBizNotice(CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�����ҵ��֪ͨ�ر�
	void OnRtnSeqBizNotice(CShfeFtdcSeqBizNoticeField *pSeqBizNotice) ;

	///��ѯϯλ�ʽ�Ӧ��
	void OnRspRiskQryBrokerDeposit(CShfeFtdcQueryBrokerDepositField *pQueryBrokerDeposit, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��ز�����Ϣ�ر�
	void OnRtnRiskParamInfo(CShfeFtdcRiskParamInfoField *pRiskParamInfo) ;

	///�޸�Ͷ���߲���Ӧ��
	void OnRspModRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///ɾ��Ͷ���߲���Ӧ��
	void OnRspRemRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///Ͷ���߲����ر�
	void OnRtnRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam) ;

	///ɾ��Ͷ���߲����ر�
	void OnRtnDelRiskInvestorParam(CShfeFtdcRiskInvestorParamField *pRiskInvestorParam) ;

	///ǿ�Ʒ���û��ǳ�Ӧ��
	void OnRspForceRiskUserLogout(CShfeFtdcRiskLoginInfoField *pRiskLoginInfo, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///ǿ�Ʒ���û��ǳ��ر�
	void OnRtnForceRiskUserLogout(CShfeFtdcRiskLoginInfoField *pRiskLoginInfo) ;

	///���ӷ��֪ͨģ��Ӧ��
	void OnRspAddRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�޸ķ��֪ͨģ��Ӧ��
	void OnRspModRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///ɾ�����֪ͨģ��Ӧ��
	void OnRspRemRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///���֪ͨģ��ر�
	void OnRtnRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern) ;

	///ɾ�����֪ͨģ��ر�
	void OnRtnDelRiskPattern(CShfeFtdcRiskPatternField *pRiskPattern) ;

	///����Ͷ����֪ͨģ��Ӧ��
	void OnRspAddInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�޸�Ͷ����֪ͨģ��Ӧ��
	void OnRspModInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///ɾ��Ͷ����֪ͨģ��Ӧ��
	void OnRspRemInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///Ͷ����֪ͨģ��ر�
	void OnRtnInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern) ;

	///ɾ��Ͷ����֪ͨģ��ر�
	void OnRtnDelInvestorPattern(CShfeFtdcInvestorPatternField *pInvestorPattern) ;

	///����֪ͨģ���Զ��滻�ֶλر�
	void OnRtnRiskNotifyToken(CShfeFtdcRiskNotifyTokenField *pRiskNotifyToken) ;

	///����ķ���֪ͨ�ر��汾B����Ӧ��������ReqSubSeqRiskNotifyB
	void OnRtnSeqRiskNotifyB(CShfeFtdcSeqRiskNotifyBField *pSeqRiskNotifyB) ;

	///��ѯ�ֲ�����Ӧ��
	void OnRspQryPositionStat(CShfeFtdcPositionStatField *pPositionStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��ѯ�ɽ�����Ӧ��
	void OnRspQryTradeStat(CShfeFtdcTradeStatField *pTradeStat, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///Ͷ���߼���ϵ����Ϣ��ϣֵ��ѯӦ��
	void OnRspQryInvestorLinkManHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��ѯͶ������֯�ܹ�HashֵӦ��
	void OnRspQryInvestorDepartmentHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///ѹ������Ӧ��
	void OnRspQryStressTest(CShfeFtdcInvestorRiskAccountField *pInvestorRiskAccount, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��ѯ�ͱ�Ͷ����hashֵӦ��
	void OnRspQryLowMarginInvestorHash(CShfeFtdcInvestorHashField *pInvestorHash, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��ѯ�ͱ�Ͷ����Ӧ��
	void OnRspQryLowMarginInvestor(CShfeFtdcBrokerInvestorField *pBrokerInvestor, CShfeFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�ͱ�Ͷ���߻ر�
	void OnRtnLowMarginInvestor(CShfeFtdcBrokerInvestorField *pBrokerInvestor) ;

	///ɾ���ͱ�Ͷ���߻ر�
	void OnRtnDelLowMarginInvestor(CShfeFtdcBrokerInvestorField *pBrokerInvestor) ;
protected:
	CWriteLog* m_pWriteLog;
	MaxNo& m_MaxNo;
};
