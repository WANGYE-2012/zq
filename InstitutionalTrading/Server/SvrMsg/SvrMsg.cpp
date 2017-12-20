// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include "stdafx.h"
#include <windows.h>
#include "SvrMsg.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "..\SvrDBOpr\Interface_SvrDBOpr.h"
#include "..\\SvrNotifyAndAsk\\Interface_SvrNotifyAndAsk.h"
#include "..\SvrLogin\CustomSvrLoginInterface.h"
#include "UserMsgCenter.h"


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
#pragma comment(lib, "SvrDBOpr.lib")
#pragma comment(lib, "SvrLogin.lib")



#define WRITELOGID_SvrMsg




//ȫ�ֻ�����
Ceasymutex			g_mutex;
CUserMsgCenter		g_userMsgCenter;

//�̲߳���
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;
BOOL				g_bRunInMaster = 0;

DWORD ThreadWorker(void *arg);

// ����һ�����ݸı��¼�
bool ParseDataChangedEvent(const Stru_NotifyEvent& dataEvt);
//����һ�����յ������ݰ�
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

// �����ݿ��м���ĳ���û���ĳ�����ڵķ��ͻ���յ�������Ϣ
int DB_LoadMsgs_UserID(int nUserID, int nDate, bool bIsSend, vector<UserMsgStru>& vecRetMsg);

//// ת���û���Ϣ������������
//bool SendUserMsgToOtherServer(int hSrcSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);
bool TurnSendReqToMaster(const Stru_UniPkgHead& PkgHead,const void* pPkgData);

//ģ���ʼ�������������̲߳���SvrTcp���ĸ���Ȥ�����ݰ�
SVRMSG_API void InitFunc(void)
{
	//int nPathLen = 0;
	//char localPath[256];
	//memset(localPath, 0, 256);
	//nPathLen = GetModuleFileName( NULL, localPath, 256);
	//string filename=localPath;
	//size_t splitpos=filename.find_last_of('\\');
	//string strPath(filename.substr(0, splitpos+1).c_str());
	//strPath += "svrMsg.ini";
	//
	//g_bRunInMaster = ::GetPrivateProfileInt("RunSetting", "IsMaster", 0, strPath.c_str());

	g_bRunInMaster = !CInterface_SvrLogin::getObj().IsSlaver() ? 1 : 0;
	g_userMsgCenter.SetMaster(!CInterface_SvrLogin::getObj().IsSlaver());
	
	//���������߳�
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);
		
}

//ģ��������ͷ���Դ���رչ����߳�
SVRMSG_API void ReleaseFunc(void)
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
	}
	g_userMsgCenter.Release();
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
	// ���ﶩ�ĸ���Ȥ��Event
	CInterface_SvrNotifyAndAsk::getObj().SubscribeNotifyEvent(EventTypeSlaverLoginedMaster, g_idThread); 
	
	//���涩�ı��̸߳���Ȥ��Tcp���ݰ�
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_SvrTcpDisconn,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_TcpClientConnect,g_idThread); //�ӷ�����������������
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_TcpClientDisconn,g_idThread); //�ӷ����������������Ͽ�
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_UserMsg_SubsMsg_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_UserMsg_UnSubsMsg_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_UserMsg_Send_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_UserMsg_Query_Send_Req,g_idThread);
	CInterface_SvrTcp::getObj().SubscribePkg(Cmd_UserMsg_Query_Recv_Req,g_idThread);
	if(g_bRunInMaster) {
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_UserMsg_QueryTargetMsgFromSlave_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_UserMsg_TurnSendReqToMaster_Req,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_UserMsg_SynchroSentStatusToMaster_Req,g_idThread);
	}
	else {
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_UserMsg_SynchrPushMsg,g_idThread);
		CInterface_SvrTcp::getObj().SubscribePkg(Cmd_UserMsg_SynchroSentStatusToSlave_Req,g_idThread);
	}
	
	g_userMsgCenter.Init();

	MSG Msg;
	
	while(GetMessage(&Msg,NULL,0,0)) {
		if(Msg.message==WM_COMMAND&&Msg.wParam==WndCmd_YourPkgArrival2) {
			//���ݰ��Ĵ���ID
			unsigned int EventParamID=(unsigned int)Msg.lParam;
			//���ݰ�����
			int PkgLen;
			//Socket���
			int hSocket;
			if(CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,NULL,&PkgLen,0,NULL)) {
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
		else if(Msg.message==WM_COMMAND&&Msg.wParam==WndCmd_NotifyEventArrival) {
			unsigned int EventParamID=(unsigned int)Msg.lParam;
			//���ݰ�����
			Stru_NotifyEvent ls;
			int nLen = sizeof(ls);
			if(CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,
					NULL,&ls,&nLen,nLen,NULL)) {
				//�������ݰ��������������ݰ�
				ParseDataChangedEvent(ls);
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
	case EventTypeSlaverLoginedMaster:
		{
			//OUTPUT_LOG("Close All User Socket but manager");
			//���͸������ն�
			_SetMasterSocket(dataEvt.mnDataID);
			if(!g_bRunInMaster)
				g_userMsgCenter.ReqSynchrQuery();
		}
		break;
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
	case CMDID_SvrTcpDisconn:
		break;
	case CMDID_TcpClientConnect:
		break;
	case CMDID_TcpClientDisconn:
		if(!g_bRunInMaster) {
			_SetMasterSocket(0);
			g_userMsgCenter.ClearData();
		}
		break;
	case Cmd_UserMsg_SubsMsg_Req:		//�û�������Ϣ����
		g_userMsgCenter.SubsUser(PkgHead.userdata1);
		CInterface_SvrTcp::getObj().SendPkgData(hSocket,
			Cmd_UserMsg_SubsMsg_Rsp, NULL, 0, PkgHead.seq, 0, CF_ERROR_SUCCESS, PkgHead.userdata1, 0, 0);
		break;
	case Cmd_UserMsg_UnSubsMsg_Req:		//�û�ȡ��������Ϣ����
		g_userMsgCenter.UnsubsUser(PkgHead.userdata1);
		CInterface_SvrTcp::getObj().SendPkgData(hSocket,
			Cmd_UserMsg_UnSubsMsg_Rsp, NULL, 0, PkgHead.seq, 0, CF_ERROR_SUCCESS, PkgHead.userdata1, 0, 0);
		break;
	case Cmd_UserMsg_Send_Req:			//�û�������Ϣ����
	case Cmd_UserMsg_TurnSendReqToMaster_Req:
	{
		UserSendMsgStru * pMsg = (UserSendMsgStru*)pPkgData;

		if(PkgHead.len < sizeof(UserSendMsgStru) || pMsg->TargetUserCount<1) {
            const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_INPUT_PARAM);
            CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_UserMsg_Send_Rsp, 
                (void*)lErrorString, 
                strlen(lErrorString)+1, PkgHead.seq, 0, CF_ERROR_COMMON_INPUT_PARAM);
			break;
		}

		if(PkgHead.cmdid==Cmd_UserMsg_Send_Req) {
			if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata4,MESSAGE_SEND)) {
				const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
				CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_UserMsg_Send_Rsp, 
					(void*)lErrorString, 
					strlen(lErrorString)+1, PkgHead.seq, 0, CF_ERROR_COMMON_NO_PRIVILEGE);
				break;
			}
		}

		if(g_bRunInMaster) {
			set<int> setTarget;
			for(int i=0; i<pMsg->TargetUserCount; i++) {
				setTarget.insert(pMsg->TargetUsers[i]);
			}
			g_userMsgCenter.AppendOrigMsg(pMsg->OwnerUserID, setTarget, conManualMsg, 
					pMsg->ExpiredDate, string(pMsg->Title), string(pMsg->Msg));
			CInterface_SvrTcp::getObj().SendPkgData(hSocket,
					Cmd_UserMsg_Send_Rsp, NULL, 0, PkgHead.seq, 0, 
					CF_ERROR_SUCCESS, PkgHead.userdata1, 0, 0);
		}
		else {
			if(PkgHead.cmdid==Cmd_UserMsg_Send_Req) 
				TurnSendReqToMaster(PkgHead, pPkgData);
		}
		return;
	}
		break;
	case Cmd_UserMsg_SynchrPushMsg:
	{
		UserMsgStru * pMsg = (UserMsgStru*)pPkgData;

		if(PkgHead.len != sizeof(UserMsgStru)) {
            const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_INPUT_PARAM);
            CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_UserMsg_Send_Rsp, 
                (void*)lErrorString, 
                strlen(lErrorString)+1, PkgHead.seq, 0, CF_ERROR_COMMON_INPUT_PARAM);
			break;
		}

		if(!g_bRunInMaster) {
			g_userMsgCenter.AppendTargetMsg(*pMsg);
		}
		return;
	}
		break;
	case Cmd_UserMsg_QueryTargetMsgFromSlave_Req:
		if(g_bRunInMaster) {
			g_userMsgCenter.SendTargetMsgToSlave(hSocket);
		}
		break;
	case Cmd_UserMsg_SynchroSentStatusToMaster_Req:
	case Cmd_UserMsg_SynchroSentStatusToSlave_Req:
	{
		UserMsgStatusStru * pMsg = (UserMsgStatusStru*)pPkgData;

		if(PkgHead.len != sizeof(UserMsgStatusStru)) {
            const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_INPUT_PARAM);
            CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_UserMsg_Send_Rsp, 
                (void*)lErrorString, 
                strlen(lErrorString)+1, PkgHead.seq, 0, CF_ERROR_COMMON_INPUT_PARAM);
			break;
		}

		if(g_bRunInMaster) {
			g_userMsgCenter.UpdateTargetMsgStatus(*pMsg, true, hSocket);
		}
		else {
			g_userMsgCenter.UpdateTargetMsgStatus(*pMsg, false, 0);
		}
		return;
	}
		break;
	case Cmd_UserMsg_Query_Send_Req:	//�û���ѯ������Ϣ����
	{
		int* pDate = (int*)pPkgData;

		if(PkgHead.len < sizeof(int)) {
            const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_INPUT_PARAM);
            CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_UserMsg_Query_Send_Rsp, 
                (void*)lErrorString, 
                strlen(lErrorString)+1, PkgHead.seq, 0, CF_ERROR_COMMON_INPUT_PARAM);
			return;
		}

        if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata4,MESSAGE_QUERY)) {
            const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
            CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_UserMsg_Query_Send_Rsp, 
                (void*)lErrorString, 
                strlen(lErrorString)+1, PkgHead.seq, 0, CF_ERROR_COMMON_NO_PRIVILEGE);
            return;
        }

		int nRet = 0;
		vector<UserMsgStru> vecRetMsg;

		if((nRet = DB_LoadMsgs_UserID(PkgHead.userdata1, *pDate, true, vecRetMsg))==CF_ERROR_SUCCESS) {
			CInterface_SvrTcp::getObj().SendPkgData(hSocket,
					Cmd_UserMsg_Query_Send_Rsp, 
					(vecRetMsg.size()>0 ? &vecRetMsg[0] : NULL), vecRetMsg.size()*sizeof(UserMsgStru), 
					PkgHead.seq, 0, CF_ERROR_SUCCESS, PkgHead.userdata1, 0, 0);
		}
		else 
			CInterface_SvrTcp::getObj().SendPkgData(hSocket,
					Cmd_UserMsg_Query_Send_Rsp, NULL, 0, PkgHead.seq, 0, 
					nRet, PkgHead.userdata1, 0, 0);
		return;
	}
		break;
	case Cmd_UserMsg_Query_Recv_Req:	//�û���ѯ������Ϣ����
	{
		int* pDate = (int*)pPkgData;

		if(PkgHead.len < sizeof(int)) {
            const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_INPUT_PARAM);
            CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_UserMsg_Query_Send_Rsp, 
                (void*)lErrorString, 
                strlen(lErrorString)+1, PkgHead.seq, 0, CF_ERROR_COMMON_INPUT_PARAM);
			return;
		}

        if(!CInterface_SvrNotifyAndAsk::getObj().Ask_ValidateUserPrivilege(PkgHead.userdata4,MESSAGE_QUERY)) {
            const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_NO_PRIVILEGE);
            CInterface_SvrTcp::getObj().SendPkgData(hSocket, Cmd_UserMsg_Query_Send_Rsp, 
                (void*)lErrorString, 
                strlen(lErrorString)+1, PkgHead.seq, 0, CF_ERROR_COMMON_NO_PRIVILEGE);
            return;
        }

		int nRet = 0;
		vector<UserMsgStru> vecRetMsg;

		if((nRet = DB_LoadMsgs_UserID(PkgHead.userdata1, *pDate, false, vecRetMsg))==CF_ERROR_SUCCESS) {
			CInterface_SvrTcp::getObj().SendPkgData(hSocket,
					Cmd_UserMsg_Query_Recv_Rsp, 
					(vecRetMsg.size()>0 ? &vecRetMsg[0] : NULL), vecRetMsg.size()*sizeof(UserMsgStru), 
					PkgHead.seq, 0, CF_ERROR_SUCCESS, PkgHead.userdata1, 0, 0);
		}
		else 
			CInterface_SvrTcp::getObj().SendPkgData(hSocket,
					Cmd_UserMsg_Query_Recv_Rsp, NULL, 0, PkgHead.seq, 0, 
					nRet, PkgHead.userdata1, 0, 0);
		return;
	}
		break;
	}
}

bool TurnSendReqToMaster(const Stru_UniPkgHead& PkgHead,const void* pPkgData)
{
	if(_GetMasterSocket()!=0)
		CInterface_SvrTcp::getObj().SendPkgData(_GetMasterSocket(),
				Cmd_UserMsg_TurnSendReqToMaster_Req, (void*)pPkgData, PkgHead.len, 0, 0, PkgHead.userdata1, 
				PkgHead.userdata2, PkgHead.userdata3, PkgHead.userdata4);
	return true;
}

//bool SendUserMsgToOtherServer(int hSrcSocket, const Stru_UniPkgHead& PkgHead,const void* pPkgData)
//{
//	if(g_bRunInMaster) {
//		std::vector<SOCKET> lvecSocket;
//		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lvecSocket);
//		for(int i = 0; i < lvecSocket.size(); i++) {
//			if(lvecSocket[i]==hSrcSocket)
//				continue;
//			CInterface_SvrTcp::getObj().SendPkgData(lvecSocket[i], 
//					Cmd_UserMsg_SendToSlave_Req, (void*)pPkgData, PkgHead.len, 0, 0, PkgHead.userdata1, 
//					PkgHead.userdata2, PkgHead.userdata3, PkgHead.userdata4);
//		}
//	}
//	else {
//		if(_GetMasterSocket()!=0)
//			CInterface_SvrTcp::getObj().SendPkgData(_GetMasterSocket(),
//					Cmd_UserMsg_SendToMaster_Req, (void*)pPkgData, PkgHead.len, 0, 0, PkgHead.userdata1, 
//					PkgHead.userdata2, PkgHead.userdata3, PkgHead.userdata4);
//	}
//	return true;
//}

int DB_LoadMsgs_UserID(int nUserID, int nDate, bool bIsSend, vector<UserMsgStru>& vecRetMsg)
{
	int nErrorCode = CF_ERROR_SUCCESS;

	SYSTEMTIME sysTime;
	char szBuffer[MAX_SQL_LENGTH];

	memset(szBuffer, 0, sizeof(szBuffer));
	::GetLocalTime(&sysTime);
	if(bIsSend)
		sprintf(szBuffer, 
				"select * from (select tta.msgid, tta.targetmsgid, tor.ownerid, tta.userid as targetuserid, tor.expireddate, tor.createdatetime, tta.hassend, tta.senddatetime, tor.title, tor.message from USERMSG_ORIG tor, USERMSG_TARGET tta where tta.msgid = tor.msgid) t "
				"where t.ownerid = %d AND to_date(substr(t.createdatetime, 1, 10), 'yyyy-MM-dd') = to_date('%04d-%02d-%02d', 'yyyy-MM-dd')", 
				nUserID, nDate/10000, (nDate/100)%100, nDate%100);
	else
		sprintf(szBuffer, 
				"select * from (select tta.msgid, tta.targetmsgid, tor.ownerid, tta.userid as targetuserid, tor.expireddate, tor.createdatetime, tta.hassend, tta.senddatetime, tor.title, tor.message from USERMSG_TARGET tta, USERMSG_ORIG tor where tta.msgid = tor.msgid) t "
				"where t.targetuserid = %d AND to_date(substr(t.senddatetime, 1, 10), 'yyyy-MM-dd') = to_date('%04d-%02d-%02d', 'yyyy-MM-dd')", 
				nUserID, nDate/10000, (nDate/100)%100, nDate%100);
	
	std::vector<std::vector<_variant_t>> vNode;
	if(!CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode)/* || vNode.size()==0*/) {
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else {
		UserMsgStru msg;
		for(UINT j = 0; j < vNode.size(); j++) {
			memset(&msg, 0, sizeof(msg));

			std::vector<_variant_t> vValue = vNode[j];
			/*if ( vValue.size() != 10)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{
				int i = 0;
				msg.MsgID = vValue[i++];
				msg.TargetMsgID = vValue[i++];
				msg.OwnerUserID = vValue[i++];
				msg.TargetUserID = vValue[i++];
				msg.ExpiredDate = vValue[i++];
				strncpy(msg.CreateDateTime, (_bstr_t)(vValue[i++]), sizeof(msg.CreateDateTime)-1);
				i++;// hassend
				strncpy(msg.SendDateTime, (_bstr_t)(vValue[i++]), sizeof(msg.SendDateTime)-1);
				strncpy(msg.Title, (_bstr_t)(vValue[i++]), sizeof(msg.Title)-1);
				strncpy(msg.Msg, (_bstr_t)(vValue[i++]), sizeof(msg.Msg)-1);

				vecRetMsg.push_back(msg);
			}
		}
	}

	return CF_ERROR_SUCCESS;
}
