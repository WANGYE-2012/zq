// TestorProxyDlg.h : ͷ�ļ�
//

#pragma once
#include "FastTraderManager.h"
#include "GridCtrl\GridCtrl.h"
#include <map>

using namespace std;

// CTestorProxyDlg �Ի���
class CTestorProxyDlg : public CDialog
{
public:
	enum {	conColUserSeqNum, conColUserName, conColRecvNum, 
			conColSendNum, conColMsgRecvNum, conColInsertNum, conColRejectNum };

	typedef struct {
		int nFolderNum;
		int nRecvNum;
		int nSendNum;
		int nMsgRecvNum;
		int nInsertNum;
		int nRejectNum;
	} ITEMNUM;

// ����
public:
	CTestorProxyDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTORPROXY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


protected:
	int FindRow(int nSeqNum);
	void UpdateGrid_ID(int nRow, int nSeqNum, CString& strAccount);
	void UpdateGrid_RecvNum(int nSeqNum, int nNum);
	void UpdateGrid_SendNum(int nSeqNum, int nNum);
	void UpdateGrid_MsgRecvNum(int nSeqNum, int nNum);
	void UpdateGrid_InsertNum(int nSeqNum, int nNum);
	void UpdateGrid_RejectNum(int nSeqNum, int nNum);

// ʵ��
protected:
	HICON m_hIcon;
	CFastTraderManager	m_fastTraderManager;
	CGridCtrl			m_gridStatus;
	map<int, ITEMNUM>	m_mapAccountSum;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
};
