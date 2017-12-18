// TcpServerWorker.h: interface for the CTcpServerWorker class.
//  1. ����ʱ��������m_hListenSocket����ִ��bind��listen
//  2. ���߳��д��������շ�
//  3. ���bind�Ķ˿�Ϊ0��ϵͳ�Զ�ѡ��һ�����ö˿ڡ�������GetBindAddr()��ȡ��ʵ�󶨵ĵ�ַ
//  4. ������m_hListenSocket�쳣��Ч���Զ����´���
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPSERVERWORKER_H__3C8964B8_6067_4345_9088_DDD8A9AC212E__INCLUDED_)
#define AFX_TCPSERVERWORKER_H__3C8964B8_6067_4345_9088_DDD8A9AC212E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <map>
#include <vector>
#include <stdexcept>
using std::map;
using std::vector;
#include "socketbase.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 


//��tcp�õ����ݺ󣬵��ô˻ص��������д���
typedef	void	(*TCPSVRRCVDATAHANDLER)(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
//accept�󣬵��ô˻ص��������д���
typedef	void	(*TCPSVRACCEPTHANDLER)(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);      
//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
typedef	void	(*TCPSVRDISCONNHANDLER)(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);		
//���ڵ��õĻص�������һ��ÿ������һ�Σ��������ڼ��������ʱ��
typedef void    (*TCPSVRONTIME)(void);


class CTcpServerWorker  : public CSocketBase
{
public:
	CTcpServerWorker(Stru_IPAddress BindAddr,int iMaxListen=16,
					 TCPSVRRCVDATAHANDLER pRecvFunc=NULL,
					 TCPSVRACCEPTHANDLER pAcceptFunc=NULL,
					 TCPSVRDISCONNHANDLER pDisconnFunc=NULL,
					 TCPSVRONTIME pOntimeFunc=NULL,
					 int SOSNDBUFSize=655360,int SORCVBUFSize=655360,
                     bool m_bReuseAddr=false);

    virtual ~CTcpServerWorker();


	/**************************************************************************************************//**
	*   @brief					GetServerAddr  ��ȡ��������ַ
	*   @param	N/A
	*	@return					Tcp�󶨵ĵ�ַ
	*******************************************************************************************************/
	Stru_IPAddress GetServerAddr(void);

	/**************************************************************************************************//**
	*   @brief					SendData	��������
	*   @param	[in] pData		Ҫ���͵�����
	*			[in] DataLen	Ҫ���͵����ݳ���
	*			[in] RemoteAddr	���͵�Ŀ�ĵ�ַ����IP/Port��Ϊ0����������BindAddr������Socket����
	*	@return					�ѷ��͵ĳ���
	*******************************************************************************************************/
	int SendData(char* pData,int DataLen,Stru_IPAddress RemoteAddr=Stru_IPAddress(0,0));

	/**************************************************************************************************//**
	*   @brief					SendData	��������
	*   @param	[in] pData		Ҫ���͵�����
	*			[in] DataLen	Ҫ���͵����ݳ���
	*			[in] hDataSocket���ڷ��͵�����socket���
	*	@return					�ѷ��͵ĳ���
	*******************************************************************************************************/
	int SendData(char* pData,int DataLen,SOCKET hDataSocket);

	void OffLineOneDataSocket(SOCKET hDataSocket);
	void OffLineOneDataSocket(Stru_IPAddress RemoteAddr);

    bool IsDataSocket(SOCKET hDataSocket);
    void GetAllDataSocket(vector<SOCKET>& AllDataSocket);
    Stru_IPAddress GetBindAddr(void);


private:
	HANDLE	m_hSocketThread;
	volatile bool	m_bSocketThreadNeedExit;

	char	*m_pTmpRecvBuf;									//���ڶ�ȡtcp���ݵĻ�����
	int		m_TmpRecvBufSize;


	SOCKET			m_hListenSocket;
	Stru_IPAddress	m_BindAddr;
	unsigned int	m_BindIP_Real;							//�����󶨵�IP�����BindIPΪ0�����ֵ���ڱ��ص�һ��IP
    unsigned short  m_BindPort_Real;                        //�����󶨵�Port�����BindPortΪ0��bindʱϵͳ���Զ�ָ��һ��port
    bool            m_bReuseAddr;                           //�Ƿ��ƶ�bind��socketSO_REUSEADDR
	int				m_MaxListen;
	time_t			m_CreateListenSocketTime;
	time_t			m_ShutdownListenSocketTime;


	TCPSVRRCVDATAHANDLER	m_RecvFunc;						//����socket���յ����ݺ�Ļص�����
	TCPSVRACCEPTHANDLER		m_AcceptFunc;					//Accept��Ļص�����
	TCPSVRDISCONNHANDLER	m_DisconnFunc;					//����socket�Ͽ�ǰ�Ļص�����
	TCPSVRONTIME			m_OntimeFunc;

    Stru_SocketRecvDataCallBackParam* m_pRcvDataCBParam;
    Stru_SocketCallBackParam* m_pAcceptCBParam;
    Stru_SocketCallBackParam* m_pDisconnCBParam;

	std::map<SOCKET,Stru_IPAddress> m_DataSocketMap;


	static DWORD SocketThreadWorker(void *arg);

	void OffLine_Internal(void);
	void OffLineOneDataSocket_Internal(SOCKET hDataSocket);
	void OffLineOneDataSocket_Internal(Stru_IPAddress RemoteAddr);

	/**************************************************************************************************//**
	*   @brief		CreateServerSocket_Internal  ���tcp server �Ƿ�Ҫ����socket
	*******************************************************************************************************/
	void CreateServerSocket_Internal();

	/**************************************************************************************************//**
	*   @brief		GetSocketSet_Internal  ��ȡҪselect��socketset
	*******************************************************************************************************/
	void GetSocketSet_Internal(void* pReadSocketSet,void* pWriteSocketSet,void* pExceptSocketSet);

	//**************************************************
	//   @brief		����socket�������¼�
	//**************************************************
	void CheckSocketExcept_Internal(void* pExceptSocketSet);

	//**************************************************
	//   @brief		�ͷ�һ���������ӵ�socket
	//**************************************************
	bool Check_TryReleaseOneDataSocket_Internal(SOCKET hDataSocket);

	//**************************************************
	//   @brief		�ͷ�һ������socket
	//**************************************************
	bool Check_TryReleaseOneListenSocket_Internal(SOCKET hListenSocket);

	//**************************************************
	//   @brief		����Accept
	//**************************************************
	void TryAccept_Internal(void* pReadSocketSet);

	//**************************************************
	//   @brief		����Read
	//**************************************************
	void TryRead_Internal(void* pReadSocketSet);


private:
															//����������������������ʵ�����ǣ�Ŀ���ǽ�ֹ��������͸�ֵ
	CTcpServerWorker(const CTcpServerWorker&);									
	CTcpServerWorker& operator= (const CTcpServerWorker&);				

};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_TCPSERVERWORKER_H__3C8964B8_6067_4345_9088_DDD8A9AC212E__INCLUDED_)
