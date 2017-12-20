#pragma once
class CRiskProcess;
class CThreadSharedData;
class CRiskMsgCalc
{
public:
	CRiskMsgCalc(CThreadSharedData* pThreadSharedData);
	~CRiskMsgCalc(void);
	
	//���鵽��
	//void NewDepthMarketData(const  std::string& InstrumentID );
	//�ʽ���
	void NewFundAccount(const string& InvestorID );
	//�ֲֵ���
	void NewPosition(const string& InvestorID, bool bDoResponse = false);//bDoResponse �����ظ����ֵ��¼�ҲҪ������
	//�ɽ�����
	void NewTrade(const string& InvestorID, const string&  InstrumentID );
	//��������
	void NewOrder(const string& InvestorID, const string&  InstrumentID );
	//һ���µ�Timer����ʱ�������жϳֲ���ϸ�ĳֲ�ʱ��
	void NewTimer_PosiDetailTime();
	//�������ݹ�������׼���ã��ֲ��ж�
	void TradeDataInit();

	//��ط����ı�
	void ChangeRiskPlan();

public:
	//������㵥��Լ���ֲ�
	void ActiveCalcMaxPositionRisk( string InvestorID, std::string strTime, const std::string& InstrumentID);
	
	//�г��������
	void ActiveCalcMarketShockRisk(const string& InvestorID, std::string strTime, const std::string& InstrumentID);
	
	//�˻��޿�
	void ActiveCalcAccountLossRisk(const string& InvestorID, std::string strTime, bool bDoResponse = false);//bDoResponse �����ظ����ֵ��¼�ҲҪ������

	//�����޿�
	void ActiveCalcTodayLossRisk(const string& InvestorID, std::string strTime, bool bDoResponse = false);//bDoResponse �����ظ����ֵ��¼�ҲҪ������

	//��ҹ����
	void ActiveCalcGappedMarketRisk(const string& InvestorID, std::string strTime);

	//��֤���������
	void ActiveCalcMarginRatioRisk(const string& InvestorID, std::string strTime);

	//����ֵ���� 
	void ActiveCalcFundNetValueRisk(const string& InvestorID, std::string strTime, bool bDoResponse = false);//bDoResponse �����ظ����ֵ��¼�ҲҪ������

	//�ֲ���ϸ�ĳֲ�ʱ�����
	void ActiveCalcPosiDetalMaxTimeRisk(const string& InvestorID, 
				std::vector<PlatformStru_PositionDetail>& vecPosiDetails, 
				vector<SResponse>& vecResponse);
	//��������ֲ���ϸ�ĳֲ�ʱ�������ʧ���
	void ActivePosiDetalMaxTimeRiskLose(const string& InvestorID, std::string strTime, 
				std::map<PositionDetailKey, RiskEventLevelID>& mapDetailKey, std::string& strTimeBegin, std::string& strTimeEnd);

	//���ֲַ��ն���0ֵ��Ҫ���⴦��
	void ActiveCalcMaxPosZero( string InvestorID, std::string strTime);
	//�ж��ǲ���������0���յķ�ط���
	bool HasZeroRiskPlan(string InvestorID, std::string strTime, SResponse& vecResponse, std::string& strTimeBegin, std::string& strTimeEnd);
	
	void ProcessRisk(const string& InvestorID, bool bSetTradeStaus = true);// ����ǿƽ�����ղ��ı䲻��Ҫ����
	void ProcessForbid(const string& InvestorID);//�����˻����ƿ��֣������µ���CInterface_SvrTradeExcute::getObj().RiskControl();	������Ҫÿ�ζ���ֵ������ֵ����Ϊ�˻�������

    
	

public://�ж���û�з��գ��Լ����յ���ϸ��Ϣ
	int  GetMaxPositionRisk_Level( string InvestorID, std::string strTime, const std::string& InstrumentID, double&	dblRisk, int& nVolume, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetMarketShockRisk_Level(const string& InvestorID, std::string strTime, const std::string& InstrumentID, double& dblRisk, int& nVolume,  double& total, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetAccountLossRisk_Level(const string& InvestorID, std::string strTime, double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetTodayLossRisk_Level(const string& InvestorID, std::string strTime, double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetGappedMarketRisk_Level(const string& InvestorID, std::string strTime, double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetMarginRatioRisk_Level(const string& InvestorID, std::string strTime, double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetFundNetValueRisk_Level(const string& InvestorID, std::string strTime, double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);


private:
	int GetRiskLevel(string InvestorID, std::string strTime, RiskIndicatorType RiskType, double value, SResponse& vecResponse,int& LevelID, std::string& strTimeBegin, std::string& strTimeEnd);
	int GetMaxPositionRiskLevel(string InvestorID, std::string strTime, RiskIndicatorType RiskType, std::string InstrumentID, double dblRisk, SResponse& vecResponse,int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	bool CalcGetFundParam( const string& InvestorID, std::string strTime, NetFundParam& netFundParam);
	int CalcOverTime_DealerPosiDetail(const string& InvestorID, const string& strProdID, 
				SResponse& retRsp, int nOpenTime, int& nLevel);
	int CalcRemainTime(std::string& strInstID, std::string& strOpenTime);
	int CalcRealHoldingTime(std::string& strExchangeID, int nTime);
	int GetRemainTime(std::string& strOpenTime);
	//bool GetPosiDetailWarning(int nOrgID, SResponse& retWarning, std::vector<std::string>& vecAccount);
	void DoPosiDetailRisk(std::map<std::string, std::vector<PlatformStru_PositionDetail>>& mapDetail, 
					SResponse& warnings, int nLevel, time_t currTime, std::string& strTimeBegin, std::string& strTimeEnd);



private:
	CRiskProcess* m_RiskProcess;
	CThreadSharedData* m_pThreadShareData;

public:
	void SendQuto(PlatformStru_DepthMarketData& DepthMarketData);
	void SendFund(std::string strInvestor);
	void SendOrder(PlatformStru_OrderInfo* lpBuf);
	void SendTrader(PlatformStru_TradeInfo* lpBuf);
	void SendPosition(std::string strInvestor);


public:
	//�����¼�ʧЧ��ʱ�����
	void  ActiveEventPassTime();
	//�������Ʒ���
	void ActiveCalcTradeForbid(const string& InvestorID, std::string strTime, const std::string& InstrumentID);
	int  GetTradeForbid_Level(const string& InvestorID, std::string strTime, const std::string& InstrumentID, double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetTradeForbidRiskLevel(string InvestorID, std::string strTime, RiskIndicatorType RiskType, std::string InstrumentID, double dblRisk, SResponse& vecResponse,int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

	//��֤������
	void ActiveCalcMarginForbid(const string& InvestorID, std::string strTime);
	//dblRisk:��֤��/��̬Ȩ���������������� ; dbRiskMargin:��֤�𲻳��������
	int  GetMarginForbid_Level(const string& InvestorID, std::string strTime,  double& dblRisk, double& dbRiskMargin, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetMarginForbidRiskLevel(string InvestorID, std::string strTime, RiskIndicatorType RiskType, double dblRisk, double& dbRiskMargin, SResponse& vecResponse,int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

	//���ƿ���
	void ActiveCalcLossForbid(const string& InvestorID, std::string strTime);
	////dblRisk:������������������ ; dbRiskLoss:�����ܶ���������
	int  GetLossForbid_Level(const string& InvestorID, std::string strTime,  double& dblRisk, double& dbRiskLoss, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetLossForbidRiskLevel(string InvestorID, std::string strTime, RiskIndicatorType RiskType, double dblRisk, double& dbRiskLoss, SResponse& vecResponse,int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

	//����������
	void ActiveCalcOnedayLargetsLossForbid(const string& InvestorID, std::string strTime);
	int  GetOnedayLargetsLoss_Level(const string& InvestorID, std::string strTime,  double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

	//�������ֵ
	void ActiveCalcLossMax(const string& InvestorID, std::string strTime);
	int  GetLossMax_Level(const string& InvestorID, std::string strTime,  double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);


	//���س�ֵ
	void ActiveCalcMaxRetrace(const string& InvestorID, std::string strTime);
	int  GetMaxRetrace_Level(const string& InvestorID, std::string strTime,  double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

	//�˻��ֲֺ�Լ��ֵ
	void ActiveCalcContractsValuesForbid(const string& InvestorID, std::string strTime);
	int  GetContractsValues_Level(const string& InvestorID, std::string strTime,  double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

	//��������Լ��ֵ
	void ActiveBullBearValuesForbid(const string& InvestorID, std::string strTime);
	int  GetBullBearValues_Level(const string& InvestorID, std::string strTime,  double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

	//������������
	void ActiveLossContinueDays(const string& InvestorID, std::string strTime);
	int  GetLossContinueDays_Level(const string& InvestorID, std::string strTime,  double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

	//���ƿ�������
	void ActiveLossPercent(const string& InvestorID, std::string strTime);
	int  GetLossPercent_Level(const string& InvestorID, std::string strTime,  double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetLossPercentRiskLevel(string InvestorID, std::string strTime, RiskIndicatorType RiskType, double dbdynamic, double& dbInit, SResponse& vecResponse,int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

};
