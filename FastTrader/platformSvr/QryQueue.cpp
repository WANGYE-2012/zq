#include "stdafx.h"

#include "wx/file.h"
#include "wx/textfile.h"

#include <set>
#include <vector>
#include <string>
#include <map>
using std::set;
using std::string;
using std::map;
using std::vector;

#include "ServiceProxy.hpp"
#include "PlatFormService.hpp"
#include "QryQueue.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
#include "ReqOnRtnTrade.h"
#include "../../Miscellaneous/Tools/inc/FileOpr.h"
#include "../inc/Module-Misc2/GlobalConfigMgr.h"

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

CQryQueue::CQryQueue(CPlatFormService& PlatFormService)
:    m_RefPlatFormService(PlatFormService)
    ,m_bStartQryRate(false)
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


    //��SendQryCmd()��Ҫ�õ�1Mջ�ռ�
    m_hThread=CreateThread(NULL,3*1024*1024,(LPTHREAD_START_ROUTINE)ThreadProc,this,0,&threadId);
    if(GlobalConfigManager::m_Test_bLogThreadID)
        CFileOpr::getObj().writelocallog("threadid","TID:\t% 8u\tCQryQueue::ThreadProc",threadId);
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
    if(!m_RefPlatFormService.m_bInQry)
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


//�����ѯ���͹�˾���ײ���
void CQryQueue::ReqQryBrokerTradingParams(int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryBrokerTradingParams");

    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqBrokerTradingParam,0,0,(DWORD)nRequestID,0,NULL);
}


//�����ѯƷ��
int CQryQueue::ReqQryProduct(PlatformStru_QryProduct &QryProduct, int nRequestID) 
{
    LOG_INFO("CQryQueue::ReqQryProduct:ProductID=%s",QryProduct.Thost.ProductID);

    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryProduct,0,0,(DWORD)nRequestID,sizeof(QryProduct),&QryProduct);

    return 0;
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

//�����ѯִ������
int CQryQueue::ReqQryExecOrder(PlatformStru_QryExecOrderField& QryExecOrder, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryExecOrder");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryExecOrder,0,0,(DWORD)nRequestID,sizeof(QryExecOrder),&QryExecOrder);
    return 0;
}

//��ѯ��󱨵���������
int CQryQueue::ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume &QueryMaxOrderVolume, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQueryMaxOrderVolume");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQueryMaxOrderVolume,0,0,(DWORD)nRequestID,sizeof(QueryMaxOrderVolume),&QueryMaxOrderVolume);
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
	return m_RefPlatFormService.GetTradeStatus()==CTPCONNSTATUS_Connected;
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
            if(pQryQueue->SendQryCmd())
            {
                pQryQueue->UpdateQryTime();
                Sleep(1000);
            }
            else
            {
                Sleep(200);
            }
        }
        if(pQryQueue->IsCacheEmpty())
        {
		    CReqOnRtnTrade::CheckOnTime( pQryQueue->m_RefPlatFormService.m_rServiceProxy);
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
//bTrySend�����Ƿ����˷���
//ReqRlt��ʾ��������˲�ѯ����ѯ����ֵ��0:����ɹ���-1:��������ʧ�ܣ�-2:δ�������󳬹��������-3:ÿ�뷢�������������������
bool CQryQueue::SendQryCmd()
{
    char PkgDataBuf[1*1024*1024];              //�߳�ջ�ռ�Ϊ3M�������õ�1M
    PackageHeadStru PackageHead;
    bool bHavePkg=false;

     
    //ReqRlt��ʾ��������˲�ѯ����ѯ����ֵ��0:����ɹ���-1:��������ʧ�ܣ�-2:δ�������󳬹��������-3:ÿ�뷢�������������������-999:δ��ѯ��
    int ReqRlt=-999;
    memset(&PackageHead,0,sizeof(PackageHead));

    //ɾ�����ܴ��ڵ���Ч��
    while(!IsCacheEmpty())
    {
        if(m_pReqQueue->GetCurPackageDataLen()>(int)sizeof(PkgDataBuf)) 
        {
            m_pReqQueue->DeleteCurPackage();
            continue;
        }
        if(!m_pReqQueue->PeekFrom(&PackageHead,PkgDataBuf,sizeof(PkgDataBuf))) break;
        if(!IsReqQryPackageInCacheValid(PackageHead,PkgDataBuf)) 
        {
            m_pReqQueue->DeleteCurPackage();
            continue;
        }
        break;
    }
    while(!IsCache2Empty())
    {
        if(m_pReqQueue2->GetCurPackageDataLen()>(int)sizeof(PkgDataBuf))
        {
            m_pReqQueue2->DeleteCurPackage();
            continue;
        }
        if(!m_pReqQueue2->PeekFrom(&PackageHead,PkgDataBuf,sizeof(PkgDataBuf))) break;
        if(!IsReqQryPackageInCacheValid(PackageHead,PkgDataBuf)) 
        {
            m_pReqQueue2->DeleteCurPackage();
            continue;
        }
        break;
    }

    bool bCache=!IsCacheEmpty();    //��ʾ�Ƿ��Ƿ���Cache�е�����
    bool bCache2=!IsCache2Empty();  //��ʾ�Ƿ��Ƿ���Cache2�е�����

    if(bCache2&&(bCache||!m_RefPlatFormService.m_rServiceProxy.m_bInitQrySucc))
        bCache2=false;              //������ڳ�ʼ����ѯ������Cache������������Ͳ�Ҫ������ˡ��ȳ�ʼ����ѯ�������ٲ顣

    if(bCache||bCache2)
    {
        //���Ͷ������д����͵Ĳ�ѯ����
        (bCache?m_pReqQueue:m_pReqQueue2)->PeekFrom(&PackageHead,PkgDataBuf,sizeof(PkgDataBuf));
        bHavePkg=true;
    }
    else if(m_RefPlatFormService.m_rServiceProxy.m_bInitQrySucc&&m_RefPlatFormService.IsNeedCheckCommissionRateAndMarginRate())
    {                          
        //���Ͷ�����û�д����Ͳ�ѯ������ִ���������ʺͱ�֤���ʵĲ�ѯ
        if(!m_bStartQryRate)
        {
            //��m_bInitQrySucc��false��Ϊtrueʱ����ʼ��ѯ���ʡ�
            m_bStartQryRate=true;

            //��ѯ���ʵĴ�����ͷ4�Σ���InstrumentIDΪ�յķ�ʽ��ѯ�������ʡ���֤���ʡ���Ȩ�������ʡ���Ȩ���׳ɱ����Եõ��гֲֵķ�������
            m_QryRateCount=0;                 

            //ȷ����ѯ˳�򡣽�Ҫ��ѯ�ĺ�Լ��˳��ŵ�m_InstrumentIDsNeedQry��
            m_InstrumentIDsNeedQry.clear();
            m_InstrumentIDsNeedQry2.clear();

            //1. ������InstrumentIDΪ�ղ�4�Σ��õ��гֲֵĺ�Լ�ķ���
            //2. RunMode==1/2: �Ȳ�ѯ�ڻ���Լ���ʣ��ٲ�ѯ��Ȩ��Լ����
            //   RunMode=3: �Ȳ�ѯ������ڻ���Լ���ʣ��ٲ�ѯȡ��Ȩ���ʣ�����Ǳ�����ڻ���Լ����
            //3. ���ĳ����Լ��ѯ�޷��ʣ���ͬƷ�ֵ�������Լ�Ƶ���ѯ���еĺ���ȥ������ٲ顣

            set<string> AllInstruments;
            vector<string> AllFutureInstruments,AllOptionInstruments;
            set<string> UnderlyingInstruments;

            m_RefPlatFormService.GetInstrumentList(AllInstruments);
            if(GlobalConfigManager::m_RunMode==2||GlobalConfigManager::m_RunMode==3)
                m_RefPlatFormService.GetAllOptionUnderlyingInstrumentIDs(UnderlyingInstruments);

            for(set<string>::iterator it_InstrumentID=AllInstruments.begin();it_InstrumentID!=AllInstruments.end();it_InstrumentID++)
            {
                if(it_InstrumentID->empty()) continue;
                char ProductClass=m_RefPlatFormService.GetProductClassType(*it_InstrumentID);
                if(ProductClass==THOST_FTDC_PC_Futures) AllFutureInstruments.push_back(*it_InstrumentID);
                else if(ProductClass==THOST_FTDC_PC_Options||ProductClass==THOST_FTDC_PC_SpotOption) AllOptionInstruments.push_back(*it_InstrumentID);
            }

            //qrystep: ��ѯ���裬0/1/2/3��Ӧ��Ȩ������ڻ���Լ(RunMode3)/��Ȩ��Լ(RunMode3)/�����ڻ���Լ(RunMode123)/��Ȩ��Լ(RunMode12)
            for(int qrystep=0;qrystep<4&&!bHavePkg;qrystep++)
            {
                //��Ȩ������ڻ���Լ
                if(qrystep==0&&GlobalConfigManager::m_RunMode==3)
                {
                    for(vector<string>::iterator it=AllFutureInstruments.begin();it!=AllFutureInstruments.end();)
                    {
                        if(UnderlyingInstruments.find(*it)!=UnderlyingInstruments.end())
                        {
                            m_InstrumentIDsNeedQry.push_back(*it);
                            it=AllFutureInstruments.erase(it);
                        }
                        else it++;
                    }
                }

                //��Ȩ��Լ
                if(qrystep==1&&GlobalConfigManager::m_RunMode==3||qrystep==3&&(GlobalConfigManager::m_RunMode==1||GlobalConfigManager::m_RunMode==2))
                {
                    m_InstrumentIDsNeedQry.insert(m_InstrumentIDsNeedQry.end(),AllOptionInstruments.begin(),AllOptionInstruments.end());
                    AllOptionInstruments.clear();
                }

                //�ڻ���Լ
                if(qrystep==2)
                {
                    m_InstrumentIDsNeedQry.insert(m_InstrumentIDsNeedQry.end(),AllFutureInstruments.begin(),AllFutureInstruments.end());
                    AllFutureInstruments.clear();
                }
            }
        }


        if(m_QryRateCount==0||m_QryRateCount==1)
        {
            //��InstrumentIDΪ�յķ�ʽ��ֲֵ���������/��Ȩ��������
            CThostFtdcQryInstrumentCommissionRateField& ReqData=*((CThostFtdcQryInstrumentCommissionRateField*)PkgDataBuf);
            memset(&ReqData,0,sizeof(ReqData));
            strncpy(ReqData.BrokerID,m_RefPlatFormService.GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
            strncpy(ReqData.InvestorID,m_RefPlatFormService.GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);;

            memset(&PackageHead,0,sizeof(PackageHead));
            PackageHead.CmdID=QryID_ReqQryInstrumentCommissionRate;
            bHavePkg=true;
        }
        else if(m_QryRateCount==2||m_QryRateCount==3)
        {
            //��InstrumentIDΪ�յķ�ʽ��ֲֵı�֤����/��Ȩ���׳ɱ�
            CThostFtdcQryInstrumentMarginRateField& ReqData=*((CThostFtdcQryInstrumentMarginRateField*)PkgDataBuf);
            memset(&ReqData,0,sizeof(ReqData));
            strncpy(ReqData.BrokerID,m_RefPlatFormService.GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
            strncpy(ReqData.InvestorID,m_RefPlatFormService.GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);
            ReqData.HedgeFlag=THOST_FTDC_HF_Speculation;

            memset(&PackageHead,0,sizeof(PackageHead));
            PackageHead.CmdID=QryID_ReqQryInstrumentMarginRate;
            bHavePkg=true;
        }
        else
        {
            map<string,time_t>::iterator it_GetTime;
            map<string,int>::iterator it_GetCount;
            time_t curtime=time(NULL);

            //�Ȳ�m_InstrumentIDsNeedQry��ĺ�Լ�����δ�鵽���ŵ�m_InstrumentIDsNeedQry2�
            for(int step=0;step<2&&!bHavePkg;step++)
            {
                vector<string>& vecInstrumentIDs = step==0 ? m_InstrumentIDsNeedQry : m_InstrumentIDsNeedQry2;
                while(!vecInstrumentIDs.empty())
                {
                    //���ǶԶ��еĵ�һ����Լ���в���

                    string strInstrumentID=vecInstrumentIDs[0];

                    //���û���������ʣ���֮
                    map<string,int>::iterator it_GetCommissionRateCount=m_QryCommissionRateCount.find(strInstrumentID);
                    bool bHaveCommissionRate=m_RefPlatFormService.IsCommissionRateExist(strInstrumentID);
                    if(!bHaveCommissionRate&&(it_GetCommissionRateCount==m_QryCommissionRateCount.end()||it_GetCommissionRateCount->second==0))
                    {
                        CThostFtdcQryInstrumentCommissionRateField& ReqData=*((CThostFtdcQryInstrumentCommissionRateField*)PkgDataBuf);
                        memset(&ReqData,0,sizeof(ReqData));
                        strncpy(ReqData.BrokerID,m_RefPlatFormService.GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
                        strncpy(ReqData.InvestorID,m_RefPlatFormService.GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);;
                        strncpy(ReqData.InstrumentID,strInstrumentID.c_str(),sizeof(ReqData.InstrumentID)-1);

                        memset(&PackageHead,0,sizeof(PackageHead));
                        PackageHead.CmdID=QryID_ReqQryInstrumentCommissionRate;
                        bHavePkg=true;
                        break;
                    }

                    //���û�б�֤���ʣ���֮
                    map<string,int>::iterator it_GetMarginCount=m_QryMarginRateCount.find(strInstrumentID);
                    bool bHaveMarginRate=m_RefPlatFormService.IsMarginRateExist(strInstrumentID);
                    if(!bHaveMarginRate&&(it_GetMarginCount==m_QryMarginRateCount.end()||it_GetMarginCount->second==0))
                    {
                        CThostFtdcQryInstrumentMarginRateField& ReqData=*((CThostFtdcQryInstrumentMarginRateField*)PkgDataBuf);
                        memset(&ReqData,0,sizeof(ReqData));
                        strncpy(ReqData.BrokerID,m_RefPlatFormService.GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
                        strncpy(ReqData.InvestorID,m_RefPlatFormService.GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);
                        strncpy(ReqData.InstrumentID,strInstrumentID.c_str(),sizeof(ReqData.InstrumentID)-1);
                        ReqData.HedgeFlag=THOST_FTDC_HF_Speculation;

                        memset(&PackageHead,0,sizeof(PackageHead));
                        PackageHead.CmdID=QryID_ReqQryInstrumentMarginRate;
                        bHavePkg=true;
                        break;
                    }

                    //���step==0���Ҳ鲻���������ʻ�֤���ʣ���ͬƷ�ֺ�Լ���Ƶ�m_InstrumentIDsNeedQry2��
                    if(step==0&&(!bHaveCommissionRate||!bHaveMarginRate))
                    {
                        string strProductID=m_RefPlatFormService.GetProductID(strInstrumentID);
                        for(vector<string>::iterator it=vecInstrumentIDs.begin();it!=vecInstrumentIDs.end();)
                        {
                            if(m_RefPlatFormService.GetProductID(*it)==strProductID)
                            {
                                m_InstrumentIDsNeedQry2.push_back(*it);
                                it=vecInstrumentIDs.erase(it);
                            }
                            else it++;
                        }
                        if(!bHaveCommissionRate) m_QryCommissionRateCount[strInstrumentID]=0;
                        if(!bHaveMarginRate) m_QryMarginRateCount[strInstrumentID]=0;
                        continue;
                    }

                    //ʣ�µ�������������Լ�����ˣ�������m_InstrumentIDsNeedQry2��鲻���ģ��Ӷ�����ɾ��
                    vecInstrumentIDs.erase(vecInstrumentIDs.begin());
                }
            }
        }

        if(bHavePkg) m_QryRateCount++;
        if(!bHavePkg)
        {
                                                        //���к�Լ�ķ��ʶ��Ѿ����ˣ�������ı�־
            m_RefPlatFormService.SetNeedCheckCommissionRateAndMarginRate(false);
        }
    }

    if(bHavePkg)
    {
        //��ǰ��m_RefPlatFormService.m_bInQry��Ϊtrue���������˴���̫�죬�Ѿ��õ��������ִ��m_RefPlatFormService.m_bInQry=true���Ӷ����³�ʱ�����
        m_RefPlatFormService.m_bInQry=true;

        switch(PackageHead.CmdID)
        {
            case QryID_ReqBrokerTradingParam:               //��ѯ���͹�˾���ײ���
            {
                CThostFtdcQryBrokerTradingParamsField QryInfo={0};
                const PlatformStru_LoginInfo& LoginInfo=m_RefPlatFormService.GetTradingLoginInfo();
                strncpy(QryInfo.BrokerID,LoginInfo.BrokerID,sizeof(QryInfo.BrokerID)-1);
                strncpy(QryInfo.InvestorID,LoginInfo.UserID,sizeof(QryInfo.InvestorID)-1);

                ReqRlt=m_RefPlatFormService.ReqQryBrokerTradingParams(&QryInfo,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ���͹�˾���ײ���)ReqQryBrokerTradingParams : BrokerID=%s InvestorID=%s Rlt=%d",QryInfo.BrokerID,QryInfo.InvestorID,ReqRlt);
                break;
            }

            case QryID_ReqQryProduct:                       //��ѯƷ����Ϣ
                ReqRlt=m_RefPlatFormService.ReqQryProduct((PlatformStru_QryProduct*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯƷ����Ϣ)ReqQryProduct : ProductID=%s Rlt=%d",((PlatformStru_QryProduct*)PkgDataBuf)->Thost.ProductID,ReqRlt);
                break;

            case QryID_ReqQryInstrument:                    //��ѯ��Լ��Ϣ
                ReqRlt=m_RefPlatFormService.ReqQryInstrument((PlatformStru_QryInstrument*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ��Լ��Ϣ)ReqQryInstrument : InstrumentID=%s Rlt=%d",((PlatformStru_QryInstrument*)PkgDataBuf)->Thost.InstrumentID,ReqRlt);
                break;

            case QryID_ReqQryTradingAccount:                //��ѯ�ʽ�
                ReqRlt=m_RefPlatFormService.ReqQryTradingAccount((PlatformStru_QryTradingAccount*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ʽ���Ϣ)ReqQryTradingAccount Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryTrade:                         //��ѯ�ɽ�
                ReqRlt=m_RefPlatFormService.ReqQryTrade((PlatformStru_QryTrade*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ɽ���Ϣ)ReqQryTrade Rlt=%d ",ReqRlt);
                break;

            case QryID_ReqQryOrder:                         //��ѯ����
                ReqRlt=m_RefPlatFormService.ReqQryOrder((PlatformStru_QryOrder*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ������Ϣ)QryID_ReqQryOrder Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryInvestorPositionDetail:        //��ѯ�ֲ���ϸ
                ReqRlt=m_RefPlatFormService.ReqQryInvestorPositionDetail((PlatformStru_QryInvestorPositionDetail*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ֲ���ϸ)ReqQryInvestorPositionDetail Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryInvestorPosition:              //��ѯ�ֲ�
                ReqRlt=m_RefPlatFormService.ReqQryInvestorPosition((PlatformStru_QryInvestorPosition*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ֲֻ���)ReqQryInvestorPosition Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQrySettlementInfo:                //��ѯͶ���߽�����
                ReqRlt=m_RefPlatFormService.ReqQrySettlementInfo((PlatformStru_QrySettlementInfo*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯͶ���߽�����)ReqQrySettlementInfo Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryCFMMCTradingAccountKey:        //�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
                ReqRlt=m_RefPlatFormService.ReqQryCFMMCTradingAccountKey((PlatformStru_QryCFMMCTradingAccountKey*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ)ReqQrySettlementInfo Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryDepthMarketData:               //��ѯ������
                ReqRlt=m_RefPlatFormService.ReqQryDepthMarketData((PlatformStru_QryDepthMarketData*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ����)ReqQryDepthMarketData Rlt=%d",ReqRlt);
                break;




            case QryID_ReqQryInvestorPositionCombineDetail: //�����ѯͶ���ֲ߳���ϸ(���)
                ReqRlt=m_RefPlatFormService.ReqQryInvestorPositionCombineDetail((PlatformStru_QryInvestorPositionCombineDetail*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯͶ���ֲ߳���ϸ(���))ReqQryInvestorPositionCombineDetail Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryNotice:                        //�����ѯ�ͻ�֪ͨ
                ReqRlt=m_RefPlatFormService.ReqQryNotice((PlatformStru_QryNotice*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ͻ�֪ͨ)ReqQryNotice Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQrySettlementInfoConfirm:         //�����ѯ������Ϣȷ��
                ReqRlt=m_RefPlatFormService.ReqQrySettlementInfoConfirm((PlatformStru_QrySettlementInfoConfirm*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ������Ϣȷ��)ReqQrySettlementInfoConfirm Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryTransferBank:                  //�����ѯת������
                ReqRlt=m_RefPlatFormService.ReqQryTransferBank((PlatformStru_QryTransferBank*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯת������)ReqQryTransferBank Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryTransferSerial:                //�����ѯת����ˮ
                ReqRlt=m_RefPlatFormService.ReqQryTransferSerial((PlatformStru_QryTransferSerial*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯת����ˮ)ReqQryTransferSerial Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryContractBank:                  //�����ѯǩԼ����
                ReqRlt=m_RefPlatFormService.ReqQryContractBank((PlatformStru_QryContractBank*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯǩԼ����)ReqQryContractBank Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryAccountregister:               //�����ѯ����ǩԼ��ϵ
                ReqRlt=m_RefPlatFormService.ReqQryAccountregister((PlatformStru_QryAccountRegister*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ����ǩԼ��ϵ)ReqQryAccountregister Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryExchangeRate:                  //�����ѯ����
                ReqRlt=m_RefPlatFormService.ReqQryExchangeRate((PlatformStru_QryExchangeRate*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ����)ReqQryExchangeRate Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryParkedOrder:                   //�����ѯԤ��
                ReqRlt=m_RefPlatFormService.ReqQryParkedOrder((PlatformStru_QryParkedOrder*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯԤ��)ReqQryParkedOrder Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryParkedOrderAction:             //�����ѯԤ�񳷵�
                ReqRlt=m_RefPlatFormService.ReqQryParkedOrderAction((PlatformStru_QryParkedOrderAction*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯԤ�񳷵�)ReqQryParkedOrderAction Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQueryBankAccountMoneyByFuture:    //��ѯ�������
                ReqRlt=m_RefPlatFormService.ReqQueryBankAccountMoneyByFuture((PlatformStru_ReqQueryAccount*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�������)ReqQueryBankAccountMoneyByFuture Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryExecOrder:                     //��ѯִ������
                ReqRlt=m_RefPlatFormService.ReqQryExecOrder(*(PlatformStru_QryExecOrderField*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯִ������)ReqQryExecOrder Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQueryMaxOrderVolume:              //��ѯ��󱨵�����
                ReqRlt=m_RefPlatFormService.ReqQueryMaxOrderVolume((PlatformStru_QueryMaxOrderVolume*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ��󱨵�����)ReqQueryMaxOrderVolume Rlt=%d",ReqRlt);
                break;



            case QryID_ReqQryInstrumentCommissionRate:      //��ѯ��Լ��������. �ڻ���ֱ�Ӳ��������ʣ���Ȩ������Ȩ�������ʣ�InstrumentIDΪ�գ�m_QryRateCount==1����������; 2����Ȩ��������
            {
                CThostFtdcQryInstrumentCommissionRateField& QryCommRate(*((CThostFtdcQryInstrumentCommissionRateField*)PkgDataBuf));
                string strInstrumentID(QryCommRate.InstrumentID);
                m_RefPlatFormService.SetReqCommissionRateInstrumentID(strInstrumentID);
                char ProductClass;
                bool bInstrumentIDEmpty=strInstrumentID.empty();
                if(!bInstrumentIDEmpty) ProductClass=m_RefPlatFormService.GetProductClassType(strInstrumentID);
                if(bInstrumentIDEmpty&&m_QryRateCount==1||!bInstrumentIDEmpty&&ProductClass==THOST_FTDC_PC_Futures)
                {
                    ReqRlt=m_RefPlatFormService.ReqQryInstrumentCommissionRate(&QryCommRate,(int)PackageHead.dwSeq);
                }
#ifdef CTP060300
                else if(bInstrumentIDEmpty&&m_QryRateCount==2||!bInstrumentIDEmpty&&(ProductClass==THOST_FTDC_PC_Options||ProductClass==THOST_FTDC_PC_SpotOption))
                {
                    CThostFtdcQryOptionInstrCommRateField QryOptionCommRate={0};
                    strncpy(QryOptionCommRate.BrokerID,QryCommRate.BrokerID,sizeof(QryOptionCommRate.BrokerID)-1);
                    strncpy(QryOptionCommRate.InvestorID,QryCommRate.InvestorID,sizeof(QryOptionCommRate.InvestorID)-1);;
                    strncpy(QryOptionCommRate.InstrumentID,QryCommRate.InstrumentID,sizeof(QryOptionCommRate.InstrumentID)-1);

                    ReqRlt=m_RefPlatFormService.ReqQryOptionInstrCommRate(&QryOptionCommRate,(int)PackageHead.dwSeq);
                }
#endif

                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ڻ�/��Ȩ��Լ��������)ReqQryInstrumentCommissionRate��InstrumentID=%s ProductClass=%d Rlt=%d",((CThostFtdcQryInstrumentCommissionRateField*)PkgDataBuf)->InstrumentID,ProductClass,ReqRlt);
                if(ReqRlt==0)
                {
                    m_QryCommissionRateTime[strInstrumentID]=time(NULL);
                    map<string,int>::iterator it_GetCount=m_QryCommissionRateCount.find(strInstrumentID);
                    if(it_GetCount==m_QryCommissionRateCount.end())
                        m_QryCommissionRateCount[strInstrumentID]=1;
                    else
                        it_GetCount->second=it_GetCount->second+1;
                }
                break;
            }

            case QryID_ReqQryInstrumentMarginRate:          //�����ѯ��Լ��֤���ʡ��ڻ���ֱ�Ӳ鱣֤���ʣ���Ȩ���齻�׳ɱ���InstrumentIDΪ�գ�m_QryRateCount==3�鱣֤����; 4����Ȩ���׳ɱ�
            {
                CThostFtdcQryInstrumentMarginRateField& QryMarginRate(*((CThostFtdcQryInstrumentMarginRateField*)PkgDataBuf));
                string strInstrumentID(QryMarginRate.InstrumentID);
                char ProductClass;
                bool bInstrumentIDEmpty=strInstrumentID.empty();
                if(!bInstrumentIDEmpty) ProductClass=m_RefPlatFormService.GetProductClassType(strInstrumentID);
                if(bInstrumentIDEmpty&&m_QryRateCount==3||!bInstrumentIDEmpty&&ProductClass==THOST_FTDC_PC_Futures)
                {
                    ReqRlt=m_RefPlatFormService.ReqQryInstrumentMarginRate(&QryMarginRate,(int)PackageHead.dwSeq);
                }
#ifdef CTP060300
                else if(bInstrumentIDEmpty&&m_QryRateCount==4||!bInstrumentIDEmpty&&(ProductClass==THOST_FTDC_PC_Options||ProductClass==THOST_FTDC_PC_SpotOption))
                {
                    CThostFtdcQryOptionInstrTradeCostField QryOptionTradeCost={0};
                    strncpy(QryOptionTradeCost.BrokerID,QryMarginRate.BrokerID,sizeof(QryOptionTradeCost.BrokerID)-1);
                    strncpy(QryOptionTradeCost.InvestorID,QryMarginRate.InvestorID,sizeof(QryOptionTradeCost.InvestorID)-1);;
                    strncpy(QryOptionTradeCost.InstrumentID,QryMarginRate.InstrumentID,sizeof(QryOptionTradeCost.InstrumentID)-1);
                    QryOptionTradeCost.HedgeFlag=QryMarginRate.HedgeFlag;
                    QryOptionTradeCost.InputPrice=0;
                    QryOptionTradeCost.UnderlyingPrice=0;

                    ReqRlt=m_RefPlatFormService.ReqQryOptionInstrTradeCost(&QryOptionTradeCost,(int)PackageHead.dwSeq);
                }
#endif

                LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ڻ���Լ��֤����/��Ȩ��Լ���׳ɱ�)ReqQryInstrumentMarginRate��InstrumentID=%s ProductClass=%d Rlt=%d",((CThostFtdcQryInstrumentMarginRateField*)PkgDataBuf)->InstrumentID,ProductClass,ReqRlt);
                if(ReqRlt==0)
                {
                    m_QryMarginRateTime[strInstrumentID]=time(NULL);
                    map<string,int>::iterator it_count=m_QryMarginRateCount.find(strInstrumentID);
                    if(it_count==m_QryMarginRateCount.end())
                        m_QryMarginRateCount[strInstrumentID]=1;
                    else
                        it_count->second=it_count->second+1;
                }
                break;
            }
            default:
                ReqRlt=-1;
                break;
        }

        if(ReqRlt==0) 
        { 
            //�ɹ�������ѯ����
            (bCache?m_pReqQueue:m_pReqQueue2)->DeleteCurPackage();
            return true; 
        }                                                               
    }

    return false;
}

//�ж�Cache�е������ѯ�����ݰ��Ƿ���ȷ
//������ݳ��ȣ��Լ������ʱ����ط����Ƿ��Ѿ�����
bool CQryQueue::IsReqQryPackageInCacheValid(const PackageHeadStru& PackageHead,const void* pPackageData)
{
    switch(PackageHead.CmdID)
    {
        case QryID_ReqBrokerTradingParam:               return PackageHead.DataLen==0;
        case QryID_ReqQryProduct:                       return PackageHead.DataLen==(int)sizeof(PlatformStru_QryProduct);
        case QryID_ReqQryInstrument:                    return PackageHead.DataLen==(int)sizeof(PlatformStru_QryInstrument);
        case QryID_ReqQryTradingAccount:                return PackageHead.DataLen==(int)sizeof(PlatformStru_QryTradingAccount);
        case QryID_ReqQryTrade:                         return PackageHead.DataLen==(int)sizeof(PlatformStru_QryTrade);
        case QryID_ReqQryOrder:                         return PackageHead.DataLen==(int)sizeof(PlatformStru_QryOrder);
        case QryID_ReqQryInvestorPositionDetail:        return PackageHead.DataLen==(int)sizeof(PlatformStru_QryInvestorPositionDetail);
        case QryID_ReqQryInvestorPosition:              return PackageHead.DataLen==(int)sizeof(PlatformStru_QryInvestorPosition);
        case QryID_ReqQrySettlementInfo:                return PackageHead.DataLen==(int)sizeof(PlatformStru_QrySettlementInfo);
        case QryID_ReqQryCFMMCTradingAccountKey:        return PackageHead.DataLen==(int)sizeof(PlatformStru_QryCFMMCTradingAccountKey);
        case QryID_ReqQryDepthMarketData:               return PackageHead.DataLen==(int)sizeof(PlatformStru_QryDepthMarketData);
        case QryID_ReqQryInvestorPositionCombineDetail: return PackageHead.DataLen==(int)sizeof(PlatformStru_QryInvestorPositionCombineDetail);
        case QryID_ReqQryNotice:                        return PackageHead.DataLen==(int)sizeof(PlatformStru_QryNotice);
        case QryID_ReqQrySettlementInfoConfirm:         return PackageHead.DataLen==(int)sizeof(PlatformStru_QrySettlementInfoConfirm);
        case QryID_ReqQryTransferBank:                  return PackageHead.DataLen==(int)sizeof(PlatformStru_QryTransferBank);
        case QryID_ReqQryTransferSerial:                return PackageHead.DataLen==(int)sizeof(PlatformStru_QryTransferSerial);
        case QryID_ReqQryContractBank:                  return PackageHead.DataLen==(int)sizeof(PlatformStru_QryContractBank);
        case QryID_ReqQryAccountregister:               return PackageHead.DataLen==(int)sizeof(PlatformStru_QryAccountRegister);
        case QryID_ReqQryExchangeRate:                  return PackageHead.DataLen==(int)sizeof(PlatformStru_QryExchangeRate);
        case QryID_ReqQryParkedOrder:                   return PackageHead.DataLen==(int)sizeof(PlatformStru_QryParkedOrder);
        case QryID_ReqQryParkedOrderAction:             return PackageHead.DataLen==(int)sizeof(PlatformStru_QryParkedOrderAction);
        case QryID_ReqQueryBankAccountMoneyByFuture:    return PackageHead.DataLen==(int)sizeof(PlatformStru_ReqQueryAccount);
        case QryID_ReqQryExecOrder:                     return PackageHead.DataLen==(int)sizeof(PlatformStru_QryExecOrderField);
        case QryID_ReqQueryMaxOrderVolume:              return PackageHead.DataLen==(int)sizeof(PlatformStru_QueryMaxOrderVolume);
        case QryID_ReqQryInstrumentCommissionRate:      //��ѯ��Լ��������
        {
            if(PackageHead.DataLen!=(int)sizeof(CThostFtdcQryInstrumentCommissionRateField))  return false;
            string strInstrumentID(((CThostFtdcQryInstrumentCommissionRateField*)pPackageData)->InstrumentID);
            if(strInstrumentID.empty()) return false;

            ///��ȡ��Լ��������, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
            PlatformStru_InstrumentCommissionRate CommissionRate;
		    int ErrNo=m_RefPlatFormService.GetCommissionRate(strInstrumentID, CommissionRate);
            if(ErrNo==0||ErrNo==CustomErrorID) return false;
            //�������ˣ�����Ͳ�����
            if(m_QryCommissionRateCount.find(strInstrumentID)!=m_QryCommissionRateCount.end()) return false;
            //��������£����Բ�
            return true;
        }
        case QryID_ReqQryInstrumentMarginRate:          //�����ѯ��Լ��֤����
        {
            if(PackageHead.DataLen!=(int)sizeof(CThostFtdcQryInstrumentMarginRateField))  return false;
            string strInstrumentID(((CThostFtdcQryInstrumentMarginRateField*)pPackageData)->InstrumentID);
            if(strInstrumentID.empty()) return false;

            ///��ȡ��Լ��֤����, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
            PlatformStru_InstrumentMarginRate MarginRate;
		    int ErrNo=m_RefPlatFormService.GetMarginRate(strInstrumentID, MarginRate);
            if(ErrNo==0||ErrNo==CustomErrorID) return false;
            //�������ˣ�����Ͳ�����
            if(m_QryMarginRateTime.find(strInstrumentID)!=m_QryMarginRateTime.end()) return false;
            //��������£����Բ�
            return true;
        }
    }
    return false;
}
