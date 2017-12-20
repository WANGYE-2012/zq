#include "StdAfx.h"
#include "COrderInputPanel_kq.h"
#include "../inc/Module-Misc/zqControl.h"
#include "tools/util.h"
#include "wx\odcombo.h"


using namespace util;

#define ROW_HEIGHT  13
#define ROW_WIDTH   12
#define WDPANEL_WIDTH 146
#define ORDERPANEL_WIDTH 174


BEGIN_EVENT_TABLE(COrderInputPanel_kq, CBaseInputPanel)
	EVT_PAINT(OnPaint)

	EVT_TEXT(ID_OIKQ_INSTRUMENTID_TEXT, OnInstrumentIdTextChanged)
	EVT_TEXT_MAXLEN(ID_OIKQ_INSTRUMENTID_TEXT, OnTextMaxLen) 

	EVT_STATIC_LEFT_UP(OnStaticLeftUp)
	EVT_SPIN_UP(ID_OIKQ_PRICE_SPINCTRL, OnPriceSpinUp)
	EVT_SPIN_DOWN(ID_OIKQ_PRICE_SPINCTRL, OnPriceSpinDown)
	EVT_SPIN_UP(ID_OIKQ_VOLUME_SPINCTRL, OnVolumeSpinUp)
	EVT_SPIN_DOWN(ID_OIKQ_VOLUME_SPINCTRL, OnVolumeSpinDown)

    EVT_STATIC_LEFT_UP(OnStaticLeftUp)

	//EVT_COMMAND(wxID_ANY, wxEVT_TEXT_FOCUS, OnInstrumentIDClicked) 
	EVT_COMMAND(wxID_ANY, wxEVT_CODECLICKED, OnNextCodeClicked) 
	EVT_COMMAND(wxID_ANY, wxEVT_PRICECLICKED, OnNextPriceClicked) 
	EVT_COMMAND(wxID_ANY, wxEVT_VOLUMECLICKED, OnNextVolumeClicked) 

	EVT_COMMAND(wxID_ANY, wxEVT_MOUSEINPUT_CODE, OnMouseInputCode) 
	EVT_COMMAND(wxID_ANY, wxEVT_MOUSEINPUT_DIGIT, OnMouseInputDigit) 
	EVT_COMMAND(wxID_ANY, wxEVT_MOUSEINPUT_VOLUME, OnMouseInputVolume) 

	EVT_CFG_CHANGE(OnCfgChanged)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


COrderInputPanel_kq::COrderInputPanel_kq(wxWindow *parent,
			wxWindowID winid,
			const wxPoint& pos,
			const wxSize& size,
			long style,
			const wxString& name):
	CBaseInputPanel(parent,winid,pos,size,style,name),
	m_MouseHandyPopupPane(NULL),
	m_MouseHandyPopupPaneVolume(NULL),
	m_MouseHandyPopupPaneDigit(NULL),
	m_bIsLockForSetChange(FALSE),
	m_bNoKeyChangeText(TRUE),
	m_colorBak(0,0,0),
	m_bIsInitOK(FALSE)
{

	SetBackgroundColour(DEFAULT_COLOUR);
	wxFont oldfont=GetFont();
	oldfont.SetWeight(wxBOLD);
	SetFont(oldfont);


	//m_MouseHandyPopupPane = new MouseHandyInputPane(this);	//
	m_MouseHandyPopupPane = new CMouseHandyInputPane(this, wxID_ANY, wxString(""), wxDefaultPosition, wxSize(300, 400));	//
	m_MouseHandyPopupPane->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(COrderInputPanel_kq::OnCodePanelKeyAscii),NULL,this);
	m_MouseHandyPopupPaneVolume = new MouseHandyInputPaneVolume(this);
	m_MouseHandyPopupPaneVolume->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(COrderInputPanel_kq::OnVolumePanelKeyAscii),NULL,this);
	m_MouseHandyPopupPaneDigit = new CMouseHandyInputPaneDigit(this, wxID_ANY, wxString(""), wxDefaultPosition, wxSize(300, 400));
	m_MouseHandyPopupPaneDigit->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(COrderInputPanel_kq::OnPricePanelKeyAscii),NULL,this);

	
	int nFW = 8, nFH = 18;
	int nVW = nFW+2, nVH = nFH+4;
	wxPoint ctlPos(5, 5);
	wxSize ctlSz(0,0);

	wxWindow* pWin = NULL;


	ctlPos.y+=3;
	pWin = new wxStaticText( this, ID_OIKQ_STATIC1, STATIC_CONTACT_TEXT, ctlPos);
	ctlPos.y += nVH;
	pWin = new wxStaticText( this, ID_OIKQ_STATIC2, STATIC_DIRECT_TEXT, ctlPos);
	ctlPos.y += nVH;
	ToggleStaticText* itemOpenClose = new ToggleStaticText( this, ID_OIKQ_AUTOOPENCLOSE_STATIC, 
			STATIC_OFFSETFLAG_TEXT, STATIC_OFFSETFLAG_AUTO);
	itemOpenClose->SetPosition(ctlPos);
	ctlPos.y += nVH;
	pWin = new wxStaticText( this, ID_OIKQ_STATIC3, STATIC_VOLUME_TEXT, ctlPos);
	ToggleStaticText* itemAutoTrackPrice = new ToggleStaticText( this, ID_OIKQ_AUTOTRACKPRICE_STATIC, 
			STATIC_LIMITPRICE_TEXT, STATIC_MARKETPRICE_TEXT);
	ctlPos.y += nVH;
	itemAutoTrackPrice->SetPosition(ctlPos);
	ctlPos.y += nVH;
	pWin = new wxStaticText( this, ID_OIKQ_STATIC4, ("�ʺ�:"), ctlPos);


	wxArrayString strArrCodeList;
	std::set<std::string> outData;
	// ��ȡ��Լ�б�
	outData.clear();
	if(g_pPlatform) g_pPlatform->GetInstrumentList(outData);
	std::set<std::string>::iterator it=outData.begin();
	for(; it!=outData.end(); ++it) {
		strArrCodeList.Add(*it);
	}


	ctlPos=wxPoint(5+nFW*6, 5);
	

	ctlSz.Set(116, 20);
	wxTextCtrl* pText = new wxTextCtrl(this, ID_OIKQ_INSTRUMENTID_TEXT, wxEmptyString, ctlPos, ctlSz, 0);
	pText->SetMaxLength(30);
	pText->Connect(ID_OIKQ_INSTRUMENTID_TEXT, wxEVT_LEFT_UP, 
			wxMouseEventHandler(COrderInputPanel_kq::OnCodeClicked), NULL, this);

	ctlSz.Set(116, 20);
	//WXLOG_INFO("wxOwnerDrawnComboBox--%d",GetTickCount());
	pWin = new wxOwnerDrawnComboBox(this, ID_OIKQ_INSTRUMENTID_COMBOCTRL, wxT(""), 
			wxPoint(-1000, -1000), ctlSz, strArrCodeList, wxCB_DROPDOWN);//|wxCB_SORT);
	//WXLOG_INFO("wxOwnerDrawnComboBox--%d",GetTickCount());
	//ctlSz.Set(116, 0);
	//wxOwnerDrawnComboBox* pComboCtrl = new wxOwnerDrawnComboBox();
	//pComboCtrl->Create(this, ID_OIKQ_INSTRUMENTID_COMBOCTRL, wxEmptyString,
	//			ctlPos, ctlSz, strArrCodeList, wxCB_DROPDOWN);

	wxArrayString strArrBS;
	strArrBS.Add(wxT("����"));
	strArrBS.Add(wxT("����"));
	ctlPos.y += nVH;
	ctlSz.Set(nFW*8, 20);
	wxOwnerDrawnComboBox* pCmbBuySell = new wxOwnerDrawnComboBox(this, ID_OIKQ_BUYSELL_COMBOBOX, wxT("����"), 
			ctlPos, ctlSz, strArrBS, wxCB_READONLY);
	pCmbBuySell->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(COrderInputPanel_kq::OnCmbBuySell), NULL, this);

	ctlSz.Set(52, 20);
	pWin = new wxCheckBox( this, ID_OIKQ_HEDGE_CHECKBOX, _("��ֵ"), wxPoint(ctlPos.x+nFW*9-4, ctlPos.y+4));

	wxArrayString strArrOC;
	strArrOC.Add(wxT("����"));
	strArrOC.Add(wxT("ƽ��"));
	strArrOC.Add(wxT("ƽ��"));
	ctlPos.y += nVH;
	ctlSz.Set(116, 20);
	wxOwnerDrawnComboBox* pCmbOpenClose = new wxOwnerDrawnComboBox(this, ID_OIKQ_OPENCLOSE_COMBOBOX, wxT("����"), 
			ctlPos, ctlSz, strArrOC, wxCB_READONLY);
	pCmbOpenClose->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, 
			wxCommandEventHandler(COrderInputPanel_kq::OnCmbOpenClose), NULL, this);

	ctlPos.y += nVH;
	CreateFloatSpinCtrl_Volume(ctlPos);

	ctlPos.y += nVH;
	CreateFloatSpinCtrl_Price(ctlPos);
	wxArrayString strArrAccount;
	string strInvestorID="", strBrokerID="";
	if(g_pPlatform&&g_pPlatform->GetPlatformId()==PID_CTP) {
		if(g_pPlatform) g_pPlatform->GetBrokerIDUserID(strInvestorID, strBrokerID);
		strArrAccount.Add(wxT(strInvestorID));
	}
	else
	{
		set<string> setAccount;
		if(g_pPlatform) g_pPlatform->GetBrokerIDUserID(strInvestorID, strBrokerID, &setAccount);
		//setAccount.insert("QQ123456");
		//setAccount.insert("PATS9876543");
		//setAccount.insert("IB991753");
		if(setAccount.size()!=0) {
			for(set<string>::iterator it = setAccount.begin(); it != setAccount.end(); it++)
				strArrAccount.Add(wxT(*it));
		}
	}
	ctlPos.y += nVH;
	ctlSz.Set(116, 20);
	wxOwnerDrawnComboBox* pComboBox = new wxOwnerDrawnComboBox(this, ID_OIKQ_ACCOUNT_TEXT, 
		wxT(""), ctlPos, ctlSz, strArrAccount, wxCB_DROPDOWN);//|wxCB_SORT);
	pComboBox->GetTextCtrl()->SetMaxLength(20);
	if(pComboBox->GetCount()>0)
		pComboBox->Select(0);

	InitOrderView();
	SetSellBuyColor();

	m_bIsInitOK = TRUE;

}

COrderInputPanel_kq::~COrderInputPanel_kq()
{
}

COrderInputPanel_kq* COrderInputPanel_kq::CreatePane(wxWindow* parent)
{
    return new COrderInputPanel_kq(parent, wxID_ANY);
}

wxTextCtrl* COrderInputPanel_kq::CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(100, 20);
	wxTextCtrl* itemTextCtrl3 = new wxTextCtrl( this, ID_OIKQ_PRICE_TEXT, wxEmptyString, ctlPos, ctlSz, 0 );

	itemTextCtrl3->Connect(ID_OIKQ_PRICE_TEXT, wxEVT_LEFT_UP, 
			wxMouseEventHandler(COrderInputPanel_kq::OnPriceClicked), NULL, this);

	ctlPos.x += 100;
	ctlSz.Set(16, 20);
    wxSpinButton* itemSpinButton4 = new wxSpinButton( this, ID_OIKQ_PRICE_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(0, 1000000);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

wxTextCtrl* COrderInputPanel_kq::CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos=ctrPosInit;

	ctlSz.Set(100, 20);
    wxTextCtrl* itemTextCtrl3 = 
			new wxTextCtrl(this, ID_OIKQ_VOLUME_TEXT, wxEmptyString, ctlPos, ctlSz, 0);

	itemTextCtrl3->Connect(ID_OIKQ_VOLUME_TEXT, wxEVT_LEFT_UP, 
			wxMouseEventHandler(COrderInputPanel_kq::OnVolumeClicked), NULL, this);
	itemTextCtrl3->SetMaxLength(4);

	ctlPos.x += 100;
	ctlSz.Set(16, 20);
    wxSpinButton* itemSpinButton4 = 
			new wxSpinButton( this, ID_OIKQ_VOLUME_SPINCTRL, ctlPos, ctlSz, wxSP_VERTICAL );
    itemSpinButton4->SetRange(0, 9999);
    itemSpinButton4->SetValue(0);
    return itemTextCtrl3;
}

void COrderInputPanel_kq::SetViewMode(int nViewMode)
{
	wxWindow* window = NULL;
	for(int i=ID_OIKQ_FIRST+1; i<SUMNUM_OIKQ_ID; i++) {
		window = FindWindowById(i, this);
		if(window!=NULL) window->Enable(true);
	}
	switch(nViewMode) {
	case 0:		// �����µ�
		break;
	case 1:		// �޸������µ�
		window=FindWindowById(ID_OIKQ_INSTRUMENTID_TEXT, this);
		window->Enable(false);
		//window=FindWindowById(ID_OIKQ_LONGCODE_COMBOBOX, this);
		//window->Enable(false);
		break;
	case 2:		// �޸�ӯ���µ�
		window=FindWindowById(ID_OIKQ_INSTRUMENTID_TEXT, this);
		window->Enable(false);
		//window=FindWindowById(ID_OIKQ_LONGCODE_COMBOBOX, this);
		//window->Enable(false);
		window=FindWindowById(ID_OIKQ_BUYSELL_COMBOBOX, this);
		window->Enable(false);
		window=FindWindowById(ID_OIKQ_OPENCLOSE_COMBOBOX, this);
		window->Enable(false);
		window=FindWindowById(ID_OIKQ_PRICE_TEXT, this);
		window->Enable(false);
		window=FindWindowById(ID_OIKQ_VOLUME_TEXT, this);
		window->Enable(false);
		window=FindWindowById(ID_OIKQ_HEDGE_CHECKBOX, this);
		window->Enable(false);
		window=FindWindowById(ID_OIKQ_PRICE_SPINCTRL, this);
		window->Enable(false);
		window=FindWindowById(ID_OIKQ_VOLUME_SPINCTRL, this);
		window->Enable(false);
		break;
	case 3:		// ��ӯ��
		window=FindWindowById(ID_OIKQ_AUTOOPENCLOSE_STATIC, this);
		window->Enable(false);
		window=FindWindowById(ID_OIKQ_OPENCLOSE_COMBOBOX, this);
		window->Enable(false);
		SetOpenClose(0);
		break;
	};
}

BOOL COrderInputPanel_kq::NeedMouseHandyPane_Code()
{
	// �����xml�ж��Ƿ�Ҫȷ�ϵ�����
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bMouseInputInstrumentId;
}

BOOL COrderInputPanel_kq::NeedMouseHandyPane_Volume()
{
	// �����xml�ж��Ƿ�Ҫȷ�ϵ�����
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bMouseInputVolume;
}

BOOL COrderInputPanel_kq::NeedMouseHandyPane_Price()
{
	// �����xml�ж��Ƿ�Ҫȷ�ϵ�����
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bMouseInputPrice;
}

// ����������д��ƴ��
BOOL COrderInputPanel_kq::NeedInputAbbreviation()
{
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bInputAbbreviation;
}

wxString COrderInputPanel_kq::Number2String(int num)
{
    wxString str="-";
    if(num!=-1) 
       str.Printf("%d",num);
    return str;
}

wxString COrderInputPanel_kq::Double2String(double num)
{
	wxString strPriceText="-";
	if(num!=-1.0f && (!util::isInvalidValue(num))) {
		//һλ����λ��û��С��λ
		int percentnum=(int)(m_fPriceTick*10000.0f);
		if(percentnum%10!=0) {			//��λС��λ
			strPriceText.Printf("%.4f",num);
		}
		else if(percentnum%100!=0) {	//��λС��λ
			strPriceText.Printf("%.3f",num);
		}
		else if(percentnum%1000!=0) {	//��λС��λ
			strPriceText.Printf("%.2f",num);
		}
		else if(percentnum%10000!=0) {	//һλС��λ
			strPriceText.Printf("%.1f",num);
		}
		else {							//û��С��λ
			strPriceText.Printf("%.0f",num);
		}
	}

	return strPriceText;
}

void COrderInputPanel_kq::SetInstrumentId(wxString& strInstrumentId, BOOL bSendChangeEvent)
{
	
	m_bNoKeyChangeText = FALSE;

	wxWindow* window=FindWindowById(ID_OIKQ_INSTRUMENTID_TEXT, this);
	window->SetLabel(strInstrumentId);

	m_bNoKeyChangeText = TRUE;
}

wxString COrderInputPanel_kq::GetInstrumentId()
{
	wxTextCtrl* window=(wxTextCtrl*)FindWindowById(ID_OIKQ_INSTRUMENTID_TEXT, this);
	return window->GetValue();
}
void COrderInputPanel_kq::SetAccount(wxString& strAccount)
{
	wxWindow* window=FindWindowById(ID_OIKQ_ACCOUNT_TEXT, this);
	window->SetLabel(strAccount);
}

wxString COrderInputPanel_kq::GetAccount()
{
	wxOwnerDrawnComboBox* window=(wxOwnerDrawnComboBox*)FindWindowById(ID_OIKQ_ACCOUNT_TEXT, this);
	return window->GetValue();
}

void COrderInputPanel_kq::SetDirection(BOOL bIsBuy)
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OIKQ_BUYSELL_COMBOBOX,this);
	pComboBox->SetSelection(bIsBuy?0:1);
	SetSellBuyColor();
}

// �����������TRUEΪ����
BOOL COrderInputPanel_kq::GetDirection()
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OIKQ_BUYSELL_COMBOBOX,this);
	return pComboBox->GetSelection()==0;
}

// ���ÿ�ƽ��ģʽ��0 ���֣�1 ƽ��2 ƽ��
void COrderInputPanel_kq::SetOpenClose(int nOCMode)
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OIKQ_OPENCLOSE_COMBOBOX,this);
	pComboBox->SetSelection(nOCMode);
}

// ��ÿ�ƽ��ģʽ��0 ���֣�1 ƽ��2 ƽ��
int COrderInputPanel_kq::GetOpenClose()
{
	wxOwnerDrawnComboBox* pComboBox=(wxOwnerDrawnComboBox*)FindWindowById(ID_OIKQ_OPENCLOSE_COMBOBOX,this);
	return pComboBox->GetSelection();
}

void COrderInputPanel_kq::SetPriceText(wxString& strPrice)
{
	wxWindow* window=FindWindowById(ID_OIKQ_PRICE_TEXT, this);
	window->SetLabel(strPrice);
}

wxString COrderInputPanel_kq::GetPriceText()
{
	wxWindow* window=FindWindowById(ID_OIKQ_PRICE_TEXT, this);
	return window->GetLabel();
}

void COrderInputPanel_kq::SetPrice(double fPrice)
{
	wxWindow* window=FindWindowById(ID_OIKQ_PRICE_TEXT,this);
	wxString str;
	if(fPrice==-1||util::isInvalidValue(fPrice)) 
		str="-";
	window->SetLabel(Double2String(fPrice));
}

double COrderInputPanel_kq::GetPrice()
{
	wxWindow* window=FindWindowById(ID_OIKQ_PRICE_TEXT,this);
	wxString str=window->GetLabel();
	double t=0;
	str.ToDouble(&t);
	return t;
}

void COrderInputPanel_kq::SetQty(int qty)
{
	wxString strText;
	strText.Printf("%d", qty);
    wxWindow* window=FindWindowById(ID_OIKQ_VOLUME_TEXT,this);
	window->SetLabel(strText);
}

int COrderInputPanel_kq::GetQty()
{
	wxString strText;
    wxWindow* window=FindWindowById(ID_OIKQ_VOLUME_TEXT,this);
	strText = window->GetLabel();
	return atoi(strText.c_str());
}

// �����Ƿ�ֵ
void COrderInputPanel_kq::SetHedge(BOOL bIsHedge)
{
   wxCheckBox* pCheckBox=(wxCheckBox*)FindWindowById(ID_OIKQ_HEDGE_CHECKBOX,this);
   pCheckBox->SetValue(bIsHedge==TRUE);
}

BOOL COrderInputPanel_kq::GetHedge()
{
   wxCheckBox* pCheckBox=(wxCheckBox*)FindWindowById(ID_OIKQ_HEDGE_CHECKBOX,this);
   return pCheckBox->GetValue();
}

// �����Զ���ƽģʽ
void COrderInputPanel_kq::SetAutoOpenClose(BOOL bIsAuto)
{
   ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OIKQ_AUTOOPENCLOSE_STATIC,this);
   pSText->SetNormal(!bIsAuto);
}

BOOL COrderInputPanel_kq::GetAutoOpenClose()
{
   ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OIKQ_AUTOOPENCLOSE_STATIC,this);
   return !pSText->IsNormal();
}

// ���ü۸����ģʽ
void COrderInputPanel_kq::SetAutoTrackPrice(BOOL bIsTrack)
{
   ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OIKQ_AUTOTRACKPRICE_STATIC,this);
   pSText->SetNormal(!bIsTrack);
}

BOOL COrderInputPanel_kq::GetAutoTrackPrice()
{
   ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(ID_OIKQ_AUTOTRACKPRICE_STATIC,this);
   return !pSText->IsNormal();
}

void COrderInputPanel_kq::InitOrderView()
{
    wxString strText;
	SetInstrumentId(strText);
    SetDirection(TRUE);
	SetOpenClose(0);		// ���ÿ�ƽ��ģʽ��0 ���֣�1 ƽ��2 ƽ��
    SetPriceText(strText);
    SetQty(0);				// ��������
	SetHedge(FALSE);		// �����Ƿ�ֵ
}

void COrderInputPanel_kq::ClearViewData(int nMode)
{
	wxString strText;
	//1,�µ�������µ���2,ȫ�����3,��ռ۸������4ֻ�������
	switch(nMode) {
	case 2:
		SetInstrumentId(strText);
		//if(sm_nViewStyle==3)		// ����ǽ��˴���
		//	SetPanelItemText(ID_CMBLONGCODE, strText);
		SetDirection(TRUE);
		SetOpenClose(0);
		FindWindowById(ID_OIKQ_PRICE_TEXT, this)->SetLabel(wxT(""));
		FindWindowById(ID_OIKQ_VOLUME_TEXT, this)->SetLabel(wxT(""));
		break;
	case 3:
		FindWindowById(ID_OIKQ_PRICE_TEXT, this)->SetLabel(wxT(""));
		FindWindowById(ID_OIKQ_VOLUME_TEXT, this)->SetLabel(wxT(""));
		break;
	case 4:
		FindWindowById(ID_OIKQ_VOLUME_TEXT, this)->SetLabel(wxT(""));
		break;
	};
}

void COrderInputPanel_kq::FillOrder(PlatformStru_InputOrder& req)
{
     
	memset(&req, 0, sizeof(req));

	//�����˺�
	wxString strAccount = GetAccount();
	strncpy(req.strAccount, strAccount.c_str(), sizeof(req.strAccount)-1);

	//��Լ����
	wxString strName = GetInstrumentId();
	strncpy(req.InstrumentID, strName.c_str(), sizeof(req.InstrumentID)-1);

	//��������
	wxOwnerDrawnComboBox* pCmbBuySell = (wxOwnerDrawnComboBox*)FindWindowById(ID_OIKQ_BUYSELL_COMBOBOX, this);
	req.Direction = (pCmbBuySell->GetSelection()==0 ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell);

	wxTextCtrl* pTxtPrice = (wxTextCtrl*)FindWindowById(ID_OIKQ_PRICE_TEXT, this);
	wxString strPriceText = pTxtPrice->GetLabel();

	double fPrice = 0.0f;
	BOOL bIsMarketPrice = FALSE;

	if(strPriceText.compare("�м�")==0) {
		PlatformStru_DepthMarketData field;
        memset(&field,0,sizeof(field));
		if(g_pPlatform) g_pPlatform->GetQuotInfo(strName.c_str(), field);
		bIsMarketPrice=TRUE;
		if(req.Direction==THOST_FTDC_D_Buy) {
            fPrice = field.UpperLimitPrice;
		}
		else {
            fPrice = field.LowerLimitPrice;
		}
	}
	else {
		bIsMarketPrice=FALSE;
		fPrice=atof(strPriceText.c_str());
	}

	//�����۸�����
	if(bIsMarketPrice) {		/*�м۵�*/
		PlatformStru_InstrumentInfo outData;
		memset(&outData,0,sizeof(outData));
		CfgMgr* pMgr = CFG_MGR_DEFAULT();
		if(pMgr == NULL) return;
		LPORDER_CFG p = pMgr->GetOrderCfg();
		
		if(g_pPlatform) g_pPlatform->GetInstrumentInfo(req.InstrumentID, outData);

		if((!strcmp(outData.ExchangeID,"SHFE")) && (p->bSimulate)) {	//��������֧���м۵������޼۵�ģ��
			
			req.LimitPrice = fPrice;

			req.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
			//��Ч������
			req.TimeCondition=THOST_FTDC_TC_GFD;
		}
		else {
			req.OrderPriceType=THOST_FTDC_OPT_AnyPrice;
			//�۸�
			req.LimitPrice=0.0f;
			//��Ч������
			req.TimeCondition=THOST_FTDC_TC_IOC;
		}
	}
	else {	/*�޼۵�*/
		req.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
		//�޼ۼ۸�
		req.LimitPrice=fPrice;
		//��Ч������
		req.TimeCondition=THOST_FTDC_TC_GFD;
	}

	wxOwnerDrawnComboBox* pCmbOC = (wxOwnerDrawnComboBox*)FindWindowById(ID_OIKQ_OPENCLOSE_COMBOBOX, this);
	//��ƽ��־
	switch(pCmbOC->GetSelection()) {
	case 0:
		req.CombOffsetFlag[0]=THOST_FTDC_OF_Open;
		break;
	case 1: 
		req.CombOffsetFlag[0]=THOST_FTDC_OF_CloseToday;//ƽ��
		break;
	case 2:
		req.CombOffsetFlag[0]=THOST_FTDC_OF_Close;//ƽ��
		break;
	};

	wxCheckBox* pChkHedge = (wxCheckBox*)FindWindowById(ID_OIKQ_HEDGE_CHECKBOX, this);
	//���Ͷ���ױ���־��ֻ�е�һ���ֽ���Ч
	if(!pChkHedge->GetValue())
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;
	else
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Hedge;

	////�޼ۼ۸�
	//req.LimitPrice=GetContractPrice();
	//����
	req.VolumeTotalOriginal=GetQty();
	//�ɽ�������
	req.VolumeCondition = THOST_FTDC_VC_AV; 
	//��С�ɽ���
	req.MinVolume=1;
	//��������
	req.ContingentCondition=THOST_FTDC_CC_Immediately; 
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;

}

// �����ݽṹ�ж����ݲ���ʾ
void COrderInputPanel_kq::ShowOrder(PlatformStru_InputOrder& req)
{
	wxString strText;

	LockForSetChange();

	strText=req.InstrumentID;
	SetInstrumentId(strText);
	SetDirection(req.Direction==THOST_FTDC_D_Buy? 1 : 0);
	SetOpenClose(req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? 0 :
		(req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? 1 : 2));		// ���ÿ�ƽ��ģʽ��0 ���֣�1 ƽ��2 ƽ��
	if(req.OrderPriceType==THOST_FTDC_OPT_AnyPrice) {
		strText=_("�м�");
		SetPriceText(strText);
	}
	else
		SetPrice(req.LimitPrice);
	SetQty(req.VolumeTotalOriginal);							// ��������
	SetHedge(req.CombHedgeFlag[0]==THOST_FTDC_HF_Hedge);		// �����Ƿ�ֵ

	UnlockForSetChange();
}

void COrderInputPanel_kq::SetSellBuyColor()
{
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	wxColor BuyColor=DEFAULT_COLOUR;
	wxColor SellColor=wxColor(0,128,255);
	if(pCfgMgr) {
		LPORDER_CFG	pOrder=pCfgMgr->GetOrderCfg();
		if(pOrder) {
			BuyColor=pOrder->BuyColor;
			SellColor=pOrder->SellColor;
		}
	}
	wxColor clr=(GetDirection())?BuyColor:SellColor;
	m_colorBak = clr;

	wxWindow* pWin = NULL;
	for(int i=ID_OIKQ_STATIC1; i<SUMNUM_OIKQ_ID; i++) {
		pWin = FindWindowById(i, this);
		if(pWin) pWin->SetBackgroundColour(m_colorBak);
	}
	//pWin = FindWindowById(ID_OIKQ_BUYSELL_COMBOBOX, this);
	//if(pWin) pWin->SetBackgroundColour(m_colorBak);

	//SetBackgroundColour(clr);
	Refresh();

}

void COrderInputPanel_kq::OnInstrumentIdTextChanged(wxCommandEvent& event)
{
	
	if(!m_bIsInitOK)
		return;

	if(m_bIsLockForSetChange)
		return;

	wxWindow* pWin = FindWindowById(ID_OIKQ_INSTRUMENTID_COMBOCTRL, this);
	if(pWin != NULL) pWin->SetLabel(event.GetString());
	//wxComboBox* pCmbLongCode = (wxComboBox*)FindWindowById(ID_OIKQ_LONGCODE_COMBOBOX);
	//pCmbLongCode->SetValue(event.GetString());

	// һ����ø������OnInstrumentIdTextChanged()�����������Լ����ı��¼�


	//// �������ֿ�ݼ�
	//// �����xml�ж��Ƿ�Ҫȷ�ϵ�����
	//CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	//if(pMgr != NULL) {
	//	std::vector<NumberShortKey>* pVec = pMgr->GetNumberShortKeyVector();

	//	wxString strInput = event.GetString();
	//	for(int i=0; i<(int)pVec->size(); i++) {
	//		NumberShortKey& tSKey = (*pVec)[i];
	//		if(strInput.compare(tSKey.ShortKey.c_str())==0) {
	//			//m_bNoKeyChangeText = FALSE;
	//			wxString strSymbol = tSKey.Symbol.c_str();
	//			SetInstrumentId(strSymbol);
	//			event.SetString(strSymbol);
	//			//m_bNoKeyChangeText = TRUE;
	//			break;
	//		}
	//	}
	//}


	// �򸸴��巢�ͺ�Լ����ı��¼�
	wxCommandEvent evtOut(wxEVT_KQPANEL_INSTRUMENTID_CHANGED, event.GetId());
	evtOut.SetString(event.GetString());
	if(GetParent())
		GetParent()->ProcessEvent(evtOut);
	
	// ����ƴ��ת��
	if(NeedInputAbbreviation() && m_bNoKeyChangeText) {
		wxString strText = event.GetString();
		if(strText.Length()>0) {
			wxWindow *objRect =FindWindowById(ID_OIKQ_INSTRUMENTID_TEXT, this);
			wxPoint pos = objRect->GetPosition();
			pos = ClientToScreen(pos);
			wxSize sz = objRect->GetSize();
			pos.y+=sz.y;
			m_MouseHandyPopupPane->SetPosition(pos);
			if(!m_MouseHandyPopupPane->IsShown()) {
				m_MouseHandyPopupPane->Show();
			}
			m_MouseHandyPopupPane->ShowView(strText, FALSE);
			set<string> InstrumentIDArray;
			InstrumentIDArray.clear();
			if(g_pPlatform) g_pPlatform->GetInstrumentList(InstrumentIDArray);
			if(InstrumentIDArray.find(strText.c_str()) != InstrumentIDArray.end()) {
				m_MouseHandyPopupPane->Show(false);
			}
		}
		else {
			m_MouseHandyPopupPane->Show(false);
		}
		FindWindowById(ID_OIKQ_INSTRUMENTID_TEXT)->SetFocus();
	}

}

void COrderInputPanel_kq::OnTextMaxLen(wxCommandEvent& event)
{
	int nLen = event.GetInt();
}

void COrderInputPanel_kq::OnHookKeyPress(wxKeyEvent& evt)
{

	wxSpinEvent evtSpin;
	int thisID = evt.GetId();
	if(evt.GetKeyCode()==WXK_RETURN || evt.GetKeyCode()==VK_RETURN) {
		if(thisID == ID_OIKQ_INSTRUMENTID_TEXT && m_MouseHandyPopupPane->IsShown()) {
			m_MouseHandyPopupPane->Show(false);
			evt.Skip();
			return;
		}
		if(thisID == ID_OIKQ_PRICE_TEXT && m_MouseHandyPopupPaneDigit->IsShown()) {
			m_MouseHandyPopupPaneDigit->Show(false);
			evt.Skip();
			return;
		}
	}
	else if(evt.GetKeyCode()==WXK_DOWN || evt.GetKeyCode()==VK_DOWN) {
		if(thisID == ID_OIKQ_INSTRUMENTID_TEXT && m_MouseHandyPopupPane->IsShown()) {
			m_MouseHandyPopupPane->SetFocus();
			return;
		}
	}
	else if(evt.GetKeyCode()==WXK_LEFT || evt.GetKeyCode()==VK_LEFT) {
		wxOwnerDrawnComboBox* itemCombo = NULL;
		switch(thisID) {
		case ID_OIKQ_INSTRUMENTID_TEXT:
			itemCombo = wxDynamicCast(FindWindowById(ID_OIKQ_INSTRUMENTID_COMBOCTRL, this),wxOwnerDrawnComboBox);
			if(itemCombo != NULL) {
				int CurSel=0;
				BOOL bIsFound=FALSE, bIsEequal=FALSE;
				wxString strValue=GetInstrumentId();
				for(CurSel=0;CurSel<(int)itemCombo->GetCount();CurSel++) {
					if(itemCombo->GetString(CurSel).Find(strValue)==0) {
						bIsFound = TRUE;
						bIsEequal = itemCombo->GetString(CurSel).compare(strValue)==0;
						break;
					}
				}
				if(strValue.Length()==0) {
					bIsFound=TRUE;
					CurSel=0;
				}
				if(bIsFound) {
					m_bIsLockForSetChange=TRUE;
					if(bIsEequal) {
						itemCombo->SetSelection(CurSel>0 ? CurSel-1 : 0);
					}
					else {
						itemCombo->SetSelection(CurSel);
					}
					SetInstrumentId(itemCombo->GetValue());
					m_bIsLockForSetChange=FALSE;
					//wxComboBox* pCmbLongCode = (wxComboBox*)FindWindowById(ID_OIJSD_LONGCODE_COMBOBOX);
					//pCmbLongCode->SetValue(itemCombo->GetValue());
					// ��Ҫ���ú�Լ����ı��¼�
					// �򸸴��巢�ͺ�Լ����ı��¼�
					wxCommandEvent evtOut(wxEVT_KQPANEL_INSTRUMENTID_CHANGED, ID_OIKQ_INSTRUMENTID_TEXT);
					evtOut.SetString(itemCombo->GetValue());
					if(GetParent())
						GetParent()->ProcessEvent(evtOut);
				}
				else {
					evt.Skip();
				}
			}
			return;
			break;
		case ID_OIKQ_PRICE_TEXT:
			evtSpin.SetId(ID_OIKQ_PRICE_SPINCTRL);
			OnPriceSpinDown(evtSpin);
			return;
			break;
		case ID_OIKQ_VOLUME_TEXT:
			evtSpin.SetId(ID_OIKQ_VOLUME_SPINCTRL);
			OnVolumeSpinDown(evtSpin);
			return;
			break;
		};
	}
	else if(evt.GetKeyCode()==WXK_RIGHT || evt.GetKeyCode()==VK_RIGHT) {
		wxOwnerDrawnComboBox* itemCombo = NULL;
		int thisID = evt.GetId();
		switch(thisID) {
		case ID_OIKQ_INSTRUMENTID_TEXT:
			itemCombo = wxDynamicCast(FindWindowById(ID_OIKQ_INSTRUMENTID_COMBOCTRL, this),wxOwnerDrawnComboBox);
			if(itemCombo != NULL) {
				int CurSel=0;
				BOOL bIsFound=FALSE, bIsEequal=FALSE;
				wxString strValue=GetInstrumentId();
				for(CurSel=0;CurSel<(int)itemCombo->GetCount();CurSel++) {
					if(itemCombo->GetString(CurSel).Find(strValue)==0) {
						bIsFound = TRUE;
						bIsEequal = itemCombo->GetString(CurSel).compare(strValue)==0;
						break;
					}
				}
				if(strValue.Length()==0) {
					bIsFound=TRUE;
					CurSel=0;
				}
				if(bIsFound) {
					m_bIsLockForSetChange=TRUE;
					if(bIsEequal) {
						itemCombo->SetSelection(
								CurSel<(int)itemCombo->GetCount()-1 ? CurSel+1 : (int)itemCombo->GetCount()-1);
					}
					else {
						itemCombo->SetSelection(CurSel);
					}
					SetInstrumentId(itemCombo->GetValue());
					m_bIsLockForSetChange=FALSE;
					//wxComboBox* pCmbLongCode = (wxComboBox*)FindWindowById(ID_OIJSD_LONGCODE_COMBOBOX);
					//pCmbLongCode->SetValue(itemCombo->GetValue());
					// ��Ҫ���ú�Լ����ı��¼�
					// �򸸴��巢�ͺ�Լ����ı��¼�
					wxCommandEvent evtOut(wxEVT_KQPANEL_INSTRUMENTID_CHANGED, ID_OIKQ_INSTRUMENTID_TEXT);
					evtOut.SetString(itemCombo->GetValue());
					if(GetParent())
						GetParent()->ProcessEvent(evtOut);
				}
				else {
					evt.Skip();
				}
			}
			return;
			break;
		case ID_OIKQ_PRICE_TEXT:
			evtSpin.SetId(ID_OIKQ_PRICE_SPINCTRL);
			OnPriceSpinUp(evtSpin);
			return;
			break;
		case ID_OIKQ_VOLUME_TEXT:
			evtSpin.SetId(ID_OIKQ_VOLUME_SPINCTRL);
			OnVolumeSpinUp(evtSpin);
			return;
			break;
		};
	}
	else {
		int thisID = evt.GetId();

		// �����xml�ж��Ƿ�Ҫȷ�ϵ�����
		CfgMgr* pMgr = CFG_MGR_DEFAULT();  
		if(pMgr != NULL) {
			LPORDER_CFG p = pMgr->GetOrderCfg();
			int nSrcChar = evt.GetKeyCode();
			int nChar = nSrcChar;
			if(nSrcChar >= VK_NUMPAD0 && nSrcChar <= VK_NUMPAD9)
				nChar = nSrcChar - VK_NUMPAD0 + '0';
			if(nSrcChar >= WXK_NUMPAD0 && nSrcChar <= WXK_NUMPAD9)
				nChar = nSrcChar - WXK_NUMPAD0 + '0';

			if(thisID == ID_OIKQ_BUYSELL_COMBOBOX) {
				if(nChar==p->cBuyHotKey) {
					SetDirection(TRUE);
					return;
				}
				else if(nChar==p->cSelHotKey) {
					SetDirection(FALSE);
					return;
				}
			}
			else if(thisID == ID_OIKQ_OPENCLOSE_COMBOBOX) {
				if(nChar==p->cOpenOrderHotKey) {
					SetOpenClose(0);
					return;
				}
				else if(nChar==p->cCloseTodayVolumnHotKey) {
					SetOpenClose(1);
					return;
				}
				else if(nChar==p->cCloseOrderHotKey) {
					SetOpenClose(2);
					return;
				}
			}
		}
	}
	evt.Skip();

}

void COrderInputPanel_kq::OnStaticLeftUp(wxCommandEvent& event)
{
	ToggleStaticText* pSText=(ToggleStaticText*)FindWindowById(event.GetId(), this);
	if(event.GetId()==ID_OIKQ_AUTOOPENCLOSE_STATIC) {
		// �򸸴��巢���Զ���ƽ�ָı��¼�
		wxCommandEvent evtOut(wxEVT_KQPANEL_AUTOOPENCLOSE_CHANGED, event.GetId());
		evtOut.SetInt(GetAutoOpenClose());
		if(GetParent())
			GetParent()->AddPendingEvent(evtOut);
	}
	else if(event.GetId()==ID_OIKQ_AUTOTRACKPRICE_STATIC) {
		// �򸸴��巢���Զ����ټ۸�ı��¼�
		wxCommandEvent evtOut(wxEVT_KQPANEL_AUTOTRACKPRICE_CHANGED, event.GetId());
		evtOut.SetInt(GetAutoTrackPrice());
		if(GetParent())
			GetParent()->AddPendingEvent(evtOut);
	}

}

void COrderInputPanel_kq::OnPriceSpinUp(wxSpinEvent& event)
{
    double fPrice=GetPrice();
    SetPrice(fPrice+m_fPriceTick);
}

void COrderInputPanel_kq::OnPriceSpinDown(wxSpinEvent& event)
{
    double price=GetPrice();
    SetPrice(price-m_fPriceTick);
}

void COrderInputPanel_kq::OnVolumeSpinUp(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume<9999)
		nVolume++;
	SetQty(nVolume);
}

void COrderInputPanel_kq::OnVolumeSpinDown(wxSpinEvent& event)
{
	int nVolume = GetQty();
	if(nVolume>0)
		nVolume--;
	SetQty(nVolume);
}

void COrderInputPanel_kq::OnCmbBuySell(wxCommandEvent& evt)
{
	int nSel = evt.GetSelection();
	SetSellBuyColor();
	// �򸸴��巢���Զ����ټ۸�ı��¼�
	wxCommandEvent evtOut(wxEVT_KQPANEL_BUYSELLSEL_CHANGED, evt.GetId());
	evtOut.SetInt(GetDirection());
	if(GetParent())
		GetParent()->AddPendingEvent(evtOut);
}

void COrderInputPanel_kq::OnCmbOpenClose(wxCommandEvent& evt)
{
	SetAutoOpenClose(FALSE);
}

void COrderInputPanel_kq::OnCodeClicked(wxMouseEvent& evt)//wxMouseEvent
{
	wxCommandEvent evttmp(wxEVT_CODECLICKED, evt.GetId());
	::wxPostEvent(this, evttmp);
	evt.Skip();
}

void COrderInputPanel_kq::OnPriceClicked(wxMouseEvent& evt)
{
	wxCommandEvent evttmp(wxEVT_PRICECLICKED, evt.GetId());
	::wxPostEvent(this, evttmp);
	evt.Skip();
}

void COrderInputPanel_kq::OnVolumeClicked(wxMouseEvent& evt)
{
	wxCommandEvent evttmp(wxEVT_VOLUMECLICKED, evt.GetId());
	::wxPostEvent(this, evttmp);
	evt.Skip();
}

void COrderInputPanel_kq::OnNextCodeClicked(wxCommandEvent& evt)
{
	wxString strText;
	if(NeedMouseHandyPane_Code()) {
		try {
			wxWindow *objRect =FindWindowById(evt.GetId());
			wxPoint pos = ClientToScreen(objRect->GetPosition());
			wxSize sz = objRect->GetSize();
			pos.y+=sz.y;
			m_MouseHandyPopupPane->SetPosition(pos);
			m_MouseHandyPopupPane->Show(true);
			SetWindowPos((HWND)m_MouseHandyPopupPane->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE); 
			m_MouseHandyPopupPane->ShowView(strText, TRUE);
			objRect->SetFocus();
		}
		catch(...) {}
	}
	evt.Skip();
}

void COrderInputPanel_kq::OnNextPriceClicked(wxCommandEvent& evt)
{

	if(NeedMouseHandyPane_Price()) {
		try {
			wxWindow *objRect =FindWindowById(evt.GetId());
			wxRect rect = objRect->GetRect();
			wxPoint pos = ClientToScreen(wxPoint(rect.x,rect.y));
			wxSize sz = objRect->GetSize();
			pos.y+=sz.y;
			m_MouseHandyPopupPaneDigit->SetPosition(pos);
			m_MouseHandyPopupPaneDigit->Show(true);
			SetWindowPos((HWND)m_MouseHandyPopupPaneDigit->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE); 
			objRect->SetFocus();
		}
		catch(...) {}
	}
	evt.Skip();
}

void COrderInputPanel_kq::OnNextVolumeClicked(wxCommandEvent& evt)
{
	if(NeedMouseHandyPane_Volume()) {
		try {
			wxWindow *objRect =FindWindowById(evt.GetId());
			wxRect rect = objRect->GetRect();
			wxPoint pos = ClientToScreen(wxPoint(rect.x,rect.y));
			wxSize sz = objRect->GetSize();
			m_MouseHandyPopupPaneVolume->Position(pos, wxSize(0,sz.y) );
			m_MouseHandyPopupPaneVolume->Popup();
		}
		catch(...) {}
	}
	evt.Skip();
}

void COrderInputPanel_kq::OnMouseInputCode(wxCommandEvent& evt)
{
	wxString strText = evt.GetString();
	m_bNoKeyChangeText = FALSE;
	SetInstrumentId(evt.GetString(), TRUE);
	m_bNoKeyChangeText = TRUE;
	FindWindowById(ID_OIKQ_INSTRUMENTID_TEXT)->SetFocus();
	evt.Skip();
}

void COrderInputPanel_kq::OnMouseInputDigit(wxCommandEvent& evt)
{

	int nMode = 0;
	char cKeyCode = 0;
	wxString strText = evt.GetString();
	sscanf_s(strText.c_str(), "%d,%c", &nMode, &cKeyCode);

	switch(nMode) {
	case 0:
	{
		FindWindowById(ID_OIKQ_PRICE_TEXT, this)->SetFocus();
		INPUT tInput={0};
		tInput.type = INPUT_KEYBOARD;
		tInput.ki.wVk = cKeyCode;//VK_RETURN;
		tInput.ki.wScan = 0;
		tInput.ki.dwFlags = 0;//KEYEVENTF_KEYUP;
		//tInput.ki.time = ;
		//tInput.ki.dwExtraInfo = 0;
		// �������������갴���¼�
		SendInput(1, &tInput, sizeof(INPUT));
	}
		break;
	case 1:
	{
		wxString strText(wxT(""));
		SetPriceText(strText);
	}
		break;
	case 2:
	{
		wxString strText(wxT("�м�"));
		SetPriceText(strText);
	}
		break;
	};

//	wxString strText = evt.GetString();
//	if(strText[0] == '$') {
//		wxString strText(wxT("�м�"));
//		SetPriceText(strText);
//	}
//	else {
//		SetPriceText(evt.GetString());
//	}
//	evt.Skip();

}

void COrderInputPanel_kq::OnMouseInputVolume(wxCommandEvent& evt)
{
	wxString strText = evt.GetString();
	SetQty(atoi(evt.GetString().c_str()));
	evt.Skip();
}

void COrderInputPanel_kq::OnCodeTextKeyAscii(wxKeyEvent& evt)
{
////	if(NeedMouseHandyPane_Volume()) {
//		wxString strText = GetPanelItemText(ID_CONTRANCTNAME);
//		if(evt.GetKeyCode() != '\b') {
//			strText += (char)(evt.GetKeyCode());
//		}
//		try {
//			wxWindow *objRect =FindWindowById(evt.GetId());
//			wxPoint pos = ClientToScreen(wxPoint(60, 3));
//			wxSize sz = objRect->GetSize();
//			m_MouseHandyPopupPane->Position(pos, wxSize(0,sz.y) );
//			m_MouseHandyPopupPane->ShowKeyInput(strText);
//			if(strText.length()>0) {
//				m_MouseHandyPopupPane->Show(true);
//			}
//			else {
//				wxWindow* window=FindWindowById(ID_CONTRANCTNAME,this);
//				m_MouseHandyPopupPane->Popup(window);
//			}
//		}
//		catch(...) {}
////	}
	evt.Skip();
}

void COrderInputPanel_kq::OnCodePanelKeyAscii(wxKeyEvent& evt)
{
	evt.Skip();
}

void COrderInputPanel_kq::OnVolumePanelKeyAscii(wxKeyEvent& evt)
{
	evt.Skip();
}

void COrderInputPanel_kq::OnPricePanelKeyAscii(wxKeyEvent& evt)
{
	evt.Skip();
}

void COrderInputPanel_kq::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
	wxRect rect(0,0,ORDERPANEL_WIDTH,26*5+10);
	dc.SetPen(wxPen(wxColour(172,168,153)));
	dc.SetBrush(wxBrush(m_colorBak));
	dc.DrawRectangle(rect);
	dc.SetPen(wxPen(wxColour(255,255,255)));
	dc.DrawLine(1,1,1,26*5+10-1);
	//dc.GradientFillLinear(rect,m_colorBak,m_colorBak);
}

void COrderInputPanel_kq::OnCfgChanged(wxCommandEvent& evt)
{
	SetSellBuyColor();

	ToggleStaticText* itemText = (ToggleStaticText*)
			FindWindowById(ID_OIKQ_AUTOOPENCLOSE_STATIC);
	// �����xml�ж��Ƿ�Ҫȷ�ϵ�����
	CfgMgr* pMgr =  CFG_MGR_DEFAULT();  
	if(pMgr != NULL) {
		LPORDER_CFG p = pMgr->GetOrderCfg();
		// ���ý����ϵ��Զ���ƽ��ť
		// ������Ĭ���Զ���ƽ
		itemText->SetNormal(!p->bAutoOpenClose);
	}
	evt.Skip();
}

void COrderInputPanel_kq::ReloadInstrumentIDList()
{
	wxArrayString strArrCodeList;
	std::set<std::string> outData;
	// ��ȡ��Լ�б�
	outData.clear();
	if(g_pPlatform) g_pPlatform->GetInstrumentList(outData);
	std::set<std::string>::iterator it=outData.begin();
	for(; it!=outData.end(); ++it) {
		strArrCodeList.Add(*it);
	}
	wxOwnerDrawnComboBox* pComboBox = (wxOwnerDrawnComboBox*)FindWindowById(ID_OIKQ_INSTRUMENTID_COMBOCTRL, this);
	if(pComboBox) {
		pComboBox->Clear();
		for(int i=0; i<(int)strArrCodeList.size(); i++) {
			pComboBox->Insert(wxString(strArrCodeList[i]), i);
		}
	}

}

void COrderInputPanel_kq::ResetAccountList(vector<string>& accountList)
{
	wxOwnerDrawnComboBox* pComboBox = (wxOwnerDrawnComboBox*)FindWindowById(ID_OIKQ_ACCOUNT_TEXT, this);
	if(pComboBox != NULL) {
		pComboBox->Clear();
		for(int i=0; i<(int)accountList.size(); i++) {
			pComboBox->Insert(accountList[i], i);
			pComboBox->Select(0);
		}
	}
}