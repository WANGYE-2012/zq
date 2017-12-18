#pragma once
#include "DataBase.h"
class DBAccess;
class CWriteLog;
class CDataCenter:CDataBase
{
public:
	typedef std::map<std::string,std::map<TSequenceNoType,SequencialPositionField>> POSITIONSTRUCT;
	typedef std::map<std::string,std::map<TSequenceNoType,SequencialTradeField>> TRADESTRUCT;
	typedef std::map<std::string,std::map<TSequenceNoType,SequencialOrderField>> ORDERSTRUCT; 
	typedef std::map<RiskIndModuleType,std::map<RiskLevelType,RiskResponse>> RISKWARNINGSTRUCT; 
public:
	CDataCenter(void);
	virtual ~CDataCenter(void);
	static void Init(void);
	static void Release(void);
	static CDataCenter* Get();
	void MakeRiskWarningMap(RiskIndicatorType type,RiskIndModuleType module,
		int level,const RiskResponse& warning,RISKWARNINGSTRUCT* pMapVec);
	void MakeRiskWarningMap(RiskIndicatorType type,std::string strAcc,
		std::string ProductID,int level,const RiskResponse& warning,
		std::map<std::string,std::map<RiskLevelType,RiskResponse>>& tmap);
	void ReadRiskWarning();
	void ReadConfigXml(const std::string strfilename);
	void AddProduct(const ProductField& Product );
	void AddInstrument(const InstrumentField& Instrument);
	void AddMarketData(const DepthMarketDataField& MarketData);
	void AddRiskFundAccount(const RiskSyncAccountField& RiskSyncAccount);
	void AddPreRiskFundAccount(const SeqPreRiskAccountField& PreRiskAccount);
	void AddPosition(const SequencialPositionField& Position);
	void AddTrade(const SequencialTradeField& Trade);
	void AddOrder(const SequencialOrderField& Order);
	void AddSeqDeposit(const SeqDepositField& Deposit);
	void OnTimerStatistics();

	void AddAcoount2NetValue(NetValueInfo& netValueInfo);
public:
	void NewActiveEvent( const RiskEvent& evt );
	void DeleteActiveEvent( const RiskEvent& evt );
	void AddHandling(const RiskEventHandling& r);
	void AddHandling(const RiskEventHandlingUserReq& r);
	DBAccess& GetDBAccess(){return m_dbAccess; }
	//��ȡ�û���¼����ṹ��
	void GetUserLoginField(ReqRiskUserLoginField& LoginField) const;
	//��ȡǰ�˷�������ַ�б�
	void GetrFrontAddr(std::vector<std::string>& strFrontAddr ) const;
	//��ȡ���׷�������ַ
	void GetTradeAddr(std::string& strTradeAddr) const;
	bool GetForceClosePermission(int AccID) const;
	bool GetEventMessageTemplate(RiskIndicatorType nRiskIndicatorID,
		RiskLevelType nRiskLevelID,EventMessageTemplate& msgtemplate) const;
	bool GetRiskMgmtUserInfo(AssetOrgProRelation org,std::vector<TargetAccount>& Accounts);
public:
	bool AccountListIsExist(const std::string& InvestorID);
	//��ȡ�����ȫ���˻��б�
	void GetAccountList( std::vector<std::string>& accounts );
	void CDataCenter::GetAccountList( std::map<std::string,TrustTradeAccount>& AccountList);
	//
	RiskEventSubIDType GetRiskEventSubID(RiskEventIDType EventID);
	//��InvestorID����˻����ݿ�ID
	TradeAccountIDType GetTradeAccountID( const std::string& InvestorID );
	//
	AssetMgmtOrgIDType GetAssetMgmtOrgID( const std::string& InvestorID );
	//
	FinancialProductIDType GetFinancialProductID( const std::string& InvestorID );	
	//
	std::string GetTradeAccountID( TradeAccountIDType aID ) const;
	//��ȡί�н����˻���Ϣ
	bool GetTradeAccount(const std::string& InvestorID, TrustTradeAccount& account);
	//��InvestorID����˻����ݿ�ID
	RiskIndModuleType GetRiskIndModuleID( const std::string& InvestorID ) const;
	//��ȡ��Լ�б�
	void GetInstrumentList(std::vector<std::string>& vec) const;
	//��ȡƷ���б�
	void GetProductIDList(std::vector<std::string>& vec);
	//����������ȡ��Լ���г��ֲܳ�
	double  GetTotalPostion(const std::string& Instrument);
	//��ȡһ���˻��ض��ĺ�Լ���ֲܳ�
	int  GetInstrumentPostion(const std::string& Investor,const std::string& Instrument);
	bool GetWarningMapLevel(double value,std::map<int,RiskResponse>& warnings,int& LevelID) const;
	bool  GetMaxPositionRiskLevel(const std::string& Investor, const std::string& InstrumentID,double value,
		std::map<int,RiskResponse>& warnings,int& LevelID) const;
	bool  GetFundLossRiskLevel(const std::string& Investor, double value,
		std::map<int,RiskResponse>& warnings,int& LevelID) const;	
	bool  GetGappedMarketRiskLevel(const std::string& Investor,double value,
		std::map<int,RiskResponse>& warnings,int& LevelID) const;	
	bool  GetMarginRatioRiskLevel(const std::string& Investor,double value,
		std::map<int,RiskResponse>& warnings,int& LevelID) const;	

	bool  GetMaxProductPositionRiskLevel(const std::string& Investor, const std::string& ProductID,double value,
		std::map<int,RiskResponse>& warnings,int& LevelID) const;

	bool  GetFundNetValueLevel(const std::string& Investor, double value,
		std::map<int,RiskResponse>& warnings,int& LevelID) const;	
	bool GetWarningMapLevel_FundNetValue(double value,std::map<int,RiskResponse>& warnings,int& LevelID) const;

	//��ȡһ���˻��ض��ĺ�Լ���ֲܳ�
	void  GetProductPostion(const std::string& Investor, std::map<std::string, int>& mapProductCount);

	//�õ�����ĳ�˻������
	bool GetWithDraw( const std::string& InvestorID, double& dbWithDraw);
	double GetCurrMargin( const std::string& InvestorID );
	bool GetBalance( const std::string& InvestorID, double& dbValue );
	//����˻��ĳ�ʼȨ��
	double GetInitFund( const std::string& InvestorID );
	//����˻��ĳֲ�ӯ��
	double GetPositionProfit( const std::string& InvestorID );

	bool GetPreBalance(const std::string& InvestorID, double& dbValue);
	//��ȡ��������
	bool GetDepthMarketData(const std::string& Instrument, DepthMarketDataField& MarketDataField);
	//��ȡ��Լ����
	bool GetInstrumentInfo(const std::string& strInstrument,InstrumentField& InstrumentInfo) const;
	//
	void GetInstrumentInfoList( std::vector<InstrumentField>& vec );
	double GetSimulateGappedLoss( const std::string& InvestorID );
	std::string GetSpecialIDFromProduct(const std::string& Investor, const std::string& ProductID) const;
	std::string GetSpecialIDFromInstrument(const std::string& Investor, const std::string& InstrumentID) const;

	//����ֵ����
	bool GetNetFundParam(const std::string& Investor, NetFundParam& netFundParam);
	//����ǿƽ����
	bool GetForceCloseParam(const std::string& Investor, ForceCloseParam& forceCloseParam);

	//����ֵǿƽȨ��
	bool HasForceClosePermission(const std::string& Investor);

	//��ȡδ�ɽ���
	void GetOpenOrder(std::string strInvestor, std::vector<SequencialOrderField>& vec);
	//��ȡ�ֲ�
	void GetPosition(std::string strInvestor, std::vector<SequencialPositionField>& vec);
	//ͨ����Լ��ȡ������ID
	bool GetExchangeID(std::string strInstrumentID, std::string& strExchangeID);
	//ȡĳ����Լ���ǵ�ͣ��
	bool GetLimitPrice(std::string strInstrumentID, double& dUpperLimit, double& dLowerLimit);
	//��ȡĳ����Լ�Ĺҵ���
	void GetOpenOrderVolumn(std::string strInvestor, std::string strInstrumentID, char Direction,
		int& nTodayVolumn, int& nYestodayVolumn );

	//�õ���Ч������ʷ����
	bool GetGatherData(int nIDType, int nID, SGatherData& sGathData);

public:
	//��ȡ�����
	void GetDepositList( int AccID, std::vector<SeqDepositField>& vec ) const;
	//��ȡ�ɽ���¼
	void GetTradeList( int AccID, std::vector<SequencialTradeField>& vec) const;
	//��ȡ����
	void GetOrderList( int AccID, std::vector<SequencialOrderField>& vec ) const;
	//��ȡ�ֲ�
	void GetPositionList( int AccID, std::vector<SequencialPositionField>& vec) const;
	//��ȡ�˻����ս����ʽ���Ϣ
	void GetAllAccountFundList( int AccID, std::vector<RiskAllAccountField>& vec ) const;
	//��ȡ����
	void GetQuotList( std::string InstrumentID, std::vector<DepthMarketDataField>& vec ) const;
	//�����¼�
	void GetRiskEventList( int AccID, std::vector<RiskEvent>& vecEvent ) const;
	//�����¼�����
	void GetRiskEvtHandlingList( int AccID, std::vector<RiskEventHandling>& vec ) const;
	//ͳ��
	void GetStatisticsEntryList(const AssetOrgProRelation& org, std::vector<StatisticsEntry>& vec,bool bProtected=true) const;
	
	//�õ�����ֵ����
	void GetAllNetValueInfo( int AccID, std::vector<NetValueInfo>& vec ) const;
public://���ֶ���
	void SubscribeFund(int AccID);
	void SubscribePosition(int AccID);
	void SubscribeTrade(int AccID);
	void SubscribeOrder(int AccID);
	void SubscribeDeposit(int AccID);
	void SubscribeRiskEvent(int AccID);
	void SubscribeRiskEvtHandling(int AccID);
	void SubscribeQuot(const std::string& strInstrument);
	void SubscribeStatistics(const AssetOrgProRelation& org);
	void SubscribeNetValue(int AccID);

	void UnSubscribeFund(int AccID);
	void UnSubscribePosition(int AccID);
	void UnSubscribeTrade(int AccID);
	void UnSubscribeOrder(int AccID);
	void UnSubscribeDeposit(int AccID);
	void UnSubscribeRiskEvent(int AccID);
	void UnSubscribeRiskEvtHandling(int AccID);
	void UnSubscribeQuot(const std::string& strInstrument);
	void UnSubscribeStatistics(const AssetOrgProRelation& org);
	void UnSubscribeNetValue(int AccID);
public:
	void ReadAssetOrgProRelation();
	void ReadTrustTradeAccount();
	void ReadForceClosePermission();
	void ReadEventMessageTemplate();
	void ReadRiskMgmtUserInfo();
	void ReadNetFundParam();
	void ReadForceCloseParam();//����ǿƽ����
	void ReadGatherData();//�õ���Чͳ������
	void GetUpperLevelOrgID(AssetMgmtOrgIDType id,std::set<AssetMgmtOrgIDType>& ups);
	void InsertStatisticsItem( const TrustTradeAccount&  acc );
	void SaveAllData();
	void SaveGatherData();
	bool GetLowerAssetOrg( int nCurOrgID, std::vector<AssetMgmtOrganization>& vOut );
	void CalcLowerData( std::map<int, std::map<int, AssetMgmtOrganization>>& mapAssetOrgTree, 
		int nOwnerID, std::vector<AssetMgmtOrganization>& vOutData );
	void FilterAssetOrg( int nCurOrgID, std::vector<AssetMgmtOrganization>& vAssetOrg, 
		std::vector<AssetMgmtOrganization>& vOut );
	bool ProcessGatherAccountVec(int nTypeID, int nID, std::vector<TrustTradeAccount>& vec, std::vector<SGatherData>& vGatherData);
	
	void FrontDisconnected( int nReason );
	void SuccessLogin();
	bool IsOK();
	std::map<std::string,double>& GetComplexMapIterator(
		std::map<AssetOrgProRelation,std::map<std::string,double>>& itemmap,AssetOrgProRelation org);
	void Reset();
	void IdleBusinessDataMain();
	void UpdateBaseData();
	void GetRiskEventID( int& EventID );
	void GetRiskEventHandlingID( int& HandlingID );
	template<typename K,typename V>
	bool IsSameMap(const std::map<K,V>& map1,const std::map<K,V>& map2);
	template<typename K>
	bool IsSameRiskWarning(const std::map<K,std::map<RiskLevelType,RiskResponse>>& map1,
		const std::map<K,std::map<RiskLevelType,RiskResponse>>& map2);
	bool IsRiskWarningChanged();
	void FirstPrintf(const char* pMsg,bool bClear=false);
	void WriteLogInfo(const char* pMsg, const char* pContent, int nID=-1);
	CWriteLog* m_pWriteLogTest;
public:
		//DIRTYITEM(RiskWarning)
		//DIRTYITEM(AssetOrgProRelation)
		//DIRTYITEM(TrustTradeAccount)
		//DIRTYITEM(ForceClosePermission)
protected:
	static CDataCenter* m_DataCenter;
	CWriteLog* m_pWriteLog;
private://���������ؽ�������ȡȫ��������
	//����ʽ��˻�������Ϣ
	std::map<std::string,SeqPreRiskAccountField>	m_PreRiskFundAccount;
	CReadWriteLock									m_Mutex_PreRiskFundAccount;
	//���ͬ��Ͷ�����ʽ��˻�
	std::map<std::string,RiskSyncAccountField>		m_RiskFundAccount;
	CReadWriteLock									m_Mutex_RiskFundAccount;

	//����ֵʵʱ����
	std::map<std::string, NetValueInfo>					m_Accout2NetValue;
	CReadWriteLock									m_Mutex_Accout2NetValue;

	//�ֲ�
	POSITIONSTRUCT									m_AllPosition;
	CReadWriteLock									m_Mutex_AllPosition;
	//����
	TRADESTRUCT										m_AllTrade;
	CReadWriteLock									m_Mutex_AllTrade;
	//����
	ORDERSTRUCT										m_AllOrder;
	CReadWriteLock									m_Mutex_AllOrder;
	//��Լ
	std::map<std::string,ProductField>				m_AllProduct;
	CReadWriteLock									m_Mutex_AllProduct;
	//��Լ
	std::map<std::string,InstrumentField>			m_AllInstrument;
	CReadWriteLock									m_Mutex_AllInstrument;
	//�����
	std::map<std::string,std::map<int,SeqDepositField>> m_AllDeposit;
	CReadWriteLock									m_Mutex_AllDeposit;
	//����
	std::map<std::string,DepthMarketDataField>		m_DepthMarketData;
	CReadWriteLock									m_Mutex_DepthMarketData;

	

private://������ȫ������Ӧ������Ϊ��
	std::map<std::string,SubscribeType> m_SubscribeInfo;
	CReadWriteLock						m_Mutex_SubscribeInfo;
	std::set<std::string>				m_SubscribeQuot;
	CReadWriteLock						m_Mutex_SubscribeQuot;
	std::set<AssetOrgProRelation>		m_SubscribeStatistics;
	CReadWriteLock						m_Mutex_SubscribeStatistics;
private://������ȫ������Ӧ���ض����ݿ�
	std::map<std::string,TrustTradeAccount> m_AccountList;//�˻��б�<�˻�-��ʼȨ��>
	std::map<int,std::string>				m_AccountID2Str;//�����˻����˻�ID�Ķ�Ӧ��ϵ	
	CReadWriteLock							m_Mutex_AccountList;
	//ǿƽȨ��<����˻�ID>
	std::set<int>							m_ForceClosePermission;
	CReadWriteLock							m_Mutex_ForceClosePermission;
	//
	std::map<int,EventMessageTemplate>		m_EventMessageTemplate;
	CReadWriteLock							m_Mutex_EventMessageTemplate;

	std::map<AssetOrgProRelation,std::vector<TargetAccount>>	m_RiskMgmtUserInfo;
	CReadWriteLock							m_Mutex_RiskMgmtUserInfo;

	//����ֵ����
	std::map<std::string,NetFundParam>				m_mapAccount2NetFundParam;
	CReadWriteLock									m_Mutex_Account2NetFundParam;

	//����ǿƽ����
	std::map<std::string,ForceCloseParam>			m_mapAccount2ForceCloseParam;
	CReadWriteLock									m_Mutex_Account2ForceCloseParam;

	//������֯����ID�õ���һ����������ʷ��Ч��������
	std::map<int,  SGatherData>						m_mapOrgID2GatherData;
	CReadWriteLock									m_Mutex_OrgID2GatherData;

	//������Ʋ�Ʒ�õ���һ����������ʷ��Ч��������
	std::map<int,  SGatherData>						m_mapFinancialID2GatherData;
	CReadWriteLock									m_Mutex_FinancialID2GatherData;

	//���ݽ����˻��õ���һ����������ʷ��Ч��������
	std::map<int,  SGatherData>						m_mapAccountID2GatherData;
	CReadWriteLock									m_Mutex_AccountID2GatherData;

private://������ȫ������Ӧ���ض����ݿ�
	//����Ԥ��
	std::map<std::string,std::map<RiskLevelType,RiskResponse>>			m_SpecialMaxPositionRiskWarning;//<AccID+ProductID>
	RISKWARNINGSTRUCT	m_MaxPositionRiskWarning;	
	RISKWARNINGSTRUCT	m_FundLossRiskWarning;
	RISKWARNINGSTRUCT	m_GappedMarketRiskWarning;
	RISKWARNINGSTRUCT	m_MarginRatoRiskWarning;
	
	RISKWARNINGSTRUCT	m_ProductMaxPositionRiskWarning;	
	std::map<std::string,std::map<RiskLevelType,RiskResponse>>			m_SpecialProductMaxPositionRiskWarning;//<AccID+ProductID>

	RISKWARNINGSTRUCT	m_FundNetRiskWarning;
	bool				m_bRiskWarningChanged;
	CReadWriteLock														m_Mutex_RiskWarning;
	DISALLOW_COPY_AND_ASSIGN(CDataCenter);
private://������ȫ������Ӧ�ñ��浱������
	//�����¼���¼�б�
	std::map<RiskEventIDType,std::vector<RiskEvent>>			m_ActiveEvent;
	std::map<RiskEventIDType,std::vector<RiskEvent>>			m_HistoryEvent;
	std::map<RiskEventIDType,std::vector<RiskEventHandling>>    m_Handling;
	int															m_nRiskEventHandlingID;
	int															m_nRiskEventID;
	CReadWriteLock												m_Mutex_RiskEvent;
private:
	std::vector<std::string>							m_strFrontAddr;
	std::string											m_strTradeAddr;
	ReqRiskUserLoginField								m_LoginField;
	time_t												m_LoginTime;
	bool												m_lastStatus;
	std::set<int>										m_DayLoadSavedSet[2];
	DBAccess											m_dbAccess;
	//ͳ��
	volatile long m_StatisticsUpdate;
	std::map<AssetMgmtOrgIDType,AssetMgmtOrgIDType>				m_OrgIDMap;	
	std::map<AssetOrgProRelation,std::set<std::string>>			m_StatisticsItem;
	std::map<AssetOrgProRelation,double>					   m_StatisticsTotalValue[3];
	std::map<AssetOrgProRelation,std::map<std::string,double>> m_StatisticsInstrValue[2];
	CReadWriteLock											   m_Mutex_Statistics;

public:
	//�������ֵ
	double CalcFundNetValueRisk( const std::string& InvestorID);


	int  getExpoBase2(const double& d);
	bool equals(const double& d1, const double& d2);
};
