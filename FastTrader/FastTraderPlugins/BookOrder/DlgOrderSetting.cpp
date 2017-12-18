// DlgOrderSetting.cpp : implementation file
//

#include "stdafx.h"
#include "Module-BookOrder.h"
#include "DlgOrderSetting.h"


// CDlgOrderSetting dialog

IMPLEMENT_DYNAMIC(CDlgOrderSetting, CDialog)

CDlgOrderSetting::CDlgOrderSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOrderSetting::IDD, pParent)
	, m_nOrderOffset(0)
	, m_bHasLocalSetting(FALSE)
{

}

CDlgOrderSetting::~CDlgOrderSetting()
{
}

void CDlgOrderSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_AutoOCKey, m_cmbAuotOCKey);
	DDX_Control(pDX, IDC_COMBO_OpenKey, m_cmbOpenKey);
	DDX_Control(pDX, IDC_COMBO_CloseKey, m_cmbCloseKey);
	DDX_Control(pDX, IDC_COMBO_CondOrderPriceMode, m_cmbCondOrderPriceMode);
	DDX_Text(pDX, IDC_EDIT_ORDEROFFSET, m_nOrderOffset);
	DDV_MinMaxInt(pDX, m_nOrderOffset, -10000, 10000);
	DDX_Check(pDX, IDC_CHECK_HASLOCALSETTING, m_bHasLocalSetting);
}


BEGIN_MESSAGE_MAP(CDlgOrderSetting, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgOrderSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgOrderSetting::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgOrderSetting message handlers

BOOL CDlgOrderSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_cmbAuotOCKey.AddString("");
	m_cmbAuotOCKey.AddString("Shift");
	m_cmbAuotOCKey.AddString("Ctrl");
	m_cmbAuotOCKey.AddString("Alt");
	m_cmbAuotOCKey.SetCurSel(m_orderSetting.nAutoOrderKey);

	m_cmbOpenKey.AddString("");
	m_cmbOpenKey.AddString("Shift");
	m_cmbOpenKey.AddString("Ctrl");
	m_cmbOpenKey.AddString("Alt");
	m_cmbOpenKey.SetCurSel(m_orderSetting.nOpenOrderKey);

	m_cmbCloseKey.AddString("");
	m_cmbCloseKey.AddString("Shift");
	m_cmbCloseKey.AddString("Ctrl");
	m_cmbCloseKey.AddString("Alt");
	m_cmbCloseKey.SetCurSel(m_orderSetting.nCloseOrderKey);

	m_cmbCondOrderPriceMode.AddString("�м�");
	m_cmbCondOrderPriceMode.AddString("�޼�");
	m_cmbCondOrderPriceMode.AddString("������");
	m_cmbCondOrderPriceMode.SetCurSel(m_orderSetting.nCondOrderPriceMode);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOrderSetting::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	if(!UpdateData())
		return;

	if(m_cmbAuotOCKey.GetCurSel()==m_cmbOpenKey.GetCurSel() || 
			m_cmbOpenKey.GetCurSel()==m_cmbCloseKey.GetCurSel() ||
			m_cmbAuotOCKey.GetCurSel()==m_cmbCloseKey.GetCurSel()) {
		MessageBox("�Զ�/����/ƽ�ֿ�ݼ��������ظ�����������ύ��");
		return;
	}

	if(m_cmbCondOrderPriceMode.GetCurSel()==conOrderByMarket) {
		MessageBox("�������н�������֧���м۵�����ȷ����Ҫ���׵ĺ�Լ�Ƿ�֧���м۵���");
	}

	m_orderSetting.nAutoOrderKey = m_cmbAuotOCKey.GetCurSel();
	m_orderSetting.nOpenOrderKey = m_cmbOpenKey.GetCurSel();
	m_orderSetting.nCloseOrderKey = m_cmbCloseKey.GetCurSel();
	m_orderSetting.nCondOrderPriceMode = m_cmbCondOrderPriceMode.GetCurSel();
	m_orderSetting.nCondOrderOffset = m_nOrderOffset;

	OnOK();
}

void CDlgOrderSetting::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
