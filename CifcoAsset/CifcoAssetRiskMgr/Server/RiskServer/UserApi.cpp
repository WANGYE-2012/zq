#include "StdAfx.h"
#include "FtdcRiskUserApi.h"
#include "UserApi.h"
#include "UserSpi.h"
#include "Convert2Ctp.h"
#define CONVERTSTRUCT2CTP(TYPE)\
	CShfeFtdc##TYPE##Field* np##TYPE=new CShfeFtdc##TYPE##Field();\
	CSafePtr<CShfeFtdc##TYPE##Field> atuo_p##TYPE(np##TYPE);\
	Convert2Ctp::Get##TYPE##Field(*p##TYPE,*np##TYPE);


#define CONVERTSTRUCT2CTP_ARRAY(TYPE,CNT)\
	CShfeFtdc##TYPE##Field* np##TYPE=new CShfeFtdc##TYPE##Field[CNT];\
	CSafeArrayPtr<CShfeFtdc##TYPE##Field> atuo_p##TYPE(np##TYPE);\
	for (int i=0;i<CNT;++i)\
	{\
	Convert2Ctp::Get##TYPE##Field(*(p##TYPE+i),*(np##TYPE+i));\
	}

UserApi::UserApi(const char *pszFlowPath)
:m_Api(NULL)
{
	m_pMaxNo=new MaxNo();
	m_Api=CShfeFtdcRiskUserApi::CreateFtdcRiskUserApi(pszFlowPath);
}
UserApi::~UserApi()
{
	SAFE_DELETE(m_pMaxNo);
}
UserApi* UserApi::CreateUserApi(const char *pszFlowPath)
{
	return new UserApi(pszFlowPath);
}
///ɾ���ӿڶ�����
void UserApi::Release()
{
	m_Api->RegisterSpi(NULL);
	m_Api->Release();
	SAFE_DELETE(m_Spi);
	delete this;
}
///��ʼ��
///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
void UserApi::Init()
{
	m_Api->Init();
}

///�ȴ��ӿ��߳̽�������
///@return �߳��˳�����
int UserApi::Join()
{
	return m_Api->Join();
}

///��ȡ��ǰ������
///@retrun ��ȡ���Ľ�����
///@remark ֻ�е�¼�ɹ���,���ܵõ���ȷ�Ľ�����
const char *UserApi::GetTradingDay()
{
	m_strTradingDay=m_Api->GetTradingDay();
	return m_strTradingDay.c_str();
}

///ע��ǰ�û������ַ
///@param pszFrontAddress��ǰ�û������ַ��
///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
void UserApi::RegisterFront(char *pszFrontAddress)
{
	m_Api->RegisterFront(pszFrontAddress);
}

///ע��ص��ӿ�
///@param pSpi �����Իص��ӿ����ʵ��
void UserApi::RegisterSpi()
{
	m_Spi=new UserSpi(*m_pMaxNo);
	m_Api->RegisterSpi(static_cast<CShfeFtdcRiskUserSpi*>(m_Spi));
}

///�����г����顣
int UserApi::ReqRiskUserLogin(ReqRiskUserLoginField *pReqRiskUserLogin)
{
	CONVERTSTRUCT2CTP(ReqRiskUserLogin);										
	return m_Api->ReqRiskUserLogin(npReqRiskUserLogin,GetNextID());
}
///��ѯͶ���߱�֤����
int UserApi::ReqQryInvestorMarginRate(QryInvestorMarginRateField *pQryInvestorMarginRate)
{
	CONVERTSTRUCT2CTP(QryInvestorMarginRate);										
	return  m_Api->ReqQryInvestorMarginRate(npQryInvestorMarginRate,GetNextID());
}

///��ѯ������������
int UserApi::ReqQryOrderStat(QryStatField *pQryStat)
{
	CONVERTSTRUCT2CTP(QryStat);										
	return  m_Api->ReqQryOrderStat(npQryStat,GetNextID());										
}

///���ķ��ʵʱ����
int UserApi::ReqSubRiskMarketData(SubMarketDataField *pSubMarketData, int SubMarketDataCnt)
{	
	CONVERTSTRUCT2CTP_ARRAY(SubMarketData,SubMarketDataCnt);										
	return m_Api->ReqSubRiskMarketData(npSubMarketData, SubMarketDataCnt,GetNextID());
}

///�˶����ʵʱ����
int UserApi::ReqUnSubRiskMarketData(SubMarketDataField *pSubMarketData)
{
	CONVERTSTRUCT2CTP(SubMarketData);										
	return m_Api->ReqUnSubRiskMarketData(npSubMarketData,GetNextID());
}

///��Լ�ֱֲ�����ѯ
int UserApi::ReqQryInstPositionRate(QryInstPositionRateField *pQryInstPositionRate, int QryInstPositionRateCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(QryInstPositionRate,QryInstPositionRateCnt);										
	return  m_Api->ReqQryInstPositionRate(npQryInstPositionRate, QryInstPositionRateCnt,GetNextID());
}

///��Ʒ�ֱֲ�����ѯ
int UserApi::ReqQryProductPositionRate(QryProductPositionRateField *pQryProductPositionRate)
{
	CONVERTSTRUCT2CTP(QryProductPositionRate);										
	return  m_Api->ReqQryProductPositionRate(npQryProductPositionRate,GetNextID());
}

///��ѯ���ױ���Hashֵ����
// InvestorIDBeg    ��ʼͶ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ���СͶ���ߴ���
// InvestorIdEnd    ����Ͷ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ����Ͷ���ߴ���
// ���InvestorIDBeg��InvestorIDEnd��Ϊ�գ�����������Զ��ֶ�����䷵�ع�ϣֵ������ֻ������������һ����ϣֵ
// hashֵ���㷽����
int UserApi::ReqQryTradingCodeHash(InvestorIDRangeField *pInvestorIDRange)
{
	CONVERTSTRUCT2CTP(InvestorIDRange);										
	return  m_Api->ReqQryTradingCodeHash(npInvestorIDRange,GetNextID());
}

///��ѯ���ױ�������ʹ�÷�������ReqQryInvestorLinkMan
int UserApi::ReqQryTradingCode(InvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(InvestorIDRange,InvestorIDRangeCnt);										
	return  m_Api->ReqQryTradingCode(npInvestorIDRange, InvestorIDRangeCnt,GetNextID());
}

///��������ĳɽ����ɸ����ѽ��յ�RtnSequencialTrade������UniqSequenceNo��ʵ������
int UserApi::ReqSubscribeTrade(NotifySequenceField *pNotifySequence)
{
	if(pNotifySequence&&m_pMaxNo)
		pNotifySequence->SequenceNo=m_pMaxNo->MaxTradeNo;
	CONVERTSTRUCT2CTP(NotifySequence);										
	return m_Api->ReqSubscribeTrade(npNotifySequence,GetNextID());
}

///��������ĳɽ����ɸ����ѽ��յ�RtnSequencialOrder������UniqSequenceNo��ʵ������
int UserApi::ReqSubscribeOrder(NotifySequenceField *pNotifySequence)
{
	if(pNotifySequence&&m_pMaxNo)
		pNotifySequence->SequenceNo=m_pMaxNo->MaxOrderNo;
	CONVERTSTRUCT2CTP(NotifySequence);										
	return m_Api->ReqSubscribeOrder(npNotifySequence,GetNextID());
}

///��������ľ��͹�˾�û��¼����ɸ����ѽ��յ�RtnSequencialBrokerUserEvent������UniqSequenceNo��ʵ������
int UserApi::ReqSubBrokerUserEvent(NotifySequenceField *pNotifySequence)
{
	CONVERTSTRUCT2CTP(NotifySequence);										
	return m_Api->ReqSubBrokerUserEvent(npNotifySequence,GetNextID());
}

///��ر���¼������
int UserApi::ReqRiskOrderInsert(RiskForceCloseOrderField *pRiskForceCloseOrder)
{
	CONVERTSTRUCT2CTP(RiskForceCloseOrder);										
	return m_Api->ReqRiskOrderInsert(npRiskForceCloseOrder,GetNextID());
}

///��ر�����������
int UserApi::ReqRiskOrderAction(InputOrderActionField *pInputOrderAction)
{
	CONVERTSTRUCT2CTP(InputOrderAction);										
	return m_Api->ReqRiskOrderAction(npInputOrderAction,GetNextID());
}

///��������ĳֲ֣��ɸ����ѽ��յ�RtnSequencialPosition������UniqSequenceNo��ʵ������
int UserApi::ReqSubscribePosition(NotifySequenceField *pNotifySequence)
{
	if(pNotifySequence&&m_pMaxNo)
		pNotifySequence->SequenceNo=m_pMaxNo->MaxPositionNo;
	CONVERTSTRUCT2CTP(NotifySequence);										
	return m_Api->ReqSubscribePosition(npNotifySequence,GetNextID());
}

///�ͻ�����֪ͨ������������
// v4.2֮ǰ RiskNotifyCommand.ReserveΪ�գ�ֻ�ܷ���ϵͳ֪ͨ
// v4.2 RiskNotifyCommand.Reserve=NoDup��Dup�����Է���ϵͳ���ʼ�֪ͨ��NoDup-�������ظ����͸��ͻ���ͬ�������֪ͨ��Dup-�����ظ����͸��ͻ���ͬ�������֪ͨ
// v5.2 ����IsNormal��IsWarn�ֶ�
int UserApi::ReqRiskNotifyCommand(RiskNotifyCommandField *pRiskNotifyCommand)
{
	CONVERTSTRUCT2CTP(RiskNotifyCommand);										
	return m_Api->ReqRiskNotifyCommand(npRiskNotifyCommand,GetNextID());
}

///����ǿƽ��������, version:{6,v541},{7:v551}
int UserApi::ReqBatchForceCloseCalc(ForceCloseStandardField *pForceCloseStandard, ForceClosePositionRuleField *pForceClosePositionRule, int ForceClosePositionRuleCnt, 
									ForceCloseListField *pForceCloseList, int ForceCloseListCnt)		
{			
	CONVERTSTRUCT2CTP(ForceCloseStandard);								
	CONVERTSTRUCT2CTP_ARRAY(ForceClosePositionRule,ForceClosePositionRuleCnt);	
	CONVERTSTRUCT2CTP_ARRAY(ForceCloseList,ForceCloseListCnt);	
	return m_Api->ReqBatchForceCloseCalc(npForceCloseStandard,npForceClosePositionRule,
		ForceClosePositionRuleCnt,npForceCloseList,ForceCloseListCnt,GetNextID());									
}

///����Ͷ����ǿƽ��������, version:{6,v541},{7:v551}
int UserApi::ReqForceCloseCalc(ForceCloseStandardField *pForceCloseStandard, ForceClosePositionField *pForceClosePosition, int ForceClosePositionCnt, 
							   ForceCloseListField *pForceCloseList)				
{
	CONVERTSTRUCT2CTP(ForceCloseStandard);	
	CONVERTSTRUCT2CTP_ARRAY(ForceClosePosition,ForceClosePositionCnt);	
	CONVERTSTRUCT2CTP(ForceCloseList);	
	return m_Api->ReqForceCloseCalc(npForceCloseStandard, npForceClosePosition,ForceClosePositionCnt, npForceCloseList,GetNextID()); 
}

///���ֱֲ�֤��ָ������������󣻶�ӦӦ��RspSetIndexNPPPara����Ӧ�ر�RtnIndexNPP
int UserApi::ReqSetIndexNPPPara(IndexNPPField *pIndexNPP)
{
	CONVERTSTRUCT2CTP(IndexNPP);										
	return m_Api->ReqSetIndexNPPPara(npIndexNPP,GetNextID());
}

///ɾ�����ֱֲ�֤��ָ��������󣻶�ӦӦ��RspRiskDelIndexNPPPara����Ӧ�ر�RtnDelIndexNPP
int UserApi::ReqRiskDelIndexNPPPara(IndexNPPField *pIndexNPP)
{
	CONVERTSTRUCT2CTP(IndexNPP);										
	return m_Api->ReqRiskDelIndexNPPPara(npIndexNPP,GetNextID());
}

///��ѯ��½����
int UserApi::ReqQryLogin(NormalRiskQueryField *pNormalRiskQuery)
{
	CONVERTSTRUCT2CTP(NormalRiskQuery);										
	return  m_Api->ReqQryLogin(npNormalRiskQuery,GetNextID());										
}

///��ѯ�۸񲨶���Ͷ���߷��յ�Ӱ��
int UserApi::ReqQryPriceVaryEffect(QryPriceVaryEffectField *pQryPriceVaryEffect)
{
	CONVERTSTRUCT2CTP(QryPriceVaryEffect);										
	return  m_Api->ReqQryPriceVaryEffect(npQryPriceVaryEffect,GetNextID());
}

///Ȩ�淴������룬Ӧ��ӿ�RspQrySafePriceRange��RspSafePriceAccount
int UserApi::ReqCalSafePriceRange(QrySafePriceRangeField *pQrySafePriceRange, PriceVaryParamField *pPriceVaryParam, int PriceVaryParamCnt, 
								  BrokerInvestorField *pBrokerInvestor, int BrokerInvestorCnt)			
{
	CONVERTSTRUCT2CTP(QrySafePriceRange);	
	CONVERTSTRUCT2CTP_ARRAY(PriceVaryParam,PriceVaryParamCnt);
	CONVERTSTRUCT2CTP_ARRAY(BrokerInvestor,BrokerInvestorCnt);
	return m_Api->ReqCalSafePriceRange(npQrySafePriceRange,npPriceVaryParam, PriceVaryParamCnt,npBrokerInvestor,BrokerInvestorCnt,GetNextID());
}

///���Ԥ��¼������
int UserApi::ReqRiskParkedOrderInsert(RiskParkedOrderField *pRiskParkedOrder)
{
	CONVERTSTRUCT2CTP(RiskParkedOrder);										
	return m_Api->ReqRiskParkedOrderInsert(npRiskParkedOrder,GetNextID());
}

///����ɾ��Ԥ��
int UserApi::ReqRemoveRiskParkedOrder(RemoveRiskParkedOrderField *pRemoveRiskParkedOrder)
{
	CONVERTSTRUCT2CTP(RemoveRiskParkedOrder);										
	return m_Api->ReqRemoveRiskParkedOrder(npRemoveRiskParkedOrder,GetNextID());
}

///��������ķ��Ԥ��
int UserApi::ReqSubRiskParkedOrder(NotifySequenceField *pNotifySequence)
{
	CONVERTSTRUCT2CTP(NotifySequence);										
	return m_Api->ReqSubRiskParkedOrder(npNotifySequence,GetNextID());
}

///����û������޸�����
int UserApi::ReqRiskUserPasswordUpd(UserPasswordUpdateField *pUserPasswordUpdate)
{
	CONVERTSTRUCT2CTP(UserPasswordUpdate);										
	return m_Api->ReqRiskUserPasswordUpd(npUserPasswordUpdate,GetNextID());
}

///��������ĳ���𣬿ɸ����ѽ��յ�RtnSeqDeposit������UniqSequenceNo��ʵ������
int UserApi::ReqSubSeqDeposit(RiskNtfSequenceField *pRiskNtfSequence)
{
	if(pRiskNtfSequence&&m_pMaxNo)
		pRiskNtfSequence->SequenceNo=m_pMaxNo->MaxDepositNo;
	CONVERTSTRUCT2CTP(RiskNtfSequence);										
	return m_Api->ReqSubSeqDeposit(npRiskNtfSequence,GetNextID());
}

///�������û��¼�����
int UserApi::ReqAddRiskUserEvent(RiskUserEventField *pRiskUserEvent)
{
	CONVERTSTRUCT2CTP(RiskUserEvent);										
	return m_Api->ReqAddRiskUserEvent(npRiskUserEvent,GetNextID());
}

///����Ԥ������(InstrumentPrice��v5.1_2֮����Ч)
int UserApi::ReqPredictRisk(PredictRiskParamField *pPredictRiskParam, 
							BrokerInvestorField *pBrokerInvestor, int BrokerInvestorCnt, 
							ProductLimitsField *pProductLimits, int ProductLimitsCnt, 				
							InstrumentPriceField *pInstrumentPrice, int InstrumentPriceCnt)				
{
	CONVERTSTRUCT2CTP(PredictRiskParam);	
	CONVERTSTRUCT2CTP_ARRAY(BrokerInvestor,BrokerInvestorCnt);
	CONVERTSTRUCT2CTP_ARRAY(ProductLimits,ProductLimitsCnt);
	CONVERTSTRUCT2CTP_ARRAY(InstrumentPrice,InstrumentPriceCnt);
	return m_Api->ReqPredictRisk(npPredictRiskParam, npBrokerInvestor, BrokerInvestorCnt,
		npProductLimits,ProductLimitsCnt,npInstrumentPrice,InstrumentPriceCnt,GetNextID());								
}

///��ѯͶ���ߺ���ϵ����Ϣ����
int UserApi::ReqQryInvestorLinkMan(InvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(InvestorIDRange,InvestorIDRangeCnt);										
	return  m_Api->ReqQryInvestorLinkMan(npInvestorIDRange, InvestorIDRangeCnt,GetNextID());
}
///��ѯ��֯�ܹ�Ͷ���߶�Ӧ��ϵ����ʹ�÷�������ReqQryInvestorLinkMan
int UserApi::ReqQryInvestorDepartment(InvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(InvestorIDRange,InvestorIDRangeCnt);										
	return  m_Api->ReqQryInvestorDepartment(npInvestorIDRange, InvestorIDRangeCnt,GetNextID());
}
///��������������ʽ𣬿ɸ����ѽ��յ�RtnSeqPreRiskAccount������UniqSequenceNo��ʵ�������������ʽ�ʹ��RtnRiskSyncAccount
int UserApi::ReqSubPreRiskAccount(RiskNtfSequenceField *pRiskNtfSequence)
{
	if(pRiskNtfSequence&&m_pMaxNo)
		pRiskNtfSequence->SequenceNo=m_pMaxNo->MaxPreRiskAccountNo;
	CONVERTSTRUCT2CTP(RiskNtfSequence);										
	return m_Api->ReqSubPreRiskAccount(npRiskNtfSequence,GetNextID());
}
///����֪ͨģ������
int UserApi::ReqModNoticePattern(NoticePatternField *pNoticePattern)
{
	CONVERTSTRUCT2CTP(NoticePattern);										
	return m_Api->ReqModNoticePattern(npNoticePattern,GetNextID());
}

///���ı仯��������Ŀǰֻ�������¼�
// ����֮�����¼۱仯ʱ�ᷢ��RtnVaryMarketData�ر���һ��������⣺�ϴ����¼ۺ����¼۶�����PriceRange.Price1��Price2֮��ʱ��������RtnVaryMarketData�ر�
int UserApi::ReqSubVaryMarketData(PriceRangeField *pPriceRange, int PriceRangeCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(PriceRange,PriceRangeCnt);										
	return m_Api->ReqSubVaryMarketData(npPriceRange, PriceRangeCnt,GetNextID());
}

///�˶��仯��������Ŀǰֻ�˶����¼�
int UserApi::ReqUnSubVaryMarketData(SubMarketDataField *pSubMarketData, int SubMarketDataCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(SubMarketData,SubMarketDataCnt);										
	return m_Api->ReqUnSubVaryMarketData(npSubMarketData, SubMarketDataCnt,GetNextID());
}

///��������֪ͨ����
int UserApi::ReqAddRiskNotifyA(RiskNotifyAField *pRiskNotifyA)
{
	CONVERTSTRUCT2CTP(RiskNotifyA);										
	return m_Api->ReqAddRiskNotifyA(npRiskNotifyA,GetNextID());
}

///����ҵ��֪ͨ����
int UserApi::ReqAddBizNotice(BizNoticeField *pBizNotice)
{
	CONVERTSTRUCT2CTP(BizNotice);										
	return m_Api->ReqAddBizNotice(npBizNotice,GetNextID());
}

///����������������
int UserApi::ReqSubSeqData(RiskNtfSequenceField *pRiskNtfSequence)
{
	CONVERTSTRUCT2CTP(RiskNtfSequence);										
	return m_Api->ReqSubSeqData(npRiskNtfSequence,GetNextID());
}

///��ѯϯλ�ʽ�����
int UserApi::ReqRiskQryBrokerDeposit(QueryBrokerDepositField *pQueryBrokerDeposit)
{
	CONVERTSTRUCT2CTP(QueryBrokerDeposit);										
	return m_Api->ReqRiskQryBrokerDeposit(npQueryBrokerDeposit,GetNextID());
}

///�޸�Ͷ���߲�������
int UserApi::ReqModRiskInvestorParam(RiskInvestorParamField *pRiskInvestorParam)
{
	CONVERTSTRUCT2CTP(RiskInvestorParam);										
	return m_Api->ReqModRiskInvestorParam(npRiskInvestorParam,GetNextID());
}

///ɾ��Ͷ���߲�������
int UserApi::ReqRemRiskInvestorParam(RiskInvestorParamField *pRiskInvestorParam)
{
	CONVERTSTRUCT2CTP(RiskInvestorParam);										
	return m_Api->ReqRemRiskInvestorParam(npRiskInvestorParam,GetNextID());
}

///ǿ�Ʒ���û��ǳ�����
int UserApi::ReqForceRiskUserLogout(RiskLoginInfoField *pRiskLoginInfo)
{
	CONVERTSTRUCT2CTP(RiskLoginInfo);										
	return m_Api->ReqForceRiskUserLogout(npRiskLoginInfo,GetNextID());
}

///���ӷ��֪ͨģ������
int UserApi::ReqAddRiskPattern(RiskPatternField *pRiskPattern)										
{
	CONVERTSTRUCT2CTP(RiskPattern);										
	return m_Api->ReqAddRiskPattern(npRiskPattern,GetNextID());
}

///�޸ķ��֪ͨģ������
int UserApi::ReqModRiskPattern(RiskPatternField *pRiskPattern)
{
	CONVERTSTRUCT2CTP(RiskPattern);										
	return m_Api->ReqModRiskPattern(npRiskPattern,GetNextID());
}

///ɾ�����֪ͨģ������
int UserApi::ReqRemRiskPattern(RiskPatternField *pRiskPattern)
{
	CONVERTSTRUCT2CTP(RiskPattern);										
	return m_Api->ReqRemRiskPattern(npRiskPattern,GetNextID());
}

///����Ͷ����֪ͨģ������
int UserApi::ReqAddInvestorPattern(InvestorPatternField *pInvestorPattern, int InvestorPatternCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(InvestorPattern,InvestorPatternCnt);										
	return m_Api->ReqAddInvestorPattern(npInvestorPattern, InvestorPatternCnt,GetNextID());
}

///�޸�Ͷ����֪ͨģ������
int UserApi::ReqModInvestorPattern(InvestorPatternField *pInvestorPattern, int InvestorPatternCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(InvestorPattern,InvestorPatternCnt);										
	return m_Api->ReqModInvestorPattern(npInvestorPattern, InvestorPatternCnt,GetNextID())	;										
}

///ɾ��Ͷ����֪ͨģ������
int UserApi::ReqRemInvestorPattern(InvestorPatternField *pInvestorPattern, int InvestorPatternCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(InvestorPattern,InvestorPatternCnt);										
	return m_Api->ReqRemInvestorPattern(npInvestorPattern, InvestorPatternCnt,GetNextID());
}

///��������ķ���֪ͨ�汾B���ɸ����ѽ��յ�RtnSeqRiskNotifyB������UniqSequenceNo��ʵ������
int UserApi::ReqSubSeqRiskNotifyB(RiskNtfSequenceField *pRiskNtfSequence)
{
	CONVERTSTRUCT2CTP(RiskNtfSequence);										
	return m_Api->ReqSubSeqRiskNotifyB(npRiskNtfSequence,GetNextID());
}

///��ѯ�ֲ���������
int UserApi::ReqQryPositionStat(QryStatField *pQryStat)
{
	CONVERTSTRUCT2CTP(QryStat);	
	return  m_Api->ReqQryPositionStat(npQryStat,GetNextID());
}

///��ѯ�ɽ���������
int UserApi::ReqQryTradeStat(QryStatField *pQryStat)
{
	CONVERTSTRUCT2CTP(QryStat);
	return  m_Api->ReqQryTradeStat(npQryStat,GetNextID());
}

///Ͷ���߼���ϵ����Ϣ��ϣֵ��ѯ����
// InvestorIDBeg    ��ʼͶ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ���СͶ���ߴ���
// InvestorIdEnd    ����Ͷ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ����Ͷ���ߴ���
// ���InvestorIDBeg��InvestorIDEnd��Ϊ�գ�����������Զ��ֶ�����䷵�ع�ϣֵ������ֻ������������һ����ϣֵ
int UserApi::ReqQryInvestorLinkManHash(InvestorIDRangeField *pInvestorIDRange)
{
	CONVERTSTRUCT2CTP(InvestorIDRange);
	return  m_Api->ReqQryInvestorLinkManHash(npInvestorIDRange,GetNextID());
}

///��ѯͶ������֯�ܹ�Hashֵ����
// InvestorIDBeg    ��ʼͶ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ���СͶ���ߴ���
// InvestorIdEnd    ����Ͷ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ����Ͷ���ߴ���
// ���InvestorIDBeg��InvestorIDEnd��Ϊ�գ�����������Զ��ֶ�����䷵�ع�ϣֵ������ֻ������������һ����ϣֵ
int UserApi::ReqQryInvestorDepartmentHash(InvestorIDRangeField *pInvestorIDRange)
{
	CONVERTSTRUCT2CTP(InvestorIDRange);
	return  m_Api->ReqQryInvestorDepartmentHash(npInvestorIDRange,GetNextID());
}

///ѹ����������
int UserApi::ReqQryStressTest(STPriceField *pSTPrice, int STPriceCnt, 
							  STMarginRateField *pSTMarginRate, int STMarginRateCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(STPrice,STPriceCnt);
	CONVERTSTRUCT2CTP_ARRAY(STMarginRate,STMarginRateCnt);
	return  m_Api->ReqQryStressTest(npSTPrice,STPriceCnt,npSTMarginRate,STMarginRateCnt,GetNextID());
}

///��ѯ�ͱ�Ͷ����hashֵ����
int UserApi::ReqQryLowMarginInvestorHash(InvestorIDRangeField *pInvestorIDRange)
{
	CONVERTSTRUCT2CTP(InvestorIDRange);
	return  m_Api->ReqQryLowMarginInvestorHash(npInvestorIDRange,GetNextID());
}

///��ѯ�ͱ�Ͷ��������
int UserApi::ReqQryLowMarginInvestor(InvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt)
{
	CONVERTSTRUCT2CTP_ARRAY(InvestorIDRange,InvestorIDRangeCnt);
	return  m_Api->ReqQryLowMarginInvestor(npInvestorIDRange,InvestorIDRangeCnt,GetNextID());
}

int UserApi::GetNextID()
{
	return 0;
}
