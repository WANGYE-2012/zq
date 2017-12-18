#pragma once

#include "TcpClientWorker.h"
#define  nDataPkgArriveMSGID  (WM_USER +9991)
#define  nDataPkgArriveNextMSGID (WM_USER +9992)
class CTCPDataService
{
public:
	enum {	enmNetNothing, 
			enmNetDisconnect, 
			enmNetConnectWaiting, 
			enmNetConnectSuccess, 
			enmNetConnectFail, 
			enmNetDisconnectWaiting 
	};

public:
	static CTCPDataService* GetOnlyOneInstance();
	static void ReleaseOnlyOneInstance();

	static void	CallbackReceiveData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);	//��tcp�õ����ݺ󣬵��ô˻ص��������д���
	static void CallbackStartConnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);							//��ʼ����connect()�󣬵��ô˺���֪ͨӦ�ò�
	static void	CallbackDisconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);							//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
	static void	CallbackConnectSuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);						//���ӳɹ��󣬵��ô˻ص��������д���
	static void	CallbackConnectFail(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);							//����ʧ�ܺ󣬵��ô˻ص��������д���
	
	static DWORD WINAPI Proc_PushToForm(LPVOID lpvParam); 
	static VOID CALLBACK TimerProc_HeartBeat(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

public:

	int GetNetStatus() {
		return m_nNetStatus;
	};
	void SetNetStatus(int nNetStatus) {
		m_nNetStatus = nNetStatus;
	};

	bool RegisterForm(HWND hWnd);
	bool UnregisterForm(HWND hWnd);

	bool Connect();
	bool SendData(char* pBuff, int nLen);
	void OnTimer_HeartBeat(UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

protected:
	CTCPDataService(void);
	virtual ~CTCPDataService(void);

	void LoadConfig();
	bool Init();

protected:
	CTcpClientWorker*	m_poTcpClient;
	wxString			m_strBNCMIPAddr;
	WORD				m_wBNCMPort;

	int					m_nNetStatus;
	UINT GetDataPkgArriveMSGID();
	UINT GetDataPkgArriveNextMSGID();
};
