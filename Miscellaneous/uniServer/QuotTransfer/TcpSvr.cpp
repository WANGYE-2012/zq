#include "QuotTransfer.h"
#include "SocketBase.h"
#include "WinService.h"


#define WRITELOGID 0


#pragma pack(push)
#pragma pack(4)

//ע�����ݰ�
typedef struct
{
	DWORD	Head;					//��ͷ
	int		PackageLen;				//������
	DWORD	CRCValue;				//CRCУ��ֵ
	DWORD	Cmd;					//������
	char	UserName[32];			//�û���
	char	UserPW[32];				//����
	DWORD	Tail;					//��β
}Stru_Reg;

//ע��ɹ��ر����ݰ�
typedef struct
{
	DWORD	Head;					//��ͷ
	int		PackageLen;				//������
	DWORD	CRCValue;				//CRCУ��ֵ
	DWORD	Cmd;					//������
	DWORD	Tail;					//��β
}Stru_RegSucc;

//������
typedef struct
{
	DWORD	Head;					//��ͷ
	int		PackageLen;				//������
	DWORD	CRCValue;				//CRCУ��ֵ
	DWORD	Cmd;					//������
	DWORD	Tail;					//��β
}Stru_Heartbeat;

//IB��Լ��Ϣ�����
typedef struct
{
	DWORD	Head;					//��ͷ
	int		PackageLen;				//������
	DWORD	CRCValue;				//CRCУ��ֵ
	DWORD	Cmd;					//������
	char	UserName[32];			//�û���
	char	UserPW[32];				//����
	DWORD	Tail;					//��β
}Stru_ReqIBContractsInfo;

//CTP������Լ��Ϣ�����
typedef struct
{
	DWORD	Head;					//��ͷ
	int		PackageLen;				//������
	DWORD	CRCValue;				//CRCУ��ֵ
	DWORD	Cmd;					//������
	char	UserName[32];			//�û���
	char	UserPW[32];				//����
	DWORD	Tail;					//��β
}Stru_ReqCTPMainContractsInfo;

#pragma pack(pop)


//ת����������
void SendNewQuotData(Stru_Quot2& QuotData)
{
	if(g_pQuotCache)
		g_pQuotCache->SaveDataTo(&QuotData);
}


//accept�󣬵��ô˻ص��������д���
QUOTTRANSFER_API void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	std::map<SOCKET,Stru_DataSocket>::iterator it;
	Stru_DataSocket TmpDataSocket;
	TmpDataSocket.hScoket=hSocket;
	TmpDataSocket.pStreamCache=new CMemoryCache(10240);
	TmpDataSocket.RemoteAddr=RemoteAddr;
	time(&TmpDataSocket.HeartbeatTime);

	if(g_pSem) 
	{
		CTools_Win32::TakeWinSem(g_pSem);
	}


	it=g_TcpUsers.find(hSocket);
	if(it!=g_TcpUsers.end())
	{
		if(it->second.pStreamCache)
		{
			delete it->second.pStreamCache;
			it->second.pStreamCache=NULL;
		}

		g_TcpUsers.erase(it);
	}

	g_TcpUsers[hSocket]=TmpDataSocket;


	if(g_pSem) 
	{
		CTools_Win32::GiveWinSem(g_pSem);
	}

	if(g_pWriteLog)
    {
        char buf[10240];
        sprintf(buf,"hSocket=%d Addr=%s:%d",
				hSocket,
                CTools_Ansi::ConvertdwIP(TmpDataSocket.RemoteAddr.IP),
				TmpDataSocket.RemoteAddr.Port);
		g_pWriteLog->WriteLog_Fmt("��������",WriteLogLevel_DEBUGINFO,"%s",buf);
        if(!CWinService::IsRunInServiceMode())
            printf("%s\n",buf);
    }

}


//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
QUOTTRANSFER_API void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	std::map<SOCKET,Stru_DataSocket>::iterator it;

	if(g_pSem) 
	{
		CTools_Win32::TakeWinSem(g_pSem);
	}

	it=g_TcpUsers.find(hSocket);
	if(it!=g_TcpUsers.end())
	{
		if(g_pWriteLog)
        {
            char buf[10240];
            sprintf(buf,"hSocket=%d Addr=%s:%d",
					hSocket,
                    CTools_Ansi::ConvertdwIP(it->second.RemoteAddr.IP),
					it->second.RemoteAddr.Port);
			g_pWriteLog->WriteLog_Fmt("�Ͽ�����",WriteLogLevel_DEBUGINFO,"%s",buf);
            if(!CWinService::IsRunInServiceMode())
                printf("%s\n",buf);
        }

		if(it->second.pStreamCache)
		{
			delete it->second.pStreamCache;
			it->second.pStreamCache=NULL;
		}

		g_TcpUsers.erase(it);
	}

	if(g_pSem) 
	{
		CTools_Win32::GiveWinSem(g_pSem);
	}

}


//��tcp�õ����ݺ󣬵��ô˻ص��������д���
QUOTTRANSFER_API void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	std::map<SOCKET,Stru_DataSocket>::iterator it;
	std::map<SOCKET,Stru_RegSucc> TmpRegSuccSet;
	std::set<SOCKET> TmpReqIBContractsInfoSet;
	std::set<SOCKET> TmpReqCTPMainContractsInfoSet;

	if(!pData) return;

	if(g_pWriteLog)
	{
		char Buf[512];
		int i;

		memset(Buf,0,sizeof(Buf));
		for(i=0;i<DataLen&&i<=80;i++)
		{
			sprintf(Buf+strlen(Buf),"%02x",(BYTE)pData[i]);
			if(i%4==3) strcat(Buf," ");
		}

        char buf[20480];
        sprintf(buf,"%s:%d Len=%d Content=%s",
				CTools_Ansi::ConvertdwIP(RemoteAddr.IP),
				RemoteAddr.Port,DataLen,Buf);
		g_pWriteLog->WriteLog_Fmt("���յ��ͻ�������",WriteLogLevel_DEBUGINFO,"%s",buf);
        if(!CWinService::IsRunInServiceMode())
            printf("%s\n",buf);
	}

	if(g_pSem) 
	{
		CTools_Win32::TakeWinSem(g_pSem);
	}


	TmpRegSuccSet.clear();
	TmpReqIBContractsInfoSet.clear();
    TmpReqCTPMainContractsInfoSet.clear();

	it=g_TcpUsers.find(hSocket);
	if(it!=g_TcpUsers.end()&&it->second.pStreamCache!=NULL)
	{
		time(&it->second.HeartbeatTime);

		it->second.pStreamCache->SaveDataTo(pData,DataLen);

		int CacheLen,PackageLen,PackageCmd;
		DWORD PackageHead,PackageCRC;
		char FirstBuf[16];
		while( (CacheLen=it->second.pStreamCache->GetLen()) >= 16 )
		{
			memset(FirstBuf,0,sizeof(FirstBuf));

			it->second.pStreamCache->PeekFrom(FirstBuf,16);
			PackageHead=*(DWORD*)FirstBuf;
			PackageLen=*(int*)(FirstBuf+4);
			PackageCRC=*(DWORD*)(FirstBuf+8);
			PackageCmd=*(int*)(FirstBuf+12);

			//��顢�������鶩�ĵ�¼����
			if(PackageHead==PACKAGEHEAD&&
				PackageLen>=sizeof(Stru_Reg)&&
				(PackageCmd==CMD_RegQuotI||PackageCmd==CMD_RegQuotII||PackageCmd==CMD_RegQuotIII))
			{
				Stru_Reg TmpPkg;
				memset(&TmpPkg,0,sizeof(TmpPkg));
				it->second.pStreamCache->GetFrom(&TmpPkg,sizeof(Stru_Reg));
				if(TmpPkg.Tail==PACKAGETAIL)
				{			
					//�����¼��
					std::map<std::string,std::string>::iterator it_ValidUsers;

					TmpPkg.UserName[sizeof(TmpPkg.UserName)-1]=0;
					TmpPkg.UserPW[sizeof(TmpPkg.UserPW)-1]=0;
					if( (TmpPkg.Cmd==CMD_RegQuotI||TmpPkg.Cmd==CMD_RegQuotIII)&&
						(it_ValidUsers=g_ValidUsersI.find(std::string(TmpPkg.UserName)))!=g_ValidUsersI.end()&&
						it_ValidUsers->second==std::string(TmpPkg.UserPW)||
						TmpPkg.Cmd==CMD_RegQuotII&&
						(it_ValidUsers=g_ValidUsersII.find(std::string(TmpPkg.UserName)))!=g_ValidUsersII.end()&&
						it_ValidUsers->second==std::string(TmpPkg.UserPW))
					{
						//�Ϸ��û�
						it->second.bLogin=true;
					    if(TmpPkg.Cmd==CMD_RegQuotI) it->second.ReqType=1;
					    else if(TmpPkg.Cmd==CMD_RegQuotII) it->second.ReqType=2;
					    else if(TmpPkg.Cmd==CMD_RegQuotIII) it->second.ReqType=3;
						memcpy(it->second.UserName,TmpPkg.UserName,sizeof(it->second.UserName));

						//���ͳɹ��ر�
						Stru_RegSucc OutputPkg;
						OutputPkg.Head=PACKAGEHEAD;
						OutputPkg.PackageLen=sizeof(OutputPkg);
						OutputPkg.CRCValue=0;
						OutputPkg.Tail=PACKAGETAIL;
					    if(TmpPkg.Cmd==CMD_RegQuotI) OutputPkg.Cmd=CMD_RegQuotOKI;
					    else if(TmpPkg.Cmd==CMD_RegQuotII) OutputPkg.Cmd=CMD_RegQuotOKII;
					    else if(TmpPkg.Cmd==CMD_RegQuotIII) OutputPkg.Cmd=CMD_RegQuotOKIII;
						TmpRegSuccSet[hSocket]=OutputPkg;

						if(g_pWriteLog)
                        {
                            char buf[10240];
                            sprintf(buf,"%s:%d",
									CTools_Ansi::ConvertdwIP(it->second.RemoteAddr.IP),
									it->second.RemoteAddr.Port);
                            g_pWriteLog->WriteLog_Fmt("���յ�¼����",WriteLogLevel_DEBUGINFO,"%s",buf);
                            if(!CWinService::IsRunInServiceMode())
                                printf("%s\n",buf);
                        }

					}
				}
				continue;
			}
			
			//��顢����������
			if(PackageHead==PACKAGEHEAD&&
				PackageLen>=sizeof(Stru_Heartbeat)&&
				PackageCmd==CMD_Heartbeat)
			{
				Stru_Heartbeat TmpPkg;
				memset(&TmpPkg,0,sizeof(TmpPkg));
				it->second.pStreamCache->GetFrom(&TmpPkg,sizeof(Stru_Heartbeat));
				if(TmpPkg.Tail==PACKAGETAIL)
				{			
					//����������


					if(g_pWriteLog)
                    {
                        char buf[10240];
                        sprintf(buf,"%s:%d",
								CTools_Ansi::ConvertdwIP(it->second.RemoteAddr.IP),
								it->second.RemoteAddr.Port);
                        g_pWriteLog->WriteLog_Fmt("����������",WriteLogLevel_DEBUGINFO,"%s",buf);
                        if(!CWinService::IsRunInServiceMode())
                            printf("%s\n",buf);
                    }

				}
				continue;
			}
			
			//��顢����IB��Լ��Ϣ�����
			if(PackageHead==PACKAGEHEAD&&
				PackageLen>=sizeof(Stru_ReqIBContractsInfo)&&
				PackageCmd==CMD_ReqIBContractsInfo)
			{
				Stru_ReqIBContractsInfo TmpPkg;
				memset(&TmpPkg,0,sizeof(TmpPkg));
				it->second.pStreamCache->GetFrom(&TmpPkg,sizeof(Stru_ReqIBContractsInfo));
				if(TmpPkg.Tail==PACKAGETAIL)
				{			
					std::map<std::string,std::string>::iterator it_ValidUsers;
				
					TmpPkg.UserName[sizeof(TmpPkg.UserName)-1]=0;
					TmpPkg.UserPW[sizeof(TmpPkg.UserPW)-1]=0;
					if( TmpPkg.Cmd==CMD_ReqIBContractsInfo&&
						(it_ValidUsers=g_IBContractsInfoReqUser.find(std::string(TmpPkg.UserName)))!=g_IBContractsInfoReqUser.end()&&
						it_ValidUsers->second==std::string(TmpPkg.UserPW))
					{
						TmpReqIBContractsInfoSet.insert(hSocket);

						if(g_pWriteLog)
                        {
                            char buf[10240];
                            sprintf(buf,"%s:%d",
									CTools_Ansi::ConvertdwIP(it->second.RemoteAddr.IP),
									it->second.RemoteAddr.Port);
                            g_pWriteLog->WriteLog_Fmt("����IB��Լ��Ϣ����",WriteLogLevel_DEBUGINFO,"%s",buf);
                            if(!CWinService::IsRunInServiceMode())
                                printf("%s\n",buf);
                        }

					}
				}
				continue;
			}

			//��顢����CTP������Լ��Ϣ�����
			if(PackageHead==PACKAGEHEAD&&
				PackageLen>=sizeof(Stru_ReqCTPMainContractsInfo)&&
				PackageCmd==CMD_ReqCTPMainContractsInfo)
			{
				Stru_ReqCTPMainContractsInfo TmpPkg;
				memset(&TmpPkg,0,sizeof(TmpPkg));
				it->second.pStreamCache->GetFrom(&TmpPkg,sizeof(Stru_ReqCTPMainContractsInfo));
				if(TmpPkg.Tail==PACKAGETAIL)
				{			
					std::map<std::string,std::string>::iterator it_ValidUsers;
				
					TmpPkg.UserName[sizeof(TmpPkg.UserName)-1]=0;
					TmpPkg.UserPW[sizeof(TmpPkg.UserPW)-1]=0;
					if( TmpPkg.Cmd==CMD_ReqCTPMainContractsInfo&&
						(it_ValidUsers=g_CTPMainContractsInfoReqUser.find(std::string(TmpPkg.UserName)))!=g_CTPMainContractsInfoReqUser.end()&&
						it_ValidUsers->second==std::string(TmpPkg.UserPW))
					{
						TmpReqCTPMainContractsInfoSet.insert(hSocket);

						if(g_pWriteLog)
                        {
                            char buf[10240];
                            sprintf(buf,"%s:%d",
									CTools_Ansi::ConvertdwIP(it->second.RemoteAddr.IP),
									it->second.RemoteAddr.Port);
                            g_pWriteLog->WriteLog_Fmt("����CTP������Լ��Ϣ����",WriteLogLevel_DEBUGINFO,"%s",buf);
                            if(!CWinService::IsRunInServiceMode())
                                printf("%s\n",buf);
                        }

					}
				}
				continue;
			}

			//��鲢������ͷ����Ĳ���
			if(PackageHead!=PACKAGEHEAD)
			{
				int i;
				for(i=0;i<16-3;i++)
					if(*(DWORD*)(FirstBuf+i)==PACKAGEHEAD)
						break;
				it->second.pStreamCache->DeleteData(i);
				continue;
			}

			//��鲢����������������һ��������������ǰ��ò�����������Ϊ��һ�������������
			if(CacheLen>=16+PackageLen+4)
			{
				it->second.pStreamCache->DeleteData(16+PackageLen+4);
				continue;
			}

			break;

		}

	}


	if(g_pSem) 
	{
		CTools_Win32::GiveWinSem(g_pSem);
	}



    std::map<SOCKET,Stru_RegSucc>::iterator it_TmpRegSuccSet;
	std::set<SOCKET>::iterator it_TmpReqIBContractsInfoSet;
	std::set<SOCKET>::iterator it_TmpReqCTPMainContractsInfoSet;
	for(it_TmpRegSuccSet=TmpRegSuccSet.begin();it_TmpRegSuccSet!=TmpRegSuccSet.end();it_TmpRegSuccSet++)
	{
        CSocketBase::SendDataTcp(it_TmpRegSuccSet->first,&(it_TmpRegSuccSet->second),sizeof(Stru_RegSucc));
	}
	if(g_IBContractsInfo!=NULL&&g_IBContractsLen>0)
	{
		for(it_TmpReqIBContractsInfoSet=TmpReqIBContractsInfoSet.begin();it_TmpReqIBContractsInfoSet!=TmpReqIBContractsInfoSet.end();it_TmpReqIBContractsInfoSet++)
		{
			//ע�⣺����g_IBContractsInfo�Ѿ�������ͷ��Cmd��DataLen�Ͱ�β��
            CSocketBase::SendDataTcp(*it_TmpReqIBContractsInfoSet,g_IBContractsInfo,g_IBContractsLen);
		}
	}
	if(g_CTPMainContractsInfo!=NULL&&g_CTPMainContractsLen>0)
	{
		for(it_TmpReqCTPMainContractsInfoSet=TmpReqCTPMainContractsInfoSet.begin();it_TmpReqCTPMainContractsInfoSet!=TmpReqCTPMainContractsInfoSet.end();it_TmpReqCTPMainContractsInfoSet++)
		{
			//ע�⣺����g_CTPMainContractsInfo�Ѿ�������ͷ��Cmd��DataLen�Ͱ�β��
            CSocketBase::SendDataTcp(*it_TmpReqCTPMainContractsInfoSet,g_CTPMainContractsInfo,g_CTPMainContractsLen);
		}
	}

}

//tcp�Ķ�ʱ����
QUOTTRANSFER_API void TcpSvrOntimeFunc(void)
{
	std::map<SOCKET,Stru_DataSocket>::iterator it;
	std::map<SOCKET,Stru_DataSocket>::iterator it1;
	time_t CurTime;
	int DeltaSecs;
	std::set<SOCKET> TmpSockets;
	std::set<SOCKET>::iterator it_Sockets;


	time(&CurTime);

	//��鳬ʱ����
	if(g_pSem) 
		CTools_Win32::TakeWinSem(g_pSem);

	for(it=g_TcpUsers.begin();it!=g_TcpUsers.end();)
	{
		DeltaSecs=(int)(CurTime-it->second.HeartbeatTime);
		if(g_HeartbeatTimeout>0&&DeltaSecs>=g_HeartbeatTimeout)
		{
            SOCKET hsck=it->first;
			if(g_pWriteLog)
            {
                char buf[10240];
                sprintf(buf,"%s:%d ��ʱ%ds",
						CTools_Ansi::ConvertdwIP(it->second.RemoteAddr.IP),
						it->second.RemoteAddr.Port,
						DeltaSecs);
				g_pWriteLog->WriteLog_Fmt("���ӳ�ʱ���Ͽ�����",WriteLogLevel_DEBUGINFO,"%s",buf);
                if(!CWinService::IsRunInServiceMode())
                    printf("%s\n",buf);
            }

            it=g_TcpUsers.erase(it);

            CSocketBase::CloseSocket(hsck);
		}
		else it++;
	}

	if(g_pSem) 
		CTools_Win32::GiveWinSem(g_pSem);


	//ת������
	if(g_pQuotCache)
	{
		Stru_Quot2 QuotData;
		std::map<SOCKET,Stru_DataSocket> TmpUserMap;
		std::map<SOCKET,Stru_DataSocket>::iterator it;
		std::set<std::string>::iterator it1;


		if(g_pSem) 
		{
			CTools_Win32::TakeWinSem(g_pSem);
		}

		TmpUserMap=g_TcpUsers;

		if(g_pSem) 
		{
			CTools_Win32::GiveWinSem(g_pSem);
		}


		while(g_pQuotCache->GetFrom(&QuotData))
		{
			QuotData.InstrumentID[sizeof(QuotData.InstrumentID)-1]=0;
			for(it=TmpUserMap.begin();it!=TmpUserMap.end();it++)
			{
				if(!it->second.bLogin) continue;
				if(it->second.ReqType==2)
				{
					it1=g_MainInstruments.find(std::string(QuotData.InstrumentID));
					if(it1==g_MainInstruments.end())
						continue;
				}
				if(it->second.ReqType==1) QuotData.Cmd=CMD_Quot1;
				else if(it->second.ReqType==2) QuotData.Cmd=CMD_Quot2;
				else if(it->second.ReqType==3) QuotData.Cmd=CMD_Quot3;
				else continue;

				if(it->second.ReqType==1||it->second.ReqType==2)
				{
					Stru_Quot QuotData1;
					QuotData1.Head=QuotData.Head;
					QuotData1.PackageLen=QuotData.PackageLen;
					QuotData1.CRCValue=0;
					QuotData1.Cmd=QuotData.Cmd;
					memcpy(QuotData1.InstrumentID,QuotData.InstrumentID,sizeof(QuotData1.InstrumentID));
					QuotData1.LastPrice=QuotData.LastPrice;
					QuotData1.Volume=QuotData.Volume;
					QuotData1.OpenInterest=QuotData.OpenInterest;
					QuotData1.PreSettlementPrice=QuotData.PreSettlementPrice;
					QuotData1.PreClosePrice=QuotData.PreClosePrice;
					QuotData1.UpdateTime_utc=QuotData.UpdateTime_utc;
					QuotData1.UpdateTimeMS=QuotData.UpdateTimeMS;
					QuotData1.Tail=QuotData.Tail;

                    CSocketBase::SendDataTcp(it->first,&QuotData1,sizeof(QuotData1));
				}
				else if(it->second.ReqType==3)
				{
                    CSocketBase::SendDataTcp(it->first,&QuotData,sizeof(QuotData));
				}

                if(g_pWriteLog)
                {
                    char buf[10240];
                    sprintf(buf,"%s:%d : %s,%dbytes",
							CTools_Ansi::ConvertdwIP(it->second.RemoteAddr.IP),
							it->second.RemoteAddr.Port,
							QuotData.InstrumentID,
                            sizeof(QuotData));
                    g_pWriteLog->WriteLog_Fmt("�ַ�����",WriteLogLevel_DEBUGINFO,"%s",buf);
                    if(!CWinService::IsRunInServiceMode())
                        printf("%s\n",buf);
                }
			}
		}
	}

}
