#pragma once

#include "Interface_SvrCommon.h"

#ifndef SVRTRADINGSTRATEGY_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRTRADINGSTRATEGY_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRTRADINGSTRATEGY_EXPORTS
#define SVRTRADINGSTRATEGY_API __declspec(dllexport)
#else
#define SVRTRADINGSTRATEGY_API __declspec(dllimport)
#endif
#endif

#include "CommonStruct.h"
#include <map>
#include <set>
#include <vector>

class SVRTRADINGSTRATEGY_API CInterface_SvrTradingStrategy : public CInterface_SvrCommon
{
public:
    static CInterface_SvrTradingStrategy& getObj(void);

    //��ѯ���ײ���
    virtual void GetTradeStrategy(std::vector<TradeStrategy>& vecStrategy )=0;
    //��ѯ���ײ�������֯������ϵ
    virtual void GetAssetOrgStrategyRelation(std::map<int,std::vector<int>>& mapRelation)=0;
    //ͨ����֯����ID��ý��ײ���
    virtual void GetStrategyIDsByOrgID(int nOrgID,std::vector<int>& vecStrategyID )=0;
    //ͨ�����ײ���ID�����֯���� 
    virtual void GetOrgIDsByStrategyID (int nStrategyID,std::vector<int>& vecOrgID )=0;
    //ͨ�����ײ���IDȡ������Ϣ
    virtual void GetInfoByStrategyID (int nStrategyID,TradeStrategy& oStrategy)=0;
};







