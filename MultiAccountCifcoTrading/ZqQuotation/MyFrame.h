#pragma once
#include "wx/frame.h"

DECLARE_EVENT_TYPE(wxEVT_Data_InTime, 3700)
DECLARE_EVENT_TYPE(wxEVT_GetTime, 3701)
DECLARE_EVENT_TYPE(wxEVT_SetLineViewTime, 3702)
DECLARE_EVENT_TYPE(wxEVT_ReSend_Message, 3703)
class MyFrame: public wxFrame
{
public:
	MyFrame(WXHWND hWnd );
	virtual ~MyFrame();
	// Menu commands
  void PhraseMin1(wxCommandEvent& event);
  void PhraseMin5(wxCommandEvent& event);
  void PhraseMin15(wxCommandEvent& event);
  void PhraseMin30(wxCommandEvent& event);
  void PhraseMin60(wxCommandEvent& event);

  void PhraseDay(wxCommandEvent& event);
  void PhraseWeek(wxCommandEvent& event);
  void PhraseMonth(wxCommandEvent& event);
  void Quit(wxCommandEvent& WXUNUSED(event) );

  void CheckPhraseUI(int nID);

  void MarkTriangleUp(wxCommandEvent& event);
  void MarkTriangleDown(wxCommandEvent& event);

  void KXIAN(wxCommandEvent& event);
  void FENSHI(wxCommandEvent& event);

  void OnUpdateUI(wxUpdateUIEvent& evt);

  void OnSize(wxSizeEvent& event);

  void OnDataInTime(PlatformStru_DepthMarketData* pData, CFData* pFData, bool bUpdate);//bupdate ��Ҫ���������ǲ��ǵ�ǰ��ʾ���ڻ����飬����ˢ�£�������ˢ��
  void OnReSendMessage(wxCommandEvent& event);

  
private:
	DECLARE_EVENT_TABLE()
	DECLARE_NO_COPY_CLASS(MyFrame)

	CKLineView*		m_pKLineView;
	CTLineView*		m_pTLineView;
	CQPanel*		m_pQPanel;		
	KSplitterWindow* m_splitter;

	string			m_ExchangeName; //����������
	string			m_ContractName; //��Լ����
	string			m_ContractDate; //��Լ����
public:
	wxTimer			m_timerClose;
	void			OnTimer1(wxTimerEvent& event);


	
private:
	WXHWND   m_parent;
public:


	WXLRESULT MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam); 
	void OnClose(wxCloseEvent& evt);

	void SetTodayScales(struct tm tmDay, wxString ExchangeName, vector<struct tm>& vecTimeScales);
	void SetTodayScales(struct tm tmDay, wxString strTransSpan, wxString ExchangeName, vector<struct tm>& vecTimeScales);
	void SetTodayScales(string strTimes, vector<struct tm>& vecTimeScales);
private:
	void OnFuncLDBClickKLine(wxCommandEvent& event);	
	void OnFuncLDBClickTLine(wxCommandEvent& event);

	void OnFuncKeyDownTLine(wxCommandEvent& event);
	void OnFuncKeyDownKLine(wxCommandEvent& event);
	void OnFuncKeyQPanel(wxCommandEvent& event);



private:
	bool InitFromCFG();
	bool InitCfg(TiXmlElement *root);

	struct  SSpan
	{
		struct tm tmBegin;
		struct tm tmEnd;
	};
	map<string, vector<SSpan*>*> m_mapTransTime;//����ʱ�䣬�����Ѿ��ò����ˣ����ĳ�콻��ʱ��ĵ������������������ô��������õ��ϵģ��ȱ������룬�Ա��������ܵĸ��ġ�


	
public:
	wxTimer			m_timerTrans;
	int				m_nTime;//timer��ʱ������
	DWORD			m_dwBeginTickCount;
	struct tm		m_tmLastTrans;//�����ʱ��
	void			SetLastTransTime(struct tm tmLastTrans);
	bool			m_bChangedTime;//ʱ����ı�־
	string			m_strTradingDay; //��������
	bool			m_bInitTimerTrans;//��һ��ִ�У����ڳ�ʼ������
private:
	//CTCPDataService*	m_poTCPDataService;
	

public:
	PlusinAPI*		m_pApi;
	int OnRecvApiMsg_Process( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId  );
	static int OnRecvApiMsg(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId);

	void ProcessConsoleMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen , int nRequestId);
	void ProcessExchangeMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen , int nRequestId);
	void ProcessInstrumentMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId );
	void ProcessQuoteMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId );
	void ProcessAccountMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId );

	void ClearFData();
	map<string,  CFData *>  m_mapID2FData;
	map<DWORD,  string>     m_mapNum2ID;     //���ذ���˳�򣬶�Ӧ��ֻ�ڻ���id
	int						m_nQueryNum;     //��������

	CFData*		GetFDataByQueryID(int nQueryNum);
	int			GetNextRequestId();
	void        InitGUI(CFData *pData, PlatformStru_DepthMarketData *pQutoData, bool bRemain);
	void        InitGUIData(CFData *pFData, PlatformStru_DepthMarketData *pQutoData);

	string		m_strSelFData;//��ֻ����ʾ��

protected:
	vector<string> m_vecFData;    //���۱��е����к�Լ
	int			   m_nFDataIndex; //�ڱ��۱�����
	void		   OnPrior();     //���۱��ǰһ����Լ����
	void		   OnNext();      //���۱����һ����Լ����

	void		   DelFData(int nCount);//��������ֻ��Լ��ɾ���ڴ��ж����
protected:
	int			   m_nTrace;	
	BOOL GetRegIntValue(HKEY hRoot,wxString strKeyName,wxString strValueName,int& nValue);


protected:
	void OnFuncReturnKLine(wxCommandEvent& event);
	void OnFuncReturnTLine(wxCommandEvent& event);
	void OnFuncReturnBaoJiaBiao(wxCommandEvent& event);
	void OnFuncReturnBaoJiaBiao2(wxCommandEvent& event);


	void OnReturnKLine();
	void OnReturnTLine();
	void OnReturnBaoJiaoBiao();

	int m_nRightSpanMiniWidth;
	void SetMiniWidth(unsigned long value);
};

struct SMessage
{
	int nCmdId;
	int nErrorCode;
	void* pData;
	int nDataLen;
	int nRequestId;

	SMessage(int nId, int nCode, void* p, int nLen, int nRId)
	{
		nCmdId			= nId;
		nErrorCode		= nCode;
		if(nLen >0)
		{
			pData       = new char[nLen];
			memcpy(pData, p, nLen);
		}
		else 
			pData  = NULL;
		
		nDataLen = nLen;
		nRequestId = nRId;
	}

};