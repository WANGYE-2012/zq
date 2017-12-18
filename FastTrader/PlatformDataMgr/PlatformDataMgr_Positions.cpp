//ά���ײ�ֲ���Ϣ

#pragma warning(disable:4996)

#include <vector>
#include <string>
#include <map>
using std::vector;
using std::string;
using std::map;

#include "PlatformDataMgr_Positions.h"
#include "PlatformDataMgrMacro.h"
#include "FileOpr.h"
#include "Module-Misc2/GlobalFunc.h"


#ifdef WIN32
#pragma unmanaged
#endif 



CPlatformDataMgr_Positions::CPlatformDataMgr_Positions(
    volatile long& baseFTID,
    volatile long& baseUpdateSeq,
    CPlatformDataMgr_Instruments& refInstruments,
    CPlatformDataMgr_Rates& refRates,
    CPlatformDataMgr_Quots& refQuots,
    CPlatformDataMgr_Trades& refTrades,
    CPlatformDataMgr_Orders& refOrders,
    CPlatformDataMgr_PositionDetails& refPositionDetails,
    bool bUseSettlementPriceToCalculateProfitAndMarginAsPossible)
: m_RefInstruments(refInstruments),
  m_RefRates(refRates),
  m_RefQuots(refQuots),
  m_RefTrades(refTrades),
  m_RefOrders(refOrders),
  m_RefPositionDetails(refPositionDetails),
  m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible(bUseSettlementPriceToCalculateProfitAndMarginAsPossible),
  m_PositionData(baseFTID,baseUpdateSeq)
{
    //��ѯ�������ʱ���Ƿ���Ҫ����mapQryRlt. ���ڳֲ֣���Ҫ���������ںϲ����ճֲֺ���ʷ�ֲ�
    m_PositionData.SetbNeedKey2QryRlt(true);
}

CPlatformDataMgr_Positions::~CPlatformDataMgr_Positions(void)
{
    LOG_INFO("CPlatformDataMgr_Positions::~CPlatformDataMgr_Positions(����CPlatformDataMgr_Positions����)");
}

//��ѯ�ر�����
void CPlatformDataMgr_Positions::QryPositionReach( PlatformStru_Position& PositionInfo,int ErrID,bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr_Positions::QryPositionReach:InstrumentID=%s",PositionInfo.InstrumentID);

    string strInstrumentID(PositionInfo.InstrumentID);
    PlatformStru_InstrumentInfo InstrumentInfo;

    Lock_CPlatformDataMgr;

    //����ֻ�����Ⱥ�Լ�ĳֲ֡��������Ϻ�Լ��ֻ����bIsLast�����
    if(ErrID==0)
    {
        if(!m_RefInstruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo))
            ErrID=-1;
        else if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Combination)
            ErrID=-1;
        else 
        {
            //���ö�Ӧ��PriceTick��Currency��ExchangeID(�ֲֲ�ѯ�������û��ExchangeID��Ϣ����InstrumentInfo��ȡ)
            PositionInfo.PriceTick=InstrumentInfo.PriceTick;
            memset(PositionInfo.Currency,0,sizeof(PositionInfo.Currency));
            strncpy(PositionInfo.Currency,InstrumentInfo.Currency,sizeof(PositionInfo.Currency)-1);
            memset(PositionInfo.ExchangeID,0,sizeof(PositionInfo.ExchangeID));
            strncpy(PositionInfo.ExchangeID,InstrumentInfo.ExchangeID,sizeof(PositionInfo.ExchangeID)-1);

            //�п���ͬһ��λ�Ľ��ճֲֺ���ʷ�ֲַ�������¼�õ������ｫ���Ǻϲ�
	        PositionKey positionKey(PositionInfo);
            PlatformStru_Position *pLastPosition;
            if(m_PositionData.GetpValueInQryRlt_Internal(pLastPosition,positionKey))
            {
		        MergePositions_RspQryPosition(*pLastPosition,PositionInfo);

                //���������Ѿ��ϲ���ԭ���ݼ�¼���ˣ��������Ѿ��������á���ErrID����Ϊ-1���������ͳһ����
                ErrID=-1;
            }
        }
    }

    m_PositionData.QryRltReach_Internal(PositionInfo,ErrID,bIsLast);

    if(bIsLast)
    {
        set<string> setInstruments;
        m_PositionData.GetInstrumentSetOfQryRlt(setInstruments);

		//��̬���³ֲֵĳֲ�ӯ��������ӯ���Ͷ�����ӯ��
        for(set<string>::iterator it_Instruments=setInstruments.begin();it_Instruments!=setInstruments.end();it_Instruments++)
		{
            PlatformStru_InstrumentInfo InstrumentInfo;
            if(!m_RefInstruments.GetInstrumentInfo(*it_Instruments,InstrumentInfo))
                break;

            //ȡʵʱ��������
		    PlatformStru_DepthMarketData QuotData;
			bool bQuot=m_RefQuots.GetQuotInfo(*it_Instruments,QuotData);

            map<string,set<long>>::iterator it_Index2=m_PositionData.m_allIndex2.find(*it_Instruments);
            if(it_Index2!=m_PositionData.m_allIndex2.end()&&!it_Index2->second.empty())
            {
                for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
                {
                    PlatformStru_Position* pPosition;
                    if(m_PositionData.GetValue2_Internal(pPosition,*itFTID))
                    {
                        //�������ʣ���λ
                        pPosition->YdPositionRemain=pPosition->Position-pPosition->TodayPosition;

                        //�����б�����¼�м�����ƽ�ֹҵ�����ƽ�ֹҵ�����ƽ��ҵ�����ƽ��ҵ�������ƽ��������ƽ��������ƽ����
                        int CloseVolume,CloseTodayVolume,CloseYesterdayVolume;
                        char CloseOrderDirection = pPosition->PosiDirection==THOST_FTDC_PD_Long?THOST_FTDC_D_Sell:THOST_FTDC_D_Buy;
                        if(m_RefOrders.GetCloseOrderVolume(string(pPosition->Account),*it_Instruments,CloseOrderDirection,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,pPosition->HedgeFlag)==0)
                            CalcuCloseValueInPositionByAllOrders_Internal(*pPosition,CloseVolume,CloseTodayVolume,CloseYesterdayVolume);

                        //����ֲ־��ۺͿ��־���
                        pPosition->AveragePositionPrice = pPosition->Position>0&&InstrumentInfo.VolumeMultiple ? pPosition->PositionCost/pPosition->Position/InstrumentInfo.VolumeMultiple : 0;
                        pPosition->AverageOpenPrice = pPosition->Position>0&&InstrumentInfo.VolumeMultiple ? pPosition->OpenCost/pPosition->Position/InstrumentInfo.VolumeMultiple : 0;

                        pPosition->UpdateSeq=m_PositionData.NewUpdateSeq(pPosition->FTID);

					    //��̬���³ֲֵĳֲ�ӯ��������ӯ���Ͷ�����ӯ��
					    if(bQuot) UpdatePositionByQuot_Internal(*pPosition,InstrumentInfo,QuotData,true);
                    }
                }
			}
		}
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Positions::QryPositionReach-end");
}






//��ѯ�ֲ�ʱ�ϲ�ͬһ��Լͬһ�������ʷ�ֲֺͽ��ճֲּ�¼
void CPlatformDataMgr_Positions::MergePositions_RspQryPosition( PlatformStru_Position& firstPositionInfo, const PlatformStru_Position& secondPositionInfo )
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



//�ڳֲ������У�����ƽ�����Ϳ�ƽ����, (�������йҵ�)����ȫ��
void CPlatformDataMgr_Positions::CalcuCloseValueInPositionByAllOrders_Internal(PlatformStru_Position& PositionData,
                                                                               int CloseVolume,
                                                                               int CloseTodayVolume,
                                                                               int CloseYesterdayVolume)
{
    PositionData.CloseOrderVolume = CloseVolume;
    PositionData.CloseTodayOrderVolume = CloseTodayVolume;
    PositionData.CloseYesterdayOrderVolume = CloseYesterdayVolume;
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

//��̬���³ֲּ�¼�ĳֲ�ӯ��(����)������ӯ��(���)��������ӯ��������ۡ������ۡ��ּ�
//����ӯ������ʹ�õļ۸�˳��Ϊ������� > ���¼� > ������
void CPlatformDataMgr_Positions::UpdatePositionByQuot_Internal(PlatformStru_Position& PositionInfo,
                                                               const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                               const PlatformStru_DepthMarketData& QuotData,
                                                               bool bForceCalculate) 
{
    string strInstrument(QuotData.InstrumentID);
    if(strInstrument.empty())
        return;

    if(strInstrument!=string(PositionInfo.InstrumentID))
        return;

    //�����õļ۸�
    double CurPrice;
    if(m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible&&!util::isInvalidValue(QuotData.SettlementPrice)) CurPrice=QuotData.SettlementPrice;
    else if(QuotData.Volume>0) CurPrice=QuotData.LastPrice;
    else CurPrice=QuotData.PreSettlementPrice;

    //��ǿ�Ƽ���ʱ����۸�δ���£������м���
    if(!bForceCalculate&&CurPrice==PositionInfo.CurPrice)
        return;


    //�õ�ǰ�۸�ֱ�Ӽ���ֲ�ӯ��������ӯ����������ӯ��
    double PositionProfit=util::GetDoubleInvalidValue();
    double PositionProfitByTrade=util::GetDoubleInvalidValue();
    double TotalPositionProfitByDate=util::GetDoubleInvalidValue();
    if(!util::isInvalidValue(CurPrice))
    {
        PositionProfit=0;
        PositionProfitByTrade=0;
        TotalPositionProfitByDate=0;

        double CurCost=CurPrice*PositionInfo.Position*InstrumentInfo.VolumeMultiple;
        if(PositionInfo.PosiDirection==THOST_FTDC_PD_Long)
        {
            PositionProfit=CurCost-PositionInfo.PositionCost;
            PositionProfitByTrade=CurCost-PositionInfo.OpenCost;
        }
        else if(PositionInfo.PosiDirection==THOST_FTDC_PD_Short)
        {
            PositionProfit=PositionInfo.PositionCost-CurCost;
            PositionProfitByTrade=PositionInfo.OpenCost-CurCost;
        }
        TotalPositionProfitByDate=PositionProfit+PositionInfo.CloseProfitByDate;
    }

    bool bUpdate=false;
    if(PositionInfo.PreSettlementPrice!=QuotData.PreSettlementPrice){ PositionInfo.PreSettlementPrice=QuotData.PreSettlementPrice; bUpdate=true; }
    if(PositionInfo.SettlementPrice!=QuotData.SettlementPrice){ PositionInfo.SettlementPrice=QuotData.SettlementPrice; bUpdate=true; }
    if(PositionInfo.PositionProfit!=PositionProfit){ PositionInfo.PositionProfit=PositionProfit; bUpdate=true; }
    if(PositionInfo.PositionProfitByTrade!=PositionProfitByTrade){ PositionInfo.PositionProfitByTrade=PositionProfitByTrade; bUpdate=true; }
    if(PositionInfo.TotalPositionProfitByDate!=TotalPositionProfitByDate){ PositionInfo.TotalPositionProfitByDate=TotalPositionProfitByDate; bUpdate=true; }
    if(PositionInfo.CurPrice!=CurPrice) { PositionInfo.CurPrice=CurPrice;  bUpdate=true;}
    if(bUpdate)
        PositionInfo.UpdateSeq=m_PositionData.NewUpdateSeq(PositionInfo.FTID);
}




//�����ر�ʱ�����µ��Ⱥ�Լ�ֲ���صĹҵ��Ķ��������ѺͶ��ᱣ֤��
void CPlatformDataMgr_Positions::UpdatePositionFreezeFields_RtnOrder(const PlatformStru_OrderInfo& Order,
                                                                     const PlatformStru_OrderInfo& OldOrderInfo,
                                                                     const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    string strInstrumentID(Order.InstrumentID);

	Lock_CPlatformDataMgr;

    //����һ�����ֱ����仯���ݸ��³ֲֵĶ��������ѡ����ᱣ֤��Ͷ�����
    RefreshPositionFreezeByOneOpenOrder_Internal(Order,OldOrderInfo,InstrumentInfo);

    //����һ��ƽ�ֱ����仯���ݸ��³ֲֵĹҵ�������ƽ��
    //�ֲ��ǵ��Ⱥ�Լ�ֲ֣���������Ϻ�Լ���Ⱥ�Լ
    RefreshPositionCloseOrderVolumeByOneOrder_Internal(Order,OldOrderInfo,InstrumentInfo);

    Unlock_CPlatformDataMgr;
}


//����һ�����ֱ����仯���ݸ��³ֲֵĶ��������ѡ����ᱣ֤��Ͷ�����
//�ֲ��ǵ��Ⱥ�Լ�������ǵ��Ⱥ�Լ����Ϻ�Լ
void CPlatformDataMgr_Positions::RefreshPositionFreezeByOneOpenOrder_Internal(const PlatformStru_OrderInfo& Order,
                                                                              const PlatformStru_OrderInfo& OldOrderInfo,
                                                                              const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    string Leg1InstrumentID,Leg2InstrumentID;
    PlatformStru_Position *pPosition;

    if(Order.CombOffsetFlag[0]!=THOST_FTDC_OF_Open) return;

    if(InstrumentInfo.ProductClass!=THOST_FTDC_PC_Combination)
    {
        //���Ⱥ�Լ����
        PositionKey key(Order);
        if(m_PositionData.GetValue_Internal(pPosition,key))
        {
            bool bUpdate=false;
            if(!util::isInvalidValue(pPosition->FrozenMargin))
            {
                if(!util::isInvalidValue(Order.freezeMargin))
                {
                    pPosition->FrozenMargin+=Order.freezeMargin;
                    if(!OldOrderInfo.IsInvalidRecord())
                    {
                        if(!util::isInvalidValue(OldOrderInfo.freezeMargin)) pPosition->FrozenMargin-=OldOrderInfo.freezeMargin;
                        else pPosition->FrozenMargin=util::GetDoubleInvalidValue();
                    }
                }
                else pPosition->FrozenMargin=util::GetDoubleInvalidValue();
                bUpdate=true;
            }
            if(!util::isInvalidValue(pPosition->FrozenCommission))
            {
                if(!util::isInvalidValue(Order.troubleMoney))
                {
                    pPosition->FrozenCommission+=Order.troubleMoney;
                    if(!OldOrderInfo.IsInvalidRecord())
                    {
                        if(!util::isInvalidValue(OldOrderInfo.troubleMoney)) pPosition->FrozenCommission-=OldOrderInfo.troubleMoney;
                        else pPosition->FrozenCommission=util::GetDoubleInvalidValue();
                    }
                }
                else pPosition->FrozenCommission=util::GetDoubleInvalidValue();
                bUpdate=true;
            }
            if(bUpdate)
            {
                if(!util::isInvalidValue(pPosition->FrozenMargin)&&!util::isInvalidValue(pPosition->FrozenCommission)) pPosition->FrozenCash=pPosition->FrozenMargin+pPosition->FrozenCommission;
                else pPosition->FrozenCash=util::GetDoubleInvalidValue();
                pPosition->UpdateSeq=m_PositionData.NewUpdateSeq(pPosition->FTID);
            }
        }
    }
    else if(m_RefInstruments.GetLegInstrumentID(string(Order.InstrumentID),Leg1InstrumentID,Leg2InstrumentID))
    {
        //��Ϻ�Լ����-��һ�ȳֲ�
        char LegPosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
        PositionKey key1(Order.Account, Leg1InstrumentID.c_str(), LegPosiDirection, Order.CombHedgeFlag[0]);
        if(m_PositionData.GetValue_Internal(pPosition,key1))
        {
            bool bUpdate=false;
            if(!util::isInvalidValue(pPosition->FrozenMargin))
            {
                if(!util::isInvalidValue(Order.Leg1FreezeMargin))
                {
                    pPosition->FrozenMargin+=Order.Leg1FreezeMargin;
                    if(!OldOrderInfo.IsInvalidRecord())
                    {
                        if(!util::isInvalidValue(OldOrderInfo.Leg1FreezeMargin)) pPosition->FrozenMargin-=OldOrderInfo.Leg1FreezeMargin;
                        else pPosition->FrozenMargin=util::GetDoubleInvalidValue();
                    }
                }
                else pPosition->FrozenMargin=util::GetDoubleInvalidValue();
                bUpdate=true;
            }
            if(!util::isInvalidValue(pPosition->FrozenCommission))
            {
                if(!util::isInvalidValue(Order.Leg1TroubleMoney))
                {
                    pPosition->FrozenCommission+=Order.Leg1TroubleMoney;
                    if(!OldOrderInfo.IsInvalidRecord())
                    {
                        if(!util::isInvalidValue(OldOrderInfo.Leg1TroubleMoney)) pPosition->FrozenCommission-=OldOrderInfo.Leg1TroubleMoney;
                        else pPosition->FrozenCommission=util::GetDoubleInvalidValue();
                    }
                }
                else pPosition->FrozenCommission=util::GetDoubleInvalidValue();
                bUpdate=true;
            }
            if(bUpdate)
            {
                if(!util::isInvalidValue(pPosition->FrozenMargin)&&!util::isInvalidValue(pPosition->FrozenCommission)) pPosition->FrozenCash=pPosition->FrozenMargin+pPosition->FrozenCommission;
                else pPosition->FrozenCash=util::GetDoubleInvalidValue();
                pPosition->UpdateSeq=m_PositionData.NewUpdateSeq(pPosition->FTID);
            }
        }

        //��Ϻ�Լ����-�ڶ��ȳֲ�
        LegPosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Short : THOST_FTDC_PD_Long;
        PositionKey key2(Order.Account, Leg2InstrumentID.c_str(), LegPosiDirection, Order.CombHedgeFlag[0]);
        if(m_PositionData.GetValue_Internal(pPosition,key2))
        {
            bool bUpdate=false;
            if(!util::isInvalidValue(pPosition->FrozenMargin))
            {
                if(!util::isInvalidValue(Order.Leg2FreezeMargin))
                {
                    pPosition->FrozenMargin+=Order.Leg2FreezeMargin;
                    if(!OldOrderInfo.IsInvalidRecord())
                    {
                        if(!util::isInvalidValue(OldOrderInfo.Leg2FreezeMargin)) pPosition->FrozenMargin-=OldOrderInfo.Leg2FreezeMargin;
                        else pPosition->FrozenMargin=util::GetDoubleInvalidValue();
                    }
                }
                else pPosition->FrozenMargin=util::GetDoubleInvalidValue();
                bUpdate=true;
            }
            if(!util::isInvalidValue(pPosition->FrozenCommission))
            {
                if(!util::isInvalidValue(Order.Leg2TroubleMoney))
                {
                    pPosition->FrozenCommission+=Order.Leg2TroubleMoney;
                    if(!OldOrderInfo.IsInvalidRecord())
                    {
                        if(!util::isInvalidValue(OldOrderInfo.Leg2TroubleMoney)) pPosition->FrozenCommission-=OldOrderInfo.Leg2TroubleMoney;
                        else pPosition->FrozenCommission=util::GetDoubleInvalidValue();
                    }
                }
                else pPosition->FrozenCommission=util::GetDoubleInvalidValue();
                bUpdate=true;
            }
            if(bUpdate)
            {
                if(!util::isInvalidValue(pPosition->FrozenMargin)&&!util::isInvalidValue(pPosition->FrozenCommission)) pPosition->FrozenCash=pPosition->FrozenMargin+pPosition->FrozenCommission;
                else pPosition->FrozenCash=util::GetDoubleInvalidValue();
                pPosition->UpdateSeq=m_PositionData.NewUpdateSeq(pPosition->FTID);
            }
        }

    }
}

//����һ��ƽ�ֱ����仯���ݸ��³ֲֵĹҵ�������ƽ��
//�ֲ��ǵ��Ⱥ�Լ�ֲ֣���������Ϻ�Լ���Ⱥ�Լ
void CPlatformDataMgr_Positions::RefreshPositionCloseOrderVolumeByOneOrder_Internal(const PlatformStru_OrderInfo& Order,
                                                                                    const PlatformStru_OrderInfo& OldOrderInfo,
                                                                                    const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    string Leg1InstrumentID,Leg2InstrumentID;
    PositionKey key;
    PlatformStru_Position *pPosition;
    char ClosePosiDirection;

    if(Order.CombOffsetFlag[0]!=THOST_FTDC_OF_Close&&
        Order.CombOffsetFlag[0]!=THOST_FTDC_OF_ForceClose&&
		Order.CombOffsetFlag[0]!=THOST_FTDC_OF_CloseToday&&
		Order.CombOffsetFlag[0]!=THOST_FTDC_OF_CloseYesterday&&
		Order.CombOffsetFlag[0]!=THOST_FTDC_OF_LocalForceClose) return;

    if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Combination&&
        !m_RefInstruments.GetLegInstrumentID(string(Order.InstrumentID),Leg1InstrumentID,Leg2InstrumentID))
        return;

    for(int i=0;i<2;i++)
    {
        if(i==0)
        {
		    ClosePosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Short : THOST_FTDC_PD_Long;
            if(InstrumentInfo.ProductClass!=THOST_FTDC_PC_Combination) key=PositionKey(Order.Account, Order.InstrumentID, ClosePosiDirection, Order.CombHedgeFlag[0]);
            else key=PositionKey(Order.Account, Leg1InstrumentID.c_str(), ClosePosiDirection, Order.CombHedgeFlag[0]);
        }
        else
        {
            if(InstrumentInfo.ProductClass!=THOST_FTDC_PC_Combination) break;
		    ClosePosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
            key=PositionKey(Order.Account, Leg2InstrumentID.c_str(), ClosePosiDirection, Order.CombHedgeFlag[0]);
        }
        if(m_PositionData.GetValue_Internal(pPosition,key))
        {
            //����ƽ�ֹҵ�����ƽ�ֹҵ�����ƽ��ҵ�����ƽ��ҵ�������ƽ��������ƽ��������ƽ����
            int CloseVolume=0,CloseTodayVolume=0,CloseYesterdayVolume=0;
            int OldCloseVolume=0,OldCloseTodayVolume=0,OldCloseYesterdayVolume=0;

            if(CPlatformDataMgr_Orders::IsOrderWorking(Order))
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
                CPlatformDataMgr_Orders::IsOrderWorking(OldOrderInfo))
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

            //�ڳֲ������У�����ƽ�����Ϳ�ƽ����, (����һ���ҵ�)������������
            //Deltaxxx��������һ����˵�Ǹ�ֵ
            CalcuCloseValueInPositionByOneOrder_Internal(*pPosition,CloseVolume,CloseTodayVolume,CloseYesterdayVolume);
        }
    }
}

//�ڳֲ������У�����ƽ�����Ϳ�ƽ����, (����һ���ҵ�)������������
//Deltaxxx��������һ����˵�Ǹ�ֵ
//�˺�����Ե��Ⱥ�Լ�ֲ�
void CPlatformDataMgr_Positions::CalcuCloseValueInPositionByOneOrder_Internal(PlatformStru_Position& PositionData,
                                                                              int DeltaCloseVolume,int DeltaCloseTodayVolume,int DeltaCloseYesterdayVolume)
{
    PositionData.CloseOrderVolume += DeltaCloseVolume;
    PositionData.CloseTodayOrderVolume += DeltaCloseTodayVolume;
    PositionData.CloseYesterdayOrderVolume += DeltaCloseYesterdayVolume;

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
    PositionData.UpdateSeq=m_PositionData.NewUpdateSeq(PositionData.FTID);
}



//�ӳɽ���¼���³ֲ֡��˺����ӳֲ���ϸ�����´���������ֲ֡������ڳֲ���ϸ�������ڳֲ���ϸ��������ٸ��³ֲ�
//�ֲ���ϸ�ͳֲֶ��ǵ��Ⱥ�Լ
void CPlatformDataMgr_Positions::UpdatePosition_RtnTrade(const PlatformStru_TradeInfo& TradeInfo,
                                                         const map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                                         const PlatformStru_InstrumentInfo& InstrumentInfo)
{
	LOG_INFO("CPlatformDataMgr_Positions::UpdatePosition_RtnTrade(�ɽ��ر����³ֲ�):InstrumentID=%s",TradeInfo.InstrumentID);

	string strInstrument(TradeInfo.InstrumentID);
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

    //�����ֲ�keyֵ
    PositionKey CurPositionKey( TradeInfo.Account,
		                        strInstrument.c_str(), 
		                        PosiDirection,
		                        TradeInfo.HedgeFlag);

	PlatformStru_Position* pPositionData;


    Lock_CPlatformDataMgr;

    //�ȿ���û��ԭ�гֲ�
    bool bExist=m_PositionData.GetValue_Internal(pPositionData,CurPositionKey);
    if(!bExist)
    {
        PlatformStru_Position NewPositionData;

        strncpy(NewPositionData.Account,TradeInfo.Account,sizeof(NewPositionData.Account)-1);
	    strncpy(NewPositionData.InstrumentID,TradeInfo.InstrumentID,sizeof(NewPositionData.InstrumentID)-1);
	    strncpy(NewPositionData.BrokerID,TradeInfo.BrokerID,sizeof(NewPositionData.BrokerID)-1);
	    strncpy(NewPositionData.InvestorID,TradeInfo.InvestorID,sizeof(NewPositionData.InvestorID)-1);
	    NewPositionData.PosiDirection=PosiDirection;
	    NewPositionData.HedgeFlag=TradeInfo.HedgeFlag;
	    NewPositionData.PositionDate=THOST_FTDC_PSD_Today;
        strncpy(NewPositionData.TradingDay,m_RefInstruments.GetTradingDay().c_str(),sizeof(NewPositionData.TradingDay)-1);
	    NewPositionData.SettlementID=TradeInfo.SettlementID;

	    NewPositionData.Position=0;
	    NewPositionData.TodayPosition=0;
	    NewPositionData.YdPosition=0;
        NewPositionData.YdPositionRemain=0;

	    NewPositionData.OpenVolume=0;
	    NewPositionData.CloseVolume=0;
	    NewPositionData.OpenAmount=0;
	    NewPositionData.CloseAmount=0;
	    NewPositionData.PositionCost=0;
	    NewPositionData.OpenCost=0;

	    NewPositionData.LongFrozen=0;
	    NewPositionData.ShortFrozen=0;
	    NewPositionData.LongFrozenAmount=0;
	    NewPositionData.ShortFrozenAmount=0;
	    NewPositionData.FrozenMargin=0;
	    NewPositionData.FrozenCommission=0;
	    NewPositionData.FrozenCash=0;

	    NewPositionData.Commission=0;
	    NewPositionData.PreMargin=0;
	    NewPositionData.UseMargin=0;
	    NewPositionData.ExchangeMargin = 0;
	    NewPositionData.MarginRateByMoney = util::GetDoubleInvalidValue();
	    NewPositionData.MarginRateByVolume = util::GetDoubleInvalidValue();

	    NewPositionData.CashIn=0;

	    NewPositionData.PositionProfit=0;
	    NewPositionData.CloseProfit=0;
	    NewPositionData.CloseProfitByDate=0;
	    NewPositionData.CloseProfitByTrade=0;

	    NewPositionData.PreSettlementPrice=util::GetDoubleInvalidValue();
	    NewPositionData.SettlementPrice=util::GetDoubleInvalidValue();

	    NewPositionData.CombPosition=0;
	    NewPositionData.CombLongFrozen=0;
	    NewPositionData.CombShortFrozen=0;

        NewPositionData.PositionProfitByTrade=0;
	    NewPositionData.TotalPositionProfitByDate=0;

        NewPositionData.AveragePositionPrice=0;
	    NewPositionData.AverageOpenPrice=0;

        NewPositionData.TodayCloseYdVolume=0;

        NewPositionData.CurPrice=util::GetDoubleInvalidValue(); //�ȴ����������

        //���ö�Ӧ��PriceTick
        NewPositionData.PriceTick=InstrumentInfo.PriceTick;
        strncpy(NewPositionData.Currency,InstrumentInfo.Currency,sizeof(NewPositionData.Currency)-1);
        strncpy(NewPositionData.ExchangeID,InstrumentInfo.ExchangeID,sizeof(NewPositionData.ExchangeID)-1);

        m_PositionData.NewReach_Internal(NewPositionData);

        if(!m_PositionData.GetValue_Internal(pPositionData,CurPositionKey))
        {
            Unlock_CPlatformDataMgr;
            return;
        }
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

        //��ϳֲ���ϸ����ɢ����Գֲֵ�Ӱ�죺
        //1. ������ֱ���ɢ�����������һ�ȱ���ɢ
        //2. �ɽ���Ӧ����һ�ȳֲ���ϸֻ����ֲֳ���ϸ�������ٵ����
        //3. ��һ�ȿ��ܳ����½���ͨ�ֲ���ϸ������Ӧ��ͨ�ֲ���ϸ�������ӵ����������Ӱ����ɱ���ӯ������˲������⴦��

        //����ֲֳɱ������ֳɱ���ƽ��ӯ��(���С���ʣ�����Ҫ������Чֵ)�����ݳֲ���ϸ�ı仯�����㣩
        //int i,imax;
        int ClosedVolume;
        //imax=ModifiedPositionDetails.size();
        PlatformStru_PositionDetail detail;
        for(map<PositionDetailKey,PlatformStru_PositionDetail>::const_iterator it=ModifiedPositionDetails.begin();it!=ModifiedPositionDetails.end();it++)
        {
            if(!m_RefPositionDetails.GetPositionDetail(it->first,detail)) continue;
            if(CurPositionKey.IsSamePositionKey(detail))
            {
                ClosedVolume=it->second.Volume-detail.Volume;
                if(ClosedVolume>0)
                {
                    if(detail.bToday)
                        pPositionData->PositionCost -= ClosedVolume*detail.OpenPrice*InstrumentInfo.VolumeMultiple;
                    else 
                        pPositionData->PositionCost -= ClosedVolume*detail.LastSettlementPrice*InstrumentInfo.VolumeMultiple;
                    pPositionData->OpenCost -= ClosedVolume*detail.OpenPrice*InstrumentInfo.VolumeMultiple;
                    pPositionData->CloseProfit += detail.CloseProfitByDate - it->second.CloseProfitByDate;
                    pPositionData->CloseProfitByDate += detail.CloseProfitByDate - it->second.CloseProfitByDate;
                    pPositionData->CloseProfitByTrade += detail.CloseProfitByTrade - it->second.CloseProfitByTrade;
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

    if(!bExist)
    {
	    //������½��ֲ֣����³ֲֵı��������
	    UpdatePositionFreezeFieldsByAllOrders_Internal(*pPositionData);
    }
    else
    {
        //����Ҫ���¿�ƽ������ֵ
        CalcuCloseValueInPositionByOneOrder_Internal(*pPositionData,0,0,0);
    }

    //���³ֲּ�¼�ĳֲ�ӯ��(����)������ӯ��(���)�Ͷ�����ӯ��������ۺ������ۣ���������
    PlatformStru_DepthMarketData QuotInfo;
    if(m_RefQuots.GetQuotInfo(strInstrument,QuotInfo))
        UpdatePositionByQuot_Internal(*pPositionData,InstrumentInfo,QuotInfo,true); 
    
    //���³ֲּ�¼��ռ�ñ�֤��
    //����Ҫ������ϵ�����ɢ���������һ��(ָ����ɢ������)������ͨ�ֲ���ϸ����Ӧ�ĳֲֵı�֤�������
    UpdatePositionByMarginRate_RtnTrade_Internal(*pPositionData,ModifiedPositionDetails);

    //���³ֲּ�¼��������
    if(pPositionData->Commission!=util::GetDoubleInvalidValue()&&
        TradeInfo.TradeCommission!=util::GetDoubleInvalidValue())
        pPositionData->Commission += TradeInfo.TradeCommission;
    else if(pPositionData->Commission!=util::GetDoubleInvalidValue())
        pPositionData->Commission=util::GetDoubleInvalidValue();

    pPositionData->UpdateSeq=m_PositionData.NewUpdateSeq(pPositionData->FTID);

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Positions::UpdatePosition_RtnTrade-end");
    return;
}

//���³ֲ���صĹҵ��Ķ��������ѺͶ��ᱣ֤�𡣳ɽ��½��ֲ�ʱ���õ�����ʱ����
void CPlatformDataMgr_Positions::UpdatePositionFreezeFieldsByAllOrders_Internal(PlatformStru_Position& PositionData)
{
	string strInstrumentID(PositionData.InstrumentID);
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


	string strAccount(PositionData.Account);

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
    PositionData.CombPosition=0;
	PositionData.CombLongFrozen=0;
	PositionData.CombShortFrozen=0;


    //����ӱ����м��㶳����
	PositionData.FrozenMargin=0;
	PositionData.FrozenCommission=0;
	PositionData.FrozenCash=0;
    m_RefOrders.GetFreezeMargin_Commission2(strAccount,strInstrumentID,Direction,PositionData.HedgeFlag,
        PositionData.FrozenMargin,PositionData.FrozenCommission);

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
    int CloseVolume=0,CloseTodayVolume=0,CloseYesterdayVolume=0;
    if(m_RefOrders.GetCloseOrderVolume(strAccount,strInstrumentID,CloseDirection,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,PositionData.HedgeFlag)==0)
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
        PositionData.UpdateSeq=m_PositionData.NewUpdateSeq(PositionData.FTID);
    }
}


//��̬���³ֲּ�¼�ı�֤�𡢱�֤���ʣ��ɽ��޸ĳֲ�ʱ����
//����Ҫ������ϵ�����ɢ���������һ��(ָ����ɢ������)������ͨ�ֲ���ϸ����Ӧ�ĳֲֵı�֤�������
void CPlatformDataMgr_Positions::UpdatePositionByMarginRate_RtnTrade_Internal(PlatformStru_Position& PositionInfo,
                                                                              const map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails) 
{
    LOG_INFO("CPlatformDataMgr_Positions::UpdatePositionByMarginRate_RtnTrade_Internal ���³ֲ�ռ�ñ�֤�� InstrumentID=%s",PositionInfo.InstrumentID);

    char	Direction;
    if(PositionInfo.PosiDirection==THOST_FTDC_PD_Long) Direction=THOST_FTDC_D_Buy;
    else if(PositionInfo.PosiDirection==THOST_FTDC_PD_Short) Direction=THOST_FTDC_D_Sell;
    else return;

    double OldUseMargin=PositionInfo.UseMargin;
    double OldExchangeMargin=PositionInfo.ExchangeMargin;

    PositionKey CurPositionKey(PositionInfo);
    //���㱣֤�𣨸��ݳֲ���ϸ�ı仯�����㣩
    PlatformStru_PositionDetail detail;
    for(map<PositionDetailKey,PlatformStru_PositionDetail>::const_iterator it=ModifiedPositionDetails.begin();it!=ModifiedPositionDetails.end();it++)
    {
        if(!m_RefPositionDetails.GetPositionDetail(it->first,detail)) continue;

        //���ﲻ������ϳֲ���ϸ�еı�֤��
        if(detail.TradeType==THOST_FTDC_TRDT_CombinationDerived)
            continue;

        if(CurPositionKey.IsSamePositionKey(detail))
        {
			//�����Լ���һ��
            if(!util::isInvalidValue(PositionInfo.UseMargin))
		    {
                if(util::isInvalidValue(detail.Margin)||!it->second.IsInvalidRecord()&&util::isInvalidValue(it->second.Margin))
                    PositionInfo.UseMargin=util::GetDoubleInvalidValue();
                else PositionInfo.UseMargin += detail.Margin - (it->second.IsInvalidRecord() ? 0 : it->second.Margin);
		    }
            if(!util::isInvalidValue(PositionInfo.ExchangeMargin))
		    {
                if(util::isInvalidValue(detail.ExchMargin)||!it->second.IsInvalidRecord()&&util::isInvalidValue(it->second.ExchMargin))
                    PositionInfo.ExchangeMargin=util::GetDoubleInvalidValue();
                else PositionInfo.ExchangeMargin += detail.ExchMargin - (it->second.IsInvalidRecord() ? 0 : it->second.ExchMargin);
            }
        }
        else
        {
			//���Ǵ�ɢ�����һ�ȣ���Ϊ��ͨ�ֲ֣����ҵ���Ӧ�ĳֲ֣�Ȼ����±�֤��
		    PositionKey OppoPositionKey(detail.Account, 
			    detail.InstrumentID,
                detail.Direction==THOST_FTDC_D_Buy?THOST_FTDC_PD_Long:THOST_FTDC_PD_Short,
			    detail.HedgeFlag);
            PlatformStru_Position *pOppoPosition;
		    if(m_PositionData.GetValue_Internal(pOppoPosition,OppoPositionKey))
            {
                if(!util::isInvalidValue(pOppoPosition->UseMargin))
                {
                    if(util::isInvalidValue(detail.Margin)||!it->second.IsInvalidRecord()&&util::isInvalidValue(it->second.Margin))
                        pOppoPosition->UseMargin=util::GetDoubleInvalidValue();
                    else pOppoPosition->UseMargin += detail.Margin - (it->second.IsInvalidRecord() ? 0 : it->second.Margin);
                    pOppoPosition->UpdateSeq = m_PositionData.NewUpdateSeq(pOppoPosition->FTID);
                }
                if(!util::isInvalidValue(pOppoPosition->ExchangeMargin))
                {
                    if(util::isInvalidValue(detail.ExchMargin)||!it->second.IsInvalidRecord()&&util::isInvalidValue(it->second.ExchMargin))
                        pOppoPosition->ExchangeMargin=util::GetDoubleInvalidValue();
                    else pOppoPosition->ExchangeMargin += detail.ExchMargin - (it->second.IsInvalidRecord() ? 0 : it->second.ExchMargin);
                    pOppoPosition->UpdateSeq = m_PositionData.NewUpdateSeq(pOppoPosition->FTID);
                }
            }
        }
    }

    if(PositionInfo.UseMargin!=OldUseMargin||PositionInfo.ExchangeMargin!=OldExchangeMargin)
        PositionInfo.UpdateSeq=m_PositionData.NewUpdateSeq(PositionInfo.FTID);

    LOG_INFO("CPlatformDataMgr_Positions::UpdatePositionByMarginRate_RtnTrade_Internal ���³ֲ�ռ�ñ�֤����ɣ� InstrumentID=%s UseMargin=%g ExchangeMargin=%g UpdateSeq=%u",PositionInfo.InstrumentID,PositionInfo.UseMargin,PositionInfo.ExchangeMargin,PositionInfo.UpdateSeq);
}


//��̬���³ֲּ�¼�ĳֲ�ӯ��(����)������ӯ��(���)��������ӯ��������ۡ������ۣ��õ�������ʱ��һ��
void CPlatformDataMgr_Positions::UpdatePosition_RtnQuot(const PlatformStru_DepthMarketData& QuotData,
                                                        const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                        bool bForceCalculate) 
{
    Lock_CPlatformDataMgr;

    map<string,set<long>>::iterator it_Index2=m_PositionData.m_allIndex2.find(string(QuotData.InstrumentID));
    if(it_Index2!=m_PositionData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            map<long,PlatformStru_Position*>::iterator it_Info2=m_PositionData.m_all2.find(*itFTID);
            if(it_Info2!=m_PositionData.m_all2.end()&&it_Info2->second!=NULL)
            {
                UpdatePositionByQuot_Internal(*(it_Info2->second),InstrumentInfo,QuotData,bForceCalculate);
            }
        }
    }

    Unlock_CPlatformDataMgr;

}

//����ʱ���³ֲ֡����ݽ���ۣ����³ֲֵ�ӯ������֤��
void CPlatformDataMgr_Positions::UpdatePosition_OnSettlement(void)
{
    Lock_CPlatformDataMgr;

    PlatformStru_DepthMarketData QuotData;
    PlatformStru_InstrumentInfo InstrumentInfo;
    PlatformStru_InstrumentMarginRate MarginRate;
    for(std::set<long>::iterator it=m_PositionData.m_allFTID.begin();it!=m_PositionData.m_allFTID.end();it++)
    {
        map<long,PlatformStru_Position*>::iterator it_Info2=m_PositionData.m_all2.find(*it);
        if(it_Info2!=m_PositionData.m_all2.end()&&it_Info2->second!=NULL)
        {
            if(m_RefInstruments.GetInstrumentInfo(string(it_Info2->second->InstrumentID),InstrumentInfo)&&
                m_RefQuots.GetQuotInfo(string(it_Info2->second->InstrumentID),QuotData)&&
                m_RefRates.GetMarginRate(string(it_Info2->second->InstrumentID),MarginRate)==0)
            {
                //����ӯ��������۵�
                UpdatePositionByQuot_Internal(*(it_Info2->second),InstrumentInfo,QuotData,true);
                //��ʱ�ֲ���ϸ�еı�֤���Ѿ����ˣ�����ֱֲ�֤��
                UpdatePositionByMarginRate_RspQryRate_OnSettlement_Internal(MarginRate,*(it_Info2->second),true);
            }
        }
    }

    Unlock_CPlatformDataMgr;
}

//��̬���³ֲּ�¼�ı�֤�𡢱�֤���ʣ���ѯ��֤������Ӧʱ���á������Ƿ�����˸���
bool CPlatformDataMgr_Positions::UpdatePositionByMarginRate_RspQryRate(const PlatformStru_InstrumentMarginRate& MarginRate) 
{
    bool brlt=false;
    string strInstrument(MarginRate.InstrumentID);

    Lock_CPlatformDataMgr;

    map<string,set<long>>::iterator it_Index2=m_PositionData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_PositionData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            map<long,PlatformStru_Position*>::iterator it_Info2=m_PositionData.m_all2.find(*itFTID);
            //�ֲֵı�֤��һ��Ϊ��Чֵ�Ž��и���
            if(it_Info2!=m_PositionData.m_all2.end()&&it_Info2->second!=NULL&&
                util::isInvalidValue(it_Info2->second->UseMargin))
            {
                UpdatePositionByMarginRate_RspQryRate_OnSettlement_Internal(MarginRate,*(it_Info2->second),false);
                if(!brlt) brlt=true;
            }
        }
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

//��̬���³ֲּ�¼�ı�֤�𡢱�֤���ʣ���ѯ��֤������Ӧ�ͽ���ʱ����
void CPlatformDataMgr_Positions::UpdatePositionByMarginRate_RspQryRate_OnSettlement_Internal(const PlatformStru_InstrumentMarginRate& MarginRate,
                                                                                             PlatformStru_Position& PositionInfo,
                                                                                             bool bForce)
{
    PlatformStru_InstrumentInfo InstrumentInfo;

    //�Ѿ�����ˣ�������
    if(!bForce&&PositionInfo.UseMargin!=util::GetDoubleInvalidValue())
    {
        LOG_INFO("CPlatformDataMgr_Positions::UpdatePositionByMarginRate_RspQryRate_OnSettlement_Internal �������㣬 InstrumentID=%s��bForce=%d, UseMargin=%g, ExchangeMargin=%g",PositionInfo.InstrumentID,bForce,PositionInfo.UseMargin,PositionInfo.ExchangeMargin);
        return;
    }

    char	Direction;
    if(PositionInfo.PosiDirection==THOST_FTDC_PD_Long) Direction=THOST_FTDC_D_Buy;
    else if(PositionInfo.PosiDirection==THOST_FTDC_PD_Short) Direction=THOST_FTDC_D_Sell;
    else return;

    string strInstrument(PositionInfo.InstrumentID);

    //��������Ӧ�ĺ�Լ��Ϣ
    if(!m_RefInstruments.GetInstrumentInfo(strInstrument,InstrumentInfo))
        return;

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

    //����ָ����Լ�ı�֤��
    double UseMargin=0,ExchangeMargin=0;
    m_RefPositionDetails.GetMarginByInstrument(strInstrument,string(PositionInfo.Account),Direction,PositionInfo.HedgeFlag,UseMargin,ExchangeMargin);

    bool bUpdate=false;
    if(PositionInfo.UseMargin!=UseMargin){ PositionInfo.UseMargin=UseMargin;  bUpdate=true; }
    if(PositionInfo.ExchangeMargin!=ExchangeMargin){ PositionInfo.ExchangeMargin=ExchangeMargin;  bUpdate=true; }
    if(PositionInfo.MarginRateByMoney!=MarginRateByMoney){ PositionInfo.MarginRateByMoney=MarginRateByMoney;  bUpdate=true; }
    if(PositionInfo.MarginRateByVolume!=MarginRateByVolume){ PositionInfo.MarginRateByVolume=MarginRateByVolume;  bUpdate=true; }
    if(bUpdate)
        PositionInfo.UpdateSeq=m_PositionData.NewUpdateSeq(PositionInfo.FTID);
}

//��̬���³ֲּ�¼�������ѣ��ɽ��޸ĳֲֺͲ�ѯ����������Ӧʱ����
bool CPlatformDataMgr_Positions::UpdatePositionByCommissionRate_RspQryRate(const PlatformStru_InstrumentCommissionRate& CommissionRate) 
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    string strInstrumentID(CommissionRate.InstrumentID);
    map<string,set<long>>::iterator it_Index2=m_PositionData.m_allIndex2.find(strInstrumentID);
    if(it_Index2!=m_PositionData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            map<long,PlatformStru_Position*>::iterator it_Info2=m_PositionData.m_all2.find(*itFTID);
            if(it_Info2!=m_PositionData.m_all2.end()&&it_Info2->second!=NULL)
            {
                char Direction;
                if(it_Info2->second->PosiDirection==THOST_FTDC_PD_Long) Direction=THOST_FTDC_D_Buy;
                else if(it_Info2->second->PosiDirection==THOST_FTDC_PD_Short) Direction=THOST_FTDC_D_Sell;

                //�ӳɽ���¼�м���������
                double Commission=m_RefTrades.GetCommission(strInstrumentID,string(it_Info2->second->Account),it_Info2->second->HedgeFlag,Direction);
                if(it_Info2->second->Commission!=Commission)
                { 
                    it_Info2->second->Commission=Commission;  
                    it_Info2->second->UpdateSeq=m_PositionData.NewUpdateSeq(it_Info2->second->FTID);
                }

                if(!brlt) brlt=true;
            }
        }
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

//���ʲ�ѯ��Ӧʱ�����³ֲ���صĹҵ��Ķ��������ѺͶ��ᱣ֤��
void CPlatformDataMgr_Positions::UpdatePositionFreezeFieldsByOrders_RspQryRate(const string& strInstrument)
{
    Lock_CPlatformDataMgr;

    map<string,set<long>>::iterator it_Index2=m_PositionData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_PositionData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            map<long,PlatformStru_Position*>::iterator it_Info2=m_PositionData.m_all2.find(*itFTID);
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

//��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
int CPlatformDataMgr_Positions::GetPositionKeySet(set<PositionKey> &PositionKeySet)
{
    LOG_INFO("CPlatformDataMgr_Positions::GetPositionKeySet");

    PositionKeySet.clear();

    Lock_CPlatformDataMgr;

    map<PositionKey,PlatformStru_Position*>::iterator it=m_PositionData.m_all.begin();
    for(;it!=m_PositionData.m_all.end();it++)
    {
        if(it->second!=NULL)
            PositionKeySet.insert(it->first);
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Positions::GetPositionKeySet-end");
    return PositionKeySet.size();
}

//��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr_Positions::GetPositionData(const string& strAccount,const string& InstrumentID,char PosiDirection,char HedgeFlag,PlatformStru_Position& PositionData)
{
    //LOG_INFO("CPlatformDataMgr_Positions::GetPositionData");

    PositionKey tmpKey( strAccount.c_str(),
						InstrumentID.c_str(), 
                        PosiDirection,
                        HedgeFlag);

    int irlt;

    Lock_CPlatformDataMgr;

    map<PositionKey,PlatformStru_Position*>::iterator it=m_PositionData.m_all.find(tmpKey);
    if(it!=m_PositionData.m_all.end()&&it->second!=NULL)
    {
        PositionData=*(it->second);
        irlt=0;
    }
    else irlt=-1;

    Unlock_CPlatformDataMgr;

    //LOG_INFO("CPlatformDataMgr_Positions::GetPositionData-end");
    return irlt;
}

//��ȡ���гֲּ�¼���ų�����Ϊ0�ļ�¼�����ؽ������
int CPlatformDataMgr_Positions::GetPositions(vector<PlatformStru_Position>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

	map<PositionKey,PlatformStru_Position*>::iterator it = m_PositionData.m_all.begin();
	for ( ; it != m_PositionData.m_all.end(); ++it )
	{
        if(it->second!=NULL&&it->second->Position>0)
            outData.push_back(*(it->second));
	}

    Unlock_CPlatformDataMgr;

    return outData.size();
}

//��ȡ���гֲּ�¼����������Ϊ0�ļ�¼�����ؽ������
int CPlatformDataMgr_Positions::GetPositions4(std::vector<PlatformStru_Position>& outData)
{
    outData.clear();

    Lock_CPlatformDataMgr;

	map<PositionKey,PlatformStru_Position*>::iterator it = m_PositionData.m_all.begin();
	for ( ; it != m_PositionData.m_all.end(); ++it )
	{
        outData.push_back(*(it->second));
	}

    Unlock_CPlatformDataMgr;

    return outData.size();
}


//����0���ɹ������������ݣ�1: �ɹ���������û�и��£����������ݣ�-1��ʧ��
int CPlatformDataMgr_Positions::GetPositions2(const string& strInstrument,set<long>& setFTID,vector<PlatformStru_Position>& outData,long& lastseq)
{
    int rlt;

    outData.clear();

    Lock_CPlatformDataMgr;

    if(lastseq==m_PositionData.GetLastUpdateSeq())
        rlt=1;
    else
    {
        set<long>* pFTIDSet=NULL;
        if(m_PositionData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
        {
            setFTID=(*pFTIDSet);

            PlatformStru_Position *pData=NULL;
            for(set<long>::iterator itFTID=pFTIDSet->begin();itFTID!=pFTIDSet->end();itFTID++)
            {
                if(m_PositionData.GetValue2_Internal(pData,(*itFTID))&&pData->Position>0)
                    outData.push_back(*pData);
                else
                    setFTID.erase(*itFTID);
            }
        }
        lastseq=m_PositionData.GetLastUpdateSeq();
        rlt=0;
    }

    Unlock_CPlatformDataMgr;

    return rlt;
}

//����0���ɹ������������ݣ�1: �ɹ���������û�и��£����������ݣ�-1��ʧ��
int CPlatformDataMgr_Positions::GetPositions3(const string& strInstrument,vector<PlatformStru_Position>& outData,long& lastseq)
{
    int rlt;

    outData.clear();

    Lock_CPlatformDataMgr;

    if(lastseq==m_PositionData.GetLastUpdateSeq())
        rlt=1;
    else
    {
        set<long>* pFTIDSet;
        if(m_PositionData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
        {
            PlatformStru_Position *pData;
            for(set<long>::iterator itFTID=pFTIDSet->begin();itFTID!=pFTIDSet->end();itFTID++)
            {
                if(m_PositionData.GetValue2_Internal(pData,(*itFTID))&&pData->Position>0)
                    outData.push_back(*pData);
            }
        }

        lastseq=m_PositionData.GetLastUpdateSeq();
        rlt=0;
    }

    Unlock_CPlatformDataMgr;

    return rlt;
}

//��ղ�ѯ�ֲֵ�map
void CPlatformDataMgr_Positions::ClearMapQryRltPositions(const string& strQryInstrument)
{
    Lock_CPlatformDataMgr;
    m_PositionData.WillQry_Internal(strQryInstrument);
    Unlock_CPlatformDataMgr;
}

//��ȡ��ѯ�ĺ�ԼID
void CPlatformDataMgr_Positions::GetQryPositionInstrument(string& outQryInstrument)
{
    outQryInstrument.clear();

    Lock_CPlatformDataMgr;
    m_PositionData.GetQryInstrument(outQryInstrument);
    Unlock_CPlatformDataMgr;
}

//��ȡ�ֲ�FTID���ϣ����ؽ������
int CPlatformDataMgr_Positions::GetPositionFTIDList(vector<long>& vec)
{
	vec.clear();
	Lock_CPlatformDataMgr;
	set<long>::iterator it_FTID = m_PositionData.m_allFTID.begin();
	for ( ; it_FTID != m_PositionData.m_allFTID.end(); ++it_FTID )
		vec.push_back(*it_FTID);
	Unlock_CPlatformDataMgr;

	return vec.size();
}

//��ȡָ��FTID�ĳֲ֣��ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr_Positions::GetPositionInfo(long lFTID, PlatformStru_Position& outData)
{
	int nRet = 0;
	Lock_CPlatformDataMgr;
	map<long, PlatformStru_Position*>::iterator it_FTID = m_PositionData.m_all2.find(lFTID);
	if ( it_FTID != m_PositionData.m_all2.end())
		outData = *(it_FTID->second);
	else
		nRet = -1;
	Unlock_CPlatformDataMgr;

	return nRet;
}

//��ȡָ��key�ĳֲ���Ϣ���ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr_Positions::GetPositionInfo(PositionKey key, PlatformStru_Position& outData)
{
	int nRet = 0;
	Lock_CPlatformDataMgr;
	map<PositionKey, PlatformStru_Position*>::iterator it_all = m_PositionData.m_all.find(key);
	if ( it_all != m_PositionData.m_all.end())
		outData = *(it_all->second);
	else
		nRet = -1;
	Unlock_CPlatformDataMgr;

	return nRet;
}

//ȡȫ���ֲֵ�ռ�ñ�֤�𡣿������ڴ��߱�֤���Ż�
void CPlatformDataMgr_Positions::GetAllMargin(double& TotalMargin)
{
	TotalMargin=0;

	Lock_CPlatformDataMgr;

    //������Ҫ�����ڵ��߱�֤���Żݼ���ĳֱֲ�֤��ע�⣬PositionKey��InstrumentID��ProductID
    map<PositionKey,double> ProductMargin;
    map<PositionKey,double> ProductExchangeMargin;
    map<PositionKey,double>::iterator itMargin,itMargin2,itMargin3,itMargin4;

    PlatformStru_InstrumentInfo InstrumentInfo;
	map<PositionKey,PlatformStru_Position*>::iterator it = m_PositionData.m_all.begin();
	for(;it != m_PositionData.m_all.end(); it++)
	{
        if(it->second!=NULL&&!util::isInvalidValue(it->second->UseMargin))
        {
            if(!m_RefInstruments.GetInstrumentInfo(string(it->first.InstrumentID),InstrumentInfo))
            {
                TotalMargin=util::GetDoubleInvalidValue();
                break;
            }

            if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Options||InstrumentInfo.ProductClass==THOST_FTDC_PC_SpotOption) 
            {
                //��Ȩ�����Ǵ��߱�֤���Ż�
                TotalMargin+=it->second->UseMargin;
            }
            else
            {
                if(InstrumentInfo.CombMarginDiscountMode!=4) TotalMargin+=it->second->UseMargin;
                else
                {
                    PositionKey tmpkey(it->first.Account,InstrumentInfo.ProductID,it->first.PosiDirection,it->first.HedgeFlag);
                    itMargin2=ProductMargin.find(tmpkey);
                    if(itMargin2==ProductMargin.end()) ProductMargin[tmpkey]=it->second->UseMargin;
                    else itMargin2->second+=it->second->UseMargin;

                    itMargin2=ProductExchangeMargin.find(tmpkey);
                    if(itMargin2==ProductExchangeMargin.end()) ProductExchangeMargin[tmpkey]=it->second->ExchangeMargin;
                    else itMargin2->second+=it->second->ExchangeMargin;
                }
            }
        }
        else 
        {
            TotalMargin=util::GetDoubleInvalidValue();
            break;
        }
	}

    //ͳ��ProductMargin��ı�֤��ע�⣬�����InstrumentID��ProductID. ���ڴ���Żݣ���Ҫ���ݽ�������֤��������
    if(!util::isInvalidValue(TotalMargin))
    {
        for(itMargin=ProductMargin.begin();itMargin!=ProductMargin.end();itMargin++)
        {
            if(itMargin->second==0) continue;

            PositionKey tmpkeys(itMargin->first.Account,itMargin->first.InstrumentID,itMargin->first.PosiDirection==THOST_FTDC_PD_Long?THOST_FTDC_PD_Short:THOST_FTDC_PD_Long,itMargin->first.HedgeFlag);
            itMargin2=ProductMargin.find(tmpkeys);
            if(itMargin2==ProductMargin.end())
            {
                //ֻ�е��߱�֤��
                TotalMargin+=itMargin->second;
                itMargin->second=0;
                continue;
            }
            else
            {
                //��˫�߱�֤���Ҵ��
                itMargin3=ProductExchangeMargin.find(itMargin->first);
                itMargin4=ProductExchangeMargin.find(tmpkeys);
                if(itMargin3!=ProductExchangeMargin.end()&&itMargin4!=ProductExchangeMargin.end()&&
                    !util::isInvalidValue(itMargin3->second)&&!util::isInvalidValue(itMargin4->second)&&
                    itMargin3->second!=itMargin4->second)
                {
                    if(itMargin3->second>itMargin4->second) TotalMargin+=itMargin->second;
                    else TotalMargin+=itMargin2->second;
                }
                else
                {
                    if(itMargin->second>=itMargin2->second) TotalMargin+=itMargin->second;
                    else TotalMargin+=itMargin2->second;
                }

                itMargin2->second=0;
                itMargin->second=0;
                continue;
            }
        }
    }

	Unlock_CPlatformDataMgr;
}

//ȡȫ���ֲֵ�ƽ��ӯ��(���ն���)
void CPlatformDataMgr_Positions::GetAllCloseProfitByDate(double&TotalCloseProfitByDate)
{
    TotalCloseProfitByDate=0;

	Lock_CPlatformDataMgr;

	map<PositionKey,PlatformStru_Position*>::iterator it = m_PositionData.m_all.begin();
	for(;it != m_PositionData.m_all.end(); it++)
	{
        if(it->second!=NULL&&!util::isInvalidValue(it->second->CloseProfit))
            TotalCloseProfitByDate+=it->second->CloseProfit;
        else 
        {
            TotalCloseProfitByDate=util::GetDoubleInvalidValue();
            break;
        }
	}

	Unlock_CPlatformDataMgr;
}

//ȡȫ���ֲֵĳֲ�ӯ��(���ն���)
void CPlatformDataMgr_Positions::GetAllPositionProfitByDate(double&TotalPositionProfitByDate)
{
	TotalPositionProfitByDate=0;
	std::map<PositionKey,PlatformStru_Position*>::iterator iter;
    double InvalideValue=util::GetDoubleInvalidValue();

	Lock_CPlatformDataMgr;
	iter = m_PositionData.m_all.begin();
	for(;iter != m_PositionData.m_all.end(); iter++)
	{
        //�ų���ϳֲ֡���ϳֲֵ�ӯ���ڷ�����ϳֲ��м���
        if(GlobalFunc::IsCombInstrument2(iter->second->InstrumentID))
            continue;
        if(TotalPositionProfitByDate!=InvalideValue)
        {
            if(iter->second!=NULL&&iter->second->PositionProfit!=InvalideValue)
                TotalPositionProfitByDate+=iter->second->PositionProfit;
            else TotalPositionProfitByDate=InvalideValue;
        }
	}
	Unlock_CPlatformDataMgr;
}


void CPlatformDataMgr_Positions::clear(void)
{
	Lock_CPlatformDataMgr;
    m_PositionData.Clear();
	Unlock_CPlatformDataMgr;
}
bool CPlatformDataMgr_Positions::empty(void)
{
    Lock_CPlatformDataMgr;
    bool brlt=m_PositionData.empty();
    Unlock_CPlatformDataMgr;
    return brlt;
}



