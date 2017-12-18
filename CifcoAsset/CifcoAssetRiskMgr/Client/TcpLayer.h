#ifndef _TCPLAYER_H_
#define _TCPLAYER_H_

#include "easymutex.h"
#include "SocketBase.h"

#include <map>
#include <string>
using namespace std;

//TcpClient �շ��������Ĵ�С
#define SocketBufSize (10*1024*1024)

//Tcp���պ�ȴ���������ݻ�������С
#define TcpRecvBufSize (100*1024*1024)

//Tcp�ȴ����͵����ݻ�������С
#define TcpSendBufSize (10*1024*1024)


#define WM_COMMAND_Win              0x4111		//WM_COMMAND��ֵ����WinUser.h��ȡ���ģ���Ϊ����clr���й�����йܴ�����õķ��㣬û�а���windows.h


//֪ͨ���߳������ݵ����WM_COMMAND��Ϣ��wParam����
#define WndCmd_DataArrival 0x3bbb
//֪ͨģ�鴰������Ϣ���lParam����Ϣ����ָ�룬Stru_UniPkgHead + ����
#define WndCmd_YourPkgArrival 0x3bbc

#define Debug_TcpLayer 1

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CTcpLayer
{
public:
	static void Init();
	static void Release();
	static void Stop();

	//��������, ��sendData��һ����װ
	static bool SendData(UINT nCMDID, void* pData, int nLen, UINT seq);

	//��������
	static bool Senddata(const char* pData,int DataLen);


	//���������ھ��
	static void SetMainWndHandle(int hMainWnd);


	//ģ�鶩����Ϣ����ָ����ϢID��ģ�鴰�ڵľ��
	static void SubscribePkg(unsigned int CmdID,int hModuleWnd);
	//ģ���˶���Ϣ����ָ����ϢID��ģ�鴰�ڵľ��
	static void UnsubscribePkg(unsigned int CmdID,int hModuleWnd);
	//ģ���˶�ȫ����Ϣ����ָ����ϢID��ģ�鴰�ڵľ��
	static void UnsubscribeAllPkg(int hModuleWnd);


	//�����߳��е��ã����ڽ������ַ��������ݰ���
	//���յ����ݺ󣬻������̷߳���WM_COMMAND������߳��ٵ��ô˺������н����ͷַ���
	//�����ڴ����߳��д������ݣ���Ƚϰ�ȫ
	static void ParseRecvPackageInWindowThread(void);
	
	//�ж��Ƿ�Ͽ�
	static bool IsDisconnected();
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

	//�����ھ��
	static int m_hMainWnd;

	//������Ϣ��ģ�鴰�ھ��
	static multimap<unsigned int,int> m_Cmd2hWnd;

	//�ϴη�������ʱ��
	static time_t m_HeartbeatTime;
};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif


