#include "StdAfx.h"
#include "RiskMsgCalc.h"
#include "DBAccess.h"
#include "DataCenter.h"
#include "RiskProcess.h"
#include "UserApi.h"
CRiskMsgCalc::CRiskMsgCalc(UserApi* pApi)
:m_pApi(pApi)
{
	m_RiskProcess=new CRiskProcess();
}
CRiskMsgCalc::~CRiskMsgCalc()
{
	SAFE_DELETE(m_RiskProcess);
}
//���鵽��
void CRiskMsgCalc::NewDepthMarketData(const  std::string& InstrumentID )
{
	std::vector<std::string> accounts;
	SAFE_GET_DATACENTER()->GetAccountList(accounts);
	std::vector<std::string>::const_iterator it;
	for(it=accounts.begin();it!=accounts.end();++it)
	{
		const std::string InvestorID=*it;
		ActiveCalcMaxPositionRisk(InvestorID,InstrumentID);
		ActiveCalcProductMaxPositionRisk(InvestorID);
		ActiveCalcFundNetValueRisk(InvestorID);	
	}
}
//�ʽ���
void CRiskMsgCalc::NewFundAccount(const  std::string& InvestorID )
{
	ActiveCalcFundLossRisk(InvestorID);
	ActiveCalcGappedMarketRisk(InvestorID);	
	ActiveCalcMarginRatoRisk(InvestorID);	
	ActiveCalcFundNetValueRisk(InvestorID);	
}
//�ֲֵ���
void CRiskMsgCalc::NewPosition(const  std::string& InvestorID ,const std::string& InstrumentID)
{
	ActiveCalcMaxPositionRisk(InvestorID,InstrumentID);
	ActiveCalcGappedMarketRisk(InvestorID);
	ActiveCalcProductMaxPositionRisk(InvestorID);
	ActiveCalcFundNetValueRisk(InvestorID);	
}
//�ɽ�����
void CRiskMsgCalc::NewTrade(const  std::string& InvestorID )
{
	ActiveCalcProductMaxPositionRisk(InvestorID);
	ActiveCalcFundNetValueRisk(InvestorID);	
}
//��������
void CRiskMsgCalc::NewOrder(const  std::string& InvestorID )
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//������㵥��Լ���ֲ�
void CRiskMsgCalc::ActiveCalcMaxPositionRisk( const std::string& InvestorID, const std::string& InstrumentID )
{
	double dblValue=0.0;
	int nVolume = 0;
	std::map<int,RiskResponse>  warnings;
	int levelID=0;
	if(CDataCenter::Get())
	{
		dblValue=CalcMaxPositionRisk(InvestorID,InstrumentID, nVolume);
		if(CDataCenter::Get()->GetMaxPositionRiskLevel(InvestorID,InstrumentID,dblValue,warnings,levelID))
		{
			m_RiskProcess->AddMaxPositionEvent(InvestorID,InstrumentID,dblValue,nVolume,levelID);
		}
		else if(m_RiskProcess->HasMaxPositionRisk(InvestorID,InstrumentID))
		{//levelID =0;�����¼���ʧ
			m_RiskProcess->AddMaxPositionEvent(InvestorID,InstrumentID,dblValue,nVolume,0);
		}
	}	
}
//��������޿�Ԥ��
void CRiskMsgCalc::ActiveCalcFundLossRisk( const std::string& InvestorID )
{
	int levelID=0;
	double dblValue=0.0;
	std::map<int,RiskResponse>  warnings;
	if(CDataCenter::Get())
	{
		dblValue=CalcFundLossRisk(InvestorID);
		if(CDataCenter::Get()->GetFundLossRiskLevel(InvestorID,dblValue,warnings,levelID))
		{
			m_RiskProcess->AddFundLossEvent(InvestorID,dblValue,levelID);
		}
		else if(m_RiskProcess->HasFundLossRisk(InvestorID))
		{//levelID =0;�����¼���ʧ
			m_RiskProcess->AddFundLossEvent(InvestorID,dblValue,0);
		}
	}	
}
//�������ֲַ��ճ���Ԥ��
void CRiskMsgCalc::ActiveCalcGappedMarketRisk(const std::string& InvestorID)
{
	int levelID=0;
	double dblValue=0.0;
	std::map<int,RiskResponse>  warnings;
	if(CDataCenter::Get())
	{
		dblValue=CalcGappedMarketRisk(InvestorID);
		if(CDataCenter::Get()->GetGappedMarketRiskLevel(InvestorID,dblValue,warnings,levelID))
		{
			m_RiskProcess->AddGappedMarketEvent(InvestorID,dblValue,levelID);
		}
		else if(m_RiskProcess->HasGappedMarketRisk(InvestorID))
		{//levelID =0;�����¼���ʧ
			m_RiskProcess->AddGappedMarketEvent(InvestorID,dblValue,0);
		}


	}	
}

//���֤�����Ԥ��
void CRiskMsgCalc::ActiveCalcMarginRatoRisk(const std::string& InvestorID)
{
	int levelID=0;
	double dblValue=0.0;
	std::map<int,RiskResponse>  warnings;
	if(CDataCenter::Get())
	{
		dblValue=CalcMarginRatoRisk(InvestorID);
		if(CDataCenter::Get()->GetMarginRatioRiskLevel(InvestorID,dblValue,warnings,levelID))
		{
			m_RiskProcess->AddMarginRatioEvent(InvestorID,dblValue,levelID);
		}
		else if(m_RiskProcess->HasMarginRatioRisk(InvestorID))
		{//levelID =0;�����¼���ʧ
			m_RiskProcess->AddMarginRatioEvent(InvestorID,dblValue,0);
		}

	}	
}
//��������Ʒ���ֲַ���
void CRiskMsgCalc::ActiveCalcProductMaxPositionRisk( const std::string& InvestorID)
{
	std::map<int,RiskResponse>  warnings;
	int levelID=0;
	if(CDataCenter::Get())
	{
		std::map<std::string, int> mapProductCount;
		CalcProductMaxPositionRisk(InvestorID,mapProductCount);

		for(std::map<std::string, int>::iterator it= mapProductCount.begin(); it!= mapProductCount.end(); it++)
		{
			std::string strProduct = it->first;
			double dblValue		   = it->second;
			if(CDataCenter::Get()->GetMaxProductPositionRiskLevel(InvestorID,strProduct,dblValue,warnings,levelID))
			{
				m_RiskProcess->AddMaxProductPositionEvent(InvestorID,strProduct,dblValue,levelID);
			}
			else if(m_RiskProcess->HasMaxProductPositionRisk(InvestorID,strProduct))
			{//levelID =0;�����¼���ʧ
				m_RiskProcess->AddMaxProductPositionEvent(InvestorID,strProduct,dblValue,0);
			}
		}
	}	
}
//�������ֵ����
void CRiskMsgCalc::ActiveCalcFundNetValueRisk( const std::string& InvestorID )
{
	int levelID=0;
	double dblValue=0.0;
	std::map<int,RiskResponse>  warnings;
	if(CDataCenter::Get())
	{
		dblValue = CalcFundNetValueRisk(InvestorID);
		if(dblValue <0)
			return;//û�����ø��˻�����ϸ�ʽ���Ϣ���޷����з����ж�

		if(CDataCenter::Get()->GetFundNetValueLevel(InvestorID,dblValue,warnings,levelID))
		{
			m_RiskProcess->AddFundNetValueEvent(InvestorID,dblValue,levelID);
		}
		else if(m_RiskProcess->HasFundNetValueRisk(InvestorID))
		{//levelID =0;�����¼���ʧ
			m_RiskProcess->AddFundNetValueEvent(InvestorID,dblValue,0);
		}
	}	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//����Լ���ֲ�,��Ӧ����ı�ͳֲָı�
double CRiskMsgCalc::CalcMaxPositionRisk(const std::string& InvestorID,const std::string& InstrumentID, int& nVolume)
{
	double dblRisk=0.0;
	nVolume = 0;
	if(CDataCenter::Get())
	{
		double total=CDataCenter::Get()->GetTotalPostion(InstrumentID);
		if(total>0)
		{
			int pos=CDataCenter::Get()->GetInstrumentPostion(InvestorID,InstrumentID);
			nVolume = pos;
			dblRisk=pos/total;
		}
	}
	return dblRisk;
}
//�޿�Ԥ������Ӧ�ʽ�ı�
double CRiskMsgCalc::CalcFundLossRisk(const std::string& InvestorID)
{
	double dblRisk=0.0;
	if(CDataCenter::Get())
	{
		double dblMaxFundLoss=CDataCenter::Get()->GetInitFund(InvestorID);
		if(dblMaxFundLoss>0)
		{
			double dblBalance= 0.0;
			CDataCenter::Get()->GetBalance(InvestorID, dblBalance);//�˻���Ȩ��
			dblRisk=1.00-dblBalance/dblMaxFundLoss;
		}
	}
	return dblRisk;
}
//�ֲַ��ճ���Ԥ��
double CRiskMsgCalc::CalcGappedMarketRisk(const std::string& InvestorID)
{
	double dblRisk=0.0;
	if(CDataCenter::Get())
	{
		double dblMaxGappedLoss=CDataCenter::Get()->GetInitFund(InvestorID);
		if(dblMaxGappedLoss>0)
		{
			double dblBalance= 0.0;
			CDataCenter::Get()->GetBalance(InvestorID, dblBalance);//�˻���Ȩ��
			double dblSimulateGappedLoss=CDataCenter::Get()->GetSimulateGappedLoss(InvestorID);
			dblRisk=(dblMaxGappedLoss-dblBalance+dblSimulateGappedLoss)/dblMaxGappedLoss;
		}
	}
	return dblRisk;
}
//��֤�����Ԥ��
// ���ָ�� ����һ��  ��֤��ٷֱ�   ��ʽ����ռ�ñ�֤��/��̬Ȩ��  ���ڵ��� ���õķ�ֵ  �ʹ�����������
double CRiskMsgCalc::CalcMarginRatoRisk(const std::string& InvestorID)
{
	double dblRisk=0.0;
	if(CDataCenter::Get())
	{
		double dblMaxGappedLoss=CDataCenter::Get()->GetInitFund(InvestorID);
		if(dblMaxGappedLoss>0)
		{
			double dblCurrMargin=CDataCenter::Get()->GetCurrMargin(InvestorID);//ռ�ñ�֤��
			double dblBalance= 0.0;
			CDataCenter::Get()->GetBalance(InvestorID, dblBalance);//�˻���Ȩ��		
			
			dblRisk=dblCurrMargin/dblBalance;
		}
	}
	return dblRisk;
}
//Ʒ�����ֲ֣�����ĳ��Ʒ�ֵĻ��ֲܳ�����Ԥ����������гֲ�������Ԥ��ֵ���򴥷���Ӧ���ա�
void CRiskMsgCalc::CalcProductMaxPositionRisk(const std::string& InvestorID, std::map<std::string, int>& mapProductCount)
{	
	if(CDataCenter::Get())
	{
		CDataCenter::Get()->GetProductPostion(InvestorID, mapProductCount);		
	}
}
//����ֵԤ��
/*
����ֵ���á�  
					����			���ڣ��ڻ���
�ݶ�				Ao				Ai
�ʲ���ֵ			Qo				Qi
��λ��ֵ			Po = Qo / Ao	Pi = Qi / Ai
���ʲ���ֵ			P = ��Qo + Qi��/��Ao + Ai��	
���С��ݶ�Ao��Ai ���ֶ��趨�ġ�  �ʲ���ֵQo��ÿ���ֶ������ˡ� Qi��ÿ�����еĶ�̬Ȩ�档   ���ڵ�λ��ֵ�����ʲ���ֵ��ʵʱ����ġ�
P<���õ�ֵ��

*/
double CRiskMsgCalc::CalcFundNetValueRisk( const std::string& InvestorID)
{
/*	double dblBalance=CDataCenter::Get()->GetBalance(InvestorID);//�˻���Ȩ��		
	NetFundParam netFundParam;
	if(!CDataCenter::Get()->GetNetFundParam(InvestorID, netFundParam))
		return -1;
	
	double dbP = 0.0;
	if(netFundParam.dOuterVolumn + netFundParam.dInnerVolumn > 0)
		dbP = (netFundParam.dOuterNetAsset + dblBalance)/(netFundParam.dOuterVolumn + netFundParam.dInnerVolumn);
*/
	double dbP = CDataCenter::Get()->CalcFundNetValueRisk(InvestorID);	
	return dbP;

	
}