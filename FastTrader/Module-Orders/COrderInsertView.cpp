#include "StdAfx.h"
//#include "OrderInsertPanel.h"
#include "../inc/Module-Misc2/tools_util.h"
#include "../inc/Module-Misc/zqControl.h"

#include "../inc/Module-Misc2/tools_util.h"
#include "COrderBatchDailog.h"
#include "CEditOrderChangeDailog.h"
#include "CEditOrderReplaceDailog.h"
#include "cfgMgr/CfgMgr.h"
#include "COrderConfirmDlg.h"
#include "COrderParkedSelectDlg.h"
#include "COrderInsertView.h"

#include <math.h>

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


UIENTITY COrderInsertView::s_tStdView[conUI_MAX] = {
	{NULL, 3, 3, 160, 256, true, true},							// �嵵�����б��
	{NULL, 160+6, 3, 180, 120, true, true},						// ��׼�����
	{NULL, 160+6, 3+120+3, 180, 44, true, true},				// ��ť���ɿ�
	{NULL, 160+6, 3+120+3+44+3, 180, 64, true, true},			// ��Ϣ��ʾ���
	{NULL, 160+6, 3+120+3, 180, 72, false, true},				// ��������
	{NULL, 160+6, 3+120+3, 180, 72, false, true},				// ��������
	{NULL, 160+6, 3+120+3, 180, 72, false, true},				// ֹͣ����
	{NULL, 160+180+9, 3+16, 220, 24*11+10, false, true},		// ӯ�𵥿�
	{NULL, 160+6, 3+120+3+44+3+64, 90-2, 20, true, true},		// check�嵵����
	{NULL, 160+6+90+2, 3+120+3+44+3+64, 90-2, 20, false, true},	// checkǰ�˴���
	{NULL, 160+180+12, 3, 90-2, 20, false, true},				// checkֹӯֹ��
	{NULL, 160+180+12+90+2, 3, 90-2, 20, false, true},			// check����ί��
	{NULL, 160+6, 3, 180, 140, false, true},					// Pats�����
	{NULL, 160+6, 3+120+3, 180, 72, false, true}				    // FAKFOK����
};

UIENTITY COrderInsertView::s_tPositionView[conUI_MAX] = {
	{NULL, 3, 3, 160, 256, true, true},							// �嵵�����б��
	{NULL, 160+6, 3, 180, 120, true, true},						// ��׼�����
	{NULL, 160+6, 3+120+3, 180, 44, true, true},				// ��ť���ɿ�
	{NULL, 160+6, 3+120+3+44+3, 180, 64, true, true},			// ��Ϣ��ʾ���
	{NULL, 160+6, 3+120+3, 180, 72, false, true},				// ��������
	{NULL, 160+6, 3+120+3, 180, 72, false, true},				// ��������
	{NULL, 160+6, 3+120+3, 180, 72, false, true},				// ֹͣ����
	{NULL, 160+180+9, 3+16, 220, 24*11+2, true, true},			// ӯ�𵥿�
	{NULL, 160+6, 3+120+3+44+3+64, 90-2, 20, true, true},		// check�嵵����
	{NULL, 160+6+90+2, 3+120+3+44+3+64, 90-2, 20, false, true},	// checkǰ�˴���
	{NULL, 160+180+12, 3, 90-2, 20, true, true},				// checkֹӯֹ��
	{NULL, 160+180+12+90+2, 3, 90-2, 20, false, true},			// check����ί��
	{NULL, 160+6, 3, 180, 140, false, true},					// Pats�����
	{NULL, 160+6, 3+120+3, 180, 72, false, true}				    // FAKFOK����
};

UIENTITY COrderInsertView::s_tBatchView[conUI_MAX] = {
	{NULL, 3, 3, 160, 256, true, true},							// �嵵�����б��
	{NULL, 160+6, 3, 180, 120, true, true},						// ��׼�����
	{NULL, 160+6, 3+120+72, 180, 44, true, true},				// ��ť���ɿ�
	{NULL, 160+6, 3+120+3+44+3, 180, 64, false, true},			// ��Ϣ��ʾ���
	{NULL, 160+6, 3+120, 180, 72, true, true},				    // ��������
	{NULL, 160+6, 3+120, 180, 72, false, true},				    // ��������
	{NULL, 160+6, 3+120, 180, 72, false, true},				    // ֹͣ����
	{NULL, 160+180+9, 3+16, 220, 24*11+2, false, true},			// ӯ�𵥿�
	{NULL, 160+6, 3+120+72+44, 90-2, 20, true, true},		    // check�嵵����
	{NULL, 160+6+90+2, 3+120+3+72+44+3, 90-2, 20, false, true},	// checkǰ�˴���
	{NULL, 160+180+12, 3, 90-2, 20, false, true},				// checkֹӯֹ��
	{NULL, 160+180+12+90+2, 3, 90-2, 20, false, true},			// check����ί��
	{NULL, 160+6, 3, 180, 140, false, true},					// Pats�����
	{NULL, 160+6, 3+120, 180, 72, false, true}				    // FAKFOK����
};

UIENTITY COrderInsertView::s_tConditionView[conUI_MAX] = {
	{NULL, 3, 3, 160, 256, true, true},							// �嵵�����б��
	{NULL, 160+6, 3, 180, 120, true, true},						// ��׼�����
	{NULL, 160+6, 3+120+72, 180, 44, true, true},				// ��ť���ɿ�
	{NULL, 160+6, 3+120+3+44+3, 180, 64, false, true},			// ��Ϣ��ʾ���
	{NULL, 160+6, 3+120, 180, 72, false, true},				    // ��������
	{NULL, 160+6, 3+120, 180, 72, true, true},				    // ��������
	{NULL, 160+6, 3+120, 180, 72, false, true},				    // ֹͣ����
	{NULL, 160+180+9, 3+16, 220, 24*11+2, false, true},			// ӯ�𵥿�
	{NULL, 160+6, 3+120+72+44, 90-2, 20, true, true},		    // check�嵵����
	{NULL, 160+6+90+2, 3+120+72+44, 90-2, 20, false, true},	    // checkǰ�˴���
	{NULL, 160+180+12, 3, 90-2, 20, false, true},				// checkֹӯֹ��
	{NULL, 160+180+12+90+2, 3, 90-2, 20, false, true},			// check����ί��
	{NULL, 160+6, 3, 180, 140, false, true},					// Pats�����
	{NULL, 160+6, 3+120, 180, 72, false, true}				    // FAKFOK����
};

UIENTITY COrderInsertView::s_tStopView[conUI_MAX] = {
	{NULL, 3, 3, 160, 256, true, true},							// �嵵�����б��
	{NULL, 160+6, 3, 180, 140, true, true},						// ��׼�����
	{NULL, 160+6, 3+140+70, 180, 44, true, true},				// ��ť���ɿ�
	{NULL, 160+6, 3+140+3+44+3, 180, 64, false, true},			// ��Ϣ��ʾ���
	{NULL, 160+6, 3+140+3, 180, 72, false, true},				// ��������
	{NULL, 160+6, 3+140+3, 180, 72, false, true},				// ��������
	{NULL, 160+6, 3+140+3, 180, 72, true, true},				// ֹͣ����
	{NULL, 160+180+9, 3+16, 220, 24*11+10, false, true},		// ӯ�𵥿�
	{NULL, 160+6, 3+140+3+72+44+3, 90-2, 20, true, true},		// check�嵵����
	{NULL, 160+6+90+2, 3+140+3+72+44+3, 90-2, 20, false, true},	// checkǰ�˴���
	{NULL, 160+180+12, 3, 90-2, 20, false, true},				// checkֹӯֹ��
	{NULL, 160+180+12+90+2, 3, 90-2, 20, false, true},			// check����ί��
	{NULL, 160+6, 3, 180, 140, false, true},					// Pats�����
	{NULL, 160+6, 3+140+3, 180, 72, false, true}				    // FAKFOK����
};

UIENTITY COrderInsertView::s_tPatsView[conUI_MAX] = {
	{NULL, 3, 3, 160, 256, true, true},							// �嵵�����б��
	{NULL, 160+6, 3, 180, 164, false, true},					// ��׼�����
	{NULL, 160+6, 3+164+3, 180, 44, true, true},				// ��ť���ɿ�
	{NULL, 160+6, 3+164+3+44+3, 180, 44, true, true},			// ��Ϣ��ʾ���
	{NULL, 160+6, 3+164+3, 180, 72, false, true},				// ��������
	{NULL, 160+6, 3+164+3, 180, 72, false, true},				// ��������
	{NULL, 160+6, 3+164+3, 180, 72, false, true},				// ֹͣ����
	{NULL, 160+180+9, 3+16, 220, 24*11+10, false, true},		// ӯ�𵥿�
	{NULL, 160+6, 3+164+3+44+44+3, 90-2, 20, true, true},		// check�嵵����
	{NULL, 160+6+90+2, 3+164+3+44+44+3, 90-2, 20, false, true},	// checkǰ�˴���
	{NULL, 160+180+12, 3, 90-2, 20, false, true},				// checkֹӯֹ��
	{NULL, 160+180+12+90+2, 3, 90-2, 20, false, true},			// check����ί��
	{NULL, 160+6, 3, 180, 164, true, true},						// Pats�����
	{NULL, 160+6, 3+164+3, 180, 72, false, true}				    // FAKFOK����
};

UIENTITY COrderInsertView::s_tFAKFOKView[conUI_MAX] = {
	{NULL, 3, 3, 160, 256, true, true},							// �嵵�����б��
	{NULL, 160+6, 3, 180, 120, true, true},						// ��׼�����
	{NULL, 160+6, 3+120+72, 180, 44, true, true},				// ��ť���ɿ�
	{NULL, 160+6, 3+120+3+44+3, 180, 64, false, true},			// ��Ϣ��ʾ���
	{NULL, 160+6, 3+120, 180, 72, false, true},				    // ��������
	{NULL, 160+6, 3+120, 180, 72, false, true},				    // ��������
	{NULL, 160+6, 3+120, 180, 72, false, true},				    // ֹͣ����
	{NULL, 160+180+9, 3+16, 220, 24*11+2, false, true},			// ӯ�𵥿�
	{NULL, 160+6, 3+120+72+44, 90-2, 20, true, true},		    // check�嵵����
	{NULL, 160+6+90+2, 3+120+3+72+44+3, 90-2, 20, false, true},	// checkǰ�˴���
	{NULL, 160+180+12, 3, 90-2, 20, false, true},				// checkֹӯֹ��
	{NULL, 160+180+12+90+2, 3, 90-2, 20, false, true},			// check����ί��
	{NULL, 160+6, 3, 180, 140, false, true},					// Pats�����
	{NULL, 160+6, 3+120, 180, 72, true, true}				    // FAKFOK����
};


COrderInsertView::COrderInsertView(wxWindow *parent,
            wxWindowID winid ,
            const wxPoint& pos ,
            const wxSize& size ,
            long style,
            const wxString& name ):
	wxScrolledWindow(parent,winid,pos,size,style,name),
	m_poStaticFivePriceList(NULL),
	m_poPatsOrderInputPanel(NULL),
	m_poOrderInputPanel(NULL),
	m_poOrderInsertCommandPanel(NULL),
	m_poOrderRealMsgPanel(NULL),
	m_poOrderBatchPanel(NULL),
	m_poOrderConditionPanel(NULL),
	m_poOrderStopPanel(NULL),
	m_poOrderPositionPanel(NULL),
	m_colorBak(0,0,0),
	m_posWD(3,3),
	m_posOrder(3+WDPANEL_WIDTH,3)
{

	m_nViewStyle=-1;		//������ 3 ���˴�
	m_nViewSubStyle=-1;		//������ 3 ���˴�

	SetBackgroundColour(DEFAULT_COLOUR);
	wxFont oldfont=GetFont();
	oldfont.SetPointSize(9);
	//oldfont.SetWeight(wxBOLD);
	SetFont(oldfont);

	wxPoint ctlPos(0,0);
	wxSize ctlSz(0,0);

	m_poStaticFivePriceList = new CStaticFivePriceList(this, ID_CTL_WDSHOW);
	s_tStdView[conUI_WDList].pWin=(LPVOID)m_poStaticFivePriceList;
	s_tPositionView[conUI_WDList].pWin=(LPVOID)m_poStaticFivePriceList;
	s_tBatchView[conUI_WDList].pWin=(LPVOID)m_poStaticFivePriceList;
	s_tConditionView[conUI_WDList].pWin=(LPVOID)m_poStaticFivePriceList;
	s_tStopView[conUI_WDList].pWin=(LPVOID)m_poStaticFivePriceList;
	s_tPatsView[conUI_WDList].pWin=(LPVOID)m_poStaticFivePriceList;
	s_tFAKFOKView[conUI_WDList].pWin=(LPVOID)m_poStaticFivePriceList;

	// ���ﲻ��Ҫ����
	// �����������ResetLanguage()�����

	ctlPos.y+=2;
	wxCheckBox* itemCheckWDShow = new wxCheckBox(this, ID_CHECK_WDSHOW, wxEmptyString, ctlPos, wxDefaultSize, 0);//_("�嵵����")
	itemCheckWDShow->SetValue(true);
	s_tStdView[conUI_CheckWD].pWin=(LPVOID)itemCheckWDShow;
	s_tPositionView[conUI_CheckWD].pWin=(LPVOID)itemCheckWDShow;
	s_tBatchView[conUI_CheckWD].pWin=(LPVOID)itemCheckWDShow;
	s_tConditionView[conUI_CheckWD].pWin=(LPVOID)itemCheckWDShow;
	s_tStopView[conUI_CheckWD].pWin=(LPVOID)itemCheckWDShow;
	s_tPatsView[conUI_CheckWD].pWin=(LPVOID)itemCheckWDShow;
	s_tFAKFOKView[conUI_CheckWD].pWin=(LPVOID)itemCheckWDShow;

	wxCheckBox* itemCheckBox3 = new wxCheckBox( this, ID_CHECK_WINDOWTOPALWAYS, wxEmptyString);//_("ǰ�˴���")
	s_tStdView[conUI_CheckTopMost].pWin=(LPVOID)itemCheckBox3;
	s_tPositionView[conUI_CheckTopMost].pWin=(LPVOID)itemCheckBox3;
	s_tBatchView[conUI_CheckTopMost].pWin=(LPVOID)itemCheckBox3;
	s_tConditionView[conUI_CheckTopMost].pWin=(LPVOID)itemCheckBox3;
	s_tStopView[conUI_CheckTopMost].pWin=(LPVOID)itemCheckBox3;
	s_tPatsView[conUI_CheckTopMost].pWin=(LPVOID)itemCheckBox3;
	s_tFAKFOKView[conUI_CheckTopMost].pWin=(LPVOID)itemCheckBox3;

	wxCheckBox* itemCheckBox4 = new wxCheckBox( this, ID_DOSTOPPOSITION_CHECK, wxEmptyString);//_("ֹӯֹ��")
	s_tStdView[conUI_CheckPosition].pWin=(LPVOID)itemCheckBox4;
	s_tPositionView[conUI_CheckPosition].pWin=(LPVOID)itemCheckBox4;
	s_tBatchView[conUI_CheckPosition].pWin=(LPVOID)itemCheckBox4;
	s_tConditionView[conUI_CheckPosition].pWin=(LPVOID)itemCheckBox4;
	s_tStopView[conUI_CheckPosition].pWin=(LPVOID)itemCheckBox4;
	s_tPatsView[conUI_CheckPosition].pWin=(LPVOID)itemCheckBox4;
	s_tFAKFOKView[conUI_CheckPosition].pWin=(LPVOID)itemCheckBox4;

	wxCheckBox* itemCheckBox5 = new wxCheckBox( this, ID_DOCONDITION_CHECK, wxEmptyString);//_("����ί��")
	s_tStdView[conUI_CheckCondition].pWin=(LPVOID)itemCheckBox5;
	s_tPositionView[conUI_CheckCondition].pWin=(LPVOID)itemCheckBox5;
	s_tBatchView[conUI_CheckCondition].pWin=(LPVOID)itemCheckBox5;
	s_tConditionView[conUI_CheckCondition].pWin=(LPVOID)itemCheckBox5;
	s_tStopView[conUI_CheckCondition].pWin=(LPVOID)itemCheckBox5;
	s_tPatsView[conUI_CheckCondition].pWin=(LPVOID)itemCheckBox5;
	s_tFAKFOKView[conUI_CheckCondition].pWin=(LPVOID)itemCheckBox5;
	
	//ctlSz.Set(ORDERPANEL_WIDTH, 44);
	m_poOrderInsertCommandPanel = new COrderInsertCommandPanel(this);
	s_tStdView[conUI_Command].pWin=(LPVOID)m_poOrderInsertCommandPanel;
	s_tPositionView[conUI_Command].pWin=(LPVOID)m_poOrderInsertCommandPanel;
	s_tBatchView[conUI_Command].pWin=(LPVOID)m_poOrderInsertCommandPanel;
	s_tConditionView[conUI_Command].pWin=(LPVOID)m_poOrderInsertCommandPanel;
	s_tStopView[conUI_Command].pWin=(LPVOID)m_poOrderInsertCommandPanel;
	s_tPatsView[conUI_Command].pWin=(LPVOID)m_poOrderInsertCommandPanel;
	s_tFAKFOKView[conUI_Command].pWin=(LPVOID)m_poOrderInsertCommandPanel;
	
	//ctlSz.Set(ORDERPANEL_WIDTH, 64);
	m_poOrderRealMsgPanel = new COrderRealMsgPanel(this);
	s_tStdView[conUI_RealMsg].pWin=(LPVOID)m_poOrderRealMsgPanel;
	s_tPositionView[conUI_RealMsg].pWin=(LPVOID)m_poOrderRealMsgPanel;
	s_tBatchView[conUI_RealMsg].pWin=(LPVOID)m_poOrderRealMsgPanel;
	s_tConditionView[conUI_RealMsg].pWin=(LPVOID)m_poOrderRealMsgPanel;
	s_tStopView[conUI_RealMsg].pWin=(LPVOID)m_poOrderRealMsgPanel;
	s_tPatsView[conUI_RealMsg].pWin=(LPVOID)m_poOrderRealMsgPanel;
	s_tFAKFOKView[conUI_RealMsg].pWin=(LPVOID)m_poOrderRealMsgPanel;

	m_poOrderInputPanel = new COrderInputPanel(this);
	s_tStdView[conUI_StdInput].pWin=(LPVOID)m_poOrderInputPanel;
	s_tPositionView[conUI_StdInput].pWin=(LPVOID)m_poOrderInputPanel;
	s_tBatchView[conUI_StdInput].pWin=(LPVOID)m_poOrderInputPanel;
	s_tConditionView[conUI_StdInput].pWin=(LPVOID)m_poOrderInputPanel;
	s_tStopView[conUI_StdInput].pWin=(LPVOID)m_poOrderInputPanel;
	s_tPatsView[conUI_StdInput].pWin=(LPVOID)m_poOrderInputPanel;
	s_tFAKFOKView[conUI_StdInput].pWin=(LPVOID)m_poOrderInputPanel;
	
	m_poOrderBatchPanel = new COrderBatchPanel(this);
	s_tStdView[conUI_Batch].pWin=(LPVOID)m_poOrderBatchPanel;
	s_tPositionView[conUI_Batch].pWin=(LPVOID)m_poOrderBatchPanel;
	s_tBatchView[conUI_Batch].pWin=(LPVOID)m_poOrderBatchPanel;
	s_tConditionView[conUI_Batch].pWin=(LPVOID)m_poOrderBatchPanel;
	s_tStopView[conUI_Batch].pWin=(LPVOID)m_poOrderBatchPanel;
	s_tPatsView[conUI_Batch].pWin=(LPVOID)m_poOrderBatchPanel;
	s_tFAKFOKView[conUI_Batch].pWin=(LPVOID)m_poOrderBatchPanel;

	m_poOrderConditionPanel = new COrderConditionPanel(this);
	s_tStdView[conUI_Condition].pWin=(LPVOID)m_poOrderConditionPanel;
	s_tPositionView[conUI_Condition].pWin=(LPVOID)m_poOrderConditionPanel;
	s_tBatchView[conUI_Condition].pWin=(LPVOID)m_poOrderConditionPanel;
	s_tConditionView[conUI_Condition].pWin=(LPVOID)m_poOrderConditionPanel;
	s_tStopView[conUI_Condition].pWin=(LPVOID)m_poOrderConditionPanel;
	s_tPatsView[conUI_Condition].pWin=(LPVOID)m_poOrderConditionPanel;
	s_tFAKFOKView[conUI_Condition].pWin=(LPVOID)m_poOrderConditionPanel;

	m_poOrderStopPanel = new COrderStopPanel(this);
	s_tStdView[conUI_Stop].pWin=(LPVOID)m_poOrderStopPanel;
	s_tPositionView[conUI_Stop].pWin=(LPVOID)m_poOrderStopPanel;
	s_tBatchView[conUI_Stop].pWin=(LPVOID)m_poOrderStopPanel;
	s_tConditionView[conUI_Stop].pWin=(LPVOID)m_poOrderStopPanel;
	s_tStopView[conUI_Stop].pWin=(LPVOID)m_poOrderStopPanel;
	s_tPatsView[conUI_Stop].pWin=(LPVOID)m_poOrderStopPanel;
	s_tFAKFOKView[conUI_Stop].pWin=(LPVOID)m_poOrderStopPanel;

	m_poOrderPositionPanel = new COrderPositionPanel(this);
	s_tStdView[conUI_Position].pWin=(LPVOID)m_poOrderPositionPanel;
	s_tPositionView[conUI_Position].pWin=(LPVOID)m_poOrderPositionPanel;
	s_tBatchView[conUI_Position].pWin=(LPVOID)m_poOrderPositionPanel;
	s_tConditionView[conUI_Position].pWin=(LPVOID)m_poOrderPositionPanel;
	s_tStopView[conUI_Position].pWin=(LPVOID)m_poOrderPositionPanel;
	s_tPatsView[conUI_Position].pWin=(LPVOID)m_poOrderPositionPanel;
	s_tFAKFOKView[conUI_Position].pWin=(LPVOID)m_poOrderPositionPanel;

	m_poPatsOrderInputPanel = new CPatsOrderInputPanel(this);
	s_tStdView[conUI_PatsInput].pWin=(LPVOID)m_poPatsOrderInputPanel;
	s_tPositionView[conUI_PatsInput].pWin=(LPVOID)m_poPatsOrderInputPanel;
	s_tBatchView[conUI_PatsInput].pWin=(LPVOID)m_poPatsOrderInputPanel;
	s_tConditionView[conUI_PatsInput].pWin=(LPVOID)m_poPatsOrderInputPanel;
	s_tStopView[conUI_PatsInput].pWin=(LPVOID)m_poPatsOrderInputPanel;
	s_tPatsView[conUI_PatsInput].pWin=(LPVOID)m_poPatsOrderInputPanel;
	s_tFAKFOKView[conUI_PatsInput].pWin=(LPVOID)m_poPatsOrderInputPanel;

	m_poOrderFAKFOKPanel = new COrderFAKFOKPanel(this);
	s_tStdView[conUI_FAKFOK].pWin=(LPVOID)m_poOrderFAKFOKPanel;
	s_tPositionView[conUI_FAKFOK].pWin=(LPVOID)m_poOrderFAKFOKPanel;
	s_tBatchView[conUI_FAKFOK].pWin=(LPVOID)m_poOrderFAKFOKPanel;
	s_tConditionView[conUI_FAKFOK].pWin=(LPVOID)m_poOrderFAKFOKPanel;
	s_tStopView[conUI_FAKFOK].pWin=(LPVOID)m_poOrderFAKFOKPanel;
	s_tPatsView[conUI_FAKFOK].pWin=(LPVOID)m_poOrderFAKFOKPanel;
	s_tFAKFOKView[conUI_FAKFOK].pWin=(LPVOID)m_poOrderFAKFOKPanel;

    SetScrollRate(10,10);
	wxCommandEvent MyEvent(wxEVT_GET_STYLE);
	((wxApp*)wxApp::GetInstance())->GetTopWindow()->ProcessEvent(MyEvent);
	m_nViewStyle=MyEvent.GetInt();
    void *p=MyEvent.GetClientData();
    SAFE_DELETE(p);
	m_poOrderInputPanel->SetViewStyle(m_nViewStyle);
	LoadStyleUI(m_nViewStyle, 0);
	m_nViewSubStyle=0;

}

void COrderInsertView::LoadStyleUI(int nViewStyle, int nViewSubStyle)
{

	Scroll(0, 0);

	m_poOrderInputPanel->SetViewMode(0);
//	if(nViewStyle!=m_nViewStyle) {
//		m_poOrderInputPanel->InitOrderView();
//	}
	m_poOrderPositionPanel->InitPositionPanel();

	UIENTITY* pUIArr = NULL;

	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		switch(nViewSubStyle) {
		case conSubStyle_Std:
			pUIArr = s_tStdView;
			break;
		case conSubStyle_Batch:
			pUIArr = s_tBatchView;
			break;
		case conSubStyle_Condition:
			pUIArr = s_tConditionView;
			break;
		case conSubStyle_Stop:
			pUIArr = s_tStopView;
			m_poOrderInputPanel->SetViewMode(4);
			break;
		case conSubStyle_Position:
			pUIArr = s_tPositionView;
			break;
		case conSubStyle_FAKFOK:
			pUIArr = s_tFAKFOKView;
			break;
		default:
			return;
			break;
		};
	}
	else {
		pUIArr = s_tPatsView;
	}

	int nNewPosX = 0;
	wxCheckBox* pCheckItem = (wxCheckBox*)FindWindowById(ID_CHECK_WDSHOW,this);
	if(pCheckItem)
	{
		if(!pCheckItem->GetValue())
			nNewPosX = -pUIArr[conUI_WDList].width;
	}

	for(int i=0; i<conUI_MAX; i++) {
		((wxWindow*)pUIArr[i].pWin)->SetPosition(wxPoint(nNewPosX+pUIArr[i].left, pUIArr[i].top));
		((wxWindow*)pUIArr[i].pWin)->SetSize(wxSize(pUIArr[i].width, pUIArr[i].height));
		((wxWindow*)pUIArr[i].pWin)->Show(pUIArr[i].isShow);
		((wxWindow*)pUIArr[i].pWin)->Enable(pUIArr[i].isEnable);
	}

	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		switch(nViewSubStyle) {
		case conSubStyle_Std:
			SetVirtualSize(wxSize(nNewPosX+pUIArr[conUI_WDList].width
					+pUIArr[conUI_StdInput].width+5, pUIArr[conUI_WDList].height));
			break;
		case conSubStyle_Batch:
		case conSubStyle_Condition:
		case conSubStyle_Stop:
		case conSubStyle_FAKFOK:
			SetVirtualSize(wxSize(nNewPosX+pUIArr[conUI_WDList].width
					+pUIArr[conUI_StdInput].width+5, pUIArr[conUI_WDList].height));
			break;
		case conSubStyle_Position:
			SetVirtualSize(wxSize(nNewPosX+pUIArr[conUI_WDList].width
					+pUIArr[conUI_StdInput].width+pUIArr[conUI_Position].width+5, 
					pUIArr[conUI_WDList].height));
			break;
		};
	}


	m_poOrderInsertCommandPanel->ResetViewStyle(nViewSubStyle);

	//wxCheckBox* pCheckItem = (wxCheckBox*)FindWindowById(ID_CHECK_WDSHOW,this);
	//pCheckItem->SetValue(true);

	SetConditionSelect(FALSE); 
	SetStopPositionSelect(FALSE);

	m_TabOrderVector.clear();

	//tab order
//	m_TabOrderVector.push_back(ID_OISTD_INSTRUMENTID_TEXT);
//	m_TabOrderVector.push_back(ID_OISTD_BUYSELL_COMBOBOX);
//	//m_TabOrderVector.push_back(ID_OISTD_HEDGE_CHECKBOX);
//	m_TabOrderVector.push_back(ID_OISTD_OPENCLOSE_COMBOBOX);
//	m_TabOrderVector.push_back(ID_OISTD_VOLUME_TEXT);
//	m_TabOrderVector.push_back(ID_OISTD_PRICE_TEXT);
//	m_TabOrderVector.push_back(ID_OISTD_ACCOUNT_COMBOCTRL);
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		m_poOrderInputPanel->InsertCtrlID2TabCtrl(m_TabOrderVector);
	}
	else {
		m_poPatsOrderInputPanel->InsertCtrlID2TabCtrl(m_TabOrderVector);
	}

	m_poOrderInsertCommandPanel->InsertCtrlID2TabCtrl(m_TabOrderVector);
	//switch(nViewSubStyle) {
	//case 0:
	//	m_TabOrderVector.push_back(ID_BUTTON_OK);
	//	break;
	//case 1:
	//	//m_TabOrderVector.push_back(ID_OBATCH_RADIOINPUTPRICE);
	//	m_TabOrderVector.push_back(ID_OBATCH_SPINCTRLVOLUME);
	//	m_TabOrderVector.push_back(ID_OBATCH_SPINCTRLTIMER);
	//	m_TabOrderVector.push_back(ID_BUTTON_BATCHORDER);
	//	break;
	//case 2:
	//	//m_TabOrderVector.push_back(ID_CONDITION_RADIONEWPRICE);
	//	m_TabOrderVector.push_back(ID_CONDITION_COMBOCONDITION);
	//	m_TabOrderVector.push_back(ID_CONDITION_TEXTPRICE);
	//	m_TabOrderVector.push_back(ID_BUTTON_CONDITIONORDER);
	//	break;
	//case 3:
	//	m_TabOrderVector.push_back(ID_BUTTON_OK);
	//	m_TabOrderVector.push_back(ID_BUTTON_POSITIONORDER);
	//	break;
	//case 4:
	//	//m_TabOrderVector.push_back(ID_STOP_RADIONEWPRICE);
	//	m_TabOrderVector.push_back(ID_STOP_TEXTPRICE);
	//	m_TabOrderVector.push_back(ID_STOP_TEXTTRIGGERPRICE);
	//	m_TabOrderVector.push_back(ID_BUTTON_STOPORDER);
	//	break;
	//}
	//m_TabOrderVector.push_back(ID_BUTTON_CANCEL);
	//m_TabOrderVector.push_back(ID_BUTTON_PARKEDORDER);
	//m_TabOrderVector.push_back(ID_BUTTON_MARKETPRICE);

    if(nViewSubStyle==conSubStyle_Batch&&m_poOrderBatchPanel!=NULL)
    {
        if(m_poOrderBatchPanel->GetPriceType()!=0)
        {
            EnablePriceInputCtl(false);
        }
    }

}

COrderInsertView::~COrderInsertView()
{
}

void COrderInsertView::SetInputInstrumentID(wxString& name, BOOL bSendChangeEvent)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return;
		m_poOrderInputPanel->SetInstrumentId(name, bSendChangeEvent);
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return;
		m_poPatsOrderInputPanel->SetInstrumentId(name, bSendChangeEvent);
	}
}

wxString COrderInsertView::GetInputInstrumentID()
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return wxT("");
		return m_poOrderInputPanel->GetInstrumentId();
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return wxT("");
		return m_poPatsOrderInputPanel->GetInstrumentId();
	}
}

void COrderInsertView::SetInputAccount(wxString& strAccount)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return;
		m_poOrderInputPanel->SetAccount(strAccount);
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return;
		m_poPatsOrderInputPanel->SetAccount(strAccount);
	}
}

wxString COrderInsertView::GetInputAccount()
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return wxT("");
		return m_poOrderInputPanel->GetAccount();
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return wxT("");
		return m_poPatsOrderInputPanel->GetAccount();
	}
}

void COrderInsertView::SetInputOrderType(int nOrderType)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		return;
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return;
		m_poPatsOrderInputPanel->SetOrderType(nOrderType);
	}
}

int COrderInsertView::GetInputOrderType()
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		return -1;
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return -1;
		return m_poPatsOrderInputPanel->GetOrderType();
	}
}

void COrderInsertView::SetInputPrice(double price)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return;
		m_poOrderInputPanel->SetPrice(price);
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return;
		m_poPatsOrderInputPanel->SetPrice(price);
	}
}

double COrderInsertView::GetInputPrice()
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return 0.0;
		return m_poOrderInputPanel->GetPrice();
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return 0.0;
		return m_poPatsOrderInputPanel->GetPrice();
	}
}

void COrderInsertView::SetInputPriceText(wxString& strPrice)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return;
		m_poOrderInputPanel->SetPriceText(strPrice);
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return;
		m_poPatsOrderInputPanel->SetPriceText(strPrice);
	}
}

wxString COrderInsertView::GetInputPriceText()
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return wxT("");
		return m_poOrderInputPanel->GetPriceText();
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return wxT("");
		return m_poPatsOrderInputPanel->GetPriceText();
	}
}

void COrderInsertView::SetInputLimitPrice(double price)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		return;
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return;
		m_poPatsOrderInputPanel->SetLimitPrice(price);
	}
}

double COrderInsertView::GetInputLimitPrice()
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		return 0.0;
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return 0.0;
		return m_poPatsOrderInputPanel->GetLimitPrice();
	}
}

void COrderInsertView::SetInputLimitPriceText(wxString& strPrice)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		return;
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return;
		m_poPatsOrderInputPanel->SetLimitPriceText(strPrice);
	}
}

wxString COrderInsertView::GetInputLimitPriceText()
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		return wxEmptyString;
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return wxEmptyString;
		return m_poPatsOrderInputPanel->GetLimitPriceText();
	}
}

void COrderInsertView::SetInputQty(int qty)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return;
		m_poOrderInputPanel->SetQty(qty);
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return;
		m_poPatsOrderInputPanel->SetQty(qty);
	}
}

int COrderInsertView::GetInputQty()
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return 0;
		return m_poOrderInputPanel->GetQty();
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return 0;
		return m_poPatsOrderInputPanel->GetQty();
	}
}

BOOL COrderInsertView::GetInputDirection()
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return 0;
		return m_poOrderInputPanel->GetDirection();
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return 0;
		return m_poPatsOrderInputPanel->GetDirection();
	}
}

void COrderInsertView::SetInputDirection(BOOL bDirection)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return;
		m_poOrderInputPanel->SetDirection(bDirection);
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return;
		m_poPatsOrderInputPanel->SetDirection(bDirection);
	}
}

int COrderInsertView::GetInputOCMode()
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return -1;
		return m_poOrderInputPanel->GetOpenClose();
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return -1;
		return m_poPatsOrderInputPanel->GetOpenClose()==0 ? 0 : 2;
	}
}

void COrderInsertView::SetInputOCMode(int nOCMode)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return;
		m_poOrderInputPanel->SetOpenClose(nOCMode);
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return;
		m_poPatsOrderInputPanel->SetOpenClose(nOCMode==0?0:1);
	}
}

BOOL COrderInsertView::GetInputHedge()
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return FALSE;
		return m_poOrderInputPanel->GetHedge();
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return FALSE;
		return m_poPatsOrderInputPanel->GetHedge();
	}
}

void COrderInsertView::SetInputHedge(int nHedgeStatus)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return;
		m_poOrderInputPanel->SetHedge(nHedgeStatus);
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return;
		m_poPatsOrderInputPanel->SetHedge(nHedgeStatus);
	}
}

BOOL COrderInsertView::GetInputAutoOpenClose()
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return FALSE;
		return m_poOrderInputPanel->GetAutoOpenClose();
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return FALSE;
		return m_poPatsOrderInputPanel->GetAutoOpenClose();
	}
}

void COrderInsertView::SetInputAutoOpenClose(BOOL bIsAuto)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return;
		m_poOrderInputPanel->SetAutoOpenClose(bIsAuto);
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return;
		m_poPatsOrderInputPanel->SetAutoOpenClose(bIsAuto);
	}
}

// ���ü۸����ģʽ
void COrderInsertView::SetInputAutoTrackPrice(BOOL bIsTrack)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return;
		m_poOrderInputPanel->SetAutoTrackPrice(bIsTrack);
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return;
		m_poPatsOrderInputPanel->SetAutoTrackPrice(bIsTrack);
	}
}

BOOL COrderInsertView::GetInputAutoTrackPrice()
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		if(m_poOrderInputPanel==NULL) return FALSE;
		return m_poOrderInputPanel->GetAutoTrackPrice();
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return FALSE;
		return m_poPatsOrderInputPanel->GetAutoTrackPrice();
	}
}

// ���ü۸����ģʽ
void COrderInsertView::SetInputAutoTrackLimitPrice(BOOL bIsTrack)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		return;
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return;
		m_poPatsOrderInputPanel->SetAutoTrackLimitPrice(bIsTrack);
	}
}

BOOL COrderInsertView::GetInputAutoTrackLimitPrice()
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
		return FALSE;
	}
	else {
		if(m_poPatsOrderInputPanel==NULL) return FALSE;
		return m_poPatsOrderInputPanel->GetAutoTrackLimitPrice();
	}
}

// �Ƿ���������
BOOL COrderInsertView::GetConditionSelect() 
{
	wxCheckBox* window=NULL;
	window=(wxCheckBox*)FindWindowById(ID_DOCONDITION_CHECK,this);
	return window->GetValue();
};

// �Ƿ�����ֹӯֹ��
BOOL COrderInsertView::GetPositionSelect() 
{
	wxCheckBox* window=NULL;
	window=(wxCheckBox*)FindWindowById(ID_DOSTOPPOSITION_CHECK,this);
	return window->GetValue();
};

void COrderInsertView::SetConditionSelect(BOOL bIsEdit) 
{
	wxCheckBox* pCheck = (wxCheckBox*)FindWindowById(ID_DOCONDITION_CHECK, this);
	if(pCheck==NULL) return;
	if(bIsEdit) {
		pCheck->Enable(false);
		pCheck->SetValue(1);
	}
	else {
		pCheck->Enable(true);
		pCheck->SetValue(0);
	}
};

void COrderInsertView::SetStopPositionSelect(BOOL bIsEdit) 
{
	wxCheckBox* pCheck = (wxCheckBox*)FindWindowById(ID_DOSTOPPOSITION_CHECK, this);
	if(pCheck==NULL) return;
	if(bIsEdit) {
		pCheck->Enable(false);
		pCheck->SetValue(1);
	}
	else {
		pCheck->Enable(true);
		pCheck->SetValue(0);
	}
};
