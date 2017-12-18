#pragma once
#include "afxwin.h"
#define  TECHINDEX_MAX 2

// CKLineView
#include "FData.h"
#include "TechIndex.h"
enum enumKType
{	
	enumKLine=2,   //k��ͼ
	enumKVolume, //k������
	enumKScales,  //k�߿̶�
	enumKLeftBar, //k�������Ϣ��

	enumKSplitBar,//�ָ���
};
struct SHitTestKView
{
	enumKType	eType;	
	int			nIndex; //����ָ��������ǵڼ����ָ���
};
class __declspec(dllexport)  CKLineView : public CWnd
{
	DECLARE_DYNAMIC(CKLineView)

public:
	CKLineView();
	virtual ~CKLineView();
	BOOL Create(CRect rtClient, CWnd* pParentWnd, UINT nID);

	std::string    m_ExchangeName;
	std::string    m_ContractName;
	
	vector<SKLine> m_vecKLine;        //�������Զ������

	CBitmap			m_Bmp;
	
	COLORREF		m_clBackGround;           //����ɫ
	COLORREF		m_clBorder;               //�߿�ɫ

	COLORREF		m_clRedLine;
	COLORREF		m_clGreenLine;
	COLORREF		m_cldashLine;
	COLORREF		m_clCurSelLine;

	COLORREF		m_clTipsBk;
	COLORREF		m_clSAR;//SAR����ָ�����ɫ
	COLORREF		m_clTriangle;
	COLORREF		m_clText;

	HFONT				m_hSysFontHead;//ͷ������
	CFont				m_FontHead;

	HFONT				m_hSysFontTip;//��ʾ������
	CFont				m_FontTip;


	HFONT				m_hSysFontLeft;//�����Ϣ������
	CFont				m_FontLeft;    //������ֵ�����

	HFONT				m_hSysFont1;//���Ҳ�̶�����
	CFont				m_FontScales;

	HFONT				m_hSysFontDownScalesLeft;//�¿̶ȳ��������
	CFont				m_FontDownScalesLeft;    //�¿̶ȳ��������

	int				m_nBorderWidth;           //�߿���

	int				m_nShowCount;//һ����ʾ���ٸ�k��
	double			m_dbDayWidth;//���߿��
	int				m_nDaySpan;//���߼��
	int				m_nBegin;//��ʼ����
	int				m_nEnd;//�������������������
	int				m_nCurSel;

	double			m_fMaxValue;
	double			m_fMinValue;
	CRect			m_RectOffSet;

	int				m_nMaxPos;
	int				m_nMinPos;//�����������ͼ�


	UINT			m_uTimerarr[2];
	BOOL			m_bMaxMinChange;


	int				m_nLeftPos;
	int				m_nRightPos;

	bool			m_bIsMouse;    //�����Ϣ
	bool			m_bIsKeyBoard; //������Ϣ

	bool			m_bShowHeight;
	bool			m_bShowTime;

	EnumPhrase		 m_enumPhrase;   //ʱ������


	//KLine���õ�
	COLORREF		m_clLineName;//�ߵ�����

	CPoint			m_ptMouseMoveLast;

	bool			m_bShowCross;			//��ʾʮ�ֹ��
	bool			m_bMouseIn;				//����ǲ���������
	int				m_nNumDigits;


	double			m_fMaxValue_KVolume;
	double			m_fMinValue_KVolume;

protected://������ɫ������Ͻ���ʾ��ר��
	COLORREF		m_LeftpanelName; 
	COLORREF		m_LeftpanelTime;	//ʱ��
	COLORREF		m_Leftpanelzhang;	//��
	COLORREF		m_Leftpaneldie;		//��
	COLORREF		m_Leftpanelhengpan;	//����

	void SetTextColor(CDC* pDC, double dbClosePrice);
	void GetTimeText(EnumPhrase	 enumPhrase, CString& strText, CString& strText2);

	int				m_nLeftPanelHeight;

private:
	int		JiaGe2GaoDu(double fJiaGe);
	double	GaoDu2JiaGe(int nHeight);
	int		WeiZhi2XuHao(int nWidthPos);
	float   XuHao2WeiZhi(int nItem, int& nLeft, int& nRight);

	int  	time2WeiZhi(struct tm tmTime, int& nLeft, int& nRight);
	virtual void	GetBeginEnd();
	virtual void    CalcMaxMin();
	void OnChar( int nkeyCode );
	bool GetShowCount(int  nMax);//nMax 1:�Ŵ�; -1:��С;0���ֲ���
	void GetLineName(CString& strLineName);

	void	CalcMaxMin_Volume();
	int		JiaGe2GaoDu_Volume(double fJiaGe);
	double	GaoDu2JiaGe_Volume(int nHeight);

	CString GetWeekName(int week);
protected:
	void OnPaint();
	void DrawBitmap(CDC *pDC);
	void DrawScales(CDC *pDC);
	void DrawBorder(CDC *pDC);
	void DrawHead(CDC *pDC);
	void DrawItems(CDC *pDC);
	void DrawOneItem(CDC *pDC, int nLeft, int nRight, int nTop1, int nTop2, int nTop3, int nTop4);
	void DrawZuiGaoZuiDiJia(CDC *pDC);
	void DrawCommonLast(CDC* pDC);
	void DrawCurSelLine(CDC *pDC);
	void DrawTips(CDC *pDC);
	void DrawTips_Time(CDC* pDC, CRect rtClient, int nX);
	void DrawOneItemData(CDC* pDC,  CRect rtClient);

	//������
	void DrawBitmap_Volume(CDC *pDC);
	void DrawHead_Volume(CDC *pDC);
	void DrawScales_Volume(CDC *pDC);
	void DrawItems_Volume(CDC* pDC);
	void DrawOneItem_Volume(CDC* pDC, int nLeft, int nRight, int nTop1, bool IsZhang);
	//���̶ȳ�
	void DrawBitmap_Scales(CDC *pDC);
	void DrawLeftText(CDC *pDC);
	void DrawItems_Scales(CDC *pDC);
	void DrawItems_Day(CDC *pDC);
	void DrawItems_MINS(CDC *pDC);
	void ProcessMins(CDC *pDC, EnumPhrase enumPhrase, int i, int& nItemSpan, struct tm tm33, struct tm& tmPrevious, int& nFirstEnd);


	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:	
	void SetFuture(string ExchangeName,string ContractName);
	BOOL SetVectorData(vector<SKLine>& vecKLine);
	void SetPhrase(EnumPhrase enumPhrase); 
	EnumPhrase  GetPhrase(){ return m_enumPhrase;};
	void GetFuture(string& ContractName);	
	void SetMarks();	

	void SetShowCross(bool bShowCross);
	bool GetShowCross(){ return m_bShowCross; };
	void SetMouseIn(bool bMouseIn);

	
	int					m_nKViewCount;		//������ͼ
	CRect				m_rtKLine;			//K�߾�������

	CRect				m_rtKVolume;		//K���ܾ�������
	CRect				m_rtScales;			//K�̶ȳ߾�������
	//////////////////////////////////////////////////////////////////////////
	bool					HitTest(CPoint pt, SHitTestKView& hitTest);
	eMouseModes				m_MouseMode;
	bool					m_bLbuttonDown;
	CPoint					m_LbuttonDownPoint; 
	int						m_nResizeRange; //2������֮�ڿ�ʼ�������״
	int						m_nMaxSpan;     //������СΪ5������
	double					m_dbKLine;
	double					m_dbKVolume;

	void					OnView1();
	void					OnView2();

public:
	BOOL					SetInTimeData(CFData& fData);

	void OnRButtonUp(UINT nFlags, CPoint point);
	void OnToKLine();
	void OnToK1();
	void OnToK5();
	void OnToK15();
	void OnToK30();
	void OnToK60();
	void OnToKDay();
	void OnToKWeek();
	void OnToKMonth();
	void OnIndexExpress();
	void OnIndexConfig();

public:
	CTechIndex  m_pTechIndex[TECHINDEX_MAX];
	CString    m_strTechIndexName[TECHINDEX_MAX];

public:
	bool GetTechIndexNames(std::vector<std::string>& vecTechNames,  EnumPhrase& enumPhrase);
public:
	void DrawTechIndex(CDC *pDC);//������ָ��
	COLORREF m_clLineColor[COLOR_MAX];
	void SetTechIndex(CString strTechIndex);
	
	void DrawTechIndex_Volume(CDC *pDC);//������ָ��

	SHitTestKView  m_hitTestLast;
};


