#include "stdafx.h"

#include <shellapi.h>

#include "TraderSpi.h"
#include "PlatFormService.hpp"
#include "Module-Misc2/BusinessData.hpp"
#include "Module-Misc2/packagecache.h"
#include "Module-Misc2/SimpleWriteLog.h"
#include "Module-Misc/orderCommonFunc.h"
#include "gui-common/VersionDefine.h"
#include "Module-Misc2/GlobalConfigMgr.h"
#include "platmormSvr-InstitudeTrading/ServiceProxy.hpp"
#include "qryqueue.h"


#if 0
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CTraderSpi", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif


static CThostFtdcRspInfoField BlankRspInfo;

CTraderSpi::CTraderSpi(CPlatFormService& RefPlatFormService)
:
m_RefTraderApi(*(RefPlatFormService.m_pTradeApi)),
m_RefPackageCache(*(RefPlatFormService.m_PCache)),
m_PlatformParam(RefPlatFormService.m_PlatformParam),
m_RefPlatFormService(RefPlatFormService)
{
    memset(&BlankRspInfo,0,sizeof(BlankRspInfo));

    m_PlatformParam.TradeStatus = CTPCONNSTATUS_Disconnected;

    m_pWriteLog=new zqWriteLog();
    LOG_INFO("CTraderSpi::CTraderSpi(����CTraderSpi����)");
}

CTraderSpi::~CTraderSpi()
{
    LOG_INFO("CTraderSpi::~CTraderSpi(����CTraderSpi����)");
    delete m_pWriteLog;
}
#ifndef _NEED_AUTHENTICATE
///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CTraderSpi::OnFrontConnected()
{
	DataFrontConnected tmp(true);
	m_RefPackageCache.SaveDataTo(BID_FrontConnected, 0, 0, 0, sizeof(tmp), &tmp);
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strncpy(req.BrokerID, m_PlatformParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
    strncpy(req.UserID, m_PlatformParam.UserID.c_str(),sizeof(req.UserID)-1);
    strncpy(req.Password, m_PlatformParam.UserPW.c_str(),sizeof(req.Password)-1);
    vector<string> vervec;
    if(SplitString(VER_STRING,vervec)==4)
    {
        char tmp[256]={0};
        sprintf(tmp,"ft%02d%02d%02d%02d",
            atoi(vervec[0].c_str()),
            atoi(vervec[1].c_str()),
            atoi(vervec[2].c_str()),
            atoi(vervec[3].c_str()));
	    strncpy(req.UserProductInfo,tmp,sizeof(req.UserProductInfo)-1);
    }
    else
	    strncpy(req.UserProductInfo, "ft00000000",sizeof(req.UserProductInfo)-1);
	strncpy(req.OneTimePassword,m_PlatformParam.OneTimePassword.c_str(),sizeof(req.OneTimePassword)-1);

    m_PlatformParam.TradeStatus=CTPCONNSTATUS_Logining;

    int iRequestID=m_PlatformParam.RequestID++;
    int iRlt = m_RefTraderApi.ReqUserLogin(&req, iRequestID);

    LOG_INFO("OnFrontConnected(�뽻�����ɹ�����TCP����)\n"
                     "\t\t\t --->ReqUserLogin(�û���¼����):ret=[%d],iRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t UserID=[%s],\t UserProductInfo=[%s]",
                     iRlt, iRequestID,
                     req.BrokerID, req.UserID,req.UserProductInfo);
}
#else
///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CTraderSpi::OnFrontConnected()
{
    GetLocalTime(&m_ConnectTime);//�������ʱ��
	DataFrontConnected tmp;
	tmp.Head.BID=BID_FrontConnected;
	tmp.Head.Size=sizeof(DataFrontConnected);
	tmp.bTrader=true;
	m_RefPackageCache.SaveDataTo(BID_FrontConnected, 0, 0, 0, sizeof(tmp), &tmp);
    CThostFtdcReqAuthenticateField req;
    memset(&req, 0, sizeof(req));
    strncpy(req.BrokerID, m_PlatformParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
    strncpy(req.UserID, m_PlatformParam.UserID.c_str(),sizeof(req.UserID)-1);
	strncpy(req.UserProductInfo, "������ϢFastTrader",sizeof(req.UserProductInfo)-1);
	strcpy(req.AuthCode,"");
	m_PlatformParam.TradeStatus=CTPCONNSTATUS_Logining;

    int iRequestID=m_PlatformParam.RequestID++;

    int iRlt = m_RefTraderApi.ReqAuthenticate(&req, iRequestID);

    LOG_INFO("OnFrontConnected(�뽻�����ɹ�����TCP����)\n"
                     "\t\t\t --->ReqAuthenticate(�ͻ�����֤����):ret=[%d],iRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t UserID=[%s],\t UserProductInfo=[%s]",
                     iRlt, iRequestID,
                     req.BrokerID, req.UserID,req.UserProductInfo);
}
//�ͻ�����֤��Ӧ
void CTraderSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(0==pRspInfo->ErrorID)//����ɹ�
	{
		CThostFtdcReqUserLoginField req;
		memset(&req, 0, sizeof(req));
		strncpy(req.BrokerID, m_PlatformParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
		strncpy(req.UserID, m_PlatformParam.UserID.c_str(),sizeof(req.UserID)-1);
		strncpy(req.Password, m_PlatformParam.UserPW.c_str(),sizeof(req.Password)-1);
	    strncpy(req.UserProductInfo, "������ϢFastTrader",sizeof(req.UserProductInfo)-1);
		strncpy(req.OneTimePassword,m_PlatformParam.OneTimePassword.c_str(),sizeof(req.OneTimePassword)-1);

		int iRequestID=m_PlatformParam.RequestID++;

		int iRlt = m_RefTraderApi.ReqUserLogin(&req, iRequestID);

		LOG_INFO("OnRspAuthenticate(�ͻ�����֤��Ӧ)\n"
			"\t\t\t --->ReqUserLogin(�û���¼����):ret=[%d],iRequestID=[%d],\n"
			"\t\t\t BrokerID=[%s],\t UserID=[%s],\t UserProductInfo=[%s]",
			iRlt, iRequestID,
			req.BrokerID, req.UserID,req.UserProductInfo);
	}
	else
	{
        m_PlatformParam.TradeStatus=CTPCONNSTATUS_LoginFailure;
		if(pRspInfo) memcpy(&m_RefPlatFormService.m_ErrorInfoField,pRspInfo,sizeof(CThostFtdcRspInfoField));
	}
}
#endif
///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á�
///��������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void CTraderSpi::OnFrontDisconnected(int nReason)
{
    LOG_INFO("OnFrontDisconnected(�뽻����ʧȥTCP����), Reason=[%#x]", nReason);
    DataFrontDisconnected tmp(true,nReason);
    m_RefPackageCache.SaveDataTo(BID_FrontDisconnected, 0, 0, 0, sizeof(tmp), &tmp);
    m_PlatformParam.TradeStatus=CTPCONNSTATUS_Disconnected;
}
    
///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
///�ۺϽ���ƽ̨��API�Ѿ����δ˽ӿ�
void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
    LOG_INFO("OnHeartBeatWarning:nTimeLapse=[%d]", nTimeLapse);
}


///��¼������Ӧ
void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(pRspUserLogin)
    {
		m_RefPlatFormService.SetOrderRef(atoi(pRspUserLogin->MaxOrderRef));
        DataRspUserLogin tmp(true,nRequestID,bIsLast,GetTickCount(),*pRspUserLogin,pRspInfo?(*pRspInfo):BlankRspInfo);
        m_RefPackageCache.SaveDataTo(BID_RspUserLogin, 0, 0, 0, sizeof(tmp), &tmp);
        
        LOG_INFO("OnRspUserLogin(��¼������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]\n"
                        "\t\t\t TradingDay=[%s],\t LoginTime=[%s],\t BrokerID=[%s],\t UserID=[%s],\t SystemName=[%s]\n"
                        "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t MaxOrderRef[%s],\t SHFETime=[%s],\t DCETime=[%s],\n"
                        "\t\t\t CZCETime[%s],\t FFEXTime=[%s],\t CurTickCount=[%ul]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
                        pRspUserLogin->TradingDay,pRspUserLogin->LoginTime,pRspUserLogin->BrokerID   , pRspUserLogin->UserID,pRspUserLogin->SystemName, 
                        pRspUserLogin->FrontID  , pRspUserLogin->SessionID ,pRspUserLogin->MaxOrderRef, pRspUserLogin->SHFETime , pRspUserLogin->DCETime, 
                        pRspUserLogin->CZCETime   , pRspUserLogin->FFEXTime,tmp.CurTickCount);
    }
    else
    {
        LOG_INFO("OnRspUserLogin(��¼������Ӧ),pRspUserLogin is NULL");
    }
    
    if(0==pRspInfo->ErrorID)//����ɹ�
    {
        PlatformStru_QrySettlementInfoConfirm req;
        memset(&req, 0, sizeof(req));
        strcpy(req.Thost.BrokerID, m_PlatformParam.BrokerID.c_str());
        strcpy(req.Thost.InvestorID, m_PlatformParam.InvestorID.c_str());

        int iRequestID=m_PlatformParam.RequestID++;
        int iRlt = m_RefTraderApi.ReqQrySettlementInfoConfirm(&req, iRequestID);

        LOG_INFO("OnRspUserLogin(��¼������Ӧ)\n"
                        "\t\t\t --->ReqQrySettlementInfoConfirm(�����ѯͶ���߽�����ȷ��):ret=[%d],iRequestID=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
                        iRlt, iRequestID, req.Thost.BrokerID, req.Thost.InvestorID); 
        m_PlatformParam.TradeStatus=CTPCONNSTATUS_TradeSettlementInfoConfirming;
        //m_PlatformParam.TradeStatus=CTPCONNSTATUS_Connected;
    }
    else
    {
        m_PlatformParam.TradeStatus=CTPCONNSTATUS_LoginFailure;
		if(pRspInfo) memcpy(&m_RefPlatFormService.m_ErrorInfoField,pRspInfo,sizeof(CThostFtdcRspInfoField));
    }

}


///Ͷ���߽�����ȷ����Ӧ
void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LOG_INFO("OnRspSettlementInfoConfirm(Ͷ���߽�����ȷ����Ӧ),ErrorID=[%d],ErrorMsg=[%s]", pRspInfo->ErrorID, pRspInfo->ErrorMsg);

    //ȷ�Ͻ�������ͨ��QryQueue�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
    if(pRspInfo->ErrorID==0)
    {
	    PlatformStru_QryCFMMCTradingAccountKey temp;
	    memset(&temp,0,sizeof(temp) );

	    m_RefPlatFormService.m_rServiceProxy.ReqQryCFMMCTradingAccountKey( temp ) ;
    }

}


///��ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ��Ӧ
void CTraderSpi::OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    if(pRspInfo)
        LOG_INFO("OnRspQryCFMMCTradingAccountKey(��ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ��Ӧ) ErrorID=[%d],ErrorMsg=[%s], nRequestID=%d bIsLast=%d", pRspInfo->ErrorID, pRspInfo->ErrorMsg,nRequestID,bIsLast);
    else
        LOG_INFO("OnRspQryCFMMCTradingAccountKey(��ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ��Ӧ) pRspInfo==NULL, nRequestID=%d bIsLast=%d", nRequestID,bIsLast);

    if(pCFMMCTradingAccountKey&&(pRspInfo==NULL||pRspInfo&&pRspInfo->ErrorID==0)&&bIsLast)
    {
        char Urlbuf[1024];
        pCFMMCTradingAccountKey->ParticipantID[sizeof(pCFMMCTradingAccountKey->ParticipantID)-1]=0;
        pCFMMCTradingAccountKey->AccountID[sizeof(pCFMMCTradingAccountKey->AccountID)-1]=0;
        pCFMMCTradingAccountKey->CurrentKey[sizeof(pCFMMCTradingAccountKey->CurrentKey)-1]=0;
        sprintf(Urlbuf,"https://investorservice.cfmmc.com/loginByKey.do?companyID=%s&userid=%s&keyid=%d&passwd=%s",
            pCFMMCTradingAccountKey->ParticipantID,
            pCFMMCTradingAccountKey->AccountID,
            pCFMMCTradingAccountKey->KeyID,
            pCFMMCTradingAccountKey->CurrentKey);

//	    ShellExecute(NULL, _T("open"), _T("explorer.exe"), Urlbuf, NULL, SW_SHOW);
	    ShellExecute(NULL, ("open"), Urlbuf, "",NULL, SW_SHOW);

        LOG_INFO("�򿪱�֤����ϵͳ�ʽ��˻�ҳ�棺Urlbuf=[%s]", Urlbuf);
    }
}




///�ǳ�������Ӧ
void CTraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pUserLogout)
    {
        LOG_INFO("OnRspUserLogout(�ǳ�������Ӧ),pUserLogout is NULL");
        return;
    }

    DataRspUserLogout tmp(0,nRequestID,bIsLast,*pUserLogout,pRspInfo?(*pRspInfo):BlankRspInfo);
    m_RefPackageCache.SaveDataTo(BID_RspUserLogout, 0, 0, 0, sizeof(tmp), &tmp);

    LOG_INFO("OnRspUserLogout(�ǳ�������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                    "\t\t\t BrokerID=[%s],\t UserID=[%s],",
                    tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                    pUserLogout->BrokerID, pUserLogout->UserID);
}

///�û��������������Ӧ
void CTraderSpi::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pUserPasswordUpdate)
    {
        LOG_INFO("OnRspUserPasswordUpdate(�û��������������Ӧ),pUserPasswordUpdate is NULL");
        return;
    }

    DataRspUserPasswordUpdate tmp(nRequestID,bIsLast,*pUserPasswordUpdate,pRspInfo?(*pRspInfo):BlankRspInfo);

	if(tmp.RspInfoField.ErrorID==0)
	{
		m_PlatformParam.InvestorPW=pUserPasswordUpdate->NewPassword;
	}

    m_RefPackageCache.SaveDataTo(BID_RspUserPasswordUpdate, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRspUserPasswordUpdate(�û��������������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                    "\t\t\t BrokerID=[%s],\t UserID=[%s],\t OldPassword=[******],\t NewPassword=[******]",
                    tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                    pUserPasswordUpdate->BrokerID, pUserPasswordUpdate->UserID);
}

///�ʽ��˻��������������Ӧ
void CTraderSpi::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pTradingAccountPasswordUpdate)
    {
        LOG_INFO("OnRspTradingAccountPasswordUpdate(�ʽ��˻��������������Ӧ),pTradingAccountPasswordUpdate is NULL");
        return;
    }

    DataRspTradingAccountPasswordUpdate tmp(nRequestID,bIsLast,*pTradingAccountPasswordUpdate,pRspInfo?(*pRspInfo):BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspTradingAccountPasswordUpdate, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRspTradingAccountPasswordUpdate(�ʽ��˻��������������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],BrokerID=[%s] AccountID=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, 
			pTradingAccountPasswordUpdate->BrokerID, pTradingAccountPasswordUpdate->AccountID, nRequestID,bIsLast);
}

///����¼��������Ӧ
///���ݡ��ۺϽ���ƽ̨API����FAQ��: 
///     CTP�ڼ��ί�з��ִ���ʱ���������ί�е�Ͷ���߷���OnRspInsertOrder��ͬʱ����OnErrRtnOrder����صĽ���Ա�����ԣ���ΪͶ���߿��Բ�����OnErrRtnOrder
void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pInputOrder)
    {
        LOG_INFO("OnRspOrderInsert(����¼��������Ӧ),pInputOrder is NULL");
        return;
    }

    DataRspOrderInsert tmp(nRequestID,bIsLast,*pInputOrder,pRspInfo?(*pRspInfo):BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspOrderInsert, 0, 0, 0, sizeof(tmp), &tmp); 
    LOG_INFO("OnRspOrderInsert(����¼��������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
             "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
             "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
             "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
             "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
             "\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
             tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
             pInputOrder->BrokerID, pInputOrder->InvestorID, pInputOrder->InstrumentID, pInputOrder->OrderRef,pInputOrder->UserID,
             pInputOrder->OrderPriceType, pInputOrder->Direction, pInputOrder->CombOffsetFlag, pInputOrder->CombHedgeFlag,pInputOrder->LimitPrice,
             pInputOrder->VolumeTotalOriginal, pInputOrder->TimeCondition, pInputOrder->GTDDate, pInputOrder->VolumeCondition,pInputOrder->MinVolume,
             pInputOrder->ContingentCondition, pInputOrder->StopPrice, pInputOrder->ForceCloseReason, pInputOrder->IsAutoSuspend,pInputOrder->BusinessUnit,
             pInputOrder->RequestID, pInputOrder->UserForceClose);
}

///��������������Ӧ
void CTraderSpi::OnRspOrderAction(PlatformStru_InputOrderAction *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pInputOrderAction)
    {
        LOG_INFO("OnRspOrderAction(��������������Ӧ),pInputOrderAction is NULL");
        return;
    }

	CThostFtdcInputOrderActionField newField;
	memcpy(&newField, pInputOrderAction, sizeof(newField));
    DataRspOrderAction tmp(nRequestID,bIsLast,newField,pRspInfo?(*pRspInfo):BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspOrderAction, 0, 0, 0, sizeof(tmp), &tmp);  
    LOG_INFO("OnRspOrderAction(��������������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
                     "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
                     "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
                     tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                     pInputOrderAction->Thost.BrokerID, pInputOrderAction->Thost.InvestorID, pInputOrderAction->Thost.OrderActionRef, pInputOrderAction->Thost.OrderRef,pInputOrderAction->Thost.RequestID,
                     pInputOrderAction->Thost.FrontID, pInputOrderAction->Thost.SessionID, pInputOrderAction->Thost.ExchangeID, pInputOrderAction->Thost.OrderSysID,pInputOrderAction->Thost.ActionFlag,
                     pInputOrderAction->Thost.LimitPrice, pInputOrderAction->Thost.VolumeChange, pInputOrderAction->Thost.UserID, pInputOrderAction->Thost.InstrumentID);
}

///��ѯ��󱨵�������Ӧ
void CTraderSpi::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    LOG_INFO("OnRspQueryMaxOrderVolume(��ѯ��󱨵�������Ӧ),Unimplemented");
}



///Ԥ��¼��������Ӧ
void CTraderSpi::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pParkedOrder)
    {
        LOG_INFO("OnRspParkedOrderInsert(Ԥ��¼��������Ӧ),pParkedOrder is NULL");
        return;
    }

    DataRspParkedOrderInsert tmp(nRequestID,bIsLast,*pParkedOrder,pRspInfo?(*pRspInfo):BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspParkedOrderInsert, 0, 0, 0, sizeof(tmp), &tmp); 
    LOG_INFO("OnRspParkedOrderInsert(Ԥ��¼��������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
             "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
             "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
             "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
             "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
             "\t\t\t RequestID=[%d],\t UserForceClose=[%d],\t ExchangeID=[%s],\t ParkedOrderID=[%s],\t UserType=[%c],\n"
			 "\t\t\t Status=[%c],\t ErrorID=[%d],\t ErrorMsg=[%s]",
             tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
             pParkedOrder->BrokerID, pParkedOrder->InvestorID, pParkedOrder->InstrumentID, pParkedOrder->OrderRef,pParkedOrder->UserID,
             pParkedOrder->OrderPriceType, pParkedOrder->Direction, pParkedOrder->CombOffsetFlag, pParkedOrder->CombHedgeFlag,pParkedOrder->LimitPrice,
             pParkedOrder->VolumeTotalOriginal, pParkedOrder->TimeCondition, pParkedOrder->GTDDate, pParkedOrder->VolumeCondition,pParkedOrder->MinVolume,
             pParkedOrder->ContingentCondition, pParkedOrder->StopPrice, pParkedOrder->ForceCloseReason, pParkedOrder->IsAutoSuspend,pParkedOrder->BusinessUnit,
             pParkedOrder->RequestID, pParkedOrder->UserForceClose, pParkedOrder->ExchangeID, pParkedOrder->ParkedOrderID, pParkedOrder->UserType, 
			 pParkedOrder->Status, pParkedOrder->ErrorID, pParkedOrder->ErrorMsg);
}

///Ԥ�񳷵�¼��������Ӧ
void CTraderSpi::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pParkedOrderAction)
    {
        LOG_INFO("OnRspParkedOrderAction(Ԥ�񳷵�¼��������Ӧ),pParkedOrderAction is NULL");
        return;
    }

    DataRspParkedOrderAction tmp(nRequestID,bIsLast,*pParkedOrderAction,pRspInfo?(*pRspInfo):BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspParkedOrderAction, 0, 0, 0, sizeof(tmp), &tmp); 
    LOG_INFO("OnRspParkedOrderAction(Ԥ�񳷵�¼��������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
                     "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
                     "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s],\t ParkedOrderActionID=[%d],\n"
					 "\t\t\t UserType=[%c],\t Status=[%c],\t ErrorID=[%d],\t ErrorMsg=[%s]", 
                     tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                     pParkedOrderAction->BrokerID, pParkedOrderAction->InvestorID, pParkedOrderAction->OrderActionRef, pParkedOrderAction->OrderRef,pParkedOrderAction->RequestID,
                     pParkedOrderAction->FrontID, pParkedOrderAction->SessionID, pParkedOrderAction->ExchangeID, pParkedOrderAction->OrderSysID,pParkedOrderAction->ActionFlag,
                     pParkedOrderAction->LimitPrice, pParkedOrderAction->VolumeChange, pParkedOrderAction->UserID, pParkedOrderAction->InstrumentID, pParkedOrderAction->ParkedOrderActionID, 
					 pParkedOrderAction->UserType, pParkedOrderAction->Status, pParkedOrderAction->ErrorID, pParkedOrderAction->ErrorMsg);
}

///ɾ��Ԥ����Ӧ
void CTraderSpi::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pRemoveParkedOrder)
    {
        LOG_INFO("OnRspRemoveParkedOrder(ɾ��Ԥ����Ӧ),pRemoveParkedOrder is NULL");
        return;
    }

    DataRspRemoveParkedOrder tmp(nRequestID,bIsLast,*pRemoveParkedOrder,pRspInfo?(*pRspInfo):BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspRemoveParkedOrder, 0, 0, 0, sizeof(tmp), &tmp); 
    LOG_INFO("OnRspRemoveParkedOrder(ɾ��Ԥ����Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ParkedOrderID=[%s]",
                     tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                     pRemoveParkedOrder->BrokerID, pRemoveParkedOrder->InvestorID, pRemoveParkedOrder->ParkedOrderID);
}

///ɾ��Ԥ�񳷵���Ӧ
void CTraderSpi::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pRemoveParkedOrderAction)
    {
        LOG_INFO("OnRspRemoveParkedOrderAction(ɾ��Ԥ�񳷵���Ӧ),pRemoveParkedOrderAction is NULL");
        return;
    }

    DataRspRemoveParkedOrderAction tmp(nRequestID,bIsLast,*pRemoveParkedOrderAction,pRspInfo?(*pRspInfo):BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspRemoveParkedOrderAction, 0, 0, 0, sizeof(tmp), &tmp); 
    LOG_INFO("OnRspRemoveParkedOrderAction(ɾ��Ԥ�񳷵���Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ParkedOrderActionID=[%s]",
                     tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                     pRemoveParkedOrderAction->BrokerID, pRemoveParkedOrderAction->InvestorID, pRemoveParkedOrderAction->ParkedOrderActionID);
}

///�����ѯԤ����Ӧ
void CTraderSpi::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    CThostFtdcParkedOrderField ParkedOrder;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    if(!pParkedOrder)//��ѯ���Ϊ��ʱ��pParkedOrder��ֵ��ΪNULL
    {
        memset(&ParkedOrder,0,sizeof(ParkedOrder));
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        ParkedOrder = *pParkedOrder;
    }

    DataRspQryParkedOrder tmp(nRequestID,bIsLast,ParkedOrder,RspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspQryParkedOrder, 0, 0, 0, sizeof(tmp), &tmp); 

    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryParkedOrder(�����ѯԤ����Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        LOG_INFO("OnRspQryParkedOrder(�����ѯԤ����Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                        "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
                        "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
                        "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
                        "\t\t\t RequestID=[%d],\t UserForceClose=[%d],\t ExchangeID=[%s],\t ParkedOrderID=[%s],\t UserType=[%c], \n"
						"\t\t\t Status=[%c],\t ErrorID=[%d],\t ErrorMsg=[%s]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
                        pParkedOrder->BrokerID,pParkedOrder->InvestorID,pParkedOrder->InstrumentID,pParkedOrder->OrderRef,pParkedOrder->UserID,
                        pParkedOrder->OrderPriceType,pParkedOrder->Direction,pParkedOrder->CombOffsetFlag,pParkedOrder->CombHedgeFlag,pParkedOrder->LimitPrice,
                        pParkedOrder->VolumeTotalOriginal,pParkedOrder->TimeCondition,pParkedOrder->GTDDate,pParkedOrder->VolumeCondition,pParkedOrder->MinVolume,
                        pParkedOrder->ContingentCondition,pParkedOrder->StopPrice,pParkedOrder->ForceCloseReason,pParkedOrder->IsAutoSuspend,pParkedOrder->BusinessUnit,
                        pParkedOrder->RequestID, pParkedOrder->UserForceClose,pParkedOrder->ExchangeID, pParkedOrder->ParkedOrderID, pParkedOrder->UserType, 
						pParkedOrder->Status, pParkedOrder->ErrorID, pParkedOrder->ErrorMsg
						);
    }
}

///�����ѯԤ�񳷵���Ӧ
void CTraderSpi::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    CThostFtdcParkedOrderActionField ParkedOrderAction;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    if(!pParkedOrderAction)//��ѯ���Ϊ��ʱ��pParkedOrderAction��ֵ��ΪNULL
    {
        memset(&ParkedOrderAction,0,sizeof(ParkedOrderAction));
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        ParkedOrderAction = *pParkedOrderAction;
    }

    DataRspQryParkedOrderAction tmp(nRequestID,bIsLast,ParkedOrderAction,RspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspQryParkedOrderAction, 0, 0, 0, sizeof(tmp), &tmp); 

    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryParkedOrderAction(�����ѯԤ�񳷵���Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        LOG_INFO("OnRspQryParkedOrderAction(�����ѯԤ�񳷵���Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%s],\t OrderRef=[%s],\t RequestID=[%d],\n"
			"\t\t\t FrontID=[%d],\t SessionID=[%d],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
            "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s],\t ParkedOrderActionID=[%s],\n"
			"\t\t\t UserType=[%c],\t Status=[%c],\t ErrorID=[%d],\t ErrorMsg=[%s]",
            tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
            pParkedOrderAction->BrokerID, pParkedOrderAction->InvestorID, pParkedOrderAction->OrderActionRef, pParkedOrderAction->OrderRef, pParkedOrderAction->RequestID, 
			pParkedOrderAction->FrontID, pParkedOrderAction->SessionID, pParkedOrderAction->ExchangeID, pParkedOrderAction->OrderSysID, pParkedOrderAction->ActionFlag, 
            pParkedOrderAction->LimitPrice, pParkedOrderAction->VolumeChange, pParkedOrderAction->UserID, pParkedOrderAction->InstrumentID, pParkedOrderAction->ParkedOrderActionID,
			pParkedOrderAction->UserType, pParkedOrderAction->Status, pParkedOrderAction->ErrorID, pParkedOrderAction->ErrorMsg
			);
    }
}

///�����ѯ������Ӧ
void CTraderSpi::OnRspQryOrder(PlatformStru_OrderInfo *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    PlatformStru_OrderInfo OrderField;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    if(!pOrder)
    {
        OrderField.Clear();
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
		//CopyOrderField(OrderField,*pOrder);
		OrderField = *pOrder;
    }

    DataRspQryOrder tmp(nRequestID,bIsLast,OrderField,RspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspQryOrder, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(!pOrder)
    {
        LOG_INFO("OnRspQryOrder(�����ѯ������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );
    }
    else
    {
        PlatformStru_OrderInfo* pOrder = &tmp.OrderField;
        LOG_INFO("OnRspQryOrder(�����ѯ������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                        "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
                        "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
                        "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
                        "\t\t\t RequestID=[%d],\t OrderLocalID=[%s],\t ExchangeID=[%s],\t ParticipantID=[%s],\t ClientID=[%s],\n"
                        "\t\t\t ExchangeInstID=[%s],\t TraderID=[%s],\t InstallID=[%d],\t OrderSubmitStatus=[%d],\t NotifySequence=[%d],\n"
                        "\t\t\t TradingDay=[%s],\t SettlementID=[%d],\t OrderSysID=[%s],\t OrderSource=[%d],\t OrderStatus=[%d],\n"
                        "\t\t\t OrderType=[%d],\t VolumeTraded=[%d],\t VolumeTotal=[%d],\t InsertDate=[%s],\t InsertTime=[%s],\n"
                        "\t\t\t ActiveTime=[%s],\t SuspendTime=[%s],\t UpdateTime=[%s],\t CancelTime=[%s],\t ActiveTraderID=[%s]\n"
                        "\t\t\t ClearingPartID=[%s],\t SequenceNo=[%d],\t FrontID=[%d],\t SessionID=[%#x],\t UserProductInfo=[%s],\n"
                        "\t\t\t StatusMsg=[%s],\t UserForceClose=[%d],\t ActiveUserID=[%s],\t BrokerOrderSeq=[%d],RelativeOrderSysID=[%s]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
                        pOrder->BrokerID,pOrder->InvestorID,pOrder->InstrumentID,pOrder->OrderRef,pOrder->UserID,
                        pOrder->OrderPriceType,pOrder->Direction,pOrder->CombOffsetFlag,pOrder->CombHedgeFlag,pOrder->LimitPrice,
                        pOrder->VolumeTotalOriginal,pOrder->TimeCondition,pOrder->GTDDate,pOrder->VolumeCondition,pOrder->MinVolume,
                        pOrder->ContingentCondition,pOrder->StopPrice,pOrder->ForceCloseReason,pOrder->IsAutoSuspend,pOrder->BusinessUnit,
						pOrder->RequestID,pOrder->OrderLocalID,pOrder->ExchangeID,pOrder->ParticipantID,pOrder->ClientID,
                        pOrder->ExchangeInstID,pOrder->TraderID,pOrder->InstallID,pOrder->OrderSubmitStatus,pOrder->NotifySequence,
                        pOrder->TradingDay,pOrder->SettlementID,pOrder->OrderSysID,pOrder->OrderSource,pOrder->OrderStatus,
                        pOrder->OrderType,pOrder->VolumeTraded,pOrder->VolumeTotal,pOrder->InsertDate,pOrder->InsertTime,
                        pOrder->ActiveTime,pOrder->SuspendTime,pOrder->UpdateTime,pOrder->CancelTime,pOrder->ActiveTraderID,
                        pOrder->ClearingPartID,pOrder->SequenceNo,pOrder->FrontID,pOrder->SessionID,pOrder->UserProductInfo,
                        pOrder->StatusMsg,pOrder->UserForceClose,pOrder->ActiveUserID,pOrder->BrokerOrderSeq,pOrder->RelativeOrderSysID
                    );
    }
}

///�����ѯ�ɽ���Ӧ
void CTraderSpi::OnRspQryTrade(PlatformStru_TradeInfo *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    PlatformStru_TradeInfo TradeField;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    if(!pTrade)
    {
        TradeField.Clear();
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {    
        //CopyTradeRecordField( TradeField, *pTrade);
        TradeField = *pTrade;
    }

    DataRspQryTrade tmp(nRequestID,bIsLast,TradeField,RspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspQryTrade, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryTrade(�����ѯ�ɽ���Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else if(pTrade!=NULL)
    {
        LOG_INFO("OnRspQryTrade(�����ѯ�ɽ���Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                        "\t\t\t ExchangeID=[%s],\t TradeID=[%s],\t Direction=[%d],\t OrderSysID=[%s],\t ParticipantID=[%s],\n"
                        "\t\t\t ClientID=[%s],\t TradingRole=[%d],\t ExchangeInstID=[%s],\t OffsetFlag=[%d],\t HedgeFlag=[%d],\n"
                        "\t\t\t Price=[%g],\t Volume=[%d],\t TradeDate=[%s],\t TradeTime=[%s],\t TradeType=[%d],\n"
                        "\t\t\t PriceSource=[%d],\t TraderID=[%s],\t OrderLocalID=[%s],\t ClearingPartID=[%s],\t BusinessUnit=[%s],\n"
                        "\t\t\t SequenceNo=[%d],\t TradingDay=[%s],\t SettlementID=[%d],\t BrokerOrderSeq=[%d],\t TradeSource=[%c]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
                        pTrade->BrokerID,pTrade->InvestorID,pTrade->InstrumentID,pTrade->OrderRef,pTrade->UserID,
                        pTrade->ExchangeID,pTrade->TradeID,pTrade->Direction,pTrade->OrderSysID,pTrade->ParticipantID,
                        pTrade->ClientID,pTrade->TradingRole,pTrade->ExchangeInstID,pTrade->OffsetFlag,pTrade->HedgeFlag,
                        pTrade->Price,pTrade->Volume,pTrade->TradeDate,pTrade->TradeTime,pTrade->TradeType,
                        pTrade->PriceSource,pTrade->TraderID,pTrade->OrderLocalID,pTrade->ClearingPartID,pTrade->BusinessUnit,
                        pTrade->SequenceNo,pTrade->TradingDay,pTrade->SettlementID,pTrade->BrokerOrderSeq, pTrade->TradeSource
                        );
    }
}

///�����ѯ�ʽ��˻���Ӧ
void CTraderSpi::OnRspQryTradingAccount(PlatformStru_TradingAccountInfo *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    if(!pTradingAccount)
    {
        LOG_INFO("OnRspQryTradingAccount(�����ѯ�ʽ��˻���Ӧ),pTradingAccount is NULL");
        return;
    }

    PlatformStru_TradingAccountInfo TradingAccountField;
    TradingAccountField.clear();
    strcpy(TradingAccountField.AccountID, pTradingAccount->AccountID);
    strcpy(TradingAccountField.BrokerID, pTradingAccount->BrokerID);
    TradingAccountField.PreMortgage=pTradingAccount->PreMortgage;
    TradingAccountField.PreCredit=pTradingAccount->PreCredit;
    TradingAccountField.PreDeposit=pTradingAccount->PreDeposit;
    TradingAccountField.PreBalance=pTradingAccount->PreBalance;
    TradingAccountField.PreMargin=pTradingAccount->PreMargin;
    TradingAccountField.InterestBase=pTradingAccount->InterestBase;
    TradingAccountField.Interest=pTradingAccount->Interest;
    TradingAccountField.Deposit=pTradingAccount->Deposit;

    TradingAccountField.Withdraw=pTradingAccount->Withdraw;
    TradingAccountField.FrozenMargin=pTradingAccount->FrozenMargin;
    TradingAccountField.FrozenCommission=pTradingAccount->FrozenCommission;
    TradingAccountField.CurrMargin=pTradingAccount->CurrMargin;
    TradingAccountField.CashIn=pTradingAccount->CashIn;
    TradingAccountField.Commission=pTradingAccount->Commission;
    TradingAccountField.CloseProfit=pTradingAccount->CloseProfit;

    TradingAccountField.PositionProfit=pTradingAccount->PositionProfit;
    TradingAccountField.Balance=pTradingAccount->Balance;
    TradingAccountField.Available=pTradingAccount->Available;
    TradingAccountField.WithdrawQuota=pTradingAccount->WithdrawQuota;
    TradingAccountField.Reserve=pTradingAccount->Reserve;
    strcpy(TradingAccountField.TradingDay, pTradingAccount->TradingDay);
    TradingAccountField.SettlementID=pTradingAccount->SettlementID;
    TradingAccountField.Credit=pTradingAccount->Credit;
    TradingAccountField.Mortgage=pTradingAccount->Mortgage;
    TradingAccountField.ExchangeMargin=pTradingAccount->ExchangeMargin;
    TradingAccountField.DeliveryMargin=pTradingAccount->DeliveryMargin;
    TradingAccountField.ExchangeDeliveryMargin=pTradingAccount->ExchangeDeliveryMargin;

    DataRspQryTradingAccount tmp(nRequestID,bIsLast,TradingAccountField,pRspInfo?(*pRspInfo):BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspQryTradingAccount, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRspQryTradingAccount(�����ѯ�ʽ��˻���Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                    "\t\t\t BrokerID=[%s],\t AccountID=[%s],\t PreMortgage=[%g],\t PreCredit=[%g],\t PreDeposit=[%g],\n"
                    "\t\t\t PreBalance=[%g],\t PreMargin=[%g],\t InterestBase=[%g],\t Interest=[%g],\t Deposit=[%g],\n "
                    "\t\t\t Withdraw=[%g],\t FrozenMargin=[%g],\t FrozenCash=[%g],\t FrozenCommission=[%g],\t CurrMargin=[%g],\n "
                    "\t\t\t CashIn=[%g],\t Commission=[%g],\t CloseProfit=[%g],\t PositionProfit=[%g],\t Balance=[%g],\n "
                    "\t\t\t Available=[%g],\t WithdrawQuota=[%g],\t Reserve=[%g],\t TradingDay=[%s],\t SettlementID=[%d],\n "
                    "\t\t\t Credit=[%g],\t Mortgage=[%g],\t ExchangeMargin=[%g],\t DeliveryMargin=[%g],\t ExchangeDeliveryMargin=[%g]",
                    tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                    pTradingAccount->BrokerID,pTradingAccount->AccountID,pTradingAccount->PreMortgage,pTradingAccount->PreCredit,pTradingAccount->PreDeposit, 
                    pTradingAccount->PreBalance,pTradingAccount->PreMargin,pTradingAccount->InterestBase,pTradingAccount->Interest,pTradingAccount->Deposit, 
                    pTradingAccount->Withdraw,pTradingAccount->FrozenMargin,pTradingAccount->FrozenCash,pTradingAccount->FrozenCommission,pTradingAccount->CurrMargin, 
                    pTradingAccount->CashIn,pTradingAccount->Commission,pTradingAccount->CloseProfit,pTradingAccount->PositionProfit,pTradingAccount->Balance, 
                    pTradingAccount->Available,pTradingAccount->WithdrawQuota,pTradingAccount->Reserve,pTradingAccount->TradingDay,pTradingAccount->SettlementID, 
                    pTradingAccount->Credit,pTradingAccount->Mortgage,pTradingAccount->ExchangeMargin,pTradingAccount->DeliveryMargin,pTradingAccount->ExchangeDeliveryMargin
                    );
}

///�����ѯͶ������Ӧ
void CTraderSpi::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    CThostFtdcInvestorField Investor;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    if(!pInvestor)
    {
        memset(&Investor,0,sizeof(Investor));
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        Investor = *pInvestor;
    }

    DataRspQryInvestor tmp(nRequestID,bIsLast,Investor,RspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspQryInvestor, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryInvestor(�����ѯͶ������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        pInvestor= &tmp.InvestorField;
        LOG_INFO("OnRspQryInvestor(�����ѯͶ������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InvestorID=[%s],\t BrokerID=[%s],\t InvestorGroupID=[%s],\t InvestorName=[%s],\t IdentifiedCardType=[%d],\n"
                        "\t\t\t IdentifiedCardNo=[%s],\t IsActive=[%d],\t Telephone=[%s],\t Address=[%s]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                        pInvestor->InvestorID, pInvestor->BrokerID,pInvestor->InvestorGroupID,pInvestor->InvestorName,pInvestor->IdentifiedCardType,
                        pInvestor->IdentifiedCardNo, pInvestor->IsActive,pInvestor->Telephone,pInvestor->Address
                        );
    }
}

///�����ѯ���ױ�����Ӧ
void CTraderSpi::OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    if(!pTradingCode)
    {
        LOG_INFO("OnRspQryTradingCode(�����ѯ���ױ�����Ӧ),pTradingCode is NULL");
        return;
    }

    DataRspQryTradingCode tmp(nRequestID,bIsLast,*pTradingCode,pRspInfo?(*pRspInfo):BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspQryTradingCode, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRspQryTradingCode(�����ѯ���ױ�����Ӧ) : ErrorID=[%d],ErrorMsg=[%s],InvestorID=[%s],BrokerID=[%s],ClientID=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, 
			pTradingCode->InvestorID, pTradingCode->BrokerID,pTradingCode->ClientID,nRequestID,bIsLast);
}

///�����ѯ��Լ��֤������Ӧ
void CTraderSpi::OnRspQryInstrumentMarginRate(PlatformStru_InstrumentMarginRate *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    PlatformStru_InstrumentMarginRate InstrumentMarginRateField;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    if(!pInstrumentMarginRate)
    {
        InstrumentMarginRateField.clear();
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        InstrumentMarginRateField = *((PlatformStru_InstrumentMarginRate*)pInstrumentMarginRate);
    }

    DataRspQryInstrumentMarginRate tmp(nRequestID,bIsLast,InstrumentMarginRateField,RspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspQryInstrumentMarginRate, 0, 0, 0, sizeof(tmp), &tmp);

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryInstrumentMarginRate(�����ѯ��Լ��֤������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        //pInstrumentMarginRate=(CThostFtdcInstrumentMarginRateField*)&tmp.InstrumentMarginRateField;
        LOG_INFO("OnRspQryInstrumentMarginRate(�����ѯ��Լ��֤������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InstrumentID=[%s],\t InvestorRange=[%d],\t BrokerID=[%s],\t InvestorID=[%s],\t HedgeFlag=[%d],\n"
                        "\t\t\t LongMarginRatioByMoney=[%g],\t LongMarginRatioByVolume=[%g],\t ShortMarginRatioByMoney=[%g],\t ShortMarginRatioByVolume=[%g],\t IsRelative=[%d]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                        pInstrumentMarginRate->InstrumentID,
                        pInstrumentMarginRate->InvestorRange,
                        pInstrumentMarginRate->BrokerID,
                        pInstrumentMarginRate->InvestorID,
                        pInstrumentMarginRate->HedgeFlag,
                        pInstrumentMarginRate->LongMarginRatioByMoney,
                        pInstrumentMarginRate->LongMarginRatioByVolume,
                        pInstrumentMarginRate->ShortMarginRatioByMoney,
                        pInstrumentMarginRate->ShortMarginRatioByVolume,
                        pInstrumentMarginRate->IsRelative
                        );
        }
}

///�����ѯ��Լ����������Ӧ
void CTraderSpi::OnRspQryInstrumentCommissionRate(PlatformStru_InstrumentCommissionRate *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    PlatformStru_InstrumentCommissionRate InstrumentCommissionRateField;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    if(!pInstrumentCommissionRate)
    {
        InstrumentCommissionRateField.clear();
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        InstrumentCommissionRateField = *((PlatformStru_InstrumentCommissionRate*)pInstrumentCommissionRate);
    }

    DataRspQryInstrumentCommissionRate tmp(nRequestID,bIsLast,InstrumentCommissionRateField,RspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspQryInstrumentCommissionRate, 0, 0, 0, sizeof(tmp), &tmp);

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryInstrumentCommissionRate(�����ѯ��Լ����������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, 
                         tmp.RspInfoField.ErrorMsg,
                         nRequestID,bIsLast
                         );

    }
    else
    {
        //pInstrumentCommissionRate=(CThostFtdcInstrumentCommissionRateField*)&tmp.InstrumentCommissionRateField;
        LOG_INFO("OnRspQryInstrumentCommissionRate(�����ѯ��Լ����������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InstrumentID=[%s],\t InvestorRange=[%d],\t BrokerID=[%s],\t InvestorID=[%s],\t OpenRatioByMoney=[%g],\n"
                        "\t\t\t OpenRatioByVolume=[%g],\t CloseRatioByMoney=[%g],\t CloseRatioByVolume=[%g],\t CloseTodayRatioByMoney=[%g],\t CloseTodayRatioByVolume=[%g]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                        pInstrumentCommissionRate->InstrumentID,
                        pInstrumentCommissionRate->InvestorRange,
                        pInstrumentCommissionRate->BrokerID,
                        pInstrumentCommissionRate->InvestorID,
                        pInstrumentCommissionRate->OpenRatioByMoney,
                        pInstrumentCommissionRate->OpenRatioByVolume,
                        pInstrumentCommissionRate->CloseRatioByMoney,
                        pInstrumentCommissionRate->CloseRatioByVolume,
                        pInstrumentCommissionRate->CloseTodayRatioByMoney,
                        pInstrumentCommissionRate->CloseTodayRatioByVolume
                        );
    }

}

///�����ѯ��������Ӧ
void CTraderSpi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    if(!pExchange)
    {
        LOG_INFO("OnRspQryExchange(�����ѯ��������Ӧ),pExchange is NULL");
        return;
    }

    DataRspQryExchange tmp(nRequestID,bIsLast,*pExchange,pRspInfo?(*pRspInfo):BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspQryExchange, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRspQryExchange(�����ѯ��������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],ExchangeID=[%s],ExchangeName=[%s],ExchangeProperty=[%c],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, 
			pExchange->ExchangeID, pExchange->ExchangeName,pExchange->ExchangeProperty,nRequestID,bIsLast);
}

///�����ѯ��Լ��Ӧ
void CTraderSpi::OnRspQryInstrument(PlatformStru_InstrumentInfo *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    if(!pInstrument)
    {
        LOG_INFO("OnRspQryInstrument(�����ѯ��Լ��Ӧ),pInstrument is NULL");
        return;
    }

    PlatformStru_InstrumentInfo InstrumentField;
//    if(pInstrument->ProductClass!=THOST_FTDC_PC_Futures && pInstrument->ProductClass!=THOST_FTDC_PC_Combination)
//        InstrumentField.clear();
//    else
	    //CopyInstrumentField( InstrumentField, *pInstrument);
		InstrumentField = *pInstrument;

    DataRspQryInstrument tmp(nRequestID,bIsLast,InstrumentField,pRspInfo?(*pRspInfo):BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspQryInstrument, 0, 0, 0, sizeof(tmp), &tmp); 

    LOG_INFO("OnRspQryInstrument(�����ѯ��Լ��Ӧ) : ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
			 "\t\t\t InstrumentID=[%s],\t ExchangeID=[%s],\t InstrumentName=[%s],\t ExchangeInstID=[%s],\t ProductID=[%s], \n"
			 "\t\t\t ProductClass=[%c],\t DeliveryYear=[%u],\t DeliveryMonth=[%u],\t MaxMarketOrderVolume=[%d],\t MinMarketOrderVolume=[%d], \n"
			 "\t\t\t MaxLimitOrderVolume=[%d],\t MinLimitOrderVolume=[%d],\t VolumeMultiple=[%d],\t PriceTick=[%g],\t CreateDate=[%s], \n"
			 "\t\t\t OpenDate=[%s],\t ExpireDate=[%s],\t StartDelivDate=[%s],\t EndDelivDate=[%s],\t InstLifePhase=[%c], \n"
			 "\t\t\t IsTrading=[%d],\t PositionType=[%c],\t PositionDateType=[%c],\t LongMarginRatio=[%g],\t ShortMarginRatio=[%g]\n"
             "\t\t\t MaxMarginSideAlgorithm=[%d]", 
			 tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID, bIsLast,
			 pInstrument->InstrumentID, pInstrument->ExchangeID, pInstrument->InstrumentName, pInstrument->ExchangeInstID, pInstrument->ProductID, 
			 pInstrument->ProductClass, pInstrument->DeliveryYear, pInstrument->DeliveryMonth, pInstrument->MaxMarketOrderVolume, pInstrument->MinMarketOrderVolume, 
			 pInstrument->MaxLimitOrderVolume, pInstrument->MinLimitOrderVolume, pInstrument->VolumeMultiple, pInstrument->PriceTick, pInstrument->CreateDate, 
			 pInstrument->OpenDate, pInstrument->ExpireDate, pInstrument->StartDelivDate, pInstrument->EndDelivDate, pInstrument->InstLifePhase, 
			 pInstrument->IsTrading, pInstrument->PositionType, pInstrument->PositionDateType, pInstrument->LongMarginRatio, pInstrument->ShortMarginRatio,
             pInstrument->MaxMarginSideAlgorithm
			 );

}

///�����ѯ������Ӧ
void CTraderSpi::OnRspQryDepthMarketData(PlatformStru_DepthMarketData *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    if(pDepthMarketData&&m_RefPlatFormService.m_pQuotSpi)
    {
        m_RefPlatFormService.m_pQuotSpi->SetRtnDepthMarketData(pDepthMarketData);
    }

    if(pDepthMarketData)
    {
        //PlatformStru_DepthMarketData field(*pDepthMarketData);
        DataRspQryDepthMarketData tmp(nRequestID,bIsLast,*pDepthMarketData,pRspInfo?(*pRspInfo):BlankRspInfo);

        m_RefPackageCache.SaveDataTo(BID_RspQryDepthMarketData, 0, 0, 0, sizeof(tmp), &tmp); 
    }
    else
    {
        DataRspQryDepthMarketData tmp;
        m_RefPackageCache.SaveDataTo(BID_RspQryDepthMarketData, 0, 0, 0, sizeof(tmp), &tmp); 
    }

    LOG_INFO("OnRspQryDepthMarketData,Unimplemented");
}

///�����ѯͶ���߽�������Ӧ
void CTraderSpi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    CThostFtdcSettlementInfoField SettlementInfoField;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    if(!pSettlementInfo)
    {
        memset(&SettlementInfoField,0,sizeof(SettlementInfoField));
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        SettlementInfoField = *pSettlementInfo;
    }

    DataRspQrySettlementInfo tmp(nRequestID,bIsLast,SettlementInfoField,RspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspQrySettlementInfo, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQrySettlementInfo(�����ѯͶ���߽�������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );
		if(pRspInfo) memcpy(&m_RefPlatFormService.m_ErrorInfoField,pRspInfo,sizeof(CThostFtdcRspInfoField));
    }
    else
    {
        pSettlementInfo= &tmp.SettlementInfoField;
        LOG_INFO("OnRspQrySettlementInfo(�����ѯͶ���߽�������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t TradingDay=[%s],\t SettlementID=[%d],\t BrokerID=[%s],\t InvestorID=[%s],\t SequenceNo=[%d],\n"
                        "\t\t\t Content=[%s]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                        pSettlementInfo->TradingDay, pSettlementInfo->SettlementID,pSettlementInfo->BrokerID,pSettlementInfo->InvestorID,pSettlementInfo->SequenceNo,
                        pSettlementInfo->Content
                        );
    }
}

///�����ѯת��������Ӧ
void CTraderSpi::OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
   if(bIsLast)  CQryQueue::ClearbInQry();

   LOG_INFO("OnRspQryTransferBank,Unimplemented");
}

///�����ѯͶ���ֲ߳���ϸ��Ӧ
void CTraderSpi::OnRspQryInvestorPositionDetail(PlatformStru_PositionDetail *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast) CQryQueue::ClearbInQry();

    PlatformStru_PositionDetail InvestorPositionDetailField;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    TThostFtdcInstrumentIDType	QryInstrumentID;
    memset(&QryInstrumentID,0,sizeof(QryInstrumentID));
    if(!pInvestorPositionDetail)
    {
        InvestorPositionDetailField.Clear();
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        //CopyPositionDetailField( InvestorPositionDetailField, *pInvestorPositionDetail);
        InvestorPositionDetailField = *pInvestorPositionDetail;
    }

    DataRspQryInvestorPositionDetail tmp(nRequestID,bIsLast,InvestorPositionDetailField,RspInfo,QryInstrumentID);

    m_RefPackageCache.SaveDataTo(BID_RspQryInvestorPositionDetail, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryInvestorPositionDetail(�����ѯͶ���ֲ߳���ϸ��Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        //pInvestorPositionDetail=&tmp.InvestorPositionDetailField;
        LOG_INFO("OnRspQryInvestorPositionDetail(�����ѯͶ���ֲ߳���ϸ��Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InstrumentID=[%s],\t BrokerID=[%s],\t InvestorID=[%s],\t HedgeFlag=[%d],\t Direction=[%d],\n"
                        "\t\t\t OpenDate=[%s],\t TradeID=[%s],\t Volume=[%d],\t OpenPrice=[%g],\t TradingDay=[%s],\n"
                        "\t\t\t SettlementID=[%d],\t  TradeType=[%d],\t CombInstrumentID=[%s],\t ExchangeID=[%s],\t CloseProfitByDate=[%g],\n"
                        "\t\t\t CloseProfitByTrade=[%g],\t PositionProfitByDate=[%g],\t PositionProfitByTrade=[%g],\t Margin=[%g],\t ExchMargin=[%g],\n"
                        "\t\t\t MarginRateByMoney=[%g],\t MarginRateByVolume=[%g],\t LastSettlementPrice=[%g],\t SettlementPrice=[%g],CloseVolume=[%d],\n"
                        "\t\t\t CloseAmount=[%g]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                        pInvestorPositionDetail->InstrumentID, pInvestorPositionDetail->BrokerID,pInvestorPositionDetail->InvestorID,pInvestorPositionDetail->HedgeFlag,pInvestorPositionDetail->Direction,
                        pInvestorPositionDetail->OpenDate, pInvestorPositionDetail->TradeID,pInvestorPositionDetail->Volume,pInvestorPositionDetail->OpenPrice,pInvestorPositionDetail->TradingDay,
                        pInvestorPositionDetail->SettlementID, pInvestorPositionDetail->TradeType,pInvestorPositionDetail->CombInstrumentID,pInvestorPositionDetail->ExchangeID,pInvestorPositionDetail->CloseProfitByDate,
                        pInvestorPositionDetail->CloseProfitByTrade, pInvestorPositionDetail->PositionProfitByDate,pInvestorPositionDetail->PositionProfitByTrade,pInvestorPositionDetail->Margin,pInvestorPositionDetail->ExchMargin,
                        pInvestorPositionDetail->MarginRateByMoney, pInvestorPositionDetail->MarginRateByVolume,pInvestorPositionDetail->LastSettlementPrice,pInvestorPositionDetail->SettlementPrice,pInvestorPositionDetail->CloseVolume,
                        pInvestorPositionDetail->CloseAmount
                        );
    }
}

///�����ѯͶ���ֲ߳���Ӧ
void CTraderSpi::OnRspQryInvestorPosition(PlatformStru_Position *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    PlatformStru_Position InvestorPositionField;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    TThostFtdcInstrumentIDType	QryInstrumentID;
    memset(&QryInstrumentID,0,sizeof(QryInstrumentID));
    if(!pInvestorPosition)
    {
        InvestorPositionField.Clear();
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {    
        //CopyPositionField( InvestorPositionField, *pInvestorPosition );
        InvestorPositionField = *pInvestorPosition;
    }

    DataRspQryInvestorPosition tmp(nRequestID,bIsLast,InvestorPositionField,RspInfo,QryInstrumentID);

    m_RefPackageCache.SaveDataTo(BID_RspQryInvestorPosition, 0, 0, 0, sizeof(tmp), &tmp);

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryInvestorPosition(�����ѯͶ���ֲ߳���Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else if(pInvestorPosition!=NULL)
    {
        LOG_INFO("OnRspQryInvestorPosition(�����ѯͶ���ֲ߳���Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InstrumentID=[%s],\t BrokerID=[%s],\t InvestorID=[%s],\t PosiDirection=[%d],\t HedgeFlag=[%d],\n"
                        "\t\t\t PositionDate=[%d],\t YdPosition=[%d],\t Position=[%d],\t LongFrozen=[%d],\t ShortFrozen=[%d],\n"
                        "\t\t\t LongFrozenAmount=[%g],\t  ShortFrozenAmount=[%g],\t OpenVolume=[%d],\t CloseVolume=[%d],\t OpenAmount=[%g],\n"
                        "\t\t\t CloseAmount=[%g],\t PositionCost=[%g],\t PreMargin=[%g],\t UseMargin=[%g],\t FrozenMargin=[%g],\n"
                        "\t\t\t FrozenCash=[%g],\t FrozenCommission=[%g],\t CashIn=[%g],\t Commission=[%g],CloseProfit=[%g],\n"
                        "\t\t\t PositionProfit=[%g],\t PreSettlementPrice=[%g],\t SettlementPrice=[%g],\t TradingDay=[%s],\t SettlementID=[%d],\n"
                        "\t\t\t OpenCost=[%g],\t ExchangeMargin=[%g],\t CombPosition=[%d],\t CombLongFrozen=[%d],\t CombShortFrozen=[%d],\n"
                        "\t\t\t CloseProfitByDate=[%g],\t CloseProfitByTrade=[%g],\t TodayPosition=[%d],\t MarginRateByMoney=[%g],\t MarginRateByVolume=[%g]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                        pInvestorPosition->InstrumentID, pInvestorPosition->BrokerID,pInvestorPosition->InvestorID,pInvestorPosition->PosiDirection,pInvestorPosition->HedgeFlag,
                        pInvestorPosition->PositionDate, pInvestorPosition->YdPosition,pInvestorPosition->Position,pInvestorPosition->LongFrozen,pInvestorPosition->ShortFrozen,
                        pInvestorPosition->LongFrozenAmount, pInvestorPosition->ShortFrozenAmount,pInvestorPosition->OpenVolume,pInvestorPosition->CloseVolume,pInvestorPosition->OpenAmount,
                        pInvestorPosition->CloseAmount, pInvestorPosition->PositionCost,pInvestorPosition->PreMargin,pInvestorPosition->UseMargin,pInvestorPosition->FrozenMargin,
                        pInvestorPosition->FrozenCash, pInvestorPosition->FrozenCommission,pInvestorPosition->CashIn,pInvestorPosition->Commission,pInvestorPosition->CloseProfit,
                        pInvestorPosition->PositionProfit, pInvestorPosition->PreSettlementPrice,pInvestorPosition->SettlementPrice,pInvestorPosition->TradingDay,pInvestorPosition->SettlementID,
                        pInvestorPosition->OpenCost, pInvestorPosition->ExchangeMargin,pInvestorPosition->CombPosition,pInvestorPosition->CombLongFrozen,pInvestorPosition->CombShortFrozen,
                        pInvestorPosition->CloseProfitByDate, pInvestorPosition->CloseProfitByTrade,pInvestorPosition->TodayPosition,pInvestorPosition->MarginRateByMoney,pInvestorPosition->MarginRateByVolume
                        );
    }


}


///�����ѯͶ������ϳֲ���ϸ��Ӧ
void CTraderSpi::OnRspQryInvestorPositionCombineDetail(PlatformStru_PositionDetailComb *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    if(!GlobalConfigManager::m_bUseComb)
        return;

    PlatformStru_PositionDetailComb InvestorPositionCombineDetailField;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    TThostFtdcInstrumentIDType	QryInstrumentID;
    memset(&QryInstrumentID,0,sizeof(QryInstrumentID));
    if(!pInvestorPositionCombineDetail)
    {
        InvestorPositionCombineDetailField.Clear();
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
		//CopyPositionCombDetailField( InvestorPositionCombineDetailField, *pInvestorPositionCombineDetail);
		InvestorPositionCombineDetailField = *pInvestorPositionCombineDetail;
    }

    DataRspQryInvestorPositionCombDetail tmp(nRequestID,bIsLast,InvestorPositionCombineDetailField,RspInfo,QryInstrumentID);

    m_RefPackageCache.SaveDataTo(BID_RspQryInvestorPositionCombDetail, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryInvestorPositionCombineDetail(�����ѯͶ������ϳֲ���ϸ��Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );
    }
    else
    {
		//CThostFtdcInvestorPositionCombineDetailField* pDetail = pInvestorPositionCombineDetail;
		PlatformStru_PositionDetailComb* pDetail = pInvestorPositionCombineDetail;
		LOG_INFO("OnRspQryInvestorPositionCombineDetail(�����ѯͶ������ϳֲ���ϸ��Ӧ) : ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
				 "\t\t\t TradingDay=[%s],\t OpenDate=[%s],\t ExchangeID=[%s],\t SettlementID=[%d],\t BrokerID=[%s], \n"
				 "\t\t\t InvestorID=[%s],\t ComTradeID=[%s],\t TradeID=[%s],\t InstrumentID=[%s],\t HedgeFlag=[%c], \n"
				 "\t\t\t Direction=[%c],\t TotalAmt=[%d],\t Margin=[%g],\t ExchMargin=[%g],\t MarginRateByMoney=[%g], \n"
				 "\t\t\t MarginRateByVolume=[%g],\t LegID=[%d],\t LegMultiple=[%d],\t CombInstrumentID=[%s]", 
				 tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
				 pDetail->TradingDay, pDetail->OpenDate, pDetail->ExchangeID, pDetail->SettlementID, pDetail->BrokerID, 
				 pDetail->InvestorID, pDetail->ComTradeID, pDetail->TradeID, pDetail->InstrumentID, pDetail->HedgeFlag, 
				 pDetail->Direction, pDetail->TotalAmt, pDetail->Margin, pDetail->ExchMargin, pDetail->MarginRateByMoney, 
				 pDetail->MarginRateByVolume, pDetail->LegID, pDetail->LegMultiple, pDetail->CombInstrumentID
				 );
    }
}



///�����ѯ�ͻ�֪ͨ��Ӧ
void CTraderSpi::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    CThostFtdcNoticeField NoticeField;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    if(!pNotice)
    {
		memset(&NoticeField,0,sizeof(NoticeField));
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        NoticeField=*pNotice;
    }

    DataRspQryNotice tmp(nRequestID,bIsLast,NoticeField,RspInfo);
    m_RefPackageCache.SaveDataTo(BID_RspQryNotice, 0, 0, 0, sizeof(tmp), &tmp);
    if(pNotice)
        LOG_INFO("OnRspQryNotice(�����ѯ�ͻ�֪ͨ��Ӧ)\n\t\t\t BrokerID=%s Content=%s SequenceLabel=%s",
        pNotice->BrokerID,pNotice->Content,pNotice->SequenceLabel);
    else
        LOG_INFO("OnRspQryNotice(�����ѯ�ͻ�֪ͨ��Ӧ)");
}

///�����ѯ������Ϣȷ����Ӧ
void CTraderSpi::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();
	if(pSettlementInfoConfirm==NULL)
		m_RefPlatFormService.m_bSettlementInfoConfirmed=false;
	else
		m_RefPlatFormService.m_bSettlementInfoConfirmed=pSettlementInfoConfirm->ConfirmTime[0]!=0?true:false;
	m_PlatformParam.TradeStatus=CTPCONNSTATUS_Connected;
    CThostFtdcSettlementInfoConfirmField SettlementInfoConfirm;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    if(!pSettlementInfoConfirm)
    {
        memset(&SettlementInfoConfirm,0,sizeof(SettlementInfoConfirm));
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        SettlementInfoConfirm=*pSettlementInfoConfirm;
    }

    DataRspQrySettlementInfoConfirm tmp(nRequestID,bIsLast,SettlementInfoConfirm,RspInfo);
    m_RefPackageCache.SaveDataTo(BID_RspQrySettlementInfoConfirm,0,0,0,sizeof(tmp), &tmp);
	if(pRspInfo) memcpy(&m_RefPlatFormService.m_ErrorInfoField,pRspInfo,sizeof(CThostFtdcRspInfoField));
    LOG_INFO("OnRspQrySettlementInfoConfirm(�����ѯͶ���߽�����ȷ����Ӧ)\n"
        "\t\t\t m_bSettlementInfoConfirmed=%d\n"
        "\t\t\t pSettlementInfoConfirm=%x \n"
        "\t\t\t BrokerID=%s InvestorID=%s ConfirmDate=%s ConfirmTime=%s",
        m_RefPlatFormService.m_bSettlementInfoConfirmed,
        pSettlementInfoConfirm,
        pSettlementInfoConfirm?pSettlementInfoConfirm->BrokerID:"-",
        pSettlementInfoConfirm?pSettlementInfoConfirm->InvestorID:"-",
        pSettlementInfoConfirm?pSettlementInfoConfirm->ConfirmDate:"-",
        pSettlementInfoConfirm?pSettlementInfoConfirm->ConfirmTime:"-");
}

///�����ѯת����ˮ��Ӧ
void CTraderSpi::OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    CThostFtdcTransferSerialField TransferSerialField;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    if(!pTransferSerial)
    {
        memset(&TransferSerialField,0,sizeof(TransferSerialField));
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        TransferSerialField=*pTransferSerial;
    }

    DataRspQryTransferSerial tmp(nRequestID,bIsLast,TransferSerialField,RspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspQryTransferSerial, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryTransferSerial(�����ѯת����ˮ��Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        pTransferSerial=& tmp.TransferSerialField;
        LOG_INFO("OnRspQryTransferSerial(�����ѯת����ˮ��Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t PlateSerial=[%d],\t TradeDate=[%s],\t TradingDay=[%s],\t TradeTime=[%s],\t TradeCode=[%s],\n"
                        "\t\t\t SessionID=[%#x],\t BankID=[%s],\t BankBranchID=[%s],\t BankAccType=[%d],\t BankAccount=[%s],\n"
                        "\t\t\t BankSerial=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\t FutureAccType=[%d],\t AccountID=[%s],\n"
                        "\t\t\t InvestorID=[%s],\t FutureSerial=[%d],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\t CurrencyID=[%s],\n"
                        "\t\t\t TradeAmount=[%g],\t CustFee=[%g],BrokerFee=[%g],\t AvailabilityFlag=[%d],\t OperatorCode=[%s],\n"
                        "\t\t\t BankNewAccount=[%s],\t --ErrorID=[%d],\t --ErrorMsg=[%d]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                        pTransferSerial->PlateSerial, pTransferSerial->TradeDate,pTransferSerial->TradingDay,pTransferSerial->TradeTime,pTransferSerial->TradeCode,
                        pTransferSerial->SessionID, pTransferSerial->BankID,pTransferSerial->BankBranchID,pTransferSerial->BankAccType,pTransferSerial->BankAccount,
                        pTransferSerial->BankSerial, pTransferSerial->BrokerID,pTransferSerial->BrokerBranchID,pTransferSerial->FutureAccType,pTransferSerial->AccountID,
                        pTransferSerial->InvestorID, pTransferSerial->FutureSerial,pTransferSerial->IdCardType,pTransferSerial->IdentifiedCardNo,pTransferSerial->CurrencyID,
                        pTransferSerial->TradeAmount,pTransferSerial->CustFee, pTransferSerial->BrokerFee,pTransferSerial->AvailabilityFlag,pTransferSerial->OperatorCode,
                        pTransferSerial->BankNewAccount, pTransferSerial->ErrorID, pTransferSerial->ErrorMsg
                        );
    }
}

///����Ӧ��
void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
LOG_INFO("CTraderSpi::OnRspError,ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
          pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, int(bIsLast));
}

///����֪ͨ
void CTraderSpi::OnRtnOrder(PlatformStru_OrderInfo *pOrder)
{
    if(!pOrder)
    {
        LOG_INFO("OnRtnOrder(����֪ͨ),pOrder is NULL");
        return;
    }

    PlatformStru_OrderInfo OrderField;
    //CopyOrderField(OrderField,*pOrder);
    OrderField = *pOrder;
    DataRtnOrder tmp(OrderField);

    int OriSetOff=-1,NewSetOff=-1;
    bool bSave=m_RefPackageCache.SaveDataTo(BID_RtnOrder, 0, 0, 0, sizeof(tmp), &tmp,&OriSetOff,&NewSetOff);
    LOG_INFO("OnRtnOrder(����֪ͨ) : \n\t\t\t bSaveOK=%d OriSetOff=%d NewSetOff=%d SaveLen=%d\n"
                    "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                    "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
                    "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
                    "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
                    "\t\t\t RequestID=[%d],\t OrderLocalID=[%s],\t ExchangeID=[%s],\t ParticipantID=[%s],\t ClientID=[%s],\n"
                    "\t\t\t ExchangeInstID=[%s],\t TraderID=[%s],\t InstallID=[%d],\t OrderSubmitStatus=[%d],\t NotifySequence=[%d],\n"
                    "\t\t\t TradingDay=[%s],\t SettlementID=[%d],\t OrderSysID=[%s],\t OrderSource=[%d],\t OrderStatus=[%d],\n"
                    "\t\t\t OrderType=[%d],\t VolumeTraded=[%d],\t VolumeTotal=[%d],\t InsertDate=[%s],\t InsertTime=[%s],\n"
                    "\t\t\t ActiveTime=[%s],\t SuspendTime=[%s],\t UpdateTime=[%s],\t CancelTime=[%s],\t ActiveTraderID=[%s]\n"
                    "\t\t\t ClearingPartID=[%s],\t SequenceNo=[%d],\t FrontID=[%d],\t SessionID=[%#x],\t UserProductInfo=[%s],\n"
                    "\t\t\t StatusMsg=[%s],\t UserForceClose=[%d],\t ActiveUserID=[%s],\t BrokerOrderSeq=[%d],RelativeOrderSysID=[%s]",
                    bSave,OriSetOff,NewSetOff,sizeof(tmp),
                    pOrder->BrokerID,pOrder->InvestorID,pOrder->InstrumentID,pOrder->OrderRef,pOrder->UserID,
                    pOrder->OrderPriceType,pOrder->Direction,pOrder->CombOffsetFlag,pOrder->CombHedgeFlag,pOrder->LimitPrice,
                    pOrder->VolumeTotalOriginal,pOrder->TimeCondition,pOrder->GTDDate,pOrder->VolumeCondition,pOrder->MinVolume,
                    pOrder->ContingentCondition,pOrder->StopPrice,pOrder->ForceCloseReason,pOrder->IsAutoSuspend,pOrder->BusinessUnit,
                    pOrder->RequestID,pOrder->OrderLocalID,pOrder->ExchangeID,pOrder->ParticipantID,pOrder->ClientID,
                    pOrder->ExchangeInstID,pOrder->TraderID,pOrder->InstallID,pOrder->OrderSubmitStatus,pOrder->NotifySequence,
                    pOrder->TradingDay,pOrder->SettlementID,pOrder->OrderSysID,pOrder->OrderSource,pOrder->OrderStatus,
                    pOrder->OrderType,pOrder->VolumeTraded,pOrder->VolumeTotal,pOrder->InsertDate,pOrder->InsertTime,
                    pOrder->ActiveTime,pOrder->SuspendTime,pOrder->UpdateTime,pOrder->CancelTime,pOrder->ActiveTraderID,
                    pOrder->ClearingPartID,pOrder->SequenceNo,pOrder->FrontID,pOrder->SessionID,pOrder->UserProductInfo,
                    pOrder->StatusMsg,pOrder->UserForceClose,pOrder->ActiveUserID,pOrder->BrokerOrderSeq,pOrder->RelativeOrderSysID
                    );
}

///�ɽ�֪ͨ
void CTraderSpi::OnRtnTrade(PlatformStru_TradeInfo *pTrade)
{
    if(!pTrade)
    {
        LOG_INFO("OnRtnTrade(�ɽ�֪ͨ),pTrade is NULL");
        return;
    }

    PlatformStru_TradeInfo  TradeField;
    //CopyTradeRecordField( TradeField, *pTrade);
    TradeField = *pTrade;
    DataRtnTrade tmp(TradeField);

    int OriSetOff=-1,NewSetOff=-1;
    bool bSave=m_RefPackageCache.SaveDataTo(BID_RtnTrade, 0, 0, 0, sizeof(tmp), &tmp,&OriSetOff,&NewSetOff);
    LOG_INFO("OnRtnTrade(�ɽ�֪ͨ) : \n\t\t\t bSaveOK=%d OriSetOff=%d NewSetOff=%d SaveLen=%d\n"
                    "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                    "\t\t\t ExchangeID=[%s],\t TradeID=[%s],\t Direction=[%d],\t OrderSysID=[%s],\t ParticipantID=[%s],\n"
                    "\t\t\t ClientID=[%s],\t TradingRole=[%d],\t ExchangeInstID=[%s],\t OffsetFlag=[%d],\t HedgeFlag=[%d],\n"
                    "\t\t\t Price=[%g],\t Volume=[%d],\t TradeDate=[%s],\t TradeTime=[%s],\t TradeType=[%d],\n"
                    "\t\t\t PriceSource=[%d],\t TraderID=[%s],\t OrderLocalID=[%s],\t ClearingPartID=[%s],\t BusinessUnit=[%s],\n"
                    "\t\t\t SequenceNo=[%d],\t TradingDay=[%s],\t SettlementID=[%d],\t BrokerOrderSeq=[%d]",
                    bSave,OriSetOff,NewSetOff,sizeof(tmp),
                    pTrade->BrokerID,pTrade->InvestorID,pTrade->InstrumentID,pTrade->OrderRef,pTrade->UserID,
                    pTrade->ExchangeID,pTrade->TradeID,pTrade->Direction,pTrade->OrderSysID,pTrade->ParticipantID,
                    pTrade->ClientID,pTrade->TradingRole,pTrade->ExchangeInstID,pTrade->OffsetFlag,pTrade->HedgeFlag,
                    pTrade->Price,pTrade->Volume,pTrade->TradeDate,pTrade->TradeTime,pTrade->TradeType,
                    pTrade->PriceSource,pTrade->TraderID,pTrade->OrderLocalID,pTrade->ClearingPartID,pTrade->BusinessUnit,
                    pTrade->SequenceNo,pTrade->TradingDay,pTrade->SettlementID,pTrade->BrokerOrderSeq,pTrade->TradeSource
                    );
}

///����¼�����ر�
void CTraderSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
    if(!pInputOrder)
    {
        LOG_INFO("OnRspOrderInsert(����¼�����ر�),pInputOrder is NULL");
        return;
    }

    DataErrRtnOrderInsert tmp(-1,true,*pInputOrder,pRspInfo?(*pRspInfo):BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_ErrRtnOrderInsert, 0, 0, 0, sizeof(tmp), &tmp); 
    LOG_INFO("OnErrRtnOrderInsert(����¼�����ر�) : ErrorID=[%d],ErrorMsg=[%s],\n"
                    "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                    "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
                    "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
                    "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
                    "\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
                    tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, 
                    pInputOrder->BrokerID, pInputOrder->InvestorID, pInputOrder->InstrumentID, pInputOrder->OrderRef,pInputOrder->UserID,
                    pInputOrder->OrderPriceType, pInputOrder->Direction, pInputOrder->CombOffsetFlag, pInputOrder->CombHedgeFlag,pInputOrder->LimitPrice,
                    pInputOrder->VolumeTotalOriginal, pInputOrder->TimeCondition, pInputOrder->GTDDate, pInputOrder->VolumeCondition,pInputOrder->MinVolume,
                    pInputOrder->ContingentCondition, pInputOrder->StopPrice, pInputOrder->ForceCloseReason, pInputOrder->IsAutoSuspend,pInputOrder->BusinessUnit,
                    pInputOrder->RequestID, pInputOrder->UserForceClose);
}

///������������ر�
void CTraderSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
    if(!pOrderAction)
    {
        LOG_INFO("OnErrRtnOrderAction(������������ر�),pOrderAction is NULL");
        return;
    }

    DataErrRtnOrderAction tmp(*pOrderAction,pRspInfo?(*pRspInfo):BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_ErrRtnOrderAction, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnErrRtnOrderAction(������������ر�) : ErrorID=[%d],ErrorMsg=[%s],\n"
                    "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
                    "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
                    "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t ActionDate=[%s],\t ActionTime=[%s],\t TraderID=[%s],\n"
                    "\t\t\t InstallID=[%d],\t OrderLocalID=[%s],\t ActionLocalID=[%s],\t ParticipantID=[%s],\t ClientID=[%s],\n"
                    "\t\t\t BusinessUnit=[%s],\t OrderActionStatus=[%d],\t UserID=[%s],\t StatusMsg=[%s],\t InstrumentID=[%s]",
                    tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,
                    pOrderAction->BrokerID, pOrderAction->InvestorID, pOrderAction->OrderActionRef, pOrderAction->OrderRef,pOrderAction->RequestID,
                    pOrderAction->FrontID, pOrderAction->SessionID, pOrderAction->ExchangeID, pOrderAction->OrderSysID,pOrderAction->ActionFlag,
                    pOrderAction->LimitPrice, pOrderAction->VolumeChange, pOrderAction->ActionDate, pOrderAction->ActionTime,pOrderAction->TraderID,
                    pOrderAction->InstallID, pOrderAction->OrderLocalID, pOrderAction->ActionLocalID, pOrderAction->ParticipantID,pOrderAction->ClientID,
                    pOrderAction->BusinessUnit, pOrderAction->OrderActionStatus, pOrderAction->UserID, pOrderAction->StatusMsg,pOrderAction->InstrumentID
    			);
 }

///��Լ����״̬֪ͨ
void CTraderSpi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
    if (!pInstrumentStatus)
    {
        LOG_INFO("OnRtnInstrumentStatus(��Լ����״̬֪ͨ),pInstrumentStatus is NULL");
        return;
    }

    DataInstrumentStatus tmp(-1,true,*pInstrumentStatus);

    m_RefPackageCache.SaveDataTo(BID_RtnInstrumentStatus, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRtnInstrumentStatus(��Լ����״̬֪ͨ):\n"
                     "\t\t\t ExchangeID=[%s],\t ExchangeInstID=[%s],\t SettlementGroupID=[%s],\t InstrumentID=[%s],\t InstrumentStatus=[%d],\n"
                     "\t\t\t TradingSegmentSN=[%d],\t EnterTime=[%s],\t EnterReason=[%d]",
                     pInstrumentStatus->ExchangeID,
                     pInstrumentStatus->ExchangeInstID,
                     pInstrumentStatus->SettlementGroupID,
                     pInstrumentStatus->InstrumentID,
                     pInstrumentStatus->InstrumentStatus,
                     pInstrumentStatus->TradingSegmentSN,
                     pInstrumentStatus->EnterTime,
                     pInstrumentStatus->EnterReason);
}

///����֪ͨ
void CTraderSpi::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo)
{
    if(!pTradingNoticeInfo)
    {
        LOG_INFO("OnRtnTradingNotice(����֪ͨ),pTradingNoticeInfo is NULL");
        return;
    }

    DataTradingNoticeInfoField tmp(*pTradingNoticeInfo);

    m_RefPackageCache.SaveDataTo(BID_RtnTradingNotice, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRtnTradingNotice(����֪ͨ):\n"
           "\t\t\t BrokerID=[%s],\tInvestorID=[%s],SendTime=[%s],\n"
           "\t\t\t FieldContent=[%s],\n"
           "\t\t\t SequenceSeries=[%d],SequenceNo=[%d]",
             pTradingNoticeInfo->BrokerID,
             pTradingNoticeInfo->InvestorID,
             pTradingNoticeInfo->SendTime,
             pTradingNoticeInfo->FieldContent,
             pTradingNoticeInfo->SequenceSeries,
             pTradingNoticeInfo->SequenceNo);
}

///�����ѯǩԼ������Ӧ
void CTraderSpi::OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    if(!pContractBank)
    {
        LOG_INFO("OnRspQryContractBank(�ڻ������ѯ�������֪ͨ),pContractBank is NULL");
        return;
    }

    DataRspQryContractBank tmp(nRequestID,bIsLast,*pContractBank,pRspInfo?(*pRspInfo):BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspQryContractBank, 0, 0, 0, sizeof(tmp), &tmp);

    LOG_INFO("OnRspQryContractBank(�����ѯǩԼ������Ӧ):  bIsLast=%d\n"
           "\t\t\t BrokerID=[%s],BankID=[%s],BankBrchID=[%s],BankName=[%s]",
             bIsLast,
             pContractBank->BrokerID,
             pContractBank->BankID,
             pContractBank->BankBrchID,
             pContractBank->BankName);
}

///�����ѯ����֪ͨ��Ӧ
void CTraderSpi::OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    LOG_INFO("OnRspQryTradingNotice,Unimplemented");
}

///�����ѯ���͹�˾���ײ�����Ӧ
void CTraderSpi::OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    LOG_INFO("OnRspQryBrokerTradingParams,Unimplemented");
}

///�����ѯ���͹�˾�����㷨��Ӧ
void CTraderSpi::OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    LOG_INFO("OnRspQryBrokerTradingAlgos,Unimplemented");
}

///���з��������ʽ�ת�ڻ�֪ͨ
void CTraderSpi::OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer)
{
LOG_INFO("OnRtnFromBankToFutureByBank,Unimplemented");
}

///���з����ڻ��ʽ�ת����֪ͨ
void CTraderSpi::OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer)
{
LOG_INFO("OnRtnFromFutureToBankByBank,Unimplemented");
}

///���з����������ת�ڻ�֪ͨ
void CTraderSpi::OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal)
{
LOG_INFO("OnRtnRepealFromBankToFutureByBank,Unimplemented");
}

///���з�������ڻ�ת����֪ͨ
void CTraderSpi::OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal)
{
LOG_INFO("OnRtnRepealFromFutureToBankByBank,Unimplemented");
}

///�ڻ����������ʽ�ת�ڻ�֪ͨ
void CTraderSpi::OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
if(!pRspTransfer)
    {
        LOG_INFO("OnRtnFromBankToFutureByFuture(�ڻ����������ʽ�ת�ڻ�֪ͨ),pRspTransfer is NULL");
        return;
    }
    DataRtnFromBankToFutureByFuture tmp(-1,true,*pRspTransfer,BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_RtnFromBankToFutureByFuture, 0, 0, 0, sizeof(tmp), &tmp);

//LOG_INFO("OnRtnFromBankToFutureByFuture(�ڻ����������ʽ�ת�ڻ�֪ͨ) : TradeCode=[%s],BankID=[%s],BankBranchID=[%d],BrokerID=[%d],\n"
//                        "\t\t\t BrokerBranchID=[%s],\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\n"
//                        "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t PlateSerial=[%d],\n"
//                        "\t\t\t LastFragment=[%s],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
//                        "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[%s],\n"
//                        "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
//                        "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
//                        "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
//                        "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
//                        "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d],\t ErrorID=[%d],\t ErrorMsg=[%s]",
//                        pRspTransfer->TradeCode, pRspTransfer->BankID,pRspTransfer->BankBranchID,pRspTransfer->BrokerID,pRspTransfer->BrokerBranchID,
//                        pRspTransfer->TradeDate, pRspTransfer->TradeTime,pRspTransfer->BankSerial,pRspTransfer->TradingDay,pRspTransfer->TradeDate,
//                        pRspTransfer->TradeTime, pRspTransfer->BankSerial, pRspTransfer->PlateSerial,
//                        pRspTransfer->LastFragment, pRspTransfer->SessionID,pRspTransfer->CustomerName,pRspTransfer->IdCardType,pRspTransfer->IdentifiedCardNo,
//                        pRspTransfer->CustType, pRspTransfer->BankAccount,pRspTransfer->BankPassWord,pRspTransfer->AccountID,pRspTransfer->Password,
//                        pRspTransfer->InstallID,pRspTransfer->FutureSerial, pRspTransfer->UserID,pRspTransfer->VerifyCertNoFlag,pRspTransfer->CurrencyID,
//                        pRspTransfer->TradeAmount,pRspTransfer->FutureFetchAmount, pRspTransfer->FeePayFlag,pRspTransfer->CustFee,pRspTransfer->BrokerFee,
//                        pRspTransfer->Message, pRspTransfer->Digest, pRspTransfer->BankAccType,pRspTransfer->DeviceID,pRspTransfer->BankSecuAccType,
//                        pRspTransfer->BrokerIDByBank, pRspTransfer->BankSecuAcc, pRspTransfer->BankPwdFlag,pRspTransfer->SecuPwdFlag,pRspTransfer->OperNo,
//                        pRspTransfer->RequestID, pRspTransfer->TID, pRspTransfer->TransferStatus,pRspTransfer->ErrorID,pRspTransfer->ErrorMsg
//                        );
}

///�ڻ������ڻ��ʽ�ת����֪ͨ
void CTraderSpi::OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
if(!pRspTransfer)
    {
        LOG_INFO("OnRtnFromFutureToBankByFuture(�ڻ������ڻ��ʽ�ת����֪ͨ),pRspTransfer is NULL");
        return;
    }
    DataRtnFromFutureToBankByFuture tmp(-1,true,*pRspTransfer,BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_RtnFromFutureToBankByFuture, 0, 0, 0, sizeof(tmp), &tmp);

//LOG_INFO("OnRtnFromFutureToBankByFuture(�ڻ������ڻ��ʽ�ת����֪ͨ) : TradeCode=[%s],BankID=[%s],BankBranchID=[%d],BrokerID=[%d],\n"
//                        "\t\t\t BrokerBranchID=[%s],\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\n"
//                        "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t PlateSerial=[%d],\n"
//                        "\t\t\t LastFragment=[%s],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
//                        "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[%s],\n"
//                        "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
//                        "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
//                        "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
//                        "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
//                        "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d],\t ErrorID=[%d],\t ErrorMsg=[%s]",
//                        pRspTransfer->TradeCode, pRspTransfer->BankID,pRspTransfer->BankBranchID,pRspTransfer->BrokerID,pRspTransfer->BrokerBranchID,
//                        pRspTransfer->TradeDate, pRspTransfer->TradeTime,pRspTransfer->BankSerial,pRspTransfer->TradingDay,pRspTransfer->TradeDate,
//                        pRspTransfer->TradeTime, pRspTransfer->BankSerial, pRspTransfer->PlateSerial,
//                        pRspTransfer->LastFragment, pRspTransfer->SessionID,pRspTransfer->CustomerName,pRspTransfer->IdCardType,pRspTransfer->IdentifiedCardNo,
//                        pRspTransfer->CustType, pRspTransfer->BankAccount,pRspTransfer->BankPassWord,pRspTransfer->AccountID,pRspTransfer->Password,
//                        pRspTransfer->InstallID,pRspTransfer->FutureSerial, pRspTransfer->UserID,pRspTransfer->VerifyCertNoFlag,pRspTransfer->CurrencyID,
//                        pRspTransfer->TradeAmount,pRspTransfer->FutureFetchAmount, pRspTransfer->FeePayFlag,pRspTransfer->CustFee,pRspTransfer->BrokerFee,
//                        pRspTransfer->Message, pRspTransfer->Digest, pRspTransfer->BankAccType,pRspTransfer->DeviceID,pRspTransfer->BankSecuAccType,
//                        pRspTransfer->BrokerIDByBank, pRspTransfer->BankSecuAcc, pRspTransfer->BankPwdFlag,pRspTransfer->SecuPwdFlag,pRspTransfer->OperNo,
//                        pRspTransfer->RequestID, pRspTransfer->TID, pRspTransfer->TransferStatus,pRspTransfer->ErrorID,pRspTransfer->ErrorMsg
//                        );
}

///ϵͳ����ʱ�ڻ����ֹ������������ת�ڻ��������д�����Ϻ��̷��ص�֪ͨ
void CTraderSpi::OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal)
{
LOG_INFO("OnRtnRepealFromBankToFutureByFutureManual,Unimplemented");
}

///ϵͳ����ʱ�ڻ����ֹ���������ڻ�ת�����������д�����Ϻ��̷��ص�֪ͨ
void CTraderSpi::OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal)
{
LOG_INFO("OnRtnRepealFromFutureToBankByFutureManual,Unimplemented");
}

///�ڻ������ѯ�������֪ͨ
void CTraderSpi::OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount)
{
    if(!pNotifyQueryAccount)
    {
        LOG_INFO("OnRtnQueryBankBalanceByFuture(�ڻ������ѯ�������֪ͨ),pNotifyQueryAccount is NULL");
        return;
    }
    DataRtnQueryBankAccountMoneyByFuture tmp(-1,true,*pNotifyQueryAccount,BlankRspInfo);

    m_RefPackageCache.SaveDataTo(BID_RtnQueryBankAccountMoneyByFuture, 0, 0, 0, sizeof(tmp), &tmp);
    //LOG_INFO("OnRtnTradingNotice(����֪ͨ):\n"
    //       "\t\t\t BrokerID=[%s],\tInvestorID=[%s],SendTime=[%s],\n"
    //       "\t\t\t FieldContent=[%s],\n"
    //       "\t\t\t SequenceSeries=[%d],SequenceNo=[%d]",
    //         pTradingNoticeInfo->BrokerID,
    //         pTradingNoticeInfo->InvestorID,
    //         pTradingNoticeInfo->SendTime,
    //         pTradingNoticeInfo->FieldContent,
    //         pTradingNoticeInfo->SequenceSeries,
    //         pTradingNoticeInfo->SequenceNo);
}

///�����ѯ����ǩԼ��ϵ��Ӧ
void CTraderSpi::OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(bIsLast)  CQryQueue::ClearbInQry();

    CThostFtdcAccountregisterField Accountregister;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    if(!pAccountregister)
    {
        memset(&Accountregister,0,sizeof(Accountregister));
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        Accountregister=*pAccountregister;
    }

    DataRspQryAccountregister tmp(nRequestID,bIsLast,Accountregister,RspInfo);

    if(bIsLast)  CQryQueue::ClearbInQry();

    m_RefPackageCache.SaveDataTo(BID_RspQryAccountregister, 0, 0, 0, sizeof(tmp), &tmp);

    ////log
    //if(tmp.RspInfoField.ErrorID!=0)
    //{
    //    LOG_INFO("OnRspQryTransferSerial(�����ѯ����ǩԼ��ϵ��Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
    //                     tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
    //                     );

    //}
    //else
    //{
    //    pTransferSerial=& tmp.TransferSerialField;
    //    LOG_INFO("OnRspQryTransferSerial(�����ѯ����ǩԼ��ϵ��Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
    //                    "\t\t\t PlateSerial=[%d],\t TradeDate=[%s],\t TradingDay=[%s],\t TradeTime=[%s],\t TradeCode=[%s],\n"
    //                    "\t\t\t SessionID=[%#x],\t BankID=[%s],\t BankBranchID=[%s],\t BankAccType=[%d],\t BankAccount=[%s],\n"
    //                    "\t\t\t BankSerial=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\t FutureAccType=[%d],\t AccountID=[%s],\n"
    //                    "\t\t\t InvestorID=[%s],\t FutureSerial=[%d],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\t CurrencyID=[%s],\n"
    //                    "\t\t\t TradeAmount=[%g],\t CustFee=[%g],BrokerFee=[%g],\t AvailabilityFlag=[%d],\t OperatorCode=[%s],\n"
    //                    "\t\t\t BankNewAccount=[%s],\t --ErrorID=[%d],\t --ErrorMsg=[%d]",
    //                    tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
    //                    pTransferSerial->PlateSerial, pTransferSerial->TradeDate,pTransferSerial->TradingDay,pTransferSerial->TradeTime,pTransferSerial->TradeCode,
    //                    pTransferSerial->SessionID, pTransferSerial->BankID,pTransferSerial->BankBranchID,pTransferSerial->BankAccType,pTransferSerial->BankAccount,
    //                    pTransferSerial->BankSerial, pTransferSerial->BrokerID,pTransferSerial->BrokerBranchID,pTransferSerial->FutureAccType,pTransferSerial->AccountID,
    //                    pTransferSerial->InvestorID, pTransferSerial->FutureSerial,pTransferSerial->IdCardType,pTransferSerial->IdentifiedCardNo,pTransferSerial->CurrencyID,
    //                    pTransferSerial->TradeAmount,pTransferSerial->CustFee, pTransferSerial->BrokerFee,pTransferSerial->AvailabilityFlag,pTransferSerial->OperatorCode,
    //                    pTransferSerial->BankNewAccount, pTransferSerial->ErrorID, pTransferSerial->ErrorMsg
    //                    );
    //}


}

///�ڻ����������ʽ�ת�ڻ�����ر�
void CTraderSpi::OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo)
{
LOG_INFO("OnErrRtnBankToFutureByFuture,Unimplemented");
}

///�ڻ������ڻ��ʽ�ת���д���ر�
void CTraderSpi::OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo)
{
LOG_INFO("OnErrRtnFutureToBankByFuture,Unimplemented");
}

///ϵͳ����ʱ�ڻ����ֹ������������ת�ڻ�����ر�
void CTraderSpi::OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo)
{
LOG_INFO("OnErrRtnRepealBankToFutureByFutureManual,Unimplemented");
}

///ϵͳ����ʱ�ڻ����ֹ���������ڻ�ת���д���ر�
void CTraderSpi::OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo)
{
LOG_INFO("OnErrRtnRepealFutureToBankByFutureManual,Unimplemented");
}

///�ڻ������ѯ����������ر�
void CTraderSpi::OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo)
{
LOG_INFO("OnErrRtnQueryBankBalanceByFuture,Unimplemented");
}

///�ڻ������������ת�ڻ��������д�����Ϻ��̷��ص�֪ͨ
void CTraderSpi::OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal)
{
LOG_INFO("OnRtnRepealFromBankToFutureByFuture,Unimplemented");
}

///�ڻ���������ڻ�ת�����������д�����Ϻ��̷��ص�֪ͨ
void CTraderSpi::OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal)
{
LOG_INFO("OnRtnRepealFromFutureToBankByFuture,Unimplemented");
}

///�ڻ����������ʽ�ת�ڻ�Ӧ��
void CTraderSpi::OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CThostFtdcReqTransferField TransferField;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    if(!pReqTransfer)
    {
        memset(&TransferField,0,sizeof(TransferField));
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        TransferField=*pReqTransfer;
    }

    DataRspFromBankToFutureByFuture tmp(nRequestID,bIsLast,TransferField,RspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspFromBankToFutureByFuture, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspFromBankToFutureByFuture(�ڻ����������ʽ�ת�ڻ�Ӧ��) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        pReqTransfer=& tmp.TransferField;
        //LOG_INFO("OnRspFromBankToFutureByFuture(�ڻ����������ʽ�ת�ڻ�Ӧ��) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
        //                "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
        //                "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
        //                "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
        //                "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[%s],\n"
        //                "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
        //                "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
        //                "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
        //                "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
        //                "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d]",
        //                tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
        //                pReqTransfer->TradeCode, pReqTransfer->BankID,pReqTransfer->BankBranchID,pReqTransfer->BrokerID,pReqTransfer->BrokerBranchID,
        //                pReqTransfer->TradeDate, pReqTransfer->TradeTime,pReqTransfer->BankSerial,pReqTransfer->TradingDay,pReqTransfer->PlateSerial,
        //                pReqTransfer->LastFragment, pReqTransfer->SessionID,pReqTransfer->CustomerName,pReqTransfer->IdCardType,pReqTransfer->IdentifiedCardNo,
        //                pReqTransfer->CustType, pReqTransfer->BankAccount,pReqTransfer->BankPassWord,pReqTransfer->AccountID,pReqTransfer->Password,
        //                pReqTransfer->InstallID,pReqTransfer->FutureSerial, pReqTransfer->UserID,pReqTransfer->VerifyCertNoFlag,pReqTransfer->CurrencyID,
        //                pReqTransfer->TradeAmount,pReqTransfer->FutureFetchAmount, pReqTransfer->FeePayFlag,pReqTransfer->CustFee,pReqTransfer->BrokerFee,
        //                pReqTransfer->Message, pReqTransfer->Digest, pReqTransfer->BankAccType,pReqTransfer->DeviceID,pReqTransfer->BankSecuAccType,
        //                pReqTransfer->BrokerIDByBank, pReqTransfer->BankSecuAcc, pReqTransfer->BankPwdFlag,pReqTransfer->SecuPwdFlag,pReqTransfer->OperNo,
        //                pReqTransfer->RequestID, pReqTransfer->TID, pReqTransfer->TransferStatus
        //                );
    }
}

///�ڻ������ڻ��ʽ�ת����Ӧ��
void CTraderSpi::OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CThostFtdcReqTransferField TransferField;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    if(!pReqTransfer)
    {
        memset(&TransferField,0,sizeof(TransferField));
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        TransferField=*pReqTransfer;
    }

    DataRspFromFutureToBankByFuture tmp(nRequestID,bIsLast,TransferField,RspInfo);

    m_RefPackageCache.SaveDataTo(BID_RspFromFutureToBankByFuture, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspFromFutureToBankByFuture(�ڻ������ڻ��ʽ�ת����Ӧ��) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        pReqTransfer=& tmp.TransferField;
        //LOG_INFO("OnRspFromFutureToBankByFuture(�ڻ������ڻ��ʽ�ת����Ӧ��) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
        //                "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
        //                "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
        //                "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
        //                "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[%s],\n"
        //                "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
        //                "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
        //                "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
        //                "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
        //                "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d]",
        //                tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
        //                pReqTransfer->TradeCode, pReqTransfer->BankID,pReqTransfer->BankBranchID,pReqTransfer->BrokerID,pReqTransfer->BrokerBranchID,
        //                pReqTransfer->TradeDate, pReqTransfer->TradeTime,pReqTransfer->BankSerial,pReqTransfer->TradingDay,pReqTransfer->PlateSerial,
        //                pReqTransfer->LastFragment, pReqTransfer->SessionID,pReqTransfer->CustomerName,pReqTransfer->IdCardType,pReqTransfer->IdentifiedCardNo,
        //                pReqTransfer->CustType, pReqTransfer->BankAccount,pReqTransfer->BankPassWord,pReqTransfer->AccountID,pReqTransfer->Password,
        //                pReqTransfer->InstallID,pReqTransfer->FutureSerial, pReqTransfer->UserID,pReqTransfer->VerifyCertNoFlag,pReqTransfer->CurrencyID,
        //                pReqTransfer->TradeAmount,pReqTransfer->FutureFetchAmount, pReqTransfer->FeePayFlag,pReqTransfer->CustFee,pReqTransfer->BrokerFee,
        //                pReqTransfer->Message, pReqTransfer->Digest, pReqTransfer->BankAccType,pReqTransfer->DeviceID,pReqTransfer->BankSecuAccType,
        //                pReqTransfer->BrokerIDByBank, pReqTransfer->BankSecuAcc, pReqTransfer->BankPwdFlag,pReqTransfer->SecuPwdFlag,pReqTransfer->OperNo,
        //                pReqTransfer->RequestID, pReqTransfer->TID, pReqTransfer->TransferStatus
        //                );
    }

}

///�ڻ������ѯ�������Ӧ��
void CTraderSpi::OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CThostFtdcReqQueryAccountField QueryAccountField;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    if(!pReqQueryAccount)
    {
        memset(&QueryAccountField,0,sizeof(QueryAccountField));
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        QueryAccountField=*pReqQueryAccount;
    }

    DataRspQueryBankAccountMoneyByFuture tmp(nRequestID,bIsLast,QueryAccountField,RspInfo);

    if(bIsLast)  CQryQueue::ClearbInQry();

    m_RefPackageCache.SaveDataTo(BID_RspQueryBankAccountMoneyByFuture, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQueryBankAccountMoneyByFuture(�ڻ������ѯ�������Ӧ��) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        pReqQueryAccount=& tmp.QueryAccountField;
        //LOG_INFO("OnRspQueryBankAccountMoneyByFuture(�ڻ������ѯ�������Ӧ��) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
        //                "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
        //                "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
        //                "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
        //                "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[%s],\n"
        //                "\t\t\t FutureSerial=[%d],\t InstallID=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
        //                "\t\t\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],BrokerIDByBank=[%s],\n"
        //                "\t\t\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],RequestID=[%d],\n"
        //                "\t\t\t TID=[%d]",
        //                tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
        //                pReqQueryAccount->TradeCode, pReqQueryAccount->BankID,pReqQueryAccount->BankBranchID,pReqQueryAccount->BrokerID,pReqQueryAccount->BrokerBranchID,
        //                pReqQueryAccount->TradeDate, pReqQueryAccount->TradeTime,pReqQueryAccount->BankSerial,pReqQueryAccount->TradingDay,pReqQueryAccount->PlateSerial,
        //                pReqQueryAccount->LastFragment, pReqQueryAccount->SessionID,pReqQueryAccount->CustomerName,pReqQueryAccount->IdCardType,pReqQueryAccount->IdentifiedCardNo,
        //                pReqQueryAccount->CustType, pReqQueryAccount->BankAccount,pReqQueryAccount->BankPassWord,pReqQueryAccount->AccountID,pReqQueryAccount->Password,
        //                pReqQueryAccount->FutureSerial, pReqQueryAccount->InstallID,pReqQueryAccount->UserID,pReqQueryAccount->VerifyCertNoFlag,pReqQueryAccount->CurrencyID,
        //                pReqQueryAccount->Digest, pReqQueryAccount->BankAccType,pReqQueryAccount->DeviceID,pReqQueryAccount->BankSecuAccType,pReqQueryAccount->BrokerIDByBank,
        //                pReqQueryAccount->BankSecuAcc, pReqQueryAccount->BankPwdFlag,pReqQueryAccount->SecuPwdFlag,pReqQueryAccount->OperNo,pReqQueryAccount->RequestID,
        //                pReqQueryAccount->TID
        //                );
    }
}



///�����ѯ������Ӧ
void CTraderSpi::OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CThostFtdcExchangeRateField RspDataField;
    CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
    if(!pExchangeRate)
    {
        memset(&RspDataField,0,sizeof(RspDataField));
        if(RspInfo.ErrorID==0)
        {
            RspInfo.ErrorID=CustomErrorID;
            strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        RspDataField=*pExchangeRate;
    }

    DataRspQryExchangeRate tmp(nRequestID,bIsLast,RspDataField,RspInfo);

    if(bIsLast)  CQryQueue::ClearbInQry();

    m_RefPackageCache.SaveDataTo(BID_RspQryExchangeRate, 0, 0, 0, sizeof(tmp), &tmp); 
    
    if(pExchangeRate&&pRspInfo)
    {
        LOG_INFO("OnRspQryExchangeRate(�����ѯ������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t FromCurrencyID=[%s],\t FromCurrencyUnit=[%g],\t ToCurrencyID=[%s],\t ExchangeRate=[%g]",
                        pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID,bIsLast,
                        pExchangeRate->BrokerID,pExchangeRate->FromCurrencyID,pExchangeRate->FromCurrencyUnit,pExchangeRate->ToCurrencyID,pExchangeRate->ExchangeRate);
    }
}
