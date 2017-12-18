// AlgoTradingNo1.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "AlgoTradingNo1.h"
#include "AlgoTradingNo1Dlg.h"
#include "CommonDef.h"
#include "PlusinAPI.h"
#include "KernelStruct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HWND g_hWnd_FT=NULL;

// CAlgoTradingNo1App

BEGIN_MESSAGE_MAP(CAlgoTradingNo1App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAlgoTradingNo1App ����

CAlgoTradingNo1App::CAlgoTradingNo1App()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAlgoTradingNo1App ����

CAlgoTradingNo1App theApp;

// CAlgoTradingNo1App ��ʼ��

BOOL CAlgoTradingNo1App::InitInstance()
{
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

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


	if(__argc>1)
	{
//		::MessageBox(NULL,__argv[1],"",MB_OK);
		g_hWnd_FT = (HWND)atol(__argv[1]);
		if(!::IsWindow(g_hWnd_FT))
			g_hWnd_FT=NULL;
	}


	CAlgoTradingNo1Dlg dlg;
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

int CAlgoTradingNo1App::ExitInstance()
{

	return CWinApp::ExitInstance();
}


