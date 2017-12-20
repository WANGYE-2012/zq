//ά���ײ�ֲ���ϸ��Ϣ

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

#if 1
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
        char logbuf[512],logbuf2[512];\
        logbuf[0]=1;  logbuf2[0]=1;\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CPlatformDataMgr-PositionDetail", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif


//��ѯ�ֲ���ϸ��¼�Ľ��������ע�⣬�����ǲ�ѯָ����Լ�ĳֲ���ϸ
void CPlatformDataMgr::QryPositionDetailReach(PlatformStru_PositionDetail& PositionDetailInfo,int ErrID,const bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr::QryPositionDetailReach:InstrumentID=%s",PositionDetailInfo.InstrumentID);

    std::set<string> setInstruments;
    std::set<string>::iterator it_Instrument;

    Lock_CPlatformDataMgr;

    //��Ҫ���˵�VolumeΪ��ĳֲ���ϸ
    if(PositionDetailInfo.Volume==0)
    {
        if(ErrID==0) ErrID=-1;
    }

    if(ErrID==0)
	    m_tempOriPositionDetail[PositionDetailInfo.TradeID] = PositionDetailInfo;

    if(ErrID==0)
    {
        //���ö�Ӧ��PriceTick
        PlatformStru_InstrumentInfo InstrumentInfo;
        if(GetInstrumentInfo(std::string(PositionDetailInfo.InstrumentID),InstrumentInfo)==0)
        {
            PositionDetailInfo.PriceTick=InstrumentInfo.PriceTick;
        }
        PositionDetailInfo.bToday=IsTodayPosition( PositionDetailInfo.OpenDate);
    }

    m_PositionDetailData.QryRltReach_Internal(PositionDetailInfo,ErrID,bIsLast);

    if(bIsLast)
    {
        m_PositionDetailData.GetInstrumentSetOfQryRlt(setInstruments);
    }

    Unlock_CPlatformDataMgr;

    if(bIsLast&&!setInstruments.empty())
    {
        PlatformStru_DepthMarketData QuotData;
        //��̬���³ֲ���ϸ�ĳֲ�ӯ���͸���ӯ��
        for(it_Instrument=setInstruments.begin();it_Instrument!=setInstruments.end();it_Instrument++)
        {
            PlatformStru_InstrumentInfo InstrumentInfo;
            if(GetInstrumentInfo(*it_Instrument,InstrumentInfo)==0&&
                GetQuotInfo(*it_Instrument,QuotData)==0)
            {
               UpdatePositionDetailByQuot(QuotData,InstrumentInfo,true);
            }
        }

        //����ȡ����
        for(it_Instrument=setInstruments.begin();it_Instrument!=setInstruments.end();it_Instrument++)
        {
			if ( m_PlatFormService )
			{
				m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(*it_Instrument);
				m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(*it_Instrument);
			}
        }
    }

    LOG_INFO("CPlatformDataMgr::QryPositionDetailReach-end");

}

//ȡ���ֲ���ϸ�еļ�¼����
int CPlatformDataMgr::GetRecordCountOfPositionDetail(void)
{
    int count;

    Lock_CPlatformDataMgr;

    count = m_PositionDetailData.m_all.size();

    Unlock_CPlatformDataMgr;

    return count;
}

//�ӳɽ���¼���³ֲ���ϸ
//����ƽ���ĳֲ���ϸ�ļ�¼�������ں�������
//ModifiedPositionDetails�����ܵ�Ӱ��ĳֲ���ϸ��¼��n*2+0��ԭ�������ݣ�n*2+1��������
void CPlatformDataMgr::UpdatePositionDetail_RtnTrade(PlatformStru_TradeInfo& TradeInfo,
                                                     std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                                     const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    LOG_INFO("CPlatformDataMgr::UpdatePositionDetail_RtnTrade(�ɽ��ر����³ֲ���ϸ):InstrumentID=%s",TradeInfo.InstrumentID);

    std::string strInstrument=std::string(TradeInfo.InstrumentID);
    if(strInstrument.empty())
        return;

    Lock_CPlatformDataMgr;


    if(TradeInfo.OffsetFlag==THOST_FTDC_OF_Open)
    {                                                   
		PlatformStru_PositionDetail PositionDetailInfo;

//#ifdef _USE_COMBOINSTRUMENT
	    //��ϵ�����ʱ��������ϳֲ���ϸ��¼
	    if (TradeInfo.TradeType == THOST_FTDC_TRDT_CombinationDerived)
	    {
		    CreatePositionDetailCombFromOpenTrade_Internal(TradeInfo);
	    }
//#endif
        CreatePositionDetailFromOpenTrade_Internal(PositionDetailInfo,TradeInfo);

        //��̬���³ֲ���ϸ�ĳֲ�ӯ���͸���ӯ��
	    PlatformStru_DepthMarketData QuotInfo;
	    if(GetQuotInfo(std::string(TradeInfo.InstrumentID),QuotInfo)==0)
            UpdatePositionDetailByQuot_Internal(QuotInfo,PositionDetailInfo,InstrumentInfo,true); 

        //��̬���³ֲ���ϸ��¼�ı�֤�𡢱�֤���ʣ��ɽ��޸ĳֲ���ϸ�Ͳ�ѯ��֤������Ӧʱ����
	    PlatformStru_InstrumentMarginRate MarginRate;
	    if(GetMarginRate_Internal(TradeInfo.InstrumentID,MarginRate)==0)
            UpdatePositionDetailByMarginRate_Internal(MarginRate,PositionDetailInfo,InstrumentInfo);
        else
        {
            LOG_INFO("CPlatformDataMgr::UpdatePositionDetail_RtnTrade (�������)ȡ��֤����ʧ�ܣ� InstrumentID=%s",TradeInfo.InstrumentID);
        }

        if(PositionDetailInfo.Volume>0)
        {
            //���ö�Ӧ��PriceTick
            PositionDetailInfo.PriceTick=InstrumentInfo.PriceTick;
            PositionDetailInfo.bToday=true;

            m_PositionDetailData.NewReach_Internal(PositionDetailInfo);
            LOG_INFO("CPlatformDataMgr::UpdatePositionDetail_RtnTrade(�������ɳֲ���ϸ):%s",PositionDetailInfo.tostring(logbuf,sizeof(logbuf)));

            //����Ӱ�쵽�ĳֲ���ϸ��¼
            ModifiedPositionDetails.push_back(PlatformStru_PositionDetail::GetInvalidRecord());
            ModifiedPositionDetails.push_back(PositionDetailInfo);
        }
    }
    else if( TradeInfo.OffsetFlag==THOST_FTDC_OF_Close||
        TradeInfo.OffsetFlag==THOST_FTDC_OF_ForceClose||
        TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseToday||
        TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseYesterday||
        TradeInfo.OffsetFlag==THOST_FTDC_OF_LocalForceClose)
    { 
        UpdatePositionDetailFromCloseTrade_Internal(TradeInfo,ModifiedPositionDetails,InstrumentInfo);
    }

	//��鱻�ı�ĳֲ���ϸ�������ȫ��ƽ����ɾ��֮
	int count=ModifiedPositionDetails.size();
	for(int i=0;i<count/2;i++)
	{
		if(ModifiedPositionDetails[i*2+1].Volume==0)
		{
			PositionDetailKey key(ModifiedPositionDetails[i*2+1]);
            LOG_INFO("CPlatformDataMgr::UpdatePositionDetail_RtnTrade(ɾ��ȫƽ���ĳֲ���ϸ):%s",key.tostring(logbuf,sizeof(logbuf)));
			m_PositionDetailData.DeleteRecord2_Internal(key);
			m_PositionDetailData.NewUpdateSeq();
		}
	}

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::UpdatePositionDetail_RtnTrade-end");
}

//�ӿ��ֳɽ���¼�����ֲ���ϸ��ӯ���ͱ�֤���ں������
void CPlatformDataMgr::CreatePositionDetailFromOpenTrade_Internal(PlatformStru_PositionDetail& PositionDetailInfo,PlatformStru_TradeInfo& TradeInfo)
{
    std::string strInstrument=std::string(TradeInfo.InstrumentID);
    if(strInstrument.empty())
        return;

    if(TradeInfo.OffsetFlag!=THOST_FTDC_OF_Open)
        return;

    PositionDetailInfo.Clear();

	strncpy(PositionDetailInfo.szAccount,TradeInfo.szAccount,sizeof(PositionDetailInfo.szAccount)-1);
    strncpy(PositionDetailInfo.InstrumentID,TradeInfo.InstrumentID,sizeof(PositionDetailInfo.InstrumentID)-1);
    strncpy(PositionDetailInfo.BrokerID,TradeInfo.BrokerID,sizeof(PositionDetailInfo.BrokerID)-1);
    strncpy(PositionDetailInfo.InvestorID,TradeInfo.InvestorID,sizeof(PositionDetailInfo.InvestorID)-1);
    PositionDetailInfo.HedgeFlag=TradeInfo.HedgeFlag;
    PositionDetailInfo.Direction=TradeInfo.Direction;
    strncpy(PositionDetailInfo.OpenDate,TradeInfo.TradeDate,sizeof(PositionDetailInfo.OpenDate)-1);
    strncpy(PositionDetailInfo.TradeID,TradeInfo.TradeID,sizeof(PositionDetailInfo.TradeID)-1);
    PositionDetailInfo.Volume=TradeInfo.Volume;
    PositionDetailInfo.OpenPrice=TradeInfo.Price;
    strncpy(PositionDetailInfo.TradingDay,TradeInfo.TradingDay,sizeof(PositionDetailInfo.TradingDay)-1);
    PositionDetailInfo.SettlementID=TradeInfo.SettlementID;
    PositionDetailInfo.TradeType=TradeInfo.TradeType;
    strncpy(PositionDetailInfo.ExchangeID,TradeInfo.ExchangeID,sizeof(PositionDetailInfo.ExchangeID)-1);
    PositionDetailInfo.CloseProfitByDate=0;
    PositionDetailInfo.CloseProfitByTrade=0;
    PositionDetailInfo.PositionProfitByDate=0;
    PositionDetailInfo.PositionProfitByTrade=0;

    PositionDetailInfo.Margin=util::GetDoubleInvalidValue();
    PositionDetailInfo.MarginRateByMoney=util::GetDoubleInvalidValue();
    PositionDetailInfo.MarginRateByVolume=util::GetDoubleInvalidValue();
    PositionDetailInfo.ExchMargin=util::GetDoubleInvalidValue();
    PositionDetailInfo.LastSettlementPrice=util::GetDoubleInvalidValue();
    PositionDetailInfo.SettlementPrice=util::GetDoubleInvalidValue();
    PositionDetailInfo.CloseVolume=0;
    PositionDetailInfo.CloseAmount=0;
}

//��ƽ�ֳɽ���¼���³ֲ���ϸ��ModifiedPositionDetails�����ܵ�Ӱ��ĳֲ���ϸ��¼��first��ԭ�������ݣ�second��������
void CPlatformDataMgr::UpdatePositionDetailFromCloseTrade_Internal(PlatformStru_TradeInfo& TradeInfo,
                                                                   std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                                                   const PlatformStru_InstrumentInfo& InstrumentInfo)
{
    bool bHaveMarginRateInfo=false;
    bool bHaveQuot=false;

    if( TradeInfo.OffsetFlag!=THOST_FTDC_OF_Close&&
        TradeInfo.OffsetFlag!=THOST_FTDC_OF_ForceClose&&
        TradeInfo.OffsetFlag!=THOST_FTDC_OF_CloseToday&&
        TradeInfo.OffsetFlag!=THOST_FTDC_OF_CloseYesterday&&
        TradeInfo.OffsetFlag!=THOST_FTDC_OF_LocalForceClose)
        return;

    std::string strInstrument=std::string(TradeInfo.InstrumentID);
    if(strInstrument.empty())
        return;

    int nVolumeMultiple=InstrumentInfo.VolumeMultiple;

	PlatformStru_InstrumentMarginRate MarginRate;
	if(GetMarginRate_Internal(strInstrument,MarginRate)==0)
        bHaveMarginRateInfo=true;

	PlatformStru_DepthMarketData QuotInfo;
	if(GetQuotInfo(strInstrument,QuotInfo)==0)
        bHaveQuot=true;


	//����ƽ��
    //����TradeInfo�ĺ�Լ����ƽ��־����������ƽ����Ӧ�Ĳ�λ���������е�������ӯ������֤�𡢽�����
    int CloseVolume=TradeInfo.Volume;


    //ѭ������ֱ��ƽ������������
	//��һ�飬�ȴ���һ�ֲ�

    //ע�⣺std::set<PositionDetailKey>���ǰ�ʱ�������
    std::map<std::string,std::set<PositionDetailKey>>::iterator it_Index;
    std::set<PositionDetailKey>::iterator itKey;
    std::map<PositionDetailKey,PlatformStru_PositionDetail*>::iterator it_PositionDetail;

    it_Index=m_PositionDetailData.m_allIndex.find(strInstrument);
    if(it_Index!=m_PositionDetailData.m_allIndex.end()&&!it_Index->second.empty())
    {
        //��һ����������ͨƽ�ֳɽ�������ͨ�ֲ���ϸ��ƽ��
        if(TradeInfo.TradeType!=THOST_FTDC_TRDT_CombinationDerived&&CloseVolume>0)
        {
            for(itKey=it_Index->second.begin() ; CloseVolume>0 && itKey!=it_Index->second.end() ; itKey++)
            {
		        it_PositionDetail=m_PositionDetailData.m_all.find(*itKey);
		        if(it_PositionDetail==m_PositionDetailData.m_all.end()) continue;
                if(it_PositionDetail->second==NULL) continue;

                PlatformStru_PositionDetail& PositionDetail = *(it_PositionDetail->second);
                if(!IsOpponentPositionDetailOfTrade_IgnoreTradeType(PositionDetail,TradeInfo)) continue;
                if(PositionDetail.TradeType==THOST_FTDC_TRDT_CombinationDerived) continue;

                PlatformStru_PositionDetail OldPositionDetail=PositionDetail;

                CloseVolume = CloseSinglePosition_Internal(CloseVolume, PositionDetail, nVolumeMultiple, TradeInfo.Price);
                LOG_INFO("CPlatformDataMgr::UpdatePositionDetailFromCloseTrade_Internal(��һ��ƽ��):Old:%s New:%s",OldPositionDetail.tostring(logbuf,sizeof(logbuf)),PositionDetail.tostring(logbuf2,sizeof(logbuf2)));

		        //���³ֲ���ϸ�ĳֲ�ӯ���͸���ӯ��
		        if(bHaveQuot) UpdatePositionDetailByQuot_Internal(QuotInfo,PositionDetail,InstrumentInfo,true); 
		        //���³ֲ���ϸ��¼�ı�֤�𡢱�֤����
		        if(bHaveMarginRateInfo) UpdatePositionDetailByMarginRate_Internal(MarginRate,PositionDetail,InstrumentInfo);

                //��¼����ǰ��ĳֲ���ϸ����
                ModifiedPositionDetails.push_back(OldPositionDetail);
                ModifiedPositionDetails.push_back(PositionDetail);
            }
        }

        //�ڶ�����������ͨƽ�ֳɽ�������ϳֲ���ϸ�д�ɢƽ��
        if(TradeInfo.TradeType!=THOST_FTDC_TRDT_CombinationDerived&&CloseVolume>0)
        {
            for(itKey=it_Index->second.begin() ; CloseVolume>0 && itKey!=it_Index->second.end() ; itKey++)
            {
		        it_PositionDetail=m_PositionDetailData.m_all.find(*itKey);
		        if(it_PositionDetail==m_PositionDetailData.m_all.end()) continue;
                if(it_PositionDetail->second==NULL) continue;

                PlatformStru_PositionDetail& PositionDetail = *(it_PositionDetail->second);
                if(!IsOpponentPositionDetailOfTrade_IgnoreTradeType(PositionDetail,TradeInfo)) continue;
                if(PositionDetail.TradeType!=THOST_FTDC_TRDT_CombinationDerived) continue;

                //��ȡ���ֲ�λ
                PlatformStru_PositionDetail& OppoPositionDetail=GetOpponentPositionDetailComb(PositionDetail);
                if(OppoPositionDetail.IsInvalidRecord()) continue;

                PlatformStru_PositionDetail OldPositionDetail=PositionDetail;

                CloseVolume = CloseSinglePosition_Internal(CloseVolume, PositionDetail, nVolumeMultiple, TradeInfo.Price);
                LOG_INFO("CPlatformDataMgr::UpdatePositionDetailFromCloseTrade_Internal(�ڶ���ƽ��):Old:%s New:%s",OldPositionDetail.tostring(logbuf,sizeof(logbuf)),PositionDetail.tostring(logbuf2,sizeof(logbuf2)));


		        //���³ֲ���ϸ�ĳֲ�ӯ���͸���ӯ��
		        if(bHaveQuot) UpdatePositionDetailByQuot_Internal(QuotInfo,PositionDetail,InstrumentInfo,true); 
		        //���³ֲ���ϸ��¼�ı�֤�𡢱�֤����
		        if(bHaveMarginRateInfo) UpdatePositionDetailByMarginRate_Internal(MarginRate,PositionDetail,InstrumentInfo);

                //��¼����ǰ��ĳֲ���ϸ����
                ModifiedPositionDetails.push_back(OldPositionDetail);
                ModifiedPositionDetails.push_back(PositionDetail);

                //��Ϊ�Ǵ�����ϣ�һ�ȱ�ƽ����һ��(���ֲ�λ)Ҫ��ɢ
                int DeltaVolume=OldPositionDetail.Volume - PositionDetail.Volume;
                BreakupPositionDetailComb(OppoPositionDetail,DeltaVolume,ModifiedPositionDetails);
            }
        }

        //��������������������ɽ�������ϳֲ���ϸ��ƽ��
        if(TradeInfo.TradeType==THOST_FTDC_TRDT_CombinationDerived&&CloseVolume>0)
        {
            for(itKey=it_Index->second.begin() ; CloseVolume>0 && itKey!=it_Index->second.end() ; itKey++)
            {
		        it_PositionDetail=m_PositionDetailData.m_all.find(*itKey);
		        if(it_PositionDetail==m_PositionDetailData.m_all.end()) continue;
                if(it_PositionDetail->second==NULL) continue;

                PlatformStru_PositionDetail& PositionDetail = *(it_PositionDetail->second);
                if(!IsOpponentPositionDetailOfTrade_IgnoreTradeType(PositionDetail,TradeInfo)) continue;
                if(PositionDetail.TradeType!=THOST_FTDC_TRDT_CombinationDerived) continue;

                PlatformStru_PositionDetail OldPositionDetail=PositionDetail;

                CloseVolume = CloseSinglePosition_Internal(CloseVolume, PositionDetail, nVolumeMultiple, TradeInfo.Price);
                LOG_INFO("CPlatformDataMgr::UpdatePositionDetailFromCloseTrade_Internal(������ƽ��):Old:%s New:%s",OldPositionDetail.tostring(logbuf,sizeof(logbuf)),PositionDetail.tostring(logbuf2,sizeof(logbuf2)));

		        //���³ֲ���ϸ�ĳֲ�ӯ���͸���ӯ��
		        if(bHaveQuot) UpdatePositionDetailByQuot_Internal(QuotInfo,PositionDetail,InstrumentInfo,true); 
		        //���³ֲ���ϸ��¼�ı�֤�𡢱�֤����
		        if(bHaveMarginRateInfo) UpdatePositionDetailByMarginRate_Internal(MarginRate,PositionDetail,InstrumentInfo);

                //��¼����ǰ��ĳֲ���ϸ����
                ModifiedPositionDetails.push_back(OldPositionDetail);
                ModifiedPositionDetails.push_back(PositionDetail);
            }
        }
    }

}

//��̬���³ֲ���ϸ��¼�ĳֲ�ӯ��������ӯ��������ۡ������ۡ��ּۣ��������飬�õ�������ʱ��һ��
//���������bForceCalculateΪfalse����ʾ���ڼ۸�䶯ʱ���£��������Ϊtrue
void CPlatformDataMgr::UpdatePositionDetailByQuot(PlatformStru_DepthMarketData& QuotData,const PlatformStru_InstrumentInfo& InstrumentInfo,bool bForceCalculate) 
{
    std::string strInstrument(QuotData.InstrumentID);
    std::map<std::string,std::set<long>>::iterator it_Index2;
    std::set<long>::iterator itFTID;
    std::map<long,PlatformStru_PositionDetail*>::iterator it_Info2;

    Lock_CPlatformDataMgr;

    it_Index2=m_PositionDetailData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_PositionDetailData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            it_Info2=m_PositionDetailData.m_all2.find(*itFTID);
            if(it_Info2!=m_PositionDetailData.m_all2.end()&&it_Info2->second!=NULL)
            {
                UpdatePositionDetailByQuot_Internal(QuotData,*(it_Info2->second),InstrumentInfo,bForceCalculate);
            }
        }
    }

    Unlock_CPlatformDataMgr;

}

//��̬���³ֲ���ϸ��¼�ĳֲ�ӯ��(����)������ӯ��(���)������ۡ������ۡ��ּۣ���������
//���������bForceCalculateΪfalse����ʾ���ڼ۸�䶯ʱ���£��������Ϊtrue
void CPlatformDataMgr::UpdatePositionDetailByQuot_Internal(PlatformStru_DepthMarketData& QuotData,
                                                           PlatformStru_PositionDetail& PositionDetail,
                                                           const PlatformStru_InstrumentInfo InstrumentInfo,
                                                           bool bForceCalculate) 
{
    bool bTodayPositionDetail;

    std::string strInstrument(QuotData.InstrumentID);

    double CurPriceUsedByCalcu=QuotData.Volume>0?QuotData.LastPrice:QuotData.PreSettlementPrice;

    if(CurPriceUsedByCalcu==util::GetDoubleInvalidValue())
        return;

    //��ǿ�Ƽ���ʱ����۸�δ���£������м���
    if(!bForceCalculate&&
        CurPriceUsedByCalcu==PositionDetail.CurPriceUsedByCalcu)
        return;

    if(strInstrument!=std::string(PositionDetail.InstrumentID))
        return;

    double PositionProfitByDate=PositionDetail.PositionProfitByDate;
    double PositionProfitByTrade=PositionDetail.PositionProfitByTrade;
    double LastSettlementPrice=PositionDetail.LastSettlementPrice;
    double SettlementPrice=PositionDetail.SettlementPrice;


    //����ֲ�ӯ��(����)
    bTodayPositionDetail=IsTodayPositionDetail(PositionDetail);
    if(bTodayPositionDetail)
    {
        if(PositionDetail.Direction==THOST_FTDC_D_Buy)
            PositionProfitByDate=(CurPriceUsedByCalcu-PositionDetail.OpenPrice)*PositionDetail.Volume*InstrumentInfo.VolumeMultiple;    //���ն��гֲ�ӯ��
        else
            PositionProfitByDate=-1*(CurPriceUsedByCalcu-PositionDetail.OpenPrice)*PositionDetail.Volume*InstrumentInfo.VolumeMultiple;    //���ն��гֲ�ӯ��
    }
    else 
    {
        if(PositionDetail.Direction==THOST_FTDC_D_Buy)
            PositionProfitByDate=(CurPriceUsedByCalcu-PositionDetail.LastSettlementPrice)*PositionDetail.Volume*InstrumentInfo.VolumeMultiple;   //���ն��гֲ�ӯ��
        else
            PositionProfitByDate=-1*(CurPriceUsedByCalcu-PositionDetail.LastSettlementPrice)*PositionDetail.Volume*InstrumentInfo.VolumeMultiple;   //���ն��гֲ�ӯ��
    }
    if(PositionProfitByDate==-0) PositionProfitByDate=0;


    //����ֲ�ӯ��(���������)
    if(PositionDetail.Direction==THOST_FTDC_D_Buy)
        PositionProfitByTrade=(CurPriceUsedByCalcu-PositionDetail.OpenPrice)*PositionDetail.Volume*InstrumentInfo.VolumeMultiple;    //���ʶԳ�ֲ�ӯ��
    else
        PositionProfitByTrade=-1*(CurPriceUsedByCalcu-PositionDetail.OpenPrice)*PositionDetail.Volume*InstrumentInfo.VolumeMultiple;    //���ʶԳ�ֲ�ӯ��
    if(PositionProfitByTrade==-0) PositionProfitByTrade=0;

    if(LastSettlementPrice==util::GetDoubleInvalidValue())
        LastSettlementPrice=QuotData.PreSettlementPrice;
    if(SettlementPrice==util::GetDoubleInvalidValue())
        SettlementPrice=QuotData.SettlementPrice;


    bool bUpdate=false;
    if( PositionProfitByDate!=PositionDetail.PositionProfitByDate){ PositionDetail.PositionProfitByDate=PositionProfitByDate; bUpdate=true; }
    if( PositionProfitByTrade!=PositionDetail.PositionProfitByTrade){ PositionDetail.PositionProfitByTrade=PositionProfitByTrade; bUpdate=true; }
    if( LastSettlementPrice!=PositionDetail.LastSettlementPrice){ PositionDetail.LastSettlementPrice=LastSettlementPrice; bUpdate=true; }
    if( SettlementPrice!=PositionDetail.SettlementPrice){ PositionDetail.SettlementPrice=SettlementPrice; bUpdate=true; }
    if( PositionDetail.CurPrice!=QuotData.LastPrice) { PositionDetail.CurPrice=QuotData.LastPrice;  bUpdate=true;}
    if( CurPriceUsedByCalcu!=PositionDetail.CurPriceUsedByCalcu) { PositionDetail.CurPriceUsedByCalcu=CurPriceUsedByCalcu; bUpdate=true; }
    if(bUpdate)
        PositionDetail.UpdateSeq=m_PositionDetailData.NewUpdateSeq();
}


//��̬���³ֲ���ϸ��¼�ı�֤�𡢱�֤���ʣ���ѯ��֤������Ӧʱ���ã�ֻ�Խ�ִ���
bool CPlatformDataMgr::UpdatePositionDetailByMarginRate_RspQryRate(PlatformStru_InstrumentMarginRate& MarginRate,
                                                        const PlatformStru_InstrumentInfo& InstrumentInfo) 
{
    bool brlt=false;
    std::string strInstrument(MarginRate.InstrumentID);
    std::map<std::string,std::set<long>>::iterator it_Index2;
    std::set<long>::iterator itFTID;
    std::map<long,PlatformStru_PositionDetail*>::iterator it_Info2;

    Lock_CPlatformDataMgr;

    it_Index2=m_PositionDetailData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_PositionDetailData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
            it_Info2=m_PositionDetailData.m_all2.find(*itFTID);
            if(it_Info2!=m_PositionDetailData.m_all2.end()&&it_Info2->second!=NULL)
            {
                UpdatePositionDetailByMarginRate_Internal(MarginRate,*(it_Info2->second),InstrumentInfo);
                if(!brlt) brlt=true;
            }
        }
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

//��̬���³ֲ���ϸ��¼�ı�֤�𡢱�֤���ʣ��ɽ��޸ĳֲ���ϸ�Ͳ�ѯ��֤������Ӧʱ���ã�ֻ�Խ�ִ���
void CPlatformDataMgr::UpdatePositionDetailByMarginRate_Internal(PlatformStru_InstrumentMarginRate& MarginRate,
                                                                 PlatformStru_PositionDetail& PositionDetail,
                                                                 const PlatformStru_InstrumentInfo& InstrumentInfo) 
{
    //�Ѿ�����ˣ�������
    //if(PositionDetail.Margin!=util::GetDoubleInvalidValue())
    //    return;

    std::string strInstrument(PositionDetail.InstrumentID);
    if(strInstrument.empty())
        return;

	double dPrice;
    if(!IsTodayPositionDetail(PositionDetail))
	{
        dPrice=PositionDetail.LastSettlementPrice;
	}
	else
	{
		dPrice=PositionDetail.OpenPrice;
	}

    double Margin=PositionDetail.Margin;
    double MarginRateByMoney=PositionDetail.MarginRateByMoney;
    double MarginRateByVolume=PositionDetail.MarginRateByVolume;

    if(PositionDetail.Direction==THOST_FTDC_D_Buy)
    {
        Margin=  PositionDetail.Volume*
                                InstrumentInfo.VolumeMultiple*
                                dPrice*
                                MarginRate.LongMarginRatioByMoney+
                                PositionDetail.Volume*
                                MarginRate.LongMarginRatioByVolume;
        MarginRateByMoney=MarginRate.LongMarginRatioByMoney;
        MarginRateByVolume=MarginRate.LongMarginRatioByVolume;
    }
    else
    {
        Margin=  PositionDetail.Volume*
                                InstrumentInfo.VolumeMultiple*
                                dPrice*
                                MarginRate.ShortMarginRatioByMoney+
                                PositionDetail.Volume*
                                MarginRate.ShortMarginRatioByVolume;
        MarginRateByMoney=MarginRate.ShortMarginRatioByMoney;
        MarginRateByVolume=MarginRate.ShortMarginRatioByVolume;
    }

    bool bUpdate=false;
    if( PositionDetail.Margin!=Margin){ PositionDetail.Margin=Margin; bUpdate=true; }
    if( PositionDetail.MarginRateByMoney!=MarginRateByMoney){ PositionDetail.MarginRateByMoney=MarginRateByMoney; bUpdate=true; }
    if( PositionDetail.MarginRateByVolume!=MarginRateByVolume){ PositionDetail.MarginRateByVolume=MarginRateByVolume; bUpdate=true; }
    if(bUpdate)
        PositionDetail.UpdateSeq=m_PositionDetailData.NewUpdateSeq();

}




int CPlatformDataMgr::GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq)
{
    outData.clear();

    Lock_CPlatformDataMgr;

	std::map<PositionDetailKey,PlatformStru_PositionDetail*>::iterator it= m_PositionDetailData.m_all.begin();
	for( ; it != m_PositionDetailData.m_all.end(); it++)
	{
        if ( it->second!=NULL &&
             it->second->Volume > 0)
		{
            outData.push_back(*(it->second));
		}
	}
    lastseq=m_PositionDetailData.GetUpdateSeq();

    Unlock_CPlatformDataMgr;

    return outData.size();
}



//����0���ɹ������������ݣ�1: �ɹ���������û�и��£����������ݣ�-1��ʧ��
int CPlatformDataMgr::GetPositionDetails3(const std::string& strInstrument,
                                          std::set<long>& setFTID,
                                          std::vector<PlatformStru_PositionDetail>& vecValue,
                                          long& lastseq)
{
    int rlt;

    setFTID.clear();
    vecValue.clear();

    Lock_CPlatformDataMgr;

    if(lastseq==m_PositionDetailData.GetUpdateSeq())
    {
        rlt=1;
    }
    else
    {
        std::set<long>* pFTIDSet;
        if(m_PositionDetailData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
        {
            setFTID=(*pFTIDSet);

            PlatformStru_PositionDetail *pData;
            std::set<long>::iterator itFTID;
            for(itFTID=setFTID.begin();itFTID!=setFTID.end();itFTID++)
            {
                if(m_PositionDetailData.GetValue2_Internal(pData,(*itFTID)))
                    vecValue.push_back(*pData);
            }
        }

        lastseq=m_PositionDetailData.GetUpdateSeq();
        rlt=0;
    }

    Unlock_CPlatformDataMgr;

    return rlt;
}

bool CPlatformDataMgr::HavePositionDetail(const std::string& strInstrumentID)
{
    bool brlt=false;
    std::map<std::string,std::set<PositionDetailKey>>::iterator it_Index;
    std::set<PositionDetailKey>::iterator itKey;

    Lock_CPlatformDataMgr;

    it_Index=m_PositionDetailData.m_allIndex.find(strInstrumentID);
    if(it_Index!=m_PositionDetailData.m_allIndex.end()&&!it_Index->second.empty())
        brlt=true;

    Unlock_CPlatformDataMgr;

    return brlt;
}

//��ղ�ѯ�ֲ���ϸ��map
void CPlatformDataMgr::ClearMapQryRltPositionDetails(std::string& strQryInstrument)
{
    Lock_CPlatformDataMgr;

    m_PositionDetailData.WillQry_Internal(strQryInstrument);

    Unlock_CPlatformDataMgr;

}

//��ȡ��ѯ�ĺ�ԼID
void CPlatformDataMgr::GetQryPositionDetailInstrument(std::string& outQryInstrument)
{
    Lock_CPlatformDataMgr;

    m_PositionDetailData.GetQryInstrument(outQryInstrument);

    Unlock_CPlatformDataMgr;

}

int CPlatformDataMgr::CloseSinglePosition_Internal( int CloseVolume, PlatformStru_PositionDetail &positionDetail, 
                                           int nVolumeMultiple, double dPrice)
{

	int thisVolume=CloseVolume;
	if(thisVolume>positionDetail.Volume) thisVolume=positionDetail.Volume;

	//���²�λ����
	positionDetail.Volume-=thisVolume;
	CloseVolume-=thisVolume;

	//����ƽ����
	positionDetail.CloseVolume+=thisVolume;                                            
	//����ƽ�ֽ��
	positionDetail.CloseAmount+=thisVolume*nVolumeMultiple*dPrice;

	//����ƽ��ӯ��(����)
	bool bTodayPositionDetail = IsTodayPosition(positionDetail.OpenDate);
	if(bTodayPositionDetail)
	{
		if(positionDetail.Direction==THOST_FTDC_D_Buy)
			positionDetail.CloseProfitByDate+=(dPrice-positionDetail.OpenPrice)*thisVolume*nVolumeMultiple;                  //���ն���ƽ��ӯ��
		else
			positionDetail.CloseProfitByDate+=-1*(dPrice-positionDetail.OpenPrice)*thisVolume*nVolumeMultiple;                  //���ն���ƽ��ӯ��
	}
	else 
	{
		if(positionDetail.Direction==THOST_FTDC_D_Buy)
			positionDetail.CloseProfitByDate+=(dPrice-positionDetail.LastSettlementPrice)*thisVolume*nVolumeMultiple;                //���ն���ƽ��ӯ��
		else
			positionDetail.CloseProfitByDate+=-1*(dPrice-positionDetail.LastSettlementPrice)*thisVolume*nVolumeMultiple;                //���ն���ƽ��ӯ��
	}
	if(positionDetail.CloseProfitByDate==-0) positionDetail.CloseProfitByDate=0;

	//����ƽ��ӯ��(���)
	if(positionDetail.Direction==THOST_FTDC_D_Buy)
		positionDetail.CloseProfitByTrade+=(dPrice-positionDetail.OpenPrice)*thisVolume*nVolumeMultiple;                 //��ʶԳ�ƽ��ӯ��
	else
		positionDetail.CloseProfitByTrade+=-1*(dPrice-positionDetail.OpenPrice)*thisVolume*nVolumeMultiple;                 //��ʶԳ�ƽ��ӯ��
	if(positionDetail.CloseProfitByTrade==-0) positionDetail.CloseProfitByTrade=0;


	positionDetail.UpdateSeq=m_PositionDetailData.NewUpdateSeq();	
	return CloseVolume;
}
//�жϳֲ���ϸ�ǲ���(ƽ��)�ɽ��Ķ��ֲֳ�
bool CPlatformDataMgr::IsOpponentPositionDetailOfTrade_IgnoreTradeType(PlatformStru_PositionDetail& PositionDetail,PlatformStru_TradeInfo& TradeInfo)
{
    if(PositionDetail.Volume<=0) return false;
    if(PositionDetail.Direction==TradeInfo.Direction) return false;
    if(PositionDetail.HedgeFlag!=TradeInfo.HedgeFlag) return false;

    bool curbTodayPositionDetail=IsTodayPositionDetail(PositionDetail);
    if(TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseToday&&!curbTodayPositionDetail) return false;
    if(TradeInfo.OffsetFlag==THOST_FTDC_OF_CloseYesterday&&curbTodayPositionDetail) return false;
    return true;
}

//��ȡĳһ��(���)�ֲ���ϸ�Ķ��ֲֳ���ϸ��¼��ע�⣬ֻ���������Խ�ֻ����
//���ڳɽ����³ֲ���ϸʱ��ɢ��ϳֲֵļ��㡣��ȡʱ�õ���ϳֲ���ϸ������
PlatformStru_PositionDetail& CPlatformDataMgr::GetOpponentPositionDetailComb(const PlatformStru_PositionDetail& OriPositionDetail)
{
    std::string OriTradeID(std::string(OriPositionDetail.TradeID));
    std::string OppoTradeID;
    std::string OppoInstrumentID;
    char OppoDirection;

    //��ȡ��Ӧ����ϳֲ���ϸ����
    PlatformStru_PositionComb& PositionDetailComb=GetPositionComb3_Internal(OriTradeID);
    if(PositionDetailComb.IsInvalidRecord())
        return PlatformStru_PositionDetail::GetInvalidRecord();

    if(OriTradeID==std::string(PositionDetailComb.Leg1TradeID))
    {
        OppoTradeID=std::string(PositionDetailComb.Leg2TradeID);
        OppoInstrumentID=std::string(PositionDetailComb.Leg2InstrumentID);
    }
    else 
    {
        OppoTradeID=std::string(PositionDetailComb.Leg1TradeID);
        OppoInstrumentID=std::string(PositionDetailComb.Leg1InstrumentID);
    }
    if(OriPositionDetail.Direction==THOST_FTDC_D_Buy) OppoDirection=THOST_FTDC_D_Sell;
    else OppoDirection=THOST_FTDC_D_Buy;

    PositionDetailKey OppoKey(OriPositionDetail.szAccount, 
                                OppoInstrumentID.c_str(),
                                OriPositionDetail.OpenDate,
                                OppoTradeID.c_str(),
                                OppoDirection, 
                                OriPositionDetail.HedgeFlag,
                                OriPositionDetail.TradeType);

    std::map<PositionDetailKey,PlatformStru_PositionDetail*>::iterator it_PositionDetail;

    it_PositionDetail=m_PositionDetailData.m_all.find(OppoKey);
    if(it_PositionDetail!=m_PositionDetailData.m_all.end()&&it_PositionDetail->second!=NULL) 
        return *(it_PositionDetail->second);
    else return PlatformStru_PositionDetail::GetInvalidRecord();
}


//��ͨ�ɽ�(ƽ��)��Ҫƽ��ϵ�ʱ����ɢ���ֲֳ���ϸ
//1. ���ɻ����������(��ͨ)��λ����������ClosedVolume
//2. ���¶���(���)��λ��������ȥClosedVolume
bool CPlatformDataMgr::BreakupPositionDetailComb(PlatformStru_PositionDetail&OppoPositionDetail,
                                                 int ClosedVolume,
                                                 std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails)
{
    if(ClosedVolume<=0) return false;
    if(OppoPositionDetail.TradeType!=THOST_FTDC_TRDT_CombinationDerived) return false;
    if(OppoPositionDetail.Volume<ClosedVolume) return false;

    const double dInvalidDouble=util::GetIntInvalidValue();

    //����(����ɢ����ͨ)�ֲ���ϸ��key
    PositionDetailKey DerivedKey(OppoPositionDetail);
    DerivedKey.TradeType=THOST_FTDC_TRDT_Common;

    std::map<PositionDetailKey,PlatformStru_PositionDetail*>::iterator it_DerivedPositionDetail;
    it_DerivedPositionDetail=m_PositionDetailData.m_all.find(DerivedKey);
	if(it_DerivedPositionDetail==m_PositionDetailData.m_all.end()) 
    {
        //��OppoPositionDetail�����յ�NewPositionDetail
        PlatformStru_PositionDetail NewPositionDetail=OppoPositionDetail;

        NewPositionDetail.Volume=0;
        NewPositionDetail.TradeType=THOST_FTDC_TRDT_Common;
        memset(NewPositionDetail.CombInstrumentID,0,sizeof(NewPositionDetail.CombInstrumentID));
        NewPositionDetail.CloseProfitByDate=0;
        NewPositionDetail.CloseProfitByTrade=0;
        NewPositionDetail.PositionProfitByDate=0;
        NewPositionDetail.PositionProfitByTrade=0;
        NewPositionDetail.Margin=0;
        NewPositionDetail.ExchMargin=0;
        NewPositionDetail.CloseVolume=0;
        NewPositionDetail.CloseAmount=0;

        NewPositionDetail.nBuyVolumn=0;
        NewPositionDetail.nSellVolumn=0;
        NewPositionDetail.dOpenAvgPrice=0;
        NewPositionDetail.dOpenProfit=0;
        NewPositionDetail.dTotalProfit=0;
        NewPositionDetail.dMarginPerLot=0;
        NewPositionDetail.dPLBurnRate=0;
        NewPositionDetail.dOpenPositionExposure=0;
        NewPositionDetail.dBuyPowerRemain=0;
		NewPositionDetail.dBuyPowerUsed = dInvalidDouble;
		NewPositionDetail.dTotalMarginPaid = dInvalidDouble;
        NewPositionDetail.FTID=0;
        NewPositionDetail.UpdateSeq=0;

        //���ö�Ӧ��PriceTick
        NewPositionDetail.PriceTick=OppoPositionDetail.PriceTick;
        NewPositionDetail.bToday=OppoPositionDetail.bToday;

        m_PositionDetailData.NewReach_Internal(NewPositionDetail);

        it_DerivedPositionDetail=m_PositionDetailData.m_all.find(DerivedKey);
    }

	if(it_DerivedPositionDetail==m_PositionDetailData.m_all.end()||
        it_DerivedPositionDetail->second==NULL) 
        return false;



    //������(��ͨ)�ֲ���ϸ
    PlatformStru_PositionDetail& DerivedPositionDetail=*(it_DerivedPositionDetail->second);

    //����ԭ���ĳֲ���ϸ
    PlatformStru_PositionDetail OriOppoPositionDetail=OppoPositionDetail;
    PlatformStru_PositionDetail OriDerivedPositionDetail=DerivedPositionDetail;

    //���³ֲ���ϸ
    double DeltaValue;   

    //���ն��гֲ�ӯ��
    if(OppoPositionDetail.PositionProfitByDate!=dInvalidDouble)
    {
        DeltaValue=OppoPositionDetail.PositionProfitByDate*ClosedVolume/OppoPositionDetail.Volume;
        OppoPositionDetail.PositionProfitByDate-=DeltaValue;
        DerivedPositionDetail.PositionProfitByDate+=DeltaValue;
    }
    else
        DerivedPositionDetail.PositionProfitByDate=dInvalidDouble;

    //��ʶԳ�ֲ�ӯ��
    if(OppoPositionDetail.PositionProfitByTrade!=dInvalidDouble)
    {
        DeltaValue=OppoPositionDetail.PositionProfitByTrade*ClosedVolume/OppoPositionDetail.Volume;
        OppoPositionDetail.PositionProfitByTrade-=DeltaValue;
        DerivedPositionDetail.PositionProfitByTrade+=DeltaValue;
    }
    else
        DerivedPositionDetail.PositionProfitByTrade=dInvalidDouble;

    //��֤��
    if(OppoPositionDetail.Margin!=dInvalidDouble)
    {
        DeltaValue=OppoPositionDetail.Margin*ClosedVolume/OppoPositionDetail.Volume;
        OppoPositionDetail.Margin-=DeltaValue;
        DerivedPositionDetail.Margin+=DeltaValue;
    }
    else
        DerivedPositionDetail.Margin=dInvalidDouble;

    //��������֤��
    if(OppoPositionDetail.ExchMargin!=dInvalidDouble)
    {
        DeltaValue=OppoPositionDetail.ExchMargin*ClosedVolume/OppoPositionDetail.Volume;
        OppoPositionDetail.ExchMargin-=DeltaValue;
        DerivedPositionDetail.ExchMargin+=DeltaValue;
    }

    //�ֲ���
    OppoPositionDetail.Volume-=ClosedVolume;
    DerivedPositionDetail.Volume+=ClosedVolume;

    //�������к�
    OppoPositionDetail.UpdateSeq=m_PositionDetailData.NewUpdateSeq();
    DerivedPositionDetail.UpdateSeq=m_PositionDetailData.NewUpdateSeq();

    //��¼����ǰ��ĳֲ���ϸ����
    ModifiedPositionDetails.push_back(OriOppoPositionDetail);
    ModifiedPositionDetails.push_back(OppoPositionDetail);
    ModifiedPositionDetails.push_back(OriDerivedPositionDetail);
    ModifiedPositionDetails.push_back(DerivedPositionDetail);

    //��ϳֲ���ϸ������
    PlatformStru_PositionComb& PositionDetailComb=GetPositionComb3_Internal(OppoPositionDetail.TradeID);
    if(PositionDetailComb.TotalAmt>=ClosedVolume)
    {
        if(PositionDetailComb.Leg1Margin!=dInvalidDouble)
            PositionDetailComb.Leg1Margin -= PositionDetailComb.Leg1Margin*ClosedVolume/PositionDetailComb.TotalAmt;
        else PositionDetailComb.Leg1Margin = dInvalidDouble;

        if(PositionDetailComb.Leg1ExchMargin!=dInvalidDouble)
            PositionDetailComb.Leg1ExchMargin -= PositionDetailComb.Leg1ExchMargin*ClosedVolume/PositionDetailComb.TotalAmt;
        else PositionDetailComb.Leg1ExchMargin = dInvalidDouble;

        if(PositionDetailComb.Leg2Margin!=dInvalidDouble)
            PositionDetailComb.Leg2Margin -= PositionDetailComb.Leg2Margin*ClosedVolume/PositionDetailComb.TotalAmt;
        else PositionDetailComb.Leg2Margin = dInvalidDouble;

        if(PositionDetailComb.Leg2ExchMargin!=dInvalidDouble)
            PositionDetailComb.Leg2ExchMargin -= PositionDetailComb.Leg2ExchMargin*ClosedVolume/PositionDetailComb.TotalAmt;
        else PositionDetailComb.Leg2ExchMargin = dInvalidDouble;

        PositionDetailComb.TotalAmt-=ClosedVolume;

        PositionDetailComb.UpdateSeq=m_PositionCombData.NewUpdateSeq();
    }


    return true;
}
