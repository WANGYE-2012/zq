#pragma once
class CRiskProcess;
class UserApi;
class CRiskMsgCalc
{
public:
	CRiskMsgCalc(UserApi* pApi);
	virtual ~CRiskMsgCalc();
	//���鵽��
	void NewDepthMarketData(const  std::string& InstrumentID );
	//�ʽ���
	void NewFundAccount(const  std::string& InvestorID );
	//�ֲֵ���
	void NewPosition(const std::string& InvestorID,const std::string& InstrumentID);
	//�ɽ�����
	void NewTrade(const std::string& InvestorID );
	//��������
	void NewOrder(const std::string& InvestorID );
public:
	//������㵥��Լ���ֲ�
	void ActiveCalcMaxPositionRisk( const std::string& InvestorID, const std::string& InstrumentID );
	//��������޿�Ԥ��
	void ActiveCalcFundLossRisk( const std::string& InvestorID );
	//�������ֲַ��ճ���Ԥ��
	void ActiveCalcGappedMarketRisk(const std::string& InvestorID);

	//���֤�����Ԥ��
	void ActiveCalcMarginRatoRisk(const std::string& InvestorID);

	//�����Ʒ���ֲַ���
	void ActiveCalcProductMaxPositionRisk( const std::string& InvestorID);

	//�������ֵ����
	void ActiveCalcFundNetValueRisk( const std::string& InvestorID);
private:
	//���㵥��Լ���ֲ�
	double CalcMaxPositionRisk(const std::string& InvestorID,const std::string& InstrumentID, int& nVolume);
	//�����޿�Ԥ��
	double CalcFundLossRisk(const std::string& InvestorID);
	//�г��������
	double CalcGappedMarketRisk(const std::string& InvestorID);
	//��֤��ٷֱ�Ԥ��
	double CalcMarginRatoRisk(const std::string& InvestorID);
	//Ʒ�����ֲ�Ԥ��
	void CalcProductMaxPositionRisk(const std::string& InvestorID, std::map<std::string, int>& mapProductCount);
public://�漰����ֵʵʱ���ͣ������ⲿ��Ҫ����
	//����ֵԤ��
	double CalcFundNetValueRisk( const std::string& InvestorID);
private:
	UserApi*			 m_pApi;
	CRiskProcess*		  m_RiskProcess;
};
