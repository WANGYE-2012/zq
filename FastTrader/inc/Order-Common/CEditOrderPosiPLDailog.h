#pragma once
#include "stdafx.h"
#include <wx/spinctrl.h>
#include "CfgMgr/CfgGlobal.h"

#include "../inc/Order-Common/CLocalOrderService.h"


enum
{
	ID_EDITPOSIPL_OK=3200,
	ID_EDITPOSIPL_CANCEL,
	ID_EDITPOSIPL_CONDPRICETYPE_COMBOBOX,
	ID_EDITPOSIPL_STOPGAINPRICEOFFSET_TEXT,
	ID_EDITPOSIPL_STOPLOSEPRICEOFFSET_TEXT,
	ID_EDITPOSIPL_DOPRICEOFFSET_TEXT,
	ID_EDITPOSIPL_STOPGAIN_CHECK,
	ID_EDITPOSIPL_STOPLOSE_CHECK,
	ID_EDITPOSIPL_DOPRICEOFFSET_RADIO,
	ID_EDITPOSIPL_DOPRICELIMIT_RADIO,
	ID_EDITPOSIPL_STOPGAINPRICEOFFSET_SPINCTRL,
	ID_EDITPOSIPL_STOPLOSEPRICEOFFSET_SPINCTRL,
	ID_EDITPOSIPL_DOPRICEOFFSET_SPINCTRL,

	ID_EDITPOSIPL_POSITIONINFO_LABEL,
	ID_EDITPOSIPL_CLOSEMODE_LABEL,
	ID_EDITPOSIPL_QTY_LABEL,
	ID_EDITPOSIPL_CONDPRICE_LABEL,
//	ID_EDITPOSIPL_POSIVOLUME_LABEL,		// ƽ��/ƽ���ƽ����������ƽ��/ƽ����������ֹӯֹ������

	ID_EDITPOSIPL_DETAIL1_LABEL,
	ID_EDITPOSIPL_DETAIL2_LABEL,
	ID_EDITPOSIPL_DETAIL3_LABEL,
	
	ID_EDITPOSIPL_STATICBOX2,
	EDITPOSIPLSUMNUM_ID	// ��־ID�ŵ�����
};

class CEditOrderPosiPLDailog : public wxDialog
{
public:
	CEditOrderPosiPLDailog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr);
	~CEditOrderPosiPLDailog();

    virtual bool Show( bool show = true );

    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
	void OnCreateWindow(wxWindowCreateEvent& evt);
	void OnDestroyWindow(wxWindowDestroyEvent& evt);
	void OnDialogCharHook(wxKeyEvent& evt);
	void OnPriceSpinUp(wxSpinEvent& event);
	void OnPriceSpinDown(wxSpinEvent& event);

DECLARE_EVENT_TABLE()

public:

	void SetPLDetailOrder(CLocalOrderService::PLDETAILORDER& tPLDetailOrder) {
		m_tPLDetailOrder = tPLDetailOrder;
	};
	CLocalOrderService::PLDETAILORDER& GetPLDetailOrder() {
		return m_tPLDetailOrder;
	};

	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};

	BOOL IsAbsCondPrice() {
		return m_bIsAbsCondPrice;
	};

	void SetUpperLimitPrice(double fPrice) {
		m_fUpperLimitPrice = fPrice;
	};
	double GetUpperLimitPrice() {
		return m_fUpperLimitPrice;
	};
	void SetLowerLimitPrice(double fPrice) {
		m_fLowerLimitPrice = fPrice;
	};
	double GetLowerLimitPrice() {
		return m_fLowerLimitPrice;
	};

	// ���ֹӯ�۸���Ϣ
	BOOL GetGainOffsetPrice(double& fOffsetPrice);
	void SetGainOffsetPrice(BOOL bIsStopGain, double fOffsetPrice);
	// ���ֹ��۸���Ϣ
	BOOL GetLoseOffsetPrice(double& fOffsetPrice);
	void SetLoseOffsetPrice(BOOL bIsStopLose, double fOffsetPrice);

	// �����۸����ͣ�1 ���¼ۣ�2 �����ۣ�3 ��һ�ۣ�4 ��һ��
	int GetCondPriceType();
	void SetCondPriceType(int nPriceType);

	// ƽ�ַ�ʽ��1 �����ǵ�ͣ�ۣ�2 ����Ӽ�
	int GetCloseMode();
	void SetCloseMode(int nCloseMode);
	double GetCloseOffsetPrice();
	void SetCloseOffsetPrice(double fOffsetPrice);

	// �������ڱ���ִ�л��Ƿ�������ִ��
	void SetIsRunInServer(BOOL bIsRunInServer);
	BOOL GetIsRunInServer();

	BOOL Init(BOOL bIsAbsCond=TRUE);
	
private:
	void ShowTradeInfoDlg(const wxString& Title, const wxString& errormessage, BOOL bIsShow);
	void ShowOrderPLDetail();
	BOOL MakePosiPL(std::string& strAccount, std::string& strInstrumentID,
					BOOL bIsBuy, int nOCMode, double fPrice, int nVolume, 
					CLocalOrderService::PLDETAILORDER& tPLDetail);
	void CalcVolume();
	int GetCondVolume(BOOL bIsServ, CLocalOrderService::ORDERREF& condRef, string& strInstruemtID);
	int CalcPLCondVolume(CLocalOrderService::PLDETAILORDER& plOrder);
	int CalcCanPLVolume(BOOL bIsToday);
	wxString GetCanCloseMsg(BOOL bIsToday);
	wxString GetStaticText(UINT nUIID);
	wxString GetUserInputText();
	wxString GetReqLog();

public:
	static wxWindow*			sm_poDailog;
	static HHOOK				sm_hDailogKeyBoard;

private:
	CLocalOrderService*			m_poLocalOrderService;

	BOOL						m_bIsAbsCondPrice;
	double						m_fPriceTick;
	double						m_fUpperLimitPrice;
	double						m_fLowerLimitPrice;
	int							m_nPosiCanCloseToday;
	int							m_nPosiCanCloseYsday;
	int							m_nPosiPLCloseToday;
	int							m_nPosiPLCloseYsday;
	int							m_nCondCloseToday;
	int							m_nCondCloseYsday;

    std::vector<unsigned int>	m_TabOrderVector;
	CLocalOrderService::PLDETAILORDER		m_tPLDetailOrder;

};