// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LICENSEVARIFY_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LICENSEVARIFY_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LICENSEVARIFY_EXPORTS
#define LICENSEVARIFY_API __declspec(dllexport)
#else
#define LICENSEVARIFY_API __declspec(dllimport)
#endif

#include "SocketBase.h"
#include "MemoryCache.h"

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

LICENSEVARIFY_API void InitFunc(void);
LICENSEVARIFY_API void ReleaseFunc(void);
LICENSEVARIFY_API void UdpRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
LICENSEVARIFY_API void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
LICENSEVARIFY_API void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
LICENSEVARIFY_API void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
LICENSEVARIFY_API void TcpSvrOntimeFunc(void);

// This class is exported from the LicenseVarify.dll
class LICENSEVARIFY_API CLicenseVarify {
public:
	CLicenseVarify(void);
	// TODO: add your methods here.
};

extern LICENSEVARIFY_API int nLicenseVarify;

LICENSEVARIFY_API int fnLicenseVarify(void);
