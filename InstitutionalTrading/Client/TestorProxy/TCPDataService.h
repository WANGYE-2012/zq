#pragma once

#include "TcpClientWorker.h"
#include "CommonPkg.h"
#include <set>
#include <queue>


#define RECV_DATA_MSG WM_USER+500

struct RecvData 
{
	Stru_UniPkgHead head;	//��ͷ
	void* pData;			//���յ�����������
};

class CMemoryCache;

class CTCPDataService
{
public:
	enum 
	{	
		enmNetNothing, 
		enmNetDisconnect, 
		enmNetConnectWaiting, 
		enmNetConnectSuccess, 
		enmNetConnectFail, 
		enmNetDisconnectWaiting 
	};

public:
	static CTCPDataService* GetInstance();
	static void DestroyInstance();

	static void	CallbackReceiveData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);	//��tcp�õ����ݺ󣬵��ô˻ص��������д���
	static void CallbackStartConnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);							//��ʼ����connect()�󣬵��ô˺���֪ͨӦ�ò�
	static void	CallbackDisconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);							//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
	static void	CallbackConnectSuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);						//���ӳɹ��󣬵��ô˻ص��������д���
	static void	CallbackConnectFail(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);							//����ʧ�ܺ󣬵��ô˻ص��������д���

	static void CALLBACK OnTimer_HeartBeat(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

	static DWORD ThreadFunc(void* pData);

public:
	int GetNetStatus() { return m_nNetStatus;};
	void SetNetStatus(int nNetStatus) { m_nNetStatus = nNetStatus;};
	bool IsConnected();

	bool Connect( std::string strIP, int nPort );
	bool SendData(UINT nCMDID, void* pBuff, int nLen, UINT seq = 0, bool bNeedLogon = true);
	

	void RegisteHWnd( HWND hWnd);
	void UnRegisteHWnd( HWND hWnd);

	void SetLogonStatus(bool bLogon);
	bool IsLogon();
	std::string GetLastError();

private:
	CTCPDataService(void);
	virtual ~CTCPDataService(void);

	static bool VerifyRequest(Stru_UniPkgHead& head);

private:
	static CTCPDataService* m_pInstance;
	CTcpClientWorker*	m_pTcpClient;
	CMemoryCache*		m_pStreamCache;	
	int					m_nNetStatus;

	Stru_ServerAddr		m_addr;
	std::set<HWND>		m_hWndSet;
	CRITICAL_SECTION	m_csHWnd;
	CRITICAL_SECTION	m_csMsgQueue;

	std::queue<RecvData> m_msgQueue;
	HANDLE				m_hWorkerThread;
	bool				m_bWorkerThreadExit;

	bool				m_bLogon;
	std::string			m_strLastError;
};
