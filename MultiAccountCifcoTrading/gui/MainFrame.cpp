#include "StdAfx.h"
#include <set>
#include "zq.h"
#include "wx/renderer.h"
#include "wx/html/htmlwin.h"
#include "wx/filename.h"
#include "wx/dir.h"
#include "wx/textfile.h"
#include "UserLoginDlg.h"
#include "TradingNoticeDlg.h"
#include "MyStatusBar.h" //״̬��
#include "ModifyPwdDlg.h"
#include "MainTaskBarIcon.h"
#include "MainFrame.h"
#include "LogDlg.h"
#include "LockFrameDlg.h"
#include "HisCalDlg.h"
#include "FrameHook.h"
#include "DisConnPromptDlg.h"
#include "AskNameDlg.h"
#include "AboutDlg.h"
#include "../inc/Order-QueryPanel/CQueryLocalOrderPanel.h"
#include "../inc/Order-QueryPanel/CQueryOrderPositionPanel.h"
#include "../inc/Order-QueryPanel/CQueryOrderParkedPanel.h"
#include "../inc/Order-QueryPanel/CQueryOrderConditionPanel.h"
#include "../inc/Order-QueryPanel/CQueryOrderBatchPanel.h"
#include "../inc/Order-Common/COrderServiceThread.h"
#ifdef _USE_TOP_SPLITTER
	#include "../inc/Module-WebPanel/MultiSplitter.h"
#endif
#include "../inc/Module-TradeRecord/TradeRecordPanel.h"
#include "../inc/Module-QuotFrame/QuotFrame.h"    //���۱�
#include "../inc/Module-Query/QueryTrader.h"
#include "../inc/Module-Query/QueryReport.h"
#include "../inc/Module-Query/QueryPosition.h"
#include "../inc/Module-Query/QueryOrderType.h"
#include "../inc/Module-Query/QueryExchangeRate.h"
#include "../inc/Module-Query/QueryExchange.h"
#include "../inc/Module-Query/QueryContract.h"
#include "../inc/Module-Query/QueryCommodity.h"
#include "../inc/Module-Position/PositionPanel.h"
#include "../inc/Module-Orders/OrderInsertPanel.h"
#include "../inc/Module-Misc/zqControl.h"
#include "../inc/Module-Misc/LogDefine.h"
#include "../inc/Module-Misc/ImportExport.h"
#include "../inc/Module-MainNoteBook/zqAuiNotebook.h" 
#include "../inc/Module-InstrumentList/InstrumentPanel.h"//��Լ�б�
#include "../inc/Module-FundAccount/FundAccountModule.h"
#include "../inc/Module-Config/J_ZiXuanHeYueDlg.h"
#include "../inc/Module-Config/J_XiTongCanShu.h"
#include "../inc/Module-Config/J_XiaDanShuRuShunXu.h"
#include "../inc/Module-Config/J_WeiTuoCanShu.h"
#include "../inc/Module-Config/J_ShuZiKuaiJieJian.h"
#include "../inc/Module-Config/CJ_ModifyPwdDlg.h"
#include "../inc/Module-Config/CJ_JieSuanDlg.h"
#include "../inc/Module-Config/CJ_ChuRuJinDlg.h"
#include "../inc/Module-Bank/BankTranferDlg.h"//����ת�˽�ʿ�ﴰ��
#include "../inc/Module-Bank/BankFutureTransferDlg.h"
#include "../inc/Module-AllOrderPanel/AllOrdersPanel.h" //����ί�е�
#include "../inc/Module-MultiAccount/MultiAccountPanel.h"
#include "../inc/Module-MultiAccount/MultiPositionPanel.h"
#include "../inc/MultiAccountCore/MultiAccountCore.h"
#include "../inc/MultiAccountCore/MAConnSocket.h"
#include "../MAServer/include/MAAccount/MultiAccountStruct.h"
#include "../inc/Module-Misc/MD5.h"
#include "../ConfigApp/ConfigAppDlg.h"
#include "FloatPanelDlg.h"
#include "TopException.h"
#include "NoticeContent.h"
#include "Plusin/Plusin.h"
#include "MyProcess.h"
#include "MsgDlg.h"
#include "MsgListDlg.h"
#include "ChangeMAPwdDlg.h"

CPlusin g_Plusin;
static MainFrame* s_pMainFrame=NULL;
#define HOTKEY_ID 700
#define LOCK_FRAME_SECOND 300
#define ACCOUNT_PANEL_HEIGHT	45
#define IE_MAGIC_NUMBER  0x4945
#define IE_MSG_LOGIN		1
#define IE_MSG_CONTENT		2
#define IE_MSG_EXIT			3
extern DWORD g_StartIdleTime;
typedef bool (*GETAME)(char* pRtnString,int length);
#pragma comment(lib, "Psapi.lib")


#if 1
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("MainFrame", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif

//��׼���캯��,��û�д�������
MainFrame::MainFrame():m_notebook(NULL),
m_pBankFutureTransferDlg(NULL),
m_pDisConnPromptDlg(NULL),
m_pTradingNoticeDlg(NULL),
m_poOrderServiceThread(NULL),
m_MainTaskBarIcon(NULL),
m_bMaxBak_MainFrame(FALSE),
m_posBak_MainFrame(0,0),
m_RegisterHotKey(false)
, m_timer(this)
, m_LockFrameDlg( NULL )
, m_pMsgDlg( NULL )
, m_vLogData()
, m_nDeltaOrderPanelSashPos(0),
m_hypertextNoticeWnd(NULL),
m_CifcoServicesHallProcessId(0),
m_pBeepThread(NULL)
{
	WXLOG_INFO("MainFrame::MainFrame");
	m_mapMsgInfo.clear();
	m_vMsgInfo.clear();
	s_pMainFrame=this;
    m_MainFrameCfg=NULL;
    m_NoticeContent=new NoticeContent();
#ifdef _USE_TRACE_THREAD
	m_pTrackThread=new CTrackThread();
	if(m_pTrackThread->Create()== wxTHREAD_NO_ERROR)
	{
		m_pTrackThread->Run();
	}
#endif
	m_pBeepThread=new CBeepThread();
	if(m_pBeepThread->Create()== wxTHREAD_NO_ERROR)
	{
		m_pBeepThread->Run();
	}
	wxRegisterId(wxID_HIGHEST+1);
	m_timer.Start( 500 );
	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RtnInstrumentStatus,GID_ANY,InstrumentStatusCallBackFunc);
	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_FrontDisconnected,GID_ANY,FrontDisconnectedCallBackFunc);
	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_FrontConnected,GID_ANY,FrontConnectedCallBackFunc);
	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspUserLogin,GID_ANY,RspUserLoginCallBackFunc);
	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RtnTradingNotice,GID_ANY,TradingNoticeCallFunc);
    if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_AlertMessage,GID_ANY,AlertMessageCallBackFunc);
    if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryNotice,GID_ANY,RspQryNoticeCallBackFunc);
    if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspQrySettlementInfoConfirm,GID_ANY,RspQrySettlementInfoConfirmCallBackFunc);

}
//��׼��������
MainFrame::~MainFrame(void)
{
	WXLOG_INFO("MainFrame::~MainFrame");
	for ( int i = 0; i < (int)m_vLogData.size(); i++ )
	{
		USER_LOG userLog = m_vLogData[i];
		if ( NULL != userLog.pData )
		{
			delete userLog.pData;
			userLog.pData = NULL;
		}
	}
	m_vLogData.clear();

#ifdef _USE_TRACE_THREAD
	if(m_pTrackThread)
	{
		m_pTrackThread->Delete();
        m_pTrackThread=NULL;
	}
#endif
	if(m_pBeepThread)
	{
		m_pBeepThread->Delete();
		m_pBeepThread=NULL;
	}
	wxDELETE(m_poOrderServiceThread)
    wxDELETE(m_NoticeContent)
    wxDELETE(m_pTradingNoticeDlg);//����֪ͨ�Ի���
	SetMainHook(this,false);
    LOG_INFO("MainFrame::~MainFrame()");
}
//�麯������ֹ״̬����ʾ�˵���ʾ
void MainFrame::DoGiveHelp(const wxString& text, bool show)
{
	//�麯����ʲôҲ����
}
UINT MainFrame::GetBeepThreadId()
{
	return m_pBeepThread?m_pBeepThread->GetId():0;
}
//�麯�������ڲ���
bool MainFrame::Layout()
{

	if(m_notebook)
	{
		wxWindow* pParent=m_notebook->GetParent();
		if(pParent==this)
        {
			m_notebook->SetSize(GetClientSize());
            //toolsbar�ĸ߶ȿ��ܱ��Զ���������28->25����ʱm_notebook��λ��Ҳ��Ҫ������Ӧ����
            //�����ʹm_notebook�������status�ص��������Ļˢ�²��ɾ�
            wxPoint pt=m_notebook->GetPosition();
            if(pt.x!=0||pt.y!=0)
            {
                pt.x=pt.y=0;
                m_notebook->SetPosition(pt);
            }
        }
		else if(wxDynamicCast(pParent,wxSplitterWindow))
		{
			int minsize=((wxSplitterWindow*)pParent)->GetMinimumPaneSize();
			((wxSplitterWindow*)pParent)->SetSashPosition(GetClientSize().x-minsize);
			pParent->SetSize(GetClientSize());
		}
		else
			pParent->SetSize(GetClientSize());
	}
	return FrameBase::Layout();
}
BEGIN_EVENT_TABLE(MainFrame,FrameBase)
EVT_MENU    (ID_LOCK_FRAME, MainFrame::OnLockFrame)
EVT_MENU	(ID_READ_MSG, MainFrame::OnReadMsg)
EVT_MENU    (wxID_EXIT,MainFrame::OnExit)
EVT_CLOSE   (               MainFrame::OnMainClose)
EVT_MENU    (ID_TRANSFER,   MainFrame::OnBankFutureTransfer)	
EVT_MENU    (ID_QUERY_TRADER,   MainFrame::OnQueryTrader)
EVT_MENU    (ID_QUERY_CONTRACT,   MainFrame::OnQueryContract)
EVT_MENU    (ID_QUERY_EXCHANGE,   MainFrame::OnQueryExchange)
EVT_MENU    (ID_QUERY_COMMODITY,   MainFrame::OnQueryCommodity)
EVT_MENU    (ID_QUERY_ORDERTYPE,   MainFrame::OnQueryOrderType)
EVT_MENU    (ID_QUERY_POSITION,   MainFrame::OnQueryPosition)
EVT_MENU    (ID_QUERY_REPORT,   MainFrame::OnQueryReport)
EVT_MENU    (ID_QUERY_EXCHANGE_RATE,   MainFrame::OnQueryExchangeRate)
//EVT_MENU    (ID_TRADE_REPORT,MainFrame::OnTradeReport)
//EVT_MENU    (ID_TODAY_REPORT,MainFrame::OnTradeReport_Today)
//EVT_MENU    (ID_WEEK_REPORT,MainFrame::OnTradeReport_Week)
//EVT_MENU    (ID_MONTH_REPORT,MainFrame::OnTradeReport_Month)
EVT_MENU    (ID_CONFIG_OPTION,MainFrame::OnConfigOption)
EVT_MENU    (ID_DEFAULT_CONFIG,MainFrame::OnApplyDefaultConfig)
EVT_MENU    (ID_MODIFY_PSW, MainFrame::OnModifyPwd)
EVT_MENU    (ID_MODIFY_MA_PASSWORD, MainFrame::ModifyMAPwd)
EVT_MENU    (ID_QRY_SETTLE, MainFrame::HisCal)
EVT_MENU    (ID_HELP_CONTENT,MainFrame::OnHelpContent)
EVT_MENU    (wxID_ABOUT, MainFrame::OnAbout)
//EVT_TIMER(ID_TIMER, MainFrame::OnTimer)
EVT_CHAR_HOOK (MainFrame::OnCharHook)
EVT_CHAR_EX(MainFrame::OnCharHook)
EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY,MainFrame::OnFloatPaneOpen)
EVT_AUI_PANE_CLOSE(MainFrame::OnFloatPaneClose)
EVT_MENU    (ID_QRY_ACCOUNT,MainFrame::OnQryAccount)
EVT_COMMAND(wxID_ANY, wxEVT_QRY_ACCOUNT_DLG,MainFrame::OnQryAccount)
EVT_COMMAND(wxID_ANY, wxEVT_FRONT_DISCONNECTED,OnFrontDisconnected)
EVT_COMMAND(wxID_ANY, wxEVT_CONNECT_LOGIN,OnConnectLogin)
EVT_COMMAND(wxID_ANY, wxEVT_TRADING_NOTICE,OnTradingNotice)
EVT_COMMAND(wxID_ANY, wxEVT_INSTRUMENT_STATUS,OnInstrumentStatus)
EVT_COMMAND(wxID_ANY, wxEVT_USER_NOTICE,OnUserNotice)
EVT_COMMAND(wxID_ANY, wxEVT_MAINFRAME_QUOT_SETFOCUS,OnQuotSetFocus)
EVT_COMMAND(wxID_ANY, wxEVT_BUYSELL5_SHOW_CHANGE, OnBuySell5ShowChanged)

EVT_MENU(ID_MENU_PROFIT,MainFrame::OnOrderProfit)              
EVT_MENU(ID_MENU_CONDITION,MainFrame::OnOrderCondition)           
EVT_MENU(ID_MENU_DELSELECT ,MainFrame::OnConfigSelect)          
EVT_MENU(ID_MENU_CANCELOPDER,MainFrame::CancerlOrder)         
EVT_MENU(ID_MENU_CONDITION_DELETE,MainFrame::ConditionPanelButton)    
EVT_MENU(ID_MENU_CONDITION_PAUSE ,MainFrame::ConditionPanelButton)    
EVT_MENU(ID_MENU_CONDITION_ACTIVE,MainFrame::ConditionPanelButton)    
EVT_MENU(ID_MENU_CONDITION_MODIFY ,MainFrame::ConditionPanelButton)   

EVT_MENU(ID_STYLE_SAVE,MainFrame::OnStyleSave)
EVT_COMMAND_RANGE(ID_BASE_LAYOUT,ID_BASE_LAYOUT+100,wxEVT_COMMAND_MENU_SELECTED,MainFrame::OnStyleChanged)
EVT_UPDATE_UI_RANGE(ID_BASE_LAYOUT,ID_BASE_LAYOUT+100, MainFrame::OnUpdateUIOnStyleChanged)
EVT_COMMAND_RANGE(ID_PLUGIN_ITEM_BEGIN,ID_PLUGIN_ITEM_END,wxEVT_COMMAND_MENU_SELECTED,MainFrame::OnClickPluginMenu)
EVT_ICONIZE(MainFrame::OnIconize)
EVT_IDLE(MainFrame::OnIdle)
//���˴�˵�
EVT_MENU(ID_LOGIN_AGAIN,MainFrame::OnLoginAgain)
EVT_MENU(ID_UPDATE_APP,MainFrame::OnUpdateApp)

EVT_MENU(ID_CONFIG_SELECT, MainFrame::OnConfigSelect) 
EVT_MENU(ID_CONFIG_ORDER,MainFrame::OnConfigOrder)
EVT_MENU(ID_CONFIG_SYSTEM,MainFrame::OnConfigSystem)
EVT_MENU(ID_CONFIG_PASSWORD,MainFrame::OnConfigPassword)
EVT_MENU(ID_CONFIG_ACCE,MainFrame::OnConfigAcce)

EVT_UPDATE_UI(ID_CONFIG_SELECT, MainFrame::OnUpdateUIConfigSelect) 
EVT_UPDATE_UI(ID_CONFIG_ORDER,MainFrame::OnUpdateUIConfigOrder)
EVT_UPDATE_UI(ID_CONFIG_SYSTEM,MainFrame::OnUpdateUIConfigSystem)
EVT_UPDATE_UI(ID_CONFIG_PASSWORD,MainFrame::OnUpdateUIConfigPassword)
EVT_UPDATE_UI(ID_CONFIG_ACCE,MainFrame::OnUpdateUIConfigAcce)

EVT_MENU(ID_CONFIG_Gaoji,MainFrame::OnConfigGaoji)
EVT_MENU(ID_ORDER_STANDARD,MainFrame::OnOrderStandard)
EVT_MENU(ID_ORDER_SMALLWIN,MainFrame::OnOrderSmallWin)
EVT_MENU(ID_ORDER_BATCH,MainFrame::OnOrderBatch)
EVT_MENU(ID_ORDER_CONDITION,MainFrame::OnOrderCondition)
EVT_MENU(ID_ORDER_PROFIT,MainFrame::OnOrderProfit)
EVT_MENU(ID_ORDER_STOPLIMIT,MainFrame::OnOrderStopLimit)
EVT_MENU(ID_QUERY_WITHDRAW,MainFrame::OnQueryWithdraw)
EVT_MENU(ID_QUERY_SETTLEMENT,MainFrame::OnQuerySettlement)
EVT_MENU(ID_TRANSFER_BANK,MainFrame::OnTransferBank)
EVT_MENU(ID_TRANSFER_WITHDRAW,MainFrame::OnTransferWithdraw)
EVT_MENU(ID_HELP_DOC,MainFrame::OnHelpDoc)
EVT_MENU(ID_HELP_LOG,MainFrame::OnHelpLog)
EVT_BUTTON(ID_RETURN_NORMAL_UI,OnReturnNormalUI)
EVT_MENU(ID_POS_MOVE,OnPositionMove)
EVT_MENU(ID_POS_SWAP,OnPositionSwap)
EVT_MENU(ID_PLUSIN, OnShowPlusInDlg)
//���²˵�
EVT_UPDATE_UI(ID_ORDER_SMALLWIN, OnUpdateUI_Order)
EVT_UPDATE_UI(ID_ORDER_STANDARD,OnUpdateUI_Order)
EVT_UPDATE_UI(ID_ORDER_BATCH,OnUpdateUI_Order)
EVT_UPDATE_UI(ID_ORDER_CONDITION,OnUpdateUI_Order)
EVT_UPDATE_UI(ID_ORDER_PROFIT,OnUpdateUI_Order)
EVT_UPDATE_UI(ID_ORDER_STOPLIMIT,OnUpdateUI_Order)
EVT_UPDATE_UI(ID_CONFIG_OPTION,OnUpdateUI_Order)
EVT_UPDATE_UI(ID_POS_MOVE,OnUpdateUI_PosMove)
EVT_UPDATE_UI(ID_POS_SWAP,OnUpdateUI_PosSwap)
//ת����Ϣ
EVT_COMMAND(wxID_ANY,wxEVT_ORDERINSERT_INSTRUMENTID_CHANGED,OnInstrumentIdCHange)
EVT_COMMAND(wxID_ANY,wxEVT_GRID_INSTRUMENTID_CHANGED,OnPane2OrderPane)
EVT_COMMAND(wxID_ANY,wxEVT_ORDERINSERT_MOUSEORDER,OnPane2OrderPane)
EVT_COMMAND(wxID_ANY,wxEVT_UPDATE_CONFIG_PANE,OnGetConfigPaneStatus)
EVT_COMMAND(wxID_ANY,wxEVT_LOAD_CONFIG_APP,OnLoadConfigApp)
//EVT_COMMAND(wxID_ANY,wxEVT_GET_PANE_CAPTION,OnGetPaneCaption)
//EVT_COMMAND(wxID_ANY,wxEVT_GET_ALL_COMMISSIONRATE,OnGetAllCommissionRate)
EVT_COMMAND(wxID_ANY,wxEVT_ORDERINSERT_ORDERCHANGE,OnPane2OrderPane)
EVT_COMMAND(wxID_ANY,wxEVT_ORDERINSERT_ORDERREPLACE,OnPane2OrderPane)
EVT_COMMAND(wxID_ANY,wxEVT_QUOT_SET_GROUP,OnPane2QuotPane)
//EVT_COMMAND(wxID_ANY,wxEVT_QUOT_GET_GROUP,OnPane2QuotPane)
EVT_COMMAND(wxID_ANY,wxEVT_ORDERINSERT_MOUSECLOSE,OnPane2OrderPane)
EVT_COMMAND(wxID_ANY,wxEVT_MA_ORDERINSERT_ORDERCHANGE,OnPane2OrderPane)
EVT_COMMAND(wxID_ANY,wxEVT_MA_ORDERINSERT_MOUSECLOSE,OnPane2OrderPane)
EVT_COMMAND(wxID_ANY,wxEVT_SUCESS_TRANSFER,OnSucessTransfer)
//EVT_COMMAND(wxID_ANY,wxEVT_INITQRY_MAIN,OnInitQryStepMain)
EVT_COMMAND(wxID_ANY,wxEVT_WRITE_USER_LOG,OnWriteUserLog)
EVT_COMMAND(wxID_ANY,wxEVT_GET_STYLE,OnGetStyle)
EVT_COMMAND(wxID_ANY,wxEVT_LOAD_PLUSIN,OnLoadPlusIn)
EVT_COMMAND(wxID_ANY,wxEVT_DULPLICATE_KEY_PROMPT,OnDulplicateKeyPrompt)
EVT_COMMAND(wxID_ANY,wxEVT_RECV_MSG,OnRecvMsg)
EVT_TIMER(wxID_ANY, OnTimer)
EVT_COMMAND(wxID_ANY,wxEVT_MA_SOCKET,OnMASocket)
//�����ļ��ı�
EVT_COMMAND(wxID_ANY,wxEVT_CONFIG_CHANGED,OnCfgChanged)
EVT_COMMAND(wxID_ANY, wxEVT_UPDATE_LIST_TABLE, OnUpdateListTable)
	EVT_COMMAND(wxID_ANY,wxEVT_QUERY_BATCH_REQUERY, OnOrderPanel2BatchQuery)
	EVT_COMMAND(wxID_ANY,wxEVT_QUERY_PARKED_REQUERY, OnOrderPanel2ParkedQuery)
	EVT_COMMAND(wxID_ANY,wxEVT_QUERY_CONDITION_REQUERY, OnOrderPanel2ConditionQuery)
	EVT_COMMAND(wxID_ANY,wxEVT_QUERY_STOP_REQUERY, OnOrderPanel2StopQuery)
	EVT_COMMAND(wxID_ANY,wxEVT_SEND_IE_CONTENT,OnSendIEContent)
	EVT_COMMAND(wxID_ANY,wxEVT_RTN_RECONNECTED,OnRtnReconnected)
	EVT_COMMAND(wxID_ANY,wxEVT_PLUSIN_MENU_CHANGE,OnPluginMenuChange)
	EVT_COMMAND(wxID_ANY,wxEVT_MA_PLAT_MODIFY,OnMAPlatModify)
	EVT_COMMAND(wxID_ANY,wxEVT_MA_PLAT_ADD,OnMAPlatAddOrDelete)
	EVT_COMMAND(wxID_ANY,wxEVT_MA_PLAT_DELETE,OnMAPlatAddOrDelete)
END_EVENT_TABLE()
//���Ͷ���ҵ���¼�
void MainFrame::SendSubscribeEventToChildren(bool IsSubscribe)
{
    if(m_notebook==NULL) return;
	int i,pagecount=m_notebook->GetPageCount();
	wxCommandEvent myEvent(wxEVT_SUBSCRIBE);
	myEvent.SetInt(IsSubscribe?1:0);
	for(i=0;i<pagecount;i++)
	{
		wxWindow* win = m_notebook->GetPageWindow(i);
		if ( NULL != win )
		{
			win->ProcessEvent(myEvent);
		}   
	}
}
void MainFrame::OnMAPlatModify(wxCommandEvent& event)
{
	if(m_notebook==NULL) return;
	if(event.GetId()!=GetId()) return;
	int i,pagecount=m_notebook->GetPageCount();
	wxCommandEvent myEvent(event.GetEventType());
	myEvent.SetInt(event.GetInt());
	for(i=0;i<pagecount;i++)
	{
		wxWindow* win = m_notebook->GetPageWindow(i);
		if ( NULL != win )
		{

			win->ProcessEvent(myEvent);
		}   
	}
}
void MainFrame::OnMAPlatAddOrDelete(wxCommandEvent& event)
{
	if(m_notebook==NULL) return;
	if(event.GetId()!=GetId()) return;
	int i,pagecount=m_notebook->GetPageCount();
	wxCommandEvent myEvent(event.GetEventType());
	myEvent.SetInt(event.GetInt());
	for(i=0;i<pagecount;i++)
	{
		wxWindow* win = m_notebook->GetPageWindow(i);
		if ( NULL != win )
		{
			myEvent.SetId(win->GetId());
			win->ProcessEvent(myEvent);
		}   
	}
}
//���������ļ��ı��¼�
void MainFrame::SendCfgXmlChangeEventToChildren()
{
    if(m_notebook==NULL) return;
	//��������˵�
	if(m_notebook->GetLayoutStyle()==STYLE_SunGard
		&& DEFAULT_SVR() && DEFAULT_SVR()->IsSunGardSupported())
		CreateMenuBar_SunGard();
	else
		CreateMenuBar_Q7(); 
	int i,pagecount=m_notebook->GetPageCount();
	wxCommandEvent myEvent(wxEVT_CFG_CHANGE);
	for(i=0;i<pagecount;i++)
	{
		m_notebook->GetPageWindow(i)->ProcessEvent(myEvent);     
	}
	Refresh();
}

void MainFrame::SendStyleChangeEventToChildren(int nApplyType)
{
	if ( m_notebook == NULL )
	{
		return;
	}

	wxCommandEvent myEvent( wxEVT_STYLE_CHANGED );
	myEvent.SetInt(nApplyType);
	for ( size_t i = 0; i <m_notebook->GetPageCount(); i++)
	{
		wxWindow* pWnd = m_notebook->GetPageWindow(i);
		if ( NULL != pWnd )
		{
			pWnd->AddPendingEvent( myEvent );
		}
	}
}

//�������µ�����صĲ˵��򹤾���
void MainFrame::OnUpdateUI_Order(wxUpdateUIEvent& event)
{
    if(m_notebook==NULL) return;
	static int MenuID[]={ID_ORDER_STANDARD,ID_ORDER_BATCH,ID_ORDER_CONDITION,ID_ORDER_PROFIT,ID_ORDER_STOPLIMIT};
	bool bFlag = m_notebook->IsConfigPaneOpen();
	event.Enable(!bFlag);
	bool bCheck=false;;
	for(int index=0;index<sizeof(MenuID)/sizeof(int);index++)
	{
		if(event.GetId()==MenuID[index])
		{
			OrderInsertPanel* pWinOrder = (OrderInsertPanel*)FindWindowById(GID_ORDER_INSERT_PANEL, this);
			bCheck=(pWinOrder&&pWinOrder->GetViewSubStyle()==index);
			break;
		}
	}
	event.Check(bCheck);
}
//��ȡ���������ʾ״̬
void MainFrame::OnGetConfigPaneStatus(wxCommandEvent& event)
{
    if(m_notebook==NULL) return;
	event.SetInt(m_notebook->IsConfigPaneOpen()?1:0);
}
//����configapp����
void MainFrame::OnLoadConfigApp(wxCommandEvent& event)
{
    if(m_notebook==NULL) return;
	if(!m_notebook->IsConfigPaneOpen())
		ShowConfigAppDlg(event.GetInt());
	else
		wxMessageBox(LOADSTRING(GUI_LOADCONFIGAPP),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
}
//��ȡ��������ʾ����
//void MainFrame::OnGetPaneCaption(wxCommandEvent& event)
//{
 //   if(m_notebook==NULL) return;
//	int i,pagecount=m_notebook->GetPageCount();
//	wxString caption;
//	for(i=0;i<pagecount;i++)
//	{
//		if(m_notebook->GetPageWindow(i)->GetId()==event.GetId())
//		{
//			if(event.GetInt()==1)
//				caption=GetModuleCaption(event.GetId());
//			else
//			{
//				caption=m_notebook->GetPageCaption(i);
//				caption=caption.BeforeFirst('(');
//			}
//			event.SetString(caption.c_str());
//			break;
//		}
//	}
//}
#if 0
//��ȡȫ��������֮��
void MainFrame::OnGetAllCommissionRate(wxCommandEvent& event)
{
	TradeRecordPanel* pTradeRecordPanel=(TradeRecordPanel*)FindWindowById(GID_THRADE_RECORD);
	pTradeRecordPanel->ProcessEvent(event);
}
#endif
//���˴�����ջ��·���������壬�ص�����ҳ��
void MainFrame::OnReturnNormalUI(wxCommandEvent& evt)
{
	DoReturnNormalUI();
}
void MainFrame::DoReturnNormalUI()
{
    if(m_notebook==NULL) return;
	//�������ò���
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
	m_MainFrameCfg=pCfgMgr->GetMainWindowCfg();         
	m_notebook->FixedPane(m_MainFrameCfg->bFixPanel);
	//
	m_notebook->DoConfigPaneClose();
	m_curConfigPanel=NULL;
	m_curConfigPanelClassName.Clear();
	//���͸ı���Ϣ
	//wxCommandEvent myEvent(wxEVT_FILE_CHANGE,(int)GetHWND());////���CITHINC-953
	//AddPendingEvent(myEvent); 
	wxCommandEvent myEvent(wxEVT_CFG_CHANGE);
	GetModuleWindow(GID_QUOT_TABLE)->ProcessEvent(myEvent);
	GetModuleWindow(GID_QUOT_TABLE)->Refresh();
	MyRegisterHotKey();
}
void TRACE(const char* format,...);
BOOL s_bNeedHide = FALSE;
DWORD s_dwNeedTime = 0;
//��С��������
void MainFrame::OnIconize(wxIconizeEvent& evt)
{
//	if(evt.Iconized()&&m_MainTaskBarIcon&&m_MainFrameCfg)
//	{
//		if(m_MainFrameCfg->bIsMinTray)
//		{
//			Hide();
//			m_MainTaskBarIcon->SetIcon(wxIcon(_T("zqICON_MAIN")));
//		}
//		else
//			m_MainTaskBarIcon->RemoveIcon();
//	}
	if(m_MainFrameCfg==NULL)
		return;
	if(m_MainFrameCfg->bIsMinTray)
	{
		if(evt.Iconized())
		{
			s_bNeedHide = TRUE;
			s_dwNeedTime = ::GetTickCount();
		}
		else {
			//m_MainTaskBarIcon->RemoveIcon();
			s_bNeedHide = FALSE;
		}
	}
}
#define WINDOW_IS_ACTIVE() ((m_MainTaskBarIcon&&m_MainTaskBarIcon->IsIconInstalled())||IsShown())
//������Ϣ��Ӧ
void MainFrame::OnIdle(wxIdleEvent& event)
{ 
	// WXLOG_INFO("MainFrame::OnIdle");   
	if(!m_notebook) return;

	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	if( pCfgMgr->GetMainWindowCfg()->bAutoLock && WINDOW_IS_ACTIVE()
		&& (GetTickCount() - g_StartIdleTime) / 1000 >= LOCK_FRAME_SECOND )
	{
		wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED,ID_LOCK_FRAME);
		AddPendingEvent(evt);
	}

}
//�ı����ú����¶����������
void MainFrame::CustomDefineUI()
{
	WXLOG_INFO("MainFrame::CustomDefineUI");
    if(m_notebook==NULL) return;
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
	if(pCfgMgr==NULL) return;  
	m_MainFrameCfg=pCfgMgr->GetMainWindowCfg();
	//SetWindowStyle(m_MainFrameCfg->IsTop!=0?(GetWindowStyle()|wxSTAY_ON_TOP):(GetWindowStyle()&(~wxSTAY_ON_TOP)));
	//MainFrameCfg->ColorPlan;				//��ɫ����           
	m_notebook->FixedPane(m_MainFrameCfg->bFixPanel);//�̶����
	//ShowExtListContextMenuStyle();
	//MainFrameCfg->bShowPanelShortKey;      //��������ʾ��ݼ�  
	std::vector< PanelInfo>* pPaneInfoVector=pCfgMgr->GetPanelSetVector();
	m_ID2AccelMap.clear();
	for(size_t i=0;i<pPaneInfoVector->size();i++)
	{
		PanelInfo& pInfo=(*pPaneInfoVector)[i];
		if( strlen(pInfo.AliasName) > 0)
			SetAliasNameShortCutKey(pInfo.id,pInfo.AliasName,pInfo.ShortKey);	
		else
			SetAliasNameShortCutKey(pInfo.id, GetModuleCaption(pInfo.id).c_str(),pInfo.ShortKey);	
	}
	MyRegisterHotKey();
}
//ע���ȼ�
void MainFrame::MyRegisterHotKey()
{
    if(m_notebook==NULL) return;
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
	if(pCfgMgr==NULL) return;  
	if(m_RegisterHotKey)
	{
		UnregisterHotKey(HOTKEY_ID);
		m_RegisterHotKey=false;
	}

	if(m_notebook->GetLayoutStyle()==STYLE_SunGard)
	{
		SystemParamInfo* pInfo=pCfgMgr->GetSystemParamInfo();
		if(pInfo->bUse)
		{
			//"F3"->VK_F3
			int key=0;
			if( strlen(pInfo->key1) > 1) key=VK_F1+atoi(&pInfo->key1[1])-1;
			if(key>0)
			{
				size_t fsModifiers=0;
				if( strlen(pInfo->key2) > 0) fsModifiers+=MOD_ALT; 
				if( strlen(pInfo->key3) > 0) fsModifiers+=MOD_CONTROL; 
				if( strlen(pInfo->key4) > 0) fsModifiers+=MOD_SHIFT; 
				if( strlen(pInfo->key5) > 0) fsModifiers+=MOD_WIN; 

				if(RegisterHotKey(HOTKEY_ID,fsModifiers,key))
					m_RegisterHotKey=true;
			}
		}
	} 
}
//���ڲ˵���Ӧ:�������
void MainFrame::OnLockFrame(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::LockFrame");
	if ( m_LockFrameDlg )
	{
		return;
	}

	if(m_MainTaskBarIcon&&m_MainTaskBarIcon->IsIconInstalled())
	{
		wxTaskBarIconEvent evt(wxEVT_TASKBAR_LEFT_DOWN,m_MainTaskBarIcon);
		m_MainTaskBarIcon->ProcessEvent(evt);
	}
	ShowMgr tempShowMgr(this); 

	LockFrameDlg dlg(NULL);
	m_LockFrameDlg=&dlg;
	dlg.ShowModal();
	m_LockFrameDlg = NULL;

	//��������ʱ��
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	if( pCfgMgr->GetMainWindowCfg()->bAutoLock )
	{
		g_StartIdleTime = GetTickCount();
	}
}
//���ڲ˵���Ӧ:�˳�
void MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close();
}
//ϵͳ��ť���رա�����Ӧ
void MainFrame::OnMainClose(wxCloseEvent& event)
{
	set<IPlatformSingleSvr*> svr_set;
	set<IPlatformSingleSvr*>::iterator svr_it;
	g_pPlatformMgr->GetAllPlatformSvr(svr_set);
    LOG_INFO("MainFrame::OnMainClose BEGIN");
	TRADEINFODLG_RELEASE();
	if(!m_bForceClose)
	{
		if ( event.CanVeto() )
		{
			wxString strMessage;
			BOOL bHasLocalOrder = FALSE;
			COrderServiceThread* poOrderServiceThread;
			poOrderServiceThread = COrderServiceThread::GetInstance();
			if(poOrderServiceThread)
			{
				poOrderServiceThread->LockObject();
				bHasLocalOrder = poOrderServiceThread->HasNoTouchLocalOrder();
				poOrderServiceThread->UnlockObject();
			}
			if(bHasLocalOrder)
				strMessage=LOADSTRING(MAINFRAME_CLOSEMSGBOX_MSG_HASLOCALORDER);
			else
				strMessage=LOADSTRING(MAINFRAME_CLOSEMSGBOX_MSG);
			int iret=wxMessageBox(strMessage,
				LOADSTRING(MAINFRAME_CLOSEMSGBOX_TITLE),
				wxYES_NO | wxICON_QUESTION, this);
			if(iret==wxNO)
			{
				TRADEINFODLG_CREATE(this);
				event.Veto();
				return;
			}
		}
	}
	else//�ⲿɱ��
	{
		HWND nHwnd=GetWindow(GetDesktopWindow(),GW_CHILD);
		while(nHwnd!=0)
		{
			wxWindow* pThis=wxWindow::FindItemByHWND(nHwnd);
			if(pThis)
			{
				wxWindow* pParent=pThis->GetParent();
				if(pParent&&pParent==this)
				{
					wxDialog* dlg=wxDynamicCast(pThis,wxDialog);
					if(dlg&&dlg->IsModal())
					{
						dlg->Close();
					}		
				}
			}
			nHwnd=GetWindow(nHwnd,GW_HWNDNEXT);
		}

	}
    m_timer.Stop();
	wxWindow* pWinOrder = FindWindowById(GID_ORDER_INSERT_PANEL, this);
	if(pWinOrder)
	{
		wxWindow* pParentWin = pWinOrder->GetParent();
		if(pParentWin->GetWindowStyle() & wxPOPUP_WINDOW) {
			pParentWin->Close();
		}
	}
	CTPLogin::LoadSaveServerParam(false);
	for (svr_it=svr_set.begin();svr_it!=svr_set.end();++svr_it)
	{
		(*svr_it)->UnSubscribeBusinessData(BID_RtnInstrumentStatus, GID_ANY);
		(*svr_it)->UnSubscribeBusinessData(BID_FrontDisconnected, GID_ANY);
	}

	SaveCommissionRateAndMarginRate();

	if(m_MainTaskBarIcon) m_MainTaskBarIcon->RemoveIcon();
	if(m_LockFrameDlg)	  m_LockFrameDlg->EndModal(0);
	wxDELETE(m_MainTaskBarIcon);
	if(m_configWizardProcessId)
	{
		::wxKill(m_configWizardProcessId);
	}
	if(m_CifcoServicesHallProcessId)
	{
		::wxKill(m_CifcoServicesHallProcessId);
	}
	if(m_RegisterHotKey)
	{
		UnregisterHotKey(HOTKEY_ID);
		m_RegisterHotKey=false;
	}
	wxGetApp().StopLogSystem();
	if(m_notebook!=NULL) 
	{
		SendSubscribeEventToChildren(false);
		std::string strAppPath;
		GlobalFunc::GetPath(strAppPath);

		strAppPath+=LAYOUT_FILENAME;
		m_notebook->SavePerspectiveToFile(strAppPath.c_str());
	}
	//�ر�Ƕ�봰��
	if(m_hypertextNoticeWnd&&IsWindow(m_hypertextNoticeWnd))
		SendMessage(m_hypertextNoticeWnd,WM_CLOSE,0,0);
    /////////////////////////////////////////////////////////////////////////////
	wxCommandEvent stopevt(wxEVT_STOP_PLATFORM);
	COrderServiceThread::GetInstance()->ProcessEvent(stopevt);
#ifdef _USE_TRACE_THREAD
	if(m_pTrackThread)
	{
		m_pTrackThread->Delete();
        m_pTrackThread=NULL;
	}
#endif
	if(m_pBeepThread)
	{
		m_pBeepThread->Delete();
		m_pBeepThread=NULL;
	}
    LOG_INFO("MainFrame::OnMainClose START DEFAULT_SVR()->StopThread");
	for (svr_it=svr_set.begin();svr_it!=svr_set.end();++svr_it)
	{
		(*svr_it)->StopThread();
	}
    LOG_INFO("MainFrame::OnMainClose START Uninitialize");
    Uninitialize();
    LOG_INFO("MainFrame::OnMainClose START DEFAULT_SVR()->Stop");
	for (svr_it=svr_set.begin();svr_it!=svr_set.end();++svr_it)
	{
		(*svr_it)->Stop();	
	}
    LOG_INFO("MainFrame::OnMainClose START IPlatformSvr::Release");
	//g_pPlatformMgr->ReleaseAccount();
/////////////////////////////////////////////////////////////////////////////////
	event.Skip(); 
    LOG_INFO("MainFrame::OnMainClose END");
}
//���ڲ˵���Ӧ���޸�����
void MainFrame::OnModifyPwd(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::ModifyPwd");
	ModifyPwdDlg dlg(this);
	dlg.ShowModal();

}

//���ڲ˵���Ӧ���޸ĵ�¼����
void MainFrame::ModifyMAPwd(wxCommandEvent& WXUNUSED(event))
{
	ChangeMAPwdDlg dlg(this);
	if( wxID_OK == dlg.ShowModal())
	{
		ModifyPwd modifyPwd;
		memset(&modifyPwd, 0, sizeof(modifyPwd));
		modifyPwd.nAccountID = MA_CORE()->m_pLoginRsp.nAccountID;

		m_strNewPwd = dlg.GetNewPwd();
		MD5 md5Pwd(m_strNewPwd.c_str());
		strcpy(modifyPwd.szPassword, md5Pwd.toString().c_str());
		CMAConnSocket::GetInstance()->SendData(Cmd_MA_ModifyPwd_Req, &modifyPwd, sizeof(modifyPwd));
	}
}

//wxFile::Exists(UPDATE_FILENAME)
//�����ļ��ı���
void MainFrame::OnStyleSave(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::OnConfigSave");
	if(m_notebook==NULL) return;
	//���浽��Ӧ�ļ�����
	AskNameDlg dlg(this,m_pImportExport);
	if(dlg.ShowModal()==wxID_OK)
	{
		wxWindow* pWinOrder = FindWindowById(GID_ORDER_INSERT_PANEL, this);
		if(pWinOrder)m_notebook->DoFloatPaneClose(pWinOrder);
		//������廹ԭ
		if ( m_notebook->IsConfigPaneOpen())
		{
			DoReturnNormalUI();
		}
		//���浽��ǰ·��
		m_notebook->SetLayoutCaption(dlg.GetCaption());
		m_notebook->SavePerspectiveToFile(m_pImportExport->GetLayoutFileName());
		m_pImportExport->DoConfigSave(dlg.GetCaption());
	}
	////��̬���²˵�
	wxMenuBar *menubar = GetMenuBar();
	wxMenu* pSubMenu=NULL,*pOptionMenu=NULL;

	//Ӣ��ʹ��&A��Ϊ��ݼ�����������ʹ��(&A)
	if(SVR_LANGUAGE==LANGUAGE_ENGLISH)
	{
		string DestText=LOADSTRING(ID_OPTION_CAPTION);
		EXCLUDE_AND_SIGNAL(DestText,'&');
		for(int pos=0;pos<(int)menubar->GetMenuCount();pos++)
		{
			string strTtile=menubar->GetLabelTop(pos).c_str();
			EXCLUDE_AND_SIGNAL(strTtile,'&');
			if(strTtile==DestText)
			{
				pOptionMenu=menubar->GetMenu(pos);
				break;
			}
		}
		if(pOptionMenu)
		{
			wxMenuItemList::compatibility_iterator node =pOptionMenu->GetMenuItems().GetLast();
			string LoadCaptionText=LOADSTRING(ID_CONFIG_LOAD_CAPTION);
			EXCLUDE_AND_SIGNAL(LoadCaptionText,'&');
			while(node)
			{
				string strText=node->GetData()->GetText().c_str();
				EXCLUDE_AND_SIGNAL(strText,'&');
				if(strText==LoadCaptionText)
				{
					pSubMenu=node->GetData()->GetSubMenu();
					break;
				}
				node=node->GetPrevious();
			}
			if(pSubMenu) m_pImportExport->CreateLayoutMenu(pSubMenu);
		}
	}
	else
	{
		wxString DestText=LOADSTRING(ID_OPTION_CAPTION);
		DestText=DestText.BeforeFirst('(');
		for(int pos=0;pos<(int)menubar->GetMenuCount();pos++)
		{
			wxString strTtile=menubar->GetLabelTop(pos);
			if(strTtile.BeforeFirst('(')==DestText)
			{
				pOptionMenu=menubar->GetMenu(pos);
				break;
			}
		}
		if(pOptionMenu)
		{
			wxMenuItemList::compatibility_iterator node =pOptionMenu->GetMenuItems().GetLast();
			wxString LoadCaptionText=LOADSTRING(ID_CONFIG_LOAD_CAPTION);
			LoadCaptionText=LoadCaptionText.BeforeFirst('(');
			while(node)
			{
				wxString strText=node->GetData()->GetText();
				if(strText.BeforeFirst('(')==LoadCaptionText)
				{
					pSubMenu=node->GetData()->GetSubMenu();
					break;
				}
				node=node->GetPrevious();
			}
			if(pSubMenu) m_pImportExport->CreateLayoutMenu(pSubMenu);
		}
	}

	PaintMenuButton(true);//CITHINC-1155

} 
void MainFrame::OnUpdateUIOnStyleChanged(wxUpdateUIEvent& event)
{
	static int StyleGroup[]={STYLE_SunGard,STYLE_Q7,STYLE_SIMPLE,STYLE_MOUSE};
    if(m_notebook==NULL) return;
    wxString str=m_notebook->GetLayoutCaption();
    if(str.IsEmpty())
    {
        int index=event.GetId()-ID_BASE_LAYOUT;
        if(index>=0&&index<4)
        {
            event.Check(m_notebook->GetLayoutStyle()==StyleGroup[index]);
        }
    }
    else
    {
        wxArrayString items;
        m_pImportExport->GetLayoutArray(items,false);
        int index=event.GetId()-(ID_BASE_LAYOUT+50);
        if(index>=0&&index<(int)items.GetCount())
            event.Check(str==items[index]);
    }
}
//������ı�
void MainFrame::OnStyleChanged(wxCommandEvent& event)
{
	WXLOG_INFO("MainFrame::OnConfigLoad");
    if(m_notebook==NULL) return;
	if(m_curConfigPanel) DoReturnNormalUI();
	wxWindow* pOrderPanel = GetModuleWindow(GID_ORDER_INSERT_PANEL);
	if (pOrderPanel&&m_notebook&&m_notebook->IsFloatPaneOpen(pOrderPanel) )
	{
		m_notebook->DoFloatPaneClose(pOrderPanel);
	}
	if ( m_notebook->IsCurStyleChanged() )
	{
		int nRet = wxMessageBox(LOADSTRING(SAVE_CURRENT_STYLE), LOADSTRING(USERLOGINDLG_INFORMATION), wxYES_NO | wxICON_QUESTION, this);
		if( nRet == wxYES )
		{
			OnStyleSave(event);
		}
	}
	ShowMgr tempShowMgr(this); 
	DoChangeStyle(event.GetId());
	m_notebook->LoadPerspectiveFromFile(m_pImportExport->GetLayoutFileName());
	DoStyleChanged(TYPE_APPLYCHANGED);
}
void MainFrame::DoStyleChanged(int nApplyType)
{
    if(s_pMainFrame==NULL||s_pMainFrame->m_notebook==NULL) return;
	s_pMainFrame->m_nDeltaOrderPanelSashPos = 0;
	s_pMainFrame->SendStyleChangeEventToChildren(nApplyType);
	//��������˵�
	if(s_pMainFrame->m_notebook->GetLayoutStyle()==STYLE_SunGard
		&& DEFAULT_SVR() && DEFAULT_SVR()->IsSunGardSupported())
		s_pMainFrame->CreateMenuBar_SunGard();
	else
		s_pMainFrame->CreateMenuBar_Q7(); 
	
}
//���ڲ˵���Ӧ������ʷ���㵥
void MainFrame::HisCal(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::HisCal");
	HisCalDlg dlg(this);
	dlg.ShowModal();
}
//���ڲ˵���Ӧ��ʹ��˵��
void MainFrame::OnHelpContent(wxCommandEvent& WXUNUSED(event))
{
	std::string apppath;
	GlobalFunc::GetPath(apppath);
	apppath+="help.chm";
	if(wxFileExists(apppath))
		ShellExecute(NULL,"open",apppath.c_str(),NULL,NULL,SW_SHOWNORMAL);
	else
		wxMessageBox(LOADSTRING(GUI_INSTRUCTION_FOR_USE),LOADSTRING(USERLOGINDLG_INFORMATION));
}
//���ڲ˵���Ӧ������
void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::OnAbout");
	AboutDlg dlg(this);
	dlg.ShowModal();
}
//���ڲ˵���Ӧ�����ڻ��˻��ʽ�
void MainFrame::OnQryAccount(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::OnQryAccount");
	wxWindow *p=FindWindowById(GID_ACCOUNT);
	if(p)
	{
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED,wxID_OK);	
		p->AddPendingEvent(evt);
	}
}
//��������ת�˶Ի���
void MainFrame::OnBankFutureTransfer(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::OnBankFutureTransfer");
	if(m_pBankFutureTransferDlg)
		wxDELETE(m_pBankFutureTransferDlg);
	m_pBankFutureTransferDlg= new CBankFutureTransferDlg(this);

	m_pBankFutureTransferDlg->Center();
	m_pBankFutureTransferDlg->ShowModal();
}

//������ѯ�˺ŶԻ���
void MainFrame::OnQueryTrader(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::OnQueryTrader");
	CQueryTraderDlg dlg(this);
	dlg.ShowModal();
}
//������ѯ�������Ի���
void MainFrame::OnQueryExchange(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::OnQueryExchange");
    CQueryExchangeDlg dlg(this);
 	dlg.ShowModal();   
}
//������ѯ��Լ�Ի���
void MainFrame::OnQueryContract(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::OnQueryContract");
    CQueryContractDlg dlg(this);
 	dlg.ShowModal(); 
}
//������ѯ��Ʒ�Ի���
void MainFrame::OnQueryCommodity(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::OnQueryCommodity");
	CQueryCommodityDlg dlg(this);
	dlg.ShowModal();
}
//������ѯ�������ͶԻ���
void MainFrame::OnQueryOrderType(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::OnQueryOrderType");
	CQueryOrderTypeDlg dlg(this);
	dlg.ShowModal();
}
//������ѯ�ֲ����ͶԻ���
void MainFrame::OnQueryPosition(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::OnQueryPosition");
	CQueryPositionDlg dlg(this);
	dlg.ShowModal();
}

//������ѯ�ֲ����ͶԻ���
void MainFrame::OnQueryReport(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::OnQueryReport");
	CQueryReportDlg dlg(this);
	dlg.ShowModal();
}

//������ѯ���ʶԻ���
void MainFrame::OnQueryExchangeRate(wxCommandEvent& event)
{
	WXLOG_INFO("MainFrame::OnQueryExchangeRate");
	CQueryExchangeRateDlg dlg(this);
	dlg.ShowModal();
}
//���ڲ˵���Ӧ:ѡ������
void MainFrame::OnConfigOption(wxCommandEvent& WXUNUSED(event))
{
	ShowConfigAppDlg();
}
void MainFrame::ShowConfigAppDlg(int id)
{
    if(m_notebook==NULL) return;
	IPlatformSingleSvr* pSvr=g_pPlatformMgr->GetCurrentPlatformSvr();
	WXLOG_INFO("MainFrame::OnConfigOption");
	wxWindow* pWinOrder = FindWindowById(GID_ORDER_INSERT_PANEL, this);
	m_notebook->DoFloatPaneClose(pWinOrder);
	//�������ò���
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
	m_MainFrameCfg=pCfgMgr->GetMainWindowCfg();         
	m_notebook->FixedPane(m_MainFrameCfg->bFixPanel);
	CConfigAppDlg dlg(this,id,pSvr);
	dlg.ShowModal();
}
void MainFrame::DoChangeStyle(int cmdid)
{
	if(m_pImportExport)
	{
		bool bQ7=m_pImportExport->DoChangeStyle(cmdid);
		
		if(IsMaximized())
		{
			int nWidth = bQ7 ? 1024 : 800;
			int nHeiht = bQ7 ? 768 : 600;

			WINDOWPLACEMENT placement;
			GetWindowPlacement((HWND)m_hWnd, &placement);
			placement.rcNormalPosition.right = placement.rcNormalPosition.left + nWidth;
			placement.rcNormalPosition.bottom = placement.rcNormalPosition.top + nHeiht;
			SetWindowPlacement((HWND)m_hWnd, &placement);

			Restore();
		}
		else
		{
			Freeze();
			SetSize(bQ7?wxSize(1024,768):wxSize(800,600));
			Thaw();
		}
	}
}
void MainFrame::OnApplyDefaultConfig(wxCommandEvent& event)
{
	WXLOG_INFO("MainFrame::OnApplyDefaultConfig");
	if(wxMessageBox(LOADSTRING(GUI_APPLYDEFAULT),LOADSTRING(USERLOGINDLG_INFORMATION),wxYES_NO)==wxYES)
	{
		//wxCopyFile(m_pImportExport->GetSystemCfgFileName(),m_pImportExport->GetCfgFileName() );
		wxRemoveFile(m_pImportExport->GetCfgFileName());
		ShowMgr tempShowMgr(this); 
		if(m_curConfigPanel) DoReturnNormalUI();
		wxWindow* pOrderPanel = GetModuleWindow(GID_ORDER_INSERT_PANEL);
		if (pOrderPanel&&m_notebook&&m_notebook->IsFloatPaneOpen(pOrderPanel) )
		{
			m_notebook->DoFloatPaneClose(pOrderPanel);
		}
		CFG_MGR_DEFAULT_REINITCFG();
		m_curConfigPanelClassName = "";
		AsyncHandyOrderMapAndKeyMultiplyVolumeMap();
		SendCfgXmlChangeEventToChildren();
		CustomDefineUI();
		DoChangeStyle(ID_BASE_LAYOUT);


		m_notebook->LoadPerspectiveFromFile(m_pImportExport->GetLayoutFileName());
		DoStyleChanged(TYPE_APPLYDEFAULT);
	}
}

wxString MainFrame::GetMainFrameCaption()
{
	string strInvestorID,strBrokerID;
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetBrokerIDUserID(strInvestorID,strBrokerID);
	wxString strMainFrameTitle=LOADSTRING(MAINFRAME_TITLE);
	strMainFrameTitle+="  -  ";
	strMainFrameTitle+=strInvestorID.c_str();
	return strMainFrameTitle;
}
//��ʼ������
bool MainFrame::Initialize(wxWindow *parent,
						   wxWindowID id,
						   const wxString& title,
						   const wxPoint& pos ,
						   const wxSize& size,
						   long style,
						   const wxString& name)
{
	WXLOG_INFO("MainFrame::Initialize"); 
	m_OptionCaption=LOADSTRING(ID_OPTION_CAPTION);
	InitializeNCButton();
	if(!Create(parent,id,GetMainFrameCaption(),pos,size,style,name))
		return false;
	m_MainTaskBarIcon=new MainTaskBarIcon(this);
	SetIcon(wxIcon(_T("zqICON_MAIN")));
	SetMainHook(this,true);

	wxWindow* notebook_parent=this;
#ifdef _USE_TOP_SPLITTER
	notebook_parent = new wxSplitterWindow(this);
	if(!CreateRightSplitter(notebook_parent))
		return false;
#endif
	if(!CreateNotebook(notebook_parent)) 
		return false; 
	
	if( !CreateMyStatusBar() )
		return false;
#ifdef _USE_TOP_SPLITTER
	if(m_notebook&&m_right_splitter_wnd)
	{
		((wxSplitterWindow*)notebook_parent)->SetSashGravity(1.0);
		if(m_bShowVideoInfo)
		{
			((wxSplitterWindow*)notebook_parent)->SplitVertically(m_notebook,m_right_splitter_wnd);
		}
		else
		{
			((wxSplitterWindow*)notebook_parent)->Initialize(m_notebook);
		}
		((wxSplitterWindow*)notebook_parent)->SetMinimumPaneSize(350);
	}
#endif
	DoStyleChanged(TYPE_APPLYINIT);
	SetAutoLayout(true);
	AsyncHandyOrderMapAndKeyMultiplyVolumeMap();
	CustomDefineUI(); 
	LoadHypertextNotice(m_NoticeContent,true);
	m_pTradingNoticeDlg=new TradingNoticeDlg(this);
	if(!m_TradingNoticeArray.empty())
	{
		wxString strContent;
		std::map<int,std::string>::iterator it=m_TradingNoticeArray.begin();
		for(it;it!=m_TradingNoticeArray.end();++it)
		{
			strContent+=it->second.c_str();
			strContent+="\r\n";
		}
		m_TradingNoticeArray.clear();
		m_pTradingNoticeDlg->SetContent(strContent);
		m_pTradingNoticeDlg->CenterOnScreen();
		m_pTradingNoticeDlg->Show();
	}
	SendSubscribeEventToChildren(true);
	TRADEINFODLG_CREATE(this);//���С��ί��ʱ�������������ڲ���MainFrame������
#ifdef _USE_MULTI_LANGUAGE
	ChangeLanguage(m_languageId);
#endif

	if(DEFAULT_SVR()&&DEFAULT_SVR()->SupportPlugins())
	{
		g_Plusin.Init(this, CreatePlugPane, DeletePlugPane);
		wxCommandEvent evtLoadPlusIn(wxEVT_LOAD_PLUSIN);
		AddPendingEvent(evtLoadPlusIn);
	}
	//���FASTTRADER-1340 ���̣��ö������µ�¼�����նˣ��ö�ʧЧ��
	CfgMgr* pMgr = CFG_MGR_DEFAULT();
	if ( pMgr )
	{
		LPMAIN_WINDOW_CFG pMainWindowCfg = pMgr->GetMainWindowCfg();
		SetWindowStyle(pMainWindowCfg->bIsTop?(GetWindowStyle()|wxSTAY_ON_TOP):(GetWindowStyle()&(~wxSTAY_ON_TOP)));
	}

	//����δ���յ���Ϣ�б�
	SYSTEMTIME time;
	GetLocalTime(&time);
	char szBuffer[100];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "%4d%02d%02d", time.wYear, time.wMonth, time.wDay);
	LoadSavedMessages(szBuffer, m_vMsgInfo);

	CMAConnSocket::GetInstance()->RegisteWindow(this);
	int nAccountID = MA_CORE()->m_pLoginRsp.nAccountID;
	CMAConnSocket::GetInstance()->SendData(Cmd_MA_QryUnReadMessage_Req, &nAccountID, sizeof(nAccountID));

	wxCommandEvent dulplicateKeyEvt(wxEVT_DULPLICATE_KEY_PROMPT);
	AddPendingEvent(dulplicateKeyEvt);

	return true;
}
void MainFrame::Uninitialize()
{
	CMAConnSocket::GetInstance()->UnRegisteWindow(this);
	CMAConnSocket::DestroyInstance();
	SaveMessage();

    if(DEFAULT_SVR()&&DEFAULT_SVR()->SupportPlugins())
	{
		g_Plusin.UnInit();
	}
	WXLOG_INFO("MainFrame::Uninitialize"); 
	RestorePosition();


    SAFE_DELETE(m_notebook);
	MyStatusBar* pBar=(MyStatusBar*)GetStatusBar();
    SAFE_DELETE(pBar);
}
//����״̬��
bool MainFrame::CreateMyStatusBar()
{ 
	WXLOG_INFO("MainFrame::CreateMyStatusBar");
	MyStatusBar* pStaBar = new MyStatusBar(this);
	SetStatusBar(pStaBar);

	if ( NULL != LogDlg::GetInstance())
	{
		for( int i = 0; i < (int)m_vLogData.size(); i++)
		{
			WriteUserLog( m_vLogData.at(i).strLogType, m_vLogData.at(i).pData );
		}

		m_vLogData.clear();
	}

	return true;
}
bool MainFrame::NotebookAddPage(int id,wxWindow** win,wxString& caption)
{
	*win=s_pMainFrame->GetModuleWindow(id);
	caption=s_pMainFrame->GetModuleCaption(id);
	return *win!=NULL;
}
//�������ؼ�Notebook
bool MainFrame::CreateNotebook(wxWindow * pParent)
{
	WXLOG_INFO("MainFrame::CreateNotebook");
	COrderServiceThread::GetInstance()->InitService();
	m_notebook = new zqAuiNotebook(pParent);
    if(m_notebook==NULL) return false;
	m_notebook->SetCallbackFunc(NotebookAddPage,DoStyleChanged);

	std::string strAppPath;
	GlobalFunc::GetPath(strAppPath);
	strAppPath+=LAYOUT_FILENAME;
	m_notebook->LoadPerspectiveFromFile(strAppPath.c_str());
	CustomDefineUI();
	return true;
}
#ifdef _USE_TOP_SPLITTER

bool MainFrame::CreateRightSplitter(wxWindow * pParent)
{
	m_right_splitter_wnd=new MultiSplitter(pParent);
	m_right_splitter_wnd->Show(m_bShowVideoInfo);
	return true;
}
#endif
wxWindow* MainFrame::GetModuleWindow(int ModuleID)
{
    if(m_notebook==NULL) return NULL;
	wxWindow* pPanel=m_mapModuleWindow[ModuleID];
	if(pPanel!=NULL) return pPanel;
	
	switch(ModuleID)
	{
	case GID_QUOT_TABLE://���۱�
		pPanel=new CQuotFrame(m_notebook);
		break;
	case GID_ORDER_INSERT_PANEL://�µ����
		pPanel=OrderInsertPanel::CreatePane(m_notebook);
		break;
	case GID_POSITIONALL://�ֲ�
		pPanel=new PositionPanel(m_notebook);
		break;
	case GID_INSTRUMENT://��Լ�б�
		pPanel=new InstrumentPanel(m_notebook,GID_INSTRUMENT);
		break;
	case GID_THRADE_RECORD://�ɽ���¼
		pPanel=new TradeRecordPanel(m_notebook,GID_THRADE_RECORD);
		break;
	case  GID_ACCOUNT://�ʽ��˻�
		pPanel=FundAccountModule::CreatePane(m_notebook);
		break;
	case GID_OPEN_ORDER://δ�ɽ���
		//pPanel=new COpenOrder(m_notebook,GID_OPEN_ORDER);
		pPanel=new CAllOrdersPanel(CAllOrdersPanel::conOpenOrderStyle, m_notebook, GID_OPEN_ORDER);
		break;
	case GID_ALL_ORDER://ί�е�
		pPanel=new CAllOrdersPanel(CAllOrdersPanel::conAllOrderStyle, m_notebook, GID_ALL_ORDER);
		break;
	case GID_QUERY_ORDER_BATCH://������
		if((pPanel=FindWindowById(GID_QUERYLOCAL_ORDER, this))==NULL)
			new CQueryLocalOrderPanel(m_notebook);
		pPanel=FindWindowById(GID_QUERY_ORDER_BATCH, this);
		TRACE("MainFrame::GetModuleWindow(GID_QUERY_ORDER_BATCH)\n");
	//	pPanel=new CQueryOrderBatchPanel(m_notebook);
		break;
	case GID_QUERY_ORDER_POSITION://ӯ��
		if((pPanel=FindWindowById(GID_QUERYLOCAL_ORDER, this))==NULL)
			new CQueryLocalOrderPanel(m_notebook);
		pPanel=FindWindowById(GID_QUERY_ORDER_POSITION, this);
		TRACE("MainFrame::GetModuleWindow(GID_QUERY_ORDER_POSITION)\n");
	//	pPanel=new CQueryOrderPositionPanel(m_notebook);
		break;
	case GID_QUERY_ORDER_CONDITION://������
		if((pPanel=FindWindowById(GID_QUERYLOCAL_ORDER, this))==NULL)
			new CQueryLocalOrderPanel(m_notebook);
		pPanel=FindWindowById(GID_QUERY_ORDER_CONDITION, this);
		TRACE("MainFrame::GetModuleWindow(GID_QUERY_ORDER_CONDITION)\n");
	//	pPanel=new CQueryOrderConditionPanel(m_notebook);
		break;
	case GID_QUERY_ORDER_PARKED://Ԥ��
		if((pPanel=FindWindowById(GID_QUERYLOCAL_ORDER, this))==NULL)
			new CQueryLocalOrderPanel(m_notebook);
		pPanel=FindWindowById(GID_QUERY_ORDER_PARKED, this);
		TRACE("MainFrame::GetModuleWindow(GID_QUERY_ORDER_PARKED)\n");
	//	pPanel=new CQueryOrderParkedPanel(m_notebook);
		break;
	case GID_QUERYLOCAL_ORDER:
		if((pPanel=FindWindowById(GID_QUERYLOCAL_ORDER, this))==NULL)
			pPanel=new CQueryLocalOrderPanel(m_notebook);
		TRACE("MainFrame::GetModuleWindow(GID_QUERYLOCAL_ORDER)\n");
		break;
	case GID_MULTI_ACCOUNT://���˺�ģ��
		pPanel=new MultiAccountPanel(m_notebook,GID_MULTI_ACCOUNT);
		break;	
	case GID_MULTI_POSITION://�ֲֻ���ģ��
		pPanel=new MultiPositionPanel(m_notebook,GID_MULTI_POSITION);
		break;	
	default:
		break;
	}
	
	if ( NULL == pPanel )
	{
		return NULL;
	}
	m_mapModuleWindow[ModuleID]=pPanel;
	return pPanel;
}
wxString MainFrame::GetModuleCaption(int ModuleID)
{
#define CASE_RETURN(cap) case cap:return LOADSTRING(cap##_CAPTION)
	wxString str;
	switch(ModuleID)
	{
		CASE_RETURN(GID_QUOT_TABLE);
		CASE_RETURN(GID_ORDER_INSERT_PANEL);
		CASE_RETURN(GID_POSITIONALL);
		CASE_RETURN(GID_INSTRUMENT);
		CASE_RETURN(GID_THRADE_RECORD);
		CASE_RETURN(GID_ACCOUNT);
		CASE_RETURN(GID_OPEN_ORDER);
		CASE_RETURN(GID_ALL_ORDER);
		CASE_RETURN(GID_QUERYLOCAL_ORDER);
		CASE_RETURN(GID_QUERY_ORDER_BATCH);
		CASE_RETURN(GID_QUERY_ORDER_CONDITION);
		CASE_RETURN(GID_QUERY_ORDER_POSITION);
		CASE_RETURN(GID_QUERY_ORDER_PARKED);
		CASE_RETURN(GID_POSITIONCOMB);
		CASE_RETURN(GID_MULTI_ACCOUNT);
		CASE_RETURN(GID_MULTI_POSITION);
	}
	return str;
}
bool MainFrame::SelectPageFromAccel(int keycode)
{
	std::map<int,int>::iterator it;
	int pageid=-1;
	for(it=m_ID2AccelMap.begin();it!=m_ID2AccelMap.end();++it) {
		if(it->second==keycode) {
			pageid=it->first;
			break;
		}
	}
	if(pageid>0) {
		wxWindow* window=FindWindowById(pageid,this);
		if(m_notebook&&window)
		{
			m_notebook->SetSelection(window);
			wxCommandEvent myEvent(wxEVT_PANEL_FOCUS);
			window->ProcessEvent(myEvent);
		}
		return true;
	}
	return false;
}
//����CHAR_HOOK��ָ������
bool MainFrame::TransmitCharHook(wxWindow *panel,wxKeyEvent& event)
{
	if(!wxDynamicCast(panel,wxWindow)) return false;                // ����panelΪNULL������������� added by l. 20110412
	WXLOG_INFO("TransmitCharHook(%s,%d)",panel->GetLabel(),event.GetKeyCode());
	bool bDeal=false;
	//wxWindow *win = FindFocus();
	//if(win == NULL) return false;
	//WXLOG_INFO("FindFocus()=0x%08X,%s",win,win->GetLabel());
	//while(win)
	//{
	//	if(win==panel)
	//	{
	//		wxCommandEvent myEvent(wxEVT_PANEL_CHAR_HOOK);
	//		myEvent.SetClientData(new wxKeyEvent(event));
	//		panel->ProcessEvent(myEvent); 
	//		return !myEvent.GetSkipped();
	//	}
	//	win=win->GetParent();
	//}
	// ����״̬�µİ��������͸��µ��壬�Է����µ��崦���ݼ�
	//if(win)	{
		wxCommandEvent myEvent(wxEVT_PANEL_CHAR_HOOK);
		myEvent.SetClientData(new wxKeyEvent(event));
		panel->ProcessEvent(myEvent); 
		return !myEvent.GetSkipped();
	//}
	return false;  
}
//������̹��ܼ�����
void MainFrame::OnCharHook (wxKeyEvent& event)
{
    if(m_notebook==NULL) return;
	int keycode=event.GetKeyCode();
	WXLOG_INFO("MainFrame::OnCharHook(%d)",keycode);
	SelectPageFromAccel(keycode);
	if(m_curConfigPanel)
	{
		if(!TransmitCharHook(m_curConfigPanel,event))
			return;
	}
	wxWindow* pWinOpen = FindWindowById(GID_OPEN_ORDER, this);
	if(pWinOpen)
	{
		if(TransmitCharHook(pWinOpen, event))
			return;
	}
	wxWindow* pWinALLOrder = FindWindowById(GID_ALL_ORDER, this);
	if(pWinALLOrder)
	{
		if(TransmitCharHook(pWinALLOrder, event))
			return;
	}
	wxWindow* pWinMultiAccount = FindWindowById(GID_MULTI_ACCOUNT, this);
	if(pWinMultiAccount)
	{
		if(TransmitCharHook(pWinMultiAccount, event))
			return;
	}
	wxWindow* pWinOrder = FindWindowById(GID_ORDER_INSERT_PANEL, this);
	if(pWinOrder)
	{
		bool bDeal=TransmitCharHook(pWinOrder, event);
		if(!bDeal) 
			event.Skip();
	}


	// ������̿����µ�
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return;
	LPORDER_CFG p = pMgr->GetOrderCfg();

	wxString strCurrCode = _("");

	// ���Ȼ�ñ�۱�ĵ�ǰ��Լ����
	// ...

	// �ж��Ƿ����ü��̿����µ�
	if(p->bOpenQuotKeyBoardOrder) {
		bool bIsCurrQuotList =m_notebook->IsSelected(GetModuleWindow(GID_QUOT_TABLE));
		map<int,string>::iterator itKey;
		//��ü������ַ�����ӳ���
		map<int,string> addr = pMgr->GetShortCutKeyNameMap();
		itKey = addr.find(keycode);
		if(itKey != addr.end()) {
			std::string strKeyName = itKey->second;
			std::map<std::string, KeyOrderInfo>::iterator itOrder;
			itOrder = m_KeyFuncMap.find(strKeyName);
			if(itOrder != m_KeyFuncMap.end()) {
				wxString strArea;
				KeyOrderInfo keyOrder;
				keyOrder = itOrder->second;
				if(keyOrder.nActionArea==0 || bIsCurrQuotList) {
					strCurrCode = GetQuotCurInstrumentID();

//					KeyOrderInfo* pKeyOrder= new KeyOrderInfo;
//					*pKeyOrder = keyOrder;
					wxCommandEvent sendevt(wxEVT_ORDERINSERT_KEYORDER);
//					sendevt.SetString(strCurrCode.c_str());
//					sendevt.SetClientData(pKeyOrder);

                    if(1)
                    {
	                    DWORD EvtParamID;
	                    std::string EvtParamStr(strCurrCode.c_str());
	                    CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,&keyOrder,sizeof(KeyOrderInfo),0);
	                    sendevt.SetInt((int)EvtParamID);
                    }

                    
                    if(pWinOrder)
						pWinOrder->ProcessEvent(sendevt);

					//m_poOrderInsertPanel->DoKeyOrder(strCurrCode, keyOrder);
					//if(p->bReturnQuotTable) {
					//	wxWindow* pWinQuot = FindWindowById(GID_QUOT_TABLE, this);
					//	if(pWinQuot)
					//		pWinQuot->SetFocus();
					//}
				}
			}
		}
	}

	////	// �ж��Ƿ����ü��̿����µ�
	////	if(p->bOpenQuotKeyBoardOrder) 
	//	{
	//		map<int,string>::iterator itKey;
	//		//��ü������ַ�����ӳ���
	//		map<int,string>* pAddr = pMgr->GetShortCutKeyNameMap();
	//		itKey = pAddr->find(keycode);
	//		if(itKey != pAddr->end()) {
	//			std::string strKeyName = itKey->second;
	//			std::map<std::string, int>::iterator itOrder;
	//			itOrder = m_KeyMultiplyVolumeMap.find(strKeyName);
	//			if(itOrder != m_KeyMultiplyVolumeMap.end()) {
	//				int nVolume = itOrder->second;
	//				m_poOrderInsertPanel->SetMultiplyVolume(nVolume);
	//			}
	//		}
	//	}

}

void MainFrame::OnQuotSetFocus(wxCommandEvent& evt)
{
	wxWindow* pWinQuot = FindWindowById(GID_QUOT_TABLE, this);
	if(pWinQuot)
		pWinQuot->SetFocus();
}

//nSel >=0 ����ָ��ҳ��
//nSel <0  �����µ����
void MainFrame::FloatPaneOpen()
{
	if(m_configWizardProcessId)
	{
		::wxKill(m_configWizardProcessId);
	}
    if(m_notebook==NULL) return;
	wxRect rect(GetSize().x-240,24,200,335);
	OrderInsertPanel* pWinOrder = (OrderInsertPanel*)FindWindowById(GID_ORDER_INSERT_PANEL, this);
	if(pWinOrder!=NULL) {
        BOOL bFivePriceList=COrderInsertOperations::NeedFivePriceList();
        int Wid,Hei;
        RECT ScnRect;
        ::GetWindowRect(::GetDesktopWindow(),&ScnRect);
		switch(pWinOrder->GetViewSubStyle()) {
		case 0:
            Wid=355;
//			rect = wxRect(GetSize().x-350-20,24,355,335);
			break;
		case 1:
            Wid=355;
//			rect = wxRect(GetSize().x-350-20,24,355,335);
			break;
		case 2:
            Wid=355;
//			rect = wxRect(GetSize().x-350-20,24,355,335);
			break;
		case 3:
            Wid=576;
//			rect = wxRect(GetSize().x-350-20-200,24,355+200,335);
			break;
		}
        Hei=320;
        if(!bFivePriceList)
            Wid-=160;

        rect = wxRect(ScnRect.right-20-Wid,20,Wid,Hei);
//		rect = wxRect(GetSize().x-350-20,24,355,335);


		m_notebook->DoFloatPaneOpen(pWinOrder,rect);
		m_notebook->FixedPane(true);//��������
	}
}
//������嵯����Ӧ
void MainFrame::OnFloatPaneOpen(wxAuiNotebookEvent& event)
{
	FloatPaneOpen();

	m_bMaxBak_MainFrame = IsMaximized();
	m_posBak_MainFrame = GetPosition();
	wxWindow* pWinOrder = FindWindowById(GID_ORDER_INSERT_PANEL, this);
	wxWindow* pParentWin = pWinOrder->GetParent();
	long nStyle = pParentWin->GetWindowStyle();
	nStyle = nStyle | wxPOPUP_WINDOW;
	pParentWin->SetWindowStyle(nStyle);

	SetMainHookForDialog(pWinOrder, TRUE);
	//	SetMainHookForOrderInsertPanel(true);

	//	m_poOrderInsertPanel->BringWindow2TopAlways();

	event.Veto();
}
//��������ջ���Ӧ
void MainFrame::OnFloatPaneClose(wxAuiManagerEvent& event)
{ 
    if(m_notebook==NULL) return;
	SetPosition(m_posBak_MainFrame);
	Maximize(m_bMaxBak_MainFrame>0);
	SetMainHookForDialog(NULL, FALSE);
	//�������ò���
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();  
	m_MainFrameCfg=pCfgMgr->GetMainWindowCfg();         
	m_notebook->FixedPane(m_MainFrameCfg->bFixPanel);
}
//д״̬��log��¼
void MainFrame::WriteUserLog(string strType, LOG_DATA* pData, string strTime)
{
	LogDlg* pDlg = LogDlg::GetInstance();
	if ( NULL == pDlg )
	{
		return;
	}
	else
	{
		pDlg->WriteLog(strType, pData, strTime);
	}
}
//��Լ����״̬֪ͨ�ص�����
int MainFrame::InstrumentStatusCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)
{
	if(data.BID!=BID_RtnInstrumentStatus || data.Size!=sizeof(DataInstrumentStatus))
		return 0;	

	DataInstrumentStatus& refData = *(DataInstrumentStatus*)(const_cast<AbstractBusinessData*>(&data));	
	CThostFtdcInstrumentStatusField* pField=new CThostFtdcInstrumentStatusField();
	memcpy(pField,&(refData.InstrumentStatusField),sizeof(CThostFtdcInstrumentStatusField));
	wxCommandEvent event(wxEVT_INSTRUMENT_STATUS,wxID_ANY);
	event.SetClientData((void*)pField);
	s_pMainFrame->AddPendingEvent(event);
	return 0;
}
//��Լ����״̬֪ͨ��Ӧ
void MainFrame::OnInstrumentStatus(wxCommandEvent& evt)
{
	CThostFtdcInstrumentStatusField* pField=(CThostFtdcInstrumentStatusField*)evt.GetClientData();
	SYSTEMTIME lp;
	GetLocalTime(&lp);
	char timebuf[100];
	sprintf(timebuf,LOG_TIME_FORMAT_STRING,lp.wYear,lp.wMonth,lp.wDay,lp.wHour,lp.wMinute,lp.wSecond,lp.wMilliseconds);
	string strTitle="";
    if(DEFAULT_SVR())
    	strTitle= (DEFAULT_SVR()->ExchangeID2String(pField->ExchangeID)).c_str();
	string strFormat;
	switch(pField->InstrumentStatus)
	{
	case THOST_FTDC_IS_BeforeTrading:
		strFormat = "LOG_STR_BEFORE_TRADE";
		break;
	case THOST_FTDC_IS_NoTrading:
		strFormat = "LOG_STR_NO_TRADE";
		break;
	case THOST_FTDC_IS_Continous:
		strFormat = "LOG_STR_CONTINOUS";
		break;
	case THOST_FTDC_IS_AuctionOrdering:
		strFormat = "LOG_STR_AUCTION_ORDERING";
		break;
	case THOST_FTDC_IS_AuctionBalance:
		strFormat = "LOG_STR_AUCTION_BALANCE";
		break;
	case THOST_FTDC_IS_AuctionMatch:
		strFormat = "LOG_STR_AUCTION_MATCH";
		break;
	case THOST_FTDC_IS_Closed:
		strFormat = "LOG_STR_CLOSE";
		break;
	}

	LOG_DATA* pData = new LOG_DATA;
	pData->strTitle = strTitle;
	pData->strFormat = strFormat;
	if( NULL == s_pMainFrame->GetStatusBar() || NULL == LogDlg::GetInstance())
	{
		USER_LOG userLog;
		userLog.strLogType = "LOG_TYPE_GLOBAL";
		userLog.pData = pData;
		m_vLogData.push_back(userLog);
	}
	else
	{
		s_pMainFrame->WriteUserLog( "LOG_TYPE_GLOBAL", pData, timebuf);
	}

	delete pField;
	// ʵ�� �������̼�С����Ϣ��������ʾ
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if(pMgr)
	{
		SystemParamInfo* p = pMgr->GetSystemParamInfo();
		if(p->nTradeTimeSoundPrompt)
		{
			PostBeepMessage(2);
		}
	}
}
//�뽻�׺�̨ͨ�����ӶϿ��¼���Ӧ
void MainFrame::OnFrontDisconnected(wxCommandEvent& evt)
{
	if(GetHwnd()==NULL) return;
	int nReason=(int)evt.GetClientData();
	if(nReason==0) return;
	if(m_pDisConnPromptDlg==NULL)
		m_pDisConnPromptDlg=new DisConnPromptDlg(this);
	m_pDisConnPromptDlg->CenterOnScreen();
	if(IsShown()&&IsIconized()) Restore();//CITHINC-1179 
	m_pDisConnPromptDlg->Show();
}
//�뽻�������ӣ��Ͽ��͵�¼��Ӧ
void MainFrame::OnConnectLogin(wxCommandEvent& evt)
{
    std::string EvtParamStr;
    if(1)
    {
        DWORD EvtParamID=(DWORD)evt.GetInt();
        if(CEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        {
            CEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }

    wxString Msg(EvtParamStr);

	LOG_DATA* pData = new LOG_DATA;
	pData->strFormat = Msg.c_str();
	if( NULL == s_pMainFrame->GetStatusBar() || NULL == LogDlg::GetInstance())
	{
		USER_LOG userLog;
		userLog.strLogType = "LOG_TYPE_SYSTEM";
		userLog.pData = pData;
		m_vLogData.push_back(userLog);
	}
	else
	{
		s_pMainFrame->WriteUserLog( "LOG_TYPE_SYSTEM", pData);
	}
}
//�뽻�׺�̨ͨ�����ӳɹ��ص�����
int MainFrame::FrontConnectedCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)
{
	if(data.BID!=BID_FrontConnected || data.Size!=sizeof(DataFrontConnected))
		return 0;	
	DataFrontConnected& refData = *(DataFrontConnected*)(const_cast<AbstractBusinessData*>(&data));	
	wxCommandEvent event(wxEVT_CONNECT_LOGIN,wxID_ANY);
	if(refData.bTrader)
    {
        DWORD EvtParamID;
        std::string EvtParamStr("LOG_STR_HOST_SERVER_CONNECTED");
        CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0);
        event.SetInt((int)EvtParamID);
    }
	else
    {
        DWORD EvtParamID;
        std::string EvtParamStr("LOG_STR_QUOTE_SERVER_CONNECTED");
        CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0);
        event.SetInt((int)EvtParamID);
    }
	s_pMainFrame->AddPendingEvent(event);
	return 0;
}
//�뽻�׺�̨ͨ�����ӶϿ��ص�����
int MainFrame::FrontDisconnectedCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)
{
	if(data.BID!=BID_FrontDisconnected || data.Size!=sizeof(DataFrontDisconnected))
		return 0;	
	DataFrontDisconnected& refData = *(DataFrontDisconnected*)(const_cast<AbstractBusinessData*>(&data));	
	wxCommandEvent event(wxEVT_FRONT_DISCONNECTED,wxID_ANY);
	event.SetClientData((void*)refData.nReason);
	s_pMainFrame->AddPendingEvent(event);
	wxCommandEvent evt(wxEVT_CONNECT_LOGIN,wxID_ANY);

    DWORD EvtParamID;
    std::string EvtParamStr(refData.bTrader?"LOG_STR_HOST_SERVER_DISCONNECTED":"LOG_STR_QUOTE_SERVER_DISCONNECTED");
    CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0);
    evt.SetInt((int)EvtParamID);

//    if(refData.bTrader)
//		evt.SetString("LOG_STR_HOST_SERVER_DISCONNECTED");
//	else
//		evt.SetString("LOG_STR_QUOTE_SERVER_DISCONNECTED");
	s_pMainFrame->AddPendingEvent(evt);
	return 0;
}
int MainFrame::RspUserLoginCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)
{
	if(data.BID!=BID_RspUserLogin || data.Size!=sizeof(DataRspUserLogin))
		return 0;	
	DataRspUserLogin& refData = *(DataRspUserLogin*)(const_cast<AbstractBusinessData*>(&data));	
	wxCommandEvent evt(wxEVT_CONNECT_LOGIN,wxID_ANY);
    DWORD EvtParamID;
    std::string EvtParamStr(refData.bTrader?"LOG_STR_HOST_SERVER_LOGON":"LOG_STR_QUOTE_SERVER_LOGON");
    CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0);
    evt.SetInt((int)EvtParamID);

//    if(refData.bTrader)
//		evt.SetString("LOG_STR_HOST_SERVER_LOGON");
//	else
//		evt.SetString("LOG_STR_QUOTE_SERVER_LOGON");
	s_pMainFrame->AddPendingEvent(evt);
	return 0;
}
//����֪ͨ�ص�����
int MainFrame::TradingNoticeCallFunc(const GUIModuleID GID,const AbstractBusinessData &data)
{
	if(data.BID!=BID_RtnTradingNotice || data.Size!=sizeof(DataTradingNoticeInfoField))
		return 0;	
	DataTradingNoticeInfoField& refData = *(DataTradingNoticeInfoField*)(const_cast<AbstractBusinessData*>(&data));	
	CThostFtdcTradingNoticeInfoField* pField=new CThostFtdcTradingNoticeInfoField();
	memcpy(pField,&(refData.TradingNoticeInfoField),sizeof(CThostFtdcTradingNoticeInfoField));
	wxCommandEvent event(wxEVT_TRADING_NOTICE,wxID_ANY);
	event.SetInt(0);
	event.SetClientData((void*)pField);
	s_pMainFrame->AddPendingEvent(event);
	return 0;
}
int MainFrame::AlertMessageCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)
{
	if(data.BID!=BID_AlertMessage || data.Size!=sizeof(DataMessageStruct))
		return 0;	
	DataMessageStruct& refData = *(DataMessageStruct*)(const_cast<AbstractBusinessData*>(&data));	
	PlatformStru_MessageStruct* pField=new PlatformStru_MessageStruct();
	memcpy(pField,&(refData.MessageStructField),sizeof(PlatformStru_MessageStruct));
	wxCommandEvent event(wxEVT_TRADING_NOTICE,wxID_ANY);
	event.SetInt(1);
	event.SetClientData((void*)pField);
	s_pMainFrame->AddPendingEvent(event);
	return 0;

}
int MainFrame::RspQryNoticeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)
{
	if(data.BID!=BID_RspQryNotice || data.Size!=sizeof(DataRspQryNotice))
		return 0;	
	DataRspQryNotice& refData = *(DataRspQryNotice*)(const_cast<AbstractBusinessData*>(&data));	
	DataRspQryNotice* pField=new DataRspQryNotice();
	memcpy(pField,&refData,sizeof(DataRspQryNotice));
	wxCommandEvent event(wxEVT_USER_NOTICE,wxID_ANY);
	event.SetClientData((void*)pField);
	s_pMainFrame->AddPendingEvent(event);
	return 0;
}
int MainFrame::RspQrySettlementInfoConfirmCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)
{
	if(data.BID!=BID_RspQrySettlementInfoConfirm || data.Size!=sizeof(DataRspQrySettlementInfoConfirm))
		return 0;	
	DataRspQrySettlementInfoConfirm& refData = *(DataRspQrySettlementInfoConfirm*)(const_cast<AbstractBusinessData*>(&data));	
	wxCommandEvent event(wxEVT_RTN_RECONNECTED);
	s_pMainFrame->AddPendingEvent(event);
	return 0;
}
//����֪ͨ�¼���Ӧ
void MainFrame::OnTradingNotice(wxCommandEvent& evt)
{
	if((HWND)GetHWND()==NULL)return;
	if(evt.GetInt()==0)//����
	{
		CThostFtdcTradingNoticeInfoField* pField=(CThostFtdcTradingNoticeInfoField*)evt.GetClientData();
		std::string content=pField->FieldContent;
		if(content.empty()) return;
		if(m_pTradingNoticeDlg)
		{
			m_pTradingNoticeDlg->SetContent(content);
			m_pTradingNoticeDlg->CenterOnScreen();
			m_pTradingNoticeDlg->Show();
		}
		else
		{
			if(pField->SequenceSeries==0)
			{
				content=":\t"+content;
				content=pField->SendTime+content;
			}
			std::map<int,std::string>::iterator it=m_TradingNoticeArray.find(pField->SequenceNo);
			if(it==m_TradingNoticeArray.end())
				m_TradingNoticeArray.insert(std::pair<int,std::string>(pField->SequenceNo,content));
			else
			{
				it->second+=content;
				content=it->second;
			}
		}
		delete pField;
	}
	else//PATS Alert Message
	{
		PlatformStru_MessageStruct* pField=(PlatformStru_MessageStruct*)evt.GetClientData();
        CTradeInfoDlg* pDlg=TRADEINFODLG(this);
        if(pDlg)
		{
			wxString str=wxString::Format(LOADFORMATSTRING(ALERT_MESSAGE_TEXT,"%s%s%s"),//�ı��������%s  %s  %s
				pField->MsgID,
				pField->MsgText,
				pField->IsAlert=='N'?LOADSTRING(NO_TEXT):LOADSTRING(YES_TEXT));
			pDlg->ShowTradeMsg(str, -1, LOADSTRING(ALERT_MESSAGE), -1, TRUE);
			LOG_DATA* pData = new LOG_DATA;
			pData->strTitle = "ALERT_MESSAGE";
			pData->strFormat = "ALERT_MESSAGE_TEXT";
			pData->vParam.push_back(LOG_PARAM(pField->MsgID));
			pData->vParam.push_back(LOG_PARAM(pField->MsgText));
			pData->vParam.push_back(LOG_PARAM(pField->IsAlert=='N'? "NO_TEXT":"YES_TEXT", LOAD_STRING_TYPE));
			pDlg->WriteLog( pData );
			//pDlg->SetTitle(LOADSTRING(ALERT_MESSAGE));
			//pDlg->SetInfo(str);
			//pDlg->Show();
		}
		delete pField;
	}
}
void MainFrame::OnUserNotice(wxCommandEvent& event)
{
    DataRspQryNotice* pField=(DataRspQryNotice*)event.GetClientData();
    m_NoticeContent->SetField(pField->NoticeField);
    if(pField->bIsLast)
    {
		LoadHypertextNotice(m_NoticeContent,false);
    }
	delete pField;
}
void MainFrame::LoadHypertextNotice(NoticeContent* pContent,bool waitFinished)
{
	bool bLoad=GetHWND()&&pContent&&(!pContent->GetContent().IsEmpty());
	if(waitFinished)
		bLoad=bLoad&&pContent->GetFinished();
	if(bLoad)
	{
		PROCESS_INFORMATION pi;
		STARTUPINFO si;
		memset(&si,0,sizeof(si));
		si.cb=sizeof(si);
		si.wShowWindow=SW_SHOW;
		si.dwFlags=STARTF_USESHOWWINDOW;
		std::string strExe="HypertextNotice.exe";
		GlobalFunc::GetPath(strExe);
		wxString strcmd=wxString::Format("\"%s\" %d",strExe.c_str(),GetHWND());
		BOOL bRet=CreateProcess(NULL,(LPSTR)strcmd.c_str(),NULL,FALSE,NULL,NULL,NULL,NULL,&si,&pi);
	}
	else if(!waitFinished)
	{
		pContent->SetFinished();
	}
}
void MainFrame::OnBuySell5ShowChanged(wxCommandEvent& evt)
{
	wxWindow* pOrderPanel = GetModuleWindow(GID_ORDER_INSERT_PANEL);
	if ( NULL == pOrderPanel || NULL == m_notebook )
	{
		return;
	}

	int nMinWidth = evt.GetExtraLong();
	nMinWidth += 30;//Ԥ���ָ����Ŀ�ȼ��������Ŀ��
	bool bCheck = (evt.GetInt()==1);
	if ( m_notebook->IsFloatPaneOpen(pOrderPanel))
	{
		MyMiniFrame* pParent = wxDynamicCast(pOrderPanel->GetParent(), MyMiniFrame);
		if ( NULL != pParent )
		{
			pParent->AdjustSashPos( nMinWidth, bCheck );
			wxSize rSize = pParent->GetSize();
			pParent->SetSize(wxSize(nMinWidth, rSize.GetHeight()));
		}
	}
	else
	{
		wxAuiNotebook* pNoteBook = wxDynamicCast(pOrderPanel->GetParent(), wxAuiNotebook);
		if ( NULL == pNoteBook )
		{
			return;
		}

		wxSplitterWindow* pSplitter = wxDynamicCast(pNoteBook->GetParent(), wxSplitterWindow);
		if ( NULL == pSplitter )
		{
			return;
		}

		if ( !pSplitter->IsSplit())
		{
			return;
		}

		if ( pSplitter->GetSplitMode() == wxSPLIT_VERTICAL )
		{
			wxSize splitterSize = pSplitter->GetSize();
			int nSashPos = pSplitter->GetSashPosition();
			if ( bCheck )
			{
				if ( pNoteBook == pSplitter->GetWindow1())
				{
					if ( nSashPos < nMinWidth )
					{
						m_nDeltaOrderPanelSashPos = nMinWidth - nSashPos;
						pSplitter->SetSashPosition(nMinWidth);
					}
				}
				else
				{
					if ( splitterSize.GetWidth() - nSashPos < nMinWidth )
					{
						m_nDeltaOrderPanelSashPos = splitterSize.GetWidth() - nMinWidth - nSashPos;
						pSplitter->SetSashPosition( splitterSize.GetWidth() - nMinWidth );
					}
				}

				pNoteBook->SetMinSize(wxSize(nMinWidth, 200));
				pSplitter->SetMinSize(wxSize(nMinWidth, 200));
			}
			else
			{
				pNoteBook->SetMinSize(wxSize(200, 200));
				pSplitter->SetMinSize(wxSize(200, 200));

				nSashPos -= m_nDeltaOrderPanelSashPos;
				m_nDeltaOrderPanelSashPos = 0;
				pSplitter->SetSashPosition( nSashPos );
			}


		}
	}
}

//��������ʾ��ݼ�
void MainFrame::SetAliasNameShortCutKey(int id,std::string alias,std::string keycode)
{
    if(m_notebook==NULL) return;
	wxString caption;
	std::map<int, std::string> shortCut = CFG_MGR_DEFAULT()->GetShortCutKeyNameMap();
	std::map<int, std::string>::iterator it=shortCut.begin();
	for(it;it!=shortCut.end();++it)
	{
		if(it->second==keycode)
		{
			m_ID2AccelMap[id]=it->first;
			break;
		}
	}
	int i,pagecount=m_notebook->GetPageCount();
	for(i=0;i<pagecount;i++)
	{
		if(m_notebook->GetPageWindow(i)->GetId()==id)
		{
			caption=alias.c_str();
			if(m_MainFrameCfg->bShowPanelShortKey&&(!keycode.empty()))
			{
				caption+="(";
				caption+=keycode;
				caption+=")";
			}
			m_notebook->SetPageCaption(i,caption);
			break;
		}
	}
}
string  MainFrame::GetQuotCurInstrumentID()
{
    string strInstrumentID;
    GetCurInstrumentID_QuotPanel(strInstrumentID);
//	wxCommandEvent evt(wxEVT_QUOT_GET_INSTRUMENTID);
//	string strInstrumentID;
//	if(GetModuleWindow(GID_QUOT_TABLE))
//	{
//		GetModuleWindow(GID_QUOT_TABLE)->ProcessEvent(evt);
//		strInstrumentID=evt.GetString();
//	}
	return strInstrumentID;

}
void MainFrame::OnSendIEContent(wxCommandEvent& event)
{
	HWND pRemote=(HWND)event.GetInt();//���
	wxString strContent=m_NoticeContent->GetContent();
	if(pRemote&&::IsWindow(pRemote))
	{
		m_hypertextNoticeWnd=pRemote;
		COPYDATASTRUCT data;
		ZeroMemory( &data, sizeof(COPYDATASTRUCT)); 
		data.dwData=IE_MSG_CONTENT;//����
		data.cbData =strContent.Length();
		data.lpData =(void*)strContent.c_str();
		::SendMessage(m_hypertextNoticeWnd, WM_COPYDATA,IE_MAGIC_NUMBER, (LPARAM)&data);
	}
}
//����windows�����¼�WM_COPYDATA��WM_HOTKEY
WXLRESULT MainFrame::MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam) 
{
	if(message==WM_COPYDATA)
	{
		COPYDATASTRUCT* pRecvStruct=(COPYDATASTRUCT*)lParam;	
		if (pRecvStruct)
		{
			if(wParam==IE_MAGIC_NUMBER)
			{
				if(pRecvStruct->dwData==IE_MSG_LOGIN
					&&pRecvStruct->cbData==sizeof(HWND))
				{
					wxCommandEvent evt(wxEVT_SEND_IE_CONTENT);
					evt.SetInt(*(UINT*)pRecvStruct->lpData);//���
					AddPendingEvent(evt);
				}
				else if (pRecvStruct->dwData==IE_MSG_EXIT)
				{
					m_hypertextNoticeWnd=NULL;
				}
			}
			else
			{
				if(DEFAULT_SVR()&&DEFAULT_SVR()->SupportPlugins())
				{
					g_Plusin.ProcessMessage( (WPARAM)wParam, pRecvStruct );	
				}
			}
		}

	}
	else if(message==WM_HOTKEY) 
	{
		if(IsIconized())
		{
			wxTaskBarIconEvent evt(wxEVT_TASKBAR_LEFT_DOWN,m_MainTaskBarIcon);
			m_MainTaskBarIcon->ProcessEvent(evt);
		}
		else
			Iconize(true);
	}
	return FrameBase::MSWWindowProc(message,wParam,lParam); 
}
//����������̿����µ��������ݺ��µ���������
void MainFrame::AsyncHandyOrderMapAndKeyMultiplyVolumeMap()
{
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	// �����xml�ж����̿����µ���������
	m_KeyFuncMap.clear();
	for(int i=0; i<p->KeyOrderDataNum; i++) {
		std::string strKey(p->KeyOrderData[i].ShortKey);
		m_KeyFuncMap[strKey] = p->KeyOrderData[i];
	}
	//// �����xml�ж��µ���������
	//m_KeyMultiplyVolumeMap.clear();
	//for(int i=0; i<p->OrderMultiDataNum; i++) {
	//	std::string strKey(p->OrderMultiData[i].ShortKey);
	//	m_KeyMultiplyVolumeMap[strKey] = p->OrderMultiData[i].Volume;
	//}

}
//���˴�˵���Ӧ�����µ�¼
void MainFrame::OnLoginAgain(wxCommandEvent& WXUNUSED(event))
{
	//���FASTTRADER-742 
	CTPLogin::LoadSaveServerParam(false);
	//���FASTTRADER-1330
	CfgMgr* pMgr = CfgMgr::GetInstance();
	if ( pMgr ) pMgr->Save();
	DoLoginAgain();
}
//���˴�˵���Ӧ���Զ�����
void MainFrame::OnUpdateApp(wxCommandEvent& WXUNUSED(event))
{

}
//���˴�˵���Ӧ����ѡ��Լ
void MainFrame::OnConfigSelect(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::OnConfigSelect");
	SHOW_FLOAT_PANEL(J_ZiXuanHeYueDlg);

} 
//���˴�˵���Ӧ��ί�в���
void MainFrame::OnConfigOrder(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::OnConfigOrder");
	SHOW_FLOAT_PANEL(J_WeiTuoCanShuDlg);
}
//���˴�˵���Ӧ��ϵͳ����
void MainFrame::OnConfigSystem(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::OnConfigSystem");
	SHOW_FLOAT_PANEL(J_XiTongCanShuDlg);
}
//���˴�˵���Ӧ���޸�����
void MainFrame::OnConfigPassword(wxCommandEvent& WXUNUSED(event))
{
	SHOW_FLOAT_PANEL(CJ_ModifyPwdDlg);
}
//���˴�˵���Ӧ�����ֿ�ݼ�
void MainFrame::OnConfigAcce(wxCommandEvent& WXUNUSED(event))
{
	WXLOG_INFO("MainFrame::OnConfigAcce");
	SHOW_FLOAT_PANEL(J_ShuZiKuaiJieJianDlg);
}
//���˴�˵���Ӧ����ѡ��Լ
void MainFrame::OnUpdateUIConfigSelect(wxUpdateUIEvent& event)
{
	event.Check(m_curConfigPanelClassName=="J_ZiXuanHeYueDlg");
}
//���˴�˵���Ӧ��ί�в���
void MainFrame::OnUpdateUIConfigOrder(wxUpdateUIEvent& event)
{
	event.Check(m_curConfigPanelClassName=="J_WeiTuoCanShuDlg");
}
//���˴�˵���Ӧ��ϵͳ����
void MainFrame::OnUpdateUIConfigSystem(wxUpdateUIEvent& event)
{
	event.Check(m_curConfigPanelClassName=="J_XiTongCanShuDlg");
}
//���˴�˵���Ӧ���޸�����
void MainFrame::OnUpdateUIConfigPassword(wxUpdateUIEvent& event)
{
	event.Check(m_curConfigPanelClassName=="CJ_ModifyPwdDlg");
}
//���˴�˵���Ӧ�����ֿ�ݼ�
void MainFrame::OnUpdateUIConfigAcce(wxUpdateUIEvent& event)
{
	event.Check(m_curConfigPanelClassName=="J_ShuZiKuaiJieJianDlg");
}
//���˴�˵���Ӧ���߼�
void MainFrame::OnConfigGaoji(wxCommandEvent& WXUNUSED(event))
{
	//GetBtArray()[1]->SetCheck(false);
	//wxString str = wxT("��ʿ��");
	//LoadConfigApp(str);
}
//�����µ���
void MainFrame::PopupOrderInsertPanel()
{
	//Freeze();
	//FloatPaneClose(m_poOrderInsertPanel); 
	//Thaw();
	FloatPaneOpen();
	m_bMaxBak_MainFrame = IsMaximized();
	m_posBak_MainFrame = GetPosition();
	//m_poOrderInsertPanel->ShowViewSubStyle(style);
	wxWindow* pWinOrder = FindWindowById(GID_ORDER_INSERT_PANEL, this);
	wxWindow* pParentWin = pWinOrder->GetParent();
	pParentWin->SetWindowStyle((pParentWin->GetWindowStyle() | wxPOPUP_WINDOW) & (~wxRESIZE_BORDER));
	SetMainHookForDialog(pWinOrder, TRUE);
	//m_poOrderInsertPanel->BringWindow2TopAlways();
	//SetMainHookForOrderInsertPanel(true);
	pWinOrder->Refresh();
}
//���˴�˵���Ӧ��С��ί��
void MainFrame::OnOrderSmallWin(wxCommandEvent& WXUNUSED(event))
{
	PopupOrderInsertPanel();
	if(IsMaximized())
		Restore();
	Move(wxPoint(-20000, -20000));
}
//���˴�˵���Ӧ����׼�µ���
void MainFrame::OnOrderStandard(wxCommandEvent& WXUNUSED(event))
{
    if(m_notebook==NULL) return;
	//PopupOrderInsertPanel(wxRect(GetSize().x-240,24,200,335),0);
	//	m_poOrderInsertPanel->ShowViewStyle(OrderInsertPanel::sm_nViewStyle, 0);
	wxCommandEvent sendevt(wxEVT_ORDERINSERT_VIEWMODE_CHANGED);
	sendevt.SetInt(m_notebook->GetLayoutStyle()*256+0);
	wxWindow* pWinOrder = FindWindowById(GID_ORDER_INSERT_PANEL, this);
	if(pWinOrder)
		pWinOrder->ProcessEvent(sendevt);
}
//���˴�˵���Ӧ�������µ�
void MainFrame::OnOrderBatch(wxCommandEvent& WXUNUSED(event))
{
    if(m_notebook==NULL) return;
	//PopupOrderInsertPanel(wxRect(GetSize().x-360,24,335,335),2);
	//	m_poOrderInsertPanel->ShowViewStyle(OrderInsertPanel::sm_nViewStyle, 1);
	wxCommandEvent sendevt(wxEVT_ORDERINSERT_VIEWMODE_CHANGED);
	sendevt.SetInt(m_notebook->GetLayoutStyle()*256+1);
	wxWindow* pWinOrder = FindWindowById(GID_ORDER_INSERT_PANEL, this);
	if(pWinOrder)
		pWinOrder->ProcessEvent(sendevt);
}
//���˴�˵���Ӧ��������
void MainFrame::OnOrderCondition(wxCommandEvent& WXUNUSED(event))
{
    if(m_notebook==NULL) return;
	//PopupOrderInsertPanel(wxRect(GetSize().x-360,24,335,335),3);
	//	m_poOrderInsertPanel->ShowViewStyle(OrderInsertPanel::sm_nViewStyle, 2);
	wxCommandEvent sendevt(wxEVT_ORDERINSERT_VIEWMODE_CHANGED);
	sendevt.SetInt(m_notebook->GetLayoutStyle()*256+2);
	wxWindow* pWinOrder = FindWindowById(GID_ORDER_INSERT_PANEL, this);
	if(pWinOrder)
		pWinOrder->ProcessEvent(sendevt);
}
//���˴�˵���Ӧ��ӯ��
void MainFrame::OnOrderProfit(wxCommandEvent& WXUNUSED(event))
{ 
    if(m_notebook==NULL) return;
	//PopupOrderInsertPanel(wxRect(GetSize().x-360-196,24,335+196,335),4);
	//	m_poOrderInsertPanel->ShowViewStyle(OrderInsertPanel::sm_nViewStyle, 3);
	wxCommandEvent sendevt(wxEVT_ORDERINSERT_VIEWMODE_CHANGED);
	sendevt.SetInt(m_notebook->GetLayoutStyle()*256+3);
	wxWindow* pWinOrder = FindWindowById(GID_ORDER_INSERT_PANEL, this);
	if(pWinOrder)
		pWinOrder->ProcessEvent(sendevt);
}
//���̲˵���Ӧ��Stop or Stop Limit Order
void MainFrame::OnOrderStopLimit(wxCommandEvent& WXUNUSED(event))
{
    if(m_notebook==NULL) return;
	wxCommandEvent sendevt(wxEVT_ORDERINSERT_VIEWMODE_CHANGED);
	sendevt.SetInt(m_notebook->GetLayoutStyle()*256+4);
	wxWindow* pWinOrder = FindWindowById(GID_ORDER_INSERT_PANEL, this);
	if(pWinOrder)
		pWinOrder->ProcessEvent(sendevt);
}
//���˴�˵���Ӧ����ѯ�����
void MainFrame::OnQueryWithdraw(wxCommandEvent& WXUNUSED(event))
{
	SHOW_FLOAT_PANEL(CJ_ChuRuJinDlg);
}
//���˴�˵���Ӧ����ѯ���㵥
void MainFrame::OnQuerySettlement(wxCommandEvent& WXUNUSED(event))
{
	SHOW_FLOAT_PANEL(CJ_JieSuanDlg);
}

//���˴�˵���Ӧ������ת��
void MainFrame::OnTransferBank(wxCommandEvent& WXUNUSED(event))
{
	SHOW_FLOAT_PANEL(CBankTransferDlg);
}
//���˴��Ҽ��˵���Ӧ������ί�е�ģ��
void MainFrame::CancerlOrder(wxCommandEvent& WXUNUSED(event))
{
	wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, CAllOrdersPanel::ID_BUTTON_REMOVE);	
	FindWindowById(GID_ALL_ORDER)->AddPendingEvent(evt);
}
//���˴��Ҽ��˵���Ӧ����������ģ��
void MainFrame::ConditionPanelButton(wxCommandEvent& event)
{
	wxWindow* pList=FindWindowById(GID_QUERY_ORDER_CONDITION);
	CQueryOrderConditionPanel* pPanel=static_cast<CQueryOrderConditionPanel*>(pList->GetParent());
	if(pPanel==NULL) return;
	wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED);
	switch(event.GetId())
	{
	case ID_MENU_CONDITION_DELETE: 
		evt.SetId(CQueryOrderConditionPanel::ID_QUERYCONDITION_BUTTONDELETE);
		break;   
	case ID_MENU_CONDITION_PAUSE :  
		evt.SetId(CQueryOrderConditionPanel::ID_QUERYCONDITION_BUTTONPAUSE); 
		break;   
	case ID_MENU_CONDITION_ACTIVE:  
		evt.SetId(CQueryOrderConditionPanel::ID_QUERYCONDITION_BUTTONACTIVE); 
		break;   
	case ID_MENU_CONDITION_MODIFY:  
		evt.SetId(CQueryOrderConditionPanel::ID_QUERYCONDITION_BUTTONEDIT);
		break; 
	}
	pPanel->AddPendingEvent(evt);
}
//���˴�˵���Ӧ��ԤԼ����
void MainFrame::OnTransferWithdraw(wxCommandEvent& WXUNUSED(event))
{

}
//���˴�˵���Ӧ�������ĵ�
void MainFrame::OnHelpDoc(wxCommandEvent& WXUNUSED(event))
{
	std::string apppath;
	GlobalFunc::GetPath(apppath);
	apppath+="help.chm";
	if(wxFileExists(apppath))
		ShellExecute(NULL,"open",apppath.c_str(),NULL,NULL,SW_SHOWNORMAL);
	else
		wxMessageBox(LOADSTRING(GUI_INSTRUCTION_FOR_USE), LOADSTRING(USERLOGINDLG_INFORMATION));
}
//���˴�˵���Ӧ������־
void MainFrame::OnHelpLog(wxCommandEvent& WXUNUSED(event))
{
	std::string appexe,apppath,filename;
	GlobalFunc::GetPath(apppath);
	wxGetApp().GetFileName(filename,false);
	appexe=apppath+"log_";
	appexe+=filename;
	if(wxDirExists(appexe.c_str()))
		ShellExecute(NULL,"open",appexe.c_str(),NULL,NULL,SW_SHOWNORMAL);
	else
		ShellExecute(NULL,"open",apppath.c_str(),NULL,NULL,SW_SHOWNORMAL);
}

//�µ����Լ�ı���Ϣ
void MainFrame::OnInstrumentIdCHange(wxCommandEvent& event)
{
    if(DEFAULT_SVR()&&DEFAULT_SVR()->SupportPlugins())
	{
		string strName;
		if( CEventParam::GetEventParam(event.GetInt(),NULL,&strName,NULL,NULL,0,NULL))
		{
			g_Plusin.SetOrderModuleCurInstrumentID(strName);
		}
	}
	wxWindow* pWinQuot = FindWindowById(GID_QUOT_TABLE, this);
	if(pWinQuot&&event.GetEventObject()!=pWinQuot)
		pWinQuot->ProcessEvent(event);
}
//�����µ�����Ϣ
void MainFrame::OnPane2OrderPane(wxCommandEvent& event)
{
	wxWindow* pWinOrder = FindWindowById(GID_ORDER_INSERT_PANEL, this);
	if(pWinOrder&&event.GetEventObject()!=pWinOrder)
		pWinOrder->ProcessEvent(event);
}
//�������۱���Ϣ
void MainFrame::OnPane2QuotPane(wxCommandEvent& event)
{
	wxWindow* poQuotPanel=GetModuleWindow(GID_QUOT_TABLE);
	if(poQuotPanel&&event.GetEventObject()!=poQuotPanel)
		poQuotPanel->ProcessEvent(event);
}
//����ģ��ת�˳ɹ�
void MainFrame::OnSucessTransfer(wxCommandEvent& event)
{
	PlatformStru_QryTradingAccount QryTradingAccount;
	memset(&QryTradingAccount, 0, sizeof(QryTradingAccount));
	if(DEFAULT_SVR()) DEFAULT_SVR()->ReqQryTradingAccount(QryTradingAccount);
}

void MainFrame::OnOrderPanel2BatchQuery(wxCommandEvent& event)
{
	wxWindow* dstWin=NULL;
	dstWin=FindWindowById(GID_QUERY_ORDER_BATCH,this);
	if(dstWin&&event.GetEventObject()!=dstWin)
		dstWin->ProcessEvent(event);
}
void MainFrame::OnOrderPanel2ParkedQuery(wxCommandEvent& event)
{
	wxWindow* dstWin=NULL;
	dstWin=FindWindowById(GID_QUERY_ORDER_PARKED,this);
	if(dstWin&&event.GetEventObject()!=dstWin)
		dstWin->ProcessEvent(event);
}
void MainFrame::OnOrderPanel2ConditionQuery(wxCommandEvent& event)
{
	wxWindow* dstWin=NULL;
	dstWin=FindWindowById(GID_QUERY_ORDER_CONDITION,this);
	if(dstWin&&event.GetEventObject()!=dstWin)
		dstWin->ProcessEvent(event);
}
void MainFrame::OnOrderPanel2StopQuery(wxCommandEvent& event)
{
	wxWindow* dstWin=NULL;
	dstWin=FindWindowById(GID_QUERY_ORDER_POSITION,this);
	if(dstWin&&event.GetEventObject()!=dstWin)
		dstWin->ProcessEvent(event);
}

//��ʼ����̨��ѯ������Ϣ��Ӧ
//void MainFrame::OnInitQryStepMain(wxCommandEvent& event)
//{
//    std::string EvtParamStr;
//    int EvtInt;
//    if(1)
//    {
//        DWORD EvtParamID=(DWORD)event.GetInt();
//        if(CEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,&EvtInt))
//        {
//            CEventParam::DeleteEventParam(EvtParamID);
//        }
//        else
//        {
//            return;
//        }
//    }
//
//    if(!m_notebook) return;
//	wxString strError(EvtParamStr);
//	if(strError.IsEmpty())
//	{
//		int i,step=EvtInt,pagecount=m_notebook->GetPageCount();
//		if(DEFAULT_SVR()) DEFAULT_SVR()->_SetInitQryStep((step==INITQRY_QryEnd)?(step+1):step);
//		wxCommandEvent myEvent(wxEVT_INITQRY,wxID_ANY);
//		myEvent.SetInt(step);
//		for(i=0;i<pagecount;i++)
//		{
//			m_notebook->GetPageWindow(i)->ProcessEvent(myEvent);     
//		}
//	}
//	else
//		wxMessageBox(strError,LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
//
//	if ( EvtInt == INITQRY_MainInstrument)
//	{
//		std::string strLogonReason="";
//        if(DEFAULT_SVR())
//            strLogonReason = DEFAULT_SVR()->GetLogonReason();
//		if ( !strLogonReason.empty())
//		{
//			wxMessageBox( strLogonReason, LOADSTRING(USERLOGINDLG_INFORMATION) );
//		}
//	}
//}
void MainFrame::OnWriteUserLog(wxCommandEvent& event)
{
	LOG_DATA* pData = (LOG_DATA*)(event.GetClientData());
	if ( NULL == pData )
	{
		return;
	}

	LogDlg* pDlg = LogDlg::GetInstance();
	if ( NULL == pDlg )
	{
		USER_LOG userLog;
		userLog.strLogType = "LOG_TYPE_ORDER";
		userLog.pData = pData;
		m_vLogData.push_back(userLog);
	}
	else
	{
		pDlg->WriteLog( "LOG_TYPE_ORDER", pData);
	}
}
void MainFrame::OnGetStyle(wxCommandEvent& event)
{
    if(m_notebook==NULL) return;
	event.SetInt(m_notebook->GetLayoutStyle());
    event.SetClientData(new wxString(m_notebook->GetLayoutCaption()));
}
void MainFrame::OnCfgChanged( wxCommandEvent& event )
{

    std::string EvtParamStr;
    if(1)
    {
        DWORD EvtParamID=(DWORD)event.GetInt();
        if(CEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        {
            CEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }

    wxString strMsg(EvtParamStr.c_str());
//    wxString strMsg = event.GetString();
//	if ( strMsg.IsEmpty())
//	{
//		return;
//	}

	vector<MSG_TYPE> vecMsgType;
	MSG_TYPE eMsgType = INVALID_MSG;
	int nPos = strMsg.Find( STRING_MSG_SEPERATOR );
	while( nPos != -1 )
	{
		wxString strTemp = strMsg.Left( nPos );
		eMsgType = static_cast<MSG_TYPE>(atoi( strTemp.GetData()));
		if ( INVALID_MSG != eMsgType)
		{
			vecMsgType.push_back(eMsgType);
		}
		strMsg = strMsg.Right( strMsg.Length() - nPos - 1);
		nPos = strMsg.Find( STRING_MSG_SEPERATOR );
	}

	eMsgType = static_cast<MSG_TYPE>(atoi( strMsg.GetData()));
	if ( INVALID_MSG != eMsgType)
	{
		vecMsgType.push_back(eMsgType);
	}

//////////////////////////////////////////////////////////////////////////////////
#define CASE_MODULE(msg,gid)\
		case msg:\
			{\
				wxWindow* pWnd = GetModuleWindow(gid);\
				if ( NULL != pWnd )\
				{\
					setPanel.insert( pWnd );\
				}\
			}\
			break
///////////////////////////////////////////////////////////////////////////////
	set<wxWindow*> setPanel;
	for ( unsigned int i = 0; i < vecMsgType.size(); i++)
	{
		MSG_TYPE eMsgType = vecMsgType.at(i);
		switch( eMsgType )
		{
		case MSG_VARIETYINFO_CHANGE:
			break;
		case MSG_DISPLAY_CHANGE:
			CustomDefineUI();
			break;
		CASE_MODULE(MSG_INSTRUMENT_GROUP_CHANGE,GID_QUOT_TABLE);
		CASE_MODULE(MSG_ORDER_CHANGE,GID_ORDER_INSERT_PANEL);
		CASE_MODULE(MSG_CANCEL_ORDER_CHANGE,GID_OPEN_ORDER);
		CASE_MODULE(MSG_LIST_BAOJIABIAO_CHANGE,GID_QUOT_TABLE);
		CASE_MODULE(MSG_LIST_TRADE_RECORD_CHANGE,GID_THRADE_RECORD);
		CASE_MODULE(MSG_LIST_TRADE_STATIC_CHANGE,GID_THRADE_RECORD);
		CASE_MODULE(MSG_LIST_POSITION_CHANGE,GID_POSITIONALL);
		CASE_MODULE(MSG_LIST_COMBO_POSITION_CHANGE,GID_POSITIONALL);
		CASE_MODULE(MSG_LIST_MULTIACCOUNT_CHANGE,GID_MULTI_ACCOUNT);
		CASE_MODULE(MSG_LIST_MULTIPOSITION_CHANGE,GID_MULTI_POSITION);
		CASE_MODULE(MSG_LIST_POSITION_DETAIL_CHANGE,GID_POSITIONALL);
		CASE_MODULE(MSG_LIST_QUERY_ORDER_POSITION_CHANGE,GID_QUERY_ORDER_POSITION);
		CASE_MODULE(MSG_LIST_ORDER_INSERT_CHANGE,GID_ORDER_INSERT_PANEL);
		CASE_MODULE(MSG_LIST_QUERY_ORDER_BATCH_CHANGE,GID_QUERY_ORDER_BATCH);
		CASE_MODULE(MSG_LIST_QUERY_ORDER_CONDITION_CHANGE,GID_QUERY_ORDER_CONDITION);
		CASE_MODULE(MSG_LIST_QUERY_ORDER_PARKED_CHANGE,GID_QUERY_ORDER_PARKED);
		CASE_MODULE(MSG_LIST_ACCOUNT_CHANGE,GID_ACCOUNT);
		CASE_MODULE(MSG_LIST_ALL_ORDER_CHANGE,GID_ALL_ORDER);
		CASE_MODULE(MSG_LIST_OPEN_ORDER_CHANGE,GID_OPEN_ORDER);
		CASE_MODULE(MSG_LIST_INSTRUMENT_CHANGE,GID_INSTRUMENT);
		CASE_MODULE(MSG_USER_GROUP_CHANGE,GID_MULTI_ACCOUNT);
		default:
			break;
		}

		if(DEFAULT_SVR()&&DEFAULT_SVR()->SupportPlugins())
		{
			if ( eMsgType == MSG_ORDER_CHANGE)
			{
				COPYDATASTRUCT copyData;
				memset(&copyData, 0 ,sizeof(copyData));
				copyData.dwData = MAKELONG(CMDID_CONFIG_CHANGE, 0);
				g_Plusin.Send(copyData);
			}
		}
	}

	wxCommandEvent myEvent(wxEVT_CFG_CHANGE);
	set<wxWindow*>::iterator it = setPanel.begin();
	for ( ; it != setPanel.end(); ++it)
	{
		if ( NULL != *it)
		{
			(*it)->AddPendingEvent(myEvent);
			(*it)->Refresh();
		}
	}

	AsyncHandyOrderMapAndKeyMultiplyVolumeMap();

	UpdateWindow((HWND)m_hWnd);
}

void MainFrame::OnUpdateListTable( wxCommandEvent& event )
{
	wxCommandEvent myEvent(wxEVT_CFG_CHANGE);
	int nMsgType = event.GetInt();
	if ( nMsgType == MSG_LIST_BAOJIABIAO_CHANGE )
	{
		wxWindow* pWnd = GetModuleWindow(GID_QUOT_TABLE);
		if ( NULL != pWnd )
		{
			pWnd->ProcessEvent(myEvent);
			pWnd->Refresh();
		}
	}
    else if ( nMsgType == MSG_ALL_LIST_CHANGE )
    {
        if(m_notebook)
        {
            for ( unsigned int i = 0; i < m_notebook->GetPageCount(); ++i)
            {
                wxWindow* pWnd =m_notebook->GetPageWindow(i);
                if ( NULL != pWnd )
                {
                    pWnd->ProcessEvent(myEvent);
                    pWnd->Refresh();
                }
            }
        }
    }
	else
	{

	}
}
//�˵���Ӧ���Ʋ�
void MainFrame::OnPositionMove(wxCommandEvent& event)
{
	wxWindow* pWin=FindWindowById(GID_POSITIONALL,this);
	if(pWin)
	{
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED,ID_BUTTON_MOVEPOSITION);
		pWin->AddPendingEvent(evt);
	}
}
//�˵���Ӧ������
void MainFrame::OnPositionSwap(wxCommandEvent& event)
{
	wxWindow* pWin=FindWindowById(GID_POSITIONALL,this);
	if(pWin)
	{
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED,ID_BUTTON_SWAPPOSITION);
		pWin->AddPendingEvent(evt);
	}
}

//�˵���Ӧ���������
void MainFrame::OnShowPlusInDlg(wxCommandEvent& event )
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->SupportPlugins())
	{
		g_Plusin.ShowPlusinDlg(this);
	}
}

//���²˵��򹤾���
void MainFrame::OnUpdateUI_PosMove(wxUpdateUIEvent& event)
{
	int bEnable=0;
	wxWindow* pWin=FindWindowById(GID_POSITIONALL,this);
	if(pWin)
	{
		wxCommandEvent evt(wxEVT_POS_MOVE_SWAP_UPDATE,ID_BUTTON_MOVEPOSITION);
		pWin->ProcessEvent(evt);
		bEnable=evt.GetInt();
	}
	event.Enable(bEnable!=0);	
}
//���²˵��򹤾���
void MainFrame::OnUpdateUI_PosSwap(wxUpdateUIEvent& event)
{
	int bEnable=0;
	wxWindow* pWin=FindWindowById(GID_POSITIONALL,this);
	if(pWin)
	{
		wxCommandEvent evt(wxEVT_POS_MOVE_SWAP_UPDATE,ID_BUTTON_SWAPPOSITION);
		pWin->ProcessEvent(evt);
		bEnable=evt.GetInt();
	}
	event.Enable(bEnable!=0);	
}
#ifdef _USE_MULTI_LANGUAGE
void MainFrame::ChangeLanguage(int langid)
{
	FrameBase::ChangeLanguage(langid);
    ChangeFrameLanguage(langid);
	int i,pagecount=m_notebook->GetPageCount();
	wxCommandEvent myEvent(wxEVT_LANGUAGE_CHANGE);
	myEvent.SetInt(langid);
	for(i=0;i<pagecount;i++)
	{
		m_notebook->GetPageWindow(i)->ProcessEvent(myEvent);     
	}
	if(m_curConfigPanel)m_curConfigPanel->ProcessEvent(myEvent); 

	//״̬�����Ըı�
	GetStatusBar()->ProcessEvent(myEvent);
	LogDlg::GetInstance()->ProcessEvent(myEvent);
	if(TRADEINFODLG(this))
		TRADEINFODLG(this)->ResetLanguage();

}
void MainFrame::ChangeFrameLanguage(int langid)
{
	CfgMgr* pMgr = CfgMgr::GetInstance();
	string strOldLabel,strNewLabel;
	//���ñ���
	SetTitle(GetMainFrameCaption());
	//��������˵�
	if(m_notebook->GetLayoutStyle()==STYLE_SunGard
		&& DEFAULT_SVR()&& DEFAULT_SVR()->IsSunGardSupported())
		CreateMenuBar_SunGard();
	else
		CreateMenuBar_Q7(); 
	//����ģ������

    wxString strCaption,strTemp;
	int i,pagecount=m_notebook->GetPageCount();
	for(i=0;i<pagecount;i++)
	{
		strTemp=m_notebook->GetPageCaption(i);
		strTemp=strTemp.AfterFirst('(');
		if(!strTemp.IsEmpty()) strTemp='('+strTemp;
		strCaption=GetModuleCaption(m_notebook->GetPageWindow(i)->GetId());
		m_notebook->SetPageCaption(i,strCaption+strTemp);
	}
	m_OptionCaption=LOADSTRING(ID_OPTION_CAPTION);
}
#endif
#ifdef _USE_TOP_SPLITTER
void MainFrame::OnToggleViewVideoInfo(bool bCheck)
{

	wxSplitterWindow* pTopSplit=(wxSplitterWindow*)m_right_splitter_wnd->GetParent();
	if(pTopSplit->IsSplit())
	{
		//ȥ���ұ���Ŀ
		m_right_splitter_wnd->Hide();
		((MultiSplitter*)m_right_splitter_wnd)->StopTimer();
		pTopSplit->Initialize(m_notebook);
		pTopSplit->SizeWindows();
	}
	else//����ұ���Ŀ
	{
		m_right_splitter_wnd->Show();
		((MultiSplitter*)m_right_splitter_wnd)->StartTimer();
	 	pTopSplit->SplitVertically(m_notebook,m_right_splitter_wnd,-350);
	}
	FrameBase::OnToggleViewVideoInfo(bCheck);
}
#endif

wxWindow* MainFrame::CreatePlugPane(int nPos,wxString caption,wxString AppID)
{
	if( s_pMainFrame == NULL || s_pMainFrame->m_notebook == NULL )
	{
		return NULL;
	}

    return s_pMainFrame->m_notebook->NewBlankPage(nPos,caption,AppID);
}

bool MainFrame::DeletePlugPane(int npos, wxString AppID)
{
	if( s_pMainFrame == NULL || s_pMainFrame->m_notebook == NULL )
	{
		return false;
	}

    return s_pMainFrame->m_notebook->DeleteBlankPage(npos,AppID);
}

void MainFrame::OnTimer( wxTimerEvent& event )
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->SupportPlugins())
	{
		g_Plusin.ReleasePlusInRC();
	}
	if(s_bNeedHide&&m_MainTaskBarIcon!=NULL) {
		DWORD dwCurrTime = ::GetTickCount();
		if(IsIconized()&&dwCurrTime-s_dwNeedTime>=500&&dwCurrTime-s_dwNeedTime<=1500) {
			s_bNeedHide = FALSE;
			Hide(); 
			m_MainTaskBarIcon->SetIcon(wxIcon(_T("zqICON_MAIN")));
		}
	}
}

void MainFrame::OnLoadPlusIn(wxCommandEvent& event)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->SupportPlugins())
	{
		g_Plusin.LoadPlusin();
	}
}

void MainFrame::RestorePosition()
{
	CfgMgr* pMgr = CfgMgr::GetInstance();
	if ( NULL != pMgr )
	{
		if ( IsIconized() )
		{
			pMgr->SetStatus(2);
			WINDOWPLACEMENT placement;
			GetWindowPlacement( (HWND)m_hWnd, &placement);
			RECT rc = placement.rcNormalPosition;
			pMgr->SetPos(rc.left, rc.top);
			pMgr->SetSize(rc.right-rc.left, rc.bottom-rc.top);
		}
		else if ( IsMaximized() )
		{
			pMgr->SetStatus(1);
			WINDOWPLACEMENT placement;
			GetWindowPlacement( (HWND)m_hWnd, &placement);
			RECT rc = placement.rcNormalPosition;
			pMgr->SetPos(rc.left, rc.top);
			pMgr->SetSize(rc.right-rc.left, rc.bottom-rc.top);
		}
		else
		{
			wxPoint pt = GetPosition();
			pMgr->SetPos(pt.x, pt.y);

			wxSize size = GetSize();
			if ( size.GetWidth() > 0 && size.GetHeight() > 0)
			{
				pMgr->SetSize(size.GetWidth(), size.GetHeight());
			}

			pMgr->SetStatus(0);
		}
	}
}


bool MainFrame::GetCurInstrumentGrpInfo_QuotPanel(std::string& GrpName,int& GrpID)
{
	wxWindow* poQuotPanel=GetModuleWindow(GID_QUOT_TABLE);
	if(poQuotPanel)
		return ((CQuotFrame*)poQuotPanel)->GetCurInstrumentGrpInfo(GrpName,GrpID);
    else return false;
}


void MainFrame::GetCurInstrumentID_QuotPanel(std::string& strInstrument)
{
	wxWindow* poQuotPanel=GetModuleWindow(GID_QUOT_TABLE);
	if(poQuotPanel)
		((CQuotFrame*)poQuotPanel)->GetCurInstrumentID(strInstrument);
    else strInstrument.clear();
}

//���Ҫ���½���ɾ����壬type==1�½���2ɾ��
wxWindow* MainFrame::Plugin_New_Del_Panel_QuotFrame(int type)
{
	wxWindow* poQuotPanel=GetModuleWindow(GID_QUOT_TABLE);
	if(poQuotPanel)
		return ((CQuotFrame*)poQuotPanel)->Plugin_New_Del_Panel(type);
    else return NULL;
}

//��ȡ��������ʾ����
string MainFrame::GetPanelCaption(int PanelID,int type)
{
    if(m_notebook==NULL) return string("");
	int i,pagecount=m_notebook->GetPageCount();
	wxString caption;
	for(i=0;i<pagecount;i++)
	{
		if(m_notebook->GetPageWindow(i)->GetId()==PanelID)
		{
			if(type==1)
				caption=GetModuleCaption(PanelID);
			else
			{
				caption=m_notebook->GetPageCaption(i);
				caption=caption.BeforeFirst('(');
			}
            return string(caption.c_str());
		}
	}
    return string("");
}
string MainFrame::GetCifcoServicesHallLocation()
{
	const char strSubKeyRegPath[]="Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\CifcoServicesHall";
	char InstallLocation[MAX_PATH]="C:\\Program Files\\CifcoServicesHall";
	//��ȡע���
	HKEY key;
	DWORD dwType=REG_SZ,dwSize=MAX_PATH;
	if(ERROR_SUCCESS==RegOpenKey(HKEY_LOCAL_MACHINE,strSubKeyRegPath,&key))
	{
		RegQueryValueEx(key,"InstallLocation",0,&dwType,(LPBYTE)InstallLocation,&dwSize);
		RegCloseKey(key); 
	}
	if(InstallLocation[0]&&InstallLocation[strlen(InstallLocation)-1]!='\\')
		strcat(InstallLocation,"\\");
	return InstallLocation;


}
//�������ڿͻ�����ƽ̨
void MainFrame::LoadCifcoServicesHallApp()
{
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT(); 
	if(pCfgMgr&&pCfgMgr->GetSystemParamInfo()&&pCfgMgr->GetSystemParamInfo()->bWebCifco)
	{
		string InstallLocation=GetCifcoServicesHallLocation();
		char XML_BEGIN[]="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<body>\n\t<userName>";
		char XML_END[]="</userName>\n</body>";
		std::string UserID,BrokerID;
		//��ȡUserID
		if(DEFAULT_SVR()) DEFAULT_SVR()->GetBrokerIDUserID(UserID,BrokerID);
		//����xml����
		std::string content=XML_BEGIN;
		content+=UserID;
		content+=XML_END;
		//��ȡxml·��
		std::string strxmlpath=InstallLocation+"CifcoServicesHallNoLogin\\";
		if(!wxDirExists(strxmlpath.c_str())) return;
		strxmlpath+="userinfo.xml";
		//д���ļ�
		HANDLE hHandle=CreateFile(strxmlpath.c_str(),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
		if(hHandle!=INVALID_HANDLE_VALUE)
		{
			DWORD retCount;
			WriteFile(hHandle,content.c_str(),content.length(),&retCount,NULL);
			CloseHandle(hHandle);
		}
		//����CifcoServicesHall.exe
		std::string strpath=InstallLocation+"CifcoServicesHallNoLogin\\CifcoServicesHallNoLogin.exe";
		if(wxFileExists(strpath.c_str()))
		{
			cProcess *process = new cProcess(this,m_CifcoServicesHallProcessId);
			process->Execute(strpath.c_str(), wxEXEC_ASYNC);
		}
	}
}
void MainFrame::OnRtnReconnected(wxCommandEvent& event)
{
	static bool bMsgBoxShow=false;
	if(m_notebook==NULL||bMsgBoxShow) return;
	int i,itemindex=0,itemcount=0,pagecount=m_notebook->GetPageCount();
	wxCommandEvent myEvent(wxEVT_REQ_REQRY);
	myEvent.SetInt(1);//��ȡ��Ҫˢ�µ�����
	for(i=0;i<pagecount;i++)
	{
		myEvent.SetExtraLong(0);
		wxWindow* win = m_notebook->GetPageWindow(i);
		if ( NULL != win )
		{
			win->ProcessEvent(myEvent);
		}
		long exlong=myEvent.GetExtraLong();
		if(exlong>0)itemindex++;
		itemcount+=exlong;
	}
	if(itemindex>0&&itemcount>GlobalConfigManager::GetMaxItemCountForAutoResort_ListCtrl()*itemindex)
	{
		bMsgBoxShow=true;
		int msgret=wxMessageBox("�ؽӽ��׷������ɹ����й����ݿ����ѹ��ڣ��Ƿ����²�ѯ��",LOADSTRING(USERLOGINDLG_INFORMATION),wxYES_NO);
		bMsgBoxShow=false;		
		if(msgret==wxYES)
		{
			myEvent.SetInt(0);//ˢ��
			for(i=0;i<pagecount;i++)
			{
				wxWindow* win = m_notebook->GetPageWindow(i);
				if ( NULL != win )
				{
					win->AddPendingEvent(myEvent);
				}   
			}
		}
	}
	else
	{
		myEvent.SetInt(0);//ˢ��
		for(i=0;i<pagecount;i++)
		{
			wxWindow* win = m_notebook->GetPageWindow(i);
			if ( NULL != win )
			{
				win->AddPendingEvent(myEvent);
			}   
		}
	}
}
void MainFrame::OnPluginMenuChange(wxCommandEvent& event)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->SupportPlugins())
	{
		wxMenuBar* pMenuBar=GetMenuBar();
		wxString strMenu=LOADSTRING(ID_PLUSIN_CAPTION);
		for (size_t i=0;i<pMenuBar->GetMenuCount();++i)
		{
			if(strMenu==pMenuBar->GetMenuLabel(i))
			{
				CreateMenuBar_Plugins(pMenuBar,i);
				break;
			}
		}
	}
}
void MainFrame::OnClickPluginMenu(wxCommandEvent& evt)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->SupportPlugins())
	{
		wxMenuBar* pMenuBar=GetMenuBar();
		wxMenuItem* pItem=pMenuBar->FindItem(evt.GetId());
		if(pItem)
		{
			g_Plusin.ShowPlusin(pItem->GetHelp());
		}
	}
}

void MainFrame::OnDulplicateKeyPrompt( wxCommandEvent& event )
{
	CfgMgr* pMgr = CFG_MGR_DEFAULT();
	if ( NULL == pMgr )
	{
		return;
	}

	std::string strPromptMsg = pMgr->GetDulplicateKeyString();
	if ( strPromptMsg.empty() )
	{
		return;
	}

	std::string strTemp = LOADSTRING(DUPLICATE_SHORTCUTKEY_PROMPT);
	strTemp += strPromptMsg;
	wxMessageBox( strTemp );
}

void MainFrame::OnMASocket( wxCommandEvent& event )
{
	RecvData* pRecvData=(RecvData*)event.GetClientData();
	if(pRecvData==NULL)return;

	switch(pRecvData->head.cmdid)
	{
	case Cmd_MA_SendUnReadMessage_Rsp:
		{
			int nMsgCount = pRecvData->head.len / sizeof(MessageInfo);
			for ( int i = 0; i < nMsgCount; i++ )
			{
				MessageInfo* pMsgInfo = (MessageInfo*)((char*)pRecvData->pData + sizeof(MessageInfo)*i);
				m_mapMsgInfo[pMsgInfo->nMessageID] = *pMsgInfo;
			
				wxCommandEvent myEvent(wxEVT_RECV_MSG);
				myEvent.SetId(pMsgInfo->nMessageID);
				this->AddPendingEvent(myEvent);
			}

		}
		break;
	case Cmd_MA_ModifyPwd_Rsp:
		{
			if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS)
			{
				IPlatformSingleSvr* pSvr = DEFAULT_SVR();
				if ( NULL != pSvr )
				{
					pSvr->SetParticipantInfo("", pSvr->GetAccount(), m_strNewPwd, "");
				}

				wxMessageBox("�޸�����ɹ�", LOADSTRING(USERLOGINDLG_INFORMATION));
			}
			else
			{
				wxMessageBox(wxString::Format("�޸�����ʧ�ܣ�\n%s",(char*)(pRecvData->pData)),LOADSTRING(USERLOGINDLG_INFORMATION));
			}
		}
	case Cmd_MA_ForceCloseClient_Rsp:
		{
			wxMessageBox("���˻��ѱ�ͣ�ã����������ߣ�\n�������⣬����ϵ�ͷ���", LOADSTRING(USERLOGINDLG_INFORMATION));
			m_bForceClose = true;
			wxCloseEvent evtClose(wxEVT_CLOSE_WINDOW);
			AddPendingEvent(evtClose);
		}
	default:
		break;
	}

	return;
}

void MainFrame::OnRecvMsg( wxCommandEvent& event )
{
	std::map<int, MessageInfo>::iterator it = m_mapMsgInfo.find(event.GetId());
	if ( it == m_mapMsgInfo.end() )
	{
		return;
	}

	if ( NULL == m_pMsgDlg )
	{
		m_pMsgDlg = new MsgDlg(this);
		m_pMsgDlg->CenterOnScreen();
	}

	m_pMsgDlg->AddMessage(it->second);
	m_pMsgDlg->Show();
}

void MainFrame::OnReadMsg( wxCommandEvent& evt )
{
	std::map<int, MessageInfo> msgMap = m_mapMsgInfo;
	for ( UINT i = 0; i < m_vMsgInfo.size(); i++ )
	{
		msgMap[m_vMsgInfo[i].nMessageID] = m_vMsgInfo[i];
	}

	MsgListDlg dlg(this);
	dlg.SetMessageInfo(msgMap);
	dlg.ShowModal();
}

bool MainFrame::LoadSavedMessages( std::string strDate, std::vector<MessageInfo>& vMsgInfo )
{
	vMsgInfo.clear();
	
	std::string strPath = "Message\\" + strDate;
	strPath += ".msg";
	GlobalFunc::GetPath(strPath);
	if ( !wxFile::Exists(strPath.c_str()))
	{
		return false;
	}

	wxFile file;
	if( file.Open(strPath.c_str(),wxFile::read))
	{
		int nMsgCount = file.Length() / sizeof(MessageInfo);
		for ( int i = 0; i < nMsgCount; i++ )
		{
			MessageInfo msgInfo;
			memset(&msgInfo, 0, sizeof(msgInfo));
			file.Read(&msgInfo, sizeof(msgInfo));
			vMsgInfo.push_back(msgInfo);
		}

		file.Close();
		return true;
	}
	else
	{
		return false;
	}
}

bool MainFrame::SaveMessage()
{
	if ( m_mapMsgInfo.empty())
	{
		return true;
	}

	SYSTEMTIME time;
	GetLocalTime(&time);
	char szBuffer[100];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "Message\\%4d%02d%02d.msg", time.wYear, time.wMonth, time.wDay);
	std::string strPath = szBuffer;
	GlobalFunc::GetPath(strPath);

	wxFile file;
	if( file.Open(strPath.c_str(),wxFile::write_append))
	{
		std::map<int, MessageInfo>::iterator it = m_mapMsgInfo.begin();
		for ( ; it != m_mapMsgInfo.end(); it++ )
		{
			MessageInfo msgInfo = it->second;
			file.Write(&msgInfo, sizeof(msgInfo));
		}

		file.Flush();
		file.Close();
		return true;
	}
	else
	{
		return false;
	}
}

