// GeneralParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AlgoAutoOrder.h"
#include "GeneralParamDlg.h"
#include "Tools_AlgoTrading.h"
#include "Module-Misc2/tools_util.h"
#include "AlgoAutoOrderDlg.h"
#include "MessageBoxDialog.h"

// CGeneralParamDlg dialog

IMPLEMENT_DYNAMIC(CGeneralParamDlg, CDialog)

CGeneralParamDlg::CGeneralParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog/*CBookOrderOperations*/(CGeneralParamDlg::IDD, pParent)
    , m_nCurVolume(0)
    , m_bOkClicked(FALSE)
    ,m_bCloseupComboBasicprice(TRUE)
{
}

CGeneralParamDlg::~CGeneralParamDlg()
{
}

void CGeneralParamDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_INSTRUMENTID, m_editInstrumentID);
    DDX_Control(pDX, IDC_CHECK_ACTIVEOPTIMIZE, m_checkActiveOptimize);
    DDX_Control(pDX, IDC_COMBO_BUYSELL, m_comboBuysell);
    DDX_Control(pDX, IDC_COMBO_OPENCLOSE, m_comboOpenclose);
    DDX_Control(pDX, IDC_EDIT_INSTRUMENTCOUNT, m_editInstrumentCount);
    DDX_Control(pDX, IDC_EDIT_PRICELIMIT, m_editPriceLimit);
    DDX_Control(pDX, IDC_SPIN_PRICELIMIT, m_spinPriceLimit);
    DDX_Control(pDX, IDC_DATETIMEPICKER_START, m_editInstrumentStartTime);
    DDX_Control(pDX, IDC_DATETIMEPICKER_END, m_editInstrumentEndTime);
    DDX_Control(pDX, IDC_SLID_MARKETDEGREED, m_slidMarketDegreed);
    DDX_Control(pDX, IDC_EDIT_MARKETDEGREED, m_editMarketDegreed);
    DDX_Control(pDX, IDC_SPIN_MARKETDEGREED, m_spinMarketDegreed);
    DDX_Control(pDX, IDC_EDIT_ORDERCOUNT, m_editOrderCount);
    DDX_Control(pDX, IDC_COMBO_ORDERCOUNT, m_comboOrderCount);
//     DDX_Control(pDX, IDC_EDIT_RESETORDERCOUNT, m_editResetOrderCount);
//     DDX_Control(pDX, IDC_COMBO_RESETORDERCOUNT1, m_comboResetOrderCount);
    DDX_Control(pDX, IDC_COMBO_BASICPRICE, m_comboBasicPrice);
    DDX_Control(pDX, IDC_EDIT_BASICPRICE, m_editBasicPrice);
//     DDX_Control(pDX, IDC_COMBO_SLIDPRICESPREAD, m_comboSlidPriceSpread);
//     DDX_Control(pDX, IDC_EDIT_SLIDPRICESPREAD1, m_editSlidPriceSpread1);
    DDX_Control(pDX, IDC_EDIT_SLIDPRICESPREAD2, m_editSlidPriceSpread2);
    DDX_Control(pDX, IDC_EDIT_SLIDPRICESPREAD3, m_editSlidPriceSpread3);
    DDX_Control(pDX, IDC_EDIT_SLIDPRICESPREAD4, m_editSlidPriceSpread4);
    DDX_Control(pDX, IDC_EDIT_SLIDPRICESPREAD5, m_editSlidPriceSpread5);
//     DDX_Control(pDX, IDC_COMBO_RESETORDERCOUNT2, m_comboRelateOrderCount);
//     DDX_Control(pDX, IDC_EDIT_RESETORDERCOUNT1, m_editRelateOrderCount1);
    DDX_Control(pDX, IDC_EDIT_RESETORDERCOUNT2, m_editRelateOrderCount2);
    DDX_Control(pDX, IDC_EDIT_RESETORDERCOUNT3, m_editRelateOrderCount3);
    DDX_Control(pDX, IDC_EDIT_RESETORDERCOUNT4, m_editRelateOrderCount4);
    DDX_Control(pDX, IDC_EDIT_RESETORDERCOUNT5, m_editRelateOrderCount5);
    DDX_Control(pDX, IDC_STATIC_SELLPRICE, m_staticSellPrice);
    DDX_Control(pDX, IDC_STATIC_LATESTPRICE, m_staticLatestPrice);
    DDX_Control(pDX, IDC_STATIC_BUYPRICE, m_staticBuyPrice);
    DDX_Control(pDX, IDC_STATIC_MIDDLEPRICE, m_staticMiddlePrice);
    DDX_Control(pDX, IDC_STATIC_YESTERDAYPRICE, m_staticYesterdayPrice);
    DDX_Control(pDX, IDC_COMBO_ACTIVEOPTIMIZE, m_comboActiveOptimize);
    DDX_Control(pDX, IDC_COMBO_IDLETIME, m_comboIdleTime);
    DDX_Control(pDX, IDC_COMBO_EXECUTETIME, m_comboExecuteTime);
    DDX_Control(pDX, IDC_COMBO_OVERRETRYTOTAL, m_comboOverTimeTotal);
    DDX_Control(pDX, IDC_EDIT_IDLETIME, m_editIdleTime);
    DDX_Control(pDX, IDC_EDIT_EXECUTETIME, m_editExecuteTime);
    DDX_Control(pDX, IDC_CHECK_OVERRETRYCOUNT, m_checkOverTimeRetryCount);
    DDX_Control(pDX, IDC_EDIT_OVERRETRYCOUNT, m_editOverTimeRetryCount);
    DDX_Control(pDX, IDC_CHECK_OVERRETRYTOTAL, m_checkOverTimeTotal);
    DDX_Control(pDX, IDC_EDIT_OVERRETRYTOTAL, m_editOverTimeTotal);

    DDX_Control(pDX, IDC_BUTTON_PLAY, m_btnPlay);
    DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStop);
    DDX_Control(pDX, IDC_BTN_PARAMSETUP, m_btnOptSetup);
    DDX_Control(pDX, IDC_STATIC_TITLE_SELLPRICE, m_staticTitleSellPrice);
    DDX_Control(pDX, IDC_STATIC_TITLE_LATESTPRICE, m_staticTitleLatestPrice);
    DDX_Control(pDX, IDC_STATIC_TITLE_BUYPRICE, m_staticTitleBuyPrice);
    DDX_Control(pDX, IDC_STATIC_TITLE_MIDDLEPRICE, m_staticTitleMiddlePrice);
    DDX_Control(pDX, IDC_STATIC_TITLE_YESTERDAYPRICE, m_staticTitleYesterdayPrice);
    DDX_Control(pDX, IDC_STATIC_MUST1, m_staticMust1);
    DDX_Control(pDX, IDC_STATIC_MUST2, m_staticMust2);
    DDX_Control(pDX, IDC_STATIC_MUST3, m_staticMust3);
    DDX_Control(pDX, IDC_STATIC_MUST4, m_staticMust4);
    DDX_Control(pDX, IDC_STATIC_SPLIT, m_staticSplit);
    DDX_Control(pDX, IDC_CHECK_MULTIPRICE, m_btnCheckMultiPrice);
    DDX_Control(pDX, IDC_COMBO_MULTIPRICE, m_comboMultiPrice);
}


BEGIN_MESSAGE_MAP(CGeneralParamDlg, CDialog)
    ON_BN_CLICKED(IDC_BTN_PARAMSETUP, &CGeneralParamDlg::OnBnClickedBtnParamsetup)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PRICELIMIT, &CGeneralParamDlg::OnDeltaposSpinPricelimit)
    ON_WM_HSCROLL()
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MARKETDEGREED, &CGeneralParamDlg::OnDeltaposSpinMarketdegreed)    
    ON_EN_CHANGE(IDC_EDIT_MARKETDEGREED, &CGeneralParamDlg::OnEnChangeEditMarketdegreed)
	ON_EN_CHANGE(IDC_EDIT_INSTRUMENTID, &CGeneralParamDlg::OnEnChangeEditInstrumentid)
    ON_BN_CLICKED(IDC_BUTTON_PLAY, &CGeneralParamDlg::OnBnClickedBtnOk)
    ON_BN_CLICKED(IDC_BUTTON_STOP, &CGeneralParamDlg::OnBnClickedBtnCancel)
	ON_WM_PAINT()
    ON_CBN_SELCHANGE(IDC_COMBO_ORDERCOUNT, &CGeneralParamDlg::OnCbnSelchangeComboOrdercount)
    ON_BN_CLICKED(IDC_CHECK_MULTIPRICE, &CGeneralParamDlg::OnBnClickedCheckMultiPrice)
    ON_CBN_CLOSEUP(IDC_COMBO_BASICPRICE, &CGeneralParamDlg::OnCbnCloseupComboBasicprice)
    ON_CBN_DROPDOWN(IDC_COMBO_BASICPRICE, &CGeneralParamDlg::OnCbnDropdownComboBasicprice)
    ON_CBN_SELCHANGE(IDC_COMBO_BASICPRICE, &CGeneralParamDlg::OnCbnSelchangeComboBasicprice)
END_MESSAGE_MAP()


// CGeneralParamDlg message handlers

void CGeneralParamDlg::OnBnClickedBtnParamsetup()
{
    // TODO: Add your control notification handler code here
    if(m_comboActiveOptimize.IsWindowEnabled())   
        m_OptimizeSetupDlg.SetSelectOptIndex(m_comboActiveOptimize.GetCurSel());
    
    m_OptimizeSetupDlg.SetParentWnd(this);
    m_OptimizeSetupDlg.DoModal();
}

BOOL CGeneralParamDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_staticMust1.Set_TextColor(RGB(255,0,0));;
    m_staticMust2.Set_TextColor(RGB(255,0,0));;
    m_staticMust3.Set_TextColor(RGB(255,0,0));;
    m_staticMust4.Set_TextColor(RGB(255,0,0));;

    m_staticTitleSellPrice.Set_TextColor(RGB(255,0,0));
    m_staticTitleLatestPrice.Set_TextColor(RGB(255,0,0));
    m_staticTitleBuyPrice.Set_TextColor(RGB(255,0,0)); 
    m_staticTitleMiddlePrice.Set_TextColor(RGB(255,0,0)); 
    m_staticTitleYesterdayPrice.Set_TextColor(RGB(255,0,0));

    m_staticSellPrice.Set_TextColor(RGB(255,0,0)); 
    m_staticLatestPrice.Set_TextColor(RGB(255,0,0));
    m_staticBuyPrice.Set_TextColor(RGB(255,0,0)); 
    m_staticMiddlePrice.Set_TextColor(RGB(255,0,0)); 
    m_staticYesterdayPrice.Set_TextColor(RGB(255,0,0)); 
    

    m_comboBuysell.InsertString(0, "����");
    m_comboBuysell.InsertString(1, "����");
    m_comboBuysell.SetCurSel(0);

    m_comboOpenclose.InsertString(0, "����");
    m_comboOpenclose.InsertString(1, "ƽ��");
    m_comboOpenclose.InsertString(2, "ƽ��");
    m_comboOpenclose.SetCurSel(0);

    m_comboOrderCount.InsertString(0, "�ٷֱ�");
    m_comboOrderCount.InsertString(1, "����");
    m_comboOrderCount.SetCurSel(1);

//     m_comboResetOrderCount.InsertString(0, "�ٷֱ�");
//     m_comboResetOrderCount.InsertString(1, "����");
//     m_comboResetOrderCount.SetCurSel(1);

//     m_comboRelateOrderCount.InsertString(0, "�ٷֱ�");
//     m_comboRelateOrderCount.InsertString(1, "����");
//     m_comboRelateOrderCount.SetCurSel(1);

    m_comboBasicPrice.InsertString(0, "���");
    m_comboBasicPrice.InsertString(1, "���¼�");
    m_comboBasicPrice.InsertString(2, "����");
    m_comboBasicPrice.InsertString(3, "�м��");
    m_comboBasicPrice.InsertString(4, "������");
    m_comboBasicPrice.InsertString(5, "�Զ���۸�");
    m_comboBasicPrice.SetCurSel(1);
    m_editBasicPrice.EnableWindow(FALSE);

//     m_comboSlidPriceSpread.InsertString(0, "�ٷֱ�");
//     m_comboSlidPriceSpread.InsertString(1, "��λ");
//     m_comboSlidPriceSpread.SetCurSel(1);

    m_comboMultiPrice.InsertString(0, "-5");
    m_comboMultiPrice.InsertString(1, "-4");
    m_comboMultiPrice.InsertString(2, "-3");
    m_comboMultiPrice.InsertString(3, "-2");
    m_comboMultiPrice.InsertString(4, "-1");
    m_comboMultiPrice.InsertString(5, "0");
    m_comboMultiPrice.InsertString(6, "1");
    m_comboMultiPrice.InsertString(7, "2");
    m_comboMultiPrice.InsertString(8, "3");
    m_comboMultiPrice.InsertString(9, "4");
    m_comboMultiPrice.InsertString(10, "5");
    m_comboMultiPrice.SetCurSel(5);

    m_comboIdleTime.InsertString(0, "����");
    m_comboIdleTime.InsertString(1, "����");
    m_comboIdleTime.InsertString(2, "ʱ��");
    m_comboIdleTime.SetCurSel(0);

    m_comboExecuteTime.InsertString(0, "����");
    m_comboExecuteTime.InsertString(1, "����");
    m_comboExecuteTime.InsertString(2, "ʱ��");
    m_comboExecuteTime.SetCurSel(0);

    m_comboOverTimeTotal.InsertString(0, "ʱ��");
    m_comboOverTimeTotal.InsertString(1, "����");
    m_comboOverTimeTotal.InsertString(2, "����");
    m_comboOverTimeTotal.SetCurSel(0);

    m_slidMarketDegreed.SetRange(0,100);
    m_slidMarketDegreed.SetTicFreq(10);
    m_slidMarketDegreed.SetPos(0);
    
    m_staticBuyPrice.SetWindowText("��");
    m_staticSellPrice.SetWindowText("��");
    m_staticMiddlePrice.SetWindowText("��");
    m_staticYesterdayPrice.SetWindowText("��");
    m_staticLatestPrice.SetWindowText("��");

    m_editMarketDegreed.SetWindowText("0");

	m_editInstrumentStartTime.SetFormat("HH:mm:ss tt");
	m_editInstrumentEndTime.SetFormat("HH:mm:ss tt");

    CTime t = CTime::GetCurrentTime(); //��ȡϵͳ����
    int h=t.GetHour();
	int mm=t.GetMinute(); 
	int s=t.GetSecond(); 

	int verH = 15-h;
    int verM = 30-mm;
	t = t + CTimeSpan(0,verH,verM,-s);
    m_editInstrumentEndTime.SetTime(&t);
     
    m_btnPlay.LoadBitmaps(IDB_BITMAP_PLAY,1,1,1,1,1); 
    m_btnStop.LoadBitmaps(IDB_BITMAP_STOP,1,1,1,1,1);
    m_btnOptSetup.LoadBitmaps(IDB_BITMAP_DEFAULT,1,1,1,1,1);

    AddToolTip();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CGeneralParamDlg::SetInstrumentID(CString& strInstrumentID)
{
    m_editInstrumentID.SetSel(0, -1);   
    m_editInstrumentID.ReplaceSel(strInstrumentID);
}

CString CGeneralParamDlg::GetInstrumentID()
{
    char strText[64];
    ZeroMemory(strText, sizeof(strText));
    m_editInstrumentID.GetLine(0, strText, 63);
    return strText;
}

void CGeneralParamDlg::UpdateDepthMarketContent(PlatformStru_DepthMarketData& depthMarketDate)
{
	CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
	if(pAlgo == NULL) return;

    PlatformStru_InstrumentInfo *pInfo = (PlatformStru_InstrumentInfo*)pAlgo->GetInstrumentInfo();
    if(pInfo != NULL)
        pAlgo->m_pAlgoAutoData->PriceTick = pInfo->PriceTick;
	
//     if( !util::isInvalidValue(depthMarketDate.BidPrice1)&&
//         !util::isInvalidValue(depthMarketDate.AskPrice1)&&
//         !util::isInvalidValue(depthMarketDate.LastPrice)&&
//         !util::isInvalidValue(depthMarketDate.PreSettlementPrice))
    {
		if (pAlgo->GetOldInstrumentID().CompareNoCase(depthMarketDate.InstrumentID) != 0)
			CTools_AlgoTrading::SetStringToCEdit(m_editInstrumentID,depthMarketDate.InstrumentID);
        if(!util::isInvalidValue(depthMarketDate.BidPrice1))
            CTools_AlgoTrading::SetStringToCStatic(m_staticBuyPrice,CTools_AlgoTrading::StringFromDouble(depthMarketDate.BidPrice1));
        if(!util::isInvalidValue(depthMarketDate.AskPrice1))
            CTools_AlgoTrading::SetStringToCStatic(m_staticSellPrice,CTools_AlgoTrading::StringFromDouble(depthMarketDate.AskPrice1));
        if(!util::isInvalidValue(depthMarketDate.LastPrice))
            CTools_AlgoTrading::SetStringToCStatic(m_staticLatestPrice,CTools_AlgoTrading::StringFromDouble(depthMarketDate.LastPrice));
    
        if (pAlgo->m_pAlgoAutoData->PriceTick != 0.0f)
        {
            double price = (depthMarketDate.BidPrice1+depthMarketDate.AskPrice1)/2;
            if ((int)(10000*price)%(int)(10000*(pAlgo->m_pAlgoAutoData->PriceTick)) != 0)
            {
                switch(m_comboBuysell.GetCurSel())
                {
                case 0://����        
                    CheckSumPrice(price,pAlgo->m_pAlgoAutoData->PriceTick,-1);             
                    break;
                case 1://����
                    CheckSumPrice(price,pAlgo->m_pAlgoAutoData->PriceTick,1);    
                    break;
                }

                if(m_comboBasicPrice.GetCurSel() == 3 && m_bCloseupComboBasicprice)
                    CTools_AlgoTrading::SetDoubleToCEdit(m_editBasicPrice,price);

                if(!util::isInvalidValue(price))
                    CTools_AlgoTrading::SetdoubleToCStatic(m_staticMiddlePrice,price);
            }
        }

        if(!util::isInvalidValue(depthMarketDate.PreSettlementPrice))
            CTools_AlgoTrading::SetStringToCStatic(m_staticYesterdayPrice,CTools_AlgoTrading::StringFromDouble(depthMarketDate.PreSettlementPrice));
    
        if(m_bCloseupComboBasicprice)
        {
            switch(m_comboBasicPrice.GetCurSel())
            {
            case 0://���           
                CTools_AlgoTrading::SetStringToCEdit(m_editBasicPrice,CTools_AlgoTrading::GetStringFromCStatic(m_staticBuyPrice).c_str());
                break;
            case 1://���¼�            
                CTools_AlgoTrading::SetStringToCEdit(m_editBasicPrice,CTools_AlgoTrading::GetStringFromCStatic(m_staticLatestPrice).c_str());
                break;
            case 2://����           
                CTools_AlgoTrading::SetStringToCEdit(m_editBasicPrice,CTools_AlgoTrading::GetStringFromCStatic(m_staticSellPrice).c_str());
                break;
            case 3://�м��            
                //CTools_AlgoTrading::SetStringToCEdit(m_editBasicPrice,CTools_AlgoTrading::GetStringFromCStatic(m_staticMiddlePrice).c_str());
                break;
            case 4://�����          
                CTools_AlgoTrading::SetStringToCEdit(m_editBasicPrice,CTools_AlgoTrading::GetStringFromCStatic(m_staticYesterdayPrice).c_str());
                break;
            } 
        }    

        pAlgo->m_pAlgoAutoData->InstrumentName = CTools_AlgoTrading::GetCStringFromCEdit(m_editInstrumentID);
        if (CTools_AlgoTrading::GetStringFromCComboBox(m_comboOpenclose)=="����")
            pAlgo->m_pAlgoAutoData->InstrumentOffsetID = 0;
        else if (CTools_AlgoTrading::GetStringFromCComboBox(m_comboOpenclose)=="ƽ��")
            pAlgo->m_pAlgoAutoData->InstrumentOffsetID = 1;
        else 
            pAlgo->m_pAlgoAutoData->InstrumentOffsetID = 2;

        pAlgo->m_pAlgoAutoData->InstrumentDirection = CTools_AlgoTrading::GetStringFromCComboBox(m_comboBuysell)=="����"?0:1;
        pAlgo->m_pAlgoAutoData->InstrumentCount = CTools_AlgoTrading::GetStringFromCEdit(m_editInstrumentCount);
        pAlgo->m_pAlgoAutoData->LimitPrice = CTools_AlgoTrading::GetStringFromCEdit(m_editPriceLimit);



        m_depthMarketDate = depthMarketDate;

        //�������
        PlatformStru_InstrumentInfo *pInfo = (PlatformStru_InstrumentInfo*)pAlgo->GetInstrumentInfo();
        if(pInfo != NULL)
            pAlgo->m_pAlgoAutoData->PriceTick = pInfo->PriceTick;

        pAlgo->m_pAlgoAutoData->BasicPrice.value = CTools_AlgoTrading::GetStringFromCEdit(m_editBasicPrice);

        pAlgo->m_pAlgoAutoData->NewVolume = depthMarketDate.Volume - m_nCurVolume;
        pAlgo->m_pAlgoAutoData->LatestPrice = depthMarketDate.LastPrice;

        m_nCurVolume = depthMarketDate.Volume;
        pAlgo->m_pAlgoAutoData->BidVolume = m_depthMarketDate.BidVolume1;
        pAlgo->m_pAlgoAutoData->AskVolume = m_depthMarketDate.AskVolume1;
    }
}

void CGeneralParamDlg::OnDeltaposSpinPricelimit(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    double fPrice,dVolume = 0.0;
    char strQty[64];
    ZeroMemory(strQty, sizeof(strQty));
    m_editPriceLimit.GetLine(0, strQty, 63);
    dVolume = atof(strQty);

	CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
	if(pAlgo == NULL)
		return;

    PlatformStru_InstrumentInfo *pInfo = (PlatformStru_InstrumentInfo*)pAlgo->GetInstrumentInfo();
    if(pInfo != NULL)
        pAlgo->m_pAlgoAutoData->PriceTick = pInfo->PriceTick;

    CheckSumPrice(dVolume, pAlgo->m_pAlgoAutoData->PriceTick,0);
    dVolume -= pNMUpDown->iDelta * (pAlgo->m_pAlgoAutoData->PriceTick);

    fPrice = AnalysePrice(dVolume,pAlgo->GetUpperLimitPrice(),pAlgo->GetLowerLimitPrice());

    sprintf_s(strQty, "%.4f", fPrice);
    m_editPriceLimit.SetSel(0, -1);
    m_editPriceLimit.ReplaceSel(strQty);

    *pResult = 0;
}
void CGeneralParamDlg::OnDeltaposSpinMarketdegreed(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    int nVolume = 0;
    char strQty[64];
    ZeroMemory(strQty, sizeof(strQty));
    m_editMarketDegreed.GetLine(0, strQty, 63);
    nVolume = atoi(strQty);

    nVolume -= pNMUpDown->iDelta;
    if(nVolume<=0)
        nVolume = 0;
    if(nVolume>100)
        nVolume = 100;
    sprintf_s(strQty, "%d", nVolume);
    m_editMarketDegreed.SetSel(0, -1);
    m_editMarketDegreed.ReplaceSel(strQty);

    m_slidMarketDegreed.SetPos(nVolume);

    *pResult = 0;
}

void CGeneralParamDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default

    int nCur=m_slidMarketDegreed.GetPos();
    char strQty[64];
    ZeroMemory(strQty, sizeof(strQty));
    sprintf_s(strQty, "%d", nCur);
    m_editMarketDegreed.SetSel(0, -1);
    m_editMarketDegreed.ReplaceSel(strQty);

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CGeneralParamDlg::OnCbnDropdownComboBasicprice()
{
    // TODO: Add your control notification handler code here
    m_bCloseupComboBasicprice = FALSE;
}

void CGeneralParamDlg::OnCbnCloseupComboBasicprice()
{
    // TODO: Add your control notification handler code here
    m_bCloseupComboBasicprice = TRUE;
}

void CGeneralParamDlg::OnCbnSelchangeComboBasicprice()
{
    // TODO: Add your control notification handler code here
    CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
    if(pAlgo == NULL) return;

    if(5==m_comboBasicPrice.GetCurSel())
    {
        m_editBasicPrice.EnableWindow(TRUE);
        m_editBasicPrice.SetWindowText(_T(""));
    }else
        m_editBasicPrice.EnableWindow(FALSE);
}

void CGeneralParamDlg::OnEnChangeEditMarketdegreed()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
    int nVolume = 0;
    char strQty[64];
    ZeroMemory(strQty, sizeof(strQty));
    m_editMarketDegreed.GetLine(0, strQty, 63);
    nVolume = atoi(strQty);

    m_slidMarketDegreed.SetPos(nVolume);
}

void CGeneralParamDlg::OnBnClickedBtnOk()
{
    // TODO: Add your control notification handler code here
    CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
    if(pAlgo == NULL)
        return;

    if(!CheckParam()) 
        return; 

    m_bOkClicked = TRUE;
    pAlgo->QueryPositionInfo(GetInstrumentID());
    //pAlgo->StartTrading();    
}

BOOL CGeneralParamDlg::CheckParam(void)
{
    CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
    if(pAlgo == NULL) 
        return FALSE;

    CString strTemp,strTemp1,strTemp2,strTemp3,strTemp4,strTemp5,Temp1,Temp2,Temp3,Temp4,Temp5;  
    CTime startTime,endTime;

    m_editInstrumentID.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
    {
        ::MessageBox(NULL, "���׺�ԼΪ�գ�������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    if (CTools_AlgoTrading::IsHaveDot(strTemp))
    {
        ::MessageBox(NULL, "���׺�Լ�Ƿ��ַ�������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    m_editInstrumentCount.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
    {
        ::MessageBox(NULL, "��������Ϊ�գ�������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    if (CTools_AlgoTrading::IsHaveDot(strTemp))
    {
        ::MessageBox(NULL, "����������֧��С��������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

    m_editPriceLimit.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
    {
        ::MessageBox(NULL, "�ɱ��޼�Ϊ�գ�������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }
    
    double fPrice=CTools_AlgoTrading::GetDoubleFromCEdit(m_editPriceLimit);
    if(CheckSumPrice(fPrice, pAlgo->m_pAlgoAutoData->PriceTick))
    {
        ::MessageBox(NULL, "�ɱ��޼۲�����С�䶯�۸�������������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }
    //fPrice = AnalysePrice(fPrice,pAlgo->m_tDepthMarketData.UpperLimitPrice,pAlgo->m_tDepthMarketData.LowerLimitPrice);
    

	m_editInstrumentStartTime.GetTime(startTime);
	m_editInstrumentEndTime.GetTime(endTime);
	if (endTime <= startTime)
	{
 		::MessageBox(NULL, "����ʱ��������������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
 		return FALSE;
	}

    m_editMarketDegreed.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
    {
        ::MessageBox(NULL, "�г������Ϊ�գ�������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }else
    {
        if (CTools_AlgoTrading::IsHaveDot(strTemp))
        {
            ::MessageBox(NULL, "�г�����Ȳ�֧��С��������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }
    }

    m_editIdleTime.GetWindowText(Temp1); 
    m_editExecuteTime.GetWindowText(Temp2);
    m_editOverTimeRetryCount.GetWindowText(Temp3);
    m_editOverTimeTotal.GetWindowText(Temp4);

    if (!Temp1.IsEmpty())
    {
        if(CTools_AlgoTrading::IsHaveDot(Temp1))
        {
            ::MessageBox(NULL, "����ʱ�䲻֧��С��������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }
    }

    if (!Temp2.IsEmpty())
    {
        if(CTools_AlgoTrading::IsHaveDot(Temp2))
        {
            ::MessageBox(NULL, "ִ��ʱ�䲻֧��С��������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }
    }

    if((BST_UNCHECKED == m_checkOverTimeRetryCount.GetCheck())?FALSE:TRUE)
    {
        if (Temp3.IsEmpty())
        {
            ::MessageBox(NULL, "ѡ��ʱ���Դ����󣬱������þ������ֵ", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }

        if(CTools_AlgoTrading::IsHaveDot(Temp3))
        {
            ::MessageBox(NULL, "��ʱ���Դ�����֧��С��������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }

        if (Temp1.IsEmpty() && Temp2.IsEmpty())
        {
            ::MessageBox(NULL, "ѡ��ʱ���Դ����󣬱������ÿ��л�ִ��ʱ��", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }
    }

    if((BST_UNCHECKED == m_checkOverTimeTotal.GetCheck())?FALSE:TRUE )
    {
        if (Temp4.IsEmpty())
        {
            ::MessageBox(NULL, "ѡ��ʱ������ʱ���󣬱������þ������ֵ", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }

        if(CTools_AlgoTrading::IsHaveDot(Temp4))
        {
            ::MessageBox(NULL, "��ʱ������ʱ����֧��С��������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }

        if (Temp1.IsEmpty() && Temp2.IsEmpty() )
        {
            ::MessageBox(NULL, "ѡ��ʱ������ʱ���󣬱������ÿ��л�ִ��ʱ��", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }
    }

    m_editOrderCount.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
    {
        ::MessageBox(NULL, "�µ�����Ϊ�գ�������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }else
    {
        if(CTools_AlgoTrading::IsHaveDot(strTemp))
        {
            ::MessageBox(NULL, "�µ�������֧��С��������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }
    }

//     m_editResetOrderCount.GetWindowText(strTemp);
//     if(CTools_AlgoTrading::IsHaveDot(strTemp))
//     {
//         ::MessageBox(NULL, "����������֧��С��������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
//         return FALSE;
//     }

    int nOrderCount;
    if(1 == m_comboOrderCount.GetCurSel())//����
        nOrderCount = CTools_AlgoTrading::GetIntFromCEdit(m_editOrderCount);
    else if(0 == m_comboOrderCount.GetCurSel())//�ٷֱ�
    {
        if(0 == m_comboBuysell.GetCurSel())//����
        {
            nOrderCount = (int)((CTools_AlgoTrading::GetDoubleFromCEdit(m_editOrderCount)/100.0f) * pAlgo->m_pAlgoAutoData->AskVolume);
        }else if(1 == m_comboBuysell.GetCurSel())//����        
        {
            nOrderCount = (int)((CTools_AlgoTrading::GetDoubleFromCEdit(m_editOrderCount)/100.0f) * pAlgo->m_pAlgoAutoData->BidVolume);
        }
    }    

//     int nResetOrderCount;
// 	if(1 == m_comboResetOrderCount.GetCurSel())//����
// 	{    
// 		nResetOrderCount = CTools_AlgoTrading::GetIntFromCEdit(m_editResetOrderCount);
// 
//         if (nResetOrderCount > nOrderCount)
//         {
//             ::MessageBox(NULL, "����������Ӧ�ó����µ�����������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
//             return FALSE;
//         }
// 	}else if(0 == m_comboResetOrderCount.GetCurSel())//�ٷֱ�
// 	{
// 		nResetOrderCount = (int)(CTools_AlgoTrading::GetDoubleFromCEdit(m_editResetOrderCount)/100.0f * nOrderCount);
// 	}  

    int nTotalCount = CTools_AlgoTrading::GetIntFromCEdit(m_editInstrumentCount);
    if (nOrderCount > nTotalCount)
    {
        ::MessageBox(NULL, "�µ��������ܳ����������������������г������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
        return FALSE;
    }

//     if(1 == m_comboResetOrderCount.GetCurSel())//����
//     {
//         m_editResetOrderCount.GetWindowText(strTemp);
//         if (CTools_AlgoTrading::IsHaveDot(strTemp))
//         {
//             ::MessageBox(NULL, "���������ڵ�λ�²�֧��С��������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
//             return FALSE;
//         }
//     }

//     m_editSlidPriceSpread1.GetWindowText(Temp1);
    m_comboMultiPrice.GetLBText(m_comboMultiPrice.GetCurSel(),Temp1);
//     m_editSlidPriceSpread2.GetWindowText(Temp2);
//     m_editSlidPriceSpread3.GetWindowText(Temp3);
//     m_editSlidPriceSpread4.GetWindowText(Temp4);
//     m_editSlidPriceSpread5.GetWindowText(Temp5);
    
//     if(1 == m_comboSlidPriceSpread.GetCurSel())//��λ
//     {
        if (CTools_AlgoTrading::IsHaveDot(Temp1)/*||
            CTools_AlgoTrading::IsHaveDot(Temp2)||
            CTools_AlgoTrading::IsHaveDot(Temp3)||
            CTools_AlgoTrading::IsHaveDot(Temp4)||
            CTools_AlgoTrading::IsHaveDot(Temp5)*/)
        {
            ::MessageBox(NULL, "��Ӧ�µ�����λ�ڵ�λ�²�֧��С��������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
            return FALSE;
        }         

//     }


//     double value1 = CTools_AlgoTrading::GetDoubleFromCEdit(m_editRelateOrderCount1);
//     double value2 = CTools_AlgoTrading::GetDoubleFromCEdit(m_editRelateOrderCount2);
//     double value3 = CTools_AlgoTrading::GetDoubleFromCEdit(m_editRelateOrderCount3);
//     double value4 = CTools_AlgoTrading::GetDoubleFromCEdit(m_editRelateOrderCount4);
//     double value5 = CTools_AlgoTrading::GetDoubleFromCEdit(m_editRelateOrderCount5);

//     switch(m_comboRelateOrderCount.GetCurSel())
//     {
//     case 0://�ٷֱ�
//         if ((value1+value2+value3+value4+value5) != 100.0f)
//         {
//             ::MessageBox(NULL, "��Ӧ�µ����ٷֱ�����������100%������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
//             return FALSE;
//         }
//         break;
//     case 1://����
//         if (1 == m_comboOrderCount.GetCurSel())//����
//         {
//             if((int)(value1+value2+value3+value4+value5) != CTools_AlgoTrading::GetIntFromCEdit(m_editOrderCount))
//             {
//                 ::MessageBox(NULL, "��Ӧ�µ������������������µ�����������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
//                 return FALSE;
//             }
//         }else if(0 == m_comboOrderCount.GetCurSel())//�ٷֱ�
//         {
//             int nOrder = 0;
//             switch(m_comboBuysell.GetCurSel())
//             {
//             case 0://����
//                 nOrder = (int)((CTools_AlgoTrading::GetDoubleFromCEdit(m_editOrderCount)/100.0f) * m_depthMarketDate.AskVolume1);
//                 break;
//             case 1://����
//                 nOrder = (int)((CTools_AlgoTrading::GetDoubleFromCEdit(m_editOrderCount)/100.0f) * m_depthMarketDate.BidVolume1);
//             }
// 
//             if((int)(value1+value2+value3+value4+value5) > nOrder)
//             {
//                 ::MessageBox(NULL, "��Ӧ�µ����ٷֱ�����Ӧ�õ���100%������������", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
//                 return FALSE;
//             }
//         }
//         break;
//     }

// 	m_editRelateOrderCount1.GetWindowText(strTemp1);
// 	m_editRelateOrderCount2.GetWindowText(strTemp2);
// 	m_editRelateOrderCount3.GetWindowText(strTemp3);
// 	m_editRelateOrderCount4.GetWindowText(strTemp4);
// 	m_editRelateOrderCount5.GetWindowText(strTemp5);

// 	if (Temp1.IsEmpty() && !strTemp1.IsEmpty() || !Temp1.IsEmpty() && strTemp1.IsEmpty())
// 	{
// 		::MessageBox(NULL, "�����۲����µ���Ҫ�ɶԳ��֣�����д����", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
// 		return FALSE;
// 	}
//     else if (Temp2.IsEmpty() && !strTemp2.IsEmpty() || !Temp2.IsEmpty() && strTemp2.IsEmpty())
// 	{
// 		::MessageBox(NULL, "�����۲����µ���Ҫ�ɶԳ��֣�����д����", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
// 		return FALSE;
// 	}else if (Temp3.IsEmpty() && !strTemp3.IsEmpty() || !Temp3.IsEmpty() && strTemp3.IsEmpty())
// 	{
// 		::MessageBox(NULL, "�����۲����µ���Ҫ�ɶԳ��֣�����д����", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
// 		return FALSE;
// 	}else if (Temp4.IsEmpty() && !strTemp4.IsEmpty() || !Temp4.IsEmpty() && strTemp4.IsEmpty())
// 	{
// 		::MessageBox(NULL, "�����۲����µ���Ҫ�ɶԳ��֣�����д����", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
// 		return FALSE;
// 	}else if (Temp5.IsEmpty() && !strTemp5.IsEmpty() || !Temp5.IsEmpty() && strTemp5.IsEmpty())
// 	{
// 		::MessageBox(NULL, "�����۲����µ���Ҫ�ɶԳ��֣�����д����", "������ʾ", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
// 		return FALSE;
// 	}

    return TRUE;
}

void CGeneralParamDlg::AddToolTip()
{
    EnableToolTips(TRUE);
    m_toolTip.Create(this);
    m_toolTip.Activate(TRUE);
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_INSTRUMENTID),"���׺�Լ����Լ����򣬿����޸ĺ�Լ��");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_INSTRUMENTID),"���׺�Լ��");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_BUYSELL),"���׷���");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_COMBO_BUYSELL),"���׷��������������");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_COMBO_OPENCLOSE),"��ƽ�����֡�ƽ�ֻ�ƽ��");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_INSTRUMENTCOUNT),"Ŀ��ֵ��ϵͳ���������ﵽ�ĳɽ�������");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_INSTRUMENTCOUNT),"Ŀ��ֵ��ϵͳ���������ﵽ�ĳɽ�������");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_PRICELIMIT),"���ʽ�ɱ��������ƣ����뷽��������ֵ����������������ֵ��������������Ϊ��ֵ�µ�");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_PRICELIMIT),"���ʽ�ɱ��������ƣ����뷽��������ֵ����������������ֵ��������������Ϊ��ֵ�µ���");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_START),"���׿�ʼʱ�䡣");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_DATETIMEPICKER_START),"���׿�ʼʱ�䡣");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_END),"���׽���ʱ�䡣");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_DATETIMEPICKER_END),"���׽���ʱ�䡣");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_MARKETDEGREED),"�����µ������ܳ����г�������������ĳ˻����������Ϊ0��������ơ�");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_MARKETDEGREED),"�����µ������ܳ����г�������������ĳ˻����������Ϊ0��������ơ�");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_ORDERCOUNT),"ÿ���µ�������");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_ORDERCOUNT),"ÿ���µ�������");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_RESETORDERCOUNT),"���ѳɽ������ﵽ��������ʱ��δ�ɽ����������������°���ǰ�̿�����ҵ���");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_RESETORDERCOUNT),"���ѳɽ������ﵽ��������ʱ��δ�ɽ����������������°���ǰ�̿�����ҵ���");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_BASICPRICE),"��ʾÿ���µ��ļ۸�Ļ���ֵ�����������µ��۸���Դ�Ϊ�������������۲����ֵ�����ж��Ƿ��г����޼ۣ���δ�������Դ˼۸��µ�����������޼������޼ۼ۸�����µ�������");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_IDLETIME),"����ָ����ʱ�����޳ɽ��������ҵ������µ���");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_IDLETIME),"����ָ����ʱ�����޳ɽ��������ҵ������µ���");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_EXECUTETIME),"���ÿ�ʹҵ�ִ��ʱ�䳬��ָ����ʱ�䣬������ǰ�ҵ�����ʼ��һ���µ���");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_EXECUTETIME),"���ÿ�ʹҵ�ִ��ʱ�䳬��ָ����ʱ�䣬������ǰ�ҵ�����ʼ��һ���µ���");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_OVERRETRYCOUNT),"�����ѡ����жϳ�ʱ���Դ����ۼ�ֵ���糬�����趨ֵ����ϵͳ����ֹͣ��δ�ɽ���������δ�ﵽ��������������ʧ��");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_EDIT_OVERRETRYTOTAL),"�����ѡ����ж����ڿ���ʱ���ִ��ʱ������µĳ����������µ�����ʱ�����糬�����趨ֵ����ϵͳ����ֹͣ��δ�ɽ���������δ�ﵽ��������������ʧ��");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_SLIDPRICESPREAD),"�����۲������˵���Ϊ �ٷֱ�/��λ ѡ��Ϊ��λʱ��������µ��۸�Ϊ ���̼۸�+�۲�*��С�䶯��λ;���ѡ��Ϊ�ٷֱȣ����µ��۸�Ϊ ���̼۸�*��1+�����۲���ݺ�Լ����С�䶯��λ������ȡ��ӽ���ֵ��");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_COMBO_SLIDPRICESPREAD),"�����۲������˵���Ϊ �ٷֱ�/��λ ѡ��Ϊ��λʱ��������µ��۸�Ϊ ���̼۸�+�۲�*��С�䶯��λ;���ѡ��Ϊ�ٷֱȣ����µ��۸�Ϊ ���̼۸�*��1+�����۲���ݺ�Լ����С�䶯��λ������ȡ��ӽ���ֵ��");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_STATIC_RESETORDERCOUNT2),"��Ӧ�µ�����Ϊ���ٷֱȡ�������������,���ѡ���ǰٷֱ�,����Ҫ��֤�������ֺ�Ϊ100;���ѡ�������������ֻ����д��������");
    m_toolTip.AddTool((CWnd*)GetDlgItem(IDC_COMBO_RESETORDERCOUNT2),"��Ӧ�µ�����Ϊ���ٷֱȡ�������������,���ѡ���ǰٷֱ�,����Ҫ��֤�������ֺ�Ϊ100;���ѡ�������������ֻ����д��������");
}

void CGeneralParamDlg::GetAlgoParamFromWnd(void)
{
    CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
    if(pAlgo == NULL) 
        return;

    //�������
    CString strTemp;
    pAlgo->m_pAlgoAutoData->InstrumentName = CTools_AlgoTrading::GetStringFromCEdit(m_editInstrumentID);    
    pAlgo->m_pAlgoAutoData->InstrumentDirection = m_comboBuysell.GetCurSel();
    pAlgo->m_pAlgoAutoData->InstrumentOffsetID = m_comboOpenclose.GetCurSel();
    pAlgo->m_pAlgoAutoData->InstrumentCount = CTools_AlgoTrading::GetStringFromCEdit(m_editInstrumentCount);
    pAlgo->m_pAlgoAutoData->LimitPrice = CTools_AlgoTrading::GetStringFromCEdit(m_editPriceLimit);
    m_editInstrumentStartTime.GetTime(pAlgo->m_pAlgoAutoData->StartTime); 
    m_editInstrumentEndTime.GetTime(pAlgo->m_pAlgoAutoData->EndTime); 
    pAlgo->m_pAlgoAutoData->MarketDegreed = CTools_AlgoTrading::GetIntFromCEdit(m_editMarketDegreed);
    pAlgo->m_pAlgoAutoData->OrderCount.value = CTools_AlgoTrading::GetStringFromCEdit(m_editOrderCount);
    pAlgo->m_pAlgoAutoData->OrderCount.type = m_comboOrderCount.GetCurSel();
//     pAlgo->m_pAlgoAutoData->ResetCount.value = CTools_AlgoTrading::GetStringFromCEdit(m_editResetOrderCount);
//     pAlgo->m_pAlgoAutoData->ResetCount.type = m_comboResetOrderCount.GetCurSel();
    pAlgo->m_pAlgoAutoData->BasicPrice.value = CTools_AlgoTrading::GetStringFromCEdit(m_editBasicPrice);
    pAlgo->m_pAlgoAutoData->BasicPrice.type = m_comboBasicPrice.GetCurSel();
    pAlgo->m_pAlgoAutoData->IdleTime.value = CTools_AlgoTrading::GetStringFromCEdit(m_editIdleTime);
    pAlgo->m_pAlgoAutoData->IdleTime.type = m_comboIdleTime.GetCurSel();
    pAlgo->m_pAlgoAutoData->ExecuteTime.value = CTools_AlgoTrading::GetStringFromCEdit(m_editExecuteTime);
    pAlgo->m_pAlgoAutoData->ExecuteTime.type = m_comboExecuteTime.GetCurSel();
    pAlgo->m_pAlgoAutoData->RetryCount = CTools_AlgoTrading::GetStringFromCEdit(m_editOverTimeRetryCount);
    pAlgo->m_pAlgoAutoData->RetryTotalTime.value = CTools_AlgoTrading::GetStringFromCEdit(m_editOverTimeTotal);
    pAlgo->m_pAlgoAutoData->RetryTotalTime.type = m_comboOverTimeTotal.GetCurSel();
    pAlgo->m_pAlgoAutoData->bActiceOpt = (BST_CHECKED == m_checkActiveOptimize.GetCheck())?true:false;
    pAlgo->m_pAlgoAutoData->OptimizeIndex = m_comboActiveOptimize.GetCurSel();
//     pAlgo->m_pAlgoAutoData->bMultiPrice = (BST_CHECKED == m_btnCheckMultiPrice.GetCheck())?true:false;

    m_editIdleTime.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
        pAlgo->m_pAlgoAutoData->bIdleTime = FALSE;
    else
        pAlgo->m_pAlgoAutoData->bIdleTime = TRUE;

    m_editExecuteTime.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
        pAlgo->m_pAlgoAutoData->bExecuteTime = FALSE;
    else
        pAlgo->m_pAlgoAutoData->bExecuteTime = TRUE;

//     pAlgo->m_pAlgoAutoData->bMultiPrice = (BST_CHECKED == m_btnCheckMultiPrice.GetCheck())?true:false;

    //�嵵
    pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.data.clear();
//     pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.data.clear();
    
    CString str0,str1,str2,str3,str4;
//     m_editSlidPriceSpread1.GetWindowText(str0);
    int nIndex = m_comboMultiPrice.GetCurSel();
    if(nIndex>0)
        m_comboMultiPrice.GetLBText(nIndex,str0);
//     m_editSlidPriceSpread2.GetWindowText(str1);
//     m_editSlidPriceSpread3.GetWindowText(str2);
//     m_editSlidPriceSpread4.GetWindowText(str3);
//     m_editSlidPriceSpread5.GetWindowText(str4);
    

    pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.index = 1;//m_comboSlidPriceSpread.GetCurSel();
    pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.data.push_back(str0.GetBuffer(0));
//     pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.data.push_back(str1.GetBuffer(0));
//     pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.data.push_back(str2.GetBuffer(0));
//     pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.data.push_back(str3.GetBuffer(0));
//     pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.data.push_back(str4.GetBuffer(0));

//     str0.ReleaseBuffer();
//     str1.ReleaseBuffer();
//     str2.ReleaseBuffer();
//     str3.ReleaseBuffer();
//     str4.ReleaseBuffer();

//     m_editRelateOrderCount1.GetWindowText(str0);
//     m_editRelateOrderCount2.GetWindowText(str1);
//     m_editRelateOrderCount3.GetWindowText(str2);
//     m_editRelateOrderCount4.GetWindowText(str3);
//     m_editRelateOrderCount5.GetWindowText(str4);
//     pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.index = m_comboRelateOrderCount.GetCurSel();//0;
//     pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.data.push_back(str0.GetBuffer(0));
//     pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.data.push_back(str1.GetBuffer(0));
//     pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.data.push_back(str2.GetBuffer(0));
//     pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.data.push_back(str3.GetBuffer(0));
//     pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.data.push_back(str4.GetBuffer(0));

    str0.ReleaseBuffer();
//     str1.ReleaseBuffer();
//     str2.ReleaseBuffer();
//     str3.ReleaseBuffer();
//     str4.ReleaseBuffer();

    pAlgo->m_pAlgoAutoData->bActiceOverTime = (BST_UNCHECKED == m_checkOverTimeRetryCount.GetCheck())?FALSE:TRUE;
    pAlgo->m_pAlgoAutoData->bActiceTotalTime = (BST_UNCHECKED == m_checkOverTimeTotal.GetCheck())?FALSE:TRUE;

    //�������
    PlatformStru_InstrumentInfo *pInfo = (PlatformStru_InstrumentInfo*)pAlgo->GetInstrumentInfo();
    if(pInfo != NULL)
        pAlgo->m_pAlgoAutoData->PriceTick = pInfo->PriceTick;
}


void CGeneralParamDlg::SetAlgoParamToWnd(BOOL bEditChanged/* = FALSE*/)
{
    CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
    if(pAlgo == NULL) 
        return;

    if(!bEditChanged)
        CTools_AlgoTrading::SetStringToCEdit(m_editInstrumentID,pAlgo->m_pAlgoAutoData->InstrumentName.c_str());
    m_comboBuysell.SetCurSel(pAlgo->m_pAlgoAutoData->InstrumentDirection);
    m_comboOpenclose.SetCurSel(pAlgo->m_pAlgoAutoData->InstrumentOffsetID);
    CTools_AlgoTrading::SetStringToCEdit(m_editInstrumentCount,pAlgo->m_pAlgoAutoData->InstrumentCount.c_str());
    CTools_AlgoTrading::SetStringToCEdit(m_editPriceLimit,pAlgo->m_pAlgoAutoData->LimitPrice.c_str());
    m_spinMarketDegreed.SetPos(pAlgo->m_pAlgoAutoData->MarketDegreed);
    CTools_AlgoTrading::SetIntToCEdit(m_editMarketDegreed,pAlgo->m_pAlgoAutoData->MarketDegreed);
    CTools_AlgoTrading::SetStringToCEdit(m_editOrderCount,pAlgo->m_pAlgoAutoData->OrderCount.value.c_str());
    m_comboOrderCount.SetCurSel(pAlgo->m_pAlgoAutoData->OrderCount.type);
//     CTools_AlgoTrading::SetStringToCEdit(m_editResetOrderCount,pAlgo->m_pAlgoAutoData->ResetCount.value.c_str());
//     m_comboResetOrderCount.SetCurSel(pAlgo->m_pAlgoAutoData->ResetCount.type);
//     m_comboResetOrderCount.EnableWindow(pAlgo->m_pAlgoAutoData->OrderCount.type);
    m_comboBasicPrice.SetCurSel(pAlgo->m_pAlgoAutoData->BasicPrice.type);
    m_editBasicPrice.EnableWindow((5==pAlgo->m_pAlgoAutoData->BasicPrice.type)?TRUE:FALSE);
    //��������
    BOOL bHaveData = FALSE;
    vector<OPTPARAM*>::iterator iter = pAlgo->m_pAlgoAutoData->vecOptInfo.begin();
    for (;iter!=pAlgo->m_pAlgoAutoData->vecOptInfo.end();iter++)
    {
        m_comboActiveOptimize.AddString((*iter)->Name.c_str());
        bHaveData = TRUE;
    }
    m_checkActiveOptimize.EnableWindow(bHaveData);  
    m_comboActiveOptimize.EnableWindow(bHaveData); 

    if (pAlgo->m_pAlgoAutoData->bActiceOpt)    
        m_checkActiveOptimize.SetCheck(BST_CHECKED);        
    else
        m_checkActiveOptimize.SetCheck(BST_UNCHECKED); 

    m_comboActiveOptimize.SetCurSel(pAlgo->m_pAlgoAutoData->OptimizeIndex);

    CTools_AlgoTrading::SetStringToCEdit(m_editIdleTime,pAlgo->m_pAlgoAutoData->IdleTime.value.c_str());
    m_comboIdleTime.SetCurSel(pAlgo->m_pAlgoAutoData->IdleTime.type);
    CTools_AlgoTrading::SetStringToCEdit(m_editExecuteTime,pAlgo->m_pAlgoAutoData->ExecuteTime.value.c_str());
    m_comboExecuteTime.SetCurSel(pAlgo->m_pAlgoAutoData->ExecuteTime.type);    
    
    if(pAlgo->m_pAlgoAutoData->bActiceOverTime)
    {
        m_checkOverTimeRetryCount.SetCheck(BST_CHECKED);
    }else
    {
        m_checkOverTimeRetryCount.SetCheck(BST_UNCHECKED);
    }
    
    CTools_AlgoTrading::SetStringToCEdit(m_editOverTimeRetryCount,pAlgo->m_pAlgoAutoData->RetryCount.c_str());
    CTools_AlgoTrading::SetStringToCEdit(m_editOverTimeTotal,pAlgo->m_pAlgoAutoData->RetryTotalTime.value.c_str());
    m_comboOverTimeTotal.SetCurSel(pAlgo->m_pAlgoAutoData->RetryTotalTime.type);

    if(pAlgo->m_pAlgoAutoData->bActiceTotalTime)
    {
        m_checkOverTimeTotal.SetCheck(BST_CHECKED);
    }else
    {
        m_checkOverTimeTotal.SetCheck(BST_UNCHECKED);
    }

//     if (pAlgo->m_pAlgoAutoData->bMultiPrice)
//         m_btnCheckMultiPrice.SetCheck(BST_CHECKED);
//     else
//         m_btnCheckMultiPrice.SetCheck(BST_UNCHECKED);   

//     EnableMultiPriceControls(pAlgo->m_pAlgoAutoData->bMultiPrice);


    //�嵵
    vector<string> vecPriceSpread = pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.data;
//     vector<string> vecOrderCount = pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.data;

//     m_comboSlidPriceSpread.SetCurSel(pAlgo->m_pAlgoAutoData->WDDATA_PriceSpread.index);
//     for (int i=0;i<(int)vecPriceSpread.size();i++)
//     {
//         if(i == 0)
//             m_editSlidPriceSpread1.SetWindowText(vecPriceSpread[i].c_str()); 
            int nIndex = m_comboMultiPrice.FindStringExact( 0, vecPriceSpread[0].c_str());
            m_comboMultiPrice.SetCurSel((nIndex<0)?5:nIndex);
//         else if(i == 1)
//             m_editSlidPriceSpread2.SetWindowText(vecPriceSpread[i].c_str());
//         else if(i == 2)
//             m_editSlidPriceSpread3.SetWindowText(vecPriceSpread[i].c_str());
//         else if(i == 3)
//             m_editSlidPriceSpread4.SetWindowText(vecPriceSpread[i].c_str());
//         else if(i == 4)
//             m_editSlidPriceSpread5.SetWindowText(vecPriceSpread[i].c_str());
// 
//     }

//     m_comboRelateOrderCount.SetCurSel(pAlgo->m_pAlgoAutoData->WDDATA_OrderCount.index);
//     m_comboRelateOrderCount.EnableWindow(pAlgo->m_pAlgoAutoData->OrderCount.type);    
//     for (int i=0;i<(int)vecOrderCount.size();i++)
//     {
//         if(i == 0)
//             m_editRelateOrderCount1.SetWindowText(vecOrderCount[0].c_str());
//         else if(i == 1)
//             m_editRelateOrderCount2.SetWindowText(vecOrderCount[i].c_str());
//         else if(i == 2)
//             m_editRelateOrderCount3.SetWindowText(vecOrderCount[i].c_str());
//         else if(i == 3)
//             m_editRelateOrderCount4.SetWindowText(vecOrderCount[i].c_str());
//         else if(i == 4)
//             m_editRelateOrderCount5.SetWindowText(vecOrderCount[i].c_str());
//     }
}


void CGeneralParamDlg::UpdataOptCombo(int optIndex,BOOL bHaveItem)
{
    if (bHaveItem)
    {
        CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
        if(pAlgo == NULL) return;

        SINGLEOPT *pSingle = NULL;

        m_comboActiveOptimize.ResetContent();
        vector<OPTPARAM*>::iterator iter = pAlgo->m_pAlgoAutoData->vecOptInfo.begin();
        for (;iter!=pAlgo->m_pAlgoAutoData->vecOptInfo.end();iter++)
        {
            m_comboActiveOptimize.AddString((*iter)->Name.c_str());
        }

        int nIndex = (optIndex == -1)?0:optIndex;
        m_comboActiveOptimize.SetCurSel(nIndex);//ѡ���Ż�����
        pAlgo->m_pAlgoAutoData->OptimizeIndex = nIndex;

        m_comboActiveOptimize.EnableWindow(TRUE);
        m_checkActiveOptimize.EnableWindow(TRUE);
    }else
    {
        m_comboActiveOptimize.ResetContent();
        m_comboActiveOptimize.EnableWindow(FALSE);
        m_checkActiveOptimize.EnableWindow(FALSE);
        m_checkActiveOptimize.SetCheck(BST_UNCHECKED);
    }
}

void CGeneralParamDlg::OnEnChangeEditInstrumentid()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_staticBuyPrice.SetWindowText("��");
    m_staticSellPrice.SetWindowText("��");
    m_staticMiddlePrice.SetWindowText("��");
    m_staticYesterdayPrice.SetWindowText("��");
    m_staticLatestPrice.SetWindowText("��");

	CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
	if(pAlgo == NULL) return;

	char strInstrumentID[64];
	ZeroMemory(strInstrumentID, sizeof(strInstrumentID));
	m_editInstrumentID.GetLine(0, strInstrumentID, sizeof(strInstrumentID)-1);


    pAlgo->UnsubscribeQuot(pAlgo->GetOldInstrumentID());
	pAlgo->QueryInstrumentInfo(CString(strInstrumentID));
	pAlgo->SubscribeQuot(CString(strInstrumentID));
	pAlgo->QueryQuotSnapshot(CString(strInstrumentID));	
	pAlgo->SetOldInstrumentID(CString(strInstrumentID));

    if(pAlgo->m_pAlgoAutoData->ReadIni((string)strInstrumentID))
        SetAlgoParamToWnd(TRUE);
}

void CGeneralParamDlg::OnBnClickedBtnCancel()
{
    // TODO: Add your control notification handler code here
    CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
    if(pAlgo != NULL)
    {
        CMessageBoxDialog dlg(this);
        if(!pAlgo->IsTradeTime())
            dlg.m_Text = "����δ��ɣ��ǽ���ʱ���ֹͣ���ף���������������";
        else
            dlg.m_Text = "����δ��ɣ����йҵ���������";
        if(dlg.DoModal() == IDOK)  
        {
            pAlgo->StopTrading();
        }
        else
            return;    
    }    
}

void CGeneralParamDlg::EnableCtrls(bool bEnable)
{
    m_btnPlay.EnableWindow(bEnable);
    m_btnOptSetup.EnableWindow(bEnable);
    m_editInstrumentID.EnableWindow(bEnable);
    m_comboBuysell.EnableWindow(bEnable);
    m_comboOpenclose.EnableWindow(bEnable);
    m_editInstrumentCount.EnableWindow(bEnable);
    m_editPriceLimit.EnableWindow(bEnable);
    m_spinPriceLimit.EnableWindow(bEnable);
    m_editInstrumentStartTime.EnableWindow(bEnable);
    m_editInstrumentEndTime.EnableWindow(bEnable);
    m_slidMarketDegreed.EnableWindow(bEnable);
    m_editMarketDegreed.EnableWindow(bEnable);
    m_spinMarketDegreed.EnableWindow(bEnable);
    m_editOrderCount.EnableWindow(bEnable);
    m_comboOrderCount.EnableWindow(bEnable);
//     m_editResetOrderCount.EnableWindow(bEnable);
//     m_comboResetOrderCount.EnableWindow(bEnable);
    m_comboBasicPrice.EnableWindow(bEnable);
    m_editBasicPrice.EnableWindow(bEnable);
//     m_comboSlidPriceSpread.EnableWindow(bEnable);
    m_comboMultiPrice.EnableWindow(bEnable);
//     m_editSlidPriceSpread1.EnableWindow(bEnable);
    m_editSlidPriceSpread2.EnableWindow(bEnable);
    m_editSlidPriceSpread3.EnableWindow(bEnable);
    m_editSlidPriceSpread4.EnableWindow(bEnable);
    m_editSlidPriceSpread5.EnableWindow(bEnable);
//     m_comboRelateOrderCount.EnableWindow(bEnable);
//     m_editRelateOrderCount1.EnableWindow(bEnable);
    m_editRelateOrderCount2.EnableWindow(bEnable);
    m_editRelateOrderCount3.EnableWindow(bEnable);
    m_editRelateOrderCount4.EnableWindow(bEnable);
    m_editRelateOrderCount5.EnableWindow(bEnable);    
    m_comboIdleTime.EnableWindow(bEnable);
    m_comboExecuteTime.EnableWindow(bEnable);
    m_comboOverTimeTotal.EnableWindow(bEnable);
    m_editIdleTime.EnableWindow(bEnable);
    m_editExecuteTime.EnableWindow(bEnable);
    m_checkOverTimeRetryCount.EnableWindow(bEnable);
    m_editOverTimeRetryCount.EnableWindow(bEnable);
    m_checkOverTimeTotal.EnableWindow(bEnable);
    m_editOverTimeTotal.EnableWindow(bEnable);
    m_btnCheckMultiPrice.EnableWindow(bEnable);

    if(!m_btnCheckMultiPrice.GetCheck())
        EnableMultiPriceControls(false);

//     if (0 == m_comboOrderCount.GetCurSel())//�ٷֱ�
//     {
//         m_comboResetOrderCount.SetCurSel(0);
//         m_comboResetOrderCount.EnableWindow(FALSE);

//         m_comboRelateOrderCount.SetCurSel(0);
//         m_comboRelateOrderCount.EnableWindow(FALSE);
//     }else
//     {
//         if (bEnable)
//         {
//             m_comboResetOrderCount.EnableWindow(TRUE);
//             m_comboRelateOrderCount.EnableWindow(TRUE);
//         } 
//     }

    if (bEnable)
    {
        if (m_comboActiveOptimize.GetCount() > 0)
        { 
            m_checkActiveOptimize.EnableWindow(bEnable);
            m_comboActiveOptimize.EnableWindow(bEnable);
        }

        m_btnPlay.SetWindowText("���¿�ʼ");
    }else
    {
        m_checkActiveOptimize.EnableWindow(bEnable);
        m_comboActiveOptimize.EnableWindow(bEnable);
        m_btnPlay.SetWindowText("��ʼִ��");
    }
}

void CGeneralParamDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	if (m_staticSplit.GetSafeHwnd() != NULL)
	{
		CPen pen(PS_SOLID,1,RGB(192,192,192));
		CPen *oldpen=dc.SelectObject(&pen);

		RECT rect;
		m_staticSplit.GetWindowRect(&rect);
		ScreenToClient(&rect);
		int wid = rect.right - rect.left;

		dc.MoveTo(rect.left,rect.top+18);
		dc.LineTo(rect.left+wid,rect.top+18);
		dc.SelectObject(oldpen);
	}
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void CGeneralParamDlg::OnCbnSelchangeComboOrdercount()
{
    // TODO: Add your control notification handler code here
//     if (0 == m_comboOrderCount.GetCurSel())//�ٷֱ�
//     {
//         m_comboResetOrderCount.SetCurSel(0);
//         m_comboResetOrderCount.EnableWindow(FALSE);

//         m_comboRelateOrderCount.SetCurSel(0);
//         m_comboRelateOrderCount.EnableWindow(FALSE);
//     }else
//     {
//         m_comboResetOrderCount.EnableWindow(TRUE);
//         m_comboRelateOrderCount.EnableWindow(TRUE);
//     }
}

BOOL CGeneralParamDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (NULL != m_toolTip.GetSafeHwnd())
    {
        m_toolTip.RelayEvent(pMsg);
    }

    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)  
        return TRUE;
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
        return TRUE;

    return CDialog::PreTranslateMessage(pMsg);
}
void CGeneralParamDlg::OnBnClickedCheckMultiPrice()
{
    // TODO: Add your control notification handler code here
    EnableMultiPriceControls(m_btnCheckMultiPrice.GetCheck());
}

void CGeneralParamDlg::EnableMultiPriceControls(BOOL bEnable)
{
    m_editSlidPriceSpread2.EnableWindow(bEnable);
    m_editSlidPriceSpread3.EnableWindow(bEnable);
    m_editSlidPriceSpread4.EnableWindow(bEnable);
    m_editSlidPriceSpread5.EnableWindow(bEnable);
    m_editRelateOrderCount2.EnableWindow(bEnable);
    m_editRelateOrderCount3.EnableWindow(bEnable);
    m_editRelateOrderCount4.EnableWindow(bEnable);
    m_editRelateOrderCount5.EnableWindow(bEnable);
}

