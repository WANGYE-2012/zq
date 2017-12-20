// ConfigMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"

#include "tinyxml.h"
#include "GLobalFunc.h"
#include "GlobalConfigMgr.h"
#include <string>
#include "tools_tools.h"

bool GlobalConfigManager::m_bLoad=false;
bool GlobalConfigManager::m_bCalculate_OnRtnTrade=true;
bool GlobalConfigManager::m_bReq_OnRtnTrade=true;
int GlobalConfigManager::m_nDelayBefReq_OnRtnTrade=2;
int GlobalConfigManager::m_nMaxRecordReqPositionDetail_OnRtnTrade=500;
int GlobalConfigManager::m_nMaxItemCountForAutoResort_ListCtrl=200;
bool GlobalConfigManager::m_bCifcoServicesHall_ExtraFunction=false;
bool GlobalConfigManager::m_bDisorderWhenInsertOrder=false;


void GlobalConfigManager::Load()
{
    std::string strPath = "platform.xml";
	GlobalFunc::GetPath(strPath);


    TiXmlDocument doc(strPath.c_str()); 
	if( doc.LoadFile())
	{
		TiXmlElement* root = doc.RootElement();
		if( root == NULL ) 
			return;

        int iVal;

		iVal = GetnValueByTitle(root,1,"OnRtnTrade","IsCalculate");
        m_bCalculate_OnRtnTrade = iVal==0 ? false : true;

        iVal = GetnValueByTitle(root,1,"OnRtnTrade","IsReq");
        m_bReq_OnRtnTrade = iVal==0 ? false : true;

        m_nDelayBefReq_OnRtnTrade = GetnValueByTitle(root,2,"OnRtnTrade","DelayBefReq");

        m_nMaxRecordReqPositionDetail_OnRtnTrade = GetnValueByTitle(root,500,"OnRtnTrade","MaxRecordReqPositionDetail");

        m_nMaxItemCountForAutoResort_ListCtrl = GetnValueByTitle(root,200,"ListCtrl","MaxItemCountForAutoResort");

        iVal = GetnValueByTitle(root,1,"ExtraFunction","CifcoServicesHall");
        m_bCifcoServicesHall_ExtraFunction = iVal==0 ? false : true;

        iVal = GetnValueByTitle(root,0,"MultiAccount","DisorderWhenInsertOrder");
        m_bDisorderWhenInsertOrder = iVal==0 ? false : true;

        m_bLoad=true;

		return;
	}
}

std::string GlobalConfigManager::GetstrValueByTitle(void* root,const char* DefValue,
                                                    const char* pTitle1,
                                                    const char* pTitle2,
                                                    const char* pTitle3,
                                                    const char* pTitle4)
{
    std::string strRlt;
    const char *ptext;

    if(DefValue) strRlt.assign(DefValue);

    TiXmlElement *pElement=(TiXmlElement*)root;
    
    if(!pElement||!pTitle1)
    {
        if(pElement&&!pTitle1&&(ptext=pElement->GetText())!=NULL) strRlt.assign(ptext);
        return strRlt;
    }

    pElement=pElement->FirstChildElement(pTitle1);
    if(!pElement||!pTitle2)
    {
        if(pElement&&!pTitle2&&(ptext=pElement->GetText())!=NULL) strRlt.assign(ptext);
        return strRlt;
    }

    pElement=pElement->FirstChildElement(pTitle2);
    if(!pElement||!pTitle3)
    {
        if(pElement&&!pTitle3&&(ptext=pElement->GetText())!=NULL) strRlt.assign(ptext);
        return strRlt;
    }

    pElement=pElement->FirstChildElement(pTitle3);
    if(!pElement||!pTitle4)
    {
        if(pElement&&!pTitle4&&(ptext=pElement->GetText())!=NULL) strRlt.assign(ptext);
        return strRlt;
    }

    pElement=pElement->FirstChildElement(pTitle4);
    if(pElement&&(ptext=pElement->GetText())!=NULL) strRlt.assign(ptext);
    return strRlt;
}



int GlobalConfigManager::GetnValueByTitle(void* proot,const int DefValue,
                                            const char* pTitle1,
                                            const char* pTitle2,
                                            const char* pTitle3,
                                            const char* pTitle4)
{
    std::string strRlt=GetstrValueByTitle(proot,"",pTitle1,pTitle2,pTitle3,pTitle4);
    CTools::mytrim(strRlt);
    if(strRlt.empty()) return DefValue;
    else return atoi(strRlt.c_str());
}


bool GlobalConfigManager::IsCalculate_OnRtnTrade()
{
	return m_bCalculate_OnRtnTrade;
}


//��ȡ�ɽ��ر����Ƿ񴥷��Ĳ���
bool GlobalConfigManager::IsReq_OnRtnTrade(void)
{
    return m_bReq_OnRtnTrade;
}

//��ȡ�ɽ��ر��󴥷���ѯ���ӳ�����
int GlobalConfigManager::GetDelayBefReq_OnRtnTrade(void)
{
    return m_nDelayBefReq_OnRtnTrade;
}

//��ȡ�ɽ�������ѯ�ֲ���ϸ������¼������ֵ���������ֵ�Ͳ��Զ���ѯ�ֲ���ϸ
int GlobalConfigManager::GetMaxRecordReqPositionDetail_OnRtnTrade(void)
{
    return m_nMaxRecordReqPositionDetail_OnRtnTrade;
}

//�б���Զ�����ʱ������������������������ֹͣ�Զ���������Ӱ���ٶ�
int GlobalConfigManager::GetMaxItemCountForAutoResort_ListCtrl(void)
{
    return m_nMaxItemCountForAutoResort_ListCtrl;
}

    //�Ƿ�����CifcoServicesHall����
bool GlobalConfigManager::IsCifcoServicesHall_ExtraFunction()
{
    return m_bCifcoServicesHall_ExtraFunction;
}

//���˺��Ƿ������µ�
bool GlobalConfigManager::IsDisorderWhenInsertOrder_MultiAccount()
{
    return m_bDisorderWhenInsertOrder;
}
