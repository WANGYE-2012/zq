#pragma once
#include "stdafx.h"
#include "wx/defs.h"

#include "wx/window.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


#include <time.h>
#include "wx\msw\font.h"
#include "../inc/Quotation/TCtrl.h"


class   CTScales : public CTCtrl
{
public:
	 CTScales(void);
	 ~CTScales(void);



private:
	wxColour			m_clBackGround; //����ɫ
	wxColour			m_clBorder;               //�߿�ɫ
	wxColour			m_cldashLine;
	wxColour			m_clPrice;
	wxColour			m_clAveragePrice;
	wxColour			m_clCurSelLine;
	wxColour			m_clTipsBk;

	wxColour			m_clName; 
	wxColour			m_clTime; //ʱ��
	wxColour			m_zhang;  //��
	wxColour			m_die;    //��
	wxColour			m_hengpan;//����
	wxColour			m_clLineName;//�ߵ�����

private:
	void		CalcMaxMin();
public:
	bool InitCfg(TiXmlElement *root);	

public://configר��
	void SetBackGround(unsigned long lValue);
	void SetBorder(unsigned long lValue);
	void SetdashLine(unsigned long lValue);
	void SetPriceColor(unsigned long lValue);
	void SetAveragePriceColor(unsigned long lValue);
	void SetCurSelLineColor(unsigned long lValue);
	void SetTipsBkColor(unsigned long lValue);
	void SetNameColor(unsigned long lValue);
	void SetTimeColor(unsigned long lValue);
	void SetzhangColor(unsigned long lValue);
	void SetdieColor(unsigned long lValue);
	void SethengpanColor(unsigned long lValue);
	void SetLineNameColor(unsigned long lValue);
private:
	DECLARE_DYNAMIC_CLASS (CTLine)
	DECLARE_EVENT_TABLE()
	void DrawDC(wxDC& dc);
	void OnPaint (wxPaintEvent & PaintEvent);

	void DrawBorder(wxDC& dc, wxRect& rt);
	void DrawScales(wxDC& dc);
	void SetTextColor(wxDC& dc, double dbClosePrice);
	void DrawCurSelLine(wxDC& dc);
	void DrawTips(wxDC& dc);

	void DrawTips_Time(wxDC& dc, wxRect rtClient, int nX, bool bIsMouse);

};
