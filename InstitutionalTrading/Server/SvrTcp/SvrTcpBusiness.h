/************************************************************************************************
����SvrTcp���������࣬Ҳ�ǽӿ�������ࡣ
�ӿ���Ľӿڷ����ڴ�����ʵ�֡�
��ʵ����
************************************************************************************************/

#pragma once

#include <time.h>
#include <map>
using namespace std;
#include "easymutex.h"
//#include "SvrTcp.h"
#include "SocketBase.h"
#include "Interface_SvrTcp.h"

//ÿ��tcp���ӵĽ��ջ�������С
#define RcvBufSize (2*1024*1024)


class CSvrTcpBusiness : public CInterface_SvrTcp
{
    //��ȡ/����Singleton��. ���캯��Ϊprivate, ��������Ϊpublic(Ϊ����auto_ptr����������)
    public: static CSvrTcpBusiness& getObj(void);
    private:static auto_ptr<CSvrTcpBusiness> m_pObj;
    private: CSvrTcpBusiness();
    public: virtual ~CSvrTcpBusiness();



	//�����ǽӿڷ�����������ҵ��ģ����ʹ��
public:
	//ҵ��ģ�鶩����Ϣ����ָ����ϢID���߳�ID�������յ����ݰ���Post����Ӧ�߳���
	virtual void SubscribePkg(unsigned int CmdID,unsigned int idThread);
	//ģ���˶���Ϣ����ָ����ϢID���߳�ID
	virtual void UnsubscribePkg(unsigned int CmdID,unsigned int idThread);
	//ģ���˶�ȫ����Ϣ����ָ����ϢID���߳�ID
	virtual void UnsubscribeAllPkg(unsigned int idThread);

	//�ж�Socket����Ƿ���TcpSvr�ľ����true: ��һ��TcpSvr accept�õ�������socket; false: ��һ��TcpClient��socket
	virtual bool IsTcpSvr(int socket);
	//��ȡSocket�����Ӧ��Զ�˵�ַ
	virtual bool GetRemoteAddr(int socket,unsigned int& outip,unsigned short& outport);
	//��ȡ����TcpClient���ӵ�socket�����Զ�˵�ַ
	virtual void GetSocket_RemoteAddr_of_TcpClient(map<int,pair<unsigned int,unsigned short>>& outData);
	//��ȡ����TcpSvr���ӵ�socket�����Զ�˵�ַ
	virtual void GetSocket_RemoteAddr_of_TcpServer(map<int,pair<unsigned int,unsigned short>>& outData);

	//��ȡ������RequestID
	virtual unsigned int get_inc_RequestID(void);

	//��������
	virtual bool SendData(int hsocket,const char* pData,int DataLen);
	//��������, ��sendData��һ����װ
	virtual bool SendPkgData(int hsocket,unsigned int nCMDID, const void* pData, int nLen, unsigned int seq,
		unsigned int Subseq=0,unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0);

	//�ر�һ���ͻ���socket
	void Disconnect(int hsocket);

	//��ȡEventParam�����ö�������ȡ�¼���������
	virtual CEventParam& getEventParamObj(void);

	//����/��ȡSocket�ĵ�¼��Ϣ
	virtual bool SetLoginInfo(int hsocket,const Stru_LoginInfo& logininfo);
	virtual bool GetLoginInfo(int hsocket,Stru_LoginInfo& logininfo);

	//��ȡ�������ӵ�������������socket��������������Ǵӷ��񣬻�ȡ�������socket��������������������񣬻�ȡ�Ѿ���¼�ɹ������дӷ�����socket
	virtual set<int> GetAllServerSockets();


	//����ķ�����SvrTcp��ʹ��
public:
	void ClearConnections(void);
	void InsertConnection(Stru_IPAddress& LocalAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket,bool bTcpSvr);
	void EraseConnection(SOCKET hSocket);
	void RecvData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
	void DisconnectTimeoutConnection(void);

public:
	//ÿ�����ӵ��������
	struct Stru_TcpConnection
	{
		Stru_TcpConnection()
		:hSocket(INVALID_SOCKET),
		 bTcpSvr(true),
		 pRcvData(NULL),
		 RcvDataLen(0),
		 RxTime(0),
		 LoginInfo()
		{
		}
		Stru_TcpConnection(SOCKET hsocket,Stru_IPAddress localAddr,Stru_IPAddress remoteAddr,bool btcpSvr)
		:hSocket(hsocket),
		 LocalAddr(localAddr),
		 RemoteAddr(remoteAddr),
		 bTcpSvr(btcpSvr),
		 pRcvData(NULL),
		 RcvDataLen(0),
		 RxTime(0),
		 LoginInfo(hsocket)
		{
			pRcvData=(char*)malloc(RcvBufSize);
			RcvDataLen=0;
			RxTime=time(NULL);
		}
		Stru_TcpConnection(const Stru_TcpConnection& r)
		:hSocket(INVALID_SOCKET),
		 bTcpSvr(true),
		 pRcvData(NULL),
		 RcvDataLen(0),
		 RxTime(0)
		{
			(*this)=r;
		}
		Stru_TcpConnection& operator= (const Stru_TcpConnection& r)
		{
			if(pRcvData)
			{
				free(pRcvData);
				pRcvData=NULL;
			}

			hSocket=r.hSocket;
			LocalAddr=r.LocalAddr;
			RemoteAddr=r.RemoteAddr;
			bTcpSvr=r.bTcpSvr;
			pRcvData=(char*)malloc(RcvBufSize);
			RcvDataLen=r.RcvDataLen;
			RxTime=r.RxTime;
			LoginInfo=r.LoginInfo;

			if(pRcvData&&r.pRcvData&&RcvDataLen>0)
				memcpy(pRcvData,r.pRcvData,RcvDataLen);

			//��Ҫ��r.hSocket����ΪINVALID_SOCKET������r����ʱ�رո�hSocket
			const_cast<Stru_TcpConnection&>(r).hSocket=INVALID_SOCKET;
			return *this;
		}
		~Stru_TcpConnection()
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
		Stru_IPAddress LocalAddr;
		Stru_IPAddress RemoteAddr;
		//true:TcpSvr, false: TcpClient
		bool	bTcpSvr;
		//Socket�Ľ��ջ����� 
		char*	pRcvData;
		//Socket���ջ���������Ч���ݵĳ���
		int		RcvDataLen;
		//Socket������ʱ�䡣1������û�н��յ����ݣ���Ͽ����ӡ��ͻ�����Ҫ��20s��������������˷�������
		time_t	RxTime;

		//���ӵĵ�¼��Ϣ
		Stru_LoginInfo LoginInfo;
	};



private:
	//ά��Ψһ��RequestID
	unsigned int		m_baseRequestID;

	//������
	Ceasymutex			m_mutex;

	//��ǰ����Socket�ļ���
	map<SOCKET,Stru_TcpConnection> m_Connections;
	//�������ӵ���������socket��������������Ǵӷ��񣬰���������socket��������������������񣬰��������ѵ�¼�Ĵӷ����socket
	set<int>			m_ServerScokets;		

	//�������ݰ����߳�map, cmdid -> idThread��
	//�յ����ݰ�ʱ����PostThreadMessage()�����ݰ����͸����ĵ��̡߳�
	//wParam=WndCmd_YourPkgArrival2
	//lParam=EventParamID��EventParam��pEventData�ǰ����ݣ�Stru_UniPkgHead + ���� + ��β(4)��userint=hSocket
	multimap<unsigned int,unsigned int> m_cmdid2idthread;
};



