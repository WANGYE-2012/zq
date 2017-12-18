/*
    ���ڱ����ά��CTP�ĳɽ�����
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <map>
#include <set>
#include <string>
using std::vector;
using std::map;
using std::multimap;
using std::set;
using std::string;

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Rates.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 


class CPlatformDataMgr_Trades
{
public:
    CPlatformDataMgr_Trades(
        volatile long& baseFTID,
        volatile long& baseUpdateSeq,
        CPlatformDataMgr_Instruments& refInstruments,
        CPlatformDataMgr_Rates& refRates);
    ~CPlatformDataMgr_Trades(void);


    //��ѯ�ĳɽ��ر�����
    void QryTradeReach( PlatformStru_TradeInfo& TradeInfo,int ErrID,bool bIsLast);

    //�µĳɽ��ر�����
    void NewTradeReady( PlatformStru_TradeInfo& TradeInfo,const PlatformStru_InstrumentInfo& InstrumentInfo);

    //���³ɽ���¼��ƽ������ƽ��ӯ�������¼��������ѡ�
    void UpdateTradeCloseTodayVolume_CloseProfit_RecalcuCommission( long TradeFTID,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        int CloseTodayVolume,
        double CloseProfitByDate,
        double CloseProfitByTrade,
        double& outCommission);

    //��ȡ�������ʺ󣬸������гɽ��е������ѣ�����ֵ��ʾ����������Ƿ��иı䣬�Դ˾����Ƿ���Ҫ�����ʽ���塣
    bool UpdateTradeCommission_RspQryCommissionRate(const string& strInstrumentID,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        const PlatformStru_InstrumentCommissionRate& CommissionRate);


    //��ȡȫ���ɽ��������������������
    int GetAllTrades(vector<PlatformStru_TradeInfo>& outData);
    //��ȡָ���ɽ���Ϣ
    bool GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData);
    bool GetTradeInfo(long FTID, PlatformStru_TradeInfo& outData);
    //��ȡȫ���ɽ���FTID������ԼID���ࡣͬһ����Լ��FTID�������
    void GetFTIDsByInstrument(map<string,vector<long>>& outFTIDs);
    //��ȡָ����Լ�ĳɽ��������������������
    int GetTradesOfInstrument(const string& strInstrument,vector<PlatformStru_TradeInfo>& outData);
    int GetTradeBriefsOfInstrument(const string& strInstrument,vector<PlatformStru_TradeInfoBrief>& outData);
    //��ȡ����������֮��
    double GetTotalCommission(void);
    //��ȡ�ֲֽ���������
    double GetCommission(const string& strInstrument,const string& strAccount,char HedgeFlag,char Direction);

    //׼����ѯ�ɽ�ǰ���Ҫ��ѯ�ĺ�Լ��
    void ClearMapQryRltTrades();


    //��ȡָ��TradeID�ĳɽ���¼Key��
    void GetTradeKeysOfTradeID(const string& strTradeID,vector<TradeKey>& outData);

    //��ϳɽ�ʱ����ȡOrderSysIDһ�µĳɽ���ϸ���ϡ�Volume>0
    void GetTradeKeysByTradeComb_OrderSysID(const TradeKey& tradekey,set<TradeKey>& outData);
    void GetTradeKeysByTradeComb_OrderSysID2(const TradeKey& tradekey,map<string,TradeKey>& outData);

    //��ȡָ���ɽ�key��Ӧ��Volume
    bool GetTradeVolume(const TradeKey& tradekey,int& Volume);

    //���ݳɽ���¼����ȡ�ɽ�ͳ��key�ļ���
    void GetTradeTotalKeysByTrades(set<TradeTotalKey>& outData);
    //���ݳɽ���¼����ȡָ��key�ĳɽ�ͳ������
    bool GetTradeTotalValue(const TradeTotalKey& TotalKey,PlatformStru_TradeTotalInfo& TradeTotalInfo);

    //�����������
    void clear(void);
    //�ж��Ƿ�Ϊ��
    bool empty(void);


    ///��ȡ���ڵ���MinFTID��ȫ�����ݣ������������������������������С�m_bUpdateSeq2FTIDΪtrueʱ��Ч
    int GetAll_GE_UpdateSeq(long MinUpdateSeq,vector<PlatformStru_TradeInfo>& outDatas);

private:
    //����ɽ������ѡ�ע�⣬���ڴ���֣�̣�ʹ��ÿ����Լʣ���ƽ���Ż��������
    void CalcuTradeCommission_Internal(PlatformStru_TradeInfo& TradeInfo,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        const PlatformStru_InstrumentCommissionRate& CommissionRate);

private:
    Ceasymutex				        m_mutex;

    CPlatformDataMgr_Instruments    &m_RefInstruments;
    CPlatformDataMgr_Rates          &m_RefRates;

    //----------�����ǳɽ����ݼ�----------
    CPlatformDataSetList<TradeKey,PlatformStru_TradeInfo> m_TradeData;

	//OrderSysID �� TradeKey ��ӳ���ϵ����Ҫ����������ϳɽ�����Բ�ѯ
    multimap<string, TradeKey>      m_mapOrderSysID2TradeKey;
	//TradeID �� TradeKey ��ӳ���ϵ����Ҫ����������ϳɽ�����Բ�ѯ
    multimap<string, TradeKey>      m_mapTradeID2TradeKey;

    //ÿ����Լʣ���ƽ���Ż������������Դ���֣�̡���������������ƽ���Żݣ����ǰ�ƽ���ĳֲ���ϸ�Ƿ������жϣ����ǰ��Ƿ��н�ֿ�ƽ���жϡ�
    //      ���磬�����1�ֽ��1�֣�ƽ1�֣�ƽ������֣����������н�֣�����������Ż�
    //�����н�ƽ���ĳֲ���ϸ�Ƿ������ж�
    //�ֶ�ͷ�Ϳ�ͷ
    //map<string,int>                 m_mapInstrument2LongVolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE;
    //map<string,int>                 m_mapInstrument2ShortVolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE;
    map<PositionKey,int>            m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE;

};

#ifdef WIN32
#pragma managed(pop)
#endif 
