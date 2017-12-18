#include "stdafx.h"

#include <time.h>
#include "CmeApiSvr.h"
#include "DataCenter.hpp"
#include "../inc/Module-Misc/packagecache.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "PlatFormService.hpp"

#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CDataCenter", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
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
#define MUTEX_LOCK()\
	g_LockCount=0;\
	while(m_mutex.TryLock()!=wxMUTEX_NO_ERROR)\
	{\
		g_LockCount++;\
		if(g_LockCount==1000)\
		{\
				LOG_INFO("Lock Failed at (%s)--->(%s)--->(%s)--->(%s)--->(%s)",LastFunction1,LastFunction2,LastFunction3,LastFunction4,__FUNCTION__);\
		}\
		wxThread::Sleep(10);\
	}\
    m_mutex.Unlock();\
	strcpy(LastFunction1,LastFunction2);\
	strcpy(LastFunction2,LastFunction3);\
	strcpy(LastFunction3,LastFunction4);\
    strcpy(LastFunction4,__FUNCTION__);\
    wxMutexLocker lock(m_mutex)
class CDataCenterWorkThread : public wxThread
{
public:
    CDataCenterWorkThread(CDataCenter& dc)
        :wxThread(),m_DataCenter(dc)
    {

    };

    // thread execution starts here
    virtual void *Entry()
    {
	    zqWriteLog* m_pWriteLog2;
        m_pWriteLog2 = new zqWriteLog(LOGMODE_LOCALFILE, "DataCenterWorkThread.log");
        m_pWriteLog2->WriteLog_Fmt("DataCenterWorkThread", LOGLEVEL_DEBUGINFO, "DataCenterWorkThread running... ");

        CPackageCache& RefPackageCache=m_DataCenter.m_dataSource;
        char readBuf[4096];
        PackageHeadStru *pPackageHead=(PackageHeadStru *)readBuf;
        char *pData= readBuf+ sizeof(PackageHeadStru);
        while(!TestDestroy())
        {
            if(RefPackageCache.IsEmpty())
            {
                wxThread::Sleep(2);
                continue;
            }
           
            if(!RefPackageCache.PeekFrom(pPackageHead, pData, sizeof(readBuf)-sizeof(PackageHeadStru)))
                continue;

            if(pPackageHead->DataLen<sizeof(AbstractBusinessData))
            {
                RefPackageCache.DeleteCurPackage();
                continue;
            }
            m_DataCenter.doProcessBusinessData(*((AbstractBusinessData*)pData)); 
            RefPackageCache.DeleteCurPackage();
            
        }
        m_pWriteLog2->WriteLog_Fmt("DataCenterWorkThread", LOGLEVEL_DEBUGINFO, "DataCenterWorkThread end ");
        delete m_pWriteLog2;

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
};



CDataCenter::CDataCenter(CPackageCache& dataSource,CPlatFormService* pHost)
:m_dataSource(dataSource),
m_PlatFormService(pHost)
{
    m_pWriteLog=new zqWriteLog();
    m_pWorkThread = NULL;
	m_pDataMgr=new CPlatformDataMgr(pHost);
}

CDataCenter::~CDataCenter()
{
	SAFE_DELETE(m_pDataMgr);
	SAFE_DELETE(m_pWriteLog);
}

///����ҵ������
int CDataCenter::SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo)
{
    if(BID>=BID_MAX || GID>=GID_MAX || callback==NULL)
        return -1;


    int ret;
    ret=m_pDataMgr->SubscribeBusinessData(BID,GID,callback);
    if(ret<0) return ret;
    
    if(!sendCurrentInfo)
        return 0;

    switch(BID)
    {
        case BID_RtnDepthMarketData:
        {
            std::set<std::string> InstrumentSet;
            std::set<std::string>::iterator it;
//            CThostFtdcDepthMarketDataField QuotData;
            DataRtnDepthMarketData tmp;
            PlatformStru_DepthMarketData MarketData;

            if(m_pDataMgr->GetInstrumentList(InstrumentSet)>0)
            {
                for(it=InstrumentSet.begin();it!=InstrumentSet.end();++it)
                {
                    if(m_pDataMgr->GetQuotInfo(*it,MarketData)==0)
                    {
                        tmp.DepthMarketDataField=MarketData;
                        (callback)(*(AbstractBusinessData*)(&tmp));
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
    return m_pDataMgr->UnSubscribeBusinessData(BID,GID);
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
    if(!m_pWorkThread)
        return 0;

    LOG_INFO("Stop:�ر�DataCenter���ڲ������߳�");
    m_pWorkThread->Delete();
    m_pWorkThread=NULL; //will also be set NULL in calling CDataCenterWorkThread::OnExit()

    return 0;
}

int CDataCenter::doProcessBusinessData(AbstractBusinessData& data)
{
    if(data.BID!=BID_RtnDepthMarketData)
        LOG_INFO("CDataCenter::doProcessBusinessData, BID=%d, Size=%d", data.BID, data.Size);

//    MUTEX_LOCK();

    bool toBeContinue=true;
    switch(data.BID)
    {
        case BID_RtnDepthMarketData://�������
        {
            DataRtnDepthMarketData& NewBusiness= (DataRtnDepthMarketData&)data;            
            
            PlatformStru_DepthMarketData MarketData2;
            memset(&MarketData2,0,sizeof(MarketData2));
            MarketData2=NewBusiness.DepthMarketDataField;
            toBeContinue=m_pDataMgr->SetQuotInfo(MarketData2);
            m_pDataMgr->UpdateProfitInDynamicPositionDetail(NewBusiness.DepthMarketDataField);      //���³ֲ���ϸ��¼�еĳֲ�ӯ��
            m_pDataMgr->DynamicUpdateTradingAccount_NewQuot(std::string(NewBusiness.DepthMarketDataField.InstrumentID));


            break;
        }
        case BID_RtnOrder://�����ر�,���±�������
        {
			DataRtnOrder& NewBusiness= (DataRtnOrder&)data;

            m_pDataMgr->NewOrderReady(NewBusiness.OrderField);
            m_pDataMgr->DynamicUpdateTradingAccount();
			m_pDataMgr->DynamicUpdateAllOrderPanel(NewBusiness.OrderField);
			m_PlatFormService->SetSubscribeStatus(NewBusiness.OrderField.InstrumentID,S_STATUS_ORDER);
            break;
        }
        case BID_RtnTrade://�ɽ��ر�,���±�������
        {
			//DataRtnTrade& NewBusiness= (DataRtnTrade&)data;

			//PlatformStru_TradeInfo TradeInfo2;
			//memset(&TradeInfo2,0,sizeof(TradeInfo2));
			//TradeInfo2=NewBusiness.TradeField;
			//m_pDataMgr->NewTradeReady(TradeInfo2);

			//m_pDataMgr->DynamicUpdateTradingAccount();
			//m_PlatFormService->SetSubscribeStatus(NewBusiness.TradeField.InstrumentID,S_STATUS_TRADE);
			m_pDataMgr->RefreshPositionData();
            break;
        }
        case BID_RspQryInstrument://�����ѯ��Լ��Ӧ
        {
            DataRspQryInstrument& NewBusiness= (DataRspQryInstrument&)data; 
            if(NewBusiness.RspInfoField.ErrorID==0)
            {
                if(NewBusiness.InstrumentField.InstrumentID[0]!=0)
                {
                    PlatformStru_InstrumentInfo Info2;
                    Info2=NewBusiness.InstrumentField;
                    m_pDataMgr->SetInstrumentInfo(NewBusiness.InstrumentField.InstrumentID,Info2);
                }
                if(NewBusiness.bIsLast)
                {
                    //m_pDataMgr->ProcessNewTradeList();
                    m_pDataMgr->DynamicUpdateTradingAccount();
                }

            }
        }
        break;
        case BID_RspQryInvestorPositionDetail://�����ѯͶ���ֲ߳���ϸ��Ӧ
        {
            DataRspQryInvestorPositionDetail& NewBusiness= (DataRspQryInvestorPositionDetail&)data; 
            if(NewBusiness.RspInfoField.ErrorID==0||NewBusiness.RspInfoField.ErrorID==CustomErrorID)
            {
                //��ӳֲ���ϸ��¼
                m_pDataMgr->OriginPositionDetailReady(NewBusiness.InvestorPositionDetailField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);
                if(NewBusiness.bIsLast)
                    m_pDataMgr->DynamicUpdateTradingAccount();
				m_PlatFormService->SetSubscribeStatus(NewBusiness.InvestorPositionDetailField.InstrumentID,S_STATUS_POS);
            }
            break;
        }
        case BID_RspQryTrade:
        {
			DataRspQryTrade& NewBusiness= (DataRspQryTrade&)data;
			CThostFtdcRspInfoField& RspInfoField = NewBusiness.RspInfoField;
			if(RspInfoField.ErrorID == 0||RspInfoField.ErrorID==CustomErrorID)
			{
				//���³ֲ���ϸ
				m_pDataMgr->OriginTradeReady(NewBusiness.TradeField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);
				if(NewBusiness.bIsLast)
					m_pDataMgr->DynamicUpdateTradingAccount();
				m_PlatFormService->SetSubscribeStatus(NewBusiness.TradeField.InstrumentID,S_STATUS_TRADE);
			}
        }
        break;
        case BID_RspQryOrder://�����ѯ������Ӧ
        {
            DataRspQryOrder& NewBusiness= (DataRspQryOrder&)data;
            if(NewBusiness.RspInfoField.ErrorID==0||NewBusiness.RspInfoField.ErrorID==CustomErrorID)
            {
                m_pDataMgr->OriginOrderReady(NewBusiness.OrderField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);
                if(NewBusiness.bIsLast)
                    m_pDataMgr->DynamicUpdateTradingAccount();
				m_PlatFormService->SetSubscribeStatus(NewBusiness.OrderField.InstrumentID,S_STATUS_ORDER);
            }
            break;        
        }
        case BID_RspOrderInsert://����¼��ر�
        {
            DataRspOrderInsert& NewBusiness= (DataRspOrderInsert&)data;
            if(NewBusiness.RspInfoField.ErrorID!=0)
            {
			   m_pDataMgr->UpdateOrdersforOrderInsert(NewBusiness.InputOrderField,NewBusiness.RspInfoField);
			   m_PlatFormService->SetSubscribeStatus(NewBusiness.InputOrderField.InstrumentID,S_STATUS_ORDER);
            }
        }
        break;  
        case BID_RspQryTradingAccount://�����ʽ��˻���Ϣ
        {
            DataRspQryTradingAccount& NewBusiness= (DataRspQryTradingAccount&)data;
            if(NewBusiness.RspInfoField.ErrorID==0)
            {
                m_pDataMgr->SetTradingAccount(NewBusiness);
                m_pDataMgr->DynamicUpdateTradingAccount();
            }
        }
        break;
        case BID_RspQryInvestor://�����ѯͶ������Ӧ
        {
            DataRspQryInvestor& NewBusiness= (DataRspQryInvestor&)data;
            if(NewBusiness.RspInfoField.ErrorID==0)
            {
                m_pDataMgr->SetInvestorInfo(NewBusiness);
            }
        }
        break;
        case BID_RspQryInstrumentMarginRate://�����ѯ��Լ��֤������Ӧ
            {
                DataRspQryInstrumentMarginRate& NewBusiness= (DataRspQryInstrumentMarginRate&)data;
                if(NewBusiness.RspInfoField.ErrorID==0)
                {
                    PlatformStru_InstrumentMarginRate MarginRate;
                    MarginRate=NewBusiness.InstrumentMarginRateField;
                    m_pDataMgr->SetMarginRate(NewBusiness.InstrumentMarginRateField.InstrumentID,MarginRate);
                    //m_pDataMgr->ProcessNewTradeList();
                    m_pDataMgr->DynamicUpdateTradingAccount();
                }
            }
            break;
        case BID_RspQryInstrumentCommissionRate://�����ѯ��Լ����������Ӧ
            {
                DataRspQryInstrumentCommissionRate& NewBusiness= (DataRspQryInstrumentCommissionRate&)data;
                if(NewBusiness.RspInfoField.ErrorID==0)
                {
                    //NewBusiness.InstrumentCommissionRateField.InstrumentID,�����ԣ���ʱ���Ǻ�ԼID,��ʱ����Ʒ�ִ���
                    //ͳһΪ��ԼID

                    //m_allProductID��ʱ�Ѿ�������(��ѯ��Լ��Ϣ������ ��ѯ��������֮ǰ)
                    std::set<std::string> InstrumentList;
                    std::string QryInstrumentID;
                    int ret;
                    ret=m_pDataMgr->GetInstrumentListByProductID(NewBusiness.InstrumentCommissionRateField.InstrumentID,InstrumentList);
                    if(ret>0)
                    {//��Ʒ�ִ���
                        m_pDataMgr->GetReqCommissionRateInstrumentID(QryInstrumentID);
                        for(std::set<std::string>::iterator it2=InstrumentList.begin(); it2!=InstrumentList.end(); ++it2)
                        {
                            if(*it2==QryInstrumentID)
                            {
                                PlatformStru_InstrumentCommissionRate Rate2;
                                memset(&Rate2,0,sizeof(Rate2));
                                Rate2=NewBusiness.InstrumentCommissionRateField;
                                m_pDataMgr->SetCommissionRate(*it2,Rate2);
                            }
                        }
                    }
                    else
                    {//�Ǻ�ԼID
                        PlatformStru_InstrumentCommissionRate Rate2;
                        memset(&Rate2,0,sizeof(Rate2));
                        Rate2=NewBusiness.InstrumentCommissionRateField;
                        m_pDataMgr->SetCommissionRate(NewBusiness.InstrumentCommissionRateField.InstrumentID,Rate2);
                    }
                    m_pDataMgr->DynamicUpdateTradingAccount();
                }
                else if(std::string(NewBusiness.RspInfoField.ErrorMsg)==std::string("��ѯ�޼�¼"))
                {
                }
            }
            break;
        case BID_RspQrySettlementInfo://�����ѯͶ���߽�������Ӧ
        {
            DataRspQrySettlementInfo& NewBusiness= (DataRspQrySettlementInfo&)data;
            if(NewBusiness.RspInfoField.ErrorID==0)
            {
                m_pDataMgr->AppendLastSettlementInfo(std::string(NewBusiness.SettlementInfoField.Content),NewBusiness.bIsLast);
            }
            else
            {
                m_pDataMgr->ClearLastSettlementInfo();
            }
        }
        break;
        case BID_RspUserLogin://������Ӧ����Ҫ��¼��������ʱ��
        {
            DataRspUserLogin& NewBusiness= (DataRspUserLogin&)data;
            if(NewBusiness.RspInfoField.ErrorID==0 && NewBusiness.bTrader)
            {
                ExchangeTime exchangeTime;
                m_pDataMgr->GetExchangeTime(exchangeTime);
                exchangeTime.CurTickCount = NewBusiness.CurTickCount;
                memcpy(&exchangeTime.TradingDay, &NewBusiness.UserLoginField.TradingDay, sizeof(exchangeTime.TradingDay));
                memcpy(&exchangeTime.SHFETime, &NewBusiness.UserLoginField.SHFETime, sizeof(exchangeTime.SHFETime));
                memcpy(&exchangeTime.DCETime, &NewBusiness.UserLoginField.DCETime, sizeof(exchangeTime.DCETime));
                memcpy(&exchangeTime.CZCETime, &NewBusiness.UserLoginField.CZCETime, sizeof(exchangeTime.CZCETime));
                memcpy(&exchangeTime.FFEXTime, &NewBusiness.UserLoginField.FFEXTime, sizeof(exchangeTime.FFEXTime));
                m_pDataMgr->SetExchangeTime(exchangeTime);
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

        default:
        break;                  
    }


    if(!toBeContinue)
        return -1;


    std::list<GuiCallBackFunc> CallbackFuncList;
    std::list<GuiCallBackFunc>::iterator it_callback;
    int ret;
    ret=m_pDataMgr->GetSubscribeBusinessData(data.BID,CallbackFuncList);
    if(ret==0)
    {
        for(it_callback=CallbackFuncList.begin();it_callback!=CallbackFuncList.end();++it_callback)
        {
            (*it_callback)(data);
        }
    }
    return 0;  
}
///��ȡ��Լ�б�
int CDataCenter::GetInstrumentList(std::vector<GroupInfo> &outData)
{
    return m_pDataMgr->GetInstrumentList(outData);
}
///��ȡ��Լ�б�
int CDataCenter::GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID)
{
    return m_pDataMgr->GetInstrumentList(outData,ExchangeID);
}
//���������Լ�б�
int CDataCenter::AddMainInstrumentList(std::string instrument)
{
	 return m_pDataMgr->AddMainInstrumentList(instrument);
}
///��ȡ������Լ�б�
int CDataCenter::GetMainInstrumentList(std::set<std::string> &outData)
{
    return m_pDataMgr->GetMainInstrumentList(outData);
}
///���ú�Լ�ݲ��б�
void CDataCenter::SetInstrumentVarietyMap(map<string, string>& inData)
{
    m_pDataMgr->SetInstrumentVarietyMap(inData);
}

// ��ȡ��Լ�ݲ��б�
int CDataCenter::GetInstrumentVarietyMap(map<string, string>& outData)
{
    return m_pDataMgr->GetInstrumentVarietyMap(outData);
}

///��ȡָ����Լ��Ϣ
int CDataCenter::GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    return m_pDataMgr->GetInstrumentInfo(InstrumentID,outData);
}
///����ָ����Լ��Ϣ
void CDataCenter::SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    m_pDataMgr->SetInstrumentInfo(InstrumentID,outData);
    
}

///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
char CDataCenter::GetProductClassType(const std::string& InstrumentID)
{
    return m_pDataMgr->GetProductClassType(InstrumentID);
}


///��ȡ��ԼƷ�ִ���
int CDataCenter::GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID)
{
    return m_pDataMgr->GetProductID(outData,ExchangeID);
}

///��ȡ��Լ��������
int CDataCenter::GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    return m_pDataMgr->GetCommissionRate(InstrumentID,outData);
}
///���ú�Լ��������
int CDataCenter::SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    m_pDataMgr->SetCommissionRate(InstrumentID,outData);
    return 0;
}
///�������ڲ�ѯ�������ʵĺ�Լ
void CDataCenter::SetReqCommissionRateInstrumentID(const std::string& InstrumentID)
{
    m_pDataMgr->SetReqCommissionRateInstrumentID(InstrumentID);
}
///��ȡ��Լ��֤����
int CDataCenter::GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    return m_pDataMgr->GetMarginRate(InstrumentID,outData);
}
///���ú�Լ��֤����
int CDataCenter::SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    m_pDataMgr->SetMarginRate(InstrumentID,outData);
    return 0;
}

///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
int CDataCenter::GetInstrumentMultiple(const std::string& InstrumentID)
{
    return m_pDataMgr->GetInstrumentMultiple(InstrumentID);
}
///��ȡָ��Ʒ�ֵĺ�Լ�б�,���غ�Լ����
int CDataCenter::GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData)
{
    return m_pDataMgr->GetInstrumentListByProductID(ProductID,outData);
}



///��ȡָ����Լ����
int CDataCenter::GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    return m_pDataMgr->GetQuotInfo(InstrumentID,outData);
}


///��ȡָ����Լ������
int CDataCenter::GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    return m_pDataMgr->GetOldQuotInfo(InstrumentID,outData);
}

///��ȡָ����Լ���ּۣ�ʧ�ܷ���0.0
double CDataCenter::GetCurPrice(const std::string& InstrumentID)
{
    return m_pDataMgr->GetCurPrice(InstrumentID);
}
///��ȡ����ί�е���Ϣ
 int CDataCenter::GetAllOrdersEx(std::map <AllOrderKey,  PlatformStru_OrderInfo> &AllOrdersMap)
 {
	 return m_pDataMgr->GetAllOrdersEx(AllOrdersMap);
 }

int CDataCenter::GetPositionDetails(std::multimap<std::string, PositionDetailStru> &PositionDetailMap)
{
    return m_pDataMgr->GetPositionDetails(PositionDetailMap);
}

int CDataCenter::GetPositionDetails(const std::string& InstrumentID,std::multimap<std::string, PositionDetailStru> &PositionDetailMap)
{
    return m_pDataMgr->GetPositionDetails(InstrumentID,PositionDetailMap);
}

//��ȡ���к�Լ�ĳֲ���ϸ��¼
int CDataCenter::GetPositionDetails(std::list<PositionDetailStru> &PositionDetailList)
{
    return m_pDataMgr->GetPositionDetails(PositionDetailList);
}

//��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
int CDataCenter::GetPositionKeyList(std::list<PositionKey> &PositionKeyList)
{
    return m_pDataMgr->GetPositionKeyList(PositionKeyList);
}

//��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
int CDataCenter::GetPositionKeySet(std::set<PositionKey> &PositionKeySet)
{
    return m_pDataMgr->GetPositionKeySet(PositionKeySet);
}

//��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
int CDataCenter::GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PositionStru& PositionData)
{
    return m_pDataMgr->GetPositionData(strAccount, InstrumentID,Direction,HedgeFlag,PositionData);
}



string CDataCenter::GetTradingDay()
{
    return m_pDataMgr->GetTradingDay();
}
///��ȡ������ʱ��
unsigned long CDataCenter::GetExchangeTime(const string& ExchangeID,string& time)
{
    return m_pDataMgr->GetExchangeTime(ExchangeID,time);
}
///��ȡȫ������
int CDataCenter::GetAllOrders(std::map<OrderKey,  PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetAllOrders(outData);
}

//��ȡָ����Լ��ָ������ָ����ƽ�Ĺҵ�������������
int  CDataCenter::GetWaitOrderVolume(const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag)
{
    return m_pDataMgr->GetWaitOrderVolume(strInstrumentID,Direction,CloseType);
}

//��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
int CDataCenter::GetCloseOrderVolume(const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
    return m_pDataMgr->GetCloseOrderVolume(strInstrumentID, Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume);
}

///��ȡȫ���ɽ���
int CDataCenter::GetAllTrades(std::map<TradeKey,  PlatformStru_TradeInfo>& outData)
{
    return m_pDataMgr->GetAllTrades(outData);
}

///��ȡ�ɽ�ͳ��
int CDataCenter::GetAllTradeTotalDatas(vector<TotalInfo>& outData)
{
    return m_pDataMgr->GetAllTradeTotalDatas(outData);
}

///�ڳɽ�ͳ�Ʋ��Ҳ���rawData
int CDataCenter::FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData )
{
    return m_pDataMgr->FindIndexFromAllTradeTotalDatas(rawData);
}
int CDataCenter::GetTradingAccountAvailable(double& fAvailable)
{
    return m_pDataMgr->GetTradingAccountAvailable(fAvailable);
}
int CDataCenter::GetTradingAccountWithdrawQuota(double& fWithdrawQuota)
{
    return m_pDataMgr->GetTradingAccountWithdrawQuota(fWithdrawQuota);
}
int CDataCenter::GetTradingAccountID(char* AccountID)
{
    return m_pDataMgr->GetTradingAccountID(AccountID);
}
///��ȡ�����ʽ��˻���Ϣ
int CDataCenter::GetTradingAccount(PlatformStru_TradingAccountInfo& outData)
{
    return m_pDataMgr->GetTradingAccount(outData);
}
///���ý����ʽ��˻���Ϣ
void CDataCenter::SetTradingAccount(const PlatformStru_TradingAccountInfo& inData)
{
    m_pDataMgr->SetTradingAccount(inData);
}
    
///�����ѯͶ���߽�������Ӧ
int CDataCenter::GetLastSettlementInfo(std::string& outData)
{
    return m_pDataMgr->GetLastSettlementInfo(outData);
}
//������Ϻ�Լ�����������
int CDataCenter::MakeComboDepthMarketData(PlatformStru_DepthMarketData *pDepthMarketData,int index)
{
	return m_pDataMgr->MakeComboDepthMarketData(pDepthMarketData,index);
}

int CDataCenter::InitCombRelation()
{
	std::vector<DataRtnDepthMarketData> tmp;
	std::vector<DataRtnDepthMarketData>::iterator it ;
	int ret=m_pDataMgr->InitCombRelation(tmp);
	for(it=tmp.begin();it!=tmp.end();++it)
	{
		bool ret1=m_dataSource.SaveDataTo(BID_RtnDepthMarketData, 0, 0, 0, sizeof(*it), &*it); 
		if(!ret1)
		{
			LOG_INFO("CQuotSpi::OnRtnDepthMarketData,Failed to SaveDataTo m_RefPackageCache:"
				"InstrumentID=%s,LastPrice=%g",
				it->DepthMarketDataField.InstrumentID,it->DepthMarketDataField.LastPrice);
		}
	}
	return ret;
}
SERVER_PARAM_CFG&  CDataCenter::GetServerParamCfg()
{
    return m_pDataMgr->GetServerParamCfg();
}
bool CDataCenter::IsNeedCheckCommissionRateAndMarginRate(void) 
{ 
	return m_pDataMgr->IsNeedCheckCommissionRateAndMarginRate();
}
void CDataCenter::SetNeedCheckCommissionRateAndMarginRate(bool bNeedCheck) 
{
	m_pDataMgr->SetNeedCheckCommissionRateAndMarginRate(bNeedCheck);
}
