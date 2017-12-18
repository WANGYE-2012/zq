#pragma once
#include "AbstractReportDataProvider.h"
using namespace System;
using namespace System::Data;
using namespace System::Windows::Forms::DataVisualization::Charting;


namespace ReportDataProvider {


	
	public ref class CAccountInterestReportDataProvider :public IReportDataProvider
	{	
	private:
		

		array<String^>^ mrCloumeHeaderString;
		cliext::map<String^,TimeStaticBalanceType^> ^ mStaticBalance; //���˺�ͳ�Ƶĳֲֽ��
	public:
		CAccountInterestReportDataProvider()
		{
			mStaticBalance = gcnew cliext::map<String^,TimeStaticBalanceType^>();
			mrCloumeHeaderString = gcnew array<String^>{"����","���͹�˾����","Ͷ���ߴ���","Ͷ�����˺�","�ϴ���Ѻ���","�ϴ����ö��","�ϴδ���",
				"�ϴ���Ȩ��","�ϴ�ռ�õı�֤��","����׼����","�ϴν�������֤��","��ʷǿƽ����",
				"Ͷ���߽��֤��","���������֤��","�������","��ǰ��֤���ܶ�","������","ƽ��ӯ��",
				"�ֲ�ӯ��","��Ȩ��","�����ʽ�","��Ѻ���","��������֤��","��ȡ�ʽ�","���ö��",
				"��֤�𶳽�","�����Ѷ���"};
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
				System::Windows::Forms::DataVisualization::Charting::Series ^ mrSeriesGraphList;
				System::Windows::Forms::DataVisualization::Charting::ChartArea^  mrChartArea ;
				System::Windows::Forms::DataVisualization::Charting::Legend^  mrLegendLine ;
				System::Windows::Forms::DataVisualization::Charting::Legend^  mrLegendGraphList;
				System::Windows::Forms::DataVisualization::Charting::Title^  mrTitle;

				mrSeriesLine = gcnew System::Windows::Forms::DataVisualization::Charting::Series();				
				mrSeriesGraphList = gcnew System::Windows::Forms::DataVisualization::Charting::Series();
				mrChartArea = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
				mrLegendLine = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
				mrLegendGraphList = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
				mrSeriesLine->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
				mrSeriesGraphList->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Column;
				mrTitle = gcnew System::Windows::Forms::DataVisualization::Charting::Title();

				//Legend
				mrLegendLine->Name = L"LegendLine";
				mrLegendGraphList->Name = L"LegendGraphList";
				mrLegendLine->Docking = System::Windows::Forms::DataVisualization::Charting::Docking::Bottom;
				mrLegendGraphList->Docking = System::Windows::Forms::DataVisualization::Charting::Docking::Bottom;
				mrLegendGraphList->Alignment = System::Drawing::StringAlignment::Center;
				mrLegendLine->Alignment = System::Drawing::StringAlignment::Center;


				//nrChart->BackColor = System::Drawing::Color::Transparent;
				mrChartArea->Area3DStyle->Enable3D = false;
				mrChartArea->BackColor = System::Drawing::Color::Transparent;
				mrChartArea->BackSecondaryColor = System::Drawing::Color::Transparent;
				mrChartArea->BorderColor = System::Drawing::Color::White;
				mrChartArea->Name = L"ChartArea1";
				mrChartArea->ShadowOffset = 5;			
				mrChartArea->AxisX->Interval = 1;//DateTimeIntervalType::Days;

				//Series
			
				mrSeriesGraphList->ChartArea = L"ChartArea1";
				mrSeriesGraphList->CustomProperties = L"DrawingStyle=Cylinder, LabelStyle=Top";
				mrSeriesGraphList->Legend = L"LegendGraphList";
				mrSeriesGraphList->Name = L"ÿ��Ȩ��仯��";
				mrSeriesGraphList->XValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::DateTime;

				mrSeriesLine->ChartArea = L"ChartArea1";				
				mrSeriesLine->Color = System::Drawing::Color::FromArgb(220,120,150,20);
				mrSeriesLine->Legend = L"LegendLine";
				mrSeriesLine->Name = L"����Ȩ������";
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
				nChart->Legends->Add(mrLegendGraphList);
				//chart area
				nChart->ChartAreas->Clear();				
				nChart->ChartAreas->Add(mrChartArea);			

				//Series
				nChart->Series->Clear();
				nChart->Series->Add(mrSeriesLine);
				nChart->Series->Add(mrSeriesGraphList);
				nChart->Titles->Add(mrTitle);			

				mrSeriesLine->Points->Clear();
				mrSeriesGraphList->Points->Clear();			
				//nrChart->DataSource = RealDataTable;
			}
			return true;
		}
	public:
		//�����¼�
		virtual void SubscribeEvt(CQueryConditions^ nCondition,IntPtr hWnd) override
		{
			CTcpLayer::SubscribePkg(Cmd_RM_QryHistroyRiskFund_Rsp,(int)hWnd);
			//CTcpLayer::SubscribePkg(Cmd_RM_QryHistroyTrade_Rsp,(int)hWnd);

			//CTcpLayer::SendData()
			HisTradeDataQueryKey lHisQuery ;
			memset(&lHisQuery,0,sizeof(lHisQuery));
			std::string lstrStartTime = Tools::String2string(nCondition->mstartTime.ToString("yyyy-MM-dd"));
			std::string lstrEndTime = Tools::String2string(nCondition->mendTime.ToString("yyyy-MM-dd"));
			strcpy(lHisQuery.TradingDayFrom,lstrStartTime.c_str());
			strcpy(lHisQuery.TradingDayTo,lstrEndTime.c_str());
			CTcpLayer::SendData(Cmd_RM_QryHistroyRiskFund_Req, (void*)&lHisQuery, sizeof(HisTradeDataQueryKey), 0);
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
					RealDataTable->Rows->Clear();
					RealDataTable->Columns->Clear();
					for(Int32 i = 0; i < mrCloumeHeaderString->Length;i++)
					{
						RealDataTable->Columns->Add(mrCloumeHeaderString[i]);
					}
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
								lTempVector->push_back(pStatic->cur.Balance);
								lTempVector->push_back(pStatic->pre.PreBalance);
								ltempTimeHode->insert(cliext::make_pair(lstrTime,lTempVector));
							}
							else
							{
								//�ҵ���Ӧ���ڵļ�¼���޸�
								cliext::vector<Double> ^ lTempVector = lIterTime->second;
								lTempVector[0] += pStatic->cur.Balance;
								lTempVector[1] += pStatic->pre.PreBalance;
							}
						}
						else
						{
							//û���ҵ���Ӧ���ʺţ�����һ��
							TimeStaticBalanceType ^ ltempTimeHode = gcnew TimeStaticBalanceType();
							String ^ lstrTime = Tools::string2String(pStatic->pre.RecvTime);							
							cliext::vector<Double> ^ lTempVector = gcnew cliext::vector<Double>();
							lTempVector->push_back(pStatic->cur.Balance);
							lTempVector->push_back(pStatic->pre.PreBalance);
							ltempTimeHode->insert(cliext::make_pair(lstrTime,lTempVector));

							mStaticBalance->insert(cliext::make_pair(lstrName,ltempTimeHode));

						}
						DataRow ^ nrDataRow = RealDataTable->NewRow();						
						nrDataRow[mrCloumeHeaderString[0]] =  Tools::string2String(pStatic->cur.RecvTime);				
						nrDataRow[mrCloumeHeaderString[1]] =  Tools::string2String(pStatic->pre.BrokerID);
						nrDataRow[mrCloumeHeaderString[2]] =  Tools::string2String(pStatic->pre.InvestorID);
						nrDataRow[mrCloumeHeaderString[3]] =  Tools::string2String(pStatic->pre.AccountID);
						nrDataRow[mrCloumeHeaderString[4]] =  Convert::ToDouble(pStatic->pre.PreMortgage).ToString("N2");
						nrDataRow[mrCloumeHeaderString[5]] =  Convert::ToDouble(pStatic->pre.PreCredit).ToString("N2");						
						nrDataRow[mrCloumeHeaderString[6]] =  Convert::ToDouble(pStatic->pre.PreDeposit).ToString("N2");

						nrDataRow[mrCloumeHeaderString[7]] =  Convert::ToDouble(pStatic->pre.PreBalance).ToString("N2");
						nrDataRow[mrCloumeHeaderString[8]] =  Convert::ToDouble(pStatic->pre.PreMargin).ToString("N2");
						nrDataRow[mrCloumeHeaderString[9]] =  Convert::ToDouble(pStatic->pre.Reserve).ToString("N2");						
						nrDataRow[mrCloumeHeaderString[10]] = Convert::ToDouble(pStatic->pre.PreExchMargin).ToString("N2");
						nrDataRow[mrCloumeHeaderString[11]] = pStatic->pre.ForceCloseStat;
						nrDataRow[mrCloumeHeaderString[12]] = Convert::ToDouble(pStatic->pre.DeliveryMargin).ToString("N2");
						nrDataRow[mrCloumeHeaderString[13]] = Convert::ToDouble(pStatic->pre.ExchangeDeliveryMargin).ToString("N2");
						nrDataRow[mrCloumeHeaderString[14]] = Convert::ToDouble(pStatic->cur.Withdraw).ToString("N2");
						nrDataRow[mrCloumeHeaderString[15]] = Convert::ToDouble(pStatic->cur.CurrMargin).ToString("N2");
						nrDataRow[mrCloumeHeaderString[16]] = Convert::ToDouble(pStatic->cur.Commission).ToString("N2");					
						nrDataRow[mrCloumeHeaderString[17]] = Convert::ToDouble(pStatic->cur.CloseProfit).ToString("N2");
						nrDataRow[mrCloumeHeaderString[18]] = Convert::ToDouble(pStatic->cur.PositionProfit).ToString("N2");
						nrDataRow[mrCloumeHeaderString[19]] = Convert::ToDouble(pStatic->cur.Balance).ToString("N2");
						nrDataRow[mrCloumeHeaderString[20]] = Convert::ToDouble(pStatic->cur.Available).ToString("N2");
						nrDataRow[mrCloumeHeaderString[21]] = Convert::ToDouble(pStatic->cur.Mortgage).ToString("N2");
						nrDataRow[mrCloumeHeaderString[22]] = Convert::ToDouble(pStatic->cur.ExchangeMargin).ToString("N2");
						nrDataRow[mrCloumeHeaderString[23]] = Convert::ToDouble(pStatic->cur.WithdrawQuota).ToString("N2");
						nrDataRow[mrCloumeHeaderString[24]] = Convert::ToDouble(pStatic->cur.Credit).ToString("N2");
						nrDataRow[mrCloumeHeaderString[25]] = Convert::ToDouble(pStatic->cur.FrozenMargin).ToString("N2");
						nrDataRow[mrCloumeHeaderString[26]] = Convert::ToDouble(pStatic->cur.FrozenCommission).ToString("N2");
					

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
			/*for(Int32 i = 0; i < mrCloumeHeaderString->Length;i++)
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
			//		nrDataRow[mrCloumeHeaderString[19]] = System::Random::Random(Guid::NewGuid().GetHashCode()).Next(10000,1000000);
			//		nrDataRow[mrCloumeHeaderString[20]] = i % 2 ?System::Random::Random(Guid::NewGuid().GetHashCode()).Next(-600000,600000) :-System::Random::Random(Guid::NewGuid().GetHashCode()).Next(-600000,600000);
			//		nrDataRow[mrCloumeHeaderString[21]] = "43455";
			//		nrDataRow[mrCloumeHeaderString[22]] = "424434";
			//		nrDataRow[mrCloumeHeaderString[23]] = "233";
			//		nrDataRow[mrCloumeHeaderString[24]] = "4444";
			//	
			//		RealDataTable->Rows->Add(nrDataRow);
			//	}
			//	//nrView->DataSource = RealDataTable;				
			//}
			return true;
		}

		//���chart
		virtual bool FillChart(System::Windows::Forms::FlowLayoutPanel^ nrChartContainer) override
		{
			//����һ��chartarea ������serial��chart
			//����һ��serialΪ����ͼ ����һ��Ϊ��״ͼ
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
					for (lIterTime = ltempTimeHode->begin(); lIterTime != ltempTimeHode->end();lIterTime++)
					{
						cliext::vector<Double> ^ lTempVector = lIterTime->second;
						lChart->Series["����Ȩ������"]->Points->AddXY(lIterTime->first,lTempVector[0]);
						lChart->Series["ÿ��Ȩ��仯��"]->Points->AddXY(lIterTime->first,lTempVector[0] - lTempVector[1]);
					}

					//���tooltip�¼�
					lChart->GetToolTipText += gcnew EventHandler<ToolTipEventArgs^>(this,&CAccountInterestReportDataProvider::ChartLineGetToolTipText);
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