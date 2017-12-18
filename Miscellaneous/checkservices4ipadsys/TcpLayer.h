#ifndef _TCPLAYER_H_
#define _TCPLAYER_H_

#include "SocketBase.h"

#include <map>
#include <string>
using namespace std;
//����ͷ��Ϣ״̬
union DataHeadStatus
{
	struct
	{
		unsigned char m_cStatus;
	} S;
	struct
	{
		unsigned char m_cError	:1;
		unsigned char m_cLog	:1;
		unsigned char bOther	:6;
	} s;
};
//����ͷ��Ϣ�ṹ��
typedef struct tagDataHead
{
	unsigned short m_usHeadID;
	unsigned short m_usVersion;
	unsigned long m_ulDataID;
    // ��������, 0:ɾ��m_ulDataID��Ӧ������
	unsigned short m_usDataType;
    // ���ݰ�����(�����ͷ)
	unsigned long m_ulDataLength;
    // ���ܷ���, 0:�޼���;
	unsigned short m_usEncryptType;
    // ���ܳ���.
	unsigned long m_ulEncryptLength;
    // ǩ������, 0:��ѹ��,����;
	unsigned short m_usSignatureType;
    // ǩ������.
	unsigned long m_ulSignatureLength;
	unsigned long m_ulSrcCode;
	unsigned long m_ulDestCode;
    // ��־λ.
	DataHeadStatus m_rStatus;
	char m_pcReserve[15];
}DataHead;

//����ͷ��Ϣ����
#define _DataHeadLength_		50


//TcpClient �շ��������Ĵ�С
#define SocketBufSize (10*1024*1024)

//Tcp���պ�ȴ���������ݻ�������С
#define TcpRecvBufSize (100*1024*1024)

//Tcp�ȴ����͵����ݻ�������С
#define TcpSendBufSize (10*1024*1024)




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

	//��������
	static bool Senddata(const char* pData,int DataLen);
	//�������ݣ�������
	static bool Senddata_withoutlock(const char* pData,int DataLen);



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

	//���ڽ������ַ��������ݰ���
	static void ParseRecvPackageInWindowThread(void);
	//�õ���¼���������
	static  int getloginReqData(char* loginReq);
	//�õ��������������
	static int getHQReqData(char* pData);

	////���ڴ��ȡDataHead
	static long g_Mem2Head(DataHead*, char*);
	//��DataHeadд���ڴ�
	static long g_Head2Mem(char*, DataHead*);
	////���ڴ��ȡ1������
	static long g_Mem2Int(void*, char*, int);
	////��1������д���ڴ�
	static long g_Int2Mem(char*, void*, int);
	//��ʼ��DataHead
	static void g_InitHead(DataHead*);
	////���ڴ��ȡһ���ַ���
	//long g_Mem2String(CString&, char*, int);
	////���ڴ��ȡһ���ַ���
	//CString g_Mem2String(char*, int);
	////���ַ���д���ڴ�
	static long g_String2Mem(char*, std::string, int);
	//�ҽ���ID
	BOOL IsProcessExist(const char *iModuleName,DWORD *pProcessID);

private:
	static Ceasymutex m_mutex;
	static void* m_pTcpClient;
	static bool m_bStop;

	static char* m_pRcvBuf;
	static int m_RcvBufLen;
	static char* m_pSndBuf;
	static int m_SndBufLen;
	static time_t m_ReceivePkgTime;
 
    //static DataHead m_rHead;// 
	//�ϴη�������ʱ��
	static time_t m_HeartbeatTime;
};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif


