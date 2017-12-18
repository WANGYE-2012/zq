#include "StdAfx.h"
#include "COrderFAKFOKPanel.h"
#include "COrderInsertView.h"



BEGIN_EVENT_TABLE(COrderFAKFOKPanel, wxPanel)
	EVT_RADIOBUTTON(ID_FAKFOK_RADIOFAK,OnRadioButton)
	EVT_RADIOBUTTON(ID_FAKFOK_RADIOFOK,OnRadioButton)
    EVT_PANEL_CHAR_HOOK(OnPanelCharHook)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


COrderFAKFOKPanel::COrderFAKFOKPanel(wxWindow *parent,
            wxWindowID winid ,
            const wxPoint& pos ,
            const wxSize& size ,
            long style,
            const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{

	wxPoint posInit(5, 5);
	int nFW = 8, nFH = 16;
	int nVW = nFW+2, nVH = nFH+6;
	wxPoint ctlPos(0, 0);
	wxSize ctlSz(0, 0);

	ctlPos = wxPoint(posInit.x, posInit.y+4);
	ctlSz.Set(nFW*7, nFH);
	wxRadioButton* itemRadioBox1 = new wxRadioButton( this, ID_FAKFOK_RADIOFAK, wxEmptyString, ctlPos, ctlSz);//�����//, wxRB_GROUP);
	itemRadioBox1->SetValue(true);
	ctlPos = wxPoint(posInit.x+nFW*7+nFW/2, posInit.y+4);
	ctlSz.Set(nFW*7, nFH);
	wxRadioButton* itemRadioBox2 = new wxRadioButton( this, ID_FAKFOK_RADIOFOK, wxEmptyString, ctlPos, ctlSz);//����//, wxRB_GROUP);

	ctlPos = wxPoint(posInit.x, posInit.y+nVH+2);
	ctlSz.Set(nFW*10, nFH);
	wxStaticText* itemStaticText1 = new wxStaticText( this, ID_FAKFOK_STATICE1, wxEmptyString, ctlPos, ctlSz, wxALIGN_RIGHT);//ÿ������
	
	ctlPos = wxPoint(posInit.x+nFW*10+2, posInit.y+nVH);
	ctlSz.Set(nFW*10, 20);
	wxSpinCtrl* itemSpinCtrl1 = new wxSpinCtrl( this, ID_FAKFOK_SPINCTRLMINVOLUME, wxEmptyString, ctlPos, ctlSz);
	itemSpinCtrl1->SetMin(0);
	itemSpinCtrl1->SetMax(9999);

	m_TabOrderVector.clear();
	//tab order
	m_TabOrderVector.push_back(ID_FAKFOK_RADIOFAK);
	m_TabOrderVector.push_back(ID_FAKFOK_RADIOFOK);
	m_TabOrderVector.push_back(ID_FAKFOK_SPINCTRLMINVOLUME);

	ResetLanguage();
}

COrderFAKFOKPanel::~COrderFAKFOKPanel()
{
}
COrderFAKFOKPanel* COrderFAKFOKPanel::CreatePane(wxWindow* parent)
{
    return  new COrderFAKFOKPanel(parent, GID_ORDER_INSERT_PANEL);
}
void COrderFAKFOKPanel::OnPanelCharHook(wxCommandEvent& evt)
{
    wxKeyEvent* pEvent=(wxKeyEvent*)evt.GetClientData();
    if(!pEvent) return;
    wxWindow *win = FindFocus();
    if(win == NULL) {
        evt.Skip();
        return;
    }
    if(pEvent->GetKeyCode() == WXK_RETURN)
    {
        //int id,thisID=win->GetId(),count=m_TabOrderVector.size();
        //for(id=0;id<count;id++)
        //{
        //    if(m_TabOrderVector[id]==thisID)
        //    {
        //        if(id<count-1)
        //        {
        //            FindWindowById(m_TabOrderVector[id+1],this)->SetFocus();
        //            return;
        //        }
        //    }
        //}
    }
    else if(pEvent->GetKeyCode() == WXK_LEFT)
    {
		//wxSpinEvent evtSpin;
		//wxComboBox* itemCombo = NULL;
		//int thisID=win->GetId();
		//switch(thisID) {
		//case ID_OBATCH_SPINCTRLVOLUME:
		//	if(sm_nViewStyle == 3) {	// ����ǽ��˴���
		//		itemCombo = (wxComboBox*)FindWindowById(ID_OBATCH_SPINCTRLVOLUME, this);
		//		if(itemCombo != NULL) {
		//			int CurSel=0;
		//			if((CurSel=itemCombo->GetSelection())>0)
		//				itemCombo->SetSelection(itemCombo->GetSelection()-1);
		//			else
		//			{
		//				wxString strValue=itemCombo->GetValue();
		//				for(CurSel=0;CurSel<(int)itemCombo->GetCount();CurSel++)
		//				{
		//					if(itemCombo->GetString(CurSel).Find(strValue)==0) {
		//						break;
		//					}
		//				}
		//				if(CurSel<(int)itemCombo->GetCount())
		//					itemCombo->SetSelection(CurSel);
		//				else 
		//					itemCombo->SetSelection(0);
		//			}
		//			SetPanelItemText(ID_CMBLONGCODE, itemCombo->GetValue());
		//			m_DynamicItem.IsValid=true;
		//			m_NeedUpdatePrice=true;
		//			FindWindowById(ID_CTLWDSHOW,this)->Refresh();
		//		}
		//	}
		//	break;
		//case ID_OBATCH_SPINCTRLTIMER:
		//	evtSpin.SetId(ID_SPINCTRL_VOLUME_BUTTON);
		//	OnPriceSpinDown(evtSpin);
		//	break;
		//case ID_SPINCTRL_VOLUME_TEXT:
		//	evtSpin.SetId(ID_SPINCTRL_BUTTON);
		//	OnVolumeSpinDown(evtSpin);
		//	break;
		//};
    }
    else if(pEvent->GetKeyCode() == WXK_RIGHT)
    {
		//wxSpinEvent evtSpin;
		//wxComboBox* itemCombo = NULL;
		//int thisID=win->GetId();
		//switch(thisID) {
		//case ID_CONTRANCTNAME:
		//	if(sm_nViewStyle == 3) {	// ����ǽ��˴���
		//		itemCombo = (wxComboBox*)FindWindowById(ID_CONTRANCTNAME, this);
		//		if(itemCombo != NULL) {
		//			int CurSel=0;
		//			if((CurSel=itemCombo->GetSelection())<(int)itemCombo->GetCount())
		//				itemCombo->SetSelection(itemCombo->GetSelection()+1);
		//			else
		//			{
		//				wxString strValue=itemCombo->GetValue();
		//				for(CurSel=0;CurSel<(int)itemCombo->GetCount();CurSel++)
		//				{
		//					if(itemCombo->GetString(CurSel).Find(strValue)==0) {
		//						break;
		//					}
		//				}
		//				if(CurSel<(int)itemCombo->GetCount())
		//					itemCombo->SetSelection(CurSel);
		//				else 
		//					itemCombo->SetSelection(0);
		//			}
		//			SetPanelItemText(ID_CMBLONGCODE, itemCombo->GetValue());
		//			m_DynamicItem.IsValid=true;
		//			m_NeedUpdatePrice=true;
		//			FindWindowById(ID_CTLWDSHOW,this)->Refresh();
		//		}
		//	}
		//	break;
		//case ID_SPINCTRL_TEXT:
		//	evtSpin.SetId(ID_SPINCTRL_VOLUME_BUTTON);
		//	OnPriceSpinUp(evtSpin);
		//	break;
		//case ID_SPINCTRL_VOLUME_TEXT:
		//	evtSpin.SetId(ID_SPINCTRL_BUTTON);
		//	OnVolumeSpinUp(evtSpin);
		//	break;
		//};
    }
    else if(pEvent->GetKeyCode() == WXK_UP)
    {
        int id,thisID=win->GetId(),count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                if(id>0)
                {
                    FindWindowById(m_TabOrderVector[id-1],this)->SetFocus();
                    return;
                }
            }
        }
    }
    else if(pEvent->GetKeyCode() == WXK_DOWN)
    {
        int id,thisID=win->GetId(),count=m_TabOrderVector.size();
        for(id=0;id<count;id++)
        {
            if(m_TabOrderVector[id]==thisID)
            {
                if(id<count-1)
                {
                    FindWindowById(m_TabOrderVector[id+1],this)->SetFocus();
                    return;
                }
            }
        }
    }
    //evt.Skip();
}

void COrderFAKFOKPanel::ResetLanguage()
{
	wxWindow* pWin = NULL;
	pWin = FindWindowById(ID_FAKFOK_RADIOFAK, this);
	if(pWin) pWin->SetLabel(LOADSTRING(FAK));
	pWin = FindWindowById(ID_FAKFOK_RADIOFOK, this);
	if(pWin) pWin->SetLabel(LOADSTRING(FOK));
	pWin = FindWindowById(ID_FAKFOK_STATICE1, this);
	if(pWin) pWin->SetLabel(LOADSTRING(ID_FAKFOK_STATICE1));
}

// ����������־�ļ����
wxString COrderFAKFOKPanel::GetUserInputText()
{
	wxString strRet;

	strRet = LOADSTRING(FAK/FOK����:);
	{
		wxRadioButton* window=NULL;
		window=(wxRadioButton*)FindWindowById(ID_FAKFOK_RADIOFAK,this);
		if(window->GetValue()) {
			strRet+=LOADSTRING(ָ������=[);
			strRet+=window->GetLabel();
			strRet+=LOADSTRING(OBP_INPUT_END);
		}
		window=(wxRadioButton*)FindWindowById(ID_FAKFOK_RADIOFOK,this);
		if(window->GetValue()) {
			strRet+=LOADSTRING(OBP_INPUT_MIDDLE);
			strRet+=window->GetLabel();
			strRet+=LOADSTRING(OBP_INPUT_END);
		}
	}

	{
		wxSpinCtrl* window=NULL;
		window=(wxSpinCtrl*)FindWindowById(ID_FAKFOK_SPINCTRLMINVOLUME,this);
		if(window) { 
			strRet+=LOADSTRING(��С����=[);
			strRet+=Number2String(window->GetValue());
			strRet+=LOADSTRING(OBP_INPUT_END);
		}
	}
	
	strRet+="\n";
	return strRet;

}

void  COrderFAKFOKPanel::OnRadioButton(wxCommandEvent& event)
{
	int id=event.GetId();
	bool bEnabled;

	if(id==ID_FAKFOK_RADIOFAK) bEnabled=true;
	else bEnabled=false;

	wxWindow* pWin = NULL;
	pWin = FindWindowById(ID_FAKFOK_SPINCTRLMINVOLUME, this);
	if(pWin) 
		pWin->Enable(bEnabled);

     //((COrderInsertView*)(this->GetParent()))->EnablePriceInputCtl(bEnabled);
}
