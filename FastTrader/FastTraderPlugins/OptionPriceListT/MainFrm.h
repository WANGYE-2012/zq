// MainFrm.h : CMainFrame ��Ľӿ�
//

#include "DlgOptionTTable.h"
#include "DlgOptionTTablePopup.h"

#pragma once

class CMainFrame : public CFrameWnd
{
public:
	enum {	conTimerRepaint = 123457 };


public:
	CMainFrame();
	
protected: // �������л�����
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

protected:
	void ParseCmd(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
	void ParseCmd_Console(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
	void ParseCmd_BaseBusiness(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);

	void QueryCurrInstrumentID();
	void QueryProductShortNames();
	void QueryExchangeIDs();
	void QueryInstrumentIDs(CString& strExchangeID);
	void QueryInstrumentInfo(CString& strInstrumentID);
	void QueryCurrInstrumentList();
	void ShowNewTTableDialog(CString& strInstrumentID);
	void ActiveDialog(HWND hWnd);

// ����
public:
	static int CallbackRecvApiMsg(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId);

// ��д
public:

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CDlgOptionTTable	m_dlgOptionTTable;

// ���ɵ���Ϣӳ�亯��
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

protected:
	BOOL	m_bExitApp;
	BOOL	m_bWaitForNewTTableDlg;
	std::queue<std::string> m_queueExchangeForQuery;
	std::queue<std::string> m_queueInstrumentForQuery;

public:
};


