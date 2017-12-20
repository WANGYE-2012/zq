#include "stdafx.h"

#include "wx/file.h"
#include "wx/textfile.h"

#include <vector>
#include <string>
#include <map>

#include "ServiceProxy.hpp"
#include "PlatFormService.hpp"
#include "QryQueue.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "ReqOnRtnTrade.h"

#pragma warning(disable:4996)

#if 1
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("QryQueue", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif
#define REQQUEUE_DELETECURPACKAGE()\
do{\
    if(bCache)\
        m_pReqQueue->DeleteCurPackage();\
    else if(bCache2)\
        m_pReqQueue2->DeleteCurPackage();\
}while(0)

CQryQueue::CQryQueue()
{
    DWORD threadId;


    m_pWriteLog=new zqWriteLog(LOGMODE_LOCALFILE,"QryQueue.log");

    m_pReqQueue= new CPackageCache(20*1024*1024,3);
    m_pReqQueue2= new CPackageCache(20*1024*1024,3);
    m_LastQryTime=0;
    m_bNeedExit=false;
	m_bDoingInitQry=false;                        //���ڽ��г�ʼ����ѯ����ʱ���ܲ����
	m_bInQry=false;
    m_pPlatFormService=NULL;

    m_hThread=CreateThread(NULL,2*1024*1024,(LPTHREAD_START_ROUTINE)ThreadProc,this,0,&threadId);
}
void CQryQueue::StopThread()
{
	DWORD			ExitCode;
    m_bNeedExit=true;
	if(m_hThread)
	{
		WaitForSingleObject((HANDLE)m_hThread,300);
		if(GetExitCodeThread((HANDLE)m_hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)m_hThread,0);
		CloseHandle(m_hThread);
		m_hThread=NULL;
	}
}
CQryQueue::~CQryQueue(void)
{
    StopThread();
    SAFE_DELETE(m_pReqQueue);
    SAFE_DELETE(m_pReqQueue2);
    LOG_INFO("CQryQueue::~CQryQueue(����CQryQueue����)");
    SAFE_DELETE(m_pWriteLog);
}
//�Ƿ���Ҫ�߳��˳�
bool CQryQueue::IsThreadNeedExit(void)
{
    return m_bNeedExit;
}



//�Ƿ񵽴��ѯʱ�̡�ctp�涨ÿ��ֻ����һ�β�ѯ 
bool CQryQueue::IsQryTime(void)
{
    DWORD CurTickCount=GetTickCount();
    if(!m_bInQry)
    {                                                       //û����;��ѯ
        if(CurTickCount>=m_LastQryTime && CurTickCount-m_LastQryTime > 1000 ||
            CurTickCount<m_LastQryTime && CurTickCount+((DWORD)0xffffffff-m_LastQryTime) > 1000)
            return true;
    }
    else
    {                                                       //����;��ѯ����ʱΪ8s
        if(CurTickCount>=m_LastQryTime && CurTickCount-m_LastQryTime > 8000 ||
            CurTickCount<m_LastQryTime && CurTickCount+((DWORD)0xffffffff-m_LastQryTime) > 8000)
            return true;
    }
    return false;
}

//���²�ѯʱ��
void CQryQueue::UpdateQryTime(void)
{
    m_LastQryTime = GetTickCount();
}





//�����ѯ��Լ
int CQryQueue::ReqQryInstrument(PlatformStru_QryInstrument &QryInstrument, int nRequestID) 
{
    LOG_INFO("CQryQueue::ReqQryInstrument:InstrumentID=%s",QryInstrument.Thost.InstrumentID);

    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryInstrument,0,0,(DWORD)nRequestID,sizeof(QryInstrument),&QryInstrument);

    return 0;
}

//�����ѯ�ʽ��˻�
int CQryQueue::ReqQryTradingAccount(PlatformStru_QryTradingAccount &QryTradingAccount, int nRequestID) 
{
    LOG_INFO("CQryQueue::ReqQryTradingAccount");

    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryTradingAccount,0,0,(DWORD)nRequestID,sizeof(QryTradingAccount),&QryTradingAccount);
    return 0;
}

//�����ѯ�ɽ�
int CQryQueue::ReqQryTrade(PlatformStru_QryTrade &QryTrade, int nRequestID) 
{
    LOG_INFO("CQryQueue::ReqQryTrade");

    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryTrade,0,0,(DWORD)nRequestID,sizeof(QryTrade),&QryTrade);
    return 0;
}

//�����ѯ����
int CQryQueue::ReqQryOrder(PlatformStru_QryOrder &QryOrder, int nRequestID) 
{
    LOG_INFO("CQryQueue::ReqQryOrder");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryOrder,0,0,(DWORD)nRequestID,sizeof(QryOrder),&QryOrder);
    return 0;
}

//�����ѯͶ���ֲ߳���ϸ
int CQryQueue::ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail &QryInvestorPositionDetail, int nRequestID) 
{
    LOG_INFO("CQryQueue::ReqQryInvestorPositionDetail");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryInvestorPositionDetail,0,0,(DWORD)nRequestID,sizeof(QryInvestorPositionDetail),&QryInvestorPositionDetail);
    return 0;
}

//�����ѯͶ���ֲ߳�
int CQryQueue::ReqQryInvestorPosition(PlatformStru_QryInvestorPosition &QryInvestorPosition, int nRequestID) 
{
    LOG_INFO("CQryQueue::ReqQryInvestorPosition");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryInvestorPosition,0,0,(DWORD)nRequestID,sizeof(QryInvestorPosition),&QryInvestorPosition);
    return 0;
}

//�����ѯͶ���߽�����
int CQryQueue::ReqQrySettlementInfo(PlatformStru_QrySettlementInfo &QrySettlementInfo, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQrySettlementInfo");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQrySettlementInfo,0,0,(DWORD)nRequestID,sizeof(QrySettlementInfo),&QrySettlementInfo);
    return 0;
}

//�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
int CQryQueue::ReqQryCFMMCTradingAccountKey(PlatformStru_QryCFMMCTradingAccountKey &QryCFMMCTradingAccountKey, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryCFMMCTradingAccountKey");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryCFMMCTradingAccountKey,0,0,(DWORD)nRequestID,sizeof(QryCFMMCTradingAccountKey),&QryCFMMCTradingAccountKey);
    return 0;
}



//�����ѯ����
int CQryQueue::ReqQryDepthMarketData(PlatformStru_QryDepthMarketData &QryDepthMarketData, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryDepthMarketData");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryDepthMarketData,0,0,(DWORD)nRequestID,sizeof(QryDepthMarketData),&QryDepthMarketData);
    return 0;
}




///�����ѯͶ���ֲ߳���ϸ(���)
int CQryQueue::ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail &pQryInvestorPositionCombineDetail, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryInvestorPositionCombineDetail");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryInvestorPositionCombineDetail,0,0,(DWORD)nRequestID,sizeof(pQryInvestorPositionCombineDetail),&pQryInvestorPositionCombineDetail);
    return 0;
}

///�����ѯ�ͻ�֪ͨ
int CQryQueue::ReqQryNotice(PlatformStru_QryNotice &pQryNotice, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryNotice");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryNotice,0,0,(DWORD)nRequestID,sizeof(pQryNotice),&pQryNotice);
    return 0;
}

///�����ѯ������Ϣȷ��
int CQryQueue::ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm &pQrySettlementInfoConfirm, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQrySettlementInfoConfirm");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQrySettlementInfoConfirm,0,0,(DWORD)nRequestID,sizeof(pQrySettlementInfoConfirm),&pQrySettlementInfoConfirm);
    return 0;
}

///�����ѯת������
int CQryQueue::ReqQryTransferBank(PlatformStru_QryTransferBank &pQryTransferBank, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryTransferBank");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryTransferBank,0,0,(DWORD)nRequestID,sizeof(pQryTransferBank),&pQryTransferBank);
    return 0;
}

///�����ѯת����ˮ
int CQryQueue::ReqQryTransferSerial(PlatformStru_QryTransferSerial &pQryTransferSerial, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryTransferSerial");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryTransferSerial,0,0,(DWORD)nRequestID,sizeof(pQryTransferSerial),&pQryTransferSerial);
    return 0;
}

///�����ѯǩԼ����
int CQryQueue::ReqQryContractBank(PlatformStru_QryContractBank &pQryContractBank, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryContractBank");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryContractBank,0,0,(DWORD)nRequestID,sizeof(pQryContractBank),&pQryContractBank);
    return 0;
}
    
///�����ѯ����ǩԼ��ϵ
int CQryQueue::ReqQryAccountregister(PlatformStru_QryAccountRegister &pQryAccountregister, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryAccountregister");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryAccountregister,0,0,(DWORD)nRequestID,sizeof(pQryAccountregister),&pQryAccountregister);
    return 0;
}

///�����ѯԤ��
int CQryQueue::ReqQryParkedOrder(PlatformStru_QryParkedOrder &pQryParkedOrder, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryParkedOrder");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryParkedOrder,0,0,(DWORD)nRequestID,sizeof(pQryParkedOrder),&pQryParkedOrder);
    return 0;
}

///�����ѯԤ�񳷵�
int CQryQueue::ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction &pQryParkedOrderAction, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryParkedOrderAction");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryParkedOrderAction,0,0,(DWORD)nRequestID,sizeof(pQryParkedOrderAction),&pQryParkedOrderAction);
    return 0;
}

///�ڻ������ѯ�����������
int CQryQueue::ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount &pReqQueryAccount, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQueryBankAccountMoneyByFuture");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQueryBankAccountMoneyByFuture,0,0,(DWORD)nRequestID,sizeof(pReqQueryAccount),&pReqQueryAccount);
    return 0;
}





//�����ѯ��Լ��������
int CQryQueue::ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField &QryInstrumentCommissionRate, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryInstrumentCommissionRate : InstrumentID=%s",QryInstrumentCommissionRate.InstrumentID);
    if(m_pReqQueue2)
        m_pReqQueue2->SaveDataTo(QryID_ReqQryInstrumentCommissionRate,0,0,(DWORD)nRequestID,sizeof(QryInstrumentCommissionRate),&QryInstrumentCommissionRate);
    return 0;
}

//�����ѯ��Լ��֤����
int CQryQueue::ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField &QryInstrumentMarginRate, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryInstrumentMarginRate : InstrumentID=%s",QryInstrumentMarginRate.InstrumentID);
    if(m_pReqQueue2)
        m_pReqQueue2->SaveDataTo(QryID_ReqQryInstrumentMarginRate,0,0,(DWORD)nRequestID,sizeof(QryInstrumentMarginRate),&QryInstrumentMarginRate);
    return 0;
}
bool CQryQueue::TradeStatusIsConnected(void)
{
	if(m_pPlatFormService==NULL) return false;
	return m_pPlatFormService->GetTradeStatus()==CPlatFormParam::CONNSTATUS_Connected;
}

//�̺߳���
DWORD CQryQueue::ThreadProc(void *arg)
{
    CQryQueue   *pQryQueue=(CQryQueue*)arg;
    int ReqRlt;

    while(!pQryQueue->IsThreadNeedExit())
    {
        if(!pQryQueue->TradeStatusIsConnected())
        {
            Sleep(200);
            continue;
        }
        else if( !pQryQueue->IsQryTime())
        {
            Sleep(50);
            continue;
        }
        else 
        {
            if(pQryQueue->SendQryCmd(ReqRlt))
            {
                pQryQueue->UpdateQryTime();
                Sleep(1000);
            }
            else
            {
                if(pQryQueue->IsCacheEmpty()&&pQryQueue->IsCache2Empty())
                    Sleep(100);
                else
                {
                    if(ReqRlt<0)
                        Sleep(200);
                }
            }
        }
        if(pQryQueue->IsCacheEmpty())
        {
			if ( pQryQueue->m_pPlatFormService )
			{
				pQryQueue->m_pPlatFormService->CheckOnTime();
			}
        }
    }
    return 0;
}




//���ReqChache�Ƿ�Ϊ��
bool CQryQueue::IsCacheEmpty(void)
{
    if(m_pReqQueue && !m_pReqQueue->IsEmpty()) return false;
    else return true;
}
bool CQryQueue::IsCache2Empty(void)
{
    if(m_pReqQueue2 && !m_pReqQueue2->IsEmpty()) return false;
    else return true;
}
//���Ͳ�ѯ���󣬷���ֵ��ʾ�Ƿ�ɹ���������
//ReqRlt��ʾ��������˲�ѯ����ѯ����ֵ��<0��ʾ��ѯʧ��
bool CQryQueue::SendQryCmd(int& ReqRlt)
{
    PackageHeadStru PackageHead;
    bool bDataErr;
    bool bDataIngore=false;

     
    memset(&PackageHead,0,sizeof(PackageHead));
    ReqRlt=0;
    bool bCache=!IsCacheEmpty();
    bool bCache2=!IsCache2Empty();

    if(m_bDoingInitQry) 
        bCache2=false;              //������ڳ�ʼ����ѯ���Ͳ�Ҫ������ˡ��ȳ�ʼ����ѯ�������ٲ顣

    if(bCache||bCache2)
    {
        if(bCache)
        {
            if(m_pReqQueue->GetCurPackageDataLen()>sizeof(m_SendQryCmdBuf))
            {
                m_pReqQueue->DeleteCurPackage();
                return false;
            }
            m_pReqQueue->PeekFrom(&PackageHead,m_SendQryCmdBuf,sizeof(m_SendQryCmdBuf));
        }
        else if(bCache2)
        {
            if(m_pReqQueue2->GetCurPackageDataLen()>sizeof(m_SendQryCmdBuf))
            {
                m_pReqQueue2->DeleteCurPackage();
                return false;
            }
            m_pReqQueue2->PeekFrom(&PackageHead,m_SendQryCmdBuf,sizeof(m_SendQryCmdBuf));
        }


//        ReqRlt=-999;
        bDataErr=false;
        bDataIngore=false;
        switch(PackageHead.CmdID)
        {
            case QryID_ReqQryInstrument:                    //��ѯ��Լ��Ϣ
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryInstrument))  bDataErr=true;
                else 
                    ReqRlt=m_pPlatFormService->ReqQryInstrument((PlatformStru_QryInstrument*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ��Լ��Ϣ)ReqQryInstrument : InstrumentID=%s Rlt=%d",((PlatformStru_QryInstrument*)m_SendQryCmdBuf)->Thost.InstrumentID,ReqRlt);
                break;

            case QryID_ReqQryTradingAccount:                //��ѯ�ʽ�
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryTradingAccount))  bDataErr=true;
                else 
                    ReqRlt=m_pPlatFormService->ReqQryTradingAccount((PlatformStru_QryTradingAccount*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ʽ���Ϣ)ReqQryTradingAccount Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryTrade:                         //��ѯ�ɽ�
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryTrade))  bDataErr=true;
                else 
                    ReqRlt=m_pPlatFormService->ReqQryTrade((PlatformStru_QryTrade*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ɽ���Ϣ)ReqQryTrade Rlt=%d ",ReqRlt);
                break;

            case QryID_ReqQryOrder:                         //��ѯ����
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryOrder))  bDataErr=true;
                else 
                    ReqRlt=m_pPlatFormService->ReqQryOrder((PlatformStru_QryOrder*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ������Ϣ)QryID_ReqQryOrder Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryInvestorPositionDetail:        //��ѯ�ֲ���ϸ
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryInvestorPositionDetail))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryInvestorPositionDetail((PlatformStru_QryInvestorPositionDetail*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ֲ���ϸ)ReqQryInvestorPositionDetail Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryInvestorPosition:              //��ѯ�ֲֻ���
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryInvestorPosition))  bDataErr=true;
                else 
                    ReqRlt=m_pPlatFormService->ReqQryInvestorPosition((PlatformStru_QryInvestorPosition*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ֲֻ���)ReqQryInvestorPosition Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQrySettlementInfo:              //��ѯͶ���߽�����
                if(PackageHead.DataLen!=sizeof(PlatformStru_QrySettlementInfo))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQrySettlementInfo((PlatformStru_QrySettlementInfo*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯͶ���߽�����)ReqQrySettlementInfo Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryCFMMCTradingAccountKey:        //�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryCFMMCTradingAccountKey))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryCFMMCTradingAccountKey((PlatformStru_QryCFMMCTradingAccountKey*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ)ReqQrySettlementInfo Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryDepthMarketData:              //��ѯ������
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryDepthMarketData))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryDepthMarketData((PlatformStru_QryDepthMarketData*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ����)ReqQryDepthMarketData Rlt=%d",ReqRlt);
                break;




            case QryID_ReqQryInvestorPositionCombineDetail:              //�����ѯͶ���ֲ߳���ϸ(���)
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryInvestorPositionCombineDetail))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryInvestorPositionCombineDetail((PlatformStru_QryInvestorPositionCombineDetail*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯͶ���ֲ߳���ϸ(���))ReqQryInvestorPositionCombineDetail Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryNotice:              //�����ѯ�ͻ�֪ͨ
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryNotice))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryNotice((PlatformStru_QryNotice*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ͻ�֪ͨ)ReqQryNotice Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQrySettlementInfoConfirm:              //�����ѯ������Ϣȷ��
                if(PackageHead.DataLen!=sizeof(PlatformStru_QrySettlementInfoConfirm))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQrySettlementInfoConfirm((PlatformStru_QrySettlementInfoConfirm*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ������Ϣȷ��)ReqQrySettlementInfoConfirm Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryTransferBank:              //�����ѯת������
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryTransferBank))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryTransferBank((PlatformStru_QryTransferBank*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯת������)ReqQryTransferBank Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryTransferSerial:              //�����ѯת����ˮ
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryTransferSerial))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryTransferSerial((PlatformStru_QryTransferSerial*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯת����ˮ)ReqQryTransferSerial Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryContractBank:              //�����ѯǩԼ����
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryContractBank))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryContractBank((PlatformStru_QryContractBank*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯǩԼ����)ReqQryContractBank Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryAccountregister:              //�����ѯ����ǩԼ��ϵ
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryAccountRegister))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryAccountregister((PlatformStru_QryAccountRegister*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ����ǩԼ��ϵ)ReqQryAccountregister Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryParkedOrder:              //�����ѯԤ��
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryParkedOrder))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryParkedOrder((PlatformStru_QryParkedOrder*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯԤ��)ReqQryParkedOrder Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryParkedOrderAction:              //�����ѯԤ�񳷵�
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryParkedOrderAction))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryParkedOrderAction((PlatformStru_QryParkedOrderAction*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯԤ�񳷵�)ReqQryParkedOrderAction Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQueryBankAccountMoneyByFuture:              //��ѯ�������
                if(PackageHead.DataLen!=sizeof(PlatformStru_ReqQueryAccount))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQueryBankAccountMoneyByFuture((PlatformStru_ReqQueryAccount*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�������)ReqQueryBankAccountMoneyByFuture Rlt=%d",ReqRlt);
                break;



            case QryID_ReqQryInstrumentCommissionRate:      //��ѯ��Լ��������

                if(PackageHead.DataLen!=sizeof(CThostFtdcQryInstrumentCommissionRateField))  
                {
                    bDataErr=true;
                }
                else
                {
                    PlatformStru_InstrumentCommissionRate CommissionRate;
					int ErrNo=m_pPlatFormService->GetCommissionRate(((CThostFtdcQryInstrumentCommissionRateField*)m_SendQryCmdBuf)->InstrumentID, CommissionRate);
                    if(ErrNo==0||ErrNo==CustomErrorID)
                    {
//                        ReqRlt=-1;                          //�Ѿ����ˣ����ò���
                        bDataIngore=true;
                    }
                    else if(((CThostFtdcQryInstrumentCommissionRateField*)m_SendQryCmdBuf)->InstrumentID[0]==0)
                    {
                        bDataIngore=true;
                    }
                    else 
                    {
                        DWORD CurTickCount=GetTickCount();
                        std::map<std::string,unsigned int>::iterator it;
                        std::map<std::string,int>::iterator it_count;
                        std::string strInstrumentID=std::string(((CThostFtdcQryInstrumentCommissionRateField*)m_SendQryCmdBuf)->InstrumentID);
                        it=m_QryCommissionRateTime.find(strInstrumentID);
                        it_count=m_QryCommissionRateCount.find(strInstrumentID);
                        if(it!=m_QryCommissionRateTime.end()&&
                            (CurTickCount>=it->second&&CurTickCount-it->second<60000||
                             CurTickCount<it->second&&0xffffffff-it->second+CurTickCount<60000))
                        {                                   //ͬһ����Լ����ѯһ�κ󣬱����60s���ٴβ�ѯ
                            REQQUEUE_DELETECURPACKAGE();
                        }
                        else
                        {
                            m_pPlatFormService->SetReqCommissionRateInstrumentID(std::string(((CThostFtdcQryInstrumentCommissionRateField*)m_SendQryCmdBuf)->InstrumentID));
                            ReqRlt=m_pPlatFormService->ReqQryInstrumentCommissionRate((CThostFtdcQryInstrumentCommissionRateField*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                            LOG_INFO("CQryQueue::SendQryCmd : (��ѯ��Լ��������)ReqQryInstrumentCommissionRate��InstrumentID=%s Rlt=%d",((CThostFtdcQryInstrumentCommissionRateField*)m_SendQryCmdBuf)->InstrumentID,ReqRlt);
                            if(ReqRlt==0)
                            {
                                m_QryCommissionRateTime[strInstrumentID]=GetTickCount();
                                if(it_count==m_QryCommissionRateCount.end())
                                    m_QryCommissionRateCount[strInstrumentID]=1;
                                else
                                    it_count->second=it_count->second+1;

                            }
                        }
                    }
                }
                break;

            case QryID_ReqQryInstrumentMarginRate:          //�����ѯ��Լ��֤����
                if(PackageHead.DataLen!=sizeof(CThostFtdcQryInstrumentMarginRateField))  
                {
                    bDataErr=true;
                }
                else
                {
                    PlatformStru_InstrumentMarginRate MarginRate;
					int ErrNo=m_pPlatFormService->GetMarginRate(((CThostFtdcQryInstrumentMarginRateField*)m_SendQryCmdBuf)->InstrumentID, MarginRate);
                    if(ErrNo==0||ErrNo==CustomErrorID)
                    {
//                        ReqRlt=-1;                          //�Ѿ����ˣ����ò���
                        bDataIngore=true;
                    }
                    else if(((CThostFtdcQryInstrumentMarginRateField*)m_SendQryCmdBuf)->InstrumentID[0]==0)
                    {
                        bDataIngore=true;
                    }
                    else 
                    {
                        DWORD CurTickCount=GetTickCount();
                        std::map<std::string,unsigned int>::iterator it;
                        std::map<std::string,int>::iterator it_count;
                        std::string strInstrumentID;
                        strInstrumentID=std::string(((CThostFtdcQryInstrumentMarginRateField*)m_SendQryCmdBuf)->InstrumentID);
                        it=m_QryMarginRateTime.find(strInstrumentID);
                        it_count=m_QryMarginRateCount.find(strInstrumentID);
                        if(it!=m_QryMarginRateTime.end()&&
                            (CurTickCount>=it->second&&CurTickCount-it->second<60000||
                             CurTickCount<it->second&&0xffffffff-it->second+CurTickCount<60000))
                        {                                   //ͬһ����Լ����ѯһ�κ󣬱����60s���ٴβ�ѯ
                            REQQUEUE_DELETECURPACKAGE();
                        }
                        else
                        {
                            ReqRlt=m_pPlatFormService->ReqQryInstrumentMarginRate((CThostFtdcQryInstrumentMarginRateField*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                            LOG_INFO("CQryQueue::SendQryCmd : (��ѯ��Լ��֤����)ReqQryInstrumentMarginRate��InstrumentID=%s Rlt=%d",((CThostFtdcQryInstrumentMarginRateField*)m_SendQryCmdBuf)->InstrumentID,ReqRlt);
                            if(ReqRlt==0)
                            {
                                m_QryMarginRateTime[strInstrumentID]=GetTickCount();
                                if(it_count==m_QryMarginRateCount.end())
                                    m_QryMarginRateCount[strInstrumentID]=1;
                                else
                                    it_count->second=it_count->second+1;
                            }
                        }
                    }
                }
                break;

        }
        if(bDataIngore)
        {
            REQQUEUE_DELETECURPACKAGE();
            return false;
        }
        else if(bDataErr)
        {
            REQQUEUE_DELETECURPACKAGE();
            return false;
        }
        else if(ReqRlt==0) 
        { 
            m_bInQry=true;
            REQQUEUE_DELETECURPACKAGE();
            return true; 
        }                                                               
        else 
        {
            return false;         
        }
    }
    else
    {                                                       //���Ͷ�����û�д����Ͳ�ѯ������ִ���������ʺͱ�֤���ʵĲ�ѯ
       if(m_pPlatFormService->IsNeedCheckCommissionRateAndMarginRate())
       {
            std::set<std::string>::iterator it_InstrumentID;
            std::set<std::string> AllInstruments;
            PlatformStru_InstrumentInfo InstrumentInfo;
            PlatformStru_InstrumentCommissionRate CommissionRate;
            PlatformStru_InstrumentMarginRate MarginRate;
            DWORD CurTickCount=GetTickCount();
            bool bGetAllRate=true;

            m_pPlatFormService->GetInstrumentList(AllInstruments);
            it_InstrumentID=AllInstruments.begin();

            for(;it_InstrumentID!=AllInstruments.end();++it_InstrumentID)
            {
                if(it_InstrumentID->empty()) continue;
                if(m_pPlatFormService->GetInstrumentInfo(*it_InstrumentID,InstrumentInfo)<0) continue;
                if(InstrumentInfo.ProductClass!=THOST_FTDC_PC_Futures) continue;    //ֻ��ѯ�ڻ��ķ���
                if(!m_pPlatFormService) return false;

                                                            //������ÿ����Լ�Ƿ��Ѿ��з��ʣ����û�У����ʱ�ᴥ��һ�η��ʵĲ�ѯ
                if( m_pPlatFormService->GetCommissionRate(*it_InstrumentID,CommissionRate)<0)
                {
                    std::map<std::string,unsigned int>::iterator it;
                    std::map<std::string,int>::iterator it_count;
                    it=m_QryCommissionRateTime.find(*it_InstrumentID);
                    it_count=m_QryCommissionRateCount.find(*it_InstrumentID);
                    if((it==m_QryCommissionRateTime.end()||
                        CurTickCount>=it->second&&CurTickCount-it->second>60000||
                        CurTickCount<it->second&&0xffffffff-it->second+CurTickCount>60000)&&
                        (it_count==m_QryCommissionRateCount.end()||
                         it_count->second<=3))
					{
						//�����ǰû�и÷��ʣ����һ������ѯ������
						CThostFtdcQryInstrumentCommissionRateField ReqData;
						memset(&ReqData,0,sizeof(ReqData));
						strncpy(ReqData.BrokerID,m_pPlatFormService->GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
						strncpy(ReqData.InvestorID,m_pPlatFormService->GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);;
						strncpy(ReqData.InstrumentID,it_InstrumentID->c_str(),sizeof(ReqData.InstrumentID)-1);;

						ReqQryInstrumentCommissionRate(ReqData,0);
						return false;
					}
                    if(bGetAllRate) bGetAllRate=false;
                }

                if( m_pPlatFormService->GetMarginRate(*it_InstrumentID,MarginRate)<0)
                {
                    std::map<std::string,unsigned int>::iterator it;
                    std::map<std::string,int>::iterator it_count;
                    it=m_QryMarginRateTime.find(*it_InstrumentID);
                    it_count=m_QryMarginRateCount.find(*it_InstrumentID);
                    if((it==m_QryMarginRateTime.end()||
                        CurTickCount>=it->second&&CurTickCount-it->second>60000||
                        CurTickCount<it->second&&0xffffffff-it->second+CurTickCount>60000)&&
                        (it_count==m_QryMarginRateCount.end()||
                         it_count->second<=3))
					{                                                  //�����ǰû�и÷��ʣ����һ������ѯ������
						CThostFtdcQryInstrumentMarginRateField ReqData;
						memset(&ReqData,0,sizeof(ReqData));
						strncpy(ReqData.BrokerID,m_pPlatFormService->GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
						strncpy(ReqData.InvestorID,m_pPlatFormService->GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);
						strncpy(ReqData.InstrumentID,it_InstrumentID->c_str(),sizeof(ReqData.InstrumentID)-1);
						ReqData.HedgeFlag=THOST_FTDC_HF_Speculation;

						ReqQryInstrumentMarginRate(ReqData,0);

						return false;
					}
                    if(bGetAllRate) bGetAllRate=false;
                }

            }
            if(it_InstrumentID==AllInstruments.end()&&bGetAllRate)
            {
                                                            //��Ϊ���к�Լ�ķ��ʶ��Ѿ����ˣ�������ı�־
                m_pPlatFormService->SetNeedCheckCommissionRateAndMarginRate(false);
                return false;
            }
        }
    }
    return false;
}
