#pragma once
#include "windows.h"
#include "CommonDef.h"
#include "CommonPkg.h"
#include "TestTcpBusiness.h"
#include "TraderManager.h"
#include "CommandManager.h"
#include "Tools.h"
#include "TestCmdDef.h"
#include "orderForm.h"
#include "TraderList.h"
namespace TesterAdmin {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			mTraderManager = new CTraderManager();
			mCommandManager = new CCommandManager();
			mpsCommand = new sCommand();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
			if(mTraderManager)
				delete mTraderManager;
			if(mCommandManager)
				delete mCommandManager;
			if(mpsCommand)
				delete mpsCommand;
		}

	protected: 






	private: System::Windows::Forms::ImageList^  imageList1;




	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  �ļ�ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �˳�ToolStripMenuItem;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton1;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ԱToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �鿴���н���ԱToolStripMenuItem;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton2;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton3;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton4;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton5;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::ListView^  listView1;
	private: System::Windows::Forms::ColumnHeader^  ѡ��;
	private: System::Windows::Forms::ColumnHeader^  ����Ա;
	private: System::Windows::Forms::ColumnHeader^  IP;
	private: System::Windows::Forms::ColumnHeader^  Socket;
	private: System::Windows::Forms::ColumnHeader^  ��ע;

	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::RichTextBox^  richTextBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label1;
	private: System::ComponentModel::IContainer^  components;

	private: CTraderManager*  mTraderManager;
	private: sCommand*  mpsCommand;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton6;
	private: CCommandManager*  mCommandManager;


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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->imageList1 = (gcnew System::Windows::Forms::ImageList(this->components));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->�ļ�ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�˳�ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ԱToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�鿴���н���ԱToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton3 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton4 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton5 = (gcnew System::Windows::Forms::ToolStripButton());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->ѡ�� = (gcnew System::Windows::Forms::ColumnHeader());
			this->����Ա = (gcnew System::Windows::Forms::ColumnHeader());
			this->IP = (gcnew System::Windows::Forms::ColumnHeader());
			this->Socket = (gcnew System::Windows::Forms::ColumnHeader());
			this->��ע = (gcnew System::Windows::Forms::ColumnHeader());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->toolStripButton6 = (gcnew System::Windows::Forms::ToolStripButton());
			this->menuStrip1->SuspendLayout();
			this->toolStrip1->SuspendLayout();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->Panel2->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			this->SuspendLayout();
			// 
			// imageList1
			// 
			this->imageList1->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"imageList1.ImageStream")));
			this->imageList1->TransparentColor = System::Drawing::Color::Transparent;
			this->imageList1->Images->SetKeyName(0, L"32f7bdcc87ff7e41573532ddfd2ff46a.jpg");
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->�ļ�ToolStripMenuItem, 
				this->����ԱToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(749, 24);
			this->menuStrip1->TabIndex = 4;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// �ļ�ToolStripMenuItem
			// 
			this->�ļ�ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->����ToolStripMenuItem, 
				this->�˳�ToolStripMenuItem});
			this->�ļ�ToolStripMenuItem->Name = L"�ļ�ToolStripMenuItem";
			this->�ļ�ToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->�ļ�ToolStripMenuItem->Text = L"�ļ�";
			// 
			// ����ToolStripMenuItem
			// 
			this->����ToolStripMenuItem->Name = L"����ToolStripMenuItem";
			this->����ToolStripMenuItem->Size = System::Drawing::Size(94, 22);
			this->����ToolStripMenuItem->Text = L"����";
			// 
			// �˳�ToolStripMenuItem
			// 
			this->�˳�ToolStripMenuItem->Name = L"�˳�ToolStripMenuItem";
			this->�˳�ToolStripMenuItem->Size = System::Drawing::Size(94, 22);
			this->�˳�ToolStripMenuItem->Text = L"�˳�";
			this->�˳�ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::�˳�ToolStripMenuItem_Click);
			// 
			// ����ԱToolStripMenuItem
			// 
			this->����ԱToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->�鿴���н���ԱToolStripMenuItem});
			this->����ԱToolStripMenuItem->Name = L"����ԱToolStripMenuItem";
			this->����ԱToolStripMenuItem->Size = System::Drawing::Size(53, 20);
			this->����ԱToolStripMenuItem->Text = L"����Ա";
			// 
			// �鿴���н���ԱToolStripMenuItem
			// 
			this->�鿴���н���ԱToolStripMenuItem->Name = L"�鿴���н���ԱToolStripMenuItem";
			this->�鿴���н���ԱToolStripMenuItem->Size = System::Drawing::Size(154, 22);
			this->�鿴���н���ԱToolStripMenuItem->Text = L"�鿴���н���Ա";
			this->�鿴���н���ԱToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::�鿴���н���ԱToolStripMenuItem_Click);
			// 
			// toolStrip1
			// 
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->toolStripButton6, 
				this->toolStripButton1, this->toolStripButton2, this->toolStripButton3, this->toolStripButton4, this->toolStripButton5});
			this->toolStrip1->Location = System::Drawing::Point(0, 24);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(749, 25);
			this->toolStrip1->TabIndex = 5;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// toolStripButton1
			// 
			this->toolStripButton1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton1.Image")));
			this->toolStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton1->Name = L"toolStripButton1";
			this->toolStripButton1->Size = System::Drawing::Size(109, 22);
			this->toolStripButton1->Text = L"�Զ����佻��Ա";
			this->toolStripButton1->Click += gcnew System::EventHandler(this, &Form1::toolStripButton1_Click);
			// 
			// toolStripButton2
			// 
			this->toolStripButton2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton2.Image")));
			this->toolStripButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton2->Name = L"toolStripButton2";
			this->toolStripButton2->Size = System::Drawing::Size(85, 22);
			this->toolStripButton2->Text = L"���³�ʼ��";
			this->toolStripButton2->Click += gcnew System::EventHandler(this, &Form1::toolStripButton2_Click);
			// 
			// toolStripButton3
			// 
			this->toolStripButton3->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton3.Image")));
			this->toolStripButton3->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton3->Name = L"toolStripButton3";
			this->toolStripButton3->Size = System::Drawing::Size(49, 22);
			this->toolStripButton3->Text = L"����";
			this->toolStripButton3->Click += gcnew System::EventHandler(this, &Form1::toolStripButton3_Click);
			// 
			// toolStripButton4
			// 
			this->toolStripButton4->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton4.Image")));
			this->toolStripButton4->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton4->Name = L"toolStripButton4";
			this->toolStripButton4->Size = System::Drawing::Size(49, 22);
			this->toolStripButton4->Text = L"�ر�";
			this->toolStripButton4->Click += gcnew System::EventHandler(this, &Form1::toolStripButton4_Click);
			// 
			// toolStripButton5
			// 
			this->toolStripButton5->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton5.Image")));
			this->toolStripButton5->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton5->Name = L"toolStripButton5";
			this->toolStripButton5->Size = System::Drawing::Size(49, 22);
			this->toolStripButton5->Text = L"�µ�";
			this->toolStripButton5->Click += gcnew System::EventHandler(this, &Form1::toolStripButton5_Click);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 49);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->listView1);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->splitContainer2);
			this->splitContainer1->Size = System::Drawing::Size(749, 404);
			this->splitContainer1->SplitterDistance = 311;
			this->splitContainer1->TabIndex = 6;
			// 
			// listView1
			// 
			this->listView1->CheckBoxes = true;
			this->listView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(5) {this->ѡ��, this->����Ա, this->IP, 
				this->Socket, this->��ע});
			this->listView1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listView1->Location = System::Drawing::Point(0, 0);
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(311, 404);
			this->listView1->TabIndex = 0;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->View = System::Windows::Forms::View::Details;
			// 
			// ѡ��
			// 
			this->ѡ��->Text = L"ѡ��";
			// 
			// ����Ա
			// 
			this->����Ա->Text = L"����Ա";
			// 
			// IP
			// 
			this->IP->Text = L"IP";
			// 
			// Socket
			// 
			this->Socket->Text = L"Socket";
			// 
			// ��ע
			// 
			this->��ע->Text = L"���";
			// 
			// splitContainer2
			// 
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 0);
			this->splitContainer2->Name = L"splitContainer2";
			this->splitContainer2->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Controls->Add(this->richTextBox1);
			// 
			// splitContainer2.Panel2
			// 
			this->splitContainer2->Panel2->Controls->Add(this->button1);
			this->splitContainer2->Panel2->Controls->Add(this->label1);
			this->splitContainer2->Size = System::Drawing::Size(434, 404);
			this->splitContainer2->SplitterDistance = 249;
			this->splitContainer2->TabIndex = 0;
			// 
			// richTextBox1
			// 
			this->richTextBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->richTextBox1->Location = System::Drawing::Point(0, 0);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(434, 249);
			this->richTextBox1->TabIndex = 0;
			this->richTextBox1->Text = L"";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(327, 85);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(133, 31);
			this->button1->TabIndex = 1;
			this->button1->Text = L"ֹͣ";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(15, 14);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"��ǰ����";
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1000;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::TimerTick);
			// 
			// toolStripButton6
			// 
			this->toolStripButton6->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton6.Image")));
			this->toolStripButton6->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton6->Name = L"toolStripButton6";
			this->toolStripButton6->Size = System::Drawing::Size(49, 22);
			this->toolStripButton6->Text = L"ȫѡ";
			this->toolStripButton6->Click += gcnew System::EventHandler(this, &Form1::toolStripButton6_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(749, 453);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->menuStrip1);
			this->IsMdiContainer = true;
			this->Name = L"Form1";
			this->Text = L"�����ܿ�";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel2->ResumeLayout(false);
			this->splitContainer2->Panel2->PerformLayout();
			this->splitContainer2->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void ����ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
private: System::Void listView1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 //�������ӺͶϿ�
			 int nWnd = this->Handle.ToInt32();	
			 CTestTcpBusiness::getObj().SubscribePkg(CMDID_SvrTcpDisconn, nWnd);
			 CTestTcpBusiness::getObj().SubscribePkg(CMDID_SvrTcpCon, nWnd);

			 //װ�����еĽ���Ա
			 mTraderManager->Load();

		 }


		 protected:
	virtual void WndProc( Message% m ) override{
	if (  m.Msg == WM_TCP_MESSAGE  )
	   {
		  /* char *p = (char*)m.LParam.ToInt32();
		   Stru_UniPkgHead head;					
		   memset(&head, 0, sizeof(head));
		   memcpy(&head, p, sizeof(head));*/

		   switch(m.WParam.ToInt32()) {
			case CMDID_SvrTcpDisconn:
				{
					//ɾ��socket��Ӧ��item
					//���շ���Ľ���Ա					
					for(int i = listView1->Items->Count-1; i >= 0;i--)
					{		
						ListViewItem^ selection = listView1->Items[i];
						std::string lsUser = Tools::String2string(selection->SubItems[1]->Text);
						Int32 nShocket = Convert::ToInt32(selection->SubItems[3]->Text);
						Int32 nIdex = Convert::ToInt32(selection->SubItems[4]->Text);
						mTraderManager->SetTraderStatus(lsUser,false);
						if(selection->SubItems[3]->Text == m.LParam.ToString())
							listView1->Items->RemoveAt(i);
					}
					
				}
				break;	
			case CMDID_SvrTcpCon:
				{
					std::string lsIp;
					CTestTcpBusiness::getObj().GetIPAddress(m.LParam.ToInt32(),lsIp);
					//���socket��Ӧ�Ľ���Ա
					for(int i = 0; i < mTraderManager->mnMaxNode;i++)
					{
						ListViewItem ^ lItem = gcnew ListViewItem();
						lItem->Checked = false;						
						lItem->SubItems->Add("δ����");
						lItem->SubItems->Add(Tools::string2String(lsIp.c_str()));
						lItem->SubItems->Add(m.LParam.ToString());
						lItem->SubItems->Add(i.ToString());
						listView1->Items->Add(lItem);
					}
				
				}
				break;	
		   };
	   }
	   Form::WndProc ( m );
	}
private: System::Void toolStripButton1_Click(System::Object^  sender, System::EventArgs^  e) {
			 //�Զ����佻��Ա
			 for(int i = 0; i < listView1->Items->Count;i++)
			 {		
				 ListViewItem^ selection = listView1->Items[i];
				 if(selection->SubItems[1]->Text == "δ����")
				 {
					 TraderInfo lTraderInfo;
					 mTraderManager->FetchOneAviableTrader(lTraderInfo);
					 if(!lTraderInfo.msName.empty())
					 {
						 selection->SubItems[1]->Text = Tools::string2String(lTraderInfo.msName.c_str());
						 mTraderManager->SetTraderStatus(lTraderInfo.msName,true);

					 }
				 }
			 }
		 }
private: System::Void toolStripButton2_Click(System::Object^  sender, System::EventArgs^  e) {
			 //���³�ʼ��
			 CTestTcpBusiness::getObj().BroadcastPkgData(CMDID_REINIT,NULL,0,0);
		 }
private: System::Void toolStripButton3_Click(System::Object^  sender, System::EventArgs^  e) {
			 //����
			 for each(ListViewItem ^ lItem in listView1->Items)
			 {
					if(lItem->Checked != false && lItem->SubItems[1]->Text != "δ����" )
					{
						std::string lsUser = Tools::String2string(lItem->SubItems[1]->Text);
						Int32 nShocket = Convert::ToInt32(lItem->SubItems[3]->Text);
						Int32 nIndex = Convert::ToInt32(lItem->SubItems[4]->Text);
						std::string lsPass ;
						mTraderManager->GetTraderPassword(lsUser,lsPass);
						sTrader lTrader;
						strcpy(lTrader.msName,lsUser.c_str());
						strcpy(lTrader.msPass,lsPass.c_str());
						CTestTcpBusiness::getObj().SendPkgData(nShocket,CMDID_LOGIN,(void*)&lTrader,sizeof(lTrader),0,0,nShocket,nIndex);
					}
										
			 }			
			
		 }
private: System::Void toolStripButton4_Click(System::Object^  sender, System::EventArgs^  e) {
			 //�ر�
			 for each(ListViewItem ^ lItem in listView1->Items)
			 {
				 if(lItem->Checked != false && lItem->SubItems[1]->Text != "δ����" )
					{
						std::string lsUser = Tools::String2string(lItem->SubItems[1]->Text);
						Int32 nShocket = Convert::ToInt32(lItem->SubItems[3]->Text);
						Int32 nIndex = Convert::ToInt32(lItem->SubItems[4]->Text);
						
						CTestTcpBusiness::getObj().SendPkgData(nShocket,CMDID_LOGOFF,(void*)lsUser.c_str(),lsUser.length(),0,0,nShocket,nIndex);
					}

			 }			
		 }
private: System::Void toolStripButton5_Click(System::Object^  sender, System::EventArgs^  e) {
			 //�µ�
			 OrderForm ^ ldlg = gcnew OrderForm();
			 ldlg->SetCommand(mpsCommand);
			 std::set<std::string> lsetProduct ;
			 mTraderManager->GetProductSet(lsetProduct);
			 std::set<std::string>::iterator lIter = lsetProduct.begin();
			 for(;lIter != lsetProduct.end();lIter++)
			 {
				 ldlg->AddProduct(Tools::string2String(lIter->c_str()));
			 }
			 if(::DialogResult::OK == ldlg->ShowDialog())
			 {
				 if(mCommandManager->IsRuning())
				 {
					 MessageBox::Show("��һ������û��ִ�н���","��ʾ");
				     return;
				 }

				 mpsCommand->Clear();
				 //���ѡ����û�
				 for each(ListViewItem ^ lItem in listView1->Items)
				 {
					 if(lItem->Checked != false && lItem->SubItems[1]->Text != "δ����" )
					 {
						 sUserAndSocket ltempUserAndSocket;
						 ltempUserAndSocket.msTraderName = Tools::String2string(lItem->SubItems[1]->Text);
						 ltempUserAndSocket.mnSocket = Convert::ToInt32(lItem->SubItems[3]->Text);
						 ltempUserAndSocket.mnIndex = Convert::ToInt32(lItem->SubItems[4]->Text);
						 mpsCommand->AddUserAndSocket(ltempUserAndSocket);						
					 }
				 }	
				 if(mpsCommand->msDestionTrader.size() == 0)
				 {					 
					 MessageBox::Show("û��ѡ���û��������޷�����","��ʾ");
					 return;
				 }
				 mCommandManager->SetCurrentCommand(*mpsCommand);
				 mCommandManager->Start();
				 richTextBox1->Text = "";
			 }
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 //ֹͣ
			 if(!mCommandManager->IsRuning())
			 {
				 MessageBox::Show("��ǰû���������е���������ֹͣ","��ʾ");
				 return;
			 }

			// mCommandManager->SetCurrentCommand(mpsCommand);
			 mCommandManager->End();
		 }
private: System::Void �鿴���н���ԱToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 TraderList ^ ldlg = gcnew TraderList();
			 std::map<std::string,TraderInfo> lTraderMap ;
			 mTraderManager->GetAllTraderInfo(lTraderMap);
			 std::map<std::string,TraderInfo>::iterator lIter = lTraderMap.begin();
			 for(;lIter != lTraderMap.end();lIter++)
			 {
				 ldlg->AddTrader(Tools::string2String(lIter->first.c_str()),lIter->second.mnAllocNum);
			 }
			 if(::DialogResult::OK == ldlg->ShowDialog())
			 {
				 //sCommand ls = 
			 }
		 }
private: System::Void �˳�ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
private: System::Void TimerTick(System::Object^  sender, System::EventArgs^  e) {
			 //
			 std::vector<sOrderAction> lvecAction;
			 if(mCommandManager->FetchCustomOrderAction(lvecAction))		
			 {
				 std::string nDesc;
				 mCommandManager->GetDesc(nDesc);
				 label1->Text = Tools::string2String(nDesc.c_str());
				 button1->Enabled = true;
				// std::vector<sOrderAction> lvecAction;
				// if(mCommandManager->FetchCustomOrderAction(lvecAction))
				 {
					 for(int i = 0; i < lvecAction.size(); i++)
					 {
						 char lsTempDesc[1024] = {0};
						 //char ls1[10] =  lvecAction[i].mnOrder == OrderType_OrderInput ? "�µ�":"����";
						 //char ls2[10] =  lvecAction[i].mnOpenClose == OpenCloseType_Open ?"��":"ƽ";
						 sprintf(lsTempDesc, "����һ���������� �û� %s,Socket:%d,Index:%d, %s,%s,Product:%s\n",
							 lvecAction[i].mUserName,
							 lvecAction[i].mnSocket,
							 lvecAction[i].mnIndex,
							 lvecAction[i].mnOrder == OrderType_OrderInput ? "�µ�":"����",
							 lvecAction[i].mnOpenClose == OpenCloseType_Open ?"��":"ƽ",
							 lvecAction[i].mProduct);
						 richTextBox1->AppendText(Tools::string2String(lsTempDesc));
						 CTestTcpBusiness::getObj().SendPkgData(lvecAction[i].mnSocket,CMDID_ORDER,(void*)&lvecAction[i],sizeof(sOrderAction),0);

					 }
				 }
			 }
			 else
			 {
				// label1->Text = "";
				 button1->Enabled = false;
			 }
		 }
private: System::Void toolStripButton6_Click(System::Object^  sender, System::EventArgs^  e) {
			 if( toolStripButton6->Text == "ȫѡ")
			 {
				 for each(ListViewItem ^ lItem in listView1->Items)
				 {
					 lItem->Checked = true;					 	
				 }	
				 toolStripButton6->Text = "ȡ��ȫѡ";			
			 }
			 else
			 {
				 for each(ListViewItem ^ lItem in listView1->Items)
				 {
					 lItem->Checked = false;					 	
				 }	
				 toolStripButton6->Text = "ȫѡ";		
			 }
			
		 }
};
}

