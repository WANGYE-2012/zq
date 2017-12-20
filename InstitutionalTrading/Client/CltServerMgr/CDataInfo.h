#pragma once
#include "CommonPkg.h"
#include "CommonStruct.h"
#include "TcpLayer.h"
#include "popmsgwin.h"
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

#define SELECT_ALL "ȫ��"
#define STATUS_LOGIN "�ѵ�¼"
#define STATUS_LOGINOUT "δ��¼"

enum MODULE_NAME
{
    FINANCE_PRODUCT,
    TRADE_STRATEGY,
    TRADER_ACCOUNT
};

enum eUserType
{
    eManagerType,   //����Ա
    eRiskerType     //���Ա
};

typedef std::pair<int, int> Key;

class CDataInfo
{
public:
	enum {	
		conInitQueryOrgan,				// ��ʼ��ѯ��֯����
		conInitQueryUser,				// ��ʼ��ѯ�û�
		conInitQueryOrganUserRelate,	// ��ʼ��ѯ�û�����֯��ϵ
		conInitItemCount				// ��ѯ������
	};

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
    void ClearOrganization();
    void SetOrganization(std::map<int, Organization>& mapOrganization);
    void SetOrganization(const Organization* pOrganization);
    void DelOrganization(int nOrgID);
    void GetOrganization(std::map<int, std::map<int, Organization>>& mapAssetMgmtOrgTree);
    bool GetOrganizationByID(int nAssetMgmtOrgID, Organization& sOrg);
    int  GetOrganizationIDByName(std::string& sName);
    string  GetOrganizationNameByID(int nOrgID);
    int  GetOrganizationScaleLevelByID(int nOrgID);
    bool IsValidOfOrganizationName(const string& sName);

    //�û�
    void ClearUserInfo();
    void SetUserInfo(std::map<int, UserInfo>& mapUserInfo);
    void GetUserInfo(std::map<int, UserInfo>& mapUserInfo);
    void AddUserInfo(const UserInfo* pUserInfo);
    void DelUserInfo(int nUserID);
    bool GetUserInfoByUserID(int nUserID,UserInfo& sUserInfo);
    bool GetUserInfoByAccount(string szUserAccount,UserInfo& sUserInfo);
    int GetUserIDByAccount(string szUserAccount);
    int IsValidOfUserAccount(std::string strAccount, UserType nType);
    bool IsTraderTypeByUserID(int nUserID);
    void SetUserStatus(int nUserID);

    //��ɫ
    void ClearRole();
    void SetRole(std::map<int, RoleInfo>& mapRole);
    void SetRole(const RoleInfo& oRole);
    void GetRole(std::map<int, RoleInfo>& mapRole);
    bool GetRoleInfoByID(int nRoleID,RoleInfo& oRoleInfo);
    string GetRoleNameByID(int nRoleID);
    int GetRoleIDByName(string& szName);
    void DelRole(int nRoleID);
    bool IsValidOfRoleName(std::string strName);

    //�û����ɫ��ϵ
    void ClearUserRoleRelation();
    void ClearUserRoleRelationByUserID(int nUserID);
    void SetUserRoleRelation(const UserRole* pUserRole);
    void SetUserRoleRelation(std::map<int, std::vector<UserRole>>& mapUserRole);
    void GetUserRoleRelation(std::map<int, std::vector<UserRole>>& mapUserRole);
    void GetUserType2RoleRelation(std::map<int,std::vector<RoleInfo>>& mapUserType2Role);
    void GetRoleByUserID(int nUserID,std::vector<UserRole>& vecRole);
    string GetRoleByUserID(int nUserID);

    //�û�����֯������ϵ
    void ClearUserAndOrgRelation();
    void DelUserAndOrgRelation(int nID, int nUserType);
    void SetUserAndOrgRelation(std::vector<UserAndOrgRelation>& vecUserAndOrgRelation);
    void SetUserAndOrgListenRelation(std::vector<UserAndOrgRelation>& vecUserAndOrgListenRelation);
    void InitUserAndOrgRelation(const UserAndOrgRelation* pRelation);
    void AddUserAndOrgRelation(const UserAndOrgRelation* pRelation);
    void SetUserAndOrgRelation(const UserAndOrgRelation* pRelation);
    void GetUserAndOrgRelation(std::vector<UserAndOrgRelation>& vecUserAndOrgRelation);
    string GetOrgNameByUserID(int nUserID);

    void GetUserInfosByOrgID(int nOrgID, UserType nUserType, vector<UserInfo>& vecUserInfo, bool bSubOrg);
	void GetUserInfosBySubOrgID(UserType nUserType, map<int, map<int, Organization>>& mapMain, map<int, Organization>& mapSub, vector<UserInfo>& vecUserInfo, bool bSubOrg);
	bool GetDealerByOrganID(int nOrganID, set<int>& setDealerID);

	//Ȩ��    
    void ClearPrivilege();
    void SetPrivilege(std::map<int,Privilege>& mapPrivilege);
    void GetPrivilege(std::map<int,Privilege>& mapPrivilege);
    void SetUserType2Privilege(std::map<int,std::vector<Privilege>>& mapUserType2Privilege);
    void SetParentID2Privilege(std::map<int,std::vector<Privilege>>& mapParentID2Privilege);
    void GetUserType2Privilege(std::map<int,std::vector<Privilege>>& mapUserType2Privilege);
    void GetParentID2Privilege(std::map<int,std::vector<Privilege>>& mapParentID2Privilege);

    //��ɫ��Ȩ�޹�ϵ
    void ClearRolePrivilegeRelation();
    void SetRolePrivilegeRelation(std::map<int,std::vector<int>>& mapRoleID2Privilege);
    void SetRolePrivilegeRelation(int nRoleID, std::vector<int>& vecPrivilegeID);
    void GetRolePrivilegeRelation(std::map<int,std::vector<int>>& mapRoleID2Privilege);

	//���͹�˾
    void ClearBrokerInfos();
	void SetBrokerInfos(std::map<int,BrokerInfo>& mapBrokerInfo);
	void SetBrokerInfo(BrokerInfo& brokerInfo);
    void SetBrokerInfoQuot(int nBrokerID, const string& szAccount, const string& szPWD);
	void DelBrokerInfo(int nBrokerID);    
	void GetBrokerInfos(std::map<int,BrokerInfo>& mapBrokerInfo);
    int  GetBrokerIDByName(std::string szName);
    std::string GetBrokerCodeByID(int brokerID);
    std::string GetBrokerNameByID(int brokerID);
    int  GetBrokerIDByCode(const string& szCode);
    bool IsValidOfBrokerCode(std::string strBrokerCode);
    bool IsQuotBrokerByID(int nBrokerID);


    //��������
    void ClearServerGroups();
	void SetServerGroups(std::map<int,std::map<int,ServerGroup>>& mapServerGroup);
	void GetServerGroups(int nBrokerID, std::map<int,ServerGroup>& mapServerGroup);
    bool GetServerGroupByID(int nBrokerID, int nServerID, ServerGroup& oServerGroup);
    std::string GetServerGroupNameByID(int nBrokerID,int nServerGroupID);
    int  GetServerGroupID(int nBrokerID, std::string szServerName);
	void SetServerGroup(int nBrokerID, ServerGroup& serverGroup);
	void DelServerGroup(int nBrokerID, int nServerID);
    bool IsValidOfServerName(int nBrokerID, std::string strServerGroup);
    bool IsValidOfSettle(int nBrokerID,bool bChecked);


    //���������ַ
	void SetServerAddress(std::map<int, std::map<int, ServerAddress>>& mapServerAddr);
	void SetServerAddress(int nServerID, ServerAddress& addr);
	void DelServerAddress(int nServerID, int nAddrID);
	void GetServerAddress(int nServerID, std::map<int, ServerAddress>& mapServerAddr);
    bool GetServerAddrByID(int nServerID, int nAddrID, ServerAddress& oServerAddr);

    //������������ַ
    void ClearServerAddressQuot();
    void SetServerAddressQuot(std::map<int, ServerAddressQuot>& mapServerAddr);
    void SetServerAddressQuot(ServerAddressQuot& addr);
    void DelServerAddressQuot(int nAddrID);
    void GetServerAddressQuot(std::map<int, ServerAddressQuot>& mapServerAddr);
    bool GetServerAddrQuotByID(int nAddrID, ServerAddressQuot& oServerAddr);


    //ί�н����˺�
    void ClearTradeAccount();
    void SetTradeAccount(std::map<int, TradeAccount>& mapTradeAccount);
    void SetTradeAccount(const TradeAccount* pTradeAccount);
    void DelTradeAccount(int nTradeAccountID);
    void GetTradeAccount(std::map<int, TradeAccount>& mapTradeAccount);
    bool GetTradeAccountByAccount(string& sAccount,TradeAccount& oTradeAccount);
    string GetTradeAccountByID(int nTradeAccountID);
    int  GetBrokerIDByAccountID(int nTradeAccountID);
    int GetTradeAccountIDByAccount(string szTradeAccount);
    bool IsValidOfTradeAccount(std::string strTradeAccount);



    //�û���ί�н����˺�
    void ClearUserAndTraderAccountRelation();
    void SetUserAndTraderAccountRelation(std::map<int, UserAndTradeAccountRelation>& mapUserAndOrgRelation);
    void AddUserAndTraderAccountRelation(const UserAndTradeAccountRelation* pRelation);
    void GetUserAndTraderAccountRelation(std::map<int, UserAndTradeAccountRelation>& mapUserAndOrgRelation);

    //��֯������ί�н����˺�
    void ClearTradeAccountAndOrgRelation();
    void SetTradeAccountAndOrgRelation(const TradeAccountAndOrg& relation);
    void SetTradeAccountAndOrgRelation(std::map<int, TradeAccountAndOrg>& mapTradeAccountAndOrgRelation);
    string GetOrgByTradeAccountID(const int nTradeAccountID);


    //��Ʋ�Ʒ;
    void ClearFinanceProduct(); 
    void SetFinancialProduct(const FinancialProduct *pFP); 
    void GetFinancialProduct(std::map<int, FinancialProduct>& mapFinancialProduct);
    bool GetFinancialProduct(int nFinanceProductID, FinancialProduct& oFinancialProduct);
    string GetFinancialProductNameByID(int nProductID);
    int  GetFinancialProductIDByName(string& sName); 
    void DeleteFinanceProduct(int nProductID);
    bool IsValidOfProductName(const string& sName);  

    //��Ʋ�Ʒ�뽻��Ա�Ĺ�ϵ
    void ClearProductTraderRelation();
    void ClearProductTraderRelation(int nType, int nID);
    void SetProductTraderRelation(const TraderProRelation *pRelate); 
    void GetProTraderRelation(std::vector<TraderProRelation>& vec);
    void GetProductByTraderID(int nTraderID,std::set<int>& vec);
    string GetProductByUserID(int nUserID);
    
    //���ײ���
    void ClearTradeStrategy();
    void SetTradeStrategy(const TradeStrategy *pStrategy);
    void DeleteTradeStrategy(int nTradeStrategyID);    
    void GetTradeStrategy(std::map<int, TradeStrategy>&	mapTradeStrategy);
    bool GetTradeStrategyByID(int nTradeStrategyID, TradeStrategy& oStrategy);
    bool GetTradeStrategyByName(TradeStrategy& oStrategy,string& sName); 
    int  GetTradeStrategyIDByName(string& sName); 
    bool IsValidOfStrategyName(const string& sName);

    void SetAccountStatus(const sAccountStatus& sAS);
    EnumAccountStatus GetAccountStatus(const string& sAccount);

    //���ײ�������֯�����Ĺ�ϵ
    void ClearStrategyOrgRelation();
    void SetStrategyOrgRelation(std::map<int, std::vector<int>>& mapRelate);
    void SetStrategyOrgRelation(int nStrategyID, std::vector<int>& vecOrgID);
    void GetStrategyOrgRelation(std::vector<AssetOrgStrategyRelation>& vec);

    //�����
    void ClearFundInOut();
    void SetFundInOut(sFundInOut& oFundInOut);
    void GetStrategyOrgRelation(std::vector<sFundInOut>& vecFundInOut);

	//����
	void SetOriginalSettlementPrice(std::map<std::string,SettlementPriceField>&  lMapFiled);
	void SetCommissionMarginRate(std::map<RateKey,CommissionMarginRate>& lMapFiled );
	void ClearCommissionMarginRate();
	void GetAllProducts(set<string>& setProductID, int exchange); //�õ���ԼƷ��
    const string ExchangeID2String(const string& ExchangeID);
	const string ExchangeTabIndex2String(const int& ExchangeID);
	bool GetSettlementPrice(std::vector<SettlementPriceField>& data, const string& ProductID);
	double GetSettlementPrice(const string& InstrumentID);
	double GetLastSettlementPrice(const string& InstrumentID);
	bool ModifySettlementPrice(SettlementPriceField& Filed);
	bool ModifyLocalPrice(SettlementPriceField& Filed);

	bool CheckSettlePrice();
	void SetInstrumentInfo(std::map<std::string, PlatformStru_InstrumentInfo>& lMapFiled );
	bool GetCommissionMarginRate(std::vector<CommissionMarginRate>& vecRate ,int exchandeID,const std::string& AccountID,const std::string& BrokerID);
	bool GetCommissionMarginRate(std::vector<CommissionMarginRate>& vecRate);
    string GetExchangeID(string InstrumentID);
	bool GetBrokerIDByBrokerCode(std::string szName,int& brokerID);
	bool GetCommissionMarginRate(RateKey& key, CommissionMarginRate& Filed);
	void SetModifiedRate(const CommissionMarginRate *pFiled);
	void GetModifiedRate(std::map<RateKey,CommissionMarginRate>&  lMapFiled);
	void ModifyCommissionMarginRate(CommissionMarginRate& Field );
	void ModifyCommissionMarginRate();
	void CDataInfo::ClearModifiedRate();
	bool CheckRate();
	const string ExchangeName2ID( const string& name);
	void SetTrade(std::map<TradeKey,PlatformStru_TradeInfo>& lMapFiled );
	void ClearTrade();
	bool HaveModifiedRate();
	void ModifyTrade(PlatformStru_TradeInfo&  originaldata, PlatformStru_TradeInfo&  newdata);
	void AddTrade(PlatformStru_TradeInfo& field);
	void DeleteTrade(PlatformStru_TradeInfo& field);
	bool GetInstrumentFieldByProductID(string strID, vector<PlatformStru_InstrumentInfo>& setInstrumentField);
	void GetAllProducts(set<string>& setProductID ); //�õ���ԼƷ��
	void GetTrade(std::vector<PlatformStru_TradeInfo> &vecTrade,const std::string& UserID);
	const char* Direction2String(char param);
	const char* OffsetFlag2String(char param);
	const char* HedgeFlag2String(char param);
	//const char* TradeType2String(char TradeType);
	void SetPositionDetail(std::multimap<std::string,PlatformStru_PositionDetail>& lMapFiled );
	void ClearPositionDetail();
	void SetPosition(std::multimap<std::string,PlatformStru_Position>& lMapFiled );
	void ClearPosition();
	void GetPositionDetail(std::vector<PlatformStru_PositionDetail>& vecField );
	void GetPosition(std::vector<PlatformStru_Position>& vecField );
	void SetFund(std::map<std::string,PlatformStru_TradingAccountInfo>& lMapFiled );
	void SetFund(PlatformStru_TradingAccountInfo& Filed );
	void ClearFund();
	void SetModifiedFund(PlatformStru_TradingAccountInfo& Field);
	void GteModifiedFund(std::map<std::string,PlatformStru_TradingAccountInfo>& lMapFiled );
	void ClearModifiedFund();
	void GetFund(std::vector<PlatformStru_TradingAccountInfo>& vecField ,const std::string& UserID );
	const char HedgeFlag2ID( const string& name);
	const char Direction2ID( const string& name);
	bool IsExitIns(const string& Ins);
	bool GetInstrunmentInfobyID(const string& Ins, PlatformStru_InstrumentInfo& field);
	bool GetTradeAccountByUserID(std::string& strUserID, std::string& strBrokerID, std::string& strInvestID);
	bool GetRateByKey(RateKey& key,CommissionMarginRate& rate);
	bool GetPositionDetail(const string& userID,const string& traderID,PlatformStru_PositionDetail& field);
	void SetPositionDetail(const PlatformStru_PositionDetail *pField);
	const char* TradeDirection2String(char param);
	const char TradeDirection2ID( const string& name);
	const char* TradeType2String(char TradeType);
	const char TradeType2ID(const string& name);
	const char OffsetFlag2ID(const string& name);
	bool IsExitTradeID(TradeKey& key);
	bool GetTradeInfo(TradeKey& key, PlatformStru_TradeInfo& field);
	void SetTradeInfo(const PlatformStru_TradeInfo *pField);
	bool GetFund( PlatformStru_TradingAccountInfo&Filed,const std::string& UserID);

    void SetTradeDataStatus(eTradeDataStatus tradeDataStatus);
    eTradeDataStatus GetTradeDataStatus();
	bool IsCombination(string InstrumentID);//�������ȷ���£������Ϻ�Լ���ʿ���Ϊ��
	void GetQryRate(std::vector<RateKey>& vec,bool IsAll);
	void SetQryRate();
	void SetCommissionMarginRate(RateKey& key,CommissionMarginRate& Filed );
	void SaveSortedTrade(std::vector<PlatformStru_TradeInfo>& vecTrade);
	void GetSortedTrade(std::vector<PlatformStru_TradeInfo>& vecTrade);
	void SaveSortedPositionDetail(std::vector<PlatformStru_PositionDetail>& vecPD);
	void GetSortedPositionDetail(std::vector<PlatformStru_PositionDetail>& vecPD);
	bool CompareSettlePrice(string & InsName);

protected:
	void ResetDealerOrganRelationMap();
	void ResetDealerOrganRelation(const UserAndOrgRelation* pRelation);
	void DelDealerOrganRelation(int nDealerID, int nOrganID);

public:

    bool m_bRoleLoad;
    bool m_bPrivilegeLoad;
    bool m_bRolePrivilegeRelationLoad;
    bool m_bUserRoleRelationLoad;

private:	

    CPopMsgWin  *m_PopMsgWin;//��Ϣ������Ϣ����

    sLoginRsp m_loginRsp;	//�˻���Ϣ

    std::map<int, std::map<int, Organization>>      m_mapAssetMgmtOrgTree;//һ����֯����ID�µ�������֯������Ϣ
	std::map<int, Organization>	                    m_mapOrganization;//��֯����
    std::map<int, UserInfo>                         m_mapUserInfo;//�û�   
    std::map<int, RoleInfo>                         m_mapRoleInfo;//��ɫ   
    std::map<int, std::vector<UserRole>>            m_mapUserRole;//�û����ɫ��ϵ
	std::vector<UserAndOrgRelation>	                m_vecUserAndOrgRelation; //�û�����֯������ϵ
    std::map<int, UserAndTradeAccountRelation>      m_mapUserAndTradeAccountRelation;//�û���ί�н����˻���ϵ
    std::map<int, TradeAccountAndOrg>               m_mapTradeAccountAndOrgRelation;//��֯������ί�н����˻���ϵ

    std::map<int, set<int>>							m_mapDealerOrganRelation;	//�û�����֯�����Ĺ�ϵmap


    std::map<int, Privilege>                        m_mapPrivilege;//Ȩ��
    std::map<int,std::vector<int>>                  m_mapRoleID2Privilege;//��ɫ��Ȩ�޹�ϵ
    std::map<int, std::vector<Privilege>>           m_mapUserType2Privilege;//�û�������Ȩ�޹�ϵ
    std::map<int, std::vector<Privilege>>           m_mapParentID2Privilege;//Ȩ�޽ڵ��ϵ
	
	std::map<int, BrokerInfo>                       m_mapBrokerInfo;//���͹�˾	
	std::map<int, std::map<int, ServerGroup>>       m_mapServerGroup;//��������	
	std::map<int, std::map<int, ServerAddress>>     m_mapServerAddr;//���׵�ַ   
    std::map<int, ServerAddressQuot>                m_mapServerAddrQuot;//�����ַ   
    std::map<int, TradeAccount>                     m_mapTradeAccount;//ί�н����˻�
    std::map<std::string,sAccountStatus>            m_mapAccount2Status;//ί�н����˺�ģ�鱣���˺�ʵʱ��¼״̬

    
    std::map<int, FinancialProduct>	                m_mapFinancialProduct;//��Ʋ�Ʒ 
    std::map<int, std::vector<TraderProRelation>>   m_mapTraderProRelation;//��Ʋ�Ʒ�뽻��Ա��ϵ 
    std::map<int, TradeStrategy>                    m_mapTradeStrategy; //���ײ���              
    std::map<int, std::vector<int>>                 m_mapStrategyOrgRelation;//���ײ�������֯�����Ĺ�ϵ    

    std::vector<sFundInOut>                         m_vecFundInOut;//�����

	std::map<std::string,SettlementPriceField>      m_mapLocalPrice;
	std::map<std::string,SettlementPriceField>      m_mapSettlePrice;
	std::map<RateKey,CommissionMarginRate>          m_mapCommissionMarginRate;
	std::map<std::string, PlatformStru_InstrumentInfo> m_mapInstrumentInfo;
	std::map<RateKey,CommissionMarginRate>          m_mapModifiedRate;
	std::map<TradeKey,PlatformStru_TradeInfo>       m_mapTrade;
	std::multimap<std::string,PlatformStru_PositionDetail>  m_mapPositionDetail;
	std::multimap<std::string,PlatformStru_Position>  m_mapPosition;
	std::map<std::string,PlatformStru_TradingAccountInfo>  m_mapFund;
	std::map<std::string,PlatformStru_TradingAccountInfo> m_mapModifyFund;
	std::vector<RateKey>  m_vecAllRate;
	std::vector<RateKey>  m_vecVacancyRate;
	std::vector<PlatformStru_TradeInfo>  m_vecSortedTrade;
	std::vector<PlatformStru_PositionDetail> m_vecSortedPositionDetail;

    eTradeDataStatus m_tradeDataStatus; //����״̬
	

};
