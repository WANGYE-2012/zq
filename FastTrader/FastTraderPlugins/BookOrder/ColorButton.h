#pragma once
#include "afxwin.h"

class CColorButton : public CButton
{
DECLARE_DYNAMIC(CColorButton) 

public:
	CColorButton(void);
	virtual ~CColorButton(void);
	
	BOOL Attach(const UINT nID, CWnd* pParent, 
			const COLORREF BGColor = RGB(192, 123, 192), const COLORREF FGColor = RGB(1, 1, 1)); 
	void SetFGColor(COLORREF FGColor) {
		m_fg = FGColor; 
	};
	COLORREF GetFGColor() { 
		return m_fg; 
	}; 
	void SetBGColor(COLORREF BGColor) {
		m_bg = BGColor; 
	};
	COLORREF GetBGColor() { 
		return m_bg; 
	}; 
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected: 
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS); //�ض������⺯��DrawItem 
	void DrawFrame(CDC *DC, CRect R); //���ư�Ŧ�� 
	void DrawFrameSunken(CDC *DC, CRect R); //���ư�Ŧ�� 
	void DrawFilledRect(CDC *DC, CRect R, COLORREF color); //��䰴Ŧ�� 
	void DrawLine(CDC *DC, CRect EndPoints, COLORREF color); 
	void DrawLine(CDC *DC, long left, long top, long right, long bottom, COLORREF color); 
	void DrawButtonText(CDC *DC, CRect R, const char *Buf, COLORREF TextColor); //���ư�Ŧ�ϵ��ı� 

private: 
	COLORREF m_fg;
	COLORREF m_bg; 
	BOOL m_bPressDown;
};
