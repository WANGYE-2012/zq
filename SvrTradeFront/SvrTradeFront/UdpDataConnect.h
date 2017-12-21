#ifndef _TCPDATACONNECT_H_
#define _TCPDATACONNECT_H_

#include "SocketBase.h"

class CUdpDataConnect
{
public:
	static CUdpDataConnect* CreateConnect(string& SvrIP, WORD SvrPort);

private:
	//���ڵ��õĻص�������һ��ÿ������һ�Σ��������ڼ��������ʱ��
	static void OnTimeCBFunc();

	static CUdpDataConnect* sm_pConnect;

public:
	CUdpDataConnect(string& SvrIP, WORD SvrPort);
	virtual ~CUdpDataConnect();

	bool GetSvrAddr(string& SvrIP, WORD& SvrPort) {
		SvrIP = m_strSvrIP;
		SvrPort = m_wSvrPort;
	};

	LPVOID Init();
	void Release();
	void Stop();
	void OnTimer();

	//��������
	int Senddata(const char* pData,int DataLen);

	//��������
	int senddata_internal(void);

private:
	void*				m_pUdpClient;
	SOCKET				m_hConnect;
	bool				m_bStop;

	char*				m_pSndBuf;
	int					m_SndBufLen;

	string				m_strSvrIP;
	WORD				m_wSvrPort;
};

#endif
