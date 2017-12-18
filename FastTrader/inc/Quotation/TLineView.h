#pragma once
#include "wx\msw\dialog.h"
#include "TLine.h"
#include "TVolume.h"
#include "TScales.h"
#include "KSplitterWindow.h"
#include <string>
#include "../inc/Quotation/CritSec.h"
#include "../inc/Quotation/AutoLock.h"
#include "../inc/Quotation/FData.h"
DECLARE_EVENT_TYPE(wxEVT_RETURN_KLINE, 4101)
DECLARE_EVENT_TYPE(wxEVT_RETURN_BAOJIABIAO2, 4102)
DECLARE_EVENT_TYPE(wxEVT_TLINEVIW_INFO, 4103)
class MyFrame;
class CTLineView :public wxPanel
{
public:
	CTLineView(MyFrame* myFrame);

	wxWindow* CreateDlg(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER|wxCLIP_CHILDREN,
		const wxString& name = wxPanelNameStr);


	~CTLineView(void);

	DECLARE_DYNAMIC_CLASS (CTLineView)
	 
	DECLARE_EVENT_TABLE()

	void OnPaint (wxPaintEvent & PaintEvent);
	void OnSize(wxSizeEvent& event);
	void OnEraseBackground(wxEraseEvent& event)
	{
		event.Skip();
	}
protected:
	CTLine	 *m_pTLineCtrl;
	CTVolume *m_pTVolumeCtrl;
	wxPanel  *m_pTTechIndex;
	CTScales *m_pTScalesCtrl;
	CFData   *m_pFData;//��ֻ�ڻ���������


	long     m_lKLineType;//���е�ͼ����̬
	
	KSplitterWindow* m_splitter;
	KSplitterWindow* m_splitterTop;
	KSplitterWindow* m_splitterBottom;
	
	MyFrame* m_myFrame;




public:
	

public:
	virtual void CfgShow(long lKLineType);											//��ʾ��Щk�ߣ����ܣ�����ָ�꣬ʱ����
	void ShowTip(int nType, BOOL bShow = FALSE);
	wxPanel *GetControl(TTYPE kType);				//�õ��ڲ�ĳ���ؼ��ĵ�ַ
	void SetLastClosePrice(double dbPrice);
	BOOL SetInTimeData();
	void SetLastTransTime(struct tm tmTime);
	bool InitCfg(TiXmlElement *root);
	void SetFData(CFData *pFData);



public://�����ö�ȡ���ݣ���Щ�����ǴӴ��ǻ��Ҽ������������������� shanghai_IF_1107_min1��shanghai_IF_1107_min5��shanghai_IF_1107_day��shanghai_IF_1107_week��shanghai_IF_1107_month
//	void ReadFileTest();
//	void split(const string& s, char c, vector<string>& v);
protected:
	void ClearMemory();

public:
	void OnFuncKeyDown(wxCommandEvent& event);
	void OnFuncLButtonDown(wxCommandEvent& event);	
	void OnFuncLDBClick(wxCommandEvent& event);		
	void OnFuncRButtonUp(wxCommandEvent& event);
	void OnFuncMouseMoveHook(wxCommandEvent& event);
	void OnFuncMouseLeave(wxCommandEvent& event);
	void OnFuncMouseEnter(wxCommandEvent& event);

	void CrossShow(wxCommandEvent& event);

	void OnKTLineAdd(wxCommandEvent& event);
	void OnKTLineMaxSize(wxCommandEvent& event);

	void OnReturnKLine(wxCommandEvent& event);
	void OnReturnBaoJiaBiao(wxCommandEvent& event);

	int  m_nTViewCount;
	void OnViewOne(wxCommandEvent& WXUNUSED(event));
	void OnViewTwo(wxCommandEvent& WXUNUSED(event));
	void OnViewInfo(wxCommandEvent& WXUNUSED(event));
	void				OnView1();
	void				OnView2();

	bool				m_bShowRightPanel;
public:
	void				SetShowRightPanel(bool bShowRightPanel){	m_bShowRightPanel = bShowRightPanel;};
};
