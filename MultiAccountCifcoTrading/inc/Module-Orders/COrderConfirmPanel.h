#pragma once
#include "stdafx.h"
#include "CBaseInputPanel.h"
#include "CfgMgr/CfgGlobal.h"
#include <wx/spinbutt.h>
#include "../inc/Order-Common/CMouseHandyInputPaneDigit.h"

enum
{
    ID_ORDERCONFIRMPANEL_NEWPRICE_SPINCTRL=1900,
	ID_ORDERCONFIRMPANEL_NEWVOLUME_SPINCTRL,
	ID_ORDERCONFIRMPANEL_ISHEDGE_CHECK,
	ID_ORDERCONFIRMPANEL_ISMARKETPRICE_CHECK,
	ID_ORDERCONFIRMPANEL_OLDPRICE_STATIC,
	ID_ORDERCONFIRMPANEL_OLDVOLUME_STATIC,
	ID_ORDERCONFIRMPANEL_STATIC1,
	ID_ORDERCONFIRMPANEL_STATIC2,
	ID_ORDERCONFIRMPANEL_STATIC3,
	ID_ORDERCONFIRMPANEL_STATIC4,
	ID_ORDERCONFIRMPANEL_STATIC5,
	ID_ORDERCONFIRMPANEL_STATIC6,
	ID_ORDERCONFIRMPANEL_STATIC7,
	ID_ORDERCONFIRMPANEL_NEWPRICE_TEXT=2000,
	ID_ORDERCONFIRMPANEL_NEWVOLUME_TEXT,
	ORDERCONFIRMPANELSUMNUM_ID	// ��־ID�ŵ�����
};

class COrderConfirmPanel : public CBaseInputPanel//wxPanel
{
public:
	COrderConfirmPanel(wxWindow *parent,
			wxWindowID winid = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxTAB_TRAVERSAL | wxNO_BORDER,
			const wxString& name = wxPanelNameStr);
	~COrderConfirmPanel();
    
	static COrderConfirmPanel* CreatePane(wxWindow* parent);
    BOOL NeedMouseHandyPane_Price();

	void OnPriceSpinUp(wxSpinEvent& event);
	void OnPriceSpinDown(wxSpinEvent& event);
	void OnVolumeSpinUp(wxSpinEvent& event);
	void OnVolumeSpinDown(wxSpinEvent& event);

	void OnPriceClicked(wxMouseEvent& evt);
	void OnNextPriceClicked(wxCommandEvent& evt);
    void OnPricePanelKeyAscii(wxKeyEvent& evt);
	void OnMouseInputDigit(wxCommandEvent& evt);

    void OnHookKeyPress(wxKeyEvent& evt);
    void OnPriceTextFocus(wxFocusEvent& evt);

	void OnCfgChanged(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE()

public:

    void SetPriceText(wxString& strPrice);
    wxString GetPriceText();
    void SetPrice(double price);
    double GetPrice();
	BOOL GetIsPriceMode();				// �۸��Ƿ�Ϊ�м�
    void SetQty(int qty);				// ��������
    int GetQty();						// ��ȡ����

	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};
	double GetPriceTick() {
		return m_fPriceTick;
	};
	void SetIsAuto(BOOL bIsAuto) {
		m_bIsAuto = bIsAuto;
	};
	BOOL GetIsAuto() {
		return m_bIsAuto;
	};
	void SetIsMarketPrice(BOOL bIsMarketPrice) {
		m_bIsMarketPrice = bIsMarketPrice;
	};
	BOOL GetIsMarketPrice() {
        return stricmp(GetPriceText().c_str(), LOADSTRING(OLS_MARKET))==0;
		//return m_bIsMarketPrice;
	};

	void SetGroupIndex(int nGroupIndex) {
		m_nGroupIndex = nGroupIndex;
	}

	void CreateGUIControls();
	void InitOrderView();					// ���ý���Ϊ��ʼֵ
    void FillOrder(PlatformStru_InputOrder& ftdcField);		// �ѽ����������䵽���ݽṹ��
    void FillOrder(PlatformStru_ParkedOrder& ftdcField);		// �ѽ����������䵽���ݽṹ��
    void ShowOrder(PlatformStru_InputOrder& ftdcField);		// �����ݽṹ�ж����ݲ���ʾ
    void ShowOrder(PlatformStru_ParkedOrder& ftdcField);		// �����ݽṹ�ж����ݲ���ʾ

private:

	wxTextCtrl* CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit);
	wxTextCtrl* CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit);

private:

	double	m_fPriceTick;
	BOOL	m_bIsAuto;
	BOOL	m_bIsMarketPrice;
	int		m_nGroupIndex;
    wxString    m_strInstrumentId;

	CMouseHandyInputPaneDigit*	m_MouseHandyPopupPaneDigit;

	wxColor m_colorBak;

};