// UdpSimpleWorker.h: interface for the CUdpSimpleWorker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UDPSIMPLEWORKER_H__026DD890_5F2D_4183_B856_C2EFB7217378__INCLUDED_)
#define AFX_UDPSIMPLEWORKER_H__026DD890_5F2D_4183_B856_C2EFB7217378__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)


#include <map>
#include <vector>
using namespace std;
#include "socketbase.h"

//��udp simple�õ����ݺ󣬵��ô˻ص��������д���
typedef	void	(*UDPSIMPLERCVDATAHANDLER)(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
//���ڵ��õĻص�������һ��ÿ������һ�Σ��������ڼ��������ʱ��
typedef void    (*UDPSIMPLEONTIME)(void);

#define MaxUdpRcvPkgCntPerLoop 128

class CUdpSimpleWorker  : public CSocketBase
{
public:
	CUdpSimpleWorker(Stru_IPAddress BindAddr,
					 UDPSIMPLERCVDATAHANDLER pRecvFunc=NULL,
					 UDPSIMPLEONTIME pOntimeFunc=NULL,
					 int SOSNDBUFSize=655360,int SORCVBUFSize=655360);
	virtual ~CUdpSimpleWorker();


	/**************************************************************************************************//**
	*   @brief					GetServerAddr  ��ȡ��������ַ���б�
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
	int SendData(char* pData,int DataLen,Stru_IPAddress RemoteAddr);


private:
	HANDLE	m_hSocketThread;
	bool	m_bSocketThreadNeedExit;
	char	*m_pTmpRecvBuf;									//���ڶ�ȡtcp���ݵĻ�����
	int		m_TmpRecvBufSize;


	SOCKET			m_hSocket;
	Stru_IPAddress	m_BindAddr;
	unsigned int	m_BindIP_Real;							//�����󶨵�IP�����BindIPΪ0�����ֵ���ڱ��ص�һ��IP
	time_t			m_CreateSocketTime;
	time_t			m_ShutdownSocketTime;

	UDPSIMPLERCVDATAHANDLER m_RecvFunc;
	UDPSIMPLEONTIME m_OntimeFunc;

    Stru_SocketRecvDataCallBackParam* m_pRcvDataCBParam;

	static DWORD SocketThreadWorker(void *arg);

	void OffLine_Internal(void);

	/**************************************************************************************************//**
	*   @brief		CreateSocket_Internal  ���udp simple�Ƿ�Ҫ����socket
	*******************************************************************************************************/
	void CreateSocket_Internal(time_t CurTime);

	/**************************************************************************************************//**
	*   @brief		GetSocketSet_Internal  ��ȡҪselect��socketset
	*******************************************************************************************************/
	void GetSocketSet_Internal(fd_set& ReadSocketSet,fd_set& WriteSocketSet,fd_set& ExceptSocketSet);

	//**************************************************
	//   @brief		����socket�������¼�
	//**************************************************
	void CheckSocketExcept_Internal(fd_set& ExceptSocketSet);

	//**************************************************
	//   @brief		����Read
	//**************************************************
	void TryRead_Internal(fd_set& ReadSocketSet);

};

#endif // !defined(AFX_UDPSIMPLEWORKER_H__026DD890_5F2D_4183_B856_C2EFB7217378__INCLUDED_)
