#include "stdafx.h"

#include <vector>
#include <string>
#include "..\\inc\\cfgMgr\\CfgMgr.h"
#include "../inc/cfgMgr/MultiAccountMgr.h"
#include "COrderAccountTypePanel.h"


BEGIN_EVENT_TABLE(COrderAccountTypePanel, wxPanel)

END_EVENT_TABLE()

COrderAccountTypePanel::COrderAccountTypePanel(OrderStrategy eStrategy, 
							   wxWindow *parent,
							   wxWindowID winid ,
							   const wxPoint& pos ,
							   const wxSize& size ,
							   long style,
							   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{
	m_eStrategy = eStrategy;

	wxString strLabelLine1;
	wxString strLabelLine2;
	switch(eStrategy)
	{
	case FirstAccountAlign:
		strLabelLine1 = "��һ���˻����µ��������µ�����ȣ������˻����µ�����=";
		strLabelLine2 = "�µ����������������͵Ľ�";
		break;
	case AccountScaleDivided:
		strLabelLine1 = "�µ���ָ��������Ϊ���������ո��˻������ʽ��Ȩ�ؽ���";
		strLabelLine2 = "����";
		break;
	default:
		break;
	}

	const int nLeftBorder = 50;
	const int nTop = 25;
	const int nControlHeight = 20;

	int nControlTop = nTop;
	new wxStaticText(this, wxID_ANY, strLabelLine1, wxPoint(nLeftBorder, nControlTop), wxDefaultSize);
	nControlTop += 25;
	new wxStaticText(this, wxID_ANY, strLabelLine2, wxPoint(nLeftBorder, nControlTop), wxDefaultSize);

	nControlTop += 50;
	new wxStaticText(this, wxID_ANY, "���ڼ�����ʽ�����", wxPoint(nLeftBorder, nControlTop), wxSize(120, nControlHeight));
	m_pCbxAccountType = new wxOwnerDrawnComboBox(this, ID_COMBOX, "", wxPoint(nLeftBorder+125, nControlTop), wxSize(100, nControlHeight),0, wxCB_READONLY|wxST_NO_AUTORESIZE);
	m_pCbxAccountType->Append("��̬Ȩ��");
	m_pCbxAccountType->Append("�����ʽ�");
	m_pCbxAccountType->Append("��̬Ȩ��");

	ReadCfg();
	InitControls();
}

COrderAccountTypePanel::~COrderAccountTypePanel()
{

}

void COrderAccountTypePanel::InitControls()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return;

	MULTIACCOUNT* pMultiAccount = pMgr->GetMultiAccountCfg();
	switch(m_eStrategy)
	{
	case FirstAccountAlign:
		m_pCbxAccountType->SetSelection(pMultiAccount->nFirstAccountAlign);
		break;
	case AccountScaleDivided:
		m_pCbxAccountType->SetSelection(pMultiAccount->nAccountScaleDivided);
		break;
	default:
		break;
	}
}

bool COrderAccountTypePanel::WriteCfg()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return false;

	MULTIACCOUNT* pMultiAccount = pMgr->GetMultiAccountCfg();
	switch(m_eStrategy)
	{
	case FirstAccountAlign:
		pMultiAccount->nFirstAccountAlign = m_pCbxAccountType->GetSelection();
		break;
	case AccountScaleDivided:
		pMultiAccount->nAccountScaleDivided = m_pCbxAccountType->GetSelection();
		break;
	default:
		break;
	}

	return true;
}

void COrderAccountTypePanel::ReadCfg()
{

}


