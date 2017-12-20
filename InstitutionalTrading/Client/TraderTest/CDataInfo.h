#pragma once
#include "CommonPkg.h"
#include "CommonStruct.h"
#include "TcpLayer.h"
#include <vector>
#include <map>
#include <set>
#include <algorithm>

enum MODULE_NAME
{
    FINANCE_PRODUCT,
    TRADE_STRATEGY,
    TRADER_ACCOUNT
};

class CDataInfo
{
private:
	CDataInfo();
	~CDataInfo();

public:
	static CDataInfo* NewInstance();
	static void DestroyInstance();
	static CDataInfo* m_pInstance;

public:
	void Clear();

    //��¼
    void SetLogonInfo(const sLoginRsp& loginRsp);
    void GetLogonInfo(sLoginRsp& loginRsp);


    //��֯����
    void SetAeestMgmtOrg(std::map<int, Organization>&   mapAssetMgmtOrg);
    void GetAssetMgmtOrg(std::map<int, std::map<int, Organization>>&	mapAssetMgmtOrgTree);
    bool GetAssetMgmtOrgByID(int nAssetMgmtOrgID, Organization& sOrg);
    int  GetAssetMgmtOrgIDByName(std::string& sName);
    string  GetAssetMgmtOrgNameByID(int nOrgID);


    //�û�
    void ClearUserInfo();
    void SetUserInfo(std::map<int, UserInfo>& mapUserInfo);
    void GetUserInfo(std::map<int, UserInfo>& mapUserInfo);
    bool GetUserInfoByUserID(int nUserID,UserInfo& sUserInfo);

    //��ɫ
    void ClearRole();
    void SetRole(std::map<int, RoleInfo>& mapRole);
    void GetRole(std::map<int, RoleInfo>& mapRole);
    string GetRoleNameByID(int nRoleID);

    //�û����ɫ��ϵ
    void ClearUserRoleRelation();
    void SetUserRoleRelation(std::map<int, std::vector<UserRole>>& mapUserRole);
    void GetUserRoleRelation(std::map<int, std::vector<UserRole>>& mapUserRole);
    void GetRoleByUserID(int nUserID,std::vector<UserRole>& vecRole);

    //�û�����֯������ϵ
    void ClearUserAndOrgRelation();
    void SetUserAndOrgRelation(std::map<int, UserAndOrgRelation>& mapUserAndOrgRelation);
    void GetUserAndOrgRelation(std::map<int, UserAndOrgRelation>& mapUserAndOrgRelation);


	//Ȩ��
	void SetPrivilegeNodes(std::map<int, PrivilegeType>& mapPrivilegeNode);
	void GetPrivilegeNodes(std::map<int, PrivilegeType>& mapPrivilegeNode);
	void SetPrivilegeActions(std::map<int, PrivilegeAction>& mapPrivilegeAction);
	void GetPrivilegeActions(std::map<int, PrivilegeAction>& mapPrivilegeAction);
	void SetPrivilegeNames(std::map<int, RoleInfo>& mapPrivilegeName);
	void GetPrivilegeNames(std::map<int, RoleInfo>& mapPrivilegeName);
	void SetAllPrivilegeValues(std::map<int, std::map<int, Privilege>>& allPrivilegeValue);
	void GetPrivilegeValues(int nPrivilegeID, std::map<int, Privilege>& mapValues);
	void SetPrivilegeValues(int nPrivilegeID, std::map<int, Privilege>& mapValues);
	void UpdatePrivilegeNameType(int nPrivilegeID, int nUserType);
	void SetCurPrivilegeValues(int nUserType, std::map<int, Privilege>& mapValues);
	void GetCurPrivilegeValues(int& nUserType, std::map<int, Privilege>& mapValues);
	void SetPrivilegeName(RoleInfo& name);
	void DelPrivilegeName(int nPrivilegeID);
	void SetCurPrivilegeName(RoleInfo& name);
	RoleInfo GetCurPrivilegeName();


	//���͹�˾
	void SetBrokerInfos(std::map<int,BrokerInfo>& mapBrokerInfo);
	void SetBrokerInfo(BrokerInfo& brokerInfo);
	void DelBrokerInfo(int nBrokerID);    
	void GetBrokerInfos(std::map<int,BrokerInfo>& mapBrokerInfo);
    bool GetBrokerIDByName(std::string szName,int& brokerID);
    bool GetBrokerNameByID(int brokerID,std::string& szName);


    //��������
    void SetServerGroupInfo(std::map<int,ServerGroup>& mapServerGroup);
	void SetServerGroups(std::map<int,std::map<int,ServerGroup>>& mapServerGroup);
	void GetServerGroups(int nBrokerID, std::map<int,ServerGroup>& mapServerGroup);
    bool GetServerGroupNameByGroupID(int nServerGroupID,std::string& szName);
    bool GetServerGroupIDByBrokerIDAndServerName(int nBrokerID,std::string szName,int& nServerGroupID);
	void SetServerGroup(int nBrokerID, ServerGroup& serverGroup);
	void DelServerGroup(int nBrokerID, int nServerID);


    //���������ַ
	void SetServerAddress(std::map<int, std::map<int, ServerAddress>>& mapServerAddr);
	void SetServerAddress(int nServerID, ServerAddress& addr);
	void DelServerAddress(int nServerID, int nAddrID);
	void GetServerAddress(int nServerID, std::map<int, ServerAddress>& mapServerAddr);


    //ί�н����˺�
    void SetTradeAccount(std::map<int, TradeAccount>& mapTradeAccount);
    void SetTradeAccount(const TradeAccount* pTradeAccount);
    void DelTradeAccount(int nTradeAccountID);
    void GetTradeAccount(std::map<int, TradeAccount>& mapTradeAccount);
    bool GetTradeAccountByAccount(string& sAccount,TradeAccount& oTradeAccount);
    string GetTradeAccountByID(int nTradeAccountID);
    bool IsValidOfTradeAccount(std::string strTradeAccount);

    //�û���ί�н����˺�
    void ClearUserAndTraderAccountRelation();
    void SetUserAndTraderAccountRelation(std::map<int, UserAndTradeAccountRelation>& mapUserAndOrgRelation);
    void GetUserAndTraderAccountRelation(std::map<int, UserAndTradeAccountRelation>& mapUserAndOrgRelation);

    //��Ʋ�Ʒ;
    void ClearDelFinanceProduct();    
    void SetDelFinancialProduct(int nProductID);
    void SetFinancialProductStatus();
    void GetDelFinanceProduct(std::set<int>& setDel); 

    void ClearFinanceProuct(); 
    void SetFinancialProduct(const FinancialProduct *pFP); 
    void GetFinancialProduct(std::map<int, FinancialProduct>& mapFinancialProduct);
    string GetFinancialProductNameByID(int nProductID);
    int  GetFinancialProductIDByName(string& sName); 
    void AddFinancialProduct(const FinancialProduct *pFP);
    void ModifyFinancialProduct(const FinancialProduct *pFP);

    bool IsValidOfProductName(const string& sName);  

    //��Ʋ�Ʒ�뽻��Ա�Ĺ�ϵ
    void ClearProductTraderRelation();
    void SetProductTraderRelation(const TraderProRelation *pRelate); 
    void GetProTraderRelation(std::vector<TraderProRelation>& vec);
    void AddProductTraderRelation(const TraderProRelation *pRelate);
    void DeleteProductTraderRelation(int nProductID,bool bReal);
    void DeleteProductTraderRelation(const TraderProRelation *pRelate,bool bReal);
    
    //���ײ���
    void SetTradeStrategy(const TradeStrategy *pStrategy);
    void DeleteTradeStrategyByID(int& id);    
    void GetTradeStrategy(std::map<int, TradeStrategy>&	mapTradeStrategy);
    bool GetTradeStrategyByID(int nTradeStrategyID, TradeStrategy& oStrategy);
    bool GetTradeStrategyByName(TradeStrategy& oStrategy,string& sName); 
    void ClearTradeStrategy();
    bool IsValidOfStrategyName(const string& sName);

    //���ײ�������֯�����Ĺ�ϵ
    void ClearOrgStrategyRelation();
    void SetOrgStrategyRelation(const AssetOrgStrategyRelation *pRelate);
    void GetOrgStrategyRelation(std::vector<AssetOrgStrategyRelation>& vec);
    void DeleteOrgStrategyRelationByID(int nStrategyID);

public:
    int  GetNextSeqNum(MODULE_NAME eName);

private:	

    sLoginRsp m_loginRsp;	//�˻���Ϣ

	std::map<int, std::map<int, Organization>>     m_mapAssetMgmtOrgTree;	//��֯����
	std::map<int, Organization>	m_mapAssetMgmtOrg;    
    std::map<int, UserInfo>   m_mapUserInfo;//�û�   
    std::map<int, RoleInfo>   m_mapRoleInfo;//��ɫ   
    std::map<int, std::vector<UserRole>>   m_mapUserRole;//�û����ɫ��ϵ
	std::map<int, UserAndOrgRelation>	m_mapUserAndOrgRelation; //�û�����֯������ϵ
    std::map<int, UserAndTradeAccountRelation> m_mapUserAndTradeAccountRelation;//�û���ί�н����˻���ϵ

	std::map<int, PrivilegeType>   m_mapNode; //keyΪNodeID
	std::map<int, PrivilegeAction>     m_mapAction;	//keyΪActionID
	std::map<int, RoleInfo>   m_mapPrivilegeName;	//keyΪPrivilegeID

	std::map<int, std::map<int, Privilege>>   m_mapAllPrivilegeValue; //���keyΪPrivilegeID�����keyΪActionID
	std::map<int, Privilege>  m_curPrivilegeValue;
	RoleInfo  m_curPrivilegeName;	//��ǰѡ�е�Ȩ����
	int m_nCurUserType;
	std::map<int, Privilege>  m_mapCurPrivilegeValue;	

	
	std::map<int, BrokerInfo>   m_mapBrokerInfo;//���͹�˾	
    std::map<int, ServerGroup>  m_mapServerGroupInfo;
	std::map<int, std::map<int, ServerGroup>>   m_mapServerGroup;//��������	
	std::map<int, std::map<int, ServerAddress>>     m_mapServerAddr;//���׵�ַ    
    std::map<int, TradeAccount>     m_mapTradeAccount;//ί�н����˻�
    
    std::set<int>                       m_setDelFinanceProduct;//����ɾ���Ĳ�ƷID
    std::map<int, FinancialProduct>	    m_mapFinancialProduct;   //��Ʋ�Ʒ 
    std::map<Key,TraderProRelation>     m_mapTraderProRelation; //��Ʋ�Ʒ�뽻��Ա��ϵ 
    std::map<int, TradeStrategy>        m_mapTradeStrategy; //���ײ���              
    std::vector<AssetOrgStrategyRelation>   m_vecOrgStrategyRelation;//���ײ�������֯�����Ĺ�ϵ     


};
