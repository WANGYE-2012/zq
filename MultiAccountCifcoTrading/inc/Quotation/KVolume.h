#pragma once
#include "stdafx.h"
#include "wx/defs.h"

#include "wx/window.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


#include <time.h>
#include "wx\msw\font.h"
#include "../inc/Quotation/KCtrl.h"
#include "../inc/Quotation/FData.h"



class   CKVolume : public CKCtrl
{
public:
	 CKVolume(void);
	 ~CKVolume(void);
private:
	wxColour  m_clBackGround;           //����ɫ
	wxColour  m_clBorder;               //�߿�ɫ

	wxColour	m_clRedLine;
	wxColour	m_clGreenLine;
	wxColour	m_cldashLine;
	wxColour	m_clCurSelLine;
	wxColour	m_clLineName;
	wxColour	m_clTipsBk;

	wxColour		m_clTrangleUp;			//��ͼ�õģ����������ε���ɫ
	wxColour		m_clTrangleUpBorder;    //�߿���ɫ
	wxColour		m_clTrangleDown;		//��ͼ�õģ����������ε���ɫ
	wxColour		m_clTrangleDownBorder;  //�߿���ɫ
	

public:
	bool InitCfg(TiXmlElement *root);
	
public://configר��
	void SetBackGround(unsigned long lValue);
	void SetBorder(unsigned long lValue);
	void SetRedLine(unsigned long lValue);
	void SetGreenLine(unsigned long lValue);
	void SetdashLine(unsigned long lValue);
	void SetCurSelLine(unsigned long lValue);
	void SetTipsBk(unsigned long lValue);
	void SetLineName(unsigned long lValue);
	void SetTrangleUp(unsigned long lValue);
	void SetTrangleDown(unsigned long lValue);
	void SetLineColor(int nIndex, unsigned long lValue);
	//void SetLeftSpanWidth(int nWidth);//�������ҿ̶����Ŀ��
 private:
    DECLARE_DYNAMIC_CLASS (CKVolume)
    DECLARE_EVENT_TABLE()

	void CalcMaxMin();

	void OnPaint (wxPaintEvent & PaintEvent);
	void DrawDC(wxDC& dc);
	void DrawHead(wxDC& dc);
	void DrawBorder(wxDC& dc, wxRect& rt);
	void DrawItems(wxDC& dc);
	void DrawOneItem(wxDC& dc, int nLeft, int nRight, int nTop1, bool IsZhang);
	void DrawScales(wxDC& dc);
	void DrawZuiGaoZuiDiJia(wxDC& dc);
	void DrawCurSelLine(wxDC& dc);
	void DrawTips(wxDC& dc);

private://ͼ��ͼ�õ�
	void				DrawMarks(wxDC& dc);
	
//	void OnLeftDown(wxMouseEvent& event);
//	void OnLeftUp(wxMouseEvent& event);
//	void OnRightDown(wxMouseEvent& event);	
//	void OnRightUp(wxMouseEvent& event);

public:
	wxColour m_clLineColor[COLOR_MAX];

};
