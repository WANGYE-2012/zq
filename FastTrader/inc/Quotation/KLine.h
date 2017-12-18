#pragma once
#include "wx/defs.h"

#include "wx/window.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
//#include "KItemData.h"

#include <time.h>
#include "wx\msw\font.h"
#include "../inc/Quotation/KCtrl.h"
#include "../inc/Quotation/FData.h"





class   CKLine : public CKCtrl
{
public:
	 CKLine(void);
	 ~CKLine(void);

private:
	//vector<SKLine *> m_vecKLine;        //�������Զ������
	wxColour  m_clBackGround;           //����ɫ
	wxColour  m_clBorder;               //�߿�ɫ
	wxColour	m_clRedLine;
	wxColour	m_clGreenLine;
	wxColour	m_cldashLine;
	wxColour	m_clCurSelLine;
	wxColour	m_clTipsBk;
	wxColour	m_clLineName;//�ߵ�����
	wxColour    m_clSAR;//SAR����ָ�����ɫ

public:	
	bool InitCfg(TiXmlElement *root);
	virtual void SetCursel(SItem& sItem);
	virtual void DoMouseMove(wxPoint mousePos);
public://configר��
	void SetBackGround(unsigned long lValue);
	void SetBorder(unsigned long lValue);
	void SetRedLine(unsigned long lValue);
	void SetGreenLine(unsigned long lValue);
	void SetdashLine(unsigned long lValue);
	void SetCurSelLine(unsigned long lValue);
	void SetTipsBk(unsigned long lValue);
	void SetLineName(unsigned long lValue);
	void SetSARName(unsigned long lValue);
	void SetLeftpanelName(unsigned long lValue);
	void SetLeftpanelTime(unsigned long lValue);
	void SetLeftpanelzhang(unsigned long lValue);
	void SetLeftpaneldie(unsigned long lValue);
	void SetLeftpanelhengpan(unsigned long lValue);
	void SetTrangleUp(unsigned long lValue);
	void SetTrangleDown(unsigned long lValue);
	void SetLineColor(int nIndex, unsigned long lValue);

 private:
    DECLARE_DYNAMIC_CLASS (CKLine)
    DECLARE_EVENT_TABLE()


	virtual void CalcMaxMin();
	

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
	
	void DrawOneItemData(wxDC& dc, wxRect rtClient);//����߼۸���ʾ�򣬲�������k��ģ�����滭����ʵ���Ӧ�õ���һ��ģ��
	void DrawSAR(wxDC& dc);	
private:
	void GetLineName(wxString& strLineName);
	

private://������ɫ������Ͻ���ʾ��ר��
	wxColour m_LeftpanelName; 
	wxColour m_LeftpanelTime; //ʱ��
	wxColour m_Leftpanelzhang;  //��
	wxColour m_Leftpaneldie;    //��
	wxColour m_Leftpanelhengpan;//����

	void SetTextColor(wxDC& dc, double dbClosePrice);
	void GetTimeText(EnumPhrase	 enumPhrase, wxString& strText, wxString& strText2);

	int  m_nLeftPanelHeight;

private://ͼ��ͼ�õ�
	virtual void DrawMarks(wxDC& dc);
	wxColour		m_clTrangleUp;			//��ͼ�õģ����������ε���ɫ
	wxColour		m_clTrangleUpBorder;    //�߿���ɫ
	wxColour		m_clTrangleDown;		//��ͼ�õģ����������ε���ɫ
	wxColour		m_clTrangleDownBorder;  //�߿���ɫ

public:
	wxColour m_clLineColor[COLOR_MAX];


};
