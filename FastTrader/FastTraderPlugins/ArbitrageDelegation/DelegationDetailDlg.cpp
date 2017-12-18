// DelegationDetail.cpp : implementation file
//

#include "stdafx.h"
#include "Tools.h"
#include "PlusinAPI.h"
#include "ArbitrageDelegation.h"
#include "DelegationDetailDlg.h"


// CDelegationDetailDlg dialog

IMPLEMENT_DYNAMIC(CDelegationDetailDlg, CDialog)

CDelegationDetailDlg::CDelegationDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDelegationDetailDlg::IDD, pParent)
{
}

CDelegationDetailDlg::CDelegationDetailDlg(set<long>& setFTID,CWnd* pParent /*=NULL*/)
: CDialog(CDelegationDetailDlg::IDD, pParent)
{
    m_setFTID = setFTID;
}

CDelegationDetailDlg::CDelegationDetailDlg(set<OrderKey>& setOrderKey,CWnd* pParent /*=NULL*/)
: CDialog(CDelegationDetailDlg::IDD, pParent)
{   
    m_setOrderKey = setOrderKey;
}

CDelegationDetailDlg::~CDelegationDetailDlg()
{
    m_mapOrderInfo.clear();
    m_setFTID.clear();
}

void CDelegationDetailDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_DETAIL, m_listDetailCtrl);
}


BEGIN_MESSAGE_MAP(CDelegationDetailDlg, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_REVOKE, &CDelegationDetailDlg::OnBnClickedButtonRevoke)
    ON_BN_CLICKED(IDOK, &CDelegationDetailDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CDelegationDetailDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDelegationDetailDlg message handlers

BOOL CDelegationDetailDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_listDetailCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

    //������ 
    int i=0;
    m_listDetailCtrl.InsertColumn( i++, "�������",     LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "��Լ",         LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "����",         LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "��ƽ",         LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "�ҵ�״̬",     LVCFMT_CENTER,    120 ); 
    m_listDetailCtrl.InsertColumn( i++, "�����۸�",     LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "��������",     LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "δ�ɽ�����",   LVCFMT_CENTER,    80 );
    m_listDetailCtrl.InsertColumn( i++, "�ɽ�����",     LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "��ϸ״̬",     LVCFMT_CENTER,    120 ); 
    m_listDetailCtrl.InsertColumn( i++, "����ʱ��",     LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "���ᱣ֤��",   LVCFMT_CENTER,    80 ); 
    m_listDetailCtrl.InsertColumn( i++, "����������",   LVCFMT_CENTER,    80 ); 
    m_listDetailCtrl.InsertColumn( i++, "Ͷ��",         LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "������",       LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "��������",     LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "��������",     LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "�Ƿ�ǿƽ",     LVCFMT_CENTER,    60 ); 
    m_listDetailCtrl.InsertColumn( i++, "ǿƽ��Ϣ",     LVCFMT_CENTER,    60 ); 

    if(!m_mapOrderInfo.empty())
        SetDataToListCtrl();
    if(!m_setOrderKey.empty())
    {
        if(g_hWnd_FT!=NULL)
        {
            PlusinAPI* pAPI=PlusinAPI::GetInstance();
            if(pAPI)
            {
                for(set<OrderKey>::iterator itset=m_setOrderKey.begin();itset!=m_setOrderKey.end();itset++)
                    pAPI->Send(CMDID_ALL_ORDER_INFO_QUERY2,&(*itset),sizeof(OrderKey));
            }
        }        
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDelegationDetailDlg::SetDataToListCtrl()
{    
    int index=0;
    for(map<string,PlatformStru_OrderInfo>::iterator it=m_mapOrderInfo.begin();it!=m_mapOrderInfo.end();it++,index++)
    {
        PlatformStru_OrderInfo *pOrderData = &(it->second);

        std::string strCombOffsetFlag,strHedgeFlag,strOrderStatus,strForceCloseReason;
        switch(pOrderData->CombOffsetFlag[0])
        {
        case THOST_FTDC_OF_Open:
            strCombOffsetFlag = "����";
            break;
        case THOST_FTDC_OF_Close:
            strCombOffsetFlag = "ƽ��";
            break;
        case THOST_FTDC_OF_ForceClose:
            strCombOffsetFlag = "ǿƽ";
            break;
        case THOST_FTDC_OF_CloseToday:
            strCombOffsetFlag = "ƽ��";
            break;
        case THOST_FTDC_OF_CloseYesterday:
            strCombOffsetFlag = "ƽ��";
            break;
        case THOST_FTDC_OF_ForceOff:
            strCombOffsetFlag = "ǿ��";
            break;
        case THOST_FTDC_OF_LocalForceClose:
            strCombOffsetFlag = "����ǿƽ";
            break;
        }
        switch(pOrderData->CombHedgeFlag[0])
        {
        case THOST_FTDC_HF_Speculation: 
            strHedgeFlag = "Ͷ��";
            break;
        case THOST_FTDC_HF_Arbitrage:
            strHedgeFlag = "����";
            break;
        case THOST_FTDC_HF_Hedge:
            strHedgeFlag = "�ױ�";
            break;
        }

        switch(pOrderData->OrderStatus)
        {
        case THOST_FTDC_OST_AllTraded:
            strOrderStatus = "ȫ���ɽ�";
            break;
        case THOST_FTDC_OST_PartTradedQueueing:
            strOrderStatus = "���ֳɽ����ڶ�����";
            break;
        case THOST_FTDC_OST_PartTradedNotQueueing:
            strOrderStatus = "���ֳɽ����ڶ�����";
            break;
        case THOST_FTDC_OST_NoTradeQueueing:
            strOrderStatus = "δ�ɽ����ڶ�����";
            break;
        case THOST_FTDC_OST_NoTradeNotQueueing:
            strOrderStatus = "δ�ɽ����ڶ�����";
            break;
        case THOST_FTDC_OST_Canceled:
            strOrderStatus = "����";
            break;
        case THOST_FTDC_OST_Unknown:
            strOrderStatus = "δ֪";
            break;
        case THOST_FTDC_OST_NotTouched:
            strOrderStatus = "��δ����";
            break;
        case THOST_FTDC_OST_Touched:
            strOrderStatus = "�Ѵ���";
            break;
        }

        switch(pOrderData->ForceCloseReason)
        {
        case THOST_FTDC_FCC_NotForceClose:
            strForceCloseReason = "��ǿƽ";
            break;
        case THOST_FTDC_FCC_LackDeposit:
            strForceCloseReason = "�ʽ���";
            break;
        case THOST_FTDC_OST_PartTradedNotQueueing:
            strForceCloseReason = "�ͻ�����";
            break;
        case THOST_FTDC_FCC_MemberOverPositionLimit:
            strForceCloseReason = "��Ա����";
            break;
        case THOST_FTDC_FCC_NotMultiple:
            strForceCloseReason = "�ֲַ�������";
            break;
        case THOST_FTDC_FCC_Violation:
            strForceCloseReason = "Υ��";
            break;
        case THOST_FTDC_FCC_Other:
            strForceCloseReason = "����";
            break;
        case THOST_FTDC_FCC_PersonDeliv:
            strForceCloseReason = "��Ȼ���ٽ�����";
            break;
        case THOST_FTDC_FCC_ForceClose:
            strForceCloseReason = "ǿƽ";
            break;                
        }

        int i=1;
        m_listDetailCtrl.InsertItem(index,pOrderData->OrderSysID);                                                                   //�������
        m_listDetailCtrl.SetItemText(index,i++,pOrderData->InstrumentID);                                                            //��Լ
        m_listDetailCtrl.SetItemText(index,i++,pOrderData->Direction=='1'?"��":"��");                                                     //����
        m_listDetailCtrl.SetItemText(index,i++,strCombOffsetFlag.c_str());                                                      //��ƽ
        m_listDetailCtrl.SetItemText(index,i++,strOrderStatus.c_str());                                                         //�ҵ�״̬
        m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromDouble(pOrderData->LimitPrice).c_str());                            //�����۸�
        m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromInt(pOrderData->VolumeTotalOriginal).c_str());                      //��������
        m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromInt(pOrderData->VolumeTotal).c_str());                              //δ�ɽ�����
        m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromInt(pOrderData->VolumeTotalOriginal-pOrderData->VolumeTotal).c_str());   //�ɽ�����
        m_listDetailCtrl.SetItemText(index,i++,pOrderData->StatusMsg);                                                               //��ϸ״̬
        m_listDetailCtrl.SetItemText(index,i++,pOrderData->InsertTime);                                                              //����ʱ��
        m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromDouble(pOrderData->freezeMargin).c_str());                          //���ᱣ֤��
        m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromDouble(pOrderData->troubleMoney).c_str());                          //����������
        m_listDetailCtrl.SetItemText(index,i++,strHedgeFlag.c_str());                                                           //Ͷ��
        m_listDetailCtrl.SetItemText(index,i++,pOrderData->ExchangeID);                                                              //������
        m_listDetailCtrl.SetItemText(index,i++,pOrderData->OrderRef);                                                                //��������
        m_listDetailCtrl.SetItemText(index,i++,pOrderData->UserForceClose?"��":"��");                                                //�Ƿ�ǿƽ
        m_listDetailCtrl.SetItemText(index,i++,strForceCloseReason.c_str());                                                    //ǿƽ��Ϣ

        //��
        m_listDetailCtrl.SetItemData(index,(DWORD_PTR)pOrderData);
    }
}

int CDelegationDetailDlg::GetListSelectedIndex()
{
    POSITION pos = m_listDetailCtrl.GetFirstSelectedItemPosition(); 
    return m_listDetailCtrl.GetNextSelectedItem(pos);
}

void CDelegationDetailDlg::OnBnClickedButtonRevoke()
{
    // TODO: Add your control notification handler code here
    int nSelectedIndex = GetListSelectedIndex();
    if(nSelectedIndex<0)
        return;

    if(IDCANCEL == ::MessageBox(NULL, "�Ƿ�ȷ�ϳ���?", "����", MB_OKCANCEL|MB_ICONWARNING|MB_SYSTEMMODAL))
        return;

    PlatformStru_OrderInfo* pOrderData = (PlatformStru_OrderInfo*)m_listDetailCtrl.GetItemData(nSelectedIndex);
    if(pOrderData!=NULL)
        RevokeOrder(pOrderData);
}

void CDelegationDetailDlg::RevokeOrder(PlatformStru_OrderInfo* pOrderData)
{
    TRACE("RevokeOrder OrderSysID = %s\n",pOrderData->OrderSysID);
    PlatformStru_InputOrderAction tAction;
    ZeroMemory(&tAction, sizeof(tAction));

    tAction.Thost.FrontID=pOrderData->FrontID;
    tAction.Thost.SessionID=pOrderData->SessionID;
    memcpy(tAction.Thost.OrderRef, pOrderData->OrderRef, sizeof(pOrderData->OrderRef));
    memcpy(tAction.Thost.OrderSysID, pOrderData->OrderSysID, sizeof(pOrderData->OrderSysID));
    memcpy(tAction.Thost.ExchangeID, pOrderData->ExchangeID, sizeof(pOrderData->ExchangeID));
    memcpy(tAction.Thost.InstrumentID, pOrderData->InstrumentID, sizeof(pOrderData->InstrumentID));


    PlusinAPI* pAPI=PlusinAPI::GetInstance();
    if(pAPI) 
        pAPI->Send(CMDID_ORDER_CANCEL_ORDER, &tAction, sizeof(tAction),1000);
}
void CDelegationDetailDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    ::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_USER_DETAILDLG_CLOSE,0,0);

    OnOK();
}

void CDelegationDetailDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    ::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_USER_DETAILDLG_CLOSE,0,0);

    OnCancel();
}

void CDelegationDetailDlg::Update(const PlatformStru_OrderInfo* pOrderData)
{
    if(pOrderData == NULL)
        return;    

    m_mapOrderInfo[pOrderData->InstrumentID] = *pOrderData;

    std::string strCombOffsetFlag,strHedgeFlag,strOrderStatus,strForceCloseReason,strOrderType;
    switch(pOrderData->CombOffsetFlag[0])
    {
    case THOST_FTDC_OF_Open:
        strCombOffsetFlag = "����";
        break;
    case THOST_FTDC_OF_Close:
        strCombOffsetFlag = "ƽ��";
        break;
    case THOST_FTDC_OF_ForceClose:
        strCombOffsetFlag = "ǿƽ";
        break;
    case THOST_FTDC_OF_CloseToday:
        strCombOffsetFlag = "ƽ��";
        break;
    case THOST_FTDC_OF_CloseYesterday:
        strCombOffsetFlag = "ƽ��";
        break;
    case THOST_FTDC_OF_ForceOff:
        strCombOffsetFlag = "ǿ��";
        break;
    case THOST_FTDC_OF_LocalForceClose:
        strCombOffsetFlag = "����ǿƽ";
        break;
    }
    switch(pOrderData->CombHedgeFlag[0])
    {
    case THOST_FTDC_HF_Speculation: 
        strHedgeFlag = "Ͷ��";
        break;
    case THOST_FTDC_HF_Arbitrage:
        strHedgeFlag = "����";
        break;
    case THOST_FTDC_HF_Hedge:
        strHedgeFlag = "�ױ�";
        break;
    }

    switch(pOrderData->OrderStatus)
    {
    case THOST_FTDC_OST_AllTraded:
        strOrderStatus = "ȫ���ɽ�";
        break;
    case THOST_FTDC_OST_PartTradedQueueing:
        strOrderStatus = "���ֳɽ����ڶ�����";
        break;
    case THOST_FTDC_OST_PartTradedNotQueueing:
        strOrderStatus = "���ֳɽ����ڶ�����";
        break;
    case THOST_FTDC_OST_NoTradeQueueing:
        strOrderStatus = "δ�ɽ����ڶ�����";
        break;
    case THOST_FTDC_OST_NoTradeNotQueueing:
        strOrderStatus = "δ�ɽ����ڶ�����";
        break;
    case THOST_FTDC_OST_Canceled:
        strOrderStatus = "����";
        break;
    case THOST_FTDC_OST_Unknown:
        strOrderStatus = "δ֪";
        break;
    case THOST_FTDC_OST_NotTouched:
        strOrderStatus = "��δ����";
        break;
    case THOST_FTDC_OST_Touched:
        strOrderStatus = "�Ѵ���";
        break;
    }

    switch(pOrderData->ForceCloseReason)
    {
    case THOST_FTDC_FCC_NotForceClose:
        strForceCloseReason = "��ǿƽ";
        break;
    case THOST_FTDC_FCC_LackDeposit:
        strForceCloseReason = "�ʽ���";
        break;
    case THOST_FTDC_OST_PartTradedNotQueueing:
        strForceCloseReason = "�ͻ�����";
        break;
    case THOST_FTDC_FCC_MemberOverPositionLimit:
        strForceCloseReason = "��Ա����";
        break;
    case THOST_FTDC_FCC_NotMultiple:
        strForceCloseReason = "�ֲַ�������";
        break;
    case THOST_FTDC_FCC_Violation:
        strForceCloseReason = "Υ��";
        break;
    case THOST_FTDC_FCC_Other:
        strForceCloseReason = "����";
        break;
    case THOST_FTDC_FCC_PersonDeliv:
        strForceCloseReason = "��Ȼ���ٽ�����";
        break;
    case THOST_FTDC_FCC_ForceClose:
        strForceCloseReason = "ǿƽ";
        break;                
    }

    switch(pOrderData->OrderType)
    {
    case THOST_FTDC_ORDT_Normal:
        strOrderType = "����";
        break;
    case THOST_FTDC_ORDT_DeriveFromQuote:
        strOrderType = "��������";
        break;
    case THOST_FTDC_ORDT_DeriveFromCombination:
        strOrderType = "�������";
        break;
    case THOST_FTDC_ORDT_Combination:
        strOrderType = "��ϱ���";
        break;
    case THOST_FTDC_ORDT_ConditionalOrder:
        strOrderType = "������";
        break;
    case THOST_FTDC_ORDT_Swap:
        strOrderType = "������";
        break;
    }


    bool bMatch = false;
    for (int index=0;index<m_listDetailCtrl.GetItemCount();index++)
    {
        //����
        CString szInst = m_listDetailCtrl.GetItemText(index,1);
        if(szInst.CompareNoCase(pOrderData->InstrumentID)==0)
        {
            bMatch = true;

            int i=0;
            m_listDetailCtrl.SetItemText(index,i++,pOrderData->OrderSysID);                                                                   //�������
            m_listDetailCtrl.SetItemText(index,i++,pOrderData->InstrumentID);                                                            //��Լ
            m_listDetailCtrl.SetItemText(index,i++,pOrderData->Direction=='1'?"��":"��");                                                     //����
            m_listDetailCtrl.SetItemText(index,i++,strCombOffsetFlag.c_str());                                                      //��ƽ
            m_listDetailCtrl.SetItemText(index,i++,strOrderStatus.c_str());                                                         //�ҵ�״̬
            m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromDouble(pOrderData->LimitPrice).c_str());                            //�����۸�
            m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromInt(pOrderData->VolumeTotalOriginal).c_str());                      //��������
            m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromInt(pOrderData->VolumeTotal).c_str());                              //δ�ɽ�����
            m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromInt(pOrderData->VolumeTotalOriginal-pOrderData->VolumeTotal).c_str());   //�ɽ�����
            m_listDetailCtrl.SetItemText(index,i++,pOrderData->StatusMsg);                                                               //��ϸ״̬
            m_listDetailCtrl.SetItemText(index,i++,pOrderData->InsertTime);                                                              //����ʱ��
            m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromDouble(pOrderData->freezeMargin).c_str());                          //���ᱣ֤��
            m_listDetailCtrl.SetItemText(index,i++,CTools::StringFromDouble(pOrderData->troubleMoney).c_str());                          //����������
            m_listDetailCtrl.SetItemText(index,i++,strHedgeFlag.c_str());                                                           //Ͷ��
            m_listDetailCtrl.SetItemText(index,i++,pOrderData->ExchangeID);                                                              //������
            m_listDetailCtrl.SetItemText(index,i++,pOrderData->OrderRef);                                                                //��������
            m_listDetailCtrl.SetItemText(index,i++,strOrderType.c_str());                                                                //��������            
            m_listDetailCtrl.SetItemText(index,i++,pOrderData->UserForceClose?"��":"��");                                                //�Ƿ�ǿƽ
            m_listDetailCtrl.SetItemText(index,i++,strForceCloseReason.c_str());                                                    //ǿƽ��Ϣ
        }
    }  

    if(!bMatch)//���
    {
        int nCount = m_listDetailCtrl.GetItemCount();

        int i=1;
        m_listDetailCtrl.InsertItem(nCount,pOrderData->OrderSysID);                                                                   //�������
        m_listDetailCtrl.SetItemText(nCount,i++,pOrderData->InstrumentID);                                                            //��Լ
        m_listDetailCtrl.SetItemText(nCount,i++,pOrderData->Direction=='1'?"��":"��");                                                     //����
        m_listDetailCtrl.SetItemText(nCount,i++,strCombOffsetFlag.c_str());                                                      //��ƽ
        m_listDetailCtrl.SetItemText(nCount,i++,strOrderStatus.c_str());                                                         //�ҵ�״̬
        m_listDetailCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pOrderData->LimitPrice).c_str());                            //�����۸�
        m_listDetailCtrl.SetItemText(nCount,i++,CTools::StringFromInt(pOrderData->VolumeTotalOriginal).c_str());                      //��������
        m_listDetailCtrl.SetItemText(nCount,i++,CTools::StringFromInt(pOrderData->VolumeTotal).c_str());                              //δ�ɽ�����
        m_listDetailCtrl.SetItemText(nCount,i++,CTools::StringFromInt(pOrderData->VolumeTotalOriginal-pOrderData->VolumeTotal).c_str());   //�ɽ�����
        m_listDetailCtrl.SetItemText(nCount,i++,pOrderData->StatusMsg);                                                               //��ϸ״̬
        m_listDetailCtrl.SetItemText(nCount,i++,pOrderData->InsertTime);                                                              //����ʱ��
        m_listDetailCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pOrderData->freezeMargin).c_str());                          //���ᱣ֤��
        m_listDetailCtrl.SetItemText(nCount,i++,CTools::StringFromDouble(pOrderData->troubleMoney).c_str());                          //����������
        m_listDetailCtrl.SetItemText(nCount,i++,strHedgeFlag.c_str());                                                           //Ͷ��
        m_listDetailCtrl.SetItemText(nCount,i++,pOrderData->ExchangeID);                                                              //������
        m_listDetailCtrl.SetItemText(nCount,i++,pOrderData->OrderRef);                                                                //��������
        m_listDetailCtrl.SetItemText(nCount,i++,strOrderType.c_str());                                                                //��������  
        m_listDetailCtrl.SetItemText(nCount,i++,pOrderData->UserForceClose?"��":"��");                                                //�Ƿ�ǿƽ
        m_listDetailCtrl.SetItemText(nCount,i++,strForceCloseReason.c_str());                                                    //ǿƽ��Ϣ

        //��
        m_listDetailCtrl.SetItemData(nCount,(DWORD_PTR)&m_mapOrderInfo[pOrderData->InstrumentID]);
    }
}
