// NCButton.h: interface for the CNCButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NCBUTTON_H__F74B28E4_0F45_4E80_B6C4_59FD5968D2AA__INCLUDED_)
#define AFX_NCBUTTON_H__F74B28E4_0F45_4E80_B6C4_59FD5968D2AA__INCLUDED_

using namespace std;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ���뷽ʽ

#define ALIGN_LEFT				0x0001
#define ALIGN_TOP				0x0002
#define ALIGN_RIGHT				0x0004
#define ALIGN_BOTTOM			0x0008

// ��ť״̬
#define BTNSTATE_NORMAL			0x0000		// ����
#define BTNSTATE_MOUSEOVER		0x0001		// ���������
#define BTNSTATE_LBUTTONDOWN	0x0002		// ����������

// ��ťλͼ
#define BTNBMP_NORMAL			0x0000
#define BTNBMP_MOUSEOVER		0x0001
#define BTNBMP_LBUTTONDOWN		0x0002

class CNCButton
{
private:
	HWND	m_hParentWnd;			// �����ھ��
	UINT	m_nID;					// ��ťID
	RECT	m_Rect;					// ��ť����λ��
	SIZE	m_sizeBtn;				// �ߴ�
	POINT	m_ptOffset;				// λ��
	BOOL	m_bCreated;				// �����ɹ��ı�־
	string	m_strText;				// ��ť����
	string  m_strTooltip;			// ������ʾ
	UINT	m_nAlign;				// ���뷽ʽ
	UINT	m_nState;				// ״̬
	UINT	m_nBmpNormal;			// ����ʱ���λͼID
	UINT	m_nBmpHover;			// �ȵ�λͼID
	UINT	m_nBmpDown;				// ����ʱ��λͼID
	HFONT	m_hFont;				// ����
	HWND	m_hTooltip;				// ������ʾ

public:
	void	SetTooltip(LPCTSTR lpszText);
	void	ShowTooltip(BOOL bShow);
	void	SetText(LPCTSTR lpszText, LPCTSTR lpszFont, int nSize, BYTE lfCharSet = GB2312_CHARSET);
	void	SetButtonBitmap(UINT nBmpID, UINT nCase);
	void	DrawTransparentBmp(HDC hdc, int nXOriginDest, int nYOriginDest, int nWidth, int nHeight, WORD wBmpID, COLORREF crTransparent);
	void	FillSolidRect(HDC hdc, int x, int y, int cx, int cy, COLORREF clr);
	void	Draw3dRect(HDC hdc, LPRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight );
	UINT	GetButtonID() { return m_nID; }
	UINT	GetState() { return m_nState; }
	void	SetState(UINT nState) { m_nState = nState;	}
	HWND	GetParentWnd() { return m_hParentWnd; }
	BOOL	OnHitTest(POINT point);
	void	SetAlign(UINT nAlign);
	BOOL	Create(LPCTSTR lpszText, HWND hParentWnd, POINT& ptOffset, SIZE& sizeBtn, UINT nID);
	BOOL	IsCreated() { return m_bCreated; }
	void	OnPaint();

	CNCButton();

	virtual void OnSetCursor();
	virtual void OnParentSize(UINT nSide, LPRECT lpRect );
	virtual void OnPaintDown(HDC hdc);
	virtual void OnPaintHover(HDC hdc);
	virtual void OnPaintNormal(HDC hdc);
	virtual void OnMouseMove(POINT point);
	virtual void OnMouseLButtonDown();
	virtual void OnMouseLButtonUp();
	virtual void OnMouseClick();
	virtual void OnMouseLeave();
	virtual ~CNCButton();

};

#endif // !defined(AFX_NCBUTTON_H__F74B28E4_0F45_4E80_B6C4_59FD5968D2AA__INCLUDED_)
