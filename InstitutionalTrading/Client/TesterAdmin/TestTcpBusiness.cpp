// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "Tools_Ansi.h"
#include "TestTcpBusiness.h"
#include "tmpstackbuf.h"
#include "CommonDef.h"




#define WRITELOGID_SvrTcp




CTestTcpBusiness*	CTestTcpBusiness::m_pObj=NULL;
Ceasymutex			CTestTcpBusiness::m_mutex;

CTestTcpBusiness::CTestTcpBusiness()
:m_baseRequestID(0)
{
}

CTestTcpBusiness::~CTestTcpBusiness()
{
}

CTestTcpBusiness& CTestTcpBusiness::getObj(void)
{
    m_mutex.lock();
    if(!m_pObj)
        m_pObj=new CTestTcpBusiness();
    m_mutex.unlock();
    return *m_pObj;
}




void CTestTcpBusiness::ClearConnections(void)
{
    m_mutex.lock();
	m_Connections.clear();
    m_mutex.unlock();
}

void CTestTcpBusiness::InsertConnection(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
    m_mutex.lock();
	map<SOCKET,Stru_SvrTcpConnection>::iterator it=m_Connections.find(hSocket);
	if(it==m_Connections.end())
		m_Connections[hSocket]=Stru_SvrTcpConnection(hSocket,RemoteAddr);
    m_mutex.unlock();

#ifdef WRITELOGID_SvrTcp
	printf("���յ�Tcp���ӣ�hSocket=%d, ��ַΪ%s:%u\n",(int)hSocket,CTools_Ansi::ConvertdwIP(RemoteAddr.IP),RemoteAddr.Port);
#endif

	//���������ӶϿ���Ϣ���̷߳�������
	struct 
	{
		Stru_UniPkgHead Head;
		int Tail;
	}Pkg;
	Pkg.Head.Set(0,CMDID_SvrTcpCon,0,0);
	Pkg.Tail=TailID_UniPkg;
	multimap<unsigned int,unsigned int>::iterator it2lower=m_cmdid2idthread.lower_bound(Pkg.Head.cmdid);
	multimap<unsigned int,unsigned int>::iterator it2upper=m_cmdid2idthread.upper_bound(Pkg.Head.cmdid);
	for(;it2lower!=it2upper;it2lower++)
	{
	/*	unsigned int EventParamID;
		if(CEventParam::getObj().CreateEventParam(EventParamID,NULL,NULL,&Pkg,sizeof(Pkg),(int)hSocket))
			PostThreadMessage(it2lower->second,WM_USER_Win,WndCmd_YourPkgArrival2,(LPARAM)EventParamID);*/
		if(IsWindow((HWND)(it2lower->second)))
			SendMessage((HWND)(it2lower->second),WM_TCP_MESSAGE,CMDID_SvrTcpCon,(LPARAM)hSocket);

	}

}

//�ر�һ���ͻ���socket
void CTestTcpBusiness::Disconnect(int hsocket)
{
	EraseConnection(Stru_IPAddress(),Stru_IPAddress(),(SOCKET)hsocket);
}

void CTestTcpBusiness::EraseConnection(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
#ifdef WRITELOGID_SvrTcp
	printf("�Ͽ�Tcp���ӣ�hSocket=%d, ��ַΪ%s:%u\n",(int)hSocket,CTools_Ansi::ConvertdwIP(RemoteAddr.IP),RemoteAddr.Port);
#endif

	m_mutex.lock();
    map<SOCKET,Stru_SvrTcpConnection>::iterator it=m_Connections.find(hSocket);
    if(it!=m_Connections.end()) 
		m_Connections.erase(it);

	//���������ӶϿ���Ϣ���̷߳�������
	struct 
	{
		Stru_UniPkgHead Head;
		int Tail;
	}Pkg;
	Pkg.Head.Set(0,CMDID_SvrTcpDisconn,0,0);
	Pkg.Tail=TailID_UniPkg;
	multimap<unsigned int,unsigned int>::iterator it2lower=m_cmdid2idthread.lower_bound(Pkg.Head.cmdid);
	multimap<unsigned int,unsigned int>::iterator it2upper=m_cmdid2idthread.upper_bound(Pkg.Head.cmdid);
	for(;it2lower!=it2upper;it2lower++)
	{
	/*	unsigned int EventParamID;
		if(CEventParam::getObj().CreateEventParam(EventParamID,NULL,NULL,&Pkg,sizeof(Pkg),(int)hSocket))
			PostThreadMessage(it2lower->second,WM_USER_Win,WndCmd_YourPkgArrival2,(LPARAM)EventParamID);*/
		if(IsWindow((HWND)(it2lower->second)) )
			SendMessage((HWND)(it2lower->second),WM_TCP_MESSAGE,CMDID_SvrTcpDisconn,(LPARAM)hSocket);

	}

    m_mutex.unlock();
}

void CTestTcpBusiness::RecvData(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	m_mutex.lock();

    map<SOCKET,Stru_SvrTcpConnection>::iterator it=m_Connections.find(hSocket);
    if(it!=m_Connections.end()&&
		it->second.RcvDataLen+DataLen<=RcvBufSize) 
	{
		memcpy(it->second.pRcvData+it->second.RcvDataLen,pData,DataLen);
		it->second.RcvDataLen+=DataLen;
		it->second.RxTime=time(NULL);

	    //ѭ���������յ�������
		int off=0;
		while(1)
		{
			//���������ܳ��ֵ���Ч����
			int lastoff=off;
			while(off<it->second.RcvDataLen&&Stru_UniPkgHead::IsInvalidPkgHead(it->second.pRcvData+off,it->second.RcvDataLen-off)) off++;
#ifdef WRITELOGID_SvrTcp
			if(off>lastoff)
				printf("������Ч���ݣ�hSocket=%d, Len=%d\n",(int)hSocket,off-lastoff);
#endif

			int LengthUsedByPkg;
			if(!Stru_UniPkgHead::IsValidPkg(it->second.pRcvData+off,it->second.RcvDataLen-off,&LengthUsedByPkg))
				break;

			//�ַ������
			Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)(it->second.pRcvData+off);
			multimap<unsigned int,unsigned int>::iterator it2lower=m_cmdid2idthread.lower_bound(PkgHead.cmdid);
			multimap<unsigned int,unsigned int>::iterator it2upper=m_cmdid2idthread.upper_bound(PkgHead.cmdid);
			for(;it2lower!=it2upper;it2lower++)
			{
				/*unsigned int EventParamID;
				if(CEventParam::getObj().CreateEventParam(EventParamID,NULL,NULL,it->second.pRcvData+off,LengthUsedByPkg,(int)hSocket))
					PostThreadMessage(it2lower->second,WM_TCP_MESSAGE,WndCmd_YourPkgArrival2,(LPARAM)EventParamID);*/

				if(IsWindow((HWND)(it2lower->second)) )
					SendMessage((HWND)(it2lower->second),WM_TCP_MESSAGE,PkgHead.cmdid,(LPARAM)&PkgHead);
			}

			off+=LengthUsedByPkg;

#ifdef WRITELOGID_SvrTcp
			printf("���ղ��ַ����ݰ���hSocket=%d, CmdID=%u, DataLen=%d\n",(int)hSocket,PkgHead.cmdid,PkgHead.len);
#endif

		}

		if(off>0)
	    {
			memmove(it->second.pRcvData,it->second.pRcvData+off,it->second.RcvDataLen-off);
		    it->second.RcvDataLen -= off;
	    }
	}

	m_mutex.unlock();
}

void CTestTcpBusiness::DisconnectTimeoutConnection(void)
{
	m_mutex.lock();

	time_t curtime=time(NULL);
	for(map<SOCKET,Stru_SvrTcpConnection>::iterator it=m_Connections.begin();it!=m_Connections.end();)
	{
		//�Ͽ�û���������1�������ϵ����ӡ��ͻ�����Ҫ��20s��������������˷�������
		if(it->second.hSocket!=INVALID_SOCKET&&
			curtime-it->second.RxTime>=60)
		{
			it=m_Connections.erase(it);
		}
		else it++;
	}

	m_mutex.unlock();
}



//***************************************************************************************************
//	�����ǽӿڷ���ʵ��
//***************************************************************************************************


//ҵ��ģ�鶩����Ϣ����ָ����ϢID���߳�ID�������յ����ݰ���Post����Ӧ�߳���
void CTestTcpBusiness::SubscribePkg(unsigned int CmdID,unsigned int idThread)
{
	m_mutex.lock();

	multimap<unsigned int,unsigned int>::iterator itlower=m_cmdid2idthread.lower_bound(CmdID);
	multimap<unsigned int,unsigned int>::iterator itupper=m_cmdid2idthread.upper_bound(CmdID);

	while(itlower!=itupper&&itlower->second!=idThread) itlower++;
	if(itlower==itupper)
		m_cmdid2idthread.insert(pair<unsigned int,unsigned int>(CmdID,idThread));

	m_mutex.unlock();
}

//ģ���˶���Ϣ����ָ����ϢID���߳�ID
void CTestTcpBusiness::UnsubscribePkg(unsigned int CmdID,unsigned int idThread)
{
	m_mutex.lock();

	multimap<unsigned int,unsigned int>::iterator itlower=m_cmdid2idthread.lower_bound(CmdID);
	multimap<unsigned int,unsigned int>::iterator itupper=m_cmdid2idthread.upper_bound(CmdID);

	while(itlower!=itupper&&itlower->second!=idThread) itlower++;
	if(itlower!=itupper)
		m_cmdid2idthread.erase(itlower);

	m_mutex.unlock();
}

//ģ���˶�ȫ����Ϣ����ָ����ϢID���߳�ID
void CTestTcpBusiness::UnsubscribeAllPkg(unsigned int idThread)
{
	m_mutex.lock();

	for(multimap<unsigned int,unsigned int>::iterator it=m_cmdid2idthread.begin();it!=m_cmdid2idthread.end();)
	{
		if(it->second==idThread)
			it=m_cmdid2idthread.erase(it);
		else it++;
	}

	m_mutex.unlock();
}

//��ȡ������RequestID
unsigned int CTestTcpBusiness::get_inc_RequestID(void)
{
	m_mutex.lock();
	unsigned int irlt=m_baseRequestID;
	++m_baseRequestID;
	m_mutex.unlock();
	return irlt;
}

//��������
bool CTestTcpBusiness::SendData(int hsocket,const char* pData,int DataLen)
{
	bool brlt=false;
	
	m_mutex.lock();

	map<SOCKET,Stru_SvrTcpConnection>::iterator it=m_Connections.find((SOCKET)hsocket);
	if(it!=m_Connections.end()&&it->second.hSocket!=INVALID_SOCKET)
	{
		int sentlen= CSocketBase::SendDataTcp(it->second.hSocket,pData,DataLen);
		if(sentlen==DataLen) brlt=true;
	}

	m_mutex.unlock();

	return brlt;
}

//��������, ��sendData��һ����װ
bool CTestTcpBusiness::SendPkgData(int hsocket,unsigned int nCMDID, void* pData, int nLen, unsigned int seq,
									unsigned int Subseq,unsigned int Userdata1,unsigned int Userdata2,unsigned int Userdata3,unsigned int Userdata4)
{
	bool brlt=false;
	
	m_mutex.lock();
	

	map<SOCKET,Stru_SvrTcpConnection>::iterator it=m_Connections.find((SOCKET)hsocket);
	if(it!=m_Connections.end()&&it->second.hSocket!=INVALID_SOCKET)
	{
		int PkgLen=sizeof(Stru_UniPkgHead)+nLen+4;
		AllocTmpStackBuf(tmpbuf,PkgLen,10*1024);
		if(tmpbuf.m_pbuf!=NULL)
		{
			Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)tmpbuf.m_pbuf;
			PkgHead.Set(nLen,nCMDID,0,seq,Subseq,Userdata1,Userdata2,Userdata3,Userdata4);
			if(pData&&nLen>0)
				memcpy((char*)tmpbuf.m_pbuf+sizeof(Stru_UniPkgHead),pData,nLen);
			*(unsigned int*)((char*)tmpbuf.m_pbuf+sizeof(Stru_UniPkgHead) + nLen)=TailID_UniPkg;

			int sentlen = CSocketBase::SendDataTcp(it->second.hSocket,tmpbuf.m_pbuf,PkgLen);
			if(sentlen==PkgLen) brlt=true;
		}
	}

	m_mutex.unlock();

	return brlt;
}
bool CTestTcpBusiness::BroadcastPkgData(unsigned int nCMDID, void* pData, int nLen, unsigned int seq,
					  unsigned int Subseq,unsigned int Userdata1,unsigned int Userdata2,unsigned int Userdata3,unsigned int Userdata4)
{
	bool brlt=false;

	m_mutex.lock();
	map<SOCKET,Stru_SvrTcpConnection>::iterator it=m_Connections.begin();
	for(;it != m_Connections.end();it++)
	{
		if(it->second.hSocket!=INVALID_SOCKET)
			SendPkgData(it->second.hSocket, nCMDID,  pData,  nLen,  seq, Subseq,Userdata1,Userdata2,Userdata3,Userdata4);
		
	}
	m_mutex.unlock();

	return brlt;

}
bool CTestTcpBusiness::GetIPAddress(int hSocket,string & nAdd)
{
	bool brlt=false;

	m_mutex.lock();


	map<SOCKET,Stru_SvrTcpConnection>::iterator it=m_Connections.find((SOCKET)hSocket);
	if(it != m_Connections.end())
	{
		nAdd = CTools_Ansi::ConvertdwIP(it->second.IPAddress.IP);
		brlt = true;
	}


	m_mutex.unlock();

	return brlt;
}