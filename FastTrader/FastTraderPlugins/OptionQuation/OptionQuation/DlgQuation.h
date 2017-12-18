#pragma once
#include "resource.h"
#include "FData.h"
#include "QView.h"
#define  QUERY_MIN1_DAYCOUNT	20
#define  QUERY_MIN5_DAYCOUNT	100
#define  QUERY_MIN15_DAYCOUNT	100
#define  QUERY_MIN30_DAYCOUNT	100
#define  QUERY_MIN60_DAYCOUNT	100
#define  QUERY_DAY_DAYCOUNT		3650//10�������Ӧ���ǹ���
#define  QUERY_WEEK_DAYCOUNT	3650
#define  QUERY_MONTH_DAYCOUNT	3650

#define  QUERY_MAX_RETURN		1000
struct SGetInfo
{
	int nType;    //nType =0 ��ʱ��str����ʱ��(��ʽ�� 10:15:50)���Ժ�ntype���ܻ��б����չ
	std::string str;
	SGetInfo()
	{
		nType =0;
		str = "";
	}
};
// CDlgQuation dialog
typedef bool (*CallBackGetTime)(SGetInfo& sGetInfo);

class __declspec(dllexport) CDlgQuation : public CDialog
{
	DECLARE_DYNAMIC(CDlgQuation)

public:
	CDlgQuation(CWnd* pParent = NULL); 
	virtual ~CDlgQuation();
	HICON m_hIcon;
	EnumPhrase m_enumPhrase;

	CallBackGetTime m_callbackGet;
	void SetCallBackFunction(CallBackGetTime callbackGet);


	//���ý�����
	void SetTradingDay(std::string strTradingDay);//20140828
	//Ҫ��ʾ�ĺ�Լ��
	void ShowInstrument(std::string strInstrument);	//IF1410
	//��Լ����
	bool SetInstrumentInfo(PlatformStru_InstrumentInfo& InstrumentData);
	//���յĽ��׿̶�
	void SetTradingScales(std::string& strInstrument, std::string& strTime);//ʱ��ĸ�ʽ�� 201408280915201408281130  201408281300201408281515
	//�ú�Լ���������
	void SetDepthMarketData(std::string& strInstrument, PlatformStru_DepthMarketData* QutoData);

	//���������к��������꣬�ٵ����������
	void InitFinish(std::string strInstrument);


// Dialog Data
	enum { IDD = IDD_DIALOG_QUATION };

	CQView		  m_QView;
//////////////////////////////////////////////////////////////////////////
//���ز�����
public:
/*	CFData		  m_FData;	
	void ReadTLineFileTest();	
	void ReadFileTest_KLineDay();
	void ReadFileTest_KLine60();*/
//////////////////////////////////////////////////////////////////////////
	
	void SetTodayScales(string strTimes, vector<struct tm>& vecTimeScales);
private:
	map<string,  CFData>		m_mapID2FData;
	std::queue<std::string>		m_queueInstrument;
	bool						m_bConnectHQServer;  //����ʵʱ����������״̬
	bool						m_bConnectHQHistoryServer;//������ʷ����������״̬
	std::string				    m_strInstrument; //���ĵ�����
	std::string					m_strTradingDay;//����Ľ�����	
	std::string					m_strUpdateTime;//�������µ�ʱ��

	string						m_strSelFData;  //��ֻ����ʾ���ڻ�
	void						OnDataInTime(PlatformStru_DepthMarketData* pData, CFData* pFData, bool bUpdate);

	map<int, KLineQuery>		m_mapIndex2QueryInfo;
	int nQueryIndex;
	
protected:
	void SetPhraseVector(EnumPhrase enumPhrase, CFData& fdata);

	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

	LRESULT OnMessageReturn(WPARAM wP,LPARAM lP);
	LRESULT OnMessageQutoInTime(WPARAM wP,LPARAM lP);
	LRESULT OnMessageConnectSuccess(WPARAM wP,LPARAM lP);
	LRESULT OnMessageConnectfail(WPARAM wP,LPARAM lP);
	LRESULT OnMessageChangeTimeSpan(WPARAM wP,LPARAM lP);
	LRESULT OnMessageQutoTLineHistory(WPARAM wP,LPARAM lP);	
	LRESULT OnMessageQutoKLineHistory(WPARAM wP,LPARAM lP);	
	LRESULT OnMessageIndexExpress(WPARAM wP,LPARAM lP);	
	LRESULT OnMessageIndexConfig(WPARAM wP,LPARAM lP);	
	LRESULT OnMessageConfigChange(WPARAM wP,LPARAM lP);	
	

	void	DelFData(std::string strInstrument);
	void    SubscribleInstrument(std::string strInstrument);
	void    UnSubscribleInstrument(std::string strInstrument);

	void    QueryHistoryTData(CFData& fData);
	void    QueryHistoryKLine(CFData& fData, EnumPhrase enumPhrase);	
	void	QueryHistoryKLine_Mins(CFData& fData, EnumPhrase	enumPhrase, int nDayCount);
	
	void	QueryHistoryKLine_Day(CFData& fData);
	void	QueryHistoryKLine_Week(CFData& fData);
	void	QueryHistoryKLine_Month(CFData& fData);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void OnDestroy();
	//��ʷ�����������ַ
	std::string		m_HisSvrIP;
	unsigned short	m_HisSvrPort;
	//ʵʱ�����������ַ
	std::string		m_IntimeQuotSvrIP;
	unsigned short  m_IntimeQuotSvrPort;
	void Load(std::string& HisSvrIP, unsigned short& HisSvrPort, std::string& IntimeQuotSvrIP, unsigned short& IntimeQuotSvrPort);
};
