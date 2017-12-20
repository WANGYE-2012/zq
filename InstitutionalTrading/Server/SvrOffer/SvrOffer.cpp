#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#include "StdAfx.h"
#include <windows.h>
#include "SvrOffer.h"
#include "CommonPkg.h"

#include "EventParam.h"
#include "tmpstackbuf.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"

#include "OfferMainInterface.h"
#include "CommonDataChangeEvent.h"
#include "ToolThread.h"

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
#pragma comment(lib, "SvrBrokerInfo.lib")
#pragma comment(lib, "SvrUserOrg.lib")
#pragma comment(lib, "SvrNotifyAndAsk.lib")
#pragma comment(lib, "SvrLogin.lib")



#define WRITELOGID_SvrOffer




//ȫ�ֻ�����
Ceasymutex			g_mutex;

//�̲߳���
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;


COfferMainInterface*			g_pOfferMain;
UINT_PTR            g_lTime;//��ʱ����˺���ע��ǰ��
extern UINT_PTR g_lTimeDisconnect;//�˺ŶϿ�
extern UINT_PTR g_lTimeQuto;//��������
//CToolThread*		g_hThreadProcessTradeData;
//DWORD WINAPI ThreadProcTradeDataQuery (void* pParam);
DWORD ThreadWorker(void *arg);

//����һ�����յ������ݰ�
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//�����ڲ�֪ͨ���¼�
bool ParseDataChangedEvent(Stru_NotifyEvent& dataEvt);

bool LoadXmlConfig(std::string strConfig, std::string& strIP, int& nPort)
{
	std::string apppath=strConfig;	
	char szLocalPath[256];
	memset(szLocalPath, 0, 256);
	GetModuleFileName( NULL, szLocalPath, 256 );
	std::string strSystemPath( szLocalPath );
	int nPos = strSystemPath.rfind( '\\' );
	if ( -1 != nPos )
	{
		strSystemPath = strSystemPath.substr( 0, nPos + 1 );
		apppath = strSystemPath + apppath;
	}	
	
	TiXmlDocument* pDocument=new TiXmlDocument(apppath.c_str());
	TiXmlDocument& myDocument=*pDocument; 
	bool retval=false;
	if(myDocument.LoadFile())
	{
		TiXmlElement* RootElement=myDocument.RootElement();
		if(RootElement)
		{
			const char* pRootElement = RootElement->Value();
			if(strcmp(pRootElement, "root") ==0)
			{
				TiXmlNode* pNode=RootElement->FirstChild();
				if(pNode)
				{
					const char* pElement = pNode->Value();
					if(strcmp(pElement, "MarketData") ==0)
					{
						TiXmlElement* pChild=pNode->FirstChildElement();
						if(pChild)
						{
							std::string str = pChild->GetText();

							int nPos = str.find(':');
							if ( nPos != -1 )
							{
								strIP = str.substr(0, nPos);
								nPort = atoi(str.substr(nPos+1, str.length()).c_str());
							//	CTCPDataService::GetInstance()->Connect(strIP, nPort);
							}
						}

					}
					
				}

			}			
		}
	}

	return true;
}

//ģ���ʼ�������������̲߳���SvrTcp���ĸ���Ȥ�����ݰ�
SVROFFER_API void InitFunc(void)
{		
	g_pOfferMain = NULL;
	

	

	//���������߳�
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);

	//g_hThreadProcessTradeData = new CToolThread((LPTHREAD_START_ROUTINE)ThreadProcTradeDataQuery,NULL);
		
	//���涩�ı��̸߳���Ȥ�����ݰ�
//	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_HEARTBEAT,g_idThread);
	//CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ReInitQuotServerEvent_Req,g_idThread);
	//CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QuotServerTimeResetEvent_Req,g_idThread);
	//CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QryQuotServerTimeEvent_Req,g_idThread);
	//	

	////���Ľ������ݹ���ģ������ݰ�
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryInstrument, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryCommission, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryMargin, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryOrder, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryTrader, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedSingleLoginTrade, g_idThread);

	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedGetInstrument, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedAllLoginTrade, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedAllLoginSettlement, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedLogoffSettlement, g_idThread);
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeStartSettlement, g_idThread);//�Ͽ�ctp����
	//CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeInitFinish, g_idThread);

	
	
	//CInterface_SvrTradeData::getObj().TradeDataInit();

	

	
}

//ģ��������ͷ���Դ���رչ����߳�
SVROFFER_API void ReleaseFunc(void)
{
	if(g_pOfferMain)
	{
		delete g_pOfferMain;
		g_pOfferMain = NULL;
	}

	if(g_hThread)
	{
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
	//In the thread to which the message will be posted, call PeekMessage as shown here to force the system to create the message queue.
	MSG MsgTemp;
	PeekMessage(&MsgTemp, NULL, WM_USER, WM_USER, PM_NOREMOVE);//��������������ڽ�����Ϣ����

	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ReInitQuotServerEvent_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QuotServerTimeResetEvent_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QryQuotServerTimeEvent_Req,g_idThread);

	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QrySimulateDay_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QrySimulatePlayMode_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SimulatePlay_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SimulatePause_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SimulateStop_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SimulateApply_Req,g_idThread);



	//���Ľ������ݹ���ģ������ݰ�
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryInstrument, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryCommission, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryMargin, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryOrder, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryTrader, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedSingleLoginTrade, g_idThread);

	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryFund, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryPosition, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedQryPositionDetail, g_idThread);

	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedGetInstrument, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedAllLoginTrade, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedAllLoginSettlement, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeNeedLogoffSettlement, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeStartSettlement, g_idThread);//�Ͽ�ctp����
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeInitFinish, g_idThread);
	
	CInterface_SvrTradeData::getObj().TradeDataInit();

	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{
		if(Msg.message == WM_TIMER)
		{
			if(g_lTimeDisconnect == Msg.wParam)
			{
				CCTPAccount *pCtpAccount = NULL;
				bool bGet = g_pOfferMain->GetCtpAccountByTime(Msg.wParam, pCtpAccount);
				if(bGet && pCtpAccount)
				{//�����������ԶϿ����Ӻ��
					SLogin login;
					pCtpAccount->GetLogin(login);
					CInterface_SvrTradeData::getObj().SetAccountStatus(login.strUserID, login.strBrokerID, ACCOUNT_STATUS_CannotConnect);
					g_pOfferMain->RemoveCtpAccountByTime(Msg.wParam);
					KillTimer(NULL, Msg.wParam);	

					if(pCtpAccount->GetIsQueryRatio())
					{
						OFFER_INFO("���ò�����˺ŶϿ�����ctp״̬�� %s, %s\n", login.strBrokerID.c_str(),login.strUserID.c_str())
						PUSH_LOG_ERROR(FirstLevelError,true,true,"���ò�����˺ŶϿ�����ctp״̬�� %s, %s\n", login.strBrokerID.c_str(),login.strUserID.c_str())
					}
					else
					{
						OFFER_INFO("�������˺ŶϿ�����ctp״̬�� %s, %s\n", login.strBrokerID.c_str(),login.strUserID.c_str())
						PUSH_LOG_ERROR(FirstLevelError,true,true,"�������˺ŶϿ�����ctp״̬�� %s, %s\n", login.strBrokerID.c_str(),login.strUserID.c_str())

					}

					continue;
				}
			}

			if(Msg.wParam == g_lTime)
			{//���������������˻�����10��û���ϣ���ǲ��ϵ�
				if(g_pOfferMain)
				{				
					g_pOfferMain->CheckAccountsConnect();
					
				}
				KillTimer(NULL, g_lTime);
			}
			if(Msg.wParam == g_lTimeQuto)
			{//��������
				std::map<std::string, int> mapInstrumentID;
				CTCPDataService::GetInstance()->GetQutoInstruments(mapInstrumentID);
				if(g_pOfferMain && mapInstrumentID.size()>0)
				{				
					g_pOfferMain->QueryQuot(mapInstrumentID);
				}				
			}			
		}
		if(Msg.message==WM_COMMAND&&Msg.wParam==WndCmd_YourPkgArrival2)
		{
			//���ݰ��Ĵ���ID
			unsigned int EventParamID=(unsigned int)Msg.lParam;
			//���ݰ�����
			int PkgLen;
			//Socket���
			int hSocket;
			if(CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,NULL,&PkgLen,0,NULL))
			{
				//������ʱ�ռ䲢��ȡ���ݰ�
				//AllocTmpStackBuf(TmpPkg,PkgLen);
				//if(TmpPkg.m_pbuf&&
				//	CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,TmpPkg.m_pbuf,&PkgLen,PkgLen,&hSocket)&&
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
		else if(Msg.message==WM_COMMAND&&Msg.wParam==WndCmd_NotifyEventArrival)		
		{
				//���ݰ��Ĵ���ID
				unsigned int EventParamID=(unsigned int)Msg.lParam;
				//���ݰ�����
				Stru_NotifyEvent ls;
				int nLen = sizeof(ls);
				if(CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,&ls,&nLen,nLen,NULL))
				{			
					//�������ݰ��������������ݰ�
					ParseDataChangedEvent(ls);			
					//�ͷ�EventParam
					CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
				}
		}
	}
	return 0;
}

//����һ��SvrTcp���͹��������ݰ�
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	//-----------------------------------------------------------------------------------
	//	����������ݰ��������֣��������ݰ�
	//-----------------------------------------------------------------------------------
	switch(PkgHead.cmdid)
	{
		case CMDID_HEARTBEAT:
			{
				break;
			}	
		case Cmd_ReInitQuotServerEvent_Req:
			{
				if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,CONTROL_SERVER_QUOT_RESET))
				{
					const char * lErrorString = FormatErrorCode(CF_ERROR_CONTROL_SERVER_QUOT_RESET_NOPRIVILEDGE);
					CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ReInitQuotServerEvent_Rsp, 
						(void*)lErrorString, 
						strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
					break;
				}				
				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_ReInitQuotServerEvent_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
		case Cmd_QuotServerTimeResetEvent_Req:
			{
				if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,CONTROL_TIME_QUOT_SETUP))
				{
					const char * lErrorString = FormatErrorCode(CF_ERROR_CONTROL_TIME_QUOT_SETUP_NOPRIVILEDGE);
					CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_QuotServerTimeResetEvent_Rsp, 
						(void*)lErrorString, 
						strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
					break;
				}

				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_QuotServerTimeResetEvent_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
		case Cmd_QryQuotServerTimeEvent_Req:
			{
				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_QryQuotServerTimeEvent_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
		case Cmd_QrySimulateDay_Req:
			{
				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_QrySimulateDay_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
		case Cmd_QrySimulatePlayMode_Req:
			{
				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_QrySimulatePlayMode_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
		case Cmd_SimulatePlay_Req:
			{
				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_SimulatePlay_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
		case Cmd_SimulatePause_Req:
			{
				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_SimulatePause_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
		case Cmd_SimulateStop_Req:
			{
				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_SimulateStop_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
		case Cmd_SimulateApply_Req:
			{
				int nSeqID = CTCPDataService::GetInstance()->InsertmapSeqSubID2Socket(hSocket);
				CTCPDataService::GetInstance()->SendData(Cmd_SimulateApply_Req, const_cast<void*>(pPkgData), PkgHead.len, nSeqID);
			}
			break;
	}
}
// ����һ������֪ͨ�¼�
bool ParseDataChangedEvent(Stru_NotifyEvent& dataEvt)
{
	switch(dataEvt.meEventType)
	{
	case EventTypeInitFinish:
		{//��ʼ����ɣ����������������ʼ���ģ���ҪΪ�˼������ݹ���׼���óֲֵ����⡣
			std::string strIP;
			int nPort;
			LoadXmlConfig("offer-config.xml", strIP, nPort);
			CTCPDataService::GetInstance()->Connect(strIP, nPort);	
			OFFER_INFO("���������������IP[%s], Port[%d]\n", strIP.c_str(), nPort);
			PUSH_LOG_ERROR(Logger,false,true,"���������������IP[%s], Port[%d]\n", strIP.c_str(), nPort);
		}
		break;
	case EventTypeNeedQryInstrument:			
		{//��ѯ��Լ

			if(g_pOfferMain == NULL)
				return false;
		
			g_pOfferMain->QueryInstruments();	
			OFFER_INFO("�յ���ѯ��Լ\n");
			PUSH_LOG_ERROR(Logger,false,true,"�յ���ѯ��Լ����\n");
		}
		break;
	case EventTypeNeedGetInstrument:
		{
			if(g_pOfferMain)
				g_pOfferMain->AddInstrument();
			OFFER_INFO("��Լ�ӽ������ݻ�ȡ\n");
			PUSH_LOG_ERROR(Logger,false,true,"�յ��ӽ������ݻ�ȡ��Լ����\n");
		}
		break;
	case EventTypeNeedAllLoginTrade:
		{
			OFFER_INFO("�����յ���½����ί�н����ʺ�ָ��\n");
			PUSH_LOG_ERROR(Logger,false,true,"�����յ���½����ί�н����ʺ�ָ��\n");
			KillTimer(NULL, g_lTime);
			if(g_pOfferMain)
			{
				delete g_pOfferMain;
				g_pOfferMain = NULL;
			}
			int nMorning =  dataEvt.mnDataID;//�����¼�IDΪ0 ,ҹ��Ϊ1
			#ifdef  SIMU//ģ�⽻��				
				g_pOfferMain = new COfferMain_Simu;
				
			#else
				g_pOfferMain = new COfferMain;				
			#endif

				g_pOfferMain->Init(0, nMorning);//�����¼�ID��nMorningΪ0 ,ҹ��Ϊ1	
			//��ʱ10s����˺��ǲ���ǰ������ʧ��
			#ifndef  SIMU
				g_lTime =SetTimer(NULL,TIMER_CHECK_ACCOUNTSTATUS,10000,(TIMERPROC)(NULL)); 	
            #endif

			
//#ifdef  SIMU//ģ�⽻��
//			CInterface_SvrSimulateTrade::getObj().Reinit();
//#endif
		}
		break;
	case EventTypeNeedSingleLoginTrade:
		{//��ѯ����
			if(g_pOfferMain == NULL)
				return false;

			int nTradeAccount = *(int*) dataEvt.marrDataByte;		
			g_pOfferMain->RestartAccount(nTradeAccount);		
			OFFER_INFO("�����˻��� %d\n", nTradeAccount);
			PUSH_LOG_ERROR(Logger,false,true,"���µ�½�˻��� %d\n", nTradeAccount);
		}
		break;	
	case EventTypeStartSettlement:
		{ //��ʼ���㣬�Ͽ�����ctp����
			KillTimer(NULL, g_lTime);
			if(g_pOfferMain)
			{
				delete g_pOfferMain;
				g_pOfferMain = NULL;
			}
			//HANDLE hEvent = (HANDLE)dataEvt.mhEvent;		
			//SetEvent(hEvent);
		}
		break;
	case EventTypeNeedAllLoginSettlement:
		{
			KillTimer(NULL, g_lTime);
			if(g_pOfferMain)
			{
				delete g_pOfferMain;
				g_pOfferMain = NULL;
			}
			#ifdef  SIMU//ģ�⽻��
				g_pOfferMain = new COfferMain_Simu;
			#else
				g_pOfferMain = new COfferMain;
			#endif

			g_pOfferMain->Init(1, 2);	//2�����ֵû����
			//��ʱ10s����˺��ǲ���ǰ������ʧ��
#ifndef  SIMU
			g_lTime =SetTimer(NULL,TIMER_CHECK_ACCOUNTSTATUS,10000,(TIMERPROC)(NULL)); 	
#endif
		}
		break;
	case EventTypeNeedQryCommission:
		{//��ѯ����
			if(g_pOfferMain == NULL)
				return false;

			BrokerAccountKey *BAKey = (BrokerAccountKey*) dataEvt.marrDataByte;
			
			g_pOfferMain->QueryCommission(*BAKey);		
			OFFER_INFO("�յ���ѯ�������ʣ� %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
			PUSH_LOG_ERROR(Logger,false,true,"�յ���ѯ������������ %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
		}
		break;	
	case EventTypeNeedQryMargin:
		{
			if(g_pOfferMain == NULL)
				return false;
			BrokerAccountKey *BAKey = (BrokerAccountKey*) dataEvt.marrDataByte;
			g_pOfferMain->QueryMargin(*BAKey);	
			OFFER_INFO("�յ���ѯ��֤���ʣ� %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
			PUSH_LOG_ERROR(Logger,false,true,"�յ���ѯ��֤�������� %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
		}
		break;   	 
   case EventTypeNeedQryOrder:
	   {
		   if(g_pOfferMain == NULL)
			   return false;
		  BrokerAccountKey *BAKey = (BrokerAccountKey*) dataEvt.marrDataByte;
		  g_pOfferMain->QueryOrder(*BAKey);	
		  OFFER_INFO("�յ���ѯ������ %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
		  PUSH_LOG_ERROR(Logger,false,true,"�յ���ѯ�������� %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
	   }
	   break;
   case EventTypeNeedQryTrader:
	   {//��ѯ�ɽ�
		   if(g_pOfferMain == NULL)
			   return false;
		   BrokerAccountKey *BAKey = (BrokerAccountKey*) dataEvt.marrDataByte;
		   g_pOfferMain->QueryTrade(*BAKey);	
		   OFFER_INFO("�յ���ѯ�ɽ��� %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
		   PUSH_LOG_ERROR(Logger,false,true,"�յ���ѯ�ɽ����� %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
	   }
	   break;
   case EventTypeNeedLogoffSettlement:
	   {//�Ͽ�ctp�˺�����
		   if(g_pOfferMain == NULL)
			   return false;
		   BrokerAccountKey *BAKey = (BrokerAccountKey*) dataEvt.marrDataByte;
		   g_pOfferMain->LogoffAccount(*BAKey);
			
	   }
	   break;
   case EventTypeNeedQryFund:
	   {
		   if(g_pOfferMain == NULL)
			   return false;
		   BrokerAccountKey *BAKey = (BrokerAccountKey*) dataEvt.marrDataByte;
		   g_pOfferMain->QueryFund(*BAKey);	
		   OFFER_INFO("�յ���ѯ�ʽ� %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
		   PUSH_LOG_ERROR(Logger,false,true,"�յ��ʽ����� %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
	   }
	   break;
   case EventTypeNeedQryPosition:
	   {
		   if(g_pOfferMain == NULL)
			   return false;
		   BrokerAccountKey *BAKey = (BrokerAccountKey*) dataEvt.marrDataByte;
		   g_pOfferMain->QueryPosition(*BAKey);	
		   OFFER_INFO("�յ���ѯ�ֲ֣� %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
		   PUSH_LOG_ERROR(Logger,false,true,"�յ��ֲ����� %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
	   }
	   break;
   case EventTypeNeedQryPositionDetail:
	   {//�ֲ���ϸ����ѯ����Ϊ��ѯ�߳̽��������ı�ʶ
		   if(g_pOfferMain == NULL)
			   return false;
		   BrokerAccountKey *BAKey = (BrokerAccountKey*) dataEvt.marrDataByte;
		   g_pOfferMain->QueryPositionDetail(*BAKey);	
		   OFFER_INFO("�յ���ѯ�ֲ���ϸ�� %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
		   PUSH_LOG_ERROR(Logger,false,true,"�յ��ֲ���ϸ���� %s, %s\n", BAKey->BrokerID, BAKey->AccountID);
	   }
	   break;
	}
	return true;
}