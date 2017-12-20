#pragma once
#include "stdafx.h"
/*#include "DockForm.h"*/
#include "BaseForm.h"
#include "TcpLayer.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "FinanceProductMgrForm.h"
#include "TradeStrategyMgrForm.h"
#include "CDataInfo.h"
#include "PrivilegeForm.h"
#include "BrokerAndGroupManage.h"
#include "TradeAccountMgrForm.h"
#include "SystemUserMgrForm.h"
#include "SettlementLoadDataForm.h"
#include "OrganizeMgrForm.h"
#include "HistorySettlementForm.h"
#include "FundForm.h"
#include "SetAutoSettleTime.h"
#include "FormQueryHistoryFund.h"
#include "FormQueryHistoryPosition.h"
#include "FormQueryHistoryPositionDetail.h"
#include "FormQueryHistoryOrder.h"
#include "FormQueryHistoryTrade.h"
#include "CFormSendMsgMultiUsers.h"
#include "CFormUserMsgQuery.h"
#include "CFormUserMsgShow.h"
#include "ReinitTimeOption.h"
#include "TradingTimeOption.h"
#include "QuotServerTimeReSetForm.h"
#include "SettlementConfirm.h"
#include "SimulateQuotServerSetupForm.h"

#include "CommonUserOpr.h"


namespace CltServerMgr {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace WeifenLuo::WinFormsUI::Docking;

	/// <summary>
	/// Summary for FormMain
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormMain : public System::Windows::Forms::Form
	{
	public:
		FormMain(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
 			formUserMsg = gcnew CFormUserMsgShow();
            m_pLogin = new sLoginRsp;
            m_pDataInfo = CDataInfo::NewInstance();
       }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormMain()
		{
			if (components)
			{
				delete components;
			}

            CDataInfo::DestroyInstance();
            if(m_pLogin)
            {
                delete m_pLogin;
                m_pLogin = NULL;
            }
		}

	private: CFormUserMsgShow^ formUserMsg;

	//public:DockForm^ dockForm; 
	private: BaseForm^ form1;
	private: BaseForm^ form2;
	private: BaseForm^ form3;
	private: BaseForm^ form4;
	private: BaseForm^ form5;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;


	private: System::Windows::Forms::ToolStripMenuItem^  ExitToolStripMenuItem;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: WeifenLuo::WinFormsUI::Docking::DockPanel^  dockPanel1;
    private: System::Windows::Forms::ToolStripButton^  toolStripButton1;
    private: System::Windows::Forms::ToolStripButton^  toolStripButton2;
    private: System::Windows::Forms::ToolStripButton^  toolStripButton3;
    private: System::Windows::Forms::ToolStripButton^  toolStripButton4;
    private: System::Windows::Forms::ToolStripButton^  toolStripButton5;
    private: System::Windows::Forms::ToolStripButton^  toolStripButton6;
    private: System::Windows::Forms::ToolStripMenuItem^  EToolStripMenuItem_User;
    private: System::Windows::Forms::ToolStripMenuItem^  AToolStripMenuItem_UserManage;
    private: System::Windows::Forms::ToolStripMenuItem^  SToolStripMenuItem_FundInOutManage;

    private: System::Windows::Forms::ToolStripMenuItem^  MToolStripMenuItem_Manage;
    private: System::Windows::Forms::ToolStripMenuItem^  PToolStripMenuItem_Privilege;
    private: System::Windows::Forms::ToolStripMenuItem^  SToolStripMenuItem_PrivilegeManage;













    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_ProductManage;

    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_System;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_Strategy;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_StrategyManage;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_Settle;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_SettleToday;








    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_BrokerManage;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_OrgManage;





    private: System::Windows::Forms::ToolStripMenuItem^  ����Ա���ɽ����ToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  �������ToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  �˻����ʹ���ToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  ���׺�Լ����ToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  ���̹���ToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  �Զ���������ToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  ��ʷ�ֶ�����ToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_SettleHistory;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_SettleTime;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_QueryHistoryData;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_QueryFund;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_QueryPosition;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_QueryPositionDetail;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_QueryTrade;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_QueryOrder;
	private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_UserMsg;
	private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_UserMsg_Query;
	private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_UserMsg_SendMsg;
private: System::Windows::Forms::ToolStripMenuItem^  ����ToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ��������ʼ��ToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ��ʱ����ʱ������ToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_TradeAccountManage;
private: System::ComponentModel::IContainer^  components;




	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


        sLoginRsp* m_pLogin;
private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_QuotServer_ReInit;

private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_QuotServer_TimeSetup;
private: System::Windows::Forms::Timer^  timer1;
private: System::Windows::Forms::ToolStripMenuItem^  ����ʱ�������ToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  �����Զ�����ToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ���׷�����ǿ�Ƴ�ʼ��ToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_SimulateQuotServer;
         CDataInfo* m_pDataInfo;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(FormMain::typeid));
			WeifenLuo::WinFormsUI::Docking::DockPanelSkin^  dockPanelSkin1 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanelSkin());
			WeifenLuo::WinFormsUI::Docking::AutoHideStripSkin^  autoHideStripSkin1 = (gcnew WeifenLuo::WinFormsUI::Docking::AutoHideStripSkin());
			WeifenLuo::WinFormsUI::Docking::DockPanelGradient^  dockPanelGradient1 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanelGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient1 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::DockPaneStripSkin^  dockPaneStripSkin1 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPaneStripSkin());
			WeifenLuo::WinFormsUI::Docking::DockPaneStripGradient^  dockPaneStripGradient1 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPaneStripGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient2 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::DockPanelGradient^  dockPanelGradient2 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanelGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient3 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::DockPaneStripToolWindowGradient^  dockPaneStripToolWindowGradient1 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPaneStripToolWindowGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient4 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient5 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::DockPanelGradient^  dockPanelGradient3 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanelGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient6 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			WeifenLuo::WinFormsUI::Docking::TabGradient^  tabGradient7 = (gcnew WeifenLuo::WinFormsUI::Docking::TabGradient());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->ToolStripMenuItem_System = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ExitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->EToolStripMenuItem_User = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->AToolStripMenuItem_UserManage = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->SToolStripMenuItem_FundInOutManage = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_TradeAccountManage = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->MToolStripMenuItem_Manage = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_ProductManage = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_BrokerManage = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_OrgManage = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_Strategy = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_StrategyManage = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->PToolStripMenuItem_Privilege = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->SToolStripMenuItem_PrivilegeManage = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_Settle = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_SettleToday = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_SettleHistory = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_SettleTime = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�����Զ�����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_QueryHistoryData = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_QueryFund = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_QueryPosition = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_QueryPositionDetail = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_QueryTrade = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_QueryOrder = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_UserMsg = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_UserMsg_Query = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_UserMsg_SendMsg = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��������ʼ��ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->���׷�����ǿ�Ƴ�ʼ��ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��ʱ����ʱ������ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_QuotServer_ReInit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_QuotServer_TimeSetup = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ʱ�������ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ToolStripMenuItem_SimulateQuotServer = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton3 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton4 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton5 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton6 = (gcnew System::Windows::Forms::ToolStripButton());
			this->dockPanel1 = (gcnew WeifenLuo::WinFormsUI::Docking::DockPanel());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->menuStrip1->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			this->toolStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(9) {this->ToolStripMenuItem_System, 
				this->EToolStripMenuItem_User, this->MToolStripMenuItem_Manage, this->ToolStripMenuItem_Strategy, this->PToolStripMenuItem_Privilege, 
				this->ToolStripMenuItem_Settle, this->ToolStripMenuItem_QueryHistoryData, this->ToolStripMenuItem_UserMsg, this->����ToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(963, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// ToolStripMenuItem_System
			// 
			this->ToolStripMenuItem_System->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->ExitToolStripMenuItem});
			this->ToolStripMenuItem_System->Name = L"ToolStripMenuItem_System";
			this->ToolStripMenuItem_System->Size = System::Drawing::Size(41, 20);
			this->ToolStripMenuItem_System->Text = L"ϵͳ";
			// 
			// ExitToolStripMenuItem
			// 
			this->ExitToolStripMenuItem->Name = L"ExitToolStripMenuItem";
			this->ExitToolStripMenuItem->Size = System::Drawing::Size(94, 22);
			this->ExitToolStripMenuItem->Text = L"�˳�";
			this->ExitToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::ExitToolStripMenuItem_Click);
			// 
			// EToolStripMenuItem_User
			// 
			this->EToolStripMenuItem_User->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->AToolStripMenuItem_UserManage, 
				this->SToolStripMenuItem_FundInOutManage, this->ToolStripMenuItem_TradeAccountManage});
			this->EToolStripMenuItem_User->Name = L"EToolStripMenuItem_User";
			this->EToolStripMenuItem_User->Size = System::Drawing::Size(41, 20);
			this->EToolStripMenuItem_User->Text = L"�˻�";
			// 
			// AToolStripMenuItem_UserManage
			// 
			this->AToolStripMenuItem_UserManage->Name = L"AToolStripMenuItem_UserManage";
			this->AToolStripMenuItem_UserManage->Size = System::Drawing::Size(166, 22);
			this->AToolStripMenuItem_UserManage->Text = L"ϵͳ�û�����";
			this->AToolStripMenuItem_UserManage->Click += gcnew System::EventHandler(this, &FormMain::AToolStripMenuItem_UserManage_Click);
			// 
			// SToolStripMenuItem_FundInOutManage
			// 
			this->SToolStripMenuItem_FundInOutManage->Name = L"SToolStripMenuItem_FundInOutManage";
			this->SToolStripMenuItem_FundInOutManage->Size = System::Drawing::Size(166, 22);
			this->SToolStripMenuItem_FundInOutManage->Text = L"����Ա��������";
			this->SToolStripMenuItem_FundInOutManage->Click += gcnew System::EventHandler(this, &FormMain::SToolStripMenuItem_FundInOutManage_Click);
			// 
			// ToolStripMenuItem_TradeAccountManage
			// 
			this->ToolStripMenuItem_TradeAccountManage->Name = L"ToolStripMenuItem_TradeAccountManage";
			this->ToolStripMenuItem_TradeAccountManage->Size = System::Drawing::Size(166, 22);
			this->ToolStripMenuItem_TradeAccountManage->Text = L"ί�н����˺Ź���";
			this->ToolStripMenuItem_TradeAccountManage->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_TradeAccountManage_Click);
			// 
			// MToolStripMenuItem_Manage
			// 
			this->MToolStripMenuItem_Manage->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->ToolStripMenuItem_ProductManage, 
				this->ToolStripMenuItem_BrokerManage, this->ToolStripMenuItem_OrgManage});
			this->MToolStripMenuItem_Manage->Name = L"MToolStripMenuItem_Manage";
			this->MToolStripMenuItem_Manage->Size = System::Drawing::Size(41, 20);
			this->MToolStripMenuItem_Manage->Text = L"����";
			// 
			// ToolStripMenuItem_ProductManage
			// 
			this->ToolStripMenuItem_ProductManage->Name = L"ToolStripMenuItem_ProductManage";
			this->ToolStripMenuItem_ProductManage->Size = System::Drawing::Size(190, 22);
			this->ToolStripMenuItem_ProductManage->Text = L"��Ʋ�Ʒ����";
			this->ToolStripMenuItem_ProductManage->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_ProductManage_Click);
			// 
			// ToolStripMenuItem_BrokerManage
			// 
			this->ToolStripMenuItem_BrokerManage->Name = L"ToolStripMenuItem_BrokerManage";
			this->ToolStripMenuItem_BrokerManage->Size = System::Drawing::Size(190, 22);
			this->ToolStripMenuItem_BrokerManage->Text = L"���͹�˾�����������";
			this->ToolStripMenuItem_BrokerManage->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_BrokerManage_Click);
			// 
			// ToolStripMenuItem_OrgManage
			// 
			this->ToolStripMenuItem_OrgManage->Name = L"ToolStripMenuItem_OrgManage";
			this->ToolStripMenuItem_OrgManage->Size = System::Drawing::Size(190, 22);
			this->ToolStripMenuItem_OrgManage->Text = L"��֯��������";
			this->ToolStripMenuItem_OrgManage->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_OrgManage_Click);
			// 
			// ToolStripMenuItem_Strategy
			// 
			this->ToolStripMenuItem_Strategy->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->ToolStripMenuItem_StrategyManage});
			this->ToolStripMenuItem_Strategy->Name = L"ToolStripMenuItem_Strategy";
			this->ToolStripMenuItem_Strategy->Size = System::Drawing::Size(41, 20);
			this->ToolStripMenuItem_Strategy->Text = L"����";
			// 
			// ToolStripMenuItem_StrategyManage
			// 
			this->ToolStripMenuItem_StrategyManage->Name = L"ToolStripMenuItem_StrategyManage";
			this->ToolStripMenuItem_StrategyManage->Size = System::Drawing::Size(142, 22);
			this->ToolStripMenuItem_StrategyManage->Text = L"���ײ��Թ���";
			this->ToolStripMenuItem_StrategyManage->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_StrategyManage_Click);
			// 
			// PToolStripMenuItem_Privilege
			// 
			this->PToolStripMenuItem_Privilege->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->SToolStripMenuItem_PrivilegeManage});
			this->PToolStripMenuItem_Privilege->Name = L"PToolStripMenuItem_Privilege";
			this->PToolStripMenuItem_Privilege->Size = System::Drawing::Size(41, 20);
			this->PToolStripMenuItem_Privilege->Text = L"Ȩ��";
			// 
			// SToolStripMenuItem_PrivilegeManage
			// 
			this->SToolStripMenuItem_PrivilegeManage->Name = L"SToolStripMenuItem_PrivilegeManage";
			this->SToolStripMenuItem_PrivilegeManage->Size = System::Drawing::Size(118, 22);
			this->SToolStripMenuItem_PrivilegeManage->Text = L"Ȩ�޹���";
			this->SToolStripMenuItem_PrivilegeManage->Click += gcnew System::EventHandler(this, &FormMain::SToolStripMenuItem_PrivilegeManage_Click);
			// 
			// ToolStripMenuItem_Settle
			// 
			this->ToolStripMenuItem_Settle->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->ToolStripMenuItem_SettleToday, 
				this->ToolStripMenuItem_SettleHistory, this->ToolStripMenuItem_SettleTime, this->�����Զ�����ToolStripMenuItem});
			this->ToolStripMenuItem_Settle->Name = L"ToolStripMenuItem_Settle";
			this->ToolStripMenuItem_Settle->Size = System::Drawing::Size(41, 20);
			this->ToolStripMenuItem_Settle->Text = L"����";
			// 
			// ToolStripMenuItem_SettleToday
			// 
			this->ToolStripMenuItem_SettleToday->Name = L"ToolStripMenuItem_SettleToday";
			this->ToolStripMenuItem_SettleToday->Size = System::Drawing::Size(166, 22);
			this->ToolStripMenuItem_SettleToday->Text = L"�������ݽ���";
			this->ToolStripMenuItem_SettleToday->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_SettleToday_Click);
			// 
			// ToolStripMenuItem_SettleHistory
			// 
			this->ToolStripMenuItem_SettleHistory->Name = L"ToolStripMenuItem_SettleHistory";
			this->ToolStripMenuItem_SettleHistory->Size = System::Drawing::Size(166, 22);
			this->ToolStripMenuItem_SettleHistory->Text = L"��ʷ����";
			this->ToolStripMenuItem_SettleHistory->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_SettleHistory_Click);
			// 
			// ToolStripMenuItem_SettleTime
			// 
			this->ToolStripMenuItem_SettleTime->Name = L"ToolStripMenuItem_SettleTime";
			this->ToolStripMenuItem_SettleTime->Size = System::Drawing::Size(166, 22);
			this->ToolStripMenuItem_SettleTime->Text = L"�����Զ�����ʱ��";
			this->ToolStripMenuItem_SettleTime->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_SettleTime_Click);
			// 
			// �����Զ�����ToolStripMenuItem
			// 
			this->�����Զ�����ToolStripMenuItem->Name = L"�����Զ�����ToolStripMenuItem";
			this->�����Զ�����ToolStripMenuItem->Size = System::Drawing::Size(166, 22);
			this->�����Զ�����ToolStripMenuItem->Text = L"�����Զ�����";
			this->�����Զ�����ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::�����Զ�����ToolStripMenuItem_Click);
			// 
			// ToolStripMenuItem_QueryHistoryData
			// 
			this->ToolStripMenuItem_QueryHistoryData->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->ToolStripMenuItem_QueryFund, 
				this->ToolStripMenuItem_QueryPosition, this->ToolStripMenuItem_QueryPositionDetail, this->ToolStripMenuItem_QueryTrade, this->ToolStripMenuItem_QueryOrder});
			this->ToolStripMenuItem_QueryHistoryData->Name = L"ToolStripMenuItem_QueryHistoryData";
			this->ToolStripMenuItem_QueryHistoryData->Size = System::Drawing::Size(89, 20);
			this->ToolStripMenuItem_QueryHistoryData->Text = L"��ʷ���ݲ�ѯ";
			// 
			// ToolStripMenuItem_QueryFund
			// 
			this->ToolStripMenuItem_QueryFund->Name = L"ToolStripMenuItem_QueryFund";
			this->ToolStripMenuItem_QueryFund->Size = System::Drawing::Size(166, 22);
			this->ToolStripMenuItem_QueryFund->Text = L"��ѯ��ʷ�ʽ�";
			this->ToolStripMenuItem_QueryFund->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_QueryFund_Click);
			// 
			// ToolStripMenuItem_QueryPosition
			// 
			this->ToolStripMenuItem_QueryPosition->Name = L"ToolStripMenuItem_QueryPosition";
			this->ToolStripMenuItem_QueryPosition->Size = System::Drawing::Size(166, 22);
			this->ToolStripMenuItem_QueryPosition->Text = L"��ѯ��ʷ�ֲ�";
			this->ToolStripMenuItem_QueryPosition->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_QueryPosition_Click);
			// 
			// ToolStripMenuItem_QueryPositionDetail
			// 
			this->ToolStripMenuItem_QueryPositionDetail->Name = L"ToolStripMenuItem_QueryPositionDetail";
			this->ToolStripMenuItem_QueryPositionDetail->Size = System::Drawing::Size(166, 22);
			this->ToolStripMenuItem_QueryPositionDetail->Text = L"��ѯ��ʷ�ֲ���ϸ";
			this->ToolStripMenuItem_QueryPositionDetail->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_QueryPositionDetail_Click);
			// 
			// ToolStripMenuItem_QueryTrade
			// 
			this->ToolStripMenuItem_QueryTrade->Name = L"ToolStripMenuItem_QueryTrade";
			this->ToolStripMenuItem_QueryTrade->Size = System::Drawing::Size(166, 22);
			this->ToolStripMenuItem_QueryTrade->Text = L"��ѯ��ʷ�ɽ�";
			this->ToolStripMenuItem_QueryTrade->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_QueryTrade_Click);
			// 
			// ToolStripMenuItem_QueryOrder
			// 
			this->ToolStripMenuItem_QueryOrder->Name = L"ToolStripMenuItem_QueryOrder";
			this->ToolStripMenuItem_QueryOrder->Size = System::Drawing::Size(166, 22);
			this->ToolStripMenuItem_QueryOrder->Text = L"��ѯ��ʷ����";
			this->ToolStripMenuItem_QueryOrder->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_QueryOrder_Click);
			// 
			// ToolStripMenuItem_UserMsg
			// 
			this->ToolStripMenuItem_UserMsg->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->ToolStripMenuItem_UserMsg_Query, 
				this->ToolStripMenuItem_UserMsg_SendMsg});
			this->ToolStripMenuItem_UserMsg->Name = L"ToolStripMenuItem_UserMsg";
			this->ToolStripMenuItem_UserMsg->Size = System::Drawing::Size(65, 20);
			this->ToolStripMenuItem_UserMsg->Text = L"�û���Ϣ";
			// 
			// ToolStripMenuItem_UserMsg_Query
			// 
			this->ToolStripMenuItem_UserMsg_Query->Name = L"ToolStripMenuItem_UserMsg_Query";
			this->ToolStripMenuItem_UserMsg_Query->Size = System::Drawing::Size(118, 22);
			this->ToolStripMenuItem_UserMsg_Query->Text = L"��Ϣ��ѯ";
			this->ToolStripMenuItem_UserMsg_Query->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_UserMsg_Query_Click);
			// 
			// ToolStripMenuItem_UserMsg_SendMsg
			// 
			this->ToolStripMenuItem_UserMsg_SendMsg->Name = L"ToolStripMenuItem_UserMsg_SendMsg";
			this->ToolStripMenuItem_UserMsg_SendMsg->Size = System::Drawing::Size(118, 22);
			this->ToolStripMenuItem_UserMsg_SendMsg->Text = L"��Ϣ����";
			this->ToolStripMenuItem_UserMsg_SendMsg->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_UserMsg_SendMsg_Click);
			// 
			// ����ToolStripMenuItem
			// 
			this->����ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->��������ʼ��ToolStripMenuItem, 
				this->���׷�����ǿ�Ƴ�ʼ��ToolStripMenuItem, this->��ʱ����ʱ������ToolStripMenuItem, this->ToolStripMenuItem_QuotServer_ReInit, this->ToolStripMenuItem_QuotServer_TimeSetup, 
				this->����ʱ�������ToolStripMenuItem, this->ToolStripMenuItem_SimulateQuotServer});
			this->����ToolStripMenuItem->Name = L"����ToolStripMenuItem";
			this->����ToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->����ToolStripMenuItem->Text = L"����";
			// 
			// ��������ʼ��ToolStripMenuItem
			// 
			this->��������ʼ��ToolStripMenuItem->Name = L"��������ʼ��ToolStripMenuItem";
			this->��������ʼ��ToolStripMenuItem->Size = System::Drawing::Size(238, 22);
			this->��������ʼ��ToolStripMenuItem->Text = L"���׷�������ʼ��";
			this->��������ʼ��ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::��������ʼ��ToolStripMenuItem_Click);
			// 
			// ���׷�����ǿ�Ƴ�ʼ��ToolStripMenuItem
			// 
			this->���׷�����ǿ�Ƴ�ʼ��ToolStripMenuItem->Name = L"���׷�����ǿ�Ƴ�ʼ��ToolStripMenuItem";
			this->���׷�����ǿ�Ƴ�ʼ��ToolStripMenuItem->Size = System::Drawing::Size(238, 22);
			this->���׷�����ǿ�Ƴ�ʼ��ToolStripMenuItem->Text = L"���׷�����ǿ�Ƴ�ʼ��";
			this->���׷�����ǿ�Ƴ�ʼ��ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::���׷�����ǿ�Ƴ�ʼ��ToolStripMenuItem_Click);
			// 
			// ��ʱ����ʱ������ToolStripMenuItem
			// 
			this->��ʱ����ʱ������ToolStripMenuItem->Name = L"��ʱ����ʱ������ToolStripMenuItem";
			this->��ʱ����ʱ������ToolStripMenuItem->Size = System::Drawing::Size(238, 22);
			this->��ʱ����ʱ������ToolStripMenuItem->Text = L"���׷�������ʱ��ʼ��ʱ������";
			this->��ʱ����ʱ������ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::��ʱ����ʱ������ToolStripMenuItem_Click);
			// 
			// ToolStripMenuItem_QuotServer_ReInit
			// 
			this->ToolStripMenuItem_QuotServer_ReInit->Name = L"ToolStripMenuItem_QuotServer_ReInit";
			this->ToolStripMenuItem_QuotServer_ReInit->Size = System::Drawing::Size(238, 22);
			this->ToolStripMenuItem_QuotServer_ReInit->Text = L"�����������ʼ��";
			this->ToolStripMenuItem_QuotServer_ReInit->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_QuotServer_ReInit_Click);
			// 
			// ToolStripMenuItem_QuotServer_TimeSetup
			// 
			this->ToolStripMenuItem_QuotServer_TimeSetup->Name = L"ToolStripMenuItem_QuotServer_TimeSetup";
			this->ToolStripMenuItem_QuotServer_TimeSetup->Size = System::Drawing::Size(238, 22);
			this->ToolStripMenuItem_QuotServer_TimeSetup->Text = L"�����������ʱ����ʱ������";
			this->ToolStripMenuItem_QuotServer_TimeSetup->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_QuotServer_TimeSetup_Click);
			// 
			// ����ʱ�������ToolStripMenuItem
			// 
			this->����ʱ�������ToolStripMenuItem->Name = L"����ʱ�������ToolStripMenuItem";
			this->����ʱ�������ToolStripMenuItem->Size = System::Drawing::Size(238, 22);
			this->����ʱ�������ToolStripMenuItem->Text = L"����ʱ�������";
			this->����ʱ�������ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::����ʱ�������ToolStripMenuItem_Click);
			// 
			// ToolStripMenuItem_SimulateQuotServer
			// 
			this->ToolStripMenuItem_SimulateQuotServer->Name = L"ToolStripMenuItem_SimulateQuotServer";
			this->ToolStripMenuItem_SimulateQuotServer->Size = System::Drawing::Size(238, 22);
			this->ToolStripMenuItem_SimulateQuotServer->Text = L"ģ���������������";
			this->ToolStripMenuItem_SimulateQuotServer->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_SimulateQuotServer_Click);
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripStatusLabel1});
			this->statusStrip1->Location = System::Drawing::Point(0, 577);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(963, 22);
			this->statusStrip1->TabIndex = 1;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(41, 17);
			this->toolStripStatusLabel1->Text = L"�ѵ�¼";
			// 
			// toolStrip1
			// 
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->toolStripButton1, 
				this->toolStripButton2, this->toolStripButton3, this->toolStripButton4, this->toolStripButton5, this->toolStripButton6});
			this->toolStrip1->Location = System::Drawing::Point(0, 24);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(963, 25);
			this->toolStrip1->TabIndex = 2;
			this->toolStrip1->Text = L"toolStrip1";
			this->toolStrip1->Visible = false;
			// 
			// toolStripButton1
			// 
			this->toolStripButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton1.Image")));
			this->toolStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton1->Name = L"toolStripButton1";
			this->toolStripButton1->Size = System::Drawing::Size(23, 22);
			this->toolStripButton1->Text = L"���";
			// 
			// toolStripButton2
			// 
			this->toolStripButton2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton2.Image")));
			this->toolStripButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton2->Name = L"toolStripButton2";
			this->toolStripButton2->Size = System::Drawing::Size(23, 22);
			this->toolStripButton2->Text = L"�޸�";
			// 
			// toolStripButton3
			// 
			this->toolStripButton3->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton3->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton3.Image")));
			this->toolStripButton3->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton3->Name = L"toolStripButton3";
			this->toolStripButton3->Size = System::Drawing::Size(23, 22);
			this->toolStripButton3->Text = L"ɾ��";
			// 
			// toolStripButton4
			// 
			this->toolStripButton4->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton4->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton4.Image")));
			this->toolStripButton4->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton4->Name = L"toolStripButton4";
			this->toolStripButton4->Size = System::Drawing::Size(23, 22);
			this->toolStripButton4->Text = L"����/ͣ��";
			// 
			// toolStripButton5
			// 
			this->toolStripButton5->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton5->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton5.Image")));
			this->toolStripButton5->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton5->Name = L"toolStripButton5";
			this->toolStripButton5->Size = System::Drawing::Size(23, 22);
			this->toolStripButton5->Text = L"����ָ������";
			// 
			// toolStripButton6
			// 
			this->toolStripButton6->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton6->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton6.Image")));
			this->toolStripButton6->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton6->Name = L"toolStripButton6";
			this->toolStripButton6->Size = System::Drawing::Size(23, 22);
			this->toolStripButton6->Text = L"ˢ��";
			// 
			// dockPanel1
			// 
			this->dockPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dockPanel1->DockRightPortion = 0.75;
			this->dockPanel1->Location = System::Drawing::Point(0, 0);
			this->dockPanel1->Name = L"dockPanel1";
			this->dockPanel1->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->dockPanel1->RightToLeftLayout = true;
			this->dockPanel1->Size = System::Drawing::Size(963, 599);
			dockPanelGradient1->EndColor = System::Drawing::SystemColors::ControlLight;
			dockPanelGradient1->StartColor = System::Drawing::SystemColors::ControlLight;
			autoHideStripSkin1->DockStripGradient = dockPanelGradient1;
			tabGradient1->EndColor = System::Drawing::SystemColors::Control;
			tabGradient1->StartColor = System::Drawing::SystemColors::Control;
			tabGradient1->TextColor = System::Drawing::SystemColors::ControlDarkDark;
			autoHideStripSkin1->TabGradient = tabGradient1;
			autoHideStripSkin1->TextFont = (gcnew System::Drawing::Font(L"SimSun", 9));
			dockPanelSkin1->AutoHideStripSkin = autoHideStripSkin1;
			tabGradient2->EndColor = System::Drawing::SystemColors::ControlLightLight;
			tabGradient2->StartColor = System::Drawing::SystemColors::ControlLightLight;
			tabGradient2->TextColor = System::Drawing::SystemColors::ControlText;
			dockPaneStripGradient1->ActiveTabGradient = tabGradient2;
			dockPanelGradient2->EndColor = System::Drawing::SystemColors::Control;
			dockPanelGradient2->StartColor = System::Drawing::SystemColors::Control;
			dockPaneStripGradient1->DockStripGradient = dockPanelGradient2;
			tabGradient3->EndColor = System::Drawing::SystemColors::ControlLight;
			tabGradient3->StartColor = System::Drawing::SystemColors::ControlLight;
			tabGradient3->TextColor = System::Drawing::SystemColors::ControlText;
			dockPaneStripGradient1->InactiveTabGradient = tabGradient3;
			dockPaneStripSkin1->DocumentGradient = dockPaneStripGradient1;
			dockPaneStripSkin1->TextFont = (gcnew System::Drawing::Font(L"SimSun", 9));
			tabGradient4->EndColor = System::Drawing::SystemColors::ActiveCaption;
			tabGradient4->LinearGradientMode = System::Drawing::Drawing2D::LinearGradientMode::Vertical;
			tabGradient4->StartColor = System::Drawing::SystemColors::GradientActiveCaption;
			tabGradient4->TextColor = System::Drawing::SystemColors::ActiveCaptionText;
			dockPaneStripToolWindowGradient1->ActiveCaptionGradient = tabGradient4;
			tabGradient5->EndColor = System::Drawing::SystemColors::Control;
			tabGradient5->StartColor = System::Drawing::SystemColors::Control;
			tabGradient5->TextColor = System::Drawing::SystemColors::ControlText;
			dockPaneStripToolWindowGradient1->ActiveTabGradient = tabGradient5;
			dockPanelGradient3->EndColor = System::Drawing::SystemColors::ControlLight;
			dockPanelGradient3->StartColor = System::Drawing::SystemColors::ControlLight;
			dockPaneStripToolWindowGradient1->DockStripGradient = dockPanelGradient3;
			tabGradient6->EndColor = System::Drawing::SystemColors::InactiveCaption;
			tabGradient6->LinearGradientMode = System::Drawing::Drawing2D::LinearGradientMode::Vertical;
			tabGradient6->StartColor = System::Drawing::SystemColors::GradientInactiveCaption;
			tabGradient6->TextColor = System::Drawing::SystemColors::InactiveCaptionText;
			dockPaneStripToolWindowGradient1->InactiveCaptionGradient = tabGradient6;
			tabGradient7->EndColor = System::Drawing::Color::Transparent;
			tabGradient7->StartColor = System::Drawing::Color::Transparent;
			tabGradient7->TextColor = System::Drawing::SystemColors::ControlDarkDark;
			dockPaneStripToolWindowGradient1->InactiveTabGradient = tabGradient7;
			dockPaneStripSkin1->ToolWindowGradient = dockPaneStripToolWindowGradient1;
			dockPanelSkin1->DockPaneStripSkin = dockPaneStripSkin1;
			this->dockPanel1->Skin = dockPanelSkin1;
			this->dockPanel1->TabIndex = 0;
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1000;
			this->timer1->Tick += gcnew System::EventHandler(this, &FormMain::timer1_Tick);
			// 
			// FormMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(963, 599);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->menuStrip1);
			this->Controls->Add(this->dockPanel1);
			this->IsMdiContainer = true;
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"FormMain";
			this->Text = L"�����ն�";
			this->Load += gcnew System::EventHandler(this, &FormMain::FormMain_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FormMain::FormMain_FormClosed);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormMain::FormMain_FormClosing);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	protected:virtual void WndProc( Message% m ) override
		  {
              LOG_INFO("Enter WndProc");
			  if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_DataArrival )
			  {
                  //LOG_INFO("m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_DataArrival");
				  CTcpLayer::ParseRecvPackageInWindowThread();						
			  }
              else if( m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_ConnStatus )
              {
                   //LOG_INFO("m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_ConnStatus");
                   int nConnectStatus = m.LParam.ToInt32();
                   //  0/1/2/3�ֱ��ʾ���ӶϿ�/���ӿ�ʼ/���ӳɹ�/����ʧ��
                   switch(nConnectStatus)
                   {
                   case 0:
                       toolStripStatusLabel1->Text = "��������Ͽ�����,�����µ�¼";
                       break;
                   case 1:
                       //toolStripStatusLabel1->Text = "�������������...";
                       break;
                   case 2:
                       toolStripStatusLabel1->Text = "����������ӳɹ�";
                       break;
                   case 3:
                       //toolStripStatusLabel1->Text = "�����������ʧ��,�����µ�¼";
                       break;
                   }
              }
              else if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival )
              {
                  //LOG_INFO("m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival");
                  char *p = (char*)m.LParam.ToInt32();
                  Stru_UniPkgHead head;					
                  memset(&head, 0, sizeof(head));
                  memcpy(&head, p, sizeof(head));

                  String^ strError = String::Empty;
                  bool bShowMsg = true;

                  switch ( head.cmdid )
                  {
                  case Cmd_UserMsg_PushMsg: 
                      bShowMsg = false;
                      if ( head.userdata1 == CF_ERROR_SUCCESS )
                      {
                          int nCount = head.len / sizeof(UserMsgStru);
                          for ( int i = 0; i < nCount; i++ )
                          {
                              UserMsgStru* pMsg = (UserMsgStru*)(p+sizeof(Stru_UniPkgHead) + i*sizeof(UserMsgStru));
                              //UserMsgMemStru* pMsg = (UserMsgMemStru*)(p+sizeof(Stru_UniPkgHead) + i*sizeof(UserMsgMemStru));
                              formUserMsg->AppendUserMsg(*pMsg, this);
                          }
                      }
					  break;
				  case Cmd_TradeDataManager_ReInit_Rsp:
					  if(head.userdata3 == CF_ERROR_SUCCESS)
					  {						
                          timer1->Start();
						  strError = "�������³�ʼ������������ɹ��������ĵȴ���������ʼ��";
					  }
					  else
					  {
						  strError = "�������³�ʼ������������ʧ�ܣ�ʧ��ԭ���ǣ�"+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));;
					  }
					  break;
				  case Cmd_TradeDataManager_ReForceInit_Rsp:
					  if(head.userdata3 == CF_ERROR_SUCCESS)
					  {						
						  timer1->Start();
						  strError = "��������ǿ�Ƴ�ʼ������������ɹ��������ĵȴ���������ʼ��";
					  }
					  else
					  {
						  strError = "��������ǿ�Ƴ�ʼ������������ʧ�ܣ�ʧ��ԭ���ǣ�"+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));;
					  }
					  break;
                  case Cmd_ReInitQuotServerEvent_Rsp:
                      if(head.userdata1 == CF_ERROR_SUCCESS)
                      {		
                          strError = "����������������������ɹ�";
                      }
                      else
                      {
                          strError = "���������������������ʧ�ܣ�ʧ��ԭ���ǣ�"+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));;
                      }
                      break;
                  case Cmd_QuotServerTimeResetEvent_Rsp:
                      if(head.userdata1 == CF_ERROR_SUCCESS)
                      {		
                          strError = "�����������������ʱ������ɹ�";
                      }
                      else
                      {
                          strError = "�����������������ʱ�����ʧ�ܣ�ʧ��ԭ���ǣ�"+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));;
                      }
                      break;
                  case Cmd_QryRole_Rsp:
                      {
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              m_pDataInfo->ClearRole();

                              std::map<int, RoleInfo> mapRoleInfo;
                              int nCount = head.len / sizeof(RoleInfo);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  RoleInfo* pRoInfo = (RoleInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(RoleInfo));
                                  mapRoleInfo[pRoInfo->nRoleID] = *pRoInfo;
                              }

                              m_pDataInfo->SetRole(mapRoleInfo);
                              m_pDataInfo->m_bRoleLoad = true;
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      }
                  case Cmd_QryPrivilege_Rsp:
                      {
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              std::map<int, Privilege> mapPrivilege;
                              std::map<int,std::vector<Privilege>> mapUserType2Privilege;
                              std::map<int,std::vector<Privilege>> mapParentID2Privilege;

                              int nCount = head.len / sizeof(Privilege);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  Privilege* pPrivilege = (Privilege*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(Privilege));
                                  mapPrivilege[pPrivilege->nPrivilegeID] = *pPrivilege;

                                  if ( pPrivilege->nUserTypeID != -1 )
                                      mapUserType2Privilege[pPrivilege->nUserTypeID].push_back(*pPrivilege);
                                  if ( pPrivilege->nParentID != -1 )
                                      mapParentID2Privilege[pPrivilege->nParentID].push_back(*pPrivilege);
                              }

                              m_pDataInfo->SetPrivilege(mapPrivilege);
                              m_pDataInfo->SetUserType2Privilege(mapUserType2Privilege);
                              m_pDataInfo->SetParentID2Privilege(mapParentID2Privilege);

                              m_pDataInfo->m_bPrivilegeLoad = true;
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      }
                  case Cmd_QueryRolePrivilegeRelation_Rsp:
                      {
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              m_pDataInfo->ClearRolePrivilegeRelation();                           

                              std::map<int, std::vector<int>> mapRole2Privilege;
                              int nCount = head.len / sizeof(RoleAndPrivilegeRelation);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  RoleAndPrivilegeRelation* pRelation = (RoleAndPrivilegeRelation*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(RoleAndPrivilegeRelation));
                                  mapRole2Privilege[pRelation->nRoleID].push_back(pRelation->nPrivilegeID);
                              }

                              m_pDataInfo->SetRolePrivilegeRelation(mapRole2Privilege);

                              m_pDataInfo->m_bRolePrivilegeRelationLoad = true;
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      }
                  case Cmd_QryUserRoleRelation_Rsp:
                      {            
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              m_pDataInfo->ClearUserRoleRelation();

                              std::map<int, std::vector<UserRole>> mapUserRole;
                              int nCount = head.len / sizeof(UserRole);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  UserRole* pUserRole = (UserRole*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(UserRole));
                                  mapUserRole[pUserRole->nUserID].push_back(*pUserRole);
                              }

                              m_pDataInfo->SetUserRoleRelation(mapUserRole);

                              m_pDataInfo->m_bUserRoleRelationLoad = true;

                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }
                          break;
                      }
                  case Cmd_TradeDataManager_QryStatus_Rsp:
                      {
                          LOG_INFO("Cmd_TradeDataManager_QryStatus_Rsp start");
                          bShowMsg = false;
                          if ( head.userdata3 == CF_ERROR_SUCCESS )
                          {
                              eTradeDataStatus tradeDataStatus = *(eTradeDataStatus*)(p+sizeof(Stru_UniPkgHead));
                              m_pDataInfo->SetTradeDataStatus(tradeDataStatus);

                              switch(tradeDataStatus)
                              {                              
                              case TradeDataStatusInitialing:
                                  toolStripStatusLabel1->Text = "�����������ڳ�ʼ��״̬";
                                  break;
                              case TradeDataStatusLastDaySettlemtNotOK:
                                  toolStripStatusLabel1->Text = "�������������д����㽻����״̬";
                                  break;
                              case TradeDataStatusNoTradingDay:
                                  toolStripStatusLabel1->Text = "�������������޽�����״̬";
                                  break;
                              case TradeDataStatusTrading:
                                  toolStripStatusLabel1->Text = "�����������ڽ���״̬ ";
                                  break;
                              case TradeDataStatusSettlemtIniting:
                                  toolStripStatusLabel1->Text = "�����������ڽ����ʼ��״̬";
                                  break;
                              case TradeDataStatusSettlemting:
                                  toolStripStatusLabel1->Text = "�����������ڽ���״̬";
                                  break;
                              case TradeDataStatusSettlemtEnd:
                                  toolStripStatusLabel1->Text = "�����������ڽ������״̬";
                                  break;
                              case TradeDataStatusInitFail:
                                  toolStripStatusLabel1->Text = "��������ʼ��ʧ��";
                                  break;
                              }
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          LOG_INFO("Cmd_TradeDataManager_QryStatus_Rsp finished");
                          break;
                      }
                  case CMDID_GetALLError_Push:
                      {     
                          bShowMsg = false;
                          if ( head.userdata3 == CF_ERROR_SUCCESS )
                          {
                              std::vector<sErrorInfo> lvecError;
                              int nCount = head.len / sizeof(sErrorInfo);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  sErrorInfo* pErrorInfo = (sErrorInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(sErrorInfo));
                                  switch (pErrorInfo->meErrorType)
                                  {
                                  case FirstLevelError:
                                      strError = "1������Ӱ��ϵͳ���еĴ���"+Tools::string2String(pErrorInfo->msError);
                                      break;
                                  case SecondLevelError:
                                      strError = "2�����󣬲�Ӱ��ϵͳ���еĴ���"+Tools::string2String(pErrorInfo->msError);
                                      break;
                                  case ThirdLevelError:
                                      strError = "3�����󣬲�Ӱ��ϵͳ����"+Tools::string2String(pErrorInfo->msError);
                                      break;
								  case Logger:
									  strError = "ϵͳ��������"+Tools::string2String(pErrorInfo->msError);
									  break;
                                  case OperationLogNeedSend:
                                      strError = Tools::string2String(pErrorInfo->msError);
                                      break;
                                  }  

                                  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
                                      CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
                                      Tools::String2string(strError).c_str());
                              }
                          }

                          break;
                      }
                  }

                  if(bShowMsg)
                  {
                      LOG_INFO("_GetPopMsg()->AppendStr");
						_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
								CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
                                Tools::String2string(strError).c_str());
                  }
              }
			  Form::WndProc ( m );
              LOG_INFO("Leave WndProc");
		  }
	bool SubsUserMsg(int nUserID)
	{
		return CTcpLayer::SendData(Cmd_UserMsg_SubsMsg_Req, NULL, 0, 0, nUserID);
	}
		//�˳�
	private: System::Void ExitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		{
            LOG_INFO("Զ�̷�������ַ<%s> �û�<%d> ����<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_SYSTEM_EXIT));
			Close();
		}
private: System::Void toolStripContainer1_LeftToolStripPanel_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void splitContainer1_Panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		 }
private: System::Void FormMain_Load(System::Object^  sender, System::EventArgs^  e) {
			 //dockForm = gcnew DockForm();
			 //dockForm->Show(dockPanel1,DockState::DockLeft);

			 int nHwnd = this->Handle.ToInt32();
			 CTcpLayer::SetMainWndHandle(nHwnd);

// 			 form1 = gcnew BaseForm();
// 			 form1->TabText = L"ί�н����˺�";
// 			 form1->Show(dockPanel1);
// 
// 			 form2 = gcnew BaseForm();
// 			 form2->TabText = L"����Ա";
// 			 form2->Show(dockPanel1);
// 
// 			 form3 = gcnew BaseForm();
// 			 form3->TabText = L"����Ա";
// 			 form3->Show(dockPanel1);
// 
// 			 form4 = gcnew BaseForm();
// 			 form4->TabText = L"���Ա";
// 			 form4->Show(dockPanel1);
// 
// 			 form5 = gcnew BaseForm();
// 			 form5->TabText = L"���׼��Ա";
// 			 form5->Show(dockPanel1);
// 
// 			 form1->Activate();

			 this->CenterToParent();

             toolStrip1->Visible = false;
             dockPanel1->Visible = false;             
             
             //����������Ϣ 
             CTcpLayer::SubscribePkg(CMDID_GetALLError_Push, nHwnd);
			 CTcpLayer::SubscribePkg(Cmd_UserMsg_PushMsg, nHwnd);
			 CTcpLayer::SubscribePkg(Cmd_TradeDataManager_ReInit_Rsp, nHwnd);
			 CTcpLayer::SubscribePkg(Cmd_TradeDataManager_ReForceInit_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_ReInitQuotServerEvent_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_QuotServerTimeResetEvent_Rsp, nHwnd);


			 CDataInfo::NewInstance()->GetLogonInfo(*m_pLogin);
			 SubsUserMsg(m_pLogin->mnUserID);

             CTcpLayer::SubscribePkg(Cmd_QryRole_Rsp, nHwnd);
             CTcpLayer::SendData(Cmd_QryRole_Req, NULL,0, 0, m_pLogin->mnUserID);

             CTcpLayer::SubscribePkg(Cmd_QryPrivilege_Rsp, nHwnd);
             CTcpLayer::SendData(Cmd_QryPrivilege_Req, NULL,0, 0, m_pLogin->mnUserID);

             CTcpLayer::SubscribePkg(Cmd_QueryRolePrivilegeRelation_Rsp, nHwnd);
             CTcpLayer::SendData(Cmd_QueryRolePrivilegeRelation_Req, NULL,0, 0, m_pLogin->mnUserID);

             CTcpLayer::SubscribePkg(Cmd_QryUserRoleRelation_Rsp, nHwnd);
             CTcpLayer::SendData(Cmd_QryUserRoleRelation_Req, NULL,0, 0, m_pLogin->mnUserID);

             CTcpLayer::SubscribePkg(Cmd_TradeDataManager_QryStatus_Rsp, nHwnd);


             UserInfo userInfo;
             m_pDataInfo->GetUserInfoByUserID(m_pLogin->mnUserID,userInfo);
             this->Text = L"�����ն�-" + Tools::string2String(userInfo.szAccount);

             timer1->Start();
		 }
private: System::Void ToolStripMenuItem_ProductManage_Click(System::Object^  sender, System::EventArgs^  e) {
             FinanceProductMgrForm^ dlg = gcnew FinanceProductMgrForm();
             dlg->ShowDialog();
         }
private: System::Void ToolStripMenuItem_StrategyManage_Click(System::Object^  sender, System::EventArgs^  e) {
             TradeStrategyMgrForm^ dlg = gcnew TradeStrategyMgrForm();
             dlg->ShowDialog();
         }
private: System::Void SToolStripMenuItem_PrivilegeManage_Click(System::Object^  sender, System::EventArgs^  e) {
			 PrivilegeForm^ dlg = gcnew PrivilegeForm();
			 dlg->ShowDialog();
		 }
private: System::Void ToolStripMenuItem_BrokerManage_Click(System::Object^  sender, System::EventArgs^  e) {
			 BrokerAndGroupManage^ dlg = gcnew BrokerAndGroupManage();
			 dlg->ShowDialog();
		 }
private: System::Void AToolStripMenuItem_UserManage_Click(System::Object^  sender, System::EventArgs^  e) {
             SystemUserMgrForm^ dlg = gcnew SystemUserMgrForm();
             dlg->ShowDialog();
         }
private: System::Void ToolStripMenuItem_SettleToday_Click(System::Object^  sender, System::EventArgs^  e) {
			 SettlementLoadDataForm^ dlg = gcnew SettlementLoadDataForm();
			 dlg->ShowDialog();
		 }
private: System::Void ToolStripMenuItem_OrgManage_Click(System::Object^  sender, System::EventArgs^  e) {
             OrganizeMgrForm^ dlg = gcnew OrganizeMgrForm();
             dlg->ShowDialog();
         }
private: System::Void ToolStripMenuItem_TradeAccountManage_Click(System::Object^  sender, System::EventArgs^  e) {
             TradeAccountMgrForm^ dlg = gcnew TradeAccountMgrForm();
             dlg->ShowDialog();
         }
private: System::Void ToolStripMenuItem_SettleHistory_Click(System::Object^  sender, System::EventArgs^  e) {
			 HistorySettlementForm^ dlg = gcnew HistorySettlementForm();
			 dlg->ShowDialog();
		 }
private: System::Void SToolStripMenuItem_FundInOutManage_Click(System::Object^  sender, System::EventArgs^  e) {
             FundForm^ dlg = gcnew FundForm();
             dlg->ShowDialog();
         }
private: System::Void ToolStripMenuItem_SettleTime_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 SetAutoSettleTime^ dlg =gcnew SetAutoSettleTime();
			 dlg->ShowDialog();
		 }
private: System::Void ToolStripMenuItem_QueryFund_Click(System::Object^  sender, System::EventArgs^  e) {
			 FormQueryHistoryFund^ dlg = gcnew FormQueryHistoryFund();
			 dlg->ShowDialog();
		 }
private: System::Void ToolStripMenuItem_QueryPosition_Click(System::Object^  sender, System::EventArgs^  e) {
			 FormQueryHistoryPosition^ dlg = gcnew FormQueryHistoryPosition();
			 dlg->ShowDialog();
		 }
private: System::Void ToolStripMenuItem_QueryPositionDetail_Click(System::Object^  sender, System::EventArgs^  e) {
			 FormQueryHistoryPositionDetail^ dlg = gcnew FormQueryHistoryPositionDetail();
			 dlg->ShowDialog();
		 }
private: System::Void ToolStripMenuItem_QueryTrade_Click(System::Object^  sender, System::EventArgs^  e) {
			 FormQueryHistoryTrade^ dlg = gcnew FormQueryHistoryTrade();
			 dlg->ShowDialog();
		 }
private: System::Void ToolStripMenuItem_QueryOrder_Click(System::Object^  sender, System::EventArgs^  e) {
			 FormQueryHistoryOrder^ dlg = gcnew FormQueryHistoryOrder();
			 dlg->ShowDialog();
		 }
private: System::Void ToolStripMenuItem_UserMsg_Query_Click(System::Object^  sender, System::EventArgs^  e) {
			CFormUserMsgQuery^ dlg = gcnew CFormUserMsgQuery();
            dlg->ShowDialog();
		 }
private: System::Void ToolStripMenuItem_UserMsg_SendMsg_Click(System::Object^  sender, System::EventArgs^  e) {
			 sLoginRsp logInfo;

			 memset(&logInfo, 0, sizeof(logInfo));
			 CDataInfo::NewInstance()->GetLogonInfo(logInfo);
			 CFormSendMsgMultiUsers^ dlg = gcnew CFormSendMsgMultiUsers();
			 dlg->ResetUser(logInfo.mnUserID);
             dlg->ShowDialog();
		 }
	private: System::Void FormMain_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
                 LOG_INFO("Enter FormMain_FormClosing");
                 timer1->Stop();

				 formUserMsg->ResetExit(true);
				 formUserMsg->Close();
				 e->Cancel = false;
                 LOG_INFO("formUserMsg->Close");

                 //�˶���Ϣ 
                 int nHwnd = this->Handle.ToInt32();
                 CTcpLayer::UnsubscribePkg(CMDID_GetALLError_Push, nHwnd);
                 CTcpLayer::UnsubscribePkg(Cmd_UserMsg_PushMsg, nHwnd);
                 CTcpLayer::UnsubscribePkg(Cmd_TradeDataManager_ReInit_Rsp, nHwnd);
                 CTcpLayer::UnsubscribePkg(Cmd_ReInitQuotServerEvent_Rsp, nHwnd);
                 CTcpLayer::UnsubscribePkg(Cmd_QuotServerTimeResetEvent_Rsp, nHwnd);
                 CTcpLayer::UnsubscribePkg(Cmd_QryRole_Rsp, nHwnd);
                 CTcpLayer::UnsubscribePkg(Cmd_QryPrivilege_Rsp, nHwnd);
                 CTcpLayer::UnsubscribePkg(Cmd_QueryRolePrivilegeRelation_Rsp, nHwnd);
                 CTcpLayer::UnsubscribePkg(Cmd_QryUserRoleRelation_Rsp, nHwnd);
                 CTcpLayer::UnsubscribePkg(Cmd_TradeDataManager_QryStatus_Rsp, nHwnd);

                 LOG_INFO("UnsubscribePkg RSP");
			 }
	private: System::Void FormMain_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {                 
                 //timer1->Stop();
                 LOG_INFO("FormMain_FormClosed");
			 }
private: System::Void ��������ʼ��ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("Զ�̷�������ַ<%s> �û�<%d> ����<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_CONTROL_TRADESERVERINIT));
			 if(::DialogResult::OK ==  MessageBox::Show("ֻ�н�����ɺ�ſ������³�ʼ����������������³�ʼ�����������������ڴ���ȫ�����ݱ���ա�ȷ��Ҫ����������������", "��ʾ", MessageBoxButtons::OKCancel, MessageBoxIcon::Warning))
			 {
                 timer1->Stop();
				 CTcpLayer::SendData(Cmd_TradeDataManager_ReInit_Req,NULL,0,0,m_pLogin->mnUserID);
			 }
			 return;             
		 }
private: System::Void ��ʱ����ʱ������ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("Զ�̷�������ַ<%s> �û�<%d> ����<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_CONTROL_TRADESERVERTIMEAUTOINIT));
			 ReinitTimeOption^ dlg = gcnew ReinitTimeOption();
			 dlg->ShowDialog();		
		 }
private: System::Void ToolStripMenuItem_QuotServer_ReInit_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("Զ�̷�������ַ<%s> �û�<%d> ����<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_CONTROL_QUOTSERVERINIT));
             if(::DialogResult::OK ==  MessageBox::Show("ȷ��Ҫ���³�ʼ����������", "��ʾ", MessageBoxButtons::OKCancel, MessageBoxIcon::Warning))
             {
                 CTcpLayer::SendData(Cmd_ReInitQuotServerEvent_Req,NULL,0,0,m_pLogin->mnUserID);
             }
             return;
         }
private: System::Void ToolStripMenuItem_QuotServer_TimeSetup_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("Զ�̷�������ַ<%s> �û�<%d> ����<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_CONTROL_QUOTSERVERTIMEAUTOINIT));
             QuotServerTimeResetForm^ dlg = gcnew QuotServerTimeResetForm();
             dlg->ShowDialog();	
         }
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
             CTcpLayer::SendData(Cmd_TradeDataManager_QryStatus_Req, NULL, 0, 0, m_pLogin->mnUserID);
         }
private: System::Void ����ʱ�������ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("Զ�̷�������ַ<%s> �û�<%d> ����<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_CONTROL_TRADETIMESET));
			 TradingTimeOption^ dlg = gcnew TradingTimeOption();
			 dlg->ShowDialog();	
		 }
private: System::Void �����Զ�����ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		 {			 
			 SettlementConfirm^ dlg = gcnew SettlementConfirm();
			 dlg->ShowDialog();
		 }
private: System::Void ���׷�����ǿ�Ƴ�ʼ��ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 LOG_INFO("Զ�̷�������ַ<%s> �û�<%d> ����<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_CONTROL_TRADESERVERINIT));
			 if(::DialogResult::OK ==  MessageBox::Show("ֻ�н�����ɺ�ſ������³�ʼ����������������³�ʼ�����������������ڴ���ȫ�����ݱ���ա�ȷ��Ҫ����������������", "��ʾ", MessageBoxButtons::OKCancel, MessageBoxIcon::Warning))
			 {
				 timer1->Stop();
				 CTcpLayer::SendData(Cmd_TradeDataManager_ReForceInit_Req,NULL,0,0,m_pLogin->mnUserID);
			 }
		 }
private: System::Void ToolStripMenuItem_SimulateQuotServer_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("Զ�̷�������ַ<%s> �û�<%d> ����<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_CONTROL_SIMULATEQUOTSERVERSETUP));
             SimulateQuotServerSetupForm^ dlg = gcnew SimulateQuotServerSetupForm();
             dlg->ShowDialog();
         }
};
}
