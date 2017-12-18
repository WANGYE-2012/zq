#include "stdafx.h"

#include "wx/file.h"
#include "wx/textfile.h"

#include <vector>
#include <string>
#include <map>

#include "CmeApiSvr.h"
#include "PlatFormService.hpp"
#include "PlatformDataMgr.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "../inc/Module-Misc/tools_util.h"
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

class IPlatformSvr;
extern IPlatformSvr* g_pPlatform;


CPlatformDataMgr::CPlatformDataMgr(CPlatFormService* pHost):
m_PlatFormService(pHost)
{
    InitializeCriticalSection(&m_CS);

    m_pWriteLog=new zqWriteLog(LOGMODE_LOCALFILE,"PlatformDataMgr.log");

    m_TradingAccount.Head.BID=BID_MAX;//���m_TradingAccount��Ч
    m_bNeedCheckCommissionRateAndMarginRate=false;

    m_MaxPositionDetailID=1;
    m_bGetOriginOrderData=false;
    m_bGetOriginTradeData=false;
    m_bGetOriginPositionDetailData=false;
    m_InvestorInfo.Head.BID=BID_MAX;  //���m_InvestorInfo��Ч
    m_bSettlementInfoIsReady=false;



}

CPlatformDataMgr::~CPlatformDataMgr(void)
{
    if(m_pWriteLog)
    {
        delete m_pWriteLog;
        m_pWriteLog=NULL;
    }

    DeleteCriticalSection(&m_CS);
}


///���ý����ʽ��˻���Ϣ
void CPlatformDataMgr::SetTradingAccount(const PlatformStru_TradingAccountInfo& inData)
{
    LOG_INFO("CPlatformDataMgr::SetTradingAccount1");

    EnterCriticalSection(&m_CS);
   // SetAccountField(m_TradingAccount, inData.Thost );
    memcpy( &m_TradingAccount.TradingAccountField, &inData, sizeof(PlatformStru_TradingAccountInfo));
    LeaveCriticalSection(&m_CS);

    LOG_INFO("CPlatformDataMgr::SetTradingAccount1-end");

}
void CPlatformDataMgr::SetAccountField(DataRspQryTradingAccountEx& accex, const CThostFtdcTradingAccountFieldEx& field )
{
	SetAccountField(accex,*((CThostFtdcTradingAccountField*)&field));
}
void CPlatformDataMgr::SetAccountField(DataRspQryTradingAccountEx& accex, const CThostFtdcTradingAccountField& field )
{
	CThostFtdcTradingAccountField data = field;	
	CThostFtdcTradingAccountFieldEx& TradingAccountField=accex.TradingAccountField;
	memcpy( &TradingAccountField, &data, sizeof(CThostFtdcTradingAccountField));

	if(g_pPlatform&&g_pPlatform->GetPlatformId()==PID_CTP)
	{
		TradingAccountField.StaticProfit = TradingAccountField.PreBalance
			- TradingAccountField.PreCredit - TradingAccountField.PreMortgage
			+ TradingAccountField.Mortgage - TradingAccountField.Withdraw 
			+ TradingAccountField.Deposit;
		TradingAccountField.DynamicProfit = TradingAccountField.StaticProfit 
			+ TradingAccountField.CloseProfit + TradingAccountField.PositionProfit 
			- TradingAccountField.Commission;
		TradingAccountField.Available = TradingAccountField.DynamicProfit
			- TradingAccountField.CurrMargin - TradingAccountField.FrozenMargin
			- TradingAccountField.FrozenCommission 
			- TradingAccountField.DeliveryMargin + TradingAccountField.Credit;
		TradingAccountField.FrozenCash = TradingAccountField.FrozenMargin 
			+ TradingAccountField.FrozenCommission;
		if ( util::equals( TradingAccountField.DynamicProfit, 0) )
		{
			TradingAccountField.RiskDegree = 0;
		}
		else
		{
			TradingAccountField.RiskDegree = ( TradingAccountField.CurrMargin 
				+ TradingAccountField.DeliveryMargin ) 
				* 100 / TradingAccountField.DynamicProfit;
		}
	}
	else
		TradingAccountField.DynamicProfit = TradingAccountField.CashIn;

}
void CPlatformDataMgr::Assign(DataRspQryTradingAccountEx& TradingAccountField,const DataRspQryTradingAccount& data )
{
	TradingAccountField.Head = data.Head;
	TradingAccountField.nRequestID = data.nRequestID;
	TradingAccountField.bIsLast = data.bIsLast;
	//SetAccountField(TradingAccountField,data.TradingAccountField );
	TradingAccountField.RspInfoField = data.RspInfoField;
}
///���ý����ʽ��˻���Ϣ
void CPlatformDataMgr::SetTradingAccount(const DataRspQryTradingAccount& outData)
{
    LOG_INFO("CPlatformDataMgr::SetTradingAccount2");

    EnterCriticalSection(&m_CS);
    //Assign(m_TradingAccount,outData );
	m_TradingAccount.Head = outData.Head;
	m_TradingAccount.nRequestID = outData.nRequestID;
	m_TradingAccount.bIsLast = outData.bIsLast;
	memcpy( &m_TradingAccount, &outData, sizeof(DataRspQryTradingAccount));
	m_TradingAccount.RspInfoField = outData.RspInfoField;
    LeaveCriticalSection(&m_CS);

    LOG_INFO("CPlatformDataMgr::SetTradingAccount2-end");
}
int CPlatformDataMgr::GetTradingAccountAvailable(double& fAvailable)
{
    int ret;
    EnterCriticalSection(&m_CS);
    if(m_TradingAccount.Head.BID==BID_MAX)
    {
        ret= -1;
    }
    else
    {
        fAvailable=m_TradingAccount.TradingAccountField.Available;
        ret=0;
    }
    LeaveCriticalSection(&m_CS);
    return ret;   
}
int CPlatformDataMgr::GetTradingAccountWithdrawQuota(double& fWithdrawQuota)
{
    int ret;
    EnterCriticalSection(&m_CS);
    if(m_TradingAccount.Head.BID==BID_MAX)
    {
        ret= -1;
    }
    else
    {
        fWithdrawQuota=m_TradingAccount.TradingAccountField.WithdrawQuota;
        ret=0;
    }
    LeaveCriticalSection(&m_CS);
    return ret;   
}
int CPlatformDataMgr::GetTradingAccountID(char* AccountID)
{
    int ret;
    EnterCriticalSection(&m_CS);
    if(m_TradingAccount.Head.BID==BID_MAX)
    {
        ret= -1;
    }
    else
    {
		strcpy(AccountID,m_TradingAccount.TradingAccountField.AccountID);
        ret=0;
    }
    LeaveCriticalSection(&m_CS);
    return ret;   
}
///��ȡ�����ʽ��˻���Ϣ
int CPlatformDataMgr::GetTradingAccount(PlatformStru_TradingAccountInfo& outData)
{
    int ret;

    LOG_INFO("CPlatformDataMgr::GetTradingAccount");

    EnterCriticalSection(&m_CS);
    if(m_TradingAccount.Head.BID==BID_MAX)
    {
        memset(&outData,0,sizeof(outData));
        ret= -1;
    }
    else
    {
		memcpy(&outData,&m_TradingAccount.TradingAccountField,sizeof(PlatformStru_TradingAccountInfo));
        ret=0;
    }
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::GetTradingAccount-end");
    return ret;   
}

///��̬�����ʽ��˻���Ϣ�����ݳɽ��ر��������ر������ʲ�ѯ�ر�����Լ��ѯ�ر�
void CPlatformDataMgr::DynamicUpdateTradingAccount(void)
{
    LOG_INFO("CPlatformDataMgr::DynamicUpdateTradingAccount");

    EnterCriticalSection(&m_CS);

    GetFreezeMargin_Commission_Internal(m_TradingAccount.TradingAccountField.FrozenMargin,
                                        m_TradingAccount.TradingAccountField.FrozenCommission);
    GetAllCommission_Internal(m_TradingAccount.TradingAccountField.Commission);
    GetAllMargin_PositionProfitByDate_Internal( m_TradingAccount.TradingAccountField.CurrMargin,
                                                m_TradingAccount.TradingAccountField.PositionProfit,
                                                m_TradingAccount.TradingAccountField.CloseProfit);

    LeaveCriticalSection(&m_CS);

    LOG_INFO("CPlatformDataMgr::DynamicUpdateTradingAccount-end");

}

///��̬�����ʽ��˻���Ϣ����������
void CPlatformDataMgr::DynamicUpdateTradingAccount_NewQuot(std::string InstrumentID)
{
    LOG_INFO("CPlatformDataMgr::DynamicUpdateTradingAccount_NewQuot");

    EnterCriticalSection(&m_CS);

    std::multimap<std::string,OrderKey>::iterator it_Order;
    it_Order=m_allOrderIndex.find(InstrumentID);
    if(it_Order!=m_allOrderIndex.end())
    {
        GetFreezeMargin_Commission_Internal(m_TradingAccount.TradingAccountField.FrozenMargin,
                                            m_TradingAccount.TradingAccountField.FrozenCommission);
    }

    GetAllMargin_PositionProfitByDate_Internal( m_TradingAccount.TradingAccountField.CurrMargin,
                                                m_TradingAccount.TradingAccountField.PositionProfit,
                                                m_TradingAccount.TradingAccountField.CloseProfit);

    LeaveCriticalSection(&m_CS);

    LOG_INFO("CPlatformDataMgr::DynamicUpdateTradingAccount_NewQuot-end");

}



///����ָ����Լ��Ϣ
void CPlatformDataMgr::SetInstrumentInfo(const std::string& InstrumentID, const PlatformStru_InstrumentInfo& inData)
{
    LOG_INFO("CPlatformDataMgr::SetInstrumentInfo");
    EnterCriticalSection(&m_CS);
    m_allInstruments[InstrumentID] = inData;
    m_allInstrumentList.insert(InstrumentID);
    m_allProductID[inData.ProductID].insert(InstrumentID);
    m_bNeedCheckCommissionRateAndMarginRate=true;
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::SetInstrumentInfo-end");
}
///��ȡָ����Լ��Ϣ
int CPlatformDataMgr::GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    LOG_INFO("CPlatformDataMgr::GetInstrumentInfo");
    int ret;
    EnterCriticalSection(&m_CS);
    ret=GetInstrumentInfo_Internal(InstrumentID,outData);
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::GetInstrumentInfo-end");
    return ret;
}

///��ȡָ����Լ��Ϣ���ڲ����ã����ü���
int CPlatformDataMgr::GetInstrumentInfo_Internal(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    LOG_INFO("CPlatformDataMgr::GetInstrumentInfo_Internal");
    int ret;
    stdext::hash_map<std::string,  PlatformStru_InstrumentInfo>::iterator it;
    it =  m_allInstruments.find(InstrumentID);
    memset(&outData,0,sizeof(outData));
    if(it==m_allInstruments.end())
    {
        ret=-1;
    }
    else 
    {
        outData = it->second;
        ret=0;
    }
    LOG_INFO("CPlatformDataMgr::GetInstrumentInfo_Internal-end");
    return ret;
}

///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
char CPlatformDataMgr::GetProductClassType(const std::string& InstrumentID)
{
    //LOG_INFO("CPlatformDataMgr::GetProductClassType");
    char ProductClassType;
    stdext::hash_map<std::string,  PlatformStru_InstrumentInfo>::iterator it;
    EnterCriticalSection(&m_CS);
    it =  m_allInstruments.find(InstrumentID);
    if(it==m_allInstruments.end())
        ProductClassType=-1;
    else 
        ProductClassType = it->second.ProductClass;
    LeaveCriticalSection(&m_CS);
    //LOG_INFO("CPlatformDataMgr::GetProductClassType-end");
    return ProductClassType;
}
///��ȡ��Լ�б�
int CPlatformDataMgr::GetInstrumentList(std::vector<GroupInfo> &outData)
{
	if(m_allGroupInfo.empty())
	{
		std::map<std::string,int> GroupName2ID;
		std::map<std::string,int>::iterator GroupName2ID_it;
		PlatformStru_InstrumentInfo instInfo;
		std::set<std::string>::iterator it=m_allInstrumentList.begin();
		for(it;it!=m_allInstrumentList.end();++it)
		{
			if(GetInstrumentInfo_Internal(*it,instInfo)==0)
			{
				GroupName2ID_it=GroupName2ID.find(instInfo.ExchangeID);
				if(GroupName2ID_it==GroupName2ID.end())
				{
					GroupInfo ginfo;
					ginfo.GroupName=instInfo.ExchangeID;
					ginfo.InstrVec.push_back(InstrumentInfo(instInfo.InstrumentID,instInfo.InstrumentName));
					m_allGroupInfo.push_back(ginfo);
					GroupName2ID[instInfo.ExchangeID]=m_allGroupInfo.size()-1;
				}
				else
				{
					m_allGroupInfo[GroupName2ID_it->second].InstrVec.push_back(InstrumentInfo(instInfo.InstrumentID,instInfo.InstrumentName));
				}
			}
		}
	}
	outData=m_allGroupInfo;
	return 0;
}
///��ȡ��Լ�б�
int CPlatformDataMgr::GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID)
{
    LOG_INFO("CPlatformDataMgr::GetInstrumentList");
    EnterCriticalSection(&m_CS);
	if(ExchangeID.empty())
		outData = m_allInstrumentList;
	else
	{
		outData.clear();
		std::set<std::string>::iterator it=m_allInstrumentList.begin();
		PlatformStru_InstrumentInfo instInfo;
		for(it;it!=m_allInstrumentList.end();++it)
		{
			if(GetInstrumentInfo_Internal(*it,instInfo)==0&&ExchangeID==instInfo.ExchangeID)
			{
				outData.insert(*it);
			}
		}
	}
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::GetInstrumentList-end");
    return outData.size();
}
//���������Լ�б�
int CPlatformDataMgr::AddMainInstrumentList(std::string instrument)
{
	EnterCriticalSection(&m_CS);
	m_allMainInstrumentList.insert(instrument);
    LeaveCriticalSection(&m_CS);
	return m_allMainInstrumentList.size();
}
///��ȡ��Լ�б�
int CPlatformDataMgr::GetMainInstrumentList(std::set<std::string> &outData)
{
    LOG_INFO("CPlatformDataMgr::GetMainInstrumentList");
	EnterCriticalSection(&m_CS);
#ifdef _NEED_MAIN_INSTRUMENT
		outData = m_allMainInstrumentList;
#else
		outData=m_allInstrumentList;
#endif	
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::GetMainInstrumentList-end");
    return outData.size();
}
///���ú�Լ�ݲ��б�
void CPlatformDataMgr::SetInstrumentVarietyMap(map<string, string>& inData)
{
    LOG_INFO("CPlatformDataMgr::SetInstrumentVarietyMap");
    EnterCriticalSection(&m_CS);
    m_allInstrumentVarietyMap = inData;
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::SetInstrumentVarietyMap-end");
}

// ��ȡ��Լ�ݲ��б�
int CPlatformDataMgr::GetInstrumentVarietyMap(map<string, string>& outData)
{
    LOG_INFO("CPlatformDataMgr::SetInstrumentVarietyMap");
    EnterCriticalSection(&m_CS);
    outData = m_allInstrumentVarietyMap;
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::SetInstrumentVarietyMap-end");
    return m_allInstrumentVarietyMap.size();
}

///��ȡָ��Ʒ�ֵĺ�Լ�б�,���غ�Լ����
int CPlatformDataMgr::GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData)
{
    LOG_INFO("CPlatformDataMgr::GetInstrumentListByProductID");
    int ret;
    std::map<std::string, std::set<std::string> >::iterator it;
    EnterCriticalSection(&m_CS);
    it=m_allProductID.find(ProductID);
    if(it==m_allProductID.end()) 
        ret=-1;
    else
    {
        outData = it->second;
        ret=outData.size();
    }
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::GetInstrumentListByProductID-end");
    return ret;
}

///��ȡ��ԼƷ�ִ���
int CPlatformDataMgr::GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID)
{
    LOG_INFO("CPlatformDataMgr::GetProductID");
    EnterCriticalSection(&m_CS);
    outData=m_allProductID;
	if(!ExchangeID.empty())
	{
		PlatformStru_InstrumentInfo instInfo;
		std::map<std::string, std::set<std::string> >::iterator it;
		for(it=outData.begin();it!=outData.end();)
		{
			if(it->second.size()==0||
				GetInstrumentInfo_Internal(*(it->second.begin()),instInfo)!=0||
				ExchangeID!=instInfo.ExchangeID)
			{
				outData.erase(it++);
			}
			else
				++it;
		}
	}
    LeaveCriticalSection(&m_CS);
    return outData.size(); 
    LOG_INFO("CPlatformDataMgr::GetProductID-end");
}


///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
int CPlatformDataMgr::GetInstrumentMultiple(const std::string& InstrumentID)
{
    LOG_INFO("CPlatformDataMgr::GetInstrumentMultiple");
    int Multiple;
    stdext::hash_map<std::string,  PlatformStru_InstrumentInfo>::iterator it;
    EnterCriticalSection(&m_CS);
    it =  m_allInstruments.find(InstrumentID);
    if(it==m_allInstruments.end())
        Multiple=-1;
    else 
        Multiple= it->second.VolumeMultiple;
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::GetInstrumentMultiple-end");
    return Multiple;
}

///��ȡ��Լ��������
int CPlatformDataMgr::GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    LOG_INFO("CPlatformDataMgr::GetCommissionRate");
    int ret;
    EnterCriticalSection(&m_CS);
    ret=GetCommissionRate_Internal(InstrumentID,outData);
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::GetCommissionRate-end");
    return ret;
}

///��ȡ��Լ�������ʣ��ڲ����ã����ü���
int CPlatformDataMgr::GetCommissionRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    LOG_INFO("CPlatformDataMgr::GetCommissionRate_Internal");
    int ret=0;
    //std::map<std::string, PlatformStru_InstrumentCommissionRate>::iterator it1;
    //it1=m_InstrumentCommissionRate.find(InstrumentID);
    //memset(&outData,0,sizeof(outData));
    //if(it1==m_InstrumentCommissionRate.end())
    //{
    //    ret=-1;
    //}
    //else
    //{
    //    outData = it1->second;
    //    ret=0;
    //}
	ZeroMemory(&outData,sizeof(PlatformStru_InstrumentCommissionRate));
	strcpy(outData.InstrumentID,InstrumentID.c_str());
    LOG_INFO("CPlatformDataMgr::GetCommissionRate_Internal-end");
    return ret;
}

///���ú�Լ��������
void CPlatformDataMgr::SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    LOG_INFO("CPlatformDataMgr::SetCommissionRate");
    EnterCriticalSection(&m_CS);
    m_InstrumentCommissionRate[InstrumentID]=outData;
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::SetCommissionRate-end");
}

///�������ڲ�ѯ�������ʵĺ�Լ,���ڲ�ѯ����ʱ��Ӧ
void CPlatformDataMgr::SetReqCommissionRateInstrumentID(const std::string& InstrumentID)
{
    LOG_INFO("CPlatformDataMgr::SetReqCommissionRateInstrumentID");
    EnterCriticalSection(&m_CS);
    m_ReqCommissionRateInstrumentID=InstrumentID;
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::SetReqCommissionRateInstrumentID-end");
}

///��ȡ���ڲ�ѯ�������ʵĺ�Լ
void CPlatformDataMgr::GetReqCommissionRateInstrumentID(std::string& outData)
{
    LOG_INFO("CPlatformDataMgr::GetReqCommissionRateInstrumentID");
    EnterCriticalSection(&m_CS);
    outData=m_ReqCommissionRateInstrumentID;
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::GetReqCommissionRateInstrumentID-end");
}

///��ȡ��Լ��֤����
int CPlatformDataMgr::GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    LOG_INFO("CPlatformDataMgr::GetMarginRate");
    int ret;
    EnterCriticalSection(&m_CS);
    ret=GetMarginRate_Internal(InstrumentID,outData);
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::GetMarginRate-end");
    return ret; 
}

///��ȡ��Լ��֤���ʣ��ڲ����ã����ü���
int CPlatformDataMgr::GetMarginRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    LOG_INFO("CPlatformDataMgr::GetMarginRate_Internal");
    int ret=0;
    //std::map<std::string, PlatformStru_InstrumentMarginRate>::iterator it;
    //it=m_InstrumentMarginRate.find(InstrumentID);
    //memset(&outData,0,sizeof(outData));
    //if(it==m_InstrumentMarginRate.end())
    //{
    //    ret=-1;
    //}
    //else
    //{
    //    outData = it->second;
    //    ret=0;
    //}
	ZeroMemory(&outData,sizeof(PlatformStru_InstrumentMarginRate));
	strcpy(outData.InstrumentID,InstrumentID.c_str());
    LOG_INFO("CPlatformDataMgr::GetMarginRate_Internal-end");
    return ret; 
}

///���ú�Լ��֤����
void CPlatformDataMgr::SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    LOG_INFO("CPlatformDataMgr::SetMarginRate");
    EnterCriticalSection(&m_CS);
    m_InstrumentMarginRate[InstrumentID]=outData;
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::SetMarginRate-end");
}



//ԭʼ�ĳɽ��ر�����
void CPlatformDataMgr::OriginTradeReady( const PlatformStru_TradeInfo TradeInfo,const int ErrID,const bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr::OriginTradeReady");

    if(ErrID==0)
    {
        TradeKey tmpKey(TradeInfo.InstrumentID);
		if(strlen(tmpKey.InstrumentID)==0&&
            strlen(tmpKey.TradeID)==0&&
            strlen(tmpKey.TradeDate)==0&&
            strlen(tmpKey.OrderSysID)==0)
			return;
        EnterCriticalSection(&m_CS);
        PlatformStru_TradeInfo TradeInfo2;
        memset(&TradeInfo2,0,sizeof(TradeInfo2));
        TradeInfo2=TradeInfo;
        m_allTrades[tmpKey]=TradeInfo2;
        m_originTrades[tmpKey]=TradeInfo2;
        m_allTradeIndex.insert(make_pair(std::string((char*)TradeInfo.InstrumentID),tmpKey));
        LeaveCriticalSection(&m_CS);

        PlatformStru_InstrumentCommissionRate CommissionRate;
        PlatformStru_InstrumentMarginRate MarginRate;
        memset(&CommissionRate,0,sizeof(CommissionRate));
        memset(&MarginRate,0,sizeof(MarginRate));
        if(g_pPlatform) g_pPlatform->GetCommissionRate_TriggerReq(TradeInfo.InstrumentID,CommissionRate);
        if(g_pPlatform) g_pPlatform->GetMarginRate_TriggerReq(TradeInfo.InstrumentID,MarginRate);
		
		// �ڴ���ӳɽ�ͳ�ƴ���
		SumTradeData(TradeInfo);
    }

    if(bIsLast)
    {
        EnterCriticalSection(&m_CS);
        m_bGetOriginTradeData=true;
        LeaveCriticalSection(&m_CS);

        //ProcessNewTradeList();
    }

    LOG_INFO("CPlatformDataMgr::OriginTradeReady-end");

}

//�µĳɽ��ر����������ɽ����ŵ�m_TradesListWaitForAppend�У�Ȼ�����ProcessNewTradeList����
void CPlatformDataMgr::NewTradeReady( const PlatformStru_TradeInfo TradeInfo)
{
    LOG_INFO("CPlatformDataMgr::NewTradeReady");
    TradeKey tmpKey(TradeInfo);

    EnterCriticalSection(&m_CS);
    m_TradesListWaitForAppend.insert(m_TradesListWaitForAppend.end(),TradeInfo);
    LeaveCriticalSection(&m_CS);

    PlatformStru_InstrumentCommissionRate CommissionRate;
    PlatformStru_InstrumentMarginRate MarginRate;
    memset(&CommissionRate,0,sizeof(CommissionRate));
    memset(&MarginRate,0,sizeof(MarginRate));
    if(g_pPlatform) g_pPlatform->GetCommissionRate_TriggerReq(TradeInfo.InstrumentID,CommissionRate);
    if(g_pPlatform) g_pPlatform->GetMarginRate_TriggerReq(TradeInfo.InstrumentID,MarginRate);

    ProcessNewTradeList();

	// �ڴ���ӳɽ�ͳ�ƴ���
    SumTradeData(TradeInfo);

    LOG_INFO("CPlatformDataMgr::NewTradeReady-end");
}


//��̬���³ֲ���ϸ��¼��ÿ�γɽ��ر��͵õ���֤����ʱ��һ��
//����m_TradesListWaitForAppend������m_allTrades��m_allPositionDetail
void CPlatformDataMgr::ProcessNewTradeList(void)         
{
    LOG_INFO("CPlatformDataMgr::ProcessNewTradeList");


    EnterCriticalSection(&m_CS);

    if( m_bGetOriginTradeData&&
        m_bGetOriginPositionDetailData&&
        m_TradesListWaitForAppend.size()>0)
    {
        std::list<PlatformStru_TradeInfo>::iterator it1,ittmp;
        std::map<TradeKey, PlatformStru_TradeInfo>::iterator it2;
        it1=m_TradesListWaitForAppend.begin();
        while(it1!=m_TradesListWaitForAppend.end())
        {
            TradeKey tmpKey(*it1);

            it2=m_originTrades.find(tmpKey);

            
            if(it2==m_originTrades.end())
            {   
				//ԭʼ�ɽ���Ϣ��û�У������³ɽ�����,ˢ�³ֲ�����
				RefreshPositionData();
            }

            if(it2!=m_originTrades.end())
            {
                m_allTrades[tmpKey]=*it1;
                m_allTradeIndex.insert(make_pair(std::string(it1->InstrumentID),tmpKey));

                ittmp=it1;
                ++it1;
                m_TradesListWaitForAppend.erase(ittmp);
            }
            else ++it1;
        }
    }

    LeaveCriticalSection(&m_CS);

    LOG_INFO("CPlatformDataMgr::ProcessNewTradeList-end");
}





///��ȡȫ���ɽ���
int CPlatformDataMgr::GetAllTrades(std::map<TradeKey,  PlatformStru_TradeInfo>& outData)
{
    LOG_INFO("CPlatformDataMgr::GetAllTrades");
    EnterCriticalSection(&m_CS);
    outData=m_allTrades;
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::GetAllTrades-end");
    return outData.size();
}

///��ȡȫ���ɽ���
int CPlatformDataMgr::GetAllTradeTotalDatas(vector<TotalInfo>& outData)
{
    LOG_INFO("CPlatformDataMgr::GetAllTradeTotalDatas");
    EnterCriticalSection(&m_CS);
    outData=m_TradeTotalData;
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::GetAllTradeTotalDatas-end");
    return outData.size();
}

//�ӳɽ���ȡ�����ѡ����û�з��ʣ���0����
void CPlatformDataMgr::GetAllCommission_Internal(double& TotalCommission)
{
    LOG_INFO("CPlatformDataMgr::GetAllCommission_Internal");
    TotalCommission=0;

    std::map<TradeKey,  PlatformStru_TradeInfo>::iterator iter;
    PlatformStru_InstrumentCommissionRate CommissionRate;
    PlatformStru_InstrumentInfo InstrumentInfo;
    std::string strInstrumentID;

    iter = m_allTrades.begin();
    for(;iter != m_allTrades.end(); ++iter)
    {
        strInstrumentID=std::string(iter->second.InstrumentID);
        if(GetInstrumentInfo_Internal(strInstrumentID,InstrumentInfo)==0)
        {
            //����������
            if(GetCommissionRate_Internal(strInstrumentID,CommissionRate)==0)
            {
                if(iter->second.OffsetFlag == THOST_FTDC_OF_Open)
                    TotalCommission+=  iter->second.Price*iter->second.Volume*InstrumentInfo.VolumeMultiple*CommissionRate.OpenRatioByMoney+
                                        iter->second.Volume*CommissionRate.OpenRatioByVolume;
                else if(iter->second.OffsetFlag == THOST_FTDC_OF_Close||
                        iter->second.OffsetFlag == THOST_FTDC_OF_CloseYesterday||
                        iter->second.OffsetFlag == THOST_FTDC_OF_ForceClose||
                        iter->second.OffsetFlag == THOST_FTDC_OF_LocalForceClose)
                    TotalCommission+=  iter->second.Price*iter->second.Volume*InstrumentInfo.VolumeMultiple*CommissionRate.CloseRatioByMoney+
                                        iter->second.Volume*CommissionRate.CloseRatioByVolume;
                else if(iter->second.OffsetFlag == THOST_FTDC_OF_CloseToday)
                    TotalCommission+=  iter->second.Price*iter->second.Volume*InstrumentInfo.VolumeMultiple*CommissionRate.CloseTodayRatioByMoney+
                                        iter->second.Volume*CommissionRate.CloseTodayRatioByVolume;
            }
        }
    }
    LOG_INFO("CPlatformDataMgr::GetAllCommission_Internal-end");
}








//ԭʼ��������
void CPlatformDataMgr::OriginOrderReady(const PlatformStru_OrderInfo& OrderInfo,const int ErrID,const bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr::OriginOrderReady");

    PlatformStru_InstrumentCommissionRate CommissionRate;
    PlatformStru_InstrumentMarginRate MarginRate;

    PlatformStru_OrderInfo OrderInfo2;
    memset(&OrderInfo2,0,sizeof(OrderInfo2));
    OrderInfo2=OrderInfo;

	if(ErrID==0)
	{
		OrderKey tmp(OrderInfo);
		if(tmp.InstrumentID==""&&tmp.FrontID==0&&tmp.SessionID==0&&tmp.OrderRef=="")
			return;
		EnterCriticalSection(&m_CS);
		m_originOrders[tmp]= OrderInfo2;
		m_allOrders[tmp]= OrderInfo2;
		m_allOrderIndex.insert(make_pair(std::string(OrderInfo.InstrumentID),tmp));
		//����ί�е�ģ��
		AllOrderKey tmpKey(std::string(OrderInfo.InstrumentID), 
		                    OrderInfo.FrontID, 
		                    OrderInfo.SessionID, 
		                    std::string((char*)OrderInfo.OrderRef),
		                    OrderInfo.OrderStatus);

		PlatformStru_OrderInfo MyData;
		memset(&MyData,0,sizeof(MyData));
		memcpy(&MyData,&OrderInfo,sizeof(PlatformStru_OrderInfo));

		/*wxString strtmp;
		if(MyData.ContingentCondition > THOST_FTDC_CC_ParkedOrder)
		{
			wxString strCondDir;
			switch(MyData.ContingentCondition) 
			{
			case THOST_FTDC_CC_LastPriceGreaterThanStopPrice:	///���¼۴���������
				strCondDir = wxT("���¼� > ");
				break;
			case THOST_FTDC_CC_LastPriceGreaterEqualStopPrice:	///���¼۴��ڵ���������
				strCondDir = wxT("���¼� >= ");
				break;
			case THOST_FTDC_CC_LastPriceLesserThanStopPrice:	///���¼�С��������
				strCondDir = wxT("���¼� < ");
				break;
			case THOST_FTDC_CC_LastPriceLesserEqualStopPrice:	///���¼�С�ڵ���������
				strCondDir = wxT("���¼� <= ");
				break;
			case THOST_FTDC_CC_AskPriceGreaterThanStopPrice:	///��һ�۴���������
				strCondDir = wxT("��һ�� > ");
				break;
			case THOST_FTDC_CC_AskPriceGreaterEqualStopPrice:	///��һ�۴��ڵ���������
				strCondDir = wxT("��һ�� >= ");
				break;
			case THOST_FTDC_CC_AskPriceLesserThanStopPrice:		///��һ��С��������
				strCondDir = wxT("��һ�� < ");
				break;
			case THOST_FTDC_CC_AskPriceLesserEqualStopPrice:	///��һ��С�ڵ���������
				strCondDir = wxT("��һ�� <= ");
				break;
			case THOST_FTDC_CC_BidPriceGreaterThanStopPrice:	///��һ�۴���������
				strCondDir = wxT("��һ�� > ");
				break;
			case THOST_FTDC_CC_BidPriceGreaterEqualStopPrice:	///��һ�۴��ڵ���������
				strCondDir = wxT("��һ�� >= ");
				break;
			case THOST_FTDC_CC_BidPriceLesserThanStopPrice:		///��һ��С��������
				strCondDir = wxT("��һ�� < ");
				break;
			case THOST_FTDC_CC_BidPriceLesserEqualStopPrice:	///��һ��С�ڵ���������
				strCondDir = wxT("��һ�� <= ");
				break;
			};
			wxString strPrice;
			if(MyData.OrderPriceType==THOST_FTDC_OPT_AnyPrice)
				strPrice.Printf("�м� ʱ����");
			else
				strPrice.Printf("%0.2f ʱ����", MyData.LimitPrice);
			strtmp=strCondDir+strPrice;
		}
		else 
		{
			strtmp=MyData.StatusMsg;
		}
		memcpy(MyData.StatusMsg,strtmp.c_str(),sizeof(MyData.StatusMsg));
*/

		//wxTimer m_timer;//��ʱ ��ѯ��Լ���������뱣֤����
		PlatformStru_InstrumentMarginRate MarginRateField;// ��֤��
		PlatformStru_InstrumentInfo InstrumentFields;  // �Ӻ�Լ�б���ȡ
		TThostFtdcRatioType d_RatioType;

		MyData.freezeMargin = 0.00;     //���ᱣ֤�� --���ͷ��֤����*ί�м�
		MyData.troubleMoney = 0.00;     //���������� --�����������ʼ���

		if ( MyData.OrderStatus == THOST_FTDC_OST_PartTradedQueueing	//���ֳɽ����ڶ����� '1'				
			|| MyData.OrderStatus == THOST_FTDC_OST_NoTradeQueueing	//δ�ɽ����ڶ����� '3'
			|| MyData.OrderStatus == THOST_FTDC_OST_Unknown )		//δ֪ -- ���ύ  ���ҵ� 'a'

		{
            int iVolumeMultiple = 0;//GetInstrumentInfo	
			if( ( g_pPlatform&&g_pPlatform->GetInstrumentInfo( MyData.InstrumentID, InstrumentFields) ) == 0 )//VolumeMultiple��Լ���� 
			{
				iVolumeMultiple = InstrumentFields.VolumeMultiple;
			}
			if( ( g_pPlatform&&g_pPlatform->GetMarginRate( MyData.InstrumentID, MarginRateField) ) == 0 )//��֤������
			{
				if ( MyData.Direction == THOST_FTDC_D_Buy ) {
                    d_RatioType = MarginRateField.LongMarginRatioByMoney; ///��ͷ��֤����
					if (MarginRateField.LongMarginRatioByVolume > 0) {
						MyData.freezeMargin = MarginRateField.LongMarginRatioByVolume * MyData.VolumeTotal;//��ͷ��֤������
					}
				}else if ( MyData.Direction == THOST_FTDC_D_Sell ) {
					d_RatioType = MarginRateField.ShortMarginRatioByMoney;	 ///��ͷ��֤����
					if (MarginRateField.ShortMarginRatioByVolume > 0 ) {
						MyData.freezeMargin = MarginRateField.ShortMarginRatioByVolume * MyData.VolumeTotal;//��ͷ��֤������
					}
				}

				// ���ᱣ֤�� = ί�м۸� * δ�ɽ����� *  (��Լ����)* ��֤����
				if ( MyData.freezeMargin <= 0 ) {
					MyData.freezeMargin = MyData.LimitPrice * MyData.VolumeTotal * iVolumeMultiple * d_RatioType;
				}                
			} 
			else
			{
				MyData.freezeMargin =-1;
			}



			PlatformStru_InstrumentCommissionRate outData;
			if( ( g_pPlatform&&g_pPlatform->GetCommissionRate( MyData.InstrumentID, outData) ) == 0 )//��������
			{
                double d_RatioByMoney = 0.00;
				if ( MyData.CombOffsetFlag[0] == THOST_FTDC_OF_Open ) { //����
                    d_RatioByMoney = outData.OpenRatioByMoney;
					if ( outData.OpenRatioByVolume > 0 ) {	//���������Ѱ�����
						MyData.troubleMoney =  MyData.VolumeTotal * outData.OpenRatioByVolume;
					} else {
						//  δ�ɽ����� * �����۸� * ��Լ���� * ������������
						MyData.troubleMoney =  MyData.VolumeTotal * MyData.LimitPrice * iVolumeMultiple * d_RatioByMoney;
					}
				}

				if ( MyData.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday ) { //ƽ�� -- ƽ��
					d_RatioByMoney = outData.CloseRatioByMoney;
					if ( outData.OpenRatioByVolume > 0 ) {	//���������Ѱ�����
						MyData.troubleMoney =  MyData.VolumeTotal * outData.CloseRatioByVolume;
					} else {
						//  δ�ɽ����� * �����۸� * ��Լ���� * ������������
						MyData.troubleMoney =  MyData.VolumeTotal * MyData.LimitPrice * iVolumeMultiple * d_RatioByMoney;
					}
				}
				if ( MyData.CombOffsetFlag[0] == THOST_FTDC_OF_Close 
					||MyData.CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose 
					||MyData.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday ) { //ƽ�� -- ��ͨƽ�ֺ�ƽ��
						d_RatioByMoney = outData.CloseRatioByMoney;
						if ( outData.CloseRatioByVolume > 0 ) {	//���������Ѱ�����      //OpenRatioByVolume��ΪCloseRatioByVolume, ����Ӧ���Ǽ���ƽ�������ѡ� modified by l. 2010.12.30
							MyData.troubleMoney =  MyData.VolumeTotal * outData.CloseRatioByVolume;
						} else {
							//  δ�ɽ����� * �����۸� * ��Լ���� * ������������
							MyData.troubleMoney =  MyData.VolumeTotal * MyData.LimitPrice * iVolumeMultiple * d_RatioByMoney;
						}
				}

			}
			else
			{
				MyData.freezeMargin =-1;

			}

		}


		if ( MyData.CombOffsetFlag[0] == THOST_FTDC_OF_Close 
			||MyData.CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose 
			||MyData.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday
			||MyData.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday )
		{MyData.freezeMargin=0.00;}/*ƽ��ʱ���ᱣ֤��Ϊ0*/

		m_OrdersPanel[tmpKey] = MyData;

        LeaveCriticalSection(&m_CS);



        memset(&CommissionRate,0,sizeof(CommissionRate));
        memset(&MarginRate,0,sizeof(MarginRate));
		if(g_pPlatform) g_pPlatform->GetCommissionRate_TriggerReq(OrderInfo.InstrumentID,CommissionRate);
		if(g_pPlatform) g_pPlatform->GetMarginRate_TriggerReq(OrderInfo.InstrumentID,MarginRate);
	}

    //����Ѿ�����ԭʼ�������ݣ���m_OrdersListWaitForAppend�еı���������ӵ�m_allOrders��ȥ
    if(bIsLast)
    {
        OrderKey tmpKey;

        EnterCriticalSection(&m_CS);
        std::list<PlatformStru_OrderInfo>::iterator it,ittmp;
        it=m_OrdersListWaitForAppend.begin();
        while(it!=m_OrdersListWaitForAppend.end())
        {
            tmpKey=OrderKey(it->InstrumentID, 
                            it->FrontID, 
                            it->SessionID, 
                            std::string((char*)it->OrderRef));
	        m_allOrders[tmpKey]= *it;
            m_allOrderIndex.insert(make_pair(std::string(it->InstrumentID),tmpKey));
            ++it;
        }
        m_OrdersListWaitForAppend.clear();
        LeaveCriticalSection(&m_CS);


        m_bGetOriginOrderData=true;
    }


    LOG_INFO("CPlatformDataMgr::OriginOrderReady-end");
}

//�±�������,ע�⣬������ԭ�б�������״̬
void CPlatformDataMgr::NewOrderReady(const PlatformStru_OrderInfo& OrderInfo)
{
    LOG_INFO("CPlatformDataMgr::NewOrderReady");

    PlatformStru_InstrumentCommissionRate CommissionRate;
    PlatformStru_InstrumentMarginRate MarginRate;

    PlatformStru_OrderInfo OrderInfo2;
    memset(&OrderInfo2,0,sizeof(OrderInfo2));
    OrderInfo2=OrderInfo;
    EnterCriticalSection(&m_CS);
    if(m_bGetOriginOrderData)
    {
        OrderKey tmp(   std::string(OrderInfo.InstrumentID), 
                        OrderInfo.FrontID, 
                        OrderInfo.SessionID, 
                        std::string((char*)OrderInfo.OrderRef));
        m_allOrders[tmp]= OrderInfo2;
        m_allOrderIndex.insert(make_pair(std::string(OrderInfo.InstrumentID),tmp));

    }
    else
    {
        //���ԭʼ������Ϣδ�յ����򽫱�����Ϣ���뵽m_OrdersListWaitForAppend�У��ȴ�ԭʼ��Ϣ��ȫ������ӵ�m_allOrders��
        m_OrdersListWaitForAppend.insert(m_OrdersListWaitForAppend.end(),OrderInfo2);
    }
    LeaveCriticalSection(&m_CS);

    memset(&CommissionRate,0,sizeof(CommissionRate));
    memset(&MarginRate,0,sizeof(MarginRate));
    if(g_pPlatform) g_pPlatform->GetCommissionRate_TriggerReq(OrderInfo.InstrumentID,CommissionRate);
    if(g_pPlatform) g_pPlatform->GetMarginRate_TriggerReq(OrderInfo.InstrumentID,MarginRate);
    
    LOG_INFO("CPlatformDataMgr::NewOrderReady-end");
}
//����¼��ر�����������ί�е�
void CPlatformDataMgr::UpdateOrdersforOrderInsert(CThostFtdcInputOrderField &OrderInfo,CThostFtdcRspInfoField &RspInfoField)
{
    LOG_INFO("CPlatformDataMgr::UpdateOrdersforOrderInsert");
	PlatformStru_OrderInfo MyData;
	memset(&MyData,0,sizeof(MyData));
	AllOrderKey tmpKey(std::string(OrderInfo.InstrumentID), 
		ORDER_INSERT_FRONTID, 
		ORDER_INSERT_SESSIONID, 
		std::string((char*)OrderInfo.OrderRef),
		ORDER_INSERT_ORDERSTATUS);

	EnterCriticalSection(&m_CS);

     memcpy(MyData.BrokerID,OrderInfo.BrokerID,sizeof(MyData.BrokerID));
     memcpy( MyData.InvestorID,OrderInfo.InvestorID,sizeof(MyData.InvestorID));
     memcpy( MyData.InstrumentID,OrderInfo.InstrumentID,sizeof(MyData.InstrumentID));
     memcpy( MyData.OrderRef,OrderInfo.OrderRef,sizeof(MyData.OrderRef));
     memcpy( MyData.UserID,OrderInfo.UserID,sizeof(MyData.UserID));
     MyData.OrderPriceType=OrderInfo.OrderPriceType;
     MyData.Direction=OrderInfo.Direction;
     memcpy( MyData.CombOffsetFlag,OrderInfo.CombOffsetFlag,sizeof(MyData.CombOffsetFlag));
     memcpy(  MyData.CombHedgeFlag,OrderInfo.CombHedgeFlag,sizeof(MyData.CombHedgeFlag));
     MyData.LimitPrice=OrderInfo.LimitPrice;
     MyData.VolumeTotalOriginal=OrderInfo.VolumeTotalOriginal;
     MyData.TimeCondition=OrderInfo.TimeCondition;
     memcpy(  MyData.GTDDate,OrderInfo.GTDDate,sizeof(MyData.GTDDate));
     MyData.VolumeCondition=OrderInfo.VolumeCondition;
     MyData.MinVolume=OrderInfo.MinVolume;
     MyData.ContingentCondition=OrderInfo.ContingentCondition;
     MyData.StopPrice=OrderInfo.StopPrice;
     MyData.ForceCloseReason=OrderInfo.ForceCloseReason;
     MyData.IsAutoSuspend=OrderInfo.IsAutoSuspend;
     memcpy(  MyData.BusinessUnit,OrderInfo.BusinessUnit,sizeof(MyData.BusinessUnit));
     MyData.RequestID=OrderInfo.RequestID;
     MyData.OrderStatus=THOST_FTDC_OST_ERROE;
     MyData.VolumeTotal = OrderInfo.VolumeTotalOriginal;
     memcpy(  MyData.StatusMsg, RspInfoField.ErrorMsg, sizeof(MyData.StatusMsg));
	 
	 m_OrdersPanel[tmpKey] = MyData;
	 LeaveCriticalSection(&m_CS);
    


}
//�±�������,��������ί�е�
void CPlatformDataMgr::DynamicUpdateAllOrderPanel(const PlatformStru_OrderInfo& OrderInfo)
{
	LOG_INFO("CPlatformDataMgr::DynamicUpdateAllOrderPanel");
	PlatformStru_OrderInfo MyData;
	memset(&MyData,0,sizeof(MyData));
	AllOrderKey tmpKey(std::string(OrderInfo.InstrumentID), 
		OrderInfo.FrontID, 
		OrderInfo.SessionID, 
		std::string((char*)OrderInfo.OrderRef),
		OrderInfo.OrderStatus);

	EnterCriticalSection(&m_CS);

	AllOrderKey oldKey(std::string(OrderInfo.InstrumentID), 
		OrderInfo.FrontID, 
		OrderInfo.SessionID, 
		std::string((char*)OrderInfo.OrderRef),
		THOST_FTDC_OST_Unknown);

	std::map <AllOrderKey,  PlatformStru_OrderInfo>::iterator it=m_OrdersPanel.find(oldKey);
	
      if(it!=m_OrdersPanel.end())
	  {
          m_OrdersPanel.erase(it);
	  }

	  	AllOrderKey old2Key(std::string(OrderInfo.InstrumentID), 
		OrderInfo.FrontID, 
		OrderInfo.SessionID, 
		std::string((char*)OrderInfo.OrderRef),
		THOST_FTDC_OST_PartTradedQueueing);

	std::map <AllOrderKey,  PlatformStru_OrderInfo>::iterator it2=m_OrdersPanel.find(old2Key);
	
      if(it2!=m_OrdersPanel.end())
	  {
          m_OrdersPanel.erase(it2);
	  }
	  AllOrderKey old3Key(std::string(OrderInfo.InstrumentID), 
		OrderInfo.FrontID, 
		OrderInfo.SessionID, 
		std::string((char*)OrderInfo.OrderRef),
		THOST_FTDC_OST_PartTradedNotQueueing);

	std::map <AllOrderKey,  PlatformStru_OrderInfo>::iterator it3=m_OrdersPanel.find(old3Key);
	
      if(it3!=m_OrdersPanel.end())
	  {
          m_OrdersPanel.erase(it3);
	  }

	  AllOrderKey old4Key(std::string(OrderInfo.InstrumentID), 
		OrderInfo.FrontID, 
		OrderInfo.SessionID, 
		std::string((char*)OrderInfo.OrderRef),
		THOST_FTDC_OST_NoTradeQueueing);

	std::map <AllOrderKey,  PlatformStru_OrderInfo>::iterator it4=m_OrdersPanel.find(old4Key);
	
      if(it4!=m_OrdersPanel.end())
	  {
          m_OrdersPanel.erase(it4);
	  }
	 
	   AllOrderKey old5Key(std::string(OrderInfo.InstrumentID), 
		OrderInfo.FrontID, 
		OrderInfo.SessionID, 
		std::string((char*)OrderInfo.OrderRef),
		THOST_FTDC_OST_NoTradeNotQueueing);

	std::map <AllOrderKey,  PlatformStru_OrderInfo>::iterator it5=m_OrdersPanel.find(old5Key);
	
      if(it5!=m_OrdersPanel.end())
	  {
          m_OrdersPanel.erase(it5);
	  }
	  AllOrderKey old6Key(std::string(OrderInfo.InstrumentID), 
		OrderInfo.FrontID, 
		OrderInfo.SessionID, 
		std::string((char*)OrderInfo.OrderRef),
		THOST_FTDC_OST_NotTouched);

	std::map <AllOrderKey,  PlatformStru_OrderInfo>::iterator it6=m_OrdersPanel.find(old6Key);
	
      if(it6!=m_OrdersPanel.end())
	  {
          m_OrdersPanel.erase(it6);
	  }
	  AllOrderKey old7Key(std::string(OrderInfo.InstrumentID), 
		OrderInfo.FrontID, 
		OrderInfo.SessionID, 
		std::string((char*)OrderInfo.OrderRef),
		THOST_FTDC_OST_Touched);

	std::map <AllOrderKey,  PlatformStru_OrderInfo>::iterator it7=m_OrdersPanel.find(old7Key);
	
      if(it7!=m_OrdersPanel.end())
	  {
          m_OrdersPanel.erase(it7);
	  }

	memcpy(&MyData,&OrderInfo,sizeof(PlatformStru_OrderInfo));

	wxString strtmp;
	if(MyData.ContingentCondition > THOST_FTDC_CC_ParkedOrder)
	{
		wxString strCondDir;
		switch(MyData.ContingentCondition) 
		{
		case THOST_FTDC_CC_LastPriceGreaterThanStopPrice:	///���¼۴���������
			strCondDir = wxT("���¼� > ");
			break;
		case THOST_FTDC_CC_LastPriceGreaterEqualStopPrice:	///���¼۴��ڵ���������
			strCondDir = wxT("���¼� >= ");
			break;
		case THOST_FTDC_CC_LastPriceLesserThanStopPrice:	///���¼�С��������
			strCondDir = wxT("���¼� < ");
			break;
		case THOST_FTDC_CC_LastPriceLesserEqualStopPrice:	///���¼�С�ڵ���������
			strCondDir = wxT("���¼� <= ");
			break;
		case THOST_FTDC_CC_AskPriceGreaterThanStopPrice:	///��һ�۴���������
			strCondDir = wxT("��һ�� > ");
			break;
		case THOST_FTDC_CC_AskPriceGreaterEqualStopPrice:	///��һ�۴��ڵ���������
			strCondDir = wxT("��һ�� >= ");
			break;
		case THOST_FTDC_CC_AskPriceLesserThanStopPrice:		///��һ��С��������
			strCondDir = wxT("��һ�� < ");
			break;
		case THOST_FTDC_CC_AskPriceLesserEqualStopPrice:	///��һ��С�ڵ���������
			strCondDir = wxT("��һ�� <= ");
			break;
		case THOST_FTDC_CC_BidPriceGreaterThanStopPrice:	///��һ�۴���������
			strCondDir = wxT("��һ�� > ");
			break;
		case THOST_FTDC_CC_BidPriceGreaterEqualStopPrice:	///��һ�۴��ڵ���������
			strCondDir = wxT("��һ�� >= ");
			break;
		case THOST_FTDC_CC_BidPriceLesserThanStopPrice:		///��һ��С��������
			strCondDir = wxT("��һ�� < ");
			break;
		case THOST_FTDC_CC_BidPriceLesserEqualStopPrice:	///��һ��С�ڵ���������
			strCondDir = wxT("��һ�� <= ");
			break;
		};
		wxString strPrice;
		if(MyData.OrderPriceType==THOST_FTDC_OPT_AnyPrice)
			strPrice.Printf("�м� ʱ����");
		else
			strPrice.Printf("%0.2f ʱ����", MyData.LimitPrice);
		strtmp=strCondDir+strPrice;
	}
	else 
	{
		strtmp=MyData.StatusMsg;
	}
	strcpy(MyData.StatusMsg,strtmp.c_str());


    //wxTimer m_timer;//��ʱ ��ѯ��Լ���������뱣֤����
	PlatformStru_InstrumentMarginRate MarginRateField;// �ӱ�֤��
	PlatformStru_InstrumentInfo InstrumentFields;  // �Ӻ�Լ�б���ȡ
	TThostFtdcRatioType d_RatioType;
	
	MyData.freezeMargin = 0.00;     //���ᱣ֤�� --���ͷ��֤����*ί�м�
	MyData.troubleMoney = 0.00;     //���������� --�����������ʼ���

	if ( MyData.OrderStatus == THOST_FTDC_OST_PartTradedQueueing	//���ֳɽ����ڶ����� '1'				
		|| MyData.OrderStatus == THOST_FTDC_OST_NoTradeQueueing	//δ�ɽ����ڶ����� '3'
		|| MyData.OrderStatus == THOST_FTDC_OST_Unknown )		//δ֪ -- ���ύ  ���ҵ� 'a'

	{
        int iVolumeMultiple = 0;//GetInstrumentInfo	
		if( ( g_pPlatform&&g_pPlatform->GetInstrumentInfo( MyData.InstrumentID, InstrumentFields) ) == 0 )//VolumeMultiple��Լ���� 
		{
			iVolumeMultiple = InstrumentFields.VolumeMultiple;
		}

		if( ( g_pPlatform&&g_pPlatform->GetMarginRate( MyData.InstrumentID, MarginRateField) ) == 0 )//��֤������
		{
			if ( MyData.Direction == THOST_FTDC_D_Buy ) {
                d_RatioType = MarginRateField.LongMarginRatioByMoney; ///��ͷ��֤����
				if (MarginRateField.LongMarginRatioByVolume > 0) {
					MyData.freezeMargin = MarginRateField.LongMarginRatioByVolume * MyData.VolumeTotal;//��ͷ��֤������
				}
			}else if ( MyData.Direction == THOST_FTDC_D_Sell ) {
				d_RatioType = MarginRateField.ShortMarginRatioByMoney;	 ///��ͷ��֤����
				if (MarginRateField.ShortMarginRatioByVolume > 0 ) {
					MyData.freezeMargin = MarginRateField.ShortMarginRatioByVolume * MyData.VolumeTotal;//��ͷ��֤������
				}
			}

			// ���ᱣ֤�� = ί�м۸� * δ�ɽ����� *  (��Լ����)* ��֤����
			if ( MyData.freezeMargin <= 0 ) {
				MyData.freezeMargin = MyData.LimitPrice * MyData.VolumeTotal * iVolumeMultiple * d_RatioType;
			}                
		} 
		else
		{
			MyData.freezeMargin =-1;
		}
	


		PlatformStru_InstrumentCommissionRate outData;
		
		if( ( g_pPlatform&&g_pPlatform->GetCommissionRate( MyData.InstrumentID, outData) ) == 0 )//��������
		{
            double d_RatioByMoney = 0.00;
			if ( MyData.CombOffsetFlag[0] == THOST_FTDC_OF_Open ) { //����
                d_RatioByMoney = outData.OpenRatioByMoney;
				if ( outData.OpenRatioByVolume > 0 ) {	//���������Ѱ�����
					MyData.troubleMoney =  MyData.VolumeTotal * outData.OpenRatioByVolume;
				} else {
					//  δ�ɽ����� * �����۸� * ��Լ���� * ������������
					MyData.troubleMoney =  MyData.VolumeTotal * MyData.LimitPrice * iVolumeMultiple * d_RatioByMoney;
				}
			}

			if ( MyData.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday ) { //ƽ�� -- ƽ��
				d_RatioByMoney = outData.CloseRatioByMoney;
				if ( outData.OpenRatioByVolume > 0 ) {	//���������Ѱ�����
					MyData.troubleMoney =  MyData.VolumeTotal * outData.CloseRatioByVolume;
				} else {
					//  δ�ɽ����� * �����۸� * ��Լ���� * ������������
					MyData.troubleMoney =  MyData.VolumeTotal * MyData.LimitPrice * iVolumeMultiple * d_RatioByMoney;
				}
			}
			if ( MyData.CombOffsetFlag[0] == THOST_FTDC_OF_Close 
				||MyData.CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose 
				||MyData.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday ) { //ƽ�� -- ��ͨƽ�ֺ�ƽ��
					d_RatioByMoney = outData.CloseRatioByMoney;
					if ( outData.CloseRatioByVolume > 0 ) {	//���������Ѱ�����      //OpenRatioByVolume��ΪCloseRatioByVolume, ����Ӧ���Ǽ���ƽ�������ѡ� modified by l. 2010.12.30
						MyData.troubleMoney =  MyData.VolumeTotal * outData.CloseRatioByVolume;
					} else {
						//  δ�ɽ����� * �����۸� * ��Լ���� * ������������
						MyData.troubleMoney =  MyData.VolumeTotal * MyData.LimitPrice * iVolumeMultiple * d_RatioByMoney;
					}
			}

		}
         else
		{
			MyData.freezeMargin =-1;

		}

	}

		
	if ( MyData.CombOffsetFlag[0] == THOST_FTDC_OF_Close 
		||MyData.CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose 
		||MyData.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday
		||MyData.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday )
	{MyData.freezeMargin=0.00;}/*ƽ��ʱ���ᱣ֤��Ϊ0*/

	m_OrdersPanel[tmpKey] = MyData;


	LeaveCriticalSection(&m_CS);

}


///��ȡȫ������
int CPlatformDataMgr::GetAllOrders(std::map<OrderKey,  PlatformStru_OrderInfo>& outData)
{
    LOG_INFO("CPlatformDataMgr::GetAllOrders");
    EnterCriticalSection(&m_CS);
    outData=m_allOrders;
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::GetAllOrders-end");
    return outData.size();
}

//��ȡָ����Լ��ָ������ָ����ƽ�Ĺҵ�������������
int  CPlatformDataMgr::GetWaitOrderVolume(const std::string &strInstrumentID, const char Direction, const char CloseType, const char HedgeFlag)
{
    LOG_INFO("CPlatformDataMgr::GetWaitOrderVolume");
	int tnVolume = 0;
    std::map<OrderKey,  PlatformStru_OrderInfo>::iterator iter;

    EnterCriticalSection(&m_CS);
    iter = m_allOrders.begin();
    for(;iter != m_allOrders.end(); ++iter)
    {
        if(iter->second.InstrumentID == strInstrumentID &&
            iter->second.Direction == Direction &&
            iter->second.CombOffsetFlag[0] == CloseType&&
            iter->second.CombHedgeFlag[0]==HedgeFlag&&
            (iter->second.OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
             iter->second.OrderStatus==THOST_FTDC_OST_NoTradeQueueing))
            tnVolume += iter->second.VolumeTotal;
    }
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::GetWaitOrderVolume-end");
	return tnVolume;
}

//��ȡָ����Լ��ͬ����Ĺҵ������ڲ����ã���������������Ͷ��
void  CPlatformDataMgr::GetWaitOrderVolume_Internal(std::map<std::string,WaitOrderVolumeStru>& WaitOrderVolume)
{
    LOG_INFO("CPlatformDataMgr::GetWaitOrderVolume_Internal");
    WaitOrderVolume.clear();
    std::map<OrderKey,  PlatformStru_OrderInfo>::iterator iter;
    std::map<std::string,WaitOrderVolumeStru>::iterator it_volume;
    WaitOrderVolumeStru BlankVolume;
    std::string strInstrumentID;

    memset(&BlankVolume,0,sizeof(BlankVolume));

    for(iter = m_allOrders.begin();iter != m_allOrders.end(); ++iter)
    {
        if(iter->second.OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
             iter->second.OrderStatus==THOST_FTDC_OST_NoTradeQueueing)
        {
            strInstrumentID=std::string(iter->second.InstrumentID);
            it_volume=WaitOrderVolume.find(strInstrumentID);
            if(it_volume==WaitOrderVolume.end())
            {
                WaitOrderVolume.insert(make_pair(strInstrumentID,BlankVolume));
                it_volume=WaitOrderVolume.find(strInstrumentID);
            }

            if(it_volume==WaitOrderVolume.end())
                continue;

            if(iter->second.CombHedgeFlag[0]==THOST_FTDC_HF_Speculation)
            {
                if(iter->second.Direction==THOST_FTDC_D_Buy)
                {
                    if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
                        it_volume->second.OpenVolume_Buy_Speculation+=iter->second.VolumeTotal;
                    else if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_Close||
                            iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
                            iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
                        it_volume->second.CloseVolume_Buy_Speculation+=iter->second.VolumeTotal;
                    else if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
                        it_volume->second.CloseTodayVolume_Buy_Speculation+=iter->second.VolumeTotal;
                    else if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
                        it_volume->second.CloseYesterdayVolume_Buy_Speculation+=iter->second.VolumeTotal;
                }
                else
                {
                    if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
                        it_volume->second.OpenVolume_Sell_Speculation+=iter->second.VolumeTotal;
                    else if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_Close||
                            iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
                            iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
                        it_volume->second.CloseVolume_Sell_Speculation+=iter->second.VolumeTotal;
                    else if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
                        it_volume->second.CloseTodayVolume_Sell_Speculation+=iter->second.VolumeTotal;
                    else if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
                        it_volume->second.CloseYesterdayVolume_Sell_Speculation+=iter->second.VolumeTotal;
                }
            }
            else if(iter->second.CombHedgeFlag[0]==THOST_FTDC_HF_Hedge)
            {
                if(iter->second.Direction==THOST_FTDC_D_Buy)
                {
                    if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
                        it_volume->second.OpenVolume_Buy_Hedge+=iter->second.VolumeTotal;
                    else if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_Close||
                            iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
                            iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
                        it_volume->second.CloseVolume_Buy_Hedge+=iter->second.VolumeTotal;
                    else if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
                        it_volume->second.CloseTodayVolume_Buy_Hedge+=iter->second.VolumeTotal;
                    else if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
                        it_volume->second.CloseYesterdayVolume_Buy_Hedge+=iter->second.VolumeTotal;
                }
                else
                {
                    if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
                        it_volume->second.OpenVolume_Sell_Hedge+=iter->second.VolumeTotal;
                    else if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_Close||
                            iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
                            iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
                        it_volume->second.CloseVolume_Sell_Hedge+=iter->second.VolumeTotal;
                    else if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
                        it_volume->second.CloseTodayVolume_Sell_Hedge+=iter->second.VolumeTotal;
                    else if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
                        it_volume->second.CloseYesterdayVolume_Sell_Hedge+=iter->second.VolumeTotal;
                }
            }
        }
    }
    LOG_INFO("CPlatformDataMgr::GetWaitOrderVolume_Internal-end");
}



//��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ��
//�ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
int  CPlatformDataMgr::GetCloseOrderVolume(const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
    LOG_INFO("CPlatformDataMgr::GetCloseOrderVolume");
    CloseVolume=CloseTodayVolume=CloseYesterdayVolume=0;
    std::map<OrderKey,  CThostFtdcOrderField>::iterator iter;

    int ret;
    EnterCriticalSection(&m_CS);
    ret=GetCloseOrderVolume_Internal(strInstrumentID,Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,HedgeFlag);
    LeaveCriticalSection(&m_CS);
    LOG_INFO("CPlatformDataMgr::GetCloseOrderVolume-end");
	return ret;
}

//��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ڲ����ã�������
//�ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
int  CPlatformDataMgr::GetCloseOrderVolume_Internal(const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
    LOG_INFO("CPlatformDataMgr::GetCloseOrderVolume_Internal");
    CloseVolume=CloseTodayVolume=CloseYesterdayVolume=0;
    std::map<OrderKey,  PlatformStru_OrderInfo>::iterator iter;

    iter = m_allOrders.begin();
    for(;iter != m_allOrders.end(); ++iter)
    {
        if(iter->second.InstrumentID == strInstrumentID &&
            iter->second.Direction == Direction &&
            iter->second.CombHedgeFlag[0]==HedgeFlag&&
            (iter->second.OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
             iter->second.OrderStatus==THOST_FTDC_OST_NoTradeQueueing))
        {
            if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_Close||
                iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
                iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
                CloseVolume += iter->second.VolumeTotal;
            if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
                CloseTodayVolume += iter->second.VolumeTotal;
            if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
                CloseYesterdayVolume += iter->second.VolumeTotal;
        }
    }
    LOG_INFO("CPlatformDataMgr::GetCloseOrderVolume_Internal-end");
	return 0;
}



//ȡȫ���ҵ��Ķ��������ѺͶ��ᱣ֤�����û�з��ʣ���0����
void CPlatformDataMgr::GetFreezeMargin_Commission_Internal(double& FreezeMargin,double& FreezeCommission)
{
    LOG_INFO("CPlatformDataMgr::GetFreezeMargin_Commission_Internal");
    FreezeMargin=0;
    FreezeCommission=0;

    std::map<OrderKey,  PlatformStru_OrderInfo>::iterator iter;
    PlatformStru_InstrumentMarginRate MarginRate;
    PlatformStru_InstrumentCommissionRate CommissionRate;
    PlatformStru_InstrumentInfo InstrumentInfo;
    std::string strInstrumentID;

    iter = m_allOrders.begin();
    for(;iter != m_allOrders.end(); ++iter)
    {
        if((iter->second.OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
             iter->second.OrderStatus==THOST_FTDC_OST_NoTradeQueueing)&&
             GetInstrumentInfo_Internal((strInstrumentID=std::string(iter->second.InstrumentID)),InstrumentInfo)==0)
        {
            if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_Open&&
                GetMarginRate_Internal(strInstrumentID,MarginRate)==0)
            {
                // ���ᱣ֤�� = ί�м۸� * δ�ɽ����� *  (��Լ����)* ��֤����
                if(iter->second.Direction==THOST_FTDC_D_Buy)
                {
                    FreezeMargin+=  iter->second.LimitPrice*iter->second.VolumeTotal*InstrumentInfo.VolumeMultiple*MarginRate.LongMarginRatioByMoney+
                                    iter->second.VolumeTotal*MarginRate.LongMarginRatioByVolume;
                }
                else
                {
                    FreezeMargin+=  iter->second.LimitPrice*iter->second.VolumeTotal*InstrumentInfo.VolumeMultiple*MarginRate.ShortMarginRatioByMoney+
                                    iter->second.VolumeTotal*MarginRate.ShortMarginRatioByVolume;
                }
            }

            //���㿪ƽ�ֵ���������
            if(GetCommissionRate_Internal(strInstrumentID,CommissionRate)==0)
            {
                if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
                    FreezeCommission+=  iter->second.LimitPrice*iter->second.VolumeTotal*InstrumentInfo.VolumeMultiple*CommissionRate.OpenRatioByMoney+
                                        iter->second.VolumeTotal*CommissionRate.OpenRatioByVolume;
                else if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_Close||
                        iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday||
                        iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
                        iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
                    FreezeCommission+=  iter->second.LimitPrice*iter->second.VolumeTotal*InstrumentInfo.VolumeMultiple*CommissionRate.CloseRatioByMoney+
                                        iter->second.VolumeTotal*CommissionRate.CloseRatioByVolume;
                else if(iter->second.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
                    FreezeCommission+=  iter->second.LimitPrice*iter->second.VolumeTotal*InstrumentInfo.VolumeMultiple*CommissionRate.CloseTodayRatioByMoney+
                                        iter->second.VolumeTotal*CommissionRate.CloseTodayRatioByVolume;
            }
        }
    }
    LOG_INFO("CPlatformDataMgr::GetFreezeMargin_Commission_Internal-end");
}






//ԭʼ�ֲ���ϸ��¼����
void CPlatformDataMgr::OriginPositionDetailReady(PlatformStru_Position PositionDetailInfo,const int ErrID,const bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr::OriginPositionDetailReady:InstrumentID=%s",
        PositionDetailInfo.InstrumentID);

    if(ErrID==0)
    {
		PositionDetailKey tmpKey(PositionDetailInfo);

        EnterCriticalSection(&m_CS);

		PositionDetailStru MyData;
        MyData.ThostData=PositionDetailInfo;
        MyData.ID=m_MaxPositionDetailID++;
        m_allPosition[tmpKey] = MyData;
        m_allPositionIndex.insert(make_pair(std::string((char*)PositionDetailInfo.InstrumentID),tmpKey));

        LeaveCriticalSection(&m_CS);
    }

    if(bIsLast)
    {
        EnterCriticalSection(&m_CS);
        m_bGetOriginPositionDetailData=true;
        LeaveCriticalSection(&m_CS);

        //ProcessNewTradeList();
    }

    LOG_INFO("CPlatformDataMgr::OriginPositionDetailReady-end");

}

//���¶�̬�ֲ���ϸ��¼�е�ӯ����ֵ���õ�������ʱ��һ��
void CPlatformDataMgr::UpdateProfitInDynamicPositionDetail(PlatformStru_DepthMarketData& QuotData) 
{
    LOG_INFO("CPlatformDataMgr::UpdateProfitInDynamicPositionDetail(�������):InstrumentID=%s",
        QuotData.InstrumentID);

	// ���ú�Լ�гֲ���Ϣ������¸óֲ���Ϣ
	std::multimap<std::string,PositionDetailKey>::iterator it_Index_lower = m_allPositionIndex.lower_bound( QuotData.InstrumentID );
	std::multimap<std::string,PositionDetailKey>::iterator it_Index_upper = m_allPositionIndex.upper_bound( QuotData.InstrumentID );
	for ( ; it_Index_lower != it_Index_upper; ++it_Index_lower)
	{
		std::map<PositionDetailKey,PositionDetailStru>::iterator it_Info = m_allPosition.find( it_Index_lower->second );
		if ( it_Info == m_allPosition.end())
		{
			continue;
		}

		PlatformStru_Position field;
		if (m_PlatFormService&&m_PlatFormService->GetPositionInfo( 
			it_Info->second.ThostData.szAccount,
			it_Info->second.ThostData.InstrumentID,
			field ))
		{
			EnterCriticalSection(&m_CS);
			PositionDetailStru positionDetail = it_Info->second;
			positionDetail.ThostData = field;
			m_allPosition[it_Index_lower->second] = positionDetail;
			LeaveCriticalSection(&m_CS);
		}
	}

    LOG_INFO("CPlatformDataMgr::UpdateProfitInDynamicPositionDetail-end");
}


//�ж�һ���ַ��������Ƿ��ǵ���
bool CPlatformDataMgr::IsTodayPosition(const char *OpenDate)
{
    int tradingday=0;

	if(OpenDate==NULL) return false;

	tradingday=atoi(m_exchangeTime.TradingDay);
    if(tradingday==0) 
    {
		time_t tCurdate = time(NULL);
		struct tm local;
		localtime_s(&local, &tCurdate);
		tradingday= ((local.tm_year+1900)*100+(local.tm_mon+1))*100+local.tm_mday;
    }
    return (tradingday==atoi(OpenDate));
}

///��ȡ����ί�е���Ϣ
int CPlatformDataMgr::GetAllOrdersEx(std::map <AllOrderKey,  PlatformStru_OrderInfo> &AllOrdersMap)
{
    LOG_INFO("CPlatformDataMgr::GetAllOrdersEx");
    AllOrdersMap.clear();

    EnterCriticalSection(&m_CS);

    AllOrdersMap=m_OrdersPanel;

    LeaveCriticalSection(&m_CS);

    LOG_INFO("CPlatformDataMgr::GetAllOrdersEx-end");
    return AllOrdersMap.size();
}

//��ȡ�ֲ���ϸ��¼
int CPlatformDataMgr::GetPositionDetails(std::multimap<std::string, PositionDetailStru > &PositionDetailMap)
{
    LOG_INFO("CPlatformDataMgr::GetPositionDetails1");
	PositionDetailMap.clear();

    LOG_INFO("CPlatformDataMgr::GetPositionDetails1-end");
    return PositionDetailMap.size();
}

//��ȡ�ֲ���ϸ��¼
int CPlatformDataMgr::GetPositionDetails(const std::string& InstrumentID,std::multimap<std::string, PositionDetailStru > &PositionDetailMap)
{
    LOG_INFO("CPlatformDataMgr::GetPositionDetails2");
    PositionDetailMap.clear();

    LOG_INFO("CPlatformDataMgr::GetPositionDetails2-end");
    return PositionDetailMap.size();
}


//��ȡ���к�Լ�ĳֲ���ϸ��¼
int  CPlatformDataMgr::GetPositionDetails(std::list<PositionDetailStru> &PositionDetailList)
{
    LOG_INFO("CPlatformDataMgr::GetPositionDetails");
    PositionDetailList.clear();

    LOG_INFO("CPlatformDataMgr::GetPositionDetails-end");
    return PositionDetailList.size();
}


//��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
int CPlatformDataMgr::GetPositionKeyList(std::list<PositionKey> &PositionKeyList)
{
    LOG_INFO("CPlatformDataMgr::GetPositionKeyList");
    PositionKeyList.clear();

    std::map<PositionDetailKey,PositionDetailStru>::iterator it;

    EnterCriticalSection(&m_CS);

    it=m_allPosition.begin();
    for(;it!=m_allPosition.end();++it)
    {
        if(it->second.ThostData.Volume>0)
        {
            PositionKey tmpKey(it->second.ThostData.szAccount, it->second.ThostData.InstrumentID,it->second.ThostData.Direction,it->second.ThostData.HedgeFlag);
            PositionKeyList.insert(PositionKeyList.end(),tmpKey);
        }
    }

    LeaveCriticalSection(&m_CS);

    LOG_INFO("CPlatformDataMgr::GetPositionKeyList-end");
    return PositionKeyList.size();
}

//��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
int CPlatformDataMgr::GetPositionKeySet(std::set<PositionKey> &PositionKeySet)
{
    LOG_INFO("CPlatformDataMgr::GetPositionKeySet");
    PositionKeySet.clear();

    std::map<PositionDetailKey,PositionDetailStru>::iterator it;

    EnterCriticalSection(&m_CS);

    it=m_allPosition.begin();
    for(;it!=m_allPosition.end();++it)
    {
        if(it->second.ThostData.Volume>0)
        {
            PositionKey tmpKey(it->second.ThostData.szAccount, it->second.ThostData.InstrumentID,it->second.ThostData.Direction,it->second.ThostData.HedgeFlag);
            PositionKeySet.insert(tmpKey);
        }
    }

    LeaveCriticalSection(&m_CS);

    LOG_INFO("CPlatformDataMgr::GetPositionKeySet-end");
    return PositionKeySet.size();
}


//��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr::GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PositionStru& PositionData)
{
    LOG_INFO("CPlatformDataMgr::GetPositionData");

	std::multimap<std::string,PositionDetailKey>::iterator it_index_lower = m_allPositionIndex.lower_bound( InstrumentID );
	std::multimap<std::string,PositionDetailKey>::iterator it_index_upper = m_allPositionIndex.upper_bound( InstrumentID );
	if ( it_index_lower == it_index_upper )
	{
		return -1;
	}

	bool bFind = false;
	for ( ; it_index_lower != it_index_upper; ++it_index_lower )
	{
		if ( it_index_lower->second.account == strAccount )
		{
			bFind = true;
			break;
		}
	}

	if ( !bFind )
	{
		return -1;
	}

	std::map<PositionDetailKey,PositionDetailStru>::iterator it_Info = m_allPosition.find( it_index_lower->second );
	if ( it_Info == m_allPosition.end())
	{
		return -1;
	}

	PlatformStru_Position field = it_Info->second.ThostData;
	strncpy(PositionData.InstrumentID,InstrumentID.c_str(),sizeof(PositionData.InstrumentID)-1);
	strncpy(PositionData.ExchangeID, field.ExchangeID, sizeof(PositionData.ExchangeID)-1);
	strncpy(PositionData.strAccount, field.szAccount, sizeof(PositionData.strAccount)-1);
	PositionData.Direction=Direction;
	PositionData.HedgeFlag=HedgeFlag;
	PositionData.TotalVolume = field.Volume;
	PositionData.dOpenProfit = field.dOpenProfit;
	PositionData.dOpenAvgPrice = field.dOpenAvgPrice;
	PositionData.nBuyVolumn = field.nBuyVolumn;
	PositionData.nSellVolumn = field.nSellVolumn;
	PositionData.dTotalProfit = field.dTotalProfit;
	PositionData.dMarginPerLot = field.dMarginPerLot;
	PositionData.dPLBurnRate = field.dPLBurnRate;
	PositionData.dOpenPositionExposure = field.dOpenPositionExposure;
	PositionData.dBuyPowerRemain = field.dBuyPowerRemain;
	PositionData.dBuyPowerUsed = field.dBuyPowerUsed;
	PositionData.dTotalMarginPaid = field.dTotalMarginPaid;
	PositionData.dCloseProfit = field.dTotalProfit - field.dOpenProfit;

    LOG_INFO("CPlatformDataMgr::GetPositionData-end");
    return 0;
}


//ȡȫ���ֲֵ�ռ�ñ�֤��ͳֲ�ӯ��(���ն���)��ƽ��ӯ��(���ն���)�����û�з��ʣ���0����
void CPlatformDataMgr::GetAllMargin_PositionProfitByDate_Internal(double& TotalMargin,double&TotalPositionProfitByDate,double&TotalCloseProfitByDate)
{
    LOG_INFO("CPlatformDataMgr::GetAllMargin_PositionProfitByDate_Internal");
    TotalMargin=0;
    TotalPositionProfitByDate=0;
    TotalCloseProfitByDate=0;

    std::map<PositionDetailKey,PositionDetailStru>::iterator iter = m_allPosition.begin();
    for(;iter != m_allPosition.end(); ++iter)
    {
        if(iter->second.ThostData.Volume>0)
        {
            TotalMargin+=iter->second.ThostData.Margin;
            TotalPositionProfitByDate+=iter->second.ThostData.PositionProfitByDate;
        }
        TotalCloseProfitByDate+=iter->second.ThostData.CloseProfitByDate;
    }
    LOG_INFO("CPlatformDataMgr::GetAllMargin_PositionProfitByDate_Internal-end");
}




//��ȡָ����Լ���飬�ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr::GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    int ret;

    EnterCriticalSection(&m_CS);
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
    LeaveCriticalSection(&m_CS);

    return ret;
}

//����ָ����Լ���飬���õ�������£��򷵻�true�����򷵻�false
//������Ϻ�Լ���������
bool CPlatformDataMgr::SetQuotInfo(const PlatformStru_DepthMarketData& inData)
{
    std::map<std::string,  PlatformStru_DepthMarketData>::iterator it = m_lastQuot.find(std::string(inData.InstrumentID));
    bool toBeContinue=true;

    EnterCriticalSection(&m_CS);

    if(it==m_lastQuot.end())
    {
        m_lastQuot[std::string(inData.InstrumentID)] = inData;
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
            LOG_INFO("CDataCenter::doProcessBusinessData:Old BID_RtnDepthMarketData InstrumentID=[%s],OldUpdateTime=[%s],NewUpdateTime=[%s]",
                inData.InstrumentID,
                     OldBusiness.UpdateTime,
                     inData.UpdateTime);
        }
    }
    LeaveCriticalSection(&m_CS);

    return toBeContinue;

}
void CPlatformDataMgr::ConvertComboInstrumentID(const std::string strComboID, std::string& strInstrumentID1, std::string& strInstrumentID2)
{
	char strTemp[128];
	char strCode1[128];
	char strCode2[128];
	ZeroMemory(strCode1, 128);
	ZeroMemory(strCode2, 128);
    sscanf_s(strComboID.c_str(), "%s %[^&]&%s", strTemp, sizeof(strTemp)-1, strCode1, sizeof(strCode1)-1, strCode2, sizeof(strCode2)-1);
	strInstrumentID1 = strCode1;
	strInstrumentID2 = strCode2;
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
		EnterCriticalSection(&m_CS);
		std::map<std::string,  PlatformStru_DepthMarketData>::iterator it =  m_lastQuot.find(InstrumentID);
		if(it!=m_lastQuot.end())
		{
            if(it->second.Volume>0) ret= it->second.LastPrice;
            else ret= it->second.PreSettlementPrice;
		}
		LeaveCriticalSection(&m_CS);
	}
    return ret;
}

//��ȡָ����Լ�����飬�ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr::GetOldQuotInfo (const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    int ret;

    EnterCriticalSection(&m_CS);

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

    LeaveCriticalSection(&m_CS);

    return ret;
}


//ȡͶ������Ϣ���ɹ�����0��ʧ�ܷ���-1
//int CPlatformDataMgr::GetInvestorInfo(DataRspQryInvestor& outData)
//{
//    int ret;
//    EnterCriticalSection(&m_CS);
//    if(m_InvestorInfo.Head.BID==BID_MAX)
//    {
//        memset(&outData,0,sizeof(outData));
//        outData.Head.BID=BID_MAX;  //���m_InvestorInfo��Ч
//        ret=-1;
//    }
//    else
//    {
//        outData=m_InvestorInfo;
//        ret=0; 
//    }
//    LeaveCriticalSection(&m_CS);
//
//    return ret;
//}

//����Ͷ������Ϣ
void CPlatformDataMgr::SetInvestorInfo(const DataRspQryInvestor& inData)
{
    EnterCriticalSection(&m_CS);
    m_InvestorInfo=inData;
    LeaveCriticalSection(&m_CS);
}
string CPlatformDataMgr::GetTradingDay()
{
	if(m_exchangeTime.CurTickCount>0)
		return string(m_exchangeTime.TradingDay);
	return string("");
}

//��ȡ������ʱ����Ϣ
unsigned long CPlatformDataMgr::GetExchangeTime(const string& ExchangeID,string& time)
{
    return 0;
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
//���ý�����ʱ����Ϣ
void CPlatformDataMgr::SetExchangeTime(const ExchangeTime& inData)
{
    m_exchangeTime=inData;
}



//��ȡ������Ϣ���ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr::GetLastSettlementInfo(std::string& outData)
{
    int ret;

    EnterCriticalSection(&m_CS);

    if(m_bSettlementInfoIsReady)
    {
        outData=m_LastSettlementInfo;
        ret=0;
    }
    else
        ret=-1;
    LeaveCriticalSection(&m_CS);

    return ret;
}

//���������Ϣ
void CPlatformDataMgr::ClearLastSettlementInfo(void)
{
    EnterCriticalSection(&m_CS);

    m_LastSettlementInfo.clear();
    m_bSettlementInfoIsReady=false;

    LeaveCriticalSection(&m_CS);
}

//��ӽ�����Ϣ
void CPlatformDataMgr::AppendLastSettlementInfo(const std::string& inData,bool bLast)
{
    EnterCriticalSection(&m_CS);

    if(m_bSettlementInfoIsReady)
    {
        m_LastSettlementInfo.clear();
        m_bSettlementInfoIsReady=false;
    }

    m_LastSettlementInfo.append(inData);
    if(bLast)
        m_bSettlementInfoIsReady=true;

    LeaveCriticalSection(&m_CS);

}


//ҵ���ƣ��ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr::SubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID, const GuiCallBackFunc& callback)
{
    int ret;

    if(BID>=BID_MAX || GID>=GID_MAX || callback==NULL)
        return -1;


    EnterCriticalSection(&m_CS);

    ret=0;
    try
    {
        m_subscribeInfo[BID][GID]=callback;
    }
    catch(...)
    {
        ret=-2;
    }
    LeaveCriticalSection(&m_CS);

    return ret;
}


///ȡ��ҵ�����ݵĶ��ƣ��ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr::UnSubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID)
{
    int ret;

    if(BID>=BID_MAX || GID>=GID_MAX)
        return -1;

    EnterCriticalSection(&m_CS);

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
    LeaveCriticalSection(&m_CS);

    return ret;
}


///��ȡҵ������Ϣ���ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr::GetSubscribeBusinessData(const BusinessID BID, std::list<GuiCallBackFunc>& outData)
{
    int ret=0;
    outData.clear();

    EnterCriticalSection(&m_CS);

    std::map<BusinessID, std::map<GUIModuleID, GuiCallBackFunc> >::iterator it1=m_subscribeInfo.find(BID);
    if(it1==m_subscribeInfo.end())
        ret=-1;
    else
    {
        std::map<GUIModuleID, GuiCallBackFunc>::iterator it2;
        for(it2=it1->second.begin(); it2!=it1->second.end(); ++it2)
        {
            if(it2->second)
            {
                outData.insert(outData.end(),it2->second);
            }
        }
        ret=0;
    }
	LeaveCriticalSection(&m_CS);

	return ret;
}
//������Ϻ�Լ�����������
int CPlatformDataMgr::MakeComboDepthMarketData(PlatformStru_DepthMarketData *pDepthMarketData,int index)
{
	std::string InstrumentID=pDepthMarketData->InstrumentID;
	stdext::hash_map<std::string,  PlatformStru_InstrumentInfo>::iterator it; 
	it =  m_allInstruments.find(InstrumentID);
    if(it==m_allInstruments.end())
        return -1;
	char clstype=it->second.ProductClass;
	if(clstype==THOST_FTDC_PC_Combination)
	{
		std::string id1,id2;
        PlatformStru_DepthMarketData field1,field2;
		ConvertComboInstrumentID(InstrumentID,id1,id2);
		if(GetQuotInfo(id1,field1)==0&&GetQuotInfo(id2,field2)==0)
		{
			double LastPrice1=util::isInvalidValue(field1.LastPrice)?field1.PreSettlementPrice:field1.LastPrice;
			double LastPrice2=util::isInvalidValue(field2.LastPrice)?field2.PreSettlementPrice:field2.LastPrice;
			pDepthMarketData->LastPrice=LastPrice1-LastPrice2;
			pDepthMarketData->AskPrice1=field1.AskPrice1-field2.BidPrice1;
			pDepthMarketData->BidPrice1=field1.BidPrice1-field2.AskPrice1;
			return 0;
		}
	}
	else if(clstype==THOST_FTDC_PC_Futures)
	{
		std::multimap<std::string,std::vector<std::string>>::iterator it;
		//��Ϲ�ϵ<ID1,<ALL,X,ID2>>,<ID2,<ALL,ID1>>
		if(m_CombRelation.count(InstrumentID)>0)
		{
			int indexcount=0;
			for (it=m_CombRelation.equal_range(InstrumentID).first; 
				it!=m_CombRelation.equal_range(InstrumentID).second; ++it)
			{
				if(indexcount==index)
				{
					std::vector<std::string>& vec=it->second;			
					PlatformStru_DepthMarketData combfield;
					if(vec.size()==3)
					{
						PlatformStru_DepthMarketData field2;
						if(GetQuotInfo(vec[0],combfield)==0&&GetQuotInfo(vec[2],field2)==0)
						{
                			double LastPrice1=util::isInvalidValue(pDepthMarketData->LastPrice)?
								pDepthMarketData->PreSettlementPrice:pDepthMarketData->LastPrice;
							double LastPrice2=util::isInvalidValue(field2.LastPrice)?field2.PreSettlementPrice:field2.LastPrice;            
							combfield.LastPrice=LastPrice1-LastPrice2;
							combfield.AskPrice1=pDepthMarketData->AskPrice1-field2.BidPrice1;
							combfield.BidPrice1=pDepthMarketData->BidPrice1-field2.AskPrice1;
						}
                        *pDepthMarketData=combfield;
						return 1;
					}
					else if(vec.size()==2)
					{
						PlatformStru_DepthMarketData field1;
						if(GetQuotInfo(vec[0],combfield)==0&&GetQuotInfo(vec[1],field1)==0)
						{
        //                    double LastPrice1=util::isInvalidValue(field1.LastPrice)?field1.PreSettlementPrice:field1.LastPrice;            
        //        			double LastPrice2=util::isInvalidValue(pDepthMarketData->LastPrice)?
								//pDepthMarketData->PreSettlementPrice:pDepthMarketData->LastPrice;
							combfield.LastPrice=field1.LastPrice-pDepthMarketData->LastPrice;
							combfield.AskPrice1=field1.BidPrice1-pDepthMarketData->AskPrice1;
							combfield.BidPrice1=field1.AskPrice1-pDepthMarketData->BidPrice1;
						}
                        *pDepthMarketData=combfield;
						return 1;
					}
				}
				indexcount++;
			}
		}
	}
	return -1;
}
//��ȡ��Լ�б�󣬳�ʼ����Ϲ�ϵmap
int CPlatformDataMgr::InitCombRelation(std::vector<DataRtnDepthMarketData>& vec)
{
    EnterCriticalSection(&m_CS);
	std::set<std::string>::iterator it;
	DataRtnDepthMarketData tmp;
	for(it=m_allInstrumentList.begin();it!=m_allInstrumentList.end();++it)
	{
		if(GetProductClassType(*it)==THOST_FTDC_PC_Combination)
		{
			std::string id1,id2;
			ConvertComboInstrumentID(*it,id1,id2);
			std::vector<std::string> vec1,vec2;
			vec1.push_back(*it);
			vec1.push_back(std::string());
			vec1.push_back(id2);
			m_CombRelation.insert(std::pair<std::string,std::vector<std::string>>(id1,vec1));
			vec2.push_back(*it);
			vec2.push_back(id1);
			m_CombRelation.insert(std::pair<std::string,std::vector<std::string>>(id2,vec2));
            PlatformStru_DepthMarketData MarketData2;
            memset(&MarketData2,0,sizeof(MarketData2));
			if(GetQuotInfo(it->c_str(),MarketData2)==0)
            {
                tmp.DepthMarketDataField=MarketData2;
				MakeComboDepthMarketData(&(tmp.DepthMarketDataField));
            }
			else
			{
				strcpy(tmp.DepthMarketDataField.InstrumentID,it->c_str());
				MakeComboDepthMarketData(&(tmp.DepthMarketDataField));
			}
			vec.push_back(tmp);
		}
	}
    LeaveCriticalSection(&m_CS);
	return 0;
}

const static string __UNKOWN__="δ֪";

///��ƽ��־
const string& OffsetFlag2String(char param)
{
	const static string Open="����";
	const static string Close="ƽ��";
	const static string ForceClose="ǿƽ";
	const static string CloseToday="ƽ��";
	const static string CloseYesterday="ƽ��";
	switch(param)
	{
		case THOST_FTDC_OF_Open:		return Open;
		case THOST_FTDC_OF_Close:		return Close;
		case THOST_FTDC_OF_ForceClose:	return ForceClose;
		case THOST_FTDC_OF_CloseToday:	return CloseToday;
		case THOST_FTDC_OF_CloseYesterday:return CloseYesterday;
		default: return __UNKOWN__;
	}
}

///��������
const string& Direction2String(char param)
{
	const static string Buy="��";
	const static string Sell="��";
	switch(param)
	{
		case THOST_FTDC_D_Buy:return Buy;
		case THOST_FTDC_D_Sell:return Sell;
		default: return __UNKOWN__;
	}
}

//��ȡ������
double CPlatformDataMgr::GetShouXuFei(const PlatformStru_TradeInfo& raw)
{
	double money;
	string name = raw.InstrumentID;
    PlatformStru_InstrumentCommissionRate  result;
	GetCommissionRate(name, result);
    PlatformStru_InstrumentInfo info;
	GetInstrumentInfo(name, info);
	switch( raw.OffsetFlag )
	{
		case THOST_FTDC_OF_Open:
            money = result.OpenRatioByVolume == 0? result.OpenRatioByMoney*raw.Price*info.VolumeMultiple:result.OpenRatioByVolume;
		     break;
		case THOST_FTDC_OF_Close:
		case THOST_FTDC_OF_ForceClose:
		case THOST_FTDC_OF_CloseYesterday:
			money = result.CloseRatioByVolume ==0? result.CloseRatioByMoney*raw.Price*info.VolumeMultiple : result.CloseRatioByVolume;
			break;
		case THOST_FTDC_OF_CloseToday:
            money = result.CloseTodayRatioByVolume ==0? result.CloseTodayRatioByMoney*raw.Price*info.VolumeMultiple:result.CloseTodayRatioByVolume;
			break;
		default:
			money = 0;
			break;
	}
    return money*raw.Volume;
}

int CPlatformDataMgr::FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData )
{
    EnterCriticalSection(&m_CS);
	for( int i=0; i<(int)m_TradeTotalData.size(); i++ )
	{    
		if( m_TradeTotalData[i].Instrument == string(rawData.InstrumentID) &&
				m_TradeTotalData[i].OpenClose == OffsetFlag2String(rawData.OffsetFlag).c_str() &&
				m_TradeTotalData[i].BuySell == Direction2String(rawData.Direction).c_str()) {
			LeaveCriticalSection(&m_CS);
			return i;
		}
	}
	LeaveCriticalSection(&m_CS);
	return -1;
}

void CPlatformDataMgr::SumTradeData(const PlatformStru_TradeInfo& rawData)
{
	TotalInfo  info;
	//m_TotalData.size()
	int i = FindIndexFromAllTradeTotalDatas(rawData);
	if( i ==-1)
	{
		info.EcChangeID = rawData.ExchangeID;
		info.Instrument = rawData.InstrumentID;
		info.price      = rawData.Price;
		info.ShouXuFei  = GetShouXuFei(rawData);
		info.volume     = rawData.Volume;
        info.OpenClose  = OffsetFlag2String(rawData.OffsetFlag).c_str();
        info.BuySell    = Direction2String(rawData.Direction).c_str();
		
		EnterCriticalSection(&m_CS);
		m_TradeTotalData.push_back(info);
		LeaveCriticalSection(&m_CS);
	}
    
	else
	{
		double fShouXuFei =  GetShouXuFei(rawData);
		EnterCriticalSection(&m_CS);
		m_TradeTotalData[i].price  = (m_TradeTotalData[i].price * m_TradeTotalData[i].volume 
				+ rawData.Volume * rawData.Price)/(m_TradeTotalData[i].volume + rawData.Volume);
	    m_TradeTotalData[i].volume = m_TradeTotalData[i].volume + rawData.Volume;
		m_TradeTotalData[i].ShouXuFei += fShouXuFei;
		LeaveCriticalSection(&m_CS);
	}
}
SERVER_PARAM_CFG&  CPlatformDataMgr::GetServerParamCfg()
{
	return m_ServerParamCfg;
}

void CPlatformDataMgr::RefreshPositionData()
{
	std::vector<PlatformStru_Position> vecPosition;
	if(m_PlatFormService)
		m_PlatFormService->GetAllPositionData(vecPosition);

	EnterCriticalSection(&m_CS);
	m_allPositionIndex.clear();
	m_allPosition.clear();
	for ( int i = 0; i < (int)vecPosition.size(); i++)
	{
		PlatformStru_Position position = vecPosition.at(i);
		PositionKey tmpKey( position);

		PositionDetailStru MyData;
		MyData.ThostData=position;
		MyData.ID=m_MaxPositionDetailID++;
		m_allPosition[tmpKey] = MyData;
		m_allPositionIndex.insert(make_pair(position.InstrumentID,tmpKey));
	}

	LeaveCriticalSection(&m_CS);
}


