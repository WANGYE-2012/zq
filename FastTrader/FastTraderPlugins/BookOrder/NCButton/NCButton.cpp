// NCButton.cpp: implementation of the CNCButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void TransparentBlt2( HDC hdcDest,      // Ŀ��DC
					 int nXOriginDest,   // Ŀ��Xƫ��
					 int nYOriginDest,   // Ŀ��Yƫ��
					 int nWidthDest,     // Ŀ����
					 int nHeightDest,    // Ŀ��߶�
					 HDC hdcSrc,         // ԴDC
					 int nXOriginSrc,    // ԴX���
					 int nYOriginSrc,    // ԴY���
					 int nWidthSrc,      // Դ���
					 int nHeightSrc,     // Դ�߶�
					 UINT crTransparent  // ͸��ɫ,COLORREF����
					 )
{
	HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);	// ��������λͼ
	HBITMAP hOldMaskBMP, hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);			// ������ɫ����λͼ
	HDC		hImageDC = CreateCompatibleDC(hdcDest);
	HDC		hMaskDC = CreateCompatibleDC(hdcDest);
	hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
	hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);
	
	// ��ԴDC�е�λͼ��������ʱDC��
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
	else
		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, 
		hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);
	
	// ����͸��ɫ
	SetBkColor(hImageDC, crTransparent);
	
	// ����͸������Ϊ��ɫ����������Ϊ��ɫ������λͼ
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);
	
	// ����͸������Ϊ��ɫ���������򱣳ֲ����λͼ
	SetBkColor(hImageDC, RGB(0,0,0));
	SetTextColor(hImageDC, RGB(255,255,255));
	BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
	
	// ͸�����ֱ�����Ļ���䣬�������ֱ�ɺ�ɫ
	SetBkColor(hdcDest,RGB(255,255,255));
	SetTextColor(hdcDest,RGB(0,0,0));
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
	
	// "��"����,��������Ч��
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);
	
	// �����ָ�	
	SelectObject(hImageDC, hOldImageBMP);
	DeleteDC(hImageDC);
	SelectObject(hMaskDC, hOldMaskBMP);
	DeleteDC(hMaskDC);
	DeleteObject(hImageBMP);
	DeleteObject(hMaskBMP);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNCButton::CNCButton()
{
	m_bCreated = FALSE;
	m_hParentWnd = NULL;
	m_hTooltip = NULL;
	m_strText = "";
	m_nAlign = ALIGN_RIGHT | ALIGN_TOP;
	m_nState = BTNSTATE_NORMAL;
	m_nBmpNormal = 0;
	m_nBmpHover = 0;
	m_nBmpDown = 0;
	m_hFont = NULL;
}

CNCButton::~CNCButton()
{
	if(m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}	
}

//------------------------------------------------
//	���ư�ť
//------------------------------------------------

void CNCButton::OnPaint()
{
	//CCriticalSection cs;
	//cs.Lock();

	HDC hdc = GetWindowDC(m_hParentWnd);

	POINT posOrg;
	SetViewportOrgEx(hdc, m_Rect.left, m_Rect.top, &posOrg);

	// ���ݲ�ͬ��״̬����ť
	if((m_nState & BTNSTATE_MOUSEOVER) == BTNSTATE_MOUSEOVER)
	{
		if((m_nState & BTNSTATE_LBUTTONDOWN) == BTNSTATE_LBUTTONDOWN)
		{
			OnPaintDown(hdc);
		}
		else
		{
			OnPaintHover(hdc);
		}
	}
	else
	{
		OnPaintNormal(hdc);
	}
	SetViewportOrgEx(hdc, posOrg.x, posOrg.y, NULL);
	ReleaseDC(m_hParentWnd, hdc);

}

//----------------------------------------------------------------
//	������ť
//	ptOffsetΪX�����λ����Y�����λ�ã������λ������뷽ʽ�й�
//	sizeBtn�ǰ�ť�Ŀ����߶�
//----------------------------------------------------------------

BOOL CNCButton::Create(LPCTSTR lpszText, HWND hParentWnd, POINT& ptOffset, SIZE& sizeBtn, UINT nID)
{
	if(m_bCreated)
		return FALSE;

	m_hParentWnd = hParentWnd;
	m_strText = lpszText;
	m_ptOffset = ptOffset;
	m_sizeBtn = sizeBtn;
	m_nID = nID;

	if(m_hParentWnd)
	{	
		if(strlen(lpszText) > 0)
		{
			// ����Ĭ������
			LOGFONT logFont;
			ZeroMemory((void*)&logFont, sizeof(logFont));
			strcpy(logFont.lfFaceName, "����");
			logFont.lfHeight = -12;
			logFont.lfWeight = 400;
			logFont.lfCharSet = GB2312_CHARSET;
			logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			logFont.lfClipPrecision = OUT_DEFAULT_PRECIS;
			logFont.lfQuality = PROOF_QUALITY;
			logFont.lfPitchAndFamily = VARIABLE_PITCH|FF_ROMAN;
			m_hFont = CreateFontIndirect(&logFont);
		}

		RECT rectParent;
		GetWindowRect(hParentWnd, &rectParent);
		OnParentSize(0, &rectParent);
		m_bCreated = TRUE;
	}
	else
	{
		m_bCreated = FALSE;
	}

	return m_bCreated;
}

//-----------------------------------------------------
//	�ı��Сʱ����
//	hParentWndΪ������, lpRectΪ�����ڴ�С
//-----------------------------------------------------

void CNCButton::OnParentSize(UINT nSide, LPRECT lpRect)
{
	RECT rectParent;

	rectParent.left = 0;
	rectParent.top = 0;
	rectParent.right = lpRect->right - lpRect->left;
	rectParent.bottom = lpRect->bottom - lpRect->top;

	if((m_nAlign & ALIGN_RIGHT) == ALIGN_RIGHT)
	{
		m_Rect.right = rectParent.right - m_ptOffset.x;
		m_Rect.left = m_Rect.right - m_sizeBtn.cx;
	}
	else
	{
		if((m_nAlign & ALIGN_LEFT) == ALIGN_LEFT)
		{
			m_Rect.left = rectParent.left + m_ptOffset.x;
			m_Rect.right = m_Rect.left + m_sizeBtn.cx;
		}
	}
	if((m_nAlign & ALIGN_BOTTOM) == ALIGN_BOTTOM)
	{
		m_Rect.bottom = rectParent.bottom - m_ptOffset.y;
		m_Rect.top = m_Rect.bottom - m_sizeBtn.cy;
	}
	else
	{
		if((m_nAlign & ALIGN_TOP) == ALIGN_TOP)
		{
			m_Rect.top = rectParent.top + m_ptOffset.y;
			m_Rect.bottom = m_Rect.top + m_sizeBtn.cy;
		}
	}
}

//------------------------------------------------
//	���ö��뷽ʽ
//------------------------------------------------

void CNCButton::SetAlign(UINT nAlign)
{
	m_nAlign = nAlign;
}

//------------------------------------------------
//	WM_HITTEST��Ϣ����
//------------------------------------------------

BOOL CNCButton::OnHitTest(POINT point)
{
	//if(m_Rect.left == 272)
//	TRACE("%d,%d,%d,%d;%d,%d;()%d\r\n", m_Rect.left,m_Rect.top,m_Rect.right,m_Rect.bottom,
//		point.x,point.y,PtInRect(&m_Rect, point));
	
	if(PtInRect(&m_Rect, point))
	{
		m_nState |= BTNSTATE_MOUSEOVER;
		return TRUE;
	}
	else
	{
		m_nState &= ~BTNSTATE_MOUSEOVER;
		//TRACE("Hittest ���:FALSE\r\n");
		return FALSE;
	}
}

//---------------------------------------------
//	����ƶ�����Ϣ����
//---------------------------------------------

void CNCButton::OnMouseMove(POINT point)
{
	SetState(GetState() | BTNSTATE_MOUSEOVER);
	if(GetCapture() != m_hParentWnd)
	{
		SetCapture(m_hParentWnd);

		OnPaint();
		OnSetCursor();
		ShowTooltip(TRUE);
	}
}

//---------------------------------------------
//	������µ���Ϣ����
//---------------------------------------------

void CNCButton::OnMouseLButtonDown()
{
	SetState(GetState() | BTNSTATE_LBUTTONDOWN);
	SetCapture(m_hParentWnd);
	ShowTooltip(FALSE);
}

//---------------------------------------------
//	����ͷŵ���Ϣ����
//---------------------------------------------

void CNCButton::OnMouseLButtonUp()
{
	SetState(GetState() & ~BTNSTATE_LBUTTONDOWN);
	if((GetState() & BTNSTATE_MOUSEOVER) != BTNSTATE_MOUSEOVER)
		ReleaseCapture();
}

//---------------------------------------------
//	����뿪��ť֮��
//---------------------------------------------

void CNCButton::OnMouseLeave()
{
//	TRACE("MouseLeave\r\n");
	if((GetState() & BTNSTATE_LBUTTONDOWN) != BTNSTATE_LBUTTONDOWN)
		ReleaseCapture();
	ShowTooltip(FALSE);
}

void CNCButton::OnMouseClick()
{
	//MessageBox(m_hParentWnd, "Message", "VCKBASE", MB_OK);
}

//------------------------------------------------
// ��3D����
//------------------------------------------------

void CNCButton::Draw3dRect(HDC hdc, LPRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	int nCX = lpRect->right - lpRect->left;
	int nCY = lpRect->bottom - lpRect->top;
	FillSolidRect(hdc, lpRect->left, lpRect->top, nCX - 1, 1, clrTopLeft);
	FillSolidRect(hdc, lpRect->left, lpRect->top, 1, nCY - 1, clrTopLeft);
	FillSolidRect(hdc, lpRect->left + nCX, lpRect->top, -1, nCY, clrBottomRight);
	FillSolidRect(hdc, lpRect->left, lpRect->top + nCY, nCX, -1, clrBottomRight);
}

//-------------------------------------------------
//	��䷽��
//-------------------------------------------------

void CNCButton::FillSolidRect(HDC hdc, int x, int y, int cx, int cy, COLORREF clr)
{
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + cx;
	rect.bottom = y + cy;

	SetBkColor(hdc, clr);
	ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
}

//------------------------------------------
//	����״̬��ť
//------------------------------------------

void CNCButton::OnPaintNormal(HDC hdc)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = m_sizeBtn.cx;
	rect.bottom = m_sizeBtn.cy;
	Draw3dRect(hdc, &rect, GetSysColor(COLOR_3DHIGHLIGHT), GetSysColor(COLOR_3DDKSHADOW));
	
	rect.left++;
	rect.top++;
	rect.right--;
	rect.bottom--;
	Draw3dRect(hdc, &rect, GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNSHADOW));
	FillSolidRect(hdc, rect.left + 1, rect.top + 1, rect.right - 3, rect.bottom - 3, GetSysColor(COLOR_BTNFACE));

	// ��λͼ
	UINT nBmpID = m_nBmpNormal;
	if(nBmpID != 0)
		DrawTransparentBmp(hdc, 2, 2, rect.right - 3, rect.bottom - 3, nBmpID, RGB(0xD8, 0xD4, 0xD0));

	// �������
	HFONT hOldFont = NULL;
	if(m_hFont)
		hOldFont = (HFONT)SelectObject(hdc, m_hFont);

	SetBkMode(hdc, TRANSPARENT);
	//SetBkColor(hdc, RGB(0xff, 0x99, 0));
	DrawText(hdc, m_strText.c_str(), m_strText.size(), &rect, DT_CENTER | DT_VCENTER);

	if(hOldFont)
		SelectObject(hdc, hOldFont);
}

//------------------------------------------
//	���������ʱ�İ�ť
//------------------------------------------

void CNCButton::OnPaintHover(HDC hdc)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = m_sizeBtn.cx;
	rect.bottom = m_sizeBtn.cy;
	Draw3dRect(hdc, &rect, GetSysColor(COLOR_3DHIGHLIGHT), GetSysColor(COLOR_3DDKSHADOW));
	
	rect.left++;
	rect.top++;
	rect.right--;
	rect.bottom--;
	Draw3dRect(hdc, &rect, GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNSHADOW));
	
	FillSolidRect(hdc, rect.left + 1, rect.top + 1, rect.right - 3, rect.bottom - 3, GetSysColor(COLOR_BTNFACE));
	
	// ��λͼ
	UINT nBmpID;
	if(m_nBmpHover != 0)
		nBmpID = m_nBmpHover;
	else
		nBmpID = m_nBmpNormal;

	if(nBmpID != 0)
		DrawTransparentBmp(hdc, 2, 2, rect.right - 3, rect.bottom - 3, nBmpID, RGB(0xD8, 0xD4, 0xD0));
		//DrawTransparentBmp(hdc, 2, 1, 10, 10, nBmpID, RGB(0xD4, 0xD0, 0xC8));

	// �������
	HFONT hOldFont = NULL;
	if(m_hFont)
		hOldFont = (HFONT)SelectObject(hdc, m_hFont);
	
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0xf0));
	DrawText(hdc, m_strText.c_str(), m_strText.size(), &rect, DT_CENTER | DT_VCENTER);
	
	if(hOldFont)
		SelectObject(hdc, hOldFont);
}

//------------------------------------------
//	��갴��ʱ�İ�ť
//------------------------------------------

void CNCButton::OnPaintDown(HDC hdc)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = m_sizeBtn.cx;
	rect.bottom = m_sizeBtn.cy;
	Draw3dRect(hdc, &rect, GetSysColor(COLOR_3DDKSHADOW), GetSysColor(COLOR_3DHIGHLIGHT));
	
	rect.left++;
	rect.top++;
	rect.right--;
	rect.bottom--;
	Draw3dRect(hdc, &rect, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNFACE));
	
	FillSolidRect(hdc, rect.left + 1, rect.top + 1, rect.right - 3, rect.bottom - 3, GetSysColor(COLOR_BTNFACE));	

	// ��λͼ
	UINT nBmpID;
	if(m_nBmpDown != 0)
		nBmpID = m_nBmpDown;
	else
		nBmpID = m_nBmpHover;

	if(nBmpID != 0)
		DrawTransparentBmp(hdc, 3, 2, rect.right - 3, rect.bottom - 3, nBmpID, RGB(0xD8, 0xD4, 0xD0));

	// �������
	HFONT hOldFont = NULL;
	if(m_hFont)
		hOldFont = (HFONT)SelectObject(hdc, m_hFont);
	
	rect.left += 2;
	rect.top += 2;
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0xf0));
	DrawText(hdc, m_strText.c_str(), m_strText.size(), &rect, DT_CENTER | DT_VCENTER);
	
	if(hOldFont)
		SelectObject(hdc, hOldFont);
}

//--------------------------------------------------------
//	��͸��λͼ
//--------------------------------------------------------

void CNCButton::DrawTransparentBmp(HDC hdc, int nXOriginDest, int nYOriginDest, int nWidth, int nHeight, WORD wBmpID, COLORREF crTransparent)
{
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(m_hParentWnd, GWL_HINSTANCE);
	HBITMAP hBmp = LoadBitmap(hInstance, MAKEINTRESOURCE(wBmpID));
	HDC hTempDC = CreateCompatibleDC(hdc);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hTempDC, hBmp);
	
	TransparentBlt2(hdc, nXOriginDest, nYOriginDest, nWidth, nHeight, hTempDC, 0, 0, nWidth, nHeight, crTransparent);
	
	if(hOldBmp)
		SelectObject(hTempDC, hOldBmp);
	
	DeleteDC(hTempDC);
	DeleteObject(hBmp);
}

//------------------------------------------------------
//	���ð�ť�ڸ���״̬�µ�λͼ
//------------------------------------------------------

void CNCButton::SetButtonBitmap(UINT nBmpID, UINT nCase)
{
	switch(nCase)
	{
	case BTNBMP_NORMAL:
		m_nBmpNormal = nBmpID;
		break;
	case BTNBMP_MOUSEOVER:
		m_nBmpHover = nBmpID;
		break;
	case BTNBMP_LBUTTONDOWN:
		m_nBmpDown = nBmpID;
		break;
	}
}

//---------------------------------------------------------
//	���ð�ť����
//---------------------------------------------------------

void CNCButton::SetText(LPCTSTR lpszText, LPCTSTR lpszFont, int nSize, BYTE lfCharSet /* = GB2312_CHARSET*/)
{
	if(m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}

	// ��������
	LOGFONT logFont;
	ZeroMemory((void*)&logFont, sizeof(logFont));
	strcpy(logFont.lfFaceName, lpszFont);
	logFont.lfHeight = -nSize;
	logFont.lfWeight = 400;
	logFont.lfCharSet = lfCharSet;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfClipPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfQuality = PROOF_QUALITY;
	logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	m_hFont = CreateFontIndirect(&logFont);

	m_strText = lpszText;
}

void CNCButton::ShowTooltip(BOOL bShow)
{	
	if(m_strTooltip.size() <= 0)
		return;

	if(m_hTooltip == NULL)
	{
		HINSTANCE hInstance = (HINSTANCE)GetWindowLong(m_hParentWnd, GWL_HINSTANCE);			
		m_hTooltip = CreateWindowEx(
			WS_EX_TOPMOST,
			"Tooltips_class32",
			NULL,
			WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,		
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			m_hParentWnd,
			NULL,
			hInstance,
			NULL
			);
	}

	TOOLINFO ti;
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_TRACK;
	ti.hwnd = NULL;
	ti.hinst = NULL;
	ti.uId = (UINT)m_hTooltip;
	ti.lpszText = (char *)m_strTooltip.c_str();

	if(bShow == TRUE)
	{
		RECT rect;
		GetWindowRect(m_hParentWnd, &rect);
		POINT point;
		GetCursorPos(&point);
		
		::SendMessage(m_hTooltip, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
		::SendMessage(m_hTooltip, TTM_TRACKPOSITION, 0, (LPARAM)(DWORD) MAKELONG(point.x + 16, point.y + 16));
		::SendMessage(m_hTooltip, TTM_TRACKACTIVATE, TRUE, (LPARAM)(LPTOOLINFO) &ti);
	}
	else
		::SendMessage(m_hTooltip, TTM_TRACKACTIVATE, FALSE, (LPARAM)(LPTOOLINFO) &ti);
		
/*
		}
		else
		{
			SendMessage(m_hTooltip, TTM_UPDATETIPTEXT, 0, (LPARAM) (LPTOOLINFO) &ti);
			
			SendMessage(m_hTooltip, TTM_UPDATE, 0, (LPARAM) (LPTOOLINFO) &ti);
		}*/
	

}

//-------------------------------------------------
//	���ù�����ʾ
//-------------------------------------------------

void CNCButton::SetTooltip(LPCTSTR lpszText)
{
	m_strTooltip = lpszText;
}

//-------------------------------------------------
//	���ù��
//-------------------------------------------------

void CNCButton::OnSetCursor()
{
	SetCursor(LoadCursor(NULL, IDC_ARROW));	
}
