// TestTPriceListDlg.h : ͷ�ļ�
//

#pragma once


// CTestTPriceListDlg �Ի���
class CTestTPriceListDlg : public CDialog
{
// ����
public:
	CTestTPriceListDlg(CWnd* pParent = NULL);	// ��׼���캯��
	BOOL CreateTPriceList(int nMode);

// �Ի�������
	enum { IDD = IDD_TESTTPRICELIST_DIALOG };
	enum {	conTimerCreateTPriceList = 123456 };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

protected:
	HWND	m_pPanel;
public:
};
