#include "stdafx.h"

#include "wx/file.h"
#include "wx/textfile.h"

#include <vector>
#include <string>
#include <map>

#include "ServiceProxy.hpp"
#include "PlatFormService.hpp"
#include "QryQueue.h"
#include "Module-Misc2/SimpleWriteLog.h"
#include "ReqOnRtnTrade.h"

#pragma warning(disable:4996)

bool CQryQueue::m_bInQry=false;
//bool CQryQueue::m_bDoingInitQry=false;                        //���ڽ��г�ʼ����ѯ����ʱ���ܲ����



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

    CReqOnRtnTrade::Init();

    m_pWriteLog=new zqWriteLog(LOGMODE_LOCALFILE,"QryQueue.log");

	if(!IS_MULTIACCOUNT_VERSION)
	{
		m_pReqQueue= new CPackageCache(20*1024*1024,3);
		m_pReqQueue2= new CPackageCache(20*1024*1024,3);
	}
	else
	{
		m_pReqQueue= new CPackageCache(10*1024*1024,3);
		m_pReqQueue2= new CPackageCache(10*1024*1024,3);
	}
    m_LastQryTime=0;
    m_bNeedExit=false;

    m_pPlatFormService=NULL;

    m_hThread=CreateThread(NULL,2*1024*1024,(LPTHREAD_START_ROUTINE)ThreadProc,this,0,&threadId);
}
void CQryQueue::StopThread()
{
	DWORD			ExitCode;
    m_bNeedExit=true;
	if(m_hThread)
	{
		WaitForSingleObject((HANDLE)m_hThread,2000);
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

    CReqOnRtnTrade::Release();

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
#if 1
        if(CurTickCount>=m_LastQryTime && CurTickCount-m_LastQryTime > 8000 ||
            CurTickCount<m_LastQryTime && CurTickCount+((DWORD)0xffffffff-m_LastQryTime) > 8000)
            return true;
#else
        if(CurTickCount>=m_LastQryTime && CurTickCount-m_LastQryTime > 1000 ||
            CurTickCount<m_LastQryTime && CurTickCount+((DWORD)0xffffffff-m_LastQryTime) > 1000)
            return true;
#endif
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

///�����ѯ����
int CQryQueue::ReqQryExchangeRate(PlatformStru_QryExchangeRate &QryExchangeRate, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryExchangeRate");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryExchangeRate,0,0,(DWORD)nRequestID,sizeof(QryExchangeRate),&QryExchangeRate);
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
	return m_pPlatFormService->GetTradeStatus()==CTPCONNSTATUS_Connected;
}

//�����ѯ���������ָ�����͵Ĳ�ѯ��������TCP��������ز�ǰ�Ĵ���
//ReqCmdID:1:�ʽ��ѯ��2:�ɽ���ѯ��4:������ѯ��8:�ֲ���ϸ��ѯ��16:�ֲֲ�ѯ��32:��ϳֲ���ϸ��ѯ
//ReqCmdID���Ի���
void CQryQueue::ClearReqInQryQueue(int ReqCmdID)
{
    LOG_INFO("CQryQueue::ClearReqInQryQueue(%d)",ReqCmdID);
    if(!m_pReqQueue) return;
    if((ReqCmdID&1)==1) m_pReqQueue->ClearCmdIDInPackageHead(QryID_ReqQryTradingAccount);
    if((ReqCmdID&2)==2) m_pReqQueue->ClearCmdIDInPackageHead(QryID_ReqQryTrade);
    if((ReqCmdID&4)==4) m_pReqQueue->ClearCmdIDInPackageHead(QryID_ReqQryOrder);
    if((ReqCmdID&8)==8) m_pReqQueue->ClearCmdIDInPackageHead(QryID_ReqQryInvestorPositionDetail);
    if((ReqCmdID&16)==16) m_pReqQueue->ClearCmdIDInPackageHead(QryID_ReqQryInvestorPosition);
    if((ReqCmdID&32)==32) m_pReqQueue->ClearCmdIDInPackageHead(QryID_ReqQryInvestorPositionCombineDetail);
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
				CReqOnRtnTrade::CheckOnTime( pQryQueue->m_pPlatFormService->m_rServiceProxy);
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
    static char Buf[1*1024*1024];
    PackageHeadStru PackageHead;
    bool bDataErr;
    bool bDataIngore=false;

     
    memset(&PackageHead,0,sizeof(PackageHead));
    ReqRlt=0;
    bool bCache=!IsCacheEmpty();    //��ʾ�Ƿ��Ƿ���Cache�е�����
    bool bCache2=!IsCache2Empty();  //��ʾ�Ƿ��Ƿ���Cache2�е�����

    //if(bCache||m_bDoingInitQry) 
    if(bCache||!m_pPlatFormService->m_rServiceProxy.m_bInitQrySucc) 
        bCache2=false;              //������ڳ�ʼ����ѯ������Cache������������Ͳ�Ҫ������ˡ��ȳ�ʼ����ѯ�������ٲ顣

    if(bCache||bCache2)
    {
        if(bCache)
        {
            if(m_pReqQueue->GetCurPackageDataLen()>sizeof(Buf))
            {
                m_pReqQueue->DeleteCurPackage();
                return false;
            }
            m_pReqQueue->PeekFrom(&PackageHead,Buf,sizeof(Buf));
        }
        else if(bCache2)
        {
            if(m_pReqQueue2->GetCurPackageDataLen()>sizeof(Buf))
            {
                m_pReqQueue2->DeleteCurPackage();
                return false;
            }
            m_pReqQueue2->PeekFrom(&PackageHead,Buf,sizeof(Buf));
        }


//        ReqRlt=-999;
        bDataErr=false;
        bDataIngore=false;
        //��ǰ��m_bInQry��Ϊtrue���������˴���̫�죬�Ѿ��õ��������ִ��m_bInQry=true���Ӷ����³�ʱ�����
        m_bInQry=true;
        switch(PackageHead.CmdID)
        {
            case QryID_ReqQryInstrument:                    //��ѯ��Լ��Ϣ
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryInstrument))  bDataErr=true;
                else 
                    ReqRlt=m_pPlatFormService->ReqQryInstrument((PlatformStru_QryInstrument*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ��Լ��Ϣ)ReqQryInstrument : InstrumentID=%s Rlt=%d",((PlatformStru_QryInstrument*)Buf)->Thost.InstrumentID,ReqRlt);
                break;

            case QryID_ReqQryTradingAccount:                //��ѯ�ʽ�
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryTradingAccount))  bDataErr=true;
                else 
                    ReqRlt=m_pPlatFormService->ReqQryTradingAccount((PlatformStru_QryTradingAccount*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ʽ���Ϣ)ReqQryTradingAccount Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryTrade:                         //��ѯ�ɽ�
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryTrade))  bDataErr=true;
                else 
                    ReqRlt=m_pPlatFormService->ReqQryTrade((PlatformStru_QryTrade*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ɽ���Ϣ)ReqQryTrade Rlt=%d ",ReqRlt);
                break;

            case QryID_ReqQryOrder:                         //��ѯ����
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryOrder))  bDataErr=true;
                else 
                    ReqRlt=m_pPlatFormService->ReqQryOrder((PlatformStru_QryOrder*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ������Ϣ)QryID_ReqQryOrder Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryInvestorPositionDetail:        //��ѯ�ֲ���ϸ
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryInvestorPositionDetail))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryInvestorPositionDetail((PlatformStru_QryInvestorPositionDetail*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ֲ���ϸ)ReqQryInvestorPositionDetail Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryInvestorPosition:              //��ѯ�ֲ�
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryInvestorPosition))  bDataErr=true;
                else 
                    ReqRlt=m_pPlatFormService->ReqQryInvestorPosition((PlatformStru_QryInvestorPosition*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ֲֻ���)ReqQryInvestorPosition Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQrySettlementInfo:              //��ѯͶ���߽�����
                if(PackageHead.DataLen!=sizeof(PlatformStru_QrySettlementInfo))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQrySettlementInfo((PlatformStru_QrySettlementInfo*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯͶ���߽�����)ReqQrySettlementInfo Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryCFMMCTradingAccountKey:        //�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryCFMMCTradingAccountKey))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryCFMMCTradingAccountKey((PlatformStru_QryCFMMCTradingAccountKey*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ)ReqQrySettlementInfo Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryDepthMarketData:              //��ѯ������
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryDepthMarketData))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryDepthMarketData((PlatformStru_QryDepthMarketData*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ����)ReqQryDepthMarketData Rlt=%d",ReqRlt);
                break;




            case QryID_ReqQryInvestorPositionCombineDetail:              //�����ѯͶ���ֲ߳���ϸ(���)
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryInvestorPositionCombineDetail))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryInvestorPositionCombineDetail((PlatformStru_QryInvestorPositionCombineDetail*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯͶ���ֲ߳���ϸ(���))ReqQryInvestorPositionCombineDetail Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryNotice:              //�����ѯ�ͻ�֪ͨ
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryNotice))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryNotice((PlatformStru_QryNotice*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ͻ�֪ͨ)ReqQryNotice Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQrySettlementInfoConfirm:              //�����ѯ������Ϣȷ��
                if(PackageHead.DataLen!=sizeof(PlatformStru_QrySettlementInfoConfirm))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQrySettlementInfoConfirm((PlatformStru_QrySettlementInfoConfirm*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ������Ϣȷ��)ReqQrySettlementInfoConfirm Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryTransferBank:              //�����ѯת������
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryTransferBank))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryTransferBank((PlatformStru_QryTransferBank*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯת������)ReqQryTransferBank Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryTransferSerial:              //�����ѯת����ˮ
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryTransferSerial))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryTransferSerial((PlatformStru_QryTransferSerial*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯת����ˮ)ReqQryTransferSerial Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryContractBank:              //�����ѯǩԼ����
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryContractBank))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryContractBank((PlatformStru_QryContractBank*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯǩԼ����)ReqQryContractBank Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryAccountregister:              //�����ѯ����ǩԼ��ϵ
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryAccountRegister))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryAccountregister((PlatformStru_QryAccountRegister*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ����ǩԼ��ϵ)ReqQryAccountregister Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryExchangeRate:                  //�����ѯ����
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryExchangeRate))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryExchangeRate((PlatformStru_QryExchangeRate*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ����)ReqQryExchangeRate Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryParkedOrder:              //�����ѯԤ��
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryParkedOrder))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryParkedOrder((PlatformStru_QryParkedOrder*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯԤ��)ReqQryParkedOrder Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryParkedOrderAction:              //�����ѯԤ�񳷵�
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryParkedOrderAction))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryParkedOrderAction((PlatformStru_QryParkedOrderAction*)Buf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯԤ�񳷵�)ReqQryParkedOrderAction Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQueryBankAccountMoneyByFuture:              //��ѯ�������
                if(PackageHead.DataLen!=sizeof(PlatformStru_ReqQueryAccount))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQueryBankAccountMoneyByFuture((PlatformStru_ReqQueryAccount*)Buf,(int)PackageHead.dwSeq);
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
					int ErrNo=m_pPlatFormService->GetCommissionRate(((CThostFtdcQryInstrumentCommissionRateField*)Buf)->InstrumentID, CommissionRate);
                    if(ErrNo==0||ErrNo==CustomErrorID)
                    {
//                        ReqRlt=-1;                          //�Ѿ����ˣ����ò���
                        bDataIngore=true;
                    }
                    else if(((CThostFtdcQryInstrumentCommissionRateField*)Buf)->InstrumentID[0]==0)
                    {
                        bDataIngore=true;
                    }
                    else 
                    {
                        DWORD CurTickCount=GetTickCount();
                        std::map<std::string,unsigned int>::iterator it;
                        std::map<std::string,int>::iterator it_count;
                        std::string strInstrumentID=std::string(((CThostFtdcQryInstrumentCommissionRateField*)Buf)->InstrumentID);
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
                            m_pPlatFormService->SetReqCommissionRateInstrumentID(std::string(((CThostFtdcQryInstrumentCommissionRateField*)Buf)->InstrumentID));
                            ReqRlt=m_pPlatFormService->ReqQryInstrumentCommissionRate((CThostFtdcQryInstrumentCommissionRateField*)Buf,(int)PackageHead.dwSeq);
                            LOG_INFO("CQryQueue::SendQryCmd : (��ѯ��Լ��������)ReqQryInstrumentCommissionRate��InstrumentID=%s Rlt=%d",((CThostFtdcQryInstrumentCommissionRateField*)Buf)->InstrumentID,ReqRlt);
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
					int ErrNo=m_pPlatFormService->GetMarginRate(((CThostFtdcQryInstrumentMarginRateField*)Buf)->InstrumentID, MarginRate);
                    if(ErrNo==0||ErrNo==CustomErrorID)
                    {
//                        ReqRlt=-1;                          //�Ѿ����ˣ����ò���
                        bDataIngore=true;
                    }
                    else if(((CThostFtdcQryInstrumentMarginRateField*)Buf)->InstrumentID[0]==0)
                    {
                        bDataIngore=true;
                    }
                    else 
                    {
                        DWORD CurTickCount=GetTickCount();
                        std::map<std::string,unsigned int>::iterator it;
                        std::map<std::string,int>::iterator it_count;
                        std::string strInstrumentID;
                        strInstrumentID=std::string(((CThostFtdcQryInstrumentMarginRateField*)Buf)->InstrumentID);
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
                            ReqRlt=m_pPlatFormService->ReqQryInstrumentMarginRate((CThostFtdcQryInstrumentMarginRateField*)Buf,(int)PackageHead.dwSeq);
                            LOG_INFO("CQryQueue::SendQryCmd : (��ѯ��Լ��֤����)ReqQryInstrumentMarginRate��InstrumentID=%s Rlt=%d",((CThostFtdcQryInstrumentMarginRateField*)Buf)->InstrumentID,ReqRlt);
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
            default:
                ReqRlt=-999;
                break;

        }

        if(bDataIngore)
        {
            REQQUEUE_DELETECURPACKAGE();
            m_bInQry=false;
            return false;
        }
        else if(bDataErr)
        {
            REQQUEUE_DELETECURPACKAGE();
            m_bInQry=false;
            return false;
        }
        else if(ReqRlt==0) 
        { 
            //�ɹ�������ѯ����
            REQQUEUE_DELETECURPACKAGE();
            return true; 
        }                                                               
        else if(ReqRlt==-2)
        {
            //��������ʧ�ܣ�δ�������󳬹��������Ҳ������һ�ε�����û�����ꡣ
            //����m_bInQry��������true���Ը��·��������ʱ�䣬����Ƶ������
            return true; 
        }
        else
        {
            m_bInQry=false;
            return false;         
        }
    }
    //else if(!m_bDoingInitQry)
    else if(m_pPlatFormService->m_rServiceProxy.m_bInitQrySucc)
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
