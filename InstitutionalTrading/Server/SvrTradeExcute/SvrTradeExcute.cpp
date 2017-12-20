// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include "stdafx.h"


#include "SvrTradeExcute.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "Interface_SvrTradeExcute.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "RiskManageStruct.h"
#include "SvrTradeExcuteImpl.h"
#include "../SvrLogin/Interface_SvrLogin.h"
#include "../SvrNotifyAndAsk/Interface_SvrNotifyAndAsk.h"

#include "../SvrUserOrg/Interface_SvrUserOrg.h"

#include "ExeCmdDealFun.h"
#include "tools_win32.h"
//#include "ThostFtdcUserApiDataType.h"




//-----------------------------------------------------------------------------------
//	import Tools4dll��
//-----------------------------------------------------------------------------------
#ifdef _DEBUG
#pragma comment(lib, "Tools4dllD.lib")											
#else 
#pragma comment(lib, "Tools4dll.lib")											
#endif

//-----------------------------------------------------------------------------------
//	����import��ģ����Ҫ���õ�����ģ��
//-----------------------------------------------------------------------------------
#pragma comment(lib, "SvrTcp.lib")
#pragma comment(lib, "SvrTradeData.lib")
#pragma comment(lib, "SvrNotifyAndAsk.lib")
#pragma comment(lib, "SvrLogin.lib")
#pragma comment(lib, "SvrOffer.lib")
#pragma comment(lib, "SvrUserOrg.lib")
#pragma comment(lib, "SvrBrokerInfo.lib")

#define WRITELOGID_SvrTradeExcute




//ȫ�ֻ�����
Ceasymutex			g_mutex;

//�̲߳���
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;
DWORD ThreadWorker(void *arg);

//�̲߳���
HANDLE				g_hThreadSend=NULL;
DWORD				g_idThreadSend=0;
DWORD ThreadWorkerSend(void *arg);

//����һ�����յ������ݰ�
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//// ����һ������֪ͨ�¼�
bool ParseDataChangedEvent(const Stru_NotifyEvent& dataEvt);

//ģ���ʼ�������������̲߳���SvrTcp���ĸ���Ȥ�����ݰ�
SVRTRADEEXCUTE_API void InitFunc(void)
{
	//���������µ������߳�
	g_hThread=CreateThread(NULL,20*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);

	//��������ر������߳�
	g_hThreadSend=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorkerSend,0,0,&g_idThreadSend);

	CInterface_SvrTradeExcute::getObj();
	
	////���涩�ı��̸߳���Ȥ�����ݰ�
	//CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeExcute_InputOrder_Req,g_idThread);
	//CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeExcute_VerifyOrder_Rsp,g_idThread);
	//CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeExcute_CancelOrder_Req,g_idThread);
	//CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeExcute_QryApprovingOrder_Req,g_idThread);


	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeRspOrderAction, g_idThreadSend);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeErrOrderAction, g_idThreadSend);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeOrderRtn, g_idThreadSend);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeTradeRtn, g_idThreadSend);

}

//ģ��������ͷ���Դ���رչ����߳�
SVRTRADEEXCUTE_API void ReleaseFunc(void)
{
	if(g_hThread)
	{
		//�˶����ݰ�
		CInterface_SvrTcp::getObj().UnsubscribeAllPkg(g_idThread);

		//����WM_QUIT֪ͨ�߳̽���
		PostThreadMessage(g_idThread,WM_QUIT,0,0);
		//����WM_QUIT֪ͨ�߳̽���
		PostThreadMessage(g_idThreadSend,WM_QUIT,0,0);
		//�ȴ��߳��˳�
		DWORD ExitCode;
		WaitForSingleObject((HANDLE)g_hThread,8000);
		if(GetExitCodeThread((HANDLE)g_hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)g_hThread,0);
		CloseHandle(g_hThread);
		g_hThread=NULL;
		g_idThread=0;

		WaitForSingleObject((HANDLE)g_hThreadSend,8000);
		if(GetExitCodeThread((HANDLE)g_hThreadSend,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)g_hThreadSend,0);
		CloseHandle(g_hThreadSend);
		g_hThreadSend=NULL;
		g_idThreadSend=0;
	}

}
//�����߳�
DWORD ThreadWorkerSend(void *arg)
{
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeRspOrderAction, g_idThreadSend);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeErrOrderAction, g_idThreadSend);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeOrderRtn, g_idThreadSend);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeTradeRtn, g_idThreadSend);

	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{
		if(Msg.message==WM_COMMAND&&Msg.wParam==WndCmd_NotifyEventArrival)
		{
			unsigned int EventParamID=(unsigned int)Msg.lParam;
			//���ݰ�����
			Stru_NotifyEvent ls;
			int nLen = sizeof(ls);
			if(CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,&ls,&nLen,nLen,NULL))
			{		
				//�ͷ�EventParam
				//int n=GetTickCount();	

				//�������ݰ��������������ݰ�
				ParseDataChangedEvent(ls);			
				
				CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
				//int n3=GetTickCount();
				//TRADE_EXT_LOG("ProcessOneUniPkg_InThread %d" ,n3-n);
			}


		}
	}
	return 0;
}


void DealPakage(unsigned int EventParamID,int& PkgLen,int& hSocket)
{
	AllocTmpStackBuf(TmpPkg,PkgLen);
	if(TmpPkg.m_pbuf&&
		CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,TmpPkg.m_pbuf,&PkgLen,PkgLen,&hSocket)&&
		Stru_UniPkgHead::IsValidPkg(TmpPkg.m_pbuf,PkgLen))
	{
		Stru_UniPkgHead& PkgHead=*((Stru_UniPkgHead*)TmpPkg.m_pbuf);
		void* pPkgData=(char*)TmpPkg.m_pbuf+sizeof(Stru_UniPkgHead);
		
		//�������ݰ��������������ݰ�
		ProcessOneUniPkg_InThread(hSocket,PkgHead,pPkgData);	
	}
}
//�����߳�
DWORD ThreadWorker(void *arg)
{
	//���涩�ı��̸߳���Ȥ�����ݰ�
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeExcute_InputOrder_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeExcute_VerifyOrder_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeExcute_CancelOrder_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeExcute_QryApprovingOrder_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeExcute_StategyInputOrder_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeExcute_StategyActionOrder_Req,g_idThread);


	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_ToExecute_OrderAction_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_ToExecute_VerifyOrder_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_ToExecute_ForceCloseOrderInsert_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_RM_ToExecute_RiskForce_Req,g_idThread);

	//LONGLONG lFfreq = CTools_Win32::MyGetCpuTickFreq();
	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{
		
		if(Msg.message==WM_COMMAND&&Msg.wParam==WndCmd_YourPkgArrival2)
		{
			//LONGLONG n1 =  CTools_Win32::MyGetCpuTickCounter();
			//���ݰ��Ĵ���ID
			unsigned int EventParamID=(unsigned int)Msg.lParam;
			//���ݰ�����
			int PkgLen;
			//Socket���
			int hSocket;
			if(CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,NULL,&PkgLen,0,&hSocket))
			{
				
				//LONGLONG n2 =  CTools_Win32::MyGetCpuTickCounter();
				DealPakage(EventParamID,PkgLen,hSocket);
				//LONGLONG n3 =  CTools_Win32::MyGetCpuTickCounter();

				//�ͷ�EventParam
				CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
				//NGLONG n4 =  CTools_Win32::MyGetCpuTickCounter();

			/*	static double dbMax1 = 0;
				static double dbMax2 = 0;
				static double dbMax3 = 0;
				static double dbMin1 = 100000000000.00;
				static double dbMin2 = 100000000000.00;
				static double dbMin3 = 100000000000.00;
				double d1 = (n2-n1)*1000.0/lFfreq;
				if(d1 > dbMax1)
					dbMax1 = d1;
				if(d1 < dbMin1)
					dbMin1 = d1;

				double d2 = (n3-n2)*1000.0/lFfreq;
				if(d2 > dbMax2)
					dbMax2 = d2;
				if(d2 < dbMin2)
					dbMin2 = d2;

				double d3 = (n4-n3)*1000.0/lFfreq;
				if(d3 > dbMax3)
					dbMax3 = d3;
				if(d3 < dbMin3)
					dbMin3 = d3;

				static double db1 = 0;
				db1 += d1;
				static double db2 = 0;
				db2 += d2;
				static double db3 = 0;
				db3 += d3;
				static int ginputcount = 0;
				if((ginputcount++ % 1000) == 0)
					OUTPUT_LOG("input order count %d  %.3f %.3f %.3f max single %.3f %.3f %.3f  Min single %.3f %.3f %.3f " ,
					ginputcount,db1, db2, db3,dbMax1,dbMax2,dbMax3,dbMin1,dbMin2,dbMin3);*/

			}

		}
	}
	return 0;
}

// ����һ������֪ͨ�¼�
bool ParseDataChangedEvent(const Stru_NotifyEvent& dataEvt)
{
	//TRADE_EXT_LOG("Event Arrived with id %d",dataEvt.meEventType);
	switch(dataEvt.meEventType) 
	{
	case EventTypeOrderRtn:			//�µ�һ������	
		{
		//	TRADE_EXT_LOG("NewOrderReach");
			PlatformStru_OrderInfo * lpBuf = (PlatformStru_OrderInfo*)dataEvt.marrDataByte;		
			if(lpBuf)
			{
				TRADE_EXT_LOG("NewOrderReach %d %d %s %s",lpBuf->FrontID,
					lpBuf->SessionID,
					lpBuf->OrderRef,
					lpBuf->StatusMsg,
					strlen(lpBuf->OrderSysID) == 0 ? "��OrderSysID" : "��OrderSysID");
		
				BroadcastBufToClient(Cmd_TradeExcute_OrderStatus_Rsp,lpBuf->InvestorID,lpBuf,dataEvt.mnValidateLen);
			}
			break;
		}
	case EventTypeTradeRtn:         //�µ�һ���ɽ�
		{
		//	TRADE_EXT_LOG("NewTraderReach");
			PlatformStru_TradeInfo * lpBuf = (PlatformStru_TradeInfo*)dataEvt.marrDataByte;		
			if(lpBuf)
			{
				TRADE_EXT_LOG("NewTraderReach %s %s %s",lpBuf->InvestorID,lpBuf->OrderSysID,lpBuf->OrderRef);
				BroadcastBufToClient(Cmd_TradeExcute_Trader_Rsp,lpBuf->InvestorID,lpBuf,dataEvt.mnValidateLen);
			}
			break;
		}
	//case EventTypeErrOrderInsert:   //��������
	//case EventTypeRspOrderInsert:   //����ʧ��
	//	{		
	//	//	TRADE_EXT_LOG("Order Insert Failed Or Error");
	//		PlatformStru_OrderInfo * lpBuf = (PlatformStru_OrderInfo*)dataEvt.marrDataByte;		
	//		if(lpBuf)
	//		{
	//			lpBuf->OrderStatus = THOST_FTDC_OST_Canceled;
	//			lpBuf->ExStatus = PlatformStru_OrderInfo::ExSta_none;
	//			BroadcastBufToClient(Cmd_TradeExcute_OrderStatus_Rsp,lpBuf->InvestorID,lpBuf,dataEvt.mnValidateLen);
	//		}
	//		break;
	//	}	
	case EventTypeRspOrderAction:   //����ʧ��
		{
		//	TRADE_EXT_LOG("Order Action Failed Or Error");
			CThostFtdcInputOrderActionField * lpBuf = (CThostFtdcInputOrderActionField*)dataEvt.marrDataByte;	
		
			if(lpBuf)
			{	
				int nRequestID = (int)(lpBuf + sizeof(CThostFtdcInputOrderActionField) + sizeof(CThostFtdcRspInfoField));	
				BroadcastBufToClient(Cmd_TradeExcute_CancelOrder_Fail_RSP,lpBuf->InvestorID,lpBuf,dataEvt.mnValidateLen,nRequestID);
			}
			break;
		}
	case EventTypeErrOrderAction:   //��������	
		{
		//	TRADE_EXT_LOG("Order Error Failed Or Error");
			CThostFtdcInputOrderActionField * lpBuf = (CThostFtdcInputOrderActionField*)dataEvt.marrDataByte;		
			if(lpBuf)
			{			
				BroadcastBufToClient(Cmd_TradeExcute_CancelOrder_Error_RSP,lpBuf->InvestorID,lpBuf,dataEvt.mnValidateLen);
			}
			break;
		}
		break;
	//case EventTypeStartSettlement:	
	//case EventTypeEndSettlement:
	//	{
	//		//��ʼ����,�ȵ��������ִ�е�ʱ�򣬽���ִ�е��߳��Ѿ������ٵ��������ķ��ʽ������ݹ�����ڴ���룬
	//		//�������ݹ���ģ��Ϳ��԰�ȫ������ڴ濪ʼ����Ĳ���
	//		//HANDLE hEvent = (HANDLE)dataEvt.mhEvent;		
	//		//SetEvent(hEvent);
	//	}
	//	break;
	default: break;
	};
	return true;
}

//����һ��SvrTcp���͹��������ݰ�
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	//-----------------------------------------------------------------------------------
	//	����������ݰ��������֣��������ݰ�
	//-----------------------------------------------------------------------------------
	switch(PkgHead.cmdid)
	{

	case Cmd_TradeExcute_StategyInputOrder_Req:
		{
			StragegyInputOrderReqDeal(hSocket,PkgHead,pPkgData);
			break;			
		}
	case Cmd_TradeExcute_StategyActionOrder_Req:
		{
			StragegyActionOrderReqDeal(hSocket,PkgHead,pPkgData);
			break;			
		}
	case Cmd_TradeExcute_InputOrder_Req:
		{
			InputOrderReqDeal(hSocket,PkgHead,pPkgData);
			break;			
		}
	case Cmd_TradeExcute_VerifyOrder_Req:
		{
			VerifyedOrderDeal(hSocket,PkgHead,pPkgData);
		}
	case Cmd_TradeExcute_CancelOrder_Req:
		{
			ActionOrderReqDeal(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeExcute_QryApprovingOrder_Req:
		{
			QryApprovingOrderInfo(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_RM_ToExecute_OrderAction_Req:
		{
			TRADE_EXT_LOG("Rec slave server order action request");		
			SRiskInputOrderAction * lpInputOrderAction = (SRiskInputOrderAction*)pPkgData;
			if(PkgHead.len == sizeof(SRiskInputOrderAction) && lpInputOrderAction )
			{
				UserInfo userInfo;
				CInterface_SvrUserOrg::getObj().GetUserInfoByAccount(lpInputOrderAction->nInputOrderAction.Thost.InvestorID, userInfo);	

				CF_ERROR lRet = CSvrTradeExcuteImpl::getObj().RiskForceActionOrder(lpInputOrderAction->nInputOrderAction,
					userInfo.nUserID,lpInputOrderAction->nRequestID,lpInputOrderAction->nsActionReson);
				
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,
					Cmd_RM_ToExecute_OrderAction_Rsp,
					lpInputOrderAction,
					sizeof(SRiskInputOrderAction),
					PkgHead.seq,
					0,
					lRet,
					PkgHead.userdata2,
					PkgHead.userdata3,
					PkgHead.userdata4);
			}
			break;
		}
	case Cmd_RM_ToExecute_VerifyOrder_Req:
		{
			TRADE_EXT_LOG("Rec slave server verify order request");	
			SVerisyOrder * lpVerifyOrder = (SVerisyOrder*)pPkgData;
			if(PkgHead.len == sizeof(SVerisyOrder) && lpVerifyOrder )
			{
			
				CF_ERROR lRet = CSvrTradeExcuteImpl::getObj().SetVerifyOrder(lpVerifyOrder->orderKey,lpVerifyOrder->nResult);				

				CInterface_SvrTcp::getObj().SendPkgData(hSocket,
					Cmd_RM_ToExecute_VerifyOrder_Rsp,
					lpVerifyOrder,
					sizeof(SVerisyOrder),
					PkgHead.seq,
					0,
					lRet,
					PkgHead.userdata2,
					PkgHead.userdata3,
					PkgHead.userdata4);
			}
		
			break;
		}
	case Cmd_RM_ToExecute_RiskForce_Req:
		{
			TRADE_EXT_LOG("Rec slave server risk control request");	
			SRiskControl2Execute * lpRiskControl = (SRiskControl2Execute*)pPkgData;
			
			std::multimap<RiskWarningType, SRiskControl> lmapControl;
			RiskWarningType lCurrentWaringType = WarningType_None;	
			SRiskControl lCurrentRiskControl;
			std::string lsInvestorName ;
			for(int i = 0; i < PkgHead.len/sizeof(SRiskControl2Execute) && lpRiskControl; i++,lpRiskControl++)
			{
				lsInvestorName = lpRiskControl->Investor;
//////////////////////////////////////////////////////////////////////////
				if(lpRiskControl)
				{
					if(PkgHead.len/sizeof(SRiskControl2Execute) == 1)
					{
						if(lpRiskControl->nResponseType == WarningType_None)
						{//��������ǻָ��˻�����Ȩ��
							CSvrTradeExcuteImpl::getObj().RiskControl(lsInvestorName,lmapControl);			
							return;
						}
					}

				}
//////////////////////////////////////////////////////////////////////////
			
				if(lpRiskControl->nResponseType != lCurrentWaringType)
				{
					//������һ����map
					/*if(lCurrentWaringType != WarningType_None)
						lmapControl[lCurrentWaringType] = lCurrentRiskControl;*/

					lCurrentWaringType = lpRiskControl->nResponseType;
					lCurrentRiskControl.nEventID = lpRiskControl->nEventID;
					lCurrentRiskControl.nResponseType = lpRiskControl->nResponseType;
					lCurrentRiskControl.nRiskIndicatorID = lpRiskControl->nRiskIndicatorID;
					lCurrentRiskControl.nRiskLevelID = lpRiskControl->nRiskLevelID;
					lCurrentRiskControl.setInstrument.clear();
					lCurrentRiskControl.setInstrument.insert(lpRiskControl->Instrument);
					memcpy(&lCurrentRiskControl.PostionDetail,&lpRiskControl->PostionDetail,sizeof(PositionDetailKey));

					lmapControl.insert(std::make_pair(lCurrentWaringType, lCurrentRiskControl));
					//lmapControl[lCurrentWaringType] = lCurrentRiskControl;

				}
				else
				{
					lCurrentWaringType = lpRiskControl->nResponseType;
					lCurrentRiskControl.nEventID = lpRiskControl->nEventID;
					lCurrentRiskControl.nResponseType = lpRiskControl->nResponseType;
					lCurrentRiskControl.nRiskIndicatorID = lpRiskControl->nRiskIndicatorID;
					lCurrentRiskControl.nRiskLevelID = lpRiskControl->nRiskLevelID;
					lCurrentRiskControl.setInstrument.clear();
					lCurrentRiskControl.setInstrument.insert(lpRiskControl->Instrument);
					memcpy(&lCurrentRiskControl.PostionDetail,&lpRiskControl->PostionDetail,sizeof(PositionDetailKey));

					lmapControl.insert(std::make_pair(lCurrentWaringType, lCurrentRiskControl));
				//	lCurrentRiskControl.setInstrument.insert(lpRiskControl->Instrument);
				}				
				
			}
			if(lmapControl.size() != 0)
				CSvrTradeExcuteImpl::getObj().RiskControl(lsInvestorName,lmapControl);				

			break;
		}
	case Cmd_RM_ToExecute_ForceCloseOrderInsert_Req:
		{
			TRADE_EXT_LOG("Rec slave server force close order request");	
			SManualForceClose * lpManualForceClose = (SManualForceClose*)pPkgData;
			if(PkgHead.len == sizeof(SManualForceClose) && lpManualForceClose )
			{
				SManualForceClose sMClose = *lpManualForceClose;
				InputOrderKey lKey;
				lKey.nFrontID		= lpManualForceClose->nFrontID;//���ֵ�պ��Ƿ��ԱID
				lKey.nSessionID		= lpManualForceClose->nSessionID;
				strcpy(lKey.szOrderRef, lpManualForceClose->szOrderRef);
				std::string  nsForceReason = lpManualForceClose->szReason;


				UserInfo userInfo;
				CInterface_SvrUserOrg::getObj().GetUserInfoByAccount(lpManualForceClose->nInputOrder.InvestorID, userInfo);	
				int nTradeInvestorID = userInfo.nUserID;

				//��UserForceClose = 1�Լ�ForceCloseReason = THOST_FTDC_FCC_ForceClose����ǿƽ�������ɳ���
				lpManualForceClose->nInputOrder.UserForceClose = 1;
				lpManualForceClose->nInputOrder.ForceCloseReason = THOST_FTDC_FCC_ForceClose;
				
				std::string strInvestor = lpManualForceClose->nInputOrder.InvestorID;
				CF_ERROR lRet = CSvrTradeExcuteImpl::getObj().RiskForceCloseOrder(lpManualForceClose->nInputOrder,
					lKey,
					nTradeInvestorID, 
					nsForceReason);				

				strcpy(lpManualForceClose->nInputOrder.InvestorID, strInvestor.c_str());//�ı�ؽ���Ա
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,
					Cmd_RM_ToExecute_ForceCloseOrderInsert_Rsp,
					&sMClose,
					sizeof(SManualForceClose),
					PkgHead.seq,
					0,
					lRet,
					PkgHead.userdata2,
					PkgHead.userdata3,
					PkgHead.userdata4);
			}

			break;
		}
	default:
		break;
	}
}