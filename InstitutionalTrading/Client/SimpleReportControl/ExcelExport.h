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


namespace SimpleReportControl
{
	 delegate void ShowProgressCallback(String^ nsText,Int32 nProgress);
	 public ref class CFileDBUtil
	 {
	 public: static bool ExportToExcel(System::Data::DataTable^ nrTable,
				 System::Windows::Forms::FlowLayoutPanel^ nrChartContainer,
				 String^ nsFilePath,
				 ShowProgressCallback^ pFunShowProgress
				 )   //����excel
		 {
			 if(nrChartContainer == nullptr && nrTable == nullptr)
				 return false;

			
			Type^ type = Type::GetTypeFromProgID("Excel.Application");
			if(type == nullptr)
			{
				MessageBox::Show("Excel�޷�����", "����", MessageBoxButtons::OK, MessageBoxIcon::Error);
				// pFunShowProgress("Excel�޷�����,����ʧ��",100);
				return false;
			}

		     Excel::Application^ lrExcel = gcnew  Excel::ApplicationClass();
			 if (lrExcel == nullptr)
			 {
				 MessageBox::Show("Excel�޷�����", "����", MessageBoxButtons::OK, MessageBoxIcon::Error);
				// pFunShowProgress("Excel�޷�����,����ʧ��",100);
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
				 pFunShowProgress("д�����",5);
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

				 pFunShowProgress("д����ֵ",10);
				 for (int r = 0; r < nrTable->Rows->Count; r++)
				 {
					 String ^ ls = String::Format("д����{0}/{1}",r,nrTable->Rows->Count);					
					 pFunShowProgress(ls, 10 + ((60*r)/nrTable->Rows->Count));
					 (worksheetData->Range[worksheetData->Cells[r+2,1], worksheetData->Cells[r+2, nrTable->Columns->Count]])->Value2 = nrTable->Rows[r]->ItemArray;
					/* for (int i = 0; i < nrTable->Columns->Count; i++)		
					 {					
						 worksheetData->Cells[r + 2, i + 1] = "'"+nrTable->Rows[r][i]->ToString(); 
					 }*/
					
					System::Windows::Forms::Application::DoEvents();
				 }           

				 pFunShowProgress("д����ֵ�ɹ�",50);
				 worksheetData->Columns->EntireColumn->AutoFit();
				 
			 }
			 if(nrChartContainer != nullptr)
			 {
				 pFunShowProgress("����ͼƬ",70);
				 lnLastRow ++; //ͼƬ�����ݸ�һ��
				 System::Collections::IEnumerator ^ liEnum = nrChartContainer->Controls->GetEnumerator();
				 while(liEnum && liEnum->MoveNext())
				 {
					  Control ^ lTemp = (Control^)liEnum->Current;
					  System::Windows::Forms::DataVisualization::Charting::Chart ^ lChart = dynamic_cast<System::Windows::Forms::DataVisualization::Charting::Chart ^>(lTemp);
					  if(lChart)
					  {
						  String^ lsPath = System::IO::Path::GetTempFileName();				
						  lChart->SaveImage(lsPath,System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Png);
						  
						  Excel::Range^ insertRange = worksheetData->Range["A"+lnLastRow.ToString(),"B"+lnLastRow.ToString()];//(Excel::Range^)worksheetData->Cells[lnLastRow+1, 1];
						  insertRange->Select();
						  //Excel::Pictures^ pics = (Excel::Pictures^)worksheetData->Pictures(System::Reflection::Missing::Value);
						  //pics->Insert(lsPath, System::Reflection::Missing::Value);     
						  worksheetData->Shapes->AddPicture(lsPath,Microsoft::Office::Core::MsoTriState::msoFalse, 
							  Microsoft::Office::Core::MsoTriState::msoTrue, Convert::ToSingle(insertRange->Left),
							  Convert::ToSingle(insertRange->Top), 1400, 500); 

						  lnLastRow += 36;//23;

					  }

				 }				 
				 
				 //worksheetData->ro
			 }


			 try
			 {				
					
				 pFunShowProgress("���浽�ļ�",90);
				 workbookData->Saved = true;
				 workbookData->SaveAs(nsFilePath, System::Reflection::Missing::Value, System::Reflection::Missing::Value, 
					     System::Reflection::Missing::Value,false, false,
						 Excel::XlSaveAsAccessMode::xlNoChange,
						 System::Reflection::Missing::Value, System::Reflection::Missing::Value, System::Reflection::Missing::Value,
						 System::Reflection::Missing::Value, System::Reflection::Missing::Value);
				

				 pFunShowProgress("�����ɹ�",100);
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