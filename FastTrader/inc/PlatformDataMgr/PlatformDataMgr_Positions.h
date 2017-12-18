/*
    ���ڱ����ά��CTP�ĳֲ�����
    ������Ϻ�Լ�ĳֲ֣�ֻ��������ȳֲ����ݣ���������ϳֲ�����
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <set>
#include <string>

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Trades.h"
#include "PlatformDataMgr_Orders.h"
#include "PlatformDataMgr_PositionDetails.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 


class CPlatformDataMgr_Positions
{
public:
    CPlatformDataMgr_Positions(
        volatile long& baseFTID,
        volatile long& baseUpdateSeq,
        CPlatformDataMgr_Instruments& refInstruments,
        CPlatformDataMgr_Rates& refRates,
        CPlatformDataMgr_Quots& refQuots,
        CPlatformDataMgr_Trades& refTrades,
        CPlatformDataMgr_Orders& refOrders,
        CPlatformDataMgr_PositionDetails& refPositionDetails,
        bool bUseSettlementPriceToCalculateProfitAndMarginAsPossible);
    ~CPlatformDataMgr_Positions(void);

    //��ѯ�ر�����
    void QryPositionReach( PlatformStru_Position& PositionInfo,int ErrID,bool bIsLast);

	//�����ر�ʱ�����³ֲ���صĹҵ��Ķ��������ѺͶ��ᱣ֤��
	void UpdatePositionFreezeFields_RtnOrder(const PlatformStru_OrderInfo& Order,
        const PlatformStru_OrderInfo& OldOrderInfo,
        const PlatformStru_InstrumentInfo& InstrumentInfo);

    //�ӳɽ���¼���³ֲ֡��˺����ӳֲ���ϸ�����´���������ֲ֡������ڳֲ���ϸ�������ڳֲ���ϸ��������ٸ��³ֲ�
    void UpdatePosition_RtnTrade(const PlatformStru_TradeInfo& TradeInfo,
        const map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
        const PlatformStru_InstrumentInfo& InstrumentInfo);

    //��̬���³ֲּ�¼�ĳֲ�ӯ��(����)������ӯ��(���)��������ӯ��������ۡ������ۣ��õ�������ʱ��һ��
    void UpdatePosition_RtnQuot(const PlatformStru_DepthMarketData& QuotData,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        bool bForceCalculate);
    //����ʱ���³ֲ֡����ݽ���ۣ����³ֲֵ�ӯ������֤��
    void UpdatePosition_OnSettlement(void);
    //��̬���³ֲּ�¼�ı�֤�𡢱�֤���ʣ���ѯ��֤������Ӧʱ���á������Ƿ�����˸���
    bool UpdatePositionByMarginRate_RspQryRate(const PlatformStru_InstrumentMarginRate& MarginRate);

    //��̬���³ֲּ�¼�������ѣ��ɽ��޸ĳֲֺͲ�ѯ����������Ӧʱ����
    bool UpdatePositionByCommissionRate_RspQryRate(const PlatformStru_InstrumentCommissionRate& CommissionRate);
    //���ʲ�ѯ��Ӧʱ�����³ֲ���صĹҵ��Ķ��������ѺͶ��ᱣ֤��
    void UpdatePositionFreezeFieldsByOrders_RspQryRate(const std::string& strInstrument);

    //��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
    int GetPositionKeySet(std::set<PositionKey> &PositionKeySet);
    //��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
    int GetPositionData(const std::string& strAccount,const std::string& InstrumentID,char PosiDirection,char HedgeFlag,PlatformStru_Position& PositionData);
    //��ȡ���гֲּ�¼���ų�����Ϊ0�ļ�¼�����ؽ������
    int GetPositions(std::vector<PlatformStru_Position>& outData);
    //��ȡ���гֲּ�¼����������Ϊ0�ļ�¼�����ؽ������
    int GetPositions4(std::vector<PlatformStru_Position>& outData);
    //����0���ɹ������������ݣ�1: �ɹ���������û�и��£����������ݣ�-1��ʧ��; 
    //lastseq: ��ʾ���ݸ��µ����кš������ǰ�ĸ������кŵ���lastseq����ʾû�и��£�����1(����������)��0��ʾ���жϡ���������ʱ��lastseq������Ϊ���µĸ������к�
    int GetPositions2(const std::string& strInstrument,std::set<long>& setFTID,std::vector<PlatformStru_Position>& outData,long& lastseq);
    //����0���ɹ������������ݣ�1: �ɹ���������û�и��£����������ݣ�-1��ʧ��. lastseqע�ͼ���һ��
    int GetPositions3(const std::string& strInstrument,std::vector<PlatformStru_Position>& outData,long& lastseq);

    //��ղ�ѯ�ֲֵ�map
    void ClearMapQryRltPositions(const std::string& strQryInstrument);
    //��ȡ��ѯ�ĺ�ԼID
    void GetQryPositionInstrument(std::string& outQryInstrument);

    //��ȡ�ֲ�FTID���ϣ����ؽ������
    int GetPositionFTIDList(std::vector<long>& vec);
    //��ȡָ��FTID�ĳֲ֣��ɹ�����0��ʧ�ܷ���-1
    int GetPositionInfo(long lFTID, PlatformStru_Position& outData);
    //��ȡָ��key�ĳֲ���Ϣ���ɹ�����0��ʧ�ܷ���-1
    int GetPositionInfo(PositionKey key, PlatformStru_Position& outData);

    //ȡȫ���ֲֵ�ռ�ñ�֤�𡣿������ڴ��߱�֤���Ż�
    void GetAllMargin(double& TotalMargin);
    //ȡƽ��ӯ��(���ն���)
    void GetAllCloseProfitByDate(double&TotalCloseProfitByDate);
    //ȡȫ���ֲֵĳֲ�ӯ��(���ն���)
    void GetAllPositionProfitByDate(double&TotalPositionProfitByDate);

    //�����������
    void clear(void);
    //�ж��Ƿ�Ϊ��
    bool empty(void);


public:
    //ע�⣺���涨�弸��static��������Ϊ������ϳֲ���Ҳ��ʹ��

	//��ѯ�ֲ�ʱ�ϲ�ͬһ��Լͬһ�������ʷ�ֲֺͽ��ճֲּ�¼
	static void MergePositions_RspQryPosition( PlatformStru_Position& firstPositionInfo, const PlatformStru_Position& secondPositionInfo );

    //�ڳֲ������У�����ƽ�����Ϳ�ƽ����, (�������йҵ�)����ȫ��
    static void CalcuCloseValueInPositionByAllOrders_Internal(PlatformStru_Position& PositionData,
        int CloseVolume,
        int CloseTodayVolume,
        int CloseYesterdayVolume);

    //��������н���۵Ļ����Ƿ�ʹ�ý���ۼ���ӯ���ͱ�֤��
    void SetbUseSettlementPriceToCalculateProfitAndMarginAsPossible(bool bUse)
    {
        m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible=bUse;
    }

private:
    //��̬���³ֲּ�¼�ĳֲ�ӯ��(����)������ӯ��(���)�Ͷ�����ӯ��������ۡ������ۺ��ּ�
    //����ӯ������ʹ�õļ۸�˳��Ϊ������� > ���¼� > ������
    void UpdatePositionByQuot_Internal(PlatformStru_Position& PositionInfo,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        const PlatformStru_DepthMarketData& QuotData,
        bool bForceCalculate);
    //����һ�����ֱ����仯���ݸ��³ֲֵĶ��������ѡ����ᱣ֤��Ͷ�����
    //�ֲ��ǵ��Ⱥ�Լ�������ǵ��Ⱥ�Լ����Ϻ�Լ
    void RefreshPositionFreezeByOneOpenOrder_Internal(const PlatformStru_OrderInfo& Order,
        const PlatformStru_OrderInfo& OldOrderInfo,
        const PlatformStru_InstrumentInfo& InstrumentInfo);
    //����һ��ƽ�ֱ����仯���ݸ��³ֲֵĹҵ�������ƽ��
    //�ֲ�һ���ǵ��ȳֲ֣������Ƿ���ϱ�������ϱ���
    void RefreshPositionCloseOrderVolumeByOneOrder_Internal(const PlatformStru_OrderInfo& Order,
        const PlatformStru_OrderInfo& OldOrderInfo,
        const PlatformStru_InstrumentInfo& InstrumentInfo);
    //�ڳֲ������У�����ƽ�����Ϳ�ƽ����, (����һ���ҵ�)������������
    //Deltaxxx��������һ����˵�Ǹ�ֵ
    //�˺�����Ե��Ⱥ�Լ�ֲ�
    void CalcuCloseValueInPositionByOneOrder_Internal(PlatformStru_Position& PositionData,
        int DeltaCloseVolume,int DeltaCloseTodayVolume,int DeltaCloseYesterdayVolume);


    //���³ֲ���صĹҵ��Ķ��������ѺͶ��ᱣ֤�𡣳ɽ��½��ֲ�ʱ���õ�����ʱ����
    void UpdatePositionFreezeFieldsByAllOrders_Internal(PlatformStru_Position& PositionData);

    //��̬���³ֲּ�¼�ı�֤�𡢱�֤���ʣ��ɽ��޸ĳֲ�ʱ����
    //����Ҫ������ϵ�����ɢ���������һ��(ָ����ɢ������)������ͨ�ֲ���ϸ����Ӧ�ĳֲֵı�֤�������
    void UpdatePositionByMarginRate_RtnTrade_Internal(PlatformStru_Position& PositionInfo,
        const map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails);

    //��̬���³ֲּ�¼�ı�֤�𡢱�֤���ʣ���ѯ��֤������Ӧ�ͽ���ʱ����
    void UpdatePositionByMarginRate_RspQryRate_OnSettlement_Internal(const PlatformStru_InstrumentMarginRate& MarginRate,
        PlatformStru_Position& PositionInfo,
        bool bForce);


private:
    Ceasymutex				            m_mutex;

    CPlatformDataMgr_Instruments        &m_RefInstruments;
    CPlatformDataMgr_Rates              &m_RefRates;
    CPlatformDataMgr_Quots              &m_RefQuots;
    CPlatformDataMgr_Trades             &m_RefTrades;
    CPlatformDataMgr_Orders             &m_RefOrders;
    CPlatformDataMgr_PositionDetails    &m_RefPositionDetails;

    //����н���۵Ļ����Ƿ�ʹ�ý���ۼ���ӯ���ͱ�֤��
    bool                                    m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible;

    //----------���������ݼ�----------
    CPlatformDataSetList<PositionKey,PlatformStru_Position> m_PositionData;
};

#ifdef WIN32
#pragma managed(pop)
#endif 
