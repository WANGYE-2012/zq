// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include "SvrTradingStrategy.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "..\\SvrNotifyAndAsk\\Interface_SvrNotifyAndAsk.h"
#include "SvrTradingStrategyBusiness.h"
#include "CommonMacro.h"


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
#pragma comment(lib, "SvrNotifyAndAsk.lib")
#pragma comment(lib, "SvrUserOrg.lib")


#define WRITELOGID_SvrTradingStrategy




//ȫ�ֻ�����
Ceasymutex			g_mutex;

//�̲߳���
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;
DWORD ThreadWorker(void *arg);


//����һ�����յ������ݰ�
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);



//ģ���ʼ�������������̲߳���SvrTcp���ĸ���Ȥ�����ݰ�
SVRTRADINGSTRATEGY_API void InitFunc(void)
{
	//���������߳�
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);
	
    CInterface_SvrTradingStrategy::getObj();

    //�����ݿ�õ����ײ���
    CSvrTradingStrategyBusiness::getObj().LoadDataFromDB();
}

//ģ��������ͷ���Դ���رչ����߳�
SVRTRADINGSTRATEGY_API void ReleaseFunc(void)
{
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

        CSvrTradingStrategyBusiness::deleteObj();
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
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_AddTradeStrategy_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ModifyTradeStrategy_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_DeleteTradeStrategy_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryTradeStrategy_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SaveStrategyOrgRelation_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryStrategyOrgRelation_Req,g_idThread);

    CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_Orgnazation,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradingStrategy::getObj()));
    CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_TradeStrategy,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrTradingStrategy::getObj()));


	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{
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
    case Cmd_AddTradeStrategy_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(TradeStrategy),hSocket,Cmd_AddTradeStrategy_Rsp,PkgHead.seq);
            if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,STRATEGY_ADD))
            {
                const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddTradeStrategy_Rsp, 
                    (void*)lErrorString, 
                    strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                break;
            }

            TradeStrategy *pStrategy = (TradeStrategy*)pPkgData;

            int nPKID = 0;
            int nErrorCode = CSvrTradingStrategyBusiness::getObj().AddStrategy(PkgHead.userdata1,pStrategy, nPKID);
            if(CF_ERROR_SUCCESS != nErrorCode)
            {
                //���ʹ���ش�
                const char * lErrorString = FormatErrorCode(nErrorCode);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddTradeStrategy_Rsp, 
                                                        (void*)lErrorString, 
                                                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_AddTradeStrategy_Rsp,
                                                        (void*)&nPKID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                //����һ���㲥�¼�		
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_STRATEGY),
                                        hSocket,
                                        eSendManager);
                Stru_NotifyEvent lsEvent(EventTypeTradeStrategyAdd,nPKID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_DeleteTradeStrategy_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(int),hSocket,Cmd_DeleteTradeStrategy_Rsp,PkgHead.seq);
            if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,STRATEGY_DEL))
            {
                const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DeleteTradeStrategy_Rsp, 
                    (void*)lErrorString, 
                    strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                break;
            }

            int nStrategyID = *(int*)pPkgData;

            //��ѯ������״̬
            CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_TradeStrategy,
                                                                            AskOperationTypeDelete,
                                                                            nStrategyID,
                                                                            NULL);
            if(lRet != CF_ERROR_SUCCESS)			
            {
                //���ʹ���ش�
                SendRspError(hSocket,
                            Cmd_DeleteTradeStrategy_Rsp,
                            PkgHead.seq,
                            lRet,
                            lRet,
                            0,0,0);
                break;
            }

            int nErrorCode = CSvrTradingStrategyBusiness::getObj().DeleteStrategy(PkgHead.userdata1,nStrategyID);
            if(CF_ERROR_SUCCESS != nErrorCode)
            {
                //���ʹ���ش�
                const char * lErrorString = FormatErrorCode(nErrorCode);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DeleteTradeStrategy_Rsp, 
                                                        (void*)lErrorString, 
                                                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_DeleteTradeStrategy_Rsp,
                                            &nStrategyID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                //����һ���㲥�¼�	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_STRATEGY),
                                        hSocket,
                                        eSendManager);
                Stru_NotifyEvent lsEvent(EventTypeTradeStrategyDelete,nStrategyID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_ModifyTradeStrategy_Req:
        {            
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(TradeStrategy),hSocket,Cmd_ModifyTradeStrategy_Rsp,PkgHead.seq);
            if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,STRATEGY_MODIFY))
            {
                const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyTradeStrategy_Rsp, 
                    (void*)lErrorString, 
                    strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                break;
            }

            TradeStrategy *pStrategy = (TradeStrategy*)pPkgData;

            CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_TradeStrategy,
                                                                            AskOperationTypeUpdate,
                                                                            pStrategy->nID,
                                                                            NULL);
            if(lRet != CF_ERROR_SUCCESS)			
            {
                //���ʹ���ش�
                SendRspError(hSocket,
                    Cmd_ModifyTradeStrategy_Rsp,
                    PkgHead.seq,
                    lRet,
                    lRet,
                    0,0,0);
                break;
            }

            int nErrorCode = CSvrTradingStrategyBusiness::getObj().ModifyStrategy(PkgHead.userdata1,pStrategy);
            if(CF_ERROR_SUCCESS != nErrorCode)
            {
                //���ʹ���ش�
                const char * lErrorString = FormatErrorCode(nErrorCode);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyTradeStrategy_Rsp, 
                                                        (void*)lErrorString, 
                                                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_ModifyTradeStrategy_Rsp,
                            (void*)&(pStrategy->nID),sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                //����һ���㲥�¼�	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_STRATEGY),
                                        hSocket,
                                        eSendManager);
                Stru_NotifyEvent lsEvent(EventTypeTradeStrategyUpdate,pStrategy->nID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_QueryTradeStrategy_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QueryTradeStrategy_Rsp,PkgHead.seq);

            std::vector<TradeStrategy> vec;
            CSvrTradingStrategyBusiness::getObj().GetTradeStrategy(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryTradeStrategy_Rsp,
                            &vec[0],sizeof(TradeStrategy)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryTradeStrategy_Rsp,
                                                        NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }
    case Cmd_SaveStrategyOrgRelation_Req:
        {
            if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,STRATEGY_ASSIGN_ORGANIZATION))
            {
                const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveStrategyOrgRelation_Rsp, 
                    (void*)lErrorString, 
                    strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                break;
            }

            int nStrategyID = *(int*)pPkgData;
            int nCount = (PkgHead.len - sizeof(int)) / sizeof(int);
            std::vector<int> vec;
            for ( int i = 0; i < nCount; i++ )
            {
                int nOrgID = *(int*)((char*)pPkgData + sizeof(int) + sizeof(int)*i);
                vec.push_back(nOrgID);
            }

            //��ѯ
            CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_OrgAndStrategy,
                                                                            AskOperationTypeUpdate,
                                                                            nStrategyID,
                                                                            NULL);
            if(lRet != CF_ERROR_SUCCESS)			
            {
                //���ʹ���ش�
                SendRspError(hSocket,
                    Cmd_SaveStrategyOrgRelation_Rsp,
                    PkgHead.seq,
                    lRet,
                    lRet,
                    0,0,0);
                break;
            }

            int nErrorCode = CSvrTradingStrategyBusiness::getObj().SaveStrategyOrganizationRelation(PkgHead.userdata1,nStrategyID,vec);
            if(CF_ERROR_SUCCESS != nErrorCode)
            {
                //���ʹ���ش�
                const char * lErrorString = FormatErrorCode(nErrorCode);
                CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveStrategyOrgRelation_Rsp, 
                                                        (void*)lErrorString, 
                                                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_SaveStrategyOrgRelation_Rsp,
                                                (void*)&nStrategyID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                //����һ���㲥�¼�
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_STRATEGYANDORG),
                                        hSocket,
                                        eSendManager);
                Stru_NotifyEvent lsEvent(EventTypeTradeStrategyAndOrgRelation,nStrategyID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }

    case Cmd_QueryStrategyOrgRelation_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QueryStrategyOrgRelation_Rsp,PkgHead.seq);

            std::vector<AssetOrgStrategyRelation> vec;
            CSvrTradingStrategyBusiness::getObj().GetStrategyOrganizationRelation(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryStrategyOrgRelation_Rsp,
                    &vec[0],sizeof(AssetOrgStrategyRelation)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryStrategyOrgRelation_Rsp,
                                                        NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }
    }
}