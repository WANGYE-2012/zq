// ReadZQLogDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include <map>

// CReadZQLogDlg �Ի���
class CReadZQLogDlg : public CDialog
{
// ����
public:
	CReadZQLogDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_READZQLOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:
	BYTE ConvAscII(BYTE cAsc);
	BOOL OpenAndShowFile(LPCSTR pcsFName);


// ʵ��
protected:
	HICON m_hIcon;
	CRichEditCtrl m_richLog;
	std::map<BYTE, BYTE>	m_mapAscConv;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()
public:
};
