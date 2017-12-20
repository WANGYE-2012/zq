// ReadZQLogDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReadZQLog.h"
#include "ReadZQLogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// �ַ�ӳ���
const int g_AscConv[] = {
	228, 	178, 	68, 	155, 	118, 	177, 	35, 	221, 	126, 	231, 	
	180, 	46, 	161, 	208, 	63, 	249, 	47, 	116, 	182, 	210, 	
	36, 	235, 	176, 	166, 	100, 	58, 	150, 	223, 	237, 	198, 	
	137, 	169, 	81, 	69, 	119, 	234, 	52, 	111, 	127, 	164, 	
	195, 	64, 	136, 	60, 	142, 	101, 	70, 	191, 	192, 	190, 	
	255, 	165, 	230, 	132, 	160, 	187, 	139, 	43, 	92, 	114, 	
	250, 	107, 	88, 	33, 	84, 	233, 	51, 	91, 	144, 	44, 	
	207, 	181, 	93, 	90, 	40, 	227, 	185, 	152, 	240, 	121, 	
	163, 	123, 	143, 	246, 	239, 	124, 	244, 	154, 	77, 	94, 	
	175, 	254, 	85, 	215, 	162, 	170, 	134, 	86, 	98, 	211, 	
	61, 	48, 	120, 	217, 	138, 	95, 	146, 	99, 	213, 	75, 	
	65, 	87, 	197, 	125, 	106, 	140, 	149, 	115, 	96, 	199, 	
	194, 	39, 	97, 	252, 	103, 	109, 	37, 	209, 	245, 	128, 	
	117, 	49, 	67, 	214, 	129, 	151, 	148, 	45, 	41, 	205, 	
	80, 	225, 	168, 	108, 	79, 	174, 	171, 	236, 	38, 	202, 	
	32, 	220, 	229, 	72, 	184, 	110, 	159, 	59, 	226, 	156, 	
	183, 	242, 	189, 	206, 	57, 	248, 	56, 	172, 	73, 	83, 	
	200, 	74, 	53, 	157, 	167, 	89, 	147, 	201, 	42, 	82, 	
	112, 	179, 	131, 	186, 	50, 	158, 	130, 	62, 	141, 	54, 	
	102, 	204, 	105, 	153, 	122, 	145, 	238, 	251, 	203, 	241, 	
	218, 	188, 	212, 	193, 	113, 	135, 	133, 	76, 	104, 	196, 	
	55, 	232, 	66, 	243, 	173, 	216, 	247, 	253, 	78, 	34, 	
	219, 	222, 	224, 	71 };


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


// CReadZQLogDlg �Ի���




CReadZQLogDlg::CReadZQLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReadZQLogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	int nSize = sizeof(g_AscConv)/sizeof(int);
	for(int i=0; i<nSize; i++) {
		m_mapAscConv.insert(std::make_pair((BYTE)g_AscConv[i], (BYTE)(i+32)));
	}
}

void CReadZQLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHZQLOG, m_richLog);
}

BEGIN_MESSAGE_MAP(CReadZQLogDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CReadZQLogDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CReadZQLogDlg::OnBnClickedCancel)
	ON_WM_SIZE()
END_MESSAGE_MAP()


BYTE CReadZQLogDlg::ConvAscII(BYTE cAsc)
{
	std::map<BYTE, BYTE>::iterator it = 
		m_mapAscConv.find(cAsc);
	if(it==m_mapAscConv.end())
		return cAsc;
	else
		return it->second;
}

BOOL CReadZQLogDlg::OpenAndShowFile(LPCSTR pcsFName)
{
	CFile file;
	if(!file.Open(pcsFName, CFile::modeRead)) 
		return FALSE;

	ULONGLONG dwSize = file.GetLength();
	if(dwSize>10*1024*1024) {
		file.Close();
		return FALSE;
	}
	BYTE* pBuff = new BYTE[dwSize+8];
	ZeroMemory(pBuff, dwSize+8);

	file.Read(pBuff, dwSize);
	file.Close();

	for(DWORD i=0; i<dwSize; i++) {
		pBuff[i] = ConvAscII(pBuff[i]);
	}
	m_richLog.SetSel(0, -1);
	m_richLog.ReplaceSel((LPCTSTR)pBuff);
	return TRUE;
}


// CReadZQLogDlg ��Ϣ�������

BOOL CReadZQLogDlg::OnInitDialog()
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

	CRect rect;
	
	GetClientRect(&rect);
	CWnd* pWnd = GetDlgItem(IDC_RICHZQLOG);
	if(pWnd!=NULL) {
		pWnd->MoveWindow(0, 0, rect.Width(), rect.Height());
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CReadZQLogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CReadZQLogDlg::OnPaint()
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
HCURSOR CReadZQLogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CReadZQLogDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CReadZQLogDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CReadZQLogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	CWnd* pWnd = GetDlgItem(IDC_RICHZQLOG);
	if(pWnd!=NULL) {
		pWnd->MoveWindow(0, 0, cx, cy);
	}
}

void CReadZQLogDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default

	int nFileCount = ::DragQueryFile(hDropInfo,-1,NULL,NULL);
	if(nFileCount<=0)
		return;
	
	int nFSize = ::DragQueryFile(hDropInfo, 0, NULL, NULL);
	HANDLE hHeap = ::GetProcessHeap();
	char* pcsFName = (char *)::HeapAlloc(hHeap, HEAP_ZERO_MEMORY, ++nFSize);
	::DragQueryFile(hDropInfo, 0, (LPSTR)pcsFName, nFSize);

	if(OpenAndShowFile((LPCSTR)pcsFName))
		SetWindowText((LPCSTR)pcsFName);
	
	::HeapFree(hDropInfo, HEAP_ZERO_MEMORY, pcsFName);
	
	CDialog::OnDropFiles(hDropInfo);
}
