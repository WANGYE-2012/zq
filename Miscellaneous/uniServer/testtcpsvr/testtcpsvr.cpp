// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include "testtcpsvr.h"
#include "SocketBase.h"
#include "WinService.h"
#include "CommonPkg.h"

#ifdef _DEBUG
#pragma comment(lib, "Tools4dllD.lib")											
#else 
#pragma comment(lib, "Tools4dll.lib")											
#endif



#define WRITELOGID 0

#include <map>
#include <string>
using namespace std;

#define RcvBufSize (10*1024*1024)
//ÿһ��socket����һ�����ջ�����
map<SOCKET,char*> g_mapRcvBuf;
map<SOCKET,int> g_mapRcvBufLen;



//����Pkg
void ProcessPkg(SOCKET hSocket,const char* pPkg);
TESTTCPSVR_API void InitFunc(void)
{
}

TESTTCPSVR_API void ReleaseFunc(void)
{
    map<SOCKET,char*>::iterator it;
    for(it=g_mapRcvBuf.begin();it!=g_mapRcvBuf.end();it++)
    {
        free(it->second);
    }
}


//accept�󣬵��ô˻ص��������д���
TESTTCPSVR_API void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
    char* pBuf=(char*)malloc(RcvBufSize);
    if(pBuf)
    {
        g_mapRcvBuf[hSocket]=pBuf;
        g_mapRcvBufLen[hSocket]=0;
    }
}


//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
TESTTCPSVR_API void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
    map<SOCKET,char*>::iterator it1=g_mapRcvBuf.find(hSocket);
    if(it1!=g_mapRcvBuf.end()) 
    {
        free(it1->second);
        g_mapRcvBuf.erase(it1);
    }
    map<SOCKET,int>::iterator it2=g_mapRcvBufLen.find(hSocket);
    if(it2!=g_mapRcvBufLen.end()) g_mapRcvBufLen.erase(it2);
}


//��tcp�õ����ݺ󣬵��ô˻ص��������д���
TESTTCPSVR_API void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
    map<SOCKET,char*>::iterator itBuf=g_mapRcvBuf.find(hSocket);
    map<SOCKET,int>::iterator itLen=g_mapRcvBufLen.find(hSocket);

    if(itBuf==g_mapRcvBuf.end()||itLen==g_mapRcvBufLen.end())
        return;

    if(itLen->second+DataLen<=RcvBufSize)
	{
        memcpy(((char*)itBuf->second)+itLen->second,pData,DataLen);
		itLen->second+=DataLen;
	}

    if(itLen->second>0)
    {
	    //���������ܳ��ֵ���Ч����
	    int off=0;
	    while(off<itLen->second&&Stru_UniPkgHead::IsInvalidPkgHead(itBuf->second+off,itLen->second-off)) off++;

	    //ѭ���������յ�������
	    int LengthUsedByPkg;
	    while(Stru_UniPkgHead::IsValidPkg(itBuf->second+off,itLen->second-off,&LengthUsedByPkg))
	    {
            ProcessPkg(hSocket,itBuf->second+off);

		    off+=LengthUsedByPkg;
	    }
	    if(off>0)
	    {
		    memmove(itBuf->second,itBuf->second+off,itLen->second-off);
		    itLen->second-=off;
	    }
    }

}


//tcp�Ķ�ʱ����
TESTTCPSVR_API void TcpSvrOntimeFunc(void)
{
}

//�����������ͷ����¼�
#define Cmd_RM_RiskEvent_Req					0x190001	//(��ʱ���룬��ʾ�ͻ����������������Ĺ���)
#define Cmd_RM_RiskEvent_Push					0x190002	
//�����¼����ݽṹ
#pragma pack(push)
#pragma pack(1)
struct RiskEvent
{
	int nRiskEventID;
	long lEventTime;			//��1970���������
	int nTradeAccountID;
	int nRiskIndModuleID;
	int nRiskIndicatorID;
	int nRiskLevelID;
	float fThresholdValue;
	int nMsgSendStatus;
	int nIsValid;					//0 �����¼���Ч 1 �����¼���Ч
};
#pragma pack(pop)

//����Pkg
void ProcessPkg(SOCKET hSocket,const char* pPkg)
{
    const Stru_UniPkgHead& Pkg=*(const Stru_UniPkgHead*)pPkg;
    const char* pData=pPkg+sizeof(Pkg);

    if(Pkg.cmdid==Cmd_RM_RiskEvent_Req)
    {
	    char buf[sizeof(Stru_UniPkgHead)+sizeof(RiskEvent)+4];
	    Stru_UniPkgHead& outPkgHead=*(Stru_UniPkgHead*)buf;
	    outPkgHead.Set(sizeof(RiskEvent),Cmd_RM_RiskEvent_Push,0,0);
        RiskEvent& outData=*(RiskEvent*)(buf+sizeof(outPkgHead));
        outData.nRiskEventID=(int)GetTickCount();
        outData.lEventTime=(long)time(NULL);
        outData.nTradeAccountID=123;
        outData.nRiskIndModuleID=3;
        outData.nRiskIndicatorID=4;
        outData.fThresholdValue=23.0;
        outData.nMsgSendStatus=0;
        outData.nIsValid=0;
	    *(unsigned int*)(buf+sizeof(outPkgHead)+sizeof(outData))=TailID_UniPkg;

         CSocketBase::SendDataTcp(hSocket,buf,sizeof(buf));
    }
    else if(Pkg.cmdid==CMDID_HEARTBEAT)
    {
	    char buf[sizeof(Stru_UniPkgHead)+4];
	    Stru_UniPkgHead& outPkgHead=*(Stru_UniPkgHead*)buf;
	    outPkgHead.Set(0,CMDID_HEARTBEATRSP,0,0);
	    *(unsigned int*)(buf+sizeof(outPkgHead))=TailID_UniPkg;

        CSocketBase::SendDataTcp(hSocket,buf,sizeof(buf));
        printf("%d: �յ�������\n",(int)time(NULL)%3600);

    }
}