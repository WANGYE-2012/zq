#include "stdafx.h"

#include "wx/file.h"
#include "wx/textfile.h"

#include <vector>
#include <string>
#include <map>

#include "ServiceProxy.hpp"
#include "PlatFormService.hpp"
#include "PlatformDataMgr.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "../inc/Module-Misc/tools_util.h"
#include "../inc/Module-Misc/orderCommonFunc.h"
#include "../inc/Module-Misc/GlobalFunc.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#pragma warning(disable:4996)

#if 1
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CPlatformDataMgr", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif

extern PlatformSvrMgr* g_pPlatformMgr;


CPlatformDataMgr::CPlatformDataMgr(CPlatFormService* pHost):
m_PlatFormService(pHost)
{
    InitializeCriticalSection(&m_CS);
    InitializeCriticalSection(&m_CS_InstrumentInfo);
    InitializeCriticalSection(&m_CS_Quot);

    m_pWriteLog=new zqWriteLog(LOGMODE_LOCALFILE,"PlatformDataMgr.log");

    m_TradingAccount.Head.BID=BID_MAX;//���m_TradingAccount��Ч
    m_bNeedCheckCommissionRateAndMarginRate=false;

    m_InvestorInfo.Head.BID=BID_MAX;  //���m_InvestorInfo��Ч
    m_bSettlementInfoIsReady=false;
    m_bHavePostionDetail=false;
    m_ModuleDataQry=true;
	m_combinLegs.clear();
	m_tempOriPositionDetail.clear();
	m_tempTradeCombinLegs.clear();
	m_reqMarginRateError.clear();
	m_reqCommissionRateError.clear();

    //��ѯ�������ʱ���Ƿ���Ҫ����mapQryRlt. ���ڳֲ֣���Ҫ���������ںϲ����ճֲֺ���ʷ�ֲ�
    m_PositionData.SetbNeedsetKeyOfQryRlt(true);
}

CPlatformDataMgr::~CPlatformDataMgr(void)
{
    LOG_INFO("CPlatformDataMgr::~CPlatformDataMgr(����CPlatformDataMgr����)");
    if(m_pWriteLog)
    {
        delete m_pWriteLog;
        m_pWriteLog=NULL;
    }


    DeleteCriticalSection(&m_CS_Quot);
    DeleteCriticalSection(&m_CS_InstrumentInfo);
    DeleteCriticalSection(&m_CS);
}


///���ý����ʽ��˻���Ϣ
void CPlatformDataMgr::SetTradingAccount(const PlatformStru_TradingAccountInfo& inData)
{
    LOG_INFO("CPlatformDataMgr::SetTradingAccount1");

    Lock_CPlatformDataMgr;

    //SetAccountField(m_TradingAccount,inData.Thost );
    memcpy( &m_TradingAccount.TradingAccountField, &inData, sizeof(PlatformStru_TradingAccountInfo));

    Unlock_CPlatformDataMgr;


    LOG_INFO("CPlatformDataMgr::SetTradingAccount1-end");

}
void CPlatformDataMgr::SetAccountField(DataRspQryTradingAccountEx& accex, const CThostFtdcTradingAccountFieldEx& field )
{
	//SetAccountField(accex,*((CThostFtdcTradingAccountField*)&field));
}
void CPlatformDataMgr::SetAccountField(DataRspQryTradingAccount& field )
{
	PlatformStru_TradingAccountInfo& TradingAccountField=field.TradingAccountField;

	//if(PLATFORM_DEFAULT==PTYPE_CTP)
	{
		TradingAccountField.StaticProfit = TradingAccountField.PreBalance
			- TradingAccountField.PreCredit - TradingAccountField.PreMortgage
			+ TradingAccountField.Mortgage - TradingAccountField.Withdraw 
			+ TradingAccountField.Deposit;
        RefreshDynamicProficOfTradingAccount(TradingAccountField);
        RefreshAvailOfTradingAccount(TradingAccountField);
		if(util::isInvalidValue(TradingAccountField.FrozenMargin)
			||util::isInvalidValue(TradingAccountField.FrozenCommission))
		{
			TradingAccountField.FrozenCash=util::GetDoubleInvalidValue();

		}
		else
		{
			TradingAccountField.FrozenCash = TradingAccountField.FrozenMargin 
				+ TradingAccountField.FrozenCommission;
		}
        RefreshRiskDegreeOfTradingAccount(TradingAccountField);
	}
	//else
		TradingAccountField.DynamicProfit = TradingAccountField.CashIn;

}

//�����ʽ������еĶ�̬Ȩ��ֵ
void CPlatformDataMgr::RefreshDynamicProficOfTradingAccount(PlatformStru_TradingAccountInfo& TradingAccountField)
{
    if(util::isInvalidValue(TradingAccountField.StaticProfit)||
        util::isInvalidValue(TradingAccountField.CloseProfit)||
        util::isInvalidValue(TradingAccountField.PositionProfit)||
        util::isInvalidValue(TradingAccountField.Commission))
    {
        TradingAccountField.DynamicProfit=util::GetDoubleInvalidValue();
    }
    else
    {
		TradingAccountField.DynamicProfit =  TradingAccountField.StaticProfit
		                                                    + TradingAccountField.CloseProfit + 
                                                            + TradingAccountField.PositionProfit
		                                                    - TradingAccountField.Commission;
    }        
}

//�����ʽ������еķ��ն�ֵ
void CPlatformDataMgr::RefreshRiskDegreeOfTradingAccount(PlatformStru_TradingAccountInfo& TradingAccountField)
{
    if(util::isInvalidValue(TradingAccountField.DynamicProfit))
        TradingAccountField.RiskDegree=util::GetDoubleInvalidValue();
	else if ( util::equals( TradingAccountField.DynamicProfit, 0) )
	{
		TradingAccountField.RiskDegree = 0;
	}
	else if(!util::isInvalidValue(TradingAccountField.CurrMargin)&&
            !util::isInvalidValue(TradingAccountField.DeliveryMargin))
	{
		TradingAccountField.RiskDegree = ( TradingAccountField.CurrMargin + 
                                                            TradingAccountField.DeliveryMargin )
			                                             *100
                                                         /TradingAccountField.DynamicProfit;
	}
    else TradingAccountField.RiskDegree=util::GetDoubleInvalidValue(); 
}

//�����ʽ������еĿ����ʽ�
void CPlatformDataMgr::RefreshAvailOfTradingAccount(PlatformStru_TradingAccountInfo& TradingAccountField)
{
    if( util::isInvalidValue(TradingAccountField.DynamicProfit)||
        util::isInvalidValue(TradingAccountField.CurrMargin)||
        util::isInvalidValue(TradingAccountField.FrozenMargin)||
        util::isInvalidValue(TradingAccountField.FrozenCommission)||
        util::isInvalidValue(TradingAccountField.DeliveryMargin)||
        util::isInvalidValue(TradingAccountField.Credit)||
        util::isInvalidValue(TradingAccountField.PositionProfit))
    {
        TradingAccountField.Available = util::GetDoubleInvalidValue();
    }
    else
    {
	    TradingAccountField.Available =    TradingAccountField.DynamicProfit 
                                                          - TradingAccountField.CurrMargin 
                                                          - TradingAccountField.FrozenMargin 
		                                                  - TradingAccountField.FrozenCommission 
                                                          - TradingAccountField.DeliveryMargin 
                                                          + TradingAccountField.Credit;
        if(TradingAccountField.PositionProfit>0)
            TradingAccountField.Available-=TradingAccountField.PositionProfit;
    }
}

///���ý����ʽ��˻���Ϣ
void CPlatformDataMgr::SetTradingAccount(const DataRspQryTradingAccount& outData)
{
    LOG_INFO("CPlatformDataMgr::SetTradingAccount2");

    Lock_CPlatformDataMgr;

    m_TradingAccount.Head = outData.Head;
	m_TradingAccount.nRequestID = outData.nRequestID;
	m_TradingAccount.bIsLast = outData.bIsLast;
	memcpy( &m_TradingAccount, &outData, sizeof(DataRspQryTradingAccount));
	m_TradingAccount.RspInfoField = outData.RspInfoField;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::SetTradingAccount2-end");
}
int CPlatformDataMgr::GetTradingAccountAvailable(double& fAvailable)
{
    int ret;

    Lock_CPlatformDataMgr;

    if(m_TradingAccount.Head.BID==BID_MAX)
    {
        ret= -1;
    }
    else
    {
        fAvailable=m_TradingAccount.TradingAccountField.Available;
        ret=0;
    }

    Unlock_CPlatformDataMgr;

    return ret;   
}
int CPlatformDataMgr::GetTradingAccountWithdrawQuota(double& fWithdrawQuota)
{
    int ret;

    Lock_CPlatformDataMgr;

    if(m_TradingAccount.Head.BID==BID_MAX)
    {
        ret= -1;
    }
    else
    {
        fWithdrawQuota=m_TradingAccount.TradingAccountField.WithdrawQuota;
        ret=0;
    }

    Unlock_CPlatformDataMgr;

    return ret;   
}
int CPlatformDataMgr::GetTradingAccountID(char* AccountID)
{
    int ret;

    Lock_CPlatformDataMgr;

    if(m_TradingAccount.Head.BID==BID_MAX)
    {
        ret= -1;
    }
    else
    {
		strcpy(AccountID,m_TradingAccount.TradingAccountField.AccountID);
        ret=0;
    }

    Unlock_CPlatformDataMgr;

    return ret;   
}
///��ȡ�����ʽ��˻���Ϣ
int CPlatformDataMgr::GetTradingAccount(PlatformStru_TradingAccountInfo& outData)
{
    int ret;

    LOG_INFO("CPlatformDataMgr::GetTradingAccount");

    Lock_CPlatformDataMgr;

    if(m_TradingAccount.Head.BID==BID_MAX)
    {
        memset(&outData,0,sizeof(outData));
        ret= -1;
    }
    else
    {
		SetAccountField(m_TradingAccount);
		memcpy(&outData,&m_TradingAccount.TradingAccountField,sizeof(PlatformStru_TradingAccountInfo));
        ret=0;
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::GetTradingAccount-end");
    return ret;   
}

///��ȡ�˻��ʽ��ı���Ϣ
int CPlatformDataMgr::GetAccountText(std::string& outData,int language)
{
    int ret=0;

    LOG_INFO("CPlatformDataMgr::GetAccountText");

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

		_snprintf(temp,FMT_BUFFERSIZE,"  �ϴν���׼����:                         %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.PreBalance));
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- �ϴ����ö��                             %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.PreCredit));
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- �ϴ���Ѻ���:                            %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.PreMortgage));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ ��Ѻ���:                                 %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Mortgage));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- ���ճ���:                                  %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Withdraw));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ �������:                                 %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Deposit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;

		_snprintf(temp,FMT_BUFFERSIZE,"= ���վ�̬Ȩ��:                           %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.StaticProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ ƽ��ӯ��:                                 %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.CloseProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		if(m_TradingAccount.TradingAccountField.PositionProfit>0.0)
			_snprintf(temp,FMT_BUFFERSIZE,"+ �ֲ�ӯ��:                                 %s(����������ʽ�)\r\n",AC_FM(m_TradingAccount.TradingAccountField.PositionProfit));
		else
			_snprintf(temp,FMT_BUFFERSIZE,"+ �ֲ�ӯ��:                                 %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.PositionProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- ������:                                     %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Commission));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;

		_snprintf(temp,FMT_BUFFERSIZE,"= ��̬Ȩ��:                                %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.DynamicProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- ռ�ñ�֤��:                               %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.CurrMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- ���ᱣ֤��:                               %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.FrozenMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- ����������:                               %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.FrozenCommission));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- ���֤��:                               %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.DeliveryMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ ���ý��                                  %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Credit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;
		_snprintf(temp,FMT_BUFFERSIZE,"= �����ʽ�:                                %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Available));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		outData+=strSeparator;
		outData+="\r\n\r\n";
		outData+=strSeparator;
		_snprintf(temp,FMT_BUFFERSIZE,"= �����ʽ�:                                 %.2f\r\n",0.0);
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"= ��ȡ�ʽ�:                                 %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.WithdrawQuota));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		outData+=strSeparator;
	}
	if(language==LANGUAGE_ENGLISH)
	{
		_snprintf(temp,FMT_BUFFERSIZE,"  PreBalance:                                        %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.PreBalance));
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- PreCredit                                            %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.PreCredit));
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- PreMortgage:                                      %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.PreMortgage));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Mortgage:                                         %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Mortgage));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Today Withdraw:                                 %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Withdraw));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Today Deposit:                                   %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Deposit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;

		_snprintf(temp,FMT_BUFFERSIZE,"= Today Static Profit:                             %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.StaticProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Close Profit:                                        %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.CloseProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Position Profit:                                     %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.PositionProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Commission:                                         %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Commission));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;

		_snprintf(temp,FMT_BUFFERSIZE,"= Dynamic Profit:                                   %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.DynamicProfit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Current Margin:                                    %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.CurrMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Frozen Margin:                                     %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.FrozenMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Frozen Commission:                               %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.FrozenCommission));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"- Delivery Margin:                                    %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.DeliveryMargin));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"+ Credit                                                %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Credit));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;

		outData+=strSeparator2;
		_snprintf(temp,FMT_BUFFERSIZE,"= Available:                                           %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.Available));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		outData+=strSeparator;
		outData+="\r\n\r\n";
		outData+=strSeparator;
		_snprintf(temp,FMT_BUFFERSIZE,"= Minimum Quota:                                  %.2f\r\n",0.0);
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		_snprintf(temp,FMT_BUFFERSIZE,"= Withdraw Quota:                                %s\r\n",AC_FM(m_TradingAccount.TradingAccountField.WithdrawQuota));
		strTemp.replace(0,strlen(temp),temp);
		outData+=temp;
		outData+=strSeparator;
	}

    Unlock_CPlatformDataMgr;

	LOG_INFO("CPlatformDataMgr::GetAccountText-end");
	return ret;   
}

///��̬�����ʽ��˻��еĶ����ʽ𣬲�ѯ�������ء���ѯ���ʷ���
void CPlatformDataMgr::DynamicUpdateTradingAccount_Freeze()
{
    Lock_CPlatformDataMgr;

    m_TradingAccount.TradingAccountField.FrozenMargin=0;
    m_TradingAccount.TradingAccountField.FrozenCommission=0;

    std::map<OrderKey,  PlatformStru_OrderInfo*>::iterator iter;
    std::string strInstrumentID;

    for(iter = m_OrderData.m_all.begin();iter != m_OrderData.m_all.end(); iter++)
    {
        if(iter->second!=NULL&&
            (iter->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
             iter->second->OrderStatus==THOST_FTDC_OST_NoTradeQueueing))
        {
            if(iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_Open)
            {
                if(!util::isInvalidValue(iter->second->freezeMargin)&&iter->second->freezeMargin>0)
                    m_TradingAccount.TradingAccountField.FrozenMargin+=iter->second->freezeMargin;
            }

            if(!util::isInvalidValue(iter->second->troubleMoney)&&iter->second->troubleMoney>0)
                m_TradingAccount.TradingAccountField.FrozenCommission+=iter->second->troubleMoney;
        }
    }

    //�����ʽ������еĿ����ʽ�
    RefreshAvailOfTradingAccount(m_TradingAccount.TradingAccountField);

    Unlock_CPlatformDataMgr;

}

///��̬�����ʽ��˻��еĶ����ʽ𣬱����ر�ʱ����
void CPlatformDataMgr::DynamicUpdateTradingAccount_Freeze_RtnOrder(const PlatformStru_OrderInfo& Order, 
                                                                    const PlatformStru_OrderInfo& OldOrderInfo)
{
    double DeltaValue;

    Lock_CPlatformDataMgr;

    DeltaValue=util::GetDoubleInvalidValue();
    if(OldOrderInfo.IsInvalidRecord()&&!Order.IsInvalidRecord())
        DeltaValue=Order.freezeMargin;
    else if(!OldOrderInfo.IsInvalidRecord()&&!Order.IsInvalidRecord())
    {
        if(Order.freezeMargin!=util::GetDoubleInvalidValue()&&
            OldOrderInfo.freezeMargin!=util::GetDoubleInvalidValue())
            DeltaValue=Order.freezeMargin-OldOrderInfo.freezeMargin;
    }
    if(DeltaValue==util::GetDoubleInvalidValue()&&
        m_TradingAccount.TradingAccountField.FrozenMargin!=util::GetDoubleInvalidValue())
        m_TradingAccount.TradingAccountField.FrozenMargin=util::GetDoubleInvalidValue();
    else m_TradingAccount.TradingAccountField.FrozenMargin += DeltaValue;

    DeltaValue=util::GetDoubleInvalidValue();
    if(OldOrderInfo.IsInvalidRecord()&&!Order.IsInvalidRecord())
        DeltaValue=Order.troubleMoney;
    else if(!OldOrderInfo.IsInvalidRecord()&&!Order.IsInvalidRecord())
    {
        if(Order.troubleMoney!=util::GetDoubleInvalidValue()&&
            OldOrderInfo.troubleMoney!=util::GetDoubleInvalidValue())
            DeltaValue=Order.troubleMoney-OldOrderInfo.troubleMoney;
    }
    if(DeltaValue==util::GetDoubleInvalidValue()&&
        m_TradingAccount.TradingAccountField.FrozenCommission!=util::GetDoubleInvalidValue())
        m_TradingAccount.TradingAccountField.FrozenCommission=util::GetDoubleInvalidValue();
    else m_TradingAccount.TradingAccountField.FrozenCommission += DeltaValue;

    //�����ʽ������еĿ����ʽ�
    RefreshAvailOfTradingAccount(m_TradingAccount.TradingAccountField);

    Unlock_CPlatformDataMgr;

}

///��̬�����ʽ��˻��еĽ���Ӷ�𣬲�ѯ�ɽ����ء��ɽ��ر�����ѯ���ʷ���
void CPlatformDataMgr::DynamicUpdateTradingAccount_TradeCommission_Internal()
{
    LOG_INFO("CPlatformDataMgr::DynamicUpdateTradingAccount_TradeCommission_Internal");

    m_TradingAccount.TradingAccountField.Commission=0;

    std::map<TradeKey,  PlatformStru_TradeInfo*>::iterator iter;
    std::string strInstrumentID;

    for(iter = m_TradeData.m_all.begin();iter != m_TradeData.m_all.end(); iter++)
    {
        if(iter->second!=NULL&&!util::isInvalidValue(iter->second->TradeCommission))
            m_TradingAccount.TradingAccountField.Commission+=iter->second->TradeCommission;
        //else
        //{
        //    m_TradingAccount.TradingAccountField.Commission=util::GetDoubleInvalidValue();
        //    break;
        //}
    }

    LOG_INFO("CPlatformDataMgr::DynamicUpdateTradingAccount_TradeCommission_Internal-end");
}



///�������ʱ����̬�����ʽ��˻���Ϣ
void CPlatformDataMgr::DynamicUpdateTradingAccount_RtnDepthMarketData(const std::string InstrumentID)
{
    Lock_CPlatformDataMgr;


    //����гֲ���ϸ���Ž��и���
    if(m_PositionDetailData.HaveDataOfInstrumentID(InstrumentID))
    {
        GetAllPositionProfitByDate_Internal( m_TradingAccount.TradingAccountField.PositionProfit);

        //�����ʽ������еĶ�̬Ȩ��ֵ
        RefreshDynamicProficOfTradingAccount(m_TradingAccount.TradingAccountField);
        //�����ʽ������еķ��ն�ֵ
        RefreshRiskDegreeOfTradingAccount(m_TradingAccount.TradingAccountField);
        //�����ʽ������еĿ����ʽ�
        RefreshAvailOfTradingAccount(m_TradingAccount.TradingAccountField);
    }

    Unlock_CPlatformDataMgr;

}


///�����ر�ʱ����̬�����ʽ��˻���Ϣ
void CPlatformDataMgr::DynamicUpdateTradingAccount_RtnOrder(const PlatformStru_OrderInfo& Order, 
                                                            const PlatformStru_OrderInfo& OldOrderInfo)
{
    Lock_CPlatformDataMgr;

    DynamicUpdateTradingAccount_Freeze_RtnOrder(Order,OldOrderInfo);

    Unlock_CPlatformDataMgr;

}

///�ɽ��ر�ʱ����̬�����ʽ��˻���Ϣ
void CPlatformDataMgr::DynamicUpdateTradingAccount_RtnTrade(void)
{
    Lock_CPlatformDataMgr;

    GetAllPositionProfitByDate_Internal( m_TradingAccount.TradingAccountField.PositionProfit);
    GetAllMargin_CloseProfitByDate_Internal(m_TradingAccount.TradingAccountField.CurrMargin,m_TradingAccount.TradingAccountField.CloseProfit);

    DynamicUpdateTradingAccount_TradeCommission_Internal();

    //�����ʽ������еĶ�̬Ȩ��ֵ
    RefreshDynamicProficOfTradingAccount(m_TradingAccount.TradingAccountField);
    //�����ʽ������еķ��ն�ֵ
    RefreshRiskDegreeOfTradingAccount(m_TradingAccount.TradingAccountField);
    //�����ʽ������еĿ����ʽ�
    RefreshAvailOfTradingAccount(m_TradingAccount.TradingAccountField);

    Unlock_CPlatformDataMgr;

}


///��ѯ�ֲ���Ӧʱ����̬�����ʽ��˻���Ϣ
void CPlatformDataMgr::DynamicUpdateTradingAccount_RspQryInvestorPositionLast(void)
{
    Lock_CPlatformDataMgr;

    GetAllPositionProfitByDate_Internal( m_TradingAccount.TradingAccountField.PositionProfit);
    GetAllMargin_CloseProfitByDate_Internal(m_TradingAccount.TradingAccountField.CurrMargin,m_TradingAccount.TradingAccountField.CloseProfit);

    DynamicUpdateTradingAccount_TradeCommission_Internal();

    //�����ʽ������еĶ�̬Ȩ��ֵ
    RefreshDynamicProficOfTradingAccount(m_TradingAccount.TradingAccountField);
    //�����ʽ������еķ��ն�ֵ
    RefreshRiskDegreeOfTradingAccount(m_TradingAccount.TradingAccountField);
    //�����ʽ������еĿ����ʽ�
    RefreshAvailOfTradingAccount(m_TradingAccount.TradingAccountField);

    Unlock_CPlatformDataMgr;

}


///��ѯ�ɽ���Ӧʱ����̬�����ʽ��˻���Ϣ
void CPlatformDataMgr::DynamicUpdateTradingAccount_RspQryTrade(void)
{
    Lock_CPlatformDataMgr;

    GetAllPositionProfitByDate_Internal( m_TradingAccount.TradingAccountField.PositionProfit);
    GetAllMargin_CloseProfitByDate_Internal(m_TradingAccount.TradingAccountField.CurrMargin,m_TradingAccount.TradingAccountField.CloseProfit);

    DynamicUpdateTradingAccount_TradeCommission_Internal();

    //�����ʽ������еĶ�̬Ȩ��ֵ
    RefreshDynamicProficOfTradingAccount(m_TradingAccount.TradingAccountField);
    //�����ʽ������еķ��ն�ֵ
    RefreshRiskDegreeOfTradingAccount(m_TradingAccount.TradingAccountField);
    //�����ʽ������еĿ����ʽ�
    RefreshAvailOfTradingAccount(m_TradingAccount.TradingAccountField);

    Unlock_CPlatformDataMgr;

}

///��ѯ������Ӧʱ����̬�����ʽ��˻���Ϣ
void CPlatformDataMgr::DynamicUpdateTradingAccount_RspQryOrder(void)
{
    DynamicUpdateTradingAccount_Freeze();
}


///��ѯ��֤������Ӧʱ����̬�����ʽ��˻���Ϣ
void CPlatformDataMgr::DynamicUpdateTradingAccount_RspQryRate(void)
{
    Lock_CPlatformDataMgr;

    //�����ʽ������еĶ�̬Ȩ��ֵ(��ѯ��֤������Ӧʱ����Ҫ���¶�̬Ȩ��)
    //RefreshDynamicProficOfTradingAccount(m_TradingAccount.TradingAccountField);
    //�����ʽ������еķ��ն�ֵ
    RefreshRiskDegreeOfTradingAccount(m_TradingAccount.TradingAccountField);
    //�����ʽ������еĿ����ʽ�
    RefreshAvailOfTradingAccount(m_TradingAccount.TradingAccountField);

    Unlock_CPlatformDataMgr;

}

///��ѯ����������Ӧʱ����̬�����ʽ��˻���Ϣ
void CPlatformDataMgr::DynamicUpdateTradingAccount_RspQryInstrumentCommissionRate(void)
{
    Lock_CPlatformDataMgr;

    DynamicUpdateTradingAccount_TradeCommission_Internal();

    //�����ʽ������еĶ�̬Ȩ��ֵ
    RefreshDynamicProficOfTradingAccount(m_TradingAccount.TradingAccountField);
    //�����ʽ������еķ��ն�ֵ
    RefreshRiskDegreeOfTradingAccount(m_TradingAccount.TradingAccountField);
    //�����ʽ������еĿ����ʽ�
    RefreshAvailOfTradingAccount(m_TradingAccount.TradingAccountField);

    Unlock_CPlatformDataMgr;

}


const string CPlatformDataMgr::Instrument2ExchangeID(const string& Instrument)
{
	//ֻ�ʺϲ��Է�����
	const string SHFE="SHFE";
	const string CZCE="CZCE";
	const string DCE="DCE";
	const string CFFEX="CFFEX";
	size_t len= Instrument.length();
	if(len==6)
	{
//		string produceID=Instrument.substr(0,2);
//		transform( produceID.begin(), produceID.end(), produceID.begin(), toupper);
//		if(produceID.compare("IF")==0)
        char ch1=Instrument.at(0);
        char ch2=Instrument.at(1);
        if(isupper(ch1)&&isupper(ch2))  //like IFxxxx��TFxxxx, which is instrument of cffex
			return CFFEX;
		else
			return SHFE;
	}
	else if(len==5)
	{
		char ch=Instrument.at(1);
		if(ch>='0'&&ch<='9')
			return DCE;
		else
			return CZCE;
	}
	else
		return "";
}


///����ָ����Լ��Ϣ
void CPlatformDataMgr::SetInstrumentInfo(const std::string& InstrumentID, const PlatformStru_InstrumentInfo& inData)
{
	LOG_INFO("CPlatformDataMgr::SetInstrumentInfo");

    PlatformStru_InstrumentInfo  newData;
	memcpy(&newData,&inData,sizeof(PlatformStru_InstrumentInfo));

    //m_ServerParamCfg����Ҫ����(from ouyang)
    if(m_ServerParamCfg.vec[m_ServerParamCfg.current].CTPSimulate==1)
	{
		string strNewID=Instrument2ExchangeID(newData.InstrumentID);
		if(!strNewID.empty())
			strcpy(newData.ExchangeID,strNewID.c_str());
	}

    EnterCriticalSection(&m_CS_InstrumentInfo);

    m_allInstruments[InstrumentID] = newData;
    m_allInstrumentList.insert(InstrumentID);
    if(inData.ProductClass!=THOST_FTDC_PC_Combination)
        m_allInstrumentListWithoutComb.insert(InstrumentID);
    m_allProductID[inData.ProductID].insert(InstrumentID);

    LeaveCriticalSection(&m_CS_InstrumentInfo);

    //���Ҳ����Ҫ����
    m_bNeedCheckCommissionRateAndMarginRate=true;


    LOG_INFO("CPlatformDataMgr::SetInstrumentInfo-end");
}
///��ȡָ����Լ��Ϣ
int CPlatformDataMgr::GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    LOG_INFO("CPlatformDataMgr::GetInstrumentInfo");
    int ret;
    std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it;

    EnterCriticalSection(&m_CS_InstrumentInfo);

    it =  m_allInstruments.find(InstrumentID);
    if(it==m_allInstruments.end())
    {
        memset(&outData,0,sizeof(outData));
        ret=-1;
    }
    else 
    {
        outData = it->second;
        ret=0;
    }
    LeaveCriticalSection(&m_CS_InstrumentInfo);

    LOG_INFO("CPlatformDataMgr::GetInstrumentInfo-end");
    return ret;
}

///ָ����Լ�Ƿ�Ϸ�(����)
bool CPlatformDataMgr::InstrumentIsValid(const std::string& InstrumentID)
{
    LOG_INFO("CPlatformDataMgr::InstrumentIsValid");
    bool ret;
    std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it;

    EnterCriticalSection(&m_CS_InstrumentInfo);

    it =  m_allInstruments.find(InstrumentID);
    if(it==m_allInstruments.end()) ret=false;
    else ret=true;

    LeaveCriticalSection(&m_CS_InstrumentInfo);

    LOG_INFO("CPlatformDataMgr::InstrumentIsValid-end");
    return ret;
}


///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
char CPlatformDataMgr::GetProductClassType(const std::string& InstrumentID)
{
    //LOG_INFO("CPlatformDataMgr::GetProductClassType");
    char ProductClassType;
    std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it;

    EnterCriticalSection(&m_CS_InstrumentInfo);

    it =  m_allInstruments.find(InstrumentID);
    if(it==m_allInstruments.end())
        ProductClassType=-1;
    else 
        ProductClassType = it->second.ProductClass;

    LeaveCriticalSection(&m_CS_InstrumentInfo);

    //LOG_INFO("CPlatformDataMgr::GetProductClassType-end");
    return ProductClassType;
}
///��ȡ��Լ�б�
int CPlatformDataMgr::GetInstrumentList(std::vector<GroupInfo> &outData)
{
	outData.clear();
    LOG_INFO("CPlatformDataMgr::GetInstrumentList");

    std::map<std::string,PlatformStru_InstrumentInfo> allInstruments; //��ǰ���к�Լ��Ϣ
    std::map<std::string,PlatformStru_InstrumentInfo>::iterator it_all;

    EnterCriticalSection(&m_CS_InstrumentInfo);

    allInstruments=m_allInstruments;

    LeaveCriticalSection(&m_CS_InstrumentInfo);

    Lock_CPlatformDataMgr;

	std::map<std::string,int> GroupName2ID;
	std::map<std::string,int>::iterator GroupName2ID_it;
    for(it_all=allInstruments.begin();it_all!=allInstruments.end();it_all++)
	{
#ifndef _USE_COMBOINSTRUMENT
        if(it_all->second.ProductClass==THOST_FTDC_PC_Combination)
            continue;
#endif
        GroupName2ID_it=GroupName2ID.find(it_all->second.ExchangeID);
		if(GroupName2ID_it==GroupName2ID.end())
		{
			GroupInfo ginfo;
            ginfo.GroupName=it_all->second.ExchangeID;
			ginfo.InstrVec.push_back(InstrumentInfo(it_all->second.InstrumentID,it_all->second.InstrumentName));
			outData.push_back(ginfo);
			GroupName2ID[it_all->second.ExchangeID]=outData.size()-1;
		}
		else
		{
			outData[GroupName2ID_it->second].InstrVec.push_back(InstrumentInfo(it_all->second.InstrumentID,it_all->second.InstrumentName));
		}
	}

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::GetInstrumentList-end");
	return 0;
}
///��ȡ��Լ�б�
int CPlatformDataMgr::GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID)
{
    LOG_INFO("CPlatformDataMgr::GetInstrumentList");

    outData.clear();

    EnterCriticalSection(&m_CS_InstrumentInfo);

    if(ExchangeID.empty())
    {
#ifndef _USE_COMBOINSTRUMENT
		outData = m_allInstrumentListWithoutComb;
#else
        outData = m_allInstrumentList;
#endif
    }
	else
	{
		outData.clear();
        std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it2;
        for(it2=m_allInstruments.begin();it2!=m_allInstruments.end();it2++)
        {
#ifndef _USE_COMBOINSTRUMENT
            if(it2->second.ProductClass==THOST_FTDC_PC_Combination)
                continue;
#endif
            if(std::string(it2->second.ExchangeID)==ExchangeID)
                outData.insert(it2->first);
        }
	}
    LeaveCriticalSection(&m_CS_InstrumentInfo);

    LOG_INFO("CPlatformDataMgr::GetInstrumentList-end");
    return outData.size();
}
//���������Լ�б�
int CPlatformDataMgr::AddMainInstrumentList(std::string instrument)
{
    Lock_CPlatformDataMgr;

    m_allMainInstrumentList.insert(instrument);

    Unlock_CPlatformDataMgr;

	return m_allMainInstrumentList.size();
}
///��ȡ��Լ�б�
int CPlatformDataMgr::GetMainInstrumentList(std::set<std::string> &outData)
{
    LOG_INFO("CPlatformDataMgr::GetMainInstrumentList");

    outData.clear();


#ifdef _NEED_MAIN_INSTRUMENT
        Lock_CPlatformDataMgr;
		outData = m_allMainInstrumentList;
        Unlock_CPlatformDataMgr;
#else
        EnterCriticalSection(&m_CS_InstrumentInfo);
#ifndef _USE_COMBOINSTRUMENT
		outData = m_allInstrumentListWithoutComb;
#else
        outData = m_allInstrumentList;
#endif
        LeaveCriticalSection(&m_CS_InstrumentInfo);
#endif	


    LOG_INFO("CPlatformDataMgr::GetMainInstrumentList-end");
    return outData.size();
}
///���ú�Լ�ݲ��б�
void CPlatformDataMgr::SetInstrumentVarietyMap(map<string, string>& inData)
{
    LOG_INFO("CPlatformDataMgr::SetInstrumentVarietyMap");

    Lock_CPlatformDataMgr;

    m_allInstrumentVarietyMap = inData;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::SetInstrumentVarietyMap-end");
}

// ��ȡ��Լ�ݲ��б�
int CPlatformDataMgr::GetInstrumentVarietyMap(map<string, string>& outData)
{
    LOG_INFO("CPlatformDataMgr::SetInstrumentVarietyMap");

    Lock_CPlatformDataMgr;

    outData = m_allInstrumentVarietyMap;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::SetInstrumentVarietyMap-end");
    return m_allInstrumentVarietyMap.size();
}

///��ȡָ��Ʒ�ֵĺ�Լ�б�,���غ�Լ����
int CPlatformDataMgr::GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData)
{
    LOG_INFO("CPlatformDataMgr::GetInstrumentListByProductID");
    int ret;
    std::map<std::string, std::set<std::string> >::iterator it;

    outData.clear();

    EnterCriticalSection(&m_CS_InstrumentInfo);

    it=m_allProductID.find(ProductID);
    if(it==m_allProductID.end()) 
        ret=-1;
    else
    {
        outData = it->second;
        ret=outData.size();
    }

    LeaveCriticalSection(&m_CS_InstrumentInfo);

    LOG_INFO("CPlatformDataMgr::GetInstrumentListByProductID-end");
    return ret;
}

///��ȡ��ԼƷ�ִ���
int CPlatformDataMgr::GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID)
{
    LOG_INFO("CPlatformDataMgr::GetProductID");

    std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it2; m_allInstruments;
    outData.clear();
    EnterCriticalSection(&m_CS_InstrumentInfo);

    outData=m_allProductID;
	if(!ExchangeID.empty())
	{
		std::map<std::string, std::set<std::string> >::iterator it;
		for(it=outData.begin();it!=outData.end();)
		{
			if(it->second.size()==0||
                (it2=m_allInstruments.find(*(it->second.begin())))==m_allInstruments.end()||
                std::string(it2->second.ExchangeID)!=ExchangeID)
			{
				it=outData.erase(it);
			}
			else
				++it;
		}
	}

    LeaveCriticalSection(&m_CS_InstrumentInfo);

    return outData.size(); 
    LOG_INFO("CPlatformDataMgr::GetProductID-end");
}


///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
int CPlatformDataMgr::GetInstrumentMultiple(const std::string& InstrumentID)
{
    LOG_INFO("CPlatformDataMgr::GetInstrumentMultiple");
    int Multiple;
    std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it;

    EnterCriticalSection(&m_CS_InstrumentInfo);

    it =  m_allInstruments.find(InstrumentID);
    if(it==m_allInstruments.end())
        Multiple=-1;
    else 
        Multiple= it->second.VolumeMultiple;

    LeaveCriticalSection(&m_CS_InstrumentInfo);

    LOG_INFO("CPlatformDataMgr::GetInstrumentMultiple-end");
    return Multiple;
}

///��ȡ��Լ��������
int CPlatformDataMgr::GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    LOG_INFO("CPlatformDataMgr::GetCommissionRate");
    int ret;

    Lock_CPlatformDataMgr;

    ret=GetCommissionRate_Internal(InstrumentID,outData);

	if(m_reqCommissionRateError[InstrumentID]>3)	
		ret=CustomErrorID;
    Unlock_CPlatformDataMgr;
    LOG_INFO("CPlatformDataMgr::GetCommissionRate-end");
    return ret;
}

///��ȡ��Լ�������ʣ��ڲ����ã����ü���
int CPlatformDataMgr::GetCommissionRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    LOG_INFO("CPlatformDataMgr::GetCommissionRate_Internal");
    int ret;
    std::map<std::string, PlatformStru_InstrumentCommissionRate>::iterator it1;
    it1=m_InstrumentCommissionRate.find(InstrumentID);
    memset(&outData,0,sizeof(outData));
    if(it1==m_InstrumentCommissionRate.end())
    {
        ret=-1;
    }
    else
    {
        outData = it1->second;
        ret=0;
    }
    LOG_INFO("CPlatformDataMgr::GetCommissionRate_Internal-end");
    return ret;
}

///���ú�Լ��������
void CPlatformDataMgr::SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    LOG_INFO("CPlatformDataMgr::SetCommissionRate");

    Lock_CPlatformDataMgr;

    m_InstrumentCommissionRate[InstrumentID]=outData;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::SetCommissionRate-end");
}

///�������ڲ�ѯ�������ʵĺ�Լ,���ڲ�ѯ����ʱ��Ӧ
void CPlatformDataMgr::SetReqCommissionRateInstrumentID(const std::string& InstrumentID)
{
    LOG_INFO("CPlatformDataMgr::SetReqCommissionRateInstrumentID");

    Lock_CPlatformDataMgr;

    m_ReqCommissionRateInstrumentID=InstrumentID;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::SetReqCommissionRateInstrumentID-end");
}

///��ȡ���ڲ�ѯ�������ʵĺ�Լ
void CPlatformDataMgr::GetReqCommissionRateInstrumentID(std::string& outData)
{
    LOG_INFO("CPlatformDataMgr::GetReqCommissionRateInstrumentID");

    Lock_CPlatformDataMgr;

    outData=m_ReqCommissionRateInstrumentID;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::GetReqCommissionRateInstrumentID-end");
}

///��ȡ��Լ��֤����
int CPlatformDataMgr::GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    LOG_INFO("CPlatformDataMgr::GetMarginRate");
    int ret;

    Lock_CPlatformDataMgr;

    ret=GetMarginRate_Internal(InstrumentID,outData);

	if(m_reqMarginRateError[InstrumentID]>3)	
		ret=CustomErrorID;

	Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::GetMarginRate-end");
    return ret; 
}

///��ȡ��Լ��֤���ʣ��ڲ����ã����ü���
int CPlatformDataMgr::GetMarginRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    LOG_INFO("CPlatformDataMgr::GetMarginRate_Internal");
    int ret;
    std::map<std::string, PlatformStru_InstrumentMarginRate>::iterator it;
    it=m_InstrumentMarginRate.find(InstrumentID);
    memset(&outData,0,sizeof(outData));
    if(it==m_InstrumentMarginRate.end())
    {
        ret=-1;
    }
    else
    {
        outData = it->second;
        ret=0;
    }
    LOG_INFO("CPlatformDataMgr::GetMarginRate_Internal-end");
    return ret; 
}

///���ú�Լ��֤����
void CPlatformDataMgr::SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    LOG_INFO("CPlatformDataMgr::SetMarginRate");

    Lock_CPlatformDataMgr;

    m_InstrumentMarginRate[InstrumentID]=outData;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::SetMarginRate-end");
}

//�ж�һ���ַ��������Ƿ��ǵ���
bool CPlatformDataMgr::IsTodayPosition(const char *OpenDate)
{
    int tradingday=0;

	if(OpenDate==NULL) return false;

	tradingday=atoi(m_exchangeTime.TradingDay);
    if(tradingday==0) 
    {
    	char OpenYear[5]={0}, OpenMonth[3]={0},OpenDay[3]={0};
		time_t tCurdate = time(NULL);
		struct tm local;
		localtime_s(&local, &tCurdate);
		tradingday= ((local.tm_year+1900)*10000+(local.tm_mon+1))*100+local.tm_mday;
    }
    return (tradingday==atoi(OpenDate));
}

bool CPlatformDataMgr::IsTodayPositionDetail(const PlatformStru_PositionDetail& PositionDetail)
{
    int tradingday=0;

	tradingday=atoi(m_exchangeTime.TradingDay);
    if(tradingday==0) 
    {
    	char OpenYear[5]={0}, OpenMonth[3]={0},OpenDay[3]={0};
		time_t tCurdate = time(NULL);
		struct tm local;
		localtime_s(&local, &tCurdate);
		tradingday= ((local.tm_year+1900)*10000+(local.tm_mon+1))*100+local.tm_mday;
    }
    return (tradingday==atoi(PositionDetail.OpenDate));
}


//ȡȫ���ֲֵ�ռ�ñ�֤���ƽ��ӯ��(���ն���)��
void CPlatformDataMgr::GetAllMargin_CloseProfitByDate_Internal(double& TotalMargin,double&TotalCloseProfitByDate)
{
	TotalMargin=0;
    TotalCloseProfitByDate=0;
	std::map<PositionKey,PlatformStru_Position*>::iterator iter;
    double InvalideValue=util::GetDoubleInvalidValue();

	iter = m_PositionData.m_all.begin();
	for(;iter != m_PositionData.m_all.end(); iter++)
	{
        if(TotalMargin!=InvalideValue)
        {
            if(iter->second!=NULL&&iter->second->UseMargin!=InvalideValue)
                TotalMargin+=iter->second->UseMargin;
            else TotalMargin=InvalideValue;
        }
        if(TotalCloseProfitByDate!=InvalideValue)
        {
            if(iter->second!=NULL&&iter->second->CloseProfit!=InvalideValue)
                TotalCloseProfitByDate+=iter->second->CloseProfit;
            else TotalCloseProfitByDate=InvalideValue;
        }
	}
}


//ȡȫ���ֲֵĳֲ�ӯ��(���ն���)
void CPlatformDataMgr::GetAllPositionProfitByDate_Internal(double&TotalPositionProfitByDate)
{
	TotalPositionProfitByDate=0;
	std::map<PositionKey,PlatformStru_Position*>::iterator iter;
    double InvalideValue=util::GetDoubleInvalidValue();

	iter = m_PositionData.m_all.begin();
	for(;iter != m_PositionData.m_all.end(); iter++)
	{
        if(TotalPositionProfitByDate!=InvalideValue)
        {
            if(iter->second!=NULL&&iter->second->PositionProfit!=InvalideValue)
                TotalPositionProfitByDate+=iter->second->PositionProfit;
            else TotalPositionProfitByDate=InvalideValue;
        }
	}
}

//ȡȫ���ֲֵ�ƽ��ӯ��(���ն���)
void CPlatformDataMgr::GetAllCloseProfitByDate_Internal(double&TotalCloseProfitByDate)
{
	TotalCloseProfitByDate=0;
	std::map<PositionKey,PlatformStru_Position*>::iterator iter;
    double InvalideValue=util::GetDoubleInvalidValue();

	iter = m_PositionData.m_all.begin();
	for(;iter != m_PositionData.m_all.end(); iter++)
	{
        if(TotalCloseProfitByDate!=InvalideValue)
        {
            if(iter->second!=NULL&&iter->second->CloseProfit!=InvalideValue)
                TotalCloseProfitByDate+=iter->second->CloseProfit;
            else TotalCloseProfitByDate=InvalideValue;
        }
	}
}

bool CompareTradeInfo(PlatformStru_TradeInfo& a,PlatformStru_TradeInfo& b)
{
    int i1,j1,len1,i2,j2,len2;

    for(i1=0;i1<sizeof(a.TradeID);i1++)
        if(a.TradeID[i1]!=' '&&a.TradeID[i1]!='0') break;
    for(j1=sizeof(a.TradeID)-1;j1>i1;j1--)
        if(a.TradeID[j1-1]!=' ') break;
    len1=j1-i1;

    for(i2=0;i2<sizeof(b.TradeID);i2++)
        if(b.TradeID[i2]!=' '&&b.TradeID[i2]!='0') break;
    for(j2=sizeof(b.TradeID)-1;j2>i2;j2--)
        if(b.TradeID[j2-1]!=' ') break;
    len2=j2-i2;

    if(len1<len2) return true;
    else if(len1>len2) return false;
    else return (memcmp(a.TradeID+i1,b.TradeID+i2,len1)<0?true:false);
}


//��ȡָ����Լ���飬�ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr::GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    int ret;

    EnterCriticalSection(&m_CS_Quot);

    std::map<std::string,  PlatformStru_DepthMarketData>::iterator it =  m_lastQuot.find(InstrumentID);
    if(it==m_lastQuot.end())
    {
        memset(&outData,0,sizeof(outData));
        ret=-1;
    }
    else
    {
        outData = it->second;
        ret=0;
    }

    LeaveCriticalSection(&m_CS_Quot);

    return ret;
}

//����ָ����Լ���飬���õ�������£��򷵻�true�����򷵻�false
//������Ϻ�Լ���������
//bFirstQuot��ʾ�Ƿ��Ǻ�Լ���״����飬�����������Ҫ���³ֲ���ϸ�ͳֲֵ�ռ�ñ�֤��
bool CPlatformDataMgr::SetQuotInfo(const PlatformStru_DepthMarketData& inData,bool& bFirstQuot)
{
    std::map<std::string,  PlatformStru_DepthMarketData>::iterator it = m_lastQuot.find(std::string(inData.InstrumentID));
    bool toBeContinue=true;

    bFirstQuot=false;


    EnterCriticalSection(&m_CS_Quot);

    if(it==m_lastQuot.end())
    {
        m_lastQuot[std::string(inData.InstrumentID)] = inData;
        bFirstQuot=true;
    }
    else
    {
        PlatformStru_DepthMarketData& OldBusiness= it->second;
        //�Ƚ������ʱ�䣬����������
        if (memcmp(inData.UpdateTime, OldBusiness.UpdateTime, sizeof(OldBusiness.UpdateTime))<0)
            toBeContinue=false;
        if(toBeContinue)
        {
            m_OldQuot[std::string(inData.InstrumentID)]=m_lastQuot[std::string(inData.InstrumentID)];
            m_lastQuot[std::string(inData.InstrumentID)] = inData;
        }
        else
        {
            LOG_INFO("CPlatformDataMgr::SetQuotInfo:Old BID_RtnDepthMarketData InstrumentID=[%s],OldUpdateTime=[%s],NewUpdateTime=[%s]",
                inData.InstrumentID,
                     OldBusiness.UpdateTime,
                     inData.UpdateTime);
        }
    }

    LeaveCriticalSection(&m_CS_Quot);

    return toBeContinue;

}


//���������Ⱥ�Լ�ϳ���Ϻ�Լ�����سɹ���񡣺ϳ�ʱ�����Լ�б����Ƿ��и���Ϻ�Լ
bool CPlatformDataMgr::GetComboInstrumentID_Internal(const std::string strInstrumentID1, 
                                                     const std::string strInstrumentID2, 
                                                     std::string& strComInstrumentID,
                                                     std::string& strLeg1InstrumentID,
                                                     std::string& strLeg2InstrumentID)
{
    PlatformStru_InstrumentInfo InstrumentInfo1,InstrumentInfo2,InstrumentInfo3;
    if(GetInstrumentInfo(strInstrumentID1,InstrumentInfo1)==-1||
        GetInstrumentInfo(strInstrumentID2,InstrumentInfo2)==-1)
        return false;

    string Prefix;
    if(string(InstrumentInfo1.ExchangeID)==string("DCE"))
    {
        //��������SP a1209&a1301 ��SPC l1209&v1209
        if(memcmp(InstrumentInfo1.ProductID,InstrumentInfo2.ProductID,sizeof(InstrumentInfo1.ProductID))==0)
            Prefix="SP ";
        else Prefix="SPC ";
    }
    else if(string(InstrumentInfo1.ExchangeID)==string("CZCE"))
    {
        //֣������ֻ�п���������SPD CF205&CF207
        Prefix="SPD ";
    }
    else return false;

    strComInstrumentID="SP "+strInstrumentID1+"&"+strInstrumentID2;
    if(GetInstrumentInfo(strComInstrumentID,InstrumentInfo3)==0) 
    {
        strLeg1InstrumentID=strInstrumentID1;
        strLeg2InstrumentID=strInstrumentID2;
        return true;
    }
    strComInstrumentID="SP "+strInstrumentID2+"&"+strInstrumentID1;
    if(GetInstrumentInfo(strComInstrumentID,InstrumentInfo3)==0) 
    {
        strLeg1InstrumentID=strInstrumentID2;
        strLeg2InstrumentID=strInstrumentID2;
        return true;
    }
    return false;
}

//��ȡָ����Լ�����¼�
double CPlatformDataMgr::GetCurPrice (const std::string& InstrumentID)
{
    double ret=0.0;
	//��Ϻ�Լ
	char clstype=GetProductClassType(InstrumentID);
	if(clstype==THOST_FTDC_PC_Combination)
	{
		std::string id1,id2;
		ConvertComboInstrumentID(InstrumentID,id1,id2);
		double prc1=GetCurPrice(id1);
		double prc2=GetCurPrice(id2);
		ret= prc1-prc2;
	}
	else if(clstype==THOST_FTDC_PC_Futures)
	{
        EnterCriticalSection(&m_CS_Quot);
		
        std::map<std::string,  PlatformStru_DepthMarketData>::iterator it =  m_lastQuot.find(InstrumentID);
		if(it!=m_lastQuot.end())
		{
            if(it->second.Volume>0) ret= it->second.LastPrice;
            else ret= it->second.PreSettlementPrice;
		}

        LeaveCriticalSection(&m_CS_Quot);
	}
    return ret;
}

//��ȡָ����Լ�����飬�ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr::GetOldQuotInfo (const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    int ret;

    EnterCriticalSection(&m_CS_Quot);

    std::map<std::string,  PlatformStru_DepthMarketData>::iterator it =  m_OldQuot.find(InstrumentID);
    if(it==m_OldQuot.end())
    {
        memset(&outData,0,sizeof(outData));
        ret=-1;
    }
    else
    {
        outData = it->second;
        ret=0;
    }

    LeaveCriticalSection(&m_CS_Quot);

    return ret;
}



//����Ͷ������Ϣ
void CPlatformDataMgr::SetInvestorInfo(const DataRspQryInvestor& inData)
{
    Lock_CPlatformDataMgr;

    m_InvestorInfo=inData;

    Unlock_CPlatformDataMgr;

}
string CPlatformDataMgr::GetTradingDay(const string& ExchangeID)
{
	if(m_exchangeTime.CurTickCount>0)
		return string(m_exchangeTime.TradingDay);
	return string("");
}

//��ȡ������ʱ����Ϣ
unsigned long CPlatformDataMgr::GetExchangeTime(const string& ExchangeID,string& time)
{
    if(ExchangeID=="SHFE")
		time=m_exchangeTime.SHFETime;
	else if(ExchangeID=="DCE")
		time=m_exchangeTime.DCETime;
    else if(ExchangeID=="CZCE")
		time=m_exchangeTime.CZCETime;
	else if(ExchangeID=="CFFEX")
		time=m_exchangeTime.FFEXTime;
    return m_exchangeTime.CurTickCount;
}
int CPlatformDataMgr::GetExchangeTime(ExchangeTime& outData)
{
    if(m_exchangeTime.CurTickCount==0)
    {
        memset(&outData,0,sizeof(outData));
        return -1;
    }
    outData=m_exchangeTime;
    return 0;
}
	//��ȡȫ��������
int CPlatformDataMgr::GetExchangeIDs(vector<string>& outData)
{
    Lock_CPlatformDataMgr;

	outData.clear();
	outData.push_back("SHFE");
	outData.push_back("CZCE");
	outData.push_back("DCE");
	outData.push_back("CFFEX");

    Unlock_CPlatformDataMgr;

	return outData.size();
}
//���ý�����ʱ����Ϣ
void CPlatformDataMgr::SetExchangeTime(const ExchangeTime& inData)
{
    m_exchangeTime=inData;
}



//��ȡ������Ϣ���ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr::GetLastSettlementInfo(std::string& outData)
{
    int ret;

    Lock_CPlatformDataMgr;

    if(m_bSettlementInfoIsReady)
    {
        outData=m_LastSettlementInfo;
        ret=0;
    }
    else
        ret=-1;

    Unlock_CPlatformDataMgr;

    return ret;
}

//���������Ϣ
void CPlatformDataMgr::ClearLastSettlementInfo(void)
{
    Lock_CPlatformDataMgr;

    m_LastSettlementInfo.clear();
    m_bSettlementInfoIsReady=false;

    Unlock_CPlatformDataMgr;

}

//��ӽ�����Ϣ
void CPlatformDataMgr::AppendLastSettlementInfo(const std::string& inData,bool bLast)
{
    Lock_CPlatformDataMgr;

    if(m_bSettlementInfoIsReady)
    {
        m_LastSettlementInfo.clear();
        m_bSettlementInfoIsReady=false;
    }

    m_LastSettlementInfo.append(inData);
    if(bLast)
        m_bSettlementInfoIsReady=true;

    Unlock_CPlatformDataMgr;

}


//ҵ���ƣ��ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr::SubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID, const GuiCallBackFunc& callback)
{
    int ret;

    if(BID>=BID_MAX || GID>=GID_MAX || callback==NULL)
        return -1;


    Lock_CPlatformDataMgr;

    ret=0;
    try
    {
        m_subscribeInfo[BID][GID]=callback;
    }
    catch(...)
    {
        ret=-2;
    }

    Unlock_CPlatformDataMgr;

    return ret;
}


///ȡ��ҵ�����ݵĶ��ƣ��ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr::UnSubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID)
{
    int ret;

    if(BID>=BID_MAX || GID>=GID_MAX)
        return -1;

    Lock_CPlatformDataMgr;

    ret=0;

    std::map<BusinessID, std::map<GUIModuleID, GuiCallBackFunc> >::iterator it1=m_subscribeInfo.find(BID);
    if(it1==m_subscribeInfo.end())
        ret=-3;
    else
    {
        std::map<GUIModuleID, GuiCallBackFunc>::iterator it2 = it1->second.find(GID);
        if(it2==it1->second.end())
            ret=-3;
        else
        {
            it1->second.erase(it2);
            if(it1->second.empty())
                m_subscribeInfo.erase(it1);
            ret=0;
        }
    }

    Unlock_CPlatformDataMgr;

    return ret;
}


///��ȡҵ������Ϣ���ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr::GetSubscribeBusinessData(const BusinessID BID, std::map<GUIModuleID,GuiCallBackFunc>& outData)
{
    int ret=0;
    outData.clear();

    Lock_CPlatformDataMgr;

    std::map<BusinessID, std::map<GUIModuleID, GuiCallBackFunc> >::iterator it1=m_subscribeInfo.find(BID);
    if(it1==m_subscribeInfo.end())
        ret=-1;
    else
    {
        outData=it1->second;
        ret=0;
    }

    Unlock_CPlatformDataMgr;

	return ret;
}



SERVER_PARAM_CFG&  CPlatformDataMgr::GetServerParamCfg()
{
	return m_ServerParamCfg;
}
void CPlatformDataMgr::SetModuleDataQry(bool bQry)
{
    m_ModuleDataQry=bQry;
}
bool CPlatformDataMgr::GetModuleDataQry()
{
    return m_ModuleDataQry;
}

//�洢�ͻ�ǩԼ��������
void CPlatformDataMgr:: SaveBankName(CThostFtdcContractBankField& inData)
{
	LOG_INFO("CPlatformDataMgr::SaveBankName");
   
    m_BankName[inData.BankName]=inData;
    LOG_INFO("CPlatformDataMgr::SaveBankName-end");
}
//���¿ͻ�ǩԼ������Ϣ
void CPlatformDataMgr:: UpdateContractBank(CThostFtdcAccountregisterField& inData)
{
	LOG_INFO("CPlatformDataMgr::UpdateContractBank");
    PlatformStru_ContractBank   ContractBank;
	memset(&ContractBank,0,sizeof(PlatformStru_ContractBank));
    strcpy(ContractBank.BankID,inData.BankID);
	strcpy(ContractBank.BankAccount,inData.BankAccount);
	strcpy(ContractBank.CurrencyID,inData.CurrencyID);
	strcpy(ContractBank.BankBranchID,inData.BankBranchID);
	strcpy(ContractBank.BrokerID,inData.BrokerID);


	std::map<std::string,  CThostFtdcContractBankField>::iterator it=m_BankName.begin();
	for(; it!=m_BankName.end(); ++it)
	{
		if(!strcmp(inData.BankID, it->second.BankID))
		{
			strcpy(ContractBank.BankName,it->second.BankName);
			strcpy(ContractBank.BankBranchID,it->second.BankBrchID);
			break;

		}

	}

    Lock_CPlatformDataMgr;

	m_ContractBankInfo[ContractBank.BankName]=ContractBank;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::UpdateContractBank-end");
	//m_Accountregister.push_back(inData.BankID);
	//m_timer.Start(1000,true);
}

//��ȡ�ͻ�ǩԼ������Ϣ
int CPlatformDataMgr::GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData)
{
	LOG_INFO("CPlatformDataMgr::GetContractBank");

    Lock_CPlatformDataMgr;

	outData=m_ContractBankInfo;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::GetContractBank-end");
	return outData.size();

}


///���õ�ǰ��ʾ�ĳֲ����ݡ�1:�ֲ֣�2:�ֲ���ϸ��3:��ϳֲ�
void CPlatformDataMgr::SetCurrentPositionContent(int PositionContentMode)
{
    switch(PositionContentMode)
    {
        case 1:m_PlatFormService->SetQryPrior(1);      break;
        case 2:m_PlatFormService->SetQryPrior(2);      break;
        case 3:m_PlatFormService->SetQryPrior(3);      break;
    }
}
void CPlatformDataMgr::IncrementReqMarginRateError(const std::string& InstrumentID)
{
    Lock_CPlatformDataMgr
	if(m_reqMarginRateError.find(InstrumentID)==m_reqMarginRateError.end())
		m_reqMarginRateError[InstrumentID]=0;
	else
		m_reqMarginRateError[InstrumentID]++;
	Unlock_CPlatformDataMgr
}
void CPlatformDataMgr::IncrementReqCommissionRateError(const std::string& InstrumentID)
{
    Lock_CPlatformDataMgr
	if(m_reqCommissionRateError.find(InstrumentID)==m_reqCommissionRateError.end())
		m_reqCommissionRateError[InstrumentID]=0;
	else
		m_reqCommissionRateError[InstrumentID]++;
	Unlock_CPlatformDataMgr
}

