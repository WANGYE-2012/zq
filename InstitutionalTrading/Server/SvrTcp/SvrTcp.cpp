// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include "SvrTcpBusiness.h"




//ģ���ʼ����ûʲôҪ����
SVRTCP_API void InitFunc(void)
{
}

//ģ��������ͷ���Դ
SVRTCP_API void ReleaseFunc(void)
{
	CSvrTcpBusiness::getObj().ClearConnections();
}


//*******************************************************************************************************
//	������TcpSvr�ص�����
//*******************************************************************************************************

//accept�󣬵��ô˻ص��������д���
SVRTCP_API void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CSvrTcpBusiness::getObj().InsertConnection(SvrAddr,RemoteAddr,hSocket,true);
}


//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
SVRTCP_API void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CSvrTcpBusiness::getObj().EraseConnection(hSocket);
}


//��tcp�õ����ݺ󣬵��ô˻ص��������д���
SVRTCP_API void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CSvrTcpBusiness::getObj().RecvData(pData,DataLen,SvrAddr,RemoteAddr,hSocket);
}


//tcp�Ķ�ʱ����
SVRTCP_API void TcpSvrOntimeFunc(void)
{
	CSvrTcpBusiness::getObj().DisconnectTimeoutConnection();
}

//*******************************************************************************************************
//	������TcpClient�ص�����
//*******************************************************************************************************

//��tcp�õ����ݺ󣬵��ô˻ص��������д���
SVRTCP_API void	TcpClientRecvFunc(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	CSvrTcpBusiness::getObj().RecvData(pData,DataLen,LocalAddr,SvrAddr,hSocket);
}
//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
SVRTCP_API void	TcpClientDisconnFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	CSvrTcpBusiness::getObj().EraseConnection(hSocket);
}
//���ӳɹ��󣬵��ô˻ص��������д���
SVRTCP_API void	TcpClientConnSuccFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	CSvrTcpBusiness::getObj().InsertConnection(LocalAddr,SvrAddr,hSocket,false);
}
