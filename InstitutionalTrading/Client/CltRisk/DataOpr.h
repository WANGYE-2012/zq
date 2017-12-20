#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace Microsoft::Office::Core;
using namespace Microsoft::Office::Interop;
using namespace Microsoft::Office::Interop::Excel;


namespace CltRisk
{
	 public ref class CDataOpr
	 {
	 public: static bool ExportToExcel(System::Data::DataTable^ nrTable,String^ nsFilePath)   //����excel
		 {
			 if(nrTable == nullptr)
				 return false;

			 Excel::Application^ lrExcel = gcnew  Excel::ApplicationClass();
			 if (lrExcel == nullptr)
			 {
				 MessageBox::Show("Excel�޷�����", "����", MessageBoxButtons::OK, MessageBoxIcon::Error);
				 return false;
			 }

			 Excel::Workbook ^ workbookData = nullptr;
			 Excel::Worksheet ^ worksheetData = nullptr;
			 Range^ range;
			 workbookData = lrExcel->Workbooks->Add(System::Reflection::Missing::Value);		
             worksheetData = (Excel::Worksheet^)workbookData->Worksheets[1];
			 Int32 lnLastRow = 0;
			 if(nrTable != nullptr)
			 {
				 
				 lnLastRow = nrTable->Rows->Count + 2;
				 //д�����
				 for (int i = 0; i < nrTable->Columns->Count; i++)
				 {					
					 worksheetData->Cells[1, i + 1] = nrTable->Columns[i]->ColumnName;
					 range = (Range^)worksheetData->Cells[1, i + 1];
					 range->Interior->ColorIndex = 15;
					 range->Font->Bold = true;
					 range->NumberFormatLocal = "@";//�ı���ʽ 
					 // range.EntireColumn.AutoFit();//�Զ������п� 
					 // range.WrapText = true; //�ı��Զ�����   
					 range->ColumnWidth = 15;				

				 }
				 //д����ֵ

				 for (int r = 0; r < nrTable->Rows->Count; r++)
				 {
					 for (int i = 0; i < nrTable->Columns->Count; i++)		
					 {					
						 worksheetData->Cells[r + 2, i + 1] = "'"+nrTable->Rows[r][i]->ToString(); 
					 }
					
					 System::Windows::Forms::Application::DoEvents();
				 }           

				 worksheetData->Columns->EntireColumn->AutoFit();
				 
			 }
			 

			 try
			 {				
					
				 workbookData->Saved = true;
				 workbookData->SaveAs(nsFilePath, System::Reflection::Missing::Value, System::Reflection::Missing::Value, 
					                             System::Reflection::Missing::Value,false, false,
						                         Excel::XlSaveAsAccessMode::xlNoChange,
						                         System::Reflection::Missing::Value, System::Reflection::Missing::Value, System::Reflection::Missing::Value,
						                         System::Reflection::Missing::Value, System::Reflection::Missing::Value);
				

			 }
			 catch (System::Exception ^ )
			 {				
				 lrExcel->Quit();				
				 return false;
			 }
			
			 lrExcel->Quit();			
			 return true;
		 }
	 };

}