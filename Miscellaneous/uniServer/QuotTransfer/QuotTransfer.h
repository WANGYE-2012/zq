#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the QUOTTRANSFER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// QUOTTRANSFER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef QUOTTRANSFER_EXPORTS
#define QUOTTRANSFER_API __declspec(dllexport)
#else
#define QUOTTRANSFER_API __declspec(dllimport)
#endif

#include "MemoryCache.h"
#include "SocketBase.h"
#include <vector>
using namespace std;

QUOTTRANSFER_API void InitFunc(void);
QUOTTRANSFER_API void ReleaseFunc(void);
QUOTTRANSFER_API void UdpRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
QUOTTRANSFER_API void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
QUOTTRANSFER_API void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
QUOTTRANSFER_API void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
QUOTTRANSFER_API void TcpSvrOntimeFunc(void);


#pragma warning(disable : 4996)
#pragma warning(disable : 4786)


#include <set>
#include <map>
#include <string>
#include "Tools_Ansi.h"
#include "WinService.h"
#include "TcpServerWorker.h"
#include "UdpSimpleWorker.h"
#include "IniFile.h"
#include "StreamCache.h"
#include "WriteLog.h"
#include "FixedPackage.h"
#include "ThostFtdcUserApiStruct.h"


#define PACKAGEHEAD   0x7a8b0a6d
#define PACKAGETAIL   0xa38ca16a

#define CMD_RegQuotI	1			//ע��I���������ݣ��������к�Լ����
#define CMD_RegQuotII	2			//ע��II���������ݣ�����������Լ����
#define CMD_RegQuotOKI	10			//ע��ɹ��ر�
#define CMD_RegQuotOKII	20			//ע��ɹ��ر�
#define CMD_Heartbeat	3			//������
#define CMD_Quot1		4			//I����������
#define CMD_Quot2		5			//II����������

#define CMD_ReqIBContractsInfo 	31	//����IB��Լ��Ϣ
#define CMD_IBContractsInfo		32	//����IB��Լ��Ϣ

#define CMD_ReqCTPMainContractsInfo 	41	//����CTP������Լ��Ϣ
#define CMD_CTPMainContractsInfo		42	//����CTP������Լ��Ϣ

#define CMD_Quot3		 40			//II����������
#define CMD_RegQuotIII	 41			//ע��III���������ݣ����󺬿��̼۵����к�Լ����
#define CMD_RegQuotOKIII 42			//ע��ɹ��ر�




#pragma pack(push)
#pragma pack(4)
typedef struct
{
	DWORD	Head;					//��ͷ
	int		PackageLen;				//������
	DWORD	CRCValue;				//CRCУ��ֵ
	DWORD	Cmd;					//������
	char	InstrumentID[32];		//��Լ����
	double	LastPrice;				//���¼�
	int 	Volume;					//�ɽ���
	int		OpenInterest;			//�ֲ���
	double	PreSettlementPrice;		//������
	double  PreClosePrice;			//�����̼�
	DWORD	UpdateTime_utc;			//����ʱ��,utc��ʽ����λΪ��
	int		UpdateTimeMS;			//����ʱ�������
	DWORD	Tail;					//��β
}Stru_Quot;

typedef struct
{
	DWORD	Head;					//��ͷ
	int		PackageLen;				//������
	DWORD	CRCValue;				//CRCУ��ֵ
	DWORD	Cmd;					//������
	char	InstrumentID[32];		//��Լ����
	double	OpenPrice;				//���̼�
	double	LastPrice;				//���¼�
	int 	Volume;					//�ɽ���
	int		OpenInterest;			//�ֲ���
	double	PreSettlementPrice;		//������
	double  PreClosePrice;			//�����̼�
	DWORD	UpdateTime_utc;			//����ʱ��,utc��ʽ����λΪ��
	int		UpdateTimeMS;			//����ʱ�������
	DWORD	Tail;					//��β
}Stru_Quot2;

#pragma pack(pop)


//ÿ��tcp�������ӵĲ���
struct Stru_DataSocket
{
	Stru_DataSocket()
	{
		hScoket=INVALID_SOCKET;
		memset(UserName,0,sizeof(UserName));
		bLogin=false;
		ReqType=2;
		time(&HeartbeatTime);
		pStreamCache=NULL;
		RemoteAddr.IP=0;
		RemoteAddr.Port=0;
	}

	SOCKET	hScoket;
	char	UserName[32];			//��¼�û���
	bool	bLogin;					//�Ƿ��Ѿ���¼
	int		ReqType;				//�����������ͣ�1:I��(����),2:II��(����)
	time_t	HeartbeatTime;			//���һ������ʱ��
	CMemoryCache *pStreamCache;		//��Cache
	Stru_IPAddress RemoteAddr;
};




extern std::map<std::string,Stru_Quot2> g_Quots;
extern std::set<std::string> g_MainInstruments;				//������Լ��
extern std::map<std::string,std::string> g_ValidUsersI;		//�Ϸ���I���û�
extern std::map<std::string,std::string> g_ValidUsersII;	//�Ϸ���II���û�
extern std::map<SOCKET,Stru_DataSocket> g_TcpUsers;			//�Ѿ�����tcp���ӵ��û�
extern CWriteLog *g_pWriteLog;
extern char	    *g_pSem;										//��д������
extern CFixedPackage *g_pQuotCache;
extern char    *g_IBContractsInfo;							//IB��Լ��Ϣ����
extern int		g_IBContractsLen;									//IB��Լ��Ϣ���ݳ��ȣ��ֽ���
extern std::map<std::string,std::string> g_IBContractsInfoReqUser;	//�Ϸ�������IB��Լ��Ϣ���û�
extern int g_HeartbeatTimeout;

extern char    *g_CTPMainContractsInfo;					    //CTP������Լ��Ϣ����
extern int		g_CTPMainContractsLen;						//CTP������Լ��Ϣ���ݳ��ȣ��ֽ���
extern std::map<std::string,std::string> g_CTPMainContractsInfoReqUser;	//�Ϸ�������CTP������Լ��Ϣ���û�
