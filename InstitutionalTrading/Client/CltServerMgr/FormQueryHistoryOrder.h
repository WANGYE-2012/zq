#pragma once

#include "TcpLayer.h"
#include "defineGridCols.h"
#include "CCommonFunc.h"
#include "CDataInfo.h"
#include "FormQueryHistoryOrderData.h"
#include "PanelOrganDealerTree.h"
#include "FieldDetail.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltServerMgr {

	/// <summary>
	/// Summary for FormQueryHistoryOrder
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormQueryHistoryOrder : public System::Windows::Forms::Form
	{
	public:
		FormQueryHistoryOrder(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			save = NULL;
			m_pdataInfo = CDataInfo::NewInstance();
			m_bQuerying = false;
			m_bHasExit = false;
			m_bHasChanged_QueryParam = true;
			save = FormQueryHistoryOrderData::NewInstance();
			dTableOrder = gcnew DataTable();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormQueryHistoryOrder()
		{
			if (components)
			{
				delete components;
			}
		}
	private: CDataInfo* m_pdataInfo;
	private: FormQueryHistoryOrderData* save;
	private: bool m_bQuerying;
	private: bool m_bHasExit;
	private: bool m_bHasChanged_QueryParam;
	private: unsigned long dwTcpStartTime;
	private: unsigned long dwTcpEndTime;
	private: unsigned long dwShowStartTime;
	private: unsigned long dwShowEndTime;
	private: DataTable ^ dTableOrder;

	private: PanelOrganDealerTree^  panelOrganDealerTreeList;

	protected: 
	private: System::Windows::Forms::Button^  btnResetZero;
	private: System::Windows::Forms::Button^  btnQuery;
	private: System::Windows::Forms::DateTimePicker^  dtpEnd;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::DateTimePicker^  dtpStart;
	private: System::Windows::Forms::Label^  label1;
	private: AnywndComboBox::AnywndComboBoxControl^  comboAccount;
	private: System::Windows::Forms::Label^  label4;
	private: SimpleReportControl::SimpleReportControlControl^  viewGridOrder;
	private: System::Windows::Forms::Timer^  timerTimeout;
	private: System::Windows::Forms::ProgressBar^  progQueryBar;

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
			this->btnResetZero = (gcnew System::Windows::Forms::Button());
			this->btnQuery = (gcnew System::Windows::Forms::Button());
			this->dtpEnd = (gcnew System::Windows::Forms::DateTimePicker());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->dtpStart = (gcnew System::Windows::Forms::DateTimePicker());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->timerTimeout = (gcnew System::Windows::Forms::Timer(this->components));
			this->progQueryBar = (gcnew System::Windows::Forms::ProgressBar());
			this->comboAccount = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->viewGridOrder = (gcnew SimpleReportControl::SimpleReportControlControl());
			this->SuspendLayout();
			// 
			// btnResetZero
			// 
			this->btnResetZero->Location = System::Drawing::Point(273, 58);
			this->btnResetZero->Name = L"btnResetZero";
			this->btnResetZero->Size = System::Drawing::Size(83, 24);
			this->btnResetZero->TabIndex = 45;
			this->btnResetZero->Text = L"���";
			this->btnResetZero->UseVisualStyleBackColor = true;
			this->btnResetZero->Click += gcnew System::EventHandler(this, &FormQueryHistoryOrder::btnResetZero_Click);
			// 
			// btnQuery
			// 
			this->btnQuery->Location = System::Drawing::Point(68, 58);
			this->btnQuery->Name = L"btnQuery";
			this->btnQuery->Size = System::Drawing::Size(83, 24);
			this->btnQuery->TabIndex = 46;
			this->btnQuery->Text = L"��ѯ";
			this->btnQuery->UseVisualStyleBackColor = true;
			this->btnQuery->Click += gcnew System::EventHandler(this, &FormQueryHistoryOrder::btnQuery_Click);
			// 
			// dtpEnd
			// 
			this->dtpEnd->CustomFormat = L"yyyy-MM-dd";
			this->dtpEnd->Location = System::Drawing::Point(246, 31);
			this->dtpEnd->Name = L"dtpEnd";
			this->dtpEnd->Size = System::Drawing::Size(110, 21);
			this->dtpEnd->TabIndex = 44;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(186, 36);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 41;
			this->label2->Text = L"��������";
			// 
			// dtpStart
			// 
			this->dtpStart->CustomFormat = L"yyyy-MM-dd";
			this->dtpStart->Location = System::Drawing::Point(68, 31);
			this->dtpStart->Name = L"dtpStart";
			this->dtpStart->Size = System::Drawing::Size(110, 21);
			this->dtpStart->TabIndex = 43;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(9, 36);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 42;
			this->label1->Text = L"��ʼ����";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(9, 12);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(41, 12);
			this->label4->TabIndex = 39;
			this->label4->Text = L"�˻���";
			// 
			// timerTimeout
			// 
			this->timerTimeout->Interval = 3000;
			this->timerTimeout->Tick += gcnew System::EventHandler(this, &FormQueryHistoryOrder::timerTimeout_Tick);
			// 
			// progQueryBar
			// 
			this->progQueryBar->Location = System::Drawing::Point(157, 65);
			this->progQueryBar->Name = L"progQueryBar";
			this->progQueryBar->Size = System::Drawing::Size(109, 10);
			this->progQueryBar->TabIndex = 22;
			// 
			// comboAccount
			// 
			this->comboAccount->AllowDrop = true;
			this->comboAccount->BackColor = System::Drawing::Color::White;
			this->comboAccount->ChildControl = nullptr;
			this->comboAccount->DropDownHeight = 1;
			this->comboAccount->DropDownWidth = 1;
			this->comboAccount->FormattingEnabled = true;
			this->comboAccount->IntegralHeight = false;
			this->comboAccount->Location = System::Drawing::Point(68, 7);
			this->comboAccount->MaxDropDownItems = 1;
			this->comboAccount->Name = L"comboAccount";
			this->comboAccount->Size = System::Drawing::Size(288, 20);
			this->comboAccount->TabIndex = 40;
			// 
			// viewGridOrder
			// 
			this->viewGridOrder->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->viewGridOrder->ConfigGuid = L"{EBF1E46D-9C72-4eb7-AC93-C476D6BA7AA0}";
			this->viewGridOrder->Location = System::Drawing::Point(8, 86);
			this->viewGridOrder->Name = L"viewGridOrder";
			this->viewGridOrder->RealDataTable = nullptr;
			this->viewGridOrder->SimpleMode = true;
			this->viewGridOrder->Size = System::Drawing::Size(795, 435);
			this->viewGridOrder->StatisticDataTable = nullptr;
			this->viewGridOrder->TabIndex = 48;
			// 
			// FormQueryHistoryOrder
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(813, 531);
			this->Controls->Add(this->progQueryBar);
			this->Controls->Add(this->viewGridOrder);
			this->Controls->Add(this->btnResetZero);
			this->Controls->Add(this->btnQuery);
			this->Controls->Add(this->dtpEnd);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->dtpStart);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->comboAccount);
			this->Controls->Add(this->label4);
			this->Name = L"FormQueryHistoryOrder";
			this->Text = L"��ѯ������ʷ����";
			this->Load += gcnew System::EventHandler(this, &FormQueryHistoryOrder::FormQueryHistoryOrder_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FormQueryHistoryOrder::FormQueryHistoryOrder_FormClosed);
			this->Resize += gcnew System::EventHandler(this, &FormQueryHistoryOrder::FormQueryHistoryOrder_Resize);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	protected:virtual void WndProc( Message% m ) override {

		if(m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival) {
			char *p = (char*)m.LParam.ToInt32();
			Stru_UniPkgHead head;
			
			memset(&head, 0, sizeof(head));
			memcpy(&head, p, sizeof(head));
			switch(head.cmdid) {
			case Cmd_RM_QryHistoryOrders_Rsp:
			{
				if(head.userdata1 == CF_ERROR_SUCCESS) {
					PlatformStru_OrderInfo* pOrder = NULL;
					int nItemCount = head.len/sizeof(PlatformStru_OrderInfo);

					save->LockObject();
					for(int i=0; i<nItemCount; i++) {
						pOrder = (PlatformStru_OrderInfo*)
								(p+sizeof(Stru_UniPkgHead)+i*sizeof(PlatformStru_OrderInfo));

						// ����Ӧ�ò���Ҫ��һ�����Ϊ���ؼ�Ϊ��Ҫ������
						std::set<std::string>::iterator it = 
								save->setQueryAccountID.find(pOrder->InvestorID);
						if( it== save->setQueryAccountID.end())//�ж��ǲ������ҳ���ѯ�ģ������򷵻�
							continue;

						save->queueOrderInfo.push(*pOrder);
					}
					save->UnlockObject();
					
					progQueryBar->Value = progQueryBar->Value+1;

					if(progQueryBar->Value==progQueryBar->Maximum) {
						m_bQuerying = false;
						this->btnQuery->Text = gcnew String("��ѯ");
						save->LockObject();
						if(!save->queueOrderInfo.empty()) {
							ShowQueue(save->queueOrderInfo);
						}
						else {
							_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
									CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, "��ѯ��ʷ�����������ݷ��ء�");
						}
						save->UnlockObject();
					}
					else
						save->NextQuery();


					//if(save->queueOrderInfo.size() == 0) {
					//	_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
					//			CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
					//			"��ʷ������ѯ�����β�ѯ�����ݷ��ء�");
					//	return;
					//}
					//
					////std::queue<PlatformStru_OrderInfo> queueRet;
					////save->CheckNewData(m_pdataInfo, queueRet);
					//ShowQueue(save->queueOrderInfo);

				}
				else {
					_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
							CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, p+sizeof(Stru_UniPkgHead));
				}
			}
				break;
			};
		}

		Form::WndProc(m);
	}

	int GetColIndex(string& strColName) {
#if 1
		for(int i=0; i<dTableOrder->Columns->Count; i++) {
			if(dTableOrder->Columns[i]->Caption == Tools::string2String(strColName.c_str()))
				return dTableOrder->Columns->IndexOf(dTableOrder->Columns[i]->ColumnName);
		}
		return -1;
#else
		return dTableOrder->Columns->IndexOf(Tools::string2String(strColName.c_str()));
#endif
	}

	void InitGridHead() {

		dTableOrder->Columns->Clear();

		int nColID = 0;
		System::Data::DataColumn^ gridColumn;

		gridColumn = dTableOrder->Columns->Add(L"������", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_TradingDay));

		gridColumn = dTableOrder->Columns->Add(L"�˻���", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_InvestorID));

		//gridColumn = dTableOrder->Columns->Add(L"�˻�����", Type::GetType("System.String"));
		//gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_InvestorName));

		gridColumn = dTableOrder->Columns->Add(L"��Լ����", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_InstrumentID));

		gridColumn = dTableOrder->Columns->Add(L"��������", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_Direction));

		gridColumn = dTableOrder->Columns->Add(L"��ƽ��־", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_CombOffsetFlag));

		gridColumn = dTableOrder->Columns->Add(L"Ͷ���ױ���־", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_CombHedgeFlag));

		gridColumn = dTableOrder->Columns->Add(L"��������", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_OrderRef));

		gridColumn = dTableOrder->Columns->Add(L"�����۸�����", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_OrderPriceType));

		gridColumn = dTableOrder->Columns->Add(L"�۸�", Type::GetType("System.Double"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_LimitPrice));

		gridColumn = dTableOrder->Columns->Add(L"����", Type::GetType("System.Int32"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_VolumeTotalOriginal));

		gridColumn = dTableOrder->Columns->Add(L"��Ч������", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_TimeCondition));

		gridColumn = dTableOrder->Columns->Add(L"GTD����", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_GTDDate));

		gridColumn = dTableOrder->Columns->Add(L"�������", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_OrderSysID));

		gridColumn = dTableOrder->Columns->Add(L"�ɽ�������", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_VolumeCondition));

		gridColumn = dTableOrder->Columns->Add(L"��С�ɽ���", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_MinVolume));

		gridColumn = dTableOrder->Columns->Add(L"��������", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_ContingentCondition));

		gridColumn = dTableOrder->Columns->Add(L"ֹ���", Type::GetType("System.Double"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_StopPrice));

		gridColumn = dTableOrder->Columns->Add(L"ǿƽԭ��", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_ForceCloseReason));

		gridColumn = dTableOrder->Columns->Add(L"�Զ������־", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_IsAutoSuspend));

		gridColumn = dTableOrder->Columns->Add(L"ҵ��Ԫ", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_BusinessUnit));

		gridColumn = dTableOrder->Columns->Add(L"������", Type::GetType("System.Int32"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_RequestID));

		gridColumn = dTableOrder->Columns->Add(L"���ر������", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_OrderLocalID));

		gridColumn = dTableOrder->Columns->Add(L"����������", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_ExchangeID));

		gridColumn = dTableOrder->Columns->Add(L"��Ա����", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_ParticipantID));

		gridColumn = dTableOrder->Columns->Add(L"�ͻ�����", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_ClientID));

		gridColumn = dTableOrder->Columns->Add(L"��Լ�ڽ������Ĵ���", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_ExchangeInstID));

		gridColumn = dTableOrder->Columns->Add(L"����������Ա����", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_TraderID));

		gridColumn = dTableOrder->Columns->Add(L"��װ���", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_InstallID));

		gridColumn = dTableOrder->Columns->Add(L"�����ύ״̬", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_OrderSubmitStatus));

		gridColumn = dTableOrder->Columns->Add(L"������ʾ���", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_NotifySequence));

		gridColumn = dTableOrder->Columns->Add(L"������Դ", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_OrderSource));

		gridColumn = dTableOrder->Columns->Add(L"����״̬", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_OrderStatus));

		gridColumn = dTableOrder->Columns->Add(L"��������", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_OrderType));

		gridColumn = dTableOrder->Columns->Add(L"��ɽ�����", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_VolumeTraded));

		gridColumn = dTableOrder->Columns->Add(L"ʣ������", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_VolumeTotal));

		gridColumn = dTableOrder->Columns->Add(L"��������", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_InsertDate));

		gridColumn = dTableOrder->Columns->Add(L"ί��ʱ��", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_InsertTime));

		gridColumn = dTableOrder->Columns->Add(L"����ʱ��", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_ActiveTime));

		gridColumn = dTableOrder->Columns->Add(L"����ʱ��", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_SuspendTime));

		gridColumn = dTableOrder->Columns->Add(L"����޸�ʱ��", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_UpdateTime));

		gridColumn = dTableOrder->Columns->Add(L"����ʱ��", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_CancelTime));

		gridColumn = dTableOrder->Columns->Add(L"����޸Ľ���������Ա����", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_ActiveTraderID));

		gridColumn = dTableOrder->Columns->Add(L"���", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_SequenceNo));

		gridColumn = dTableOrder->Columns->Add(L"ǰ�ñ��", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_FrontID));

		gridColumn = dTableOrder->Columns->Add(L"�Ự���", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_SessionID));

		gridColumn = dTableOrder->Columns->Add(L"�û��˲�Ʒ��Ϣ", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_UserProductInfo));

		gridColumn = dTableOrder->Columns->Add(L"״̬��Ϣ", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_StatusMsg));

		gridColumn = dTableOrder->Columns->Add(L"�û�ǿ����־", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_UserForceClose));

		gridColumn = dTableOrder->Columns->Add(L"�����û�����", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_ActiveUserID));

		gridColumn = dTableOrder->Columns->Add(L"���͹�˾�������", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_BrokerOrderSeq));

		gridColumn = dTableOrder->Columns->Add(L"��ر���", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQO_RelativeOrderSysID));

	}

	void UpdateRow(DataRow^ row, PlatformStru_OrderInfo& order) {

		if(row == nullptr)
			return;
		int nCol = -1;
		char strTemp[1024];

		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_TradingDay))))>=0) {
			row[nCol] = Tools::string2String(order.TradingDay);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_InvestorID))))>=0) {
			row[nCol] = Tools::string2String(order.InvestorID);
		}
		//if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_InvestorName))))>=0) {
		//	row[nCol] = Tools::string2String(order.Account);
		//}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_InstrumentID))))>=0) {
			row[nCol] = Tools::string2String(order.InstrumentID);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_OrderRef))))>=0) {
			row[nCol] = Tools::string2String(order.OrderRef);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_OrderPriceType))))>=0) {
			sprintf(strTemp, "%s", CFieldDetail::OrderPriceType2String(order.OrderPriceType));
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_Direction))))>=0) {
			sprintf(strTemp, "%s", CFieldDetail::Direction2String(order.Direction));
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_CombOffsetFlag))))>=0) {
			sprintf(strTemp, "%s", CFieldDetail::OffsetFlag2String(order.CombOffsetFlag[0]));
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_CombHedgeFlag))))>=0) {
			sprintf(strTemp, "%s", CFieldDetail::HedgeFlag2String(order.CombHedgeFlag[0]));
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_LimitPrice))))>=0) {
			sprintf(strTemp, "%0.3f", order.LimitPrice);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_VolumeTotalOriginal))))>=0) {
			sprintf(strTemp, "%d", order.VolumeTotalOriginal);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_TimeCondition))))>=0) {
			sprintf(strTemp, "%s", CFieldDetail::TimeCondition2String(order.TimeCondition));
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_GTDDate))))>=0) {
			row[nCol] = Tools::string2String(order.GTDDate);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_VolumeCondition))))>=0) {
			sprintf(strTemp, "%s", CFieldDetail::VolumeCondition2String(order.VolumeCondition));
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_MinVolume))))>=0) {
			sprintf(strTemp, "%d", order.MinVolume);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_ContingentCondition))))>=0) {
			sprintf(strTemp, "%c", order.ContingentCondition);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_StopPrice))))>=0) {
			sprintf(strTemp, "%0.3f", order.StopPrice);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_ForceCloseReason))))>=0) {
			sprintf(strTemp, "%s", CFieldDetail::ForceCloseReason2String(order.ForceCloseReason));
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_IsAutoSuspend))))>=0) {
			sprintf(strTemp, "%d", order.IsAutoSuspend);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_BusinessUnit))))>=0) {
			row[nCol] = Tools::string2String(order.BusinessUnit);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_RequestID))))>=0) {
			sprintf(strTemp, "%d", order.RequestID);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_OrderLocalID))))>=0) {
			row[nCol] = Tools::string2String(order.OrderLocalID);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_ExchangeID))))>=0) {
			row[nCol] = Tools::string2String(order.ExchangeID);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_ParticipantID))))>=0) {
			row[nCol] = Tools::string2String(order.ParticipantID);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_ClientID))))>=0) {
			row[nCol] = Tools::string2String(order.ClientID);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_ExchangeInstID))))>=0) {
			row[nCol] = Tools::string2String(order.ExchangeInstID);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_TraderID))))>=0) {
			row[nCol] = Tools::string2String(order.TraderID);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_InstallID))))>=0) {
			sprintf(strTemp, "%d", order.InstallID);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_OrderSubmitStatus))))>=0) {
			sprintf(strTemp, "%s", CFieldDetail::OrderSubmitStatus2String(order.OrderSubmitStatus));
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_NotifySequence))))>=0) {
			sprintf(strTemp, "%d", order.NotifySequence);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_OrderSysID))))>=0) {
			row[nCol] = Tools::string2String(order.OrderSysID);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_OrderSource))))>=0) {
			sprintf(strTemp, "%c", order.OrderSource);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_OrderStatus))))>=0) {
			sprintf(strTemp, "%s", CFieldDetail::OrderStatus2String(order.OrderStatus));
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_OrderType))))>=0) {
			sprintf(strTemp, "%s", CFieldDetail::OrderType2String(order.OrderType));
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_VolumeTraded))))>=0) {
			sprintf(strTemp, "%d", order.VolumeTraded);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_VolumeTotal))))>=0) {
			sprintf(strTemp, "%d", order.VolumeTotal);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_InsertDate))))>=0) {
			row[nCol] = Tools::string2String(order.InsertDate);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_InsertTime))))>=0) {
			row[nCol] = Tools::string2String(order.InsertTime);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_ActiveTime))))>=0) {
			row[nCol] = Tools::string2String(order.ActiveTime);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_SuspendTime))))>=0) {
			row[nCol] = Tools::string2String(order.SuspendTime);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_UpdateTime))))>=0) {
			row[nCol] = Tools::string2String(order.UpdateTime);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_CancelTime))))>=0) {
			row[nCol] = Tools::string2String(order.CancelTime);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_ActiveTraderID))))>=0) {
			row[nCol] = Tools::string2String(order.ActiveTraderID);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_SequenceNo))))>=0) {
			sprintf(strTemp, "%d", order.SequenceNo);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_FrontID))))>=0) {
			sprintf(strTemp, "%d", order.FrontID);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_SessionID))))>=0) {
			sprintf(strTemp, "%x", order.SessionID);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_UserProductInfo))))>=0) {
			row[nCol] = Tools::string2String(order.UserProductInfo);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_StatusMsg))))>=0) {
			row[nCol] = Tools::string2String(order.StatusMsg);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_UserForceClose))))>=0) {
			sprintf(strTemp, "%s", CFieldDetail::UserForceClose2String(order.UserForceClose));
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_ActiveUserID))))>=0) {
			sprintf(strTemp, "%s", order.ActiveUserID);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_BrokerOrderSeq))))>=0) {
			sprintf(strTemp, "%d", order.BrokerOrderSeq);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQO_RelativeOrderSysID))))>=0) {
			sprintf(strTemp, "%s", order.RelativeOrderSysID);
			row[nCol] = Tools::string2String(strTemp);
		}
	}

	void ShowQueue(std::queue<PlatformStru_OrderInfo>& queueRet) {

		save->LockObject();
		
		while(!queueRet.empty()) {
			PlatformStru_OrderInfo field = queueRet.front();
			queueRet.pop();

			DataRow^ newRow = dTableOrder->NewRow();
			UpdateRow(newRow, field);
			dTableOrder->Rows->Add(newRow);
		}
		save->UnlockObject();

		viewGridOrder->Refresh2();
	}

	private: System::Void FormQueryHistoryOrder_Load(System::Object^  sender, System::EventArgs^  e) {
				 InitGridHead();
				 panelOrganDealerTreeList = gcnew PanelOrganDealerTree();
				 panelOrganDealerTreeList->ResetRelateCombo(comboAccount);
				 comboAccount->ChildControl = panelOrganDealerTreeList;

				 IntPtr hWnd=this->Handle;
				 CTcpLayer::SubscribePkg(Cmd_RM_QryHistoryOrders_Rsp, (int)hWnd);

				 ResetComboBoxTextReadOnly(comboAccount);
				 comboAccount->Enabled = true;

				 viewGridOrder->RealDataTable = dTableOrder;
			 }
	private: System::Void FormQueryHistoryOrder_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
				 IntPtr hWnd=this->Handle;
				 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
			 }
	private: System::Void FormQueryHistoryOrder_Resize(System::Object^  sender, System::EventArgs^  e) {
				 System::Drawing::Rectangle rect = this->ClientRectangle;
				 System::Drawing::Rectangle rectGrid = viewGridOrder->DisplayRectangle;
				 System::Drawing::Size size(rect.Width-viewGridOrder->Left-10, rect.Height-viewGridOrder->Top-10);
				 viewGridOrder->Size = size;
			 }
	private: System::Void btnQuery_Click(System::Object^  sender, System::EventArgs^  e) {

				 if(m_bQuerying) {
					if(MessageBox::Show("�Ƿ���ֹ���β�ѯ��", 
							"����", MessageBoxButtons::YesNo, MessageBoxIcon::Warning)
							==::DialogResult::Yes) {
						 save->StopQuery();
						 m_bQuerying = false;
						 progQueryBar->Value = 0;
						 this->btnQuery->Text = gcnew String("��ѯ");
						 if(MessageBox::Show("�Ƿ���ʾδ��ɵĲ�ѯ�����", 
								"����", MessageBoxButtons::YesNo, MessageBoxIcon::Warning)
								==::DialogResult::Yes) {
							save->LockObject();
							if(!save->queueOrderInfo.empty()) {
								ShowQueue(save->queueOrderInfo);
							}
							else {
								_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
										CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, "��ѯ��ʷ�����������ݷ��ء�");
							}
							save->UnlockObject();
						}
					}
					 return;
				 }
				 
				 if(!m_bHasChanged_QueryParam)
					 return;

				 bool bHasData = false;
				 viewGridOrder->Clear();
				 dTableOrder->Rows->Clear();

				 save->ResetQuery();
				 progQueryBar->Value = 0;

				 std::string strStartDate;
				 std::string strEndDate;
				 std::set<std::string> setAccountID;
				 // Ȼ����ݲ�ѯ���������Ҫ���ĵ�AccountID����

				 strStartDate = Tools::String2string(dtpStart->Value.Date.ToString("yyyyMMdd"));
				 strEndDate = Tools::String2string(dtpEnd->Value.Date.ToString("yyyyMMdd"));
				 if(atoi(strEndDate.c_str()) < atoi(strStartDate.c_str())) {
					 MessageBox::Show("��ʼ���ڱ���С�ڵ��ڽ������ڡ�");
					 return;
				 }

				 strStartDate = Tools::String2string(dtpStart->Value.Date.ToString("yyyy-MM-dd"));
				 strEndDate = Tools::String2string(dtpEnd->Value.Date.ToString("yyyy-MM-dd"));

				 // ����ѯ���ϱ�������
				 save->LockObject();
				 bHasData = panelOrganDealerTreeList->GetSelectID(save->setQueryAccountID);
				 save->UnlockObject();

				 dwTcpStartTime = GetTickCount();
				 dwTcpEndTime = 0;
				 dwShowStartTime = 0;
				 dwShowEndTime = 0;

				 if(bHasData) {
					 int nCount = 0;
					 sLoginRsp loginRsp;
					 memset(&loginRsp, 0, sizeof(loginRsp));
					 m_pdataInfo->GetLogonInfo(loginRsp);
					 save->NewQuery(strStartDate, strEndDate, loginRsp.mnUserID, nCount);
					 progQueryBar->Minimum = 0;
					 progQueryBar->Maximum = nCount;
					 //this->btnQuery->Enabled = false;
					 //this->timerTimeout->Start();
					 this->btnQuery->Text = gcnew String("ֹͣ");
					 m_bQuerying = true;
				 }
				 else
					 MessageBox::Show("�����ѯ����δɸѡ���κ��ʺš�", 
							"��ʾ", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			 }
	private: System::Void btnResetZero_Click(System::Object^  sender, System::EventArgs^  e) {
				 m_bHasChanged_QueryParam = true;
				 panelOrganDealerTreeList->Clear();
				 if(!m_bHasChanged_QueryParam)
					 return;

				 viewGridOrder->Clear();
				 dTableOrder->Rows->Clear();
				 save->ResetQuery();
			 }
private: System::Void timerTimeout_Tick(System::Object^  sender, System::EventArgs^  e) {
			 this->btnQuery->Enabled = true;
			 this->timerTimeout->Stop();
		 }
};
}
