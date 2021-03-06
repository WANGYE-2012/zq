#pragma once
#include <cliext/set>
#include <cliext/vector>
#include <sstream>
#include "RiskData.h"
#include "FormModifyPsw.h"
#include "FormLockWindow.h"
#include "FormAutoSelectInstSetup.h"
#include "FormTableColumnSetup.h"
#include "FormForceClose.h"
#include "FormRiskPlanSetup_Add.h"
#include "FormRiskPlanSetup_Del.h"
#include "RiskManageStruct.h"
#include "QueryInvestorInfo.h"
#include "QueryRiskEvt.h"
#include "QueryFundNetValue.h"
#include "QueryVerifyOrder.h"
#include "CFormSendMsgMultiUsers.h"
#include "CFromSendMsgSingleUser.h"
#include "CFormUserMsgQuery.h"
#include "CFormUserMsgShow.h"
#include "QueryForceClose.h"
#include "DataOpr.h"
#include "Tools.h"

#pragma unmanaged
//风控方案
string           g_riskPlanOrg;
Key_RiskPlan     g_riskPlanKey;
vector<RiskPlan> g_vecRiskPlan;
map<int,double>  g_mapRiskLevelInfo2;//新增和修改缓存的数据，没有真正提交服务器

//持仓推送
map<PositionKey,PlatformStru_Position> g_mapPositionPush;

//map<表名, map(列标题，索引)> 表格列右击隐藏
map<string, map<string,int>>  g_mapColumnText2Index;

/*暂时注销，后期在做---------------------start
//FormTableColumnSetup中使用
vector<string> g_vecTreeNodeTitle;//组织机构的名称
map<string,vector<string>> g_mapTreeNode2Column;//组织机构名称对应的列表名称

std::vector<std::string> g_vecTitleDepthMarket;
std::vector<std::string> g_vecTitleRiskEvent;
std::vector<std::string> g_vecTitleTradingAccount;
std::vector<std::string> g_vecTitleAllOrder;
std::vector<std::string> g_vecTitleTrade;
std::vector<std::string> g_vecTitlePosition;
std::vector<std::string> g_vecTitleUnTrade;
std::vector<std::string> g_vecTitleClosePosition;
std::vector<std::string> g_vecTitleNeedVerify;
/*暂时注销，后期在做---------------------end*/

//FormAutoSelectInstSetup中使用
vector<string> g_vecInstSelected;
//报价表现量
map<string,int>  g_mapCurrString2Volumn;

#include "FileOpr.h"

#if 0
#define LOG_INFO(fmt, ...)  ;
#else
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","CltRisk", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif



#pragma managed
namespace CltRisk {


	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;   
    using namespace System::Collections;
    using namespace System::Collections::Specialized;
    using namespace System::Xml;
    using namespace System::Xml::Serialization;
    using namespace System::Text::RegularExpressions;
    using namespace System::Diagnostics;
    using namespace System::Threading;


  

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

            m_FormLockWindowDlg = nullptr;
            m_bLoad = false;
            m_FormForceClose = nullptr;
            m_FormRiskPlanSetup_Add = nullptr;
            m_activeDataGridView = nullptr;
            m_activeDataTable = nullptr;
            m_pLogin = new sLoginRsp;
            m_vecTraderAccountFromTree = gcnew cliext::vector<String^>();
            m_nCurrColumnHeaderIndex_MouseRight = -1;
            nCurrUserID_MouseRight = -1;
            formUserMsg = gcnew CFormUserMsgShow();            

            vector<string> vecExchange2Inst;            
            CConfigManager::GetAutoSelectedInst(vecExchange2Inst);
            CRiskData::GetInstance()->SetConfigFile(vecExchange2Inst,XML_TYPE_INST);

//             vector<string> vecDataGridView2Column;
//             CConfigManager::GetAutoSelectedColumn(vecDataGridView2Column);
//             CRiskData::GetInstance()->SetConfigFile(vecDataGridView2Column,XML_TYPE_COLUMN);
// 
//             vector<string> vecInstrumentInfo;
//             CConfigManager::GetInstrumentInfo(vecInstrumentInfo);
//             CRiskData::GetInstance()->SetConfigFile(vecInstrumentInfo,XML_TYPE_INFO);

            g_mapCurrString2Volumn.clear();

            m_currInstrumentText = String::Empty;
            bCurrUserID_MouseRight = false;
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
            if (m_pLogin)
            {
                delete m_pLogin;
                m_pLogin = NULL;
            }
		}



    private: int bCurrUserID_MouseRight;
	private: int nCurrUserID_MouseRight;
	private: CFormUserMsgShow^ formUserMsg;
    

	private: System::Windows::Forms::MenuStrip^  menuStrip1;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_System;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_ModifyPSW;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_LockWindow;
	private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_Exit;

    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_Setup;

    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_AutoInstSetup;

    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_Message;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_MsgQuery;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_MsgSend;

    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_Query;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_QueryHistoryRiskEvent;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_QueryHistoryMsgRecord;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_QueryHistoryFund;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_QueryOrderVerify;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_QueryForceClose;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_QueryTraderBaseInfo;

    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_Help;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_HelpDocument;
    private: System::Windows::Forms::ToolStripMenuItem^  ToolStripMenuItem_VersionIllustration;

    private: System::Windows::Forms::SplitContainer^  splitContainer_LeftRight;
    private: System::Windows::Forms::SplitContainer^  splitContainer_MiddleDown;
    private: System::Windows::Forms::SplitContainer^  splitContainer_UpMiddle;   
    private: System::Windows::Forms::SplitContainer^  splitContainer_AllOrder;
    private: System::Windows::Forms::SplitContainer^  splitContainer_OrgRiskSetup;

    private: System::Windows::Forms::TabControl^  tabControl_Organization;    
    private: System::Windows::Forms::TabPage^  tabPage_Organization;    
    private: System::Windows::Forms::TreeView^  treeView_Organization;

    private: System::Windows::Forms::TabControl^  tabControl_Up;
    private: System::Windows::Forms::TabPage^  tabPage_QuotOrderTable;   
    private: System::Windows::Forms::DataGridView^  dataGridView_QuotOrderTable; 
    private: System::Windows::Forms::TabPage^  tabPage_RiskEvent;
    private: System::Windows::Forms::DataGridView^  dataGridView_RiskEvent;








    private: System::Windows::Forms::TabControl^  tabControl_Middle;
    private: System::Windows::Forms::TabPage^  tabPage_AccountFundInfo;
    private: System::Windows::Forms::DataGridView^  dataGridView_AccountFundInfo;

    private: System::Windows::Forms::TabPage^  tabPage_OrgRiskSetup;
    private: System::Windows::Forms::DataGridView^  dataGridView_OrgRiskSetup;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column55;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column56;
    private: System::Windows::Forms::DataGridViewButtonColumn^  Column57;
    private: System::Windows::Forms::Button^  button_PlanDel;
    private: System::Windows::Forms::Button^  button_PlanAdd;

    private: System::Windows::Forms::TabControl^  tabControl_Down;
    private: System::Windows::Forms::TabPage^  tabPage_AllOrder;
    private: System::Windows::Forms::DataGridView^  dataGridView_AllOrder;
    private: System::Windows::Forms::TabPage^  tabPage_UnTradeRecord;
    private: System::Windows::Forms::SplitContainer^  splitContainer_UnTradeRecord;
    private: System::Windows::Forms::TabPage^  tabPage_TradeRecord;   

    private: System::Windows::Forms::SplitContainer^  splitContainer_TradeRecord;

    private: System::Windows::Forms::TabPage^  tabPage_OpenPosition;
    private: System::Windows::Forms::SplitContainer^  splitContainer_OpenPosition;

    private: System::Windows::Forms::TabPage^  tabPage_ClosePosition;
    private: System::Windows::Forms::SplitContainer^  splitContainer_ClosePosition;

    private: System::Windows::Forms::TabPage^  tabPage_VerifyOrder;
    private: System::Windows::Forms::StatusStrip^  statusStrip;

    private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
    private: System::Windows::Forms::Timer^  timer_FormMain;


    private: System::Windows::Forms::SplitContainer^  splitContainer_NeedVerify;
    private: System::Windows::Forms::DataGridView^  dataGridView_TradeRecord;
    private: System::Windows::Forms::DataGridView^  dataGridView_UnTradeRecord;
    private: System::Windows::Forms::DataGridView^  dataGridView_Position;
    private: System::Windows::Forms::RadioButton^  radioButton_PositionDetail;
    private: System::Windows::Forms::RadioButton^  radioButton_Position;

    private: System::Windows::Forms::BindingNavigator^  bindingNavigator1;
    private: System::Windows::Forms::ToolStripLabel^  bindingNavigatorCountItem;
    private: System::Windows::Forms::ToolStripButton^  bindingNavigatorMoveFirstItem;
    private: System::Windows::Forms::ToolStripButton^  bindingNavigatorMovePreviousItem;
    private: System::Windows::Forms::ToolStripSeparator^  bindingNavigatorSeparator;
    private: System::Windows::Forms::ToolStripTextBox^  bindingNavigatorPositionItem;
    private: System::Windows::Forms::ToolStripSeparator^  bindingNavigatorSeparator1;
    private: System::Windows::Forms::ToolStripButton^  bindingNavigatorMoveNextItem;
    private: System::Windows::Forms::ToolStripButton^  bindingNavigatorMoveLastItem;
    private: System::Windows::Forms::ToolStripSeparator^  bindingNavigatorSeparator2;
    private: System::Windows::Forms::DataGridView^  dataGridView_ClosePosition;
    private: System::Windows::Forms::DataGridView^  dataGridView_NeedVerify;
    private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip_Sub;

    private: System::Windows::Forms::Button^  button_ForbidOrder;
    private: System::Windows::Forms::ContextMenuStrip^  cntMenuOriganTree;
    private: System::Windows::Forms::ToolStripMenuItem^  发送消息ToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  交易限制ToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  取消限制ToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  手动审核ToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  取消审核ToolStripMenuItem;
    private: System::Windows::Forms::Button^  button_PassVerify;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
private: System::Windows::Forms::Button^  button_PlanModify;
private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel2;











    private: System::ComponentModel::IContainer^  components;


    private:
		/// <summary>
		/// Required designer variable.
		/// </summary>  


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->components = (gcnew System::ComponentModel::Container());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle3 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle4 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle5 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle6 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle7 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle8 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle9 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle10 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle11 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle12 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle13 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle14 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle15 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle16 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle17 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(FormMain::typeid));
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle18 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle19 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle20 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle21 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle22 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle23 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle24 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle25 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle26 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle27 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle28 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle29 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
            this->ToolStripMenuItem_System = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_ModifyPSW = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_LockWindow = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_Exit = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_Setup = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_AutoInstSetup = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_Message = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_MsgQuery = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_MsgSend = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_Query = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_QueryHistoryRiskEvent = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_QueryHistoryMsgRecord = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_QueryHistoryFund = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_QueryOrderVerify = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_QueryForceClose = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_QueryTraderBaseInfo = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_Help = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_HelpDocument = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ToolStripMenuItem_VersionIllustration = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->statusStrip = (gcnew System::Windows::Forms::StatusStrip());
            this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            this->toolStripStatusLabel2 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            this->splitContainer_LeftRight = (gcnew System::Windows::Forms::SplitContainer());
            this->tabControl_Organization = (gcnew System::Windows::Forms::TabControl());
            this->tabPage_Organization = (gcnew System::Windows::Forms::TabPage());
            this->treeView_Organization = (gcnew System::Windows::Forms::TreeView());
            this->splitContainer_MiddleDown = (gcnew System::Windows::Forms::SplitContainer());
            this->splitContainer_UpMiddle = (gcnew System::Windows::Forms::SplitContainer());
            this->tabControl_Up = (gcnew System::Windows::Forms::TabControl());
            this->tabPage_QuotOrderTable = (gcnew System::Windows::Forms::TabPage());
            this->dataGridView_QuotOrderTable = (gcnew System::Windows::Forms::DataGridView());
            this->tabPage_RiskEvent = (gcnew System::Windows::Forms::TabPage());
            this->dataGridView_RiskEvent = (gcnew System::Windows::Forms::DataGridView());
            this->tabControl_Middle = (gcnew System::Windows::Forms::TabControl());
            this->tabPage_AccountFundInfo = (gcnew System::Windows::Forms::TabPage());
            this->dataGridView_AccountFundInfo = (gcnew System::Windows::Forms::DataGridView());
            this->tabPage_OrgRiskSetup = (gcnew System::Windows::Forms::TabPage());
            this->splitContainer_OrgRiskSetup = (gcnew System::Windows::Forms::SplitContainer());
            this->dataGridView_OrgRiskSetup = (gcnew System::Windows::Forms::DataGridView());
            this->Column55 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column56 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column57 = (gcnew System::Windows::Forms::DataGridViewButtonColumn());
            this->button_PlanModify = (gcnew System::Windows::Forms::Button());
            this->button_PlanDel = (gcnew System::Windows::Forms::Button());
            this->button_PlanAdd = (gcnew System::Windows::Forms::Button());
            this->tabControl_Down = (gcnew System::Windows::Forms::TabControl());
            this->tabPage_AllOrder = (gcnew System::Windows::Forms::TabPage());
            this->splitContainer_AllOrder = (gcnew System::Windows::Forms::SplitContainer());
            this->dataGridView_AllOrder = (gcnew System::Windows::Forms::DataGridView());
            this->bindingNavigator1 = (gcnew System::Windows::Forms::BindingNavigator(this->components));
            this->bindingNavigatorCountItem = (gcnew System::Windows::Forms::ToolStripLabel());
            this->bindingNavigatorMoveFirstItem = (gcnew System::Windows::Forms::ToolStripButton());
            this->bindingNavigatorMovePreviousItem = (gcnew System::Windows::Forms::ToolStripButton());
            this->bindingNavigatorSeparator = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->bindingNavigatorPositionItem = (gcnew System::Windows::Forms::ToolStripTextBox());
            this->bindingNavigatorSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->bindingNavigatorMoveNextItem = (gcnew System::Windows::Forms::ToolStripButton());
            this->bindingNavigatorMoveLastItem = (gcnew System::Windows::Forms::ToolStripButton());
            this->bindingNavigatorSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->tabPage_UnTradeRecord = (gcnew System::Windows::Forms::TabPage());
            this->splitContainer_UnTradeRecord = (gcnew System::Windows::Forms::SplitContainer());
            this->dataGridView_UnTradeRecord = (gcnew System::Windows::Forms::DataGridView());
            this->tabPage_TradeRecord = (gcnew System::Windows::Forms::TabPage());
            this->splitContainer_TradeRecord = (gcnew System::Windows::Forms::SplitContainer());
            this->dataGridView_TradeRecord = (gcnew System::Windows::Forms::DataGridView());
            this->tabPage_OpenPosition = (gcnew System::Windows::Forms::TabPage());
            this->splitContainer_OpenPosition = (gcnew System::Windows::Forms::SplitContainer());
            this->dataGridView_Position = (gcnew System::Windows::Forms::DataGridView());
            this->radioButton_PositionDetail = (gcnew System::Windows::Forms::RadioButton());
            this->radioButton_Position = (gcnew System::Windows::Forms::RadioButton());
            this->tabPage_ClosePosition = (gcnew System::Windows::Forms::TabPage());
            this->splitContainer_ClosePosition = (gcnew System::Windows::Forms::SplitContainer());
            this->dataGridView_ClosePosition = (gcnew System::Windows::Forms::DataGridView());
            this->tabPage_VerifyOrder = (gcnew System::Windows::Forms::TabPage());
            this->splitContainer_NeedVerify = (gcnew System::Windows::Forms::SplitContainer());
            this->dataGridView_NeedVerify = (gcnew System::Windows::Forms::DataGridView());
            this->button_ForbidOrder = (gcnew System::Windows::Forms::Button());
            this->button_PassVerify = (gcnew System::Windows::Forms::Button());
            this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->timer_FormMain = (gcnew System::Windows::Forms::Timer(this->components));
            this->contextMenuStrip_Sub = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->cntMenuOriganTree = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->发送消息ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->交易限制ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->取消限制ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->手动审核ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->取消审核ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuStrip1->SuspendLayout();
            this->statusStrip->SuspendLayout();
            this->splitContainer_LeftRight->Panel1->SuspendLayout();
            this->splitContainer_LeftRight->Panel2->SuspendLayout();
            this->splitContainer_LeftRight->SuspendLayout();
            this->tabControl_Organization->SuspendLayout();
            this->tabPage_Organization->SuspendLayout();
            this->splitContainer_MiddleDown->Panel1->SuspendLayout();
            this->splitContainer_MiddleDown->Panel2->SuspendLayout();
            this->splitContainer_MiddleDown->SuspendLayout();
            this->splitContainer_UpMiddle->Panel1->SuspendLayout();
            this->splitContainer_UpMiddle->Panel2->SuspendLayout();
            this->splitContainer_UpMiddle->SuspendLayout();
            this->tabControl_Up->SuspendLayout();
            this->tabPage_QuotOrderTable->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_QuotOrderTable))->BeginInit();
            this->tabPage_RiskEvent->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_RiskEvent))->BeginInit();
            this->tabControl_Middle->SuspendLayout();
            this->tabPage_AccountFundInfo->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_AccountFundInfo))->BeginInit();
            this->tabPage_OrgRiskSetup->SuspendLayout();
            this->splitContainer_OrgRiskSetup->Panel1->SuspendLayout();
            this->splitContainer_OrgRiskSetup->Panel2->SuspendLayout();
            this->splitContainer_OrgRiskSetup->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_OrgRiskSetup))->BeginInit();
            this->tabControl_Down->SuspendLayout();
            this->tabPage_AllOrder->SuspendLayout();
            this->splitContainer_AllOrder->Panel1->SuspendLayout();
            this->splitContainer_AllOrder->Panel2->SuspendLayout();
            this->splitContainer_AllOrder->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_AllOrder))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->bindingNavigator1))->BeginInit();
            this->bindingNavigator1->SuspendLayout();
            this->tabPage_UnTradeRecord->SuspendLayout();
            this->splitContainer_UnTradeRecord->Panel1->SuspendLayout();
            this->splitContainer_UnTradeRecord->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_UnTradeRecord))->BeginInit();
            this->tabPage_TradeRecord->SuspendLayout();
            this->splitContainer_TradeRecord->Panel1->SuspendLayout();
            this->splitContainer_TradeRecord->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_TradeRecord))->BeginInit();
            this->tabPage_OpenPosition->SuspendLayout();
            this->splitContainer_OpenPosition->Panel1->SuspendLayout();
            this->splitContainer_OpenPosition->Panel2->SuspendLayout();
            this->splitContainer_OpenPosition->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_Position))->BeginInit();
            this->tabPage_ClosePosition->SuspendLayout();
            this->splitContainer_ClosePosition->Panel1->SuspendLayout();
            this->splitContainer_ClosePosition->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_ClosePosition))->BeginInit();
            this->tabPage_VerifyOrder->SuspendLayout();
            this->splitContainer_NeedVerify->Panel1->SuspendLayout();
            this->splitContainer_NeedVerify->Panel2->SuspendLayout();
            this->splitContainer_NeedVerify->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_NeedVerify))->BeginInit();
            this->SuspendLayout();
            // 
            // menuStrip1
            // 
            this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->ToolStripMenuItem_System, 
                this->ToolStripMenuItem_Setup, this->ToolStripMenuItem_Message, this->ToolStripMenuItem_Query, this->ToolStripMenuItem_Help});
            this->menuStrip1->Location = System::Drawing::Point(0, 0);
            this->menuStrip1->Name = L"menuStrip1";
            this->menuStrip1->Size = System::Drawing::Size(1334, 24);
            this->menuStrip1->TabIndex = 0;
            this->menuStrip1->Text = L"menuStrip1";
            // 
            // ToolStripMenuItem_System
            // 
            this->ToolStripMenuItem_System->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->ToolStripMenuItem_ModifyPSW, 
                this->ToolStripMenuItem_LockWindow, this->ToolStripMenuItem_Exit});
            this->ToolStripMenuItem_System->Name = L"ToolStripMenuItem_System";
            this->ToolStripMenuItem_System->Size = System::Drawing::Size(41, 20);
            this->ToolStripMenuItem_System->Text = L"系统";
            // 
            // ToolStripMenuItem_ModifyPSW
            // 
            this->ToolStripMenuItem_ModifyPSW->Name = L"ToolStripMenuItem_ModifyPSW";
            this->ToolStripMenuItem_ModifyPSW->Size = System::Drawing::Size(118, 22);
            this->ToolStripMenuItem_ModifyPSW->Text = L"修改密码";
            this->ToolStripMenuItem_ModifyPSW->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_ModifyPSW_Click);
            // 
            // ToolStripMenuItem_LockWindow
            // 
            this->ToolStripMenuItem_LockWindow->Name = L"ToolStripMenuItem_LockWindow";
            this->ToolStripMenuItem_LockWindow->Size = System::Drawing::Size(118, 22);
            this->ToolStripMenuItem_LockWindow->Text = L"锁定窗口";
            this->ToolStripMenuItem_LockWindow->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_LockWindow_Click);
            // 
            // ToolStripMenuItem_Exit
            // 
            this->ToolStripMenuItem_Exit->Name = L"ToolStripMenuItem_Exit";
            this->ToolStripMenuItem_Exit->Size = System::Drawing::Size(118, 22);
            this->ToolStripMenuItem_Exit->Text = L"退出";
            this->ToolStripMenuItem_Exit->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_Exit_Click);
            // 
            // ToolStripMenuItem_Setup
            // 
            this->ToolStripMenuItem_Setup->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->ToolStripMenuItem_AutoInstSetup});
            this->ToolStripMenuItem_Setup->Name = L"ToolStripMenuItem_Setup";
            this->ToolStripMenuItem_Setup->Size = System::Drawing::Size(41, 20);
            this->ToolStripMenuItem_Setup->Text = L"设置";
            // 
            // ToolStripMenuItem_AutoInstSetup
            // 
            this->ToolStripMenuItem_AutoInstSetup->Name = L"ToolStripMenuItem_AutoInstSetup";
            this->ToolStripMenuItem_AutoInstSetup->Size = System::Drawing::Size(142, 22);
            this->ToolStripMenuItem_AutoInstSetup->Text = L"自选合约设置";
            this->ToolStripMenuItem_AutoInstSetup->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_AutoInstSetup_Click);
            // 
            // ToolStripMenuItem_Message
            // 
            this->ToolStripMenuItem_Message->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->ToolStripMenuItem_MsgQuery, 
                this->ToolStripMenuItem_MsgSend});
            this->ToolStripMenuItem_Message->Name = L"ToolStripMenuItem_Message";
            this->ToolStripMenuItem_Message->Size = System::Drawing::Size(41, 20);
            this->ToolStripMenuItem_Message->Text = L"消息";
            // 
            // ToolStripMenuItem_MsgQuery
            // 
            this->ToolStripMenuItem_MsgQuery->Name = L"ToolStripMenuItem_MsgQuery";
            this->ToolStripMenuItem_MsgQuery->Size = System::Drawing::Size(118, 22);
            this->ToolStripMenuItem_MsgQuery->Text = L"消息查询";
            this->ToolStripMenuItem_MsgQuery->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_MsgQuery_Click);
            // 
            // ToolStripMenuItem_MsgSend
            // 
            this->ToolStripMenuItem_MsgSend->Name = L"ToolStripMenuItem_MsgSend";
            this->ToolStripMenuItem_MsgSend->Size = System::Drawing::Size(118, 22);
            this->ToolStripMenuItem_MsgSend->Text = L"消息发送";
            this->ToolStripMenuItem_MsgSend->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_MsgSend_Click);
            // 
            // ToolStripMenuItem_Query
            // 
            this->ToolStripMenuItem_Query->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->ToolStripMenuItem_QueryHistoryRiskEvent, 
                this->ToolStripMenuItem_QueryHistoryMsgRecord, this->ToolStripMenuItem_QueryHistoryFund, this->ToolStripMenuItem_QueryOrderVerify, 
                this->ToolStripMenuItem_QueryForceClose, this->ToolStripMenuItem_QueryTraderBaseInfo});
            this->ToolStripMenuItem_Query->Name = L"ToolStripMenuItem_Query";
            this->ToolStripMenuItem_Query->Size = System::Drawing::Size(41, 20);
            this->ToolStripMenuItem_Query->Text = L"查询";
            // 
            // ToolStripMenuItem_QueryHistoryRiskEvent
            // 
            this->ToolStripMenuItem_QueryHistoryRiskEvent->Name = L"ToolStripMenuItem_QueryHistoryRiskEvent";
            this->ToolStripMenuItem_QueryHistoryRiskEvent->Size = System::Drawing::Size(178, 22);
            this->ToolStripMenuItem_QueryHistoryRiskEvent->Text = L"历史风险事件查询";
            this->ToolStripMenuItem_QueryHistoryRiskEvent->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_QueryHistoryRiskEvent_Click);
            // 
            // ToolStripMenuItem_QueryHistoryMsgRecord
            // 
            this->ToolStripMenuItem_QueryHistoryMsgRecord->Name = L"ToolStripMenuItem_QueryHistoryMsgRecord";
            this->ToolStripMenuItem_QueryHistoryMsgRecord->Size = System::Drawing::Size(178, 22);
            this->ToolStripMenuItem_QueryHistoryMsgRecord->Text = L"历史消息记录查询";
            this->ToolStripMenuItem_QueryHistoryMsgRecord->Visible = false;
            this->ToolStripMenuItem_QueryHistoryMsgRecord->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_QueryHistoryMsgRecord_Click);
            // 
            // ToolStripMenuItem_QueryHistoryFund
            // 
            this->ToolStripMenuItem_QueryHistoryFund->Name = L"ToolStripMenuItem_QueryHistoryFund";
            this->ToolStripMenuItem_QueryHistoryFund->Size = System::Drawing::Size(178, 22);
            this->ToolStripMenuItem_QueryHistoryFund->Text = L"历史基金净值查询";
            this->ToolStripMenuItem_QueryHistoryFund->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_HistoryFundValue_Click);
            // 
            // ToolStripMenuItem_QueryOrderVerify
            // 
            this->ToolStripMenuItem_QueryOrderVerify->Name = L"ToolStripMenuItem_QueryOrderVerify";
            this->ToolStripMenuItem_QueryOrderVerify->Size = System::Drawing::Size(178, 22);
            this->ToolStripMenuItem_QueryOrderVerify->Text = L"报单审核记录查询";
            this->ToolStripMenuItem_QueryOrderVerify->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_QueryOrderVerify_Click);
            // 
            // ToolStripMenuItem_QueryForceClose
            // 
            this->ToolStripMenuItem_QueryForceClose->Name = L"ToolStripMenuItem_QueryForceClose";
            this->ToolStripMenuItem_QueryForceClose->Size = System::Drawing::Size(178, 22);
            this->ToolStripMenuItem_QueryForceClose->Text = L"强平记录查询";
            this->ToolStripMenuItem_QueryForceClose->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_QueryForceClose_Click);
            // 
            // ToolStripMenuItem_QueryTraderBaseInfo
            // 
            this->ToolStripMenuItem_QueryTraderBaseInfo->Name = L"ToolStripMenuItem_QueryTraderBaseInfo";
            this->ToolStripMenuItem_QueryTraderBaseInfo->Size = System::Drawing::Size(178, 22);
            this->ToolStripMenuItem_QueryTraderBaseInfo->Text = L"交易员基础信息查询";
            this->ToolStripMenuItem_QueryTraderBaseInfo->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_QueryTraderBaseInfo_Click);
            // 
            // ToolStripMenuItem_Help
            // 
            this->ToolStripMenuItem_Help->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->ToolStripMenuItem_HelpDocument, 
                this->ToolStripMenuItem_VersionIllustration});
            this->ToolStripMenuItem_Help->Name = L"ToolStripMenuItem_Help";
            this->ToolStripMenuItem_Help->Size = System::Drawing::Size(41, 20);
            this->ToolStripMenuItem_Help->Text = L"帮助";
            // 
            // ToolStripMenuItem_HelpDocument
            // 
            this->ToolStripMenuItem_HelpDocument->Name = L"ToolStripMenuItem_HelpDocument";
            this->ToolStripMenuItem_HelpDocument->Size = System::Drawing::Size(118, 22);
            this->ToolStripMenuItem_HelpDocument->Text = L"帮助文档";
            this->ToolStripMenuItem_HelpDocument->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_HelpDocument_Click);
            // 
            // ToolStripMenuItem_VersionIllustration
            // 
            this->ToolStripMenuItem_VersionIllustration->Name = L"ToolStripMenuItem_VersionIllustration";
            this->ToolStripMenuItem_VersionIllustration->Size = System::Drawing::Size(118, 22);
            this->ToolStripMenuItem_VersionIllustration->Text = L"版本说明";
            this->ToolStripMenuItem_VersionIllustration->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_VersionIllustration_Click);
            // 
            // statusStrip
            // 
            this->statusStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->toolStripStatusLabel1, 
                this->toolStripStatusLabel2});
            this->statusStrip->Location = System::Drawing::Point(0, 621);
            this->statusStrip->Name = L"statusStrip";
            this->statusStrip->Size = System::Drawing::Size(1334, 22);
            this->statusStrip->TabIndex = 1;
            this->statusStrip->Text = L"statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
            this->toolStripStatusLabel1->Size = System::Drawing::Size(125, 17);
            this->toolStripStatusLabel1->Text = L"与交易服务器连接成功";
            // 
            // toolStripStatusLabel2
            // 
            this->toolStripStatusLabel2->Name = L"toolStripStatusLabel2";
            this->toolStripStatusLabel2->Size = System::Drawing::Size(131, 17);
            this->toolStripStatusLabel2->Text = L"与行情服务器连接中...";
            // 
            // splitContainer_LeftRight
            // 
            this->splitContainer_LeftRight->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer_LeftRight->Location = System::Drawing::Point(0, 24);
            this->splitContainer_LeftRight->Name = L"splitContainer_LeftRight";
            // 
            // splitContainer_LeftRight.Panel1
            // 
            this->splitContainer_LeftRight->Panel1->Controls->Add(this->tabControl_Organization);
            // 
            // splitContainer_LeftRight.Panel2
            // 
            this->splitContainer_LeftRight->Panel2->Controls->Add(this->splitContainer_MiddleDown);
            this->splitContainer_LeftRight->Size = System::Drawing::Size(1334, 597);
            this->splitContainer_LeftRight->SplitterDistance = 157;
            this->splitContainer_LeftRight->TabIndex = 2;
            // 
            // tabControl_Organization
            // 
            this->tabControl_Organization->Controls->Add(this->tabPage_Organization);
            this->tabControl_Organization->Dock = System::Windows::Forms::DockStyle::Fill;
            this->tabControl_Organization->Location = System::Drawing::Point(0, 0);
            this->tabControl_Organization->Name = L"tabControl_Organization";
            this->tabControl_Organization->SelectedIndex = 0;
            this->tabControl_Organization->Size = System::Drawing::Size(157, 597);
            this->tabControl_Organization->TabIndex = 0;
            // 
            // tabPage_Organization
            // 
            this->tabPage_Organization->Controls->Add(this->treeView_Organization);
            this->tabPage_Organization->Location = System::Drawing::Point(4, 21);
            this->tabPage_Organization->Name = L"tabPage_Organization";
            this->tabPage_Organization->Padding = System::Windows::Forms::Padding(3);
            this->tabPage_Organization->Size = System::Drawing::Size(149, 572);
            this->tabPage_Organization->TabIndex = 0;
            this->tabPage_Organization->Text = L"部门架构";
            this->tabPage_Organization->UseVisualStyleBackColor = true;
            // 
            // treeView_Organization
            // 
            this->treeView_Organization->BackColor = System::Drawing::SystemColors::Window;
            this->treeView_Organization->Dock = System::Windows::Forms::DockStyle::Fill;
            this->treeView_Organization->HideSelection = false;
            this->treeView_Organization->Location = System::Drawing::Point(3, 3);
            this->treeView_Organization->Name = L"treeView_Organization";
            this->treeView_Organization->Size = System::Drawing::Size(143, 566);
            this->treeView_Organization->TabIndex = 0;
            this->treeView_Organization->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &FormMain::treeView_Organization_MouseUp);
            this->treeView_Organization->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &FormMain::treeView_Organization_AfterSelect);
            // 
            // splitContainer_MiddleDown
            // 
            this->splitContainer_MiddleDown->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer_MiddleDown->Location = System::Drawing::Point(0, 0);
            this->splitContainer_MiddleDown->Name = L"splitContainer_MiddleDown";
            this->splitContainer_MiddleDown->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer_MiddleDown.Panel1
            // 
            this->splitContainer_MiddleDown->Panel1->Controls->Add(this->splitContainer_UpMiddle);
            this->splitContainer_MiddleDown->Panel1MinSize = 125;
            // 
            // splitContainer_MiddleDown.Panel2
            // 
            this->splitContainer_MiddleDown->Panel2->Controls->Add(this->tabControl_Down);
            this->splitContainer_MiddleDown->Panel2MinSize = 60;
            this->splitContainer_MiddleDown->Size = System::Drawing::Size(1173, 597);
            this->splitContainer_MiddleDown->SplitterDistance = 378;
            this->splitContainer_MiddleDown->TabIndex = 0;
            // 
            // splitContainer_UpMiddle
            // 
            this->splitContainer_UpMiddle->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer_UpMiddle->Location = System::Drawing::Point(0, 0);
            this->splitContainer_UpMiddle->Name = L"splitContainer_UpMiddle";
            this->splitContainer_UpMiddle->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer_UpMiddle.Panel1
            // 
            this->splitContainer_UpMiddle->Panel1->Controls->Add(this->tabControl_Up);
            // 
            // splitContainer_UpMiddle.Panel2
            // 
            this->splitContainer_UpMiddle->Panel2->Controls->Add(this->tabControl_Middle);
            this->splitContainer_UpMiddle->Size = System::Drawing::Size(1173, 378);
            this->splitContainer_UpMiddle->SplitterDistance = 160;
            this->splitContainer_UpMiddle->TabIndex = 0;
            // 
            // tabControl_Up
            // 
            this->tabControl_Up->Controls->Add(this->tabPage_QuotOrderTable);
            this->tabControl_Up->Controls->Add(this->tabPage_RiskEvent);
            this->tabControl_Up->Dock = System::Windows::Forms::DockStyle::Fill;
            this->tabControl_Up->Location = System::Drawing::Point(0, 0);
            this->tabControl_Up->Name = L"tabControl_Up";
            this->tabControl_Up->SelectedIndex = 0;
            this->tabControl_Up->Size = System::Drawing::Size(1173, 160);
            this->tabControl_Up->TabIndex = 0;
            // 
            // tabPage_QuotOrderTable
            // 
            this->tabPage_QuotOrderTable->Controls->Add(this->dataGridView_QuotOrderTable);
            this->tabPage_QuotOrderTable->Location = System::Drawing::Point(4, 21);
            this->tabPage_QuotOrderTable->Name = L"tabPage_QuotOrderTable";
            this->tabPage_QuotOrderTable->Padding = System::Windows::Forms::Padding(3);
            this->tabPage_QuotOrderTable->Size = System::Drawing::Size(1165, 135);
            this->tabPage_QuotOrderTable->TabIndex = 0;
            this->tabPage_QuotOrderTable->Text = L"报价表";
            this->tabPage_QuotOrderTable->UseVisualStyleBackColor = true;
            // 
            // dataGridView_QuotOrderTable
            // 
            this->dataGridView_QuotOrderTable->AllowUserToAddRows = false;
            this->dataGridView_QuotOrderTable->AllowUserToDeleteRows = false;
            this->dataGridView_QuotOrderTable->AllowUserToOrderColumns = true;
            this->dataGridView_QuotOrderTable->AllowUserToResizeRows = false;
            dataGridViewCellStyle1->BackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle1->ForeColor = System::Drawing::Color::White;
            dataGridViewCellStyle1->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            this->dataGridView_QuotOrderTable->AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
            this->dataGridView_QuotOrderTable->BackgroundColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
            dataGridViewCellStyle2->BackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle2->ForeColor = System::Drawing::Color::White;
            dataGridViewCellStyle2->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle2->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle2->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_QuotOrderTable->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle2;
            this->dataGridView_QuotOrderTable->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
            dataGridViewCellStyle3->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle3->BackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle3->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle3->ForeColor = System::Drawing::Color::White;
            dataGridViewCellStyle3->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle3->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle3->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridView_QuotOrderTable->DefaultCellStyle = dataGridViewCellStyle3;
            this->dataGridView_QuotOrderTable->Dock = System::Windows::Forms::DockStyle::Fill;
            this->dataGridView_QuotOrderTable->GridColor = System::Drawing::Color::Black;
            this->dataGridView_QuotOrderTable->Location = System::Drawing::Point(3, 3);
            this->dataGridView_QuotOrderTable->MultiSelect = false;
            this->dataGridView_QuotOrderTable->Name = L"dataGridView_QuotOrderTable";
            this->dataGridView_QuotOrderTable->ReadOnly = true;
            dataGridViewCellStyle4->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle4->BackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle4->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle4->ForeColor = System::Drawing::Color::White;
            dataGridViewCellStyle4->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle4->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle4->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_QuotOrderTable->RowHeadersDefaultCellStyle = dataGridViewCellStyle4;
            this->dataGridView_QuotOrderTable->RowHeadersVisible = false;
            this->dataGridView_QuotOrderTable->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
            dataGridViewCellStyle5->BackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle5->ForeColor = System::Drawing::Color::White;
            dataGridViewCellStyle5->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle5->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            this->dataGridView_QuotOrderTable->RowsDefaultCellStyle = dataGridViewCellStyle5;
            this->dataGridView_QuotOrderTable->RowTemplate->Height = 23;
            this->dataGridView_QuotOrderTable->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->dataGridView_QuotOrderTable->Size = System::Drawing::Size(1159, 129);
            this->dataGridView_QuotOrderTable->TabIndex = 0;
            this->dataGridView_QuotOrderTable->ColumnHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormMain::dataGridView_QuotOrderTable_ColumnHeaderMouseClick);
            this->dataGridView_QuotOrderTable->CellPainting += gcnew System::Windows::Forms::DataGridViewCellPaintingEventHandler(this, &FormMain::dataGridView_QuotOrderTable_CellPainting);
            // 
            // tabPage_RiskEvent
            // 
            this->tabPage_RiskEvent->Controls->Add(this->dataGridView_RiskEvent);
            this->tabPage_RiskEvent->Location = System::Drawing::Point(4, 21);
            this->tabPage_RiskEvent->Name = L"tabPage_RiskEvent";
            this->tabPage_RiskEvent->Padding = System::Windows::Forms::Padding(3);
            this->tabPage_RiskEvent->Size = System::Drawing::Size(1165, 135);
            this->tabPage_RiskEvent->TabIndex = 1;
            this->tabPage_RiskEvent->Text = L"当前风险事件";
            this->tabPage_RiskEvent->UseVisualStyleBackColor = true;
            // 
            // dataGridView_RiskEvent
            // 
            this->dataGridView_RiskEvent->AllowUserToAddRows = false;
            this->dataGridView_RiskEvent->AllowUserToDeleteRows = false;
            this->dataGridView_RiskEvent->AllowUserToResizeRows = false;
            dataGridViewCellStyle6->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle6->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle6->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle6->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            this->dataGridView_RiskEvent->AlternatingRowsDefaultCellStyle = dataGridViewCellStyle6;
            dataGridViewCellStyle7->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle7->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle7->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle7->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle7->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle7->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle7->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_RiskEvent->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle7;
            this->dataGridView_RiskEvent->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
            dataGridViewCellStyle8->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle8->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle8->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle8->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle8->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle8->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle8->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridView_RiskEvent->DefaultCellStyle = dataGridViewCellStyle8;
            this->dataGridView_RiskEvent->Dock = System::Windows::Forms::DockStyle::Fill;
            this->dataGridView_RiskEvent->Location = System::Drawing::Point(3, 3);
            this->dataGridView_RiskEvent->MultiSelect = false;
            this->dataGridView_RiskEvent->Name = L"dataGridView_RiskEvent";
            this->dataGridView_RiskEvent->ReadOnly = true;
            dataGridViewCellStyle9->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle9->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle9->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle9->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle9->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle9->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle9->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_RiskEvent->RowHeadersDefaultCellStyle = dataGridViewCellStyle9;
            this->dataGridView_RiskEvent->RowHeadersVisible = false;
            dataGridViewCellStyle10->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle10->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle10->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle10->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            this->dataGridView_RiskEvent->RowsDefaultCellStyle = dataGridViewCellStyle10;
            this->dataGridView_RiskEvent->RowTemplate->Height = 23;
            this->dataGridView_RiskEvent->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->dataGridView_RiskEvent->Size = System::Drawing::Size(1159, 129);
            this->dataGridView_RiskEvent->TabIndex = 0;
            this->dataGridView_RiskEvent->ColumnHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormMain::dataGridView_RiskEvent_ColumnHeaderMouseClick);
            // 
            // tabControl_Middle
            // 
            this->tabControl_Middle->Controls->Add(this->tabPage_AccountFundInfo);
            this->tabControl_Middle->Controls->Add(this->tabPage_OrgRiskSetup);
            this->tabControl_Middle->Dock = System::Windows::Forms::DockStyle::Fill;
            this->tabControl_Middle->Location = System::Drawing::Point(0, 0);
            this->tabControl_Middle->Name = L"tabControl_Middle";
            this->tabControl_Middle->SelectedIndex = 0;
            this->tabControl_Middle->Size = System::Drawing::Size(1173, 214);
            this->tabControl_Middle->TabIndex = 0;
            // 
            // tabPage_AccountFundInfo
            // 
            this->tabPage_AccountFundInfo->Controls->Add(this->dataGridView_AccountFundInfo);
            this->tabPage_AccountFundInfo->Location = System::Drawing::Point(4, 21);
            this->tabPage_AccountFundInfo->Name = L"tabPage_AccountFundInfo";
            this->tabPage_AccountFundInfo->Padding = System::Windows::Forms::Padding(3);
            this->tabPage_AccountFundInfo->Size = System::Drawing::Size(1165, 189);
            this->tabPage_AccountFundInfo->TabIndex = 0;
            this->tabPage_AccountFundInfo->Text = L"账户及资金信息";
            this->tabPage_AccountFundInfo->UseVisualStyleBackColor = true;
            // 
            // dataGridView_AccountFundInfo
            // 
            this->dataGridView_AccountFundInfo->AllowUserToAddRows = false;
            this->dataGridView_AccountFundInfo->AllowUserToDeleteRows = false;
            this->dataGridView_AccountFundInfo->AllowUserToResizeRows = false;
            dataGridViewCellStyle11->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle11->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle11->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle11->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle11->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle11->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle11->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_AccountFundInfo->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle11;
            this->dataGridView_AccountFundInfo->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
            dataGridViewCellStyle12->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle12->BackColor = System::Drawing::SystemColors::Window;
            dataGridViewCellStyle12->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle12->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle12->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle12->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle12->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridView_AccountFundInfo->DefaultCellStyle = dataGridViewCellStyle12;
            this->dataGridView_AccountFundInfo->Dock = System::Windows::Forms::DockStyle::Fill;
            this->dataGridView_AccountFundInfo->Location = System::Drawing::Point(3, 3);
            this->dataGridView_AccountFundInfo->MultiSelect = false;
            this->dataGridView_AccountFundInfo->Name = L"dataGridView_AccountFundInfo";
            this->dataGridView_AccountFundInfo->ReadOnly = true;
            dataGridViewCellStyle13->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle13->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle13->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle13->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle13->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle13->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle13->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_AccountFundInfo->RowHeadersDefaultCellStyle = dataGridViewCellStyle13;
            this->dataGridView_AccountFundInfo->RowHeadersVisible = false;
            this->dataGridView_AccountFundInfo->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
            this->dataGridView_AccountFundInfo->RowTemplate->Height = 23;
            this->dataGridView_AccountFundInfo->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->dataGridView_AccountFundInfo->Size = System::Drawing::Size(1159, 183);
            this->dataGridView_AccountFundInfo->TabIndex = 0;
            this->dataGridView_AccountFundInfo->CellMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormMain::dataGridView_AccountFundInfo_CellMouseClick);
            this->dataGridView_AccountFundInfo->ColumnHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormMain::dataGridView_AccountFundInfo_ColumnHeaderMouseClick);
            // 
            // tabPage_OrgRiskSetup
            // 
            this->tabPage_OrgRiskSetup->Controls->Add(this->splitContainer_OrgRiskSetup);
            this->tabPage_OrgRiskSetup->Location = System::Drawing::Point(4, 21);
            this->tabPage_OrgRiskSetup->Name = L"tabPage_OrgRiskSetup";
            this->tabPage_OrgRiskSetup->Padding = System::Windows::Forms::Padding(3);
            this->tabPage_OrgRiskSetup->Size = System::Drawing::Size(1165, 189);
            this->tabPage_OrgRiskSetup->TabIndex = 1;
            this->tabPage_OrgRiskSetup->Text = L"部门风控设置列表";
            this->tabPage_OrgRiskSetup->UseVisualStyleBackColor = true;
            // 
            // splitContainer_OrgRiskSetup
            // 
            this->splitContainer_OrgRiskSetup->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer_OrgRiskSetup->FixedPanel = System::Windows::Forms::FixedPanel::Panel2;
            this->splitContainer_OrgRiskSetup->IsSplitterFixed = true;
            this->splitContainer_OrgRiskSetup->Location = System::Drawing::Point(3, 3);
            this->splitContainer_OrgRiskSetup->Name = L"splitContainer_OrgRiskSetup";
            this->splitContainer_OrgRiskSetup->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer_OrgRiskSetup.Panel1
            // 
            this->splitContainer_OrgRiskSetup->Panel1->Controls->Add(this->dataGridView_OrgRiskSetup);
            // 
            // splitContainer_OrgRiskSetup.Panel2
            // 
            this->splitContainer_OrgRiskSetup->Panel2->Controls->Add(this->button_PlanModify);
            this->splitContainer_OrgRiskSetup->Panel2->Controls->Add(this->button_PlanDel);
            this->splitContainer_OrgRiskSetup->Panel2->Controls->Add(this->button_PlanAdd);
            this->splitContainer_OrgRiskSetup->Size = System::Drawing::Size(1159, 183);
            this->splitContainer_OrgRiskSetup->SplitterDistance = 154;
            this->splitContainer_OrgRiskSetup->TabIndex = 0;
            // 
            // dataGridView_OrgRiskSetup
            // 
            this->dataGridView_OrgRiskSetup->AllowUserToAddRows = false;
            this->dataGridView_OrgRiskSetup->AllowUserToDeleteRows = false;
            this->dataGridView_OrgRiskSetup->AllowUserToResizeRows = false;
            dataGridViewCellStyle14->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle14->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle14->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle14->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle14->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle14->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle14->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_OrgRiskSetup->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle14;
            this->dataGridView_OrgRiskSetup->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridView_OrgRiskSetup->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {this->Column55, 
                this->Column56, this->Column57});
            this->dataGridView_OrgRiskSetup->Dock = System::Windows::Forms::DockStyle::Fill;
            this->dataGridView_OrgRiskSetup->Location = System::Drawing::Point(0, 0);
            this->dataGridView_OrgRiskSetup->MultiSelect = false;
            this->dataGridView_OrgRiskSetup->Name = L"dataGridView_OrgRiskSetup";
            this->dataGridView_OrgRiskSetup->ReadOnly = true;
            this->dataGridView_OrgRiskSetup->RowHeadersVisible = false;
            this->dataGridView_OrgRiskSetup->RowTemplate->Height = 23;
            this->dataGridView_OrgRiskSetup->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->dataGridView_OrgRiskSetup->Size = System::Drawing::Size(1159, 154);
            this->dataGridView_OrgRiskSetup->TabIndex = 0;
            this->dataGridView_OrgRiskSetup->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &FormMain::dataGridView_OrgRiskSetup_CellContentClick);
            // 
            // Column55
            // 
            this->Column55->HeaderText = L"部门";
            this->Column55->Name = L"Column55";
            this->Column55->ReadOnly = true;
            // 
            // Column56
            // 
            this->Column56->HeaderText = L"风控方案";
            this->Column56->Name = L"Column56";
            this->Column56->ReadOnly = true;
            this->Column56->Width = 200;
            // 
            // Column57
            // 
            this->Column57->HeaderText = L"操作";
            this->Column57->Name = L"Column57";
            this->Column57->ReadOnly = true;
            this->Column57->Width = 80;
            // 
            // button_PlanModify
            // 
            this->button_PlanModify->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom));
            this->button_PlanModify->Location = System::Drawing::Point(121, 3);
            this->button_PlanModify->Name = L"button_PlanModify";
            this->button_PlanModify->Size = System::Drawing::Size(75, 23);
            this->button_PlanModify->TabIndex = 2;
            this->button_PlanModify->Text = L"修改方案";
            this->button_PlanModify->UseVisualStyleBackColor = true;
            this->button_PlanModify->Click += gcnew System::EventHandler(this, &FormMain::button_PlanModify_Click);
            // 
            // button_PlanDel
            // 
            this->button_PlanDel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom));
            this->button_PlanDel->Location = System::Drawing::Point(221, 3);
            this->button_PlanDel->Name = L"button_PlanDel";
            this->button_PlanDel->Size = System::Drawing::Size(75, 23);
            this->button_PlanDel->TabIndex = 1;
            this->button_PlanDel->Text = L"删除方案";
            this->button_PlanDel->UseVisualStyleBackColor = true;
            this->button_PlanDel->Click += gcnew System::EventHandler(this, &FormMain::button_PlanDel_Click);
            // 
            // button_PlanAdd
            // 
            this->button_PlanAdd->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom));
            this->button_PlanAdd->Location = System::Drawing::Point(16, 3);
            this->button_PlanAdd->Name = L"button_PlanAdd";
            this->button_PlanAdd->Size = System::Drawing::Size(75, 23);
            this->button_PlanAdd->TabIndex = 0;
            this->button_PlanAdd->Text = L"新增方案";
            this->button_PlanAdd->UseVisualStyleBackColor = true;
            this->button_PlanAdd->Click += gcnew System::EventHandler(this, &FormMain::button_PlanAdd_Click);
            // 
            // tabControl_Down
            // 
            this->tabControl_Down->Controls->Add(this->tabPage_AllOrder);
            this->tabControl_Down->Controls->Add(this->tabPage_UnTradeRecord);
            this->tabControl_Down->Controls->Add(this->tabPage_TradeRecord);
            this->tabControl_Down->Controls->Add(this->tabPage_OpenPosition);
            this->tabControl_Down->Controls->Add(this->tabPage_ClosePosition);
            this->tabControl_Down->Controls->Add(this->tabPage_VerifyOrder);
            this->tabControl_Down->Dock = System::Windows::Forms::DockStyle::Fill;
            this->tabControl_Down->Location = System::Drawing::Point(0, 0);
            this->tabControl_Down->Name = L"tabControl_Down";
            this->tabControl_Down->SelectedIndex = 0;
            this->tabControl_Down->Size = System::Drawing::Size(1173, 215);
            this->tabControl_Down->TabIndex = 0;
            // 
            // tabPage_AllOrder
            // 
            this->tabPage_AllOrder->Controls->Add(this->splitContainer_AllOrder);
            this->tabPage_AllOrder->Location = System::Drawing::Point(4, 21);
            this->tabPage_AllOrder->Name = L"tabPage_AllOrder";
            this->tabPage_AllOrder->Padding = System::Windows::Forms::Padding(3);
            this->tabPage_AllOrder->Size = System::Drawing::Size(1165, 190);
            this->tabPage_AllOrder->TabIndex = 0;
            this->tabPage_AllOrder->Text = L"所有委托单";
            this->tabPage_AllOrder->UseVisualStyleBackColor = true;
            // 
            // splitContainer_AllOrder
            // 
            this->splitContainer_AllOrder->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer_AllOrder->IsSplitterFixed = true;
            this->splitContainer_AllOrder->Location = System::Drawing::Point(3, 3);
            this->splitContainer_AllOrder->Name = L"splitContainer_AllOrder";
            this->splitContainer_AllOrder->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer_AllOrder.Panel1
            // 
            this->splitContainer_AllOrder->Panel1->Controls->Add(this->dataGridView_AllOrder);
            this->splitContainer_AllOrder->Panel1MinSize = 50;
            // 
            // splitContainer_AllOrder.Panel2
            // 
            this->splitContainer_AllOrder->Panel2->Controls->Add(this->bindingNavigator1);
            this->splitContainer_AllOrder->Panel2Collapsed = true;
            this->splitContainer_AllOrder->Size = System::Drawing::Size(1159, 184);
            this->splitContainer_AllOrder->SplitterDistance = 150;
            this->splitContainer_AllOrder->TabIndex = 0;
            // 
            // dataGridView_AllOrder
            // 
            this->dataGridView_AllOrder->AllowUserToAddRows = false;
            this->dataGridView_AllOrder->AllowUserToDeleteRows = false;
            this->dataGridView_AllOrder->AllowUserToResizeRows = false;
            dataGridViewCellStyle15->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle15->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle15->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle15->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle15->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle15->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle15->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_AllOrder->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle15;
            this->dataGridView_AllOrder->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
            dataGridViewCellStyle16->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle16->BackColor = System::Drawing::SystemColors::Window;
            dataGridViewCellStyle16->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle16->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle16->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle16->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle16->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridView_AllOrder->DefaultCellStyle = dataGridViewCellStyle16;
            this->dataGridView_AllOrder->Dock = System::Windows::Forms::DockStyle::Fill;
            this->dataGridView_AllOrder->Location = System::Drawing::Point(0, 0);
            this->dataGridView_AllOrder->MultiSelect = false;
            this->dataGridView_AllOrder->Name = L"dataGridView_AllOrder";
            this->dataGridView_AllOrder->ReadOnly = true;
            dataGridViewCellStyle17->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle17->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle17->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle17->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle17->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle17->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle17->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_AllOrder->RowHeadersDefaultCellStyle = dataGridViewCellStyle17;
            this->dataGridView_AllOrder->RowHeadersVisible = false;
            this->dataGridView_AllOrder->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
            this->dataGridView_AllOrder->RowTemplate->Height = 23;
            this->dataGridView_AllOrder->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->dataGridView_AllOrder->Size = System::Drawing::Size(1159, 184);
            this->dataGridView_AllOrder->TabIndex = 0;
            this->dataGridView_AllOrder->CellMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormMain::dataGridView_AllOrder_CellMouseClick);
            this->dataGridView_AllOrder->ColumnHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormMain::dataGridView_AllOrder_ColumnHeaderMouseClick);
            // 
            // bindingNavigator1
            // 
            this->bindingNavigator1->AddNewItem = nullptr;
            this->bindingNavigator1->CountItem = this->bindingNavigatorCountItem;
            this->bindingNavigator1->DeleteItem = nullptr;
            this->bindingNavigator1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(9) {this->bindingNavigatorMoveFirstItem, 
                this->bindingNavigatorMovePreviousItem, this->bindingNavigatorSeparator, this->bindingNavigatorPositionItem, this->bindingNavigatorCountItem, 
                this->bindingNavigatorSeparator1, this->bindingNavigatorMoveNextItem, this->bindingNavigatorMoveLastItem, this->bindingNavigatorSeparator2});
            this->bindingNavigator1->Location = System::Drawing::Point(0, 0);
            this->bindingNavigator1->MoveFirstItem = this->bindingNavigatorMoveFirstItem;
            this->bindingNavigator1->MoveLastItem = this->bindingNavigatorMoveLastItem;
            this->bindingNavigator1->MoveNextItem = this->bindingNavigatorMoveNextItem;
            this->bindingNavigator1->MovePreviousItem = this->bindingNavigatorMovePreviousItem;
            this->bindingNavigator1->Name = L"bindingNavigator1";
            this->bindingNavigator1->PositionItem = this->bindingNavigatorPositionItem;
            this->bindingNavigator1->Size = System::Drawing::Size(150, 25);
            this->bindingNavigator1->TabIndex = 0;
            this->bindingNavigator1->Text = L"bindingNavigator1";
            this->bindingNavigator1->Visible = false;
            // 
            // bindingNavigatorCountItem
            // 
            this->bindingNavigatorCountItem->Name = L"bindingNavigatorCountItem";
            this->bindingNavigatorCountItem->Size = System::Drawing::Size(41, 12);
            this->bindingNavigatorCountItem->Text = L"of {0}";
            // 
            // bindingNavigatorMoveFirstItem
            // 
            this->bindingNavigatorMoveFirstItem->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->bindingNavigatorMoveFirstItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"bindingNavigatorMoveFirstItem.Image")));
            this->bindingNavigatorMoveFirstItem->Name = L"bindingNavigatorMoveFirstItem";
            this->bindingNavigatorMoveFirstItem->RightToLeftAutoMirrorImage = true;
            this->bindingNavigatorMoveFirstItem->Size = System::Drawing::Size(23, 22);
            this->bindingNavigatorMoveFirstItem->Text = L"Move first";
            // 
            // bindingNavigatorMovePreviousItem
            // 
            this->bindingNavigatorMovePreviousItem->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->bindingNavigatorMovePreviousItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"bindingNavigatorMovePreviousItem.Image")));
            this->bindingNavigatorMovePreviousItem->Name = L"bindingNavigatorMovePreviousItem";
            this->bindingNavigatorMovePreviousItem->RightToLeftAutoMirrorImage = true;
            this->bindingNavigatorMovePreviousItem->Size = System::Drawing::Size(23, 22);
            this->bindingNavigatorMovePreviousItem->Text = L"Move previous";
            // 
            // bindingNavigatorSeparator
            // 
            this->bindingNavigatorSeparator->Name = L"bindingNavigatorSeparator";
            this->bindingNavigatorSeparator->Size = System::Drawing::Size(6, 25);
            // 
            // bindingNavigatorPositionItem
            // 
            this->bindingNavigatorPositionItem->AccessibleName = L"Position";
            this->bindingNavigatorPositionItem->AutoSize = false;
            this->bindingNavigatorPositionItem->Name = L"bindingNavigatorPositionItem";
            this->bindingNavigatorPositionItem->Size = System::Drawing::Size(50, 21);
            this->bindingNavigatorPositionItem->Text = L"0";
            // 
            // bindingNavigatorSeparator1
            // 
            this->bindingNavigatorSeparator1->Name = L"bindingNavigatorSeparator1";
            this->bindingNavigatorSeparator1->Size = System::Drawing::Size(6, 25);
            // 
            // bindingNavigatorMoveNextItem
            // 
            this->bindingNavigatorMoveNextItem->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->bindingNavigatorMoveNextItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"bindingNavigatorMoveNextItem.Image")));
            this->bindingNavigatorMoveNextItem->Name = L"bindingNavigatorMoveNextItem";
            this->bindingNavigatorMoveNextItem->RightToLeftAutoMirrorImage = true;
            this->bindingNavigatorMoveNextItem->Size = System::Drawing::Size(23, 20);
            this->bindingNavigatorMoveNextItem->Text = L"Move next";
            // 
            // bindingNavigatorMoveLastItem
            // 
            this->bindingNavigatorMoveLastItem->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->bindingNavigatorMoveLastItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"bindingNavigatorMoveLastItem.Image")));
            this->bindingNavigatorMoveLastItem->Name = L"bindingNavigatorMoveLastItem";
            this->bindingNavigatorMoveLastItem->RightToLeftAutoMirrorImage = true;
            this->bindingNavigatorMoveLastItem->Size = System::Drawing::Size(23, 20);
            this->bindingNavigatorMoveLastItem->Text = L"Move last";
            // 
            // bindingNavigatorSeparator2
            // 
            this->bindingNavigatorSeparator2->Name = L"bindingNavigatorSeparator2";
            this->bindingNavigatorSeparator2->Size = System::Drawing::Size(6, 25);
            // 
            // tabPage_UnTradeRecord
            // 
            this->tabPage_UnTradeRecord->Controls->Add(this->splitContainer_UnTradeRecord);
            this->tabPage_UnTradeRecord->Location = System::Drawing::Point(4, 21);
            this->tabPage_UnTradeRecord->Name = L"tabPage_UnTradeRecord";
            this->tabPage_UnTradeRecord->Padding = System::Windows::Forms::Padding(3);
            this->tabPage_UnTradeRecord->Size = System::Drawing::Size(1165, 190);
            this->tabPage_UnTradeRecord->TabIndex = 1;
            this->tabPage_UnTradeRecord->Text = L"未成交记录";
            this->tabPage_UnTradeRecord->UseVisualStyleBackColor = true;
            // 
            // splitContainer_UnTradeRecord
            // 
            this->splitContainer_UnTradeRecord->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer_UnTradeRecord->Location = System::Drawing::Point(3, 3);
            this->splitContainer_UnTradeRecord->Name = L"splitContainer_UnTradeRecord";
            this->splitContainer_UnTradeRecord->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer_UnTradeRecord.Panel1
            // 
            this->splitContainer_UnTradeRecord->Panel1->Controls->Add(this->dataGridView_UnTradeRecord);
            this->splitContainer_UnTradeRecord->Panel2Collapsed = true;
            this->splitContainer_UnTradeRecord->Size = System::Drawing::Size(1159, 184);
            this->splitContainer_UnTradeRecord->SplitterDistance = 155;
            this->splitContainer_UnTradeRecord->TabIndex = 0;
            // 
            // dataGridView_UnTradeRecord
            // 
            this->dataGridView_UnTradeRecord->AllowUserToAddRows = false;
            this->dataGridView_UnTradeRecord->AllowUserToDeleteRows = false;
            this->dataGridView_UnTradeRecord->AllowUserToResizeRows = false;
            this->dataGridView_UnTradeRecord->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
            this->dataGridView_UnTradeRecord->Dock = System::Windows::Forms::DockStyle::Fill;
            this->dataGridView_UnTradeRecord->Location = System::Drawing::Point(0, 0);
            this->dataGridView_UnTradeRecord->MultiSelect = false;
            this->dataGridView_UnTradeRecord->Name = L"dataGridView_UnTradeRecord";
            this->dataGridView_UnTradeRecord->ReadOnly = true;
            this->dataGridView_UnTradeRecord->RowHeadersVisible = false;
            this->dataGridView_UnTradeRecord->RowTemplate->Height = 23;
            this->dataGridView_UnTradeRecord->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->dataGridView_UnTradeRecord->Size = System::Drawing::Size(1159, 184);
            this->dataGridView_UnTradeRecord->TabIndex = 0;
            this->dataGridView_UnTradeRecord->CellMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormMain::dataGridView_UnTradeRecord_CellMouseClick);
            this->dataGridView_UnTradeRecord->ColumnHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormMain::dataGridView_UnTradeRecord_ColumnHeaderMouseClick);
            // 
            // tabPage_TradeRecord
            // 
            this->tabPage_TradeRecord->Controls->Add(this->splitContainer_TradeRecord);
            this->tabPage_TradeRecord->Location = System::Drawing::Point(4, 21);
            this->tabPage_TradeRecord->Name = L"tabPage_TradeRecord";
            this->tabPage_TradeRecord->Size = System::Drawing::Size(1165, 190);
            this->tabPage_TradeRecord->TabIndex = 2;
            this->tabPage_TradeRecord->Text = L"成交记录";
            this->tabPage_TradeRecord->UseVisualStyleBackColor = true;
            // 
            // splitContainer_TradeRecord
            // 
            this->splitContainer_TradeRecord->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer_TradeRecord->Location = System::Drawing::Point(0, 0);
            this->splitContainer_TradeRecord->Name = L"splitContainer_TradeRecord";
            this->splitContainer_TradeRecord->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer_TradeRecord.Panel1
            // 
            this->splitContainer_TradeRecord->Panel1->Controls->Add(this->dataGridView_TradeRecord);
            this->splitContainer_TradeRecord->Panel2Collapsed = true;
            this->splitContainer_TradeRecord->Size = System::Drawing::Size(1165, 190);
            this->splitContainer_TradeRecord->SplitterDistance = 161;
            this->splitContainer_TradeRecord->TabIndex = 0;
            // 
            // dataGridView_TradeRecord
            // 
            this->dataGridView_TradeRecord->AllowUserToAddRows = false;
            this->dataGridView_TradeRecord->AllowUserToDeleteRows = false;
            dataGridViewCellStyle18->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle18->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle18->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle18->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle18->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle18->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle18->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_TradeRecord->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle18;
            this->dataGridView_TradeRecord->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
            dataGridViewCellStyle19->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle19->BackColor = System::Drawing::SystemColors::Window;
            dataGridViewCellStyle19->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle19->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle19->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle19->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle19->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridView_TradeRecord->DefaultCellStyle = dataGridViewCellStyle19;
            this->dataGridView_TradeRecord->Dock = System::Windows::Forms::DockStyle::Fill;
            this->dataGridView_TradeRecord->Location = System::Drawing::Point(0, 0);
            this->dataGridView_TradeRecord->MultiSelect = false;
            this->dataGridView_TradeRecord->Name = L"dataGridView_TradeRecord";
            this->dataGridView_TradeRecord->ReadOnly = true;
            dataGridViewCellStyle20->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle20->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle20->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle20->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle20->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle20->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle20->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_TradeRecord->RowHeadersDefaultCellStyle = dataGridViewCellStyle20;
            this->dataGridView_TradeRecord->RowHeadersVisible = false;
            this->dataGridView_TradeRecord->RowTemplate->Height = 23;
            this->dataGridView_TradeRecord->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->dataGridView_TradeRecord->Size = System::Drawing::Size(1165, 190);
            this->dataGridView_TradeRecord->TabIndex = 0;
            this->dataGridView_TradeRecord->CellMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormMain::dataGridView_TradeRecord_CellMouseClick);
            this->dataGridView_TradeRecord->ColumnHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormMain::dataGridView_TradeRecord_ColumnHeaderMouseClick);
            // 
            // tabPage_OpenPosition
            // 
            this->tabPage_OpenPosition->Controls->Add(this->splitContainer_OpenPosition);
            this->tabPage_OpenPosition->Location = System::Drawing::Point(4, 21);
            this->tabPage_OpenPosition->Name = L"tabPage_OpenPosition";
            this->tabPage_OpenPosition->Size = System::Drawing::Size(1165, 190);
            this->tabPage_OpenPosition->TabIndex = 3;
            this->tabPage_OpenPosition->Text = L"持仓";
            this->tabPage_OpenPosition->UseVisualStyleBackColor = true;
            // 
            // splitContainer_OpenPosition
            // 
            this->splitContainer_OpenPosition->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer_OpenPosition->Location = System::Drawing::Point(0, 0);
            this->splitContainer_OpenPosition->Name = L"splitContainer_OpenPosition";
            this->splitContainer_OpenPosition->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer_OpenPosition.Panel1
            // 
            this->splitContainer_OpenPosition->Panel1->Controls->Add(this->dataGridView_Position);
            // 
            // splitContainer_OpenPosition.Panel2
            // 
            this->splitContainer_OpenPosition->Panel2->Controls->Add(this->radioButton_PositionDetail);
            this->splitContainer_OpenPosition->Panel2->Controls->Add(this->radioButton_Position);
            this->splitContainer_OpenPosition->Panel2Collapsed = true;
            this->splitContainer_OpenPosition->Size = System::Drawing::Size(1165, 190);
            this->splitContainer_OpenPosition->SplitterDistance = 161;
            this->splitContainer_OpenPosition->TabIndex = 0;
            // 
            // dataGridView_Position
            // 
            this->dataGridView_Position->AllowUserToAddRows = false;
            this->dataGridView_Position->AllowUserToDeleteRows = false;
            this->dataGridView_Position->AllowUserToResizeRows = false;
            dataGridViewCellStyle21->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle21->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle21->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle21->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle21->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle21->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle21->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_Position->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle21;
            this->dataGridView_Position->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
            dataGridViewCellStyle22->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle22->BackColor = System::Drawing::SystemColors::Window;
            dataGridViewCellStyle22->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle22->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle22->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle22->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle22->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridView_Position->DefaultCellStyle = dataGridViewCellStyle22;
            this->dataGridView_Position->Dock = System::Windows::Forms::DockStyle::Fill;
            this->dataGridView_Position->Location = System::Drawing::Point(0, 0);
            this->dataGridView_Position->MultiSelect = false;
            this->dataGridView_Position->Name = L"dataGridView_Position";
            this->dataGridView_Position->ReadOnly = true;
            dataGridViewCellStyle23->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle23->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle23->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle23->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle23->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle23->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle23->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_Position->RowHeadersDefaultCellStyle = dataGridViewCellStyle23;
            this->dataGridView_Position->RowHeadersVisible = false;
            this->dataGridView_Position->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
            this->dataGridView_Position->RowTemplate->Height = 23;
            this->dataGridView_Position->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->dataGridView_Position->ShowCellToolTips = false;
            this->dataGridView_Position->Size = System::Drawing::Size(1165, 190);
            this->dataGridView_Position->TabIndex = 0;
            this->dataGridView_Position->CellMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormMain::dataGridView_Position_CellMouseClick);
            this->dataGridView_Position->ColumnHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormMain::dataGridView_Position_ColumnHeaderMouseClick);
            // 
            // radioButton_PositionDetail
            // 
            this->radioButton_PositionDetail->AutoSize = true;
            this->radioButton_PositionDetail->Location = System::Drawing::Point(151, 6);
            this->radioButton_PositionDetail->Name = L"radioButton_PositionDetail";
            this->radioButton_PositionDetail->Size = System::Drawing::Size(71, 16);
            this->radioButton_PositionDetail->TabIndex = 1;
            this->radioButton_PositionDetail->Text = L"持仓明细";
            this->radioButton_PositionDetail->UseVisualStyleBackColor = true;
            // 
            // radioButton_Position
            // 
            this->radioButton_Position->AutoSize = true;
            this->radioButton_Position->Checked = true;
            this->radioButton_Position->Location = System::Drawing::Point(27, 6);
            this->radioButton_Position->Name = L"radioButton_Position";
            this->radioButton_Position->Size = System::Drawing::Size(47, 16);
            this->radioButton_Position->TabIndex = 0;
            this->radioButton_Position->TabStop = true;
            this->radioButton_Position->Text = L"持仓";
            this->radioButton_Position->UseVisualStyleBackColor = true;
            this->radioButton_Position->CheckedChanged += gcnew System::EventHandler(this, &FormMain::radioButton_Position_CheckedChanged);
            // 
            // tabPage_ClosePosition
            // 
            this->tabPage_ClosePosition->Controls->Add(this->splitContainer_ClosePosition);
            this->tabPage_ClosePosition->Location = System::Drawing::Point(4, 21);
            this->tabPage_ClosePosition->Name = L"tabPage_ClosePosition";
            this->tabPage_ClosePosition->Size = System::Drawing::Size(1165, 190);
            this->tabPage_ClosePosition->TabIndex = 4;
            this->tabPage_ClosePosition->Text = L"平仓";
            this->tabPage_ClosePosition->UseVisualStyleBackColor = true;
            // 
            // splitContainer_ClosePosition
            // 
            this->splitContainer_ClosePosition->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer_ClosePosition->Location = System::Drawing::Point(0, 0);
            this->splitContainer_ClosePosition->Name = L"splitContainer_ClosePosition";
            this->splitContainer_ClosePosition->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer_ClosePosition.Panel1
            // 
            this->splitContainer_ClosePosition->Panel1->Controls->Add(this->dataGridView_ClosePosition);
            this->splitContainer_ClosePosition->Panel2Collapsed = true;
            this->splitContainer_ClosePosition->Size = System::Drawing::Size(1165, 190);
            this->splitContainer_ClosePosition->SplitterDistance = 161;
            this->splitContainer_ClosePosition->TabIndex = 0;
            // 
            // dataGridView_ClosePosition
            // 
            this->dataGridView_ClosePosition->AllowUserToAddRows = false;
            this->dataGridView_ClosePosition->AllowUserToDeleteRows = false;
            dataGridViewCellStyle24->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle24->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle24->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle24->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle24->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle24->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle24->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_ClosePosition->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle24;
            this->dataGridView_ClosePosition->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
            dataGridViewCellStyle25->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle25->BackColor = System::Drawing::SystemColors::Window;
            dataGridViewCellStyle25->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle25->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle25->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle25->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle25->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridView_ClosePosition->DefaultCellStyle = dataGridViewCellStyle25;
            this->dataGridView_ClosePosition->Dock = System::Windows::Forms::DockStyle::Fill;
            this->dataGridView_ClosePosition->Location = System::Drawing::Point(0, 0);
            this->dataGridView_ClosePosition->MultiSelect = false;
            this->dataGridView_ClosePosition->Name = L"dataGridView_ClosePosition";
            this->dataGridView_ClosePosition->ReadOnly = true;
            dataGridViewCellStyle26->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle26->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle26->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle26->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle26->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle26->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle26->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_ClosePosition->RowHeadersDefaultCellStyle = dataGridViewCellStyle26;
            this->dataGridView_ClosePosition->RowHeadersVisible = false;
            this->dataGridView_ClosePosition->RowTemplate->Height = 23;
            this->dataGridView_ClosePosition->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->dataGridView_ClosePosition->Size = System::Drawing::Size(1165, 190);
            this->dataGridView_ClosePosition->TabIndex = 0;
            this->dataGridView_ClosePosition->CellMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormMain::dataGridView_ClosePosition_CellMouseClick);
            this->dataGridView_ClosePosition->ColumnHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormMain::dataGridView_ClosePosition_ColumnHeaderMouseClick);
            // 
            // tabPage_VerifyOrder
            // 
            this->tabPage_VerifyOrder->Controls->Add(this->splitContainer_NeedVerify);
            this->tabPage_VerifyOrder->Location = System::Drawing::Point(4, 21);
            this->tabPage_VerifyOrder->Name = L"tabPage_VerifyOrder";
            this->tabPage_VerifyOrder->Size = System::Drawing::Size(1165, 190);
            this->tabPage_VerifyOrder->TabIndex = 5;
            this->tabPage_VerifyOrder->Text = L"待审核单";
            this->tabPage_VerifyOrder->UseVisualStyleBackColor = true;
            // 
            // splitContainer_NeedVerify
            // 
            this->splitContainer_NeedVerify->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer_NeedVerify->FixedPanel = System::Windows::Forms::FixedPanel::Panel2;
            this->splitContainer_NeedVerify->IsSplitterFixed = true;
            this->splitContainer_NeedVerify->Location = System::Drawing::Point(0, 0);
            this->splitContainer_NeedVerify->Name = L"splitContainer_NeedVerify";
            this->splitContainer_NeedVerify->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer_NeedVerify.Panel1
            // 
            this->splitContainer_NeedVerify->Panel1->Controls->Add(this->dataGridView_NeedVerify);
            // 
            // splitContainer_NeedVerify.Panel2
            // 
            this->splitContainer_NeedVerify->Panel2->Controls->Add(this->button_ForbidOrder);
            this->splitContainer_NeedVerify->Panel2->Controls->Add(this->button_PassVerify);
            this->splitContainer_NeedVerify->Size = System::Drawing::Size(1165, 190);
            this->splitContainer_NeedVerify->SplitterDistance = 161;
            this->splitContainer_NeedVerify->TabIndex = 0;
            // 
            // dataGridView_NeedVerify
            // 
            this->dataGridView_NeedVerify->AllowUserToAddRows = false;
            this->dataGridView_NeedVerify->AllowUserToDeleteRows = false;
            dataGridViewCellStyle27->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle27->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle27->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle27->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle27->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle27->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle27->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_NeedVerify->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle27;
            this->dataGridView_NeedVerify->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
            dataGridViewCellStyle28->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle28->BackColor = System::Drawing::SystemColors::Window;
            dataGridViewCellStyle28->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle28->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle28->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle28->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle28->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridView_NeedVerify->DefaultCellStyle = dataGridViewCellStyle28;
            this->dataGridView_NeedVerify->Dock = System::Windows::Forms::DockStyle::Fill;
            this->dataGridView_NeedVerify->Location = System::Drawing::Point(0, 0);
            this->dataGridView_NeedVerify->MultiSelect = false;
            this->dataGridView_NeedVerify->Name = L"dataGridView_NeedVerify";
            this->dataGridView_NeedVerify->ReadOnly = true;
            dataGridViewCellStyle29->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle29->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle29->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle29->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle29->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle29->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle29->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_NeedVerify->RowHeadersDefaultCellStyle = dataGridViewCellStyle29;
            this->dataGridView_NeedVerify->RowHeadersVisible = false;
            this->dataGridView_NeedVerify->RowTemplate->Height = 23;
            this->dataGridView_NeedVerify->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->dataGridView_NeedVerify->Size = System::Drawing::Size(1165, 161);
            this->dataGridView_NeedVerify->TabIndex = 0;
            this->dataGridView_NeedVerify->CellMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormMain::dataGridView_NeedVerify_CellMouseClick);
            this->dataGridView_NeedVerify->ColumnHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormMain::dataGridView_NeedVerify_ColumnHeaderMouseClick);
            // 
            // button_ForbidOrder
            // 
            this->button_ForbidOrder->Location = System::Drawing::Point(94, 0);
            this->button_ForbidOrder->Name = L"button_ForbidOrder";
            this->button_ForbidOrder->Size = System::Drawing::Size(75, 23);
            this->button_ForbidOrder->TabIndex = 0;
            this->button_ForbidOrder->Text = L"禁止下单";
            this->button_ForbidOrder->Click += gcnew System::EventHandler(this, &FormMain::button_ForbidOrder_Click);
            // 
            // button_PassVerify
            // 
            this->button_PassVerify->Location = System::Drawing::Point(0, 0);
            this->button_PassVerify->Name = L"button_PassVerify";
            this->button_PassVerify->Size = System::Drawing::Size(75, 23);
            this->button_PassVerify->TabIndex = 1;
            this->button_PassVerify->Text = L"通过审核";
            this->button_PassVerify->Click += gcnew System::EventHandler(this, &FormMain::button_PassVerify_Click);
            // 
            // Column1
            // 
            this->Column1->Name = L"Column1";
            // 
            // timer_FormMain
            // 
            this->timer_FormMain->Interval = 200;
            this->timer_FormMain->Tick += gcnew System::EventHandler(this, &FormMain::timer_FormMain_Tick);
            // 
            // contextMenuStrip_Sub
            // 
            this->contextMenuStrip_Sub->Name = L"contextMenuStrip_Sub";
            this->contextMenuStrip_Sub->Size = System::Drawing::Size(61, 4);
            // 
            // cntMenuOriganTree
            // 
            this->cntMenuOriganTree->Name = L"cntMenuOriganTree";
            this->cntMenuOriganTree->Size = System::Drawing::Size(61, 4);
            // 
            // 发送消息ToolStripMenuItem
            // 
            this->发送消息ToolStripMenuItem->Name = L"发送消息ToolStripMenuItem";
            this->发送消息ToolStripMenuItem->Size = System::Drawing::Size(118, 22);
            this->发送消息ToolStripMenuItem->Text = L"发送消息";
            this->发送消息ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::发送消息ToolStripMenuItem_Click);
            // 
            // 交易限制ToolStripMenuItem
            // 
            this->交易限制ToolStripMenuItem->Name = L"交易限制ToolStripMenuItem";
            this->交易限制ToolStripMenuItem->Size = System::Drawing::Size(118, 22);
            this->交易限制ToolStripMenuItem->Text = L"交易限制";
            this->交易限制ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_TradeLimit_Click);
            // 
            // 取消限制ToolStripMenuItem
            // 
            this->取消限制ToolStripMenuItem->Name = L"取消限制ToolStripMenuItem";
            this->取消限制ToolStripMenuItem->Size = System::Drawing::Size(118, 22);
            this->取消限制ToolStripMenuItem->Text = L"取消限制";
            this->取消限制ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_CancelLimit_Click);
            // 
            // 手动审核ToolStripMenuItem
            // 
            this->手动审核ToolStripMenuItem->Name = L"手动审核ToolStripMenuItem";
            this->手动审核ToolStripMenuItem->Size = System::Drawing::Size(118, 22);
            this->手动审核ToolStripMenuItem->Text = L"手动审核";
            this->手动审核ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_ManualVerify_Click);
            // 
            // 取消审核ToolStripMenuItem
            // 
            this->取消审核ToolStripMenuItem->Name = L"取消审核ToolStripMenuItem";
            this->取消审核ToolStripMenuItem->Size = System::Drawing::Size(118, 22);
            this->取消审核ToolStripMenuItem->Text = L"取消审核";
            this->取消审核ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_CancelVerify_Click);
            // 
            // FormMain
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->AutoSize = true;
            this->ClientSize = System::Drawing::Size(1334, 643);
            this->Controls->Add(this->splitContainer_LeftRight);
            this->Controls->Add(this->statusStrip);
            this->Controls->Add(this->menuStrip1);
            this->MainMenuStrip = this->menuStrip1;
            this->Name = L"FormMain";
            this->Text = L"风控终端";
            this->Load += gcnew System::EventHandler(this, &FormMain::FormMain_Load);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FormMain::FormMain_FormClosed);
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormMain::FormMain_FormClosing);
            this->menuStrip1->ResumeLayout(false);
            this->menuStrip1->PerformLayout();
            this->statusStrip->ResumeLayout(false);
            this->statusStrip->PerformLayout();
            this->splitContainer_LeftRight->Panel1->ResumeLayout(false);
            this->splitContainer_LeftRight->Panel2->ResumeLayout(false);
            this->splitContainer_LeftRight->ResumeLayout(false);
            this->tabControl_Organization->ResumeLayout(false);
            this->tabPage_Organization->ResumeLayout(false);
            this->splitContainer_MiddleDown->Panel1->ResumeLayout(false);
            this->splitContainer_MiddleDown->Panel2->ResumeLayout(false);
            this->splitContainer_MiddleDown->ResumeLayout(false);
            this->splitContainer_UpMiddle->Panel1->ResumeLayout(false);
            this->splitContainer_UpMiddle->Panel2->ResumeLayout(false);
            this->splitContainer_UpMiddle->ResumeLayout(false);
            this->tabControl_Up->ResumeLayout(false);
            this->tabPage_QuotOrderTable->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_QuotOrderTable))->EndInit();
            this->tabPage_RiskEvent->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_RiskEvent))->EndInit();
            this->tabControl_Middle->ResumeLayout(false);
            this->tabPage_AccountFundInfo->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_AccountFundInfo))->EndInit();
            this->tabPage_OrgRiskSetup->ResumeLayout(false);
            this->splitContainer_OrgRiskSetup->Panel1->ResumeLayout(false);
            this->splitContainer_OrgRiskSetup->Panel2->ResumeLayout(false);
            this->splitContainer_OrgRiskSetup->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_OrgRiskSetup))->EndInit();
            this->tabControl_Down->ResumeLayout(false);
            this->tabPage_AllOrder->ResumeLayout(false);
            this->splitContainer_AllOrder->Panel1->ResumeLayout(false);
            this->splitContainer_AllOrder->Panel2->ResumeLayout(false);
            this->splitContainer_AllOrder->Panel2->PerformLayout();
            this->splitContainer_AllOrder->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_AllOrder))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->bindingNavigator1))->EndInit();
            this->bindingNavigator1->ResumeLayout(false);
            this->bindingNavigator1->PerformLayout();
            this->tabPage_UnTradeRecord->ResumeLayout(false);
            this->splitContainer_UnTradeRecord->Panel1->ResumeLayout(false);
            this->splitContainer_UnTradeRecord->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_UnTradeRecord))->EndInit();
            this->tabPage_TradeRecord->ResumeLayout(false);
            this->splitContainer_TradeRecord->Panel1->ResumeLayout(false);
            this->splitContainer_TradeRecord->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_TradeRecord))->EndInit();
            this->tabPage_OpenPosition->ResumeLayout(false);
            this->splitContainer_OpenPosition->Panel1->ResumeLayout(false);
            this->splitContainer_OpenPosition->Panel2->ResumeLayout(false);
            this->splitContainer_OpenPosition->Panel2->PerformLayout();
            this->splitContainer_OpenPosition->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_Position))->EndInit();
            this->tabPage_ClosePosition->ResumeLayout(false);
            this->splitContainer_ClosePosition->Panel1->ResumeLayout(false);
            this->splitContainer_ClosePosition->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_ClosePosition))->EndInit();
            this->tabPage_VerifyOrder->ResumeLayout(false);
            this->splitContainer_NeedVerify->Panel1->ResumeLayout(false);
            this->splitContainer_NeedVerify->Panel2->ResumeLayout(false);
            this->splitContainer_NeedVerify->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_NeedVerify))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion


private:

        sLoginRsp* m_pLogin;

        System::Data::DataTable^  m_DataTableDepthMarketData;//行情

        System::Data::DataTable^  m_DataTableRiskEvent;//风险事件

        System::Data::DataTable^  m_DataTableTradingAccount;//账户资金

        System::Data::DataTable^  m_DataTableAllOrder;//委托单
        System::Data::DataView^   m_DataViewUnTrade;//未成交记录
        System::Data::DataView^   m_DataViewNeedVerify;//待审核单

        System::Data::DataTable^  m_DataTableTrade;//成交记录
        System::Data::DataView^   m_DataViewClosePosition;//平仓
        
        System::Data::DataTable^ m_DataTablePosition;//持仓

        FormLockWindow^ m_FormLockWindowDlg;
        FormForceClose^ m_FormForceClose;
        FormRiskPlanSetup_Add^ m_FormRiskPlanSetup_Add;

        System::Windows::Forms::DataGridView^ m_activeDataGridView;//当前活跃的控件
        System::Data::DataTable^              m_activeDataTable;//当前活跃的DataTable


        cliext::vector<String^>  m_vecTraderAccountFromTree;
        cliext::set<int> m_setRiskEventID;

        bool m_bLoad;

        int     m_nCurrColumnHeaderIndex_MouseRight;
        String^ m_CurrDataGridViewText;
        String^ m_CurrColumnHeaderText;
        String^ m_currInstrumentText;


        void Update_dataGridView_AccountFundInfo(const SLimitTrade* pLimitTrade)
        {
            UserInfo userInfo;
            if(CRiskData::GetInstance()->GetUserInfoByID(pLimitTrade->nUserID,userInfo))
            {
                for(int i=0;i<dataGridView_AccountFundInfo->RowCount;i++)
                {
                    String^ szUserName = dataGridView_AccountFundInfo->Rows[i]->Cells["交易员"]->Value->ToString();
                    if(szUserName->CompareTo(Tools::string2String(userInfo.szAccount))==0)                    
                        dataGridView_AccountFundInfo->Rows[i]->Cells["交易限制"]->Value = (pLimitTrade->bLimitTrade)?"是":"否";
                }
            }
        }
        void Update_dataGridView_AccountFundInfo(const SManualVerify* pSManualVerify)
        {
            UserInfo userInfo;
            if(CRiskData::GetInstance()->GetUserInfoByID(pSManualVerify->nUserID,userInfo))
            {
                for(int i=0;i<dataGridView_AccountFundInfo->RowCount;i++)
                {
                    String^ szUserName = dataGridView_AccountFundInfo->Rows[i]->Cells["交易员"]->Value->ToString();
                    if(szUserName->CompareTo(Tools::string2String(userInfo.szAccount))==0)                    
                        dataGridView_AccountFundInfo->Rows[i]->Cells["手动审核"]->Value = (pSManualVerify->bManualVerify)?"是":"否";
                }
            }
        }
        void Update_dataGridView_AccountFundInfo(map<string,bool>& mapLogin)
        {
            if(mapLogin.empty())
                return;

            String^ szLoginStatus;
            for(int i=0;i<dataGridView_AccountFundInfo->RowCount;i++)
            {
                std::string szUserName = Tools::String2string(dataGridView_AccountFundInfo->Rows[i]->Cells["交易员"]->Value->ToString());
                map<string,bool>::iterator it=mapLogin.find(szUserName);
                if(it==mapLogin.end())
                    szLoginStatus = "未登录"; 
                else
                    szLoginStatus = "已登录";
                
                dataGridView_AccountFundInfo->Rows[i]->Cells["登录状态"]->Value = szLoginStatus;
            }            
        }
        void Update_dataGridView_AccountFundInfo(const UserAndTradeAccountRelation* pRelation)
        {
            UserInfo userInfo;
            if(CRiskData::GetInstance()->GetUserInfoByID(pRelation->nUserID,userInfo))
            {
                for(int i=0;i<dataGridView_AccountFundInfo->RowCount;i++)
                {
                    String^ szUserName = dataGridView_AccountFundInfo->Rows[i]->Cells["交易员"]->Value->ToString();
                    if(szUserName->CompareTo(Tools::string2String(userInfo.szAccount))==0)                    
                    {
                        dataGridView_AccountFundInfo->Rows[i]->Cells["交易限制"]->Value = (0==pRelation->nCanInputOrder)?"是":"否";
                        dataGridView_AccountFundInfo->Rows[i]->Cells["手动审核"]->Value = (0==pRelation->nNeedVerify)?"否":"是";
                    }
                }
            }
        }
        double GetLastPrice(char* szInstID)
        {
            for(int i=0;i<dataGridView_QuotOrderTable->RowCount;i++)
            {
                String^ szInstName = dataGridView_QuotOrderTable->Rows[i]->Cells["合约"]->Value->ToString();
                if(szInstName->CompareTo(Tools::string2String(szInstID))==0)
                    return Convert::ToDouble(dataGridView_QuotOrderTable->Rows[i]->Cells["最新价"]->Value->ToString());
            }

            //报价表没有，但是持仓已订阅，信息保存在CRiskData
            double fPriceTick = CRiskData::GetInstance()->GetPriceTickByInstID(szInstID);

            sInstrumentInfo instrumentInfo;
            CRiskData::GetInstance()->GetInstrumentInfo((string)szInstID,instrumentInfo);
            return Convert::ToDouble(Tools::string2String(Price2string(instrumentInfo.fLastPrice,fPriceTick).c_str()));
        }
        String^ GetUpperLimitPrice(char* szInstID)
        {
            double fPriceTick = CRiskData::GetInstance()->GetPriceTickByInstID(szInstID);

            for(int i=0;i<dataGridView_QuotOrderTable->RowCount;i++)
            {
                String^ szInstName = dataGridView_QuotOrderTable->Rows[i]->Cells["合约"]->Value->ToString();
                if(szInstName->CompareTo(Tools::string2String(szInstID))==0)
                    return Tools::string2String(Price2string(Convert::ToDouble(dataGridView_QuotOrderTable->Rows[i]->Cells["涨停价"]->Value->ToString()),fPriceTick).c_str());
            }

            //报价表没有，但是持仓已订阅，信息保存在CRiskData
            sInstrumentInfo instrumentInfo;
            CRiskData::GetInstance()->GetInstrumentInfo((string)szInstID,instrumentInfo);
            return Tools::string2String(Price2string(instrumentInfo.fUpperLimitPrice,fPriceTick).c_str());

        }
        String^ GetLowerLimitPrice(char* szInstID)
        {
            double fPriceTick = CRiskData::GetInstance()->GetPriceTickByInstID(szInstID);

            for(int i=0;i<dataGridView_QuotOrderTable->RowCount;i++)
            {
                String^ szInstName = dataGridView_QuotOrderTable->Rows[i]->Cells["合约"]->Value->ToString();
                if(szInstName->CompareTo(Tools::string2String(szInstID))==0)
                    return Tools::string2String(Price2string(Convert::ToDouble(dataGridView_QuotOrderTable->Rows[i]->Cells["跌停价"]->Value->ToString()),fPriceTick).c_str());
            }

            //报价表没有，但是持仓已订阅，信息保存在CRiskData
            sInstrumentInfo instrumentInfo;
            CRiskData::GetInstance()->GetInstrumentInfo((string)szInstID,instrumentInfo);
            return Tools::string2String(Price2string(instrumentInfo.fLowerLimitPrice,fPriceTick).c_str());
        }
        void GetUserIDFormOrgTreeNode(TreeNode^ tn,vector<int>& vecUserID)    
        {          
            if(Regex::IsMatch(tn->Text, "^[0-9]*$"))
            {
                int nUserID = (int)(tn->Tag);
                vecUserID.push_back(nUserID);
            }

            if (tn->Nodes->Count == 0) 
                return;       

            for each(TreeNode^ children in tn->Nodes)  
                GetUserIDFormOrgTreeNode(children,vecUserID);  
        }
        int GetTagFromText(String^ szText)
         {
             if(szText->CompareTo("合约")==0)           return conCol_DepthMarket_InstrumentID;//PrimaryKey
             if(szText->CompareTo("最新价")==0)         return conCol_DepthMarket_LastPrice;
             if(szText->CompareTo("涨跌")==0)           return conCol_DepthMarket_Mixed;
             if(szText->CompareTo("涨跌幅")==0)         return conCol_DepthMarket_MixedRange;
             if(szText->CompareTo("买价")==0)           return conCol_DepthMarket_BidPrice1;
             if(szText->CompareTo("买量")==0)           return conCol_DepthMarket_BidVolume1;
             if(szText->CompareTo("卖价")==0)           return conCol_DepthMarket_AskPrice1;
             if(szText->CompareTo("卖量")==0)           return conCol_DepthMarket_AskVolume1;
             if(szText->CompareTo("成交量")==0)         return conCol_DepthMarket_Volume;
             if(szText->CompareTo("持仓量")==0)         return conCol_DepthMarket_OpenInterest;
             if(szText->CompareTo("现量")==0)           return conCol_DepthMarket_CurVolume;
             if(szText->CompareTo("今开盘")==0)         return conCol_DepthMarket_OpenPrice;
             if(szText->CompareTo("昨结算")==0)         return conCol_DepthMarket_SettlementPrice;
             if(szText->CompareTo("最高价")==0)         return conCol_DepthMarket_HighestPrice;
             if(szText->CompareTo("最低价")==0)         return conCol_DepthMarket_LowestPrice;
             if(szText->CompareTo("涨停价")==0)         return conCol_DepthMarket_UpperLimitPrice;
             if(szText->CompareTo("跌停价")==0)         return conCol_DepthMarket_LowerLimitPrice;
             if(szText->CompareTo("更新时间")==0)       return conCol_DepthMarket_UpdateTime;

             if(szText->CompareTo("风险事件ID")==0)     return conCol_RiskEvent_RiskEventID;//PrimaryKey
             if(szText->CompareTo("交易员ID")==0)       return conCol_RiskEvent_TradeInvestorID;
             if(szText->CompareTo("交易员名称")==0)     return conCol_RiskEvent_TraderName;
             if(szText->CompareTo("风险指标类型")==0)   return conCol_RiskEvent_IndicatorName;
             if(szText->CompareTo("风险详情")==0)       return conCol_RiskEvent_RiskDesc;
             if(szText->CompareTo("更新时间")==0)       return conCol_RiskEvent_EventTime;
             if(szText->CompareTo("风险事件状态")==0)   return conCol_RiskEvent_EventStatus;

             if(szText->CompareTo("交易员")==0)         return conCol_TradingAccount_Account;//PrimaryKey
             if(szText->CompareTo("交易限制")==0)       return conCol_TradingAccount_LimitTade;
             if(szText->CompareTo("手动审核")==0)       return conCol_TradingAccount_ManuVerify;
             if(szText->CompareTo("登录状态")==0)       return conCol_TradingAccount_LoginStatus;
             if(szText->CompareTo("风险度(%)")==0)      return conCol_TradingAccount_RiskDegree;
             if(szText->CompareTo("动态权益")==0)       return conCol_TradingAccount_DynamicProfit;
             if(szText->CompareTo("持仓盈亏")==0)       return conCol_TradingAccount_PositionProfit;
             if(szText->CompareTo("平仓盈亏")==0)       return conCol_TradingAccount_CloseProfit;
             if(szText->CompareTo("占用保证金")==0)     return conCol_TradingAccount_CurrMargin;
             if(szText->CompareTo("手续费")==0)         return conCol_TradingAccount_Commission;
             if(szText->CompareTo("冻结保证金")==0)     return conCol_TradingAccount_FrozenMargin;
             if(szText->CompareTo("冻结手续费")==0)     return conCol_TradingAccount_FrozenCommission;

             if(szText->CompareTo("交易员")==0)         return conCol_AllOrder_InvestorID;//PrimaryKey
             if(szText->CompareTo("报单编号")==0)       return conCol_AllOrder_OrderSysID;
             if(szText->CompareTo("合约")==0)           return conCol_AllOrder_InstrumentID;//PrimaryKey
             if(szText->CompareTo("买卖")==0)           return conCol_AllOrder_Direction;
             if(szText->CompareTo("开平")==0)           return conCol_AllOrder_CombOffsetFlag;
             if(szText->CompareTo("挂单状态")==0)       return conCol_AllOrder_OrderStatus;
             if(szText->CompareTo("报单价格")==0)       return conCol_AllOrder_LimitPrice;
             if(szText->CompareTo("报单手数")==0)       return conCol_AllOrder_VolumeTotalOriginal;
             if(szText->CompareTo("未成交手数")==0)     return conCol_AllOrder_VolumeTotal;
             if(szText->CompareTo("成交手数")==0)       return conCol_AllOrder_Volume;
             if(szText->CompareTo("成交手数")==0)       return conCol_AllOrder_Volume;
             if(szText->CompareTo("成交手数")==0)       return conCol_AllOrder_Volume;
             if(szText->CompareTo("详细状态")==0)       return conCol_AllOrder_StatusMsg;
             if(szText->CompareTo("报单时间")==0)       return conCol_AllOrder_InsertTime;
             if(szText->CompareTo("冻结保证金")==0)     return conCol_AllOrder_freezeMargin;
             if(szText->CompareTo("冻结手续费")==0)     return conCol_AllOrder_troubleMoney;
             if(szText->CompareTo("投保")==0)           return conCol_AllOrder_HedgeFlag;
             if(szText->CompareTo("交易所")==0)         return conCol_AllOrder_ExchangeID;
             if(szText->CompareTo("前置机")==0)         return conCol_AllOrder_FrontID;//PrimaryKey
             if(szText->CompareTo("会话机")==0)         return conCol_AllOrder_SessionID;//PrimaryKey
             if(szText->CompareTo("报单引用")==0)       return conCol_AllOrder_OrderRef;//PrimaryKey
             if(szText->CompareTo("状态")==0)           return conCol_AllOrder_ExStatus;//RowFilter 用于未成交列表和审核列表

             if(szText->CompareTo("交易员")==0)         return conCol_Trade_InvestorID;
             if(szText->CompareTo("合约")==0)           return conCol_Trade_InstrumentID;
             if(szText->CompareTo("买卖")==0)           return conCol_Trade_Direction;
             if(szText->CompareTo("开平")==0)           return conCol_Trade_OffsetFlag;
             if(szText->CompareTo("成交价格")==0)       return conCol_Trade_Price;
             if(szText->CompareTo("成交手数")==0)       return conCol_Trade_Volume;
             if(szText->CompareTo("手续费")==0)         return conCol_Trade_TradeCommission;
             if(szText->CompareTo("成交时间")==0)       return conCol_Trade_TradeTime;
             if(szText->CompareTo("成交类型")==0)       return conCol_Trade_TradeType;
             if(szText->CompareTo("投保")==0)           return conCol_Trade_HedgeFlag;
             if(szText->CompareTo("成交编号")==0)       return conCol_Trade_TradeID;
             if(szText->CompareTo("报单编号")==0)       return conCol_Trade_OrderSysID;
             if(szText->CompareTo("交易所")==0)         return conCol_Trade_ExchangeID;

             if(szText->CompareTo("交易员")==0)         return conCol_Position_InvestorID;//PrimaryKey
             if(szText->CompareTo("合约")==0)           return conCol_Position_InstrumentID;//PrimaryKey
             if(szText->CompareTo("买卖")==0)           return conCol_Position_Direction;//PrimaryKey
             if(szText->CompareTo("投保")==0)           return conCol_Position_HedgeFlag;//PrimaryKey
             if(szText->CompareTo("总持仓")==0)         return conCol_Position_Position;
             if(szText->CompareTo("可平量")==0)         return conCol_Position_CanCloseVolume;
             if(szText->CompareTo("现价")==0)           return conCol_Position_LastPrice;
             if(szText->CompareTo("持仓均价")==0)       return conCol_Position_AveragePositionPrice;
             if(szText->CompareTo("开仓均价")==0)       return conCol_Position_AverageOpenPrice;
             if(szText->CompareTo("持仓盈亏")==0)       return conCol_Position_PositionProfit;
             if(szText->CompareTo("浮动盈亏")==0)       return conCol_Position_PositionProfitByTrade;
             if(szText->CompareTo("盯市总盈亏")==0)     return conCol_Position_TotalPositionProfitByDate;
             if(szText->CompareTo("占用保证金")==0)     return conCol_Position_UseMargin;
             if(szText->CompareTo("总多仓")==0)         return conCol_Position_MultiBuyPosition;
             if(szText->CompareTo("总空仓")==0)         return conCol_Position_MultiSellPosition;
             if(szText->CompareTo("今多仓")==0)         return conCol_Position_TodayBuyPosition;
             if(szText->CompareTo("今空仓")==0)         return conCol_Position_TodaySellPosition;
             if(szText->CompareTo("昨仓")==0)           return conCol_Position_YesterdayPosition;
             if(szText->CompareTo("今仓")==0)           return conCol_Position_TodayPosition;
             if(szText->CompareTo("今开仓量")==0)       return conCol_Position_OpenVolume;
             if(szText->CompareTo("今平仓量")==0)       return conCol_Position_CloseVolume;
             if(szText->CompareTo("总可平今量")==0)     return conCol_Position_TotalCanCloseTodayVolume;
             if(szText->CompareTo("总可平昨量")==0)     return conCol_Position_TotalCanCloseYesterdayVolume;
             if(szText->CompareTo("平今挂单量")==0)     return conCol_Position_CloseTodayOrderVolume;
             if(szText->CompareTo("平仓挂单量")==0)     return conCol_Position_CloseOrderVolume;
             if(szText->CompareTo("可平今量")==0)       return conCol_Position_CanCloseTodayVolume;
             if(szText->CompareTo("可平昨量")==0)       return conCol_Position_CanCloseYesterdayVolume;
             if(szText->CompareTo("交易所")==0)         return conCol_Position_ExchangeID;

             return conCol_Position_None;
         }

        void RowsClearAt(int nUserID)
         {
             UserInfo userInfo;     
             for (int i=dataGridView_Position->Rows->Count-1;i>=0;i--)
             {
                 String^ szAccount = dataGridView_Position->Rows[i]->Cells[0]->Value->ToString()->Trim();
                 if(CRiskData::GetInstance()->GetUserInfoByAccount(Tools::String2string(szAccount),userInfo))
                 {
                     if(nUserID == userInfo.nUserID)
                     {
                         //将原先有持仓，现在无持仓的合约从m_mapExchange2Inst2中删除，不然从设置中操作订阅时报价表会删不掉该合约
                         String^ szExchangeID = dataGridView_Position->Rows[i]->Cells["交易所"]->Value->ToString()->Trim();
                         String^ szInstID = dataGridView_Position->Rows[i]->Cells["合约"]->Value->ToString()->Trim();
                         CRiskData::GetInstance()->UnSubScribeInst2(Tools::String2string(szExchangeID),Tools::String2string(szInstID));

//                          LOG_INFO("删除:szAccount=%s",userInfo.szAccount);
                         m_DataTablePosition->Rows[i]->Delete();
                     }
                 }
             }
    
             m_DataTablePosition->AcceptChanges();             
         }
        void RowsClearAt(map<PositionKey,PlatformStru_Position> mapPositionPush)
        {
            map<PositionKey,PlatformStru_Position>::iterator it = mapPositionPush.begin();
            String^ strAccount = Tools::string2String(it->first.Account);

            for (int i = 0; i < m_DataTablePosition->Rows->Count; i++) 
            { 
                String^ szAccount = m_DataTablePosition->Rows[i]["交易员"]->ToString()->Trim();
                String^ szInstID = m_DataTablePosition->Rows[i]["合约"]->ToString()->Trim();
                String^ szDirection = m_DataTablePosition->Rows[i]["买卖"]->ToString()->Trim();
                String^ szHeadFlag = m_DataTablePosition->Rows[i]["投保"]->ToString()->Trim();
                String^ szExchangeID = m_DataTablePosition->Rows[i]["交易所"]->ToString()->Trim();

                char chDirect,chHeadFlag;
                chDirect = (szDirection->CompareTo("买")==0)?THOST_FTDC_PD_Long:THOST_FTDC_PD_Short;

                if(szDirection->CompareTo("买")==0)
                    chDirect = THOST_FTDC_PD_Long;
                if(szDirection->CompareTo("卖")==0)
                    chDirect = THOST_FTDC_PD_Short;
                if(szDirection->CompareTo("净")==0)
                    chDirect = THOST_FTDC_PD_Net;

                if(szHeadFlag->CompareTo("投机")==0)
                    chHeadFlag = THOST_FTDC_HF_Speculation;
                if(szHeadFlag->CompareTo("套利")==0)
                    chHeadFlag = THOST_FTDC_HF_Arbitrage;
                if(szHeadFlag->CompareTo("套保")==0)
                    chHeadFlag = THOST_FTDC_HF_Hedge;
      
                PositionKey key(Tools::String2string(szAccount).c_str(),Tools::String2string(szInstID).c_str(),chDirect,chHeadFlag);
                map<PositionKey,PlatformStru_Position>::iterator it2 = mapPositionPush.find(key);                
                if(it2==mapPositionPush.end() && strAccount->CompareTo(szAccount)==0)//没有找到key
                {
                    cli::array<String^>^ key = {
                                                    szAccount,
                                                    szInstID,
                                                    szDirection,
                                                    szHeadFlag
                                                };

                    DataRow^  dr = nullptr;
                    dr = m_DataTablePosition->Rows->Find(key); 
                    if(dr)
                    {
                        //只有当m_mapExchange2Inst没有该合约时才能真正的退订
                        if(!CRiskData::GetInstance()->bSubScribeInst((char*)(Tools::String2string(szInstID).c_str())))
                            CTcpDataConnect::SendData(Cmd_UnSubscribeQuotEvent_Req, (char*)(Tools::String2string(szInstID).c_str()), strlen(Tools::String2string(szInstID).c_str())+1, 0, m_pLogin->mnUserID);
                            
                        CRiskData::GetInstance()->UnSubScribeInst2(Tools::String2string(szExchangeID),Tools::String2string(szInstID));

                        m_DataTablePosition->Rows->Remove(dr); 
//                         LOG_INFO("删除:szAccount=%s,InstrumentID=%s",//,PosiDirection=%c,HedgeFlag=%c",
//                             Tools::String2string(szAccount),Tools::String2string(szInstID)/*,chDirect,chHeadFlag*/);
                    }
                }
            }
        }
        void Subscribe(vector<int>& vecUserID)
        {
            if(!vecUserID.empty())
            {                
                CTcpLayer::SendData(Cmd_RM_SubscribeFund_Req, &vecUserID[0],sizeof(int)*vecUserID.size(), 0, m_pLogin->mnUserID);
                CTcpLayer::SendData(Cmd_RM_SubscribeOrder_Req, &vecUserID[0],sizeof(int)*vecUserID.size(), 0, m_pLogin->mnUserID);
                CTcpLayer::SendData(Cmd_RM_SubscribePosition_Req, &vecUserID[0],sizeof(int)*vecUserID.size(), 0, m_pLogin->mnUserID);
                CTcpLayer::SendData(Cmd_RM_SubscribeTrade_Req, &vecUserID[0],sizeof(int)*vecUserID.size(), 0, m_pLogin->mnUserID);
            }
        }
        void UnSubscribe(vector<int>& vecUserID)
        {
            int nHwnd = this->Handle.ToInt32();
            CTcpLayer::UnsubscribePkg(Cmd_ModifyUserPassword_Req, nHwnd);


            if(!vecUserID.empty())
            {
                CTcpLayer::SendData(Cmd_RM_UnSubscribeRiskEvent_Req, &vecUserID[0],sizeof(int)*vecUserID.size(), 0, m_pLogin->mnUserID);
                CTcpLayer::SendData(Cmd_RM_UnSubscribeFund_Req, &vecUserID[0],sizeof(int)*vecUserID.size(), 0, m_pLogin->mnUserID);
                CTcpLayer::SendData(Cmd_RM_UnSubscribeOrder_Req, &vecUserID[0],sizeof(int)*vecUserID.size(), 0, m_pLogin->mnUserID);
                CTcpLayer::SendData(Cmd_RM_UnSubscribePosition_Req, &vecUserID[0],sizeof(int)*vecUserID.size(), 0, m_pLogin->mnUserID);
                CTcpLayer::SendData(Cmd_RM_UnSubscribeTrade_Req, &vecUserID[0],sizeof(int)*vecUserID.size(), 0, m_pLogin->mnUserID);
            }
        }
        bool GetForceCloseInfo(sForceClose& oFC)
        {
            int nRowIndex = dataGridView_Position->CurrentCell->RowIndex;  
            if(nRowIndex<0)
                return false;

            memcpy(oFC.AccountID,Tools::String2string(dataGridView_Position->Rows[nRowIndex]->Cells[0]->Value->ToString()).c_str(),sizeof(TThostFtdcAccountIDType));
            memcpy(oFC.InstrumentID,Tools::String2string(dataGridView_Position->Rows[nRowIndex]->Cells[1]->Value->ToString()).c_str(),sizeof(TThostFtdcInstrumentIDType));
            //检查行情是否已经订阅
            if((!CRiskData::GetInstance()->bSubScribeInst(oFC.InstrumentID)) && (!CRiskData::GetInstance()->bSubScribeInst2(oFC.InstrumentID)))
            {
                MessageBox::Show("请先订阅此合约，获得涨跌停价格");
                return false;
            }

            oFC.Direction = (dataGridView_Position->Rows[nRowIndex]->Cells[2]->Value->ToString() == "卖")?THOST_FTDC_D_Buy:THOST_FTDC_D_Sell;
            oFC.UpperLimitPrice = Convert::ToDouble(GetUpperLimitPrice(oFC.InstrumentID));
            oFC.LowerLimitPrice = Convert::ToDouble(GetLowerLimitPrice(oFC.InstrumentID));
            oFC.OpenInterest = Convert::ToInt32(dataGridView_Position->Rows[nRowIndex]->Cells[4]->Value->ToString());
            oFC.TodayOpenInterest = Convert::ToInt32(dataGridView_Position->Rows[nRowIndex]->Cells[25]->Value->ToString());   

            return true;
        }
        bool IsANonHeaderButtonCell(DataGridViewCellEventArgs^ cellEvent)
        {
            if (dynamic_cast<DataGridViewButtonColumn^>(dataGridView_OrgRiskSetup->Columns[cellEvent->ColumnIndex]) != nullptr
                &&
                cellEvent->RowIndex != -1)
            { return true; }
            else { return (false); }
        }
        bool IsHaveItem(String^ szOrgText, String^ szRiskIndicatorText)
        {
            for (int i=0;i<dataGridView_OrgRiskSetup->Rows->Count;i++)
            {
                if(0==szOrgText->CompareTo(dataGridView_OrgRiskSetup->Rows[i]->Cells[0]->Value->ToString()) &&
                   0==szRiskIndicatorText->CompareTo(dataGridView_OrgRiskSetup->Rows[i]->Cells[1]->Value->ToString()))
                   return true;
            }

            return false;
        }
        void UpdateTextColor(TreeNode^ nodes, const SLimitTrade* pSLimitTrade)
        {
            string szUserAccount;
            if(CRiskData::GetInstance()->GetUserNameByID(pSLimitTrade->nUserID,szUserAccount))
            for each(TreeNode^ node in nodes->Nodes)
            {
                if (node->Text->CompareTo(Tools::string2String(szUserAccount.c_str())) == 0)
                {
                    bool bIsManualVerify = CRiskData::GetInstance()->IsManualVerify(pSLimitTrade->nUserID);

                    if(pSLimitTrade->bLimitTrade && !bIsManualVerify)
                        node->ForeColor = Color::Red;
                    else if(!pSLimitTrade->bLimitTrade && bIsManualVerify)
                        node->ForeColor = Color::FromArgb(255,182,0);//Orange;
                    else if(pSLimitTrade->bLimitTrade && bIsManualVerify)
                        node->ForeColor = Color::Red;
                    else if(!pSLimitTrade->bLimitTrade && !bIsManualVerify)
                        node->ForeColor = Color::FromArgb(0,255,64);//Green

                    break;
                }

                if (node->Nodes->Count>0)
                    UpdateTextColor(node,pSLimitTrade);
            }
        }
        void UpdateTextColor(TreeNode^ nodes, const SManualVerify* pSManualVerify)
        {
            string szUserAccount;
            if(CRiskData::GetInstance()->GetUserNameByID(pSManualVerify->nUserID,szUserAccount))
                for each(TreeNode^ node in nodes->Nodes)
                {
                    if (node->Text->CompareTo(Tools::string2String(szUserAccount.c_str())) == 0)
                    {
                        bool bIsLimitTrade = CRiskData::GetInstance()->IsLimitTrade(pSManualVerify->nUserID);

                        if(bIsLimitTrade && !pSManualVerify->bManualVerify)
                            node->ForeColor = Color::Red;
                        else if(!bIsLimitTrade && pSManualVerify->bManualVerify)
                            node->ForeColor = Color::FromArgb(255,182,0);//Orange;
                        else if(bIsLimitTrade && pSManualVerify->bManualVerify)
                            node->ForeColor = Color::Red;
                        else if(!bIsLimitTrade && !pSManualVerify->bManualVerify)
                            node->ForeColor = Color::FromArgb(0,255,64);//Green

                        break;
                    }

                    if (node->Nodes->Count>0)
                        UpdateTextColor(node,pSManualVerify);
                }
        }
        void LoasSubAssertMgmtOrgan(TreeNode^ nodeMain,map<int, map<int, Organization>>& mapMain,map<int, Organization>& mapSub, vector<int>& vecUserID) 
        {
            map<int, map<int, Organization>>::iterator it;
            map<int, Organization>::iterator itSub;

            TreeNode^ subNode;

            itSub = mapSub.begin();
            for(; itSub != mapSub.end(); itSub++) 
            {
                subNode = nodeMain->Nodes->Add(itSub->first.ToString(), Tools::string2String(itSub->second.szName));
                subNode->Tag = itSub->first;
                subNode->ForeColor = Color::Blue;

                if(CRiskData::GetInstance()->IsMonitorOrg(itSub->first))
                {
                    vector<UserInfo> vecUserInfo;
                    CRiskData::GetInstance()->GetUserInfosByOrgID(itSub->first,USER_TYPE_TRADER,vecUserInfo,false);
                    if(!vecUserInfo.empty())
                    {
                        TreeNode^ traderNode;
                        bool bIsLimitTrade,bIsManualVerify; 
                        vector<UserInfo>::iterator itUserInfo = vecUserInfo.begin();
                        for (;itUserInfo!=vecUserInfo.end();itUserInfo++)
                        {
                            traderNode = subNode->Nodes->Add(itUserInfo->nUserID.ToString(),Tools::string2String(itUserInfo->szAccount));
                            traderNode->Tag = itUserInfo->nUserID;

                            bIsLimitTrade = CRiskData::GetInstance()->IsLimitTrade(itUserInfo->nUserID);
                            bIsManualVerify = CRiskData::GetInstance()->IsManualVerify(itUserInfo->nUserID);
                            //有下单限制+无人工审核
                            if(bIsLimitTrade && !bIsManualVerify)
                                traderNode->ForeColor = Color::Red;
                            //无下单限制+有人工审核
                            else if(!bIsLimitTrade && bIsManualVerify)
                                traderNode->ForeColor = Color::FromArgb(255,182,0);//Orange;
                            //有下单限制+有人工审核
                            else if(bIsLimitTrade && bIsManualVerify)
                                traderNode->ForeColor = Color::Red;
                            //无下单限制+无人工审核
                            else if(!bIsLimitTrade && !bIsManualVerify)
                                traderNode->ForeColor = Color::FromArgb(0,255,64);//Green

                            vecUserID.push_back(itUserInfo->nUserID);
                        }
                    }
                }

                it = mapMain.find(itSub->first);
                if(it != mapMain.end())
                    LoasSubAssertMgmtOrgan(subNode, mapMain, it->second, vecUserID);              
            }
        }

        void LoadAssertMgmtOrgan(int nOrgID,vector<int>& vecUserID) 
        {      
            vecUserID.clear();
            treeView_Organization->Nodes->Clear();

            Organization assertOrg;
            memset(&assertOrg, 0, sizeof(assertOrg));
            if(!CRiskData::GetInstance()->GetOrganizationByID(nOrgID, assertOrg))
                return;

            TreeNode^ node;
            node = treeView_Organization->Nodes->Add(assertOrg.nAssetMgmtOrgID.ToString(), Tools::string2String(assertOrg.szName));
            node->Tag = assertOrg.nAssetMgmtOrgID;
            node->ForeColor = Color::Blue;
            treeView_Organization->SelectedNode = node;

            if(CRiskData::GetInstance()->IsMonitorOrg(assertOrg.nAssetMgmtOrgID))
            {
                vector<UserInfo> vecUserInfo;
                CRiskData::GetInstance()->GetUserInfosByOrgID(assertOrg.nAssetMgmtOrgID,USER_TYPE_TRADER,vecUserInfo,false);
                if(!vecUserInfo.empty())
                {
                    TreeNode^ traderNode;
                    bool bIsLimitTrade,bIsManualVerify; 

                    vector<UserInfo>::iterator itUserInfo = vecUserInfo.begin();
                    for (;itUserInfo!=vecUserInfo.end();itUserInfo++)
                    {
                        traderNode = node->Nodes->Add(itUserInfo->nUserID.ToString(),Tools::string2String(itUserInfo->szAccount));
                        traderNode->Tag = itUserInfo->nUserID;

                        bIsLimitTrade = CRiskData::GetInstance()->IsLimitTrade(itUserInfo->nUserID);
                        bIsManualVerify = CRiskData::GetInstance()->IsManualVerify(itUserInfo->nUserID);
                        //有下单限制+无人工审核
                        if(bIsLimitTrade && !bIsManualVerify)
                            traderNode->ForeColor = Color::Red;
                        //无下单限制+有人工审核
                        else if(!bIsLimitTrade && bIsManualVerify)
                            traderNode->ForeColor = Color::FromArgb(255,182,0);//Orange;
                        //有下单限制+有人工审核
                        else if(bIsLimitTrade && bIsManualVerify)
                            traderNode->ForeColor = Color::Red;
                        //无下单限制+无人工审核
                        else if(!bIsLimitTrade && !bIsManualVerify)
                            traderNode->ForeColor = Color::FromArgb(0,255,64);//Green

                        vecUserID.push_back(itUserInfo->nUserID);
                    }
                }
            }



            map<int, map<int, Organization>> mapAssetMgmtOrgTree;
            CRiskData::GetInstance()->GetOrganization(mapAssetMgmtOrgTree);

            map<int, map<int, Organization>>::iterator it = mapAssetMgmtOrgTree.find(nOrgID);
            if(it == mapAssetMgmtOrgTree.end())
                return;

            map<int, Organization>& mapSub = it->second;
            if(mapSub.begin()==mapSub.end())
                return;

            LoasSubAssertMgmtOrgan(node, mapAssetMgmtOrgTree, mapSub, vecUserID);

            node->Expand();
        }
        void Beep()
        {
            System::Console::Beep(800,300);
        }
        void Shake()
        {
			if(this->WindowState == System::Windows::Forms::FormWindowState::Minimized)
				this->WindowState = System::Windows::Forms::FormWindowState::Normal;//如果是最小化则先还原后震动

            if(this->WindowState == System::Windows::Forms::FormWindowState::Maximized)
                return;

            Random^ ran = gcnew Random((int)DateTime::Now.Ticks);
            System::Drawing::Point point = this->Location;
            for (int i = 0; i < 40; i++)
            {
                this->Location = System::Drawing::Point(point.X + ran->Next(8) - 4, point.Y + ran->Next(8) - 4);
                System::Threading::Thread::Sleep(15);
                this->Location = point;
                System::Threading::Thread::Sleep(15);
            }
        }
        void ShowMsg(RiskEvent* pRiskEvent)
        {
            char szTip[512];
            memset(szTip,0,sizeof(szTip));

            string szTraderName,szIndicatorName,szEventStatus;
            char szIndicatorValue[64];

            UserInfo userInfo;
            if(!CRiskData::GetInstance()->GetUserInfoByID(pRiskEvent->nTradeInvestorID,userInfo))
                return; 

            map<RiskIndicatorType,RiskIndicator> mapRiskIndicator;
            CRiskData::GetInstance()->GetRiskIndicator(mapRiskIndicator);

            map<RiskIndicatorType,RiskIndicator>::iterator it = mapRiskIndicator.find(pRiskEvent->nRiskIndicatorID);
            if(it!=mapRiskIndicator.end())
            {
                szIndicatorName = (string)(it->second.szName);
            }

            memset(szIndicatorValue,0,sizeof(szIndicatorValue));
            switch (pRiskEvent->nRiskIndicatorID)
            {
            case RI_MaxPosition://合约最大持仓;   合约持仓总数（买卖方向汇总）
                sprintf(szIndicatorValue, "%.0f手", pRiskEvent->dblIndicatorValue);
                break;
            case RI_MarketShock://市场冲击风险;   合约持仓总数（买卖方向汇总）占市场总持仓数百分比     
            case RI_AccountLoss://账户限亏		  （今日动态权益 -- 账户初始权益）/账户初始权益
            case RI_TodayLoss://今日限亏		  账户今日亏损占日初权益的百分比
            case RI_GappedMarket://隔夜跳空        账户各合约持仓综合后（买卖方向中和），预测第二天最大跳空亏损占账户初始权益百分比
            case RI_MarginRatio://保证金比例风险  保证金和动态权益的比值（风险度）
                sprintf(szIndicatorValue, "%.2f%%", pRiskEvent->dblIndicatorValue*100);
                break;
            case RI_SingleHoldTime://单笔持仓时间  
                sprintf(szIndicatorValue, "%.0f秒", pRiskEvent->dblIndicatorValue);
                break;
            case RI_FundNetValue://基金净值风险
                sprintf(szIndicatorValue, "%.5f", pRiskEvent->dblIndicatorValue);
                break;
            }


            String^ szRiskDesc = String::Empty;
            if( pRiskEvent->nRiskIndicatorID == RI_MaxPosition || 
                pRiskEvent->nRiskIndicatorID == RI_MarketShock ||
                pRiskEvent->nRiskIndicatorID == RI_SingleHoldTime)
                szRiskDesc = Tools::string2String(pRiskEvent->InstrumentID)+":"+pRiskEvent->nRiskLevelID.ToString()+"级风险";
            else
                szRiskDesc = pRiskEvent->nRiskLevelID.ToString()+"级风险";

            tm *tmThatTime = _localtime64(&(pRiskEvent->lEventTime));  
            char tTimeBuffer[30];  
            memset(tTimeBuffer, 0, 30);  
            sprintf(tTimeBuffer, "%04d/%02d/%02d %02d:%02d:%02d", tmThatTime->tm_year+1900, tmThatTime->tm_mon+1, tmThatTime->tm_mday, tmThatTime->tm_hour, tmThatTime->tm_min, tmThatTime->tm_sec);  

            switch(pRiskEvent->nMsgSendStatus)
            {
            case MsgStatus_New:				//服务器记录：发生
                szEventStatus = "发生";
                break;
            case MsgStatus_ChangeLevel:		//服务器记录：等级变更
                szEventStatus = "等级变更";
                break;
            case MsgStatus_Delete:			//服务器记录：消失
                szEventStatus = "消失";
                break;
            case MsgStatus_Appoint:			//手动操作，委派给交易员
                szEventStatus = "委派给交易员";
                break;
            case MsgStatus_Finished:			//手动操作，风险事件解决记录
                szEventStatus = "风险事件解决记录";
                break;
            case MsgStatus_ForceClose:		//手动操作，执行手动强平记录 
                szEventStatus = "执行手动强平记录";
                break;
            }


            sprintf(szTip,"风险事件ID: %d,交易员ID: %s,交易员名称: %s,风险指标类型: %s,风险详情: %s,当前风险阀值: %s,风险等级更新时间: %s,风险事件状态: %s\r\n",
                           pRiskEvent->nRiskEventID,
                           userInfo.szAccount,
                           userInfo.szAlias,
                           szIndicatorName.c_str(),
                           szRiskDesc,
                           pRiskEvent->dblIndicatorCurrentValue,//szIndicatorValue,
                           tTimeBuffer,
                           szEventStatus.c_str());
            
            _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
                CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
                szTip);
        }
        void ProcessRiskAction(RiskEvent* pRiskEvent)
        {
//             vector<int> vecOrgID;
//             CRiskData::GetInstance()->GetOrgIDByUserID(pRiskEvent->nTradeInvestorID,USER_ORG_RELATION_ATTACH,vecOrgID);
//             if(vecOrgID.empty())
//                 return;
// 
//             Key_RiskPlan key;
//             key.first = vecOrgID[0]; 
//             key.second = pRiskEvent->nRiskIndicatorID;
// 
//             RiskPlan riskPlan;
//             if(CRiskData::GetInstance()->GetRiskPlan(key,pRiskEvent->nRiskLevelID,riskPlan))
//             {
                int nActionValue = pRiskEvent->nResponseType;//riskPlan.WaringLevel.nResponseType;
                if(nActionValue & WarningType_Beep)
                    Beep();

                if(nActionValue & WarningType_Vibrate)
                    Shake();
                
                //if(nActionValue & WarningType_ShowMsg)
                    //ShowMsg(pRiskEvent);
//             }            
        }
        bool IsNotTrigger(int nRiskEventID)
        {
            cliext::set<int>::iterator it = m_setRiskEventID.begin();
            for(;it!=m_setRiskEventID.end();it++)
            {
                if(*it == nRiskEventID)
                    return false;
            }
                
            return true;
        }
        void SetDataGridviewColumns()
        {
//             String^ szColumnText = String::Empty;
//             int nColumnIndex = -1;
//             vector<string> vecColumn;
// 
//             map<string,vector<string>> mapDataGridView2Column = CRiskData::GetInstance()->GetConfigFile(XML_TYPE_COLUMN);
//             for(map<string,vector<string>>::iterator it = mapDataGridView2Column.begin();it != mapDataGridView2Column.end();it++)
//             {
//                 szDataGridView = it->first;
//                 if(0==strcmp(szDataGridView.c_str(),DataGridView_DepthMarket))
//                 {
//                     vecColumn = it->second;
//                     for (vector<string>::iterator it_column = vecColumn.begin();it_column != vecColumn.end();it_column++)
//                     {
//                         szColumnText = Tools::string2String((*it_column).c_str());
//                         if(szColumnText->CompareTo("合约")==0)     {m_activeDataGridView->Columns[it->second]->Visible = true;}{m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("合约",      ::System::String::typeid));continue;}//PrimaryKey
//                         if(nColumnIndex == conCol_DepthMarket_LastPrice)        {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("最新价",    ::System::String::typeid));continue;}
//                         if(nColumnIndex == conCol_DepthMarket_Mixed)            {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("涨跌",      ::System::String::typeid));continue;}
//                         if(nColumnIndex == conCol_DepthMarket_MixedRange)       {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("涨跌幅",    ::System::String::typeid));continue;}
//                         if(nColumnIndex == conCol_DepthMarket_BidPrice1)        {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("买价",      ::System::String::typeid));continue;}
//                         if(nColumnIndex == conCol_DepthMarket_BidVolume1)       {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("买量",      ::System::String::typeid));continue;}
//                         if(nColumnIndex == conCol_DepthMarket_AskPrice1)        {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("卖价",      ::System::String::typeid));continue;}
//                         if(nColumnIndex == conCol_DepthMarket_AskVolume1)       {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("卖量",      ::System::String::typeid));continue;}
//                         if(nColumnIndex == conCol_DepthMarket_Volume)           {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("成交量",    ::System::String::typeid));continue;}
//                         if(nColumnIndex == conCol_DepthMarket_OpenInterest)     {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("持仓量",    ::System::String::typeid));continue;}
//                         if(nColumnIndex == conCol_DepthMarket_CurVolume)        {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("现量",      ::System::String::typeid));continue;}
//                         if(nColumnIndex == conCol_DepthMarket_OpenPrice)        {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("今开盘",    ::System::String::typeid));continue;}
//                         if(nColumnIndex == conCol_DepthMarket_SettlementPrice)  {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("昨结算",    ::System::String::typeid));continue;}
//                         if(nColumnIndex == conCol_DepthMarket_HighestPrice)     {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("最高价",    ::System::String::typeid));continue;}
//                         if(nColumnIndex == conCol_DepthMarket_LowestPrice)      {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("最低价",    ::System::String::typeid));continue;}
//                         if(nColumnIndex == conCol_DepthMarket_UpperLimitPrice)  {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("涨停价",    ::System::String::typeid));continue;}
//                         if(nColumnIndex == conCol_DepthMarket_LowerLimitPrice)  {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("跌停价",    ::System::String::typeid));continue;}
//                         if(nColumnIndex == conCol_DepthMarket_UpdateTime)       {m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("更新时间",  ::System::String::typeid));continue;}
//                     }
// 
//                     continue;
//                 }
//             }
        }
        void SetDataTableColumns()
        {
            g_mapColumnText2Index.clear();
            m_DataTableDepthMarketData->Columns->Clear();//报价表
            m_DataTableRiskEvent->Columns->Clear();//风险事件
            m_DataTableTradingAccount->Columns->Clear();//账户资金
            m_DataTableAllOrder->Columns->Clear();//委托单
            m_DataTableTrade->Columns->Clear();//成交记录
            m_DataTablePosition->Columns->Clear();//持仓

            string szDataGridView = "";
            int nColumnIndex = -1;
            vector<string> vecColumn;

            if(m_DataTableDepthMarketData == nullptr)
                return;

            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("合约",      ::System::String::typeid));//PrimaryKey
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("最新价",    ::System::Double::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("涨跌",      ::System::Double::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("涨跌幅",    ::System::String::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("买价",      ::System::Double::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("买量",      ::System::Int32::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("卖价",      ::System::Double::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("卖量",      ::System::Int32::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("成交量",    ::System::Int32::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("持仓量",    ::System::Int32::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("现量",      ::System::Int32::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("今开盘",    ::System::Double::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("昨结算",    ::System::Double::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("最高价",    ::System::Double::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("最低价",    ::System::Double::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("涨停价",    ::System::Double::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("跌停价",    ::System::Double::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("更新时间",  ::System::String::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("成交额",    ::System::Double::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("今收盘",    ::System::Double::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("结算价",    ::System::Double::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("昨收盘",    ::System::Double::typeid));
            m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("昨持仓量",  ::System::Int32::typeid));
            
            if(m_DataTableRiskEvent == nullptr)
                return;

            m_DataTableRiskEvent->Columns->Add(gcnew DataColumn("风险事件ID",    ::System::String::typeid));//PrimaryKey
            m_DataTableRiskEvent->Columns->Add(gcnew DataColumn("交易员ID",      ::System::String::typeid));
            m_DataTableRiskEvent->Columns->Add(gcnew DataColumn("交易员名称",    ::System::String::typeid));
            m_DataTableRiskEvent->Columns->Add(gcnew DataColumn("风险指标类型",  ::System::String::typeid));
            m_DataTableRiskEvent->Columns->Add(gcnew DataColumn("风险详情",      ::System::String::typeid));
             m_DataTableRiskEvent->Columns->Add(gcnew DataColumn("当前风险阀值", ::System::String::typeid));
            m_DataTableRiskEvent->Columns->Add(gcnew DataColumn("风险等级更新时间",::System::String::typeid));
            m_DataTableRiskEvent->Columns->Add(gcnew DataColumn("风险事件状态",  ::System::String::typeid));           

            if(m_DataTableTradingAccount == nullptr)
                return;

            m_DataTableTradingAccount->Columns->Add(gcnew DataColumn("交易员",      ::System::String::typeid));//PrimaryKey
            m_DataTableTradingAccount->Columns->Add(gcnew DataColumn("交易限制",    ::System::String::typeid));
            m_DataTableTradingAccount->Columns->Add(gcnew DataColumn("手动审核",    ::System::String::typeid));
            m_DataTableTradingAccount->Columns->Add(gcnew DataColumn("登录状态",    ::System::String::typeid));
            m_DataTableTradingAccount->Columns->Add(gcnew DataColumn("风险度(%)",   ::System::String::typeid));
            m_DataTableTradingAccount->Columns->Add(gcnew DataColumn("动态权益",    ::System::Double::typeid));
            m_DataTableTradingAccount->Columns->Add(gcnew DataColumn("持仓盈亏",    ::System::Double::typeid));
            m_DataTableTradingAccount->Columns->Add(gcnew DataColumn("平仓盈亏",    ::System::Double::typeid));
            m_DataTableTradingAccount->Columns->Add(gcnew DataColumn("占用保证金",  ::System::Double::typeid));
            m_DataTableTradingAccount->Columns->Add(gcnew DataColumn("手续费",      ::System::Double::typeid));
            m_DataTableTradingAccount->Columns->Add(gcnew DataColumn("冻结保证金",  ::System::Double::typeid));
            m_DataTableTradingAccount->Columns->Add(gcnew DataColumn("冻结手续费",  ::System::Double::typeid));

            if(m_DataTableAllOrder == nullptr)
                return;

            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("交易员",      ::System::String::typeid));//PrimaryKey
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("报单编号",    ::System::String::typeid));
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("合约",        ::System::String::typeid));//PrimaryKey
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("买卖",        ::System::String::typeid));
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("开平",        ::System::String::typeid));
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("挂单状态",    ::System::String::typeid));
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("报单价格",    ::System::Double::typeid));
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("报单手数",    ::System::Int32::typeid));
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("未成交手数",  ::System::Int32::typeid));
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("成交手数",    ::System::Int32::typeid));
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("详细状态",    ::System::String::typeid));
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("报单时间",    ::System::String::typeid));
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("冻结保证金",  ::System::Double::typeid));
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("冻结手续费",  ::System::Double::typeid));
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("投保",        ::System::String::typeid));
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("交易所",      ::System::String::typeid));
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("前置机",      ::System::String::typeid));//PrimaryKey
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("会话机",      ::System::String::typeid));//PrimaryKey
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("报单引用",    ::System::String::typeid));//PrimaryKey
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("状态",        ::System::String::typeid));//RowFilter 用于未成交列表和审核列表
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("是否强平",    ::System::String::typeid));
            m_DataTableAllOrder->Columns->Add(gcnew DataColumn("强平信息",    ::System::String::typeid));

            if(m_DataTableTrade == nullptr)
                return;

            m_DataTableTrade->Columns->Add(gcnew DataColumn("交易员",      ::System::String::typeid));
            m_DataTableTrade->Columns->Add(gcnew DataColumn("合约",        ::System::String::typeid));
            m_DataTableTrade->Columns->Add(gcnew DataColumn("买卖",        ::System::String::typeid));
            m_DataTableTrade->Columns->Add(gcnew DataColumn("开平",        ::System::String::typeid));
            m_DataTableTrade->Columns->Add(gcnew DataColumn("成交价格",    ::System::Double::typeid));
            m_DataTableTrade->Columns->Add(gcnew DataColumn("成交手数",    ::System::Int32::typeid));
            m_DataTableTrade->Columns->Add(gcnew DataColumn("手续费",      ::System::Double::typeid));
            m_DataTableTrade->Columns->Add(gcnew DataColumn("成交时间",    ::System::String::typeid));
            m_DataTableTrade->Columns->Add(gcnew DataColumn("成交类型",    ::System::String::typeid));
            m_DataTableTrade->Columns->Add(gcnew DataColumn("投保",        ::System::String::typeid));
            m_DataTableTrade->Columns->Add(gcnew DataColumn("成交编号",    ::System::String::typeid));
            m_DataTableTrade->Columns->Add(gcnew DataColumn("报单编号",    ::System::String::typeid));
            m_DataTableTrade->Columns->Add(gcnew DataColumn("交易所",      ::System::String::typeid));
            
            if(m_DataTablePosition == nullptr)
                return;

            m_DataTablePosition->Columns->Add(gcnew DataColumn("交易员",      ::System::String::typeid));//PrimaryKey
            m_DataTablePosition->Columns->Add(gcnew DataColumn("合约",        ::System::String::typeid));//PrimaryKey
            m_DataTablePosition->Columns->Add(gcnew DataColumn("买卖",        ::System::String::typeid));//PrimaryKey
            m_DataTablePosition->Columns->Add(gcnew DataColumn("投保",        ::System::String::typeid));//PrimaryKey
            m_DataTablePosition->Columns->Add(gcnew DataColumn("总持仓",      ::System::Int32::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("可平量",      ::System::Int32::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("现价",        ::System::Double::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("持仓均价",    ::System::Double::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("开仓均价",    ::System::Double::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("持仓盈亏",    ::System::Double::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("浮动盈亏",    ::System::Double::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("盯市总盈亏",  ::System::Double::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("占用保证金",  ::System::Double::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("总多仓",      ::System::Int32::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("总空仓",      ::System::Int32::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("今多仓",      ::System::Int32::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("今空仓",      ::System::Int32::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("昨仓",        ::System::Int32::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("今仓",        ::System::Int32::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("今开仓量",    ::System::Int32::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("今平仓量",    ::System::Int32::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("总可平今量",  ::System::Int32::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("总可平昨量",  ::System::Int32::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("平今挂单量",  ::System::Int32::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("平仓挂单量",  ::System::Int32::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("可平今量",    ::System::Int32::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("可平昨量",    ::System::Int32::typeid));
            m_DataTablePosition->Columns->Add(gcnew DataColumn("交易所",      ::System::String::typeid));

        }
        void SetDataGridViewCellStyle()
        {
            dataGridView_QuotOrderTable->Columns["最新价"]->DefaultCellStyle->Format = "N2";
            dataGridView_QuotOrderTable->Columns["涨跌"]->DefaultCellStyle->Format   = "N2";
            dataGridView_QuotOrderTable->Columns["涨跌幅"]->DefaultCellStyle->Format = "N2";
            dataGridView_QuotOrderTable->Columns["买价"]->DefaultCellStyle->Format   = "N2";
            dataGridView_QuotOrderTable->Columns["卖价"]->DefaultCellStyle->Format   = "N2";
            dataGridView_QuotOrderTable->Columns["今开盘"]->DefaultCellStyle->Format = "N2";
            dataGridView_QuotOrderTable->Columns["昨结算"]->DefaultCellStyle->Format = "N2";
            dataGridView_QuotOrderTable->Columns["最高价"]->DefaultCellStyle->Format = "N2";
            dataGridView_QuotOrderTable->Columns["最低价"]->DefaultCellStyle->Format = "N2";
            dataGridView_QuotOrderTable->Columns["涨停价"]->DefaultCellStyle->Format = "N2";
            dataGridView_QuotOrderTable->Columns["跌停价"]->DefaultCellStyle->Format = "N2";
            dataGridView_QuotOrderTable->Columns["成交额"]->DefaultCellStyle->Format = "N2";
            dataGridView_QuotOrderTable->Columns["今收盘"]->DefaultCellStyle->Format = "N2";
            dataGridView_QuotOrderTable->Columns["结算价"]->DefaultCellStyle->Format = "N2";
            dataGridView_QuotOrderTable->Columns["昨收盘"]->DefaultCellStyle->Format = "N2";

            dataGridView_AccountFundInfo->Columns["动态权益"]->DefaultCellStyle->Format = "N2";
            dataGridView_AccountFundInfo->Columns["持仓盈亏"]->DefaultCellStyle->Format = "N2";
            dataGridView_AccountFundInfo->Columns["平仓盈亏"]->DefaultCellStyle->Format = "N2";
            dataGridView_AccountFundInfo->Columns["占用保证金"]->DefaultCellStyle->Format = "N2";
            dataGridView_AccountFundInfo->Columns["手续费"]->DefaultCellStyle->Format   = "N2";
            dataGridView_AccountFundInfo->Columns["冻结保证金"]->DefaultCellStyle->Format = "N2";
            dataGridView_AccountFundInfo->Columns["冻结手续费"]->DefaultCellStyle->Format = "N2";

            dataGridView_AllOrder->Columns["报单价格"]->DefaultCellStyle->Format   = "N2";
            dataGridView_AllOrder->Columns["冻结保证金"]->DefaultCellStyle->Format = "N2";
            dataGridView_AllOrder->Columns["冻结手续费"]->DefaultCellStyle->Format = "N2";

            dataGridView_TradeRecord->Columns["成交价格"]->DefaultCellStyle->Format = "N2";
            dataGridView_TradeRecord->Columns["手续费"]->DefaultCellStyle->Format = "N2";


            dataGridView_Position->Columns["现价"]->DefaultCellStyle->Format = "N2";
            dataGridView_Position->Columns["持仓均价"]->DefaultCellStyle->Format = "N2";
            dataGridView_Position->Columns["开仓均价"]->DefaultCellStyle->Format = "N2";
            dataGridView_Position->Columns["持仓盈亏"]->DefaultCellStyle->Format = "N2";
            dataGridView_Position->Columns["浮动盈亏"]->DefaultCellStyle->Format = "N2";
            dataGridView_Position->Columns["盯市总盈亏"]->DefaultCellStyle->Format = "N2";
            dataGridView_Position->Columns["占用保证金"]->DefaultCellStyle->Format = "N2";
        }
        void SetDataTableColumns(bool bPositionDetail)
        {
            if(m_DataTablePosition == nullptr)
                return;

            m_DataTablePosition->Columns->Clear();

            if (bPositionDetail)
            {
                m_DataTablePosition->Columns->Add(gcnew DataColumn("交易员",      ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("合约",        ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("买卖",        ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("投保",        ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("手数",        ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("最新价",      ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("开仓价",      ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("开仓时间",    ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("成交编号",    ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("持仓类型",    ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("昨结算",      ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("占用保证金",  ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("平仓量",      ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("持仓盈亏",    ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("平仓盈亏",    ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("浮动盈亏",    ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("交易所",      ::System::String::typeid));
            }
            else
            {
                m_DataTablePosition->Columns->Add(gcnew DataColumn("交易员",      ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("合约",        ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("买卖",        ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("投保",        ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("总持仓",      ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("可平量",      ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("现价",        ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("持仓均价",    ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("开仓均价",    ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("持仓盈亏",    ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("浮动盈亏",    ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("盯市总盈亏",  ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("占用保证金",  ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("总多仓",      ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("总空仓",      ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("今多仓",      ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("今空仓",      ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("昨仓",        ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("今仓",        ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("今开仓量",    ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("今平仓量",    ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("总可平今量",  ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("总可平昨量",  ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("平今挂单量",  ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("平仓挂单量",  ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("可平今量",    ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("可平昨量",    ::System::String::typeid));
                m_DataTablePosition->Columns->Add(gcnew DataColumn("交易所",      ::System::String::typeid));
            }
        }
        void SetDataTable(PlatformStru_DepthMarketData* pDepthMarketData)
        {
            //没有订阅的合约不用显示 是持仓里有的，要更新最新价
            if(!CRiskData::GetInstance()->bSubScribeInst((string)pDepthMarketData->InstrumentID))
            {
                for(int i=0;i<dataGridView_Position->RowCount;i++)
                {
                    String^ szInstName = dataGridView_Position->Rows[i]->Cells["合约"]->Value->ToString();
                    if(szInstName->CompareTo(Tools::string2String(pDepthMarketData->InstrumentID))==0)
                    {
                        double fPriceTick = CRiskData::GetInstance()->GetPriceTickByInstID(pDepthMarketData->InstrumentID);                        
                        dataGridView_Position->Rows[i]->Cells["现价"]->Value = Tools::string2String(Price2string(pDepthMarketData->LastPrice,fPriceTick).c_str());
                        return;
                    }
                } 

                return;
            }

            DataRow^  dr = nullptr;
            dr = m_DataTableDepthMarketData->Rows->Find(Tools::string2String(pDepthMarketData->InstrumentID));

            double fPriceTick = CRiskData::GetInstance()->GetPriceTickByInstID(pDepthMarketData->InstrumentID);

            int nVolumn = 0;
            map<string,int>::iterator it_pre = g_mapCurrString2Volumn.find(pDepthMarketData->InstrumentID);
            if(it_pre!=g_mapCurrString2Volumn.end())
                nVolumn = it_pre->second;
            else
            {
                //非交易时间时
                nVolumn = pDepthMarketData->Volume;
            }

            //更新
            if(dr!=nullptr)
            {						
                int i=0;
                dr[i++] = Tools::string2String(pDepthMarketData->InstrumentID);
                dr[i++] = util::isInvalidValue(pDepthMarketData->LastPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->LastPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pDepthMarketData->PreSettlementPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->LastPrice-pDepthMarketData->PreSettlementPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pDepthMarketData->PreSettlementPrice)?System::Convert::DBNull:(((pDepthMarketData->LastPrice-pDepthMarketData->PreSettlementPrice)*100/pDepthMarketData->PreSettlementPrice)).ToString("F2")+"%";
                dr[i++] = util::isInvalidValue(pDepthMarketData->BidPrice1)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->BidPrice1,fPriceTick).c_str());
                dr[i++] = pDepthMarketData->BidVolume1.ToString();
                dr[i++] = util::isInvalidValue(pDepthMarketData->AskPrice1)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->AskPrice1,fPriceTick).c_str());
                dr[i++] = pDepthMarketData->AskVolume1.ToString();
                dr[i++] = pDepthMarketData->Volume.ToString();
                dr[i++] = pDepthMarketData->OpenInterest.ToString();
                dr[i++] = (pDepthMarketData->Volume-nVolumn).ToString();
                dr[i++] = util::isInvalidValue(pDepthMarketData->OpenPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->OpenPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pDepthMarketData->PreSettlementPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->PreSettlementPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pDepthMarketData->HighestPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->HighestPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pDepthMarketData->LowestPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->LowestPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pDepthMarketData->UpperLimitPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->UpperLimitPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pDepthMarketData->LowerLimitPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->LowerLimitPrice,fPriceTick).c_str());
                dr[i++] = Tools::string2String(pDepthMarketData->UpdateTime);
                dr[i++] = util::isInvalidValue(pDepthMarketData->Turnover)?System::Convert::DBNull:pDepthMarketData->Turnover.ToString("F2");
                dr[i++] = util::isInvalidValue(pDepthMarketData->ClosePrice)?System::Convert::DBNull:(Tools::string2String(Price2string(pDepthMarketData->ClosePrice,fPriceTick).c_str()));
                dr[i++] = util::isInvalidValue(pDepthMarketData->SettlementPrice)?System::Convert::DBNull:(Tools::string2String(Price2string(pDepthMarketData->SettlementPrice,fPriceTick).c_str()));
                dr[i++] = util::isInvalidValue(pDepthMarketData->PreClosePrice)?System::Convert::DBNull:(Tools::string2String(Price2string(pDepthMarketData->PreClosePrice,fPriceTick).c_str()));
                dr[i++] = pDepthMarketData->PreOpenInterest.ToString("G");
            }
            else//添加
            {                
                DataRow^ dr = m_DataTableDepthMarketData->NewRow();
                int i=0;
                dr[i++] = Tools::string2String(pDepthMarketData->InstrumentID);
                dr[i++] = util::isInvalidValue(pDepthMarketData->LastPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->LastPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pDepthMarketData->PreSettlementPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->LastPrice-pDepthMarketData->PreSettlementPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pDepthMarketData->PreSettlementPrice)?System::Convert::DBNull:(((pDepthMarketData->LastPrice-pDepthMarketData->PreSettlementPrice)*100/pDepthMarketData->PreSettlementPrice)).ToString("F2")+"%";
                dr[i++] = util::isInvalidValue(pDepthMarketData->BidPrice1)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->BidPrice1,fPriceTick).c_str());
                dr[i++] = pDepthMarketData->BidVolume1.ToString();
                dr[i++] = util::isInvalidValue(pDepthMarketData->AskPrice1)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->AskPrice1,fPriceTick).c_str());
                dr[i++] = pDepthMarketData->AskVolume1.ToString();
                dr[i++] = pDepthMarketData->Volume.ToString();
                dr[i++] = pDepthMarketData->OpenInterest.ToString();
                dr[i++] = (pDepthMarketData->Volume-nVolumn).ToString();
                dr[i++] = util::isInvalidValue(pDepthMarketData->OpenPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->OpenPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pDepthMarketData->PreSettlementPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->PreSettlementPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pDepthMarketData->HighestPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->HighestPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pDepthMarketData->LowestPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->LowestPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pDepthMarketData->UpperLimitPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->UpperLimitPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pDepthMarketData->LowerLimitPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pDepthMarketData->LowerLimitPrice,fPriceTick).c_str());
                dr[i++] = Tools::string2String(pDepthMarketData->UpdateTime);
                dr[i++] = util::isInvalidValue(pDepthMarketData->Turnover)?System::Convert::DBNull:pDepthMarketData->Turnover.ToString("F2");
                dr[i++] = util::isInvalidValue(pDepthMarketData->ClosePrice)?System::Convert::DBNull:(Tools::string2String(Price2string(pDepthMarketData->ClosePrice,fPriceTick).c_str()));
                dr[i++] = util::isInvalidValue(pDepthMarketData->SettlementPrice)?System::Convert::DBNull:(Tools::string2String(Price2string(pDepthMarketData->SettlementPrice,fPriceTick).c_str()));
                dr[i++] = util::isInvalidValue(pDepthMarketData->PreClosePrice)?System::Convert::DBNull:(Tools::string2String(Price2string(pDepthMarketData->PreClosePrice,fPriceTick).c_str()));
                dr[i++] = pDepthMarketData->PreOpenInterest.ToString("G");

                m_DataTableDepthMarketData->Rows->Add(dr);
            }

            map<string,int>::iterator it = g_mapCurrString2Volumn.find(pDepthMarketData->InstrumentID);
            if(it!=g_mapCurrString2Volumn.end())
                g_mapCurrString2Volumn[pDepthMarketData->InstrumentID] = pDepthMarketData->Volume;
            else
                g_mapCurrString2Volumn.insert(make_pair(pDepthMarketData->InstrumentID,pDepthMarketData->Volume));
        }
        void SetDataTable(RiskEvent* pRiskEvent)
        {            
//             LOG_INFO("接收到风险事件:\
//                 nTradeInvestorID=%d,InstrumentID=%s,nRiskEventID=%d,\
//                 nRiskEventSubID=%d,nRiskIndicatorID=%d,nRiskLevelID=%d\
//                 nIsValid=%d,nMsgSendStatus=%d,lEventTime=%d,dblIndicatorValue=%f",
//                 pRiskEvent->nTradeInvestorID,pRiskEvent->InstrumentID,pRiskEvent->nRiskEventID,
//                 pRiskEvent->nRiskEventSubID,pRiskEvent->nRiskIndicatorID,pRiskEvent->nRiskLevelID,
//                 pRiskEvent->nIsValid,pRiskEvent->nMsgSendStatus,pRiskEvent->lEventTime,pRiskEvent->dblIndicatorValue);

            if(m_DataTableRiskEvent == nullptr || pRiskEvent == NULL)
                return;

            string szTraderName,szIndicatorName,szEventStatus;
            char szIndicatorValue[64];

            UserInfo userInfo;
            if(!CRiskData::GetInstance()->GetUserInfoByID(pRiskEvent->nTradeInvestorID,userInfo))
                return;           

            map<RiskIndicatorType,RiskIndicator> mapRiskIndicator;
            CRiskData::GetInstance()->GetRiskIndicator(mapRiskIndicator);

            map<RiskIndicatorType,RiskIndicator>::iterator it = mapRiskIndicator.find(pRiskEvent->nRiskIndicatorID);
            if(it!=mapRiskIndicator.end())
                szIndicatorName = (string)(it->second.szName);
            else
                return;

            memset(szIndicatorValue,0,sizeof(szIndicatorValue));
            switch (pRiskEvent->nRiskIndicatorID)
            {
            case RI_MaxPosition://合约最大持仓;   合约持仓总数（买卖方向汇总）
                sprintf(szIndicatorValue, "%d手", (int)(pRiskEvent->dblIndicatorValue));
                break;
            case RI_MarketShock://市场冲击风险;   合约持仓总数（买卖方向汇总）占市场总持仓数百分比     
            case RI_AccountLoss://账户限亏		  （今日动态权益 -- 账户初始权益）/账户初始权益
            case RI_TodayLoss://今日限亏		  账户今日亏损占日初权益的百分比
            case RI_GappedMarket://隔夜跳空        账户各合约持仓综合后（买卖方向中和），预测第二天最大跳空亏损占账户初始权益百分比
            case RI_MarginRatio://保证金比例风险  保证金和动态权益的比值（风险度）
                sprintf(szIndicatorValue, "%.6f%%", pRiskEvent->dblIndicatorValue*100);
                break;
            case RI_SingleHoldTime://单笔持仓时间  
                sprintf(szIndicatorValue, "%d秒", (int)(pRiskEvent->dblIndicatorValue));
                break;
            case RI_FundNetValue://基金净值风险
                sprintf(szIndicatorValue, "%.5f", pRiskEvent->dblIndicatorValue);
                break;
            }


            String^ szRiskDesc = String::Empty;
            if( pRiskEvent->nRiskIndicatorID == RI_MaxPosition || 
                pRiskEvent->nRiskIndicatorID == RI_MarketShock ||
                pRiskEvent->nRiskIndicatorID == RI_SingleHoldTime)
                szRiskDesc = Tools::string2String(pRiskEvent->InstrumentID)+":"+pRiskEvent->nRiskLevelID.ToString()+"级风险";
            else
                szRiskDesc = pRiskEvent->nRiskLevelID.ToString()+"级风险";

            tm *tmThatTime = _localtime64(&(pRiskEvent->lEventTime));  
            char tTimeBuffer[30];  
            memset(tTimeBuffer, 0, 30);  
            sprintf(tTimeBuffer, "%04d/%02d/%02d %02d:%02d:%02d", tmThatTime->tm_year+1900, tmThatTime->tm_mon+1, tmThatTime->tm_mday, tmThatTime->tm_hour, tmThatTime->tm_min, tmThatTime->tm_sec);  

            switch(pRiskEvent->nMsgSendStatus)
            {
            case MsgStatus_New:				//服务器记录：发生
                szEventStatus = "发生";
                break;
            case MsgStatus_ChangeLevel:		//服务器记录：等级变更
                szEventStatus = "等级变更";
                break;
            case MsgStatus_Delete:			//服务器记录：消失
                szEventStatus = "消失";
                break;
            case MsgStatus_Appoint:			//手动操作，委派给交易员
                szEventStatus = "委派给交易员";
                break;
            case MsgStatus_Finished:			//手动操作，风险事件解决记录
                szEventStatus = "风险事件解决记录";
                break;
            case MsgStatus_ForceClose:		//手动操作，执行手动强平记录 
                szEventStatus = "执行手动强平记录";
                break;
            }

            DataRow^  dr = nullptr;
            dr = m_DataTableRiskEvent->Rows->Find(pRiskEvent->nRiskEventID.ToString());

            //更新
            if(dr)
            {
                if(pRiskEvent->nMsgSendStatus == MsgStatus_Delete)//风险事件消失
                {
                    m_DataTableRiskEvent->Rows->Remove(dr);

                     cliext::set<int>::iterator it = m_setRiskEventID.find(pRiskEvent->nRiskEventID);
                     if(it!=m_setRiskEventID.end())
                         m_setRiskEventID.erase(it);
                }
                else
                {
                    dr[3] = Tools::string2String(szIndicatorName.c_str());
                    dr[4] = szRiskDesc;
                    dr[5] = Tools::string2String(szIndicatorValue);
                    dr[6] = Tools::string2String(tTimeBuffer); 
                    dr[7] = Tools::string2String(szEventStatus.c_str());                     

                    if(IsNotTrigger(pRiskEvent->nRiskEventID) || (pRiskEvent->nIsValid == RiskEvent_Realtime && pRiskEvent->nMsgSendStatus == MsgStatus_ChangeLevel))
                        ProcessRiskAction(pRiskEvent);
                }
    
            }
            else//添加
            {                
                DataRow^ dr = m_DataTableRiskEvent->NewRow();
                int i=0;
                dr[i++] = pRiskEvent->nRiskEventID.ToString(); 
                dr[i++] = Tools::string2String(userInfo.szAccount); 
                dr[i++] = Tools::string2String(userInfo.szAlias);
                dr[i++] = Tools::string2String(szIndicatorName.c_str());
                dr[i++] = szRiskDesc;
                dr[i++] = Tools::string2String(szIndicatorValue);
                dr[i++] = Tools::string2String(tTimeBuffer); 
                dr[i++] = Tools::string2String(szEventStatus.c_str()); 

                m_DataTableRiskEvent->Rows->Add(dr);

                if(IsNotTrigger(pRiskEvent->nRiskEventID) || (pRiskEvent->nIsValid == RiskEvent_Realtime && pRiskEvent->nMsgSendStatus == MsgStatus_ChangeLevel))
                    ProcessRiskAction(pRiskEvent);
            }

            m_setRiskEventID.insert(pRiskEvent->nRiskEventID);
        }
        void SetDataTable(PlatformStru_TradingAccountInfo* pTradingAccountInfo)
        {
            

            UserInfo userInfo;
            if(!CRiskData::GetInstance()->GetUserInfoByAccount(pTradingAccountInfo->AccountID,userInfo))
                return;



            //过滤
            bool bSelected = false;
            for each (String^ szTraderAccount in m_vecTraderAccountFromTree)
            {
                if(0==strcmp(pTradingAccountInfo->AccountID,Tools::String2string(szTraderAccount).c_str()))
                {
                    bSelected = true;
                    break;
                }
            }

            if(!bSelected)
                return;



            bool bLimitTade = CRiskData::GetInstance()->IsLimitTrade(userInfo.nUserID);
            bool bManuVerify = CRiskData::GetInstance()->IsManualVerify(userInfo.nUserID);

            String^ szLoginStatus;
            map<string,bool> mapLogin;
            CRiskData::GetInstance()->GetUserLoginStatus(mapLogin);
            map<string,bool>::iterator it=mapLogin.find(userInfo.szAccount);
            if(it==mapLogin.end())
                szLoginStatus = "未登录"; 
            else
                szLoginStatus = "已登录";

            DataRow^  dr = nullptr;
            dr = m_DataTableTradingAccount->Rows->Find(Tools::string2String(userInfo.szAccount));


            //更新
            if(dr)
            {	
                //LOG_INFO("账户资金更新:szAccount=%s,PositionProfit=%f",userInfo.szAccount,pTradingAccountInfo->PositionProfit);
                //char buff[512] = {0};
                //sprintf(buff,"账户资金更新:szAccount=%s,PositionProfit=%f\n",userInfo.szAccount,pTradingAccountInfo->PositionProfit);
                //OutputDebugStr(buff);

                int i=1;
                dr[i++] = (bLimitTade)?("是"):("否");
                dr[i++] = (bManuVerify)?("是"):("否");
                dr[i++] = szLoginStatus;
                dr[i++] = pTradingAccountInfo->RiskDegree.ToString("N");
                dr[i++] = util::isInvalidValue(pTradingAccountInfo->DynamicProfit)?System::Convert::DBNull:pTradingAccountInfo->DynamicProfit.ToString("N");
                dr[i++] = util::isInvalidValue(pTradingAccountInfo->PositionProfit)?System::Convert::DBNull:pTradingAccountInfo->PositionProfit.ToString("N");
                dr[i++] = util::isInvalidValue(pTradingAccountInfo->PositionProfit)?System::Convert::DBNull:pTradingAccountInfo->CloseProfit.ToString("N");
                dr[i++] = util::isInvalidValue(pTradingAccountInfo->PositionProfit)?System::Convert::DBNull:pTradingAccountInfo->CurrMargin.ToString("N");
                dr[i++] = util::isInvalidValue(pTradingAccountInfo->PositionProfit)?System::Convert::DBNull:pTradingAccountInfo->Commission.ToString("N");
                dr[i++] = util::isInvalidValue(pTradingAccountInfo->PositionProfit)?System::Convert::DBNull:pTradingAccountInfo->FrozenMargin.ToString("N");
                dr[i++] = util::isInvalidValue(pTradingAccountInfo->PositionProfit)?System::Convert::DBNull:pTradingAccountInfo->FrozenCommission.ToString("N");
            }
            else//添加
            {            
                //LOG_INFO("账户资金添加:szAccount=%s,PositionProfit=%f",userInfo.szAccount,pTradingAccountInfo->PositionProfit);

                DataRow^ dr = m_DataTableTradingAccount->NewRow();
                int i=0;
                dr[i++] = Tools::string2String(userInfo.szAccount);
                dr[i++] = (bLimitTade)?("是"):("否");
                dr[i++] = (bManuVerify)?("是"):("否");
                dr[i++] = szLoginStatus;
                dr[i++] = pTradingAccountInfo->RiskDegree.ToString("N");
                dr[i++] = util::isInvalidValue(pTradingAccountInfo->DynamicProfit)?System::Convert::DBNull:pTradingAccountInfo->DynamicProfit.ToString("N");
                dr[i++] = util::isInvalidValue(pTradingAccountInfo->PositionProfit)?System::Convert::DBNull:pTradingAccountInfo->PositionProfit.ToString("N");
                dr[i++] = util::isInvalidValue(pTradingAccountInfo->PositionProfit)?System::Convert::DBNull:pTradingAccountInfo->CloseProfit.ToString("N");
                dr[i++] = util::isInvalidValue(pTradingAccountInfo->PositionProfit)?System::Convert::DBNull:pTradingAccountInfo->CurrMargin.ToString("N");
                dr[i++] = util::isInvalidValue(pTradingAccountInfo->PositionProfit)?System::Convert::DBNull:pTradingAccountInfo->Commission.ToString("N");
                dr[i++] = util::isInvalidValue(pTradingAccountInfo->PositionProfit)?System::Convert::DBNull:pTradingAccountInfo->FrozenMargin.ToString("N");
                dr[i++] = util::isInvalidValue(pTradingAccountInfo->PositionProfit)?System::Convert::DBNull:pTradingAccountInfo->FrozenCommission.ToString("N");
                m_DataTableTradingAccount->Rows->Add(dr);
            }

            dataGridView_AccountFundInfo->Invalidate();
        }
        void SetDataTable(PlatformStru_OrderInfo* pOrderInfo)
        {
            if(m_DataTableAllOrder == nullptr || pOrderInfo == NULL)
                return;

            //过滤
            bool bSelected = false;
            for each (String^ szTraderAccount in m_vecTraderAccountFromTree)
            {
                if(0==strcmp(pOrderInfo->InvestorID,Tools::String2string(szTraderAccount).c_str()))
                {
                    bSelected = true;
                    break;
                }
            }

            if(!bSelected)
                return;

            double fPriceTick = CRiskData::GetInstance()->GetPriceTickByInstID(pOrderInfo->InstrumentID);

            std::string strCombOffsetFlag,strHedgeFlag,strOrderStatus,strForceCloseReason;
            switch(pOrderInfo->CombOffsetFlag[0])
            {
            case THOST_FTDC_OF_Open:
                strCombOffsetFlag = "开仓";
                break;
            case THOST_FTDC_OF_Close:
                strCombOffsetFlag = "平仓";
                break;
            case THOST_FTDC_OF_ForceClose:
                strCombOffsetFlag = "强平";
                break;
            case THOST_FTDC_OF_CloseToday:
                strCombOffsetFlag = "平今";
                break;
            case THOST_FTDC_OF_CloseYesterday:
                strCombOffsetFlag = "平昨";
                break;
            case THOST_FTDC_OF_ForceOff:
                strCombOffsetFlag = "强减";
                break;
            case THOST_FTDC_OF_LocalForceClose:
                strCombOffsetFlag = "本地强平";
                break;
            }
            switch(pOrderInfo->CombHedgeFlag[0])
            {
            case THOST_FTDC_HF_Speculation: 
                strHedgeFlag = "投机";
                break;
            case THOST_FTDC_HF_Arbitrage:
                strHedgeFlag = "套利";
                break;
            case THOST_FTDC_HF_Hedge:
                strHedgeFlag = "套保";
                break;
            }

            switch(pOrderInfo->OrderStatus)
            {
            case THOST_FTDC_OST_AllTraded:
                strOrderStatus = "全部成交";
                break;
            case THOST_FTDC_OST_PartTradedQueueing:
                strOrderStatus = "部分成交还在队列中";
                break;
            case THOST_FTDC_OST_PartTradedNotQueueing:
                strOrderStatus = "部分成交不在队列中";
                break;
            case THOST_FTDC_OST_NoTradeQueueing:
                strOrderStatus = "未成交还在队列中";
                break;
            case THOST_FTDC_OST_NoTradeNotQueueing:
                strOrderStatus = "未成交不在队列中";
                break;
            case THOST_FTDC_OST_Canceled:
                strOrderStatus = "撤单";
                break;
            case THOST_FTDC_OST_Unknown:
                strOrderStatus = "未知";
                break;
            case THOST_FTDC_OST_NotTouched:
                strOrderStatus = "尚未触发";
                break;
            case THOST_FTDC_OST_Touched:
                strOrderStatus = "已触发";
                break;
            }

            switch(pOrderInfo->ForceCloseReason)
            {
            case THOST_FTDC_FCC_NotForceClose:
                strForceCloseReason = "非强平";
                break;
            case THOST_FTDC_FCC_LackDeposit:
                strForceCloseReason = "资金不足";
                break;
            case THOST_FTDC_OST_PartTradedNotQueueing:
                strForceCloseReason = "客户超仓";
                break;
            case THOST_FTDC_FCC_MemberOverPositionLimit:
                strForceCloseReason = "会员超仓";
                break;
            case THOST_FTDC_FCC_NotMultiple:
                strForceCloseReason = "持仓非整数倍";
                break;
            case THOST_FTDC_FCC_Violation:
                strForceCloseReason = "违规";
                break;
            case THOST_FTDC_FCC_Other:
                strForceCloseReason = "其它";
                break;
            case THOST_FTDC_FCC_PersonDeliv:
                strForceCloseReason = "自然人临近交割";
                break;
            case THOST_FTDC_FCC_ForceClose:
                strForceCloseReason = "强平";
                break;                
            }


            char szTip[256];
            memset(szTip,0,sizeof(szTip));
            if(THOST_FTDC_OST_Unknown == pOrderInfo->OrderStatus &&
                /*EnumExStatus::ExSta_approving*/1 == pOrderInfo->ExStatus)
            {
               sprintf(szTip,"%s交易员有待审核单需要审核",pOrderInfo->InvestorID);

            _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
                CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
                szTip);
            }

            cli::array<String^>^ key = {
                Tools::string2String(pOrderInfo->InvestorID),
                Tools::string2String(pOrderInfo->InstrumentID),
                pOrderInfo->FrontID.ToString(),
                pOrderInfo->SessionID.ToString(),
                Tools::string2String(pOrderInfo->OrderRef)
            };

            DataRow^  dr = nullptr;
            dr = m_DataTableAllOrder->Rows->Find(key);

            //更新
            if(dr)
            {						
                int i=0;
                dr[i++] = Tools::string2String(pOrderInfo->InvestorID);
                dr[i++] = Tools::string2String(pOrderInfo->OrderSysID);
                dr[i++] = Tools::string2String(pOrderInfo->InstrumentID);
                dr[i++] = (pOrderInfo->Direction == THOST_FTDC_D_Buy?"买":"卖");
                dr[i++] = Tools::string2String(strCombOffsetFlag.c_str());
                dr[i++] = Tools::string2String(strOrderStatus.c_str());
                dr[i++] = util::isInvalidValue(pOrderInfo->LimitPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pOrderInfo->LimitPrice,fPriceTick).c_str());
                dr[i++] = pOrderInfo->VolumeTotalOriginal.ToString();
                dr[i++] = pOrderInfo->VolumeTotal.ToString();
                dr[i++] = (pOrderInfo->VolumeTotalOriginal-pOrderInfo->VolumeTotal).ToString();
                dr[i++] = Tools::string2String(pOrderInfo->StatusMsg);
                dr[i++] = Tools::string2String(pOrderInfo->InsertTime);
                dr[i++] = util::isInvalidValue(pOrderInfo->freezeMargin)?System::Convert::DBNull:pOrderInfo->freezeMargin.ToString("N");
                dr[i++] = util::isInvalidValue(pOrderInfo->troubleMoney)?System::Convert::DBNull:pOrderInfo->troubleMoney.ToString("N");
                dr[i++] = Tools::string2String(strHedgeFlag.c_str());
                dr[i++] = Tools::string2String(pOrderInfo->ExchangeID) ;
                dr[i++] = pOrderInfo->FrontID.ToString() ;
                dr[i++] = pOrderInfo->SessionID.ToString() ;
                dr[i++] = Tools::string2String(pOrderInfo->OrderRef);
                dr[i++] = (pOrderInfo->ExStatus)?"使用":"未使用";
                dr[i++] = (pOrderInfo->UserForceClose?"是":"否");
                dr[i++] = Tools::string2String(strForceCloseReason.c_str()) ;
            }
            else//添加
            {                
                DataRow^ dr = m_DataTableAllOrder->NewRow();
                int i=0;
                dr[i++] = Tools::string2String(pOrderInfo->InvestorID);
                dr[i++] = Tools::string2String(pOrderInfo->OrderSysID);
                dr[i++] = Tools::string2String(pOrderInfo->InstrumentID);
                dr[i++] = (pOrderInfo->Direction == THOST_FTDC_D_Buy?"买":"卖");
                dr[i++] = Tools::string2String(strCombOffsetFlag.c_str());
                dr[i++] = Tools::string2String(strOrderStatus.c_str());
                dr[i++] = util::isInvalidValue(pOrderInfo->LimitPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pOrderInfo->LimitPrice,fPriceTick).c_str());
                dr[i++] = pOrderInfo->VolumeTotalOriginal.ToString();
                dr[i++] = pOrderInfo->VolumeTotal.ToString();
                dr[i++] = (pOrderInfo->VolumeTotalOriginal-pOrderInfo->VolumeTotal).ToString();
                dr[i++] = Tools::string2String(pOrderInfo->StatusMsg);
                dr[i++] = Tools::string2String(pOrderInfo->InsertTime);
                dr[i++] = util::isInvalidValue(pOrderInfo->freezeMargin)?System::Convert::DBNull:pOrderInfo->freezeMargin.ToString("N");
                dr[i++] = util::isInvalidValue(pOrderInfo->troubleMoney)?System::Convert::DBNull:pOrderInfo->troubleMoney.ToString("N");
                dr[i++] = Tools::string2String(strHedgeFlag.c_str());
                dr[i++] = Tools::string2String(pOrderInfo->ExchangeID) ;
                dr[i++] = pOrderInfo->FrontID.ToString() ;
                dr[i++] = pOrderInfo->SessionID.ToString() ;
                dr[i++] = Tools::string2String(pOrderInfo->OrderRef);
                dr[i++] = (pOrderInfo->ExStatus)?"使用":"未使用";
                dr[i++] = (pOrderInfo->UserForceClose?"是":"否");
                dr[i++] = Tools::string2String(strForceCloseReason.c_str()) ;

                m_DataTableAllOrder->Rows->Add(dr);
            }
        }
        void SetDataTable(PlatformStru_Position* pPosition)
        {           
            if(m_DataTablePosition == nullptr || pPosition == NULL)
                return;

            //过滤
            bool bSelected = false;
            for each (String^ szTraderAccount in m_vecTraderAccountFromTree)
            {
                if(0==strcmp(pPosition->InvestorID,Tools::String2string(szTraderAccount).c_str()))
                {
                    bSelected = true;
                    break;
                }
            }

            if(!bSelected)
                return;            

            //从交易端来的没有订阅的合约要订阅行情
            if(!CRiskData::GetInstance()->bSubScribeInst2((string)pPosition->InstrumentID))
            {
                CTcpDataConnect::SendData(Cmd_SubscribeQuotEvent_Req, pPosition->InstrumentID, strlen(pPosition->InstrumentID)+1, 0, m_pLogin->mnUserID);
                CRiskData::GetInstance()->SubScribeInst2(pPosition->ExchangeID,pPosition->InstrumentID);
            }
            

            double fPriceTick = CRiskData::GetInstance()->GetPriceTickByInstID(pPosition->InstrumentID);

            int nEmptyVolume = 0;
            std::string strPosiDirection,strHedgeFlag;

            switch(pPosition->PosiDirection)
            {
            case THOST_FTDC_PD_Long: 
                strPosiDirection = "买";
                break;
            case THOST_FTDC_PD_Short:
                strPosiDirection = "卖";
                break;
            case THOST_FTDC_PD_Net:
                strPosiDirection = "净";
                break;
            }

            switch(pPosition->HedgeFlag)
            {
            case THOST_FTDC_HF_Speculation: 
                strHedgeFlag = "投机";
                break;
            case THOST_FTDC_HF_Arbitrage:
                strHedgeFlag = "套利";
                break;
            case THOST_FTDC_HF_Hedge:
                strHedgeFlag = "套保";
                break;
            }

            
            cli::array<String^>^ key = {
                                            Tools::string2String(pPosition->InvestorID),
                                            Tools::string2String(pPosition->InstrumentID),
                                            Tools::string2String(strPosiDirection.c_str()),
                                            Tools::string2String(strHedgeFlag.c_str())
                                        };

            DataRow^  dr = nullptr;
            dr = m_DataTablePosition->Rows->Find(key); 

	         //更新
            if(dr)
            {	
//                 LOG_INFO("更新:szAccount=%s,InstrumentID=%s,PosiDirection=%c,HedgeFlag=%c",
//                     pPosition->szAccount,pPosition->InstrumentID,pPosition->PosiDirection,pPosition->HedgeFlag);

                //LOG_INFO("持仓更新:szAccount=%s,PositionProfit=%f",pPosition->InvestorID,pPosition->PositionProfit);

                int i=0;
                dr[i++] = Tools::string2String(pPosition->InvestorID);
                dr[i++] = Tools::string2String(pPosition->InstrumentID);
                dr[i++] = Tools::string2String(strPosiDirection.c_str());
                dr[i++] = Tools::string2String(strHedgeFlag.c_str());
                dr[i++] = pPosition->Position.ToString();
                dr[i++] = pPosition->CanCloseVolume.ToString();
                dr[i++] = util::isInvalidValue(GetLastPrice(pPosition->InstrumentID))?System::Convert::DBNull:GetLastPrice(pPosition->InstrumentID);
                dr[i++] = util::isInvalidValue(pPosition->AveragePositionPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pPosition->AveragePositionPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pPosition->AverageOpenPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pPosition->AverageOpenPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pPosition->PositionProfit)?System::Convert::DBNull:pPosition->PositionProfit.ToString("N");
                dr[i++] = util::isInvalidValue(pPosition->PositionProfitByTrade)?System::Convert::DBNull:pPosition->PositionProfitByTrade.ToString("N");
                dr[i++] = util::isInvalidValue(pPosition->TotalPositionProfitByDate)?System::Convert::DBNull:pPosition->TotalPositionProfitByDate.ToString("N");
                dr[i++] = util::isInvalidValue(pPosition->UseMargin)?System::Convert::DBNull:pPosition->UseMargin.ToString("N");
                dr[i++] = (pPosition->PosiDirection == THOST_FTDC_PD_Long)?pPosition->Position.ToString():"0";
                dr[i++] = (pPosition->PosiDirection == THOST_FTDC_PD_Short)?pPosition->Position.ToString():"0";
                dr[i++] = (pPosition->PosiDirection == THOST_FTDC_PD_Long)?pPosition->TodayPosition.ToString():"0";
                dr[i++] = (pPosition->PosiDirection == THOST_FTDC_PD_Short)?pPosition->TodayPosition.ToString():"0";
                dr[i++] = (pPosition->Position - pPosition->TodayPosition).ToString();
                dr[i++] = pPosition->TodayPosition.ToString();
                dr[i++] = pPosition->OpenVolume.ToString();
                dr[i++] = pPosition->CloseVolume.ToString();
                dr[i++] = pPosition->CanCloseTodayVolume.ToString();
                dr[i++] = pPosition->CanCloseydVolume.ToString();
                dr[i++] = pPosition->CloseTodayOrderVolume.ToString();
                dr[i++] = pPosition->CloseOrderVolume.ToString();
                dr[i++] = pPosition->CanCloseTodayVolume.ToString();
                dr[i++] = pPosition->CanCloseydVolume.ToString();
                dr[i++] = Tools::string2String(pPosition->ExchangeID); 
            }
            else//添加            
            {            
//                 LOG_INFO("添加:szAccount=%s,InstrumentID=%s,PosiDirection=%c,HedgeFlag=%c",
//                     pPosition->szAccount,pPosition->InstrumentID,pPosition->PosiDirection,pPosition->HedgeFlag);

                //LOG_INFO("持仓添加:szAccount=%s,PositionProfit=%f",pPosition->InvestorID,pPosition->PositionProfit);

                DataRow^ dr = m_DataTablePosition->NewRow();

                int i=0;
                dr[i++] = Tools::string2String(pPosition->InvestorID);
                dr[i++] = Tools::string2String(pPosition->InstrumentID);
                dr[i++] = Tools::string2String(strPosiDirection.c_str());
                dr[i++] = Tools::string2String(strHedgeFlag.c_str());
                dr[i++] = pPosition->Position.ToString();
                dr[i++] = pPosition->CanCloseVolume.ToString();
                dr[i++] = util::isInvalidValue(GetLastPrice(pPosition->InstrumentID))?System::Convert::DBNull:GetLastPrice(pPosition->InstrumentID);
                dr[i++] = util::isInvalidValue(pPosition->AveragePositionPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pPosition->AveragePositionPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pPosition->AverageOpenPrice)?System::Convert::DBNull:Tools::string2String(Price2string(pPosition->AverageOpenPrice,fPriceTick).c_str());
                dr[i++] = util::isInvalidValue(pPosition->PositionProfit)?System::Convert::DBNull:pPosition->PositionProfit.ToString("N");
                dr[i++] = util::isInvalidValue(pPosition->PositionProfitByTrade)?System::Convert::DBNull:pPosition->PositionProfitByTrade.ToString("N");
                dr[i++] = util::isInvalidValue(pPosition->TotalPositionProfitByDate)?System::Convert::DBNull:pPosition->TotalPositionProfitByDate.ToString("N");
                dr[i++] = util::isInvalidValue(pPosition->UseMargin)?System::Convert::DBNull:pPosition->UseMargin.ToString("N");
                dr[i++] = (pPosition->PosiDirection == THOST_FTDC_PD_Long)?pPosition->Position.ToString():"0";
                dr[i++] = (pPosition->PosiDirection == THOST_FTDC_PD_Short)?pPosition->Position.ToString():"0";
                dr[i++] = (pPosition->PosiDirection == THOST_FTDC_PD_Long)?pPosition->TodayPosition.ToString():"0";
                dr[i++] = (pPosition->PosiDirection == THOST_FTDC_PD_Short)?pPosition->TodayPosition.ToString():"0";
                dr[i++] = (pPosition->Position - pPosition->TodayPosition).ToString();
                dr[i++] = pPosition->TodayPosition.ToString();
                dr[i++] = pPosition->OpenVolume.ToString();
                dr[i++] = pPosition->CloseVolume.ToString();
                dr[i++] = pPosition->CanCloseTodayVolume.ToString();
                dr[i++] = pPosition->CanCloseydVolume.ToString();
                dr[i++] = pPosition->CloseTodayOrderVolume.ToString();
                dr[i++] = pPosition->CloseOrderVolume.ToString();
                dr[i++] = pPosition->CanCloseTodayVolume.ToString();
                dr[i++] = pPosition->CanCloseydVolume.ToString();
                dr[i++] = Tools::string2String(pPosition->ExchangeID); 

                m_DataTablePosition->Rows->Add(dr);

            }

            dataGridView_Position->Invalidate();
        }
        void SetDataTable(PlatformStru_TradeInfo* pTradeInfo)
        {
            if(m_DataTableTrade == nullptr || pTradeInfo == NULL)
                return;

            

            //过滤
            bool bSelected = false;
            for each (String^ szTraderAccount in m_vecTraderAccountFromTree)
            {
                if(0==strcmp(pTradeInfo->InvestorID,Tools::String2string(szTraderAccount).c_str()))
                {
                    bSelected = true;
                    break;
                }
            }

            if(!bSelected)
                return;

            double fPriceTick = CRiskData::GetInstance()->GetPriceTickByInstID(pTradeInfo->InstrumentID);

            std::string strDirection,strOffsetFlag,strHedgeFlag,strTradeType;

            strDirection = (pTradeInfo->Direction == THOST_FTDC_D_Buy?"买":"卖");

            switch(pTradeInfo->OffsetFlag)
            {
            case THOST_FTDC_OF_Open:
                strOffsetFlag = "开仓";
                break;
            case THOST_FTDC_OF_Close:
                strOffsetFlag = "平仓";
                break;
            case THOST_FTDC_OF_ForceClose:
                strOffsetFlag = "强平";
                break;
            case THOST_FTDC_OF_CloseToday:
                strOffsetFlag = "平今";
                break;
            case THOST_FTDC_OF_CloseYesterday:
                strOffsetFlag = "平昨";
                break;
            case THOST_FTDC_OF_ForceOff:
                strOffsetFlag = "强减";
                break;
            case THOST_FTDC_OF_LocalForceClose:
                strOffsetFlag = "本地强平";
                break;
            }
            switch(pTradeInfo->HedgeFlag)
            {
            case THOST_FTDC_HF_Speculation: 
                strHedgeFlag = "投机";
                break;
            case THOST_FTDC_HF_Arbitrage:
                strHedgeFlag = "套利";
                break;
            case THOST_FTDC_HF_Hedge:
                strHedgeFlag = "套保";
                break;
            }
            switch(pTradeInfo->TradeType)
            {
            case THOST_FTDC_TRDT_Common:
                strTradeType = "普通成交";
                break;
            case THOST_FTDC_TRDT_OptionsExecution:
                strTradeType = "期权执行";
                break;
            case THOST_FTDC_TRDT_OTC:
                strTradeType = "OTC成交";
                break;
            case THOST_FTDC_TRDT_EFPDerived:
                strTradeType = "期转现衍生成交";
                break;
            case THOST_FTDC_TRDT_CombinationDerived:
                strTradeType = "组合衍生成交";
                break;
            default:
                strTradeType = "普通成交";
                break;
            }

            //添加
            DataRow^ dr = m_DataTableTrade->NewRow();

            int i=0;
            dr[i++] = Tools::string2String(pTradeInfo->InvestorID);
            dr[i++] = Tools::string2String(pTradeInfo->InstrumentID);
            dr[i++] = Tools::string2String(strDirection.c_str());
            dr[i++] = Tools::string2String(strOffsetFlag.c_str());
			dr[i++] = util::isInvalidValue(pTradeInfo->Price)?System::Convert::DBNull:Tools::string2String(Price2string(pTradeInfo->Price,fPriceTick).c_str());
            dr[i++] = pTradeInfo->Volume.ToString();
            dr[i++] = util::isInvalidValue(pTradeInfo->TradeCommission)?System::Convert::DBNull:pTradeInfo->TradeCommission.ToString("N");
            dr[i++] = Tools::string2String(pTradeInfo->TradeTime);
            dr[i++] = Tools::string2String(strTradeType.c_str());
            dr[i++] = Tools::string2String(strHedgeFlag.c_str());
            dr[i++] = Tools::string2String(pTradeInfo->TradeID);
            dr[i++] = Tools::string2String(pTradeInfo->OrderSysID);
            dr[i++] = Tools::string2String(pTradeInfo->ExchangeID);

            m_DataTableTrade->Rows->Add(dr);
        }
	bool SubsUserMsg(int nUserID)
	{
		return CTcpLayer::SendData(Cmd_UserMsg_SubsMsg_Req, NULL, 0, 0, nUserID);
	}


private: System::Void FormMain_Load(System::Object^  sender, System::EventArgs^  e) {
             this->CenterToParent();

             m_DataTableDepthMarketData = gcnew System::Data::DataTable("DataTableDepthMarketData");
             m_DataTableRiskEvent = gcnew System::Data::DataTable("DataTableRiskEvent");            
             m_DataTableTradingAccount = gcnew System::Data::DataTable("DataTableTradingAccount");
             m_DataTableAllOrder = gcnew System::Data::DataTable("DataTableAllOrder"); 
             m_DataTableTrade = gcnew System::Data::DataTable("DataTableTrade");
             m_DataTablePosition = gcnew System::Data::DataTable("DataTablePosition");

             SetDataTableColumns();   

             dataGridView_QuotOrderTable->DataSource = m_DataTableDepthMarketData;
             dataGridView_RiskEvent->DataSource = m_DataTableRiskEvent;
             dataGridView_AccountFundInfo->DataSource = m_DataTableTradingAccount;
             dataGridView_AllOrder->DataSource = m_DataTableAllOrder;
             dataGridView_TradeRecord->DataSource = m_DataTableTrade;
             dataGridView_Position->DataSource = m_DataTablePosition;   

             //排序按数值大小，显示按N2
             SetDataGridViewCellStyle();

             //主键设置
             array<DataColumn^>^ dcDepthMarketData = gcnew array<DataColumn^>(1);
             dcDepthMarketData[0] = m_DataTableDepthMarketData->Columns[0];
             m_DataTableDepthMarketData->PrimaryKey = dcDepthMarketData;

             array<DataColumn^>^ dcRiskEvent = gcnew array<DataColumn^>(1);
             dcRiskEvent[0] = m_DataTableRiskEvent->Columns[0];
             m_DataTableRiskEvent->PrimaryKey = dcRiskEvent;

             array<DataColumn^>^ dcTradingAccount = gcnew array<DataColumn^>(1);
             dcTradingAccount[0] = m_DataTableTradingAccount->Columns[0];
             m_DataTableTradingAccount->PrimaryKey = dcTradingAccount;

             array<DataColumn^>^ dcAllOrder = gcnew array<DataColumn^>(5);
             dcAllOrder[0] = m_DataTableAllOrder->Columns[0];
             dcAllOrder[1] = m_DataTableAllOrder->Columns[2];
             dcAllOrder[2] = m_DataTableAllOrder->Columns[16];
             dcAllOrder[3] = m_DataTableAllOrder->Columns[17];
             dcAllOrder[4] = m_DataTableAllOrder->Columns[18];
             m_DataTableAllOrder->PrimaryKey = dcAllOrder;

             array<DataColumn^>^ dcPosition = gcnew array<DataColumn^>(4);
             dcPosition[0] = m_DataTablePosition->Columns[0];
             dcPosition[1] = m_DataTablePosition->Columns[1];
             dcPosition[2] = m_DataTablePosition->Columns[2];
             dcPosition[3] = m_DataTablePosition->Columns[3];
             m_DataTablePosition->PrimaryKey = dcPosition;


             m_DataViewUnTrade = gcnew System::Data::DataView(m_DataTableAllOrder);
             m_DataViewUnTrade->RowFilter = "挂单状态 IN ('部分成交还在队列中','未成交还在队列中')";//'部分成交不在队列中', '未成交不在队列中'
             dataGridView_UnTradeRecord->DataSource = m_DataViewUnTrade;

             m_DataViewNeedVerify = gcnew System::Data::DataView(m_DataTableAllOrder);
             m_DataViewNeedVerify->RowFilter = "挂单状态 = '未知' AND 状态 = '使用'";
             dataGridView_NeedVerify->DataSource = m_DataViewNeedVerify;             

             m_DataViewClosePosition = gcnew System::Data::DataView(m_DataTableTrade);
             m_DataViewClosePosition->RowFilter = "开平 = '平仓'";
             dataGridView_ClosePosition->DataSource = m_DataViewClosePosition;
             

             dataGridView_AllOrder->Columns["前置机"]->Visible = false;
             dataGridView_AllOrder->Columns["会话机"]->Visible = false;
             dataGridView_AllOrder->Columns["报单引用"]->Visible = false;
             dataGridView_AllOrder->Columns["状态"]->Visible = false;

             dataGridView_UnTradeRecord->Columns["前置机"]->Visible = false;
             dataGridView_UnTradeRecord->Columns["会话机"]->Visible = false;
             dataGridView_UnTradeRecord->Columns["报单引用"]->Visible = false;
             dataGridView_UnTradeRecord->Columns["状态"]->Visible = false;

             dataGridView_NeedVerify->Columns["前置机"]->Visible = false;
             dataGridView_NeedVerify->Columns["会话机"]->Visible = false;
             dataGridView_NeedVerify->Columns["报单引用"]->Visible = false;
             dataGridView_NeedVerify->Columns["状态"]->Visible = false;

             int nHwnd = this->Handle.ToInt32();
             CTcpLayer::SetMainWndHandle(nHwnd);
             CTcpDataConnect::SetMainWndHandle(nHwnd);
             //连接行情服务器
             CTcpDataConnect::Init();

             //等待连接上
             Sleep(2000);
             
             CRiskData::GetInstance()->GetLogonInfo(*m_pLogin);
    

             //密码修改
             CTcpLayer::SubscribePkg(Cmd_ModifyUserPassword_Rsp, nHwnd);


             //组织架构              
             CTcpLayer::SubscribePkg(Cmd_RM_QryOrganization_Rsp, nHwnd);
             CTcpLayer::SendData(Cmd_RM_QryOrganization_Req, NULL,0, 0,m_pLogin->mnUserID);
             //交易员
             CTcpLayer::SubscribePkg(Cmd_RM_QryUser_Rsp, nHwnd);
             CTcpLayer::SendData(Cmd_RM_QryUser_Req, NULL,0, 0, m_pLogin->mnUserID);  
             CTcpLayer::SubscribePkg(CMDID_AllUserLoginStatus_Push, nHwnd);
             CTcpLayer::SubscribePkg(CMDID_SubscriberAllUserLoginStatus_Rsp, nHwnd);
             CTcpLayer::SendData(CMDID_SubscriberAllUserLoginStatus_Req, NULL,0, 0, m_pLogin->mnUserID);
             //交易员交易限制和审核
             CTcpLayer::SubscribePkg(Cmd_RM_SetLimitTrade_Rsp	, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_RM_SetManualVerify_Rsp, nHwnd);
			 CTcpLayer::SubscribePkg(Cmd_RM_SetLimitAndVerify_Rsp, nHwnd);			 
             //交易员与组织机构关系    
             CTcpLayer::SubscribePkg(Cmd_RM_QryUserAndOrgRelation_Rsp, nHwnd);
             CTcpLayer::SendData(Cmd_RM_QryUserAndOrgRelation_Req, NULL,0, m_pLogin->mnUserID);
             //交易员与委托交易账号关系
             CTcpLayer::SubscribePkg(Cmd_RM_QryTraderAndTradeAccountRelation_Rsp, nHwnd);
             CTcpLayer::SendData(Cmd_RM_QryTraderAndTradeAccountRelation_Req, NULL,0, 0, m_pLogin->mnUserID); 
             //风控指标
             CTcpLayer::SubscribePkg(Cmd_RM_QryRiskIndicator_Rsp, nHwnd);
             CTcpLayer::SendData(Cmd_RM_QryRiskIndicator_Req, NULL,0, 0, m_pLogin->mnUserID);
             //风控动作
             CTcpLayer::SubscribePkg(Cmd_RM_QryResponse_Rsp, nHwnd);
             CTcpLayer::SendData(Cmd_RM_QryResponse_Req, NULL, 0, 0, m_pLogin->mnUserID);
             //风控方案 在加载完组织机构 风控动作 风控指标后请求
             CTcpLayer::SubscribePkg(Cmd_RM_AddRiskPlan_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_RM_DeleteRiskPlan_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_RM_UseRiskPlanAdd_Rsp,nHwnd);                
             CTcpLayer::SubscribePkg(Cmd_RM_QueryRiskPlan_Rsp, nHwnd);
             //风控事件 在加载完组织机构 风控动作 风控指标后后请求
             CTcpLayer::SubscribePkg(Cmd_RM_RiskEvent_Push, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_RM_SubscribeRiskEvent_Rsp, nHwnd); 
             //合约 IF、TF
             CTcpLayer::SubscribePkg(Cmd_RM_QryProducts_Rsp, nHwnd);
             CTcpLayer::SendData(Cmd_RM_QryProducts_Req, NULL, 0, 0, m_pLogin->mnUserID);
             //合约品种 IF1308、TF1314
             PlatformStru_QryInstrument Field;
             CTcpLayer::SubscribePkg(Cmd_TradeDataManager_QryInstrument_Rsp, nHwnd);
             CTcpLayer::SendData(Cmd_TradeDataManager_QryInstrument_Req, &Field, sizeof(PlatformStru_QryInstrument), 0, m_pLogin->mnUserID);             
             //行情 在加载完合约后请求
             CTcpDataConnect::SubscribePkg(Cmd_QuotEvent_Push, nHwnd);
             CTcpDataConnect::SubscribePkg(Cmd_SubscribeQuotEvent_Rsp, nHwnd);  
             CTcpDataConnect::SubscribePkg(Cmd_UnSubscribeQuotEvent_Rsp,nHwnd);
             map<string,vector<string>> mapExchange2Inst = CRiskData::GetInstance()->GetConfigFile(XML_TYPE_INST);             
             for(map<string,vector<string>>::iterator it = mapExchange2Inst.begin();it!=mapExchange2Inst.end();it++)
             {
                 for (vector<string>::iterator it2 = it->second.begin();it2!=it->second.end();it2++)
                 {
                     CTcpDataConnect::SendData(Cmd_SubscribeQuotEvent_Req, (char*)(*it2).c_str(), strlen((*it2).c_str())+1, 0, m_pLogin->mnUserID);
                 }
             }
             //订阅资金 在加载完组织机构后请求
             CTcpLayer::SubscribePkg(Cmd_RM_Fund_Push, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_RM_SubscribeFund_Rsp, nHwnd);             
             //订阅报单 在加载完组织机构后请求
             CTcpLayer::SubscribePkg(Cmd_RM_Order_Push, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_RM_SubscribeOrder_Rsp, nHwnd);
             //订阅持仓 在加载完组织机构后请求
             CTcpLayer::SubscribePkg(Cmd_RM_Position_Push, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_RM_SubscribePosition_Rsp, nHwnd);
             //订阅成交 在加载完组织机构后请求
             CTcpLayer::SubscribePkg(Cmd_RM_Trade_Push, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_RM_SubscribeTrade_Rsp, nHwnd);         
             //撤单
             CTcpLayer::SubscribePkg(Cmd_RM_OrderAction_Rsp, nHwnd);
             //手动强平
             CTcpLayer::SubscribePkg(Cmd_RM_ForceCloseOrderInsert_Rsp, nHwnd);
             //订阅推送消息
			 CTcpLayer::SubscribePkg(Cmd_UserMsg_PushMsg, nHwnd);

             CTcpLayer::SubscribePkg(CMDID_GetALLError_Push, nHwnd);

			 SubsUserMsg(m_pLogin->mnUserID);

             timer_FormMain->Start();

             m_FormLockWindowDlg = gcnew FormLockWindow(this);
         }
private: System::Void timer_FormMain_Tick(System::Object^  sender, System::EventArgs^  e) {
             CTcpLayer::SendData(CMDID_SubscriberAllUserLoginStatus_Req, NULL,0, 0, m_pLogin->mnUserID);

             if( !m_bLoad &&
                 CRiskData::GetInstance()->GetOrganizationLoad() && 
                 CRiskData::GetInstance()->GetUserAndOrgRelationLoad() &&
                 CRiskData::GetInstance()->GetUserInfoLoad() &&
                 CRiskData::GetInstance()->GetUserAndTraderAccountRelationLoad()&&
                 CRiskData::GetInstance()->GetRiskIndicatorLoad()&&
                 CRiskData::GetInstance()->GetRiskResponseLoad())
             {
                 m_bLoad = true;

                 //加载组织机构
                 vector<int> vecUserID;
                 vector<int> vecOrgID;
                 UserInfo userInfo;

                 CRiskData::GetInstance()->GetOrgIDByUserID(m_pLogin->mnUserID,USER_ORG_RELATION_ATTACH,vecOrgID);                 
                 if(!vecOrgID.empty())
                 {                     
                     LoadAssertMgmtOrgan(vecOrgID[0],vecUserID); 

                     vecOrgID.clear();
                     CRiskData::GetInstance()->GetOrgIDByUserID(m_pLogin->mnUserID,USER_ORG_RELATION_MONITOR,vecOrgID);
                     if(!vecOrgID.empty())
                         CTcpLayer::SendData(Cmd_RM_QueryRiskPlan_Req, &vecOrgID[0],sizeof(int)*vecOrgID.size(), 0, m_pLogin->mnUserID);

                     if(!vecUserID.empty())
                     {                         
                         for(vector<int>::iterator it=vecUserID.begin();it!=vecUserID.end();it++)
                         {
                             if(CRiskData::GetInstance()->GetUserInfoByID(*it,userInfo))
                                 m_vecTraderAccountFromTree.push_back(Tools::string2String(userInfo.szAccount));
                         }

                         //风险事件 程序启动后到结束只订阅一次，不存在退订
                         CTcpLayer::SendData(Cmd_RM_SubscribeRiskEvent_Req, &vecUserID[0],sizeof(int)*vecUserID.size(), 0, m_pLogin->mnUserID);

                         Subscribe(vecUserID);
                     }
                 }
             }
         }
protected:virtual void WndProc( Message% m ) override{
              if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_DataArrival )
              {
                  CTcpLayer::ParseRecvPackageInWindowThread();
              } 
              else if (  m.Msg == WM_USER_Win_Quot && m.WParam.ToInt32()==WndUserCmd_DataArrival_Quot )
              {
                  CTcpDataConnect::ParseRecvPackageInWindowThread();
              }
              else if( m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_ConnStatus )
              {
                  int nConnectStatus = m.LParam.ToInt32();
                  switch(nConnectStatus)
                  {
                  case 0:
                      toolStripStatusLabel1->Text = "与交易服务器断开连接,请重新登录";
                      break;
                  case 1:
                      //toolStripStatusLabel1->Text = "与交易服务器连接中...";
                      break;
                  case 2:
                      toolStripStatusLabel1->Text = "与交易服务器连接成功";
                      break;
                  case 3:
                      //toolStripStatusLabel1->Text = "与交易服务器连接失败,,请重新登录";
                      break;
                  }
              } 
              else if( m.Msg == WM_USER_Win_Quot && m.WParam.ToInt32()==WndUserCmd_QuotConnStatus_Quot )
              {
                  int nConnectStatus = m.LParam.ToInt32();
                  switch(nConnectStatus)
                  {
                  case 0:
                      toolStripStatusLabel2->Text = "与行情服务器断开连接,请重新登录";
                      break;
                  case 1:
                      //toolStripStatusLabel2->Text = "与行情服务器连接中...";
                      break;
                  case 2:
                      toolStripStatusLabel2->Text = "与行情服务器连接成功";
                      break;
                  case 3:
                      //toolStripStatusLabel2->Text = "与行情服务器连接失败,请重新登录";
                      break;
                  }
              }
              else if ( m.Msg == WM_USER_Win_Quot && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival_Quot )
              {
                  char *p = (char*)m.LParam.ToInt32();
                  Stru_UniPkgHead head;					
                  memset(&head, 0, sizeof(head));
                  memcpy(&head, p, sizeof(head));

                  switch ( head.cmdid )
                  {	 
                  case Cmd_QuotEvent_Push://行情
                      {          
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              int nCount = head.len / sizeof(PlatformStru_DepthMarketData);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  PlatformStru_DepthMarketData* pDepthMarketData = (PlatformStru_DepthMarketData*)(p+sizeof(Stru_UniPkgHead) + i*sizeof(PlatformStru_DepthMarketData));
                                  SetDataTable(pDepthMarketData);

                                  sInstrumentInfo instrumentInfo;
                                  //instrumentInfo.Exchange = (string)pDepthMarketData->ExchangeID;
                                  instrumentInfo.Volume = pDepthMarketData->Volume;
                                  instrumentInfo.fLastPrice = pDepthMarketData->LastPrice;
                                  instrumentInfo.fUpperLimitPrice = pDepthMarketData->UpperLimitPrice;
                                  instrumentInfo.fLowerLimitPrice = pDepthMarketData->LowerLimitPrice;

                                  CRiskData::GetInstance()->SetInstrumentInfo((string)pDepthMarketData->InstrumentID,instrumentInfo);
                              }
                          }
                          break;
                      }
                  case Cmd_SubscribeQuotEvent_Rsp:
                      {

                          break;
                      }
                  case Cmd_UnSubscribeQuotEvent_Rsp:
                      {
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              char* pInstrument = (char*)(p+sizeof(Stru_UniPkgHead));

                              //将此合约从datagridview删除
                              DataRow^  dr = nullptr;
                              dr = m_DataTableDepthMarketData->Rows->Find(Tools::string2String(pInstrument));
                              if(dr!=nullptr)
                                  m_DataTableDepthMarketData->Rows->Remove(dr);
                          }
                          break;
                      }
                  }
              }
              else if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival )
              {
                  char *p = (char*)m.LParam.ToInt32();
                  Stru_UniPkgHead head;					
                  memset(&head, 0, sizeof(head));
                  memcpy(&head, p, sizeof(head));

                  String^ strError = String::Empty;
                  bool bShowMsg = true;

                  switch ( head.cmdid )
                  {	 
                  case Cmd_ModifyUserPassword_Rsp:
                      {
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {	
                              strError = "修改密码操作成功";
                          }
                          else
                          {
                              strError = "修改密码操作失败,原因是："+ Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }
                          break;
                      }

                  case Cmd_RM_QryOrganization_Rsp:
                      {
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              CRiskData::GetInstance()->ClearOrganization();

                              map<int, Organization> mapAssetMgmtOrg;
                              int nCount = head.len / sizeof(Organization);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  Organization* pAssetMgmtOrg = (Organization*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(Organization));
                                  mapAssetMgmtOrg[pAssetMgmtOrg->nAssetMgmtOrgID] = *pAssetMgmtOrg;
                              }

                              CRiskData::GetInstance()->SetOrganization(mapAssetMgmtOrg);
                              CRiskData::GetInstance()->SetOrganizationLoad(true);
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      }
                  case Cmd_RM_QryUser_Rsp:
                      {                
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              CRiskData::GetInstance()->ClearUserInfo();

                              map<int, UserInfo> mapUserInfo;
                              int nCount = head.len / sizeof(UserInfo);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  UserInfo* pUserInfo = (UserInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(UserInfo));
                                  mapUserInfo[pUserInfo->nUserID] = *pUserInfo;
                              }

                              CRiskData::GetInstance()->SetUserInfo(mapUserInfo);
                              CRiskData::GetInstance()->SetUserInfoLoad(true);

                              UserInfo userInfo;
                              CRiskData::GetInstance()->GetUserInfoByID(m_pLogin->mnUserID,userInfo);
                              this->Text = L"风控终端-" + Tools::string2String(userInfo.szAccount);
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }
                          break;
                      }
                  case Cmd_RM_SetLimitTrade_Rsp://交易限制
                      {                              
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              SLimitTrade* pSLimitTrade = (SLimitTrade*)(char*)(p+sizeof(Stru_UniPkgHead));
                              CRiskData::GetInstance()->SetLimitTrade(pSLimitTrade);

                              for each(TreeNode^ node in treeView_Organization->Nodes)
                                  UpdateTextColor(node,pSLimitTrade);

                              Update_dataGridView_AccountFundInfo(pSLimitTrade);

                              strError = "设置交易限制操作成功";
                          }
                          else
                          {
                              strError = "设置交易限制操作失败,原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }
                          break;
                      }
                  case Cmd_RM_SetManualVerify_Rsp://交易审核
                      {                              
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              SManualVerify* pSManualVerify = (SManualVerify*)(char*)(p+sizeof(Stru_UniPkgHead));
                              CRiskData::GetInstance()->SetManualVerify(pSManualVerify);

                              for each(TreeNode^ node in treeView_Organization->Nodes)
                                  UpdateTextColor(node,pSManualVerify);

                              Update_dataGridView_AccountFundInfo(pSManualVerify);

                              strError = "设置交易审核操作成功";
                          }
                          else
                          {
                              strError = "设置交易审核操作失败,原因是：" + Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }
                          break;
                      }
                  case Cmd_RM_SetLimitAndVerify_Rsp://交易限制 交易审核 来自后台管理终端的修改
                      {           
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              UserAndTradeAccountRelation* pRelation = (UserAndTradeAccountRelation*)(char*)(p+sizeof(Stru_UniPkgHead));
                              SLimitTrade sLimit;
                              sLimit.nUserID = pRelation->nUserID;
                              sLimit.bLimitTrade = (pRelation->nCanInputOrder==0)?true:false;
                              CRiskData::GetInstance()->SetLimitTrade(&sLimit);
                              
                              for each(TreeNode^ node in treeView_Organization->Nodes)
                                  UpdateTextColor(node,&sLimit);



                              SManualVerify sVerify;
                              sVerify.nUserID = pRelation->nUserID;
                              sVerify.bManualVerify = (pRelation->nNeedVerify==0)?false:true;
                              CRiskData::GetInstance()->SetManualVerify(&sVerify);

                              for each(TreeNode^ node in treeView_Organization->Nodes)
                                  UpdateTextColor(node,&sVerify);

                              Update_dataGridView_AccountFundInfo(pRelation);
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }
                          break;
                      }
                  //case CMDID_GetALLLoginStatus_Rsp:
                  case CMDID_SubscriberAllUserLoginStatus_Rsp:
                  case CMDID_AllUserLoginStatus_Push:
                      {
                          bShowMsg = false;
                          if ( head.userdata3 == CF_ERROR_SUCCESS )
                          {
                              int nCount = head.len / sizeof(int);
                              map<string,bool> mapLogin;
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  int nUserID = *(int*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(int));

                                  UserInfo userInfo;
                                  if(CRiskData::GetInstance()->GetUserInfoByID(nUserID,userInfo))
                                      mapLogin.insert(make_pair(userInfo.szAccount,true));
                              }

                              CRiskData::GetInstance()->SetUserLoginStatus(mapLogin);

                              Update_dataGridView_AccountFundInfo(mapLogin);
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      }
                  case Cmd_RM_QryUserAndOrgRelation_Rsp:
                      {
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              CRiskData::GetInstance()->ClearUserAndOrgRelation();

                              vector<UserAndOrgRelation> vecUserAndOrgRelation;
                              int nCount = head.len / sizeof(UserAndOrgRelation);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  UserAndOrgRelation* pUserAndOrgRelation = (UserAndOrgRelation*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(UserAndOrgRelation));
                                  vecUserAndOrgRelation.push_back(*pUserAndOrgRelation);                             
                              }

                              CRiskData::GetInstance()->SetUserAndOrgRelation(vecUserAndOrgRelation);	
                              CRiskData::GetInstance()->SetUserAndOrgRelationLoad(true);
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      }
                  case Cmd_RM_QryTraderAndTradeAccountRelation_Rsp:
                      {
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              CRiskData::GetInstance()->ClearUserAndTraderAccountRelation();
                              map<int, UserAndTradeAccountRelation> mapUserAndTradeAccountRelation;
                              int nCount = head.len / sizeof(UserAndTradeAccountRelation);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  UserAndTradeAccountRelation* pRelation = (UserAndTradeAccountRelation*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(UserAndTradeAccountRelation));
                                  mapUserAndTradeAccountRelation.insert(make_pair(pRelation->nUserID, *pRelation));                             
                              }

                              CRiskData::GetInstance()->SetUserAndTraderAccountRelation(mapUserAndTradeAccountRelation);
                              CRiskData::GetInstance()->SetUserAndTraderAccountRelationLoad(true);
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      } 
                  case Cmd_RM_QryRiskIndicator_Rsp:
                      {
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              CRiskData::GetInstance()->ClearRiskIndicator();

                              map<RiskIndicatorType, RiskIndicator> mapRiskIndicator;
                              int nCount = head.len / sizeof(RiskIndicator);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  RiskIndicator* pRiskIndicator = (RiskIndicator*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(RiskIndicator));
                                  mapRiskIndicator[pRiskIndicator->nRiskIndicatorID] = *pRiskIndicator;
                              }

                              CRiskData::GetInstance()->SetRiskIndicator(mapRiskIndicator);  
                              CRiskData::GetInstance()->SetRiskIndicatorLoad(true);
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      }
                  case Cmd_RM_AddRiskPlan_Rsp:
                      {
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              CRiskData::GetInstance()->UpdateRiskPlan(g_riskPlanOrg,g_vecRiskPlan);
                              CRiskData::GetInstance()->UpdateRiskPlan(g_riskPlanKey,g_vecRiskPlan);

                              if(m_FormRiskPlanSetup_Add->GetOrgText() == String::Empty || 
                                 m_FormRiskPlanSetup_Add->GetRiskIndicatorText() == String::Empty)
                                  return;

                              if(IsHaveItem(m_FormRiskPlanSetup_Add->GetOrgText(),m_FormRiskPlanSetup_Add->GetRiskIndicatorText()))
                                  return;

                              int index=dataGridView_OrgRiskSetup->Rows->Add();
                              dataGridView_OrgRiskSetup->Rows[index]->Cells[0]->Value = m_FormRiskPlanSetup_Add->GetOrgText(); 
                              dataGridView_OrgRiskSetup->Rows[index]->Cells[1]->Value = m_FormRiskPlanSetup_Add->GetRiskIndicatorText(); 
                              dataGridView_OrgRiskSetup->Rows[index]->Cells[2]->Value = "停用";

                              strError = "增加风控方案操作成功";
                          }
                          else
                          {
                              strError = "增加风控方案操作失败,原因是：" + Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      }
                  case Cmd_RM_DeleteRiskPlan_Rsp:
                      {
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              CRiskData::GetInstance()->DelRiskPlan(g_riskPlanOrg,g_riskPlanKey);

                              int nCount = head.len / sizeof(RiskChange);
                              if(nCount==0)
                                  break;

                              RiskChange* pRiskPlan = (RiskChange*)((char*)(p+sizeof(Stru_UniPkgHead)));

                              int nOrgID;
                              RiskIndicatorType nRiskIndicatorID;

                              for (int i=0;i<dataGridView_OrgRiskSetup->Rows->Count;i++)
                              {
                                  String^ szOrgName = dataGridView_OrgRiskSetup->Rows[i]->Cells[0]->Value->ToString();
                                  String^ szIndicator = dataGridView_OrgRiskSetup->Rows[i]->Cells[1]->Value->ToString();
                                  CRiskData::GetInstance()->GetOrganizationIDByName(Tools::String2string(szOrgName),nOrgID);
                                  CRiskData::GetInstance()->GetIndicatorIDByName(Tools::String2string(szIndicator),nRiskIndicatorID);

                                  if(pRiskPlan->nOrgID == nOrgID && pRiskPlan->nRiskIndicatorID == nRiskIndicatorID)
                                  {
                                        dataGridView_OrgRiskSetup->Rows->RemoveAt(i);
                                        break;
                                  }
                              }

                              strError = "删除风控方案操作成功";
                          }
                          else
                          {
                              strError = "删除风控方案操作失败,原因是：" + Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      }
                  case Cmd_RM_UseRiskPlanAdd_Rsp:
                      { 
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              int nCount = head.len / sizeof(RiskChange);
                              if(nCount==0)
                                  break;      

                              String^ szText = String::Empty;
                              szText = dataGridView_OrgRiskSetup->SelectedRows[0]->Cells[2]->Value->ToString(); 

                              dataGridView_OrgRiskSetup->SelectedRows[0]->Cells[2]->Value = (0==szText->CompareTo("启用"))?"停用":"启用";

                              RiskChange* pRiskPlan = (RiskChange*)((char*)(p+sizeof(Stru_UniPkgHead)));
                              CRiskData::GetInstance()->SetRiskPlanStatus(*pRiskPlan);

                              strError = (0==szText->CompareTo("启用"))?"风控方案启用操作成功":"风控方案停用操作成功";
                          }
                          else
                          {
                              String^ szText = String::Empty;
                              szText = dataGridView_OrgRiskSetup->SelectedRows[0]->Cells[2]->Value->ToString(); 
                              strError = (0==szText->CompareTo("启用"))?"风控方案停用操作失败":"风控方案启用操作失败,失败原因是：" + Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      }
                  case Cmd_RM_QueryRiskPlan_Rsp:
                      {
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              dataGridView_OrgRiskSetup->Rows->Clear();
                              CRiskData::GetInstance()->ClearRiskPlan();

                              string szOrgName,szRiskIndicator;
                              map<string, vector<RiskPlan>> mapRiskPlan;
                              map<Key_RiskPlan,vector<RiskPlan>> mapKey2RiskPlan;
                              int nCount = head.len / sizeof(RiskPlan);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  RiskPlan* pRiskPlan = (RiskPlan*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(RiskPlan));
                                  if(CRiskData::GetInstance()->GetOrganizationLoad() && 
                                     CRiskData::GetInstance()->GetOrganizationNameByID(pRiskPlan->OrgIDPlanRelation.nOrgID,szOrgName))
                                  {
                                      mapRiskPlan[szOrgName].push_back(*pRiskPlan);

                                      Key_RiskPlan key(pRiskPlan->OrgIDPlanRelation.nOrgID,pRiskPlan->OrgIDPlanRelation.nRiskIndicatorID);
                                      mapKey2RiskPlan[key].push_back(*pRiskPlan);

                                      switch(pRiskPlan->OrgIDPlanRelation.nRiskIndicatorID)
                                      {
                                      case RI_MaxPosition:
                                          szRiskIndicator = "合约最大持仓";
                                          break;
                                      case RI_MarketShock:
                                          szRiskIndicator = "市场冲击风险";
                                          break;
                                      case RI_AccountLoss:
                                          szRiskIndicator = "账户限亏";
                                          break;
                                      case RI_TodayLoss:
                                          szRiskIndicator = "今日限亏";
                                          break;
                                      case RI_GappedMarket:
                                          szRiskIndicator = "隔夜跳空";
                                          break;
                                      case RI_MarginRatio:
                                          szRiskIndicator = "保证金比例风险";
                                          break;
                                      case RI_SingleHoldTime:
                                          szRiskIndicator = "单笔持仓时间";
                                          break;
                                      case RI_FundNetValue:
                                          szRiskIndicator = "基金净值风险";
                                          break;
                                       }



                                      bool bFind = false;
                                      int nOrgID;
                                      RiskIndicatorType nRiskIndicatorID;

                                      map<RiskIndicatorType,RiskIndicator> mapRiskIndicator;
                                      CRiskData::GetInstance()->GetRiskIndicator(mapRiskIndicator);
                                      if((int)mapRiskIndicator.size()>0)
                                      {
                                          int nRowCount = this->dataGridView_OrgRiskSetup->Rows->Count;
                                          for (int i=0; i<nRowCount; i++)
                                          {  
                                              DataGridViewRow^ dataGridViewRow = dataGridView_OrgRiskSetup->Rows[i];                                           
                                              CRiskData::GetInstance()->GetOrganizationIDByName(Tools::String2string(dataGridViewRow->Cells[0]->Value->ToString()),nOrgID);
                                              CRiskData::GetInstance()->GetIndicatorIDByName(Tools::String2string(dataGridViewRow->Cells[1]->Value->ToString()),nRiskIndicatorID);

                                              if (key.first == nOrgID && key.second == nRiskIndicatorID)  
                                              {  
                                                  bFind = true;
                                                  break;
                                              }
                                          }

                                          if(bFind)
                                              continue;

                                          int index=dataGridView_OrgRiskSetup->Rows->Add();
                                          dataGridView_OrgRiskSetup->Rows[index]->Cells[0]->Value = Tools::string2String(szOrgName.c_str()); 
                                          dataGridView_OrgRiskSetup->Rows[index]->Cells[1]->Value = Tools::string2String(szRiskIndicator.c_str()); 
                                          dataGridView_OrgRiskSetup->Rows[index]->Cells[2]->Value = pRiskPlan->OrgIDPlanRelation.bUse?"停用":"启用";
                                      }

                                  }
                              }

                              CRiskData::GetInstance()->SetRiskPlan(mapRiskPlan);  
                              CRiskData::GetInstance()->SetKey2RiskPlan(mapKey2RiskPlan);
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      }
                  case Cmd_RM_QryResponse_Rsp:
                      {
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              CRiskData::GetInstance()->ClearRiskResponse();

                              vector<RiskResponse> vecResponse;
                              int nCount = head.len / sizeof(UserInfo);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  RiskResponse* pResponse = (RiskResponse*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(RiskResponse));
                                  vecResponse.push_back(*pResponse);;
                              }

                              CRiskData::GetInstance()->SetRiskResponse(vecResponse);
                              CRiskData::GetInstance()->SetRiskResponseLoad(true);
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }
                          break;

                      }
                  case Cmd_RM_SubscribeRiskEvent_Rsp://风险事件返回
                  case Cmd_RM_RiskEvent_Push:        //风险事件推送
                      {            
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {	
                              int nCount = head.len / sizeof(RiskEvent);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  RiskEvent* pRiskEvent = (RiskEvent*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(RiskEvent));
                                  SetDataTable(pRiskEvent);
                              }
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));				
                          }

                          break;
                      }
                  case Cmd_RM_QryProducts_Rsp://品种
                      {
                          bShowMsg = false;
                          if ( head.userdata3 == CF_ERROR_SUCCESS )
                          {
                              vector<SProduct> vecProduct;
                              int nCount = head.len / sizeof(SProduct);
                              if(nCount == 0)
                                  break;
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  SProduct* pProduct = (SProduct*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(SProduct));
                                  vecProduct.push_back(*pProduct);
                              }

                              CRiskData::GetInstance()->SetInstrumentStyle(vecProduct);
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      }

                  case Cmd_TradeDataManager_QryInstrument_Rsp://合约
                      {
                          bShowMsg = false;
                          if ( head.userdata3 == CF_ERROR_SUCCESS )
                          {
                              int nCount = head.len / sizeof(PlatformStru_InstrumentInfo);
                              if(nCount == 0)
                                  break;

                              map<string, double> mapInst2Tick;
                              map<string, vector<string>>   mapExchange2Instrument;
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  PlatformStru_InstrumentInfo* pInstrumentInfo = (PlatformStru_InstrumentInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(PlatformStru_InstrumentInfo));
                                  mapExchange2Instrument[pInstrumentInfo->ExchangeID].push_back(pInstrumentInfo->InstrumentID);  
                                  mapInst2Tick[pInstrumentInfo->InstrumentID] = pInstrumentInfo->PriceTick;
                              }

                              CRiskData::GetInstance()->SetExchange2Instrument(mapExchange2Instrument);
                              CRiskData::GetInstance()->SetInstrument2PriceTick(mapInst2Tick);
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      }


                  case Cmd_RM_SubscribeOrder_Rsp://报单返回
                  case Cmd_RM_Order_Push:        //报单推送
                    {
                        bShowMsg = false;
                        if ( head.userdata1 == CF_ERROR_SUCCESS )
                        {
                            int nCount = head.len / sizeof(PlatformStru_OrderInfo);
                            for ( int i = 0; i < nCount; i++ )
                            {
                                PlatformStru_OrderInfo* pOrderInfo = (PlatformStru_OrderInfo*)(p+sizeof(Stru_UniPkgHead) + i*sizeof(PlatformStru_OrderInfo));
								//int nStatus = pOrderInfo->ExStatus;
                                SetDataTable(pOrderInfo);
                            }
                        }
                        break;
                    }
                  case Cmd_RM_SubscribePosition_Rsp://持仓返回
                      {
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {  
                              int nCount = head.len / sizeof(PlatformStru_Position);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  PlatformStru_Position* pPosition = (PlatformStru_Position*)(p+sizeof(Stru_UniPkgHead) + i*sizeof(PlatformStru_Position));   
                                  SetDataTable(pPosition);                
                              } 
                          }
                          break;
                      }
                  case Cmd_RM_Position_Push:        //持仓推送
                      {
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {   
                              g_mapPositionPush.clear();
                              
                              int nCount = head.len / sizeof(PlatformStru_Position);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  PlatformStru_Position* pPosition = (PlatformStru_Position*)(p+sizeof(Stru_UniPkgHead) + i*sizeof(PlatformStru_Position));
                                  PositionKey key(pPosition->Account,pPosition->InstrumentID,pPosition->PosiDirection,pPosition->HedgeFlag);
                                  g_mapPositionPush.insert(make_pair(key,*pPosition));
                              }

                              if(nCount>0)
                                  RowsClearAt(g_mapPositionPush);
                              else if(nCount==0)
                                  RowsClearAt(head.userdata2);

                              map<PositionKey,PlatformStru_Position>::iterator  it = g_mapPositionPush.begin();
                              for ( ; it != g_mapPositionPush.end(); it++ )
                              {                                 
                                  SetDataTable(&(it->second));
                              }
                          }
                          break;
                      }
                  case Cmd_RM_OrderAction_Rsp://撤单
                      {
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                             strError = "撤单操作成功";
                          }
                          else
                          {
                              strError = "撤单操作失败,失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }
                          break;
                      }
                  case Cmd_RM_ForceCloseOrderInsert_Rsp://手动强平返回
                      {
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          { 
                              strError = "手动强平操作成功";
                          }
                          else
                          {
                              strError = "手动强平操作失败,失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }
                          break;
                      }
                  case Cmd_RM_SubscribeFund_Rsp://资金返回
                  case Cmd_RM_Fund_Push://资金推送
                      {
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              int nCount = head.len / sizeof(PlatformStru_TradingAccountInfo);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  PlatformStru_TradingAccountInfo* pTradingAccountInfo = (PlatformStru_TradingAccountInfo*)(p+sizeof(Stru_UniPkgHead) + i*sizeof(PlatformStru_TradingAccountInfo));
                                  SetDataTable(pTradingAccountInfo);
                              }
                          }
                          break;
                      }                      
                  case Cmd_RM_SubscribeTrade_Rsp://成交返回1
                  case Cmd_RM_Trade_Push://成交推送
                      {
                          bShowMsg = false;
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              int nCount = head.len / sizeof(PlatformStru_TradeInfo);
                              for ( int i = 0; i < nCount; i++ )
                              {
                                  PlatformStru_TradeInfo* pTradeInfo = (PlatformStru_TradeInfo*)(p+sizeof(Stru_UniPkgHead) + i*sizeof(PlatformStru_TradeInfo));
                                  SetDataTable(pTradeInfo);
                              }
                          }
                          break;
                      }
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
                                      strError = "1级错误，影响系统运行的错误"+Tools::string2String(pErrorInfo->msError);
                                      break;
                                  case SecondLevelError:
                                      strError = "2级错误，不影响系统运行的错误"+Tools::string2String(pErrorInfo->msError);
                                      break;
                                  case ThirdLevelError:
                                      strError = "3级错误，不影响系统运行"+Tools::string2String(pErrorInfo->msError);
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
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }
                          break;
                      }
                  }   
                  
                  if(bShowMsg)
                      _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
                      CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
                      Tools::String2string(strError).c_str());
              }

              Form::WndProc ( m );
          }
private: System::Void FormMain_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
             int nHwnd = this->Handle.ToInt32();
             CTcpLayer::UnsubscribeAllPkg(nHwnd);
             CTcpDataConnect::UnsubscribeAllPkg(nHwnd);

             //保存配置文件 自选合约
             map<string,vector<string>> mapExchange2Inst = CRiskData::GetInstance()->GetConfigFile(XML_TYPE_INST);
             CConfigManager::SaveSelectedInst(mapExchange2Inst);

//              String^ szTreeText = DataGridView_DepthMarket;
//              String^ szColumnText = String::Empty;
//              String^ szColumnIndex = String::Empty;
//              vector<string> vecDataGridView2Column;               
//              for (int i = 0; i < dataGridView_QuotOrderTable->ColumnCount; i++)
//              {
//                  if(dataGridView_QuotOrderTable->Columns[i]->Visible)
//                  {
//                      szColumnText = dataGridView_QuotOrderTable->Columns[i]-> Name->ToString();
//                      szColumnIndex = GetTagFromText(szColumnText).ToString();
//                      vecDataGridView2Column.push_back(Tools::String2string(szColumnIndex)); 
//                  }
//              }
// 
//              CRiskData::GetInstance()->UpdaeConfigFile(Tools::String2string(szTreeText),vecDataGridView2Column,XML_TYPE_COLUMN);
// 
//              map<string,vector<string>> mapDataGridView2Column = CRiskData::GetInstance()->GetConfigFile(XML_TYPE_COLUMN);
//              CConfigManager::SaveSelectedColumn(mapDataGridView2Column);

             //保存配置文件 合约
//              map<string,sInstrumentInfo> mapInstrumentInfo =  CRiskData::GetInstance()->GetInstrumentInfo();
//              char buff[512];
//              vector<string> vecValue;
//              map<string,vector<string>> mapInstrumentInfo2;
//              
//              map<string,sInstrumentInfo>::iterator it = mapInstrumentInfo.begin();
//              for(;it!=mapInstrumentInfo.end();it++)
//              {
//                  string szInstrumentID = it->first;
// 
//                  vecValue.clear();
//                  //vecValue.push_back((string)(it->second.Exchange));
// 
//                  memset(buff,0,512);
//                  sprintf(buff,"%d",it->second.Volume);
//                  vecValue.push_back((string)buff);
// 
//                  memset(buff,0,512);
//                  sprintf(buff,"%f",it->second.fLastPrice);
//                  vecValue.push_back((string)buff);
// 
//                  memset(buff,0,512);
//                  sprintf(buff,"%f",it->second.fUpperLimitPrice);
//                  vecValue.push_back((string)buff);
// 
//                  memset(buff,0,512);
//                  sprintf(buff,"%f",it->second.fLowerLimitPrice);
//                  vecValue.push_back((string)buff);
// 
//                  mapInstrumentInfo2[szInstrumentID] = vecValue;
//              }
// 
//              CConfigManager::SaveInstrumentInfo(mapInstrumentInfo2); 

             CRiskData::DestroyInstance();
         }
private: System::Void FormMain_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			 formUserMsg->ResetExit(true);
			 formUserMsg->Close();
			 e->Cancel = false;

             //退订  
             UserInfo userInfo;
             vector<int> vecUserID;

             for each (String^ szAccount in m_vecTraderAccountFromTree)
                 if(CRiskData::GetInstance()->GetUserInfoByAccount(Tools::String2string(szAccount),userInfo))
                     vecUserID.push_back(userInfo.nUserID);

             if(!vecUserID.empty())
                 UnSubscribe(vecUserID);
		 }
private: System::Void ToolStripMenuItem_ModifyPSW_Click(System::Object^  sender, System::EventArgs^  e){
             FormModifyPsw^ dlg = gcnew FormModifyPsw;
             if(::DialogResult::OK == dlg->ShowDialog())
             {
                 UserPassword sMP;
                 memset(&sMP,0,sizeof(UserPassword));

                 dlg->GetModifyPSW(sMP);
                 CTcpLayer::SendData(Cmd_ModifyUserPassword_Req, (void*)&sMP,sizeof(UserPassword), 0, m_pLogin->mnUserID);
             }
         }
private: System::Void ToolStripMenuItem_LockWindow_Click(System::Object^  sender, System::EventArgs^  e){
             Hide();
             m_FormLockWindowDlg->Show();
         }
private: System::Void ToolStripMenuItem_Exit_Click(System::Object^  sender, System::EventArgs^  e){
             Close();
         }
private: System::Void ToolStripMenuItem_TableColumnSetup_Click(System::Object^  sender, System::EventArgs^  e) {
             FormTableColumnSetup^ dlg = gcnew FormTableColumnSetup();
             if(System::Windows::Forms::DialogResult::OK ==dlg->ShowDialog())
             {
                //设置后隐藏和显示列
                SetDataGridviewColumns();
             }
         }
private: System::Void ToolStripMenuItem_AutoInstSetup_Click(System::Object^  sender, System::EventArgs^  e) {
             FormAutoSelectInstSetup^ dlg = gcnew FormAutoSelectInstSetup();
             if(System::Windows::Forms::DialogResult::OK == dlg->ShowDialog())
             {
                 string szExchange,szInst;
                 std::map<string,std::vector<string>> mapExchange2InstSelected;
                 dlg->GetSelectInst(mapExchange2InstSelected);

                 vector<string> vecInstSelected;
                 for (std::map<std::string,std::vector<std::string>>::iterator it=mapExchange2InstSelected.begin();it!=mapExchange2InstSelected.end();it++)
                 {
                     for (std::vector<std::string>::iterator it_inst=it->second.begin();it_inst!=it->second.end();it_inst++)
                         vecInstSelected.push_back(*it_inst);
                 }

                 if(!vecInstSelected.empty())
                 {                     
                     vector<string> vecInstSub;
                     vector<string> vecInstUnSub;
                     
                     CRiskData::GetInstance()->AnalyseSelectInst(vecInstSelected,vecInstSub,vecInstUnSub);
                     for(vector<string>::iterator it_unsub=vecInstUnSub.begin();it_unsub!=vecInstUnSub.end();it_unsub++)
                     {
                         szInst = *it_unsub;
                         szExchange = CRiskData::GetInstance()->GetExchangeByInstrument(szInst);
                         //只有当m_mapExchange2Inst2没有该合约时才能真正的退订
                         if(!CRiskData::GetInstance()->bSubScribeInst2(szInst))
                             CTcpDataConnect::SendData(Cmd_UnSubscribeQuotEvent_Req, (char*)szInst.c_str(), szInst.length()+1, 0, m_pLogin->mnUserID);
                         CRiskData::GetInstance()->UnSubScribeInst(szExchange,szInst);

                         //报价表中要删除
                         for (int i=0;i<dataGridView_QuotOrderTable->Rows->Count;i++)
                         {
                             String^ szInstID = dataGridView_QuotOrderTable->Rows[i]->Cells["合约"]->Value->ToString()->Trim();
                             String^ szDest = Tools::string2String(szInst.c_str());
                             if(szInstID->CompareTo(szDest) == 0)
                             {
                                 dataGridView_QuotOrderTable->Rows->RemoveAt(i); 
                                 break;
                             }
                         }
                     }
                     for(vector<string>::iterator it_sub=vecInstSub.begin();it_sub!=vecInstSub.end();it_sub++)
                     {
                         szInst = *it_sub;
                         szExchange = CRiskData::GetInstance()->GetExchangeByInstrument(szInst);

                         CTcpDataConnect::SendData(Cmd_SubscribeQuotEvent_Req, (char*)szInst.c_str(), szInst.length()+1, 0, m_pLogin->mnUserID);
                         CRiskData::GetInstance()->SubScribeInst(szExchange,szInst);
                     }
                 }
                 else
                 {
                     map<string,vector<string>> mapAutoSelectedInst = CRiskData::GetInstance()->GetConfigFile(XML_TYPE_INST);
                     for (map<string,vector<string>>::iterator it = mapAutoSelectedInst.begin();it!=mapAutoSelectedInst.end();++it)
                     {
                         vector<string> tempVec = it->second;
                         for(vector<string>::iterator it_inst=tempVec.begin();it_inst!=tempVec.end();it_inst++)
                         {
                             szInst = *it_inst;
                             szExchange = CRiskData::GetInstance()->GetExchangeByInstrument(szInst);
                             //只有当m_mapExchange2Inst2没有该合约时才能真正的退订
                             if(!CRiskData::GetInstance()->bSubScribeInst2(szInst))
                                 CTcpDataConnect::SendData(Cmd_UnSubscribeQuotEvent_Req, (char*)szInst.c_str(), szInst.length()+1, 0, m_pLogin->mnUserID);
                             CRiskData::GetInstance()->UnSubScribeInst(szExchange,szInst);

                             //报价表中要删除
                             for (int i=0;i<dataGridView_QuotOrderTable->Rows->Count;i++)
                             {
                                 String^ szInstID = dataGridView_QuotOrderTable->Rows[i]->Cells["合约"]->Value->ToString()->Trim();
                                 String^ szDest = Tools::string2String(szInst.c_str());
                                 if(szInstID->CompareTo(szDest) == 0)
                                 {
                                     dataGridView_QuotOrderTable->Rows->RemoveAt(i); 
                                     break;
                                 }
                             }
                         }
                     }

//                      map<string,vector<string>>::iterator it4 = mapAutoSelectedInst.find(Tools::String2string(szExchange));
//                      if(it4!=mapAutoSelectedInst.end())
//                      {
//                          for(vector<string>::iterator it5=it4->second.begin();it5!=it4->second.end();it5++)
//                          {
//                              //只有当m_mapExchange2Inst2没有该合约时才能真正的退订
//                              if(!CRiskData::GetInstance()->bSubScribeInst2((char*)(*it5).c_str()))
//                                  CTcpDataConnect::SendData(Cmd_UnSubscribeQuotEvent_Req, (char*)(*it5).c_str(), (*it5).length()+1, 0, m_pLogin->mnUserID);
//                              CRiskData::GetInstance()->UnSubScribeInst(Tools::String2string(szExchange),(string)((*it5).c_str()));
// 
//                              //报价表中要删除
//                              for (int i=0;i<dataGridView_QuotOrderTable->Rows->Count;i++)
//                              {
//                                  String^ szInstID = dataGridView_QuotOrderTable->Rows[i]->Cells["合约"]->Value->ToString()->Trim();
//                                  String^ szDest = Tools::string2String((char*)(*it5).c_str());
//                                  if(szInstID->CompareTo(szDest) == 0)
//                                  {
//                                      dataGridView_QuotOrderTable->Rows->RemoveAt(i); 
//                                      break;
//                                  }
//                              }
//                          }
//                      }
                 }
             }
         }
private: System::Void ToolStripMenuItem_MsgQuery_Click(System::Object^  sender, System::EventArgs^  e) {
			CFormUserMsgQuery^ dlg = gcnew CFormUserMsgQuery();
            dlg->ShowDialog();
        }
private: System::Void ToolStripMenuItem_MsgSend_Click(System::Object^  sender, System::EventArgs^  e) {
			 sLoginRsp logInfo;

			 memset(&logInfo, 0, sizeof(logInfo));
			 CRiskData::GetInstance()->GetLogonInfo(logInfo);
			 CFormSendMsgMultiUsers^ dlg = gcnew CFormSendMsgMultiUsers();
			 dlg->ResetUser(logInfo.mnUserID);
             dlg->ShowDialog();
		 }
private: System::Void ToolStripMenuItem_QueryHistoryRiskEvent_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 QueryRiskEvt^ dlg =gcnew QueryRiskEvt;
			 /*if(::DialogResult::OK == dlg->ShowDialog())
			 {
				 ShowDialog();                 
			 }*/
			 dlg->ShowDialog();
         }
private: System::Void ToolStripMenuItem_QueryHistoryMsgRecord_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 /*QueryMessage^ dlg =gcnew QueryMessage;
			 if(::DialogResult::OK == dlg->ShowDialog())
			 {
				 ShowDialog();                 
			 }*/
         }
private: System::Void ToolStripMenuItem_HistoryFundValue_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 QueryFundNetValue ^dlg = gcnew QueryFundNetValue;
			 if(::DialogResult::OK == dlg->ShowDialog())
			 {
				 ShowDialog();                 
			 }
         }
private: System::Void ToolStripMenuItem_QueryOrderVerify_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 QueryVerifyOrder ^dlg = gcnew QueryVerifyOrder;
			 if(::DialogResult::OK == dlg->ShowDialog())
			 {
				 ShowDialog();                 
			 }

         }
private: System::Void ToolStripMenuItem_QueryForceClose_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 QueryForceClose ^ dlg = gcnew QueryForceClose;
			 if(::DialogResult::OK == dlg->ShowDialog())
			 {
				 ShowDialog();                 
			 }
         }
private: System::Void ToolStripMenuItem_QueryTraderBaseInfo_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 QueryInvestorInfo^ dlg = gcnew QueryInvestorInfo;
			 if(::DialogResult::OK == dlg->ShowDialog())
			 {
				 ShowDialog();                 
			 }
         }
private: System::Void ToolStripMenuItem_HelpDocument_Click(System::Object^  sender, System::EventArgs^  e) {
         }
private: System::Void ToolStripMenuItem_VersionIllustration_Click(System::Object^  sender, System::EventArgs^  e) {
         }
private: System::Void button_PlanAdd_Click(System::Object^  sender, System::EventArgs^  e) {
             FormRiskPlanSetup_Add^ dlg = gcnew FormRiskPlanSetup_Add;
             dlg->SetOperType(Oper_Add);
             m_FormRiskPlanSetup_Add = dlg;

             if(dataGridView_OrgRiskSetup->SelectedRows != nullptr && dataGridView_OrgRiskSetup->Rows->Count>0)
             {
                 int nRowIndex = dataGridView_OrgRiskSetup->CurrentCell->RowIndex;  
                 dlg->SetOrgText(dataGridView_OrgRiskSetup->Rows[nRowIndex]->Cells[0]->Value->ToString());             
             }

             dlg->ShowDialog();
         }
private: System::Void button_PlanModify_Click(System::Object^  sender, System::EventArgs^  e) {
             if (dataGridView_OrgRiskSetup->SelectedRows != nullptr && dataGridView_OrgRiskSetup->Rows->Count>0)
             {
                 int nRowIndex = dataGridView_OrgRiskSetup->CurrentCell->RowIndex;  

                 FormRiskPlanSetup_Add^ dlg = gcnew FormRiskPlanSetup_Add;
                 dlg->SetOperType(Oper_Modify);
                 m_FormRiskPlanSetup_Add = dlg;
                 dlg->SetOrgText(dataGridView_OrgRiskSetup->Rows[nRowIndex]->Cells[0]->Value->ToString());
                 dlg->SetRiskPlanText(dataGridView_OrgRiskSetup->Rows[nRowIndex]->Cells[1]->Value->ToString());
                 dlg->ShowDialog();                 
             }
             else
                 MessageBox::Show("请选择内容!");
         }
private: System::Void button_PlanDel_Click(System::Object^  sender, System::EventArgs^  e) {
             if (dataGridView_OrgRiskSetup->SelectedRows != nullptr && dataGridView_OrgRiskSetup->Rows->Count>0)
             {
                 FormRiskPlanSetup_Del^ dlg = gcnew FormRiskPlanSetup_Del;
                 if(::DialogResult::OK == dlg->ShowDialog())
                 {
                     int nOrgID;
                     RiskIndicatorType nRiskIndicatorID;

                     int nRowIndex = dataGridView_OrgRiskSetup->CurrentCell->RowIndex;  
                     string szOrg = Tools::String2string(dataGridView_OrgRiskSetup->Rows[nRowIndex]->Cells[0]->Value->ToString());
                     string szIndicator = Tools::String2string(dataGridView_OrgRiskSetup->Rows[nRowIndex]->Cells[1]->Value->ToString());

                     if(!CRiskData::GetInstance()->GetOrganizationIDByName(szOrg,nOrgID))
                         return;
                     if(!CRiskData::GetInstance()->GetIndicatorIDByName(szIndicator,nRiskIndicatorID))
                         return;

                     RiskChange key;
                     key.nOrgID = nOrgID;
                     key.nRiskIndicatorID = nRiskIndicatorID;

                     g_riskPlanOrg = szOrg;
                     g_riskPlanKey.first = nOrgID;
                     g_riskPlanKey.second = nRiskIndicatorID;

                     CTcpLayer::SendData(Cmd_RM_DeleteRiskPlan_Req, &key, sizeof(RiskChange), 0, m_pLogin->mnUserID); 
                 }
             }
             else
                 MessageBox::Show("请选择内容!");
         }
private: System::Void button_SelectAll_Click(System::Object^  sender, System::EventArgs^  e) {
         }
private: System::Void button_UnSelectAll_Click(System::Object^  sender, System::EventArgs^  e) {
         }
private: System::Void button_OrderForbid_Click(System::Object^  sender, System::EventArgs^  e) {
         }

private: System::Void button_PassVerify_Click(System::Object^  sender, System::EventArgs^  e) {
             if (dataGridView_NeedVerify->RowCount == 0) {
                 MessageBox::Show("请至少选择一条数据！", "提示");
                 return;
             }
             SendVerifyOrder(true);
         }
private: System::Void button_ForbidOrder_Click(System::Object^  sender, System::EventArgs^  e) {
             if (dataGridView_NeedVerify->RowCount == 0) {
                 MessageBox::Show("请至少选择一条数据！", "提示");
                 return;
             }
             SendVerifyOrder(false);
         }
private: System::Void dataGridView_OrgRiskSetup_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
             if (IsANonHeaderButtonCell(e))
             {
                 int nOrgID;
                 RiskIndicatorType nRiskIndicatorID;

                 if(!CRiskData::GetInstance()->GetOrganizationIDByName(Tools::String2string(dataGridView_OrgRiskSetup->Rows[e->RowIndex]->Cells[0]->Value->ToString()),nOrgID))
                     return;
                 if(!CRiskData::GetInstance()->GetIndicatorIDByName(Tools::String2string(dataGridView_OrgRiskSetup->Rows[e->RowIndex]->Cells[1]->Value->ToString()),nRiskIndicatorID))
                     return;

                String^ szText = dataGridView_OrgRiskSetup->Rows[e->RowIndex]->Cells[2]->Value->ToString();


                RiskChange key;
                key.nOrgID = nOrgID;
                key.nRiskIndicatorID = nRiskIndicatorID;
                key.bUse = (0==szText->CompareTo("启用"))?true:false;                

                CTcpLayer::SendData(Cmd_RM_UseRiskPlanAdd_Req, &key, sizeof(RiskChange), 0, m_pLogin->mnUserID); 
             }
         }
private: System::Void dataGridView_AllOrder_CellMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if (e->Button == System::Windows::Forms::MouseButtons::Right)
             {
                 if (e->RowIndex >= 0 && e->RowIndex < dataGridView_AllOrder->RowCount)
                 {                         
                     //若行已是选中状态就不再进行设置
                     if (dataGridView_AllOrder->Rows[e->RowIndex]->Selected == false)
                     {
                         dataGridView_AllOrder->ClearSelection();
                         dataGridView_AllOrder->Rows[e->RowIndex]->Selected = true;
                     }
                     //只选中一行时设置活动单元格
                     if (dataGridView_AllOrder->SelectedRows->Count == 1)
                     {
                         dataGridView_AllOrder->CurrentCell = dataGridView_AllOrder->Rows[e->RowIndex]->Cells[e->ColumnIndex];
                     }

                     m_activeDataGridView = dataGridView_AllOrder;
                     m_activeDataTable = m_DataTableAllOrder;

                     //弹出操作菜单
                     contextMenuStrip_Sub->Items->Clear();

                     ToolStripItem^ item_output = gcnew ToolStripMenuItem("导出列表");
                     item_output->Click += gcnew System::EventHandler(this, &FormMain::item_output_Click);
                     contextMenuStrip_Sub->Items->Add(item_output);

                     ToolStripItem^ item_sendmsg = gcnew ToolStripMenuItem("撤单");
                     item_sendmsg->Click += gcnew System::EventHandler(this, &FormMain::item_closeorder_Click);
                     contextMenuStrip_Sub->Items->Add(item_sendmsg);

                     ToolStripItem^ item_addto = gcnew ToolStripMenuItem("添加到行情列表");
                     item_addto->Click += gcnew System::EventHandler(this, &FormMain::item_addto_Click);
                     contextMenuStrip_Sub->Items->Add(item_addto);

                     contextMenuStrip_Sub->Show(MousePosition.X, MousePosition.Y);
                 }
             }
         }
private: System::Void dataGridView_AllOrder_ColumnHeaderMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if (e->Button == System::Windows::Forms::MouseButtons::Right)   
             {   
                 m_activeDataGridView = dataGridView_AllOrder;
                 m_activeDataTable = m_DataTableAllOrder;

                 //弹出操作菜单
                 contextMenuStrip_Sub->Items->Clear(); 

                 ToolStripItem^ item_hide = gcnew ToolStripMenuItem("隐藏此列");
                 item_hide->Click += gcnew System::EventHandler(this, &FormMain::item_hide_Click);
                 contextMenuStrip_Sub->Items->Add(item_hide);

                 //已经隐藏的列
                 map<string, map<string, int>>::iterator it = g_mapColumnText2Index.find(DataGridView_AllOrder);
                 if(it!=g_mapColumnText2Index.end())
                 {
                     map<string, int> mapColumnText2Index = it->second;

                     ToolStripItem^ item_show = gcnew ToolStripMenuItem("显示列");
                     contextMenuStrip_Sub->Items->Add(item_show);

                     map<string,int>::iterator it = mapColumnText2Index.begin();
                     for (;it!=mapColumnText2Index.end();it++)
                     {
                         ToolStripItem^ item_hide_lists = gcnew ToolStripMenuItem(Tools::string2String((it->first).c_str()));
                         item_hide_lists->Tag = Tools::string2String((it->first).c_str());
                         item_hide_lists->Click += gcnew System::EventHandler(this, &FormMain::item_hide_lists_Click);
                         ((ToolStripDropDownItem^)(contextMenuStrip_Sub->Items[1]))->DropDownItems->Add(item_hide_lists);
                     }
                 }

                 ToolStripItem^ item_auto = gcnew ToolStripMenuItem("自动调整列宽");
                 item_auto->Click += gcnew System::EventHandler(this, &FormMain::item_auto_Click);
                 contextMenuStrip_Sub->Items->Add(item_auto);

                 //                  ToolStripItem^ item_setup = gcnew ToolStripMenuItem("表格列设置");
                 //                  item_setup->Click += gcnew System::EventHandler(this, &FormMain::item_setup_Click);
                 //                  contextMenuStrip_Sub->Items->Add(item_setup);

                 ToolStripItem^ item_output = gcnew ToolStripMenuItem("导出列表");
                 item_output->Click += gcnew System::EventHandler(this, &FormMain::item_output_Click);
                 contextMenuStrip_Sub->Items->Add(item_output);

                 m_nCurrColumnHeaderIndex_MouseRight = e->ColumnIndex;
                 m_CurrDataGridViewText = DataGridView_AllOrder;
                 m_CurrColumnHeaderText = this->dataGridView_AllOrder->Columns[e->ColumnIndex]->Name;  

                 contextMenuStrip_Sub->Show(MousePosition.X, MousePosition.Y);
             }   
         }
private: System::Void dataGridView_TradeRecord_CellMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if (e->Button == System::Windows::Forms::MouseButtons::Right)
             {
                 if (e->RowIndex >= 0 && e->RowIndex < dataGridView_TradeRecord->RowCount)
                 {                         
                     //若行已是选中状态就不再进行设置
                     if (dataGridView_TradeRecord->Rows[e->RowIndex]->Selected == false)
                     {
                         dataGridView_TradeRecord->ClearSelection();
                         dataGridView_TradeRecord->Rows[e->RowIndex]->Selected = true;
                     }
                     //只选中一行时设置活动单元格
                     if (dataGridView_TradeRecord->SelectedRows->Count == 1)
                     {
                         dataGridView_TradeRecord->CurrentCell = dataGridView_TradeRecord->Rows[e->RowIndex]->Cells[e->ColumnIndex];
                     }

                     m_activeDataGridView = dataGridView_TradeRecord;
                     m_activeDataTable = m_DataTableTrade;

                     //弹出操作菜单
                     contextMenuStrip_Sub->Items->Clear();

                     ToolStripItem^ item_output = gcnew ToolStripMenuItem("导出列表");
                     item_output->Click += gcnew System::EventHandler(this, &FormMain::item_output_Click);
                     contextMenuStrip_Sub->Items->Add(item_output);

                     ToolStripItem^ item_addto = gcnew ToolStripMenuItem("添加到行情列表");
                     item_addto->Click += gcnew System::EventHandler(this, &FormMain::item_addto_Click);
                     contextMenuStrip_Sub->Items->Add(item_addto);

                     contextMenuStrip_Sub->Show(MousePosition.X, MousePosition.Y);
                 }
             }
         }
private: System::Void dataGridView_TradeRecord_ColumnHeaderMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if (e->Button == System::Windows::Forms::MouseButtons::Right)   
             {   
                 m_activeDataGridView = dataGridView_TradeRecord;
                 m_activeDataTable = m_DataTableTrade;

                 //弹出操作菜单
                 contextMenuStrip_Sub->Items->Clear(); 

                 ToolStripItem^ item_hide = gcnew ToolStripMenuItem("隐藏此列");
                 item_hide->Click += gcnew System::EventHandler(this, &FormMain::item_hide_Click);
                 contextMenuStrip_Sub->Items->Add(item_hide);

                 //已经隐藏的列
                 map<string, map<string, int>>::iterator it = g_mapColumnText2Index.find(DataGridView_Trade);
                 if(it!=g_mapColumnText2Index.end())
                 {
                     map<string, int> mapColumnText2Index = it->second;

                     ToolStripItem^ item_show = gcnew ToolStripMenuItem("显示列");
                     contextMenuStrip_Sub->Items->Add(item_show);

                     map<string,int>::iterator it = mapColumnText2Index.begin();
                     for (;it!=mapColumnText2Index.end();it++)
                     {
                         ToolStripItem^ item_hide_lists = gcnew ToolStripMenuItem(Tools::string2String((it->first).c_str()));
                         item_hide_lists->Tag = Tools::string2String((it->first).c_str());
                         item_hide_lists->Click += gcnew System::EventHandler(this, &FormMain::item_hide_lists_Click);
                         ((ToolStripDropDownItem^)(contextMenuStrip_Sub->Items[1]))->DropDownItems->Add(item_hide_lists);
                     }
                 } 

                 ToolStripItem^ item_auto = gcnew ToolStripMenuItem("自动调整列宽");
                 item_auto->Click += gcnew System::EventHandler(this, &FormMain::item_auto_Click);
                 contextMenuStrip_Sub->Items->Add(item_auto);

                 //                  ToolStripItem^ item_setup = gcnew ToolStripMenuItem("表格列设置");
                 //                  item_setup->Click += gcnew System::EventHandler(this, &FormMain::item_setup_Click);
                 //                  contextMenuStrip_Sub->Items->Add(item_setup);

                 ToolStripItem^ item_output = gcnew ToolStripMenuItem("导出列表");
                 item_output->Click += gcnew System::EventHandler(this, &FormMain::item_output_Click);
                 contextMenuStrip_Sub->Items->Add(item_output);

                 m_nCurrColumnHeaderIndex_MouseRight = e->ColumnIndex;
                 m_CurrDataGridViewText = DataGridView_Trade;
                 m_CurrColumnHeaderText = this->dataGridView_TradeRecord->Columns[e->ColumnIndex]->Name;  

                 contextMenuStrip_Sub->Show(MousePosition.X, MousePosition.Y);
             }
         }
private: System::Void dataGridView_NeedVerify_CellMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if (e->Button == System::Windows::Forms::MouseButtons::Right)
             {
                 if (e->RowIndex >= 0 && e->RowIndex < dataGridView_NeedVerify->RowCount)
                 {                         
                     //若行已是选中状态就不再进行设置
                     if (dataGridView_NeedVerify->Rows[e->RowIndex]->Selected == false)
                     {
                         dataGridView_NeedVerify->ClearSelection();
                         dataGridView_NeedVerify->Rows[e->RowIndex]->Selected = true;
                     }
                     //只选中一行时设置活动单元格
                     if (dataGridView_NeedVerify->SelectedRows->Count == 1)
                     {
                         dataGridView_NeedVerify->CurrentCell = dataGridView_NeedVerify->Rows[e->RowIndex]->Cells[e->ColumnIndex];
                     }

                     m_activeDataGridView = dataGridView_NeedVerify;
                     m_activeDataTable = m_DataTableAllOrder;

                     //弹出操作菜单
                     contextMenuStrip_Sub->Items->Clear();

                     ToolStripItem^ item_output = gcnew ToolStripMenuItem("导出列表");
                     item_output->Click += gcnew System::EventHandler(this, &FormMain::item_output_Click);
                     contextMenuStrip_Sub->Items->Add(item_output);

                     ToolStripItem^ item_addto = gcnew ToolStripMenuItem("添加到行情列表");
                     item_addto->Click += gcnew System::EventHandler(this, &FormMain::item_addto_Click);
                     contextMenuStrip_Sub->Items->Add(item_addto);

                     contextMenuStrip_Sub->Show(MousePosition.X, MousePosition.Y);
                 }
             }
         }
private: System::Void dataGridView_NeedVerify_ColumnHeaderMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if (e->Button == System::Windows::Forms::MouseButtons::Right)   
             {   
                 m_activeDataGridView = dataGridView_NeedVerify;
                 m_activeDataTable = m_DataTableAllOrder;

                 //弹出操作菜单
                 contextMenuStrip_Sub->Items->Clear(); 

                 ToolStripItem^ item_hide = gcnew ToolStripMenuItem("隐藏此列");
                 item_hide->Click += gcnew System::EventHandler(this, &FormMain::item_hide_Click);
                 contextMenuStrip_Sub->Items->Add(item_hide);

                 //已经隐藏的列
                 map<string, map<string, int>>::iterator it = g_mapColumnText2Index.find(DataGridView_NeedVerify);
                 if(it!=g_mapColumnText2Index.end())
                 {
                     map<string, int> mapColumnText2Index = it->second;

                     ToolStripItem^ item_show = gcnew ToolStripMenuItem("显示列");
                     contextMenuStrip_Sub->Items->Add(item_show);

                     map<string,int>::iterator it = mapColumnText2Index.begin();
                     for (;it!=mapColumnText2Index.end();it++)
                     {
                         ToolStripItem^ item_hide_lists = gcnew ToolStripMenuItem(Tools::string2String((it->first).c_str()));
                         item_hide_lists->Tag = Tools::string2String((it->first).c_str());
                         item_hide_lists->Click += gcnew System::EventHandler(this, &FormMain::item_hide_lists_Click);
                         ((ToolStripDropDownItem^)(contextMenuStrip_Sub->Items[1]))->DropDownItems->Add(item_hide_lists);
                     }
                 } 

                 ToolStripItem^ item_auto = gcnew ToolStripMenuItem("自动调整列宽");
                 item_auto->Click += gcnew System::EventHandler(this, &FormMain::item_auto_Click);
                 contextMenuStrip_Sub->Items->Add(item_auto);

                 //                  ToolStripItem^ item_setup = gcnew ToolStripMenuItem("表格列设置");
                 //                  item_setup->Click += gcnew System::EventHandler(this, &FormMain::item_setup_Click);
                 //                  contextMenuStrip_Sub->Items->Add(item_setup);

                 ToolStripItem^ item_output = gcnew ToolStripMenuItem("导出列表");
                 item_output->Click += gcnew System::EventHandler(this, &FormMain::item_output_Click);
                 contextMenuStrip_Sub->Items->Add(item_output);


                 m_nCurrColumnHeaderIndex_MouseRight = e->ColumnIndex;
                 m_CurrDataGridViewText = DataGridView_NeedVerify;
                 m_CurrColumnHeaderText = this->dataGridView_NeedVerify->Columns[e->ColumnIndex]->Name;  

                 contextMenuStrip_Sub->Show(MousePosition.X, MousePosition.Y);
             }
         }
private: System::Void dataGridView_UnTradeRecord_CellMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if (e->Button == System::Windows::Forms::MouseButtons::Right)
             {
                 if (e->RowIndex >= 0 && e->RowIndex < dataGridView_UnTradeRecord->RowCount)
                 {                         
                     //若行已是选中状态就不再进行设置
                     if (dataGridView_UnTradeRecord->Rows[e->RowIndex]->Selected == false)
                     {
                         dataGridView_UnTradeRecord->ClearSelection();
                         dataGridView_UnTradeRecord->Rows[e->RowIndex]->Selected = true;
                     }
                     //只选中一行时设置活动单元格
                     if (dataGridView_UnTradeRecord->SelectedRows->Count == 1)
                     {
                         dataGridView_UnTradeRecord->CurrentCell = dataGridView_UnTradeRecord->Rows[e->RowIndex]->Cells[e->ColumnIndex];
                     }

                     m_activeDataGridView = dataGridView_UnTradeRecord;
                     m_activeDataTable = m_DataTableAllOrder;

                     //弹出操作菜单
                     contextMenuStrip_Sub->Items->Clear();

                     ToolStripItem^ item_output = gcnew ToolStripMenuItem("导出列表");
                     item_output->Click += gcnew System::EventHandler(this, &FormMain::item_output_Click);
                     contextMenuStrip_Sub->Items->Add(item_output);

                     ToolStripItem^ item_sendmsg = gcnew ToolStripMenuItem("撤单");
                     item_sendmsg->Click += gcnew System::EventHandler(this, &FormMain::item_closeorder_Click);
                     contextMenuStrip_Sub->Items->Add(item_sendmsg);

                     ToolStripItem^ item_addto = gcnew ToolStripMenuItem("添加到行情列表");
                     item_addto->Click += gcnew System::EventHandler(this, &FormMain::item_addto_Click);
                     contextMenuStrip_Sub->Items->Add(item_addto);

                     contextMenuStrip_Sub->Show(MousePosition.X, MousePosition.Y);
                 }
             }
         }
private: System::Void dataGridView_UnTradeRecord_ColumnHeaderMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if (e->Button == System::Windows::Forms::MouseButtons::Right)   
             {   
                 m_activeDataGridView = dataGridView_UnTradeRecord;
                 m_activeDataTable = m_DataTableAllOrder;

                 //弹出操作菜单
                 contextMenuStrip_Sub->Items->Clear(); 

                 ToolStripItem^ item_hide = gcnew ToolStripMenuItem("隐藏此列");
                 item_hide->Click += gcnew System::EventHandler(this, &FormMain::item_hide_Click);
                 contextMenuStrip_Sub->Items->Add(item_hide);

                 //已经隐藏的列
                 map<string, map<string, int>>::iterator it = g_mapColumnText2Index.find(DataGridView_UnTrade);
                 if(it!=g_mapColumnText2Index.end())
                 {
                     map<string, int> mapColumnText2Index = it->second;

                     ToolStripItem^ item_show = gcnew ToolStripMenuItem("显示列");
                     contextMenuStrip_Sub->Items->Add(item_show);

                     map<string,int>::iterator it = mapColumnText2Index.begin();
                     for (;it!=mapColumnText2Index.end();it++)
                     {
                         ToolStripItem^ item_hide_lists = gcnew ToolStripMenuItem(Tools::string2String((it->first).c_str()));
                         item_hide_lists->Tag = Tools::string2String((it->first).c_str());
                         item_hide_lists->Click += gcnew System::EventHandler(this, &FormMain::item_hide_lists_Click);
                         ((ToolStripDropDownItem^)(contextMenuStrip_Sub->Items[1]))->DropDownItems->Add(item_hide_lists);
                     }
                 } 

                 ToolStripItem^ item_auto = gcnew ToolStripMenuItem("自动调整列宽");
                 item_auto->Click += gcnew System::EventHandler(this, &FormMain::item_auto_Click);
                 contextMenuStrip_Sub->Items->Add(item_auto);

                 //                  ToolStripItem^ item_setup = gcnew ToolStripMenuItem("表格列设置");
                 //                  item_setup->Click += gcnew System::EventHandler(this, &FormMain::item_setup_Click);
                 //                  contextMenuStrip_Sub->Items->Add(item_setup);

                 ToolStripItem^ item_output = gcnew ToolStripMenuItem("导出列表");
                 item_output->Click += gcnew System::EventHandler(this, &FormMain::item_output_Click);
                 contextMenuStrip_Sub->Items->Add(item_output);

                 m_nCurrColumnHeaderIndex_MouseRight = e->ColumnIndex;
                 m_CurrDataGridViewText = DataGridView_UnTrade;
                 m_CurrColumnHeaderText = this->dataGridView_UnTradeRecord->Columns[e->ColumnIndex]->Name;  

                 contextMenuStrip_Sub->Show(MousePosition.X, MousePosition.Y);
             }   
         }
private: System::Void dataGridView_AccountFundInfo_CellMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if (e->Button == System::Windows::Forms::MouseButtons::Right)
             {      
                 bool bLimit = false;
                 bool bVerify = false;
                 UserInfo userInfo;
                 if (e->RowIndex >= 0 && e->RowIndex < dataGridView_AccountFundInfo->RowCount)
                 {         
                     m_activeDataGridView = dataGridView_AccountFundInfo;
                     m_activeDataTable = m_DataTableTradingAccount;   

                     //若行已是选中状态就不再进行设置
                     if (dataGridView_AccountFundInfo->Rows[e->RowIndex]->Selected == false)
                     {
                         dataGridView_AccountFundInfo->ClearSelection();
                         dataGridView_AccountFundInfo->Rows[e->RowIndex]->Selected = true;
                     }
                     //只选中一行时设置活动单元格
                     if (dataGridView_AccountFundInfo->SelectedRows->Count == 1)
                     {
                         dataGridView_AccountFundInfo->CurrentCell = dataGridView_AccountFundInfo->Rows[e->RowIndex]->Cells[e->ColumnIndex];
                     }

                     String^ szTraderAccount = dataGridView_AccountFundInfo->Rows[dataGridView_AccountFundInfo->CurrentCell->RowIndex]->Cells[0]->Value->ToString();
                     if(CRiskData::GetInstance()->GetUserInfoByAccount(Tools::String2string(szTraderAccount),userInfo))
                     {
                         bLimit = CRiskData::GetInstance()->IsLimitTrade(userInfo.nUserID);
                         bVerify = CRiskData::GetInstance()->IsManualVerify(userInfo.nUserID);
                     }

                     //弹出操作菜单
                     contextMenuStrip_Sub->Items->Clear();

                     ToolStripItem^ item_output = gcnew ToolStripMenuItem("导出列表");
                     item_output->Click += gcnew System::EventHandler(this, &FormMain::item_output_Click);
                     contextMenuStrip_Sub->Items->Add(item_output);

                     ToolStripItem^ item_sendmsg = gcnew ToolStripMenuItem("发送消息");
                     item_sendmsg->Click += gcnew System::EventHandler(this, &FormMain::item_sendmsg_Click);
                     contextMenuStrip_Sub->Items->Add(item_sendmsg);

                     if(bLimit)
                     {
                         ToolStripItem^ item_limittrade = gcnew ToolStripMenuItem("取消限制");
                         item_limittrade->Click += gcnew System::EventHandler(this, &FormMain::item_limitcancle_Click);
                         contextMenuStrip_Sub->Items->Add(item_limittrade);
                     }
                     else
                     {
                         ToolStripItem^ item_limitcancle = gcnew ToolStripMenuItem("限制交易");
                         item_limitcancle->Click += gcnew System::EventHandler(this, &FormMain::item_limittrade_Click);
                         contextMenuStrip_Sub->Items->Add(item_limitcancle);
                     }

                     if(!bVerify)
                     {
                         ToolStripItem^ item_manualverify = gcnew ToolStripMenuItem("手动审核");
                         item_manualverify->Click += gcnew System::EventHandler(this, &FormMain::item_manualverify_Click);
                         contextMenuStrip_Sub->Items->Add(item_manualverify);
                     }
                     else
                     {
                         ToolStripItem^ item_verifycancle = gcnew ToolStripMenuItem("取消审核");
                         item_verifycancle->Click += gcnew System::EventHandler(this, &FormMain::item_verifycancle_Click);
                         contextMenuStrip_Sub->Items->Add(item_verifycancle); 
                     }
   
                     
                     m_nCurrColumnHeaderIndex_MouseRight = e->ColumnIndex;
                     m_CurrDataGridViewText = DataGridView_TradingAccount;
                     m_CurrColumnHeaderText = this->dataGridView_AccountFundInfo->Columns[e->ColumnIndex]->Name;  

                     contextMenuStrip_Sub->Show(MousePosition.X, MousePosition.Y);
                 }
             }
         }
private: System::Void dataGridView_AccountFundInfo_ColumnHeaderMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if (e->Button == System::Windows::Forms::MouseButtons::Right)   
             {   
                 m_activeDataGridView = dataGridView_AccountFundInfo;
                 m_activeDataTable = m_DataTableTradingAccount;

                 //弹出操作菜单
                 contextMenuStrip_Sub->Items->Clear(); 

                 ToolStripItem^ item_hide = gcnew ToolStripMenuItem("隐藏此列");
                 item_hide->Click += gcnew System::EventHandler(this, &FormMain::item_hide_Click);
                 contextMenuStrip_Sub->Items->Add(item_hide);

                 //已经隐藏的列
                 map<string, map<string, int>>::iterator it = g_mapColumnText2Index.find(DataGridView_TradingAccount);
                 if(it!=g_mapColumnText2Index.end())
                 {
                     map<string, int> mapColumnText2Index = it->second;

                     ToolStripItem^ item_show = gcnew ToolStripMenuItem("显示列");
                     contextMenuStrip_Sub->Items->Add(item_show);

                     map<string,int>::iterator it = mapColumnText2Index.begin();
                     for (;it!=mapColumnText2Index.end();it++)
                     {
                         ToolStripItem^ item_hide_lists = gcnew ToolStripMenuItem(Tools::string2String((it->first).c_str()));
                         item_hide_lists->Tag = Tools::string2String((it->first).c_str());
                         item_hide_lists->Click += gcnew System::EventHandler(this, &FormMain::item_hide_lists_Click);
                         ((ToolStripDropDownItem^)(contextMenuStrip_Sub->Items[1]))->DropDownItems->Add(item_hide_lists);
                     }
                 }

                 ToolStripItem^ item_auto = gcnew ToolStripMenuItem("自动调整列宽");
                 item_auto->Click += gcnew System::EventHandler(this, &FormMain::item_auto_Click);
                 contextMenuStrip_Sub->Items->Add(item_auto);

                 m_nCurrColumnHeaderIndex_MouseRight = e->ColumnIndex;
                 m_CurrDataGridViewText = DataGridView_TradingAccount;
                 m_CurrColumnHeaderText = this->dataGridView_AccountFundInfo->Columns[e->ColumnIndex]->Name;  

                 contextMenuStrip_Sub->Show(MousePosition.X, MousePosition.Y);
             }   

             //排序  
//             dataGridView_AccountFundInfo->Columns["动态权益"]->ValueType = ::System::Decimal::typeid;

//             DataGridViewColumn^ clickedColumn = dataGridView_AccountFundInfo->Columns[e->ColumnIndex];
//             if(clickedColumn->SortMode != DataGridViewColumnSortMode::Automatic)
//                 this->SortRows(clickedColumn,true);


         }
private: System::Void dataGridView_Position_CellMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if (e->Button == System::Windows::Forms::MouseButtons::Right)
             {
                 if (e->RowIndex >= 0 && e->RowIndex < dataGridView_Position->RowCount)
                 {                         
                     //若行已是选中状态就不再进行设置
                     if (dataGridView_Position->Rows[e->RowIndex]->Selected == false)
                     {
                         dataGridView_Position->ClearSelection();
                         dataGridView_Position->Rows[e->RowIndex]->Selected = true;
                     }
                     //只选中一行时设置活动单元格
                     if (dataGridView_Position->SelectedRows->Count == 1)
                     {
                         dataGridView_Position->CurrentCell = dataGridView_Position->Rows[e->RowIndex]->Cells[e->ColumnIndex];
                     }

                     m_activeDataGridView = dataGridView_Position;
                     m_activeDataTable = m_DataTablePosition;

                     //弹出操作菜单
                     contextMenuStrip_Sub->Items->Clear();

                     ToolStripItem^ item_output = gcnew ToolStripMenuItem("导出列表");
                     item_output->Click += gcnew System::EventHandler(this, &FormMain::item_output_Click);
                     contextMenuStrip_Sub->Items->Add(item_output);

//                          ToolStripItem^ item_output_csv = gcnew ToolStripMenuItem("CSV");
//                          item_output_csv->Click += gcnew System::EventHandler(this, &FormMain::item_output_csv_Click);
//                          ((ToolStripDropDownItem^)(contextMenuStrip_Sub->Items[2]))->DropDownItems->Add(item_output_csv);
// 
//                          ToolStripItem^ item_output_txt = gcnew ToolStripMenuItem("TXT");
//                          item_output_txt->Click += gcnew System::EventHandler(this, &FormMain::item_output_txt_Click);
//                          ((ToolStripDropDownItem^)(contextMenuStrip_Sub->Items[2]))->DropDownItems->Add(item_output_txt);

                     ToolStripItem^ item_forceclose = gcnew ToolStripMenuItem("手动强平");
                     item_forceclose->Click += gcnew System::EventHandler(this, &FormMain::item_forceclose_Click);
                     contextMenuStrip_Sub->Items->Add(item_forceclose);

                     ToolStripItem^ item_addto = gcnew ToolStripMenuItem("添加到行情列表");
                     item_addto->Click += gcnew System::EventHandler(this, &FormMain::item_addto_Click);
                     contextMenuStrip_Sub->Items->Add(item_addto);

//                          ToolStripItem^ item_quotlist = gcnew ToolStripMenuItem("行情列表");
//                          item_quotlist->Click += gcnew System::EventHandler(this, &FormMain::item_quotlist_Click);
//                          ((ToolStripDropDownItem^)(contextMenuStrip_Sub->Items[4]))->DropDownItems->Add(item_quotlist);

                     m_currInstrumentText = dataGridView_Position->Rows[e->RowIndex]->Cells["合约"]->Value->ToString();
                     contextMenuStrip_Sub->Show(MousePosition.X, MousePosition.Y);
                 }
             }
         }


private: System::Void dataGridView_Position_ColumnHeaderMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if (e->Button == System::Windows::Forms::MouseButtons::Right)   
             {   
                 m_activeDataGridView = dataGridView_Position;
                 m_activeDataTable = m_DataTablePosition;

                 //弹出操作菜单
                 contextMenuStrip_Sub->Items->Clear(); 

                 ToolStripItem^ item_hide = gcnew ToolStripMenuItem("隐藏此列");
                 item_hide->Click += gcnew System::EventHandler(this, &FormMain::item_hide_Click);
                 contextMenuStrip_Sub->Items->Add(item_hide);

                 //已经隐藏的列
                 map<string, map<string, int>>::iterator it = g_mapColumnText2Index.find(DataGridView_Position);
                 if(it!=g_mapColumnText2Index.end())
                 {
                     map<string, int> mapColumnText2Index = it->second;

                     ToolStripItem^ item_show = gcnew ToolStripMenuItem("显示列");
                     contextMenuStrip_Sub->Items->Add(item_show);

                     map<string,int>::iterator it = mapColumnText2Index.begin();
                     for (;it!=mapColumnText2Index.end();it++)
                     {
                         ToolStripItem^ item_hide_lists = gcnew ToolStripMenuItem(Tools::string2String((it->first).c_str()));
                         item_hide_lists->Tag = Tools::string2String((it->first).c_str());
                         item_hide_lists->Click += gcnew System::EventHandler(this, &FormMain::item_hide_lists_Click);
                         ((ToolStripDropDownItem^)(contextMenuStrip_Sub->Items[1]))->DropDownItems->Add(item_hide_lists);
                     }
                 } 

                 ToolStripItem^ item_auto = gcnew ToolStripMenuItem("自动调整列宽");
                 item_auto->Click += gcnew System::EventHandler(this, &FormMain::item_auto_Click);
                 contextMenuStrip_Sub->Items->Add(item_auto);

                 //                  ToolStripItem^ item_setup = gcnew ToolStripMenuItem("表格列设置");
                 //                  item_setup->Click += gcnew System::EventHandler(this, &FormMain::item_setup_Click);
                 //                  contextMenuStrip_Sub->Items->Add(item_setup);

                 ToolStripItem^ item_output = gcnew ToolStripMenuItem("导出列表");
                 item_output->Click += gcnew System::EventHandler(this, &FormMain::item_output_Click);
                 contextMenuStrip_Sub->Items->Add(item_output);


                 m_nCurrColumnHeaderIndex_MouseRight = e->ColumnIndex;
                 m_CurrDataGridViewText = DataGridView_Position;
                 m_CurrColumnHeaderText = this->dataGridView_Position->Columns[e->ColumnIndex]->Name;  

                 contextMenuStrip_Sub->Show(MousePosition.X, MousePosition.Y);
             }
         }
private: System::Void dataGridView_ClosePosition_CellMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
                      if (e->Button == System::Windows::Forms::MouseButtons::Right)
                      {
                          if (e->RowIndex >= 0 && e->RowIndex < dataGridView_ClosePosition->RowCount)
                          {                         
                              //若行已是选中状态就不再进行设置
                              if (dataGridView_ClosePosition->Rows[e->RowIndex]->Selected == false)
                              {
                                  dataGridView_ClosePosition->ClearSelection();
                                  dataGridView_ClosePosition->Rows[e->RowIndex]->Selected = true;
                              }
                              //只选中一行时设置活动单元格
                              if (dataGridView_ClosePosition->SelectedRows->Count == 1)
                              {
                                  dataGridView_ClosePosition->CurrentCell = dataGridView_ClosePosition->Rows[e->RowIndex]->Cells[e->ColumnIndex];
                              }

                              m_activeDataGridView = dataGridView_ClosePosition;
                              m_activeDataTable = m_DataTableTrade;

                              //弹出操作菜单
                              contextMenuStrip_Sub->Items->Clear();

                              ToolStripItem^ item_output = gcnew ToolStripMenuItem("导出列表");
                              item_output->Click += gcnew System::EventHandler(this, &FormMain::item_output_Click);
                              contextMenuStrip_Sub->Items->Add(item_output);

                              ToolStripItem^ item_addto = gcnew ToolStripMenuItem("添加到行情列表");
                              item_addto->Click += gcnew System::EventHandler(this, &FormMain::item_addto_Click);
                              contextMenuStrip_Sub->Items->Add(item_addto);

                              contextMenuStrip_Sub->Show(MousePosition.X, MousePosition.Y);
                          }
                      }
                  }
private: System::Void dataGridView_ClosePosition_ColumnHeaderMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if (e->Button == System::Windows::Forms::MouseButtons::Right)   
             {   
                 m_activeDataGridView = dataGridView_ClosePosition;
                 m_activeDataTable = m_DataTableTrade;

                 //弹出操作菜单
                 contextMenuStrip_Sub->Items->Clear(); 

                 ToolStripItem^ item_hide = gcnew ToolStripMenuItem("隐藏此列");
                 item_hide->Click += gcnew System::EventHandler(this, &FormMain::item_hide_Click);
                 contextMenuStrip_Sub->Items->Add(item_hide);

                 //已经隐藏的列
                 map<string, map<string, int>>::iterator it = g_mapColumnText2Index.find(DataGridView_ClosePosition);
                 if(it!=g_mapColumnText2Index.end())
                 {
                     map<string, int> mapColumnText2Index = it->second;

                     ToolStripItem^ item_show = gcnew ToolStripMenuItem("显示列");
                     contextMenuStrip_Sub->Items->Add(item_show);

                     map<string,int>::iterator it = mapColumnText2Index.begin();
                     for (;it!=mapColumnText2Index.end();it++)
                     {
                         ToolStripItem^ item_hide_lists = gcnew ToolStripMenuItem(Tools::string2String((it->first).c_str()));
                         item_hide_lists->Tag = Tools::string2String((it->first).c_str());
                         item_hide_lists->Click += gcnew System::EventHandler(this, &FormMain::item_hide_lists_Click);
                         ((ToolStripDropDownItem^)(contextMenuStrip_Sub->Items[1]))->DropDownItems->Add(item_hide_lists);
                     }
                 } 

                 ToolStripItem^ item_auto = gcnew ToolStripMenuItem("自动调整列宽");
                 item_auto->Click += gcnew System::EventHandler(this, &FormMain::item_auto_Click);
                 contextMenuStrip_Sub->Items->Add(item_auto);

                 //                  ToolStripItem^ item_setup = gcnew ToolStripMenuItem("表格列设置");
                 //                  item_setup->Click += gcnew System::EventHandler(this, &FormMain::item_setup_Click);
                 //                  contextMenuStrip_Sub->Items->Add(item_setup);

                 ToolStripItem^ item_output = gcnew ToolStripMenuItem("导出列表");
                 item_output->Click += gcnew System::EventHandler(this, &FormMain::item_output_Click);
                 contextMenuStrip_Sub->Items->Add(item_output);


                 m_nCurrColumnHeaderIndex_MouseRight = e->ColumnIndex;
                 m_CurrDataGridViewText = DataGridView_ClosePosition;
                 m_CurrColumnHeaderText = this->dataGridView_Position->Columns[e->ColumnIndex]->Name;  

                 contextMenuStrip_Sub->Show(MousePosition.X, MousePosition.Y);
             }
         }
private: System::Void dataGridView_QuotOrderTable_ColumnHeaderMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if (e->Button == System::Windows::Forms::MouseButtons::Right)   
             {   
                 m_activeDataGridView = dataGridView_QuotOrderTable;
                 m_activeDataTable = m_DataTableDepthMarketData;

                 //弹出操作菜单
                 contextMenuStrip_Sub->Items->Clear(); 

                 ToolStripItem^ item_hide = gcnew ToolStripMenuItem("隐藏此列");
                 item_hide->Click += gcnew System::EventHandler(this, &FormMain::item_hide_Click);
                 contextMenuStrip_Sub->Items->Add(item_hide);

                 //已经隐藏的列
                 map<string, map<string, int>>::iterator it = g_mapColumnText2Index.find(DataGridView_DepthMarket);
                 if(it!=g_mapColumnText2Index.end())
                 {
                     map<string, int> mapColumnText2Index = it->second;

                     ToolStripItem^ item_show = gcnew ToolStripMenuItem("显示列");
                     contextMenuStrip_Sub->Items->Add(item_show);

                     map<string,int>::iterator it = mapColumnText2Index.begin();
                     for (;it!=mapColumnText2Index.end();it++)
                     {
                         ToolStripItem^ item_hide_lists = gcnew ToolStripMenuItem(Tools::string2String((it->first).c_str()));
                         item_hide_lists->Tag = Tools::string2String((it->first).c_str());
                         item_hide_lists->Click += gcnew System::EventHandler(this, &FormMain::item_hide_lists_Click);
                         ((ToolStripDropDownItem^)(contextMenuStrip_Sub->Items[1]))->DropDownItems->Add(item_hide_lists);
                     }
                 } 

                 ToolStripItem^ item_auto = gcnew ToolStripMenuItem("自动调整列宽");
                 item_auto->Click += gcnew System::EventHandler(this, &FormMain::item_auto_Click);
                 contextMenuStrip_Sub->Items->Add(item_auto);

//                  ToolStripItem^ item_setup = gcnew ToolStripMenuItem("表格列设置");
//                  item_setup->Click += gcnew System::EventHandler(this, &FormMain::item_setup_Click);
//                  contextMenuStrip_Sub->Items->Add(item_setup);

                 m_nCurrColumnHeaderIndex_MouseRight = e->ColumnIndex;
                 m_CurrDataGridViewText = DataGridView_DepthMarket;
                 m_CurrColumnHeaderText = this->dataGridView_QuotOrderTable->Columns[e->ColumnIndex]->Name;  

                 contextMenuStrip_Sub->Show(MousePosition.X, MousePosition.Y);
             }   
         }
private: System::Void dataGridView_QuotOrderTable_CellPainting(System::Object^  sender, System::Windows::Forms::DataGridViewCellPaintingEventArgs^  e) {
          if (e->RowIndex < 0)
              return;

          DataGridViewRow^ dr = dataGridView_QuotOrderTable->Rows[e->RowIndex];
          if (e->ColumnIndex == 0)//定位到合约列
          {
              e->CellStyle->ForeColor = Color::Yellow;  //将前景色设置为黄色，即字体颜色设置为黄色
          }//定位到最新价、涨跌、涨跌幅、买价、卖价列
          else if (e->ColumnIndex == 1 || e->ColumnIndex == 2 || e->ColumnIndex == 3 || e->ColumnIndex == 4 || e->ColumnIndex == 6)
          {
              String^ szValue = dr->Cells["涨跌"]->Value->ToString();
              double  dValue = Convert::ToDouble(szValue);

              if (dValue>=0.000001)
                  e->CellStyle->ForeColor = Color::Red;     //涨：将前景色设置为红色，即字体颜色设置为红色
              else
                  e->CellStyle->ForeColor = Color::FromArgb(0,255,64);  //跌：将前景色设置为绿色，即字体颜色设置为绿色
          }

      }
private: System::Void dataGridView_RiskEvent_ColumnHeaderMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if (e->Button == System::Windows::Forms::MouseButtons::Right)   
             {   
                 m_activeDataGridView = dataGridView_RiskEvent;
                 m_activeDataTable = m_DataTableRiskEvent;

                 //弹出操作菜单
                 contextMenuStrip_Sub->Items->Clear(); 

                 ToolStripItem^ item_hide = gcnew ToolStripMenuItem("隐藏此列");
                 item_hide->Click += gcnew System::EventHandler(this, &FormMain::item_hide_Click);
                 contextMenuStrip_Sub->Items->Add(item_hide);

                 //已经隐藏的列
                 map<string, map<string, int>>::iterator it = g_mapColumnText2Index.find(DataGridView_RiskEvent);
                 if(it!=g_mapColumnText2Index.end())
                 {
                     map<string, int> mapColumnText2Index = it->second;

                     ToolStripItem^ item_show = gcnew ToolStripMenuItem("显示列");
                     contextMenuStrip_Sub->Items->Add(item_show);

                     map<string,int>::iterator it = mapColumnText2Index.begin();
                     for (;it!=mapColumnText2Index.end();it++)
                     {
                         ToolStripItem^ item_hide_lists = gcnew ToolStripMenuItem(Tools::string2String((it->first).c_str()));
                         item_hide_lists->Tag = Tools::string2String((it->first).c_str());
                         item_hide_lists->Click += gcnew System::EventHandler(this, &FormMain::item_hide_lists_Click);
                         ((ToolStripDropDownItem^)(contextMenuStrip_Sub->Items[1]))->DropDownItems->Add(item_hide_lists);
                     }
                 }


                 ToolStripItem^ item_auto = gcnew ToolStripMenuItem("自动调整列宽");
                 item_auto->Click += gcnew System::EventHandler(this, &FormMain::item_auto_Click);
                 contextMenuStrip_Sub->Items->Add(item_auto);

//                  ToolStripItem^ item_setup = gcnew ToolStripMenuItem("表格列设置");
//                  item_setup->Click += gcnew System::EventHandler(this, &FormMain::item_setup_Click);
//                  contextMenuStrip_Sub->Items->Add(item_setup);

                 m_nCurrColumnHeaderIndex_MouseRight = e->ColumnIndex;
                 m_CurrDataGridViewText = DataGridView_RiskEvent;
                 m_CurrColumnHeaderText = this->dataGridView_RiskEvent->Columns[e->ColumnIndex]->Name;  

                 contextMenuStrip_Sub->Show(MousePosition.X, MousePosition.Y);
             }   
         }
private: System::Void item_auto_Click(System::Object^ sender, System::EventArgs^ e)
{            
    if(nullptr == m_activeDataGridView)
        return;

    m_activeDataGridView->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
}
private: System::Void item_output_Click(System::Object^ sender, System::EventArgs^ e)
{            
    SaveFileDialog^ saveFileDialog = gcnew SaveFileDialog();
    saveFileDialog->InitialDirectory = System::Environment::CurrentDirectory;

    saveFileDialog->Filter = "Excel 2007|*.xlsx|所有文件|*.*";
    saveFileDialog->RestoreDirectory = true;
    saveFileDialog->FilterIndex = 1;
    if (saveFileDialog->ShowDialog() == ::DialogResult::OK)
    {
        String^ lsPath = saveFileDialog->FileName->ToUpper();
        if (lsPath->EndsWith("XLSX"))
        {					 
            CDataOpr::ExportToExcel(m_activeDataTable,saveFileDialog->FileName);
        }
    }
}
private: System::Void item_setup_Click(System::Object^ sender, System::EventArgs^ e)
{            
}
private: System::Void item_sendmsg_Click(System::Object^ sender, System::EventArgs^ e)
{            
    String^ szTraderAccount = m_activeDataGridView->Rows[m_activeDataGridView->CurrentCell->RowIndex]->Cells[0]->Value->ToString();

    UserInfo userInfo;
	if(CRiskData::GetInstance()->GetUserInfoByAccount(Tools::String2string(szTraderAccount), userInfo)) {
		std::string strUserName(userInfo.szAccount);

		CFromSendMsgSingleUser^ dlg = gcnew CFromSendMsgSingleUser();
		dlg->ResetUser(m_pLogin->mnUserID, userInfo.nUserID, strUserName);
		dlg->ShowDialog();
	}
}
private: System::Void item_limittrade_Click(System::Object^ sender, System::EventArgs^ e)
{          
    String^ szTraderAccount = m_activeDataGridView->Rows[m_activeDataGridView->CurrentCell->RowIndex]->Cells[0]->Value->ToString();

    UserInfo userInfo;
    if(CRiskData::GetInstance()->GetUserInfoByAccount(Tools::String2string(szTraderAccount),userInfo))
    {
        SLimitTrade sLimitTrade;
        sLimitTrade.nUserID = userInfo.nUserID;
        sLimitTrade.bLimitTrade = true;

        CTcpLayer::SendData(Cmd_RM_SetLimitTrade_Req, (char*)&sLimitTrade, sizeof(SLimitTrade), 0, m_pLogin->mnUserID);
    }
}
private: System::Void item_limitcancle_Click(System::Object^ sender, System::EventArgs^ e)
{    
    String^ szTraderAccount = m_activeDataGridView->Rows[m_activeDataGridView->CurrentCell->RowIndex]->Cells[0]->Value->ToString();

    UserInfo userInfo;
    if(CRiskData::GetInstance()->GetUserInfoByAccount(Tools::String2string(szTraderAccount),userInfo))
    {
        SLimitTrade sLimitTrade;
        sLimitTrade.nUserID = userInfo.nUserID;
        sLimitTrade.bLimitTrade = false;

        CTcpLayer::SendData(Cmd_RM_SetLimitTrade_Req, (char*)&sLimitTrade, sizeof(SLimitTrade), 0, m_pLogin->mnUserID);
    }
}
private: System::Void item_manualverify_Click(System::Object^ sender, System::EventArgs^ e)
{            
    String^ szTraderAccount = m_activeDataGridView->Rows[m_activeDataGridView->CurrentCell->RowIndex]->Cells[0]->Value->ToString();

    UserInfo userInfo;
    if(CRiskData::GetInstance()->GetUserInfoByAccount(Tools::String2string(szTraderAccount),userInfo))
    {
        SManualVerify sManualVerify;
        sManualVerify.nUserID = userInfo.nUserID;
        sManualVerify.bManualVerify = true;

        CTcpLayer::SendData(Cmd_RM_SetManualVerify_Req, (char*)&sManualVerify, sizeof(SManualVerify), 0, m_pLogin->mnUserID);
    }
}
private: System::Void item_verifycancle_Click(System::Object^ sender, System::EventArgs^ e)
{           
    String^ szTraderAccount = m_activeDataGridView->Rows[m_activeDataGridView->CurrentCell->RowIndex]->Cells[0]->Value->ToString();

    UserInfo userInfo;
    if(CRiskData::GetInstance()->GetUserInfoByAccount(Tools::String2string(szTraderAccount),userInfo))
    {
        SManualVerify sManualVerify;
        sManualVerify.nUserID = userInfo.nUserID;
        sManualVerify.bManualVerify = false;

        CTcpLayer::SendData(Cmd_RM_SetManualVerify_Req, (char*)&sManualVerify, sizeof(SManualVerify), 0, m_pLogin->mnUserID);
    }
}
private: System::Void item_forceclose_Click(System::Object^ sender, System::EventArgs^ e)
         {            
             double fPriceTick = CRiskData::GetInstance()->GetPriceTickByInstID(Tools::String2string(m_currInstrumentText));

             sForceClose oFC;
             if(GetForceCloseInfo(oFC))
             {         
                 FormForceClose^ dlg = gcnew FormForceClose(oFC,fPriceTick);
                 if(::DialogResult::OK == dlg->ShowDialog())
                 {
                     PlatformStru_InputOrder OrderInsert;
                     memset(&OrderInsert,0,sizeof(OrderInsert));

                     if(dlg->GetParam(OrderInsert))
                     {
                         char szReason[256];//强平的原因（哪个风控员下手动强平指令）
                         string szRiskerAccount = CRiskData::GetInstance()->GetRiskAccount();
                         sprintf(szReason,"%s(%s)%s","风控员",szRiskerAccount.c_str(),"下的手动强平指令");                        
                        
                         SManualForceClose sManual;
                         memcpy(&sManual.nInputOrder,&OrderInsert,sizeof(PlatformStru_InputOrder));
                         sManual.nFrontID = m_pLogin->mnUserID;
                         sManual.nSessionID = m_pLogin->mnSessionID;
                         sprintf(sManual.szOrderRef,"%d",CRiskData::GetInstance()->GetOrderRef());

                         memcpy(sManual.szReason,szReason,sizeof(sManual.szReason));

                         string szRiskAccount = CRiskData::GetInstance()->GetRiskAccount();
                         memcpy(sManual.szRiskerName,szRiskAccount.c_str(),strlen(szRiskAccount.c_str())+1);


                         CTcpLayer::SendData(Cmd_RM_ForceCloseOrderInsert_Req, (char*)(&sManual), sizeof(sManual), 0, m_pLogin->mnUserID);
                     }
                 }
             }
         }
private: System::Void item_addto_Click(System::Object^ sender, System::EventArgs^ e)
{            
    String^ strInst = m_activeDataGridView->Rows[m_activeDataGridView->CurrentCell->RowIndex]->Cells["合约"]->Value->ToString();
    string szInst = Tools::String2string(strInst);
    string szExchange = CRiskData::GetInstance()->GetExchangeByInstrument(szInst);

    //检查行情是否已经订阅
    if(!CRiskData::GetInstance()->bSubScribeInst(szInst)) 
    {
        CTcpDataConnect::SendData(Cmd_SubscribeQuotEvent_Req, (char*)szInst.c_str(), szInst.length()+1, 0, 0, m_pLogin->mnUserID);        
        CRiskData::GetInstance()->SubScribeInst(szExchange,szInst);
    }
    if(!CRiskData::GetInstance()->bSubScribeInst2(szInst))
    {
        CRiskData::GetInstance()->SubScribeInst2(szExchange,szInst);
    }
}
private: System::Void item_closeorder_Click(System::Object^ sender, System::EventArgs^ e)
         {            
             int     nFrontID       = Convert::ToInt32(m_activeDataGridView->Rows[m_activeDataGridView->CurrentCell->RowIndex]->Cells[16]->Value);
             int     nSessionID     = Convert::ToInt32(m_activeDataGridView->Rows[m_activeDataGridView->CurrentCell->RowIndex]->Cells[17]->Value);
             String^ szOrderRef     = m_activeDataGridView->Rows[m_activeDataGridView->CurrentCell->RowIndex]->Cells[18]->Value->ToString();
             String^ szAccount      = m_activeDataGridView->Rows[m_activeDataGridView->CurrentCell->RowIndex]->Cells[0]->Value->ToString();
             String^ szOrderSysID   = m_activeDataGridView->Rows[m_activeDataGridView->CurrentCell->RowIndex]->Cells[1]->Value->ToString();
             String^ szExchangeID   = m_activeDataGridView->Rows[m_activeDataGridView->CurrentCell->RowIndex]->Cells[15]->Value->ToString();
             String^ szInst         = m_activeDataGridView->Rows[m_activeDataGridView->CurrentCell->RowIndex]->Cells[2]->Value->ToString();
             string szRiskAccount   = CRiskData::GetInstance()->GetRiskAccount();

             PlatformStru_InputOrderAction tAction;
             memset(&tAction, 0, sizeof(tAction));

             tAction.Thost.ActionFlag = THOST_FTDC_AF_Delete;
             tAction.Thost.FrontID=nFrontID;
             tAction.Thost.SessionID=nSessionID;
             memcpy(tAction.Thost.InvestorID,Tools::String2string(szAccount).c_str(), sizeof(TThostFtdcInvestorIDType));
             memcpy(tAction.Thost.OrderRef, Tools::String2string(szOrderRef).c_str(), sizeof(TThostFtdcOrderRefType));
             memcpy(tAction.Thost.OrderSysID, Tools::String2string(szOrderSysID).c_str(), sizeof(TThostFtdcOrderSysIDType));
             memcpy(tAction.Thost.ExchangeID, Tools::String2string(szExchangeID).c_str(), sizeof(TThostFtdcExchangeIDType));
             memcpy(tAction.Thost.InstrumentID, Tools::String2string(szInst).c_str(), sizeof(TThostFtdcInstrumentIDType));

             char szReason[256];//撤单的原因（哪个风控员下撤单指令）
             string szRiskerAccount = CRiskData::GetInstance()->GetRiskAccount();
             sprintf(szReason,"%s(%s)%s","风控员",szRiskerAccount.c_str(),"下的撤单指令");  

             SRiskInputOrderAction sInputOrder;
             memcpy(&sInputOrder.nInputOrderAction, &tAction, sizeof(PlatformStru_InputOrderAction));
             sInputOrder.nRequestID = 0;
             strcpy(sInputOrder.nsActionReson, szReason);             
             memcpy(sInputOrder.szRiskerName,szRiskAccount.c_str(),strlen(szRiskAccount.c_str())+1);

             
             CTcpLayer::SendData(Cmd_RM_OrderAction_Req, (char*)&sInputOrder, sizeof(SRiskInputOrderAction), 0, m_pLogin->mnUserID);
         }
private: System::Void item_hide_Click(System::Object^ sender, System::EventArgs^ e)
         {            
             if(-1 == m_nCurrColumnHeaderIndex_MouseRight)
                 return;

             map<string, map<string,int>>::iterator it = g_mapColumnText2Index.find(Tools::String2string(m_CurrDataGridViewText));
             if(it!=g_mapColumnText2Index.end())
             {
                 map<string,int>& mapColumnText2Index = it->second;
                 mapColumnText2Index.insert(make_pair(Tools::String2string(m_CurrColumnHeaderText),m_nCurrColumnHeaderIndex_MouseRight));
             }
             else
             {
                 map<string,int> mapColumnText2Index;
                 mapColumnText2Index.insert(make_pair(Tools::String2string(m_CurrColumnHeaderText),m_nCurrColumnHeaderIndex_MouseRight));
                 g_mapColumnText2Index.insert(make_pair(Tools::String2string(m_CurrDataGridViewText),mapColumnText2Index));
             }

             //m_DataTableDepthMarketData->Columns->RemoveAt(m_nCurrColumnHeaderIndex_MouseRight);
             m_activeDataGridView->Columns[m_nCurrColumnHeaderIndex_MouseRight]->Visible = false;
         }
private: System::Void item_hide_lists_Click(System::Object^ sender, System::EventArgs^ e)
         {            
             //m_DataTableDepthMarketData->Columns->Add(gcnew DataColumn("合约",  ::System::String::typeid));

             ToolStripItem^ tlsi = (ToolStripItem^)sender;
             String^ szText = (String^)(tlsi->Tag);

             map<string, map<string,int>>::iterator it = g_mapColumnText2Index.find(Tools::String2string(m_CurrDataGridViewText));
             if(it!=g_mapColumnText2Index.end())
             {
                 map<string,int>& mapColumnText2Index = it->second;
                 map<string,int>::iterator it_hide = mapColumnText2Index.find(Tools::String2string(szText));
                 if(it_hide!=mapColumnText2Index.end())
                 {
                     m_activeDataGridView->Columns[it_hide->second]->Visible = true;
                     mapColumnText2Index.erase(it_hide);
                 }
             }
         }
private: System::Void radioButton_Position_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
             SetDataTableColumns(radioButton_Position->Checked);
         }
private: bool SendVerifyOrder(bool bPass)
		 {
             int nSelectedCount = 0;
			 SVerisyOrder orderKey;
			 for(int i = 0; i < dataGridView_NeedVerify->SelectedRows->Count; i++) {
				 memset(&orderKey, 0, sizeof(orderKey));
				 strncpy(orderKey.orderKey.Account, 
					 Tools::String2string(dataGridView_NeedVerify->SelectedRows[i]->Cells[0]->Value->ToString()).c_str(), 
						sizeof(orderKey.orderKey.Account)-1);
				 strncpy(orderKey.orderKey.InstrumentID, 
						Tools::String2string(dataGridView_NeedVerify->SelectedRows[i]->Cells[2]->Value->ToString()).c_str(), 
						sizeof(orderKey.orderKey.InstrumentID)-1);
				 strncpy(orderKey.orderKey.OrderRef, 
						Tools::String2string(dataGridView_NeedVerify->SelectedRows[i]->Cells[18]->Value->ToString()).c_str(), 
						sizeof(orderKey.orderKey.OrderRef)-1);
				 orderKey.orderKey.FrontID = Int32::Parse(dataGridView_NeedVerify->SelectedRows[i]->Cells[16]->Value->ToString());
				 orderKey.orderKey.SessionID = Int32::Parse(dataGridView_NeedVerify->SelectedRows[i]->Cells[17]->Value->ToString());
				 orderKey.nVerifyUser = m_pLogin==NULL ? 0 : m_pLogin->mnUserID;
				 orderKey.nResult = bPass ? 1 : 0;
                 nSelectedCount++; 
				CTcpLayer::SendData(Cmd_RM_VerifyOrder_Req, 
						(void*)&orderKey, sizeof(SVerisyOrder), 0, 
						m_pLogin==NULL ? 0 : m_pLogin->mnUserID);
			 }
             if(nSelectedCount > 0) 
				return true;
			 else
				 return false;
		 }

private: System::Void treeView_Organization_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
             //过滤右击树控件来的事件
             if(bCurrUserID_MouseRight)
                 return;
             
             m_DataTableTradingAccount->Rows->Clear();
             m_DataTableAllOrder->Rows->Clear();
             m_DataTableTrade->Rows->Clear();
             m_DataTablePosition->Rows->Clear();
             //m_DataTableRiskEvent->Rows->Clear();     
             //dataGridView_OrgRiskSetup->Rows->Clear();

             //先退订部分   
             UserInfo userInfo;
             vector<int> vecUserID;

             for each (String^ szTraderAccount in m_vecTraderAccountFromTree)
             {
                 CRiskData::GetInstance()->GetUserInfoByAccount(Tools::String2string(szTraderAccount),userInfo);
                 vecUserID.push_back(userInfo.nUserID);
             }
             
             int nHwnd = this->Handle.ToInt32();
             if(!vecUserID.empty())
             {
                 CTcpLayer::SendData(Cmd_RM_UnSubscribeFund_Req, &vecUserID[0],sizeof(int)*vecUserID.size(), 0, m_pLogin->mnUserID);
                 CTcpLayer::SendData(Cmd_RM_UnSubscribeOrder_Req, &vecUserID[0],sizeof(int)*vecUserID.size(), 0, m_pLogin->mnUserID);
                 CTcpLayer::SendData(Cmd_RM_UnSubscribePosition_Req, &vecUserID[0],sizeof(int)*vecUserID.size(), 0, m_pLogin->mnUserID);
                 CTcpLayer::SendData(Cmd_RM_UnSubscribeTrade_Req, &vecUserID[0],sizeof(int)*vecUserID.size(), 0, m_pLogin->mnUserID);
             }

             vecUserID.clear();
             m_vecTraderAccountFromTree.clear();

             int nID = ::Convert::ToInt32(treeView_Organization->SelectedNode->Tag); 
             String ^szText = treeView_Organization->SelectedNode->Text;  

             //正则表达式 true： 纯数值是交易员 false：组织机构
             if (Regex::IsMatch(szText, "^[0-9]*$"))
             {
                 if(CRiskData::GetInstance()->GetUserInfoByID(nID,userInfo))
                 {
                     vecUserID.push_back(nID);
                     m_vecTraderAccountFromTree.push_back(Tools::string2String(userInfo.szAccount));
                 }
             }
             else
             {
                 //                  vector<int> vecMonitorOrgIDAll;
                 //                  CRiskData::GetInstance()->GetOrgIDByUserID(m_pLogin->mnUserID,USER_ORG_RELATION_MONITOR,vecMonitorOrgIDAll);
                 // 
                 //                  vector<int> vecMonitorOrgID;
                 //                  CRiskData::GetInstance()->GetMonitorOrgByOrgID(nID,vecMonitorOrgIDAll,vecMonitorOrgID); 
                 //                  if(!vecMonitorOrgID.empty())
                 //                      CTcpLayer::SendData(Cmd_RM_QueryRiskPlan_Req, &vecMonitorOrgID[0],sizeof(int)*vecMonitorOrgID.size(), 0, m_pLogin->mnUserID);  

                 GetUserIDFormOrgTreeNode(treeView_Organization->SelectedNode,vecUserID);
                 if(!vecUserID.empty())
                 {
                     for(vector<int>::iterator it=vecUserID.begin();it!=vecUserID.end();it++)
                     {
                         if(CRiskData::GetInstance()->GetUserInfoByID(*it,userInfo))
                             m_vecTraderAccountFromTree.push_back(Tools::string2String(userInfo.szAccount));
                     }
                 }
             }

             Subscribe(vecUserID);
         }
private: System::Void treeView_Organization_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			if(e->Button==System::Windows::Forms::MouseButtons::Right) {
				if(treeView_Organization->HitTest(e->X, e->Y)->Node==nullptr)
					return;
				System::Windows::Forms::TreeNode^ node = treeView_Organization->HitTest(e->X, e->Y)->Node;       
                
                //正则表达式 true： 纯数值是交易员 false：组织机构
                if (!Regex::IsMatch(node->Text, "^[0-9]*$"))
                    return;

				int nUserID = int::Parse(node->Tag->ToString());
				std::string strUserName;
                bCurrUserID_MouseRight = true;
				nCurrUserID_MouseRight = -1;

                //弹出操作菜单
                cntMenuOriganTree->Items->Clear();
				if(CRiskData::GetInstance()->GetUserNameByID(nUserID, strUserName)) {
					nCurrUserID_MouseRight = nUserID;

                    ToolStripItem^ item_sendmsg = gcnew ToolStripMenuItem("发送消息");
                    item_sendmsg->Click += gcnew System::EventHandler(this, &FormMain::发送消息ToolStripMenuItem_Click);
                    cntMenuOriganTree->Items->Add(item_sendmsg);

                    bool bLimit = CRiskData::GetInstance()->IsLimitTrade(nUserID);
                    bool bVerify = CRiskData::GetInstance()->IsManualVerify(nUserID);

                    if(bLimit)
                    {
                        ToolStripItem^ item_limittrade = gcnew ToolStripMenuItem("取消限制");
                        item_limittrade->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_CancelLimit_Click);
                        cntMenuOriganTree->Items->Add(item_limittrade);
                    }
                    else
                    {
                        ToolStripItem^ item_limitcancle = gcnew ToolStripMenuItem("限制交易");
                        item_limitcancle->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_TradeLimit_Click);
                        cntMenuOriganTree->Items->Add(item_limitcancle);
                    }

                    if(!bVerify)
                    {
                        ToolStripItem^ item_manualverify = gcnew ToolStripMenuItem("手动审核");
                        item_manualverify->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_ManualVerify_Click);
                        cntMenuOriganTree->Items->Add(item_manualverify);
                    }
                    else
                    {
                        ToolStripItem^ item_verifycancle = gcnew ToolStripMenuItem("取消审核");
                        item_verifycancle->Click += gcnew System::EventHandler(this, &FormMain::ToolStripMenuItem_CancelVerify_Click);
                        cntMenuOriganTree->Items->Add(item_verifycancle); 
                    }

                    cntMenuOriganTree->Show(treeView_Organization, System::Drawing::Point(e->X, e->Y));
				}

                treeView_Organization->SelectedNode = node;
            }  
       else if(e->Button==System::Windows::Forms::MouseButtons::Left)
                bCurrUserID_MouseRight = false;

		 }
private: System::Void 发送消息ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 std::string strUserName;
			 sLoginRsp logInfo;

			 memset(&logInfo, 0, sizeof(logInfo));
			 CRiskData::GetInstance()->GetLogonInfo(logInfo);
			 CRiskData::GetInstance()->GetUserNameByID(nCurrUserID_MouseRight, strUserName);
			 CFromSendMsgSingleUser^ dlg = gcnew CFromSendMsgSingleUser();
			 dlg->ResetUser(logInfo.mnUserID, nCurrUserID_MouseRight, strUserName);
             dlg->ShowDialog();
		 }
private: System::Void ToolStripMenuItem_TradeLimit_Click(System::Object^  sender, System::EventArgs^  e) {
             String^ szTraderAccount = treeView_Organization->SelectedNode->Text;

             UserInfo userInfo;
             if(CRiskData::GetInstance()->GetUserInfoByAccount(Tools::String2string(szTraderAccount),userInfo))
             {
                 SLimitTrade sLimitTrade;
                 sLimitTrade.nUserID = userInfo.nUserID;
                 sLimitTrade.bLimitTrade = true;

                 CTcpLayer::SendData(Cmd_RM_SetLimitTrade_Req, (char*)&sLimitTrade, sizeof(SLimitTrade), 0, m_pLogin->mnUserID);
             }
         }
private: System::Void ToolStripMenuItem_CancelLimit_Click(System::Object^  sender, System::EventArgs^  e) {
             String^ szTraderAccount = treeView_Organization->SelectedNode->Text;

             UserInfo userInfo;
             if(CRiskData::GetInstance()->GetUserInfoByAccount(Tools::String2string(szTraderAccount),userInfo))
             {
                 SLimitTrade sLimitTrade;
                 sLimitTrade.nUserID = userInfo.nUserID;
                 sLimitTrade.bLimitTrade = false;

                 CTcpLayer::SendData(Cmd_RM_SetLimitTrade_Req, (char*)&sLimitTrade, sizeof(SLimitTrade), 0, m_pLogin->mnUserID);
             }

         }
private: System::Void ToolStripMenuItem_ManualVerify_Click(System::Object^  sender, System::EventArgs^  e) {
             String^ szTraderAccount = treeView_Organization->SelectedNode->Text;

             UserInfo userInfo;
             if(CRiskData::GetInstance()->GetUserInfoByAccount(Tools::String2string(szTraderAccount),userInfo))
             {
                 SManualVerify sManualVerify;
                 sManualVerify.nUserID = userInfo.nUserID;
                 sManualVerify.bManualVerify = true;

                 CTcpLayer::SendData(Cmd_RM_SetManualVerify_Req, (char*)&sManualVerify, sizeof(SManualVerify), 0, m_pLogin->mnUserID);
             }

         }
private: System::Void ToolStripMenuItem_CancelVerify_Click(System::Object^  sender, System::EventArgs^  e) {
             String^ szTraderAccount = treeView_Organization->SelectedNode->Text;

             UserInfo userInfo;
             if(CRiskData::GetInstance()->GetUserInfoByAccount(Tools::String2string(szTraderAccount),userInfo))
             {
                 SManualVerify sManualVerify;
                 sManualVerify.nUserID = userInfo.nUserID;
                 sManualVerify.bManualVerify = false;

                 CTcpLayer::SendData(Cmd_RM_SetManualVerify_Req, (char*)&sManualVerify, sizeof(SManualVerify), 0, m_pLogin->mnUserID);
             }
         }
};
}

