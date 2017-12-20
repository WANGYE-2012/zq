#pragma once
#include "stdafx.h"
#include "CBaseInputPanel.h"
#include "CfgMgr/CfgGlobal.h"
#include "wx/spinctrl.h"

enum
{
    ID_OIJSD_FIRST=1900,
	ID_OIJSD_INSTRUMENTID_TEXT,
	ID_OIJSD_ACCOUNT_TEXT,
    ID_OIJSD_PRICE_TEXT,
    ID_OIJSD_VOLUME_TEXT,
	ID_OIJSD_LONGCODE_COMBOBOX,
	ID_OIJSD_BUYSELL_COMBOBOX,
	ID_OIJSD_OPENCLOSE_COMBOBOX,
    ID_OIJSD_PRICE_SPINCTRL,
	ID_OIJSD_VOLUME_SPINCTRL,
	ID_OIJSD_STATIC1,
	ID_OIJSD_STATIC2,
	ID_OIJSD_STATIC3,
	ID_OIJSD_STATIC4,
	ID_OIJSD_STATIC5,
    ID_OIJSD_AUTOOPENCLOSE_STATIC,
    ID_OIJSD_AUTOTRACKPRICE_STATIC,
    ID_OIJSD_HEDGE_CHECKBOX,
	SUMNUM_OIJSD_ID	// ��־ID�ŵ�����
};

class COrderInputPanel_jsd : public CBaseInputPanel//wxPanel
{
public:
	COrderInputPanel_jsd(wxWindow *parent,
			wxWindowID winid = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxTAB_TRAVERSAL | wxNO_BORDER,
			const wxString& name = wxPanelNameStr);
	~COrderInputPanel_jsd();
    
	static COrderInputPanel_jsd* CreatePane(wxWindow* parent);

	void OnCmbBuySell(wxCommandEvent& evt);
	void OnCmbOpenClose(wxCommandEvent& evt);
	void OnInstrumentIdTextChanged(wxCommandEvent& event);
	void OnPriceSpinUp(wxSpinEvent& event);
	void OnPriceSpinDown(wxSpinEvent& event);
	void OnVolumeSpinUp(wxSpinEvent& event);
	void OnVolumeSpinDown(wxSpinEvent& event);

    void OnStaticLeftUp(wxCommandEvent& event);
    void OnHookKeyPress(wxKeyEvent& evt);
	
	void OnPaint(wxPaintEvent& event);
	void OnCfgChanged(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE()

public:
	void SetViewMode(int nViewMode);

    void SetInstrumentId(wxString& strInstrumentId, BOOL bSendChangeEvent=TRUE);
    wxString GetInstrumentId();
    void SetAccount(wxString& strAccount);
    wxString GetAccount();
    void SetDirection(BOOL bIsBuy);
	BOOL GetDirection();				// �����������TRUEΪ����
	void SetOpenClose(int nOCMode);		// ���ÿ�ƽ��ģʽ��0 ���֣�1 ƽ��2 ƽ��
	int GetOpenClose();					// ��ÿ�ƽ��ģʽ��0 ���֣�1 ƽ��2 ƽ��
    void SetPriceText(wxString& strPrice);
    wxString GetPriceText();
    void SetPrice(double price);
    double GetPrice();
	BOOL GetIsPriceMode();				// �۸��Ƿ�Ϊ�м�
    void SetQty(int qty);				// ��������
    int GetQty();						// ��ȡ����
	void SetHedge(BOOL bIsHedge);		// �����Ƿ�ֵ
	BOOL GetHedge();

	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};
	double GetPriceTick() {
		return m_fPriceTick;
	};

    void SetAutoOpenClose(BOOL bIsAuto);	// �����Զ���ƽģʽ
	BOOL GetAutoOpenClose();
    void SetAutoTrackPrice(BOOL bIsTrack);	// ���ü۸����ģʽ
	BOOL GetAutoTrackPrice();

    void InitOrderView();					// ���ý���Ϊ��ʼֵ
    void ClearViewData(int nMode);					// ���ý���Ϊ��ʼֵ
    void FillOrder(PlatformStru_InputOrder& req);		// �ѽ����������䵽���ݽṹ��
    void ShowOrder(PlatformStru_InputOrder& req);		// �����ݽṹ�ж����ݲ���ʾ
	void ReloadInstrumentIDList();

	void ResetAccountList(vector<string>& accountList);


	void LockForSetChange() {
		m_bIsLockForSetChange = TRUE;
	};
	void UnlockForSetChange() {
		m_bIsLockForSetChange = FALSE;
	};

private:
    void SetSellBuyColor();
	//BOOL PY2Code(wxString strPY, wxString& strRetCode);

	wxTextCtrl* CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit);
	wxTextCtrl* CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit);
    wxString Number2String(int num);
    wxString Double2String(double dbl);

	BOOL NeedMouseHandyPane_Code();		// �ж��Ƿ���Ҫ��ʾ���������������
	BOOL NeedMouseHandyPane_Volume();	// �ж��Ƿ���Ҫ��ʾ����������������
	BOOL NeedMouseHandyPane_Price();	// �ж��Ƿ���Ҫ��ʾ������������۸�
	BOOL NeedInputAbbreviation();		// ����������д��ƴ��
	BOOL NeedAutoOCHideRadio();			// ʹ���Զ�ƽ��ʱ��ȡ����ƽ��ѡ��

private:

	BOOL	m_bIsInitOK;
	double	m_fPriceTick;
	//int		nViewSubMode;				// �����0 ������1 С��ί�У�2 �����µ���3 ��������4 ӯ��

	BOOL	m_bIsLockForSetChange;
	BOOL	m_bNoKeyChangeText;

	wxColor m_colorBak;

};