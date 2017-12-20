
#include "stdafx.h"
#include "ExeCmdDealFun.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "SvrTradeExcuteImpl.h"
#include "..\SvrLogin\Interface_SvrLogin.h"
#include "..\SvrUserOrg\Interface_SvrUserOrg.h"
#include "..\SvrTcp\Interface_SvrTcp.h"
#include "..\..\FastTrader\inc\ctp\ThostFtdcUserApiStruct.h"
#include "..\SvrTradeData\Interface_SvrTradeData.h"
#include "..\SvrOffer\Interface_SvrOffer.h"
#include "..\\OpenSource\\tinyxml\\tinyxml.h"
//
//void GenerateOrderInfo(const PlatformStru_InputOrder& inputOrder,PlatformStru_OrderInfo & orderInfo)
//{
//	memset(&orderInfo, 0, sizeof(orderInfo));
//	strcpy(orderInfo.BrokerID, inputOrder.BrokerID);
//	strcpy(orderInfo.InvestorID, inputOrder.InvestorID);
//	strcpy(orderInfo.InstrumentID, inputOrder.InstrumentID);
//	strcpy(orderInfo.OrderRef, inputOrder.OrderRef);
//	strcpy(orderInfo.UserID, inputOrder.UserID);
//	orderInfo.OrderPriceType = inputOrder.OrderPriceType;
//	orderInfo.Direction = inputOrder.Direction;
//	strcpy(orderInfo.CombOffsetFlag, inputOrder.CombOffsetFlag);
//	strcpy(orderInfo.CombHedgeFlag, inputOrder.CombHedgeFlag);
//	orderInfo.LimitPrice = inputOrder.LimitPrice;
//	orderInfo.VolumeTotalOriginal = inputOrder.VolumeTotalOriginal;
//	orderInfo.TimeCondition = inputOrder.TimeCondition;
//	strcpy(orderInfo.GTDDate, inputOrder.GTDDate);
//	orderInfo.VolumeCondition = inputOrder.VolumeCondition;
//	orderInfo.MinVolume = inputOrder.MinVolume;
//	orderInfo.ContingentCondition = inputOrder.ContingentCondition;
//	orderInfo.StopPrice = inputOrder.StopPrice;
//	orderInfo.ForceCloseReason = inputOrder.ForceCloseReason;
//	orderInfo.IsAutoSuspend = inputOrder.IsAutoSuspend;
//	strcpy(orderInfo.BusinessUnit, inputOrder.BusinessUnit);
//	orderInfo.MinVolume = inputOrder.MinVolume;
//	strcpy(orderInfo.szAccount, inputOrder.InvestorID);
//	orderInfo.OrderSubmitStatus = '0';
//	orderInfo.bInvalid = false;
//}
//
//void GenerateInputOrder(const PlatformStru_OrderInfo& orderInfo,PlatformStru_InputOrder & inputOrder)
//{
//	memset(&inputOrder, 0, sizeof(inputOrder));
//	strcpy(inputOrder.BrokerID, orderInfo.BrokerID);
//	strcpy(inputOrder.InvestorID, orderInfo.InvestorID);
//	strcpy(inputOrder.InstrumentID, orderInfo.InstrumentID);
//	strcpy(inputOrder.OrderRef, orderInfo.OrderRef);
//	strcpy(inputOrder.UserID, orderInfo.UserID);
//	inputOrder.OrderPriceType = orderInfo.OrderPriceType;
//	inputOrder.Direction = orderInfo.Direction;
//	strcpy(inputOrder.CombOffsetFlag, orderInfo.CombOffsetFlag);
//	strcpy(inputOrder.CombHedgeFlag, orderInfo.CombHedgeFlag);
//	inputOrder.LimitPrice = orderInfo.LimitPrice;
//	inputOrder.VolumeTotalOriginal = orderInfo.VolumeTotalOriginal;
//	inputOrder.TimeCondition = orderInfo.TimeCondition;
//	strcpy(inputOrder.GTDDate, orderInfo.GTDDate);
//	inputOrder.VolumeCondition = orderInfo.VolumeCondition;
//	inputOrder.MinVolume = orderInfo.MinVolume;
//	inputOrder.ContingentCondition = orderInfo.ContingentCondition;
//	inputOrder.StopPrice = orderInfo.StopPrice;
//	inputOrder.ForceCloseReason = orderInfo.ForceCloseReason;
//	inputOrder.IsAutoSuspend = orderInfo.IsAutoSuspend;
//	strcpy(inputOrder.BusinessUnit, orderInfo.BusinessUnit);
//	inputOrder.MinVolume = orderInfo.MinVolume;
//	strcpy(inputOrder.strAccount, orderInfo.InvestorID);
//}

bool BroadcastBufToClient(const int nRspID,const char* nspUserId,PVOID buf,int nBufLen,int nRequestID)
{
	if(nspUserId == NULL)
		return false;

	if(nRspID == Cmd_TradeExcute_OrderStatus_Rsp && nBufLen == sizeof(PlatformStru_OrderInfo))
	{
		//��ӵ��������ݹ���ģ��־û�
		PlatformStru_OrderInfo* orderInfo = (PlatformStru_OrderInfo*)buf;
		CInterface_SvrTradeData::getObj().PushExecuteOutOrder(*orderInfo);
	}
	std::vector<SOCKET> lvecSocket;
	int nUserID = 0;
	CInterface_SvrLogin::getObj().GetUserSockets(nspUserId,lvecSocket,nUserID);
	for(unsigned int i = 0; i < lvecSocket.size(); i++)
		CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[i],
			nRspID,
			buf,
			nBufLen,
			0,0,
			nUserID,
			nRequestID,
			CF_ERROR_SUCCESS);
	return true;
}


//�µ�

bool InputOrderReqDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	int n1= GetTickCount();
	static int gnInputCount = 1;
	TRADE_EXT_LOG("InputOrderReqDeal start %d",gnInputCount++);
	if(PkgHead.len != sizeof(PlatformStru_InputOrder))
	{
		SendRspError(hSocket,Cmd_TradeExcute_InputOrder_Rsp,
			PkgHead.seq,
			CF_ERROR_COMMON_INPUT_PARAM,
			PkgHead.userdata1,
			PkgHead.userdata2,
			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;

	
	}

	int n2= GetTickCount();
	PlatformStru_InputOrder order = *(PlatformStru_InputOrder*)pPkgData;
	PlatformStru_OrderInfo orderInfo;
	CInterface_SvrTradeData::getObj().GenerateOrderInfo(order,orderInfo);

	

	//��ӵ��������ݹ���ģ�黺��־û�
	CInterface_SvrTradeData::getObj().PushExecuteInOrder(orderInfo);

	int n3= GetTickCount();
	int nUserID = 0, nFrontID = 0, nSessionID = 0;
	int nErrorCode = CInterface_SvrLogin::getObj().GetSocketUserIDAndFrontSessionID(hSocket, nUserID, nFrontID, nSessionID);
	if(CF_ERROR_SUCCESS != nErrorCode )
	{
		TRADE_EXT_LOG("OrderInsert Fail:GetSocketUserIDAndFrontSessionID Fail");
		//�����ѳ�������
		SendRspError(hSocket,Cmd_TradeExcute_InputOrder_Rsp,
			PkgHead.seq,
			CF_ERROR_COMMON_INPUT_PARAM,
			PkgHead.userdata1,
			PkgHead.userdata2,
			CF_ERROR_COMMON_INPUT_PARAM,0);
		return false;
	}
	else
	{
		//�����µ��ѽ���
		orderInfo.FrontID = nFrontID;
		orderInfo.SessionID = nSessionID;
		orderInfo.OrderStatus = THOST_FTDC_OST_Unknown;
		orderInfo.ExStatus = PlatformStru_OrderInfo::ExSta_none;
		strcpy(orderInfo.StatusMsg,FormatErrorCode(CF_ERROR_TRADE_HAS_RECEVIED));
		CSvrTradeExcuteImpl::getObj().UpdateOrderDeal(orderInfo);
		CInterface_SvrTradeData::getObj().PushOrder(orderInfo,true);
	
	
	}
	int n4= GetTickCount();

	bool bNeedVerify = false;
	nErrorCode = CSvrTradeExcuteImpl::getObj().PreFilterOrder(nUserID, orderInfo, bNeedVerify);
	if ( nErrorCode != CF_ERROR_SUCCESS )
	{
		TRADE_EXT_LOG("OrderInsert Fail:PreFilterOrder Fail");
		//�����ѳ�������	
		CSvrTradeExcuteImpl::getObj().CancelOrderDeal(orderInfo,nErrorCode,true);
		return false;
	}


	TRADE_EXT_LOG("OrderInsert:InvestID:%s InstrumentID:%s FrondID:%d SessionID:%d Orderref:%s",
		orderInfo.InvestorID,
		orderInfo.InstrumentID,
		orderInfo.FrontID,
		orderInfo.SessionID,
		orderInfo.OrderRef);
	int n5= GetTickCount();
	//�жϸ��û���׼��������
	if(CInterface_SvrTradeData::getObj().GetUserInitStatus(orderInfo.InvestorID) != USER_STATUS_Inited)
	{	
		TRADE_EXT_LOG("OrderInsert Fail:GetUserInitStatus Fail");	
		CSvrTradeExcuteImpl::getObj().CancelOrderDeal(orderInfo,CF_ERROR_TRADE_USER_TRADE_NOTREADY,true);
	
		return false;
	}
	//�жϸ��û���׼��������
	PlatformStru_InstrumentInfo lTempInfo;
	if(CInterface_SvrTradeData::getObj().GetInstrumentInfo(orderInfo.InstrumentID,lTempInfo) != CF_ERROR_SUCCESS)
	{	
		TRADE_EXT_LOG("OrderInsert Fail:GetInstrumentInfo Fail");	
		CSvrTradeExcuteImpl::getObj().CancelOrderDeal(orderInfo,CF_ERROR_TRADEDATA_WRONG_INSTRUMENT,true);

		return false;
	}
	int n6= GetTickCount();
	//�ж��Ƿ��Ӧ�ĺ�Լ�з���,��֤����
	nErrorCode = CInterface_SvrTradeData::getObj().CheckUserInstrumentCommissionAndMaginExist(
		orderInfo.InvestorID,orderInfo.InstrumentID);
	if( nErrorCode != CF_ERROR_SUCCESS)
	{	
		TRADE_EXT_LOG("OrderInsert Fail:CheckUserInstrumentCommissionAndMaginExist Fail");
		CSvrTradeExcuteImpl::getObj().CancelOrderDeal(orderInfo,nErrorCode,true);	
		return false;
	}

	int n7,n8,n9,n10;
	n7=n8=n9=n10=0;
	if(bNeedVerify)
	{
		TRADE_EXT_LOG("OrderInsert Need Verify");
		//ִ�л������
		nErrorCode = CSvrTradeExcuteImpl::getObj().BaseFilterOrder(orderInfo.InvestorID, orderInfo, false);
		if( nErrorCode != CF_ERROR_SUCCESS)
		{	
			TRADE_EXT_LOG("OrderInsert Fail:BaseFilterOrder Fail");
			CSvrTradeExcuteImpl::getObj().CancelOrderDeal(orderInfo,nErrorCode,true);			
			return false;
		}

	
		//�����µ����������
		orderInfo.OrderStatus = THOST_FTDC_OST_Unknown;
		orderInfo.ExStatus = PlatformStru_OrderInfo::ExSta_approving;
		strcpy(orderInfo.StatusMsg,FormatErrorCode(CF_ERROR_TRADE_VERIFYING));
		CSvrTradeExcuteImpl::getObj().UpdateOrderDeal(orderInfo);
		//��ӵ��������ݹ���ģ����ȥ
		CInterface_SvrTradeData::getObj().PushOrder(orderInfo,true);
	
	}
	else
	{
	
		n6= GetTickCount();
		//ִ�з�غͲ���
		nErrorCode = CSvrTradeExcuteImpl::getObj().AfterFilterOrder(orderInfo.InvestorID, orderInfo, false);
		if( nErrorCode != CF_ERROR_SUCCESS)
		{	
			TRADE_EXT_LOG("OrderInsert Fail:AfterFilterOrder Fail");	
			CSvrTradeExcuteImpl::getObj().CancelOrderDeal(orderInfo,nErrorCode,true);			
			return false;
		}
	
		n7= GetTickCount();
		//ִ�л������
		nErrorCode = CSvrTradeExcuteImpl::getObj().BaseFilterOrder(orderInfo.InvestorID, orderInfo, false);
		if( nErrorCode != CF_ERROR_SUCCESS)
		{	
			TRADE_EXT_LOG("OrderInsert Fail:BaseFilterOrder Fail");		
			CSvrTradeExcuteImpl::getObj().CancelOrderDeal(orderInfo,nErrorCode,true);		
			return false;
		}

		n8= GetTickCount();
		//�µ����������ݹ���ִ�ж���
		TRADE_EXT_LOG("Push Order to tradedata to freeze fund");
		orderInfo.OrderStatus = THOST_FTDC_OST_Unknown;
		orderInfo.ExStatus = PlatformStru_OrderInfo::ExSta_none;
		strcpy(orderInfo.StatusMsg,FormatErrorCode(CF_ERROR_TRADE_HAS_SENDCTP));
		CSvrTradeExcuteImpl::getObj().UpdateOrderDeal(orderInfo);
		CInterface_SvrTradeData::getObj().PushOrder(orderInfo,true);
	
		n9= GetTickCount();
		//�µ�����
		InputOrderKey lKey;
		lKey.nFrontID = orderInfo.FrontID;
		lKey.nSessionID = orderInfo.SessionID;
		memcpy(&lKey.szOrderRef,orderInfo.OrderRef,sizeof(TThostFtdcOrderRefType));
		nErrorCode = CInterface_SvrOffer::getObj().ReqOrderInsert(&order,nUserID, lKey, 0);
		if ( nErrorCode != CF_ERROR_SUCCESS )
		{
			TRADE_EXT_LOG("OrderInsert Fail:ReqOrderInsert Fail");		
			CSvrTradeExcuteImpl::getObj().CancelOrderDeal(orderInfo,nErrorCode,true);		
			return false;
		}
		n10= GetTickCount();
	
		
	}
	//OUTPUT_LOG("single order %d %d %d %d %d %d %d %d %d ",n2-n1,n3-n2,n4-n3,n5-n4,n6-n5,n7-n6,n8-n7,n9-n8,n10-n9);
	return true;
}

//����
bool ActionOrderReqDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{

	if(PkgHead.len != sizeof(PlatformStru_InputOrderAction))
	{
		SendRspError(hSocket,Cmd_TradeExcute_CancelOrder_Error_RSP,
			PkgHead.seq,
			CF_ERROR_COMMON_INPUT_PARAM,
			PkgHead.userdata1,
			PkgHead.userdata2,
			CF_ERROR_COMMON_INPUT_PARAM,0);	
		return false;

	}
	
	PlatformStru_InputOrderAction order = *(PlatformStru_InputOrderAction*)pPkgData;
	CThostFtdcRspInfoField lRsp;
	OrderKey lKey;
	strcpy(lKey.Account,order.Thost.InvestorID);
	strcpy(lKey.InstrumentID,order.Thost.InstrumentID);
	lKey.FrontID = order.Thost.FrontID;
	lKey.SessionID = order.Thost.SessionID;
	strcpy(lKey.OrderRef,order.Thost.OrderRef);


	int nUserID = 0, nFrontID = 0, nSessionID = 0;
	int nErrorCode = CInterface_SvrLogin::getObj().GetSocketUserIDAndFrontSessionID(hSocket, nUserID, nFrontID, nSessionID);
	if(CF_ERROR_SUCCESS != nErrorCode )
	{		
        lRsp.ErrorID = nErrorCode;
		strcpy(lRsp.ErrorMsg,FormatErrorCode(nErrorCode));
		char * buf = (char*)malloc(sizeof(order)+sizeof(lRsp));
		memcpy(buf,&order,sizeof(order));
		memcpy(buf+sizeof(order),&lRsp,sizeof(lRsp));
		CInterface_SvrTcp::getObj().SendPkgData(hSocket,
			Cmd_TradeExcute_CancelOrder_Fail_RSP,
			buf,
			sizeof(PlatformStru_InputOrderAction) + sizeof(lRsp),
			PkgHead.seq,0,
			PkgHead.userdata1,
			PkgHead.userdata2,
			CF_ERROR_SUCCESS);
		free(buf);
		return false;
	}
	else
	{
		//�жϸ��û���׼��������
		if(CInterface_SvrTradeData::getObj().GetUserInitStatus(order.Thost.InvestorID) != USER_STATUS_Inited)
		{	
			lRsp.ErrorID = CF_ERROR_NOINITED;
			strcpy(lRsp.ErrorMsg,FormatErrorCode(nErrorCode));
			char * buf = (char*)malloc(sizeof(order)+sizeof(lRsp));
			memcpy(buf,&order,sizeof(order));
			memcpy(buf+sizeof(order),&lRsp,sizeof(lRsp));
			CInterface_SvrTcp::getObj().SendPkgData(hSocket,
				Cmd_TradeExcute_CancelOrder_Fail_RSP,
				buf,
				sizeof(PlatformStru_InputOrderAction) + sizeof(lRsp),
				PkgHead.seq,0,
				PkgHead.userdata1,
				PkgHead.userdata2,
				CF_ERROR_SUCCESS);
			free(buf);
			return false;
		}

		//�жϸõ��ǲ��������״̬
		PlatformStru_OrderInfo lOrderInfo;
		CF_ERROR nErrorCode = CInterface_SvrTradeData::getObj().GetUserOrderByKey(lKey.Account,lKey,lOrderInfo);
		if(nErrorCode != CF_ERROR_SUCCESS)
		{
			lRsp.ErrorID = nErrorCode;
			strcpy(lRsp.ErrorMsg,FormatErrorCode(nErrorCode));
			char * buf = (char*)malloc(sizeof(order)+sizeof(lRsp));
			memcpy(buf,&order,sizeof(order));
			memcpy(buf+sizeof(order),&lRsp,sizeof(lRsp));
			CInterface_SvrTcp::getObj().SendPkgData(hSocket,
				Cmd_TradeExcute_CancelOrder_Fail_RSP,
				buf,
				sizeof(PlatformStru_InputOrderAction) + sizeof(lRsp),
				PkgHead.seq,0,
				PkgHead.userdata1,
				PkgHead.userdata2,
				CF_ERROR_SUCCESS);
			free(buf);
			return false;
		}
		//�жϸõ��ǲ���ǿƽ��
		if(lOrderInfo.UserForceClose)
		{
			lRsp.ErrorID = nErrorCode;
			strcpy(lRsp.ErrorMsg,FormatErrorCode(CF_ERROR_TRADE_FORCE_ORDED_NO_ACTION));
			char * buf = (char*)malloc(sizeof(order)+sizeof(lRsp));
			memcpy(buf,&order,sizeof(order));
			memcpy(buf+sizeof(order),&lRsp,sizeof(lRsp));
			CInterface_SvrTcp::getObj().SendPkgData(hSocket,
				Cmd_TradeExcute_CancelOrder_Fail_RSP,
				buf,
				sizeof(PlatformStru_InputOrderAction) + sizeof(lRsp),
				PkgHead.seq,0,
				PkgHead.userdata1,
				PkgHead.userdata2,
				CF_ERROR_SUCCESS);
			free(buf);
			return false;
		}
		//��������״̬�����ڲ�����
		if(lOrderInfo.ExStatus == PlatformStru_OrderInfo::ExSta_approving)
		{
			TRADE_EXT_LOG("OrderInsert Fail:AfterFilterOrder Fail");			
			CSvrTradeExcuteImpl::getObj().CancelOrderDeal(lOrderInfo,nErrorCode,true);
			return true;
		}
		else
		{
		
			InputOrderKey lInputKey;
			lInputKey.nFrontID = lKey.FrontID;
			lInputKey.nSessionID = lKey.SessionID;
			memcpy(&lInputKey.szOrderRef,lKey.OrderRef,sizeof(TThostFtdcOrderRefType));
			CF_ERROR lRet = CInterface_SvrOffer::getObj().ReqOrderAction(&order.Thost,nUserID,lInputKey,PkgHead.userdata2);
			if(CF_ERROR_SUCCESS == lRet )
				return true;
			else
			{
				lRsp.ErrorID = lRet;
				strcpy(lRsp.ErrorMsg,FormatErrorCode(lRet));
				char * buf = (char*)malloc(sizeof(order)+sizeof(lRsp));
				memcpy(buf,&order,sizeof(order));
				memcpy(buf+sizeof(order),&lRsp,sizeof(lRsp));
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,
					Cmd_TradeExcute_CancelOrder_Fail_RSP,
					buf,
					sizeof(PlatformStru_InputOrderAction) + sizeof(lRsp),
					PkgHead.seq,0,
					PkgHead.userdata1,
					PkgHead.userdata2,
					CF_ERROR_SUCCESS);
				free(buf);					
				return false;
			}

		}
	}

}

//����������˽��
bool VerifyedOrderDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	if ( PkgHead.len != sizeof(OrderKey))
	{
		SendRspError(hSocket,Cmd_TradeExcute_VerifyOrder_RspRsp,
			PkgHead.seq,
			CF_ERROR_COMMON_INPUT_PARAM,
			PkgHead.userdata1,
			PkgHead.userdata2,
			CF_ERROR_COMMON_INPUT_PARAM,0);	
		return false;
	}

	OrderKey * lOrderKey = (OrderKey*)pPkgData;

	PlatformStru_OrderInfo lOrderInfo;
	CF_ERROR nErrorCode = CInterface_SvrTradeData::getObj().GetUserOrderByKey(lOrderKey->Account,*lOrderKey,lOrderInfo);
	if(nErrorCode != CF_ERROR_SUCCESS)
	{
		SendRspError(hSocket,Cmd_TradeExcute_VerifyOrder_RspRsp,
			PkgHead.seq,
			nErrorCode,
			PkgHead.userdata1,
			PkgHead.userdata2,
			nErrorCode,0);	
		return false;
	}

	//����ñ��������
	if(lOrderInfo.OrderStatus != THOST_FTDC_OST_Unknown ||
		lOrderInfo.ExStatus != PlatformStru_OrderInfo::ExSta_approving)
	{
		SendRspError(hSocket,Cmd_TradeExcute_VerifyOrder_RspRsp,
			PkgHead.seq,
			CF_ERROR_TRADE_HAS_VERIFYED,
			PkgHead.userdata1,
			PkgHead.userdata2,
			CF_ERROR_TRADE_HAS_VERIFYED,0);	
		return false;
	}

	
	//�ȳ����ⶳ�ʽ�
	/*lOrderInfo.OrderStatus = THOST_FTDC_OST_Canceled;
	lOrderInfo.ExStatus = PlatformStru_OrderInfo::ExSta_none;
	CInterface_SvrTradeData::getObj().PushOrder(lOrderInfo,false);*/
	
	//������˿ͻ�����˳ɹ�
	SendRspError(hSocket,Cmd_TradeExcute_VerifyOrder_RspRsp,
		PkgHead.seq,
		CF_ERROR_SUCCESS,
		PkgHead.userdata1,
		PkgHead.userdata2,
		CF_ERROR_SUCCESS,0);

	//��˲�ͨ�� ֱ�ӷ���
	if(PkgHead.userdata4 == 0)
	{
		//���ͳ���ԭ��	
		CSvrTradeExcuteImpl::getObj().CancelOrderDeal(lOrderInfo,nErrorCode,true);
			return false;
	}

	//�µ����������ݹ���ִ�ж���
	/*TRADE_EXT_LOG("Push Order to tradedata to freeze fund");
	lOrderInfo.OrderStatus = THOST_FTDC_OST_Unknown;
	lOrderInfo.ExStatus = PlatformStru_OrderInfo::ExSta_none;
	strcpy(lOrderInfo.StatusMsg,FormatErrorCode(CF_ERROR_TRADE_HAS_VERIFYED));
	CInterface_SvrTradeData::getObj().PushOrder(lOrderInfo,true);*/

	//ִ�з�غͲ���
	nErrorCode = CSvrTradeExcuteImpl::getObj().AfterFilterOrder(lOrderInfo.InvestorID, lOrderInfo, false);
	if( nErrorCode != CF_ERROR_SUCCESS)
	{	
		TRADE_EXT_LOG("OrderInsert Fail:AfterFilterOrder Fail");
		CSvrTradeExcuteImpl::getObj().CancelOrderDeal(lOrderInfo,nErrorCode,true);
		return false;
	}

	//ִ�л������
	nErrorCode = CSvrTradeExcuteImpl::getObj().BaseFilterOrder(lOrderInfo.InvestorID, lOrderInfo, false);
	if( nErrorCode != CF_ERROR_SUCCESS)
	{	
		TRADE_EXT_LOG("OrderInsert Fail:BaseFilterOrder Fail");
		CSvrTradeExcuteImpl::getObj().CancelOrderDeal(lOrderInfo,nErrorCode,true);
		return false;
	}



	//�µ�����
	InputOrderKey lKey;
	lKey.nFrontID = lOrderInfo.FrontID;
	lKey.nSessionID = lOrderInfo.SessionID;
	memcpy(&lKey.szOrderRef,lOrderInfo.OrderRef,sizeof(TThostFtdcOrderRefType));
	PlatformStru_InputOrder lInputOrder;
	CInterface_SvrTradeData::getObj().GenerateInputOrder(lOrderInfo,lInputOrder);
	int nUserID = -1;
	vector<UserInfo> lVecUeser;
	CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUeser);
	for(unsigned int i = 0;i < lVecUeser.size();i++)
	{
		if(strcmp(lVecUeser[i].szAccount,lOrderInfo.InvestorID) == 0)
		{
			nUserID = lVecUeser[i].nUserID;
			break;
		}
	}
	nErrorCode = CInterface_SvrOffer::getObj().ReqOrderInsert(&lInputOrder,nUserID, lKey, 0);
	if ( nErrorCode != CF_ERROR_SUCCESS )
	{
		TRADE_EXT_LOG("OrderInsert Fail:ReqOrderInsert Fail");	
		CSvrTradeExcuteImpl::getObj().CancelOrderDeal(lOrderInfo,nErrorCode,true);	
		return false;
	}


	return true;
}


//��ѯ�÷��Ա�����Ľ���Ա�Ĵ���˱���
bool QryApprovingOrderInfo(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	if ( PkgHead.len != 0)
	{
		SendRspError(hSocket,Cmd_TradeExcute_QryApprovingOrder_Rsp,
			PkgHead.seq,
			CF_ERROR_COMMON_INPUT_PARAM,
			PkgHead.userdata1,
			PkgHead.userdata2,
			CF_ERROR_COMMON_INPUT_PARAM,0);	
		return false;
	}
	int nUserId = PkgHead.userdata1;
    //��ȡ��UserID����֯����
	//��ȡ����֯�����µ����н���Ա
	int lnOrgId = -1;
	if(false == CInterface_SvrUserOrg::getObj().GetOrgIDByUserID(nUserId,lnOrgId))
	{
		SendRspError(hSocket,Cmd_TradeExcute_QryApprovingOrder_Rsp,
			PkgHead.seq,
			CF_ERROR_COMMON_INPUT_PARAM,
			PkgHead.userdata1,
			PkgHead.userdata2,
			CF_ERROR_COMMON_INPUT_PARAM,0);	
		return false;
	}

	std::vector<UserInfo> lVecUserInfo;
	std::vector<PlatformStru_OrderInfo> lVecOrder;
	CInterface_SvrUserOrg::getObj().GetUserInfosByOrgID(lnOrgId,lVecUserInfo);
	for(unsigned int i = 0; i < lVecUserInfo.size();i++)
	{
		if(lVecUserInfo[i].nUserType == USER_TYPE_TRADER)
		{
			std::vector<PlatformStru_OrderInfo> lTempVecOrder;
			CInterface_SvrTradeData::getObj().GetUserUnkownOrders(lVecUserInfo[i].szAccount,lTempVecOrder);
			for(unsigned int j = 0; j < lTempVecOrder.size();j++)
			{
				if(lTempVecOrder[j].ExStatus == PlatformStru_OrderInfo::ExSta_approving )
					lVecOrder.push_back(lTempVecOrder[j]);
			}		
		}
	}
	CInterface_SvrTcp::getObj().SendPkgData(hSocket,
		Cmd_TradeExcute_QryApprovingOrder_Rsp,
		lVecOrder.size() == 0 ? NULL :&lVecOrder[0],
		sizeof(PlatformStru_TradeInfo)*lVecOrder.size(),
		PkgHead.seq,
		0,
		PkgHead.userdata1,
		PkgHead.userdata2,
		CF_ERROR_SUCCESS,0);

	return true;
}

//�µ�
bool StragegyInputOrderReqDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{

	TRADE_EXT_LOG("StragegyInputOrderReqDeal start ");
	
    //�����µ��ṹ��
	char * lpBuf = (char*) pPkgData;
	if(lpBuf == NULL)
		return false;

	TiXmlDocument lDoc;
	lDoc.Parse(lpBuf);
	TiXmlElement * lpElm = lDoc.FirstChildElement(ORDERINPUT_CHANNEL_NAME);
	if(lpElm == NULL)
	{
		//��������
		TRADE_EXT_LOG("StragegyInputOrderReqDeal Fail param error no root element");	
		return false;
	}


	int lUserID = 0, lFrontID = 0, lSessionID = 0;	
	PlatformStru_InputOrder orderInput;
	memset(&orderInput,0,sizeof(orderInput));

	bool lbIsMarket = false;
	std::string lsOrderref,lsBrokerID,lsInvestorId,llsInstrumentID;

	if( TIXML_SUCCESS != lpElm->QueryIntAttribute("FrontID",&lFrontID) || 
		TIXML_SUCCESS != lpElm->QueryIntAttribute("SessionID",&lSessionID) || 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("OrderRef",&lsOrderref)|| 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("BrokerID",&lsBrokerID)|| 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("DealerID",&lsInvestorId)|| 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("InstrumentID",&llsInstrumentID)|| 
		TIXML_SUCCESS != lpElm->QueryDoubleAttribute("Price",&orderInput.LimitPrice) ||
		TIXML_SUCCESS != lpElm->QueryBoolAttribute("IsMarket",&lbIsMarket) ||
		TIXML_SUCCESS != lpElm->QueryIntAttribute("Qty",&orderInput.VolumeTotalOriginal) ||
		TIXML_SUCCESS != lpElm->QueryValueAttribute("OpenClose",&orderInput.CombOffsetFlag[0]) ||
		TIXML_SUCCESS != lpElm->QueryValueAttribute("Hedge",&orderInput.CombHedgeFlag[0]) ||
		TIXML_SUCCESS != lpElm->QueryValueAttribute("Direction",&orderInput.Direction))
	{
		//�������󣬱���ʧ��	
		TRADE_EXT_LOG("StragegyInputOrderReqDeal Fail param error no root element");	
		return false;
	}

	strcpy(orderInput.BrokerID,lsBrokerID.c_str());
	strcpy(orderInput.InvestorID,lsInvestorId.c_str());
	strcpy(orderInput.InstrumentID,llsInstrumentID.c_str());
	strcpy(orderInput.OrderRef,lsOrderref.c_str());
	orderInput.ContingentCondition = THOST_FTDC_CC_Immediately;
	orderInput.MinVolume = 1;
	orderInput.VolumeCondition = THOST_FTDC_VC_AV; 
	orderInput.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	//����µ��ṹ��
	if(lbIsMarket)
	{
		//�۸�����
		orderInput.OrderPriceType=THOST_FTDC_OPT_AnyPrice;
		//�۸�
		orderInput.LimitPrice=0;
		//��Ч������
		orderInput.TimeCondition=THOST_FTDC_TC_IOC;
	}
	else
	{
		//�۸�����
		orderInput.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
	
		//��Ч������	
		orderInput.TimeCondition=THOST_FTDC_TC_GFD;
	}

	PlatformStru_OrderInfo orderInfo;
	CInterface_SvrTradeData::getObj().GenerateOrderInfo(orderInput,orderInfo);

	//��ӵ��������ݹ���ģ�黺��־û�
	CInterface_SvrTradeData::getObj().PushExecuteInOrder(orderInfo);

	//�����µ��ѽ���
	orderInfo.FrontID = lFrontID;
	orderInfo.SessionID = lSessionID;
	orderInfo.OrderStatus = THOST_FTDC_OST_Unknown;
	orderInfo.ExStatus = PlatformStru_OrderInfo::ExSta_none;
	strcpy(orderInfo.StatusMsg,FormatErrorCode(CF_ERROR_TRADE_HAS_RECEVIED));
	CSvrTradeExcuteImpl::getObj().UpdateOrderDeal(orderInfo);
	CInterface_SvrTradeData::getObj().PushOrder(orderInfo,true);

	int nUserID = 0;
	UserInfo lUserInfo;
	if(false == CInterface_SvrUserOrg::getObj().GetUserInfoByAccount(orderInfo.InvestorID,lUserInfo))
	{
		TRADE_EXT_LOG("StragegyInputOrderReqDeal Fail:GetUserInfoByAccount Fail");
		return false;
	}
	nUserID = lUserInfo.nUserID;

	bool bNeedVerify = false;
	CF_ERROR nErrorCode = CSvrTradeExcuteImpl::getObj().PreFilterOrder(nUserID, orderInfo, bNeedVerify);
	if ( nErrorCode != CF_ERROR_SUCCESS )
	{
		TRADE_EXT_LOG("StragegyInputOrderReqDeal Fail:PreFilterOrder Fail");
		//�����ѳ�������	
		CSvrTradeExcuteImpl::getObj().CancelOrderDeal(orderInfo,nErrorCode,true);
		return false;
	}


	TRADE_EXT_LOG("StragegyInputOrderReqDeal:InvestID:%s InstrumentID:%s FrondID:%d SessionID:%d Orderref:%s",
		orderInfo.InvestorID,
		orderInfo.InstrumentID,
		orderInfo.FrontID,
		orderInfo.SessionID,
		orderInfo.OrderRef);
	int n5= GetTickCount();
	//�жϸ��û���׼��������
	if(CInterface_SvrTradeData::getObj().GetUserInitStatus(orderInfo.InvestorID) != USER_STATUS_Inited)
	{	
		TRADE_EXT_LOG("StragegyInputOrderReqDeal Fail:GetUserInitStatus Fail");	
		CSvrTradeExcuteImpl::getObj().CancelOrderDeal(orderInfo,CF_ERROR_TRADE_USER_TRADE_NOTREADY,true);

		return false;
	}
	//�жϸ��û���׼��������
	PlatformStru_InstrumentInfo lTempInfo;
	if(CInterface_SvrTradeData::getObj().GetInstrumentInfo(orderInfo.InstrumentID,lTempInfo) != CF_ERROR_SUCCESS)
	{	
		TRADE_EXT_LOG("StragegyInputOrderReqDeal Fail:GetInstrumentInfo Fail");	
		CSvrTradeExcuteImpl::getObj().CancelOrderDeal(orderInfo,CF_ERROR_TRADEDATA_WRONG_INSTRUMENT,true);

		return false;
	}
	int n6= GetTickCount();
	//�ж��Ƿ��Ӧ�ĺ�Լ�з���,��֤����
	nErrorCode = CInterface_SvrTradeData::getObj().CheckUserInstrumentCommissionAndMaginExist(
		orderInfo.InvestorID,orderInfo.InstrumentID);
	if( nErrorCode != CF_ERROR_SUCCESS)
	{	
		TRADE_EXT_LOG("StragegyInputOrderReqDeal Fail:CheckUserInstrumentCommissionAndMaginExist Fail");
		CSvrTradeExcuteImpl::getObj().CancelOrderDeal(orderInfo,nErrorCode,true);	
		return false;
	}


	if(bNeedVerify)
	{
		TRADE_EXT_LOG("StragegyInputOrderReqDeal Need Verify");
		//ִ�л������
		nErrorCode = CSvrTradeExcuteImpl::getObj().BaseFilterOrder(orderInfo.InvestorID, orderInfo, false);
		if( nErrorCode != CF_ERROR_SUCCESS)
		{	
			TRADE_EXT_LOG("StragegyInputOrderReqDeal Fail:BaseFilterOrder Fail");
			CSvrTradeExcuteImpl::getObj().CancelOrderDeal(orderInfo,nErrorCode,true);			
			return false;
		}


		//�����µ����������
		orderInfo.OrderStatus = THOST_FTDC_OST_Unknown;
		orderInfo.ExStatus = PlatformStru_OrderInfo::ExSta_approving;
		strcpy(orderInfo.StatusMsg,FormatErrorCode(CF_ERROR_TRADE_VERIFYING));
		CSvrTradeExcuteImpl::getObj().UpdateOrderDeal(orderInfo);
		//��ӵ��������ݹ���ģ����ȥ
		CInterface_SvrTradeData::getObj().PushOrder(orderInfo,true);

	}
	else
	{

	
		//ִ�з�غͲ���
		nErrorCode = CSvrTradeExcuteImpl::getObj().AfterFilterOrder(orderInfo.InvestorID, orderInfo, false);
		if( nErrorCode != CF_ERROR_SUCCESS)
		{	
			TRADE_EXT_LOG("StragegyInputOrderReqDeal Fail:AfterFilterOrder Fail");	
			CSvrTradeExcuteImpl::getObj().CancelOrderDeal(orderInfo,nErrorCode,true);			
			return false;
		}


		//ִ�л������
		nErrorCode = CSvrTradeExcuteImpl::getObj().BaseFilterOrder(orderInfo.InvestorID, orderInfo, false);
		if( nErrorCode != CF_ERROR_SUCCESS)
		{	
			TRADE_EXT_LOG("StragegyInputOrderReqDeal Fail:BaseFilterOrder Fail");		
			CSvrTradeExcuteImpl::getObj().CancelOrderDeal(orderInfo,nErrorCode,true);		
			return false;
		}

	
		//�µ����������ݹ���ִ�ж���
		TRADE_EXT_LOG("StragegyInputOrderReqDeal Push Order to tradedata to freeze fund");
		orderInfo.OrderStatus = THOST_FTDC_OST_Unknown;
		orderInfo.ExStatus = PlatformStru_OrderInfo::ExSta_none;
		strcpy(orderInfo.StatusMsg,FormatErrorCode(CF_ERROR_TRADE_HAS_SENDCTP));
		CSvrTradeExcuteImpl::getObj().UpdateOrderDeal(orderInfo);
		CInterface_SvrTradeData::getObj().PushOrder(orderInfo,true);

	
		//�µ�����
		InputOrderKey lKey;
		lKey.nFrontID = orderInfo.FrontID;
		lKey.nSessionID = orderInfo.SessionID;
		memcpy(&lKey.szOrderRef,orderInfo.OrderRef,sizeof(TThostFtdcOrderRefType));
		nErrorCode = CInterface_SvrOffer::getObj().ReqOrderInsert(&orderInput,nUserID, lKey, 0);
		if ( nErrorCode != CF_ERROR_SUCCESS )
		{
			TRADE_EXT_LOG("StragegyInputOrderReqDeal Fail:ReqOrderInsert Fail");		
			CSvrTradeExcuteImpl::getObj().CancelOrderDeal(orderInfo,nErrorCode,true);		
			return false;
		}	

	}
	return true;
}
//����
bool StragegyActionOrderReqDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	TRADE_EXT_LOG("StragegyActionOrderReqDeal start ");
	//�����µ��ṹ��
	char * lpBuf = (char*) pPkgData;
	if(lpBuf == NULL)
		return false;

	TiXmlDocument lDoc;
	lDoc.Parse(lpBuf);
	TiXmlElement * lpElm = lDoc.FirstChildElement(ORDERACTION_CHANNEL_NAME);
	if(lpElm == NULL)
	{
		//��������
		TRADE_EXT_LOG("StragegyActionOrderReqDeal Fail param error no root element");	
		return false;
	}


	int lUserID = 0, lFrontID = 0, lSessionID = 0;	
	CThostFtdcOrderActionField orderInput;
	memset(&orderInput,0,sizeof(orderInput));

	bool lbIsMarket = false;
	std::string lsOrderref,lsBrokerID,lsInvestorId,llsInstrumentID,lsOrderSysId,lsExchangeID;

	if( TIXML_SUCCESS != lpElm->QueryIntAttribute("FrontID",&orderInput.FrontID) || 
		TIXML_SUCCESS != lpElm->QueryIntAttribute("SessionID",&orderInput.SessionID) || 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("OrderRef",&lsOrderref)|| 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("BrokerID",&lsBrokerID)|| 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("DealerID",&lsInvestorId)|| 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("InstrumentID",&llsInstrumentID)|| 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("OrderSysID",&lsInvestorId)|| 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("ExchangeID",&llsInstrumentID) )
	
	{
		//�������󣬱���ʧ��	
		TRADE_EXT_LOG("StragegyActionOrderReqDeal Fail param error no root element");	
		return false;
	}

	strcpy(orderInput.BrokerID,lsBrokerID.c_str());
	strcpy(orderInput.InvestorID,lsInvestorId.c_str());
	strcpy(orderInput.InstrumentID,llsInstrumentID.c_str());
	strcpy(orderInput.OrderRef,lsOrderref.c_str());
	strcpy(orderInput.ExchangeID,lsExchangeID.c_str());
	strcpy(orderInput.OrderSysID,lsOrderSysId.c_str());
	
	

	CThostFtdcRspInfoField lRsp;
	OrderKey lKey;
	strcpy(lKey.Account,orderInput.InvestorID);
	strcpy(lKey.InstrumentID,orderInput.InstrumentID);
	lKey.FrontID = orderInput.FrontID;
	lKey.SessionID = orderInput.SessionID;
	strcpy(lKey.OrderRef,orderInput.OrderRef);

	int nUserID = 0;
	UserInfo lUserInfo;
	if(false == CInterface_SvrUserOrg::getObj().GetUserInfoByAccount(orderInput.InvestorID,lUserInfo))
	{
		TRADE_EXT_LOG("StragegyActionOrderReqDeal Fail:GetUserInfoByAccount Fail");
		lRsp.ErrorID = CF_ERROR_LOGIN_USER_NOT_EXIST;
		strcpy(lRsp.ErrorMsg,FormatErrorCode(lRsp.ErrorID));
		CInterface_SvrTradeData::getObj().PushErrRtnOrderAction(&orderInput,&lRsp);
		return false;
	}
	nUserID = lUserInfo.nUserID;

	//�жϸ��û���׼��������
	if(CInterface_SvrTradeData::getObj().GetUserInitStatus(orderInput.InvestorID) != USER_STATUS_Inited)
	{	
		lRsp.ErrorID = CF_ERROR_NOINITED;
		strcpy(lRsp.ErrorMsg,FormatErrorCode(CF_ERROR_NOINITED));
		CInterface_SvrTradeData::getObj().PushErrRtnOrderAction(&orderInput,&lRsp);
		return false;
	}

	//�жϸõ��ǲ��������״̬
	PlatformStru_OrderInfo lOrderInfo;
	CF_ERROR nErrorCode = CInterface_SvrTradeData::getObj().GetUserOrderByKey(lKey.Account,lKey,lOrderInfo);
	if(nErrorCode != CF_ERROR_SUCCESS)
	{
		TRADE_EXT_LOG("StragegyActionOrderReqDeal Fail:GetUserOrderByKey Fail");			
		lRsp.ErrorID = nErrorCode;
		strcpy(lRsp.ErrorMsg,FormatErrorCode(nErrorCode));
		CInterface_SvrTradeData::getObj().PushErrRtnOrderAction(&orderInput,&lRsp);
		return false;
	}
	//�жϸõ��ǲ���ǿƽ��
	if(lOrderInfo.UserForceClose)
	{
		TRADE_EXT_LOG("StragegyActionOrderReqDeal Fail:UserForceClose Fail");			
		lRsp.ErrorID = CF_ERROR_TRADE_FORCE_ORDED_NO_ACTION;
		strcpy(lRsp.ErrorMsg,FormatErrorCode(CF_ERROR_TRADE_FORCE_ORDED_NO_ACTION));	
		CInterface_SvrTradeData::getObj().PushErrRtnOrderAction(&orderInput,&lRsp);
		return false;
	}
	//��������״̬�����ڲ�����
	if(lOrderInfo.ExStatus == PlatformStru_OrderInfo::ExSta_approving)
	{
		TRADE_EXT_LOG("StragegyActionOrderReqDeal Fail:ExSta_approving Fail");			
		CSvrTradeExcuteImpl::getObj().CancelOrderDeal(lOrderInfo,nErrorCode,true);
		return true;
	}
	else
	{

		InputOrderKey lInputKey;
		lInputKey.nFrontID = lKey.FrontID;
		lInputKey.nSessionID = lKey.SessionID;
		memcpy(&lInputKey.szOrderRef,lKey.OrderRef,sizeof(TThostFtdcOrderRefType));
		CF_ERROR lRet = CInterface_SvrOffer::getObj().ReqOrderAction((CThostFtdcInputOrderActionField*)&orderInput,nUserID,lInputKey,PkgHead.userdata2);
		if(CF_ERROR_SUCCESS == lRet )
			return true;
		else
		{
			lRsp.ErrorID = lRet;
			strcpy(lRsp.ErrorMsg,FormatErrorCode(lRet));
			CInterface_SvrTradeData::getObj().PushErrRtnOrderAction(&orderInput,&lRsp);	
			return false;
		}

	}
	return false;
}