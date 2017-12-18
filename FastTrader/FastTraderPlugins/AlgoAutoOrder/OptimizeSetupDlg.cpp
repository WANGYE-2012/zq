// OptimizeSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AlgoAutoOrder.h"
#include "OptimizeSetupDlg.h"
#include "Tools_AlgoTrading.h"
#include "GeneralParamDlg.h"
#include "AlgoAutoOrderDlg.h"
#include "AlgoAutoData.h"
#include "Module-Misc2/tools_util.h"


// COptimizeSetupDlg dialog

IMPLEMENT_DYNAMIC(COptimizeSetupDlg, CDialog)

COptimizeSetupDlg::COptimizeSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptimizeSetupDlg::IDD, pParent)
    , m_nOptNameSelect(-1)
    , m_nOptDetilSelect(-1)
    , m_pGeneralParamDlg(NULL)
    , m_comboPrcieChangePreCurSel(0)
{
}

COptimizeSetupDlg::~COptimizeSetupDlg()
{

}

void COptimizeSetupDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_NAME, m_listName);
    DDX_Control(pDX, IDC_LIST_DETIL, m_listDetil);
    DDX_Control(pDX, IDC_BUTTON_NAMEADD, m_btnNameAdd);
    DDX_Control(pDX, IDC_BUTTON_NAMEDEL, m_btnNameDel);
    DDX_Control(pDX, IDC_BUTTON_DETILADD, m_btnDetilAdd);
    DDX_Control(pDX, IDC_BUTTON_DETILDEL, m_btnDetilDel);
    DDX_Control(pDX, IDC_BUTTON_SAVESETUP, m_btnSaveSetup);
    DDX_Control(pDX, IDC_COMBO_TIME, m_comboTime);
    DDX_Control(pDX, IDC_COMBO_PRICE, m_comboPrice);
//     DDX_Control(pDX, IDC_COMBO_SLIDEPRICE, m_comboSlidePrice);
//     DDX_Control(pDX, IDC_COMBO_RECOUNT, m_comboReCount);
    DDX_Control(pDX, IDC_EDIT_PRICEFROM, m_editPriceFrom);
    DDX_Control(pDX, IDC_EDIT_PRICETO, m_editPriceTo);
    DDX_Control(pDX, IDC_EDIT_MULTIPLE, m_editMultiple);
    DDX_Control(pDX, IDC_EDIT_MARKET, m_editMarket);
    DDX_Control(pDX, IDC_EDIT_SLIDEPRICE1, m_editSlidePrice1);
    DDX_Control(pDX, IDC_EDIT_SLIDEPRICE2, m_editSlidePrice2);
    DDX_Control(pDX, IDC_EDIT_SLIDEPRICE3, m_editSlidePrice3);
    DDX_Control(pDX, IDC_EDIT_SLIDEPRICE4, m_editSlidePrice4);
    DDX_Control(pDX, IDC_EDIT_SLIDEPRICE5, m_editSlidePrice5);
    DDX_Control(pDX, IDC_EDIT_RECOUNT1, m_editReCount1);
    DDX_Control(pDX, IDC_EDIT_RECOUNT2, m_editReCount2);
    DDX_Control(pDX, IDC_EDIT_RECOUNT3, m_editReCount3);
    DDX_Control(pDX, IDC_EDIT_RECOUNT4, m_editReCount4);
    DDX_Control(pDX, IDC_EDIT_RECOUNT5, m_editReCount5);
    DDX_Control(pDX, IDC_SLIDER_MARKET, m_sliderMarket);
    DDX_Control(pDX, IDC_SPIN_MARKET, m_spinMarket);
    DDX_Control(pDX, IDC_STATIC_RULE, m_staticRule);
    DDX_Control(pDX, IDC_EDIT_LISTNAME, m_editListName);
}


BEGIN_MESSAGE_MAP(COptimizeSetupDlg, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_NAMEADD, &COptimizeSetupDlg::OnBnClickedButtonNameadd)
    ON_BN_CLICKED(IDC_BUTTON_NAMEDELETE, &COptimizeSetupDlg::OnBnClickedButtonNamedelete)
    ON_BN_CLICKED(IDC_BUTTON_DETILADD, &COptimizeSetupDlg::OnBnClickedButtonDetiladd)
    ON_BN_CLICKED(IDC_BUTTON_DETILDELETE, &COptimizeSetupDlg::OnBnClickedButtonDetildelete)
    ON_BN_CLICKED(IDC_BUTTON_SAVESETUP, &COptimizeSetupDlg::OnBnClickedButtonSavesetup)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MARKET, &COptimizeSetupDlg::OnDeltaposSpinMarket)
    ON_EN_CHANGE(IDC_EDIT_MARKET, &COptimizeSetupDlg::OnEnChangeEditMarket)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MARKET, &COptimizeSetupDlg::OnNMCustomdrawSliderMarket)
    ON_WM_HSCROLL()
    ON_WM_PAINT()
    ON_NOTIFY(NM_CLICK, IDC_LIST_NAME, &COptimizeSetupDlg::OnNMClickListName)
    ON_NOTIFY(NM_CLICK, IDC_LIST_DETIL, &COptimizeSetupDlg::OnNMClickListDetil)
    ON_CBN_SELCHANGE(IDC_COMBO_PRICE, &COptimizeSetupDlg::OnCbnSelchangeComboPrice)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_NAME, &COptimizeSetupDlg::OnNMDblclkListName)
    ON_EN_KILLFOCUS(IDC_EDIT_LISTNAME, &COptimizeSetupDlg::OnEnKillfocusEditListname)
    ON_BN_CLICKED(IDC_BUTTON2, &COptimizeSetupDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// COptimizeSetupDlg message handlers

BOOL COptimizeSetupDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    SetWindowText("��������");

    DWORD	NewStyle = LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT;
    m_listName.SetExtendedStyle(NewStyle);
    m_listName.InsertColumn(0,"������");
    m_listName.SetColumnWidth(0,170);

    m_listDetil.SetExtendedStyle(NewStyle);
    m_listDetil.InsertColumn(0,"��Χ");
    m_listDetil.InsertColumn(1,"�Ż�����");
    m_listDetil.InsertColumn(2,"��ʷ�۸�");
    m_listDetil.SetColumnWidth(0,100);
    m_listDetil.SetColumnWidth(1,230);
    m_listDetil.SetColumnWidth(2,60);

    m_comboTime.InsertString(0, "15��");
    m_comboTime.InsertString(1, "30��");
    m_comboTime.InsertString(2, "1����");
    m_comboTime.InsertString(3, "5����");
    m_comboTime.InsertString(4, "15����");
    m_comboTime.InsertString(5, "30����");
    m_comboTime.InsertString(6, "1Сʱ");
    m_comboTime.SetCurSel(0);


    m_comboPrice.InsertString(0, "�۸�");
    m_comboPrice.InsertString(1, "�ٷֱ�");
    m_comboPrice.SetCurSel(0);


//     m_comboSlidePrice.InsertString(0, "�ٷֱ�");
//     m_comboSlidePrice.InsertString(1, "��λ");
//     m_comboSlidePrice.SetCurSel(1);

//     m_comboReCount.InsertString(0, "�ٷֱ�");
//     m_comboReCount.InsertString(1, "����");
//     m_comboReCount.SetCurSel(1);


    m_sliderMarket.SetRange(0,100);
    m_sliderMarket.SetTicFreq(10);
    m_sliderMarket.SetPos(0);

    m_btnNameAdd.LoadBitmaps(IDB_BITMAP_ADD,1,1,1,1,1); 
    m_btnNameDel.LoadBitmaps(IDB_BITMAP_DEL,1,1,1,1,1);

    m_btnDetilAdd.LoadBitmaps(IDB_BITMAP_ADD,1,1,1,1,1); 
    m_btnDetilDel.LoadBitmaps(IDB_BITMAP_DEL,1,1,1,1,1);

    m_btnSaveSetup.LoadBitmaps(IDB_BITMAP_DEFAULT,1,1,1,1,1); 

    AddToolTip();
    SetOptDataToWnd();   

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void COptimizeSetupDlg::AddToolTip()
{
    EnableToolTips(TRUE);
    m_toolTip.Create(this);
    m_toolTip.Activate(TRUE);
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_COMBO_TIME),"ʵʱ��ص�ǰʱ�����ǰһ������ʱ����ڵļ۸�䶯�Ƿ�������������������");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_COMBO_PRICE),"��ѡ��Ϊ�۸���۸�䶯=��ǰ���¼ۼ�ȥ����ʱ�������¼ۡ���ѡ��Ϊ�ٷֱȣ���۸�䶯=��ǰ���¼ۼ�ȥ����ʱ�������¼۵Ĳ���Ի���ʱ�������¼ۡ��۸�䶯��Χ�����������ֵ����С���Ҳ�������ֵ��������õķ�Χ�����ظ����۵�����ͻ��");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_PRICEFROM),"�۸�䶯��ʼ��(������)��");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_PRICETO),"�۸�䶯������(����)��");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_MULTIPLE),"��Ч���µ���Ӧ���Լ��ٱ������ó��µ���ֵ�����µ�����Чһ�κ󼴻ָ�Ĭ�����á���ÿ�����ʱ����ڶ�δ������������һ��Ϊ׼��");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_MARKET),"���۸���������䶯ʱ�����ڳɱ��ɿأ��û��������ø��ߵ��г�����ȡ��������ʾ�������ޡ�");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_MARKET),"���۸���������䶯ʱ�����ڳɱ��ɿأ��û��������ø��ߵ��г�����ȡ��������ʾ�������ޡ�");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_SLIDPRICESPREAD),"�����۲������˵�ѡ���������������ѡ����һ�£�����������Ż��������˴������ý����ǵ��������е���Ӧ��λ���á�");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_COMBO_SLIDEPRICE),"�����۲������˵�ѡ���������������ѡ����һ�£�����������Ż��������˴������ý����ǵ��������е���Ӧ��λ���á�");

}

void COptimizeSetupDlg::InitCtrl(void)
{
    m_editPriceFrom.SetWindowText("");
    m_editPriceTo.SetWindowText("");
    m_editMultiple.SetWindowText("");
    m_editMarket.SetWindowText("0");
    m_editSlidePrice1.SetWindowText("");
    m_editSlidePrice2.SetWindowText("");
    m_editSlidePrice3.SetWindowText("");
    m_editSlidePrice4.SetWindowText("");
    m_editSlidePrice5.SetWindowText("");
    m_editReCount1.SetWindowText("");
    m_editReCount2.SetWindowText("");
    m_editReCount3.SetWindowText("");
    m_editReCount4.SetWindowText("");
    m_editReCount5.SetWindowText("");
    m_comboPrice.SetCurSel(0);
    m_comboTime.SetCurSel(0);
//     m_comboSlidePrice.SetCurSel(1);
//     m_comboReCount.SetCurSel(1);
}

void COptimizeSetupDlg::EnableWindows(BOOL bEnable)
{
    m_comboTime.EnableWindow(bEnable);
    m_comboPrice.EnableWindow(bEnable);
//     m_comboSlidePrice.EnableWindow(bEnable);
//     m_comboReCount.EnableWindow(bEnable);
    m_editPriceFrom.EnableWindow(bEnable);
    m_editPriceTo.EnableWindow(bEnable);
    m_editMultiple.EnableWindow(bEnable);
    m_editMarket.EnableWindow(bEnable);
    m_sliderMarket.EnableWindow(bEnable);
    m_spinMarket.EnableWindow(bEnable);
    m_editSlidePrice1.EnableWindow(bEnable);
    m_editSlidePrice2.EnableWindow(bEnable);
    m_editSlidePrice3.EnableWindow(bEnable);
    m_editSlidePrice4.EnableWindow(bEnable);
    m_editSlidePrice5.EnableWindow(bEnable);
    m_editReCount1.EnableWindow(bEnable);
    m_editReCount2.EnableWindow(bEnable);
    m_editReCount3.EnableWindow(bEnable);
    m_editReCount4.EnableWindow(bEnable);
    m_editReCount5.EnableWindow(bEnable);
}

void COptimizeSetupDlg::OnBnClickedButtonNameadd()
{
    // TODO: Add your control notification handler code here
    m_listDetil.DeleteAllItems();
    EnableWindows(TRUE);
    InitCtrl();
    m_btnSaveSetup.EnableWindow(FALSE);
    m_btnDetilDel.EnableWindow(FALSE);
    UpdataReluRect();

    int nCount = m_listName.GetItemCount();
    m_nOptNameSelect = nCount;

    CString strTemp;
    strTemp.Format("%s%d","�Ż�",nCount+1);

    m_listName.InsertItem(nCount,strTemp);
    m_listName.SetItemText(nCount,0,strTemp);
    m_listName.SetItemState(nCount, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

    CAlgoAutoData *pAlgoData = ((CAlgoAutoOrderDlg*)((CGeneralParamDlg*)m_pGeneralParamDlg)->m_pAlgoAutoOrderDlg)->m_pAlgoAutoData;
    if(pAlgoData == NULL) return;

    OPTPARAM* pOptParam = new OPTPARAM;
    if(NULL == pOptParam) return;

    pOptParam->Index = nCount;
    pOptParam->Name = strTemp;    
    pAlgoData->vecOptInfo.push_back(pOptParam); 
}

void COptimizeSetupDlg::OnBnClickedButtonNamedelete()
{
    // TODO: Add your control notification handler code here
    CAlgoAutoData *pAlgoData = ((CAlgoAutoOrderDlg*)((CGeneralParamDlg*)m_pGeneralParamDlg)->m_pAlgoAutoOrderDlg)->m_pAlgoAutoData;
    if(pAlgoData == NULL) return;  
    //ɾ���б���
    if(-1 == m_nOptNameSelect) return;
    m_listName.DeleteItem(m_nOptNameSelect); 
    //ɾ������
    if(0 == pAlgoData->vecOptInfo.size()) return;
    vector<OPTPARAM*>::iterator iter = find(pAlgoData->vecOptInfo.begin(), 
                                            pAlgoData->vecOptInfo.end(), 
                                            pAlgoData->vecOptInfo[m_nOptNameSelect]);
    if(iter != pAlgoData->vecOptInfo.end())
    {    
        delete pAlgoData->vecOptInfo[m_nOptNameSelect];
        pAlgoData->vecOptInfo[m_nOptNameSelect] = NULL;
        iter = pAlgoData->vecOptInfo.erase(iter);
    }

    m_nOptNameSelect=-1;
    /*
    m_nOptNameSelect--;
    if(m_listName.GetItemCount() == 0)
        m_nOptNameSelect = -1;
    else
    {
        m_listName.SetItemState(m_nOptNameSelect, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
        UpdateDetilListCtrl(m_nOptNameSelect);
    }
    */
    m_listDetil.DeleteAllItems(); 
}

void COptimizeSetupDlg::OnBnClickedButtonDetiladd()
{
    // TODO: Add your control notification handler code here  
    if(-1 == m_nOptNameSelect)
    {
        ::MessageBox(NULL, "����ѡ���������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return;
    }

    if(!CheckParam() || !CheckPriceSet())    
        return; 

    //UI
    CString strIndex,strRange,strParam,strHistory;

    int nCount = m_listDetil.GetItemCount();
    m_nOptDetilSelect = nCount;

    int nNum = 5;
    if(0 ==(int)CTools_AlgoTrading::GetDoubleFromCEdit(m_editSlidePrice1))
        nNum--;
    if(0 ==(int)CTools_AlgoTrading::GetDoubleFromCEdit(m_editSlidePrice2))
        nNum--;
    if(0 ==(int)CTools_AlgoTrading::GetDoubleFromCEdit(m_editSlidePrice3))
        nNum--;
    if(0 ==(int)CTools_AlgoTrading::GetDoubleFromCEdit(m_editSlidePrice4))
        nNum--;
    if(0 ==(int)CTools_AlgoTrading::GetDoubleFromCEdit(m_editSlidePrice5))
        nNum--;
  

    strRange.Format("%.1f��%.1f",CTools_AlgoTrading::GetDoubleFromCEdit(m_editPriceFrom),
        CTools_AlgoTrading::GetDoubleFromCEdit(m_editPriceTo));

    strParam.Format("����%.1f�����г������%d��%d��",CTools_AlgoTrading::GetDoubleFromCEdit(m_editMultiple),
        (int)CTools_AlgoTrading::GetDoubleFromCEdit(m_editMarket),nNum);

    strHistory.Format("%s","--");

    m_listDetil.InsertItem(m_nOptDetilSelect,"");
    m_listDetil.SetItemText(m_nOptDetilSelect,0,strRange);
    m_listDetil.SetItemText(m_nOptDetilSelect,1,strParam);
    m_listDetil.SetItemText(m_nOptDetilSelect,2,strHistory);

   
    //Data
    CAlgoAutoData *pAlgoData = ((CAlgoAutoOrderDlg*)((CGeneralParamDlg*)m_pGeneralParamDlg)->m_pAlgoAutoOrderDlg)->m_pAlgoAutoData;
    if(pAlgoData == NULL) return;    

    section sect;
    sect.low = CTools_AlgoTrading::GetDoubleFromCEdit(m_editPriceFrom);
    sect.high = CTools_AlgoTrading::GetDoubleFromCEdit(m_editPriceTo);
    sect.index = m_nOptDetilSelect;
    pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSection.push_back(sect);

    SINGLEOPT* pSingle = new SINGLEOPT;
    InitSingleOPT(pSingle);
    pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle.push_back(pSingle); 

    UpdataReluRect();
    m_listDetil.SetItemState(m_nOptDetilSelect, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
    m_btnSaveSetup.EnableWindow(TRUE);
    m_btnDetilDel.EnableWindow(TRUE);

}

void COptimizeSetupDlg::OnBnClickedButtonDetildelete()
{
    // TODO: Add your control notification handler code here

    if(-1 == m_nOptDetilSelect) return;
	m_listDetil.DeleteItem(m_nOptDetilSelect);

    //ɾ������
    CAlgoAutoData *pAlgoData = ((CAlgoAutoOrderDlg*)((CGeneralParamDlg*)m_pGeneralParamDlg)->m_pAlgoAutoOrderDlg)->m_pAlgoAutoData;
    if(pAlgoData == NULL) return; 

    if(0 == pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle.size()) return;
    vector<SINGLEOPT*>::iterator iter = find(pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle.begin(), 
                                                pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle.end(), 
                                                pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle[m_nOptDetilSelect]);

    if(iter != pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle.end())
    {    
        delete pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle[m_nOptDetilSelect];
        pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle[m_nOptDetilSelect] = NULL;
        iter = pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle.erase(iter);
    }


    if(0 == pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSection.size()) return;
    vector<section>::iterator iter1; 

    for(iter1 = pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSection.begin(); 
        iter1 != pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSection.end();
        iter1++ )
    {
        if(iter1->index == m_nOptDetilSelect)
        {    
            pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSection.erase(iter1);
            break;
        }
    }

    int nIndex = 0;
    for(iter1 = pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSection.begin(); 
        iter1 != pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSection.end();
        iter1++ ,nIndex++)
    {
        iter1->index = nIndex;
    }

    UpdataReluRect();
    InitCtrl();

    m_nOptDetilSelect = -1;

    m_btnSaveSetup.EnableWindow(FALSE);
    m_btnDetilDel.EnableWindow(FALSE);
}

void COptimizeSetupDlg::OnNMClickListName(NMHDR *pNMHDR, LRESULT *pResult)
{
    //LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
    // TODO: Add your control notification handler code here
    DWORD dwPos = GetMessagePos();  
    POINTS ps= MAKEPOINTS(dwPos);  
    CPoint point(ps.x, ps.y);  
    m_listName.ScreenToClient(&point);  
    //ѡ����  
    UINT uFlag;  
    int item = m_listName.HitTest(point, &uFlag);  
    if (-1 == item)//ûѡ��
    {
        CAlgoAutoData *pAlgoData = ((CAlgoAutoOrderDlg*)((CGeneralParamDlg*)m_pGeneralParamDlg)->m_pAlgoAutoOrderDlg)->m_pAlgoAutoData;
        if(pAlgoData == NULL) return;

        m_nOptNameSelect = item;
        EnableWindows(FALSE);
        m_listDetil.DeleteAllItems();
        m_btnSaveSetup.EnableWindow(FALSE);
        m_btnDetilDel.EnableWindow(FALSE);
        InitCtrl();
        UpdataReluRect();        
    }else if(m_nOptNameSelect != item)  
    {  
        EnableWindows(TRUE);
        m_listDetil.DeleteAllItems();
        m_nOptNameSelect = item;

        UpdateDetilListCtrl(m_nOptNameSelect);
        UpdataReluRect();
    }  

    *pResult = 0;
}
void COptimizeSetupDlg::OnNMClickListDetil(NMHDR *pNMHDR, LRESULT *pResult)
{
    //LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
    // TODO: Add your control notification handler code here
    DWORD dwPos = GetMessagePos();  
    POINTS ps= MAKEPOINTS(dwPos);  
    CPoint point(ps.x, ps.y);  
    m_listDetil.ScreenToClient(&point);  
    //ѡ����  
    UINT uFlag;  
    int item = m_listDetil.HitTest(point, &uFlag);  
    if (-1 == item)//ûѡ��
    {
        InitCtrl();        
        m_nOptDetilSelect = item;
        m_btnSaveSetup.EnableWindow(FALSE);
        m_btnDetilDel.EnableWindow(FALSE);
    }else if(m_nOptDetilSelect != item)  
    {  
        m_nOptDetilSelect = item;
        SetOptDataToWnd(FALSE);
        m_btnSaveSetup.EnableWindow(TRUE);
        m_btnDetilDel.EnableWindow(TRUE);
    } 

    *pResult = 0;
}

BOOL COptimizeSetupDlg::CheckParam(void)
{
    if(0 == m_listName.GetSelectedCount())
    {
        ::MessageBox(NULL, "����ѡ���������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    CString strTemp,Temp1,Temp2,Temp3,Temp4,Temp5;

    m_editPriceFrom.GetWindowText(Temp1);
    m_editPriceTo.GetWindowText(Temp2);
    if (Temp1.IsEmpty() || Temp2.IsEmpty())
    {
        ::MessageBox(NULL, "�۸�䶯���ò���Ϊ�գ�������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    if(CTools_AlgoTrading::GetDoubleFromCEdit(m_editPriceFrom) >= CTools_AlgoTrading::GetDoubleFromCEdit(m_editPriceTo))
    {
        ::MessageBox(NULL, "�۸�䶯������������С���Ҳ࣬����������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    CAlgoAutoData *pAlgoData = ((CAlgoAutoOrderDlg*)((CGeneralParamDlg*)m_pGeneralParamDlg)->m_pAlgoAutoOrderDlg)->m_pAlgoAutoData;
    if(pAlgoData == NULL) 
        return FALSE;

    m_editMultiple.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
    {
        ::MessageBox(NULL, "���ٱ�������Ϊ�գ�������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    if (CTools_AlgoTrading::GetDoubleFromCEdit(m_editMultiple) < 0)
    {
        ::MessageBox(NULL, "��������Ϊ����������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    m_editMarket.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
    {
        ::MessageBox(NULL, "�г�����Ȳ���Ϊ�գ�������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    CGeneralParamDlg *pGeneralParmDlg = (CGeneralParamDlg*)m_pGeneralParamDlg;
    if(!pGeneralParmDlg) 
        return FALSE; 

//     if (pGeneralParmDlg->m_comboSlidPriceSpread.GetCurSel() != m_comboSlidePrice.GetCurSel())
//     {
//         ::MessageBox(NULL, "�Ż������������ڵĻ����۲����ò�ͬ������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
//         return FALSE;
//     }

//     if(1 == m_comboSlidePrice.GetCurSel())//����
//     {
        m_editSlidePrice1.GetWindowText(Temp1);
        m_editSlidePrice2.GetWindowText(Temp2);
        m_editSlidePrice3.GetWindowText(Temp3);
        m_editSlidePrice4.GetWindowText(Temp4);
        m_editSlidePrice5.GetWindowText(Temp5);


        if (CTools_AlgoTrading::IsHaveDot(Temp1)||
            CTools_AlgoTrading::IsHaveDot(Temp2)||
            CTools_AlgoTrading::IsHaveDot(Temp3)||
            CTools_AlgoTrading::IsHaveDot(Temp4)||
            CTools_AlgoTrading::IsHaveDot(Temp5))
        {
            ::MessageBox(NULL, "��Ӧ�µ�����λ�ڵ�λ�²�Ӧ�ó���С��������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }

//     }
    return TRUE;
}
void COptimizeSetupDlg::UpdateDetilListCtrl(int optNameIndex)
{
    if(-1 == optNameIndex) 
        return;

    CAlgoAutoData *pAlgoData = ((CAlgoAutoOrderDlg*)((CGeneralParamDlg*)m_pGeneralParamDlg)->m_pAlgoAutoOrderDlg)->m_pAlgoAutoData;
    if(pAlgoData == NULL) 
        return;
    if (pAlgoData->vecOptInfo.size() == 0)
        return;
    
    OPTPARAM *pOptParam = pAlgoData->vecOptInfo.at(optNameIndex);
    if(pOptParam == NULL)
        return;

    if (pOptParam->vecSingle.size() == 0)
    {
        InitCtrl();
        return;
    }

    vector<SINGLEOPT*>::iterator iterSingle = pOptParam->vecSingle.begin();    
    for (;iterSingle!=pOptParam->vecSingle.end();iterSingle++)
    {       
        m_listDetil.InsertItem((*iterSingle)->Index,"");
        m_listDetil.SetItemText((*iterSingle)->Index,0,(*iterSingle)->Range.c_str());
        m_listDetil.SetItemText((*iterSingle)->Index,1,(*iterSingle)->Option.c_str());
        m_listDetil.SetItemText((*iterSingle)->Index,2,(*iterSingle)->History.c_str());
    }

    m_listName.SetItemState(optNameIndex, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
    m_listDetil.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

    m_nOptDetilSelect = 0;
    SetOptDataToWnd(FALSE);
    
    m_btnSaveSetup.EnableWindow(TRUE);
    m_btnDetilDel.EnableWindow(TRUE);
}

void COptimizeSetupDlg::OnBnClickedButtonSavesetup()
{
    // TODO: Add your control notification handler code here
    CAlgoAutoData *pAlgoData = ((CAlgoAutoOrderDlg*)((CGeneralParamDlg*)m_pGeneralParamDlg)->m_pAlgoAutoOrderDlg)->m_pAlgoAutoData;
    if(pAlgoData == NULL) return;  

    InitSingleOPT(pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle[m_nOptDetilSelect]); 

    //���¼۸�����
    vector<section>::iterator iter = pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSection.begin();
    for (;iter != pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSection.end();iter++)
    {   
        if (iter->index == m_nOptDetilSelect)
        {   
            iter->low = atof(pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle[m_nOptDetilSelect]->PriceChange.valueFrom.c_str());
            iter->high = atof(pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle[m_nOptDetilSelect]->PriceChange.valueTo.c_str());
            break;
        }
    }

    m_listDetil.DeleteItem(m_nOptDetilSelect);

    m_listDetil.InsertItem(m_nOptDetilSelect,"");
    m_listDetil.SetItemText(m_nOptDetilSelect,0,pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle[m_nOptDetilSelect]->Range.c_str());
    m_listDetil.SetItemText(m_nOptDetilSelect,1,pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle[m_nOptDetilSelect]->Option.c_str());
    m_listDetil.SetItemText(m_nOptDetilSelect,2,pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle[m_nOptDetilSelect]->History.c_str());

    m_listDetil.SetItemState(m_nOptDetilSelect, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
    m_btnSaveSetup.EnableWindow(TRUE);
    m_btnDetilDel.EnableWindow(TRUE);

    UpdataReluRect();
}

void COptimizeSetupDlg::InitSingleOPT(SINGLEOPT* pSingle)
{
    if(NULL == pSingle)
        return;

    pSingle->Index = m_nOptDetilSelect;
    switch(m_comboTime.GetCurSel())
    {
    case 0:
        pSingle->BackTime = 15;
        break;
    case 1:
        pSingle->BackTime = 30;
        break;
    case 2:
        pSingle->BackTime = 1 * 60;
        break;
    case 3:
        pSingle->BackTime = 5 * 60;
        break;
    case 4:
        pSingle->BackTime = 15 * 60;
        break;
    case 5:
        pSingle->BackTime = 30 * 60;
        break;
    case 6:
        pSingle->BackTime = 1 * 60 * 60;
        break;
    }

    pSingle->PriceChange.type = m_comboPrice.GetCurSel();
    pSingle->PriceChange.valueFrom = CTools_AlgoTrading::GetStringFromCEdit(m_editPriceFrom);
    pSingle->PriceChange.valueTo = CTools_AlgoTrading::GetStringFromCEdit(m_editPriceTo);
    pSingle->Multiple = CTools_AlgoTrading::GetStringFromCEdit(m_editMultiple);
    pSingle->OptMarketDegreed = (int)CTools_AlgoTrading::GetDoubleFromCEdit(m_editMarket);

    CString str0,str1,str2,str3,str4;
    m_editSlidePrice1.GetWindowText(str0);
    m_editSlidePrice2.GetWindowText(str1);
    m_editSlidePrice3.GetWindowText(str2);
    m_editSlidePrice4.GetWindowText(str3);
    m_editSlidePrice5.GetWindowText(str4);

    pSingle->OptSlidePriceSpread.data.clear();
    pSingle->OptSlidePriceSpread.index = 1;//m_comboSlidePrice.GetCurSel();
    pSingle->OptSlidePriceSpread.data.push_back(str0.GetBuffer(0));
    pSingle->OptSlidePriceSpread.data.push_back(str1.GetBuffer(0));
    pSingle->OptSlidePriceSpread.data.push_back(str2.GetBuffer(0));
    pSingle->OptSlidePriceSpread.data.push_back(str3.GetBuffer(0));
    pSingle->OptSlidePriceSpread.data.push_back(str4.GetBuffer(0));

    str0.ReleaseBuffer();
    str1.ReleaseBuffer();
    str2.ReleaseBuffer();
    str3.ReleaseBuffer();
    str4.ReleaseBuffer();


    CString strIndex,strRange,strParam,strHistory;
    int nNum = 5;
    if(0 ==(int)CTools_AlgoTrading::GetDoubleFromCEdit(m_editSlidePrice1))
        nNum--;
    if(0 ==(int)CTools_AlgoTrading::GetDoubleFromCEdit(m_editSlidePrice2))
        nNum--;
    if(0 ==(int)CTools_AlgoTrading::GetDoubleFromCEdit(m_editSlidePrice3))
        nNum--;
    if(0 ==(int)CTools_AlgoTrading::GetDoubleFromCEdit(m_editSlidePrice4))
        nNum--;
    if(0 ==(int)CTools_AlgoTrading::GetDoubleFromCEdit(m_editSlidePrice5))
        nNum--;

    strRange.Format("%s��%s",pSingle->PriceChange.valueFrom.c_str() ,pSingle->PriceChange.valueTo.c_str());
    strParam.Format("����%s�����г������%d��%d��",pSingle->Multiple.c_str(),pSingle->OptMarketDegreed,nNum);
    strHistory.Format("%s","--");

    pSingle->Range = strRange;
    pSingle->Option = strParam;
    pSingle->History = strHistory;
  
}

void COptimizeSetupDlg::OnDeltaposSpinMarket(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    int nVolume = 0;
    char strQty[64];
    ZeroMemory(strQty, sizeof(strQty));
    m_editMarket.GetLine(0, strQty, 63);
    nVolume = atoi(strQty);

    nVolume -= pNMUpDown->iDelta;
    if(nVolume<=0)
        nVolume = 0;
    if(nVolume>100)
        nVolume = 100;
    sprintf_s(strQty, "%d", nVolume);
    m_editMarket.SetSel(0, -1);
    m_editMarket.ReplaceSel(strQty);

    m_sliderMarket.SetPos(nVolume);
    *pResult = 0;
}

void COptimizeSetupDlg::OnEnChangeEditMarket()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
    int nVolume = 0;
    char strQty[64];
    ZeroMemory(strQty, sizeof(strQty));
    m_editMarket.GetLine(0, strQty, 63);
    nVolume = atoi(strQty);

    m_sliderMarket.SetPos(nVolume);
}

void COptimizeSetupDlg::OnNMCustomdrawSliderMarket(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
}

void COptimizeSetupDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default
    int nCur = m_sliderMarket.GetPos();

    char strQty[64];
    ZeroMemory(strQty, sizeof(strQty));
    sprintf_s(strQty, "%d", nCur);
    m_editMarket.SetSel(0, -1);
    m_editMarket.ReplaceSel(strQty);

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void COptimizeSetupDlg::SetOptDataToWnd(BOOL bShow/* = TRUE*/)
{
    CAlgoAutoData *pAlgoData = ((CAlgoAutoOrderDlg*)((CGeneralParamDlg*)m_pGeneralParamDlg)->m_pAlgoAutoOrderDlg)->m_pAlgoAutoData;
    if(pAlgoData == NULL) return;
    
    if(-1 == m_nOptNameSelect || pAlgoData->vecOptInfo.size() == 0)
    {
        EnableWindows(FALSE);
        return;
    }
   
    OPTPARAM *pOptParam = pAlgoData->vecOptInfo.at(m_nOptNameSelect); 
    if(bShow)
    {
        m_listName.DeleteAllItems();
        m_listDetil.DeleteAllItems();

        vector<OPTPARAM*>::iterator iter1;
        

        if(pAlgoData->vecOptInfo.size() == 0) return;

        CString strTemp;
        iter1 = pAlgoData->vecOptInfo.begin();
        for (;iter1!=pAlgoData->vecOptInfo.end();iter1++)
        {
            m_listName.InsertItem((*iter1)->Index,(*iter1)->Name.c_str());
            m_listName.SetItemText((*iter1)->Index,0,(*iter1)->Name.c_str());
        }

        int nIndex = 0;
        vector<SINGLEOPT*>::iterator iter2 = pOptParam->vecSingle.begin();
        for (;iter2!=pOptParam->vecSingle.end();iter2++,nIndex++)
        {
            m_listDetil.InsertItem(nIndex,"");
            m_listDetil.SetItemText(nIndex,0,(*iter2)->Range.c_str());
            m_listDetil.SetItemText(nIndex,1,(*iter2)->Option.c_str());
            m_listDetil.SetItemText(nIndex,2,(*iter2)->History.c_str());
        }

        m_listName.SetItemState(m_nOptNameSelect, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
        m_listDetil.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
        
        m_nOptDetilSelect = 0;
        m_btnSaveSetup.EnableWindow(TRUE);
        m_btnDetilDel.EnableWindow(TRUE);
    }

    if(pOptParam == NULL)
        return;

    if (pOptParam->vecSingle.size() == 0)
        return;

    SINGLEOPT* pSINGLEOPT = NULL;
    if(bShow)
        pSINGLEOPT = pOptParam->vecSingle.at(0); 
    else
        pSINGLEOPT = pOptParam->vecSingle.at(m_nOptDetilSelect);
    
     switch(pSINGLEOPT->BackTime)
    {
    case 15:
        m_comboTime.SetCurSel(0);
        break;
    case 30:
        m_comboTime.SetCurSel(1);
        break;
    case 1*60:
        m_comboTime.SetCurSel(2);
        break;
    case 5*60:
        m_comboTime.SetCurSel(3);
        break;
    case 15*60:
        m_comboTime.SetCurSel(4);
        break;
    case 30*60:
        m_comboTime.SetCurSel(5);
        break;
    case 1*60*60:
        m_comboTime.SetCurSel(6);
        break;
    default:
        break;
    }

    m_comboPrice.SetCurSel(pSINGLEOPT->PriceChange.type);
    m_editPriceFrom.SetWindowText(pSINGLEOPT->PriceChange.valueFrom.c_str());
    m_editPriceTo.SetWindowText(pSINGLEOPT->PriceChange.valueTo.c_str());
    m_editMultiple.SetWindowText(pSINGLEOPT->Multiple.c_str());
    CTools_AlgoTrading::SetIntToCEdit(m_editMarket,pSINGLEOPT->OptMarketDegreed);
    
    vector<string> vecPriceSpread = pSINGLEOPT->OptSlidePriceSpread.data;

//     m_comboSlidePrice.SetCurSel(pSINGLEOPT->OptSlidePriceSpread.index);
    for (int i=0;i<(int)vecPriceSpread.size();i++)
    {
        if(i == 0)
            m_editSlidePrice1.SetWindowText(vecPriceSpread[i].c_str());
        else if(i == 1)
            m_editSlidePrice2.SetWindowText(vecPriceSpread[i].c_str());
        else if(i == 2)
            m_editSlidePrice3.SetWindowText(vecPriceSpread[i].c_str());
        else if(i == 3)
            m_editSlidePrice4.SetWindowText(vecPriceSpread[i].c_str());
        else if(i == 4)
            m_editSlidePrice5.SetWindowText(vecPriceSpread[i].c_str());
    }
}

void COptimizeSetupDlg::SetSelectOptIndex(int optIndex)
{
    m_nOptNameSelect = optIndex;    
}

void COptimizeSetupDlg::SetParentWnd(CWnd* pParentWnd)
{
    m_pGeneralParamDlg = pParentWnd;
}
void COptimizeSetupDlg::OnPaint()
{   
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CDialog::OnPaint() for painting messages
    if(m_pGeneralParamDlg == NULL)
        return;

    CAlgoAutoData *pAlgoData = ((CAlgoAutoOrderDlg*)((CGeneralParamDlg*)m_pGeneralParamDlg)->m_pAlgoAutoOrderDlg)->m_pAlgoAutoData;
    if(pAlgoData == NULL) 
        return;

    if (m_staticRule.GetSafeHwnd() != NULL)
    {

        RECT rect;
        m_staticRule.GetWindowRect(&rect);
        ScreenToClient(&rect);        
  
        int wid = rect.right - rect.left;
        int hei = rect.bottom - rect.top;

        CDC memdc;
        memdc.CreateCompatibleDC(&dc);
        CBitmap MemBmp;
        MemBmp.CreateCompatibleBitmap(&dc,wid,hei);
        CBitmap *pOldBmp=memdc.SelectObject(&MemBmp);
        memdc.FillSolidRect(0,0,wid,hei,BkColor_Rule);

        if(-1 == m_nOptNameSelect)
        {      
            dc.BitBlt(rect.left,rect.top,wid,hei,&memdc,0,0,SRCCOPY);
            memdc.SelectObject(pOldBmp);
            MemBmp.DeleteObject(); 
            memdc.DeleteDC();   
            return;
        }

        double minValueSave,minValue = 999999.0f,maxValueSave,maxValue = -99999.0f;
        vector<double> vecNumber;
        vector<SINGLEOPT*>::iterator iter = pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle.begin();
        for (;iter!=pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle.end();iter++)
        {      
             if(minValue > atof((*iter)->PriceChange.valueFrom.c_str()))
                minValue = atof((*iter)->PriceChange.valueFrom.c_str());            

            if(maxValue < atof((*iter)->PriceChange.valueTo.c_str()))
                maxValue = atof((*iter)->PriceChange.valueTo.c_str());

            vecNumber.push_back(atof((*iter)->PriceChange.valueFrom.c_str()));
            vecNumber.push_back(atof((*iter)->PriceChange.valueTo.c_str()));
        }

        if (minValue >= 999999.0f || maxValue <= -99999.0f)
        {
            dc.BitBlt(rect.left,rect.top,wid,hei,&memdc,0,0,SRCCOPY);
            memdc.SelectObject(pOldBmp);
            MemBmp.DeleteObject(); 
            memdc.DeleteDC();   
            return; 
        }

        CFont font;
        font.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,0,"������");
        CFont* poldFont=memdc.SelectObject(&font);
        memdc.SetTextColor(RGB(0,0,0));
        memdc.SetBkMode(TRANSPARENT);
        char buf[256];

        minValueSave = minValue;
        maxValueSave = maxValue;

        CheckSumPrice(minValue,1.0f,-1);
        CheckSumPrice(maxValue,1.0f,1);

        int N = (int)(maxValue - minValue);//����
        int L = wid/N;//ÿ�γ���
        int H = 2;//�̶ȵĳ���

        if(pAlgoData->vecOptInfo.size() == 0)
        {
            dc.BitBlt(rect.left,rect.top,wid,hei,&memdc,0,0,SRCCOPY);
            memdc.SelectObject(pOldBmp);
            MemBmp.DeleteObject();   
            memdc.DeleteDC();            
            return;
        }

        int xPos;//,PosSaveStart;
        int yPos;//,PosSaveEnd;

        for(int i=0;i<N+1;i++)//�̶�
        {
            xPos = i*L;
            yPos = hei/2 - H;

            memdc.FillSolidRect(xPos,yPos,2,2*H,BkColor_RuleDegreed);       
        }

        /*//���㷶Χ
        PosSaveStart = wid * (minValueSave - minValue) / (maxValue - minValue);
        PosSaveEnd = wid * (maxValueSave - minValue) / (maxValue - minValue);

        memdc.FillSolidRect(PosSaveStart,0,(maxValueSave-minValueSave)*L,hei,BkColor_Select);
        */
        vector<double>::iterator iterNum;
        for (int j=0;j<N+1;j++)//��ֵ
        {
            xPos = j*L-8;
            yPos = hei/2 + H;

            for (iterNum=vecNumber.begin();iterNum!=vecNumber.end();iterNum++)
            {
                if (fabs(*iterNum - (minValue + j*1.0f)) < 0.1)
                {
                    sprintf_s(buf,sizeof(buf)-1,"%.1f",minValue + j*1.0f);
                    memdc.TextOutA(xPos,yPos,buf,strlen(buf));
                }
            }


        }

        dc.BitBlt(rect.left,rect.top,wid,hei,&memdc,0,0,SRCCOPY);
        memdc.SelectObject(poldFont);
        memdc.SelectObject(pOldBmp);
        font.DeleteObject();
        MemBmp.DeleteObject(); 
        memdc.DeleteDC();               
    }

}

void COptimizeSetupDlg::UpdataReluRect(void)
{
    RECT rect;
    m_staticRule.GetWindowRect(&rect);
    ScreenToClient(&rect);   

    ::InvalidateRect(GetSafeHwnd(),&rect,false);
    ::SendMessage(GetSafeHwnd(),WM_PAINT,0,0);
}

void COptimizeSetupDlg::OnCbnSelchangeComboPrice()
{
    // TODO: Add your control notification handler code here
    CAlgoAutoData *pAlgoData = ((CAlgoAutoOrderDlg*)((CGeneralParamDlg*)m_pGeneralParamDlg)->m_pAlgoAutoOrderDlg)->m_pAlgoAutoData;
    if(pAlgoData == NULL) return;

    int nCurSel = m_comboPrice.GetCurSel();
    if (pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSingle.size() == 0) 
    {
        m_comboPrcieChangePreCurSel = nCurSel;
        return;
    }
    
    if (nCurSel != m_comboPrcieChangePreCurSel)
    {
        ::MessageBox(NULL, "ͬһ���Ż��������ڵĶ����Ż��������۸�仯��Ҫô�Ǽ۸�Ҫô�ǰٷֱȣ�������ͳһ�ģ�����������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        m_comboPrice.SetCurSel(m_comboPrcieChangePreCurSel);
    }
}

BOOL COptimizeSetupDlg::InterSection(vector<section> &sec,const section &dst)
{
    std::vector<section>::iterator iter;
    for (iter=sec.begin();iter!=sec.end();iter++)
    {
        if (!((iter->high<=dst.low) || (iter->low>=dst.high)))
        {
            return TRUE;
            break;
        }
    }

    return FALSE;
}
BOOL COptimizeSetupDlg::CheckPriceSet(void)
{
    CAlgoAutoData *pAlgoData = ((CAlgoAutoOrderDlg*)((CGeneralParamDlg*)m_pGeneralParamDlg)->m_pAlgoAutoOrderDlg)->m_pAlgoAutoData;
    if(pAlgoData == NULL) 
        return FALSE;

    if(-1 == m_nOptNameSelect)
        return FALSE;

    CString sNumberFrom,sNumberTo;
    m_editPriceFrom.GetWindowText(sNumberFrom);
    m_editPriceTo.GetWindowText(sNumberTo);
    double dNumberF = atof(sNumberFrom);
    double dNumberT = atof(sNumberTo);

    int nCount = m_listDetil.GetItemCount();

    section tmpSect;
    tmpSect.low = dNumberF;
    tmpSect.high = dNumberT;
    tmpSect.index = nCount;

    if(InterSection(pAlgoData->vecOptInfo[m_nOptNameSelect]->vecSection,tmpSect))
    {
        ::MessageBox(NULL, "�۸��������ó�ͻ������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;

    }else
        return TRUE;
}

BOOL COptimizeSetupDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)  return TRUE;
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;

    if (NULL != m_toolTip.GetSafeHwnd())
    {
        m_toolTip.RelayEvent(pMsg);
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void COptimizeSetupDlg::OnNMDblclkListName(NMHDR *pNMHDR, LRESULT *pResult)
{
    //LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
    // TODO: Add your control notification handler code here
    NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
    CRect rc;
    if(pNMListView->iItem!=-1)
    {
        m_row=pNMListView->iItem;//m_rowΪ��ѡ���е�����ţ�int���ͳ�Ա������
        m_column=pNMListView->iSubItem;//m_columnΪ��ѡ���е�����ţ�int���ͳ�Ա������
        m_listName.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem,LVIR_LABEL,rc);//ȡ������ľ���
        rc.left+=12;
        rc.top+=10;
        rc.right+=10;
        rc.bottom+=12;

        char * ch=new char [128];
        m_listName.GetItemText(pNMListView->iItem, pNMListView->iSubItem,ch,128);//ȡ�����������
        m_editListName.SetWindowText(ch);//�������������ʾ���༭����
        m_editListName.ShowWindow(SW_SHOW);//��ʾ�༭��
        m_editListName.MoveWindow(&rc);//���༭���ƶ����������棬������������
        m_editListName.SetFocus();//ʹ�༭��ȡ�ý���
        m_editListName.CreateSolidCaret(1,rc.Height()-5);//����һ�����
        m_editListName.ShowCaret();//��ʾ���
        m_editListName.SetSel(-1);//ʹ����Ƶ������
    }
    *pResult = 0;
}

void COptimizeSetupDlg::OnEnKillfocusEditListname()
{
    // TODO: Add your control notification handler code here
    CString str;
    m_editListName.GetWindowText(str);//ȡ�ñ༭�������
    m_listName.SetItemText(m_row,m_column,str);//�������ݸ��µ�CListCtrl��
    m_editListName.ShowWindow(SW_HIDE);//���ر༭��

    //��������
    CAlgoAutoData *pAlgoData = ((CAlgoAutoOrderDlg*)((CGeneralParamDlg*)m_pGeneralParamDlg)->m_pAlgoAutoOrderDlg)->m_pAlgoAutoData;
    if(pAlgoData == NULL) 
        return;

    vector<OPTPARAM*>::iterator iterOpt = pAlgoData->vecOptInfo.begin();
    for (;iterOpt != pAlgoData->vecOptInfo.end();iterOpt++)
    {
        if ((*iterOpt)->Index == m_row)
        {
            (*iterOpt)->Name = str;        
            return;
        }
    }

}

void COptimizeSetupDlg::OnBnClickedButtonClose()
{
    // TODO: Add your control notification handler code here
    CAlgoAutoData *pAlgoData = ((CAlgoAutoOrderDlg*)((CGeneralParamDlg*)m_pGeneralParamDlg)->m_pAlgoAutoOrderDlg)->m_pAlgoAutoData;
    if(pAlgoData == NULL) return;

    CGeneralParamDlg *pGeneralParmDlg = (CGeneralParamDlg*)m_pGeneralParamDlg;
    if(!pGeneralParmDlg) return; 

    vector<OPTPARAM*>::iterator iterOpt = pAlgoData->vecOptInfo.begin();
    for (;iterOpt != pAlgoData->vecOptInfo.end();iterOpt++)
    {
        if ((*iterOpt)->vecSingle.size() == 0)
        {
            ::MessageBox(NULL, "�Ż�û�����ò����������������˳�", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return;
        }
    }

    BOOL bHaveItem;
    if(m_listName.GetItemCount() > 0)
        bHaveItem = TRUE;
    else
        bHaveItem = FALSE;

    pGeneralParmDlg->UpdataOptCombo(m_nOptNameSelect,bHaveItem);

    CDialog::OnOK();
}
