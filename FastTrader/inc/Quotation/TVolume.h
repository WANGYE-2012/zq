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
#include "../inc/Quotation/FData.h"



class   CTVolume : public CTCtrl
{
public:
	 CTVolume(void);
	 ~CTVolume(void);


private:
	wxColour			m_clBackGround; //背景色
	wxColour			m_clBorder;               //边框色
	wxColour			m_cldashLine;
	wxColour			m_clPrice;
	wxColour			m_clAveragePrice;
	wxColour			m_clCurSelLine;
	wxColour			m_clTipsBk;

	wxColour			m_clName; 
	wxColour			m_clTime; //时间
	wxColour			m_zhang;  //涨
	wxColour			m_die;    //跌
	wxColour			m_hengpan;//横盘
	wxColour			m_clLineName;//线的性质

private:
	void		CalcMaxMin();
public:
	bool InitCfg(TiXmlElement *root);

public://config专用
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
	void SetSpanLine(int nLineType, int	nLineSize);
	void SetHourLineLine(int nLineType, int	nLineSize);
	void SetHalfHourLine(int nLineType, int	nLineSize);
private:
	DECLARE_DYNAMIC_CLASS (CTLine)
	DECLARE_EVENT_TABLE()
	void DrawDC(wxDC& dc);
	void OnPaint (wxPaintEvent & PaintEvent);

	void DrawBorder(wxDC& dc, wxRect& rt);
	void DrawScales(wxDC& dc);
	void DrawHead(wxDC& dc);
	void DrawVolumeLine(wxDC& dc);
	void SetTextColor(wxDC& dc, double dbClosePrice);
	void DrawCurSelLine(wxDC& dc);
	void DrawTips(wxDC& dc);

	void DrawTips_Time(wxDC& dc, wxRect rtClient, int nX, bool bIsMouse);


private:
	int         m_nSpanLineType;		//大时间间隔线类型 0：点划线；1：实体线
	int			m_nSpanLineSize;		//大时间间隔线宽度
	int         m_nHourLineType;		//小时间间隔线类型 0：点划线；1：实体线
	int			m_nHourLineSize;		//小时间间隔线宽度
	int         m_nHalfHourLineLineType;//半小时间间隔线类型 0：点划线；1：实体线
	int			m_nHalfHourLineSize;	//半小时间间隔线宽度

};
