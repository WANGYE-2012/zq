// TcpPortMap.h: interface for the CTcpPortMap class.
//  ʵ��tcp�˿�ӳ�书�ܡ�
//  ��һ��TcpServerWorder�����յ�һ���ͻ�������ʱ������һ��TcpClientWorker�����ӵ�ӳ��Ŀ�ĵ�ַ
//  ӳ�䷽ʽ�����֣�ֱ��ӳ�䡢http����ӳ��
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPTRANSFER_H__3C8964B8_6067_4345_9088_DDD8A9AC212E__INCLUDED_)
#define AFX_TCPTRANSFER_H__3C8964B8_6067_4345_9088_DDD8A9AC212E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <map>
#include <vector>
#include <stdexcept>
#include <memory>
using std::map;
using std::vector;
using std::auto_ptr;

#include "easymutex.h"
#include "TcpServerWorker.h"
#include "TcpClientWorker.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 



class CTcpPortMap
{
public:
	enum eMapMode
	{
		Map_Direct,		        //ֱ��ӳ��
		Map_BypassHttpProxySvr	//ͨ��http proxy serverӳ��(������֤�û�������)
	};

    //�˿�ӳ�����������
    struct Stru_PortMapParam
    {
        Stru_PortMapParam()
        {
            MaxListen=16;
            MapMode=Map_Direct;
            SOSNDBUFSize=655360;
            SORCVBUFSize=655360;
        };
        Stru_PortMapParam(const Stru_IPAddress& bindAddr,const Stru_IPAddress& destAddr,eMapMode mapMode=Map_Direct,const Stru_IPAddress& proxyServerAddr=Stru_IPAddress())
        {
            new(this) Stru_PortMapParam();
            BindAddr=bindAddr;
            DestAddr=destAddr;
            MapMode=mapMode;
            ProxyServerAddr=proxyServerAddr;
        };

        Stru_IPAddress      BindAddr;
        int                 MaxListen;
        Stru_IPAddress	    DestAddr;
        eMapMode            MapMode;
        Stru_IPAddress      ProxyServerAddr;
        int                 SOSNDBUFSize;
        int                 SORCVBUFSize;

    };
    //�˿�ӳ�����������
    struct Stru_PortMapSvr
    {
        Stru_PortMapSvr()
        {
            pTcpSvr=NULL;
        }
        Stru_PortMapSvr(const Stru_PortMapParam& param,CTcpServerWorker* ptcpSvr)
        {
            Param=param;
            pTcpSvr=ptcpSvr;
        }

        Stru_PortMapParam   Param;
        CTcpServerWorker    *pTcpSvr;
    };

    //�˿�ӳ��ͻ�������
    struct Stru_PortMapClient
    {
        Stru_PortMapClient()
        {
            hDataSocket=SOCKET_ERROR;
            pTcpClient=NULL;
            ProxySvrConnStatus=ProxySvr_UnConnect;
            memset(DataBufLen_WaitingForHttpProxyConnSucc,0,sizeof(DataBufLen_WaitingForHttpProxyConnSucc));
        };
        Stru_PortMapClient(SOCKET hdataSocket,CTcpClientWorker* ptcpClient)
        {
            hDataSocket=hdataSocket;
            pTcpClient=ptcpClient;
            ProxySvrConnStatus=ProxySvr_UnConnect;
            memset(DataBufLen_WaitingForHttpProxyConnSucc,0,sizeof(DataBufLen_WaitingForHttpProxyConnSucc));
        };

	    enum eProxySvrConnStatus
	    {
            ProxySvr_UnConnect,     //δ����
            ProxySvr_Connecting,    //��������
            ProxySvr_ConnectFail,   //����ʧ��
            ProxySvr_ConnectSucc    //���ӳɹ�
	    };

        SOCKET              hDataSocket;
        CTcpClientWorker    *pTcpClient;

        //�ȴ�HttpProxy���ӳɹ����ͳ�ȥ�����ݡ����յ�HTTP/1.0 200 OK\r\n\r\n����
        //��Щ���ݿ��Է�Ϊ���16����
        const static int    MaxPkgCount_WaitingForHttpProxyConnSucc=16;
        int                 DataBufLen_WaitingForHttpProxyConnSucc[MaxPkgCount_WaitingForHttpProxyConnSucc];
        char                DataBuf_WaitingForHttpProxyConnSucc[65536];

        //�������������״̬������Map_BypassHttpProxySvr��Ч
        eProxySvrConnStatus ProxySvrConnStatus;

        bool SaveData_WaitingForHttpProxyConnSucc(const char* pData,int DataLen)
        {
            int i=0,off=0;
            while(i<MaxPkgCount_WaitingForHttpProxyConnSucc)
            {
                if(DataBufLen_WaitingForHttpProxyConnSucc[i]==0) break;
                off+=DataBufLen_WaitingForHttpProxyConnSucc[i];
                i++;
            }
            if(DataLen>(int)sizeof(DataBuf_WaitingForHttpProxyConnSucc)-off) return false;
            if(i==MaxPkgCount_WaitingForHttpProxyConnSucc)
                i=MaxPkgCount_WaitingForHttpProxyConnSucc-1;
            memcpy(DataBuf_WaitingForHttpProxyConnSucc+off,pData,DataLen);
            DataBufLen_WaitingForHttpProxyConnSucc[i]+=DataLen;
            return true;
        }
        bool IsHaveData_WaitingForHttpProxyConnSucc()
        {
            return DataBufLen_WaitingForHttpProxyConnSucc[0]==0?false:true;
        }
    };



    //��ȡ/����Singleton��. ���캯��Ϊprivate, ��������Ϊpublic(Ϊ����auto_ptr����������)
    public: static CTcpPortMap& getObj();
    private:static auto_ptr<CTcpPortMap> m_pObj;
    private: CTcpPortMap();
    public: virtual ~CTcpPortMap();

public:

    //ע�⣺���param.BindAddr.PortΪ0���򷵻�ʱ���޸�portΪϵͳָ����ֵ
    bool NewPortMap(Stru_PortMapParam& param);
    void DelPortMap(const Stru_IPAddress& bindaddr);
    void DelPortMap2(const Stru_IPAddress& destaddr);
    bool IsDestAddrExist(const Stru_IPAddress& DestAddr);
    void GetBindAddr(const Stru_IPAddress& destaddr,vector<Stru_IPAddress>& bindaddrs);


private:
    void DelPortMap_Internal(const Stru_IPAddress& bindaddr);
    void DelPortMap2_Internal(const Stru_IPAddress& destaddr);
    void DelPortMapClient_Internal(SOCKET hDataSocket);
    void GetAllBindAddr_Internal(vector<Stru_IPAddress>& bindaddrs);
    void GetBindAddr_Internal(const Stru_IPAddress& destaddr,vector<Stru_IPAddress>& bindaddrs);
    bool GetPortMap_Internal(const Stru_IPAddress& bindaddr,Stru_PortMapSvr& PortMapSvr);
    bool GetPortMap2_Internal(SOCKET hDataSocket,Stru_PortMapSvr& PortMapSvr);
    bool GetPortMapClient_Internal(SOCKET hDataSocket,Stru_PortMapClient& PortMapClient);
    bool SetPortMapClient_Internal(SOCKET hDataSocket,const Stru_PortMapClient& PortMapClient);

private:
	static HANDLE	        m_hThread;
    static unsigned long    m_idThread;
	static unsigned long    ThreadWorker(void *arg);


private:

	Ceasymutex	m_mutex;

    //bindaddr -> PortMapSvr
    map<Stru_IPAddress,Stru_PortMapSvr> m_mapPortMapSvr;

    //DataSocket/ClientSocket -> PortMapClient
	map<SOCKET,Stru_PortMapClient> m_MapDataSocket2PortMapClient;
	map<SOCKET,Stru_PortMapClient> m_MapClientSocket2PortMapClient;

private:
    enum eTcpCBType
    {
        CBType_TcpSvr_RcvData,
        CBType_TcpSvr_Accept,
        CBType_TcpSvr_Disconn,
        CBType_TcpSvr_Ontime,
        CBType_TcpClient_RcvData,
        CBType_TcpClient_Disconn,
        CBType_TcpClient_StartConnect,
        CBType_TcpClient_ConnSuccess,
        CBType_TcpClient_ConnFailure,
        CBType_TcpClient_Ontime,
    };

    //������TcpServer�Ļص�����, ���ص���ϢPost�������߳���ȥ
    static void TcpSvr_RcvDataCB(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hDataSocket);
    static void	TcpSvr_AcceptCB(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hDataSocket);      
    static void	TcpSvr_DisconnCB(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hDataSocket);		

    //������TcpClient�Ļص�����, ���ص���ϢPost�������߳���ȥ
    static void	TcpClient_RcvDataCB(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
    static void	TcpClient_DisconnCB(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);		                
    static void	TcpClient_StartConnectCB(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);							
    static void	TcpClient_ConnSuccessCB(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
    static void	TcpClient_ConnFailureCB(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);




    //������TcpServer�Ļص�������, �ڹ����߳��ﴦ��ص�����Post��������Ϣ
    void TcpSvr_RcvData_Internal(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hDataSocket);
    void TcpSvr_Accept_Internal(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hDataSocket);      
    void TcpSvr_Disconn_Internal(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hDataSocket);		

    //������TcpClient�Ļص�������, �ڹ����߳��ﴦ��ص�����Post��������Ϣ
    void TcpClient_RcvData_Internal(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
    void TcpClient_Disconn_Internal(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);		                
    void TcpClient_StartConnect_Internal(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);							
    void TcpClient_ConnSuccess_Internal(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
    void TcpClient_ConnFailure_Internal(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);

private:
															//����������������������ʵ�����ǣ�Ŀ���ǽ�ֹ��������͸�ֵ
	CTcpPortMap(const CTcpPortMap&);									
	CTcpPortMap& operator= (const CTcpPortMap&);				

};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_TCPSERVERWORKER_H__3C8964B8_6067_4345_9088_DDD8A9AC212E__INCLUDED_)
