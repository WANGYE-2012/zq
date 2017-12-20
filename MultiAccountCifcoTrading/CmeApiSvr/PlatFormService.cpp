#include "stdafx.h"

#include "PlatFormService.hpp"
#include "DataCenter.hpp"
#include "wx/thread.h"
#include "../inc/Module-Misc/packagecache.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"

#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CPlatFormService", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)

//������CPlatFormService�ĳ�Ա����ʹ��
#define CHECK_TRADE_STATUS() \
    if(!m_pTradeApi || m_PlatformParam.TradeStatus!=CPlatFormParam::CONNSTATUS_Connected)\
    {\
        LOG_INFO("����API��δ׼�����������ܵ���δ�ɹ�����������");\
        return ret;\
    }

///�û������������
/***********************************************************
*   return  :   0:�ɹ�
*               -1:     ��������ʧ��
*               -2:     δ�������󳬹������
*               -3:     ÿ�뷢�����������������
*               -999:   ����ԭ��ʧ��
***********************************************************/
int CPlatFormService::ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate *pUserPasswordUpdate, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pUserPasswordUpdate->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pUserPasswordUpdate->Thost.BrokerID)-1); ///���͹�˾����       
    strncpy(pUserPasswordUpdate->Thost.UserID, m_PlatformParam.UserID.c_str(), sizeof(pUserPasswordUpdate->Thost.UserID)-1);  ///�û�����,����Ͷ���ߴ���
    ret = m_pTradeApi->ReqUserPasswordUpdate(&(pUserPasswordUpdate->Thost), nRequestID); 
    LOG_INFO("ReqUserPasswordUpdate(�û������������):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],UserID=[%s],OldPassword=[%s],NewPassword=[%s]",
                     ret, nRequestID,
                     pUserPasswordUpdate->Thost.BrokerID,
                     pUserPasswordUpdate->Thost.UserID,
                     pUserPasswordUpdate->Thost.OldPassword,
                     pUserPasswordUpdate->Thost.NewPassword);
    return ret;
}

///�ʽ��˻������������
int CPlatFormService::ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate *pTradingAccountPasswordUpdate, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pTradingAccountPasswordUpdate->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pTradingAccountPasswordUpdate->Thost.BrokerID)-1); ///���͹�˾����       
    strncpy(pTradingAccountPasswordUpdate->Thost.AccountID, m_PlatformParam.UserID.c_str(), sizeof(pTradingAccountPasswordUpdate->Thost.AccountID)-1);  ///�ʽ��˺�,�����û�����,Ҳ����Ͷ���ߴ���
    ret = m_pTradeApi->ReqTradingAccountPasswordUpdate(&pTradingAccountPasswordUpdate->Thost, nRequestID);  
    LOG_INFO("ReqTradingAccountPasswordUpdate(�ʽ��˻������������):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],AccountID=[%s],OldPassword=[%s],NewPassword=[%s]",
                     ret, nRequestID,
                     pTradingAccountPasswordUpdate->Thost.BrokerID,
                     pTradingAccountPasswordUpdate->Thost.AccountID,
                     pTradingAccountPasswordUpdate->Thost.OldPassword,
                     pTradingAccountPasswordUpdate->Thost.NewPassword);
    return ret;
}

///����¼������
int CPlatFormService::ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    pInputOrder->RequestID = nRequestID;
    strncpy(pInputOrder->BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pInputOrder->BrokerID)-1); ///���͹�˾����       
    strncpy(pInputOrder->InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pInputOrder->InvestorID)-1);  ///Ͷ���ߴ���
    strncpy(pInputOrder->UserID, m_PlatformParam.UserID.c_str(), sizeof(pInputOrder->UserID)-1);  ///�û�����
    //pInputOrder->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	sprintf(pInputOrder->OrderRef,"%12d",++m_CurOrderRef);
	
    ret = m_pTradeApi->ReqOrderInsert(pInputOrder, nRequestID);  
    LOG_INFO("ReqOrderInsert(����¼������):ret=[%d],nRequestID=[%d]\n"
             "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
             "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
             "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
             "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
             "\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
             ret, nRequestID,
             pInputOrder->BrokerID, pInputOrder->InvestorID, pInputOrder->InstrumentID, pInputOrder->OrderRef,pInputOrder->UserID,
             pInputOrder->OrderPriceType, pInputOrder->Direction, pInputOrder->CombOffsetFlag, pInputOrder->CombHedgeFlag,pInputOrder->LimitPrice,
             pInputOrder->VolumeTotalOriginal, pInputOrder->TimeCondition, pInputOrder->GTDDate, pInputOrder->VolumeCondition,pInputOrder->MinVolume,
             pInputOrder->ContingentCondition, pInputOrder->StopPrice, pInputOrder->ForceCloseReason, pInputOrder->IsAutoSuspend,pInputOrder->BusinessUnit,
             pInputOrder->RequestID, pInputOrder->UserForceClose);
    return ret;
}


///Ԥ��¼������
int CPlatFormService::ReqParkedOrderInsert(PlatformStru_ParkedOrder *pParkedOrder, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    pParkedOrder->RequestID = nRequestID;
    strncpy(pParkedOrder->BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pParkedOrder->BrokerID)-1); ///���͹�˾����       
    strncpy(pParkedOrder->InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pParkedOrder->InvestorID)-1);  ///Ͷ���ߴ���
    strncpy(pParkedOrder->UserID, m_PlatformParam.UserID.c_str(), sizeof(pParkedOrder->UserID)-1);  ///�û�����
    //pInputOrder->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	sprintf(pParkedOrder->OrderRef,"%12d",++m_CurOrderRef);

	///Ԥ��
	CThostFtdcParkedOrderField req={0};
	
	strncpy(req.BrokerID, pParkedOrder->BrokerID, sizeof(req.BrokerID));///���͹�˾����
	strncpy(req.InvestorID, pParkedOrder->InvestorID, sizeof(req.InvestorID));///Ͷ���ߴ���
	strncpy(req.InstrumentID, pParkedOrder->InstrumentID, sizeof(req.InstrumentID));///��Լ����
	strncpy(req.OrderRef, pParkedOrder->OrderRef, sizeof(req.OrderRef));///��������
	strncpy(req.UserID, pParkedOrder->UserID, sizeof(req.UserID));///�û�����
	req.OrderPriceType = pParkedOrder->OrderPriceType;///�����۸�����
	req.Direction = pParkedOrder->Direction;///��������
	strncpy(req.CombOffsetFlag, pParkedOrder->CombOffsetFlag, sizeof(req.CombOffsetFlag));///��Ͽ�ƽ��־
	strncpy(req.CombHedgeFlag, pParkedOrder->CombHedgeFlag, sizeof(req.CombHedgeFlag));///���Ͷ���ױ���־
	req.LimitPrice = pParkedOrder->LimitPrice;///�۸�
	req.VolumeTotalOriginal = pParkedOrder->VolumeTotalOriginal;///����
	req.TimeCondition = pParkedOrder->TimeCondition;///��Ч������
	strncpy(req.GTDDate, pParkedOrder->GTDDate, sizeof(req.GTDDate));///GTD����
	req.VolumeCondition = pParkedOrder->VolumeCondition;///�ɽ�������
	req.MinVolume = pParkedOrder->MinVolume;///��С�ɽ���
	req.ContingentCondition = pParkedOrder->ContingentCondition;///��������
	req.StopPrice = pParkedOrder->StopPrice;///ֹ���
	req.ForceCloseReason = pParkedOrder->ForceCloseReason;///ǿƽԭ��
	req.IsAutoSuspend = pParkedOrder->IsAutoSuspend;///�Զ������־
	strncpy(req.BusinessUnit, pParkedOrder->BusinessUnit, sizeof(req.BusinessUnit));///ҵ��Ԫ
	req.RequestID = pParkedOrder->RequestID;///������
	req.UserForceClose = pParkedOrder->UserForceClose;///�û�ǿ����־
	strncpy(req.ExchangeID, pParkedOrder->ExchangeID, sizeof(req.ExchangeID));///����������
	strncpy(req.ParkedOrderID, pParkedOrder->ParkedOrderID, sizeof(req.ParkedOrderID));///Ԥ�񱨵����
	req.UserType = pParkedOrder->UserType;///�û�����
	req.Status = pParkedOrder->Status;///Ԥ��״̬
	req.ErrorID = pParkedOrder->ErrorID;///�������
	strncpy(req.ErrorMsg, pParkedOrder->ErrorMsg, sizeof(req.ErrorMsg));///������Ϣ

    ret = m_pTradeApi->ReqParkedOrderInsert(&req, nRequestID);  
    LOG_INFO("ReqOrderInsert(Ԥ�񱨵�¼������):ret=[%d],nRequestID=[%d]\n"
             "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
             "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
             "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
             "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
             "\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
             ret, nRequestID,
             pParkedOrder->BrokerID, pParkedOrder->InvestorID, pParkedOrder->InstrumentID, pParkedOrder->OrderRef,pParkedOrder->UserID,
             pParkedOrder->OrderPriceType, pParkedOrder->Direction, pParkedOrder->CombOffsetFlag, pParkedOrder->CombHedgeFlag,pParkedOrder->LimitPrice,
             pParkedOrder->VolumeTotalOriginal, pParkedOrder->TimeCondition, pParkedOrder->GTDDate, pParkedOrder->VolumeCondition,pParkedOrder->MinVolume,
             pParkedOrder->ContingentCondition, pParkedOrder->StopPrice, pParkedOrder->ForceCloseReason, pParkedOrder->IsAutoSuspend,pParkedOrder->BusinessUnit,
             pParkedOrder->RequestID, pParkedOrder->UserForceClose);
    return ret;
}


///Ԥ�񳷵�¼������
int CPlatFormService::ReqParkedOrderAction(PlatformStru_ParkedOrderAction *pParkedOrderAction, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    pParkedOrderAction->Thost.RequestID = nRequestID;
    strncpy(pParkedOrderAction->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pParkedOrderAction->Thost.BrokerID)-1); ///���͹�˾����       
    strncpy(pParkedOrderAction->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pParkedOrderAction->Thost.InvestorID)-1);  ///Ͷ���ߴ���
    pParkedOrderAction->Thost.ActionFlag = THOST_FTDC_AF_Delete;
    ret = m_pTradeApi->ReqParkedOrderAction(&pParkedOrderAction->Thost, nRequestID);  
    LOG_INFO("ReqParkedOrderAction(Ԥ�񳷵�¼������):ret=[%d],nRequestID=[%d]\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
                     "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
                     "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
                     ret, nRequestID,
                     pParkedOrderAction->Thost.BrokerID, pParkedOrderAction->Thost.InvestorID, pParkedOrderAction->Thost.OrderActionRef, pParkedOrderAction->Thost.OrderRef,pParkedOrderAction->Thost.RequestID,
                     pParkedOrderAction->Thost.FrontID, pParkedOrderAction->Thost.SessionID, pParkedOrderAction->Thost.ExchangeID, pParkedOrderAction->Thost.OrderSysID,pParkedOrderAction->Thost.ActionFlag,
                     pParkedOrderAction->Thost.LimitPrice, pParkedOrderAction->Thost.VolumeChange, pParkedOrderAction->Thost.UserID, pParkedOrderAction->Thost.InstrumentID);
    return ret;
}

///������������
///
int CPlatFormService::ReqOrderAction(PlatformStru_InputOrderAction *pInputOrderAction, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    pInputOrderAction->Thost.RequestID = nRequestID;
    strncpy(pInputOrderAction->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pInputOrderAction->Thost.BrokerID)-1); ///���͹�˾����       
    strncpy(pInputOrderAction->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pInputOrderAction->Thost.InvestorID)-1);  ///Ͷ���ߴ���
    pInputOrderAction->Thost.ActionFlag = THOST_FTDC_AF_Delete;
    ret = m_pTradeApi->ReqOrderAction(&pInputOrderAction->Thost, nRequestID); 
    LOG_INFO("ReqOrderAction(������������):ret=[%d],nRequestID=[%d]\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
                     "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
                     "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
                     ret, nRequestID,
                     pInputOrderAction->Thost.BrokerID, pInputOrderAction->Thost.InvestorID, pInputOrderAction->Thost.OrderActionRef, pInputOrderAction->Thost.OrderRef,pInputOrderAction->Thost.RequestID,
                     pInputOrderAction->Thost.FrontID, pInputOrderAction->Thost.SessionID, pInputOrderAction->Thost.ExchangeID, pInputOrderAction->Thost.OrderSysID,pInputOrderAction->Thost.ActionFlag,
                     pInputOrderAction->Thost.LimitPrice, pInputOrderAction->Thost.VolumeChange, pInputOrderAction->Thost.UserID, pInputOrderAction->Thost.InstrumentID);
    return ret;
}

///��ѯ��󱨵���������
int CPlatFormService::ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume *pQueryMaxOrderVolume, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQueryMaxOrderVolume(&pQueryMaxOrderVolume->Thost, nRequestID);  
    return ret;
}

///Ͷ���߽�����ȷ��
int CPlatFormService::ReqSettlementInfoConfirm()
{
    int ret = -999;
	CHECK_TRADE_STATUS();
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_PlatformParam.BrokerID.c_str());
	strcpy(req.InvestorID, m_PlatformParam.InvestorID.c_str());
	int nRequestID=m_PlatformParam.RequestID++;
    ret = m_pTradeApi->ReqSettlementInfoConfirm(&req, nRequestID);
    LOG_INFO("ReqSettlementInfoConfirm(Ͷ���߽�����ȷ��):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ConfirmDate=[%s],\t ConfirmTime=[%s]",
                     ret, nRequestID, 
                     req.BrokerID,
                     req.InvestorID,
                     req.ConfirmDate,
                     req.ConfirmTime);
    return ret;
}


///����ɾ��Ԥ��
int CPlatFormService::ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder *pRemoveParkedOrder, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pRemoveParkedOrder->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pRemoveParkedOrder->Thost.BrokerID)-1); ///���͹�˾����       
    strncpy(pRemoveParkedOrder->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pRemoveParkedOrder->Thost.InvestorID)-1);  ///Ͷ���ߴ���
    ret = m_pTradeApi->ReqRemoveParkedOrder(&pRemoveParkedOrder->Thost, nRequestID);  
    LOG_INFO("ReqRemoveParkedOrder(����ɾ��Ԥ��):ret=[%d],nRequestID=[%d]\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ParkedOrderID=[%s]",
                     ret, nRequestID,
                     pRemoveParkedOrder->Thost.BrokerID, pRemoveParkedOrder->Thost.InvestorID, pRemoveParkedOrder->Thost.ParkedOrderID);
    return ret;
}

///����ɾ��Ԥ�񳷵�
int CPlatFormService::ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction *pRemoveParkedOrderAction, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pRemoveParkedOrderAction->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pRemoveParkedOrderAction->Thost.BrokerID)-1); ///���͹�˾����       
    strncpy(pRemoveParkedOrderAction->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pRemoveParkedOrderAction->Thost.InvestorID)-1);  ///Ͷ���ߴ���
    ret = m_pTradeApi->ReqRemoveParkedOrderAction(&pRemoveParkedOrderAction->Thost, nRequestID);  
    LOG_INFO("ReqRemoveParkedOrderAction(����ɾ��Ԥ�񳷵�):ret=[%d],nRequestID=[%d]\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ParkedOrderActionID=[%s]",
                     ret, nRequestID,
                     pRemoveParkedOrderAction->Thost.BrokerID, pRemoveParkedOrderAction->Thost.InvestorID, pRemoveParkedOrderAction->Thost.ParkedOrderActionID);
    return ret;
}

///�����ѯ����
int CPlatFormService::ReqQryOrder(PlatformStru_QryOrder *pQryOrder, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryOrder(&pQryOrder->Thost, nRequestID);
    LOG_INFO("ReqQryOrder(�����ѯ����):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s],\t OrderSysID=[%s],\n"
                     "\t\t\t InsertTimeStart=[%s],\t InsertTimeEnd=[%s]",
                     ret, nRequestID,
                     pQryOrder->Thost.BrokerID,
                     pQryOrder->Thost.InvestorID,
                     pQryOrder->Thost.InstrumentID,
                     pQryOrder->Thost.ExchangeID,
                     pQryOrder->Thost.OrderSysID,
                     pQryOrder->Thost.InsertTimeStart,
                     pQryOrder->Thost.InsertTimeEnd);
    return ret;
}

///�����ѯ�ɽ�
int CPlatFormService::ReqQryTrade(PlatformStru_QryTrade *pQryTrade, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryTrade(&pQryTrade->Thost, nRequestID);
    LOG_INFO("ReqQryTrade(�����ѯ�ɽ�):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s],\t TradeID=[%s],\n"
                     "\t\t\t TradeTimeStart=[%s],\t TradeTimeEnd=[%s]",
                     ret, nRequestID,
                     pQryTrade->Thost.BrokerID,
                     pQryTrade->Thost.InvestorID,
                     pQryTrade->Thost.InstrumentID,
                     pQryTrade->Thost.ExchangeID,
                     pQryTrade->Thost.TradeID,
                     pQryTrade->Thost.TradeTimeStart,
                     pQryTrade->Thost.TradeTimeEnd);
    return ret;
}

///�����ѯͶ���ֲ߳�
int CPlatFormService::ReqQryInvestorPosition(PlatformStru_QryInvestorPosition *pQryInvestorPosition, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryInvestorPosition(pQryInvestorPosition, nRequestID);  
    LOG_INFO("ReqQryInvestorPosition(�����ѯͶ���ֲ߳�):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
                     ret, nRequestID,
                     pQryInvestorPosition->BrokerID,
                     pQryInvestorPosition->InvestorID,
                     pQryInvestorPosition->InstrumentID);
    return ret;
}

///�����ѯ�ʽ��˻�
int CPlatFormService::ReqQryTradingAccount(PlatformStru_QryTradingAccount *pQryTradingAccount, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryTradingAccount(&pQryTradingAccount->Thost, nRequestID);  
    LOG_INFO("ReqQryTradingAccount(�����ѯ�ʽ��˻�):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
                     ret, nRequestID,
                     pQryTradingAccount->Thost.BrokerID,
                     pQryTradingAccount->Thost.InvestorID);
    return ret;
}

///�����ѯͶ����
int CPlatFormService::ReqQryInvestor(PlatformStru_QryInvestor *pQryInvestor, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryInvestor->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryInvestor->Thost.BrokerID)-1); ///���͹�˾����    
    strncpy(pQryInvestor->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryInvestor->Thost.InvestorID)-1);  ///Ͷ���ߴ���
    ret = m_pTradeApi->ReqQryInvestor(&pQryInvestor->Thost, nRequestID);  
    LOG_INFO("ReqQryInvestor(�����ѯͶ����):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
                     ret, nRequestID,
                     pQryInvestor->Thost.BrokerID,
                     pQryInvestor->Thost.InvestorID);
    return ret;
}

///�����ѯ���ױ���
int CPlatFormService::ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryTradingCode(pQryTradingCode, nRequestID);  
    return ret;
}

///�����ѯ��Լ��֤����
int CPlatFormService::ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryInstrumentMarginRate->BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryInstrumentMarginRate->BrokerID)-1); ///���͹�˾����    
	strncpy(pQryInstrumentMarginRate->InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryInstrumentMarginRate->InvestorID)-1);  ///Ͷ���ߴ���
    if(pQryInstrumentMarginRate->HedgeFlag==0) pQryInstrumentMarginRate->HedgeFlag=THOST_FTDC_HF_Speculation;
    ret = m_pTradeApi->ReqQryInstrumentMarginRate(pQryInstrumentMarginRate, nRequestID);  
    LOG_INFO("ReqQryInstrumentMarginRate(�����ѯ��Լ��֤����):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t HedgeFlag=[%d]",
                     ret, nRequestID,
                     pQryInstrumentMarginRate->BrokerID,
                     pQryInstrumentMarginRate->InvestorID,
                     pQryInstrumentMarginRate->InstrumentID,
                     pQryInstrumentMarginRate->HedgeFlag);
    return ret;
}

///�����ѯ��Լ��������
int CPlatFormService::ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryInstrumentCommissionRate->BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryInstrumentCommissionRate->BrokerID)-1); ///���͹�˾����    
	strncpy(pQryInstrumentCommissionRate->InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryInstrumentCommissionRate->InvestorID)-1);  ///Ͷ���ߴ���

    ret = m_pTradeApi->ReqQryInstrumentCommissionRate(pQryInstrumentCommissionRate, nRequestID);  
    LOG_INFO("ReqQryInstrumentCommissionRate(�����ѯ��Լ��������):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
                     ret, nRequestID,
                     pQryInstrumentCommissionRate->BrokerID,
                     pQryInstrumentCommissionRate->InvestorID,
                     pQryInstrumentCommissionRate->InstrumentID);
    return ret;
}

///�����ѯ������
int CPlatFormService::ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryExchange(pQryExchange, nRequestID);  
    return ret;
}

///�����ѯ��Լ
int CPlatFormService::ReqQryInstrument(PlatformStru_QryInstrument *pQryInstrument, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryInstrument(&pQryInstrument->Thost, nRequestID); 
    LOG_INFO("ReqQryInstrument(�����ѯ��Լ):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t InstrumentID=[%s],\t ExchangeID=[%s],\t ExchangeInstID=[%s],\tProductID=[%s]",
                     ret, nRequestID, 
                     pQryInstrument->Thost.InstrumentID,
                     pQryInstrument->Thost.ExchangeID,
                     pQryInstrument->Thost.ExchangeInstID,
                     pQryInstrument->Thost.ProductID);

    return ret;
}

///�����ѯ����
int CPlatFormService::ReqQryDepthMarketData(PlatformStru_QryDepthMarketData *pQryDepthMarketData, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryDepthMarketData(&pQryDepthMarketData->Thost, nRequestID);  
    return ret;
}

///�����ѯͶ���߽�����
int CPlatFormService::ReqQrySettlementInfo(PlatformStru_QrySettlementInfo *pQrySettlementInfo, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQrySettlementInfo->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQrySettlementInfo->Thost.BrokerID)-1); ///���͹�˾����       
    strncpy(pQrySettlementInfo->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQrySettlementInfo->Thost.InvestorID)-1);  ///Ͷ���ߴ���
    ret = m_pTradeApi->ReqQrySettlementInfo(&pQrySettlementInfo->Thost, nRequestID); 
    LOG_INFO("ReqQrySettlementInfo(�����ѯͶ���߽�����):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t TradingDay=[%s]",
                     ret, nRequestID, 
                     pQrySettlementInfo->Thost.BrokerID,
                     pQrySettlementInfo->Thost.InvestorID,
                     pQrySettlementInfo->Thost.TradingDay);
    return ret;
}

///�����ѯת������
int CPlatFormService::ReqQryTransferBank(PlatformStru_QryTransferBank *pQryTransferBank, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryTransferBank(&pQryTransferBank->Thost, nRequestID);  
    return ret;
}

///�����ѯͶ���ֲ߳���ϸ
int CPlatFormService::ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail *pQryInvestorPositionDetail, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryInvestorPositionDetail(&pQryInvestorPositionDetail->Thost, nRequestID);  
    LOG_INFO("ReqQryInvestorPositionDetail(�����ѯͶ���ֲ߳���ϸ):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
                     ret, nRequestID,
                     pQryInvestorPositionDetail->Thost.BrokerID,
                     pQryInvestorPositionDetail->Thost.InvestorID,
                     pQryInvestorPositionDetail->Thost.InstrumentID);
    return ret;
}

///�����ѯ�ͻ�֪ͨ
int CPlatFormService::ReqQryNotice(PlatformStru_QryNotice *pQryNotice, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryNotice(&pQryNotice->Thost, nRequestID);  
    return ret;
}

///�����ѯ������Ϣȷ��
int CPlatFormService::ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm *pQrySettlementInfoConfirm, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQrySettlementInfoConfirm(&pQrySettlementInfoConfirm->Thost, nRequestID);  
    LOG_INFO("ReqQrySettlementInfoConfirm(�����ѯ������Ϣȷ��):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
                     ret, nRequestID,
                     pQrySettlementInfoConfirm->Thost.BrokerID,
                     pQrySettlementInfoConfirm->Thost.InvestorID);
    return ret;
}

///�����ѯͶ���ֲ߳���ϸ
int CPlatFormService::ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail *pQryInvestorPositionCombineDetail, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryInvestorPositionCombineDetail(&pQryInvestorPositionCombineDetail->Thost, nRequestID);  
    return ret;
}

///�����ѯת����ˮ
int CPlatFormService::ReqQryTransferSerial(PlatformStru_QryTransferSerial *pQryTransferSerial, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryTransferSerial->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryTransferSerial->Thost.BrokerID)-1); ///���͹�˾����    
    strncpy(pQryTransferSerial->Thost.AccountID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryTransferSerial->Thost.AccountID)-1);  ///Ͷ���ߴ���
    ret = m_pTradeApi->ReqQryTransferSerial(&pQryTransferSerial->Thost, nRequestID); 
    LOG_INFO("ReqQryTransferSerial(�����ѯת����ˮ) : ret=[%d],nRequestID=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t AccountID=[%s],\t BankID=[%s]",
                        ret,nRequestID,
                        pQryTransferSerial->Thost.BrokerID,
                        pQryTransferSerial->Thost.AccountID,
						pQryTransferSerial->Thost.BankID);

    return ret;
}

///�����ѯǩԼ����
int CPlatFormService::ReqQryContractBank(PlatformStru_QryContractBank *pQryContractBank, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryContractBank->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryContractBank->Thost.BrokerID)-1); ///���͹�˾����    

    ret = m_pTradeApi->ReqQryContractBank(&pQryContractBank->Thost, nRequestID);  
    LOG_INFO("ReqQryContractBank(�����ѯǩԼ����) : ret=[%d],nRequestID=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t BankID=[%s],\t BankBrchID=[%s]",
                        ret,nRequestID,
                        pQryContractBank->Thost.BrokerID,
                        pQryContractBank->Thost.BankID,
						pQryContractBank->Thost.BankBrchID);
    return ret;
}
    
///�����ѯ����ǩԼ��ϵ
int CPlatFormService::ReqQryAccountregister(PlatformStru_QryAccountRegister *pQryAccountregister, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryAccountregister->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryAccountregister->Thost.BrokerID)-1); ///���͹�˾����    
    strncpy(pQryAccountregister->Thost.AccountID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryAccountregister->Thost.AccountID)-1);  ///Ͷ���ߴ���

    ret = m_pTradeApi->ReqQryAccountregister(&pQryAccountregister->Thost, nRequestID);  
    LOG_INFO("ReqQryAccountregister(�����ѯ����ǩԼ��ϵ) : ret=[%d],nRequestID=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t AccountID=[%s],\t BankID=[%s]",
                        ret,nRequestID,
                        pQryAccountregister->Thost.BrokerID,
                        pQryAccountregister->Thost.AccountID,
						pQryAccountregister->Thost.BankID);
    return ret;
}

///�����ѯԤ��
int CPlatFormService::ReqQryParkedOrder(PlatformStru_QryParkedOrder *pQryParkedOrder, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryParkedOrder->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryParkedOrder->Thost.BrokerID)-1); ///���͹�˾����       
    strncpy(pQryParkedOrder->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryParkedOrder->Thost.InvestorID)-1);  ///Ͷ���ߴ���
    ret = m_pTradeApi->ReqQryParkedOrder(&pQryParkedOrder->Thost, nRequestID);  
    LOG_INFO("ReqQryParkedOrder(�����ѯԤ��):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s]",
                     ret, nRequestID,
                     pQryParkedOrder->Thost.BrokerID,
                     pQryParkedOrder->Thost.InvestorID,
                     pQryParkedOrder->Thost.InstrumentID,
                     pQryParkedOrder->Thost.ExchangeID);
    return ret;
}

///�����ѯԤ�񳷵�
int CPlatFormService::ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction *pQryParkedOrderAction, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryParkedOrderAction->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryParkedOrderAction->Thost.BrokerID)-1); ///���͹�˾����       
    strncpy(pQryParkedOrderAction->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryParkedOrderAction->Thost.InvestorID)-1);  ///Ͷ���ߴ���
    ret = m_pTradeApi->ReqQryParkedOrderAction(&pQryParkedOrderAction->Thost, nRequestID);  
    LOG_INFO("ReqQryParkedOrderAction(�����ѯԤ�񳷵�):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s]",
                     ret, nRequestID,
                     pQryParkedOrderAction->Thost.BrokerID,
                     pQryParkedOrderAction->Thost.InvestorID,
                     pQryParkedOrderAction->Thost.InstrumentID,
                     pQryParkedOrderAction->Thost.ExchangeID);
    return ret;
}

///�����ѯ����֪ͨ
int CPlatFormService::ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryTradingNotice(pQryTradingNotice, nRequestID);  
    return ret;
}

///�����ѯ���͹�˾���ײ���
int CPlatFormService::ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryBrokerTradingParams(pQryBrokerTradingParams, nRequestID);  
    return ret;
}

///�����ѯ���͹�˾�����㷨
int CPlatFormService::ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryBrokerTradingAlgos(pQryBrokerTradingAlgos, nRequestID);  
    return ret;
}

///�ڻ����������ʽ�ת�ڻ�����
int CPlatFormService::ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer *pReqTransfer, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pReqTransfer->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pReqTransfer->Thost.BrokerID)-1); ///���͹�˾����    
    strncpy(pReqTransfer->Thost.AccountID, m_PlatformParam.InvestorID.c_str(), sizeof(pReqTransfer->Thost.AccountID)-1);  ///Ͷ���ߴ���
    ret = m_pTradeApi->ReqFromBankToFutureByFuture(&pReqTransfer->Thost, nRequestID);  
    LOG_INFO("ReqFromBankToFutureByFuture(�ڻ����������ʽ�ת�ڻ�����) : ret=[%d],nRequestID=[%d],\n"
                        "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
                        "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
                        "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
                        "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[%s],\n"
                        "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
                        "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
                        "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
                        "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
                        "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d]",
                        ret,nRequestID,
                        pReqTransfer->Thost.TradeCode, pReqTransfer->Thost.BankID,pReqTransfer->Thost.BankBranchID,pReqTransfer->Thost.BrokerID,pReqTransfer->Thost.BrokerBranchID,
                        pReqTransfer->Thost.TradeDate, pReqTransfer->Thost.TradeTime,pReqTransfer->Thost.BankSerial,pReqTransfer->Thost.TradingDay,pReqTransfer->Thost.PlateSerial,
                        pReqTransfer->Thost.LastFragment, pReqTransfer->Thost.SessionID,pReqTransfer->Thost.CustomerName,pReqTransfer->Thost.IdCardType,pReqTransfer->Thost.IdentifiedCardNo,
                        pReqTransfer->Thost.CustType, pReqTransfer->Thost.BankAccount,pReqTransfer->Thost.BankPassWord,pReqTransfer->Thost.AccountID,pReqTransfer->Thost.Password,
                        pReqTransfer->Thost.InstallID,pReqTransfer->Thost.FutureSerial, pReqTransfer->Thost.UserID,pReqTransfer->Thost.VerifyCertNoFlag,pReqTransfer->Thost.CurrencyID,
                        pReqTransfer->Thost.TradeAmount,pReqTransfer->Thost.FutureFetchAmount, pReqTransfer->Thost.FeePayFlag,pReqTransfer->Thost.CustFee,pReqTransfer->Thost.BrokerFee,
                        pReqTransfer->Thost.Message, pReqTransfer->Thost.Digest, pReqTransfer->Thost.BankAccType,pReqTransfer->Thost.DeviceID,pReqTransfer->Thost.BankSecuAccType,
                        pReqTransfer->Thost.BrokerIDByBank, pReqTransfer->Thost.BankSecuAcc, pReqTransfer->Thost.BankPwdFlag,pReqTransfer->Thost.SecuPwdFlag,pReqTransfer->Thost.OperNo,
                        pReqTransfer->Thost.RequestID, pReqTransfer->Thost.TID, pReqTransfer->Thost.TransferStatus
                        );
    return ret;
}

///�ڻ������ڻ��ʽ�ת��������
int CPlatFormService::ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer *pReqTransfer, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pReqTransfer->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pReqTransfer->Thost.BrokerID)-1); ///���͹�˾����    
    strncpy(pReqTransfer->Thost.AccountID, m_PlatformParam.InvestorID.c_str(), sizeof(pReqTransfer->Thost.AccountID)-1);  ///Ͷ���ߴ���
    ret = m_pTradeApi->ReqFromFutureToBankByFuture(&pReqTransfer->Thost, nRequestID);  
    LOG_INFO("ReqFromFutureToBankByFuture(�ڻ������ڻ��ʽ�ת��������) : ret=[%d],nRequestID=[%d],\n"
                        "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
                        "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
                        "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
                        "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[%s],\n"
                        "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
                        "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
                        "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
                        "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
                        "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d]",
                        ret,nRequestID,
                        pReqTransfer->Thost.TradeCode, pReqTransfer->Thost.BankID,pReqTransfer->Thost.BankBranchID,pReqTransfer->Thost.BrokerID,pReqTransfer->Thost.BrokerBranchID,
                        pReqTransfer->Thost.TradeDate, pReqTransfer->Thost.TradeTime,pReqTransfer->Thost.BankSerial,pReqTransfer->Thost.TradingDay,pReqTransfer->Thost.PlateSerial,
                        pReqTransfer->Thost.LastFragment, pReqTransfer->Thost.SessionID,pReqTransfer->Thost.CustomerName,pReqTransfer->Thost.IdCardType,pReqTransfer->Thost.IdentifiedCardNo,
                        pReqTransfer->Thost.CustType, pReqTransfer->Thost.BankAccount,pReqTransfer->Thost.BankPassWord,pReqTransfer->Thost.AccountID,pReqTransfer->Thost.Password,
                        pReqTransfer->Thost.InstallID,pReqTransfer->Thost.FutureSerial, pReqTransfer->Thost.UserID,pReqTransfer->Thost.VerifyCertNoFlag,pReqTransfer->Thost.CurrencyID,
                        pReqTransfer->Thost.TradeAmount,pReqTransfer->Thost.FutureFetchAmount, pReqTransfer->Thost.FeePayFlag,pReqTransfer->Thost.CustFee,pReqTransfer->Thost.BrokerFee,
                        pReqTransfer->Thost.Message, pReqTransfer->Thost.Digest, pReqTransfer->Thost.BankAccType,pReqTransfer->Thost.DeviceID,pReqTransfer->Thost.BankSecuAccType,
                        pReqTransfer->Thost.BrokerIDByBank, pReqTransfer->Thost.BankSecuAcc, pReqTransfer->Thost.BankPwdFlag,pReqTransfer->Thost.SecuPwdFlag,pReqTransfer->Thost.OperNo,
                        pReqTransfer->Thost.RequestID, pReqTransfer->Thost.TID, pReqTransfer->Thost.TransferStatus
                        );
    return ret;
}

///�ڻ������ѯ�����������
int CPlatFormService::ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount *pReqQueryAccount, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pReqQueryAccount->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pReqQueryAccount->Thost.BrokerID)-1); ///���͹�˾����    
    strncpy(pReqQueryAccount->Thost.AccountID, m_PlatformParam.InvestorID.c_str(), sizeof(pReqQueryAccount->Thost.AccountID)-1);  ///Ͷ���ߴ���
    ret = m_pTradeApi->ReqQueryBankAccountMoneyByFuture(&pReqQueryAccount->Thost, nRequestID); 
    LOG_INFO("ReqQueryBankAccountMoneyByFuture(�ڻ������ѯ�����������) : ret=[%d],nRequestID=[%d],\n"
                        "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
                        "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
                        "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
                        "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[%s],\n"
                        "\t\t\t FutureSerial=[%d],\t InstallID=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
                        "\t\t\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],BrokerIDByBank=[%s],\n"
                        "\t\t\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],RequestID=[%d],\n"
                        "\t\t\t TID=[%d]",
                        ret,nRequestID,
                        pReqQueryAccount->Thost.TradeCode, pReqQueryAccount->Thost.BankID,pReqQueryAccount->Thost.BankBranchID,pReqQueryAccount->Thost.BrokerID,pReqQueryAccount->Thost.BrokerBranchID,
                        pReqQueryAccount->Thost.TradeDate, pReqQueryAccount->Thost.TradeTime,pReqQueryAccount->Thost.BankSerial,pReqQueryAccount->Thost.TradingDay,pReqQueryAccount->Thost.PlateSerial,
                        pReqQueryAccount->Thost.LastFragment, pReqQueryAccount->Thost.SessionID,pReqQueryAccount->Thost.CustomerName,pReqQueryAccount->Thost.IdCardType,pReqQueryAccount->Thost.IdentifiedCardNo,
                        pReqQueryAccount->Thost.CustType, pReqQueryAccount->Thost.BankAccount,pReqQueryAccount->Thost.BankPassWord,pReqQueryAccount->Thost.AccountID,pReqQueryAccount->Thost.Password,
                        pReqQueryAccount->Thost.FutureSerial, pReqQueryAccount->Thost.InstallID,pReqQueryAccount->Thost.UserID,pReqQueryAccount->Thost.VerifyCertNoFlag,pReqQueryAccount->Thost.CurrencyID,
                        pReqQueryAccount->Thost.Digest, pReqQueryAccount->Thost.BankAccType,pReqQueryAccount->Thost.DeviceID,pReqQueryAccount->Thost.BankSecuAccType,pReqQueryAccount->Thost.BrokerIDByBank,
                        pReqQueryAccount->Thost.BankSecuAcc, pReqQueryAccount->Thost.BankPwdFlag,pReqQueryAccount->Thost.SecuPwdFlag,pReqQueryAccount->Thost.OperNo,pReqQueryAccount->Thost.RequestID,
                        pReqQueryAccount->Thost.TID
                        );
    return ret;
}

int CPlatFormService::SubscribeMarketData(const std::string& InstrumentID)
{
    int ret = -999;
    if(InstrumentID.empty() ||
       !m_pQuotApi          ||
       m_PlatformParam.QuotStatus!=CPlatFormParam::CONNSTATUS_Connected)
    {
        LOG_INFO("����API��δ׼�����������ܵ���δ�ɹ�����������");
        return ret;
    }

    char *pInstrumentID = const_cast<char*>(InstrumentID.c_str());
    m_MarketDataInstrumentID.insert(InstrumentID);
    ret=m_pQuotApi->SubscribeMarketData(&pInstrumentID, 1);
    LOG_INFO("SubscribeMarketData(����ָ����Լ����):ret=[%d],InstrumentID=[%s]",
                     ret, pInstrumentID);

    return ret;

}

int CPlatFormService::UnSubscribeMarketData(const std::string& InstrumentID)
{
    int ret = -999;
    if(InstrumentID.empty() ||
       !m_pQuotApi          ||
       m_PlatformParam.QuotStatus!=CPlatFormParam::CONNSTATUS_Connected)
    {
        LOG_INFO("����API��δ׼�����������ܵ���δ�ɹ�����������");
        return ret;
    }

    char *pInstrumentID = const_cast<char*>(InstrumentID.c_str());
    std::set<std::string>::iterator iter=m_MarketDataInstrumentID.find(InstrumentID);
    if(iter!=m_MarketDataInstrumentID.end()) m_MarketDataInstrumentID.erase(iter);
    ret=m_pQuotApi->UnSubscribeMarketData(&pInstrumentID, 1);
    LOG_INFO("UnSubscribeMarketData(ȡ��ָ����Լ����):ret=[%d],InstrumentID=[%s]",
                     ret, pInstrumentID);
    return ret;
}
//admin  API
int CPlatFormService::LoginTrade(const std::string& InvestorID, const std::string& InvestorPW, const std::string& InvestorNewPwd)
{
    LOG_INFO("LoginTrade:InvestorID=[%s],InvestorPW=[%s]", InvestorID.c_str(), InvestorPW.c_str());
    if(InvestorID.empty() || 
       /*InvestorPW.empty() ||*/
       m_PlatformParam.TradeFrontAddr.empty() ||
       m_PlatformParam.BrokerID.empty() ||
       m_PlatformParam.UserID.empty()/* ||
       m_PlatformParam.UserPW.empty()*/)
    {
        LOG_INFO("LoginTrade:����δ��������");
        return -1;
    }

    if(CPlatFormParam::CONNSTATUS_Connected==m_PlatformParam.TradeStatus)
    {
        LOG_INFO("LoginTrade:�Ѿ�����");
        return 0;//�Ѿ�����
    }
    if(CPlatFormParam::CONNSTATUS_Logining==m_PlatformParam.TradeStatus ||
       CPlatFormParam::CONNSTATUS_Connecting==m_PlatformParam.TradeStatus 
       ||CPlatFormParam::CONNSTATUS_TradeSettlementInfoConfirming==m_PlatformParam.TradeStatus)
    {
        LOG_INFO("LoginTrade:���ڵ�����");
        return -2;//���ڵ�����
    }

    LogoutTrade();

    m_PlatformParam.TradeStatus = CPlatFormParam::CONNSTATUS_Connecting;//��������
    m_PlatformParam.InvestorID = InvestorID;
    m_PlatformParam.InvestorPW = InvestorPW;
	m_PlatformParam.m_strInvestorNewPWD = InvestorNewPwd;

    m_PlatformParam.QuotStatus = CPlatFormParam::CONNSTATUS_Connecting;//��������
	m_pTradeApi = CCmeCoreApi::CreateFtdcApi(); 
	m_pQuotApi = m_pTradeApi;
    m_pTradeSpi = new CTraderSpi(*this);
    m_pTradeApi->RegisterSpi(m_pTradeSpi);
    m_pQuotSpi = new CQuotSpi(*this);
    m_pQuotApi->RegisterSpi(m_pQuotSpi);   
	char FrontAddr[256];
	strcpy(FrontAddr,m_PlatformParam.TradeFrontAddr.c_str());
	strcat(FrontAddr,";");
	strcat(FrontAddr,m_PlatformParam.QuotFrontAddr.c_str());
	m_pTradeApi->SetNewPwd(InvestorNewPwd);
	m_pTradeApi->RegisterFront(FrontAddr);
	m_pTradeApi->Init();
  
    return 0;
}

int CPlatFormService::LoginQuot(const std::string& InvestorID, const std::string& InvestorPW)
{
    LOG_INFO("LoginQuot:InvestorID=[%s],InvestorPW=[%s]", InvestorID.c_str(), InvestorPW.c_str());
    return 0;
}

void CPlatFormService::StopDataCenterThread(void)
{
    if(m_pDataCenter)
        m_pDataCenter->Stop();
}

int CPlatFormService::LogoutTrade()
{
    //LOG_INFO("LogoutTrade");
    if(m_pTradeApi)
    {
        LOG_INFO("LogoutTrade:�رս���API");
        m_PlatformParam.TradeStatus = CPlatFormParam::CONNSTATUS_Disconnecting;
        m_pTradeApi->Release();
		CCmeCoreApi* tempApi=m_pTradeApi;
		m_pTradeApi=NULL;
		m_pQuotApi=NULL;
		delete tempApi;
		tempApi=NULL;
        m_PlatformParam.TradeStatus = CPlatFormParam::CONNSTATUS_Disconnected;	
    }

    if(m_pTradeSpi)
    {
        delete m_pTradeSpi;
        m_pTradeSpi=NULL;
        m_pQuotSpi=NULL;
    }
    
    return 0;
}

int CPlatFormService::LogoutQuot()
{
    LOG_INFO("LogoutQuot");
    return 0;
}

///�����Ƿ��ǵ�ǰ����
int CPlatFormService::IsValidPW(const std::string& yourPW)
{
	if(m_PlatformParam.InvestorPW==yourPW)
		return 1;
	else
	    return -1;
}
int CPlatFormService::GetBrokerIDUserID(std::string& UserID, std::string& BrokerID, set<string>* accounts)
{
	
	BrokerID=m_PlatformParam.BrokerID;
	UserID=m_PlatformParam.UserID;
	if(accounts) {
		*accounts = m_PlatformParam.Accounts;
	}
	return 0;

}
int CPlatFormService::SetTradeFrontAddr(const std::string& addr)
{
    m_PlatformParam.TradeFrontAddr = addr;
	if(m_PlatformParam.TradeFrontAddr.find("tcp://")==0)
		m_PlatformParam.TradeFrontAddr=m_PlatformParam.TradeFrontAddr.substr(6);
    return 0;
}

int CPlatFormService::SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr)
{
    m_PlatformParam.TradeFrontAddrs=TradeAddr;
    return 0;
}


int CPlatFormService::SetQuotFrontAddr(const std::string& addr)
{
    m_PlatformParam.QuotFrontAddr = addr;
	if(m_PlatformParam.QuotFrontAddr.find("tcp://")==0)
		m_PlatformParam.QuotFrontAddr=m_PlatformParam.QuotFrontAddr.substr(6);
    return 0;
}

int CPlatFormService::SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr)
{
    m_PlatformParam.QuotFrontAddrs=QuotAddr;
    return 0;
}

int CPlatFormService::SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, const std::string& UserPW, const std::string& OneTimePassword)
{
    m_PlatformParam.BrokerID = BrokerID;
    m_PlatformParam.UserID   = UserID;
    m_PlatformParam.UserPW   = UserPW;
	m_PlatformParam.OneTimePassword=OneTimePassword;
    return 0;
}


CPlatFormService::CPlatFormService()
{
    //m_status = PLATFORM_STATUS_UNINIT;

    m_pWriteLog = new zqWriteLog();
    m_PCache   = new CPackageCache(4*1024*1024, 1);
    m_pDataCenter = new CDataCenter(*m_PCache,this);
    m_pDataCenter->Start();
    m_pTradeApi=NULL;
    m_pTradeSpi=NULL;
    m_pQuotApi=NULL;
    m_pQuotSpi=NULL;
	m_bSettlementInfoConfirmed=false;
	memset(&m_ErrorInfoField,0,sizeof(m_ErrorInfoField));

	m_nOrderReqID = 0;
}

CPlatFormService::~CPlatFormService()
{
    
    LogoutTrade();
    LogoutQuot();


	SAFE_DELETE(m_pDataCenter); 
    SAFE_DELETE(m_PCache);
    SAFE_DELETE(m_pWriteLog);

}
   
///�û���¼����
int CPlatFormService::ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqUserLogin(pReqUserLoginField, nRequestID);  
    LOG_INFO("ReqUserLogin(�û���¼����):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t TradingDay=[%s],\t BrokerID=[%s],\t UserID=[%s],\t Password=[%s],\t UserProductInfo=[%s],\n"
                     "\t\t\t InterfaceProductInfo=[%s],\t ProtocolInfo=[%s]",
                     ret, nRequestID,
                     pReqUserLoginField->TradingDay,
                     pReqUserLoginField->BrokerID,
                     pReqUserLoginField->UserID,
                     pReqUserLoginField->Password,
                     pReqUserLoginField->UserProductInfo,
                     pReqUserLoginField->InterfaceProductInfo,
                     pReqUserLoginField->ProtocolInfo);
    return ret;
}	

///�ǳ�����
int CPlatFormService::ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqUserLogout(pUserLogout, nRequestID);
    LOG_INFO("ReqUserLogout(�ǳ�����):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],UserID=[%s]",
                     ret, nRequestID,
                     pUserLogout->BrokerID,
                     pUserLogout->UserID);
    return ret;
}


///����ҵ������
int CPlatFormService::SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo)
{
    return m_pDataCenter->SubscribeBusinessData(BID, GID, callback, sendCurrentInfo);
}

///ȡ��ҵ�����ݵĶ���
int CPlatFormService::UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID)
{
    return m_pDataCenter->UnSubscribeBusinessData(BID, GID);
}
///��ȡ��Լ�б�
int CPlatFormService::GetInstrumentList(std::vector<GroupInfo> &outData)
{
    if(!m_PlatformParam.HaveAllInstrumentIDs)
        return -1;

    return m_pDataCenter->GetInstrumentList(outData);
}
///��ȡ��Լ�б�
int CPlatFormService::GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID)
{
    if(!m_PlatformParam.HaveAllInstrumentIDs)
        return -1;

    return m_pDataCenter->GetInstrumentList(outData,ExchangeID);
}
//���������Լ�б�
int CPlatFormService::AddMainInstrumentList(std::string instrument)
{
	return m_pDataCenter->AddMainInstrumentList(instrument);
}
///��ȡ������Լ�б�
int CPlatFormService::GetMainInstrumentList(std::set<std::string> &outData)
{
    //if(!m_PlatformParam.HaveAllInstrumentIDs)
    //    return -1;

    return m_pDataCenter->GetMainInstrumentList(outData);
}
///���ú�Լ�ݲ��б�
void CPlatFormService::SetInstrumentVarietyMap(map<string, string>& inData)
{
    if(!m_PlatformParam.HaveAllInstrumentIDs)
        return;
    m_pDataCenter->SetInstrumentVarietyMap(inData);
}

// ��ȡ��Լ�ݲ��б�
int CPlatFormService::GetInstrumentVarietyMap(map<string, string>& outData)
{
    if(!m_PlatformParam.HaveAllInstrumentIDs)
        return -1;
    return m_pDataCenter->GetInstrumentVarietyMap(outData);
}
///��ȡָ��Ʒ�ֵĺ�Լ�б�,���غ�Լ����
int CPlatFormService::GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData)
{
	return m_pDataCenter->GetInstrumentListByProductID(ProductID,outData);
}
///��ȡָ����Լ��Ϣ
int CPlatFormService::GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    return m_pDataCenter->GetInstrumentInfo(InstrumentID, outData);
}
//���ú�Լ����״̬
void CPlatFormService::SetSubscribeStatus(const std::string& InstrumentID,int status,S_Status oper)
{
	switch(oper)
	{
	case S_STATUS_ADD:
		m_SubscribeMap[InstrumentID]|=status;
		SubscribeMarketData(InstrumentID);
		break;
	case S_STATUS_DEL:
		m_SubscribeMap[InstrumentID]&=~status;
		break;
	case S_STATUS_EQU:
		m_SubscribeMap[InstrumentID]=status;
		break;
	}
}
//���º�Լ����״̬
int CPlatFormService::UpdateSubscribeStatus()
{
#ifdef _NEED_MAIN_INSTRUMENT
	std::set<std::string>::iterator it=m_MarketDataInstrumentID.begin();
	for(it;it!=m_MarketDataInstrumentID.end();++it)
	{
		string str=*it;
		if(m_SubscribeMap[str]==0)
		{
			UnSubscribeMarketData(str);
			//TRACE("UpdateSubscribeStatus(%s)\n",it->c_str());
			break;
		}
	}
#endif
	return 0;
}
///����ָ����Լ��Ϣ
void CPlatFormService::SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    m_PlatformParam.HaveAllInstrumentIDs=true;
	m_SubscribeMap[InstrumentID]=0;
    m_pDataCenter->SetInstrumentInfo(InstrumentID, outData);
}
///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
char CPlatFormService::GetProductClassType(const std::string& InstrumentID)
{
    return m_pDataCenter->GetProductClassType(InstrumentID);
}

///��ȡ��ԼƷ�ִ���
int CPlatFormService::GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID)
{
    return m_pDataCenter->GetProductID(outData,ExchangeID);
}

///��ȡ��Լ��������
int CPlatFormService::GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    return m_pDataCenter->GetCommissionRate(InstrumentID, outData);
}

///��ȡ��Լ��֤����
int CPlatFormService::GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    return m_pDataCenter->GetMarginRate(InstrumentID, outData);
}
///���ú�Լ��������
int CPlatFormService::SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    return m_pDataCenter->SetCommissionRate(InstrumentID, outData);
}
///�������ڲ�ѯ�������ʵĺ�Լ
void CPlatFormService::SetReqCommissionRateInstrumentID(const std::string& InstrumentID)
{
    m_pDataCenter->SetReqCommissionRateInstrumentID(InstrumentID);
}

///���ú�Լ��֤����
int CPlatFormService::SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    return m_pDataCenter->SetMarginRate(InstrumentID, outData);
}

///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
int CPlatFormService::GetInstrumentMultiple(const std::string& InstrumentID)
{
    return m_pDataCenter->GetInstrumentMultiple(InstrumentID);
}


///��ȡָ����Լ����
int CPlatFormService::GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    return m_pDataCenter->GetQuotInfo(InstrumentID, outData);
}

///��ȡָ����Լ������
int CPlatFormService::GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    return m_pDataCenter->GetOldQuotInfo(InstrumentID, outData);
}


///��ȡָ����Լ���ּۣ�ʧ�ܷ���0.0
double CPlatFormService::GetCurPrice(const std::string& InstrumentID)
{
    return m_pDataCenter->GetCurPrice(InstrumentID);
}
string CPlatFormService::GetTradingDay()
{
	return m_pDataCenter->GetTradingDay();
}
///��ȡ������ʱ��
unsigned long CPlatFormService::GetExchangeTime(const string& ExchangeID,string& time)
{
    return m_pDataCenter->GetExchangeTime(ExchangeID,time);
}
    ///��ȡȫ��������
int CPlatFormService::GetExchangeIDs(vector<string>& outData)
{
	if(m_pTradeApi==NULL)return 0;
	return m_pTradeApi->GetExchangeIDs(outData);//m_pDataCenter->GetExchangeIDs(outData);
}
///��ȡȫ������
int CPlatFormService::GetAllOrders(std::map<OrderKey,  PlatformStru_OrderInfo>& outData)
{
    return m_pDataCenter->GetAllOrders(outData);
}
int CPlatFormService::GetTradingAccountAvailable(double& fAvailable)
{
    return m_pDataCenter->GetTradingAccountAvailable(fAvailable);
}
int CPlatFormService::GetTradingAccountWithdrawQuota(double& fWithdrawQuota)
{
    return m_pDataCenter->GetTradingAccountWithdrawQuota(fWithdrawQuota);
}
int CPlatFormService::GetTradingAccountID(char* AccountID)
{
    return m_pDataCenter->GetTradingAccountID(AccountID);
}
///��ȡ�����ʽ��˻���Ϣ
int CPlatFormService::GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& outData)
{
	if(m_pTradeApi==NULL)return -1;
	return m_pTradeApi->GetTradingAccount(strAccount, outData);
}
///��ȡ�˻��ʽ��ı���Ϣ
int CPlatFormService::GetAccountText(std::string& outData,int language)
{
	if(m_pTradeApi==NULL)return 0;
	return m_pTradeApi->GetAccountText(outData, language);
}
///���ý����ʽ��˻���Ϣ
void CPlatFormService::SetTradingAccount(const PlatformStru_TradingAccountInfo& inData)
{
    m_pDataCenter->SetTradingAccount(inData);
}

///�����ѯͶ���߽�������Ӧ
int CPlatFormService::GetLastSettlementInfo(std::string& outData)
{
    return m_pDataCenter->GetLastSettlementInfo(outData);
}

///��ȡȫ���ɽ���
int CPlatFormService::GetAllTrades(std::map<TradeKey,  PlatformStru_TradeInfo>& outData)
{
    return m_pDataCenter->GetAllTrades(outData);
}
///��ȡ�ɽ�ͳ��
int CPlatFormService::GetAllTradeTotalDatas(vector<TotalInfo>& outData)
{
    return m_pDataCenter->GetAllTradeTotalDatas(outData);
} 
///�ڳɽ�ͳ�Ʋ��Ҳ���rawData
int CPlatFormService::FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData )
{
    return m_pDataCenter->FindIndexFromAllTradeTotalDatas(rawData);
}    
///��ȡ����ί�е���Ϣ
 int CPlatFormService::GetAllOrdersEx(std::map <AllOrderKey,  PlatformStru_OrderInfo> &AllOrdersMap)
 {
	 return m_pDataCenter->GetAllOrdersEx(AllOrdersMap);
 }


int CPlatFormService::GetPositionDetails(std::multimap<std::string, PositionDetailStru> &PositionDetailMap)
{
    return m_pDataCenter->GetPositionDetails(PositionDetailMap);
}

int CPlatFormService::GetPositionDetails(const std::string& InstrumentID,std::multimap<std::string, PositionDetailStru> &PositionDetailMap)
{
    return m_pDataCenter->GetPositionDetails(InstrumentID,PositionDetailMap);
}

int  CPlatFormService::GetPositionDetails(std::list<PositionDetailStru> &PositionDetailList)
{
	return m_pDataCenter->GetPositionDetails(PositionDetailList);
}

//��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
int CPlatFormService::GetPositionKeyList(std::list<PositionKey> &PositionKeyList)
{
    return m_pDataCenter->GetPositionKeyList(PositionKeyList);
}

//��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
int CPlatFormService::GetPositionKeySet(std::set<PositionKey> &PositionKeySet)
{
    return m_pDataCenter->GetPositionKeySet(PositionKeySet);
}

//��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
int CPlatFormService::GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PositionStru& PositionData)
{
    return m_pDataCenter->GetPositionData(strAccount, InstrumentID,Direction,HedgeFlag,PositionData);
}


int  CPlatFormService::GetWaitOrderVolume(const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag)
{
    return m_pDataCenter->GetWaitOrderVolume(strInstrumentID, Direction, CloseType,HedgeFlag);
}

//��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
int CPlatFormService::GetCloseOrderVolume(const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
    return m_pDataCenter->GetCloseOrderVolume(strInstrumentID, Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,HedgeFlag);
}



//��ȡ��������״̬����̬���������ᴴ������
CPlatFormParam::CONNSTATUS CPlatFormService::GetTradeStatus(void)
{
    return m_PlatformParam.TradeStatus;
};

//��ȡ��������״̬����̬���������ᴴ������
CPlatFormParam::CONNSTATUS CPlatFormService::GetQuotStatus(void)
{
    return m_PlatformParam.QuotStatus;
};

//��ȡ����BrokerID����̬���������ᴴ������
std::string CPlatFormService::GetTradeBrokerID(void)
{
    return m_PlatformParam.BrokerID;

};

//��ȡ����InvestorID����̬���������ᴴ������
std::string CPlatFormService::GetTradeInvestorID(void)
{
	return m_PlatformParam.InvestorID;
};

    //�ж��Ƿ���Ҫ��ѯ�º�Լ�ķ���
bool CPlatFormService::IsNeedCheckCommissionRateAndMarginRate(void)
{
    return m_pDataCenter->IsNeedCheckCommissionRateAndMarginRate();
}


//�����Ҫ��ѯ�º�Լ���ʵı�־
void CPlatFormService::SetNeedCheckCommissionRateAndMarginRate(bool bNeedCheck)
{
    m_pDataCenter->SetNeedCheckCommissionRateAndMarginRate(bNeedCheck);
}
//������Ϻ�Լ�����������
int CPlatFormService::MakeComboDepthMarketData(PlatformStru_DepthMarketData *pDepthMarketData,int index)
{
	return m_pDataCenter->MakeComboDepthMarketData(pDepthMarketData,index);
}
int CPlatFormService::InitCombRelation()
{
	return m_pDataCenter->InitCombRelation();
}
bool CPlatFormService::IsSettlementInfoConfirmed()
{
	return m_bSettlementInfoConfirmed;
}

void CPlatFormService::FillQuotData( const PlatformStru_DepthMarketData& rawQuot, QuotFiled fieldID, std::vector<std::string>& vec )
{

}
SERVER_PARAM_CFG&  CPlatFormService::GetServerParamCfg()
{
	return m_pDataCenter->GetServerParamCfg();
}

///�����˺���Ϣ
int CPlatFormService::GetTrader(std::vector<PlatformStru_Trader> &outData) 
{
	if(m_pTradeApi==NULL)return 0;
	return m_pTradeApi->GetTrader(outData);
}
///�����ѯ��������Ϣ
int CPlatFormService::GetExchange(std::vector<PlatformStru_Exchange> &outData) 
{
	if(m_pTradeApi==NULL)return 0;
	return m_pTradeApi->GetExchange(outData);
}
///�����ѯ��Լ��Ϣ
int CPlatFormService::GetContract(std::vector<PlatformStru_Contract> &outData) 
{
	if(m_pTradeApi==NULL)return 0;
	return m_pTradeApi->GetContract(outData);
}
///������Ʒ��Ϣ
int CPlatFormService::GetCommodity(std::vector<PlatformStru_Commodity> &outData) 
{
	if(m_pTradeApi==NULL)return 0;
	return m_pTradeApi->GetCommodity(outData);
}
///��ȡ����������Ϣ
int CPlatFormService::GetOrderType(std::vector<PlatformStru_OrderType> &outData) 
{
	if(m_pTradeApi==NULL)return 0;
	return m_pTradeApi->GetOrderType(outData);
}
bool CPlatFormService::GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi)
{
	if(m_pTradeApi==NULL)return false;
	return m_pTradeApi->GetTotalPosition(account,posi);
}
bool CPlatFormService::GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi)
{
	if(m_pTradeApi==NULL)return false;
	return m_pTradeApi->GetOpenPosition(InstrumentID,account,posi);
}
bool CPlatFormService::GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi)
{
	if(m_pTradeApi==NULL)return false;
	return m_pTradeApi->GetContractPosition(InstrumentID,account,posi);
}
std::string CPlatFormService::GetLogonReason()
{
	if(m_pTradeApi==NULL)return "";
	return m_pTradeApi->GetLogonReason();
}
void CPlatFormService::GetLinkStatus(int* pHost,int* pPrice)
{
	if(m_pTradeApi)
	{
        m_pTradeApi->GetLinkStatus(pHost,pPrice);
	}
	else
	{
		if(pHost)  *pHost=-1;
		if(pPrice) *pPrice=-1;
	}

}

int CPlatFormService::GetReports( map<DWORD, Report>& mapReport )
{
	if ( m_pTradeApi == NULL )
	{
		return -1;
	}

	return m_pTradeApi->GetReports(mapReport);
}
double CPlatFormService::GetExchangeRate(char* currency)
{
	if ( m_pTradeApi == NULL )
	{
		return 1.0;
	}
	return m_pTradeApi->GetExchangeRate(currency);
}