//ά���ײ㱨����Ϣ

#include "stdafx.h"

#include "wx/file.h"
#include "wx/textfile.h"

#include <vector>
#include <string>
#include <map>

#include "ServiceProxy.hpp"
#include "PlatFormService.hpp"
#include "QryQueue.h"
#include "PlatformDataMgr.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "../inc/Module-Misc/tools_util.h"
#include "../inc/Module-Misc/orderCommonFunc.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#pragma warning(disable:4996)

#if 1
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CPlatformDataMgr", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif

extern PlatformSvrMgr* g_pPlatformMgr;

//ԭʼ����������������ѯ��Ӧʱ����
void CPlatformDataMgr::QryOrderReach(PlatformStru_OrderInfo& OrderInfo,const int ErrID,const bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr::QryOrderReach");

    std::set<string> setInstruments;

    Lock_CPlatformDataMgr;

    if(ErrID==0)
    {
        //���ö�Ӧ��PriceTick
        PlatformStru_InstrumentInfo InstrumentInfo;
        if(GetInstrumentInfo(std::string(OrderInfo.InstrumentID),InstrumentInfo)==0)
        {
            OrderInfo.PriceTick=InstrumentInfo.PriceTick;
        }
    }

    m_OrderData.QryRltReach_Internal(OrderInfo,ErrID,bIsLast);

    if(bIsLast)
    {
        std::map<OrderKey,  PlatformStru_OrderInfo*>::iterator it_all;
        PlatformStru_InstrumentInfo InstrumentInfo;

        for(it_all=m_OrderData.m_all.begin();it_all!=m_OrderData.m_all.end();it_all++)
        {
            if(it_all->second==NULL) continue;
            if(GetInstrumentInfo(it_all->first.InstrumentID,InstrumentInfo)<0)
		        continue;

            CalcuOrderFreezeMargin_Internal(*(it_all->second),InstrumentInfo);
            CalcuOrderFreezeCommission_Internal(*(it_all->second),InstrumentInfo);

            if(setInstruments.find(it_all->first.InstrumentID)==setInstruments.end())
                setInstruments.insert(it_all->first.InstrumentID);
        }
    }

    Unlock_CPlatformDataMgr;

    if(!setInstruments.empty())
    {
        std::set<string>::iterator it_Instrument;
        for(it_Instrument=setInstruments.begin();it_Instrument!=setInstruments.end();it_Instrument++)
        {
			if ( m_PlatFormService )
			{
				m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(*it_Instrument);
				m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(*it_Instrument);
			}
        }
    }

    LOG_INFO("CPlatformDataMgr::QryOrderReach-end");
}

//�±��������������ر�ʱ����
//����ԭ��(����ǰ)�ı�����Ϣ��������±������򷵻ص�ԭ������Ϣ��Ч
void CPlatformDataMgr::NewOrderReady(PlatformStru_OrderInfo& OrderInfo, 
                                     PlatformStru_OrderInfo& OldOrderInfo,
                                     const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    LOG_INFO("CPlatformDataMgr::NewOrderReady");

    Lock_CPlatformDataMgr;

    PlatformStru_OrderInfo *pOrder;

    OrderKey OldOrderKey(OrderInfo);
    if(m_OrderData.GetValue_Internal(pOrder,OldOrderKey))
        OldOrderInfo=*pOrder;
    else OldOrderInfo=PlatformStru_OrderInfo::GetInvalidRecord();

    CalcuOrderFreezeMargin_Internal(OrderInfo,InstrumentInfo);
    CalcuOrderFreezeCommission_Internal(OrderInfo,InstrumentInfo);

    //���ö�Ӧ��PriceTick
    OrderInfo.PriceTick=InstrumentInfo.PriceTick;

    m_OrderData.NewReach_Internal(OrderInfo);

    Unlock_CPlatformDataMgr;

	if( m_PlatFormService )
	{
		m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(OrderInfo.InstrumentID);
		m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(OrderInfo.InstrumentID);
	}
    
    LOG_INFO("CPlatformDataMgr::NewOrderReady-end");
}


//�������б����еĶ��ᱣ֤�𣬷��غ�Լ����Ƿ��б����������Ƿ���Ҫ�����ʽ���塣��ѯ��֤������Ӧʱ����
bool CPlatformDataMgr::UpdateOrderFreezeMargin_RspQryMarginRate(std::string&	strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    std::set<long>* pFTIDSet=NULL;
    PlatformStru_OrderInfo *pOrderInfo;
    if(m_OrderData.GetFTIDSetOfInstrument(pFTIDSet,strInstrumentID)&&pFTIDSet!=NULL)
    {
        std::set<long>::iterator itFTID;
        for(itFTID=pFTIDSet->begin();itFTID!=pFTIDSet->end();itFTID++)
        {
            if(m_OrderData.GetValue2_Internal(pOrderInfo,*itFTID)&&pOrderInfo!=NULL)
            {
                CalcuOrderFreezeMargin_Internal(*pOrderInfo,InstrumentInfo);
                pOrderInfo->UpdateSeq=m_OrderData.NewUpdateSeq();
            }
        }
        brlt=true;
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

//�������б����еĶ��������ѣ����غ�Լ����Ƿ��б����������Ƿ���Ҫ�����ʽ���塣��ѯ����������Ӧʱ����
bool CPlatformDataMgr::UpdateOrderFreezeCommission_RspQryCommissionRate(std::string& strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    std::set<long>* pFTIDSet=NULL;
    PlatformStru_OrderInfo *pOrderInfo;
    if(m_OrderData.GetFTIDSetOfInstrument(pFTIDSet,strInstrumentID)&&pFTIDSet!=NULL)
    {
        std::set<long>::iterator itFTID;
        for(itFTID=pFTIDSet->begin();itFTID!=pFTIDSet->end();itFTID++)
        {
            if(m_OrderData.GetValue2_Internal(pOrderInfo,*itFTID)&&pOrderInfo!=NULL)
            {
                CalcuOrderFreezeCommission_Internal(*pOrderInfo,InstrumentInfo);
                pOrderInfo->UpdateSeq=m_OrderData.NewUpdateSeq();
            }
        }
        brlt=true;
    }


    Unlock_CPlatformDataMgr;

    return brlt;
}


//���㱨����Ϣ�еĶ��ᱣ֤��
void CPlatformDataMgr::CalcuOrderFreezeMargin_Internal(PlatformStru_OrderInfo& OrderInfo,
                                                       const PlatformStru_InstrumentInfo& InstrumentInfo)
{
	PlatformStru_InstrumentMarginRate MarginRateField;// ��֤��
    double freezeMargin=util::GetDoubleInvalidValue();

    //���ᱣ֤������Ͳ���������
    if(OrderInfo.freezeMargin!=util::GetDoubleInvalidValue())
        return;

	if ( OrderInfo.CombOffsetFlag[0] == THOST_FTDC_OF_Open &&
            (OrderInfo.OrderStatus == THOST_FTDC_OST_PartTradedQueueing	||          //���ֳɽ����ڶ����� '1'				
		     OrderInfo.OrderStatus == THOST_FTDC_OST_NoTradeQueueing))	            //δ�ɽ����ڶ����� '3'

	{	
		if( ( GetMarginRate_Internal( OrderInfo.InstrumentID, MarginRateField) ) == 0 )
		{
		    if ( OrderInfo.Direction == THOST_FTDC_D_Buy ) 
            {
		        freezeMargin =  OrderInfo.LimitPrice * OrderInfo.VolumeTotal * InstrumentInfo.VolumeMultiple * MarginRateField.LongMarginRatioByMoney +
                                MarginRateField.LongMarginRatioByVolume * OrderInfo.VolumeTotal;
		    }
            else if ( OrderInfo.Direction == THOST_FTDC_D_Sell ) 
            {
		        freezeMargin =  OrderInfo.LimitPrice * OrderInfo.VolumeTotal * InstrumentInfo.VolumeMultiple * MarginRateField.ShortMarginRatioByMoney +
                                MarginRateField.ShortMarginRatioByVolume * OrderInfo.VolumeTotal;
		    }
            else freezeMargin=0.0;
        }
	}
    else
    {
        freezeMargin=0.00;    //����������ᱣ֤��Ϊ0
    }

    if(freezeMargin!=OrderInfo.freezeMargin)
    {
        OrderInfo.freezeMargin=freezeMargin;
        OrderInfo.UpdateSeq=m_OrderData.NewUpdateSeq();
    }
}

//���㱨����Ϣ�еĶ���������
void CPlatformDataMgr::CalcuOrderFreezeCommission_Internal(PlatformStru_OrderInfo& OrderInfo,
                                                           const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    PlatformStru_InstrumentCommissionRate outData;
	double troubleMoney=util::GetDoubleInvalidValue();

    //��������������Ͳ���������
    if(OrderInfo.troubleMoney!=util::GetDoubleInvalidValue())
        return;

	//����ί�е�ģ��
	if ( OrderInfo.OrderStatus == THOST_FTDC_OST_PartTradedQueueing	||  //���ֳɽ����ڶ����� '1'				
		 OrderInfo.OrderStatus == THOST_FTDC_OST_NoTradeQueueing)	    //δ�ɽ����ڶ����� '3'
	{	
		if( ( GetCommissionRate_Internal( OrderInfo.InstrumentID, outData) ) == 0 )
        {
            switch ( OrderInfo.CombOffsetFlag[0])
            {
                case THOST_FTDC_OF_Open://����
                    troubleMoney =    OrderInfo.VolumeTotal * outData.OpenRatioByVolume+
                                                OrderInfo.VolumeTotal * OrderInfo.LimitPrice * InstrumentInfo.VolumeMultiple * outData.OpenRatioByMoney;
                    break;
                case THOST_FTDC_OF_CloseToday: //ƽ�� -- ƽ��
			        troubleMoney =    OrderInfo.VolumeTotal * outData.CloseTodayRatioByVolume+
                                                OrderInfo.VolumeTotal * OrderInfo.LimitPrice * InstrumentInfo.VolumeMultiple * outData.CloseTodayRatioByMoney;
                    break;
                case THOST_FTDC_OF_Close: 
                case THOST_FTDC_OF_ForceClose: 
                case THOST_FTDC_OF_CloseYesterday: //ƽ�� -- ��ͨƽ�ֺ�ƽ��
                    troubleMoney =    OrderInfo.VolumeTotal * outData.CloseRatioByVolume+
                                                OrderInfo.VolumeTotal * OrderInfo.LimitPrice * InstrumentInfo.VolumeMultiple * outData.CloseRatioByMoney;
                    break;
                default:
                    troubleMoney = 0.0;
            }
        }
	}
    else
    {
        troubleMoney = 0.00;
    }

    if(troubleMoney!=OrderInfo.troubleMoney)
    {
        OrderInfo.troubleMoney=troubleMoney;
        OrderInfo.UpdateSeq=m_OrderData.NewUpdateSeq();
    }
}



///��ȡָ������
bool CPlatformDataMgr::GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData)
{
    bool brlt;

    Lock_CPlatformDataMgr;

    PlatformStru_OrderInfo *pOrder;
    if(m_OrderData.GetValue_Internal(pOrder,inOrderKey))
    {
        outData=*pOrder;
        brlt=true;
    }
    else
    {
        outData=PlatformStru_OrderInfo::GetInvalidRecord();
        brlt=false;
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

///��ȡȫ������
int CPlatformDataMgr::GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    int irlt;

    Lock_CPlatformDataMgr;

    irlt=m_OrderData.GetAll2_Internal(outData);

    Unlock_CPlatformDataMgr;

    return irlt;
}
///��ȡȫ������
int CPlatformDataMgr::GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    int irlt;

    Lock_CPlatformDataMgr;

    irlt=m_OrderData.GetAllAboutInstrument2_Internal(strInstrument,outData);

    Unlock_CPlatformDataMgr;

    return irlt;
}

///��ȡȫ���Ѵ����ı���
int CPlatformDataMgr::GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it;
    for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
    {
        if(it->second==NULL) continue;
        if( it->second->ContingentCondition == THOST_FTDC_CC_Immediately ||
			it->second->ContingentCondition == THOST_FTDC_CC_Touch ||
			it->second->ContingentCondition == THOST_FTDC_CC_TouchProfit)
        {
            outData.push_back(*(it->second));
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

///��ȡ��Լ��ص��Ѵ����ı���
int CPlatformDataMgr::GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    std::set<long>* pFTIDSet=NULL;
    std::set<long>::iterator it;
    PlatformStru_OrderInfo *pOrderInfo;
    long FTID;

    if(m_OrderData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
    {
        for(it=pFTIDSet->begin();it!=pFTIDSet->end();it++)
        {
            FTID=*it;
            if(m_OrderData.GetValue2_Internal(pOrderInfo,FTID)&&
                (pOrderInfo->ContingentCondition == THOST_FTDC_CC_Immediately ||
			     pOrderInfo->ContingentCondition == THOST_FTDC_CC_Touch ||
			     pOrderInfo->ContingentCondition == THOST_FTDC_CC_TouchProfit))
            {
                outData.push_back(*pOrderInfo);
            }
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

///��ȡָ���Ѵ����ı���
bool CPlatformDataMgr::GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it=m_OrderData.m_all.find(orderkey);
    if(it!=m_OrderData.m_all.end()&&
        it->second!=NULL&&
        (   it->second->ContingentCondition == THOST_FTDC_CC_Immediately ||
		    it->second->ContingentCondition == THOST_FTDC_CC_Touch ||
			it->second->ContingentCondition == THOST_FTDC_CC_TouchProfit))
    {
        outData=*(it->second);
        brlt=true;
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}


///��ȡ�ѳɽ�����
int CPlatformDataMgr::GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    std::map<OrderKey,  PlatformStru_OrderInfo*>::iterator it;
    for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
    {
        if(it->second==NULL) continue;
        if( it->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
            it->second->OrderStatus==THOST_FTDC_OST_AllTraded)
        {
            outData.push_back(*(it->second));
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

///��ȡ�ѳɽ�����
int CPlatformDataMgr::GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    std::set<long>* pFTIDSet=NULL;
    std::set<long>::iterator it;
    PlatformStru_OrderInfo *pOrderInfo;
    long FTID;

    if(m_OrderData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
    {
        for(it=pFTIDSet->begin();it!=pFTIDSet->end();it++)
        {
            FTID=*it;
            if(m_OrderData.GetValue2_Internal(pOrderInfo,FTID)&&
                (pOrderInfo->OrderStatus == THOST_FTDC_OST_PartTradedQueueing ||
			     pOrderInfo->OrderStatus == THOST_FTDC_OST_AllTraded))
            {
                outData.push_back(*pOrderInfo);
            }
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

///��ȡָ�����ѳɽ�����
bool CPlatformDataMgr::GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it=m_OrderData.m_all.find(orderkey);
    if(it!=m_OrderData.m_all.end()&&
        it->second!=NULL&&
        (   it->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
            it->second->OrderStatus==THOST_FTDC_OST_AllTraded))
    {
        outData=*(it->second);
        brlt=true;
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

///��ȡ�ѳ����ʹ��󱨵�
int CPlatformDataMgr::GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it;
    for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
    {
        if(it->second==NULL) continue;
        if( it->second->OrderStatus==THOST_FTDC_OST_Canceled||
            it->second->OrderStatus==THOST_FTDC_OST_ERROE)
        {
            outData.push_back(*(it->second));
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

///��ȡ�ѳ����ʹ��󱨵�
int CPlatformDataMgr::GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    std::set<long>* pFTIDSet=NULL;
    std::set<long>::iterator it;
    PlatformStru_OrderInfo *pOrderInfo;
    long FTID;

    if(m_OrderData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
    {
        for(it=pFTIDSet->begin();it!=pFTIDSet->end();it++)
        {
            FTID=*it;
            if(m_OrderData.GetValue2_Internal(pOrderInfo,FTID)&&
                (pOrderInfo->OrderStatus == THOST_FTDC_OST_Canceled ||
			     pOrderInfo->OrderStatus == THOST_FTDC_OST_ERROE))
            {
                outData.push_back(*pOrderInfo);
            }
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

///��ȡָ�����ѳ����ʹ��󱨵�
bool CPlatformDataMgr::GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it=m_OrderData.m_all.find(orderkey);
    if(it!=m_OrderData.m_all.end()&&
        it->second!=NULL&&
        (   it->second->OrderStatus==THOST_FTDC_OST_Canceled||
            it->second->OrderStatus==THOST_FTDC_OST_ERROE))
    {
        outData=*(it->second);
        brlt=true;
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}


///��ȡδ�ɽ�����
int CPlatformDataMgr::GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    std::map<OrderKey,  PlatformStru_OrderInfo*>::iterator it;
    for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
    {
        if(it->second==NULL) continue;
        if( it->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
            it->second->OrderStatus==THOST_FTDC_OST_NoTradeQueueing)
        {
            outData.push_back(*(it->second));
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

///��ȡδ�ɽ�����
int CPlatformDataMgr::GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

    std::set<long>* pFTIDSet=NULL;
    std::set<long>::iterator it;
    PlatformStru_OrderInfo *pOrderInfo;
    long FTID;

    if(m_OrderData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
    {
        for(it=pFTIDSet->begin();it!=pFTIDSet->end();it++)
        {
            FTID=*it;
            if(m_OrderData.GetValue2_Internal(pOrderInfo,FTID)&&
                (pOrderInfo->OrderStatus == THOST_FTDC_OST_PartTradedQueueing ||
			     pOrderInfo->OrderStatus == THOST_FTDC_OST_NoTradeQueueing))
            {
                outData.push_back(*pOrderInfo);
            }
        }
    }

    Unlock_CPlatformDataMgr;

    return outData.size();
}

///��ȡָ����δ�ɽ�����
bool CPlatformDataMgr::GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it=m_OrderData.m_all.find(orderkey);
    if(it!=m_OrderData.m_all.end()&&
        it->second!=NULL&&
        (   it->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
            it->second->OrderStatus==THOST_FTDC_OST_NoTradeQueueing))
    {
        outData=*(it->second);
        brlt=true;
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

//��ȡָ����Լ��ָ������ָ����ƽ�Ĺҵ�������������
int  CPlatformDataMgr::GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, const char CloseType, const char HedgeFlag)
{
	int tnVolume = 0;
    std::map<long,  PlatformStru_OrderInfo*>::iterator iter;
    std::set<long>::iterator itFTID;
    std::map<std::string,std::set<long>>::iterator it_Index2;

    if(strInstrumentID.empty())
        return 0;

    Lock_CPlatformDataMgr;

    it_Index2=m_OrderData.m_allIndex2.find(strInstrumentID);
    if(it_Index2!=m_OrderData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            iter=m_OrderData.m_all2.find(*itFTID);
            if(iter!=m_OrderData.m_all2.end()&&
                iter->second!=NULL&&
                iter->second->Direction == Direction &&
                iter->second->CombOffsetFlag[0] == CloseType&&
                iter->second->CombHedgeFlag[0]==HedgeFlag&&
                (iter->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
                 iter->second->OrderStatus==THOST_FTDC_OST_NoTradeQueueing)&&
			     strAccount==std::string(iter->second->szAccount))
                tnVolume += iter->second->VolumeTotal;
        }
    }

    Unlock_CPlatformDataMgr;

	return tnVolume;
}




//��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ��
//�ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
int  CPlatformDataMgr::GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
    int ret;

    Lock_CPlatformDataMgr;

    ret=GetCloseOrderVolume_Internal(strAccount,strInstrumentID,Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,HedgeFlag);

    Unlock_CPlatformDataMgr;

	return ret;
}

//��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ڲ����ã�������
//�ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
int  CPlatformDataMgr::GetCloseOrderVolume_Internal(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
    CloseVolume=CloseTodayVolume=CloseYesterdayVolume=0;

    std::map<std::string,std::set<long>>::iterator it_Index2;
    std::set<long>::iterator itFTID;
    std::map<long,  PlatformStru_OrderInfo*>::iterator iter;

    if(strInstrumentID.empty()) return -1;

    it_Index2=m_OrderData.m_allIndex2.find(strInstrumentID);
    if(it_Index2!=m_OrderData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            iter=m_OrderData.m_all2.find(*itFTID);
            if(iter!=m_OrderData.m_all2.end()&&
                iter->second!=NULL&&
                iter->second->Direction == Direction &&
                iter->second->CombHedgeFlag[0]==HedgeFlag&&
                (iter->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
                 iter->second->OrderStatus==THOST_FTDC_OST_NoTradeQueueing)&&
			     strAccount==std::string(iter->second->szAccount))
            {
                if(iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_Close||
                    iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
                    iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
                    CloseVolume += iter->second->VolumeTotal;
                if(iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
                    CloseTodayVolume += iter->second->VolumeTotal;
                if(iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
                    CloseYesterdayVolume += iter->second->VolumeTotal;
            }
        }
    }
	return 0;
}




//ȡȫ���ҵ��Ķ��������ѺͶ��ᱣ֤��
void CPlatformDataMgr::GetFreezeMargin_Commission_Internal(double& FreezeMargin,double& FreezeCommission)
{
    LOG_INFO("CPlatformDataMgr::GetFreezeMargin_Commission_Internal");
    FreezeMargin=0;
    FreezeCommission=0;

    std::map<OrderKey,  PlatformStru_OrderInfo*>::iterator iter;
    std::string strInstrumentID;

    iter = m_OrderData.m_all.begin();
    for(;iter != m_OrderData.m_all.end(); iter++)
    {
        if(iter->second!=NULL&&
            (iter->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
             iter->second->OrderStatus==THOST_FTDC_OST_NoTradeQueueing))
        {
            if(FreezeMargin!=util::GetDoubleInvalidValue())
            {
                if(iter->second->freezeMargin!=util::GetDoubleInvalidValue())
                    FreezeMargin+=iter->second->freezeMargin;
                else FreezeMargin=util::GetDoubleInvalidValue();
            }

            if(FreezeCommission!=util::GetDoubleInvalidValue())
            {
                if(iter->second->troubleMoney!=util::GetDoubleInvalidValue())
                    FreezeCommission+=iter->second->troubleMoney;
                else FreezeCommission=util::GetDoubleInvalidValue();
            }
        }
    }
    LOG_INFO("CPlatformDataMgr::GetFreezeMargin_Commission_Internal-end");
}




//��ղ�ѯ������map
void CPlatformDataMgr::ClearMapQryRltOrders()
{
    Lock_CPlatformDataMgr;

    m_OrderData.WillQry_Internal(std::string(""));

    Unlock_CPlatformDataMgr;

}

int CPlatformDataMgr::GetAllOrderFTIDList(std::vector<long>& vec)
{
	vec.clear();
	Lock_CPlatformDataMgr;
	std::set<long>::iterator it_FTID = m_OrderData.m_allFTID.begin();
	for ( ; it_FTID != m_OrderData.m_allFTID.end(); ++it_FTID )
	{
		vec.push_back(*it_FTID);
	}
	Unlock_CPlatformDataMgr;

	return vec.size();
}

int CPlatformDataMgr::GetAllOrderInfo(long lFTID, PlatformStru_OrderInfo& outData)
{
	int nRet = 0;
	Lock_CPlatformDataMgr;
	std::map<long, PlatformStru_OrderInfo*>::iterator it_FTID = m_OrderData.m_all2.find(lFTID);
	if ( it_FTID != m_OrderData.m_all2.end())
	{
		outData = *(it_FTID->second);
	}
	else
	{
		nRet = -1;
	}
	Unlock_CPlatformDataMgr;

	return nRet;
}

int CPlatformDataMgr::GetAllOrderInfo(OrderKey key, PlatformStru_OrderInfo& outData)
{
	int nRet = 0;
	Lock_CPlatformDataMgr;
	std::map<OrderKey, PlatformStru_OrderInfo*>::iterator it_all = m_OrderData.m_all.find(key);
	if ( it_all != m_OrderData.m_all.end())
	{
		outData = *(it_all->second);
	}
	else
	{
		nRet = -1;
	}
	Unlock_CPlatformDataMgr;

	return nRet;
}