// AlgoTradingNo1Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "tabpage2_chart.h"
#include "tabpage1_listctrl.h"
#include "ExtTabCtrl.h"
#include "afxwin.h"
#include "common-AlgoTradingNo1Dlg.h"
#include "Algo_No1.hpp"
#include "KernelStruct.h"

#include <string>
#include <sstream>
#include <vector>
using namespace std;



// CAlgoTradingNo1Dlg �Ի���
class CAlgoTradingNo1Dlg : public CDialog
{
// ����
public:
	CAlgoTradingNo1Dlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CAlgoTradingNo1Dlg();

// �Ի�������
	enum { IDD = IDD_ALGOTRADINGNO1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CBrush m_brush;


	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnClose();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// ��������ǩҳ
	CExtTabCtrl m_tab_result;
	CTabpage2_Chart m_PageChart;
	CTabPage1_ListCtrl m_PageListCtrl;



	// ��Լ����
	CEdit m_InstrumentName;
	// ����
	CComboBox m_Direction;
	// ����/ƽ��
	CComboBox m_Offset;
	//	�������ݣ�������/������
	CComboBox m_TradeTrend;

	// ��ʼ�۸�
	CEdit m_S0;
	// ����
	CEdit m_X0;
	// ���ʱ��
	CEdit m_T;
	// ʱ��ֶ���
	CEdit m_N;
	// ����ƫ�æ�
	CEdit m_lambda;
	// �ղ����ʦ�
	CEdit m_Sigma;
	// �������
	CEdit m_Alpha;
	// ���ڳ���ɱ���
	CEdit m_Eta;
	// ���ڳ���ɱ���
	CEdit m_Epsilon;
	// ���ڳ���ɱ���
	CEdit m_Gamma;
	// ��
	CEdit m_tau;
	// xbar
	CEdit m_xbar;
	// ��bar
	CEdit m_EtaBar;
	// k^2
	CEdit m_k2;
	// k
	CEdit m_k;
	// kT
	CEdit m_kT;
	// k��
	CEdit m_ktau;
	// E(X)
	CEdit m_EX;
	// V(X)
	CEdit m_VX;

	//����
	CButton m_btnCalculate;
	//ִ��
	CButton m_btnTrade;
	//ȡ��
	CButton m_btnCancelTrading;

	CStatusBarCtrl m_wndStatusBar;

public:
	//��ȡini�ļ���������Ϣ
	void ReadIni(void);
	//����������Ϣ��ini�ļ�
	void SaveIni(void);

	//������,xk(������),nk(������),xk2(������),nk2(������)
	CAlgo_No1 m_algo;

	//�Ƿ��¼�ɹ�
	bool m_bLogon;
	//��ǰ���˺ţ���FastTrader�л�ȡ
	string m_Account;
	//��һ�ζ�������ĺ�Լ
	string m_SubscribedInstrumentName;

	//�Ƿ�����ִ�н���
	bool m_bTrading;
	//��ʼִ�е�ʱ��
	time_t m_StartTradingUTC;
	//��ǰִ�еĽڣ�0 ... N-1
	int m_TradingSession;


	static int PluginsAPICallBack(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId);
	static CAlgoTradingNo1Dlg* m_pDlg;


	//ʹ�ܻ�������пؼ�
	void EnableAllControls(BOOL bEnable);
	//����һ���µ�,����m_algo��m_TradingSession��VolumeΪ-1ʱ���Զ������µ���
	void SendInsertOrder(int Volume=-1);
	//����ɽ��ر�
	void TradeInfoReach(PlatformStru_TradeInfo& TradeInfo);
	//�������ر���
	void OnOrderRtn(PlatformStru_OrderInfo& OrderInfo);
	//ֹͣ����
	void StopTrading(void);

	//�����Ӵ��ڴ�С
	void AdjustChildWindowSize(void);
	//���ؼ��е�ֵ�ŵ�m_algo��ȥ
	void GetAlgoParamFromWnd(void);
	//��m_algo�е�ֵ���õ��ؼ���ȥ
	void SetAlgoParamToWnd(void);

//	afx_msg void OnEnUpdateEdit1();
};

