#include "StdAfx.h"
#include "ColorButton.h"

IMPLEMENT_DYNAMIC(CColorButton, CButton) 

CColorButton::CColorButton(void)
{
	m_fg = RGB(1, 1, 1);
	m_bg = RGB(192, 123, 192);
	m_bPressDown = FALSE;
}

CColorButton::~CColorButton(void)
{
}

//����Attach�������� 
BOOL CColorButton::Attach(const UINT nID, CWnd* pParent, const COLORREF BGColor, const COLORREF FGColor) 
{ 
	if(!SubclassDlgItem(nID, pParent)) 
		return FALSE; 
	m_fg = FGColor; 
	m_bg = BGColor; 
	return TRUE; 
} 

BOOL CColorButton::PreTranslateMessage(MSG* pMsg)
{
	BOOL bResult = FALSE;
	CRect rect;
	CPoint pos;

	switch(pMsg->message)
	{
		case WM_MOUSEMOVE:
			{
				::GetCursorPos(&pos);
				GetWindowRect(&rect);
				if(!rect.PtInRect(pos) && m_bPressDown)
				{
					m_bPressDown = FALSE;
					Invalidate();
					bResult = TRUE;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				::GetCursorPos(&pos);
				GetWindowRect(&rect);
				if(rect.PtInRect(pos))
				{
					m_bPressDown = TRUE;
					Invalidate();
					bResult = TRUE;
				}
			}
			break;
		case WM_LBUTTONUP:
			{
				::GetCursorPos(&pos);
				GetWindowRect(&rect);
				if(rect.PtInRect(pos))
				{
					m_bPressDown = FALSE;
					Invalidate();
					bResult = TRUE;
				}
			}
			break;
	}
	
	return __super::PreTranslateMessage(pMsg);
;
}

//����DrawItem���� 
void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{ 
	CDC* pDC = CDC::FromHandle(lpDIS->hDC); 
	UINT state = lpDIS->itemState; 
	CRect focusRect, btnRect; 
	focusRect.CopyRect(&lpDIS->rcItem); //��Ŧ��ѡ�����߿� 
	btnRect.CopyRect(&lpDIS->rcItem); // ���ñ�ʾ��Ŧ��ѡ�е����߿� 
	focusRect.left += 4; 
	focusRect.right -= 4; 
	focusRect.top += 4; 
	focusRect.bottom -= 4; 
	// ��Ŧ���� 
	const int bufSize = 512; 
	TCHAR buffer[bufSize]; 
	GetWindowText(buffer, bufSize); 
	// ���Ʋ���־��Ŧ 
	DrawFilledRect(pDC, btnRect, GetBGColor()); 
	if(m_bPressDown)
		DrawFrameSunken(pDC, btnRect); 
	else
		DrawFrame(pDC, btnRect); 
	DrawButtonText(pDC, btnRect, buffer, GetFGColor()); 
	// �����Ŧ����ѡ��״̬�������ϻ���ѡ�����߿� 
	if (state & ODS_FOCUS) { 
		DrawFocusRect(lpDIS->hDC, (LPRECT)&focusRect); 
	} 
} 

void CColorButton::DrawFrame(CDC *DC, CRect R) 
{ 
	//���ư�Ŧ���û�ͨ�����Ƹú�����ʵ�ֲ�ͬ��״�İ�Ŧ�� 
	DrawLine(DC, R.left, R.top, R.right, R.top, RGB(255, 255, 255)); 
	DrawLine(DC, R.left, R.top, R.left, R.bottom, RGB(255, 255, 255)); 
	//���»��ư�Ŧ����Χ������ʹ��Ŧ������� 
	DrawLine(DC, R.left + 1, R.bottom - 1, R.right, R.bottom - 1, RGB(1, 1, 1)); 
	//���ư�Ŧ����ߺ��Ͽ��� 
	DrawLine(DC, R.right - 1, R.top + 1, R.right - 1, R.bottom, RGB(1, 1, 1)); 
	//���ư�Ŧ�ҿ��ߺ��¿��� 
} 

void CColorButton::DrawFrameSunken(CDC *DC, CRect R) 
{ 
	//���ư�Ŧ���û�ͨ�����Ƹú�����ʵ�ֲ�ͬ��״�İ�Ŧ�� 
	DrawLine(DC, R.left, R.top, R.right, R.top, RGB(1, 1, 1)); 
	DrawLine(DC, R.left, R.top, R.left, R.bottom, RGB(1, 1, 1)); 
	//���»��ư�Ŧ����Χ������ʹ��Ŧ������� 
	DrawLine(DC, R.left + 1, R.bottom - 1, R.right, R.bottom - 1, RGB(255, 255, 255)); 
	//���ư�Ŧ����ߺ��Ͽ��� 
	DrawLine(DC, R.right - 1, R.top + 1, R.right - 1, R.bottom, RGB(255, 255, 255)); 
	//���ư�Ŧ�ҿ��ߺ��¿��� 
} 

//��ɫ����䰴Ŧ�� 
void CColorButton::DrawFilledRect(CDC *DC, CRect R, COLORREF color) 
{ 
	CBrush B; 
	B.CreateSolidBrush(color); 
	DC->FillRect(R, &B); 
} 

// DrawLine���ڻ��ư�Ŧ����Ϊ��̬���� 
void CColorButton::DrawLine(CDC *DC, CRect EndPoints, COLORREF color)
{
} 

void CColorButton::DrawLine(CDC *DC, long left, long top, long right, long bottom, COLORREF color) 
{
	CPen penCurr;

	penCurr.CreatePen(PS_SOLID, 1, color);
	CPen* pOldPen = pOldPen = DC->SelectObject(&penCurr);
	DC->MoveTo(left, top);
	DC->LineTo(right, bottom);
	DC->SelectObject(pOldPen);
	penCurr.DeleteObject();
} 

//���ư�Ŧ�ı� 
void CColorButton::DrawButtonText(CDC *DC, CRect R, const char *Buf, COLORREF TextColor) 
{ 
	COLORREF prevColor = DC->SetTextColor(TextColor); 
	DC->SetBkMode(TRANSPARENT); 
	DC->DrawText(Buf, strlen(Buf), R, DT_CENTER|DT_VCENTER|DT_SINGLELINE); 
	DC->SetTextColor(prevColor); 
} 
