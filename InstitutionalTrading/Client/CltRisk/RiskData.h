#pragma once
#include "CommonPkg.h"
#include "CommonStruct.h"
#include "RiskManageStruct.h"
#include "TcpLayer.h"
#include "TcpDataConnect.h"
#include "defineGridCols.h"
#include "Tools.h"

#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

typedef pair<int, int> Key_RiskPlan;

#define  NULL_QUERY_CONDITION    "��ѯ����Ϊ�գ������ò�ѯ����"

struct sForceClose  
{
    ///Ͷ�����ʺ�
    TThostFtdcAccountIDType	AccountID;
    ///��Լ����
    TThostFtdcInstrumentIDType	InstrumentID;
    ///��������
    TThostFtdcDirectionType		Direction;
    ///��ƽ��־
    TThostFtdcOffsetFlagType	OffsetFlag;
    ///����
    TThostFtdcVolumeType	Volume;   
    ///�۸�
    TThostFtdcPriceType	Price;
    ///��ͣ���
    TThostFtdcPriceType	UpperLimitPrice;
    ///��ͣ���
    TThostFtdcPriceType	LowerLimitPrice;
    ///��ƽ��
    TThostFtdcLargeVolumeType	OpenInterest;
    ///��ƽ����
    TThostFtdcLargeVolumeType	TodayOpenInterest;

    sForceClose()
    {
        memset(AccountID,0,sizeof(AccountID));
        memset(InstrumentID,0,sizeof(InstrumentID));
        Direction = THOST_FTDC_D_Sell;
        OffsetFlag = THOST_FTDC_OF_Close;
        Volume = 0;
        Price = 0.0f;
        UpperLimitPrice = 0.0f;
        LowerLimitPrice = 0.0f;
        OpenInterest = 0;
        TodayOpenInterest = 0;
    }
};

struct sInstrumentInfo
{
    //string  Exchange;           //����������
    int     Volume;             //�ɽ���
    double  fLastPrice;         //�������¼�
    double  fUpperLimitPrice;   //��ͣ��
    double  fLowerLimitPrice;   //��ͣ��
};

#define DataGridView_DepthMarket    "DepthMarket"
#define DataGridView_RiskEvent      "RiskEvent"
#define DataGridView_TradingAccount "TradingAccount"
#define DataGridView_AllOrder       "AllOrder"
#define DataGridView_Trade          "Trade"
#define DataGridView_UnTrade        "UnTrade"
#define DataGridView_Position       "Position"
#define DataGridView_ClosePosition  "ClosePosition"
#define DataGridView_NeedVerify     "NeedVerify"

enum eXmlDataType
{
    XML_TYPE_INST,      //��ѡ��Լ
    XML_TYPE_COLUMN,    //datagridview��
    XML_TYPE_INFO       //����ĺ�Լ��Ϣ
};

class CRiskData
{
public:
    static CRiskData* GetInstance();
    static void DestroyInstance();
    static CRiskData* m_pInstance;

private:
    CRiskData();
    ~CRiskData();


private:
    void InitData();

public:

    //�����ļ�
    void SetConfigFile(vector<string>& vecData, eXmlDataType eType);
    map<string,vector<string>> GetConfigFile(eXmlDataType eType);
//     void UpdaeConfigFile(string szKey,vector<string> vecLastData,eXmlDataType eType);

    //��¼��Ϣ
    void SetLogonInfo(const sLoginRsp& sloginInfo);
    void GetLogonInfo(sLoginRsp& sloginInfo);
    int  GetOrderRef();//���ֶ�ǿƽ�й�
    void SetRiskerAccount(string& riskAccount);
    string GetRiskAccount();

    //��֯����
    void SetOrganizationLoad(bool bLoad);
    bool GetOrganizationLoad();
    void ClearOrganization();
    void SetOrganization(map<int, Organization>& mapOrganization);
    void GetOrganization(map<int, map<int, Organization>>& mapAssetMgmtOrgTree);
    bool GetOrganizationByID(int nAssetMgmtOrgID, Organization& sOrg);
    bool GetOrganizationIDByName(string& szName, int& nID);
    bool GetOrganizationNameByID(int nID, string& szName);
    bool IsOrganizationID(int nOrgID);
    bool IsMonitorOrg(int nOrgID);
    void GetMonitorOrgByOrgID(int nOrgID,vector<int> vecMonitorOrgIDAll,vector<int>& vecMonitorOrgID);

    //�û�
    void SetUserInfoLoad(bool bLoad);
    bool GetUserInfoLoad();
    void ClearUserInfo();
    void SetUserInfo(map<int, UserInfo>& mapUserInfo);
    bool GetAllUserInfo(map<int, UserInfo>& mapUserInfo);
    bool GetUserNameByID(int nUserID, string& szUserName);
    bool GetUserInfoByAccount(const string& szAccount, UserInfo& userInfo);
    bool IsLimitTrade(int nUserID);
    bool IsManualVerify(int nUserID);
    void SetLimitTrade(const SLimitTrade* pData);
    void SetManualVerify(const SManualVerify* pData);
	bool GetUserInfoByID(int nUserID,UserInfo& userinfo);
	const std::string UserType2String(UserType & userType);
    bool IsTraderID(int nTraderID);
    void SetUserLoginStatus(map<string,bool>& mapLogin);
    void GetUserLoginStatus(map<string,bool>& mapLogin);

    //ί�н����˺�
    void ClearTradeAccount();
    void SetTradeAccount(map<int, TradeAccount>& mapTradeAccount);
    bool GetTradeAccountByID(int nTradeAccountID,string& szTradeAccount);

    //�û�����֯������ϵ
    void SetUserAndOrgRelationLoad(bool bLoad);
    bool GetUserAndOrgRelationLoad();
    void ClearUserAndOrgRelation();
    void SetUserAndOrgRelation(vector<UserAndOrgRelation>& vecUserAndOrgRelation);
    void GetUserAndOrgRelation(vector<UserAndOrgRelation>& vecUserAndOrgRelation);
    int  GetOrgIDByUserID(int nUserID);
    void GetOrgIDByUserID(int nUserID, eUSERANDORGRELATIONTYPE eType, vector<int>& vecOrgID);
    void GetUserInfosByOrgID(int nOrgID, UserType nUserType, vector<UserInfo>& vecUserInfo, bool bSubOrg);
    void GetUserInfosBySubOrgID(UserType nUserType, map<int, map<int, Organization>>& mapMain, map<int, Organization>& mapSub, vector<UserInfo>& vecUserInfo, bool bSubOrg);


    //�û���ί�н����˺�
    void SetUserAndTraderAccountRelationLoad(bool bLoad);
    bool GetUserAndTraderAccountRelationLoad();
    void ClearUserAndTraderAccountRelation();
    void SetUserAndTraderAccountRelation(map<int, UserAndTradeAccountRelation>& mapUserAndOrgRelation);
    void GetTradeAccountByTraderCode(const string& szTraderCode, string& szTradeAccount);//Ŀǰһ������Ա����ֻ��һ��ί�н����˺�

    //���ָ��
    void SetRiskIndicatorLoad(bool bLoad);
    bool GetRiskIndicatorLoad();
    void ClearRiskIndicator();
    void SetRiskIndicator(map<RiskIndicatorType,RiskIndicator>& mapRiskIndicator);
    void GetRiskIndicator(map<RiskIndicatorType,RiskIndicator>& mapRiskIndicator);
    bool GetIndicatorIDByName(string& szName, RiskIndicatorType& nID);
    bool GetIndicatorDescByID(RiskIndicatorType nIndicatorID, string& szRiskDesc);


    //��ط���
    void ClearRiskPlan();
    void SetRiskPlan(map<string,vector<RiskPlan>>& mapRiskPlan);
    void GetRiskPlan(map<string,vector<RiskPlan>>& mapRiskPlan);
    void SetRiskPlanStatus(RiskChange& riskChange);
    void SetKey2RiskPlan(map<Key_RiskPlan,vector<RiskPlan>>& mapKey2RiskPlan);
    bool GetKey2RiskPlan(Key_RiskPlan key,vector<RiskPlan>& vecRiskPlan);
    bool GetRiskPlan(Key_RiskPlan key,int nRiskLevel,RiskPlan& RiskPlan);
    bool GetRiskWaring(int nOrgID,RiskIndicatorType nRiskIndicatorType,int nRiskLevelID,RiskWaring& riskWaring);
    void DelRiskPlan(string& szOrgName,Key_RiskPlan key);
    void UpdateRiskPlan(string& szOrgName, vector<RiskPlan>& vecRiskPlan);
    void UpdateRiskPlan(Key_RiskPlan key, vector<RiskPlan>& vecRiskPlan);
    void SetNetFundParam(Key_RiskPlan key, const NetFundParam* pParam);

    //��ض���
    void SetRiskResponseLoad(bool bLoad);
    bool GetRiskResponseLoad();
    void ClearRiskResponse();
    void SetRiskResponse(vector<RiskResponse>& vecRiskResponse);
    void GetRiskResponse(vector<RiskResponse>& vecRiskResponse);

    //��ԼƷ��
    void SetInstrumentStyle(vector<SProduct>& vecProduct);
    void GetInstrumentStyle(vector<SProduct>& vecProduct);

    void SetExchange2Instrument(map<string, vector<string>>& mapExchange2Ist);
    void GetExchange2Instrument(map<string, vector<string>>& mapExchange2Ist);
    string GetExchangeByInstrument(const string& szInst);

    void SetInstrument2PriceTick(map<string, double>& mapInst2Tick);
    double GetPriceTickByInstID(const string& szInstID);

    bool bSubScribeInst(const string& szInst);
    void SubScribeInst(const string& szExchange,const string& szInst);
    void UnSubScribeInst(const string& szExchange,const string& szInst);
    void AnalyseSelectInst(vector<string> vecSrc, vector<string>& vecSub, vector<string>& vecUnSub);

    bool bSubScribeInst2(const string& szInst);
    void SubScribeInst2(const string& szExchange,const string& szInst);
    void UnSubScribeInst2(const string& szExchange,const string& szInst);

    void SetInstrumentInfo(const string& szInst, const sInstrumentInfo& sInstInfo);
    void GetInstrumentInfo(const string& szInst, sInstrumentInfo& sInstInfo);
    map<string, sInstrumentInfo> GetInstrumentInfo();

private:
    bool m_bOrganizationLoad;
    bool m_bUserInfoLoad;
    bool m_bUserAndOrgRelationLoad;
    bool m_bUserAndTraderAccountRelationLoad;
    bool m_bRiskIndicatorLoad;
    bool m_bRiskResponseLoad;

private:

    //�����ļ�
    map<string,vector<string>>                  m_mapExchange2Inst;//�����ļ� �����ļ����ֹ�����
    map<string,vector<string>>                  m_mapExchange2Inst2;//���׶��µ������ĺ�Լ(û�ж��ĵ�)
    map<string,vector<string>>                  m_mapDataGridView2Column;

    //��¼��Ϣ
    int                                         m_nManualForceCount;
    sLoginRsp                                   m_sLoginInfo;
    string                                      m_szRiskAccount;

    //��֯����
    map<int, map<int, Organization>>            m_mapAssetMgmtOrgTree;//һ����֯����ID�µ�������֯������Ϣ
    map<int, Organization>	                    m_mapOrganization;    

    //�û�
    map<int, UserInfo>                          m_mapUserInfo;
    map<string, bool>                           m_mapUserLoginStatus;

    //ί�н����˺�
    map<int, TradeAccount>                      m_mapTradeAccount;

    //�û�����֯������ϵ
    vector<UserAndOrgRelation>	                m_vecUserAndOrgRelation;

    //�û���ί�н����˻���ϵ
    map<int, UserAndTradeAccountRelation>       m_mapUserAndTradeAccountRelation;

    //���ָ��
    map<RiskIndicatorType, RiskIndicator>                     m_mapRiskIndicator;

    //��ط���
    map<string, vector<RiskPlan>>               m_mapRiskPlan;
    map<Key_RiskPlan,vector<RiskPlan>>          m_mapKey2RiskPlan;//Key = <nOrgID,nRiskIndicatorType>

    //��ض���
    vector<RiskResponse>                        m_vecRiskResponse;

public:
    //�����¼�	
    multimap<int, RiskEvent>				   m_mapCurRiskEvent;

private:
    //��ԼƷ��
    vector<SProduct>                            m_vecProduct;

    //���������Լ��ϵ
    map<string, vector<string>>                 m_mapExchange2Instrument; //�ӷ����������   

    //��Լ����Ϣ(�������)
    map<string, sInstrumentInfo>                m_mapInstrumentInfo;

    //��Լ����Ϣ(��С�䶯��λ)
    map<string, double>                         m_mapInstrument2PriceTick;

public:
	std::map <int,int>                          m_mapMessageID2RowID;

};