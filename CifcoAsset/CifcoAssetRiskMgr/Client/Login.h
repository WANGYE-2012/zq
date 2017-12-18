#pragma once
#include "StdAfx.h"
//#include <windows.h>
#include "RiskManageCmd.h"
#include "RiskManageStruct.h"
#include "CommonPkg.h"
#include "TcpLayer.h"
#include "md5.h"
#include "CDataInfo.h"
#include "RiskEventMessage.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Client {

	/// <summary>
	/// Summary for Login
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Login : public System::Windows::Forms::Form
	{
	public:
		Login(void)
		{
			InitializeComponent();
			this->CenterToParent();
			//
			//TODO: Add the constructor code here
			//
			m_pdataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Login()
		{
			CDataInfo::DestroyInstance();
			if (components)
			{
				delete components;
			}
		}
		
	public:
	delegate void LoginEventEventHandler(Object^ sender, RiskEventEventArgs^ e);
		// �����¼�
	event LoginEventEventHandler^ LoginEvent1;

	public: System::Windows::Forms::TextBox^  textBox1;
	protected: 
	public: System::Windows::Forms::TextBox^  textBox2;


	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

	public: CDataInfo*  m_pdataInfo;

			void SetAutoStartPara(String^ strName, String^ strPwd)
			{
				textBox1->Text = strName;
				textBox2->Text = strPwd;
			}
			void LoginClick()
			{
				System::Object^  sender;
				System::EventArgs^ p;
				login_Click(sender, p); 
			}

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(111, 47);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(220, 21);
			this->textBox1->TabIndex = 0;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(111, 94);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(220, 21);
			this->textBox2->TabIndex = 1;
			this->textBox2->UseSystemPasswordChar = true;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(35, 56);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 4;
			this->label2->Text = L"�û�����";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(35, 97);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(41, 12);
			this->label3->TabIndex = 5;
			this->label3->Text = L"���룺";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(141, 155);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(83, 24);
			this->button1->TabIndex = 6;
			this->button1->Text = L"��¼";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Login::login_Click);
			// 
			// button2
			// 
			this->button2->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->button2->Location = System::Drawing::Point(248, 155);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(83, 23);
			this->button2->TabIndex = 7;
			this->button2->Text = L"ȡ��";
			this->button2->UseVisualStyleBackColor = true;
			// 
			// Login
			// 
			this->AcceptButton = this->button1;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(361, 188);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"Login";
			this->ShowInTaskbar = false;
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"�û���¼";
			this->Load += gcnew System::EventHandler(this, &Login::Login_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}	
#pragma endregion
	private: System::Void login_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 String ^strUserName = textBox1->Text;
				 String ^strPWD      = textBox2->Text;

				 if(strUserName->IsNullOrEmpty(strUserName))
				 {
					 MessageBox::Show("�û�������Ϊ�գ�", "��ʾ",MessageBoxButtons::OK);
					return;
				 }				
				 if(strPWD->IsNullOrEmpty(strPWD))
				 {					
					 MessageBox::Show("���벻��Ϊ�գ�", "��ʾ",MessageBoxButtons::OK);
					 return;
				 }		
				 const char* pUSER = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(strUserName)).ToPointer();
				 const char* pPWND = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(strPWD)).ToPointer();
			
				 LoginReq sLogin;
				 strcpy(sLogin.szAccount, pUSER);
				 strcpy(sLogin.szPassword, pPWND);	
				 MD5 md5(sLogin.szPassword);
				 strcpy(sLogin.szPassword, md5.toString().c_str());
				 sLogin.nRoleType = ROLE_TYPE_RISK_CONTROL;

				 CTcpLayer::SendData(Cmd_RM_Login_Req, (void*)&sLogin,sizeof(sLogin), 0);
			/*	char buf[sizeof(Stru_UniPkgHead)+ sizeof(sLogin) +4];
				 Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)buf;
				 PkgHead.Set(sizeof(sLogin),Cmd_RM_Login_Req,0,0);
				 memcpy(buf+sizeof(Stru_UniPkgHead),(void*)&sLogin,sizeof(sLogin));
				 *(unsigned int*)(buf+sizeof(PkgHead) + sizeof(sLogin))=TailID_UniPkg;
				 CTcpLayer::Senddata(buf,sizeof(buf));*/
			 }


			 //����WndProc���Դ���WM_COMMAND��Ϣ
	protected:virtual void WndProc( Message% m ) override
			  {
				  if (  m.Msg == WM_COMMAND_Win && m.WParam.ToInt32()==WndCmd_YourPkgArrival )
				  {
						char *p = (char*)m.LParam.ToInt32();
						Stru_UniPkgHead head;					
						memset(&head, 0, sizeof(head));
						memcpy(&head, p, sizeof(head));

						if ( head.cmdid == Cmd_RM_Login_Rsp )
						{
							if ( head.userdata1 == OPERATOR_SUCCESS )
							{	
								m_pdataInfo->Clear();

								String ^strUserName = textBox1->Text;
								String ^strPWD      = textBox2->Text;
								RiskEventEventArgs^ AccountArgs = gcnew RiskEventEventArgs();
								AccountArgs->SetStr(strUserName);
								LoginEvent1( this, AccountArgs );
								
								const char* pUSER = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(strUserName)).ToPointer();
								const char* pPWND = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(strPWD)).ToPointer();
								string strAccount, strPassWord;
								strAccount  = pUSER;
								strPassWord = pPWND;	
								m_pdataInfo->SetUserName(strAccount);
								m_pdataInfo->SetPwd(strPassWord);

								LoginRsp loginReq;
								memset(&loginReq, 0, sizeof(LoginRsp));
								memcpy(&loginReq, p+sizeof(Stru_UniPkgHead), sizeof(LoginRsp));
								m_pdataInfo->SetLoginRsp(loginReq);

								///��¼�ɹ�����ؿͻ�����Ҫ����Ļ�������
	
								//��ѯ�����ʹ���������(ֻ���ܲ�����˺�ʹ�ã�
								CTcpLayer::SendData(Cmd_RM_QryAssetMgmtOrg_Req, NULL, 0, 0);
								//��ѯ��Ʋ�Ʒ��Ϣ����
								CTcpLayer::SendData(Cmd_RM_QryFinancialProduct_Req, NULL, 0, 0);
								//��ѯ����ָ��ģ����Ϣ����
								CTcpLayer::SendData(Cmd_RM_QryRiskIndModule_Req, NULL, 0, 0);
								//��ѯ�ʹ���֯����Ʋ�Ʒ��ϵ
								CTcpLayer::SendData(Cmd_RM_QryAssetOrgProRelation_Req, NULL, 0, 0);
								//��ѯ����ָ����Ϣ����
								CTcpLayer::SendData(Cmd_RM_QryRiskIndicator_Req, NULL, 0, 0);
								//��ѯ����ί�н����˺�����
								int nID = loginReq.nAssetMgmtOrgID;
								CTcpLayer::SendData(Cmd_RM_QryTradeAccountByOrgID_Req,(void *)&nID, sizeof(int), 0);
								//��ѯ����Ա��Ϣ����							
								CTcpLayer::SendData(Cmd_RM_QryTraderByOrgID_Req, (void *)&nID, sizeof(int), 0);

								//��Լ�����б�����
								CTcpLayer::SendData(Cmd_RM_InstrumentInfoList_Req, NULL, 0, 0);
								
								//��ѯȨ����������
								int nPermission = loginReq.nPermissionID;
								CTcpLayer::SendData(Cmd_RM_QryPermissionValueByID_Req, (void *)&nPermission, sizeof(int), 0);
								//��ѯĳ�ʹ���֯���������˺�����
								CTcpLayer::SendData(Cmd_RM_QryAccountByOrgID_Req,(void *)&nID, sizeof(int), 0);
								
								//��ѯ����Ԥ����Ϣ
								CTcpLayer::SendData(Cmd_RM_QryRiskWarning_Req, NULL, 0, 0);
							}
							else
							{
								MessageBox::Show("�û�������������󣬵�¼ʧ�ܣ�", "��ʾ",MessageBoxButtons::OK);
							}
						}
				  }
				  Form::WndProc ( m );
			  }
	private: System::Void Login_Load(System::Object^  sender, System::EventArgs^  e)
			 {
				 //�����¼�
				 IntPtr hWnd=this->Handle;
				 CTcpLayer::SubscribePkg(Cmd_RM_Login_Rsp, (int)(hWnd.ToInt32()));
				 CenterToParent();
			 }
	
};
}
