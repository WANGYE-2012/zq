// OptionPriceListT.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "OptionPriceListT.h"
#include "MainFrm.h"

#include "OptionPriceListTDoc.h"
#include "OptionPriceListTView.h"
#include "DlgOptionTTable.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COptionPriceListTApp

BEGIN_MESSAGE_MAP(COptionPriceListTApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &COptionPriceListTApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// COptionPriceListTApp ����

COptionPriceListTApp::COptionPriceListTApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� COptionPriceListTApp ����

COptionPriceListTApp theApp;


// COptionPriceListTApp ��ʼ��

BOOL COptionPriceListTApp::InitInstance()
{

	if(__argc!=2) {
		//AfxMessageBox("NULL == lpCmdLine");
		//return FALSE;
	}
	HWND hWnd = (HWND)atol(__argv[1]);
	//hWnd = (HWND)0x00091046;
	if(!IsWindow(hWnd)) {
		//AfxMessageBox("!IsWindow(hWnd)");
		//return FALSE;
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

	int nRunMode = ::GetPrivateProfileInt("Startup", "RunMode", conModePopup, GetIniFile());
	_SetRunMode(nRunMode);//conModePopup);//conModeChild);//
	char csTemp[256] = {0};
	int nRet = ::GetPrivateProfileString("Startup", "NewOptionType", "2", csTemp, sizeof(csTemp)-1, GetIniFile());
	csTemp[nRet] = 0;
	_SetNewOptionType(csTemp[0]);

	InitGridMap();
	// ����ȷʵ����ʾ��
	ResetDefaultShowCol();

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
	//LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	//// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	//// �������ĵ�����ܴ��ں���ͼ֮�������
	//CSingleDocTemplate* pDocTemplate;
	//pDocTemplate = new CSingleDocTemplate(
	//	IDR_MAINFRAME,
	//	RUNTIME_CLASS(COptionPriceListTDoc),
	//	RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
	//	RUNTIME_CLASS(COptionPriceListTView));
	//if (!pDocTemplate)
	//	return FALSE;
	//AddDocTemplate(pDocTemplate);


	LoadConfig();

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
	pFrame->Create(NULL, NULL, 0);
	//pFrame->LoadFrame(IDR_MAINFRAME, 0);
	//pFrame->LoadFrame(IDR_MAINFRAME,
	//	WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
	//	NULL);


	//// ������׼������DDE�����ļ�������������
	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);


	//// ��������������ָ����������
	//// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;

	LONG rtn = GetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE);
	//rtn |= WS_EX_TOOLWINDOW;
	rtn &= ~WS_EX_APPWINDOW;
	SetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE, rtn);

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	//pFrame->SetWindowPos(NULL, 300, 300, 980, 700, SWP_SHOWWINDOW);//|SWP_NOMOVE|SWP_NOSIZE);
	//pFrame->SetWindowPos(NULL, -10000, -10000, 0, 0, SWP_SHOWWINDOW);
	//pFrame->SetTitle("��ȨT�ͱ���");
	pFrame->SetWindowText("��ȨT�ͱ���");
	//pFrame->ShowWindow(SW_HIDE);
	pFrame->UpdateWindow();

	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	return TRUE;
}

int COptionPriceListTApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	SetPlusinAPI(NULL);
	PlusinAPI::DestroyInstance();
	SaveConfig();

	return CWinApp::ExitInstance();
}



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
void COptionPriceListTApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// COptionPriceListTApp ��Ϣ�������


BOOL COptionPriceListTApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(_GetRunMode()==conModePopup) {
		if(pMsg->message==WM_KEYDOWN) {
			if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE) {
				PlusinAPI* poPlusinApi = GetPlusinAPI();
				if(poPlusinApi!=NULL) {
					poPlusinApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, 0);
				}
			}
		}
	}

	return CWinApp::PreTranslateMessage(pMsg);
}
