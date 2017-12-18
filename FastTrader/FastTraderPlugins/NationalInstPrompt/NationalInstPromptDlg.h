// NationalInstPromptDlg.h : header file
//

#pragma once

#include "OptSetupDlg.h"

#include <map>
#include <set>
#include <string>
using namespace std;


// CNationalInstPromptDlg dialog
class CNationalInstPromptDlg : public CDialog
{
// Construction
public:
	CNationalInstPromptDlg(CWnd* pParent = NULL);	// standard constructor
    virtual ~CNationalInstPromptDlg();

// Dialog Data
	enum { IDD = IDD_NATIONALINSTPROMPT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();  
    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnBnClickedButtonModify();
    afx_msg void OnBnClickedButtonDelete();
    afx_msg void OnBnClickedButtonApply();
    afx_msg void OnClose();   
    afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);    
    afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg LRESULT OnChildClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

    CListCtrl m_listCtrl;
    CButton m_btnApplyCtrl;

public:
    // �Ƿ��¼��CTP
    bool m_bLogon;  
    string m_szAccount;                         //��ǰ���˺�
    set<std::string> m_setExchangeID;           //������
    set<string> m_setInstrumentID;              //��Լ

    COptSetupDlg* m_pOptDlg;                    //��ǰ��OptSetupDlg
    static CNationalInstPromptDlg* m_pMainDlg;
    static int PluginsAPICallBack(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId);


    //�����б�����
    void EnterAlgorithm(const PlatformStru_DepthMarketData* pMarketData);
    void UpdataListCtrl(const PlatformStru_DepthMarketData* pMarketData);

private:

    //��ȡini�ļ���������Ϣ
    bool ReadData(void);
    //����������Ϣ��ini�ļ�
    void SaveData(void);

    void SetDataToListCtrl();
    void SubscribeInstrument();
    //void StartThread();
    //�µ�
    void SendInsertOrder(CParamData* pData);

    //�õ���ǰѡ��listctrl������
    int GetListSelectedIndex();

private:  
						
    // �Ӵ����Ƿ�ر�
    bool m_bChildOpened;
    //�������е�����
    map<int, CParamData*> m_mapIndex2Data;


};
