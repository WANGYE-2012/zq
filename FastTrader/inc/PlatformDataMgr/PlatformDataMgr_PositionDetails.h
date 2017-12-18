/*
    ���ڱ����ά��CTP�ĳֲ���ϸ����
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <set>
#include <string>
#include <map>
using std::string;
using std::set;
using std::vector;
using std::map;
using std::multimap;

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Rates.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Trades.h"
#include "PlatformDataMgr_Orders.h"
#include "PlatformDataMgr_PositionDetailCombs.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 



class CPlatformDataMgr_PositionDetails
{
public:
    CPlatformDataMgr_PositionDetails(
        volatile long& baseFTID,
        volatile long& baseUpdateSeq,
        CPlatformDataMgr_Instruments& refInstruments,
        CPlatformDataMgr_Rates& refRates,
        CPlatformDataMgr_Quots& refQuots,
        CPlatformDataMgr_Trades& refTrades,
        CPlatformDataMgr_Orders& refOrders,
        CPlatformDataMgr_PositionDetailCombs& refPositionDetailCombs,
        int DiscountCZCE_Comb=3,
        int DiscountDCE_Comb=0,
        int UnwindPrioritiesCZCE_Comb=1,
        int UnwindPrioritiesDCE_Comb=0,
        int PriceForMarginOfTodayPositionCZCE_Comb=0,
        int PriceForMarginOfTodayPositionDCE_Comb=1,
        bool bUseSettlementPriceToCalculateProfitAndMarginAsPossible=false);

    ~CPlatformDataMgr_PositionDetails(void);

    //��ѯ�ر�����
    void QryPositionDetailReach( PlatformStru_PositionDetail& PositionDetailInfo,int ErrID,bool bIsLast);

    //�ӳɽ���¼���³ֲ���ϸ�����޸�TradeInfo��ƽ������ƽ��ӯ��
    //ModifiedPositionDetails�����ܵ�Ӱ��ĳֲ���ϸ��¼��n*2+0��ԭ�������ݣ�n*2+1��������
    //ComTradeID2CloseVolume:ƽ���������λʱ����ƽ������ϳɽ�ID�Ͷ�Ӧƽ�����Ĺ�ϵ
    void UpdatePositionDetail_RtnTrade(
        PlatformStru_TradeInfo& TradeInfo,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
        map<string,int>& ComTradeID2CloseVolume);

    //���³ֲ���ϸ��¼�ĳֲ�ӯ��������ӯ��������ۡ������ۡ��ּۣ��������飬�õ�������ʱ��һ��
    void UpdatePositionDetail_RtnDepthMarketData(
        const PlatformStru_DepthMarketData& QuotData,
        const PlatformStru_InstrumentInfo& InstrumentInfo);

    //����ʱ���ý���۸��³ֲ���ϸ��¼�ĳֲ�ӯ��(����)������ӯ��(���)����֤�𡢽���ۡ������ۡ��ּ�
    void UpdatePositionDetail_OnSettlement();

    //�����ճ��ֲ���ϸ�����ڳɽ���¼�����¼���ɽ���ƽ������ƽ��ӯ����������
    //��ʼ����ѯ��ɽ��ͳֲ���ϸ�����
    void ReCalcuTradeInfo_OnInitQryCompleted(void);

    //����б仯�ĳֲ���ϸ��������ϳֲ���ϸ�ı�֤���Żݡ���Ҫ�ڸ��³ֲ���ϸ�͸�����ϳֲ���ϸ����С����ݱ�֤���Ż����ý��м��㣬������Ӧ�ֲ���ϸ����ϳֲ���ϸ�ı�֤���ֶΣ�(Ϊ��֤��©�������ȶ��㣬�е��ظ�)
    void CalcuPositionDetailMarginDiscount_RtnTrade_RspQryRate_OnSettlement(
        map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
        map<PositionDetailCombKey,PlatformStru_PositionDetailComb>& ModifiedPositionDetailCombs);

    //����б仯�ĳֲ���ϸ������֤���������õ���ϳֲ���ϸ�С���ʱδ�����Ż�
    void SetMarginOfPositionDetailComb(map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails);

    //���³ֲ���ϸ��¼�ı�֤��, ��֤����, ��ѯ��֤������Ӧʱ����, ֻ�Ա�֤��Ϊ��Чֵ����Ŀ���д���. ���ڲ�ѯ�����ĳֲ���ϸ, ��֤���������Ѿ����ˣ���������. 
    void UpdatePositionDetailByMarginRate_RspQryRate(
        const PlatformStru_InstrumentMarginRate& MarginRate,
        const PlatformStru_InstrumentInfo& InstrumentInfo);


    //ƽ�ֳɽ�ʱ�����ݳֲ���ϸ�ı仯��ȡ�ֲ���ϸ��λ�仯ֵ
    void GetCloseDeltaVolume(const map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,map<PositionDetailKey,int>& mapCloseDeltaVolume);

    //ȡ���ֲ���ϸ�еļ�¼��������ƽ���ļ�¼
    int GetRecordCountOfPositionDetail(void);

    //����keyȡvalue
    bool GetPositionDetail(const PositionDetailKey& key,PlatformStru_PositionDetail& value);

    //ȡ��Ч�ֲ���ϸ(Volume>0)�����ؼ�¼������lastseq�������ݸ��µ����к�
    int GetPositionDetails(vector<PlatformStru_PositionDetail>& outData,long& lastseq);
    
    //��ȡָ����Լ��Ӧ����Ч�ֲ���ϸ��¼(Volume>0)������0���ɹ�������������(�����ǿ�����)��1: �ɹ���������û�и��£�����������
    //���lastseq��ԭʼ�����е�updateseqһ�£���ʾ����û�б仯������1�������������ݣ�
    //���򷵻�0��setFTID��strInstrument��Ӧ��ȫ����Ч(��λ������)��¼��FTID�������ж���û����Ҫɾ�����vecValue���ض�Ӧ����Ч��������
    int GetPositionDetails3(
        const std::string& strInstrument,
        std::set<long>& setFTID,
        std::vector<PlatformStru_PositionDetail>& vecValue,
        long& lastseq);

    //ȡȫ���ֲ���ϸ����������Ϊ��ļ�¼��lastseq�������ݸ��µ����кš�
    void GetPositionDetails4(vector<PlatformStru_PositionDetail>& outData,long& lastseq);

    //�Ƿ���ָ����Լ����Ч�ֲ���ϸ��¼
    bool HavePositionDetail(const string& strInstrumentID);

    //��ղ�ѯ�ֲ���ϸ��map
    void ClearMapQryRltPositionDetails(const string& strQryInstrument);

    //��ȡ��ѯ�ĺ�ԼID
    void GetQryPositionDetailInstrument(string& outQryInstrument);

    //����ָ��[��Լ-�˺�-����-Ͷ��]�ֲ���ϸ�ı�֤��֮��
    void GetMarginByInstrument(
        const string& strInstrumentID,
        const string& strAccount,
        char Direction,
        char HedgeFlag,
        double& outMargin,
        double& outExchMargin);

    //�Ƿ�����ϳֲ���ϸ����, �ж�m_bHaveComb
    bool HaveCombPositionDetail(void);

    //��ȡƽ��ʱ��ɢ�����ĳֲ���ϸ��ԼiD
    void GetDerivedInstrumentID_OnCloseTrade(set<string>& InstrumentIDs);

    //��ȡ��ѯ����ĺ�Լ����
    void GetInstrumentSetOfQryRlt(set<std::string>& outData);

    //�����������
    void clear(void);

    //�ж��Ƿ�Ϊ��
    bool empty(void);

    //��m_mapOpenTime2Key��ȡ��ָ��ʱ�䷶Χ��PositionDetail
    //ʱ����time_t������LowerOpenTime<=���OpenTimeUTC<UpperOpenTime����ֲֳ���ϸ��OpenTimeUTCΪ����������ʱ�������׼ȷ��
    //�ֲ���ϸ��OpenTimeUTC=CTools_Ansi::Get_time_t_utc_from_strTime(OpenDate,OpenTime)
    void GetPositionDetailsInOpenTimeScope(int LowerOpenTime,int UpperOpenTime,vector<PlatformStru_PositionDetail>& outPositionDetails);

    //��ȡ���гֲ���ϸ��¼��FTID
    void GetAllFTID(set<long>& outFTIDs);

    //����Ϻ�ԼID���õ���Ӧ�ĳֲ���ϸ��
    void SetCombInstrumentIDs(const vector<PositionDetailCombKey>& combdetailkeys);

    //���ֳɽ�ʱ����Ϻ�ԼID���õ���Ӧ�ĳֲ���ϸ�С�������ϳֲ���ϸ�ı仯
    void SetCombInstrumentID_OpenTrade(const map<PositionDetailCombKey,PlatformStru_PositionDetailComb> ModifiedPositionDetailCombs);

    static void Convert_PositionDetailCombKey_to_PositionDetailKey(const PositionDetailCombKey& combkey,PositionDetailKey& detailkey);

    //������Ȩ���Ȩ�������ʹ�õļ۸�����. 0: ���ɽ��ۼ���(ȱʡ)��1: �������ۼ���
    void SetPriceForRoyalty_Option(int PriceForRoyalty_Option) { m_PriceForRoyalty_Option=PriceForRoyalty_Option; }

    //��������н���۵Ļ����Ƿ�ʹ�ý���ۼ���ӯ���ͱ�֤��
    void SetbUseSettlementPriceToCalculateProfitAndMarginAsPossible(bool bUse)
    {
        m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible=bUse;
    }

private:

    //�ӳɽ��ر�������سֲ���ϸ��λ�ı仯��ע�⣬ֻ���²�λ������������ֵ
    //PositionDetailData�����ݼ���������m_PositionDetailData,Ҳ��������ʱ���ݼ�(Ϊ��ƥ��ɽ��ͳֲ���ϸ)
    //bCareCombOppoDetail:��ʾ�ֲ���ϸ���������, �Ƿ������ֲֳ���ϸ���д�ɢ�����������������Ҫ���ڳ�ʼ����ѯ��������������������֣�����Ͳ���Ҫ
    //ComTradeID2CloseVolume:ƽ���������λʱ����ƽ������ϳɽ�ID�Ͷ�Ӧƽ�����Ĺ�ϵ
    void UpdatePositionDetailVolumeOnTrade_Internal(
        PlatformStru_TradeInfo& TradeInfo,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
        map<string,int>& ComTradeID2CloseVolume,
        CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& PositionDetailData,
        CPlatformDataMgr_PositionDetailCombs& PositionDetailCombData,
        bool bCareCombOppoDetail);

    //��̬���º�Լ��Ӧ�ĳֲ���ϸ��¼���ּۡ������ۡ�����ۡ��ֲ�ӯ��(����)������ӯ��(���)����������
    //���������bForceCalculateΪfalse����ʾ���ڼ۸�䶯ʱ���£��������Ϊtrue
    //����ӯ������ʹ�õļ۸�˳��Ϊ������� > ���¼� > ������
    void UpdatePositionDetailByQuot2_Internal(
        const PlatformStru_DepthMarketData& QuotData,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        bool bForceCalculate);

    //��̬���³ֲ���ϸ��¼���ּۡ������ۡ�����ۡ��ֲ�ӯ��(����)������ӯ��(���)����������
    //���������bForceCalculateΪfalse����ʾ���ڼ۸�䶯ʱ���£��������Ϊtrue
    //����ӯ������ʹ�õļ۸�˳��Ϊ������� > ���¼� > ������
    void UpdatePositionDetailByQuot_Internal(
        const PlatformStru_DepthMarketData& QuotData,
        const PlatformStru_InstrumentInfo InstrumentInfo,
        bool bForceCalculate,
        PlatformStru_PositionDetail& detail,
        bool& bUpdate);

    //���³ֲ���ϸ��¼�ı�֤�𡢱�֤���ʡ���������֤�𣬳ɽ��޸ĳֲ���ϸ�Ͳ�ѯ��֤������Ӧʱ���ã�ֻ�Խ�ִ���
    //����н���ۣ�������ۼ��㣻���򣬰����̼ۻ������ۼ���(������ϳֲ���ϸ�����Լ�����������)
    //����ϳֲ���ϸ�����ﲻ���Ǳ�֤���Ż�
    void CalculateMargin_Internal(
        const PlatformStru_InstrumentMarginRate& MarginRate,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        PlatformStru_PositionDetail& PositionDetail,
        bool& bUpdate);

    //������Ȩ��Լ�ֲ���ϸ��¼�ı�֤��, ��������֤��, �ɽ��޸ĳֲ���ϸ�Ͳ�ѯ��֤������Ӧʱ����
    //  ÿ�ֱ�֤��=MAX(Ȩ���� + Ͷ������Ȩ��Լ��֤�𲻱䲿��, Ͷ������Ȩ��Լ��С��֤��)
    //  ���Ȩ���� = if(��������[��Ȩ��Լ���̼������۵Ľϴ�ֵ]��[��Ȩ��Լ�����]) �� ��Ȩ��Լ����
    //  �²�Ȩ���� = �۸�������/�ɽ��ۣ���OptionRoyaltyPriceType����*��Ȩ��Լ����
    void CalculateMargin_Option_Internal(
        const PlatformStru_InstrumentMarginRate& MarginRate,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        PlatformStru_PositionDetail& PositionDetail,
        bool& bUpdate);


    //�жϳֲ���ϸ�ǲ���(ƽ��)�ɽ��Ķ��ֲֳ�
    bool IsPositionDetailForCloseTrade_IgnoreTradeType(
        const PlatformStru_PositionDetail& PositionDetail,
        const PlatformStru_TradeInfo& TradeInfo);

    ////��ȡĳһ��(���)�ֲ���ϸ�Ķ��ֲֳ���ϸ��¼��ע�⣬ֻ���������Խ�ֻ����
    ////���ڳɽ����³ֲ���ϸʱ��ɢ��ϳֲֵļ��㡣��ȡʱ�õ���ϳֲ���ϸ������
    //bool GetOpponentPositionDetailKey_Comb_Internal(
    //    bool bToday,
    //    const PlatformStru_PositionDetail& OriPositionDetail,
    //    PositionDetailKey& OppoPositionDetailKey,
    //    CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& PositionDetailData);



    //��m_mapOpenTime2Key��ȡ��ָ���˺ţ�ָ��ʱ�䷶Χ��key
    void GetKeysFrommapOpenTime2Key_Internal(int LowerOpenTime,int UpperOpenTime,vector<PositionDetailKey>& outkeys);

    //��ȡ�ճ��ֲ���ϸ
    void GetStaticPositionDetail_Internal(CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& StaticPositionDetail);

    //�ӳɽ���¼���³ֲ���ϸ
    //����ƽ���ĳֲ���ϸ�ļ�¼�������ں������㡣ModifiedPositionDetails�����ܵ�Ӱ��ĳֲ���ϸ��¼��n*2+0��ԭ�������ݣ�n*2+1��������
    //���޸�TradeInfo��ƽ������ƽ��ӯ��
    //bPartCalcu:ֻ����ƽ������ƽ��ӯ���������㱣֤�𡢳ֲ�ӯ����
    //ComTradeID2CloseVolume:ƽ���������λʱ����ƽ������ϳɽ�ID�Ͷ�Ӧƽ�����Ĺ�ϵ
    void UpdatePositionDetail_RtnTrade_Internal(
        PlatformStru_TradeInfo& TradeInfo,
        bool bPartCalcu,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
        map<string,int>& ComTradeID2CloseVolume,
        CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& PositionDetails);

private:
    Ceasymutex				                m_mutex;

    CPlatformDataMgr_Instruments            &m_RefInstruments;
    CPlatformDataMgr_Rates                  &m_RefRates;
    CPlatformDataMgr_Quots                  &m_RefQuots;
    CPlatformDataMgr_Trades                 &m_RefTrades;
    CPlatformDataMgr_Orders                 &m_RefOrders;
    CPlatformDataMgr_PositionDetailCombs    &m_RefPositionDetailCombs;

    //ƽ��˳��: 0: ͳһ�ȿ���ƽ��1: �ȵ�һ������ȿ���ƽ
    int                                     m_UnwindPrioritiesCZCE_Comb;
    int                                     m_UnwindPrioritiesDCE_Comb;

    //������ϵ���ֱ�֤��ʹ�õļ۸�: 0: ���ɽ��ۼ���(ȱʡ)��1: �������ۼ���
    int                                     m_PriceForMarginOfTodayPositionCZCE_Comb;
    int                                     m_PriceForMarginOfTodayPositionDCE_Comb;
    int                                     m_PriceForRoyalty_Option;

    //��ϵ��Żݲ��ԣ�0-���Żݣ�1-��һ���Żݣ�2-�ڶ����Żݣ�3-����֤��Ƚϸߵ�һ����ȡ(ȱʡ)
    int                                     m_DiscountDCE_Comb;      
    int                                     m_DiscountCZCE_Comb;

    //----------���������ݼ�----------
    CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail> m_PositionDetailData;

    //map: OpenTimeUTC -> PositionDetailKey�����ڿ��ٲ��ҳֲ�ʱ����ڶ��ٵ���ϸ
    multimap<int,PositionDetailKey>         m_mapOpenTime2Key;  

    //�Ƿ�����������ĳֲ���ϸ��¼ 
    bool                                    m_bHaveComb;       

    //����н���۵Ļ����Ƿ�ʹ�ý���ۼ���ӯ���ͱ�֤��
    bool                                    m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible;

    //��һ��ƽ�ֳɽ�ʱ���Ƿ��б���ɢ����������ͨ�ֲ���ϸ�ĺ�ԼID����GUI�и��³ֲ���ϸʱ����Ҫ���´�������Լ�ĳֲ���ϸ
    set<string>                             m_DerivedInstrumentIDs_OnCloseTrade;


};

#ifdef WIN32
#pragma managed(pop)
#endif 
