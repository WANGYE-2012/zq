// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include "SvrFinanceProduct.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "..\\SvrNotifyAndAsk\\Interface_SvrNotifyAndAsk.h"
#include "..\\SvrUserOrg\\Interface_SvrUserOrg.h"
#include "..\\OpenSource\\tinyxml\\tinyxml.h"
#include "SvrFinanceProductBusiness.h"
#include "CommonMacro.h"
#include "DataWrap.h"
#include "MyXML.h"


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


#define WRITELOGID_SvrFinanceProduct



//ȫ�ֻ�����
Ceasymutex			g_mutex;

//�̲߳���
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;
DWORD ThreadWorker(void *arg);


//����һ�����յ����¼���
void ParseDataChangedEvent(const Stru_NotifyEvent& dataEvt);
//����һ�����յ������ݰ�
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//extend
set<int> g_setServerSocket; 
eServerRunMode g_bServerRunMode = ServerRunMode_Master;


//ģ���ʼ�������������̲߳���SvrTcp���ĸ���Ȥ�����ݰ�
SVRFINANCEPRODUCT_API void InitFunc(void)
{
    //��ȡMaster/Slave
	bool mbSlave = CMyXML("InstitutionalTrading.xml").GetNodeContentbool2(false,"root","SlaveServer");
    g_bServerRunMode = mbSlave?ServerRunMode_Slave:ServerRunMode_Master;


	//���������߳�
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);
    
    CSvrFinanceProductBusiness::getObj().SetServerRunMode(g_bServerRunMode);
    if(ServerRunMode_Master == g_bServerRunMode)
    {
        //�����ݿ��м�������
        CSvrFinanceProductBusiness::getObj().LoadDataFromDB();
    }
}

//ģ��������ͷ���Դ���رչ����߳�
SVRFINANCEPRODUCT_API void ReleaseFunc(void)
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

        CSvrFinanceProductBusiness::deleteObj();
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
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_AddFinicialProduct_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_ModifyFinicialProduct_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_DeleteFinicialProduct_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryFinicialProduct_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryProductTraderRelation_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SaveProductTraderRelation_FromUser_Req,g_idThread);
    CInterface_SvrTcp::getObj().SubscribePkg(Cmd_SaveProductTraderRelation_FromProduct_Req,g_idThread);

    CInterface_SvrNotifyAndAsk::getObj().RegistAsk(AskType_FinacialProduct,dynamic_cast<CInterface_SvrCommon*const>(&CInterface_SvrFinanceProduct::getObj()));

    if(ServerRunMode_Master == g_bServerRunMode)
    {
        CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryProductModule_Req,g_idThread);
    }
    else if(ServerRunMode_Slave == g_bServerRunMode)
    {
        CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeSlaverLoginedMaster ,  g_idThread); 
        CInterface_SvrTcp::getObj().SubscribePkg(Cmd_QueryProductModule_Rsp,g_idThread);
    }

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
                //�������ݰ��������������ݰ�
                ParseDataChangedEvent(ls);			
                //�ͷ�EventParam
                CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
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
	}
	return 0;
}

// ����һ������֪ͨ�¼�
void ParseDataChangedEvent(const Stru_NotifyEvent& dataEvt)
{
    switch(dataEvt.meEventType) 
    {
    case  EventTypeSlaverLoginedMaster:
        {
            int nSocket = dataEvt.mnDataID;
            CInterface_SvrTcp::getObj().SendPkgData(nSocket,Cmd_QueryProductModule_Req,NULL,0,dataEvt.mnValidateLen);
            break;
        }

    default: break;
    };
}

//����һ��SvrTcp���͹��������ݰ�
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	//-----------------------------------------------------------------------------------
	//	����������ݰ��������֣��������ݰ�
	//-----------------------------------------------------------------------------------
    
    switch(PkgHead.cmdid)
    {
    case Cmd_QueryProductModule_Req:
        {
            if(ServerRunMode_Slave == g_bServerRunMode)
                break;

            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QueryProductModule_Rsp,PkgHead.seq);

            int nDataLen = 0;
            char* pData = CSvrFinanceProductBusiness::getObj().GetData(nDataLen);
            if (pData)
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryProductModule_Rsp,
                    pData,nDataLen,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryProductModule_Rsp,
                    NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            if(pData)
            {
                delete pData;
                pData = NULL;
            }  

            g_setServerSocket.insert(hSocket);

            break;
        }
    case Cmd_QueryProductModule_Rsp:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
                break;

            char* pBuffer = (char*)pPkgData;
            if(!pBuffer || PkgHead.len==0)
                break;

            std::map<int, FinancialProduct>	mapFinancialProduct;
            std::map<int, std::vector<int>>	mapProductID2TraderIDsRelation;

            int nFinancialProductSize   = *(int*)(pBuffer); 
            int nProductID2TraderIDSize = *(int*)(pBuffer+sizeof(int));


            int i;
            int nSize = 0;
            for ( i = 0; i < nFinancialProductSize; i++ )
            {
                FinancialProduct* pFinancialProduct = (FinancialProduct*)(pBuffer+2*sizeof(int)+i*sizeof(FinancialProduct));
                mapFinancialProduct[pFinancialProduct->nID] = *pFinancialProduct;
            }
            nSize += nFinancialProductSize*sizeof(FinancialProduct);

            for ( i = 0; i < nProductID2TraderIDSize; i++ )
            {        
                TraderProRelation* pRelation = (TraderProRelation*)(pBuffer+2*sizeof(int)+nSize+i*sizeof(TraderProRelation));
                mapProductID2TraderIDsRelation[pRelation->nFinancialProductID].push_back(pRelation->nTraderID);
            }

            CSvrFinanceProductBusiness::getObj().SetData(  mapFinancialProduct, mapProductID2TraderIDsRelation ); 

            break;
        }
    case Cmd_AddFinicialProduct_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(FinancialProduct),hSocket,Cmd_AddFinicialProduct_Rsp,PkgHead.seq);

                //�ж�Ȩ��
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,FIN_PRODUCT_ADD))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddFinicialProduct_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                FinancialProduct *pFP = (FinancialProduct*)pPkgData;

                int nPKID = 0;
                int nErrorCode = CSvrFinanceProductBusiness::getObj().AddFinancialProduct(PkgHead.userdata1,pFP, nPKID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //���ʹ���ش�
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_AddFinicialProduct_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_AddFinicialProduct_Rsp,
                        &nPKID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //����һ���㲥�¼�	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_PRODUCT),
                        hSocket,
                        eSendManager|eSendPresentation);

                    Stru_NotifyEvent lsEvent(EventTypeFinicialProductAdd,nPKID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //���͸��ӷ�����
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_AddFinicialProduct_Req, pFP, sizeof(FinancialProduct), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                FinancialProduct *pFP = (FinancialProduct*)pPkgData;
                int nPKID = 0;
                CSvrFinanceProductBusiness::getObj().AddFinancialProduct(PkgHead.userdata1,pFP, nPKID);

                //����һ���㲥�¼�	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_PRODUCT),
                    hSocket,
                    eSendManager|eSendPresentation);

                Stru_NotifyEvent lsEvent(EventTypeFinicialProductAdd,nPKID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_DeleteFinicialProduct_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(int),hSocket,Cmd_DeleteFinicialProduct_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,FIN_PRODUCT_DEL))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DeleteFinicialProduct_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                int nFPID = *(int*)pPkgData;

                //��ѯ����Ա�Ƿ�����
                CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_FinacialProduct,
                    AskOperationTypeDelete,
                    nFPID,
                    NULL);
                if(lRet != CF_ERROR_SUCCESS)			
                {
                    //���ʹ���ش�
                    SendRspError(hSocket,
                        Cmd_DeleteFinicialProduct_Rsp,
                        PkgHead.seq,
                        lRet,
                        lRet,
                        0,0,0);
                    break;
                }


                int nErrorCode = CSvrFinanceProductBusiness::getObj().DeleteFinancialProduct(PkgHead.userdata1,nFPID);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //���ʹ���ش�
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_DeleteFinicialProduct_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_DeleteFinicialProduct_Rsp,
                        &nFPID,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //����һ���㲥�¼�	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_PRODUCT),
                        hSocket,
                        eSendManager|eSendPresentation);

                    Stru_NotifyEvent lsEvent(EventTypeFinicialProductDelete,nFPID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //���͸��ӷ�����
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_DeleteFinicialProduct_Req, &nFPID, sizeof(int), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                int nFPID = *(int*)pPkgData;
                CSvrFinanceProductBusiness::getObj().DeleteFinancialProduct(PkgHead.userdata1,nFPID);

                //����һ���㲥�¼�	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_PRODUCT),
                    hSocket,
                    eSendManager|eSendPresentation);

                Stru_NotifyEvent lsEvent(EventTypeFinicialProductDelete,nFPID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_ModifyFinicialProduct_Req:
        {            
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,sizeof(FinancialProduct),hSocket,Cmd_ModifyFinicialProduct_Rsp,PkgHead.seq);
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,FIN_PRODUCT_MODIFY))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyFinicialProduct_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                FinancialProduct *pFP = (FinancialProduct*)pPkgData;

                CF_ERROR lRet = CInterface_SvrNotifyAndAsk::getObj().Ask(AskType_FinacialProduct,
                    AskOperationTypeUpdate,
                    pFP->nID,
                    NULL);
                if(lRet != CF_ERROR_SUCCESS)			
                {
                    //���ʹ���ش�
                    SendRspError(hSocket,
                        Cmd_ModifyFinicialProduct_Rsp,
                        PkgHead.seq,
                        lRet,
                        lRet,
                        0,0,0);
                    break;
                }

                int nErrorCode = CSvrFinanceProductBusiness::getObj().ModifyFinancialProduct(PkgHead.userdata1,pFP);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //���ʹ���ش�
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_ModifyFinicialProduct_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_ModifyFinicialProduct_Rsp,
                        pFP,sizeof(FinancialProduct),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //����һ���㲥�¼�	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                                            (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_PRODUCT),
                                            hSocket,
                                            eSendManager|eSendPresentation|eSendRisk|eSendTrader);

                    Stru_NotifyEvent lsEvent(EventTypeFinicialProductUpdate,pFP->nID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //���͸��ӷ�����
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_ModifyFinicialProduct_Req, pFP, sizeof(FinancialProduct), PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                FinancialProduct *pFP = (FinancialProduct*)pPkgData;
                CSvrFinanceProductBusiness::getObj().ModifyFinancialProduct(PkgHead.userdata1,pFP);

                //����һ���㲥�¼�	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_PRODUCT),
                    hSocket,
                    eSendManager|eSendPresentation|eSendRisk|eSendTrader);

                Stru_NotifyEvent lsEvent(EventTypeFinicialProductUpdate,pFP->nID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_QueryFinicialProduct_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QueryFinicialProduct_Rsp,PkgHead.seq);

            std::vector<FinancialProduct> vec;
            CSvrFinanceProductBusiness::getObj().GetFinancialProduct(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryFinicialProduct_Rsp,
                    &vec[0],sizeof(FinancialProduct)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryFinicialProduct_Rsp,
                    NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }

    case Cmd_SaveProductTraderRelation_FromUser_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,USER_TRADER_ASSIGN_PRODUCT))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveProductTraderRelation_FromUser_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                int nErrorCode = 0;
                int nUserID = 0;
                int nCount = 0;
                int nDataLen = 0;
                std::vector<int> vec;
                bool bAdd = false;

                int nType = *(int*)pPkgData;
                if (TRADER_ASSGIN_PRODUCT_TYPE == nType)
                {
                    nUserID = *(int*)((char*)pPkgData+sizeof(int));;
                    nCount = (PkgHead.len - 2*sizeof(int)) / sizeof(int);
                    for ( int i = 0; i < nCount; i++ )
                    {
                        int nProductID = *(int*)((char*)pPkgData + 2*sizeof(int) + sizeof(int)*i);
                        vec.push_back(nProductID);
                    }
                }

                nDataLen = 2*sizeof(int)+vec.size()*sizeof(int);

                nErrorCode = CSvrFinanceProductBusiness::getObj().SaveProductTraderRelation(PkgHead.userdata1, nType, nUserID, vec);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //���ʹ���ش�
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveProductTraderRelation_FromUser_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_SaveProductTraderRelation_FromUser_Rsp,
                        &nType,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //����һ���㲥�¼�		
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USERANDPRODUCT),
                        hSocket,
                        eSendManager|eSendPresentation|eSendSpecialUserClient);

                    Stru_NotifyEvent lsEvent(EventTypeFinicialProductAndUserRelation,nUserID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //���͸��ӷ�����
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_SaveProductTraderRelation_FromUser_Req, (char*)pPkgData, nDataLen, PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                int nUserID;
                int nCount;                
                int nType = *(int*)pPkgData;
                
                std::vector<int> vec;
                if (TRADER_ASSGIN_PRODUCT_TYPE == nType)
                {
                    nUserID = *(int*)((char*)pPkgData+sizeof(int));;
                    nCount = (PkgHead.len - 2*sizeof(int)) / sizeof(int);
                    for ( int i = 0; i < nCount; i++ )
                    {
                        int nProductID = *(int*)((char*)pPkgData + 2*sizeof(int) + sizeof(int)*i);
                        vec.push_back(nProductID);
                    }
                }

                CSvrFinanceProductBusiness::getObj().SaveProductTraderRelation(PkgHead.userdata1, nType, nUserID, vec);

                //����һ���㲥�¼�		
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USERANDPRODUCT),
                    hSocket,
                    eSendManager|eSendPresentation|eSendSpecialUserClient);

                Stru_NotifyEvent lsEvent(EventTypeFinicialProductAndUserRelation,nUserID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_SaveProductTraderRelation_FromProduct_Req:
        {
            if(ServerRunMode_Master == g_bServerRunMode)
            {
                if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata1,FIN_PRODUCT_ASSIGN_TRADER))
                {
                    const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveProductTraderRelation_FromProduct_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,CF_ERROR_COMMON_NO_PRIVILEGE);
                    break;
                }

                int nErrorCode = 0;
                int nProductID = 0;
                int nCount = 0;
                int nDataLen = 0;
                std::vector<int> vec;
                bool bAdd = false;
                int nOrgID=-1;

                int nType = *(int*)pPkgData;
                if (PRODUCT_ASSGIN_TRADER_TYPE == nType)
                {
                    nProductID = *(int*)((char*)pPkgData+sizeof(int));;
                    nCount = (PkgHead.len - 2*sizeof(int)) / sizeof(int);
                    for ( int i = 0; i < nCount; i++ )
                    {
                        int nTraderID = *(int*)((char*)pPkgData + 2*sizeof(int) + sizeof(int)*i);
                        vec.push_back(nTraderID);
                    }
                } 

                nDataLen = 2*sizeof(int)+vec.size()*sizeof(int);

                nErrorCode = CSvrFinanceProductBusiness::getObj().SaveProductTraderRelation(PkgHead.userdata1, nType, nProductID, vec);
                if(CF_ERROR_SUCCESS != nErrorCode)
                {
                    //���ʹ���ش�
                    const char * lErrorString = FormatErrorCode(nErrorCode);
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_SaveProductTraderRelation_FromProduct_Rsp, 
                        (void*)lErrorString, 
                        strlen(lErrorString)+1, PkgHead.seq,0,nErrorCode);			
                }
                else
                {
                    CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_SaveProductTraderRelation_FromProduct_Rsp,
                        &nType,sizeof(int),PkgHead.seq,0,CF_ERROR_SUCCESS);

                    //����һ���㲥�¼�	
                    sErrorInfo ssErrorInfo(OperationLogNeedSend,
                        (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USERANDPRODUCT),
                        hSocket,
                        eSendManager|eSendPresentation|eSendSpecialUserClient);

                    Stru_NotifyEvent lsEvent(EventTypeFinicialProductAndUserRelation,nProductID);
                    lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                    CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);

                    //���͸��ӷ�����
                    for(set<int>::iterator it=g_setServerSocket.begin();it!=g_setServerSocket.end();++it)
                        CInterface_SvrTcp::getObj().SendPkgData(*it,Cmd_SaveProductTraderRelation_FromProduct_Req, (char*)pPkgData, nDataLen, PkgHead.seq);

                }
            }
            else if(ServerRunMode_Slave == g_bServerRunMode)
            {
                int nProductID;
                int nCount;                
                int nType = *(int*)pPkgData;

                std::vector<int> vec;
                if (PRODUCT_ASSGIN_TRADER_TYPE == nType)
                {
                    nProductID = *(int*)((char*)pPkgData+sizeof(int));;
                    nCount = (PkgHead.len - 2*sizeof(int)) / sizeof(int);
                    for ( int i = 0; i < nCount; i++ )
                    {
                        int nTraderID = *(int*)((char*)pPkgData + 2*sizeof(int) + sizeof(int)*i);
                        vec.push_back(nTraderID);
                    }
                }

                CSvrFinanceProductBusiness::getObj().SaveProductTraderRelation(PkgHead.userdata1, nType, nProductID, vec);

                //����һ���㲥�¼�	
                sErrorInfo ssErrorInfo(OperationLogNeedSend,
                    (char*)FormatErrorCode(CF_ERROR_DATA_UPDATE_USERANDPRODUCT),
                    hSocket,
                    eSendManager|eSendPresentation|eSendSpecialUserClient);

                Stru_NotifyEvent lsEvent(EventTypeFinicialProductAndUserRelation,nProductID);
                lsEvent.AddBufer((void*)&ssErrorInfo,sizeof(ssErrorInfo));
                CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsEvent);
            }

            break;
        }
    case Cmd_QueryProductTraderRelation_Req:
        {
            BREAK_ON_DIFF_AND_SEND_ERROR_RSP(PkgHead.len,0,hSocket,Cmd_QueryFinicialProduct_Rsp,PkgHead.seq);

            std::vector<TraderProRelation> vec;
            CSvrFinanceProductBusiness::getObj().GetTraderProRelation(vec);
            if (!vec.empty())
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryProductTraderRelation_Rsp,
                    &vec[0],sizeof(TraderProRelation)*vec.size(),PkgHead.seq,0,CF_ERROR_SUCCESS);
            }
            else
            {
                CInterface_SvrTcp::getObj().SendPkgData(hSocket,Cmd_QueryProductTraderRelation_Rsp,
                    NULL,0,PkgHead.seq,0,CF_ERROR_SUCCESS);
            }

            break;
        }
    }
}

