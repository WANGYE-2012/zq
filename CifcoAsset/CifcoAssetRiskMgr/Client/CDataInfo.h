#pragma once
#include "RiskManageCmd.h"
#include "RiskManageStruct.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"
#include "TcpLayer.h"
#include <vector>
#include <map>
#include <set>
using namespace std;

typedef std::map<int, AssetMgmtOrganization>				  MAPAssetMgmtOrg;
typedef std::map<int, std::map<int, AssetMgmtOrganization>>   MAPAeestMgmtOrganization;
typedef std::map<int, FinancialProduct>						  MAPFinancialProduct;
typedef std::map<int, RiskIndicatorModule>					  MAPRiskIndicatorModule;
typedef std::map<int, RiskIndicator>						  MAPRiskIndicator;
typedef std::map<int, TrustTradeAccount>					  MAPTrustTradeAccount;
typedef std::map<string, TrustTradeAccount>					  MAPTrustTradeAccountByName;
typedef std::map<int, TraderInfo>							  MAPTraderInfo;

typedef std::multimap<int, TrustTradeAccount>				  MAPOrgID2TrustTradeAccount;  //������֯������Ӧ�Ľ����˺�
typedef std::multimap<int, TrustTradeAccount>				  MAPFinancialID2TrustTradeAccount;  //������Ʋ�Ʒ��Ӧ�Ľ����˺�

typedef std::map<string, InstrumentField>					  MAPInstrumentID2InstrumentField; //��Լidӳ�䵽��Լ����
typedef std::multimap<string, InstrumentField>				  MAPProductID2InstrumentField;    //���ݲ�Ʒ����ӳ�䵽��Լ����

typedef std::map<int, PermissionValue>						  MAPID2PermissionValue;

typedef std::multimap<int, RiskEventHandling>				  MAPAccountID2RiskEventHandling;


typedef std::multimap<int, RiskMgmtUserInfo>				  MAPOrgID2RiskMgmtUserInfo;


typedef multimap<int, RiskEvent>  MAPInt2RiskEvent;
struct RiskEventHistory
{
	RiskEvent				riskEvent;	
	RSTIME					lEventTime;//������ʧʱ��	
	void set(RiskEvent&	rEvent, RSTIME lTime)
	{
		riskEvent.nRiskEventID		= rEvent.nRiskEventID;
		riskEvent.lEventTime		= rEvent.lEventTime;	
		riskEvent.nTradeAccountID	= rEvent.nTradeAccountID;
		strcpy(riskEvent.InstrumentID, rEvent.InstrumentID);
		riskEvent.nRiskIndModuleID	= rEvent.nRiskIndModuleID;
		riskEvent.nRiskIndicatorID	= rEvent.nRiskIndicatorID;
		riskEvent.nRiskLevelID		= rEvent.nRiskLevelID;
		riskEvent.dblIndicatorValue	= rEvent.dblIndicatorValue;
		riskEvent.nMsgSendStatus	= rEvent.nMsgSendStatus;
		riskEvent.nIsValid			= rEvent.nIsValid;
	}
};

typedef map<int, RiskEventHistory>  MAPInt2RiskEventHistory;

struct RiskWarningKey
{
	int	  nRiskIndModuleID; //����ģ��
	int   nRiskIndicatorID; //����ָ��	
	int	  nRiskLevelID;		//���յȼ�
	bool operator<(const RiskWarningKey item) const
	{
		if(nRiskIndModuleID < item.nRiskIndModuleID)
			return true;
		else if(nRiskIndModuleID > item.nRiskIndModuleID)
			return false;

		if(nRiskIndicatorID < item.nRiskIndicatorID)
			return true;
		else if(nRiskIndicatorID > item.nRiskIndicatorID)
			return false;

		if(nRiskLevelID < item.nRiskLevelID)
			return true;
		else  if(nRiskLevelID > item.nRiskLevelID)
			return false;
		
		return false;
	}
};

typedef map<RiskWarningKey, RiskWarning>  MAPRiskWarning;

struct ProductRiskWarningKey
{
	int				  nTradeAccountID;
	string			  szProductID;	//Ʒ������
	int				  nRiskIndicatorID;
	int				  nRiskLevelID;
	bool operator<(const ProductRiskWarningKey item) const
	{
		if(nTradeAccountID < item.nTradeAccountID)
			return true;
		else if(nTradeAccountID > item.nTradeAccountID)
			return false;

		int i = strcmp(szProductID.c_str(), item.szProductID.c_str());			
		if(i<0)
			return true;
		else if(i>0)
			return false;

		if(nRiskIndicatorID < item.nRiskIndicatorID)
			return true;
		else if(nRiskIndicatorID > item.nRiskIndicatorID)
			return false;

		if(nRiskLevelID < item.nRiskLevelID)
			return true;
		else if(nRiskLevelID > item.nRiskLevelID)
			return false;

		return false;
	}
};

typedef map<ProductRiskWarningKey, ProductRiskWarning>  MAPProductRiskWarningKey;

class CDataInfo
{
private:
	CDataInfo(){
		Clear();
	};
	~CDataInfo(){};
public:
	static CDataInfo* NewInstance();
	static void DestroyInstance();
	static CDataInfo* m_pInstance;


protected:	
	string  m_strUserName;					//��¼�û���
	string  m_strPwd;						//��¼����
	LoginRsp	m_loginRsp;					//��¼�ɹ��󷵻ص���Ϣ

	//�ʹ���֯
	MAPAeestMgmtOrganization	m_mapAssetMgmtOrgTree;	
	MAPAssetMgmtOrg				m_mapAssetMgmtOrg;//����ID����֯�����Ĺ�ϵ
	bool m_bAeestMgmtOrganization;	

	//��Ʋ�Ʒ
	MAPFinancialProduct			m_mapFinancialProduct;
	bool m_bFinancialProduct;	

	//����ָ��ģ�ͽṹ
	MAPRiskIndicatorModule		m_mapRiskIndicatorModule;
	bool m_bRiskIndicatorModule;	

	//����ָ��
	MAPRiskIndicator			m_mapRiskIndicator;
	bool m_bRiskIndicator;	

	//ί�н����˻��ṹ
	MAPTrustTradeAccount		m_mapTrustTradeAccount;
	MAPTrustTradeAccountByName	m_mapTrustTradeAccountByName;

	//������֯������Ӧ�Ľ����˺�
	MAPOrgID2TrustTradeAccount  m_multimapOrgID2TrustTradeAccount;

	//������Ʋ�Ʒ��Ӧ�Ľ����˺�
	MAPFinancialID2TrustTradeAccount m_multimapFinancialID2TrustTradeAccount;
	bool m_bTrustTradeAccount;

	//����Ա��Ϣ
	MAPTraderInfo				m_mapTraderInfo;
	bool m_bTraderInfo;

	//��ѯ�ʹ���֯����Ʋ�Ʒ��ϵ
	multimap<int, int>			m_mulmapMgmtOrg2Financial;
	bool m_bMgmtOrg2Financial;

	//���ݺ�ԼID�õ���Լ����
	MAPInstrumentID2InstrumentField			m_mapInstrumentID2InstrumentField;

	//���ݲ�ƷID�õ���Լ����
	MAPProductID2InstrumentField			m_mapProductID2InstrumentField;
	bool m_bInstrumentField;

	//�����ն�Ȩ��
	MAPID2PermissionValue					m_mapID2PermissionValue;
	bool m_bPermissionValue;

	//�˻������¼(�ǻ�������)
	MAPAccountID2RiskEventHandling			m_mapAccountID2RiskEventHandling;

	//��ѯĳ�ʹ���֯����������
	MAPOrgID2RiskMgmtUserInfo				m_mapOrgID2RiskMgmtUserInfo;
	bool m_bRiskMgmtUserInfo;

	//���վ�ʾ
	MAPRiskWarning					m_mapRiskWarning;
	bool m_bRiskWarning;
	
	MAPProductRiskWarningKey		m_mapProductRiskWarningKey;
	bool m_bProductRiskWarning;
public:	
	MAPInt2RiskEvent						m_mapCurRiskEvent;        //��ǰ�����¼�
	MAPInt2RiskEventHistory					m_mapHisEvent;
	map<int, __int64>						m_mapInt2Time;			  //�����¼�ID��Ӧ��ʧʱ��

public://��ȡ�������ݵ��ⲿ�ӿ�
	bool IsInit();//�ж��Ƿ��ʼ�����

	void GetLoginRsp(LoginRsp&	loginRsp);  //�õ���¼��Ϣ
	void GetPwd(string& strPwd);			//�õ���¼����
	void GetUserName(string& strUserName);	//�õ���¼�û���

	//�õ��ʹ���֯
	void GetAeestMgmtOrg(MAPAeestMgmtOrganization&		mapAssetMgmtOrgTree);
	//�����ʹ�ID�õ��ʹܽṹ
	bool GetAssetMgmtOrg(int nAssetMgmtOrgID, AssetMgmtOrganization& sOrg);

	//��Ʋ�Ʒ
	void GetFinancialProduct(MAPFinancialProduct&	mapFinancialProduct);
	//������Ʋ�ƷID�ŵõ���Ʋ�Ʒ�ṹ
	bool GetFinancialProduct(int nFinancialID, FinancialProduct& fP);

	//����ָ��ģ�ͽṹ
	void GetRiskIndicatorModule(MAPRiskIndicatorModule&	mapRiskIndicatorModule);	
	//���ݷ���ָ��ģ��ID�õ�����ָ��ģ�ͽṹ
	bool GetRiskIndicatorModule(int nRiskIndicatorModuleID, RiskIndicatorModule&	sRiskIndicatorModule);

	//����ָ��
	void GetRiskIndicator(MAPRiskIndicator&	mapRiskIndicator);
	//���ݷ���ָ��ID�õ�����ָ��ṹ
	bool GetRiskIndicator(int nRiskIndicator, RiskIndicator&	sRiskIndicator);

	//ί�н����˻�
	void GetTrustTradeAccount(MAPTrustTradeAccount&	mapTrustTradeAccount);
	//���ݽ����˺�ID�ŵõ������˺Žṹ
	bool GetTrustTradeAccount(int nTradeAccountID, TrustTradeAccount& fP);
	//���ݽ����˺����Ƶõ������˺Žṹ
	bool GetTrustTradeAccount(string strTradeAccountID, TrustTradeAccount& fP);

	//����Ա��Ϣ
	void GetTraderInfo(MAPTraderInfo&	mapTraderInfo);
	//���ݽ���ԱID�õ����׽ṹ
	bool GetTraderInfo(int nID, TraderInfo&	 sTraderInfo);

	/*
	ͨ����֯������Ų�ѯ��ص���Ʋ�Ʒ��
    ��������֯����ID
          �Ƿ�����ӻ�������Ʋ�Ʒ
          ������Ʋ�ƷID��set
    ����ֵ��int����Ʋ�Ʒ��������-1 ��ʾʧ��*/
	int GetFinancialProduct(int  nAssetMgmtOrg, bool bIncludeChildren, set<int>& setFPID);

	/*
	ͨ����֯������Ų�ѯ��صĽ����ʺš�
    ��������֯����ID
          �Ƿ�����ӻ����Ľ����˺�
          ���������˺�ID��set
    ����ֵ��int�������˺ŵ�������-1 ��ʾʧ��*/
	int GetTrustTradeAccountByOrgID(int  nAssetMgmtOrg, bool bIncludeChildren, set<int>& setFPID);
	int GetTrustTradeAccountByOrgID(int  nAssetMgmtOrg, bool bIncludeChildren, set<TrustTradeAccount>& setFPID);
	
	/*
	ͨ����Ʋ�Ʒ��Ų�ѯ��صĽ����ʺš�
    ��������Ʋ�ƷID
          ���������˺�ID��set
		  ����ֵ��int�������˺ŵ�������-1 ��ʾʧ��*/
	int GetTrustTradeAccountByFinancialID(int  nFinancialID, set<int>& setFPID);
	int GetTrustTradeAccountByFinancialID(int  nFinancialID, set<TrustTradeAccount>& setFPID);
	
	/*
	ͨ����֯������ź���Ʋ�Ʒ��Ų�ѯ��صĽ����ʺš�
	��������֯����ID
	�Ƿ�����ӻ����Ľ����˺�
	���������˺�ID��set
	����ֵ��int�������˺ŵ�������-1 ��ʾʧ��*/
	int GetTrustTradeAccountByOrgIDFinancialID(int  nAssetMgmtOrg, bool bIncludeChildren,int  nFinancialID, set<int>& setFPID);
	int GetTrustTradeAccountByOrgIDFinancialID(int  nAssetMgmtOrg, bool bIncludeChildren,int  nFinancialID, set<TrustTradeAccount>& setFPID);
	
	//ͨ����Լ��������غ�Լ��Ϣ
	bool GetInstrumentFieldByInstrumentID(string strID, InstrumentField& instrumentField);

	//ͨ��Ʒ�ִ�������غ�Լ��Ϣ
	bool GetInstrumentFieldByProductID(string strID, vector<InstrumentField>& setInstrumentField);
	
	//�������Ʒ�ִ���
	void GetAllProducts(set<string>& setProductID);

	//�õ����к�Լ
	void GetAllProducts(MAPInstrumentID2InstrumentField& mapID2InstrumentField);

	//�õ����й����Ȩ��
	void GetPermission(MAPID2PermissionValue&	mapPermissionValue);
	//�ж�ĳһȨ���Ƿ�����
	bool HasPermission(PermissionActionType PAType);

	//�õ�ĳһ�˺��£�ĳһ����ID�Ĵ����¼; ���nRiskEventID Ϊ-1�����ʾ��ȡ���˺������еķ����¼�
	bool GetRiskEvent(int nAccountID, int nRiskEventID, vector<RiskEventHandling>& vexRiskEventHandling);

	//ͨ����֯������ŵõ���һ��������һ���ķ��Ա�˻���
	int GetRiskMgmtUserInfoByOrgID(int  nAssetMgmtOrg, bool bIncludeChildren, vector<RiskMgmtUserInfo>& vec);
	void GetmapOrgID2RiskMgmtUserInfo(MAPOrgID2RiskMgmtUserInfo& mapOrgID2RiskMgmtUserInfo);

	//�õ����վ�ʾ
	bool GetRiskWarning(RiskWarningKey key, RiskWarning& rw);

	//�õ�Ʒ�ַ��վ�ʾ
	bool GetProductRiskWarning(ProductRiskWarningKey key, ProductRiskWarning& rw);

public://���û������ݵĽӿڣ�һ��ӷ�������ȡ����ʱ���õ�
	void Clear();
	
	void FinishAeestMgmtOrganization(bool bFinish);
	void FinishFinancialProduct(bool bFinish);
	void FinishRiskIndicatorModule(bool bFinish);
	void FinishRiskIndicator(bool bFinish);
	void FinishTrustTradeAccount(bool bFinish);
	void FinishTraderInfo(bool bFinish);
	void FinishMgmtOrg2Financial(bool bFinish);
	void FinishInstrumentField(bool bFinish);
	void FinishPermissionValue(bool bFinish);
	void FinishRiskMgmtUserInfo(bool bFinish);
	void FinishProductRiskWarning(bool bFinish);
	void FinishRiskWarning(bool bFinish);

	double GetinitProgress();

	void SetLoginRsp(LoginRsp	loginRsp);  //���õ�¼��Ϣ
	void SetPwd(string strPwd);				//���õ�¼����
	void SetUserName(string strUserName);	//���õ�¼�û���
	

	void SetAeestMgmtOrg(MAPAssetMgmtOrg&		mapAssetMgmtOrg);
	

	void SetFinancialProduct(FinancialProduct *pFP);
	void SetRiskIndicatorModule(RiskIndicatorModule *pRIM);
	

	void SetRiskIndicator(RiskIndicator *pRI);
	void SetTrustTradeAccount(TrustTradeAccount *pRI);

	void SetOrgID2TrustTradeAccount(TrustTradeAccount *pRI);
	void SetFinancialID2TrustTradeAccount(TrustTradeAccount *pRI);
	void SetTraderInfo(TraderInfo *pRI);

	void SetMulmapMgmtOrg2Financial(AssetOrgProRelation *pAopr);

	void SetInstrumentField(InstrumentField *pField);


	void SetRiskEvent(RiskEvent *pRiskEvent);

	void SetPermissionValue(PermissionValue* pValue);

	void SetRiskEventHandling(RiskEventHandling* pValue);

	void SetRiskMgmtUserInfo(RiskMgmtUserInfo *pValue);

	//����Ʒ�ֲ�ѯ����Ԥ������
	void SetProductRiskWarning(ProductRiskWarning* pValue);

	//���ò�ѯ����Ԥ��
	void SetRiskWarning(RiskWarning* pValue);
};
