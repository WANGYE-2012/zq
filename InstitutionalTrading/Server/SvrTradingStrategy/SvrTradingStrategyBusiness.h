#pragma once
#include "rwlock.h"
#include "interface_svrtradingstrategy.h"

class CSvrTradingStrategyBusiness : public CInterface_SvrTradingStrategy
{

public:
    static CSvrTradingStrategyBusiness& getObj();
    static void deleteObj();

    CF_ERROR ResponseToAsk(const eAskType& neDataType,const eAskOperationType& neChangeType,const int nllDataID,const char * npDataBuf);

    //���ײ���
    int AddStrategy(const int nUserID, TradeStrategy *pStrategy,int& nID);
    int DeleteStrategy(const int nUserID, int nID);
    int ModifyStrategy(const int nUserID, const TradeStrategy *pStrategy);

    //���ײ�������֯�����Ĺ�ϵ
    int SaveStrategyOrganizationRelation(const int nUserID, int nStrategyID, std::vector<int> vecOrgID);
    void GetStrategyOrganizationRelation(std::vector<AssetOrgStrategyRelation>& vecRelation);

    //ģ��ӿ�
    void GetTradeStrategy(std::vector<TradeStrategy>& vecStrategy ); 
    void GetAssetOrgStrategyRelation(std::map<int,std::vector<int>>& mapRelation);
    void GetStrategyIDsByOrgID(int nOrgID,std::vector<int>& vecStrategyID );
    void GetOrgIDsByStrategyID (int nStrategyID,std::vector<int>& vecOrgID );
    void GetInfoByStrategyID (int nStrategyID,TradeStrategy& oStrategy);

public:
    //�����ݿ��������
    bool LoadDataFromDB();
private:
    //��ȡ���ײ���
    bool LoadStrategyFromDB();
    //��ȡ���ײ�������֯�����Ĺ�ϵ
    bool LoadOrgStrategyRelationFromDB();

private:
    CSvrTradingStrategyBusiness(void);
    virtual ~CSvrTradingStrategyBusiness(void);
    static CSvrTradingStrategyBusiness* m_pObj;

private:
    //�����Խ��ײ���IDΪ�����Ľ��ײ���
    std::map<int, TradeStrategy>							m_mapStrategy;
    //�����Խ��ײ���IDΪ�����Ľ��ײ��Ժ���֯������ϵ
    std::map<int, std::vector<int>>	                        m_mapStrategyID2OrgIDRelation;

    /*
           sign      Year        Month        Day                   id   
    32bit  |-1-|------6------|-----5-----|-----5-----|--------------15---------------|  
    */
    int m_nNextTradeStrategyID; 

    rwlock_t    m_Lock;
};
