//ά���ײ�ֲ���ϸ��Ϣ

#pragma warning(disable:4996)

#include <assert.h>
#include <vector>
#include <string>
#include <map>
using namespace std;

#include "PlatformDataMgr_PositionDetails.h"
#include "PlatformDataMgrMacro.h"
#include "FileOpr.h"
#include "Module-Misc2/GlobalFunc.h"


#ifdef WIN32
#pragma unmanaged
#endif 



CPlatformDataMgr_PositionDetails::CPlatformDataMgr_PositionDetails( 
    volatile long& baseFTID,
    volatile long& baseUpdateSeq,
    CPlatformDataMgr_Instruments& refInstruments,
    CPlatformDataMgr_Rates& refRates,
    CPlatformDataMgr_Quots& refQuots,
    CPlatformDataMgr_Trades& refTrades,
    CPlatformDataMgr_Orders& refOrders,
    CPlatformDataMgr_PositionDetailCombs& refPositionDetailCombs,
    int DiscountCZCE_Comb,
    int DiscountDCE_Comb,
    int UnwindPrioritiesCZCE_Comb,
    int UnwindPrioritiesDCE_Comb,
    int PriceForMarginOfTodayPositionCZCE_Comb,
    int PriceForMarginOfTodayPositionDCE_Comb,
    bool bUseSettlementPriceToCalculateProfitAndMarginAsPossible)
: m_RefInstruments(refInstruments),
  m_RefRates(refRates),
  m_RefQuots(refQuots),
  m_RefTrades(refTrades),
  m_RefOrders(refOrders),
  m_RefPositionDetailCombs(refPositionDetailCombs),
  m_DiscountCZCE_Comb(DiscountCZCE_Comb),
  m_DiscountDCE_Comb(DiscountDCE_Comb),
  m_PositionDetailData(baseFTID,baseUpdateSeq),
  m_UnwindPrioritiesCZCE_Comb(UnwindPrioritiesCZCE_Comb),
  m_UnwindPrioritiesDCE_Comb(UnwindPrioritiesDCE_Comb),
  m_PriceForMarginOfTodayPositionCZCE_Comb(PriceForMarginOfTodayPositionCZCE_Comb),
  m_PriceForMarginOfTodayPositionDCE_Comb(PriceForMarginOfTodayPositionDCE_Comb),
  m_PriceForRoyalty_Option(0),
  m_bHaveComb(false),
  m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible(bUseSettlementPriceToCalculateProfitAndMarginAsPossible)
{
}

CPlatformDataMgr_PositionDetails::~CPlatformDataMgr_PositionDetails(void)
{
    LOG_INFO("CPlatformDataMgr_PositionDetails::~CPlatformDataMgr_PositionDetails(����CPlatformDataMgr_PositionDetails����)");
}


//��ѯ�ر�����
void CPlatformDataMgr_PositionDetails::QryPositionDetailReach( PlatformStru_PositionDetail& PositionDetailInfo,int ErrID,bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr_PositionDetails::QryPositionDetailReach:InstrumentID=%s",PositionDetailInfo.InstrumentID);

    if(ErrID==0)
    {
        //���ö�Ӧ��PriceTick
        string strInstrumentID=string(PositionDetailInfo.InstrumentID);
        PlatformStru_InstrumentInfo InstrumentInfo;
        if(m_RefInstruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo))
            PositionDetailInfo.PriceTick=InstrumentInfo.PriceTick;

        //�����Ƿ���
        PositionDetailInfo.bToday=m_RefInstruments.IsToday(PositionDetailInfo.OpenDate);
        //���ÿ���ʱ��, ���Ϊ��Ӧ�Ŀ��ֳɽ�ʱ��, ���Ϊ00:00:00
        memset(PositionDetailInfo.OpenTime,0,sizeof(PositionDetailInfo.OpenTime));
        if(!PositionDetailInfo.bToday) 
            strncpy(PositionDetailInfo.OpenTime,"00:00:00",sizeof(PositionDetailInfo.OpenTime));
        else
        {
            vector<TradeKey> TradeKeys;
            PlatformStru_TradeInfo TradeInfo;
            m_RefTrades.GetTradeKeysOfTradeID(PositionDetailInfo.TradeID,TradeKeys);
            for(int i=0;i<(int)TradeKeys.size();i++)
            {
                if(m_RefTrades.GetTradeInfo(TradeKeys[i],TradeInfo)&&strInstrumentID==string(TradeInfo.InstrumentID))
                {
                    strncpy(PositionDetailInfo.OpenTime,TradeInfo.TradeTime,sizeof(PositionDetailInfo.OpenTime)-1);
                    break;
                }
            }
        }
        PositionDetailInfo.OpenTimeUTC=(int)CTools_Ansi::Get_time_t_utc_from_strTime(PositionDetailInfo.OpenDate,PositionDetailInfo.OpenTime);
    }

    m_mutex.lock();

    //�������͵����ݼ���
    m_PositionDetailData.QryRltReach_Internal(PositionDetailInfo,ErrID,bIsLast);

    if(ErrID==0)
    {
        CTools_Ansi::AddKeyValueToMultimap<int,PositionDetailKey>(m_mapOpenTime2Key,PositionDetailInfo.OpenTimeUTC,PositionDetailKey(PositionDetailInfo));
        if(PositionDetailInfo.TradeType==THOST_FTDC_TRDT_CombinationDerived)
            m_bHaveComb=true;
    }

    if(bIsLast)
    {
        //���³ֲ���ϸ�ĳֲ�ӯ��, ����ӯ��, ������, �����, �ּ�
        set<string> setInstruments;
        m_PositionDetailData.GetInstrumentSetOfQryRlt(setInstruments);

        set<string>::iterator it_Instrument;
        PlatformStru_DepthMarketData QuotData;
        PlatformStru_InstrumentInfo InstrumentInfo;

        for(it_Instrument=setInstruments.begin();it_Instrument!=setInstruments.end();it_Instrument++)
        {
            if(m_RefInstruments.GetInstrumentInfo(*it_Instrument,InstrumentInfo)&&
                m_RefQuots.GetQuotInfo(*it_Instrument,QuotData))
            {
               UpdatePositionDetailByQuot2_Internal(QuotData,InstrumentInfo,true);
            }
        }
    }

    m_mutex.unlock();

    LOG_INFO("CPlatformDataMgr_PositionDetails::QryPositionDetailReach-end");
}



//��̬���³ֲ���ϸ��¼�ĳֲ�ӯ��, ����ӯ��, �����, ������, �ּ�, ��������, �õ�������ʱ��һ��
void CPlatformDataMgr_PositionDetails::UpdatePositionDetail_RtnDepthMarketData(const PlatformStru_DepthMarketData& QuotData,
                                                                               const PlatformStru_InstrumentInfo& InstrumentInfo) 
{
    CeasymutexGuard guard(m_mutex);

    UpdatePositionDetailByQuot2_Internal(QuotData,InstrumentInfo,false);
}

//���º�Լ��Ӧ�ĳֲ���ϸ��¼�ĳֲ�ӯ��, ����ӯ��, �����, ������, �ּ�, ��������, �õ�������ʱ��һ��
//���������bForceCalculateΪfalse, ��ʾ���ڼ۸�䶯ʱ����, �������Ϊtrue
void CPlatformDataMgr_PositionDetails::UpdatePositionDetailByQuot2_Internal(const PlatformStru_DepthMarketData& QuotData,
                                                                            const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                                            bool bForceCalculate) 
{
    map<string,set<long>>::iterator it_Index2=m_PositionDetailData.m_allIndex2.find(string(QuotData.InstrumentID));
    if(it_Index2!=m_PositionDetailData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            PlatformStru_PositionDetail* pdetail;
            if(m_PositionDetailData.GetValue2_Internal(pdetail,*itFTID))
            {
                bool bUpdate;
                UpdatePositionDetailByQuot_Internal(QuotData,InstrumentInfo,bForceCalculate,*pdetail,bUpdate);
                if(bUpdate) pdetail->UpdateSeq=m_PositionDetailData.NewUpdateSeq(pdetail->FTID);
            }
        }
    }
}

//���³ֲ���ϸ��¼�ĳֲ�ӯ��(����), ����ӯ��(���), �����, ������, �ּ�, ��������
//���������bForceCalculateΪfalse, ��ʾ���ڼ۸�䶯ʱ����, �������Ϊtrue
//����ӯ������ʹ�õļ۸�˳��Ϊ: ����� > ���¼� > ������
void CPlatformDataMgr_PositionDetails::UpdatePositionDetailByQuot_Internal(const PlatformStru_DepthMarketData& QuotData,
                                                                           const PlatformStru_InstrumentInfo InstrumentInfo,
                                                                           bool bForceCalculate,
                                                                           PlatformStru_PositionDetail& detail,
                                                                           bool& bUpdate)
{
    bUpdate=false;

    double CurPrice;
    if(m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible&&!util::isInvalidValue(QuotData.SettlementPrice)) CurPrice=QuotData.SettlementPrice;
    else if(QuotData.Volume>0) CurPrice=QuotData.LastPrice;
    else CurPrice=QuotData.PreSettlementPrice;
    if(util::isInvalidValue(CurPrice))
        return;

    //���½���ۡ�������
    if(util::isInvalidValue(detail.LastSettlementPrice)&&!util::isInvalidValue(QuotData.PreSettlementPrice)) { detail.LastSettlementPrice=QuotData.PreSettlementPrice;  bUpdate=true; }
    if(util::isInvalidValue(detail.SettlementPrice)&&!util::isInvalidValue(QuotData.SettlementPrice)) { detail.SettlementPrice=QuotData.SettlementPrice;  bUpdate=true; }

    //��ǿ�Ƽ���ʱ, ��۸�δ����, �����м���
    if(!bForceCalculate&&CurPrice==detail.CurPrice) return;

    //�����ּ�
    if(CurPrice!=detail.CurPrice) { detail.CurPrice=CurPrice;  bUpdate=true; }

    //�������ն��гֲ�ӯ��
    double LastPrice = detail.bToday ? detail.OpenPrice : detail.LastSettlementPrice;
    double DeltaPrice = detail.Direction==THOST_FTDC_D_Buy ? CurPrice-LastPrice : LastPrice-CurPrice;
    double PositionProfitByDate = DeltaPrice*detail.Volume*InstrumentInfo.VolumeMultiple;
    if(PositionProfitByDate==-0) PositionProfitByDate=0;

    //���㴥�ʶԳ�ֲ�ӯ��(����ӯ��)
    DeltaPrice = detail.Direction==THOST_FTDC_D_Buy ? CurPrice-detail.OpenPrice : detail.OpenPrice-CurPrice;
    double PositionProfitByTrade=DeltaPrice*detail.Volume*InstrumentInfo.VolumeMultiple;
    if(PositionProfitByTrade==-0) PositionProfitByTrade=0;

    if( PositionProfitByDate!=detail.PositionProfitByDate){ detail.PositionProfitByDate=PositionProfitByDate; bUpdate=true; }
    if( PositionProfitByTrade!=detail.PositionProfitByTrade){ detail.PositionProfitByTrade=PositionProfitByTrade; bUpdate=true; }
}

//����ʱ, �ý���۸��³ֲ���ϸ��¼�ĳֲ�ӯ��(����), ����ӯ��(���), ��֤��, �����, ������, �ּ�
void CPlatformDataMgr_PositionDetails::UpdatePositionDetail_OnSettlement() 
{
    m_mutex.lock();

    PlatformStru_DepthMarketData QuotData;
    PlatformStru_InstrumentInfo InstrumentInfo;
    PlatformStru_InstrumentMarginRate MarginRate;
    map<PositionDetailKey,PlatformStru_PositionDetail> ModifiedPositionDetails;
    map<PositionDetailCombKey,PlatformStru_PositionDetailComb> ModifiedPositionDetailCombs;
    for(set<long>::iterator it=m_PositionDetailData.m_allFTID.begin();it!=m_PositionDetailData.m_allFTID.end();it++)
    {
        map<long,PlatformStru_PositionDetail*>::iterator it_Info2=m_PositionDetailData.m_all2.find(*it);
        if(it_Info2!=m_PositionDetailData.m_all2.end()&&it_Info2->second!=NULL)
        {
            if(m_RefInstruments.GetInstrumentInfo(string(it_Info2->second->InstrumentID),InstrumentInfo)&&
                m_RefQuots.GetQuotInfo(string(it_Info2->second->InstrumentID),QuotData)&&
                m_RefRates.GetMarginRate(string(it_Info2->second->InstrumentID),MarginRate)==0)
            {
                bool bUpdate1,bUpdate2;

                PlatformStru_PositionDetail detail=*(it_Info2->second);

                //���³ֲ���ϸ�ĳֲ�ӯ��, ����ӯ��, ������, �����, �ּ�
                UpdatePositionDetailByQuot_Internal(QuotData,InstrumentInfo,true,*(it_Info2->second),bUpdate1); 

                //��ʱ�ֲ���ϸ�еĽ�����Ѿ�����, ���㱣֤��(����������Ż�)
                CalculateMargin_Internal(MarginRate,InstrumentInfo,*(it_Info2->second),bUpdate2);

                if(bUpdate1||bUpdate2) 
                {
                    it_Info2->second->UpdateSeq=m_PositionDetailData.NewUpdateSeq(it_Info2->second->FTID);
                    PositionDetailKey key(detail);
                    if(ModifiedPositionDetails.find(key)==ModifiedPositionDetails.end())
                        ModifiedPositionDetails[key]=detail;
                }
            }
        }
    }

    m_mutex.unlock();

    //����б仯�ĳֲ���ϸ, ������ϳֲ���ϸ�ı�֤���Ż�. ��Ҫ�ڸ��³ֲ���ϸ�͸�����ϳֲ���ϸ�����. 
    CalcuPositionDetailMarginDiscount_RtnTrade_RspQryRate_OnSettlement(ModifiedPositionDetails,ModifiedPositionDetailCombs);
}


//ȡ���ֲ���ϸ�еļ�¼����
int CPlatformDataMgr_PositionDetails::GetRecordCountOfPositionDetail(void)
{
    CeasymutexGuard guard(m_mutex);

    return (int)(m_PositionDetailData.m_all.size());
}

//�ӳɽ���¼���³ֲ���ϸ. ���޸�TradeInfo��ƽ����, ƽ��ӯ��
//ModifiedPositionDetails�����ܵ�Ӱ��ĳֲ���ϸ��¼, n*2+0��ԭ��������, n*2+1��������
//ComTradeID2CloseVolume:ƽ���������λʱ����ƽ������ϳɽ�ID�Ͷ�Ӧƽ�����Ĺ�ϵ
void CPlatformDataMgr_PositionDetails::UpdatePositionDetail_RtnTrade(PlatformStru_TradeInfo& TradeInfo,
                                                                     const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                                     map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                                                     map<string,int>& ComTradeID2CloseVolume)
{
    LOG_INFO("CPlatformDataMgr_PositionDetails::UpdatePositionDetail_RtnTrade(�ɽ��ر����³ֲ���ϸ):InstrumentID=%s",TradeInfo.InstrumentID);

    string strInstrument(TradeInfo.InstrumentID);
    if(strInstrument.empty())
        return;

    CeasymutexGuard guard(m_mutex);

    m_DerivedInstrumentIDs_OnCloseTrade.clear();

    UpdatePositionDetail_RtnTrade_Internal(TradeInfo,false,InstrumentInfo,ModifiedPositionDetails,ComTradeID2CloseVolume,m_PositionDetailData);

    for(map<PositionDetailKey,PlatformStru_PositionDetail>::const_iterator it=ModifiedPositionDetails.begin();it!=ModifiedPositionDetails.end();it++)
    {
        //��ǰ�ֲ���ϸ�ĺ�ԼID
        string thisInstrumentID(it->first.InstrumentID);

        //���ƽ�ֳɽ����Ƿ��б���ɢ����������ͨ�ֲ���ϸ�ĺ�ԼID. ��GUI�и��³ֲ���ϸʱ, ��Ҫ���´�������Լ�ĳֲ���ϸ
        if(thisInstrumentID!=strInstrument)
        {
            if(m_DerivedInstrumentIDs_OnCloseTrade.find(thisInstrumentID)==m_DerivedInstrumentIDs_OnCloseTrade.end())
                m_DerivedInstrumentIDs_OnCloseTrade.insert(thisInstrumentID);
        }

        //������½��ĳֲ���ϸ, �������ֳɽ���ƽ�ִ�ɢ����������, ά��m_mapOpenTime2Key��m_bHaveComb
        PlatformStru_PositionDetail* pdetail;
        if(it->second.IsInvalidRecord()&&
            m_PositionDetailData.GetValue_Internal(pdetail,it->first))
        {
            CTools_Ansi::AddKeyValueToMultimap<int,PositionDetailKey>(m_mapOpenTime2Key,pdetail->OpenTimeUTC,PositionDetailKey(*pdetail));
            if(it->first.TradeType==THOST_FTDC_TRDT_CombinationDerived)
                m_bHaveComb=true;
        }
    }

    LOG_INFO("CPlatformDataMgr_PositionDetails::UpdatePositionDetail_RtnTrade-end");
}

//�ӳɽ���¼���³ֲ���ϸ
//����ƽ���ĳֲ���ϸ�ļ�¼��, ���ں�������. ModifiedPositionDetails�����ܵ�Ӱ��ĳֲ���ϸ��¼, n*2+0��ԭ��������, n*2+1��������
//���޸�TradeInfo��ƽ����, ƽ��ӯ��
//bPartCalcu:ֻ����ƽ����, ƽ��ӯ��, �����㱣֤��, �ֲ�ӯ����
//ComTradeID2CloseVolume:ƽ���������λʱ����ƽ������ϳɽ�ID�Ͷ�Ӧƽ�����Ĺ�ϵ
void CPlatformDataMgr_PositionDetails::UpdatePositionDetail_RtnTrade_Internal(PlatformStru_TradeInfo& TradeInfo,
                                                                              bool bPartCalcu,
                                                                              const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                                              map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                                                              map<string,int>& ComTradeID2CloseVolume,
                                                                              CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& PositionDetails)
{
    string strInstrument(TradeInfo.InstrumentID);
    if(strInstrument.empty())
        return;


    TradeInfo.CloseTodayVolume=0;
    TradeInfo.CloseProfitByDate=0;
    TradeInfo.CloseProfitByTrade=0;

    //1. �ӳɽ��ر�������سֲ���ϸ�Ĳ�λ�仯
    UpdatePositionDetailVolumeOnTrade_Internal(TradeInfo,InstrumentInfo,ModifiedPositionDetails,ComTradeID2CloseVolume,PositionDetails,m_RefPositionDetailCombs,true);

    //2. ����ֲ���ϸ��Ӱ�������ֵ
    PlatformStru_DepthMarketData QuotInfo;
    bool bHaveQuot = bPartCalcu ? false : m_RefQuots.GetQuotInfo(strInstrument,QuotInfo);

    PlatformStru_InstrumentMarginRate MarginRate;
    bool bHaveMarginRate = bPartCalcu ? false : m_RefRates.GetMarginRate(strInstrument,MarginRate)==0;

    PlatformStru_InstrumentInfo otherInstrumentInfo;
    PlatformStru_InstrumentMarginRate otherMarginRate;
    PlatformStru_DepthMarketData otherQuotInfo;
    PlatformStru_PositionDetail *pdetail;
    bool bUpdate;
    for(map<PositionDetailKey,PlatformStru_PositionDetail>::const_iterator it=ModifiedPositionDetails.begin();it!=ModifiedPositionDetails.end();it++)
    {
        if(!PositionDetails.GetValue_Internal(pdetail,it->first)) continue;

        //��ǰ�ֲ���ϸ�ĺ�ԼID
        string thisInstrumentID=string(it->first.InstrumentID);

        //��ǰ�ֲ���ϸ�ĺ�Լ��Ϣ
        if(thisInstrumentID!=strInstrument&&
            !m_RefInstruments.GetInstrumentInfo(thisInstrumentID,otherInstrumentInfo)) 
            continue;
        const PlatformStru_InstrumentInfo& thisInstrumentInfo = thisInstrumentID==strInstrument ? InstrumentInfo : otherInstrumentInfo;

        if(!bPartCalcu)
        {
            //���³ֲ���ϸ�Ľ����, ������, �ּ�, �ֲ�ӯ��(����), ����ӯ��(���)
            if(thisInstrumentID==strInstrument ? bHaveQuot : (m_RefQuots.GetQuotInfo(thisInstrumentID,otherQuotInfo)))
            {
                UpdatePositionDetailByQuot_Internal(thisInstrumentID==strInstrument?QuotInfo:otherQuotInfo,thisInstrumentInfo,true,*pdetail,bUpdate); 
            }

            //���³ֲ���ϸ�ı�֤��, ��֤����, ��������֤�����ﲻ��������Ż�
            if(thisInstrumentID==strInstrument ? bHaveMarginRate : (m_RefRates.GetMarginRate(thisInstrumentID,otherMarginRate)==0))
            {
                CalculateMargin_Internal(thisInstrumentID==strInstrument?MarginRate:otherMarginRate,thisInstrumentInfo,*pdetail,bUpdate);
            }
        }

        pdetail->UpdateSeq=PositionDetails.NewUpdateSeq(pdetail->FTID);
    }

}

//�����ճ��ֲ���ϸ, ���ڳɽ���¼, ���¼���ɽ���ƽ����, ƽ��ӯ��, ������
//��ʼ����ѯ��ɽ��ͳֲ���ϸ�����
//ע��: 1. ����֣��ƽ���Ż�, ���ǰ�ƽ���ĳֲ���ϸ�Ƿ������ж�, ���ǰ��Ƿ��н�ֿ�ƽ���ж�. 
//         ����, �����1�ֽ��1��, ƽ1��, ƽ�������, ���������н��, ����������Ż�
//      2. ���̡����ڡ��н��ȿ���ƽԭ�����ƽ�֣����ĳ����Լ�Ŀ�ƽ�֣����ÿ�����Ϻ�Լ�Ķ���
//         ֣�̰��ȵ�һ����ϵ�ԭ�����ƽ�֣���Ҫ������Ϻ�Լ�Ķ��֡���֣�̸�ҹ�����д�ɢ���飬�������ϳֲ���ϸ�ͳֲ���ϸһһ��Ӧ�����Դ��ճ��ֲ���ϸ�������ϳֲ���ϸ
void CPlatformDataMgr_PositionDetails::ReCalcuTradeInfo_OnInitQryCompleted(void)
{
    volatile long baseFTID=1;
    volatile long baseUpdateSeq=1;
    CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail> PositionDetails(baseFTID,baseUpdateSeq);

    CeasymutexGuard guard(m_mutex);

    //��ȡȫ���ɽ���FTID, ����ԼID����. ͬһ����Լ��FTID�������
    map<string,vector<long>> TradeFTIDs;
    m_RefTrades.GetFTIDsByInstrument(TradeFTIDs);

    //��ȡ�ճ��ֲ���ϸ
    GetStaticPositionDetail_Internal(PositionDetails);

    //��ȡ֣�̵��ճ���ϳֲ���ϸ
    vector<PlatformStru_PositionDetailComb> vecPositionDetailCombs_CZCE;
    m_RefPositionDetailCombs.GetStaticPositionDetailComb_CZCE(PositionDetails,vecPositionDetailCombs_CZCE);
    CPlatformDataMgr_PositionDetailCombs StaticPositionDetailCombs_CZCE(baseFTID,baseUpdateSeq,m_RefInstruments,m_RefQuots,m_RefTrades,m_RefOrders);
    CPlatformDataMgr_PositionDetailCombs StaticPositionDetailCombs2(baseFTID,baseUpdateSeq,m_RefInstruments,m_RefQuots,m_RefTrades,m_RefOrders);
    for(int i=0;i<(int)vecPositionDetailCombs_CZCE.size();i++)
        StaticPositionDetailCombs_CZCE.QryPositionDetailCombReach(vecPositionDetailCombs_CZCE[i],0,i==(int)vecPositionDetailCombs_CZCE.size()-1?true:false);


    //��ÿһ�ʳɽ�, ���³ֲ���ϸ, �Լ��ɽ��������

    map<PositionDetailKey,PlatformStru_PositionDetail> ModifiedPositionDetails;
    map<PositionDetailCombKey,PlatformStru_PositionDetailComb> ModifiedPositionDetailCombs;
    map<string,int> ComTradeID2CloseVolume;
    PlatformStru_TradeInfo TradeInfo;
    PlatformStru_InstrumentInfo InstrumentInfo;
    string strExchangeID;

    //���̡����ں��н����ȿ���ƽ�����ÿ�����ϴ�ɢ
    //֣�������ȵ�һ����ϣ���Ҫ������ϴ�ɢ
    for(map<string,vector<long>>::iterator it=TradeFTIDs.begin();it!=TradeFTIDs.end();it++)
    {
        if(!m_RefInstruments.GetInstrumentInfo(it->first,InstrumentInfo)) continue;
        strExchangeID=string(InstrumentInfo.ExchangeID);

        for(int i=0;i<(int)it->second.size();i++)
        {
            if(!m_RefTrades.GetTradeInfo(it->second[i],TradeInfo)) continue;

            ModifiedPositionDetails.clear();

            TradeInfo.CloseTodayVolume=0;
            TradeInfo.CloseProfitByDate=0;
            TradeInfo.CloseProfitByTrade=0;

            //�ӳɽ��ر�������سֲ���ϸ�Ĳ�λ�仯
            if(strExchangeID==string("CZCE"))
                UpdatePositionDetailVolumeOnTrade_Internal(TradeInfo,InstrumentInfo,ModifiedPositionDetails,ComTradeID2CloseVolume,PositionDetails,StaticPositionDetailCombs_CZCE,true);
            else
                UpdatePositionDetailVolumeOnTrade_Internal(TradeInfo,InstrumentInfo,ModifiedPositionDetails,ComTradeID2CloseVolume,PositionDetails,StaticPositionDetailCombs2,false);

            //���³ɽ���¼�е�ƽ����, ƽ��ӯ��, ����������. 
            m_RefTrades.UpdateTradeCloseTodayVolume_CloseProfit_RecalcuCommission(TradeInfo.FTID,
                InstrumentInfo,
                TradeInfo.CloseTodayVolume,
                TradeInfo.CloseProfitByDate,
                TradeInfo.CloseProfitByTrade,
                TradeInfo.TradeCommission);
        }
    }
}


//��ȡ�ճ��ֲ���ϸ
void CPlatformDataMgr_PositionDetails::GetStaticPositionDetail_Internal(CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& StaticPositionDetail)
{
    StaticPositionDetail.Clear();
    vector<long> FTIDs;
    PlatformStru_PositionDetail detail;

    m_PositionDetailData.GetAllFTID_Internal(FTIDs);
    for(int i=0;i<(int)FTIDs.size();i++)
    {
        if(m_PositionDetailData.GetValue3_Internal(detail,FTIDs[i])&&
            !detail.bToday)
        {
            int CloseVolume=detail.CloseVolume;
            if(CloseVolume>0)
            {
                detail.Volume+=CloseVolume;
                detail.CloseVolume=0;
                detail.CloseAmount=0;
                detail.CloseProfitByDate=0;
                detail.CloseProfitByTrade=0;
                detail.PositionProfitByDate=0;
                detail.PositionProfitByTrade=0;
                detail.CurPrice=detail.LastSettlementPrice;
            }
            StaticPositionDetail.NewReach_Internal(detail);
        }
    }
}

//�ӳɽ��ر�������سֲ���ϸ��λ�ı仯. ע��, ֻ���³ֲ���ϸ�Ĳ�λ��ƽ������ƽ�ֽ��, ����������ֵ�����³ɽ���Ϣ�е�ƽ������ƽ��ӯ��
//PositionDetailData�����ݼ�. ������m_PositionDetailData,Ҳ��������ʱ���ݼ�(Ϊ��ƥ��ɽ��ͳֲ���ϸ)
//bCareCombOppoDetail:��ʾ�ֲ���ϸ���������, �Ƿ������ֲֳ���ϸ���д�ɢ�����������������Ҫ���ڳ�ʼ����ѯ��������������������֣�����Ͳ���Ҫ
//ComTradeID2CloseVolume:ƽ���������λʱ����ƽ������ϳɽ�ID�Ͷ�Ӧƽ�����Ĺ�ϵ
void CPlatformDataMgr_PositionDetails::UpdatePositionDetailVolumeOnTrade_Internal(PlatformStru_TradeInfo& TradeInfo,
                                                                                  const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                                                  map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                                                                  map<string,int>& ComTradeID2CloseVolume,
                                                                                  CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& PositionDetailData,
                                                                                  CPlatformDataMgr_PositionDetailCombs& PositionDetailCombData,
                                                                                  bool bCareCombOppoDetail)
{
    if(TradeInfo.Volume<=0)
        return;
    string strInstrument(TradeInfo.InstrumentID);
    if(strInstrument.empty())
        return;

    ComTradeID2CloseVolume.clear();

    string strExchangeID(TradeInfo.ExchangeID);

    if(TradeInfo.OffsetFlag==THOST_FTDC_OF_Open)
    {                                                   
        //���ֳɽ������ֲ���ϸ

	    PlatformStru_PositionDetail PositionDetailInfo(TradeInfo);

        //�������ĳֲ���ϸ�͵����ݼ���
        PositionDetailData.NewReach_Internal(PositionDetailInfo);

        //����Ӱ�쵽�ĳֲ���ϸ��¼
        PositionDetailKey key(PositionDetailInfo);
        if(ModifiedPositionDetails.find(key)==ModifiedPositionDetails.end())
            ModifiedPositionDetails[key]=PlatformStru_PositionDetail(true);
    }

    else if( TradeInfo.OffsetFlag==THOST_FTDC_OF_Close||
        TradeInfo.OffsetFlag==THOST_FTDC_OF_ForceClose||
        TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseToday||
        TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseYesterday||
        TradeInfo.OffsetFlag==THOST_FTDC_OF_LocalForceClose)
    { 
        //ƽ�ֳɽ��޸ĳֲ���ϸ
        //����TradeInfo�ĺ�Լ, ��ƽ��־, ��������, ƽ����Ӧ�Ĳ�λ, ������λ, ƽ����, ƽ�ֽ��
        //ѭ������, ֱ��ƽ������������
        //1. ֣�̴�����ϳɽ�, ����������ȵ�һ����ϵ�ƽ��˳��, ��ƽ��һ�ֲ�
        //2. ����������ȿ���ƽԭ��

        int CloseVolume=TradeInfo.Volume;

        //ע��: set<PositionDetailKey>���ǰ�ʱ�������, ��ǰ������, ���ܱ�֤�ȿ���ƽ
        map<string,set<PositionDetailKey>>::iterator it_keyset=PositionDetailData.m_allIndex.find(strInstrument);
        if(it_keyset==PositionDetailData.m_allIndex.end()||it_keyset->second.empty())
            return;

        //��һ��, ��ƽ��һ�ֲ�. (ֻ��֣�̴����������ȵ�һ����ϵ�ƽ��˳����Ч)
        //�ڶ���, ��ʣ���ƽ����, ͳһ���ȿ���ƽԭ�����ƽ��
        for(int step=0;step<2&&CloseVolume>0;step++)
        {
            if(step==0&&
                !(strExchangeID==string("CZCE")&&m_UnwindPrioritiesCZCE_Comb==1||
                  strExchangeID==string("DCE")&&m_UnwindPrioritiesDCE_Comb==1))
            {
                //��������������ȵ�һ����ϵ�ƽ�ֹ���, ������һ��
                continue;
            }

	        for(set<PositionDetailKey>::iterator itKey=it_keyset->second.begin() ; CloseVolume>0&&itKey!=it_keyset->second.end() ; itKey++)
	        {
                //����ǵ�һ��, ������ϳֲ���ϸ
                if(step==0&&itKey->TradeType==THOST_FTDC_TRDT_CombinationDerived) continue;

                PlatformStru_PositionDetail *pPositionDetail;
                if(!PositionDetailData.GetValue_Internal(pPositionDetail,*itKey)) continue;

                //��������Ϊ��ĳֲ���ϸ
                if(pPositionDetail->Volume==0) continue;

                //�����Ƕ�Ӧ�ֲ���ϸ
		        if(!IsPositionDetailForCloseTrade_IgnoreTradeType(*pPositionDetail,TradeInfo)) continue;

		        //��¼����ǰ�ĳֲ���ϸ����
                if(ModifiedPositionDetails.find(*itKey)==ModifiedPositionDetails.end())
                    ModifiedPositionDetails[*itKey]=*pPositionDetail;

		        //PlatformStru_PositionDetail OldPositionDetail=*pPositionDetail;

                //����ֲ���ϸ����ƽ��������
                int thisVolume = CloseVolume>pPositionDetail->Volume ? pPositionDetail->Volume : CloseVolume;

	            //���²�λ����
                pPositionDetail->Volume-=thisVolume;
	            CloseVolume-=thisVolume;

                //����ƽ����, ƽ�ֽ��
                pPositionDetail->CloseVolume+=thisVolume;
                pPositionDetail->CloseAmount+=thisVolume*InstrumentInfo.VolumeMultiple*TradeInfo.Price;
                if(pPositionDetail->bToday) TradeInfo.CloseTodayVolume+=thisVolume;

                //����ƽ��ӯ��
                double dLastPrice = pPositionDetail->bToday ? pPositionDetail->OpenPrice : pPositionDetail->LastSettlementPrice;
                double DeltaPrice = pPositionDetail->Direction==THOST_FTDC_D_Buy ? TradeInfo.Price-dLastPrice : dLastPrice-TradeInfo.Price;
                double CloseProfitByDate=DeltaPrice*thisVolume*InstrumentInfo.VolumeMultiple;
                if(CloseProfitByDate==-0) CloseProfitByDate=0;
                pPositionDetail->CloseProfitByDate+=CloseProfitByDate;
                TradeInfo.CloseProfitByDate+=CloseProfitByDate;

                DeltaPrice = pPositionDetail->Direction==THOST_FTDC_D_Buy ? TradeInfo.Price-pPositionDetail->OpenPrice : pPositionDetail->OpenPrice-TradeInfo.Price;
                double CloseProfitByTrade=DeltaPrice*thisVolume*InstrumentInfo.VolumeMultiple;
                if(CloseProfitByTrade==-0) CloseProfitByTrade=0;
                pPositionDetail->CloseProfitByTrade+=CloseProfitByTrade;
                TradeInfo.CloseProfitByTrade+=CloseProfitByTrade;


                //����ֲ���ϸ���������, ������ֲֳ���ϸ���д�ɢ����
                //�����ж�����ֲֳ���ϸ��֮��Ӧ
                if(pPositionDetail->TradeType==THOST_FTDC_TRDT_CombinationDerived&&
                    bCareCombOppoDetail)
                {
                    int thisVolume2=thisVolume;

                    set<PositionDetailCombKey> combkeys;
                    PositionDetailCombData.GetCombKeysByPositionDetailKey(*itKey,combkeys,true);
                    set<string> ComTradeIDs;
                    for(set<PositionDetailCombKey>::const_iterator itcombkey=combkeys.begin();itcombkey!=combkeys.end();itcombkey++)
                    {
                        string strComTradeID(itcombkey->ComTradeID);
                        if(ComTradeIDs.find(strComTradeID)==ComTradeIDs.end()) ComTradeIDs.insert(strComTradeID);
                    }

                    for(set<string>::const_iterator itComTradeID=ComTradeIDs.begin();thisVolume2>0&&itComTradeID!=ComTradeIDs.end();itComTradeID++)
                    {
                        set<PositionDetailCombKey> combkeys2;
                        PositionDetailCombData.GetCombKeysByComTradeID(*itComTradeID,combkeys2,true);
                        for(set<PositionDetailCombKey>::const_iterator itkey=combkeys2.begin();thisVolume2>0&&itkey!=combkeys2.end();itkey++)
                        {
                            //ֻ���������ϳֲ���ϸ
                            if(itkey->Direction==pPositionDetail->Direction) continue;

                            PositionDetailKey oppodetailkey;
                            Convert_PositionDetailCombKey_to_PositionDetailKey(*itkey,oppodetailkey);
                            PlatformStru_PositionDetail *poppodetail;
                            if(!PositionDetailData.GetValue_Internal(poppodetail,oppodetailkey)) continue;

                            assert(poppodetail->TradeType==THOST_FTDC_TRDT_CombinationDerived);

                            //����(����ɢ����ͨ)�ֲ���ϸ��key
                            PositionDetailKey DerivedKey(oppodetailkey);
                            DerivedKey.TradeType=THOST_FTDC_TRDT_Common;

                            //pExistCommonPositionDetail����������ͨ�ĳֲ���ϸ, �������״α���ɢ������, Ҳ�����Ƕ�α���ɢ������
                            PlatformStru_PositionDetail* pExistCommonPositionDetail;
                            bool bExistCommonPositionDetail=PositionDetailData.GetValue_Internal(pExistCommonPositionDetail,DerivedKey);
                            if(!bExistCommonPositionDetail)
                            {
                                //��OppoPositionDetail�����յ�NewPositionDetail
                                PlatformStru_PositionDetail NewPositionDetail=*poppodetail;

                                NewPositionDetail.Volume=0;
                                NewPositionDetail.TradeType=THOST_FTDC_TRDT_Common;
                                memset(NewPositionDetail.CombInstrumentID,0,sizeof(NewPositionDetail.CombInstrumentID));
                                NewPositionDetail.CloseProfitByDate=0;
                                NewPositionDetail.CloseProfitByTrade=0;
                                NewPositionDetail.PositionProfitByDate=0;
                                NewPositionDetail.PositionProfitByTrade=0;
                                NewPositionDetail.Margin=0;
                                NewPositionDetail.MarginRateByMoney=util::GetDoubleInvalidValue();
                                NewPositionDetail.MarginRateByVolume=util::GetDoubleInvalidValue();
                                NewPositionDetail.ExchMargin=0;
                                NewPositionDetail.CloseVolume=0;
                                NewPositionDetail.CloseAmount=0;

                                PositionDetailData.NewReach_Internal(NewPositionDetail);

                                PositionDetailData.GetValue_Internal(pExistCommonPositionDetail,DerivedKey);
                            }

		                    //��¼����ǰ�ĳֲ���ϸ����
                            if(ModifiedPositionDetails.find(oppodetailkey)==ModifiedPositionDetails.end())
                                ModifiedPositionDetails[oppodetailkey]=*poppodetail;
                            if(ModifiedPositionDetails.find(DerivedKey)==ModifiedPositionDetails.end())
                                ModifiedPositionDetails[DerivedKey]=*pExistCommonPositionDetail;

                            //���³ֲ���
                            int DeltaVolume=poppodetail->Volume;
                            if(DeltaVolume>thisVolume2) DeltaVolume=thisVolume2;

                            thisVolume2-=DeltaVolume;
                            poppodetail->Volume-=DeltaVolume;
                            pExistCommonPositionDetail->Volume+=DeltaVolume;

                            //ά����ƽ������ϳɽ�ID�Ͷ�Ӧƽ�����Ĺ�ϵ
                            map<string,int>::iterator itComTradeID2CloseVolume=ComTradeID2CloseVolume.find(*itComTradeID);
                            if(itComTradeID2CloseVolume==ComTradeID2CloseVolume.end())
                                ComTradeID2CloseVolume.insert(pair<string,int>(*itComTradeID,DeltaVolume));
                            else itComTradeID2CloseVolume->second+=DeltaVolume;

                            //�������к�
                            poppodetail->UpdateSeq=m_PositionDetailData.NewUpdateSeq(poppodetail->FTID);
                            pExistCommonPositionDetail->UpdateSeq=m_PositionDetailData.NewUpdateSeq(pExistCommonPositionDetail->FTID);
                        }
                    }
                }
	        }
        }
    }
}



//����б仯�ĳֲ���ϸ, ������ϳֲ���ϸ�ı�֤���Ż�. ��Ҫ�ڸ��³ֲ���ϸ�͸�����ϳֲ���ϸ�����. ���ݱ�֤���Ż����ý��м���, ������Ӧ�ֲ���ϸ����ϳֲ���ϸ�ı�֤���ֶ�, (Ϊ��֤��©��, ���ȶ���, �е��ظ�)
//�ֲ���ϸ�������Ѿ���δ�Żݵı�֤����
void CPlatformDataMgr_PositionDetails::CalcuPositionDetailMarginDiscount_RtnTrade_RspQryRate_OnSettlement(map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                                                                                          map<PositionDetailCombKey,PlatformStru_PositionDetailComb>& ModifiedPositionDetailCombs)
{
    CeasymutexGuard guard(m_mutex);

    //����Ҫ�����PositionDetailKey�ŵ�keys��
    set<PositionDetailKey> keys;
    for(map<PositionDetailKey,PlatformStru_PositionDetail>::const_iterator it=ModifiedPositionDetails.begin();it!=ModifiedPositionDetails.end();it++)
    {
        //������������ֲ���ϸ���д���.
        if(it->first.TradeType==THOST_FTDC_TRDT_CombinationDerived)
        {
            keys.insert(it->first);
        }
    }

    //ȡ���гֲ���ϸkey��Ӧ��ComTradeID
    set<string> ComTradeIDs;
    m_RefPositionDetailCombs.GetComTradeIDsByPositionDetailKeys(keys,ComTradeIDs,true);

    //��ÿһ��ComTradeID���б�֤���Żݼ���
    for(set<string>::const_iterator it=ComTradeIDs.begin();it!=ComTradeIDs.end();it++)
    {
        set<PositionDetailCombKey> leg1keys;
        set<PositionDetailCombKey> leg2keys;
        m_RefPositionDetailCombs.GetCombKeysByComTradeID2(*it,leg1keys,leg2keys,true);

        if(leg1keys.empty()||leg2keys.empty()) continue;

        string leg1InstrumentID(leg1keys.begin()->InstrumentID);
        string leg2InstrumentID(leg2keys.begin()->InstrumentID);

        string strExchangeID;
        if(!m_RefInstruments.GetExchangeID(leg1InstrumentID,strExchangeID)) continue;

        int Discount=0;
        if(strExchangeID==string("CZCE")) Discount=m_DiscountCZCE_Comb;
        else if(strExchangeID==string("DCE")) Discount=m_DiscountDCE_Comb;

        int discountLegID=-1;
        if(Discount==1||Discount==2)
        {
            //��һ���Żݻ�ڶ����Ż�
            discountLegID = Discount==1 ? 0 : 1;
        }
        if(Discount==3)
        {
            //С���Ż�
            double leg1ExchMargin=0,leg2ExchMargin=0;
            for(set<PositionDetailCombKey>::const_iterator it2=leg1keys.begin();it2!=leg1keys.end();it2++)
            {
                double exchMargin=m_RefPositionDetailCombs.GetExchMargin(*it2);
                if(util::isInvalidValue(exchMargin))
                {
                    leg1ExchMargin=util::GetDoubleInvalidValue();
                    break;
                }
                else leg1ExchMargin+=exchMargin;
            }
            for(set<PositionDetailCombKey>::const_iterator it2=leg2keys.begin();it2!=leg2keys.end();it2++)
            {
                double exchMargin=m_RefPositionDetailCombs.GetExchMargin(*it2);
                if(util::isInvalidValue(exchMargin))
                {
                    leg2ExchMargin=util::GetDoubleInvalidValue();
                    break;
                }
                else leg2ExchMargin+=exchMargin;
            }

            if(util::isInvalidValue(leg1ExchMargin)||util::isInvalidValue(leg2ExchMargin)) continue;

            if(leg1ExchMargin<leg2ExchMargin) discountLegID=0;
            else if(leg1ExchMargin>leg2ExchMargin) discountLegID=1;
            else
            {
                //�������, �Żݵ��ڶ���
                discountLegID = 1;
            }
        }

        const set<PositionDetailCombKey>& tmpcombkeys=discountLegID==0?leg1keys:leg2keys;
        PlatformStru_PositionDetail *pdetail;
        for(set<PositionDetailCombKey>::const_iterator it2=tmpcombkeys.begin();it2!=tmpcombkeys.end();it2++)
        {
            double Margin,ExchMargin;
            m_RefPositionDetailCombs.GetMargin(*it2,Margin,ExchMargin);
            assert(!util::isInvalidValue(Margin)&&!util::isInvalidValue(ExchMargin));

            PositionDetailKey detailkey;
            Convert_PositionDetailCombKey_to_PositionDetailKey(*it2,detailkey);
            if(m_PositionDetailData.GetValue_Internal(pdetail,detailkey))
            {
                if(ModifiedPositionDetails.find(detailkey)==ModifiedPositionDetails.end())
                    ModifiedPositionDetails[detailkey]=*pdetail;

                pdetail->Margin-=Margin;
                pdetail->ExchMargin-=ExchMargin;
                pdetail->UpdateSeq=m_PositionDetailData.NewUpdateSeq(pdetail->FTID);
            }

            if(ModifiedPositionDetailCombs.find(*it2)==ModifiedPositionDetailCombs.end())
            {
                PlatformStru_PositionDetailComb detailcomb;
                if(m_RefPositionDetailCombs.GetPositionDetailComb(*it2,detailcomb))
                    ModifiedPositionDetailCombs[*it2]=detailcomb;
            }

            m_RefPositionDetailCombs.SetMargin(*it2,0,0);
        }
    }
}

//����б仯�ĳֲ���ϸ������֤���������õ���ϳֲ���ϸ�С���ʱδ�����Ż�
//��ÿ���ֲ���ϸkey���Ҷ�Ӧ����ϳֲ���ϸkey�Ͷ�����ϳֲ���ϸkey���ٸ�����Щ��ϳֲ���ϸkey������صĳֲ���ϸkey
void CPlatformDataMgr_PositionDetails::SetMarginOfPositionDetailComb(map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails)
{
    CeasymutexGuard guard(m_mutex);

    PositionDetailKey tmpdetailkey;
    set<PositionDetailKey> refdetailkeys;
    for(map<PositionDetailKey,PlatformStru_PositionDetail>::const_iterator it=ModifiedPositionDetails.begin();it!=ModifiedPositionDetails.end();it++)
    {
        map<string,pair<set<PositionDetailCombKey>,set<PositionDetailCombKey>>> combkeys;
        m_RefPositionDetailCombs.GetCombKeysByPositionDetailKey2(it->first,combkeys,true);
        for(map<string,pair<set<PositionDetailCombKey>,set<PositionDetailCombKey>>>::const_iterator it_combkeys=combkeys.begin();it_combkeys!=combkeys.end();it_combkeys++)
        {
            for(set<PositionDetailCombKey>::const_iterator it_combkey2=it_combkeys->second.first.begin();it_combkey2!=it_combkeys->second.first.end();it_combkey2++)
            {
                Convert_PositionDetailCombKey_to_PositionDetailKey(*it_combkey2,tmpdetailkey);
                if(refdetailkeys.find(tmpdetailkey)==refdetailkeys.end())
                    refdetailkeys.insert(tmpdetailkey);
            }
            for(set<PositionDetailCombKey>::const_iterator it_combkey2=it_combkeys->second.second.begin();it_combkey2!=it_combkeys->second.second.end();it_combkey2++)
            {
                Convert_PositionDetailCombKey_to_PositionDetailKey(*it_combkey2,tmpdetailkey);
                if(refdetailkeys.find(tmpdetailkey)==refdetailkeys.end())
                    refdetailkeys.insert(tmpdetailkey);
            }
        }
    }

    PlatformStru_PositionDetail* pdetail;
    for(set<PositionDetailKey>::const_iterator it=refdetailkeys.begin();it!=refdetailkeys.end();it++)
    {
        if(m_PositionDetailData.GetValue_Internal(pdetail,*it))
        {
            m_RefPositionDetailCombs.SetMarginByPositionDetail(*pdetail);
        }
    }
}

//���³ֲ���ϸ��¼�ı�֤��, ��֤����, ��������֤��, �ɽ��޸ĳֲ���ϸ�Ͳ�ѯ��֤������Ӧʱ����
//����н����, ������ۼ���; ����, �����̼ۻ������ۼ���(������ϳֲ���ϸ�����Լ�����������)
//����ϳֲ���ϸ, ���ﲻ���Ǳ�֤���Ż�
void CPlatformDataMgr_PositionDetails::CalculateMargin_Internal(const PlatformStru_InstrumentMarginRate& MarginRate,
                                                                const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                                PlatformStru_PositionDetail& PositionDetail,
                                                                bool& bUpdate)
{
    if(InstrumentInfo.ProductClass==THOST_FTDC_PC_Options||InstrumentInfo.ProductClass==THOST_FTDC_PC_SpotOption)
    {
        //��Ȩ��Լ�����д���
        CalculateMargin_Option_Internal(MarginRate,InstrumentInfo,PositionDetail,bUpdate);
        return;
    }

    bUpdate=false;

    string strInstrument(PositionDetail.InstrumentID);
    if(strInstrument.empty())
        return;

    //CFileOpr::getObj().writelocallog("���㱣֤��","Start:\tInstrumentID:%s,TradeID:%s,OpenDate:%s,OpenPrice:%g",PositionDetail.InstrumentID,PositionDetail.TradeID,PositionDetail.OpenDate,PositionDetail.OpenPrice);

    double Margin=PositionDetail.Margin;
    double MarginRateByMoney=PositionDetail.MarginRateByMoney;
    double MarginRateByVolume=PositionDetail.MarginRateByVolume;
    double ExchMargin=PositionDetail.ExchMargin;

    //����н����, ������ۼ���; ����, �����ּۻ������ۼ���(������ϳֲ���ϸ�����Լ�����������)
    //������ϳֲ���ϸ, ��Ҫ��������, ����ʹ�ÿ��ּۻ���������
    double dPrice;
    if(m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible&&!util::isInvalidValue(PositionDetail.SettlementPrice)) 
    {
        dPrice=PositionDetail.SettlementPrice;
        //CFileOpr::getObj().writelocallog("���㱣֤��","1.1:\t�ý����");
    }
    else if(!PositionDetail.bToday) 
    {
        dPrice=PositionDetail.LastSettlementPrice;
        //CFileOpr::getObj().writelocallog("���㱣֤��","1.2:\t��������");
    }
    else if(PositionDetail.TradeType==THOST_FTDC_TRDT_CombinationDerived&&
        (strncmp(PositionDetail.ExchangeID,"CZCE",sizeof(PositionDetail.ExchangeID)-1)==0&&m_PriceForMarginOfTodayPositionCZCE_Comb==1||
         strncmp(PositionDetail.ExchangeID, "DCE",sizeof(PositionDetail.ExchangeID)-1)==0&&m_PriceForMarginOfTodayPositionDCE_Comb==1))
    {
        dPrice=PositionDetail.LastSettlementPrice;
        //CFileOpr::getObj().writelocallog("���㱣֤��","1.3:\t��������");
    }
    else 
    {
        dPrice=PositionDetail.OpenPrice;
        //CFileOpr::getObj().writelocallog("���㱣֤��","1.4:\t�ÿ��ּ�");
    }

    //CFileOpr::getObj().writelocallog("���㱣֤��","2:\tdPrice=%g",dPrice);

    //���ﲻ������ϵ��ֲ���ϸ��֤���Ż�
    if(PositionDetail.Volume==0)
    {
        Margin=0;
        ExchMargin=0;
        MarginRateByMoney=MarginRate.LongMarginRatioByMoney;
        MarginRateByVolume=MarginRate.LongMarginRatioByVolume;
    }
    else if(PositionDetail.Direction==THOST_FTDC_D_Buy)
    {
        if(!util::isInvalidValue(dPrice)&&!util::isInvalidValue(MarginRate.LongMarginRatioByMoney)&&!util::isInvalidValue(MarginRate.LongMarginRatioByVolume))
            Margin= PositionDetail.Volume*InstrumentInfo.VolumeMultiple*dPrice*MarginRate.LongMarginRatioByMoney+
                    PositionDetail.Volume*MarginRate.LongMarginRatioByVolume;
        else Margin=util::GetDoubleInvalidValue();

        if(!util::isInvalidValue(dPrice)&&!util::isInvalidValue(InstrumentInfo.LongMarginRatio))
            ExchMargin= PositionDetail.Volume*InstrumentInfo.VolumeMultiple*dPrice*InstrumentInfo.LongMarginRatio;
        else ExchMargin=util::GetDoubleInvalidValue();

        MarginRateByMoney=MarginRate.LongMarginRatioByMoney;
        MarginRateByVolume=MarginRate.LongMarginRatioByVolume;
    }
    else
    {
        if(!util::isInvalidValue(dPrice)&&!util::isInvalidValue(MarginRate.ShortMarginRatioByMoney)&&!util::isInvalidValue(MarginRate.ShortMarginRatioByVolume))
            Margin= PositionDetail.Volume*InstrumentInfo.VolumeMultiple*dPrice*MarginRate.ShortMarginRatioByMoney+
                    PositionDetail.Volume*MarginRate.ShortMarginRatioByVolume;
        else Margin=util::GetDoubleInvalidValue();

        if(!util::isInvalidValue(dPrice)&&!util::isInvalidValue(InstrumentInfo.ShortMarginRatio))
            ExchMargin= PositionDetail.Volume*InstrumentInfo.VolumeMultiple*dPrice*InstrumentInfo.ShortMarginRatio;
        else ExchMargin=util::GetDoubleInvalidValue();

        MarginRateByMoney=MarginRate.ShortMarginRatioByMoney;
        MarginRateByVolume=MarginRate.ShortMarginRatioByVolume;
    }
    //CFileOpr::getObj().writelocallog("���㱣֤��","3:\tMargin=%g/%g",Margin,ExchMargin);

    if( PositionDetail.Margin!=Margin){ PositionDetail.Margin=Margin; bUpdate=true; }
    if( PositionDetail.MarginRateByMoney!=MarginRateByMoney){ PositionDetail.MarginRateByMoney=MarginRateByMoney; bUpdate=true; }
    if( PositionDetail.MarginRateByVolume!=MarginRateByVolume){ PositionDetail.MarginRateByVolume=MarginRateByVolume; bUpdate=true; }
    if( PositionDetail.ExchMargin!=ExchMargin){ PositionDetail.ExchMargin=ExchMargin; bUpdate=true; }

    //���ݳֲ���ϸ�ı�֤�����ݣ�������ϳֲ���ϸ�ı�֤�𣬲������Żݡ��ڽ����Żݼ���ǰ��һ�¡�
    m_RefPositionDetailCombs.SetMarginByPositionDetail(PositionDetail);
}

//������Ȩ��Լ�ֲ���ϸ��¼�ı�֤��, ��������֤��, �ɽ��޸ĳֲ���ϸ�Ͳ�ѯ��֤������Ӧʱ����
//  ÿ�ֱ�֤��=MAX(Ȩ���� + Ͷ������Ȩ��Լ��֤�𲻱䲿��, Ͷ������Ȩ��Լ��С��֤��)
//  ���Ȩ���� = if(��������[��Ȩ��Լ���̼������۵Ľϴ�ֵ]��[��Ȩ��Լ�����]) �� ��Ȩ��Լ����
//  �²�Ȩ���� = �۸�������/�ɽ��ۣ���OptionRoyaltyPriceType����*��Ȩ��Լ����
void CPlatformDataMgr_PositionDetails::CalculateMargin_Option_Internal(const PlatformStru_InstrumentMarginRate& MarginRate,
                                                                const PlatformStru_InstrumentInfo& InstrumentInfo,
                                                                PlatformStru_PositionDetail& PositionDetail,
                                                                bool& bUpdate)
{
    bUpdate=false;

    string strInstrument(PositionDetail.InstrumentID);
    if(strInstrument.empty())
        return;

    CFileOpr::getObj().writelocallog("������Ȩ��֤��","Start:\tInstrumentID:%s,TradeID:%s,OpenDate:%s,OpenPrice:%g",PositionDetail.InstrumentID,PositionDetail.TradeID,PositionDetail.OpenDate,PositionDetail.OpenPrice);

    double Margin=PositionDetail.Margin;
    double ExchMargin=PositionDetail.ExchMargin;

    if(PositionDetail.Direction==THOST_FTDC_D_Sell)
    {
        //��Ȩ�򷽲��б�֤��

        //�ж��ǽ�ֻ�����֡�����Ѿ����㣬���Ҳ����ִ���
        bool bToday=PositionDetail.bToday;

        //ȡ����
        PlatformStru_DepthMarketData    QuotData;
        if(!m_RefQuots.GetQuotInfo(string(InstrumentInfo.InstrumentID),QuotData))
            QuotData.clear();

        //CFileOpr::getObj().writelocallog("���㱣֤��","1.1 ȡ����:\t�ý���� SettlementPrice=%g PreSettlementPrice=%g PreClosePrice=%g",QuotData.SettlementPrice,QuotData.PreSettlementPrice,QuotData.PreClosePrice);

        //ȡ����Ȩ����ʹ�õļ۸�
        double dPrice;
        if(bToday)
        {
            //���
            if(m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible&&!util::isInvalidValue(QuotData.SettlementPrice)) 
            {
                //�Ѿ����㣬������ִ���
                if(strncmp(PositionDetail.ExchangeID, "SHFE",sizeof(PositionDetail.ExchangeID)-1)!=0)
                    dPrice = QuotData.SettlementPrice;
                else dPrice = QuotData.SettlementPrice >= QuotData.ClosePrice ? QuotData.SettlementPrice : QuotData.ClosePrice;
                //CFileOpr::getObj().writelocallog("���㱣֤��","1.2.2 ����ѽ���:\tdPrice=%g",dPrice);
            }
            else 
            {
                //δ����
                dPrice = m_PriceForRoyalty_Option==1 ? QuotData.PreSettlementPrice : PositionDetail.OpenPrice;
                //CFileOpr::getObj().writelocallog("���㱣֤��","1.2.1 ���δ����:\tdPrice=%g",dPrice);
            }
        }
        else
        {
            //���
            if(strncmp(PositionDetail.ExchangeID, "SHFE",sizeof(PositionDetail.ExchangeID)-1)!=0)
                dPrice = QuotData.PreSettlementPrice;
            else dPrice = QuotData.PreSettlementPrice >= QuotData.PreClosePrice ? QuotData.PreSettlementPrice : QuotData.PreClosePrice;
            //CFileOpr::getObj().writelocallog("���㱣֤��","1.2.3 ���:\tdPrice=%g",dPrice);
        }

        //����Ȩ����
        double Royalty = util::isInvalidValue(dPrice)? util::GetDoubleInvalidValue() : dPrice*InstrumentInfo.VolumeMultiple;
        //CFileOpr::getObj().writelocallog("���㱣֤��","1.3 ����Ȩ����:\tRoyalty=%g",Royalty);

        //���㱣֤��
        if(util::isInvalidValue(Royalty)||util::isInvalidValue(MarginRate.FixedMargin)||util::isInvalidValue(MarginRate.MiniMargin))
            Margin = util::GetDoubleInvalidValue();
        else Margin = (util::greaterOrEqual(Royalty+MarginRate.FixedMargin,MarginRate.MiniMargin) ? (Royalty+MarginRate.FixedMargin) : MarginRate.MiniMargin)*PositionDetail.Volume;

        if(util::isInvalidValue(Royalty)||util::isInvalidValue(MarginRate.ExchFixedMargin)||util::isInvalidValue(MarginRate.ExchMiniMargin))
            ExchMargin = util::GetDoubleInvalidValue();
        else ExchMargin = (util::greaterOrEqual(Royalty+MarginRate.ExchFixedMargin,MarginRate.ExchMiniMargin) ? (Royalty+MarginRate.ExchFixedMargin) : MarginRate.ExchMiniMargin)*PositionDetail.Volume;
        //CFileOpr::getObj().writelocallog("���㱣֤��","1.4 ���㱣֤��:\tMargin=%g ExchMargin=%g",Margin,ExchMargin);
    }
    else
    {
        //��Ȩ��û�б�֤��
        Margin=0.0;
        ExchMargin=0.0;

        //CFileOpr::getObj().writelocallog("���㱣֤��","2.1:\t��Ȩ����Ϊ0");
    }

    if( PositionDetail.Margin!=Margin){ PositionDetail.Margin=Margin; bUpdate=true; }
    if( PositionDetail.ExchMargin!=ExchMargin){ PositionDetail.ExchMargin=ExchMargin; bUpdate=true; }
}

//ƽ�ֳɽ�ʱ�����ݳֲ���ϸ�ı仯��ȡ�ֲ���ϸ��λ�仯ֵ
void CPlatformDataMgr_PositionDetails::GetCloseDeltaVolume(const map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,map<PositionDetailKey,int>& mapCloseDeltaVolume)
{
    CeasymutexGuard guard(m_mutex);

    PlatformStru_PositionDetail* pdetail;
    mapCloseDeltaVolume.clear();
    for(map<PositionDetailKey,PlatformStru_PositionDetail>::const_iterator it=ModifiedPositionDetails.begin();it!=ModifiedPositionDetails.end();it++)
    {
        if(m_PositionDetailData.GetValue_Internal(pdetail,it->first))
        {
            mapCloseDeltaVolume[it->first]=it->second.Volume-pdetail->Volume;
        }
    }
}

//���ֳɽ�ʱ����ѯ��ϳֲ���ϸ���ʱ������Ϻ�ԼID���õ���Ӧ�ĳֲ���ϸ�С�ע�⣬����һ���ֲ���ϸ���Ӧ�����ϳֲ���ϸ����[ | ]������һ��
void CPlatformDataMgr_PositionDetails::SetCombInstrumentIDs(const vector<PositionDetailCombKey>& combdetailkeys)
{
    CeasymutexGuard guard(m_mutex);

    string CombInstrumentID;
    PositionDetailKey key;
	map<PositionDetailKey,set<string>> key2CombInstrumentID;        //detail key - set of CombInstrumentID
    for(int i=0;i<(int)combdetailkeys.size();i++)
    {
        if(m_RefPositionDetailCombs.GetCombInstrumentID(combdetailkeys[i],CombInstrumentID))
        {
            Convert_PositionDetailCombKey_to_PositionDetailKey(combdetailkeys[i],key);
            map<PositionDetailKey,set<string>>::iterator it=key2CombInstrumentID.find(key);
            if(it==key2CombInstrumentID.end())
            {
                set<string> combinsts;
                combinsts.insert(CombInstrumentID);
                key2CombInstrumentID[key]=combinsts;
            }
            else 
            {
                set<string>& combinsts=it->second;
                if(combinsts.find(CombInstrumentID)==combinsts.end())
                    combinsts.insert(CombInstrumentID);
            }
        }
    }

    PlatformStru_PositionDetail *pdetail;
    for(map<PositionDetailKey,set<string>>::const_iterator it=key2CombInstrumentID.begin();it!=key2CombInstrumentID.end();it++)
    {
        if(m_PositionDetailData.GetValue_Internal(pdetail,it->first))
        {
            string combinst;
            const set<string>& combinsts=it->second;
            assert((int)combinsts.size()>=1);
            set<string>::const_iterator it2=combinsts.begin();
            combinst=*it2;
            for(it2++;it2!=combinsts.end();it2++)
            {
                combinst += string(" | ") + *it2;
            }

            memset(pdetail->CombInstrumentID,0,sizeof(pdetail->CombInstrumentID));
            strncpy(pdetail->CombInstrumentID,combinst.c_str(),sizeof(pdetail->CombInstrumentID)-1);
            pdetail->UpdateSeq=m_PositionDetailData.NewUpdateSeq(pdetail->FTID);
        }
    }
}

//���ֳɽ�ʱ����Ϻ�ԼID���õ���Ӧ�ĳֲ���ϸ��. ������ϳֲ���ϸ�ı仯
void CPlatformDataMgr_PositionDetails::SetCombInstrumentID_OpenTrade(const map<PositionDetailCombKey,PlatformStru_PositionDetailComb> ModifiedPositionDetailCombs)
{
    vector<PositionDetailCombKey> combkeys;
    for(map<PositionDetailCombKey,PlatformStru_PositionDetailComb>::const_iterator it=ModifiedPositionDetailCombs.begin();it!=ModifiedPositionDetailCombs.end();it++)
    {
        combkeys.push_back(it->first);
    }
    SetCombInstrumentIDs(combkeys);
}


//�жϳֲ���ϸ�ǲ���(ƽ��)�ɽ��Ķ�Ӧ�ֲ���ϸ. �����Ƿ����෴, ������ͬ
bool CPlatformDataMgr_PositionDetails::IsPositionDetailForCloseTrade_IgnoreTradeType(const PlatformStru_PositionDetail& PositionDetail,
                                                                                     const PlatformStru_TradeInfo& TradeInfo)
{
    if(PositionDetail.Direction==TradeInfo.Direction) return false;
    if(PositionDetail.HedgeFlag!=TradeInfo.HedgeFlag) return false;

    if(TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseToday&&!PositionDetail.bToday) return false;
    if(TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseYesterday&&PositionDetail.bToday) return false;
    return true;
}


//���³ֲ���ϸ��¼�ı�֤��, ��֤����, ��ѯ��֤������Ӧʱ����, ֻ�Ա�֤��Ϊ��Чֵ����Ŀ���д���. ���ڲ�ѯ�����ĳֲ���ϸ, ��֤���������Ѿ����ˣ���������. 
void CPlatformDataMgr_PositionDetails::UpdatePositionDetailByMarginRate_RspQryRate(const PlatformStru_InstrumentMarginRate& MarginRate,
                                                                                   const PlatformStru_InstrumentInfo& InstrumentInfo) 
{
    string strInstrument(MarginRate.InstrumentID);
    map<string,set<long>>::iterator it_Index2;
    set<long>::iterator itFTID;
    map<long,PlatformStru_PositionDetail*>::iterator it_Info2;
    map<PositionDetailKey,PlatformStru_PositionDetail> ModifiedPositionDetails;
    map<PositionDetailCombKey,PlatformStru_PositionDetailComb> ModifiedPositionDetailCombs;

    m_mutex.lock();

    it_Index2=m_PositionDetailData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_PositionDetailData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        //��Ժ�Լ��Ӧ�ĳֲ���ϸ���������㱣֤��
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            it_Info2=m_PositionDetailData.m_all2.find(*itFTID);
            if(it_Info2!=m_PositionDetailData.m_all2.end()&&it_Info2->second!=NULL&&
                util::isInvalidValue(it_Info2->second->Margin))
            {
                bool bUpdate;
                PlatformStru_PositionDetail oridetail=*(it_Info2->second);

                //����ֲ���ϸ��֤��
                CalculateMargin_Internal(MarginRate,InstrumentInfo,*(it_Info2->second),bUpdate);

                if(bUpdate) 
                {
                    it_Info2->second->UpdateSeq=m_PositionDetailData.NewUpdateSeq(it_Info2->second->FTID);
                    
                    PositionDetailKey key(oridetail);
                    if(ModifiedPositionDetails.find(key)!=ModifiedPositionDetails.end())
                        ModifiedPositionDetails[key]=oridetail;
                }
            }
        }
    }

    m_mutex.unlock();

    //����б仯�ĳֲ���ϸ, ������ϳֲ���ϸ�ı�֤���Ż�. ��Ҫ�ڸ��³ֲ���ϸ�͸�����ϳֲ���ϸ�����. 
    CalcuPositionDetailMarginDiscount_RtnTrade_RspQryRate_OnSettlement(ModifiedPositionDetails,ModifiedPositionDetailCombs);
}

//ȡ��Ч�ֲ���ϸ(Volume>0). ���ؼ�¼����, lastseq�������ݸ��µ����к�
int CPlatformDataMgr_PositionDetails::GetPositionDetails(vector<PlatformStru_PositionDetail>& outData,long& lastseq)
{
    outData.clear();

    CeasymutexGuard guard(m_mutex);

	map<PositionDetailKey,PlatformStru_PositionDetail*>::iterator it= m_PositionDetailData.m_all.begin();
	for( ; it != m_PositionDetailData.m_all.end(); it++)
	{
        if ( it->second!=NULL && it->second->Volume>0)
            outData.push_back(*(it->second));
	}
    lastseq=m_PositionDetailData.GetLastUpdateSeq();

    return outData.size();
}

//ȡȫ���ֲ���ϸ, ��������Ϊ��ļ�¼
void CPlatformDataMgr_PositionDetails::GetPositionDetails4(vector<PlatformStru_PositionDetail>& outData,long& lastseq)
{
    outData.clear();

    CeasymutexGuard guard(m_mutex);

	map<PositionDetailKey,PlatformStru_PositionDetail*>::iterator it= m_PositionDetailData.m_all.begin();
	for( ; it != m_PositionDetailData.m_all.end(); it++)
	{
        if ( it->second!=NULL)
            outData.push_back(*(it->second));
	}
    lastseq=m_PositionDetailData.GetLastUpdateSeq();
}


//��ȡָ����Լ��Ӧ����Ч�ֲ���ϸ��¼(Volume>0). ����0: �ɹ�, ����������(�����ǿ�����)�� 1: �ɹ�, ������û�и���, ����������
//���lastseq��ԭʼ�����е�updateseqһ��, ��ʾ����û�б仯, ����1, ������������; 
//���򷵻�0, setFTID��strInstrument��Ӧ��ȫ����Ч(��λ������)��¼��FTID, �����ж���û����Ҫɾ������; vecValue���ض�Ӧ����Ч��������
int CPlatformDataMgr_PositionDetails::GetPositionDetails3(const string& strInstrument,
                                                          set<long>& setFTID,
                                                          vector<PlatformStru_PositionDetail>& vecValue,
                                                          long& lastseq)
{
    setFTID.clear();
    vecValue.clear();

    CeasymutexGuard guard(m_mutex);

    if(lastseq==m_PositionDetailData.GetLastUpdateSeq())
    {
        return 1;
    }
    else
    {
        set<long>* pFTIDSet;
        if(m_PositionDetailData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
        {
            PlatformStru_PositionDetail *pdetail;
            set<long>::iterator itFTID;
            for(itFTID=pFTIDSet->begin();itFTID!=pFTIDSet->end();itFTID++)
            {
                if(m_PositionDetailData.GetValue2_Internal(pdetail,(*itFTID))&&
                    pdetail->Volume>0)
                {
                    vecValue.push_back(*pdetail);
                    setFTID.insert(pdetail->FTID);
                }
            }
        }

        lastseq=m_PositionDetailData.GetLastUpdateSeq();
        return 0;
    }
}

//����keyȡvalue
bool CPlatformDataMgr_PositionDetails::GetPositionDetail(const PositionDetailKey& key,PlatformStru_PositionDetail& value)
{
    CeasymutexGuard guard(m_mutex);

    return m_PositionDetailData.GetValue4_Internal(value,key);
}

//�Ƿ���ָ����Լ����Ч�ֲ���ϸ��¼
bool CPlatformDataMgr_PositionDetails::HavePositionDetail(const string& strInstrumentID)
{
    bool brlt=false;

    CeasymutexGuard guard(m_mutex);

    PlatformStru_PositionDetail *pdetail;
    map<string,set<long>>::const_iterator it_Index=m_PositionDetailData.m_allIndex2.find(strInstrumentID);
    if(it_Index!=m_PositionDetailData.m_allIndex2.end()&&!it_Index->second.empty())
    {
        for(set<long>::const_iterator it2=it_Index->second.begin();it2!=it_Index->second.end();it2++)
        {
            if(m_PositionDetailData.GetValue2_Internal(pdetail,*it2)&&pdetail->Volume>0)
            {
                brlt=true;
                break;
            }
        }
    }

    return brlt;
}

//��ղ�ѯ�ֲ���ϸ��map
void CPlatformDataMgr_PositionDetails::ClearMapQryRltPositionDetails(const string& strQryInstrument)
{
    CeasymutexGuard guard(m_mutex);

    m_PositionDetailData.WillQry_Internal(strQryInstrument);
}

//��ȡ��ѯ�ĺ�ԼID
void CPlatformDataMgr_PositionDetails::GetQryPositionDetailInstrument(string& outQryInstrument)
{
    CeasymutexGuard guard(m_mutex);

    m_PositionDetailData.GetQryInstrument(outQryInstrument);
}

//����ָ��[��Լ-�˺�-����-Ͷ��]�ֲ���ϸ�ı�֤��֮��
void CPlatformDataMgr_PositionDetails::GetMarginByInstrument(const string& strInstrumentID,
                                                               const string& strAccount,
                                                               char Direction,
                                                               char HedgeFlag,
                                                               double& outMargin,
                                                               double& outExchMargin)
{
    outMargin=0;
    outExchMargin=0;

    CeasymutexGuard guard(m_mutex);

    //�����гֲ���ϸ�м��㱣֤��
    map<string,set<long>>::iterator it_Index2=m_PositionDetailData.m_allIndex2.find(strInstrumentID);
    if(it_Index2!=m_PositionDetailData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            map<long,PlatformStru_PositionDetail*>::iterator it_Info2=m_PositionDetailData.m_all2.find(*itFTID);
            if(it_Info2==m_PositionDetailData.m_all2.end()) continue;
            if(it_Info2->second==NULL) continue;
		    if(string(it_Info2->second->Account)!=strAccount) continue;
            if(it_Info2->second->Direction!=Direction) continue;
            if(it_Info2->second->HedgeFlag!=HedgeFlag) continue;
            if(it_Info2->second->TradeType==THOST_FTDC_TRDT_CombinationDerived) continue;

            if(it_Info2->second->Volume>0)
            {
                if(!util::isInvalidValue(outMargin))
                {
                    if(!util::isInvalidValue(it_Info2->second->Margin)) outMargin+=it_Info2->second->Margin;
                    else outMargin=util::GetDoubleInvalidValue();
                }
                if(!util::isInvalidValue(outExchMargin))
                {
                    if(!util::isInvalidValue(it_Info2->second->ExchMargin)) outExchMargin+=it_Info2->second->ExchMargin;
                    else outExchMargin=util::GetDoubleInvalidValue();
                }
                if(util::isInvalidValue(outMargin)&&util::isInvalidValue(outExchMargin)) break;
            }
        }
    }
}


//�Ƿ�����ϳֲ���ϸ����(TradeTypeΪ�������������)
bool CPlatformDataMgr_PositionDetails::HaveCombPositionDetail(void)
{
    return m_bHaveComb;
}

//��ȡƽ��ʱ��ɢ�����ĳֲ���ϸ��ԼiD
void CPlatformDataMgr_PositionDetails::GetDerivedInstrumentID_OnCloseTrade(set<string>& InstrumentIDs)
{
    CeasymutexGuard guard(m_mutex);

    InstrumentIDs=m_DerivedInstrumentIDs_OnCloseTrade;
}

///��ȡ��ѯ����ĺ�Լ����
void CPlatformDataMgr_PositionDetails::GetInstrumentSetOfQryRlt(set<string>& outData)
{
    CeasymutexGuard guard(m_mutex);

    m_PositionDetailData.GetInstrumentSetOfQryRlt(outData);
}

void CPlatformDataMgr_PositionDetails::clear(void)
{
    CeasymutexGuard guard(m_mutex);

    m_PositionDetailData.Clear();
    m_mapOpenTime2Key.clear();
    m_bHaveComb=false;
}
bool CPlatformDataMgr_PositionDetails::empty(void)
{
    CeasymutexGuard guard(m_mutex);

    return m_PositionDetailData.empty();
}


//��m_mapOpenTime2Key��ȡ��ָ��ʱ�䷶Χ��PositionDetail
//ʱ����time_t����, LowerOpenTime<=���OpenTimeUTC<UpperOpenTime. ��ֲֳ���ϸ��OpenTimeUTCΪ����������ʱ; �����׼ȷ��
//�ֲ���ϸ��OpenTimeUTC=CTools_Ansi::Get_time_t_utc_from_strTime(OpenDate,OpenTime)
void CPlatformDataMgr_PositionDetails::GetPositionDetailsInOpenTimeScope(int LowerOpenTime,int UpperOpenTime,vector<PlatformStru_PositionDetail>& outPositionDetails)
{
    CeasymutexGuard guard(m_mutex);

    outPositionDetails.clear();
    vector<PositionDetailKey> keys;
    PlatformStru_PositionDetail *pdetail;
    GetKeysFrommapOpenTime2Key_Internal(LowerOpenTime,UpperOpenTime,keys);
    for(int i=0;i<(int)keys.size();i++)
      if(m_PositionDetailData.GetValue_Internal(pdetail,keys[i]))
          outPositionDetails.push_back(*pdetail);
}

//��m_mapOpenTime2Key��ȡ��ָ������ʱ�䷶Χ��key
void CPlatformDataMgr_PositionDetails::GetKeysFrommapOpenTime2Key_Internal(int LowerOpenTime,int UpperOpenTime,vector<PositionDetailKey>& outkeys)
{
	multimap<int,PositionDetailKey>::iterator it_upper,it_lower;
    it_lower=m_mapOpenTime2Key.lower_bound(LowerOpenTime);
    it_upper=m_mapOpenTime2Key.lower_bound(UpperOpenTime);
    while(it_lower!=m_mapOpenTime2Key.end()&&it_lower!=it_upper)
    {
        outkeys.push_back(it_lower->second);
	    it_lower++;
    }
}


//��ȡ���гֲ���ϸ��¼��FTID
void CPlatformDataMgr_PositionDetails::GetAllFTID(set<long>& outFTIDs)
{
    CeasymutexGuard guard(m_mutex);

    m_PositionDetailData.GetAllFTID_Internal(outFTIDs);
}

//������ϳֲ���ϸkey�����ɳֲ���ϸkey
void CPlatformDataMgr_PositionDetails::Convert_PositionDetailCombKey_to_PositionDetailKey(const PositionDetailCombKey& combkey,PositionDetailKey& detailkey)
{
    detailkey.set(combkey.Account,combkey.InstrumentID,combkey.OpenDate,combkey.TradeID,combkey.Direction,THOST_FTDC_TRDT_CombinationDerived,combkey.HedgeFlag);
}
