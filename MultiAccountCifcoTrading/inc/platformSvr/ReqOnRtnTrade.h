/*
    �����ڳɽ��ر����ѯ�ֲ֡��ֲ���ϸ���ʽ�
*/


#pragma once

#include <windows.h>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <string>
#include "../inc/ISvr/BusinessData.hpp"
#include "../inc/Module-Misc/packagecache.h"



struct Stru_ReqKeyOnRtnTrade
{
    Stru_ReqKeyOnRtnTrade(const std::string& strbrokerid,const std::string& strinvestorid,const std::string& strinstrumentid)
    {
        strBrokerID=strbrokerid;
        strInvestorID=strinvestorid;
        strInstrumentID=strinstrumentid;
    };
    bool operator< (const Stru_ReqKeyOnRtnTrade& r) const
    {
        if(strBrokerID<r.strBrokerID) return true;
        if(strBrokerID>r.strBrokerID) return false;
        if(strInvestorID<r.strInvestorID) return true;
        if(strInvestorID>r.strInvestorID) return false;
        if(strInstrumentID<r.strInstrumentID) return true;
        if(strInstrumentID>r.strInstrumentID) return false;
        return false;
    };
    bool operator== (const Stru_ReqKeyOnRtnTrade& r) const
    {
        if(strBrokerID==r.strBrokerID||
            strInvestorID==r.strInvestorID||
            strInstrumentID==r.strInstrumentID) 
            return true;
        return false;
    };
    bool operator!= (const Stru_ReqKeyOnRtnTrade& r) const
    {
        return !(*this == r);
    }
    
    bool operator> (const Stru_ReqKeyOnRtnTrade& r) const
    {
        return !(*this < r ||*this == r);
    }


	///���͹�˾����
    std::string strBrokerID;
	///Ͷ���ߴ���
    std::string strInvestorID;
	///��Լ����
    std::string strInstrumentID;
};

struct Stru_ReqOnRtnTradeTime
{
    Stru_ReqOnRtnTradeTime()
    {
        memset(this,0,sizeof(*this));
    }

    DWORD OnRtnTradeTime;
    DWORD QryPositionDetailTime;
    DWORD QryPositionTime;
    DWORD QryPositionCombTime;
    DWORD QryAccountTime;
    bool bQryPositionComb;             //�Ƿ���Ҫ��ѯ��ϳֲ�
    int RecordCountOfPositionDetail;    //�ֲ���ϸ��¼���Ĵ�С��������ֵ���Զ���ѯ�ֲ���ϸ
};

class CReqOnRtnTrade
{
public:
    CReqOnRtnTrade();
    ~CReqOnRtnTrade();

    //�гɽ��ر�����
    void OnRtnTrade(const std::string& strBrokerID,
                            const std::string& strInvestorID,
                            const std::string& strInstrumentID,
                            bool bPositionComb,
                            int RecordCountOfPositionDetail);

    //���óֲֲ�ѯ���ȼ���1:�ֲ֣�2:�ֲ���ϸ��3:��ϳֲ�
    void SetQryPrior(int QryPrior);

    //��ʱ����Ƿ���Ҫ������ѯ
    void CheckOnTime(CServiceProxy& rServiceProxy);

private:
    //��ѯ�ֲ�
    void ReqPosition_Internal(const Stru_ReqKeyOnRtnTrade& ReqKey,Stru_ReqOnRtnTradeTime& ReqTime, CServiceProxy& rServiceProxy);
    //��ѯ�ֲ���ϸ
    void ReqPositionDetail_Internal(const Stru_ReqKeyOnRtnTrade& ReqKey,Stru_ReqOnRtnTradeTime& ReqTime, CServiceProxy& rServiceProxy);
    //��ѯ��ϳֲ���ϸ
    void ReqPositionComb_Internal(const Stru_ReqKeyOnRtnTrade& ReqKey,Stru_ReqOnRtnTradeTime& ReqTime, CServiceProxy& rServiceProxy);
    //��ѯ�ʽ�
    void ReqTradingAccount_Internal(const Stru_ReqKeyOnRtnTrade& ReqKey,Stru_ReqOnRtnTradeTime& ReqTime, CServiceProxy& rServiceProxy);


private:

    CRITICAL_SECTION m_CS;

    //�ɽ��ر��ĺ�Լ��ʱ��(����)��map
    std::map<Stru_ReqKeyOnRtnTrade,Stru_ReqOnRtnTradeTime> m_mapReqTime;

    //�ֲֲ�ѯ���ȼ���1:�ֲ֣�2:�ֲ���ϸ��3:��ϳֲ֡���ѯ˳��(��m_QryPrior==1)�� �ֲ֡��ʽ𡢳ֲ���ϸ����ϳֲ֣�ע�⣬�ʽ��ѯһ���ڳֲ�֮��
    int m_QryPrior;


};

