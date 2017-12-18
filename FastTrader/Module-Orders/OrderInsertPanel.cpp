#include "stdafx.h"
#include "OrderInsertPanel.h"
#include "../inc/Module-Misc2/tools_util.h"
#include "COrderBatchDailog.h"
#include "CEditOrderChangeDailog.h"
#include "CEditOrderReplaceDailog.h"
#include "COrderConfirmDlg.h"
#include "COrderParkedSelectDlg.h"
#include "../inc/Module-Misc2/EventParam.h"
#include "../Plusin/Plusin.h"
#include "Module-Misc2/GlobalConfigMgr.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

OrderInsertPanel* s_pOrderInsertPanel=NULL;

int OrderInsertPanel::RspOrderInsertCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(!s_pOrderInsertPanel)
		return 0;

	wxCommandEvent evt(wxEVT_ORDER_INSERT_PANEL_NEW_ORDER, wxID_ANY);
	if(data.BID==BID_RspOrderInsert && data.Size==sizeof(DataRspOrderInsert))
	{
		DataRspOrderInsert* pTmp=new DataRspOrderInsert();
		memcpy(pTmp, &data, sizeof(DataRspOrderInsert));
		evt.SetClientData(pTmp);
	}
	else if(data.BID==BID_ErrRtnOrderInsert && data.Size==sizeof(DataErrRtnOrderInsert))
	{
		DataErrRtnOrderInsert* pTmp=new DataErrRtnOrderInsert();
		memcpy(pTmp, &data, sizeof(DataErrRtnOrderInsert));
		evt.SetClientData(pTmp);
	}
	else if(data.BID==BID_RtnOrder && data.Size==sizeof(DataRtnOrder))
	{
		DataRtnOrder* pTmp=new DataRtnOrder();
		memcpy(pTmp, &data, sizeof(DataRtnOrder));
		evt.SetClientData(pTmp);
	}
	else if(data.BID==BID_RspOrderAction && data.Size==sizeof(DataRspOrderAction))
	{
		DataRspOrderAction* pTmp=new DataRspOrderAction();
		memcpy(pTmp, &data, sizeof(DataRspOrderAction));
		evt.SetClientData(pTmp);
	}
	else if(data.BID==BID_ErrRtnOrderAction && data.Size==sizeof(DataErrRtnOrderAction))
	{
		DataErrRtnOrderAction* pTmp=new DataErrRtnOrderAction();
		memcpy(pTmp, &data, sizeof(DataErrRtnOrderAction));
		evt.SetClientData(pTmp);
	}
	else
	{
		return 0;
	}

	//post event to OrderInsertPanel
	s_pOrderInsertPanel->AddPendingEvent(evt);

	return 0;
}

int OrderInsertPanel::TradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)	  //�ɽ��ر�
{
    if(data.BID!=BID_RtnTrade || data.Size!=sizeof(DataRtnTrade))
        return 0;

    if(!s_pOrderInsertPanel)
        return 0;

    DataRtnTrade* pTmp=new DataRtnTrade();
    memcpy(pTmp, &data, sizeof(DataRtnTrade));

    //post event to CAllOrdersPanel
    wxCommandEvent evt(wxEVT_ORDER_INSERT_PANEL_NEW_ORDER, wxID_ANY);
    evt.SetClientData(pTmp);

    s_pOrderInsertPanel->AddPendingEvent(evt);
    return 0;
}

int OrderInsertPanel::SubscribeForQuoteCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
    if(data.BID!=BID_RspSubForQuoteRsp || data.Size!=sizeof(DataRspSubForQuoteRsp))
        return 0;

    if(!s_pOrderInsertPanel)
        return 0;

    DataRspSubForQuoteRsp* pTmp=new DataRspSubForQuoteRsp();
    memcpy(pTmp, &data, sizeof(DataRspSubForQuoteRsp));

    //post event to CAllOrdersPanel
    wxCommandEvent evt(wxEVT_ORDER_INSERT_PANEL_SUBSFORQUOTE, wxID_ANY);
    evt.SetClientData(pTmp);

    s_pOrderInsertPanel->AddPendingEvent(evt);
    return 0;
}

int OrderInsertPanel::UnSubscribeForQuoteCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
    if(data.BID!=BID_RspUnSubForQuoteRsp || data.Size!=sizeof(DataRspUnSubForQuoteRsp))
        return 0;

    if(!s_pOrderInsertPanel)
        return 0;

    DataRspUnSubForQuoteRsp* pTmp=new DataRspUnSubForQuoteRsp();
    memcpy(pTmp, &data, sizeof(DataRspUnSubForQuoteRsp));

    //post event to CAllOrdersPanel
    wxCommandEvent evt(wxEVT_ORDER_INSERT_PANEL_UNSUBSFORQUOTE, wxID_ANY);
    evt.SetClientData(pTmp);

    s_pOrderInsertPanel->AddPendingEvent(evt);
    return 0;
}

int OrderInsertPanel::ForQuoteRspCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
    if(data.BID!=BID_RtnForQuoteRsp || data.Size!=sizeof(DataRtnForQuoteRsp))
        return 0;

    if(!s_pOrderInsertPanel)
        return 0;

    DataRtnForQuoteRsp* pTmp=new DataRtnForQuoteRsp();
    memcpy(pTmp, &data, sizeof(DataRtnForQuoteRsp));

    //post event to CAllOrdersPanel
    wxCommandEvent evt(wxEVT_ORDER_INSERT_PANEL_FORQUOTERSP, wxID_ANY);
    evt.SetClientData(pTmp);

    s_pOrderInsertPanel->AddPendingEvent(evt);
    return 0;
}

BEGIN_EVENT_TABLE(OrderInsertPanel,COrderInsertOperations)
    EVT_BUTTON(ID_BUTTON_OK,OnOk)
    EVT_BUTTON(ID_BUTTON_CANCEL,OnCancel)
    EVT_BUTTON(ID_BUTTON_PARKEDORDER,OnBtnParkedOrder)
    EVT_BUTTON(ID_BUTTON_MARKETPRICE,OnBtnMarketPrice)
    EVT_BUTTON(ID_BUTTON_BATCHORDER,OnBtnBatchOrder)
    EVT_BUTTON(ID_BUTTON_CONDITIONORDER,OnBtnConditionOrder)
    EVT_BUTTON(ID_BUTTON_POSITIONORDER,OnBtnPositionOrder)
    EVT_BUTTON(ID_BUTTON_STOPORDER,OnBtnStopOrder)
    EVT_BUTTON(ID_BUTTON_PATSORDER,OnBtnPatsOrder)
    EVT_BUTTON(ID_BUTTON_FAKFOKORDER,OnBtnFAKFOKOrder)
    EVT_CHECKBOX(ID_CHECK_WDSHOW,OnCheckWDShowClicked)
    EVT_CHECKBOX(ID_CHECK_WINDOWTOPALWAYS,OnCheckWinTopAlwaysClicked)
	EVT_CHECKBOX(ID_DOSTOPPOSITION_CHECK,OnCheckDoStopClicked)
    EVT_TIMER(ID_QUOTTIMER, OnTimer)
	EVT_PANEL_CHAR_HOOK(OnPanelCharHook)
	EVT_PANEL_FOCUS(OnPanelFocus)
	EVT_PAINT(OnPaint)
	EVT_COMMAND(wxID_ANY, wxEVT_ORDER_INSERT_PANEL_NEW_ORDER, OrderInsertPanel::OnRcvOrder)
	EVT_COMMAND(wxID_ANY, wxEVT_ORDER_INSERT_PANEL_SUBSFORQUOTE, OrderInsertPanel::OnRcvSubsForQuote)
	EVT_COMMAND(wxID_ANY, wxEVT_ORDER_INSERT_PANEL_UNSUBSFORQUOTE, OrderInsertPanel::OnRcvUnSubsForQuote)
	EVT_COMMAND(wxID_ANY, wxEVT_ORDER_INSERT_PANEL_FORQUOTERSP, OrderInsertPanel::OnRcvForQuoteRsp)
	EVT_COMMAND(wxID_ANY, wxEVT_FIVEPRICE_PRICESELECTED, OnFivePriceSelected) 
	EVT_COMMAND(wxID_ANY, wxEVT_KQPANEL_INSTRUMENTID_CHANGED, OnInstrumentIdTextChanged) 
	EVT_COMMAND(wxID_ANY, wxEVT_KQPANEL_AUTOOPENCLOSE_CHANGED, OnAutoOpenCloseChanged) 
	EVT_COMMAND(wxID_ANY, wxEVT_KQPANEL_AUTOTRACKPRICE_CHANGED, OnAutoTrackPriceChanged) 
	EVT_COMMAND(wxID_ANY, wxEVT_KQPANEL_BUYSELLSEL_CHANGED, OnBuySellSelChanged) 
	EVT_COMMAND(wxID_ANY, wxEVT_JSDPANEL_INSTRUMENTID_CHANGED, OnInstrumentIdTextChanged) 
	EVT_COMMAND(wxID_ANY, wxEVT_JSDPANEL_AUTOOPENCLOSE_CHANGED, OnAutoOpenCloseChanged) 
	EVT_COMMAND(wxID_ANY, wxEVT_JSDPANEL_AUTOTRACKPRICE_CHANGED, OnAutoTrackPriceChanged) 
    EVT_STYLE_CHANGE(OnCfgChanged)//���ı�ʱ���ػ��µ���
	EVT_CFG_CHANGE(OnCfgChanged)
	EVT_SUBSCRIBE(OnInitSubscribe)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_ADD, OnMAPlatformAdd)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_DELETE, OnMAPlatformDelete)
	EVT_COMMAND(wxID_ANY, wxEVT_GRID_INSTRUMENTID_CHANGED, OnCMDGridInstrumentIDChanged) 
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERINSERT_VIEWMODE_CHANGED, OnCMDViewModeChanged) 
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERINSERT_KEYORDER, OnCMDKeyOrder) 
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERINSERT_MOUSEORDER, OnCMDMouseOrder) 
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERINSERT_MOUSECLOSE, OnCMDMouseClose) 
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERINSERT_ORDERCHANGE, OnCMDOrderChange) 
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERINSERT_ORDERREPLACE, OnCMDOrderReplace) 
	EVT_MOUSEWHEEL(OnMouseWheel)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


OrderInsertPanel::OrderInsertPanel(wxWindow *parent,
            wxWindowID winid ,
            const wxPoint& pos ,
            const wxSize& size ,
            long style,
            const wxString& name ):
	COrderInsertOperations(parent,winid,pos,size,style,name),
    m_timer(this,ID_QUOTTIMER),
	m_bUpdateRealMsg_Multiply(true),
	m_bUpdateRealMsg_Available(true),
	m_bUpdateRealMsg_OpenVolume(true),
	m_bUpdateRealMsg_PreClose(true)
{
    //������¶�ʱ��
    m_timer.Start(1000);
	s_pOrderInsertPanel=this;


	wxCommandEvent event(wxEVT_LANGUAGE_CHANGE,wxID_ANY);
	OnLanguageChanged(event);
    DoInitQryStep();

	CPopMsgWin::Stru_Param msgwinpam;
    strcpy(msgwinpam.szTitle,"ѯ��֪ͨ");
    msgwinpam.MaxMsgLen=655360;
	msgwinpam.InitPox=10;
    msgwinpam.InitWid=300;
    msgwinpam.InitHei=400;
	m_pPopMsgWnd = NULL;
	if(GlobalConfigManager::m_CanReceiveForQuote) {
		m_pPopMsgWnd = new CPopMsgWin(msgwinpam);
		while(!m_pPopMsgWnd->IsWndCreated()) 
			Sleep(5);
	}
}

OrderInsertPanel::~OrderInsertPanel()
{
	m_timer.Stop();
	s_pOrderInsertPanel=NULL;
	if(m_pPopMsgWnd != NULL) {
		delete m_pPopMsgWnd;
		m_pPopMsgWnd = NULL;
	}
}

OrderInsertPanel * OrderInsertPanel::CreatePane(wxWindow* parent)
{
    return new OrderInsertPanel(parent, GID_ORDER_INSERT_PANEL);
}

void OrderInsertPanel::SetViewStyle(int nViewStyle, int nViewSubStyle)
{
	
	__super::SetViewStyle(nViewStyle, nViewSubStyle);

	wxCheckBox* itemCheckWDShow = (wxCheckBox*)FindWindowById(ID_CHECK_WDSHOW, this);
	if(itemCheckWDShow!=NULL) {
		itemCheckWDShow->SetValue(NeedFivePriceList()?true:false);
		wxCommandEvent evttemp(wxEVT_COMMAND_CHECKBOX_CLICKED, ID_CHECK_WDSHOW);
		evttemp.SetInt(NeedFivePriceList());
		OnCheckWDShowClicked(evttemp);
	}

}

void OrderInsertPanel::SetViewSubStyle(int nViewSubStyle)
{
	
	__super::SetViewSubStyle(nViewSubStyle);

	wxCheckBox* itemCheckWDShow = (wxCheckBox*)FindWindowById(ID_CHECK_WDSHOW, this);
	if(itemCheckWDShow!=NULL) {
		itemCheckWDShow->SetValue(NeedFivePriceList()?true:false);
        //wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED,ID_BUTTON_REMOVE);
        //ProcessEvent(evt);
		wxCommandEvent evttemp(wxEVT_COMMAND_BUTTON_CLICKED, ID_CHECK_WDSHOW);
		evttemp.SetInt(NeedFivePriceList());
		OnCheckWDShowClicked(evttemp);
	}

}

void OrderInsertPanel::NormalOrder(int nOrderType, int nMinVolume)
{
	wxString strName=GetInputInstrumentID();

		set<string> InstrumentIDArray;
		InstrumentIDArray.clear();
		if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
		if(InstrumentIDArray.find(strName.c_str())==InstrumentIDArray.end()) {
			ShowTradeInfoDlg("OIP_ERROR","OIP_CONTRERR",TRUE);
			return;
		}

	wxString strAccount=GetInputAccount();
    if(GetInputQty()<=0)
    {
		ShowTradeInfoDlg("OIP_ERROR","OIP_QTYBIGZERO",TRUE);
        return;
    }

	double fPrice = GetInputPrice();
	if(CheckSumPrice(fPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_NORMAL),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}

    wxString strMsg;

   //��ü۸񣬺��Ƿ��м�
	wxString strPriceText=GetInputPriceText();
    BOOL bIsMarketPrice = FALSE;
	fPrice = 0.0f;
	if(strPriceText.compare(LOADSTRING(OLS_MARKET))==0) {
		bIsMarketPrice=TRUE;
	}
	else {
		bIsMarketPrice=FALSE;
		fPrice = atof(strPriceText.c_str());
	}

	// ��ȡ����
	int nVolume = GetInputQty();

	if(nOrderType==OrderType_FT_ImmeFAK && (nMinVolume<0 || nMinVolume>nVolume)) {
		wxMessageDialog dialog(NULL,LOADSTRING(FAK_ERROR_MSG),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxYES_NO|wxICON_ERROR);
		if(dialog.ShowModal()!=wxID_YES)
			return;
	}

	// ��ȡ����
	BOOL bDirection = GetInputDirection();// GetButtonCheck(ID_RADIOBUTTON1);
	
	// ��ȡ��ƽ��
	int nOCMode = GetInputOCMode();

	//��ȡ�Ƿ��Զ�
	BOOL bIsAuto = GetInputAutoOpenClose();

	//bool bHedge=!((wxCheckBox*)FindWindowById(ID_CHECKBOX1,this))->GetValue();
	int nHedgeStatus = GetInputHedge();
	
	COrderInsertEntity::ORIORDERREQ tOriReq={0};
	tOriReq.nOrderType = nOrderType;
	strcpy(tOriReq.strInstrumentID, strName.c_str());
	strcpy(tOriReq.strAccount, strAccount.c_str());
	tOriReq.bIsParketOrder = FALSE;
	tOriReq.bDirection = bDirection;
	tOriReq.nOCMode = nOCMode;
	tOriReq.fPrice = fPrice;
	tOriReq.bIsMarketPrice = bIsMarketPrice;
	tOriReq.bSimulate = NeedSimulateMarketPrice();
	tOriReq.nHedgeStatus = nHedgeStatus;
	tOriReq.nVolume = nVolume;
	tOriReq.nDefaultVolume = nVolume;//GetDefaultVolume(strName);��OK��ť�£�ʹ������������ΪĬ������
	tOriReq.bIsAuto = FALSE; //bIsAuto;
	NeedAutoOpenClose(tOriReq.nAutoMode);
	tOriReq.nMinVolume = nMinVolume;

	BOOL bHasOrderInsert = FALSE;
	CConfirmItemChange confirmChg;
	confirmChg.SetItemNum(CConfirmItemChange::conStdOrderConfirm);
	DoOrderInsert(tOriReq, 0, bHasOrderInsert, confirmChg, TRUE);

}

void OrderInsertPanel::OnOk(wxCommandEvent& event)
{

	wxWindow* pWin = FindWindowById(ID_BUTTON_OK, this);
	if(pWin != NULL) {
		if(!pWin->IsShown() || !pWin->IsEnabled())
			return;
	}

	// ��ȡ��ƽ��
	int nOCMode = GetInputOCMode();
    if(nOCMode<0||nOCMode>2)
    {
        wxMessageBox(LOADSTRING(MSG_PleaseSelectOpenCloseMode));
        return;
    }

	USERLOG_INFO("�û������µ���[�µ���ť]\n%s", m_poOrderInputPanel->GetUserInputText().c_str());

	NormalOrder();
	
}

void OrderInsertPanel::OnCancel(wxCommandEvent& event)
{
	USERLOG_INFO("�û������µ���[ȡ����ť]\n");

    SetInputQty(0); 
}

void OrderInsertPanel::OnAutoOpenCloseChanged(wxCommandEvent& event)
{
	wxString strCode = GetInputInstrumentID();
	if(event.GetInt()) {
		int nSumVolume = 0, nSumTodayVolume=0;
		BOOL bIsBuy = GetInputDirection();
		BOOL bIsHedge = GetInputHedge();
		// ��ƽ�� �ܲ�/���
		COrderInsertEntity::GetPreVolume(strCode.c_str(), !bIsBuy, bIsHedge, nSumVolume, nSumTodayVolume, string(GetInputAccount().c_str()));
		if(nSumTodayVolume>0) {
			SetInputOCMode(1);
			//SetInputQty(nSumTodayVolume);
		}
		else if(nSumVolume>0){
			SetInputOCMode(2);
			//SetInputQty(nSumVolume-nSumTodayVolume);
		}
		else {
			SetInputOCMode(0);
			//SetInputQty(GetDefaultVolume(strCode));
		}
	}
	else {
		//SetInputQty(GetDefaultVolume(strCode));
	}

}

void OrderInsertPanel::OnAutoTrackPriceChanged(wxCommandEvent& event)
{
}

void OrderInsertPanel::OnBuySellSelChanged(wxCommandEvent& event)
{
	m_bUpdateRealMsg_OpenVolume = true;
	m_bUpdateRealMsg_PreClose = true;
}

void OrderInsertPanel::OnInstrumentIdTextChanged(wxCommandEvent& event)
{

	double fPriceTick = 0.0;

    std::string EvtParamStr;
    if(1)
    {
        DWORD EvtParamID=(DWORD)event.GetInt();
        if(CFTEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        {
            CFTEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }

    wxString strName(EvtParamStr);
//	wxString strName=event.GetString();
	wxString strLowerName = strName;
	strLowerName.LowerCase();

	// ���û�����ĺ�Լ��������ݴ���ת��
	if(!m_bIsGridChangeInstrumentID) {
		map<string, string>::iterator it;
		map<string, string> mapConstractIDVariety;
		mapConstractIDVariety.clear();
		if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentVarietyMap(mapConstractIDVariety);
		it = mapConstractIDVariety.find(strLowerName.c_str());
		if(it != mapConstractIDVariety.end()) {
			if(strName.Cmp(it->second.c_str())!=0) {
				SetInputInstrumentID(wxString(it->second.c_str()));
				return;
			}
		}
	}
	
	// �����������ĸ�������
	//post event to COrderServiceThread
//	if(event.GetInt()==0) {
	if(!m_bIsGridChangeInstrumentID) {
		wxCommandEvent evtnew(wxEVT_ORDERINSERT_INSTRUMENTID_CHANGED, GetId());
	    if(1)
	    {
		    DWORD EvtParamID;
		    std::string EvtParamStr(strName.c_str());
		    if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
		    {
			    evtnew.SetInt(EvtParamID);
			    GETTOPWINDOW()->AddPendingEvent(evtnew);
		    }
	    }
//		evtnew.SetString(strName.c_str());
//		GETTOPWINDOW()->AddPendingEvent(evtnew);
	}

	m_bUpdateRealMsg_Multiply = true;
	m_bUpdateRealMsg_OpenVolume = true;
	m_bUpdateRealMsg_PreClose = true;

	if(GetInputAutoOpenClose()) {
		int nSumVolume = 0, nSumTodayVolume=0;
		BOOL bIsBuy = GetInputDirection();
		BOOL bIsHedge = GetInputHedge();
		// ��ƽ�� �ܲ�/���
		COrderInsertEntity::GetPreVolume(strName.c_str(), !bIsBuy, bIsHedge, nSumVolume, nSumTodayVolume, string(GetInputAccount().c_str()));
		if(nSumTodayVolume>0) {
			SetInputOCMode(1);
			//SetInputQty(nSumTodayVolume);
		}
		else if(nSumVolume>0){
			SetInputOCMode(2);
			//SetInputQty(nSumVolume-nSumTodayVolume);
		}
		else {
			SetInputOCMode(0);
			//SetInputQty(GetDefaultVolume(strCode));
		}
	}
	else {
		//SetInputQty(GetDefaultVolume(strCode));
	}

//	if(m_poStaticFivePriceList!=NULL) {
//		m_poStaticFivePriceList->ResetDepthMarketData(strName);
//	}
	set<string> InstrumentIDArray;
	InstrumentIDArray.clear();
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
    if(InstrumentIDArray.find(strName.c_str())==InstrumentIDArray.end()) {
        //InitDynamicUpdateItem();
		if(m_poStaticFivePriceList!=NULL) {
			m_poStaticFivePriceList->ResetDepthMarketData();
 			m_poStaticFivePriceList->Refresh();
		}
		return;
    }

    PlatformStru_InstrumentInfo insInfo;
	memset(&insInfo,0,sizeof(insInfo));
    if(DEFAULT_SVR()&&DEFAULT_SVR()->GetInstrumentInfo(strName.c_str(),insInfo)!=0)
    {
        //InitDynamicUpdateItem();
		if(m_poStaticFivePriceList!=NULL) {
			m_poStaticFivePriceList->ResetDepthMarketData();
			m_poStaticFivePriceList->Refresh();
		}
        return;
    }
 	if(m_poOrderInputPanel!=NULL) {
		m_poOrderInputPanel->SetPriceTick(insInfo.PriceTick);
	}
 	if(m_poPatsOrderInputPanel!=NULL) {
		m_poPatsOrderInputPanel->SetPriceTick(insInfo.PriceTick);
	}
	if(m_poOrderConditionPanel!=NULL) {
		m_poOrderConditionPanel->SetPriceTick(insInfo.PriceTick);
	}
	if(m_poOrderPositionPanel!=NULL) {
		m_poOrderPositionPanel->SetPriceTick(insInfo.PriceTick);
	}
	if(m_poOrderStopPanel!=NULL) {
		m_poOrderStopPanel->SetPriceTick(insInfo.PriceTick);
	}
	if(m_poStaticFivePriceList!=NULL) {
		m_poStaticFivePriceList->SetPriceTick(insInfo.PriceTick);
	}
	m_currPriceTick=insInfo.PriceTick;

	//ʵʱ��������
	static wxString LastValidInstrumentID;
	BOOL bIsComboCode = FALSE;
	string strCmbSubCode1, strCmbSubCode2;
	if(LastValidInstrumentID!=strName && DEFAULT_SVR())
	{
		if(!LastValidInstrumentID.empty()) {
			bIsComboCode = DEFAULT_SVR()->GetProductClassType(LastValidInstrumentID.c_str()) == THOST_FTDC_PC_Combination;
			if(bIsComboCode) {
				ConvertComboInstrumentID(string(LastValidInstrumentID.c_str()), strCmbSubCode1, strCmbSubCode2);
				DEFAULT_SVR()->SetSubscribeStatus( strCmbSubCode1,GID_ORDER_INSERT_PANEL,eUnsubscribeMarketData);
				DEFAULT_SVR()->SetSubscribeStatus( strCmbSubCode2,GID_ORDER_INSERT_PANEL,eUnsubscribeMarketData);
			}
			DEFAULT_SVR()->SetSubscribeStatus( LastValidInstrumentID.c_str(),GID_ORDER_INSERT_PANEL,eUnsubscribeMarketData);
		}
		bIsComboCode = DEFAULT_SVR()->GetProductClassType(strName.c_str()) == THOST_FTDC_PC_Combination;
		if(bIsComboCode) {
			ConvertComboInstrumentID(string(strName.c_str()), strCmbSubCode1, strCmbSubCode2);
			DEFAULT_SVR()->SetSubscribeStatus( strCmbSubCode1,GID_ORDER_INSERT_PANEL);
			DEFAULT_SVR()->SetSubscribeStatus( strCmbSubCode2,GID_ORDER_INSERT_PANEL);
		}
		DEFAULT_SVR()->SetSubscribeStatus( strName.c_str(),GID_ORDER_INSERT_PANEL);
		LastValidInstrumentID=strName;
	}

	PlatformStru_DepthMarketData field;
	memset(&field,0,sizeof(field));
	m_NeedUpdatePrice = true;
	BOOL bHasQuot = FALSE;
    if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(strName.c_str(),field)==0) {
		bHasQuot = TRUE;
    }
 	if(m_poOrderInputPanel!=NULL) {
		m_poOrderInputPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderInputPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	if(m_poOrderConditionPanel!=NULL) {
		m_poOrderConditionPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderConditionPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	if(m_poOrderPositionPanel!=NULL) {
		m_poOrderPositionPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderPositionPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	if(m_poOrderStopPanel!=NULL) {
		m_poOrderStopPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderStopPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	m_currUpperLimitPrice=field.UpperLimitPrice;
	m_currLowerLimitPrice=field.LowerLimitPrice;

	if(m_poStaticFivePriceList->ResetDepthMarketData(field)) {	
	}
    m_poStaticFivePriceList->Refresh();
	if(bHasQuot) {
		if(GetInputDirection()) {
			if(util::isInvalidValue(field.AskPrice1)) {
				if(field.Volume > 0)
					SetInputPrice(field.LastPrice);
				else 
					SetInputPrice(field.PreSettlementPrice);
			}
			else
				SetInputPrice(field.AskPrice1);
		}
		else {
			if(util::isInvalidValue(field.BidPrice1)) {
				if(field.Volume > 0)
					SetInputPrice(field.LastPrice);
				else 
					SetInputPrice(field.PreSettlementPrice);
			}
			else
				SetInputPrice(field.BidPrice1);
		}
		m_NeedUpdatePrice = false;
	}
	SetInputQty(GetDefaultVolume(strName));


	wxCheckBox* pitemCheck = (wxCheckBox*)FindWindowById(ID_DOSTOPPOSITION_CHECK, this);
	if(pitemCheck!=NULL) {
		if(pitemCheck->GetValue()!=0) {

			// �����xml�ж��Ƿ�Ҫȷ�ϵ�����
			CfgMgr* pMgr = CFG_MGR_DEFAULT();  
			if(pMgr != NULL) {

			vector<VarietyInfo>* parrVarietyInfo;
			parrVarietyInfo = pMgr->GetVarietyInfoVector();
			VarietyInfo tvInfo;
			wxString strCode = GetInputInstrumentID();
			double fGainOffset = 0.0;
			double fLoseOffset = 0.0;
			double fReverseOffset = 0.0;
			double fRetPrice = 0.0;
			BOOL bFound = FALSE;
			
			for(int i=0; i<(int)parrVarietyInfo->size(); i++) {
				tvInfo = (*parrVarietyInfo)[i];
				wxString strValue = tvInfo.InstruName;
				if(strCode.compare(strValue.c_str())==0 
						|| strnicmp(strValue.c_str(), strCode.c_str(), strValue.size())==0) {
					fLoseOffset = atof(tvInfo.loss);
					fGainOffset = atof(tvInfo.profit);
					fReverseOffset = atof(tvInfo.reverse);
					bFound = TRUE;
					break;
				}
			}
			if(bFound) {
				m_poOrderPositionPanel->SetLoseOffsetPrice(
						m_poOrderPositionPanel->GetLoseOffsetPrice(fRetPrice), 
						fLoseOffset*m_currPriceTick);
				m_poOrderPositionPanel->SetGainOffsetPrice(
						m_poOrderPositionPanel->GetGainOffsetPrice(fRetPrice), 
						fGainOffset*m_currPriceTick);
				m_poOrderPositionPanel->SetCloseOffsetPrice(
						fReverseOffset*m_currPriceTick);
			}
			else {
				m_poOrderPositionPanel->SetLoseOffsetPrice(
						m_poOrderPositionPanel->GetLoseOffsetPrice(fRetPrice), 0.0f);
				m_poOrderPositionPanel->SetGainOffsetPrice(
						m_poOrderPositionPanel->GetGainOffsetPrice(fRetPrice), 0.0f);
				m_poOrderPositionPanel->SetCloseOffsetPrice(0.0f);
			}
			}
		}
	}

}

void OrderInsertPanel::OnTimer(wxTimerEvent& event)
{
    wxString name=GetInputInstrumentID();
    PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	double fPriceTick = m_currPriceTick;
	
	BOOL bHasQuot = FALSE;
	// ����ط���Ҫ���в�ѯ�Ż��������ԼΪ�ӷ�����������ϣ���δ�����������ϣ���δ���
    if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(name.c_str(),field)==0) {
		bHasQuot = TRUE;
    }
 	if(m_poOrderInputPanel!=NULL) {
		m_poOrderInputPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderInputPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	if(m_poOrderConditionPanel!=NULL) {
		m_poOrderConditionPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderConditionPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	if(m_poOrderPositionPanel!=NULL) {
		m_poOrderPositionPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderPositionPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	if(m_poOrderStopPanel!=NULL) {
		m_poOrderStopPanel->SetUpperLimitPrice(field.UpperLimitPrice);
		m_poOrderStopPanel->SetLowerLimitPrice(field.LowerLimitPrice);
	}
	m_currUpperLimitPrice=field.UpperLimitPrice;
	m_currLowerLimitPrice=field.LowerLimitPrice;

	m_currPriceTick = fPriceTick;

	if(bHasQuot) {
		//���¸��̼�
		if(GetInputAutoTrackPrice()) {
			if(GetInputDirection())
				SetInputPrice(field.AskPrice1);
			else
				SetInputPrice(field.BidPrice1);
		}
		else {
			if(m_NeedUpdatePrice) {
				m_NeedUpdatePrice=FALSE;
				if(GetInputDirection())
					SetInputPrice(field.AskPrice1);
				else
					SetInputPrice(field.BidPrice1);
			}
		}
	}
	if(m_poStaticFivePriceList->ResetDepthMarketData(field)) {
		m_poStaticFivePriceList->Refresh();
	}

	static int cnt1=0,cnt2=0;

	if(m_bWaitingRsp_OrderInsert) {
		cnt1++;
		if(cnt1>6) {
			m_bWaitingRsp_OrderInsert=false;
			cnt1=0;
            //wxMessageBox("�µ��ɹ��������ۺϽ���ƽ̨û�з���!");
		}
	}
	else {
		cnt1=0;
	}

	if(m_bWaitingRsp_OrderDelete) {
		cnt2++;
		if(cnt2>6) {
			m_bWaitingRsp_OrderDelete=false;
			cnt2=0;
            //wxMessageBox("�����ɹ��������ۺϽ���ƽ̨û�з���!");
		}
	}
	else {
		cnt2=0;
	}


	// ������Щ�µ�ǰ��Ҫ����ǰ����عҵ���ԭʼ��
	std::vector<COrderInsertEntity*> arrOverdue;
	std::map<COrderInsertEntity*, int>::iterator itOrder;
	itOrder = m_mapOrderInsertWaitForCancel.begin();

	while(itOrder!=m_mapOrderInsertWaitForCancel.end()) {
		if(itOrder->second>0) itOrder->second--;
		if(itOrder->second>0) {
			// δ����
			m_mapOrderInsertWaitForCancel[itOrder->first] = itOrder->second;
		}
		else {
			// �ѹ���
			arrOverdue.push_back(itOrder->first);
		}
		itOrder++;
	}

	// ������й��ڵĶ���
	for(int i=0; i<(int)arrOverdue.size(); i++) {
		COrderInsertEntity* pEntity = arrOverdue[i];
		m_mapOrderInsertWaitForCancel.erase(pEntity);
		wxString strMsg = pEntity->GetRevokeOrderDesciption();
		strMsg += pEntity->GetOrderDesciption();
		ShowTradeInfoDlg(LOADSTRING(CANCEL_OVER_TIME), strMsg.c_str(), TRUE);
		try {
			delete pEntity;
		}
		catch(...) {}
	}
	arrOverdue.clear();

	wxString strCode = GetInputInstrumentID();
	double fPrice = GetInputPrice();
	BOOL bIsBuy = GetInputDirection();
	BOOL bIsHedge = GetInputHedge();
	int nDefaultVolume = 1, nMultiplyVolume = 1;
	GetDefaultVolume(strCode, nDefaultVolume, nMultiplyVolume);

    BOOL bIsMarketPrice = FALSE;
	wxString strPriceText = GetInputPriceText();
	if(strPriceText.compare(LOADSTRING(OLS_MARKET))==0) {
		bIsMarketPrice=TRUE;
	}
	else {
		bIsMarketPrice=FALSE;
	}

	m_poOrderRealMsgPanel->ResetMsg(strCode, bIsBuy, bIsHedge, fPrice, 
			nDefaultVolume, nMultiplyVolume, bIsMarketPrice?true:false, wxString(GetInputAccount().c_str()), 
			m_bUpdateRealMsg_PreClose);
	m_bUpdateRealMsg_Multiply = m_bUpdateRealMsg_PreClose = false;
	//m_bUpdateRealMsg_Available = m_bUpdateRealMsg_OpenVolume = false;

}

void OrderInsertPanel::OnRcvOrder(wxCommandEvent& evt)
{
	AbstractBusinessData* pAbstractBusinessData=static_cast<AbstractBusinessData*>(evt.GetClientData());
	if(!pAbstractBusinessData)
	{
		wxASSERT(pAbstractBusinessData);
		return;
	}

	switch (pAbstractBusinessData->BID)
	{
		case BID_RtnOrder:
		{
			DataRtnOrder& RawData= *(DataRtnOrder*)(pAbstractBusinessData);

			if(!HasOwner(RawData.OrderField.FrontID, RawData.OrderField.SessionID))
				return;

			{
			// ������Щ�µ�ǰ��Ҫ����ǰ����عҵ���ԭʼ��
			std::vector<COrderInsertEntity*> arrComplete;
			std::map<COrderInsertEntity*, int>::iterator itOrder;
			itOrder = m_mapOrderInsertWaitForCancel.begin();

			while(itOrder!=m_mapOrderInsertWaitForCancel.end()) {
				COrderInsertEntity* pEntity = itOrder->first;
				pEntity->MarkRevokeItemState(RawData);
				if(pEntity->IsRevokeAllOK()) {
					arrComplete.push_back(pEntity);
				}
				itOrder++;
			}

			// ������й��ڵĶ���
			for(int i=0; i<(int)arrComplete.size(); i++) {
				COrderInsertEntity* pEntity = arrComplete[i];
				m_mapOrderInsertWaitForCancel.erase(pEntity);
				int nErrPos=-1;
				wxString strMsg;
				pEntity->DoOrder(nErrPos, strMsg);
				if(nErrPos==-1) {
					m_bWaitingRsp_OrderInsert=true;
				}
				else {
					//���ô�����Ϣ
					//strcpy(tmpData.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(ret).c_str());
					//ShowTradeInfoDlg(wxString("�µ�ʧ��"), tmpData);
					//if(NeedOrderFailConfirm())
					ShowTradeInfoDlg(LOADSTRING(OLS_SEND_ORDER_FAIL), pEntity->GetOrderReq(nErrPos),NeedOrderFailConfirm());
				}
				try {
					delete pEntity;
				}
				catch(...) {}
			}
			arrComplete.clear();
			}

			{
			// ������Щ�µ�ǰ��Ҫ����ǰ����عҵ���ԭʼ��
			std::vector<COrderChangeEntity*> arrComplete;
			std::map<COrderChangeEntity*, int>::iterator itOrder;
			itOrder = m_mapOrderChangeWaitForCancel.begin();

			while(itOrder!=m_mapOrderChangeWaitForCancel.end()) {
				COrderChangeEntity* pEntity = itOrder->first;
				pEntity->MarkRevokeItemState(RawData);
				if(pEntity->IsRevokeAllOK()) {
					arrComplete.push_back(pEntity);
				}
				itOrder++;
			}

			// ������й��ڵĶ���
			for(int i=0; i<(int)arrComplete.size(); i++) {
				COrderChangeEntity* pEntity = arrComplete[i];
				m_mapOrderChangeWaitForCancel.erase(pEntity);
				int nErrPos=-1;
				wxString strMsg;
				pEntity->DoOrder(nErrPos, strMsg);
				if(nErrPos==-1) {
					m_bWaitingRsp_OrderInsert=true;
				}
				else {
					//���ô�����Ϣ
					//strcpy(tmpData.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(ret).c_str());
					//ShowTradeInfoDlg(wxString("�µ�ʧ��"), tmpData);
					//if(NeedOrderFailConfirm())
					ShowTradeInfoDlg(LOADSTRING(OLS_SEND_ORDER_FAIL), pEntity->GetOrderReq(nErrPos),NeedOrderFailConfirm());
				}
				try {
					delete pEntity;
				}
				catch(...) {}
			}
			arrComplete.clear();
			}

			m_bUpdateRealMsg_Available = true;
			m_bUpdateRealMsg_OpenVolume = true;
		}
			break;
		case BID_RspOrderInsert:
		{
		}
			break;
		case BID_ErrRtnOrderInsert:
		{
		}
		break;
		case BID_RspOrderAction:
		{
		}
		break;
		case BID_ErrRtnOrderAction:
		{
			DataErrRtnOrderAction& RawData= *(DataErrRtnOrderAction*)(pAbstractBusinessData);
			//�˴����Դ�����ʧ�ܵ����
			//OrderActionField.OrderActionStatus == THOST_FTDC_OAS_Rejected // ���ܾ�
			
			// ������Щ�µ�ǰ��Ҫ����ǰ����عҵ���ԭʼ��
			std::vector<COrderInsertEntity*> arrComplete;
			std::map<COrderInsertEntity*, int>::iterator itOrder;
			itOrder = m_mapOrderInsertWaitForCancel.begin();

			while(itOrder!=m_mapOrderInsertWaitForCancel.end()) {
				COrderInsertEntity* pEntity = itOrder->first;
				if(pEntity->FindRspOrderAction(RawData)) {
					arrComplete.push_back(pEntity);
				}
				itOrder++;
			}

			// ������й��ڵĶ���
			for(int i=0; i<(int)arrComplete.size(); i++) {
				COrderInsertEntity* pEntity = arrComplete[i];
				m_mapOrderInsertWaitForCancel.erase(pEntity);
				int nErrPos=-1;
				wxString strMsg;
				pEntity->DoOrder(nErrPos, strMsg);
				if(nErrPos==-1) {
					m_bWaitingRsp_OrderInsert=true;
				}
				else {
					//���ô�����Ϣ
					//strcpy(tmpData.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(ret).c_str());
					//ShowTradeInfoDlg(wxString("�µ�ʧ��"), tmpData);
					//if(NeedOrderFailConfirm())
					ShowTradeInfoDlg(LOADSTRING(OLS_CANCEL_ORDER_FAIL), pEntity->GetOrderReq(nErrPos),NeedOrderFailConfirm());
				}
				try {
					delete pEntity;
				}
				catch(...) {}
			}
			arrComplete.clear();

			if(!m_bWaitingRsp_OrderDelete)
				break;
			//if(RawData.OrderActionField.RequestID!=COrderServiceThread::sm_nReqNum)
			if(!DEFAULT_SVR()||RawData.OrderActionField.RequestID!=DEFAULT_SVR()->GetNextReqID())
				break;			

//			if(NeedOrderFailConfirm())
//				ShowTradeInfoDlg(wxString("����ʧ��"), RawData.OrderActionField);
//			m_bWaitingRsp_OrderDelete=false;
		}
			break;
		case BID_RtnTrade:
		{
			DataRtnTrade& RawData= *(DataRtnTrade*)(pAbstractBusinessData);
			if(!HasOwnerTrade(RawData.TradeField))
				return;

			if(GetInputAutoOpenClose()) {
				wxString strInstrumentID = GetInputInstrumentID();
				BOOL bDirection = GetInputDirection();
				BOOL bIsBuy = RawData.TradeField.Direction == THOST_FTDC_D_Buy;
				if(strInstrumentID.Cmp(RawData.TradeField.InstrumentID)==0 && bDirection!=bIsBuy) {
					SetInputOCMode(1);
				}
			}
			wxString strInstrumentID = GetInputInstrumentID();
			if(strInstrumentID.CmpNoCase(RawData.TradeField.InstrumentID)==0)
			{
				m_bUpdateRealMsg_PreClose = true;
			}
		}
			break;
		default:
			break;
	}
	
	delete (void*)pAbstractBusinessData;
	pAbstractBusinessData=NULL;
	evt.SetClientData(NULL);
}

void OrderInsertPanel::OnRcvSubsForQuote(wxCommandEvent& evt)
{
	AbstractBusinessData* pAbstractBusinessData=static_cast<AbstractBusinessData*>(evt.GetClientData());
	if(!pAbstractBusinessData || pAbstractBusinessData->BID!=BID_RspSubForQuoteRsp) {
		wxASSERT(pAbstractBusinessData);
		return;
	}

	DataRspSubForQuoteRsp& RawData= *(DataRspSubForQuoteRsp*)(pAbstractBusinessData);
	wxString strText;

	strText.Printf("����ѯ��:\r\n��Լ[%s]", 
			RawData.InstrumentID);

	
	if(m_pPopMsgWnd != NULL)
		m_pPopMsgWnd->AppendStr(CPopMsgWin::ForceShow,
				CPopMsgWin::NoDelOldContent,
				CPopMsgWin::AddCRLF,
				CPopMsgWin::InsertTime, 
				strText.c_str());

	delete (void*)pAbstractBusinessData;
	evt.SetClientData(NULL);
}

void OrderInsertPanel::OnRcvUnSubsForQuote(wxCommandEvent& evt)
{
	AbstractBusinessData* pAbstractBusinessData=static_cast<AbstractBusinessData*>(evt.GetClientData());
	if(!pAbstractBusinessData || pAbstractBusinessData->BID!=BID_RspUnSubForQuoteRsp) {
		wxASSERT(pAbstractBusinessData);
		return;
	}

	DataRspUnSubForQuoteRsp& RawData= *(DataRspUnSubForQuoteRsp*)(pAbstractBusinessData);
	wxString strText;

	strText.Printf("�˶�ѯ��:\r\n��Լ[%s]", 
			RawData.InstrumentID);

	if(m_pPopMsgWnd != NULL)
		m_pPopMsgWnd->AppendStr(CPopMsgWin::ForceShow,
				CPopMsgWin::NoDelOldContent,
				CPopMsgWin::AddCRLF,
				CPopMsgWin::InsertTime, 
				strText.c_str());

	delete (void*)pAbstractBusinessData;
	evt.SetClientData(NULL);
}

void OrderInsertPanel::OnRcvForQuoteRsp(wxCommandEvent& evt)
{
	AbstractBusinessData* pAbstractBusinessData=static_cast<AbstractBusinessData*>(evt.GetClientData());
	if(!pAbstractBusinessData || pAbstractBusinessData->BID!=BID_RtnForQuoteRsp) {
		wxASSERT(pAbstractBusinessData);
		return;
	}

	DataRtnForQuoteRsp& RawData= *(DataRtnForQuoteRsp*)(pAbstractBusinessData);
	wxString strText;

	strText.Printf("ѯ��֪ͨ:\r\n��Լ[%s], ʱ��[%s], ����[%s]", 
			RawData.ForQuoteRsp.InstrumentID,
			RawData.ForQuoteRsp.ForQuoteTime,
			RawData.ForQuoteRsp.ActionDay);

	if(m_pPopMsgWnd != NULL)
		m_pPopMsgWnd->AppendStr(CPopMsgWin::ForceShow,
				CPopMsgWin::NoDelOldContent,
				CPopMsgWin::AddCRLF,
				CPopMsgWin::InsertTime, 
				strText.c_str());

	delete (void*)pAbstractBusinessData;
	evt.SetClientData(NULL);
}

void OrderInsertPanel::OnPanelCharHook(wxCommandEvent& evt)
{
    wxKeyEvent* pEvent=(wxKeyEvent*)evt.GetClientData();

	if(!pEvent) return;
    wxWindow *win = FindFocus();
    if(win == NULL) {
		evt.Skip();
        return;
    }
	//pEvent->SetId(win->GetId());
	pEvent->SetId(CBaseInputPanel::ConvertOwnerDrawnComboBoxID(win->GetId(), this));


	// �����xml�ж��Ƿ�Ҫȷ�ϵ�����
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return;
	
	// Ĭ���µ�����
    int keycode=pEvent->GetKeyCode();
	map<int,string>::iterator itKey;
	//��ü������ַ�����ӳ���
	map<int,string> addr = pMgr->GetShortCutKeyNameMap();
	itKey = addr.find(keycode);
	if(itKey != addr.end()) {
		std::string strKeyName = itKey->second;
		std::map<std::string, int>::iterator itOrder;
		itOrder = m_KeyMultiplyVolumeMap.find(strKeyName);
		if(itOrder != m_KeyMultiplyVolumeMap.end()) {
			int nVolume = itOrder->second;
			SetMultiplyVolume(nVolume);
			m_bUpdateRealMsg_Multiply = true;
		}
	// �����xml�ж��Ƿ�Ҫȷ�ϵ�����
		LPORDER_CFG p = pMgr->GetOrderCfg();
		// ����µ���ť��ݼ�
		if(strKeyName.compare(p->szOpenOrderBtnHotKey)==0) {
			wxCommandEvent newEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_BUTTON_OK);
			this->AddPendingEvent(newEvent);
		}
		// �����Ԥ�񵥰�ť��ݼ�
		if(strKeyName.compare(p->szOrderParkedBtnHotKey)==0) {
			wxCommandEvent newEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_BUTTON_PARKEDORDER);
			this->AddPendingEvent(newEvent);
		}
	}

	//��ü������ַ�����ӳ���
	addr = pMgr->GetShortCutKeyNameMap();
	std::vector<VarietyInfo>* pVInfo = pMgr->GetVarietyInfoVector();
	VarietyInfo tvInfo;

	if(!(pEvent->GetId()==ID_OISTD_PRICE_TEXT || pEvent->GetId()==ID_OISTD_VOLUME_TEXT) || 
			!(pEvent->GetId()==ID_OIPATS_PRICE_TEXT || pEvent->GetId()==ID_OIPATS_VOLUME_TEXT)) {
		itKey = addr.find(keycode);
		if(itKey != addr.end()) {
			std::string strKeyName = itKey->second;
			for(int i=0; i<(int)pVInfo->size(); i++) {
				tvInfo = (*pVInfo)[i];
				wxString strValue = tvInfo.ShortKey;
				if(strValue.compare(strKeyName.c_str()) == 0) {
					wxString strInstrumentID = tvInfo.InstruName;
					SetInputInstrumentID(strInstrumentID);
					SetInputQty(tvInfo.volume);
					//bFound = TRUE;
					if(pEvent->GetId()==ID_OISTD_INSTRUMENTID_TEXT || 
							pEvent->GetId()==ID_OIPATS_INSTRUMENTID_TEXT) {
						win->SetFocus();
						return;
					}
					else
						break;
				}
			}
		}
	}

	int thisID=CBaseInputPanel::ConvertOwnerDrawnComboBoxID(win->GetId(), this);
	wxWindow* pTmpWin = NULL;

    if(pEvent->GetKeyCode()==WXK_RETURN || pEvent->GetKeyCode()==VK_RETURN) {
		if(m_nViewStyle!=3) {
			if(m_poOrderInputPanel) {
				wxKeyEvent keyEvent(*pEvent);
				m_poOrderInputPanel->OnHookKeyPress(keyEvent);
			}
		}
        int id, count=m_TabOrderVector.size();
		if(thisID==ID_BUTTON_OK || thisID==ID_BUTTON_BATCHORDER 
				|| thisID==ID_BUTTON_CONDITIONORDER || thisID==ID_BUTTON_POSITIONORDER 
				|| thisID==ID_BUTTON_CANCEL || thisID==ID_BUTTON_MARKETPRICE 
				|| thisID==ID_BUTTON_PARKEDORDER) {
			evt.Skip();
			return;
		}
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                if(id<count-1)
                {
                    FindWindowById(m_TabOrderVector[id+1],this)->SetFocus();
					//evt.Skip();
                    return;
                }
            }
        }
    }
	else if (pEvent->GetKeyCode()==WXK_TAB || pEvent->GetKeyCode()==VK_TAB) {
        bool backward = pEvent->ShiftDown ();
        int id, count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                if(backward)
                    id=id>0?id-1:count-1;
                else
                    id=id<count-1?id+1:0;
                FindWindowById(m_TabOrderVector[id],this)->SetFocus();
				//evt.Skip();
                return;
            }
        }
    }
    else if(pEvent->GetKeyCode()==WXK_UP || pEvent->GetKeyCode()==VK_UP)
    {
        int id, count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                int nIndex=id>0?id-1:count-1;
				FindWindowById(m_TabOrderVector[nIndex],this)->SetFocus();
				return;
            }
        }
 		//return;
	}
    else if(pEvent->GetKeyCode()==WXK_DOWN || pEvent->GetKeyCode()==VK_DOWN)
    {
		//if(m_poOrderInputPanel) {
		//	wxKeyEvent keyEvent(*pEvent);
		//	m_poOrderInputPanel->OnHookKeyPress(keyEvent);
		//	if(!keyEvent.GetSkipped()) {
		//		return;
		//	}
		//}

        int id, count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
				int nIndex=id<count-1?id+1:0;
				FindWindowById(m_TabOrderVector[nIndex],this)->SetFocus();
				//evt.Skip();
				return;
			}
        }
		//return;
    }
	else {
		if(DEFAULT_SVR()!=NULL) {
		if(DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
			if(m_poOrderInputPanel) {
				wxKeyEvent keyEvent(*pEvent);
				m_poOrderInputPanel->OnHookKeyPress(keyEvent);
				if(!keyEvent.GetSkipped()) {
					return;
				}
			}
		}
		else {//PTYPE_PATSAPI
			if(m_poPatsOrderInputPanel) {
				wxKeyEvent keyEvent(*pEvent);
				m_poPatsOrderInputPanel->OnHookKeyPress(keyEvent);
				if(!keyEvent.GetSkipped()) {
					return;
				}
			}
		}
		}
//		if(m_poOrderBatchPanel) {
//			wxKeyEvent keyEvent(*pEvent);
//			m_poOrderBatchPanel->OnHookKeyPress(keyEvent);
//			if(!keyEvent.GetSkipped()) {
//				return;
//			}
//		}
		if(m_poOrderConditionPanel) {
			wxKeyEvent keyEvent(*pEvent);
			m_poOrderConditionPanel->OnHookKeyPress(keyEvent);
			if(!keyEvent.GetSkipped()) {
				return;
			}
		}
		if(m_poOrderPositionPanel) {
			wxKeyEvent keyEvent(*pEvent);
			m_poOrderPositionPanel->OnHookKeyPress(keyEvent);
			if(!keyEvent.GetSkipped()) {
				return;
			}
		}
		//evt.Skip();
	}
	evt.Skip();
}

void OrderInsertPanel::OnBtnParkedOrder(wxCommandEvent& event)
{
	USERLOG_INFO("�û������µ���[Ԥ�񵥰�ť]\n%s", m_poOrderInputPanel->GetUserInputText().c_str());
	
	wxWindow* pWin = FindWindowById(ID_BUTTON_PARKEDORDER, this);
	if(pWin != NULL) {
		if(!pWin->IsShown() || !pWin->IsEnabled())
			return;
	}

	wxString strName=GetInputInstrumentID();

		set<string> InstrumentIDArray;
		DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
		if(InstrumentIDArray.find(strName.c_str())==InstrumentIDArray.end()) {
			ShowTradeInfoDlg("OIP_ERROR","OIP_CONTRERR",TRUE);
			return;
		}

    if(GetInputQty()<=0)
    {
		ShowTradeInfoDlg("OIP_ERROR","OIP_QTYBIGZERO",TRUE);
        return;
    }

	double fPrice = GetInputPrice();
	if(CheckSumPrice(fPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_NORMAL),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}

	if(GetInputQty() > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITQTY), LOADSTRING(OIP_FASTTRADER), 
			wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES)
			return;
	}

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// ����ط���Ҫ���в�ѯ�Ż��������ԼΪ�ӷ�����������ϣ���δ�����������ϣ���δ���
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strName.c_str(),field);

	BOOL bIsPriceLimit = GetInputPrice()>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
			|| GetInputPrice()<field.LastPrice - m_currPriceTick*GetRaskDifferPrice();
	bIsPriceLimit = bIsPriceLimit && GetRaskDifferPrice();
	if(bIsPriceLimit) {
			wxMessageDialog dialog( NULL,LOADSTRING(OIP_OVERLIMITPRICE),LOADSTRING(OIP_FASTTRADER), 
					wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			if(dialog.ShowModal()!=wxID_YES) 
				return;
	}

	// �����ǵ�ͣ����ʾ
	if(!util::isInvalidValue(field.UpperLimitPrice) && !util::isInvalidValue(field.LowerLimitPrice)
			&& stricmp(GetInputPriceText().c_str(), LOADSTRING(OLS_MARKET))!=0) 
	{
		bool bOverLimit = util::greater(GetInputPrice(), field.UpperLimitPrice) 
			|| util::less(GetInputPrice(), field.LowerLimitPrice);
		if(bOverLimit) {
			wxMessageDialog dialog(NULL, LOADSTRING(OIP_OVERLIMITPRICE_UL), LOADSTRING(OIP_FASTTRADER), 
					wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			if(dialog.ShowModal()!=wxID_YES) 
				return;
		}
	}
	
	wxString strMsg;

   //��ü۸񣬺��Ƿ��м�
	wxString strPriceText=GetInputPriceText();
    BOOL bIsMarketPrice = FALSE;
	fPrice = 0.0;
	if(strPriceText.compare(LOADSTRING(OLS_MARKET))==0) {
		bIsMarketPrice=TRUE;
	}
	else {
		bIsMarketPrice=FALSE;
		fPrice = atof(strPriceText.c_str());
		//fPrice = AnalysePrice(fPrice, strName);
		//SetInputPrice(fPrice);
	}

	// ��ȡ����
	int nVolume = GetInputQty();

	// ��ȡ����
	BOOL bDirection = GetInputDirection();// GetButtonCheck(ID_RADIOBUTTON1);
	
	// ��ȡ��ƽ��
	int nOCMode = GetInputOCMode();

	//��ȡ�Ƿ��Զ�
	BOOL bIsAuto = GetInputAutoOpenClose();

	int nHedgeStatus = GetInputHedge();

	COrderParkedSelectDlg dlg(NULL, bIsMarketPrice==TRUE);
	dlg.SetTitle(LOADSTRING(OIP_PARK_TRIGGER));
	if(dlg.ShowModal()!=wxID_YES) {
		USERLOG_INFO("Ԥ����ʾѡ�񣬱���[%s]��\n�û�ѡ��[��]",dlg.GetTitle());
		return;
	}

	int nParkedMode = dlg.GetParkedMode();
	USERLOG_INFO("Ԥ����ʾѡ�񣬱���[%s]��\n�û�ѡ��[��]��%s",dlg.GetTitle(),dlg.GetUserInputText().c_str());

	if(nParkedMode<2) {
		
		CLocalOrderService::PARKEDORDER tOrder = {0};

		PlatformStru_InputOrder req;
		ZeroMemory(&req, sizeof(PlatformStru_InputOrder));
		FillInputOrderField(req);

		tOrder.nState = conCondActive;
		tOrder.bIsAuto = (nParkedMode==1);
		tOrder.nCreateTime = ::wxGetUTCTime();
		tOrder.req = req;

		PlatformStru_InstrumentInfo insInfo;
		memset(&insInfo,0,sizeof(insInfo));
		if(DEFAULT_SVR()&&DEFAULT_SVR()->GetInstrumentInfo(tOrder.req.InstrumentID, insInfo)==0) {
			strcpy(tOrder.ExchangeID, insInfo.ExchangeID);
			if(m_poLocalOrderService) {
				m_poLocalOrderService->OrderParkedAdd(tOrder);
			}
			
			wxCommandEvent evtnew(wxEVT_QUERY_PARKED_REQUERY, GetId());
			GETTOPWINDOW()->AddPendingEvent(evtnew);
		}
		else {
			// ��Ԥ��ʧ�ܣ�ԭ����δ�ܲ���Լ�����Ǹ�������
		}

	}
	else {
		COrderInsertEntity::ORIORDERREQ tOriReq={0};
		strcpy(tOriReq.strInstrumentID, strName.c_str());
		tOriReq.bIsParketOrder = TRUE;
		tOriReq.bDirection = bDirection;
		tOriReq.nOCMode = nOCMode;
		tOriReq.fPrice = fPrice;
		tOriReq.bIsMarketPrice = bIsMarketPrice;
		tOriReq.bSimulate = NeedSimulateMarketPrice();
		tOriReq.nHedgeStatus = nHedgeStatus;
		tOriReq.nVolume = nVolume;
		tOriReq.nDefaultVolume = nVolume;//GetDefaultVolume(strName);��OK��ť�£�ʹ������������ΪĬ������
		tOriReq.bIsAuto = FALSE; //bIsAuto;
		NeedAutoOpenClose(tOriReq.nAutoMode);

		BOOL bHasOrderInsert = FALSE;
		CConfirmItemChange confirmChg;
		confirmChg.SetItemNum(CConfirmItemChange::conStdOrderConfirm);
		DoOrderInsert(tOriReq, 0, bHasOrderInsert, confirmChg, TRUE);
	}
}

void OrderInsertPanel::OnBtnMarketPrice(wxCommandEvent& event)
{
    wxString strText(LOADSTRING(OLS_MARKET));
	SetInputPriceText(strText);

	USERLOG_INFO("�û������µ���[�м۵���ť]\n%s", m_poOrderInputPanel->GetUserInputText().c_str());
	
	NormalOrder();
}

void OrderInsertPanel::OnCheckWDShowClicked(wxCommandEvent& event)
{

	Scroll(0, 0);

	UIENTITY* pUIArr = NULL;

	if(DEFAULT_SVR()==NULL)
		return;
	if(DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		switch(m_nViewSubStyle) {
		case conSubStyle_Std:
			pUIArr = s_tStdView;
			break;
		case conSubStyle_Batch:
			pUIArr = s_tBatchView;
			break;
		case conSubStyle_Condition:
			pUIArr = s_tConditionView;
			break;
		case conSubStyle_Stop:
			pUIArr = s_tStopView;
			m_poOrderInputPanel->SetViewMode(4);
			break;
		case conSubStyle_Position:
			pUIArr = s_tPositionView;
			break;
		case conSubStyle_FAKFOK:
			pUIArr = s_tFAKFOKView;
			break;
		default:
			return;
			break;
		};
	}
	else {
		pUIArr = s_tPatsView;
	}

	int nWidth=0;

	if(!event.IsChecked()) {
		nWidth=pUIArr[conUI_WDList].width+pUIArr[conUI_WDList].left;
	}

	for(int i=0; i<conUI_MAX; i++) {
		((wxWindow*)pUIArr[i].pWin)->SetPosition(wxPoint(pUIArr[i].left-nWidth, pUIArr[i].top));
	}

    //long nStyle; 
    //nStyle = GetParent()->GetWindowStyle();
    //if((nStyle&wxPOPUP_WINDOW)==wxPOPUP_WINDOW)
    //{
    //    int OriWid,OriHei;
    //    int OriPox,OriPoy;
    //    GetParent()->GetSize(&OriWid,&OriHei);
    //    GetParent()->GetPosition(&OriPox,&OriPoy);
    //    if(event.IsChecked())
    //    {
    //        OriWid+=160;
    //        OriPox-=160;
    //        if(OriPox<0) OriPox=0;
    //    }
    //    else
    //    {
    //        OriWid-=160;
    //        OriPox+=160;
    //        if(OriWid<180) OriWid=180;
    //        RECT ScnRect;
    //        ::GetWindowRect(::GetDesktopWindow(),&ScnRect);
    //        if(OriPox+OriWid>ScnRect.right)
    //            OriPox=ScnRect.right-OriWid;
    //    }
    //    GetParent()->SetSize(OriPox,OriPoy,OriWid,OriHei);

    //}

	int nVirWidth = 0;
	switch(m_nViewSubStyle) {
	case conSubStyle_Std:		// Ƕ�����ʱ
	case conSubStyle_Batch:		// �����µ�ʱ
	case conSubStyle_Condition:		// ������ʱ
	case conSubStyle_Stop:		// ֹͣ��ʱ
		if(event.IsChecked())
			nVirWidth = pUIArr[conUI_WDList].width+pUIArr[conUI_StdInput].width+5;
		else
			nVirWidth = pUIArr[conUI_WDList].width+5;
		break;
	case conSubStyle_Position:		// ӯ��ʱ
		if(event.IsChecked())
			nVirWidth = pUIArr[conUI_WDList].width+pUIArr[conUI_StdInput].width+pUIArr[conUI_Position].width+5;
		else
			nVirWidth = pUIArr[conUI_StdInput].width+pUIArr[conUI_Position].width+5;
	};
	SetVirtualSize(wxSize(nVirWidth, pUIArr[conUI_WDList].height));
	SaveFivePriceList(event.IsChecked());

	wxCommandEvent evtnew(wxEVT_BUYSELL5_SHOW_CHANGE);
	evtnew.SetInt(event.IsChecked());
	evtnew.SetExtraLong(nVirWidth);
	GETTOPWINDOW()->AddPendingEvent(evtnew);

	this->Refresh();
}

void OrderInsertPanel::OnCheckWinTopAlwaysClicked(wxCommandEvent& event)
{
	//SetWindowStyle(this->GetWindowStyle() | wxSTAY_ON_TOP);
	////this->SetWindowStyle( this->GetWindowStyle() & (~wxSTAY_ON_TOP) );

	wxWindow* pParentWin = GetParent();
	wxCheckBox* pCheckBox = (wxCheckBox*)FindWindowById(ID_CHECK_WINDOWTOPALWAYS,this);
	if(pParentWin) {
		if(pCheckBox->GetValue()) {
			SetWindowPos((HWND)pParentWin->GetHWND(), HWND_TOPMOST,
					0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); 
		}
		else {
			SetWindowPos((HWND)pParentWin->GetHWND(), HWND_NOTOPMOST,
					0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); 
		}
	}
}

void OrderInsertPanel::OnBtnBatchOrder(wxCommandEvent& event)
{
	USERLOG_INFO("�û������µ���[��������ť]\n%s%s", 
			m_poOrderInputPanel->GetUserInputText().c_str(), 
			m_poOrderBatchPanel->GetUserInputText().c_str());
	
	wxString strName=GetInputInstrumentID();

		set<string> InstrumentIDArray;
		DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
		if(InstrumentIDArray.find(strName.c_str())==InstrumentIDArray.end()) {
			ShowTradeInfoDlg("OIP_ERROR","OIP_CONTRERR",TRUE);
			return;
		}

	if(GetInputQty()<=0) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_QTYBIGZERO",TRUE);
		return;
	}
	if(m_poOrderBatchPanel->GetPriceType()==-1) {
		ShowTradeInfoDlg("OIP_ERROR","SELECT_BATCH_MODE",TRUE);
		return;
	}
	if(m_poOrderBatchPanel->GetBatchEachNum()<=0) {
		ShowTradeInfoDlg("OIP_ERROR","BATCH_QTY_BIG_ZERO",TRUE);
		return;
	}
	if(m_poOrderBatchPanel->GetTimerInterval()<=0) {
		ShowTradeInfoDlg("OIP_ERROR","BATCH_TIME_BIG_ZERO",TRUE);
		return;
	}

	double fPrice = GetInputPrice();
	if(CheckSumPrice(fPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_NORMAL),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}

	if(GetInputQty() > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITQTY), LOADSTRING(OIP_FASTTRADER), 
				wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES)
			return;
	}

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// ����ط���Ҫ���в�ѯ�Ż��������ԼΪ�ӷ�����������ϣ���δ�����������ϣ���δ���
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strName.c_str(),field);

	BOOL bIsPriceLimit = GetInputPrice()>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
			|| GetInputPrice()<field.LastPrice - m_currPriceTick*GetRaskDifferPrice();
	bIsPriceLimit = bIsPriceLimit && GetRaskDifferPrice();
	if(bIsPriceLimit) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITPRICE),LOADSTRING(OIP_FASTTRADER), 
				wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES) 
			return;
	}

	// �����ǵ�ͣ����ʾ
	if(!util::isInvalidValue(field.UpperLimitPrice) && !util::isInvalidValue(field.LowerLimitPrice)
			&& stricmp(GetInputPriceText().c_str(), LOADSTRING(OLS_MARKET))!=0) 
	{
		bool bOverLimit = util::greater(GetInputPrice(), field.UpperLimitPrice) 
			|| util::less(GetInputPrice(), field.LowerLimitPrice);
		if(bOverLimit) {
			wxMessageDialog dialog(NULL, LOADSTRING(OIP_OVERLIMITPRICE_UL), LOADSTRING(OIP_FASTTRADER), 
					wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			if(dialog.ShowModal()!=wxID_YES) 
				return;
		}
	}

	PlatformStru_InputOrder req={0};
//	m_poOrderInputPanel->FillOrder(req);

	BOOL bIsMarketPrice = FALSE;
	fPrice = 0.0;
	string strPriceText = m_poOrderInputPanel->GetPriceText().c_str();
	string strstlName = strName.c_str();
	BOOL bIsBuy = m_poOrderInputPanel->GetDirection();

	//Macro
	AUTOMARKETPRICE(strstlName, bIsBuy, strPriceText, bIsMarketPrice, fPrice)
	
	MakeReq_Order(req, strstlName, 
			m_poOrderInputPanel->GetAccount().c_str(), OrderType_FT_Immediately, 
			bIsBuy, m_poOrderInputPanel->GetOpenClose(), 
			m_poOrderInputPanel->GetQty(), fPrice, 0, m_poOrderInputPanel->GetHedge(), 
			bIsMarketPrice, FALSE);
	
//	req.CombOffsetFlag[0]=THOST_FTDC_OF_Open;

	wxString strMsg;
	strMsg.Printf(LOADFORMATSTRING(BATCH_PRCQTY_FORMAT,"%s%s%s%s%d%s%d%d%s"),//�ı��������%s %s %s %s %d %s %d %d  %s 
				Price2String(req.LimitPrice, m_currPriceTick).c_str(),
				req.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY) :LOADSTRING(OLS_SELL),
				req.InstrumentID, 
				req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ?LOADSTRING(OLS_OPEN): 
				(req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)),
				req.VolumeTotalOriginal,
				req.CombHedgeFlag[0]==THOST_FTDC_HF_Hedge ? LOADSTRING(OLS_HEDGE) : LOADSTRING(OLS_SPECULATE),
				m_poOrderBatchPanel->GetTimerInterval(), 
				m_poOrderBatchPanel->GetBatchEachNum(), 
				m_poOrderBatchPanel->GetPriceType()==0 ? LOADSTRING(OLS_INPUTPRICE) : 
				(m_poOrderBatchPanel->GetPriceType()==1 ? LOADSTRING(OLS_LASTPRICE3) : LOADSTRING(OLS_B_S_PRICE3)));
	BOOL bOK = TRUE;
	if(NeedConfirm()) {
        wxString strText;
        strText.Printf("%s\n", LOADSTRING(MARKETTYPE_CONFIRM_MESSAGE));
        if(bIsMarketPrice)
        {
            strText += strMsg;
            strMsg = strText;
        }
		wxMessageDialog dialog( NULL,strMsg ,LOADSTRING(OIP_BATCHCONF), wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
		bOK = dialog.ShowModal()==wxID_YES;
		USERLOG_INFO("��������ʾѡ�񣬱���[%s]��\n�û�ѡ��[%s]",dialog.GetTitle(),bOK?"��":"��");
	}
	if(bOK) {
		COrderBatchDailog* pDailog = NULL;
		pDailog = new COrderBatchDailog(this, wxID_ANY, LOADSTRING(SEND_BACTH_ORDER), wxDefaultPosition, wxSize(310, 250));
		pDailog->SetLocalOrderService(m_poLocalOrderService);
		pDailog->SetPriceType(m_poOrderBatchPanel->GetPriceType());
		pDailog->SetOrderReq(req);
		pDailog->SetHedge(GetInputHedge());
		pDailog->SetBatchEachNum(m_poOrderBatchPanel->GetBatchEachNum());
		pDailog->SetTimer(m_poOrderBatchPanel->GetTimerInterval());
		pDailog->Show();
		if(GetInputAutoTrackPrice()) {
			SetInputAutoTrackPrice(NeedContinueTrackPrice());
		}
	}
}

void OrderInsertPanel::OnBtnConditionOrder(wxCommandEvent& event)
{
	USERLOG_INFO("�û������µ���[��������ť]\n%s%s", 
			m_poOrderInputPanel->GetUserInputText().c_str(), 
			m_poOrderConditionPanel->GetUserInputText().c_str());
	
	wxString strName=GetInputInstrumentID();

		set<string> InstrumentIDArray;
		DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
		if(InstrumentIDArray.find(strName.c_str())==InstrumentIDArray.end()) {
			ShowTradeInfoDlg("OIP_ERROR","OIP_CONTRERR",TRUE);
			return;
		}

	if(GetInputQty()<=0) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_QTYBIGZERO",TRUE);
		return;
	}
	if(m_poOrderConditionPanel->GetPriceType()==-1) {
		ShowTradeInfoDlg("OIP_ERROR","SELECT_CONDITION_MODE",TRUE);
		return;
	}
	if(m_poOrderConditionPanel->GetConditionType()<=0) {
		ShowTradeInfoDlg("OIP_ERROR","SELECT_CONDI_OPERATOR",TRUE);
		return;
	}
	//if(m_poOrderConditionPanel->GetConditionPrice()<=0) {
	//	ShowTradeInfoDlg("OIP_ERROR","OIP_CONDITIONPRCNOT0",TRUE);
	//	return;
	//}

	double fPrice = GetInputPrice();
	if(CheckSumPrice(fPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_NORMAL),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		//SetInputPrice(fPrice);
		dialog.ShowModal();
		return;
	}
	fPrice = m_poOrderConditionPanel->GetConditionPrice();
	if(CheckSumPrice(fPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_CONDITION),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		//SetInputPrice(fPrice);
		dialog.ShowModal();
		return;
	}

	if(GetInputQty() > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITQTY),LOADSTRING(OIP_FASTTRADER),wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES)
			return;
	}

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// ����ط���Ҫ���в�ѯ�Ż��������ԼΪ�ӷ�����������ϣ���δ�����������ϣ���δ���
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strName.c_str(),field);

	BOOL bIsPriceLimit = GetInputPrice()>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
			|| GetInputPrice()<field.LastPrice - m_currPriceTick*GetRaskDifferPrice();
	bIsPriceLimit = bIsPriceLimit && GetRaskDifferPrice();
	if(bIsPriceLimit) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITPRICE),LOADSTRING(OIP_FASTTRADER), 
				wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES) 
			return;
	}

	// �����ǵ�ͣ����ʾ
	if(!util::isInvalidValue(field.UpperLimitPrice) && !util::isInvalidValue(field.LowerLimitPrice)
			&& stricmp(GetInputPriceText().c_str(), LOADSTRING(OLS_MARKET))!=0) 
	{
		bool bOverLimit = util::greater(GetInputPrice(), field.UpperLimitPrice) 
			|| util::less(GetInputPrice(), field.LowerLimitPrice);
		if(bOverLimit) {
			wxMessageDialog dialog(NULL, LOADSTRING(OIP_OVERLIMITPRICE_UL), LOADSTRING(OIP_FASTTRADER), 
					wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			if(dialog.ShowModal()!=wxID_YES) 
				return;
		}
	}

	CLocalOrderService::CONDITIONORDER tOrder={0};
	PlatformStru_InputOrder req;

	ZeroMemory(&tOrder, sizeof(tOrder));
	ZeroMemory(&req, sizeof(req));
	FillInputOrderField(req);

	tOrder.nState = conCondActive;
	tOrder.nCreateTime = ::wxGetUTCTime();
	strncpy(tOrder.InstrumentID, GetInputInstrumentID().c_str(), 
			sizeof(tOrder.InstrumentID)-1);

	tOrder.cond.nPriceType = m_poOrderConditionPanel->GetPriceType();
	tOrder.cond.nConditionType = m_poOrderConditionPanel->GetConditionType();
	tOrder.cond.fConditionPrice = m_poOrderConditionPanel->GetConditionPrice();
	
	strncpy(tOrder.orderSrc.InstrumentID, GetInputInstrumentID().c_str(), 
			sizeof(tOrder.orderSrc.InstrumentID)-1);
	tOrder.orderSrc.nOrderType = OrderType_FT_Immediately;
	tOrder.orderSrc.nOCMode = GetInputOCMode();
	tOrder.orderSrc.bIsBuy = GetInputDirection();
	tOrder.orderSrc.bIsMarket = stricmp(GetInputPriceText().c_str(), LOADSTRING(OLS_MARKET))==0;
	tOrder.orderSrc.fPrice = GetInputPrice();
	tOrder.orderSrc.nVolume = GetInputQty();
	tOrder.orderSrc.nHedgeStatus = 0;

	wxString strMsg;
	double fInputPrice=0.0;
	BOOL bIsSuccess = FALSE;

	switch(tOrder.cond.nPriceType) {
	case 1:
		fInputPrice = field.LastPrice;
		break;
	case 2:
		fInputPrice = field.BidPrice1;
		break;
	case 3:
		fInputPrice = field.AskPrice1;
		break;
	};
	if(!util::isInvalidValue(fInputPrice)) 
    {
        if( (tOrder.cond.nConditionType==1 && util::compare(fInputPrice,tOrder.cond.fConditionPrice)<=0)
				|| (tOrder.cond.nConditionType==2 && util::compare(fInputPrice,tOrder.cond.fConditionPrice)>=0)
				|| (tOrder.cond.nConditionType==3 && util::compare(fInputPrice,tOrder.cond.fConditionPrice)<0)
				|| (tOrder.cond.nConditionType==4 && util::compare(fInputPrice,tOrder.cond.fConditionPrice)>0))
			bIsSuccess = TRUE;
		//if( (tOrder.cond.nConditionType==1 && fInputPrice <= tOrder.cond.fConditionPrice)
		//		|| (tOrder.cond.nConditionType==2 && fInputPrice >= tOrder.cond.fConditionPrice)
		//		|| (tOrder.cond.nConditionType==3 && fInputPrice < tOrder.cond.fConditionPrice)
		//		|| (tOrder.cond.nConditionType==4 && fInputPrice > tOrder.cond.fConditionPrice))
		//	bIsSuccess = TRUE;
	}

	BOOL bOK = TRUE;
	if(bIsSuccess) {
        wxString strText;
        if(GetInputPriceText().compare(LOADSTRING(OLS_MARKET))==0)
        {
            strText.Printf("%s\n%s", LOADSTRING(MARKETTYPE_CONFIRM_MESSAGE), LOADSTRING(OIP_CONDITIONMEETCONF));
        }
        else
        {
            strText.Printf("%s", LOADSTRING(OIP_CONDITIONMEETCONF));
        }
        wxMessageDialog dialog(NULL, strText, LOADSTRING(OIP_CONDITIONCONF), wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
		bOK = dialog.ShowModal()==wxID_YES;
		USERLOG_INFO("�����������µ���ʾѡ�񣬱���[%s]��\n�û�ѡ��[%s]\n",LOADSTRING(OIP_CONDITIONCONF),bOK?"��":"��");
		if(bOK) {
 			int ret = VerifyOrder(req);
			if(ret) {
				DataRspOrderInsert tRspReq;
				//tRspReq.InputOrderField = *((CThostFtdcInputOrderField*)(&req));
				strcpy(tRspReq.InputOrderField.InstrumentID, req.InstrumentID);
				tRspReq.InputOrderField.Direction = req.Direction;
				tRspReq.InputOrderField.CombOffsetFlag[0] = req.CombOffsetFlag[0];
				tRspReq.InputOrderField.CombHedgeFlag[0] = req.CombHedgeFlag[0];
				tRspReq.InputOrderField.LimitPrice = req.LimitPrice;
				tRspReq.InputOrderField.StopPrice = req.StopPrice;
				tRspReq.InputOrderField.VolumeTotalOriginal = req.VolumeTotalOriginal;				//���ô�����Ϣ
				strcpy(tRspReq.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(ret,SVR_LANGUAGE).c_str());
				ShowTradeInfoDlg("OIP_SOF", tRspReq, NeedOrderFailConfirm());
			}
		}
		return;
	}

	wxString strCondDir;
	switch(tOrder.cond.nConditionType) {
	case 1:
		strCondDir = wxT("<=");
		break;
	case 2: 
		strCondDir = wxT(">=");
		break;
	case 3:
		strCondDir = wxT("<");
		break;
	case 4: 
		strCondDir = wxT(">");
		break;
	};
	strMsg.Printf(LOADFORMATSTRING(OIP_EXECONPRC_FORMAT,"%s%s%s%s%s%s%s%s%d"), //�ı��������%s%s%s%s %s%s%s%s %d
				m_poOrderConditionPanel->GetIsRunInServer() ? LOADSTRING(OIP_SERVER) : LOADSTRING(OIP_LOCAL), 
				tOrder.InstrumentID, 
				tOrder.cond.nPriceType==2 ? LOADSTRING(OIP_BIDPRICE) : (tOrder.cond.nPriceType==3 ? LOADSTRING(OIP_ASKPRICE) : LOADSTRING(OIP_LASTPRICE)), 
				strCondDir, 
				Price2String(tOrder.cond.fConditionPrice, m_currPriceTick).c_str(), 
				Price2String(tOrder.orderSrc.fPrice, m_currPriceTick).c_str(), 
				tOrder.orderSrc.bIsBuy ? LOADSTRING(OLS_BUY):LOADSTRING(OLS_SELL),
				tOrder.orderSrc.nOCMode==0 ? LOADSTRING(OLS_OPEN): (
				tOrder.orderSrc.nOCMode==1 ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)), 
				tOrder.orderSrc.nVolume);
	if(NeedConfirm()) {
        wxString strText;
        strText.Printf("%s\n", LOADSTRING(MARKETTYPE_CONFIRM_MESSAGE));
        if(GetInputPriceText().compare(LOADSTRING(OLS_MARKET))==0)
        {
            strText += strMsg;
            strMsg = strText;
        }
		wxMessageDialog dialog( NULL,strMsg ,LOADSTRING(OIP_CONDITIONCONF), wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
		bOK = dialog.ShowModal()==wxID_YES;
		USERLOG_INFO("������������ʾѡ�񣬱���[%s]��\n�û�ѡ��[%s]\n",LOADSTRING(OIP_CONDITIONCONF),bOK?"��":"��");
	}
	if(bOK) {
		if(m_poOrderConditionPanel->GetIsRunInServer()) {
			req.StopPrice = tOrder.cond.fConditionPrice;
			switch(tOrder.cond.nPriceType) {
			case 1:	// 1 ����
				switch(tOrder.cond.nConditionType) {
				case 1:		// 1 С�ڵ���ָ���۸�
					req.ContingentCondition = THOST_FTDC_CC_LastPriceLesserEqualStopPrice;
					break;
				case 2:		// 2 ���ڵ���ָ���۸�
					req.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterEqualStopPrice;
					break;
				case 3:		// 3 С��
					req.ContingentCondition = THOST_FTDC_CC_LastPriceLesserThanStopPrice;
					break;
				case 4:		// 4 ����
					req.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterThanStopPrice;
					break;
				};
				break;
			case 2:	// 2 ���
				switch(tOrder.cond.nConditionType) {
				case 1:		// 1 С�ڵ���ָ���۸�
					req.ContingentCondition = THOST_FTDC_CC_BidPriceLesserEqualStopPrice;
					break;
				case 2:		// 2 ���ڵ���ָ���۸�
					req.ContingentCondition = THOST_FTDC_CC_BidPriceGreaterEqualStopPrice;
					break;
				case 3:		// 3 С��
					req.ContingentCondition = THOST_FTDC_CC_BidPriceLesserThanStopPrice;
					break;
				case 4:		// 4 ����
					req.ContingentCondition = THOST_FTDC_CC_BidPriceGreaterThanStopPrice;
					break;
				};
				break;
			case 3: // 3 ����
				switch(tOrder.cond.nConditionType) {
				case 1:		// 1 С�ڵ���ָ���۸�
					req.ContingentCondition = THOST_FTDC_CC_AskPriceLesserEqualStopPrice;
					break;
				case 2:		// 2 ���ڵ���ָ���۸�
					req.ContingentCondition = THOST_FTDC_CC_AskPriceGreaterEqualStopPrice;
					break;
				case 3:		// 3 С��
					req.ContingentCondition = THOST_FTDC_CC_AskPriceLesserThanStopPrice;
					break;
				case 4:		// 4 ����
					req.ContingentCondition = THOST_FTDC_CC_AskPriceGreaterThanStopPrice;
					break;
				};
				break;
			};
			
			int nRet = -1;
			wxString strRetMsg="";
 			nRet = VerifyOrder(req);
			if(GetInputAutoTrackPrice()) {
				SetInputAutoTrackPrice(NeedContinueTrackPrice());
			}
			if(nRet!=0) {
				//COrderServiceThread::sm_nReqNum--;
				DataRspOrderInsert tInsertOrder;
				ZeroMemory(&tInsertOrder, sizeof(DataRspOrderInsert));
				//tInsertOrder.InputOrderField = *(CThostFtdcInputOrderField*)(&req);
				strcpy(tInsertOrder.InputOrderField.InstrumentID, req.InstrumentID);
				tInsertOrder.InputOrderField.Direction = req.Direction;
				tInsertOrder.InputOrderField.CombOffsetFlag[0] = req.CombOffsetFlag[0];
				tInsertOrder.InputOrderField.CombHedgeFlag[0] = req.CombHedgeFlag[0];
				tInsertOrder.InputOrderField.LimitPrice = req.LimitPrice;
				tInsertOrder.InputOrderField.StopPrice = req.StopPrice;
				tInsertOrder.InputOrderField.VolumeTotalOriginal = req.VolumeTotalOriginal;				//���ô�����Ϣ
				strcpy(tInsertOrder.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
				ShowTradeInfoDlg("OIP_SOF", tInsertOrder, NeedOrderFailConfirm());
			}
		}
		else {
			if(m_poLocalOrderService) {
				m_poLocalOrderService->OrderConditionAdd(tOrder);

				if(GetInputAutoTrackPrice()) {
					SetInputAutoTrackPrice(NeedContinueTrackPrice());
				}
				wxCommandEvent evtnew(wxEVT_QUERY_CONDITION_REQUERY, GetId());
				GETTOPWINDOW()->AddPendingEvent(evtnew);
			}
		}
	}

}

void OrderInsertPanel::OnBtnPositionOrder(wxCommandEvent& event)
{
	USERLOG_INFO("�û������µ���[ӯ�𵥰�ť]\n%s%s", 
			m_poOrderInputPanel->GetUserInputText().c_str(), 
			m_poOrderPositionPanel->GetUserInputText().c_str());
	
	wxString strName=GetInputInstrumentID();

		set<string> InstrumentIDArray;
		DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
		if(InstrumentIDArray.find(strName.c_str())==InstrumentIDArray.end()) {
			ShowTradeInfoDlg("OIP_ERROR","OIP_CONTRERR", TRUE);
			return;
		}

	PlatformStru_InstrumentInfo instrInfo;
	DEFAULT_SVR()->GetInstrumentInfo(std::string(strName.c_str()), instrInfo);
	if(instrInfo.ProductClass==THOST_FTDC_PC_Combination) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_COMBNOSUPPORT", TRUE);
		return;
	}

	if(GetInputQty()<=0) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_QTYBIGZERO", TRUE);
		return;
	}

	double fPrice = GetInputPrice();
	if(CheckSumPrice(fPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_NORMAL),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		//SetInputPrice(fPrice);
		dialog.ShowModal();
		return;
	}

	int nOffsetPriceObject = 0;

	nOffsetPriceObject = m_poOrderPositionPanel->GetOffsetPriceObject();

	BOOL bIsCheckLose = FALSE, bIsCheckGain = FALSE;
	double fLoseOffsetPrice = 0.0f, fGainOffsetPrice = 0.0f;

	bIsCheckLose = m_poOrderPositionPanel->GetLoseOffsetPrice(fLoseOffsetPrice);
	bIsCheckGain = m_poOrderPositionPanel->GetGainOffsetPrice(fGainOffsetPrice);

	if(bIsCheckLose && CheckSumPrice(fLoseOffsetPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_LOSE),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}
	if(bIsCheckGain && CheckSumPrice(fGainOffsetPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_GAIN),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}
	
	int nCloseMode = 0;
	double fCloseOffsetPrice = 0.0;

	nCloseMode = m_poOrderPositionPanel->GetCloseMode();
	fCloseOffsetPrice = m_poOrderPositionPanel->GetCloseOffsetPrice();
	if(CheckSumPrice(fCloseOffsetPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_CLOSEREVERSE),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}


	if(bIsCheckLose == FALSE && bIsCheckGain == FALSE) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_SOPPO", TRUE);
		return;
	}
	if(bIsCheckLose != FALSE && fLoseOffsetPrice <= 0.0f) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_STEOPPRCBIGZERO", TRUE);
		return;
	}
	if(bIsCheckGain != FALSE && fGainOffsetPrice <= 0.0f) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_PRCDELTABIGZERO", TRUE);
		return;
	}

	if(GetInputQty() > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITQTY), LOADSTRING(OIP_FASTTRADER), 
			wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES)
			return;
	}

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// ����ط���Ҫ���в�ѯ�Ż��������ԼΪ�ӷ�����������ϣ���δ�����������ϣ���δ���
	if(DEFAULT_SVR()) 
		DEFAULT_SVR()->GetQuotInfo(strName.c_str(),field);

	BOOL bIsPriceLimit = GetInputPrice()>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
			|| GetInputPrice()<field.LastPrice - m_currPriceTick*GetRaskDifferPrice();
	bIsPriceLimit = bIsPriceLimit && GetRaskDifferPrice();
	if(bIsPriceLimit) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITPRICE), LOADSTRING(OIP_FASTTRADER), 
			wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES) 
			return;

	}

	// �����ǵ�ͣ����ʾ
	if(!util::isInvalidValue(field.UpperLimitPrice) && !util::isInvalidValue(field.LowerLimitPrice)
			&& stricmp(GetInputPriceText().c_str(), LOADSTRING(OLS_MARKET))!=0) 
	{
		bool bOverLimit = util::greater(GetInputPrice(), field.UpperLimitPrice) 
			|| util::less(GetInputPrice(), field.LowerLimitPrice);
		if(bOverLimit) {
			wxMessageDialog dialog(NULL, LOADSTRING(OIP_OVERLIMITPRICE_UL), LOADSTRING(OIP_FASTTRADER), 
					wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			if(dialog.ShowModal()!=wxID_YES) 
				return;
		}
	}

	int nConditionPriceType = 0;
	nConditionPriceType = m_poOrderPositionPanel->GetPriceType();

	PlatformStru_InputOrder req={0};
	ZeroMemory(&req, sizeof(req));

	int ret=0;
    
	// ���Ȱѽ�����������룬Ȼ���ٽ����Զ�ƽ�ִ���
	FillInputOrderField(req);
	req.CombOffsetFlag[0]=THOST_FTDC_OF_Open;

	wxString strText;
	strText.Printf(LOADFORMATSTRING(OIP_LRTSFC,"%f"), fCloseOffsetPrice);//�ı��������%f

	wxString strMsg;
	BOOL bOK = TRUE;
	if(NeedConfirm()) {
		int nOpenClose = 0;
		if(req.CombOffsetFlag[0]==THOST_FTDC_OF_Open)
			nOpenClose=0;
		else if(req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday)
			nOpenClose=1;
		else
			nOpenClose=2;
		//	strAutoOpenClose,
		strMsg.Printf(LOADFORMATSTRING(COPQSOC_FOMRAT,"%s%s%s%s%s%d%f%f%s"),//�ı��������%s %s��%s��%s %s %d %0.2f %0.2f %s
				strName.Trim(),
				GetInputHedge() ? LOADSTRING(OLS_HEDGE):LOADSTRING(OLS_SPECULATE),
				req.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY):LOADSTRING(OLS_SELL),
				req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(OLS_OPEN): 
				(req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY): LOADSTRING(OLS_CLOSE)),
				Price2String(req.LimitPrice, m_currPriceTick).c_str(), 
				req.VolumeTotalOriginal,
				fLoseOffsetPrice, 
				fGainOffsetPrice, 
				nCloseMode==1 ? LOADSTRING(OLS_PRICE_LIMIT) : strText);		//wxString strLose, strGain;
		//strLose.Printf("ֹ��۲%s", Price2String(fLoseOffsetPrice, m_currPriceTick).c_str());
		//strGain.Printf("ֹӯ�۲%s", Price2String(fGainOffsetPrice, m_currPriceTick).c_str());
		//strMsg.Printf(LOADFORMATSTRING(COPQSOC_FOMRAT,"%s%s%s%s%s%d%f%f%s"),//�ı��������%s %s��%s��%s %s %d %0.2f %0.2f %s
		//		strName.Trim(),
		//		GetInputHedge() ? LOADSTRING(OLS_HEDGE):LOADSTRING(OLS_SPECULATE),
		//		req.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY):LOADSTRING(OLS_SELL),
		//		req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(OLS_OPEN): 
		//		(req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY): LOADSTRING(OLS_CLOSE)),
		//		Price2String(req.LimitPrice, m_currPriceTick).c_str(), 
		//		req.VolumeTotalOriginal,
		//		bIsCheckLose?strLose.c_str():"", bIsCheckGain?strGain.c_str():"", 
		//		nCloseMode==1 ? LOADSTRING(OLS_PRICE_LIMIT) : strText);
        wxString strText;
        strText.Printf("%s\n", LOADSTRING(MARKETTYPE_CONFIRM_MESSAGE));
        if(GetInputPriceText().compare(LOADSTRING(OLS_MARKET))==0)
        {
            strText += strMsg;
            strMsg = strText;
        }
		wxMessageDialog dialog( NULL,strMsg ,LOADSTRING(OIP_PLCONF),wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
		bOK = dialog.ShowModal()==wxID_YES;
		USERLOG_INFO("ӯ����ʾѡ�񣬱���[%s]��\n�û�ѡ��[%s]\n",LOADSTRING(OIP_PLCONF),bOK?"��":"��");
	}
	if(bOK) {
 		ret = VerifyOrder(req);
		if(GetInputAutoTrackPrice()) {
			SetInputAutoTrackPrice(NeedContinueTrackPrice());
		}
		if(ret) {
			//COrderServiceThread::sm_nReqNum--;
			DataRspOrderInsert tmpData;
			ZeroMemory(&tmpData, sizeof(DataRspOrderInsert));
			tmpData.InputOrderField = *(CThostFtdcInputOrderField*)(&req);

			//���ô�����Ϣ
			strcpy(tmpData.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(ret,SVR_LANGUAGE).c_str());
			//if(NeedOrderFailConfirm())
			ShowTradeInfoDlg("OIP_SOF", tmpData, NeedOrderFailConfirm());
			return;
		}
	}

	wxString strCaption;
	if(bOK && ret==0) {
		m_bWaitingRsp_OrderInsert=true;

		PlatformStru_DepthMarketData tInstrfield;
        memset(&tInstrfield,0,sizeof(tInstrfield));
		// �ӷ��������¼۸�
		if(DEFAULT_SVR()) 
			DEFAULT_SVR()->GetQuotInfo(strName.c_str(), tInstrfield);

		CLocalOrderService::PLORDER tPosOrder={0};
		ZeroMemory(&tPosOrder, sizeof(tPosOrder));

		tPosOrder.bIsRunInServer = m_poOrderPositionPanel->GetIsRunInServer();
		tPosOrder.nState = conPLActive;
		strcpy(tPosOrder.InstrumentID, req.InstrumentID);

		tPosOrder.ref.RequestID = req.RequestID;

		tPosOrder.plparam.nPriceOffsetType = nOffsetPriceObject;
		tPosOrder.plparam.bDoStopLose = bIsCheckLose;
		tPosOrder.plparam.fStopLosePriceOffset = bIsCheckLose ? fLoseOffsetPrice : 0.0;
		tPosOrder.plparam.bDoStopGain = bIsCheckGain;
		tPosOrder.plparam.fStopGainPriceOffset = bIsCheckGain ? fGainOffsetPrice : 0.0;
		tPosOrder.plparam.nPriceType = nConditionPriceType;
		tPosOrder.plparam.nCloseMode = nCloseMode;
		tPosOrder.plparam.fClosePriceOffset = fCloseOffsetPrice;

		strncpy(tPosOrder.orderSrc.InstrumentID, GetInputInstrumentID().c_str(), 
				sizeof(tPosOrder.orderSrc.InstrumentID)-1);
		tPosOrder.orderSrc.nOrderType = OrderType_FT_Immediately;
		tPosOrder.orderSrc.nOCMode = GetInputOCMode();
		tPosOrder.orderSrc.bIsBuy = GetInputDirection();
		tPosOrder.orderSrc.bIsMarket = stricmp(GetInputPriceText().c_str(), LOADSTRING(OLS_MARKET))==0;
		tPosOrder.orderSrc.fPrice = GetInputPrice();
		tPosOrder.orderSrc.nVolume = GetInputQty();
		tPosOrder.orderSrc.nHedgeStatus = 0;

		m_poLocalOrderService->OrderPLAdd(tPosOrder);

		wxCommandEvent evtnew(wxEVT_QUERY_CONDITION_REQUERY, GetId());
		GETTOPWINDOW()->AddPendingEvent(evtnew);

		wxCommandEvent evtnew2(wxEVT_QUERY_STOP_REQUERY, GetId());
		GETTOPWINDOW()->AddPendingEvent(evtnew2);

	}

	if(bOK)
	{
		SetStopPositionSelect(FALSE);
		m_poOrderPositionPanel->InitPositionPanel();

		wxCommandEvent evt(wxEVT_COMMAND_CHECKBOX_CLICKED, ID_DOSTOPPOSITION_CHECK);
		evt.SetInt(0);
		::wxPostEvent(this,evt);
	}
}

void OrderInsertPanel::OnBtnStopOrder(wxCommandEvent& event)
{
	
	USERLOG_INFO("�û������µ���[ֹͣ����ť]\n%s%s", 
			m_poOrderInputPanel->GetUserInputText().c_str(), 
			m_poOrderStopPanel->GetUserInputText().c_str());

	wxString strName=GetInputInstrumentID();

		set<string> InstrumentIDArray;
		DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
		if(InstrumentIDArray.find(strName.c_str())==InstrumentIDArray.end()) {
			ShowTradeInfoDlg("OIP_ERROR","OIP_CONTRERR", TRUE);
			return;
		}

	if(GetInputQty()<=0) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_QTYBIGZERO", TRUE);
		return;
	}

	double fPrice = GetInputPrice();
	if(CheckSumPrice(fPrice, m_currPriceTick)) {
		wxMessageDialog dialog(NULL,LOADSTRING(CHECKSUM_PRICE_NORMAL),
				LOADSTRING(OIP_ERROR),wxNO_DEFAULT|wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}

	if(m_poOrderStopPanel->GetStopType()==-1) {
		ShowTradeInfoDlg("OIP_ERROR","SELECT_STOP_MODE", TRUE);
		return;
	}

	if(GetInputQty() > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITQTY),LOADSTRING(OIP_FASTTRADER),wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES)
			return;
	}

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// ����ط���Ҫ���в�ѯ�Ż��������ԼΪ�ӷ�����������ϣ���δ�����������ϣ���δ���
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strName.c_str(),field);

	BOOL bIsPriceLimit = GetInputPrice()>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
			|| GetInputPrice()<field.LastPrice - m_currPriceTick*GetRaskDifferPrice();
	bIsPriceLimit = bIsPriceLimit && GetRaskDifferPrice();
	if(bIsPriceLimit) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITPRICE),LOADSTRING(OIP_FASTTRADER), 
				wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES) 
			return;
	}

	// �����ǵ�ͣ����ʾ
	if(!util::isInvalidValue(field.UpperLimitPrice) && !util::isInvalidValue(field.LowerLimitPrice)
			&& stricmp(GetInputPriceText().c_str(), LOADSTRING(OLS_MARKET))!=0) 
	{
		bool bOverLimit = util::greater(GetInputPrice(), field.UpperLimitPrice) 
			|| util::less(GetInputPrice(), field.LowerLimitPrice);
		if(bOverLimit) {
			wxMessageDialog dialog(NULL, LOADSTRING(OIP_OVERLIMITPRICE_UL), LOADSTRING(OIP_FASTTRADER), 
					wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			if(dialog.ShowModal()!=wxID_YES) 
				return;
		}
	}

	CLocalOrderService::CONDITIONORDER tOrder={0};
	PlatformStru_InputOrder req;
	ZeroMemory(&tOrder, sizeof(tOrder));
	ZeroMemory(&req, sizeof(PlatformStru_InputOrder));
	
	FillInputOrderField(req);
//	req.CombOffsetFlag[0]=THOST_FTDC_OF_Open;

	tOrder.nState = conCondActive;
	tOrder.nCreateTime = ::wxGetUTCTime();
	strncpy(tOrder.InstrumentID, GetInputInstrumentID().c_str(), 
			sizeof(tOrder.InstrumentID)-1);

	tOrder.cond.nPriceType = m_poOrderConditionPanel->GetPriceType();
	tOrder.cond.nConditionType = m_poOrderConditionPanel->GetConditionType();
	tOrder.cond.fConditionPrice = m_poOrderConditionPanel->GetConditionPrice();
	
	strncpy(tOrder.orderSrc.InstrumentID, GetInputInstrumentID().c_str(), 
			sizeof(tOrder.orderSrc.InstrumentID)-1);
	tOrder.orderSrc.nOrderType = OrderType_FT_Immediately;
	tOrder.orderSrc.nOCMode = GetInputOCMode();
	tOrder.orderSrc.bIsBuy = GetInputDirection();
	tOrder.orderSrc.bIsMarket = stricmp(GetInputPriceText().c_str(), LOADSTRING(OLS_MARKET))==0;
	tOrder.orderSrc.fPrice = GetInputPrice();
	tOrder.orderSrc.nVolume = GetInputQty();
	tOrder.orderSrc.nHedgeStatus = 0;

	wxString strMsg;
	double fInputPrice=0.0;
	BOOL bIsSuccess = FALSE;

	if(m_poOrderStopPanel->GetStopType()==1)
		strMsg.Printf(LOADFORMATSTRING(STOP_TYPE_FORMAT,"%s%s%s%s%s%d"), //�ı��������%s %s %s %s %s %d 
		m_poOrderStopPanel->GetStopType()==1 ? LOADSTRING(ORDER_STOP_TYPE) : LOADSTRING(ORDER_STOPLIMIT_TYPE), 
		tOrder.InstrumentID, 
		Price2String(tOrder.cond.fConditionPrice, m_currPriceTick).c_str(), 
		tOrder.orderSrc.bIsBuy ? LOADSTRING(OLS_BUY):LOADSTRING(OLS_SELL),
		tOrder.orderSrc.nOCMode==0 ? LOADSTRING(OLS_OPEN) : (
		tOrder.orderSrc.nOCMode==1 ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)), 
		tOrder.orderSrc.nVolume);
	else
		strMsg.Printf(LOADFORMATSTRING(STOPLIMIT_TYPE_FORMAT,"%s%s%s%s%s%s%d"), //�ı��������%s%s%s%s%s%s%d
		m_poOrderStopPanel->GetStopType()==1 ? LOADSTRING(ORDER_STOP_TYPE) : LOADSTRING(ORDER_STOPLIMIT_TYPE), 
		tOrder.InstrumentID, 
		Price2String(tOrder.cond.fConditionPrice, m_currPriceTick).c_str(), 
		Price2String(m_poOrderStopPanel->GetPrice(), m_currPriceTick).c_str(), 
		tOrder.orderSrc.bIsBuy ? LOADSTRING(OLS_BUY):LOADSTRING(OLS_SELL),
		tOrder.orderSrc.nOCMode==0 ? LOADSTRING(OLS_OPEN) : (
		tOrder.orderSrc.nOCMode==1 ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)), 
		tOrder.orderSrc.nVolume);

	BOOL bOK = TRUE;
	if(NeedConfirm()) {
        wxString strText;
        strText.Printf("%s\n", LOADSTRING(MARKETTYPE_CONFIRM_MESSAGE));
        strText += strMsg;
        strMsg = strText;
		wxMessageDialog dialog( NULL,strMsg ,LOADSTRING(OIP_STOPCONF), wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
		bOK = dialog.ShowModal()==wxID_YES;
		USERLOG_INFO("ֹͣ����ʾѡ�񣬱���[%s]��\n�û�ѡ��[%s]\n",dialog.GetTitle().c_str(),bOK?"��":"��");
	}
	if(bOK) {
		//if(m_poOrderConditionPanel->GetIsRunInServer()) {
		if(1) {
			if(m_poOrderStopPanel->GetStopType()==1) {
				req.OrderPriceType=THOST_FTDC_OPT_AnyPrice;
				req.LimitPrice=0.0f;
				req.TimeCondition=THOST_FTDC_TC_IOC;

				req.StopPrice = tOrder.cond.fConditionPrice;
				req.ContingentCondition = THOST_FTDC_CC_Touch;
			}
			else {
				req.LimitPrice = m_poOrderStopPanel->GetPrice();
				req.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
				req.TimeCondition=THOST_FTDC_TC_GFD;

				req.StopPrice = tOrder.cond.fConditionPrice;
				req.ContingentCondition = THOST_FTDC_CC_Touch;
			}
			
			int nRet = -1;
			wxString strRetMsg="";
 			nRet = VerifyOrder(req);
			if(GetInputAutoTrackPrice()) {
				SetInputAutoTrackPrice(NeedContinueTrackPrice());
			}
			if(nRet!=0) {
				//COrderServiceThread::sm_nReqNum--;
				DataRspOrderInsert tInsertOrder;
				ZeroMemory(&tInsertOrder, sizeof(DataRspOrderInsert));
				//tInsertOrder.InputOrderField = *(CThostFtdcInputOrderField*)(&req);
				strcpy(tInsertOrder.InputOrderField.InstrumentID, req.InstrumentID);
				tInsertOrder.InputOrderField.Direction = req.Direction;
				tInsertOrder.InputOrderField.CombOffsetFlag[0] = req.CombOffsetFlag[0];
				tInsertOrder.InputOrderField.CombHedgeFlag[0] = req.CombHedgeFlag[0];
				tInsertOrder.InputOrderField.LimitPrice = req.LimitPrice;
				tInsertOrder.InputOrderField.StopPrice = req.StopPrice;
				tInsertOrder.InputOrderField.VolumeTotalOriginal = req.VolumeTotalOriginal;				//���ô�����Ϣ
				strcpy(tInsertOrder.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
				ShowTradeInfoDlg("OIP_SOF", tInsertOrder, NeedOrderFailConfirm());
			}
		}
		else {
			if(m_poLocalOrderService) {
				m_poLocalOrderService->OrderConditionAdd(tOrder);

				wxCommandEvent evtnew(wxEVT_QUERY_CONDITION_REQUERY, GetId());
				GETTOPWINDOW()->AddPendingEvent(evtnew);
			}
		}
	}

}

void OrderInsertPanel::OnBtnPatsOrder(wxCommandEvent& event)
{
	
	USERLOG_INFO("�û������µ���[Pats�µ���ť]\n%s", 
			m_poPatsOrderInputPanel->GetUserInputText().c_str());

	if(DEFAULT_SVR()==NULL)
		return;

	wxString strName=GetInputInstrumentID();

		set<string> InstrumentIDArray;
		DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
		if(InstrumentIDArray.find(strName.c_str())==InstrumentIDArray.end()) {
			ShowTradeInfoDlg(LOADSTRING(OIP_ERROR),LOADSTRING(OIP_CONTRERR), TRUE);
			return;
		}

	if(GetInputQty()<=0) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_QTYBIGZERO",TRUE);
		return;
	}

	if(GetInputQty() > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITQTY),LOADSTRING(OIP_FASTTRADER),wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES)
			return;
	}

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// ����ط���Ҫ���в�ѯ�Ż��������ԼΪ�ӷ�����������ϣ���δ�����������ϣ���δ���
	if(DEFAULT_SVR()!=NULL)
		DEFAULT_SVR()->GetQuotInfo(strName.c_str(),field);

	BOOL bIsPriceLimit = GetInputPrice()>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
			|| GetInputPrice()<field.LastPrice - m_currPriceTick*GetRaskDifferPrice();
	bIsPriceLimit = bIsPriceLimit && GetRaskDifferPrice();
	if(bIsPriceLimit) {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITPRICE),LOADSTRING(OIP_FASTTRADER), 
				wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES) 
			return;
	}

	PlatformStru_InputOrder req;
	ZeroMemory(&req, sizeof(PlatformStru_InputOrder));
	
	FillInputOrderField(req);
//	req.CombOffsetFlag[0]=THOST_FTDC_OF_Open;

	wxString strMsg;
	double fInputPrice=0.0;
	BOOL bIsSuccess = FALSE;

	strMsg = Printf_StdOrderReq(req, LOADSTRING(Printf_AccountTitle), LOADSTRING(Printf_OrderTypeTitle), 
			LOADSTRING(Printf_InstrumentIDTitle), LOADSTRING(Printf_DirectionTitle), 
			LOADSTRING(Printf_OpenCloseTitle), LOADSTRING(Printf_QtyTitle), 
			LOADSTRING(Printf_PriceTitle), LOADSTRING(Printf_TriggerPriceTitle), 
			LOADSTRING(Printf_HedgeTitle), m_currPriceTick, TRUE/*PLATFORM_SVR().HaveOrderType(UIOT_ACCOUNT)*/,
			DEFAULT_SVR()->HaveOrderType(UIOT_ITEM_OPENCLOSE), DEFAULT_SVR()->HaveOrderType(UIOT_ITEM_HEDGE));

	BOOL bOK = TRUE;
	if(NeedConfirm()) {
		wxMessageDialog dialog( NULL,strMsg ,LOADSTRING(OIP_ORDERCONF), wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
		bOK = dialog.ShowModal()==wxID_YES;
		USERLOG_INFO("Pats�µ���ʾѡ�񣬱���[%s]��\n�û�ѡ��[%s]\n",dialog.GetTitle().c_str(),bOK?"��":"��");
	}
	if(bOK) {
		//if(m_poOrderConditionPanel->GetIsRunInServer()) {
		if(1) {			
			int nRet = -1;
			wxString strRetMsg="";
 			nRet = VerifyOrder(req);
			if(nRet!=0) {
				DataRspOrderInsert tInsertOrder;
				ZeroMemory(&tInsertOrder, sizeof(DataRspOrderInsert));
				tInsertOrder.InputOrderField = *(CThostFtdcInputOrderField*)(&req);
				//���ô�����Ϣ
				strcpy(tInsertOrder.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(nRet,DEFAULT_SVR()->GetServerParamCfg().LanguageId).c_str());
				ShowTradeInfoDlg(LOADSTRING(OIP_SOF), tInsertOrder, NeedOrderFailConfirm());
			}
		}
	}

}

void OrderInsertPanel::OnBtnFAKFOKOrder(wxCommandEvent& event)
{

	if(m_poOrderFAKFOKPanel == NULL)
		return;

	USERLOG_INFO("�û������µ���[FAK/FOK�µ���ť]\n%s\n%s", 
			m_poOrderInputPanel->GetUserInputText().c_str(),
			m_poOrderFAKFOKPanel->GetUserInputText().c_str());

	int nMinVolume = m_poOrderFAKFOKPanel->GetMinVolume();
	int nOrderType = m_poOrderFAKFOKPanel->GetType();
	if(nOrderType==0)
		nOrderType = OrderType_FT_ImmeFAK;
	else
		nOrderType = OrderType_FT_ImmeFOK;

	NormalOrder(nOrderType, nMinVolume);
	
}

void OrderInsertPanel::OnFivePriceSelected(wxCommandEvent& event)
{
    std::string EvtParamStr;
    if(1)
    {
        DWORD EvtParamID=(DWORD)event.GetInt();
        if(CFTEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        {
            CFTEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }
    wxString strText(EvtParamStr.c_str());
	SetInputPriceText(strText);
}

void OrderInsertPanel::OnCheckDoStopClicked(wxCommandEvent& event)
{
	
	// �����xml�ж��Ƿ�Ҫȷ�ϵ�����
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return;

	vector<VarietyInfo>* parrVarietyInfo;
	parrVarietyInfo = pMgr->GetVarietyInfoVector();
	VarietyInfo tvInfo;
	wxString strCode = GetInputInstrumentID();
	double fGainOffset = 0.0;
	double fLoseOffset = 0.0;
	double fReverseOffset = 0.0;
	BOOL bFound = FALSE;

	if(event.GetInt()) {
		for(int i=0; i<(int)parrVarietyInfo->size(); i++) {
			tvInfo = (*parrVarietyInfo)[i];
			wxString strValue = tvInfo.InstruName;
			if(strCode.compare(strValue.c_str()) == 0 
						|| strnicmp(strValue.c_str(), strCode.c_str(), strValue.size())==0) {
				fLoseOffset = atof(tvInfo.loss);
				fGainOffset = atof(tvInfo.profit);
				fReverseOffset = atof(tvInfo.reverse);
				bFound = TRUE;
				break;
			}
		}
	}
	if(bFound) {
		m_poOrderPositionPanel->SetLoseOffsetPrice(event.GetInt(), fLoseOffset*m_currPriceTick);
		m_poOrderPositionPanel->SetGainOffsetPrice(event.GetInt(), fGainOffset*m_currPriceTick);
		m_poOrderPositionPanel->SetCloseOffsetPrice(fReverseOffset*m_currPriceTick);
	}
	else {
		BOOL bIsCheckLose = FALSE, bIsCheckGain = FALSE;
		double fLoseOffsetPrice = 0.0, fGainOffsetPrice = 0.0;

		bIsCheckLose = m_poOrderPositionPanel->GetLoseOffsetPrice(fLoseOffsetPrice);
		bIsCheckGain = m_poOrderPositionPanel->GetGainOffsetPrice(fGainOffsetPrice);

		m_poOrderPositionPanel->SetLoseOffsetPrice(event.GetInt(), fLoseOffsetPrice);
		m_poOrderPositionPanel->SetGainOffsetPrice(event.GetInt(), fGainOffsetPrice);
		//m_poOrderPositionPanel->SetCloseOffsetPrice(0.0);
	}
	
	if(event.GetInt()) {
		for(int i=0; i<(int)m_TabOrderVector.size(); i++) {
			if(m_TabOrderVector[i]==ID_BUTTON_OK) {
				m_TabOrderVector[i]=ID_BUTTON_POSITIONORDER;
				break;
			}
		}
	}
	else {
		for(int i=0; i<(int)m_TabOrderVector.size(); i++) {
			if(m_TabOrderVector[i]==ID_BUTTON_POSITIONORDER) {
				m_TabOrderVector[i]=ID_BUTTON_OK;
				break;
			}
		}
	}
	
	m_poOrderInsertCommandPanel->ViewStyleSwitch_DoStop(event.GetInt());
	if(event.GetInt()) {
		m_poOrderInputPanel->SetViewMode(3);
	}
	else {
		m_poOrderInputPanel->SetViewMode(0);
	}
}

void OrderInsertPanel::OnMAPlatformAdd(wxCommandEvent& evt)
{
	wxCommandEvent myEvent(wxEVT_SUBSCRIBE);
	myEvent.SetInt(1);
	ProcessEvent(myEvent);
}
void OrderInsertPanel::OnMAPlatformDelete(wxCommandEvent& evt)
{
	wxCommandEvent myEvent(wxEVT_SUBSCRIBE);
	myEvent.SetInt(0);
	ProcessEvent(myEvent);
	myEvent.SetInt(1);
	ProcessEvent(myEvent);
}
void OrderInsertPanel::OnInitSubscribe(wxCommandEvent& evt)
{
    if(!g_pPlatformMgr) return;
    vector<IPlatformSingleSvr*> pSvrs=g_pPlatformMgr->GetLogonPlatformSvr();

    for(int i=0;i<(int)pSvrs.size();i++)
    {
  	    if(evt.GetInt()) 
        {
		    //����ҵ����Ϣ
		    pSvrs[i]->SubscribeBusinessData(BID_RspOrderInsert, GID_ORDER_INSERT_PANEL, RspOrderInsertCallBackFunc);
		    pSvrs[i]->SubscribeBusinessData(BID_ErrRtnOrderInsert, GID_ORDER_INSERT_PANEL, RspOrderInsertCallBackFunc);
		    pSvrs[i]->SubscribeBusinessData(BID_RtnOrder, GID_ORDER_INSERT_PANEL, RspOrderInsertCallBackFunc);
		    pSvrs[i]->SubscribeBusinessData(BID_RspOrderAction, GID_ORDER_INSERT_PANEL, RspOrderInsertCallBackFunc);
		    pSvrs[i]->SubscribeBusinessData(BID_ErrRtnOrderAction, GID_ORDER_INSERT_PANEL, RspOrderInsertCallBackFunc);
		    pSvrs[i]->SubscribeBusinessData(BID_RtnTrade, GID_ORDER_INSERT_PANEL, TradeCallBackFunc);	//���ĳɽ��ر�
		    pSvrs[i]->SubscribeBusinessData(BID_RspSubForQuoteRsp, GID_ORDER_INSERT_PANEL, SubscribeForQuoteCallBackFunc);	//���ĳɽ��ر�
		    pSvrs[i]->SubscribeBusinessData(BID_RspUnSubForQuoteRsp, GID_ORDER_INSERT_PANEL, UnSubscribeForQuoteCallBackFunc);	//���ĳɽ��ر�
		    pSvrs[i]->SubscribeBusinessData(BID_RtnForQuoteRsp, GID_ORDER_INSERT_PANEL, ForQuoteRspCallBackFunc);	//���ĳɽ��ر�
	    }
	    else 
        {
		    //ȡ������
		    pSvrs[i]->UnSubscribeBusinessData(BID_RspOrderInsert, GID_ORDER_INSERT_PANEL);
		    pSvrs[i]->UnSubscribeBusinessData(BID_ErrRtnOrderInsert, GID_ORDER_INSERT_PANEL);
		    pSvrs[i]->UnSubscribeBusinessData(BID_RtnOrder, GID_ORDER_INSERT_PANEL);
		    pSvrs[i]->UnSubscribeBusinessData(BID_RspOrderAction, GID_ORDER_INSERT_PANEL);
		    pSvrs[i]->UnSubscribeBusinessData(BID_ErrRtnOrderAction, GID_ORDER_INSERT_PANEL);
		    pSvrs[i]->UnSubscribeBusinessData(BID_RtnTrade, GID_ORDER_INSERT_PANEL);	//���ĳɽ��ر�
		    pSvrs[i]->UnSubscribeBusinessData(BID_RspSubForQuoteRsp, GID_ORDER_INSERT_PANEL);	//���ĳɽ��ر�
		    pSvrs[i]->UnSubscribeBusinessData(BID_RspUnSubForQuoteRsp, GID_ORDER_INSERT_PANEL);	//���ĳɽ��ر�
		    pSvrs[i]->UnSubscribeBusinessData(BID_RtnForQuoteRsp, GID_ORDER_INSERT_PANEL);	//���ĳɽ��ر�
	    }
    }
}

void OrderInsertPanel::OnCfgChanged(wxCommandEvent& evt)
{
	wxCommandEvent MyEvent(wxEVT_GET_STYLE);
	((wxApp*)wxApp::GetInstance())->GetTopWindow()->ProcessEvent(MyEvent);
    int nViewStyle=MyEvent.GetInt();
    void *p=MyEvent.GetClientData();
    SAFE_DELETE(p);
	m_poOrderInputPanel->DynamicShowCtrl();
	m_poOrderInputPanel->SetViewStyle(nViewStyle);
    if(nViewStyle!=m_nViewStyle) { 
        //SetViewStyle(-1, 0);
        LoadStyleUI(nViewStyle, 0);
        SetViewStyle(nViewStyle, 0);
		//SetSellBuyColor();
    }
	if(evt.GetInt()==TYPE_APPLYDEFAULT)//�ָ�Ĭ������ʱ����������������
	{
		m_nMultiplyVolume = 1;
		m_bUpdateRealMsg_Multiply = true;
	}
	//ShowViewSubStyle(0);
	// �����xml�ж��Ƿ�Ҫȷ�ϵ�����
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr != NULL) {
		LPORDER_CFG p = pMgr->GetOrderCfg();
		// ���ý����ϵ��Զ���ƽ��ť
		//������Ĭ���Զ���ƽ
		SetInputAutoOpenClose(p->bAutoOpenClose);

		// �����xml�ж��µ���������
		m_KeyMultiplyVolumeMap.clear();
		for(int i=0; i<p->OrderMultiDataNum; i++) {
			std::string strKey(p->OrderMultiData[i].ShortKey);
			m_KeyMultiplyVolumeMap[strKey] = p->OrderMultiData[i].Volume;
		}
	}

	wxCheckBox* itemCheckWDShow = (wxCheckBox*)FindWindowById(ID_CHECK_WDSHOW, this);
	if(itemCheckWDShow!=NULL) {
		wxCommandEvent evttemp(wxEVT_COMMAND_CHECKBOX_CLICKED, ID_CHECK_WDSHOW);
		if(evt.GetInt()!=TYPE_APPLYINIT &&  evt.GetEventType()==wxEVT_STYLE_CHANGED)//�ָ�Ĭ������ʱ����������������
		{
			itemCheckWDShow->SetValue(false);
			evttemp.SetInt(0);
			OnCheckWDShowClicked(evttemp);
		}
		else {
			itemCheckWDShow->SetValue(NeedFivePriceList()?true:false);
			evttemp.SetInt(NeedFivePriceList());
			OnCheckWDShowClicked(evttemp);
		}
	}
	evt.Skip();
}

void OrderInsertPanel::OnPanelFocus(wxCommandEvent& evt)
{
	wxWindow* pWin = NULL;
	pWin = FindWindowById(ID_OISTD_INSTRUMENTID_TEXT, this);
	if(pWin!=NULL) pWin->SetFocus();
}

void OrderInsertPanel::OnCMDGridInstrumentIDChanged(wxCommandEvent& evt)
{
	TRACE("OrderInsertPanel::OnCMDGridInstrumentIDChanged(wxCommandEvent& evt)\n");

	string EvtParamStr;
    int SenderGID=(int)evt.GetExtraLong();
    DWORD EvtParamID=(DWORD)evt.GetInt();
    if(CFTEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        CFTEventParam::DeleteEventParam(EvtParamID);
    else return;

	// ��Լ���䣬��������
	if(EvtParamStr==GetInputInstrumentID().c_str())
		return;

    if(SenderGID==GID_QUOT_TABLE||SenderGID==GID_OptionTPrice) {
		m_bNoKeyChangeText = FALSE;
		m_bIsGridChangeInstrumentID = TRUE;
        SetInputInstrumentID(wxString(EvtParamStr.c_str()));
		m_bIsGridChangeInstrumentID = FALSE;
		m_bNoKeyChangeText = TRUE;
	}
	else {
//		m_bNoKeyChangeText = FALSE;
//		SetInputInstrumentID(evt.GetString());
//		m_bNoKeyChangeText = TRUE;
	}
}

void OrderInsertPanel::OnCMDViewModeChanged(wxCommandEvent& evt)
{
	int nViewStyle = 0, nViewSubStyle = 0;
	nViewStyle = evt.GetInt()/256;
	nViewSubStyle = evt.GetInt()%256;
	ShowViewStyle(nViewStyle, nViewSubStyle);
}

void LogKeyOrder(KeyOrderInfo& keyOrder, string& strInstID)
{
	string strBuySell;
	string strPrice;
	string strAction;
	string strActionArea;

	switch(keyOrder.nBuySell) {
	case 0:
		strBuySell = LOADSTRING(OLS_BUY);
		break;
	case 1:
	default:
		strBuySell = LOADSTRING(OLS_SELL);
		break;
	};

	switch(keyOrder.nPrice) {
	case 0:
		strPrice = LOADSTRING(OIP_LASTPRICE);
		break;
	case 1:
		strPrice = LOADSTRING(OIP_BIDPRICE);
		break;
	case 2:
		strPrice = LOADSTRING(OIP_ASKPRICE);
		break;
	default:
		break;
	};

	switch(keyOrder.nAction) {
	case 0:
		strAction = "";
		break;
	case 1:
		strAction = LOADSTRING(SEND_ORDER_WITH__CONFIRM);
		break;
	case 2:
		strAction = LOADSTRING(SEND_ORDER_NOT_WITH__CONFIRM);
		break;
	case 3:
		strAction = LOADSTRING(DO_NOT_MOVE_FOCUS_AFTER_ORDER_SEND);
		break;
	case 4:
		strAction = LOADSTRING(FOCUS_MOVE_TO_CONTRACTID_FIELD_AFTER_ORDER_SEND);
		break;
	case 5:
		strAction = LOADSTRING(FOCUS_MOVE_TO_BUYSELL_FIELD_AFTER_ORDER_SEND);
		break;
	case 6:
		strAction = LOADSTRING(FOCUS_MOVE_TO_OPENCLOSE_FIELD_AFTER_ORDER_SEND);
		break;
	case 7:
		strAction = LOADSTRING(FOCUS_MOVE_TO_VOLUMN_FIELD_AFTER_ORDER_SEND);
		break;
	case 8:
		strAction = LOADSTRING(FOCUS_MOVE_TO_PRICE_FIELD_AFTER_ORDER_SEND);
		break;
	case 9:
		strAction = LOADSTRING(FOCUS_MOVE_TO_OPEN_ORDER_BUTTON_AFTER_ORDER_SEND);
		break;
	default:
		break;
	};

	switch(keyOrder.nActionArea) {
	case 0:
		strActionArea = LOADSTRING(GLOBAL);
		break;
	case 1:
		strActionArea = LOADSTRING(IDT_QUOTETABLE);
		break;
	default:
		break;
	};

	USERLOG_INFO("��ݼ������µ�����Լ[%s]��%s[%s]��"
			"%s[%s]��%s[%s]��%s[%d]��%s[%s]��%s[%s]\n", 
			strInstID.c_str(), 
			LOADSTRING(ACCELERATOR), keyOrder.ShortKey,
			LOADSTRING(OLS_DIRECTION), strBuySell.c_str(),
			LOADSTRING(OLS_PRICE), strPrice.c_str(),
			LOADSTRING(OVER_PRICE), keyOrder.nOverPrice,
			LOADSTRING(ACTION), strAction.c_str(),
			LOADSTRING(ACTIONZONE), strActionArea.c_str());
}

void OrderInsertPanel::OnCMDKeyOrder(wxCommandEvent& evt)
{
	KeyOrderInfo keyOrder;
    std::string EvtParamStr;
    if(1)
    {
        DWORD EvtParamID=(DWORD)evt.GetInt();
        bool brlt=false;
        int EvtPtrLen;
        if(CFTEventParam::GetEventParam(EvtParamID,NULL,NULL,NULL,&EvtPtrLen,0,NULL)&&
            EvtPtrLen==sizeof(KeyOrderInfo))
        {
            CFTEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,&keyOrder,&EvtPtrLen,EvtPtrLen,NULL);
            CFTEventParam::DeleteEventParam(EvtParamID);
            brlt=true;
        }
        if(!brlt)
        {
            return;
        }
    }
	wxString strInstrumentID(EvtParamStr);
	LogKeyOrder(keyOrder, string(strInstrumentID.c_str()));
	//USERLOG_INFO("��ݼ������µ�����Լ[%s]����ݼ�[%s]������[%d]���۸�[%d]������[%d]���[%d]����������[%d]\n", 
	//		EvtParamStr.c_str(), 
	//		keyOrder.ShortKey,
	//		keyOrder.nBuySell,
	//		keyOrder.nPrice,
	//		keyOrder.nOverPrice,
	//		keyOrder.nAction,
	//		keyOrder.nActionArea);
	DoKeyOrder(strInstrumentID, keyOrder);
	m_bUpdateRealMsg_PreClose = true;
}

void LogMouseOrder(string& strInstID, char cDirection, int nDefaultVolume, int nAction)
{
	string strAction;
	switch(nAction) {
	case 0:
		strAction = "";
		break;
	case 1:
		strAction = LOADSTRING(SEND_ORDER_WITH__CONFIRM);
		break;
	case 2:
		strAction = LOADSTRING(SEND_ORDER_NOT_WITH__CONFIRM);
		break;
	case 3:
		strAction = LOADSTRING(DO_NOT_MOVE_FOCUS_AFTER_ORDER_SEND);
		break;
	case 4:
		strAction = LOADSTRING(FOCUS_MOVE_TO_CONTRACTID_FIELD_AFTER_ORDER_SEND);
		break;
	case 5:
		strAction = LOADSTRING(FOCUS_MOVE_TO_BUYSELL_FIELD_AFTER_ORDER_SEND);
		break;
	case 6:
		strAction = LOADSTRING(FOCUS_MOVE_TO_OPENCLOSE_FIELD_AFTER_ORDER_SEND);
		break;
	case 7:
		strAction = LOADSTRING(FOCUS_MOVE_TO_VOLUMN_FIELD_AFTER_ORDER_SEND);
		break;
	case 8:
		strAction = LOADSTRING(FOCUS_MOVE_TO_PRICE_FIELD_AFTER_ORDER_SEND);
		break;
	case 9:
		strAction = LOADSTRING(FOCUS_MOVE_TO_OPEN_ORDER_BUTTON_AFTER_ORDER_SEND);
		break;
	default:
		break;
	};

	USERLOG_INFO("�������µ�����Լ[%s]����������[%s]��Ĭ������[%d]��%s[%s]\n", 
            strInstID.c_str(), 
			CFieldValueTool::Direction2String(cDirection, SVR_LANGUAGE).c_str(),
			nDefaultVolume, 
			LOADSTRING(ACTIONZONE), strAction.c_str());
}

void OrderInsertPanel::OnCMDMouseOrder(wxCommandEvent& evt)
{
	//LOG_INFO("�����û��������µ�\n");
	//SetFocus();

	TRACE("OrderInsertPanel::OnCMDMouseOrder(wxCommandEvent& evt)\n");

    std::string EvtParamStr;
    int EvtUserInt;
    if(1)
    {
        DWORD EvtParamID=(DWORD)evt.GetInt();
        if(CFTEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,&EvtUserInt))
        {
            CFTEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }

	// �����xml�ж��Ƿ�Ҫȷ�ϵ�����
	CfgMgr* pMgr = CFG_MGR_DEFAULT(); 
	LPORDER_CFG pOrderCfg = NULL;
	if(pMgr == NULL) return;
	pOrderCfg = pMgr->GetOrderCfg();
	
	int nAction = 0;
	switch(EvtUserInt) {
	case 1:
		nAction = pOrderCfg->ClickQuotTableAction;
		break;
	case 2:
		nAction = pOrderCfg->DoubleClickQuotTableAction;
		break;
	default:
		evt.Skip();
		return;
		break;
	}
	LogMouseOrder(EvtParamStr, evt.GetExtraLong(), 
			GetDefaultVolume(wxString(EvtParamStr.c_str())), nAction);
	//USERLOG_INFO("�������µ�����Լ[%s]����������[%s]��Ĭ������[%d]���[%d]\n", 
 //           EvtParamStr.c_str(), 
	//		CFieldValueTool::Direction2String(evt.GetExtraLong(), SVR_LANGUAGE).c_str(),
 //           GetDefaultVolume(wxString(EvtParamStr.c_str())),
	//		nAction);
	if(nAction==0)	// �������ٿ����У�Action==0��ʾ�޻��Ӧ��������
		return;
	//BOOL bIsAutoOpenClose = GetInputAutoOpenClose();
	//

	// ��������ƽ��ʱ���Զ�����������ӯ��״̬
	if(m_nViewSubStyle==conSubStyle_Position) {
		SetStopPositionSelect(FALSE);
		m_poOrderPositionPanel->InitPositionPanel();
		wxCommandEvent evt(wxEVT_COMMAND_CHECKBOX_CLICKED, ID_DOSTOPPOSITION_CHECK);
		evt.SetInt(0);
		ProcessEvent(evt);
	}

    wxString strCode(EvtParamStr.c_str());				// ���ת��ĺ�Լ����
	int nDefaultVolume = GetDefaultVolume(strCode);	// ��ú�Լ��Ĭ����
	double fPrice = 0.0;
	BOOL bDirect = FALSE;

	set<string> InstrumentIDArray;
	InstrumentIDArray.clear();
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
	if(InstrumentIDArray.find(strCode.c_str())==InstrumentIDArray.end()) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_CONTRERR",TRUE);
		return;
    }
	//m_bNoKeyChangeText = FALSE;
	//SetInputInstrumentID(strCode);
	//m_bNoKeyChangeText = TRUE;

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// ����ط���Ҫ���в�ѯ�Ż��������ԼΪ�ӷ�����������ϣ���δ�����������ϣ���δ���
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strCode.c_str(),field);

	// �����������򼰼۸�
	// pOrderCfg->MouseFastOrderDirection, 1 ͬ��, 0 ����
    if(evt.GetExtraLong()==THOST_FTDC_D_Buy) {
		fPrice = field.BidPrice1;
		bDirect = pOrderCfg->MouseFastOrderDirection ? TRUE : FALSE;
		if((KEYDOWN(VK_LCONTROL) | KEYDOWN(VK_RCONTROL)) && pOrderCfg->bCtrlReverse)
			bDirect = !bDirect;
		SetInputDirection(bDirect);
		if(!util::isInvalidValue(fPrice)) {
			if(bDirect)
				fPrice += pOrderCfg->PositionVolume*m_currPriceTick;
			else
				fPrice -= pOrderCfg->PositionVolume*m_currPriceTick;
			fPrice = AnalysePrice(fPrice, strCode);
		}
		SetInputPrice(fPrice);
    }
    else if(evt.GetExtraLong()==THOST_FTDC_D_Sell) {
		fPrice = field.AskPrice1;
		bDirect = pOrderCfg->MouseFastOrderDirection ? FALSE : TRUE;
		if((KEYDOWN(VK_LCONTROL) | KEYDOWN(VK_RCONTROL)) && pOrderCfg->bCtrlReverse)
			bDirect = !bDirect;
		SetInputDirection(bDirect);
		if(!util::isInvalidValue(fPrice)) {
			if(bDirect)
				fPrice += pOrderCfg->PositionVolume*m_currPriceTick;
			else
				fPrice -= pOrderCfg->PositionVolume*m_currPriceTick;
			fPrice = AnalysePrice(fPrice, strCode);
		}
		SetInputPrice(fPrice);
    }
	else {
		if(GetInputDirection()) {
			fPrice = field.AskPrice1;// + pOrderCfg->PositionVolume*m_DynamicItem.PriceTick;
			//fPrice = AnalysePrice(fPrice);
			SetInputPrice(fPrice);
		}
		else {
			fPrice = field.BidPrice1;// - pOrderCfg->PositionVolume*m_DynamicItem.PriceTick;
			//fPrice = AnalysePrice(fPrice);
			SetInputPrice(fPrice);
		}
	}

	SetInputOrderType(OrderType_PATS_LIMIT);

	// ��ȡ����
	BOOL bDirection = GetInputDirection();

	COrderInsertEntity::ORIORDERREQ tOriReq={0};

	//��ȡ�Ƿ��Զ�
	BOOL bIsAuto = TRUE;
	NeedAutoOpenClose(tOriReq.nAutoMode);

	int nVolume = nDefaultVolume;
	int nSumVolume = 0, nSumTodayVolume=0;

//	// �����Զ���ƽ������
	if(bIsAuto) 
	{
//		SetInputAutoOpenClose(TRUE);
		BOOL bIsBuy = GetInputDirection();
		BOOL bIsHedge = GetInputHedge();
		// ��ƽ�� �ܲ�/���
		//COrderInsertEntity::GetPreVolume(strCode.c_str(), !bIsBuy, bIsHedge, nSumVolume, nSumTodayVolume, string(GetInputAccount().c_str()));
		if(COrderInsertEntity::GetHoldingVolume(strCode.c_str(), !bIsBuy, bIsHedge, nSumVolume, nSumTodayVolume, string(GetInputAccount().c_str()))) {
			nVolume = nSumVolume;
		}
		//if(nSumVolume-nSumTodayVolume>0) {
		//	SetInputOCMode(2);
		//	//SetInputQty(nSumTodayVolume);
		//}
		//else if(nSumTodayVolume>0){
		//	SetInputOCMode(1);
		//	//SetInputQty(nSumVolume-nSumTodayVolume);
		//}
		//else {
		//	SetInputOCMode(0);
		//	//SetInputQty(GetDefaultVolume(strCode));
		//}
	}

	if(NeedAlwaysOpen() || nSumVolume==0) 
	{
		nVolume = nDefaultVolume;
		//SetInputQty(nDefaultVolume);
		//SetInputOCMode(0);
		bIsAuto = FALSE;
	}
	else 
	{
		//if(tOriReq.nAutoMode==1)
		//	SetInputQty(nSumVolume);
		//else 
		//	SetInputQty(nDefaultVolume);
	}

	// ��ȡ��ƽ��
//	int nOCMode = GetInputOCMode();

//	BOOL bHedge = GetInputHedge();

	wxString strAccount=GetInputAccount();

	strcpy(tOriReq.strInstrumentID, strCode.c_str());
	strcpy(tOriReq.strAccount, strAccount.c_str());
	tOriReq.bIsParketOrder = FALSE;
	tOriReq.bDirection = bDirection;
	tOriReq.nOCMode = 0;//nOCMode;
	tOriReq.fPrice = fPrice;
	tOriReq.bIsMarketPrice = FALSE;
	tOriReq.bSimulate = NeedSimulateMarketPrice();
	tOriReq.nVolume = nVolume;
	tOriReq.nDefaultVolume = nDefaultVolume;		// ��OK��ť�£�ʹ������������ΪĬ������
	tOriReq.bIsAuto = bIsAuto;
	NeedAutoOpenClose(tOriReq.nAutoMode);
	tOriReq.nHedgeStatus = 0;

	BOOL bHasOrderInsert = FALSE;
	CConfirmItemChange confirmChg;
	confirmChg.SetItemNum(EvtUserInt==1 ? CConfirmItemChange::conAutoQuotClickConfirm 
			: (EvtUserInt==2 ? CConfirmItemChange::conAutoQuotDBClickConfirm
			: CConfirmItemChange::conStdOrderConfirm)); 
	DoOrderInsert(tOriReq, nAction, bHasOrderInsert, confirmChg, TRUE);
	m_bUpdateRealMsg_PreClose = true;

}

void OrderInsertPanel::OnCMDMouseClose(wxCommandEvent& evt)
{
	//LOG_INFO("�����û�������ƽ��\n");
	//SetFocus();

	// �����xml�ж��Ƿ�Ҫȷ�ϵ�����
	CfgMgr* pMgr = CFG_MGR_DEFAULT(); 
	LPORDER_CFG pOrderCfg = NULL;
	if(pMgr == NULL) return;
	pOrderCfg = pMgr->GetOrderCfg();
	wxString strAccount;

	PlatformStru_Position* pField=(PlatformStru_Position*)evt.GetClientData();
	
	int nAction = 0;
	switch(evt.GetInt()) {
	case 1:
		nAction = pOrderCfg->ClickPositionAction;
		break;
	case 2:
		nAction = pOrderCfg->DoubleClickPositionAction;
		break;
	case 3:
		nAction = NeedConfirm() ? 1 : 2;
		break;
	default:
		delete pField;
		return;
		break;
	}
	if(!nAction)	// ��������ƽ���У�Action==0��ʾ�޻��Ӧ��������
		return;
	//BOOL bIsAutoOpenClose = GetInputAutoOpenClose();
	//

	// ��������ƽ��ʱ���Զ�����������ӯ��״̬
	if(m_nViewSubStyle==conSubStyle_Position) {
		SetStopPositionSelect(FALSE);
		m_poOrderPositionPanel->InitPositionPanel();
		wxCommandEvent evt(wxEVT_COMMAND_CHECKBOX_CLICKED, ID_DOSTOPPOSITION_CHECK);
		evt.SetInt(0);
		ProcessEvent(evt);
	}

	wxString strCode = pField->InstrumentID;				// ���ת��ĺ�Լ����
	int nDefaultVolume = GetDefaultVolume(strCode);			// ��ú�Լ��Ĭ����
	double fPrice = 0.0;

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// ����ط���Ҫ���в�ѯ�Ż��������ԼΪ�ӷ�����������ϣ���δ�����������ϣ���δ���
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strCode.c_str(),field);

	int nSumVolume=0, nSumTodayVolume=0;
	COrderInsertEntity::GetHoldingVolume(strCode.c_str(), 
			pField->PosiDirection==THOST_FTDC_PD_Long, pField->HedgeFlag==THOST_FTDC_HF_Hedge, 
			nSumVolume, nSumTodayVolume, string(pField->Account));

//	// ��ȡ����
	int nVolume = pField->Position;
//	// ��ȡ����
	BOOL bDirection = pField->PosiDirection!=THOST_FTDC_PD_Long;
//	// ��ȡ��ƽ��
	int nOCMode = nSumVolume>nSumTodayVolume?2:1;
	int nHedgeStatus = pField->HedgeFlag-THOST_FTDC_HF_Speculation;

	m_bNoKeyChangeText = FALSE;
	m_bIsGridChangeInstrumentID = TRUE;
	SetInputInstrumentID(strCode);
	m_bIsGridChangeInstrumentID = FALSE;
	m_bNoKeyChangeText = TRUE;
	strAccount = pField->Account;
	SetInputAccount(strAccount);
	//SetInputDirection(pField->PosiDirection!=THOST_FTDC_PD_Long);
	//SetInputOCMode(nSumVolume>nSumTodayVolume?2:1);
	//SetInputHedge(pField->HedgeFlag==THOST_FTDC_HF_Hedge);
	SetInputOrderType(OrderType_PATS_LIMIT);
	//SetInputAccount(wxString(pField->Account));
	if(pField->PosiDirection==THOST_FTDC_PD_Long)	{	// �ֲ�Ϊ���ǣ��õ�ǰ���ִ��ƽ������
		fPrice = field.BidPrice1 - pOrderCfg->PositionVolume*m_currPriceTick;
		if(util::isInvalidValue(fPrice))
			fPrice = field.LastPrice - pOrderCfg->PositionVolume*m_currPriceTick;
		fPrice = AnalysePrice(fPrice, strCode);
		SetInputPrice(fPrice);
	}
	else {
		fPrice = field.AskPrice1 + pOrderCfg->PositionVolume*m_currPriceTick;
		if(util::isInvalidValue(fPrice))
			fPrice = field.LastPrice + pOrderCfg->PositionVolume*m_currPriceTick;
		fPrice = AnalysePrice(fPrice, strCode);
		SetInputPrice(fPrice);
	}
    ////SetInputQty(pField->CloseVolume);
	//SetInputQty(pField->Position);
    delete pField;

//	SetInputAutoOpenClose(TRUE);

//	// ��ȡ����
//	int nVolume = GetInputQty();
//	// ��ȡ����
//	BOOL bDirection = GetInputDirection();// GetButtonCheck(ID_RADIOBUTTON1);
//	// ��ȡ��ƽ��
//	int nOCMode = GetInputOCMode();
	//��ȡ�Ƿ��Զ�
	BOOL bIsAuto = TRUE;

	//strAccount = GetInputAccount();

	COrderInsertEntity::ORIORDERREQ tOriReq={0};
	strcpy(tOriReq.strInstrumentID, strCode.c_str());
	strcpy(tOriReq.strAccount, strAccount.c_str());
	tOriReq.bIsParketOrder = FALSE;
	tOriReq.bDirection = bDirection;
	tOriReq.nOCMode = nOCMode;
	tOriReq.fPrice = fPrice;
	tOriReq.bIsMarketPrice = FALSE;
	tOriReq.bSimulate = NeedSimulateMarketPrice();
	tOriReq.nVolume = nVolume;
	tOriReq.nDefaultVolume = evt.GetInt()==3?nVolume:GetDefaultVolume(strCode);		// ��OK��ť�£�ʹ������������ΪĬ������
	tOriReq.bIsAuto = bIsAuto;
	NeedAutoOpenClose(tOriReq.nAutoMode);
	if(evt.GetInt()==3)
		tOriReq.nAutoMode=1;
	tOriReq.nHedgeStatus = nHedgeStatus;

	BOOL bHasOrderInsert = FALSE;
	CConfirmItemChange confirmChg;
	confirmChg.SetItemNum(evt.GetInt()==1 ? 
			CConfirmItemChange::conAutoPositionClickConfirm 
			: (evt.GetInt()==2 ? CConfirmItemChange::conAutoPositionDBClickConfirm
			: CConfirmItemChange::conStdOrderConfirm)); 
	DoOrderInsert(tOriReq, nAction, bHasOrderInsert, confirmChg, TRUE);
	m_bUpdateRealMsg_PreClose = true;

}

void OrderInsertPanel::OnCMDOrderChange(wxCommandEvent& evt)
{
	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return;
	//LOG_INFO("�����û���ݸĵ�\n");

//	wxString strParam(evt.GetString());
    std::string EvtParamStr;
    int EvtParamInt;
    if(1)
    {
        DWORD EvtParamID=(DWORD)evt.GetInt();
        if(CFTEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,&EvtParamInt))
        {
            CFTEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }
	wxString strParam(EvtParamStr);


	switch(EvtParamInt) {
	case 1:		// �м۷���
	{
		string strAccount;
		string strID;
//		char strTmpAccount[1024];
//		char strTmpID[1024];
		char cDirection=0;
		char cHedge=0;
		PlatformStru_Position PositionData;
//		ZeroMemory(strTmpAccount, sizeof(strTmpAccount));
//		ZeroMemory(strTmpID, sizeof(strTmpID));
		//ZeroMemory(&PositionData, sizeof(PositionData));
//		if(strParam[0]==',') {
//			strParam=strParam.substr(1, strParam.size()-1);
//			sscanf_s(strParam.c_str(), "%[^,],%c,%c", strTmpID, sizeof(strTmpID)-1, 
//					&cDirection, sizeof(cDirection), &cHedge, sizeof(cHedge));
//		}
//		else
//			sscanf_s(strParam.c_str(), "%[^,],%[^,],%c,%c", strTmpAccount, sizeof(strTmpAccount)-1, 
//					strTmpID, sizeof(strTmpID)-1, &cDirection, sizeof(cDirection), &cHedge, sizeof(cHedge));
//		strAccount = strTmpAccount;
//		strID = strTmpID;
		vector<string> outData;
		if(SplitString(strParam.c_str(), outData)<4) 
			return;
		strAccount = outData[0];
		strID = outData[1];
		if(outData[2].size()>0)
			cDirection = outData[2][0];
		if(outData[3].size()>0)
			cHedge = outData[3][0];
        IPlatformSingleSvr* pSvr = g_pPlatformMgr->GetPlatformSvr(strAccount);
	    if(pSvr!=NULL&&(pSvr->GetPositionData(strAccount, strID, cDirection, cHedge, PositionData))==0) 
        {
			DoBackHand(strAccount, strID, cDirection, cHedge, PositionData.Position);
			m_bUpdateRealMsg_PreClose = true;
		}
	}
		break;
	case 2:		// ���ٸĵ�
	{
		BOOL bIsFound = FALSE;
		PlatformStru_OrderInfo ftdcField;
		vector<PlatformStru_OrderInfo> outData;

        string strAccount;
		string strExchangeID;
		string strOrderSysID;
		vector<string> outDataStr;
		if(SplitString(strParam.c_str(), outDataStr)<3) 
			return;
        strAccount = outDataStr[0];
		strExchangeID = outDataStr[1];
		strOrderSysID = outDataStr[2];

        IPlatformSingleSvr* pSvr = g_pPlatformMgr->GetPlatformSvr(strAccount);
	    if(pSvr==NULL) 
            return;

		outData.clear();
		if(pSvr->GetAllOrders(outData)>0) 
        {
            int i,count;
            count=outData.size();
            for(i=0;i<count;i++)
            {
				if(strOrderSysID.compare(outData[i].OrderSysID)==0 && strExchangeID.compare(outData[i].ExchangeID)==0) 
                {
                    ftdcField = outData[i];
					bIsFound = TRUE;
					break;
				}
			}
			if(bIsFound)
			{
				CLocalOrderService* poService = CLocalOrderService::GetInstance();
				if(poService!=NULL)
				{
					poService->LockObject();
					CLocalOrderService::MAPPLORDER plMap = poService->GetOrderPLMap();
					poService->UnlockObject();
					CLocalOrderService::MAPPLORDERITEM it = plMap.begin();
					for(; it!=plMap.end(); it++)
					{
						if(strncmp(it->second.ref.OrderSysID, ftdcField.OrderSysID, sizeof(ftdcField.OrderSysID))==0)
						{
							if(wxMessageBox(LOADSTRING(MSG_CANCELPLORDER_CONTENT), 
									LOADSTRING(MSG_CANCELPLORDER_TITLE), wxYES_NO|wxICON_QUESTION)!=wxYES)
								return;
						}
					}
				}
				DoOrderChange(ftdcField, FALSE);
				m_bUpdateRealMsg_PreClose = true;
			}
		}
		break;
	}
	case 3:		// �������
	{
		string strAccount;
		string strID;
//		char strTmpAccount[1024];
//		char strTmpID[1024];
		char cDirection=0;
		char cHedge=0;
		PlatformStru_Position PositionData;
//		ZeroMemory(strTmpAccount, sizeof(strTmpAccount));
//		ZeroMemory(strTmpID, sizeof(strTmpID));
		//ZeroMemory(&PositionData, sizeof(PositionData));
//		if(strParam[0]==',') {
//			strParam=strParam.substr(1, strParam.size()-1);
//			sscanf_s(strParam.c_str(), "%[^,],%c,%c", strTmpID, sizeof(strTmpID)-1, 
//					&cDirection, sizeof(cDirection), &cHedge, sizeof(cHedge));
//		}
//		else
//			sscanf_s(strParam.c_str(), "%[^,],%[^,],%c,%c", strTmpAccount, sizeof(strTmpAccount)-1, 
//					strTmpID, sizeof(strTmpID)-1, &cDirection, sizeof(cDirection), &cHedge, sizeof(cHedge));
//		strAccount = strTmpAccount;
//		strID = strTmpID;
		vector<string> outData;
		if(SplitString(strParam.c_str(), outData)<4) 
			return;
		strAccount = outData[0];
		strID = outData[1];
		if(outData[2].size()>0)
			cDirection = outData[2][0];
		if(outData[3].size()>0)
			cHedge = outData[3][0];
		if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPositionData(strAccount, strID, cDirection, cHedge, PositionData)==0) {
			DoBackHand(strAccount, strID, cDirection, cHedge, PositionData.Position, TRUE);
			m_bUpdateRealMsg_PreClose = true;
		}
	}
		break;
	};
}

void OrderInsertPanel::OnCMDOrderReplace(wxCommandEvent& evt)
{
	//LOG_INFO("�����û����̸ĵ�\n");

    std::string EvtParamStr;
    int EvtParamInt;
    if(1)
    {
        DWORD EvtParamID=(DWORD)evt.GetInt();
        if(CFTEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,&EvtParamInt))
        {
            CFTEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }
	wxString strParam(EvtParamStr);

	if(g_pPlatformMgr==NULL)
		return;

	BOOL bIsFound = FALSE;
	PlatformStru_OrderInfo ftdcField;
	vector<PlatformStru_OrderInfo> outData;
    string strAccount;
	string strExchangeID;
	string strOrderSysID;
	vector<string> outDataStr;
	if(SplitString(strParam.c_str(), outDataStr)<3) 
		return;
	strAccount = outDataStr[0];
	strExchangeID = outDataStr[1];
	strOrderSysID = outDataStr[2];
	outData.clear();

    IPlatformSingleSvr* pSvr = g_pPlatformMgr->GetPlatformSvr(strAccount);
    IPlatformSingleSvr* pQuotSvr = IS_MULTIACCOUNT_VERSION ? CURRENT_SVR() : pSvr;
	if(pSvr==NULL || pQuotSvr==NULL) 
		return ;

	if(pSvr->GetAllOrders(outData)>0) 
    {
        int i,count;
        count=outData.size();
        for(i=0;i<count;i++)
        {
            if( strAccount.compare(outData[i].Account)==0 &&
                strOrderSysID.compare(outData[i].OrderSysID)==0 && 
                strExchangeID.compare(outData[i].ExchangeID)==0) 
            {
                ftdcField = outData[i];
				bIsFound = TRUE;
				break;
			}
		}
	}

	if(!bIsFound)
		return;

	BOOL bIsAsyncView = FALSE;

	int nSourceVolume = ftdcField.VolumeTotal;
	double fSourcePrice = ftdcField.LimitPrice;

	// ��ȡ����
	BOOL bDirection = (ftdcField.Direction==THOST_FTDC_D_Buy ? 1 : 0);

	if(bIsAsyncView) {
	//	m_bNoKeyChangeText = FALSE;
	//	SetInputInstrumentID(strInstrumentID);
	//	m_bNoKeyChangeText = TRUE;

	//	SetInputDirection(bDirection);
	//	SetInputQty(nVolume);
	//	wxString strPriceText(wxT("�м�"));
	//	SetInputPriceText(strPriceText);
	}

//	// ��ȡ��ƽ��
//	int nOCMode = GetInputOCMode();
//	//��ȡ�Ƿ��Զ�
//	BOOL bIsAuto = TRUE;
	wxString strName = ftdcField.InstrumentID;
    PlatformStru_DepthMarketData field;
    memset(&field,0,sizeof(field));
    pQuotSvr->GetQuotInfo(strName.c_str(),field);
    PlatformStru_InstrumentInfo insInfo;
    if(pSvr->GetInstrumentInfo(strName.c_str(),insInfo)==0) {
		m_currPriceTick=insInfo.PriceTick;
    }
	if(m_poStaticFivePriceList->ResetDepthMarketData(field)) {
		m_poStaticFivePriceList->Refresh();
	}

	int nNewVolume = 0;
	double fNewPrice = 0.0;
	BOOL bNewIsMarketPrice = FALSE;

	
	// ����һ���Ի���������ʾ�������Ĵ��壬�����û��޸ļ۸������
	// ...
	PlatformStru_InputOrder tFtdcOrderInputField = {0};
	CEditOrderReplaceDailog* pDlg = new CEditOrderReplaceDailog(this, wxID_ANY, 
			LOADSTRING(OLS_REPLACE_CURRENT_ORDER), wxDefaultPosition, wxSize(310, 250));
	pDlg->SetPriceTick(m_currPriceTick);
	pDlg->SetFtdcOrderField(ftdcField);
	if(pDlg->ShowModal()==wxID_OK) 
    {
		tFtdcOrderInputField = pDlg->GetFtdcOrderInputField();
		pDlg->Destroy();
	}
	else 
    {
		pDlg->Destroy();
		return;
	}

	// ������չ�������
	if(tFtdcOrderInputField.VolumeTotalOriginal > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) 
    {
		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITPRICE), LOADSTRING(OIP_FASTTRADER), 
				wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES)
			return;
	}

	// ������չ����۸�
	BOOL bIsPriceLimit = tFtdcOrderInputField.LimitPrice>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
		|| tFtdcOrderInputField.LimitPrice<field.LastPrice - m_currPriceTick*GetRaskDifferPrice();
	bIsPriceLimit = bIsPriceLimit && GetRaskDifferPrice();
	if(bIsPriceLimit) 
    {
		wxMessageDialog dialog( NULL,LOADSTRING(OIP_OVERLIMITPRICE), LOADSTRING(OIP_FASTTRADER), 
				wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		if(dialog.ShowModal()!=wxID_YES) 
			return;
	}

	BOOL bOK = TRUE;
	int nRet = 0;
	wxString strErrMsg;
	wxString strMsg;
	if(NeedConfirm()) 
    {
		PlatformStru_InputOrder& req = tFtdcOrderInputField;
		strMsg.Printf(LOADFORMATSTRING(PRCQTY_FORMAT,"%s%s%s%s%s%d%s"),//�ı��������%s %s %s %s %s %d %s
					Price2String(req.LimitPrice, m_currPriceTick).c_str(),
					wxEmptyString,
					req.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY) :LOADSTRING(OLS_SELL),
					req.InstrumentID, 
					req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ?LOADSTRING(OLS_OPEN): 
					(req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)),
					req.VolumeTotalOriginal,
					req.CombHedgeFlag[0]==THOST_FTDC_HF_Hedge ? LOADSTRING(OLS_HEDGE): LOADSTRING(OLS_SPECULATE));
		wxMessageDialog dialog(NULL, strMsg, LOADSTRING(OLS_ORDER_CONFIRM), 
				wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
		bOK = dialog.ShowModal()==wxID_YES;
	}
	if(bOK) 
    {
 		nRet = VerifyOrder(tFtdcOrderInputField);
	}
	wxString strCaption;
	if(nRet==0) {
		m_bWaitingRsp_OrderInsert=true;
	}
	else {
		//���ô�����Ϣ
		strErrMsg = CFieldValueTool::ReturnCode2String(nRet, SVR_LANGUAGE).c_str();
		//if(NeedOrderFailConfirm())
		ShowTradeInfoDlg("OLS_SEND_ORDER_FAIL", strErrMsg.c_str(), NeedOrderFailConfirm());
	}

	if(bOK) {
		if(bIsAsyncView) {
			//1 �µ�������µ���, 2 ȫ�����, 3 ��ռ۸������, 4 ֻ�������
			m_poOrderInputPanel->ClearViewData(GetActionAfterOrder());
		}
	}

	return;

}

void OrderInsertPanel::OnMouseWheel(wxMouseEvent& evt)
{
	if(FindFocus()!=FindWindowById(ID_BUTTON_OK,this))
		FindWindowById(ID_BUTTON_OK, this)->SetFocus();
	evt.Skip();
}

void OrderInsertPanel::DoInitQryStep()
{
    vector<string> accountList;
    accountList.clear();
    //if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_PATSAPI) {
        if(DEFAULT_SVR()&&DEFAULT_SVR()->GetAccountList(accountList)>0) {
            m_poOrderInputPanel->ResetAccountList(accountList);
            m_poPatsOrderInputPanel->ResetAccountList(accountList);
        }
    //}
    m_poOrderInputPanel->ResetContractList();
    m_poPatsOrderInputPanel->ResetContractList();
}

void OrderInsertPanel::OnLanguageChanged( wxCommandEvent& event )
{
	wxWindow* pWin = NULL;
	pWin = FindWindowById(ID_CHECK_WDSHOW, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_FIVE_PRICE_LIST));
	pWin = FindWindowById(ID_CHECK_WINDOWTOPALWAYS, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_MOST_TOP_WINDOW));
	pWin = FindWindowById(ID_DOSTOPPOSITION_CHECK, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_STOP_P_L));
	pWin = FindWindowById(ID_DOCONDITION_CHECK, this);
	if(pWin) pWin->SetLabel(LOADSTRING(OLS_CONDITION_SEND_ORDER));

	m_poOrderInputPanel->ResetLanguage();
	m_poPatsOrderInputPanel->ResetLanguage();
	m_poOrderBatchPanel->ResetLanguage();
	m_poOrderConditionPanel->ResetLanguage();
	m_poOrderPositionPanel->ResetLanguage();
	m_poOrderStopPanel->ResetLanguage();
	m_poOrderInsertCommandPanel->ResetLanguage();
	m_poStaticFivePriceList->Refresh();
}

void OrderInsertPanel::OnInfoListFocus(wxFocusEvent& evt)
{
	wxWindow* pWin = FindWindowById(ID_BUTTON_MARKETPRICE, this);
	if(pWin) pWin->SetFocus();
}
