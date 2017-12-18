// TcpServerWorker.h: interface for the CTcpServerWorker class.
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
using namespace std;

#include "socketbase.h"


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
					 int SOSNDBUFSize=655360,int SORCVBUFSize=655360);
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



private:
	HANDLE	m_hSocketThread;
	volatile bool	m_bSocketThreadNeedExit;

	char	*m_pTmpRecvBuf;									//���ڶ�ȡtcp���ݵĻ�����
	int		m_TmpRecvBufSize;


	SOCKET			m_hListenSocket;
	Stru_IPAddress	m_BindAddr;
	unsigned int	m_BindIP_Real;							//�����󶨵�IP�����BindIPΪ0�����ֵ���ڱ��ص�һ��IP
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
	void CreateServerSocket_Internal(time_t CurTime);

	/**************************************************************************************************//**
	*   @brief		GetSocketSet_Internal  ��ȡҪselect��socketset
	*******************************************************************************************************/
	void GetSocketSet_Internal(fd_set& ReadSocketSet,fd_set& WriteSocketSet,fd_set& ExceptSocketSet);

	//**************************************************
	//   @brief		����socket�������¼�
	//**************************************************
	void CheckSocketExcept_Internal(fd_set& ExceptSocketSet);

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
	void TryAccept_Internal(fd_set& ReadSocketSet);

	//**************************************************
	//   @brief		����Read
	//**************************************************
	void TryRead_Internal(fd_set& ReadSocketSet);

};

#endif // !defined(AFX_TCPSERVERWORKER_H__3C8964B8_6067_4345_9088_DDD8A9AC212E__INCLUDED_)
