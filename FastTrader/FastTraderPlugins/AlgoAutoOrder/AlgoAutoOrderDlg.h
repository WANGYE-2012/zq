#pragma once

#include "BookOrderOperations.h"
#include "afxcmn.h"
#include "ExtTabCtrl.h"
#include "GeneralParamDlg.h"
#include "FeedbackDlg.h"
#include "afxwin.h"
#include "AlgoAutoData.h"

// CAlgoAutoOrderDlg dialog
#define OPT_BACKTIMER   65000
#define WAITCANCLETIMER  1000    
#define COM_IDLETIMER   2000  
#define COM_EXECUTETIMER  3000  

#define MAXVALUE 8000

class CAlgoAutoOrderDlg : public CBookOrderOperations
{
	DECLARE_DYNAMIC(CAlgoAutoOrderDlg)

public:
	CAlgoAutoOrderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAlgoAutoOrderDlg();

// Dialog Data
	enum { IDD = IDD_ALGOAUTOORDER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
 
    afx_msg void OnClose();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnTcnSelchangeShowtab(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

protected:        
    HICON m_hIcon;

public:
    static DWORD WINAPI ThreadProc(LPVOID lpParameter);
    void ParseCmd(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);   

    CString GetInstrumentID();
    void    SetInstrumentID(CString& strInstrumentID);
  
    void ReadIni(void);

    void StartTrading(void);
    void StopTrading(void);

    BOOL IsTradeTime(){return m_bInTradeTimeSpan;}
    
    double GetUpperLimitPrice(){return m_tDepthMarketData.UpperLimitPrice;}
    double GetLowerLimitPrice(){return m_tDepthMarketData.LowerLimitPrice;};
    
public:  
    CAlgoAutoData *m_pAlgoAutoData; 
    CString m_strInstrumentName;

protected:
    void ParseCmd_Base(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
    void ParseCmd_Business(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);
    void ParseCmd_Config(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum);

private:
    void AdjustChildWindowSize(void);//�����Ӵ��ڴ�С
    int  TradeRun(void);
    BOOL CancleWaitOrder();//���ҵ�
    int GetWaitOrderCount();// �������������Ĺҵ�����
    BOOL ChangeGeneralParam(SINGLEOPT* pSingle);
    void EndTrading(void);//ȡ�������½��� 
    void SaveIni(void);
    void ThreadQuitClear(BOOL bSuccess);
    

    CGeneralParamDlg m_PageGeneralParam;
    CFeedbackDlg  m_PageFeedback;

    BOOL m_bOptTrigger;// �Ż�����������    

    CExtTabCtrl m_TabCtrl;
    CStatusBarCtrl m_wndStatusBar;

    HANDLE m_hThread;
	HANDLE m_hCloseEvent;
    
    BOOL m_bThreadExit;
    BOOL m_bTrading;

    time_t m_StartTradingUTC;//��ʼִ�е�ʱ��
    // ��¼ÿ����������
    int m_SecondCount;
    // ����ÿ�������۸�
    map<int,double> m_mapSnapShot;
    // Ҫ�Ż������ʵ��
    SINGLEOPT* m_singleOptInstance;
    // ��¼�ȴ������µ�����״̬�ر���ʱ�䣨�ȴ�3��Ž����´��µ���
    int m_nWaitStatusTime;

    // �Ƿ����ڳ���
    BOOL m_bCancleOrdering;
    // �Ƿ񱨵�ȫ��״̬���£�ֻ��ȫ�������˲��ܳ���
    BOOL m_bAllOrderStatusUpdata;
    // �����Ƿ��Ѿ��µ�
    BOOL m_bOrderYet;
    // ����ֹͣ����
    BOOL m_bStopTrade;
    // ��¼�����쳣���µ�������ֻ�е��µ����������쳣����ʱ���ſɽ��д���
    int m_nExceptCount;
    // 0:�н�����Լʱ��� 1:��Ʒ�ڻ�ʱ���
    int m_nTradeType;
    // �Ƿ��ǽ���ʱ���
    BOOL m_bTradeTime;
    BOOL m_bInTradeTimeSpan;
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
