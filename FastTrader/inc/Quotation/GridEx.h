#pragma once
#include "wx\msw\dialog.h"
#include "KSplitterWindow.h"
#include <string>
#include "../inc/Quotation/CritSec.h"
#include "../inc/Quotation/AutoLock.h"
#include "..\\inc\\Module-Misc2\KernelStruct.h"
#include "../inc/Quotation/FData.h"
#include "wx/grid.h"


DECLARE_EVENT_TYPE(wxEVT_GridEx_KeyDown, 3900)
extern long ColorStr2Long(const char* pColTextColor);
class wxGridEx :public wxGrid
{
public:
	wxGridEx();

	wxGridEx( wxWindow *parent,
	wxWindowID id,
	const wxPoint& pos = wxDefaultPosition,
	const wxSize& size = wxDefaultSize,
	long style = wxWANTS_CHARS,
	const wxString& name = wxGridNameStr );

	virtual ~wxGridEx(void);

	DECLARE_DYNAMIC_CLASS (wxGridEx)
	DECLARE_EVENT_TABLE()
	void OnKeyDown( wxKeyEvent& event );
	virtual void DrawColLabel( wxDC& dc, int col );	
	void OnSize( wxSizeEvent& WXUNUSED(event) );
public:
	wxColour			 m_Leftpanelzhang;  //��
	wxColour			 m_TextColor;  //��
	int					 m_nHeadFontSize;
	void				 SetHeadFontSize(unsigned long lValue){ m_nHeadFontSize = lValue; }	

	WXLRESULT MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam);
	void SetScrollbar(int orient,	int pos,int pageSize,int range,	bool refresh);
	void SetScrollPos( int orient, int pos, bool refresh);

	virtual void AdjustScrollbars();
protected:
	bool m_created;
	bool m_bSize;//ֻ��size��ʱ����ƶ������
};
struct SRow
{
	string strTime;
	string strPrince;
	string strVolume;
	string strVOpen;
	string strVClose;
	string strDesc;
	string strRefData;//Ψһ�Ա�ʶ
};
class BigGridTable : public wxGridTableBase
{
public:
	BigGridTable(long lRows, long lCols);
	int GetNumberRows();
	int GetNumberCols();
	wxString GetValue( int row, int col );
	wxGridCellAttr* GetAttr( int row, int col,wxGridCellAttr::wxAttrKind  kind );
	bool DeleteRows( size_t pos, size_t numRows );
	void SetValue( int , int , const wxString&  );
	bool IsEmptyCell( int , int  );
	void SetNumberRows(int count);
	void SetFData(CFData*	pFData);
	void SetColLabelValue( int col, const wxString& str);
	wxString GetColLabelValue( int col );
	bool AppendRows( size_t numRows = 1 );
	void SetLastPrice(double dbPrice);

	bool InitCfg(TiXmlElement *root);
	void SetclText(unsigned long lValue);
	void SetLeftpanelzhang(unsigned long lValue);
	void SetLeftpaneldie(unsigned long lValue);
	void SetLeftpanelhengpan(unsigned long lValue);
	void SetclVolume(unsigned long lValue);
	void SetBodyfontsize(unsigned long lValue);	
	void SetNumDigits(int nNumDigits);
private:
	void SetVolumeDescColor(int row, wxGridCellAttr *pCellAttr);//����������ɫ������
	int      m_rowcount;
	int      m_colcount;
	
//	vector<SQIntime *>*  m_vecQIntime;//ʵʱ
	CFData*				m_pFData;
	map<int, wxString>  m_mapLabels;

	double   m_dbLastPrice;

	wxColour m_clText;
	wxColour m_Leftpanelzhang;
	wxColour m_Leftpaneldie;
	wxColour m_Leftpanelhengpan;
	wxColour m_clVolume;

	int				   m_nNumDigits;//��Ч����

	int		 m_nBodyfontSize;
};