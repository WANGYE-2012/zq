// TSSCtrlPluginDlg.h : ͷ�ļ�
//

#pragma once

#include "TSSCtrlPluginOperations.h"
#include "afxcmn.h"
#include "GridCtrl\GridCtrl.h"

// CTSSCtrlPluginDlg �Ի���
class CTSSCtrlPluginDlg : public CTSSCtrlPluginOperations
{
protected:
	void ParseCmd(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
	void ParseCmd_Console(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
	void ParseCmd_BaseBusiness(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
	void ParseCmd_Business(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
	void ParseCmd_Verify(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
	void ParseCmd_Check(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);

// ����
public:
	static int CallbackRecvApiMsg(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId);

protected:
	BOOL		m_bExitApp;
	CGridCtrl	m_gridPlotCtrl;
	CGridCtrl	m_gridPosition;
	CGridCtrl	m_gridPosiDetail;
	CRichEditCtrl m_ctlMsgText;

protected:
	void ResetPlotCtrlData();

	int GetRowIndexByRowID_Plot(int nRowID);
	int GetColIndexByColID_Plot(int nColID);
	int GetRowIndexByRowID_Position(int nRowID);
	int GetColIndexByColID_Position(int nColID);
	int GetRowIndexByRowID_PosiDetail(int nRowID);
	int GetColIndexByColID_PosiDetail(int nColID);
	void UpdateRow(int nIndex, INSTRUMENTCTRLDIRECT& plotCtrl); 
	void UpdateRow(int nIndex, PLOTCTRLITEMCFG& plotCtrl); 
	void UpdateRow(int nIndex, PlatformStru_Position& position); 
	void UpdateRow(int nIndex, PlatformStru_PositionDetail& posiDetail); 
	void ShowQueue(std::queue<INSTRUMENTCTRLDIRECT>& queueData); 
	void ShowQueue(std::queue<PLOTCTRLITEMCFG>& queueData); 
	void ShowQueue(std::queue<PlatformStru_Position>& queueData, int nItemCount); 
	void ShowQueue(std::queue<PlatformStru_PositionDetail>& queueData, int nItemCount); 
	bool DeleteRow_Plot(std::string& strKey); 

	void ShowMsgLog(CString& strMsg);

protected:
    void CancleOrderLast(PlatformStru_OrderInfo& tInfo);

// ����
public:
	CTSSCtrlPluginDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TSSCTRLPLUGIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CTabCtrl m_ctlTabPosition;

	// ���ɵ���Ϣӳ�亯��
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnNMClickTabposition(NMHDR *pNMHDR, LRESULT *pResult);
};
