
#include "stdafx.h"
#include "InstitudeTraderApi.h"
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
CInstitudeTraderApi* CInstitudeTraderApi::CreateInstitudeTraderApi(const char *pszFlowPath, const bool bIsUsingUdp) 
{
	return new CInstitudeTraderApi();
}

CInstitudeTraderApi::CInstitudeTraderApi()
{
	m_pSpi = NULL;
	m_pTcpConnect = NULL;
	m_wSvrPort = 0;
	ZeroMemory(&m_UserLogin, sizeof(m_UserLogin));
	ZeroMemory(&m_ReqUser, sizeof(m_ReqUser));
	ZeroMemory(&m_UserPasswordUpdate, sizeof(m_UserPasswordUpdate));

	m_eventExit = ::CreateEvent(NULL, TRUE, FALSE, "");
	m_mutex = ::CreateMutex(NULL, FALSE, "");
}

CInstitudeTraderApi::~CInstitudeTraderApi()
{
	CloseHandle(m_eventExit);
	CloseHandle(m_mutex);
}

///ɾ���ӿڶ�����
///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
void CInstitudeTraderApi::Release()
{
	//CTcpLayer::Stop();
	//CTcpLayer::UnRegisterApi(this);
	//CTcpLayer::Release();
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
void CInstitudeTraderApi::Init()
{
	//CTcpLayer::RegisterApi(this);
	//CTcpLayer::Init();
	m_pTcpConnect = CTcpDataConnect::CreateConnect(m_strSvrIP, m_wSvrPort, this);
	ResetEvent(m_eventExit);
}

///�ȴ��ӿ��߳̽�������
///@return �߳��˳�����
int CInstitudeTraderApi::Join()
{
	WaitForSingleObject(m_eventExit, INFINITE);
	return 0;
}

///ע��ǰ�û������ַ
///@param pszFrontAddress��ǰ�û������ַ��
///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
void CInstitudeTraderApi::RegisterFront(char *pszFrontAddress)
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
void CInstitudeTraderApi::RegisterSpi(CTraderSpi* pSpi)
{
	LockMe();
	m_pSpi = pSpi;
	UnLockMe();
}

///����˽������
///@param nResumeType ˽�����ش���ʽ  
///        THOST_TERT_RESTART:�ӱ������տ�ʼ�ش�
///        THOST_TERT_RESUME:���ϴ��յ�������
///        THOST_TERT_QUICK:ֻ���͵�¼��˽����������
///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ�˽���������ݡ�
void CInstitudeTraderApi::SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType)
{
}

///���Ĺ�������
///@param nResumeType �������ش���ʽ  
///        THOST_TERT_RESTART:�ӱ������տ�ʼ�ش�
///        THOST_TERT_RESUME:���ϴ��յ�������
///        THOST_TERT_QUICK:ֻ���͵�¼�󹫹���������
///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ������������ݡ�
void CInstitudeTraderApi::SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType)
{
}

///�ͻ�����֤����
int CInstitudeTraderApi::ReqAuthenticate(CThostFtdcReqAuthenticateField* pReqAuthenticateField, int nRequestID)
{
	return 0;
}

///�û���¼����
int CInstitudeTraderApi::ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
{
	if(pReqUserLoginField==NULL)
		return 999;
#if 0
	MD5 md5(pReqUserLoginField->Password);
	strcpy(pReqUserLoginField->Password, md5.toString().c_str());
	return CTcpLayer::SendData(CMDID_Trader_Login_Req, (void*)pReqUserLoginField,sizeof(*pReqUserLoginField), 0);
#else
	m_ReqUser = *pReqUserLoginField;
	//strcpy(m_UserLogin.BrokerID, pReqUserLoginField->BrokerID);
	//strcpy(m_UserLogin.UserID, pReqUserLoginField->UserID);

	sLoginReq sLogin;
	strcpy(sLogin.msUserName, pReqUserLoginField->UserID);
	strcpy(sLogin.msPassWord, pReqUserLoginField->Password);	
	MD5 md5(sLogin.msPassWord);
	strcpy(sLogin.msPassWord, md5.toString().c_str());
	sLogin.meUserType = USER_TYPE_TRADER;
	//sLogin.meUserType = USER_TYPE_MANAGE;
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(CMDID_Trader_Login_Req, (void*)&sLogin,sizeof(sLogin), 0)?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(CMDID_Trader_Login_Req, (void*)&sLogin,sizeof(sLogin), 0)?0:999);
#endif
}

///�ǳ�����
int CInstitudeTraderApi::ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID)
{
	if(pUserLogout==NULL)
		return 999;

	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(CMDID_Logoff_Req, NULL, 0, 0)?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(CMDID_Trader_Login_Req, (void*)&sLogin,sizeof(sLogin), 0)?0:999);

	return 0;
}

///�û������������
int CInstitudeTraderApi::ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate *pUserPasswordUpdate, int nRequestID)
{
	m_UserPasswordUpdate = *pUserPasswordUpdate;

	UserPassword userPasswd;
	memset(&userPasswd, 0, sizeof(userPasswd));
	userPasswd.nUserID = m_UserLogin.mnUserID;
	MD5 md51(pUserPasswordUpdate->OldPassword);
	strncpy(userPasswd.szOldPassword, md51.toString().c_str(), sizeof(userPasswd.szOldPassword));
	MD5 md52(pUserPasswordUpdate->NewPassword);
	strncpy(userPasswd.szNewPassword, md52.toString().c_str(), sizeof(userPasswd.szNewPassword));
	//strncpy(userPasswd.szNewPassword, pUserPasswordUpdate->Thost.NewPassword, sizeof(userPasswd.szNewPassword));
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_ModifyUserPassword_Req, (void*)&userPasswd, 
				sizeof(userPasswd), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(Cmd_RM_ModifyUserPassword_Req, (void*)&userPasswd, 
	//		sizeof(userPasswd), 0, m_UserLogin.mnUserID, nRequestID) ?0:999);
}

///�ʽ��˻������������
int CInstitudeTraderApi::ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate *pTradingAccountPasswordUpdate, int nRequestID)
{
	return 0;
}

///����¼������
int CInstitudeTraderApi::ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nRequestID)
{
	if(GlobalConfigManager::HasSaveOrderTime()) {
		std::string strFile;
		GlobalFunc::GetPath(strFile);
		strFile += "userdata\\";
		strFile += m_UserLogin.mTradingDay;
		strFile += "-input.dat";

		SYSTEMTIME sysTime = {0};
		ORDERTIME item = {0};

		::GetLocalTime(&sysTime);
		item.FrontID = m_UserLogin.mnFrontID;
		item.SessionID = m_UserLogin.mnSessionID;
		strncpy(item.OrderRef, pInputOrder->OrderRef, sizeof(item.OrderRef));
		strncpy(item.BrokerID, pInputOrder->BrokerID, sizeof(item.BrokerID));
		strncpy(item.InvestorID, pInputOrder->InvestorID, sizeof(item.InvestorID));
		item.DateTime = sysTime;
		WriteOrderTimeToFile(strFile, item);
	}

#if 1
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeExcute_InputOrder_Req, (void*)pInputOrder, 
			sizeof(*pInputOrder), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(Cmd_TradeExcute_InputOrder_Req, (void*)pInputOrder, 
	//		sizeof(*pInputOrder), 0, m_UserLogin.mnUserID, nRequestID) ?0:999);
#else
	CThostFtdcRspInfoField rspInfo;

	ZeroMemory(&rspInfo, sizeof(rspInfo));
	m_pSpi->OnRspQryOrder(NULL, &rspInfo, nRequestID, true);
	return 0;
#endif
}

///Ԥ��¼������
int CInstitudeTraderApi::ReqParkedOrderInsert(PlatformStru_ParkedOrder *pParkedOrder, int nRequestID)
{
	return 0;
}

///Ԥ�񳷵�¼������
int CInstitudeTraderApi::ReqParkedOrderAction(PlatformStru_ParkedOrderAction *pParkedOrderAction, int nRequestID)
{
	return 0;
}

///������������
int CInstitudeTraderApi::ReqOrderAction(PlatformStru_InputOrderAction *pInputOrderAction, int nRequestID)
{
#if 1
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeExcute_CancelOrder_Req, (void*)pInputOrderAction, 
			sizeof(*pInputOrderAction), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(Cmd_TradeExcute_CancelOrder_Req, (void*)pInputOrderAction, 
	//		sizeof(*pInputOrderAction), 0, m_UserLogin.mnUserID, nRequestID) ?0:999);
#else
	CThostFtdcRspInfoField rspInfo;

	ZeroMemory(&rspInfo, sizeof(rspInfo));
	m_pSpi->OnRspQryOrder(NULL, &rspInfo, nRequestID, true);
	return 0;
#endif
}

///��ѯ��󱨵���������
int CInstitudeTraderApi::ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume *pQueryMaxOrderVolume, int nRequestID)
{
	return 0;
}

///Ͷ���߽�����ȷ��
int CInstitudeTraderApi::ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID)
{
#if 1
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_SettlementConfirm_Req, (void*)pSettlementInfoConfirm, 
			sizeof(*pSettlementInfoConfirm), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(Cmd_TradeExcute_CancelOrder_Req, (void*)pInputOrderAction, 
	//		sizeof(*pInputOrderAction), 0, m_UserLogin.mnUserID, nRequestID) ?0:999);
#else
	//����ʵ��ģʽʵ�֣���Ҫ���͵�������
	CThostFtdcSettlementInfoConfirmField settleInfoConfirm;
	CThostFtdcRspInfoField rspInfo;

	ZeroMemory(&settleInfoConfirm, sizeof(settleInfoConfirm));
	ZeroMemory(&rspInfo, sizeof(rspInfo));

	///���͹�˾����
	strncpy(settleInfoConfirm.BrokerID, pSettlementInfoConfirm->BrokerID, sizeof(settleInfoConfirm.BrokerID)-1);
	///Ͷ���ߴ���
	strncpy(settleInfoConfirm.InvestorID, pSettlementInfoConfirm->InvestorID, sizeof(settleInfoConfirm.InvestorID)-1);
	strcpy(settleInfoConfirm.ConfirmDate, "00000000");
	strcpy(settleInfoConfirm.ConfirmTime, "00:00:00");
	m_pSpi->OnRspSettlementInfoConfirm(&settleInfoConfirm, &rspInfo, nRequestID, true);
	return 0;
#endif
}

///����ɾ��Ԥ��
int CInstitudeTraderApi::ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder *pRemoveParkedOrder, int nRequestID)
{
	return 0;
}

///����ɾ��Ԥ�񳷵�
int CInstitudeTraderApi::ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction *pRemoveParkedOrderAction, int nRequestID)
{
	return 0;
}

///�����ѯ����
int CInstitudeTraderApi::ReqQryOrder(PlatformStru_QryOrder *pQryOrder, int nRequestID)
{
#if 1
	strncpy(pQryOrder->Thost.BrokerID, m_ReqUser.BrokerID, sizeof(pQryOrder->Thost.BrokerID)-1);
	strncpy(pQryOrder->Thost.InvestorID, m_ReqUser.UserID, sizeof(pQryOrder->Thost.InvestorID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryOrder_Req, (void*)pQryOrder, 
			sizeof(*pQryOrder), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(Cmd_TradeDataManager_QryOrder_Req, (void*)pQryOrder, 
	//		sizeof(*pQryOrder), 0, m_UserLogin.mnUserID, nRequestID) ?0:999);
#else
	CThostFtdcRspInfoField rspInfo;

	ZeroMemory(&rspInfo, sizeof(rspInfo));
	m_pSpi->OnRspQryOrder(NULL, &rspInfo, nRequestID, true);
	return 0;
#endif
}

///�����ѯ�ɽ�
int CInstitudeTraderApi::ReqQryTrade(PlatformStru_QryTrade *pQryTrade, int nRequestID)
{
#if 1
	strncpy(pQryTrade->Thost.BrokerID, m_ReqUser.BrokerID, sizeof(pQryTrade->Thost.BrokerID)-1);
	strncpy(pQryTrade->Thost.InvestorID, m_ReqUser.UserID, sizeof(pQryTrade->Thost.InvestorID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryTrade_Req, (void*)pQryTrade, 
			sizeof(*pQryTrade), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(Cmd_TradeDataManager_QryTrade_Req, (void*)pQryTrade, 
	//		sizeof(*pQryTrade), 0, m_UserLogin.mnUserID, nRequestID) ?0:999);
#else
	CThostFtdcRspInfoField rspInfo;

	ZeroMemory(&rspInfo, sizeof(rspInfo));
	m_pSpi->OnRspQryTrade(NULL, &rspInfo, nRequestID, true);
	return 0;
#endif
}

///�����ѯͶ���ֲ߳�
int CInstitudeTraderApi::ReqQryInvestorPosition(PlatformStru_QryInvestorPosition *pQryInvestorPosition, int nRequestID)
{
#if 1
	strncpy(pQryInvestorPosition->BrokerID, m_ReqUser.BrokerID, sizeof(pQryInvestorPosition->BrokerID)-1);
	strncpy(pQryInvestorPosition->InvestorID, m_ReqUser.UserID, sizeof(pQryInvestorPosition->InvestorID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryPosition_Req, (void*)pQryInvestorPosition, 
			sizeof(*pQryInvestorPosition), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(Cmd_TradeDataManager_QryPosition_Req, (void*)pQryInvestorPosition, 
	//		sizeof(*pQryInvestorPosition), 0, m_UserLogin.mnUserID, nRequestID) ?0:999);
#else
	CThostFtdcRspInfoField rspInfo;

	ZeroMemory(&rspInfo, sizeof(rspInfo));
	m_pSpi->OnRspQryInvestorPosition(NULL, &rspInfo, nRequestID, true);
	return 0;
#endif
}

///�����ѯ�ʽ��˻�
int CInstitudeTraderApi::ReqQryTradingAccount(PlatformStru_QryTradingAccount *pQryTradingAccount, int nRequestID)
{
#if 1
	strncpy(pQryTradingAccount->BrokerID, m_ReqUser.BrokerID, sizeof(pQryTradingAccount->BrokerID)-1);
	strncpy(pQryTradingAccount->InvestorID, m_ReqUser.UserID, sizeof(pQryTradingAccount->InvestorID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryTraderAccount_Req, (void*)pQryTradingAccount, 
			sizeof(*pQryTradingAccount), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(Cmd_TradeDataManager_QryTraderAccount_Req, (void*)pQryTradingAccount, 
	//		sizeof(*pQryTradingAccount), 0, m_UserLogin.mnUserID, nRequestID) ?0:999);
#else
	PlatformStru_TradingAccountInfo tInfo;
	CThostFtdcRspInfoField rspInfo;

	ZeroMemory(&tInfo, sizeof(tInfo));
	ZeroMemory(&rspInfo, sizeof(rspInfo));
	strcpy(tInfo.BrokerID, m_UserLogin.BrokerID);
	strcpy(tInfo.AccountID, m_UserLogin.UserID);
	tInfo.Available = 100000.0;
	tInfo.PreMortgage = 100000.0;
	m_pSpi->OnRspQryTradingAccount(&tInfo, &rspInfo, nRequestID, true);
	return 0;
#endif
}

///�����ѯͶ����
int CInstitudeTraderApi::ReqQryInvestor(PlatformStru_QryInvestor *pQryInvestor, int nRequestID)
{
	return 0;
}

///�����ѯ���ױ���
int CInstitudeTraderApi::ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID)
{
	return 0;
}

///�����ѯ��Լ��֤����
int CInstitudeTraderApi::ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID)
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
int CInstitudeTraderApi::ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryCommissionRat_Req, (void*)pQryInstrumentCommissionRate, 
			sizeof(*pQryInstrumentCommissionRate), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(Cmd_TradeDataManager_QryCommissionRat_Req, (void*)pQryInstrumentCommissionRate, 
	//		sizeof(*pQryInstrumentCommissionRate), 0, m_UserLogin.mnUserID, nRequestID) ?0:999);
}

///�����ѯ������
int CInstitudeTraderApi::ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID)
{
	return 0;
}

///�����ѯ��Լ
int CInstitudeTraderApi::ReqQryInstrument(PlatformStru_QryInstrument *pQryInstrument, int nRequestID)
{
#if 1
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryInstrument_Req, (void*)pQryInstrument, 
			sizeof(*pQryInstrument), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(Cmd_TradeDataManager_QryInstrument_Req, (void*)pQryInstrument, 
	//		sizeof(*pQryInstrument), 0, m_UserLogin.mnUserID, nRequestID) ?0:999);
#else
	PlatformStru_InstrumentInfo tInfo;
	CThostFtdcRspInfoField rspInfo;

	ZeroMemory(&tInfo, sizeof(tInfo));
	ZeroMemory(&rspInfo, sizeof(rspInfo));
	strcpy(tInfo.InstrumentID, "IF1309");
	strcpy(tInfo.ExchangeID, "CFFEX");
	strcpy(tInfo.InstrumentName, "����300��ָ1309");
	strcpy(tInfo.ExchangeInstID, "IF1309");
	strcpy(tInfo.ProductID, "IF");
	tInfo.ProductClass='1';
	tInfo.VolumeMultiple=300;
	tInfo.PriceTick=0.2;
	tInfo.ShortMarginRatio = tInfo.LongMarginRatio = 0.15;
	m_pSpi->OnRspQryInstrument(&tInfo, &rspInfo, nRequestID, false);
	strcpy(tInfo.InstrumentID, "IF1312");
	strcpy(tInfo.ExchangeID, "CFFEX");
	strcpy(tInfo.InstrumentName, "����300��ָ1312");
	strcpy(tInfo.ExchangeInstID, "IF1312");
	m_pSpi->OnRspQryInstrument(&tInfo, &rspInfo, nRequestID, true);
	return 0;
#endif
}

///�����ѯ����
int CInstitudeTraderApi::ReqQryDepthMarketData(PlatformStru_QryDepthMarketData *pQryDepthMarketData, int nRequestID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryDepthMarket_Req, (void*)pQryDepthMarketData, 
			sizeof(*pQryDepthMarketData), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

///�����ѯͶ���߽�����
int CInstitudeTraderApi::ReqQrySettlementInfo(PlatformStru_QrySettlementInfo *pQrySettlementInfo, int nRequestID)
{
#if 1
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_Settlement_SettleBill_Req, (void*)pQrySettlementInfo, 
			sizeof(*pQrySettlementInfo), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(Cmd_TradeDataManager_QryInstrument_Req, (void*)pQryInstrument, 
	//		sizeof(*pQryInstrument), 0, m_UserLogin.mnUserID, nRequestID) ?0:999);
#else
	CThostFtdcSettlementInfoField settleInfo;
	CThostFtdcRspInfoField rspInfo;

	ZeroMemory(&settleInfo, sizeof(settleInfo));
	ZeroMemory(&rspInfo, sizeof(rspInfo));

	///���͹�˾����
	strncpy(settleInfo.BrokerID, pQrySettlementInfo->Thost.BrokerID, sizeof(settleInfo.BrokerID)-1);
	///Ͷ���ߴ���
	strncpy(settleInfo.InvestorID, pQrySettlementInfo->Thost.InvestorID, sizeof(settleInfo.InvestorID)-1);
	strncpy(settleInfo.TradingDay, pQrySettlementInfo->Thost.TradingDay, sizeof(settleInfo.TradingDay)-1);
	strcpy(settleInfo.Content, "û�н��㵥��");
	m_pSpi->OnRspQrySettlementInfo(&settleInfo, &rspInfo, nRequestID, true);
	return 0;
#endif
}

///�����ѯת������
int CInstitudeTraderApi::ReqQryTransferBank(PlatformStru_QryTransferBank *pQryTransferBank, int nRequestID)
{
	return 0;
}

///�����ѯͶ���ֲ߳���ϸ
int CInstitudeTraderApi::ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail *pQryInvestorPositionDetail, int nRequestID)
{
#if 1
	strncpy(pQryInvestorPositionDetail->BrokerID, m_ReqUser.BrokerID, sizeof(pQryInvestorPositionDetail->BrokerID)-1);
	strncpy(pQryInvestorPositionDetail->InvestorID, m_ReqUser.UserID, sizeof(pQryInvestorPositionDetail->InvestorID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryPositionDetail_Req, (void*)pQryInvestorPositionDetail, 
			sizeof(*pQryInvestorPositionDetail), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(Cmd_TradeDataManager_QryPositionDetail_Req, (void*)pQryInvestorPositionDetail, 
	//		sizeof(*pQryInvestorPositionDetail), 0, m_UserLogin.mnUserID, nRequestID) ?0:999);
#else
	CThostFtdcRspInfoField rspInfo;

	ZeroMemory(&rspInfo, sizeof(rspInfo));
	m_pSpi->OnRspQryInvestorPositionDetail(NULL, &rspInfo, nRequestID, true);
	return 0;
#endif
}

///�����ѯ�ͻ�֪ͨ
int CInstitudeTraderApi::ReqQryNotice(PlatformStru_QryNotice *pQryNotice, int nRequestID)
{
#if 1
	strncpy(pQryNotice->Thost.BrokerID, m_ReqUser.BrokerID, sizeof(pQryNotice->Thost.BrokerID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryNotice_Req, (void*)pQryNotice, 
			sizeof(*pQryNotice), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
#else
	CThostFtdcNoticeField tNotice;
	CThostFtdcRspInfoField rspInfo;

	ZeroMemory(&tNotice, sizeof(tNotice));
	ZeroMemory(&rspInfo, sizeof(rspInfo));
	strncpy(tNotice.BrokerID, pQryNotice->Thost.BrokerID, sizeof(tNotice.BrokerID)-1);
	strcpy(tNotice.Content, "��ӭʹ��������Ϣ���ʺ�ϵͳ��");
	strcpy(tNotice.SequenceLabel, "0");
	m_pSpi->OnRspQryNotice(&tNotice, &rspInfo, nRequestID, true);
	return 0;
#endif
}

///�����ѯ������Ϣȷ��
int CInstitudeTraderApi::ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm *pQrySettlementInfoConfirm, int nRequestID)
{
#if 1
	strncpy(pQrySettlementInfoConfirm->Thost.BrokerID, m_ReqUser.BrokerID, sizeof(pQrySettlementInfoConfirm->Thost.BrokerID)-1);
	strncpy(pQrySettlementInfoConfirm->Thost.InvestorID, m_ReqUser.UserID, sizeof(pQrySettlementInfoConfirm->Thost.InvestorID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QrySettlementConfirm_Req, (void*)pQrySettlementInfoConfirm, 
			sizeof(*pQrySettlementInfoConfirm), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(Cmd_TradeDataManager_QrySettlementConfirm_Req, (void*)pQrySettlementInfoConfirm, 
	//		sizeof(*pQrySettlementInfoConfirm), 0, m_UserLogin.mnUserID, nRequestID) ?0:999);
#else
	//����ʵ��ģʽʵ�֣���Ҫ���͵�������
	CThostFtdcSettlementInfoConfirmField settleInfoConfirm;
	CThostFtdcRspInfoField rspInfo;

	ZeroMemory(&settleInfoConfirm, sizeof(settleInfoConfirm));
	ZeroMemory(&rspInfo, sizeof(rspInfo));

	///���͹�˾����
	strncpy(settleInfoConfirm.BrokerID, pQrySettlementInfoConfirm->Thost.BrokerID, sizeof(settleInfoConfirm.BrokerID)-1);
	///Ͷ���ߴ���
	strncpy(settleInfoConfirm.InvestorID, pQrySettlementInfoConfirm->Thost.InvestorID, sizeof(settleInfoConfirm.InvestorID)-1);
	strcpy(settleInfoConfirm.ConfirmDate, "00000000");
	strcpy(settleInfoConfirm.ConfirmTime, "00:00:00");
	m_pSpi->OnRspQrySettlementInfoConfirm(&settleInfoConfirm, &rspInfo, nRequestID, true);
	return 0;
#endif
}

///�����ѯͶ���ֲ߳���ϸ
int CInstitudeTraderApi::ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail *pQryInvestorPositionCombineDetail, int nRequestID)
{
#if 1
	strncpy(pQryInvestorPositionCombineDetail->BrokerID, m_ReqUser.BrokerID, sizeof(pQryInvestorPositionCombineDetail->BrokerID)-1);
	strncpy(pQryInvestorPositionCombineDetail->InvestorID, m_ReqUser.UserID, sizeof(pQryInvestorPositionCombineDetail->InvestorID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryComPositionDetail_Req, (void*)pQryInvestorPositionCombineDetail, 
			sizeof(*pQryInvestorPositionCombineDetail), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
	//return (CTcpLayer::SendData(Cmd_TradeDataManager_QryComPositionDetail_Req, (void*)pQryInvestorPositionCombineDetail, 
	//		sizeof(*pQryInvestorPositionCombineDetail), 0, m_UserLogin.mnUserID, nRequestID) ?0:999);
#else
	CThostFtdcRspInfoField rspInfo;

	ZeroMemory(&rspInfo, sizeof(rspInfo));
	m_pSpi->OnRspQryInvestorPositionCombineDetail(NULL, &rspInfo, nRequestID, true);
	return 0;
#endif
}

///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
int CInstitudeTraderApi::ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField *pQryCFMMCTradingAccountKey, int nRequestID)
{
	return 0;
}

///�����ѯ�ֵ��۵���Ϣ
int CInstitudeTraderApi::ReqQryEWarrantOffset(CThostFtdcQryEWarrantOffsetField *pQryEWarrantOffset, int nRequestID)
{
	return 0;
}

///�����ѯת����ˮ
int CInstitudeTraderApi::ReqQryTransferSerial(CThostFtdcQryTransferSerialField *pQryTransferSerial, int nRequestID)
{
	return 0;
}

///�����ѯ����ǩԼ��ϵ
int CInstitudeTraderApi::ReqQryAccountregister(CThostFtdcQryAccountregisterField *pQryAccountregister, int nRequestID)
{
	return 0;
}

///�����ѯ����
int CInstitudeTraderApi::ReqQryExchangeRate(PlatformStru_QryExchangeRate *pQryExchangeRate, int nRequestID)
{
	return 0;
}

///�����ѯǩԼ����
int CInstitudeTraderApi::ReqQryContractBank(CThostFtdcQryContractBankField *pQryContractBank, int nRequestID)
{
#if 1
	strncpy(pQryContractBank->BrokerID, m_ReqUser.BrokerID, sizeof(pQryContractBank->BrokerID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryContractBank_Req, (void*)pQryContractBank, 
			sizeof(*pQryContractBank), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
#else
	CThostFtdcRspInfoField rspInfo;

	ZeroMemory(&rspInfo, sizeof(rspInfo));
	m_pSpi->OnRspQryContractBank(NULL, &rspInfo, nRequestID, true);
	return 0;
#endif
}

///�����ѯԤ��
int CInstitudeTraderApi::ReqQryParkedOrder(PlatformStru_QryParkedOrder *pQryParkedOrder, int nRequestID)
{
	strncpy(pQryParkedOrder->Thost.BrokerID, m_ReqUser.BrokerID, sizeof(pQryParkedOrder->Thost.BrokerID)-1);
	strncpy(pQryParkedOrder->Thost.InvestorID, m_ReqUser.UserID, sizeof(pQryParkedOrder->Thost.InvestorID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryParkedOrder_Req, (void*)pQryParkedOrder, 
			sizeof(*pQryParkedOrder), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

///�����ѯԤ�񳷵�
int CInstitudeTraderApi::ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction *pQryParkedOrderAction, int nRequestID)
{
	strncpy(pQryParkedOrderAction->Thost.BrokerID, m_ReqUser.BrokerID, sizeof(pQryParkedOrderAction->Thost.BrokerID)-1);
	strncpy(pQryParkedOrderAction->Thost.InvestorID, m_ReqUser.UserID, sizeof(pQryParkedOrderAction->Thost.InvestorID)-1);
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_TradeDataManager_QryParkedOrderAction_Req, (void*)pQryParkedOrderAction, 
			sizeof(*pQryParkedOrderAction), 0, m_UserLogin.mnUserID, nRequestID) ?0:999;
	else
		return 999;
}

///�����ѯ����֪ͨ
int CInstitudeTraderApi::ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID)
{
	SubsUserMsg(m_UserLogin.mnUserID);
	return 0;
}

///�����ѯ���͹�˾���ײ���
int CInstitudeTraderApi::ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID)
{
	return 0;
}

///�����ѯ���͹�˾�����㷨
int CInstitudeTraderApi::ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID)
{
	return 0;
}

///�ڻ����������ʽ�ת�ڻ�����
int CInstitudeTraderApi::ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer *pReqTransfer, int nRequestID)
{
	return 0;
}

///�ڻ������ڻ��ʽ�ת��������
int CInstitudeTraderApi::ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer *pReqTransfer, int nRequestID)
{
	return 0;
}

///�ڻ������ѯ�����������
int CInstitudeTraderApi::ReqQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, int nRequestID)
{
	return 0;
}

bool CInstitudeTraderApi::SubsUserMsg(int nUserID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_UserMsg_SubsMsg_Req, NULL, 0, 0, nUserID) ? true : false;
	else
		return false;
	//return CTcpLayer::SendData(Cmd_UserMsg_SubsMsg_Req, NULL, 0, 0, nUserID);
}

// �˶��û���Ϣ
bool CInstitudeTraderApi::UnsubsUserMsg(int nUserID)
{
	if(m_pTcpConnect != NULL)
		return m_pTcpConnect->SendData(Cmd_UserMsg_UnSubsMsg_Req, NULL, 0, 0, nUserID) ? true : false;
	else
		return false;
	//return CTcpLayer::SendData(Cmd_UserMsg_UnSubsMsg_Req, NULL, 0, 0, nUserID);
}

void CInstitudeTraderApi::Convert(sTraderLoginRsp& loginRsp, CThostFtdcRspUserLoginField& login)
{
	///������
	strncpy(login.TradingDay, loginRsp.mTradingDay, sizeof(login.TradingDay)-1);
	///��¼�ɹ�ʱ��
	strncpy(login.LoginTime, loginRsp.mLoginTime, sizeof(login.LoginTime)-1);
	///���͹�˾����
	strncpy(login.BrokerID, m_ReqUser.BrokerID, sizeof(login.BrokerID)-1);
	//strncpy(login.BrokerID, loginRsp.mBrokerID, sizeof(login.BrokerID)-1);
	///�û�����
	strncpy(login.UserID, m_ReqUser.UserID, sizeof(login.UserID)-1);
	//itoa(loginRsp.mnUserID, login.UserID, 10);
	///����ϵͳ����
	strncpy(login.SystemName, loginRsp.mSystemName, sizeof(login.SystemName)-1);
	///ǰ�ñ��
	login.FrontID = loginRsp.mnFrontID;
	///�Ự���
	login.SessionID = loginRsp.mnSessionID;
	///��󱨵�����
	_itoa_s(loginRsp.mnMinOrderRef, login.MaxOrderRef, sizeof(login.MaxOrderRef)-1, 10);
	///������ʱ��
	strncpy(login.SHFETime, loginRsp.mSHFETime, sizeof(login.SHFETime)-1);
	///������ʱ��
	strncpy(login.DCETime, loginRsp.mDCETime, sizeof(login.DCETime)-1);
	///֣����ʱ��
	strncpy(login.CZCETime, loginRsp.mCZCETime, sizeof(login.CZCETime)-1);
	///�н���ʱ��
	strncpy(login.FFEXTime, loginRsp.mFFEXTime, sizeof(login.FFEXTime)-1);
}

void CInstitudeTraderApi::OnFrontConnected()
{
	LockMe();
	if(m_pSpi!=NULL)
		m_pSpi->OnFrontConnected();
	UnLockMe();
}

void CInstitudeTraderApi::OnFrontDisconnected()
{
	LockMe();
	if(m_pSpi!=NULL)
		m_pSpi->OnFrontDisconnected(0x2001);
	UnLockMe();
}

// �������ݣ������յ�tcp����ʱ����
BOOL CInstitudeTraderApi::ParseData(char* pData, int nDataLen, DWORD nCmdId, DWORD nModuleId, 
		DWORD nSeq, DWORD nSubSeq, DWORD nUserData1, DWORD nUserData2, DWORD nUserData3, 
		DWORD nUserData4)
{
	if(pData == NULL && nDataLen<=0)
		return FALSE;

	int nRequestID = 0;
	CThostFtdcRspInfoField rspInfo = {0};
	switch(nCmdId) {
	case CMDID_Trader_Login_Rsp:
	{
		if(nUserData1 == CF_ERROR_SUCCESS) {	
			if(nDataLen==sizeof(sTraderLoginRsp)) {

				CThostFtdcRspUserLoginField login;
				ZeroMemory(&login, sizeof(login));

				sTraderLoginRsp loginRsp;
				ZeroMemory(&loginRsp, sizeof(loginRsp));

				memcpy(&loginRsp, pData, sizeof(loginRsp));
				Convert(loginRsp, login);
				m_UserLogin = loginRsp;

				SubsUserMsg(m_UserLogin.mnUserID);

				//m_UserLogin = login;
				LockMe();
				if(m_pSpi!=NULL)
					m_pSpi->OnRspUserLogin(&login, &rspInfo, nUserData2, true);
				UnLockMe();
			}
		}
		else {
			rspInfo.ErrorID = 3;
			strncpy(rspInfo.ErrorMsg, pData, min(nDataLen, sizeof(rspInfo.ErrorMsg)-1));
			LockMe();
			if(m_pSpi!=NULL)
				m_pSpi->OnRspUserLogin(NULL, &rspInfo, nUserData2, true);
			UnLockMe();
		}
	}
		break;
	case Cmd_ModifyUserPassword_Rsp:
	{
		if(nUserData1 == CF_ERROR_SUCCESS) {	

			CThostFtdcUserPasswordUpdateField rspChg;
			ZeroMemory(&rspChg, sizeof(rspChg));

			strncpy(rspChg.BrokerID, m_ReqUser.BrokerID, sizeof(rspChg.BrokerID)-1);
			strncpy(rspChg.UserID, m_ReqUser.UserID, sizeof(rspChg.UserID)-1);
			strncpy(rspChg.NewPassword, m_UserPasswordUpdate.NewPassword, sizeof(rspChg.NewPassword)-1);
			strncpy(rspChg.OldPassword, m_UserPasswordUpdate.OldPassword, sizeof(rspChg.OldPassword)-1);
			LockMe();
			if(m_pSpi!=NULL)
				m_pSpi->OnRspUserPasswordUpdate(&rspChg, &rspInfo, nUserData2, true);
			UnLockMe();
		}
		else {
			rspInfo.ErrorID = 999;
			strncpy(rspInfo.ErrorMsg, pData, min(nDataLen, sizeof(rspInfo.ErrorMsg)-1));
			LockMe();
			if(m_pSpi!=NULL)
				m_pSpi->OnRspUserPasswordUpdate(NULL, &rspInfo, nUserData2, true);
			UnLockMe();
		}
	}
		break;
	case Cmd_TradeDataManager_QrySettlementConfirm_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {	
			if(nDataLen==sizeof(CThostFtdcSettlementInfoConfirmField)) {

				CThostFtdcSettlementInfoConfirmField sttlConfirm;
				ZeroMemory(&sttlConfirm, sizeof(sttlConfirm));

				memcpy(&sttlConfirm, pData, sizeof(sttlConfirm));

				LockMe();
				if(m_pSpi!=NULL)
					m_pSpi->OnRspQrySettlementInfoConfirm(&sttlConfirm, &rspInfo, nUserData2, true);
				UnLockMe();
			}
		}
		else {
			rspInfo.ErrorID = 999;
			strncpy(rspInfo.ErrorMsg, pData, min(nDataLen, sizeof(rspInfo.ErrorMsg)-1));
			LockMe();
			if(m_pSpi!=NULL)
				m_pSpi->OnRspQrySettlementInfoConfirm(NULL, &rspInfo, nUserData2, true);
			UnLockMe();
		}
	}
		break;
	case Cmd_TradeDataManager_SettlementConfirm_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {	
			if(nDataLen==sizeof(CThostFtdcSettlementInfoConfirmField)) {

				CThostFtdcSettlementInfoConfirmField sttlConfirm;
				ZeroMemory(&sttlConfirm, sizeof(sttlConfirm));

				memcpy(&sttlConfirm, pData, sizeof(sttlConfirm));

				LockMe();
				if(m_pSpi!=NULL)
					m_pSpi->OnRspSettlementInfoConfirm(&sttlConfirm, &rspInfo, nUserData2, true);
				UnLockMe();
			}
		}
		else {
			rspInfo.ErrorID = 999;
			strncpy(rspInfo.ErrorMsg, pData, min(nDataLen, sizeof(rspInfo.ErrorMsg)-1));
			LockMe();
			if(m_pSpi!=NULL)
				m_pSpi->OnRspSettlementInfoConfirm(NULL, &rspInfo, nUserData2, true);
			UnLockMe();
		}
	}
		break;
	case Cmd_Settlement_SettleBill_Rsp:
	{
#if 1
		if(nUserData3 == CF_ERROR_SUCCESS) {	
			if(nDataLen%sizeof(CThostFtdcSettlementInfoField)==0) {
			
				int nItemCount = nDataLen/sizeof(CThostFtdcSettlementInfoField);
				if(nItemCount > 0) {
					CThostFtdcSettlementInfoField settleInfo;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&settleInfo, pData+i*sizeof(settleInfo), sizeof(settleInfo));
						LockMe();
						if(m_pSpi!=NULL)
							m_pSpi->OnRspQrySettlementInfo(&settleInfo, &rspInfo, nUserData2, nUserData4);
						UnLockMe();
					}
				}
				else {
					LockMe();
					if(m_pSpi!=NULL)
						m_pSpi->OnRspQrySettlementInfo(NULL, &rspInfo, nUserData2, true);
					UnLockMe();
				}
			}
		}
		else {
			rspInfo.ErrorID = 999;
			strncpy(rspInfo.ErrorMsg, pData, min(nDataLen, sizeof(rspInfo.ErrorMsg)-1));
			LockMe();
			if(m_pSpi!=NULL)
				m_pSpi->OnRspQrySettlementInfo(NULL, &rspInfo, nUserData2, true);
			UnLockMe();
		}
#else
		CThostFtdcSettlementInfoField settleInfo = {0};
		strncpy(settleInfo.BrokerID, m_ReqUser.BrokerID, sizeof(settleInfo.BrokerID)-1);
		strncpy(settleInfo.InvestorID, m_ReqUser.UserID, sizeof(settleInfo.InvestorID)-1);
		strcpy(settleInfo.Content, "��ǰ�޽��㵥����ֱ�ӵ�ȷ�ϰ�ť����ȷ�ϡ�");
		LockMe();
		if(m_pSpi!=NULL)
			m_pSpi->OnRspQrySettlementInfo(&settleInfo, &rspInfo, nUserData2, true);
		UnLockMe();
#endif
	}
		break;
	case Cmd_TradeDataManager_QryInstrument_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_InstrumentInfo)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_InstrumentInfo);
				if(nItemCount > 0) {
					bool bIsLast = false;
					PlatformStru_InstrumentInfo instInfo;

					for(int i=0; i<nItemCount; i++) {
						bIsLast = (i==nItemCount-1);
						memcpy(&instInfo, pData+i*sizeof(instInfo), sizeof(instInfo));
						LockMe();
						if(m_pSpi!=NULL)
							m_pSpi->OnRspQryInstrument(&instInfo, &rspInfo, nUserData2, bIsLast);
						UnLockMe();
					}
				}
				else {
					LockMe();
					if(m_pSpi!=NULL)
						m_pSpi->OnRspQryInstrument(NULL, &rspInfo, nUserData2, true);
					UnLockMe();
				}
			}
		}
		else {
			rspInfo.ErrorID = 999;
			strncpy(rspInfo.ErrorMsg, pData, min(nDataLen, sizeof(rspInfo.ErrorMsg)-1));
			LockMe();
			if(m_pSpi!=NULL)
				m_pSpi->OnRspQryInstrument(NULL, &rspInfo, nUserData2, true);
			UnLockMe();
		}
	}
		break;
	case Cmd_TradeDataManager_QryTraderAccount_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_TradingAccountInfo)==0) {
				
				int nItemCount = nDataLen/sizeof(PlatformStru_TradingAccountInfo);
				if(nItemCount > 0) {
					bool bIsLast = false;
					PlatformStru_TradingAccountInfo fundInfo;

					for(int i=0; i<nItemCount; i++) {
						bIsLast = (i==nItemCount-1);
						memcpy(&fundInfo, pData+i*sizeof(fundInfo), sizeof(fundInfo));
						strncpy(fundInfo.BrokerID, m_ReqUser.BrokerID, sizeof(fundInfo.BrokerID)-1);
						strncpy(fundInfo.AccountID, m_ReqUser.UserID, sizeof(fundInfo.AccountID)-1);
						LockMe();
						if(m_pSpi!=NULL)
							m_pSpi->OnRspQryTradingAccount(&fundInfo, &rspInfo, nUserData2, bIsLast);
						UnLockMe();
					}
				}
				else {
					LockMe();
					if(m_pSpi!=NULL)
						m_pSpi->OnRspQryTradingAccount(NULL, &rspInfo, nUserData2, true);
					UnLockMe();
				}
			}
		}
		else {
			rspInfo.ErrorID = 999;
			strncpy(rspInfo.ErrorMsg, pData, min(nDataLen, sizeof(rspInfo.ErrorMsg)-1));
			LockMe();
			if(m_pSpi!=NULL)
				m_pSpi->OnRspQryTradingAccount(NULL, &rspInfo, nUserData2, true);
			UnLockMe();
		}
	}
		break;
	case Cmd_TradeDataManager_QryTrade_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_TradeInfo)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_TradeInfo);
				if(nItemCount > 0) {
				bool bIsLast = false;
					PlatformStru_TradeInfo tradeInfo;

					for(int i=0; i<nItemCount; i++) {
						bIsLast = (i==nItemCount-1);
						memcpy(&tradeInfo, pData+i*sizeof(tradeInfo), sizeof(tradeInfo));
						LockMe();
						if(m_pSpi!=NULL) {
							m_pSpi->OnRspQryTrade(&tradeInfo, &rspInfo, nUserData2, bIsLast);
						}
						UnLockMe();
					}
				}
				else {
					LockMe();
					if(m_pSpi!=NULL) {
						m_pSpi->OnRspQryTrade(NULL, &rspInfo, nUserData2, true);
					}
					UnLockMe();
				}
			}
		}
		else {
			rspInfo.ErrorID = 999;
			strncpy(rspInfo.ErrorMsg, pData, min(nDataLen, sizeof(rspInfo.ErrorMsg)-1));
			LockMe();
			if(m_pSpi!=NULL) {
				m_pSpi->OnRspQryTrade(NULL, &rspInfo, nUserData2, true);
			}
			UnLockMe();
		}
	}
		break;
	case Cmd_TradeDataManager_QryOrder_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_OrderInfo)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_OrderInfo);
				if(nItemCount > 0) {
					bool bIsLast = false;
					PlatformStru_OrderInfo orderInfo;

					for(int i=0; i<nItemCount; i++) {
						bIsLast = (i==nItemCount-1);
						memcpy(&orderInfo, pData+i*sizeof(orderInfo), sizeof(orderInfo));
						LockMe();
						if(m_pSpi!=NULL) {
							m_pSpi->OnRspQryOrder(&orderInfo, &rspInfo, nUserData2, bIsLast);
						}
						UnLockMe();
					}
				}
				else {
					LockMe();
					if(m_pSpi!=NULL) {
						m_pSpi->OnRspQryOrder(NULL, &rspInfo, nUserData2, true);
					}
					UnLockMe();
				}
			}
		}
		else {
			rspInfo.ErrorID = 999;
			strncpy(rspInfo.ErrorMsg, pData, min(nDataLen, sizeof(rspInfo.ErrorMsg)-1));
			LockMe();
			if(m_pSpi!=NULL) {
				m_pSpi->OnRspQryOrder(NULL, &rspInfo, nUserData2, true);
			}
			UnLockMe();
		}
	}
		break;
	case Cmd_TradeDataManager_QryPositionDetail_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_PositionDetail)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_PositionDetail);
				if(nItemCount > 0) {
					bool bIsLast = false;
					PlatformStru_PositionDetail detailInfo;

					for(int i=0; i<nItemCount; i++) {
						bIsLast = (i==nItemCount-1);
						memcpy(&detailInfo, pData+i*sizeof(detailInfo), sizeof(detailInfo));
						LockMe();
						if(m_pSpi!=NULL)
							m_pSpi->OnRspQryInvestorPositionDetail(&detailInfo, &rspInfo, nUserData2, bIsLast);
						UnLockMe();
					}
				}
				else {
					LockMe();
					if(m_pSpi!=NULL)
						m_pSpi->OnRspQryInvestorPositionDetail(NULL, &rspInfo, nUserData2, true);
					UnLockMe();
				}
			}
		}
		else {
			rspInfo.ErrorID = 999;
			strncpy(rspInfo.ErrorMsg, pData, min(nDataLen, sizeof(rspInfo.ErrorMsg)-1));
			LockMe();
			if(m_pSpi!=NULL)
				m_pSpi->OnRspQryInvestorPositionDetail(NULL, &rspInfo, nUserData2, true);
			UnLockMe();
		}
	}
		break;
	case Cmd_TradeDataManager_QryPosition_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_Position)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_Position);
				if(nItemCount > 0) {
					bool bIsLast = false;
					PlatformStru_Position posiInfo;

					for(int i=0; i<nItemCount; i++) {
						bIsLast = (i==nItemCount-1);
						memcpy(&posiInfo, pData+i*sizeof(posiInfo), sizeof(posiInfo));
						LockMe();
						if(m_pSpi!=NULL)
							m_pSpi->OnRspQryInvestorPosition(&posiInfo, &rspInfo, nUserData2, bIsLast);
						UnLockMe();
					}
				}
				else {
					LockMe();
					if(m_pSpi!=NULL)
						m_pSpi->OnRspQryInvestorPosition(NULL, &rspInfo, nUserData2, true);
					UnLockMe();
				}
			}
		}
		else {
			rspInfo.ErrorID = 999;
			rspInfo.ErrorID = 0;
			strncpy(rspInfo.ErrorMsg, pData, min(nDataLen, sizeof(rspInfo.ErrorMsg)-1));
			LockMe();
			if(m_pSpi!=NULL)
				m_pSpi->OnRspQryInvestorPosition(NULL, &rspInfo, nUserData2, true);
			UnLockMe();
		}
	}
		break;
	case Cmd_TradeDataManager_QryComPositionDetail_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_PositionDetailComb)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_PositionDetailComb);
				if(nItemCount > 0) {
					bool bIsLast = false;
					PlatformStru_PositionDetailComb detailCombInfo;

					for(int i=0; i<nItemCount; i++) {
						bIsLast = (i==nItemCount-1);
						memcpy(&detailCombInfo, pData+i*sizeof(detailCombInfo), sizeof(detailCombInfo));
						LockMe();
						if(m_pSpi!=NULL)
							m_pSpi->OnRspQryInvestorPositionCombineDetail(&detailCombInfo, &rspInfo, nUserData2, bIsLast);
						UnLockMe();
					}
				}
				else {
					LockMe();
					if(m_pSpi!=NULL)
						m_pSpi->OnRspQryInvestorPositionCombineDetail(NULL, &rspInfo, nUserData2, true);
					UnLockMe();
				}
			}
		}
		else {
			rspInfo.ErrorID = 999;
			strncpy(rspInfo.ErrorMsg, pData, min(nDataLen, sizeof(rspInfo.ErrorMsg)-1));
			LockMe();
			if(m_pSpi!=NULL)
				m_pSpi->OnRspQryInvestorPositionCombineDetail(NULL, &rspInfo, nUserData2, true);
			UnLockMe();
		}
	}
		break;
	case Cmd_TradeDataManager_QryCommissionRat_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_InstrumentCommissionRate)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_InstrumentCommissionRate);
				if(nItemCount > 0) {
					bool bIsLast = false;
					PlatformStru_InstrumentCommissionRate csrInfo;

					for(int i=0; i<nItemCount; i++) {
						bIsLast = (i==nItemCount-1);
						memcpy(&csrInfo, pData+i*sizeof(csrInfo), sizeof(csrInfo));
						LockMe();
						if(m_pSpi!=NULL)
							m_pSpi->OnRspQryInstrumentCommissionRate(&csrInfo, &rspInfo, nUserData2, bIsLast);
						UnLockMe();
					}
				}
				else {
					LockMe();
					if(m_pSpi!=NULL)
						m_pSpi->OnRspQryInstrumentCommissionRate(NULL, &rspInfo, nUserData2, true);
					UnLockMe();
				}
			}
		}
		else {
			rspInfo.ErrorID = CustomErrorID;
			strncpy(rspInfo.ErrorMsg, pData, min(nDataLen, sizeof(rspInfo.ErrorMsg)-1));
			LockMe();
			if(m_pSpi!=NULL)
				m_pSpi->OnRspQryInstrumentCommissionRate(NULL, &rspInfo, nUserData2, true);
			UnLockMe();
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
						if(m_pSpi!=NULL)
							m_pSpi->OnRspQryInstrumentMarginRate(&mgrInfo, &rspInfo, nUserData2, bIsLast);
						UnLockMe();
					}
				}
				else {
					LockMe();
					if(m_pSpi!=NULL)
						m_pSpi->OnRspQryInstrumentMarginRate(NULL, &rspInfo, nUserData2, true);
					UnLockMe();
				}
			}
		}
		else {
			rspInfo.ErrorID = CustomErrorID;
			strncpy(rspInfo.ErrorMsg, pData, min(nDataLen, sizeof(rspInfo.ErrorMsg)-1));
			LockMe();
			if(m_pSpi!=NULL)
				m_pSpi->OnRspQryInstrumentMarginRate(NULL, &rspInfo, nUserData2, true);
			UnLockMe();
		}
	}
		break;
	case Cmd_TradeExcute_Trader_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen==sizeof(PlatformStru_TradeInfo)) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_TradeInfo);
				if(nItemCount > 0) {
					PlatformStru_TradeInfo tradeInfo;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&tradeInfo, pData+i*sizeof(tradeInfo), sizeof(tradeInfo));
						LockMe();
						if(m_pSpi!=NULL) {
							m_pSpi->OnRtnTrade(&tradeInfo);
						}
						UnLockMe();
					}
				}
			}
		}
	}
		break;
	case Cmd_TradeExcute_OrderStatus_Rsp:
	{
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen==sizeof(PlatformStru_OrderInfo)) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_OrderInfo);
				if(nItemCount > 0) {
					PlatformStru_OrderInfo orderInfo;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&orderInfo, pData+i*sizeof(orderInfo), sizeof(orderInfo));

						if(GlobalConfigManager::HasSaveOrderTime()) {
							static std::set<std::string> ssetFirstOrder;
							static std::set<std::string> ssetFirstCtp;
							std::set<std::string>::iterator it;
							bool bHasWrite = false;
							std::string strFile;
							char csKey[1024];
							GlobalFunc::GetPath(strFile);
							strFile += "userdata\\";
							strFile += m_UserLogin.mTradingDay;
							
							memset(csKey, 0, sizeof(csKey));
							sprintf_s(csKey, "%d,%d,%s", orderInfo.FrontID, 
										orderInfo.SessionID, orderInfo.OrderRef);
							if(ssetFirstOrder.find(csKey)==ssetFirstOrder.end()) {
								bHasWrite = true;
								ssetFirstOrder.insert(csKey);
								strFile += "-first.dat";
							}

							if(!bHasWrite && orderInfo.OrderLocalID[0]!=0) {
								memset(csKey, 0, sizeof(csKey));
								sprintf_s(csKey, "%d,%d,%s", orderInfo.FrontID, 
											orderInfo.SessionID, orderInfo.OrderRef);
								if(!bHasWrite && ssetFirstCtp.find(csKey)==ssetFirstCtp.end()) {
									bHasWrite = true;
									ssetFirstCtp.insert(csKey);
									strFile += "-ctp.dat";
								}
							}

							if(bHasWrite) {
								SYSTEMTIME sysTime = {0};
								ORDERTIME item = {0};

								::GetLocalTime(&sysTime);
								item.FrontID = m_UserLogin.mnFrontID;
								item.SessionID = m_UserLogin.mnSessionID;
								strncpy(item.OrderRef, orderInfo.OrderRef, sizeof(item.OrderRef));
								strncpy(item.BrokerID, orderInfo.BrokerID, sizeof(item.BrokerID));
								strncpy(item.InvestorID, orderInfo.InvestorID, sizeof(item.InvestorID));
								item.DateTime = sysTime;
								WriteOrderTimeToFile(strFile, item);
							}
						}

						LockMe();
						if(m_pSpi!=NULL) {
							m_pSpi->OnRtnOrder(&orderInfo);
						}
						UnLockMe();
					}
				}
			}
		}
	}
		break;
	case Cmd_TradeExcute_CancelOrder_Fail_RSP:
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen==(sizeof(PlatformStru_InputOrderAction)+sizeof(CThostFtdcRspInfoField))) {
			
				PlatformStru_InputOrderAction iaInfo;

				memcpy(&iaInfo, pData, sizeof(iaInfo));
				memcpy(&rspInfo, pData+sizeof(iaInfo), sizeof(rspInfo));
				LockMe();
				if(m_pSpi!=NULL)
					m_pSpi->OnRspOrderAction(&iaInfo, &rspInfo, nUserData2, true);
				UnLockMe();
			}
		}
		break;
	case Cmd_TradeExcute_CancelOrder_Error_RSP:
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen==sizeof(CThostFtdcOrderActionField)+sizeof(CThostFtdcRspInfoField)) {
			
				CThostFtdcOrderActionField oaInfo;

				memcpy(&oaInfo, pData, sizeof(oaInfo));
				memcpy(&rspInfo, pData+sizeof(oaInfo), sizeof(rspInfo));
				LockMe();
				if(m_pSpi!=NULL)
					m_pSpi->OnErrRtnOrderAction(&oaInfo, &rspInfo);
				UnLockMe();
			}
		}
		break;
	case Cmd_TradeDataManager_QryNotice_Rsp:
		if(nUserData3 == CF_ERROR_SUCCESS) {
			//if(nDataLen==0 || nDataLen==sizeof(CThostFtdcNoticeField)+sizeof(CThostFtdcRspInfoField)) {
			if(nDataLen==0) {
				LockMe();
				if(m_pSpi!=NULL)
					m_pSpi->OnRspQryNotice(NULL, &rspInfo, nUserData2, true);
				UnLockMe();
			}
		}
		break;
	case Cmd_TradeDataManager_QryContractBank_Rsp:
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen==0) {
			//if(nDataLen==0 || nDataLen==sizeof(CThostFtdcContractBankField)+sizeof(CThostFtdcRspInfoField)) {
				LockMe();
				if(m_pSpi!=NULL)
					m_pSpi->OnRspQryContractBank(NULL, &rspInfo, nUserData2, true);
				UnLockMe();
			}
		}
		break;
	case Cmd_TradeDataManager_QryParkerOrder_Rsp:
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen==0) {
			//if(nDataLen==0 || nDataLen==sizeof(CThostFtdcContractBankField)+sizeof(CThostFtdcRspInfoField)) {
				LockMe();
				if(m_pSpi!=NULL)
					m_pSpi->OnRspQryParkedOrder(NULL, &rspInfo, nUserData2, true);
				UnLockMe();
			}
		}
		break;
	case Cmd_TradeDataManager_QryParkerOrderAction_Rsp:
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen==0) {
			//if(nDataLen==0 || nDataLen==sizeof(CThostFtdcContractBankField)+sizeof(CThostFtdcRspInfoField)) {
				LockMe();
				if(m_pSpi!=NULL)
					m_pSpi->OnRspQryParkedOrderAction(NULL, &rspInfo, nUserData2, true);
				UnLockMe();
			}
		}
		break;
	case Cmd_TradeDataManager_QryDepthMarket_Rsp:
		if(nUserData3 == CF_ERROR_SUCCESS) {
			if(nDataLen%sizeof(PlatformStru_DepthMarketData)==0) {
			
				int nItemCount = nDataLen/sizeof(PlatformStru_DepthMarketData);
				if(nItemCount > 0) {
					PlatformStru_DepthMarketData depth;

					for(int i=0; i<nItemCount; i++) {
						memcpy(&depth, pData+i*sizeof(depth), sizeof(depth));
						LockMe();
						if(m_pSpi!=NULL) {
							m_pSpi->OnRspQryDepthMarketData(&depth, &rspInfo, nUserData2, i<nItemCount-1 ? false : true);
						}
						UnLockMe();
					}
				}
			}
		}
		break;
	case Cmd_UserMsg_SubsMsg_Rsp:
		if(nUserData1 == CF_ERROR_SUCCESS) {
			if(nDataLen==0) {
			}
		}
		break;
	case Cmd_UserMsg_PushMsg:
		if(nUserData1 == CF_ERROR_SUCCESS) {
			int nCount = nDataLen / sizeof(UserMsgStru);
			CThostFtdcTradingNoticeInfoField notice;
			for(int i = 0; i < nCount; i++) {
				UserMsgStru* pMsg = (UserMsgStru*)(pData + i*sizeof(UserMsgStru));
				memset(&notice, 0, sizeof(notice));
				notice.SequenceNo = pMsg->MsgID;
				strncpy(notice.InvestorID, m_ReqUser.UserID, sizeof(notice.InvestorID));
				strncpy(notice.SendTime, pMsg->SendDateTime+11, sizeof(notice.SendTime)-1);
				sprintf_s(notice.FieldContent, sizeof(notice.FieldContent)-1, 
						"����: %s\r\n����: \r\n%s", pMsg->Title, pMsg->Msg);
				LockMe();
				if(m_pSpi!=NULL)
					m_pSpi->OnRtnTradingNotice(&notice);
				UnLockMe();
				//TRACE("Recv User Msg:\nTitle:\n%s\nContent:\n%s\n", pMsg->Title, pMsg->Msg);
			}
		}
		break;
	};
	return 0;
}

BOOL CInstitudeTraderApi::WriteOrderTimeToFile(std::string& strFile, ORDERTIME& item)
{
	HANDLE hFile = ::CreateFile(strFile.c_str(), GENERIC_WRITE, 
                   FILE_SHARE_READ, NULL, 
                   OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 
				   NULL);
	if(hFile != INVALID_HANDLE_VALUE) {
		DWORD dwHighLen = 0;
		DWORD dwLen = ::GetFileSize(hFile, &dwHighLen);
		::SetFilePointer(hFile, (LONG)dwLen, (LONG*)&dwHighLen, FILE_BEGIN);
		::WriteFile(hFile, &item, sizeof(item), &dwLen, NULL);
		::CloseHandle(hFile);
		return TRUE;
	}
	return FALSE;
}
