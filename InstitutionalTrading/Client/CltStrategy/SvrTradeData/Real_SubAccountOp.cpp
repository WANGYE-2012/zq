#include "stdafx.h"
#include "Real_SubAccountOp.h"
#include "CommonMacro.h"
#include "CommonDataChangeEvent.h"
//#include "..\SvrNotifyAndAsk\Interface_SvrNotifyAndAsk.h"


void CSubAccountOp::clear()
{
	if(mpPlatformDataRates)
		mpPlatformDataRates->clear();
	InterlockedExchange((volatile long*)(&meReady),CommissionMargin_NoReady);	
	SetAccountCTPStatus(ACCOUNT_STATUS_UnInit);
}


CSubAccountOp::~CSubAccountOp()
{
	CSafeLock ll(mMutexData);
	if(mpPlatformTraderAccount)
		delete mpPlatformTraderAccount;
	if(mpPlatformPositionsCombs)
		delete mpPlatformPositionsCombs;
	if(mpPlatformPositions)
		delete mpPlatformPositions;
	if(mpPlatformPositionDetail)
		delete mpPlatformPositionDetail;
	if(mpPlatformPositionDetailCombs)
		delete mpPlatformPositionDetailCombs;
	if(mpPlatformTraderTotal)
		delete mpPlatformTraderTotal;
	if(mpPlatformTraders)
		delete mpPlatformTraders;
	if(mpPlatformOrders)
		delete mpPlatformOrders;
}

//�����û���CTP����״̬
CF_ERROR CSubAccountOp::SetAccountCTPStatus(const EnumAccountStatus & nStatus)
{
	InterlockedExchange((volatile long*)(&meCTPStatus),nStatus);
	mMutexTime.lock();		
	time(&mtimeCtp);	
	mMutexTime.unlock();
	return CF_ERROR_SUCCESS;
}

//��ȡ�û���CTP����״̬
//nDiffTime ���ؽ����״̬��ʱ��
EnumAccountStatus CSubAccountOp::GetAccountCTPStatus(double & nDifftime) 
{
	EnumAccountStatus leStatus = ACCOUNT_STATUS_UnInit;
	InterlockedExchange((volatile long*)(&leStatus),meCTPStatus);
	mMutexTime.lock();
	time_t lTime;
	time(&lTime);	
	nDifftime = difftime(lTime, mtimeCtp);
	mMutexTime.unlock();
	return leStatus;
}


eCommissionMarginReady CSubAccountOp::GetCommissonMarginReady() 
{
	eCommissionMarginReady leStatus = CommissionMargin_NoReady;
	InterlockedExchange((volatile long*)(&leStatus),meReady);
	return leStatus;
}

void CSubAccountOp::SetCommissionMarginReady(const eCommissionMarginReady & nReady) 
{
	//_InterlockedOr((volatile long*)(&meReady),nReady);
	//InterlockedExchange((volatile long*)(&meUserInitStatus),USER_STATUS_InitQryRatesReady);
	SetAccountInitStatus(USER_STATUS_InitQryRatesReady);


	if(mpPlatformInstruments  && mpPlatformDataRates && mpPlatformQutos && mpPlatformOrders == NULL)
	{
		//��ʱ��Լ
		//mpPlatformQutos = new CPlatformDataMgr_Quots(*mpPlatformInstruments);

		mpPlatformOrders = new CPlatformDataMgr_Orders(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
			*mpPlatformDataRates,
			*mpPlatformQutos,3,3,true);
		mpPlatformTraders = new CPlatformDataMgr_Trades(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
			*mpPlatformDataRates);
		mpPlatformTraderTotal = new CPlatformDataMgr_TradeTotals(mbaseFTID,mbaseUpdateSeq,*mpPlatformTraders);


		mpPlatformPositionDetailCombs = new CPlatformDataMgr_PositionDetailCombs(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
			*mpPlatformQutos,			
			*mpPlatformTraders,
			*mpPlatformOrders);

		mpPlatformPositionDetail = new CPlatformDataMgr_PositionDetails(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
			*mpPlatformDataRates,
			*mpPlatformQutos,
			*mpPlatformTraders,
			*mpPlatformOrders,
			*mpPlatformPositionDetailCombs,3,0,1,0,0,1);

		mpPlatformPositions = new CPlatformDataMgr_Positions(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
			*mpPlatformDataRates,
			*mpPlatformQutos,
			*mpPlatformTraders,
			*mpPlatformOrders,
			*mpPlatformPositionDetail);

		mpPlatformPositionsCombs = new CPlatformDataMgr_PositionCombs(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
			*mpPlatformQutos,
			*mpPlatformTraders,
			*mpPlatformOrders,
			*mpPlatformPositionDetailCombs,
			*mpPlatformPositionDetail);

		mpPlatformTraderAccount = new CPlatformDataMgr_TradingAccount(*mpPlatformInstruments,
			*mpPlatformQutos,
			*mpPlatformTraders,	
			*mpPlatformOrders,	
			*mpPlatformPositionDetailCombs,
			*mpPlatformPositionDetail,
			*mpPlatformPositionsCombs,
			*mpPlatformPositions);
	}
}

///���ú�Լ��֤����
void CSubAccountOp::SetMarginRate(const std::string& InstrumentID,
						   PlatformStru_InstrumentMarginRate& outData) 
{
	CSafeLock lLock(mMutexData);
	if(mpPlatformDataRates)
		mpPlatformDataRates->SetMarginRate(InstrumentID,outData);

}

int CSubAccountOp::GetMarginRate(const std::string& InstrumentID, 
						  PlatformStru_InstrumentMarginRate& outData) 
{

	CSafeLock lLock(mMutexData);
	if(mpPlatformDataRates)
		return mpPlatformDataRates->GetMarginRate(InstrumentID,outData);
	
	return -1;
}

///���ú�Լ��������
void CSubAccountOp::SetCommissionRate(const std::string& InstrumentID, 
							   PlatformStru_InstrumentCommissionRate& outData) 
{
	CSafeLock lLock(mMutexData);
	if(mpPlatformDataRates)
		mpPlatformDataRates->SetCommissionRate(InstrumentID,outData);
}

int CSubAccountOp::GetCommissionRate(const std::string& InstrumentID, 
							  PlatformStru_InstrumentCommissionRate& outData) 
{
	CSafeLock lLock(mMutexData);
	if(mpPlatformDataRates)
		return mpPlatformDataRates->GetCommissionRate(InstrumentID,outData);

	return -1;
}

CPlatformDataMgr_Rates* CSubAccountOp::GetPlatFormRates()
{
	return mpPlatformDataRates;
}

//���鵽������ļ���
bool CSubAccountOp::CalulateByNewQuto(const PlatformStru_DepthMarketData & nQutosData,bool nbSettlement)
{
	bool lbRet = false;

	do 
	{
		eUserInitStatus leStatus = GetAccountInitStatus();
		if( mpPlatformInstruments)
		{
			PlatformStru_InstrumentInfo InstrumentInfo;		//ȡ��Լ��Ϣ
			if(!mpPlatformInstruments->GetInstrumentInfo(nQutosData.InstrumentID,InstrumentInfo))
				break;

			if(mpPlatformPositionDetail == NULL)
				break;

			if(!mpPlatformPositionDetail->HavePositionDetail(nQutosData.InstrumentID))
				break;

			if(nbSettlement == false)
			{
				if(mpPlatformPositionDetail)
					mpPlatformPositionDetail->UpdatePositionDetail_RtnDepthMarketData(nQutosData,InstrumentInfo);

				if(mpPlatformPositions)
					mpPlatformPositions->UpdatePosition_RtnQuot(nQutosData,InstrumentInfo,true);

				if(mpPlatformTraderAccount)
					mpPlatformTraderAccount->DynamicUpdateTradingAccount_RtnDepthMarketData(nQutosData.InstrumentID);
			}
			else
			{

				if(mpPlatformPositionDetail)
					mpPlatformPositionDetail->UpdatePositionDetail_OnSettlement();

				if(mpPlatformPositions)
					mpPlatformPositions->UpdatePosition_OnSettlement();

				if(mpPlatformTraderAccount)
					mpPlatformTraderAccount->DynamicUpdateTradingAccount_RtnDepthMarketData(nQutosData.InstrumentID);
			}
			lbRet = true;
		}
		break;
	} while (true);

	return lbRet;
}

//��ѯ�����ĵ���
CF_ERROR  CSubAccountOp::QryOrderReach(PlatformStru_OrderInfo& OrderInfo,
									   const int ErrID,
									   const bool bIsLast)
{
	CSafeLock ll(mMutexData);
	if(	mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	else
	{
		
		mpPlatformOrders->QryOrderReach(OrderInfo,ErrID,bIsLast);
		//if(mpPlatformPositions && mpPlatformTraderAccount)
		//{
		//	//���µ���Լ�ֲֶ����ƽ������
		//	mpPlatformPositions->UpdatePositionFreezeFields_RtnOrder(OrderInfo, OldOrderInfo);

		//	//��Ϻ�Լ���� : ������Ϻ�Լ�ֲֶ����ƽ����
		//	/*if(bComb)
		//	mpPlatformPositionsCombs->UpdatePositionCombFreezeFields_RtnOrder(nOrder, OldOrderInfo);*/

		//	//�����ʽ�����
		//	mpPlatformTraderAccount->DynamicUpdateTradingAccount_RtnOrder(OrderInfo, OldOrderInfo);

		//}

		if(bIsLast)
			SetAccountInitStatus(USER_STATUS_InitQryOrdersReady);

		if(bIsLast )
		{
			UpdateQryWaitOrderPosition();

			////��ʼ����ѯ��ɣ������ճ��ֲ���ϸ�����ڳɽ���¼�����¼���ɽ���ƽ������ƽ��ӯ����������
			//if(mpPlatformPositionDetail)			
			//	mpPlatformPositionDetail->ReCalcuTradeInfo_OnInitQryCompleted();

			if(mpPlatformTraderAccount)
				mpPlatformTraderAccount->UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData();
		}
		return CF_ERROR_SUCCESS;
	}

}
CF_ERROR CSubAccountOp::PushOrder( PlatformStru_OrderInfo & nOrder)
{
	CSafeLock ll(mMutexData);
	if(GetAccountInitStatus() != USER_STATUS_Inited ||
		mpPlatformOrders == NULL)
	{
		TRADE_LOG("PushOrder GetAccountInitStatus NotInit ��Discard this order BrokerID:%s UserID %s",
			nOrder.BrokerID,nOrder.InvestorID);
		return CF_ERROR_NOINITED;
	}



	std::string strInstrumentID(nOrder.InstrumentID);
	PlatformStru_InstrumentInfo InstrumentInfo;
	if(!mpPlatformInstruments->GetInstrumentInfo(strInstrumentID,InstrumentInfo))
		return CF_ERROR_TRADEDATA_NO_INSTRUMENTS;

	PlatformStru_OrderInfo OldOrderInfo;

	//���±�������
	mpPlatformOrders->NewOrderReady(nOrder,OldOrderInfo,InstrumentInfo);

	//����Ƿ�����Ϻ�Լ����
	//std::string Leg1InstrumentID,Leg2InstrumentID;
	//bool bComb=GlobalFunc::IsCombInstrument(strInstrumentID,Leg1InstrumentID,Leg2InstrumentID);

	////������Ҫ������ѯ���ʡ��������Ϻ�Լ�������Ȳ�ѯ
	//if(bComb)
	//{
	//	m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(Leg1InstrumentID);
	//	m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(Leg1InstrumentID);
	//	m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(Leg2InstrumentID);
	//	m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(Leg2InstrumentID);
	//}
	//else
	//{
	//	m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(strInstrumentID);
	//	m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(strInstrumentID);
	//}


	//���µ���Լ�ֲֶ����ƽ������
	mpPlatformPositions->UpdatePositionFreezeFields_RtnOrder(nOrder, OldOrderInfo,InstrumentInfo);

	//��Ϻ�Լ���� : ������Ϻ�Լ�ֲֶ����ƽ����
	/*if(bComb)
	mpPlatformPositionsCombs->UpdatePositionCombFreezeFields_RtnOrder(nOrder, OldOrderInfo);*/

	//�����ʽ�����
	mpPlatformTraderAccount->UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData( );


	return CF_ERROR_SUCCESS;
}

///��ȡָ������
CF_ERROR CSubAccountOp::GetOrderByKey(const OrderKey &nOrderKey,
							   PlatformStru_OrderInfo& outData)
{
	CSafeLock ll(mMutexData);
	if(mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	if( false != mpPlatformOrders->GetOrder(nOrderKey,outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_ORDER;

}
///��ȡȫ������
CF_ERROR  CSubAccountOp::GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData)
{

	CSafeLock ll(mMutexData);
	if( mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	if( 0 != mpPlatformOrders->GetAllOrders(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_ORDER;

}
/////��ȡȫ���Ѵ����ı���
CF_ERROR  CSubAccountOp::GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
	CSafeLock ll(mMutexData);
	if(mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	if( 0 != mpPlatformOrders->GetTriggerOrders(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_ORDER;
}

/////��ȡ�ѳɽ�����
CF_ERROR  CSubAccountOp::GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
	CSafeLock ll(mMutexData);
	if(mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	if( 0 != mpPlatformOrders->GetTradedOrders(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_ORDER;
}

/////��ȡ�ѳ����ʹ��󱨵�
CF_ERROR  CSubAccountOp::GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
	CSafeLock ll(mMutexData);
	if( mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	if( 0 != mpPlatformOrders->GetCanceledOrders(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_ORDER;
}

/////��ȡδ�ɽ�����
CF_ERROR  CSubAccountOp::GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
	CSafeLock ll(mMutexData);
	if(mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	if( 0 != mpPlatformOrders->GetWaitOrders(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_ORDER;
}

/////��ȡδ֪״̬�ı���
CF_ERROR CSubAccountOp::GetUnkownOrders(std::vector<PlatformStru_OrderInfo>& outData) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	if( 0 != mpPlatformOrders->GetUnknownStatusOrders(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_ORDER;
}



//��ѯ�ɽ��ĵ���
CF_ERROR CSubAccountOp::QryTradeReach(PlatformStru_TradeInfo& nTrader,
									  const int ErrID,
									  const bool bIsLast) 
{
	CSafeLock ll(mMutexData);
	if(	mpPlatformTraders == NULL)
		return CF_ERROR_NOINITED;	
	else
	{
		//if(bIsLast)	
		//	mbQryTradeLast = true;
		//mpPlatformTraders->QryTradeReach(nTrader,ErrID,bIsLast);

		if(bIsLast)
			SetAccountInitStatus(USER_STATUS_InitQryTradesReady);

		PlatformStru_TradeInfo lTempTrade;
		TradeKey lTradeKey(nTrader);
		if(mpPlatformTraders->GetTradeInfo(lTradeKey,lTempTrade))
		{
			TRADE_LOG("The same trade key exist ��discard this tradeinfo");
			return CF_ERROR_NOINITED;
		}

		std::string strInstrumentID(nTrader.InstrumentID);
		PlatformStru_InstrumentInfo InstrumentInfo;
		if(!mpPlatformInstruments->GetInstrumentInfo(strInstrumentID,InstrumentInfo))
			return CF_ERROR_TRADEDATA_NO_INSTRUMENTS;

		nTrader.TradeCommission = util::GetDoubleInvalidValue();
		nTrader.CloseProfitByTrade = util::GetDoubleInvalidValue();
		nTrader.CloseProfitByDate = util::GetDoubleInvalidValue();


		//�����ѯ�����������͵����ݼ���
		mpPlatformTraders->QryTradeReach(nTrader,ErrID,bIsLast);

		if(bIsLast)
		{
			//�ɽ���ȫ�󣬼���ɽ�ͳ��
			mpPlatformTraderTotal->QryTradeTotalReach(true);

			//�����ʽ�����
			mpPlatformTraderAccount->UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted();
		}

		//���±�������
		//mpPlatformTraders->NewTradeReady(nTrader,InstrumentInfo);

		//if(mpPlatformPositionDetail && 
		//	mpPlatformPositions && 
		//	mpPlatformPositionDetailCombs 
		//	&& mpPlatformPositionsCombs 
		//	&& mpPlatformTraderAccount)
		//{
		//	//ModifiedPositionDetails���汻�ɽ��޸ĵĳֲ���ϸ��¼
		//	//std::vector<PlatformStru_PositionDetail> ModifiedPositionDetails;
		//	std::vector<PlatformStru_PositionDetailComb> ModifiedPositionDetailsComb;
		//	map<PositionDetailKey,PlatformStru_PositionDetail> ModifiedPositionDetails;
		//	map<string,int> ComTradeID2CloseVolume;
		//	//���³ֲ���ϸ
		//	mpPlatformPositionDetail->UpdatePositionDetail_RtnTrade(nTrader,InstrumentInfo,ModifiedPositionDetails,ComTradeID2CloseVolume);

  //          //���³ɽ���¼�е�ƽ������ƽ��ӯ�������������ѡ�
  //          mpPlatformTraders->UpdateTradeCloseTodayVolume_CloseProfit_RecalcuCommission(
		//		nTrader.FTID,
  //              InstrumentInfo,
  //              nTrader.CloseTodayVolume,
  //              nTrader.CloseProfitByDate,
  //              nTrader.CloseProfitByTrade,
		//		nTrader.TradeCommission);

		//	//���³ֲ�
		//	mpPlatformPositions->UpdatePosition_RtnTrade(nTrader,ModifiedPositionDetails,InstrumentInfo);

		//	//const map<PositionDetailKey,int>& mapCloseDeltaVolume,
		//	//	const map<string,int>& ComTradeID2CloseVolume,
		//	//	map<PositionDetailCombKey,PlatformStru_PositionDetailComb>&ModifiedPositionDetailCombs
		//	////������ϳֲ���ϸ
		//	//mpPlatformPositionDetailCombs->Update_RtnTrade(nTrader,ModifiedPositionDetails,ModifiedPositionDetailsComb);

		//	////������ϳֲ�
		//	//mpPlatformPositionsCombs->Update_RtnTrade(nTrader,ModifiedPositionDetailsComb);


		//	//�����ʽ�
		//	mpPlatformTraderAccount->UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted();
		//}

		return CF_ERROR_SUCCESS;
	}

}
CF_ERROR CSubAccountOp::PushTrader( PlatformStru_TradeInfo & nTrader)
{
	CSafeLock ll(mMutexData);
	if(GetAccountInitStatus() != USER_STATUS_Inited 
		|| mpPlatformTraders == NULL)
	{
		TRADE_LOG("GetAccountInitStatus NotInit ��Discard this Trade BrokerID:%s UserID %s",nTrader.BrokerID,nTrader.InvestorID);
		return CF_ERROR_NOINITED;
	}
	
	PlatformStru_TradeInfo lTempTrade;
	TradeKey lTradeKey(nTrader);
	if(mpPlatformTraders->GetTradeInfo(lTradeKey,lTempTrade))
	{
		TRADE_LOG("The same trade key exist ��discard this tradeinfo");
		return CF_ERROR_NOINITED;
	}

	std::string strInstrumentID(nTrader.InstrumentID);
	PlatformStru_InstrumentInfo InstrumentInfo;
	if(!mpPlatformInstruments->GetInstrumentInfo(strInstrumentID,InstrumentInfo))
		return CF_ERROR_NOINITED;


	////����Ǵӱ�Ŀͻ��˲����ĳɽ�
	//if(false == CheckTradeOrderSysIDIsOwn(nTrader.OrderSysID))
	//	return CF_ERROR_TRADEDATA_TRADE_FROM_OTHER;

	//���±�������
	mpPlatformTraders->NewTradeReady(nTrader,InstrumentInfo);

	if(mpPlatformPositionDetail && 
		mpPlatformPositions && 
		mpPlatformPositionDetailCombs 
		&& mpPlatformPositionsCombs 
		&& mpPlatformTraderAccount)
	{
		//ModifiedPositionDetails���汻�ɽ��޸ĵĳֲ���ϸ��¼
		
		std::vector<PlatformStru_PositionDetailComb> ModifiedPositionDetailsComb;

		map<PositionDetailKey,PlatformStru_PositionDetail> ModifiedPositionDetails;
		map<string,int> ComTradeID2CloseVolume;
		//���³ֲ���ϸ
		mpPlatformPositionDetail->UpdatePositionDetail_RtnTrade(nTrader,InstrumentInfo,ModifiedPositionDetails,ComTradeID2CloseVolume);

        //���³ɽ���¼�е�ƽ������ƽ��ӯ�������������ѡ�
        mpPlatformTraders->UpdateTradeCloseTodayVolume_CloseProfit_RecalcuCommission(
			nTrader.FTID,
            InstrumentInfo,
            nTrader.CloseTodayVolume,
            nTrader.CloseProfitByDate,
            nTrader.CloseProfitByTrade,
			nTrader.TradeCommission);


		//���³ֲ�
		mpPlatformPositions->UpdatePosition_RtnTrade(nTrader,ModifiedPositionDetails,InstrumentInfo);

		////������ϳֲ���ϸ
		//mpPlatformPositionDetailCombs->Update_RtnTrade(nTrader,ModifiedPositionDetails,ModifiedPositionDetailsComb);

		////������ϳֲ�
		//mpPlatformPositionsCombs->Update_RtnTrade(nTrader,ModifiedPositionDetailsComb);


		//�����ʽ�
		mpPlatformTraderAccount->UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted();
	}


	return CF_ERROR_SUCCESS;
}

//��ȡȫ���ɽ��������������������
CF_ERROR CSubAccountOp::GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformTraders == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ��Discard this call");
		return CF_ERROR_NOINITED;
	}

	if( mpPlatformTraders->GetAllTrades(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_TRADE;
}



//���Գɽ��Ƿ��Ѵ���
bool CSubAccountOp::TestTradeInfoExist(const TradeKey &key)
{
	CSafeLock ll(mMutexData);
	if(mpPlatformTraders == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ��Discard this call");
		return false;
	}

	PlatformStru_TradeInfo lTemp;
	return  mpPlatformTraders->GetTradeInfo(key,lTemp);

}
//�ճ���ѯ�ֲֵĵ���
CF_ERROR  CSubAccountOp::QryPositionReach( 
	PlatformStru_Position& PositionInfo,
	int ErrID,
	bool bIsLast) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformPositions == NULL)
		return CF_ERROR_NOINITED;	
	else
	{	
		mpPlatformPositions->QryPositionReach(PositionInfo,ErrID,bIsLast);
	
		if(bIsLast)
		{
			mpPlatformPositions->QryPositionReach(PositionInfo,-1,bIsLast);
			if(mpPlatformTraderAccount)
				mpPlatformTraderAccount->UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted();
			
			TRADE_LOG("SetAccountInitStatus QryPosition Ready ");
			SetAccountInitStatus(USER_STATUS_InitQryPositionReady);
		}
		return CF_ERROR_SUCCESS;	
	}
}
//��ȡ�ֲֻ��ܣ����������������
CF_ERROR  CSubAccountOp::GetPositions(const std::string & nInstrumentID,
									  std::vector<PlatformStru_Position>& outData,
									  bool bNeedZeroVolumePosition ) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformPositions == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ��Discard thiscall");
		return CF_ERROR_NOINITED;
	}

	int lnRet = -1;
	if(nInstrumentID.empty())
	{
		if(bNeedZeroVolumePosition)
			lnRet = mpPlatformPositions->GetPositions4(outData);
		else
			lnRet = mpPlatformPositions->GetPositions(outData);
		if(lnRet != 0)
			return CF_ERROR_SUCCESS;
		else
			return CF_ERROR_TRADEDATA_NO_POSITION;	
	}
	else
	{
		long lSeq = -1;
		lnRet = mpPlatformPositions->GetPositions3(nInstrumentID,outData,lSeq);
		if(lnRet == 0)
			return CF_ERROR_SUCCESS;
		else
			return CF_ERROR_TRADEDATA_NO_POSITION;	
	}

}

//�ж��Ƿ��гֲ�
bool CSubAccountOp::HasPositions() 
{
	bool lbRet = false;
	if(mpPlatformPositions)
	{
		//lbRet = !mpPlatformPositions->empty();
		std::vector<PlatformStru_Position> lVecPos;
		mpPlatformPositions->GetPositions(lVecPos);

		if(lVecPos.size() == 0)
			lbRet = false;
		else
		{
			//���˳ֲ���Ϊ0�ĳֲ�
			std::vector<PlatformStru_Position>::iterator lItr = lVecPos.begin();
			for(;lItr != lVecPos.end();lItr++)
			{
				if(lItr->Position != 0)
				{
				   lbRet = true;
				   break;
				}
			}
		}
	}
	return lbRet;
	
}
//�ճ���ѯ�ֲ���ϸ�ĵ���
CF_ERROR  CSubAccountOp::QryPositionDetailReach( 
	PlatformStru_PositionDetail& PositionDetailInfo,
	int ErrID,
	bool bIsLast) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformPositionDetail == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ��Discard this call");
		return CF_ERROR_NOINITED;	
	}
	else
	{	
		mpPlatformPositionDetail->QryPositionDetailReach(PositionDetailInfo,ErrID,bIsLast);
		if(bIsLast)
		{
			TRADE_LOG("SetAccountInitStatus QryPositionDetail Ready ");
			SetAccountInitStatus(USER_STATUS_InitQryPositionDetailReady);
		}
		return CF_ERROR_SUCCESS;	
	}
}
//��ȡ�ֲ���ϸ�����������������
CF_ERROR  CSubAccountOp::GetPositionDetail(const std::string & nInstrumentID,
										   std::vector<PlatformStru_PositionDetail>& outData) 
{
	CSafeLock ll(mMutexData);
	if( mpPlatformPositionDetail == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ��Discard this call");
		return CF_ERROR_NOINITED;
	}
	int lnRet = -1;
	long lSeq = -1;
	if(nInstrumentID.empty())
	{
		lnRet = mpPlatformPositionDetail->GetPositionDetails( outData,lSeq);
		if(lnRet != 0)
			return CF_ERROR_SUCCESS;
		else
			return CF_ERROR_TRADEDATA_NO_POSDETAIL;	
	}
	else
	{	
		std::set<long> setFTID;
		lnRet = mpPlatformPositionDetail->GetPositionDetails3(nInstrumentID,setFTID,outData,lSeq);
		if(lnRet == 0)
			return CF_ERROR_SUCCESS;
		else
			return CF_ERROR_TRADEDATA_NO_POSDETAIL;	
	}
}
//��ȡ�ֲ���ϸ�����������������
CF_ERROR   CSubAccountOp::GetPositionDetail(const PositionDetailKey & nKey,
									PlatformStru_PositionDetail& outData) 
{
	CSafeLock ll(mMutexData);
	if( mpPlatformPositionDetail == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ��Discard this call");
		return CF_ERROR_NOINITED;
	}	

	if( mpPlatformPositionDetail->GetPositionDetail(nKey,outData) != false)
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_POSDETAIL;	

}
//��ȡĳ��ʱ��εĳֲ���ϸ
CF_ERROR  CSubAccountOp::GetPositionDetail(int nLowtime,int nUpperTime,
									std::vector<PlatformStru_PositionDetail>& outData) 
{

	CSafeLock ll(mMutexData);
	if( mpPlatformPositionDetail == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ��Discard this call");
		return CF_ERROR_NOINITED;
	}	

	mpPlatformPositionDetail->GetPositionDetailsInOpenTimeScope(nLowtime,nUpperTime,outData);
	return CF_ERROR_SUCCESS;
}



//�����ճ����ʽ�
CF_ERROR CSubAccountOp::SetQryFundInfo(const PlatformStru_TradingAccountInfo& inData) 
{
	CSafeLock ll(mMutexData);
	if(	mpPlatformTraderAccount == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ��Discard this call");
		return CF_ERROR_NOINITED;	
	}
	else
	{	
		TRADE_LOG("SetAccountInitStatus QryFundInfo Ready ");

		//DataRspQryTradingAccount lRspAccount;
		//memset(&lRspAccount,0,sizeof(lRspAccount));
		//lRspAccount.Head.BID = BID_RspQryTradingAccount;
		//lRspAccount.Head.Size = sizeof(inData);
		//memcpy(&lRspAccount.TradingAccountField,&inData,sizeof(inData));
		//lRspAccount.bIsLast = true;
		mpPlatformTraderAccount->SetTradingAccount(inData);

		SetAccountInitStatus(USER_STATUS_InitQryFundInfoReady);
		return CF_ERROR_SUCCESS;	
	}

}
//��ȡ���ʺ��ʽ���Ϣ
CF_ERROR  CSubAccountOp::GetFundInfo(PlatformStru_TradingAccountInfo& outData) 
{
	CSafeLock ll(mMutexData);

	if(mpPlatformTraderAccount == NULL)
	{
	//	TRADE_LOG("GetAccountInitStatus NotInit ��Discard this call");
		return CF_ERROR_NOINITED;
	}
	if(0 == mpPlatformTraderAccount->GetTradingAccount(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_FUND;
}


//��ȡ���еĳֲ���ϸ
CF_ERROR  CSubAccountOp::GetAllPositionDetail(
	std::vector<PlatformStru_PositionDetail> & outData) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformPositionDetail == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ��Discard this call");
		return CF_ERROR_NOINITED;
	}


	long lastSq = 0;
	mpPlatformPositionDetail->GetPositionDetails(outData,lastSq);


	return CF_ERROR_SUCCESS;
}

//��ȡ���еĳֲ�
CF_ERROR  CSubAccountOp::GetAllPosition(
										std::vector<PlatformStru_Position>& outData) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformPositions == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ��Discard this call");
		return CF_ERROR_NOINITED;
	}


	mpPlatformPositions->GetPositions(outData);
	return CF_ERROR_SUCCESS;
}


//����ֲ֣��ֲ���ϸ���ʽ�ͳɽ��Ľ�������
void CSubAccountOp::ClearTraderAndPosition()
{
	CSafeLock ll(mMutexData);
	TRADE_LOG("Clear all subaccount TraderAndPosition");

	
	
	if(mpPlatformTraderAccount)
		mpPlatformTraderAccount->clear();
	if(mpPlatformPositionsCombs)
		mpPlatformPositionsCombs->clear();
	if(mpPlatformPositions)
		mpPlatformPositions->clear();
	if(mpPlatformPositionDetail)
		mpPlatformPositionDetail->clear();
	if(mpPlatformPositionDetailCombs)
		mpPlatformPositionDetailCombs->clear();
	if(mpPlatformTraderTotal)
		mpPlatformTraderTotal->clear();
	if(mpPlatformTraders)
		mpPlatformTraders->clear();

}

void  CSubAccountOp::ClearAll()
{
	CSafeLock ll(mMutexData);
	TRADE_LOG("Clear all subaccount info");
	InterlockedExchange((volatile long*)(&meReady),CommissionMargin_NoReady);	
	SetAccountCTPStatus(ACCOUNT_STATUS_UnInit);
	SetEqualAccountInitStatus(USER_STATUS_UnInit);


	mnLastSaveOrderUpdateSeq = 1;
	mnLastSaveTraderUpdateSeq= 1;

	mbaseFTID = mbaseUpdateSeq = 1;
	mbSynFinished = false;
	if(mpPlatformTraderAccount)
		mpPlatformTraderAccount->clear();
	if(mpPlatformPositionsCombs)
		mpPlatformPositionsCombs->clear();
	if(mpPlatformPositions)
		mpPlatformPositions->clear();
	if(mpPlatformPositionDetail)
		mpPlatformPositionDetail->clear();
	if(mpPlatformPositionDetailCombs)
		mpPlatformPositionDetailCombs->clear();
	if(mpPlatformTraderTotal)
		mpPlatformTraderTotal->clear();
	if(mpPlatformTraders)
		mpPlatformTraders->clear();
	if(mpPlatformOrders)
		mpPlatformOrders->clear();


}


//����ʱ�����е�δ�ɽ������ֳɽ�������˵ı���ȫ����Ϊ�ѳ���
void CSubAccountOp::CancelAllUnkonwnOrders()
{
	std::vector<PlatformStru_OrderInfo> lvecOrder;
	GetAllOrders(lvecOrder);
	for(unsigned int i = 0; i < lvecOrder.size(); i++)
	{
		if(lvecOrder[i].OrderStatus == THOST_FTDC_OST_PartTradedQueueing ||
			lvecOrder[i].OrderStatus == THOST_FTDC_OST_NoTradeQueueing||
			lvecOrder[i].OrderStatus == THOST_FTDC_OST_PartTradedNotQueueing||
			lvecOrder[i].OrderStatus == THOST_FTDC_OST_NoTradeNotQueueing ||
			lvecOrder[i].OrderStatus == THOST_FTDC_OST_Unknown )
		{
			lvecOrder[i].OrderStatus = THOST_FTDC_OST_Canceled;
			strcpy(lvecOrder[i].StatusMsg,"����������");
			PushOrder(lvecOrder[i]);
		}
	}
	return;
}



//��ѯ��ɺ󣬸������йҵ���Ӧ�ĳֲֶ�����
void CSubAccountOp::UpdateQryWaitOrderPosition()
{
	std::vector<PlatformStru_OrderInfo> lvecOrder;
	GetWaitOrders(lvecOrder);
	for(unsigned int i = 0; i < lvecOrder.size(); i++)
	{
		if(mpPlatformPositions)
		{
			PlatformStru_OrderInfo lOrder(true); //����һ����Ч��Order
			std::string strInstrumentID(lvecOrder[i].InstrumentID);
			PlatformStru_InstrumentInfo InstrumentInfo;
			if(mpPlatformInstruments->GetInstrumentInfo(strInstrumentID,InstrumentInfo))
			{
				mpPlatformPositions->UpdatePositionFreezeFields_RtnOrder(lvecOrder[i],lOrder,InstrumentInfo);
			}
		}
	}
}

void CSubAccountOp::GetOrderAndTradeGeSeq( int nOrderSeq,
								   int nTraderSeq,
								   std::vector<PlatformStru_OrderInfo> & nvecOrder,
								   std::vector<PlatformStru_TradeInfo> & nvecTrader) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformTraders && mpPlatformOrders)
	{
		mpPlatformTraders->GetAll_GE_UpdateSeq(nTraderSeq,nvecTrader);
		mpPlatformOrders->GetAll_GE_UpdateSeq(nOrderSeq,nvecOrder);
	}
	return;
}