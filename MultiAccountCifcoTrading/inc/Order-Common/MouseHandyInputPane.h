#pragma once
//#include "wx/wxprec.h"
//#ifndef WX_PRECOMP
//    #include "wx/wx.h"
//#endif
//#include "wx/popupwin.h"
//#include "wx/spinctrl.h"
//#include "wx/listctrl.h"
//
//
////----------------------------------------------------------------------------
//// SimpleTransientPopup
////----------------------------------------------------------------------------
//class MouseHandyInputPane : public wxPopupTransientWindow
//{
//private:
//	enum { conMaxButtonNum = 30 };
//
//// IDs for the controls and the menu commands
//enum
//{
//    ID_PopupButton = 1,
//    ID_PopupList = ID_PopupButton+100
//};
//
//
////#define CODEHEADS wxT("cu,ͭ,t,al,��,l,zn,п,x,ru,��,xj,fu,ȼ��,ry,au,�ƽ�,hj,rb,���Ƹ�,lwg,wr,�߲�,xc,IF,��ָ,gz,a,��һ,dy,b,����,de,m,����,dp,y,����,dy,c,������,hym,l,����ϩ,jyx,p,���,zl,v,������ϩ,jlyx,SR,����,bt,CF,�޻�,mh,GN,�̶�,ld,RO,����,cy,TA,PTA,pta,WS,ǿ��,qm,WT,Ӳ��,ym,ER,���̵�,zxd")
//
//typedef struct {
//	char strCodeHead[8];	// ��Լ����ͷ
//	char strCodeName[16];	// ��Լ���
//	char strCodeNameJP[8];	// ��Լ��Ƶ�ƴ������ĸ
//} CODEHEADDATA, *PCODEHEADDATA;
//
//
//public:
//    MouseHandyInputPane( wxWindow *parent );
//    virtual ~MouseHandyInputPane();
//
//	void ShowKeyInput(wxString& strKeyInput) {
//		m_bKeyInput = TRUE;
//		m_strKeyInput = strKeyInput;
//	};
//
//    // wxPopupTransientWindow virtual methods are all overridden to log them
//    virtual void Popup(wxWindow *focus = NULL);
//    virtual void OnDismiss();
//    virtual bool ProcessLeftDown(wxMouseEvent& event);
//    virtual bool Show( bool show = true );
//    wxScrolledWindow* GetChild() { return m_panel; }
//
//private:
//    wxScrolledWindow *m_panel;
//    wxButton*		m_button[conMaxButtonNum];
//	wxListCtrl*		m_listBox;
//	BOOL			m_bKeyInput;
//	wxString		m_strKeyInput;
//	int				m_nCodeHeadsCount;
//	CODEHEADDATA*	m_ptCodeHeads;
//
//private:
//	void OnMouse(wxMouseEvent &event);
//    void OnSize( wxSizeEvent &event );
//    void OnSetFocus( wxFocusEvent &event );
//    void OnKillFocus( wxFocusEvent &event );
//    void OnButton( wxCommandEvent& event );
//    void OnListBoxSelected(wxListEvent& event);
//
//private:
//    DECLARE_CLASS(MouseHandyInputPane)
//    DECLARE_EVENT_TABLE()
//};
