#include "StdAfx.h"
#include "MyApp.h"
#include "TradingNoticeDlg.h"
IMPLEMENT_APP(CMyApp)
CMyApp::CMyApp(void):
m_DestHWnd(NULL)
{
}

CMyApp::~CMyApp(void)
{
}
bool CMyApp::OnInit()
{
	if(argc==2)
	{
		m_DestHWnd=(HWND)atoi(argv[1]);
		if(m_DestHWnd&&::IsWindow(m_DestHWnd))
		{
			//��ʾ������
			CTradingNoticeDlg* dlg=new CTradingNoticeDlg(NULL);	
			dlg->Show();
			//�޸�����
			HWND hThis=(HWND)dlg->GetHWND();
			::SetWindowLong(hThis,GWL_HWNDPARENT,(long)m_DestHWnd);
			//����������Ϣ
			COPYDATASTRUCT data;
			ZeroMemory( &data, sizeof(COPYDATASTRUCT));
			data.dwData=IE_MSG_LOGIN;//����
			data.cbData =sizeof(HWND);
			data.lpData =(void*)&hThis;//���
			::SendMessage(m_DestHWnd, WM_COPYDATA,IE_MAGIC_NUMBER, (LPARAM)&data);
			return true;
		}
	}
	return false;
}
int CMyApp::OnExit()
{
	//����������Ϣ
	COPYDATASTRUCT data;
	ZeroMemory( &data, sizeof(COPYDATASTRUCT));
	data.dwData=IE_MSG_EXIT;//�˳�
	::SendMessage(m_DestHWnd, WM_COPYDATA,IE_MAGIC_NUMBER, (LPARAM)&data);
	return wxApp::OnExit();
}