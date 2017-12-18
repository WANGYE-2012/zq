//ά���ײ���ϳֲ���ϸ��Ϣ

#pragma warning(disable:4996)

#include <assert.h>
#include "PlatformDataMgr_PositionDetailCombs.h"
#include "PlatformDataMgrMacro.h"
#include "FileOpr.h"
#include "Module-Misc2/GlobalFunc.h"


#ifdef WIN32
#pragma unmanaged
#endif 


CPlatformDataMgr_PositionDetailCombs::CPlatformDataMgr_PositionDetailCombs( 
    volatile long& baseFTID,
    volatile long& baseUpdateSeq,
    CPlatformDataMgr_Instruments& refInstruments,
    CPlatformDataMgr_Quots& refQuots,
    CPlatformDataMgr_Trades& refTrades,
    CPlatformDataMgr_Orders& refOrders)
: m_RefInstruments(refInstruments),
  m_RefQuots(refQuots),
  m_RefTrades(refTrades),
  m_RefOrders(refOrders),
  m_PositionDetailCombData(baseFTID,baseUpdateSeq)
{
}

CPlatformDataMgr_PositionDetailCombs::~CPlatformDataMgr_PositionDetailCombs(void)
{
    LOG_INFO("CPlatformDataMgr_PositionDetailCombs::~CPlatformDataMgr_PositionDetailCombs(����CPlatformDataMgr_PositionDetailCombs����)");
}

//��ѯ�ر�����
void CPlatformDataMgr_PositionDetailCombs::QryPositionDetailCombReach( PlatformStru_PositionDetailComb& PositionDetailCombInfo,int ErrID,bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr_PositionDetailCombs::QryPositionDetailCombReach:InstrumentID=%s",PositionDetailCombInfo.InstrumentID);

    Lock_CPlatformDataMgr;

    if(ErrID==0)
    {
        //�ж��Ƿ��ǽ��
        PositionDetailCombInfo.bToday=m_RefInstruments.IsToday(PositionDetailCombInfo.OpenDate);

        //����ǽ�֣�ȡԭ����ComTradeID������ı�m_PositionDetailCombData�е�keyֵ
        if(PositionDetailCombInfo.bToday)
        {
            PositionDetailKey detailkey(PositionDetailCombInfo.Account,PositionDetailCombInfo.InstrumentID,PositionDetailCombInfo.OpenDate,PositionDetailCombInfo.TradeID,PositionDetailCombInfo.Direction,THOST_FTDC_TRDT_CombinationDerived,PositionDetailCombInfo.HedgeFlag);
            map<PositionDetailKey,set<PositionDetailCombKey>>::const_iterator it=m_mapPositionDetailKey2Key.find(detailkey);
            if(it!=m_mapPositionDetailKey2Key.end()&&it->second.size()==1)
            {
                memset(PositionDetailCombInfo.ComTradeID,0,sizeof(PositionDetailCombInfo.ComTradeID));
                strncpy(PositionDetailCombInfo.ComTradeID,it->second.begin()->ComTradeID,sizeof(PositionDetailCombInfo.ComTradeID)-1);
            }
        }
    }

    //�������͵����ݼ���
    m_PositionDetailCombData.QryRltReach_Internal(PositionDetailCombInfo,ErrID,bIsLast);

    if(ErrID==0)
    {
        string strCombInstrumentID(PositionDetailCombInfo.CombInstrumentID);
        PositionDetailCombKey key(PositionDetailCombInfo);

        //ά��CombInstrumentID��Key֮��Ĺ�ϵ
        map<string,set<PositionDetailCombKey>>::iterator it1=m_mapCombInstrumentID2Key.find(strCombInstrumentID);
        if(it1==m_mapCombInstrumentID2Key.end())
        {
            m_mapCombInstrumentID2Key[strCombInstrumentID]=set<PositionDetailCombKey>();
            it1=m_mapCombInstrumentID2Key.find(strCombInstrumentID);
        }
        if(it1->second.find(key)==it1->second.end()) 
            it1->second.insert(key);

        //ά��LegInstrumentID��CombInstrumentID֮��Ĺ�ϵ
        string strLegInstrumentID(PositionDetailCombInfo.InstrumentID);
        if(!CTools_Ansi::IsKeyValueInMultimap<string,string>(m_mapLegInstrumentID2CombInstrumentID,strLegInstrumentID,strCombInstrumentID))
            m_mapLegInstrumentID2CombInstrumentID.insert(make_pair(strLegInstrumentID,strCombInstrumentID));

        //ά��ComTradeID��key֮��Ĺ�ϵ
        string strComTradeID(PositionDetailCombInfo.ComTradeID);
        CTools_Ansi::mytrim(strComTradeID);
        if(!CTools_Ansi::IsKeyValueInMultimap<string,PositionDetailCombKey>(m_mapComTradeID2Key,strComTradeID,key))
            m_mapComTradeID2Key.insert(make_pair(strComTradeID,key));

        //ά����ϳֲ�key����ϳֲ���ϸkey֮��Ĺ�ϵ
        PositionKey positionkey(
            PositionDetailCombInfo.Account,
            PositionDetailCombInfo.CombInstrumentID,
            PositionDetailCombInfo.Direction==THOST_FTDC_D_Buy?(PositionDetailCombInfo.LegID==0?THOST_FTDC_PD_Long:THOST_FTDC_PD_Short):(PositionDetailCombInfo.LegID==0?THOST_FTDC_PD_Short:THOST_FTDC_PD_Long),
            PositionDetailCombInfo.HedgeFlag);
        if(!CTools_Ansi::IsKeyValueInMultimap<PositionKey,PositionDetailCombKey>(m_mapPositionCombKey2Key,positionkey,key))
            m_mapPositionCombKey2Key.insert(make_pair(positionkey,key));

        //ά���ֲ���ϸKey�����Key֮��Ĺ�ϵ�������һ��һ����ֿ�����һ�Զ�
        PositionDetailKey detailkey(
            PositionDetailCombInfo.Account,
            PositionDetailCombInfo.InstrumentID,
            PositionDetailCombInfo.OpenDate,
            PositionDetailCombInfo.TradeID,
            PositionDetailCombInfo.Direction,
            THOST_FTDC_TRDT_CombinationDerived,
            PositionDetailCombInfo.HedgeFlag);
        map<PositionDetailKey,set<PositionDetailCombKey>>::iterator it2=m_mapPositionDetailKey2Key.find(detailkey);
        if(it2==m_mapPositionDetailKey2Key.end())
        {
            m_mapPositionDetailKey2Key[detailkey]=set<PositionDetailCombKey>();
            it2=m_mapPositionDetailKey2Key.find(detailkey);
        }
        if(it2->second.find(key)==it2->second.end()) 
            it2->second.insert(key);

    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_PositionDetailCombs::QryPositionDetailCombReach-end");
}



//�ӳɽ���¼������ϳֲ���ϸ�������ڳֲ���ϸ�ı仯�������ڳֲ���ϸ��������ٸ��³ֲ�
//���ܱ�֤������ֶ�(�ڳֲ���ϸģ���д���)
//�������гɽ������ģ���OrderSysID_xxxx��ʱ���壬һ��OrderSysID���ܶ�Ӧ�����ϳֲ���ϸ��
//ComTradeID2CloseVolume:ƽ���������λʱ����ƽ������ϳɽ�ID�Ͷ�Ӧƽ�����Ĺ�ϵ
void CPlatformDataMgr_PositionDetailCombs::Update_RtnTrade(const PlatformStru_TradeInfo& TradeInfo,
                                                           const map<PositionDetailKey,int>& mapCloseDeltaVolume,
                                                           const map<string,int>& ComTradeID2CloseVolume,
                                                           map<PositionDetailCombKey,PlatformStru_PositionDetailComb>&ModifiedPositionDetailCombs)
{
    LOG_INFO("CPlatformDataMgr_PositionDetailCombs::Update_RtnTrade:InstrumentID=%s",TradeInfo.InstrumentID);

    Lock_CPlatformDataMgr;

    if(TradeInfo.OffsetFlag==THOST_FTDC_OF_Open)
    {
        if(TradeInfo.TradeType==THOST_FTDC_TRDT_CombinationDerived)
        {
            //����ϳɽ�������ϳֲ���ϸ
            //��Ҫ����һ����ϳɽ���Ӧ�����ɽ���¼���������a1b1a1b1��˳�򣬻�����a1a1b2
            CreatePositionDetailCombFromOpenTrade_Internal(TradeInfo,ModifiedPositionDetailCombs);
        }
    }
    else
    {
        //ƽ�֣������漰����ϳֲ���ϸ�����
        //Ҫ����һ���ֲ���ϸ��Ӧһ��������ϳֲ���ϸ
        for(map<PositionDetailKey,int>::const_iterator itdetailkey=mapCloseDeltaVolume.begin();itdetailkey!=mapCloseDeltaVolume.end();itdetailkey++)
        {
            if(itdetailkey->first.TradeType!=THOST_FTDC_TRDT_CombinationDerived)
                continue;

            int DeltaVolume=itdetailkey->second;
            if(DeltaVolume<=0) continue;

            map<PositionDetailKey,set<PositionDetailCombKey>>::const_iterator itkeys=m_mapPositionDetailKey2Key.find(itdetailkey->first);
            if(itkeys!=m_mapPositionDetailKey2Key.end()&&itkeys->second.size()>0)
            {
                for(set<PositionDetailCombKey>::const_iterator itkey=itkeys->second.begin();DeltaVolume>0&&itkey!=itkeys->second.end();itkey++)
                {
                    PlatformStru_PositionDetailComb *pComb;
                    if(!m_PositionDetailCombData.GetValue_Internal(pComb,*itkey)) continue;
                    if(pComb->TotalAmt==0) continue;

                    //ComTradeID��Ҫ���ϳֲ���ϸƽ��ʱ��Ӧ��ֵ
                    string strComTradeID(pComb->ComTradeID);
                    map<string,int>::const_iterator itComTradeID2CloseVolume=ComTradeID2CloseVolume.find(strComTradeID);
                    if(itComTradeID2CloseVolume==ComTradeID2CloseVolume.end())
                        continue;

                    if(ModifiedPositionDetailCombs.find(*itkey)==ModifiedPositionDetailCombs.end())
                        ModifiedPositionDetailCombs[*itkey]=*pComb;

                    //�޸�ԭ������ϳֲ���ϸ
                    int thisVolume=DeltaVolume;
                    if(thisVolume>pComb->TotalAmt) thisVolume=pComb->TotalAmt;
                    if(thisVolume>itComTradeID2CloseVolume->second) thisVolume=itComTradeID2CloseVolume->second;

                    if(!util::isInvalidValue(pComb->Margin))
                        pComb->Margin=pComb->Margin*(pComb->TotalAmt-thisVolume)/pComb->TotalAmt;
                    if(!util::isInvalidValue(pComb->ExchMargin))
                        pComb->ExchMargin=pComb->ExchMargin*(pComb->TotalAmt-thisVolume)/pComb->TotalAmt;

                    DeltaVolume-=thisVolume;
                    pComb->TotalAmt-=thisVolume;

                    pComb->UpdateSeq=m_PositionDetailCombData.NewUpdateSeq(pComb->FTID);
                }
            }

        }
    }

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_PositionDetailCombs::Update_RtnTrade-end");
}

//����ϳɽ�������ϳֲ���ϸ
//ע�⣺1. һ����Ͽ��ֱ��������ܶ�Ӧ��Գɽ���������a1b1a1b1��˳�򣬻�����a1a1b2
//      2. ModifiedPositionDetailCombs���汻�ɽ��޸ĵ���ϳֲ���ϸ��¼�����ڸ�����ϳֲ�
void CPlatformDataMgr_PositionDetailCombs::CreatePositionDetailCombFromOpenTrade_Internal(const PlatformStru_TradeInfo &TradeInfo,
                                                                                          map<PositionDetailCombKey,PlatformStru_PositionDetailComb>& ModifiedPositionDetailCombs)
{
    //ֻ������������ɽ�
    if(TradeInfo.TradeType!=THOST_FTDC_TRDT_CombinationDerived)
        return;

    //ֻ�����ֳɽ�
    if(TradeInfo.OffsetFlag!=THOST_FTDC_OF_Open)
        return;

    string strInstrumentID(TradeInfo.InstrumentID);
    string strOppoInstrumentID("");
    string strCombInstrumentID("");
    string strLeg1InstrumentID("");
    string strLeg2InstrumentID("");
    int LegID=util::GetIntInvalidValue();
    int OppoLegID=util::GetIntInvalidValue();

    //����ȷ�����ֺ�ԼID����Ϻ�ԼID��LegID��
    map<string,TradeKey> TradeKeys;
    int TradeVolume=0,OppoTradeVolume=0,thisVolume;
    m_RefTrades.GetTradeKeysByTradeComb_OrderSysID2(TradeKey(TradeInfo),TradeKeys);
    for(map<string,TradeKey>::const_iterator it=TradeKeys.begin();it!=TradeKeys.end();it++)
    {
        if(!m_RefTrades.GetTradeVolume(it->second,thisVolume)) 
            return;

        string tmpInstrument(it->second.InstrumentID);
        if(it->second.Direction==TradeInfo.Direction)//tmpInstrument==strInstrumentID)
        {
            TradeVolume+=thisVolume;
        }
        else
        {
            OppoTradeVolume+=thisVolume;

            if(strOppoInstrumentID.empty())
            {
                strOppoInstrumentID=tmpInstrument;
                if(m_RefInstruments.GetComboInstrumentID(strInstrumentID,strOppoInstrumentID,strCombInstrumentID,strLeg1InstrumentID,strLeg2InstrumentID))
                {
                    LegID= strLeg1InstrumentID==strInstrumentID ? 0 : 1;
                    OppoLegID = LegID==0 ? 1 : 0;
                }
            }
        }
    }
    //�ж��ֳɽ����������߳ɽ�����һ��ʱ���Ž��д���
    if(strOppoInstrumentID.empty()||strCombInstrumentID.empty()||TradeVolume!=OppoTradeVolume)
        return;

    //��ʽ����ʱComTradeIDʹ�õ�OrderSysID���ų�_xxxx\0��λ��
    TThostFtdcTradeIDType ComTradeID;
    TThostFtdcTradeIDType baseComTradeID;
    TThostFtdcOrderSysIDType tmpOrderSysID;
    CTools_Ansi::mymemcpy_trim(tmpOrderSysID,sizeof(tmpOrderSysID),TradeInfo.OrderSysID);
    int len=(int)strlen(tmpOrderSysID);
    int maxlen=(int)sizeof(baseComTradeID)-1-5;
    if(len>maxlen)
    {
        memcpy(baseComTradeID,tmpOrderSysID+len-maxlen,maxlen);
        baseComTradeID[maxlen]=0;
    }
    else 
    {
        memcpy(baseComTradeID,tmpOrderSysID,len);
        baseComTradeID[len]=0;
    }

    //���洴����ϳֲ���ϸ����֮ͬǰ����δ��Զ�û�д�����һ��
    //��TradeVolume==OppoTradeVolumeʱ����һ����ϳֲ���ϸ
    int ind=1;
    TradeVolume=0;
    OppoTradeVolume=0;
    sprintf(ComTradeID,"%s%04d",baseComTradeID,ind);
    string strComTradeID(ComTradeID);
    CTools_Ansi::mytrim(strComTradeID);

    for(map<string,TradeKey>::const_iterator it=TradeKeys.begin();it!=TradeKeys.end();it++)
    {
        PlatformStru_TradeInfo tmptrade;
        if(!m_RefTrades.GetTradeInfo(it->second,tmptrade)) continue;
        string tmpInstrument(it->second.InstrumentID);

        PlatformStru_PositionDetailComb PositionDetailComb(tmptrade,true,strComTradeID,strCombInstrumentID,tmpInstrument==strInstrumentID?LegID:OppoLegID);
        PositionDetailCombKey key(PositionDetailComb);
        if(!m_PositionDetailCombData.IsExist(key))
        {
            m_PositionDetailCombData.NewReach_Internal(PositionDetailComb);
            if(ModifiedPositionDetailCombs.find(key)==ModifiedPositionDetailCombs.end())
                ModifiedPositionDetailCombs[key]=PlatformStru_PositionDetailComb(true);

            //ά��CombInstrumentID��Key֮��Ĺ�ϵ
            map<string,set<PositionDetailCombKey>>::iterator it1=m_mapCombInstrumentID2Key.find(strCombInstrumentID);
            if(it1==m_mapCombInstrumentID2Key.end())
            {
                m_mapCombInstrumentID2Key[strCombInstrumentID]=set<PositionDetailCombKey>();
                it1=m_mapCombInstrumentID2Key.find(strCombInstrumentID);
            }
            if(it1->second.find(key)==it1->second.end()) 
                it1->second.insert(key);


            //ά��LegInstrumentID��CombInstrumentID֮��Ĺ�ϵ
            if(!CTools_Ansi::IsKeyValueInMultimap<string,string>(m_mapLegInstrumentID2CombInstrumentID,tmpInstrument,strCombInstrumentID))
                m_mapLegInstrumentID2CombInstrumentID.insert(make_pair(tmpInstrument,strCombInstrumentID));

            //ά��ComTradeID��key֮��Ĺ�ϵ
            if(!CTools_Ansi::IsKeyValueInMultimap<string,PositionDetailCombKey>(m_mapComTradeID2Key,strComTradeID,key))
                m_mapComTradeID2Key.insert(make_pair(strComTradeID,key));

            //ά����ϳֲ�key����ϳֲ���ϸkey֮��Ĺ�ϵ
            PositionKey positionkey(
                PositionDetailComb.Account,
                strCombInstrumentID.c_str(),
                PositionDetailComb.Direction==THOST_FTDC_D_Buy?(PositionDetailComb.LegID==0?THOST_FTDC_PD_Long:THOST_FTDC_PD_Short):(PositionDetailComb.LegID==0?THOST_FTDC_PD_Short:THOST_FTDC_PD_Long),
                PositionDetailComb.HedgeFlag);
            if(!CTools_Ansi::IsKeyValueInMultimap<PositionKey,PositionDetailCombKey>(m_mapPositionCombKey2Key,positionkey,key))
                m_mapPositionCombKey2Key.insert(make_pair(positionkey,key));

            //ά���ֲ���ϸKey�����Key֮��Ĺ�ϵ�������һ��һ����ֿ�����һ�Զ�
            PositionDetailKey detailkey(
                it->second.Account,
                it->second.InstrumentID,
                it->second.TradeDate,
                it->second.TradeID,
                it->second.Direction,
                THOST_FTDC_TRDT_CombinationDerived,
                it->second.HedgeFlag);
            map<PositionDetailKey,set<PositionDetailCombKey>>::iterator itdetailkey=m_mapPositionDetailKey2Key.find(detailkey);
            if(itdetailkey==m_mapPositionDetailKey2Key.end())
            {
                m_mapPositionDetailKey2Key[detailkey]=set<PositionDetailCombKey>();
                itdetailkey=m_mapPositionDetailKey2Key.find(detailkey);
            }
            if(itdetailkey->second.find(key)==itdetailkey->second.end()) 
                itdetailkey->second.insert(key);
        }

        if(it->second.Direction==TradeInfo.Direction) TradeVolume+=thisVolume;
        else OppoTradeVolume+=thisVolume;
        //if(tmpInstrument==strInstrumentID) TradeVolume+=thisVolume;
        //else OppoTradeVolume+=thisVolume;
        if(TradeVolume==OppoTradeVolume)
        {
            ind++;
            sprintf(ComTradeID,"%s%04d",baseComTradeID,ind);
            strComTradeID=string(ComTradeID);
            CTools_Ansi::mytrim(strComTradeID);
        }
    }

    LOG_INFO("CPlatformDataMgr_PositionDetailCombs::CreatePositionDetailCombFromOpenTrade_Internal(���ֳɽ�������ϳֲ���ϸ��¼1):%s",
        PositionDetailComb.tostring(logbuf,sizeof(logbuf)));
}




//��ȡ������ϳֲ���ϸkey
//������ϳֲ���ϸ�����ж��
////��֣�1. TradeID��OrderSysID��2. OrderSysID������TradeID��3. ����InstrumentID��TotalAmt�������key�ŵ�orikeys�л�oppokeys��
////��֣�1. ��CombInstrumentIDһ�¡�ComTradeIDһ�£�����InstrumentID��TotalAmt�������key�ŵ�orikeys�л�oppokeys��
//��ComTradeIDһ�£�����InstrumentID��TotalAmt�������key�ŵ�orikeys�л�oppokeys��
//ע�⣬�������oppokeys������ݲ�һ����ͬһ����Լ����ԭʼ��Լ��c1305���ҵ��Ķ��ֽ�������ܰ�����c1309��c1311���������ڴ��̽���������ܲ����Ľ��
//����ֵ��map<string,pair<set<PositionDetailCombKey>,set<PositionDetailCombKey>>>, ComTradeID -> pair<leg1keys,leg2keys>
void CPlatformDataMgr_PositionDetailCombs::GetCombKeysByPositionDetailKey2(const PositionDetailKey& detailkey,
                                                                           map<string,pair<set<PositionDetailCombKey>,set<PositionDetailCombKey>>>& outkeys,
                                                                           bool bFilterZeroVolume)
{
    Lock_CPlatformDataMgr;

    outkeys.clear();

    map<PositionDetailKey,set<PositionDetailCombKey>>::const_iterator it=m_mapPositionDetailKey2Key.find(detailkey);
    if(it!=m_mapPositionDetailKey2Key.end()&&it->second.size()>0)
    {
        //string oriInstrumentID(orikey.InstrumentID);
        PlatformStru_PositionDetailComb *pComb;
        for(set<PositionDetailCombKey>::const_iterator it2=it->second.begin();it2!=it->second.end();it2++)
        {
            if(!m_PositionDetailCombData.GetValue_Internal(pComb,*it2)) continue;
            if(bFilterZeroVolume&&pComb->TotalAmt==0) continue;

            //ȡ��ComTradeID��ͬ�����keys
            string strComTradeID(pComb->ComTradeID);
            string strComTradeID2(strComTradeID);
            CTools_Ansi::mytrim(strComTradeID2);

            if(outkeys.find(strComTradeID)!=outkeys.end()) continue;
            vector<PositionDetailCombKey> comkeys_smaeComTradeID;
            CTools_Ansi::GetValuesInMultimap<string,PositionDetailCombKey>(m_mapComTradeID2Key,strComTradeID2,comkeys_smaeComTradeID);

            //ȡ��ComTradeID��Ӧ��leg1keys��leg2keys
            set<PositionDetailCombKey> leg1keys;
            set<PositionDetailCombKey> leg2keys;
            for(vector<PositionDetailCombKey>::const_iterator it3=comkeys_smaeComTradeID.begin();it3!=comkeys_smaeComTradeID.end();it3++)
            {
                if(!m_PositionDetailCombData.GetValue_Internal(pComb,*it3)) continue;
                if(bFilterZeroVolume&&pComb->TotalAmt==0) continue;
                if(pComb->LegID==0) leg1keys.insert(*it3);
                else leg2keys.insert(*it3);
            }

            //��leg1keys��leg2keys�ŵ�outkeys��
            outkeys[strComTradeID]=make_pair(leg1keys,leg2keys);
        }
    }

    Unlock_CPlatformDataMgr;
}

void CPlatformDataMgr_PositionDetailCombs::GetCombKeysByPositionDetailKey(const PositionDetailKey& detailkey,set<PositionDetailCombKey>& outkeys,bool bFilterZeroVolume)
{
    CeasymutexGuard guard(m_mutex);

    outkeys.clear();

    map<PositionDetailKey,set<PositionDetailCombKey>>::const_iterator it=m_mapPositionDetailKey2Key.find(detailkey);
    if(it!=m_mapPositionDetailKey2Key.end()&&it->second.size()>0)
    {
        outkeys=it->second;

        if(bFilterZeroVolume)
        {
            PlatformStru_PositionDetailComb *pComb;
            for(set<PositionDetailCombKey>::const_iterator it2=outkeys.begin();it2!=outkeys.end();)
            {
                if(!m_PositionDetailCombData.GetValue_Internal(pComb,*it2)||pComb->TotalAmt==0)
                    it2=outkeys.erase(it2);
                else it2++;
            }
        }
    }
}
void CPlatformDataMgr_PositionDetailCombs::GetCombKeysByComTradeID(const string& ComTradeID,set<PositionDetailCombKey>& outkeys,bool bFilterZeroVolume)
{
    CeasymutexGuard guard(m_mutex);

    outkeys.clear();

    string ComTradeID2(ComTradeID);
    CTools_Ansi::mytrim(ComTradeID2);
    CTools_Ansi::GetValuesInMultimap<string,PositionDetailCombKey>(m_mapComTradeID2Key,ComTradeID2,outkeys);

    if(bFilterZeroVolume)
    {
        PlatformStru_PositionDetailComb *pComb;
        for(set<PositionDetailCombKey>::const_iterator it2=outkeys.begin();it2!=outkeys.end();)
        {
            if(!m_PositionDetailCombData.GetValue_Internal(pComb,*it2)||pComb->TotalAmt==0)
                it2=outkeys.erase(it2);
            else it2++;
        }
    }
}
void CPlatformDataMgr_PositionDetailCombs::GetCombKeysByComTradeID2(const string& ComTradeID,set<PositionDetailCombKey>& leg1keys,set<PositionDetailCombKey>& leg2keys,bool bFilterZeroVolume)
{
    CeasymutexGuard guard(m_mutex);

    leg1keys.clear();
    leg2keys.clear();

    set<PositionDetailCombKey> keys;

    string ComTradeID2(ComTradeID);
    CTools_Ansi::mytrim(ComTradeID2);
    CTools_Ansi::GetValuesInMultimap<string,PositionDetailCombKey>(m_mapComTradeID2Key,ComTradeID2,keys);

    PlatformStru_PositionDetailComb *pComb;
    for(set<PositionDetailCombKey>::const_iterator it2=keys.begin();it2!=keys.end();it2++)
    {
        if(!m_PositionDetailCombData.GetValue_Internal(pComb,*it2)) continue;
        if(bFilterZeroVolume&&pComb->TotalAmt==0) continue;
        if(pComb->LegID==0) leg1keys.insert(*it2);
        else leg2keys.insert(*it2);
    }
}
void CPlatformDataMgr_PositionDetailCombs::GetComTradeIDsByPositionDetailKeys(const set<PositionDetailKey>& detailkeys,set<string>& outdatas,bool bFilterZeroVolume)
{
    CeasymutexGuard guard(m_mutex);

    PlatformStru_PositionDetailComb *pComb;

    outdatas.clear();

    for(set<PositionDetailKey>::const_iterator itdetailkey=detailkeys.begin();itdetailkey!=detailkeys.end();itdetailkey++)
    {
        map<PositionDetailKey,set<PositionDetailCombKey>>::const_iterator it=m_mapPositionDetailKey2Key.find(*itdetailkey);
        if(it!=m_mapPositionDetailKey2Key.end()&&it->second.size()>0)
        {
            for(set<PositionDetailCombKey>::const_iterator it2=it->second.begin();it2!=it->second.end();it2++)
            {
                if(bFilterZeroVolume&&
                    (!m_PositionDetailCombData.GetValue_Internal(pComb,*it2)||pComb->TotalAmt==0))
                {
                    continue;
                }
                string strComTradeID(it2->ComTradeID);
                if(outdatas.find(strComTradeID)==outdatas.end())
                    outdatas.insert(strComTradeID);
            }
        }
    }
}

//ȡ��������֤��ʧ�ܷ�����Чֵ
double CPlatformDataMgr_PositionDetailCombs::GetExchMargin(const PositionDetailCombKey& key)
{
    CeasymutexGuard guard(m_mutex);
    PlatformStru_PositionDetailComb *pComb;
    if(!m_PositionDetailCombData.GetValue_Internal(pComb,key)) return util::GetDoubleInvalidValue();
    else return pComb->ExchMargin;
}
//ȡ��֤��
bool CPlatformDataMgr_PositionDetailCombs::GetMargin(const PositionDetailCombKey& key,double& Margin,double& ExchMargin)
{
    CeasymutexGuard guard(m_mutex);
    PlatformStru_PositionDetailComb *pComb;
    if(!m_PositionDetailCombData.GetValue_Internal(pComb,key)) 
    {
        Margin=util::GetDoubleInvalidValue();
        ExchMargin=util::GetDoubleInvalidValue();
        return false;
    }
    else 
    {
        Margin=pComb->Margin;
        ExchMargin=pComb->ExchMargin;
        return true;
    }
}
//���ñ�֤��
bool CPlatformDataMgr_PositionDetailCombs::SetMargin(const PositionDetailCombKey& key,double Margin,double ExchMargin)
{
    CeasymutexGuard guard(m_mutex);
    PlatformStru_PositionDetailComb *pComb;
    if(!m_PositionDetailCombData.GetValue_Internal(pComb,key)) return false;
    pComb->Margin=Margin;
    pComb->ExchMargin=ExchMargin;
    return true;
}

//���ݳֲ���ϸ�ı�֤�����ݣ�������ϳֲ���ϸ�ı�֤�𣬲������Żݡ��ڽ����Żݼ���ǰ��һ�¡�
void CPlatformDataMgr_PositionDetailCombs::SetMarginByPositionDetail(const PlatformStru_PositionDetail& detail)
{
    CeasymutexGuard guard(m_mutex);

    PositionDetailKey detailkey(detail);
    map<PositionDetailKey,set<PositionDetailCombKey>>::const_iterator it=m_mapPositionDetailKey2Key.find(detailkey);
    if(it!=m_mapPositionDetailKey2Key.end()&&it->second.size()>0)
    {
        PlatformStru_PositionDetailComb *pComb;
        for(set<PositionDetailCombKey>::const_iterator it2=it->second.begin();it2!=it->second.end();it2++)
        {
            if(!m_PositionDetailCombData.GetValue_Internal(pComb,*it2))
                continue;
            pComb->MarginRateByMoney=detail.MarginRateByMoney;
            pComb->MarginRateByVolume=detail.MarginRateByVolume;

            if(pComb->TotalAmt==0) pComb->Margin=0;
            else if(detail.Volume==0) pComb->Margin=0;
            else if(util::isInvalidValue(detail.Margin)) pComb->Margin=util::GetDoubleInvalidValue();
            else pComb->Margin=detail.Margin*pComb->TotalAmt/detail.Volume;

            if(pComb->TotalAmt==0) pComb->ExchMargin=0;
            else if(detail.Volume==0) pComb->ExchMargin=0;
            else if(util::isInvalidValue(detail.ExchMargin)) pComb->ExchMargin=util::GetDoubleInvalidValue();
            else pComb->ExchMargin=detail.ExchMargin*pComb->TotalAmt/detail.Volume;

            pComb->UpdateSeq=m_PositionDetailCombData.NewUpdateSeq(pComb->FTID);
        }
    }
}


//��ղ�ѯ��ϳֲ���ϸ��map
void CPlatformDataMgr_PositionDetailCombs::ClearMapQryRltPositionDetailCombs(string& strQryInstrument)
{
    Lock_CPlatformDataMgr;
	m_PositionDetailCombData.WillQry_Internal(strQryInstrument);
    Unlock_CPlatformDataMgr;
}

//��ȡ��ѯ�ĺ�ԼID
void CPlatformDataMgr_PositionDetailCombs::GetQryPositionDetailCombInstrument(string& outQryInstrument)
{
    Lock_CPlatformDataMgr;
    m_PositionDetailCombData.GetQryInstrument(outQryInstrument);
    Unlock_CPlatformDataMgr;
}

void CPlatformDataMgr_PositionDetailCombs::clear(void)
{
    Lock_CPlatformDataMgr;
    m_PositionDetailCombData.Clear();
    m_mapCombInstrumentID2Key.clear();
    m_mapPositionCombKey2Key.clear();
    m_mapLegInstrumentID2CombInstrumentID.clear();
    m_mapComTradeID2Key.clear();
    m_mapPositionDetailKey2Key.clear();
    Unlock_CPlatformDataMgr;
}
bool CPlatformDataMgr_PositionDetailCombs::empty(void)
{
    Lock_CPlatformDataMgr;
    bool brlt=m_PositionDetailCombData.empty();
    Unlock_CPlatformDataMgr;
    return brlt;
}

//���ñ�֤���Żݵ����ɽ��޸���ϳֲ���ϸʱ������Ҫ�Żݵ�ĳһ�ȵĳֲ���ϸ�ı�֤��
bool CPlatformDataMgr_PositionDetailCombs::SetMargin(const PositionDetailCombKey& key,double Margin,double ExchMargin,double MarginRateByMoney,double MarginRateByVolume)
{
    Lock_CPlatformDataMgr;
    PlatformStru_PositionDetailComb* pDetailComb;
    bool brlt=m_PositionDetailCombData.GetValue_Internal(pDetailComb,key);
    if(brlt)
    {
        pDetailComb->Margin=Margin;
        pDetailComb->ExchMargin=ExchMargin;
        pDetailComb->MarginRateByMoney=MarginRateByMoney;
        pDetailComb->MarginRateByVolume=MarginRateByVolume;
        pDetailComb->UpdateSeq=m_PositionDetailCombData.NewUpdateSeq(pDetailComb->FTID);
    }
    Unlock_CPlatformDataMgr;
    return brlt;
}

//ȡLegID
bool CPlatformDataMgr_PositionDetailCombs::GetLegID(const PositionDetailCombKey& key,int& outLegID)
{
    Lock_CPlatformDataMgr;
    PlatformStru_PositionDetailComb* pDetailComb;
    bool brlt=m_PositionDetailCombData.GetValue_Internal(pDetailComb,key);
    if(brlt)
    {
        outLegID=pDetailComb->LegID;
    }
    Unlock_CPlatformDataMgr;
    return brlt;
}

//����һ�����Ⱥ�Լ����ȡ�����ϳֲֶ�Ӧ�ı�֤�𡣲�ѯ��֤���ʷ���ʱ��������ϳֲ���ϸ�ı�֤��������ϳֱֲ�֤��
void CPlatformDataMgr_PositionDetailCombs::GetPositionMarginByLegInstrument(const string& LegInstrumentID,map<PositionKey,double>& outMargin,map<PositionKey,double>& outExchMargin)
{
    Lock_CPlatformDataMgr;

    outMargin.clear();
    outExchMargin.clear();

    //��ÿһ����ص���Ϻ�Լ������key
    multimap<string,string>::const_iterator itlower=m_mapLegInstrumentID2CombInstrumentID.lower_bound(LegInstrumentID);
    multimap<string,string>::const_iterator itupper=m_mapLegInstrumentID2CombInstrumentID.upper_bound(LegInstrumentID);
    while(itlower!=itupper)
    {
        set<PositionDetailCombKey> keys;
        map<string,set<PositionDetailCombKey>>::iterator it1=m_mapCombInstrumentID2Key.find(itlower->second);
        if(it1!=m_mapCombInstrumentID2Key.end())

        //��keys���ÿ��ֵ�������ӦPositionKey, �Ͷ�Ӧ�ı�֤��
        for(set<PositionDetailCombKey>::const_iterator it=keys.begin();it!=keys.end();it++)
        {
            PlatformStru_PositionDetailComb *pdetailcomb;
            if(!m_PositionDetailCombData.GetValue_Internal(pdetailcomb,*it)) continue;

            PositionKey positionkey(it->Account,itlower->second.c_str(),it->Direction==THOST_FTDC_D_Buy?THOST_FTDC_PD_Long:THOST_FTDC_PD_Short,it->HedgeFlag);
            if(pdetailcomb->LegID==0) positionkey.PosiDirection=it->Direction==THOST_FTDC_D_Buy?THOST_FTDC_PD_Long:THOST_FTDC_PD_Short;
            else if(pdetailcomb->LegID==1) positionkey.PosiDirection=it->Direction==THOST_FTDC_D_Buy?THOST_FTDC_PD_Short:THOST_FTDC_PD_Long;
            else continue;

            map<PositionKey,double>::iterator itout;
            
            itout=outMargin.find(positionkey);
            if(itout==outMargin.end())
            {
                outMargin.insert(make_pair(positionkey,0));
                itout=outMargin.find(positionkey);
            }
            assert(itout!=outMargin.end());
            if(!util::isInvalidValue(itout->second))
            {
                if(!util::isInvalidValue(pdetailcomb->Margin)) itout->second+=pdetailcomb->Margin;
                else itout->second=util::GetDoubleInvalidValue();
            }

            itout=outExchMargin.find(positionkey);
            if(itout==outExchMargin.end())
            {
                outExchMargin.insert(make_pair(positionkey,0));
                itout=outExchMargin.find(positionkey);
            }
            assert(itout!=outExchMargin.end());
            if(itout!=outExchMargin.end()&&!util::isInvalidValue(itout->second))
            {
                if(!util::isInvalidValue(pdetailcomb->ExchMargin)) itout->second+=pdetailcomb->ExchMargin;
                else itout->second=util::GetDoubleInvalidValue();
            }
        }

        itlower++;
    }

    Unlock_CPlatformDataMgr;
}

//��ȡһ����ϳֲֶ�Ӧ��������ϳֲ���ϸkeys
void CPlatformDataMgr_PositionDetailCombs::GetPositionDetailCombKeysByPositionCombKey(const PositionKey& positionkey,vector<PositionDetailCombKey>& outkeys)
{
    Lock_CPlatformDataMgr;
    outkeys.clear();
    multimap<PositionKey,PositionDetailCombKey>::const_iterator itlower=m_mapPositionCombKey2Key.lower_bound(positionkey);
    multimap<PositionKey,PositionDetailCombKey>::const_iterator itupper=m_mapPositionCombKey2Key.upper_bound(positionkey);
    while(itlower!=itupper)
    {
        outkeys.push_back(itlower->second);
        itlower++;
    }
    Unlock_CPlatformDataMgr;
}

//ȡ��ϳֲ���ϸ
bool CPlatformDataMgr_PositionDetailCombs::GetPositionDetailComb(const PositionDetailCombKey& detailcombkey, PlatformStru_PositionDetailComb& outData)
{
    CeasymutexGuard guard(m_mutex);
    PlatformStru_PositionDetailComb *pdetailcomb;
    bool brlt=m_PositionDetailCombData.GetValue_Internal(pdetailcomb,detailcombkey);
    if(brlt) outData=*pdetailcomb;
    return brlt;
}
//ȡ��Ϻ�ԼID
bool CPlatformDataMgr_PositionDetailCombs::GetCombInstrumentID(const PositionDetailCombKey& detailcombkey,string& CombInstrumentID)
{
    CeasymutexGuard guard(m_mutex);
    PlatformStru_PositionDetailComb *pdetailcomb;
    bool brlt=m_PositionDetailCombData.GetValue_Internal(pdetailcomb,detailcombkey);
    if(brlt) CombInstrumentID=string(pdetailcomb->CombInstrumentID);
    return brlt;
}

//ȡ��Ϻ�Լ��Ӧ��������ϳֲ���ϸkey�ļ���
void CPlatformDataMgr_PositionDetailCombs::GetKeysByCombInstrumentID(const string& CombInstrumentID,set<PositionDetailCombKey>& keys)
{
    map<string,set<PositionDetailCombKey>>::const_iterator it=m_mapCombInstrumentID2Key.find(CombInstrumentID);
    if(it!=m_mapCombInstrumentID2Key.end())
        keys=it->second;
    else keys.clear();
}

//�����ճ��ֲ���ϸ����ȡ�ճ���ϳֲ���ϸ������֣����Ч
void CPlatformDataMgr_PositionDetailCombs::GetStaticPositionDetailComb_CZCE(CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& StaticPositionDetail,
                                                                            vector<PlatformStru_PositionDetailComb>& vecPositionDetailCombs)
{
    CeasymutexGuard guard(m_mutex);

    vecPositionDetailCombs.clear();
    vector<long> FTIDs;
    PlatformStru_PositionDetailComb detailcomb;

    m_PositionDetailCombData.GetAllFTID_Internal(FTIDs);
    for(int i=0;i<(int)FTIDs.size();i++)
    {
        if(m_PositionDetailCombData.GetValue3_Internal(detailcomb,FTIDs[i])&&
            !detailcomb.bToday)
        {
            PositionDetailKey detailkey(detailcomb.Account,detailcomb.InstrumentID,detailcomb.OpenDate,detailcomb.TradeID,detailcomb.Direction,THOST_FTDC_TRDT_CombinationDerived,detailcomb.HedgeFlag);
            PlatformStru_PositionDetail *pdetail;
            if(StaticPositionDetail.GetValue_Internal(pdetail,detailkey))
            {
                detailcomb.TotalAmt=pdetail->Volume;
                vecPositionDetailCombs.push_back(detailcomb);
            }
        }
    }
}
//ȡ����δƽ�յ�key
void CPlatformDataMgr_PositionDetailCombs::GetAllKeys(vector<PositionDetailCombKey>& outData)
{
    CeasymutexGuard guard(m_mutex);
    m_PositionDetailCombData.GetAllKeys(outData);
    vector<PositionDetailCombKey>::iterator it=outData.begin();
    PlatformStru_PositionDetailComb *pdetailcomb;
    while(it!=outData.end())
    {
        if(!m_PositionDetailCombData.GetValue_Internal(pdetailcomb,*it)||pdetailcomb->TotalAmt==0)
            it=outData.erase(it);
        else it++;
    }
}
