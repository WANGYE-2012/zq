// Module-BookOrder.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Module-BookOrder.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CModuleBookOrderApp

BEGIN_MESSAGE_MAP(CModuleBookOrderApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CModuleBookOrderApp::OnAppAbout)
END_MESSAGE_MAP()


// CModuleBookOrderApp ����

CModuleBookOrderApp::CModuleBookOrderApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CModuleBookOrderApp ����

CModuleBookOrderApp theApp;


// CModuleBookOrderApp ��ʼ��

BOOL CModuleBookOrderApp::InitInstance()
{
	if(__argc!=2)
	{
		//AfxMessageBox("NULL == lpCmdLine");
		return FALSE;
	}
	HWND hWnd = (HWND)atol(__argv[1]);
	//hWnd = (HWND)0x00091046;
	if(!IsWindow(hWnd))
	{
		//AfxMessageBox("!IsWindow(hWnd)");
		return FALSE;
	}
	SetTargetHWnd(hWnd);
	
#ifdef _DEBUG
	{
		CString strText;
		strText.Format("%x", hWnd);
		if(::MessageBox(NULL, 
					  "����Ҫ��ʾ���No��ť��\n"
					  "��Ҫ�������Yes��ť�������Ϊ������һ��ASSERT��Ȼ������\"����\"��\n"
					  "Ȼ��ѡ�д�\"Module-BookOrder\"���̵�VC2008IDE����IDE��׽�����Ǻ�\n"
					  "��ʾ\"Break\"����\"Continue\"������ѡ��\"Continue\"��", 
					  "���� ��ʾ", 
					  MB_YESNO|MB_ICONWARNING|MB_APPLMODAL)==IDYES) 
		{
			//AfxMessageBox(strText);
			ASSERT(FALSE);
		}
	}
#endif

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���

	LoadConfig();
	BOOKORDERDLGCOLOR colorStyle;
	GetColorStyle(GetColorStyle(), colorStyle);
	SetBookOrderDlgColor(colorStyle, GetColorStyle());

	DWORD dwTimeout = -1;  
	::SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)&dwTimeout, 0);  
	if (dwTimeout >= 100) {  
		::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)0, SPIF_SENDCHANGE | SPIF_UPDATEINIFILE);  
	}

	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// ���������ؿ�ܼ�����Դ
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);


	GetChangeOrderManager().Init();


	LONG rtn = GetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE);
	rtn |= WS_EX_TOOLWINDOW;
	//rtn &= ~WS_EX_APPWINDOW;
	SetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE, rtn);


	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	//pFrame->ShowWindow(SW_HIDE);//SW_SHOW);
	pFrame->SetWindowPos(NULL, -10000, -10000, 0, 0, SWP_SHOWWINDOW);
	pFrame->SetTitle("һ���µ�");
	//pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	return TRUE;
}


// CModuleBookOrderApp ��Ϣ�������




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

// �������жԻ����Ӧ�ó�������
void CModuleBookOrderApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CModuleBookOrderApp ��Ϣ�������


int CModuleBookOrderApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	SetPlusinAPI(NULL);
	PlusinAPI::DestroyInstance();
	SaveConfig();

	return CWinApp::ExitInstance();
}
