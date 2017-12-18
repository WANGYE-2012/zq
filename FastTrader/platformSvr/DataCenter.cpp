#include "stdafx.h"

#include <time.h>
#include "ServiceProxy.hpp"
#include "DataCenter.hpp"
#include "Module-Misc2/packagecache.h"
#include "Module-Misc2/SimpleWriteLog.h"
#include "PlatFormService.hpp"
#include "Module-Misc2/globalfunc.h"
#include "ISvr/IPlatformSingleSvr.h"
#include "ISvr/PlatformSvrMgr.h"
#include "Module-Misc2/GlobalConfigMgr.h"
#include "Module-Misc2/globalDefines.h"
#include "ReqOnRtnTrade.h"
#include "Tools_Win32.h"
#include "popmsgwin.h"
#include "easytime.h"
#include "Module-Misc2/tools_tools.h"
#include "FileOpr.h"
#include "myxml.h"
#include "TradingInfo.h"

extern PlatformSvrMgr* g_pPlatformMgr;

//�����Ƿ�BusinessData���ݰ������ļ����Ա��طŽ��е���
//#ifdef _DEBUG
//#define SAVEBUSINESSDATA
//bool g_SaveBusinessData=true;              //�Ƿ񱣴�ײ�ҵ������
//bool g_PlayBusinessData=false;              //�Ƿ�طŵײ�ҵ������
//unsigned int g_PlayTimeDuration=3600*4; //�طŶ೤ʱ���ҵ�����ݣ���λΪ�롣��ʾ�طŵ�ǰʱ���ȥ���ֵ��ʱ���Ժ������
//#endif

#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CDataCenter", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
    }while(0)

#define LOG_INFO2(fmt, ...) \
    do{\
		if(m_DataCenter.m_pWriteLog)\
		{\
		m_DataCenter.m_pWriteLog->WriteLog_Fmt("CDataCenter", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
    }while(0)


#define INIT_OUTDATA(structname)\
    structname field;\
    memset(&field,0,sizeof(structname));\
    outData=field

int g_LockCount=0;
char LastFunction1[256]={0};
char LastFunction2[256]={0};
char LastFunction3[256]={0};
char LastFunction4[256]={0};
//#define MUTEX_LOCK()\
//	g_LockCount=0;\
//	while(m_mutex.TryLock()!=wxMUTEX_NO_ERROR)\
//	{\
//		g_LockCount++;\
//		if(g_LockCount==1000)\
//		{\
//				LOG_INFO("Lock Failed at (%s)--->(%s)--->(%s)--->(%s)--->(%s)",LastFunction1,LastFunction2,LastFunction3,LastFunction4,__FUNCTION__);\
//		}\
//		wxThread::Sleep(10);\
//	}\
//    m_mutex.Unlock();\
//	strcpy(LastFunction1,LastFunction2);\
//	strcpy(LastFunction2,LastFunction3);\
//	strcpy(LastFunction3,LastFunction4);\
//    strcpy(LastFunction4,__FUNCTION__);\
//    wxMutexLocker lock(m_mutex)


class CDataCenterWorkThread : public wxThread
{
public:
    CDataCenterWorkThread(CDataCenter& dc)
    :   wxThread(),m_DataCenter(dc)
        ,StartNo_Test_PlayBusinessData(0)
        ,EndNo_Test_PlayBusinessData(20000)

    {

    };

    // thread execution starts here
    virtual void *Entry()
    {
        char Buf[512];
        sprintf(Buf,"CDataCenter::ThreadId=%u\n", GetCurrentThreadId());
        OutputDebugString(Buf);

        if(GlobalConfigManager::m_Test_bLogThreadID)
            CFileOpr::getObj().writelocallog("threadid","TID:\t% 8u\tCDataCenterWorkThread::Entry",GetCurrentThreadId());

        CPackageCache& RefPackageCache=m_DataCenter.m_dataSource;
        char readBuf[10240];
        PackageHeadStru *pPackageHead=(PackageHeadStru *)readBuf;
        char *pData= readBuf+ sizeof(PackageHeadStru);

        unsigned int BeginTickCount=GetTickCount();
        unsigned int StartInitQryTickCount=0;

        while(!TestDestroy())
        {
            //����ʱ���ط�businessdata.rec�е�ҵ���
            unsigned int CurTickCount=GetTickCount();
            if(GlobalConfigManager::m_Test_bPlayBusinessData)
            {
                if(!StartInitQryTickCount&&m_DataCenter.m_PlatFormService->m_rServiceProxy.m_bInitQryStart)
                    StartInitQryTickCount=CurTickCount;

                if(StartInitQryTickCount>0&&
                    CurTickCount-StartInitQryTickCount>=3000/*&&
                    m_DataCenter.m_PlatFormService->m_rServiceProxy.m_bInitQryStart*/)
                {
                    m_mutex_Test_SaveLoadBusinessData.lock();

                    char szAccountID[32]={0};
                    char sztmpAccountID[32];
                    strncpy(szAccountID,m_DataCenter.m_LoginInvestorID.c_str(),sizeof(szAccountID)-1);

                    static CPopMsgWin *pmsgwin1=NULL;
                    static CPopMsgWin *pmsgwin2=NULL;

                    if(!pmsgwin1||!pmsgwin2)
                    {
                        CPopMsgWin::Stru_Param msgwinpam;
                        strcpy(msgwinpam.szTitle,"OriPackage");
                        msgwinpam.MaxMsgLen=655360;
                        msgwinpam.InitPox=10;
                        msgwinpam.InitWid=500;
                        msgwinpam.InitHei=100;
                        pmsgwin1=new CPopMsgWin(msgwinpam);

                        strcpy(msgwinpam.szTitle,"PlaybackPackage");
                        msgwinpam.InitPox+=msgwinpam.InitWid;
                        pmsgwin2=new CPopMsgWin(msgwinpam);

                        while(!pmsgwin1->IsWndCreated()) Sleep(10);
                        while(!pmsgwin2->IsWndCreated()) Sleep(10);
                    }

                    char filename[256];
                    memset(filename,0,sizeof(filename));
                    CTools_Win32::GetMainPath(filename,sizeof(filename));
                    int len=strlen(filename);
                    strncpy(filename+len,"\\businessdata.rec",sizeof(filename)-1-len);

                    static char readBuf2[10240];
                    PackageHeadStru *pPackageHead2=(PackageHeadStru *)readBuf2;
                    char *pData2= readBuf2+ sizeof(PackageHeadStru);

                    HFILE hF;
                    char buf[512];

                    static bool bFirst=true;
                    static int RecNum=0;
                    if(bFirst)
                    {
                        bFirst=false;

                        //���ڹ��˵�����Ҫ������
                        map<string,DataRtnDepthMarketData> mapDepthMarketData;

                        hF=_lopen(filename,OF_READ);
                        if(hF!=HFILE_ERROR)
                        {
                            int filelen=_llseek(hF,0,2);

                            //��������ʱ����businessdata.rec���ҵ���������ʾ��popmsgwin��ȥ
                            int off=0;
                            _llseek(hF,0,0);
                            while(off<filelen)
                            {
                                _lread(hF,pPackageHead2,sizeof(PackageHeadStru));
                                _lread(hF,pData2,pPackageHead2->DataLen);
                                _lread(hF,sztmpAccountID,sizeof(sztmpAccountID));
                                sztmpAccountID[sizeof(sztmpAccountID)-1]=0;

                                bool bFilter=false;
                                if(!bFilter&&((AbstractBusinessData*)pData2)->BID==BID_RtnDepthMarketData)
                                {
                                    //���˵�����Ҫ������
                                    DataRtnDepthMarketData& MarketBusiness= *((DataRtnDepthMarketData*)pData2);
                                    string strInst=string(MarketBusiness.DepthMarketDataField.InstrumentID);
                                    map<string,DataRtnDepthMarketData>::const_iterator it=mapDepthMarketData.find(strInst);
                                    if(it!=mapDepthMarketData.end()&&
                                        it->second.DepthMarketDataField.LastPrice==MarketBusiness.DepthMarketDataField.LastPrice&&
                                        it->second.DepthMarketDataField.SettlementPrice==MarketBusiness.DepthMarketDataField.SettlementPrice&&
                                        it->second.DepthMarketDataField.PreSettlementPrice==MarketBusiness.DepthMarketDataField.PreSettlementPrice)
                                    {
                                        bFilter=true;
                                    }
                                    if(!bFilter) mapDepthMarketData[strInst]=MarketBusiness;
                                }

                                if(!bFilter)
                                {
                                    sprintf(buf,"%d: ",RecNum);
                                    CEasyTime((time_t)pPackageHead2->dwCRC).getstrshorttime(buf+strlen(buf),sizeof(buf)-strlen(buf));
                                    sprintf(buf+strlen(buf),":%s\t@%s",CTools::BusinessID2String((int)(((AbstractBusinessData*)pData2)->BID)),sztmpAccountID);
                                    pmsgwin1->AppendStr(CPopMsgWin::ForceShow,CPopMsgWin::NoDelOldContent,CPopMsgWin::AddCRLF,CPopMsgWin::NoInsertTime,buf);
                                }
                                RecNum++;
                                off+=sizeof(PackageHeadStru)+pPackageHead2->DataLen+sizeof(sztmpAccountID);
                            }
                            _lclose(hF);
                        }
                    }

                    //�ط����ΪStartNo-EndNo��ҵ���(StartNo <= No < EndNo)
                    //EndNo=RecNum;
                    static int EndNo_Static=20000;
                    if(EndNo_Test_PlayBusinessData!=EndNo_Static) EndNo_Test_PlayBusinessData=EndNo_Static;
                    if(EndNo_Test_PlayBusinessData>StartNo_Test_PlayBusinessData)
                    {
                        hF=_lopen(filename,OF_READ);
                        if(hF!=HFILE_ERROR)
                        {
                            int filelen=_llseek(hF,0,2);

                            //���ڹ��˵�����Ҫ������
                            map<string,DataRtnDepthMarketData> mapDepthMarketData;

                            int off=0;
                            int ind=0;
                            _llseek(hF,0,0);
                            while(off<filelen&&ind<StartNo_Test_PlayBusinessData)
                            {
                                _lread(hF,pPackageHead2,sizeof(PackageHeadStru));
                                _lread(hF,pData2,pPackageHead2->DataLen);
                                _lread(hF,sztmpAccountID,sizeof(sztmpAccountID));
                                sztmpAccountID[sizeof(sztmpAccountID)-1]=0;
                                off+=sizeof(PackageHeadStru)+pPackageHead2->DataLen+sizeof(sztmpAccountID);
                                ind++;
                            }

                            while(off<filelen&&ind<EndNo_Test_PlayBusinessData)
                            {
                                _lread(hF,pPackageHead2,sizeof(PackageHeadStru));
                                _lread(hF,pData2,pPackageHead2->DataLen);
                                _lread(hF,sztmpAccountID,sizeof(sztmpAccountID));
                                sztmpAccountID[sizeof(sztmpAccountID)-1]=0;

                                bool bFilter=false;
                                if(memcmp(szAccountID,sztmpAccountID,sizeof(szAccountID))!=0)
                                    bFilter=true;
                                if(!bFilter&&((AbstractBusinessData*)pData2)->BID==BID_RtnDepthMarketData)
                                {
                                    //���˵�����Ҫ������
                                    DataRtnDepthMarketData& MarketBusiness= *((DataRtnDepthMarketData*)pData2);
                                    string strInst=string(MarketBusiness.DepthMarketDataField.InstrumentID);
                                    map<string,DataRtnDepthMarketData>::const_iterator it=mapDepthMarketData.find(strInst);
                                    if(it!=mapDepthMarketData.end()&&
                                        it->second.DepthMarketDataField.LastPrice==MarketBusiness.DepthMarketDataField.LastPrice&&
                                        it->second.DepthMarketDataField.SettlementPrice==MarketBusiness.DepthMarketDataField.SettlementPrice&&
                                        it->second.DepthMarketDataField.PreSettlementPrice==MarketBusiness.DepthMarketDataField.PreSettlementPrice)
                                    {
                                        bFilter=true;
                                    }
                                    if(!bFilter) mapDepthMarketData[strInst]=MarketBusiness;
                                }

                                if(!bFilter)
                                {
                                    m_DataCenter.doProcessBusinessData(*((AbstractBusinessData*)pData2)); 


                                    //�����BID_RspQrySettlementInfoConfirm����Ҫ����m_RefPlatFormService.m_bSettlementInfoConfirmed
                                    if(((AbstractBusinessData*)pData2)->BID==BID_RspQrySettlementInfoConfirm)
                                    {
                                        m_DataCenter.m_PlatFormService->m_bSettlementInfoConfirmed=((DataRspQrySettlementInfoConfirm*)pData2)->SettlementInfoConfirm.BrokerID[0]!=0?true:false;
                                        m_DataCenter.m_PlatFormService->m_PlatformParam.TradeStatus=CTPCONNSTATUS_Connected;

                                    }

                                    //if( ((AbstractBusinessData*)pData2)->BID==BID_RtnTrade||
                                    //    ((AbstractBusinessData*)pData2)->BID==BID_RtnOrder)
                                    //    Sleep(200);

                                    sprintf(buf,"%d: ",ind);
                                    CEasyTime((time_t)pPackageHead2->dwCRC).getstrshorttime(buf+strlen(buf),sizeof(buf)-strlen(buf));
                                    sprintf(buf+strlen(buf),":%s\t%s",CTools::BusinessID2String((int)(((AbstractBusinessData*)pData2)->BID)),sztmpAccountID);
                                    pmsgwin2->AppendStr(CPopMsgWin::ForceShow,CPopMsgWin::NoDelOldContent,CPopMsgWin::AddCRLF,CPopMsgWin::NoInsertTime,buf);

                                    if( ((AbstractBusinessData*)pData2)->BID==BID_RspQrySettlementInfo&&((DataRspQrySettlementInfo*)pData2)->bIsLast ||
                                        ((AbstractBusinessData*)pData2)->BID==BID_RspQrySettlementInfoConfirm&&((DataRspQrySettlementInfoConfirm*)pData2)->bIsLast ||
                                        ((AbstractBusinessData*)pData2)->BID==BID_RspQryInstrument&&((DataRspQryInstrument*)pData2)->bIsLast ||
                                        ((AbstractBusinessData*)pData2)->BID==BID_RspQryAccountregister&&((DataRspQryAccountregister*)pData2)->bIsLast ||
                                        ((AbstractBusinessData*)pData2)->BID==BID_RspQryTradingAccount&&((DataRspQryTradingAccount*)pData2)->bIsLast ||
                                        ((AbstractBusinessData*)pData2)->BID==BID_RspQryTrade&&((DataRspQryTrade*)pData2)->bIsLast ||
                                        ((AbstractBusinessData*)pData2)->BID==BID_RspQryOrder&&((DataRspQryOrder*)pData2)->bIsLast ||
                                        ((AbstractBusinessData*)pData2)->BID==BID_RspQryInvestorPositionDetail&&((DataRspQryInvestorPositionDetail*)pData2)->bIsLast ||
                                        ((AbstractBusinessData*)pData2)->BID==BID_RspQryInvestorPosition&&((DataRspQryInvestorPosition*)pData2)->bIsLast ||
                                        ((AbstractBusinessData*)pData2)->BID==BID_RspQryContractBank&&((DataRspQryContractBank*)pData2)->bIsLast ||
                                        ((AbstractBusinessData*)pData2)->BID==BID_RspQryContractBank&&((DataRspQryContractBank*)pData2)->bIsLast )
                                    {
                                        m_mutex_Test_SaveLoadBusinessData.unlock();
                                        Sleep(2000);
                                        m_mutex_Test_SaveLoadBusinessData.lock();
                                    }


                                }

                                off+=sizeof(PackageHeadStru)+pPackageHead2->DataLen+sizeof(sztmpAccountID);
                                ind++;

                                ////BID_RspQryAccountregister�ǳ�ʼ����ѯ�����һ��
                                //if(((AbstractBusinessData*)pData2)->BID==BID_RspQryAccountregister&&
                                //    ((DataRspQryParkedOrderAction*)pData2)->bIsLast)
                                //{
                                //    Sleep(12000);
                                //}
                            }
                            StartNo_Test_PlayBusinessData=EndNo_Test_PlayBusinessData;

                            _lclose(hF);
                        }
                    }
                    m_mutex_Test_SaveLoadBusinessData.unlock();
                }

            }



            if(RefPackageCache.IsEmpty())
            {
                wxThread::Sleep(2);
                continue;
            }
           
            int GetOff=-1;
            if(!RefPackageCache.PeekFrom(pPackageHead, pData, sizeof(readBuf)-sizeof(PackageHeadStru),&GetOff))
                continue;
            
            //LOG_INFO2("CDataCenterWorkThread PeekFrom:CMDID=%d DataLen=%d GetOff=%d",pPackageHead->CmdID,pPackageHead->DataLen,GetOff);

            if(pPackageHead->DataLen<sizeof(AbstractBusinessData))
            {
                int OriGetOff,NewGetOff;
                RefPackageCache.DeleteCurPackage(&OriGetOff,&NewGetOff);
                LOG_INFO2("CDataCenterWorkThread DeleteCurPackage(���Ȳ���):OriGetOff=%d NewGetOff=%d, (package CMDID=%d DataLen=%d)",
                    OriGetOff,NewGetOff,pPackageHead->CmdID,pPackageHead->DataLen);
                continue;
            }

            AbstractBusinessData tmpAbstractBusinessData=*((AbstractBusinessData*)pData);
            PackageHeadStru tmpPackageHead=*pPackageHead;
            int tmpGetOff=GetOff;

            if(GlobalConfigManager::m_Test_bSaveBusinessData)
            {
                //�ѵײ�ҵ�����ݱ��浽�ļ���ȥ
                m_mutex_Test_SaveLoadBusinessData.lock();

                HFILE hF;
                char filename[256];
                memset(filename,0,sizeof(filename));
                CTools_Win32::GetMainPath(filename,sizeof(filename));
                int len=strlen(filename);
                strncpy(filename+len,"\\businessdata.rec",sizeof(filename)-1-len);

                //��������ʱ�������û�оɵ�businessdata.rec������У�����Ϊbusinessdata_yyyymmdd_hhmmss.rec
                static bool bFirst=true;
                if(bFirst)
                {
                    hF=_lopen(filename,OF_READ);
                    if(hF!=HFILE_ERROR)
                    {
                        _lclose(hF);
                        char filename2[512];
                        SYSTEMTIME curTime;
                        GetLocalTime(&curTime);
                        memset(filename2,0,sizeof(filename2));
                        CTools_Win32::GetMainPath(filename2,sizeof(filename2));
                        int len2=strlen(filename2);
                        sprintf(filename2+len2,"\\businessdata_%04d%02d%02d_%02d%02d%02d.rec",curTime.wYear,curTime.wMonth,curTime.wDay,curTime.wHour,curTime.wMinute,curTime.wSecond);
                        rename(filename,filename2);
                    }
                    bFirst=false;
                }


                hF=_lopen(filename,OF_READWRITE);
                if(hF==HFILE_ERROR) hF=_lcreat(filename,0);
                _llseek(hF,0,2);
                pPackageHead->dwCRC=(unsigned int)time(NULL);
                _lwrite(hF,(char*)pPackageHead,sizeof(PackageHeadStru));
                _lwrite(hF,pData,pPackageHead->DataLen);

                //��д��AccountID��32�ֽ��ַ�����
                char szAccountID[32]={0};
                strncpy(szAccountID,m_DataCenter.m_LoginInvestorID.c_str(),sizeof(szAccountID)-1);
                _lwrite(hF,szAccountID,sizeof(szAccountID));

                _lclose(hF);

                m_mutex_Test_SaveLoadBusinessData.unlock();
            }

            //DataCenter�Ѿ��͵ײ�ͬ���ϣ����Կ�ʼ��ʼ����ѯ�ˡ�
            if(((AbstractBusinessData*)pData)->BID==BID_RspQrySettlementInfoConfirm)
                m_DataCenter.bRspQrySettlementInfoConfirm=true;

            if(!GlobalConfigManager::m_Test_bPlayBusinessData||
                ((AbstractBusinessData*)pData)->BID==BID_RspUserLogin)
            {
                m_DataCenter.doProcessBusinessData(*((AbstractBusinessData*)pData)); 
            }

            int OriGetOff,NewGetOff;
            RefPackageCache.DeleteCurPackage(&OriGetOff,&NewGetOff);
            //LOG_INFO2("CDataCenter DeleteCurPackage:OriGetOff=%d NewGetOff=%d",OriGetOff,NewGetOff);

            if(tmpAbstractBusinessData.BID!=BID_RtnDepthMarketData)
            {
                LOG_INFO2("CDataCenter peek package, (BID=%d, Size=%d, CMDID=%d DataLen=%d GetOff=%d) DeleteCurPackage(OriGetOff=%d NewGetOff=%d)", 
                    tmpAbstractBusinessData.BID, tmpAbstractBusinessData.Size,tmpPackageHead.CmdID,tmpPackageHead.DataLen,tmpGetOff,
                    OriGetOff,NewGetOff);
            }


        }

        return NULL;
    };

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit()
    {
        m_DataCenter.m_pWorkThread = NULL;
    };

private:
    CDataCenter& m_DataCenter;
    //����Testʱ��¼/�طŲ������ݡ��ڶ��˺Ű汾�У����ⲻͬ�˺ŵ��߳�ͬʱ����
    static Ceasymutex m_mutex_Test_SaveLoadBusinessData;

    //��Test�ط�ʱ����¼��ʼ��¼��źͽ�����¼���
    int StartNo_Test_PlayBusinessData;
    int EndNo_Test_PlayBusinessData;

};
Ceasymutex CDataCenterWorkThread::m_mutex_Test_SaveLoadBusinessData;



CDataCenter::CDataCenter(CPackageCache& dataSource,CPlatFormService* pHost,int PlatformID,int MaxPlatformID)
:   m_dataSource(dataSource)
    ,m_pWriteLog(NULL)
    ,m_pWorkThread(NULL)
    ,m_pDataMgr(NULL)
    ,m_PlatFormService(pHost)
{
    bRspQrySettlementInfoConfirm=false;
    
    m_pWriteLog=new zqWriteLog();
    m_pWorkThread = NULL;
	m_pDataMgr=new CPlatformDataMgr(
        GlobalConfigManager::IsCalculate_OnRtnTrade(),
        GlobalConfigManager::m_bUseComb,
        GlobalConfigManager::m_DiscountCZCE,
        GlobalConfigManager::m_DiscountDCE,
        GlobalConfigManager::m_UnwindPrioritiesCZCE,
        GlobalConfigManager::m_UnwindPrioritiesDCE,
        GlobalConfigManager::m_PriceForMarginOfTodayPositionCZCE,
        GlobalConfigManager::m_PriceForMarginOfTodayPositionDCE,
        GlobalConfigManager::IsFreezeOrderUnknownStatus(),
        PlatformID,
        MaxPlatformID,
        GlobalConfigManager::m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible);

}

CDataCenter::~CDataCenter()
{
    Stop();

    //���浱������������Ϣ��commissionrate.xml��
    vector<PlatformStru_InstrumentCommissionRate> CommissionRates;
    m_pDataMgr->m_Rates.GetAllCommissionRates(CommissionRates);
    if(CommissionRates.size()>0)
        CTradingInfo_PlatformStru_InstrumentCommissionRate(GetServerParamCfg().CurSvrGrpID,string(GetTradingLoginInfo().UserID),string(GetTradingLoginInfo().TradingDay)).SaveXML(CommissionRates);

    //���浱�ձ�֤������Ϣ��marginrate.xml��
    vector<PlatformStru_InstrumentMarginRate> MarginRates;
    m_pDataMgr->m_Rates.GetAllMarginRates(MarginRates);
    if(MarginRates.size()>0)
        CTradingInfo_PlatformStru_InstrumentMarginRate(GetServerParamCfg().CurSvrGrpID,string(GetTradingLoginInfo().UserID),string(GetTradingLoginInfo().TradingDay)).SaveXML(MarginRates);

	SAFE_DELETE(m_pDataMgr);
    LOG_INFO("CDataCenter::~CDataCenter(����CDataCenter����)");
	SAFE_DELETE(m_pWriteLog);
}

///����ҵ������
int CDataCenter::SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo)
{
    if(BID>=BID_MAX || GID>=GID_MAX || callback==NULL)
        return -1;

    m_mutex.lock();
    int ret=0;
    try
    {
        m_subscribeInfo[BID][GID]=callback;
    }
    catch(...)
    {
        ret=-2;
    }
    m_mutex.unlock();

    if(ret<0) return ret;


    if(!sendCurrentInfo)
        return 0;

    switch(BID)
    {
        case BID_RtnDepthMarketData:
        {
            set<string> InstrumentSet;
            set<string>::iterator it;
            DataRtnDepthMarketData tmp;

            m_pDataMgr->m_Instruments.GetInstrumentList(InstrumentSet);
            if((int)InstrumentSet.size()>0)
            {
                for(it=InstrumentSet.begin();it!=InstrumentSet.end();it++)
                {
                    if(m_pDataMgr->m_Quots.GetQuotInfo(*it,tmp.DepthMarketDataField))
                    {
                        (callback)(GID, *(AbstractBusinessData*)(&tmp));
                    }
                }
            }
            break;
        }
        default:
            break;
    }

    return 0;
}

///ȡ��ҵ�����ݵĶ���
int CDataCenter::UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID)
{
    CeasymutexGuard guard(m_mutex);

    int ret=0;

    if(BID>=BID_MAX || GID>=GID_MAX)
        return -1;

    map<BusinessID, map<GUIModuleID, GuiCallBackFunc> >::iterator it1=m_subscribeInfo.find(BID);
    if(it1==m_subscribeInfo.end())
        ret=-3;
    else
    {
        map<GUIModuleID, GuiCallBackFunc>::iterator it2 = it1->second.find(GID);
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

    return ret;
}

///����
int CDataCenter::Start()
{
    LOG_INFO("Start");
    if(m_pWorkThread)
        return 0;

    m_pWorkThread = new CDataCenterWorkThread(*this);
    if(m_pWorkThread->Create(512*1024) != wxTHREAD_NO_ERROR)
        LOG_INFO("Start:����DataCenter���ڲ������߳�ʧ��");
    else
        LOG_INFO("Start:����DataCenter���ڲ������̳߳ɹ�");

    if(m_pWorkThread->Run() != wxTHREAD_NO_ERROR)
    {
         LOG_INFO("Start:����DataCenter���ڲ������߳�ʧ��!");
         return -1;
    }
    else
    {
         LOG_INFO("Start:����DataCenter���ڲ������̳߳ɹ�!");
    }

    return 0;
}

///ֹͣ,������Ϣ�������ٴ�����֮�󣬶�����Ȼ��Ч
int CDataCenter::Stop()
{
    LOG_INFO("Stop");
    if(m_pWorkThread)
    {
        LOG_INFO("Stop:�ر�DataCenter���ڲ������߳�");
        m_pWorkThread->Delete();
        m_pWorkThread=NULL; //will also be set NULL in calling CDataCenterWorkThread::OnExit()
    }

    return 0;
}

int CDataCenter::doProcessBusinessData(AbstractBusinessData& data)
{
    //if(data.BID!=BID_RtnDepthMarketData)
    //    LOG_INFO("CDataCenter::doProcessBusinessData, BID=%d, Size=%d", data.BID, data.Size);

    DWORD t1,t2,t3;
    t1=GetTickCount();

    bool toBeContinue=true;
    switch(data.BID)
    {
        case BID_RtnDepthMarketData:    //�������
        {
            DataRtnDepthMarketData& NewBusiness= (DataRtnDepthMarketData&)data;   
            string strInstrument(NewBusiness.DepthMarketDataField.InstrumentID);
            if(strInstrument.empty()) toBeContinue=false;
            else 
            {
                //�ڽṹ�б�����Լ�Ƿ�����Ч�ֲ֡��˴�ֻ�ܵ��ȳֲ֣���ϳֲֲ�����ӯ�������ù�
                NewBusiness.bHavePosition=m_pDataMgr->m_PositionDetails.HavePositionDetail(strInstrument);
                m_pDataMgr->OnRtnDepthMarketData(NewBusiness.DepthMarketDataField);
            }
            break;
        }
        case BID_RspQryDepthMarketData://�����ѯ������Ӧ������Ϻ�Լ������Ҫ�����������ʱ���ᴥ����ѯ���Ⱥ�Լ������
        {
            DataRspQryDepthMarketData& NewBusiness= (DataRspQryDepthMarketData&)data;
            if(NewBusiness.RspInfoField.ErrorID==0)
            {
                m_pDataMgr->OnRspQryDepthMarketData(NewBusiness.DepthMarketDataField);
            }
            break;
        }
        case BID_RtnOrder://�����ر�
        {
			DataRtnOrder& NewBusiness= (DataRtnOrder&)data;

            string strInstrumentID(NewBusiness.OrderField.InstrumentID);
            if(strInstrumentID.empty()) break;
            PlatformStru_InstrumentInfo InstrumentInfo;
            if(!m_pDataMgr->m_Instruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo))
		        break;

            m_pDataMgr->OnRtnOrder(NewBusiness.OrderField);

            //����Ƿ�����Ϻ�Լ����
            string Leg1InstrumentID,Leg2InstrumentID;
            bool bComb = InstrumentInfo.ProductClass==THOST_FTDC_PC_Combination;
            if(bComb) m_pDataMgr->m_Instruments.GetLegInstrumentID(strInstrumentID,Leg1InstrumentID,Leg2InstrumentID);

            //������Ҫ������ѯ���ʡ��������Ϻ�Լ�������Ȳ�ѯ
            if(bComb)
            {
		        m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(Leg1InstrumentID);
		        m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(Leg1InstrumentID);
		        m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(Leg2InstrumentID);
		        m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(Leg2InstrumentID);
            }
            else
            {
		        m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(strInstrumentID);
		        m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(strInstrumentID);
            }

            //��ϱ������㶳�ᱣ֤�����������Ҫ�õ�������������ۣ����û�����飬������ѯ
            if(bComb)
            {
                if(util::isInvalidValue(NewBusiness.OrderField.freezeMargin)||
                   util::isInvalidValue(NewBusiness.OrderField.troubleMoney))
                {
                    PlatformStru_DepthMarketData LegQuotData;
                    if(!m_pDataMgr->m_Quots.GetQuotInfo(Leg1InstrumentID,LegQuotData))
                    {
                        PlatformStru_QryDepthMarketData QryDepthMarketData;
                        strncpy(QryDepthMarketData.Thost.InstrumentID,Leg1InstrumentID.c_str(),sizeof(QryDepthMarketData.Thost.InstrumentID)-1);
                        m_PlatFormService->m_rServiceProxy.ReqQryDepthMarketData(QryDepthMarketData);
                    }
                    if(!m_pDataMgr->m_Quots.GetQuotInfo(Leg2InstrumentID,LegQuotData))
                    {
                        PlatformStru_QryDepthMarketData QryDepthMarketData;
                        strncpy(QryDepthMarketData.Thost.InstrumentID,Leg2InstrumentID.c_str(),sizeof(QryDepthMarketData.Thost.InstrumentID)-1);
                        m_PlatFormService->m_rServiceProxy.ReqQryDepthMarketData(QryDepthMarketData);
                    }
                }
            }
            else if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Options||InstrumentInfo.ProductClass==THOST_FTDC_PC_SpotOption)
            {
                //��Ȩ���ᱣ֤�����ҲҪ�õ�����
                if(util::isInvalidValue(NewBusiness.OrderField.freezeMargin))
                {
                    PlatformStru_DepthMarketData QuotData;
                    if(!m_pDataMgr->m_Quots.GetQuotInfo(strInstrumentID,QuotData))
                    {
                        PlatformStru_QryDepthMarketData QryDepthMarketData;
                        strncpy(QryDepthMarketData.Thost.InstrumentID,strInstrumentID.c_str(),sizeof(QryDepthMarketData.Thost.InstrumentID)-1);
                        m_PlatFormService->m_rServiceProxy.ReqQryDepthMarketData(QryDepthMarketData);
                    }
                }
            }

            //���δ�ɽ��Լ�״̬δ֪�ı�����������
            if(m_pDataMgr->m_Orders.IsOrderWorking(NewBusiness.OrderField))
            {
                m_PlatFormService->SetSubscribeStatus(strInstrumentID,GID_PlatformSvr);
                if(bComb)
                {
                    //��ϵ���Ҫ����˫������
                    m_PlatFormService->SetSubscribeStatus(Leg1InstrumentID,GID_PlatformSvr);
                    m_PlatFormService->SetSubscribeStatus(Leg2InstrumentID,GID_PlatformSvr);
                }
            }
            break;
        }
        case BID_RtnTrade://�ɽ��ر�
        {
			DataRtnTrade& NewBusiness= (DataRtnTrade&)data;

            string strInstrumentID(NewBusiness.TradeField.InstrumentID);
            if(strInstrumentID.empty()) break;
            PlatformStru_InstrumentInfo InstrumentInfo;
            if(!m_pDataMgr->m_Instruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo))
		        break;

            m_pDataMgr->OnRtnTrade(NewBusiness.TradeField);

            //������ѯ����
	        m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(strInstrumentID);
	        m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(strInstrumentID);

            //�������ָ���˳ɽ�����Ҫ��ѯ�����Ϣ��ִ�в�ѯ����
            //����ڳ�ʼ����ѯ�����У�����гɽ��ر�������Ҳ������غ�Լ�Ĳ�ѯ������©����������
            if(GlobalConfigManager::IsReq_OnRtnTrade()||
                m_PlatFormService->m_rServiceProxy.m_bInitQryStart)
            {
                CReqOnRtnTrade::OnRtnTrade(string(NewBusiness.TradeField.BrokerID),
                    string(NewBusiness.TradeField.InvestorID),
                    string(NewBusiness.TradeField.InstrumentID),
                    (NewBusiness.TradeField.TradeType==THOST_FTDC_TRDT_CombinationDerived?true:false),
                    m_pDataMgr->m_PositionDetails.GetRecordCountOfPositionDetail());
            }
            break;
        }
        case BID_RspQryProduct://�����ѯ��Ʒ��Ӧ
        {
			DataRspQryProduct& NewBusiness= (DataRspQryProduct&)data; 
			if(NewBusiness.RspInfoField.ErrorID==0)
			{
				if(NewBusiness.ProductField.ProductID[0]!=0)
				{
                    m_pDataMgr->OnRspQryProduct(NewBusiness.ProductField);
				}
            }
            break;
        }
        case BID_RspQryInstrument://�����ѯ��Լ��Ӧ
        {
			DataRspQryInstrument& NewBusiness= (DataRspQryInstrument&)data; 
			if(NewBusiness.RspInfoField.ErrorID==0)
			{
				if(NewBusiness.InstrumentField.InstrumentID[0]!=0)
				{
                    bool bCTPSimulate=false;
                    if((int)m_pDataMgr->m_ServerParamCfg.vec.size()>0&&
                        m_pDataMgr->m_ServerParamCfg.CurSvrGrpID<(int)m_pDataMgr->m_ServerParamCfg.vec.size()&&
                        m_pDataMgr->m_ServerParamCfg.vec[m_pDataMgr->m_ServerParamCfg.CurSvrGrpID].CTPSimulate==1)
                        bCTPSimulate=true;

                    m_pDataMgr->OnRspQryInstrument(NewBusiness.InstrumentField,bCTPSimulate,NewBusiness.bIsLast);

                    if(NewBusiness.bIsLast)
                    {
                        //���浱�պ�Լ��Ϣ��instrument.xml��
                        vector<PlatformStru_InstrumentInfo> InstrumentInfos;
                        m_pDataMgr->m_Instruments.GetInstrumentInfos(InstrumentInfos);
                        CTradingInfo_PlatformStru_InstrumentInfo(GetServerParamCfg().CurSvrGrpID,string(GetTradingLoginInfo().UserID),string(GetTradingLoginInfo().TradingDay)).SaveXML(InstrumentInfos);

                        //�Ӳ�Ʒ�б���ɾ��û�к�Լ�Ĳ�Ʒ
                        m_pDataMgr->m_Instruments.DeleteEmptyProduct();

                        //���浱��Ʒ����Ϣ��product.xml��
                        vector<PlatformStru_ProductInfo> ProductInfos;
                        m_pDataMgr->m_Instruments.GetProductInfos(ProductInfos);
                        CTradingInfo_PlatformStru_ProductInfo(GetServerParamCfg().CurSvrGrpID,string(GetTradingLoginInfo().UserID),string(GetTradingLoginInfo().TradingDay)).SaveXML(ProductInfos);
                    }
				}
            }
            break;
        }
        case BID_RspQryInvestorPosition://�����ѯͶ���ֲ߳���Ӧ
        {
            DataRspQryInvestorPosition& NewBusiness= (DataRspQryInvestorPosition&)data; 
            if(NewBusiness.RspInfoField.ErrorID==0||NewBusiness.RspInfoField.ErrorID==CustomErrorID||NewBusiness.bIsLast)
            {
                //��Ҫ��ѯ�ĺ�ԼID����������������ȥ
                string QryInstrument;
                m_pDataMgr->m_Positions.GetQryPositionInstrument(QryInstrument);
                strncpy(NewBusiness.QryInstrumentID,QryInstrument.c_str(),sizeof(NewBusiness.QryInstrumentID)-1);

                m_pDataMgr->OnRspQryInvestorPosition(NewBusiness.InvestorPositionField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);
            }
            break;
        }
        case BID_RspQryInvestorPositionDetail://�����ѯͶ���ֲ߳���ϸ��Ӧ
        {
            DataRspQryInvestorPositionDetail& NewBusiness= (DataRspQryInvestorPositionDetail&)data; 
            if(NewBusiness.RspInfoField.ErrorID==0||NewBusiness.RspInfoField.ErrorID==CustomErrorID)
            {
                //��Ҫ��ѯ�ĺ�ԼID����������������ȥ
                string QryInstrument;
                m_pDataMgr->m_PositionDetails.GetQryPositionDetailInstrument(QryInstrument);
                strncpy(NewBusiness.QryInstrumentID,QryInstrument.c_str(),sizeof(NewBusiness.QryInstrumentID)-1);

                m_pDataMgr->OnRspQryInvestorPositionDetail(NewBusiness.InvestorPositionDetailField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);

                //������Ҫ�������ʲ�ѯ��������ϳֲֵĳֲ���ϸ�������Ƿ��ȷ��صģ���˲���Ҫ�����⴦��
                if(NewBusiness.bIsLast)
                {
                    set<string> setInstruments;
                    m_pDataMgr->m_PositionDetails.GetInstrumentSetOfQryRlt(setInstruments);
                    if(!setInstruments.empty())
                    {
                        set<string>::iterator it_Instrument;
                        for(it_Instrument=setInstruments.begin();it_Instrument!=setInstruments.end();it_Instrument++)
                        {
			                m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(*it_Instrument);
			                m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(*it_Instrument);
                        }
                    }
                }

                m_PlatFormService->SetSubscribeStatus(NewBusiness.InvestorPositionDetailField.InstrumentID,GID_PlatformSvr);
            }
            break;
        }
        case BID_RspQryInvestorPositionCombDetail://�����ѯͶ������ϳֲ���ϸ��Ӧ
        {
            DataRspQryInvestorPositionCombDetail& NewBusiness= (DataRspQryInvestorPositionCombDetail&)data; 
			if (NewBusiness.RspInfoField.ErrorID == 0 || NewBusiness.RspInfoField.ErrorID == CustomErrorID)
			{
                //��Ҫ��ѯ�ĺ�ԼID����������������ȥ
                string QryInstrument;
                m_pDataMgr->m_PositionCombs.GetQryPositionCombInstrument(QryInstrument);
                strncpy(NewBusiness.QryInstrumentID,QryInstrument.c_str(),sizeof(NewBusiness.QryInstrumentID)-1);

                m_pDataMgr->OnRspQryInvestorPositionCombDetail(NewBusiness.InvestorPositionCombineDetailField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);
			}
			break;
        }
        case BID_RspQryTrade://�����ѯ�ɽ���Ӧ
        {
            DataRspQryTrade& NewBusiness= (DataRspQryTrade&)data;
            if(NewBusiness.RspInfoField.ErrorID == 0||NewBusiness.RspInfoField.ErrorID==CustomErrorID||NewBusiness.bIsLast)
            {
                m_pDataMgr->OnRspQryTrade(NewBusiness.TradeField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);
            }
            break;
        }
        case BID_RspQryOrder://�����ѯ������Ӧ
        {
            DataRspQryOrder& NewBusiness= (DataRspQryOrder&)data;
            if(NewBusiness.RspInfoField.ErrorID==0||NewBusiness.RspInfoField.ErrorID==CustomErrorID)
            {
                m_pDataMgr->OnRspQryOrder(NewBusiness.OrderField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);

                if(NewBusiness.bIsLast)
                {
                    //����Ҫ������ѯ���ʣ���������
                    set<string> setInstruments;
                    m_pDataMgr->m_Orders.GetInstrumentIDs_Working(setInstruments);
                    set<string>::iterator it_Instrument;
                    for(it_Instrument=setInstruments.begin();it_Instrument!=setInstruments.end();it_Instrument++)
                    {
                        m_PlatFormService->SetSubscribeStatus(*it_Instrument,GID_PlatformSvr);

                        //�������Ϻ�Լ�������Ȳ�ѯ
                        string Leg1InstrumentID,Leg2InstrumentID;
                        if(GlobalFunc::IsCombInstrument(*it_Instrument,Leg1InstrumentID,Leg2InstrumentID))
                        {
		                    m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(Leg1InstrumentID);
		                    m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(Leg1InstrumentID);
		                    m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(Leg2InstrumentID);
		                    m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(Leg2InstrumentID);

                            //��ϵ���Ҫ����˫������
                            m_PlatFormService->SetSubscribeStatus(Leg1InstrumentID,GID_PlatformSvr);
                            m_PlatFormService->SetSubscribeStatus(Leg2InstrumentID,GID_PlatformSvr);
                        }
                        else
                        {
		                    m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(*it_Instrument);
		                    m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(*it_Instrument);
                        }
                    }

                    //ȡ�ȴ�������ж��ᱣ֤��������Ѽ���ĺ�Լ������ѯ����
                    m_pDataMgr->m_Orders.GetInstrumentIDs_WaitQuot(setInstruments);
                    for(it_Instrument=setInstruments.begin();it_Instrument!=setInstruments.end();it_Instrument++)
                    {
                        PlatformStru_QryDepthMarketData QryDepthMarketData;
                        strncpy(QryDepthMarketData.Thost.InstrumentID,it_Instrument->c_str(),sizeof(QryDepthMarketData.Thost.InstrumentID)-1);
                        m_PlatFormService->m_rServiceProxy.ReqQryDepthMarketData(QryDepthMarketData);
                    }
                }
            }
            break;        
        }
        case BID_RspOrderInsert://����¼��ر�
        {
            DataRspOrderInsert& NewBusiness= (DataRspOrderInsert&)data;
      //      if(NewBusiness.RspInfoField.ErrorID!=0)
      //      {
			   //m_PlatFormService->SetSubscribeStatus(NewBusiness.InputOrderField.InstrumentID,S_STATUS_ORDERINSERT);
      //      }
            break;  
        }
		case BID_ErrRtnOrderInsert://����¼��ر�
		{
			DataErrRtnOrderInsert& NewBusiness= (DataErrRtnOrderInsert&)data;
			//if(NewBusiness.RspInfoField.ErrorID!=0)
			//{
			//	m_PlatFormService->SetSubscribeStatus(NewBusiness.InputOrderField.InstrumentID,S_STATUS_ORDERINSERT);
			//}
		    break;  
		}
        case BID_RspQryTradingAccount://��ѯ�����ʽ��˻���Ϣ��Ӧ
        {
            DataRspQryTradingAccount& NewBusiness= (DataRspQryTradingAccount&)data;
            if(NewBusiness.RspInfoField.ErrorID==0)
            {
                m_pDataMgr->OnRspQryTradingAccount(NewBusiness.TradingAccountField);
            }
            break;
        }
        case BID_RspQryInvestor://�����ѯͶ������Ӧ
        {
            DataRspQryInvestor& NewBusiness= (DataRspQryInvestor&)data;
            if(NewBusiness.RspInfoField.ErrorID==0)
            {
                m_pDataMgr->SetInvestorInfo(NewBusiness.InvestorField);
            }
            break;
        }
        case BID_RspQryInstrumentMarginRate://�����ѯ��Լ��֤������Ӧ
        {
            DataRspQryInstrumentMarginRate& NewBusiness= (DataRspQryInstrumentMarginRate&)data;
            m_pDataMgr->OnRspQryInstrumentMarginRate(NewBusiness.InstrumentMarginRateField,NewBusiness.RspInfoField.ErrorID);
            break;
        }
        case BID_RspQryInstrumentCommissionRate://�����ѯ��Լ����������Ӧ
        {
            DataRspQryInstrumentCommissionRate& NewBusiness= (DataRspQryInstrumentCommissionRate&)data;

            if(NewBusiness.RspInfoField.ErrorID!=0)
            {
                //���������DataMgr�д���
                m_pDataMgr->OnRspQryInstrumentCommissionRate(NewBusiness.InstrumentCommissionRateField,NewBusiness.RspInfoField.ErrorID);
            }
            else
            {
                string strInstrument(NewBusiness.InstrumentCommissionRateField.InstrumentID);
                if(strInstrument.empty()) break;
                set<string> InstrumentList;
                m_pDataMgr->m_Instruments.GetInstrumentListByProductID(strInstrument,InstrumentList);
                if((int)InstrumentList.size()>0)
                {
                    //��Ʒ�ִ���
                    for(set<string>::iterator it2=InstrumentList.begin(); it2!=InstrumentList.end(); ++it2)
                    {
                        PlatformStru_InstrumentCommissionRate Rate2=NewBusiness.InstrumentCommissionRateField;
                        CTools_Ansi::mymemcpy_trim(Rate2.InstrumentID,sizeof(Rate2.InstrumentID),it2->c_str());
                        m_pDataMgr->OnRspQryInstrumentCommissionRate(Rate2,0);
                    }
                }
                else
                {
                    //�Ǻ�Լ����
                    m_pDataMgr->OnRspQryInstrumentCommissionRate(NewBusiness.InstrumentCommissionRateField,0);
                }
            }
            break;
        }
        case BID_RspQryOptionInstrCommRate://�����ѯ��Ȩ��Լ��������Ӧ
        {
            DataRspQryOptionInstrCommRate& NewBusiness= (DataRspQryOptionInstrCommRate&)data;

            if(NewBusiness.RspInfoField.ErrorID!=0)
            {
                //���������DataMgr�д���
                m_pDataMgr->OnRspQryInstrumentCommissionRate(NewBusiness.InstrumentCommissionRateField,NewBusiness.RspInfoField.ErrorID);
            }
            else
            {
                string strInstrument(NewBusiness.InstrumentCommissionRateField.InstrumentID);
                if(strInstrument.empty()) break;
                set<string> InstrumentList;
                m_pDataMgr->m_Instruments.GetInstrumentListByProductID(strInstrument,InstrumentList);
                if((int)InstrumentList.size()>0)
                {
                    //��Ʒ�ִ���
                    for(set<string>::iterator it2=InstrumentList.begin(); it2!=InstrumentList.end(); ++it2)
                    {
                        PlatformStru_InstrumentCommissionRate Rate2=NewBusiness.InstrumentCommissionRateField;
                        CTools_Ansi::mymemcpy_trim(Rate2.InstrumentID,sizeof(Rate2.InstrumentID),it2->c_str());
                        m_pDataMgr->OnRspQryInstrumentCommissionRate(Rate2,0);
                    }
                }
                else
                {
                    //�Ǻ�Լ����
                    m_pDataMgr->OnRspQryInstrumentCommissionRate(NewBusiness.InstrumentCommissionRateField,0);
                }
            }
            break;
        }
        case BID_RspQryOptionInstrTradeCost://�����ѯ��Ȩ���׳ɱ���Ӧ
        {
            DataRspQryOptionInstrTradeCost& NewBusiness= (DataRspQryOptionInstrTradeCost&)data;
            m_pDataMgr->OnRspQryInstrumentMarginRate(NewBusiness.InstrumentMarginRateField,NewBusiness.RspInfoField.ErrorID);
            break;
        }
        case BID_RspQrySettlementInfo://�����ѯͶ���߽�������Ӧ
        {
            DataRspQrySettlementInfo& NewBusiness= (DataRspQrySettlementInfo&)data;
            if(NewBusiness.RspInfoField.ErrorID==0)
            {
                m_pDataMgr->AppendLastSettlementInfo(string(NewBusiness.SettlementInfoField.Content),NewBusiness.bIsLast);
            }
            else
            {
                m_pDataMgr->ClearLastSettlementInfo();
            }
            break;
        }
        case BID_RspUserLogin://������Ӧ����Ҫ��¼��������ʱ��
        {
            DataRspUserLogin& NewBusiness= (DataRspUserLogin&)data;
            if(NewBusiness.RspInfoField.ErrorID==0 && NewBusiness.bTrader)
            {
                //��¼������ʱ��
                ExchangeTime exchangeTime;
                m_pDataMgr->m_Instruments.GetExchangeTime(exchangeTime);
                exchangeTime.CurTickCount = NewBusiness.CurTickCount;
                memcpy(&exchangeTime.TradingDay, &NewBusiness.UserLoginField.TradingDay, sizeof(exchangeTime.TradingDay));
                memcpy(&exchangeTime.SHFETime, &NewBusiness.UserLoginField.SHFETime, sizeof(exchangeTime.SHFETime));
                memcpy(&exchangeTime.DCETime, &NewBusiness.UserLoginField.DCETime, sizeof(exchangeTime.DCETime));
                memcpy(&exchangeTime.CZCETime, &NewBusiness.UserLoginField.CZCETime, sizeof(exchangeTime.CZCETime));
                memcpy(&exchangeTime.FFEXTime, &NewBusiness.UserLoginField.FFEXTime, sizeof(exchangeTime.FFEXTime));
                m_pDataMgr->m_Instruments.SetExchangeTime(exchangeTime);

                m_pDataMgr->m_LoginInfo.SetTradingLoginInfo(PlatformStru_LoginInfo(NewBusiness.UserLoginField));


                //Loadʱ������xml�еĻ��������Ƿ��ǵ���ģ�1. ������ǵ�ǰ�����գ�����������ļ�; 2. ���û��SvrGrpIDxxx-Accountyyy�ڣ��򴴽�֮
                //���Դ�tradingparam.xml��ȡ��Ʒ����Ϣ
                CTradingInfo_PlatformStru_BrokerTradingParams BrokerTradingParams(GetServerParamCfg().CurSvrGrpID,string(GetTradingLoginInfo().UserID),string(GetTradingLoginInfo().TradingDay));
                BrokerTradingParams.Load();
                if(BrokerTradingParams.m_bValid&&BrokerTradingParams.m_LineCnt>0)
                {
                    m_pDataMgr->SetBrokerTradingParam(BrokerTradingParams.m_pLineData[0]);
                }

                //���Դ�product.xml��ȡ��Ʒ����Ϣ
                CTradingInfo_PlatformStru_ProductInfo ProductInfo(GetServerParamCfg().CurSvrGrpID,string(GetTradingLoginInfo().UserID),string(GetTradingLoginInfo().TradingDay));
                ProductInfo.Load();
                if(ProductInfo.m_bValid&&ProductInfo.m_LineCnt>0)
                {
                    for(int i=0;i<(int)ProductInfo.m_LineCnt;i++)
                        m_pDataMgr->m_Instruments.SetProductInfo(ProductInfo.m_pLineData[i].ProductID,ProductInfo.m_pLineData[i]);
                }
                //���Դ�instrument.xml��ȡ����Լ��Ϣ
                CTradingInfo_PlatformStru_InstrumentInfo InstrumentInfo(GetServerParamCfg().CurSvrGrpID,string(GetTradingLoginInfo().UserID),string(GetTradingLoginInfo().TradingDay));
                InstrumentInfo.Load();
                if(InstrumentInfo.m_bValid&&InstrumentInfo.m_LineCnt>0)
                {
                    bool bCTPSimulate=false;
                    if((int)m_pDataMgr->m_ServerParamCfg.vec.size()>0&&
                        m_pDataMgr->m_ServerParamCfg.CurSvrGrpID<(int)m_pDataMgr->m_ServerParamCfg.vec.size()&&
                        m_pDataMgr->m_ServerParamCfg.vec[m_pDataMgr->m_ServerParamCfg.CurSvrGrpID].CTPSimulate==1)
                        bCTPSimulate=true;

                    for(int i=0;i<(int)InstrumentInfo.m_LineCnt;i++)
                        m_pDataMgr->m_Instruments.SetInstrumentInfo(InstrumentInfo.m_pLineData[i].InstrumentID,InstrumentInfo.m_pLineData[i],bCTPSimulate,i==InstrumentInfo.m_LineCnt-1);
                }

                //���Դ�commissionrate.xml��ȡ������������Ϣ
                CTradingInfo_PlatformStru_InstrumentCommissionRate InstrumentCommissionRate(GetServerParamCfg().CurSvrGrpID,string(GetTradingLoginInfo().UserID),string(GetTradingLoginInfo().TradingDay));
                InstrumentCommissionRate.Load();
                if(InstrumentCommissionRate.m_bValid&&InstrumentCommissionRate.m_LineCnt>0)
                {
                    for(int i=0;i<(int)InstrumentCommissionRate.m_LineCnt;i++)
                        m_pDataMgr->m_Rates.SetCommissionRate(InstrumentCommissionRate.m_pLineData[i].InstrumentID,InstrumentCommissionRate.m_pLineData[i]);
                }

                //���Դ�marginrate.xml��ȡ����֤������Ϣ
                CTradingInfo_PlatformStru_InstrumentMarginRate InstrumentMarginRate(GetServerParamCfg().CurSvrGrpID,string(GetTradingLoginInfo().UserID),string(GetTradingLoginInfo().TradingDay));
                InstrumentMarginRate.Load();
                if(InstrumentMarginRate.m_bValid&&InstrumentMarginRate.m_LineCnt>0)
                {
                    for(int i=0;i<(int)InstrumentMarginRate.m_LineCnt;i++)
                        m_pDataMgr->m_Rates.SetMarginRate(InstrumentMarginRate.m_pLineData[i].InstrumentID,InstrumentMarginRate.m_pLineData[i]);
                }

            }
            break;
        }
		case BID_RspQryContractBank://�����ѯǩԼ������Ӧ
        {
            DataRspQryContractBank& pTmp= (DataRspQryContractBank&)data;
            //CThostFtdcContractBankField* pRawData= &pTmp->ContractBankField;
            if(pTmp.RspInfoField.ErrorID == 0)
            {   

                m_pDataMgr->SaveBankName(pTmp.ContractBankField);
				PlatformStru_QryAccountRegister  QryAccountregister;
				memset(&QryAccountregister,0,sizeof(QryAccountregister));
				if(pTmp.bIsLast==1)
				{
					if( m_PlatFormService )
						m_PlatFormService->m_rServiceProxy.ReqQryAccountregister(QryAccountregister);
				}
            }
            break;
        }
		
		case   BID_RspQryAccountregister://�����ѯ����ǩԼ��ϵ��Ӧ
        {
            DataRspQryAccountregister& pTmp= (DataRspQryAccountregister&)data;
            CThostFtdcAccountregisterField* pRawData= &pTmp.Accountregister;
            if(pTmp.RspInfoField.ErrorID == 0)
            {   
               
                m_pDataMgr->UpdateContractBank(pTmp.Accountregister);
            }
            break;
        }

        case BID_RspQryExchangeRate:        //�����ѯ���ʷ���
        {
            DataRspQryExchangeRate& Tmp = (DataRspQryExchangeRate&)data;
            if(Tmp.RspInfoField.ErrorID == 0)
            {
                PlatformStru_ExchangeRate exchangerate;
                strncpy(exchangerate.BrokerID,Tmp.ExchangeRateField.BrokerID,sizeof(exchangerate.BrokerID)-1);
                strncpy(exchangerate.FromCurrencyID,Tmp.ExchangeRateField.FromCurrencyID,sizeof(exchangerate.FromCurrencyID)-1);
                strncpy(exchangerate.ToCurrencyID,Tmp.ExchangeRateField.ToCurrencyID,sizeof(exchangerate.ToCurrencyID)-1);
                exchangerate.FromCurrencyUnit=Tmp.ExchangeRateField.FromCurrencyUnit;
                exchangerate.ExchangeRate=Tmp.ExchangeRateField.ExchangeRate;
                m_pDataMgr->SaveExchangeRate(exchangerate);
            }
            break;
        }


        case BID_RspParkedOrderInsert:       //Ԥ��¼��������Ӧ
        case BID_RspParkedOrderAction:       //Ԥ�񳷵�¼��������Ӧ
        case BID_RspRemoveParkedOrder:       //ɾ��Ԥ����Ӧ    
        case BID_RspRemoveParkedOrderAction: //ɾ��Ԥ�񳷵���Ӧ
        case BID_RspQryParkedOrder:          //�����ѯԤ����Ӧ
        case BID_RspQryParkedOrderAction:    //�����ѯԤ�񳷵���Ӧ
        {
            break;
        }

        case BID_RspQrySettlementInfoConfirm://RunMode==2/3ʱ����ѯ�������Ϣȷ�Ͻ���󣬲�ѯ���͹�˾���ײ����Լ�Ʒ�֡���Լ�����ʵ�
        {
            if(GlobalConfigManager::m_RunMode==2||GlobalConfigManager::m_RunMode==3)
            {
                if(!m_pDataMgr->IsHaveBrokerTradingParam())
                {
                    //������Ϣδ���棬������ѯ
                    m_PlatFormService->m_rServiceProxy.ReqQryBrokerTradingParams();
                }
            }
            break;
        }
        case BID_RspQryBrokerTradingParams:   //�����ѯ���͹�˾���ײ�����Ӧ
        {
            DataRspQryBrokerTradingParams& Tmp = (DataRspQryBrokerTradingParams&)data;
            if(Tmp.RspInfoField.ErrorID == 0)
            {
                m_pDataMgr->SetBrokerTradingParam(Tmp.BrokerTradingParams);

                //���浱�վ��͹�˾���ײ�����Ϣ��tradingparam.xml��
                CTradingInfo_PlatformStru_BrokerTradingParams(GetServerParamCfg().CurSvrGrpID,string(GetTradingLoginInfo().UserID),string(GetTradingLoginInfo().TradingDay)).SaveXML(Tmp.BrokerTradingParams);
            }
            break;
        }


        default:
        break;                  
    }


    if(!toBeContinue)
        return -1;


    t2=GetTickCount();

    map<GUIModuleID,GuiCallBackFunc> CallbackFuncList;
    m_mutex.lock();
    map<BusinessID, map<GUIModuleID, GuiCallBackFunc> >::iterator it1=m_subscribeInfo.find(data.BID);
    if(it1!=m_subscribeInfo.end())
        CallbackFuncList=it1->second;
    m_mutex.unlock();

    if((int)CallbackFuncList.size()>0)
    {
        data.PlatformID = m_PlatFormService->m_rServiceProxy.GetPlatformID();

        map<GUIModuleID,GuiCallBackFunc>::iterator it_callback;
        for(it_callback=CallbackFuncList.begin();it_callback!=CallbackFuncList.end();it_callback++)
        {
            if(it_callback->second)
                (it_callback->second)(it_callback->first,data);
        }
    }


  //  if(m_pDataMgr->GetSubscribeBusinessData(data.BID,CallbackFuncList)==0)
  //  {
		//data.PlatformID = m_PlatFormService->GetPlatFormID();
  //      int i;
  //      for(i=0,it_callback=CallbackFuncList.begin();it_callback!=CallbackFuncList.end();it_callback++,i++)
  //      {
  //          if(it_callback->second)
  //              (it_callback->second)(it_callback->first,data);
  //      }
  //  }

    t3=GetTickCount();


    //char Buf[512];
    //sprintf(Buf,"doProcessBusinessData(%03d): - \t"
    //            "t1: %03u \t"
    //            "t2: %03u \t"
    //            "Total: %03u    ----", 
    //            data.BID,
    //            t2-t1,t3-t2,t3-t1);
//    OutputDebugString(Buf);

    return 0;  
}

////��xml����ȡ�����͹�˾���ײ���
////		<BrokerTradingParams Algorithm="52"  AvailIncludeCloseProfit="48"  CurrencyID=""  MarginPriceType="50"  OptionRoyaltyPriceType="49"  />
//void CDataCenter::GetBrokerTradingParamsFromXML(CMyXML& nodexml)
//{
//    PlatformStru_BrokerTradingParams BrokerTradingParams;
//    strncpy(BrokerTradingParams.BrokerID,GetTradingLoginInfo().BrokerID,sizeof(BrokerTradingParams.BrokerID)-1);
//    strncpy(BrokerTradingParams.InvestorID,GetTradingLoginInfo().UserID,sizeof(BrokerTradingParams.InvestorID)-1);
//    BrokerTradingParams.MarginPriceType=(char)CTools_Ansi::GetIntegerFromString(nodexml.GetNodeAttrValue2(string(""),"MarginPriceType"));
//    BrokerTradingParams.Algorithm=(char)CTools_Ansi::GetIntegerFromString(nodexml.GetNodeAttrValue2(string(""),"Algorithm"));
//    BrokerTradingParams.AvailIncludeCloseProfit=(char)CTools_Ansi::GetIntegerFromString(nodexml.GetNodeAttrValue2(string(""),"AvailIncludeCloseProfit"));
//    strncpy(BrokerTradingParams.CurrencyID,nodexml.GetNodeAttrValue2(string(""),"CurrencyID").c_str(),sizeof(BrokerTradingParams.CurrencyID)-1);
//    BrokerTradingParams.OptionRoyaltyPriceType=(char)CTools_Ansi::GetIntegerFromString(nodexml.GetNodeAttrValue2(string(""),"OptionRoyaltyPriceType"));
//
//    m_pDataMgr->SetBrokerTradingParam(BrokerTradingParams);
//}



///��ȡ��Լ�б�
int CDataCenter::GetInstrumentList(vector<InstrumentGroupInfo> &outData)
{
    m_pDataMgr->m_Instruments.GetInstrumentList(outData);
    return outData.size();
}
///��ȡ��Լ�б�
int CDataCenter::GetInstrumentList(set<string> &outData,string ExchangeID)
{
    m_pDataMgr->m_Instruments.GetInstrumentList(outData,ExchangeID);
    return outData.size();
}
int CDataCenter::GetInstrumentList(vector<string> &outData,string ExchangeID)
{
    m_pDataMgr->m_Instruments.GetInstrumentList(outData,ExchangeID);
    return outData.size();
}
//���������Լ�б�
void CDataCenter::AddMainInstrumentList(string instrument)
{
	 m_pDataMgr->m_Instruments.AddMainInstrumentList(instrument);
}
///��ȡ������Լ�б�
int CDataCenter::GetMainInstrumentList(set<string> &outData)
{
    return m_pDataMgr->m_Instruments.GetMainInstrumentList(outData);
}
///���ú�Լ�ݲ��б�
void CDataCenter::SetInstrumentVarietyMap(map<string, string>& inData)
{
    m_pDataMgr->m_Instruments.SetInstrumentVarietyMap(inData);
}

// ��ȡ��Լ�ݲ��б�
int CDataCenter::GetInstrumentVarietyMap(map<string, string>& outData)
{
    m_pDataMgr->m_Instruments.GetInstrumentVarietyMap(outData);
    return outData.size();
}

///��ȡָ����Լ��Ϣ
int CDataCenter::GetInstrumentInfo(const string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    bool brlt=m_pDataMgr->m_Instruments.GetInstrumentInfo(InstrumentID,outData);
    return brlt?0:-1;
}
bool CDataCenter::GetLegInstrumentID(const string& strComInstrumentID,
                                     string& strLeg1InstrumentID,
                                     string& strLeg2InstrumentID)
{
    return m_pDataMgr->m_Instruments.GetLegInstrumentID(strComInstrumentID,strLeg1InstrumentID,strLeg2InstrumentID);
}
///����ָ����Լ��Ϣ
void CDataCenter::SetInstrumentInfo(const string& InstrumentID, PlatformStru_InstrumentInfo& outData,bool bLast)
{
    bool bCTPSimulate = false;
    if((int)m_pDataMgr->m_ServerParamCfg.vec.size()>0&&
        m_pDataMgr->m_ServerParamCfg.CurSvrGrpID<(int)m_pDataMgr->m_ServerParamCfg.vec.size()&&
        m_pDataMgr->m_ServerParamCfg.vec[m_pDataMgr->m_ServerParamCfg.CurSvrGrpID].CTPSimulate==1)
        bCTPSimulate=true;
    m_pDataMgr->m_Instruments.SetInstrumentInfo(InstrumentID,outData,bCTPSimulate,bLast);
    m_pDataMgr->m_Rates.SetNeedCheckCommissionRateAndMarginRate(true);

    //char buf[10240]={0};
    //const PlatformStru_InstrumentInfo *pInstrument=&outData;
    //sprintf(buf,
			 //"\t\t\t InstrumentID=[%s],\t ExchangeID=[%s],\t InstrumentName=[%s],\t ExchangeInstID=[%s],\t ProductID=[%s], ProductClass=[%d],\n"
			 //"\t\t\t DeliveryYear=[%d],\t DeliveryMonth=[%d],\n"
    //         "\t\t\t MaxMarketOrderVolume=[%d],\t MinMarketOrderVolume=[%d],\t MaxLimitOrderVolume=[%d],\t MinLimitOrderVolume=[%d],\n"
    //         "\t\t\t VolumeMultiple=[%d],\t PriceTick=[%g],\n"
    //         "\t\t\t CreateDate=[%s],\t OpenDate=[%s],\t ExpireDate=[%s],\t StartDelivDate=[%s],\t EndDelivDate=[%s],\n"
    //         "\t\t\t InstLifePhase=[%d],\t IsTrading=[%d],\t PositionType=[%d],\t PositionDateType=[%d],\n"
    //         "\t\t\t LongMarginRatio=[%g],\t ShortMarginRatio=[%g],\t MaxMarginSideAlgorithm=[%d],\n"
    //         "\t\t\t UnderlyingInstrID=[%s],\t StrikePrice=[%g],\t OptionsType=[%d],\t UnderlyingMultiple=[%d],\t CombinationType=[%d]\n", 
			 //pInstrument->InstrumentID, pInstrument->ExchangeID, pInstrument->InstrumentName, pInstrument->ExchangeInstID, pInstrument->ProductID, pInstrument->ProductClass, 
    //         pInstrument->DeliveryYear, pInstrument->DeliveryMonth, 
    //         pInstrument->MaxMarketOrderVolume, pInstrument->MinMarketOrderVolume, pInstrument->MaxLimitOrderVolume, pInstrument->MinLimitOrderVolume, 
    //         pInstrument->VolumeMultiple, pInstrument->PriceTick, 
    //         pInstrument->CreateDate, pInstrument->OpenDate, pInstrument->ExpireDate, pInstrument->StartDelivDate, pInstrument->EndDelivDate, 
    //         pInstrument->InstLifePhase, pInstrument->IsTrading, pInstrument->PositionType, pInstrument->PositionDateType, \
    //         pInstrument->LongMarginRatio, pInstrument->ShortMarginRatio,pInstrument->MaxMarginSideAlgorithm,
    //         pInstrument->UnderlyingInstrID,pInstrument->StrikePrice,pInstrument->OptionsType,pInstrument->UnderlyingMultiple,pInstrument->CombinationType
			 //);
    //CFileOpr::getObj().writelocallog2("log","SetInstrumentInfo",true,false,false,"%s",buf);

}
///����ָ��Ʒ����Ϣ
void CDataCenter::SetProductInfo(const string& ProductID, PlatformStru_ProductInfo& outData)
{
    m_pDataMgr->m_Instruments.SetProductInfo(ProductID,outData);

    //char buf[10240]={0};
    //const PlatformStru_ProductInfo *pProduct=&outData;
    //sprintf(buf,
			 //"\n\t\t\t ProductID=[%s] \t ProductName=[%s] \t ExchangeID=[%s] "
			 //"\n\t\t\t ProductClass=[%d] \t VolumeMultiple=[%d] \t PriceTick=[%g] "
			 //"\n\t\t\t MaxMarketOrderVolume=[%d] \t MinMarketOrderVolume=[%d] \t MaxLimitOrderVolume=[%d] \t MinLimitOrderVolume=[%d] "
			 //"\n\t\t\t PositionType=[%d] \t PositionDateType=[%d] \t CloseDealType=[%d] \t TradeCurrencyID=[%s] \t MortgageFundUseRange=[%d] "
			 //"\n\t\t\t ExchangeProductID=[%s] \t UnderlyingMultiple=[%d] "
			 //,pProduct->ProductID, pProduct->ProductName, pProduct->ExchangeID
			 //,pProduct->ProductClass, pProduct->VolumeMultiple, pProduct->PriceTick
			 //,pProduct->MaxMarketOrderVolume, pProduct->MinMarketOrderVolume, pProduct->MaxLimitOrderVolume, pProduct->MinLimitOrderVolume
			 //,pProduct->PositionType, pProduct->PositionDateType, pProduct->CloseDealType, pProduct->TradeCurrencyID, pProduct->MortgageFundUseRange
    //         ,pProduct->ExchangeProductID, pProduct->UnderlyingMultiple
			 //);
    //CFileOpr::getObj().writelocallog2("log","SetProductInfo",true,false,false,"%s",buf);
}

///��ȡƷ���б�
int CDataCenter::GetProductList(vector<string> &outData,const string& ExchangeID)
{
    return m_pDataMgr->m_Instruments.GetProductList(outData,ExchangeID);
}

///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
char CDataCenter::GetProductClassType(const string& InstrumentID)
{
    return m_pDataMgr->m_Instruments.GetProductClassType(InstrumentID);
}
///��ȡ��Լ��ProductID
string CDataCenter::GetProductID(const string& strInstrumentID)
{
    return m_pDataMgr->m_Instruments.GetProductID(strInstrumentID);
}
string CDataCenter::GetExchangeID(const string& strInstrumentID)
{
    return m_pDataMgr->m_Instruments.GetExchangeID(strInstrumentID);
}

///��ȡ�����˵��Ⱥ�Լ����Ϻ�Լ�б�
bool CDataCenter::GetCombInstrumentIDs_IncludeLeg(const string& LegInstrument,vector<string>& vecCombInstruments)
{
    return m_pDataMgr->m_Instruments.GetCombInstrumentIDs_IncludeLeg(LegInstrument,vecCombInstruments);
}

///��ȡָ��������������Ʒ��-��Լ
int CDataCenter::GetProductID_InstrumentIDsByExchangeID(map<string, set<string> >& outData,string ExchangeID)
{
    m_pDataMgr->m_Instruments.GetProductID_InstrumentIDsByExchangeID(outData,ExchangeID);
    return outData.size();
}
///��ȡȫ����������Ʒ�ֺ�ԼID vector<pair<ExchangeID,vector<pair<ProductID,vector<InstrumentID>>>>>
void CDataCenter::GetExchangeID_ProductID_InstrumentIDs(vector<pair<string,vector<pair<string,vector<string>>>>>& outData)
{
    m_pDataMgr->m_Instruments.GetExchangeID_ProductID_InstrumentIDs(outData);
}
///��ȡȫ����������Ʒ����Ϣ vector<pair<ExchangeID,vector<PlatformStru_ProductInfo>>>
void CDataCenter::GetExchangeID_ProductInfos(vector<pair<string,vector<PlatformStru_ProductInfo>>>& outData)
{
    m_pDataMgr->m_Instruments.GetExchangeID_ProductInfos(outData);
}

///���ݺ�ԼID��ȡƷ����Ϣ
bool CDataCenter::GetProductInfo(const string& strInstrumentID,PlatformStru_ProductInfo& outData)
{
    return m_pDataMgr->m_Instruments.GetProductInfo(strInstrumentID,outData);
}
///����Ʒ��ID��ȡƷ����Ϣ
bool CDataCenter::GetProductInfo2(const string& strProductID,PlatformStru_ProductInfo& outData)
{
    return m_pDataMgr->m_Instruments.GetProductInfo2(strProductID,outData);
}
//��ȡָ������������Ȩ������ڻ�Ʒ��
int CDataCenter::GetOptionProductIDsByExchangeID(const string& ExchangeID,vector<string>& outProductIDs)
{
    return m_pDataMgr->m_Instruments.GetOptionProductIDsByExchangeID(ExchangeID,outProductIDs);
}
//��ȡָ��������ڻ���Լ��Ӧ����Ȩ��ԼID������Ŀ����Ȩ��Լ��������UnderlyingInstrumentIDΪ���򷵻�ȫ����Ȩ��Լ
int CDataCenter::GetOptionInstrumentIDsByUnderlyingInstrumentID(const string& UnderlyingInstrumentID,vector<string>& outOptionInstrumentIDs)
{
    return m_pDataMgr->m_Instruments.GetOptionInstrumentIDsByUnderlyingInstrumentID(UnderlyingInstrumentID,outOptionInstrumentIDs);
}
//��ȡָ��������ڻ���Լ��Ӧ�Ŀ�����Ȩ��ԼID����Ϳ�����Ȩ��ԼID���顣UnderlyingInstrumentIDΪ���򷵻�ȫ����Ȩ��Լ
void CDataCenter::GetOptionInstrumentIDsByUnderlyingInstrumentID(const string& UnderlyingInstrumentID,vector<string>& outCallOptionInstrumentIDs,vector<string>& outPutOptionInstrumentIDs)
{
    return m_pDataMgr->m_Instruments.GetOptionInstrumentIDsByUnderlyingInstrumentID(UnderlyingInstrumentID,outCallOptionInstrumentIDs,outPutOptionInstrumentIDs);
}
//��ȡ���б�����ڻ���Լ
int CDataCenter::GetAllOptionUnderlyingInstrumentIDs(vector<string>& outUnderlyingInstrumentIDs)
{
    return m_pDataMgr->m_Instruments.GetAllOptionUnderlyingInstrumentIDs(outUnderlyingInstrumentIDs);
}
int CDataCenter::GetAllOptionUnderlyingInstrumentIDs(set<string>& outUnderlyingInstrumentIDs)
{
    return m_pDataMgr->m_Instruments.GetAllOptionUnderlyingInstrumentIDs(outUnderlyingInstrumentIDs);
}
//�ж��Ƿ�����Ȩ������ڻ���Լ
bool CDataCenter::IsUnderlyingInstrumentID(const string& InstrumentID)
{
    return m_pDataMgr->m_Instruments.IsUnderlyingInstrumentID(InstrumentID);
}

///��ȡ��Լ��������
int CDataCenter::GetCommissionRate(const string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    return m_pDataMgr->m_Rates.GetCommissionRate(InstrumentID,outData);
}
bool CDataCenter::IsCommissionRateExist(const string& InstrumentID)
{
    return m_pDataMgr->m_Rates.IsCommissionRateExist(InstrumentID);
}
///���ú�Լ��������
int CDataCenter::SetCommissionRate(const string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    m_pDataMgr->m_Rates.SetCommissionRate(InstrumentID,outData);
    return 0;
}
///�������ڲ�ѯ�������ʵĺ�Լ
void CDataCenter::SetReqCommissionRateInstrumentID(const string& InstrumentID)
{
    m_pDataMgr->m_Rates.SetReqCommissionRateInstrumentID(InstrumentID);
}
///��ȡ��Լ��֤����
int CDataCenter::GetMarginRate(const string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    return m_pDataMgr->m_Rates.GetMarginRate(InstrumentID,outData);
}
bool CDataCenter::IsMarginRateExist(const string& InstrumentID)
{
    return m_pDataMgr->m_Rates.IsMarginRateExist(InstrumentID);
}
///���ú�Լ��֤����
int CDataCenter::SetMarginRate(const string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    m_pDataMgr->m_Rates.SetMarginRate(InstrumentID,outData);
    return 0;
}

///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
int CDataCenter::GetInstrumentMultiple(const string& InstrumentID)
{
    return m_pDataMgr->m_Instruments.GetInstrumentMultiple(InstrumentID);
}
///��ȡָ��Ʒ�ֵĺ�Լ�б�,���غ�Լ����
int CDataCenter::GetInstrumentListByProductID(const string& ProductID,set<string> &outData)
{
    m_pDataMgr->m_Instruments.GetInstrumentListByProductID(ProductID,outData);
    return outData.size();
}
int CDataCenter::GetInstrumentListByProductID(const string& ProductID,vector<string> &outData)
{
    m_pDataMgr->m_Instruments.GetInstrumentListByProductID(ProductID,outData);
    return outData.size();
}



///��ȡָ����Լ����
int CDataCenter::GetQuotInfo(const string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    return m_pDataMgr->m_Quots.GetQuotInfo(InstrumentID,outData)?0:-1;
}


///��ȡָ����Լ������
int CDataCenter::GetOldQuotInfo(const string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    return m_pDataMgr->m_Quots.GetOldQuotInfo(InstrumentID,outData)?0:-1;
}

///��ȡָ����Լ���ּۣ�ʧ�ܷ�����Чֵ
double CDataCenter::GetCurPrice(const string& InstrumentID)
{
    return m_pDataMgr->m_Quots.GetCurPrice(InstrumentID);
}

int CDataCenter::GetPositions2(const string& strInstrument,
                               set<long>& setFTID,
                               vector<PlatformStru_Position>& outData,
                               long& lastseq)
{
    return m_pDataMgr->m_Positions.GetPositions2(strInstrument,setFTID,outData,lastseq);
}

int CDataCenter::GetPositions3(const string& strInstrument,
                               vector<PlatformStru_Position>& outData,
                               long& lastseq)
{
    return m_pDataMgr->m_Positions.GetPositions3(strInstrument,outData,lastseq);
}

int CDataCenter::GetPositionDetails3(const string& strInstrument,
                          set<long>& setFTID,
                          vector<PlatformStru_PositionDetail>& vecValue,
                          long& lastseq)
{
    return m_pDataMgr->m_PositionDetails.GetPositionDetails3(strInstrument,setFTID,vecValue,lastseq);
}

int CDataCenter::GetPositionCombs2(const string& strInstrument,vector<PlatformStru_Position>& outData)
{
    return m_pDataMgr->m_PositionCombs.GetPositionCombs2(strInstrument, outData);
}

int CDataCenter::GetPositions(vector<PlatformStru_Position>& outData)
{
    return m_pDataMgr->m_Positions.GetPositions(outData);
}

int CDataCenter::GetPositionDetails(vector<PlatformStru_PositionDetail>& outData,long& lastseq)
{
    return m_pDataMgr->m_PositionDetails.GetPositionDetails(outData,lastseq);
}


int CDataCenter::GetPositionCombs(vector<PlatformStru_Position>& outData)
{
    return m_pDataMgr->m_PositionCombs.GetPositionCombs(outData);
}



bool CDataCenter::HavePositionDetail(const string& strInstrumentID)
{
    return m_pDataMgr->m_PositionDetails.HavePositionDetail(strInstrumentID);
}
bool CDataCenter::HaveCombPositionDetail()
{
    return m_pDataMgr->m_PositionDetails.HaveCombPositionDetail();
}
void CDataCenter::GetDerivedInstrumentID_OnCloseTrade(set<string>& InstrumentIDs)
{
    m_pDataMgr->m_PositionDetails.GetDerivedInstrumentID_OnCloseTrade(InstrumentIDs);
}


//��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
int CDataCenter::GetPositionKeySet(set<PositionKey> &PositionKeySet)
{
    return m_pDataMgr->m_Positions.GetPositionKeySet(PositionKeySet);
}

//��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
int CDataCenter::GetPositionData(const string& strAccount, const string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData)
{
    return m_pDataMgr->m_Positions.GetPositionData(strAccount,InstrumentID,Direction,HedgeFlag,PositionData);
}



string CDataCenter::GetTradingDay()
{
    return m_pDataMgr->m_Instruments.GetTradingDay();
}

bool CDataCenter::IsTodayPosition( const char* pOpenData )
{
	return m_pDataMgr->m_Instruments.IsToday( pOpenData );
}

///��ȡ������ʱ��
unsigned long CDataCenter::GetExchangeTime(const string& ExchangeID,string& time)
{
    return m_pDataMgr->m_Instruments.GetExchangeTime(ExchangeID,time);
}
    ///��ȡȫ��������
int CDataCenter::GetExchangeIDs(vector<string>& outData)
{
	return m_pDataMgr->m_Instruments.GetExchangeIDs(outData);
}

///���õ�ǰ��ʾ�ĳֲ����ݡ�1:�ֲ֣�2:�ֲ���ϸ��3:��ϳֲ�
void CDataCenter::SetCurrentPositionContent(int PositionContentMode)
{
    switch(PositionContentMode)
    {
        case 1:CReqOnRtnTrade::SetQryPrior(1);      break;
        case 2:CReqOnRtnTrade::SetQryPrior(2);      break;
        case 3:CReqOnRtnTrade::SetQryPrior(3);      break;
    }
}



///��ȡȫ������
int CDataCenter::GetAllOrders(vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->m_Orders.GetAllOrders(outData);
}
///��ȡȫ������
int CDataCenter::GetAllOrders2(const string& strInstrument,vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->m_Orders.GetAllOrders2(strInstrument,outData);
}
///��ȡָ������
bool CDataCenter::GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->m_Orders.GetOrder(inOrderKey,outData);
}
bool CDataCenter::GetOrder2(const string& strOrderSysID,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->m_Orders.GetOrder2(strOrderSysID,outData);
}

int CDataCenter::GetTriggerOrders(vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->m_Orders.GetTriggerOrders(outData);
}

///��ȡ��Լ��ص��Ѵ����ı���
int CDataCenter::GetTriggerOrders2(const string& strInstrument,vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->m_Orders.GetTriggerOrders2(strInstrument,outData);
}

///��ȡָ���Ѵ����ı���
bool CDataCenter::GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->m_Orders.GetTriggerOrder(orderkey,outData);
}

///��ȡ�ѳɽ�����
int CDataCenter::GetTradedOrders(vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->m_Orders.GetTradedOrders(outData);
}

///��ȡ�ѳɽ�����
int CDataCenter::GetTradedOrders2(const string& strInstrument,vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->m_Orders.GetTradedOrders2(strInstrument,outData);
}

///��ȡָ���ѳɽ�����
bool CDataCenter::GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->m_Orders.GetTradedOrder(orderkey,outData);
}

///��ȡ�ѳ����ʹ��󱨵�
int CDataCenter::GetCanceledOrders(vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->m_Orders.GetCanceledOrders(outData);
}

///��ȡ�ѳ����ʹ��󱨵�
int CDataCenter::GetCanceledOrders2(const string& strInstrument,vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->m_Orders.GetCanceledOrders2(strInstrument,outData);
}

///��ȡָ���ѳ����ʹ��󱨵�
bool CDataCenter::GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->m_Orders.GetCanceledOrder(orderkey,outData);
}

///��ȡδ�ɽ�����
int CDataCenter::GetWaitOrders(vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->m_Orders.GetWaitOrders(outData);
}

///��ȡδ�ɽ�����
int CDataCenter::GetWaitOrders3(const string& strInstrument,vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->m_Orders.GetWaitOrders3(strInstrument,outData);
}
///��ȡָ����Լ��ص�δ�ɽ�����, �������ֹ�����еı�������Լ�ǵ���Լ�������Ƕ�Ӧ������������ú�Լ����ϱ���
int CDataCenter::GetWaitOrders2(const string& strInstrument,vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->m_Orders.GetWaitOrders2(strInstrument,outData);
}

///��ȡָ��δ�ɽ�����
bool CDataCenter::GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->m_Orders.GetWaitOrder(orderkey,outData);
}



//��ȡָ����Լ��ָ������ָ����ƽ�Ĺҵ�������������
int  CDataCenter::GetWaitOrderVolume(const string& strAccount, const string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag)
{
    return m_pDataMgr->m_Orders.GetWaitOrderVolume(strAccount,strInstrumentID,Direction,CloseType);
}

//��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
int CDataCenter::GetCloseOrderVolume(const string& strAccount,const string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
    return m_pDataMgr->m_Orders.GetCloseOrderVolume(strAccount,strInstrumentID, Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume);
}

///��ȡȫ���ɽ���
int CDataCenter::GetAllTrades(vector<PlatformStru_TradeInfo>& outData)
{
    return m_pDataMgr->m_Trades.GetAllTrades(outData);
}
///��ȡָ���ɽ���Ϣ
bool CDataCenter::GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData)
{
    return m_pDataMgr->m_Trades.GetTradeInfo(tradekey,outData);
}
///��ȡָ���ɽ���Ϣ
int CDataCenter::GetTradesOfInstrument(const string& strInstrument,vector<PlatformStru_TradeInfo>& outData)
{
    return m_pDataMgr->m_Trades.GetTradesOfInstrument(strInstrument,outData);
}
int CDataCenter::GetTradeBriefsOfInstrument(const string& strInstrument,vector<PlatformStru_TradeInfoBrief>& outData)
{
    return m_pDataMgr->m_Trades.GetTradeBriefsOfInstrument(strInstrument,outData);
}

///��ȡȫ���ɽ�ͳ�Ƽ�¼
int CDataCenter::GetAllTradeTotals(vector<PlatformStru_TradeTotalInfo>& outData)
{
    return m_pDataMgr->m_TradeTotals.GetAllTradeTotals(outData);
}

///��ȡָ����Լ�ĳɽ�ͳ�Ƽ�¼�ɹ�����0��ʧ�ܷ���-1
int CDataCenter::GetTradeTotalOfInstrument(const string& strInstrument, vector<PlatformStru_TradeTotalInfo>& outData)
{
    return m_pDataMgr->m_TradeTotals.GetTradeTotalOfInstrument(strInstrument,outData);
}

///��ȡ�ɽ�ͳ��
int CDataCenter::GetAllTradeTotalDatas(vector<TotalInfo>& outData)
{
    return 0;
}

///�ڳɽ�ͳ�Ʋ��Ҳ���rawData
int CDataCenter::FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData )
{
    return 0;
}
int CDataCenter::GetTradingAccountAvailable(double& fAvailable)
{
    return m_pDataMgr->m_TradingAccount.GetTradingAccountAvailable(fAvailable);
}
int CDataCenter::GetTradingAccountWithdrawQuota(double& fWithdrawQuota)
{
    return m_pDataMgr->m_TradingAccount.GetTradingAccountWithdrawQuota(fWithdrawQuota);
}
int CDataCenter::GetTradingAccountID(char* AccountID,int rltsize)
{
    return m_pDataMgr->m_TradingAccount.GetTradingAccountID(AccountID,rltsize);
}
///��ȡ�����ʽ��˻���Ϣ
int CDataCenter::GetTradingAccount(PlatformStru_TradingAccountInfo& outData)
{
    return m_pDataMgr->m_TradingAccount.GetTradingAccount(outData);
}
///��ȡ�˻��ʽ��ı���Ϣ
int CDataCenter::GetAccountText(string& outData,int language)
{
    return m_pDataMgr->m_TradingAccount.GetAccountText(outData,language);
}
    
///�����ѯͶ���߽�������Ӧ
int CDataCenter::GetLastSettlementInfo(string& outData)
{
    return m_pDataMgr->GetLastSettlementInfo(outData);
}
 //��ȡ�ͻ�ǩԼ������Ϣ
int CDataCenter::GetContractBank(map<string,  PlatformStru_ContractBank>& outData)
{
    return m_pDataMgr->GetContractBank(outData);
}

SERVER_PARAM_CFG&  CDataCenter::GetServerParamCfg()
{
    return m_pDataMgr->GetServerParamCfg();
}

void CDataCenter::ClearMapQryRltOrders()
{
    m_pDataMgr->m_Orders.ClearMapQryRltOrders();
}

void CDataCenter::ClearMapQryRltTrades()
{
    m_pDataMgr->m_Trades.ClearMapQryRltTrades();
//    m_pDataMgr->ClearMapQryRltTrades();
}

void CDataCenter::ClearMapQryRltPositionDetails(string& strQryInstrument)
{
    m_pDataMgr->m_PositionDetails.ClearMapQryRltPositionDetails(strQryInstrument);
}

void CDataCenter::ClearMapQryRltPositions(string& strQryInstrument)
{
    m_pDataMgr->m_Positions.ClearMapQryRltPositions(strQryInstrument);
    m_pDataMgr->m_PositionCombs.ClearMapQryRltPositionCombs(strQryInstrument);
}

void CDataCenter::ClearMapQryRltPositionDetailComb(string& strQryInstrument)
{
    m_pDataMgr->m_PositionDetailCombs.ClearMapQryRltPositionDetailCombs(strQryInstrument);
}

//UI��֪ͨ�ײ㣬��ʼ����ѯ��ʼ�����
void CDataCenter::NotifyInitQryStart()
{
    LOG_INFO("CDataCenter::NotifyInitQryStart - %s","��ʼ����ѯ��ʼ");

    //if(bStart)
    //    m_bStartInitQry=true;
    //else
    //    m_bStopInitQry=true;

  //  if(!bStart)
  //  {
  //      //��ʼ����ѯ��ɣ������ճ��ֲ���ϸ�����ڳɽ���¼�����¼���ɽ���ƽ������ƽ��ӯ����������
  //      m_pDataMgr->m_PositionDetails.ReCalcuTradeInfo_OnInitQryCompleted();

  //      //������ϳֲ���ϸ�ͳֲ���ϸ��������ϳֲֵĳֲֳɱ������ֳɱ����ֲ־��ۺͿ��־��ۡ�
  //      m_pDataMgr->m_PositionCombs.UpdateCostByPositionDetialComb_AfterInitQry();

  //      //�����ʽ�
		//m_pDataMgr->m_TradingAccount.UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted();
  //  }
}
//UI��֪ͨ�ײ㣬��ʼ����ѯ��ʼ�����
void CDataCenter::NotifyInitQrySucc()
{
    LOG_INFO("CDataCenter::NotifyInitQrySucc - %s","��ʼ����ѯ�ɹ�");

    //if(bStart)
    //    m_bStartInitQry=true;
    //else
    //    m_bStopInitQry=true;

    //if(!bStart)
    //{
    //��ʼ����ѯ��ɣ������ճ��ֲ���ϸ�����ڳɽ���¼�����¼���ɽ���ƽ������ƽ��ӯ����������
    m_pDataMgr->m_PositionDetails.ReCalcuTradeInfo_OnInitQryCompleted();

    //������ϳֲ���ϸ�ͳֲ���ϸ��������ϳֲֵĳֲֳɱ������ֳɱ����ֲ־��ۺͿ��־��ۡ�
    m_pDataMgr->m_PositionCombs.UpdateCostByPositionDetialComb_AfterInitQry();

    //�����ʽ�
	m_pDataMgr->m_TradingAccount.UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted();
    //}
}

int CDataCenter::GetAllOrderFTIDList( vector<long>& vec )
{
	return m_pDataMgr->m_Orders.GetAllOrderFTIDList(vec);
}

int CDataCenter::GetAllOrderInfo( long lFTID, PlatformStru_OrderInfo& outData )
{
	return m_pDataMgr->m_Orders.GetAllOrderInfo(lFTID, outData);
}

int CDataCenter::GetAllOrderInfo( OrderKey key, PlatformStru_OrderInfo& outData )
{
	return m_pDataMgr->m_Orders.GetAllOrderInfo(key, outData);
}

int CDataCenter::GetPositionFTIDList( vector<long>& vec )
{
	return m_pDataMgr->m_Positions.GetPositionFTIDList(vec);
}

int CDataCenter::GetPositionInfo( long lFTID, PlatformStru_Position& outData )
{
	return m_pDataMgr->m_Positions.GetPositionInfo(lFTID, outData);
}

int CDataCenter::GetPositionInfo( PositionKey key, PlatformStru_Position& outData )
{
	return m_pDataMgr->m_Positions.GetPositionInfo(key, outData);
}

int CDataCenter::GetPositionCombFTIDList( vector<long>& vec )
{
	return m_pDataMgr->m_PositionCombs.GetPositionCombFTIDList(vec);
}

int CDataCenter::GetPositionCombInfo( long lFTID, PlatformStru_Position& outData )
{
	return m_pDataMgr->m_PositionCombs.GetPositionCombInfo(lFTID, outData);
}

//��ȡ�������ӵ�FrontID��SessionID��������ֵ�ڽ��׵�¼ʱ��ctp����
const PlatformStru_LoginInfo& CDataCenter::GetTradingLoginInfo()
{
    return m_pDataMgr->m_LoginInfo.GetTradingLoginInfo();
}

//��ȡ������Ϣ
vector<string> CDataCenter::GetCurrencys(const string& BankName,bool bAddUSD,bool bAddCNY)
{
    return m_pDataMgr->GetCurrencys(BankName,bAddUSD,bAddCNY);
}
vector<string> CDataCenter::GetCurrencys(bool bAddUSD,bool bAddCNY)
{
    return m_pDataMgr->GetCurrencys(bAddUSD,bAddCNY);
}
