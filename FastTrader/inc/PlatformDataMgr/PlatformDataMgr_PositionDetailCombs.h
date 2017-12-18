/*
    ���ڱ����ά��CTP����ϳֲ���ϸ����
    ע�⣺���Ȳ�һ����һ��һ�Ĺ�ϵ��������a&b, ������a1b1a1b1�ĳɽ�˳��Ҳ������a1a1b2�ĳɽ�˳��
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <map>
#include <string>
#include <set>
using std::string;
using std::map;
using std::vector;
using std::set;

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Trades.h"
#include "PlatformDataMgr_Orders.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CPlatformDataMgr_PositionDetailCombs
{
public:
    CPlatformDataMgr_PositionDetailCombs(
        volatile long& baseFTID,
        volatile long& baseUpdateSeq,
        CPlatformDataMgr_Instruments& refInstruments,
        CPlatformDataMgr_Quots& refQuots,
        CPlatformDataMgr_Trades& refTrades,
        CPlatformDataMgr_Orders& refOrders);
    ~CPlatformDataMgr_PositionDetailCombs(void);

    //��ѯ�ر�����
    void QryPositionDetailCombReach( 
        PlatformStru_PositionDetailComb& PositionDetailCombInfo,
        int ErrID,
        bool bIsLast);

    //�ӳɽ���¼������ϳֲ���ϸ�������ڳֲ���ϸ�ı仯�������ڳֲ���ϸ��������ٸ��³ֲ�
    //ComTradeID2CloseVolume:ƽ���������λʱ����ƽ������ϳɽ�ID�Ͷ�Ӧƽ�����Ĺ�ϵ
    void Update_RtnTrade(
        const PlatformStru_TradeInfo& TradeInfo,
        const map<PositionDetailKey,int>& mapCloseDeltaVolume,
        const map<string,int>& ComTradeID2CloseVolume,
        map<PositionDetailCombKey,PlatformStru_PositionDetailComb>&ModifiedPositionDetailCombs);



    //���ݳֲ���ϸkey��ȡ��Ӧ����ϳֲ���ϸkey�Ͷ�����ϳֲ���ϸkey
    //����ֵ��map<string,pair<set<PositionDetailCombKey>,set<PositionDetailCombKey>>>, ComTradeID -> pair<leg1keys,leg2keys>
    void GetCombKeysByPositionDetailKey2(const PositionDetailKey& detailkey,map<string,pair<set<PositionDetailCombKey>,set<PositionDetailCombKey>>>& outkeys,bool bFilterZeroVolume);
    //���ݳֲ���ϸkey��ȡ��Ӧ����ϳֲ���ϸkey
    void GetCombKeysByPositionDetailKey(const PositionDetailKey& detailkey,set<PositionDetailCombKey>& outkeys,bool bFilterZeroVolume);
    void GetCombKeysByComTradeID(const string& ComTradeID,set<PositionDetailCombKey>& outkeys,bool bFilterZeroVolume);
    void GetCombKeysByComTradeID2(const string& ComTradeID,set<PositionDetailCombKey>& leg1keys,set<PositionDetailCombKey>& leg2keys,bool bFilterZeroVolume);
    void GetComTradeIDsByPositionDetailKeys(const set<PositionDetailKey>& detailkeys,set<string>& outdatas,bool bFilterZeroVolume);

    //ȡ��֤��ʧ�ܷ�����Чֵ
    double GetExchMargin(const PositionDetailCombKey& key);
    bool GetMargin(const PositionDetailCombKey& key,double& Margin,double& ExchMargin);
    //���ñ�֤��
    bool SetMargin(const PositionDetailCombKey& key,double Margin,double ExchMargin);

    //���ݳֲ���ϸ�ı�֤�����ݣ�������ϳֲ���ϸ�ı�֤�𣬲������Żݡ��ڽ����Żݼ���ǰ��һ�¡�
    void SetMarginByPositionDetail(const PlatformStru_PositionDetail& detail);

    //��ղ�ѯ��ϳֲ���ϸ��map
    void ClearMapQryRltPositionDetailCombs(string& strQryInstrument);

    //��ȡ��ѯ�ĺ�ԼID
    void GetQryPositionDetailCombInstrument(string& outQryInstrument);

    //�����������
    void clear(void);

    //���ñ�֤�𡣳ɽ��޸���ϳֲ���ϸʱ������Ҫ�Żݵ�ĳһ�ȵĳֲ���ϸ�ı�֤��
    bool SetMargin(const PositionDetailCombKey& key,double Margin,double ExchMargin,double MarginRateByMoney,double MarginRateByVolume);

    //ȡLegID
    bool GetLegID(const PositionDetailCombKey& key,int& outLegID);

    //����һ�����Ⱥ�Լ����ȡ�����ϳֲֶ�Ӧ�ı�֤�𡣲�ѯ��֤���ʷ���ʱ��������ϳֲ���ϸ�ı�֤��������ϳֱֲ�֤��
    void GetPositionMarginByLegInstrument(const string& LegInstrumentID,map<PositionKey,double>& outMargin,map<PositionKey,double>& outExchMargin);

    //��ȡһ����ϳֲֶ�Ӧ��������ϳֲ���ϸkeys
    void GetPositionDetailCombKeysByPositionCombKey(const PositionKey& positionkey,vector<PositionDetailCombKey>& outkeys);

    //ȡ��ϳֲ���ϸ
    bool GetPositionDetailComb(const PositionDetailCombKey& detailcombkey, PlatformStru_PositionDetailComb& outData);
    //ȡ��Ϻ�ԼID
    bool GetCombInstrumentID(const PositionDetailCombKey& detailcombkey,string& CombInstrumentID);

    //ȡ��Ϻ�Լ��Ӧ��������ϳֲ���ϸkey�ļ���
    void GetKeysByCombInstrumentID(const string& CombInstrumentID,set<PositionDetailCombKey>& keys);

    //�����ճ��ֲ���ϸ����ȡ�ճ���ϳֲ���ϸ������֣����Ч
    void GetStaticPositionDetailComb_CZCE(CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& StaticPositionDetail,
        vector<PlatformStru_PositionDetailComb>& vecPositionDetailCombs);

    //ȡ����δƽ�յ�key
    void GetAllKeys(vector<PositionDetailCombKey>& outData);


private:
    //����ϳɽ�������ϳֲ���ϸ
    void CreatePositionDetailCombFromOpenTrade_Internal(const PlatformStru_TradeInfo &TradeInfo,map<PositionDetailCombKey,PlatformStru_PositionDetailComb>& ModifiedPositionDetailCombs);


private:
    Ceasymutex				        m_mutex;
    //�ж��Ƿ�Ϊ��
    bool empty(void);

    CPlatformDataMgr_Instruments    &m_RefInstruments;
    CPlatformDataMgr_Quots          &m_RefQuots;
    CPlatformDataMgr_Trades         &m_RefTrades;
    CPlatformDataMgr_Orders         &m_RefOrders;


    //----------���������ݼ�----------
    CPlatformDataSetList<PositionDetailCombKey,PlatformStru_PositionDetailComb> m_PositionDetailCombData;

    //���Ⱥ�Լ����Ϻ�Լ֮��Ĺ�ϵ
    multimap<string,string>                             m_mapLegInstrumentID2CombInstrumentID;

    //��Ϻ�Լ����ϳֲ���ϸKey֮��Ĺ�ϵ��key�������
    map<string,set<PositionDetailCombKey>>              m_mapCombInstrumentID2Key;

    //��ϳֲ�key����ϳֲ���ϸkey֮��Ĺ�ϵ
    multimap<PositionKey,PositionDetailCombKey>         m_mapPositionCombKey2Key;

    //ComTradeID��Key֮��Ĺ�ϵ
    multimap<string,PositionDetailCombKey>              m_mapComTradeID2Key;

    //�ֲ���ϸKey�����Key֮��Ĺ�ϵ�������һ��һ����ֿ�����һ�Զ�
    map<PositionDetailKey,set<PositionDetailCombKey>>   m_mapPositionDetailKey2Key;
};

#ifdef WIN32
#pragma managed(pop)
#endif 

