#pragma once
class CRiskProcess;
class CThreadSharedData;
class CRiskMsgCalc_Account
{
public:
	CRiskMsgCalc_Account(CThreadSharedData* pThreadSharedData);
	~CRiskMsgCalc_Account(void);
	
	//�ʽ���
	void NewFundAccount(const std::string & BrokerID, const string& InvestorID );
	//�ֲֵ���
	void NewPosition(const std::string & BrokerID, const string& InvestorID, bool bDoResponse = false);//bDoResponse �����ظ����ֵ��¼�ҲҪ������
	//�ɽ�����
	void NewTrade(const std::string & BrokerID, const string& InvestorID, const string&  InstrumentID );
	//��������
	void NewOrder(const std::string & BrokerID, const string& InvestorID, const string&  InstrumentID );
	//һ���µ�Timer����ʱ�������жϳֲ���ϸ�ĳֲ�ʱ��
	void NewTimer_PosiDetailTime();
	//�������ݹ�������׼���ã��ֲ��ж�
	void TradeDataInit();

	//��ط����ı�
	void ChangeRiskPlan();

public:
	//������㵥��Լ���ֲ�
	void ActiveCalcMaxPositionRisk(const std::string & BrokerID, string InvestorID, std::string strTime, const std::string& InstrumentID);
	
	//�г��������
	void ActiveCalcMarketShockRisk(const std::string & BrokerID, const string& InvestorID, std::string strTime, const std::string& InstrumentID);
	
	//�����޿�
	void ActiveCalcTodayLossRisk(const std::string & BrokerID, const string& InvestorID, std::string strTime, bool bDoResponse = false);//bDoResponse �����ظ����ֵ��¼�ҲҪ������

	//��֤���������
	void ActiveCalcMarginRatioRisk(const std::string & BrokerID, const string& InvestorID, std::string strTime);

	//����ֵ���� 
	void ActiveCalcFundNetValueRisk(const std::string & BrokerID, const string& InvestorID, std::string strTime, bool bDoResponse = false);//bDoResponse �����ظ����ֵ��¼�ҲҪ������

	//�ֲ���ϸ�ĳֲ�ʱ�����
//	void ActiveCalcPosiDetalMaxTimeRisk(const string& InvestorID, 
//				std::vector<PlatformStru_PositionDetail>& vecPosiDetails, 
//				vector<SResponse>& vecResponse);
	//��������ֲ���ϸ�ĳֲ�ʱ�������ʧ���
	void ActivePosiDetalMaxTimeRiskLose(const string& InvestorID, std::string strTime, 
				std::map<PositionDetailKey, RiskEventLevelID>& mapDetailKey, std::string& strTimeBegin, std::string& strTimeEnd);

	//�������


public://�ж���û�з��գ��Լ����յ���ϸ��Ϣ
	int  GetMaxPositionRisk_Level(const std::string & BrokerID, string InvestorID, std::string strTime,const std::string& InstrumentID, double&	dblRisk, int& nVolume, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetMarketShockRisk_Level(const std::string & BrokerID,const string& InvestorID, std::string strTime,const std::string& InstrumentID, double& dblRisk, int& nVolume,  double& total, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetTodayLossRisk_Level(const std::string & BrokerID,const string& InvestorID, std::string strTime,double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetMarginRatioRisk_Level(const std::string & BrokerID,const string& InvestorID, std::string strTime,double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetFundNetValueRisk_Level(const std::string & BrokerID,const string& InvestorID, std::string strTime,double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

private:
	int GetRiskLevel(const std::string & BrokerID, string InvestorID,std::string strTime, RiskIndicatorType RiskType, double value, SResponse& vecResponse,int& LevelID, std::string& strTimeBegin, std::string& strTimeEnd);
	int GetMaxPositionRiskLevel(const std::string & BrokerID, string InvestorID,std::string strTime, RiskIndicatorType RiskType, std::string InstrumentID, double dblRisk, SResponse& vecResponse,int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	bool CalcGetFundParam(const std::string & BrokerID, const string& InvestorID, NetFundParam& netFundParam);
	int CalcOverTime_DealerPosiDetail(const string& InvestorID, const string& strProdID, 
				SResponse& retRsp, int nOpenTime, int& nLevel);
	int CalcRemainTime(std::string& strInstID, std::string& strOpenTime);
	int CalcRealHoldingTime(std::string& strExchangeID, int nTime);
	int GetRemainTime(std::string& strOpenTime);
	void DoPosiDetailRisk(std::map<std::string, std::vector<PlatformStru_PositionDetail>>& mapDetail, 
					SResponse& warnings, int nLevel, time_t currTime, std::string& strTimeBegin, std::string& strTimeEnd);

	

private:
	CRiskProcess* m_RiskProcess;
	CThreadSharedData* m_pThreadShareData;


};
