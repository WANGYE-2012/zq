/************************************************************************************************
����SvrTcp���������࣬Ҳ�ǽӿ�������ࡣ
�ӿ���Ľӿڷ����ڴ�����ʵ�֡�
************************************************************************************************/

#pragma once

#include <time.h>
#include <map>
using namespace std;
#include "easymutex.h"


#include <time.h>
#include <map>
using namespace std;
#include "SocketBase.h"
#define  WM_TCP_MESSAGE WM_USER+9

//ÿ��tcp���ӵĽ��ջ�������С
#define RcvBufSize (10*1024*1024)


//ÿ�����ӵ��������
struct Stru_SvrTcpConnection
{
	Stru_SvrTcpConnection()
		:hSocket(INVALID_SOCKET),
		pRcvData(NULL),
		RcvDataLen(0),
		RxTime(0)
	{
	}
	Stru_SvrTcpConnection(SOCKET hsocket,Stru_IPAddress & nAdd)
		:hSocket(INVALID_SOCKET),
		pRcvData(NULL),
		RcvDataLen(0),
		RxTime(0),
		IPAddress(nAdd)
	{
		hSocket=hsocket;
		pRcvData=(char*)malloc(RcvBufSize);
		RcvDataLen=0;
		RxTime=time(NULL);
	}
	Stru_SvrTcpConnection(const Stru_SvrTcpConnection& r)
		:hSocket(INVALID_SOCKET),
		pRcvData(NULL),
		RcvDataLen(0),
		RxTime(0)
	{
		(*this)=r;
	}
	Stru_SvrTcpConnection& operator= (const Stru_SvrTcpConnection& r)
	{
		if(pRcvData)
		{
			free(pRcvData);
			pRcvData=NULL;
		}

		hSocket=r.hSocket;
		pRcvData=(char*)malloc(RcvBufSize);
		RcvDataLen=r.RcvDataLen;
		RxTime=r.RxTime;
		IPAddress = r.IPAddress;

		if(pRcvData&&r.pRcvData&&RcvDataLen>0)
			memcpy(pRcvData,r.pRcvData,RcvDataLen);

		//��Ҫ��r.hSocket����ΪINVALID_SOCKET������r����ʱ�رո�hSocket
		const_cast<Stru_SvrTcpConnection&>(r).hSocket=INVALID_SOCKET;
		return *this;
	}
	~Stru_SvrTcpConnection()
	{
		if(pRcvData)
		{
			free(pRcvData);
			pRcvData=NULL;
		}
		if(hSocket!=INVALID_SOCKET)
		{
			CSocketBase::CloseSocket(hSocket);
			hSocket=INVALID_SOCKET;
		}
	}

	SOCKET	hSocket;
	//Socket�Ľ��ջ����� 
	char*	pRcvData;
	//Socket���ջ���������Ч���ݵĳ���
	int		RcvDataLen;
	//Socket������ʱ�䡣1������û�н��յ����ݣ���Ͽ����ӡ��ͻ�����Ҫ��20s��������������˷�������
	time_t	RxTime;
	//
	Stru_IPAddress IPAddress;
};


class CTestTcpBusiness 
{
	//����getObj()������/����������m_pObj���ڶ��嵥ʵ����
public:
	//���ڻ�ȡ�ӿڶ���ͨ���ö���ʹ�ýӿڷ���
	static CTestTcpBusiness& getObj(void);
private:
    CTestTcpBusiness();
    virtual ~ CTestTcpBusiness();
    static CTestTcpBusiness* m_pObj;


	//����ķ�����SvrTcp��ʹ��
public:
	void ClearConnections(void);
	void InsertConnection(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
	void EraseConnection(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
	void RecvData(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
	void DisconnectTimeoutConnection(void);
	//�ر�һ���ͻ���socket
	void Disconnect(int hsocket);

	//�����ǽӿڷ���

	//ҵ��ģ�鶩����Ϣ����ָ����ϢID���߳�ID�������յ����ݰ���Post����Ӧ�߳���
	virtual void SubscribePkg(unsigned int CmdID,unsigned int idThread);
	//ģ���˶���Ϣ����ָ����ϢID���߳�ID
	virtual void UnsubscribePkg(unsigned int CmdID,unsigned int idThread);
	//ģ���˶�ȫ����Ϣ����ָ����ϢID���߳�ID
	virtual void UnsubscribeAllPkg(unsigned int idThread);

	//��ȡ������RequestID
	virtual unsigned int get_inc_RequestID(void);

	//��������
	virtual bool SendData(int hsocket,const char* pData,int DataLen);
	//��������, ��sendData��һ����װ
	virtual bool SendPkgData(int hsocket,unsigned int nCMDID, void* pData, int nLen, unsigned int seq,
		unsigned int Subseq=0,unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0);


public:
	void AllocateUser(int hSocket,string & nUserName);
	bool GetIPAddress(int hSocket,string & nAdd);

	bool BroadcastPkgData(unsigned int nCMDID, void* pData, int nLen, unsigned int seq,
		unsigned int Subseq=0,unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0);
private:
	//ά��Ψһ��RequestID
	unsigned int		m_baseRequestID;

	//������
	static Ceasymutex			m_mutex;


	//��ǰ����Socket�ļ���
	map<SOCKET,Stru_SvrTcpConnection> m_Connections;
	multimap<string,SOCKET> m_UserSocket; //�ҵ���Ӧ�û���socket

	//�������ݰ����߳�map, cmdid -> hwnd��
	//�յ����ݰ�ʱ����PostThreadMessage()�����ݰ����͸����ĵ��̡߳�
	//wParam=WndCmd_YourPkgArrival2
	//lParam=EventParamID��EventParam��pEventData�ǰ����ݣ�Stru_UniPkgHead + ���� + ��β(4)��userint=hSocket
	multimap<unsigned int,unsigned int> m_cmdid2idthread;
};

