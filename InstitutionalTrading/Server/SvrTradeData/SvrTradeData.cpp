// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#include "stdafx.h"
#include <windows.h>
#include "SvrTradeData.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "CommonDataChangeEvent.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "CommonMacro.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "..\\SvrNotifyAndAsk\Interface_SvrNotifyAndAsk.h"
#include "..\\SvrUserOrg\Interface_SvrUserOrg.h"
#include "..\\SvrLogin\Interface_SvrLogin.h"
#include "..\\SvrDBOpr\Interface_SvrDBOpr.h"
#include "Real_SvrTradeData.h"
#include "CmdDealFun.h"


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
#pragma comment(lib, "SvrNotifyAndAsk.lib")
#pragma comment(lib, "SvrUserOrg.lib")
#pragma comment(lib, "SvrDBOpr.lib")
#pragma comment(lib, "SvrBrokerInfo.lib")
#pragma comment(lib, "SvrLogin.lib")
#define WRITELOGID_SvrTradeData




//ȫ�ֻ�����
Ceasymutex			g_mutex;

const int gTimerInterval = 5; 
//�̲߳���
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;
DWORD ThreadWorker(void *arg);


//�̲߳���
HANDLE              g_SubEvent = NULL;
HANDLE				g_hThreadSubscribe=NULL;
DWORD				g_idThreadSubcribe=0;
DWORD ThreadWorkerSubscribe(void *arg);


HANDLE		g_hTimerThread = NULL;
DWORD		g_dwTimerThreadID = 0;
BOOL		g_bExit = FALSE;


DWORD __stdcall Proc_Timer(LPVOID lpParameter);

//����һ�����յ������ݰ�
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//// ����һ������֪ͨ�¼�
bool ParseDataChangedEvent(const Stru_NotifyEvent& dataEvt);

//ģ���ʼ�������������̲߳���SvrTcp���ĸ���Ȥ�����ݰ�
SVRTRADEDATA_API void InitFunc(void)
{

	//���������߳�
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);

	if(CReal_SvrTradeData::getObj().NeedCache())
    {
        g_SubEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
		CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorkerSubscribe,0,0,&g_idThreadSubcribe);

        WaitForSingleObject(g_SubEvent, INFINITE);
        CReal_SvrTradeData::getObj().ConnectRedisServer();
    }
}

//ģ��������ͷ���Դ���رչ����߳�
SVRTRADEDATA_API void ReleaseFunc(void)
{
    if(CReal_SvrTradeData::getObj().NeedCache())
    {
        if(g_SubEvent)
            CloseHandle(g_SubEvent);

        CReal_SvrTradeData::getObj().DisConnectRedisServer();
        TRADE_LOG("���������˳����رղ��Է�������Redis�ͻ���\n");
    }

	if(g_hThread)
	{
		//�˶����ݰ�
		CInterface_SvrTcp::getObj().UnsubscribeAllPkg(g_idThread);
		//CInterface_SvrNotifyAndAsk::getObj().UnRegistryAllDataCanOpAskInterface((CInterface_SvrCommon*)CReal_SvrTradeData::getObj());

		//����WM_QUIT֪ͨ�߳̽���
		PostThreadMessage(g_idThread,WM_QUIT,0,0);

		g_bExit = TRUE;

		//�ȴ��߳��˳�
		DWORD ExitCode;

		WaitForSingleObject(g_hTimerThread, 8000);
		if(GetExitCodeThread((HANDLE)g_hTimerThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)g_hTimerThread,0);
		CloseHandle(g_hTimerThread);
		g_hTimerThread=NULL;
		g_dwTimerThreadID=0;

		WaitForSingleObject((HANDLE)g_hThread,8000);
		if(GetExitCodeThread((HANDLE)g_hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)g_hThread,0);
		CloseHandle(g_hThread);
		g_hThread=NULL;
		g_idThread=0;

	}
}

void  DealTimerWork(UINT_PTR & nCheckCTPTimer,UINT_PTR & nCheckUserTimer)
{
	//printf("hello!\n");
	TRADE_LOG("Timer Working");


	
	time_t lTime;
	struct tm * lTimeinfo = NULL; 
	time(&lTime);
	lTimeinfo = localtime(&lTime);


	sReinitTime lsReinitTime;
	CReal_SvrTradeData::getObj().GetReinitTime(lsReinitTime);
	struct tm  lNigthTimeinfo ;
	struct tm  lDayTimeinfo ; 	
	//lNigthTimeinfo = localtime(&lTime);
	//lDayTimeinfo = localtime(&lTime);
	memcpy(&lNigthTimeinfo,lTimeinfo,sizeof(tm));
	memcpy(&lDayTimeinfo,lTimeinfo,sizeof(tm));

	sscanf_s(lsReinitTime.mNightTime,"%02d:%02d:%02d",&lNigthTimeinfo.tm_hour,&lNigthTimeinfo.tm_min,&lNigthTimeinfo.tm_sec);
	sscanf_s(lsReinitTime.mDayTime,"%02d:%02d:%02d",&lDayTimeinfo.tm_hour,&lDayTimeinfo.tm_min,&lDayTimeinfo.tm_sec);

	time_t lNigthTime = mktime(&lNigthTimeinfo);
	time_t lDayTime = mktime(&lDayTimeinfo);
	//���������8:30�㣬�����ý������ݹ���ģ����ڴ�
	time_t lDifftimeNight = lTime - lNigthTime;
	time_t lDifftimeDay = lTime - lDayTime;
	if(( 0 <= lDifftimeNight && lDifftimeNight < gTimerInterval*60 )|| 
		(0 <= lDifftimeDay && lDifftimeDay < gTimerInterval*60)  )
	{
		TRADE_LOG("Timer Working Reset adll time is %d-%d-%d %d:%d",lTimeinfo->tm_year+1900,
			lTimeinfo->tm_mon+1,
			lTimeinfo->tm_mday,
			lTimeinfo->tm_hour,
			lTimeinfo->tm_min);
	
		if(nCheckCTPTimer != 0) KillTimer(NULL,nCheckCTPTimer);
		if(nCheckUserTimer != 0) KillTimer(NULL,nCheckUserTimer);
		nCheckCTPTimer = nCheckUserTimer = 0;
		CReal_SvrTradeData::getObj().TradeDataInit();
		return;
	}

	//����������Сʱ��
	if( ( lDifftimeNight >= gTimerInterval*60  && lDifftimeNight < 30*60)
		|| ( lDifftimeDay >= gTimerInterval*60  && lDifftimeDay < 30*60))
	{
		
	
		if(CReal_SvrTradeData::getObj().GetTraderDataStatus() == TradeDataStatusNoTradingDay)
		{
			TRADE_LOG("Trading Status with no tradingday need reinit %d-%d-%d %d:%d",lTimeinfo->tm_year+1900,
				lTimeinfo->tm_mon+1,
				lTimeinfo->tm_mday,
				lTimeinfo->tm_hour,
				lTimeinfo->tm_min);
			TRADE_LOG("ReInit");

			if(nCheckCTPTimer != 0) KillTimer(NULL,nCheckCTPTimer);
			if(nCheckUserTimer != 0) KillTimer(NULL,nCheckUserTimer);
			nCheckCTPTimer = nCheckUserTimer = 0;
			CReal_SvrTradeData::getObj().TradeDataInit();
		}
		return;
	}

	sSaveTime lsSaveTime;
	CReal_SvrTradeData::getObj().GetSaveTime(lsSaveTime);
	struct tm  lSaveTime1Info ;
	struct tm  lSaveTime2Info ; 	
	//lNigthTimeinfo = localtime(&lTime);
	//lDayTimeinfo = localtime(&lTime);
	memcpy(&lSaveTime1Info,lTimeinfo,sizeof(tm));
	memcpy(&lSaveTime2Info,lTimeinfo,sizeof(tm));

	sscanf_s(lsSaveTime.mSaveTime1,"%02d:%02d:%02d",&lSaveTime1Info.tm_hour,&lSaveTime1Info.tm_min,&lSaveTime1Info.tm_sec);
	sscanf_s(lsSaveTime.mSaveTime2,"%02d:%02d:%02d",&lSaveTime2Info.tm_hour,&lSaveTime2Info.tm_min,&lSaveTime2Info.tm_sec);

	time_t lSaveTime1 = mktime(&lSaveTime1Info);
	time_t lSaveTime2 = mktime(&lSaveTime2Info);
	time_t lDifftime1 = lTime - lSaveTime1;
	time_t lDifftime2 = lTime - lSaveTime2;
	//����11��30 ����3��15�ִ洢һ����������
	//if( (lTimeinfo->tm_hour == 11 && lTimeinfo->tm_min > 30 && lTimeinfo->tm_min <= 35) ||
	//	(lTimeinfo->tm_hour == 15 && lTimeinfo->tm_min > 16 && lTimeinfo->tm_min <= 21))
	if(( 0 <= lDifftime1 && lDifftime1 < gTimerInterval*60 )|| 
			(0 <= lDifftime2 && lDifftime2 < gTimerInterval*60)  )
	{
		TRADE_LOG("Timer Working SaveAllTradingData time is %d-%d-%d %d:%d",lTimeinfo->tm_year+1900,
			lTimeinfo->tm_mon+1,
			lTimeinfo->tm_mday,
			lTimeinfo->tm_hour,
			lTimeinfo->tm_min);
		CReal_SvrTradeData::getObj().SaveAllTradingData();
		
	}
	else
	{
		//�翪�̺�����̺�ÿ5���ӱ���һ������
		if( 0 <= lDifftimeNight || 0 <= lDifftimeDay)
		{
			TRADE_LOG("Timer Working TimerSaveTradingData time is %d-%d-%d %d:%d",lTimeinfo->tm_year+1900,
				lTimeinfo->tm_mon+1,
				lTimeinfo->tm_mday,
				lTimeinfo->tm_hour,
				lTimeinfo->tm_min);
			//��ʱ�洢���еı����ͳɽ��Ľ�������
			CReal_SvrTradeData::getObj().TimerSaveTradingData();
		}
	}

	
	  

	//�������̺�
	sTradingTime lsTradingTime;
	CReal_SvrTradeData::getObj().GetTradingTime(lsTradingTime);
	struct tm  lTradingTimeDayEndInfo ;	
	memcpy(&lTradingTimeDayEndInfo,lTimeinfo,sizeof(tm));	
	sscanf_s(lsTradingTime.mTradingDayEnd,"%02d:%02d:%02d",&lTradingTimeDayEndInfo.tm_hour,&lTradingTimeDayEndInfo.tm_min,&lTradingTimeDayEndInfo.tm_sec);

	time_t lTradingTimeDayEnd = mktime(&lTradingTimeDayEndInfo);	
	time_t lDifftimeTradingDayEnd = lTime - lTradingTimeDayEnd;
	if( 0 <= lDifftimeTradingDayEnd && lDifftimeTradingDayEnd < gTimerInterval*60 )
	{
		TRADE_LOG("Trading Day End withe Time  %d-%d-%d %d:%d",lTimeinfo->tm_year+1900,
			lTimeinfo->tm_mon+1,
			lTimeinfo->tm_mday,
			lTimeinfo->tm_hour,
			lTimeinfo->tm_min);
		CReal_SvrTradeData::getObj().CancelAllUnkonwnOrder();
		CReal_SvrTradeData::getObj().SaveAllTradingData();
	}
	
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
	//
	//���涩�ı��̸߳���Ȥ�����ݰ�
	if(!CInterface_SvrLogin::getObj().IsSlaver())
	{
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryInstrument_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryTraderAccount_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryTrade_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryOrder_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryPosition_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryPositionDetail_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryComPositionDetail_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryCommissionRat_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryMarginRate_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QrySettlementConfirm_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_FundInOut_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryFundInOut_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryAccountStatus_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_LoginAccount_Req,g_idThread);	
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_SettlementConfirm_Req,g_idThread);	
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryUserInitStatus_Req,g_idThread);	
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_InitUser_Req,g_idThread);	
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_ReInit_Req,g_idThread);	
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryReinitTime_Req,g_idThread);	
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_SetReinitTime_Req,g_idThread);	
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryStatus_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryTradingTime_Req,g_idThread);	
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_SetTradingTime_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryNotice_Req,g_idThread);	
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryContractBank_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryParkedOrder_Req,g_idThread);	
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryParkedOrderAction_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryDepthMarket_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_ReForceInit_Req,g_idThread);
	
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_SlaverQryTradingDay_Req,g_idThread);	
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_SlaverQryOrderAndTraderBiggerFTID_Req,g_idThread);


		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_Login,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));
		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_UserAndTradeAccount,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));
		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_OrgAndStrategy,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));
		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_TraderAndProduct,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));
		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_UserAndOrg,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));
		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_BrokerAndServerGroup,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));
		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_BrokerAndTradeAccount,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));
		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_BrokerAndServerGroupAddress,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));
		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_Orgnazation,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));
		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_FinacialProduct,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));
		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_TradeStrategy,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));
		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_User,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));
		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_Account,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));
		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_Broker,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));
		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_ServerGroup,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));
		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_ServerAddress,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));
		CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_UserLoginCheck,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradeData::getObj()));

        CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_MasterQuot_Req,g_idThread);


	}
	else
	{

		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryInstrument_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_SlaverQryTradingDay_Rsp,g_idThread);	
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_SlaverQryOrderAndTraderBiggerFTID_Rsp,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_MasterOrder_Push,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_MasterTrader_Push,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_MasterQryCommission_Push,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_MasterQryFund_Push,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_MasterQryMargin_Push,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_MasterQryPosition_Push,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_MasterQryPositionDetail_Push,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_MasterQryCTPOrder_Push,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_MasterQryCTPTrader_Push,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_MasterUserInit_Push,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_FundInOut_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_MasterQuot_Push,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_MasterErrOrderInsert_Push,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_MasterErrOrderAction_Push,g_idThread);
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeUserLogin, g_idThread);
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeUserDiscon, g_idThread);
		CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeSlaverLoginedMaster, g_idThread);

		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryCommissionRat_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_TradeDataManager_QryMarginRate_Req,g_idThread);

//         CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeServerQuit, g_idThread);
//         CInterface_SvrTcp::getObj().SubscribePkg(CMDID_TcpClientDisconn,g_idThread); //�ӷ����������������Ͽ�
	}

	//����һ��ÿ5���Ӵ���һ�εĶ�ʱ��
	UINT_PTR lSaveTime = 0; 	
	UINT_PTR lCheckCTPTime = 0;
	UINT_PTR lCheckTime = 0;
	UINT_PTR lCheckSYNCTime = 0;
    int      lCheckSYNCOverTime =0;

	if(!CInterface_SvrLogin::getObj().IsSlaver())
		lSaveTime = SetTimer(NULL,NULL,gTimerInterval*60*1000,(TIMERPROC)(NULL)); 

	if(CInterface_SvrDBOpr::getObj().IsConnected() == false)
	{
		PUSH_LOG_ERROR(FirstLevelError,false,true,"���ݿ����Ӵ������������ļ������ݿ�������Ƿ�����");
		TRADE_LOG("���ݿ����Ӵ������������ļ������ݿ�������Ƿ�����");
	}

	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{
		//if(Msg.message == TMESSAGE_TRADEINIT_AFTER_GET_TRADINGDAY)
		//{
		//	std::string lsTradingDay;
		//	CReal_SvrTradeData::getObj().GetCurrentTradingDay(lsTradingDay);
		//	TRADE_LOG("Trading Init after get Trading Day %s",lsTradingDay.c_str());

		//	//ִ�л�ȡ�����յĳ�ʼ������
		//	CReal_SvrTradeData::getObj().TradeDataInitAfterGetTradingDay();


		//}	
//         if(Msg.message == TMESSAGE_START_SUBCRIBE_CACHE_CHANNEL)
//         {
//             PUSH_LOG_ERROR(Logger,false,true,"��ʼ����Cache���µ��ͳ���ͨ��");
//             CReal_SvrTradeData::getObj().SubscribeCacheChannel();			
//         }
        if(Msg.message == TMESSAGE_START_DEAL_CACHE_CHANNEL_COMMAND)
        {
            TRADE_LOG("Deal channel command");
            //��ʱ�洢���еı����ͳɽ��Ľ�������
            CReal_SvrTradeData::getObj().DealCommand();
        }
		if(Msg.message == TMESSAGE_TRADEINIT_CHECK_USER_CTP_STATUS)
		{
			if(lCheckCTPTime != 0) KillTimer(NULL,lCheckCTPTime);
			lCheckCTPTime = SetTimer(NULL,NULL,1000,(TIMERPROC)(NULL));
		}
		if(Msg.message == TMESSAGE_TRADEINIT_CHECK_USER_STATUS)
		{
			if(lCheckTime != 0) KillTimer(NULL,lCheckTime);
			lCheckTime = SetTimer(NULL,NULL,1000,(TIMERPROC)(NULL));
		}
		if(Msg.message == TMESSAGE_TRADEINIT_CHECK_USER_ACCOUNT_SYNC)
		{
			if(lCheckSYNCTime != 0) KillTimer(NULL,lCheckSYNCTime);

			lCheckSYNCOverTime = 0;
			lCheckSYNCTime = SetTimer(NULL,NULL,1000,(TIMERPROC)(NULL));
		}
		if(Msg.message == TMESSAGE_TRADING_BUFFER_STORE)
		{
			TRADE_LOG("one of six Buffer is full,we need save to db");
			//��ʱ�洢���еı����ͳɽ��Ľ�������
			CReal_SvrTradeData::getObj().TimerSaveTradingData(false);

		}
		if(Msg.message == TMESSAGE_TRADING_END_USER_CHECK)
		{
			TRADE_LOG("We need to stop check");
			if(lCheckCTPTime != 0) 
				KillTimer(NULL,lCheckCTPTime);
			if(lCheckTime != 0)
				KillTimer(NULL,lCheckTime);
			lCheckTime = 0;
			lCheckCTPTime = 0;
		}
		if(Msg.message == WM_TIMER )
		{
			if(Msg.wParam == lSaveTime)
				DealTimerWork(lCheckCTPTime,lCheckTime);	
			else if(Msg.wParam == lCheckCTPTime)
			{
				//��ʱ���ÿ���û��Ƿ������˶���
				//�ж��û���ctp status Ϊ���Ӳ��ϻ��¼ʧ��
				int lnLoginCount = 0;
				bool lbFinished = CReal_SvrTradeData::getObj().CheckAllUserFinishedCTP(lnLoginCount);
				if(lbFinished)
				{
					std::string lsCurrentTradingDay;
					CReal_SvrTradeData::getObj().GetCurrentTradingDay(lsCurrentTradingDay);
					if( !lsCurrentTradingDay.empty())
					{
						PUSH_LOG_ERROR(Logger,false,true,"��ȡ��������%s",lsCurrentTradingDay.c_str());
						TRADE_LOG("Get Rigth Current Trading Day is %s",lsCurrentTradingDay.c_str());
						CReal_SvrTradeData::getObj().TradeDataInitAfterGetTradingDay();
					}
					else
					{
						TRADE_LOG("No Trading Day,we need check inittradingday with no settlement ");
						PUSH_LOG_ERROR(Logger,false,true,"û�л�ȡ��������");
						CReal_SvrTradeData::getObj().TradeDataInitAfterNoTradingDay();
					}
					KillTimer(NULL,lCheckCTPTime);
					lCheckCTPTime = 0;
				}
			}
			else if(Msg.wParam == lCheckTime)
			{
				//��ʱ���ÿ���û��Ƿ������˶���
				//�ж��û���ctp status Ϊ���Ӳ��ϻ��¼ʧ��
				bool lbFinished = CReal_SvrTradeData::getObj().CheckAllUserFinishedInit();
				if(lbFinished)
				{

					if(CReal_SvrTradeData::getObj().GetTraderDataStatus() != TradeDataStatusLastDaySettlemtNotOK)
					{
						TRADE_LOG("Set TradeStatus to Trading status")
						Sleep(4000);
						PUSH_LOG_ERROR(Logger,false,true,"�����û���ʼ����ɣ����÷��������뽻��״̬");
						PUSH_LOG_ERROR(Logger,false,true,"-----------------��ʼ�����-----------------");						

						//������������
						TRADE_LOG("Init Finished we should save all data");
						CReal_SvrTradeData::getObj().TimerSaveTradingData(true);

						CReal_SvrTradeData::getObj().SaveAllTradingData(true);

						//���ͳ�ʼ������¼�
						CReal_SvrTradeData::getObj().SetTraderDataStatus(TradeDataStatusTrading);
						Stru_NotifyEvent lDataEventInit(EventTypeInitFinish,0);
						CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEventInit);
					
					}
					else
					{
						TRADE_LOG("Is TradeDataStatusLastDaySettlemtNotOK status; not set to Trading status");
					}
					KillTimer(NULL,lCheckTime);
					lCheckTime = 0;
				}
			}
			else if(lCheckSYNCTime == Msg.wParam)
			{
				//�ж��û������˻��Ƿ�ͬ����
				bool lbFinished = CReal_SvrTradeData::getObj().CheckAllUserAccountSyn();
				if(lbFinished)
				{				

					TRADE_LOG("Set TradeStatus to Trading status")
					Sleep(4000);
					PUSH_LOG_ERROR(Logger,false,true,"�ӷ����������û���ͬ��������ɣ���ʼ�����");
					PUSH_LOG_ERROR(Logger,false,true,"-----------------��ʼ�����-----------------");						

				
					//���ͳ�ʼ������¼�
					//CReal_SvrTradeData::getObj().SetTraderDataStatus(TradeDataStatusTrading);
					Stru_NotifyEvent lDataEventInit(EventTypeInitFinish,0);
					CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEventInit);

                    //�������������������Ϣ���Է���¼����ն˺�ֲ�ӯ��Ϊ0
                    CInterface_SvrTcp::getObj().SendPkgData(CReal_SvrTradeData::getObj().GetMasterSocket(),
                                                            Cmd_TradeDataManager_MasterQuot_Req,
                                                            NULL,
                                                            0,
                                                            0,
                                                            0,
                                                            0,
                                                            0,
                                                            CF_ERROR_SUCCESS,0);


					KillTimer(NULL,lCheckSYNCTime);
					lCheckSYNCTime = 0;
				}else
				{
					lCheckSYNCOverTime++;
					//5min����ʾ�û���ʼ��ʧ��
					if(lCheckSYNCOverTime == 5*60)
					{
						TRADE_LOG("slave server init failed,please retry again!");
						Sleep(4000);
						PUSH_LOG_ERROR(Logger,false,true,"�ӷ����������û�ͬ�������⣬��ʼ��ʧ��");
						PUSH_LOG_ERROR(Logger,false,true,"-----------------��ʼ��ʧ��-----------------");	

						KillTimer(NULL,lCheckSYNCTime);
						lCheckSYNCTime = 0;
					}
				}
			}
		}
		if(Msg.message==WM_COMMAND&&Msg.wParam==WndCmd_NotifyEventArrival)
		{
			unsigned int EventParamID=(unsigned int)Msg.lParam;
			//���ݰ�����
			Stru_NotifyEvent ls;
			int nLen = sizeof(ls);
			if(CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,
				NULL,&ls,&nLen,nLen,NULL))
			{			
					//�������ݰ��������������ݰ�
				ParseDataChangedEvent(ls);			
				//�ͷ�EventParam
				CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
			}

		}
		else if(Msg.message==WM_COMMAND&&Msg.wParam==WndCmd_YourPkgArrival2)
		{
			//���ݰ��Ĵ���ID
			unsigned int EventParamID=(unsigned int)Msg.lParam;
			//���ݰ�����
			int PkgLen;
			//Socket���
			int hSocket;
			if(CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,
				NULL,NULL,&PkgLen,0,NULL))
			{
				////������ʱ�ռ䲢��ȡ���ݰ�
				//AllocTmpStackBuf(TmpPkg,PkgLen);
				//if(TmpPkg.m_pbuf&&
				//	CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,
				//	NULL,TmpPkg.m_pbuf,&PkgLen,PkgLen,&hSocket)&&
				//	Stru_UniPkgHead::IsValidPkg(TmpPkg.m_pbuf,PkgLen))
				//{
				//	Stru_UniPkgHead& PkgHead=*((Stru_UniPkgHead*)TmpPkg.m_pbuf);
				//	void* pPkgData=(char*)TmpPkg.m_pbuf+sizeof(Stru_UniPkgHead);

				//	//�������ݰ��������������ݰ�
				//	ProcessOneUniPkg_InThread(hSocket,PkgHead,pPkgData);
				//}
				DealPakage(EventParamID,PkgLen,hSocket);

				//�ͷ�EventParam
				CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
			}
		}

	}

	KillTimer(NULL,lSaveTime);
	return 0;
}

// ����һ������֪ͨ�¼�
bool ParseDataChangedEvent(const Stru_NotifyEvent& dataEvt)
{
	switch(dataEvt.meEventType) 
	{
	case EventTypeUserAdd:			//�¼�һ���û�
		//dataEvt.meDataChangeType;      //���ݸı�����
		//dataEvt.mllDataID;             //����ID
		break;
	//case EventTypeUserLogin:
	//	{
	//		int nUserID = dataEvt.mnDataID;
	//		//����û��ĵ�½�ͻ���>0
	//		sUserOnlineInfo lsInfo;
	//		CInterface_SvrLogin::getObj().GetOnlineUserStatus(nUserID,lsInfo);
	//		if(lsInfo.mUserOnlineCount > 0 && nUserID != SLAVE_USER_ID)
	//		{
	//			UserInfo lUserInfo;
	//			CInterface_SvrUserOrg::getObj().GetUserByID(nUserID,lUserInfo);
	//			//CReal_SvrTradeData::getObj().PushTraderUseroffLine(lUserInfo.szAccount,false);
	//		}
	//		break;;
	//	}
	//case EventTypeUserDiscon:
	//	{
	//		int nUserID = dataEvt.mnDataID;
	//		//����û��ĵ�½�ͻ���Ϊ0��������֪ͨ����
	//		sUserOnlineInfo lsInfo;
	//		CInterface_SvrLogin::getObj().GetOnlineUserStatus(nUserID,lsInfo);
	//		if(lsInfo.mUserOnlineCount == 0 && nUserID != SLAVE_USER_ID)
	//		{
	//			UserInfo lUserInfo;
	//			CInterface_SvrUserOrg::getObj().GetUserByID(nUserID,lUserInfo);
	//			//CReal_SvrTradeData::getObj().PushTraderUseroffLine(lUserInfo.szAccount,true);
	//		}
	//		break;;
	//	}
	case EventTypeSlaverLoginedMaster:
		{
			//��סMaster Socket
			//������������ȡ�����գ�����״̬
			//��ȡ�ճ�������
			//��������ץȡ�����û������ݱ������ɽ�����
			int lSocket = dataEvt.mnDataID;
			CReal_SvrTradeData::getObj().SetMasterSocket(lSocket);

			TRADE_LOG("Slaver Init... ");
			if(false == CReal_SvrTradeData::getObj().SlaverInit())
				break;

			TRADE_LOG("Send query trading dey to master ");
			CInterface_SvrTcp::getObj().SendPkgData(lSocket,
				Cmd_TradeDataManager_SlaverQryTradingDay_Req,
				NULL,
				0,
				0,
				0,
				0,
				0,
				0,0);
			break;
		}
	
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
	case Cmd_TradeDataManager_QryInstrument_Req:
		{			
			QryInstrumentDeal(hSocket,PkgHead,pPkgData);			
			break;
		}
	case Cmd_TradeDataManager_QryCommissionRat_Req:
		{			
			QryCommissionRateDeal(hSocket,PkgHead,pPkgData);			
			break;
		}
	case Cmd_TradeDataManager_QryMarginRate_Req:
		{			
			QryMarginRateDeal(hSocket,PkgHead,pPkgData);			
			break;
		}
	case Cmd_TradeDataManager_QryPosition_Req:
		{			
			QryUserPositionDeal(hSocket,PkgHead,pPkgData);			
			break;
		}
	case Cmd_TradeDataManager_QryTraderAccount_Req:
		{			
			QryUserAccountInfoDeal(hSocket,PkgHead,pPkgData);			
			break;
		}
	case Cmd_TradeDataManager_QryPositionDetail_Req:
		{			
			QryUserPositionDetailDeal(hSocket,PkgHead,pPkgData);			
			break;
		}
	case Cmd_TradeDataManager_QryComPositionDetail_Req:
		{			
			QryUserComPositionDetailDeal(hSocket,PkgHead,pPkgData);			
			break;
		}
	case Cmd_TradeDataManager_QryOrder_Req:
		{			
			QryUserOrderDeal(hSocket,PkgHead,pPkgData);			
			break;
		}
	case Cmd_TradeDataManager_QryTrade_Req:
		{			
			QryUserTraderDeal(hSocket,PkgHead,pPkgData);			
			break;
		}
	case Cmd_TradeDataManager_FundInOut_Req:
		{
			ReqFundInOut(hSocket,PkgHead,pPkgData);			
			break;
		}
	case Cmd_TradeDataManager_QryFundInOut_Req:
		{
			ReqQryFundInOut(hSocket,PkgHead,pPkgData);			
			break;
		}
	case Cmd_TradeDataManager_QrySettlementConfirm_Req:
		{
			ReqQrySettlementConfirm(hSocket,PkgHead,pPkgData);	
			break;
		}
	case Cmd_TradeDataManager_SettlementConfirm_Req:
		{
			ReqSettlementConfirm(hSocket,PkgHead,pPkgData);	
			break;
		}
	case Cmd_TradeDataManager_QryAccountStatus_Req:
		{
			ReqQryTraderAccountStatus(hSocket,PkgHead,pPkgData);	
			break;
		}
	case Cmd_TradeDataManager_LoginAccount_Req:
		{
			ReqQryTraderAccountInit(hSocket,PkgHead,pPkgData);	
			break;
		}
	case Cmd_TradeDataManager_QryUserInitStatus_Req:
		{
			ReqQryUserInitStatus(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_InitUser_Req:
		{
			ReqInitNewUser(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_ReInit_Req:
		{
			ReInitModule(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_QryReinitTime_Req:
		{
			QryReinitTime(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_SetReinitTime_Req:
		{
			SetReinitTime(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_QryStatus_Req:
		{
			QryCurrentStatus(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_QryTradingTime_Req:
		{
			QryTradingTime(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_SetTradingTime_Req:
		{
			SetTradingTime(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_QryNotice_Req:
		{
			QryNotice(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_QryContractBank_Req:
		{
			QryContractBank(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_QryParkedOrder_Req:
		{
			QryParkedOrder(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_QryParkedOrderAction_Req:
		{
			QryParkedOrderAction(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_QryDepthMarket_Req:
		{
			QryDepthMarket(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_ReForceInit_Req:
		{
			ReForceInitModule(hSocket,PkgHead,pPkgData);
			break;
		}
////////////////////////////////////////////////////////////////////////////////////////////////
//������
	case Cmd_TradeDataManager_SlaverQryTradingDay_Req:
		{
			SlaverQryTradingDay(hSocket,PkgHead,pPkgData);
			break;;
		}
	
	case Cmd_TradeDataManager_SlaverQryOrderAndTraderBiggerFTID_Req:
		{
			//�ѳɽ��ͱ���������͹�ȥ
			SlaverQrySyncReq(hSocket,PkgHead,pPkgData);
			break;
		}
    case Cmd_TradeDataManager_MasterQuot_Req:
        {
            SlaverReqMasterQuotSend(hSocket,PkgHead,pPkgData);
            break;
        }

////////////////////////////////////////////////////////////////////////////////////////////////
//�ӷ���
	case Cmd_TradeDataManager_SlaverQryTradingDay_Rsp:
		{
			SlaverQryTradingDayRsp(hSocket,PkgHead,pPkgData);
			break;;
		}

	case Cmd_TradeDataManager_MasterTrader_Push:
		{
			SlaverRecMasterTraderPush(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_MasterOrder_Push:
		{
			SlaverRecMasterOrderPush(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_MasterQuot_Push:
		{
			SlaverRecMasterQuotPush(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_SlaverQryOrderAndTraderBiggerFTID_Rsp:
		{
			//��ѯ1���ӵķ���
			SlaverQrySyncRsp(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_MasterCTPTrader_Push:
		{
			SlaverRecMasterCTPTraderPush(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_MasterCTPOrder_Push:
		{
			SlaverRecMasterCTPOrderPush(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_MasterQryCommission_Push:
		{		
			SlaverRecMasterQryCommissionPush(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_MasterQryMargin_Push	:	  
		{			
			SlaverRecMasterQryMarginPush(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_MasterQryFund_Push:
		{
			SlaverRecMasterQryFundPush(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_MasterQryPosition_Push	:
		{
			SlaverRecMasterQryPositionPush(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_MasterQryPositionDetail_Push	:
		{
			SlaverRecMasterQryPositionDetailPush(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_MasterQryCTPOrder_Push	:
		{
			SlaverRecMasterQryOrderPush(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_MasterQryCTPTrader_Push	:
		{
			SlaverRecMasterQryTraderPush(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_MasterUserInit_Push		:
		{
			SlaverRecUserInitPush(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_MasterErrOrderInsert_Push:
		{
			SlaverRecMasterErrOrderInsertPush(hSocket,PkgHead,pPkgData);
			break;
		}
	case Cmd_TradeDataManager_MasterErrOrderAction_Push:
		{
			SlaverRecMasterErrOrderActionPush(hSocket,PkgHead,pPkgData);
			break;
		}

	default: break;

	}
}

//�����߳�
DWORD ThreadWorkerSubscribe(void *arg)
{
   SetEvent(g_SubEvent); 

	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{		
		if(Msg.message == TMESSAGE_START_SUBCRIBE_CACHE_CHANNEL)
		{
			PUSH_LOG_ERROR(Logger,false,true,"��ʼ����Cache���µ��ͳ���ͨ��");
			CReal_SvrTradeData::getObj().SubscribeCacheChannel();			
		}
	}
	return 0;
}