/***************************************************************************
*   @file         InstrumentList.cpp
*   @brief        ��Լ�б���,���ʵ��
*
*   @author     ying.qiu
*   @version    0.0.1
*   @date       created on 2010/09/26
********************************************************************************/
#include "stdafx.h"

#include "cfgMgr/CfgMgr.h"
#include "cfgMgr/CfgGlobal.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../inc/Module-Misc2/tools_util.h"
#include "../ConfigApp/Const.h"
#include "../inc/Module-Misc2/GlobalFunc.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/Module-Misc2/globalDefines.h"
#include "../inc/Module-Misc/orderCommonFunc.h"
#include "InstrumentListPanel.h"
#include "gui-common/FrameBase.h"
#include "Module-Misc2/GlobalConfigMgr.h"

extern PlatformSvrMgr* g_pPlatformMgr;

using namespace std;

#define BUTTON_WIDTH    64
#define BUTTON_HEIGHT	25

InstrumentListPanel* g_pInstrumentList=NULL;
wxWindow* InstrumentListPanel::m_InstrumentParent = NULL;

BEGIN_EVENT_TABLE(InstrumentListPanel,wxPanel)
	EVT_SIZE(OnSize)
	EVT_CFG_CHANGE(OnCfgChanged)
	EVT_SET_FOCUS(OnSetFocus)
	EVT_KILL_FOCUS(OnKillFocus)
	EVT_LIST_EXT_ITEM_ACTIVATED(wxID_ANY, InstrumentListPanel::OnActivated)
	EVT_LIST_EXT_COL_END_DRAG(wxID_ANY, OnColumnResize)
	EVT_COMMAND(wxID_ANY, wxEVT_INSTRUMENT_LIST_MARGINRATE_UPDATE,  InstrumentListPanel::OnUpdateForInstrument)
	EVT_COMMAND(wxID_ANY, wxEVT_INSTRUMENT_LIST_COMMISSIONRATE_UPDATE,  InstrumentListPanel::OnUpdateForInstrument)
	EVT_LIST_EXT_CONTEXT_MENU(InstrumentListPanel::OnContext)
	EVT_BUTTON(wxID_ANY, OnGroupButtonClick)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_MODIFY,OnMAPlatformModify)
END_EVENT_TABLE()

InstrumentListPanel::InstrumentListPanel(wxWindow *parent,
								 wxWindowID id,
								 const wxPoint& pos,
								 const wxSize& size,
								 long style,
								 const wxString& name)
: wxPanel(parent, id, pos, size, style, name)
, m_bFocus(false)
{
	g_pInstrumentList = this;
	m_InstrumentParent = parent;
	m_pListCtrl = new wxExtListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxLCEXT_REPORT|wxLCEXT_VRULES|wxLCEXT_TOGGLE_COLOUR|wxLCEXT_MASK_SORT, wxDefaultValidator, name );

	m_vGroupButton.clear();
	m_strExchangeID = "";
    m_GroupBtnInd=-1;

	InitListCtrl();

    //��׼�棬ֱ����ʾ�����˺Ű棬��OnMAPlatformModify()����ʾ
    if(!IS_MULTIACCOUNT_VERSION)
        DoInitQryStep();
}

InstrumentListPanel::~InstrumentListPanel()
{
    IPlatformSingleSvr* pSvr=CURRENT_SVR();
    if(pSvr)
    {
	    pSvr->UnSubscribeBusinessData(BID_RspQryInstrumentMarginRate, GID_INSTRUMENT);
	    pSvr->UnSubscribeBusinessData(BID_RspQryInstrumentCommissionRate, GID_INSTRUMENT);
	    pSvr->UnSubscribeBusinessData(BID_RspQryOptionInstrTradeCost, GID_INSTRUMENT);
	    pSvr->UnSubscribeBusinessData(BID_RspQryOptionInstrCommRate, GID_INSTRUMENT);
    }
}

/*�õ��е�����*/
int InstrumentListPanel::GetColValue(int ColId, PlatformStru_InstrumentInfo* pField, 
								 PlatformStru_InstrumentCommissionRate* commField,
								 PlatformStru_InstrumentMarginRate* margField,
								 std::string &strValue)
{	
	char buf[256];
	memset(buf,0,sizeof(buf));

	strValue="";
    IPlatformSingleSvr* pSvr=CURRENT_SVR();

	switch(ColId)
	{
	case INSTRUMENT_FIELD_ID_PRODUCTID:
		if(pField)
			strValue = pField->ProductID;					                                        //Ʒ�ִ���
		break;
	case INSTRUMENT_FIELD_ID_INSTRUMENTID:
		if(pField)
			strValue = pField->InstrumentID;					                             //��Լ
		break; 
	case INSTRUMENT_FIELD_ID_INSTRUMENTNAME:
		if(pField)
			strValue = pField->InstrumentName;					//��Լ��					
		break;
	case INSTRUMENT_FIELD_ID_EXCHANGE:            
		if(pField&&pSvr)
			strValue =pSvr->ExchangeID2String( pField->ExchangeID);                  //������			
		break;
	case INSTRUMENT_FIELD_ID_MULTIPLY:
		if(pField)
			strValue = GlobalFunc::ConvertToString(pField->VolumeMultiple);					//��Լ����
		break;
	case INSTRUMENT_FIELD_ID_PRICETICK:
		if(pField)
			strValue = Double2String(pField->PriceTick);					//��С�۸�䶯��λ
		break;
	case INSTRUMENT_FIELD_ID_PRODUCTCLASS:
		if(pField)
			strValue = CFieldValueTool::ProductType2String(pField->ProductClass,SVR_LANGUAGE);		//��Ʒ����							
		break;
	case INSTRUMENT_FIELD_ID_EXPIREDATE:
		if(pField)
			strValue = GlobalFunc::ConvertToString(pField->ExpireDate);					        //�������
		break;
	case INSTRUMENT_FIELD_ID_LONGMARGINRATIO:
		if(!pSvr||!pSvr->MarginRatioInInstrumentInfo())
		{
			if(margField)
			{
				if(strlen(margField->InstrumentID)==0) break;
				if(!util::equals(margField->LongMarginRatioByMoney,0)&&!util::isInvalidValue(margField->LongMarginRatioByMoney))
					strValue = Double2String(margField->LongMarginRatioByMoney*100)+"%"; 
				else
					strValue="";
			}
		}
		else
		{
			if ( pField )
			{
				if(!util::equals(pField->LongMarginRatio,0)&&!util::isInvalidValue(pField->LongMarginRatio))
					strValue = Double2String(pField->LongMarginRatio*100)+"%"; 
				else
					strValue="";
			}	
		}
		break;
	case INSTRUMENT_FIELD_ID_SHORTMARGINRATIO:
		if(!pSvr||!pSvr->MarginRatioInInstrumentInfo())
		{
			if(margField)
			{
				if(strlen(margField->InstrumentID)==0) break;
				if(!util::equals(margField->ShortMarginRatioByMoney,0)&&!util::isInvalidValue(margField->ShortMarginRatioByMoney))
					strValue = Double2String(margField->ShortMarginRatioByMoney*100)+"%";  
				else
					strValue="";
			}
		}
		else
		{
			if ( pField )
			{
				if(!util::equals(pField->ShortMarginRatio,0)&&!util::isInvalidValue(pField->ShortMarginRatio))
					strValue = Double2String(pField->ShortMarginRatio*100)+"%"; 
				else
					strValue="";
			}
		}
		break;

	case INSTRUMENT_FIELD_ID_OPENRATIO:	
		if(commField)
		{
			if(strlen(commField->InstrumentID)==0) break;
			if(!util::equals(commField->OpenRatioByVolume,0)&&!util::isInvalidValue(commField->OpenRatioByVolume))
				strValue = Double2String(commField->OpenRatioByVolume);    //���������� 
			else
				strValue="";
		}
		break;
	case INSTRUMENT_FIELD_ID_CLOSERATIO:
		if(commField)
		{
			if(strlen(commField->InstrumentID)==0) break;
			if(!util::equals(commField->CloseRatioByVolume,0)&&!util::isInvalidValue(commField->CloseRatioByVolume))
				strValue = Double2String(commField->CloseRatioByVolume);					//ƽ��������  
			else
				strValue="";           
		}
		break;
	case INSTRUMENT_FIELD_ID_CLOSETODAYRATIO:		
		if(commField)
		{
			if(strlen(commField->InstrumentID)==0) break;	      
			if(!util::equals(commField->CloseTodayRatioByVolume,0)&&!util::isInvalidValue(commField->CloseTodayRatioByVolume))                                     						//ƽ��������
				strValue = Double2String(commField->CloseTodayRatioByVolume);   
			else
				strValue="";
		}
		break;
	case INSTRUMENT_FIELD_ID_OPENRATIOBYMONEY:
		if(commField)
		{
			if(strlen(commField->InstrumentID)==0) break;
			if(!util::equals(commField->OpenRatioByMoney,0)&&!util::isInvalidValue(commField->OpenRatioByMoney))
				strValue = Double2String(commField->OpenRatioByMoney*100)+"%";     //������������ 
			else
				strValue="";
		}
		break;
	case INSTRUMENT_FIELD_ID_CLOSERATIOBYMONEY:
		if(commField)
		{
			if(strlen(commField->InstrumentID)==0) break;
			if(!util::equals(commField->CloseRatioByMoney,0)&&!util::isInvalidValue(commField->CloseRatioByMoney))
				strValue = Double2String(commField->CloseRatioByMoney*100)+"%";    //ƽ���������� 
			else
				strValue="";
		}
		break;
	case INSTRUMENT_FIELD_ID_CLOSETODAYRATIOBYMONEY:
		if(commField)
		{
			if(strlen(commField->InstrumentID)==0) break;
			if(!util::equals(commField->CloseRatioByMoney,0)&&!util::isInvalidValue(commField->CloseRatioByMoney))
				strValue = Double2String(commField->CloseTodayRatioByMoney*100)+"%";    //ƽ���������� 
			else
				strValue="";
		}
		break;

	case INSTRUMENT_FIELD_ID_LONGMARGINRATIOBYVOLUME:
		if(margField)
		{
			if(strlen(margField->InstrumentID)==0) break;
			if(!util::equals(margField->LongMarginRatioByVolume,0)&&!util::isInvalidValue(margField->LongMarginRatioByVolume))
				strValue = GlobalFunc::GetAccountFormatString(margField->LongMarginRatioByVolume);    //��ͷ��֤��� 
			else
				strValue="";
		}
		break;
	case INSTRUMENT_FIELD_ID_SHORTMARGINRATIOBYVOLUME:
		if(margField)
		{
			if(strlen(margField->InstrumentID)==0) break;
			if(!util::equals(margField->ShortMarginRatioByVolume,0)&&!util::isInvalidValue(margField->ShortMarginRatioByVolume))
				strValue = GlobalFunc::GetAccountFormatString(margField->ShortMarginRatioByVolume);   //��ͷ��֤��� 
			else
				strValue="";
		}
		break;
	case INSTRUMENT_FIELD_ID_CURRENCY:
		if ( pField )
		{
			strValue = GlobalFunc::ConvertToString( pField->Currency );
		}
		break;
	case INSTRUMENT_FIELD_ID_TICKSPERPOINT:
		if(pField)
		{			
		    strValue = GlobalFunc::ConvertToString(pField->TicksPerPoint);   // 		
		}
		break;
	case INSTRUMENT_FIELD_ID_TICKSIZE:
		if(pField)
		{		
			strValue = GlobalFunc::ConvertToString(pField->TickSize);   // 		
		}
		break;
	case INSTRUMENT_FIELD_ID_OptionStrikeRatioByMoney:
		if(commField)       //��Ȩ��Ȩ��������
		{
			if(strlen(commField->InstrumentID)==0) break;
            if(!util::equals(commField->StrikeRatioByMoney,0)&&!util::isInvalidValue(commField->StrikeRatioByMoney))
				strValue = Double2String(commField->StrikeRatioByMoney);
            else strValue="";
		}
		break;
	case INSTRUMENT_FIELD_ID_OptionStrikeRatioByVolume:	
		if(commField)       //��Ȩ��Ȩ������
		{
			if(strlen(commField->InstrumentID)==0) break;
			if(!util::equals(commField->StrikeRatioByVolume,0)&&!util::isInvalidValue(commField->StrikeRatioByVolume))
				strValue = Double2String(commField->StrikeRatioByVolume);
            else strValue="";
		}
		break;
	case INSTRUMENT_FIELD_ID_OptionFixedMargin:	
		if(margField)       //��Ȩ��֤�𲻱䲿��
		{
			if(strlen(margField->InstrumentID)==0) break;
			if(!util::equals(margField->FixedMargin,0)&&!util::isInvalidValue(margField->FixedMargin))
				strValue = Double2String(margField->FixedMargin);
            else strValue="";
		}
		break;
	case INSTRUMENT_FIELD_ID_OptionMiniMargin:	
		if(margField)       //��Ȩ��С��֤��
		{
			if(strlen(margField->InstrumentID)==0) break;
			if((!util::equals(margField->MiniMargin,0)||!util::equals(margField->FixedMargin,0))&&!util::isInvalidValue(margField->MiniMargin))
				strValue = Double2String(margField->MiniMargin);
            else strValue="";
		}
		break;

	}
	return 0;

}

//�����ļ������仯
void InstrumentListPanel::OnCfgChanged(wxCommandEvent& evt)
{
	WXLOG_INFO("CInstrumentList::OnCfgChanged");

	wxFont newFont;
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	wxASSERT(pCfgMgr);
	LPLIST_TABLE_CFG pListCfg = pCfgMgr->GetListCfg(GID_INSTRUMENT);
	wxASSERT(pListCfg);
	bool isChanged=false;
	if(m_bakHeadBackgroundColor != wxColor(pListCfg->HeadBackgroundColor))
	{
		m_bakHeadBackgroundColor = wxColor(pListCfg->HeadBackgroundColor);
		m_pListCtrl->SetHeaderBackgroundColour(m_bakHeadBackgroundColor);
	}
	if(m_bakHeadColor != wxColor(pListCfg->HeadColor))
	{
		m_bakHeadColor = wxColor(pListCfg->HeadColor);
		m_pListCtrl->SetHeaderForegroundColour(m_bakHeadColor);
	}
	if(m_bakBackgroundColor != wxColor(pListCfg->BackgroundColor))
	{
		m_bakBackgroundColor = wxColor(pListCfg->BackgroundColor);
		m_pListCtrl->SetBackgroundColour(m_bakBackgroundColor);
	}
	if(m_bakTextColor != wxColor(pListCfg->TextColor))
	{
		m_bakTextColor = wxColor(pListCfg->TextColor);
		m_pListCtrl->SetForegroundColour(m_bakTextColor);
	}
	newFont.SetNativeFontInfoUserDesc(pListCfg->szFont); 
	if(newFont != m_BakFont)
	{
		m_BakFont=newFont;
		m_pListCtrl->SetFont(m_BakFont);
	}
	if(m_bakEvenLineBgColor != wxColor(pListCfg->EvenLineBackgroundColor))
	{
		m_bakEvenLineBgColor=wxColor(pListCfg->EvenLineBackgroundColor);
		m_pListCtrl->SetEvenRowColour(pListCfg->EvenLineBackgroundColor);
	}
	if(m_bakOddLineBgColor != wxColor(pListCfg->OddLineBackgroundColor))
	{
		m_bakOddLineBgColor= wxColor(pListCfg->OddLineBackgroundColor);
		m_pListCtrl->SetOddRowColour(pListCfg->OddLineBackgroundColor);
	}

	m_pListCtrl->ClearAll();
	m_InstrumentID2ColMap.clear();
	m_mapVisibleField.clear();

	InitListCtrl();
	ShowInstrument();

	TRANSFORM_COLUMN_TEXT(GID_INSTRUMENT,m_pListCtrl);
	m_pListCtrl->ReSortItems();
	Refresh();
}
void InstrumentListPanel::OnSetFocus(wxFocusEvent& event)
{
	WXLOG_INFO("CInstrumentList::OnSetFocus");
	m_bFocus=true;
}
void InstrumentListPanel::OnKillFocus(wxFocusEvent& event)
{
	WXLOG_INFO("CInstrumentList::OnKillFocus");
	m_bFocus=false;
}
void InstrumentListPanel::AddShowRateInstrumentID(const std::string& strID)
{
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
	wxASSERT(pCfgMgr);

	std::string strValue;
	std::map<std::string,  int>::iterator it=m_InstrumentID2RowMap.find(strID);
	if(it!=m_InstrumentID2RowMap.end())
	{
		//�������
		PlatformStru_InstrumentInfo tmpInstrument;
		memset(&tmpInstrument,0,sizeof(tmpInstrument));
		PlatformStru_InstrumentCommissionRate commField;
		memset(&commField,0,sizeof(commField));
		PlatformStru_InstrumentMarginRate     margField;
        IPlatformSingleSvr* pSvr=CURRENT_SVR();
        int ret_Instrument=pSvr?pSvr->GetInstrumentInfo(strID,tmpInstrument):-1;
        int ret_Comm=pSvr?pSvr->GetCommissionRate(strID, commField):-1;
        int ret_Marg=pSvr?pSvr->GetMarginRate(strID,margField):-1;
		//��������һ��
		int nColCount = pCfgMgr->GetListVisibleColCount(GID_INSTRUMENT);
		for(int i = 0; i < nColCount; i++)           //���ø��е���Ϣ
		{
			LPLIST_COLUMN_CFG pColCfg =pCfgMgr->GetListVisibleColCfg(GID_INSTRUMENT, i);
			GetColValue(pColCfg->id, 
				ret_Instrument==0?(&tmpInstrument):NULL,
                ret_Comm==0?(&commField):NULL,
                ret_Marg==0?(&margField):NULL,  
				strValue);                         
			m_pListCtrl->SetItem(it->second, i, strValue);
		}            
	}
}
void InstrumentListPanel::OnActivated(wxExtListEvent& event)
{
	std::map<std::string,  int>::iterator it=m_InstrumentID2RowMap.begin();
	std::string InstrumentID;
	//��õ����ĺ�Լ
	for(it;it!=m_InstrumentID2RowMap.end();++it)
	{
		if(it->second==event.GetIndex())
		{
			InstrumentID=it->first;
			break;
		}
	}

	if(!InstrumentID.empty())
	{
        IPlatformSingleSvr* pSvr=CURRENT_SVR();
		if(pSvr) pSvr->GetCommissionRate_TriggerReq(InstrumentID);
		if(pSvr) pSvr->GetMarginRate_TriggerReq(InstrumentID);
	}
}

//��ѯ�ڻ���֤������Ӧ�Ļص����������±�֤���ʵ���ʾ
int InstrumentListPanel::CallBackFunc_BID_RspQryInstrumentMarginRate(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if( data.BID!=BID_RspQryInstrumentMarginRate || data.Size!=sizeof(DataRspQryInstrumentMarginRate) )
		return 0;
    if(!g_pInstrumentList) 
        return 0;

	wxCommandEvent evt(wxEVT_INSTRUMENT_LIST_MARGINRATE_UPDATE, wxID_ANY);
    DWORD EvtParamID;
    string EvtParamStr(((DataRspQryInstrumentMarginRate&)data).InstrumentMarginRateField.InstrumentID);
    if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
        evt.SetInt((int)EvtParamID);

	if( g_pInstrumentList )
		g_pInstrumentList->AddPendingEvent(evt);
	return 0;
}

//��ѯ�ڻ�����������Ӧ�Ļص������������������ʵ���ʾ
int InstrumentListPanel::CallBackFunc_BID_RspQryInstrumentCommissionRate(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!=BID_RspQryInstrumentCommissionRate || data.Size!=sizeof(DataRspQryInstrumentCommissionRate))
		return 0;
    if(!g_pInstrumentList) 
        return 0;

	wxCommandEvent evt(wxEVT_INSTRUMENT_LIST_COMMISSIONRATE_UPDATE, wxID_ANY);
	PlatformStru_InstrumentCommissionRate& icr=((DataRspQryInstrumentCommissionRate&)data).InstrumentCommissionRateField;
	//�п����Ǻ�ԼƷ��
	set<string> InstrumentList;
    DWORD EvtParamID;
	if(CURRENT_SVR()&&CURRENT_SVR()->GetInstrumentListByProductID(icr.InstrumentID,InstrumentList)>0)
	{
        //��Ʒ��
		for(set<string>::iterator it2=InstrumentList.begin(); it2!=InstrumentList.end(); ++it2)
		{
            string EvtParamStr(*it2);
            if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
                evt.SetInt((int)EvtParamID);

			if( g_pInstrumentList )
				g_pInstrumentList->AddPendingEvent(evt);
		}
	}
	else
	{
        string EvtParamStr(icr.InstrumentID);
        if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
            evt.SetInt((int)EvtParamID);
        
		if( g_pInstrumentList )
			g_pInstrumentList->AddPendingEvent(evt);
	}
	return 0;
}

//��ѯ��Ȩ���׳ɱ���Ӧ�Ļص����������±�֤���ʵ���ʾ
int InstrumentListPanel::CallBackFunc_BID_RspQryOptionInstrTradeCost(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if( data.BID!=BID_RspQryOptionInstrTradeCost || data.Size!=sizeof(DataRspQryOptionInstrTradeCost) )
		return 0;
    if(!g_pInstrumentList) 
        return 0;

	wxCommandEvent evt(wxEVT_INSTRUMENT_LIST_MARGINRATE_UPDATE, wxID_ANY);
    DWORD EvtParamID;
    string EvtParamStr(((DataRspQryOptionInstrTradeCost&)data).InstrumentMarginRateField.InstrumentID);
    if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
        evt.SetInt((int)EvtParamID);

	if( g_pInstrumentList )
		g_pInstrumentList->AddPendingEvent(evt);
	return 0;
}

//��ѯ��Ȩ����������Ӧ�Ļص������������������ʵ���ʾ
int InstrumentListPanel::CallBackFunc_BID_RspQryOptionInstrCommRate(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!=BID_RspQryOptionInstrCommRate || data.Size!=sizeof(DataRspQryOptionInstrCommRate))
		return 0;
    if(!g_pInstrumentList) 
        return 0;

	wxCommandEvent evt(wxEVT_INSTRUMENT_LIST_COMMISSIONRATE_UPDATE, wxID_ANY);
	PlatformStru_InstrumentCommissionRate& icr=((DataRspQryInstrumentCommissionRate&)data).InstrumentCommissionRateField;
	//�п����Ǻ�ԼƷ��
	set<string> InstrumentList;
    DWORD EvtParamID;
	if(CURRENT_SVR()&&CURRENT_SVR()->GetInstrumentListByProductID(icr.InstrumentID,InstrumentList)>0)
	{
        //��Ʒ��
		for(set<string>::iterator it2=InstrumentList.begin(); it2!=InstrumentList.end(); ++it2)
		{
            string EvtParamStr(*it2);
            if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
                evt.SetInt((int)EvtParamID);

			if( g_pInstrumentList )
				g_pInstrumentList->AddPendingEvent(evt);
		}
	}
	else
	{
        string EvtParamStr(icr.InstrumentID);
        if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
            evt.SetInt((int)EvtParamID);
        
		if( g_pInstrumentList )
			g_pInstrumentList->AddPendingEvent(evt);
	}
	return 0;
}

//���µ������Ѻͱ�֤�������ˣ�������ʾ
void InstrumentListPanel::OnUpdateForInstrument(wxCommandEvent& evt)
{
    string EvtParamStr;
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CFTEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        CFTEventParam::DeleteEventParam(EvtParamID);
    else return;
    wxString strInstrumentID(EvtParamStr.c_str());
    bool brlt=false;


    CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
    wxASSERT(pCfgMgr);
    map<string, int>::iterator it_inst = m_InstrumentID2RowMap.find(strInstrumentID.c_str());
    if(it_inst == m_InstrumentID2RowMap.end())
	    return;

    PlatformStru_InstrumentCommissionRate commField;
    PlatformStru_InstrumentMarginRate margField;
    IPlatformSingleSvr* pSvr=CURRENT_SVR();
    int ret_Comm=pSvr?pSvr->GetCommissionRate(strInstrumentID.c_str(), commField):-1;
    int ret_Marg=pSvr?pSvr->GetMarginRate(strInstrumentID.c_str(),margField):-1;
    int nColCount = pCfgMgr->GetListVisibleColCount(GID_INSTRUMENT);
    for(int i = 0; i < nColCount; i++)           //���ø��е���Ϣ
    {
	    LPLIST_COLUMN_CFG pColCfg =pCfgMgr->GetListVisibleColCfg(GID_INSTRUMENT, i);
	    if( pColCfg->id ==INSTRUMENT_FIELD_ID_LONGMARGINRATIO||
		    pColCfg->id ==INSTRUMENT_FIELD_ID_SHORTMARGINRATIO||
		    pColCfg->id ==INSTRUMENT_FIELD_ID_LONGMARGINRATIOBYVOLUME||
		    pColCfg->id ==INSTRUMENT_FIELD_ID_SHORTMARGINRATIOBYVOLUME||
		    pColCfg->id ==INSTRUMENT_FIELD_ID_OPENRATIO||
		    pColCfg->id ==INSTRUMENT_FIELD_ID_CLOSERATIO||
		    pColCfg->id ==INSTRUMENT_FIELD_ID_CLOSETODAYRATIO||
		    pColCfg->id ==INSTRUMENT_FIELD_ID_OPENRATIOBYMONEY||
		    pColCfg->id ==INSTRUMENT_FIELD_ID_CLOSERATIOBYMONEY||
		    pColCfg->id ==INSTRUMENT_FIELD_ID_CLOSETODAYRATIOBYMONEY||
		    pColCfg->id ==INSTRUMENT_FIELD_ID_OptionFixedMargin||
		    pColCfg->id ==INSTRUMENT_FIELD_ID_OptionMiniMargin||
		    pColCfg->id ==INSTRUMENT_FIELD_ID_OptionStrikeRatioByMoney||
		    pColCfg->id ==INSTRUMENT_FIELD_ID_OptionMiniMargin)
	    {
		    std::string strValue;
		    GetColValue(pColCfg->id, 
			    NULL,
			    ret_Comm==0?(&commField):NULL,
			    ret_Marg==0?(&margField):NULL,  
			    strValue);

		    m_pListCtrl->SetItem(it_inst->second, i, strValue);
	    }
    }  

	m_pListCtrl->ReSortItems();
}

std::string InstrumentListPanel::GetItemText(int row, int col)
{
	if(row ==-1)
		return "";
	if(col ==-1)
		return "";

	wxExtListItem item;
	item.m_itemId = row;
	item.m_col = col;
	if(m_pListCtrl->GetItem(item))
	{
		return std::string(item.m_text);	
	}
	return "";
}
//�Ҽ��˵�
void InstrumentListPanel::OnContext(wxContextMenuEvent& evt)
{
	wxMenu menu;
	menu.Append(ID_MENU_AUTO_ADJUST_COL_WIDTH, LOADSTRING(CONTEXTMENU_AUTO_ADJUST_COL_WIDTH));
	menu.Append(ID_MENU_EXPORT,LOADSTRING(CONTEXTMENU_EXPORT));
	menu.Append(ID_MENU_COLUMNCONFIG,LOADSTRING(CONTEXTMENU_COLUMNCONFIG));

	BOOL bIsRowCLick = evt.GetId();//bIsRowCLick=TRUE��ʾ��������У�=FALSE��ʾ���������
	if ( bIsRowCLick )
	{
		menu.AppendSeparator();
        INSERT_ADDTOMYSELF_ITEM(menu);
	}

	POINT pt;
	::GetCursorPos(&pt);
	BOOL retcmd=zqPopupMenu(&menu,pt.x,pt.y,this);

	//������ѡ
    CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	if( retcmd - ID_MENU_ADDTOMYSELF >= 0 )
	{
		bool bAdded = false;
		for ( int i = 0; i < m_pListCtrl->GetItemCount(); i++)
		{
			if ( m_pListCtrl->IsSelected( i ))
			{
				string InstrumentId = (m_pListCtrl->GetItemText( i )).c_str();
                IPlatformSingleSvr* pSvr=CURRENT_SVR();
				if(!pSvr||pCfgMgr->GetInstrumentGroupMemberCount(retcmd - ID_MENU_ADDTOMYSELF)>=pSvr->GetGroupMaxContractNum())
				{
					wxMessageBox(LOADSTRING(OVER_LIMIT_OF_GROUP),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
					break;
				}
				else
				{
					pCfgMgr->AddInstrument( retcmd - ID_MENU_ADDTOMYSELF, InstrumentId.c_str()); 
					bAdded = true;					
				}
			}
		}
   
		if ( bAdded ){SEND_CONFIG_CHANGED_EVENT(MSG_INSTRUMENT_GROUP_CHANGE);}
		return;
	}

	switch(retcmd)
	{
	    case ID_MENU_AUTO_ADJUST_COL_WIDTH:
		    m_pListCtrl->AutoAdjustColumnWidth();
		    SaveColWidth();
		    break;
	    case ID_MENU_EXPORT://�����б�
		{
	        CfgMgr * pMgr = CFG_MGR_DEFAULT();  
            wxString cap = pMgr!=NULL ? (wxString)pMgr->GetPanelCaption(GetId()) : wxString("InstrumentList");
            LIST_EXPORT_CSV_FILE(cap, m_pListCtrl);
		    break;
		}
	    case ID_MENU_COLUMNCONFIG://���������
            SEND_LOAD_CONFIG_APP_EVENT(ID_ITEM_INSTRUMENTLIST);
		    break;
	    default:
		    break;
	}

	evt.Skip();
}
//��Լ�б��ѯ��ɺ���ʾ��Լ�б�
void InstrumentListPanel::DoInitQryStep()
{
    vector<string> exchangeIDset;
    exchangeIDset.clear();
    IPlatformSingleSvr* pSvr=CURRENT_SVR();
    if(!pSvr) return;
    pSvr->GetExchangeIDs( exchangeIDset );
    if ( exchangeIDset.empty())
    {
        return;
    }

    wxRect rect = GetRect();
    int nBeginPosX = rect.GetLeft() + 5;
    int nBeginPosY = rect.GetHeight() - 30;
    int nDeltaX = 2;
    vector<string>::iterator it = exchangeIDset.begin();
    bool bHaveCZCE=false,bHaveDCE=false;
    int i = 1;
    for ( ; it != exchangeIDset.end(); it++ )
    {
        if(*it==wxString("CZCE")) bHaveCZCE=true;
        if(*it==wxString("DCE")) bHaveDCE=true;
        wxString strText = pSvr->ExchangeID2String( *it ).c_str();
        wxButton* pBtn = new wxButton(this, i, strText, wxPoint(nBeginPosX, nBeginPosY), wxSize(BUTTON_WIDTH, BUTTON_HEIGHT));
        if ( 1 == i )
        {
            m_strExchangeID = *it;
            m_GroupBtnInd=0;
            pBtn->SetBackgroundColour(wxColor(214,48,18));
        }
        else
        {
            pBtn->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
        }

        GROUPINFO groupInfo( pBtn, (*it).c_str(), false);
        m_vGroupButton.push_back( groupInfo );
        nBeginPosX = nBeginPosX + BUTTON_WIDTH + nDeltaX;
        i++;
    }

    if(bHaveCZCE)
    {
        wxString strText = wxString("֣�����");
        wxButton* pBtn = new wxButton(this, i, strText, wxPoint(nBeginPosX, nBeginPosY), wxSize(BUTTON_WIDTH, BUTTON_HEIGHT));
        pBtn->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
        m_vGroupButton.push_back( GROUPINFO( pBtn, wxString("CZCE"), true) );
        nBeginPosX = nBeginPosX + BUTTON_WIDTH + nDeltaX;
        i++;
    }
    if(bHaveDCE)
    {
        wxString strText = wxString("�������");
        wxButton* pBtn = new wxButton(this, i, strText, wxPoint(nBeginPosX, nBeginPosY), wxSize(BUTTON_WIDTH, BUTTON_HEIGHT));
        pBtn->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
        m_vGroupButton.push_back( GROUPINFO( pBtn, wxString("DCE"), true) );
        nBeginPosX = nBeginPosX + BUTTON_WIDTH + nDeltaX;
        i++;
    }

    ShowInstrument();

    pSvr->SubscribeBusinessData(BID_RspQryInstrumentMarginRate, GID_INSTRUMENT, CallBackFunc_BID_RspQryInstrumentMarginRate);
    pSvr->SubscribeBusinessData(BID_RspQryInstrumentCommissionRate, GID_INSTRUMENT, CallBackFunc_BID_RspQryInstrumentCommissionRate);
    pSvr->SubscribeBusinessData(BID_RspQryOptionInstrTradeCost, GID_INSTRUMENT, CallBackFunc_BID_RspQryOptionInstrTradeCost);
    pSvr->SubscribeBusinessData(BID_RspQryOptionInstrCommRate, GID_INSTRUMENT, CallBackFunc_BID_RspQryOptionInstrCommRate);
}
//��ʼ���б�������Ϣ
void InstrumentListPanel::InitListCtrl()
{
    SetBackgroundColour(DEFAULT_COLOUR); 
	/*��ȡ������Ϣ*/
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
	wxASSERT(pCfgMgr);
	LPLIST_TABLE_CFG pListCfg = pCfgMgr->GetListCfg(GID_INSTRUMENT);
	wxASSERT(pListCfg);

	/*�����б���ɫ*/
	m_bakHeadBackgroundColor = wxColor(pListCfg->HeadBackgroundColor);
	m_bakHeadColor = wxColor(pListCfg->HeadColor);
	m_bakBackgroundColor = wxColor(pListCfg->BackgroundColor);
	m_bakTextColor = wxColor(pListCfg->TextColor);
	m_BakFont.SetNativeFontInfoUserDesc(pListCfg->szFont); 
	m_bakEvenLineBgColor = wxColor(pListCfg->EvenLineBackgroundColor);
	m_bakOddLineBgColor = wxColor(pListCfg->OddLineBackgroundColor);

	m_pListCtrl->SetHeaderBackgroundColour(m_bakHeadBackgroundColor);
	m_pListCtrl->SetHeaderForegroundColour(m_bakHeadColor);
	m_pListCtrl->SetBackgroundColour(m_bakBackgroundColor);
	m_pListCtrl->SetForegroundColour(m_bakTextColor);
	m_pListCtrl->SetFont(m_BakFont);
	m_pListCtrl->SetOddRowColour(pListCfg->OddLineBackgroundColor);
	m_pListCtrl->SetEvenRowColour(pListCfg->EvenLineBackgroundColor);

	wxExtListItem col; 
	int nColCount = pCfgMgr->GetListColCount(GID_INSTRUMENT);
	for(int i = 0,index=0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListColCfg(GID_INSTRUMENT, i);
		//"Ʒ�ִ��룬��Լ����Լ��"���������
		if(pColCfg->id == INSTRUMENT_FIELD_ID_PRODUCTID || pColCfg->id == INSTRUMENT_FIELD_ID_INSTRUMENTID ||pColCfg->id == INSTRUMENT_FIELD_ID_INSTRUMENTNAME )
			col.SetAlign(wxLISTEXT_FORMAT_LEFT);
		else//����ģ��Ҷ���
			col.SetAlign(wxLISTEXT_FORMAT_RIGHT);
		if(pColCfg->Visibility != 0)
		{
			col.SetText(pCfgMgr->GetColCfgCaption(GID_INSTRUMENT,pColCfg->id));
			col.SetWidth(pColCfg->Width);
			col.SetColData(pColCfg->id);
			m_pListCtrl->InsertColumn(index,col); 
			m_mapVisibleField[pColCfg->id] = *pColCfg;
			m_InstrumentID2ColMap[pColCfg->id]=index++;
		}
	}
}
//���ݴ����С�����ؼ��Ĵ�С��λ��
void InstrumentListPanel::OnSize( wxSizeEvent& event )
{
	wxRect rect = GetRect();
	int nBeginPosX = rect.GetLeft() + 5;
	int nBeginPosY = rect.GetHeight() - 30;
	int nDeltaX = 10;
	for ( size_t i = 0; i < m_vGroupButton.size(); i++)
	{
		wxButton* pBtn = m_vGroupButton[i].pBtn;
		if(pBtn)
		{
		    pBtn->Move( nBeginPosX, nBeginPosY);
		    nBeginPosX = nBeginPosX + BUTTON_WIDTH + nDeltaX;
		}
	}
    if(m_pListCtrl)
		m_pListCtrl->SetSize( rect.width, rect.height - 35);
}
//���б��в���һ����������ȫ����Լ�б�
void InstrumentListPanel::ShowInstrument()
{
    if(m_pListCtrl->GetColumnCount()==0)
        return;
    if(m_GroupBtnInd<0||m_GroupBtnInd>=(int)m_vGroupButton.size())
        return;
    IPlatformSingleSvr* pSvr=CURRENT_SVR();
    if(!pSvr) return;

	int InstrumentCount = 0;
	set<string> setInstrumentIDArray;
	setInstrumentIDArray.clear();
    InstrumentCount = pSvr->GetInstrumentList(setInstrumentIDArray, m_strExchangeID);
	std::set<std::string>::iterator it=setInstrumentIDArray.begin();
	for(; it!=setInstrumentIDArray.end(); ++it)
	{ 
        if(m_strExchangeID==string("CZCE")||m_strExchangeID==string("DCE"))
        {
            if(!DEFAULT_SVR()) continue;
            char ProductClass=pSvr->GetProductClassType(*it);
            if(!m_vGroupButton[m_GroupBtnInd].bComb&&ProductClass==THOST_FTDC_PC_Combination||
               m_vGroupButton[m_GroupBtnInd].bComb&&ProductClass!=THOST_FTDC_PC_Combination)
               continue;
        }

		wxExtListItem item;
		item.SetAlign(wxLISTEXT_FORMAT_LEFT);
		item.m_itemId = m_pListCtrl->GetItemCount();
		int nRow = m_pListCtrl->InsertItem(item);   //����һ���¼�¼
		m_InstrumentID2RowMap[*it]=nRow;

		AddShowRateInstrumentID(*it);
	}
}

//�����ť�л���������ʾ
void InstrumentListPanel::OnGroupButtonClick( wxCommandEvent& event )
{
	int nIndex = event.GetId();
	if ( nIndex == 0 || nIndex > (int)m_vGroupButton.size())
	{
		return;
	}
    if(nIndex-1==m_GroupBtnInd) return;
    m_GroupBtnInd=nIndex-1;
	m_strExchangeID = (m_vGroupButton[m_GroupBtnInd].strGroup).c_str();
	m_pListCtrl->DeleteAllItems();
	m_InstrumentID2RowMap.clear();
	ShowInstrument();
	m_pListCtrl->ReSortItems();
    //���޴�ֱ�������л����д�ֱ�������Ĵ��ڣ����ˮƽ�����������ұߣ����ڱ�ͷδˢ�µ�����
    m_pListCtrl->RefreshHeader();

	for ( size_t i = 0; i < m_vGroupButton.size(); i++)
	{
		if ( i == m_GroupBtnInd )
		{
			(m_vGroupButton[i].pBtn)->SetBackgroundColour(wxColor(214,48,18));//��ɫ		
		}
		else
		{
			(m_vGroupButton[i].pBtn)->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
		}
	}
}
//���Ըı���Ϣ
void InstrumentListPanel::OnLanguageChanged( wxCommandEvent& event )
{
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
	wxASSERT(pCfgMgr);

	TRANSFORM_COLUMN_TEXT(GID_INSTRUMENT,m_pListCtrl);

    //��������Ʒ��������
    for(map<std::string, int>::const_iterator it=m_InstrumentID2RowMap.begin();it!=m_InstrumentID2RowMap.end();it++)
    {
        if(!DEFAULT_SVR()) continue;
        char ProductClass=DEFAULT_SVR()->GetProductClassType(it->first);

		for(int col = 0; col < pCfgMgr->GetListVisibleColCount(GID_INSTRUMENT); col++)
		{
			LPLIST_COLUMN_CFG pColCfg =pCfgMgr->GetListVisibleColCfg(GID_INSTRUMENT, col);
			if ( pColCfg->id == INSTRUMENT_FIELD_ID_PRODUCTCLASS &&DEFAULT_SVR())
			{
				string strValue =CFieldValueTool::ProductType2String(ProductClass,SVR_LANGUAGE);
				m_pListCtrl->SetItem( it->second, col, strValue );
			}
		}
    }
}
//���wxExtListCtrl�̳���wxGrid�����б���ı���ʱ�����п�
void InstrumentListPanel::OnColumnResize( wxExtListEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nCol = event.GetColumn();
	if ( nCol < 0 || nCol > pCfgMgr->GetListVisibleColCount( GID_INSTRUMENT ))
	{
		return;
	}

	LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_INSTRUMENT, nCol);
	pColCfg->Width = m_pListCtrl->GetColumnWidth( nCol );
}

void InstrumentListPanel::SaveColWidth()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr || NULL == m_pListCtrl )
	{
		return;
	}

	int nColCount = m_pListCtrl->GetColumnCount();
	if ( nColCount != pCfgMgr->GetListVisibleColCount( GID_INSTRUMENT ))
	{
		return;
	}

	for ( int i = 0; i < nColCount; i++)
	{
		LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListVisibleColCfg(GID_INSTRUMENT, i);
		pColCfg->Width = m_pListCtrl->GetColumnWidth( i );
	}
}
//���˺�ʱ���л����˺�
void InstrumentListPanel::OnMAPlatformModify(wxCommandEvent& event)
{
	m_pListCtrl->DeleteAllItems();
	m_InstrumentID2RowMap.clear();
	for (size_t i=0;i<m_vGroupButton.size();++i)
	{
		SAFE_DELETE(m_vGroupButton[i].pBtn);
	}
	m_vGroupButton.clear();
	m_strExchangeID = "";
	DoInitQryStep();
}
