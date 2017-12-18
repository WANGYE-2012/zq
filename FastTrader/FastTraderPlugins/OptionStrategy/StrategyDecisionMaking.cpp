#include "StdAfx.h"
#include "StrategyDecisionMaking.h"
#include "CommPublic.h"

StrategyDecisionMaking::StrategyDecisionMaking(void)
{
}

StrategyDecisionMaking::~StrategyDecisionMaking(void)
{
}

bool StrategyDecisionMaking::GetOptionStrategyOrders( eStrategyID     strategyID,
							 const string      strUnderlyingInstrumentID,
							 vector<PlatformStru_InstrumentInfo>  vecOptionIns,
							 map<string,PlatformStru_DepthMarketData>   maplastQuot,

							 vector<PlatformStru_OrderInfo>&      outOrders)
{

	if(strUnderlyingInstrumentID.empty() || vecOptionIns.size() ==0 || maplastQuot.size() ==0 )
		return false;
	

	PlatformStru_OrderInfo order;
	memset(&order,0,sizeof(order));
	memcpy(order.Account,CDataInfo::NewInstance()->GetAccount().c_str(),sizeof(order.Account));
	order.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	order.CombOffsetFlag[0] = THOST_FTDC_OF_Open;

	switch(strategyID)
	{
	case StrategyID_Rose://. ����������һ����Ȩ����С����ֵ������Ȩ��Լ��
		{			
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			vector<PlatformStru_InstrumentInfo> vecTempIns;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())					 
				{
					if (itIns->StrikePrice > itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);				
					}
				}
				
			}
			GetMiniPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_Crash://. ����������һ����Ȩ��������ֵ������Ȩ��Լ��
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			vector<PlatformStru_InstrumentInfo> vecTempIns;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			
			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions  && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);		
					}
				}
				
			}
			GetMaxPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_Peakback://.��������  ���������һ����Ȩ����С����ֵ������Ȩ��Լ��
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			vector<PlatformStru_InstrumentInfo> vecTempIns;
			
			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			
			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice > itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);	
					}
				}
				
			}
			GetMiniPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_Bottomout://. ���׻����� ���������һ����Ȩ��������ֵ������Ȩ��Լ
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			vector<PlatformStru_InstrumentInfo> vecTempIns;
			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);	

					}
				}
				
			}
			GetMaxPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_Breakout_Straddle://. ͻ��-�����ʽ���ԡ� 1. ����������һ����Ȩ������ʵֵ������Ȩ��Լ ͬʱ ����������һ����ͬ��Ȩ�۵Ŀ�����Ȩ��Լ��
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			vector<PlatformStru_InstrumentInfo> vecTempIns;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);	

					}
				}
				
			}
			GetMaxPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

			memset(&order,0,sizeof(order));
			itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice == Ins.StrikePrice)
					{				
						Ins=*itIns;
					}
				}
				
			}
			itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_Breakout_Strangle://. ͻ��-������ʽ���ԡ� 1 ���������һ����Ȩ�۽�С����ֵ������Ȩ��Լ ͬʱ ����������һ����Ȩ�۽ϴ����ֵ������Ȩ��Լ
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			double dbPrice=0.0;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			dbPrice = itFuture->second.LastPrice;

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			vector<PlatformStru_InstrumentInfo> vecTempIns;
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
				    if (itIns->StrikePrice > itFuture->second.LastPrice)
				    {
					    vecTempIns.push_back(*itIns);					

				    }
				}
			}

			GetMiniPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

			memset(&order,0,sizeof(order));
			vecTempIns.clear();
			itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{				
						vecTempIns.push_back(*itIns);	
					}
				}
				
			}

			GetMaxPriceIns(vecTempIns,Ins);
			itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_Consolidation_Straddle://.����-������ʽ���ԡ� ���������һ����Ȩ�۽ϴ��ʵֵ������Ȩ��Լ ͬʱ ���������һ����ͬ������ ��ͬ��Ȩ�۵Ŀ�����Ȩ��
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			vector<PlatformStru_InstrumentInfo> vecTempIns;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);	

					}
				}
				
			}
			GetMaxPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

			memset(&order,0,sizeof(order));
			itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice == Ins.StrikePrice)
					{				
						Ins=*itIns;
					}
				}
				
			}
			itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_Consolidation_Strangle://. ����-������ʽ���ԡ� ���������һ����Ȩ�۽�С����ֵ������Ȩ��Լ ͬʱ ���������һ����Ȩ�۽ϴ����ֵ������Ȩ��
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			double dbPrice=0.0;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			dbPrice = itFuture->second.LastPrice;

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			vector<PlatformStru_InstrumentInfo> vecTempIns;
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice > itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);					

					}
				}
				
			}

			GetMiniPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

			memset(&order,0,sizeof(order));
			vecTempIns.clear();
			itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{				
						vecTempIns.push_back(*itIns);	
					}
				}
				
			}

			GetMaxPriceIns(vecTempIns,Ins);
			itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_ConsolidationUp_Call://.����Call�� ����������һ����Ȩ�۽ϴ��ʵֵ������Ȩ��Լ ͬʱ ���������һ����Ȩ�۽�С����ֵ������Ȩ��
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			double dbPrice=0.0;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			dbPrice = itFuture->second.LastPrice;

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			vector<PlatformStru_InstrumentInfo> vecTempIns;
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);					

					}
				}
				
			}

			GetMaxPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

			memset(&order,0,sizeof(order));
			vecTempIns.clear();
			itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice > itFuture->second.LastPrice)
					{				
						vecTempIns.push_back(*itIns);	
					}
				}
				
			}

			GetMiniPriceIns(vecTempIns,Ins);
			itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_ConsolidationUp_Put://.����Put�� ����������һ����Ȩ�۽ϴ����ֵ������Ȩ��Լ ͬʱ ���������һ����Ȩ�۽�С��ʵֵ������Ȩ
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			double dbPrice=0.0;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			dbPrice = itFuture->second.LastPrice;

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			vector<PlatformStru_InstrumentInfo> vecTempIns;
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice< itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);					

					}
				}
				
			}

			GetMaxPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

			memset(&order,0,sizeof(order));
			vecTempIns.clear();
			itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice > itFuture->second.LastPrice)
					{				
						vecTempIns.push_back(*itIns);	
					}
				}
				
			}

			GetMiniPriceIns(vecTempIns,Ins);
			itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_ConsolidationDown_Call://.�̵�Call�� ����������һ����Ȩ�۽�С����ֵ������Ȩ��Լ ͬʱ ���������һ����Ȩ�۽ϴ��ʵֵ������Ȩ
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			double dbPrice=0.0;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			dbPrice = itFuture->second.LastPrice;

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			vector<PlatformStru_InstrumentInfo> vecTempIns;
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice > itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);					

					}
				}
				
			}

			GetMiniPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

			memset(&order,0,sizeof(order));
			vecTempIns.clear();
			itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{				
						vecTempIns.push_back(*itIns);	
					}
				}
				
			}

			GetMaxPriceIns(vecTempIns,Ins);
			itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_ConsolidationDown_Put://.�̵�Put�� ������������һ����Ȩ�۽�С��ʵֵ������Ȩ��Լ ͬʱ ���������һ����Ȩ�۽ϴ����ֵ������Ȩ��
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			double dbPrice=0.0;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			dbPrice = itFuture->second.LastPrice;

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			vector<PlatformStru_InstrumentInfo> vecTempIns;
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice > itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);					

					}
				}
				
			}

			GetMiniPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

			memset(&order,0,sizeof(order));
			vecTempIns.clear();
			itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())					
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{				
						vecTempIns.push_back(*itIns);	
					}
				}
				
			}

			GetMaxPriceIns(vecTempIns,Ins);
			itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	default:
		break;
	}
	return true;

}
void StrategyDecisionMaking::GetMiniPriceIns(vector<PlatformStru_InstrumentInfo>& vecTempIns,PlatformStru_InstrumentInfo& outData)
{
    if(vecTempIns.empty())
        return;

	vector<PlatformStru_InstrumentInfo>::iterator it = vecTempIns.begin();
	outData = *it;
	for (++it; it !=vecTempIns.end(); it++)
	{

		if ( it->StrikePrice < outData.StrikePrice )
		{
			outData = *it;
		}
	}

	
}

void StrategyDecisionMaking::GetMaxPriceIns(vector<PlatformStru_InstrumentInfo>& vecTempIns,PlatformStru_InstrumentInfo& outData)
{

	vector<PlatformStru_InstrumentInfo>::iterator it = vecTempIns.begin();
	outData = *it;
	for (++it; it !=vecTempIns.end(); it++)
	{

		if ( it->StrikePrice > outData.StrikePrice )
		{
			outData = *it;
		}
	}


}