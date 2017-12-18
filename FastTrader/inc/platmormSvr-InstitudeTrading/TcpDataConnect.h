#ifndef _TCPDATACONNECT_H_
#define _TCPDATACONNECT_H_

#include "easymutex.h"
#include "SocketBase.h"
#include "InstitudeApi.h"

#include <map>
#include <set>
#include <string>
using namespace std;

//TcpClient �շ��������Ĵ�С
#define SocketBufSize (10*1024*1024)

//Tcp���պ�ȴ���������ݻ�������С
#define TcpRecvBufSize (20*1024*1024)

//Tcp�ȴ����͵����ݻ�������С
#define TcpSendBufSize (10*1024*1024)


#define Debug_TcpLayer 1

class CTcpDataConnect
{
public:
	enum { scktConnect = 1, scktDisConnect, scktDataRecv, scktOnTime };

public:
	// ������ʼ�յ����ݲ��ַ����̺߳���
	static DWORD WINAPI Proc_RecvData(LPVOID lpParameter);

	static CTcpDataConnect* CreateConnect(string& SvrIP, WORD SvrPort, CInstitudeApi* pApi);

	static LPVOID FindTcpConnect(SOCKET hConn);

private:
	//��tcp�õ����ݺ󣬵��ô˻ص��������д���
	static void	RecvDataCBFunc(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket, int UserData);

	//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
	static void	DisconnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket, int UserData);

	//��ʼ����connect()�󣬵��ô˺���֪ͨӦ�ò�
	static void	StartConnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket, int UserData);

	//���ӳɹ��󣬵��ô˻ص��������д���
	static void	ConnSuccCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket, int UserData);

	//����ʧ�ܺ󣬵��ô˻ص��������д���
	static void	ConnFailCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket, int UserData);

	//���ڵ��õĻص�������һ��ÿ������һ�Σ��������ڼ��������ʱ��
	static void OnTimeCBFunc(int UserData);

public:
	CTcpDataConnect(string& SvrIP, WORD SvrPort, CInstitudeApi* pApi);
	~CTcpDataConnect();

	bool GetSvrAddr(string& SvrIP, WORD& SvrPort) {
		SvrIP = m_strSvrIP;
		SvrPort = m_wSvrPort;
	};
	CInstitudeApi* GetApi() {
		return m_pApi;
	};

	LPVOID Init();
	void Release();
	void Stop();
	void OnTimer();

	//��������, ��sendData��һ����װ
	bool SendData(UINT nCMDID, void* pData, int nLen, UINT seq, UINT userData1=0, UINT userData2=0, UINT userData3=0, UINT userData4=0);

	//��������
	bool Senddata(const char* pData,int DataLen);

	//�����߳��е��ã����ڽ������ַ��������ݰ���
	//���յ����ݺ󣬻������̷߳���WM_COMMAND������߳��ٵ��ô˺������н����ͷַ���
	//�����ڴ����߳��д������ݣ���Ƚϰ�ȫ
	void ParseRecvPackage(int nMode, SOCKET hConnect);

	//��������
	void senddata_internal(void);

private:
	static Ceasymutex	m_mutex;
	static UINT			m_nObjectCount;
	static BOOL			m_bMainThreadActive;
	static HANDLE		m_hMainThread;
	static DWORD		m_dwMainThreadID;
	static UINT			m_nRecvDataMSGID;
	static map<LPVOID, CTcpDataConnect*>	m_mapWorker;
	static map<SOCKET, CTcpDataConnect*>	m_mapConn;
	static map<SOCKET, CInstitudeApi*>		m_mapApi;

private:
	void*				m_pTcpClient;
	SOCKET				m_hConnect;
	bool				m_bStop;

	char*				m_pRcvBuf;
	int					m_RcvBufLen;
	char*				m_pSndBuf;
	int					m_SndBufLen;

	//�ϴη�������ʱ��
	DWORD				m_HeartbeatTime;

	string				m_strSvrIP;
	WORD				m_wSvrPort;
	CInstitudeApi*		m_pApi;
};

#endif
