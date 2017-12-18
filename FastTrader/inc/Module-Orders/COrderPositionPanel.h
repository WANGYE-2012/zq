#pragma once
#include "stdafx.h"
#include <wx/spinctrl.h>
#include "../inc/Module-Misc/orderDefines.h"
#include "CBaseInputPanel.h"

// ��Ҫʱ��������COrderPositionPanel��ΪCOrderStopPanel������Ǹ������ֹӯֹ��ĸ���
class COrderPositionPanel :public CBaseInputPanel
{
public:
	enum {	conROW_HEIGHT=13,
			conROW_WIDTH=12
	};

public:

    COrderPositionPanel(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL,
            const wxString& name = wxPanelNameStr);
   ~COrderPositionPanel();

	void OnPriceSpinUp(wxSpinEvent& event);
	void OnPriceSpinDown(wxSpinEvent& event);
	void OnCheckDoStopClicked(wxCommandEvent& event);
	void OnHookKeyPress(wxKeyEvent& evt);
	DECLARE_EVENT_TABLE()
public:
    static COrderPositionPanel* CreatePane(wxWindow* parent);

	void SetViewMode(int nViewMode);

	void InitPositionPanel();
	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};
	double GetPriceTick() {
		return m_fPriceTick;
	};
	// �۸�ƫ�ƶ��󣬳ɽ���/ί�м�
	int GetOffsetPriceObject();
	void SetOffsetPriceObject(int nPriceObject);
	// ���ֹ��۸���Ϣ
	BOOL GetLoseOffsetPrice(double& fOffsetPrice);
	void SetLoseOffsetPrice(BOOL bIsStopLose, double fOffsetPrice);
	// ���ֹӯ�۸���Ϣ
	BOOL GetGainOffsetPrice(double& fOffsetPrice);
	void SetGainOffsetPrice(BOOL bIsStopGain, double fOffsetPrice);
	// �����۸����ͣ�1 ���¼ۣ�2 ������
	int GetPriceType();
	void SetPriceType(int nPriceType);
	// ƽ�ַ�ʽ��1 �����ǵ�ͣ�ۣ�2 ����Ӽ�
	int GetCloseMode();
	void SetCloseMode(int nCloseMode);
	double GetCloseOffsetPrice();
	void SetCloseOffsetPrice(double fOffsetPrice);

	// �������ڱ���ִ�л��Ƿ�������ִ��
	void SetIsRunInServer(BOOL bIsRunInServer);
	BOOL GetIsRunInServer();

	void ResetLanguage();
	wxString GetUserInputText();

private:
	int		m_nViewMode;
	double	m_fPriceTick;

};