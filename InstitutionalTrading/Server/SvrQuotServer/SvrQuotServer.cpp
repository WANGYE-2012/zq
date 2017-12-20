// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//
#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#include <math.h>
#include <windows.h>
#include "SvrQuotServer.h"
#include "CommonPkg.h"
#include "CommonMacro.h"
#include "CommonStruct.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "SvrQuotServerImpl.h"
#include "PlatformCTPQuotImpl.h"
#include "SimulateQuotMgr.h"
#include "SimpletradeSpi.h"
#include "MyXML.h"
#ifdef _Stock_
#include "..\SvrDBFTransfer\Interface_SvrDBFTransfer.h"
#endif


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
#pragma comment(lib, "SvrDBOpr.lib")
#ifdef _Stock_
#pragma comment(lib, "SvrDBFTransfer.lib")
#endif
#pragma comment(lib, "thostmduserapi.lib")	
#pragma comment(lib, "thosttraderapi.lib")




#define WRITELOGID_SvrQuotServer

//ȫ�ֻ�����
Ceasymutex			g_mutex;
Ceasymutex			g_mutexImpl;


//�̲߳���
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;
DWORD ThreadWorker(void *arg);

string g_szErrorText = "�����������������CTP....";

UINT_PTR lReinitTime=0;
UINT_PTR lConnetCTPTime=0;

bool        g_bSimulateAutoMode=true;      //�Ƿ����Զ�ģʽ


UINT_PTR g_ReLoginTime=0;
extern HANDLE g_hEvent ;
CThostFtdcTraderApi *g_pUserApi=NULL;
volatile bool bLoginCTPTrade=false;

//ģ��������������
#ifdef _SIMULATE_
    Ceasymutex	g_mutexSimulateAuto;
    HANDLE      g_hQuitSimulateAutoEvent = NULL;

    UINT_PTR    lSimulateTime=0;
    
    bool        g_bSimulateTimeAuto=false;     //�Ƿ񵽴��Զ�ģ������ʱ��

    HANDLE  g_hSimulateReadQuotThread = NULL;
    DWORD   g_idSimulateReadQuotThread = 0;
    HANDLE  g_hSimulateSendQuotThread = NULL;
    DWORD   g_idSimulateSendQuotThread = 0;

    DWORD SimulateReadQuotThreadProcess(void* lpParam);
    DWORD SimulateSendQuotThreadProcess(void* lpParam);

    void CheckAutoSimulateTime();

    queue<PlatformStru_DepthMarketData>  _queueSimulateQuot; //���ﱣ������ݿ��ж�ȡ��ÿ�������������

    void _SetSimulateQuot(const PlatformStru_DepthMarketData& pDepthMarketData);
    void _GetSimulateQuot(std::vector<PlatformStru_DepthMarketData>& vecdepthMarketData);
    int  _GetSimulateQuotSize();
#endif
    void SubscribeQuotDeal(char* pInstrument, int hSocket);
	void LoginCTPTrade();


//����һ�����յ������ݰ�
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//ģ���ʼ�������������̲߳���SvrTcp���ĸ���Ȥ�����ݰ�
SVRQUOTSERVER_API void InitFunc(void)
{   
	int nSize = sizeof(PlatformStru_DepthMarketData);

    //���������߳�
    g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);

    BrokerInfo brokerInfo;
    std::map<std::string, std::vector<std::string>> mapBrokerCode2AddrParam;

    //�����ݿ��ȡ���͹�˾��Ϣ������ǰ�û���ַ
    bool bLoadResult = CSvrQuotServerImpl::getObj().LoadConnectCtpParamFromDB(brokerInfo,mapBrokerCode2AddrParam);
    if(!bLoadResult)
    {
        LOG_INFO("��ȡ����CTP����ʧ��,������������");
    }
    else
    {
		LOG_INFO("��ȡ����CTP�����ɹ�")
        CPlatformCTPQuotImpl::GetInstance().SetCTPConnectParam(brokerInfo,mapBrokerCode2AddrParam);
        CPlatformCTPQuotImpl::GetInstance().ReInitCtpConnect();
    }

#ifdef _Stock_
	CInterface_SvrDBFTransfer::regeditCallBackPtr(&CPlatformCTPQuotImpl::GetInstance());
#endif

#ifdef _SIMULATE_       
	CSimulateQuotMgr::getObj();

    //��ȡ�����߳�
    g_hSimulateReadQuotThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)SimulateReadQuotThreadProcess,0,0,&g_idSimulateReadQuotThread);
    //���������߳�
    g_hSimulateSendQuotThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)SimulateSendQuotThreadProcess,0,0,&g_idSimulateSendQuotThread);
#endif
}

//ģ��������ͷ���Դ���رչ����߳�
SVRQUOTSERVER_API void ReleaseFunc(void)
{ 
	//�ͷ�APIʵ��
	g_pUserApi->Release();


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

        KillTimer(NULL,lReinitTime);
        KillTimer(NULL,lConnetCTPTime);
		KillTimer(NULL,g_ReLoginTime);

		if(g_hEvent)
		{
			CloseHandle(g_hEvent);
			g_hEvent =NULL;
		}

#ifdef _SIMULATE_
        KillTimer(NULL,lSimulateTime);
        lSimulateTime=0;

        SetEvent(g_hQuitSimulateAutoEvent);
        Sleep(1000);

        //���߳�
        WaitForSingleObject((HANDLE)g_hSimulateReadQuotThread,8000);
        if(GetExitCodeThread((HANDLE)g_hSimulateReadQuotThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
            TerminateThread((HANDLE)g_hSimulateReadQuotThread,0);
        CloseHandle(g_hSimulateReadQuotThread);

        g_hSimulateReadQuotThread=NULL;
        g_idSimulateReadQuotThread=0;

        //�����߳�
        WaitForSingleObject((HANDLE)g_hSimulateSendQuotThread,8000);
        if(GetExitCodeThread((HANDLE)g_hSimulateSendQuotThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
            TerminateThread((HANDLE)g_hSimulateSendQuotThread,0);
        CloseHandle(g_hSimulateSendQuotThread);

        g_hSimulateSendQuotThread=NULL;
        g_idSimulateSendQuotThread=0;

        CloseHandle(g_hQuitSimulateAutoEvent);
        g_hQuitSimulateAutoEvent = NULL;
#endif    
        
        CSvrQuotServerImpl::getObj().deleteObj();
        CPlatformCTPQuotImpl::GetInstance().DeleteInstance();
    }
}
void  DealReinitTimerWork()
{
    time_t lTime;
    struct tm * lTimeinfo = NULL; 
    time(&lTime);
    lTimeinfo = localtime(&lTime);


    struct tm  lNigthTimeinfo ;
    struct tm  lDayTimeinfo ; 	
    memcpy(&lNigthTimeinfo,lTimeinfo,sizeof(tm));
    memcpy(&lDayTimeinfo,lTimeinfo,sizeof(tm));

    sReinitTime ssTime;
    CSvrQuotServerImpl::getObj().GetReinitTime(ssTime);

    sscanf_s(ssTime.mNightTime,"%02d:%02d:%02d",&lNigthTimeinfo.tm_hour,&lNigthTimeinfo.tm_min,&lNigthTimeinfo.tm_sec);
    sscanf_s(ssTime.mDayTime,"%02d:%02d:%02d",&lDayTimeinfo.tm_hour,&lDayTimeinfo.tm_min,&lDayTimeinfo.tm_sec);

    time_t lNigthTime = mktime(&lNigthTimeinfo);
    time_t lDayTime = mktime(&lDayTimeinfo);
    time_t lDifftimeNight = lTime - lNigthTime;
    time_t lDifftimeDay = lTime - lDayTime;
    if(( 0 <= lDifftimeNight && lDifftimeNight < 1*60 )|| //1����
        (0 <= lDifftimeDay && lDifftimeDay < 1*60)  )
    {
        CPlatformCTPQuotImpl::DeleteInstance();

        //�����ݿ��ȡ���͹�˾��Ϣ
        BrokerInfo brokerInfo;
        std::map<std::string, std::vector<std::string>> mapBrokerCode2AddrParam;
        bool bLoadResult = CSvrQuotServerImpl::getObj().LoadConnectCtpParamFromDB(brokerInfo,mapBrokerCode2AddrParam);
        if(!bLoadResult)
        {
            LOG_INFO("����������,��ȡ����CTP����ʧ��,����������ã�");
        }
        else
        {    
            LOG_INFO("����������,��ȡ����CTP�����ɹ�");
            CPlatformCTPQuotImpl::GetInstance().SetCTPConnectParam(brokerInfo,mapBrokerCode2AddrParam);

            //��¼CTP
            if(!CPlatformCTPQuotImpl::GetInstance().ReInitCtpConnect())
            {
                LOG_INFO("����������,��¼CTPʧ��,������������");
            }
            else
            {
                LOG_INFO("����������,��¼CTP�ɹ�,ʱ���ǣ� %d-%d-%d %d:%d",
                    lTimeinfo->tm_year+1900,
                    lTimeinfo->tm_mon+1,
                    lTimeinfo->tm_mday,
                    lTimeinfo->tm_hour,
                    lTimeinfo->tm_min);

#ifdef _SIMULATE_
                //���¿����Զ�ģ�����
                //��ȡ�����߳�
                if(!g_hSimulateReadQuotThread)
                    g_hSimulateReadQuotThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)SimulateReadQuotThreadProcess,0,0,&g_idSimulateReadQuotThread);
                //���������߳�
                if(!g_hSimulateSendQuotThread)
                    g_hSimulateSendQuotThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)SimulateSendQuotThreadProcess,0,0,&g_idSimulateSendQuotThread);

                //����һ��ÿ1�봥��һ�εĶ�ʱ�� ����ʵʱ����Ƿ���ģ������ʱ��
                if(!lSimulateTime)
                    lSimulateTime = SetTimer(NULL,NULL,1*1*1000,(TIMERPROC)(NULL)); 
#endif


				
            }
        }

        return;
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
    //���涩�ı��̸߳���Ȥ�����ݰ�
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_HEARTBEAT,g_idThread);

    CInterface_SvrTcp::getObj().SubscribePkg(CMDID_SvrTcpDisconn,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SubscribeQuotEvent_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_UnSubscribeQuotEvent_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ReInitQuotServerEvent_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QryQuotServerTimeEvent_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QuotServerTimeResetEvent_Req,g_idThread);

    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QrySimulateDay_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QrySimulatePlayMode_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SimulatePlay_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SimulatePause_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SimulateStop_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SimulateApply_Req,g_idThread);

    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryQuotDataEvent_Req,g_idThread);

	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryQuotKLineData_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryQuotTimeLineData_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QuotSvr_Login_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SubscribeQuotEvent2_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_UnSubscribeQuotEvent2_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryVolatilityData_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QuerySettlementPrice_Req,g_idThread);


    /*test start****************************************************/
#ifdef _TEST_ 
    PostThreadMessage(g_idThread,WM_COMMAND,WndCmd_YourPkgArrival2,100);
#endif
    /*test end  ****************************************************/


    //����һ��ÿ1���Ӵ���һ�εĶ�ʱ�� ��������������  
    lReinitTime = SetTimer(NULL,NULL,1*60*1000,(TIMERPROC)(NULL));
    //����һ��ÿ30�봥��һ�εĶ�ʱ�� ����ʵʱ�����CTP����������״̬
    lConnetCTPTime = SetTimer(NULL,NULL,1*30*1000,(TIMERPROC)(NULL));
	//����һ��ÿ1���Ӵ���һ�εĶ�ʱ�� �������µ�¼����CTP 
	g_ReLoginTime = SetTimer(NULL,NULL,1*10*1000,(TIMERPROC)(NULL));
#ifdef _SIMULATE_
    //����һ��ÿ6�봥��һ�εĶ�ʱ�� ����ʵʱ����Ƿ���ģ������ʱ��
    lSimulateTime = SetTimer(NULL,NULL,1*6*1000,(TIMERPROC)(NULL)); 
#endif

    MSG Msg;
    while(GetMessage(&Msg,NULL,0,0))
    {
        if(Msg.message == WM_TIMER )
        {
            if(Msg.wParam == lReinitTime)
                DealReinitTimerWork();
			else if (Msg.wParam == g_ReLoginTime)
			{
				time_t timeLogin=CPlatformCTPQuotImpl::GetInstance().GetLoginQuotTime();
				if( CPlatformCTPQuotImpl::GetInstance().GetLoginQuotTime() > 0  && bLoginCTPTrade ==false )
				{
					LOG_INFO("------>>>��¼CTP���׶�ʱ��,��¼����ʱ��%d, g_pUserApi=%d,bLoginCTPTrade=%d",timeLogin,g_pUserApi,bLoginCTPTrade);
				    LoginCTPTrade();
				}
			}
#ifdef _SIMULATE_           
            else if(Msg.wParam == lSimulateTime)
                CheckAutoSimulateTime();
#endif
            else if(Msg.wParam == lConnetCTPTime)
            {
                eCTPCONNSTATUS eValue=CPlatformCTPQuotImpl::GetInstance().GetConnStatus();

                switch(eValue)
                {
                case CTPCONNSTATUS_Disconnected://   =      0       ,           //δ����
                case CTPCONNSTATUS_LoginFailure://   =      3       ,           //��¼ʧ��
                case CTPCONNSTATUS_UnInit:      //   =      8                   //δ��ʼ��
                    CPlatformCTPQuotImpl::GetInstance().ReInitCtpConnect();
                    break;
                case CTPCONNSTATUS_Connecting:  //   =      1       ,           //��������
                    if(eValue=CTPCONNSTATUS_Connecting)
                        LOG_INFO("��������CTP  ...");
                    break;
                case CTPCONNSTATUS_Logining:    //   =      2       ,           //���ڵ�¼
                    if(eValue=CTPCONNSTATUS_Logining)
                        LOG_INFO("���ڵ�¼CTP  ...");
                    break;
                case CTPCONNSTATUS_Connected:   //   =      6       ,           //���ӵ�¼�ɹ�
                    if(eValue=CTPCONNSTATUS_Connected)
                        LOG_INFO("���ӵ�¼CTP�ɹ�  ...");
                    break;
                case CTPCONNSTATUS_Disconnecting://  =      7       ,           //���ڶϿ�����
                    if(eValue=CTPCONNSTATUS_Disconnecting)
                        LOG_INFO("���ڶϿ�CTP����  ...");
                    break;
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
                DealPakage(EventParamID,PkgLen,hSocket);

                //�ͷ�EventParam
                CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
            }

            /*test start****************************************************/
#ifdef _TEST_ 
            if(EventParamID == 100)
            {
                Stru_UniPkgHead PkgHead;
                PkgHead.cmdid = Cmd_SubscribeQuotEvent_Req;
                char *szInst[4] = {"IF1403","IF1406","IF1409","TF1406"};
                for(int i=0;i<4;i++)
                {                    
                    ProcessOneUniPkg_InThread(NULL,PkgHead,szInst[i]);
                }
            }
#endif
            /*test end  ****************************************************/
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
			//LOG_INFO("------>>>CMDID_HEARTBEAT");
			//CInterface_SvrTcp::getObj().SendPkgData(hSocket,CMDID_HEARTBEATRSP,NULL ,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
			break;
		}
    case CMDID_SvrTcpDisconn:
        {
			LOG_INFO("------>>>CMDID_SvrTcpDisconn");
            CPlatformCTPQuotImpl::GetInstance().DeleteClientSocketFromMap(hSocket);
            break;
        }
	case Cmd_QuotSvr_Login_Req:
		{
			LOG_INFO("------>>>APP�ն˵�¼Cmd_QuotSvr_Login_Req hSocket=%d",hSocket);
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(sLoginReq),hSocket,Cmd_QuotSvr_Login_Rsp,PkgHead.seq);

			const sLoginReq * lpLogin = reinterpret_cast<const sLoginReq*>(pPkgData);

			BREAK_ON_NULL_AND_SEND_ERROR_RSP(lpLogin,hSocket,Cmd_QuotSvr_Login_Rsp,PkgHead.seq);

			//�ж�����			
			CF_ERROR lRet = CPlatformCTPQuotImpl::GetInstance().IsValidUser(lpLogin->msUserName,
				lpLogin->msPassWord);
			LOG_INFO("------>>>APP�ն˵�¼Cmd_QuotSvr_Login_Req �û���=%s,����=%s,��֤ͨ��=%d",lpLogin->msUserName,lpLogin->msPassWord,lRet);
			if(CF_ERROR_SUCCESS != lRet)
			{
				//���ʹ���ش�
				SendRspError(hSocket,
					Cmd_QuotSvr_Login_Rsp,
					PkgHead.seq,
					lRet,
					lRet,
					0,0,0);				
				break;
			}
			else
			{    
				CPlatformCTPQuotImpl::GetInstance().SetValidSocket(hSocket);
				LOG_INFO("------>>>APP�ն˵�¼Cmd_QuotSvr_Login_Req SetValidSocket=%d",hSocket);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QuotSvr_Login_Rsp,NULL ,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
			}


			
			break;
		}
	case Cmd_SubscribeQuotEvent2_Req:
		{    
			LOG_INFO("------>>>����֤�ն˶�������");

			if( !CPlatformCTPQuotImpl::GetInstance().IsValidSocket(hSocket))
			{

				LOG_INFO("------>>>����֤�ն˶�������, hSocket=%d ��Ч",hSocket);

				//���ʹ���ش�
				SendRspError(hSocket,
					Cmd_SubscribeQuotEvent2_Rsp,
					PkgHead.seq,
					CF_ERROR_LOGIN_QUOTSERVER_ERROR,
					CF_ERROR_LOGIN_QUOTSERVER_ERROR,
					0,0,0);				
				break;
			}

			char* pInstrument = (char*)(pPkgData);
			LOG_INFO("------>>>����֤�ն˶�������,Instrument===%s hSocket=%d",pInstrument,hSocket);

			if( !CPlatformCTPQuotImpl::GetInstance().IsValidInstrument(pInstrument))
			{
				LOG_INFO("------>>>����֤�ն˶�������, pInstrument=%s ��Ч",pInstrument);

				//���ʹ���ش�
				SendRspError(hSocket,
					Cmd_SubscribeQuotEvent2_Rsp,
					PkgHead.seq,
					CF_ERROR_NO_INSTRUMENTS,
					CF_ERROR_NO_INSTRUMENTS,
					0,0,0);				
				break;
			}

			/*SubscribeQuotDeal( pInstrument,hSocket);*/
			//CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SubscribeQuotEvent2_Rsp, NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);	

			LOG_INFO("------>>>����֤�ն˶�������,SendPkgData  hSocket=%d ",hSocket);
			std::map<std::string,std::set<SOCKET>> vec;
			CPlatformCTPQuotImpl::GetInstance().GetAppSubscribedInst2Socket(vec);
			std::map<std::string,std::set<SOCKET>>::iterator it_inst=vec.find(pInstrument);
			if(it_inst==vec.end())
			{
				std::set<SOCKET> sset;
				sset.insert(hSocket);
				vec[pInstrument]=sset;
			}
			else
			{
				std::set<SOCKET>& sset=it_inst->second;
				sset.insert(hSocket);
			}	

			CPlatformCTPQuotImpl::GetInstance().SetAppSubscribedInst2Socket(vec);	
			CPlatformCTPQuotImpl::GetInstance().PushLastQuot(pInstrument,hSocket);
			break;
		}

	case Cmd_QueryQuotKLineData_Req:
		{     
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(KLineQuery),hSocket,Cmd_QueryQuotKLineData_Rsp,PkgHead.seq);

			if(CSvrQuotServerImpl::getObj().GetRestLoadkLineDataThread())
			{
                g_szErrorText = "������������ڳ�ʼ����Ŭ������K����ʷ���ݣ�... ���Ժ����ԣ�";
                LOG_INFO("Cmd_QueryQuotKLineData_Req ����ʧ��");
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryQuotKLineData_Rsp,(char*)g_szErrorText.c_str(),strlen(g_szErrorText.c_str()),PkgHead.seq,0,CF_ERROR_SUCCESS-1);
		
				break;
			}

			KLineQuery * pData = (KLineQuery*)(pPkgData);

			BREAK_ON_NULL_AND_SEND_ERROR_RSP(pData,hSocket,Cmd_QueryQuotKLineData_Rsp,PkgHead.seq);

			char s[32],e[32];
			strftime(s,sizeof(s),"%Y-%m-%d %H:%M:%S",localtime(&pData->szStartTime));
			strftime(e,sizeof(e),"%Y-%m-%d %H:%M:%S",localtime(&pData->szEndTime));
			LOG_INFO("------>>>K������,Instrument=%s DataType=%d Cycle=%d startTime=(%I64d)%s, endTime=(%I64d)%s hSocket=%d",
				pData->InstrumentID,pData->nDataType, (int)pData->nCycle, pData->szStartTime, s, pData->szEndTime, e, hSocket);

			CF_ERROR lError = CF_ERROR_SUCCESS;
			CSvrQuotServerImpl::getObj().QueryKlineData(pData,lError,hSocket,PkgHead.seq);

			break;
		}
	case Cmd_QueryQuotTimeLineData_Req:
		{     
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(KLineQuery),hSocket,Cmd_QueryQuotTimeLineData_Rsp,PkgHead.seq);

			if(CSvrQuotServerImpl::getObj().GetRestLoadkLineDataThread())
			{
                g_szErrorText = "������������ڳ�ʼ����Ŭ������K����ʷ���ݣ�... ���Ժ����ԣ�";
                LOG_INFO("Cmd_QueryQuotTimeLineData_Req ����ʧ��");
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryQuotTimeLineData_Rsp,(char*)g_szErrorText.c_str(),strlen(g_szErrorText.c_str()),PkgHead.seq,0,CF_ERROR_SUCCESS-1);
		
				break;
			}

			KLineQuery * pData = (KLineQuery*)(pPkgData);

			BREAK_ON_NULL_AND_SEND_ERROR_RSP(pData,hSocket,Cmd_QueryQuotTimeLineData_Rsp,PkgHead.seq);

			LOG_INFO("------>>>��ʱ������,Instrument=%s DataType=%d Cycle=%d startTime=%d, endTime=%d hSocket=%d",
				pData->InstrumentID,pData->nDataType, (int)pData->nCycle, pData->szStartTime, pData->szEndTime,hSocket);

			CF_ERROR lError = CF_ERROR_SUCCESS;
			CSvrQuotServerImpl::getObj().QueryKlineData(pData,lError,hSocket,PkgHead.seq);


			break;
		}

	case Cmd_QuerySettlementPrice_Req:
		{     
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(KLineQuery),hSocket,Cmd_QuerySettlementPrice_Rsp,PkgHead.seq);

			KLineQuery * pData = (KLineQuery*)(pPkgData);

			BREAK_ON_NULL_AND_SEND_ERROR_RSP(pData,hSocket,Cmd_QuerySettlementPrice_Rsp,PkgHead.seq);

			if( !CPlatformCTPQuotImpl::GetInstance().IsValidSocket(hSocket))
			{

				//���ʹ���ش�
				SendRspError(hSocket,
					Cmd_QuerySettlementPrice_Rsp,
					PkgHead.seq,
					CF_ERROR_LOGIN_QUOTSERVER_ERROR,
					CF_ERROR_LOGIN_QUOTSERVER_ERROR,
					0,0,0);				
				break;
			}


			LOG_INFO("------>>>���������,Instrument===%s hSocket=%d",pData->InstrumentID,hSocket);

			vector<SettlementPrice> outData;
			CF_ERROR lError = CF_ERROR_SUCCESS;
			CSvrQuotServerImpl::getObj().QuerySettlementPrice(pData,lError,outData);

			if(outData.size()>0) 
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,
					Cmd_QuerySettlementPrice_Rsp,
					&outData[0] ,
					outData.size()*sizeof(SettlementPrice),
					PkgHead.seq,
					0,
					CF_ERROR_SUCCESS);
			}
			else 
			{
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,
					Cmd_QuerySettlementPrice_Rsp,
					NULL, 0,
					PkgHead.seq,
					0,
					CF_ERROR_SUCCESS);
			}

			break;
		}

	case Cmd_QueryVolatilityData_Req:
		{     
			LOG_INFO("------>>>����������");
			BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(Volatility),hSocket,Cmd_QueryVolatilityData_Rsp,PkgHead.seq);

			Volatility * pData = (Volatility*)(pPkgData);

			BREAK_ON_NULL_AND_SEND_ERROR_RSP(pData,hSocket,Cmd_QueryVolatilityData_Rsp,PkgHead.seq);

			
			LOG_INFO("------>>>����������,Instrument===%s TradingDay=%s",pData->InstrumentID,pData->TradingDay);

			// std::vector<KLineData> vKLineData;
			CF_ERROR lError = CSvrQuotServerImpl::getObj().QueryVolatilityData(*pData);
			
			if(CF_ERROR_SUCCESS != lError)
			{
				//���ʹ���ش�
				SendRspError(hSocket,
					Cmd_QueryVolatilityData_Rsp,
					PkgHead.seq,
					lError,
					lError,
					0,0,0);				
				break;
			}
			else
			{    
				CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryVolatilityData_Rsp,pData ,sizeof(Volatility),PkgHead.seq,0,CF_ERROR_SUCCESS);
			}

			break;
		}
	case Cmd_UnSubscribeQuotEvent2_Req:
		{
			if( !CPlatformCTPQuotImpl::GetInstance().IsValidSocket(hSocket))
			{

				//���ʹ���ش�
				SendRspError(hSocket,
					Cmd_UnSubscribeQuotEvent2_Rsp,
					PkgHead.seq,
					CF_ERROR_LOGIN_QUOTSERVER_ERROR,
					CF_ERROR_LOGIN_QUOTSERVER_ERROR,
					0,0,0);				
				break;
			}
			char* pInstrument = (char*)(pPkgData);
			CPlatformCTPQuotImpl::GetInstance().UnSubscribeAppClientMarketData(pInstrument, hSocket);

			break;
		}
    case Cmd_SubscribeQuotEvent_Req:
        {     
			if(CSvrQuotServerImpl::getObj().GetRestLoadkLineDataThread())
			{
                g_szErrorText = "������������ڳ�ʼ����Ŭ������K����ʷ���ݣ�... ���Ժ����ԣ�";
                LOG_INFO("Cmd_SubscribeQuotEvent_Req ��������ʧ��");
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_SubscribeQuotEvent_Rsp,(char*)g_szErrorText.c_str(),strlen(g_szErrorText.c_str()),PkgHead.seq,0,CF_ERROR_SUCCESS-1);
		
				break;
			}

            char* pInstrument = (char*)(pPkgData);
            LOG_INFO("------>>>��������������,Instrument===%s hSocket=%d",pInstrument,hSocket);

            //Sleep(1000);
            //�Ѿ����ĵĺ�Լ�����ٴ���CTP�ύ�������Բ�������OnRspSubMarketData�ص�������Ҫ�����ﷵ������

			SubscribeQuotDeal( pInstrument,hSocket);


           /* vector<string> vetInst = CPlatformCTPQuotImpl::GetInstance().GetSubscribedInstrumentIDs();
            vector<string>::iterator it_sub=vetInst.begin();
            for (;it_sub!=vetInst.end();++it_sub)
            {
                if(strcmp(pInstrument,(*it_sub).c_str()) == 0)
                {
                    CThostFtdcSpecificInstrumentField field;
                    strncpy(field.InstrumentID,pInstrument,sizeof(field.InstrumentID));

                    LOG_INFO("------>>>�����������Ͷ������鷵��,Instrument===%s hSocket=%d",pInstrument,hSocket);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SubscribeQuotEvent_Rsp,(void*)&field,sizeof(CThostFtdcSpecificInstrumentField),0,0,CF_ERROR_SUCCESS);			
                    goto NEXT;
                }
            }


NEXT:
            std::map<std::string,std::set<SOCKET>> vec;
            CPlatformCTPQuotImpl::GetInstance().GetSubscribedInst2Socket(vec);
            std::map<std::string,std::set<SOCKET>>::iterator it_inst=vec.find(pInstrument);
            if(it_inst==vec.end())
            {
                std::set<SOCKET> sset;
                sset.insert(hSocket);
                vec[pInstrument]=sset;
            }
            else
            {
                std::set<SOCKET>& sset=it_inst->second;
                sset.insert(hSocket);
            }	

            CPlatformCTPQuotImpl::GetInstance().SetSubscribedInst2Socket(vec);	
            CPlatformCTPQuotImpl::GetInstance().SubscribeClientMarketData(pInstrument,hSocket);*/

            break;
        }
    case Cmd_UnSubscribeQuotEvent_Req:
        {
            char* pInstrument = (char*)(pPkgData);
            CPlatformCTPQuotImpl::GetInstance().UnSubscribeClientMarketData(pInstrument, hSocket);

            break;
        }
    case Cmd_ReInitQuotServerEvent_Req:
        {  
            CPlatformCTPQuotImpl::DeleteInstance();

            //�����ݿ��ȡ���͹�˾��Ϣ
            BrokerInfo brokerInfo;
            std::map<std::string, std::vector<std::string>> mapBrokerCode2AddrParam;
            bool bLoadResult = CSvrQuotServerImpl::getObj().LoadConnectCtpParamFromDB(brokerInfo,mapBrokerCode2AddrParam);
            if(!bLoadResult)
            {
                g_szErrorText = "��ȡ����CTP����ʧ��,����������ã�";
                LOG_INFO("Cmd_ReInitQuotServerEvent_Req ��ȡ����CTP����ʧ��,����������ã�");
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_ReInitQuotServerEvent_Rsp,(char*)g_szErrorText.c_str(),strlen(g_szErrorText.c_str()),PkgHead.seq,0,CF_ERROR_SUCCESS-1);
            }
            else
            {      
                LOG_INFO("Cmd_ReInitQuotServerEvent_Req ��ȡ����CTP�����ɹ�"); 
                CPlatformCTPQuotImpl::GetInstance().SetCTPConnectParam(brokerInfo,mapBrokerCode2AddrParam);

                //��¼CTP
                if(!CPlatformCTPQuotImpl::GetInstance().ReInitCtpConnect())
                {
                    g_szErrorText = "��¼CTPʧ��,���������������������";
                    LOG_INFO("Cmd_ReInitQuotServerEvent_Req ��¼CTPʧ��,������������");
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_ReInitQuotServerEvent_Rsp,(char*)g_szErrorText.c_str(),strlen(g_szErrorText.c_str()),PkgHead.seq,0,CF_ERROR_SUCCESS-1);
                }
                else
                {     
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_ReInitQuotServerEvent_Rsp,NULL ,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
                }
            }
            break;
        }
    case Cmd_QryQuotServerTimeEvent_Req:
        {
            sReinitTime ls;
            memset(&ls,0,sizeof(ls));
            CF_ERROR lError = CSvrQuotServerImpl::getObj().GetReinitTime(ls);
            if(lError == CF_ERROR_SUCCESS)
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,
                    Cmd_QryQuotServerTimeEvent_Rsp,
                    &ls ,
                    sizeof(sReinitTime),
                    PkgHead.seq,
                    0,
                    CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,
                    Cmd_QryQuotServerTimeEvent_Rsp,
                    &lError ,
                    sizeof(int),
                    PkgHead.seq,
                    0,
                    lError);
            }

            break;
        }
    case Cmd_QuotServerTimeResetEvent_Req:
        {
            if(PkgHead.len != sizeof(sReinitTime) || pPkgData == NULL)
            {
                SendRspError(hSocket,
                    Cmd_QuotServerTimeResetEvent_Rsp,
                    PkgHead.seq,
                    CF_ERROR_COMMON_INPUT_PARAM,
                    PkgHead.userdata1,
                    PkgHead.userdata2,
                    CF_ERROR_COMMON_INPUT_PARAM,0);
                break;
            }

            sReinitTime *lsReinitTime = (sReinitTime*)pPkgData;
            CF_ERROR lError = CSvrQuotServerImpl::getObj().SetReinitTime(*lsReinitTime);
            if(lError == CF_ERROR_SUCCESS)
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,
                    Cmd_QuotServerTimeResetEvent_Rsp,
                    NULL ,
                    0,
                    PkgHead.seq,
                    0,
                    CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,
                    Cmd_QuotServerTimeResetEvent_Rsp,
                    &lError ,
                    sizeof(int),
                    PkgHead.seq,
                    0,
                    lError);
            }
            break;
        }
    /////////////////////////////ģ�����/////////////////////////////////////////////
    case Cmd_QrySimulateDay_Req:
        {
            sSimulateDay ls;
            memset(&ls,0,sizeof(ls));
            int len = sizeof(sSimulateDay);
            CF_ERROR lError = CSimulateQuotMgr::getObj().GetSimulateDay(ls);
            if(lError == CF_ERROR_SUCCESS)
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,
                    Cmd_QrySimulateDay_Rsp,
                    &ls ,
                    len,
                    PkgHead.seq,
                    0,
                    CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,
                    Cmd_QrySimulateDay_Rsp,
                    &lError ,
                    sizeof(int),
                    PkgHead.seq,
                    0,
                    lError);
            }

            break;
        }
    case Cmd_QrySimulatePlayMode_Req:
        {
            eSimulatePlayMode eMode;
            memset(&eMode,0,sizeof(eMode));
            CF_ERROR lError = CSimulateQuotMgr::getObj().GetSimulatePlayMode(eMode);
            if(lError == CF_ERROR_SUCCESS)
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,
                    Cmd_QrySimulatePlayMode_Rsp,
                    &eMode ,
                    sizeof(int),
                    PkgHead.seq,
                    0,
                    CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,
                    Cmd_QrySimulatePlayMode_Rsp,
                    &lError ,
                    sizeof(int),
                    PkgHead.seq,
                    0,
                    lError);
            }

            break;
        }
    case Cmd_SimulateApply_Req:
        {
            int nLen = sizeof(sSimulateDay)+sizeof(int);
            if(PkgHead.len != nLen || pPkgData == NULL)
            {
                SendRspError(hSocket,
                    Cmd_SimulateApply_Rsp,
                    PkgHead.seq,
                    CF_ERROR_COMMON_INPUT_PARAM,
                    PkgHead.userdata1,
                    PkgHead.userdata2,
                    CF_ERROR_COMMON_INPUT_PARAM,0);
                break;
            }

            sSimulateDay *lsSimulateDay = (sSimulateDay*)pPkgData;
            CF_ERROR lError = CSimulateQuotMgr::getObj().SetSimulateDay(*lsSimulateDay);
            if(lError == CF_ERROR_SUCCESS)
            {
                int nSimulatePlayMode = *(int*)((char*)pPkgData+sizeof(sSimulateDay));
                eSimulatePlayMode eMode = (0==nSimulatePlayMode)?SimulatePlayMode_Normal:SimulatePlayMode_NoDelay;
                CF_ERROR lError = CSimulateQuotMgr::getObj().SetSimulatePlayMode(eMode);
                if(lError == CF_ERROR_SUCCESS)
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,
                        Cmd_SimulateApply_Rsp,
                        NULL ,
                        0,
                        PkgHead.seq,
                        0,
                        CF_ERROR_SUCCESS);
                }
            }

            CInterface_SvrTcp::getObj().SendPkgData(hSocket,
                Cmd_SimulateApply_Rsp,
                &lError ,
                sizeof(int),
                PkgHead.seq,
                0,
                lError);

            break;
        }
    case Cmd_SimulatePlay_Req:
        {
            //�ֶ����ˣ��Զ�ʧЧ
#ifdef _SIMULATE_
            KillTimer(NULL,lSimulateTime);
            lSimulateTime=0;

            SetEvent(g_hQuitSimulateAutoEvent);
            Sleep(1000);

            //���߳�   
            DWORD ExitCode;
            WaitForSingleObject((HANDLE)g_hSimulateReadQuotThread,8000);
            if(GetExitCodeThread((HANDLE)g_hSimulateReadQuotThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
                TerminateThread((HANDLE)g_hSimulateReadQuotThread,0);
            CloseHandle(g_hSimulateReadQuotThread);

            g_hSimulateReadQuotThread=NULL;
            g_idSimulateReadQuotThread=0;

            //�����߳�
            WaitForSingleObject((HANDLE)g_hSimulateSendQuotThread,8000);
            if(GetExitCodeThread((HANDLE)g_hSimulateSendQuotThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
                TerminateThread((HANDLE)g_hSimulateSendQuotThread,0);
            CloseHandle(g_hSimulateSendQuotThread);

            g_hSimulateSendQuotThread=NULL;
            g_idSimulateSendQuotThread=0;

            CloseHandle(g_hQuitSimulateAutoEvent);
            g_hQuitSimulateAutoEvent = NULL;
#endif    
            g_bSimulateAutoMode = false;
            CSimulateQuotMgr::getObj().SimulateQuotPlay();

            CInterface_SvrTcp::getObj().SendPkgData(hSocket,
                Cmd_SimulatePlay_Rsp,
                NULL ,
                0,
                PkgHead.seq,
                0,
                CF_ERROR_SUCCESS);

            break;
        }
    case Cmd_SimulatePause_Req:
        {
            CSimulateQuotMgr::getObj().SimulateQuotPause();

            CInterface_SvrTcp::getObj().SendPkgData(hSocket,
                Cmd_SimulatePause_Rsp,
                NULL ,
                0,
                PkgHead.seq,
                0,
                CF_ERROR_SUCCESS);
            
            break;
        }
    case Cmd_SimulateStop_Req:
        {
            CSimulateQuotMgr::getObj().SimulateQuotStop();

            CInterface_SvrTcp::getObj().SendPkgData(hSocket,
                Cmd_SimulateStop_Rsp,
                NULL ,
                0,
                PkgHead.seq,
                0,
                CF_ERROR_SUCCESS);           

            break;
        }
    case Cmd_QueryQuotDataEvent_Req:
        {
            std::string szTime = (string)(char*)pPkgData;
            std::vector<PlatformStru_DepthMarketData_EX> vMarketData;

            CF_ERROR lError = CF_ERROR_SUCCESS;
            CSvrQuotServerImpl::getObj().QueryQuotData(szTime,vMarketData,lError);
            //if(lError == CF_ERROR_SUCCESS)
            //{
			if(vMarketData.size()>0) {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,
                    Cmd_QueryQuotDataEvent_Rsp,
                    &vMarketData[0] ,
                    vMarketData.size()*sizeof(PlatformStru_DepthMarketData_EX),
                    PkgHead.seq,
                    0,
                    CF_ERROR_SUCCESS);
			}
			else {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,
                    Cmd_QueryQuotDataEvent_Rsp,
                    NULL, 0,
                    PkgHead.seq,
                    0,
                    CF_ERROR_SUCCESS);
			}
            //}
            //else
            //{
            //    CInterface_SvrTcp::getObj().SendPkgData(hSocket,
            //        Cmd_QueryQuotDataEvent_Rsp,
            //        &lError ,
            //        sizeof(int),
            //        PkgHead.seq,
            //        0,
            //        lError);
            //}

            break;
        }
    }
}



#ifdef _SIMULATE_
DWORD SimulateReadQuotThreadProcess(void* lpParam)
{    
    int N;//׷��ǰ3��
    int nTimeSS=0;
    bool bFind=false;
    char szTableName[19]; //AQUOTINFO_20140225
    char szLocalDate[11];
    char szValiDatedate[11];
    char szFirstUpdateTime[9];
    char szNextUpdateTime[9];
    memset(szLocalDate,0,sizeof(szLocalDate));
    memset(szValiDatedate,0,sizeof(szValiDatedate));
    memset(szFirstUpdateTime,0,sizeof(szFirstUpdateTime));

    //ȡ��ǰ����ϵͳ������
    time_t timer; 
    time(&timer);
    tm* t_tm = localtime(&timer); 
    switch(t_tm->tm_wday)
    {
    case Monday:
    case Tuesday:
    case Wednesday:
    case Thursday:
    case Friday:
        N=0;
        break;
    case Saturday:
        N=1;
        break;
    case Sunday:
        N=2;
        break;
    }

    int nErrorCode = CF_ERROR_SUCCESS;
    char szBuffer[MAX_SQL_LENGTH];    
    std::vector<std::vector<_variant_t>> vNode;

    while (1)
    {
        Sleep(200);
        if(WaitForSingleObject(g_hQuitSimulateAutoEvent,0)==WAIT_OBJECT_0)
        {
            LOG_INFO("�Զ�ģ����������˳����߳�");
            return 1;
        }

        int nCount = 0;
        while( g_bSimulateTimeAuto && nCount<4 && !bFind ) 
        {  
            time_t cur_time = time(NULL) - N*24*60*60;
            t_tm = gmtime(&cur_time);

            if(t_tm->tm_wday == Saturday || t_tm->tm_wday == Sunday)
            {
                N++;
                nCount++;
                continue;
            }

            memset(szLocalDate,0,sizeof(szLocalDate));
            sprintf(szLocalDate,"%.4d%.2d%.2d",t_tm->tm_year+1900, t_tm->tm_mon+1, t_tm->tm_mday); 

            memset(szTableName,0,sizeof(szTableName));
            sprintf(szTableName,"%s_%s",QUOTTABLENAME,szLocalDate);


            //��9:00��ʼ
            memset(szBuffer, 0, sizeof(szBuffer));
            sprintf(szBuffer, "select  tt.validatedate, tt.updatetime from (select t.validatedate, t.updatetime from %s t where t.validatedate = \'%s\' and t.updatetime>='09:00:00' order by t.updatetime) tt where rownum = 1",szTableName,szLocalDate);
            LOG_INFO("%s",szBuffer);

            if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
            {
                N++;
                nCount++;
                continue;
            }
            else
            {
                std::vector<_variant_t> vValue = vNode[0]; 
                strcpy(szValiDatedate, vValue[0].operator _bstr_t());  //VALIDATEDATE 2014-01-14
                strcpy(szFirstUpdateTime, vValue[1].operator _bstr_t()); //UPDATETIME 09:00:00
                LOG_INFO("--------%s %s",szValiDatedate,szFirstUpdateTime);

                char *pPtr = szFirstUpdateTime;

                char szHH[3],szMM[3],szSS[3];
                memset(szHH,0,sizeof(szHH));
                memset(szMM,0,sizeof(szMM));
                memset(szSS,0,sizeof(szSS));

                memcpy(szHH,pPtr,2);
                pPtr += 3;
                memcpy(szMM,pPtr,2);
                pPtr += 3;
                memcpy(szSS,pPtr,2);

                int nHH = atoi(szHH);
                int nMM = atoi(szMM);
                int nSS = atoi(szSS);

                nTimeSS = nHH*60*60+nMM*60+nSS; 
                bFind = true;
                break;
            }  
        }

        if(g_bSimulateTimeAuto && !bFind)
        {
            LOG_INFO("���ݿ���û���������ݣ��޷�����ģ�ⷢ�����鹦��,ֱ����ȡ����������Ϊֹ��");
            N=0;
            Sleep(1000);
            continue;
        }

        //ÿ��ȡ60�����������
        while (g_bSimulateTimeAuto)
        {  
            if(WaitForSingleObject(g_hQuitSimulateAutoEvent,0)==WAIT_OBJECT_0)
            {
                LOG_INFO("�Զ�ģ����������˳����߳�");
                return 1;
            }

            if(_GetSimulateQuotSize()>10000) //�����в�����10000������,����10000��ʱ���ȴ�30s
            {
                Sleep(30*1000);
                LOG_INFO("_GetSimulateQuotSize()>10000");
                continue;
            }

            memset(szNextUpdateTime,0,sizeof(szNextUpdateTime));
            nTimeSS+=1*60;
            int shi = (nTimeSS/3600)%24;    
            int fen = nTimeSS/60%60;    
            int miao = nTimeSS%60;
            sprintf(szNextUpdateTime,"%.2d:%.2d:%.2d",shi,fen,miao);

            memset(szBuffer, 0, sizeof(szBuffer));
            sprintf(szBuffer, "select * from %s t where t.updatetime between \'%s\' and \'%s\' and t.validatedate = \'%s\' order by t.updatetime",szTableName,szFirstUpdateTime,szNextUpdateTime,szValiDatedate);
            LOG_INFO("%s",szBuffer);

            memcpy(szFirstUpdateTime,szNextUpdateTime,sizeof(szFirstUpdateTime));

            std::vector<std::vector<_variant_t>> vNode;
            if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
                continue;
            else
            {
                CeasymutexGuard guard(g_mutexSimulateAuto);
                LOG_INFO("=====%s CONUT(%d)",szBuffer,(int)(vNode.size()));

                for ( UINT j = 0; j < vNode.size(); j++ )
                {
                    std::vector<_variant_t> vValue = vNode[j];

                    PlatformStru_DepthMarketData depthMarketData;

                    int i = 1;
                    strcpy(depthMarketData.TradingDay, vValue[i++].operator _bstr_t());	
                    strcpy(depthMarketData.InstrumentID, vValue[i++].operator _bstr_t());
                    strcpy(depthMarketData.ExchangeID, vValue[i++].operator _bstr_t());
                    strcpy(depthMarketData.ExchangeInstID, vValue[i++].operator _bstr_t());
                    depthMarketData.LastPrice = vValue[i++].dblVal;
                    depthMarketData.PreSettlementPrice = vValue[i++].dblVal;
                    depthMarketData.PreClosePrice = vValue[i++].dblVal;
                    depthMarketData.PreOpenInterest = vValue[i++].dblVal;
                    depthMarketData.OpenPrice = vValue[i++].dblVal;
                    depthMarketData.HighestPrice = vValue[i++].dblVal;
                    depthMarketData.LowestPrice = vValue[i++].dblVal;
                    depthMarketData.Volume = vValue[i++].intVal;
                    depthMarketData.Turnover = vValue[i++].dblVal;
                    depthMarketData.OpenInterest = vValue[i++].dblVal;
                    depthMarketData.ClosePrice = vValue[i++].dblVal;
                    depthMarketData.SettlementPrice = vValue[i++].dblVal;
                    depthMarketData.UpperLimitPrice = vValue[i++].dblVal;
                    depthMarketData.LowerLimitPrice = vValue[i++].dblVal;
                    depthMarketData.PreDelta = vValue[i++].dblVal;
                    depthMarketData.CurrDelta = vValue[i++].dblVal;
                    strcpy(depthMarketData.UpdateTime, vValue[i++].operator _bstr_t());
                    depthMarketData.UpdateMillisec = vValue[i++].intVal;
                    depthMarketData.BidPrice1 = vValue[i++].dblVal;
                    depthMarketData.BidVolume1 = vValue[i++].intVal;
                    depthMarketData.AskPrice1 = vValue[i++].dblVal;
                    depthMarketData.AskVolume1 = vValue[i++].intVal;
                    depthMarketData.BidPrice2 = vValue[i++].dblVal;
                    depthMarketData.BidVolume2 = vValue[i++].intVal;
                    depthMarketData.AskPrice2 = vValue[i++].dblVal;
                    depthMarketData.AskVolume2 = vValue[i++].intVal;
                    depthMarketData.BidPrice3 = vValue[i++].dblVal;
                    depthMarketData.BidVolume3 = vValue[i++].intVal;
                    depthMarketData.AskPrice3 = vValue[i++].dblVal;
                    depthMarketData.AskVolume3 = vValue[i++].intVal;
                    depthMarketData.BidPrice4 = vValue[i++].dblVal;
                    depthMarketData.BidVolume4 = vValue[i++].intVal;
                    depthMarketData.AskPrice4 = vValue[i++].dblVal;
                    depthMarketData.AskVolume4 = vValue[i++].intVal;
                    depthMarketData.BidPrice5 = vValue[i++].dblVal;
                    depthMarketData.BidVolume5 = vValue[i++].intVal;
                    depthMarketData.AskPrice5 = vValue[i++].dblVal;
                    depthMarketData.AskVolume5 = vValue[i++].intVal;
                    depthMarketData.AveragePrice = vValue[i++].dblVal;
                    depthMarketData.BidPrice6 = vValue[i++].dblVal;
                    depthMarketData.BidVolume6 = vValue[i++].intVal;
                    depthMarketData.AskPrice6 = vValue[i++].dblVal;
                    depthMarketData.AskVolume6 = vValue[i++].intVal;
                    depthMarketData.BidPrice7 = vValue[i++].dblVal;
                    depthMarketData.BidVolume7 = vValue[i++].intVal;
                    depthMarketData.AskPrice7 = vValue[i++].dblVal;
                    depthMarketData.AskVolume7 = vValue[i++].intVal;
                    depthMarketData.BidPrice8 = vValue[i++].dblVal;
                    depthMarketData.BidVolume8 = vValue[i++].intVal;
                    depthMarketData.AskPrice8 = vValue[i++].dblVal;
                    depthMarketData.AskVolume8 = vValue[i++].intVal;
                    depthMarketData.BidPrice9 = vValue[i++].dblVal;
                    depthMarketData.BidVolume9 = vValue[i++].intVal;
                    depthMarketData.AskPrice9 = vValue[i++].dblVal;
                    depthMarketData.AskVolume9 = vValue[i++].intVal;
                    depthMarketData.BidPrice10 = vValue[i++].dblVal;
                    depthMarketData.BidVolume10 = vValue[i++].intVal;
                    depthMarketData.AskPrice10 = vValue[i++].dblVal;
                    depthMarketData.AskVolume10 = vValue[i++].intVal;
                    depthMarketData.NewVolume = vValue[i++].intVal;

                    _SetSimulateQuot(depthMarketData);
                }   
            }            
        }
    }

    return 1;
}
DWORD SimulateSendQuotThreadProcess(void* lpParam)
{
    int nTimeBase=0,nTimeSecond=0,nWaitTime;
    char szUpdateTime[9];
    bool bSecond = false;
    vector<PlatformStru_DepthMarketData> sVec;
    while (1)
    {
        if(WaitForSingleObject(g_hQuitSimulateAutoEvent,0)==WAIT_OBJECT_0)
        {
            LOG_INFO("�Զ�ģ����������˳������߳�");
            return 1;
        }

        _GetSimulateQuot(sVec);
        for (vector<PlatformStru_DepthMarketData>::iterator it = sVec.begin();it!=sVec.end();it++)
        {            
            if(WaitForSingleObject(g_hQuitSimulateAutoEvent,0)==WAIT_OBJECT_0)
            {
                LOG_INFO("�Զ�ģ����������˳����߳�");
                return 1;
            }

            PlatformStru_DepthMarketData data = (*it);

            memset(szUpdateTime,0,sizeof(szUpdateTime));
            memcpy(szUpdateTime,data.UpdateTime,sizeof(szUpdateTime));

            char *pPtr = szUpdateTime;

            char szHH[3],szMM[3],szSS[3];
            memset(szHH,0,sizeof(szHH));
            memset(szMM,0,sizeof(szMM));
            memset(szSS,0,sizeof(szSS));

            memcpy(szHH,pPtr,2);
            pPtr += 3;
            memcpy(szMM,pPtr,2);
            pPtr += 3;
            memcpy(szSS,pPtr,2);

            int nHH = atoi(szHH);
            int nMM = atoi(szMM);
            int nSS = atoi(szSS);

            nTimeSecond = nHH*60*60+nMM*60+nSS; 
            if(bSecond)
            {
                nWaitTime = abs(nTimeSecond-nTimeBase);
                Sleep(nWaitTime*1000);
            }

            LOG_INFO("ģ����� Inst=%s",data.InstrumentID);
            CPlatformCTPQuotImpl::GetInstance().OnRtnDepthMarketDataSimulate(&data);
            nTimeBase = nTimeSecond;
            bSecond = true;

            //����ģ���ڴ������һ����������
            CPlatformCTPQuotImpl::GetInstance().SetLastQuot( data );
        }

        sVec.clear();

        Sleep(200);
    }

    return 1;
}
void CheckAutoSimulateTime()
{    
    CeasymutexGuard guard(g_mutexSimulateAuto);

    time_t lTime;
    struct tm * lTimeinfo = NULL; 
    time(&lTime);
    lTimeinfo = localtime(&lTime);


    struct tm  lStartTimeinfo ;
    struct tm  lEndTimeinfo ; 	
    memcpy(&lStartTimeinfo,lTimeinfo,sizeof(tm));
    memcpy(&lEndTimeinfo,lTimeinfo,sizeof(tm));

    sSimulateTime ssTime;
    CSimulateQuotMgr::getObj().GetSimulateTime(ssTime);

    sscanf_s(ssTime.mStartTime,"%02d:%02d:%02d",&lStartTimeinfo.tm_hour,&lStartTimeinfo.tm_min,&lStartTimeinfo.tm_sec);
    sscanf_s(ssTime.mEndTime,"%02d:%02d:%02d",&lEndTimeinfo.tm_hour,&lEndTimeinfo.tm_min,&lEndTimeinfo.tm_sec);

    time_t lStartTime = mktime(&lStartTimeinfo);
    time_t lEndTime = mktime(&lEndTimeinfo);
    time_t lDifftime1 = lTime - lStartTime;
    time_t lDifftime2 = lTime - lEndTime;
    if( 0 <= lDifftime1 && lDifftime2 < 0 )
        g_bSimulateTimeAuto = true;
    else
        g_bSimulateTimeAuto = false;

    if(g_bSimulateTimeAuto)
        LOG_INFO("CheckAutoSimulateTime = %s",g_bSimulateTimeAuto?"true":"false");
}
void _SetSimulateQuot(const PlatformStru_DepthMarketData& pDepthMarketData)
{ 
    _queueSimulateQuot.push(pDepthMarketData);
}
void _GetSimulateQuot(std::vector<PlatformStru_DepthMarketData>& vecdepthMarketData)
{
    CeasymutexGuard guard(g_mutexSimulateAuto);

    while (!_queueSimulateQuot.empty())
    {                
        PlatformStru_DepthMarketData targetData = _queueSimulateQuot.front();
        _queueSimulateQuot.pop();

        vecdepthMarketData.push_back(targetData);
    } 
}
int _GetSimulateQuotSize()
{
    CeasymutexGuard guard(g_mutexSimulateAuto);

    return (int)(_queueSimulateQuot.size());
}

#endif

void SubscribeQuotDeal(char* pInstrument, int hSocket)
{
    vector<string> vetInst = CPlatformCTPQuotImpl::GetInstance().GetSubscribedInstrumentIDs();
    vector<string>::iterator it_sub=vetInst.begin();
    for (;it_sub!=vetInst.end();++it_sub)
    {
        if(strcmp(pInstrument,(*it_sub).c_str()) == 0)
        {
            CThostFtdcSpecificInstrumentField field;
            strncpy(field.InstrumentID,pInstrument,sizeof(field.InstrumentID));

            LOG_INFO("------>>>�����������Ͷ������鷵��,Instrument===%s hSocket=%d",pInstrument,hSocket);
            CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SubscribeQuotEvent_Rsp,(void*)&field,sizeof(CThostFtdcSpecificInstrumentField),0,0,CF_ERROR_SUCCESS);			
            goto NEXT;
        }
    }


NEXT:
    std::map<std::string,std::set<SOCKET>> vec;
    CPlatformCTPQuotImpl::GetInstance().GetSubscribedInst2Socket(vec);
    std::map<std::string,std::set<SOCKET>>::iterator it_inst=vec.find(pInstrument);
    if(it_inst==vec.end())
    {
        std::set<SOCKET> sset;
        sset.insert(hSocket);
        vec[pInstrument]=sset;
    }
    else
    {
        std::set<SOCKET>& sset=it_inst->second;
        sset.insert(hSocket);
    }	

    CPlatformCTPQuotImpl::GetInstance().SetSubscribedInst2Socket(vec);	
    CPlatformCTPQuotImpl::GetInstance().SubscribeClientMarketData(pInstrument,hSocket);
}
void LoginCTPTrade()
{

	LOG_INFO(" SvrQuotServer RLoginCTPTrade����ʱ��¼����");
	bLoginCTPTrade = false;
	ResetEvent( g_hEvent );

	g_pUserApi =CThostFtdcTraderApi::CreateFtdcTraderApi();
	SimpletradeSpi sh(g_pUserApi);
	g_pUserApi->RegisterSpi(&sh);
	LOG_INFO(" SvrQuotServer RLoginCTPTrade����¼���� g_pUserApi->RegisterSpi(&sh);");

	// ���ý����й�ϵͳ����ĵ�ַ������ע������ַ����
	string ServerAddr=string("tcp://")+CMyXML("Account.xml").GetNodeContent2("","root","CTPTradeServerAddress");

	g_pUserApi->RegisterFront((char*)ServerAddr.c_str());

	// ʹ�ͻ��˿�ʼ���̨����������
	g_pUserApi->Init();
	LOG_INFO(" SvrQuotServer  ��¼���� g_pUserApi->Init();");
	// �ͻ��˵ȴ���ѯ��Լ������� 
	WaitForSingleObject(g_hEvent, INFINITE);
	// �ͷ�APIʵ��
	g_pUserApi->Release();
	LOG_INFO(" SvrQuotServer ��¼���� g_pUserApi->Release();");

	map<std::string,PlatformStru_InstrumentInfo>   mapIns;
	sh.GetInstrument(mapIns);
	if (mapIns.size()>0)
	{
		LOG_INFO(" �ѱ��浱�պ�Լ");
		bLoginCTPTrade = true;
		CPlatformCTPQuotImpl::GetInstance().SetIns(mapIns);
	}
	else
	{

		bLoginCTPTrade = false;
	}

}