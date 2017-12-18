#include "stdafx.h"

#include "ServiceProxy.hpp"
#include "PlatFormService.hpp"
#include "Module-Misc2/globalDefines.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
#define CHECK_LOGIN_STATUS()    \
    if(!m_isLogin)  return -999

const static string __UNKOWN__ = "δ֪";
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CServiceProxy", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)


/*--------------------------------------------------------------------------------------------------------
           ����ҵ���������첽�ģ��趨�ƾ���ҵ�����ݣ������յ�������
  ---------------------------------------------------------------------------------------------------------*/
///�û������������
int CServiceProxy::ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate &UserPasswordUpdate, int nRequestID)
{
    CHECK_LOGIN_STATUS();
        
    return m_PlatFormService->ReqUserPasswordUpdate(&UserPasswordUpdate, nRequestID);
}

///�ʽ��˻������������
int CServiceProxy::ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate &TradingAccountPasswordUpdate, int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->ReqTradingAccountPasswordUpdate(&TradingAccountPasswordUpdate, nRequestID);
}

///����¼������. ����ʱpInputOrder�е�RrequestID��BrokerID��InvestorID��UserID��OrderRef��ֵ���޸�Ϊ�ײ�ȷ����ֵ
int CServiceProxy::ReqOrderInsert(PlatformStru_InputOrder &InputOrder, int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->ReqOrderInsert(&InputOrder, nRequestID);
}

///Ԥ��¼������. ����ʱpInputOrder�е�RrequestID��BrokerID��InvestorID��UserID��OrderRef��ֵ���޸�Ϊ�ײ�ȷ����ֵ
int CServiceProxy::ReqParkedOrderInsert(PlatformStru_ParkedOrder &ParkedOrder, int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->ReqParkedOrderInsert(&ParkedOrder, nRequestID);
}

///Ԥ�񳷵�¼������
int CServiceProxy::ReqParkedOrderAction(PlatformStru_ParkedOrderAction &ParkedOrderAction, int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->ReqParkedOrderAction(&ParkedOrderAction, nRequestID);
}

///������������
int CServiceProxy::ReqOrderAction(PlatformStru_InputOrderAction &InputOrderAction, int nRequestID)
{
    CHECK_LOGIN_STATUS();

    return m_PlatFormService->ReqOrderAction(&InputOrderAction, nRequestID);
}

///������������(����)
int CServiceProxy::ReqCancelOrder(const OrderKey& orderkey, int nRequestID)
{
    CHECK_LOGIN_STATUS();

    return m_PlatFormService->ReqCancelOrder(orderkey, nRequestID);
}

///��ѯ��󱨵���������
int CServiceProxy::ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume &QueryMaxOrderVolume, int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQueryMaxOrderVolume(QueryMaxOrderVolume,nRequestID);
    return 0;
}

///Ͷ���߽�����ȷ��
int CServiceProxy::ReqSettlementInfoConfirm() 
{
    CHECK_LOGIN_STATUS();
    

    return m_PlatFormService->ReqSettlementInfoConfirm();
}

///����ɾ��Ԥ��
int CServiceProxy::ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder &RemoveParkedOrder, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    

    return m_PlatFormService->ReqRemoveParkedOrder(&RemoveParkedOrder, nRequestID);
}

///����ɾ��Ԥ�񳷵�
int CServiceProxy::ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction &RemoveParkedOrderAction, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    

    return m_PlatFormService->ReqRemoveParkedOrderAction(&RemoveParkedOrderAction, nRequestID);
}

///�����ѯ����
int CServiceProxy::ReqQryOrder(PlatformStru_QryOrder &QryOrder, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    

    if(m_pQryQueue)
        m_pQryQueue->ReqQryOrder(QryOrder,nRequestID);
    return 0;
}

///�����ѯ�ɽ�
int CServiceProxy::ReqQryTrade(PlatformStru_QryTrade &QryTrade, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryTrade(QryTrade,nRequestID);
    return 0;
}

///�����ѯͶ���ֲ߳�
int CServiceProxy::ReqQryInvestorPosition(PlatformStru_QryInvestorPosition &QryInvestorPosition, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryInvestorPosition(QryInvestorPosition,nRequestID);
    return 0;
}

///�����ѯ�ʽ��˻�
int CServiceProxy::ReqQryTradingAccount(PlatformStru_QryTradingAccount &QryTradingAccount, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryTradingAccount(QryTradingAccount,nRequestID);
    return 0;
}

//�����ѯ���������ָ�����͵Ĳ�ѯ��������TCP��������ز�ǰ�Ĵ���
//ReqCmdID:1:�ʽ��ѯ��2:�ɽ���ѯ��4:������ѯ��8:�ֲ���ϸ��ѯ��16:�ֲֲ�ѯ��32:��ϳֲ���ϸ��ѯ
//ReqCmdID���Ի���
void CServiceProxy::ClearReqInQryQueue(int ReqCmdID)
{
    if(!m_isLogin)  return;
    
    if(m_pQryQueue)
        m_pQryQueue->ClearReqInQryQueue(ReqCmdID);
}

///�����ѯͶ����
int CServiceProxy::ReqQryInvestor(PlatformStru_QryInvestor &QryInvestor, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    

    return m_PlatFormService->ReqQryInvestor(&QryInvestor, nRequestID);
}

///�����ѯ���͹�˾���ײ���
int CServiceProxy::ReqQryBrokerTradingParams(int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryBrokerTradingParams(nRequestID);

    return 0;
}

///�����ѯƷ��
int CServiceProxy::ReqQryProduct(PlatformStru_QryProduct &QryProduct, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryProduct(QryProduct,nRequestID);

    return 0;
}

///�����ѯ��Լ
int CServiceProxy::ReqQryInstrument(PlatformStru_QryInstrument &QryInstrument, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryInstrument(QryInstrument,nRequestID);

    return 0;
}

///�����ѯ����
int CServiceProxy::ReqQryDepthMarketData(PlatformStru_QryDepthMarketData &QryDepthMarketData, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryDepthMarketData(QryDepthMarketData,nRequestID);
	return 0;
}

///�����ѯͶ���߽�����
int CServiceProxy::ReqQrySettlementInfo(PlatformStru_QrySettlementInfo &QrySettlementInfo, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
   
    if(m_pQryQueue)
        m_pQryQueue->ReqQrySettlementInfo(QrySettlementInfo,nRequestID);

    return 0;
}

///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
int CServiceProxy::ReqQryCFMMCTradingAccountKey(PlatformStru_QryCFMMCTradingAccountKey &QryCFMMCTradingAccountKey, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
   
    if(m_pQryQueue)
        m_pQryQueue->ReqQryCFMMCTradingAccountKey(QryCFMMCTradingAccountKey,nRequestID);

    return 0;
}


///�����ѯת������
int CServiceProxy::ReqQryTransferBank(PlatformStru_QryTransferBank &QryTransferBank, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryTransferBank(QryTransferBank,nRequestID);
    return 0;
}

///�����ѯͶ���ֲ߳���ϸ
int CServiceProxy::ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail &QryInvestorPositionDetail, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryInvestorPositionDetail(QryInvestorPositionDetail,nRequestID);
    return 0;
}

///�����ѯ�ͻ�֪ͨ
int CServiceProxy::ReqQryNotice(PlatformStru_QryNotice &QryNotice, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryNotice(QryNotice,nRequestID);
    return 0;
}

///�����ѯ������Ϣȷ��
int CServiceProxy::ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm &QrySettlementInfoConfirm, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQrySettlementInfoConfirm(QrySettlementInfoConfirm,nRequestID);
    return 0;
}

///�����ѯͶ���ֲ߳���ϸ
int CServiceProxy::ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail &QryInvestorPositionCombineDetail, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryInvestorPositionCombineDetail(QryInvestorPositionCombineDetail,nRequestID);
    return 0;
}

///�����ѯת����ˮ
int CServiceProxy::ReqQryTransferSerial(PlatformStru_QryTransferSerial &QryTransferSerial, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryTransferSerial(QryTransferSerial,nRequestID);
    return 0;
}

///�����ѯǩԼ����
int CServiceProxy::ReqQryContractBank(PlatformStru_QryContractBank &QryContractBank, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryContractBank(QryContractBank,nRequestID);
    return 0;
}

///�����ѯ����ǩԼ��ϵ
int CServiceProxy::ReqQryAccountregister(PlatformStru_QryAccountRegister &pQryAccountregister, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryAccountregister(pQryAccountregister,nRequestID);
    return 0;
}

///�����ѯ����
int CServiceProxy::ReqQryExchangeRate(PlatformStru_QryExchangeRate &QryExchangeRate, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryExchangeRate(QryExchangeRate,nRequestID);
    return 0;
}

///�����ѯԤ��
int CServiceProxy::ReqQryParkedOrder(PlatformStru_QryParkedOrder &QryParkedOrder, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryParkedOrder(QryParkedOrder,nRequestID);
    return 0;
}

///�����ѯԤ�񳷵�
int CServiceProxy::ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction &QryParkedOrderAction, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryParkedOrderAction(QryParkedOrderAction,nRequestID);
    return 0;
}




///�ڻ����������ʽ�ת�ڻ�����
int CServiceProxy::ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer &ReqTransfer, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    

    return m_PlatFormService->ReqFromBankToFutureByFuture(&ReqTransfer, nRequestID);
}

///�ڻ������ڻ��ʽ�ת��������
int CServiceProxy::ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer &ReqTransfer, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    

    return m_PlatFormService->ReqFromFutureToBankByFuture(&ReqTransfer, nRequestID);
}

///�ڻ������ѯ�����������
int CServiceProxy::ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount &ReqQueryAccount, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQueryBankAccountMoneyByFuture(ReqQueryAccount,nRequestID);
    return 0;
}

///����ѯ��֪ͨ��OptionInstrumentIDsΪ������ȫ����Ȩ��Լ��ѯ��֪ͨ
int  CServiceProxy::SubscribeForQuoteRsp(const vector<string>& OptionInstrumentIDs)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->SubscribeForQuoteRsp(OptionInstrumentIDs);
}
///�˶�ѯ��֪ͨ��OptionInstrumentIDsΪ�����˶�ȫ����Լ��ѯ��֪ͨ
int CServiceProxy::UnSubscribeForQuoteRsp(const vector<string>& OptionInstrumentIDs)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->UnSubscribeForQuoteRsp(OptionInstrumentIDs);
}

///����ѯ��¼��
int CServiceProxy::ReqForQuoteInsert(PlatformStru_InputForQuoteField& InputForQuote, int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->ReqForQuoteInsert(InputForQuote, nRequestID);
}
///ִ������¼������
int CServiceProxy::ReqExecOrderInsert(PlatformStru_InputExecOrderField& InputExecOrder, int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->ReqExecOrderInsert(InputExecOrder, nRequestID);
}
///ִ�������������
int CServiceProxy::ReqExecOrderAction(PlatformStru_InputExecOrderActionField& InputExecOrderAction, int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->ReqExecOrderAction(InputExecOrderAction, nRequestID);
}
///�����ѯִ������
int CServiceProxy::ReqQryExecOrder(PlatformStru_QryExecOrderField& QryExecOrder, int nRequestID)
{
    if(m_pQryQueue)
        m_pQryQueue->ReqQryExecOrder(QryExecOrder,nRequestID);
    return 0;
}

