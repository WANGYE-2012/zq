// NationalInstPromptDlg.h : header file
//

#pragma once

#include "easymutex.h"
#include "OptSetupDlg.h"
#include "DelegationDetailDlg.h"
#include "LogFile.h"

#include <map>
#include <set>
#include <string>
#include "afxwin.h"
using namespace std;


struct Stru_Inst2Price
{
    double BidPrice1;//��һ��
    double AskPrice1;//��һ��
};

enum PRICE_TYPE
{
    PRICE_TYPE_BID,
    PRICE_TYPE_ASK
};

#define BASE_YEAR 2014
/*
       sign      Year        Month        Day                   id   
32bit  |-1-|------6------|-----5-----|-----5-----|--------------15---------------|  
*/
#define  MAKE_DATEBASE_ID(_year,_month,_day,_id) \
    (int)((int)( (int)(_year) << 25 ) | (int)( (int)(_month) << 20 ) | (int)( (int)(_day) << 15 ) | (int)(_id)  )



// CArbitrageDelegationDlg dialog
class CArbitrageDelegationDlg : public CDialog
{
// Construction
public:
	CArbitrageDelegationDlg(CWnd* pParent = NULL);	// standard constructor
    virtual ~CArbitrageDelegationDlg();

// Dialog Data
	enum { IDD = IDD_ARBITRAGEDELEGATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedButtonArbitrage();    
    afx_msg void OnBnClickedButtonModify();
    afx_msg void OnBnClickedButtonRevoke();
    afx_msg void OnBnClickedButtonDelegatedetail();
    afx_msg void OnClose();   
    afx_msg LRESULT OnSetupDlgClose(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnDetailDlgClose(WPARAM wParam, LPARAM lParam);
    afx_msg void OnBnClickedCheckDisplayrevoke();
    afx_msg void OnBnClickedCheckTriggered();    
    afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

    CListCtrl m_listCtrl;


public:
    // �Ƿ��¼��CTP
    bool m_bLogon;  
    string m_szAccount;                         //��ǰ���˺�

    CLogFile*   m_pLog;                         //log
    COptSetupDlg* m_pOptDlg;                    //��ǰ��OptSetupDlg
    CDelegationDetailDlg* m_pDetailDlg;         //��ǰ��DelegationDetailDlg
    static CArbitrageDelegationDlg* m_pMainDlg;
    static int PluginsAPICallBack(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId);

private:

    //��ȡini�ļ���������Ϣ
    bool ReadData(void);
    //����������Ϣ��ini�ļ�
    void SaveData(void);

    void SetDataToListCtrl(bool bDevoke = false,bool bTrigger = false);
    void SubscribeInstrument();

    //�µ�
    void SendInsertOrder(CParamData* pData);

    //�õ���ǰѡ��listctrl������
    int GetListSelectedIndex();


    //���鵽����
    //bool bMatch(PlatformStru_DepthMarketData* pMarketData,CParamData* pDataParam);
    bool EnterAlgorithm(CParamData* pDataParam);
    void UpdateInst2Price(string InstrumentID,double BidPrice1, double AskPrice1);
    double GetPrice(string InstrumentID,PRICE_TYPE nType);
    int  GetListIndex(int nParamID);
    bool bInstsArrived(string szInst1,string szInst2);


    //�ڴ�����
    bool GetOrderRefByParamID(int nParamID,string& orderRef1,string& orderRef2);
    //�ڴ����ޣ���FastTrader����
    bool GetOrderKeyByParamID(int nParamID,set<OrderKey>& setOrderKey);
    bool GetOrderKeyByOrderRef(string szOrderRef,OrderKey& key);
    
    void SetSetupDlgOpened(int nVal);
    bool GetSetupDlgOpened();

    void SetDetailDlgOpened(int nVal);
    bool GetDetailDlgOpened();

private:  
						
    // SetupDlg�����Ƿ��
    volatile int m_bSetupDlgOpened;    
    
    // DetailDlg�����Ƿ��
    volatile int m_bDetailDlgOpened;    
    

    //�����Ѵ��������� key=Param_ID
    map<int, CParamData*> m_mapIndex2DataTriggered;

    //�����ѳ��������� key=Param_ID
    map<int, CParamData*> m_mapIndex2DataRevoked;

    //����δ������δ���������� key=Param_ID
    map<int, CParamData*> m_mapIndex2Data;
    
    //�����Լ�����
    map<string, Stru_Inst2Price> m_mapInst2Price;
    
    //���н�������Լ
    set<string> m_setInstrumentID;

    //��ˮ��
    int m_nNextArbitrageID;

    Ceasymutex m_Inst2PriceMutex;
    //Ceasymutex m_Inst2SubedMutex;
    Ceasymutex m_OrderMutex;
    Ceasymutex m_DataMutex;

    //����CheckButton
    static bool m_bRevoke;
    static bool m_bTrigger;

    //�µ���� key1:InstrumentID
    map<string,bool>       m_mapInstrument2Subscribed;        //��¼���ĵĺ�Լ

    //������������ϵ ���Եõ�Param_ID��FTID��ӳ�䣬������������FastTrader����ί�е�
    //<Param_ID OrderRef>   
    map<int,set<string>>       m_mapParamID2OrderRef;        //Param_ID��OrderRef�İ󶨹�ϵ  ���ڳ�������еĲ鿴��ϸ

    //<OrderRef OrderKey>   
    map<string,OrderKey>       m_mapOrderRef2OrderKey;        //OrderRef��OrderKey��ϸ���İ󶨹�ϵ 



    //���ĵĺ�Լ��������Ƿ�ȫ������
    volatile bool m_bInstSubedAllReached;
    
};
