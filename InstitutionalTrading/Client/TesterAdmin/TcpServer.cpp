#include "stdafx.h"
#include "TcpServer.h"
#include "TestTcpBusiness.h"



//ģ���ʼ����ûʲôҪ����
void InitFunc(void)
{
	return;
}

//ģ��������ͷ���Դ
void ReleaseFunc(void)
{
	CTestTcpBusiness::getObj().ClearConnections();
}


//accept�󣬵��ô˻ص��������д���
void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CTestTcpBusiness::getObj().InsertConnection(SvrAddr,RemoteAddr,hSocket);
}


//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CTestTcpBusiness::getObj().EraseConnection(SvrAddr,RemoteAddr,hSocket);
}


//��tcp�õ����ݺ󣬵��ô˻ص��������д���
void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CTestTcpBusiness::getObj().RecvData(pData,DataLen,SvrAddr,RemoteAddr,hSocket);
}


//tcp�Ķ�ʱ����
void TcpSvrOntimeFunc(void)
{
	CTestTcpBusiness::getObj().DisconnectTimeoutConnection();
}
