#include "StdAfx.h"

#include "../../../inc/Quotation/QPanel.h"

#include "wx/dcmirror.h"




IMPLEMENT_CLASS(CQPanel, wxPanel)
DEFINE_EVENT_TYPE(wxEVT_QPanelData_InTime)
CQPanel::CQPanel(void)
{
	m_bShowVisible = true;
	m_clBackGround			= wxColour(RGB(0,0,0));
	m_clBorder				= wxColour(RGB(192,0,0));
	
	m_Leftpanelzhang		= wxColour(240, 0, 0);
	m_Leftpaneldie			= wxColour(0, 240, 0);
	m_Leftpanelhengpan		= wxColour(255, 255, 255);


	
	m_clContractName		= wxColour(82, 255, 255);
	m_clInstrumentName		= wxColour(255, 255, 82);
	m_clText				= wxColour(192,192,192);
	m_clVolume				= wxColour(255, 255, 82);

	m_clGridLabelText       = wxColour(192,192,192);
	m_clGridLabelBK			= wxColour(48,48,48);

	m_pFData				= NULL;
	m_grid					= NULL;
	m_nHQCount				= 1;//默认只有一档行情


	m_nHeadFontSize			= 12;
	m_nDetailFontSize		= 10;

	m_nGridHeadFontSize     = 8;
	Init();
	
}
void CQPanel::Init()
{
	m_ContractName  = "-";
	m_dbLastdbPrice = 0;
	m_nXianShou     = 0;
	m_strKaiPing    = "-";

	m_MarketDataInfo.AskPrice1 = 0;	
	m_MarketDataInfo.AskPrice2 = 0;
	m_MarketDataInfo.AskPrice3 = 0;
	m_MarketDataInfo.AskPrice4 = 0;
	m_MarketDataInfo.AskPrice5 = 0;

	m_MarketDataInfo.AskVolume1 = 0;
	m_MarketDataInfo.AskVolume2 = 0;
	m_MarketDataInfo.AskVolume3 = 0;
	m_MarketDataInfo.AskVolume4 = 0;
	m_MarketDataInfo.AskVolume5 = 0;

	m_MarketDataInfo.BidPrice1 = 0;
	m_MarketDataInfo.BidPrice2 = 0;
	m_MarketDataInfo.BidPrice3 = 0;
	m_MarketDataInfo.BidPrice4 = 0;
	m_MarketDataInfo.BidPrice5 = 0;

	m_MarketDataInfo.BidVolume1 =  0;
	m_MarketDataInfo.BidVolume2 = 0;
	m_MarketDataInfo.BidVolume3 = 0;
	m_MarketDataInfo.BidVolume4 = 0;
	m_MarketDataInfo.BidVolume5 = 0;

	m_MarketDataInfo.LastPrice = 0;

	m_MarketDataInfo.OpenPrice		= 0;
	m_MarketDataInfo.HighestPrice	= 0;
	m_MarketDataInfo.LowestPrice	= 0;
	m_MarketDataInfo.Turnover		= 0;

	m_MarketDataInfo.Volume  = 0;

	m_pItemLastShow = NULL;

	for(int i =0; i< HQDANGSHU; i++)
	{
		m_BidPrice[i]	= 0;
		m_BidVolume[i]	=0;
		m_AskPrice[i]	=0;
		m_AskVolume[i]	=0;
	}
}
CQPanel::~CQPanel(void)
{
	

}

BEGIN_EVENT_TABLE(CQPanel, wxPanel)
EVT_PAINT(OnPaint)
EVT_COMMAND(wxID_ANY, wxEVT_QPanelData_InTime, CQPanel::OnFuncQPanelData_InTime)

EVT_LEFT_DCLICK(OnLeftDBClick)
EVT_SIZE(OnSize)



END_EVENT_TABLE()
void CQPanel::SetFData(CFData* pFData)
{
	m_pFData = pFData;
	m_dbLastdbPrice = m_pFData->m_PreSettlementPrice;

	//m_pstuData = &m_pFData->m_stuData;
	m_nNumDigits = m_pFData->GetDotDigits();
	if(m_table)
		m_table->SetNumDigits(m_nNumDigits);

	
}
wxWindow* CQPanel::CreateDlg(wxWindow *parent,
									 wxWindowID winid ,
									 const wxPoint& pos ,
									 const wxSize& size,
									 long style,
									 const wxString& name )
{	


	style |= wxWANTS_CHARS|wxTE_PROCESS_ENTER|wxCLIP_CHILDREN;
	bool bReturn = wxPanel::Create(parent,winid,pos, size,	style|wxCLIP_CHILDREN|wxCLIP_SIBLINGS, name);			
	if(!bReturn)
		return NULL;
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);	
	m_grid = new wxGridEx(this, wxID_ANY, wxPoint(0, 0), wxSize( 400, 300 ));
	m_table = new BigGridTable(0, 6);	

	m_grid->SetTable(m_table, true);
	m_grid->SetColSize(0, 1);
	m_grid->SetColSize(1, 1);
	m_grid->SetColSize(2, 1);
	m_grid->SetColSize(3, 1);
	m_grid->SetColSize(4, 1);
	m_grid->SetColSize(5, 1);
	
	m_grid->SetColLabelValue(0, wxT("分笔"));
	m_grid->SetColLabelValue(1, wxT("价格"));
	m_grid->SetColLabelValue(2, wxT("量"));
	m_grid->SetColLabelValue(3, wxT("开"));
	m_grid->SetColLabelValue(4, wxT("平"));
	m_grid->SetColLabelValue(5, wxT("性质"));
	m_grid->SetDefaultCellBackgroundColour(m_clBackGround);
	m_grid->SetLabelBackgroundColour(m_clGridLabelBK);
	m_grid->SetLabelTextColour(m_clGridLabelText);

	
	m_grid->SetColLabelSize(22);
	m_grid->EnableGridLines(false);
	m_grid->EnableDragRowSize(false);
	m_grid->EnableDragColSize(false);
	m_grid->SetRowLabelSize(0);

	m_grid->SetScrollLineY(18);
	return this;
}
void CQPanel::SetInTimeData(PlatformStru_DepthMarketData& MarketDataInfo)
{
	m_MarketDataInfo = MarketDataInfo;
	m_AskPrice[0] = m_MarketDataInfo.AskPrice1;
	m_AskPrice[1] = m_MarketDataInfo.AskPrice2;
	m_AskPrice[2] = m_MarketDataInfo.AskPrice3;
	m_AskPrice[3] = m_MarketDataInfo.AskPrice4;
	m_AskPrice[4] = m_MarketDataInfo.AskPrice5;
	m_AskPrice[5] = m_MarketDataInfo.AskPrice6;
	m_AskPrice[6] = m_MarketDataInfo.AskPrice7;
	m_AskPrice[7] = m_MarketDataInfo.AskPrice8;
	m_AskPrice[8] = m_MarketDataInfo.AskPrice9;
	m_AskPrice[9] = m_MarketDataInfo.AskPrice10;

	m_AskVolume[0] = m_MarketDataInfo.AskVolume1;
	m_AskVolume[1] = m_MarketDataInfo.AskVolume2;
	m_AskVolume[2] = m_MarketDataInfo.AskVolume3;
	m_AskVolume[3] = m_MarketDataInfo.AskVolume4;
	m_AskVolume[4] = m_MarketDataInfo.AskVolume5;
	m_AskVolume[5] = m_MarketDataInfo.AskVolume6;
	m_AskVolume[6] = m_MarketDataInfo.AskVolume7;
	m_AskVolume[7] = m_MarketDataInfo.AskVolume8;
	m_AskVolume[8] = m_MarketDataInfo.AskVolume9;
	m_AskVolume[9] = m_MarketDataInfo.AskVolume10;

	m_BidPrice[0] = m_MarketDataInfo.BidPrice1;
	m_BidPrice[1] = m_MarketDataInfo.BidPrice2;
	m_BidPrice[2] = m_MarketDataInfo.BidPrice3;
	m_BidPrice[3] = m_MarketDataInfo.BidPrice4;
	m_BidPrice[4] = m_MarketDataInfo.BidPrice5;
	m_BidPrice[5] = m_MarketDataInfo.BidPrice6;
	m_BidPrice[6] = m_MarketDataInfo.BidPrice7;
	m_BidPrice[7] = m_MarketDataInfo.BidPrice8;
	m_BidPrice[8] = m_MarketDataInfo.BidPrice9;
	m_BidPrice[9] = m_MarketDataInfo.BidPrice10;

	m_BidVolume[0] = m_MarketDataInfo.BidVolume1;
	m_BidVolume[1] = m_MarketDataInfo.BidVolume2;
	m_BidVolume[2] = m_MarketDataInfo.BidVolume3;
	m_BidVolume[3] = m_MarketDataInfo.BidVolume4;
	m_BidVolume[4] = m_MarketDataInfo.BidVolume5;
	m_BidVolume[5] = m_MarketDataInfo.BidVolume6;
	m_BidVolume[6] = m_MarketDataInfo.BidVolume7;
	m_BidVolume[7] = m_MarketDataInfo.BidVolume8;
	m_BidVolume[8] = m_MarketDataInfo.BidVolume9;
	m_BidVolume[9] = m_MarketDataInfo.BidVolume10;	
}
void CQPanel::SetFuture(string ExchangeName,string ContractName,string ContractDate, string InstrumentName)
{
	if(m_ExchangeName == ExchangeName
		&& m_ContractName == ContractName
		&& m_ContractDate == ContractDate)
		return;

	m_ExchangeName = ExchangeName;
	m_ContractName = ContractName;
	m_ContractDate = ContractDate;
	m_InstrumentName = InstrumentName;
}

void CQPanel::OnSize(wxSizeEvent& event)
{
	wxRect rtClient = GetClientRect();

	wxPoint pt;
	wxSize rt;
	GetGridSize(pt, rt);
	if(m_grid)
	{
		m_grid->SetPosition(pt);
		m_grid->SetSize(rt);
		int nSizeCount = rt.GetWidth()-27;//1.05:1.6:1.2:1:1:1.15
		
		int nWidth0 = nSizeCount/7.0 *1.05;
		m_grid->SetColSize(0, nWidth0);

		int nWidth1 = nSizeCount/7.0 *1.6;
		m_grid->SetColSize(1, nWidth1);

		int nWidth2 = nSizeCount/7.0 *1.2;
		m_grid->SetColSize(2, nWidth2);

		int nWidth3 = nSizeCount/7.0;
		m_grid->SetColSize(3, nWidth3);

		int nWidth4 = nSizeCount/7.0;
		m_grid->SetColSize(4, nWidth4);

		int nWidth5 = nSizeCount- nWidth0-nWidth1-nWidth2-nWidth3-nWidth4;
		m_grid->SetColSize(5, nWidth5);
	}
}

void CQPanel::OnPaint (wxPaintEvent & PaintEvent)
{
	wxPaintDC dc(this);
	wxMemoryDC memdc;  
	memdc.SetLayoutDirection(dc.GetLayoutDirection());
	wxBitmap bmp;
	wxSize size = GetClientSize();
	bmp.Create(size.GetWidth(), size.GetHeight());
	memdc.SelectObject(bmp);

	size = GetClientSize();
	wxBrush brush(m_clBackGround);
	memdc.SetBrush(brush);
	memdc.DrawRectangle(0, 0, size.x, size.y);

	DrawBorder(memdc);
	int nBottom = DrawBaoJia(memdc);
	DrawDetail(memdc, nBottom);
	dc.Blit(0, 0, size.GetWidth(),size.GetHeight(),&memdc, 0, 0);

}

void CQPanel::DrawBorder(wxDC& dc)
{
	wxSize size = GetClientSize();
	wxRect rt(0, 0, size.x, size.y);
	wxPen penBorder(wxColour(m_clBorder), 1, wxSOLID);
	dc.SetPen(penBorder);
	dc.DrawLine(rt.GetLeft(), rt.GetTop(), rt.GetRight(), rt.GetTop());
	dc.DrawLine(rt.GetLeft(), rt.GetTop(), rt.GetLeft(), rt.GetBottom());
	dc.DrawLine(rt.GetLeft(), rt.GetBottom(), rt.GetRight(), rt.GetBottom());
	dc.DrawLine(rt.GetRight(), rt.GetBottom(), rt.GetRight(), rt.GetTop());
}
int CQPanel::DrawBaoJia(wxDC& dc)
{
	wxRect rtClient = GetClientRect();	
	wxString  strText;
	wxCoord w,h;	
	wxFont wxfont(m_nHeadFontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false); 	
	dc.SetFont(wxfont);	
	strText.Printf(wxT("%s %s"), m_ContractName.c_str(), m_InstrumentName.c_str());	
	dc.GetTextExtent(strText, &w, &h);	
	int nhead = h+1;	
	dc.DrawLine(rtClient.GetLeft(), nhead, rtClient.GetRight(), h+1);

	wxPoint pt;
	pt.x = rtClient.GetLeft() + (rtClient.GetWidth() - w)/2;
	pt.y = rtClient.GetTop() + 1; 
	dc.SetTextForeground(m_clContractName); 
	strText.Printf(wxT("%s "), m_ContractName.c_str());	
	dc.DrawText(strText, pt);

	//strText = m_ContractName;
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = pt.x + w;
	dc.SetTextForeground(m_clInstrumentName); 
	strText.Printf(wxT("%s"), m_InstrumentName.c_str());	
	dc.DrawText(strText, pt);

	wxFont wxfont2(m_nDetailFontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 	
	dc.SetFont(wxfont2);	
	wxPen penBorder(wxColour(m_clBorder), 1, wxSOLID);
	dc.SetPen(penBorder);	

	strText = wxT("卖5");
	dc.GetTextExtent(strText, &w, &h);	   	
	dc.SetTextForeground(m_clText); 

	pt.x = 7;
	pt.y = nhead +1;
	for(int i = 0; i < m_nHQCount; i++)
	{
		strText.Printf(wxT("卖%d"), m_nHQCount - i);
		dc.DrawText(strText, pt);

		if(i != m_nHQCount - 1)
			pt.y = pt.y + h +3;
	}

	pt.y = pt.y +h+1;

	if(m_nHQCount != 1)//如果只有卖一 买一，则不画中间的横线，为的是美观
		dc.DrawLine(rtClient.GetLeft(), pt.y, rtClient.GetRight(),pt.y);

	pt.y = pt.y +1;
	for(int i = 0; i < m_nHQCount; i++)
	{
		strText.Printf(wxT("买%d"), i + 1);
		dc.DrawText(strText, pt);
		
		if(i != m_nHQCount - 1)
			pt.y = pt.y + h +3;
	}

	pt.y = pt.y +h+1;
	dc.DrawLine(rtClient.GetLeft(), pt.y, rtClient.GetRight(),pt.y);	
	//dc.DrawLine(rtClient.GetLeft()+14+w, nhead, rtClient.GetLeft()+14+w, pt.y);//竖线

	int nReturn = pt.y+1;
	//开始画价格和量
	wxFont wxfont3(m_nDetailFontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false); 	
	dc.SetFont(wxfont3);	
	int nMaiPanRight = rtClient.GetLeft() + 14 + w;
		
	pt.y = nhead +1;
	for(int i = 0; i < m_nHQCount; i++)
	{
		GetTextDefaultValue(strText, m_AskPrice[m_nHQCount-i-1], 0.0);
		dc.GetTextExtent(strText, &w, &h);	
		SetTextColor(dc,  m_AskPrice[m_nHQCount-i-1]);
		pt.x = rtClient.GetWidth()/2 -w;
		
		dc.DrawText(strText, pt);
		GetTextDefaultValue_Volume(strText,  m_AskVolume[m_nHQCount-i-1], 0.0);
		dc.SetTextForeground(m_clVolume);
		dc.GetTextExtent(strText, &w, &h);	
		pt.x = rtClient.GetWidth() - w -5 ;
		dc.DrawText(strText, pt);

		if(i != m_nHQCount - 1)
			pt.y = pt.y + h +3;
	}
	
	pt.y = pt.y +h+2;	
	for(int i = 0; i < m_nHQCount; i++)
	{
		GetTextDefaultValue(strText, m_BidPrice[i], 0.0);
		dc.GetTextExtent(strText, &w, &h);	
		SetTextColor(dc, m_BidPrice[i]);
		pt.x = rtClient.GetWidth()/2 -w;
		dc.DrawText(strText, pt);

		GetTextDefaultValue_Volume(strText, m_BidVolume[i], 0.0);
		dc.SetTextForeground(m_clVolume);
		dc.GetTextExtent(strText, &w, &h);	
		pt.x = rtClient.GetWidth() - w -5 ;
		dc.DrawText(strText, pt);

		if(i != m_nHQCount - 1)
			pt.y = pt.y + h +3;
	}


	return nReturn;
}
void CQPanel::SetTextColor(wxDC& dc, double dbClosePrice)
{
	double dbPrevious = m_dbLastdbPrice;
	if(dbClosePrice - dbPrevious > -0.00001 && dbClosePrice - dbPrevious < 0.00001)
		dc.SetTextForeground(m_Leftpanelhengpan); 
	else if(dbClosePrice > dbPrevious)
		dc.SetTextForeground(m_Leftpanelzhang); 
	else if(dbClosePrice < dbPrevious)
		dc.SetTextForeground(m_Leftpaneldie); 
}
void CQPanel::DrawDetail(wxDC& dc, int nYBegin)
{
	wxFont wxfont1(m_nDetailFontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 	
		
	wxFont wxfont2(m_nDetailFontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false); 	
	
	dc.SetTextForeground(m_clText); 
	wxRect rtClient = GetClientRect();	
	wxString  strText;
	wxCoord w,h;	
	wxPoint pt;
	pt.x = rtClient.GetLeft()+2;
	pt.y = nYBegin+2;
	dc.SetFont(wxfont1);
	strText = wxT("最新");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth()/2;
	//strText.Printf(wxT("%.2f"), m_MarketDataInfo.LastPrice);
	GetTextDefaultValue(strText, m_MarketDataInfo.LastPrice, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	SetTextColor(dc, m_MarketDataInfo.LastPrice);
	pt.x = rtClient.GetWidth()/2 -w;
	dc.DrawText(strText, pt);
	
	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	pt.x = rtClient.GetWidth()/2 +5;
	strText = wxT("均价");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth();
	//strText.Printf(wxT("%.2f"),m_averagePrice);
	double dbAverage = GetAveragePrice();
	GetTextDefaultValue(strText, dbAverage, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	SetTextColor(dc, dbAverage);
	pt.x = rtClient.GetWidth() -w-5;
	dc.DrawText(strText, pt);

//涨跌 振幅
	pt.x = rtClient.GetLeft()+2;
	pt.y = pt.y + h + 3;
	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	strText = wxT("涨跌");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth()/2;
	
	if(m_MarketDataInfo.LastPrice > -0.00001 && m_MarketDataInfo.LastPrice < 0.00001)
		strText = wxT("-");
	else
		GetTextDefaultValue(strText,m_MarketDataInfo.LastPrice - m_dbLastdbPrice, 0.0);		
		
	dc.GetTextExtent(strText, &w, &h);	
	SetTextColor(dc, m_MarketDataInfo.LastPrice);
	pt.x = rtClient.GetWidth()/2 -w;
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	pt.x = rtClient.GetWidth()/2 +5;
	strText = wxT("振幅");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth();
	if(m_MarketDataInfo.HighestPrice  > -0.00001 && m_MarketDataInfo.HighestPrice < 0.00001)
		strText = wxT("-");
	else
		strText.Printf(wxT("%.2f%%"), (m_MarketDataInfo.HighestPrice - m_MarketDataInfo.LowestPrice)/m_MarketDataInfo.LowestPrice*100);
	dc.GetTextExtent(strText, &w, &h);	
	SetTextColor(dc, 0.00);
	pt.x = rtClient.GetWidth() -w - 5;
	dc.DrawText(strText, pt);

	//涨幅 今开
	pt.x = rtClient.GetLeft()+2;
	pt.y = pt.y + h + 3;
	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	strText = wxT("涨幅");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth()/2;
	if(m_MarketDataInfo.LastPrice  > -0.00001 && m_MarketDataInfo.LastPrice < 0.00001)
		strText = wxT("-");
	else
		strText.Printf(wxT("%.2f%%"), (m_MarketDataInfo.LastPrice - m_dbLastdbPrice)/m_dbLastdbPrice*100);
	dc.GetTextExtent(strText, &w, &h);	
	SetTextColor(dc, m_MarketDataInfo.LastPrice);
	pt.x = rtClient.GetWidth()/2 -w;
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	pt.x = rtClient.GetWidth()/2 +5;
	strText = wxT("今开");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth();
	//strText.Printf(wxT("%.2f"), m_MarketDataInfo.OpenPrice);
	GetTextDefaultValue(strText, m_MarketDataInfo.OpenPrice, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	SetTextColor(dc, m_MarketDataInfo.OpenPrice);
	pt.x = rtClient.GetWidth() -w - 5;
	dc.DrawText(strText, pt);


	//总手，最高
	pt.x = rtClient.GetLeft()+2;
	pt.y = pt.y + h + 3;
	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	strText = wxT("总手");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth()/2;
	//strText.Printf(wxT("%.2f"), -0.00);
	GetTextDefaultValue_Volume(strText, m_MarketDataInfo.Volume, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	dc.SetTextForeground(m_clVolume); 	
	pt.x = rtClient.GetWidth()/2 -w;
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	pt.x = rtClient.GetWidth()/2 +5;
	strText = wxT("最高");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth();
//	strText.Printf(wxT("%.2f"), m_MarketDataInfo.HighestPrice);
	GetTextDefaultValue(strText, m_MarketDataInfo.HighestPrice, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	SetTextColor(dc, m_MarketDataInfo.HighestPrice);
	pt.x = rtClient.GetWidth() -w - 5;
	dc.DrawText(strText, pt);

	//现手，最低
	pt.x = rtClient.GetLeft()+2;
	pt.y = pt.y + h + 3;
	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	strText = wxT("现手");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth()/2;
//	strText.Printf(wxT("%.2f"), -0.00);
	GetTextDefaultValue_Volume(strText, m_nXianShou, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	dc.SetTextForeground(m_clVolume); 	
	pt.x = rtClient.GetWidth()/2 -w;
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	pt.x = rtClient.GetWidth()/2 +5;
	strText = wxT("最低");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth();
	//strText.Printf(wxT("%.2f"), m_MarketDataInfo.LowestPrice);
	GetTextDefaultValue(strText, m_MarketDataInfo.LowestPrice, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	SetTextColor(dc, m_MarketDataInfo.LowestPrice);
	pt.x = rtClient.GetWidth() -w - 5;
	dc.DrawText(strText, pt);

	//总额，量比
	pt.x = rtClient.GetLeft()+2;
	pt.y = pt.y + h + 3;
	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	strText = wxT("总额");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth()/2;
//	strText.Printf(wxT("%.2f"), m_MarketDataInfo.Turnover);
	GetTextDefaultValue(strText, m_MarketDataInfo.Turnover, 0.0);
	if(strText.Length()>4)
		strText = strText.Left(strText.Length()-3);

	dc.GetTextExtent(strText, &w, &h);	
	dc.SetTextForeground(m_clVolume); 	
	pt.x = rtClient.GetWidth()/2 -w;
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	pt.x = rtClient.GetWidth()/2 +5;
	strText = wxT("量比");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth();
	//strText.Printf(wxT("%.2f"), -0.00);
	GetTextDefaultValue(strText, 0.0, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	dc.SetTextForeground(m_clVolume); 	
	pt.x = rtClient.GetWidth() -w - 5;
	dc.DrawText(strText, pt);

	double dbZhangTing = 0, dbDieTing = 0;
	if(m_pFData)
	{
		dbZhangTing = m_pFData->m_UpperLimitPrice;
		dbDieTing   = m_pFData->m_LowerLimitPrice;
	}
	//涨停，跌停
	pt.x = rtClient.GetLeft()+2;
	pt.y = pt.y + h + 3;
	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	strText = wxT("涨停");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth()/2;
	//strText.Printf(wxT("%.2f%%"), -0.00);
	GetTextDefaultValue(strText, dbZhangTing, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	dc.SetTextForeground(m_Leftpanelzhang); 	
	pt.x = rtClient.GetWidth()/2 -w;
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	pt.x = rtClient.GetWidth()/2 +5;
	strText = wxT("跌停");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth();
	//strText.Printf(wxT("%.2f"), -0.00);
	GetTextDefaultValue(strText, dbDieTing, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	dc.SetTextForeground(m_Leftpaneldie); 	
	pt.x = rtClient.GetWidth() -w - 5;
	dc.DrawText(strText, pt);

	
	int nNeiPan = 0, nWaiPan = 0;
	if(m_pFData)
		m_pFData->GetNeiWaiPan(nNeiPan, nWaiPan);
	//内盘，外盘
	pt.x = rtClient.GetLeft()+2;
	pt.y = pt.y + h + 3;
	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	strText = wxT("内盘");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth()/2;
	//strText.Printf(wxT("%.2f%%"), -0.00);
	GetTextDefaultValue_Volume(strText, nNeiPan, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	dc.SetTextForeground(m_Leftpaneldie); 	
	pt.x = rtClient.GetWidth()/2 -w;
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	pt.x = rtClient.GetWidth()/2 +5;
	strText = wxT("外盘");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth();
//	strText.Printf(wxT("%.2f"), -0.00);
	GetTextDefaultValue_Volume(strText, nWaiPan, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	dc.SetTextForeground(m_Leftpanelzhang); 	
	pt.x = rtClient.GetWidth() -w - 5;
	dc.DrawText(strText, pt);

	pt.y = pt.y +h+1;
	dc.DrawLine(rtClient.GetLeft(), pt.y, rtClient.GetRight(),pt.y);	

	pt.y = pt.y +2;
	//持仓，结算
	pt.x = rtClient.GetLeft()+2;
	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	strText = wxT("持仓");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth()/2;
//	strText.Printf(wxT("%.2f%%"), -0.00);
	GetTextDefaultValue_Volume(strText, m_MarketDataInfo.OpenInterest, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	dc.SetTextForeground(m_Leftpanelhengpan); 	
	pt.x = rtClient.GetWidth()/2 -w;
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	pt.x = rtClient.GetWidth()/2 +5;
	strText = wxT("结算");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth();
//	strText.Printf(wxT("%.2f"), -0.00);
	GetTextDefaultValue(strText, m_MarketDataInfo.SettlementPrice, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	dc.SetTextForeground(m_Leftpanelhengpan); 	
	pt.x = rtClient.GetWidth() -w - 5;
	dc.DrawText(strText, pt);


	//增仓，前结
	pt.x = rtClient.GetLeft()+2;
	pt.y = pt.y + h + 3;
/*	增仓不知道怎么做的，先去掉
  dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	strText = wxT("增仓");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth()/2;
//	strText.Printf(wxT("%.2f"), -0.00);
	GetTextDefaultValue_Volume(strText, 0.0, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	dc.SetTextForeground(m_Leftpanelhengpan); 	
	pt.x = rtClient.GetWidth()/2 -w;
	dc.DrawText(strText, pt);
*/
	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	strText = wxT("日增");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth()/2;
	//	strText.Printf(wxT("%.2f"), -0.00);
	GetTextDefaultValue_Volume(strText, m_MarketDataInfo.OpenInterest - m_MarketDataInfo.PreOpenInterest, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	dc.SetTextForeground(m_Leftpanelhengpan); 	
	pt.x = rtClient.GetWidth()/2 -w;
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	pt.x = rtClient.GetWidth()/2 +5;
	strText = wxT("前结");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth();
//	strText.Printf(wxT("%.2f"), m_dbLastdbPrice);
	GetTextDefaultValue(strText, m_dbLastdbPrice, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	dc.SetTextForeground(m_Leftpanelhengpan); 	
	pt.x = rtClient.GetWidth() -w - 5;
	dc.DrawText(strText, pt);


	//日增，开平
	pt.x = rtClient.GetLeft()+2;
	pt.y = pt.y + h + 3;
/*	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	strText = wxT("日增");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont2);	
	pt.x = rtClient.GetWidth()/2;
	//strText.Printf(wxT("%.2f"), -0.00);
	GetTextDefaultValue_Volume(strText, m_MarketDataInfo.OpenInterest - m_MarketDataInfo.PreOpenInterest, 0.0);
	dc.GetTextExtent(strText, &w, &h);	
	dc.SetTextForeground(m_Leftpanelhengpan); 	
	pt.x = rtClient.GetWidth()/2 -w;
	dc.DrawText(strText, pt);
*/

	dc.SetFont(wxfont1);
	dc.SetTextForeground(m_clText); 
	pt.x = rtClient.GetWidth()/2 +5;
	strText = wxT("开平");
	dc.DrawText(strText, pt);

	dc.SetFont(wxfont1);	
	pt.x = rtClient.GetWidth();
	strText = m_strKaiPing;
	dc.GetTextExtent(strText, &w, &h);	
	dc.SetTextForeground(m_Leftpanelhengpan); 	
	pt.x = rtClient.GetWidth() -w - 5;
	dc.DrawText(strText, pt);
	pt.y = pt.y +h+1;
	dc.DrawLine(rtClient.GetLeft(), pt.y, rtClient.GetRight(),pt.y);	



	

}
void CQPanel::GetTextDefaultValue(wxString& strText, double dbValue, double dbDefault)
{
	if(dbValue - dbDefault > -0.00001 && dbValue - dbDefault < 0.00001)
		strText = wxT("-");
	else if(isInvalidValue(dbValue)|| dbValue == -6.2774385622041925e+066)
		strText = wxT("-");
	else
	{
		wxString strFormat;
		strFormat.Printf(wxT("%%.%df"), m_nNumDigits);
		strText.Printf(strFormat, dbValue);
	}
}
void CQPanel::GetTextDefaultValue_Volume(wxString& strText, double dbValue, double dbDefault)
{
	if(dbValue - dbDefault > -0.00001 && dbValue - dbDefault < 0.00001)
		strText = wxT("-");
	else if(isInvalidValue(dbValue) || dbValue == -6.2774385622041925e+066)
		strText = wxT("-");
	else
		strText.Printf(wxT("%.0f"), dbValue);
}
double CQPanel::GetAveragePrice()
{
	if(m_pFData == NULL || m_pFData->m_vecTLine.size() ==0)
		return 0.0;
	DWORD dbVolume  = 0;
	double dbVolumeMonney = 0;
	for(int i =0; i< (int)m_pFData->m_vecTLine.size(); i++)
	{
		STLine *pTLine = m_pFData->m_vecTLine[i];
		dbVolume += pTLine->dwVolume;
		dbVolumeMonney += pTLine->dwVolumeMoney;
	}
	if(dbVolume < 0.00001 && dbVolume > -0.00001)
		return 0.0;
	return dbVolumeMonney/(m_pFData->m_VolumeMultiple * dbVolume);
}
void CQPanel::GetGridSize(wxPoint& pt, wxSize& rt)
{
	WXHDC wxHDC = ((WXHDC)GetDC((HWND)m_hWnd));
	wxClientDC dc(this);	

	wxRect rtClient = GetClientRect();	
	wxString  strText;
	wxCoord w,h;	
	wxFont wxfont(m_nHeadFontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false); 	
	dc.SetFont(wxfont);	
	strText = _T("cu1206");
	dc.GetTextExtent(strText, &w, &h);	
	int nhead = h+1;
	
	pt.x = rtClient.GetLeft() + (rtClient.GetWidth() - w)/2;
	pt.y = rtClient.GetTop() + 1; 
	

	wxFont wxfont2(m_nDetailFontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 	
	dc.SetFont(wxfont2);	
	strText = wxT("卖5");
	dc.GetTextExtent(strText, &w, &h);	

	wxPen penBorder(wxColour(m_clBorder), 1, wxSOLID);
	dc.SetPen(penBorder);	


	pt.x = 7;
	pt.y = nhead +1;
	pt.y = pt.y + (h +3)*(m_nHQCount-1);
	
	pt.y = pt.y +h+1;	
	pt.y = pt.y +1;
	pt.y = pt.y + (h +3)*(m_nHQCount-1);

	pt.y = pt.y +h+1;	
	int nReturn = pt.y;
	
	wxFont wxfont1(m_nDetailFontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 	

	dc.SetFont(wxfont1);		
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft()+2;
	pt.y = nReturn+2;	
	
	pt.y = pt.y + h + 3;	
	pt.y = pt.y + h + 3;	
	pt.y = pt.y + h + 3;	
	pt.y = pt.y + h + 3;
	pt.y = pt.y + h + 3;	
	pt.y = pt.y + h + 3;	
	pt.y = pt.y + h + 3;
	pt.y = pt.y +h+1;
	pt.y = pt.y +2;
	pt.y = pt.y + h + 3;
	pt.y = pt.y + h + 3;
	pt.y = pt.y +h+1;
	pt.y =pt.y +1;
	rt.x = rtClient.GetRight()  - pt.x;
	rt.y = rtClient.GetBottom() - pt.y;

	
}
void CQPanel::SetGridLastItem()
{
	if(m_pFData == NULL)
		return;

	vector<SQIntime *>&  vecQIntime = m_pFData->m_vecQIntime;
	if(vecQIntime.size() == 0)
		return;

	SQIntime *pItemLast = vecQIntime[vecQIntime.size()-1];
	if(m_pItemLastShow == pItemLast)
		return;

	int nCount = m_grid->GetNumberRows();
	m_grid->AppendRows(1);	
	m_nXianShou = pItemLast->dwVolume;
//	m_strKaiPing = pItemLast->strDesc.c_str();
	GetDescName(pItemLast->nDesc, m_strKaiPing);
	
	bool bVisible = m_grid->IsVisible(nCount-1, 0);
	if(bVisible ||m_bShowVisible)
	{
		m_bShowVisible = false;
		m_grid->MakeCellVisible(nCount, 0);
	}
	m_pItemLastShow = pItemLast;
}
void CQPanel::SetOneItem(SQIntime *pItemLast, SQIntime *pItemLast2,int nRow, bool bRefresh)
{
	struct tm tm2;
	tm2.tm_year = 0;
	double dbPrice = m_dbLastdbPrice;
	if(pItemLast2 != NULL)
	{
		dbPrice = pItemLast2->fLastPrice;
		tm2     = pItemLast2->dwTime;
	}
	vector<string> vec;
	wxString strText;
	if(pItemLast2 == NULL)
		strText.Printf(wxT("%.2d:%.2d"), pItemLast->dwTime.tm_hour, pItemLast->dwTime.tm_min);
	else
	{
		if(pItemLast->dwTime.tm_year == pItemLast2->dwTime.tm_year
			&& pItemLast->dwTime.tm_mon == pItemLast2->dwTime.tm_mon 
			&& pItemLast->dwTime.tm_mday == pItemLast2->dwTime.tm_mday
			&& pItemLast->dwTime.tm_hour == pItemLast2->dwTime.tm_hour
			&& pItemLast->dwTime.tm_min == pItemLast2->dwTime.tm_min)
			strText.Printf(wxT("   :%.2d"),  pItemLast->dwTime.tm_sec);			
		else
			strText.Printf(wxT("%.2d:%.2d"), pItemLast->dwTime.tm_hour, pItemLast->dwTime.tm_min);
	}
	SRow s;
	s.strTime = strText;

	GetTextDefaultValue(strText,pItemLast->fLastPrice, 0.0);	
//	strText.Printf(wxT("%.2f"),  pItemLast->fLastPrice );
	s.strPrince = strText;

	strText.Printf(wxT("%d"),  pItemLast->dwVolume );
	s.strVolume = strText;

	strText.Printf(wxT("%d"),  pItemLast->dwOpenVolume );
	s.strVOpen = strText;

	strText.Printf(wxT("%d"),  pItemLast->dwCloseVolume );
	s.strVClose = strText;

//	strText =  pItemLast->strDesc.c_str() ;
	GetDescName(pItemLast->nDesc, strText);

	s.strDesc = strText;

	wxString strData;
	strData.Printf(wxT("%d"), reinterpret_cast<DWORD>(pItemLast) );
	s.strRefData = strData.c_str();

	m_vecRow.push_back(s);
	
	if(bRefresh)
	{
		m_nXianShou = pItemLast->dwVolume;
	//	m_strKaiPing = pItemLast->strDesc.c_str();
		GetDescName(pItemLast->nDesc, m_strKaiPing);		
	}
}
void CQPanel::OnFuncQPanelData_InTime(wxCommandEvent& event)
{
	SetGridLastItem();
}
void CQPanel::InitGrid()
{
	m_vecRow.clear();
	m_table->SetLastPrice(m_pFData->m_PreSettlementPrice);
	vector<SQIntime *>&  vecQIntime = m_pFData->m_vecQIntime;
	m_table->SetFData(m_pFData);

	int nAddRow = m_pFData->m_vecQIntime.size() - m_grid->GetNumberRows();
	if(nAddRow>0)
		m_grid->AppendRows(nAddRow);
	else
		m_grid->DeleteRows(0, -nAddRow);

	int n = m_grid->GetNumberRows();
	if(vecQIntime.size() == 0)
		return;
	
	int nLast = (int)m_pFData->m_vecQIntime.size() -1 ;
	SQIntime *pItemLast = vecQIntime[nLast];
	m_nXianShou = pItemLast->dwVolume;	
	GetDescName(pItemLast->nDesc, m_strKaiPing);		
	
	int nCount = m_grid->GetNumberRows();	
	m_grid->MakeCellVisible(nCount-1, 0);
}
bool CQPanel::InitCfg(TiXmlElement *root)
{		
	if(m_table)
		m_table->InitCfg(root);
	TiXmlElement *tList = root->FirstChildElement("qpanel");
	if(tList == NULL)
		return false;

	TiXmlNode *phqcount = tList->FirstChild("hqcount");
	if(phqcount == NULL)
		return false;
	const char *strValue = phqcount->ToElement()->GetText();
	unsigned long value = ColorStr2Long(strValue);
	SetHQCount(value);

	TiXmlNode *pColor = tList->FirstChild("color");
	if(pColor == NULL)
		return false;

	TiXmlNode *ListItem = pColor->FirstChild("background");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetBackGround(value);


	ListItem = pColor->FirstChild("border");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetBorder(value);

	ListItem = pColor->FirstChild("Leftpanelzhang");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();	
	value = ColorStr2Long(strValue);
	SetLeftpanelzhang(value);

	ListItem = pColor->FirstChild("Leftpaneldie");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();	
	value = ColorStr2Long(strValue);
	SetLeftpaneldie(value);

	ListItem = pColor->FirstChild("Leftpanelhengpan");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();	
	value = ColorStr2Long(strValue);
	SetLeftpanelhengpan(value);

	ListItem = pColor->FirstChild("ContractName");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();	
	value = ColorStr2Long(strValue);
	SetContractName(value);

	
	ListItem = pColor->FirstChild("InstrumentName");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();	
	value = ColorStr2Long(strValue);
	SetInstrumentName(value);
	
	ListItem = pColor->FirstChild("Text");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();	
	value = ColorStr2Long(strValue);
	SetclText(value);


	ListItem = pColor->FirstChild("Volume");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();	
	value = ColorStr2Long(strValue);
	SetclVolume(value);

	ListItem = pColor->FirstChild("GridLabelText");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();	
	value = ColorStr2Long(strValue);
	SetclGridLabelText(value);



	TiXmlNode *pfont = tList->FirstChild("font");
	if(pfont == NULL)
		return false;

	TiXmlNode *Listfont = pfont->FirstChild("headSize");
	if(Listfont == NULL)
		return false;
	strValue = Listfont->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetHeadFontSize(value);

	Listfont = pfont->FirstChild("detailSize");
	if(Listfont == NULL)
		return false;
	strValue = Listfont->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetDetailFontSize(value);


	Listfont = pfont->FirstChild("GridHeadFontSize");
	if(Listfont == NULL)
		return false;
	strValue = Listfont->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetGridHeadFontSize(value);
	return true;
}
void CQPanel::SetGridHeadFontSize(unsigned long lValue)
{
	m_nGridHeadFontSize = lValue;
	if(m_grid)
		m_grid->SetHeadFontSize(m_nGridHeadFontSize);
}
void CQPanel::SetHQCount(int nHQCount)
{
	if(nHQCount <1)
		m_nHQCount = 1;
	else if(nHQCount >10)
		nHQCount = 10;
	else
		m_nHQCount = nHQCount;
}
void CQPanel::SetBackGround(unsigned long lValue)
{
	m_clBackGround = lValue;
}
void CQPanel::SetBorder(unsigned long lValue)
{
	m_clBorder = lValue;
}
void CQPanel::SetLeftpanelzhang(unsigned long lValue)
{
	m_Leftpanelzhang = lValue;
}
void CQPanel::SetLeftpaneldie(unsigned long lValue)
{
	m_Leftpaneldie = lValue;
}
void CQPanel::SetLeftpanelhengpan(unsigned long lValue)
{
	m_Leftpanelhengpan = lValue;
}
void CQPanel::SetContractName(unsigned long lValue)
{
	m_clContractName = lValue;
}
void CQPanel::SetInstrumentName(unsigned long lValue)
{
	m_clInstrumentName = lValue;
}
void CQPanel::SetclText(unsigned long lValue)
{
	m_clText = lValue;
}
void CQPanel::SetclVolume(unsigned long lValue)
{
	m_clVolume = lValue;
}
void CQPanel::SetclGridLabelText(unsigned long lValue)
{
	m_clGridLabelText = lValue;
}
void CQPanel::GetDescName(int nDesc, wxString& strDesc)
{
	switch(nDesc)
	{
	case 0:
		strDesc = KONGKAI;
		break;
	case 1:
		strDesc = DUOKAI;
		break;
	case 2:
		strDesc = DUOPING;
		break;
	case 3:
		strDesc = KONGPING;
		break;
	case 4:
		strDesc = DUOHUAN;
		break;
	case 5:
		strDesc = KONGHUAN;
		break;
	case 6:
		strDesc = SHUANGPING;
		break;
	case 7:
		strDesc = SHUANGKAI;
		break;
	}

}
WXLRESULT CQPanel::MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam) 
{//wxpanel不支持return escape，所以在这里处理onchar	
	switch (message) 
	{ 
	case WM_KEYDOWN: 			
		{
			OnChar((int)wParam);
		}
		break;
	default:
		break;
		
	} 
	return wxPanel::MSWWindowProc(message,wParam,lParam); 
}
void CQPanel::OnChar(int nkeyCode)
{	
	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_GridEx_KeyDown);
	myEvent.SetInt(nkeyCode);
	if(window)
		window->ProcessEvent(myEvent);
}
void CQPanel::OnLeftDBClick(wxMouseEvent& event)
{
	
}
void CQPanel::OnFuncKeyDown(wxCommandEvent& event)
{
	wxWindow* window= GetParent();	
	window->ProcessEvent(event);
}
void CQPanel::SetHeadFontSize(int nHeadFontSize)
{
	m_nHeadFontSize = nHeadFontSize;
}
void CQPanel::SetDetailFontSize(int nDetailFontSize)
{
	m_nDetailFontSize = nDetailFontSize;
}