// SvrTradeFront.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
//#include "stdafx.h"
#include <math.h>
#include <windows.h>
#include <map>
#include "SvrTradeFront.h"
#include "tmpstackbuf.h"
#include "FTTraderApi.h"
#include "LinkEntity.h"


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
#pragma comment(lib, "thosttraderapi.lib")


Ceasymutex					g_mutex;
map<int,CFTTraderApi*>		g_mapSocket2API;
set<string>					g_setTradeFrontAdds;
set<string>					g_setQuotFrontAdds;

CDataPackageForwarding		g_dataPackageForwarding;

CDataPackageForwarding&		_GetForwarding() {
	return g_dataPackageForwarding;
}

LONG	g_nCurrSeqID = 1;

int _GetNextSeq() {
	return InterlockedExchangeAdd(&g_nCurrSeqID, 1);
}

DWORD dwTlsIndex = TlsAlloc();

void _SetThreadServData(CDataPackageForwarding::Stru_ThreadData& tdata)
{
	LPVOID lpvData = TlsGetValue(dwTlsIndex); 
	if(lpvData==NULL) {
		lpvData = (LPVOID)LocalAlloc(LPTR, sizeof(CDataPackageForwarding::Stru_ThreadData)); 
		TlsSetValue(dwTlsIndex, lpvData);
	}
	if(lpvData!=NULL) {
		*(CDataPackageForwarding::Stru_ThreadData*)lpvData = tdata;
	}
}

bool _GetThreadServData(CDataPackageForwarding::Stru_ThreadData& tdata)
{
	LPVOID lpvData = TlsGetValue(dwTlsIndex); 
	if(lpvData!=NULL) {
		tdata = *(CDataPackageForwarding::Stru_ThreadData*)lpvData;
		return true;
	}
	else
		return false;
}

//�̲߳���
HANDLE				g_hThread=NULL;
HANDLE				g_hSSLAcceptThread=NULL;
HANDLE				g_hSSLCloseThread=NULL;
DWORD				g_idThread=0;
UINT				g_nSSLCloseMsgID = ::RegisterWindowMessage("g_nSSLCloseMsgID");
DWORD ThreadWorker(void *arg);
DWORD WINAPI SSLAcceptProc(LPVOID pParameter);
DWORD WINAPI SSLCloseProc(LPVOID pParameter);
//����һ�����յ������ݰ�
//void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);
void ProcessOneUniPkg_InThread(int hSocket,const void* pPackage);


//ģ���ʼ�������������̲߳���SvrTcp���ĸ���Ȥ�����ݰ�
SVRTRADEFRONT_API void InitFunc(void)
{   
	CInterface_SvrTcp::getObj();
	_GetForwarding().Init();
	CLinkEntity::InitSSL();
	if(!_LoadFrontAddrs(TRADE_TYPE, g_setTradeFrontAdds) /*|| !_LoadFrontAddrs(QUOT_TYPE, g_setQuotFrontAdds)*/)
		printf("Load Front Address error,Please check !!!\n");

	DWORD dwThreadID = 0;
	//���������߳�
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);
	g_hSSLAcceptThread=CreateThread(NULL, 0, SSLAcceptProc, 0, 0, &dwThreadID);
	g_hSSLCloseThread=CreateThread(NULL, 0, SSLCloseProc, 0, 0, &dwThreadID);
}

//ģ��������ͷ���Դ���رչ����߳�
SVRTRADEFRONT_API void ReleaseFunc(void)
{ 
	CloseHandle(g_hSSLAcceptThread);
	g_hSSLAcceptThread = NULL;

	if(g_hThread) {
		//�˶����ݰ�
		CInterface_SvrTcp::getObj().UnsubscribeAllPkg(g_idThread);

		//����WM_QUIT֪ͨ�߳̽���
		PostThreadMessage(g_idThread,WM_QUIT,0,0);

		//�ȴ��߳��˳�
		DWORD ExitCode;
		WaitForSingleObject((HANDLE)g_hThread,8000);
		if(GetExitCodeThread((HANDLE)g_hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)g_hThread,0);
		CloseHandle(g_hThread);
		g_hThread=NULL;
		g_idThread=0;
	}

	_ClearAPI();
	CLinkEntity::ReleaseSSL();
	_GetForwarding().Release();
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
        //ProcessOneUniPkg_InThread(hSocket,PkgHead,pPkgData);	
		ProcessOneUniPkg_InThread(hSocket,(char*)TmpPkg.m_pbuf);	
    }
}

//�����߳�
DWORD ThreadWorker(void *arg)
{
	//���涩�ı��̸߳���Ȥ�����ݰ�
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_SvrTcpAccept,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_SvrTcpDisconn,g_idThread);
	///�û���¼����
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqUserLogin,g_idThread);
	///�ǳ�����
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqUserLogout,g_idThread);
	///�û������������
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqUserPasswordUpdate,g_idThread);
	///�ʽ��˻������������
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqTradingAccountPasswordUpdate,g_idThread);
	///����¼������
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqOrderInsert,g_idThread);
	///Ԥ��¼������
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqParkedOrderInsert,g_idThread);
	///Ԥ�񳷵�¼������
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqParkedOrderAction,g_idThread);
	///������������
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqOrderAction,g_idThread);
	///��ѯ��󱨵���������
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQueryMaxOrderVolume,g_idThread);
	///Ͷ���߽�����ȷ��
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqSettlementInfoConfirm,g_idThread);
	///����ɾ��Ԥ��
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqRemoveParkedOrder,g_idThread);
	///����ɾ��Ԥ�񳷵�
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqRemoveParkedOrderAction,g_idThread);
	///ѯ��¼������
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqForQuoteInsert,g_idThread);
	///ִ������¼������
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqExecOrderInsert,g_idThread);
	///ִ�������������
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqExecOrderAction,g_idThread);
	///�����ѯ����
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryOrder,g_idThread);
	///�����ѯ�ɽ�
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryTrade,g_idThread);
	///�����ѯͶ���ֲ߳�
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryInvestorPosition,g_idThread);
	///�����ѯ�ʽ��˻�
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryTradingAccount,g_idThread);
	///�����ѯͶ����
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryInvestor,g_idThread);
	///�����ѯ��Լ��֤����
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryInstrumentMarginRate,g_idThread);
	///�����ѯ��Լ��������
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryInstrumentCommissionRate,g_idThread);
	///�����ѯ������
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryExchange,g_idThread);
	///�����ѯ��Ʒ
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryProduct,g_idThread);
	///�����ѯ��Լ
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryInstrument,g_idThread);
	///�����ѯͶ���߽�����
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQrySettlementInfo,g_idThread);
	///�����ѯͶ���ֲ߳���ϸ
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryInvestorPositionDetail,g_idThread);
	///�����ѯ�ͻ�֪ͨ
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryNotice,g_idThread);
	///�����ѯ������Ϣȷ��
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQrySettlementInfoConfirm,g_idThread);
	///�����ѯͶ���ֲ߳���ϸ
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryInvestorPositionCombineDetail,g_idThread);
	///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryCFMMCTradingAccountKey,g_idThread);
	///�����ѯ��Ȩ���׳ɱ�
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryOptionInstrTradeCost,g_idThread);
	///�����ѯ��Ȩ��Լ������
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryOptionInstrCommRate,g_idThread);
	///�����ѯִ������
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryExecOrder,g_idThread);
	///�����ѯת����ˮ
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryTransferSerial,g_idThread);
	///�����ѯ����ǩԼ��ϵ
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryAccountregister,g_idThread);
	///�����ѯǩԼ����
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryContractBank,g_idThread);
	///�����ѯԤ��
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryParkedOrder,g_idThread);
	///�����ѯԤ�񳷵�
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryParkedOrderAction,g_idThread);
	///�����ѯ����֪ͨ
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryTradingNotice,g_idThread);
	///�����ѯ���͹�˾���ײ���
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQryBrokerTradingParams,g_idThread);
	///�ڻ����������ʽ�ת�ڻ�����
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqFromBankToFutureByFuture,g_idThread);
	///�ڻ������ڻ��ʽ�ת��������
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqFromFutureToBankByFuture,g_idThread);
	///�ڻ������ѯ�����������
	CInterface_SvrTcp::getObj().SubscribePkg(FTTCMD_ReqQueryBankAccountMoneyByFuture,g_idThread);

	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0)) {
		if(Msg.message==WM_COMMAND&&Msg.wParam==WndCmd_YourPkgArrival2) {
			//���ݰ��Ĵ���ID
			unsigned int EventParamID=(unsigned int)Msg.lParam;
			//���ݰ�����
			int PkgLen;
			//Socket���
			int hSocket;
			if(CInterface_SvrTcp::getObj().getEventParamObj().
				GetEventParam(EventParamID,NULL,NULL,NULL,&PkgLen,0,NULL)) {
					DealPakage(EventParamID,PkgLen,hSocket);
					//�ͷ�EventParam
					CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
			}
		}
	}

	return 0;
}

DWORD WINAPI SSLAcceptProc(LPVOID pParameter)
{
	struct sockaddr_in sa_cli;
	int client_len = 0;
	int sd = 0;

	client_len = sizeof(sa_cli);
	while((sd = accept(CLinkEntity::sm_SocketListen, (struct sockaddr*) &sa_cli, &client_len)) != -1) {
		CLinkEntity* pEntity = new CLinkEntity();
		pEntity->ResetCloseThread(g_hSSLCloseThread, g_nSSLCloseMsgID);
		pEntity->Init(sd, TRUE);

	}
	int error = WSAGetLastError();
	return 0;
}

DWORD WINAPI SSLCloseProc(LPVOID pParameter)
{
	SOCKET hCloseSocket = 0;

	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0)) {
		if(Msg.message==g_nSSLCloseMsgID && Msg.wParam != 0) {
			hCloseSocket = Msg.wParam;
			//CLinkEntity* pEntity = GetLinkEntity(hCloseSocket);
			//if(pEntity != NULL) {
			//	DeleteLinkEntity(hCloseSocket);
			//	pEntity->Release();
			//	delete pEntity;
			//}
		}
	}
	return 0;
}

//����һ��SvrTcp���͹��������ݰ�
//void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
void ProcessOneUniPkg_InThread(int hSocket,const void* pPackage)
{
    Stru_UniPkgHead& PkgHead=*((Stru_UniPkgHead*)pPackage);
    void* pPkgData=(char*)pPackage+sizeof(Stru_UniPkgHead);

	if(PkgHead.cmdid == CMDID_SvrTcpAccept) {
		LOG_INFO("------>>>CMDID_SvrTcpAccept Strat, Socket[%d]", hSocket);
		_AddAPI(hSocket);
		LOG_INFO("------>>>CMDID_SvrTcpAccept End, Socket[%d]", hSocket);
		return;
	}
	else if(PkgHead.cmdid == CMDID_SvrTcpDisconn) {
		LOG_INFO("------>>>CMDID_SvrTcpDisconn Strat, Socket[%d]", hSocket);
		_DeleteAPI(hSocket);			
		LOG_INFO("------>>>CMDID_SvrTcpDisconn End, Socket[%d]", hSocket);
		return;
	}

	int nServSeq = _GetForwarding().GetNextSeqID();
	_GetForwarding().SendSrcPackage((void*)pPackage, PkgHead.len+sizeof(Stru_UniPkgHead)+4, nServSeq, hSocket);
	
	CDataPackageForwarding::Stru_ThreadData tdata;
	memset(&tdata, 0, sizeof(tdata));
	tdata.base = PkgHead;
	tdata.nServSeq = nServSeq;
	tdata.nSocket = hSocket;
	_SetThreadServData(tdata);

	//-----------------------------------------------------------------------------------
	//	����������ݰ��������֣��������ݰ�
	//-----------------------------------------------------------------------------------
	CFTTraderApi *pApi = _GetAPI(hSocket);
	if(!pApi)
	{
		printf("--------error: FTTraderApi handle is NULL,Please check!----------\n");
		return;
	}

	int nRspCmdID = 0;
	CThostFtdcRspInfoField RspInfo = {0};
	RspInfo.ErrorID = -1;

	switch(PkgHead.cmdid)
	{
		///�û���¼����
	case FTTCMD_ReqUserLogin:
		{
			nRspCmdID = FTTCMD_ReqUserLogin_Rsp;
			CThostFtdcReqUserLoginField* pData = (CThostFtdcReqUserLoginField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqUserLogin(pData, PkgHead.userdata1);
			break;
		}		
		///�ǳ�����
	case FTTCMD_ReqUserLogout:
		{
			nRspCmdID = FTTCMD_ReqUserLogout_Rsp;
			CThostFtdcUserLogoutField* pData = (CThostFtdcUserLogoutField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqUserLogout(pData, PkgHead.userdata1);
			break;
		}
		///�û������������
	case FTTCMD_ReqUserPasswordUpdate:
		{
			nRspCmdID = FTTCMD_ReqUserPasswordUpdate_Rsp;
			PlatformStru_UserPasswordUpdate* pData = (PlatformStru_UserPasswordUpdate*)pPkgData;				
			RspInfo.ErrorID = pApi->ReqUserPasswordUpdate(pData, PkgHead.userdata1);				
			break;
		}
		///�ʽ��˻������������
	case FTTCMD_ReqTradingAccountPasswordUpdate:
		{
			nRspCmdID = FTTCMD_ReqTradingAccountPasswordUpdate_Rsp;
			PlatformStru_TradingAccountPasswordUpdate* pData = (PlatformStru_TradingAccountPasswordUpdate*)pPkgData;
			RspInfo.ErrorID = pApi->ReqTradingAccountPasswordUpdate(pData, PkgHead.userdata1);
			break;
		}
		///����¼������
	case FTTCMD_ReqOrderInsert:
		{
			nRspCmdID = FTTCMD_ReqOrderInsert_Rsp;
			PlatformStru_InputOrder* pData = (PlatformStru_InputOrder*)pPkgData;
			RspInfo.ErrorID = pApi->ReqOrderInsert(pData, PkgHead.userdata1);
			break;
		}
		///Ԥ��¼������
	case FTTCMD_ReqParkedOrderInsert:
		{
			nRspCmdID = FTTCMD_ReqParkedOrderInsert_Rsp;
			PlatformStru_ParkedOrder* pData = (PlatformStru_ParkedOrder*)pPkgData;
			RspInfo.ErrorID = pApi->ReqParkedOrderInsert(pData, PkgHead.userdata1);
			break;
		}
		///Ԥ�񳷵�¼������
	case FTTCMD_ReqParkedOrderAction:
		{
			nRspCmdID = FTTCMD_ReqParkedOrderAction_Rsp;
			PlatformStru_ParkedOrderAction* pData = (PlatformStru_ParkedOrderAction*)pPkgData;
			RspInfo.ErrorID = pApi->ReqParkedOrderAction(pData, PkgHead.userdata1);
			break;
		}
		///������������
	case FTTCMD_ReqOrderAction:
		{
			nRspCmdID = FTTCMD_ReqOrderAction_Rsp;
			PlatformStru_InputOrderAction* pData = (PlatformStru_InputOrderAction*)pPkgData;
			RspInfo.ErrorID = pApi->ReqOrderAction(pData, PkgHead.userdata1);
			break;
		}
		///��ѯ��󱨵���������
	case FTTCMD_ReqQueryMaxOrderVolume:
		{
			nRspCmdID = FTTCMD_ReqQueryMaxOrderVolume_Rsp;
			PlatformStru_QueryMaxOrderVolume* pData = (PlatformStru_QueryMaxOrderVolume*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQueryMaxOrderVolume(pData, PkgHead.userdata1);
			break;
		}
		///Ͷ���߽�����ȷ��
	case FTTCMD_ReqSettlementInfoConfirm:
		{
			nRspCmdID = FTTCMD_ReqSettlementInfoConfirm_Rsp;
			CThostFtdcSettlementInfoConfirmField* pData = (CThostFtdcSettlementInfoConfirmField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqSettlementInfoConfirm(pData, PkgHead.userdata1);
			break;
		}
		///����ɾ��Ԥ��
	case FTTCMD_ReqRemoveParkedOrder:
		{
			nRspCmdID = FTTCMD_ReqRemoveParkedOrder_Rsp;
			PlatformStru_RemoveParkedOrder* pData = (PlatformStru_RemoveParkedOrder*)pPkgData;
			RspInfo.ErrorID = pApi->ReqRemoveParkedOrder(pData, PkgHead.userdata1);
			break;
		}
		///����ɾ��Ԥ�񳷵�
	case FTTCMD_ReqRemoveParkedOrderAction:
		{
			nRspCmdID = FTTCMD_ReqRemoveParkedOrderAction_Rsp;
			PlatformStru_RemoveParkedOrderAction* pData = (PlatformStru_RemoveParkedOrderAction*)pPkgData;
			RspInfo.ErrorID = pApi->ReqRemoveParkedOrderAction(pData, PkgHead.userdata1);
			break;
		}
	///ѯ��¼������
	case FTTCMD_ReqForQuoteInsert:
		{
			nRspCmdID = FTTCMD_ReqForQuoteInsert_Rsp;
			PlatformStru_InputForQuoteField* pData = (PlatformStru_InputForQuoteField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqForQuoteInsert(pData, PkgHead.userdata1);
			break;
		}
		///ִ������¼������
	case FTTCMD_ReqExecOrderInsert:
		{
			nRspCmdID = FTTCMD_ReqExecOrderInsert_Rsp;
			PlatformStru_InputExecOrderField* pData = (PlatformStru_InputExecOrderField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqExecOrderInsert(pData, PkgHead.userdata1);
			break;
		}
		///ִ�������������
	case FTTCMD_ReqExecOrderAction:
		{
			nRspCmdID = FTTCMD_ReqExecOrderAction_Rsp;
			PlatformStru_InputExecOrderActionField* pData = (PlatformStru_InputExecOrderActionField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqExecOrderAction(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯ����
	case FTTCMD_ReqQryOrder:
		{
			nRspCmdID = FTTCMD_ReqQryOrder_Rsp;
			PlatformStru_QryOrder* pData = (PlatformStru_QryOrder*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryOrder(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯ�ɽ�
	case FTTCMD_ReqQryTrade:
		{
			nRspCmdID = FTTCMD_ReqQryTrade_Rsp;
			PlatformStru_QryTrade* pData = (PlatformStru_QryTrade*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryTrade(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯͶ���ֲ߳�
	case FTTCMD_ReqQryInvestorPosition:
		{
			nRspCmdID = FTTCMD_ReqQryInvestorPosition_Rsp;
			PlatformStru_QryInvestorPosition* pData = (PlatformStru_QryInvestorPosition*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryInvestorPosition(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯ�ʽ��˻�
	case FTTCMD_ReqQryTradingAccount:
		{
			nRspCmdID = FTTCMD_ReqQryTradingAccount_Rsp;
			PlatformStru_QryTradingAccount* pData = (PlatformStru_QryTradingAccount*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryTradingAccount(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯͶ����
	case FTTCMD_ReqQryInvestor:
		{
			nRspCmdID = FTTCMD_ReqQryInvestor_Rsp;
			PlatformStru_QryInvestor* pData = (PlatformStru_QryInvestor*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryInvestor(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯ��Լ��֤����
	case FTTCMD_ReqQryInstrumentMarginRate:
		{
			nRspCmdID = FTTCMD_ReqQryInstrumentMarginRate_Rsp;
			CThostFtdcQryInstrumentMarginRateField* pData = (CThostFtdcQryInstrumentMarginRateField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryInstrumentMarginRate(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯ��Լ��������
	case FTTCMD_ReqQryInstrumentCommissionRate:
		{
			nRspCmdID = FTTCMD_ReqQryInstrumentCommissionRate_Rsp;
			CThostFtdcQryInstrumentCommissionRateField* pData = (CThostFtdcQryInstrumentCommissionRateField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryInstrumentCommissionRate(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯ������
	case FTTCMD_ReqQryExchange:
		{
			nRspCmdID = FTTCMD_ReqQryExchange_Rsp;
			CThostFtdcQryExchangeField* pData = (CThostFtdcQryExchangeField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryExchange(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯ��Ʒ
	case FTTCMD_ReqQryProduct:
		{
			nRspCmdID = FTTCMD_ReqQryProduct_Rsp;
			PlatformStru_QryProduct* pData = (PlatformStru_QryProduct*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryProduct(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯ��Լ
	case FTTCMD_ReqQryInstrument:
		{
			nRspCmdID = FTTCMD_ReqQryInstrument_Rsp;
			PlatformStru_QryInstrument* pData = (PlatformStru_QryInstrument*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryInstrument(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯͶ���߽�����
	case FTTCMD_ReqQrySettlementInfo:
		{
			nRspCmdID = FTTCMD_ReqQrySettlementInfo_Rsp;
			PlatformStru_QrySettlementInfo* pData = (PlatformStru_QrySettlementInfo*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQrySettlementInfo(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯͶ���ֲ߳���ϸ
	case FTTCMD_ReqQryInvestorPositionDetail:
		{
			nRspCmdID = FTTCMD_ReqQryInvestorPositionDetail_Rsp;
			PlatformStru_QryInvestorPositionDetail* pData = (PlatformStru_QryInvestorPositionDetail*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryInvestorPositionDetail(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯ�ͻ�֪ͨ
	case FTTCMD_ReqQryNotice:
		{
			nRspCmdID = FTTCMD_ReqQryNotice_Rsp;
			PlatformStru_QryNotice* pData = (PlatformStru_QryNotice*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryNotice(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯ������Ϣȷ��
	case FTTCMD_ReqQrySettlementInfoConfirm:
		{
			nRspCmdID = FTTCMD_ReqQrySettlementInfoConfirm_Rsp;
			PlatformStru_QrySettlementInfoConfirm* pData = (PlatformStru_QrySettlementInfoConfirm*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQrySettlementInfoConfirm(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯͶ���ֲ߳���ϸ
	case FTTCMD_ReqQryInvestorPositionCombineDetail:
		{
			nRspCmdID = FTTCMD_ReqQryInvestorPositionCombineDetail_Rsp;
			PlatformStru_QryInvestorPositionCombineDetail* pData = (PlatformStru_QryInvestorPositionCombineDetail*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryInvestorPositionCombineDetail(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
	case FTTCMD_ReqQryCFMMCTradingAccountKey:
		{
			nRspCmdID = FTTCMD_ReqQryCFMMCTradingAccountKey_Rsp;
			CThostFtdcQryCFMMCTradingAccountKeyField* pData = (CThostFtdcQryCFMMCTradingAccountKeyField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryCFMMCTradingAccountKey(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯ��Ȩ���׳ɱ�
	case FTTCMD_ReqQryOptionInstrTradeCost:
		{
			nRspCmdID = FTTCMD_ReqQryOptionInstrTradeCost_Rsp;
			CThostFtdcQryOptionInstrTradeCostField* pData = (CThostFtdcQryOptionInstrTradeCostField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryOptionInstrTradeCost(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯ��Ȩ��Լ������
	case FTTCMD_ReqQryOptionInstrCommRate:
		{
			nRspCmdID = FTTCMD_ReqQryOptionInstrCommRate_Rsp;
			CThostFtdcQryOptionInstrCommRateField* pData = (CThostFtdcQryOptionInstrCommRateField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryOptionInstrCommRate(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯִ������
	case FTTCMD_ReqQryExecOrder:
		{
			nRspCmdID = FTTCMD_ReqQryExecOrder_Rsp;
			PlatformStru_QryExecOrderField* pData = (PlatformStru_QryExecOrderField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryExecOrder(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯת����ˮ
	case FTTCMD_ReqQryTransferSerial:
		{
			nRspCmdID = FTTCMD_ReqQryTransferSerial_Rsp;
			PlatformStru_QryTransferSerial* pData = (PlatformStru_QryTransferSerial*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryTransferSerial(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯ����ǩԼ��ϵ
	case FTTCMD_ReqQryAccountregister:
		{
			nRspCmdID = FTTCMD_ReqQryAccountregister_Rsp;
			PlatformStru_QryAccountRegister* pData = (PlatformStru_QryAccountRegister*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryAccountregister(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯǩԼ����
	case FTTCMD_ReqQryContractBank:
		{
			nRspCmdID = FTTCMD_ReqQryContractBank_Rsp;
			PlatformStru_QryContractBank* pData = (PlatformStru_QryContractBank*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryContractBank(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯԤ��
	case FTTCMD_ReqQryParkedOrder:
		{
			nRspCmdID = FTTCMD_ReqQryParkedOrder_Rsp;
			PlatformStru_QryParkedOrder* pData = (PlatformStru_QryParkedOrder*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryParkedOrder(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯԤ�񳷵�
	case FTTCMD_ReqQryParkedOrderAction:
		{
			nRspCmdID = FTTCMD_ReqQryParkedOrderAction_Rsp;
			PlatformStru_QryParkedOrderAction* pData = (PlatformStru_QryParkedOrderAction*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryParkedOrderAction(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯ����֪ͨ
	case FTTCMD_ReqQryTradingNotice:
		{
			nRspCmdID = FTTCMD_ReqQryTradingNotice_Rsp;
			CThostFtdcQryTradingNoticeField* pData = (CThostFtdcQryTradingNoticeField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryTradingNotice(pData, PkgHead.userdata1);
			break;
		}
		///�����ѯ���͹�˾���ײ���
	case FTTCMD_ReqQryBrokerTradingParams:
		{
			nRspCmdID = FTTCMD_ReqQryBrokerTradingParams_Rsp;
			CThostFtdcQryBrokerTradingParamsField* pData = (CThostFtdcQryBrokerTradingParamsField*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQryBrokerTradingParams(pData, PkgHead.userdata1);
			break;
		}
		///�ڻ����������ʽ�ת�ڻ�����
	case FTTCMD_ReqFromBankToFutureByFuture:
		{
			nRspCmdID = FTTCMD_ReqFromBankToFutureByFuture_Rsp;
			PlatformStru_ReqTransfer* pData = (PlatformStru_ReqTransfer*)pPkgData;
			RspInfo.ErrorID = pApi->ReqFromBankToFutureByFuture(pData, PkgHead.userdata1);
			break;
		}
		///�ڻ������ڻ��ʽ�ת��������
	case FTTCMD_ReqFromFutureToBankByFuture:
		{
			nRspCmdID = FTTCMD_ReqFromFutureToBankByFuture_Rsp;
			PlatformStru_ReqTransfer* pData = (PlatformStru_ReqTransfer*)pPkgData;
			RspInfo.ErrorID = pApi->ReqFromFutureToBankByFuture(pData, PkgHead.userdata1);
			break;
		}
		///�ڻ������ѯ�����������
	case FTTCMD_ReqQueryBankAccountMoneyByFuture:
		{
			nRspCmdID = FTTCMD_ReqQueryBankAccountMoneyByFuture_Rsp;
			PlatformStru_ReqQueryAccount* pData = (PlatformStru_ReqQueryAccount*)pPkgData;
			RspInfo.ErrorID = pApi->ReqQueryBankAccountMoneyByFuture(pData, PkgHead.userdata1);
			break;
		}
	default:
		nRspCmdID = FTTCMD_OnRspError_RON;
		sprintf_s(RspInfo.ErrorMsg, sizeof(RspInfo.ErrorMsg), "FTT: δ֪��������[%d]", PkgHead.cmdid);
	};

	int nSeq = _GetNextSeq();
	CInterface_SvrTcp::getObj().SendPkgData(hSocket, nRspCmdID, &RspInfo, sizeof(CThostFtdcRspInfoField), nSeq);
	if(RspInfo.ErrorID!=0)
		_GetForwarding().SendPackage(&RspInfo, sizeof(RspInfo), nRspCmdID, nSeq, 
				PkgHead.userdata1, 5, nServSeq, hSocket);
}

