// TSSPlotConfigDlg.h : ͷ�ļ�
//

#pragma once

#include "GridCtrl.h"
#include "afxwin.h"

// CTSSPlotConfigDlg �Ի���
class CTSSPlotConfigDlg : public CDialog
{
// ����
public:
	CTSSPlotConfigDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TSSPLOTCONFIG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:
	void ChangeUpPoint(int nPos);

// ʵ��
protected:
	HICON m_hIcon;
	CGridCtrl	m_gridPlotCtrl;
	CScrollBar m_ctlMyHScrollBar;
	double m_fMaxLose;
	UINT m_nForceCloseTimer;
	UINT m_nCancelForceTimer;
	int m_nUpTick;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedReadfile();
	afx_msg void OnBnClickedSavefile();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	unsigned int m_nLongColor;
	unsigned int m_nShortColor;
	unsigned int m_nNoneColor;
};
