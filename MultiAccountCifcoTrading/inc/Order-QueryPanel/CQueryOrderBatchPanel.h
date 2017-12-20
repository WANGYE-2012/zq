#pragma once

#include "../inc/Module-Misc/tools_util.h"
#include "Module-Misc/ExtListCtrl.h"
#include "cfgMgr/CfgMgr.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Order-Common/COrderServiceThread.h"

class CQueryOrderBatchPanel: public wxPanel
{
public:
	enum {	//GUI Enum Control ID Start
			ID_QUERYBATCH_BUTTONQUERY = 1700,
			////GUI Enum Control ID End
			ID_QUERYBATCH_NUM		//ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

public:
    CQueryOrderBatchPanel(wxWindow *parent,
            wxWindowID winid = GID_QUERY_ORDER_BATCH,//
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	virtual ~CQueryOrderBatchPanel(void);

	void SetOrderServiceThread(COrderServiceThread* poOrderServiceThread) {
		m_poOrderServiceThread = poOrderServiceThread;
	};

	void OnButtonQuery(wxCommandEvent& evt);
	void OnLanguageChange(wxCommandEvent& evt);
	void OnColumnResize(wxExtListEvent& event);
	void OnCfgChanged(wxCommandEvent& evt);////�����ļ������仯
	bool Init();

	int i_CurOrderCnt;					//��ǰί�����͵ļ�¼��
	int i_CurOrderRows;					//��ǰί�����͵ļ�¼��

private:

	void ShowOrderBatchs();
	bool GetOrderInfoByRowNo(int RowNo, DWORD& RowID, COrderServiceThread::BATCHSUBORDER& tBatchItem);
	int LoadOneOrderList(const COrderServiceThread::BATCHSUBORDER& tBatchOrder);
	void OnInitSubscribe(wxCommandEvent& evt);
	
	void OnSize( wxSizeEvent& event );
	void OnContext(wxContextMenuEvent& evt);
	void OnRequery(wxCommandEvent& evt);
	void DoInitQryStep();
	void SaveColWidth();

private:
    static  CRITICAL_SECTION m_CS_EvtCallback;

	COrderServiceThread*	m_poOrderServiceThread;
	LPLIST_TABLE_CFG		m_pListCfg;
	int m_iColIndex;
	int m_iRowCnt;

	WX_DECLARE_STRING_HASH_MAP(unsigned int, ID2RowMAP);
	std::map<int, long> m_FieldID2ColumnID; //��¼�ֶ�����һ����ʾ 
	std::map<int, LIST_COLUMN_CFG> m_FieldAttr;//��¼�ֶε���ʾ����
	//std::map<int, int> m_OrderBatchRow2Key;


	//�������ݱ����б��еı�����¼��OrderKey����ID
    //��ID��һ��˳�������Ψһֵ����ʶһ�У����к��޹�
    //added by l. 20120222
    CRITICAL_SECTION m_CS;
    DWORD m_BaseRowID;      //��9999��ʼ
    std::map<int,DWORD> m_MapOrderKey_RowID;    //BatchNum��RowID�Ķ�ӦMap
    std::map<DWORD,int> m_MapRowID_OrderKey;    //RowID��BatchNum�Ķ�ӦMap
    std::map<int, COrderServiceThread::BATCHSUBORDER> m_MapOrderKey_OrderInfo;    //BatchNum��BATCHSUBORDER�Ķ�ӦMap
    std::set<std::string> m_Set_Instruments_HaveRate;     //��÷��ʵĺ�Լ���ϣ���Ҫ������Ӧ�ı����Ķ�����ֵ

	
	wxButton*		m_pButtonQuery;
	wxExtListCtrl*	m_pwxExtListCtrl;

	wxFont		m_Font;
	wxColor		m_HeadBackgroundColor;
	wxColor		m_HeadColor;
	wxColor		m_BackgroundColor;
	wxColor		m_TextColor;

DECLARE_EVENT_TABLE()
};
