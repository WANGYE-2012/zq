#include "stdafx.h"

#include <vector>
#include <string>
#include "..\\inc\\cfgMgr\\CfgMgr.h"
#include "CQuickOrderSetPanel.h"


BEGIN_EVENT_TABLE(CQuickOrderSetPanel, wxPanel)

END_EVENT_TABLE()

CQuickOrderSetPanel::CQuickOrderSetPanel(wxWindow *parent,
							   wxWindowID winid ,
							   const wxPoint& pos ,
							   const wxSize& size ,
							   long style,
							   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{
	const int nLeftBorder = 10;
	const int nTop = 5;
	const int nRowDistance = 8;
	const int nControlHeight = 20;

	int nControlTop = nTop;
	new wxStaticText(this, wxID_ANY, "���㱨�۱����̿��ٹ��ܣ�", wxPoint(nLeftBorder, nControlTop), wxDefaultSize);

	//���޷���ֲ�ʱ
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	new wxStaticText(this, wxID_ANY, "���޷���ֲ�ʱ", wxPoint(nLeftBorder+20, nControlTop), wxDefaultSize);
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	m_pComNoOppositePosition = new wxOwnerDrawnComboBox(this, ID_OPPOSITE_POSITION1, "", wxPoint(nLeftBorder+25, nControlTop), 
		wxSize(150, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);
	m_pComNoOppositePosition->Append("��Ĭ�������Ͳ��Կ���");

	//�����з���ֲ�ʱ
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	new wxStaticText(this, wxID_ANY, "�����з���ֲ�ʱ", wxPoint(nLeftBorder+20, nControlTop), wxDefaultSize);
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	m_pComSomeHaveOppositePosition = new wxOwnerDrawnComboBox(this, ID_OPPOSITE_POSITION2, "", wxPoint(nLeftBorder+25, nControlTop), 
		wxSize(300, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);
	m_pComSomeHaveOppositePosition->Append("��Ĭ�������Ͳ��Կ���");
	m_pComSomeHaveOppositePosition->Append("���ֲ���ȫƽ");
	m_pComSomeHaveOppositePosition->Append("�в��˻���Ĭ�������Ͳ���ƽ�֣������������޲��˻�������");
	m_pComSomeHaveOppositePosition->Append("�в��˻���Ĭ�������Ͳ���ƽ�֣��������򿪲֣��޲��˻�������");
	m_pComSomeHaveOppositePosition->Append("��Ĭ�������Ͳ��ԣ��в��˻�ƽ�֣������������޲��˻�����");
	m_pComSomeHaveOppositePosition->Append("��Ĭ�������Ͳ��ԣ��в��˻�ƽ�֣��������򿪲֣��޲��˻�����");

	//���з���ֲ�ʱ
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	new wxStaticText(this, wxID_ANY, "���з���ֲ�ʱ", wxPoint(nLeftBorder+20, nControlTop), wxDefaultSize);
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	m_pComAllHaveOppositePosition = new wxOwnerDrawnComboBox(this, ID_OPPOSITE_POSITION3, "", wxPoint(nLeftBorder+25, nControlTop), 
		wxSize(200, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);
	m_pComAllHaveOppositePosition->Append("��Ĭ�������Ͳ��Կ���");
	m_pComAllHaveOppositePosition->Append("���ֲ���ȫƽ");
	m_pComAllHaveOppositePosition->Append("��Ĭ�������Ͳ���ƽ�֣�����������");
	m_pComAllHaveOppositePosition->Append("��Ĭ�������Ͳ���ƽ�֣��������򿪲�");

	//����ֲֻ�ֲֻ���
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	new wxStaticText(this, wxID_ANY, "����ֲֻ�ֲֻ��ܣ�", wxPoint(nLeftBorder, nControlTop), wxDefaultSize);
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	m_pComMouseClosePosition = new wxOwnerDrawnComboBox(this, ID_MOUSE_CLOSE_POSITION, "", wxPoint(nLeftBorder+25, nControlTop), 
		wxSize(150, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);
	m_pComMouseClosePosition->Append("���ֲ���ȫƽ");

	nControlTop = nControlTop + nControlHeight + nRowDistance;
	m_pChkActionBeforeOpenOrder = new wxCheckBox(this, ID_ACTION_BEFORE_OPEN_ORDER, "�µ�ǰ�ȳ�ԭͬ����ҵ�", wxPoint(nLeftBorder, nControlTop), wxDefaultSize);

	nControlTop = nControlTop + nControlHeight + nRowDistance;
	new wxStaticText(this, wxID_ANY, "ע�����˻��Զ���ƽ����������Ч�˻���Ϊһ�����ڲ����з���ֲ�ʱ�����в��˻�",
		wxPoint(nLeftBorder, nControlTop), wxDefaultSize);
	nControlTop = nControlTop + nControlHeight + nRowDistance;
	new wxStaticText(this, wxID_ANY, "�������������˵���顣", 
		wxPoint(nLeftBorder, nControlTop), wxDefaultSize);

	ReadCfg();
	InitControls();
}

CQuickOrderSetPanel::~CQuickOrderSetPanel()
{

}

void CQuickOrderSetPanel::InitControls()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	LPORDER_CFG pOrderCfg = pMgr->GetOrderCfg();
	wxString strLable = GetStrategyString(pOrderCfg->nAllNotHaveOppositePosition);
	m_pComNoOppositePosition->SetStringSelection(strLable);

	strLable = GetStrategyString(pOrderCfg->nSomeHaveOppositePosition);
	m_pComSomeHaveOppositePosition->SetStringSelection(strLable);

	strLable = GetStrategyString(pOrderCfg->nAllHaveOppositePosition);
	m_pComAllHaveOppositePosition->SetStringSelection(strLable);

	strLable = GetStrategyString(pOrderCfg->nMouseClosePositionStrategy);
	m_pComMouseClosePosition->SetStringSelection(strLable);

	m_pChkActionBeforeOpenOrder->SetValue(pOrderCfg->nCancelSameDirOpenOrderBeforeOrder>0 );

}

bool CQuickOrderSetPanel::WriteCfg()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return false;

	LPORDER_CFG pOrderCfg = pMgr->GetOrderCfg();
	wxString strLable = m_pComNoOppositePosition->GetStringSelection();
	pOrderCfg->nAllNotHaveOppositePosition = GetStrategyInt(strLable);

	strLable = m_pComSomeHaveOppositePosition->GetStringSelection();
	pOrderCfg->nSomeHaveOppositePosition = GetStrategyInt(strLable);

	strLable = m_pComAllHaveOppositePosition->GetStringSelection();
	pOrderCfg->nAllHaveOppositePosition = GetStrategyInt(strLable);

	strLable = m_pComMouseClosePosition->GetStringSelection();
	pOrderCfg->nMouseClosePositionStrategy = GetStrategyInt(strLable);

	if ( m_pChkActionBeforeOpenOrder->GetValue() )
	{
		pOrderCfg->nCancelSameDirOpenOrderBeforeOrder = 1;
	}
	else
	{
		pOrderCfg->nCancelSameDirOpenOrderBeforeOrder = 0;
	}

	return true;
}

void CQuickOrderSetPanel::ReadCfg()
{

}

wxString CQuickOrderSetPanel::GetStrategyString( int nQuickOrderStrategy )
{
	wxString strLable;
	switch(nQuickOrderStrategy)
	{
	case QOrder_OpenDefaultAndStrategy:
		strLable = "��Ĭ�������Ͳ��Կ���";
		break;
	case QOrder_CloseAll:
		strLable = "���ֲ���ȫƽ";
		break;
	case QOrder_AutoCloseDefaultAndNotOpen:
		strLable = "�в��˻���Ĭ�������Ͳ���ƽ�֣������������޲��˻�������";
		break;
	case QOrder_AutoReverseOpenAndNotOpen:
		strLable = "�в��˻���Ĭ�������Ͳ���ƽ�֣��������򿪲֣��޲��˻�������";
		break;
	case QOrder_AutoCloseDefaultAndOpen:
		strLable = "��Ĭ�������Ͳ��ԣ��в��˻�ƽ�֣������������޲��˻�����";
		break;
	case QOrder_AutoReverseOpenAndOpen:
		strLable = "��Ĭ�������Ͳ��ԣ��в��˻�ƽ�֣��������򿪲֣��޲��˻�����";
		break;
	case QOrder_AutoCloseDefault:
		strLable = "��Ĭ�������Ͳ���ƽ�֣�����������";
		break;
	case QOrder_AutoReverseOpen:
		strLable = "��Ĭ�������Ͳ���ƽ�֣��������򿪲�";
		break;
	default:
		break;
	}

	return strLable;
}

int CQuickOrderSetPanel::GetStrategyInt( const wxString& strLable )
{
	if ( strLable == "��Ĭ�������Ͳ��Կ���")
	{
		return QOrder_OpenDefaultAndStrategy;
	}
	else if ( strLable == "���ֲ���ȫƽ")
	{
		return QOrder_CloseAll;
	}
	else if ( strLable == "�в��˻���Ĭ�������Ͳ���ƽ�֣������������޲��˻�������")
	{
		return QOrder_AutoCloseDefaultAndNotOpen;
	}
	else if ( strLable == "�в��˻���Ĭ�������Ͳ���ƽ�֣��������򿪲֣��޲��˻�������")
	{
		return QOrder_AutoReverseOpenAndNotOpen;
	}
	else if ( strLable == "��Ĭ�������Ͳ��ԣ��в��˻�ƽ�֣������������޲��˻�����")
	{
		return QOrder_AutoCloseDefaultAndOpen;
	}
	else if ( strLable == "��Ĭ�������Ͳ��ԣ��в��˻�ƽ�֣��������򿪲֣��޲��˻�����")
	{
		return QOrder_AutoReverseOpenAndOpen;
	}
	else if ( strLable == "��Ĭ�������Ͳ���ƽ�֣�����������")
	{
		return QOrder_AutoCloseDefault;
	}
	else if ( strLable == "��Ĭ�������Ͳ���ƽ�֣��������򿪲�")
	{
		return QOrder_AutoReverseOpen;
	}
	else
	{
		return -1;
	}
}

