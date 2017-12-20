#pragma once

#include "COrderListPanel.h"
#include "../inc/Module-Misc/tools_util.h"
#include "../inc/Module-Misc/TradeInfoDlg.h"
#include "../Module-Misc/DataListCtrl.hpp"


#define  ALLORDER_MIN                      0        //��С
#define  ALLORDER_MAX                      ALLORDER_UserProductInfo        //���


#define  ALLORDER									'A'        //������ʾ���е�ί�е�
#define  OPENORDER									'O'        //������ʾ�ҵ���ί�е�
#define  TRADEDORDER								'T'        //������ʾ�ѳɽ���ί�е�
#define  CANCELORDER								'C'        //������ʾ�����ʹ����ί�е�
#define  TIMER_CYCLE								1000       //����ȡ��Լ��֤���ʺ���������ʱ�ļ��ʱ��




class CAllOrdersPanel : public COrderListPanel//wxPanel
{
public:
	enum { conAllOrderStyle, conOpenOrderStyle };
	enum { conNoticeNewOrderSuccess, conNoticeNewOrderFail, conNoticeTrader, conNoticeCancelSuccess, conNoticeCancelFail };

public:
    CAllOrdersPanel(int nViewStyle, wxWindow *parent,
            wxWindowID winid,//wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	virtual ~CAllOrdersPanel(void);

    void ShowAll();

private:
	std::map<OrderKey,  CThostFtdcOrderField> outData;
	OrdersPanelMAP m_OrdersPanelMap;

	BOOL NeedOrderSuccessConfirm();
	BOOL NeedOrderSuccessSound();
	BOOL NeedOrderFailConfirm();
	BOOL NeedOrderFailSound();
	BOOL NeedOrderTradeConfirm();
	BOOL NeedOrderTradeSound();
	BOOL NeedCancelSuccessConfirm();
	BOOL NeedCancelSuccessSound();
	BOOL NeedCancelFailConfirm();
	BOOL NeedCancelFailSound();
	bool NeedHideCancelButton();
	void NoticeSound(int nType);
	void OnSubscrible(wxCommandEvent& evt);
	void Init();
	void OnRspOrderAction1(wxCommandEvent& evt);
	void OnRspOrderAction2(wxCommandEvent& evt);
	void OnRcvTrade(wxCommandEvent& evt);
    void OnPanelCharHook(wxCommandEvent& evt);
    //������ѯ�ر�
    static int RspQryOrderCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
    static int RtnOrderCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	  //�����ر�
	static int RspOrderAction1CallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	  //������ִ1
	static int RspOrderAction2CallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	  //������ִ2
	static int TradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	  //�ɽ��ر�
    static int OrderInsertBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	  //����¼��ر�
    void OnCfgChanged(wxCommandEvent& evt);////�����ļ������仯
	void  PopupTradeDlg(IPlatformSingleSvr* pSvr,const PlatformStru_TradeInfo& rawData);
    void  PopupCancel_InsertDlg(IPlatformSingleSvr* pSvr,const PlatformStru_OrderInfo& rawData);
	int  UpdateActionRet(const PlatformStru_TradeInfo& rawData);
	void ShowInsertDlg(const string& Title, const DataRspOrderInsert& OrderInfo, BOOL bIsShow);

	void OnActivated(wxExtListEvent& event);
	void OnSize( wxSizeEvent& event );
	void OnRadioButton(wxCommandEvent& event);	
	void OnRemove(wxCommandEvent& evt);
    void OnRemoveAll(wxCommandEvent& evt);
    void OnReQry(wxCommandEvent& evt);
    void OnReConnectQry(wxCommandEvent& evt);
    void OnContext(wxContextMenuEvent& evt);
    void OnOrderInsert(wxCommandEvent& evt);
	void OnPanelFocus(wxCommandEvent& evt);
	void OnColumnResize(wxExtListEvent& event);

    //���ĳЩ��Լ�ķ��ʣ���Ҫ������Ӧ�ı����Ķ�����ֵ
    void OnGetCommissionRate(wxCommandEvent& evt);
    void OnGetMarginRate(wxCommandEvent& evt);
    //��֤���ʻر�
    static int GetMarginRateCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);
    //�������ʻر�
    static int GetCommissionRateCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);
    void OnRtnOrder(wxCommandEvent& evt);
    //������ѯ��Ӧ
    void OnRspQryOrder(wxCommandEvent& evt);
    static bool UpdateListItemCallBack(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem);
    static bool UpdateOpenListItemCallBack(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem);

#ifdef _USE_MULTI_LANGUAGE
	void OnLanguageChanged(wxCommandEvent& evt);
#endif

	void SaveColWidth();
	void OnMAPlatformAdd(wxCommandEvent& event);
	void OnMAPlatformDelete(wxCommandEvent& evt);
private:
	LPLIST_TABLE_CFG       m_pListCfg;
	WX_DECLARE_STRING_HASH_MAP(unsigned int, ID2RowMAP);
	std::map<int, long> m_FieldID2ColumnID; //��¼�ֶ�����һ����ʾ 


private:

	int		m_nViewStyle;
	GUIModuleID		m_nGID;

	CThostFtdcOrderField  m_OneOrderList;
	PlatformStru_TradeInfo  m_OneTradeList;

	wxButton *     m_pButtonRemove;
	wxButton *     m_pButtonRemoveAll;
    wxButton *     m_pButtonReQry;
	wxRadioButton *m_WxRadioButton1;
	wxRadioButton *m_WxRadioButton2;
	wxRadioButton *m_WxRadioButton3;
	wxRadioButton *m_WxRadioButton4;

	CDataListCtrl<OrderKey,PlatformStru_OrderInfo>*  m_pwxExtListCtrl;

	wxFont m_Font;
	wxColor m_HeadBackgroundColor;
	wxColor m_HeadColor;
	wxColor m_BackgroundColor;
	wxColor m_TextColor;
    wxColor m_EvenLineBgColor;
	wxColor m_OddLineBgColor;
    CTradeInfoDlg* m_pTradeInfoDlg;

    char            m_ShowType;          //��ǰ��ʾ���ݣ�����/�ҵ�/�ɽ�/ ����
    unsigned long   m_TextColor2;  //������ɫ

	set<IPlatformSingleSvr*> m_pSvrSet;
public:
	enum
		{	//GUI Enum Control ID Start
			ID_BUTTON_REMOVE = 1011,
			ID_BUTTON_REMOVEALL = 1012,	
			ID_BUTTON_REQRY = 1013,	
			ID_WXRADIOBUTTON1 = 1006,
			ID_WXRADIOBUTTON2 = 1007,
			ID_WXRADIOBUTTON3 = 1008,
			ID_WXRADIOBUTTON4 = 1009,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

DECLARE_EVENT_TABLE()
};


