#pragma once
#include "FrameBase.h"
class zqAuiNotebook;
class wxAuiNotebookEvent;
class wxAuiManagerEvent;
class CBankFutureTransferDlg;
class DisConnPromptDlg;
class TradingNoticeDlg;
class UserNoticeDlg;
class COrderServiceThread;
class MainTaskBarIcon;
class CTrackThread;
class CBeepThread;
class PlusInMgrDlg;
class NoticeContent;
class LockFrameDlg;
struct LOG_DATA;
struct MessageInfo;
class MsgDlg;

struct USER_LOG
{
	USER_LOG()
	{
		strLogType = "";
		pData = NULL;
	}
	std::string strLogType;
	LOG_DATA* pData;
};
class MainFrame:
    public FrameBase
{
public:
    //��׼���캯��,��û�д�������
    MainFrame();
	wxString GetMainFrameCaption();
    //��ʼ������
    bool Initialize(wxWindow *parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxFrameNameStr);
    //��������
    void Uninitialize();

	void RestorePosition();

	//��׼��������
    ~MainFrame(void);

	//�麯������ֹ״̬����ʾ�˵���ʾ
     virtual void DoGiveHelp(const wxString& text, bool show);
	 //�麯�������ڲ���
	 virtual bool Layout();
	//���ڲ˵���Ӧ:�������
    void OnLockFrame(wxCommandEvent& evt);
	//��Ϣ�鿴
	void OnReadMsg(wxCommandEvent& evt);
	//���ڲ˵���Ӧ:�˳�
    void OnExit(wxCommandEvent& evt);
	//ϵͳ��ť���رա�����Ӧ
    void OnMainClose(wxCloseEvent& evt);

    //void OnTradeReport(wxCommandEvent& event);
    //void OnTradeReport_Today(wxCommandEvent& event); 
    //void OnTradeReport_Week(wxCommandEvent& event); 
    //void OnTradeReport_Month(wxCommandEvent& event); 
    void ShowConfigAppDlg(int id=-1);
	//���ڲ˵���Ӧ:ѡ������
    void OnConfigOption(wxCommandEvent& event);
	//�ı�����
	void DoChangeStyle(int cmdid);
	//�ָ�Ĭ������
	void OnApplyDefaultConfig(wxCommandEvent& event);
	//���Ͷ���ҵ���¼�
	void SendSubscribeEventToChildren(bool IsSubscribe);
	//���������ļ��ı��¼�
	void SendCfgXmlChangeEventToChildren();
	//���ͷ��ı��¼�
	void SendStyleChangeEventToChildren(int nApplyType);
    //���˴�����ջ��·���������壬�ص�����ҳ��
	void OnReturnNormalUI(wxCommandEvent& evt);
	void DoReturnNormalUI();
	//����CHAR_HOOK��ָ������
	bool TransmitCharHook(wxWindow *panel,wxKeyEvent& event);
	//������̹��ܼ�����
    void OnCharHook (wxKeyEvent& event);
    //������嵯����Ӧ
    void OnFloatPaneOpen(wxAuiNotebookEvent& event);
    //��������ջ���Ӧ
    void OnFloatPaneClose(wxAuiManagerEvent& event);
	//��������ת�˶Ի���
	void OnBankFutureTransfer(wxCommandEvent& event);
	//������ѯ�˻��Ի���
	void OnQueryTrader(wxCommandEvent& event);
	//������ѯ��Լ�Ի���
	void OnQueryContract(wxCommandEvent& event);
	//������ѯ�������Ի���
	void OnQueryExchange(wxCommandEvent& event);
	//������ѯ�������ͶԻ���
	void OnQueryOrderType(wxCommandEvent& event);
	//������ѯ��Ʒ�Ի���
	void OnQueryCommodity(wxCommandEvent& event);
	//������ѯ�ֲֶԻ���
	void OnQueryPosition(wxCommandEvent& event);
	//������ѯ����Ի���
	void OnQueryReport(wxCommandEvent& event);
	//������ѯ���ʶԻ���
	void OnQueryExchangeRate(wxCommandEvent& event);
	//�����ļ��ı���
    void OnStyleSave(wxCommandEvent& event);
	//������ı�
	void OnUpdateUIOnStyleChanged(wxUpdateUIEvent& event);
    void OnStyleChanged(wxCommandEvent& event);
	//���ڲ˵���Ӧ���޸�����
	void OnModifyPwd(wxCommandEvent& event);
	//���ڲ˵���Ӧ���޸ĵ�¼����
	void ModifyMAPwd(wxCommandEvent& event);
	//���ڲ˵���Ӧ������ʷ���㵥
	void HisCal(wxCommandEvent& event);
	//���ڲ˵���Ӧ��ʹ��˵��
    void OnHelpContent(wxCommandEvent& event);
	//���ڲ˵���Ӧ������
    void OnAbout(wxCommandEvent& event);
	//���ڲ˵���Ӧ�����ڻ��˻��ʽ�
    void OnQryAccount(wxCommandEvent& evt);
	//���ڲ˵���Ӧ:������
    //void OnConfigWizard(wxCommandEvent& evt);
	//�˵���Ӧ���Ʋ�
    void OnPositionMove(wxCommandEvent& event);
	//�˵���Ӧ������
    void OnPositionSwap(wxCommandEvent& event);
	//�˵���Ӧ���������
	void OnShowPlusInDlg(wxCommandEvent& event );
  	//��Լ����״̬֪ͨ��Ӧ 
	void OnInstrumentStatus(wxCommandEvent& evt);
	//�뽻�׺�̨ͨ�����ӶϿ��¼���Ӧ
    void OnFrontDisconnected(wxCommandEvent& evt);
	//�뽻�������ӣ��Ͽ��͵�¼��Ӧ
	void OnConnectLogin(wxCommandEvent& evt);
	//����֪ͨ�¼���Ӧ
    void OnTradingNotice(wxCommandEvent& evt);
	//��С����������Ӧ
	void OnIconize(wxIconizeEvent& evt);
	//������Ϣ��Ӧ
	void OnIdle(wxIdleEvent& event);
    //���˴�˵���Ӧ�����µ�¼
    void OnLoginAgain(wxCommandEvent& event);
	//���˴�˵���Ӧ���Զ�����
    void OnUpdateApp(wxCommandEvent& event);
    //���˴�˵���Ӧ����ѡ��Լ
    void OnConfigSelect(wxCommandEvent& event); 
	//���˴�˵���Ӧ��ί�в���
    void OnConfigOrder(wxCommandEvent& event);
	//���˴�˵���Ӧ��ϵͳ����
    void OnConfigSystem(wxCommandEvent& event);
	//���˴�˵���Ӧ���޸�����
    void OnConfigPassword(wxCommandEvent& event);
	//���˴�˵���Ӧ�����ֿ�ݼ�
    void OnConfigAcce(wxCommandEvent& event);
    //���˴�˵���Ӧ����ѡ��Լ
    void OnUpdateUIConfigSelect(wxUpdateUIEvent& event); 
	//���˴�˵���Ӧ��ί�в���
    void OnUpdateUIConfigOrder(wxUpdateUIEvent& event);
	//���˴�˵���Ӧ��ϵͳ����
    void OnUpdateUIConfigSystem(wxUpdateUIEvent& event);
	//���˴�˵���Ӧ���޸�����
    void OnUpdateUIConfigPassword(wxUpdateUIEvent& event);
	//���˴�˵���Ӧ�����ֿ�ݼ�
    void OnUpdateUIConfigAcce(wxUpdateUIEvent& event);
	//���˴�˵���Ӧ���߼�
	void OnConfigGaoji(wxCommandEvent& event);
	//���˴�˵���Ӧ����׼�µ���
	void OnOrderStandard(wxCommandEvent& event);
	//���˴�˵���Ӧ��С��ί��
    void OnOrderSmallWin(wxCommandEvent& event);
	//���˴�˵���Ӧ�������µ�
    void OnOrderBatch(wxCommandEvent& event);
	//���˴�˵���Ӧ��������
    void OnOrderCondition(wxCommandEvent& event);
	//���˴�˵���Ӧ��ӯ��
    void OnOrderProfit(wxCommandEvent& event);
	//���̲˵���Ӧ��Stop or Stop Limit Order
	void OnOrderStopLimit(wxCommandEvent& event);
	//���˴�˵���Ӧ����ѯ�����
    void OnQueryWithdraw(wxCommandEvent& event);
	//���˴�˵���Ӧ����ѯ���㵥
    void OnQuerySettlement(wxCommandEvent& event);
	//���˴�˵���Ӧ������ת��
    void OnTransferBank(wxCommandEvent& event);
	//���˴�˵���Ӧ��ԤԼ����
    void OnTransferWithdraw(wxCommandEvent& event);
    //���˴��Ҽ��˵���Ӧ������ί�е�ģ��
    void CancerlOrder(wxCommandEvent& event);
	//���˴��Ҽ��˵���Ӧ����������ģ��
    void ConditionPanelButton(wxCommandEvent& event);
	//���˴�˵���Ӧ�������ĵ�
    void OnHelpDoc(wxCommandEvent& event);
	//���˴�˵���Ӧ������־
    void OnHelpLog(wxCommandEvent& event);
	//��ô��ڷ��
	//int GetStyle();
	//�µ����Լ�ı���Ϣ
	void OnInstrumentIdCHange(wxCommandEvent& event);
	//�����µ�����Ϣ
	void OnPane2OrderPane(wxCommandEvent& event);
	//�������۱���Ϣ
	void OnPane2QuotPane(wxCommandEvent& event);
	//�������µ�����صĲ˵��򹤾���
	void OnUpdateUI_Order(wxUpdateUIEvent& event);
	//���²˵��򹤾���
	void OnUpdateUI_PosMove(wxUpdateUIEvent& event);
	//���²˵��򹤾���
	void OnUpdateUI_PosSwap(wxUpdateUIEvent& event);
	//��ȡ���������ʾ״̬
	void OnGetConfigPaneStatus(wxCommandEvent& event);
	//����configapp����
	void OnLoadConfigApp(wxCommandEvent& event);
//	//��ȡ��������ʾ����
//	void OnGetPaneCaption(wxCommandEvent& event);
//	//��ȡȫ��������֮��
//	void OnGetAllCommissionRate(wxCommandEvent& event);
	//����ģ��ת�˳ɹ�
	void OnSucessTransfer(wxCommandEvent& event);
	//��ʼ����̨��ѯ������Ϣ��Ӧ
	//void OnInitQryStepMain(wxCommandEvent& event);
	//ί��ģ��д״̬��log����Ӧ����
	void OnWriteUserLog(wxCommandEvent& event);
    //����ģ���ȡ���������Ӧ����
	void OnGetStyle(wxCommandEvent& event);
	//ת��������Ϣ��������Ԥ��������ӯ�𵥲�ѯ���ִ�в�ѯ
	void OnOrderPanel2BatchQuery(wxCommandEvent& event);
	void OnOrderPanel2ParkedQuery(wxCommandEvent& event);
	void OnOrderPanel2ConditionQuery(wxCommandEvent& event);
	void OnOrderPanel2StopQuery(wxCommandEvent& event);
	//
	void OnSendIEContent(wxCommandEvent& event);
	//�����ļ��ı�,ˢ�½���
	void OnCfgChanged(wxCommandEvent& event);
	//�б���ɫ����ı䣬ˢ�½���
	void OnUpdateListTable(wxCommandEvent& event);
	//�������ӵ�����Ϣ
	void OnRtnReconnected(wxCommandEvent& event);
	//���Ҫ��˵��ı�
	void OnPluginMenuChange(wxCommandEvent& event);
	//
	void OnMAPlatModify(wxCommandEvent& event);
	//
	void OnMAPlatAddOrDelete(wxCommandEvent& event);
	//
	//��ʱ��
	void OnTimer(wxTimerEvent& event);
	
	void OnMASocket(wxCommandEvent& event);
    //
    void OnUserNotice(wxCommandEvent& event);
	//
	void LoadHypertextNotice(NoticeContent* pContent,bool waitFinished);
	//�µ����������嵵��ѡ����ȡ����ѡ��Ϣ��Ӧ
	void OnBuySell5ShowChanged(wxCommandEvent& evt);
	void OnQuotSetFocus(wxCommandEvent& evt);
	//����Զ���˵���Ӧ
	void OnClickPluginMenu(wxCommandEvent& evt);
#ifdef _USE_MULTI_LANGUAGE
	//���Ըı�
	void ChangeLanguage(int langid);
	//�ı����������
	void ChangeFrameLanguage(int langid);
#endif
public:
	//����ģ��ID����ģ��
	wxString GetModuleCaption(int ModuleID);


protected: 
	//�ɿ�ݼ�ѡ��һ��ҳ��
	bool SelectPageFromAccel(int keycode);
    //�������ؼ�
    bool CreateNotebook(wxWindow * pParent);
#ifdef _USE_TOP_SPLITTER
	//��ʾ������Ƶ��Ѷ
	void OnToggleViewVideoInfo(bool bCheck); 
	//�������ؼ��ұ߷ָ���ͼ
    bool CreateRightSplitter(wxWindow * pParent);
#endif
	//�ı����ú����¶����������
	void CustomDefineUI();
	//����״̬��
	bool CreateMyStatusBar();
	//����ģ��ID����ģ��
	wxWindow* GetModuleWindow(int ModuleID);
    //���������
    //bool  CreateChildPane();
	////����ָ�������Ҽ��˵�����
 //   void ShowExtListContextMenuStyle(wxWindow* pWin,int style);
	////����ȫ�������Ҽ��˵�����Ϊ��ǰ
 //   void ShowExtListContextMenuStyle();
	void FloatPaneOpen();
	//�����µ���
    void PopupOrderInsertPanel();
    //��������ʾ��ݼ�
	void SetAliasNameShortCutKey(int id,std::string alias,std::string keycode);
	//����������̿����µ��������ݺ��µ���������
	void AsyncHandyOrderMapAndKeyMultiplyVolumeMap();
	//ע���ȼ�
	void MyRegisterHotKey();
	//��ȡ���۱�ǰѡ���Լ
	string  GetQuotCurInstrumentID();
public:
	//����windows�����¼�WM_COPYDATA��WM_HOTKEY
    WXLRESULT MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam); 
	//д״̬��log��¼
    void WriteUserLog(string strType, LOG_DATA* pData, string strTime="");
	//��ȡ�µ������߳�
	COrderServiceThread* GetOrderServiceThread() {
		return m_poOrderServiceThread;
	};

	//�ص�����
	static bool DeletePlugPane(int npos, wxString AppID);
	static wxWindow* CreatePlugPane(int nPos,wxString caption,wxString AppID);
	void OnLoadPlusIn(wxCommandEvent& event);
	void OnDulplicateKeyPrompt(wxCommandEvent& event);
	void OnRecvMsg(wxCommandEvent& event);
	//���ش浵��Ϣ
	bool LoadSavedMessages( std::string strDate, std::vector<MessageInfo>& vMsgInfo );
	bool SaveMessage();

	UINT GetBeepThreadId();
private:
    //���ؼ�
    zqAuiNotebook* m_notebook;
	CBankFutureTransferDlg* m_pBankFutureTransferDlg;//����ת�˶Ի���
    DisConnPromptDlg*     m_pDisConnPromptDlg;//����Ͽ���ʾ�Ի���
    TradingNoticeDlg*     m_pTradingNoticeDlg;//����֪ͨ�Ի���
	HWND m_hypertextNoticeWnd;//
    DECLARE_EVENT_TABLE()
    //��ݼ�<����ID������keycode>    
    std::map<int,int> m_ID2AccelMap;
    //�����������µ���
    wxString m_LayoutPerspective;
    int      m_FloatPageIndex;
    map<int,wxWindow*> m_mapModuleWindow;
	COrderServiceThread*	m_poOrderServiceThread;

	int			m_nDeltaOrderPanelSashPos;
	BOOL		m_bMaxBak_MainFrame;
	wxPoint		m_posBak_MainFrame;
    bool        m_RegisterHotKey;
	std::map<std::string, KeyOrderInfo> m_KeyFuncMap;
//	std::map<std::string, int> m_KeyMultiplyVolumeMap;
	MainTaskBarIcon* m_MainTaskBarIcon;
    std::map<int,std::string> m_TradingNoticeArray;
    NoticeContent* m_NoticeContent;
#ifdef _USE_TRACE_THREAD
    CTrackThread*  m_pTrackThread;
#endif
	CBeepThread*  m_pBeepThread;

	wxTimer m_timer;
	LockFrameDlg* m_LockFrameDlg;
	std::string m_OptionCaption;
	std::vector<USER_LOG> m_vLogData;
	//
	long m_CifcoServicesHallProcessId;	

	//�洢��Ϣ
	std::map<int, MessageInfo> m_mapMsgInfo;
	std::vector<MessageInfo> m_vMsgInfo;
	MsgDlg* m_pMsgDlg;
	std::string m_strNewPwd;
public:	
	LPMAIN_WINDOW_CFG m_MainFrameCfg;
	//��Լ����״̬֪ͨ�ص�����
    static int InstrumentStatusCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);  
	//�뽻�׺�̨ͨ�����ӶϿ��ص�����
    static int FrontDisconnectedCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data); 
    //�뽻�׺�̨ͨ�����ӳɹ��ص�����
	static int FrontConnectedCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);
	//�û���¼
	static int RspUserLoginCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);
	//����֪ͨ�ص�����
    static int TradingNoticeCallFunc(const GUIModuleID GID,const AbstractBusinessData &data);
	//
	static int AlertMessageCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);
    //
	static int RspQryNoticeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);
	//
	static int RspQrySettlementInfoConfirmCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);

	//�ص�����
	static bool NotebookAddPage(int id,wxWindow** win,wxString& caption);

	static void DoStyleChanged(int nApplyType);

public:
    bool GetCurInstrumentGrpInfo_QuotPanel(std::string& GrpName,int& GrpID);
    void GetCurInstrumentID_QuotPanel(std::string& strInstrument);
    //���Ҫ���½���ɾ����壬type==1�½���2ɾ��
    wxWindow* Plugin_New_Del_Panel_QuotFrame(int type);
    //��ȡ��������ʾ����
    string GetPanelCaption(int PanelID,int type);
	string GetCifcoServicesHallLocation();
	void LoadCifcoServicesHallApp();
	bool LoadExDll( std::string strpath );
};
class ShowMgr
{
public:
    ShowMgr(MainFrame* pParent):m_pParent(pParent)
    {
       if(m_pParent&&m_pParent->IsShown()) 
       {
			m_pParent->Hide();
       }
    }
    ~ShowMgr()
    {
       if(m_pParent&&(!m_pParent->IsShown()))
       {
           m_pParent->Show();
       }
    }
private:
    MainFrame* m_pParent;
};