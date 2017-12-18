// TestTPriceListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestTPriceList.h"
#include "TestTPriceListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestTPriceListDlg �Ի���




CTestTPriceListDlg::CTestTPriceListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestTPriceListDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestTPriceListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestTPriceListDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CTestTPriceListDlg::OnBnClickedOk)
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTestTPriceListDlg ��Ϣ�������

BOOL CTestTPriceListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	CreateTPriceList(0);
	//SetTimer(conTimerCreateTPriceList, 15, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTestTPriceListDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestTPriceListDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTestTPriceListDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestTPriceListDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	CreateTPriceList(1);

	//OnOK();
}

void CTestTPriceListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_pPanel != NULL)
		::MoveWindow(m_pPanel, 0, 30, cx, cy-30-1, TRUE);
}

LRESULT CTestTPriceListDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if(message==Msg_PriceListT_Click) {
		int nEventID = wParam;
		int nClickMode = 0;
		std::string strInstrumentID;
		BOOL bDirection = TRUE;
		bool bRet = Get_Del_ClickParam(nEventID, nClickMode, strInstrumentID, bDirection);
		if(bRet) {
			char csText[1024] = {0};
			if(nClickMode==0 || nClickMode==1) 
				sprintf(csText, "�յ���Լ[%s]����[%s]����", strInstrumentID.c_str(), bDirection ? "buy" : "sell");
			else if(nClickMode==2)
				sprintf(csText, "KLine��ʾ�¼�����Լ[%s]", strInstrumentID.c_str());
			else
				sprintf(csText, "δ֪���¼������Բ�����");
			::AfxMessageBox(csText);
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CTestTPriceListDlg::CreateTPriceList(int nMode)
{
	HWND pPanel = CreateWnd(this->GetSafeHwnd());
	if(pPanel==NULL)
		return FALSE;

	::SetParent(pPanel, GetSafeHwnd());
	m_pPanel = pPanel;
	CRect rect;

	GetClientRect(&rect);
	::ShowWindow(m_pPanel, SW_SHOW);
	::MoveWindow(m_pPanel, 0, 30, rect.Width(), rect.Height()-30-1, TRUE);

	vector<PlatformStru_InstrumentInfo> vecOptions;
	PlatformStru_InstrumentInfo instr;

#if 0

	strncpy(instr.ExchangeID, "CZCE", sizeof(instr.ExchangeID)-1);

	strncpy(instr.InstrumentID, "SR405C3500", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "SR405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 3500.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "SR405P3500", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "SR405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 3500.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "SR405C4000", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "SR405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 4000.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "SR405P4000", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "SR405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 4000.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "SR405C4500", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "SR405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 4500.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "SR405P4500", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "SR405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 4500.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "SR405C5000", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "SR405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 5000.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "SR405P5000", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "SR405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 5000.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "SR405C5500", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "SR405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 5500.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "SR405P5500", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "SR405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 5500.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "SR405C6000", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "SR405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 6000.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "SR405P6000", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "SR405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 6000.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "SR405C6500", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "SR405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 6500.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "SR405P6500", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "SR405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 6500.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "SR405C7000", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "SR405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 7000.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "SR405P7000", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "SR405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 7000.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

#else

	strncpy(instr.ExchangeID, "DCE", sizeof(instr.ExchangeID)-1);

	strncpy(instr.InstrumentID, "m1405-C-2200", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 2200.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-P-2200", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 2200.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-C-2400", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 2400.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-P-2400", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 2400.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-C-2600", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 2600.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-P-2600", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 2600.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-C-2800", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 2800.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-P-2800", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 2800.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-C-3000", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 3000.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-P-3000", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 3000.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-C-3200", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 3200.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-P-3200", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 3200.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-C-3400", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 3400.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-P-3400", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "3400", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 3400.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-C-3600", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 3600.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-P-3600", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 3600.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-C-3800", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 3800.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-P-3800", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 3800.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-C-4000", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 4000.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-P-4000", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 4000.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-C-4200", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 4200.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-P-4200", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 4200.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-C-4400", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 4400.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-P-4400", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 4400.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-C-4600", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 4600.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-P-4600", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 4600.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-C-4800", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 4800.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-P-4800", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 4800.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-C-5000", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 5000.0;
	instr.OptionsType = THOST_FTDC_CP_CallOptions;
	vecOptions.push_back(instr);

	strncpy(instr.InstrumentID, "m1405-P-5000", sizeof(instr.InstrumentID)-1);
	strncpy(instr.UnderlyingInstrID, "m1405", sizeof(instr.UnderlyingInstrID)-1);
	instr.StrikePrice = 5000.0;
	instr.OptionsType = THOST_FTDC_CP_PutOptions;
	vecOptions.push_back(instr);

#endif

	SetOptionInstruments(vecOptions);

	PlatformStru_DepthMarketData newMD;

#if 0
	strcpy(newMD.InstrumentID, "SR405P4500");
	newMD.LastPrice = 5320.0;
	newMD.AskPrice1 = 5325.0;
	newMD.BidPrice1 = 5320.0;
	newMD.Volume = 3710;
	newMD.PreSettlementPrice = 5310.0;

	NewMarketData(newMD);
	strcpy(newMD.InstrumentID, "SR405C6500");
	newMD.LastPrice = 3210.0;
	newMD.AskPrice1 = 3215.0;
	newMD.BidPrice1 = 3210.0;
	newMD.Volume = 1098;
	newMD.PreSettlementPrice = 3390.0;
	NewMarketData(newMD);
#else
	strcpy(newMD.InstrumentID, "m1405-P-3400");
	newMD.LastPrice = 5320.0;
	newMD.AskPrice1 = 5325.0;
	newMD.BidPrice1 = 5320.0;
	newMD.Volume = 3710;
	newMD.PreSettlementPrice = 5310.0;

	NewMarketData(newMD);
	strcpy(newMD.InstrumentID, "m1405-C-5000");
	newMD.LastPrice = 3210.0;
	newMD.AskPrice1 = 3215.0;
	newMD.BidPrice1 = 3210.0;
	newMD.PreSettlementPrice = 3390.0;
	newMD.Volume = 1098;
	NewMarketData(newMD);
#endif

	if(nMode) {
		SetColorMode(1);
	}
	else {
		SetColorMode(0);
	}

	return TRUE;
}

void CTestTPriceListDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==conTimerCreateTPriceList) {
		//if(CreateTPriceList(0))
		//	KillTimer(conTimerCreateTPriceList);
	}
	CDialog::OnTimer(nIDEvent);
}
