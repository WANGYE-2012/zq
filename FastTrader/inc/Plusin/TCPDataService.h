#pragma once

#include "TcpClientWorker.h"
#include "CommonPkg.h"
#include "MemoryCache.h"

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

	enum REQ_STEP
	{
		REQ_STEP_INIT,
		REQ_STEP_REQ_SERVER_ADDR,
		REQ_STEP_SERVER_ADDR_RECVED,
		REQ_STEP_CONNECT_TO_SERVER,
		REQ_STEP_REQ_LICENCE_LIST,
		REQ_STEP_LICENCE_LIST_RECVED
	};

public:
	static CTCPDataService* GetInstance();
	static void DestroyInstance();

	static void	CallbackReceiveData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);	//��tcp�õ����ݺ󣬵��ô˻ص��������д���
	static void CallbackStartConnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);							//��ʼ����connect()�󣬵��ô˺���֪ͨӦ�ò�
	static void	CallbackDisconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);							//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
	static void	CallbackConnectSuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);						//���ӳɹ��󣬵��ô˻ص��������д���
	static void	CallbackConnectFail(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);							//����ʧ�ܺ󣬵��ô˻ص��������д���
	
	static VOID CALLBACK OnTimer1(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	static VOID CALLBACK OnTimer2(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

public:
	int GetNetStatus() { return m_nNetStatus;};
	void SetNetStatus(int nNetStatus) { m_nNetStatus = nNetStatus;};

	bool Connect( std::string strIP, int nPort );
	bool SendData(UINT nCMDID, void* pBuff, int nLen, UINT seq = 0);
	void OnTimer_HeartBeat(UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	bool LicenceRecved();
	bool GetLicence(std::string strAppID, std::string& strLicence);

private:
	CTCPDataService(void);
	virtual ~CTCPDataService(void);

	int GetPackageHeadTailLen();
	int GetPackageHeadLen();
	static bool VerifyRequest(Stru_UniPkgHead& head);

private:
	static CTCPDataService* m_pInstance;
	CTcpClientWorker*	m_pTcpClient;
	CMemoryCache*		m_pStreamCache;	
	int					m_nNetStatus;

	Stru_ServerAddr		m_addr;
	REQ_STEP			m_step;
	std::map<std::string, std::string> m_licenceMap;
};
