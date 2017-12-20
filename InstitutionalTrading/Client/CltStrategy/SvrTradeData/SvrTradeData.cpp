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
#include "Real_SvrTradeData.h"
#include "CmdDealFun.h"

#define WRITELOGID_SvrTradeData


/*

//�̲߳���
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;
DWORD ThreadWorker(void *arg);

//�̲߳���
HANDLE				g_hThreadSubscribe=NULL;
DWORD				g_idThreadSubcribe=0;
DWORD ThreadWorkerSubscribe(void *arg);

//ģ���ʼ��������Ĳ������û���ί�н����˺ţ����͹�˾����ʼ�ʽ�Redis����ָ�룩
SVRTRADEDATA_API void InitFunc(string szUser,string szBrokerCode,string szTradeAccount,double dFund )
{
    //���������߳�
    g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);
    g_hThreadSubscribe=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorkerSubscribe,0,0,&g_idThreadSubcribe);

    CReal_SvrTradeData::getObj();

    CReal_SvrTradeData::getObj().TradeDataInit(szUser,szBrokerCode,szTradeAccount,false);

    //������
    std::string szTradingDay;
    CReal_SvrTradeData::getObj().SetCurrentTradingDay(szTradingDay);

    //�˻�������CTP��״̬
    CReal_SvrTradeData::getObj().SetAccountStatus(szUser,szBrokerCode,ACCOUNT_STATUS_Connected);

    //�ʽ�
    CReal_SvrTradeData::getObj().SetUserInitFund(dFund);

    //��Լ
    PlatformStru_InstrumentInfo data;
    CReal_SvrTradeData::getObj().SetInstrumentInfo(data);

    //��֤��
    PlatformStru_InstrumentCommissionRate sCommissionRate;
    CReal_SvrTradeData::getObj().SetCommissionRate(sCommissionRate);

    //����
    PlatformStru_InstrumentMarginRate sMarginRate;
    CReal_SvrTradeData::getObj().SetMarginRate(sMarginRate);
}

//ģ��������ͷ���Դ���رչ����߳�
SVRTRADEDATA_API void ReleaseFunc(void)
{

    //����WM_QUIT֪ͨ�߳̽���
    PostThreadMessage(g_idThread,WM_QUIT,0,0);

    //�ȴ��߳��˳�
    DWORD ExitCode;
    if(g_hThread)
    {
        WaitForSingleObject((HANDLE)g_hThread,8000);
        if(GetExitCodeThread((HANDLE)g_hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
            TerminateThread((HANDLE)g_hThread,0);
        CloseHandle(g_hThread);
        g_hThread=NULL;
        g_idThread=0;
    }

    if(g_hThreadSubscribe)
    {
        WaitForSingleObject(g_hThreadSubscribe, 8000);
        if(GetExitCodeThread((HANDLE)g_hThreadSubscribe,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
            TerminateThread((HANDLE)g_hThreadSubscribe,0);
        CloseHandle(g_hThreadSubscribe);
        g_hThreadSubscribe=NULL;
        g_idThreadSubcribe=0;
    }    
}

//�����߳�
DWORD ThreadWorker(void *arg)
{
    MSG Msg;
    while(GetMessage(&Msg,NULL,0,0))
    {
        if(Msg.message == TMESSAGE_START_DEAL_CACHE_CHANNEL_COMMAND)
        {
            TRADE_LOG("Deal channel command");

            //����Reidsͨ�����µ��ͳ���
            CReal_SvrTradeData::getObj().DealCommand();
        }
    }

    return 0;
}
DWORD ThreadWorkerSubscribe(void *arg)
{
    MSG Msg;
    while(GetMessage(&Msg,NULL,0,0))
    {		
        if(Msg.message == TMESSAGE_START_SUBCRIBE_CACHE_CHANNEL)
        {
            TRADE_LOG("��ʼ����Cache���µ��ͳ���ͨ��");
            //CReal_SvrTradeData::getObj().SubscribeCacheChannel();			
        }
    }
    return 0;
}
*/