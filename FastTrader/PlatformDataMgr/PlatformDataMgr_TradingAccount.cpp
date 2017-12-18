//ά���ײ�ֲ���Ϣ

#pragma warning(disable:4996)

#include <vector>
#include <string>
#include <map>
using namespace std;

#include "PlatformDataMgr_TradingAccount.h"
#include "PlatformDataMgrMacro.h"
#include "FileOpr.h"
#include "Module-Misc2/GlobalFunc.h"


#ifdef WIN32
#pragma unmanaged
#endif 



CPlatformDataMgr_TradingAccount::CPlatformDataMgr_TradingAccount(CPlatformDataMgr_Instruments& refInstruments,
                                                                 CPlatformDataMgr_Quots& refQuots,
                                                                 CPlatformDataMgr_Trades& refTrades,
                                                                 CPlatformDataMgr_Orders& refOrders,
                                                                 CPlatformDataMgr_PositionDetailCombs& refPositionDetailCombs,
                                                                 CPlatformDataMgr_PositionDetails& refPositionDetails,
                                                                 CPlatformDataMgr_PositionCombs& refPositionCombs,
                                                                 CPlatformDataMgr_Positions& refPositions)
: m_RefInstruments(refInstruments),
  m_RefQuots(refQuots),
  m_RefTrades(refTrades),
  m_RefOrders(refOrders),
  m_RefPositionDetailCombs(refPositionDetailCombs),
  m_RefPositionDetails(refPositionDetails),
  m_RefPositionCombs(refPositionCombs),
  m_RefPositions(refPositions),
  m_bValid(false)
{
}

CPlatformDataMgr_TradingAccount::~CPlatformDataMgr_TradingAccount(void)
{
}

///���ý����ʽ��˻���Ϣ
void CPlatformDataMgr_TradingAccount::SetTradingAccount(const PlatformStru_TradingAccountInfo& inData)
{
    Lock_CPlatformDataMgr;
	memcpy( &m_TradingAccount, &inData, sizeof(m_TradingAccount));

    //��ѯ�����Ķ����ʽ�Ϊ0�����м���һ��
	if(util::isInvalidValue(m_TradingAccount.FrozenMargin)||util::isInvalidValue(m_TradingAccount.FrozenCommission))
		m_TradingAccount.FrozenCash=util::GetDoubleInvalidValue();
	else
		m_TradingAccount.FrozenCash = m_TradingAccount.FrozenMargin + m_TradingAccount.FrozenCommission;

    //ctp��ѯ�ʽ𷵻غ󣬾�̬Ȩ�桢��̬Ȩ�桢���ö�Ⱥͷ��ն���Ҫ�Լ���
    CalcuStaticProfic_Internal();
    CalcuDynamicProfic_Internal();
    CalcuAvailable_Interanl();
    CalcuRiskDegree_Internal();
    m_bValid=true;
    Unlock_CPlatformDataMgr;
}
int CPlatformDataMgr_TradingAccount::GetTradingAccountAvailable(double& fAvailable)
{
    Lock_CPlatformDataMgr;
    fAvailable=m_TradingAccount.Available;
    int ret= m_bValid?0:-1;
    Unlock_CPlatformDataMgr;
    return ret;   
}
double CPlatformDataMgr_TradingAccount::GetTradingAccountDynamicProfit(void)
{
    Lock_CPlatformDataMgr;
	double DynamicProfit=m_bValid?m_TradingAccount.DynamicProfit:util::GetDoubleInvalidValue();
    Unlock_CPlatformDataMgr;
	return DynamicProfit;
}

int CPlatformDataMgr_TradingAccount::GetTradingAccountWithdrawQuota(double& fWithdrawQuota)
{
    Lock_CPlatformDataMgr;
    fWithdrawQuota=m_TradingAccount.WithdrawQuota;
    int ret= m_bValid?0:-1;
    Unlock_CPlatformDataMgr;
    return ret;   
}
int CPlatformDataMgr_TradingAccount::GetTradingAccountID(char* AccountID,int rltsize)
{
    Lock_CPlatformDataMgr;
	strncpy(AccountID,m_TradingAccount.AccountID,rltsize);
    AccountID[rltsize-1]=0;
    int ret= m_bValid?0:-1;
    Unlock_CPlatformDataMgr;
    return ret;   
}
///��ȡ�����ʽ��˻���Ϣ
int CPlatformDataMgr_TradingAccount::GetTradingAccount(PlatformStru_TradingAccountInfo& outData)
{
    Lock_CPlatformDataMgr;
	memcpy(&outData,&m_TradingAccount,sizeof(PlatformStru_TradingAccountInfo));
    int ret= m_bValid?0:-1;
    Unlock_CPlatformDataMgr;
    return ret;   
}
///��ȡ�˻��ʽ��ı���Ϣ
int CPlatformDataMgr_TradingAccount::GetAccountText(std::string& outData,int language)
{
    int ret=0;

    LOG_INFO("CPlatformDataMgr_TradingAccount::GetAccountText");

    Lock_CPlatformDataMgr;

    std::string strTemp,strBlank,strSeparator,strSeparator2;
	strBlank.append(10,' ');
	strSeparator.append(50,'=');
	strSeparator+="\r\n";
	strSeparator2.append(100,'-');
	strSeparator2+="\r\n";
	outData+=strSeparator;
#define FMT_BUFFERSIZE  512
#define AC_FM(tag)		util::isInvalidValue(tag)?"-":GlobalFunc::GetAccountFormatString(tag,2).c_str()
	char temp[FMT_BUFFERSIZE+1]={0};
	if(language==LANGUAGE_CHINESE)
	{

		_snprintf(temp,FMT_BUFFERSIZE,"  �ϴν���׼����:                         %s\r\n",AC_FM(m_TradingAccount.PreBalance));
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- �ϴ����ö��                             %s\r\n",AC_FM(m_TradingAccount.PreCredit));
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- �ϴ���Ѻ���:                            %s\r\n",AC_FM(m_TradingAccount.PreMortgage));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ ��Ѻ���:                                 %s\r\n",AC_FM(m_TradingAccount.Mortgage));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- ���ճ���:                                  %s\r\n",AC_FM(m_TradingAccount.Withdraw));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ �������:                                 %s\r\n",AC_FM(m_TradingAccount.Deposit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;

		_snprintf(temp,FMT_BUFFERSIZE,"= ���վ�̬Ȩ��:                           %s\r\n",AC_FM(m_TradingAccount.StaticProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ ƽ��ӯ��:                                 %s\r\n",AC_FM(m_TradingAccount.CloseProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		if(m_bValid&&m_TradingAccount.PositionProfit>0.0)
			_snprintf(temp,FMT_BUFFERSIZE,"+ �ֲ�ӯ��:                                 %s(����������ʽ�)\r\n",AC_FM(m_TradingAccount.PositionProfit));
		else
			_snprintf(temp,FMT_BUFFERSIZE,"+ �ֲ�ӯ��:                                 %s\r\n",AC_FM(m_TradingAccount.PositionProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- ������:                                     %s\r\n",AC_FM(m_TradingAccount.Commission));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;

		_snprintf(temp,FMT_BUFFERSIZE,"= ��̬Ȩ��:                                %s\r\n",AC_FM(m_TradingAccount.DynamicProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- ռ�ñ�֤��:                               %s\r\n",AC_FM(m_TradingAccount.CurrMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- ���ᱣ֤��:                               %s\r\n",AC_FM(m_TradingAccount.FrozenMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- ����������:                               %s\r\n",AC_FM(m_TradingAccount.FrozenCommission));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- ���֤��:                               %s\r\n",AC_FM(m_TradingAccount.DeliveryMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ ���ý��                                  %s\r\n",AC_FM(m_TradingAccount.Credit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;
		_snprintf(temp,FMT_BUFFERSIZE,"= �����ʽ�:                                %s\r\n",AC_FM(m_TradingAccount.Available));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		outData+=strSeparator;
		outData+="\r\n\r\n";
		outData+=strSeparator;
		_snprintf(temp,FMT_BUFFERSIZE,"= �����ʽ�:                                 %.2f\r\n",0.0);
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"= ��ȡ�ʽ�:                                 %s\r\n",AC_FM(m_TradingAccount.WithdrawQuota));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		outData+=strSeparator;
	}
	if(language==LANGUAGE_ENGLISH)
	{
		_snprintf(temp,FMT_BUFFERSIZE,"  PreBalance:                                        %s\r\n",AC_FM(m_TradingAccount.PreBalance));
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- PreCredit                                            %s\r\n",AC_FM(m_TradingAccount.PreCredit));
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- PreMortgage:                                      %s\r\n",AC_FM(m_TradingAccount.PreMortgage));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Mortgage:                                         %s\r\n",AC_FM(m_TradingAccount.Mortgage));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Today Withdraw:                                 %s\r\n",AC_FM(m_TradingAccount.Withdraw));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Today Deposit:                                   %s\r\n",AC_FM(m_TradingAccount.Deposit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;

		_snprintf(temp,FMT_BUFFERSIZE,"= Today Static Profit:                             %s\r\n",AC_FM(m_TradingAccount.StaticProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Close Profit:                                        %s\r\n",AC_FM(m_TradingAccount.CloseProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Position Profit:                                     %s\r\n",AC_FM(m_TradingAccount.PositionProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Commission:                                         %s\r\n",AC_FM(m_TradingAccount.Commission));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;

		_snprintf(temp,FMT_BUFFERSIZE,"= Dynamic Profit:                                   %s\r\n",AC_FM(m_TradingAccount.DynamicProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Current Margin:                                    %s\r\n",AC_FM(m_TradingAccount.CurrMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Frozen Margin:                                     %s\r\n",AC_FM(m_TradingAccount.FrozenMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Frozen Commission:                               %s\r\n",AC_FM(m_TradingAccount.FrozenCommission));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Delivery Margin:                                    %s\r\n",AC_FM(m_TradingAccount.DeliveryMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Credit                                                %s\r\n",AC_FM(m_TradingAccount.Credit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;
		_snprintf(temp,FMT_BUFFERSIZE,"= Available:                                           %s\r\n",AC_FM(m_TradingAccount.Available));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		outData+=strSeparator;
		outData+="\r\n\r\n";
		outData+=strSeparator;
		_snprintf(temp,FMT_BUFFERSIZE,"= Minimum Quota:                                  %.2f\r\n",0.0);
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"= Withdraw Quota:                                %s\r\n",AC_FM(m_TradingAccount.WithdrawQuota));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		outData+=strSeparator;
	}

    Unlock_CPlatformDataMgr;

	LOG_INFO("CPlatformDataMgr_TradingAccount::GetAccountText-end");
	return ret;   
}
///��ѯ������Ӧʱ����̬�����ʽ��˻���Ϣ
void CPlatformDataMgr_TradingAccount::UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData(void)
{
    Lock_CPlatformDataMgr;

    //ȡ���������ѺͶ��ᱣ֤��
    m_RefOrders.GetFreezeMargin_Commission(m_TradingAccount.FrozenMargin,m_TradingAccount.FrozenCommission);

    //���㶳����
	if(util::isInvalidValue(m_TradingAccount.FrozenMargin)||util::isInvalidValue(m_TradingAccount.FrozenCommission))
		m_TradingAccount.FrozenCash=util::GetDoubleInvalidValue();
	else
		m_TradingAccount.FrozenCash = m_TradingAccount.FrozenMargin + m_TradingAccount.FrozenCommission;

    //�����ʽ������еĿ����ʽ�
    CalcuAvailable_Interanl();

    Unlock_CPlatformDataMgr;
}


///�������ʱ����̬�����ʽ��˻���Ϣ
void CPlatformDataMgr_TradingAccount::DynamicUpdateTradingAccount_RtnDepthMarketData(const string InstrumentID)
{
    Lock_CPlatformDataMgr;

    //����гֲ���ϸ���Ž��и���
    if(m_RefPositionDetails.HavePositionDetail(InstrumentID))
    {

        //�������ն��еĳֲ�ӯ��
        m_RefPositions.GetAllPositionProfitByDate( m_TradingAccount.PositionProfit);
        //�����ʽ������еĶ�̬Ȩ��ֵ
        CalcuDynamicProfic_Internal();
        //�����ʽ������еĿ����ʽ�
        CalcuAvailable_Interanl();
        //�����ʽ������еķ��ն�ֵ
        CalcuRiskDegree_Internal();
    }

    Unlock_CPlatformDataMgr;
}
///�ɽ��ر�ʱ�Ͳ�ѯ�ֲ���Ӧʱ����̬�����ʽ��˻���Ϣ
void CPlatformDataMgr_TradingAccount::UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted(void)
{
    Lock_CPlatformDataMgr;

    m_RefPositions.GetAllPositionProfitByDate( m_TradingAccount.PositionProfit);
    m_RefPositions.GetAllCloseProfitByDate(m_TradingAccount.CloseProfit);

    double Margin1=0,Margin2=0;
    m_RefPositions.GetAllMargin(Margin1);
    m_RefPositionCombs.GetAllMargin(Margin2);
    m_TradingAccount.CurrMargin=Margin1+Margin2;

    //����������
    m_TradingAccount.Commission=m_RefTrades.GetTotalCommission();
    //�����ʽ������еĶ�̬Ȩ��ֵ
    CalcuDynamicProfic_Internal();
    //�����ʽ������еĿ����ʽ�
    CalcuAvailable_Interanl();
    //�����ʽ������еķ��ն�ֵ
    CalcuRiskDegree_Internal();

    Unlock_CPlatformDataMgr;
}
///��ѯ��֤������Ӧʱ����̬�����ʽ��˻���Ϣ
void CPlatformDataMgr_TradingAccount::DynamicUpdateTradingAccount_RspQryRate(void)
{
    Lock_CPlatformDataMgr;

    //�����ʽ������еı�֤��
    double Margin1=0,Margin2=0;
    m_RefPositions.GetAllMargin(Margin1);
    m_RefPositionCombs.GetAllMargin(Margin2);
    m_TradingAccount.CurrMargin=Margin1+Margin2;

    //�����ʽ������еĿ����ʽ�
    CalcuAvailable_Interanl();
    //�����ʽ������еķ��ն�ֵ
    CalcuRiskDegree_Internal();

    Unlock_CPlatformDataMgr;
}
///��ѯ����������Ӧʱ����̬�����ʽ��˻���Ϣ
void CPlatformDataMgr_TradingAccount::DynamicUpdateTradingAccount_RspQryInstrumentCommissionRate(void)
{
    Lock_CPlatformDataMgr;

    m_TradingAccount.Commission=m_RefTrades.GetTotalCommission();

    //�����ʽ������еĶ�̬Ȩ��ֵ
    CalcuDynamicProfic_Internal();
    //�����ʽ������еĿ����ʽ�
    CalcuAvailable_Interanl();
    //�����ʽ������еķ��ն�ֵ
    CalcuRiskDegree_Internal();

    Unlock_CPlatformDataMgr;
}
void CPlatformDataMgr_TradingAccount::clear(void)
{
    Lock_CPlatformDataMgr;
    m_TradingAccount.clear();
    m_bValid=false;
    Unlock_CPlatformDataMgr;
}
bool CPlatformDataMgr_TradingAccount::IsValid(void)
{
    return m_bValid;
}

//�����ʽ������еľ�̬Ȩ��ֵ
void CPlatformDataMgr_TradingAccount::CalcuStaticProfic_Internal()
{
    if(util::isInvalidValue(m_TradingAccount.PreBalance)||
        util::isInvalidValue(m_TradingAccount.PreCredit)||
        util::isInvalidValue(m_TradingAccount.PreMortgage)||
        util::isInvalidValue(m_TradingAccount.Mortgage)||
        util::isInvalidValue(m_TradingAccount.Withdraw)||
        util::isInvalidValue(m_TradingAccount.Deposit))
    {
        m_TradingAccount.DynamicProfit=util::GetDoubleInvalidValue();
    }
    else
    {
		m_TradingAccount.StaticProfit =  
            m_TradingAccount.PreBalance
            - m_TradingAccount.PreCredit
            - m_TradingAccount.PreMortgage
            + m_TradingAccount.Mortgage
            - m_TradingAccount.Withdraw
            + m_TradingAccount.Deposit;
    }        
}

//�����ʽ������еĶ�̬Ȩ��ֵ
void CPlatformDataMgr_TradingAccount::CalcuDynamicProfic_Internal()
{
    if(util::isInvalidValue(m_TradingAccount.StaticProfit)||
        util::isInvalidValue(m_TradingAccount.CloseProfit)||
        util::isInvalidValue(m_TradingAccount.PositionProfit)||
        util::isInvalidValue(m_TradingAccount.Commission))
    {
        m_TradingAccount.DynamicProfit=util::GetDoubleInvalidValue();
    }
    else
    {
		m_TradingAccount.DynamicProfit =  
            m_TradingAccount.StaticProfit
            + m_TradingAccount.CloseProfit 
            + m_TradingAccount.PositionProfit
            - m_TradingAccount.Commission;
    }        
}
//�����ʽ������еĿ����ʽ�
void CPlatformDataMgr_TradingAccount::CalcuAvailable_Interanl()
{
    if( util::isInvalidValue(m_TradingAccount.DynamicProfit)||
        util::isInvalidValue(m_TradingAccount.CurrMargin)||
        util::isInvalidValue(m_TradingAccount.FrozenMargin)||
        util::isInvalidValue(m_TradingAccount.FrozenCommission)||
        util::isInvalidValue(m_TradingAccount.DeliveryMargin)||
        util::isInvalidValue(m_TradingAccount.Credit)||
        util::isInvalidValue(m_TradingAccount.PositionProfit))
    {
        m_TradingAccount.Available = util::GetDoubleInvalidValue();
    }
    else
    {
	    m_TradingAccount.Available =    
            m_TradingAccount.DynamicProfit 
            - m_TradingAccount.CurrMargin 
            - m_TradingAccount.FrozenMargin 
            - m_TradingAccount.FrozenCommission 
            - m_TradingAccount.DeliveryMargin 
            + m_TradingAccount.Credit;
        if(m_TradingAccount.PositionProfit>0)
            m_TradingAccount.Available-=m_TradingAccount.PositionProfit;
    }
}
//�����ʽ������еķ��ն�ֵ
void CPlatformDataMgr_TradingAccount::CalcuRiskDegree_Internal()
{
    if(util::isInvalidValue(m_TradingAccount.DynamicProfit))
        m_TradingAccount.RiskDegree=util::GetDoubleInvalidValue();
	else if ( util::equals( m_TradingAccount.DynamicProfit, 0) )
	{
		m_TradingAccount.RiskDegree = 0;
	}
	else if(!util::isInvalidValue(m_TradingAccount.CurrMargin)&&
            !util::isInvalidValue(m_TradingAccount.DeliveryMargin))
	{
		m_TradingAccount.RiskDegree = 
            ( m_TradingAccount.CurrMargin + m_TradingAccount.DeliveryMargin ) * 100
            /m_TradingAccount.DynamicProfit;
	}
    else m_TradingAccount.RiskDegree=util::GetDoubleInvalidValue(); 
}
