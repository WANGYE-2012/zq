
#include "stdafx.h"
#include "CmdDealFun.h"
#include "KernelStruct.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "Real_SvrTradeData.h"
// #include "..\SvrTcp\Interface_SvrTcp.h"
// #include "..\SvrUserOrg\Interface_SvrUserOrg.h"
// #include "..\SvrBrokerInfo\Interface_SvrBrokerInfo.h"
// #include "..\SvrNotifyAndAsk\Interface_SvrNotifyAndAsk.h"

// #include "..\SvrLogin\Interface_SvrLogin.h"

#include "tools_win32.h"
//class COutput
//{
//public:
//	COutput(std::string nsTag,int  nCount=10)
//	{
//		mCount = nCount;
//		msTag = nsTag;
//		m1 =  CTools_Win32::MyGetCpuTickCounter();;
//		
//	}
//	~COutput()
//	{
//		m2 = CTools_Win32::MyGetCpuTickCounter();;
//		LONGLONG lFfreq = CTools_Win32::MyGetCpuTickFreq();
//		if(mCount == 10)
//		{
//			std::string lsFormat = msTag + "%d %.4f";
//			OUTPUT_LOG(lsFormat.c_str() ,mCount,(m2-m1)*1000.0/lFfreq);
//		}
//	}
//private:
//	int mCount;
//	LONGLONG m1;
//	LONGLONG m2;
//	std::string msTag;
//};
bool QryInstrumentDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
//	COutput lOut("QryInstrumentDeal");
	if(PkgHead.len != sizeof(PlatformStru_QryInstrument))
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryInstrument_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}
	//�ж�Ȩ��

	if(DataReday_NoReady == CReal_SvrTradeData::getObj().GetInstrumentDataReady())
	{
		//��������û��׼���ã����Ժ��ٴβ�ѯ
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryInstrument_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_TRADEDATA_NO_INSTRUMENTS,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_TRADEDATA_NO_INSTRUMENTS,0);
		return false;
	}
	//
	std::vector<PlatformStru_InstrumentInfo> lVecInstruments;
	std::set<std::string> lInstrumentStringSet;
	CReal_SvrTradeData::getObj().GetInstrumentList(lInstrumentStringSet);
	std::set<std::string>::iterator lIter = lInstrumentStringSet.begin();
	while(lIter != lInstrumentStringSet.end())
	{
		PlatformStru_InstrumentInfo lTempInfo;
		CReal_SvrTradeData::getObj().GetInstrumentInfo(*lIter,lTempInfo);
		lVecInstruments.push_back(lTempInfo);
		lIter++;
	}

	if(lVecInstruments.size() == 0)
	{
		//��������û��׼���ã����Ժ��ٴβ�ѯ
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryInstrument_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_TRADEDATA_NO_INSTRUMENTS,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_TRADEDATA_NO_INSTRUMENTS,0);
		return false;;
	}
	else
	{
// 		CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_TradeDataManager_QryInstrument_Rsp,
// 			lVecInstruments.size() == 0 ? NULL : &lVecInstruments[0],
// 			sizeof(PlatformStru_InstrumentInfo)*lVecInstruments.size(),
// 			PkgHead.seq,
// 			0,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_SUCCESS,0);
	}

	return true;
}
bool QryCommissionRateDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
//	COutput lOut("QryCommissionRateDeal");
	if(PkgHead.len != sizeof(CThostFtdcQryInstrumentCommissionRateField))
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryCommissionRat_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}


	CThostFtdcQryInstrumentCommissionRateField * lp = (CThostFtdcQryInstrumentCommissionRateField *)pPkgData;
	PlatformStru_InstrumentCommissionRate lCommissionRate;
	memset(&lCommissionRate,0,sizeof(PlatformStru_InstrumentCommissionRate));
	if(0 == CReal_SvrTradeData::getObj().GetCommissionRate(	lp->InvestorID,
		lp->InstrumentID,
		lCommissionRate))
	{
// 		CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_TradeDataManager_QryCommissionRat_Rsp,
// 			&lCommissionRate,
// 			sizeof(PlatformStru_InstrumentCommissionRate),
// 			PkgHead.seq,
// 			0,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_SUCCESS,0);
		return true;

	}
	else
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryCommissionRat_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_TRADEDATA_NO_COMMISSION,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_TRADEDATA_NO_COMMISSION,0);
		return false;

	}


}
bool QryMarginRateDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
//	COutput lOut("QryMarginRateDeal");
	if(PkgHead.len != sizeof(CThostFtdcQryInstrumentMarginRateField))
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryMarginRate_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}


	CThostFtdcQryInstrumentMarginRateField * lp = (CThostFtdcQryInstrumentMarginRateField *)pPkgData;
	PlatformStru_InstrumentMarginRate lMarginRate;
	memset(&lMarginRate,0,sizeof(PlatformStru_InstrumentMarginRate));
	if(0 == CReal_SvrTradeData::getObj().GetMarginRate(lp->InvestorID,
		lp->InstrumentID,
		lMarginRate))
	{
// 		CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_TradeDataManager_QryMarginRate_Rsp,
// 			&lMarginRate,
// 			sizeof(PlatformStru_InstrumentMarginRate),
// 			PkgHead.seq,
// 			0,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_SUCCESS,0);
		return true;

	}
	else
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryMarginRate_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_TRADEDATA_NO_MARGIN,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_TRADEDATA_NO_MARGIN,0);
		return false;

	}
}


bool QryUserAccountInfoDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
//	COutput lOut("QryUserAccountInfoDeal");
	if(PkgHead.len != sizeof(PlatformStru_QryTradingAccount))
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryTraderAccount_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}
	PlatformStru_QryTradingAccount * lp = (PlatformStru_QryTradingAccount *)pPkgData;
	PlatformStru_TradingAccountInfo lUserFund;
	memset(&lUserFund,0,sizeof(PlatformStru_TradingAccountInfo));
	//if(CF_ERROR_SUCCESS == CReal_SvrTradeData::getObj().GetUserFundInfo(lp->InvestorID,lUserFund))
	CReal_SvrTradeData::getObj().GetUserFundInfo(lp->InvestorID,lUserFund);
	{
// 		CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 			Cmd_TradeDataManager_QryTraderAccount_Rsp,
// 			&lUserFund,
// 			sizeof(PlatformStru_TradingAccountInfo),
// 			PkgHead.seq,
// 			0,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_SUCCESS,0);
		return true;

	}
	/*else
	{
		SendRspError(hSocket,Cmd_TradeDataManager_QryTraderAccount_Rsp,
			PkgHead.seq,
			CF_ERROR_TRADEDATA_NO_FUND,
			PkgHead.userdata1,
			PkgHead.userdata2,
			CF_ERROR_TRADEDATA_NO_FUND,0);
		return false;

	}*/
}



//��ѯ�ֲ�
bool QryUserPositionDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
//	COutput lOut("QryUserPositionDeal");
	if(PkgHead.len != sizeof(PlatformStru_QryInvestorPosition))
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryPosition_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}

	vector<PlatformStru_Position> lVecPos;
	PlatformStru_QryInvestorPosition * lp = (PlatformStru_QryInvestorPosition *)pPkgData;
	//if(CF_ERROR_SUCCESS == CReal_SvrTradeData::getObj().GetUserPositions(lp->InvestorID,lp->InstrumentID,lVecPos))
	CReal_SvrTradeData::getObj().GetUserPositions(lp->InvestorID,lp->InstrumentID,lVecPos,true);
	{
// 		CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 			Cmd_TradeDataManager_QryPosition_Rsp,
// 			lVecPos.size() == 0 ? NULL :&lVecPos[0],
// 			sizeof(PlatformStru_Position)*lVecPos.size(),
// 			PkgHead.seq,
// 			0,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_SUCCESS,0);
		return true;
	}
	//else
	//{
	//	SendRspError(hSocket,Cmd_TradeDataManager_QryPosition_Rsp,
	//		PkgHead.seq,
	//		CF_ERROR_TRADEDATA_NO_POSITION,
	//		PkgHead.userdata1,
	//		PkgHead.userdata2,
	//		CF_ERROR_TRADEDATA_NO_POSITION,0);
	//	return false;
	//}

}



//��ѯ�ֲ���ϸ
bool QryUserPositionDetailDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
//	COutput lOut("QryUserPositionDetailDeal");
	if(PkgHead.len != sizeof(PlatformStru_QryInvestorPositionDetail))
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryPositionDetail_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}
	vector<PlatformStru_PositionDetail> lVecPosDetail;
	PlatformStru_QryInvestorPositionDetail * lp = (PlatformStru_QryInvestorPositionDetail *)pPkgData;
//	if(CF_ERROR_SUCCESS == CReal_SvrTradeData::getObj().GetUserPositionDetail(lp->InvestorID,lp->InstrumentID,lVecPosDetail))
	CReal_SvrTradeData::getObj().GetUserPositionDetail(lp->InvestorID,lp->InstrumentID,lVecPosDetail);
	{
// 		CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 			Cmd_TradeDataManager_QryPositionDetail_Rsp,
// 			lVecPosDetail.size() == 0 ? NULL :&lVecPosDetail[0],
// 			sizeof(PlatformStru_PositionDetail)*lVecPosDetail.size(),
// 			PkgHead.seq,
// 			0,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_SUCCESS,0);
		return true;
	}
	//else
	//{
	//	SendRspError(hSocket,Cmd_TradeDataManager_QryPositionDetail_Rsp,
	//		PkgHead.seq,
	//		CF_ERROR_TRADEDATA_NO_POSDETAIL,
	//		PkgHead.userdata1,
	//		PkgHead.userdata2,
	//		CF_ERROR_TRADEDATA_NO_POSDETAIL,0);
	//	return false;
	//}
}




//��ѯ��ϳֲ���ϸ
bool QryUserComPositionDetailDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
//		COutput lOut("QryUserComPositionDetailDeal");
	if(PkgHead.len != sizeof(PlatformStru_QryInvestorPositionCombineDetail))
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryComPositionDetail_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}
	vector<PlatformStru_PositionDetailComb> lVecPosDetailComb;
	PlatformStru_QryInvestorPositionCombineDetail * lp = (PlatformStru_QryInvestorPositionCombineDetail *)pPkgData;
	//if(CF_ERROR_SUCCESS == CReal_SvrTradeData::getObj().GetUserPositionCombDetail(lp->InvestorID,lp->CombInstrumentID,lVecPosDetailComb))
	CReal_SvrTradeData::getObj().GetUserPositionCombDetail(lp->InvestorID,lp->CombInstrumentID,lVecPosDetailComb);
	{
// 		CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 			Cmd_TradeDataManager_QryComPositionDetail_Rsp,
// 			lVecPosDetailComb.size() == 0 ? NULL :&lVecPosDetailComb[0],
// 			sizeof(PlatformStru_PositionDetailComb)*lVecPosDetailComb.size(),
// 			PkgHead.seq,
// 			0,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_SUCCESS,0);
		return true;
	}
	//else
	//{
	//	SendRspError(hSocket,Cmd_TradeDataManager_QryComPositionDetail_Rsp,
	//		PkgHead.seq,
	//		CF_ERROR_TRADEDATA_NO_COMPOSDETAIL,
	//		PkgHead.userdata1,
	//		PkgHead.userdata2,
	//		CF_ERROR_TRADEDATA_NO_COMPOSDETAIL,0);
	//	return false;
	//}
	return true;
}


//��ѯ����
bool QryUserOrderDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
//	COutput lOut("QryUserOrderDeal");
	if(PkgHead.len != sizeof(PlatformStru_QryOrder))
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryOrder_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}


	PlatformStru_QryOrder * lp = (PlatformStru_QryOrder *)pPkgData;
	vector<PlatformStru_OrderInfo> lvecOrder;
	/*if(CF_ERROR_SUCCESS == CReal_SvrTradeData::getObj().GetUserOrders(lp->Thost.InvestorID,lvecOrder))*/
	CReal_SvrTradeData::getObj().GetUserOrders(lp->Thost.InvestorID,lvecOrder);

// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 		Cmd_TradeDataManager_QryOrder_Rsp,
// 		lvecOrder.size() == 0 ? NULL :&lvecOrder[0],
// 		sizeof(PlatformStru_OrderInfo)*lvecOrder.size(),
// 		PkgHead.seq,
// 		0,
// 		PkgHead.userdata1,
// 		PkgHead.userdata2,
// 		CF_ERROR_SUCCESS,0);
	return true;
	/*}
	else
	{
		SendRspError(hSocket,Cmd_TradeDataManager_QryOrder_Rsp,
			PkgHead.seq,
			CF_ERROR_TRADEDATA_NO_ORDER,
			PkgHead.userdata1,
			PkgHead.userdata2,
			CF_ERROR_TRADEDATA_NO_ORDER,0);
		return false;
	}*/
}


//��ѯ�ɽ�
bool QryUserTraderDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
//	COutput lOut("QryUserTraderDeal");
	if(PkgHead.len != sizeof(PlatformStru_QryTrade))
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryTrade_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}

	
	PlatformStru_QryOrder * lp = (PlatformStru_QryOrder *)pPkgData;
	vector<PlatformStru_TradeInfo> lvecTrade;
	//if(CF_ERROR_SUCCESS == CReal_SvrTradeData::getObj().GetUserTradeInfo(lp->Thost.InvestorID,lvecTrade))
	//{
	CReal_SvrTradeData::getObj().GetUserTradeInfo(lp->Thost.InvestorID,lvecTrade);
// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 		Cmd_TradeDataManager_QryTrade_Rsp,
// 		lvecTrade.size() == 0 ? NULL :&lvecTrade[0],
// 		sizeof(PlatformStru_TradeInfo)*lvecTrade.size(),
// 		PkgHead.seq,
// 		0,
// 		PkgHead.userdata1,
// 		PkgHead.userdata2,
// 		CF_ERROR_SUCCESS,0);
	return true;
	//}
	//else
	//{
	//	SendRspError(hSocket,Cmd_TradeDataManager_QryTrade_Rsp,
	//		PkgHead.seq,
	//		CF_ERROR_TRADEDATA_NO_TRADE,
	//		PkgHead.userdata1,
	//		PkgHead.userdata2,
	//		CF_ERROR_TRADEDATA_NO_TRADE,0);
	//	return false;
	//}
}

//���������
bool ReqFundInOut(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	if(PkgHead.len != sizeof(sFundInOut))
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_FundInOut_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}


	sFundInOut * lp = (sFundInOut *)pPkgData;
// 	if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,
// 		(lp->meInOut == FundOut) ? FUND_OUT : FUND_IN))
// 	{		
// 		SendRspError(hSocket,Cmd_TradeDataManager_FundInOut_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_NO_PRIVILEGE,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_NO_PRIVILEGE,0);
// 		return false;
// 	}


	CF_ERROR leRet = CReal_SvrTradeData::getObj().FundInOut(*lp);
	if(CF_ERROR_SUCCESS == leRet)
	{
// 		CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 			Cmd_TradeDataManager_FundInOut_Rsp,
// 			NULL, 
// 			0,
// 			PkgHead.seq,
// 			0,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_SUCCESS,0);
// 
// 		//��������ͬ��
// 		if(!CInterface_SvrLogin::getObj().IsSlaver())
// 		{
// 			//��ӷ�����������
// 			vector<SOCKET> lVecSocket;
// 			CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 			for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 			{
// 				CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 					Cmd_TradeDataManager_FundInOut_Req,
// 					(void*)pPkgData,
// 					sizeof(sFundInOut),
// 					PkgHead.seq,
// 					0,
// 					PkgHead.userdata1,
// 					PkgHead.userdata2,
// 					CF_ERROR_SUCCESS,0);
// 			}
// 		}
	}
	else
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_FundInOut_Rsp,
// 			PkgHead.seq,
// 			leRet,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			leRet,0);
	}
	return true;
}

//��ѯ��ʷ���������
bool ReqQryFundInOut(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	if(PkgHead.len != sizeof(sQryFundInOut))
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryFundInOut_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}

	sQryFundInOut * lp = (sQryFundInOut *)pPkgData;
	vector<sFundInOut> lVecFundInOut;
	CReal_SvrTradeData::getObj().QryHistroyFundInOut(*lp,lVecFundInOut);
// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 		Cmd_TradeDataManager_QryFundInOut_Rsp,
// 		lVecFundInOut.size() == 0 ? NULL :&lVecFundInOut[0],
// 		sizeof(sFundInOut)*lVecFundInOut.size(),
// 		PkgHead.seq,
// 		0,
// 		PkgHead.userdata1,
// 		PkgHead.userdata2,
// 		CF_ERROR_SUCCESS,0);
	
	return true;
}

//��ѯ����ȷ������
bool ReqQrySettlementConfirm(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
//	COutput lOut("ReqQrySettlementConfirm");

	if(PkgHead.len != sizeof(PlatformStru_QrySettlementInfoConfirm))
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QrySettlementConfirm_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}
	PlatformStru_QrySettlementInfoConfirm * lp = (PlatformStru_QrySettlementInfoConfirm *)pPkgData;
	CThostFtdcSettlementInfoConfirmField lTemp;
	memset(&lTemp,0,sizeof(lTemp));
	memcpy(lTemp.BrokerID,lp->Thost.BrokerID,sizeof(TThostFtdcBrokerIDType));
	memcpy(lTemp.InvestorID,lp->Thost.InvestorID,sizeof(TThostFtdcInvestorIDType));
	std::string lsConfirmData,lsConfirmTime;
	//if(1/*CReal_SvrTradeData::getObj().IsUserSettlementConfirmed(lTemp.InvestorID,lsConfirmData,lsConfirmTime)*/)
	//{
	//	time_t lTime;
	//	struct tm * lTimeinfo; 
	//	time(&lTime);
	//	lTimeinfo = localtime(&lTime);
	//	sprintf_s(lTemp.ConfirmDate,sizeof(lTemp.ConfirmDate),"%04d%02d%02d",1900+lTimeinfo->tm_year,1+lTimeinfo->tm_mon,lTimeinfo->tm_mday);
	//	sprintf_s(lTemp.ConfirmTime,sizeof(lTemp.ConfirmTime),"%02d:%02d:%02d",lTimeinfo->tm_hour,lTimeinfo->tm_min,lTimeinfo->tm_sec);
	//	/*strcpy(lTemp.ConfirmDate,lsConfirmData.c_str());
	//	strcpy(lTemp.ConfirmTime,lsConfirmTime.c_str());*/

	//}
	if(CReal_SvrTradeData::getObj().IsUserSettlementConfirmed(lTemp.InvestorID,lsConfirmData,lsConfirmTime))
	{
		strcpy(lTemp.ConfirmDate,lsConfirmData.c_str());
		strcpy(lTemp.ConfirmTime,lsConfirmTime.c_str());
	}
// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 		Cmd_TradeDataManager_QrySettlementConfirm_Rsp,
// 		&lTemp, 
// 		sizeof(CThostFtdcSettlementInfoConfirmField),
// 		PkgHead.seq,
// 		0,
// 		PkgHead.userdata1,
// 		PkgHead.userdata2,
// 		CF_ERROR_SUCCESS,0);
	return true;


}
//����ȷ������
bool ReqSettlementConfirm(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
//	COutput lOut("ReqSettlementConfirm");
	if(PkgHead.len != sizeof(CThostFtdcSettlementInfoConfirmField))
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_SettlementConfirm_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}
	CThostFtdcSettlementInfoConfirmField * lp = (CThostFtdcSettlementInfoConfirmField *)pPkgData;
	CThostFtdcSettlementInfoConfirmField lTemp;
	memset(&lTemp,0,sizeof(lTemp));
	memcpy(lTemp.BrokerID,lp->BrokerID,sizeof(TThostFtdcBrokerIDType));
	memcpy(lTemp.InvestorID,lp->InvestorID,sizeof(TThostFtdcInvestorIDType));
	time_t lTime;
	struct tm * lTimeinfo; 
	time(&lTime);
	lTimeinfo = localtime(&lTime);
	sprintf_s(lTemp.ConfirmDate,sizeof(lTemp.ConfirmDate),"%04d%02d%02d",1900+lTimeinfo->tm_year,1+lTimeinfo->tm_mon,lTimeinfo->tm_mday);
	sprintf_s(lTemp.ConfirmTime,sizeof(lTemp.ConfirmTime),"%02d:%02d:%02d",lTimeinfo->tm_hour,lTimeinfo->tm_min,lTimeinfo->tm_sec);
	CReal_SvrTradeData::getObj().SetUserSettlementConfirmed(lp->InvestorID,lTemp.ConfirmDate,lTemp.ConfirmTime);


	

// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 		Cmd_TradeDataManager_SettlementConfirm_Rsp,
// 		&lTemp, 
// 		sizeof(CThostFtdcSettlementInfoConfirmField),
// 		PkgHead.seq,
// 		0,
// 		PkgHead.userdata1,
// 		PkgHead.userdata2,
// 		CF_ERROR_SUCCESS,0);

	return true;
}
//��ѯί�н����˺ŵ�״̬
bool ReqQryTraderAccountStatus(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	if(PkgHead.len != 0)
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryAccountStatus_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}
	//���ǳ�ʼ�������кͽ���״̬���ò�ѯ
	if(CReal_SvrTradeData::getObj().GetTraderDataStatus() != TradeDataStatusTrading &&
		CReal_SvrTradeData::getObj().GetTraderDataStatus() != TradeDataStatusInitialing )
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryAccountStatus_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_TRADEDATA_FORBID_QRYACCOUNT,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_TRADEDATA_FORBID_QRYACCOUNT,0);
		return false;
	}

	std::vector<sAccountStatus> lVecStatus;
	CReal_SvrTradeData::getObj().GetAllAccountStatus(lVecStatus);

// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 		Cmd_TradeDataManager_QryAccountStatus_Rsp,
// 		lVecStatus.size() == 0 ? NULL :&lVecStatus[0],
// 		sizeof(sAccountStatus)*lVecStatus.size(),
// 		PkgHead.seq,
// 		0,
// 		PkgHead.userdata1,
// 		PkgHead.userdata2,
// 		CF_ERROR_SUCCESS,0);

	return true;
}

//�ֶ�����ĳһ��ί�н����ʺŵĳ�ʼ��
bool ReqQryTraderAccountInit(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	if(PkgHead.len != sizeof(int) || pPkgData == NULL)
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_LoginAccount_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}

//     if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,ACCOUNT_LOGIN))
//     {		
//         SendRspError(hSocket,Cmd_TradeDataManager_LoginAccount_Rsp,
//             PkgHead.seq,
//             CF_ERROR_COMMON_NO_PRIVILEGE,
//             PkgHead.userdata1,
//             PkgHead.userdata2,
//             CF_ERROR_COMMON_NO_PRIVILEGE,0);
//         return false;
//     }

    //�ǽ���״̬���ó�ʼ��ί�н����ʺ�
	if(CReal_SvrTradeData::getObj().GetTraderDataStatus() != TradeDataStatusTrading)
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_LoginAccount_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_TRADEDATA_FORBID_ACCOUNTLOING,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_TRADEDATA_FORBID_ACCOUNTLOING,0);
		return false;
	}

	int lnAccountID = *(int*)pPkgData;
	TradeAccount lAccountInfo;
// 	CInterface_SvrBrokerInfo::getObj().GetTradeAccount(lnAccountID,lAccountInfo);

	BrokerInfo lBroker;
// 	CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lAccountInfo.nBrokerID,lBroker);

	
	BrokerAccountKey  lpKey(lBroker.szBrokerCode,lAccountInfo.szTradeAccount);

	CReal_SvrTradeData::getObj().LoginAdnInitTraderAccount(lpKey,lnAccountID);
// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 		Cmd_TradeDataManager_LoginAccount_Rsp,
// 		NULL ,
// 		0,
// 		PkgHead.seq,
// 		0,
// 		PkgHead.userdata1,
// 		PkgHead.userdata2,
// 		CF_ERROR_SUCCESS,0);
	return true;
}

//��ѯ�û���ʼ��״̬
bool ReqQryUserInitStatus(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	if(PkgHead.len != 0)
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryUserInitStatus_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}

	//���ǳ�ʼ�������кͽ���״̬���ò�ѯ
	if(CReal_SvrTradeData::getObj().GetTraderDataStatus() != TradeDataStatusTrading &&
		CReal_SvrTradeData::getObj().GetTraderDataStatus() != TradeDataStatusInitialing)
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryUserInitStatus_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_TRADEDATA_FORBID_QRYUSER,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_TRADEDATA_FORBID_QRYUSER,0);
		return false;
	}

	std::vector<sUserInitStatus> lVecStatus;
	CReal_SvrTradeData::getObj().GetAlUserInitStatus(lVecStatus);

// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 		Cmd_TradeDataManager_QryUserInitStatus_Rsp,
// 		lVecStatus.size() == 0 ? NULL :&lVecStatus[0],
// 		sizeof(sUserInitStatus)*lVecStatus.size(),
// 		PkgHead.seq,
// 		0,
// 		PkgHead.userdata1,
// 		PkgHead.userdata2,
// 		CF_ERROR_SUCCESS,0);

	return true;
}

//��ʼ�������û�����
bool ReqInitNewUser(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	if(PkgHead.len != sizeof(int) || pPkgData == NULL)
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_InitUser_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}

//     if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,USER_INIT))
//     {		
//         SendRspError(hSocket,Cmd_TradeDataManager_InitUser_Rsp,
//             PkgHead.seq,
//             CF_ERROR_COMMON_NO_PRIVILEGE,
//             PkgHead.userdata1,
//             PkgHead.userdata2,
//             CF_ERROR_COMMON_NO_PRIVILEGE,0);
//         return false;
//     }

	//�ǽ���״̬���ó�ʼ������Ա
	if(CReal_SvrTradeData::getObj().GetTraderDataStatus() != TradeDataStatusTrading)
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_InitUser_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_TRADEDATA_FORBID_USERINIT,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_TRADEDATA_FORBID_USERINIT,0);
		return false;
	}

	int lnUserID = *(int*)pPkgData;

	UserInfo lUserInfo;
// 	CInterface_SvrUserOrg::getObj().GetUserByID(lnUserID,lUserInfo);

	TradeAccount tradeAccount;
// 	if(!CInterface_SvrUserOrg::getObj().GetTradeAccountByUserID(lnUserID,tradeAccount))
// 	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_InitUser_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_TRADEDATA_INIT_NORELEATION,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_TRADEDATA_INIT_NORELEATION,0);
// 		return false;
// 	}
// 
// 	BrokerInfo lBroker;
// 	CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(tradeAccount.nBrokerID,lBroker);
// 
// 	CF_ERROR lError = CReal_SvrTradeData::getObj().AddTraderUserWhenTrading(lUserInfo.szAccount,tradeAccount.szTradeAccount,lBroker.szBrokerCode);
// 	if(lError == CF_ERROR_SUCCESS)
// 	{
// 		CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 			Cmd_TradeDataManager_InitUser_Rsp,
// 			NULL ,
// 			0,
// 			PkgHead.seq,
// 			lError,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			lError,0);
// 
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a UserInit to SLaver %d",lVecSocket[i]);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterUserInit_Push,
// 				(void*)pPkgData,
// 				sizeof(int),
// 				PkgHead.seq,
// 				0,
// 				PkgHead.userdata1,
// 				PkgHead.userdata2,
// 				CF_ERROR_SUCCESS,0);
// 		}
// 
// 	}
// 	else
// 	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_InitUser_Rsp,
// 			PkgHead.seq,
// 			lError,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			lError,0);
// 	}

	return true;
}

//ǿ�Ƴ�ʼ��ģ��
bool ReForceInitModule(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	if(PkgHead.len != 0 )
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_ReForceInit_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}

// 	if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,CONTROL_SERVER_RESET))
// 	{		
// 		SendRspError(hSocket,Cmd_TradeDataManager_ReForceInit_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_NO_PRIVILEGE,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_NO_PRIVILEGE,0);
// 		return false;
// 	}

	//���ǽ���������д����㽻���գ����޽�����״̬�ͳ�ʼ��ʧ��״̬�������³�ʼ��������
	if(CReal_SvrTradeData::getObj().GetTraderDataStatus() != TradeDataStatusSettlemtEnd &&
		CReal_SvrTradeData::getObj().GetTraderDataStatus() != TradeDataStatusLastDaySettlemtNotOK  &&
		CReal_SvrTradeData::getObj().GetTraderDataStatus() != TradeDataStatusNoTradingDay &&
		CReal_SvrTradeData::getObj().GetTraderDataStatus() != TradeDataStatusInitFail)
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_ReForceInit_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_TRADEDATA_FORBID_REINIT,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_TRADEDATA_FORBID_REINIT,0);
		return false;
	}



// 	CReal_SvrTradeData::getObj().TradeDataInit(string szUser,string szBrokerCode,string szTradeAccount,true);
// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 		Cmd_TradeDataManager_ReForceInit_Rsp,
// 		NULL ,
// 		0,
// 		PkgHead.seq,
// 		CF_ERROR_SUCCESS,
// 		PkgHead.userdata1,
// 		PkgHead.userdata2,
// 		CF_ERROR_SUCCESS,0);

	return true;
}

//��ʼ��ģ��
bool ReInitModule(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	if(PkgHead.len != 0 )
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_ReInit_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}

// 	if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,CONTROL_SERVER_RESET))
// 	{		
// 		SendRspError(hSocket,Cmd_TradeDataManager_ReInit_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_NO_PRIVILEGE,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_NO_PRIVILEGE,0);
// 		return false;
// 	}

	//���ǽ���������д����㽻���գ����޽�����״̬�ͳ�ʼ��ʧ��״̬�������³�ʼ��������
	if(CReal_SvrTradeData::getObj().GetTraderDataStatus() != TradeDataStatusSettlemtEnd &&
		CReal_SvrTradeData::getObj().GetTraderDataStatus() != TradeDataStatusLastDaySettlemtNotOK  &&
		CReal_SvrTradeData::getObj().GetTraderDataStatus() != TradeDataStatusNoTradingDay &&
		CReal_SvrTradeData::getObj().GetTraderDataStatus() != TradeDataStatusInitFail)
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_ReInit_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_TRADEDATA_FORBID_REINIT,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_TRADEDATA_FORBID_REINIT,0);
		return false;
	}
	
	

// 	CReal_SvrTradeData::getObj().TradeDataInit(string szUser,string szBrokerCode,string szTradeAccount,false);
// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 		Cmd_TradeDataManager_ReInit_Rsp,
// 		NULL ,
// 		0,
// 		PkgHead.seq,
// 		CF_ERROR_SUCCESS,
// 		PkgHead.userdata1,
// 		PkgHead.userdata2,
// 		CF_ERROR_SUCCESS,0);

	return true;
}



//��ѯ��ʼ��ģ��ʱ��
bool QryReinitTime(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	if(PkgHead.len != 0 )
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryReinitTime_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}


	sReinitTime lsReinitTime;
	memset(&lsReinitTime,0,sizeof(lsReinitTime));
	CF_ERROR lError = CReal_SvrTradeData::getObj().GetReinitTime(lsReinitTime);

	if(lError == CF_ERROR_SUCCESS)
	{
// 		CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 			Cmd_TradeDataManager_QryReinitTime_Rsp,
// 			&lsReinitTime ,
// 			sizeof(lsReinitTime),
// 			PkgHead.seq,
// 			lError,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			lError,0);
	}
	else
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryReinitTime_Rsp,
// 			PkgHead.seq,
// 			lError,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			lError,0);
	}
	return true;

}

//���ó�ʼ��ģ��ʱ��
bool SetReinitTime(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	if(PkgHead.len != sizeof(sReinitTime) || pPkgData == NULL)
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_SetReinitTime_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}

// 	if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,CONTROL_TIME_SETUP))
// 	{		
// 		SendRspError(hSocket,Cmd_TradeDataManager_SetReinitTime_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_NO_PRIVILEGE,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_NO_PRIVILEGE,0);
// 		return false;
// 	}

	sReinitTime *lsReinitTime = (sReinitTime*)pPkgData;
	CF_ERROR lError = CReal_SvrTradeData::getObj().SetReinitTime(*lsReinitTime);
	
	if(lError == CF_ERROR_SUCCESS)
	{
// 		CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 			Cmd_TradeDataManager_SetReinitTime_Rsp,
// 			NULL ,
// 			0,
// 			PkgHead.seq,
// 			lError,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			lError,0);
	}
	else
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_SetReinitTime_Rsp,
// 			PkgHead.seq,
// 			lError,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			lError,0);
	}
	return true;
}


//��ȡ��ǰ״̬
bool QryCurrentStatus(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	if(PkgHead.len != 0 )
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryStatus_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}

	
	eTradeDataStatus leStatus = CReal_SvrTradeData::getObj().GetTraderDataStatus();
// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 		Cmd_TradeDataManager_QryStatus_Rsp,
// 		&leStatus ,
// 		sizeof(leStatus),
// 		PkgHead.seq,
// 		CF_ERROR_SUCCESS,
// 		PkgHead.userdata1,
// 		PkgHead.userdata2,
// 		CF_ERROR_SUCCESS,0);

	return true;
}

//��ѯ��ʼ��ģ��ʱ��
bool QryTradingTime(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	if(PkgHead.len != 0 )
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryTradingTime_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}


	sTradingTime lsTradingTime;
	memset(&lsTradingTime,0,sizeof(lsTradingTime));
	CF_ERROR lError = CReal_SvrTradeData::getObj().GetTradingTime(lsTradingTime);

	if(lError == CF_ERROR_SUCCESS)
	{
// 		CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 			Cmd_TradeDataManager_QryTradingTime_Rsp,
// 			&lsTradingTime ,
// 			sizeof(lsTradingTime),
// 			PkgHead.seq,
// 			lError,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			lError,0);
	}
	else
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryTradingTime_Rsp,
// 			PkgHead.seq,
// 			lError,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			lError,0);
	}
	return true;

}

//���ó�ʼ��ģ��ʱ��
bool SetTradingTime(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	if(PkgHead.len != sizeof(sTradingTime) || pPkgData == NULL)
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_SetTradingTime_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}

// 	if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,CONTROL_TIME_SETUP))
// 	{		
// 		SendRspError(hSocket,Cmd_TradeDataManager_SetTradingTime_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_NO_PRIVILEGE,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_NO_PRIVILEGE,0);
// 		return false;
// 	}

	sTradingTime *lsTradingTime = (sTradingTime*)pPkgData;
	CF_ERROR lError = CReal_SvrTradeData::getObj().SetTradingTime(*lsTradingTime);

	if(lError == CF_ERROR_SUCCESS)
	{
// 		CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 			Cmd_TradeDataManager_SetTradingTime_Rsp,
// 			NULL ,
// 			0,
// 			PkgHead.seq,
// 			lError,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			lError,0);
	}
	else
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_SetTradingTime_Rsp,
// 			PkgHead.seq,
// 			lError,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			lError,0);
	}
	return true;
}


//�����ѯ�ͻ�֪ͨ��Ӧ
bool QryNotice(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 			Cmd_TradeDataManager_QryNotice_Rsp,
// 			NULL ,
// 			0,
// 			PkgHead.seq,
// 			0,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_SUCCESS,0);
	return true;
}

//�ڻ������ѯ�������֪ͨ
bool QryContractBank(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 			Cmd_TradeDataManager_QryContractBank_Rsp,
// 			NULL ,
// 			0,
// 			PkgHead.seq,
// 			0,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_SUCCESS,0);
	return true;
}

//�����ѯԤ��
bool QryParkedOrder(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 			Cmd_TradeDataManager_QryParkerOrder_Rsp,
// 			NULL ,
// 			0,
// 			PkgHead.seq,
// 			0,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_SUCCESS,0);
	return true;
}

//�����ѯԤ�񳷵�
bool QryParkedOrderAction(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 			Cmd_TradeDataManager_QryParkerOrderAction_Rsp,
// 			NULL ,
// 			0,
// 			PkgHead.seq,
// 			0,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_SUCCESS,0);
	return true;
}


//�����ѯ�������
bool QryDepthMarket(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	if(PkgHead.len != sizeof(PlatformStru_QryDepthMarketData) || pPkgData == NULL)
	{
// 		SendRspError(hSocket,Cmd_TradeDataManager_QryDepthMarket_Rsp,
// 			PkgHead.seq,
// 			CF_ERROR_COMMON_INPUT_PARAM,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}

	PlatformStru_QryDepthMarketData *lsQryMarket = (PlatformStru_QryDepthMarketData*)pPkgData;
	if(lsQryMarket->Thost.InstrumentID[0] != 0)
	{
		PlatformStru_DepthMarketData lRspQuot;
		CReal_SvrTradeData::getObj().GetQuotInfo(lsQryMarket->Thost.InstrumentID,lRspQuot);
// 		CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 			Cmd_TradeDataManager_QryDepthMarket_Rsp,
// 			&lRspQuot ,
// 			sizeof(lRspQuot),
// 			PkgHead.seq,
// 			CF_ERROR_SUCCESS,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_SUCCESS,0);
	}
	else
	{

		std::set<std::string> lsInstrumentList;
		std::vector<PlatformStru_DepthMarketData> lvecQuot;
		CReal_SvrTradeData::getObj().GetInstrumentList(lsInstrumentList);
		std::set<std::string>::iterator lIter = lsInstrumentList.begin();
		for(; lIter != lsInstrumentList.end(); lIter++)
		{
			PlatformStru_DepthMarketData lRspQuot;
			CReal_SvrTradeData::getObj().GetQuotInfo(*lIter,lRspQuot);
			lvecQuot.push_back(lRspQuot);
		}
// 		CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 			Cmd_TradeDataManager_QryDepthMarket_Rsp,
// 			lvecQuot.size() == 0 ? NULL :&lvecQuot[0],
// 			sizeof(PlatformStru_DepthMarketData)*lvecQuot.size(),
// 			PkgHead.seq,
// 			0,
// 			PkgHead.userdata1,
// 			PkgHead.userdata2,
// 			CF_ERROR_SUCCESS,0);
	}
	return true;
}

//////��������Ӧ������
//�����ѯ������
bool SlaverQryTradingDay(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	TRADE_LOG("Master recieve query trading day request");
	std::string lTradingDay;
	CReal_SvrTradeData::getObj().GetCurrentTradingDay(lTradingDay);
// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 		Cmd_TradeDataManager_SlaverQryTradingDay_Rsp,
// 		lTradingDay.size() == 0 ? NULL :(void*)lTradingDay.c_str(),
// 		lTradingDay.size() == 0 ? 0 : lTradingDay.size()+1,
// 		PkgHead.seq,
// 		0,
// 		PkgHead.userdata1,
// 		PkgHead.userdata2,
// 		CF_ERROR_SUCCESS,0);

	
	return true;
}

//����ͬ�������ͳɽ�
bool SlaverQrySyncReq(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	vector<PlatformStru_TradeInfo> lvecTrader;
	vector<PlatformStru_OrderInfo> lvecOrder;
	int lastOrderSeq = PkgHead.userdata1;
	int lastTraderSeq = PkgHead.userdata2;
	if(PkgHead.userdata3 == QryIsUser)
	{
		//���˻���
		std::string lsUserName = (char*)pPkgData;

		TRADE_LOG("Master recieve syn user %s request %d %d %d %d ",lsUserName.c_str(),lastOrderSeq,lastTraderSeq,lvecOrder.size(),lvecTrader.size());
		CReal_SvrTradeData::getObj().GetUserOrderAndTraderGESeq(lsUserName,lastOrderSeq,lastTraderSeq,lvecOrder,lvecTrader);
	}
	else
	{
		//���˻���
		BrokerAccountKey * lKey = (BrokerAccountKey*)pPkgData;
		TRADE_LOG("Master recieve syn account %s request %d %d %d %d",lKey->AccountID ,lastOrderSeq,lastTraderSeq,lvecOrder.size(),lvecTrader.size());
		CReal_SvrTradeData::getObj().GetAccountOrderAndTraderGESeq(lKey->BrokerID,lKey->AccountID,
			lastOrderSeq,lastTraderSeq,lvecOrder,lvecTrader);
	}
	//��һ��������͹�ȥ

	//�ɽ�
// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 		Cmd_TradeDataManager_SlaverQryOrderAndTraderBiggerFTID_Rsp,
// 		lvecTrader.size() == 0 ? NULL : &lvecTrader[0],
// 		lvecTrader.size()*sizeof(PlatformStru_TradeInfo),
// 		PkgHead.seq,
// 		0,
// 		CF_ERROR_SUCCESS,
// 		0,
// 		PkgHead.userdata3,0);
// 	//����
// 	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
// 		Cmd_TradeDataManager_SlaverQryOrderAndTraderBiggerFTID_Rsp,
// 		lvecOrder.size() == 0 ? (void*)pPkgData : &lvecOrder[0],
// 		lvecOrder.size() == 0 ? PkgHead.len :lvecOrder.size()*sizeof(PlatformStru_OrderInfo),
// 		PkgHead.seq,
// 		0,
// 		CF_ERROR_SUCCESS,
// 		QryIsOrder,
// 		PkgHead.userdata3,
// 		lvecOrder.size() == 0 ? QryIsEnd : QryIsNotEnd);

	
	return true;
}

//�ӷ�������ͬ������
bool SlaverReqMasterQuotSend(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
    std::set<std::string> outData;
    PlatformStru_DepthMarketData data;
    if(CReal_SvrTradeData::getObj().GetInstrumentList(outData))
    {
        for(std::set<std::string>::iterator it=outData.begin();it!=outData.end();it++)
        {
            std::string inst = *it;
            if(-1 == CReal_SvrTradeData::getObj().GetQuotInfo(inst,data))
            {
                //���͸�ȫ���ӷ���
                //CReal_SvrTradeData::getObj().SetQuotInfo(data);
                //�������͸�������
				if(strlen(data.InstrumentID) == 0)
					continue;
//                 CInterface_SvrTcp::getObj().SendPkgData(hSocket,
//                     Cmd_TradeDataManager_MasterQuot_Push,
//                     &data,
//                     sizeof(data),
//                     0,
//                     0,
//                     0,
//                     0,
//                     CF_ERROR_SUCCESS,0);

                TRADE_LOG("��������Ӧ�ӷ�������������������inst=%s",data.InstrumentID);
            }
        }
    }

    return true;
}

/////�ӷ�����Ӧ������
//�����ѯ��������Ӧ
bool SlaverQryTradingDayRsp(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	TRADE_LOG("Slaver recieve query trading day Response");
	if(PkgHead.len != 0)
	{
		std::string lsCurrentTradingDay;
		lsCurrentTradingDay = ((char*)pPkgData);
		CReal_SvrTradeData::getObj().SetCurrentTradingDay(lsCurrentTradingDay,true);
		//CReal_SvrTradeData::getObj().TradeDataInitAfterGetTradingDay();
		//CReal_SvrTradeData::getObj().SlaverInitAfterGetTradingDay();

	}
	return true;
}

//��Ӧ����push
bool SlaverRecMasterOrderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	TRADE_LOG("slave recieve order push");
	PlatformStru_OrderInfo * lOrder = (PlatformStru_OrderInfo*)pPkgData;
	bool lbRet = false;
	if(lOrder)
	{	
		if(CReal_SvrTradeData::getObj().GetUserSynFinished(lOrder->InvestorID))
		{
			CReal_SvrTradeData::getObj().PushOrder(*lOrder);
			lbRet = true;
		}
		else
		{
			TRADE_LOG("this user is not syn finisher,dischar this order %s %s %d",lOrder->InvestorID,lOrder->InstrumentID,lOrder->UpdateSeq);
		}
	}
	return lbRet;
}

//��Ӧ�ɽ�push
bool SlaverRecMasterTraderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	TRADE_LOG("slave recieve trader push");
	PlatformStru_TradeInfo * lTrader = (PlatformStru_TradeInfo*)pPkgData;
	bool lbRet = false;
	if(lTrader)
	{
		if(CReal_SvrTradeData::getObj().GetUserSynFinished(lTrader->InvestorID))
		{
			CReal_SvrTradeData::getObj().PushTrade(*lTrader);
			lbRet = true;
		}
		else
		{
			TRADE_LOG("this user is not syn finisher,dischar this trader %s %s %d",lTrader->InvestorID,lTrader->InstrumentID,lTrader->UpdateSeq);
		}
	}
	return lbRet;
}

//��Ӧ����push
bool SlaverRecMasterQuotPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	//TRADE_LOG("slave recieve Quot push");
	PlatformStru_DepthMarketData * lQuot = (PlatformStru_DepthMarketData*)pPkgData;
	bool lbRet = false;
	if(lQuot)
	{
		CReal_SvrTradeData::getObj().SetQuotInfo(*lQuot);
		lbRet = true;
	}
	return lbRet;
}


//��Ӧ����push
bool SlaverRecMasterCTPOrderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	TRADE_LOG("slave recieve CTP order push");
	PlatformStru_OrderInfo * lOrder = (PlatformStru_OrderInfo*)pPkgData;
	bool lbRet = false;
	if(lOrder)
	{
		CReal_SvrTradeData::getObj().PushCTPOrder(*lOrder);
		lbRet = true;
	}
	return lbRet;
}

//��Ӧ�ɽ�push
bool SlaverRecMasterCTPTraderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	TRADE_LOG("slave recieve CTP Trader push");
	PlatformStru_TradeInfo * lTrader = (PlatformStru_TradeInfo*)pPkgData;
	bool lbRet = false;
	if(lTrader)
	{
		CReal_SvrTradeData::getObj().PushCTPTrader(*lTrader);
		lbRet = true;
	}
	return lbRet;
}

//��Ӧ����push
bool SlaverRecMasterQryCommissionPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	TRADE_LOG("slave recieve Commission rate push");
	PlatformStru_InstrumentCommissionRate * lCommission = (PlatformStru_InstrumentCommissionRate*)pPkgData;
	bool lbRet = false;
	if(lCommission)
	{
		CReal_SvrTradeData::getObj().SetCommissionRate(*lCommission);
		lbRet = true;
	}
	return lbRet;
}

//��Ӧ��֤��push
bool SlaverRecMasterQryMarginPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	TRADE_LOG("slave recieve Margin rate push");
	PlatformStru_InstrumentMarginRate * lMargin = (PlatformStru_InstrumentMarginRate*)pPkgData;
	bool lbRet = false;
	if(lMargin)
	{
		CReal_SvrTradeData::getObj().SetMarginRate(*lMargin);
		lbRet = true;
	}
	return lbRet;
}

//��Ӧ�ʽ�push
bool SlaverRecMasterQryFundPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	TRADE_LOG("slave recieve fund push");
	PlatformStru_TradingAccountInfo * lInfo = (PlatformStru_TradingAccountInfo*)pPkgData;
	bool lbRet = false;
	if(lInfo)
	{
		//TRADE_LOG("EndUserQryMargin BrokerID:%s AccountID %s send qry trader",nsBrokerID.c_str(),nAccountID.c_str());
	
		//CReal_SvrTradeData::getObj().QryFundReach(*lInfo);
		lbRet = true;
	}
	return lbRet;
}

//��Ӧ�ֲ�push
bool SlaverRecMasterQryPositionPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	TRADE_LOG("slave recieve position push");
	PlatformStru_Position * lInfo = (PlatformStru_Position*)pPkgData;
	bool lbRet = false;
	if(lInfo)
	{
		CReal_SvrTradeData::getObj().QryPositionReach(*lInfo,PkgHead.userdata1,PkgHead.userdata2);
		lbRet = true;
	}
	return lbRet;
}

//��Ӧ�ֲ���ϸpush
bool SlaverRecMasterQryPositionDetailPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	TRADE_LOG("slave recieve position detail push");
	PlatformStru_PositionDetail * lInfo = (PlatformStru_PositionDetail*)pPkgData;
	bool lbRet = false;
	if(lInfo)
	{
		CReal_SvrTradeData::getObj().QryPositionDetailReach(*lInfo,PkgHead.userdata1,PkgHead.userdata2);
		lbRet = true;
	}
	return lbRet;
}


//��Ӧ�û���ʼ��
bool SlaverRecUserInitPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	TRADE_LOG("slave recieve user init push");
	int lnUserID = *(int*)pPkgData;

	UserInfo lUserInfo;
// 	if(false == CInterface_SvrUserOrg::getObj().GetUserByID(lnUserID,lUserInfo))
// 	{
// 		TRADE_LOG("can not get user by id");
// 		return false;
// 	}
// 
// 	TradeAccount tradeAccount;
// 	if(!CInterface_SvrUserOrg::getObj().GetTradeAccountByUserID(lnUserID,tradeAccount))
// 	{
// 		TRADE_LOG("can not get TraderAccount by user id");
// 		return false;
// 	}
// 
// 	BrokerInfo lBroker;
// 	CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(tradeAccount.nBrokerID,lBroker);
// 
// 	CF_ERROR lError = CReal_SvrTradeData::getObj().AddTraderUserWhenTrading(lUserInfo.szAccount,tradeAccount.szTradeAccount,lBroker.szBrokerCode);
// 	
	return true;
}


//��Ӧ�ֲ�push
bool SlaverRecMasterQryOrderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	TRADE_LOG("slave recieve Qry order push");
	PlatformStru_OrderInfo * lInfo = (PlatformStru_OrderInfo*)pPkgData;
	bool lbRet = false;
	if(lInfo)
	{
		CReal_SvrTradeData::getObj().QryOrderReach(*lInfo,PkgHead.userdata1,PkgHead.userdata2);
		lbRet = true;
	}
	return lbRet;
}

//��Ӧ�ֲ���ϸpush
bool SlaverRecMasterQryTraderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	TRADE_LOG("slave recieve Qry trader push");
	PlatformStru_TradeInfo * lInfo = (PlatformStru_TradeInfo*)pPkgData;
	bool lbRet = false;
	if(lInfo)
	{
		CReal_SvrTradeData::getObj().QryTradeReach(*lInfo,PkgHead.userdata1,PkgHead.userdata2);
		lbRet = true;
	}
	return lbRet;
}

//����ͬ�������ͳɽ�
bool SlaverQrySyncRsp(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	vector<PlatformStru_TradeInfo> lvecTrader;
	vector<PlatformStru_OrderInfo> lvecOrder;
	int lastOrderSeq = PkgHead.userdata1;
	int lastTraderSeq = PkgHead.userdata2;
	//���ʺŵ�
	if(PkgHead.userdata3 == QryIsUser && PkgHead.userdata1 == CF_ERROR_SUCCESS )
	{
		if(PkgHead.userdata2 == QryIsTrader) 
		{
			//�ɽ�
			PlatformStru_TradeInfo * lpTrader = (PlatformStru_TradeInfo*)pPkgData;
			for(unsigned int i = 0; (i < PkgHead.len /sizeof(PlatformStru_TradeInfo)) && lpTrader ; i++,lpTrader++)
			{
				TRADE_LOG("user trader %s %d updateseq:%d FTID:%d",lpTrader->InstrumentID,lpTrader->Volume,lpTrader->UpdateSeq,lpTrader->FTID);
				if(false == CReal_SvrTradeData::getObj().TestUserTraderExist(*lpTrader))
				{
					CReal_SvrTradeData::getObj().PushTrade(*lpTrader);			
				}
				else
				{
					TRADE_LOG("user trader has Exist");
				}
			
			}
			//TRADE_LOG("slave recieve syc trader rsp with user %s",lsUserName.c_str());

		}
		else if(PkgHead.userdata2 == QryIsOrder) 
		{
			//����
			std::string lsUserName;
			if(PkgHead.userdata4 == QryIsEnd)
			{
				 lsUserName = (char*)pPkgData;
			}
			else
			{
				PlatformStru_OrderInfo * lpOrder = (PlatformStru_OrderInfo*)pPkgData;
				if(lpOrder)  lsUserName = lpOrder->InvestorID;
				for(unsigned int i = 0; i < PkgHead.len /sizeof(PlatformStru_OrderInfo) && lpOrder ; i++,lpOrder++)
				{
					TRADE_LOG("User order %s %d updateseq:%d FTID:%d",lpOrder->InstrumentID,lpOrder->VolumeTotal,lpOrder->UpdateSeq,lpOrder->FTID);
					CReal_SvrTradeData::getObj().PushOrder(*lpOrder);
				}
			}
			TRADE_LOG("slave recieve syc order rsp with user %s",lsUserName.c_str());
			CReal_SvrTradeData::getObj().SetUserSynFinished(lsUserName);
			
		
		}
		
	}

	//���ʺŵ�
	if(PkgHead.userdata3 == QryIsAccount && PkgHead.userdata1 == CF_ERROR_SUCCESS )
	{
		if(PkgHead.userdata2 == QryIsTrader) 
		{
			//�ɽ�
			PlatformStru_TradeInfo * lpTrader = (PlatformStru_TradeInfo*)pPkgData;
			for(unsigned int i = 0; i < PkgHead.len /sizeof(PlatformStru_TradeInfo) && lpTrader ; i++,lpTrader++)
			{
				TRADE_LOG("account trader %s %d updateseq:%d FTID:%d",lpTrader->InstrumentID,lpTrader->Volume,lpTrader->UpdateSeq,lpTrader->FTID);
				CReal_SvrTradeData::getObj().PushCTPTrader(*lpTrader);
			}

		}
		else if(PkgHead.userdata2 == QryIsOrder) 
		{
			//����
		
			if(PkgHead.userdata4 == QryIsEnd)
			{
				BrokerAccountKey * lp = (BrokerAccountKey*)pPkgData;
				if(lp)
				{
					CReal_SvrTradeData::getObj().SetAccountSynFinished(*lp);
					TRADE_LOG("slave recieve syc order rsp with account %s",lp->AccountID);
				}
			}
			else
			{
				PlatformStru_OrderInfo * lpOrder = (PlatformStru_OrderInfo*)pPkgData;	
				if(lpOrder)
				{
					BrokerAccountKey lKey(lpOrder->BrokerID,lpOrder->InvestorID);
					CReal_SvrTradeData::getObj().SetAccountSynFinished(lKey);
					TRADE_LOG("slave recieve syc order rsp with account %s",lKey.AccountID);
				}
				for(unsigned int i = 0; i < PkgHead.len /sizeof(PlatformStru_OrderInfo) && lpOrder ; i++,lpOrder++)
				{
					TRADE_LOG("Account order %s %d updateseq:%d FTID:%d",lpOrder->InstrumentID,lpOrder->VolumeTotal,lpOrder->UpdateSeq,lpOrder->FTID);
					CReal_SvrTradeData::getObj().PushCTPOrder(*lpOrder);
				}
			}
			
		
		}	
	}

	return true;

}



//��Ӧ�µ�����
bool SlaverRecMasterErrOrderInsertPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{

	TRADE_LOG("slave recieve Err order insert push");
	PlatformStru_OrderInfo * lInfo = (PlatformStru_OrderInfo*)pPkgData;
	bool lbRet = false;
	if(lInfo)
	{
// 		CReal_SvrTradeData::getObj().CacheErrOrderInsert(*lInfo);
		lbRet = true;
	}
	return lbRet;
}

//��Ӧ��������
bool SlaverRecMasterErrOrderActionPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	TRADE_LOG("slave recieve Err order insert push");
	CThostFtdcOrderActionField * lInfo = (CThostFtdcOrderActionField*)pPkgData;
	bool lbRet = false;
	if(lInfo)
	{
// 		CReal_SvrTradeData::getObj().CacheErrOrderAction(*lInfo);
		lbRet = true;
	}
	return lbRet;
}