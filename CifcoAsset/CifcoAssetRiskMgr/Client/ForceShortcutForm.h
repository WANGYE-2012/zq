#pragma once

#include "ForceShortcutForm2.h"
#include "RiskManageCmd.h"
#include "RiskManageStruct.h"
#include "CommonPkg.h"
#include "TcpLayer.h"
#include "md5.h"
#include "CDataInfo.h"
#include "Tools.h"
#include "ForceCloseInfoData.h"
#include "RiskEvt.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Client {

	/// <summary>
	/// Summary for ForceShortcutForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ForceShortcutForm : public System::Windows::Forms::Form
	{
	public:
		ForceShortcutForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_strInstrumentID = nullptr;
            m_pTrustTradeAccount = NULL;
            m_pDepthMarketDataField = NULL;
            m_pFundInfo = NULL;
            m_pSequencialPositionField = NULL;
            m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ForceShortcutForm()
		{
			if (components)
			{
				delete components;
			}

            if (m_pDataInfo != NULL)
            {
                CDataInfo::DestroyInstance();
            }            
		}
    private: System::Windows::Forms::Label^  label1;
    protected: 
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::Label^  label4;
    private: System::Windows::Forms::Label^  label5;
    private: System::Windows::Forms::Label^  label6;
    private: System::Windows::Forms::Label^  label7;
    private: System::Windows::Forms::Label^  label8;
    private: System::Windows::Forms::Label^  label9;
    private: System::Windows::Forms::Label^  label10;
    private: System::Windows::Forms::Label^  label11;
    private: System::Windows::Forms::Label^  label12;
    private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::TextBox^  RiskEvent_textBox;
	private: System::Windows::Forms::TextBox^  Account_textBox;
	private: System::Windows::Forms::TextBox^  Name_textBox;
	private: System::Windows::Forms::TextBox^  Contract_textBox;
	private: System::Windows::Forms::TextBox^  SpeculateHedge_textBox;
	private: System::Windows::Forms::TextBox^  Business_textBox;
    private: System::Windows::Forms::TextBox^  OrderCount_textBox;
    private: System::Windows::Forms::TextBox^  OrderPrice_textBox;
    private: System::Windows::Forms::TextBox^  PecentLimit_textBox;
    private: System::Windows::Forms::TextBox^  CloseVol_textBox;
    private: System::Windows::Forms::TextBox^  TodayCloseVol_textBox;
    private: System::Windows::Forms::TextBox^  FundsAvailable_textBox;
	private: System::Windows::Forms::ComboBox^  FlatStyle_comboBox;
    private: System::Windows::Forms::CheckBox^  Force_checkBox;
	private: System::Windows::Forms::Button^  OkButton;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

        CDataInfo* m_pDataInfo;
        TrustTradeAccount* m_pTrustTradeAccount;
        DepthMarketDataField* m_pDepthMarketDataField;
        RiskAllAccountField* m_pFundInfo;
        SequencialPositionField* m_pSequencialPositionField;//ǿƽ�ĳֲ���Ϣ
        
        
        String^ m_strRiskEventID;
        String^ m_strAccount;
        String^ m_strName;
        String^ m_strInstrumentID;
        String^ m_strSpeculateHedge;
        String^ m_strDirection;
        String^ m_strFlatStyle;
        BOOL    m_bForce;
        String^ m_strCount;
        String^ m_strPrice;
        String^ m_strPecentPerLimit;//�ǵ�
        String^ m_strCloseVolume;
        String^ m_strTodayCloseVol;
        String^ m_strFundsAvailable;
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->label6 = (gcnew System::Windows::Forms::Label());
            this->label7 = (gcnew System::Windows::Forms::Label());
            this->label8 = (gcnew System::Windows::Forms::Label());
            this->label9 = (gcnew System::Windows::Forms::Label());
            this->label10 = (gcnew System::Windows::Forms::Label());
            this->label11 = (gcnew System::Windows::Forms::Label());
            this->label12 = (gcnew System::Windows::Forms::Label());
            this->label13 = (gcnew System::Windows::Forms::Label());
            this->RiskEvent_textBox = (gcnew System::Windows::Forms::TextBox());
            this->Account_textBox = (gcnew System::Windows::Forms::TextBox());
            this->Name_textBox = (gcnew System::Windows::Forms::TextBox());
            this->Contract_textBox = (gcnew System::Windows::Forms::TextBox());
            this->SpeculateHedge_textBox = (gcnew System::Windows::Forms::TextBox());
            this->Business_textBox = (gcnew System::Windows::Forms::TextBox());
            this->OrderCount_textBox = (gcnew System::Windows::Forms::TextBox());
            this->OrderPrice_textBox = (gcnew System::Windows::Forms::TextBox());
            this->PecentLimit_textBox = (gcnew System::Windows::Forms::TextBox());
            this->CloseVol_textBox = (gcnew System::Windows::Forms::TextBox());
            this->TodayCloseVol_textBox = (gcnew System::Windows::Forms::TextBox());
            this->FundsAvailable_textBox = (gcnew System::Windows::Forms::TextBox());
            this->FlatStyle_comboBox = (gcnew System::Windows::Forms::ComboBox());
            this->Force_checkBox = (gcnew System::Windows::Forms::CheckBox());
            this->OkButton = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(6, 11);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(77, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"�����¼�ID��";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(42, 38);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(41, 12);
            this->label2->TabIndex = 0;
            this->label2->Text = L"�ʺţ�";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(42, 65);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(41, 12);
            this->label3->TabIndex = 0;
            this->label3->Text = L"������";
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->Location = System::Drawing::Point(42, 92);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(41, 12);
            this->label4->TabIndex = 0;
            this->label4->Text = L"��Լ��";
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->Location = System::Drawing::Point(18, 119);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(65, 12);
            this->label5->TabIndex = 0;
            this->label5->Text = L"Ͷ���ױ���";
            // 
            // label6
            // 
            this->label6->AutoSize = true;
            this->label6->Location = System::Drawing::Point(42, 146);
            this->label6->Name = L"label6";
            this->label6->Size = System::Drawing::Size(41, 12);
            this->label6->TabIndex = 0;
            this->label6->Text = L"������";
            // 
            // label7
            // 
            this->label7->AutoSize = true;
            this->label7->Location = System::Drawing::Point(18, 173);
            this->label7->Name = L"label7";
            this->label7->Size = System::Drawing::Size(65, 12);
            this->label7->TabIndex = 0;
            this->label7->Text = L"ƽ�����ͣ�";
            // 
            // label8
            // 
            this->label8->AutoSize = true;
            this->label8->Location = System::Drawing::Point(42, 200);
            this->label8->Name = L"label8";
            this->label8->Size = System::Drawing::Size(41, 12);
            this->label8->TabIndex = 0;
            this->label8->Text = L"������";
            // 
            // label9
            // 
            this->label9->AutoSize = true;
            this->label9->Location = System::Drawing::Point(18, 227);
            this->label9->Name = L"label9";
            this->label9->Size = System::Drawing::Size(65, 12);
            this->label9->TabIndex = 0;
            this->label9->Text = L"ί�м۸�";
            // 
            // label10
            // 
            this->label10->AutoSize = true;
            this->label10->Location = System::Drawing::Point(42, 254);
            this->label10->Name = L"label10";
            this->label10->Size = System::Drawing::Size(41, 12);
            this->label10->TabIndex = 0;
            this->label10->Text = L"�ǵ���";
            // 
            // label11
            // 
            this->label11->AutoSize = true;
            this->label11->Location = System::Drawing::Point(30, 281);
            this->label11->Name = L"label11";
            this->label11->Size = System::Drawing::Size(53, 12);
            this->label11->TabIndex = 0;
            this->label11->Text = L"��ƽ����";
            // 
            // label12
            // 
            this->label12->AutoSize = true;
            this->label12->Location = System::Drawing::Point(18, 308);
            this->label12->Name = L"label12";
            this->label12->Size = System::Drawing::Size(65, 12);
            this->label12->TabIndex = 0;
            this->label12->Text = L"��ƽ������";
            // 
            // label13
            // 
            this->label13->AutoSize = true;
            this->label13->Location = System::Drawing::Point(18, 335);
            this->label13->Name = L"label13";
            this->label13->Size = System::Drawing::Size(65, 12);
            this->label13->TabIndex = 0;
            this->label13->Text = L"�����ʽ�";
            // 
            // RiskEvent_textBox
            // 
            this->RiskEvent_textBox->Enabled = false;
            this->RiskEvent_textBox->Location = System::Drawing::Point(94, 8);
            this->RiskEvent_textBox->Name = L"RiskEvent_textBox";
            this->RiskEvent_textBox->Size = System::Drawing::Size(141, 21);
            this->RiskEvent_textBox->TabIndex = 1;
            // 
            // Account_textBox
            // 
            this->Account_textBox->Enabled = false;
            this->Account_textBox->Location = System::Drawing::Point(94, 35);
            this->Account_textBox->Name = L"Account_textBox";
            this->Account_textBox->Size = System::Drawing::Size(141, 21);
            this->Account_textBox->TabIndex = 1;
            // 
            // Name_textBox
            // 
            this->Name_textBox->Enabled = false;
            this->Name_textBox->Location = System::Drawing::Point(94, 62);
            this->Name_textBox->Name = L"Name_textBox";
            this->Name_textBox->Size = System::Drawing::Size(141, 21);
            this->Name_textBox->TabIndex = 1;
            // 
            // Contract_textBox
            // 
            this->Contract_textBox->Enabled = false;
            this->Contract_textBox->Location = System::Drawing::Point(94, 89);
            this->Contract_textBox->Name = L"Contract_textBox";
            this->Contract_textBox->Size = System::Drawing::Size(141, 21);
            this->Contract_textBox->TabIndex = 1;
            // 
            // SpeculateHedge_textBox
            // 
            this->SpeculateHedge_textBox->Enabled = false;
            this->SpeculateHedge_textBox->Location = System::Drawing::Point(94, 116);
            this->SpeculateHedge_textBox->Name = L"SpeculateHedge_textBox";
            this->SpeculateHedge_textBox->Size = System::Drawing::Size(141, 21);
            this->SpeculateHedge_textBox->TabIndex = 1;
            // 
            // Business_textBox
            // 
            this->Business_textBox->Enabled = false;
            this->Business_textBox->Location = System::Drawing::Point(94, 143);
            this->Business_textBox->Name = L"Business_textBox";
            this->Business_textBox->Size = System::Drawing::Size(141, 21);
            this->Business_textBox->TabIndex = 1;
            // 
            // OrderCount_textBox
            // 
            this->OrderCount_textBox->Location = System::Drawing::Point(94, 197);
            this->OrderCount_textBox->Name = L"OrderCount_textBox";
            this->OrderCount_textBox->Size = System::Drawing::Size(141, 21);
            this->OrderCount_textBox->TabIndex = 1;
            this->OrderCount_textBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &ForceShortcutForm::OrderCount_textBox_KeyPress);
            // 
            // OrderPrice_textBox
            // 
            this->OrderPrice_textBox->Location = System::Drawing::Point(94, 224);
            this->OrderPrice_textBox->Name = L"OrderPrice_textBox";
            this->OrderPrice_textBox->Size = System::Drawing::Size(141, 21);
            this->OrderPrice_textBox->TabIndex = 1;
            this->OrderPrice_textBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &ForceShortcutForm::OrderPrice_textBox_KeyPress);
            // 
            // PecentLimit_textBox
            // 
            this->PecentLimit_textBox->Enabled = false;
            this->PecentLimit_textBox->Location = System::Drawing::Point(94, 251);
            this->PecentLimit_textBox->Name = L"PecentLimit_textBox";
            this->PecentLimit_textBox->Size = System::Drawing::Size(141, 21);
            this->PecentLimit_textBox->TabIndex = 1;
            // 
            // CloseVol_textBox
            // 
            this->CloseVol_textBox->Enabled = false;
            this->CloseVol_textBox->Location = System::Drawing::Point(94, 278);
            this->CloseVol_textBox->Name = L"CloseVol_textBox";
            this->CloseVol_textBox->Size = System::Drawing::Size(141, 21);
            this->CloseVol_textBox->TabIndex = 1;
            // 
            // TodayCloseVol_textBox
            // 
            this->TodayCloseVol_textBox->Enabled = false;
            this->TodayCloseVol_textBox->Location = System::Drawing::Point(94, 305);
            this->TodayCloseVol_textBox->Name = L"TodayCloseVol_textBox";
            this->TodayCloseVol_textBox->Size = System::Drawing::Size(141, 21);
            this->TodayCloseVol_textBox->TabIndex = 1;
            // 
            // FundsAvailable_textBox
            // 
            this->FundsAvailable_textBox->Enabled = false;
            this->FundsAvailable_textBox->Location = System::Drawing::Point(94, 332);
            this->FundsAvailable_textBox->Name = L"FundsAvailable_textBox";
            this->FundsAvailable_textBox->Size = System::Drawing::Size(141, 21);
            this->FundsAvailable_textBox->TabIndex = 1;
            // 
            // FlatStyle_comboBox
            // 
            this->FlatStyle_comboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->FlatStyle_comboBox->FormattingEnabled = true;
            this->FlatStyle_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"ƽ��", L"ƽ��"});
            this->FlatStyle_comboBox->Location = System::Drawing::Point(94, 170);
            this->FlatStyle_comboBox->Name = L"FlatStyle_comboBox";
            this->FlatStyle_comboBox->Size = System::Drawing::Size(69, 20);
            this->FlatStyle_comboBox->TabIndex = 2;
            // 
            // Force_checkBox
            // 
            this->Force_checkBox->AutoSize = true;
            this->Force_checkBox->Checked = true;
            this->Force_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
            this->Force_checkBox->Enabled = false;
            this->Force_checkBox->Location = System::Drawing::Point(169, 173);
            this->Force_checkBox->Name = L"Force_checkBox";
            this->Force_checkBox->Size = System::Drawing::Size(48, 16);
            this->Force_checkBox->TabIndex = 3;
            this->Force_checkBox->Text = L"ǿƽ";
            this->Force_checkBox->UseVisualStyleBackColor = true;
            // 
            // OkButton
            // 
            this->OkButton->Location = System::Drawing::Point(88, 365);
            this->OkButton->Name = L"OkButton";
            this->OkButton->Size = System::Drawing::Size(75, 23);
            this->OkButton->TabIndex = 4;
            this->OkButton->Text = L"ȷ��";
            this->OkButton->UseVisualStyleBackColor = true;
            this->OkButton->Click += gcnew System::EventHandler(this, &ForceShortcutForm::OkButton_Click);
            // 
            // ForceShortcutForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(251, 400);
            this->Controls->Add(this->OkButton);
            this->Controls->Add(this->Force_checkBox);
            this->Controls->Add(this->FlatStyle_comboBox);
            this->Controls->Add(this->FundsAvailable_textBox);
            this->Controls->Add(this->TodayCloseVol_textBox);
            this->Controls->Add(this->CloseVol_textBox);
            this->Controls->Add(this->PecentLimit_textBox);
            this->Controls->Add(this->OrderPrice_textBox);
            this->Controls->Add(this->OrderCount_textBox);
            this->Controls->Add(this->Business_textBox);
            this->Controls->Add(this->SpeculateHedge_textBox);
            this->Controls->Add(this->Contract_textBox);
            this->Controls->Add(this->Name_textBox);
            this->Controls->Add(this->Account_textBox);
            this->Controls->Add(this->RiskEvent_textBox);
            this->Controls->Add(this->label13);
            this->Controls->Add(this->label12);
            this->Controls->Add(this->label10);
            this->Controls->Add(this->label9);
            this->Controls->Add(this->label11);
            this->Controls->Add(this->label5);
            this->Controls->Add(this->label8);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->label7);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label6);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->Name = L"ForceShortcutForm";
            this->Text = L"����¼��";
            this->Load += gcnew System::EventHandler(this, &ForceShortcutForm::ForceShortcutForm_Load);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &ForceShortcutForm::ForceShortcutForm_Closed);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    //����WndProc���Դ���WM_COMMAND��Ϣ
    protected:virtual void WndProc( Message% m ) override
              {
                  if (  m.Msg == WM_COMMAND_Win && m.WParam.ToInt32()==WndCmd_YourPkgArrival )
                  {
                      char *p = (char*)m.LParam.ToInt32();
                      Stru_UniPkgHead head;					
                      memset(&head, 0, sizeof(head));
                      memcpy(&head, p, sizeof(head));

                      switch(head.cmdid)
                      {
                      case Cmd_RM_SubscribeQuot_Rsp://�������鷵��
                          if ( head.userdata1 == OPERATOR_SUCCESS )
                          {	
                              int nItemCount = head.len/sizeof(DepthMarketDataField);

                              for(int i=0; i<nItemCount; i++) 
                              {
                                  m_pDepthMarketDataField = (DepthMarketDataField*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(DepthMarketDataField));  
                              }     

                              char strChar[256] = {0};
                              if(m_strDirection == "��ͷ")
                              {
                                  if(m_pDepthMarketDataField != NULL)
                                  {
                                      sprintf(strChar, "%.2f",m_pDepthMarketDataField->UpperLimitPrice);
                                      m_strPrice = Tools::string2String(strChar);
                                  } 
                              }else if(m_strDirection == "��ͷ")
                              {
                                  if(m_pDepthMarketDataField != NULL)
                                  {
                                      sprintf(strChar, "%.2f",m_pDepthMarketDataField->LowerLimitPrice);
                                      m_strPrice = Tools::string2String(strChar);
                                  }
                              }

                              this->OrderPrice_textBox->Text = m_strPrice;

                              sprintf(strChar, "%.2f/%.2f", m_pDepthMarketDataField->LowerLimitPrice,m_pDepthMarketDataField->UpperLimitPrice);
                              this->PecentLimit_textBox->Text = Tools::string2String(strChar);;//�ǵ�
                          }
                          else
                          {
                              string str = (char*)(p+sizeof(Stru_UniPkgHead));
                              String^ strtemp = Tools::string2String(str.c_str());
                              MessageBox::Show(strtemp, "��ʾ",MessageBoxButtons::OK);				
                          }
                          break;
                      case Cmd_RM_Quot_Push://��������
                          if ( head.userdata1 == OPERATOR_SUCCESS )
                          {	
                              m_pDepthMarketDataField = (DepthMarketDataField*)(p+sizeof(Stru_UniPkgHead));  
                              if( 0 != strcmp(m_pDepthMarketDataField->InstrumentID,Tools::String2string(m_strInstrumentID).c_str()))
                                  break; 
                              
                              char strChar[256] = {0};
                              if(m_strDirection == "��ͷ")
                              {
                                  if(m_pDepthMarketDataField != NULL)
                                  {
                                      sprintf(strChar, "%.2f",m_pDepthMarketDataField->UpperLimitPrice);
                                      m_strPrice = Tools::string2String(strChar);
                                  } 
                              }else if(m_strDirection == "��ͷ")
                              {
                                  if(m_pDepthMarketDataField != NULL)
                                  {
                                      sprintf(strChar, "%.2f",m_pDepthMarketDataField->LowerLimitPrice);
                                      m_strPrice = Tools::string2String(strChar);
                                  }
                              }

                              this->OrderPrice_textBox->Text = m_strPrice;

                              sprintf(strChar, "%.2f/%.2f", m_pDepthMarketDataField->LowerLimitPrice,m_pDepthMarketDataField->UpperLimitPrice);
                              this->PecentLimit_textBox->Text = Tools::string2String(strChar);;//�ǵ�
                          }
                          else
                          {
                              string str = (char*)(p+sizeof(Stru_UniPkgHead));
                              String^ strtemp = Tools::string2String(str.c_str());
                              MessageBox::Show(strtemp, "��ʾ",MessageBoxButtons::OK);				
                          }
                          break;
                      case Cmd_RM_SubscribeFund_Rsp://�����ʽ𷵻�
                          {
                              if(head.userdata1 == OPERATOR_SUCCESS) 
                              {                 
                                  int nItemCount = head.len/sizeof(RiskAllAccountField);
                                  for(int i=0; i<nItemCount; i++) 
                                  {
                                      m_pFundInfo = (RiskAllAccountField*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(RiskAllAccountField));
                                  }

                                  char strChar[256] = {0};
                                  if(m_pFundInfo != NULL)
                                      sprintf(strChar, "%.2f", m_pFundInfo->cur.Available);
                                  m_strFundsAvailable = Tools::string2String(strChar);
                                  this->FundsAvailable_textBox->Text = m_strFundsAvailable;
                              }
                              else 
                              {
                                  p[sizeof(head)+head.len-1]=0;
                                  //TRACE(p+sizeof(head));
                              }
                          }
                          break;
                      case Cmd_RM_Fund_Push://�ʽ�����
                          {
                              if(m_pFundInfo == NULL)
                                  break;

                              int nItemCount = head.len/sizeof(RiskSyncAccountField);
                              if(head.userdata1 == OPERATOR_SUCCESS) 
                              {
                                  for(int i=0; i<nItemCount; i++) 
                                  {
                                      m_pFundInfo->cur = *((RiskSyncAccountField*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(RiskSyncAccountField)));
                                  }

                                  char strChar[256] = {0};
                                  if(m_pFundInfo != NULL)
                                      sprintf(strChar, "%.2f", m_pFundInfo->cur.Available);
                                  m_strFundsAvailable = Tools::string2String(strChar);
                                  this->FundsAvailable_textBox->Text = m_strFundsAvailable;
                              }
                          }
                          break;  
                      case Cmd_RM_SubscribeTrade_Rsp://���ĳɽ�����
                          if ( head.userdata1 == OPERATOR_SUCCESS )
                          {	
                              SequencialOrderField* pSequencialOrderField = NULL; 
                              int nItemCount = head.len/sizeof(SequencialOrderField);                
                              for(int i=0; i<nItemCount; i++) 
                              {
                                  pSequencialOrderField = (SequencialOrderField*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(SequencialOrderField));  
                              }    
                          }
                          else
                          {
                              string str = (char*)(p+sizeof(Stru_UniPkgHead));
                              String^ strtemp = Tools::string2String(str.c_str());
                              MessageBox::Show(strtemp, "��ʾ",MessageBoxButtons::OK);				
                          }
                          break;
                      case Cmd_RM_Trade_Push://�ɽ�����
                          if ( head.userdata1 == OPERATOR_SUCCESS )
                          {	
                              SequencialOrderField* pSequencialOrderField = NULL; 
                              int nItemCount = head.len/sizeof(SequencialOrderField);
                              for(int i=0; i<nItemCount; i++) 
                              {
                                  pSequencialOrderField = (SequencialOrderField*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(SequencialOrderField));  
                              }     
                          }
                          else
                          {
                              string str = (char*)(p+sizeof(Stru_UniPkgHead));
                              String^ strtemp = Tools::string2String(str.c_str());
                              MessageBox::Show(strtemp, "��ʾ",MessageBoxButtons::OK);				
                          }
                          break;
                      case Cmd_RM_ForceCloseOrderInsert_Rsp://ǿƽ����¼�뷵��
                          if ( head.userdata1 == OPERATOR_SUCCESS )
                          {	
                              int nItemCount = head.len/sizeof(InputOrderField);
                              for(int i=0; i<nItemCount; i++) 
                              {
                                  InputOrderField* pInputOrderField = (InputOrderField*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(InputOrderField));  
                              }
                          }
                          else
                          {
                              string str = (char*)(p+sizeof(Stru_UniPkgHead));
                              String^ strtemp = Tools::string2String(str.c_str());
                              MessageBox::Show(strtemp, "��ʾ",MessageBoxButtons::OK);				
                          }
                          break;
                      }
                  }
                  Form::WndProc ( m );
              }
public:bool init(SequencialPositionField& position,RiskEvt^ riskEvt)
       {
           if(riskEvt == nullptr)
               return false;

           std::string strTemp;
           char strChar[256];           
   
           TrustTradeAccount oTrustTradeAccount;
           if(!m_pDataInfo->GetTrustTradeAccount(position.InvestorID, oTrustTradeAccount))
                return false;

           m_pSequencialPositionField = &position;
           m_pTrustTradeAccount = &oTrustTradeAccount;           

           if(riskEvt->CurRiskEventPanel1->TradeAccountHasRiskEvent(atoi(m_pTrustTradeAccount->szAccount)))
           //if(1)
           {
               int nRiskEventID = m_pTrustTradeAccount->nRiskIndModuleID;
               m_strRiskEventID = Convert::ToString(nRiskEventID); 
               m_strAccount = Tools::string2String(m_pTrustTradeAccount->szAccount);
               m_strName = Tools::string2String(m_pTrustTradeAccount->szName);

               m_strInstrumentID = Tools::string2String(position.InstrumentID);
               switch(position.HedgeFlag)
               {
               case '1'://Ͷ��
                   strTemp = "Ͷ��";
                   break;
               case '2'://����
                   strTemp = "����";
                   break;
               case '3'://�ױ�
                   strTemp ="�ױ�"; 
                   break;
               }
               m_strSpeculateHedge = Tools::string2String(strTemp.c_str());
               switch(position.PosiDirection)
               {
               case '1'://��
                   strTemp = "��";
                   break;
               case '2'://��ͷ 
                   strTemp = "��ͷ";//���෴����
                   if(m_pDepthMarketDataField != NULL)
                   {
                       sprintf(strChar, "%.2f",m_pDepthMarketDataField->LowerLimitPrice);
                       m_strPrice = Tools::string2String(strChar);
                   }
                   break;
               case '3':
                   strTemp = "��ͷ";
                   if(m_pDepthMarketDataField != NULL)
                   {
                       sprintf(strChar, "%.2f",m_pDepthMarketDataField->UpperLimitPrice);
                       m_strPrice = Tools::string2String(strChar);
                   }
                   break;
               }
               m_strDirection = Tools::string2String(strTemp.c_str());
               if(position.TodayPosition > 0)
                   strTemp = "ƽ��";
               else if(position.YdPosition > 0) 
                   strTemp = "ƽ��";
               else
                   strTemp = "ǿƽ��������";
               m_strFlatStyle = Tools::string2String(strTemp.c_str());
               m_bForce = true;
               sprintf(strChar, "%d", position.Position);
               m_strCount = Tools::string2String(strChar);
               if(m_pDepthMarketDataField != NULL)
                   sprintf(strChar, "%.2f/%.2f", m_pDepthMarketDataField->LowerLimitPrice,m_pDepthMarketDataField->UpperLimitPrice);
               m_strPecentPerLimit = Tools::string2String(strChar);;//�ǵ�
               sprintf(strChar, "%d", position.Position);
               m_strCloseVolume = Tools::string2String(strChar);
               sprintf(strChar, "%d", position.TodayPosition);
               m_strTodayCloseVol = Tools::string2String(strChar);
               if(m_pFundInfo != NULL)
                   sprintf(strChar, "%.2f", m_pFundInfo->cur.Available);
               m_strFundsAvailable = Tools::string2String(strChar);
               this->FundsAvailable_textBox->Focus();
               return true;
           }
           else
               return false;          
       }
private: System::Void ForceShortcutForm_Load(System::Object^  sender, System::EventArgs^  e) 
         {
             IntPtr hWnd=this->Handle;  
             //��������
             ForceShortcutForm2::SubscribeQuotRsp((int)hWnd);
             ForceShortcutForm2::SubscribeQuotPush((int)hWnd);
             ForceShortcutForm2::UnsubscribeQuotRsp((int)hWnd);
             //�����ʽ�
             ForceShortcutForm2::SubscribeFundRsp((int)hWnd);
             ForceShortcutForm2::SubscribeFundPush((int)hWnd);
             ForceShortcutForm2::UnsubscribeFundRsp((int)hWnd);
             //���ĳɽ�
             ForceShortcutForm2::SubscribeTradeRsp((int)hWnd);
             ForceShortcutForm2::SubscribeTradePush((int)hWnd);
             ForceShortcutForm2::UnsubscribeTradeRsp((int)hWnd);
             //����ǿƽ
             ForceShortcutForm2::SubscribeForceCloseOrderInsertRsp((int)hWnd);

                     
             TInstrumentIDType InstrumentID = {0};
             strcpy(InstrumentID,Tools::String2string(m_strInstrumentID).c_str()); 

             //���������������
             ForceShortcutForm2::ReqSubscribeQuot(InstrumentID);
             //�����ʽ�����
             ForceShortcutForm2::ReqSubscribeFund(m_pTrustTradeAccount->nTradeAccountID);
             //���ĳɽ�����   
             ForceShortcutForm2::ReqSubscribeTrade(m_pTrustTradeAccount->nTradeAccountID);

             //��ʼ������Ԫ��
             this->RiskEvent_textBox->Text = m_strRiskEventID;
             this->Account_textBox->Text = m_strAccount;
             this->Name_textBox->Text = m_strName;
             this->Contract_textBox->Text = m_strInstrumentID;
             this->SpeculateHedge_textBox->Text = m_strSpeculateHedge;
             this->Business_textBox ->Text = m_strDirection;
             this->FlatStyle_comboBox->Text = m_strFlatStyle;
             this->OrderCount_textBox->Text = m_strCount;
             this->OrderPrice_textBox->Text = m_strPrice;
             this->PecentLimit_textBox->Text = m_strPecentPerLimit;
             this->CloseVol_textBox->Text = m_strCloseVolume;
             this->TodayCloseVol_textBox->Text = m_strTodayCloseVol;
             this->FundsAvailable_textBox->Text = m_strFundsAvailable;
       }
private: System::Void OkButton_Click(System::Object^  sender, System::EventArgs^  e) {       
             if(String::IsNullOrEmpty(OrderCount_textBox->Text))
             {
                 MessageBox::Show("ί����������Ϊ��");
                 return;
             }

             if(String::IsNullOrEmpty(OrderPrice_textBox->Text))
             {
                 MessageBox::Show("ί�м۸���Ϊ��");
                 return;
             }

             CLOSE_TYPE nCloseType = (0 == FlatStyle_comboBox->SelectedIndex)?CLOSE_TYPE_TODAY:CLOSE_TYPE_YESTERDAY;  
             int nCount = Convert::ToInt32(OrderCount_textBox->Text);
             double dPrice = Convert::ToDouble(OrderPrice_textBox->Text);
             //����������Ч��  
             if(ChenckDataLegal(nCloseType,nCount,dPrice))
             {                 
                 RiskForceCloseOrderField riskForceCloseOrderField;
                 memset(&riskForceCloseOrderField,0,sizeof(RiskForceCloseOrderField));
                 DataPackage(riskForceCloseOrderField);
                 //RiskForceCloseOrderField* pRiskForceCloseOrderField = new RiskForceCloseOrderField;
                 //memset(pRiskForceCloseOrderField,0,sizeof(RiskForceCloseOrderField));
                 //DataPackage(*pRiskForceCloseOrderField);
                 //����ǿƽ����¼��                     
                 ForceShortcutForm2::ReqForceCloseOrderInsert(riskForceCloseOrderField); 
             }
             else
             {
                 string str = "�������÷Ƿ�";
                 String^ strtemp = Tools::string2String(str.c_str());
                 MessageBox::Show(strtemp, "��ʾ",MessageBoxButtons::OK);
             }
         }
private: System::Void DataPackage(RiskForceCloseOrderField& oRiskForceCloseOrderField)
         {
             char szTemp[128] = {0};
             char szDataTemp[128]={0};
             struct tm *p;
             time_t timeData = time(NULL);
             p = localtime(&timeData);
             p->tm_year = p->tm_year + 1900;
             p->tm_mon = p->tm_mon + 1;

             sprintf(szDataTemp,"%02d:%02d:%02d",p->tm_hour,p->tm_min,p->tm_sec);

             ///���ǿƽ����
             /*//�ֹ�ǿƽ
             #define T_FCT_Manual '0'
             ///��һͶ���߸���ǿƽ
             #define T_FCT_Single '1'
             ///����Ͷ���߸���ǿƽ
             #define T_FCT_Group '2'
             */
             oRiskForceCloseOrderField.FCType = T_FCT_Single;
             ///����ǿƽ��������ʱ��
             strcpy(oRiskForceCloseOrderField.Time1,szDataTemp);
             ///����ǿƽ��������ʱ�䣨���룩
             oRiskForceCloseOrderField.Millisec1 = 225;
             ///ǿƽ�����ύʱ��
             //strcpy(oRiskForceCloseOrderField.Time2,szDataTemp);
             ///ǿƽ�����ύʱ�䣨���룩
             //oRiskForceCloseOrderField.Millisec2 = (p->tm_hour*60*60*1000)+(p->tm_min*60*1000)+(p->tm_sec*1000);;
             ///ǿƽ�������
             //sprintf(szTemp,"%d",00001);
             //strcpy(oRiskForceCloseOrderField.FCSceneId,szTemp);

             ///���뱨���ṹ
             ///���͹�˾����
             strcpy(oRiskForceCloseOrderField.BrokerID,m_pSequencialPositionField->BrokerID);
             ///Ͷ���ߴ���
             strcpy(oRiskForceCloseOrderField.InvestorID,m_pSequencialPositionField->InvestorID);
             ///��Լ����
             strcpy(oRiskForceCloseOrderField.InstrumentID,m_pSequencialPositionField->InstrumentID);
             ///��������
             oRiskForceCloseOrderField.OrderRef[0] = '0';
             ///�û�����
             //strcpy(oRiskForceCloseOrderField.UserID,m_pSequencialPositionField->InvestorID);
             ///�����۸�����
             /*
             ///�����
             #define T_OPT_AnyPrice '1'
             ///�޼�
             #define T_OPT_LimitPrice '2'
             ///���ż�
             #define T_OPT_BestPrice '3'
             ///���¼�
             #define T_OPT_LastPrice '4'
             ///���¼۸����ϸ�1��ticks
             #define T_OPT_LastPricePlusOneTicks '5'
             ///���¼۸����ϸ�2��ticks
             #define T_OPT_LastPricePlusTwoTicks '6'
             ///���¼۸����ϸ�3��ticks
             #define T_OPT_LastPricePlusThreeTicks '7'
             ///��һ��
             #define T_OPT_AskPrice1 '8'
             ///��һ�۸����ϸ�1��ticks
             #define T_OPT_AskPrice1PlusOneTicks '9'
             ///��һ�۸����ϸ�2��ticks
             #define T_OPT_AskPrice1PlusTwoTicks 'A'
             ///��һ�۸����ϸ�3��ticks
             #define T_OPT_AskPrice1PlusThreeTicks 'B'
             ///��һ��
             #define T_OPT_BidPrice1 'C'
             ///��һ�۸����ϸ�1��ticks
             #define T_OPT_BidPrice1PlusOneTicks 'D'
             ///��һ�۸����ϸ�2��ticks
             #define T_OPT_BidPrice1PlusTwoTicks 'E'
             ///��һ�۸����ϸ�3��ticks
             #define T_OPT_BidPrice1PlusThreeTicks 'F'
             */
             oRiskForceCloseOrderField.OrderPriceType = T_OPT_LimitPrice;
             ///��������
             /*
             //��
             #define T_D_Buy '0'
             ///��
             #define T_D_Sell '1'
             */
             if (this->Business_textBox->Text == "��ͷ")
                 oRiskForceCloseOrderField.Direction = T_D_Buy;
             else if(this->Business_textBox->Text == "��ͷ")
                 oRiskForceCloseOrderField.Direction = T_D_Sell;
             ///��Ͽ�ƽ��־
             /*
             ///����
             #define T_OF_Open '0'
             ///ƽ��
             #define T_OF_Close '1'
             ///ǿƽ
             #define T_OF_ForceClose '2'
             ///ƽ��
             #define T_OF_CloseToday '3'
             ///ƽ��
             #define T_OF_CloseYesterday '4'
             ///ǿ��
             #define T_OF_ForceOff '5'
             ///����ǿƽ
             #define T_OF_LocalForceClose '6'
             */          
             if(this->FlatStyle_comboBox->Text == "ƽ��")
                oRiskForceCloseOrderField.CombOffsetFlag[0] = T_OF_Close;
             else if(this->FlatStyle_comboBox->Text == "ƽ��")
                 oRiskForceCloseOrderField.CombOffsetFlag[0] = T_OF_CloseToday;

             oRiskForceCloseOrderField.CombOffsetFlag[0] = T_OF_Close;

             ///���Ͷ���ױ���־
            /*//�ױ�
            #define T_ECIDT_Hedge '1'
             ///����
            #define T_ECIDT_Arbitrage '2'
             ///Ͷ��
            #define T_ECIDT_Speculation '3'
            */
             if (this->SpeculateHedge_textBox->Text == "�ױ�")
                oRiskForceCloseOrderField.CombHedgeFlag[0] = T_ECIDT_Hedge;
             else if (this->SpeculateHedge_textBox->Text == "����")
                oRiskForceCloseOrderField.CombHedgeFlag[0] = T_ECIDT_Arbitrage;
             else if (this->SpeculateHedge_textBox->Text == "Ͷ��")
                oRiskForceCloseOrderField.CombHedgeFlag[0] = T_ECIDT_Speculation;

             oRiskForceCloseOrderField.CombHedgeFlag[0] = T_ECIDT_Hedge;
             ///�۸�
             oRiskForceCloseOrderField.LimitPrice = Convert::ToDouble(m_strPrice);
             ///����
             oRiskForceCloseOrderField.VolumeTotalOriginal = Convert::ToInt32(m_strCount);
             ///��Ч������
             /*
             ///������ɣ�������
             #define T_TC_IOC '1'
             ///������Ч
             #define T_TC_GFS '2'
             ///������Ч
             #define T_TC_GFD '3'
             ///ָ������ǰ��Ч
             #define T_TC_GTD '4'
             ///����ǰ��Ч
             #define T_TC_GTC '5'
             ///���Ͼ�����Ч
             #define T_TC_GFA '6'
             */
             oRiskForceCloseOrderField.TimeCondition = T_TC_GFD;
             ///GTD����
             //strcpy(oRiskForceCloseOrderField.GTDDate,szDataTemp);
             /*
             ///�κ�����
             #define T_VC_AV '1'
             ///��С����
             #define T_VC_MV '2'
             ///ȫ������
             #define T_VC_CV '3'
             */
             ///�ɽ�������
             oRiskForceCloseOrderField.VolumeCondition = T_VC_AV;
             ///��С�ɽ���
             oRiskForceCloseOrderField.MinVolume = 0;
             ///��������
             /*
             ///����
             #define T_CC_Immediately '1'
             ///ֹ��
             #define T_CC_Touch '2'
             ///ֹӮ
             #define T_CC_TouchProfit '3'
             ///Ԥ��
             #define T_CC_ParkedOrder '4'
             ///���¼۴���������
             #define T_CC_LastPriceGreaterThanStopPrice '5'
             ///���¼۴��ڵ���������
             #define T_CC_LastPriceGreaterEqualStopPrice '6'
             ///���¼�С��������
             #define T_CC_LastPriceLesserThanStopPrice '7'
             ///���¼�С�ڵ���������
             #define T_CC_LastPriceLesserEqualStopPrice '8'
             ///��һ�۴���������
             #define T_CC_AskPriceGreaterThanStopPrice '9'
             ///��һ�۴��ڵ���������
             #define T_CC_AskPriceGreaterEqualStopPrice 'A'
             ///��һ��С��������
             #define T_CC_AskPriceLesserThanStopPrice 'B'
             ///��һ��С�ڵ���������
             #define T_CC_AskPriceLesserEqualStopPrice 'C'
             ///��һ�۴���������
             #define T_CC_BidPriceGreaterThanStopPrice 'D'
             ///��һ�۴��ڵ���������
             #define T_CC_BidPriceGreaterEqualStopPrice 'E'
             ///��һ��С��������
             #define T_CC_BidPriceLesserThanStopPrice 'F'
             ///��һ��С�ڵ���������
             #define T_CC_BidPriceLesserEqualStopPrice 'H'
             */
             oRiskForceCloseOrderField.ContingentCondition = T_CC_Immediately;
             ///ֹ���
             oRiskForceCloseOrderField.StopPrice = 0;
             ///ǿƽԭ��
             /*
             ///��ǿƽ
             #define T_FCC_NotForceClose '0'
             ///�ʽ���
             #define T_FCC_LackDeposit '1'
             ///�ͻ�����
             #define T_FCC_ClientOverPositionLimit '2'
             ///��Ա����
             #define T_FCC_MemberOverPositionLimit '3'
             ///�ֲַ�������
             #define T_FCC_NotMultiple '4'
             ///Υ��
             #define T_FCC_Violation '5'
             ///����
             #define T_FCC_Other '6'
             ///��Ȼ���ٽ�����
             #define T_FCC_PersonDeliv '7'
             */
             //oRiskForceCloseOrderField.ForceCloseReason = T_FCC_NotForceClose;
             //oRiskForceCloseOrderField.ForceCloseReason = T_OF_ForceClose;
             ///�Զ������־
             //oRiskForceCloseOrderField.IsAutoSuspend;
             ///ҵ��Ԫ
             //oRiskForceCloseOrderField.BusinessUnit;
             ///������
             //oRiskForceCloseOrderField.RequestID;
             ///�û�ǿ����־
             oRiskForceCloseOrderField.UserForceClose = true;
             ///ǰ�ñ��
             //oRiskForceCloseOrderField.FrontID;
             ///�Ự���
             //oRiskForceCloseOrderField.SessionID;
         }
private: System::Boolean ChenckDataLegal(CLOSE_TYPE nCloseType, int nCount, double dPrice)
         {
             bool bResult = false;
             bool bPositionBuy = false;
             bool bPositionSell = false;
             int  nTPositionBuy = 0;
             int  nYPositionBuy = 0;
             int  nTPositionSell = 0;
             int  nYPositionSell = 0;


             string strInstrumentID = Tools::String2string(m_strInstrumentID);
             if (strInstrumentID == m_pSequencialPositionField->InstrumentID)
             {
                 switch(m_pSequencialPositionField->PosiDirection)
                 {
                 case 0://��ֲ�
                     bPositionBuy = true;
                     nTPositionBuy = m_pSequencialPositionField->TodayPosition;
                     nYPositionBuy = m_pSequencialPositionField->YdPosition;
                     break;
                 case 1://���ֲ�
                     bPositionSell = true;
                     nTPositionSell = m_pSequencialPositionField->TodayPosition;
                     nYPositionSell = m_pSequencialPositionField->YdPosition;
                     break;
                 }
             }

             if (Business_textBox->Text == "��")
             {     
                 switch(nCloseType)
                 {
                 case 0://CLOSE_TYPE_TODAY
                     if (nCount > nTPositionSell)
                     {
                         bResult = true;
                         string str = "ƽ����";
                         String^ strtemp = Tools::string2String(str.c_str());
                         MessageBox::Show(strtemp, "��ʾ",MessageBoxButtons::OK);
                     }
                     break;
                 case 1:
                     if (nCount > nYPositionSell)
                     {
                         bResult = true;
                         string str = "ƽ�ֲ���";
                         String^ strtemp = Tools::string2String(str.c_str());
                         MessageBox::Show(strtemp, "��ʾ",MessageBoxButtons::OK);
                     }
                     break;
                 }
             }else if(Business_textBox->Text == "��")
             {
                 switch(nCloseType)
                 {
                 case 0://CLOSE_TYPE_TODAY
                     if (nCount > nTPositionBuy)
                     {
                         bResult = true;
                         string str = "ƽ����";
                         String^ strtemp = Tools::string2String(str.c_str());
                         MessageBox::Show(strtemp, "��ʾ",MessageBoxButtons::OK);
                     }
                     break;
                 case 1:
                     if (nCount > nYPositionBuy)
                     {
                         bResult = true;
                         string str = "ƽ�ֲ���";
                         String^ strtemp = Tools::string2String(str.c_str());
                         MessageBox::Show(strtemp, "��ʾ",MessageBoxButtons::OK);
                     }
                     break;
                 }
             }


             if(bResult)
                 return false;
             else 
                 return true;
         }

private: System::Void ForceShortcutForm_Closed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
             IntPtr hWnd=this->Handle; 
             ForceShortcutForm2::UnsubscribeAllEvt((int)hWnd);
         }
//���������Ч��
private: System::Void OrderCount_textBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             if(e->KeyChar=='.' && OrderCount_textBox->Text->IndexOf(".")!=-1)  
             {  
                 e->Handled=true;  
             }  

             if(!((e->KeyChar>=48 && e->KeyChar<=57) || e->KeyChar=='.' || e->KeyChar==8))  
             {  
                 e->Handled=true;  
             }  
         }
private: System::Void OrderPrice_textBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             if(e->KeyChar=='.' && OrderCount_textBox->Text->IndexOf(".")!=-1)  
             {  
                 e->Handled=true;  
             }  

             if(!((e->KeyChar>=48 && e->KeyChar<=57) || e->KeyChar=='.' || e->KeyChar==8))  
             {  
                 e->Handled=true;  
             }  
         }
};
}
