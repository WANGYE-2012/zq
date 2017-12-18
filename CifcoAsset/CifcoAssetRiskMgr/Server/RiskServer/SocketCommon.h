#pragma once

#include "Tools/CommonPkg.h"
#include "Tools/MemoryCache.h"
#include "Tools/SocketBase.h"

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

struct RequestData
{
	SOCKET hSocket;			//������������Ӿ��
	Stru_UniPkgHead head;	//������
	void* pData;			//��������

	RequestData()
	{
		hSocket = INVALID_SOCKET;
		ZeroMemory(&head, sizeof(Stru_UniPkgHead));
		pData = NULL;
	}

	RequestData(SOCKET socket, Stru_UniPkgHead pkgHead, BYTE* p)
	{
		hSocket = socket;
		head = pkgHead;
		pData = p;
	}
};