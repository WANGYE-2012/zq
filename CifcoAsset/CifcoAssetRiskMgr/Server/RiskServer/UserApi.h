#pragma once
class CShfeFtdcRiskUserApi;
class UserSpi;
class MaxNo;
class UserApi
{
public:
	///����UserApi
	static UserApi *CreateUserApi(const char *pszFlowPath = "");
	///ɾ���ӿڶ�����
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
	const char *GetTradingDay();

	///ע��ǰ�û������ַ
	///@param pszFrontAddress��ǰ�û������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	void RegisterFront(char *pszFrontAddress);

	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	void RegisterSpi();

	///�����г����顣
	int ReqRiskUserLogin(ReqRiskUserLoginField *pReqRiskUserLogin);



	///��ѯͶ���߱�֤����
	int ReqQryInvestorMarginRate(QryInvestorMarginRateField *pQryInvestorMarginRate);

	///��ѯ������������
	int ReqQryOrderStat(QryStatField *pQryStat);

	///���ķ��ʵʱ����
	int ReqSubRiskMarketData(SubMarketDataField *pSubMarketData, int SubMarketDataCnt);

	///�˶����ʵʱ����
	int ReqUnSubRiskMarketData(SubMarketDataField *pSubMarketData);

	///��Լ�ֱֲ�����ѯ
	int ReqQryInstPositionRate(QryInstPositionRateField *pQryInstPositionRate, int QryInstPositionRateCnt);

	///��Ʒ�ֱֲ�����ѯ
	int ReqQryProductPositionRate(QryProductPositionRateField *pQryProductPositionRate);

	///��ѯ���ױ���Hashֵ����
	// InvestorIDBeg    ��ʼͶ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ���СͶ���ߴ���
	// InvestorIdEnd    ����Ͷ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ����Ͷ���ߴ���
	// ���InvestorIDBeg��InvestorIDEnd��Ϊ�գ�����������Զ��ֶ�����䷵�ع�ϣֵ������ֻ������������һ����ϣֵ
	// hashֵ���㷽����
	int ReqQryTradingCodeHash(InvestorIDRangeField *pInvestorIDRange);

	///��ѯ���ױ�������ʹ�÷�������ReqQryInvestorLinkMan
	int ReqQryTradingCode(InvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt);

	///��������ĳɽ����ɸ����ѽ��յ�RtnSequencialTrade������UniqSequenceNo��ʵ������
	int ReqSubscribeTrade(NotifySequenceField *pNotifySequence);

	///��������ĳɽ����ɸ����ѽ��յ�RtnSequencialOrder������UniqSequenceNo��ʵ������
	int ReqSubscribeOrder(NotifySequenceField *pNotifySequence);

	///��������ľ��͹�˾�û��¼����ɸ����ѽ��յ�RtnSequencialBrokerUserEvent������UniqSequenceNo��ʵ������
	int ReqSubBrokerUserEvent(NotifySequenceField *pNotifySequence);

	///��ر���¼������
	int ReqRiskOrderInsert(RiskForceCloseOrderField *pRiskForceCloseOrder);

	///��ر�����������
	int ReqRiskOrderAction(InputOrderActionField *pInputOrderAction);

	///��������ĳֲ֣��ɸ����ѽ��յ�RtnSequencialPosition������UniqSequenceNo��ʵ������
	int ReqSubscribePosition(NotifySequenceField *pNotifySequence);

	///�ͻ�����֪ͨ������������
	// v4.2֮ǰ RiskNotifyCommand.ReserveΪ�գ�ֻ�ܷ���ϵͳ֪ͨ
	// v4.2 RiskNotifyCommand.Reserve=NoDup��Dup�����Է���ϵͳ���ʼ�֪ͨ��NoDup-�������ظ����͸��ͻ���ͬ�������֪ͨ��Dup-�����ظ����͸��ͻ���ͬ�������֪ͨ
	// v5.2 ����IsNormal��IsWarn�ֶ�
	int ReqRiskNotifyCommand(RiskNotifyCommandField *pRiskNotifyCommand);

	///����ǿƽ��������, version:{6,v541},{7:v551}
	int ReqBatchForceCloseCalc(ForceCloseStandardField *pForceCloseStandard, ForceClosePositionRuleField *pForceClosePositionRule, int ForceClosePositionRuleCnt, 
		ForceCloseListField *pForceCloseList, int ForceCloseListCnt);

	///����Ͷ����ǿƽ��������, version:{6,v541},{7:v551}
	int ReqForceCloseCalc(ForceCloseStandardField *pForceCloseStandard, ForceClosePositionField *pForceClosePosition, int ForceClosePositionCnt, 
		ForceCloseListField *pForceCloseList);

	///���ֱֲ�֤��ָ������������󣻶�ӦӦ��RspSetIndexNPPPara����Ӧ�ر�RtnIndexNPP
	int ReqSetIndexNPPPara(IndexNPPField *pIndexNPP);

	///ɾ�����ֱֲ�֤��ָ��������󣻶�ӦӦ��RspRiskDelIndexNPPPara����Ӧ�ر�RtnDelIndexNPP
	int ReqRiskDelIndexNPPPara(IndexNPPField *pIndexNPP);

	///��ѯ��½����
	int ReqQryLogin(NormalRiskQueryField *pNormalRiskQuery);

	///��ѯ�۸񲨶���Ͷ���߷��յ�Ӱ��
	int ReqQryPriceVaryEffect(QryPriceVaryEffectField *pQryPriceVaryEffect);

	///Ȩ�淴������룬Ӧ��ӿ�RspQrySafePriceRange��RspSafePriceAccount
	int ReqCalSafePriceRange(QrySafePriceRangeField *pQrySafePriceRange, PriceVaryParamField *pPriceVaryParam, int PriceVaryParamCnt, 
		BrokerInvestorField *pBrokerInvestor, int BrokerInvestorCnt);

	///���Ԥ��¼������
	int ReqRiskParkedOrderInsert(RiskParkedOrderField *pRiskParkedOrder);

	///����ɾ��Ԥ��
	int ReqRemoveRiskParkedOrder(RemoveRiskParkedOrderField *pRemoveRiskParkedOrder);

	///��������ķ��Ԥ��
	int ReqSubRiskParkedOrder(NotifySequenceField *pNotifySequence);

	///����û������޸�����
	int ReqRiskUserPasswordUpd(UserPasswordUpdateField *pUserPasswordUpdate);

	///��������ĳ���𣬿ɸ����ѽ��յ�RtnSeqDeposit������UniqSequenceNo��ʵ������
	int ReqSubSeqDeposit(RiskNtfSequenceField *pRiskNtfSequence);

	///�������û��¼�����
	int ReqAddRiskUserEvent(RiskUserEventField *pRiskUserEvent);

	///����Ԥ������(InstrumentPrice��v5.1_2֮����Ч)
	int ReqPredictRisk(PredictRiskParamField *pPredictRiskParam, BrokerInvestorField *pBrokerInvestor, int BrokerInvestorCnt, 
		ProductLimitsField *pProductLimits, int ProductLimitsCnt, 
		InstrumentPriceField *pInstrumentPrice, int InstrumentPriceCnt);

	///��ѯͶ���ߺ���ϵ����Ϣ����
	int ReqQryInvestorLinkMan(InvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt);
	///��ѯ��֯�ܹ�Ͷ���߶�Ӧ��ϵ����ʹ�÷�������ReqQryInvestorLinkMan
	int ReqQryInvestorDepartment(InvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt);







	///��������������ʽ𣬿ɸ����ѽ��յ�RtnSeqPreRiskAccount������UniqSequenceNo��ʵ�������������ʽ�ʹ��RtnRiskSyncAccount
	int ReqSubPreRiskAccount(RiskNtfSequenceField *pRiskNtfSequence);





	///����֪ͨģ������
	int ReqModNoticePattern(NoticePatternField *pNoticePattern);

	///���ı仯��������Ŀǰֻ�������¼�
	// ����֮�����¼۱仯ʱ�ᷢ��RtnVaryMarketData�ر���һ��������⣺�ϴ����¼ۺ����¼۶�����PriceRange.Price1��Price2֮��ʱ��������RtnVaryMarketData�ر�
	int ReqSubVaryMarketData(PriceRangeField *pPriceRange, int PriceRangeCnt);

	///�˶��仯��������Ŀǰֻ�˶����¼�
	int ReqUnSubVaryMarketData(SubMarketDataField *pSubMarketData, int SubMarketDataCnt);

	///��������֪ͨ����
	int ReqAddRiskNotifyA(RiskNotifyAField *pRiskNotifyA);

	///����ҵ��֪ͨ����
	int ReqAddBizNotice(BizNoticeField *pBizNotice);

	///����������������
	int ReqSubSeqData(RiskNtfSequenceField *pRiskNtfSequence);

	///��ѯϯλ�ʽ�����
	int ReqRiskQryBrokerDeposit(QueryBrokerDepositField *pQueryBrokerDeposit);

	///�޸�Ͷ���߲�������
	int ReqModRiskInvestorParam(RiskInvestorParamField *pRiskInvestorParam);

	///ɾ��Ͷ���߲�������
	int ReqRemRiskInvestorParam(RiskInvestorParamField *pRiskInvestorParam);

	///ǿ�Ʒ���û��ǳ�����
	int ReqForceRiskUserLogout(RiskLoginInfoField *pRiskLoginInfo);

	///���ӷ��֪ͨģ������
	int ReqAddRiskPattern(RiskPatternField *pRiskPattern);

	///�޸ķ��֪ͨģ������
	int ReqModRiskPattern(RiskPatternField *pRiskPattern);

	///ɾ�����֪ͨģ������
	int ReqRemRiskPattern(RiskPatternField *pRiskPattern);

	///����Ͷ����֪ͨģ������
	int ReqAddInvestorPattern(InvestorPatternField *pInvestorPattern, int InvestorPatternCnt);

	///�޸�Ͷ����֪ͨģ������
	int ReqModInvestorPattern(InvestorPatternField *pInvestorPattern, int InvestorPatternCnt);

	///ɾ��Ͷ����֪ͨģ������
	int ReqRemInvestorPattern(InvestorPatternField *pInvestorPattern, int InvestorPatternCnt);

	///��������ķ���֪ͨ�汾B���ɸ����ѽ��յ�RtnSeqRiskNotifyB������UniqSequenceNo��ʵ������
	int ReqSubSeqRiskNotifyB(RiskNtfSequenceField *pRiskNtfSequence);

	///��ѯ�ֲ���������
	int ReqQryPositionStat(QryStatField *pQryStat);

	///��ѯ�ɽ���������
	int ReqQryTradeStat(QryStatField *pQryStat);

	///Ͷ���߼���ϵ����Ϣ��ϣֵ��ѯ����
	// InvestorIDBeg    ��ʼͶ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ���СͶ���ߴ���
	// InvestorIdEnd    ����Ͷ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ����Ͷ���ߴ���
	// ���InvestorIDBeg��InvestorIDEnd��Ϊ�գ�����������Զ��ֶ�����䷵�ع�ϣֵ������ֻ������������һ����ϣֵ
	int ReqQryInvestorLinkManHash(InvestorIDRangeField *pInvestorIDRange);

	///��ѯͶ������֯�ܹ�Hashֵ����
	// InvestorIDBeg    ��ʼͶ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ���СͶ���ߴ���
	// InvestorIdEnd    ����Ͷ���ߴ��룬Ϊ�ձ�ʾ�û�Ȩ���µ����Ͷ���ߴ���
	// ���InvestorIDBeg��InvestorIDEnd��Ϊ�գ�����������Զ��ֶ�����䷵�ع�ϣֵ������ֻ������������һ����ϣֵ
	int ReqQryInvestorDepartmentHash(InvestorIDRangeField *pInvestorIDRange);

	///ѹ����������
	int ReqQryStressTest(STPriceField *pSTPrice, int STPriceCnt, 
		STMarginRateField *pSTMarginRate, int STMarginRateCnt);

	///��ѯ�ͱ�Ͷ����hashֵ����
	int ReqQryLowMarginInvestorHash(InvestorIDRangeField *pInvestorIDRange);

	///��ѯ�ͱ�Ͷ��������
	int ReqQryLowMarginInvestor(InvestorIDRangeField *pInvestorIDRange, int InvestorIDRangeCnt);
protected:
	UserApi(const char *pszFlowPath);
	~UserApi();
	int GetNextID();
	MaxNo* m_pMaxNo;
private:
	CShfeFtdcRiskUserApi* m_Api;
	UserSpi* m_Spi;
	std::string m_strTradingDay;
	template<class T>
	class CSafePtr
	{
	public:
		CSafePtr(T* p):m_ptr(p){}
		~CSafePtr(){if(m_ptr) delete m_ptr;}
	private:
		T* m_ptr;
	};
	template<class T>
	class CSafeArrayPtr
	{
	public:
		CSafeArrayPtr(T* p):m_ptr(p){}
		~CSafeArrayPtr(){if(m_ptr) delete[] m_ptr;}
	private:
		T* m_ptr;
	};
};
