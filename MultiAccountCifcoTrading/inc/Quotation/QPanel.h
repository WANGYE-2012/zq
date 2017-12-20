#pragma once
#include "wx\msw\dialog.h"
#include "KSplitterWindow.h"
#include <string>
#include "../inc/Quotation/CritSec.h"
#include "../inc/Quotation/AutoLock.h"
#include "..\\inc\\ISvr\\BusinessData.hpp"
#include "../inc/Quotation/FData.h"
#include "../inc/Quotation/GridEx.h"
#define HQDANGSHU  10  //���ֵ��Ҫ���ģ�����Ķ�����Ҫ��Ӧ����ͬ�����ģ���ֹ����Խ��
DECLARE_EVENT_TYPE(wxEVT_QPanelData_InTime, 3800)
class CQPanel :public wxPanel
{
public:
	CQPanel();

	wxWindow* CreateDlg(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER|wxCLIP_CHILDREN,
		const wxString& name = wxPanelNameStr);


	~CQPanel(void);

	DECLARE_DYNAMIC_CLASS (CQPanel)
	 
	DECLARE_EVENT_TABLE()

	
	void OnPaint (wxPaintEvent & PaintEvent);
	void OnSize(wxSizeEvent& event);
	void DrawBorder(wxDC& dc);
	int  DrawBaoJia(wxDC& dc);
	void DrawDetail(wxDC& dc, int YBegin);
	WXLRESULT MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam); 
	void OnChar( int nkeyCode );
	void OnLeftDBClick(wxMouseEvent& event);
	void OnFuncKeyDown(wxCommandEvent& event);
public:
	

private:
	wxColour			 m_clBackGround; //����ɫ
	wxColour			 m_clBorder;

	wxColour			 m_Leftpanelzhang;  //��
	wxColour			 m_Leftpaneldie;    //��
	wxColour			 m_Leftpanelhengpan;//����

	wxColour			 m_clText;//
	wxColour			 m_clVolume;//������ɫ
	double				 m_dbLastdbPrice;//������
	int					 m_nXianShou;//����
	wxString		     m_strKaiPing;
public:
	void Init();
	void SetFuture(string ExchangeName,string ContractName,string ContractDate);	//ĳֻ�ڻ�������
	void SetTextColor(wxDC& dc, double dbClosePrice);
	void SetInTimeData(PlatformStru_DepthMarketData& MarketDataInfo);

	void SetFData(CFData* pFData);
private:
	void GetTextDefaultValue(wxString& strText, double dbValue, double dbDefault);
	void GetTextDefaultValue_Volume(wxString& strText, double dbValue, double dbDefault);
	double GetAveragePrice();

private:
	string				m_ExchangeName; //����������
	string				m_ContractName; //��Լ����
	string				m_ContractDate; //��Լ����

	wxColour			m_clContractName;//��Լ������ɫ


	
	PlatformStru_DepthMarketData m_MarketDataInfo;//������Ϣ

	
	
	CFData*				m_pFData;
	

//////////////////////////////////////////////////////////////////////////
//������TLine��أ�������KLine���
//////////////////////////////////////////////////////////////////////////


public://grid���
	wxGridEx*					m_grid;
	BigGridTable*				m_table;

	vector<SRow>				m_vecRow;
	//map<int,vector<string>>*	m_pTextData;
	wxColour					m_clGridLabelText;//
	wxColour					m_clGridLabelBK;//

	int							m_nHeadFontSize;
	int							m_nDetailFontSize;

	int							m_nGridHeadFontSize;

	SQIntime *m_pItemLastShow;
	void SetGridLastItem();

	void InitGrid();

public:
	bool InitCfg(TiXmlElement *root);
	void SetHQCount(int nHQCount);
	void SetBackGround(unsigned long lValue);
	void SetBorder(unsigned long lValue);
	void SetLeftpanelzhang(unsigned long lValue);
	void SetLeftpaneldie(unsigned long lValue);
	void SetLeftpanelhengpan(unsigned long lValue);
	void SetContractName(unsigned long lValue);
	void SetclText(unsigned long lValue);
	void SetclVolume(unsigned long lValue);
	void SetclGridLabelText(unsigned long lValue);
	void SetHeadFontSize(int nHeadFontSize);
	void SetDetailFontSize(int nDetailFontSize);
	void SetGridHeadFontSize(unsigned long lValue);
private:
	void GetGridSize(wxPoint& pt, wxSize& rt);
	
	void SetOneItem(SQIntime *pItemLast, SQIntime *pItemLast2,int nRow, bool bRefresh = true);
	void OnFuncQPanelData_InTime(wxCommandEvent& event);

	bool m_bShowVisible;

	int				   m_nNumDigits;//��Ч����
	void GetDescName(int nDesc, wxString& strDesc);

	int		m_nHQCount;					//�嵵���飬����嵵,����һ��
	double	m_BidPrice[HQDANGSHU];		//�����ʮ������
	int		m_BidVolume[HQDANGSHU];	//�����ʮ������
	double	m_AskPrice[HQDANGSHU];		//������ʮ������
	int		m_AskVolume[HQDANGSHU];	//������ʮ������




};
