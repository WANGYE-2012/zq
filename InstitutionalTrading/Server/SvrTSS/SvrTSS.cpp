// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include "SvrTSS.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "..\\SvrUserOrg\\Interface_SvrUserOrg.h"
#include "..\\SvrNotifyAndAsk\Interface_SvrNotifyAndAsk.h"
#include "..\SvrTradingStrategy\Interface_SvrTradingStrategy.h"
#include "..\SvrUserOrg\Interface_SvrUserOrg.h"
#include "..\SvrTradeExcute\Interface_SvrTradeExcute.h"
#include "..\SvrMsg\Interface_SvrMsg.h"

#include <queue>
#include <map>
#include "PlotImport.h"
#include "DealerPlotCenter.h"

using namespace std;


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
#pragma comment(lib, "SvrTradingStrategy.lib")
#pragma comment(lib, "SvrTradeExcute.lib")
#pragma comment(lib, "SvrMsg.lib")



#define WRITELOGID_SvrTSS




//ȫ�ֻ�����
Ceasymutex			g_mutex;
CPlotImport			g_plotImport;
CDealerPlotCenter	g_dealerPlotCenter;

//�̲߳���
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;
HANDLE				g_hTimerThread=NULL;
DWORD				g_idTimerThread=0;
bool				g_bExit = false;
//DWORD				g_nReadInterval = 30;


DWORD ThreadWorker(void *arg);
DWORD __stdcall Proc_Timer(LPVOID lpParameter);


void PostUserMsg(IMPORTPLOTDIRECT& plot, set<int>& setUsers);
//// ����һ������֪ͨ�¼�
bool ParseDataChangedEvent(const Stru_NotifyEvent& dataEvt);

//����һ�����յ������ݰ�
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

void LoadConfig();
//void LoadDBData(map<int, set<string>>& mapPlot2Dealer, vector<CPlotImport::PLOTCFGITEM>& vecPlotData);
void LoadDBData(map<string, set<string>>& mapDealerPlots, vector<CPlotImport::PLOTCFGITEM>& vecPlotData);

bool Reinit();
bool ResetPlotImports();
bool ResetDealorPlots();

//ģ���ʼ�������������̲߳���SvrTcp���ĸ���Ȥ�����ݰ�
SVRTSS_API void InitFunc(void)
{
	//map<string, set<string>> mapDealerPlots;
	//vector<CPlotImport::PLOTCFGITEM> vecPlotData;
	//LoadConfig();
	//LoadDBData(mapDealerPlots, vecPlotData);
	//g_plotImport.Init(vecPlotData);
	//g_dealerPlotCenter.Init(mapDealerPlots);
	Reinit();
	
	//���������߳�
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);
	g_hTimerThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)Proc_Timer,0,0,&g_idTimerThread);
	
	//���涩�ı��̸߳���Ȥ�����ݰ�
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_HEARTBEAT,g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeUserAdd, g_idThread);
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeStartInit, g_idThread);
	
}

//ģ��������ͷ���Դ���رչ����߳�
SVRTSS_API void ReleaseFunc(void)
{
	g_bExit = true;

	if(g_hThread)
	{
		//�˶����ݰ�
		CInterface_SvrTcp::getObj().UnsubscribeAllPkg(g_idThread);
		CInterface_SvrNotifyAndAsk::getObj().UnsubscribeNotifyEvent(g_idThread);

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

bool Reinit()
{
	map<string, set<string>> mapDealerPlots;
	vector<CPlotImport::PLOTCFGITEM> vecPlotData;

	g_plotImport.Release();
	g_dealerPlotCenter.Release();

	LoadConfig();
	LoadDBData(mapDealerPlots, vecPlotData);
	g_plotImport.Init(vecPlotData);
	g_dealerPlotCenter.Init(mapDealerPlots);

	return true;
}

bool ResetPlotImports()
{
	map<string, set<string>> mapDealerPlots;
	vector<CPlotImport::PLOTCFGITEM> vecPlotData;

	g_plotImport.Release();

	LoadDBData(mapDealerPlots, vecPlotData);
	g_plotImport.Init(vecPlotData);

	return true;
}

bool ResetDealorPlots()
{
	map<string, set<string>> mapDealerPlots;
	vector<CPlotImport::PLOTCFGITEM> vecPlotData;

	g_dealerPlotCenter.Release();

	LoadDBData(mapDealerPlots, vecPlotData);
	g_dealerPlotCenter.Init(mapDealerPlots);

	return true;
}

void LoadConfig()
{
#if 0
	char csIniFile[1024];
	char csText[1024];
	ZeroMemory(csIniFile, sizeof(csIniFile));
	ZeroMemory(csText, sizeof(csText));
	int dwLen = GetCurrentDirectory(sizeof(csIniFile)-1, csIniFile);
	if(csIniFile[dwLen-1]!='\\')
		strcat(csIniFile, "\\");
	strcat(csIniFile, "PlotCtrlServer.INI");

	g_nReadInterval = GetPrivateProfileInt("Plot", "ReadInterval", 30, csIniFile);
#endif
}

void PostUserMsg(IMPORTPLOTDIRECT& plot, set<string>& setUsers)
{
	set<int> setRealUsers;
	int nExpiredDate = 0;
	char csMsg[1024];
	SYSTEMTIME sysTime;
	UserInfo userInfo;

	for(set<string>::iterator it = setUsers.begin(); it != setUsers.end(); it++) {
		if(CInterface_SvrUserOrg::getObj().GetUserInfoByAccount(*it, userInfo))
			setRealUsers.insert(userInfo.nUserID);
	}

	::GetLocalTime(&sysTime);
	nExpiredDate = sysTime.wYear*10000 + sysTime.wMonth*100 + sysTime.wDay;

	memset(csMsg, 0, sizeof(csMsg));
	sprintf_s(csMsg, sizeof(csMsg)-1, TEXT("��Լ: %s, ����ID: %s, ����: %s"), 
			plot.instID, plot.plotID, plot.direct);
	CInterface_SvrMsg::getObj().SendMsg(0, setRealUsers, conStrategyMsg, nExpiredDate, 
			string("������ʾ"), string(csMsg));
}

DWORD __stdcall Proc_Timer(LPVOID lpParameter)
{
	while(!g_bExit) {
		queue<IMPORTPLOTDIRECT> queueImport;
		map<string, set<string>> mapInst2Dealer;
		g_plotImport.ImportPlot(queueImport);
		while(!queueImport.empty()) {
			set<string> setDealer;
			set<string>::iterator it;
			int nInstCtrl = -1;
			if(g_dealerPlotCenter.AppendPlotDirect(queueImport.front(), setDealer)) {
				//mapInst2Dealer
				//set�ϲ�set�ķ���s1.insert( s2.begin(), s2.end() );
				for(it = setDealer.begin(); it != setDealer.end(); it++) {
					if(g_dealerPlotCenter.CalcCtrl(*it, 
							string(queueImport.front().instID), nInstCtrl)) {
						// �ѷ������д������ִ��ģ����
						CInterface_SvrTradeExcute::getObj().SetOneStrategyOrderLimit(
								*it, string(queueImport.front().instID), 
								(StrategyOrderLimit)nInstCtrl);
					}
				}
				PostUserMsg(queueImport.front(), setDealer);
			}
			queueImport.pop();
		}
		Sleep(1000);

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
	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{
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
	}
	return 0;
}

// ����һ������֪ͨ�¼�
bool ParseDataChangedEvent(const Stru_NotifyEvent& dataEvt)
{
	switch(dataEvt.meEventType) 
	{
	//case EventTypeUserAdd:			// �¼�һ���û�
	//{
	//	//dataEvt.meDataChangeType;      //���ݸı�����
	//	//dataEvt.mnDataID;             //����ID
	//	int nOrgID = 0;
	//	UserInfo userInfo = {0};
	//	if(CInterface_SvrUserOrg::getObj().GetUserByID(dataEvt.mnDataID, userInfo) && 
	//			userInfo.nUserType==USER_TYPE_TRADER &&
	//			CInterface_SvrUserOrg::getObj().GetOrgIDByUserID(dataEvt.mnDataID, nOrgID)) {
	//		// ����������û�ID
	//		// ͨ���û�ID����û�����Ϣ���ж��Ƿ�Ϊ����Ա
	//		// ��ý���Ա���ڵĲ��ţ�����벿����صĲ��Լ��ϡ�
	//		// ʹ�ý���Ա���ʺ�ID��Ϊ��һ�����������Լ�����Ϊ�ڶ����������������º���
	//		vector<int> vecStrategyID;
	//		set<string> setPlotID;
	//		CInterface_SvrTradingStrategy::getObj().GetStrategyIDsByOrgID(nOrgID, vecStrategyID);
	//		for(int i=0; i<vecStrategyID.size(); i++) {
	//			TradeStrategy oStrategy = {0};
	//			//if(CInterface_SvrTradingStrategy::getObj().GetInfoByStrategyID(vecStrategyID[i], oStrategy))
	//			CInterface_SvrTradingStrategy::getObj().GetInfoByStrategyID(vecStrategyID[i], oStrategy);
	//			setPlotID.insert(oStrategy.szName);
	//		}
	//		g_dealerPlotCenter.AddDealer(userInfo.szAccount, setPlotID);
	//	}
	//}
	//	break;
	//case EventTypeUserDelete:		// ɾ��һ���û�
	//	// ����������û�ID
	//	// ͨ���û�ID����û�����Ϣ���ж��Ƿ�Ϊ����Ա
	//	// ��ý���Ա���ڵĲ��ţ�����벿����صĲ��Լ��ϡ�
	//	// ʹ�ý���Ա���ʺ�ID��Ϊ��һ�����������Լ�����Ϊ�ڶ����������������º���
	//	//g_dealerPlotCenter.DeleteDealer();
	//	//��ɾ���ʺŻ��߽��ײ��Ժ󣬱������ý���ִ��ģ��ķ�����ơ�
	//		//			CInterface_SvrTradeExcute::getObj().SetOneStrategyOrderLimit(
	//		//					*it, string(queueImport.front().instID), 
	//		//					(StrategyOrderLimit)nInstCtrl);
	//	break;
	case EventTypeTradeStrategyAdd:
	case EventTypeTradeStrategyUpdate:
	case EventTypeTradeStrategyDelete:
		ResetPlotImports();
		break;
	case EventTypeTradeStrategyAndOrgRelation:
	case EventTypeUserAndOrgRelation:
		ResetDealorPlots();
		break;
	case EventTypeStartInit:
		// ����TSS
		Reinit();
		break;
	default: 
		break;
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
		case CMDID_HEARTBEAT:
		{
			break;
		}
	}
}

void LoadDBData(map<string, set<string>>& mapDealerPlots, vector<CPlotImport::PLOTCFGITEM>& vecPlotData)
{
	// �������в���

	std::vector<TradeStrategy> vecStrategy;
	map<int, string> mapPlotID2Key;
	CPlotImport::PLOTCFGITEM plotItem = {0};

	vecPlotData.clear();
	CInterface_SvrTradingStrategy::getObj().GetTradeStrategy(vecStrategy);
	for(int i=0; i<vecStrategy.size(); i++) {
		strncpy(plotItem.plotID, vecStrategy[i].szName, sizeof(plotItem.plotID)-1);
		strncpy(plotItem.path, vecStrategy[i].szAddr, sizeof(plotItem.path)-1);
		plotItem.ReadInval = vecStrategy[i].nFrequency;
		vecPlotData.push_back(plotItem);
		mapPlotID2Key.insert(make_pair(vecStrategy[i].nID, vecStrategy[i].szName));
	}


	// ���ؽ���Ա���Թ�ϵ

	vector<Organization> vecOrg;
	map<int,vector<int>> mapPlot2Org;
	map<int, set<string>> mapOrg2Plot;
	vector<UserInfo> vecUser;
	map<int,std::vector<int>>::iterator itPlot2Org;
	map<int, string>::iterator itPID2Key;
	map<int, set<string>>::iterator itOrg2Plot;

	//CInterface_SvrUserOrg::getObj().GetOrgs(vecOrg);
	CInterface_SvrTradingStrategy::getObj().GetAssetOrgStrategyRelation(mapPlot2Org);

	mapDealerPlots.clear();

	for(itPlot2Org = mapPlot2Org.begin(); itPlot2Org != mapPlot2Org.end(); itPlot2Org++) {
		for(int k=0; k<itPlot2Org->second.size(); k++) {
			itOrg2Plot = mapOrg2Plot.find(itPlot2Org->second[k]);
			if(itOrg2Plot==mapOrg2Plot.end()) {
				set<string> setPlotID;
				itPID2Key = mapPlotID2Key.find(itPlot2Org->first);
				if(itPID2Key != mapPlotID2Key.end()) {
					setPlotID.insert(itPID2Key->second);
					mapOrg2Plot.insert(make_pair(itPlot2Org->second[k], setPlotID));
				}
			}
			else {
				itPID2Key = mapPlotID2Key.find(itPlot2Org->first);
				if(itPID2Key != mapPlotID2Key.end()) {
					itOrg2Plot->second.insert(itPID2Key->second);
				}
			}
		}
	}

	for(itOrg2Plot = mapOrg2Plot.begin(); itOrg2Plot != mapOrg2Plot.end(); itOrg2Plot++) {
		CInterface_SvrUserOrg::getObj().GetUserInfosByOrgID(itOrg2Plot->first, vecUser);
		for(int k=0; k<vecUser.size(); k++) {
			if(vecUser[k].nUserType!=USER_TYPE_TRADER)
				continue;
			mapDealerPlots.insert(make_pair(vecUser[k].szAccount, itOrg2Plot->second));
		}
	}

}
