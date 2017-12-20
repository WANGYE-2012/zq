#include "StdAfx.h"
#include "CmeApiSvr.h"
#include "PlatFormService.hpp"
#include "Module-Misc/globalDefines.h"

#define CHECK_LOGIN_STATUS()    \
	if(!isLogin)  return -999

const string __UNKOWN__ = "δ֪";
///����ƽ̨
int CCmeApiSvr::Start()
{
	m_PlatFormService=new CPlatFormService();
	return 0;
}

///�ر�ƽ̨
int CCmeApiSvr::Stop()
{
    m_PlatFormService->StopDataCenterThread();
	int ret=Logout();
	SAFE_DELETE(m_PlatFormService);
	return ret;
}

///�����Ƿ��ǵ�ǰ����
///1���ǣ�����������
int CCmeApiSvr::IsValidPW(const std::string& yourPW)
{
	return m_PlatFormService->IsValidPW(yourPW);
}
int CCmeApiSvr::GetBrokerIDUserID(std::string& UserID, std::string& BrokerID, set<string>* accounts)
{
	return m_PlatFormService->GetBrokerIDUserID(UserID,BrokerID,accounts);
}

///���ý���ǰ�û���ַ
int CCmeApiSvr::SetTradeFrontAddr(const std::string& addr)
{
	return m_PlatFormService->SetTradeFrontAddr(addr);
}

int CCmeApiSvr::SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr)
{
	return m_PlatFormService->SetTradeFrontAddrs(TradeAddr);
}

///��������ǰ�û���ַ
int CCmeApiSvr::SetQuotFrontAddr(const std::string& addr)
{
	return m_PlatFormService->SetQuotFrontAddr(addr);
}

int CCmeApiSvr::SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr)
{
	return m_PlatFormService->SetQuotFrontAddrs(QuotAddr);
}

///���þ��͹�˾������Ϣ
int CCmeApiSvr::SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, const std::string& UserPW, const std::string& OneTimePassword)
{
	return m_PlatFormService->SetParticipantInfo(BrokerID, UserID, UserPW, OneTimePassword);
}


///����
int CCmeApiSvr::Login(const std::string& InvestorID, const std::string& InvestorPW, const std::string& InvestorNewPwd)
{
	if(isLogin)
		return 0;

	int ret=m_PlatFormService->LoginTrade(InvestorID, InvestorPW, InvestorNewPwd);
	if(ret<0)
		return ret;

	ret=m_PlatFormService->LoginQuot(InvestorID, InvestorPW);
	if(ret<0)
	{
		m_PlatFormService->LogoutTrade();
		return ret;
	}

	isLogin=true; 
	return ret;
}

///�ǳ�
int CCmeApiSvr::Logout()
{
	m_PlatFormService->LogoutTrade();
	m_PlatFormService->LogoutQuot();
	isLogin = false;
	return 0;
}

///����ҵ������
int CCmeApiSvr::SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo)
{
	return m_PlatFormService->SubscribeBusinessData(BID, GID, callback, sendCurrentInfo);
}

///ȡ��ҵ�����ݵĶ���
int CCmeApiSvr::UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID)
{
	return m_PlatFormService->UnSubscribeBusinessData(BID, GID);
}

///��ȡ�ײ����ĵ�ǰ״̬
ServiceStatus CCmeApiSvr::GetCurrentStatus(int* pQuotStatus,int* pTradeStatus)
{
	if(!isLogin)
		return SS_NOLOGIN;

	CPlatFormParam& RefPlatformParam=m_PlatFormService->m_PlatformParam;
	if(pQuotStatus) *pQuotStatus=RefPlatformParam.QuotStatus;
	if(pTradeStatus) *pTradeStatus=RefPlatformParam.TradeStatus;
	if(RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_Connecting ||
		RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_Logining   ||
		RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_TradeSettlementInfoConfirming /*  ||
																							   RefPlatformParam.QuotStatus ==CPlatFormParam::CONNSTATUS_Connecting ||
																							   RefPlatformParam.QuotStatus ==CPlatFormParam::CONNSTATUS_Logining*/)
																							   return SS_LOGINING;
	else if(RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_LoginFailure /*||*/
		/*RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_TradeSettlementInfoConfirmFailure ||*/
		/*RefPlatformParam.QuotStatus ==CPlatFormParam::CONNSTATUS_LoginFailure*/)
		return SS_LOGINFAILED;
	else if(RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_Connected /*&&
																			   RefPlatformParam.QuotStatus ==CPlatFormParam::CONNSTATUS_Connected*/)
																			   return SS_LOGINED;
	else
		return SS_LOSTCONN;

}
///��ȡ��Լ�б�
int CCmeApiSvr::GetInstrumentList(std::vector<GroupInfo> &outData)
{
	int ret=m_PlatFormService->GetInstrumentList(outData);
	for(int i=0;i<outData.size();i++)
	{
		outData[i].GroupName=ExchangeID2String(outData[i].GroupName);
	}
	return ret;
}
///��ȡ��Լ�б�
int CCmeApiSvr::GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID)
{
	return m_PlatFormService->GetInstrumentList(outData,ExchangeID);
}
//���������Լ�б�
int CCmeApiSvr::AddMainInstrumentList(std::string instrument)
{
	return m_PlatFormService->AddMainInstrumentList(instrument);
}
///��ȡ������Լ�б�
int CCmeApiSvr::GetMainInstrumentList(std::set<std::string> &outData)
{
	return m_PlatFormService->GetMainInstrumentList(outData);
}
///���ú�Լ�ݲ��б�
void CCmeApiSvr::SetInstrumentVarietyMap(map<string, string>& inData)
{
	m_PlatFormService->SetInstrumentVarietyMap(inData);
}

// ��ȡ��Լ�ݲ��б�
int CCmeApiSvr::GetInstrumentVarietyMap(map<string, string>& outData)
{
	return m_PlatFormService->GetInstrumentVarietyMap(outData);
}

///��ȡָ����Լ��Ϣ
int CCmeApiSvr::GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
	return m_PlatFormService->GetInstrumentInfo(InstrumentID, outData);
}
//���ú�Լ����״̬
void CCmeApiSvr::SetSubscribeStatus(const std::string& InstrumentID,int status,S_Status oper)
{
	m_PlatFormService->SetSubscribeStatus(InstrumentID,status,oper);
}
//���º�Լ����״̬
int CCmeApiSvr::UpdateSubscribeStatus()
{
	return m_PlatFormService->UpdateSubscribeStatus();
}
///����ָ����Լ��Ϣ
void CCmeApiSvr::SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
	m_PlatFormService->SetInstrumentInfo(InstrumentID, outData);
}
///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
char CCmeApiSvr::GetProductClassType(const std::string& InstrumentID)
{
	return m_PlatFormService->GetProductClassType(InstrumentID);
}

///��ȡ��ԼƷ�ִ���
int CCmeApiSvr::GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID)
{
	return m_PlatFormService->GetProductID(outData,ExchangeID);
}

///��ȡ��Լ��������
int CCmeApiSvr::GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
	return m_PlatFormService->GetCommissionRate(InstrumentID, outData);
}

///��ȡ��Լ�������ʣ����û�У��򴥷���ѯ
int CCmeApiSvr::GetCommissionRate_TriggerReq(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
	int ret;

	ret=m_PlatFormService->GetCommissionRate(InstrumentID, outData);

	if(ret==0) return 0;
	else 
	{                                                       //�����ǰû�и÷��ʣ����һ������ѯ������
		CThostFtdcQryInstrumentCommissionRateField ReqData;
		memset(&ReqData,0,sizeof(ReqData));
		strncpy(ReqData.BrokerID,m_PlatFormService->GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
		strncpy(ReqData.InvestorID,m_PlatFormService->GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);;
		strncpy(ReqData.InstrumentID,InstrumentID.c_str(),sizeof(ReqData.InstrumentID)-1);;

		m_PlatFormService->ReqQryInstrumentCommissionRate(&ReqData,0);
		return ret;
	}

}


///��ȡ��Լ��֤����
int CCmeApiSvr::GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
	return m_PlatFormService->GetMarginRate(InstrumentID, outData);
}

///��ȡ��Լ��֤���ʣ����û�У��򴥷���ѯ
int CCmeApiSvr::GetMarginRate_TriggerReq(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
	int ret;

	ret=m_PlatFormService->GetMarginRate(InstrumentID, outData);

	if(ret==0) return 0;
	else 
	{                                                       //�����ǰû�и÷��ʣ����һ������ѯ������
		CThostFtdcQryInstrumentMarginRateField ReqData;
		memset(&ReqData,0,sizeof(ReqData));
		strncpy(ReqData.BrokerID,m_PlatFormService->GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
		strncpy(ReqData.InvestorID,m_PlatFormService->GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);
		strncpy(ReqData.InstrumentID,InstrumentID.c_str(),sizeof(ReqData.InstrumentID)-1);
		ReqData.HedgeFlag=THOST_FTDC_HF_Speculation;

		m_PlatFormService->ReqQryInstrumentMarginRate(&ReqData,0);
		return ret;
	}
}

///���ú�Լ��������
int CCmeApiSvr::SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
	return m_PlatFormService->SetCommissionRate(InstrumentID, outData);
}

///���ú�Լ��֤����
int CCmeApiSvr::SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
	return m_PlatFormService->SetMarginRate(InstrumentID, outData);
}

///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
int CCmeApiSvr::GetInstrumentMultiple(const std::string& InstrumentID)
{
	return m_PlatFormService->GetInstrumentMultiple(InstrumentID);
}
///��ȡָ��Ʒ�ֵĺ�Լ�б�,���غ�Լ����
int CCmeApiSvr::GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData)
{
	return m_PlatFormService->GetInstrumentListByProductID(ProductID,outData);
}
///��ȡָ����Լ����
int CCmeApiSvr::GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
	return m_PlatFormService->GetQuotInfo(InstrumentID, outData);
}

///��ȡָ����Լ������
int CCmeApiSvr::GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
	return m_PlatFormService->GetOldQuotInfo(InstrumentID, outData);
}

///��ȡָ����Լ���ּۣ�ʧ�ܷ���0.0
double CCmeApiSvr::GetCurPrice(const std::string& InstrumentID)
{
	return m_PlatFormService->GetCurPrice(InstrumentID);
}

///��ȡ����ί�е���Ϣ
int CCmeApiSvr::GetAllOrdersEx(std::map <AllOrderKey,  PlatformStru_OrderInfo> &AllOrdersMap)
{
	return m_PlatFormService->GetAllOrdersEx(AllOrdersMap);
}



int CCmeApiSvr::GetPositionDetails(std::multimap<std::string, PositionDetailStru> &PositionDetailMap)
{
	return m_PlatFormService->GetPositionDetails(PositionDetailMap);
}

int CCmeApiSvr::GetPositionDetails(const std::string& InstrumentID,std::multimap<std::string, PositionDetailStru> &PositionDetailMap)
{
	return m_PlatFormService->GetPositionDetails(InstrumentID,PositionDetailMap);
}

int  CCmeApiSvr::GetPositionDetails(std::list<PositionDetailStru> &PositionDetailList)
{
	return m_PlatFormService->GetPositionDetails(PositionDetailList);
}

//��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
int CCmeApiSvr::GetPositionKeyList(std::list<PositionKey> &PositionKeyList)
{
	return m_PlatFormService->GetPositionKeyList(PositionKeyList);
}

//��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
int CCmeApiSvr::GetPositionKeySet(std::set<PositionKey> &PositionKeySet)
{
	return m_PlatFormService->GetPositionKeySet(PositionKeySet);
}

//��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
int CCmeApiSvr::GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PositionStru& PositionData)
{
	return m_PlatFormService->GetPositionData(strAccount, InstrumentID,Direction,HedgeFlag,PositionData);
}



int  CCmeApiSvr::GetWaitOrderVolume(const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag)
{
	return m_PlatFormService->GetWaitOrderVolume(strInstrumentID, Direction, CloseType,HedgeFlag);
}

//��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
int CCmeApiSvr::GetCloseOrderVolume(const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
	return m_PlatFormService->GetCloseOrderVolume(strInstrumentID, Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,HedgeFlag);
}


///��ȡ������ʱ��
string CCmeApiSvr::GetTradingDay()
{
	return m_PlatFormService->GetTradingDay();
}
///��ȡ������ʱ��
unsigned long CCmeApiSvr::GetExchangeTime(const string& ExchangeID,string& time)
{
	return m_PlatFormService->GetExchangeTime(ExchangeID,time);
}
///��ȡȫ��������
int CCmeApiSvr::GetExchangeIDs(vector<string>& outData)
{
	return m_PlatFormService->GetExchangeIDs(outData);
}
int CCmeApiSvr::GetTradingAccountAvailable(double& fAvailable)
{
	return m_PlatFormService->GetTradingAccountAvailable(fAvailable);
}
int CCmeApiSvr::GetTradingAccountWithdrawQuota(double& fWithdrawQuota)
{
	return m_PlatFormService->GetTradingAccountWithdrawQuota(fWithdrawQuota);
}
int CCmeApiSvr::GetTradingAccountID(char* AccountID)
{
	return m_PlatFormService->GetTradingAccountID(AccountID);
}
///��ȡ�����ʽ��˻���Ϣ
int CCmeApiSvr::GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& outData)
{
	return m_PlatFormService->GetTradingAccount(strAccount, outData);
}
///��ȡ�˻��ʽ��ı���Ϣ
int CCmeApiSvr::GetAccountText(std::string& outData,int language)
{
	return m_PlatFormService->GetAccountText(outData,language);
}
///���ý����ʽ��˻���Ϣ
void CCmeApiSvr::SetTradingAccount(const PlatformStru_TradingAccountInfo& inData)
{
	m_PlatFormService->SetTradingAccount(inData);
}

///�����ѯͶ���߽�������Ӧ
int CCmeApiSvr::GetLastSettlementInfo(std::string& outData)
{
	return m_PlatFormService->GetLastSettlementInfo(outData);
}

///��ȡȫ������
int CCmeApiSvr::GetAllOrders(std::map<OrderKey,  PlatformStru_OrderInfo>& outData)
{
	return m_PlatFormService->GetAllOrders(outData);
}

///��ȡȫ���ɽ���
int CCmeApiSvr::GetAllTrades(std::map<TradeKey,  PlatformStru_TradeInfo>& outData)
{
	return m_PlatFormService->GetAllTrades(outData);
}
///��ȡȫ���ɽ�ͳ������
int CCmeApiSvr::GetAllTradeTotalDatas(vector<TotalInfo>& outData)
{
	return m_PlatFormService->GetAllTradeTotalDatas(outData);
}
///�ڳɽ�ͳ�Ʋ��Ҳ���rawData
int CCmeApiSvr::FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData )
{
	return m_PlatFormService->FindIndexFromAllTradeTotalDatas(rawData);
}
int CCmeApiSvr::InitCombRelation()
{
	return m_PlatFormService->InitCombRelation();
}
bool CCmeApiSvr::IsSettlementInfoConfirmed()
{
	return m_PlatFormService->IsSettlementInfoConfirmed();
}
///�����˺���Ϣ
int CCmeApiSvr::GetTrader(std::vector<PlatformStru_Trader> &outData) 
{
	return m_PlatFormService->GetTrader(outData);
}
///�����ѯ��������Ϣ
int CCmeApiSvr::GetExchange(std::vector<PlatformStru_Exchange> &outData) 
{
	return m_PlatFormService->GetExchange(outData);
}
///�����ѯ��Լ��Ϣ
int CCmeApiSvr::GetContract(std::vector<PlatformStru_Contract> &outData) 
{
	return m_PlatFormService->GetContract(outData);
}
///������Ʒ��Ϣ
int CCmeApiSvr::GetCommodity(std::vector<PlatformStru_Commodity> &outData) 
{
	return m_PlatFormService->GetCommodity(outData);
}
 ///��ȡ����������Ϣ
int CCmeApiSvr::GetOrderType(std::vector<PlatformStru_OrderType> &outData) 
{
	return m_PlatFormService->GetOrderType(outData);
}
bool CCmeApiSvr::GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi)
{
	return m_PlatFormService->GetTotalPosition(account,posi);
}
bool CCmeApiSvr::GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi)
{
	return m_PlatFormService->GetOpenPosition(InstrumentID,account,posi);
}
bool CCmeApiSvr::GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi)
{
	return m_PlatFormService->GetContractPosition(InstrumentID,account,posi);
}
/*--------------------------------------------------------------------------------------------------------
����ҵ���������첽�ģ��趨�ƾ���ҵ�����ݣ������յ�������
---------------------------------------------------------------------------------------------------------*/
///�û������������
int CCmeApiSvr::ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate &UserPasswordUpdate, int nRequestID)
{
	CHECK_LOGIN_STATUS();

	return m_PlatFormService->ReqUserPasswordUpdate(&UserPasswordUpdate, nRequestID);
}

///�ʽ��˻������������
int CCmeApiSvr::ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate &TradingAccountPasswordUpdate, int nRequestID)
{
	CHECK_LOGIN_STATUS();

	return m_PlatFormService->ReqTradingAccountPasswordUpdate(&TradingAccountPasswordUpdate, nRequestID);
}

///����¼������
int CCmeApiSvr::ReqOrderInsert(PlatformStru_InputOrder &InputOrder, int nRequestID)
{
	CHECK_LOGIN_STATUS();

	return m_PlatFormService->ReqOrderInsert(&InputOrder, nRequestID);
}

///Ԥ��¼������
int CCmeApiSvr::ReqParkedOrderInsert(PlatformStru_ParkedOrder &ParkedOrder, int nRequestID)
{
	CHECK_LOGIN_STATUS();

	return m_PlatFormService->ReqParkedOrderInsert(&ParkedOrder, nRequestID);
}

///Ԥ�񳷵�¼������
int CCmeApiSvr::ReqParkedOrderAction(PlatformStru_ParkedOrderAction &ParkedOrderAction, int nRequestID)
{
	CHECK_LOGIN_STATUS();

	return m_PlatFormService->ReqParkedOrderAction(&ParkedOrderAction, nRequestID);
}

///������������
int CCmeApiSvr::ReqOrderAction(PlatformStru_InputOrderAction &InputOrderAction, int nRequestID)
{
	CHECK_LOGIN_STATUS();


	return m_PlatFormService->ReqOrderAction(&InputOrderAction, nRequestID);
}

///��ѯ��󱨵���������
int CCmeApiSvr::ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume &QueryMaxOrderVolume, int nRequestID)
{
	CHECK_LOGIN_STATUS();


	return m_PlatFormService->ReqQueryMaxOrderVolume(&QueryMaxOrderVolume, nRequestID);
}

///Ͷ���߽�����ȷ��
int CCmeApiSvr::ReqSettlementInfoConfirm() 
{
	CHECK_LOGIN_STATUS();


	return m_PlatFormService->ReqSettlementInfoConfirm();
}

///����ɾ��Ԥ��
int CCmeApiSvr::ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder &RemoveParkedOrder, int nRequestID) 
{
	CHECK_LOGIN_STATUS();


	return m_PlatFormService->ReqRemoveParkedOrder(&RemoveParkedOrder, nRequestID);
}

///����ɾ��Ԥ�񳷵�
int CCmeApiSvr::ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction &RemoveParkedOrderAction, int nRequestID) 
{
	CHECK_LOGIN_STATUS();


	return m_PlatFormService->ReqRemoveParkedOrderAction(&RemoveParkedOrderAction, nRequestID);
}

///�����ѯ����
int CCmeApiSvr::ReqQryOrder(PlatformStru_QryOrder &QryOrder, int nRequestID) 
{
	CHECK_LOGIN_STATUS();


	return m_PlatFormService->ReqQryOrder(&QryOrder, nRequestID);
}

///�����ѯ�ɽ�
int CCmeApiSvr::ReqQryTrade(PlatformStru_QryTrade &QryTrade, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	return m_PlatFormService->ReqQryTrade(&QryTrade, nRequestID);
}

///�����ѯͶ���ֲ߳�
int CCmeApiSvr::ReqQryInvestorPosition(PlatformStru_QryInvestorPosition &QryInvestorPosition, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	return m_PlatFormService->ReqQryInvestorPosition(&QryInvestorPosition, nRequestID);
}

///�����ѯ�ʽ��˻�
int CCmeApiSvr::ReqQryTradingAccount(PlatformStru_QryTradingAccount &QryTradingAccount, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	return m_PlatFormService->ReqQryTradingAccount(&QryTradingAccount, nRequestID);
}
///�õ���ѯ�ʽ��˻�
//int CCmeApiSvr::GetTradingAccount((PlatformStru_TradingAccountInfo& Data) 
//{
//    CHECK_LOGIN_STATUS();
//    
//    return m_PlatFormService->ReqQryTradingAccount(&QryTradingAccount, nRequestID);
//}

///�����ѯͶ����
int CCmeApiSvr::ReqQryInvestor(PlatformStru_QryInvestor &QryInvestor, int nRequestID) 
{
	CHECK_LOGIN_STATUS();


	return m_PlatFormService->ReqQryInvestor(&QryInvestor, nRequestID);
}


///�����ѯ��Լ
int CCmeApiSvr::ReqQryInstrument(PlatformStru_QryInstrument &QryInstrument, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	m_PlatFormService->ReqQryInstrument(&QryInstrument,nRequestID);

	return 0;
}

///�����ѯ����
int CCmeApiSvr::ReqQryDepthMarketData(PlatformStru_QryDepthMarketData &QryDepthMarketData, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	m_PlatFormService->ReqQryDepthMarketData(&QryDepthMarketData,nRequestID);
	return 0;
}

///�����ѯͶ���߽�����
int CCmeApiSvr::ReqQrySettlementInfo(PlatformStru_QrySettlementInfo &QrySettlementInfo, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	return m_PlatFormService->ReqQrySettlementInfo(&QrySettlementInfo, nRequestID);
}

///�����ѯת������
int CCmeApiSvr::ReqQryTransferBank(PlatformStru_QryTransferBank &QryTransferBank, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	m_PlatFormService->ReqQryTransferBank(&QryTransferBank,nRequestID);
	return 0;
}

///�����ѯͶ���ֲ߳���ϸ
int CCmeApiSvr::ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail &QryInvestorPositionDetail, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	return m_PlatFormService->ReqQryInvestorPositionDetail(&QryInvestorPositionDetail, nRequestID);
}

///�����ѯ�ͻ�֪ͨ
int CCmeApiSvr::ReqQryNotice(PlatformStru_QryNotice &QryNotice, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	m_PlatFormService->ReqQryNotice(&QryNotice,nRequestID);
	return 0;
}

///�����ѯ������Ϣȷ��
int CCmeApiSvr::ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm &QrySettlementInfoConfirm, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	m_PlatFormService->ReqQrySettlementInfoConfirm(&QrySettlementInfoConfirm,nRequestID);
	return 0;
}

///�����ѯͶ���ֲ߳���ϸ
int CCmeApiSvr::ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail &QryInvestorPositionCombineDetail, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	m_PlatFormService->ReqQryInvestorPositionCombineDetail(&QryInvestorPositionCombineDetail,nRequestID);
	return 0;
}

///�����ѯת����ˮ
int CCmeApiSvr::ReqQryTransferSerial(PlatformStru_QryTransferSerial &QryTransferSerial, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	m_PlatFormService->ReqQryTransferSerial(&QryTransferSerial,nRequestID);
	return 0;
}

///�����ѯǩԼ����
int CCmeApiSvr::ReqQryContractBank(PlatformStru_QryContractBank &QryContractBank, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	m_PlatFormService->ReqQryContractBank(&QryContractBank,nRequestID);
	return 0;
}

///�����ѯ����ǩԼ��ϵ
int CCmeApiSvr::ReqQryAccountregister(PlatformStru_QryAccountRegister &pQryAccountregister, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	m_PlatFormService->ReqQryAccountregister(&pQryAccountregister,nRequestID);
	return 0;
}

///�����ѯԤ��
int CCmeApiSvr::ReqQryParkedOrder(PlatformStru_QryParkedOrder &QryParkedOrder, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	m_PlatFormService->ReqQryParkedOrder(&QryParkedOrder,nRequestID);
	return 0;
}

///�����ѯԤ�񳷵�
int CCmeApiSvr::ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction &QryParkedOrderAction, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	m_PlatFormService->ReqQryParkedOrderAction(&QryParkedOrderAction,nRequestID);
	return 0;
}




///�ڻ����������ʽ�ת�ڻ�����
int CCmeApiSvr::ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer &ReqTransfer, int nRequestID) 
{
	CHECK_LOGIN_STATUS();


	return m_PlatFormService->ReqFromBankToFutureByFuture(&ReqTransfer, nRequestID);
}

///�ڻ������ڻ��ʽ�ת��������
int CCmeApiSvr::ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer &ReqTransfer, int nRequestID) 
{
	CHECK_LOGIN_STATUS();


	return m_PlatFormService->ReqFromFutureToBankByFuture(&ReqTransfer, nRequestID);
}

///�ڻ������ѯ�����������
int CCmeApiSvr::ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount &ReqQueryAccount, int nRequestID) 
{
	CHECK_LOGIN_STATUS();

	m_PlatFormService->ReqQueryBankAccountMoneyByFuture(&ReqQueryAccount,nRequestID);
	return 0;
}

///��������
int CCmeApiSvr::SubscribeQuot(const std::string &InstrumentID)
{
	if(InstrumentID.empty())
		return -1;

	return m_PlatFormService->SubscribeMarketData(InstrumentID);
}

///ȡ������
int CCmeApiSvr::UnSubscribeQuot(const std::string &InstrumentID)
{
	if(InstrumentID.empty())
		return -1;

	return m_PlatFormService->UnSubscribeMarketData(InstrumentID);
}
void CCmeApiSvr::ReConnetServer()
{
	Logout();
	CPlatFormParam& RefPlatformParam=m_PlatFormService->m_PlatformParam;
	Login(RefPlatformParam.InvestorID,RefPlatformParam.InvestorPW);
}
CCmeApiSvr::~CCmeApiSvr()
{
}

CCmeApiSvr::CCmeApiSvr()
{
	isLogin=false;
	m_qrystep=0;
}
void CCmeApiSvr::GetErrorField(int& ErrorID)
{
	m_PlatFormService->GetErrorField(ErrorID);
}
void CCmeApiSvr::GetErrorField(string& ErrMsg)
{
	m_PlatFormService->GetErrorField(ErrMsg);
}
void CCmeApiSvr::SetReqID(int nOrderReqID) {
	return m_PlatFormService->SetReqID(nOrderReqID);
};

long CCmeApiSvr::GetReqID() {
	return m_PlatFormService->GetReqID();
};

long CCmeApiSvr::GetNextReqID()
{
	return m_PlatFormService->GetNextReqID();
}
int  CCmeApiSvr::GetAccountList(std::vector<string>& AcctList)
{
	return m_PlatFormService->GetAccountList(AcctList);
}

void CCmeApiSvr::FillQuotData( const PlatformStru_DepthMarketData& rawQuot, QuotFiled fieldID, std::vector<std::string>& vec )
{
	return m_PlatFormService->FillQuotData( rawQuot, fieldID, vec );
}

///������ID
const string CCmeApiSvr::ExchangeID2String(const string& ExchangeID)
{
	//const string STR_CME="֥�Ӹ���ҵ������";
	//const string STR_CME_CBT="֥�Ӹ罻����";
	//const string STR_LIFFE="�׶ع��ʽ�����Ȩ�ڻ�������";

	//if(ExchangeID=="CME"||ExchangeID=="CME_CBT"||ExchangeID=="CME2")
	//	return ExchangeID;
	//else
	//	return __UNKOWN__;
	return ExchangeID;
}
const string CCmeApiSvr::ExchangeName2ID( const string& name)
{
	std::vector<std::string> outData;
	m_PlatFormService->GetExchangeIDs(outData);
	std::vector<std::string>::iterator it=outData.begin();
	for(it;it!=outData.end();++it)
	{
		if(name==*it)
			return name;
	}
	return "";
}
SERVER_PARAM_CFG&  CCmeApiSvr::GetServerParamCfg()
{
	return m_PlatFormService->GetServerParamCfg();
}
int  CCmeApiSvr::GetModuleValid(vector<int>& ModuleIds)
{
	ModuleIds.clear();
	ModuleIds.push_back(GID_ACCOUNT);//�ʽ��˻�
	ModuleIds.push_back(GID_QUOT_TABLE);//���۱�
	ModuleIds.push_back(GID_INSTRUMENT);//��Լ�б�
	ModuleIds.push_back(GID_OPEN_ORDER);//δ�ɽ���
	ModuleIds.push_back(GID_ALL_ORDER);//����ί�е�
	ModuleIds.push_back(GID_ORDER_INSERT_PANEL);//�µ���
	ModuleIds.push_back(GID_POSITION);//�ֲ�
	//ModuleIds.push_back(GID_POSITION_DETAIL);//�ֲ���ϸ
	ModuleIds.push_back(GID_THRADE_RECORD);//�ɽ���¼
	ModuleIds.push_back(GID_QUERY_ORDER_BATCH);//������
	ModuleIds.push_back(GID_QUERY_ORDER_CONDITION);//��������
	ModuleIds.push_back(GID_QUERY_ORDER_POSITION);//��ӯ��
	ModuleIds.push_back(GID_QUERY_ORDER_PARKED);//Ԥ��
	return (int)ModuleIds.size();
}

bool CCmeApiSvr::IsModuleValid( int nModuleId )
{
	switch( nModuleId )
	{
	case GID_ACCOUNT:
	case GID_QUOT_TABLE:
	case GID_INSTRUMENT:
	case GID_OPEN_ORDER:
	case GID_ALL_ORDER:
	case GID_ORDER_INSERT_PANEL:
	case GID_POSITION:
	//case GID_POSITION_DETAIL:
	case GID_THRADE_RECORD:
	//case GID_QUERY_ORDER_BATCH:
	//case GID_QUERY_ORDER_CONDITION:
	//case GID_QUERY_ORDER_POSITION:
	//case GID_QUERY_ORDER_PARKED:
		return true;
	default:
		return false;
	}
}

bool CCmeApiSvr::IsFieldValid( int nModuleId, int nFieldId )
{
	switch( nModuleId )
	{
	case GID_QUOT_TABLE:
		{
			if ( nFieldId >= QUOT_FIELD_ID_MAX ||
				nFieldId == QUOT_FIELD_ID_OpenInterest ||
				nFieldId == QUOT_FIELD_ID_PreOpenInterest ||
				nFieldId == QUOT_FIELD_ID_Turnover||
				nFieldId == QUOT_FIELD_ID_LastVolume)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_INSTRUMENT:
		{
			if ( nFieldId == INSTRUMENT_FIELD_ID_OPENRATIO ||
				nFieldId == INSTRUMENT_FIELD_ID_CLOSERATIO ||
				nFieldId == INSTRUMENT_FIELD_ID_CLOSETODAYRATIO ||
				nFieldId == INSTRUMENT_FIELD_ID_OPENRATIOBYMONEY ||
				nFieldId == INSTRUMENT_FIELD_ID_CLOSERATIOBYMONEY ||
				nFieldId == INSTRUMENT_FIELD_ID_CLOSETODAYRATIOBYMONEY ||
				nFieldId == INSTRUMENT_FIELD_ID_LONGMARGINRATIOBYVOLUME ||
				nFieldId == INSTRUMENT_FIELD_ID_TICKSIZE ||
				nFieldId == INSTRUMENT_FIELD_ID_SHORTMARGINRATIOBYVOLUME ||
				nFieldId == INSTRUMENT_FIELD_ID_LONGMARGINRATIO ||
				nFieldId == INSTRUMENT_FIELD_ID_SHORTMARGINRATIO )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_THRADE_RECORD:
		{
			if ( nFieldId == TRADE_RECORD_FIELD_ID_OPENCLOSE ||
				nFieldId == TRADE_RECORD_FIELD_ID_INSURE )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_THRADE_STATISTICS:
		{
			if ( nFieldId == TRADE_RECORD_FIELD_ID_OPENCLOSE )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_POSITION:
		{
			if ( nFieldId == POSITION_FILED_ID_INSTRUMENTID ||
				nFieldId == POSITION_FILED_ID_TOTALPOSITION ||
				nFieldId == POSITION_FIELD_ID_LASTPRICE ||
				nFieldId == POSITION_FIELD_ID_EXCHANGE ||
				nFieldId == POSITION_FIELD_ID_CURRENCY ||
				nFieldId == POSITION_FIELD_ID_OPENPROFIT ||
				nFieldId == POSITION_FIELD_ID_OPENAVGPRICE ||
				nFieldId == POSITION_FIELD_ID_BUYVOLUMN ||
				nFieldId == POSITION_FIELD_ID_SELVOLUMN ||
				nFieldId == POSITION_FIELD_ID_TOTALPROFIT||
				nFieldId == POSITION_FIELD_ID_MARGINPERLOT||
				nFieldId == POSITION_FIELD_ID_PLBURNRATE||
				nFieldId == POSITION_FIELD_ID_OPENPOSITIONEXPOSURE||
				nFieldId == POSITION_FIELD_ID_BUYPOWERREMAIN||
				nFieldId == POSITION_FIELD_ID_BUYPOWERUSED||
				nFieldId == POSITION_FIELD_ID_TOTALMARGINPAID||
				nFieldId == POSITION_FIELD_ID_ACCOUNT ||
				nFieldId == POSITION_FIELD_ID_CLOSEPROFIT)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	case GID_POSITION_DETAIL:
		{
			if ( nFieldId == POSITION_DETAIL_FIELD_ID_INSTRUMENT )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	case GID_ACCOUNT:
		{
			if ( nFieldId ==FUNDACCOUNT_STATICPROFIT_COL_ID||
				nFieldId ==FUNDACCOUNT_POSITIONPROFIT_COL_ID||
				nFieldId ==FUNDACCOUNT_CLOSEPROFIT_COL_ID||
				nFieldId ==FUNDACCOUNT_DYNAMICPROFIT_COL_ID||
				nFieldId == FUNDACCOUNT_MARGINPAID_COL_ID ||
				nFieldId == FUNDACCOUNT_EXPOSURE_COL_ID ||
				nFieldId == FUNDACCOUNT_BPUSED_COL_ID ||
				nFieldId == FUNDACCOUNT_BURNRATE_COL_ID||
				nFieldId == FUNDACCOUNT_ACCOUNTID_COL_ID||
				nFieldId == FUNDACCOUNT_BPREMAINING_COL_ID||
                nFieldId == FUNDACCOUNT_TOTALPROFIT_COL_ID)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	case GID_ALL_ORDER:
		{
			if ( nFieldId == ALLORDER_UserForceClose ||
				nFieldId == ALLORDER_ForceCloseReason ||
				nFieldId == ALLORDER_UserProductInfo ||
				nFieldId == ALLORDER_CombOffsetFlag ||
				nFieldId == ALLORDER_d_freezeMargin ||
				nFieldId == ALLORDER_d_troubleMoney ||
				nFieldId == ALLORDER_OrderSubmitStatus ||
				nFieldId == ALLORDER_OrderType ||
				nFieldId == ALLORDER_OrderStatus ||
				nFieldId == ALLORDER_TimeCondition ||
				nFieldId == ALLORDER_CombHedgeFlag)
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
			if ( nFieldId == OPENORDER_CombHedgeFlag ||
				nFieldId == OPENORDER_ForceCloseReason ||
				nFieldId == OPENORDER_UserForceClose ||
				nFieldId == OPENORDER_CombOffsetFlag ||
				nFieldId == OPENORDER_d_freezeMargin ||
				nFieldId == OPENORDER_d_troubleMoney ||
				nFieldId == OPENORDER_OrderSubmitStatus ||
				nFieldId == OPENORDER_OrderType ||
				nFieldId == OPENORDER_OrderStatus ||
				nFieldId == OPENORDER_TimeCondition ||
				nFieldId == OPENORDER_UpdateTime ||
				nFieldId == OPENORDER_d_avgPrice ||
				nFieldId == OPENORDER_UserProductInfo)
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
					//nFieldId == ORDERNEWINPUT_OPENCLOSE_ROW_ID ||
					nFieldId == ORDERNEWINPUT_PRICE_ROW_ID ||
					nFieldId == ORDERNEWINPUT_VOLUME_ROW_ID ||
					nFieldId == ORDERNEWINPUT_TRADEACCOUNT_ROW_ID || 
					nFieldId == ORDERNEWINPUT_ORDERTYPE_ROW_ID ||
					nFieldId == ORDERNEWINPUT_LIMITPRICE_ROW_ID) {
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

void CCmeApiSvr::GetContractShortNames(string& strShortNames)
{
	if(m_strContractShortName.size()>0) {
		strShortNames = m_strContractShortName;
	}
	else {
		m_strContractShortName.clear();
		std::map<string, set<string>> outData;
		std::map<string, set<string>>::iterator it;
		int nCount = GetProductID(outData);   
		if(nCount == 0) return;
		for(it = outData.begin(); it != outData.end();) {
			m_strContractShortName += (it->first + ",");
			m_strContractShortName += (it->first + ",");
			m_strContractShortName += it->first;
			it++;
			if(it != outData.end())
				m_strContractShortName += ",";
		}
		for(int i=0; i<(int)m_strContractShortName.size(); i++) {
			if(m_strContractShortName[i]==' ')
				m_strContractShortName[i]='-';
		}
		strShortNames = m_strContractShortName;
	}
}
string CCmeApiSvr::GetVersionIconString()
{
	return "zqBMP_LOGIN_CME";
}
bool CCmeApiSvr::HaveOrderType(UIOrderType type)
{
	switch(type)
	{
	case UIOT_STANDARD:
		return true;
	case UIOT_BATCH:
		return false;
	case UIOT_CONDITION:
		return false;
	case UIOT_PROFIT:
		return false;
	case UIOT_HEDGE:
		return false;
	case UIOT_STOPLIMIT:
		return true;
	case UIOT_REPLACEORDER:
		return true;
	case UIOT_SIMULATEMARKET:
		return false;
	}
	return false;
}

std::string CCmeApiSvr::GetLogonReason()
{
	return m_PlatFormService->GetLogonReason();
}
void CCmeApiSvr::GetLinkStatus(int* pHost,int* pPrice)
{
	m_PlatFormService->GetLinkStatus(pHost,pPrice);
}
int CCmeApiSvr::GetReports( map<DWORD, Report>& mapReport )
{
	return m_PlatFormService->GetReports(mapReport);
}
double CCmeApiSvr::GetExchangeRate(char* currency)
{
	return m_PlatFormService->GetExchangeRate(currency);
}