#include "stdafx.h"

#include "ServiceProxy.hpp"
#include "PlatFormService.hpp"
#include "Module-Misc2/globalDefines.h"
#include "Module-Misc2/GlobalConfigMgr.h"
#include "Module-Misc2/SimpleWriteLog.h"
#include "Module-Misc2/GlobalFunc.h"




  
const static string __UNKOWN__ = "δ֪";
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CServiceProxy", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)

///����ƽ̨��PlatformID��ƽ̨ID
int CServiceProxy::Start()
{
    m_bWillStop=false;
    m_pWriteLog=new zqWriteLog();
	m_PlatFormService=new CPlatFormService(*this);
	m_pQryQueue->SetPlatFormService(m_PlatFormService);
    return 0;
}

///�ر�ƽ̨
int CServiceProxy::Stop()
{
    if(!m_bWillStop) m_bWillStop=true;
	int ret=Logout();
	SAFE_DELETE(m_PlatFormService);
	m_pQryQueue->SetPlatFormService(m_PlatFormService);
    SAFE_DELETE(m_pQryQueue);
    return ret;
}
int CServiceProxy::StopThread()
{
    if(!m_bWillStop) m_bWillStop=true;
	m_PlatFormService->StopThreads();
	m_pQryQueue->StopThread();
    
    return 0;
}
///�����Ƿ��ǵ�ǰ����
bool CServiceProxy::IsValidInvestorPW(const std::string& yourPW)
{
	return m_PlatFormService->IsValidInvestorPW(yourPW);
}
bool CServiceProxy::IsValidUserPW(const std::string& yourPW)
{
	return m_PlatFormService->IsValidUserPW(yourPW);
}
int CServiceProxy::GetBrokerIDUserID(std::string& UserID, std::string& BrokerID, set<string>* accounts)
{
	return m_PlatFormService->GetBrokerIDUserID(UserID,BrokerID,accounts);
}
string CServiceProxy::GetAccount()
{
	std::string UserID,BrokerID;
	GetBrokerIDUserID(UserID,BrokerID);
	return UserID;
}

int CServiceProxy::SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr)
{
    return m_PlatFormService->SetTradeFrontAddrs(TradeAddr);
}

///��������ǰ�û���ַ
int CServiceProxy::SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr)
{
    return m_PlatFormService->SetQuotFrontAddrs(QuotAddr);
}

///���þ��͹�˾������Ϣ
int CServiceProxy::SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, const std::string& UserPW, const std::string& OneTimePassword)
{
    return m_PlatFormService->SetParticipantInfo(BrokerID, UserID, UserPW, OneTimePassword);
}
//���ô���������ַ���
int CServiceProxy::SetProxyParam(const Stru_ProxyServerParam& ProxyParam)
{
    return m_PlatFormService->SetProxyParam(ProxyParam);
}

///����
int CServiceProxy::Login(const std::string& InvestorID, const std::string& InvestorPW, const std::string& InvestorNewPWD)
{
    if(m_isLogin)
        return 0;
    
    int ret=m_PlatFormService->LoginTrade(InvestorID, InvestorPW, InvestorNewPWD);
    if(ret<0)
        return ret;

    ret=m_PlatFormService->LoginQuot(InvestorID, InvestorPW);
    if(ret<0)
    {
        m_PlatFormService->LogoutTrade();
        return ret;
    }

    m_isLogin=true; 
    return ret;
}

///�ǳ�
int CServiceProxy::Logout()
{
    m_PlatFormService->LogoutTrade();
    m_PlatFormService->LogoutQuot();
    m_isLogin = false;
    return 0;
}

///����ҵ������
int CServiceProxy::SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo)
{
    return m_PlatFormService->SubscribeBusinessData(BID, GID, callback, sendCurrentInfo);
}

///ȡ��ҵ�����ݵĶ���
int CServiceProxy::UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID)
{
    return m_PlatFormService->UnSubscribeBusinessData(BID, GID);
}

///��ȡ�ײ����ĵ�ǰ״̬
ServiceStatus CServiceProxy::GetCurrentStatus(int* pQuotStatus,int* pTradeStatus)
{
    if(!m_isLogin)
        return SS_NOLOGIN;
    
    CPlatFormParam& RefPlatformParam=m_PlatFormService->m_PlatformParam;
    if(pQuotStatus) *pQuotStatus=RefPlatformParam.QuotStatus;
    if(pTradeStatus) *pTradeStatus=RefPlatformParam.TradeStatus;
    if(RefPlatformParam.TradeStatus==CTPCONNSTATUS_Connecting ||
       RefPlatformParam.TradeStatus==CTPCONNSTATUS_Logining   ||
       RefPlatformParam.TradeStatus==CTPCONNSTATUS_TradeSettlementInfoConfirming )
        return SS_LOGINING;
    else if(RefPlatformParam.TradeStatus==CTPCONNSTATUS_LoginFailure )
        return SS_LOGINFAILED;
    else if(RefPlatformParam.TradeStatus==CTPCONNSTATUS_Connected )
        return SS_LOGINED;
    else
        return SS_LOSTCONN;
}

///DataCenter�Ƿ��Ѿ��͵ײ�ͬ���ϣ����Կ�ʼ��ʼ����ѯ��
bool CServiceProxy::IsDataCenterReady(void)
{
    return m_PlatFormService ? m_PlatFormService->IsDataCenterReady() : false;
}

///��ȡ��Լ�б�
int CServiceProxy::GetInstrumentList(std::vector<InstrumentGroupInfo> &outData)
{
	int ret=m_PlatFormService->GetInstrumentList(outData);
	for(int i=0;i<(int)outData.size();i++)
	{
		outData[i].GroupName=ExchangeID2String(outData[i].GroupName);
	}
	return ret;
}
///��ȡ��Լ�б�
int CServiceProxy::GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID)
{
    return m_PlatFormService->GetInstrumentList(outData,ExchangeID);
}
//���������Լ�б�
void CServiceProxy::AddMainInstrumentList(std::string instrument)
{
	m_PlatFormService->AddMainInstrumentList(instrument);
}
///��ȡ������Լ�б�
int CServiceProxy::GetMainInstrumentList(std::set<std::string> &outData)
{
    return m_PlatFormService->GetMainInstrumentList(outData);
}
///���ú�Լ�ݲ��б�
void CServiceProxy::SetInstrumentVarietyMap(map<string, string>& inData)
{
    m_PlatFormService->SetInstrumentVarietyMap(inData);
}

// ��ȡ��Լ�ݲ��б�
int CServiceProxy::GetInstrumentVarietyMap(map<string, string>& outData)
{
    return m_PlatFormService->GetInstrumentVarietyMap(outData);
}

///��ȡָ����Լ��Ϣ
int CServiceProxy::GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    return m_PlatFormService->GetInstrumentInfo(InstrumentID, outData);
}
bool CServiceProxy::GetLegInstrumentID(const string& strComInstrumentID,
                                       string& strLeg1InstrumentID,
                                       string& strLeg2InstrumentID)
{
    return m_PlatFormService->GetLegInstrumentID(strComInstrumentID,strLeg1InstrumentID,strLeg2InstrumentID);
}
    //���ú�Լ����״̬
void CServiceProxy::SetSubscribeStatus(const std::string& InstrumentID,int GID,SubscribeMarketDataOper oper)
{
   m_PlatFormService->SetSubscribeStatus(InstrumentID,GID,oper);
}
//�˶�GID��Ӧ��ȫ����Լ����
void CServiceProxy::UnsubscribeMarketDataOfGID(int GID)
{
   m_PlatFormService->UnsubscribeMarketDataOfGID(GID);
}
//���GID�Ƿ�����ָ����Լ������
bool CServiceProxy::IsSubscribeMarketDataOfGID(const string& InstrumentID,int GID)
{
   return m_PlatFormService->IsSubscribeMarketDataOfGID(InstrumentID,GID);
}

///����ָ����Լ��Ϣ
void CServiceProxy::SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    m_PlatFormService->SetInstrumentInfo(InstrumentID, outData);
}
///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
char CServiceProxy::GetProductClassType(const std::string& InstrumentID)
{
    return m_PlatFormService->GetProductClassType(InstrumentID);
}

///��ȡ��ԼƷ�ִ���
int CServiceProxy::GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID)
{
    return m_PlatFormService->GetProductID(outData,ExchangeID);
}

///��ȡ��Լ��������
int CServiceProxy::GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    return m_PlatFormService->GetCommissionRate(InstrumentID, outData);
}

///��ȡ��Լ�������ʣ����û�У��򴥷���ѯ
int CServiceProxy::GetCommissionRate_TriggerReq(const std::string& InstrumentID)
{
    PlatformStru_InstrumentCommissionRate RateData;
    int ret;

    ret=m_PlatFormService->GetCommissionRate(InstrumentID, RateData);

    if(ret==0) return 0;
    else 
    {                                                       //�����ǰû�и÷��ʣ����һ������ѯ������
        CThostFtdcQryInstrumentCommissionRateField ReqData;
        memset(&ReqData,0,sizeof(ReqData));
        strncpy(ReqData.BrokerID,m_PlatFormService->GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
        strncpy(ReqData.InvestorID,m_PlatFormService->GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);;
        strncpy(ReqData.InstrumentID,InstrumentID.c_str(),sizeof(ReqData.InstrumentID)-1);

        if(m_pQryQueue)
            m_pQryQueue->ReqQryInstrumentCommissionRate(ReqData,0);
        return ret;
    }
}

///��ȡ��Լ��֤����
int CServiceProxy::GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    return m_PlatFormService->GetMarginRate(InstrumentID, outData);
}

///��ȡ��Լ��֤���ʣ����û�У��򴥷���ѯ
int CServiceProxy::GetMarginRate_TriggerReq(const std::string& InstrumentID)
{
    PlatformStru_InstrumentMarginRate RateData;
    int ret;
    
    if(InstrumentID.empty())
        return -1;
    ret=m_PlatFormService->GetMarginRate(InstrumentID, RateData);

    if(ret==0) return 0;
    else 
    {                                                       //�����ǰû�и÷��ʣ����һ������ѯ������
        CThostFtdcQryInstrumentMarginRateField ReqData;
        memset(&ReqData,0,sizeof(ReqData));
        strncpy(ReqData.BrokerID,m_PlatFormService->GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
        strncpy(ReqData.InvestorID,m_PlatFormService->GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);
        strncpy(ReqData.InstrumentID,InstrumentID.c_str(),sizeof(ReqData.InstrumentID)-1);
        ReqData.HedgeFlag=THOST_FTDC_HF_Speculation;

        if(m_pQryQueue)
            m_pQryQueue->ReqQryInstrumentMarginRate(ReqData,0);
        return ret;
    }
}

///���ú�Լ��������
int CServiceProxy::SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    return m_PlatFormService->SetCommissionRate(InstrumentID, outData);
}

///���ú�Լ��֤����
int CServiceProxy::SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    return m_PlatFormService->SetMarginRate(InstrumentID, outData);
}

///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
int CServiceProxy::GetInstrumentMultiple(const std::string& InstrumentID)
{
    return m_PlatFormService->GetInstrumentMultiple(InstrumentID);
}
///��ȡָ��Ʒ�ֵĺ�Լ�б�,���غ�Լ����
int CServiceProxy::GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData)
{
	return m_PlatFormService->GetInstrumentListByProductID(ProductID,outData);
}
///��ȡָ����Լ����
int CServiceProxy::GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    return m_PlatFormService->GetQuotInfo(InstrumentID, outData);
}

///��ȡָ����Լ������
int CServiceProxy::GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    return m_PlatFormService->GetOldQuotInfo(InstrumentID, outData);
}

///��ȡָ����Լ���ּۣ�ʧ�ܷ�����Чֵ
double CServiceProxy::GetCurPrice(const std::string& InstrumentID)
{
    return m_PlatFormService->GetCurPrice(InstrumentID);
}


int CServiceProxy::GetPositions2(const std::string& strInstrument,
                                 std::set<long>& setFTID,
                                 std::vector<PlatformStru_Position>& outData,
                                 long& lastseq)
{
    return m_PlatFormService->GetPositions2(strInstrument,setFTID,outData,lastseq);
}

int CServiceProxy::GetPositions3(const std::string& strInstrument,
                                 std::vector<PlatformStru_Position>& outData,
                                 long& lastseq)
{
    return m_PlatFormService->GetPositions3(strInstrument,outData,lastseq);
}

int CServiceProxy::GetPositionDetails3(const std::string& strInstrument,
                                          std::set<long>& setFTID,
                                          std::vector<PlatformStru_PositionDetail>& vecValue,
                                          long& lastseq)
{
    return m_PlatFormService->GetPositionDetails3(strInstrument,setFTID,vecValue,lastseq);
}

int CServiceProxy::GetPositionCombs2(const std::string& strInstrument,std::vector<PlatformStru_Position>& outData)
{
    return m_PlatFormService->GetPositionCombs2(strInstrument,outData);
}

int CServiceProxy::GetPositions(std::vector<PlatformStru_Position>& outData)
{
    return m_PlatFormService->GetPositions(outData);
}

int CServiceProxy::GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq)
{
    return m_PlatFormService->GetPositionDetails(outData,lastseq);
}


int CServiceProxy::GetPositionCombs(std::vector<PlatformStru_Position>& outData)
{
    return m_PlatFormService->GetPositionCombs(outData);
}


bool CServiceProxy::HavePositionDetail(const std::string& strInstrumentID)
{
    return m_PlatFormService->HavePositionDetail(strInstrumentID);
}

//�Ƿ�����ϳֲ���ϸ����(TradeTypeΪ�������������)
bool CServiceProxy::HaveCombPositionDetail(void)
{
    return m_PlatFormService->HaveCombPositionDetail();
}
void CServiceProxy::GetDerivedInstrumentID_OnCloseTrade(set<string>& InstrumentIDs)
{
    m_PlatFormService->GetDerivedInstrumentID_OnCloseTrade(InstrumentIDs);
}

//��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
int CServiceProxy::GetPositionKeySet(std::set<PositionKey> &PositionKeySet)
{
    return m_PlatFormService->GetPositionKeySet(PositionKeySet);
}

//��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
int CServiceProxy::GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData)
{
    return m_PlatFormService->GetPositionData(strAccount,InstrumentID,Direction,HedgeFlag,PositionData);
}



int  CServiceProxy::GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag)
{
	return m_PlatFormService->GetWaitOrderVolume(strAccount, strInstrumentID, Direction, CloseType,HedgeFlag);
}

//��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
int CServiceProxy::GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
    return m_PlatFormService->GetCloseOrderVolume(strAccount,strInstrumentID, Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,HedgeFlag);
}


///��ȡ������ʱ��
string CServiceProxy::GetTradingDay(const string& ExchangeID)
{
    return m_PlatFormService->GetTradingDay(ExchangeID);
}

bool CServiceProxy::IsTodayPosition( const char* pOpenData )
{
	return m_PlatFormService->IsTodayPosition(pOpenData);
}

///��ȡ������ʱ��
unsigned long CServiceProxy::GetExchangeTime(const string& ExchangeID,string& time)
{
    return m_PlatFormService->GetExchangeTime(ExchangeID,time);
}
///��ȡȫ��������
int CServiceProxy::GetExchangeIDs(vector<string>& outData)
{
    return m_PlatFormService->GetExchangeIDs(outData);
}
int CServiceProxy::GetTradingAccountAvailable(double& fAvailable)
{
    return m_PlatFormService->GetTradingAccountAvailable(fAvailable);
}
int CServiceProxy::GetTradingAccountWithdrawQuota(double& fWithdrawQuota)
{
    return m_PlatFormService->GetTradingAccountWithdrawQuota(fWithdrawQuota);
}
int CServiceProxy::GetTradingAccountID(char* AccountID,int rltsize)
{
    return m_PlatFormService->GetTradingAccountID(AccountID,rltsize);
}
///��ȡ�����ʽ��˻���Ϣ
int CServiceProxy::GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& outData)
{
    return m_PlatFormService->GetTradingAccount(strAccount,outData);
}
///��ȡ�˻��ʽ��ı���Ϣ
int CServiceProxy::GetAccountText(std::string& outData,int language)
{
    return m_PlatFormService->GetAccountText(outData,language);
}

///�����ѯͶ���߽�������Ӧ
int CServiceProxy::GetLastSettlementInfo(std::string& outData)
{
    return m_PlatFormService->GetLastSettlementInfo(outData);
}

///���õ�ǰ��ʾ�ĳֲ����ݡ�1:�ֲ֣�2:�ֲ���ϸ��3:��ϳֲ�
void CServiceProxy::SetCurrentPositionContent(int PositionContentMode)
{
    return m_PlatFormService->SetCurrentPositionContent(PositionContentMode);
}

///��ȡȫ������
int CServiceProxy::GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetAllOrders(outData);
}
///��ȡȫ������
int CServiceProxy::GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetAllOrders2(strInstrument,outData);
}

int CServiceProxy::GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetTriggerOrders(outData);
}

///��ȡ��Լ��ص��Ѵ����ı���
int CServiceProxy::GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetTriggerOrders2(strInstrument,outData);
}

///��ȡָ���Ѵ����ı���
bool CServiceProxy::GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_PlatFormService->GetTriggerOrder(orderkey,outData);
}

///��ȡ�ѳɽ�����
int CServiceProxy::GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetTradedOrders(outData);
}

///��ȡ�ѳɽ�����
int CServiceProxy::GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetTradedOrders2(strInstrument,outData);
}

///��ȡָ�����ѳɽ�����
bool CServiceProxy::GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_PlatFormService->GetTradedOrder(orderkey,outData);
}

///��ȡ�ѳ����ʹ��󱨵�
int CServiceProxy::GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetCanceledOrders(outData);
}

///��ȡ�ѳ����ʹ��󱨵�
int CServiceProxy::GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetCanceledOrders2(strInstrument,outData);
}

///��ȡָ�����ѳ����ʹ��󱨵�
bool CServiceProxy::GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_PlatFormService->GetCanceledOrder(orderkey,outData);
}

///��ȡδ�ɽ�����
int CServiceProxy::GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetWaitOrders(outData);
}

///��ȡδ�ɽ�����
int CServiceProxy::GetWaitOrders3(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetWaitOrders3(strInstrument,outData);
}
///��ȡָ����Լ��ص�δ�ɽ�����, �������ֹ�����еı�������Լ�ǵ���Լ�������Ƕ�Ӧ������������ú�Լ����ϱ���
int CServiceProxy::GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetWaitOrders2(strInstrument,outData);
}

///��ȡָ����δ�ɽ�����
bool CServiceProxy::GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_PlatFormService->GetWaitOrder(orderkey,outData);
}

///��ȡָ������
bool CServiceProxy::GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData)
{
    return m_PlatFormService->GetOrder(inOrderKey,outData);
}
bool CServiceProxy::GetOrder2(const string& strOrderSysID,PlatformStru_OrderInfo& outData)
{
    return m_PlatFormService->GetOrder2(strOrderSysID,outData);
}




///��ȡȫ���ɽ���
int CServiceProxy::GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData)
{
    return m_PlatFormService->GetAllTrades(outData);
}
///��ȡָ���ɽ���Ϣ
bool CServiceProxy::GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData)
{
    return m_PlatFormService->GetTradeInfo(tradekey,outData);
}
///��ȡָ���ɽ���Ϣ
int CServiceProxy::GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData)
{
    return m_PlatFormService->GetTradesOfInstrument(strInstrument,outData);
}


///��ȡȫ���ɽ�ͳ�Ƽ�¼
int CServiceProxy::GetAllTradeTotals(std::vector<PlatformStru_TradeTotalInfo>& outData)
{
    return m_PlatFormService->GetAllTradeTotals(outData);
}

///��ȡָ����Լ�ĳɽ�ͳ�Ƽ�¼�ɹ�����0��ʧ�ܷ���-1
int CServiceProxy::GetTradeTotalOfInstrument(const std::string& strInstrument, std::vector<PlatformStru_TradeTotalInfo>& outData)
{
    return m_PlatFormService->GetTradeTotalOfInstrument(strInstrument,outData);
}

///��ȡȫ���ɽ�ͳ������
int CServiceProxy::GetAllTradeTotalDatas(vector<TotalInfo>& outData)
{
    return m_PlatFormService->GetAllTradeTotalDatas(outData);
}
///�ڳɽ�ͳ�Ʋ��Ҳ���rawData
int CServiceProxy::FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData )
{
    return m_PlatFormService->FindIndexFromAllTradeTotalDatas(rawData);
}

bool CServiceProxy::IsSettlementInfoConfirmed()
{
	return m_PlatFormService->IsSettlementInfoConfirmed();
}
//��ȡ�ͻ�ǩԼ������Ϣ
int CServiceProxy::GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData)
{
    return m_PlatFormService->GetContractBank(outData);
}
//��ѯ�����Ƿ�Ϊ��
bool CServiceProxy::IsQryQueueEmpty(void)
{
	if(!m_isLogin)  return false;

	if(m_pQryQueue)
		return m_pQryQueue->IsCacheEmpty();

	return false;
}


void CServiceProxy::ReConnetServer()
{
	Logout();
	CPlatFormParam& RefPlatformParam=m_PlatFormService->m_PlatformParam;
	Login(RefPlatformParam.InvestorID,RefPlatformParam.InvestorPW);
}
CServiceProxy::~CServiceProxy()
{
    if(m_pQryQueue)
    {
        delete m_pQryQueue;
        m_pQryQueue=NULL;
    }
    LOG_INFO("CServiceProxy::~CServiceProxy(����CServiceProxy����)");
    SAFE_DELETE(m_pWriteLog);
}

CServiceProxy::CServiceProxy(const string& strAccount,int PlatformID,int MaxPlatformID)
:   IPlatformSingleSvr(strAccount,PlatformID,MaxPlatformID)
    ,m_pQryQueue( NULL )
    ,m_PlatFormService( NULL )
    ,m_isLogin( false )
    ,m_QryFinished(false)
    ,m_qrystep(0)
    ,m_bInitQryStart(false)
    ,m_bInitQrySucc(false)
    ,m_bInitQryFail(false)
//, m_bCurrentAccount(false)
{
    m_pQryQueue=new CQryQueue();
}
void CServiceProxy::GetErrorField(int& ErrorID)
{
	m_PlatFormService->GetErrorField(ErrorID);
}
void CServiceProxy::GetErrorField(string& ErrMsg)
{
	m_PlatFormService->GetErrorField(ErrMsg);
}
void CServiceProxy::SetReqID(int nOrderReqID) {
	return m_PlatFormService->SetReqID(nOrderReqID);
};

long CServiceProxy::GetReqID() {
	return m_PlatFormService->GetReqID();
};

long CServiceProxy::GetNextReqID()
{
	return m_PlatFormService->GetNextReqID();
}
int  CServiceProxy::GetAccountList(std::vector<string>& AcctList)
{
	string UserID,BrokerID;
	m_PlatFormService->GetBrokerIDUserID(UserID,BrokerID);
	AcctList.clear();
	AcctList.push_back(UserID);
	return 1;
}
void CServiceProxy::FillQuotData( const PlatformStru_DepthMarketData& rawQuot, QuotFiled fieldID,
								 std::vector<std::string>& vec )
{
}


///������ID
const string CServiceProxy::ExchangeID2String(const string& ExchangeID)
{
	const string SHFE="������";
	const string CZCE="֣����";
	const string DCE="������";
	const string CFFEX="�н���";

	if(ExchangeID=="SHFE")
		return SHFE;
	else if(ExchangeID=="CZCE")
		return CZCE;
	else if(ExchangeID=="DCE")
		return DCE;
	else if(ExchangeID=="CFFEX")
		return CFFEX;
	else
		return __UNKOWN__;
}
const string CServiceProxy::ExchangeName2ID( const string& name)
{
	const static std::string strCZCE="֣������Լ";
	const static std::string strDCE="��������Լ";
	const static std::string strSHFE="��������Լ";
	const static std::string strCFFEX="�н�����Լ";
	const static std::string strDCECOMB="�������";   
	const static std::string strCZCECOMB="֣�����";

	if(name==strCZCE)
		return std::string("CZCE");
	if(name==strDCE)
		return std::string("DCE");
	if(name==strSHFE)
		return std::string("SHFE");
	if(name==strCFFEX)
		return std::string("CFFEX");
	if(name==strCZCECOMB)
		return std::string("CZCE_COMB");
	if(name==strDCECOMB)
		return std::string("DCE_COMB");
	return "";
}
SERVER_PARAM_CFG&  CServiceProxy::GetServerParamCfg()
{
	return m_PlatFormService->GetServerParamCfg();
}

bool CServiceProxy::IsModuleValid( int nModuleId )
{
	switch( nModuleId )
	{
	case GID_ACCOUNT:
	case GID_QUOT_TABLE:
	case GID_INSTRUMENT:
	case GID_OPEN_ORDER:
	case GID_ALL_ORDER:
	case GID_ORDER_INSERT_PANEL:
	case GID_POSITIONALL:
	case GID_POSITION:
	case GID_POSITIONDETAIL:
	case GID_THRADE_RECORD:
	case GID_THRADE_STATISTICS:
	case GID_QUERY_ORDER_BATCH:
	case GID_QUERY_ORDER_CONDITION:
	case GID_QUERY_ORDER_POSITION:
	case GID_QUERY_ORDER_PARKED:
	case GID_QUERYLOCAL_ORDER:
	case GID_QUERY_ORDER_POSIPL:
		return true;
	case GID_POSITIONCOMB:
        if(GlobalConfigManager::m_bUseComb)
            return true;
        else 
            return false;
	default:
		return false;
	}
}

bool CServiceProxy::IsFieldValid( int nModuleId, int nFieldId )
{
	switch( nModuleId )
	{
	case GID_QUOT_TABLE:
		{
			if ( nFieldId <= QUOT_FIELD_ID_Turnover )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	case GID_INSTRUMENT:
		if ( nFieldId == INSTRUMENT_FIELD_ID_CURRENCY||
			nFieldId == INSTRUMENT_FIELD_ID_TICKSPERPOINT||
			nFieldId == INSTRUMENT_FIELD_ID_TICKSIZE)
		{
			return false;
		}
		else
		{
			return true;
		}
	case GID_POSITION:
		{
			if ( nFieldId == POSITION_FIELD_ID_CURRENCY )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_ACCOUNT:
	{
			if ( nFieldId >= FUNDACCOUNT_MAX_COL_ID ||
				nFieldId == FUNDACCOUNT_EXPOSURE_COL_ID ||
				nFieldId == FUNDACCOUNT_BPUSED_COL_ID ||
				nFieldId == FUNDACCOUNT_BURNRATE_COL_ID||
				nFieldId == FUNDACCOUNT_BPREMAINING_COL_ID)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_ALL_ORDER:
		{
			if ( nFieldId == ALLORDER_d_avgPrice||
				 nFieldId == ALLORDER_UpdateTime)
				
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_OPEN_ORDER:
		{
			if ( nFieldId == OPENORDER_d_avgPrice||
				nFieldId == OPENORDER_UpdateTime)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_ORDER_INSERT_PANEL:
		{
			if ( nFieldId == ORDERNEWINPUT_INSTRUMENTID_ROW_ID ||
					nFieldId == ORDERNEWINPUT_BUYSELL_ROW_ID ||
					nFieldId == ORDERNEWINPUT_OPENCLOSE_ROW_ID ||
					nFieldId == ORDERNEWINPUT_PRICE_ROW_ID ||
					nFieldId == ORDERNEWINPUT_VOLUME_ROW_ID) {
				return true;
			}
			else {
				return false;
			}
		}
		break;
	default:
		return true;
	}
}

string	g_strShortNames;

void CServiceProxy::GetContractShortNames(string& strShortNames) 
{
	if(g_strShortNames.length()==0)
	{
		HANDLE hFile = NULL;

		std::string strFile;
		GlobalFunc::GetPath(strFile);
		strFile += "ProductSName.txt";

		hFile = ::CreateFile(strFile.c_str(), GENERIC_READ, 
                       FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, 
                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 
					   NULL);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			DWORD dwHighLen = 0;
			DWORD dwLen = ::GetFileSize(hFile, &dwHighLen);
			char* pBuff = new char[dwLen+8];
			ZeroMemory(pBuff, dwLen+8);
			::ReadFile(hFile, pBuff, dwLen, &dwLen, NULL);
			::CloseHandle(hFile);
			g_strShortNames = pBuff;
			delete[] pBuff;
		}
	}
	strShortNames = g_strShortNames;
}

string CServiceProxy::GetVersionIconString()
{
	std::string s="ULD_VERSTR_"+m_PlatFormService->GetServerParamCfg().VersionStr;
	std::transform(s.begin(), s.end(), s.begin(), toupper);
	return s;
}
	//�Ƿ�֧�ֲ��
bool CServiceProxy::SupportPlugins()
{
	std::string s=m_PlatFormService->GetServerParamCfg().VersionStr;
	return s.find("plugins")!=string::npos;
}
////�Ƿ��Ƕ��˺Ű汾
//bool CServiceProxy::IsMultipleAccounts()
//{
//    return GlobalConfigManager::m_bNetVersion_MultiAccount||GlobalConfigManager::m_bLocalVersion_MultiAccount;
//}
bool CServiceProxy::HaveOrderType(UIOrderType type)
{
	switch(type) {
	case UIOT_PANE_STANDARD:
		return GlobalConfigManager::IsOrderType_UIOT_PANE_STANDARD();
	case UIOT_PANE_BATCH:
		return GlobalConfigManager::IsOrderType_UIOT_PANE_CONDITION();
	case UIOT_PANE_CONDITION:
		return GlobalConfigManager::IsOrderType_UIOT_PANE_BATCH();
	case UIOT_PANE_FAKFOK:
		return GlobalConfigManager::IsOrderType_UIOT_PANE_FAKFOK();
	case UIOT_PANE_PROFIT:
		return GlobalConfigManager::IsOrderType_UIOT_PANE_PROFIT();
	case UIOT_PANE_STOPLIMIT:
		return GlobalConfigManager::IsOrderType_UIOT_PANE_STOPLIMIT();
	case UIOT_CMD_REPLACEORDER:
		return GlobalConfigManager::IsOrderType_UIOT_CMD_REPLACEORDER();
	case UIOT_CMD_SIMULATEMARKET:
		return GlobalConfigManager::IsOrderType_UIOT_CMD_SIMULATEMARKET();
	case UIOT_CMD_PARKED:
		return GlobalConfigManager::IsOrderType_UIOT_CMD_PARKED();
	case UIOT_ITEM_HEDGE:
		return GlobalConfigManager::IsOrderType_UIOT_ITEM_HEDGE();
	case UIOT_ITEM_OPENCLOSE:
		return GlobalConfigManager::IsOrderType_UIOT_ITEM_OPENCLOSE();
	case UIOT_ITEM_ACCOUNT:
		return GlobalConfigManager::IsOrderType_UIOT_ITEM_ACCOUNT();
	case UIOT_ITEM_CONDITION_LOCAL:
		return GlobalConfigManager::IsOrderType_UIOT_ITEM_CONDITION_LOCAL();
	case UIOT_ITEM_CONDITION_SERVER:
		return GlobalConfigManager::IsOrderType_UIOT_ITEM_CONDITION_SERVER();
	case UIOT_ITEM_PARKED_LOCAL:
		return GlobalConfigManager::IsOrderType_UIOT_ITEM_PARKED_LOCAL();
	case UIOT_ITEM_PARKED_SERVER:
		return GlobalConfigManager::IsOrderType_UIOT_ITEM_PARKED_SERVER();
	case UIOT_ITEM_PROFIT_LOCAL:
		return GlobalConfigManager::IsOrderType_UIOT_ITEM_PROFIT_LOCAL();
	case UIOT_ITEM_PROFIT_SERVER:
		return GlobalConfigManager::IsOrderType_UIOT_ITEM_PROFIT_SERVER();
	};
	return false;
}

//��ȡ������Ϣ
vector<string> CServiceProxy::GetCurrencys(const string& BankName)
{
    return m_PlatFormService->GetCurrencys(BankName);
}

///��ȡ�˺���Ϣ
int CServiceProxy::GetTrader(std::vector<PlatformStru_Trader> &outData) 
{
	return 0;
}
///��ȡ��������Ϣ
int CServiceProxy::GetExchange(std::vector<PlatformStru_Exchange> &outData)
{
	return 0;
}
///��ȡ��Լ��Ϣ
int CServiceProxy::GetContract(std::vector<PlatformStru_Contract> &outData)
{
	return 0;
}
///��ȡ��Ʒ��Ϣ
int CServiceProxy::GetCommodity(std::vector<PlatformStru_Commodity> &outData)
{
	return 0;
}
///��ȡ����������Ϣ
int CServiceProxy::GetOrderType(std::vector<PlatformStru_OrderType> &outData)
{
	return 0;
}
bool CServiceProxy::GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi)
{
	return false;
}
bool CServiceProxy::GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,
									PlatformStru_AccountPosition* posi)
{
	return false;
}
bool CServiceProxy::GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,
										PlatformStru_AccountPosition* posi)
{
	return false;
}

std::string CServiceProxy::GetLogonReason()
{
	return "";
}
void CServiceProxy::GetLinkStatus(int * pHost,int * pPrice)
{
	if(pHost)  *pHost=-1;
	if(pPrice) *pPrice=-1;
}
int CServiceProxy::GetReports(map<DWORD, CTPReport>& mapReport) 
{
	return 0;
}
double CServiceProxy::GetExchangeRate(char* currency)
{
	return 1.0;
}
void CServiceProxy::NotifyInitQryStart() 
{ 
    m_bInitQryStart=true;
    m_bInitQrySucc=false;
    m_bInitQryFail=false;
	//CQryQueue::NotifyStartStopInitQry(bStart); 
    m_PlatFormService->NotifyInitQryStart();
}
void CServiceProxy::NotifyInitQrySucc() 
{ 
    m_bInitQryStart=false;
    m_bInitQrySucc=true;
    m_bInitQryFail=false;
	//CQryQueue::NotifyStartStopInitQry(bStart); 
    m_PlatFormService->NotifyInitQrySucc();
}
void CServiceProxy::NotifyInitQryFail() 
{ 
    m_bInitQryStart=false;
    m_bInitQrySucc=false;
    m_bInitQryFail=true;
	//CQryQueue::NotifyStartStopInitQry(bStart); 
    //m_PlatFormService->NotifyStartStopInitQry(bStart);
}
bool CServiceProxy::IsInitQrySucc()
{
    return m_bInitQrySucc;
}

int CServiceProxy::GetAllOrderFTIDList( std::vector<long>& vec )
{
	return m_PlatFormService->GetAllOrderFTIDList(vec);
}

int CServiceProxy::GetAllOrderInfo( long lFTID, PlatformStru_OrderInfo& outData )
{
	return m_PlatFormService->GetAllOrderInfo(lFTID, outData);
}

int CServiceProxy::GetAllOrderInfo( OrderKey key, PlatformStru_OrderInfo& outData )
{
	return m_PlatFormService->GetAllOrderInfo(key, outData);
}

int CServiceProxy::GetPositionFTIDList( std::vector<long>& vec )
{
	return m_PlatFormService->GetPositionFTIDList(vec);
}

int CServiceProxy::GetPositionInfo( long lFTID, PlatformStru_Position& outData )
{
	return m_PlatFormService->GetPositionInfo(lFTID, outData);
}

int CServiceProxy::GetPositionInfo( PositionKey key, PlatformStru_Position& outData )
{
	return m_PlatFormService->GetPositionInfo(key, outData);
}

int CServiceProxy::GetPositionCombFTIDList( std::vector<long>& vec )
{
	return m_PlatFormService->GetPositionCombFTIDList(vec);
}

int CServiceProxy::GetPositionCombInfo( long lFTID, PlatformStru_Position& outData )
{
	return m_PlatFormService->GetPositionCombInfo(lFTID, outData);
}

//��ȡ�������ӵ�FrontID��SessionID��������ֵ�ڽ��׵�¼ʱ��ctp����
void CServiceProxy::GetTradingLoginInfo(int& FrontID,int& SessionID,string& UserID)
{
	return m_PlatFormService->GetTradingLoginInfo(FrontID,SessionID,UserID);
}

int CServiceProxy::GetGroupMaxContractNum()
{
    return GlobalConfigManager::MaxSubscribeCount;
}
bool CServiceProxy::ContractCountIsFit()
{
	return true;
}
//bool CServiceProxy::IsCurrentAccount()
//{
//	return m_bCurrentAccount;
//}
//void CServiceProxy::SetCurrentAccount(bool bCurrent)
//{
//	m_bCurrentAccount=bCurrent;
//}
int CServiceProxy::GetPlatformType()
{
	return PTYPE_CTP;

}
//bool CServiceProxy::GetQryFinished()
//{
//	return m_QryFinished;
//}
//void CServiceProxy::SetQryFinished(bool bFinished)
//{
//	m_QryFinished=bFinished;
//}
