#pragma once

#include "../inc/Module-Misc/tools_util.h"
#include "cfgMgr/CfgMgr.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Order-Common/COrderServiceThread.h"
#include "../inc/Module-Misc/ExtListCtrl.h"

// ��Ҫʱ��������CQueryOrderPositionPanel��ΪCQueryOrderStopPanel������Ǹ������ֹӯֹ��ĸ���
class CQueryOrderPositionPanel: public wxPanel
{
public:
	enum {	//GUI Enum Control ID Start
			ID_QUERYPOSITION_BUTTONQUERY = 1800,
			ID_QUERYPOSITION_BUTTONEDIT,
			ID_QUERYPOSITION_BUTTONDELETE,
			ID_QUERYPOSITION_BUTTONCLEAR,
			ID_QUERYPOSITION_CHECKSHOWDELETE,
			////GUI Enum Control ID End
			ID_QUERYCONDITION_NUM		//ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

public:
    CQueryOrderPositionPanel(wxWindow *parent,
            wxWindowID winid = GID_QUERY_ORDER_POSITION,//
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	virtual ~CQueryOrderPositionPanel(void);

	void SetOrderServiceThread(COrderServiceThread* poOrderServiceThread) {
		m_poOrderServiceThread = poOrderServiceThread;
	};

	int i_CurOrderCnt;					//��ǰί�����͵ļ�¼��
	int i_CurOrderRows;					//��ǰί�����͵ļ�¼��

	void OnButtonQuery(wxCommandEvent& evt);
	void OnButtonEdit(wxCommandEvent& evt);
	void OnButtonDelete(wxCommandEvent& evt);
	void OnButtonClear(wxCommandEvent& evt);
	void OnLanguageChange(wxCommandEvent& evt);
	void OnCfgChanged(wxCommandEvent& evt);////�����ļ������仯
	bool Init();

private:

	void ShowOrderPositions(BOOL bIsShowDelete);
	int ShowOneOrder(int iRowNo, 
					 const COrderServiceThread::POSITIONORDER& tPositionOrder, 
					 BOOL bIsNew=TRUE);
	void OnInitSubscribe(wxCommandEvent& evt);
	
	void OnSize(wxSizeEvent& event );
	void OnContext(wxContextMenuEvent& evt);
	void OnRequery(wxCommandEvent& evt);
	void OnColumnResize(wxExtListEvent& event);
	void OnStatusChanged(wxCommandEvent& evt);
	void SaveColWidth();
	wxString GetListCtrlRowLog(wxExtListCtrl* pListCtrl, int nRow);
	wxString GetListCtrlAllRowLog(wxExtListCtrl* pListCtrl);

private:
    static  CRITICAL_SECTION m_CS_EvtCallback;

	COrderServiceThread*	m_poOrderServiceThread;
	LPLIST_TABLE_CFG		m_pListCfg;
	int m_iColIndex;
	int m_iRowCnt;

	WX_DECLARE_STRING_HASH_MAP(unsigned int, ID2RowMAP);
	std::map<int, long> m_FieldID2ColumnID; //��¼�ֶ�����һ����ʾ 
	std::map<int, LIST_COLUMN_CFG> m_FieldAttr;//��¼�ֶε���ʾ����
	std::map<int, int> m_OrderPositionKey2Row;
	std::map<int, int> m_OrderPositionRow2Key;
	
	wxButton*		m_pButtonQuery;
	wxButton*		m_pButtonEdit;
	wxButton*		m_pButtonDelete;
	wxButton*		m_pButtonClear;
	wxExtListCtrl*	m_pwxExtListCtrl;

	wxFont		m_Font;
	wxColor		m_HeadBackgroundColor;
	wxColor		m_HeadColor;
	wxColor		m_BackgroundColor;
	wxColor		m_TextColor;

DECLARE_EVENT_TABLE()
};
