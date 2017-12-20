//ά���ײ�ɽ���¼��Ϣ

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
#include "../inc/Module-Misc/FieldValueTool.h"
#pragma warning(disable:4996)

#if 0
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


//��ѯ�ĳɽ��ر�����
void CPlatformDataMgr::QryTradeReach( PlatformStru_TradeInfo& TradeInfo,const int ErrID,const bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr::QryTradeReach");

    Lock_CPlatformDataMgr;

    if(ErrID==0)
    {
        //���ö�Ӧ��PriceTick
        PlatformStru_InstrumentInfo InstrumentInfo;
        if(GetInstrumentInfo(std::string(TradeInfo.InstrumentID),InstrumentInfo)==0)
        {
            TradeInfo.PriceTick=InstrumentInfo.PriceTick;
        }
    }

    m_TradeData.QryRltReach_Internal(TradeInfo,ErrID,bIsLast);


    if(bIsLast)
    {
        std::map<TradeKey,  PlatformStru_TradeInfo*>::iterator it_all;
        PlatformStru_InstrumentInfo InstrumentInfo;
        
        for(it_all=m_TradeData.m_all.begin();it_all!=m_TradeData.m_all.end();it_all++)
        {
            if(it_all->second==NULL) continue;
            if(GetInstrumentInfo(it_all->first.InstrumentID,InstrumentInfo)<0)
		        continue;
            CalcuTradeCommission_Internal(*(it_all->second),InstrumentInfo);
        }
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::QryTradeReach-end");
}

//�µĳɽ��ر�����
void CPlatformDataMgr::NewTradeReady( PlatformStru_TradeInfo& TradeInfo,
                                     const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    LOG_INFO("CPlatformDataMgr::NewTradeReady InstrumentID=%s TradeID=%s",TradeInfo.InstrumentID,TradeInfo.TradeID);

    Lock_CPlatformDataMgr;

    CalcuTradeCommission_Internal(TradeInfo,InstrumentInfo);

    //���ö�Ӧ��PriceTick
    TradeInfo.PriceTick=InstrumentInfo.PriceTick;

    m_TradeData.NewReach_Internal(TradeInfo);


    Unlock_CPlatformDataMgr;

	if ( m_PlatFormService )
	{
		m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(TradeInfo.InstrumentID);
		m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(TradeInfo.InstrumentID);
	}

    LOG_INFO("CPlatformDataMgr::NewTradeReady-end TradeID=%s FTID=%u",TradeInfo.TradeID,TradeInfo.FTID);

}

//�������гɽ��е������ѣ����غ�Լ����Ƿ��гɽ����Դ˾����Ƿ���Ҫ�����ʽ���塣��ѯ����������Ӧʱ����
bool CPlatformDataMgr::UpdateTradeCommission_RspQryCommissionRate(std::string&	strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    std::map<std::string,std::set<long>>::iterator it_Index2;
    std::set<long>::iterator itFTID;
    std::map<long,PlatformStru_TradeInfo*>::iterator it2;

    it_Index2=m_TradeData.m_allIndex2.find(strInstrumentID);
    if(it_Index2!=m_TradeData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        brlt=true;
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            it2=m_TradeData.m_all2.find(*itFTID);
            if(it2!=m_TradeData.m_all2.end()&&it2->second!=NULL)
            {
                CalcuTradeCommission_Internal(*(it2->second),InstrumentInfo);
                it2->second->UpdateSeq=m_TradeData.NewUpdateSeq();
            }
        }
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

//����ɽ�������
void CPlatformDataMgr::CalcuTradeCommission_Internal(PlatformStru_TradeInfo& TradeInfo,const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    PlatformStru_InstrumentCommissionRate CommissionRate;
    std::string strInstrumentID;

    //�ɽ�����������Ͳ���������
    if(TradeInfo.TradeCommission!=util::GetDoubleInvalidValue())
        return;

    strInstrumentID=std::string(TradeInfo.InstrumentID);
    if(GetCommissionRate_Internal(strInstrumentID,CommissionRate)==0)
    {
        //�����µ������Ѽ��㹫ʽ����20111222
        if(TradeInfo.OffsetFlag == THOST_FTDC_OF_Open)
            TradeInfo.TradeCommission=  TradeInfo.Price*TradeInfo.Volume*InstrumentInfo.VolumeMultiple*CommissionRate.OpenRatioByMoney+
                                TradeInfo.Volume*CommissionRate.OpenRatioByVolume;
        else if(TradeInfo.OffsetFlag == THOST_FTDC_OF_Close||
                TradeInfo.OffsetFlag == THOST_FTDC_OF_CloseYesterday||
                TradeInfo.OffsetFlag == THOST_FTDC_OF_ForceClose||
                TradeInfo.OffsetFlag == THOST_FTDC_OF_LocalForceClose)
            TradeInfo.TradeCommission=  TradeInfo.Price*TradeInfo.Volume*InstrumentInfo.VolumeMultiple*CommissionRate.CloseRatioByMoney+
                                TradeInfo.Volume*CommissionRate.CloseRatioByVolume;
        else if(TradeInfo.OffsetFlag == THOST_FTDC_OF_CloseToday)
            TradeInfo.TradeCommission=  TradeInfo.Price*TradeInfo.Volume*InstrumentInfo.VolumeMultiple*CommissionRate.CloseTodayRatioByMoney+
                                TradeInfo.Volume*CommissionRate.CloseTodayRatioByVolume;
    }

}


///��ȡȫ���ɽ���
int CPlatformDataMgr::GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData)
{
    int irlt;
    Lock_CPlatformDataMgr;

    irlt=m_TradeData.GetAll2_Internal(outData);

    Unlock_CPlatformDataMgr;

    return irlt;
}

///��ȡָ���ɽ���Ϣ
bool CPlatformDataMgr::GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData)
{
    bool brlt;
    Lock_CPlatformDataMgr;

    PlatformStru_TradeInfo *pTrade;
    if(m_TradeData.GetValue_Internal(pTrade,tradekey))
    {
        outData=*pTrade;
        brlt=true;
    }
    else
    {
        brlt=false;
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

 ///��ȡȫ���ɽ���
int CPlatformDataMgr::GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData)
{
    int irlt;
    Lock_CPlatformDataMgr;

    irlt=m_TradeData.GetAllAboutInstrument2_Internal(strInstrument,outData);

    Unlock_CPlatformDataMgr;

    return irlt;
}







//�ӳɽ���ȡ�����ѡ����û�з��ʣ�����Ϊ��Чֵ
void CPlatformDataMgr::GetAllTradeCommission_Internal(double& TotalCommission)
{
    LOG_INFO("CPlatformDataMgr::GetAllTradeCommission_Internal");
    TotalCommission=0;

    std::map<TradeKey,  PlatformStru_TradeInfo*>::iterator iter;
    PlatformStru_InstrumentCommissionRate CommissionRate;
    PlatformStru_InstrumentInfo InstrumentInfo;
    std::string strInstrumentID;

    for(iter = m_TradeData.m_all.begin();iter != m_TradeData.m_all.end(); iter++)
    {
        if(iter->second==NULL) continue;

        strInstrumentID=iter->first.InstrumentID;
        if(GetInstrumentInfo(strInstrumentID,InstrumentInfo)<0||
           GetCommissionRate_Internal(strInstrumentID,CommissionRate)<0)
        {
            TotalCommission=util::GetDoubleInvalidValue();
            return;
        }

        if(iter->second->OffsetFlag == THOST_FTDC_OF_Open)
            TotalCommission+=     iter->second->Price*iter->second->Volume
                                * InstrumentInfo.VolumeMultiple
                                * CommissionRate.OpenRatioByMoney
                                + iter->second->Volume
                                * CommissionRate.OpenRatioByVolume;
        else if(iter->second->OffsetFlag == THOST_FTDC_OF_Close||
                iter->second->OffsetFlag == THOST_FTDC_OF_CloseYesterday||
                iter->second->OffsetFlag == THOST_FTDC_OF_ForceClose||
                iter->second->OffsetFlag == THOST_FTDC_OF_LocalForceClose)
            TotalCommission+=     iter->second->Price
                                * iter->second->Volume
                                * InstrumentInfo.VolumeMultiple
                                * CommissionRate.CloseRatioByMoney
                                + iter->second->Volume
                                * CommissionRate.CloseRatioByVolume;
        else if(iter->second->OffsetFlag == THOST_FTDC_OF_CloseToday)
            TotalCommission+=     iter->second->Price
                                * iter->second->Volume
                                * InstrumentInfo.VolumeMultiple
                                * CommissionRate.CloseTodayRatioByMoney
                                + iter->second->Volume
                                * CommissionRate.CloseTodayRatioByVolume;
    }
    LOG_INFO("CPlatformDataMgr::GetAllTradeCommission_Internal-end");
}


//��ȡ������
double CPlatformDataMgr::GetShouXuFei(const PlatformStru_TradeInfo& TradeInfo)
{
    double Commission;

    Lock_CPlatformDataMgr;

    TradeKey tmpKey(TradeInfo);
    PlatformStru_TradeInfo *pTrade;
    if(m_TradeData.GetValue_Internal(pTrade,tmpKey))
    {
        Commission=pTrade->TradeCommission;
    }
    else Commission=util::GetDoubleInvalidValue();

    Unlock_CPlatformDataMgr;

    return Commission;
}

//��ղ�ѯ�ɽ���map
void CPlatformDataMgr::ClearMapQryRltTrades()
{
    Lock_CPlatformDataMgr;

    m_TradeData.WillQry_Internal(std::string(""));

    Unlock_CPlatformDataMgr;

}

