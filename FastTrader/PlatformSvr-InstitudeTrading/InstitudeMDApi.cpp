
#include "stdafx.h"
#include "InstitudeMDApi.h"

CInstitudeMdApi* CInstitudeMdApi::CreateInstitudeMdApi(const char *pszFlowPath, const bool bIsUsingUdp)
{
	return new CInstitudeMdApi();
}

CInstitudeMdApi::CInstitudeMdApi()
{
	m_pSpi = NULL;
	m_pTcpConnect = NULL;
	m_wSvrPort = 0;
	m_eventExit = ::CreateEvent(NULL, TRUE, FALSE, "");
	m_mutex = ::CreateMutex(NULL, FALSE, "");
}

CInstitudeMdApi::~CInstitudeMdApi()
{
	CloseHandle(m_eventExit);
	CloseHandle(m_mutex);
}

///ɾ���ӿڶ�����
///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
void CInstitudeMdApi::Release()
{
    //LOG_INFO("CInstitudeTraderApi::Release() Start");
	if(m_pTcpConnect != NULL) {
		m_pTcpConnect->Stop();
		m_pTcpConnect->Release();
		delete m_pTcpConnect;
		m_pTcpConnect = NULL;
	}
    //LOG_INFO("CInstitudeTraderApi::Release() End");
	SetEvent(m_eventExit);
	delete this;
}
	
///��ʼ��
///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
void CInstitudeMdApi::Init()
{
	m_pTcpConnect = CTcpDataConnect::CreateConnect(m_strSvrIP, m_wSvrPort, this);
	ResetEvent(m_eventExit);
}
	
///�ȴ��ӿ��߳̽�������
///@return �߳��˳�����
int CInstitudeMdApi::Join()
{
	WaitForSingleObject(m_eventExit, INFINITE);
	return 0;
}

///ע��ǰ�û������ַ
///@param pszFrontAddress��ǰ�û������ַ��
///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
void CInstitudeMdApi::RegisterFront(char* pszFrontAddress)
{
	//if(strstr(pszFrontAddress, "tcp://")==NULL)
	if(strncmp(pszFrontAddress, "tcp://", strlen("tcp://"))!=0)
		return;

	char strAddr[128];
	char strPort[8];

	ZeroMemory(strAddr, sizeof(strAddr));
	ZeroMemory(strPort, sizeof(strPort));
	sscanf_s(pszFrontAddress+strlen("tcp://"), "%[^:]:%s", strAddr, sizeof(strAddr)-1, 
			strPort, sizeof(strPort)-1);
	m_strSvrIP = strAddr;
	m_wSvrPort = atoi(strPort);
	//CTcpLayer::SetSvrAddr(string(strAddr), atoi(strPort));
}
	
///ע��ص��ӿ�
///@param pSpi �����Իص��ӿ����ʵ��
void CInstitudeMdApi::RegisterSpi(CQuotSpi* pSpi)
{
	LockMe();
	m_pSpi = pSpi;
	UnLockMe();
}
	
///�������顣
///@param ppInstrumentID ��ԼID  
///@param nCount Ҫ����/�˶�����ĺ�Լ����
///@remark 
int CInstitudeMdApi::SubscribeMarketData(char* ppInstrumentID[], int nCount)
{
	if(m_pTcpConnect != NULL) {
		for(int i=0; i<nCount; i++) {
			m_pTcpConnect->SendData(Cmd_SubscribeQuotEvent_Req, (void*)ppInstrumentID[i], 
					strlen(ppInstrumentID[i])+1, 0, 0, 0);
		}
		return 0;
	}
	else
		return 999;
}

///�˶����顣
///@param ppInstrumentID ��ԼID  
///@param nCount Ҫ����/�˶�����ĺ�Լ����
///@remark 
int CInstitudeMdApi::UnSubscribeMarketData(char* ppInstrumentID[], int nCount)
{
	if(m_pTcpConnect != NULL) {
		for(int i=0; i<nCount; i++) {
			m_pTcpConnect->SendData(Cmd_UnSubscribeQuotEvent_Req, (void*)ppInstrumentID[i], 
					strlen(ppInstrumentID[i])+1, 0, 0, 0);
		}
		return 0;
	}
	else
		return 999;
}

///�û���¼����
int CInstitudeMdApi::ReqUserLogin(CThostFtdcReqUserLoginField* pReqUserLoginField, int nRequestID)
{
	CThostFtdcRspUserLoginField m_tRspUserLogin = {0};
	CThostFtdcRspInfoField m_tpRspInfo = {0};
	LockMe();
	if(m_pSpi!=NULL)
		m_pSpi->OnRspUserLogin(&m_tRspUserLogin, &m_tpRspInfo, nRequestID, true);
	UnLockMe();
	return 0;
}
	
///�ǳ�����
int CInstitudeMdApi::ReqUserLogout(CThostFtdcUserLogoutField* pUserLogout, int nRequestID)
{
	CThostFtdcUserLogoutField m_tRspUserLogout = {0};
	CThostFtdcRspInfoField m_tpRspInfo = {0};
	LockMe();
	if(m_pSpi!=NULL)
		m_pSpi->OnRspUserLogout(&m_tRspUserLogout, &m_tpRspInfo, nRequestID, true);
	UnLockMe();
	return 0;
}

void CInstitudeMdApi::OnFrontConnected()
{
	LockMe();
	if(m_pSpi!=NULL)
		m_pSpi->OnFrontConnected();
	UnLockMe();
}

void CInstitudeMdApi::OnFrontDisconnected()
{
	LockMe();
	if(m_pSpi!=NULL)
		m_pSpi->OnFrontDisconnected(0x2001);
	UnLockMe();
}

// �������ݣ������յ�tcp����ʱ����
BOOL CInstitudeMdApi::ParseData(char* pData, int nDataLen, DWORD nCmdId, DWORD nModuleId, 
		DWORD nSeq, DWORD nSubSeq, DWORD nUserData1, DWORD nUserData2, DWORD nUserData3, 
		DWORD nUserData4)
{
	if(pData == NULL && nDataLen<=0)
		return FALSE;
#if 1
	int nRequestID = 0;
	CThostFtdcRspInfoField rspInfo = {0};
	switch(nCmdId) {
	case Cmd_SubscribeQuotEvent_Rsp:
		break;
	case Cmd_UnSubscribeQuotEvent_Rsp:
		break;

	case Cmd_QuotEvent_Push:
	{
		if(nDataLen==sizeof(PlatformStru_DepthMarketData)) {

			PlatformStru_DepthMarketData depthData;
			ZeroMemory(&depthData, sizeof(depthData));

			memcpy(&depthData, pData, sizeof(depthData));

			//TRACE("%s, TradingDay := %s, UpdateTime := %s\n", 
			//		depthData.InstrumentID, 
			//		depthData.TradingDay, depthData.UpdateTime);

			LockMe();
			if(m_pSpi!=NULL)
				m_pSpi->OnRtnDepthMarketData(&depthData);
			UnLockMe();
		}
	}
		break;
	};
#endif
	return 0;
}
