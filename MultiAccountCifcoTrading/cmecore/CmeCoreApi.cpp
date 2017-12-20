#include "StdAfx.h"
#include "CmeCoreApi.h"
#include <string>
#include "../inc/Module-Misc/tools_util.h"

#include "ISvr/BusinessData.hpp"
#define TRADE_TIMER_ID  3400
#define QUOTE_TIMER_ID  3401
#define ELAPSE_TIME     8000
#define OVERTIME_LIMIT  3
static CCmeCoreApi* g_CmeCoreApi=NULL;
void CALLBACK TradeTimerProc(HWND hWnd, UINT uMsg, UINT_PTR,DWORD)
{
    g_CmeCoreApi->SendHeartbeat(true);
}
void CALLBACK QuotTimerProc(HWND hWnd, UINT uMsg, UINT_PTR,DWORD)
{
    g_CmeCoreApi->SendHeartbeat(false);
}
CCmeCoreApi::CCmeCoreApi():
m_pTradeDataWorker(NULL),
m_pQuotDataWorker(NULL),
m_pMdSpi(NULL),
m_pTraderSpi(NULL),
SubscribeUnsubscribeID(10000),
m_TradeOverTimeCount(0),
m_QuotOverTimeCount(0)
{
    g_CmeCoreApi=this;
}
CCmeCoreApi::~CCmeCoreApi()
{
}
CCmeCoreApi *CCmeCoreApi::CreateFtdcApi(const char *pszFlowPath, const bool bIsUsingUdp)
{
    return new CCmeCoreApi();
}
void CCmeCoreApi::Release()
{
}

void CCmeCoreApi::Init()
{
}
int CCmeCoreApi::Join()
{
    return 0;
}
const char * CCmeCoreApi::GetTradingDay()
{
    return "";
}
//��ʽΪTradeFrontAddr:TradeFrontPort;QuotFrontAddr:QuotFrontPort
void CCmeCoreApi::ParseAddress(char *pszFrontAddress,Stru_IPAddress& TradeFrontAddr,
                               Stru_IPAddress& QuotFrontAddr)
{
    std::string strHostAddr,strPriceAddr,strIp,strPort,strAddr(pszFrontAddress);
	int pos=strAddr.find(";");
	strHostAddr=strAddr.substr(0,pos);
    strPriceAddr=strAddr.substr(pos+1);
	pos=strHostAddr.rfind(":");
	strIp=strHostAddr.substr(0,pos);
	strPort=strHostAddr.substr(pos+1);
    TradeFrontAddr.IP=inet_addr(strIp.c_str());
    TradeFrontAddr.Port=atoi(strPort.c_str());

	pos=strPriceAddr.rfind(":");
	strIp=strPriceAddr.substr(0,pos);
	strPort=strPriceAddr.substr(pos+1);
    QuotFrontAddr.IP=inet_addr(strIp.c_str());
    QuotFrontAddr.Port=atoi(strPort.c_str());
}

void CCmeCoreApi::RegisterFront(char *pszFrontAddress)
{
    ParseAddress(pszFrontAddress,m_TradeFrontAddr,m_QuotFrontAddr);
    m_pTradeDataWorker=new CTradeDataWorker(Stru_IPAddress(),m_TradeFrontAddr,this);
    m_pQuotDataWorker=new CQuotDataWorker(Stru_IPAddress(),m_QuotFrontAddr,this);
}
void CCmeCoreApi::RegisterSpi(CPatsQuotSpi *pSpi)
{
    m_pMdSpi=pSpi;
}
void CCmeCoreApi::RegisterSpi(CPatsTraderSpi *pSpi)
{
    m_pTraderSpi=pSpi;
}
int CCmeCoreApi::SubscribeMarketData(char *ppInstrumentID[], int nCount)
{
    map<string,EPCID>::const_iterator iter;
    for(int i=0;i<nCount;++i)
    {
        iter=m_ContractName2ID.find(ppInstrumentID[i]);
        if(iter!=m_ContractName2ID.end())
        {
            m_SubscribeUnsubscribeMarketMap[SubscribeUnsubscribeID]=iter->second.ContractID;
            m_pQuotDataWorker->SubscribeMarketData(iter->second.ContractID,SubscribeUnsubscribeID++);
        }
    }
    return 0;
}
int CCmeCoreApi::UnSubscribeMarketData(char *ppInstrumentID[], int nCount)
{
    map<string,EPCID>::const_iterator iter;
    for(int i=0;i<nCount;++i)
    {
        iter=m_ContractName2ID.find(ppInstrumentID[i]);
        if(iter!=m_ContractName2ID.end())
        {
            m_SubscribeUnsubscribeMarketMap[SubscribeUnsubscribeID]=iter->second.ContractID;
             m_pQuotDataWorker->UnSubscribeMarketData(iter->second.ContractID,SubscribeUnsubscribeID++);
        }
    }
    return 0;
}
void CCmeCoreApi::SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType)
{
}

void CCmeCoreApi::SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType)
{
}
///�ͻ�����֤����
int CCmeCoreApi::ReqAuthenticate(CThostFtdcReqAuthenticateField *pReqAuthenticateField, int nRequestID) 
{
    return 0;
}


///�û���¼����
int CCmeCoreApi::ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID) 
{
    int ret=0;
    if(strlen(pReqUserLoginField->UserProductInfo)>0)
        ret=m_pTradeDataWorker->ReqUserLogin(pReqUserLoginField,nRequestID);
    else
        ret=m_pQuotDataWorker->ReqUserLogin(pReqUserLoginField,nRequestID);
    return ret;
}



///�ǳ�����
int CCmeCoreApi::ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID) 
{
    int ret=0;
    //if(strlen(pReqUserLoginField->UserProductInfo)>0)
        ret=m_pTradeDataWorker->ReqUserLogout(pUserLogout,nRequestID);
    //else
    //    ret=m_pQuotDataWorker->ReqUserLogout(pUserLogout,nRequestID);
    return ret;
}


///�û������������
int CCmeCoreApi::ReqUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID) 
{

    return m_pTradeDataWorker->ReqUserPasswordUpdate(pUserPasswordUpdate,nRequestID);
}


///�ʽ��˻������������
int CCmeCoreApi::ReqTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, int nRequestID) 
{
    return 0;
}


///����¼������
int CCmeCoreApi::ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nRequestID) 
{
    Stru_Base_OrderCmdInfo cmd;
    FillOrderCmdInfo(*pInputOrder,cmd);
    m_RequestID2OrderCmdFlag[nRequestID]=OrderCmdFlag_New;
    return m_pTradeDataWorker->ReqOrderCmd(cmd,nRequestID);
}


///Ԥ��¼������
int CCmeCoreApi::ReqParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, int nRequestID) 
{
    Stru_Base_OrderCmdInfo cmd;
    FillOrderCmdInfo(*pParkedOrder,cmd);
    m_RequestID2OrderCmdFlag[nRequestID]=OrderCmdFlag_New_Park;
    return m_pTradeDataWorker->ReqOrderCmd(cmd,nRequestID);
}


///Ԥ�񳷵�¼������
int CCmeCoreApi::ReqParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, int nRequestID) 
{
    Stru_Base_OrderCmdInfo cmd;
    FillOrderCmdInfo(*pParkedOrderAction,cmd);
    m_RequestID2OrderCmdFlag[nRequestID]=OrderCmdFlag_Cancel_Park;
    return m_pTradeDataWorker->ReqOrderCmd(cmd,nRequestID);
}


///������������
int CCmeCoreApi::ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, int nRequestID) 
{
    Stru_Base_OrderCmdInfo cmd;
    FillOrderCmdInfo(*pInputOrderAction,cmd);
    m_RequestID2OrderCmdFlag[nRequestID]=OrderCmdFlag_Cancel;
    return m_pTradeDataWorker->ReqOrderCmd(cmd,nRequestID);
}


///��ѯ��󱨵���������
int CCmeCoreApi::ReqQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, int nRequestID) 
{
    return 0;
}


///Ͷ���߽�����ȷ��
int CCmeCoreApi::ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID) 
{
   return m_pTradeDataWorker->ReqSettlementInfoConfirm(pSettlementInfoConfirm,nRequestID);
}


///����ɾ��Ԥ��
int CCmeCoreApi::ReqRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, int nRequestID) 
{
    return 0;
}


///����ɾ��Ԥ�񳷵�
int CCmeCoreApi::ReqRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, int nRequestID) 
{
    return 0;
}


///�����ѯ����
int CCmeCoreApi::ReqQryOrder(CThostFtdcQryOrderField *pQryOrder, int nRequestID) 
{
    return  m_pTradeDataWorker->ReqQryOrder(pQryOrder,nRequestID);
}


///�����ѯ�ɽ�
int CCmeCoreApi::ReqQryTrade(CThostFtdcQryTradeField *pQryTrade, int nRequestID) 
{
    return  m_pTradeDataWorker->ReqQryTrade(pQryTrade,nRequestID);
}


///�����ѯͶ���ֲ߳�
int CCmeCoreApi::ReqQryInvestorPosition(PlatformStru_QryInvestorPosition *pQryInvestorPosition, int nRequestID) 
{
    return 0;
}


///�����ѯ�ʽ��˻�
int CCmeCoreApi::ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID) 
{
   return m_pTradeDataWorker->ReqQryTradingAccount(pQryTradingAccount,nRequestID);
}
///�����ѯͶ����
int CCmeCoreApi::ReqQryInvestor(CThostFtdcQryInvestorField *pQryInvestor, int nRequestID) 
{
    return 0;
}


///�����ѯ���ױ���
int CCmeCoreApi::ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID) 
{
    return 0;
}


///�����ѯ��Լ��֤����
int CCmeCoreApi::ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID) 
{
    int ExchangeID=m_ContractName2ID[pQryInstrumentMarginRate->InstrumentID].ExchangeID;
    if(m_ReqCommissionMarginInfo.find(ExchangeID)==m_ReqCommissionMarginInfo.end())
    {
        m_ReqCommissionMarginInfo.insert(ExchangeID);
        m_pTradeDataWorker->ReqQryInstrumentCommissionRateMarginRate(ExchangeID,nRequestID);
    }
    return 0;
}


///�����ѯ��Լ��������
int CCmeCoreApi::ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID) 
{
    int ExchangeID=m_ContractName2ID[pQryInstrumentCommissionRate->InstrumentID].ExchangeID;
    if(m_ReqCommissionMarginInfo.find(ExchangeID)==m_ReqCommissionMarginInfo.end())
    {
        m_ReqCommissionMarginInfo.insert(ExchangeID);
        m_pTradeDataWorker->ReqQryInstrumentCommissionRateMarginRate(ExchangeID,nRequestID);
    }
    return 0;
}


///�����ѯ������
int CCmeCoreApi::ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID) 
{
    return 0;
}


///�����ѯ��Լ
int CCmeCoreApi::ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID) 
{
    return m_pTradeDataWorker->ReqQryInstrument(pQryInstrument,nRequestID);
}


///�����ѯ����
int CCmeCoreApi::ReqQryDepthMarketData(CThostFtdcQryDepthMarketDataField *pQryDepthMarketData, int nRequestID) 
{
    int ContractID=0;
    map<string,EPCID>::const_iterator iter;
    if(pQryDepthMarketData)
        iter=m_ContractName2ID.find(pQryDepthMarketData->InstrumentID);
    if(iter!=m_ContractName2ID.end())
        ContractID=iter->second.ContractID;
    return m_pTradeDataWorker->ReqQryDepthMarketData(ContractID,nRequestID);
}


///�����ѯͶ���߽�����
int CCmeCoreApi::ReqQrySettlementInfo(CThostFtdcQrySettlementInfoField *pQrySettlementInfo, int nRequestID) 
{
    return m_pTradeDataWorker->ReqQrySettlementInfo(pQrySettlementInfo,nRequestID);
}


///�����ѯת������
int CCmeCoreApi::ReqQryTransferBank(CThostFtdcQryTransferBankField *pQryTransferBank, int nRequestID) 
{
    return 0;
}


///�����ѯͶ���ֲ߳���ϸ
int CCmeCoreApi::ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail, int nRequestID) 
{
    return m_pTradeDataWorker->ReqQryInvestorPositionDetail(pQryInvestorPositionDetail,nRequestID);
}


///�����ѯ�ͻ�֪ͨ
int CCmeCoreApi::ReqQryNotice(CThostFtdcQryNoticeField *pQryNotice, int nRequestID) 
{
    return 0;
}


///�����ѯ������Ϣȷ��
int CCmeCoreApi::ReqQrySettlementInfoConfirm(CThostFtdcQrySettlementInfoConfirmField *pQrySettlementInfoConfirm, int nRequestID) 
{
    return m_pTradeDataWorker->ReqQrySettlementInfoConfirm(pQrySettlementInfoConfirm,nRequestID);
}


///�����ѯͶ���ֲ߳���ϸ
int CCmeCoreApi::ReqQryInvestorPositionCombineDetail(CThostFtdcQryInvestorPositionCombineDetailField *pQryInvestorPositionCombineDetail, int nRequestID) 
{
    return 0;
}


///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
int CCmeCoreApi::ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField *pQryCFMMCTradingAccountKey, int nRequestID) 
{
    return 0;
}


///�����ѯ�ֵ��۵���Ϣ
int CCmeCoreApi::ReqQryEWarrantOffset(CThostFtdcQryEWarrantOffsetField *pQryEWarrantOffset, int nRequestID) 
{
    return 0;
}


///�����ѯת����ˮ
int CCmeCoreApi::ReqQryTransferSerial(CThostFtdcQryTransferSerialField *pQryTransferSerial, int nRequestID) 
{
    return 0;
}


///�����ѯ����ǩԼ��ϵ
int CCmeCoreApi::ReqQryAccountregister(CThostFtdcQryAccountregisterField *pQryAccountregister, int nRequestID) 
{
    return 0;
}


///�����ѯǩԼ����
int CCmeCoreApi::ReqQryContractBank(CThostFtdcQryContractBankField *pQryContractBank, int nRequestID) 
{
    return 0;
}


///�����ѯԤ��
int CCmeCoreApi::ReqQryParkedOrder(CThostFtdcQryParkedOrderField *pQryParkedOrder, int nRequestID) 
{
    return 0;
}


///�����ѯԤ�񳷵�
int CCmeCoreApi::ReqQryParkedOrderAction(CThostFtdcQryParkedOrderActionField *pQryParkedOrderAction, int nRequestID) 
{
    return 0;
}


///�����ѯ����֪ͨ
int CCmeCoreApi::ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID) 
{
    return 0;
}


///�����ѯ���͹�˾���ײ���
int CCmeCoreApi::ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID) 
{
    return 0;
}


///�����ѯ���͹�˾�����㷨
int CCmeCoreApi::ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID) 
{
    return 0;
}


///�ڻ����������ʽ�ת�ڻ�����
int CCmeCoreApi::ReqFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID) 
{
    return 0;
}


///�ڻ������ڻ��ʽ�ת��������
int CCmeCoreApi::ReqFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID) 
{
    return 0;
}


///�ڻ������ѯ�����������
int CCmeCoreApi::ReqQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, int nRequestID) 
{
    return 0;
}
int CCmeCoreApi::GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& Data)
{
    return 0;
}

///��ȡ�˻��ʽ��ı���Ϣ
int CCmeCoreApi::GetAccountText(std::string& outData,int language)
{
    return 0;
}
int CCmeCoreApi::GetAccountList(std::vector<string>& AcctList)
{
    return 0;
}
bool CCmeCoreApi::GetPositionInfo( const std::string& strAccount, 
                                  const std::string& strInstrumentID, PlatformStru_Position& field )
{
    return true;
}
void CCmeCoreApi::GetAllContractPosition(std::vector<PlatformStru_Position>& vecPosition)
{

}
void CCmeCoreApi::SetNewPwd( const std::string& strNewPwd )
{

}
int CCmeCoreApi::GetExchangeIDs(vector<string>& outData)
{
    return 0;
}
///��ȡ�˺���Ϣ
int CCmeCoreApi::GetTrader(std::vector<PlatformStru_Trader> &outData)
{
    return 0;
}
///��ȡ��������Ϣ
int CCmeCoreApi::GetExchange(std::vector<PlatformStru_Exchange> &outData)
{
    return 0;
}
///��ȡ��Լ��Ϣ
int CCmeCoreApi::GetContract(std::vector<PlatformStru_Contract> &outData)
{
    return 0;
}
///��ȡ��Ʒ��Ϣ
int CCmeCoreApi::GetCommodity(std::vector<PlatformStru_Commodity> &outData)
{
    return 0;
}
///��ȡ����������Ϣ
int CCmeCoreApi::GetOrderType(std::vector<PlatformStru_OrderType>& outData)
{
    return 0;
}
// ��ȡ����
int CCmeCoreApi::GetReports(map<DWORD, Report>& mapReport)
{
    return 0;
}
bool CCmeCoreApi::GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi)
{
    return false;
}
bool CCmeCoreApi::GetOpenPosition(PlatformStru_Contract InstrumentID,
                                  std::string account,PlatformStru_AccountPosition* posi)
{
    return false;
}
bool CCmeCoreApi::GetContractPosition(PlatformStru_Contract InstrumentID,
                                      std::string account,PlatformStru_AccountPosition* posi)
{
    return false;
}
// ��ȡ��¼״̬ԭ��
std::string CCmeCoreApi::GetLogonReason()
{
    return "";
}
void CCmeCoreApi::GetLinkStatus(int* pHost,int* pPrice)
{

}
double CCmeCoreApi::GetExchangeRate(char* currency)
{
    return 1.0;
}
//��tcp�õ����ݺ󣬵��ô˻ص��������д���
void CCmeCoreApi::trade_rcvdata(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
    ParseRcvData(pData,DataLen,true);
}
//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
void CCmeCoreApi::trade_disconn(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
    m_TradeOverTimeCount=0;
    KillTimer(NULL,TRADE_TIMER_ID);
}
//��ʼ����connect()�󣬵��ô˺���֪ͨӦ�ò�
void CCmeCoreApi::trade_startconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
}
//���ӳɹ��󣬵��ô˻ص��������д���
void CCmeCoreApi::trade_connsuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
    SetTimer(NULL, TRADE_TIMER_ID, ELAPSE_TIME, TradeTimerProc); 
    m_pTraderSpi->OnFrontConnected();
}
//����ʧ�ܺ󣬵��ô˻ص��������д���
void CCmeCoreApi::trade_connfailure(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
    m_pTraderSpi->OnFrontDisconnected(1);
}
void CCmeCoreApi::trade_ontime(void)
{
}
//��tcp�õ����ݺ󣬵��ô˻ص��������д���
void CCmeCoreApi::quot_rcvdata(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
    ParseRcvData(pData,DataLen,false);
}
//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
void CCmeCoreApi::quot_disconn(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{	
    m_QuotOverTimeCount=0;
    KillTimer(NULL,QUOTE_TIMER_ID);
}
//��ʼ����connect()�󣬵��ô˺���֪ͨӦ�ò�
void CCmeCoreApi::quot_startconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
}
//���ӳɹ��󣬵��ô˻ص��������д���
void CCmeCoreApi::quot_connsuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
    SetTimer(NULL, QUOTE_TIMER_ID, ELAPSE_TIME, QuotTimerProc); 
	m_pMdSpi->OnFrontConnected();
}
//����ʧ�ܺ󣬵��ô˻ص��������д���
void CCmeCoreApi::quot_connfailure(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
    m_pMdSpi->OnFrontDisconnected(1);
}
void CCmeCoreApi::quot_ontime(void)
{
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
}
////////////////////////////////////////////////////////////
//
//�����ҵ���߼�����
//
////////////////////////////////////////////////////////////////
void CCmeCoreApi::ParseRcvData(const char *pData,int DataLen,bool bTrader)
{
    CUniPkg datapkg;
    int LengthTakenup;
    if(datapkg.Parse(pData,DataLen,LengthTakenup))
    {
        Stru_UniPkgHead head=datapkg.GetPkgHead();
        switch(head.cmdid)
        {
        case CMDID_Client_HeartbeatRlt:
            if(bTrader)
                m_TradeOverTimeCount--;
            else
                m_QuotOverTimeCount--;
            break;
        case CMDID_Client_LoginRlt://�û���½��Ӧ
            OnRspUserLogin(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_LogoutRlt://�û��˳���Ӧ
        case CMDID_Client_ForceLogout:// ��������ǿ�ƿͻ��˵ǳ�
            OnRspUserLogout(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetIsSettltmentReportConfirmedRlt://��ѯ������Ϣ�Ƿ�ȷ��
            OnRspQrySettlementInfoConfirm(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetSettltmentReportRlt://Ͷ���߽�����
            OnRspQrySettlementInfo(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_ConfirmSettltmentReportRlt://�������Կͻ���ȷ�ϵķ���
            OnRspSettlementInfoConfirm(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetCurrencyInfoRlt://������Ϣ
            OnRspGetCurrencyInfo(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_CurrencyInfoUpdate://������Ϣ�ı�֪ͨ
            OnCurrencyInfoUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetAllExchangeInfoRlt://�����ѯ��Լ��Ϣ����2.����ȫ����������Ϣ
            OnRspGetAllExchangeInfo(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_ExchangeInfoUpdate://��������Ϣ����
            OnExchangeInfoUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_ExchangeTradingModeUpdate:
            OnRtnExchangeStatus(datapkg.GetContent(),head.len,bTrader);
        case CMDID_Client_GetExchangeProductGroupInfoRlt://�����ѯ��Լ��Ϣ����4.���ؽ������²�Ʒ����Ϣ
            OnRspGetExchangeProductGroupInfo(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_ProductGroupInfoUpdate://��Ʒ����Ϣ����
            OnProductGroupInfoUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetExchangeProductInfoRlt://�����ѯ��Լ��Ϣ����6.���ؽ����������в�Ʒ��Ϣ
            OnRspGetExchangeProductInfo(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_ProductInfoUpdate:
            OnProductInfoUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_ContractInfoUpdate:
            OnContractInfoUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetInitialEquityInfoRlt://��ѯ�ʽ���Ϣ
            OnRspQryTradingAccount(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetDepositWithdrawInfoRlt://���������ȡ�������Ϣ
            OnRspGetDepositWithdrawInfo(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_DepositWithdrawInfoUpdate://�������Ϣ�ı�֪ͨ
            OnDepositWithdrawInfoUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetTradeInfoRlt://��ѯ�ɽ�
            OnRspQryTrade(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_TradeArrived://�ɽ��ı�֪ͨ
            OnRtnTrade(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetPositionInfoRlt://��ѯ�ֲ���ϸ
            OnRspQryInvestorPositionDetail(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_PositionInfoUpdate://�ֲָı���Ϣ
            OnRtnPositionDetail(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetOrderInfoRlt://��ѯ����
            OnRspQryOrder(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_OrderCmdRlt://��������
            OnRspOrderCmd(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_OrderStatusUpdate://����״̬�ı�
            OnRspOrderStatusUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_SubscribeMarketDataRlt://��������
            OnRspSubscribeMarketData(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_UnsubscribeMarketDataRlt: //�˶�����  
            OnRspUnsubscribeMarketData(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetMarketDataSnapshotRlt://�������
            OnRspGetMarketDataSnapshot(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_NewMarketData://����
            OnNewMarketData(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_ChangeLoginPWRlt://�޸�����
            OnRspUserPasswordUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetExchangeCommissionMarginInfoRlt://��ȡָ���������ı�֤�������ѷ���
            OnRspGetExchangeCommissionMarginInfo(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_CommissionMarginInfoUpdate:
            OnCommissionMarginInfoUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        default:
            break;
        }
    }
}
void CCmeCoreApi::OnRspUserLogin(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_LoginRlt))
    { 
        Stru_Client_LoginRlt& Rlt=*((Stru_Client_LoginRlt*)pData);
        CThostFtdcRspUserLoginField field={0};
        field.SessionID=Rlt.LoginSessionID;
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        if(bTrader)
        {
            m_pTraderSpi->OnRspUserLogin(&field,&pRspInfo,Rlt.RefRequestID,true);
        }
        else
        {
            m_pMdSpi->OnRspUserLogin(&field,&pRspInfo,Rlt.RefRequestID,true);
        }
    }
}
void CCmeCoreApi::OnRspUserLogout(const char *pData,int DataLen,bool bTrader)
{

    if(pData&&DataLen>=sizeof(Stru_Client_LogoutRlt))
    { 
        Stru_Client_LogoutRlt& Rlt=*((Stru_Client_LogoutRlt*)pData);
        CThostFtdcUserLogoutField field={0};
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        if(bTrader)
        {
            m_pTraderSpi->OnRspUserLogout(&field,&pRspInfo,Rlt.RefRequestID,true);
        }
        else
        {
            m_pMdSpi->OnRspUserLogout(&field,&pRspInfo,Rlt.RefRequestID,true);
        }
    }
}
void CCmeCoreApi::OnRspQrySettlementInfoConfirm(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetIsSettltmentReportConfirmedRlt))
    { 
        Stru_Client_GetIsSettltmentReportConfirmedRlt& Rlt=*((Stru_Client_GetIsSettltmentReportConfirmedRlt*)pData);
        CThostFtdcSettlementInfoConfirmField SettleInfo={0}; 
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        m_pTraderSpi->OnRspQrySettlementInfoConfirm(Rlt.IsConfirmed!=0?(&SettleInfo):NULL, &pRspInfo,Rlt.RefRequestID,true);
    }

}
void CCmeCoreApi::OnRspQrySettlementInfo(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetSettlementReportInfoRlt))
    { 
        Stru_Client_GetSettlementReportInfoRlt& Rlt=*((Stru_Client_GetSettlementReportInfoRlt*)pData);
        if(m_SettlementReportInfos.size()!=Rlt.InfoCount)   
            m_SettlementReportInfos.resize(Rlt.InfoCount); 
        m_SettlementReportInfos[Rlt.InfoOrd]=Rlt.Info;
        if(Rlt.InfoOrd==Rlt.InfoCount-1)
        {
            CThostFtdcSettlementInfoField SettleInfo={0};
            sprintf_s(SettleInfo.BrokerID,10,"%d",Rlt.Info.AccountID);
            SettleInfo.SettlementID=(int)Rlt.Info.SettlementReportID;
            string strConent;
            for(int i=0;i<(int)m_SettlementReportInfos.size();++i)
                strConent+=m_SettlementReportInfos[i].Content;
            strcpy_s(SettleInfo.Content,500,strConent.c_str());
            CThostFtdcRspInfoField pRspInfo={0};
            pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
            m_pTraderSpi->OnRspQrySettlementInfo(&SettleInfo,&pRspInfo,Rlt.RefRequestID,true);
        }
    }
}
void CCmeCoreApi::OnRspSettlementInfoConfirm(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_ConfirmSettltmentReportRlt))
    {
        Stru_Client_ConfirmSettltmentReportRlt& Rlt=*((Stru_Client_ConfirmSettltmentReportRlt*)pData);
        CThostFtdcSettlementInfoConfirmField field={0};
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        m_pTraderSpi->OnRspSettlementInfoConfirm(&field,&pRspInfo,Rlt.RefRequestID,true);
    }
}
void CCmeCoreApi::OnRspGetCurrencyInfo(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetCurrencyInfoRlt))
    {
        Stru_Client_GetCurrencyInfoRlt& Rlt=*((Stru_Client_GetCurrencyInfoRlt*)pData);
        m_CurrencyInfos[Rlt.Info.CurrencyID]=Rlt.Info;
        if(Rlt.InfoOrd==Rlt.InfoCount-1)
        {
            m_pTradeDataWorker->GetAllExchangeInfo(Rlt.RefRequestID);//�����ѯ��Լ��Ϣ����1.��ȡȫ����������Ϣ
        }
    }
}
void CCmeCoreApi::OnCurrencyInfoUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_CurrencyInfoUpdate))
    {
        Stru_Client_CurrencyInfoUpdate& Rlt=*((Stru_Client_CurrencyInfoUpdate*)pData);
        switch(Rlt.UpdateFlag)
        {
        case UpdateFlag_new:
        case UpdateFlag_modify:
            m_CurrencyInfos[Rlt.Info.CurrencyID]=Rlt.Info;
            break;
        case UpdateFlag_remove:
            m_CurrencyInfos.erase(Rlt.Info.CurrencyID);
            break;
        }
    }

}
//��������Ϣ�ı�֪ͨ
void CCmeCoreApi::OnExchangeInfoUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_ExchangeInfoUpdate))
    {
        Stru_Client_ExchangeInfoUpdate& Rlt=*((Stru_Client_ExchangeInfoUpdate*)pData);
        switch(Rlt.UpdateFlag)
        {
        case UpdateFlag_new:
        case UpdateFlag_modify:
            m_ExchangeInfos[Rlt.Info.ExchangeID]=Rlt.Info;
            break;
        case UpdateFlag_remove:
            m_ExchangeInfos.erase(Rlt.Info.ExchangeID);
            break;
        }
    }
}
// ����������״̬�ı�֪ͨ
void CCmeCoreApi::OnRtnExchangeStatus(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_ExchangeTradingModeUpdate))
    {
        Stru_Client_ExchangeTradingModeUpdate& Rlt=*((Stru_Client_ExchangeTradingModeUpdate*)pData);
        CThostFtdcInstrumentStatusField field={0};
        strcpy_s(field.ExchangeID,sizeof(field.ExchangeID)-1,m_ExchangeInfos[Rlt.ExchangeID].ExchangeCode);
        switch(Rlt.TradingMode){
            case TradingMode_NotTrading:    field.InstrumentStatus=THOST_FTDC_IS_NoTrading;break;
            case TradingMode_NomalTrading:  field.InstrumentStatus=THOST_FTDC_IS_Continous;break;
            case TradingMode_AggregateAuction:field.InstrumentStatus=THOST_FTDC_IS_AuctionOrdering;break;
            case TradingMode_AggregateMatching:field.InstrumentStatus=THOST_FTDC_IS_AuctionMatch;break;
            default:    field.InstrumentStatus=THOST_FTDC_IS_Continous;break;
        }
        m_pTraderSpi->OnRtnInstrumentStatus(&field);

    }
}
//�����ѯ��Լ��Ϣ����2.����ȫ����������Ϣ
void CCmeCoreApi::OnRspGetAllExchangeInfo(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetAllExchangeInfoRlt))
    { 
        Stru_Client_GetAllExchangeInfoRlt& Rlt=*((Stru_Client_GetAllExchangeInfoRlt*)pData);
        m_ExchangeInfos[Rlt.Info.ExchangeID]=Rlt.Info;
        if(Rlt.InfoOrd==Rlt.InfoCount-1)//
        {
            //�����ѯ��Լ��Ϣ����3.��ȡ�������²�Ʒ����Ϣ
            m_QueryExchangeInfoIterator=m_ExchangeInfos.begin();
            if(m_QueryExchangeInfoIterator!=m_ExchangeInfos.end())
            {
                m_pTradeDataWorker->GetExchangeProductGroupInfo(m_QueryExchangeInfoIterator->second.ExchangeID,Rlt.RefRequestID);
                m_QueryExchangeInfoIterator++;
            }
            else
                OnRspQryInstrument(Rlt.RefRequestID);    
        }
    }
}
//�����ѯ��Լ��Ϣ����4.���ؽ������²�Ʒ����Ϣ
void CCmeCoreApi::OnRspGetExchangeProductGroupInfo(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetExchangeProductGroupInfoRlt))
    {
        Stru_Client_GetExchangeProductGroupInfoRlt& Rlt=*((Stru_Client_GetExchangeProductGroupInfoRlt*)pData);
        m_ProductGroupInfos[Rlt.ExchangeID][Rlt.Info.ProductGroupID]=Rlt.Info;
        if(Rlt.InfoOrd==Rlt.InfoCount-1)//��ȡ�˲�Ʒ��Ϣ
        {
            //�����ѯ��Լ��Ϣ����5.��ȡ�����������в�Ʒ��Ϣ
            m_pTradeDataWorker->GetExchangeProductInfo(Rlt.Info.ExchangeID,Rlt.RefRequestID);
        }
    }
}
void CCmeCoreApi::OnProductGroupInfoUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_ProductGroupInfoUpdate))
    {
        Stru_Client_ProductGroupInfoUpdate& Rlt=*((Stru_Client_ProductGroupInfoUpdate*)pData);
        switch(Rlt.UpdateFlag)
        {
        case UpdateFlag_new:
        case UpdateFlag_modify:
            m_ProductGroupInfos[Rlt.Info.ExchangeID][Rlt.Info.ProductGroupID]=Rlt.Info;
            break;
        case UpdateFlag_remove:
            m_ProductGroupInfos[Rlt.Info.ExchangeID].erase(Rlt.Info.ProductGroupID);
            break;
        }
    }
}
//�����ѯ��Լ��Ϣ����6.���ؽ����������в�Ʒ��Ϣ
void CCmeCoreApi::OnRspGetExchangeProductInfo(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetExchangeProductInfoRlt))
    {
        Stru_Client_GetExchangeProductInfoRlt& Rlt=*((Stru_Client_GetExchangeProductInfoRlt*)pData);
        m_ProductInfos[Rlt.ExchangeID][Rlt.Info.ProductID]=Rlt.Info;
        if(Rlt.InfoOrd==Rlt.InfoCount-1)//��ȡ�˺�Լ��Ϣ
        {
            //�����ѯ��Լ��Ϣ����7.��ȡ�����������к�Լ��Ϣ
            m_pTradeDataWorker->GetExchangeContractInfo(Rlt.Info.ExchangeID,Rlt.RefRequestID);
        }
    }
}
void CCmeCoreApi::OnProductInfoUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_ProductInfoUpdate))
    {
        Stru_Client_ProductInfoUpdate& Rlt=*((Stru_Client_ProductInfoUpdate*)pData);
        switch(Rlt.UpdateFlag)
        {
        case UpdateFlag_new:
        case UpdateFlag_modify:
            m_ProductInfos[Rlt.Info.ExchangeID][Rlt.Info.ProductID]=Rlt.Info;
            break;
        case UpdateFlag_remove:
            m_ProductInfos[Rlt.Info.ExchangeID].erase(Rlt.Info.ProductID);
            break;
        }
    }
}
void CCmeCoreApi::OnContractInfoUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_ContractInfoUpdate))
    {
        Stru_Client_ContractInfoUpdate& Rlt=*((Stru_Client_ContractInfoUpdate*)pData);
        //���ExchangeID
        int ExchangeID=0;
        map<int,map<int,Stru_Base_ProductInfo>>::const_iterator iter;
        map<int,Stru_Base_ProductInfo>::const_iterator inner_iter;
        for(iter=m_ProductInfos.begin();iter!=m_ProductInfos.end();++iter)
        {
            for(inner_iter=iter->second.begin();inner_iter!=iter->second.end();++inner_iter)
            {
                if(inner_iter->first==Rlt.Info.ProductID)
                {
                    ExchangeID=inner_iter->second.ExchangeID;
                    break;
                }
            }
        }
        if(ExchangeID==0) return;
        InstrumentIDType	InstrumentID={0};
        MakeGUIContractID(m_ExchangeInfos[ExchangeID],m_ProductInfos[ExchangeID][Rlt.Info.ProductID],
            Rlt.Info,InstrumentID);
        switch(Rlt.UpdateFlag)
        {
        case UpdateFlag_new:
        case UpdateFlag_modify:
            m_ContractInfos[ExchangeID][Rlt.Info.ContractID]=Rlt.Info;
            m_ID2ContractName[Rlt.Info.ContractID]=InstrumentID;
            m_ContractName2ID[InstrumentID]=EPCID(ExchangeID,Rlt.Info.ProductID,Rlt.Info.ContractID);
            break;
        case UpdateFlag_remove:
            m_ContractInfos[ExchangeID].erase(Rlt.Info.ContractID);
            m_ID2ContractName.erase(Rlt.Info.ContractID);
            m_ContractName2ID.erase(InstrumentID);
            break;
        }
    }
}
//�����ѯ��Լ��Ϣ����8.���ؽ����������к�Լ��Ϣ
void CCmeCoreApi::GetExchangeContractInfo(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetExchangeContractInfoRlt))
    {
        Stru_Client_GetExchangeContractInfoRlt& Rlt=*((Stru_Client_GetExchangeContractInfoRlt*)pData);
        m_ContractInfos[Rlt.ExchangeID][Rlt.Info.ContractID]=Rlt.Info;
        if(Rlt.InfoOrd==Rlt.InfoCount-1)//��ȡ�˺�Լ��Ϣ
        {
            //�����ѯ��Լ��Ϣ����9.���һ���������Ĳ�ѯ,��ѯ��һ����������Ϣ
            if(m_QueryExchangeInfoIterator!=m_ExchangeInfos.end())
            {
                m_pTradeDataWorker->GetExchangeProductGroupInfo(m_QueryExchangeInfoIterator->second.ExchangeID,Rlt.RefRequestID);
                m_QueryExchangeInfoIterator++;
            }
            else
                OnRspQryInstrument(Rlt.RefRequestID);    
        }
    }
}
//�����ѯ��Լ��Ϣ����10.ȫ�����
void CCmeCoreApi::OnRspQryInstrument(int RequestID)
{
    map<int,map<int,Stru_Base_ContractInfo>> ::const_iterator iterA;
    map<int,Stru_Base_ContractInfo>::const_iterator           iterB; 
    //��ȡ��Լ����
    int ContractCount=0,ContractIndex=0;
    for(iterA=m_ContractInfos.begin();iterA!=m_ContractInfos.end();++iterA)
    {
        ContractCount+=iterA->second.size();
    }
    for(iterA=m_ContractInfos.begin();iterA!=m_ContractInfos.end();++iterA)
    {
        const map<int,Stru_Base_ContractInfo>& contractmap=iterA->second;
        for(iterB=contractmap.begin();iterB!=contractmap.end();++iterB)
        {
            PlatformStru_InstrumentInfo Instrument;
            CThostFtdcRspInfoField pRspInfo={0};
            FillInstrumentInfo(iterA->first,iterB->second,Instrument);
            m_pTraderSpi->OnRspQryInstrument(&Instrument,&pRspInfo,RequestID,(++ContractIndex)==ContractCount);
        }
    }
}
void CCmeCoreApi::MakeGUIContractID(const Stru_Base_ExchangeInfo& ExchangeInfo,
                                    const Stru_Base_ProductInfo& ProductInfo,
                                    const Stru_Base_ContractInfo& ContractInfo,char* strContract)
{
    string str=string(ProductInfo.ProductName)+ContractInfo.ContractDate+'@'+ExchangeInfo.ExchangeCode;
    strcpy_s(strContract,sizeof(strContract)-1,str.c_str());
    m_ContractName2ID[strContract]=EPCID(ExchangeInfo.ExchangeID,ProductInfo.ProductID,ContractInfo.ContractID);
    m_ID2ContractName[ContractInfo.ContractID]=strContract;
}
void CCmeCoreApi::OnRspQryTradingAccount(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetInitialEquityInfoRlt))
    {
        Stru_Client_GetInitialEquityInfoRlt& Rlt=*((Stru_Client_GetInitialEquityInfoRlt*)pData);
        if(Rlt.Rlt==1)
        {
            FillTradingAccount(Rlt.Info,m_TradingAccount);
            m_pTradeDataWorker->GetDepositWithdrawInfo(Rlt.RefRequestID);
        }
    }
}
void CCmeCoreApi::OnRspGetDepositWithdrawInfo(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetDepositWithdrawDynaRlt))
    {
        Stru_Client_GetDepositWithdrawDynaRlt& Rlt=*((Stru_Client_GetDepositWithdrawDynaRlt*)pData);
        FillTradingAccount(Rlt.Info,m_TradingAccount);
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0; 
        m_pTraderSpi->OnRspQryTradingAccount(&m_TradingAccount,&pRspInfo,Rlt.RefRequestID,Rlt.InfoOrd==Rlt.InfoCount-1);
    }
}
void CCmeCoreApi::OnDepositWithdrawInfoUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_DepositWithdrawDynaUpdate))
    {
        Stru_Client_DepositWithdrawDynaUpdate& Rlt=*((Stru_Client_DepositWithdrawDynaUpdate*)pData);
        FillTradingAccount(Rlt,m_TradingAccount);
        CThostFtdcRspInfoField pRspInfo={0};
        m_pTraderSpi->OnRspQryTradingAccount(&m_TradingAccount,&pRspInfo,0,true);
    }
}
void CCmeCoreApi::OnRspQryTrade(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetTradeInfoRlt))
    {
        Stru_Client_GetTradeInfoRlt& Rlt=*((Stru_Client_GetTradeInfoRlt*)pData);
        PlatformStru_TradeInfo Trade={0};
        FillTradeInfo(Rlt.Info,Trade);
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        m_pTraderSpi->OnRspQryTrade(&Trade,&pRspInfo,Rlt.RefRequestID,Rlt.InfoOrd==Rlt.InfoCount-1);
    }
}
void CCmeCoreApi::OnRtnTrade(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_TradeArrived))
    {
        Stru_Client_TradeArrived& Rlt=*((Stru_Client_TradeArrived*)pData);
        PlatformStru_TradeInfo Trade={0};
        FillTradeInfo(Rlt.Info,Trade);
        m_pTraderSpi->OnRtnTrade(&Trade);
    }
}
//�ֲָı�֪ͨ����ʱû��ʹ��
void CCmeCoreApi::OnRtnPositionDetail(const char *pData,int DataLen,bool bTrader)
{

}
void CCmeCoreApi::OnRspQryInvestorPositionDetail(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetPositionInfoRlt))
    {
        Stru_Client_GetPositionInfoRlt& Rlt=*((Stru_Client_GetPositionInfoRlt*)pData);
        PlatformStru_Position  InvestorPositionDetail;
        FillPositionInfo(Rlt.Info,InvestorPositionDetail);
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        m_pTraderSpi->OnRspQryInvestorPositionDetail(&InvestorPositionDetail,&pRspInfo,Rlt.RefRequestID,Rlt.InfoOrd==Rlt.InfoCount-1);
    }
}
void CCmeCoreApi::OnRspQryOrder(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetOrderInfoRlt))
    {
        Stru_Client_GetOrderInfoRlt& Rlt=*((Stru_Client_GetOrderInfoRlt*)pData);
        PlatformStru_OrderInfo OrderInfo={0};
        FillOrderInfo(Rlt.Info,OrderInfo);
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        m_pTraderSpi->OnRspQryOrder(&OrderInfo,&pRspInfo,Rlt.RefRequestID,Rlt.InfoOrd==Rlt.InfoCount-1);
    }
}
void CCmeCoreApi::OnRspOrderCmd(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_OrderCmdRlt))
    {
        Stru_Client_OrderCmdRlt& Rlt=*((Stru_Client_OrderCmdRlt*)pData);
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        map<int,EnumOrderCmdFlag>::const_iterator iter=m_RequestID2OrderCmdFlag.find(Rlt.RefRequestID);
        switch(iter->second)
        {
        case OrderCmdFlag_New:
            {
                CThostFtdcInputOrderField field={0};
                FillOrderCmdInfo(iter->second,Rlt.Info,field);
                m_pTraderSpi->OnRspOrderInsert(&field,&pRspInfo,Rlt.RefRequestID,true);
            }
            break;
        case OrderCmdFlag_Amend:
        case OrderCmdFlag_Cancel:
            {
                CThostFtdcInputOrderActionField field={0};
                FillOrderCmdInfo(iter->second,Rlt.Info,field);
                m_pTraderSpi->OnRspOrderAction(&field,&pRspInfo,Rlt.RefRequestID,true);
            }
            break;
        case OrderCmdFlag_New_Park:
            {
                CThostFtdcParkedOrderField field={0};
                FillOrderCmdInfo(iter->second,Rlt.Info,field);
                m_pTraderSpi->OnRspParkedOrderInsert(&field,&pRspInfo,Rlt.RefRequestID,true);
            }
            break;
        case OrderCmdFlag_Amend_Park:
        case OrderCmdFlag_Cancel_Park:
            {
                CThostFtdcParkedOrderActionField field={0};
                FillOrderCmdInfo(iter->second,Rlt.Info,field);
                m_pTraderSpi->OnRspParkedOrderAction(&field,&pRspInfo,Rlt.RefRequestID,true);
            }
            break;
        }
    }
}
void CCmeCoreApi::OnRspOrderStatusUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_OrderStatusUpdate))
    {
        Stru_Client_OrderStatusUpdate& Rlt=*((Stru_Client_OrderStatusUpdate*)pData);
        PlatformStru_OrderInfo info;
        FillRtnOrder(Rlt.Info,info);
        m_pTraderSpi->OnRtnOrder(&info);
    }
}
void CCmeCoreApi::OnRspSubscribeMarketData(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_SubscribeMarketDataRlt))
    {
        Stru_Client_SubscribeMarketDataRlt& Rlt=*((Stru_Client_SubscribeMarketDataRlt*)pData);
        map<int,string>::const_iterator iter=m_SubscribeUnsubscribeMarketMap.find(Rlt.RefRequestID);
        if(iter!=m_SubscribeUnsubscribeMarketMap.end())
        {
            CThostFtdcSpecificInstrumentField field={0};
            strcpy_s(field.InstrumentID,sizeof(field.InstrumentID)-1,iter->second.c_str());
            CThostFtdcRspInfoField pRspInfo={0};
            pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
            m_pMdSpi->OnRspSubMarketData(&field,&pRspInfo,Rlt.RefRequestID,true);
        }
    }
}
void CCmeCoreApi::OnRspUnsubscribeMarketData(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_UnsubscribeMarketDataRlt))
    {
        Stru_Client_UnsubscribeMarketDataRlt& Rlt=*((Stru_Client_UnsubscribeMarketDataRlt*)pData);
        map<int,string>::const_iterator iter=m_SubscribeUnsubscribeMarketMap.find(Rlt.RefRequestID);
        if(iter!=m_SubscribeUnsubscribeMarketMap.end())
        {
            CThostFtdcSpecificInstrumentField field={0};
            strcpy_s(field.InstrumentID,sizeof(field.InstrumentID)-1,iter->second.c_str());
            CThostFtdcRspInfoField pRspInfo={0};
            pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
            m_pMdSpi->OnRspUnSubMarketData(&field,&pRspInfo,Rlt.RefRequestID,true);
        }
    }
}
void CCmeCoreApi::OnRspGetMarketDataSnapshot(const char *pData,int DataLen,bool bTrader)
{
    //ֻʵ��һ������һ����Լ
    if(pData&&DataLen>=sizeof(Stru_Client_GetMarketDataSnapshotRlt))
    {
        Stru_Client_GetMarketDataSnapshotRlt& Rlt=*((Stru_Client_GetMarketDataSnapshotRlt*)pData);
        CThostFtdcDepthMarketDataField field={0};
        FillDepthMarketData(Rlt.Info,field);
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        m_pTraderSpi->OnRspQryDepthMarketData(&field,&pRspInfo,Rlt.RefRequestID,true);
    }
}
void CCmeCoreApi::OnNewMarketData(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_NewMarketData))
    {
        Stru_Client_NewMarketData& Rlt=*((Stru_Client_NewMarketData*)pData);
        PlatformStru_DepthMarketData field={0};
        FillDepthMarketData(Rlt,field);
        m_pMdSpi->OnRtnDepthMarketData(&field);
    }
}
void CCmeCoreApi::OnRspUserPasswordUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_ChangeLoginPWRlt))
    {
        Stru_Client_ChangeLoginPWRlt& Rlt=*((Stru_Client_ChangeLoginPWRlt*)pData);
        CThostFtdcUserPasswordUpdateField  UserPasswordUpdate;
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        m_pTraderSpi->OnRspUserPasswordUpdate(&UserPasswordUpdate,&pRspInfo,Rlt.RefRequestID,true);
    }
}
void CCmeCoreApi::SendHeartbeat(bool bTrader)
{
    if(bTrader)
    {
        if(m_TradeOverTimeCount>=OVERTIME_LIMIT)
        {
            Stru_IPAddress LocalAddr,SvrAddr;
            m_pTradeDataWorker->GetAddr(LocalAddr,SvrAddr);
            trade_disconn(LocalAddr,SvrAddr,INVALID_SOCKET);
        }
        else
        {
            m_TradeOverTimeCount++;
            m_pTradeDataWorker->SendHeartbeat();
        }
    }
    else
    {
        if(m_QuotOverTimeCount>=OVERTIME_LIMIT)
        {
            Stru_IPAddress LocalAddr,SvrAddr;
            m_pQuotDataWorker->GetAddr(LocalAddr,SvrAddr);
            quot_disconn(LocalAddr,SvrAddr,INVALID_SOCKET);            
        }
        else
        {
            m_QuotOverTimeCount++;
            m_pQuotDataWorker->SendHeartbeat();
        }
    }
}
void CCmeCoreApi::OnRspGetExchangeCommissionMarginInfo(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetExchangeCommissionMarginInfoRlt))
    {
        Stru_Client_GetExchangeCommissionMarginInfoRlt& Rlt=*((Stru_Client_GetExchangeCommissionMarginInfoRlt*)pData);
        PlatformStru_InstrumentMarginRate  mr={0};
        PlatformStru_InstrumentCommissionRate cr={0};
        FillCommissionMarginInfo(Rlt.CommissionInfo,cr);
        FillCommissionMarginInfo(Rlt.MarginInfo,mr);
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0; 
        m_pTraderSpi->OnRspQryInstrumentMarginRate(&mr,&pRspInfo,Rlt.RefRequestID,true) ;
        m_pTraderSpi->OnRspQryInstrumentCommissionRate(&cr,&pRspInfo,Rlt.RefRequestID,true) ;
    }
}
void CCmeCoreApi::OnCommissionMarginInfoUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_CommissionMarginInfoUpdate))
    {
        Stru_Client_CommissionMarginInfoUpdate& Rlt=*((Stru_Client_CommissionMarginInfoUpdate*)pData);
        PlatformStru_InstrumentMarginRate  mr={0};
        PlatformStru_InstrumentCommissionRate cr={0};
        FillCommissionMarginInfo(Rlt.CommissionInfo,cr);
        FillCommissionMarginInfo(Rlt.MarginInfo,mr);
        CThostFtdcRspInfoField pRspInfo={0};
        m_pTraderSpi->OnRspQryInstrumentMarginRate(&mr,&pRspInfo,0,true) ;
        m_pTraderSpi->OnRspQryInstrumentCommissionRate(&cr,&pRspInfo,0,true) ;
    }
}