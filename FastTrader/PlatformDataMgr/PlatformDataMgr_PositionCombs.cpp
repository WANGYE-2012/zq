//ά���ײ���ϳֲ���Ϣ

#pragma warning(disable:4996)

#include <vector>
#include <string>
#include <map>
using namespace std;

#include "PlatformDataMgr_PositionCombs.h"
#include "PlatformDataMgr_Positions.h"
#include "PlatformDataMgrMacro.h"
#include "FileOpr.h"
#include "Module-Misc2/GlobalFunc.h"


#ifdef WIN32
#pragma unmanaged
#endif 


CPlatformDataMgr_PositionCombs::CPlatformDataMgr_PositionCombs(
    volatile long& baseFTID,
    volatile long& baseUpdateSeq,
    CPlatformDataMgr_Instruments& refInstruments,
    CPlatformDataMgr_Quots& refQuots,
    CPlatformDataMgr_Trades& refTrades,
    CPlatformDataMgr_Orders& refOrders,
    CPlatformDataMgr_PositionDetailCombs& refPositionDetailCombs,
    CPlatformDataMgr_PositionDetails& refPositionDetails)
: m_RefInstruments(refInstruments),
  m_RefQuots(refQuots),
  m_RefTrades(refTrades),
  m_RefOrders(refOrders),
  m_RefPositionDetailCombs(refPositionDetailCombs),
  m_RefPositionDetails(refPositionDetails),
  m_PositionCombData(baseFTID,baseUpdateSeq)
{
    //��ѯ�������ʱ���Ƿ���Ҫ����mapQryRlt. ���ڳֲ֣���Ҫ���������ںϲ����ճֲֺ���ʷ�ֲ�
    m_PositionCombData.SetbNeedKey2QryRlt(true);
}

CPlatformDataMgr_PositionCombs::~CPlatformDataMgr_PositionCombs(void)
{
    LOG_INFO("CPlatformDataMgr_PositionCombs::~CPlatformDataMgr_PositionCombs(����CPlatformDataMgr_PositionCombs����)");
}

//��ѯ�ر�����
void CPlatformDataMgr_PositionCombs::QryPositionCombReach( PlatformStru_Position& PositionInfo,int ErrID,bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr_PositionCombs::QryPositionCombReach:InstrumentID=%s",PositionInfo.InstrumentID);

    string strInstrumentID(PositionInfo.InstrumentID);
    PlatformStru_InstrumentInfo InstrumentInfo;

    Lock_CPlatformDataMgr;

    //����ֻ������Ϻ�Լ�ĳֲ֡�����ǵ��Ⱥ�Լ��ֻ����bIsLast�����
    if(ErrID==0)
    {
        if(!m_RefInstruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo))
            ErrID=-1;
        else if(InstrumentInfo.ProductClass!=THOST_FTDC_PC_Combination)
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
            bool bMerge=false;
            if(m_PositionCombData.GetpValueInQryRlt_Internal(pLastPosition,positionKey))
            {
                CPlatformDataMgr_Positions::MergePositions_RspQryPosition(*pLastPosition,PositionInfo);

                //���������Ѿ��ϲ���ԭ���ݼ�¼���ˣ��������Ѿ��������á���ErrID����Ϊ-1���������ͳһ����
                ErrID=-1;
            }
        }
    }


    //ȡ��ԭ�м�¼�ĳֲֳɱ������ֳɱ����ֲ־��ۡ����־���(��Щֵ�ǳ�ʼ����ѯ���ɽ����Լ����)
    if(ErrID==0)
    {
        PlatformStru_Position* pOldPosition;
        if(m_PositionCombData.GetValue_Internal(pOldPosition,PositionKey(PositionInfo)))
        {
            PositionInfo.PositionCost=pOldPosition->PositionCost;
            PositionInfo.OpenCost=pOldPosition->OpenCost;
            PositionInfo.AveragePositionPrice=pOldPosition->AveragePositionPrice;
            PositionInfo.AverageOpenPrice=pOldPosition->AverageOpenPrice;
        }
    }

    //�������ʣ���λ
    if(ErrID==0)
    {
        PositionInfo.YdPositionRemain=PositionInfo.Position-PositionInfo.TodayPosition;
    }

    m_PositionCombData.QryRltReach_Internal(PositionInfo,ErrID,bIsLast);

  //  if(bIsLast)
  //  {
  //      set<string> setInstruments;
  //      m_PositionCombData.GetInstrumentSetOfQryRlt(setInstruments);

  //      for(set<string>::iterator it_Instruments=setInstruments.begin();it_Instruments!=setInstruments.end();it_Instruments++)
		//{
  //          map<string,set<long>>::iterator it_Index2=m_PositionCombData.m_allIndex2.find(*it_Instruments);
  //          if(it_Index2!=m_PositionCombData.m_allIndex2.end()&&!it_Index2->second.empty())
  //          {
  //              for(set<long>::iterator itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
  //              {
  //                  PlatformStru_Position* pPosition;
  //                  if(m_PositionCombData.GetValue2_Internal(pPosition,*itFTID))
  //                  {
  //                      //�������ʣ���λ
  //                      pPosition->YdPositionRemain=pPosition->Position-pPosition->TodayPosition;
  //                      pPosition->UpdateSeq=m_PositionCombData.NewUpdateSeq(pPosition->FTID);
  //                  }
  //              }
		//	}
		//}
  //  }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_PositionCombs::QryPositionCombReach-end");
}


//�ӳɽ���¼������ϳֲ֡�
//�˺�������ϳֲ���ϸ�����´�����������ϳֲ֡���������ϳֲ���ϸ����������ϳֲ���ϸ��������ٸ�����ϳֲ�
void CPlatformDataMgr_PositionCombs::Update_RtnTrade(const PlatformStru_TradeInfo& TradeInfo,
                                                     const map<PositionDetailCombKey,PlatformStru_PositionDetailComb>& ModifiedPositionDetailCombs)
{
	LOG_INFO("CPlatformDataMgr_PositionCombs::Update_RtnTrade(�ɽ��ر�������ϳֲ�):InstrumentID=%s",TradeInfo.InstrumentID);

    CeasymutexGuard guard(m_mutex);

    //��Ӧ����ϳֲ�key
    set<PositionKey> positionkeys;
    PlatformStru_PositionDetailComb detailcomb;
    for(map<PositionDetailCombKey,PlatformStru_PositionDetailComb>::const_iterator it=ModifiedPositionDetailCombs.begin();it!=ModifiedPositionDetailCombs.end();it++)
    {
        if(!m_RefPositionDetailCombs.GetPositionDetailComb(it->first,detailcomb)) continue;
        string strCombInstrumentID(detailcomb.CombInstrumentID);
        if(strCombInstrumentID.empty()) continue;
        if(detailcomb.LegID!=0&&detailcomb.LegID!=1) continue;

        char PosiDirection;
        if(detailcomb.LegID==0) PosiDirection=detailcomb.Direction==THOST_FTDC_D_Buy?THOST_FTDC_PD_Long:THOST_FTDC_PD_Short;
        else PosiDirection=detailcomb.Direction==THOST_FTDC_D_Buy?THOST_FTDC_PD_Short:THOST_FTDC_PD_Long;
        PositionKey positionkey(detailcomb.Account,strCombInstrumentID.c_str(),PosiDirection,detailcomb.HedgeFlag);

        if(positionkeys.find(positionkey)==positionkeys.end())
            positionkeys.insert(positionkey);
    }

    for(set<PositionKey>::const_iterator it=positionkeys.begin();it!=positionkeys.end();it++)
    {
        //��ȡ��Ϻ�Լ��Ϣ
        PlatformStru_InstrumentInfo CombInstrumentInfo;
        if(!m_RefInstruments.GetInstrumentInfo(string(it->InstrumentID),CombInstrumentInfo))
            continue;

        //�����û��ԭ�гֲ֣�����ǿ��ֳɽ���������Ҫ�����µ���ϳֲ�
        PlatformStru_Position* pPositionComb;
        if(!m_PositionCombData.GetValue_Internal(pPositionComb,*it))
        {
            PlatformStru_Position NewPositionData;

            strncpy(NewPositionData.Account,it->Account,sizeof(NewPositionData.Account)-1);
            strncpy(NewPositionData.ExchangeID,TradeInfo.ExchangeID,sizeof(NewPositionData.ExchangeID)-1);
            strncpy(NewPositionData.InstrumentID,it->InstrumentID,sizeof(NewPositionData.InstrumentID)-1);
            strncpy(NewPositionData.BrokerID,TradeInfo.BrokerID,sizeof(NewPositionData.BrokerID)-1);
            strncpy(NewPositionData.InvestorID,TradeInfo.InvestorID,sizeof(NewPositionData.InvestorID)-1);
            NewPositionData.PosiDirection=it->PosiDirection;
            NewPositionData.HedgeFlag=it->HedgeFlag;
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

            NewPositionData.CurPrice=util::GetDoubleInvalidValue();

            //���ö�Ӧ��PriceTick
            NewPositionData.PriceTick=CombInstrumentInfo.PriceTick;
            strncpy(NewPositionData.Currency,CombInstrumentInfo.Currency,sizeof(NewPositionData.Currency)-1);

            m_PositionCombData.NewReach_Internal(NewPositionData);

            if(!m_PositionCombData.GetValue_Internal(pPositionComb,*it))
                continue;
        }



        pPositionComb->Position=0;
        pPositionComb->PositionCost=0;
        pPositionComb->OpenCost=0;
        pPositionComb->AveragePositionPrice=0;
        pPositionComb->AverageOpenPrice=0;
        pPositionComb->UseMargin=0;
        pPositionComb->ExchangeMargin=0;
        pPositionComb->OpenVolume=util::GetIntInvalidValue();
        pPositionComb->OpenAmount=util::GetDoubleInvalidValue();
        pPositionComb->TodayPosition=util::GetIntInvalidValue();
        pPositionComb->YdPosition=util::GetIntInvalidValue();
        pPositionComb->CloseAmount=util::GetDoubleInvalidValue();
        pPositionComb->CloseProfit=util::GetDoubleInvalidValue();
        pPositionComb->CloseProfitByDate=util::GetDoubleInvalidValue();
        pPositionComb->CloseProfitByTrade=util::GetDoubleInvalidValue();

        pPositionComb->PositionProfit=util::GetDoubleInvalidValue();
        pPositionComb->PositionProfitByTrade=util::GetDoubleInvalidValue();
        pPositionComb->TotalPositionProfitByDate=util::GetDoubleInvalidValue();
        pPositionComb->Commission=util::GetDoubleInvalidValue();
        

        UpdateCostByPositionDetialComb_Internal(*it,*pPositionComb,true);
    }


    LOG_INFO("CPlatformDataMgr_PositionCombs::Update_RtnTrade-end");
    return;
}

//������ϳֲ���ϸ�ͳֲ���ϸ��������ϳֲֵĳֲֳɱ������ֳɱ����ֲ־��ۺͿ��־��ۡ��ڳ�ʼ����ѯ��Ϻ����
void CPlatformDataMgr_PositionCombs::UpdateCostByPositionDetialComb_AfterInitQry(void)
{
    CeasymutexGuard guard(m_mutex);
    for(map<PositionKey,PlatformStru_Position*>::iterator it=m_PositionCombData.m_all.begin();it!=m_PositionCombData.m_all.end();it++)
    {
        UpdateCostByPositionDetialComb_Internal(it->first,*(it->second),false);
    }
}

//������ϳֲ���ϸ�ͳֲ���ϸ��������ϳֲֵĳֲ������ֲֳɱ������ֳɱ����ֲ־��ۡ����־��ۺͱ�֤��
void CPlatformDataMgr_PositionCombs::UpdateCostByPositionDetialComb_Internal(const PositionKey& combkey,PlatformStru_Position& comb,bool bUpdateMargin_Position)
{
    CeasymutexGuard guard(m_mutex);

    CFileOpr::getObj().writelocallog("������ϳֲֳɱ�.log","UpdateCostByPositionDetialComb_Internal-��ʼ");

    comb.PositionCost=0;
    comb.OpenCost=0;
    comb.AveragePositionPrice=util::GetIntInvalidValue();
    comb.AverageOpenPrice=util::GetIntInvalidValue();
    if(bUpdateMargin_Position)
    {
        comb.Position=0;
        comb.UseMargin=0;
        comb.ExchangeMargin=0;
    }

    vector<PositionDetailCombKey> detailcombkeys;
    m_RefPositionDetailCombs.GetPositionDetailCombKeysByPositionCombKey(combkey,detailcombkeys);
    int VolumeMultiple=util::GetIntInvalidValue();

    CFileOpr::getObj().writelocallog("������ϳֲֳɱ�.log","UpdateCostByPositionDetialComb_Internal-1");

    for(int i=0;i<(int)detailcombkeys.size();i++)
    {
        //ȡ��Ӧ����ϳֲ���ϸ
        PlatformStru_PositionDetailComb detailcomb;
        if(!m_RefPositionDetailCombs.GetPositionDetailComb(detailcombkeys[i],detailcomb))
        {
            CFileOpr::getObj().writelocallog("������ϳֲֳɱ�.log","UpdateCostByPositionDetialComb_Internal-ȡ��Ӧ����ϳֲ���ϸʧ��");
            break;
        }

        //ȡ��Ӧ�ĳֲ���ϸ
        PositionDetailKey detailkey(detailcombkeys[i].Account,detailcombkeys[i].InstrumentID,detailcombkeys[i].OpenDate,detailcombkeys[i].TradeID,detailcombkeys[i].Direction,THOST_FTDC_TRDT_CombinationDerived,detailcombkeys[i].HedgeFlag);
        PlatformStru_PositionDetail detail;
        if(!m_RefPositionDetails.GetPositionDetail(detailkey,detail))
        {
            CFileOpr::getObj().writelocallog("������ϳֲֳɱ�.log","UpdateCostByPositionDetialComb_Internal-ȡ��Ӧ�ĳֲ���ϸʧ��");
            break;
        }

        bool bToday=detailcomb.bToday;

        //��ȡ���Ⱥ�Լ��Ϣ
        PlatformStru_InstrumentInfo InstrumentInfo;
        if(!m_RefInstruments.GetInstrumentInfo(string(detailcombkeys[i].InstrumentID),InstrumentInfo))
        {
            CFileOpr::getObj().writelocallog("������ϳֲֳɱ�.log","UpdateCostByPositionDetialComb_Internal-��ȡ���Ⱥ�Լ��Ϣʧ��");
            break;
        }

        if(detailcomb.LegID!=0&&detailcomb.LegID!=1)
        {
            CFileOpr::getObj().writelocallog("������ϳֲֳɱ�.log","UpdateCostByPositionDetialComb_Internal-LegID������0��1");
            break;
        }

        if(util::isInvalidValue(VolumeMultiple))
        {
            VolumeMultiple=InstrumentInfo.VolumeMultiple;
            CFileOpr::getObj().writelocallog("������ϳֲֳɱ�.log","UpdateCostByPositionDetialComb_Internal-��ȡVolumeMultiple=%d",VolumeMultiple);
        }

        //������ϳֲֵĳֲֳɱ�
        comb.PositionCost += (detailcomb.LegID==0?1:-1)*detailcomb.TotalAmt*InstrumentInfo.VolumeMultiple*(bToday?detail.OpenPrice:detail.LastSettlementPrice);

        //������ϳֲֵĿ��ֳɱ�
        comb.OpenCost += (detailcomb.LegID==0?1:-1)*detailcomb.TotalAmt*InstrumentInfo.VolumeMultiple*detail.OpenPrice;
        
        //���³ֲּ�¼�ĳֲ�����ռ�ñ�֤��
        if(bUpdateMargin_Position)
        {
            if(detailcomb.LegID==0)
                comb.Position+=detailcomb.TotalAmt;

            if(!util::isInvalidValue(comb.UseMargin))
            {
                if(!util::isInvalidValue(detailcomb.Margin))
                    comb.UseMargin+=detailcomb.Margin;
                else comb.UseMargin=util::GetDoubleInvalidValue();
            }
            if(!util::isInvalidValue(comb.ExchangeMargin))
            {
                if(!util::isInvalidValue(detailcomb.ExchMargin))
                    comb.ExchangeMargin+=detailcomb.ExchMargin;
                else comb.ExchangeMargin=util::GetDoubleInvalidValue();
            }
        }
    }
    CFileOpr::getObj().writelocallog("������ϳֲֳɱ�.log","UpdateCostByPositionDetialComb_Internal-VolumeMultiple=%d",VolumeMultiple);
    if(!util::isInvalidValue(VolumeMultiple))
    {
        comb.AveragePositionPrice=comb.Position>0&&VolumeMultiple>0?comb.PositionCost/comb.Position/VolumeMultiple:0;
        comb.AverageOpenPrice=comb.Position>0&&VolumeMultiple>0?comb.OpenCost/comb.Position/VolumeMultiple:0;
        CFileOpr::getObj().writelocallog("������ϳֲֳɱ�.log","UpdateCostByPositionDetialComb_Internal-AveragePositionPrice=%g AverageOpenPrice=%g",comb.AveragePositionPrice,comb.AverageOpenPrice);
    }
    
    CFileOpr::getObj().writelocallog("������ϳֲֳɱ�.log","UpdateCostByPositionDetialComb_Internal-����");
    comb.UpdateSeq=m_PositionCombData.NewUpdateSeq(comb.FTID);
}

//������ϳֲּ�¼�ı�֤�𡢱�֤���ʣ���ѯ��֤������Ӧʱ����
bool CPlatformDataMgr_PositionCombs::UpdatePositionByMarginRate_RspQryRate(const PlatformStru_InstrumentMarginRate& MarginRate) 
{
    bool brlt=false;
    string strInstrument(MarginRate.InstrumentID);

    Lock_CPlatformDataMgr;

    map<PositionKey,double> PositionMargin;
    map<PositionKey,double> PositionExchangeMargin;
    map<PositionKey,double>::const_iterator it;
    PlatformStru_Position* pPosition;

    m_RefPositionDetailCombs.GetPositionMarginByLegInstrument(strInstrument,PositionMargin,PositionExchangeMargin);
    for(it=PositionMargin.begin();it!=PositionMargin.end();it++)
    {
        if(m_PositionCombData.GetValue_Internal(pPosition,it->first)&&
            !util::equals(pPosition->UseMargin,it->second)) 
        {
            pPosition->UseMargin=it->second;
            if(!brlt) brlt=true;
        }
    }
    for(it=PositionExchangeMargin.begin();it!=PositionExchangeMargin.end();it++)
    {
        if(m_PositionCombData.GetValue_Internal(pPosition,it->first)&&
            !util::equals(pPosition->ExchangeMargin,it->second)) 
        {
            pPosition->ExchangeMargin=it->second;
            if(!brlt) brlt=true;
        }
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}


//��ղ�ѯ��ϳֲֵ�map
void CPlatformDataMgr_PositionCombs::ClearMapQryRltPositionCombs(string& strQryInstrument)
{
    Lock_CPlatformDataMgr;
	m_PositionCombData.WillQry_Internal(strQryInstrument);
    Unlock_CPlatformDataMgr;
}
//��ȡ��ϳֲֵļ���
int CPlatformDataMgr_PositionCombs::GetPositionCombs(vector<PlatformStru_Position>& outData)
{
	outData.clear();

    Lock_CPlatformDataMgr;

	map<PositionKey,PlatformStru_Position*>::iterator it = m_PositionCombData.m_all.begin();
	for ( ; it != m_PositionCombData.m_all.end(); it++ )
	{
        if ( it->second!=NULL && it->second->Position > 0 )
		{
            outData.push_back(*(it->second));
		}
	}

    Unlock_CPlatformDataMgr;

    return outData.size();
}
//��ȡָ����Լ����ϳֲּ���
int CPlatformDataMgr_PositionCombs::GetPositionCombs2(const string& strInstrument,vector<PlatformStru_Position>& outData)
{
	outData.clear();

    Lock_CPlatformDataMgr;

	map<PositionKey,PlatformStru_Position*>::iterator it = m_PositionCombData.m_all.begin();
	for ( ; it != m_PositionCombData.m_all.end(); it++ )
	{
        if ( it->second!=NULL && it->second->Position > 0 && 
            strncmp(strInstrument.c_str(),it->second->InstrumentID,sizeof(it->second->InstrumentID))==0)
		{
            outData.push_back(*(it->second));
		}
	}

    Unlock_CPlatformDataMgr;

    return outData.size();
}

//��ȡ��ѯ�ĺ�ԼID
void CPlatformDataMgr_PositionCombs::GetQryPositionCombInstrument(string& outQryInstrument)
{
    Lock_CPlatformDataMgr;
    m_PositionCombData.GetQryInstrument(outQryInstrument);
    Unlock_CPlatformDataMgr;
}

//��ȡ��ϳֲֵ�FTID����
int CPlatformDataMgr_PositionCombs::GetPositionCombFTIDList(vector<long>& vec)
{
	vec.clear();
	Lock_CPlatformDataMgr;
	set<long>::iterator it_FTID = m_PositionCombData.m_allFTID.begin();
	for ( ; it_FTID != m_PositionCombData.m_allFTID.end(); ++it_FTID )
	{
		vec.push_back(*it_FTID);
	}
	Unlock_CPlatformDataMgr;

	return vec.size();
}

//��ȡָ��FTID����ϳֲ���ϸ
int CPlatformDataMgr_PositionCombs::GetPositionCombInfo(long lFTID, PlatformStru_Position& outData)
{
	int nRet = 0;
	Lock_CPlatformDataMgr;
	map<long, PlatformStru_Position*>::iterator it_FTID = m_PositionCombData.m_all2.find(lFTID);
	if ( it_FTID != m_PositionCombData.m_all2.end())
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

//ȡȫ����ϳֲֵ�ռ�ñ�֤��
void CPlatformDataMgr_PositionCombs::GetAllMargin(double& TotalMargin)
{
	TotalMargin=0;
	std::map<PositionKey,PlatformStru_Position*>::iterator iter;
    double InvalideValue=util::GetDoubleInvalidValue();

	Lock_CPlatformDataMgr;
	iter = m_PositionCombData.m_all.begin();
	for(;iter != m_PositionCombData.m_all.end(); iter++)
	{
        if(TotalMargin!=InvalideValue)
        {
            if(iter->second!=NULL&&iter->second->UseMargin!=InvalideValue)
                TotalMargin+=iter->second->UseMargin;
            else TotalMargin=InvalideValue;
        }
	}
	Unlock_CPlatformDataMgr;
}

void CPlatformDataMgr_PositionCombs::clear(void)
{
	Lock_CPlatformDataMgr;
    m_PositionCombData.Clear();
	Unlock_CPlatformDataMgr;
}
bool CPlatformDataMgr_PositionCombs::empty(void)
{
    Lock_CPlatformDataMgr;
    bool brlt=m_PositionCombData.empty();
    Unlock_CPlatformDataMgr;
    return brlt;
}

