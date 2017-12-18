// RiskManageView.cpp : CRiskManageView ���ʵ��
//

#include "stdafx.h"
#include "RiskManage.h"

#include "RiskManageDoc.h"
#include "RiskManageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRiskManageView

IMPLEMENT_DYNCREATE(CRiskManageView, CFormView)

BEGIN_MESSAGE_MAP(CRiskManageView, CFormView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_VIEW, &CRiskManageView::OnTcnSelchangeTabView)
END_MESSAGE_MAP()

// CRiskManageView ����/����

CRiskManageView::CRiskManageView()
	: CFormView(CRiskManageView::IDD)
	, m_bInited(false)
{
	// TODO: �ڴ˴���ӹ������

}

CRiskManageView::~CRiskManageView()
{
}

void CRiskManageView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_VIEW, m_mainTab);
}

void CRiskManageView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_mainTab.InsertItem(0,"ί�н����˻�");
	m_mainTab.InsertItem(1,"����Ա/���Ա");
	m_mainTab.InsertItem(2,"����Ա");

	m_manageAccountDlg.Create(IDD_DIALOG_ACCOUNT, &m_mainTab);
	m_trustTradeAccountDlg.Create(IDD_DIALOG_ACCOUNT, &m_mainTab);
	m_traderAccountDlg.Create(IDD_DIALOG_ACCOUNT, &m_mainTab);

	//�����б�
	std::vector<CString> vManageTitle;
	vManageTitle.push_back(_T("�˻���"));
	vManageTitle.push_back(_T("��ɫ"));
	vManageTitle.push_back(_T("Ȩ��"));
	vManageTitle.push_back(_T("�����ʹ�"));
	vManageTitle.push_back(_T("��Ʋ�Ʒ"));
	vManageTitle.push_back(_T("�˺�״̬"));
	vManageTitle.push_back(_T("�û���"));
	vManageTitle.push_back(_T("��ϵ��ʽ"));
	m_manageAccountDlg.CreateListColumn(vManageTitle);
	m_manageAccountDlg.SetAccountType(ACCOUNT_MANAGE);

	std::vector<CString> vTradeTitle;
	vTradeTitle.push_back(_T("�˻���"));
	vTradeTitle.push_back(_T("�����ʹ�"));
	vTradeTitle.push_back(_T("��Ʋ�Ʒ"));
	vTradeTitle.push_back(_T("����ģ��"));
	vTradeTitle.push_back(_T("�û���"));
	vTradeTitle.push_back(_T("��ϵ��ʽ"));
	vTradeTitle.push_back(_T("֤������"));
	vTradeTitle.push_back(_T("֤������"));
	vTradeTitle.push_back(_T("����Ա"));
	vTradeTitle.push_back(_T("��ʼȨ��"));
	m_trustTradeAccountDlg.CreateListColumn(vTradeTitle);
	m_trustTradeAccountDlg.SetAccountType(ACCOUNT_TRUST_TRADE);

	std::vector<CString> vTraderTitle;
	vTraderTitle.push_back(_T("����"));
	vTraderTitle.push_back(_T("�����ʹ�"));
	vTraderTitle.push_back(_T("��Ʋ�Ʒ"));
	vTraderTitle.push_back(_T("����"));
	vTraderTitle.push_back(_T("�ֻ�"));
	vTraderTitle.push_back(_T("������ϵ��ʽ"));
	m_traderAccountDlg.CreateListColumn(vTraderTitle);
	m_traderAccountDlg.SetAccountType(ACCOUNT_TRADER);

	CRect rc;
	m_mainTab.GetClientRect(rc);
	CRect rcTabItem;
	m_mainTab.GetItemRect(0, rcTabItem);
	rc.top += rcTabItem.Height() + 2;
	m_manageAccountDlg.MoveWindow(rc);
	m_trustTradeAccountDlg.MoveWindow(rc);
	m_traderAccountDlg.MoveWindow(rc);

	m_trustTradeAccountDlg.ShowWindow(SW_SHOW);
	m_manageAccountDlg.ShowWindow(SW_HIDE);
	m_traderAccountDlg.ShowWindow(SW_HIDE);
	m_mainTab.SetCurSel(0);

	m_bInited = true;
}

BOOL CRiskManageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CRiskManageView ����

void CRiskManageView::OnDraw(CDC* /*pDC*/)
{
	CRiskManageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CRiskManageView ��ӡ

BOOL CRiskManageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CRiskManageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CRiskManageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CRiskManageView ���

#ifdef _DEBUG
void CRiskManageView::AssertValid() const
{
	CView::AssertValid();
}

void CRiskManageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRiskManageDoc* CRiskManageView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRiskManageDoc)));
	return (CRiskManageDoc*)m_pDocument;
}
#endif //_DEBUG


// CRiskManageView ��Ϣ�������

void CRiskManageView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if ( m_bInited )
	{
		CRect rc;
		GetClientRect(rc);
		m_mainTab.MoveWindow(rc);

		m_mainTab.GetClientRect(rc);
		CRect rcTabItem;
		m_mainTab.GetItemRect(0, rcTabItem);
		rc.top += rcTabItem.Height() + 2;
		m_manageAccountDlg.MoveWindow(rc);
		m_trustTradeAccountDlg.MoveWindow(rc);
		m_traderAccountDlg.MoveWindow(rc);
	}
}

int CRiskManageView::OnCreate( LPCREATESTRUCT lpcs )
{
	if (CFormView::OnCreate(lpcs) == -1)
		return -1;

	return 0;
}

void CRiskManageView::OnTcnSelchangeTabView(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nCurSel = m_mainTab.GetCurSel();
	int nAccountID = 0;
	switch(nCurSel)
	{
	case ACCOUNT_TRUST_TRADE:
		m_manageAccountDlg.ShowWindow(SW_HIDE);
		m_trustTradeAccountDlg.ShowWindow(SW_SHOW);
		m_traderAccountDlg.ShowWindow(SW_HIDE);
		break;
	case ACCOUNT_MANAGE:
		m_trustTradeAccountDlg.ShowWindow(SW_HIDE);
		m_manageAccountDlg.ShowWindow(SW_SHOW);
		m_traderAccountDlg.ShowWindow(SW_HIDE);
		break;
	case ACCOUNT_TRADER:
		m_manageAccountDlg.ShowWindow(SW_HIDE);
		m_trustTradeAccountDlg.ShowWindow(SW_HIDE);
		m_traderAccountDlg.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}

	AfxGetMainWnd()->SendMessage(MSG_MAIN_TAB_SEL_CHANGE, 0, 0);
}

int CRiskManageView::GetCurPageIndex()
{
	return m_mainTab.GetCurSel();
}

AccountType CRiskManageView::GetAccountType()
{
	return static_cast<AccountType>(m_mainTab.GetCurSel());
}

void CRiskManageView::GetSelectAccountID(std::vector<int>& vec)
{
	int nCurSel = m_mainTab.GetCurSel();
	switch(nCurSel)
	{
	case ACCOUNT_TRUST_TRADE:
		m_trustTradeAccountDlg.GetSelectAccountID(vec);
		break;
	case ACCOUNT_MANAGE:
		m_manageAccountDlg.GetSelectAccountID(vec);
		break;
	case ACCOUNT_TRADER:
		m_traderAccountDlg.GetSelectAccountID(vec);
		break;
	default:
		break;
	}

	return;
}

void CRiskManageView::ClearAllData()
{
	m_trustTradeAccountDlg.ClearData();
	m_manageAccountDlg.ClearData();
	m_traderAccountDlg.ClearData();
}

void CRiskManageView::ClearData(AccountType eType)
{
	if ( eType == ACCOUNT_MANAGE )
	{
		m_manageAccountDlg.ClearData();
	}
	else if( eType == ACCOUNT_TRUST_TRADE )
	{
		m_trustTradeAccountDlg.ClearData();
	}
	else
	{
		m_traderAccountDlg.ClearData();
	}
}

void CRiskManageView::SetData( std::map<int, RiskMgmtUserInfoEx>& accountMap )
{
	m_manageAccountDlg.ClearData();
	std::vector<CAccountListData> vListData;

	std::map<int, RiskMgmtUserInfoEx>::iterator it = accountMap.begin();
	for ( ; it != accountMap.end(); ++it )
	{
		CAccountListData listData;
		GenerateListData(it->second, listData);
		vListData.push_back(listData);
	}

	m_manageAccountDlg.SetData(vListData);
}

void CRiskManageView::SetData( std::map<int, TrustTradeAccountEx>& trustTradeMap )
{
	m_trustTradeAccountDlg.ClearData();
	std::vector<CAccountListData> vListData;

	DWORD t1 = GetTickCount();
	std::map<int, TrustTradeAccountEx>::iterator it = trustTradeMap.begin();
	for ( ; it != trustTradeMap.end(); ++it )
	{
		CAccountListData listData;
		GenerateListData(it->second, listData);
		vListData.push_back(listData);
	}

	DWORD t2 = GetTickCount();
	char szBuffer[100];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "step 1:%d\n", t2-t1);
	OutputDebugString(szBuffer);

	m_trustTradeAccountDlg.SetData(vListData);

	DWORD t3 = GetTickCount();
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "step 2:%d\n", t3-t2);
	OutputDebugString(szBuffer);
}

void CRiskManageView::SetData( std::map<int, TraderInfoEx>& traderMap )
{
	m_traderAccountDlg.ClearData();
	std::vector<CAccountListData> vListData;

	std::map<int, TraderInfoEx>::iterator it = traderMap.begin();
	for ( ; it != traderMap.end(); ++it )
	{
		CAccountListData listData;
		GenerateListData(it->second, listData);
		vListData.push_back(listData);
	}

	m_traderAccountDlg.SetData(vListData);
}

void CRiskManageView::AddAccount( const RiskMgmtUserInfoEx& userInfoEx )
{
	CAccountListData listData;
	GenerateListData(userInfoEx, listData);
	m_manageAccountDlg.AddAccount(listData);
}

void CRiskManageView::AddAccount( const TrustTradeAccountEx& trustTradeEx )
{
	CAccountListData listData;
	GenerateListData(trustTradeEx, listData);
	m_trustTradeAccountDlg.AddAccount(listData);
}

void CRiskManageView::AddAccount( const TraderInfoEx& traderInfoEx )
{
	CAccountListData listData;
	GenerateListData(traderInfoEx, listData);
	m_traderAccountDlg.AddAccount(listData);
}

void CRiskManageView::ModifyAccount( const TraderInfoEx& traderEx )
{
	CAccountListData listData;
	GenerateListData(traderEx, listData);
	m_traderAccountDlg.ModifyAccount(listData);
}

void CRiskManageView::ModifyAccount( const RiskMgmtUserInfoEx& userInfoEx )
{
	CAccountListData listData;
	GenerateListData(userInfoEx, listData);
	m_manageAccountDlg.ModifyAccount(listData);
}

void CRiskManageView::ModifyAccount( const TrustTradeAccountEx& trustTradeEx )
{
	CAccountListData listData;
	GenerateListData(trustTradeEx, listData);
	m_trustTradeAccountDlg.ModifyAccount(listData);
}

void CRiskManageView::DelAccount( int nAccountID, AccountType eType )
{
	if ( eType == ACCOUNT_TRUST_TRADE)
	{
		m_trustTradeAccountDlg.DelAccount(nAccountID);
	}
	else if ( eType == ACCOUNT_MANAGE )
	{
		m_manageAccountDlg.DelAccount(nAccountID);
	}
	else
	{
		m_traderAccountDlg.DelAccount(nAccountID);
	}
}

void CRiskManageView::ShowListData( AccountType eType, bool bShow )
{
	
}

void CRiskManageView::GenerateListData( const RiskMgmtUserInfoEx &userInfoEx, CAccountListData &listData )
{
	listData.SetKey(userInfoEx.userInfo.nRiskMgmtUserID);
	listData.AddValue(userInfoEx.userInfo.szAccount);
	if ( userInfoEx.userInfo.nRoleType == ROLE_TYPE_MANAGE )
	{
		listData.AddValue(_T("����Ա"));
	}
	else
	{
		listData.AddValue(_T("���Ա"));
	}
	listData.AddValue(userInfoEx.szPermissionName);
	listData.AddValue(userInfoEx.szAssetMgmtOrgName);
	listData.AddValue(userInfoEx.szFinancialProductName);
	if ( userInfoEx.userInfo.nStatus == 0 )
	{
		listData.AddValue(_T("δ����"));
	}
	else
	{
		listData.AddValue(_T("����"));
	}
	listData.AddValue(userInfoEx.userInfo.szName);
	listData.AddValue(userInfoEx.userInfo.szContactInfo);
}

void CRiskManageView::GenerateListData( const TrustTradeAccountEx &trustTradeEx, CAccountListData &listData )
{
	listData.SetKey(trustTradeEx.trustAccount.nTradeAccountID);
	listData.AddValue(trustTradeEx.trustAccount.szAccount);
	listData.AddValue(trustTradeEx.szAssetMgmtOrgName);
	listData.AddValue(trustTradeEx.szFinancialProductName);
	listData.AddValue(trustTradeEx.szRiskIncModuleName);
	listData.AddValue(trustTradeEx.trustAccount.szName);
	listData.AddValue(trustTradeEx.trustAccount.szContactInfo);
	if ( trustTradeEx.trustAccount.nIDCardType == IDENTIFY_CARD )
	{
		listData.AddValue(_T("���֤"));
	}
	else if( trustTradeEx.trustAccount.nIDCardType == MILITARY_ID )
	{
		listData.AddValue(_T("����֤"));
	}
	else
	{
		listData.AddValue(_T("����"));
	}
	listData.AddValue(trustTradeEx.trustAccount.szIDCardNo);
	listData.AddValue(trustTradeEx.szTraderName);

	CString csTemp = GetAccountFormatString(trustTradeEx.trustAccount.dProfit, 2);
	listData.AddValue(csTemp);
}

void CRiskManageView::GenerateListData( const TraderInfoEx &traderInfoEx, CAccountListData &listData )
{
	listData.SetKey(traderInfoEx.traderInfo.nTraderID);
	listData.AddValue(traderInfoEx.traderInfo.szName);
	listData.AddValue(traderInfoEx.szAssetMgmtOrgName);
	listData.AddValue(traderInfoEx.szFinancialProductName);
	listData.AddValue(traderInfoEx.traderInfo.szTel);
	listData.AddValue(traderInfoEx.traderInfo.szMobile);
	listData.AddValue(traderInfoEx.traderInfo.szOtherContactInfo);
}

//void CRiskManageView::SetPermissionData( const PermissionData& foreData, 
//											const PermissionData& bkgData )
//{
//	m_tradeAccountDlg.SetPermissionData(foreData);
//	m_manageAccountDlg.SetPermissionData(bkgData);
//}
