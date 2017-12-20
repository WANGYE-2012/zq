#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace ListPanel {

	/// <summary>
	/// Summary for ListPanelControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class ListPanelControl : public System::Windows::Forms::FlowLayoutPanel
	{
	private:
		Boolean mbVertical; //�Ƿ���vertical list or horizonal list
		Int32 mnPosition;   //λ��0�൱�� view��ͼ��λ��
		System::Timers::Timer ^mAnimateTimer; //�����Ķ�ʱ��
		System::Reflection::PropertyInfo^ mprop ;
	public:
		property Boolean Vertical
		{
			Boolean get() {return mbVertical;}
			void set(Boolean nval)
			{
				mbVertical = nval;
				if(mbVertical)
					this->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
				else
					this->FlowDirection = System::Windows::Forms::FlowDirection::LeftToRight;

			}
		}
	public:
		ListPanelControl(void)
		{
			mbVertical = true;
			mnPosition = 0;		
		
			this->WrapContents = false;
			this->AutoScroll = true;
			
			this->SetStyle(  ControlStyles::AllPaintingInWmPaint |
				ControlStyles::UserPaint |
				ControlStyles::OptimizedDoubleBuffer |
				ControlStyles::DoubleBuffer,true);

			UpdateStyles();
			InitializeComponent();
			
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ListPanelControl()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			/*System::Reflection::PropertyInfo^ aProp = 
				typeof(System::Windows::Forms::Control).GetProperty(
				"DoubleBuffered", 
				System::Reflection::BindingFlags::NonPublic | 
				System::Reflection::BindingFlags::Instance);

			aProp.SetValue(c, true, null); */
			this->SuspendLayout();
			// 
			// ListPanelControl
			// 
			this->BackColor = System::Drawing::Color::Transparent;
			this->Size = System::Drawing::Size(457, 332);
			this->MouseLeave += gcnew System::EventHandler(this, &ListPanelControl::OnMoustLeave);
			this->ControlAdded += gcnew System::Windows::Forms::ControlEventHandler(this, &ListPanelControl::OnControlAdded);
			this->MouseEnter += gcnew System::EventHandler(this, &ListPanelControl::OnMouseEnter);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void ResetControlPosition()
			 {
				 
				
				 this->SuspendLayout();
				 Int32 lCurrentPos = mnPosition;
				 System::Collections::IEnumerator ^ lEnumator = this->Controls->GetEnumerator();
                 while(lEnumator->MoveNext())
                 {
					
					 Control ^ lTemp = (Control^)lEnumator->Current;
					 if(lTemp)
					 {
						 if(mbVertical)
						 {
							 lCurrentPos = lCurrentPos + 9;
							 lTemp->Location = Point(5,lCurrentPos);							
							 lCurrentPos = lCurrentPos + lTemp->Height ;
						 }
						 else
						 {
							 lCurrentPos = lCurrentPos + 5;
							 lTemp->Location = Point(lCurrentPos,9);							 
							 lCurrentPos = lCurrentPos + lTemp->Width ;
						 }
					 }	
					 //this->Update();
					 				
                 }
				 this->ResumeLayout(false);
				
			 }
	private: System::Void OnControlAdded(System::Object^  sender, System::Windows::Forms::ControlEventArgs^  e) {				
				
				 mprop = this->GetType()->GetProperty("DoubleBuffered", 
					 System::Reflection::BindingFlags::NonPublic | 
					 System::Reflection::BindingFlags::Instance);
				 if(mprop)
					 mprop->SetValue(e->Control,true,nullptr);
			
				 if(e->Control->HasChildren)
					 SearchControl(e->Control);			
				
				// ResetControlPosition();
			 }
	private: System::Void SearchControl(Control ^ nControl)
			 {
				 if(nControl == nullptr)
					 return;
				 nControl->MouseEnter  += gcnew System::EventHandler(this, &ListPanelControl::OnMouseEnter);
				 //nControl->MouseLeave  += gcnew System::EventHandler(this, &ListPanelControl::OnMoustLeave);

				 System::Collections::IEnumerator ^ lEnumator = nControl->Controls->GetEnumerator();
				 while(lEnumator->MoveNext())
				 {
					 Control ^ lTemp = (Control^)lEnumator->Current;
					 if(mprop)
						 mprop->SetValue(lTemp,true,nullptr);
					 if(lTemp->HasChildren)
						 SearchControl(lTemp);

				 }

			 }

	//��ʼ����
	public: System::Void StartAnimate(){
				if(mAnimateTimer == nullptr)
				{
					mAnimateTimer = gcnew System::Timers::Timer(200);//ʵ����Timer�࣬���ü��ʱ��Ϊ10000���룻
					mAnimateTimer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this,&ListPanelControl::AnimateTimeOut);//����ʱ���ʱ��ִ���¼���
				    mAnimateTimer->AutoReset = true;//������ִ��һ�Σ�false������һֱִ��(true)��
				}
				mAnimateTimer->Enabled = true;//�Ƿ�ִ��System.Timers.Timer.Elapsed�¼���
			}
	public: System::Void StopAnimate(){
				if(mAnimateTimer != nullptr)
					mAnimateTimer->Enabled = false;
			}
	public: System::Void  AnimateTimeOut(Object^ source, System::Timers::ElapsedEventArgs^ e){
				mnPosition --;
				ResetControlPosition();

			}
	private: System::Void OnMouseEnter(System::Object^  sender, System::EventArgs^  e) {
				 if(mAnimateTimer != nullptr && mAnimateTimer->Enabled != false)
					 StopAnimate();
			}
    private: System::Void OnMoustLeave(System::Object^  sender, System::EventArgs^  e) {
			     if(mAnimateTimer != nullptr && mAnimateTimer->Enabled == false)
				     StartAnimate();
		    }
	//protected: property System::Windows::Forms::CreateParams^  CreateParams {
	//			virtual System::Windows::Forms::CreateParams^ get() override{
	//				 System::Windows::Forms::CreateParams^ cp = Panel::CreateParams;
	//				 cp->ExStyle |= 0x02000000;  // Turn on WS_EX_COMPOSITED
	//				 return cp;
	//			 }
	//		 } 
};
	
}
