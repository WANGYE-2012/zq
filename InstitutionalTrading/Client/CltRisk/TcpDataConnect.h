#ifndef _TCPDATACONNECT_H_
#define _TCPDATACONNECT_H_

#include "easymutex.h"
#include "SocketBase.h"

#include <map>
#include <string>
using namespace std;

//TcpClient �շ��������Ĵ�С
#define SocketBufSize_Quot (10*1024*1024)

//Tcp���պ�ȴ���������ݻ�������С
#define TcpRecvBufSize_Quot (100*1024*1024)

//Tcp�ȴ����͵����ݻ�������С
#define TcpSendBufSize_Quot (10*1024*1024)


#define WM_USER_Win_Quot              0x5111		//WM_USER+0x111��ֵ����WinUser.h��ȡ���ģ���Ϊ����clr���й�����йܴ�����õķ��㣬û�а���windows.h


//֪ͨ���̴߳��������ݵ����WM_USER_Win��Ϣ��wParamֵ
#define WndUserCmd_DataArrival_Quot 0x4bbb
//֪ͨ���̴߳�������״̬�����仯��WM_USER_Win��Ϣ��wParamֵ��lParam==0/1/2/3�ֱ��ʾ���ӶϿ�/���ӿ�ʼ/���ӳɹ�/����ʧ��
#define WndUserCmd_QuotConnStatus_Quot  0x4bbc

//֪ͨģ�鴰������Ϣ���lParam����Ϣ����ָ�룬Stru_UniPkgHead + ����
#define WndUserCmd_YourPkgArrival_Quot 0x4bcb

#define Debug_TcpDataConnect 1

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CTcpDataConnect
{
public:
    static void Init();
    static void Release();
    static void Stop();

	//�ж��Ƿ�������״̬
	static bool IsConnected();

	//��������, ��sendData��һ����װ
	static bool SendData(UINT nCMDID, void* pData, int nLen, UINT seq, unsigned int userdata1=0, unsigned int userdata2=0, unsigned int userdata3=0, unsigned int userdata4=0);

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
    //���յ����ݺ󣬻������̷߳���WM_USER_Win������߳��ٵ��ô˺������н����ͷַ���
    //�����ڴ����߳��д������ݣ���Ƚϰ�ȫ
	//****ע�⣺WndUserCmd_YourPkgArrival��Ϣ�Ľ��մ�����̲��ܱ���������MessageBox()�ĵ��õȡ�����������ᵼ��ϵͳ���������
	//****(��Ϊ��������Ϣ��������롣�ڽ��մ��ڵ���Ϣ��������������£���������ڵ���Ϣ���л�����Ϣ���������̻߳ᱣ�浱ǰ�����������������ݣ�Ȼ���ٴε�����Ϣ�������������롣(�ƺ���winform�Ļ���)�������ᵼ��m_pRcvBuf��ͻ������
    static void ParseRecvPackageInWindowThread(void);

private:
	//��tcp�õ����ݺ󣬵��ô˻ص��������д���
	static void	RecvDataCBFunc(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);

	//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
	static void	DisconnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);

	//��ʼ����connect()�󣬵��ô˺���֪ͨӦ�ò�
	static void	StartConnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);

	//���ӳɹ��󣬵��ô˻ص��������д���
	static void	ConnSuccCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);

	//����ʧ�ܺ󣬵��ô˻ص��������д���
	static void	ConnFailCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);

	//���ڵ��õĻص�������һ��ÿ������һ�Σ��������ڼ��������ʱ��
	static void OnTimeCBFunc(int UserData);

    //����tcpclient
    static void createtcpclient(void);

    //��������
    static void senddata_internal(void);

public:
    CTcpDataConnect(string& SvrIP, WORD SvrPort);
    ~CTcpDataConnect();
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

	//�Ƿ��Ѿ�����
	static bool m_bConnected;
	//�Ƿ�Ͽ������ӡ�ָ�������ӵ����ӶϿ��ı仯����������ʼ����δ����״̬
	static bool m_bDisConnected;
};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif


