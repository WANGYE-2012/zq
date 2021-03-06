#include "stdafx.h"
#include "RiskData.h"

CRiskData* CRiskData::m_pInstance = NULL;
int g_nCount = 0;
const static char* __UNKOWN__="未知";
//////////////////////////////////////////////////////////////////////////
CRiskData* CRiskData::GetInstance()
{
    g_nCount ++;
    if( m_pInstance == NULL ) 
    {
        m_pInstance = new CRiskData();
    }

    return m_pInstance;
}

void CRiskData::DestroyInstance()
{
    g_nCount --;
    if(g_nCount == 0 && m_pInstance != NULL) 
    {		
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

CRiskData::CRiskData()
{
    InitData();
}

CRiskData::~CRiskData()
{    
}
void CRiskData::InitData()
{
    m_nManualForceCount = 0;
    m_bOrganizationLoad = false;
    m_bUserInfoLoad = false;
    m_bUserAndOrgRelationLoad = false;
    m_bRiskIndicatorLoad = false;
    m_bRiskResponseLoad = false;

    m_mapInstrument2PriceTick.clear();
    m_mapInstrumentInfo.clear();
    m_mapExchange2Inst.clear();
    m_mapExchange2Inst2.clear();
    m_mapDataGridView2Column.clear();
    m_mapAssetMgmtOrgTree.clear();
    m_mapOrganization.clear();    
    m_mapUserInfo.clear();
    m_mapTradeAccount.clear();
    m_vecUserAndOrgRelation.clear();
    m_mapUserAndTradeAccountRelation.clear();
    
    m_vecProduct.clear();
    m_mapExchange2Instrument.clear();    
    m_vecRiskResponse.clear();

    memset(&m_sLoginInfo, 0, sizeof(sLoginRsp));
}
//////////////////////////////////////////////////////////////////////////
//配置文件
void CRiskData::SetConfigFile(vector<string>& vecData, eXmlDataType eType)
{
    for (vector<string>::iterator it=vecData.begin();it!=vecData.end();it++)
    {
        string szTemp = *it;
        int s=szTemp.find_first_not_of(" \t");
        int e=szTemp.find_last_not_of(" \t");
        szTemp=szTemp.substr(s,e-s+1);

        char pStr[5120];
        memset(pStr,0,5120);
        strcpy(pStr,szTemp.c_str());

        char strTemp1[5120], strTemp2[5120];
        memset(strTemp1,0,5120);
        memset(strTemp2,0,5120);
        sscanf( pStr, "%s%s", strTemp1, strTemp2);

        vector<string> arr;
        string word, text(strTemp2);
        while(1)                
        {               
            int pos = text.find(';');
            if( pos==0)
            {       
                text=text.substr(1);
                continue;
            }       
            if( pos<0)
            {
                if(text.length()>0)
                    arr.push_back(text);

                break;
            }       
            word = text.substr(0,pos);
            text = text.substr(pos+1);
            if(word.length()>0)
                arr.push_back(word);
        }
    
        if(eType == XML_TYPE_INST && !arr.empty())
            m_mapExchange2Inst.insert(make_pair(strTemp1,arr));
        else if(eType == XML_TYPE_COLUMN && !arr.empty())
            m_mapDataGridView2Column.insert(make_pair(strTemp1,arr));
        else if(eType == XML_TYPE_INFO)
        {
            sInstrumentInfo info;
            //info.Exchange = arr[0];
            info.Volume = atoi(arr[0].c_str());
            info.fLastPrice = atof(arr[1].c_str());
            info.fUpperLimitPrice = atof(arr[2].c_str());
            info.fLowerLimitPrice = atof(arr[3].c_str());

            m_mapInstrumentInfo.insert(make_pair(strTemp1,info));
        }
    }
}
map<string,vector<string>> CRiskData::GetConfigFile(eXmlDataType eType)
{
    if(eType == XML_TYPE_INST)
        return m_mapExchange2Inst;
    else if(eType == XML_TYPE_COLUMN)
        return m_mapDataGridView2Column;    
}

// void CRiskData::UpdaeConfigFile(string szKey,vector<string> vecLastData,eXmlDataType eType)
// {
//     if (XML_TYPE_INST == eType)
//     {
//         map<string,vector<string>>::iterator it_inst = m_mapExchange2Inst.find(szKey);
//         if(it_inst!=m_mapExchange2Inst.end())
//             m_mapExchange2Inst[szKey] = vecLastData;
// 
//     }else if (XML_TYPE_COLUMN == eType)
//     {
//         map<string,vector<string>>::iterator it_column = m_mapDataGridView2Column.find(szKey);
//         if(it_column!=m_mapDataGridView2Column.end())
//             m_mapDataGridView2Column[szKey] = vecLastData;
//     }
// }

//登录信息
void CRiskData::SetLogonInfo(const sLoginRsp& sloginInfo)
{
    memcpy(&m_sLoginInfo,&sloginInfo,sizeof(sLoginRsp));
}
void CRiskData::GetLogonInfo(sLoginRsp& sloginInfo)
{
    memcpy(&sloginInfo,&m_sLoginInfo,sizeof(sLoginRsp));
}
int CRiskData::GetOrderRef()
{
    return ++m_nManualForceCount;
}
void CRiskData::SetRiskerAccount(string& riskAccount)
{
    m_szRiskAccount = riskAccount;
}
string CRiskData::GetRiskAccount()
{
    return m_szRiskAccount;
}
//组织机构
void CRiskData::SetOrganizationLoad(bool bLoad)
{
    m_bOrganizationLoad = true;
}
bool CRiskData::GetOrganizationLoad()
{
    return m_bOrganizationLoad;
}
void CRiskData::ClearOrganization()
{
    m_mapOrganization.clear();
}
void CRiskData::SetOrganization(map<int, Organization>& mapAssetMgmtOrg)
{
    map<int, Organization>::iterator it_org = mapAssetMgmtOrg.begin();
    for ( ; it_org != mapAssetMgmtOrg.end(); it_org++ )
    {
        Organization info = it_org->second;
        if(m_mapAssetMgmtOrgTree.find(info.nAssetMgmtOrgID) == m_mapAssetMgmtOrgTree.end())
        {
            map<int, Organization> tempOrgMap;
            m_mapAssetMgmtOrgTree[info.nAssetMgmtOrgID] = tempOrgMap;
        }

        map<int, map<int, Organization>>::iterator it = m_mapAssetMgmtOrgTree.find(info.nUpperLevelOrgID);
        if ( it != m_mapAssetMgmtOrgTree.end() )
        {
            it->second[info.nAssetMgmtOrgID] = info;
        }
        else
        {
            map<int, Organization> tempOrgMap;
            tempOrgMap[info.nAssetMgmtOrgID] = info;
            m_mapAssetMgmtOrgTree[info.nUpperLevelOrgID] = tempOrgMap;
        }	
    }

    m_mapOrganization = mapAssetMgmtOrg;
}
void CRiskData::GetOrganization(map<int, map<int, Organization>>& mapAssetMgmtOrgTree)
{
    mapAssetMgmtOrgTree = m_mapAssetMgmtOrgTree;
}
bool CRiskData::GetOrganizationByID(int nOrganizationID, Organization& sOrg)
{
    map<int, Organization>::iterator it = m_mapOrganization.find(nOrganizationID);
    if(it == m_mapOrganization.end())
        return false;

    sOrg = it->second;
    return true;
}
bool CRiskData::GetOrganizationIDByName(string& szName, int& nID)
{
    map<int, Organization>::iterator it = m_mapOrganization.begin();
    for (;it!=m_mapOrganization.end();it++)
    {
        if( 0==strcmp(it->second.szName,szName.c_str()))
        {
            nID = it->first;
            return true;
        }
    }

    nID = -1;
    return false;
}
bool CRiskData::GetOrganizationNameByID(int nID, string& szName)
{
    map<int, Organization>::iterator it = m_mapOrganization.find(nID);
    if (it!=m_mapOrganization.end())
    {
        szName = it->second.szName;
        return true;
    }

    return false;
}
bool CRiskData::IsOrganizationID(int nOrgID)
{
    map<int, Organization>::iterator it = m_mapOrganization.find(nOrgID);
    if (it!=m_mapOrganization.end())
        return true;

    return false;
}
bool CRiskData::IsMonitorOrg(int nOrgID)
{
    vector<int> vecOrg;
    GetOrgIDByUserID(m_sLoginInfo.mnUserID,USER_ORG_RELATION_MONITOR,vecOrg);
    for (vector<int>::iterator it=vecOrg.begin();it!=vecOrg.end();it++)
    {
        if ((*it) == nOrgID)
            return true;
    }

    return false;
}
void CRiskData::GetMonitorOrgByOrgID(int nOrgID,vector<int> vecMonitorOrgIDAll,vector<int>& vecMonitorOrgID)
{
    if(IsMonitorOrg(nOrgID))
        vecMonitorOrgID.push_back(nOrgID);

    map<int, map<int, Organization>>::iterator it = m_mapAssetMgmtOrgTree.find(nOrgID);
    if(it != m_mapAssetMgmtOrgTree.end())
    {
        map<int, Organization> mapSubOrg = it->second;
        map<int, Organization>::iterator it_subOrg = mapSubOrg.begin();


        for (;it_subOrg!=mapSubOrg.end();it_subOrg++)
        {       
            int nSubOrgID = it_subOrg->first;
            GetMonitorOrgByOrgID(nSubOrgID,vecMonitorOrgIDAll,vecMonitorOrgID);

            vector<int>::iterator it_monitor = vecMonitorOrgIDAll.begin();
            for (;it_monitor!=vecMonitorOrgIDAll.end();it_monitor++)
            {
                if(nSubOrgID == *it_monitor)
                {
                    vecMonitorOrgID.push_back(nSubOrgID);
                    break;
                }
            }
        }
    }
}
//用户
void CRiskData::SetUserInfoLoad(bool bLoad)
{
    m_bUserInfoLoad = bLoad;
}
bool CRiskData::GetUserInfoLoad()
{
    return m_bUserInfoLoad;
}
void CRiskData::ClearUserInfo()
{
    m_mapUserInfo.clear();
}

void CRiskData::SetUserInfo(map<int, UserInfo>& mapUserInfo)
{
    m_mapUserInfo = mapUserInfo;
}

bool CRiskData::GetAllUserInfo(map<int, UserInfo>& mapUserInfo)
{
	mapUserInfo = m_mapUserInfo;
	return m_mapUserInfo.size()>0;
}

bool CRiskData::GetUserNameByID(int nUserID, string& szUserName)
{
    map<int,UserInfo>::iterator it = m_mapUserInfo.find(nUserID);
    if(it!=m_mapUserInfo.end())
    {
        szUserName=it->second.szAccount;
        return true;
    }

    return false;
}
bool CRiskData::GetUserInfoByAccount(const string& szAccount, UserInfo& userInfo)
{
    map<int,UserInfo>::iterator it = m_mapUserInfo.begin();
    for(;it!=m_mapUserInfo.end();it++)
    {
        if(0==strcmp(it->second.szAccount,szAccount.c_str()))
        {
            userInfo = it->second;
            return true;
        }        
    }

    return false;
}
bool CRiskData::GetUserInfoByID(int nUserID,UserInfo& userinfo)
{
	map<int,UserInfo>::iterator it = m_mapUserInfo.find(nUserID);
	if(it!=m_mapUserInfo.end())
	{
		userinfo=it->second;
		return true;
	}

	return false;

}
const std::string CRiskData::UserType2String(UserType & userType)
{
	const string MANAGE="管理员";
	const string RISK_CONTROL="风控员";
	const string TRADER="交易员";
	const string VIEWER="展现员";
	const string INVALID="无效的用户类型";

	if(userType==USER_TYPE_MANAGE)
		return MANAGE;
	else if(userType==USER_TYPE_RISK_CONTROL)
		return RISK_CONTROL;
	else if(userType==USER_TYPE_TRADER)
		return TRADER;
	else if(userType==USER_TYPE_VIEWER)
		return VIEWER;
	else if(userType==USER_TYPE_INVALID)
		return INVALID;
	else
		return __UNKOWN__;
}
bool CRiskData::IsLimitTrade(int nUserID)
{
    map<int, UserAndTradeAccountRelation>::iterator it = m_mapUserAndTradeAccountRelation.find(nUserID);
    if(it!=m_mapUserAndTradeAccountRelation.end())
    {
        if(it->second.nCanInputOrder)
            return false;
    }

    return true;
}
bool CRiskData::IsManualVerify(int nUserID)
{
    map<int, UserAndTradeAccountRelation>::iterator it = m_mapUserAndTradeAccountRelation.find(nUserID);
    if(it!=m_mapUserAndTradeAccountRelation.end())
    {
        if(it->second.nNeedVerify)
            return true;
    }

    return false;
}
void CRiskData::SetLimitTrade(const SLimitTrade* pData)
{
    map<int, UserAndTradeAccountRelation>::iterator it = m_mapUserAndTradeAccountRelation.find(pData->nUserID);
    if(it!=m_mapUserAndTradeAccountRelation.end())
        it->second.nCanInputOrder = (pData->bLimitTrade)?0:1;
}
void CRiskData::SetManualVerify(const SManualVerify* pData)
{
    map<int, UserAndTradeAccountRelation>::iterator it = m_mapUserAndTradeAccountRelation.find(pData->nUserID);
    if(it!=m_mapUserAndTradeAccountRelation.end())
        it->second.nNeedVerify = (int)pData->bManualVerify;
}
//用户与组织机构关系
void CRiskData::SetUserAndOrgRelationLoad(bool bLoad)
{
    m_bUserAndOrgRelationLoad = true;
}
bool CRiskData::GetUserAndOrgRelationLoad()
{
    return m_bUserAndOrgRelationLoad;
}
void CRiskData::ClearUserAndOrgRelation()
{
    m_vecUserAndOrgRelation.clear();
}
void CRiskData::SetUserAndOrgRelation(vector<UserAndOrgRelation>& vecUserAndOrgRelation)
{
    m_vecUserAndOrgRelation = vecUserAndOrgRelation;
}
void CRiskData::GetUserAndOrgRelation(vector<UserAndOrgRelation>& vecUserAndOrgRelation)
{
    vecUserAndOrgRelation = m_vecUserAndOrgRelation;      
}
int CRiskData::GetOrgIDByUserID(int nUserID)
{
    vector<UserAndOrgRelation>::iterator it = m_vecUserAndOrgRelation.begin();
    for ( ;it != m_vecUserAndOrgRelation.end(); it++ )
    {
        if( nUserID == it->nUserID && it->nRelationType == USER_ORG_RELATION_ATTACH)
            return it->nOrgID;
    }    

    return -1;
}
void CRiskData::GetOrgIDByUserID(int nUserID, eUSERANDORGRELATIONTYPE eType, vector<int>& vecOrgID)
{
    vector<UserAndOrgRelation>::iterator it = m_vecUserAndOrgRelation.begin();
    for ( ;it != m_vecUserAndOrgRelation.end(); it++ )
    {
        if( nUserID == it->nUserID)
        {
            if(eType == it->nRelationType)
                vecOrgID.push_back(it->nOrgID);
            else if(eType == USER_ORG_RELATION_ALL)
                vecOrgID.push_back(it->nOrgID);
        }
    }
}
void CRiskData::GetUserInfosBySubOrgID(UserType nUserType, map<int, map<int, Organization>>& mapMain, map<int, Organization>& mapSub, vector<UserInfo>& vecUserInfo, bool bSubOrg)
{
    map<int, Organization>::iterator itSub = mapSub.begin();
    for(; itSub != mapSub.end(); itSub++) 
    {
        vector<UserInfo> vecUserInfo2;
        CRiskData::GetInstance()->GetUserInfosByOrgID(itSub->first,nUserType,vecUserInfo2,bSubOrg);
        if(!vecUserInfo2.empty())
        {
            vector<UserInfo>::iterator itUserInfo = vecUserInfo2.begin();
            for (;itUserInfo!=vecUserInfo2.end();itUserInfo++)
                vecUserInfo.push_back(*itUserInfo);
        }

//         map<int, map<int, Organization>>::iterator it = mapMain.find(itSub->first);
//         if(it != mapMain.end())
//             GetUserInfosBySubOrgID(nUserType, mapMain, it->second, vecUserInfo, bSubOrg);
    }
}
void CRiskData::GetUserInfosByOrgID(int nOrgID, UserType nUserType, vector<UserInfo>& vecUserInfo, bool bSubOrg)
{
    vector<UserAndOrgRelation>::iterator it_Rela=m_vecUserAndOrgRelation.begin();
    for(;it_Rela!=m_vecUserAndOrgRelation.end();it_Rela++)
    {
        if(it_Rela->nOrgID == nOrgID)
        {
            map<int,UserInfo>::iterator itUserInfo=m_mapUserInfo.find(it_Rela->nUserID);
            if(itUserInfo!=m_mapUserInfo.end() && itUserInfo->second.nUserType==nUserType)
                vecUserInfo.push_back(itUserInfo->second);
        }
    }

    if(!bSubOrg)
        return;

    map<int, map<int, Organization>> mapAssetMgmtOrgTree;
    CRiskData::GetInstance()->GetOrganization(mapAssetMgmtOrgTree);

    map<int, map<int, Organization>>::iterator it = mapAssetMgmtOrgTree.find(nOrgID);
    if(it != mapAssetMgmtOrgTree.end())
    {
        map<int, Organization>& mapSub = it->second;
        if(mapSub.begin()==mapSub.end())
            return;

        GetUserInfosBySubOrgID(nUserType, mapAssetMgmtOrgTree, mapSub, vecUserInfo, bSubOrg);
    }


}
bool CRiskData::IsTraderID(int nTraderID)
{
    map<int, UserInfo>::iterator it = m_mapUserInfo.find(nTraderID);
    if (it!=m_mapUserInfo.end())
        return true;

    return false;
}
void CRiskData::SetUserLoginStatus(map<string,bool>& mapLogin)
{
    m_mapUserLoginStatus = mapLogin;
}
void CRiskData::GetUserLoginStatus(map<string,bool>& mapLogin)
{
    mapLogin = m_mapUserLoginStatus;
}
//委托交易账号
void CRiskData::ClearTradeAccount()
{
    m_mapTradeAccount.clear();
}
void CRiskData::SetTradeAccount(map<int, TradeAccount>& mapTradeAccount)
{
    m_mapTradeAccount = mapTradeAccount;
}
bool CRiskData::GetTradeAccountByID(int nTradeAccountID,string& szTradeAccount)
{
    map<int, TradeAccount>::iterator it = m_mapTradeAccount.find(nTradeAccountID);
    if (it==m_mapTradeAccount.end())
        return false;
    
    szTradeAccount = string(it->second.szTradeAccount);
    return true;
}

//用户与委托交易账号
void CRiskData::SetUserAndTraderAccountRelationLoad(bool bLoad)
{
    m_bUserAndTraderAccountRelationLoad = true;
}
bool CRiskData::GetUserAndTraderAccountRelationLoad()
{
    return m_bUserAndTraderAccountRelationLoad;
}
void CRiskData::ClearUserAndTraderAccountRelation()
{
    m_mapUserAndTradeAccountRelation.clear();
}
void CRiskData::SetUserAndTraderAccountRelation(map<int, UserAndTradeAccountRelation>& mapUserAndOrgRelation)
{
    m_mapUserAndTradeAccountRelation = mapUserAndOrgRelation;
}
void CRiskData::GetTradeAccountByTraderCode(const string& szTraderCode, string& szTradeAccount)
{
    UserInfo userInfo;
    if(!GetUserInfoByAccount(szTraderCode, userInfo))
        return;

    map<int, UserAndTradeAccountRelation>::iterator it = m_mapUserAndTradeAccountRelation.find(userInfo.nUserID);
    if(it!=m_mapUserAndTradeAccountRelation.end())
    {
        int nTradeAccountID = it->second.nTradeAccountID;
        GetTradeAccountByID(nTradeAccountID, szTradeAccount);
    }
}
//风控指标
void CRiskData::SetRiskIndicatorLoad(bool bLoad)
{
    m_bRiskIndicatorLoad = bLoad;
}
bool CRiskData::GetRiskIndicatorLoad()
{
    return m_bRiskIndicatorLoad;
}

void CRiskData::ClearRiskIndicator()
{
    m_mapRiskIndicator.clear();
}
void CRiskData::SetRiskIndicator(map<RiskIndicatorType,RiskIndicator>& mapRiskIndicator)
{
    m_mapRiskIndicator = mapRiskIndicator;
}
void CRiskData::GetRiskIndicator(map<RiskIndicatorType,RiskIndicator>& mapRiskIndicator)
{
    mapRiskIndicator = m_mapRiskIndicator;
}
bool CRiskData::GetIndicatorIDByName(string& szName, RiskIndicatorType& nID)
{
    map<RiskIndicatorType, RiskIndicator>::iterator it = m_mapRiskIndicator.begin();
    for (;it!=m_mapRiskIndicator.end();it++)
    {
        if(szName == it->second.szName)
        {
            nID = it->first;
            return true;
        }
    }

    return false;
}
bool CRiskData::GetIndicatorDescByID(RiskIndicatorType nIndicatorID, string& szRiskDesc)
{
    map<RiskIndicatorType, RiskIndicator>::iterator it = m_mapRiskIndicator.find(nIndicatorID);
    if (it!=m_mapRiskIndicator.end())
    {
        szRiskDesc = it->second.szAlgorithmDesc;
        return true;
    }

    return false;
}

//风控方案
void CRiskData::ClearRiskPlan()
{
    m_mapRiskPlan.clear();
}
void CRiskData::SetRiskPlan(map<string,vector<RiskPlan>>& mapRiskPlan)
{
    m_mapRiskPlan = mapRiskPlan;
}
void CRiskData::GetRiskPlan(map<string,vector<RiskPlan>>& mapRiskPlan)
{
    mapRiskPlan = m_mapRiskPlan;
}
void CRiskData::SetRiskPlanStatus(RiskChange& riskChange)
{   
    string szOrgName;
    if(GetOrganizationNameByID(riskChange.nOrgID,szOrgName))
    {
        map<string,vector<RiskPlan>>::iterator it_plan = m_mapRiskPlan.find(szOrgName);
        if(it_plan!=m_mapRiskPlan.end())
        {
            vector<RiskPlan>& vecRiskPlan = it_plan->second;
            for (vector<RiskPlan>::iterator it = vecRiskPlan.begin();it!=vecRiskPlan.end();it++)
                it->OrgIDPlanRelation.bUse = riskChange.bUse;
        }           
    }
       

    Key_RiskPlan key;
    key.first = riskChange.nOrgID;
    key.second = riskChange.nRiskIndicatorID;
    map<Key_RiskPlan,vector<RiskPlan>>::iterator it_vec = m_mapKey2RiskPlan.find(key);
    if(it_vec!=m_mapKey2RiskPlan.end())
    {
        vector<RiskPlan>& vecRiskPlan = it_vec->second;
        for (vector<RiskPlan>::iterator it = vecRiskPlan.begin();it!=vecRiskPlan.end();it++)
            it->OrgIDPlanRelation.bUse = riskChange.bUse;
    }
}
void CRiskData::SetKey2RiskPlan(map<Key_RiskPlan,vector<RiskPlan>>& mapKey2RiskPlan)
{
    m_mapKey2RiskPlan = mapKey2RiskPlan;
}
bool CRiskData::GetKey2RiskPlan(Key_RiskPlan key,vector<RiskPlan>& vecRiskPlan)
{
    map<Key_RiskPlan,vector<RiskPlan>>::iterator it = m_mapKey2RiskPlan.find(key);
    if(it!=m_mapKey2RiskPlan.end())
    {
        vecRiskPlan = it->second;
        return true;
    }

    return false;
}
bool CRiskData::GetRiskPlan(Key_RiskPlan key,int nRiskLevel,RiskPlan& riskPlan)
{
    map<Key_RiskPlan,vector<RiskPlan>>::iterator it = m_mapKey2RiskPlan.find(key);
    if(it!=m_mapKey2RiskPlan.end())
    {
        vector<RiskPlan>& vecRiskPlan = it->second;
        for(vector<RiskPlan>::iterator it_plan=vecRiskPlan.begin();it_plan!=vecRiskPlan.end();it_plan++)
        {
            if(it_plan->WaringLevel.nRiskLevelID == nRiskLevel)
            {
                riskPlan = *it_plan;
                return true;
            }
        }
    }
    else
    {
        //查找父组织机构，直到查到为止
        Organization sOrg;
        if(CRiskData::GetInstance()->GetOrganizationByID(key.first,sOrg))
        {
            Key_RiskPlan key2;
            key2.first = sOrg.nUpperLevelOrgID;
            key2.second = key.second;

            return GetRiskPlan(key2,nRiskLevel,riskPlan);
        }
    }

    return false;
}
bool CRiskData::GetRiskWaring(int nOrgID,RiskIndicatorType nRiskIndicatorType,int nRiskLevelID,RiskWaring& riskWaring)
{
    return false;
}
void CRiskData::DelRiskPlan(string& szOrgName,Key_RiskPlan key)
{
    map<string,vector<RiskPlan>>::iterator it = m_mapRiskPlan.find(szOrgName);
    if(it!=m_mapRiskPlan.end())
        m_mapRiskPlan.erase(it);

    map<Key_RiskPlan,vector<RiskPlan>>::iterator it2 = m_mapKey2RiskPlan.find(key);
    if(it2!=m_mapKey2RiskPlan.end())
        m_mapKey2RiskPlan.erase(it2);
}
void CRiskData::UpdateRiskPlan(string& szOrgName, vector<RiskPlan>& vecRiskPlan)
{
    map<string,vector<RiskPlan>>::iterator it = m_mapRiskPlan.find(szOrgName);
    if(it!=m_mapRiskPlan.end())
        m_mapRiskPlan.erase(it);


    vector<RiskPlan>::iterator it_plan = vecRiskPlan.begin();
    for(;it_plan!=vecRiskPlan.end();it_plan++)
        m_mapRiskPlan[szOrgName].push_back(*it_plan);
}
void CRiskData::UpdateRiskPlan(Key_RiskPlan key, vector<RiskPlan>& vecRiskPlan)
{
    map<Key_RiskPlan,vector<RiskPlan>>::iterator it = m_mapKey2RiskPlan.find(key);
    if(it!=m_mapKey2RiskPlan.end())
        m_mapKey2RiskPlan.erase(it);

    vector<RiskPlan>::iterator it_plan = vecRiskPlan.begin();
    for(;it_plan!=vecRiskPlan.end();it_plan++)
        m_mapKey2RiskPlan[key].push_back(*it_plan);
}
void CRiskData::SetNetFundParam(Key_RiskPlan key, const NetFundParam* pParam)
{
    map<Key_RiskPlan,vector<RiskPlan>>::iterator it = m_mapKey2RiskPlan.find(key);
    if(it!=m_mapKey2RiskPlan.end())
    {
        vector<RiskPlan>::iterator it_plan = it->second.begin();
        for(;it_plan!=it->second.end();it_plan++)
            memcpy(&(it_plan->netFundParam),pParam,sizeof(NetFundParam));
    }


}   

//风控动作
void CRiskData::SetRiskResponseLoad(bool bLoad)
{
    m_bRiskResponseLoad = bLoad;
}
bool CRiskData::GetRiskResponseLoad()
{
    return m_bRiskResponseLoad;
}

void CRiskData::ClearRiskResponse()
{
    m_vecRiskResponse.clear();
}
void CRiskData::SetRiskResponse(vector<RiskResponse>& vecRiskResponse)
{
    m_vecRiskResponse = vecRiskResponse;
}
void CRiskData::GetRiskResponse(vector<RiskResponse>& vecRiskResponse)
{
    vecRiskResponse = m_vecRiskResponse;
}

//合约品种
void CRiskData::SetInstrumentStyle(vector<SProduct>& vecProduct)
{
    m_vecProduct = vecProduct;
}
void CRiskData::GetInstrumentStyle(vector<SProduct>& vecProduct)
{
    vecProduct = m_vecProduct;
}
void CRiskData::SetExchange2Instrument(map<string, vector<string>>& mapExchange2Ist)
{
    m_mapExchange2Instrument = mapExchange2Ist;
}
void CRiskData::GetExchange2Instrument(map<string, vector<string>>& mapExchange2Ist)
{
    mapExchange2Ist = m_mapExchange2Instrument;
}
string CRiskData::GetExchangeByInstrument(const string& szInst)
{
    string szExchange,szInstrument;
    map<string, vector<string>>::iterator it = m_mapExchange2Instrument.begin();
    for (;it!=m_mapExchange2Instrument.end();++it)
    {
        szExchange = it->first;
        vector<string> vecInst = it->second;
        for (vector<string>::iterator it_inst = vecInst.begin();it_inst!=vecInst.end();++it_inst)
        {
            szInstrument = *it_inst;
            if (szInstrument == szInst)
                return szExchange;
        }
    }

    return "";
}
void CRiskData::SetInstrument2PriceTick(map<string, double>& mapInst2Tick)
{
    m_mapInstrument2PriceTick = mapInst2Tick; 
}
double CRiskData::GetPriceTickByInstID(const string& szInstID)
{
    double fValue = 0.001;
    map<string, double>::iterator it = m_mapInstrument2PriceTick.find(szInstID);
    if (it!=m_mapInstrument2PriceTick.end())
    {
        fValue = it->second;
    }

    return fValue;
}

bool CRiskData::bSubScribeInst(const string& szInst)
{
    map<string,vector<string>>::iterator it = m_mapExchange2Inst.begin();
    for (;it!=m_mapExchange2Inst.end();it++)
    {
        vector<string>::iterator it_inst = it->second.begin();
        for (;it_inst!=it->second.end();it_inst++)
        {
            if(0==strcmp((*it_inst).c_str(),szInst.c_str()))
                return true;
        }
    }

    return false;
}
void CRiskData::SubScribeInst(const string& szExchange,const string& szInst)
{
    map<string,vector<string>>::iterator it = m_mapExchange2Inst.find(szExchange);
    if(it==m_mapExchange2Inst.end())
    {
        m_mapExchange2Inst[szExchange].push_back(szInst);
        return;
    }

    vector<string>& vec = it->second;
    vec.push_back(szInst);
}
void CRiskData::UnSubScribeInst(const string& szExchange,const string& szInst)
{
    map<string,vector<string>>::iterator it = m_mapExchange2Inst.find(szExchange);
    if(it==m_mapExchange2Inst.end())
        return;

    vector<string>& vec = it->second;
    vector<string>::iterator it_inst = vec.begin();
    while (it_inst != vec.end())
    {
        string szDest = *it_inst;
        if(0==szDest.compare(szInst))
        {
            it_inst = vec.erase(it_inst);
        }
        else
            it_inst++;
    }
}
bool CRiskData::bSubScribeInst2(const string& szInst)
{
    map<string,vector<string>>::iterator it = m_mapExchange2Inst2.begin();
    for (;it!=m_mapExchange2Inst2.end();it++)
    {
        vector<string>::iterator it_inst = it->second.begin();
        for (;it_inst!=it->second.end();it_inst++)
        {
            if(0==strcmp((*it_inst).c_str(),szInst.c_str()))
                return true;
        }
    }

    return false;
}
void CRiskData::SubScribeInst2(const string& szExchange,const string& szInst)
{
    map<string,vector<string>>::iterator it = m_mapExchange2Inst2.find(szExchange);
    if(it==m_mapExchange2Inst2.end())
    {
        m_mapExchange2Inst2[szExchange].push_back(szInst);
        return;
    }

    vector<string>& vec = it->second;
    vec.push_back(szInst);
}
void CRiskData::UnSubScribeInst2(const string& szExchange,const string& szInst)
{
    map<string,vector<string>>::iterator it = m_mapExchange2Inst2.find(szExchange);
    if(it==m_mapExchange2Inst2.end())
        return;

    vector<string>& vec = it->second;
    vector<string>::iterator it_inst = vec.begin();
    while (it_inst != vec.end())
    {
        string szDest = *it_inst;
        if(0==szDest.compare(szInst))
        {
            it_inst = vec.erase(it_inst);
        }
        else
            it_inst++;
    }
}
void CRiskData::AnalyseSelectInst(vector<string> vecSrc, vector<string>& vecSub, vector<string>& vecUnSub)
{
    if(vecSrc.empty())
        return;

    vector<string> vecPreInst;
    vector<string>::iterator it_begin,it_end;    
    for (map<string,vector<string>>::iterator it = m_mapExchange2Inst.begin();it!=m_mapExchange2Inst.end();++it)
    {
        vector<string> tempVec = it->second;
        it_begin = tempVec.begin();
        it_end = tempVec.end();

        vecPreInst.insert(vecPreInst.end(),it_begin,it_end);
    }

    vector<string>::iterator it_src;
    vector<string>::iterator iter;
    for(it_src= vecSrc.begin();it_src!=vecSrc.end();++it_src)
    {
        iter = find(vecPreInst.begin(),vecPreInst.end(),*it_src);
        if(iter==vecPreInst.end())
        {
            //找出订阅的合约
            vecSub.push_back(*it_src);
        }
    }

    for(it_src = vecPreInst.begin();it_src!=vecPreInst.end();++it_src)
    {
        iter = find(vecSrc.begin(),vecSrc.end(),*it_src);
        if(iter==vecSrc.end())
        {
            //找出退订的合约
            vecUnSub.push_back(*it_src);
        }
    }
}
void CRiskData::SetInstrumentInfo(const string& szInst, const sInstrumentInfo& sInstInfo)
{
    m_mapInstrumentInfo[szInst] = sInstInfo;
}
void CRiskData::GetInstrumentInfo(const string& szInst, sInstrumentInfo& sInstInfo)
{
    map<string, sInstrumentInfo>::iterator it = m_mapInstrumentInfo.find(szInst);
    if(it!=m_mapInstrumentInfo.end())
        memcpy(&sInstInfo,&(it->second),sizeof(sInstrumentInfo));
}
map<string, sInstrumentInfo> CRiskData::GetInstrumentInfo()
{
    return m_mapInstrumentInfo;
}