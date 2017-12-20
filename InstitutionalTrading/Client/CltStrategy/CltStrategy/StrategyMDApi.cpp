
#include "stdafx.h"
#include "StrategyMDApi.h"

CStrategyMdApi* CStrategyMdApi::CreateStrategyMdApi()
{
	return new CStrategyMdApi();
}

CStrategyMdApi::CStrategyMdApi()
{
	m_pBuzSpi = NULL;
	m_pTcpConnect = NULL;
	m_wSvrPort = 0;
	m_mutex = ::CreateMutex(NULL, FALSE, "");
}

CStrategyMdApi::~CStrategyMdApi()
{
	CloseHandle(m_mutex);
}

///ɾ���ӿڶ�����
///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
void CStrategyMdApi::Release()
{
    //LOG_INFO("CInstitudeTraderApi::Release() Start");
	if(m_pTcpConnect != NULL) {
		m_pTcpConnect->Stop();
		m_pTcpConnect->Release();
		delete m_pTcpConnect;
		m_pTcpConnect = NULL;
	}
    //LOG_INFO("CInstitudeTraderApi::Release() End");
	delete this;
}
	
///��ʼ��
///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
void CStrategyMdApi::Init()
{
	m_pTcpConnect = CTcpDataConnect::CreateConnect(m_strSvrIP, m_wSvrPort, this);
}

///ע��ǰ�û������ַ
///@param pszFrontAddress��ǰ�û������ַ��
///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
void CStrategyMdApi::RegisterFront(char* pszFrontAddress)
{
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
}
	
///�������顣
///@param ppInstrumentID ��ԼID  
///@param nCount Ҫ����/�˶�����ĺ�Լ����
///@remark 
int CStrategyMdApi::SubscribeMarketData(char* ppInstrumentID[], int nCount)
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
int CStrategyMdApi::UnSubscribeMarketData(char* ppInstrumentID[], int nCount)
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
int CStrategyMdApi::ReqUserLogin(CThostFtdcReqUserLoginField* pReqUserLoginField, int nRequestID)
{
	CThostFtdcRspUserLoginField m_tRspUserLogin = {0};
	CThostFtdcRspInfoField m_tpRspInfo = {0};
	LockMe();
	//if(m_pSpi!=NULL)
	//	m_pSpi->OnRspUserLogin(&m_tRspUserLogin, &m_tpRspInfo, nRequestID, true);
	UnLockMe();
	return 0;
}

///�ǳ�����
int CStrategyMdApi::ReqUserLogout(CThostFtdcUserLogoutField* pUserLogout, int nRequestID)
{
	CThostFtdcUserLogoutField m_tRspUserLogout = {0};
	CThostFtdcRspInfoField m_tpRspInfo = {0};
	LockMe();
	//if(m_pSpi!=NULL)
	//	m_pSpi->OnRspUserLogout(&m_tRspUserLogout, &m_tpRspInfo, nRequestID, true);
	UnLockMe();
	return 0;
}

void CStrategyMdApi::OnFrontConnected()
{
	LockMe();
	UnLockMe();
}

void CStrategyMdApi::OnFrontDisconnected()
{
	LockMe();
	UnLockMe();
}

// �������ݣ������յ�tcp����ʱ����
BOOL CStrategyMdApi::OnRecvData(char* pData, int nDataLen, DWORD nCmdId, DWORD nModuleId, 
		DWORD nSeq, DWORD nSubSeq, DWORD nUserData1, DWORD nUserData2, DWORD nUserData3, 
		DWORD nUserData4, char* pSrcPackage)
{
	if(pData == NULL && nDataLen<=0)
		return FALSE;
#if 1
	int nRequestID = 0;
	CThostFtdcRspInfoField rspInfo = {0};
	switch(nCmdId) {
	case Cmd_SubscribeQuotEvent_Rsp:
	{
		if(nDataLen==sizeof(CThostFtdcSpecificInstrumentField)) {

			CThostFtdcSpecificInstrumentField field;
			ZeroMemory(&field, sizeof(field));

			memcpy(&field, pData, sizeof(field));

			LockMe();
			if(m_pBuzSpi != NULL) 
				m_pBuzSpi->OnSubscribeQuot(field.InstrumentID, 0);
			UnLockMe();
		}
	}
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
			m_mapMD[depthData.InstrumentID] = depthData;
			if(m_pBuzSpi != NULL) 
				m_pBuzSpi->OnDepthMarketData(depthData, 0);
			UnLockMe();
		}
	}
		break;
	};
#endif

	//std::map<UINT, std::set<HWND>>::iterator it;
	//std::set<HWND>::iterator itWnd;

	//LockMe();

	//it = m_mapSubs.find(nCmdId);
	//for(itWnd = it->second.begin(); itWnd != it->second.end(); itWnd++) {
	//	if(IsWindow(*itWnd)) {
	//		SendMessage(*itWnd, _GetDataRecvMsgID(), WndUserCmd_YourPkgArrival, (LPARAM)pData);
	//	}
	//}

	//UnLockMe();

	return 0;
}
