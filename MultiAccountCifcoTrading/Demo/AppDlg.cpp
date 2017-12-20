// AppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "AppDlg.h"
#include "../inc/PlusinCommon/PlusinAPI.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
CAppDlg* g_pAppDlg = NULL;
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppDlg dialog

CAppDlg::CAppDlg(CWnd* pParent /*=NULL*/, HWND hTargetWnd)
	: CDialog(CAppDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAppDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	g_pAppDlg = this;
	m_hTargetWnd = hTargetWnd;
	m_nRequestId = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAppDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_BOX, m_listBox);
}

BEGIN_MESSAGE_MAP(CAppDlg, CDialog)
	//{{AFX_MSG_MAP(CAppDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CAppDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAppDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CAppDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CAppDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &CAppDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CAppDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CAppDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CAppDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON10, &CAppDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CAppDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CAppDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON9, &CAppDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON13, &CAppDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CAppDlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON16, &CAppDlg::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON15, &CAppDlg::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON18, &CAppDlg::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON19, &CAppDlg::OnBnClickedButton19)
	ON_BN_CLICKED(IDC_BUTTON20, &CAppDlg::OnBnClickedButton20)
	ON_BN_CLICKED(IDC_BUTTON21, &CAppDlg::OnBnClickedButton21)
	ON_BN_CLICKED(IDC_BUTTON22, &CAppDlg::OnBnClickedButton22)
	ON_BN_CLICKED(IDC_BUTTON23, &CAppDlg::OnBnClickedButton23)
	ON_BN_CLICKED(IDC_BUTTON17, &CAppDlg::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON24, &CAppDlg::OnBnClickedButton24)
	ON_BN_CLICKED(IDC_BUTTON25, &CAppDlg::OnBnClickedButton25)
	ON_BN_CLICKED(IDC_BUTTON26, &CAppDlg::OnBnClickedButton26)
	ON_BN_CLICKED(IDC_BUTTON27, &CAppDlg::OnBnClickedButton27)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppDlg message handlers

BOOL CAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_HIDE);

	m_pApi = PlusinAPI::GetInstance();
	m_pApi->Init("Demo", "123", OnRecvApiMsg, m_hTargetWnd, POS_QUOTE, m_hWnd);
	m_pApi->Logon();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAppDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAppDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CAppDlg::OnRecvApiMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId )
{
	if ( nCmdId >= CMDID_CONSOLE_MIN && nCmdId <= CMDID_CONSOLE_MAX)
	{
		ProcessConsoleMsg(nCmdId, nErrorCode, pData, nDataLen);
	}
	else if ( nCmdId >= CMDID_EXCHANGE_MIN && nCmdId <= CMDID_EXCHANGE_MAX )
	{
		ProcessExchangeMsg(nCmdId, nErrorCode, pData, nDataLen);
	}
	else if ( nCmdId >= CMDID_INSTRUMENT_MIN && nCmdId <= CMDID_INSTRUMENT_MAX )
	{
		ProcessInstrumentMsg(nCmdId, nErrorCode, pData, nDataLen);
	}
	else if ( nCmdId >= CMDID_QUOTE_MIN && nCmdId <= CMDID_QUOTE_MAX )
	{
		ProcessQuoteMsg(nCmdId, nErrorCode, pData, nDataLen);
	}
	else if (nCmdId >= CMDID_ACCOUNT_MIN && nCmdId <= CMDID_ACCOUNT_MAX)
	{
		ProcessAccountMsg(nCmdId, nErrorCode, pData, nDataLen);
	}
	else if ( nCmdId >= CMDID_ALL_ORDER_MIN && nCmdId <= CMDID_ALL_ORDER_MAX )
	{
		ProcessAllOrderMsg(nCmdId, nErrorCode, pData, nDataLen);
	}
	else if ( nCmdId >= CMDID_POSITION_MIN && nCmdId <= CMDID_POSITION_MAX )
	{
		ProcessPositionMsg(nCmdId, nErrorCode, pData, nDataLen);
	}
	else if ( nCmdId >= CMDID_ORDER_INSERT_MIN && nCmdId <= CMDID_ORDER_INSERT_MAX )
	{
		ProcessOrderMsg(nCmdId, nErrorCode, pData, nDataLen);
	}
	else if ( nCmdId >= CMDID_CONFIG_MIN && nCmdId <= CMDID_CONFIG_MAX )
	{
		ProcessConfigMsg(nCmdId, nErrorCode, pData, nDataLen);
	}

	//CString csMsg;
	//csMsg.Format("RequestId: %d", nRequestId);
	//g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), csMsg);
	return 0;
}

CAppDlg::~CAppDlg()
{
	m_pApi->Logout();
	if ( NULL != m_pApi )
	{
		m_pApi->DestroyInstance();
		m_pApi = NULL;
	}
}

void CAppDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if( NULL == m_pApi)
	{
		return;
	}

	m_pApi->Send(CMDID_EXCHANGE_LIST_QUERY, NULL, 0, GetNextRequestId());
}

void CAppDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	if( NULL == m_pApi)
	{
		return;
	}

	char szExchangeID[32] = "SHFE";
	m_pApi->Send(CMDID_EXCHANGE_CURRENT_TIME_QUERY, szExchangeID, 
		sizeof(szExchangeID), GetNextRequestId());
}

void CAppDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	if( NULL == m_pApi)
	{
		return;
	}

	char szExchangeID[32] = "SHFE";
	m_pApi->Send(CMDID_EXCHANGE_TRADE_TIME_QUERY, szExchangeID, 
		sizeof(szExchangeID), GetNextRequestId());
}

void CAppDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	if( NULL == m_pApi)
	{
		return;
	}

	char szInstrumentID[64] = "IF1205";
	m_pApi->Send(CMDID_INSTRUMENT_INFO_QUERY, szInstrumentID, 
		sizeof(szInstrumentID), GetNextRequestId());
}

void CAppDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	if( NULL == m_pApi)
	{
		return;
	}

	char szExchangeID[32] = "SHFE";
	m_pApi->Send(CMDID_INSTRUMENT_LIST_QUERY, szExchangeID, 
		sizeof(szExchangeID), GetNextRequestId());
}

void CAppDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	if( NULL == m_pApi)
	{
		return;
	}

	char szInstrumentID[64] = "IF1205";
	m_pApi->Send(CMDID_QUOTE_SUBSCRIBE, szInstrumentID, 
		sizeof(szInstrumentID), GetNextRequestId());
}

void CAppDlg::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here
	if( NULL == m_pApi)
	{
		return;
	}

	char szInstrumentID[64] = "IF1205";
	m_pApi->Send(CMDID_QUOTE_UNSUBSCRIBE, szInstrumentID, 
		sizeof(szInstrumentID), GetNextRequestId());
}

void CAppDlg::OnBnClickedButton8()
{
	// TODO: Add your control notification handler code here
	m_listBox.ResetContent();
}

void CAppDlg::OnBnClickedButton10()
{
	// TODO: Add your control notification handler code here
	m_pApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, GetNextRequestId());
}

void CAppDlg::OnBnClickedButton11()
{
	// TODO: Add your control notification handler code here
	m_pApi->Send(CMDID_ACCOUNT_SUB_ACCOUNT_LIST_QUERY, NULL, 0, GetNextRequestId());
}

void CAppDlg::OnBnClickedButton12()
{
	// TODO: Add your control notification handler code here
	char szSubAccountID[13] = "123";
	m_pApi->Send(CMDID_ACCOUNT_SUB_ACCOUNT_INFO_QUERY, szSubAccountID, 
		sizeof(szSubAccountID), GetNextRequestId());
}

void CAppDlg::ProcessConsoleMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen )
{
	if ( nCmdId == CMDID_CONSOLE_LOGON_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			g_pAppDlg->ShowWindow(SW_SHOW);
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "��¼�ɹ�");
		}
		else
		{
			g_pAppDlg->SendMessage(WM_CLOSE, 0, 0);
		}
	}
	if ( nCmdId == CMDID_CONSOLE_CLOSE )
	{
		g_pAppDlg->SendMessage(WM_CLOSE, 0, 0);
	}
}

void CAppDlg::ProcessExchangeMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen )
{
	if ( nCmdId == CMDID_EXCHANGE_LIST_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			if ( NULL == pData || nDataLen % 32 != 0)
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���صĽ������б����ݸ�ʽ����");
			}
			else
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�������б����ݷ��أ�");
				char* pTemp = (char*)pData;
				for ( int i = 0; i < nDataLen/32; i++ )
				{
					char szBuf[32];
					memset(szBuf, 0, sizeof(szBuf));
					memcpy(szBuf, pTemp+i*32, 32);
					g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), szBuf);
				}
			}
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���������б�ʧ��");
		}
	}
	else if ( nCmdId == CMDID_EXCHANGE_CURRENT_TIME_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			if ( NULL == pData )
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���ؽ�������ǰʱ�����ݸ�ʽ����");
			}
			else
			{
				char szBuf[10];
				memset(szBuf, 0, sizeof(szBuf));
				memcpy(szBuf, pData, nDataLen);
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���ؽ�������ǰʱ�䣺");
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), szBuf);
			}
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "����������ǰʱ��ʧ��");
		}
	}
	else if ( nCmdId == CMDID_EXCHANGE_TRADE_TIME_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			if ( NULL == pData )
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���ؽ���������ʱ�����ݸ�ʽ����");
			}
			else
			{
				char szBuf[256];
				memset(szBuf, 0, sizeof(szBuf));
				memcpy(szBuf, pData, nDataLen);
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���ؽ���������ʱ�䣺");
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), szBuf);
			}
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "������������ʱ��ʧ��");
		}
	}
}

void CAppDlg::ProcessInstrumentMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen )
{
	if ( nCmdId == CMDID_INSTRUMENT_LIST_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			if ( NULL == pData || nDataLen % 64 != 0)
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���صĺ�Լ�б����ݸ�ʽ����");
			}
			else
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "��Լ�б����ݷ��أ�");
				char* pTemp = (char*)pData;
				for ( int i = 0; i < nDataLen/64; i++ )
				{
					char szBuf[64];
					memset(szBuf, 0, sizeof(szBuf));
					memcpy(szBuf, pTemp+i*64, 64);
					g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), szBuf);
				}
			}
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�����Լ�б�ʧ��");
		}
	}
	else if ( nCmdId == CMDID_INSTRUMENT_INFO_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			//PlatformStru_InstrumentInfo
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�����Լ����ɹ�");
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�����Լ����ʧ��");
		}
	}
	else if ( nCmdId == CMDID_INSTRUMENT_SHORTNAME_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			//PlatformStru_InstrumentInfo
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ���Լ��Ʊ�");
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), (char*)pData);
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�����Լ��Ʊ�ʧ��");
		}
	}
}

void CAppDlg::ProcessQuoteMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen )
{
	if ( nCmdId == CMDID_QUOTE_CURRENT_SEL)
	{
		char szBuf[64];
		memset(szBuf, 0, sizeof(szBuf));
		memcpy(szBuf, pData, nDataLen);
		CString csTemp;
		csTemp.Format("���۱�˫����Լ��%s", szBuf);
		g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), csTemp);
	}
	else if ( nCmdId == CMDID_QUOTE_SEND_QUOTE )
	{
		//if ( NULL == msg.lpData || msg.cbData != sizeof(PlatformStru_DepthMarketData) )
		//{

		//}
		//else
		//{
		//	PlatformStru_DepthMarketData* pMarketData = (PlatformStru_DepthMarketData*)msg.lpData;
		//	CString csTemp;
		//	csTemp.Format("�յ����飬��ԼIDΪ��%s", pMarketData->InstrumentID);
		g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ�����");
		//}
	}
	else if ( nCmdId == CMDID_QUOTE_SUBSCRIBE_RSP )
	{
		CString csMsg;
		if ( nErrorCode == SUCCESS )
		{
			csMsg = "���ĳɹ�";
		}
		else if ( nErrorCode == ERROR_ONE)
		{
			csMsg = "���ĵĺ�Լ���������������";
		}
		else
		{
			csMsg = "���ĺ�Լʧ�ܣ�δ֪����";
		}

		g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), csMsg);
	}
	else if ( nCmdId == CMDID_QUOTE_SNAPSHOT_RSP )
	{
		//PlatformStru_DepthMarketData
		g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ�������շ���");
	}
	else if ( nCmdId == CMDID_QUOTE_CURRENT_GROUP_INST_LIST )
	{
		if ( NULL == pData || nDataLen % 64 != 0)
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "��ǰ��Լ���ں�Լ�б����ݸ�ʽ����");
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ���ǰ��Լ���ں�Լ�б����ݣ�");
			char* pTemp = (char*)pData;
			for ( int i = 0; i < nDataLen/64; i++ )
			{
				char szBuf[64];
				memset(szBuf, 0, sizeof(szBuf));
				memcpy(szBuf, pTemp+i*64, 64);
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), szBuf);
			}
		}
	}
	else if ( nCmdId == CMDID_QUOTE_IS_SIMULATE_RSP)
	{
		if ( NULL != pData && nDataLen == sizeof(int))
		{
			int nSimulate = *(int*)pData;
			if ( nSimulate == 1)
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "��ǰ��¼����ģ����");
			}
			else
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "��ǰ��¼����ʵ��");
			}
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�Ƿ�ģ���̲�ѯʧ��");
		}
	}
	else
	{

	}
}

void CAppDlg::ProcessAccountMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen )
{
	if ( nCmdId == CMDID_ACCOUNT_SUB_ACCOUNT_LIST_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			if ( NULL == pData || nDataLen % 13 != 0)
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���ص����˻��б����ݸ�ʽ����");
			}
			else
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���˻��б����ݷ��أ�");
				char* pTemp = (char*)pData;
				for ( int i = 0; i < nDataLen/13; i++ )
				{
					char szBuf[13];
					memset(szBuf, 0, sizeof(szBuf));
					memcpy(szBuf, pTemp+i*13, 13);
					g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), szBuf);
				}
			}
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�������˻��б�ʧ��");
		}
	}
	else if ( nCmdId == CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			//PlatformStru_TradingAccountInfo
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�������˻�����ɹ�");
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�������˻�����ʧ��");
		}
	}
	else if ( nCmdId == CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP)
	{
		if ( nErrorCode == SUCCESS )
		{
			if ( NULL != pData && nDataLen != 0 )
			{
				CString csMsg;
				csMsg.Format("���ص�ǰ�˻�: %s", (char*)pData);
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), csMsg);
			}
			else
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���صĵ�ǰ�˻����ݴ���");
			}
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "����ǰ�˻�ʧ��");
		}
	}
	else if ( nCmdId == CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			if ( NULL != pData && nDataLen != 0 )
			{
				CString csMsg;
				csMsg.Format("���ص�ǰ���˺�: %s", (char*)pData);
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), csMsg);
			}
			else
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���صĵ�ǰ���˺����ݴ���");
			}
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "����ǰ���˺�ʧ��");
		}
	}
	else if ( nCmdId == CMDID_ACCOUNT_REQUERY )
	{
		g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ��ʽ��˻����²�ѯ�¼�");
	}
	else
	{

	}
}

void CAppDlg::ProcessAllOrderMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen )
{
	if ( nCmdId == CMDID_ALL_ORDER_LIST_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			if ( NULL != pData && nDataLen != 0 )
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ�ί�е�ID�б�:");
				char* pTemp = (char*)pData;
				for ( int i = 0; i < nDataLen/sizeof(int); i++ )
				{
					CString csMsg;
					csMsg.Format("%d", *(int*)(pTemp+i*sizeof(int)));
					g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), csMsg);
				}
			}
			else
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���ص�ί�е�ID�б����ݴ���");
			}
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "����ί�е�ID�б�ʧ��");
		}
	}
	else if ( nCmdId == CMDID_ALL_ORDER_INFO_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			//PlatformStru_OrderInfo
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ�ί�е�����");
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "����ί�е�����ʧ��");
		}
	}
	else if ( nCmdId == CMDID_ALL_ORDER_CURRENT_SEL )
	{
		if ( NULL != pData && nDataLen != 0 )
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ�ί�е�ģ�鵱ǰѡ����ID�б�:");
			char* pTemp = (char*)pData;
			for ( int i = 0; i < nDataLen/sizeof(int); i++ )
			{
				CString csMsg;
				csMsg.Format("%d", *(int*)(pTemp+i*sizeof(int)));
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), csMsg);
			}
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���ص�ί�е�ģ�鵱ǰѡ����ID�б����ݴ���");
		}
	}
	else if ( nCmdId == CMDID_ALL_ORDER_REQUERY )
	{
		g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ�ί�е����²�ѯ�¼�");
	}
}

void CAppDlg::ProcessPositionMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen )
{
	if ( nCmdId == CMDID_POSITION_LIST_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			if ( NULL != pData && nDataLen != 0 )
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ��ֲ�ID�б�:");
				char* pTemp = (char*)pData;
				for ( int i = 0; i < nDataLen/sizeof(int); i++ )
				{
					CString csMsg;
					csMsg.Format("%d", *(int*)(pTemp+i*sizeof(int)));
					g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), csMsg);
				}
			}
			else
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���صĳֲ�ID�б����ݴ���");
			}
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "����ֲ�ID�б�ʧ��");
		}
	}
	else if ( nCmdId == CMDID_POSITION_INFO_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			//PlatformStru_Position
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ��ֲ�����");
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "����ֲ�����ʧ��");
		}
	}
	else if ( nCmdId == CMDID_POSITION_CURRENT_SEL )
	{
		if ( NULL != pData && nDataLen != 0 )
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ��ֲ�ģ�鵱ǰѡ����ID�б�:");
			char* pTemp = (char*)pData;
			for ( int i = 0; i < nDataLen/sizeof(int); i++ )
			{
				CString csMsg;
				csMsg.Format("%d", *(int*)(pTemp+i*sizeof(int)));
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), csMsg);
			}
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���صĳֲ�ģ�鵱ǰѡ����ID�б����ݴ���");
		}
	}
	else if ( nCmdId == CMDID_POSITION_COMBO_LIST_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			if ( NULL != pData && nDataLen != 0 )
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ���ϳֲ�ID�б�:");
				char* pTemp = (char*)pData;
				for ( int i = 0; i < nDataLen/sizeof(int); i++ )
				{
					CString csMsg;
					csMsg.Format("%d", *(int*)(pTemp+i*sizeof(int)));
					g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), csMsg);
				}
			}
			else
			{
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���ص���ϳֲ�ID�б����ݴ���");
			}
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "������ϳֲ�ID�б�ʧ��");
		}
	}
	else if ( nCmdId == CMDID_POSITION_COMBO_INFO_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			//PlatformStru_PositionComb
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ���ϳֲ�����");
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "������ϳֲ�����ʧ��");
		}
	}
	else if ( nCmdId == CMDID_POSITION_COMBO_CURRENT_SEL )
	{
		if ( NULL != pData && nDataLen != 0 )
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ���ϳֲ�ģ�鵱ǰѡ����ID�б�:");
			char* pTemp = (char*)pData;
			for ( int i = 0; i < nDataLen/sizeof(int); i++ )
			{
				CString csMsg;
				csMsg.Format("%d", *(int*)(pTemp+i*sizeof(int)));
				g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), csMsg);
			}
		}
		else
		{
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "���ص���ϳֲ�ģ�鵱ǰѡ����ID�б����ݴ���");
		}
	}
	else if ( nCmdId == CMDID_POSITION_REQUERY )
	{
		g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ��ֲ����²�ѯ�¼�");
	}
	else if ( nCmdId == CMDID_POSITION_COMBO_REQUERY )
	{
		g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ���ϳֲ����²�ѯ�¼�");
	}
}

void CAppDlg::ProcessOrderMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen )
{
	if ( nCmdId == CMDID_ORDER_INSERT_ORDER_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			//PlatformStru_PositionComb
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�µ��ɹ�");
		}
		else
		{
			CString csMsg;
			csMsg.Format("�µ�ʧ��, ������Ϊ��%d", nErrorCode);
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), csMsg);
		}
	}
	else if ( nCmdId == CMDID_ORDER_CANCEL_ORDER_RSP )
	{
		if ( nErrorCode == SUCCESS )
		{
			//PlatformStru_PositionComb
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�����ɹ�");
		}
		else
		{
			CString csMsg;
			csMsg.Format("����ʧ��, ������Ϊ��%d", nErrorCode);
			g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), csMsg);
		}
	}
	else if ( nCmdId == CMDID_ORDER_ORDER_STATUS_CHANGE )
	{
		//PlatformStru_OrderInfo
		g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ������ر�");
	}
	else if ( nCmdId == CMDID_ORDER_TRADE_RTN )
	{
		//PlatformStru_TradeInfo
		g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ��ɽ��ر�");
	}
	else
	{

	}
}

void CAppDlg::ProcessConfigMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen )
{
	if ( nCmdId == CMDID_CONFIG_CHANGE )
	{
		//PlatformStru_TradeInfo
		g_pAppDlg->m_listBox.InsertString(g_pAppDlg->m_listBox.GetCount(), "�յ����øı���Ϣ");
	}
	else
	{
	}
}

void CAppDlg::OnBnClickedButton9()
{
	// TODO: Add your control notification handler code here
	char szInstrumentID[64] = "IF1205";
	m_pApi->Send(CMDID_QUOTE_SNAPSHOT_QUERY, szInstrumentID, 
		sizeof(szInstrumentID), GetNextRequestId());
}

int CAppDlg::GetNextRequestId()
{
	m_nRequestId++;
	return m_nRequestId;
}

void CAppDlg::OnBnClickedButton13()
{
	// TODO: Add your control notification handler code here
	m_pApi->Send(CMDID_ACCOUNT_CURRENT_ACCOUNT_QUERY, NULL, 0, GetNextRequestId());
}

void CAppDlg::OnBnClickedButton14()
{
	// TODO: Add your control notification handler code here
	m_pApi->Send(CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_QUERY, NULL, 0, GetNextRequestId());
}

void CAppDlg::OnBnClickedButton15()
{
	// TODO: Add your control notification handler code here
	m_pApi->Send(CMDID_POSITION_LIST_QUERY, NULL, 0, GetNextRequestId());
}

void CAppDlg::OnBnClickedButton16()
{
	// TODO: Add your control notification handler code here
	//����FTID
	long lFTID = 1;
	m_pApi->Send(CMDID_POSITION_INFO_QUERY1, &lFTID, sizeof(lFTID), GetNextRequestId());
}

void CAppDlg::OnBnClickedButton17()
{
	// TODO: Add your control notification handler code here
	//����PositionKey
	//PositionKey key = 0;
	//m_pApi->Send(CMDID_POSITION_INFO_QUERY2, &key, sizeof(PositionKey), GetNextRequestId());
}

void CAppDlg::OnBnClickedButton18()
{
	// TODO: Add your control notification handler code here
	m_pApi->Send(CMDID_POSITION_COMBO_LIST_QUERY, NULL, 0, GetNextRequestId());
}

void CAppDlg::OnBnClickedButton19()
{
	// TODO: Add your control notification handler code here
	//����FTID
	long lFTID = 1;
	m_pApi->Send(CMDID_POSITION_COMBO_INFO_QUERY, &lFTID, sizeof(lFTID), GetNextRequestId());
}

void CAppDlg::OnBnClickedButton20()
{
	// TODO: Add your control notification handler code here
}

void CAppDlg::OnBnClickedButton21()
{
	// TODO: Add your control notification handler code here
	m_pApi->Send(CMDID_ALL_ORDER_LIST_QUERY, NULL, 0, GetNextRequestId());
}

void CAppDlg::OnBnClickedButton22()
{
	// TODO: Add your control notification handler code here
	long lFTID = 1;
	m_pApi->Send(CMDID_ALL_ORDER_INFO_QUERY1, &lFTID, sizeof(lFTID), GetNextRequestId());
}

void CAppDlg::OnBnClickedButton23()
{
	// TODO: Add your control notification handler code here
	//����OrderKey
	//OrderKey key;
	//m_pApi->Send(CMDID_ALL_ORDER_INFO_QUERY2, &key, sizeof(OrderKey), GetNextRequestId());
}


void CAppDlg::OnBnClickedButton24()
{
	// TODO: Add your control notification handler code here
	//PlatformStru_InputOrder inputOrder;
	//m_pApi->Send(CMDID_ORDER_INSERT_ORDER, &inputOrder, sizeof(PlatformStru_InputOrder), GetNextRequestId());
}

void CAppDlg::OnBnClickedButton25()
{
	// TODO: Add your control notification handler code here
	//PlatformStru_InputOrderAction inputOrderAction;
	//m_pApi->Send(CMDID_ORDER_CANCEL_ORDER, &inputOrderAction, sizeof(PlatformStru_InputOrderAction), GetNextRequestId());
}

void CAppDlg::OnBnClickedButton26()
{
	// TODO: Add your control notification handler code here
	m_pApi->Send(CMDID_QUOTE_IS_SIMULATE_QUERY, NULL, 0, GetNextRequestId());
}

void CAppDlg::OnBnClickedButton27()
{
	// TODO: Add your control notification handler code here
	m_pApi->Send(CMDID_INSTRUMENT_SHORTNAME_QUERY, NULL, 0, GetNextRequestId());
}
