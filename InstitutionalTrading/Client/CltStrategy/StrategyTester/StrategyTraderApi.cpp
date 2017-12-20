
#include "stdafx.h"
#include "StrategyTraderApi.h"
#include <stdlib.h>
#include "MD5.h"
#include <string>
#include <set>
#include "Module-Misc2\GLobalFunc.h"
#include "Module-Misc2\GlobalConfigMgr.h"


///����TraderApi
///@param pszFlowPath ����������Ϣ�ļ���Ŀ¼��Ĭ��Ϊ��ǰĿ¼
///@return ��������UserApi
//modify for udp marketdata
CStrategyTraderApi* CStrategyTraderApi::CreateStrategyTraderApi() 
{
	return new CStrategyTraderApi();
}

CStrategyTraderApi::CStrategyTraderApi()
{
	m_pBuzSpi = NULL;
	m_pTcpConnect = NULL;
	m_wSvrPort = 0;
	ZeroMemory(&m_UserLogin, sizeof(m_UserLogin));

	m_mutex = ::CreateMutex(NULL, FALSE, "");
}

CStrategyTraderApi::~CStrategyTraderApi()
{
	CloseHandle(m_mutex);
}

///ɾ���ӿڶ�����
///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
void CStrategyTraderApi::Release()
{
	//CTcpLayer::Stop();
	//CTcpLayer::UnRegisterApi(this);
	//CTcpLayer::Release();
    //LOG_INFO("CStrategyTraderApi::Release() Start");
	if(m_pTcpConnect != NULL) {
		m_pTcpConnect->Stop();
		m_pTcpConnect->Release();
		delete m_pTcpConnect;
		m_pTcpConnect = NULL;
	}
    //LOG_INFO("CStrategyTraderApi::Release() End");
	delete this;
}

///��ʼ��
///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
void CStrategyTraderApi::Init()
{
	m_pTcpConnect = CTcpDataConnect::CreateConnect(m_strSvrIP, m_wSvrPort, this);
}

///ע��ǰ�û������ַ
///@param pszFrontAddress��ǰ�û������ַ��
///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
void CStrategyTraderApi::RegisterFront(char *pszFrontAddress)
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

///�û���¼����
int CStrategyTraderApi::ReqUserLogin(const string& strUserID, const string& strPasswd, int nRequestID)
{
	m_strUserID = strUserID;
	m_strPasswd = strPasswd;

	sLoginReq sLogin;
	strncpy(sLogin.msUserName, strUserID.c_str(), sizeof(sLogin.msUserName)-1);
	strncpy(sLogin.msPassWord, strPasswd.c_str(), sizeof(sLogin.msPassWord)-1);	
	MD5 md5(sLogin.msPassWord);
	strcpy(sLogin.msPassWord, md5.toString().c_str());
	sLogin.meUserType = USER_TYPE_TRADER;
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(CMDID_Trader_Login_Req, (void*)&sLogin,sizeof(sLogin), 0)?0:999;
	else
		return 999;
}

///�ǳ�����
int CStrategyTraderApi::ReqUserLogout(int nRequestID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(CMDID_Logoff_Req, NULL, 0, 0)?0:999;
	else
		return 999;
}

///�����ѯ��Լ
int CStrategyTraderApi::ReqQryInstrument(PlatformStru_QryInstrument *pQryInstrument, int nRequestID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryInstrument_Req, (void*)pQryInstrument, 
			sizeof(*pQryInstrument), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

///�����ѯ��Լ��֤����
int CStrategyTraderApi::ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryMarginRate_Req, (void*)pQryInstrumentMarginRate, 
			sizeof(*pQryInstrumentMarginRate), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(Cmd_TradeDataManager_QryMarginRate_Req, (void*)pQryInstrumentMarginRate, 
	//		sizeof(*pQryInstrumentMarginRate), 0, m_UserLogin.mnUserID, nRequestID) ?0:999);
}

///�����ѯ��Լ��������
int CStrategyTraderApi::ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryCommissionRat_Req, (void*)pQryInstrumentCommissionRate, 
			sizeof(*pQryInstrumentCommissionRate), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(Cmd_TradeDataManager_QryCommissionRat_Req, (void*)pQryInstrumentCommissionRate, 
	//		sizeof(*pQryInstrumentCommissionRate), 0, m_UserLogin.mnUserID, nRequestID) ?0:999);
}

void CStrategyTraderApi::OnFrontConnected()
{
	LockMe();
	if(m_pBuzSpi != NULL)
		m_pBuzSpi->OnTraderConnected();
	UnLockMe();
}

void CStrategyTraderApi::OnFrontDisconnected()
{
	LockMe();
	if(m_pBuzSpi != NULL)
		m_pBuzSpi->OnTraderDisconnected();
	UnLockMe();
}

// �������ݣ������յ�tcp����ʱ����
BOOL CStrategyTraderApi::OnRecvData(char* pData, int nDataLen, DWORD nCmdId, DWORD nModuleId, 
		DWORD nSeq, DWORD nSubSeq, DWORD nUserData1, DWORD nUserData2, DWORD nUserData3, 
		DWORD nUserData4, char* pSrcPackage)
{
	if(pData == NULL && nDataLen<=0)
		return FALSE;

	int nRequestID = 0;
	CThostFtdcRspInfoField rspInfo = {0};
	switch(nCmdId) {
	case CMDID_Trader_Login_Rsp:
	{
		sTraderLoginRsp loginRsp;
		ZeroMemory(&loginRsp, sizeof(loginRsp));

		if(nUserData1 == CF_ERROR_SUCCESS) {
			if(nDataLen==sizeof(sTraderLoginRsp)) {

				memcpy(&loginRsp, pData, sizeof(loginRsp));
				m_UserLogin = loginRsp;

				LockMe();
				if(m_pBuzSpi!=NULL)
					m_pBuzSpi->OnTraderLogin(loginRsp, 0, std::string(""));
				UnLockMe();
				
				//SubsUserMsg(m_UserLogin.mnUserID);
			}
		}
		else {
			if(pData[nDataLen-1]==0) {
				LockMe();
				if(m_pBuzSpi!=NULL)
					m_pBuzSpi->OnTraderLogin(loginRsp, nUserData1, std::string(pData));
				UnLockMe();
			}
		}
	}
		break;
	case Cmd_TradeDataManager_QryInstrument_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_InstrumentInfo)==0) {
				std::vector<PlatformStru_InstrumentInfo> vecInstr;
				int nItemCount = nDataLen/sizeof(PlatformStru_InstrumentInfo);
				if(nItemCount > 0) {
					PlatformStru_InstrumentInfo instInfo;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&instInfo, pData+i*sizeof(instInfo), sizeof(instInfo));
						vecInstr.push_back(instInfo);
						LockMe();
						m_mapInstrument[instInfo.InstrumentID] = instInfo;
						UnLockMe();
					}
				}
				LockMe();
				if(m_pBuzSpi!=NULL)
					m_pBuzSpi->OnInstrument(vecInstr, 0);
				UnLockMe();
			}
		}
	}
		break;
	case Cmd_TradeDataManager_QryCommissionRat_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_InstrumentCommissionRate)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_InstrumentCommissionRate);
				if(nItemCount > 0) {
					PlatformStru_InstrumentCommissionRate csrInfo;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&csrInfo, pData+i*sizeof(csrInfo), sizeof(csrInfo));
						LockMe();
						if(m_pBuzSpi!=NULL)
							m_pBuzSpi->OnCommissionRate(csrInfo, 0);
						UnLockMe();
					}
				}
			}
		}
	}
		break;
	case Cmd_TradeDataManager_QryMarginRate_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_InstrumentMarginRate)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_InstrumentMarginRate);
				if(nItemCount > 0) {
					bool bIsLast = false;
					PlatformStru_InstrumentMarginRate mgrInfo;

					for(int i=0; i<nItemCount; i++) {
						bIsLast = (i==nItemCount-1);
						memcpy(&mgrInfo, pData+i*sizeof(mgrInfo), sizeof(mgrInfo));
						LockMe();
						if(m_pBuzSpi!=NULL)
							m_pBuzSpi->OnMarginRate(mgrInfo, 0);
						UnLockMe();
					}
				}
			}
		}
	}
		break;
	};

	return 0;
}
