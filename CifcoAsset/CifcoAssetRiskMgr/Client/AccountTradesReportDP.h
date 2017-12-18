
#pragma once
#include "AbstractReportDataProvider.h"
#include "CFieldDetail.h"
using namespace System;
using namespace System::Data;
using namespace System::Windows::Forms::DataVisualization::Charting;


namespace ReportDataProvider {


	public ref class CAccountTradesReportDataProvider :public IReportDataProvider
	{	
	private:
		array<String^>^ mrCloumeHeaderString;
		cliext::map<String^,TimeStaticBalanceType^> ^ mStaticBalance; //���˺�ͳ�ƵĽ��׽��
	public:
		CAccountTradesReportDataProvider()
		{
			mStaticBalance = gcnew cliext::map<String^,TimeStaticBalanceType^>();
			mrCloumeHeaderString = gcnew array<String^> {"����","����Ψһ�����к�","���͹�˾����","Ͷ���ߴ���","��Լ����","��������",
				"�û�����","����������","�ɽ����","��������","�������",
				"��Ա����","�ͻ�����","���׽�ɫ","��Լ�ڽ������Ĵ���","��ƽ��־",
				"Ͷ���ױ���־","�۸�","����","�ɽ�ʱ��","�ɽ�ʱ��","�ɽ�����","�ɽ�����Դ",
				"����������Ա����","���ر������","�����Ա���","ҵ��Ԫ","���","������","������","���͹�˾�������","�ɽ���Դ"};

		}

		bool StyleChart(System::Windows::Forms::DataVisualization::Charting::Chart ^ nChart)
		{
			//��ʼ������һ�ε���			
			{
				//ͼ��ı���ɫ
				nChart->BackColor = Color::FromArgb(211, 223, 240);
				//ͼ����ɫ�Ľ��䷽ʽ
				nChart->BackGradientStyle = System::Windows::Forms::DataVisualization::Charting::GradientStyle::TopBottom;
				//ͼ��ı߿���ɫ��
				nChart->BorderlineColor = Color::FromArgb(26, 59, 105);
				//ͼ��ı߿�������ʽ
				nChart->BorderlineDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::Solid;
				//ͼ��߿������Ŀ��
				nChart->BorderlineWidth = 2;
				nChart->BorderSkin->SkinStyle =  System::Windows::Forms::DataVisualization::Charting::BorderSkinStyle::Emboss;


				System::Windows::Forms::DataVisualization::Charting::Series ^ mrSeriesLine;
				System::Windows::Forms::DataVisualization::Charting::Series ^ mrSeriesDuoGraphList;
				System::Windows::Forms::DataVisualization::Charting::Series ^ mrSeriesKongGraphList;
				System::Windows::Forms::DataVisualization::Charting::ChartArea^  mrChartArea ;
				System::Windows::Forms::DataVisualization::Charting::Legend^  mrLegendLine ;
				System::Windows::Forms::DataVisualization::Charting::Legend^  mrLegendDuoGraphList;
				System::Windows::Forms::DataVisualization::Charting::Legend^  mrLegendKongGraphList;
				System::Windows::Forms::DataVisualization::Charting::Title^  mrTitle;

				mrSeriesLine = gcnew System::Windows::Forms::DataVisualization::Charting::Series();				
				mrSeriesDuoGraphList = gcnew System::Windows::Forms::DataVisualization::Charting::Series();
				mrSeriesKongGraphList = gcnew System::Windows::Forms::DataVisualization::Charting::Series();
				mrChartArea = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
				mrLegendLine = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
				mrLegendDuoGraphList = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
				mrLegendKongGraphList = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
				mrSeriesLine->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Column;
				mrSeriesDuoGraphList->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Column;
				mrSeriesKongGraphList->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Column;
				mrTitle = gcnew System::Windows::Forms::DataVisualization::Charting::Title();

				//Legend
				mrLegendLine->Name = L"LegendLine";
				mrLegendDuoGraphList->Name = L"LegendDuoGraphList";
				mrLegendKongGraphList->Name = L"LegendKongGraphList";
				mrLegendLine->Docking = System::Windows::Forms::DataVisualization::Charting::Docking::Bottom;
				mrLegendDuoGraphList->Docking = System::Windows::Forms::DataVisualization::Charting::Docking::Bottom;
				mrLegendKongGraphList->Docking = System::Windows::Forms::DataVisualization::Charting::Docking::Bottom;
				mrLegendDuoGraphList->Alignment = System::Drawing::StringAlignment::Center;
				mrLegendKongGraphList->Alignment = System::Drawing::StringAlignment::Center;
				mrLegendLine->Alignment = System::Drawing::StringAlignment::Center;


				//nrChart->BackColor = System::Drawing::Color::Transparent;
				mrChartArea->Area3DStyle->Enable3D = false;
				mrChartArea->BackColor = System::Drawing::Color::Transparent;
				mrChartArea->BackSecondaryColor = System::Drawing::Color::Transparent;
				mrChartArea->BorderColor = System::Drawing::Color::White;
				mrChartArea->Name = L"ChartArea1";
				mrChartArea->ShadowOffset = 5;			
				mrChartArea->AxisX->Interval = 1;//DateTimeIntervalType::Days;
				mrChartArea->Area3DStyle->IsClustered = true;


				//Series			
				mrSeriesDuoGraphList->ChartArea = L"ChartArea1";
				mrSeriesDuoGraphList->CustomProperties = L"DrawingStyle=Cylinder, LabelStyle=Top";
				mrSeriesDuoGraphList->Legend = L"LegendDuoGraphList";
				mrSeriesDuoGraphList->Name = L"��ͷӯ��";
				mrSeriesDuoGraphList->XValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::DateTime;

				mrSeriesKongGraphList->ChartArea = L"ChartArea1";
				mrSeriesKongGraphList->CustomProperties = L"DrawingStyle=Cylinder, LabelStyle=Top";
				mrSeriesKongGraphList->Legend = L"LegendKongGraphList";
				mrSeriesKongGraphList->Name = L"��ͷӯ��";
				mrSeriesKongGraphList->XValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::DateTime;



				mrSeriesLine->ChartArea = L"ChartArea1";				
				mrSeriesLine->Color = System::Drawing::Color::FromArgb(220,120,150,20);
				mrSeriesLine->Legend = L"LegendLine";
				mrSeriesLine->Name = L"���ն�ͷ�Ϳ�ͷ�ϼ�ӯ��";
				mrSeriesLine->XValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::DateTime;

				//������Ӱɫ��
				mrSeriesLine->ShadowColor = Color::Black;
				//��Ӱ���
				mrSeriesLine->ShadowOffset = 2;
				//�Ƿ���ʾ��������
				mrSeriesLine->IsVisibleInLegend = true;
				//���������ݵ����Ƿ�ϡ�о���ʾ
				mrSeriesLine->IsValueShownAsLabel = false;
				//�����ϵ����ݵ�������
				mrSeriesLine->MarkerStyle = System::Windows::Forms::DataVisualization::Charting::MarkerStyle::Circle;
				//�������ݵ�Ĵ�С
				mrSeriesLine->MarkerSize = 10;

				nChart->Legends->Clear();						
				nChart->Legends->Add(mrLegendLine);
				//nChart->Legends->Add(mrLegendDuoGraphList);
				//nChart->Legends->Add(mrLegendKongGraphList);
				//chart area
				nChart->ChartAreas->Clear();					
				nChart->ChartAreas->Add(mrChartArea);				

				//Series
				nChart->Series->Clear();
				nChart->Series->Add(mrSeriesLine);
				//nChart->Series->Add(mrSeriesDuoGraphList);
				//nChart->Series->Add(mrSeriesKongGraphList);
				nChart->Titles->Add(mrTitle);	
			

				mrSeriesLine->Points->Clear();
				mrSeriesDuoGraphList->Points->Clear();
				mrSeriesDuoGraphList->Points->Clear();	
	
				//nrChart->DataSource = RealDataTable;
			}
			return true;
		}
	
	public:
		//�����¼�
		virtual void SubscribeEvt(CQueryConditions^ nCondition,IntPtr hWnd) override
		{
			CTcpLayer::SubscribePkg(Cmd_RM_QryHistroyRiskFund_Rsp,(int)hWnd);		
			CTcpLayer::SubscribePkg(Cmd_RM_QryHistroyTrade_Rsp,(int)hWnd);
		
			//CTcpLayer::SendData()
			HisTradeDataQueryKey lHisQuery ;
			memset(&lHisQuery,0,sizeof(lHisQuery));
			std::string lstrStartTime = Tools::String2string(nCondition->mstartTime.ToString("yyyy-MM-dd"));
			std::string lstrEndTime = Tools::String2string(nCondition->mendTime.ToString("yyyy-MM-dd"));
			strcpy(lHisQuery.TradingDayFrom,lstrStartTime.c_str());
			strcpy(lHisQuery.TradingDayTo,lstrEndTime.c_str());
			CTcpLayer::SendData(Cmd_RM_QryHistroyRiskFund_Req, (void*)&lHisQuery, sizeof(HisTradeDataQueryKey), 0);
			CTcpLayer::SendData(Cmd_RM_QryHistroyTrade_Req, (void*)&lHisQuery, sizeof(HisTradeDataQueryKey), 0);
			
		}
		//�˶��¼�
		virtual void UnsubscribeEvt(unsigned int hWnd) override
		{
			CTcpLayer::UnsubscribeAllPkg((int)hWnd);
		}

		//ִ��
		virtual bool ProcessData(char*  npRespond) override
		{
			if(npRespond == NULL)
				return false;
			Stru_UniPkgHead head;	
			memset(&head, 0, sizeof(head));
			memcpy(&head, npRespond, sizeof(head));
			if(head.cmdid == Cmd_RM_QryHistroyRiskFund_Rsp && head.userdata1 == OPERATOR_SUCCESS)
			{
				int count= head.len/sizeof(RiskAllAccountField);
				if(count != 0)
				{					
					mStaticBalance->clear();
					RiskAllAccountField* pStatic = NULL;
					for(Int32 i = 0; i < count;i++)
					{
						pStatic = (RiskAllAccountField*)
							(npRespond+sizeof(Stru_UniPkgHead)+i*sizeof(RiskAllAccountField));

						//����
						if(FilterAccount(std::string(pStatic->pre.InvestorID)))
							continue;
					
						//ͳ��
						//���ҵ��ʺ�
						String ^ lstrName = Tools::string2String(pStatic->pre.InvestorID);					
						cliext::map<String^,TimeStaticBalanceType^>::iterator lIterStatic = mStaticBalance->find(lstrName);
						if(lIterStatic != mStaticBalance->end())
						{
							//������
							TimeStaticBalanceType ^ ltempTimeHode = lIterStatic->second;
							String ^ lstrTime = Tools::string2String(pStatic->cur.RecvTime);						
							TimeStaticBalanceType::iterator lIterTime = ltempTimeHode->find(lstrTime);

							if(lIterTime == ltempTimeHode->end())
							{
								//û���ҵ���Ӧ���ڵļ�¼������һ��
								cliext::vector<Double> ^ lTempVector = gcnew cliext::vector<Double>();
								lTempVector->push_back(pStatic->cur.Balance-pStatic->pre.PreBalance);							
								ltempTimeHode->insert(cliext::make_pair(lstrTime,lTempVector));
							}
							else
							{
								//�ҵ���Ӧ���ڵļ�¼���޸�
								cliext::vector<Double> ^ lTempVector = lIterTime->second;
								lTempVector[0] += (pStatic->cur.Balance - pStatic->pre.PreBalance);								
							}
						}
						else
						{
							//û���ҵ���Ӧ���ʺţ�����һ��
							TimeStaticBalanceType ^ ltempTimeHode = gcnew TimeStaticBalanceType();
							String ^ lstrTime = Tools::string2String(pStatic->pre.RecvTime);							
							cliext::vector<Double> ^ lTempVector = gcnew cliext::vector<Double>();
							lTempVector->push_back(pStatic->cur.Balance - pStatic->pre.PreBalance);							
							ltempTimeHode->insert(cliext::make_pair(lstrTime,lTempVector));

							mStaticBalance->insert(cliext::make_pair(lstrName,ltempTimeHode));

						}					
					}
				}
			}		
			if(head.cmdid == Cmd_RM_QryHistroyTrade_Rsp && head.userdata1 == OPERATOR_SUCCESS)
			{
				int count= head.len/sizeof(SequencialTradeField);
				if(count != 0)
				{
					RealDataTable->Rows->Clear();
					RealDataTable->Columns->Clear();
					for(Int32 i = 0; i < mrCloumeHeaderString->Length;i++)
					{
						RealDataTable->Columns->Add(mrCloumeHeaderString[i]);
					}
					SequencialTradeField* pStatic = NULL;
					for(Int32 i = 0; i < count;i++)
					{
						pStatic = (SequencialTradeField*)
							(npRespond+sizeof(Stru_UniPkgHead)+i*sizeof(SequencialTradeField));

						if(FilterAccount(std::string(pStatic->InvestorID)))
							continue;
						if(FilterInstrument(std::string(pStatic->InstrumentID)))
							continue;

						DataRow ^ nrDataRow = RealDataTable->NewRow();						
						nrDataRow[mrCloumeHeaderString[0]] =  Tools::string2String(pStatic->TradingDay);				
						nrDataRow[mrCloumeHeaderString[1]] =  pStatic->UniqSequenceNo;
						nrDataRow[mrCloumeHeaderString[2]] =  Tools::string2String(pStatic->BrokerID);
						nrDataRow[mrCloumeHeaderString[3]] =  Tools::string2String(pStatic->InvestorID);
						nrDataRow[mrCloumeHeaderString[4]] =  Tools::string2String(pStatic->InstrumentID);						
						nrDataRow[mrCloumeHeaderString[5]] =  Tools::string2String(pStatic->OrderRef);

						nrDataRow[mrCloumeHeaderString[6]] =  Tools::string2String(pStatic->UserID);
						nrDataRow[mrCloumeHeaderString[7]] =  Tools::string2String(pStatic->ExchangeID);
						nrDataRow[mrCloumeHeaderString[8]] =  Tools::string2String(pStatic->TradeID);
						
						nrDataRow[mrCloumeHeaderString[9]] =  Tools::string2String(Client::CFieldDetail::Direction2String(pStatic->Direction));
						nrDataRow[mrCloumeHeaderString[10]] = Tools::string2String(pStatic->OrderSysID);
						nrDataRow[mrCloumeHeaderString[11]] = Tools::string2String(pStatic->ParticipantID);
						nrDataRow[mrCloumeHeaderString[12]] = Tools::string2String(pStatic->ClientID);
						nrDataRow[mrCloumeHeaderString[13]] = Tools::string2String(Client::CFieldDetail::TraderRole2String(pStatic->TradingRole));
						nrDataRow[mrCloumeHeaderString[14]] = Tools::string2String(pStatic->ExchangeInstID);
						nrDataRow[mrCloumeHeaderString[15]] = Tools::string2String(Client::CFieldDetail::OffsetFlag2String(pStatic->OffsetFlag));				
						nrDataRow[mrCloumeHeaderString[16]] = Tools::string2String(Client::CFieldDetail::HedgeFlag2String(pStatic->HedgeFlag));
						nrDataRow[mrCloumeHeaderString[17]] = pStatic->Price;
						nrDataRow[mrCloumeHeaderString[18]] = pStatic->Volume;
						nrDataRow[mrCloumeHeaderString[19]] = Tools::string2String(pStatic->TradeDate);
						nrDataRow[mrCloumeHeaderString[20]] = Tools::string2String(pStatic->TradeTime);
						nrDataRow[mrCloumeHeaderString[21]] = Tools::string2String(Client::CFieldDetail::TradeType2String(pStatic->TradeType));
						nrDataRow[mrCloumeHeaderString[22]] = Tools::string2String(Client::CFieldDetail::PriceType2String(pStatic->PriceSource));
						nrDataRow[mrCloumeHeaderString[23]] = Tools::string2String(pStatic->TraderID);
						nrDataRow[mrCloumeHeaderString[24]] = Tools::string2String(pStatic->OrderLocalID);
						nrDataRow[mrCloumeHeaderString[25]] = Tools::string2String(pStatic->ClearingPartID);
						nrDataRow[mrCloumeHeaderString[26]] = Tools::string2String(pStatic->BusinessUnit);
						nrDataRow[mrCloumeHeaderString[27]] = pStatic->SequenceNo;
						nrDataRow[mrCloumeHeaderString[28]] =  Tools::string2String(pStatic->TradingDay);
						nrDataRow[mrCloumeHeaderString[29]] = pStatic->SettlementID;
						nrDataRow[mrCloumeHeaderString[30]] = pStatic->BrokerOrderSeq;
						nrDataRow[mrCloumeHeaderString[31]] = Tools::string2String(Client::CFieldDetail::TradeSourceString(pStatic->TradeSource));
						
						RealDataTable->Rows->Add(nrDataRow);
					}


				}
			}
			return true;
		}

		//���ݽ��սӿ�
		virtual bool OnReponseDataReciver() override
		{
			return true;
		}

		//���ɱ�ͷ
		virtual bool GenerateGridViewHeader() override
		{
			/*;
			for(Int32 i = 0; i < mrCloumeHeaderString->Length;i++)
			{
				RealDataTable->Columns->Add(mrCloumeHeaderString[i]);
			}*/
			return true;
		}
		//��GridView
		virtual bool GenerateDataTable(CQueryConditions^ nCondition) override
		{
			//if(nCondition)
			//{
			//	//nrView->Rows->Clear();
			//	RealDataTable->Rows->Clear();
			//	RealDataTable->Columns->Clear();
			//	for(Int32 i = 0; i < mrCloumeHeaderString->Length;i++)
			//	{
			//		RealDataTable->Columns->Add(mrCloumeHeaderString[i]);
			//	}
			//	//sample data
			//	DateTime ^ nrDateTime = nCondition->mstartTime;
			//	for(Int32 i = 0; i < 100;i++)
			//	{
			//		DataRow ^ nrDataRow = RealDataTable->NewRow();
			//		nrDateTime = nrDateTime->AddDays(2.0);
			//		nrDataRow[mrCloumeHeaderString[0]] =  nrDateTime->ToString("yyyy-MM-dd");				
			//		nrDataRow[mrCloumeHeaderString[1]] = "001";
			//		nrDataRow[mrCloumeHeaderString[2]] = "188001";
			//		nrDataRow[mrCloumeHeaderString[3]] = "483024.00";
			//		nrDataRow[mrCloumeHeaderString[4]] = "60";
			//		nrDataRow[mrCloumeHeaderString[5]] = "434344.00";
			//		nrDataRow[mrCloumeHeaderString[6]] = "100000.00";
			//		nrDataRow[mrCloumeHeaderString[7]] = "84032.00";
			//		nrDataRow[mrCloumeHeaderString[8]] = "84032.00";
			//		nrDataRow[mrCloumeHeaderString[9]] = "4348.00";
			//		nrDataRow[mrCloumeHeaderString[10]] = "432424.00";
			//		nrDataRow[mrCloumeHeaderString[11]] = "6";
			//		nrDataRow[mrCloumeHeaderString[12]] = "44443.00";
			//		nrDataRow[mrCloumeHeaderString[13]] = "44443.00";
			//		nrDataRow[mrCloumeHeaderString[14]] = "90000.00";
			//		nrDataRow[mrCloumeHeaderString[15]] = "54344.00";
			//		nrDataRow[mrCloumeHeaderString[16]] = "2444";
			//		nrDataRow[mrCloumeHeaderString[17]] = "6000";
			//		nrDataRow[mrCloumeHeaderString[18]] = "100000";
			//		nrDataRow[mrCloumeHeaderString[19]] = System::Random::Random(Guid::NewGuid().GetHashCode()).Next(600000,1000000);
			//		nrDataRow[mrCloumeHeaderString[20]] = System::Random::Random(Guid::NewGuid().GetHashCode()).Next(10000,300000) ;
			//		nrDataRow[mrCloumeHeaderString[21]] = System::Random::Random(Guid::NewGuid().GetHashCode()).Next(10000,400000) ;
			//		nrDataRow[mrCloumeHeaderString[22]] = "424434";
			//		nrDataRow[mrCloumeHeaderString[23]] = "233";
			//		nrDataRow[mrCloumeHeaderString[24]] = "4444";
			//		nrDataRow[mrCloumeHeaderString[25]] = "43455";
			//		nrDataRow[mrCloumeHeaderString[26]] = "424434";
			//		nrDataRow[mrCloumeHeaderString[27]] = "233";
			//		nrDataRow[mrCloumeHeaderString[28]] = "4444";
			//		nrDataRow[mrCloumeHeaderString[29]] = "424434";
			//		nrDataRow[mrCloumeHeaderString[30]] = "233";
			//		nrDataRow[mrCloumeHeaderString[31]] = "4444";
			//		RealDataTable->Rows->Add(nrDataRow);
			//	}
			//	//nrView->DataSource = RealDataTable;				
			//}
			return true;
		
		}

		//���chart
		virtual bool FillChart(System::Windows::Forms::FlowLayoutPanel^ nrChartContainer) override
		{
			if(nrChartContainer)
			{
				nrChartContainer->Controls->Clear();
				nrChartContainer->Update();
				cliext::map<String^,TimeStaticBalanceType^>::iterator lIterStatic = mStaticBalance->begin();
				for(;lIterStatic != mStaticBalance->end(); lIterStatic++)
				{


					System::Windows::Forms::DataVisualization::Charting::Chart ^ lChart = 
						gcnew System::Windows::Forms::DataVisualization::Charting::Chart();

					lChart->Size = System::Drawing::Size(800,400);
					StyleChart(lChart);

					lChart->Titles[0]->Text = lIterStatic->first;

					//������ݵ�serial
					TimeStaticBalanceType ^ ltempTimeHode = lIterStatic->second;				
					TimeStaticBalanceType::iterator lIterTime ;
					Double ldbTemp = 0.0;
					for (lIterTime = ltempTimeHode->begin(); lIterTime != ltempTimeHode->end();lIterTime++)
					{
						cliext::vector<Double> ^ lTempVector = lIterTime->second;
						ldbTemp += lTempVector[0];					
						lChart->Series["���ն�ͷ�Ϳ�ͷ�ϼ�ӯ��"]->Points->AddXY(lIterTime->first,ldbTemp);
						
						//lChart->Series["ÿ��Ȩ��仯��"]->Points->AddXY(lIterTime->first,lTempVector[0] - lTempVector[1]);
					}

					//���tooltip�¼�
					lChart->GetToolTipText += gcnew EventHandler<ToolTipEventArgs^>(this,&CAccountTradesReportDataProvider::ChartLineGetToolTipText);
					nrChartContainer->Controls->Add(lChart);
				}

			}
			return true;

		
		}  

		private: void ChartLineGetToolTipText(Object^ sender,ToolTipEventArgs^ e)
    	{

			 if(e->HitTestResult->ChartElementType == ChartElementType::DataPoint)
			 {

				 int i = e->HitTestResult->PointIndex;
				 DataPoint^ dp = e->HitTestResult->Series->Points[i];

				 e->Text = String::Format("����:{0};��ֵ:{1:F1}",dp->AxisLabel,dp->YValues[0]);
			 }

		}
	};
}