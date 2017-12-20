#pragma once
#include "CommonPkg.h"
#include "commonstruct.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "TcpLayer.h"
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include "CommonStructEx.h"

#define SELECT_ALL "ȫ��"
enum MODULE_NAME
{
    FINANCE_PRODUCT,
    TRADE_STRATEGY,
    TRADER_ACCOUNT
};

typedef std::pair<int, int> Key;
typedef std::multimap<int, StatisticsEntry> RiskMapStatisticMap;
typedef std::map<int, FinancialProduct>		 MAPFinancialProduct;
typedef std::map<int, std::map<int, Organization>>   MAPAeestMgmtOrganization;
typedef std::map<string, PlatformStru_InstrumentInfo> MAPInstrumentID2InstrumentField;
typedef std::multimap<string, PlatformStru_InstrumentInfo> MAPProductID2InstrumentField;
class CDataInfo
{
public:
	enum {	
		conInitQueryOrgan,				// ��ʼ��ѯ��֯����
		conInitQueryUser,				// ��ʼ��ѯ�û�
		conInitQueryOrganUserRelate,	// ��ʼ��ѯ�û�����֯��ϵ
		conInitQueryFinical,			// ��ʼ��ѯ�û�����֯��ϵ
		conInitQueryFinicalUser	,		// ��ʼ��ѯ�û�����֯��ϵ
		conInitQueryInstrument	,		// ��ʼ��Լ
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


	//�������Ʒ�ִ���
	void SetInstrumentField(PlatformStru_InstrumentInfo *pField);
	void GetAllProducts(set<string>& setProductID);

    //��֯����
    void ClearOrganization();
    void SetOrganization(std::map<int, Organization>& mapOrganization);
    void SetOrganization(const Organization* pOrganization);
  
    void GetOrganization(std::map<int, std::map<int, Organization>>& mapAssetMgmtOrgTree);
    bool GetOrganizationByID(int nAssetMgmtOrgID, Organization& sOrg);
    int  GetOrganizationIDByName(std::string& sName);
    string  GetOrganizationNameByID(int nOrgID);
    int  GetOrganizationScaleLevelByID(int nOrgID);
    bool IsValidOfOrganizationName(const string& sName);

    //�û�
    void ClearUserInfo();
    void SetUserInfo(std::map<int, UserInfo>& mapUserInfo);
    void SetUserInfo(int nUserID, bool bAssign);
    void GetUserInfo(std::map<int, UserInfo>& mapUserInfo);
    void AddUserInfo(const UserInfo* pUserInfo);
    
    bool GetUserInfoByUserID(int nUserID,UserInfo& sUserInfo);
    bool GetUserInfoByAccount(string szUserAccount,UserInfo& sUserInfo);
    int GetUserIDByAccount(string szUserAccount);
    int IsValidOfUserAccount(std::string strAccount, UserType nType);
    bool IsTraderTypeByUserID(int nUserID);
    void SetUserStatus(int nUserID);

    //�û�����֯������ϵ
    void ClearUserAndOrgRelation();
    void DelUserAndOrgRelation(int nID, int nUserType, int nRelationType);
    void ModifyUserAndOrgRelation(const UserAndOrgRelation& oRelation);
    void SetUserAndOrgRelation(std::vector<UserAndOrgRelation>& vecUserAndOrgRelation);
    void SetUserAndOrgRelation(const UserAndOrgRelation* pRelation);
    void GetUserAndOrgRelation(std::vector<UserAndOrgRelation>& vecUserAndOrgRelation);
    string GetOrgNameByUserID(int nUserID);

	bool GetDealerByOrganID(int nOrganID, set<int>& setDealerID);
	int GetTraderUserIDsByOrgID(int  nOrganID, bool bIncludeChildren, std::set<int>& setFPID);



   


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
    void GetTraderSetByProductID(int nProductID,std::vector<int>& vec);  
  

protected:
	void ResetDealerOrganRelationMap();
	void ResetDealerOrganRelation(const UserAndOrgRelation* pRelation);
	void DelDealerOrganRelation(int nDealerID, int nOrganID);

private:	

    sLoginRsp m_loginRsp;	//�˻���Ϣ

    std::map<int, std::map<int, Organization>>      m_mapAssetMgmtOrgTree;//һ����֯����ID�µ�������֯������Ϣ
	std::map<int, Organization>	                    m_mapOrganization;//��֯����
    std::map<int, UserInfo>                         m_mapUserInfo;//�û�   
    std::map<int, RoleInfo>                         m_mapRoleInfo;//��ɫ   
    std::map<int, std::vector<UserRole>>            m_mapUserRole;//�û����ɫ��ϵ
	std::vector<UserAndOrgRelation>	                m_vecUserAndOrgRelation; //�û�����֯������ϵ
    std::map<int, UserAndTradeAccountRelation>      m_mapUserAndTradeAccountRelation;//�û���ί�н����˻���ϵ

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
    
    std::map<int, FinancialProduct>	                m_mapFinancialProduct;//��Ʋ�Ʒ 
    std::map<int, std::vector<TraderProRelation>>   m_mapTraderProRelation;//��Ʋ�Ʒ�뽻��Ա��ϵ 
    std::map<int, TradeStrategy>                    m_mapTradeStrategy; //���ײ���              
    std::map<int, std::vector<int>>                 m_mapStrategyOrgRelation;//���ײ�������֯�����Ĺ�ϵ    


	
	//���ݺ�ԼID�õ���Լ����
	MAPInstrumentID2InstrumentField			m_mapInstrumentID2InstrumentField;

	//���ݲ�ƷID�õ���Լ����
	MAPProductID2InstrumentField			m_mapProductID2InstrumentField;


};
