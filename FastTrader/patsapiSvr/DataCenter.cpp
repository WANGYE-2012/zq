#include "stdafx.h"

#include <time.h>
#include "PatsApiSvr.h"
#include "DataCenter.hpp"
#include "../inc/Module-Misc/packagecache.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "PlatFormService.hpp"
#include "../inc/Module-Misc/GlobalConfigMgr.h"
//#include "ReqOnRtnTrade.h"

#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CDataCenter", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
    }while(0)

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
            DataRtnDepthMarketData tmp;

            if(m_pDataMgr->GetInstrumentList(InstrumentSet)>0)
            {
                for(it=InstrumentSet.begin();it!=InstrumentSet.end();it++)
                {
                    if(m_pDataMgr->GetQuotInfo(*it,tmp.DepthMarketDataField)==0)
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
            std::string strInstrument(NewBusiness.DepthMarketDataField.InstrumentID);
            PlatformStru_InstrumentInfo InstrumentInfo;

            //ȡ��Լ��Ϣ
            if(GetInstrumentInfo(strInstrument,InstrumentInfo)<0)
		        break;

            //�ڽṹ�б�����Լ�Ƿ��гֲ�
            NewBusiness.bHavePosition=m_pDataMgr->HavePositionDetail(strInstrument);

            PlatformStru_DepthMarketData MarketData2;
            memset(&MarketData2,0,sizeof(MarketData2));
            MarketData2=NewBusiness.DepthMarketDataField;

            toBeContinue=m_pDataMgr->SetQuotInfo(MarketData2);
            break;
        }
        case BID_RtnOrder://�����ر�,���±�������
        {
			DataRtnOrder& NewBusiness= (DataRtnOrder&)data;

            PlatformStru_InstrumentInfo InstrumentInfo;
			ZeroMemory(&InstrumentInfo, sizeof(InstrumentInfo));
            if(GetInstrumentInfo(std::string(NewBusiness.OrderField.InstrumentID),InstrumentInfo)<0)
		        break;

            PlatformStru_OrderInfo OldOrderInfo;
            m_pDataMgr->NewOrderReady(NewBusiness.OrderField,OldOrderInfo,InstrumentInfo);

			m_PlatFormService->SetSubscribeStatus(NewBusiness.OrderField.InstrumentID,S_STATUS_ORDER);
            break;
		}
        case BID_RtnTrade://�ɽ��ر�,���±�������
        {
			DataRtnTrade& NewBusiness= (DataRtnTrade&)data;

            PlatformStru_InstrumentInfo InstrumentInfo;
            if(GetInstrumentInfo(std::string(NewBusiness.TradeField.InstrumentID),InstrumentInfo)<0)
		        break;

            m_pDataMgr->NewTradeReady(NewBusiness.TradeField,InstrumentInfo);

            //m_PlatFormService->SetSubscribeStatus(NewBusiness.TradeField.InstrumentID,S_STATUS_TRADE);
			m_pDataMgr->UpdatePosition_RtnTrade();

            //if(GlobalConfigManager::IsReq_OnRtnTrade())
            //{
            //    CReqOnRtnTrade::OnRtnTrade(std::string(NewBusiness.TradeField.BrokerID),
            //                                std::string(NewBusiness.TradeField.InvestorID),
            //                                std::string(NewBusiness.TradeField.InstrumentID),
            //                                (NewBusiness.TradeField.TradeType==THOST_FTDC_TRDT_CombinationDerived?true:false),
            //                                m_pDataMgr->GetRecordCountOfPositionDetail());
            //}
			break;
		}
        case BID_RspQryInstrument://�����ѯ��Լ��Ӧ
        {
			DataRspQryInstrument& NewBusiness= (DataRspQryInstrument&)data; 
			if(NewBusiness.RspInfoField.ErrorID==0)
			{
				if(NewBusiness.InstrumentField.InstrumentID[0]!=0)
				{
					PlatformStru_InstrumentInfo& Info2=NewBusiness.InstrumentField;
					m_pDataMgr->SetInstrumentInfo(NewBusiness.InstrumentField.InstrumentID,Info2);
				}
            }
            break;
        }
		case BID_RspQryInvestorPosition:
        {
            DataRspQryInvestorPosition& NewBusiness= (DataRspQryInvestorPosition&)data; 
            if(NewBusiness.RspInfoField.ErrorID==0||NewBusiness.RspInfoField.ErrorID==CustomErrorID)
            {
                m_pDataMgr->QryPositionReach(NewBusiness.InvestorPositionField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);

            }
            break;
        }
        case BID_RspQryInvestorPositionDetail://�����ѯͶ���ֲ߳���ϸ��Ӧ
            break;
        //case BID_RspQryInvestorPositionComb://�����ѯͶ������ϳֲ���ϸ��Ӧ
		//	break;
        case BID_RspQryTrade:
        {
            DataRspQryTrade& NewBusiness= (DataRspQryTrade&)data;
            if(NewBusiness.RspInfoField.ErrorID == 0||NewBusiness.RspInfoField.ErrorID==CustomErrorID)
            {
                m_pDataMgr->QryTradeReach(NewBusiness.TradeField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);
				//m_PlatFormService->SetSubscribeStatus(NewBusiness.TradeField.InstrumentID,S_STATUS_TRADE);
            }
            break;
        }
        break;
        case BID_RspQryOrder://�����ѯ������Ӧ
        {
            DataRspQryOrder& NewBusiness= (DataRspQryOrder&)data;
            if(NewBusiness.RspInfoField.ErrorID==0||NewBusiness.RspInfoField.ErrorID==CustomErrorID)
            {
                m_pDataMgr->QryOrderReach(NewBusiness.OrderField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);


                m_PlatFormService->SetSubscribeStatus(NewBusiness.OrderField.InstrumentID,S_STATUS_ORDER);
            }
            break;        
        }
		case BID_RspOrderInsert1://����¼��ر�
			{
				DataRspOrderInsert1& NewBusiness= (DataRspOrderInsert1&)data;
				if(NewBusiness.RspInfoField.ErrorID!=0)
				{
					m_PlatFormService->SetSubscribeStatus(NewBusiness.InputOrderField.InstrumentID,S_STATUS_ORDERINSERT);
				}
			}
			break;  
		case BID_RspOrderInsert2://����¼��ر�
			{
				DataRspOrderInsert2& NewBusiness= (DataRspOrderInsert2&)data;
				if(NewBusiness.RspInfoField.ErrorID!=0)
				{
					m_PlatFormService->SetSubscribeStatus(NewBusiness.InputOrderField.InstrumentID,S_STATUS_ORDERINSERT);
				}
			}
			break;   
        case BID_RspQryTradingAccount://�����ʽ��˻���Ϣ
        {
            DataRspQryTradingAccount& NewBusiness= (DataRspQryTradingAccount&)data;
            if(NewBusiness.RspInfoField.ErrorID==0)
            {
                m_pDataMgr->SetTradingAccount(NewBusiness);
            }
        }
        break;
        case BID_RspQryInvestor://�����ѯͶ������Ӧ
        break;
        case BID_RspQryInstrumentMarginRate://�����ѯ��Լ��֤������Ӧ
            {
    //            DataRspQryInstrumentMarginRate& NewBusiness= (DataRspQryInstrumentMarginRate&)data;
    //            if(NewBusiness.RspInfoField.ErrorID==0)
    //            {
    //                std::string strInstrument(NewBusiness.InstrumentMarginRateField.InstrumentID);
    //                PlatformStru_InstrumentInfo InstrumentInfo;
    //                if(GetInstrumentInfo(strInstrument,InstrumentInfo)<0)
		  //              break;

    //                PlatformStru_InstrumentMarginRate MarginRate;
    //                MarginRate=NewBusiness.InstrumentMarginRateField;
    //                m_pDataMgr->SetMarginRate(NewBusiness.InstrumentMarginRateField.InstrumentID,MarginRate);


    //                if(m_pDataMgr->UpdateOrderFreezeMargin_RspQryMarginRate(strInstrument,InstrumentInfo))
    //                    m_pDataMgr->DynamicUpdateTradingAccount_Freeze();

    //                m_pDataMgr->UpdatePositionDetailByMarginRate(MarginRate,InstrumentInfo);

				//	m_pDataMgr->UpdatePositionFreezeFieldsByOrders_RspQryRate_Internal(std::string(MarginRate.InstrumentID));

    //                if(m_pDataMgr->UpdatePositionByMarginRate_RspQryRate(MarginRate))
    //                    m_pDataMgr->DynamicUpdateTradingAccount_RspQryInstrumentMarginRate();
    //            }
				//else if(NewBusiness.RspInfoField.ErrorID==CustomErrorID)
				//{
				//	//���޼�¼
				//	m_pDataMgr->IncrementReqMarginRateError(NewBusiness.InstrumentMarginRateField.InstrumentID);
				//}
            }
            break;
        case BID_RspQryInstrumentCommissionRate://�����ѯ��Լ����������Ӧ
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
			//PATS��֧�ֻ�ȡ������ʱ��
            //DataRspUserLogin& NewBusiness= (DataRspUserLogin&)data;
            //if(NewBusiness.RspInfoField.ErrorID==0 && NewBusiness.bTrader)
            //{
            //    ExchangeTime exchangeTime;
            //    m_pDataMgr->GetExchangeTime(exchangeTime);
            //    exchangeTime.CurTickCount = NewBusiness.CurTickCount;
            //    memcpy(&exchangeTime.TradingDay, &NewBusiness.UserLoginField.TradingDay, sizeof(exchangeTime.TradingDay));
            //    memcpy(&exchangeTime.SHFETime, &NewBusiness.UserLoginField.SHFETime, sizeof(exchangeTime.SHFETime));
            //    memcpy(&exchangeTime.DCETime, &NewBusiness.UserLoginField.DCETime, sizeof(exchangeTime.DCETime));
            //    memcpy(&exchangeTime.CZCETime, &NewBusiness.UserLoginField.CZCETime, sizeof(exchangeTime.CZCETime));
            //    memcpy(&exchangeTime.FFEXTime, &NewBusiness.UserLoginField.FFEXTime, sizeof(exchangeTime.FFEXTime));
            //    m_pDataMgr->SetExchangeTime(exchangeTime);
            //}
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
    std::map<GUIModuleID,GuiCallBackFunc> CallbackFuncList;
    std::map<GUIModuleID,GuiCallBackFunc>::iterator it_callback;

    if(m_pDataMgr->GetSubscribeBusinessData(data.BID,CallbackFuncList)==0)
    {
        int i;
        for(i=0,it_callback=CallbackFuncList.begin();it_callback!=CallbackFuncList.end();it_callback++,i++)
        {
            if(it_callback->second)
                (it_callback->second)(it_callback->first,data);
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

//��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
int CDataCenter::GetPositionKeySet(std::set<PositionKey> &PositionKeySet)
{
    return m_pDataMgr->GetPositionKeySet(PositionKeySet);
}

//��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
int CDataCenter::GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData)
{
    return m_pDataMgr->GetPositionData(strAccount, InstrumentID,Direction,HedgeFlag,PositionData);
}

int CDataCenter::GetPositions(std::vector<PlatformStru_Position>& outData)
{
    return m_pDataMgr->GetPositions(outData);
}

int CDataCenter::GetPositions2(const std::string& strInstrument,std::set<long>& setFTID,std::vector<PlatformStru_Position>& outData,long& lastseq)
{
    return m_pDataMgr->GetPositions2(strInstrument,setFTID,outData,lastseq);
}
int CDataCenter::GetPositions3(const std::string& strInstrument3,std::vector<PlatformStru_Position>& outData,long& lastseq)
{
	return m_pDataMgr->GetPositions3(strInstrument3,outData,lastseq);
}
string CDataCenter::GetTradingDay(const string& ExchangeID)
{
    return m_pDataMgr->GetTradingDay(ExchangeID);
}
///��ȡ������ʱ��
unsigned long CDataCenter::GetExchangeTime(const string& ExchangeID,string& time)
{
    return m_pDataMgr->GetExchangeTime(ExchangeID,time);
}
///��ȡȫ������
int CDataCenter::GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetAllOrders(outData);
}
int CDataCenter::GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetAllOrders2(strInstrument,outData);
}
bool CDataCenter::GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->GetOrder(inOrderKey,outData);
}
///��ȡȫ���Ѵ����ı���
int CDataCenter::GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetTriggerOrders(outData);
}
///��ȡ��Լ��ص��Ѵ����ı���
int CDataCenter::GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetTriggerOrders2(strInstrument,outData);
}

///��ȡָ���Ѵ����ı���
bool CDataCenter::GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->GetTriggerOrder(orderkey,outData);
}

///��ȡδ�ɽ�����
int CDataCenter::GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetWaitOrders(outData);
}
///��ȡָ����δ�ɽ�����
bool CDataCenter::GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->GetWaitOrder(orderkey, outData);
}
int CDataCenter::GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetWaitOrders2(strInstrument, outData);
}
///��ȡ�ѳɽ�����
int CDataCenter::GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetTradedOrders(outData);
}
///��ȡ�ѳɽ�����
int CDataCenter::GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetTradedOrders2(strInstrument, outData);
}
///��ȡָ���ѳɽ�����
bool CDataCenter::GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->GetTradedOrder(orderkey, outData);
}
///��ȡ�ѳ����ʹ��󱨵�
int CDataCenter::GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetCanceledOrders(outData);
}
///��ȡ�ѳ����ʹ��󱨵�
int CDataCenter::GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetCanceledOrders2(strInstrument, outData);
}
///��ȡָ���ѳ����ʹ��󱨵�
bool CDataCenter::GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->GetCanceledOrder(orderkey, outData);
}

//��ȡָ����Լ��ָ������ָ����ƽ�Ĺҵ�������������
int  CDataCenter::GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag)
{
    return m_pDataMgr->GetWaitOrderVolume(strAccount, strInstrumentID,Direction,CloseType);
}

//��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
int CDataCenter::GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
    return m_pDataMgr->GetCloseOrderVolume(strAccount,strInstrumentID, Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume);
}

bool CDataCenter::GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData)
{
    return m_pDataMgr->GetTradeInfo(tradekey, outData);
}
///��ȡȫ���ɽ���
int CDataCenter::GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData)
{
    return m_pDataMgr->GetAllTrades(outData);
}
///��ȡ�ɽ�ͳ��
int CDataCenter::GetAllTradeTotalDatas(vector<TotalInfo>& outData)
{
    return -1;//m_pDataMgr->GetAllTradeTotalDatas(outData);
}
///��ȡָ���ɽ���Ϣ
int CDataCenter::GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData)
{
    return m_pDataMgr->GetTradesOfInstrument(strInstrument,outData);
}
///�ڳɽ�ͳ�Ʋ��Ҳ���rawData
int CDataCenter::FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData )
{
    return -1;//m_pDataMgr->FindIndexFromAllTradeTotalDatas(rawData);
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

SERVER_PARAM_CFG&  CDataCenter::GetServerParamCfg()
{
    return m_pDataMgr->GetServerParamCfg();
}
void CDataCenter::ClearMapQryRltOrders()
{
    m_pDataMgr->ClearMapQryRltOrders();
}

void CDataCenter::ClearMapQryRltTrades()
{
    m_pDataMgr->ClearMapQryRltTrades();
}

void CDataCenter::ClearMapQryRltPositionDetails(std::string& strQryInstrument)
{
    m_pDataMgr->ClearMapQryRltPositionDetails(strQryInstrument);
}

void CDataCenter::ClearMapQryRltPositions(std::string& strQryInstrument)
{
    m_pDataMgr->ClearMapQryRltPositions(strQryInstrument);
}

void CDataCenter::ClearMapQryRltPositionCombs(std::string& strQryInstrument)
{
    m_pDataMgr->ClearMapQryRltPositionCombs(strQryInstrument);
}

int CDataCenter::GetAllOrderFTIDList( std::vector<long>& vec )
{
	return m_pDataMgr->GetAllOrderFTIDList(vec);
}

int CDataCenter::GetAllOrderInfo( long lFTID, PlatformStru_OrderInfo& outData )
{
	return m_pDataMgr->GetAllOrderInfo(lFTID, outData);
}

int CDataCenter::GetAllOrderInfo( OrderKey key, PlatformStru_OrderInfo& outData )
{
	return m_pDataMgr->GetAllOrderInfo(key, outData);
}

int CDataCenter::GetPositionFTIDList( std::vector<long>& vec )
{
	return m_pDataMgr->GetPositionFTIDList(vec);
}

int CDataCenter::GetPositionInfo( long lFTID, PlatformStru_Position& outData )
{
	return m_pDataMgr->GetPositionInfo(lFTID, outData);
}

int CDataCenter::GetPositionInfo( PositionKey key, PlatformStru_Position& outData )
{
	return m_pDataMgr->GetPositionInfo(key, outData);
}

int CDataCenter::GetPositionCombFTIDList( std::vector<long>& vec )
{
	return m_pDataMgr->GetPositionCombFTIDList(vec);
}

int CDataCenter::GetPositionCombInfo( long lFTID, PlatformStru_PositionCombDetail& outData )
{
	return m_pDataMgr->GetPositionCombInfo(lFTID, outData);
}

