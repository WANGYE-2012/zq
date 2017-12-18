#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;

#include <set>
#include <map>
#include <queue>
#include "defineGridCols.h"
#include "CCommonFunc.h"
#include "CFieldDetail.h"
#include "TcpLayer.h"
#include "CDataInfo.h"
#include "RiskManageCmd.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"
#include "CDlgRealTimeTradeInfoData.h"
#include "CInstrumentIDTreeListDlg.h"
#include "CFinancialProductIDListDlg.h"
#include "CAssetMgmtOrganIDTreeListDlg.h"
#include "COrganAndAccountIDTreeListDlg.h"

namespace Client {

	/// <summary>
	/// Summary for CDlgRealTimeTradeInfo1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class CDlgRealTimeTradeInfo : public System::Windows::Forms::Form
	{
	public:
		CDlgRealTimeTradeInfo(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			save = NULL;
			m_pdataInfo = CDataInfo::NewInstance();
			m_bHasExit = false;
			m_bHasChanged_QueryParam = true;
			save = CDlgRealTimeTradeInfoData::NewInstance();
		}
		void ReleaseForm() {
			m_bHasExit = true;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CDlgRealTimeTradeInfo()
		{
			if (components)
			{
				delete components;
			}
			CDlgRealTimeTradeInfoData::ReleaseInstance(save);
			save = NULL;
		}

	private: CDataInfo* m_pdataInfo;
	private: CDlgRealTimeTradeInfoData* save;
	private: bool m_bHasExit;
	private: bool m_bHasChanged_QueryParam;
	private: unsigned long dwTcpStartTime;
	private: unsigned long dwTcpEndTime;
	private: unsigned long dwShowStartTime;
	private: unsigned long dwShowEndTime;

	private: CAssetMgmtOrganIDTreeListDlg^ dlgMgmtOrganIDTreeList;
	private: CFinancialProductIDListDlg^ dlgFinancialProductIDList;
	private: CInstrumentIDTreeListDlg^ dlgInstrumentIDTreeList;
	private: COrganAndAccountIDTreeListDlg^ dlgOrganAndAccountIDTreeList;

	private: System::Windows::Forms::RadioButton^  radioGroupQuery;
	private: System::Windows::Forms::DataGridView^  dataGridTrade;
	private: System::Windows::Forms::Button^  btnResetZero;
	private: System::Windows::Forms::Button^  btnQuery;
	private: AnywndComboBox::AnywndComboBoxControl^  comboOrganization;
	private: AnywndComboBox::AnywndComboBoxControl^  comboFinaProduct;
	private: AnywndComboBox::AnywndComboBoxControl^  comboInstrument;
	private: AnywndComboBox::AnywndComboBoxControl^  comboAccount;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::RadioButton^  radioAccountQuery;

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
			this->radioGroupQuery = (gcnew System::Windows::Forms::RadioButton());
			this->dataGridTrade = (gcnew System::Windows::Forms::DataGridView());
			this->btnResetZero = (gcnew System::Windows::Forms::Button());
			this->btnQuery = (gcnew System::Windows::Forms::Button());
			this->comboOrganization = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->comboFinaProduct = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->comboInstrument = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->comboAccount = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->radioAccountQuery = (gcnew System::Windows::Forms::RadioButton());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridTrade))->BeginInit();
			this->SuspendLayout();
			// 
			// radioGroupQuery
			// 
			this->radioGroupQuery->AutoSize = true;
			this->radioGroupQuery->Location = System::Drawing::Point(7, 12);
			this->radioGroupQuery->Name = L"radioGroupQuery";
			this->radioGroupQuery->Size = System::Drawing::Size(71, 16);
			this->radioGroupQuery->TabIndex = 6;
			this->radioGroupQuery->TabStop = true;
			this->radioGroupQuery->Text = L"��ϲ�ѯ";
			this->radioGroupQuery->UseVisualStyleBackColor = true;
			this->radioGroupQuery->Click += gcnew System::EventHandler(this, &CDlgRealTimeTradeInfo::radioGroupQuery_Click);
			// 
			// dataGridTrade
			// 
			this->dataGridTrade->AllowUserToAddRows = false;
			this->dataGridTrade->AllowUserToDeleteRows = false;
			this->dataGridTrade->AllowUserToResizeRows = false;
			this->dataGridTrade->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridTrade->EditMode = System::Windows::Forms::DataGridViewEditMode::EditProgrammatically;
			this->dataGridTrade->Location = System::Drawing::Point(9, 96);
			this->dataGridTrade->Margin = System::Windows::Forms::Padding(1);
			this->dataGridTrade->MultiSelect = false;
			this->dataGridTrade->Name = L"dataGridTrade";
			this->dataGridTrade->ReadOnly = true;
			this->dataGridTrade->RowHeadersVisible = false;
			this->dataGridTrade->RowTemplate->Height = 23;
			this->dataGridTrade->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridTrade->Size = System::Drawing::Size(613, 307);
			this->dataGridTrade->TabIndex = 17;
			// 
			// btnResetZero
			// 
			this->btnResetZero->Location = System::Drawing::Point(129, 62);
			this->btnResetZero->Name = L"btnResetZero";
			this->btnResetZero->Size = System::Drawing::Size(83, 24);
			this->btnResetZero->TabIndex = 15;
			this->btnResetZero->Text = L"���";
			this->btnResetZero->UseVisualStyleBackColor = true;
			this->btnResetZero->Click += gcnew System::EventHandler(this, &CDlgRealTimeTradeInfo::btnResetZero_Click);
			// 
			// btnQuery
			// 
			this->btnQuery->Location = System::Drawing::Point(26, 62);
			this->btnQuery->Name = L"btnQuery";
			this->btnQuery->Size = System::Drawing::Size(83, 24);
			this->btnQuery->TabIndex = 16;
			this->btnQuery->Text = L"��ѯ";
			this->btnQuery->UseVisualStyleBackColor = true;
			this->btnQuery->Click += gcnew System::EventHandler(this, &CDlgRealTimeTradeInfo::btnQuery_Click);
			// 
			// comboOrganization
			// 
			this->comboOrganization->BackColor = System::Drawing::Color::White;
			this->comboOrganization->ChildControl = nullptr;
			this->comboOrganization->DropDownHeight = 1;
			this->comboOrganization->DropDownWidth = 1;
			this->comboOrganization->FormattingEnabled = true;
			this->comboOrganization->IntegralHeight = false;
			this->comboOrganization->Location = System::Drawing::Point(163, 8);
			this->comboOrganization->MaxDropDownItems = 1;
			this->comboOrganization->Name = L"comboOrganization";
			this->comboOrganization->Size = System::Drawing::Size(100, 20);
			this->comboOrganization->TabIndex = 12;
			// 
			// comboFinaProduct
			// 
			this->comboFinaProduct->BackColor = System::Drawing::Color::White;
			this->comboFinaProduct->ChildControl = nullptr;
			this->comboFinaProduct->DropDownHeight = 1;
			this->comboFinaProduct->DropDownWidth = 1;
			this->comboFinaProduct->FormattingEnabled = true;
			this->comboFinaProduct->IntegralHeight = false;
			this->comboFinaProduct->Location = System::Drawing::Point(336, 8);
			this->comboFinaProduct->MaxDropDownItems = 1;
			this->comboFinaProduct->Name = L"comboFinaProduct";
			this->comboFinaProduct->Size = System::Drawing::Size(100, 20);
			this->comboFinaProduct->TabIndex = 11;
			// 
			// comboInstrument
			// 
			this->comboInstrument->BackColor = System::Drawing::Color::White;
			this->comboInstrument->ChildControl = nullptr;
			this->comboInstrument->DropDownHeight = 1;
			this->comboInstrument->DropDownWidth = 1;
			this->comboInstrument->FormattingEnabled = true;
			this->comboInstrument->IntegralHeight = false;
			this->comboInstrument->Location = System::Drawing::Point(505, 6);
			this->comboInstrument->MaxDropDownItems = 1;
			this->comboInstrument->Name = L"comboInstrument";
			this->comboInstrument->Size = System::Drawing::Size(100, 20);
			this->comboInstrument->TabIndex = 14;
			// 
			// comboAccount
			// 
			this->comboAccount->BackColor = System::Drawing::Color::White;
			this->comboAccount->ChildControl = nullptr;
			this->comboAccount->DropDownHeight = 1;
			this->comboAccount->DropDownWidth = 1;
			this->comboAccount->FormattingEnabled = true;
			this->comboAccount->IntegralHeight = false;
			this->comboAccount->Location = System::Drawing::Point(162, 34);
			this->comboAccount->MaxDropDownItems = 1;
			this->comboAccount->Name = L"comboAccount";
			this->comboAccount->Size = System::Drawing::Size(274, 20);
			this->comboAccount->TabIndex = 13;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(446, 12);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(59, 12);
			this->label3->TabIndex = 10;
			this->label3->Text = L"Ʒ��/��Լ";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(277, 14);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 8;
			this->label2->Text = L"��Ʋ�Ʒ";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(103, 40);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(41, 12);
			this->label4->TabIndex = 9;
			this->label4->Text = L"�˻���";
			// 
			// radioAccountQuery
			// 
			this->radioAccountQuery->AutoSize = true;
			this->radioAccountQuery->Location = System::Drawing::Point(7, 38);
			this->radioAccountQuery->Name = L"radioAccountQuery";
			this->radioAccountQuery->Size = System::Drawing::Size(71, 16);
			this->radioAccountQuery->TabIndex = 5;
			this->radioAccountQuery->TabStop = true;
			this->radioAccountQuery->Text = L"ָ����ѯ";
			this->radioAccountQuery->UseVisualStyleBackColor = true;
			this->radioAccountQuery->Click += gcnew System::EventHandler(this, &CDlgRealTimeTradeInfo::radioAccountQuery_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(104, 14);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 7;
			this->label1->Text = L"��֯�ܹ�";
			// 
			// CDlgRealTimeTradeInfo
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(632, 413);
			this->Controls->Add(this->radioGroupQuery);
			this->Controls->Add(this->dataGridTrade);
			this->Controls->Add(this->btnResetZero);
			this->Controls->Add(this->btnQuery);
			this->Controls->Add(this->comboInstrument);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->comboFinaProduct);
			this->Controls->Add(this->comboAccount);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->comboOrganization);
			this->Controls->Add(this->radioAccountQuery);
			this->Controls->Add(this->label1);
			this->Name = L"CDlgRealTimeTradeInfo";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"ʵʱ�ɽ�";
			this->Load += gcnew System::EventHandler(this, &CDlgRealTimeTradeInfo::CDlgRealTimeTradeInfo_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &CDlgRealTimeTradeInfo::CDlgRealTimeTradeInfo_FormClosed);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &CDlgRealTimeTradeInfo::CDlgRealTimeTradeInfo_FormClosing);
			this->Resize += gcnew System::EventHandler(this, &CDlgRealTimeTradeInfo::CDlgRealTimeTradeInfo_Resize);
			this->ResizeEnd += gcnew System::EventHandler(this, &CDlgRealTimeTradeInfo::CDlgRealTimeTradeInfo_ResizeEnd);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridTrade))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
protected:virtual void WndProc( Message% m ) override {

		if(m.Msg == WM_COMMAND_Win && m.WParam.ToInt32()==WndCmd_YourPkgArrival) {
			char *p = (char*)m.LParam.ToInt32();
			Stru_UniPkgHead head;					
			
			memset(&head, 0, sizeof(head));
			memcpy(&head, p, sizeof(head));
			switch(head.cmdid) {
			case Cmd_RM_SubscribeTrade_Rsp:
			{
				dwTcpEndTime = GetTickCount();

				if(head.userdata1 == OPERATOR_SUCCESS) {
					SequencialTradeField* pTradeInfo = NULL;
					int nItemCount = head.len/sizeof(SequencialTradeField);

					save->LockObject();
					for(int i=0; i<nItemCount; i++) {
						pTradeInfo = (SequencialTradeField*)
								(p+sizeof(Stru_UniPkgHead)+i*sizeof(SequencialTradeField));
						save->queueTradeInfo.push(*pTradeInfo);
					}
					save->UnlockObject();

					if(head.userdata2 == head.userdata3 - 1) {
						dwShowStartTime = GetTickCount();

						std::queue<SequencialTradeField> queueRet;
						save->CheckNewData(m_pdataInfo, queueRet);
						ShowQueue(queueRet);
		
						dwShowEndTime = GetTickCount();
						{
							char csText[1024] = {0};
							sprintf(csText, "ʵʱ�ɽ���ѯ�����β�ѯʱ������ ����==%u, ��ʾ==%u",
									dwTcpEndTime-dwTcpStartTime, dwShowEndTime-dwShowStartTime);
							this->Text = Tools::string2String(csText);
						}
					}
				}
				else {
					p[sizeof(head)+head.len-1]=0;
					//TRACE(p+sizeof(head));
				}
			}
				break;
			case Cmd_RM_UnSubscribeTrade_Rsp:
				if(head.userdata1 == OPERATOR_SUCCESS) {
				}
				else {
					p[sizeof(head)+head.len-1]=0;
					//TRACE(p+sizeof(head));
				}
				break;
			case Cmd_RM_Trade_Push:
			{
				if(head.userdata1 == OPERATOR_SUCCESS) {
					SequencialTradeField* pTradeInfo = NULL;
					int nItemCount = head.len/sizeof(SequencialTradeField);

					save->LockObject();
					for(int i=0; i<nItemCount; i++) {
						pTradeInfo = (SequencialTradeField*)
								(p+sizeof(Stru_UniPkgHead)+i*sizeof(SequencialTradeField));
						save->queueTradeInfo.push(*pTradeInfo);
					}
					save->UnlockObject();

					std::queue<SequencialTradeField> queueRet;
					save->CheckNewData(m_pdataInfo, queueRet);
					ShowQueue(queueRet);

				}
			}
				break;
			};
		}

		Form::WndProc(m);
	}
	int GetRowIndexByRowID(int nRowID) {
		for(int i=0; i<dataGridTrade->Rows->Count; i++) {
			if(int::Parse(dataGridTrade->Rows[i]->Tag->ToString()) == nRowID)
				return dataGridTrade->Rows[i]->Index;
		}
		return -1;
	}
	int GetColIndexByColID(int nColID) {
		for(int i=0; i<dataGridTrade->Columns->Count; i++) {
			if(int::Parse(dataGridTrade->Columns[i]->Tag->ToString()) == nColID)
				return dataGridTrade->Columns[i]->Index;
		}
		return -1;
	}
	void InitGridHead() {

		dataGridTrade->Columns->Clear();

		int nColID = 0;

		System::Windows::Forms::DataGridViewTextBoxColumn^ gridColumn = 
				(gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_InvestorID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"Ͷ���ߴ���";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_InvestorID));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_InstrumentID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"��Լ����";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_InstrumentID));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_Direction;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"��������";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_Direction));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_OffsetFlag;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"��ƽ��־";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_OffsetFlag));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_Price;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"�۸�";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_Price));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_Volume;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"����";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_Volume));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_TradeTime;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"�ɽ�ʱ��";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_TradeTime));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_AssertMgmtOrgan;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"�˺������ʹ�";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_AssertMgmtOrgan));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_FinanProduct;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"�˺�������Ʋ�Ʒ";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_FinanProduct));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_BrokerID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"���͹�˾����";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_BrokerID));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_OrderRef;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"��������";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_OrderRef));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_UserID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"�û�����";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_UserID));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_ExchangeID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"����������";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_ExchangeID));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_TradeID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"�ɽ����";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_TradeID));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_OrderSysID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"�������";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_OrderSysID));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_ParticipantID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"��Ա����";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_ParticipantID));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_ClientID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"�ͻ�����";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_ClientID));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_TradingRole;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"���׽�ɫ";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_TradingRole));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_ExchangeInstID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"��Լ�ڽ������Ĵ���";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_ExchangeInstID));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_HedgeFlag;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"Ͷ���ױ���־";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_HedgeFlag));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_TradeDate;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"�ɽ�ʱ��";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_TradeDate));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_TradeType;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"�ɽ�����";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_TradeType));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_PriceSource;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"�ɽ�����Դ";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_PriceSource));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_TraderID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"����������Ա����";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_TraderID));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_OrderLocalID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"���ر������";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_OrderLocalID));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_ClearingPartID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"�����Ա���";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_ClearingPartID));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_BusinessUnit;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"ҵ��Ԫ";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_BusinessUnit));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_SequenceNo;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"���";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_SequenceNo));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_TradingDay;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"������";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_TradingDay));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_SettlementID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"������";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_SettlementID));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_BrokerOrderSeq;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"���͹�˾�������";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_BrokerOrderSeq));
		dataGridTrade->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_RTQT_TradeSource;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"�ɽ���Դ";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQT_TradeSource));
		dataGridTrade->Columns->Add(gridColumn);
	}
	void UpdateRow(int nIndex, SequencialTradeField& tradeInfo) {
		if(nIndex<0)
			return;
		int nCol = -1;
		std::string strText;
		DataGridViewRow^ dr = dataGridTrade->Rows[nIndex];
		char strTemp[1024];
		if((nCol = GetColIndexByColID(conCol_RTQT_InvestorID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.InvestorID);
		}
		TrustTradeAccount traderAccount;
		memset(&traderAccount, 0, sizeof(traderAccount));
		if(m_pdataInfo->GetTrustTradeAccount(std::string(tradeInfo.InvestorID), traderAccount)) {
			AssetMgmtOrganization organ;
			memset(&organ, 0, sizeof(organ));
			if(m_pdataInfo->GetAssetMgmtOrg(traderAccount.nAssetMgmtOrgID, organ)) {
				if((nCol = GetColIndexByColID(conCol_RTQT_AssertMgmtOrgan))>=0) {
					dr->Cells[nCol]->Value = Tools::string2String(organ.szName);
				}
			}
			FinancialProduct finanProduct;
			memset(&finanProduct, 0, sizeof(finanProduct));
			if(m_pdataInfo->GetFinancialProduct(traderAccount.nFinancialProductID, finanProduct)) {
				if((nCol = GetColIndexByColID(conCol_RTQT_FinanProduct))>=0) {
					dr->Cells[nCol]->Value = Tools::string2String(finanProduct.szName);
				}
			}
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_BrokerID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.BrokerID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_InstrumentID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.InstrumentID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_OrderRef))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.OrderRef);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_UserID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.UserID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_ExchangeID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.ExchangeID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_TradeID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.TradeID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_Direction))>=0) {
			strText = CFieldDetail::Direction2String(tradeInfo.Direction);
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_OrderSysID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.OrderSysID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_ParticipantID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.ParticipantID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_ClientID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.ClientID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_TradingRole))>=0) {
			strText = tradeInfo.TradingRole;
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_ExchangeInstID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.ExchangeInstID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_OffsetFlag))>=0) {
			strText = CFieldDetail::OffsetFlag2String(tradeInfo.OffsetFlag);
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_HedgeFlag))>=0) {
			strText = CFieldDetail::HedgeFlag2String(tradeInfo.HedgeFlag);
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_Price))>=0) {
			sprintf(strTemp, "%0.2f", tradeInfo.Price);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_Volume))>=0) {
			sprintf(strTemp, "%d", tradeInfo.Volume);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_TradeDate))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.TradeDate);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_TradeTime))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.TradeTime);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_TradeType))>=0) {
			strText = CFieldDetail::TradeType2String(tradeInfo.TradeType);
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_PriceSource))>=0) {
			strText = tradeInfo.PriceSource;
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_TraderID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.TraderID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_OrderLocalID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.OrderLocalID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_ClearingPartID))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.ClearingPartID);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_BusinessUnit))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.BusinessUnit);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_SequenceNo))>=0) {
			sprintf(strTemp, "%d", tradeInfo.SequenceNo);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_TradingDay))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(tradeInfo.TradingDay);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_SettlementID))>=0) {
			sprintf(strTemp, "%d", tradeInfo.SettlementID);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_BrokerOrderSeq))>=0) {
			sprintf(strTemp, "%d", tradeInfo.BrokerOrderSeq);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID(conCol_RTQT_TradeSource))>=0) {
			strText = tradeInfo.TradeSource;
			dr->Cells[nCol]->Value = Tools::string2String(strText.c_str());
		}
	}


	void ShowQueue(std::queue<SequencialTradeField>& queueRet) {
		std::map<string, int>::iterator it;
		int nIndex = -1;
		char strTemp[1024];
		std::string strKey;
		strKey.reserve(1024);
		save->LockObject();
		while(!queueRet.empty()) {
			SequencialTradeField field = queueRet.front();
			queueRet.pop();

			memset(strTemp, 0, sizeof(strTemp));
			sprintf(strTemp, "%s, %s, %c", 
				field.ExchangeID, field.TradeID, field.Direction);
			strKey = strTemp;
			it = save->mapKey2Index.find(strKey);
			if(it != save->mapKey2Index.end()) {
				nIndex = GetRowIndexByRowID(it->second);
			}
			else {
				DataGridViewRow^ newRow = gcnew DataGridViewRow;
				int nRowID = save->GetNextRowID();
				newRow->Tag = nRowID;
				nIndex = dataGridTrade->Rows->Add(newRow);
				if(nIndex>=0) {
					save->mapKey2Index.insert(std::make_pair(strKey, nRowID));
					save->mapIndex2Key.insert(std::make_pair(nRowID, strKey));
				}
			}
			if(nIndex>=0) {
				UpdateRow(nIndex, field);
			}
		}
		save->UnlockObject();
	}

	private: System::Void CDlgRealTimeTradeInfo_Load(System::Object^  sender, System::EventArgs^  e) {
		 InitGridHead();

		 dlgFinancialProductIDList = gcnew CFinancialProductIDListDlg();
		 dlgFinancialProductIDList->ResetRelateCombo(comboFinaProduct);
		 comboFinaProduct->ChildControl = dlgFinancialProductIDList;

		 dlgMgmtOrganIDTreeList = gcnew CAssetMgmtOrganIDTreeListDlg();
		 dlgMgmtOrganIDTreeList->ResetFinaProductDlg(dlgFinancialProductIDList);
		 dlgMgmtOrganIDTreeList->ResetRelateCombo(comboOrganization);
		 comboOrganization->ChildControl = dlgMgmtOrganIDTreeList;

		 dlgInstrumentIDTreeList = gcnew CInstrumentIDTreeListDlg();
		 dlgInstrumentIDTreeList->ResetRelateCombo(comboInstrument);
		 comboInstrument->ChildControl = dlgInstrumentIDTreeList;
		 
		 dlgOrganAndAccountIDTreeList = gcnew COrganAndAccountIDTreeListDlg();
		 dlgOrganAndAccountIDTreeList->ResetRelateCombo(comboAccount);
		 comboAccount->ChildControl = dlgOrganAndAccountIDTreeList;

		 IntPtr hWnd=this->Handle;
		 CTcpLayer::SubscribePkg(Cmd_RM_SubscribeTrade_Rsp, (int)hWnd);
		 CTcpLayer::SubscribePkg(Cmd_RM_Trade_Push, (int)hWnd);
		 CTcpLayer::SubscribePkg(Cmd_RM_UnSubscribeTrade_Rsp, (int)hWnd);

		 ResetComboBoxTextReadOnly(comboFinaProduct);
		 ResetComboBoxTextReadOnly(comboOrganization);
		 ResetComboBoxTextReadOnly(comboAccount);
		 ResetComboBoxTextReadOnly(comboInstrument);

		 comboOrganization->Enabled = false;
		 comboFinaProduct->Enabled = false;
		 comboInstrument->Enabled = false;
		 comboAccount->Enabled = true;

		 if(m_pdataInfo!=NULL)
			 btnQuery->Enabled = m_pdataInfo->HasPermission(TODAY_TRADE_QUERY);

	}
private: System::Void CDlgRealTimeTradeInfo_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			if(!m_bHasExit) {
				Hide();
				e->Cancel = true;
			}
			save->ResetQuery();
			dataGridTrade->Rows->Clear();
		 }
private: System::Void CDlgRealTimeTradeInfo_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
		 }
private: System::Void btnQuery_Click(System::Object^  sender, System::EventArgs^  e) {

			 if(!m_bHasChanged_QueryParam)
				 return;

			 bool bHasData = false;
			 dataGridTrade->Rows->Clear();
			 save->ResetQuery();

			 std::set<int> setAccountID;
			 // Ȼ����ݲ�ѯ���������Ҫ���ĵ�AccountID����
			 if(radioGroupQuery->Checked) {

				 bool bNoChecked = true;
				 std::set<int> setOrganID;
				 std::set<int> setFinanProdID;
				 dlgMgmtOrganIDTreeList->GetSelectID(setOrganID, bNoChecked);
				 //if(!bNoChecked) {
					 dlgFinancialProductIDList->GetSelectID(setFinanProdID);
				 //}

				 std::set<int> setTemp;
				 std::set<int> setOrganAccntID;

				 std::set<int>::iterator it;
				 std::set<int>::iterator itTmp;
				 for(it = setOrganID.begin(); it != setOrganID.end(); it++) {
					 setTemp.clear();
					 m_pdataInfo->GetTrustTradeAccountByOrgID(*it, bNoChecked, setTemp);
					 for(itTmp = setTemp.begin(); itTmp != setTemp.end(); itTmp++) {
						 setOrganAccntID.insert(*itTmp);
					 }
				 }

				 std::set<int> setProdAccntID;

				 for(it = setFinanProdID.begin(); it != setFinanProdID.end(); it++) {
					 setTemp.clear();
					 m_pdataInfo->GetTrustTradeAccountByFinancialID(*it, setTemp);
					 for(itTmp = setTemp.begin(); itTmp != setTemp.end(); itTmp++) {
						 setProdAccntID.insert(*itTmp);
					 }
				 }

				 int nMax = max(setOrganAccntID.size(), setProdAccntID.size());
				 int* pTempID = new int[nMax];

				 std::set<int> setResult;
				 int* pEnd = std::set_intersection(
						setOrganAccntID.begin(), setOrganAccntID.end(), 
						setProdAccntID.begin(), setProdAccntID.end(), pTempID);
				 int* nPos = pTempID;
				 while(nPos < pEnd) {
					 setResult.insert(*nPos);
					 nPos++;
				 }
				 delete pTempID;

				 save->LockObject();
				 // ����ѯ���ϱ�������
				 save->setQueryAccountID = setResult;
				 bHasData = setResult.size()>0;

				 // ������µĹ��˺�Լ����
				 std::set<string> setFilterInstrumentID;
				 dlgInstrumentIDTreeList->GetSelectID(setFilterInstrumentID);
				 save->setFilterInstrumentID = setFilterInstrumentID;

				 save->bIsGroupQuery = true;
				 save->UnlockObject();
			 }
			 else {
				 // ����ѯ���ϱ�������
				 save->LockObject();
				 dlgOrganAndAccountIDTreeList->GetSelectID(save->setQueryAccountID);
				 bHasData = save->setQueryAccountID.size()>0;
				 save->bIsGroupQuery = false;
				 save->UnlockObject();
			 }

			 dwTcpStartTime = GetTickCount();
			 dwTcpEndTime = 0;
			 dwShowStartTime = 0;
			 dwShowEndTime = 0;

			 if(bHasData)
				 save->NewQuery();
			 else
                 MessageBox::Show("�����ѯ����δɸѡ���κ��ʺš�", 
			 				"��ʾ", MessageBoxButtons::OK, MessageBoxIcon::Warning);

			 //m_bHasChanged_QueryParam = false;

		 }
private: System::Void btnResetZero_Click(System::Object^  sender, System::EventArgs^  e) {
			 m_bHasChanged_QueryParam = true;
			 //dlgMgmtOrganIDTreeList->Clear();
			 //dlgFinancialProductIDList->Clear();
			 //dlgInstrumentIDTreeList->Clear();
			 //dlgOrganAndAccountIDTreeList->Clear();

			 dataGridTrade->Rows->Clear();
			 save->ResetQuery();
		 }
private: System::Void radioGroupQuery_Click(System::Object^  sender, System::EventArgs^  e) {
			 comboOrganization->Enabled = true;
			 comboFinaProduct->Enabled = true;
			 comboInstrument->Enabled = true;
			 comboAccount->Enabled = false;
		 }
private: System::Void radioAccountQuery_Click(System::Object^  sender, System::EventArgs^  e) {
			 comboOrganization->Enabled = false;
			 comboFinaProduct->Enabled = false;
			 comboInstrument->Enabled = false;
			 comboAccount->Enabled = true;
		 }
private: System::Void CDlgRealTimeTradeInfo_ResizeEnd(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void CDlgRealTimeTradeInfo_Resize(System::Object^  sender, System::EventArgs^  e) {
			 Rectangle rect = this->ClientRectangle;
			 Rectangle rectGrid = dataGridTrade->DisplayRectangle;
			 System::Drawing::Size size(rect.Width-dataGridTrade->Left-10, rect.Height-dataGridTrade->Top-10);
			 dataGridTrade->Size = size;
		 }
};
}
