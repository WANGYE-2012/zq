// AnalyseOrderDlg.h : ͷ�ļ�
//

#pragma once


// CAnalyseOrderDlg �Ի���
class CAnalyseOrderDlg : public CDialog
{
// ����
public:
	CAnalyseOrderDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ANALYSEORDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strTradingDay;
	afx_msg void OnBnClickedOk();
};
