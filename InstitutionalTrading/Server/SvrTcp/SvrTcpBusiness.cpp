// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
//#include "SvrTcp.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "Tools_Ansi.h"
#include "SvrTcpBusiness.h"
#include "tmpstackbuf.h"
#include "CommonDef.h"
#include "CommonMacro.h"
#include "FileOpr.h"
#include "tools_ansi.h"

#ifdef _DEBUG
#pragma comment(lib, "Tools4dllD.lib")											
#else 
#pragma comment(lib, "Tools4dll.lib")											
#endif



#if 0
	#define LOG_INFO(fmt, ...)  ;
#else 
	#if 0	//write to log file
		#define LOG_INFO(fmt, ...)   CFileOpr::getObj().writelocallogDaily("log","SvrTcp", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
	#else	//write to log file, and printf to stdio
		#define LOG_INFO(fmt, ...)   CFileOpr::getObj().writelocallogDaily3("log","SvrTcp", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
	#endif
#endif


auto_ptr<CSvrTcpBusiness> CSvrTcpBusiness::m_pObj;




//***************************************************************************************************
//	��ʵ����Ĵ�������������
//***************************************************************************************************

//��ȡ���Ψһʵ��ָ�롣��������������ֻ�ᴴ��һ�Ρ�
CSvrTcpBusiness& CSvrTcpBusiness::getObj(void)
{
#ifdef WIN32

    //��running��InterlockedCompareExchange��֤���⣬ʵ�ֶദ���������µ��̻߳��⡣volatile����������Ż�
    static volatile long running=0;
    while(InterlockedCompareExchange(&running,1,0)==1) Sleep(0);
    if(m_pObj.get()==NULL) 
        m_pObj.reset(new CSvrTcpBusiness());
    InterlockedExchange(&running,0);

#else
#endif
    return *m_pObj;
}

CSvrTcpBusiness::CSvrTcpBusiness()
:m_baseRequestID(0)
{
}

CSvrTcpBusiness::~CSvrTcpBusiness()
{
	m_pObj.release();
}



//***************************************************************************************************
//	�ӿڷ���ʵ��
//***************************************************************************************************

//ҵ��ģ�鶩����Ϣ����ָ����ϢID���߳�ID�������յ����ݰ���Post����Ӧ�߳���
void CSvrTcpBusiness::SubscribePkg(unsigned int CmdID,unsigned int idThread)
{
	CeasymutexGuard guard(m_mutex);

	if(!CTools_Ansi::IsKeyValueInMultimap<unsigned int,unsigned int>(m_cmdid2idthread,CmdID,idThread))
		m_cmdid2idthread.insert(pair<unsigned int,unsigned int>(CmdID,idThread));
}

//ģ���˶���Ϣ����ָ����ϢID���߳�ID
void CSvrTcpBusiness::UnsubscribePkg(unsigned int CmdID,unsigned int idThread)
{
	CeasymutexGuard guard(m_mutex);

	CTools_Ansi::EraseKeyValueInMultimap<unsigned int,unsigned int>(m_cmdid2idthread,CmdID,idThread);
}

//ģ���˶�ȫ����Ϣ����ָ����ϢID���߳�ID
void CSvrTcpBusiness::UnsubscribeAllPkg(unsigned int idThread)
{
	CeasymutexGuard guard(m_mutex);

	CTools_Ansi::EraseValueInMultimap<unsigned int,unsigned int>(m_cmdid2idthread,idThread);
}

//�ж�Socket����Ƿ���TcpSvr�ľ����true: ��һ��TcpSvr accept�õ�������socket; false: ��һ��TcpClient��socket
bool CSvrTcpBusiness::IsTcpSvr(int socket)
{
	CeasymutexGuard guard(m_mutex);

	map<SOCKET,Stru_TcpConnection>::const_iterator it=m_Connections.find((SOCKET)socket);
	if(it==m_Connections.end()) return false;
	return it->second.bTcpSvr;
}

//��ȡSocket�����Ӧ��Զ�˵�ַ
bool CSvrTcpBusiness::GetRemoteAddr(int socket,unsigned int& outip,unsigned short& outport)
{
	CeasymutexGuard guard(m_mutex);

	map<SOCKET,Stru_TcpConnection>::const_iterator it=m_Connections.find((SOCKET)socket);
	if(it==m_Connections.end()) return false;
	outip=it->second.RemoteAddr.IP;
	outport=it->second.RemoteAddr.Port;
	return true;
}
//��ȡ����TcpClient���ӵ�socket�����Զ�˵�ַ
void CSvrTcpBusiness::GetSocket_RemoteAddr_of_TcpClient(map<int,pair<unsigned int,unsigned short>>& outData)
{
	CeasymutexGuard guard(m_mutex);

	outData.clear();
	for(map<SOCKET,Stru_TcpConnection>::const_iterator it=m_Connections.begin();it!=m_Connections.end();it++)
	{
		if(!(it->second.bTcpSvr))
		{
			outData[it->first]=make_pair<unsigned int,unsigned short>(it->second.RemoteAddr.IP,it->second.RemoteAddr.Port);
		}
	}
}
//��ȡ����TcpSvr���ӵ�socket�����Զ�˵�ַ
void CSvrTcpBusiness::GetSocket_RemoteAddr_of_TcpServer(map<int,pair<unsigned int,unsigned short>>& outData)
{
	CeasymutexGuard guard(m_mutex);

	outData.clear();
	for(map<SOCKET,Stru_TcpConnection>::const_iterator it=m_Connections.begin();it!=m_Connections.end();it++)
	{
		if(it->second.bTcpSvr)
		{
			outData[it->first]=make_pair<unsigned int,unsigned short>(it->second.RemoteAddr.IP,it->second.RemoteAddr.Port);
		}
	}
}

//��ȡ������RequestID
unsigned int CSvrTcpBusiness::get_inc_RequestID(void)
{
	CeasymutexGuard guard(m_mutex);

	unsigned int irlt=m_baseRequestID;
	++m_baseRequestID;
	return irlt;
}

//��������
bool CSvrTcpBusiness::SendData(int hsocket,const char* pData,int DataLen)
{
	bool brlt=false;
	map<SOCKET,Stru_TcpConnection>::iterator it=m_Connections.find((SOCKET)hsocket);
	if(it!=m_Connections.end()&&it->second.hSocket!=INVALID_SOCKET)
	{
		int sentlen= CSocketBase::SendDataTcp(it->second.hSocket,pData,DataLen);
		if(sentlen==DataLen) brlt=true;
	}

	return brlt;
}

//��������, ��sendData��һ����װ
bool CSvrTcpBusiness::SendPkgData(int hsocket,unsigned int nCMDID, const void* pData, int nLen, unsigned int seq,
									unsigned int Subseq,unsigned int Userdata1,unsigned int Userdata2,unsigned int Userdata3,unsigned int Userdata4)
{
	CeasymutexGuard guard(m_mutex);

	bool brlt=false;
	
	OUTPUT_LOG("����˷�����Ϣ�� %d",  nCMDID);

	map<SOCKET,Stru_TcpConnection>::iterator it=m_Connections.find((SOCKET)hsocket);
	if(it!=m_Connections.end()&&it->second.hSocket!=INVALID_SOCKET)
	{
		int PkgLen=sizeof(Stru_UniPkgHead)+nLen+4;
		AllocTmpStackBuf(tmpbuf,PkgLen);
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

	return brlt;
}


CEventParam& CSvrTcpBusiness::getEventParamObj(void)
{
	return CEventParam::getObj();
}

//�ر�һ���ͻ���socket
void CSvrTcpBusiness::Disconnect(int hsocket)
{
	EraseConnection((SOCKET)hsocket);
}



//***************************************************************************************************
//	ģ���ڲ�ʹ��֮����ʵ��
//***************************************************************************************************

void CSvrTcpBusiness::ClearConnections(void)
{
	CeasymutexGuard guard(m_mutex);

	m_Connections.clear();
	m_ServerScokets.clear();
}

void CSvrTcpBusiness::InsertConnection(Stru_IPAddress& LocalAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket,bool bTcpSvr)
{
	CeasymutexGuard guard(m_mutex);

	map<SOCKET,Stru_TcpConnection>::iterator it=m_Connections.find(hSocket);
	if(it==m_Connections.end())
		m_Connections[hSocket]=Stru_TcpConnection(hSocket,LocalAddr,RemoteAddr,bTcpSvr);

	//���������ӽ�����Ϣ���̷߳�������
	struct 
	{
		Stru_UniPkgHead Head;
		int Tail;
	}Pkg;
	if(bTcpSvr) Pkg.Head.Set(0,CMDID_SvrTcpAccept,0,0,0,hSocket,RemoteAddr.IP,RemoteAddr.Port);
	else Pkg.Head.Set(0,CMDID_TcpClientConnect,0,0,0,hSocket,RemoteAddr.IP,RemoteAddr.Port);
	Pkg.Tail=TailID_UniPkg;
	multimap<unsigned int,unsigned int>::iterator it2lower=m_cmdid2idthread.lower_bound(Pkg.Head.cmdid);
	multimap<unsigned int,unsigned int>::iterator it2upper=m_cmdid2idthread.upper_bound(Pkg.Head.cmdid);
	for(;it2lower!=it2upper;it2lower++)
	{
		unsigned int EventParamID;
		if(CEventParam::getObj().CreateEventParam(EventParamID,NULL,NULL,&Pkg,sizeof(Pkg),(int)hSocket))
			PostThreadMessage(it2lower->second,WM_COMMAND,WndCmd_YourPkgArrival2,(LPARAM)EventParamID);
	}

	if(1)
	{
		char buf1[128],buf2[128];
		LOG_INFO("���յ�Tcp���ӣ�hSocket=%d, ���ص�ַΪ%s:%u Զ�˵�ַΪ%s:%u %s",(int)hSocket,CTools_Ansi::ConvertdwIP2(LocalAddr.IP,buf1,sizeof(buf1)),LocalAddr.Port,CTools_Ansi::ConvertdwIP2(RemoteAddr.IP,buf2,sizeof(buf2)),RemoteAddr.Port,bTcpSvr?"TcpSvr":"TcpClient");
	}
}

void CSvrTcpBusiness::EraseConnection(SOCKET hSocket)
{
	CeasymutexGuard guard(m_mutex);

    map<SOCKET,Stru_TcpConnection>::iterator it=m_Connections.find(hSocket);
    if(it!=m_Connections.end()) 
	{
		if(1)
		{
			char buf1[128],buf2[128];
			LOG_INFO("�Ͽ�Tcp���ӣ�hSocket=%d, ���ص�ַΪ%s:%u Զ�˵�ַΪ%s:%u",(int)hSocket,CTools_Ansi::ConvertdwIP2(it->second.LocalAddr.IP,buf1,sizeof(buf1)),it->second.LocalAddr.Port,CTools_Ansi::ConvertdwIP2(it->second.RemoteAddr.IP,buf2,sizeof(buf2)),it->second.RemoteAddr.Port);
		}

		//���������ӶϿ���Ϣ���̷߳�������
		struct 
		{
			Stru_UniPkgHead Head;
			int Tail;
		}Pkg;
		if(it->second.bTcpSvr) Pkg.Head.Set(0,CMDID_SvrTcpDisconn,0,0,0,hSocket,it->second.RemoteAddr.IP,it->second.RemoteAddr.Port);
		else Pkg.Head.Set(0,CMDID_TcpClientDisconn,0,0,0,hSocket,it->second.RemoteAddr.IP,it->second.RemoteAddr.Port);
		Pkg.Tail=TailID_UniPkg;
		multimap<unsigned int,unsigned int>::iterator it2lower=m_cmdid2idthread.lower_bound(Pkg.Head.cmdid);
		multimap<unsigned int,unsigned int>::iterator it2upper=m_cmdid2idthread.upper_bound(Pkg.Head.cmdid);
		for(;it2lower!=it2upper;it2lower++)
		{
			unsigned int EventParamID;
			if(CEventParam::getObj().CreateEventParam(EventParamID,NULL,NULL,&Pkg,sizeof(Pkg),(int)hSocket))
				PostThreadMessage(it2lower->second,WM_COMMAND,WndCmd_YourPkgArrival2,(LPARAM)EventParamID);
		}

		m_Connections.erase(it);
	}
	CTools_Ansi::EraseInSet<int>(m_ServerScokets,(int)hSocket);
}

void CSvrTcpBusiness::RecvData(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CeasymutexGuard guard(m_mutex);

    map<SOCKET,Stru_TcpConnection>::iterator it=m_Connections.find(hSocket);
    if(it!=m_Connections.end()&&
		it->second.pRcvData!=NULL&&
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

			if(off>lastoff)
				LOG_INFO("������Ч����, hSocket=%d, Len=%d",(int)hSocket,off-lastoff);

			int LengthUsedByPkg;
			if(!Stru_UniPkgHead::IsValidPkg(it->second.pRcvData+off,it->second.RcvDataLen-off,&LengthUsedByPkg))
				break;

			//�ַ������				
			Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)(it->second.pRcvData+off);
			//LOG_INFO("Rev a cmd package: cmd ID %x pkglen %d from %d",PkgHead.cmdid,PkgHead.len,hSocket );
			multimap<unsigned int,unsigned int>::iterator it2lower=m_cmdid2idthread.lower_bound(PkgHead.cmdid);
			multimap<unsigned int,unsigned int>::iterator it2upper=m_cmdid2idthread.upper_bound(PkgHead.cmdid);
			for(;it2lower!=it2upper;it2lower++)
			{
				unsigned int EventParamID;
				if(CEventParam::getObj().CreateEventParam(EventParamID,NULL,NULL,it->second.pRcvData+off,LengthUsedByPkg,(int)hSocket))
					if(!PostThreadMessage(it2lower->second,WM_COMMAND,WndCmd_YourPkgArrival2,(LPARAM)EventParamID))
					{
						LOG_INFO("PostTheadMessage Faild: Event ID %d ,Error %d ThradeId %d",EventParamID,GetLastError(),it2lower->second );
					}
			}

			off+=LengthUsedByPkg;

			//LOG_INFO("���ղ��ַ����ݰ���hSocket=%d, CmdID=%u, DataLen=%d\n",(int)hSocket,PkgHead.cmdid,PkgHead.len);

		}

		if(off>0)
	    {
			memmove(it->second.pRcvData,it->second.pRcvData+off,it->second.RcvDataLen-off);
		    it->second.RcvDataLen -= off;
	    }
	}
}

void CSvrTcpBusiness::DisconnectTimeoutConnection(void)
{
	m_mutex.lock();
	time_t curtime=time(NULL);
	vector<SOCKET> TimeoutSockets;
	for(map<SOCKET,Stru_TcpConnection>::iterator it=m_Connections.begin();it!=m_Connections.end();it++)
	{
		//����TcpSvr, �Ͽ�û���������1�������ϵ����ӡ��ͻ�����Ҫ��20s��������������˷�������
		if(it->second.hSocket!=INVALID_SOCKET&&it->second.bTcpSvr&&curtime-it->second.RxTime>=60)
			TimeoutSockets.push_back(it->first);
	}
	m_mutex.unlock();

	for(int i=0;i<(int)TimeoutSockets.size();i++)
		EraseConnection(TimeoutSockets[i]);
}


//����/��ȡSocket�ĵ�¼��Ϣ
bool CSvrTcpBusiness::SetLoginInfo(int hsocket,const Stru_LoginInfo& logininfo)
{
	CeasymutexGuard guard(m_mutex);
	map<SOCKET,Stru_TcpConnection>::iterator it=m_Connections.find((SOCKET)hsocket);
	if(it==m_Connections.end()) return false;
	it->second.LoginInfo=logininfo;

	if(logininfo.Usertype==USER_TYPE_MASTERSERVER||logininfo.Usertype==USER_TYPE_SLAVESERVER&&logininfo.bLogin)
		m_ServerScokets.insert(hsocket);

	return true;
}
bool CSvrTcpBusiness::GetLoginInfo(int hsocket,Stru_LoginInfo& logininfo)
{
	CeasymutexGuard guard(m_mutex);
	map<SOCKET,Stru_TcpConnection>::iterator it=m_Connections.find((SOCKET)hsocket);
	if(it==m_Connections.end()) return false;
	logininfo=it->second.LoginInfo;
	return true;
}

//��ȡ�������ӵ�������������socket��������������Ǵӷ��񣬻�ȡ�������socket��������������������񣬻�ȡ�Ѿ���¼�ɹ������дӷ�����socket
set<int> CSvrTcpBusiness::GetAllServerSockets()
{
	return m_ServerScokets;
}

