// updatefasttrader.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "updatefasttrader.h"
#include "InternetGetFile.h"

#include "ProgressDlg.h"
#include   <fstream> 

using   namespace   std; 
#define MAX_LOADSTRING 100

IMPLEMENT_APP(CUpdateFasttraderApp)


bool CUpdateFasttraderApp::OnInit()
{
	//m_pszExeName=wxApp::GetAppName();
	// Create the main application window
	// �������Ƿ��Ѿ������У�������ֱ���˳�
	wxString str;
	str.Printf(wxT("WPD800-50830-28132-57903-%d"), argc);
	HANDLE hSem = ::CreateSemaphore(NULL, 1, 1, str.c_str()); 
	if (GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 
		// �ر��ź������ 
		CloseHandle(hSem); 
		// �˳���ʵ�� 
		return false; 
	} 



	if(argc == 1)
	{
        CheckVersion::DownLoadUpdateXML_ForMainExe();
		return false;
	}
    CProgressDlg* dlg=new CProgressDlg(NULL);	
	dlg->SetMainProcExe(argv[1]);
	dlg->CreateUpdateThread();
    dlg->Show();
	return true;
}

extern char g_Url[1024];
