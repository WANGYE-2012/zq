#pragma once
#include "DepartProductItem.h"
#include "DepartProductTypeItem.h"
#include "RiskMapData.h"
#include "ProductAccountItem.h"
#include "CDataInfo.h"
#include "Tools.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
#include <cliext/map>

namespace CltPresentation {

	/// <summary>
	/// Summary for DepartmentShow
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class DepartmentShow : public System::Windows::Forms::Form
	{
	public:
		DepartmentShow(void)
		{
			InitializeComponent();
			m_pdataInfo = CDataInfo::NewInstance();
			mRiskMapData = CRiskMapData::NewInstance();
			//mMapProductItemUI = gcnew cliext::multimap<int,DepartProductItem^>();
			mMapProductTypeItemUI = gcnew cliext::multimap<String^,DepartProductTypeItem^>();
			mMapAccountItemUI = gcnew cliext::multimap<int,ProductAccountItem^>();
			Random^ rand = gcnew Random(Guid::NewGuid().GetHashCode());
			mnSeqID = rand->Next(1000000);
			mbCombinationScreen = false;
			//
			//TODO: Add the constructor code here
			//
		}
	private: bool mbCombinationScreen;
	private: System::Drawing::Point mbCombinationScreenPoint;
 	private: System::Drawing::Size  mbCombinationScreenSize;
	private: Int32 mnSeqID;
    private: CDataInfo* m_pdataInfo;
	private: CRiskMapData * mRiskMapData;
	private: Int32 mnCurrentOrgID;
	private: String^ msCurrentOrgName;
	//private: cliext::multimap<int,DepartProductItem^> ^ mMapProductItemUI;//��Ʋ�ƷUI MAP
	private: System::Windows::Forms::Panel^  panel6;
	private: System::Windows::Forms::Label^  label2;
	private: static Int32 mnMaxAccount = 20;
	private: cliext::multimap<String^,DepartProductTypeItem^> ^ mMapProductTypeItemUI;//Ʒ��UI MAP
	private: cliext::multimap<int,ProductAccountItem^> ^ mMapAccountItemUI;//�ʻ�UI map

	public: property Int32 CurrentOrgID
    {
	   Int32 get() {return mnCurrentOrgID;}
	   void set(Int32 mnID) {mnCurrentOrgID = mnID;}
    }
	public: property String^ CurrentOrgName
	{
		String^ get() {return msCurrentOrgName;}
		void set(String^ nsVal) {msCurrentOrgName = nsVal;}
				
	}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~DepartmentShow()
		{
			if(mRiskMapData)
				CRiskMapData::ReleaseInstance(mRiskMapData);
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
    private: System::Windows::Forms::PictureBox^  pictureBox3;

	protected: 
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;
	private: System::Windows::Forms::Panel^  panel2;
	private: ::SimpleGuage::SimpleGuage^  simpleGuage1;
	private: ListPanel::ListPanelControl^  listPanelControl1;
	private: ListPanel::ListPanelControl^  listPanelControl2;

	private: System::Windows::Forms::Panel^  panel3;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Panel^  panel4;
	private: System::Windows::Forms::Panel^  panel5;
	private: System::Windows::Forms::PictureBox^  pictureBox4;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(DepartmentShow::typeid));
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->listPanelControl1 = (gcnew ListPanel::ListPanelControl());
			this->listPanelControl2 = (gcnew ListPanel::ListPanelControl());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->simpleGuage1 = (gcnew ::SimpleGuage::SimpleGuage());
			this->panel4 = (gcnew System::Windows::Forms::Panel());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->panel5 = (gcnew System::Windows::Forms::Panel());
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			this->panel6 = (gcnew System::Windows::Forms::Panel());
			this->label2 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->BeginInit();
			this->panel1->SuspendLayout();
			this->panel4->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox4))->BeginInit();
			this->panel6->SuspendLayout();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.BackgroundImage")));
			this->pictureBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->pictureBox1->Location = System::Drawing::Point(0, 3);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(79, 74);
			this->pictureBox1->TabIndex = 6;
			this->pictureBox1->TabStop = false;
			// 
			// chart1
			// 
			this->chart1->BackColor = System::Drawing::Color::Transparent;
			chartArea1->AxisX->LabelStyle->ForeColor = System::Drawing::Color::White;
			chartArea1->AxisX->LineColor = System::Drawing::Color::White;
			chartArea1->AxisX->MajorGrid->LineWidth = 0;
			chartArea1->AxisY->LabelStyle->ForeColor = System::Drawing::Color::White;
			chartArea1->AxisY->LineColor = System::Drawing::Color::White;
			chartArea1->AxisY->MajorGrid->LineWidth = 0;
			chartArea1->BackColor = System::Drawing::Color::Transparent;
			chartArea1->BackSecondaryColor = System::Drawing::Color::White;
			chartArea1->BorderColor = System::Drawing::Color::Red;
			chartArea1->BorderWidth = 0;
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			this->chart1->Location = System::Drawing::Point(636, 541);
			this->chart1->Name = L"chart1";
			series1->BackGradientStyle = System::Windows::Forms::DataVisualization::Charting::GradientStyle::TopBottom;
			series1->BorderColor = System::Drawing::Color::Transparent;
			series1->ChartArea = L"ChartArea1";
			series1->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			series1->IsValueShownAsLabel = true;
			series1->LabelBackColor = System::Drawing::Color::White;
			series1->LabelForeColor = System::Drawing::Color::Red;
			series1->Name = L"Series1";
			series1->SmartLabelStyle->CalloutLineColor = System::Drawing::Color::White;
			this->chart1->Series->Add(series1);
			this->chart1->Size = System::Drawing::Size(455, 214);
			this->chart1->TabIndex = 9;
			this->chart1->Text = L"chart1";
			// 
			// panel2
			// 
			this->panel2->BackColor = System::Drawing::Color::Transparent;
			this->panel2->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel2.BackgroundImage")));
			this->panel2->Location = System::Drawing::Point(330, 175);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(68, 491);
			this->panel2->TabIndex = 13;
			// 
			// listPanelControl1
			// 
			this->listPanelControl1->AutoScroll = true;
			this->listPanelControl1->BackColor = System::Drawing::Color::Transparent;
			this->listPanelControl1->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
			this->listPanelControl1->Location = System::Drawing::Point(636, 227);
			this->listPanelControl1->Name = L"listPanelControl1";
			this->listPanelControl1->Size = System::Drawing::Size(242, 239);
			this->listPanelControl1->TabIndex = 11;
			this->listPanelControl1->Vertical = true;
			this->listPanelControl1->WrapContents = false;
			// 
			// listPanelControl2
			// 
			this->listPanelControl2->AutoScroll = true;
			this->listPanelControl2->BackColor = System::Drawing::Color::Transparent;
			this->listPanelControl2->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
			this->listPanelControl2->Location = System::Drawing::Point(884, 227);
			this->listPanelControl2->Name = L"listPanelControl2";
			this->listPanelControl2->Size = System::Drawing::Size(296, 239);
			this->listPanelControl2->TabIndex = 12;
			this->listPanelControl2->Vertical = true;
			this->listPanelControl2->WrapContents = false;
			// 
			// panel3
			// 
			this->panel3->BackColor = System::Drawing::Color::Transparent;
			this->panel3->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel3.BackgroundImage")));
			this->panel3->Location = System::Drawing::Point(1201, 227);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(61, 366);
			this->panel3->TabIndex = 14;
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::Transparent;
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->pictureBox1);
			this->panel1->Controls->Add(this->simpleGuage1);
			this->panel1->Location = System::Drawing::Point(3, 172);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(321, 349);
			this->panel1->TabIndex = 10;
			// 
			// label1
			// 
			this->label1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->label1->Font = (gcnew System::Drawing::Font(L"SimSun", 36, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label1->ForeColor = System::Drawing::Color::DarkGoldenrod;
			this->label1->Location = System::Drawing::Point(3, 284);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(312, 58);
			this->label1->TabIndex = 7;
			this->label1->Text = L"�ܷ��ն�";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// simpleGuage1
			// 
			this->simpleGuage1->BackColor = System::Drawing::Color::Transparent;
			this->simpleGuage1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"simpleGuage1.BackgroundImage")));
			this->simpleGuage1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->simpleGuage1->BigArcCount = 300;
			this->simpleGuage1->BigArcStart = 120;
			this->simpleGuage1->BigNeedleRadius = 95;
			this->simpleGuage1->Location = System::Drawing::Point(66, 63);
			this->simpleGuage1->Name = L"simpleGuage1";
			this->simpleGuage1->Size = System::Drawing::Size(202, 202);
			this->simpleGuage1->SmallArcCount = 270;
			this->simpleGuage1->SmallArcStart = 120;
			this->simpleGuage1->SmallNeedleRadius = 58;
			this->simpleGuage1->TabIndex = 5;
			this->simpleGuage1->Value = 0;
			// 
			// panel4
			// 
			this->panel4->BackColor = System::Drawing::Color::Transparent;
			this->panel4->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel4.BackgroundImage")));
			this->panel4->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->panel4->Controls->Add(this->pictureBox2);
			this->panel4->Location = System::Drawing::Point(3, 3);
			this->panel4->Name = L"panel4";
			this->panel4->Size = System::Drawing::Size(162, 136);
			this->panel4->TabIndex = 15;
			// 
			// pictureBox2
			// 
			this->pictureBox2->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox2.BackgroundImage")));
			this->pictureBox2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->pictureBox2->Location = System::Drawing::Point(35, 68);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(124, 65);
			this->pictureBox2->TabIndex = 0;
			this->pictureBox2->TabStop = false;
			// 
			// pictureBox3
			// 
			this->pictureBox3->BackColor = System::Drawing::Color::Transparent;
			this->pictureBox3->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox3.BackgroundImage")));
			this->pictureBox3->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->pictureBox3->Location = System::Drawing::Point(171, 74);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(119, 61);
			this->pictureBox3->TabIndex = 16;
			this->pictureBox3->TabStop = false;
			// 
			// panel5
			// 
			this->panel5->BackColor = System::Drawing::Color::Black;
			this->panel5->Location = System::Drawing::Point(3, 141);
			this->panel5->Name = L"panel5";
			this->panel5->Size = System::Drawing::Size(1269, 25);
			this->panel5->TabIndex = 18;
			// 
			// pictureBox4
			// 
			this->pictureBox4->BackColor = System::Drawing::Color::Transparent;
			this->pictureBox4->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox4.BackgroundImage")));
			this->pictureBox4->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->pictureBox4->Location = System::Drawing::Point(966, 3);
			this->pictureBox4->Name = L"pictureBox4";
			this->pictureBox4->Size = System::Drawing::Size(306, 136);
			this->pictureBox4->TabIndex = 19;
			this->pictureBox4->TabStop = false;
			this->pictureBox4->Click += gcnew System::EventHandler(this, &DepartmentShow::pictureBox4_Click);
			// 
			// panel6
			// 
			this->panel6->BackColor = System::Drawing::Color::Transparent;
			this->panel6->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panel6.BackgroundImage")));
			this->panel6->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->panel6->Controls->Add(this->label2);
			this->panel6->Location = System::Drawing::Point(3, 540);
			this->panel6->Name = L"panel6";
			this->panel6->Size = System::Drawing::Size(321, 260);
			this->panel6->TabIndex = 20;
			// 
			// label2
			// 
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft YaHei", 21.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label2->ForeColor = System::Drawing::Color::DarkOrange;
			this->label2->Location = System::Drawing::Point(9, 72);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(295, 54);
			this->label2->TabIndex = 0;
			this->label2->Text = L"����";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// DepartmentShow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(1440, 868);
			this->Controls->Add(this->pictureBox4);
			this->Controls->Add(this->panel5);
			this->Controls->Add(this->pictureBox3);
			this->Controls->Add(this->panel4);
			this->Controls->Add(this->chart1);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->listPanelControl1);
			this->Controls->Add(this->listPanelControl2);
			this->Controls->Add(this->panel3);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->panel6);
			//this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;
			this->Name = L"DepartmentShow";
			this->Text = L"�ʹܲ��Ż���չʾ";
			this->Load += gcnew System::EventHandler(this, &DepartmentShow::OnLoad);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &DepartmentShow::OnFormClosed);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel4->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox4))->EndInit();
			this->panel6->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void OnLoad(System::Object^  sender, System::EventArgs^  e) {
				 LayOut();
				 this->panel1->BackColor = Color::FromArgb(125,168,54,167);
				 this->listPanelControl1->BackColor = Color::FromArgb(125,43,166,30);
				 this->listPanelControl2->BackColor = Color::FromArgb(125,196,145,47);
				 this->listPanelControl2->Vertical = false;
				 this->pictureBox1->BackColor = Color::Transparent;
				 if(msCurrentOrgName != nullptr)
					 this->label2->Text = msCurrentOrgName;
				 //�����û�����
				  std::set<int> setOrgAccount;	
				 m_pdataInfo->GetTraderUserIDsByOrgID(mnCurrentOrgID, true,setOrgAccount);
				 std::set<int>::iterator lIterSett = setOrgAccount.begin();
				 for(;lIterSett != setOrgAccount.end();lIterSett++)
					 mRiskMapData->mStaticsAccountVector.push_back(*lIterSett);

				 //std::set<int> setResult;			
				 //std::set_intersection(setOrgAccount.begin(), setOrgAccount.end(), 
				 // setFinicalAccount.begin(), setFinicalAccount.end(), inserter(mRiskMapData->mStaticsAccountVector,mRiskMapData->mStaticsAccountVector.end()));
				 std::vector<int>::iterator lIte = mRiskMapData->mStaticsAccountVector.begin();
				 Int32 i = 1;
				 Int32 lnMax = (Int32)mRiskMapData->mStaticsAccountVector.size() > mnMaxAccount ? mnMaxAccount : (Int32)mRiskMapData->mStaticsAccountVector.size();
				 while(lIte != mRiskMapData->mStaticsAccountVector.end() && i <= lnMax )
				 { 
					 ProductAccountItem ^ lItem = gcnew ProductAccountItem();
					 UserInfo  lTraderInfo;
					 m_pdataInfo->GetUserInfoByUserID(*lIte,lTraderInfo);
					 lItem->AccountName = Tools::string2String(lTraderInfo.szAccount);
					 mMapAccountItemUI->insert(cliext::make_pair(*lIte,lItem));
					 this->listPanelControl1->Controls->Add(lItem);
					
					 lIte++;

				 }

				 //this->SuspendLayout();
				 //this->listPanelControl1->SuspendLayout();
				 ////������Ʋ�Ʒ����
				 //m_pdataInfo->GetFinancialProduct(mRiskMapData->mmapFinanProduct);
				 //std::set<int> mapProductID;
				 //m_pdataInfo->GetFinancialProduct( mnCurrentOrgID,  mapProductID);
				 //if(mapProductID.size()>0)
				 //{
					// std::set<int>::iterator itIns;
					// MAPFinancialProduct::iterator itInfo;
					// int i = 0;
					// for(itIns = mapProductID.begin(); itIns != mapProductID.end(); itIns++) 
					// {		
					//	 //get product name 
					//	 itInfo = mRiskMapData->mmapFinanProduct.find(*itIns);
					//	 if(itInfo != mRiskMapData->mmapFinanProduct.end())
					//	 {
					//		 DepartProductItem ^ lItem = gcnew DepartProductItem();
					//		 lItem->ProductName = Tools::string2String(itInfo->second.szName);
					//		 mMapProductItemUI->insert(cliext::make_pair(*itIns,lItem));
					//		 this->listPanelControl1->Controls->Add(lItem);
					//	 }
					//
					// }
				 //}
				/* for(int i = 0 ; i < 4; i++)
				 {
					
					 DepartProductItem ^ lItem = gcnew DepartProductItem();
					 this->listPanelControl1->Controls->Add(lItem);
					 
				 }*/
				
				/* this->listPanelControl2->SuspendLayout();
				 for(int i = 0 ; i < 4; i++)
				 {					 
					 DepartProductTypeItem ^ lItem = gcnew DepartProductTypeItem();
					 this->listPanelControl2->Controls->Add(lItem);					 
				 }
                 this->listPanelControl2->ResumeLayout(false);*/
				 //this->ResumeLayout();

				 //��������
				 SubscribeStatiacsData();
				 SubscribeAccountStatiacsData();
				// SubscribeFinicalProductStatiacsData(mnCurrentOrgID);
			 }

	private: System::Void LayOut() {
				  //�Ű�
				  this->SuspendLayout();
				 
				  Int32 lnMainXSplit = this->panel1->Location.X + this->panel1->Size.Width;
				  Int32 lnFirstYSplit = this->panel1->Location.Y ;

				  Int32 lnLeftX = this->Width - lnMainXSplit;
				  Int32 lnLeftY = this->Height - lnFirstYSplit;

				  System::Drawing::Size  lnProductPanelSize = System::Drawing::Size(476,350) ;
				  System::Drawing::Size  lnProductTypePanelSize = System::Drawing::Size(517,350) ;
				  System::Drawing::Size  lnCharSize = System::Drawing::Size(lnProductPanelSize.Width+lnProductTypePanelSize.Width + 2,this->ClientRectangle.Height - lnFirstYSplit - lnProductPanelSize.Height - 10) ;
				  Int32 lnOtherPaneWidth = (lnLeftX - lnProductPanelSize.Width - lnProductTypePanelSize.Width -10) / 2;
				 
				  this->panel5->Size = System::Drawing::Size(this->Width,this->panel5->Size.Height);
				  this->pictureBox4->Location = Point(this->ClientRectangle.Width - this->pictureBox4->Size.Width,this->pictureBox4->Location.Y);
				  if(lnOtherPaneWidth < 0)
				  {
					  this->panel2->Hide();
					  this->panel3->Hide();
					  this->listPanelControl1->Location = Point(lnMainXSplit+2,lnFirstYSplit);
					  this->listPanelControl1->Size = lnProductPanelSize;
					  this->listPanelControl2->Location = Point(this->listPanelControl1->Location.X + this->listPanelControl1->Size.Width +2,lnFirstYSplit);
					  this->listPanelControl2->Size = lnProductTypePanelSize;
					  this->chart1->Location = Point(lnMainXSplit+2,this->listPanelControl1->Location.Y + this->listPanelControl1->Size.Height +2);
					  this->chart1->Size = lnCharSize;
				  }
				  else
				  {
					  this->panel2->Show();
					  this->panel3->Show();
					  //����������ε�Pane�Ŀ�ȴ���20 �����20
					  if(lnOtherPaneWidth > 50)
						  lnOtherPaneWidth = 50;

					  this->panel2->Location = Point(lnMainXSplit+2,lnFirstYSplit);
					  this->panel2->Size = System::Drawing::Size(lnOtherPaneWidth,lnLeftY );

					  this->listPanelControl1->Location = Point(this->panel2->Location.X + this->panel2->Size.Width +2,lnFirstYSplit);
					  this->listPanelControl1->Size = lnProductPanelSize;

					  this->listPanelControl2->Location = Point(this->listPanelControl1->Location.X + this->listPanelControl1->Size.Width +2,lnFirstYSplit);
					 // this->listPanelControl2->Size = lnProductTypePanelSize; 
					  this->listPanelControl2->Size =  System::Drawing::Size(this->ClientRectangle.Width - this->listPanelControl2->Location.X - lnOtherPaneWidth - 2,lnProductTypePanelSize.Height) ;//X�Զ��Ŵ� 

					  this->panel3->Location = Point(this->listPanelControl2->Location.X + this->listPanelControl2->Size.Width +2,lnFirstYSplit);
					  this->panel3->Size = System::Drawing::Size(lnOtherPaneWidth,lnLeftY );
					 
				

				      this->chart1->Location = Point(this->listPanelControl1->Location.X,this->listPanelControl1->Location.Y + this->listPanelControl1->Size.Height +2);
					  lnCharSize = System::Drawing::Size(this->listPanelControl1->Size.Width+this->listPanelControl2->Size.Width + 2,this->ClientRectangle.Height - this->chart1->Location.Y -2) ;//Y�Զ��Ŵ�
					  this->chart1->Size = lnCharSize;
				  }
				  this->ResumeLayout(false);

			  }
private: System::Void SubscribeStatiacsData()
		  {
			  //����ͳ��
			  IntPtr hWnd=this->Handle;
			  CTcpLayer::SubscribePkg(Cmd_RM_SubscribeStatistics_Rsp,(int)hWnd);
			  CTcpLayer::SubscribePkg(Cmd_RM_Statistics_Push,(int)hWnd);

			  //��ȡ��Ҫ���ĵ���֯����Ʋ�Ʒ�Ĺ�ϵ����

			  AssetOrgProRelation re;
			  re.mnAssetMgmtOrgID = mnCurrentOrgID;
			  re.mnFinancialProductID=-1;
			  mRiskMapData->mStaticsVector.push_back(re);
				
			  if(mRiskMapData->mStaticsVector.size() > 0)
			  {				
				  if( !CTcpLayer::SendData(Cmd_RM_SubscribeStatistics_Req, &mRiskMapData->mStaticsVector[0],sizeof(AssetOrgProRelation)*mRiskMapData->mStaticsVector.size(), mnSeqID))
				  {				
					  // Windows::Forms::MessageBox::Show("���Ļ�������ʧ��", "��ʾ", Windows::Forms::MessageBoxIcon::Error);
					  MessageBox::Show("���Ļ�������ʧ��", "��ʾ",MessageBoxButtons::OK);
				  }
			  }

		  }


private: System::Void UnsubscribeStatiacsData()
		 {
			 //��ȡ��Ҫ���ĵ���֯����Ʋ�Ʒ�Ĺ�ϵ����

			 if(mRiskMapData->mStaticsVector.size() > 0)
			 {				
				 if( !CTcpLayer::SendData(Cmd_RM_UnSubscribeStatistics_Req, &mRiskMapData->mStaticsVector[0],sizeof(AssetOrgProRelation)*mRiskMapData->mStaticsVector.size(), mnSeqID))
				 {					
					 MessageBox::Show("�˶���������ʧ��", "��ʾ",MessageBoxButtons::OK);
				 }
			 }
		 }

private: System::Void SubscribeAccountStatiacsData()
		 {
			 //����ͳ��
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::SubscribePkg(Cmd_RM_SubscribeUserFund_Rsp, (int)hWnd);
			 CTcpLayer::SubscribePkg(Cmd_RM_UserFund_Push, (int)hWnd);
			 CTcpLayer::SubscribePkg(Cmd_RM_UnSubscribeUserFund_Rsp, (int)hWnd);


			 //
			 if(mRiskMapData->mStaticsAccountVector.size() > 0)
			 {				
				 if( !CTcpLayer::SendData(Cmd_RM_SubscribeUserFund_Req, &mRiskMapData->mStaticsAccountVector[0],sizeof(int)*mRiskMapData->mStaticsAccountVector.size(), mnSeqID))
				 {				
					 // Windows::Forms::MessageBox::Show("���Ļ�������ʧ��", "��ʾ", Windows::Forms::MessageBoxIcon::Error);
					 MessageBox::Show("���Ļ�������ʧ��", "��ʾ",MessageBoxButtons::OK);
				 }
			 }

		 }
private: System::Void UnsubscribeAccountStatiacsData()
		 {
			 //��ȡ��Ҫ���ĵ���֯����Ʋ�Ʒ�Ĺ�ϵ����

			 if(mRiskMapData->mStaticsAccountVector.size() > 0)
			 {				
				 if( !CTcpLayer::SendData(Cmd_RM_SubscribeUserFund_Req, &mRiskMapData->mStaticsAccountVector[0],sizeof(int)*mRiskMapData->mStaticsAccountVector.size(), 0))
				 {					
					 MessageBox::Show("�˶���������ʧ��", "��ʾ",MessageBoxButtons::OK);
				 }
			 }
		 }
//����WndProc���Դ���WM_USER_Win��Ϣ
protected:virtual void WndProc( Message% m ) override
	    {

			  if(m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival) 
			  {
				  char *p = (char*)m.LParam.ToInt32();
				  Stru_UniPkgHead head;					

				  memset(&head, 0, sizeof(head));
				  memcpy(&head, p, sizeof(head));
				  switch(head.cmdid) 
				  {
				   case Cmd_RM_SubscribeStatistics_Rsp:
					   {
						   if(head.userdata1 == CF_ERROR_SUCCESS && head.seq == mnSeqID) 
						   {
							   int count= head.len/sizeof(StatisticsEntry);
							   if(count == 0)
								   break;

							   StatisticsEntry* pStatic = (StatisticsEntry*)(p+sizeof(Stru_UniPkgHead));;
							   if(pStatic->mnAssetMgmtOrgID != mnCurrentOrgID)//������֯����
								   break;
							   mRiskMapData->LockObject();
							 

							   //���ڻ�����������ֻ��һ����Կ��Լ򵥵���clear��������
							   mRiskMapData->mmapStatics.clear();								 

							
							   bool lbStatic = false;							 
							   for(int i = 0; i< count; i++)
							   {
								   pStatic = (StatisticsEntry*)
									   (p+sizeof(Stru_UniPkgHead)+i*sizeof(StatisticsEntry));
								  
								   mRiskMapData->PushStatics(pStatic->mnAssetMgmtOrgID,pStatic);
								  
								 
							   }
							   mRiskMapData->UnlockObject();
							    UpdateHuiZong();
							    UpdateOtherPane();
						   }
						   break;
					   }
				   case Cmd_RM_Statistics_Push:
					  {
						  if(head.userdata1 == CF_ERROR_SUCCESS ) 
						  {
							  int count= head.len/sizeof(StatisticsEntry);
							  if(count == 0)
								  break;

							  StatisticsEntry* pStatic = (StatisticsEntry*)(p+sizeof(Stru_UniPkgHead));;
							  if(pStatic->mnAssetMgmtOrgID != mnCurrentOrgID)//������֯����
								  break;
							  mRiskMapData->LockObject();
							 
							  mRiskMapData->mmapStatics.erase(pStatic->mnAssetMgmtOrgID);								 
							
							 
							  bool lbStatic = false;							 
							  for(int i = 0; i< count; i++)
							  {
								  pStatic = (StatisticsEntry*)
									  (p+sizeof(Stru_UniPkgHead)+i*sizeof(StatisticsEntry));
								  mRiskMapData->PushStatics(pStatic->mnAssetMgmtOrgID,pStatic);								 
								
							  }
							  mRiskMapData->UnlockObject();							
							  UpdateHuiZong();							 
							  UpdateOtherPane();


						  }

						  break;
					  }
				   case Cmd_RM_SubscribeUserFund_Rsp:	
				   case Cmd_RM_UserFund_Push:
					   {
						   if(head.userdata3 == CF_ERROR_SUCCESS ) 
						   {
							   sFundInfoEx* pFundInfo = NULL;
							   int nItemCount = head.len/sizeof(sFundInfoEx);							
							   for(int i=0; i<nItemCount; i++)
							   {
								   pFundInfo = (sFundInfoEx*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(sFundInfoEx));
								   UserInfo acct;
								   memset(&acct,0,sizeof(acct));								 
								   m_pdataInfo->GetUserInfoByAccount(pFundInfo->mTradingAccountInfo.AccountID, acct); 
								   cliext::multimap<int,ProductAccountItem^>::iterator lIte = mMapAccountItemUI->find(acct.nUserID);
								   if(lIte != mMapAccountItemUI->end())
								   {
									   lIte->second->UpdateData(pFundInfo);									 
								   }								
							   }	
						   }

					   }
					   break;				
				  //case Cmd_RM_Statistics_Push:
					 // {
						//  if(head.userdata1 == OPERATOR_SUCCESS ) 
						//  {
						//	  int count= head.len/sizeof(StatisticsEntry);
						//	  if(count == 0)
						//		  break;
						//	 
						//	  StatisticsEntry* pStatic = (StatisticsEntry*)(p+sizeof(Stru_UniPkgHead));;
						//	 
						//	  mRiskMapData->LockObject();
						//	  if(pStatic->nAssetMgmtOrgID != mnCurrentOrgID) //������֯����
						//	  {
						//		  mRiskMapData->UnlockObject();
						//		  break;
						//	  }
						//	
						//	  bool lbStatic = false;							 
						//	  for(int i = 0; i< count; i++)
						//	  {
						//		  pStatic = (StatisticsEntry*)
						//			  (p+sizeof(Stru_UniPkgHead)+i*sizeof(StatisticsEntry));
						//		  //filter data not the current org id
						//		  if(pStatic->nAssetMgmtOrgID != mnCurrentOrgID)
						//			  continue;
						//		  //mRiskMapData->mmapStatics.insert(std::make_pair(pStatic->nAssetMgmtOrgID,*pStatic));	
						//		  if(pStatic->nFinancialProductID == -1) //��������
						//		  {
						//			  lbStatic = true;
						//			  mRiskMapData->mmapStatics.erase(pStatic->nAssetMgmtOrgID);
						//			  mRiskMapData->PushStatics(pStatic->nAssetMgmtOrgID,pStatic,nullptr);
						//		  }
						//		  else
						//		  {		
						//			  mRiskMapData->mmapFinacialProducctStatics.erase(pStatic->nFinancialProductID);//ɾ��ԭ�е�
						//			  mRiskMapData->PushFinacialProductStatics(pStatic->nFinancialProductID,pStatic);
						//		  }
						//	  }
						//	  mRiskMapData->UnlockObject();
						//	  if(lbStatic)
						//		  UpdateHuiZong();
						//	  else
						//		  UpdateOtherPane();

						//	
						//  }

						//  break;
					 // }
				  //case Cmd_RM_Statistics_Push:
					 // {
						//  //����һ���仯��������Ҫ�ж��������Ļ���ԭ���������ֵ
						//  //�������Ƶ��޸�datatabel�ı仯
						//  bool lbStatic = false;						 
						//  if(head.userdata1 == OPERATOR_SUCCESS && head.len == sizeof(StatisticsEntry))
						//  {
						//	  StatisticsEntry* pStatic = (StatisticsEntry*)(p+sizeof(Stru_UniPkgHead));
						//	  if(pStatic->nAssetMgmtOrgID != mnCurrentOrgID)
						//		  break;
						//	  mRiskMapData->LockObject();
						//	  if(pStatic->nFinancialProductID == -1) //��������
						//	  {
						//		  lbStatic = true;
						//		  mRiskMapData->PushStatics(pStatic->nAssetMgmtOrgID,pStatic,nullptr);
						//	  }
						//	  else
						//	  {								 
						//		  mRiskMapData->PushFinacialProductStatics(pStatic->nFinancialProductID,pStatic);
						//	  }

						//	  mRiskMapData->UnlockObject();
						//	  if(lbStatic)
						//		  UpdateHuiZong();
						//	  else
						//		  UpdateOtherPane();
						//	 
						//  }


						//  break;
					 // }
				  default:break;
				  }
		    }

			Form::WndProc ( m );
		  }

private: System::Void OnFormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			// UnsubscribeFinicalProductStatiacsData();
			 UnsubscribeAccountStatiacsData();
			 UnsubscribeStatiacsData();
		 }
private: System::Void UpdateHuiZong() {
			 //���°ٷֱ� ���ŵı�֤��/���ŵ���Ȩ��
			 Single lfPercent = 0.0f;			
			 if(mRiskMapData->mmapStatics.size() > 0)
			 {
				 TotalStatiscData ^ lDepartTotalData = gcnew TotalStatiscData();
				 if(mRiskMapData->mmapStatics.size() > 0)
				 {
					 RiskMapStatisticMap::iterator lIte = mRiskMapData->mmapStatics.begin();
					 while(lIte != mRiskMapData->mmapStatics.end())
					 {
						 lDepartTotalData->Add(lIte->second.mnStatisticsType,lIte->second.mdblValue);
						 lIte++;
					 }
				 }
				
				 lfPercent = (Single)(TotalStatiscData::SafeDivMulti100(lDepartTotalData->mdbBZJ ,lDepartTotalData->mdbDQQY));
				 this->simpleGuage1->Value = lfPercent;
			 }		 

		 }
private: System::Void UpdateOtherPane() {
			 //������Ʋ�Ʒҳ�棬Ʒ��ҳ�棬ͼ��ҳ��
             
			 if(mRiskMapData->mmapStatics.size() == 0 )
				 return;
			 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
             //�Ȼ�ȡ�����µĳ�ʼֵ
			 TotalStatiscData ^ lDepartTotalData = gcnew TotalStatiscData();
			 if(mRiskMapData->mmapStatics.size() > 0)
			 {
				 RiskMapStatisticMap::iterator lIte = mRiskMapData->mmapStatics.begin();
				 while(lIte != mRiskMapData->mmapStatics.end())
				 {
					 lDepartTotalData->Add(lIte->second.mnStatisticsType,lIte->second.mdblValue);
					 lIte++;
				 }
			 }

			 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////			
			
             ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			 //���ݺ�Լ ��ȡƷ�� ���Ʒ��ҳ��
			 //������Լ,ȷ��Ʒ�� �������Ʒ�� ��������
			 {
				
				 //ͳ������  
				 RiskMapStatisticMap::iterator lIteratorStart,lIteratorEnd;	
				 lIteratorStart = mRiskMapData->mmapStatics.begin();			
				 cliext::multimap<String^,TotalStatiscData^> ^  lTypeTotalData = gcnew cliext::multimap<String^,TotalStatiscData^>();
				 while(lIteratorStart != mRiskMapData->mmapStatics.end())
				 {		
					 String ^ lsIID = Tools::string2String(lIteratorStart->second.mInstrumentID);	
					 if(lsIID == "")
					 {
						 lIteratorStart++;
						 continue;
					 }
				
					 //if(m_pdataInfo->GetInstrunmentInfobyID(std::string(lIteratorStart->second.mInstrumentID), instrument))
					 {
						 //String^ strProductID = Tools::string2String(lIteratorStart->second.mInstrumentID);
						 // strProductID = strProductID.substr(0,2);
						 String ^ lsStr ="";
						 for(int i = 0; i < lsIID->Length;i++)
						 {
							 if(Char::IsLetter(lsIID,i))
							 {
								 lsStr += lsIID->Substring(i,1);
							 }
							 else
								 break;
						 }
						 String ^ lsPID = lsStr;	
						 

						 //UI ���� ����
						 cliext::multimap<String^,TotalStatiscData^> ::iterator lDataIterator = lTypeTotalData->find(lsPID);
						 if(lDataIterator == lTypeTotalData->end())
						 {
							 //׷������
							 TotalStatiscData ^ lTempData = gcnew TotalStatiscData();
							 lTempData->Add(lIteratorStart->second.mnStatisticsType,lIteratorStart->second.mdblValue);
							 lTypeTotalData->insert(cliext::make_pair(lsPID,lTempData));

						 }		
						 else
						 {
							 //��������
							  TotalStatiscData ^ lTempData = lDataIterator->second;
							  lTempData->Add(lIteratorStart->second.mnStatisticsType,lIteratorStart->second.mdblValue);							 
						 }
					 }

					 lIteratorStart++;
				 }

				 //����item����
				 cliext::multimap<String^,TotalStatiscData^> ::iterator lDataPos = lTypeTotalData->begin();				
				 while(lDataPos !=  lTypeTotalData->end())
				 {			
					
					 String ^ lsPID = lDataPos->first;	
					 //UI ���� ����
					 cliext::multimap<String^,DepartProductTypeItem^>::iterator lUIIter = mMapProductTypeItemUI->find(lsPID);
					 if(lUIIter == mMapProductTypeItemUI->end())
					 {
						 if(lDataPos->second->mdbCCL > 0.00006)
						 {						
							 //����ֲ�����Ϊ0��׷��һ��UI
							 DepartProductTypeItem ^ lItem = gcnew DepartProductTypeItem();
							 mMapProductTypeItemUI->insert(cliext::make_pair(lsPID,lItem));
							 lItem->TypeName = lsPID;					
							 lItem->TotalHode = (TotalStatiscData::SafeDivMulti100(lDataPos->second->mdbCCL, lDepartTotalData->mdbCCL));
							 lItem->TotalBJZPercent = (TotalStatiscData::SafeDivMulti100(lDataPos->second->mdbBZJ , lDepartTotalData->mdbBZJ));
							 lItem->TotalHodeCount = lDataPos->second->mdbCCL;
							 lItem->TotalBZJ = lDataPos->second->mdbBZJ;						
							 this->listPanelControl2->Controls->Add(lItem);	
						 }
					 }	
					 else
					 { 
						 DepartProductTypeItem ^ lItem = lUIIter->second;
						 if(lDataPos->second->mdbCCL < 0.00006)
						 {
							 //����ֲ���Ϊ0����UI							
							 mMapProductTypeItemUI->erase(lsPID);
							 this->listPanelControl2->Controls->Remove(lItem);	

						 }
						 else
						 {						
							 //����UI												
							 lItem->TypeName = lsPID;					
							 lItem->TotalHode = (TotalStatiscData::SafeDivMulti100(lDataPos->second->mdbCCL , lDepartTotalData->mdbCCL));
							 lItem->TotalBJZPercent = (TotalStatiscData::SafeDivMulti100(lDataPos->second->mdbBZJ , lDepartTotalData->mdbBZJ));
							 lItem->TotalHodeCount = lDataPos->second->mdbCCL;
							 lItem->TotalBZJ = lDataPos->second->mdbBZJ; 
						 }
					 }				

					 lDataPos++;
				 }

			 }

			 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			 //������״ͼ,����Լͳ��
			 { 
				 //����Լͳ��
				 RiskMapStatisticMap::iterator lIteratorStart,lIteratorEnd;	
				 lIteratorStart = mRiskMapData->mmapStatics.begin();			
				 cliext::multimap<String^,TotalStatiscData^> ^  lInstrumentTotalData = gcnew cliext::multimap<String^,TotalStatiscData^>();
				 while(lIteratorStart != mRiskMapData->mmapStatics.end())
				 {						
					 //UI ���� ����
					  String ^ lsIID = Tools::string2String(lIteratorStart->second.mInstrumentID);	
					  if(lsIID == "")
					  {
						  lIteratorStart++;
						  continue;
					  }

					  cliext::multimap<String^,TotalStatiscData^> ::iterator lDataIterator = 
						 lInstrumentTotalData->find(lsIID);

					 if(lDataIterator == lInstrumentTotalData->end())
					 {
						 //׷������
						 TotalStatiscData ^ lTempData = gcnew TotalStatiscData();
						 lTempData->Add(lIteratorStart->second.mnStatisticsType,lIteratorStart->second.mdblValue);
						 lInstrumentTotalData->insert(cliext::make_pair(lsIID,lTempData));

					 }		
					 else
					 {
						 //��������
						 TotalStatiscData ^ lTempData = lDataIterator->second;
						 lTempData->Add(lIteratorStart->second.mnStatisticsType,lIteratorStart->second.mdblValue);
					 }
				

					 lIteratorStart++;
				 }
				 //������״ͼ
				 UpdateGrid(lInstrumentTotalData);

			 }
		 }







private: System::Void UpdateGrid(cliext::multimap<String^,TotalStatiscData^> ^ nInstrumentDataMap) {
			 if(nInstrumentDataMap->size() > 0)
			 {
				 chart1->Series["Series1"]->Points->Clear();				
				 cliext::multimap<String^,TotalStatiscData^>::iterator lIter = nInstrumentDataMap->begin();
				 while(lIter != nInstrumentDataMap->end())
				 {
					 if(lIter->second->mdbCCL > 0.00006 || lIter->second->mdbCCL < -0.00006)
						chart1->Series["Series1"]->Points->AddXY(lIter->first,lIter->second->mdbCCL);					 
					  lIter++;
				 }

			 }

		 }

private: System::Void pictureBox4_Click(System::Object^  sender, System::EventArgs^  e) {
			 //ȥ������
			 if(this->FormBorderStyle != System::Windows::Forms::FormBorderStyle::None)
			 {
				 this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
				// this->Location =  System::Drawing::Point(0, 0);
				// this->Size = System::Windows::Forms::Screen::PrimaryScreen->Bounds.Size;
				 this->WindowState = System::Windows::Forms::FormWindowState::Maximized; 
				 this->TopMost = true; 				
				 LayOut();
				 this->Refresh();
			 }
			 else
			 {
				
				 this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle; 
				 this->WindowState = System::Windows::Forms::FormWindowState::Maximized; 
				 this->TopMost = false; 
				
				 LayOut(); 
				 this->Refresh();
			 }
		 }
public: System::Void CombinationScreenSetting(System::Drawing::Point  nLocation,System::Drawing::Size  nSize)
		 {
			  this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			  this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			  this->WindowState = System::Windows::Forms::FormWindowState::Normal; 
			  this->Location = nLocation;
			  this->Size = nSize;			
			  mbCombinationScreen = true;
			/*  LayOut();
			  this->Refresh();*/

		 }
};
}
