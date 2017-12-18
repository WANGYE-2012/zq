#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


#include "Tools.h"
#include "CDataInfo.h"
#include "CFinancialProductIDListDlg.h"


namespace Client {

	/// <summary>
	/// Summary for CAssetMgmtOrganIDTreeListDlg
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class CAssetMgmtOrganIDTreeListDlg : public AnywndComboBox::CAnywndComboBoxControlContainter
	{
	public:
		CAssetMgmtOrganIDTreeListDlg(void)
		{
			m_pdataInfo = NULL;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			ResetCheckAllShow(false, false);
			m_pdataInfo = CDataInfo::NewInstance();
			LoadAssertMgmtOrgan();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CAssetMgmtOrganIDTreeListDlg()
		{
			if (components)
			{
				delete components;
			}
		}

	private: CDataInfo* m_pdataInfo;
	private: CFinancialProductIDListDlg^ dlgFinaProduct;
	private: System::Windows::Forms::ComboBox^  comboRelate;

	private: System::Windows::Forms::TreeView^  treeMgmtOrgan;
	private: System::Windows::Forms::CheckBox^  checkHasAll;
	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

	public:
		void ResetCheckAllShow(bool bShow, bool bValue) {
			System::Drawing::Size size = this->Size;
			checkHasAll->Checked = bValue;
			if(bShow) {
				checkHasAll->Show();
				treeMgmtOrgan->Location = Point(0, 28);
				size.Height -= 28;
				treeMgmtOrgan->Size = size;
			}
			else {
				checkHasAll->Hide();
				treeMgmtOrgan->Location = Point(0, 0);
				treeMgmtOrgan->Size = size;
			}
		}
		bool CheckAllShown() {
			return checkHasAll->Visible;
		}
		bool HasCheckAll() {
			return checkHasAll->Checked;
		}
		void ResetRelateCombo(System::Windows::Forms::ComboBox^ comboRelate) {
			this->comboRelate = comboRelate;
		};
		void ResetFinaProductDlg(CFinancialProductIDListDlg^ dlg) {
			dlgFinaProduct = dlg;
		}
		void Clear() {
			comboRelate->Text = Tools::string2String("");
			if(treeMgmtOrgan->Nodes->Count>0)
				UncheckTreeNode(treeMgmtOrgan->Nodes[0]);
		}
		bool GetSelectID(std::set<int>& setMgmtOrganID, bool& bNoChecked) {
			if(treeMgmtOrgan->Nodes->Count==0)
				return false;

			bNoChecked = true;
			int nID = 0;
			for(int i=0; i<treeMgmtOrgan->Nodes->Count; i++) {
				TreeNode^ node = treeMgmtOrgan->Nodes[i];
				if(node->Checked) {
					String^ strKey = treeMgmtOrgan->Nodes[0]->Name;
					nID = atoi(Tools::String2string(strKey).c_str());
					setMgmtOrganID.insert(nID);
					bNoChecked = false;
				}
				GetNodeID(node, setMgmtOrganID, bNoChecked);
			}
			if(bNoChecked) {
				TreeNode^ node = treeMgmtOrgan->Nodes[0];
				String^ strKey = treeMgmtOrgan->Nodes[0]->Name;
				nID = atoi(Tools::String2string(strKey).c_str());
				setMgmtOrganID.insert(nID);
			}
			return setMgmtOrganID.size()>0;
		}

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->treeMgmtOrgan = (gcnew System::Windows::Forms::TreeView());
			this->checkHasAll = (gcnew System::Windows::Forms::CheckBox());
			this->SuspendLayout();
			// 
			// treeMgmtOrgan
			// 
			this->treeMgmtOrgan->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->treeMgmtOrgan->CheckBoxes = true;
			this->treeMgmtOrgan->FullRowSelect = true;
			this->treeMgmtOrgan->HideSelection = false;
			this->treeMgmtOrgan->Location = System::Drawing::Point(-1, 28);
			this->treeMgmtOrgan->Name = L"treeMgmtOrgan";
			this->treeMgmtOrgan->Size = System::Drawing::Size(160, 209);
			this->treeMgmtOrgan->TabIndex = 0;
			this->treeMgmtOrgan->AfterCheck += gcnew System::Windows::Forms::TreeViewEventHandler(this, &CAssetMgmtOrganIDTreeListDlg::treeMgmtOrgan_AfterCheck);
			// 
			// checkHasAll
			// 
			this->checkHasAll->AutoSize = true;
			this->checkHasAll->Location = System::Drawing::Point(6, 6);
			this->checkHasAll->Name = L"checkHasAll";
			this->checkHasAll->Size = System::Drawing::Size(120, 16);
			this->checkHasAll->TabIndex = 2;
			this->checkHasAll->Text = L"ͳ�ư����¼�����";
			this->checkHasAll->UseVisualStyleBackColor = true;
			this->checkHasAll->Visible = false;
			// 
			// CAssetMgmtOrganIDTreeListDlg
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->checkHasAll);
			this->Controls->Add(this->treeMgmtOrgan);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"CAssetMgmtOrganIDTreeListDlg";
			this->Size = System::Drawing::Size(160, 238);
			this->Load += gcnew System::EventHandler(this, &CAssetMgmtOrganIDTreeListDlg::CAssetMgmtOrganIDTreeListDlg_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: void LoasSubAssertMgmtOrgan(TreeNode^ nodeMain, 
					MAPAeestMgmtOrganization& mapMain, 
					std::map<int, AssetMgmtOrganization>& mapSub) {
#if 1
				MAPAeestMgmtOrganization::iterator it;
				std::map<int, AssetMgmtOrganization>::iterator itSub;

				TreeNode^ subNode;

				itSub = mapSub.begin();
				for(; itSub != mapSub.end(); itSub++) {
					int^ nID = itSub->first;
					String^ strText = nID->ToString();
					String^ strValue = Tools::string2String(itSub->second.szName);
					subNode = nodeMain->Nodes->Add(strText, strValue);
					
					it = mapMain.find(itSub->first);
					if(it != mapMain.end())
						LoasSubAssertMgmtOrgan(subNode, mapMain, it->second);
				}
#endif
			 }

	private: void LoadAssertMgmtOrgan() {
#if 1				

				if(!m_pdataInfo->IsInit())
					return;

				LoginRsp loginRsp;
				memset(&loginRsp, 0, sizeof(loginRsp));
				m_pdataInfo->GetLoginRsp(loginRsp);

				AssetMgmtOrganization assertOrg;
				memset(&assertOrg, 0, sizeof(assertOrg));
				if(!m_pdataInfo->GetAssetMgmtOrg(
						loginRsp.nAssetMgmtOrgID, assertOrg))
					return;

				TreeNode^ node;
				
				int^ nID = assertOrg.nAssetMgmtOrgID;
				String^ strText = nID->ToString();
				String^ strValue = Tools::string2String(assertOrg.szName);
				treeMgmtOrgan->Nodes->Clear();
				node = treeMgmtOrgan->Nodes->Add(strText, strValue);

				MAPAeestMgmtOrganization mapAssetMgmtOrgTree;
				m_pdataInfo->GetAeestMgmtOrg(mapAssetMgmtOrgTree);
				MAPAeestMgmtOrganization::iterator it = 
						mapAssetMgmtOrgTree.find(loginRsp.nAssetMgmtOrgID);
				if(it == mapAssetMgmtOrgTree.end())
					return;

				std::map<int, AssetMgmtOrganization>& mapSub = it->second;
				if(mapSub.begin()==mapSub.end())
					return;

				LoasSubAssertMgmtOrgan(node, mapAssetMgmtOrgTree, mapSub);

				node->Expand();
#endif
			 }

	private: System::Void CAssetMgmtOrganIDTreeListDlg_Load(System::Object^  sender, System::EventArgs^  e) {
				 //Rectangle rect = this->ClientRectangle;
				 //treeMgmtOrgan->Location = Point(0, 0);
				 //treeMgmtOrgan->Width = rect.Width;
				 //treeMgmtOrgan->Height = rect.Height;
			 }

	private: void GetNodeID(TreeNode^ node, std::set<int>& setMgmtOrganID, bool& bNoChecked) {
				 for(int i=0; i<node->Nodes->Count; i++) {
					 if(node->Nodes[i]->Checked) {
						String^ strKey = node->Nodes[i]->Name;
						int nID = atoi(Tools::String2string(strKey).c_str());
						setMgmtOrganID.insert(nID);
						bNoChecked = false;
					 }
					GetNodeID(node->Nodes[i], setMgmtOrganID, bNoChecked);
				 }
			 }

	private: bool HasTreeChecked(TreeNode^ node, bool& bNoChecked) {
				 for(int i=0; i<node->Nodes->Count; i++) {
					 if(node->Nodes[i]->Checked) {
						bNoChecked = false;
					 }
					HasTreeChecked(node->Nodes[i], bNoChecked);
				 }
				 return bNoChecked;
			 }

	private: void GetTreeItemText(TreeNode^ node, std::string& strText, bool bNoChecked) {
				 for(int i=0; i<node->Nodes->Count; i++) {
					 if(bNoChecked || (!bNoChecked && node->Nodes[i]->Checked)) {
						String^ strKey = node->Nodes[i]->Text;
						strText += Tools::String2string(strKey);
						strText += ", ";
					 }
					 GetTreeItemText(node->Nodes[i], strText, bNoChecked);
				 }
			 }
	private: void UncheckTreeNode(TreeNode^ node) {
				 node->Checked = false;
				 for(int i=0; i<node->Nodes->Count; i++) {
					 UncheckTreeNode(node->Nodes[i]);
				 }
			 }

	private: System::Void treeMgmtOrgan_AfterCheck(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
#if 1
				 if(treeMgmtOrgan->Nodes->Count==0)
					 return;

				 //int nID = 0;
				 //std::set<int> setMgmtOrganID;
				 //if(treeMgmtOrgan->Nodes[0]->Checked) {
					// String^ strKey = treeMgmtOrgan->Nodes[0]->Name;
					// nID = atoi(Tools::String2string(strKey).c_str());
					// setMgmtOrganID.insert(nID);
				 //}
				 //GetNodeID(treeMgmtOrgan->Nodes[0], setMgmtOrganID);
				 bool bNoChecked = true;
				 std::set<int> setMgmtOrganID;
				 GetSelectID(setMgmtOrganID, bNoChecked);
				 
				 std::set<int> setProductID;
				 std::set<int> setProdIDTemp;
				 for(std::set<int>::iterator it = setMgmtOrganID.begin(); 
						it != setMgmtOrganID.end(); it++) {
					//System::Diagnostics::Debug::Print("��֯������� %d\n", *it);
					if(m_pdataInfo->GetFinancialProduct(*it, bNoChecked, setProdIDTemp)>0) {
						std::set<int>::iterator itset;
						for(itset = setProdIDTemp.begin(); 
								itset != setProdIDTemp.end(); itset++) {
							//System::Diagnostics::Debug::Print("��ѯ���Ĳ�Ʒ��� %d\n", *itset);
							setProductID.insert(*itset);
						}
					}
				 }

				 std::map<int, FinancialProduct> mapFinaProduct;
				 FinancialProduct product;
				 
				 memset(&product, 0, sizeof(product));
				 std::set<int>::iterator it = setProductID.begin();
				 for(; it != setProductID.end(); it++) {
					 if(m_pdataInfo->GetFinancialProduct(*it, product)) {
						 mapFinaProduct.insert(make_pair(*it, product));
					 }
				 }
				 //if(bNoChecked)
					// dlgFinaProduct->LoadAllFinanProduct();
				 //else
					 dlgFinaProduct->ResetFinaProduct(mapFinaProduct, mapFinaProduct.size()==0);

				 if(e->Node->Checked)
					 e->Node->Expand();
#else
				 String^ strKey = e->Node->Name;
				 int nID = atoi(Tools::String2string(strKey).c_str());

				 std::set<int> setProductID;
				 if(m_pdataInfo->GetFinancialProduct(nID, false, setProductID)<=0)
					 return;
				 
				 std::map<int, FinancialProduct> mapFinaProduct;
				 FinancialProduct product;
				 
				 memset(&product, 0, sizeof(product));
				 std::set<int>::iterator it = setProductID.begin();
				 for(; it != setProductID.end(); it++) {
					 if(m_pdataInfo->GetFinancialProduct(*it, product)) {
						 mapFinaProduct.insert(make_pair(*it, product));
					 }
				 }
				 if(mapFinaProduct.size()>0)
					 dlgFinaProduct->ResetFinaProduct(mapFinaProduct, !e->Node->Checked);
#endif
			 }
public: virtual Void ProcessData() override
			{
				 std::string strText;
				 bool bNoChecked = true;
				 if(treeMgmtOrgan->Nodes->Count>0) {
					 bNoChecked = !treeMgmtOrgan->Nodes[0]->Checked;
					 HasTreeChecked(treeMgmtOrgan->Nodes[0], bNoChecked);
					 if(bNoChecked || (!bNoChecked && treeMgmtOrgan->Nodes[0]->Checked)) {
						String^ strKey = treeMgmtOrgan->Nodes[0]->Text;
						strText += Tools::String2string(strKey);
						strText += ", ";
					 }
					 GetTreeItemText(treeMgmtOrgan->Nodes[0], strText, bNoChecked);
					 comboRelate->Text = Tools::string2String(strText.c_str());
				 }
				 else
					 comboRelate->Text = Tools::string2String("");
			}
};
}
