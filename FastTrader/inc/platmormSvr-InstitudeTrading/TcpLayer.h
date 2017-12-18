#ifndef _TCPLAYER_H_
#define _TCPLAYER_H_

#include "easymutex.h"
#include "SocketBase.h"
#include "InstitudeApi.h"

#include <map>
#include <string>
using namespace std;

//TcpClient �շ��������Ĵ�С
#define SocketBufSize (10*1024*1024)

//Tcp���պ�ȴ���������ݻ�������С
#define TcpRecvBufSize (100*1024*1024)

//Tcp�ȴ����͵����ݻ�������С
#define TcpSendBufSize (10*1024*1024)


#define Debug_TcpLayer 1

class CTcpLayer
{
public:
	enum { scktConnect = 1, scktDisConnect, scktDataRecv };

public:
	// ������ʼ�յ����ݲ��ַ����̺߳���
	static DWORD WINAPI Proc_RecvData(LPVOID lpParameter);

	static void SetSvrAddr(string& SvrIP, WORD SvrPort) {
		m_strSvrIP = SvrIP;
		m_wSvrPort = SvrPort;
	};
	static bool GetSvrAddr(string& SvrIP, WORD& SvrPort) {
		SvrIP = m_strSvrIP;
		SvrPort = m_wSvrPort;
	};

	static void Init();
	static void Release();
	static void Stop();

	//��������, ��sendData��һ����װ
	static bool SendData(UINT nCMDID, void* pData, int nLen, UINT seq, UINT userData1=0, UINT userData2=0, UINT userData3=0, UINT userData4=0);

	//��������
	static bool Senddata(const char* pData,int DataLen);

	//ע��Api
	static void RegisterApi(CInstitudeApi* pApi);
	//ע��Api
	static void UnRegisterApi(CInstitudeApi* pApi);
	//ע������Api
	static void UnRegisterApiAll();


	//�����߳��е��ã����ڽ������ַ��������ݰ���
	//���յ����ݺ󣬻������̷߳���WM_COMMAND������߳��ٵ��ô˺������н����ͷַ���
	//�����ڴ����߳��д������ݣ���Ƚϰ�ȫ
	static void ParseRecvPackage(int nMode);

private:
	//��tcp�õ����ݺ󣬵��ô˻ص��������д���
	static void	RecvDataCBFunc(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);

	//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
	static void	DisconnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);

	//��ʼ����connect()�󣬵��ô˺���֪ͨӦ�ò�
	static void	StartConnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);

	//���ӳɹ��󣬵��ô˻ص��������д���
	static void	ConnSuccCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);

	//����ʧ�ܺ󣬵��ô˻ص��������д���
	static void	ConnFailCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);

	//���ڵ��õĻص�������һ��ÿ������һ�Σ��������ڼ��������ʱ��
	static void OnTimeCBFunc(void);

	//����tcpclient
	static void createtcpclient(void);

	//��������
	static void senddata_internal(void);

private:
	static Ceasymutex m_mutex;
	static void* m_pTcpClient;
	static bool m_bStop;

	static char* m_pRcvBuf;
	static int m_RcvBufLen;
	static char* m_pSndBuf;
	static int m_SndBufLen;

	//�ϴη�������ʱ��
	static time_t m_HeartbeatTime;



	static set<CInstitudeApi*> m_setApi;

	static string		m_strSvrIP;
	static WORD			m_wSvrPort;

	static HANDLE		m_hMainThread;
	static DWORD		m_dwMainThreadID;
	static UINT			m_nRecvDataMSGID;
};

#endif
