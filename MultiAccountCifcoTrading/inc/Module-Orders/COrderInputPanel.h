#pragma once
#include "stdafx.h"
#include "CBaseInputPanel.h"
#include "CfgMgr/CfgGlobal.h"

#include "../inc/Order-Common/CMouseHandyInputPane.h"
#include "../inc/Order-Common/MouseHandyInputPaneVolume.h"
#include "../inc/Order-Common/CMouseHandyInputPaneDigit.h"
#include "../inc/Module-Misc/orderDefines.h"


class COrderInputPanel : public CBaseInputPanel//wxPanel
{
public:
	COrderInputPanel(wxWindow *parent,
			wxWindowID winid = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxTAB_TRAVERSAL | wxNO_BORDER,
			const wxString& name = wxPanelNameStr);
	~COrderInputPanel();
    
	static COrderInputPanel* CreatePane(wxWindow* parent);

//	void OnInstrumentIdListDropdown(wxCommandEvent& evt);

	void OnCmbBuySell(wxCommandEvent& evt);
	void OnCmbOpenClose(wxCommandEvent& evt);
	void OnInstrumentIdTextChanged(wxCommandEvent& event);
	void OnPriceTextChanged(wxCommandEvent& event);
	void OnPriceSpinUp(wxSpinEvent& event);
	void OnPriceSpinDown(wxSpinEvent& event);
	void OnVolumeSpinUp(wxSpinEvent& event);
	void OnVolumeSpinDown(wxSpinEvent& event);

	void OnTextMaxLen(wxCommandEvent& event);

    void OnStaticLeftUp(wxCommandEvent& event);
    void OnHookKeyPress(wxKeyEvent& evt);

	void OnCodeTextKeyAscii(wxKeyEvent& evt);

	void OnCodePanelKeyAscii(wxKeyEvent& evt);
	void OnVolumePanelKeyAscii(wxKeyEvent& evt);
	void OnPricePanelKeyAscii(wxKeyEvent& evt);

	void OnCodeClicked(wxMouseEvent& evt);
	void OnPriceClicked(wxMouseEvent& evt);
	void OnVolumeClicked(wxMouseEvent& evt);

	void OnNextCodeClicked(wxCommandEvent& evt);
	void OnNextPriceClicked(wxCommandEvent& evt);
	void OnNextVolumeClicked(wxCommandEvent& evt);
	
	void OnMouseInputCode(wxCommandEvent& evt);
	void OnMouseInputDigit(wxCommandEvent& evt);
	void OnMouseInputVolume(wxCommandEvent& evt);
	
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnCfgChanged(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE()

public:
	void SetViewMode(int nViewMode);

    void SetInstrumentId(wxString& strInstrumentId, BOOL bSendChangeEvent=FALSE);
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

	void SetViewStyle(int nViewStyle) {
		m_nViewStyle = nViewStyle;
		if(m_MouseHandyPopupPane != NULL)
			m_MouseHandyPopupPane->SetViewStyle(nViewStyle);
	};
	int GetViewStyle() {
		return m_nViewStyle;
	};

    void InitOrderView();					// ���ý���Ϊ��ʼֵ
    void ClearViewData(int nMode);					// ���ý���Ϊ��ʼֵ
    void ShowOrder(PlatformStru_InputOrder& req);		// �����ݽṹ�ж����ݲ���ʾ

	void ResetAccountList(vector<string>& accountList);
	void ResetContractList();

	void DynamicShowCtrl();
	void InsertCtrlID2TabCtrl(vector<UINT>& tabCtrlList);
	BOOL CheckCtrlID(int nCtrlID);

	void LockForSetChange() {
		m_bIsLockForSetChange = TRUE;
	};
	void UnlockForSetChange() {
		m_bIsLockForSetChange = FALSE;
	};

	void ResetLanguage();
	wxString GetUserInputText();

    void EnablePriceInputCtl(bool bEnabled);

private:
    void SetSellBuyColor();
	//BOOL PY2Code(wxString strPY, wxString& strRetCode);

	wxTextCtrl* CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit, wxSize& ctrSize);
	wxTextCtrl* CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit, wxSize& ctrSize);

	BOOL NeedMouseHandyPane_Code();		// �ж��Ƿ���Ҫ��ʾ���������������
	BOOL NeedMouseHandyPane_Volume();	// �ж��Ƿ���Ҫ��ʾ����������������
	BOOL NeedMouseHandyPane_Price();	// �ж��Ƿ���Ҫ��ʾ������������۸�
	BOOL NeedInputAbbreviation();		// ����������д��ƴ��
	BOOL NeedAutoOCHideRadio();			// ʹ���Զ�ƽ��ʱ��ȡ����ƽ��ѡ��
	int GetContractListStyle();

private:

	int		m_nViewStyle;

	BOOL	m_bIsInitOK;
	double	m_fPriceTick;
	//int		nViewSubMode;				// �����0 ������1 С��ί�У�2 �����µ���3 ��������4 ӯ��

    CMouseHandyInputPane*		m_MouseHandyPopupPane;
	MouseHandyInputPaneVolume*	m_MouseHandyPopupPaneVolume;
	CMouseHandyInputPaneDigit*	m_MouseHandyPopupPaneDigit;

	BOOL	m_bIsLockForSetChange;
	BOOL	m_bNoKeyChangeText;

	wxColor m_colorBak;

    std::vector<int>			m_arrTabCtrl;

};