#pragma once

#include "RiskData.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

/*
extern vector<string> g_vecTreeNodeTitle;//��֯����������
extern map<string,vector<string>> g_mapTreeNode2Column;//��֯�������ƶ�Ӧ���б�����

extern std::vector<std::string> g_vecTitleDepthMarket;
extern std::vector<std::string> g_vecTitleRiskEvent;
extern std::vector<std::string> g_vecTitleTradingAccount;
extern std::vector<std::string> g_vecTitleAllOrder;
extern std::vector<std::string> g_vecTitleTrade;
extern std::vector<std::string> g_vecTitlePosition;
extern std::vector<std::string> g_vecTitleUnTrade;
extern std::vector<std::string> g_vecTitleClosePosition;
extern std::vector<std::string> g_vecTitleNeedVerify;
*/
namespace CltRisk {

	/// <summary>
	/// Summary for FormTableColumnSetup
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormTableColumnSetup : public System::Windows::Forms::Form
	{
	public:
		FormTableColumnSetup(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_selectionIdx = 0;
            LoadData();
        }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormTableColumnSetup()
		{
			if (components)
			{
				delete components;
			}
		}
    private: int m_selectionIdx;

    private: System::Windows::Forms::TreeView^  treeView;
    private: System::Windows::Forms::DataGridView^  dataGridView;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
    private: System::Windows::Forms::DataGridViewCheckBoxColumn^  Column2;

    private: System::Windows::Forms::Button^  button_OK;
    private: System::Windows::Forms::Button^  button_Cancel;
    private: System::Windows::Forms::Button^  button_up;
    private: System::Windows::Forms::Button^  button_down;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;



#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle3 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            this->treeView = (gcnew System::Windows::Forms::TreeView());
            this->dataGridView = (gcnew System::Windows::Forms::DataGridView());
            this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column2 = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
            this->button_OK = (gcnew System::Windows::Forms::Button());
            this->button_Cancel = (gcnew System::Windows::Forms::Button());
            this->button_up = (gcnew System::Windows::Forms::Button());
            this->button_down = (gcnew System::Windows::Forms::Button());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView))->BeginInit();
            this->SuspendLayout();
            // 
            // treeView
            // 
            this->treeView->HideSelection = false;
            this->treeView->Location = System::Drawing::Point(12, 12);
            this->treeView->Name = L"treeView";
            this->treeView->Size = System::Drawing::Size(144, 273);
            this->treeView->TabIndex = 0;
            this->treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &FormTableColumnSetup::treeView_AfterSelect);
            // 
            // dataGridView
            // 
            this->dataGridView->AllowDrop = true;
            this->dataGridView->AllowUserToAddRows = false;
            this->dataGridView->AllowUserToOrderColumns = true;
            this->dataGridView->AllowUserToResizeRows = false;
            this->dataGridView->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
            dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle1->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle1->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle1->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle1->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle1->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
            this->dataGridView->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
            this->dataGridView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {this->Column1, 
                this->Column2});
            dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle2->BackColor = System::Drawing::SystemColors::Window;
            dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle2->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle2->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle2->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle2->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridView->DefaultCellStyle = dataGridViewCellStyle2;
            this->dataGridView->EnableHeadersVisualStyles = false;
            this->dataGridView->Location = System::Drawing::Point(184, 12);
            this->dataGridView->MultiSelect = false;
            this->dataGridView->Name = L"dataGridView";
            this->dataGridView->RowHeadersVisible = false;
            this->dataGridView->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
            dataGridViewCellStyle3->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            this->dataGridView->RowsDefaultCellStyle = dataGridViewCellStyle3;
            this->dataGridView->RowTemplate->Height = 23;
            this->dataGridView->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->dataGridView->Size = System::Drawing::Size(166, 273);
            this->dataGridView->TabIndex = 1;
            this->dataGridView->CellMouseMove += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormTableColumnSetup::dataGridView_CellMouseMove);
            this->dataGridView->RowsAdded += gcnew System::Windows::Forms::DataGridViewRowsAddedEventHandler(this, &FormTableColumnSetup::dataGridView_RowsAdded);
            this->dataGridView->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &FormTableColumnSetup::dataGridView_DragEnter);
            this->dataGridView->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &FormTableColumnSetup::dataGridView_DragDrop);
            // 
            // Column1
            // 
            this->Column1->Frozen = true;
            this->Column1->HeaderText = L"����";
            this->Column1->Name = L"Column1";
            this->Column1->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            this->Column1->Width = 35;
            // 
            // Column2
            // 
            this->Column2->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
            this->Column2->HeaderText = L"��ʾ";
            this->Column2->Name = L"Column2";
            this->Column2->Width = 35;
            // 
            // button_OK
            // 
            this->button_OK->Location = System::Drawing::Point(97, 302);
            this->button_OK->Name = L"button_OK";
            this->button_OK->Size = System::Drawing::Size(75, 23);
            this->button_OK->TabIndex = 2;
            this->button_OK->Text = L"ȷ��";
            this->button_OK->UseVisualStyleBackColor = true;
            this->button_OK->Click += gcnew System::EventHandler(this, &FormTableColumnSetup::button_OK_Click);
            // 
            // button_Cancel
            // 
            this->button_Cancel->Location = System::Drawing::Point(190, 302);
            this->button_Cancel->Name = L"button_Cancel";
            this->button_Cancel->Size = System::Drawing::Size(75, 23);
            this->button_Cancel->TabIndex = 3;
            this->button_Cancel->Text = L"ȡ��";
            this->button_Cancel->UseVisualStyleBackColor = true;
            this->button_Cancel->Click += gcnew System::EventHandler(this, &FormTableColumnSetup::button_Cancel_Click);
            // 
            // button_up
            // 
            this->button_up->Location = System::Drawing::Point(358, 98);
            this->button_up->Name = L"button_up";
            this->button_up->Size = System::Drawing::Size(46, 23);
            this->button_up->TabIndex = 4;
            this->button_up->Text = L"����";
            this->button_up->UseVisualStyleBackColor = true;
            this->button_up->Click += gcnew System::EventHandler(this, &FormTableColumnSetup::button_up_Click);
            // 
            // button_down
            // 
            this->button_down->Location = System::Drawing::Point(358, 174);
            this->button_down->Name = L"button_down";
            this->button_down->Size = System::Drawing::Size(46, 23);
            this->button_down->TabIndex = 4;
            this->button_down->Text = L"����";
            this->button_down->UseVisualStyleBackColor = true;
            this->button_down->Click += gcnew System::EventHandler(this, &FormTableColumnSetup::button_down_Click);
            // 
            // FormTableColumnSetup
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(417, 337);
            this->Controls->Add(this->button_down);
            this->Controls->Add(this->button_up);
            this->Controls->Add(this->button_Cancel);
            this->Controls->Add(this->button_OK);
            this->Controls->Add(this->dataGridView);
            this->Controls->Add(this->treeView);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->Name = L"FormTableColumnSetup";
            this->Text = L"���������";
            this->Load += gcnew System::EventHandler(this, &FormTableColumnSetup::FormTableColumnSetup_Load);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView))->EndInit();
            this->ResumeLayout(false);

        }
#pragma endregion
private: void LoadTree() 
{  /*    
    if(g_vecTreeNodeTitle.empty())
        return;

    TreeNode^ node;
    for (int i=0;i<(int)g_vecTreeNodeTitle.size();i++)
    {           
        if(i==0)
        {                    
            node = treeView->Nodes->Add(i.ToString(), Tools::string2String(g_vecTreeNodeTitle[i].c_str()));
            node->Tag = i;
        }
        else
        {
            TreeNode^ subNode;
            subNode = node->Nodes->Add(i.ToString(),Tools::string2String(g_vecTreeNodeTitle[i].c_str()));
            subNode->Tag = i;
        }
    }

    node->Expand();*/
}
private: void LoadData()
      {/*
          vector<string> vecColumnText;
          
          //����������           
          g_vecTreeNodeTitle.push_back("���������");
          g_vecTreeNodeTitle.push_back("���۱�");
          g_vecTreeNodeTitle.push_back("��ǰ�����¼�");
          g_vecTreeNodeTitle.push_back("�˻����ʽ�");
          g_vecTreeNodeTitle.push_back("ί�е�");
          g_vecTreeNodeTitle.push_back("δ�ɽ���");
          g_vecTreeNodeTitle.push_back("�ɽ���");
          g_vecTreeNodeTitle.push_back("�ֲ�");
          g_vecTreeNodeTitle.push_back("ƽ��");
          g_vecTreeNodeTitle.push_back("����˵�");

          
          vecColumnText.push_back("��Լ");
          vecColumnText.push_back("���¼�");
          vecColumnText.push_back("�ǵ�");
          vecColumnText.push_back("�ǵ���");
          vecColumnText.push_back("���");
          vecColumnText.push_back("����");
          vecColumnText.push_back("����");
          vecColumnText.push_back("����");
          vecColumnText.push_back("�ɽ���");
          vecColumnText.push_back("�ֲ���");
          vecColumnText.push_back("����");
          vecColumnText.push_back("����");
          vecColumnText.push_back("�����");
          vecColumnText.push_back("��߼�");
          vecColumnText.push_back("��ͼ�");
          vecColumnText.push_back("��ͣ��");
          vecColumnText.push_back("��ͣ��");
          vecColumnText.push_back("����ʱ��");
          g_mapTreeNode2Column.insert(make_pair("���۱�",vecColumnText));

          vecColumnText.clear();
          vecColumnText.push_back("�����¼�ID");
          vecColumnText.push_back("����ԱID");
          vecColumnText.push_back("����Ա����");
          vecColumnText.push_back("����ָ������");
          vecColumnText.push_back("��������");
          vecColumnText.push_back("�����¼�״̬");
          g_mapTreeNode2Column.insert(make_pair("��ǰ�����¼�",vecColumnText));

          vecColumnText.clear();
          vecColumnText.push_back("����Ա");
          vecColumnText.push_back("��������");
          vecColumnText.push_back("�ֶ����");
          vecColumnText.push_back("��¼״̬");
          vecColumnText.push_back("���ն�");
          vecColumnText.push_back("��̬Ȩ��");
          vecColumnText.push_back("�ֲ�ӯ��");
          vecColumnText.push_back("ƽ��ӯ��");
          vecColumnText.push_back("ռ�ñ�֤��");
          vecColumnText.push_back("������");
          vecColumnText.push_back("���ᱣ֤��");
          vecColumnText.push_back("����������");
          g_mapTreeNode2Column.insert(make_pair("�˻����ʽ�",vecColumnText));

          vecColumnText.clear();
          vecColumnText.push_back("����Ա");
          vecColumnText.push_back("�������");
          vecColumnText.push_back("��Լ");
          vecColumnText.push_back("����");
          vecColumnText.push_back("��ƽ");
          vecColumnText.push_back("�ҵ�״̬");
          vecColumnText.push_back("�����۸�");
          vecColumnText.push_back("��������");
          vecColumnText.push_back("δ�ɽ�����");
          vecColumnText.push_back("�ɽ�����");
          vecColumnText.push_back("��ϸ״̬");
          vecColumnText.push_back("����ʱ��");
          vecColumnText.push_back("���ᱣ֤��");
          vecColumnText.push_back("����������");
          vecColumnText.push_back("Ͷ��");
          vecColumnText.push_back("������");
          vecColumnText.push_back("ǰ�û�");
          vecColumnText.push_back("�Ự��");
          vecColumnText.push_back("��������");
          vecColumnText.push_back("״̬");
          g_mapTreeNode2Column.insert(make_pair("ί�е�",vecColumnText));
          g_mapTreeNode2Column.insert(make_pair("δ�ɽ���",vecColumnText));
          g_mapTreeNode2Column.insert(make_pair("����˵�",vecColumnText));

          vecColumnText.clear();
          vecColumnText.push_back("����Ա");
          vecColumnText.push_back("��Լ");
          vecColumnText.push_back("����");
          vecColumnText.push_back("��ƽ");
          vecColumnText.push_back("�ɽ��۸�");
          vecColumnText.push_back("�ɽ�����");
          vecColumnText.push_back("������");
          vecColumnText.push_back("�ɽ�ʱ��");
          vecColumnText.push_back("�ɽ�����");
          vecColumnText.push_back("Ͷ��");
          vecColumnText.push_back("�ɽ����");
          vecColumnText.push_back("�������");
          vecColumnText.push_back("������");
          g_mapTreeNode2Column.insert(make_pair("�ɽ���",vecColumnText));
          g_mapTreeNode2Column.insert(make_pair("ƽ��",vecColumnText));

          vecColumnText.clear();
          vecColumnText.push_back("����Ա");
          vecColumnText.push_back("��Լ");
          vecColumnText.push_back("����");
          vecColumnText.push_back("Ͷ��");
          vecColumnText.push_back("�ֲܳ�");
          vecColumnText.push_back("��ƽ��");
          vecColumnText.push_back("�ּ�");
          vecColumnText.push_back("�ֲ־���");
          vecColumnText.push_back("���־���");
          vecColumnText.push_back("�ֲ�ӯ��");
          vecColumnText.push_back("����ӯ��");
          vecColumnText.push_back("������ӯ��");
          vecColumnText.push_back("ռ�ñ�֤��");
          vecColumnText.push_back("�ܶ��");
          vecColumnText.push_back("�ܿղ�");
          vecColumnText.push_back("����");
          vecColumnText.push_back("��ղ�");
          vecColumnText.push_back("���");
          vecColumnText.push_back("���");
          vecColumnText.push_back("�񿪲���");
          vecColumnText.push_back("��ƽ����");
          vecColumnText.push_back("�ܿ�ƽ����");
          vecColumnText.push_back("�ܿ�ƽ����");
          vecColumnText.push_back("ƽ��ҵ���");
          vecColumnText.push_back("ƽ�ֹҵ���");
          vecColumnText.push_back("��ƽ����");
          vecColumnText.push_back("��ƽ����");
          vecColumnText.push_back("������");
          g_mapTreeNode2Column.insert(make_pair("�ֲ�",vecColumnText));              


          //�����ļ��е�����
          string szDataGridView = "";
          int nColumnIndex = -1;
          vector<string> vecColumn;

          map<string,vector<string>> mapDataGridView2Column = CRiskData::GetInstance()->GetConfigFile(XML_TYPE_COLUMN);
           for(map<string,vector<string>>::iterator it = mapDataGridView2Column.begin();it != mapDataGridView2Column.end();it++)
           {
               szDataGridView = it->first;
               if(0==strcmp(szDataGridView.c_str(),DataGridView_DepthMarket))
               {
                   vecColumn = it->second;                     
                   for (vector<string>::iterator it_column = vecColumn.begin();it_column != vecColumn.end();it_column++)
                   {
                       nColumnIndex = atoi((*it_column).c_str());
                       if(nColumnIndex == conCol_DepthMarket_InstrumentID)     {g_vecTitleDepthMarket.push_back("��Լ");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_DepthMarket_LastPrice)        {g_vecTitleDepthMarket.push_back("���¼�");continue;}
                       if(nColumnIndex == conCol_DepthMarket_Mixed)            {g_vecTitleDepthMarket.push_back("�ǵ�");continue;}
                       if(nColumnIndex == conCol_DepthMarket_MixedRange)       {g_vecTitleDepthMarket.push_back("�ǵ���");continue;}
                       if(nColumnIndex == conCol_DepthMarket_BidPrice1)        {g_vecTitleDepthMarket.push_back("���");continue;}
                       if(nColumnIndex == conCol_DepthMarket_BidVolume1)       {g_vecTitleDepthMarket.push_back("����");continue;}
                       if(nColumnIndex == conCol_DepthMarket_AskPrice1)        {g_vecTitleDepthMarket.push_back("����");continue;}
                       if(nColumnIndex == conCol_DepthMarket_AskVolume1)       {g_vecTitleDepthMarket.push_back("����");continue;}
                       if(nColumnIndex == conCol_DepthMarket_Volume)           {g_vecTitleDepthMarket.push_back("�ɽ���");continue;}
                       if(nColumnIndex == conCol_DepthMarket_OpenInterest)     {g_vecTitleDepthMarket.push_back("�ֲ���");continue;}
                       if(nColumnIndex == conCol_DepthMarket_CurVolume)        {g_vecTitleDepthMarket.push_back("����");continue;}
                       if(nColumnIndex == conCol_DepthMarket_OpenPrice)        {g_vecTitleDepthMarket.push_back("����");continue;}
                       if(nColumnIndex == conCol_DepthMarket_SettlementPrice)  {g_vecTitleDepthMarket.push_back("�����");continue;}
                       if(nColumnIndex == conCol_DepthMarket_HighestPrice)     {g_vecTitleDepthMarket.push_back("��߼�");continue;}
                       if(nColumnIndex == conCol_DepthMarket_LowestPrice)      {g_vecTitleDepthMarket.push_back("��ͼ�");continue;}
                       if(nColumnIndex == conCol_DepthMarket_UpperLimitPrice)  {g_vecTitleDepthMarket.push_back("��ͣ��");continue;}
                       if(nColumnIndex == conCol_DepthMarket_LowerLimitPrice)  {g_vecTitleDepthMarket.push_back("��ͣ��");continue;}
                       if(nColumnIndex == conCol_DepthMarket_UpdateTime)       {g_vecTitleDepthMarket.push_back("����ʱ��");continue;}
                   }

                   continue;
               }else if(0==strcmp(szDataGridView.c_str(),DataGridView_RiskEvent))
               {
                   vecColumn = it->second;
                   for (vector<string>::iterator it_column = vecColumn.begin();it_column != vecColumn.end();it_column++)
                   {
                       nColumnIndex = atoi((*it_column).c_str());
                       if(nColumnIndex == conCol_RiskEvent_RiskEventID)     {g_vecTitleRiskEvent.push_back("�����¼�ID");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_RiskEvent_TradeInvestorID) {g_vecTitleRiskEvent.push_back("����ԱID");continue;}
                       if(nColumnIndex == conCol_RiskEvent_TraderName)      {g_vecTitleRiskEvent.push_back("����Ա����");continue;}
                       if(nColumnIndex == conCol_RiskEvent_IndicatorName)   {g_vecTitleRiskEvent.push_back("����ָ������");continue;}
                       if(nColumnIndex == conCol_RiskEvent_RiskDesc)        {g_vecTitleRiskEvent.push_back("��������");continue;}
                       if(nColumnIndex == conCol_RiskEvent_EventTime)       {g_vecTitleRiskEvent.push_back("����ʱ��");continue;}
                       if(nColumnIndex == conCol_RiskEvent_EventStatus)     {g_vecTitleRiskEvent.push_back("�����¼�״̬");continue;}
                   }

                   continue;
               }else if(0==strcmp(szDataGridView.c_str(),DataGridView_TradingAccount))
               {
                   vecColumn = it->second;
                   for (vector<string>::iterator it_column = vecColumn.begin();it_column != vecColumn.end();it_column++)
                   {
                       nColumnIndex = atoi((*it_column).c_str());
                       if(nColumnIndex == conCol_TradingAccount_Account)           {g_vecTitleTradingAccount.push_back("����Ա");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_TradingAccount_LimitTade)         {g_vecTitleTradingAccount.push_back("��������");continue;}
                       if(nColumnIndex == conCol_TradingAccount_ManuVerify)        {g_vecTitleTradingAccount.push_back("�ֶ����");continue;}
                       if(nColumnIndex == conCol_TradingAccount_LoginStatus)       {g_vecTitleTradingAccount.push_back("��¼״̬");continue;}
                       if(nColumnIndex == conCol_TradingAccount_RiskDegree)        {g_vecTitleTradingAccount.push_back("���ն�(%)");continue;}
                       if(nColumnIndex == conCol_TradingAccount_DynamicProfit)     {g_vecTitleTradingAccount.push_back("��̬Ȩ��");continue;}
                       if(nColumnIndex == conCol_TradingAccount_PositionProfit)    {g_vecTitleTradingAccount.push_back("�ֲ�ӯ��");continue;}
                       if(nColumnIndex == conCol_TradingAccount_CloseProfit)       {g_vecTitleTradingAccount.push_back("ƽ��ӯ��");continue;}
                       if(nColumnIndex == conCol_TradingAccount_CurrMargin)        {g_vecTitleTradingAccount.push_back("ռ�ñ�֤��");continue;}
                       if(nColumnIndex == conCol_TradingAccount_Commission)        {g_vecTitleTradingAccount.push_back("������");continue;}
                       if(nColumnIndex == conCol_TradingAccount_FrozenMargin)      {g_vecTitleTradingAccount.push_back("���ᱣ֤��");continue;}
                       if(nColumnIndex == conCol_TradingAccount_FrozenCommission)  {g_vecTitleTradingAccount.push_back("����������");continue;}
                   }

                   continue;
               }else if(0==strcmp(szDataGridView.c_str(),DataGridView_AllOrder))
               {
                   vecColumn = it->second;
                   for (vector<string>::iterator it_column = vecColumn.begin();it_column != vecColumn.end();it_column++)
                   {
                       nColumnIndex = atoi((*it_column).c_str());
                       if(nColumnIndex == conCol_AllOrder_InvestorID)          {g_vecTitleAllOrder.push_back("����Ա");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_AllOrder_OrderSysID)          {g_vecTitleAllOrder.push_back("�������");continue;}
                       if(nColumnIndex == conCol_AllOrder_InstrumentID)        {g_vecTitleAllOrder.push_back("��Լ");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_AllOrder_Direction)           {g_vecTitleAllOrder.push_back("����");continue;}
                       if(nColumnIndex == conCol_AllOrder_CombOffsetFlag)      {g_vecTitleAllOrder.push_back("��ƽ");continue;}
                       if(nColumnIndex == conCol_AllOrder_OrderStatus)         {g_vecTitleAllOrder.push_back("�ҵ�״̬");continue;}
                       if(nColumnIndex == conCol_AllOrder_LimitPrice)          {g_vecTitleAllOrder.push_back("�����۸�");continue;}
                       if(nColumnIndex == conCol_AllOrder_VolumeTotalOriginal) {g_vecTitleAllOrder.push_back("��������");continue;}
                       if(nColumnIndex == conCol_AllOrder_VolumeTotal)         {g_vecTitleAllOrder.push_back("δ�ɽ�����");continue;}
                       if(nColumnIndex == conCol_AllOrder_Volume)              {g_vecTitleAllOrder.push_back("�ɽ�����");continue;}
                       if(nColumnIndex == conCol_AllOrder_StatusMsg)           {g_vecTitleAllOrder.push_back("��ϸ״̬");continue;}
                       if(nColumnIndex == conCol_AllOrder_InsertTime)          {g_vecTitleAllOrder.push_back("����ʱ��");continue;}
                       if(nColumnIndex == conCol_AllOrder_freezeMargin)        {g_vecTitleAllOrder.push_back("���ᱣ֤��");continue;}
                       if(nColumnIndex == conCol_AllOrder_troubleMoney)        {g_vecTitleAllOrder.push_back("����������");continue;}
                       if(nColumnIndex == conCol_AllOrder_HedgeFlag)           {g_vecTitleAllOrder.push_back("Ͷ��");continue;}
                       if(nColumnIndex == conCol_AllOrder_ExchangeID)          {g_vecTitleAllOrder.push_back("������");continue;}
                       if(nColumnIndex == conCol_AllOrder_FrontID)             {g_vecTitleAllOrder.push_back("ǰ�û�");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_AllOrder_SessionID)           {g_vecTitleAllOrder.push_back("�Ự��");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_AllOrder_OrderRef)            {g_vecTitleAllOrder.push_back("��������");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_AllOrder_ExStatus)            {g_vecTitleAllOrder.push_back("״̬");continue;}//RowFilter ����δ�ɽ��б������б�
                   }

                   continue;
               }else if(0==strcmp(szDataGridView.c_str(),DataGridView_Trade))
               {
                   vecColumn = it->second;
                   for (vector<string>::iterator it_column = vecColumn.begin();it_column != vecColumn.end();it_column++)
                   {
                       nColumnIndex = atoi((*it_column).c_str());
                       if(nColumnIndex == conCol_Trade_InvestorID)         {g_vecTitleTrade.push_back("����Ա");continue;}
                       if(nColumnIndex == conCol_Trade_InstrumentID)       {g_vecTitleTrade.push_back("��Լ");continue;}
                       if(nColumnIndex == conCol_Trade_Direction)          {g_vecTitleTrade.push_back("����");continue;}
                       if(nColumnIndex == conCol_Trade_OffsetFlag)         {g_vecTitleTrade.push_back("��ƽ");continue;}
                       if(nColumnIndex == conCol_Trade_Price)              {g_vecTitleTrade.push_back("�ɽ��۸�");continue;}
                       if(nColumnIndex == conCol_Trade_Volume)             {g_vecTitleTrade.push_back("�ɽ�����");continue;}
                       if(nColumnIndex == conCol_Trade_TradeCommission)    {g_vecTitleTrade.push_back("������");continue;}
                       if(nColumnIndex == conCol_Trade_TradeTime)          {g_vecTitleTrade.push_back("�ɽ�ʱ��");continue;}
                       if(nColumnIndex == conCol_Trade_TradeType)          {g_vecTitleTrade.push_back("�ɽ�����");continue;}
                       if(nColumnIndex == conCol_Trade_HedgeFlag)          {g_vecTitleTrade.push_back("Ͷ��");continue;}
                       if(nColumnIndex == conCol_Trade_TradeID)            {g_vecTitleTrade.push_back("�ɽ����");continue;}
                       if(nColumnIndex == conCol_Trade_OrderSysID)         {g_vecTitleTrade.push_back("�������");continue;}
                       if(nColumnIndex == conCol_Trade_ExchangeID)         {g_vecTitleTrade.push_back("������");continue;}
                   }

                   continue;
               }else if(0==strcmp(szDataGridView.c_str(),DataGridView_Position))
               {
                   vecColumn = it->second;
                   for (vector<string>::iterator it_column = vecColumn.begin();it_column != vecColumn.end();it_column++)
                   {
                       nColumnIndex = atoi((*it_column).c_str());
                       if(nColumnIndex == conCol_Position_InvestorID)                  {g_vecTitlePosition.push_back("����Ա");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_Position_InstrumentID)                {g_vecTitlePosition.push_back("��Լ");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_Position_Direction)                   {g_vecTitlePosition.push_back("����");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_Position_HedgeFlag)                   {g_vecTitlePosition.push_back("Ͷ��");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_Position_Position)                    {g_vecTitlePosition.push_back("�ֲܳ�");continue;}
                       if(nColumnIndex == conCol_Position_CanCloseVolume)              {g_vecTitlePosition.push_back("��ƽ��");continue;}
                       if(nColumnIndex == conCol_Position_LastPrice)                   {g_vecTitlePosition.push_back("�ּ�");continue;}
                       if(nColumnIndex == conCol_Position_AveragePositionPrice)        {g_vecTitlePosition.push_back("�ֲ־���");continue;}
                       if(nColumnIndex == conCol_Position_AverageOpenPrice)            {g_vecTitlePosition.push_back("���־���");continue;}
                       if(nColumnIndex == conCol_Position_PositionProfit)              {g_vecTitlePosition.push_back("�ֲ�ӯ��");continue;}
                       if(nColumnIndex == conCol_Position_PositionProfitByTrade)       {g_vecTitlePosition.push_back("����ӯ��");continue;}
                       if(nColumnIndex == conCol_Position_TotalPositionProfitByDate)   {g_vecTitlePosition.push_back("������ӯ��");continue;}
                       if(nColumnIndex == conCol_Position_UseMargin)                   {g_vecTitlePosition.push_back("ռ�ñ�֤��");continue;}
                       if(nColumnIndex == conCol_Position_MultiBuyPosition)            {g_vecTitlePosition.push_back("�ܶ��");continue;}
                       if(nColumnIndex == conCol_Position_MultiSellPosition)           {g_vecTitlePosition.push_back("�ܿղ�");continue;}
                       if(nColumnIndex == conCol_Position_TodayBuyPosition)            {g_vecTitlePosition.push_back("����");continue;}
                       if(nColumnIndex == conCol_Position_TodaySellPosition)           {g_vecTitlePosition.push_back("��ղ�");continue;}
                       if(nColumnIndex == conCol_Position_YesterdayPosition)           {g_vecTitlePosition.push_back("���");continue;}
                       if(nColumnIndex == conCol_Position_TodayPosition)               {g_vecTitlePosition.push_back("���");continue;}
                       if(nColumnIndex == conCol_Position_OpenVolume)                  {g_vecTitlePosition.push_back("�񿪲���");continue;}
                       if(nColumnIndex == conCol_Position_CloseVolume)                 {g_vecTitlePosition.push_back("��ƽ����");continue;}
                       if(nColumnIndex == conCol_Position_TotalCanCloseTodayVolume)    {g_vecTitlePosition.push_back("�ܿ�ƽ����");continue;}
                       if(nColumnIndex == conCol_Position_TotalCanCloseYesterdayVolume){g_vecTitlePosition.push_back("�ܿ�ƽ����");continue;}
                       if(nColumnIndex == conCol_Position_CloseTodayOrderVolume)       {g_vecTitlePosition.push_back("ƽ��ҵ���");continue;}
                       if(nColumnIndex == conCol_Position_CloseOrderVolume)            {g_vecTitlePosition.push_back("ƽ�ֹҵ���");continue;}
                       if(nColumnIndex == conCol_Position_CanCloseTodayVolume)         {g_vecTitlePosition.push_back("��ƽ����");continue;}
                       if(nColumnIndex == conCol_Position_CanCloseYesterdayVolume)     {g_vecTitlePosition.push_back("��ƽ����");continue;}
                       if(nColumnIndex == conCol_Position_ExchangeID)                  {g_vecTitlePosition.push_back("������");continue;}
                   }

                   continue;
               }
           }*/
      } 
private: int GetTagFromText(String^ szText)
         {
             if(szText->CompareTo("��Լ")==0)           return conCol_DepthMarket_InstrumentID;//PrimaryKey
             if(szText->CompareTo("���¼�")==0)         return conCol_DepthMarket_LastPrice;
             if(szText->CompareTo("�ǵ�")==0)           return conCol_DepthMarket_Mixed;
             if(szText->CompareTo("�ǵ���")==0)         return conCol_DepthMarket_MixedRange;
             if(szText->CompareTo("���")==0)           return conCol_DepthMarket_BidPrice1;
             if(szText->CompareTo("����")==0)           return conCol_DepthMarket_BidVolume1;
             if(szText->CompareTo("����")==0)           return conCol_DepthMarket_AskPrice1;
             if(szText->CompareTo("����")==0)           return conCol_DepthMarket_AskVolume1;
             if(szText->CompareTo("�ɽ���")==0)         return conCol_DepthMarket_Volume;
             if(szText->CompareTo("�ֲ���")==0)         return conCol_DepthMarket_OpenInterest;
             if(szText->CompareTo("����")==0)           return conCol_DepthMarket_CurVolume;
             if(szText->CompareTo("����")==0)         return conCol_DepthMarket_OpenPrice;
             if(szText->CompareTo("�����")==0)         return conCol_DepthMarket_SettlementPrice;
             if(szText->CompareTo("��߼�")==0)         return conCol_DepthMarket_HighestPrice;
             if(szText->CompareTo("��ͼ�")==0)         return conCol_DepthMarket_LowestPrice;
             if(szText->CompareTo("��ͣ��")==0)         return conCol_DepthMarket_UpperLimitPrice;
             if(szText->CompareTo("��ͣ��")==0)         return conCol_DepthMarket_LowerLimitPrice;
             if(szText->CompareTo("����ʱ��")==0)       return conCol_DepthMarket_UpdateTime;

             if(szText->CompareTo("�����¼�ID")==0)     return conCol_RiskEvent_RiskEventID;//PrimaryKey
             if(szText->CompareTo("����ԱID")==0)       return conCol_RiskEvent_TradeInvestorID;
             if(szText->CompareTo("����Ա����")==0)     return conCol_RiskEvent_TraderName;
             if(szText->CompareTo("����ָ������")==0)   return conCol_RiskEvent_IndicatorName;
             if(szText->CompareTo("��������")==0)       return conCol_RiskEvent_RiskDesc;
             if(szText->CompareTo("����ʱ��")==0)       return conCol_RiskEvent_EventTime;
             if(szText->CompareTo("�����¼�״̬")==0)   return conCol_RiskEvent_EventStatus;

             if(szText->CompareTo("����Ա")==0)         return conCol_TradingAccount_Account;//PrimaryKey
             if(szText->CompareTo("��������")==0)       return conCol_TradingAccount_LimitTade;
             if(szText->CompareTo("�ֶ����")==0)       return conCol_TradingAccount_ManuVerify;
             if(szText->CompareTo("��¼״̬")==0)       return conCol_TradingAccount_LoginStatus;
             if(szText->CompareTo("���ն�(%)")==0)      return conCol_TradingAccount_RiskDegree;
             if(szText->CompareTo("��̬Ȩ��")==0)       return conCol_TradingAccount_DynamicProfit;
             if(szText->CompareTo("�ֲ�ӯ��")==0)       return conCol_TradingAccount_PositionProfit;
             if(szText->CompareTo("ƽ��ӯ��")==0)       return conCol_TradingAccount_CloseProfit;
             if(szText->CompareTo("ռ�ñ�֤��")==0)     return conCol_TradingAccount_CurrMargin;
             if(szText->CompareTo("������")==0)         return conCol_TradingAccount_Commission;
             if(szText->CompareTo("���ᱣ֤��")==0)     return conCol_TradingAccount_FrozenMargin;
             if(szText->CompareTo("����������")==0)     return conCol_TradingAccount_FrozenCommission;

             if(szText->CompareTo("����Ա")==0)         return conCol_AllOrder_InvestorID;//PrimaryKey
             if(szText->CompareTo("�������")==0)       return conCol_AllOrder_OrderSysID;
             if(szText->CompareTo("��Լ")==0)           return conCol_AllOrder_InstrumentID;//PrimaryKey
             if(szText->CompareTo("����")==0)           return conCol_AllOrder_Direction;
             if(szText->CompareTo("��ƽ")==0)           return conCol_AllOrder_CombOffsetFlag;
             if(szText->CompareTo("�ҵ�״̬")==0)       return conCol_AllOrder_OrderStatus;
             if(szText->CompareTo("�����۸�")==0)       return conCol_AllOrder_LimitPrice;
             if(szText->CompareTo("��������")==0)       return conCol_AllOrder_VolumeTotalOriginal;
             if(szText->CompareTo("δ�ɽ�����")==0)     return conCol_AllOrder_VolumeTotal;
             if(szText->CompareTo("�ɽ�����")==0)       return conCol_AllOrder_Volume;
             if(szText->CompareTo("�ɽ�����")==0)       return conCol_AllOrder_Volume;
             if(szText->CompareTo("�ɽ�����")==0)       return conCol_AllOrder_Volume;
             if(szText->CompareTo("��ϸ״̬")==0)       return conCol_AllOrder_StatusMsg;
             if(szText->CompareTo("����ʱ��")==0)       return conCol_AllOrder_InsertTime;
             if(szText->CompareTo("���ᱣ֤��")==0)     return conCol_AllOrder_freezeMargin;
             if(szText->CompareTo("����������")==0)     return conCol_AllOrder_troubleMoney;
             if(szText->CompareTo("Ͷ��")==0)           return conCol_AllOrder_HedgeFlag;
             if(szText->CompareTo("������")==0)         return conCol_AllOrder_ExchangeID;
             if(szText->CompareTo("ǰ�û�")==0)         return conCol_AllOrder_FrontID;//PrimaryKey
             if(szText->CompareTo("�Ự��")==0)         return conCol_AllOrder_SessionID;//PrimaryKey
             if(szText->CompareTo("��������")==0)       return conCol_AllOrder_OrderRef;//PrimaryKey
             if(szText->CompareTo("״̬")==0)           return conCol_AllOrder_ExStatus;//RowFilter ����δ�ɽ��б������б�

             if(szText->CompareTo("����Ա")==0)         return conCol_Trade_InvestorID;
             if(szText->CompareTo("��Լ")==0)           return conCol_Trade_InstrumentID;
             if(szText->CompareTo("����")==0)           return conCol_Trade_Direction;
             if(szText->CompareTo("��ƽ")==0)           return conCol_Trade_OffsetFlag;
             if(szText->CompareTo("�ɽ��۸�")==0)       return conCol_Trade_Price;
             if(szText->CompareTo("�ɽ�����")==0)       return conCol_Trade_Volume;
             if(szText->CompareTo("������")==0)         return conCol_Trade_TradeCommission;
             if(szText->CompareTo("�ɽ�ʱ��")==0)       return conCol_Trade_TradeTime;
             if(szText->CompareTo("�ɽ�����")==0)       return conCol_Trade_TradeType;
             if(szText->CompareTo("Ͷ��")==0)           return conCol_Trade_HedgeFlag;
             if(szText->CompareTo("�ɽ����")==0)       return conCol_Trade_TradeID;
             if(szText->CompareTo("�������")==0)       return conCol_Trade_OrderSysID;
             if(szText->CompareTo("������")==0)         return conCol_Trade_ExchangeID;

             if(szText->CompareTo("����Ա")==0)         return conCol_Position_InvestorID;//PrimaryKey
             if(szText->CompareTo("��Լ")==0)           return conCol_Position_InstrumentID;//PrimaryKey
             if(szText->CompareTo("����")==0)           return conCol_Position_Direction;//PrimaryKey
             if(szText->CompareTo("Ͷ��")==0)           return conCol_Position_HedgeFlag;//PrimaryKey
             if(szText->CompareTo("�ֲܳ�")==0)         return conCol_Position_Position;
             if(szText->CompareTo("��ƽ��")==0)         return conCol_Position_CanCloseVolume;
             if(szText->CompareTo("�ּ�")==0)           return conCol_Position_LastPrice;
             if(szText->CompareTo("�ֲ־���")==0)       return conCol_Position_AveragePositionPrice;
             if(szText->CompareTo("���־���")==0)       return conCol_Position_AverageOpenPrice;
             if(szText->CompareTo("�ֲ�ӯ��")==0)       return conCol_Position_PositionProfit;
             if(szText->CompareTo("����ӯ��")==0)       return conCol_Position_PositionProfitByTrade;
             if(szText->CompareTo("������ӯ��")==0)     return conCol_Position_TotalPositionProfitByDate;
             if(szText->CompareTo("ռ�ñ�֤��")==0)     return conCol_Position_UseMargin;
             if(szText->CompareTo("�ܶ��")==0)         return conCol_Position_MultiBuyPosition;
             if(szText->CompareTo("�ܿղ�")==0)         return conCol_Position_MultiSellPosition;
             if(szText->CompareTo("����")==0)         return conCol_Position_TodayBuyPosition;
             if(szText->CompareTo("��ղ�")==0)         return conCol_Position_TodaySellPosition;
             if(szText->CompareTo("���")==0)           return conCol_Position_YesterdayPosition;
             if(szText->CompareTo("���")==0)           return conCol_Position_TodayPosition;
             if(szText->CompareTo("�񿪲���")==0)       return conCol_Position_OpenVolume;
             if(szText->CompareTo("��ƽ����")==0)       return conCol_Position_CloseVolume;
             if(szText->CompareTo("�ܿ�ƽ����")==0)     return conCol_Position_TotalCanCloseTodayVolume;
             if(szText->CompareTo("�ܿ�ƽ����")==0)     return conCol_Position_TotalCanCloseYesterdayVolume;
             if(szText->CompareTo("ƽ��ҵ���")==0)     return conCol_Position_CloseTodayOrderVolume;
             if(szText->CompareTo("ƽ�ֹҵ���")==0)     return conCol_Position_CloseOrderVolume;
             if(szText->CompareTo("��ƽ����")==0)       return conCol_Position_CanCloseTodayVolume;
             if(szText->CompareTo("��ƽ����")==0)       return conCol_Position_CanCloseYesterdayVolume;
             if(szText->CompareTo("������")==0)         return conCol_Position_ExchangeID;

             return conCol_Position_None;
         }
private: System::Void FormTableColumnSetup_Load(System::Object^  sender, System::EventArgs^  e) {
             CenterToParent();
             LoadTree();
         }
private: bool IsChecked(String^ szTreeText,String^ szColumnText)
          {/*
              if(szTreeText->CompareTo("���۱�") == 0)
              {
                  vector<string>::iterator it = g_vecTitleDepthMarket.begin();
                  for (;it!=g_vecTitleDepthMarket.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }else if(szTreeText->CompareTo("��ǰ�����¼�") == 0)
              {
                  vector<string>::iterator it = g_vecTitleRiskEvent.begin();
                  for (;it!=g_vecTitleRiskEvent.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }else if(szTreeText->CompareTo("�˻����ʽ�") == 0)
              {
                  vector<string>::iterator it = g_vecTitleTradingAccount.begin();
                  for (;it!=g_vecTitleTradingAccount.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }else if(szTreeText->CompareTo("ί�е�") == 0)
              {
                  vector<string>::iterator it = g_vecTitleAllOrder.begin();
                  for (;it!=g_vecTitleAllOrder.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }else if(szTreeText->CompareTo("δ�ɽ���") == 0)
              {
                  vector<string>::iterator it = g_vecTitleUnTrade.begin();
                  for (;it!=g_vecTitleUnTrade.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }else if(szTreeText->CompareTo("�ɽ���") == 0)
              {
                  vector<string>::iterator it = g_vecTitleTrade.begin();
                  for (;it!=g_vecTitleTrade.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }else if(szTreeText->CompareTo("�ֲ�") == 0)
              {
                  vector<string>::iterator it = g_vecTitlePosition.begin();
                  for (;it!=g_vecTitlePosition.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }else if(szTreeText->CompareTo("ƽ��") == 0)
              {
                  vector<string>::iterator it = g_vecTitleClosePosition.begin();
                  for (;it!=g_vecTitleClosePosition.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }else if(szTreeText->CompareTo("����˵�") == 0)
              {
                  vector<string>::iterator it = g_vecTitleNeedVerify.begin();
                  for (;it!=g_vecTitleNeedVerify.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }*/

              return false;
          }
private: System::Void treeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
     /*        dataGridView->Rows->Clear();
             string szText = Tools::String2string(treeView->SelectedNode->Text);
             int     nTag   = (int)treeView->SelectedNode->Tag;

             map<string,vector<string>>::iterator it = g_mapTreeNode2Column.find(szText);
             if(it!=g_mapTreeNode2Column.end())
             {
                 vector<string> vec = it->second; 
                 for (int i=0;i<(int)vec.size();i++)
                 {
                     string szColumnText = vec[i];
                     array<String^>^ newRow = {Tools::string2String(szColumnText.c_str()),IsChecked(Tools::string2String(szText.c_str()),Tools::string2String(szColumnText.c_str()))?"true":"false"};    
                     dataGridView->Rows->Add(newRow);
                     dataGridView->Rows[i]->Tag = GetTagFromText(Tools::string2String(szColumnText.c_str()));                       
                 }

                     switch(nTag)
                     {
                     case 1://���۱�
                     case 2://��ǰ�����¼�
                     case 3://�˻����ʽ�
                         dataGridView->Rows[0]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         break;
                     case 4://ί�е�
                     case 5://δ�ɽ���
                     case 9://����˵�
                         dataGridView->Rows[0]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         dataGridView->Rows[2]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         dataGridView->Rows[16]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         dataGridView->Rows[17]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         dataGridView->Rows[18]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         dataGridView->Rows[19]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         break;
                     case 6://�ɽ���
                         break;
                     case 7://�ֲ�
                     case 8://ƽ��
                         dataGridView->Rows[0]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         dataGridView->Rows[1]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         dataGridView->Rows[2]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         dataGridView->Rows[3]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         break;
                     }
             }*/
         }
private: System::Void button_OK_Click(System::Object^  sender, System::EventArgs^  e) {
             String^ szTreeText = treeView->SelectedNode->Text;
             String^ szColumnText = String::Empty;
             String^ szColumnIndex = String::Empty;
             vector<string> vecDataGridView2Column;               
             for (int i = 0; i < dataGridView->Rows->Count; i++)
             {
                 if (dataGridView->Rows[i]->Cells[1]->FormattedValue->ToString() == "True")
                 {
                     szColumnText = dataGridView->Rows[i]->Cells[0]->Value->ToString();
                     szColumnIndex = GetTagFromText(szColumnText).ToString();
                     vecDataGridView2Column.push_back(Tools::String2string(szColumnIndex));      
                 }
             }

//              CRiskData::GetInstance()->UpdaeConfigFile(Tools::String2string(szTreeText),vecDataGridView2Column,XML_TYPE_COLUMN);

             this->DialogResult = ::DialogResult::OK;
             this->Close();
         }
private: System::Void button_Cancel_Click(System::Object^  sender, System::EventArgs^  e) {
             this->Close();
         }
private: System::Void button_up_Click(System::Object^  sender, System::EventArgs^  e) {
         }
private: System::Void button_down_Click(System::Object^  sender, System::EventArgs^  e) {
         }
/*������ �����Ժ���*/
private: System::Void dataGridView_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
             e->Effect = DragDropEffects::Move;
         }
private: System::Void dataGridView_CellMouseMove(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if ((e->Clicks < 2) && (e->Button == System::Windows::Forms::Control::MouseButtons/*MouseButtons::Left*/))
             {
                 if ((e->ColumnIndex == -1) && (e->RowIndex > -1))
                     dataGridView->DoDragDrop(dataGridView->Rows[e->RowIndex], DragDropEffects::Move);
             }

         }
private: System::Void dataGridView_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
             int idx = GetRowFromPoint(e->X, e->Y);

             if (idx < 0) return;

             //::System::Type::typeid
//              if (e->Data->GetDataPresent(DataGridViewRow))
//              {
//                  DataGridViewRow^ row = (DataGridViewRow^)e->Data->GetData(::System::Data::DataRowView);//(typeof(DataGridViewRow));
//                  dataGridView->Rows->Remove(row);
//                  m_selectionIdx = idx;
//                  dataGridView->Rows->Insert(idx, row);
//              }

         }
private: int GetRowFromPoint(int x, int y)
         {
             for (int i = 0; i < dataGridView->RowCount; i++)
             {
                 //Rectangle rec = dataGridView->GetRowDisplayRectangle(i, false);

//                  if (dataGridView->RectangleToScreen(rec)->System::Drawing::Rectangle::Contains(x, y))
//                      return i;
             }


             return -1;
         }

private: System::Void dataGridView_RowsAdded(System::Object^  sender, System::Windows::Forms::DataGridViewRowsAddedEventArgs^  e) {
             if (m_selectionIdx > -1)
             {
                 dataGridView->Rows[m_selectionIdx]->Selected = true;
                 dataGridView->CurrentCell = dataGridView->Rows[m_selectionIdx]->Cells[0];
             }
         }
};
}
