#include "StdAfx.h"
#include "FrameHook.h"

#include "MainFrame.h"
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0) 
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1) 
DWORD g_StartIdleTime;//��ʼ���е�ʱ��
HHOOK g_hMouse=NULL; //���Ĺ��ӹ���
HHOOK g_hKeyBoard=NULL; //���̵Ĺ��ӹ���
wxWindow*	g_poDailog = NULL;//����Ŀ�괰��

HHOOK		g_hDailogKeyBoard=NULL;//Ŀ�괰�ڵļ��̹��ӹ���

LRESULT CALLBACK MouseProc( int nCode,WPARAM wParam, LPARAM lParam)
{
    g_StartIdleTime=GetTickCount();
    return CallNextHookEx(g_hMouse,nCode,wParam,lParam);
}
LRESULT CALLBACK KeyProc( int nCode,WPARAM wParam, LPARAM lParam)
{
    g_StartIdleTime=GetTickCount();
    DWORD hiWord = HIWORD(lParam);
    if ( nCode != HC_NOREMOVE && ((hiWord & KF_UP) == 0) )
    {
        int id = wxCharCodeMSWToWX(wParam, lParam);
        if ( id == 0 )
        {
            wxKeyEvent event(wxEVT_CHAR_EX);
            if ( (HIWORD(lParam) & KF_ALTDOWN) == KF_ALTDOWN )
                event.m_altDown = true;

            event.SetEventObject(NULL);
            event.m_keyCode = wParam;
            event.m_shiftDown = wxIsShiftDown();
            event.m_controlDown = wxIsCtrlDown();
#ifndef __WXWINCE__
            event.SetTimestamp(::GetMessageTime());
#endif
            wxWindow *win = wxGetActiveWindow();
            wxEvtHandler *handler;
            if ( win )
            {
                handler = win->GetEventHandler();
                event.SetId(win->GetId());
            }
            else
            {
                handler = wxTheApp;
                event.SetId(wxID_ANY);
            }

            if ( handler && handler->ProcessEvent(event) )
            {
                // processed
                return 1;
            }
        }
    }

    return (int)CallNextHookEx(g_hKeyBoard, nCode, wParam, lParam);
}
void SetMainHook(wxWindow* pParent,bool bInit)
{
    if(bInit)
    {
        g_hMouse = SetWindowsHookEx(WH_MOUSE,MouseProc,NULL,GetCurrentThreadId());
        g_hKeyBoard = SetWindowsHookEx(WH_KEYBOARD,KeyProc,NULL,GetCurrentThreadId());
    }
    else
    {
        UnhookWindowsHookEx(g_hMouse);
        UnhookWindowsHookEx(g_hKeyBoard);
    }
}
//���̻ص������������µ���
LRESULT CALLBACK DialogKeyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	int nKeyCode = wParam;//MapVirtualKey(wParam, 3);
	if(nCode == 0 && !(lParam&0x80000000)) {
		wxCommandEvent myEvent(wxEVT_PANEL_CHAR_HOOK);
		wxKeyEvent event;
		//wxWindow *win = FindFocus();
		//event.SetId(win==NULL ? 0 : win->GetId());
		event.m_keyCode = nKeyCode;
		event.m_controlDown = (KEYDOWN(VK_LCONTROL) | KEYDOWN(VK_RCONTROL))>0;
		event.m_altDown = (lParam&0x20000000)>0;//(KEYDOWN(VK_LMENU) | KEYDOWN(VK_RMENU));
		event.m_shiftDown = (KEYDOWN(VK_LSHIFT) | KEYDOWN(VK_RSHIFT))>0;
		myEvent.SetClientData(new wxKeyEvent(event));
		if(g_poDailog) {
			g_poDailog->ProcessEvent(myEvent);
			if(!myEvent.GetSkipped())
				return 1;
			else
				return CallNextHookEx(g_hDailogKeyBoard, nCode, wParam, lParam);
		}
	}
    return CallNextHookEx(g_hDailogKeyBoard, nCode, wParam, lParam);
}
//���ü��̹��ӣ������µ���
void SetMainHookForDialog(wxWindow* pDialog, BOOL bInit)
{
    if(bInit) {
		g_poDailog = pDialog;
        g_hDailogKeyBoard = SetWindowsHookEx(WH_KEYBOARD, 
				DialogKeyProc, NULL, GetCurrentThreadId());
    }
    else {
		g_poDailog = NULL;
        UnhookWindowsHookEx(g_hDailogKeyBoard);
    }

}