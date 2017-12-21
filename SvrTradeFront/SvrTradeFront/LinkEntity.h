
#pragma once


#include "openssl\ssl.h"
#include <queue>


// һ������ʵ�壬�������պͷ���SSL����
class CLinkEntity
{
public:
	typedef struct struct_BUFFDATA {
		void* pBuff;
		int   nLen;
	} BUFFDATA, *LPBUFFDATA;

public:
	static BOOL InitSSL();
	static BOOL ReleaseSSL();

	static DWORD WINAPI Proc_SSLRead(LPVOID pParameter);
	static DWORD WINAPI Proc_SSLWrite(LPVOID pParameter);

	static SSL_METHOD*	sm_SLLMeth;
	static SSL_CTX*		sm_SSLCtx;
	static SOCKET		sm_SocketListen;

public:
	CLinkEntity(void);
	virtual ~CLinkEntity(void);

	SSL* GetSSL() {
		return m_pSSL;
	};

	void ResetCloseThread(HANDLE hThread, UINT nCloseMsgID) {
		m_hCloseThread = hThread;
		m_nCloseMsgID = nCloseMsgID;
	};

	void Lock() {};
	void Unlock() {};

	// ����ߴ���FFTApi
	BOOL Init(int nSocket, BOOL bForSSL);
	void Release();

	BOOL SSLAccept();

	// ����SSL�����̻߳��һ����������ݰ�
	LPVOID PopOneSendPackage();
	
	int SendData(void* pBuff, int nLen);
	//��������, ��sendData��һ����װ
	int CLinkEntity::SendPkgData(unsigned int nCMDID, const void* pData, int nLen, 
							 unsigned int seq, unsigned int Subseq, unsigned int Userdata1=0, 
							 unsigned int Userdata2=0, unsigned int Userdata3=0, unsigned int Userdata4=0);

	// ����SSL���SSL�����߳̽���һ�����ݰ�
	int ParseOnePackage(void* pBuff, int nPackageLen);

	BOOL ReqCloseSSLLinkEntity();

protected:

	std::queue<BUFFDATA>	m_ququeSend;
	SSL*		m_pSSL;
	SOCKET		m_hSocket;
	BOOL		m_bForSSL;
	HANDLE		m_hRecvThread;
	HANDLE		m_hSendThread;
	HANDLE		m_hCloseThread;
	UINT		m_nCloseMsgID;
	LPVOID		m_pFFTApi;

};
