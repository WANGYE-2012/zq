//ά���ײ�ֲ���Ϣ

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


//��ѯ�ĳֲּ�¼����
void CPlatformDataMgr::QryPositionReach(PlatformStru_Position& PositionInfo,const int ErrID,const bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr::QryPositionReach:InstrumentID=%s",PositionInfo.InstrumentID);

    std::set<string> setInstruments;
    std::set<string>::iterator it_Instrument;

    Lock_CPlatformDataMgr;


    if(ErrID==0)
    {
        //���ö�Ӧ��PriceTick
        PlatformStru_InstrumentInfo InstrumentInfo;
        if(GetInstrumentInfo(std::string(PositionInfo.InstrumentID),InstrumentInfo)==0)
        {
            PositionInfo.PriceTick=InstrumentInfo.PriceTick;
            memset(PositionInfo.Currency,0,sizeof(PositionInfo.Currency));
            strncpy(PositionInfo.Currency,InstrumentInfo.Currency,sizeof(PositionInfo.Currency)-1);

            //�ֲֲ�ѯ���û��ExchangeID��Ϣ����InstrumentInfo��ȡ
            memset(PositionInfo.ExchangeID,0,sizeof(PositionInfo.ExchangeID));
            strncpy(PositionInfo.ExchangeID,InstrumentInfo.ExchangeID,sizeof(PositionInfo.ExchangeID)-1);
        }
    }

    //ͬһ��λ�Ľ��ճֲֺ���ʷ�ֲַ�������¼�õ������ｫ���Ǻϲ�
	PositionKey positionKey(PositionInfo);
    PlatformStru_Position *pLastPosition;
    bool bMerge=false;
    if(ErrID==0&&m_PositionData.GetpValueInQryRlt_Internal(pLastPosition,positionKey))
    {
		MergePositions_RspQryPosition(PositionInfo, *pLastPosition);
        (*pLastPosition) = PositionInfo;
        bMerge=true;
    }
    

    if(!bMerge)
        m_PositionData.QryRltReach_Internal(PositionInfo,ErrID,bIsLast);
    else 
    {
        if(bIsLast)
            m_PositionData.QryRltReach_Internal(PositionInfo,-1,bIsLast);
    }

    if(bIsLast)
    {
        m_PositionData.GetInstrumentSetOfQryRlt(setInstruments);

		if(!setInstruments.empty())
		{

			//��̬���³ֲֵĳֲ�ӯ��������ӯ���Ͷ�����ӯ��
			std::set<std::string>::iterator it_Instruments;
			PlatformStru_DepthMarketData QuotData;
			bool bQuot;
            PlatformStru_InstrumentInfo InstrumentInfo;
            for(it_Instruments=setInstruments.begin();it_Instruments!=setInstruments.end();it_Instruments++)
			{
                if(GetInstrumentInfo(*it_Instruments,InstrumentInfo)!=0)
					break;
                 

				if(GetQuotInfo(*it_Instruments,QuotData)==0) bQuot=true;
				else bQuot=false;

                std::map<std::string,std::set<long>>::iterator it_Index2;
                std::set<long>::iterator itFTID;
				std::map<long,PlatformStru_Position*>::iterator it_position2;

				//���³ֲ������ж��Ƶ���ָ������CTP���ص���
                it_Index2=m_PositionData.m_allIndex2.find(*it_Instruments);
                if(it_Index2!=m_PositionData.m_allIndex2.end()&&!it_Index2->second.empty())
                {
                    for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
                    {
					    it_position2=m_PositionData.m_all2.find(*itFTID);
					    if(it_position2!=m_PositionData.m_all2.end()&&
                            it_position2->second!=NULL)
					    {
						    UpdatePositionFieldsWithoutCTPSupported_QryPositionReach_Internal(*(it_position2->second),InstrumentInfo);

						    if(bQuot)
						    {
							    //��̬���³ֲֵĳֲ�ӯ��������ӯ���Ͷ�����ӯ��
							    UpdatePositionByQuot_Internal(QuotData,*(it_position2->second),InstrumentInfo,true);
						    }
					    }
                    }
				}

			}
		}

    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::QryPositionReach-end");
}




//�ӳɽ���¼���³ֲ֡��˺����ӳֲ���ϸ�����´���������ֲ֡������ڳֲ���ϸ�������ڳֲ���ϸ��������ٸ��³ֲ�
void CPlatformDataMgr::UpdatePosition_RtnTrade(PlatformStru_TradeInfo& TradeInfo, 
                                               std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                               const PlatformStru_InstrumentInfo& InstrumentInfo)
{
	LOG_INFO("CPlatformDataMgr::UpdatePosition_RtnTrade(�ɽ��ر����³ֲ�):InstrumentID=%s",TradeInfo.InstrumentID);

	std::string strInstrument=std::string(TradeInfo.InstrumentID);
	if(strInstrument.empty())
		return;

	if(TradeInfo.Direction!=THOST_FTDC_D_Buy&&TradeInfo.Direction!=THOST_FTDC_D_Sell) 
		return;


    //ȷ���ֲַ���
    TThostFtdcPosiDirectionType PosiDirection;

	if(TradeInfo.OffsetFlag==THOST_FTDC_OF_Open)
	{
        PosiDirection = TradeInfo.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
	}
	else if( TradeInfo.OffsetFlag==THOST_FTDC_OF_Close||
		TradeInfo.OffsetFlag==THOST_FTDC_OF_ForceClose||
		TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseToday||
		TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseYesterday||
		TradeInfo.OffsetFlag==THOST_FTDC_OF_LocalForceClose)
	{
        PosiDirection = TradeInfo.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Short : THOST_FTDC_PD_Long;
	}
    else return;

    TThostFtdcDirectionType Direction = PosiDirection==THOST_FTDC_PD_Long ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell;
    TThostFtdcDirectionType CloseDirection = Direction==THOST_FTDC_D_Buy ? THOST_FTDC_D_Sell : THOST_FTDC_D_Buy;


    //�����ֲ�keyֵ
    PositionKey CurPositionKey( TradeInfo.szAccount,
		                        strInstrument.c_str(), 
		                        PosiDirection,
		                        TradeInfo.HedgeFlag);

    PlatformStru_Position NewPositionData;
	PlatformStru_Position* pPositionData;
    bool bNewPosition=false;


    Lock_CPlatformDataMgr;

    //�ȿ���û��ԭ�гֲ�
    std::map<PositionKey,PlatformStru_Position*>::iterator it_position;
    it_position=m_PositionData.m_all.find(CurPositionKey);
    if(it_position!=m_PositionData.m_all.end()&&it_position->second!=NULL)
    {
        pPositionData=it_position->second;
        bNewPosition=false;
    }
    else
    {
        pPositionData=&NewPositionData;
        bNewPosition=true;

        strncpy(pPositionData->szAccount,TradeInfo.szAccount,sizeof(pPositionData->szAccount)-1);
        strncpy(pPositionData->ExchangeID,TradeInfo.ExchangeID,sizeof(pPositionData->ExchangeID)-1);
	    strncpy(pPositionData->InstrumentID,TradeInfo.InstrumentID,sizeof(pPositionData->InstrumentID)-1);
	    strncpy(pPositionData->BrokerID,TradeInfo.BrokerID,sizeof(pPositionData->BrokerID)-1);
	    strncpy(pPositionData->InvestorID,TradeInfo.InvestorID,sizeof(pPositionData->InvestorID)-1);
	    pPositionData->PosiDirection=PosiDirection;
	    pPositionData->HedgeFlag=TradeInfo.HedgeFlag;
	    pPositionData->PositionDate=THOST_FTDC_PSD_Today;
	    strncpy(pPositionData->TradingDay,m_exchangeTime.TradingDay,sizeof(pPositionData->TradingDay)-1);
	    pPositionData->SettlementID=TradeInfo.SettlementID;

	    pPositionData->Position=0;
	    pPositionData->TodayPosition=0;
	    pPositionData->YdPosition=0;
        pPositionData->YdPositionRemain=0;

	    pPositionData->OpenVolume=0;
	    pPositionData->CloseVolume=0;
	    pPositionData->OpenAmount=0;
	    pPositionData->CloseAmount=0;
	    pPositionData->PositionCost=0;
	    pPositionData->OpenCost=0;

	    pPositionData->LongFrozen=util::GetIntInvalidValue();
	    pPositionData->ShortFrozen=util::GetIntInvalidValue();
	    pPositionData->LongFrozenAmount=util::GetDoubleInvalidValue();
	    pPositionData->ShortFrozenAmount=util::GetDoubleInvalidValue();
	    pPositionData->FrozenMargin=util::GetDoubleInvalidValue();
	    pPositionData->FrozenCommission=util::GetDoubleInvalidValue();
	    pPositionData->FrozenCash=util::GetDoubleInvalidValue();

	    pPositionData->Commission=0;
	    pPositionData->PreMargin=0;
	    pPositionData->UseMargin=0;
	    pPositionData->ExchangeMargin = 0;
	    pPositionData->MarginRateByMoney = util::GetDoubleInvalidValue();
	    pPositionData->MarginRateByVolume = util::GetDoubleInvalidValue();

	    pPositionData->CashIn=util::GetDoubleInvalidValue();

	    pPositionData->PositionProfit=0;
	    pPositionData->CloseProfit=0;
	    pPositionData->CloseProfitByDate=0;
	    pPositionData->CloseProfitByTrade=0;

	    pPositionData->PreSettlementPrice=util::GetDoubleInvalidValue();
	    pPositionData->SettlementPrice=util::GetDoubleInvalidValue();

	    pPositionData->CombPosition=0;
	    pPositionData->CombLongFrozen=util::GetDoubleInvalidValue();
	    pPositionData->CombShortFrozen=util::GetDoubleInvalidValue();

        pPositionData->PositionProfitByTrade=0;
	    pPositionData->TotalPositionProfitByDate=0;

        pPositionData->AveragePositionPrice=0;
	    pPositionData->AverageOpenPrice=0;

        pPositionData->TodayCloseYdVolume=0;

        pPositionData->CurPrice=TradeInfo.Price;
        pPositionData->CurPriceUsedByCalcu=TradeInfo.Price;
    }

    //���㿪������ƽ������ƽ����
    int OpenVolume,CloseTodayVolume,CloseYdVolume;
	if(TradeInfo.OffsetFlag==THOST_FTDC_OF_Open)
    {
        OpenVolume=TradeInfo.Volume;
        CloseYdVolume=0;
        CloseTodayVolume=0;
    }
    else if(TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseToday)
    {
        OpenVolume=0;
        CloseYdVolume=0;
        CloseTodayVolume=TradeInfo.Volume;
    }
    else
    {
        OpenVolume=0;
        CloseYdVolume = TradeInfo.Volume>=pPositionData->YdPositionRemain ? pPositionData->YdPositionRemain : TradeInfo.Volume;
        CloseTodayVolume=TradeInfo.Volume - CloseYdVolume;
    }

    if(CloseYdVolume>pPositionData->YdPositionRemain||
        CloseYdVolume<0||
        CloseTodayVolume>pPositionData->TodayPosition||
        CloseTodayVolume<0||
        CloseYdVolume+CloseTodayVolume>pPositionData->Position)
    {
        Unlock_CPlatformDataMgr;

        return;
    }

    if(TradeInfo.OffsetFlag==THOST_FTDC_OF_Open)
    {   
        //����
        //����ֲ֡���֡���֡�(��)��������(��)ƽ����
        pPositionData->Position += OpenVolume;
        pPositionData->TodayPosition += OpenVolume;
        pPositionData->OpenVolume+=OpenVolume;
        pPositionData->OpenAmount+=OpenVolume*TradeInfo.Price*InstrumentInfo.VolumeMultiple;

        //����ֲֳɱ��Ϳ��ֳɱ�
        pPositionData->PositionCost += OpenVolume * TradeInfo.Price * InstrumentInfo.VolumeMultiple;
        pPositionData->OpenCost += OpenVolume * TradeInfo.Price * InstrumentInfo.VolumeMultiple;
    }
    else
    {
        //ƽ��
        //����ֲ֡���֡���֡�(��)��������(��)ƽ����
        pPositionData->Position -= CloseYdVolume+CloseTodayVolume;
        pPositionData->TodayPosition -= CloseTodayVolume;
        pPositionData->YdPositionRemain -= CloseYdVolume;
        pPositionData->CloseVolume += CloseYdVolume+CloseTodayVolume;
        pPositionData->CloseAmount += (CloseYdVolume+CloseTodayVolume)*TradeInfo.Price*InstrumentInfo.VolumeMultiple;

        //����ֲֳɱ������ֳɱ���ƽ��ӯ��(���С���ʣ�����Ҫ������Чֵ)�����ݳֲ���ϸ�ı仯�����㣩
        int i,imax;
        int ClosedVolume;
        imax=ModifiedPositionDetails.size();
        for(i=0;i<imax/2;i++)
        {
            if(CurPositionKey.IsSamePositionKey(ModifiedPositionDetails[i*2]))
            {
                ClosedVolume=ModifiedPositionDetails[i*2].Volume-ModifiedPositionDetails[i*2+1].Volume;
                if(ClosedVolume>0)
                {
                    bool bTodayPositionDetail=IsTodayPositionDetail(ModifiedPositionDetails[i*2]);
                    if(bTodayPositionDetail)
                        pPositionData->PositionCost -= ClosedVolume*ModifiedPositionDetails[i*2].OpenPrice*InstrumentInfo.VolumeMultiple;
                    else 
                        pPositionData->PositionCost -= ClosedVolume*ModifiedPositionDetails[i*2].LastSettlementPrice*InstrumentInfo.VolumeMultiple;
                    pPositionData->OpenCost -= ClosedVolume*ModifiedPositionDetails[i*2].OpenPrice*InstrumentInfo.VolumeMultiple;
                    pPositionData->CloseProfit += ModifiedPositionDetails[i*2+1].CloseProfitByDate - ModifiedPositionDetails[i*2].CloseProfitByDate;
                    pPositionData->CloseProfitByDate += ModifiedPositionDetails[i*2+1].CloseProfitByDate - ModifiedPositionDetails[i*2].CloseProfitByDate;
                    pPositionData->CloseProfitByTrade += ModifiedPositionDetails[i*2+1].CloseProfitByTrade - ModifiedPositionDetails[i*2].CloseProfitByTrade;
                }
            }
        }
    }
    //����ֲ־��ۡ����־���
    if(pPositionData->Position>0&&InstrumentInfo.VolumeMultiple>0) 
	{
		pPositionData->AveragePositionPrice=pPositionData->PositionCost/pPositionData->Position/InstrumentInfo.VolumeMultiple;
		pPositionData->AverageOpenPrice=pPositionData->OpenCost/pPositionData->Position/InstrumentInfo.VolumeMultiple;
	}
    else
    {
        pPositionData->AveragePositionPrice=0;
        pPositionData->AverageOpenPrice=0;
    }

	//������½��ֲ֣����³ֲֵı��������
    if(bNewPosition)
	    UpdatePositionFreezeFieldsByAllOrders_Internal(*pPositionData);
    else
    {
        //����Ҫ���¿�ƽ������ֵ
        CalcuCloseValueInPositionByOneOrder_Internal(*pPositionData,0,0,0);
    }

    //���³ֲּ�¼�ĳֲ�ӯ��(����)������ӯ��(���)�Ͷ�����ӯ��������ۺ������ۣ���������
    PlatformStru_DepthMarketData QuotInfo;
    if(GetQuotInfo(std::string(TradeInfo.InstrumentID),QuotInfo)==0)
        UpdatePositionByQuot_Internal(QuotInfo,*pPositionData,InstrumentInfo,true); 
    
    //���³ֲּ�¼��ռ�ñ�֤��
    UpdatePositionByMarginRate_RtnTrade_Internal(*pPositionData,ModifiedPositionDetails);

    //���³ֲּ�¼��������
    if(pPositionData->Commission!=util::GetDoubleInvalidValue()&&
        TradeInfo.TradeCommission!=util::GetDoubleInvalidValue())
        pPositionData->Commission += TradeInfo.TradeCommission;
    else if(pPositionData->Commission!=util::GetDoubleInvalidValue())
        pPositionData->Commission=util::GetDoubleInvalidValue();

    pPositionData->UpdateSeq=m_PositionData.NewUpdateSeq();

    //���ֲ���Ϣ���µ��ֲ����ݼ���
    if(bNewPosition)
    {
        //���ö�Ӧ��PriceTick
        pPositionData->PriceTick=InstrumentInfo.PriceTick;
        memset(pPositionData->Currency,0,sizeof(pPositionData->Currency));
        strncpy(pPositionData->Currency,InstrumentInfo.Currency,sizeof(pPositionData->Currency)-1);

        m_PositionData.NewReach_Internal(*pPositionData);
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::UpdatePosition_RtnTrade-end");
    return;
}


//���³ֲ������ж��Ƶ���ָ������CTP���ص���
void CPlatformDataMgr::UpdatePositionFieldsWithoutCTPSupported_QryPositionReach_Internal(PlatformStru_Position& PositionData,
                                                                                         const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    PositionData.PositionProfitByTrade=util::GetDoubleInvalidValue();
    PositionData.TotalPositionProfitByDate=util::GetDoubleInvalidValue();

    PositionData.TotalCloseOrderVolume=0;
    PositionData.CloseOrderVolume=0;
    PositionData.CloseOrderVolumeTdPart=0;
    PositionData.CloseOrderVolumeYdPart=0;
    PositionData.CloseTodayOrderVolume=0;
    PositionData.CloseYesterdayOrderVolume=0;
    PositionData.CanCloseVolume=0;
    PositionData.CanCloseTodayVolume=0;
    PositionData.CanCloseydVolume=0;

	PositionData.AveragePositionPrice=util::GetDoubleInvalidValue();
	PositionData.AverageOpenPrice=util::GetDoubleInvalidValue();

	PositionData.CurPrice=util::GetDoubleInvalidValue();
	PositionData.CurPriceUsedByCalcu=util::GetDoubleInvalidValue();

    PositionData.YdPositionRemain=0;

    std::string strInstrument=std::string(PositionData.InstrumentID);
    if(strInstrument.empty())
        return;

    strncpy(PositionData.szAccount,PositionData.InvestorID,sizeof(PositionData.szAccount)-1);

	char Direction = PositionData.PosiDirection==THOST_FTDC_PD_Long?THOST_FTDC_D_Buy:THOST_FTDC_D_Sell;		//���ﲻ���Ǿ��ֲ�
	char CloseDirection = Direction==THOST_FTDC_D_Buy?THOST_FTDC_D_Sell:THOST_FTDC_D_Buy;

    PositionData.YdPositionRemain=PositionData.Position-PositionData.TodayPosition;

    //�����б�����¼�м�����ƽ�ֹҵ�����ƽ�ֹҵ�����ƽ��ҵ�����ƽ��ҵ�������ƽ��������ƽ��������ƽ����
    int CloseVolume,CloseTodayVolume,CloseYesterdayVolume;
    int ret;
	ret=GetCloseOrderVolume_Internal(std::string(PositionData.szAccount),strInstrument,CloseDirection,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,PositionData.HedgeFlag);
    if(ret==0)
    {
        CalcuCloseValueInPositionByAllOrders_Internal(PositionData,CloseVolume,CloseTodayVolume,CloseYesterdayVolume);
    }

    if(PositionData.Position>0&&InstrumentInfo.VolumeMultiple>0) 
	{
		//�ֲ־���
		PositionData.AveragePositionPrice=PositionData.PositionCost/PositionData.Position/InstrumentInfo.VolumeMultiple;
		//���־���
		PositionData.AverageOpenPrice=PositionData.OpenCost/PositionData.Position/InstrumentInfo.VolumeMultiple;
	}
    else
    {
        PositionData.AveragePositionPrice=0;
        PositionData.AverageOpenPrice=0;
    }

}




//��̬���³ֲּ�¼�ĳֲ�ӯ��(����)������ӯ��(���)��������ӯ��������ۡ������ۣ��õ�������ʱ��һ��
void CPlatformDataMgr::UpdatePositionByQuot(PlatformStru_DepthMarketData& QuotData,
                                            const PlatformStru_InstrumentInfo& InstrumentInfo,
                                            bool bForceCalculate) 
{
    std::string strInstrument(QuotData.InstrumentID);
    std::map<std::string,std::set<long>>::iterator it_Index2;
    std::set<long>::iterator itFTID;
    std::map<long,PlatformStru_Position*>::iterator it_Info2;

    Lock_CPlatformDataMgr;

    it_Index2=m_PositionData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_PositionData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            it_Info2=m_PositionData.m_all2.find(*itFTID);
            if(it_Info2!=m_PositionData.m_all2.end()&&it_Info2->second!=NULL)
            {
                UpdatePositionByQuot_Internal(QuotData,*(it_Info2->second),InstrumentInfo,bForceCalculate);
            }
        }
    }

    Unlock_CPlatformDataMgr;

}


//��̬���³ֲּ�¼�ĳֲ�ӯ��(����)������ӯ��(���)��������ӯ��������ۡ������ۣ����ݳֲ���ϸ������
void CPlatformDataMgr::UpdatePositionByQuot_Internal(PlatformStru_DepthMarketData& QuotData,
                                                     PlatformStru_Position& PositionInfo,
                                                     const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                     bool bForceCalculate) 
{
    char	Direction;
    if(PositionInfo.PosiDirection==THOST_FTDC_PD_Long) Direction=THOST_FTDC_D_Buy;
    else if(PositionInfo.PosiDirection==THOST_FTDC_PD_Short) Direction=THOST_FTDC_D_Sell;
    else return;

    std::string strInstrument(QuotData.InstrumentID);
    if(strInstrument.empty())
        return;

    if(strInstrument!=std::string(PositionInfo.InstrumentID))
        return;

    double CurPriceUsedByCalcu=QuotData.Volume>0?QuotData.LastPrice:QuotData.PreSettlementPrice;

    //��ǿ�Ƽ���ʱ����۸�δ���£������м���
    if(!bForceCalculate&&
        CurPriceUsedByCalcu==PositionInfo.CurPriceUsedByCalcu)
        return;

    double PreSettlementPrice=PositionInfo.PreSettlementPrice;
    double SettlementPrice=PositionInfo.SettlementPrice;
    double PositionProfit=PositionInfo.PositionProfit;
    double PositionProfitByTrade=PositionInfo.PositionProfitByTrade;
    double TotalPositionProfitByDate=PositionInfo.TotalPositionProfitByDate;

    double InvalideValue=util::GetDoubleInvalidValue();

    //��Ҫʱ���ý����
    if(PreSettlementPrice==util::GetDoubleInvalidValue())
        PreSettlementPrice=QuotData.PreSettlementPrice;
    if(SettlementPrice==util::GetDoubleInvalidValue())
        SettlementPrice=QuotData.SettlementPrice;

    //�õ�ǰ�۸�ֱ�Ӽ���ֲ�ӯ��������ӯ����������ӯ��
    PositionProfit=0;
    PositionProfitByTrade=0;
    TotalPositionProfitByDate=0;

    double CurCost;
    CurCost=CurPriceUsedByCalcu*PositionInfo.Position*InstrumentInfo.VolumeMultiple;
    if(PositionInfo.PosiDirection==THOST_FTDC_PD_Long)
    {
        PositionProfit=CurCost-PositionInfo.PositionCost;
        PositionProfitByTrade=CurCost-PositionInfo.OpenCost;
    }
    else 
    {
        PositionProfit=PositionInfo.PositionCost-CurCost;
        PositionProfitByTrade=PositionInfo.OpenCost-CurCost;
    }
    TotalPositionProfitByDate=PositionProfit+PositionInfo.CloseProfitByDate;

    bool bUpdate=false;
    if(PositionInfo.PreSettlementPrice!=PreSettlementPrice){ PositionInfo.PreSettlementPrice=PreSettlementPrice; bUpdate=true; }
    if(PositionInfo.SettlementPrice!=SettlementPrice){ PositionInfo.SettlementPrice=SettlementPrice; bUpdate=true; }
    if(PositionInfo.PositionProfit!=PositionProfit){ PositionInfo.PositionProfit=PositionProfit; bUpdate=true; }
    if(PositionInfo.PositionProfitByTrade!=PositionProfitByTrade){ PositionInfo.PositionProfitByTrade=PositionProfitByTrade; bUpdate=true; }
    if(PositionInfo.TotalPositionProfitByDate!=TotalPositionProfitByDate){ PositionInfo.TotalPositionProfitByDate=TotalPositionProfitByDate; bUpdate=true; }
    if(PositionInfo.CurPrice!=QuotData.LastPrice) { PositionInfo.CurPrice=QuotData.LastPrice;  bUpdate=true;}
    if(PositionInfo.CurPriceUsedByCalcu!=CurPriceUsedByCalcu) { PositionInfo.CurPriceUsedByCalcu=CurPriceUsedByCalcu;  bUpdate=true;}
    if(bUpdate)
        PositionInfo.UpdateSeq=m_PositionData.NewUpdateSeq();
}


//��̬���³ֲּ�¼�ı�֤�𡢱�֤���ʣ���ѯ��֤������Ӧʱ����
bool CPlatformDataMgr::UpdatePositionByMarginRate_RspQryRate(PlatformStru_InstrumentMarginRate& MarginRate) 
{
    bool brlt=false;
    std::string strInstrument(MarginRate.InstrumentID);
    std::map<std::string,std::set<long>>::iterator it_Index2;
    std::set<long>::iterator itFTID;
    std::map<long,PlatformStru_Position*>::iterator it_Info2;

    Lock_CPlatformDataMgr;

    it_Index2=m_PositionData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_PositionData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            it_Info2=m_PositionData.m_all2.find(*itFTID);
            if(it_Info2!=m_PositionData.m_all2.end()&&it_Info2->second!=NULL)
            {
                UpdatePositionByMarginRate_RspQryRate_FirstQuot_Internal(MarginRate,*(it_Info2->second),false);
                if(!brlt) brlt=true;
            }
        }
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

//��̬���³ֲּ�¼�ı�֤�𡢱�֤���ʣ���ѯ��֤������Ӧ�͵õ��״�����ʱ����
void CPlatformDataMgr::UpdatePositionByMarginRate_RspQryRate_FirstQuot_Internal(PlatformStru_InstrumentMarginRate& MarginRate,PlatformStru_Position& PositionInfo,bool bForce) 
{
    PlatformStru_InstrumentInfo InstrumentInfo;

    //�Ѿ�����ˣ�������
    if(!bForce&&PositionInfo.UseMargin!=util::GetDoubleInvalidValue())
    {
        LOG_INFO("CPlatformDataMgr::UpdatePositionByMarginRate_RspQryRate_FirstQuot_Internal �������㣬 InstrumentID=%s��bForce=%d, UseMargin=%g",PositionInfo.InstrumentID,bForce,PositionInfo.UseMargin);
        return;
    }

    char	Direction;
    if(PositionInfo.PosiDirection==THOST_FTDC_PD_Long) Direction=THOST_FTDC_D_Buy;
    else if(PositionInfo.PosiDirection==THOST_FTDC_PD_Short) Direction=THOST_FTDC_D_Sell;
    else return;

    std::string strInstrument(PositionInfo.InstrumentID);

    //��������Ӧ�ĺ�Լ��Ϣ
    if(GetInstrumentInfo(strInstrument,InstrumentInfo)!=0)
        return;

    double UseMargin=PositionInfo.UseMargin;
    double MarginRateByMoney=PositionInfo.MarginRateByMoney;
    double MarginRateByVolume=PositionInfo.MarginRateByVolume;

    if(PositionInfo.PosiDirection==THOST_FTDC_PD_Long)
    {
        MarginRateByMoney=MarginRate.LongMarginRatioByMoney;
        MarginRateByVolume=MarginRate.LongMarginRatioByVolume;
    }
    else
    {
        MarginRateByMoney=MarginRate.ShortMarginRatioByMoney;
        MarginRateByVolume=MarginRate.ShortMarginRatioByVolume;
    }

    UseMargin=0;


    //�����гֲ���ϸ�м��㱣֤��
    std::map<std::string,std::set<long>>::iterator it_Index2;
    std::set<long>::iterator itFTID;
    std::map<long,PlatformStru_PositionDetail*>::iterator it_Info2;

    std::string strAccount=std::string(PositionInfo.szAccount);
    it_Index2=m_PositionDetailData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_PositionDetailData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            it_Info2=m_PositionDetailData.m_all2.find(*itFTID);
            if(it_Info2==m_PositionDetailData.m_all2.end()) continue;
            if(it_Info2->second==NULL) continue;
		    if(std::string(it_Info2->second->szAccount)!=strAccount) continue;
            if(it_Info2->second->Direction!=Direction) continue;
            if(it_Info2->second->HedgeFlag!=PositionInfo.HedgeFlag) continue;

            if(it_Info2->second->Volume>0)
            {
                if(UseMargin!=util::GetDoubleInvalidValue())
                {
                    if(it_Info2->second->Margin!=util::GetDoubleInvalidValue())
                        UseMargin+=it_Info2->second->Margin;
                    else 
                    {
                        UseMargin=util::GetDoubleInvalidValue();
                        LOG_INFO("CPlatformDataMgr::UpdatePositionByMarginRate_RspQryRate_FirstQuot_Internal �ֲ���ϸ��֤����Ч�� InstrumentID=%s",PositionInfo.InstrumentID);
                        break;
                    }
                }
            }
        }
    }

    bool bUpdate=false;
    if(PositionInfo.UseMargin!=UseMargin){ PositionInfo.UseMargin=UseMargin;  bUpdate=true; }
    if(PositionInfo.MarginRateByMoney!=MarginRateByMoney){ PositionInfo.MarginRateByMoney=MarginRateByMoney;  bUpdate=true; }
    if(PositionInfo.MarginRateByVolume!=MarginRateByVolume){ PositionInfo.MarginRateByVolume=MarginRateByVolume;  bUpdate=true; }
    if(bUpdate)
        PositionInfo.UpdateSeq=m_PositionData.NewUpdateSeq();
}

//��̬���³ֲּ�¼�ı�֤�𡢱�֤���ʣ��ɽ��޸ĳֲ�ʱ����
void CPlatformDataMgr::UpdatePositionByMarginRate_RtnTrade_Internal(PlatformStru_Position& PositionInfo,
                                                                    std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails) 
{
    LOG_INFO("CPlatformDataMgr::UpdatePositionByMarginRate_RtnTrade_Internal ���³ֲ�ռ�ñ�֤�� InstrumentID=%s",PositionInfo.InstrumentID);

    char	Direction;
    if(PositionInfo.PosiDirection==THOST_FTDC_PD_Long) Direction=THOST_FTDC_D_Buy;
    else if(PositionInfo.PosiDirection==THOST_FTDC_PD_Short) Direction=THOST_FTDC_D_Sell;
    else return;

    //��ʾû�з���
    if(PositionInfo.UseMargin==util::GetDoubleInvalidValue())
    {
        LOG_INFO("CPlatformDataMgr::UpdatePositionByMarginRate_RtnTrade_Internal PositionInfo.UseMargin��Ч�� InstrumentID=%s",PositionInfo.InstrumentID);
        return;
    }

    double OldUseMargin=PositionInfo.UseMargin;

    PositionKey CurPositionKey(PositionInfo);
    //���㱣֤�𣨸��ݳֲ���ϸ�ı仯�����㣩
    int i,imax;
    imax=ModifiedPositionDetails.size();
    for(i=0;i<imax/2;i++)
    {
        if(ModifiedPositionDetails[i*2].IsInvalidRecord()&&
            !ModifiedPositionDetails[i*2+1].IsInvalidRecord()&&
            CurPositionKey.IsSamePositionKey(ModifiedPositionDetails[i*2+1]))
        {
            //���ֵ����
            if(ModifiedPositionDetails[i*2+1].Margin!=util::GetDoubleInvalidValue())
            {
                PositionInfo.UseMargin += ModifiedPositionDetails[i*2+1].Margin;
                LOG_INFO("CPlatformDataMgr::UpdatePositionByMarginRate_RtnTrade_Internal (����%s) i=%d PositionInfo.UseMargin=%g ModifiedPositionDetails[i*2+1].Margin=%g",PositionInfo.InstrumentID,i,PositionInfo.UseMargin,ModifiedPositionDetails[i*2+1].Margin);
            }
            else 
            {
                PositionInfo.UseMargin = util::GetDoubleInvalidValue();
                LOG_INFO("CPlatformDataMgr::UpdatePositionByMarginRate_RtnTrade_Internal (�������)����PositionInfo.UseMarginΪ��Ч�� InstrumentID=%s",PositionInfo.InstrumentID);
                break;
            }
        }
        else if(!ModifiedPositionDetails[i*2].IsInvalidRecord()&&
                !ModifiedPositionDetails[i*2+1].IsInvalidRecord()&&
                CurPositionKey.IsSamePositionKey(ModifiedPositionDetails[i*2]))
        {
            //ƽ�ֵ����
            if(ModifiedPositionDetails[i*2].Margin!=util::GetDoubleInvalidValue()&&
                ModifiedPositionDetails[i*2+1].Margin!=util::GetDoubleInvalidValue())
            {
                PositionInfo.UseMargin += ModifiedPositionDetails[i*2+1].Margin - ModifiedPositionDetails[i*2].Margin;
                LOG_INFO("CPlatformDataMgr::UpdatePositionByMarginRate_RtnTrade_Internal (ƽ��%s) i=%d PositionInfo.UseMargin=%g ModifiedPositionDetails[i*2/i*2+1/Delta].Margin=%g/%g/%g",PositionInfo.InstrumentID,i,PositionInfo.UseMargin,ModifiedPositionDetails[i*2].Margin,ModifiedPositionDetails[i*2+1].Margin,ModifiedPositionDetails[i*2+1].Margin-ModifiedPositionDetails[i*2].Margin);            
            }
            else 
            {
                PositionInfo.UseMargin = util::GetDoubleInvalidValue();
                LOG_INFO("CPlatformDataMgr::UpdatePositionByMarginRate_RtnTrade_Internal (ƽ�����)����PositionInfo.UseMarginΪ��Ч�� InstrumentID=%s",PositionInfo.InstrumentID);
                break;
            }
        }
    }

    if(PositionInfo.UseMargin!=OldUseMargin)
        PositionInfo.UpdateSeq=m_PositionData.NewUpdateSeq();
    LOG_INFO("CPlatformDataMgr::UpdatePositionByMarginRate_RtnTrade_Internal ���³ֲ�ռ�ñ�֤����ɣ� InstrumentID=%s UseMargin=%g UpdateSeq=%u",PositionInfo.InstrumentID,PositionInfo.UseMargin,PositionInfo.UpdateSeq);
}

//��̬���³ֲּ�¼�������ѣ��ɽ��޸ĳֲֺͲ�ѯ����������Ӧʱ����
bool CPlatformDataMgr::UpdatePositionByCommissionRate_RspQryRate(PlatformStru_InstrumentCommissionRate& CommissionRate) 
{
    bool brlt=false;
    std::string strInstrument(CommissionRate.InstrumentID);
    std::map<std::string,std::set<long>>::iterator it_Index2;
    std::set<long>::iterator itFTID;
    std::map<long,PlatformStru_Position*>::iterator it_Info2;

    Lock_CPlatformDataMgr;

    it_Index2=m_PositionData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_PositionData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            it_Info2=m_PositionData.m_all2.find(*itFTID);
            if(it_Info2!=m_PositionData.m_all2.end()&&it_Info2->second!=NULL)
            {
                UpdatePositionByCommissionRate_RspQryRate_Internal(*(it_Info2->second));
                if(!brlt)
                    brlt=true;
            }
        }
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

//��̬���³ֲּ�¼�������ѣ���ѯ����������Ӧʱ����
void CPlatformDataMgr::UpdatePositionByCommissionRate_RspQryRate_Internal(PlatformStru_Position& PositionInfo) 
{
    //�Ѿ�����ˣ�������
    if(PositionInfo.Commission!=util::GetDoubleInvalidValue())
        return;

    std::string strInstrument(PositionInfo.InstrumentID);

    double Commission=PositionInfo.Commission;

    TThostFtdcDirectionType	Direction;
    if(PositionInfo.PosiDirection==THOST_FTDC_PD_Long) Direction=THOST_FTDC_D_Buy;
    else if(PositionInfo.PosiDirection==THOST_FTDC_PD_Short) Direction=THOST_FTDC_D_Sell;
    else return;

    Commission=0.0;

    //�����гɽ���¼�м���������
    std::map<std::string,std::set<long>>::iterator it_Index2;
    std::set<long>::iterator itFTID;
    std::map<long,  PlatformStru_TradeInfo*>::iterator it_TradeInfo2;

    std::string strAccount=std::string(PositionInfo.szAccount);
    it_Index2=m_TradeData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_TradeData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            it_TradeInfo2=m_TradeData.m_all2.find(*itFTID);
            if(it_TradeInfo2==m_TradeData.m_all2.end()) continue;
            if(it_TradeInfo2->second==NULL) continue;
            if(std::string(it_TradeInfo2->second->szAccount)!=strAccount) continue;
            if(it_TradeInfo2->second->HedgeFlag!=PositionInfo.HedgeFlag) continue;
            if(it_TradeInfo2->second->Direction!=Direction) continue;

            if(it_TradeInfo2->second->TradeCommission!=util::GetDoubleInvalidValue())
            {
                Commission+=it_TradeInfo2->second->TradeCommission;
            }
            else 
            {
                Commission=util::GetDoubleInvalidValue();
                break;
            }
        }
    }


    if(PositionInfo.Commission!=Commission)
    { 
        PositionInfo.Commission=Commission;  
        PositionInfo.UpdateSeq=m_PositionData.NewUpdateSeq();
    }
}

//���ʲ�ѯ��Ӧʱ�����³ֲ���صĹҵ��Ķ��������ѺͶ��ᱣ֤��
void CPlatformDataMgr::UpdatePositionFreezeFieldsByOrders_RspQryRate(const std::string& strInstrument)
{
    std::map<std::string,std::set<long>>::iterator it_Index2;
    std::set<long>::iterator itFTID;
    std::map<long,PlatformStru_Position*>::iterator it_Info2;

    Lock_CPlatformDataMgr;

    it_Index2=m_PositionData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_PositionData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            it_Info2=m_PositionData.m_all2.find(*itFTID);
            if(it_Info2==m_PositionData.m_all2.end()||it_Info2->second==NULL) continue;

		    if(it_Info2->second->FrozenMargin==util::GetDoubleInvalidValue()||
			    it_Info2->second->FrozenCommission==util::GetDoubleInvalidValue()||
			    it_Info2->second->FrozenCash==util::GetDoubleInvalidValue())
		    {
			    UpdatePositionFreezeFieldsByAllOrders_Internal(*(it_Info2->second));
		    }
        }
    }

    Unlock_CPlatformDataMgr;
}

//�����ر�ʱ�����³ֲ���صĹҵ��Ķ��������ѺͶ��ᱣ֤��
void CPlatformDataMgr::UpdatePositionFreezeFields_RtnOrder(const PlatformStru_OrderInfo& Order, 
                                                           const PlatformStru_OrderInfo& OldOrderInfo)
{
	PositionKey tmpKey(Order);
    std::map<PositionKey,  PlatformStru_Position*>::iterator it_position;

    Lock_CPlatformDataMgr;

	it_position=m_PositionData.m_all.find(tmpKey);
    if(it_position!=m_PositionData.m_all.end()&&it_position->second!=NULL)
		UpdatePositionFreezeFieldsByOneOrder_Internal(*(it_position->second), Order, OldOrderInfo);

    Unlock_CPlatformDataMgr;

}


//���³ֲ���صĹҵ��Ķ��������ѺͶ��ᱣ֤�𡣳ɽ��½��ֲ�ʱ���õ�����ʱ����
void CPlatformDataMgr::UpdatePositionFreezeFieldsByAllOrders_Internal(PlatformStru_Position& PositionData)
{
	std::string strInstrumentID=std::string(PositionData.InstrumentID);
	if(strInstrumentID.empty())
		return;

	double OldFrozenMargin=PositionData.FrozenMargin;
	double OldFrozenCommission=PositionData.FrozenCommission;
	double OldFrozenCash=PositionData.FrozenCash;

    int OldTotalCloseOrderVolume=PositionData.TotalCloseOrderVolume;
    int OldCloseOrderVolume=PositionData.CloseOrderVolume;
    int OldCloseOrderVolumeTdPart=PositionData.CloseOrderVolumeTdPart;
    int OldCloseOrderVolumeYdPart=PositionData.CloseOrderVolumeYdPart;
    int OldCloseTodayOrderVolume=PositionData.CloseTodayOrderVolume;
    int OldCloseYesterdayOrderVolume=PositionData.CloseYesterdayOrderVolume;
    int OldCanCloseVolume=PositionData.CanCloseVolume;
    int OldCanCloseTodayVolume=PositionData.CanCloseTodayVolume;
    int OldCanCloseydVolume=PositionData.CanCloseydVolume;


	std::string strAccount=std::string(PositionData.szAccount);

	char Direction,CloseDirection;
	if(PositionData.PosiDirection==THOST_FTDC_PD_Long) Direction=THOST_FTDC_D_Buy;
	else if(PositionData.PosiDirection==THOST_FTDC_PD_Short) Direction=THOST_FTDC_D_Sell;
	else return;
	CloseDirection = Direction==THOST_FTDC_D_Buy ? THOST_FTDC_D_Sell : THOST_FTDC_D_Buy;


    //��ʱ��ȷ��������ֵ��ô��
	PositionData.LongFrozen=0;
	PositionData.ShortFrozen=0;
	PositionData.LongFrozenAmount=0;
    PositionData.ShortFrozenAmount=0;


    //����ӱ����м��㶳����
	PositionData.FrozenMargin=0;
	PositionData.FrozenCommission=0;
	PositionData.FrozenCash=0;

    //�����б�����¼�м�����ƽ�ֹҵ�����ƽ�ֹҵ�����ƽ��ҵ�����ƽ��ҵ�������ƽ��������ƽ��������ƽ����
    int CloseVolume=0,CloseTodayVolume=0,CloseYesterdayVolume=0;

    std::map<long,  PlatformStru_OrderInfo*>::iterator it_order2;
    std::map<std::string,std::set<long>>::iterator it_Index2;
    std::set<long>::iterator itFTID;

    it_Index2=m_OrderData.m_allIndex2.find(strInstrumentID);
    if(it_Index2!=m_OrderData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            it_order2=m_OrderData.m_all2.find(*itFTID);
            if(it_order2==m_OrderData.m_all2.end()||it_order2->second==NULL)
			    continue;

		    if(!(it_order2->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
                 it_order2->second->OrderStatus==THOST_FTDC_OST_NoTradeQueueing))
			    continue;
		    if(it_order2->second->CombHedgeFlag[0]!=PositionData.HedgeFlag)
			    continue;
		    if(std::string(it_order2->second->szAccount)!=strAccount)
			    continue;

            //ͳ�ƶ��ᱣ֤���������
		    if(it_order2->second->CombOffsetFlag[0]==THOST_FTDC_OF_Open&&
                it_order2->second->Direction==Direction||
		        (it_order2->second->CombOffsetFlag[0]==THOST_FTDC_OF_Close||
			     it_order2->second->CombOffsetFlag[0]==THOST_FTDC_OF_ForceClose||
			     it_order2->second->CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday||
			     it_order2->second->CombOffsetFlag[0]==THOST_FTDC_OF_CloseYesterday||
			     it_order2->second->CombOffsetFlag[0]==THOST_FTDC_OF_LocalForceClose)&&
                 it_order2->second->Direction==CloseDirection)
            {
                if(PositionData.FrozenMargin!=util::GetDoubleInvalidValue())
                {
                    if(it_order2->second->freezeMargin!=util::GetDoubleInvalidValue())
                        PositionData.FrozenMargin+=it_order2->second->freezeMargin;
                    else 
				        PositionData.FrozenMargin=util::GetDoubleInvalidValue();
                }

                if(PositionData.FrozenCommission!=util::GetDoubleInvalidValue())
                {
                    if(it_order2->second->troubleMoney!=util::GetDoubleInvalidValue())
                        PositionData.FrozenCommission+=it_order2->second->troubleMoney;
                    else 
				        PositionData.FrozenCommission=util::GetDoubleInvalidValue();
                }
            }

            //ͳ��ƽ�ֹҵ���
            if(it_order2->second->Direction==CloseDirection)
            {
                if(it_order2->second->CombOffsetFlag[0] == THOST_FTDC_OF_Close||
                    it_order2->second->CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
                    it_order2->second->CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
                    CloseVolume += it_order2->second->VolumeTotal;
                if(it_order2->second->CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
                    CloseTodayVolume += it_order2->second->VolumeTotal;
                if(it_order2->second->CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
                    CloseYesterdayVolume += it_order2->second->VolumeTotal;
            }
        }
	}
    if(PositionData.FrozenMargin!=util::GetDoubleInvalidValue()&&
		PositionData.FrozenCommission!=util::GetDoubleInvalidValue())
	{
		PositionData.FrozenCash=PositionData.FrozenMargin+PositionData.FrozenCommission;
	}
	else
	{
		PositionData.FrozenCash=util::GetDoubleInvalidValue();
	}


    //�����б�����¼�м�����ƽ�ֹҵ�����ƽ�ֹҵ�����ƽ��ҵ�����ƽ��ҵ�������ƽ��������ƽ��������ƽ����
    CalcuCloseValueInPositionByAllOrders_Internal(PositionData,CloseVolume,CloseTodayVolume,CloseYesterdayVolume);


	if( OldFrozenMargin!=PositionData.FrozenMargin||
	    OldFrozenCommission!=PositionData.FrozenCommission||
	    OldFrozenCash!=PositionData.FrozenCash||
        OldTotalCloseOrderVolume!=PositionData.TotalCloseOrderVolume||
        OldCloseOrderVolume!=PositionData.CloseOrderVolume||
        OldCloseOrderVolumeTdPart!=PositionData.CloseOrderVolumeTdPart||
        OldCloseOrderVolumeYdPart!=PositionData.CloseOrderVolumeYdPart||
        OldCloseTodayOrderVolume!=PositionData.CloseTodayOrderVolume||
        OldCloseYesterdayOrderVolume!=PositionData.CloseYesterdayOrderVolume||
        OldCanCloseVolume!=PositionData.CanCloseVolume||
        OldCanCloseTodayVolume!=PositionData.CanCloseTodayVolume||
        OldCanCloseydVolume!=PositionData.CanCloseydVolume)
    {
        PositionData.UpdateSeq=m_PositionData.NewUpdateSeq();
    }
}

//ȡ�ֲ���صĹҵ��Ķ��������ѺͶ��ᱣ֤�𣬸���һ�������ı仯�������ر�ʱ����
void CPlatformDataMgr::UpdatePositionFreezeFieldsByOneOrder_Internal(PlatformStru_Position& PositionData, 
                                                                     const PlatformStru_OrderInfo& Order, 
                                                                     const PlatformStru_OrderInfo& OldOrderInfo)
{
    std::map<OrderKey,  PlatformStru_OrderInfo>::iterator it_order;
    std::map<std::string,std::set<OrderKey>>::iterator it_Index;
    std::set<OrderKey>::iterator itKey;

	std::string strInstrumentID=std::string(PositionData.InstrumentID);
	if(strInstrumentID.empty())
		return;

	double OldFrozenMargin=PositionData.FrozenMargin;
	double OldFrozenCommission=PositionData.FrozenCommission;
	double OldFrozenCash=PositionData.FrozenCash;

    int OldTotalCloseOrderVolume=PositionData.TotalCloseOrderVolume;
    int OldCloseOrderVolume=PositionData.CloseOrderVolume;
    int OldCloseOrderVolumeTdPart=PositionData.CloseOrderVolumeTdPart;
    int OldCloseOrderVolumeYdPart=PositionData.CloseOrderVolumeYdPart;
    int OldCloseTodayOrderVolume=PositionData.CloseTodayOrderVolume;
    int OldCloseYesterdayOrderVolume=PositionData.CloseYesterdayOrderVolume;
    int OldCanCloseVolume=PositionData.CanCloseVolume;
    int OldCanCloseTodayVolume=PositionData.CanCloseTodayVolume;
    int OldCanCloseydVolume=PositionData.CanCloseydVolume;

	std::string strAccount=std::string(PositionData.szAccount);

	char Direction,CloseDirection;
	if(PositionData.PosiDirection==THOST_FTDC_PD_Long) Direction=THOST_FTDC_D_Buy;
	else if(PositionData.PosiDirection==THOST_FTDC_PD_Short) Direction=THOST_FTDC_D_Sell;
	else return;
	CloseDirection = Direction==THOST_FTDC_D_Buy ? THOST_FTDC_D_Sell : THOST_FTDC_D_Buy;


    //���㶳�ᱣ֤���������
    if(PositionData.FrozenMargin!=util::GetDoubleInvalidValue()&&
        Order.freezeMargin!=util::GetDoubleInvalidValue())
    {
        if(OldOrderInfo.IsInvalidRecord())
            PositionData.FrozenMargin += Order.freezeMargin;
        else 
            PositionData.FrozenMargin += Order.freezeMargin-OldOrderInfo.freezeMargin;
    }
    else PositionData.FrozenMargin=util::GetDoubleInvalidValue();

    if(PositionData.FrozenCommission!=util::GetDoubleInvalidValue()&&
        Order.troubleMoney!=util::GetDoubleInvalidValue())
    {
        if(OldOrderInfo.IsInvalidRecord())
            PositionData.FrozenCommission += Order.troubleMoney;
        else 
            PositionData.FrozenCommission += Order.troubleMoney-OldOrderInfo.troubleMoney;
    }
    else PositionData.FrozenCommission=util::GetDoubleInvalidValue();

    if(PositionData.FrozenMargin!=util::GetDoubleInvalidValue()&&
		PositionData.FrozenCommission!=util::GetDoubleInvalidValue())
	{
		PositionData.FrozenCash=PositionData.FrozenMargin+PositionData.FrozenCommission;
	}
	else
	{
		PositionData.FrozenCash=util::GetDoubleInvalidValue();
	}


    //����ƽ�ֹҵ�����ƽ�ֹҵ�����ƽ��ҵ�����ƽ��ҵ�������ƽ��������ƽ��������ƽ����
    int CloseVolume=0,CloseTodayVolume=0,CloseYesterdayVolume=0;
    int OldCloseVolume=0,OldCloseTodayVolume=0,OldCloseYesterdayVolume=0;
    if(Order.Direction==CloseDirection&&
       (Order.OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
        Order.OrderStatus==THOST_FTDC_OST_NoTradeQueueing))
    {
        if(Order.CombOffsetFlag[0] == THOST_FTDC_OF_Close||
            Order.CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
            Order.CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
            CloseVolume = Order.VolumeTotal;
        if(Order.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
            CloseTodayVolume = Order.VolumeTotal;
        if(Order.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
            CloseYesterdayVolume = Order.VolumeTotal;
    }
    if(!OldOrderInfo.IsInvalidRecord()&&
        OldOrderInfo.Direction==CloseDirection&&
        (OldOrderInfo.OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
         OldOrderInfo.OrderStatus==THOST_FTDC_OST_NoTradeQueueing))
    {
        if(OldOrderInfo.CombOffsetFlag[0] == THOST_FTDC_OF_Close||
            OldOrderInfo.CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
            OldOrderInfo.CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
            OldCloseVolume = OldOrderInfo.VolumeTotal;
        if(OldOrderInfo.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
            OldCloseTodayVolume = OldOrderInfo.VolumeTotal;
        if(OldOrderInfo.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
            OldCloseYesterdayVolume = OldOrderInfo.VolumeTotal;

        CloseVolume-=OldCloseVolume;
        CloseTodayVolume-=OldCloseTodayVolume;
        CloseYesterdayVolume-=CloseYesterdayVolume;
    }
    CalcuCloseValueInPositionByOneOrder_Internal(PositionData,CloseVolume,CloseTodayVolume,CloseYesterdayVolume);



	if( OldFrozenMargin!=PositionData.FrozenMargin||
	    OldFrozenCommission!=PositionData.FrozenCommission||
	    OldFrozenCash!=PositionData.FrozenCash||
        OldTotalCloseOrderVolume!=PositionData.TotalCloseOrderVolume||
        OldCloseOrderVolume!=PositionData.CloseOrderVolume||
        OldCloseTodayOrderVolume!=PositionData.CloseTodayOrderVolume||
        OldCloseYesterdayOrderVolume!=PositionData.CloseYesterdayOrderVolume||
        OldCanCloseVolume!=PositionData.CanCloseVolume||
        OldCanCloseTodayVolume!=PositionData.CanCloseTodayVolume||
        OldCanCloseydVolume!=PositionData.CanCloseydVolume)
    {
        PositionData.UpdateSeq=m_PositionData.NewUpdateSeq();
    }
}


//�ڳֲ������У�����ƽ�����Ϳ�ƽ����, (�������йҵ�)����ȫ��
void CPlatformDataMgr::CalcuCloseValueInPositionByAllOrders_Internal(PlatformStru_Position& PositionData,
                                                                     const int CloseVolume,
                                                                     const int CloseTodayVolume,
                                                                     const int CloseYesterdayVolume)
{
//    PositionData.TotalCloseOrderVolume = CloseVolume+CloseTodayVolume+CloseYesterdayVolume;
    PositionData.CloseOrderVolume = CloseVolume;
    PositionData.CloseTodayOrderVolume = CloseTodayVolume;
    PositionData.CloseYesterdayOrderVolume = CloseYesterdayVolume;
//    PositionData.CanCloseVolume=PositionData.Position-PositionData.TotalCloseOrderVolume;
//    PositionData.CanCloseTodayVolume=PositionData.TodayPosition-PositionData.CloseTodayOrderVolume;
//    PositionData.CanCloseydVolume=PositionData.YdPositionRemain-PositionData.CloseYesterdayOrderVolume;

//    int Today_CloseVolume=0;      //ƽ�������ƽ�񲿷�
//    int Yesterday_CloseVolume=0;  //ƽ�������ƽ�򲿷�
//    if ( CloseVolume > 0 )
//	{
//		if ( CloseVolume <= PositionData.YdPositionRemain - PositionData.CloseYesterdayOrderVolume )
//		{
  //          Yesterday_CloseVolume = CloseVolume;
//		}
//		else
//		{
//            Yesterday_CloseVolume = PositionData.YdPositionRemain - PositionData.CloseYesterdayOrderVolume;
//            Today_CloseVolume = CloseVolume-Yesterday_CloseVolume;
//		}
//        PositionData.CanCloseTodayVolume -= Today_CloseVolume;
//        PositionData.CanCloseydVolume -= Yesterday_CloseVolume;
//	}


    PositionData.CloseOrderVolumeYdPart = 0;
    PositionData.CloseOrderVolumeTdPart = 0;
    if ( PositionData.CloseOrderVolume > 0 )
	{                               
		if ( PositionData.CloseOrderVolume <= PositionData.YdPositionRemain - PositionData.CloseYesterdayOrderVolume )
		{
            PositionData.CloseOrderVolumeYdPart = PositionData.CloseOrderVolume;
		}
		else
		{
            PositionData.CloseOrderVolumeYdPart = PositionData.YdPositionRemain - PositionData.CloseYesterdayOrderVolume;
            PositionData.CloseOrderVolumeTdPart = PositionData.CloseOrderVolume-PositionData.CloseOrderVolumeYdPart;
		}
	}
    PositionData.TotalCloseOrderVolume = PositionData.CloseOrderVolume + PositionData.CloseTodayOrderVolume + PositionData.CloseYesterdayOrderVolume;
    PositionData.CanCloseVolume=PositionData.Position-PositionData.TotalCloseOrderVolume;
    PositionData.CanCloseTodayVolume=PositionData.TodayPosition-PositionData.CloseTodayOrderVolume-PositionData.CloseOrderVolumeTdPart;
    PositionData.CanCloseydVolume=PositionData.YdPositionRemain-PositionData.CloseYesterdayOrderVolume-PositionData.CloseOrderVolumeYdPart;
}

//�ڳֲ������У�����ƽ�����Ϳ�ƽ����, (����һ���ҵ�)������������
//Deltaxxx��������һ����˵�Ǹ�ֵ
void CPlatformDataMgr::CalcuCloseValueInPositionByOneOrder_Internal(PlatformStru_Position& PositionData,
                                                                    const int DeltaCloseVolume,
                                                                    const int DeltaCloseTodayVolume,
                                                                    const int DeltaCloseYesterdayVolume)
{
//    PositionData.TotalCloseOrderVolume += DeltaCloseVolume+DeltaCloseTodayVolume+DeltaCloseYesterdayVolume;
    PositionData.CloseOrderVolume += DeltaCloseVolume;
    PositionData.CloseTodayOrderVolume += DeltaCloseTodayVolume;
    PositionData.CloseYesterdayOrderVolume += DeltaCloseYesterdayVolume;
//    PositionData.CanCloseVolume=PositionData.Position-PositionData.TotalCloseOrderVolume;
//    PositionData.CanCloseTodayVolume=PositionData.TodayPosition-PositionData.CloseTodayOrderVolume;
//    PositionData.CanCloseydVolume=PositionData.YdPositionRemain-PositionData.CloseYesterdayOrderVolume;

    //���洦��ƽ�ֵ������
    //ƽ�ֱȽϸ��ӣ����ܰ���ƽ��ֲ��ֺ�ƽ��ֲ��֣������������ֵķ����ڳɽ�ʱ����ȷ����
    //���DeltaCloseVolume>0����ʾ��ƽ�ֵ�������ֱ�ӷ���
    //���DeltaCloseVolume<0����ʾ�ɽ��򳷵�����Ҫ���������ִ汨�������¼���

    PositionData.CloseOrderVolumeYdPart = 0;
    PositionData.CloseOrderVolumeTdPart = 0;
    if ( PositionData.CloseOrderVolume > 0 )
	{                               
		if ( PositionData.CloseOrderVolume <= PositionData.YdPositionRemain - PositionData.CloseYesterdayOrderVolume )
		{
            PositionData.CloseOrderVolumeYdPart = PositionData.CloseOrderVolume;
		}
		else
		{
            PositionData.CloseOrderVolumeYdPart = PositionData.YdPositionRemain - PositionData.CloseYesterdayOrderVolume;
            PositionData.CloseOrderVolumeTdPart = PositionData.CloseOrderVolume-PositionData.CloseOrderVolumeYdPart;
		}
//		PositionData.CloseTodayOrderVolume += Today_CloseVolume;
//		PositionData.CloseYesterdayOrderVolume = Yesterday_CloseVolume ;
//        PositionData.CanCloseTodayVolume -= Today_CloseVolume;
//        PositionData.CanCloseydVolume -= Yesterday_CloseVolume;
	}
    PositionData.TotalCloseOrderVolume = PositionData.CloseOrderVolume + PositionData.CloseTodayOrderVolume + PositionData.CloseYesterdayOrderVolume;
    PositionData.CanCloseVolume=PositionData.Position-PositionData.TotalCloseOrderVolume;
    PositionData.CanCloseTodayVolume=PositionData.TodayPosition-PositionData.CloseTodayOrderVolume-PositionData.CloseOrderVolumeTdPart;
    PositionData.CanCloseydVolume=PositionData.YdPositionRemain-PositionData.CloseYesterdayOrderVolume-PositionData.CloseOrderVolumeYdPart;
}




//��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
int CPlatformDataMgr::GetPositionKeySet(std::set<PositionKey> &PositionKeySet)
{
    LOG_INFO("CPlatformDataMgr::GetPositionKeySet");

    PositionKeySet.clear();

    std::map<PositionKey,PlatformStru_Position*>::iterator it;

    Lock_CPlatformDataMgr;

    it=m_PositionData.m_all.begin();
    for(;it!=m_PositionData.m_all.end();it++)
    {
        if(it->second!=NULL)
            PositionKeySet.insert(it->first);
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::GetPositionKeySet-end");
    return PositionKeySet.size();
}

//��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr::GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char PosiDirection,const char HedgeFlag,PlatformStru_Position& PositionData)
{
    LOG_INFO("CPlatformDataMgr::GetPositionData");

    PositionKey tmpKey( strAccount.c_str(),
						InstrumentID.c_str(), 
                        PosiDirection,
                        HedgeFlag);

    int irlt;

    Lock_CPlatformDataMgr;

    std::map<PositionKey,PlatformStru_Position*>::iterator it=m_PositionData.m_all.find(tmpKey);
    if(it!=m_PositionData.m_all.end()&&it->second!=NULL)
    {
        PositionData=*(it->second);
        irlt=0;
    }
    else irlt=-1;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::GetPositionData-end");
    return irlt;
}

int CPlatformDataMgr::GetPositions(std::vector<PlatformStru_Position>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

	std::map<PositionKey,PlatformStru_Position*>::iterator it = m_PositionData.m_all.begin();
	for ( ; it != m_PositionData.m_all.end(); ++it )
	{
        if ( it->second!=NULL &&
             it->second->Position > 0 )
		{
            outData.push_back(*(it->second));
		}
	}

    Unlock_CPlatformDataMgr;

    return outData.size();
}


//����0���ɹ������������ݣ�1: �ɹ���������û�и��£����������ݣ�-1��ʧ��
int CPlatformDataMgr::GetPositions2(const std::string& strInstrument,
                                    std::set<long>& setFTID,
                                    std::vector<PlatformStru_Position>& outData,
                                    long& lastseq)
{
    int rlt;

    outData.clear();

    Lock_CPlatformDataMgr;

    if(lastseq==m_PositionData.GetUpdateSeq())
    {
        rlt=1;
    }
    else
    {
        std::set<long>* pFTIDSet=NULL;
        if(m_PositionData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
        {
            setFTID=(*pFTIDSet);

            PlatformStru_Position *pData=NULL;
            std::set<long>::iterator itFTID;
            for(itFTID=pFTIDSet->begin();itFTID!=pFTIDSet->end();itFTID++)
            {
                if(m_PositionData.GetValue2_Internal(pData,(*itFTID))&&
                    pData->Position>0)
                {
                    outData.push_back(*pData);
                }
                else
                {
                    setFTID.erase(*itFTID);
                }
            }
        }
        lastseq=m_PositionData.GetUpdateSeq();
        rlt=0;
    }

    Unlock_CPlatformDataMgr;

    return rlt;
}

//����0���ɹ������������ݣ�1: �ɹ���������û�и��£����������ݣ�-1��ʧ��
int CPlatformDataMgr::GetPositions3(const std::string& strInstrument,
                                    std::vector<PlatformStru_Position>& outData,
                                    long& lastseq)
{
    int rlt;

    outData.clear();

    Lock_CPlatformDataMgr;

    if(lastseq==m_PositionData.GetUpdateSeq())
    {
        rlt=1;
    }
    else
    {
        std::set<long>* pFTIDSet;
        if(m_PositionData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
        {
            PlatformStru_Position *pData;
            std::set<long>::iterator itFTID;
            for(itFTID=pFTIDSet->begin();itFTID!=pFTIDSet->end();itFTID++)
            {
                if(m_PositionData.GetValue2_Internal(pData,(*itFTID))&&
                    pData->Position>0)
                {
                    outData.push_back(*pData);
                }
            }
        }

        lastseq=m_PositionData.GetUpdateSeq();
        rlt=0;
    }

    Unlock_CPlatformDataMgr;

    return rlt;
}

//��ղ�ѯ�ֲֵ�map
void CPlatformDataMgr::ClearMapQryRltPositions(std::string& strQryInstrument)
{
    Lock_CPlatformDataMgr;

    m_PositionData.WillQry_Internal(strQryInstrument);

    Unlock_CPlatformDataMgr;

}

//��ȡ��ѯ�ĺ�ԼID
void CPlatformDataMgr::GetQryPositionInstrument(std::string& outQryInstrument)
{
    outQryInstrument.clear();

    Lock_CPlatformDataMgr;

    m_PositionData.GetQryInstrument(outQryInstrument);

    Unlock_CPlatformDataMgr;

}

//��ѯ�ֲ�ʱ�ϲ�ͬһ��Լͬһ�������ʷ�ֲֺͽ��ճֲּ�¼
void CPlatformDataMgr::MergePositions_RspQryPosition( PlatformStru_Position& firstPositionInfo, PlatformStru_Position secondPositionInfo )
{
	firstPositionInfo.Position += secondPositionInfo.Position;
	firstPositionInfo.TodayPosition += secondPositionInfo.TodayPosition;
    firstPositionInfo.YdPosition += secondPositionInfo.YdPosition;
	firstPositionInfo.OpenVolume += secondPositionInfo.OpenVolume; 
	firstPositionInfo.CloseVolume += secondPositionInfo.CloseVolume; 
	firstPositionInfo.OpenAmount += secondPositionInfo.OpenAmount;
	firstPositionInfo.CloseAmount += secondPositionInfo.CloseAmount;
	firstPositionInfo.PositionCost += secondPositionInfo.PositionCost;
	firstPositionInfo.OpenCost += secondPositionInfo.OpenCost;

	firstPositionInfo.LongFrozen += secondPositionInfo.LongFrozen;
	firstPositionInfo.ShortFrozen += secondPositionInfo.ShortFrozen;
	firstPositionInfo.LongFrozenAmount += secondPositionInfo.LongFrozenAmount;
	firstPositionInfo.ShortFrozenAmount += secondPositionInfo.ShortFrozenAmount;
	firstPositionInfo.FrozenMargin += secondPositionInfo.FrozenMargin;
	firstPositionInfo.FrozenCommission += secondPositionInfo.FrozenCommission;
	firstPositionInfo.FrozenCash += secondPositionInfo.FrozenCash;

	firstPositionInfo.Commission += secondPositionInfo.Commission;
	firstPositionInfo.PreMargin += secondPositionInfo.PreMargin;
	firstPositionInfo.UseMargin += secondPositionInfo.UseMargin;
	firstPositionInfo.ExchangeMargin += secondPositionInfo.ExchangeMargin;
    if(firstPositionInfo.MarginRateByMoney==util::GetDoubleInvalidValue()&&
        secondPositionInfo.MarginRateByMoney!=util::GetDoubleInvalidValue())
	    firstPositionInfo.MarginRateByMoney = secondPositionInfo.MarginRateByMoney; 
    if(firstPositionInfo.MarginRateByVolume==util::GetDoubleInvalidValue()&&
        secondPositionInfo.MarginRateByVolume!=util::GetDoubleInvalidValue())
	    firstPositionInfo.MarginRateByVolume = secondPositionInfo.MarginRateByVolume;

	firstPositionInfo.CashIn += secondPositionInfo.CashIn;
	
	firstPositionInfo.PositionProfit += secondPositionInfo.PositionProfit;
	firstPositionInfo.CloseProfit += secondPositionInfo.CloseProfit;
	firstPositionInfo.CloseProfitByDate += secondPositionInfo.CloseProfitByDate;
	firstPositionInfo.CloseProfitByTrade += secondPositionInfo.CloseProfitByTrade;

    if(firstPositionInfo.PreSettlementPrice==util::GetDoubleInvalidValue()&&
        secondPositionInfo.PreSettlementPrice!=util::GetDoubleInvalidValue())
	    firstPositionInfo.PreSettlementPrice = secondPositionInfo.PreSettlementPrice; 
    if(firstPositionInfo.SettlementPrice==util::GetDoubleInvalidValue()&&
        secondPositionInfo.SettlementPrice!=util::GetDoubleInvalidValue())
	    firstPositionInfo.SettlementPrice = secondPositionInfo.SettlementPrice; 

	firstPositionInfo.CombPosition += secondPositionInfo.CombPosition;
	firstPositionInfo.CombLongFrozen += secondPositionInfo.CombLongFrozen;
	firstPositionInfo.CombShortFrozen += secondPositionInfo.CombShortFrozen;
}

int CPlatformDataMgr::GetPositionFTIDList(std::vector<long>& vec)
{
	vec.clear();
	Lock_CPlatformDataMgr;
	std::set<long>::iterator it_FTID = m_PositionData.m_allFTID.begin();
	for ( ; it_FTID != m_PositionData.m_allFTID.end(); ++it_FTID )
	{
		vec.push_back(*it_FTID);
	}
	Unlock_CPlatformDataMgr;

	return vec.size();
}

int CPlatformDataMgr::GetPositionInfo(long lFTID, PlatformStru_Position& outData)
{
	int nRet = 0;
	Lock_CPlatformDataMgr;
	std::map<long, PlatformStru_Position*>::iterator it_FTID = m_PositionData.m_all2.find(lFTID);
	if ( it_FTID != m_PositionData.m_all2.end())
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

int CPlatformDataMgr::GetPositionInfo(PositionKey key, PlatformStru_Position& outData)
{
	int nRet = 0;
	Lock_CPlatformDataMgr;
	std::map<PositionKey, PlatformStru_Position*>::iterator it_all = m_PositionData.m_all.find(key);
	if ( it_all != m_PositionData.m_all.end())
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