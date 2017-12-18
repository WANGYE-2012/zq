// TcpClientWorker.h: interface for the CTcpClientWorker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPCLIENTWORKER_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_)
#define AFX_TCPCLIENTWORKER_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)


#include "socketbase.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 



//��tcp�õ����ݺ󣬵��ô˻ص��������д���
typedef	void	(*TCPCLINTRCVDATAHANDLER)(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
typedef	void	(*TCPCLINTDISCONNHANDLER)(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
//��ʼ����connect()�󣬵��ô˺���֪ͨӦ�ò�
typedef void	(*TCPCLINTSTARTCONNECT)(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
//���ӳɹ��󣬵��ô˻ص��������д���
typedef	void	(*TCPCLINTCONNSUCCESSHANDLER)(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
//����ʧ�ܺ󣬵��ô˻ص��������д���
typedef	void	(*TCPCLINTCONNFAILUREHANDLER)(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
//���ڵ��õĻص�������һ��ÿ������һ�Σ��������ڼ��������ʱ��
typedef void    (*TCPCLINTONTIME)(int UserData);


//Tcp client ����״̬
enum TYPE_TcpClientConnStatus
{
	TcpClientConnStatus_Disconnected         =0,
	TcpClientConnStatus_Connecting           =1,
	TcpClientConnStatus_Connected            =2
};


class CTcpClientWorker  : public CSocketBase
{
public:
    //ReconnectInterval:�Զ�������ʱ��������λΪ�롣0��ʾ�Ͽ�������
	CTcpClientWorker(
        Stru_IPAddress LocalAddr,
        Stru_IPAddress SvrAddr,
		TCPCLINTRCVDATAHANDLER pRecvFunc=NULL,
		TCPCLINTDISCONNHANDLER pDisconnFunc=NULL,
		TCPCLINTSTARTCONNECT pStartFunc=NULL,
		TCPCLINTCONNSUCCESSHANDLER pConnSuccFunc=NULL,
		TCPCLINTCONNFAILUREHANDLER pConnFailFunc=NULL,
		TCPCLINTONTIME pOntimeFunc=NULL,
		int SOSNDBUFSize=655360,
        int SORCVBUFSize=655360,
        int ReconnectInterval=60,
        int UserData=0);
	virtual ~CTcpClientWorker();


	/**************************************************************************************************//**
	*   @brief				GetAddr  ��ȡ��ַ
	*   @param	LocalAddr	���ر��ص�ַ
	*			SvrAddr		���ط�������ַ
	*	@return				Tcp�󶨵ĵ�ַ
	*******************************************************************************************************/
	void GetAddr(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr);


	/**************************************************************************************************//**
	*   @brief					SendData	��������
	*   @param	[in] pData		Ҫ���͵�����
	*			[in] DataLen	Ҫ���͵����ݳ���
	*	@return					�ѷ��͵ĳ���
	*	@Note					���ܻ��ݴ�
	*******************************************************************************************************/
	int SendData(const void* pData,int DataLen);


	//ȡsocket���
	int getSocket(void) { return (int)m_hSocket; }
	//ȡ��ǰ����״̬
	TYPE_TcpClientConnStatus getConnStatus(void) { return m_ConnStatu; }
	//ȡ���ӽ���ʱ��
	time_t getConnEndTime(void) { return m_ConnEndTime; }

    //����Ϊ��Ҫ�������ӡ�������ִ��CSocketBase::CloseSocket(hSocket)�������ٴ�����
    void SetReconnect(void);
public:
	TCPCLINTRCVDATAHANDLER m_RecvFunc;
	TCPCLINTDISCONNHANDLER m_DisconnFunc;
	TCPCLINTSTARTCONNECT m_StartFunc;
	TCPCLINTCONNSUCCESSHANDLER m_ConnSuccFunc;
	TCPCLINTCONNFAILUREHANDLER m_ConnFailFunc;
	TCPCLINTONTIME m_OntimeFunc;


private:
	HANDLE	m_hSocketThread;
	volatile bool	m_bSocketThreadNeedExit;

	char	*m_pTmpRecvBuf;									//���ڶ�ȡtcp���ݵĻ�����
	int		m_TmpRecvBufSize;

	SOCKET			m_hSocket;
	Stru_IPAddress	m_LocalAddr;
	unsigned int	m_LocalIP_Real;							//�����󶨵�IP�����BindIPΪ0�����ֵ���ڱ��ص�һ��IP
	Stru_IPAddress	m_SvrAddr;
	time_t			m_CreateSocketTime;
	time_t			m_ShutdownSocketTime;
	time_t			m_ConnEndTime;
	TYPE_TcpClientConnStatus m_ConnStatu;
    int             m_ReconnectInterval;                    //�Զ�������ʱ��������λΪ��
	bool			m_bSuccessBefore;						//�������ӳɹ��������m_ReconnectIntervalΪ0����������

	char            *m_pSndBuf;                             //���������ȷ������buf�У�Ȼ���ٷ��͡�buf��СΪSOSNDBUFSize����
    int             m_SndBufSize;
	int             m_SndBufLen;

    int             m_UserData;



	static DWORD SocketThreadWorker(void *arg);

	void OffLine_Internal(Stru_SocketCallBackParam& DisconnCBParam);
	void OffLine_Internal2(void);
	/**************************************************************************************************//**
	*   @brief		CreateSocket_Internal  ���tcp client�Ƿ�Ҫ����socket
	*******************************************************************************************************/
	void CreateSocket_Internal(
        time_t CurTime,
        Stru_SocketCallBackParam& DisconnCBParam,
        Stru_SocketCallBackParam& StartCBParam,
        Stru_SocketCallBackParam& ConnSuccCBParam,
        Stru_SocketCallBackParam& ConnFailCBParam);

	/**************************************************************************************************//**
	*   @brief		GetSocketSet_Internal  ��ȡҪselect��socketset
	*******************************************************************************************************/
	void GetSocketSet_Internal(void* pReadSocketSet,void* pWriteSocketSet,void* pExceptSocketSet);

	//**************************************************
	//   @brief		����socket�������¼�
	//**************************************************
	void CheckSocketExcept_Internal(void* pExceptSocketSet,
		Stru_SocketCallBackParam& DisconnCBParam,
        Stru_SocketCallBackParam& ConnFailCBParam);

	//**************************************************
	//   @brief		����Read
	//**************************************************
	void TryRead_Internal(void* pReadSocketSet,
                            Stru_SocketRecvDataCallBackParam& RcvDataCBParam,
                            Stru_SocketCallBackParam& DisconnCBParam,
                            Stru_SocketCallBackParam& ConnFailCBParam);

	//**************************************************
	//   @brief		����Write
	//**************************************************
	void TryWrite_Internal(void* pWriteSocketSet,Stru_SocketCallBackParam& ConnSuccCBParam);


    //ʵ�ʷ������ݣ�����m_pSndBuf�е�����
    void senddata_internal(void);

private:
															//����������������������ʵ�����ǣ�Ŀ���ǽ�ֹ��������͸�ֵ
	CTcpClientWorker(const CTcpClientWorker&);									
	CTcpClientWorker& operator= (const CTcpClientWorker&);				

};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_TCPCLIENTWORKER_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_)
