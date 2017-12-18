#pragma once

#include "FundAccountQryDlg.h" //�ʽ��ѯ��� ��ʾ�Ի���
#include "cfgMgr/CfgMgr.h"
class FundAccountPanel :
    public wxScrolledWindow
{
public:
    enum
    {	
        ID_BUTTON=550,
		ID_BTN_LEFT,
		ID_BTN_RIGHT,
		ID_TIMER,     //��ʱ ��ѯ�ʽ���Ϣ,
        ID_LISTCTRL,
        ID_BASE_LINE=560,
        ID_BASE_LABEL=600,
        ID_BASE_ITEM=640,
        ID_REFRESHBUTTON=700,
        ID_ALLCALLBACK,
		ID_SELECT_ACCOUNT,
		ID_CBX_ACCOUNT,
        ID_RspQryTradingAccount
    };
    FundAccountPanel(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER|wxHSCROLL | wxVSCROLL,
            const wxString& name = wxPanelNameStr);
    ~FundAccountPanel(void);
	void OnSubscrible(wxCommandEvent& evt);
    void QryTradingAccount(wxCommandEvent& event);
	void OnBtnLeft(wxCommandEvent& event);
	void OnBtnRight(wxCommandEvent& event);
    void BankFutureTransfer(wxCommandEvent& event);
    void OnAllCallBack(wxCommandEvent& event);
    void OnCallBack_RspQryTradingAccount(wxCommandEvent& event);
    void UpdateUIData(PlatformStru_TradingAccountInfo& field);
    void UpdateUIData();
    //void OnQryReady(wxCommandEvent& evt);
    void OnCfgChanged(wxCommandEvent& evt);

    void RemoveAllControl();
    void InitHorizontalBar();
    void RePosition();      //�������¶�λ���ڳ�ʼ����OnSize�������ƺ����
    void InitTabBar();
    void SetItemValue(int listrow,wxString strTemp);
    int GetIdFromText(std::string strText);
    void RefreshUI();
    void OnSize( wxSizeEvent& event );
	void OnListConTextMenu(wxContextMenuEvent& event);
    void OnConTextMenu(wxContextMenuEvent& event);
    void OnConfig(wxCommandEvent& event);
	void OnExportList(wxCommandEvent& event);
	void DoInitQryStep();
	void OnAccountChangeSel(wxCommandEvent& event);
#ifdef _USE_MULTI_LANGUAGE
	void OnLanguageChange(wxCommandEvent& event);
#endif
	void OnStyleChange(wxCommandEvent& event);
    DECLARE_EVENT_TABLE()
public:
    //static int QryTradingAccountCallBackFunc(const AbstractBusinessData&);
    static FundAccountPanel * CreatePane(wxWindow* parent);
    static int AllCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
    static int TradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
private:
	void InitAccountCtrl(const vector<string>& vAccount);
    wxSize m_lineItem;
	FundAccountQryDlg* m_pFundAccountQryDlg;
private:
    wxColour m_TextColor;
    wxColour m_BgColor;
	//wxTimer m_timer;//��ʱ ��ѯ�ʽ���Ϣ
    wxFont m_mainfont;
    std::map<std::string,int> m_Name2Id;
    int m_layoutstyle;
	int m_row;
	int m_offset;
	std::map<int,int> m_mapPosition;
    //��ǰ
    PlatformStru_TradingAccountInfo m_AccountField;
	std::string m_strCurAccount;
    int m_StartBlockOrd;    //��ǰ��ʾ����ʼ������
    int m_BlockCount;       //�������ã���Ҫ��ʾ����Ŀ����
};
