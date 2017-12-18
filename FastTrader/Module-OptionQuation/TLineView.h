#pragma once
#include "FData.h"

enum enumType
{
	enumHead =1, //��ʱͼͷ��
	enumTLine,   //��ʱͼ
	enumTVolume, //��ʱͼ����
	enumScales,  //��ʱͼ�̶�
	enumLeftBar, //��ʱͼ�����Ϣ��

	enumSplitBar,//�ָ���
};
struct SHitTest
{
	enumType	eType;	
	int			nIndex; //����ָ��������ǵڼ����ָ���
};

class __declspec(dllexport) CTLineView : public CWnd
{
	DECLARE_DYNAMIC(CTLineView)

public:
	CTLineView();
	virtual ~CTLineView();
	BOOL Create(CRect rtClient, CWnd* pParentWnd, UINT nID);

	CBitmap				m_Bmp;

	COLORREF			m_clBackGround;
	COLORREF			m_clBorder;					//�߿�ɫ
	COLORREF			m_cldashLine;
	COLORREF			m_clPrice;
	COLORREF			m_clAveragePrice;
	COLORREF			m_clCurSelLine;
	COLORREF			m_clTipsBk;

	COLORREF			m_clName; 
	COLORREF			m_clTime;					//ʱ��
	COLORREF			m_zhang;					//��
	COLORREF			m_die;						//��
	COLORREF			m_hengpan;					//����
	COLORREF			m_clLineName;				//�ߵ�����

	int					m_nBorderWidth;				//�߿���
	vector<STLine>		m_vecTLine;
	CRect				m_RectOffSet;


	string				m_ExchangeName;			//����������
	string				m_ContractName;			//��Լ����
	int					m_nShowCount;//һ����ʾ���ٸ�k��
	double				m_dbDayWidth;//���߿��
	int					m_nDaySpan;//���߼��
	int					m_nBegin;				//��ʼ��ֵҪ��
	int					m_nEnd;					//������ֵС

	double				m_fLastClosePrice;		//�������̼�
	double				m_fMaxValue;            //�������ֵ
	double				m_fMinValue;            //������Сֵ
	int					m_nDayCount;            
	int					m_nOneDayScalesCount;
	vector<struct tm>	m_vecTimeScales;		//С��������
	vector<DWORD>		m_vecTMWidthSpan;
	DWORD				m_TMWidthCount;			//����ʱ��̶��ܹ���С

	int					m_nLeftPanelHeight;    //�����Ϣ��ĸ߶�
	double				m_dbVolumeMultiple;    
	int					m_nNumDigits;          //������Ч����
 

	HFONT				m_hSysFontHead;//ͷ������
	CFont				m_FontHead;

	HFONT				m_hSysFont1;//���Ҳ�̶�����
	CFont				m_FontScales;

	HFONT				m_hSysFontTip;//��ʾ������
	CFont				m_FontTip;

	HFONT				m_hSysFontLeft;//�����Ϣ������
	CFont				m_FontLeft;    //������ֵ�����

	HFONT				m_hSysFontBottomScales;//�ײ��̶�����
	CFont				m_FontBottomScales;

	CPoint				m_ptMouseMoveLast; //�������ƶ���λ��
	bool				m_bIsMouse;    //�����Ϣ
	bool				m_bIsKeyBoard; //������Ϣ

	bool				m_bShowHeight;   //��ʾ�߶ȵ�tip
	bool				m_bShowTime;     //��ʾʱ�����tip
	int					m_nCurSel;		 //��ǰѡ�е���Ŀ��
	bool				m_bShowCross;			//��ʾʮ�ֹ��
	bool				m_bMouseIn;			//����ǲ���������

	UINT				m_uTimerarr[2];

	STItem				m_sItem;
	int					m_nTViewCount;		//������ͼ
	CRect				m_rtTLine;			//��ʱͼ�߾�������
	CRect				m_rtTVolume;			//��ʱͼ���ܾ�������
	CRect				m_rtScales;			//��ʱͼ�̶ȳ߾�������
public:	
	void SetMaxMinValue(double& dbMaxValue, double& dbMinValue);
	void SetScales(vector<struct tm>& vecScales);
	void SetLastClosePrice(double dbPrice);
	void SetVolumeMultiple(double dbVolumeMultiple);
	void SetFuture(string ExchangeName,string ContractName);
	BOOL SetVectorData(vector<STLine>& vec);
protected:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	void OnToKLine();

	//���»��Ʒ�ʱͼ����
	void	DrawBitmap_TLine(CDC *pDC);
	void    DrawBorder(CDC *pDC);
	void	DrawScales(CDC* pDC);
	void	DrawHead(CDC* pDC);
	void	DrawPriceLines(CDC* pDC);
	bool	DrawPriceLines_NoData(CDC* pDC, STLine& pTLineEnd1, STLine& pTLineBegin1);
	void	DrawAveragePriceLines(CDC* pDC);
	void	DrawCurSelLine(CDC* pDC);
	void	DrawTips(CDC* pDC);
	void	DrawTips_Time(CDC* pDC, CRect rtClient, int nX, bool bIsMouse);
	void	DrawOneItemData(CDC *pDC,  CRect rtClient);
	void	SetTextColor(CDC* pDC, double dbClosePrice);
	void	DrawCommonLast(CDC* pDC);

	void		CalcMaxMin();
	void		GetBeginEnd();
	int			Time2X(struct tm tmTime);
	struct tm	X2Time(int nx);
	int			WeiZhi2XuHao(int nx);
	int			XuHao2WeiZhi(int nItem);
	int			JiaGe2GaoDu(double fJiaGe);
	double		GaoDu2JiaGe(int nHeight);
	void		DateTime2Tm(COleDateTime dt ,struct tm& tmDt);
	int			GetDrawScalesCount(CDC* pDC);
	void		SItemGetValue(bool	bIsMouse, bool bIsKeyBoard);

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

//����TVolume��غ����ͱ���
//////////////////////////////////////////////////////////////////////////
	void		DrawBitmap_TVolume(CDC *pDC);		
	void		DrawScales_TVolume(CDC* pDC);
	void		DrawVolumeLine_TVolume(CDC* pDC);

	void		CalcMaxMin_TVolume();
	int			JiaGe2GaoDu_TVolume(double fJiaGe);
	double		GaoDu2JiaGe_TVolume(int nHeight);
	double		m_fMaxValue_TVolume;
	double		m_fMinValue_TVolume;

//////////////////////////////////////////////////////////////////////////
//�̶�
	void		DrawBitmap_TScales(CDC* pDC);
	struct tm	GetDrawTime(vector<struct tm>& vecTimeScales, vector<DWORD>& vecTMWidthSpan, DWORD dwSpan);


 //////////////////////////////////////////////////////////////////////////
	bool					HitTest(CPoint pt, SHitTest& hitTest);
	eMouseModes				m_MouseMode;
	bool					m_bLbuttonDown;
	CPoint					m_LbuttonDownPoint; 
	int						m_nResizeRange; //2������֮�ڿ�ʼ�������״
	int						m_nMaxSpan;     //������СΪ5������
	double					m_dbTLine;
	double					m_dbTVolume;
	void					OnView1();
	void					OnView2();

public:
	BOOL					SetInTimeData(CFData& fData);


};


