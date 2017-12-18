/*
    ���ڱ����ά��CTP�ĳɽ�ͳ������
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <string>

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Trades.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 


class CPlatformDataMgr_TradeTotals
{
public:
    CPlatformDataMgr_TradeTotals(
        volatile long& baseFTID,
        volatile long& baseUpdateSeq,
        CPlatformDataMgr_Trades& refTrades);
    ~CPlatformDataMgr_TradeTotals(void);


    //��ѯ�ĳɽ��ر���ȫ����ʼ����ɽ�ͳ��
    void QryTradeTotalReach(bool bIsLast);
    //�ɽ��ر�ʱ��������Ӧ��Լ�ĳɽ�ͳ��
    void UpdateTradeTotal_RtnTrade(const PlatformStru_TradeInfo& TradeInfo);
    //�������������Ӧʱ��������Ӧ��Լ�ĳɽ�ͳ��
    bool UpdateTradeTotal_RspQryCommissionRate(const std::string& strInstrument);

    ///��ȡȫ���ɽ�ͳ�Ƽ�¼
    int GetAllTradeTotals(std::vector<PlatformStru_TradeTotalInfo>& outData);
    ///��ȡָ����Լ�ĳɽ�ͳ�Ƽ�¼�����������������
    int GetTradeTotalOfInstrument(const std::string& strInstrument, std::vector<PlatformStru_TradeTotalInfo>& outData);


    //�����������
    void clear(void);
    //�ж��Ƿ�Ϊ��
    bool empty(void);

private:
    Ceasymutex				m_mutex;
    CPlatformDataMgr_Trades &m_RefTrades;

    ////----------�����ǳɽ�ͳ�����ݼ�----------
    CPlatformDataSetList<TradeTotalKey,PlatformStru_TradeTotalInfo> m_TradeTotalData;
};

#ifdef WIN32
#pragma managed(pop)
#endif 
