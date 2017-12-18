/*
    ���ڱ����ά��CTP����ϳֲ�����
    ��ϳֲ�ʹ�óֲֵĽṹ��
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <string>
using std::vector;
using std::string;

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Trades.h"
#include "PlatformDataMgr_Orders.h"
#include "PlatformDataMgr_PositionDetailCombs.h"
#include "PlatformDataMgr_PositionDetails.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CPlatformDataMgr_PositionCombs
{
public:
    CPlatformDataMgr_PositionCombs(
        volatile long& baseFTID,
        volatile long& baseUpdateSeq,
        CPlatformDataMgr_Instruments& refInstruments,
        CPlatformDataMgr_Quots& refQuots,
        CPlatformDataMgr_Trades& refTrades,
        CPlatformDataMgr_Orders& refOrders,
        CPlatformDataMgr_PositionDetailCombs& refPositionDetailCombs,
        CPlatformDataMgr_PositionDetails& refPositionDetails);
    ~CPlatformDataMgr_PositionCombs(void);

    //��ѯ�ر�����
    void QryPositionCombReach( PlatformStru_Position& PositionInfo,int ErrID,bool bIsLast);

    //�ӳɽ���¼������ϳֲ֡��˺�������ϳֲ���ϸ�����´�����������ϳֲ֡���������ϳֲ���ϸ����������ϳֲ���ϸ��������ٸ�����ϳֲ�
    void Update_RtnTrade(const PlatformStru_TradeInfo& TradeInfo,
        const map<PositionDetailCombKey,PlatformStru_PositionDetailComb>& ModifiedPositionDetailCombs);
    //������ϳֲּ�¼�ı�֤�𡢱�֤���ʣ���ѯ��֤������Ӧʱ����
    bool UpdatePositionByMarginRate_RspQryRate(const PlatformStru_InstrumentMarginRate& MarginRate);

    //������ϳֲ���ϸ�ͳֲ���ϸ��������ϳֲֵĳֲֳɱ������ֳɱ����ֲ־��ۺͿ��־��ۡ��ڳ�ʼ����ѯ��Ϻ����
    void UpdateCostByPositionDetialComb_AfterInitQry(void);


    //��ղ�ѯ��ϳֲֵ�map
    void ClearMapQryRltPositionCombs(std::string& strQryInstrument);
    //��ȡ��ϳֲֵļ���
    int GetPositionCombs(std::vector<PlatformStru_Position>& outData);
    //��ȡָ����Լ����ϳֲּ���
    int GetPositionCombs2(const std::string& strInstrument,std::vector<PlatformStru_Position>& outData);
    //��ȡ��ѯ�ĺ�ԼID
    void GetQryPositionCombInstrument(std::string& outQryInstrument);

    //��ȡ��ϳֲֵ�FTID����
    int GetPositionCombFTIDList(std::vector<long>& vec);
    //��ȡָ��FTID����ϳֲ���ϸ
    int GetPositionCombInfo(long lFTID, PlatformStru_Position& outData);

    //ȡȫ���ֲֵ�ռ�ñ�֤���ƽ��ӯ��(���ն���)��
    void GetAllMargin(double& TotalMargin);

    //�����������
    void clear(void);
    //�ж��Ƿ�Ϊ��
    bool empty(void);


private:
    //������ϳֲ���ϸ�ͳֲ���ϸ��������ϳֲֵĳֲ������ֲֳɱ������ֳɱ����ֲ־��ۡ����־��ۺͱ�֤��
    void UpdateCostByPositionDetialComb_Internal(const PositionKey& combkey,PlatformStru_Position& comb,bool bUpdateMargin_Position);

private:
    Ceasymutex				                m_mutex;

    CPlatformDataMgr_Instruments            &m_RefInstruments;
    CPlatformDataMgr_Quots                  &m_RefQuots;
    CPlatformDataMgr_Trades                 &m_RefTrades;
    CPlatformDataMgr_Orders                 &m_RefOrders;
    CPlatformDataMgr_PositionDetailCombs    &m_RefPositionDetailCombs;
    CPlatformDataMgr_PositionDetails        &m_RefPositionDetails;


    //----------���������ݼ�----------
    CPlatformDataSetList<PositionKey,PlatformStru_Position> m_PositionCombData;


};

#ifdef WIN32
#pragma managed(pop)
#endif 
