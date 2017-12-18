#pragma once
#include "wx/defs.h"

#include "wx/window.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
//#include "KItemData.h"

#include <time.h>
#include "wx\msw\font.h"
#include "../inc/Quotation/FData.h"

DECLARE_EVENT_TYPE(wxEVT_KCtrl_KeyDown, 3000)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_LBUTTONDOWN, 3001)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_MOUSEMOVE, 3002)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_RBUTTONUP, 3003)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_MOUSELEAVE, 3004)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_LEFTDBCLICK, 3005)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_MARKSCHANGE, 3009)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_MOUSEENTER, 3010)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_RBUTTONDOWN, 3011)
DECLARE_EVENT_TYPE(wxEVT_KCtrl_MOUSEMOVEHOOK, 3012)





class   CKCtrl : public wxPanel
{
public:
	 CKCtrl(void);
	 ~CKCtrl(void);

bool Create(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER|wxWANTS_CHARS|wxCLIP_CHILDREN,
		const wxString& name = wxPanelNameStr);


protected:
	vector<SKLine *> m_vecKLine;        //�������Զ������
	wxColour  m_clBackGround;           //����ɫ
	wxColour  m_clBorder;               //�߿�ɫ

	wxColour	m_clRedLine;
	wxColour	m_clGreenLine;
	wxColour	m_cldashLine;
	wxColour	m_clCurSelLine;

	wxColour	m_clTipsBk;
	


	

	int       m_nBorderWidth;           //�߿���
	
	int		  m_nShowCount;//һ����ʾ���ٸ�k��
	double	  m_dbDayWidth;//���߿��
	int		  m_nDaySpan;//���߼��
	int       m_nBegin;//��ʼ����
	int		  m_nEnd;//�������������������
	int		  m_nCurSel;

	double	  m_fMaxValue;
	double	  m_fMinValue;
	wxRect	  m_RectOffSet;

	int		  m_nMaxPos;
	int		  m_nMinPos;//�����������ͼ�

	SItem	  m_sItem;
	UINT	  m_uTimerarr[2];
	BOOL	  m_bMaxMinChange;
	wxTimer	  m_timer1;
	wxTimer	  m_timer2;

	int		   m_nLeftPos;
	int		   m_nRightPos;

	wxWindow   *m_pMsgWnd;

	bool		m_bIsMouse;    //�����Ϣ
	bool		m_bIsKeyBoard; //������Ϣ

	bool		m_bShowHeight;
	bool		m_bShowTime;

	EnumPhrase		 m_enumPhrase;   //ʱ������


//KLine���õ�
	wxColour	m_clLineName;//�ߵ�����

	wxPoint		m_ptMouseMoveLast;
public:
	void SetFData(CFData *pFData);
	BOOL SetVectorData(vector<SKLine *>& vecKLine);
	void SetPhrase(EnumPhrase enumPhrase); 
	EnumPhrase  GetPhrase();
	virtual void SetCursel(SItem& sItem);
	void SetMsgWnd(wxWindow *pMsgWnd);
	void SetMarkType(EnumMark enumMark);
	void SetMarks();	
	virtual BOOL SetVectorData_InTime(vector<SKLine *>& vecKLine); //���µ�ʵʱ����

	void SetShowCross(bool bShowCross);
	bool GetShowCross(){ return m_bShowCross; };
	void SetMouseIn(bool bMouseIn);
	virtual bool InitCfg(TiXmlElement *root);

	wxString    GetTechIndexName();
public://configר��
	void SetBackGround(unsigned long lValue);
	void SetBorder(unsigned long lValue);
	void SetRedLine(unsigned long lValue);
	void SetGreenLine(unsigned long lValue);
	void SetdashLine(unsigned long lValue);
	void SetCurSelLine(unsigned long lValue);
	void SetTipsBk(unsigned long lValue);
	void SetLineName(unsigned long lValue);
	void SetLeftpanelName(unsigned long lValue);
	void SetLeftpanelTime(unsigned long lValue);
	void SetLeftpanelzhang(unsigned long lValue);
	void SetLeftpaneldie(unsigned long lValue);
	void SetLeftpanelhengpan(unsigned long lValue);
	void SetTrangleUp(unsigned long lValue);
	void SetTrangleDown(unsigned long lValue);
	void SetLineColor(int nIndex, unsigned long lValue);
	void SetLeftSpanWidth(int nWidth);//�������ҿ̶����Ŀ��

 protected:
    DECLARE_DYNAMIC_CLASS (CKCtrl)
    DECLARE_EVENT_TABLE()

	void	OnTimer1(wxTimerEvent& event);
	virtual int		JiaGe2GaoDu(float fJiaGe);
	virtual float	GaoDu2JiaGe(int nHeight);
	virtual int		WeiZhi2XuHao(int nWidthPos);
	virtual float   XuHao2WeiZhi(int nItem, int& nLeft, int& nRight);

	int  	time2WeiZhi(struct tm tmTime, int& nLeft, int& nRight);
	virtual void	GetBeginEnd();
	virtual void    CalcMaxMin();
	void OnChar( int nkeyCode );
	bool GetShowCount(int  nMax);//nMax 1:�Ŵ�; -1:��С;0���ֲ���
	void SItemGetValue(bool	bIsMouse, bool bIsKeyBoard);

	void OnPaint (wxPaintEvent & PaintEvent);
	void DrawDC(wxDC& dc);
	void DrawHead(wxDC& dc);
	void DrawBorder(wxDC& dc, wxRect& rt);
	void DrawItems(wxDC& dc);
	void DrawOneItem(wxDC& dc, int nLeft, int nRight, int nTop1, int nTop2, int nTop3, int nTop4);
	void DrawScales(wxDC& dc);
	void DrawZuiGaoZuiDiJia(wxDC& dc);
	void DrawCurSelLine(wxDC& dc);
	void DrawTips(wxDC& dc);
	
	virtual void DrawTips_Time(wxDC& dc, wxRect rtClient, int nX);
	void DrawOneItemData(wxDC& dc, wxRect rtClient);//����߼۸���ʾ�򣬲�������k��ģ�����滭����ʵ���Ӧ�õ���һ��ģ��
	bool m_bShowCross;			//��ʾʮ�ֹ��
	bool m_bMouseIn;			//����ǲ���������
	
	
	void OnLeftDown(wxMouseEvent& event);
	void OnLeftDBClick(wxMouseEvent& event);	
	void OnRightDown(wxMouseEvent& event);	
	void OnLeftUp(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent &event);
	void OnMouseLeave(wxMouseEvent &event);
	void OnMouseEnter(wxMouseEvent &event);
public:	
	void DoMouseLeave();
protected:
	void OnSize(wxSizeEvent& event);
	void OnRightUp(wxMouseEvent& event);
	


	void timeToSting(time_t time1, char *szTime);

	void GetLineName(wxString& strLineName);
	

protected://������ɫ������Ͻ���ʾ��ר��
	wxColour m_LeftpanelName; 
	wxColour m_LeftpanelTime; //ʱ��
	wxColour m_Leftpanelzhang;  //��
	wxColour m_Leftpaneldie;    //��
	wxColour m_Leftpanelhengpan;//����

	void SetTextColor(wxDC& dc, double dbClosePrice);
	void GetTimeText(EnumPhrase	 enumPhrase, wxString& strText, wxString& strText2);

	int  m_nLeftPanelHeight;

protected://ͼ��ͼ�õ�
	EnumMark			m_enumMark;
//	MAPsmark			m_mapMarks;
	int					m_KType;//�����ĸ�ģ�������ע��
	void				SetKType(int nKType);//���������Ҫ���ڽ����ͬ����ģ����ֲ�ͬ��ע�����⣻������������ָ��ģ�飬���ֱ�ע�Ķ�λ

	virtual void DrawMarks(wxDC& dc);

	void DrawTriangleUp(wxDC& dc, struct tm tm1, double dbPrice, bool bSelected);
	void DrawTriangleDown(wxDC& dc, struct tm tm1, double dbPrice, bool bSelected);

	wxColour		m_clTrangleUp;			//��ͼ�õģ����������ε���ɫ
	wxColour		m_clTrangleUpBorder;    //�߿���ɫ
	wxColour		m_clTrangleDown;		//��ͼ�õģ����������ε���ɫ
	wxColour		m_clTrangleDownBorder;  //�߿���ɫ


	public:
		virtual void DoMouseMove(wxPoint mousePos);
		void RefreshTip();

	WXLRESULT MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam);


	CFData		*m_pFData;//��ֻ�ڻ���������
	int			m_nNumDigits;

	CTechIndex* m_pTechIndex;
	wxString    m_strTechIndexName;


public:
	void DrawTechIndex(wxDC& dc);//������ָ��
	wxColour m_clLineColor[COLOR_MAX];

	void OnConfigChange(wxCommandEvent& event);
	void SetTechIndex(wxString strTechIndex);


};
