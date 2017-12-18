#pragma once
#ifdef _USE_TOP_SPLITTER
#include <wx/splitter.h> 

class CVideoInfoPanel;
class CNewsInfoPanel;
class CGuideInfoPanel;

class MultiSplitterWindow: public wxSplitterWindow
{
public:
	MultiSplitterWindow(wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxSP_3D| wxNO_BORDER);
	bool OnSashPositionChange(int WXUNUSED(newSashPosition)); 
	bool m_fixed;
};
class MultiSplitter :
	public wxPanel
{
public:
	MultiSplitter(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);
	~MultiSplitter(void);
	virtual bool Show( bool show = true );
	void OnSize(wxSizeEvent& evt);

	void StartTimer();
	void StopTimer();

	DECLARE_EVENT_TABLE()
private:
	MultiSplitterWindow* m_pSplitterBase;
	//��Ƶ
	CVideoInfoPanel* m_pVideoPanel;
	//��Ѷ
	CNewsInfoPanel*  m_pNewsPanel;
	//����ָ��
	CGuideInfoPanel* m_pGuidePanel;
};
#endif
