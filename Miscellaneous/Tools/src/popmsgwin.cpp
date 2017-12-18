//********************************************************************************************
//	
//	FileName	:popmsg.cpp
//	Target		:���嵯����Ϣ������
//	Description	:
//				 1. ���ഴ��һ���̣߳��߳��д���һ������edit���ڣ�Ӧ�ó��������ô��ڷ��͵�����Ϣ����ʾ�ڴ�����
//				 2. �����Ӧ����Win32������
//	Version		:1.0
//	Date		:2008-4-30
//	Author		:lzq
//				 Copyright (C), 2008-2028
//	
//	History		:
//
//********************************************************************************************

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#ifdef WIN32
#pragma unmanaged
#endif


#include "popmsgwin.h"
#include "FileOpr.h"

#define ID_EditWnd	10001
#define ID_OK		10002
#define ID_Cancel	10003
#define BTNWID		100
#define BTNHEI		24
#define EDITBORDER	4

#define FontHei		12

LRESULT WINAPI WndProc_PopMsgWin(HWND  hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);


//	���캯��,����ᴴ��һ���߳����ڴ�������Ϣ
CPopMsgWin::CPopMsgWin(const Stru_Param& Param)
{
    m_Param=Param;

    m_hWnd=NULL;
	m_hEdit=NULL;
	m_hThread=NULL;
	m_hWinSta_Visible=0;
	m_hWinSta_Ori=0;
	m_hDesktop_Ori=0;
	m_hDesktop_Visible=0;
	m_hFont=NULL;
	m_hBkBrush=NULL;

	m_hFont=(int)CreateFont(-1*FontHei,0,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,0,"����");		//"Arial"
	m_hBkBrush=(int)CreateSolidBrush(m_Param.BackgroundColor);

    m_bClickOKBtn=false;

	WNDCLASSEX wc;
	ZeroMemory(&wc,sizeof(wc));
	wc.cbSize			=sizeof(wc);
	wc.style			=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc		=WndProc_PopMsgWin;
	wc.cbClsExtra		=0;
	wc.cbWndExtra		=24;		//0:pObj; 8:�����Ƿ���ʾ������bVScrollVisible; 12:TextColor; 16:BackgroundColor; 20:bClearAsClose
	wc.hInstance		=GetModuleHandle(NULL); 
	wc.hCursor			=LoadCursor(NULL,IDC_ARROW);
	wc.hIcon			=LoadIcon(NULL,IDI_ASTERISK);
	wc.hbrBackground	=(HBRUSH)m_hBkBrush;
	wc.lpszMenuName		=NULL;
	wc.lpszClassName	="PopMsgWinClass"; 
	RegisterClassEx(&wc);

	m_hThread=(int)CreateThread(NULL,65536,(LPTHREAD_START_ROUTINE)WorkerThread,this,0,&m_hThreadID);
    CFileOpr::getObj().writelocallog("threadid","TID:\t% 8u\tCPopMsgWin::ThreadFunc",m_hThreadID);
}


//	��������
CPopMsgWin::~CPopMsgWin()
{
    CeasymutexGuard guard(m_mutex);

	DWORD			ExitCode;
	if(m_hThread&&m_hWnd&&IsWindow((HWND)m_hWnd))
	{
		//Post WM_QUIT��GetMessage�˳�
		PostMessage((HWND)m_hWnd,WM_QUIT,0,0);
	}
	if(m_hThread)
	{
		WaitForSingleObject((HANDLE)m_hThread,400000);
		if(GetExitCodeThread((HANDLE)m_hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)m_hThread,0);
	}
	if(m_hFont)
	{
		DeleteObject((HFONT)m_hFont);
		m_hFont=NULL;
	}
	if(m_hBkBrush)
	{
		DeleteObject((HBRUSH)m_hBkBrush);
		m_hBkBrush=NULL;
	}
	UnregisterClass("PopMsgWinClass",GetModuleHandle(NULL));

	if(m_Param.hWnd_PostCloseMsg&&IsWindow((HWND)m_Param.hWnd_PostCloseMsg))
		PostMessage((HWND)m_Param.hWnd_PostCloseMsg,WM_COMMAND,m_Param.wParam_PostCloseMsg,m_bClickOKBtn?1:0);
}

void CPopMsgWin::Show(void)
{
	ShowWindow((HWND)m_hWnd,SW_SHOW);
}
void CPopMsgWin::Hide(void)
{
	ShowWindow((HWND)m_hWnd,SW_HIDE);
}
void CPopMsgWin::Clear(void)
{
    CeasymutexGuard guard(m_mutex);

	if(m_hWnd&&m_hEdit)
	{
		SetWindowText((HWND)m_hEdit,"");
		ShowScrollBar((HWND)m_hEdit,SB_VERT,FALSE);
		SetWindowLong((HWND)m_hWnd,8,0);
	}
}
//	��һ���ַ�����ӵ�������
//	bAddCRLF: ��ӻس����е���β
//	bInsertTime: ���ʱ�䵽����
//	DelOldContent: �Ƿ�ɾ��ԭ��������
////	Mode=0:��ģʽ��ÿ���Զ������־ʱ�����β�س�
////	Mode=1:��ģʽ�����Զ���Ӷ���
void CPopMsgWin::AppendStr(eForceShow bForceShow,eDelOldContent bDelOldContent,eAddCRLF bAddCRLF,eInsertTime bInsertTime,const char *Str)
{
    CeasymutexGuard guard(m_mutex);

    AppendStr_Internal(bForceShow,bDelOldContent,bAddCRLF,bInsertTime,Str);
}
void CPopMsgWin::AppendStr_Internal(eForceShow bForceShow,eDelOldContent bDelOldContent,eAddCRLF bAddCRLF,eInsertTime bInsertTime,const char *Str)
{
	SYSTEMTIME			CurTime;
	int len;

	if(!m_hWnd||!m_hEdit||!Str) return;

	if(bDelOldContent==DelOldContent) 
	{
		if(m_hWnd)
		{
			SetWindowText((HWND)m_hEdit,"");
			ShowScrollBar((HWND)m_hEdit,SB_VERT,FALSE);
			SetWindowLong((HWND)m_hWnd,8,0);
		}
		else m_ContentBeforeWndCreated.clear();
	}
	else
	{
		if(m_hWnd&&m_Param.MaxMsgLen>0)
		{
			//���ݳ������õ�ǰһ��
			len=(int)SendMessage((HWND)m_hEdit,WM_GETTEXTLENGTH,0,0);
			if(len>m_Param.MaxMsgLen-(int)strlen(Str)-20)							
			{											
				SendMessage((HWND)m_hEdit,EM_SETSEL,0,len/2);
				SendMessage((HWND)m_hEdit,EM_REPLACESEL,FALSE,(LPARAM)(LPCSTR)" ");
			}
		}
	}

	if(bInsertTime==InsertTime)
	{
		GetLocalTime(&CurTime);
		char buf[256];
		_snprintf(buf,sizeof(buf)-1,"%02d:%02d:%02d.%03d ",CurTime.wHour,CurTime.wMinute,CurTime.wSecond,CurTime.wMilliseconds);
		if(m_hWnd)
		{
			len=(int)SendMessage((HWND)m_hEdit,WM_GETTEXTLENGTH,0,0);
			SendMessage((HWND)m_hEdit,EM_SETSEL,len,len);
			SendMessage((HWND)m_hEdit,EM_REPLACESEL,FALSE,(LPARAM)(LPCSTR)buf);
		}
		else m_ContentBeforeWndCreated=string(buf)+m_ContentBeforeWndCreated;
	}

	if(m_hWnd)
	{
		len=(int)SendMessage((HWND)m_hEdit,WM_GETTEXTLENGTH,0,0);
		SendMessage((HWND)m_hEdit,EM_SETSEL,len,len);
		SendMessage((HWND)m_hEdit,EM_REPLACESEL,FALSE,(LPARAM)(LPCSTR)Str);
	}

	if(bAddCRLF==AddCRLF)
	{
		if(m_hWnd)
		{
			len=(int)SendMessage((HWND)m_hEdit,WM_GETTEXTLENGTH,0,0);
			SendMessage((HWND)m_hEdit,EM_SETSEL,len,len);
			SendMessage((HWND)m_hEdit,EM_REPLACESEL,FALSE,(LPARAM)(LPCSTR)"\r\n");
		}
		else m_ContentBeforeWndCreated=m_ContentBeforeWndCreated+string("\r\n");
	}

	if(m_hWnd)
	{
		SendMessage((HWND)m_hEdit,EM_SCROLLCARET,0,0);

		int FirstVisibleLine=SendMessage((HWND)m_hEdit,EM_GETFIRSTVISIBLELINE,0,0);
		int bVScrollVisible=(int)GetWindowLong((HWND)m_hWnd,8);
		if(FirstVisibleLine!=0&&!bVScrollVisible)
		{
			ShowScrollBar((HWND)m_hEdit,SB_VERT,TRUE);
			SetWindowLong((HWND)m_hWnd,8,1);
		}
		else if(FirstVisibleLine==0&&bVScrollVisible)
		{
			ShowScrollBar((HWND)m_hEdit,SB_VERT,FALSE);
			SetWindowLong((HWND)m_hWnd,8,0);
		}

		if(bForceShow==ForceShow&&m_hWnd&&!IsWindowVisible((HWND)m_hWnd))
			ShowWindow((HWND)m_hWnd,SW_SHOW);
	}
}
void CPopMsgWin::AppendStr2(eForceShow bForceShow,eDelOldContent bDelOldContent,eAddCRLF bAddCRLF,eInsertTime bInsertTime,const char* fmt,...)
{
	va_list ap;
	char buf[1024];

	buf[sizeof(buf)-1]=0;
	va_start(ap,fmt);
	vsnprintf(buf,sizeof(buf)-1,fmt,ap);
	va_end(ap);

    CeasymutexGuard guard(m_mutex);

	AppendStr_Internal(bForceShow,bDelOldContent,bAddCRLF,bInsertTime,buf);
}
void CPopMsgWin::AppendStr3(const char *pStr)
{
    AppendStr(m_Param.bForceShow,m_Param.bDelOldContent,m_Param.bAddCRLF,m_Param.bInsertTime,pStr);
}
void CPopMsgWin::AppendStr4(const char* fmt,...)
{
	va_list ap;
	char buf[1024];

	buf[sizeof(buf)-1]=0;
	va_start(ap,fmt);
	vsnprintf(buf,sizeof(buf)-1,fmt,ap);
	va_end(ap);

    CeasymutexGuard guard(m_mutex);

	AppendStr_Internal(m_Param.bForceShow,m_Param.bDelOldContent,m_Param.bAddCRLF,m_Param.bInsertTime,buf);
}

//	���ô��ڱ�������
void CPopMsgWin::SetTitle(const char *pStr)
{
    CeasymutexGuard guard(m_mutex);

	if(m_hWnd) SetWindowText((HWND)m_hWnd,pStr);
	else m_TitleBeforeWndCreated=string(pStr);
}

//  �ȴ����ڴ����ɹ�
bool CPopMsgWin::WaitWndCreate(int TimeoutMS)
{
    DWORD StartMS=GetTickCount();
    while(m_hWnd==NULL||m_hEdit==NULL)
    {
        Sleep(100);
        if(TimeoutMS>0&&(int)(GetTickCount()-StartMS)>=TimeoutMS)
            return false;
    }
    return true;
}

//	��������
void CPopMsgWin::CreateWindow_Internal(void)
{
	HANDLE			hInst;

	hInst=GetModuleHandle(NULL);

    unsigned int dwExStyle;
    unsigned int dwStyle;

    dwExStyle=WS_EX_CLIENTEDGE|(m_Param.bToolWindow?WS_EX_TOOLWINDOW:0)|(m_Param.bTopMost?WS_EX_TOPMOST:0);
    dwStyle=WS_CAPTION|WS_THICKFRAME|WS_POPUP|WS_BORDER;
    if(m_Param.bInitVisible) dwStyle|=WS_VISIBLE;
    if(m_Param.bSysMenu) dwStyle|=WS_SYSMENU;
    if(m_Param.bMinMaxBox) dwStyle |= WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

	string strTitle=m_TitleBeforeWndCreated.empty()?string(m_Param.szTitle):m_TitleBeforeWndCreated;
	m_hWnd=(int)CreateWindowEx(
        dwExStyle,
		"PopMsgWinClass",strTitle.c_str(),
        dwStyle,
		m_Param.InitPox,m_Param.InitPoy,m_Param.InitWid,m_Param.InitHei,
		NULL,NULL,(HINSTANCE)hInst,NULL);
	if(m_hWnd==NULL) return;

	//��pObj�ŵ�WindowLong(0)��
	SetWindowLong((HWND)m_hWnd,0,(LONG)this);
	//��m_Param.bClearAsClose���õ�WindowLong(12)��
	SetWindowLong((HWND)m_hWnd,20,(int)m_Param.bClearAsClose);

	RECT Rect;
	GetClientRect((HWND)m_hWnd,&Rect);
	int EditHei=Rect.bottom-Rect.top;

	if(m_Param.bCreateOKBtn||m_Param.bCreateCancelBtn)
	{
		//����ȷ��/ȡ����ť
        int pox=(Rect.right-Rect.left-BTNWID)/2;
        if(m_Param.bCreateOKBtn&&m_Param.bCreateCancelBtn)
            pox=(Rect.right-Rect.left-BTNWID*2-BTNWID/2)/2;

        if(m_Param.bCreateOKBtn)
        {
		    HWND hBtn=CreateWindow("BUTTON",m_Param.szTxt_OKBtn,
						    WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON|BS_TEXT|BS_VCENTER|BS_CENTER|BS_FLAT,
						    pox,Rect.bottom-Rect.top-BTNHEI-BTNHEI/4,BTNWID,BTNHEI,
						    (HWND)m_hWnd,(HMENU)ID_OK,(HINSTANCE)hInst,NULL);
		    SendMessage(hBtn,WM_SETFONT,(WPARAM)m_hFont,0);
            pox+=BTNWID+BTNWID/2;
        }
        if(m_Param.bCreateCancelBtn)
        {
		    HWND hBtn=CreateWindow("BUTTON",m_Param.szTxt_CancelBtn,
						    WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON|BS_TEXT|BS_VCENTER|BS_CENTER|BS_FLAT,
						    pox,Rect.bottom-Rect.top-BTNHEI-BTNHEI/4,BTNWID,BTNHEI,
						    (HWND)m_hWnd,(HMENU)ID_Cancel,(HINSTANCE)hInst,NULL);
		    SendMessage(hBtn,WM_SETFONT,(WPARAM)m_hFont,0);
            pox+=BTNWID+BTNWID/2;
        }
		EditHei-=BTNHEI/4+BTNHEI+BTNHEI/4;
		if(EditHei<0) EditHei=0;
	}

	if(m_Param.bDispMessageBorder)
		m_hEdit=(int)CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",
			WS_CHILD|WS_VISIBLE|WS_VSCROLL|ES_WANTRETURN|ES_MULTILINE|ES_READONLY,
			EDITBORDER,EDITBORDER,Rect.right-Rect.left-EDITBORDER*2,EditHei-EDITBORDER*2,
			(HWND)m_hWnd,(HMENU)ID_EditWnd,(HINSTANCE)hInst,NULL);
	else
		m_hEdit=(int)CreateWindow("EDIT","",
			WS_CHILD|WS_VISIBLE|WS_VSCROLL|ES_WANTRETURN|ES_MULTILINE|ES_READONLY,
			EDITBORDER,EDITBORDER,Rect.right-Rect.left-EDITBORDER*2,EditHei-EDITBORDER*2,
			(HWND)m_hWnd,(HMENU)ID_EditWnd,(HINSTANCE)hInst,NULL);

	HideCaret((HWND)m_hEdit);
	ShowScrollBar((HWND)m_hEdit,SB_VERT,FALSE);
	SetWindowLong((HWND)m_hWnd,8,0);
	SetWindowLong((HWND)m_hWnd,12,m_Param.TextColor);
	SetWindowLong((HWND)m_hWnd,16,m_Param.BackgroundColor);

	SendMessage((HWND)m_hEdit,WM_SETFONT,(WPARAM)m_hFont,0);

	if(m_Param.MaxMsgLen>0)
		SendMessage((HWND)m_hEdit,EM_LIMITTEXT,m_Param.MaxMsgLen,0);

	if(!m_ContentBeforeWndCreated.empty())
		AppendStr_Internal(ForceShow,NoDelOldContent,NoAddCRLF,NoInsertTime,m_ContentBeforeWndCreated.c_str());
}





//	�����߳�
unsigned int CPopMsgWin::WorkerThread(void *arg)						
{
	CPopMsgWin		*pMsgWin;
	MSG				msg;

	pMsgWin=(CPopMsgWin*)arg;
	if(!pMsgWin) return 0xffffffff;

	if(pMsgWin->m_Param.bInServiceID)
		pMsgWin->OpenDisplayInService_Internal();

	pMsgWin->m_mutex.lock();
	pMsgWin->CreateWindow_Internal();
	pMsgWin->m_mutex.unlock();

	//���������bDestroyWhenClose, ������WM_CLOSEʱ�������߳�ǰ��Ҫdelete pobj��
	bool bNeedDeleteObject_if_bDestroyWhenClose=false;

	while(pMsgWin->m_hWnd&&
		IsWindow((HWND)pMsgWin->m_hWnd)&&
		GetMessage(&msg,(HWND)pMsgWin->m_hWnd,0,0))
	{
        //��WndProc_PopMsgWin��û��pMsgWin�����ֻ������������
        if(msg.message==WM_COMMAND&&LOWORD(msg.wParam)==ID_OK)
            pMsgWin->m_bClickOKBtn=true;

		if(msg.message==WM_CLOSE&&pMsgWin->m_Param.bDestroyWhenClose)
		{
			bNeedDeleteObject_if_bDestroyWhenClose=true;
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//����ʱ������WM_QUIT��GetMessage�˳������Ǵ��ڲ�δ���٣��ڴ˴�����
	if(IsWindow((HWND)pMsgWin->m_hWnd))
		DestroyWindow((HWND)pMsgWin->m_hWnd);
	pMsgWin->m_hWnd=NULL;
	pMsgWin->m_hEdit=NULL;

	if(pMsgWin->m_Param.bInServiceID)
		pMsgWin->CloseDisplayInService_Internal();

	pMsgWin->m_hThread=NULL;

	if(bNeedDeleteObject_if_bDestroyWhenClose)
	{
		//����������bDestroyWhenClose, ������WM_CLOSEʱ�������߳�ǰ��Ҫdelete pobj����ʱm_hThread�Ѿ�ΪNULL������������ѭ��
		delete pMsgWin;
	}

	return 0; 
}

//******************************************************************
//  Name:					MainWndProc
//  Description:  �����ڵ���Ϣ�������
//******************************************************************
static LRESULT WINAPI WndProc_PopMsgWin(HWND  hWnd,UINT Msg,WPARAM wParam,LPARAM lParam) 
{
  switch(Msg)
  {
	case WM_SIZE:
	{
		HWND hEdit=GetDlgItem(hWnd,ID_EditWnd);
		if(hEdit)
		{
			//����Edit��С
			RECT Rect;
			GetClientRect(hWnd,&Rect);
			int EditHei=Rect.bottom-Rect.top;

			HWND hBtnOK=GetDlgItem(hWnd,ID_OK);
			if(hBtnOK)
			{
				MoveWindow(hBtnOK,(Rect.right-Rect.left-BTNWID)/2,Rect.bottom-Rect.top-BTNHEI/4-BTNHEI,BTNWID,BTNHEI,TRUE);
				EditHei-=BTNHEI/4+BTNHEI+BTNHEI/4;
				if(EditHei<0) EditHei=0;
			}

			MoveWindow(hEdit,EDITBORDER,EDITBORDER,Rect.right-Rect.left-EDITBORDER*2,EditHei-EDITBORDER*2,TRUE);


			//�ж��Ƿ���Ҫ��ʾ/���ع�����
			bool bVScrollVisible=GetWindowLong(hWnd,8)?true:false;
			if(!bVScrollVisible)
			{
				//�ж��Ƿ���Ҫ��ʾ������
				//ע�⣬���ﲻ����WM_GETTEXTLENGTH���EM_POSFROMCHAR����ΪWM_GETTEXTLENGTH����asc�ַ����ȣ�EM_POSFROMCHARʹ�õ���UniCode����
				int LineCnt=(int)SendMessage(hEdit,EM_GETLINECOUNT,0,0);		//ȡ��������ֵ����>=1��û������Ҳ����1
				int LineIndex=(int)SendMessage(hEdit,EM_LINEINDEX,LineCnt-1,0);	//ȡ���һ�еĵ�һ���ַ���������

				//��ʱ������LineIndex��ȡPosʧ�ܣ���ô����ǰ��
				while(LineIndex>0)
				{
					LRESULT pos=SendMessage(hEdit,EM_POSFROMCHAR,(WPARAM)LineIndex,0);
					if(pos!=-1)
					{
						int poy=HIWORD(pos);
						RECT Rect;
						SendMessage(hEdit,EM_GETRECT,0,(LPARAM)&Rect);
						if(poy+FontHei>=Rect.bottom)
						{
							ShowScrollBar((HWND)hEdit,SB_VERT,TRUE);
							SetWindowLong(hWnd,8,1);
						}
						break;
					}
					else LineIndex--;
				}
			}
			else
			{
				//�ж��Ƿ���Ҫ���ع�����
				SCROLLINFO ScrollInfo;

				memset(&ScrollInfo,0,sizeof(ScrollInfo));
				ScrollInfo.cbSize=sizeof(SCROLLINFO);
				ScrollInfo.fMask=SIF_ALL;
				GetScrollInfo(hEdit,SB_VERT,&ScrollInfo);
				if(ScrollInfo.nMin+(int)ScrollInfo.nPage>ScrollInfo.nMax)
				{
					ShowScrollBar((HWND)hEdit,SB_VERT,FALSE);
					SetWindowLong(hWnd,8,0);
				}
			}
		}
		break;
	}
    case WM_CLOSE:	
	{
		//���أ�������
		ShowWindow(hWnd,SW_HIDE);

		int bClearAsClose=GetWindowLong(hWnd,20)==0?false:true;
		if(bClearAsClose)
		{
			HWND hEdit=GetDlgItem(hWnd,ID_EditWnd);
			SetWindowText((HWND)hEdit,"");
			ShowScrollBar((HWND)hEdit,SB_VERT,FALSE);
			SetWindowLong((HWND)hWnd,0,0);
		}
		return 0;
	}
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORSTATIC:
	{
		SetBkColor((HDC)wParam,(COLORREF)GetWindowLong(hWnd,16));
		SetTextColor((HDC)wParam,(COLORREF)GetWindowLong(hWnd,12));
		return (LRESULT)GetClassLong(hWnd,GCL_HBRBACKGROUND);
	}
	case WM_COMMAND:
	{
		switch LOWORD(wParam)
		{
			case ID_OK:
			{
				SendMessage(hWnd,WM_CLOSE,0,0);
				return 0;
			}
		}
		break;
	}
  }
  return(DefWindowProc(hWnd,Msg,wParam,lParam));
}


//	���� �� OpenDisplayInService
//	���� �� ���ڷ�������ʾ��Ϣ�Ļ���
//	���� �� 
//			hWinSta_Visible:���ӵĽ��̹���վ���
//			hWinSta_Ori:ԭʼ�Ľ��̹���վ���
//			hOriDesk:ԭʼ���߳����棬�ָ�ʱʹ��
//	���� �� 1. ����ʱ��hOriDesk������ǰ�ľ��ָ��
//			2. TRUE:�ɹ���FALSE:ʧ��
void CPopMsgWin::OpenDisplayInService_Internal(void)
{
	if(m_hWinSta_Visible) return;

	m_hWinSta_Visible=(int)OpenWindowStation("WinSta0",FALSE,MAXIMUM_ALLOWED); 
	m_hWinSta_Ori=(int)GetProcessWindowStation();
	if(m_hWinSta_Visible&&m_hWinSta_Ori&&m_hWinSta_Visible!=m_hWinSta_Ori)
		SetProcessWindowStation((HWINSTA)m_hWinSta_Visible); 

	m_hDesktop_Visible= (int)OpenDesktop("Default",0,FALSE,MAXIMUM_ALLOWED); 
    m_hDesktop_Ori= (int)GetThreadDesktop(m_hThreadID); 
	if(m_hDesktop_Visible&&m_hDesktop_Ori&&m_hDesktop_Visible!=m_hDesktop_Ori)
		SetThreadDesktop((HDESK)m_hDesktop_Visible); 
}



//	���� �� CloseDisplayInService
//	���� �� �ر��ڷ�������ʾ��Ϣ�Ļ���
//	���� �� 
//			hWinSta_Visible:���ӵĽ��̹���վ���
//			hWinSta_Ori:ԭʼ�Ľ��̹���վ���
//			hOriDesk:ԭʼ���߳����棬�ָ�ʱʹ��
//	���� �� TRUE:�ɹ���FALSE:ʧ��
void CPopMsgWin::CloseDisplayInService_Internal(void)
{
	if(!m_hWinSta_Visible) return;
																				//�л��߳�����
	if(m_hDesktop_Visible&&m_hDesktop_Ori&&m_hDesktop_Visible!=m_hDesktop_Ori)
		SetThreadDesktop((HDESK)m_hDesktop_Ori);
	if(m_hDesktop_Visible&&m_hDesktop_Visible!=m_hDesktop_Ori)
	{
		CloseDesktop((HDESK)m_hDesktop_Visible); 
		m_hDesktop_Visible=NULL;
	}
	m_hDesktop_Ori=NULL;

																				//�л����̹���վ
	if(m_hWinSta_Visible&&m_hWinSta_Ori&&m_hWinSta_Visible!=m_hWinSta_Ori)
	{
		SetProcessWindowStation((HWINSTA)m_hWinSta_Ori);
		m_hWinSta_Ori=NULL;
	}
	if(m_hWinSta_Visible)
	{
		CloseWindowStation((HWINSTA)m_hWinSta_Visible);
		m_hWinSta_Visible=NULL;
	}
}

//���溯������MessageBox���رռ����١����ھ��С�hWnd_PostCloseMsg;���ٶ���ʱ���Ƿ���ָ������Post WM_COMMAND��Ϣ��Ĭ��Ϊ0,��Post��wParam_PostCloseMsg;wParamֵ��lParamΪ�Ƿ�����OK��ť
void CPopMsgWin::CreateMessageBox(const char* content,const char* caption,int wid,int hei,bool btopmost)
{
	Stru_Param Param;
    Param.bTopMost=btopmost;
	Param.bDispMessageBorder=false;
	Param.InitWid=wid;
	Param.InitHei=hei;
	Param.InitPox=(GetSystemMetrics(SM_CXSCREEN)-wid)/2;
	Param.InitPoy=(GetSystemMetrics(SM_CYSCREEN)-hei)/2;
	Param.bInsertTime=NoInsertTime;
	Param.bDestroyWhenClose=true;
	if(caption) strncpy(Param.szTitle,caption,sizeof(Param.szTitle)-1);
	if(content) CPopMsgWin(Param).AppendStr3(content);
}
void CPopMsgWin::CreateMessageBoxOKCancel(const char* content,const char* caption,int hWnd_PostCloseMsg,int wParam_PostCloseMsg,int wid,int hei,bool btopmost)
{
	Stru_Param Param;
	Param.bCreateCancelBtn=true;
    Param.bTopMost=btopmost;
	Param.bDispMessageBorder=false;
	Param.InitWid=wid;
	Param.InitHei=hei;
	Param.InitPox=(GetSystemMetrics(SM_CXSCREEN)-wid)/2;
	Param.InitPoy=(GetSystemMetrics(SM_CYSCREEN)-hei)/2;
	Param.bInsertTime=NoInsertTime;
	Param.bDestroyWhenClose=true;
	Param.hWnd_PostCloseMsg=hWnd_PostCloseMsg;
	Param.wParam_PostCloseMsg=wParam_PostCloseMsg;
	if(caption) strncpy(Param.szTitle,caption,sizeof(Param.szTitle)-1);
	if(content) CPopMsgWin(Param).AppendStr3(content);
}
void CPopMsgWin::CreateMessageBoxYESNO(const char* content,const char* caption,int hWnd_PostCloseMsg,int wParam_PostCloseMsg,int wid,int hei,bool btopmost)
{
	Stru_Param Param;
	Param.bCreateCancelBtn=true;
    Param.bTopMost=btopmost;
	Param.bDispMessageBorder=false;
	Param.InitWid=wid;
	Param.InitHei=hei;
	Param.InitPox=(GetSystemMetrics(SM_CXSCREEN)-wid)/2;
	Param.InitPoy=(GetSystemMetrics(SM_CYSCREEN)-hei)/2;
	Param.bInsertTime=NoInsertTime;
	Param.bDestroyWhenClose=true;
	Param.hWnd_PostCloseMsg=hWnd_PostCloseMsg;
	Param.wParam_PostCloseMsg=wParam_PostCloseMsg;
	strncpy(Param.szTxt_OKBtn,"��",sizeof(Param.szTxt_OKBtn)-1);
	strncpy(Param.szTxt_CancelBtn,"��",sizeof(Param.szTxt_CancelBtn)-1);
	if(caption) strncpy(Param.szTitle,caption,sizeof(Param.szTitle)-1);
	if(content) CPopMsgWin(Param).AppendStr3(content);
}
