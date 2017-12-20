// NCButtonManager.cpp: implementation of the CNCButtonManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCButtonManager.h"
#include "NCButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNCButtonManager::CNCButtonManager()
{
	m_hParentWnd = NULL;
	m_pHoverButton = NULL;
	m_pPressButton = NULL;
	m_bWindowPosChanging = FALSE;
}

CNCButtonManager::~CNCButtonManager()
{
	m_TitleButtonVector.empty();
}

LRESULT CNCButtonManager::DefWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = 0;
	CNCButton *pButton = NULL;

	switch(message)
	{
	case WM_ACTIVATE:
		OnActivate(hwnd, wParam, lParam);
		break;
	case WM_ACTIVATEAPP:
		OnActivateApp(hwnd, wParam, lParam);
		break;
	case WM_NCACTIVATE:
		OnNcActivate(hwnd, wParam, lParam);
		break;
	case WM_PAINT:
	case WM_NCPAINT:
		OnPaint(hwnd);
		break;
	case WM_SIZING:
		OnParentSizing(hwnd, wParam, lParam);
		break;
	case WM_SIZE:
		OnParentSize(hwnd, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(hwnd, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		OnLButtonUp(hwnd, wParam, lParam);
		break;
	case WM_LBUTTONDBLCLK:		// ��˫����Ϊ��������
	case WM_LBUTTONDOWN:
		OnLButtonDown(hwnd, wParam, lParam);
		break;
	case WM_NCHITTEST:
		lResult = OnNcHitTest(hwnd, wParam, lParam);
		break;
	case WM_WINDOWPOSCHANGING:
		OnWindowPosChanging(hwnd, wParam, lParam);
		break;
	case WM_WINDOWPOSCHANGED:
		OnWindowPosChanged(hwnd);
		break;
	case WM_CREATE:
		m_hParentWnd = hwnd;
		break;
	}
	return lResult;
}

//----------------------------------------------------
//	��Ӱ�ť
//----------------------------------------------------

BOOL CNCButtonManager::AddButton(CNCButton *pButton)
{
	ASSERT(pButton);

	m_TitleButtonVector.push_back(pButton);
	return TRUE;
}

//-----------------------------------------------------
//	ɾ����ť
//	���ݻٰ�ť����
//-----------------------------------------------------

void CNCButtonManager::RemoveButton(CNCButton *pButton)
{
	vector<CNCButton *>::iterator pos;
	pos = find(m_TitleButtonVector.begin(), m_TitleButtonVector.end(), pButton);
	if(pos != m_TitleButtonVector.end())
		m_TitleButtonVector.erase(pos);
}

//----------------------------------------------
//	������hwndƥ��İ�ť
//----------------------------------------------

CNCButton* CNCButtonManager::EnumActiveButton(UINT& nIdx, HWND hwnd)
{
	CNCButton *pResultButton = NULL;
	for(UINT i = nIdx; i < m_TitleButtonVector.size(); i++, nIdx++)
	{
		CNCButton *pButton = m_TitleButtonVector[nIdx];
		if(pButton->GetParentWnd() == NULL || pButton->GetParentWnd() != hwnd)
		{
			continue;
		}
		else
		{
			pResultButton = pButton;
			break;
		}
	}
	nIdx++;
	return pResultButton;
}

//----------------------------------------------
//	��Ӧ�����ڵ�WM_PAINT��������Ҫ���Ƶİ�ť
//----------------------------------------------

void CNCButtonManager::OnPaint(HWND hwnd)
{
	if(m_bWindowPosChanging == TRUE)
		return;

	CNCButton *pButton;
	UINT nIdx = 0;
	while((pButton = EnumActiveButton(nIdx, hwnd)) != NULL)
	{
		pButton->OnPaint();
	}
}

void CNCButtonManager::OnParentSizing(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CNCButton *pButton;
	UINT nIdx = 0;
	while((pButton = EnumActiveButton(nIdx, hwnd)) != NULL)
	{
		pButton->OnParentSize(wParam, (LPRECT)lParam);
		//pButton->OnPaint();
	}
}

void CNCButtonManager::OnParentSize(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	RECT WndRect;
	GetWindowRect(hwnd, &WndRect);

	CNCButton *pButton;
	UINT nIdx = 0;
	while((pButton = EnumActiveButton(nIdx, hwnd)) != NULL)
	{
		pButton->OnParentSize(wParam, &WndRect);
		pButton->OnPaint();
	}
}

//---------------------------------------------------
//	����WM_NCHITTEST
//	������ť״̬
//---------------------------------------------------

LRESULT CNCButtonManager::OnNcHitTest(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = 0;

	CNCButton *pCurHoverButton = NULL;

	RECT rect;
	GetWindowRect(hwnd, &rect);
	POINT point;
	point.x = LOWORD(lParam) - rect.left;
	point.y = HIWORD(lParam) - rect.top;

	CNCButton *pButton;
	UINT nIdx = 0;
	while((pButton = EnumActiveButton(nIdx, hwnd)) != NULL)
	{
		if(pButton->OnHitTest(point))
		{
			pCurHoverButton = pButton;
			lResult = HTCLIENT;
			point.y += 1;
			pCurHoverButton->OnMouseMove(point);
			break;
		}
	}

	if(pCurHoverButton == NULL && m_pHoverButton != NULL)
	{
		m_pHoverButton->OnMouseLeave();
		m_pHoverButton->OnPaint();
	}

	m_pHoverButton = pCurHoverButton;

	return lResult;
}

//----------------------------------------------------
//	�ͷ�������
//----------------------------------------------------

void CNCButtonManager::OnLButtonUp(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if(m_pPressButton != NULL)
	{
		m_pPressButton->OnMouseLButtonUp();
		if(m_pHoverButton != NULL)
		{
			m_pHoverButton->OnMouseClick();

			// �򸸴��ڷ�����Ϣ
			UINT nButtonID = m_pHoverButton->GetButtonID();
			SendMessage(m_pHoverButton->GetParentWnd(),
				WM_COMMAND, nButtonID, 0);
		}
		m_pPressButton->OnPaint();
		m_pPressButton = NULL;
	}
}


//----------------------------------------------------
//	�ͷ�����Ҽ�
//----------------------------------------------------

BOOL CNCButtonManager::OnRButtonUp(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if(m_pHoverButton != NULL)
		return TRUE;
	else
		return FALSE;
}

//------------------------------------------------------
//	����������
//------------------------------------------------------
void CNCButtonManager::OnLButtonDown(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if(m_pHoverButton != NULL)
	{
		m_pPressButton = m_pHoverButton;
		m_pPressButton->OnMouseLButtonDown();
		m_pPressButton->OnPaint();
	}
}

//------------------------------------------------------
//	����ƶ���Ϣ
//------------------------------------------------------

void CNCButtonManager::OnMouseMove(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	// �����lParam�����Client�����꣬��Ҫת����Window
	UINT nBorderCX, nBorderCY, nCaptionCY;
	nBorderCX = GetSystemMetrics(SM_CXBORDER);
	nBorderCY = GetSystemMetrics(SM_CYBORDER);
	nCaptionCY = GetSystemMetrics(SM_CYCAPTION);
	UINT nEdgeCX = GetSystemMetrics(SM_CXEDGE);
	UINT nEdgeCY = GetSystemMetrics(SM_CYEDGE);
	
	POINT ptClient;
	ptClient.x = 0;
	ptClient.y = 0;
	ClientToScreen(m_hParentWnd, &ptClient);

	RECT rectWnd;
	GetWindowRect(m_hParentWnd, &rectWnd);

	POINT point;
	//point.x = LOWORD(lParam) +  nEdgeCX + nBorderCX + 1;
	//point.y = nEdgeCY + nBorderCY + nCaptionCY - HIWORD(lParam * -1);
	ClientToScreen(m_hParentWnd, &point);

	point.x = LOWORD(lParam) +  nEdgeCX + nBorderCX + 1;
	point.y = ptClient.y - rectWnd.top - HIWORD(lParam * -1) - 1;

	//TRACE("point:%d,%d\r\n", point.x, point.y);

	if(m_pHoverButton != NULL)
	{
		if(m_pHoverButton->OnHitTest(point) == FALSE)
		{
			m_pHoverButton->OnMouseLeave();
			m_pHoverButton->OnPaint();
			m_pHoverButton = NULL;
		}
		else
		{
			m_pHoverButton->OnMouseMove(point);
		}
	}
	else
	{
		if(m_pPressButton != NULL)
		{
			if(m_pPressButton->OnHitTest(point))
			{
				m_pHoverButton = m_pPressButton;
				m_pPressButton->OnMouseMove(point);
				m_pPressButton->OnPaint();
			}
		}
	}
}

//-------------------------------------------------
//	�����ڳߴ����ڸı�ʱ�����ػ�
//-------------------------------------------------

void CNCButtonManager::OnWindowPosChanging(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LPWINDOWPOS lpPos = (LPWINDOWPOS)lParam;
	if(
		m_hParentWnd == hwnd &&
		(lpPos->flags & SWP_NOSIZE) != SWP_NOSIZE &&
		(lpPos->flags & SWP_NOMOVE) != SWP_NOMOVE
		)
	{
		m_bWindowPosChanging = TRUE;
	}
}

void CNCButtonManager::OnWindowPosChanged(HWND hwnd)
{
	m_bWindowPosChanging = FALSE;
	OnPaint(hwnd);	
}

void CNCButtonManager::OnNcActivate(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	m_bWindowPosChanging = FALSE;
	if(hwnd == m_hParentWnd && (BOOL)wParam == FALSE)
	{
		ResetAllButtons();
	}
	OnPaint(hwnd);
}

void CNCButtonManager::OnActivateApp(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	m_bWindowPosChanging = FALSE;
	if(hwnd == m_hParentWnd && (BOOL)wParam == FALSE)
	{
		ResetAllButtons();
	}
	OnPaint(hwnd);
}


void CNCButtonManager::OnActivate(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	m_bWindowPosChanging = FALSE;
	if(hwnd == m_hParentWnd && wParam == WA_INACTIVE)
	{
		ResetAllButtons();
	}
	OnPaint(hwnd);
}

//----------------------------------------------
//	�����а�ť״̬��λ
//----------------------------------------------

void CNCButtonManager::ResetAllButtons()
{
	for(UINT i = 0; i < m_TitleButtonVector.size(); i++)
	{
		CNCButton *pButton = m_TitleButtonVector[i];
		pButton->SetState(BTNSTATE_NORMAL);
	}
}

BOOL CNCButtonManager::PreTranslateMessage(MSG *pMsg)
{
	BOOL bResult;

	switch(pMsg->message)
	{
		case WM_RBUTTONUP:
			bResult = OnRButtonUp(pMsg->hwnd, pMsg->wParam, pMsg->lParam);
			break;
		default:
			bResult = FALSE;
			break;
	}
	
	return bResult;
}
