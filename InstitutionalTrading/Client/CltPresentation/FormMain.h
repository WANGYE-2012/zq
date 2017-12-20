#pragma once
#include "RiskMapForm.h"
#include "ReportForm.h"
#include "ReportDataProviderFactory.h"
#include "RiskCombinationScreenSelect.h"
#include "RiskMapData.h"
namespace CltPresentation {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

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
			mnDefaultItem = 0;
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
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	protected: 
	private: System::Windows::Forms::ToolStripMenuItem^  ϵͳToolStripMenuItem;



	private: System::Windows::Forms::ToolStripMenuItem^  ExitToolStripMenuItem;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ͳ��ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  Ȩ�漨ЧToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ���׼�ЧToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �ֲ�ͳ��ToolStripMenuItem;	
	private: System::Windows::Forms::ToolStripMenuItem^  չ��ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  ��ͼչ��ToolStripMenuItem;



	private:Int32 mnDefaultItem;
	private: System::Windows::Forms::Timer^  timer1;
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
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->ϵͳToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ExitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ͳ��ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Ȩ�漨ЧToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->���׼�ЧToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�ֲ�ͳ��ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->չ��ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->��ͼչ��ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->menuStrip1->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->ϵͳToolStripMenuItem, 
				this->ͳ��ToolStripMenuItem, this->չ��ToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(682, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// ϵͳToolStripMenuItem
			// 
			this->ϵͳToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->����ToolStripMenuItem, 
				this->ExitToolStripMenuItem});
			this->ϵͳToolStripMenuItem->Name = L"ϵͳToolStripMenuItem";
			this->ϵͳToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->ϵͳToolStripMenuItem->Text = L"ϵͳ";
			// 
			// ����ToolStripMenuItem
			// 
			this->����ToolStripMenuItem->Name = L"����ToolStripMenuItem";
			this->����ToolStripMenuItem->Size = System::Drawing::Size(94, 22);
			this->����ToolStripMenuItem->Text = L"����";
			this->����ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::����ToolStripMenuItem_Click);
			// 
			// ExitToolStripMenuItem
			// 
			this->ExitToolStripMenuItem->Name = L"ExitToolStripMenuItem";
			this->ExitToolStripMenuItem->Size = System::Drawing::Size(94, 22);
			this->ExitToolStripMenuItem->Text = L"�˳�";
			this->ExitToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::ExitToolStripMenuItem_Click);
			// 
			// ͳ��ToolStripMenuItem
			// 
			this->ͳ��ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->Ȩ�漨ЧToolStripMenuItem, 
				this->���׼�ЧToolStripMenuItem, this->�ֲ�ͳ��ToolStripMenuItem});
			this->ͳ��ToolStripMenuItem->Name = L"ͳ��ToolStripMenuItem";
			this->ͳ��ToolStripMenuItem->Size = System::Drawing::Size(65, 20);
			this->ͳ��ToolStripMenuItem->Text = L"��ʷͳ��";
			// 
			// Ȩ�漨ЧToolStripMenuItem
			// 
			this->Ȩ�漨ЧToolStripMenuItem->Name = L"Ȩ�漨ЧToolStripMenuItem";
			this->Ȩ�漨ЧToolStripMenuItem->Size = System::Drawing::Size(118, 22);
			this->Ȩ�漨ЧToolStripMenuItem->Text = L"Ȩ�漨Ч";
			this->Ȩ�漨ЧToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::Ȩ�漨ЧToolStripMenuItem_Click);
			// 
			// ���׼�ЧToolStripMenuItem
			// 
			this->���׼�ЧToolStripMenuItem->Name = L"���׼�ЧToolStripMenuItem";
			this->���׼�ЧToolStripMenuItem->Size = System::Drawing::Size(118, 22);
			this->���׼�ЧToolStripMenuItem->Text = L"���׼�Ч";
			this->���׼�ЧToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::���׼�ЧToolStripMenuItem_Click);
			// 
			// �ֲ�ͳ��ToolStripMenuItem
			// 
			this->�ֲ�ͳ��ToolStripMenuItem->Name = L"�ֲ�ͳ��ToolStripMenuItem";
			this->�ֲ�ͳ��ToolStripMenuItem->Size = System::Drawing::Size(118, 22);
			this->�ֲ�ͳ��ToolStripMenuItem->Text = L"�ֲ�ͳ��";
			this->�ֲ�ͳ��ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::�ֲ�ͳ��ToolStripMenuItem_Click);
			// 
			// չ��ToolStripMenuItem
			// 
			this->չ��ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->��ͼչ��ToolStripMenuItem});
			this->չ��ToolStripMenuItem->Name = L"չ��ToolStripMenuItem";
			this->չ��ToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->չ��ToolStripMenuItem->Text = L"չ��";
			// 
			// ��ͼչ��ToolStripMenuItem
			// 
			this->��ͼչ��ToolStripMenuItem->Name = L"��ͼչ��ToolStripMenuItem";
			this->��ͼչ��ToolStripMenuItem->Size = System::Drawing::Size(118, 22);
			this->��ͼչ��ToolStripMenuItem->Text = L"��ͼչ��";
			this->��ͼչ��ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::��ͼչ��ToolStripMenuItem_Click);
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripStatusLabel1});
			this->statusStrip1->Location = System::Drawing::Point(0, 478);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(682, 22);
			this->statusStrip1->TabIndex = 1;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(41, 17);
			this->toolStripStatusLabel1->Text = L"�ѵ�¼";
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &FormMain::timer1_Tick);
			// 
			// FormMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(682, 500);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"FormMain";
			this->Text = L"�������װ汾ͳ��չ���ն�";
			this->Load += gcnew System::EventHandler(this, &FormMain::FormMain_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		//�˳�
	private: System::Void ExitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 Close();
			 }
			 //��¼

	private: System::Void ��ͼչ��ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 RiskMapForm^ lForm = gcnew RiskMapForm();
				 lForm->ShowDialog();
			 }
	private: System::Void Ȩ�漨ЧToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

				 ReportForm^ lReportDialog = gcnew ReportForm();
				 lReportDialog->Text = "�ʻ�Ȩ�漨Чͳ��";
				 lReportDialog->DataProvider = CAccountInterestsDataProviderFactory::Instance()->CreateDataProvider();	
				 lReportDialog->ReportConrtolConfigGuid = "{9CBB8291-655B-499a-BF46-A5ACB7DA6F92}";
				 if(lReportDialog->ShowDialog() == ::DialogResult::OK)
				 {
				 }
			 }
	private: System::Void ���׼�ЧToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 ReportForm^ lReportDialog = gcnew ReportForm();
				 lReportDialog->Text = "�ʻ����׼�Чͳ��";
				 lReportDialog->DataProvider =  CAccountTradesReportDataProviderFactory::Instance()->CreateDataProvider();
				 lReportDialog->ReportConrtolConfigGuid = "{D9E386DA-B1A6-4a07-A65B-8E6D64EDAD8D}";				
				 if(lReportDialog->ShowDialog() == ::DialogResult::OK)
				 {
				 }
			 }
	private: System::Void �ֲ�ͳ��ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 ReportForm^ lReportDialog = gcnew ReportForm();
				 lReportDialog->Text = "�ֲ�ͳ��";
				 lReportDialog->DataProvider =  CAccountHodeReportDataProviderFactory::Instance()->CreateDataProvider();
                 lReportDialog->ReportConrtolConfigGuid = "{{5D2F1664-DCB3-48e1-9A21-8089AEBA3F36}}";			
				 if(lReportDialog->ShowDialog() == ::DialogResult::OK)
				 {
				 }
			 }
	private: System::Void ����ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 RiskCombinationScreenSelect ^ lDlg = gcnew RiskCombinationScreenSelect();
				 if(lDlg->ShowDialog() == ::DialogResult::OK)
			  {
			  }

			 }
	private: System::Void FormMain_Load(System::Object^  sender, System::EventArgs^  e) {
				 int nWnd = this->Handle.ToInt32();
				 CTcpLayer::SetMainWndHandle(nWnd);
				 CTcpLayer::SubscribePkg(CMDID_GetALLError_Push, nWnd);

				 this->timer1->Interval = 100;
				 this->timer1->Start();
				



			 }
			 //����WndProc���Դ���WM_USER_Win��Ϣ
	protected:virtual void WndProc( Message% m ) override
			  {

				  if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_DataArrival )
				  {
					  CTcpLayer::ParseRecvPackageInWindowThread();						
				  }			
				  else if( m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_ConnStatus )
				  {
					  int nConnectStatus = m.LParam.ToInt32();
					  switch(nConnectStatus)
					  {
					  case 0:
						  toolStripStatusLabel1->Text = "��������Ͽ�����,�����µ�¼";
						  break;
					  case 1:
						  //toolStripStatusLabel1->Text = "�뽻�׷�����������...";
						  break;
					  case 2:
						  toolStripStatusLabel1->Text = "����������ӳɹ�";
						  break;
					  case 3:
						  //toolStripStatusLabel1->Text = "�뽻�׷���������ʧ��,,�����µ�¼";
						  break;
					  }
				  } 
				  else if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival )
				  {
					  char *p = (char*)m.LParam.ToInt32();
					  Stru_UniPkgHead head;					
					  memset(&head, 0, sizeof(head));
					  memcpy(&head, p, sizeof(head));

				
					  switch ( head.cmdid )
					  {	 
					  case CMDID_GetALLError_Push:
						  {     
							  char szError[256];
							  if ( head.userdata3 == CF_ERROR_SUCCESS )
							  {
								  std::vector<sErrorInfo> lvecError;
								  int nCount = head.len / sizeof(sErrorInfo);
								  for ( int i = 0; i < nCount; i++ )
								  {
									  memset(szError,0,sizeof(szError));
									  sErrorInfo* pErrorInfo = (sErrorInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(sErrorInfo));
									  switch (pErrorInfo->meErrorType)
									  {
									  case FirstLevelError:
										  sprintf(szError,"%s: %s","1������Ӱ��ϵͳ���еĴ���",pErrorInfo->msError); 
										  break;
									  case SecondLevelError:
										  sprintf(szError,"%s: %s","2�����󣬲�Ӱ��ϵͳ���еĴ���",pErrorInfo->msError); 
										  break;
									  case ThirdLevelError:
										  sprintf(szError,"%s: %s","3�����󣬲�Ӱ��ϵͳ����",pErrorInfo->msError); 
										  break;
									  case OperationLogNeedSend:
										  sprintf(szError,"%s: %s","ϵͳ������־�����͵������Ŀͻ���������־",pErrorInfo->msError); 
										  break;
									  }  			
									  _GetPopMsg()->SetTitle("չ��ϵͳ��Ϣ");
									  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
										  CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
										  szError);
								  }

							  }							
							  break;
						  }
					  default:break;
					  }

					
						
				  }
				  Form::WndProc ( m );
			  }
	private:System::Void LoadAutoConfig(cliext::vector<ScreenConfig^>^&  nScreensConfig)
			{
				/*
				<Screens>
				<Screen Type="1" OrgId="2" OrgName="����" PrdID="" ProName="">      
				</Screen>
				<Screen Type="2" OrgId="2" OrgName="����" ProID="33" ProName="���1">      
				</Screen>
				<Screen Type="2" OrgId="2" OrgName="����" ProID="33" ProName="���1">      
				</Screen>
				<Screen Type="3" OrgId="" OrgName="" ProID="" ProName="">    //��ͼ   
				</Screen>
				</Screens>
				*/
				//���4��Ĭ�ϵ�
				System::Xml::XmlDocument ^ mXmlDoc = gcnew System::Xml::XmlDocument();
				nScreensConfig->clear();			
				try
				{
					String ^ path = System::Windows::Forms::Application::StartupPath + "\\Screens.xml";
					mXmlDoc->Load(path);
				}
				catch(Exception ^ e)
				{
					return;
				}

				XmlNode ^ lDefaultNode = mXmlDoc->SelectSingleNode("/Screens");
				if(lDefaultNode != nullptr)
				{
					mnDefaultItem =  Convert::ToInt32(lDefaultNode->Attributes["Default"]->Value);
				}


				int i = 0;
				XmlNodeList^ lScreenNodeList = mXmlDoc->SelectNodes("/Screens/Screen");
				if(lScreenNodeList != nullptr)
				{
					for each (XmlNode^ lScreenNode in lScreenNodeList)
				 {

					 ScreenConfig ^ lConfig = gcnew ScreenConfig();

					 lConfig->mnType =  Convert::ToInt32(lScreenNode->Attributes["Type"]->Value);
					 lConfig->mnOrgID =  Convert::ToInt32(lScreenNode->Attributes["OrgId"]->Value);
					 lConfig->msOrgName =  lScreenNode->Attributes["OrgName"]->Value;
					 lConfig->mnProID =  Convert::ToInt32(lScreenNode->Attributes["ProId"]->Value);
					 lConfig->msProName =  lScreenNode->Attributes["ProName"]->Value;	

					 nScreensConfig->push_back(lConfig);
				 }
				}

			}

	public:void ShowDailogWithScreenConfig(ScreenConfig ^  nConfig,bool bAdjustSize,int nIndex)
		   {
			   System::Drawing::Size lSize =
				   System::Drawing::Size(System::Windows::Forms::Screen::PrimaryScreen->WorkingArea.Width/2,
				   System::Windows::Forms::Screen::PrimaryScreen->WorkingArea.Height/2);

			   ScreenConfig ^ lConfig = nConfig;
			   int i = nIndex;
			   if(lConfig)
			   {
				   if(lConfig->mnType == 1)
				   {
					   //��֯����
					   DepartmentShow ^ ltempShowForm = gcnew DepartmentShow();
					   ltempShowForm->CurrentOrgID = lConfig->mnOrgID;
					   ltempShowForm->CurrentOrgName = lConfig->msOrgName;
					   if(bAdjustSize)
						   ltempShowForm->CombinationScreenSetting( Point( (i%2)*lSize.Width ,(i/2)*lSize.Height ),lSize);
					   ltempShowForm->Show();


				   }
				   if(lConfig->mnType == 2)
				   {
					   //��Ʋ�Ʒ
					   ProductShow ^ lp = gcnew ProductShow();
					   lp->CurrentOrgID = lConfig->mnOrgID;
					   lp->CurrentProductID =lConfig->mnProID;
					   lp->CurrentProductName = lConfig->msProName;
					   if(bAdjustSize)
						   lp->CombinationScreenSetting( Point( (i%2)*lSize.Width ,(i/2)*lSize.Height ),lSize);
					   lp->Show();

				   }
				   if(lConfig->mnType == 3)
				   {
					   //��Ʋ�Ʒ
					   RiskMapForm ^ lp = gcnew RiskMapForm();
					   /*	 lp->CurrentOrgID = lConfig->mnOrgID;
					   lp->CurrentProductID =lConfig->mnProID;
					   lp->CurrentProductName = lConfig->msProName;*/
					   if(bAdjustSize)
						   lp->CombinationScreenSetting( Point( (i%2)*lSize.Width ,(i/2)*lSize.Height ),lSize);
					   lp->Show();

				   }
			   }
		   }

	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
				 //
				 this->timer1->Stop();
				 cliext::vector<ScreenConfig^>^ nScreensConfig = gcnew cliext::vector<ScreenConfig^>();
				 LoadAutoConfig(nScreensConfig);

				 if(mnDefaultItem == 1)
				 {
					 RiskMapForm^ lForm = gcnew RiskMapForm();
					 lForm->ShowDialog();
				 }
				 else if(mnDefaultItem == 2)
				 {
					 ShowDailogWithScreenConfig(nScreensConfig[0],false,0);
				 }
				 else if(mnDefaultItem == 3)
				 {
					 if(nScreensConfig->size() != 4 && nScreensConfig->size() > 0)
					 {
						 ShowDailogWithScreenConfig(nScreensConfig[0],false,0);

					 }
					 else if(nScreensConfig->size() == 4)
					 {
						 for(int i = 0 ; i < 4 ; i ++)
						 {				  
							 ShowDailogWithScreenConfig(nScreensConfig[i],true,i);
						 }
					 }

				 }
			 }
};
}

