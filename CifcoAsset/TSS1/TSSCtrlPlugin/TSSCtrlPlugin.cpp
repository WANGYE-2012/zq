// TSSCtrlPlugin.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "TSSCtrlPlugin.h"
#include "TSSCtrlPluginDlg.h"
#include "ConfigMgr.h"
#include "TcpLayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTSSCtrlPluginApp

BEGIN_MESSAGE_MAP(CTSSCtrlPluginApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTSSCtrlPluginApp ����

CTSSCtrlPluginApp::CTSSCtrlPluginApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTSSCtrlPluginApp ����

CTSSCtrlPluginApp theApp;


// CTSSCtrlPluginApp ��ʼ��

BOOL CTSSCtrlPluginApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
#if 1
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
#endif
	
	AfxInitRichEdit2();
 

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

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	//�������ò���
	CConfigManager::Load();

	LoadConfig();
	BOOKORDERDLGCOLOR colorStyle;
	GetColorStyle(GetColorStyle(), colorStyle);
	SetBookOrderDlgColor(colorStyle, GetColorStyle());

	////��ʼ��ͨ�Ų�
	//CTcpLayer::Init();

	GetChangeOrderManager().Init();

	//LONG rtn = GetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE);
	//rtn |= WS_EX_TOOLWINDOW;
	////rtn &= ~WS_EX_APPWINDOW;
	//SetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE, rtn);

	//// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	////pFrame->ShowWindow(SW_HIDE);//SW_SHOW);
	//pFrame->SetWindowPos(NULL, -10000, -10000, 0, 0, SWP_SHOWWINDOW);
	//pFrame->SetTitle("һ���µ�");
	////pFrame->ShowWindow(SW_SHOW);
	//pFrame->UpdateWindow();

	CTSSCtrlPluginDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CTSSCtrlPluginApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	
	//�ͷ�ͨ�Ų�
	CTcpLayer::Release();

	SetPlusinAPI(NULL);
	PlusinAPI::DestroyInstance();
	SaveConfig();

	return CWinApp::ExitInstance();
}
