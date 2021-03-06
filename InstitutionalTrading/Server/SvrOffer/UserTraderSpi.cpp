#include "StdAfx.h"
#include "CTPAccount.h"
#include "ToolThread.h"
#include "UserTraderSpi.h"
#include "OfferMainInterface.h"
extern COfferMainInterface*	g_pOfferMain;
UINT_PTR            g_lTimeDisconnect;
CUserTraderSpi::CUserTraderSpi(CThostFtdcTraderApi *pUserApi, SLogin& sLogin) : m_pUserApi(pUserApi)
{
//	memset(&m_sLogin, 0, sizeof(SLogin));
	m_sLogin = sLogin;
	m_nRequestID = 0;
	
	m_pCTPAccount = NULL;
	g_lTimeDisconnect = 0;
}
CUserTraderSpi::~CUserTraderSpi(void)
{
	KillTimer(NULL, g_lTimeDisconnect);
}
///合约交易状态通知
void CUserTraderSpi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) 
{//pInstrumentStatus 唯一性才发送

	CInterface_SvrTradeData::getObj().SetIntrumentsStatus(*pInstrumentStatus);
};

void CUserTraderSpi::SetAccount(CCTPAccount* pCTPAccount)
{
	m_pCTPAccount = pCTPAccount;
}
// 当客户端与交易托管系统建立起通信连接，客户端需要进行登录
void CUserTraderSpi::OnFrontConnected()
{
	CThostFtdcReqUserLoginField reqUserLogin;
	memset(&reqUserLogin, 0, sizeof(reqUserLogin));
	strcpy(reqUserLogin.BrokerID,	m_sLogin.strBrokerID.c_str());
	strcpy(reqUserLogin.UserID,		m_sLogin.strUserID.c_str());
	strcpy(reqUserLogin.Password,	m_sLogin.strPassword.c_str());
	strcpy(reqUserLogin.UserProductInfo, "Instituate");
	m_pUserApi->ReqUserLogin(&reqUserLogin,GetRequestID());

	m_pCTPAccount->SetAccountStatus(ACCOUNT_STATUS_Connected);

	if(!m_pCTPAccount->GetIsQueryRatio())//查询费率线程不给交易数据管理状态
	{		
		CInterface_SvrTradeData::getObj().SetAccountStatus(m_sLogin.strUserID, m_sLogin.strBrokerID, ACCOUNT_STATUS_Connected);
		OFFER_INFO("主账号建立连接，开始登陆： %s, %s\n", reqUserLogin.BrokerID, reqUserLogin.UserID);
		PUSH_LOG_ERROR(Logger,false,true,"主账号建立连接，开始登陆： %s, %s\n", reqUserLogin.BrokerID, reqUserLogin.UserID);
	}
	else
	{
		OFFER_INFO("查询费率账号建立连接，开始登陆： %s, %s\n", reqUserLogin.BrokerID, reqUserLogin.UserID);
		PUSH_LOG_ERROR(Logger,false,true,"查询费率账号建立连接，开始登陆： %s, %s\n", reqUserLogin.BrokerID, reqUserLogin.UserID);
	}
}
// 当客户端与交易托管系统通信连接断开时，该方法被调用
void CUserTraderSpi::OnFrontDisconnected(int nReason)
{	
	
	m_pCTPAccount->SetAccountStatus(ACCOUNT_STATUS_Disconnected);
	if(!m_pCTPAccount->GetIsQueryRatio())//查询费率线程不给交易数据管理状态
	{
		OFFER_INFO("主账号断开连接： %s, %s\n", m_sLogin.strBrokerID.c_str() , m_sLogin.strUserID.c_str());
		PUSH_LOG_ERROR(FirstLevelError,true,true,"主账号断开连接： %s, %s\n", m_sLogin.strBrokerID.c_str() , m_sLogin.strUserID.c_str());

		CInterface_SvrTradeData::getObj().SetAccountStatus(m_sLogin.strUserID, m_sLogin.strBrokerID, ACCOUNT_STATUS_Disconnected);
		m_pCTPAccount->SetQueryConnectFail();//查询过程中则查询失败
	
		if(!g_pOfferMain->IsCtpAccountTimerExit(m_pCTPAccount))
		{			
			g_lTimeDisconnect = SetTimer(NULL,NULL,10000,(TIMERPROC)(NULL)); 
			g_pOfferMain->SetTimer2CTPAccount(g_lTimeDisconnect, m_pCTPAccount);
			
			if(m_pCTPAccount->GetIsQueryRatio())
			{
				OFFER_INFO("查费率账号断开连接ctp： %s, %s\n", m_sLogin.strBrokerID.c_str() , m_sLogin.strUserID.c_str())
				PUSH_LOG_ERROR(FirstLevelError,true,true,"查费率账号断开连接ctp： %s, %s\n", m_sLogin.strBrokerID.c_str() , m_sLogin.strUserID.c_str())
			}
			else
			{
				OFFER_INFO("主账号断开连接ctp： %s, %s\n", m_sLogin.strBrokerID.c_str() , m_sLogin.strUserID.c_str())
				PUSH_LOG_ERROR(FirstLevelError,true,true,"主账号断开连接ctp： %s, %s\n", m_sLogin.strBrokerID.c_str() , m_sLogin.strUserID.c_str())
			}
		}
	}
	else
	{
		OFFER_INFO("查费率账号断开连接： %s, %s\n", m_sLogin.strBrokerID.c_str() , m_sLogin.strUserID.c_str());
		PUSH_LOG_ERROR(FirstLevelError,true,true,"查费率账号断开连接： %s, %s\n", m_sLogin.strBrokerID.c_str() , m_sLogin.strUserID.c_str());

	}
}

// 当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
void CUserTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
							CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo->ErrorID != 0)
	{// 端登失败，客户端需进行错误处理	

		int nCount = m_pCTPAccount->GetLoginCount();
		m_pCTPAccount->SetLoginCount(++nCount);

		if(m_pCTPAccount->GetIsQueryRatio())	
		{
			OFFER_INFO("查费率账号登录失败%d次： %s, %s\n", nCount, pRspUserLogin->BrokerID, pRspUserLogin->UserID)
			//PUSH_LOG_ERROR(FirstLevelError,true,true,"查费率账号登录失败%d次： %s, %s\n", nCount, pRspUserLogin->BrokerID, pRspUserLogin->UserID)
		}
		else
		{
			OFFER_INFO("主账号登录失败%d次： %s, %s\n", nCount, pRspUserLogin->BrokerID, pRspUserLogin->UserID)	
			PUSH_LOG_ERROR(FirstLevelError,true,true,"主账号登录失败%d次： %s, %s\n", nCount, pRspUserLogin->BrokerID, pRspUserLogin->UserID)	
		}
		
		if(nCount < 3)
		{//登录三次后不能登上则不再登录;即使当时网络
			Sleep(2000);
			CThostFtdcReqUserLoginField reqUserLogin;
			memset(&reqUserLogin, 0, sizeof(reqUserLogin));
			strcpy(reqUserLogin.BrokerID,	m_sLogin.strBrokerID.c_str());
			strcpy(reqUserLogin.UserID,		m_sLogin.strUserID.c_str());
			strcpy(reqUserLogin.Password,	m_sLogin.strPassword.c_str());
			m_pUserApi->ReqUserLogin(&reqUserLogin, GetRequestID());
		}
		else
		{//登录三次不成功，就算失败
			m_pCTPAccount->SetAccountStatus(ACCOUNT_STATUS_LoginFailure);
			if(!m_pCTPAccount->GetIsQueryRatio())//查询费率线程不给交易数据管理状态
				CInterface_SvrTradeData::getObj().SetAccountStatus(m_sLogin.strUserID, m_sLogin.strBrokerID, ACCOUNT_STATUS_LoginFailure);
			//if(m_pOfferMain)//设置账号登录完毕
			//	m_pOfferMain->SetCTPAccountFinishedLogin(pRspUserLogin->UserID);
		}
		return;
	}
	if(m_pCTPAccount->GetIsQueryRatio())
	{
		OFFER_INFO("查费率账号登录成功： %s, %s\n", pRspUserLogin->BrokerID, pRspUserLogin->UserID)
		PUSH_LOG_ERROR(Logger,false,true,"查费率账号登录成功： %s, %s\n", pRspUserLogin->BrokerID, pRspUserLogin->UserID)
	}
	else
	{
		OFFER_INFO("主账号登录成功： %s, %s\n", pRspUserLogin->BrokerID, pRspUserLogin->UserID)
		PUSH_LOG_ERROR(Logger,false,true,"主账号登录成功： %s, %s\n", pRspUserLogin->BrokerID, pRspUserLogin->UserID)
	}

	std::string TradingDay = pRspUserLogin->TradingDay;
	CInterface_SvrTradeData::getObj().SetCurrentTradingDay(TradingDay); 
//	if(m_pOfferMain)//设置账号登录完毕
//		m_pOfferMain->SetCTPAccountFinishedLogin(pRspUserLogin->UserID);
	////////////////////////////////////////////////////////////////
	//给登录模块发送登录时间
	sExchangeTimeInfo sTime;
	strcpy(sTime.CZCETime, pRspUserLogin->CZCETime);
	strcpy(sTime.DCETime, pRspUserLogin->DCETime);
	strcpy(sTime.FFEXTime, pRspUserLogin->FFEXTime);
	strcpy(sTime.SHFETime, pRspUserLogin->SHFETime);
	strcpy(sTime.TradingDay, pRspUserLogin->TradingDay);	
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf(sTime.GetTime, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);	
	CInterface_SvrLogin::getObj().SetTimeInfo(sTime);
	////////////////////////////////////////////////////////////////

	//结算确认
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_sLogin.strBrokerID.c_str());
	strcpy(req.InvestorID, m_sLogin.strUserID.c_str());
	m_pUserApi->ReqSettlementInfoConfirm(&req, GetRequestID());
	m_pCTPAccount->SetAccountStatus(ACCOUNT_STATUS_Login);
	if(!m_pCTPAccount->GetIsQueryRatio())//查询费率线程不给交易数据管理状态
		CInterface_SvrTradeData::getObj().SetAccountStatus(m_sLogin.strUserID, m_sLogin.strBrokerID, ACCOUNT_STATUS_Login);

	m_pCTPAccount->SetFrontID(pRspUserLogin->FrontID);
	m_pCTPAccount->SetSessionID(pRspUserLogin->SessionID);
	int nMaxOrderRef = atoi(pRspUserLogin->MaxOrderRef);
	m_pCTPAccount->SetCurrentOrderref(nMaxOrderRef);

	if(g_pOfferMain && g_pOfferMain->NeedQueryInstruments())
	{//判断是否需要查询合约
		g_pOfferMain->SetNeedQueryInstruments(false);
		m_pCTPAccount->QueryInstruments();
	}

//#ifdef  SIMU//模拟交易
//	if(!m_pCTPAccount->GetIsQueryRatio())//查询费率线程不给设置登录状态
//	{
//		CInterface_SvrSimulateTrade::getObj().ResertTradingDay(TradingDay);
//		CInterface_SvrSimulateTrade::getObj().Login(*pRspUserLogin);	
//	}
//#endif
}
void CUserTraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
#ifdef  SIMU//模拟交易
	CInterface_SvrSimulateTrade::getObj().Logout(*pUserLogout, nRequestID);	
#endif
	OFFER_INFO("账号登出： %s, %s\n", m_sLogin.strBrokerID.c_str() , m_sLogin.strUserID.c_str());
	PUSH_LOG_ERROR(Logger,false,true,"账号登出： %s, %s\n", m_sLogin.strBrokerID.c_str() , m_sLogin.strUserID.c_str());
};
// 针对用户请求的出错通知
void CUserTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID,
						bool bIsLast)
{
  char ls[500] = {0};

}
///请求查询合约响应
void CUserTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pInstrument == NULL)
		return;
	if(pInstrument)
	{
		OFFER_INFO("查询到合约： %s\n", pInstrument->InstrumentID);
		PUSH_LOG_ERROR(Logger,false,true,"查询到合约： %s\n", pInstrument->InstrumentID);
	}

	m_pCTPAccount->UpdateQryTime();	
	if(bIsLast && m_pCTPAccount) 
	{
		m_pCTPAccount->ClearbInQry();
		OFFER_INFO("OnRspQryInstrument：m_bInQry = false \n");
		//PUSH_LOG_ERROR(Logger,false,true,"OnRspQryInstrument：m_bInQry = false \n");
		if(pInstrument)
		{
			OFFER_INFO("查询合约结束\n");
			PUSH_LOG_ERROR(Logger,false,true,"查询合约结束\n");
		}
	}

	if(g_pOfferMain)
		g_pOfferMain->AddInstrument(pInstrument, bIsLast);

	PlatformStru_InstrumentInfo data;
	CopyInstrumentField(data, *pInstrument);
	CInterface_SvrTradeData::getObj().SetInstrumentInfo(data,bIsLast);

	if(bIsLast)
		CInterface_SvrTradeData::getObj().EndQryInstrument();
}
///请求查询合约手续费率响应
void CUserTraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	OFFER_INFO("start查询到合约手续费率,%x %d %s \n",m_pCTPAccount,GetCurrentThreadId(), m_sLogin.strUserID.c_str());
	//PUSH_LOG_ERROR(Logger,false,true,"start查询到合约手续费率,%x %d %s \n",m_pCTPAccount,GetCurrentThreadId(), m_sLogin.strUserID.c_str());
	if(m_pCTPAccount)
	{
		m_pCTPAccount->UpdateQryTime();
		m_pCTPAccount->ClearbInQry();
	}
	if(pInstrumentCommissionRate == NULL)
	{
		OFFER_INFO("查询到合约手续费率为无效值\n");
		//PUSH_LOG_ERROR(Logger,false,true,"查询到合约手续费率为无效值\n");
		return;
	}

	SLogin login;
	m_pCTPAccount->GetLogin(login);
	strcpy(pInstrumentCommissionRate->InvestorID,login.strUserID.c_str());

	std::string strProduct = pInstrumentCommissionRate->InstrumentID;
	CInterface_SvrTradeData::getObj().SetCommissionRate(*(PlatformStru_InstrumentCommissionRate*)pInstrumentCommissionRate);//这个函数会改变 pInstrumentCommissionRate->InstrumentID值

	

	std::vector<std::string > vecInstruments;
	if(g_pOfferMain)//如果以品种返回数据，则不查询同品种的合约手续费率
		g_pOfferMain->GetInstrumentsByProductID(strProduct, vecInstruments);
	
	if(vecInstruments.size() != 0)
	{//查询回来的strProduct是品种
		BrokerAccountKey BAKey;
		strcpy(BAKey.BrokerID,  login.strBrokerID.c_str());
		strcpy(BAKey.AccountID, login.strUserID.c_str());
		m_pCTPAccount->InsertCommissionProductID_Main(BAKey, strProduct);		
	}

	{
		OFFER_INFO("end查询到合约手续费率：,%x %d %s %s\n",m_pCTPAccount,GetCurrentThreadId(), m_sLogin.strUserID.c_str(), pInstrumentCommissionRate->InstrumentID);
		//PUSH_LOG_ERROR(Logger,false,true,"查询到合约手续费率：,%x %d %s %s\n",m_pCTPAccount,GetCurrentThreadId(), m_sLogin.strUserID.c_str(), pInstrumentCommissionRate->InstrumentID);
/*
		std::string str = pInstrumentCommissionRate->InstrumentID;
		char buf[256];
		sprintf(buf,"返回  手续费率： %s, %s\n", m_sLogin.strUserID.c_str(), str.c_str());
		OutputDebugString(buf);	
*/	}
}
void CUserTraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if(m_pCTPAccount)
	{
		m_pCTPAccount->UpdateQryTime();
		m_pCTPAccount->ClearbInQry();
	}
    if(pInstrumentMarginRate == NULL)
	{
		OFFER_INFO("start查询到OnRspQryInstrumentMarginRate为无效值,%x\n", m_pCTPAccount);
		//PUSH_LOG_ERROR(Logger,false,true,"start查询到OnRspQryInstrumentMarginRate为无效值,%x\n", m_pCTPAccount);
		return;
	}
	if(bIsLast && m_pCTPAccount) 
	{		
		OFFER_INFO("OnRspQryInstrumentMarginRate [%s]：m_bInQry = false \n", pInstrumentMarginRate->InstrumentID);
		//PUSH_LOG_ERROR(Logger,false,true,"OnRspQryInstrumentMarginRate [%s]：m_bInQry = false \n", pInstrumentMarginRate->InstrumentID);
	}

	if(pInstrumentMarginRate)
		strcpy(pInstrumentMarginRate->InvestorID, m_sLogin.strUserID.c_str());

	char szHedgeFlag[2];
	memset(szHedgeFlag,0,sizeof(szHedgeFlag));
	szHedgeFlag[0] = pInstrumentMarginRate->HedgeFlag;
	std::string strHedgeFlag = szHedgeFlag;
		
		
	CInterface_SvrTradeData::getObj().SetMarginRate(*(PlatformStru_InstrumentMarginRate*)pInstrumentMarginRate);
	m_pCTPAccount->FinishedQueryInstrumentMargin(pInstrumentMarginRate->InstrumentID, strHedgeFlag);

	std::string str = pInstrumentMarginRate->InstrumentID;
	OFFER_INFO("查询到合约保证金率：%s, %s ,%c\n", m_sLogin.strUserID.c_str(), str.c_str(), pInstrumentMarginRate->HedgeFlag);
	//PUSH_LOG_ERROR(Logger,false,true,"查询到合约保证金率：%s, %s ,%c\n", m_sLogin.strUserID.c_str(), str.c_str(), pInstrumentMarginRate->HedgeFlag);
/*
	std::string str = pInstrumentMarginRate->InstrumentID;
	char buf[256];
	sprintf(buf,"返回 保证金率： %s, %s ,%c\n", m_sLogin.strUserID.c_str(), str.c_str(), pInstrumentMarginRate->HedgeFlag);
	OutputDebugString(buf);*/
}
bool CUserTraderSpi::GetInputOrderKeyByAccountID(BrokerAccountKey BAKey, STransfer& keyCtp, STransfer& keyClient)
{
	CCTPAccount* pCtpAccount = NULL;
	if(g_pOfferMain)
		g_pOfferMain->GetCTPAccount(BAKey, pCtpAccount);
	if(pCtpAccount == NULL)
		return false;//一般不会有这种情况
	
	if(!pCtpAccount->GetCTP2CleintKeyTransfer(keyCtp, keyClient))
		return false;//没有下单的信息

	return true;
}
bool CUserTraderSpi::GetInputOrderKeyByAccountID_ByOrderRef(BrokerAccountKey BAKey, SOrderRef& nCtpOrderRef, STransfer& nClientOrderRef)
{
	CCTPAccount* pCtpAccount = NULL;
	if(g_pOfferMain)
		g_pOfferMain->GetCTPAccount(BAKey, pCtpAccount);
	if(pCtpAccount == NULL)
		return false;//一般不会有这种情况

	if(!pCtpAccount->GetClientOrderRef(nCtpOrderRef, nClientOrderRef))
		return false;//没有下单的信息

	return true;
}
///请求查询报单响应
void CUserTraderSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
#ifdef  SIMU//模拟交易传报单，用于挂单成交
	CInterface_SvrSimulateTrade::getObj().PushOrder(*pOrder);
#endif

	SLogin login;
	m_pCTPAccount->GetLogin(login);

	m_pCTPAccount->UpdateQryTime();
	if(bIsLast && m_pCTPAccount) 
		m_pCTPAccount->ClearbInQry();
	if(pOrder == NULL)
	{//这种情况不知道怎么回事
		OFFER_INFO("请求查询报单出现无效值 %x, acountid = [%s]\n",  m_pCTPAccount, login.strUserID.c_str());
		//PUSH_LOG_ERROR(Logger,false,true,"请求查询报单出现无效值 %x, acountid = [%s]\n",  m_pCTPAccount, login.strUserID.c_str());
		CInterface_SvrTradeData::getObj().EndUserQryOrder(login.strBrokerID, login.strUserID);
		return;
	}
	
	int nErrID = 0;
	if(pRspInfo)
		nErrID = pRspInfo->ErrorID;
	PlatformStru_OrderInfo data(false);
	CopyOrderField(data, *pOrder);	
	
	//原始数据交由交易数据管理存储
	CInterface_SvrTradeData::getObj().QryCTPOrder(data, nErrID, bIsLast);

	std::string strAccountID = login.strUserID;

	STransfer keyCtp,keyClient;
	keyCtp.key.nFrontID		= pOrder->FrontID;
	keyCtp.key.nSessionID	= pOrder->SessionID;
	sprintf(keyCtp.key.szOrderRef, "%d", atoi(pOrder->OrderRef));
	keyCtp.strBrokerID = login.strBrokerID;
	keyCtp.strID = strAccountID;
	keyCtp.ForceCloseReason = '0';
	keyCtp.UserForceClose   = 0;
	
	BrokerAccountKey BAKey;
	strcpy(BAKey.AccountID, strAccountID.c_str());
	strcpy(BAKey.BrokerID, pOrder->BrokerID);
	if(!GetInputOrderKeyByAccountID(BAKey, keyCtp, keyClient))
	{//没有得到 orderref相关的客户端key信息
		OFFER_INFO("请求查询报单响应转换失败： AccountID:%s, nFrontID:%d, nSessionID:%d, orderRef:%s",strAccountID.c_str(), pOrder->FrontID, pOrder->SessionID, pOrder->OrderRef);
		//PUSH_LOG_ERROR(FirstLevelError,true,true,"请求查询报单响应转换失败： AccountID:%s, nFrontID:%d, nSessionID:%d, orderRef:%s",strAccountID.c_str(), pOrder->FrontID, pOrder->SessionID, pOrder->OrderRef);
		if(bIsLast)
		{
			CInterface_SvrTradeData::getObj().QryOrderReach(data, -100, bIsLast);
			CInterface_SvrTradeData::getObj().EndUserQryOrder(pOrder->BrokerID, pOrder->InvestorID);
		}
		return;
	}
	data.SessionID = keyClient.key.nSessionID;
	data.FrontID   = keyClient.key.nFrontID;
	strcpy(data.OrderRef, keyClient.key.szOrderRef); 
	strcpy(data.InvestorID, keyClient.strID.c_str());
	strcpy(data.Account, keyClient.strID.c_str());
	data.ForceCloseReason = keyClient.ForceCloseReason;
	data.UserForceClose	  = keyClient.UserForceClose;

	
//	OFFER_INFO("报盘接收报单回报： OrderSysID=[%s], nFrontID = [%d], nSessionID = [%d], orderRef = [%s], StatusMsg = [%s]", data.OrderSysID,data.FrontID, data.SessionID, data.OrderRef, data.StatusMsg);

	CInterface_SvrTradeData::getObj().QryOrderReach(data, nErrID, bIsLast);

	if(bIsLast)
	{		
		OFFER_INFO("请求查询报单结束 acountid = [%s]\n", pOrder->InvestorID);
		PUSH_LOG_ERROR(Logger,false,true,"请求查询报单结束 acountid = [%s]\n", pOrder->InvestorID);
		CInterface_SvrTradeData::getObj().EndUserQryOrder(pOrder->BrokerID, pOrder->InvestorID);
	}
	
}
///报单回报
void CUserTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	SLogin login;
	m_pCTPAccount->GetLogin(login);


	PlatformStru_OrderInfo data(false);
	CopyOrderField(data, *pOrder);	
	//原始数据交由交易数据管理存储
	CInterface_SvrTradeData::getObj().PushCTPOrder(data);

	std::string strAccountID = login.strUserID;

	STransfer keyCtp,keyClient;
	keyCtp.key.nFrontID		= pOrder->FrontID;
	keyCtp.key.nSessionID	= pOrder->SessionID;
	sprintf(keyCtp.key.szOrderRef, "%d", atoi(pOrder->OrderRef));
	keyCtp.strBrokerID = login.strBrokerID;
	keyCtp.strID = strAccountID;
	keyCtp.ForceCloseReason = '0';
	keyCtp.UserForceClose   = 0;

	BrokerAccountKey BAKey(pOrder->BrokerID, pOrder->InvestorID);
	if(!GetInputOrderKeyByAccountID(BAKey, keyCtp, keyClient))
	{//没有得到 orderref相关的客户端key信息		
		OFFER_INFO("报单回报转换失败： AccountID:%s, nFrontID:%d, nSessionID:%d, orderRef:%s\n",pOrder->InvestorID, pOrder->FrontID, pOrder->SessionID, pOrder->OrderRef);
		//PUSH_LOG_ERROR(FirstLevelError,true,true,"报单回报转换失败： AccountID:%s, nFrontID:%d, nSessionID:%d, orderRef:%s\n",pOrder->InvestorID, pOrder->FrontID, pOrder->SessionID, pOrder->OrderRef);
		return;
	}
	data.SessionID = keyClient.key.nSessionID;
	data.FrontID   = keyClient.key.nFrontID;
	strcpy(data.OrderRef, keyClient.key.szOrderRef); 
	strcpy(data.InvestorID, keyClient.strID.c_str());
	strcpy(data.Account, keyClient.strID.c_str());
	data.ForceCloseReason = keyClient.ForceCloseReason;
	data.UserForceClose	  = keyClient.UserForceClose;


	if(strcmp(pOrder->OrderSysID, "")!=0)
	{
		SOrderTransfer sOrderTransfer;
		strcpy(sOrderTransfer.szAccountID, strAccountID.c_str());
		sOrderTransfer.nFrontID = keyClient.key.nFrontID;
		sOrderTransfer.nSessionID = keyClient.key.nSessionID;
		sOrderTransfer.nOrderRef = atoi(keyClient.key.szOrderRef);

		strcpy(sOrderTransfer.orderSysID, pOrder->OrderSysID);
		strcpy(sOrderTransfer.ExchangeID, pOrder->ExchangeID);
	//	CInterface_SvrDBOpr::getObj().UpdateOrderTransfer(sOrderTransfer);
		CSTransferMsgQueue::Get()->AddTransferMsg(sOrderTransfer, 0);

		SOrderSysIDRef orderSysIDRef;
		orderSysIDRef.strExchanggeID = pOrder->ExchangeID;
		orderSysIDRef.strOrderSysID  = sOrderTransfer.orderSysID;
		m_pCTPAccount->Insertordersysid2ClientOrderRef(orderSysIDRef, keyClient);
	}
//	OFFER_INFO("报盘接收报单回报： OrderSysID=[%s], nFrontID = [%d], nSessionID = [%d], orderRef = [%s], StatusMsg = [%s]", data.OrderSysID,data.FrontID, data.SessionID, data.OrderRef, data.StatusMsg);
	CInterface_SvrTradeData::getObj().PushOrder(data);
}
///请求查询成交响应
void CUserTraderSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	SLogin login;
	m_pCTPAccount->GetLogin(login);

	m_pCTPAccount->UpdateQryTime();
	if(bIsLast && m_pCTPAccount) 
		m_pCTPAccount->ClearbInQry();

	if(pTrade == NULL)
	{
		OFFER_INFO("请求查询成交出现无效值 acountid = [%s]\n", login.strUserID.c_str());
		PUSH_LOG_ERROR(Logger,false,true,"请求查询成交出现无效值 acountid = [%s]\n", login.strUserID.c_str());
		SLogin login;
		m_pCTPAccount->GetLogin(login);
		CInterface_SvrTradeData::getObj().EndUserQryTrade(login.strBrokerID, login.strUserID);
		return;
	}
	int nErrID = 0;
	if(pRspInfo)
		nErrID = pRspInfo->ErrorID;

	PlatformStru_TradeInfo data;
	CopyTradeRecordField(data, *pTrade);
	//原始数据交由交易数据管理存储
	CInterface_SvrTradeData::getObj().QryCTPTrader(data, nErrID, bIsLast);

	std::string strAccountID = login.strUserID;
	int nOrderRef = atoi(pTrade->OrderRef);
/*
	SOrderRef CtpOrderRef;
	STransfer ClientOrderRef;
	CtpOrderRef.strBrokerID = login.strBrokerID;
	CtpOrderRef.strID		= strAccountID;
	CtpOrderRef.nOrderRef	= nOrderRef;

	BrokerAccountKey BAKey(pTrade->BrokerID, pTrade->InvestorID);
	if(!GetInputOrderKeyByAccountID_ByOrderRef(BAKey, CtpOrderRef, ClientOrderRef))
	{//没有得到 orderref相关的客户端key信息
		if(bIsLast)
		{
			CInterface_SvrTradeData::getObj().QryTradeReach(data, -100, bIsLast);
			CInterface_SvrTradeData::getObj().EndUserQryTrade(pTrade->BrokerID, pTrade->InvestorID);
		}
		return;
	}	
	strcpy(data.OrderRef, ClientOrderRef.key.szOrderRef);	
	strcpy(data.InvestorID, ClientOrderRef.strID.c_str());
	strcpy(data.szAccount, ClientOrderRef.strID.c_str());
*/

	SOrderSysIDRef orderSysIDRef;
	orderSysIDRef.strExchanggeID = pTrade->ExchangeID;
	orderSysIDRef.strOrderSysID  = pTrade->OrderSysID;
	STransfer ClientRef;
	if(!m_pCTPAccount->GetRefByOrderSysID(orderSysIDRef, ClientRef))
	{
		OFFER_INFO("请求查询成交响应转换失败： AccountID:%s, ExchanggeID:%s, OrderSysID:%s \n",login.strUserID.c_str(), orderSysIDRef.strExchanggeID.c_str(), orderSysIDRef.strOrderSysID.c_str());
		//PUSH_LOG_ERROR(FirstLevelError,true,true,"请求查询成交响应转换失败： AccountID:%s, ExchanggeID:%s, OrderSysID:%s \n",login.strUserID.c_str(), orderSysIDRef.strExchanggeID.c_str(), orderSysIDRef.strOrderSysID.c_str());
		if(bIsLast)
		{			
			OFFER_INFO("查询查询成交结束--保单转化失败的结束 AccountID = [%s]\n", pTrade->InvestorID);
			//PUSH_LOG_ERROR(FirstLevelError,true,true,"查询查询成交结束--保单转化失败的结束 AccountID = [%s]\n", pTrade->InvestorID);
			CInterface_SvrTradeData::getObj().QryTradeReach(data, -100, bIsLast);
			CInterface_SvrTradeData::getObj().EndUserQryTrade(pTrade->BrokerID, pTrade->InvestorID);
		}
		return ;//不是机构版下单的信息
	}
	strcpy(data.OrderRef, ClientRef.key.szOrderRef);	
	strcpy(data.InvestorID, ClientRef.strID.c_str());
	strcpy(data.Account, ClientRef.strID.c_str());
//	OFFER_INFO("请求查询成交响应转换成功： ExchanggeID:%s, OrderSysID:%s ",orderSysIDRef.strExchanggeID.c_str(), orderSysIDRef.strOrderSysID.c_str());

	CInterface_SvrTradeData::getObj().QryTradeReach(data, nErrID, bIsLast);
	if(bIsLast)
	{
		OFFER_INFO("请求查询成交结束 AccountID = [%s]\n", pTrade->InvestorID);
		PUSH_LOG_ERROR(Logger,false,true,"请求查询成交结束 AccountID = [%s]\n", pTrade->InvestorID);
		CInterface_SvrTradeData::getObj().EndUserQryTrade(pTrade->BrokerID, pTrade->InvestorID);
	}
}
///成交通知
void CUserTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	SLogin login;
	m_pCTPAccount->GetLogin(login);

	if(pTrade == NULL)
		return;

	PlatformStru_TradeInfo data;
	CopyTradeRecordField(data, *pTrade);
	//原始数据交由交易数据管理存储
	CInterface_SvrTradeData::getObj().PushCTPTrader(data);

	std::string strAccountID = login.strUserID;
	int nOrderRef = atoi(pTrade->OrderRef);
/*
	SOrderRef CtpOrderRef;
	STransfer ClientOrderRef;
	CtpOrderRef.strBrokerID = login.strBrokerID;
	CtpOrderRef.strID		= strAccountID;
	CtpOrderRef.nOrderRef	= nOrderRef;

	BrokerAccountKey BAKey(pTrade->BrokerID, pTrade->InvestorID);
	if(!GetInputOrderKeyByAccountID_ByOrderRef(BAKey, CtpOrderRef, ClientOrderRef))
	{//没有得到 orderref相关的客户端key信息
		return;
	}	
	strcpy(data.OrderRef, ClientOrderRef.key.szOrderRef);	
	strcpy(data.InvestorID, ClientOrderRef.strID.c_str());
	strcpy(data.szAccount, ClientOrderRef.strID.c_str());
*/
	SOrderSysIDRef orderSysIDRef;
	orderSysIDRef.strExchanggeID = pTrade->ExchangeID;
	orderSysIDRef.strOrderSysID  = pTrade->OrderSysID;

	STransfer ClientRef;
	if(!m_pCTPAccount->GetRefByOrderSysID(orderSysIDRef, ClientRef))
	{
		OFFER_INFO("成交通知转换失败：AccountID:%s, ExchanggeID:%s, OrderSysID:%s \n",login.strUserID.c_str(),orderSysIDRef.strExchanggeID.c_str(), orderSysIDRef.strOrderSysID.c_str());
		//PUSH_LOG_ERROR(FirstLevelError,true,true,"成交通知转换失败：AccountID:%s, ExchanggeID:%s, OrderSysID:%s \n",login.strUserID.c_str(),orderSysIDRef.strExchanggeID.c_str(), orderSysIDRef.strOrderSysID.c_str());
		return;//不是机构版下单的信息
	}

	strcpy(data.OrderRef, ClientRef.key.szOrderRef);	
	strcpy(data.InvestorID, ClientRef.strID.c_str());
	strcpy(data.Account, ClientRef.strID.c_str());
//	OFFER_INFO("成交通知转换成功： ExchanggeID:%s, OrderSysID:%s ",orderSysIDRef.strExchanggeID.c_str(), orderSysIDRef.strOrderSysID.c_str());
	CInterface_SvrTradeData::getObj().PushTrade(data);		
};
// 报单录入应答
void CUserTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{	
	SLogin login;
	m_pCTPAccount->GetLogin(login);

	std::string strAccountID = pInputOrder->InvestorID;
	int nOrderRef = atoi(pInputOrder->OrderRef);
	
	SOrderRef CtpOrderRef;
	STransfer ClientOrderRef;
	CtpOrderRef.strBrokerID = login.strBrokerID;
	CtpOrderRef.strID		= strAccountID;
	CtpOrderRef.nOrderRef	= nOrderRef;

	BrokerAccountKey BAKey(pInputOrder->BrokerID, pInputOrder->InvestorID);
	if(!GetInputOrderKeyByAccountID_ByOrderRef(BAKey, CtpOrderRef, ClientOrderRef))
	{//没有得到 orderref相关的客户端key信息		
		OFFER_INFO("报单录入应答转换失败： AccountID:%s, nOrderRef:%d \n",strAccountID.c_str(), nOrderRef);
		//PUSH_LOG_ERROR(FirstLevelError,true,true,"报单录入应答转换失败： AccountID:%s, nOrderRef:%d \n",strAccountID.c_str(), nOrderRef);
		return;
	}	
	strcpy(pInputOrder->OrderRef, ClientOrderRef.key.szOrderRef);	
	strcpy(pInputOrder->InvestorID, ClientOrderRef.strID.c_str());
	pInputOrder->ForceCloseReason = ClientOrderRef.ForceCloseReason;
	pInputOrder->UserForceClose   = ClientOrderRef.UserForceClose;
//	OFFER_INFO("报单录入应答转换成功： AccountID:%s, nOrderRef:%d ",strAccountID.c_str(), nOrderRef);
	CInterface_SvrTradeData::getObj().PushRspOrderInsert(ClientOrderRef.key.nFrontID, ClientOrderRef.key.nSessionID, pInputOrder, pRspInfo, nRequestID, bIsLast);

	
}
///报单录入错误回报
void CUserTraderSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	SLogin login;
	m_pCTPAccount->GetLogin(login);
	
	std::string strAccountID = pInputOrder->InvestorID;
	int nOrderRef = atoi(pInputOrder->OrderRef);
	SOrderRef CtpOrderRef;
	STransfer ClientOrderRef;
	CtpOrderRef.strBrokerID = login.strBrokerID;
	CtpOrderRef.strID		= strAccountID;
	CtpOrderRef.nOrderRef	= nOrderRef;

	BrokerAccountKey BAKey(pInputOrder->BrokerID, pInputOrder->InvestorID);
	if(!GetInputOrderKeyByAccountID_ByOrderRef(BAKey, CtpOrderRef, ClientOrderRef))
	{//没有得到 orderref相关的客户端key信息
		OFFER_INFO("报单录入错误回报转换失败： AccountID:%s, nOrderRef:%d \n",pInputOrder->InvestorID, nOrderRef);
		//PUSH_LOG_ERROR(FirstLevelError,true,true,"报单录入错误回报转换失败： AccountID:%s, nOrderRef:%d \n",pInputOrder->InvestorID, nOrderRef);
		return;
	}	
	strcpy(pInputOrder->OrderRef, ClientOrderRef.key.szOrderRef);	
	strcpy(pInputOrder->InvestorID, ClientOrderRef.strID.c_str());
	pInputOrder->ForceCloseReason = ClientOrderRef.ForceCloseReason;
	pInputOrder->UserForceClose   = ClientOrderRef.UserForceClose;

//	OFFER_INFO("报单录入错误回报转换成功： AccountID:%s, nOrderRef:%d ",strAccountID.c_str(), nOrderRef);
	CInterface_SvrTradeData::getObj().PushErrRtnOrderInsert(ClientOrderRef.key.nFrontID, ClientOrderRef.key.nSessionID, pInputOrder, pRspInfo);
}
///报单操作请求响应
void CUserTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	SLogin login;
	m_pCTPAccount->GetLogin(login);

	if(pInputOrderAction == NULL)
		return;
	std::string strAccountID = pInputOrderAction->InvestorID;

	STransfer keyCtp,keyClient;
	keyCtp.key.nFrontID		= pInputOrderAction->FrontID;
	keyCtp.key.nSessionID	= pInputOrderAction->SessionID;
//	strcpy(keyCtp.key.szOrderRef, pInputOrderAction->OrderRef);
	sprintf(keyCtp.key.szOrderRef, "%d", atoi(pInputOrderAction->OrderRef));
	keyCtp.strBrokerID = login.strBrokerID;
	keyCtp.strID = strAccountID;
	keyCtp.ForceCloseReason = '0';
	keyCtp.UserForceClose   = 0;

	BrokerAccountKey BAKey(pInputOrderAction->BrokerID, pInputOrderAction->InvestorID);
	if(!GetInputOrderKeyByAccountID(BAKey, keyCtp, keyClient))
	{//没有得到 orderref相关的客户端key信息
		OFFER_INFO("报单操作请求响应转换失败： AccountID:%s, FrontID:%d, SessionID:%d, nOrderRef:%d,  Accountid = %s\n", pInputOrderAction->InvestorID, pInputOrderAction->FrontID, pInputOrderAction->SessionID, pInputOrderAction->OrderRef, strAccountID.c_str());
		//PUSH_LOG_ERROR(FirstLevelError,true,true,"报单操作请求响应转换失败： AccountID:%s, FrontID:%d, SessionID:%d, nOrderRef:%d,  Accountid = %s\n", pInputOrderAction->InvestorID, pInputOrderAction->FrontID, pInputOrderAction->SessionID, pInputOrderAction->OrderRef, strAccountID.c_str());
		return;
	}
	pInputOrderAction->SessionID = keyClient.key.nSessionID;
	pInputOrderAction->FrontID   = keyClient.key.nFrontID;
	sprintf(pInputOrderAction->OrderRef, "%s", keyClient.key.szOrderRef); 
	strcpy(pInputOrderAction->InvestorID, keyClient.strID.c_str());	


//	OFFER_INFO("报单操作请求响应转换成功： FrontID:%d, SessionID:%d, nOrderRef:%d,  Accountid = %s",pInputOrderAction->FrontID, pInputOrderAction->SessionID, pInputOrderAction->OrderRef, strAccountID.c_str());


	CInterface_SvrTradeData::getObj().PushRspOrderAction(pInputOrderAction, pRspInfo,nRequestID, bIsLast);

}
///报单操作错误回报
void CUserTraderSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	SLogin login;
	m_pCTPAccount->GetLogin(login);
	if(pOrderAction == NULL)
		return;

	std::string strAccountID = login.strUserID;

	STransfer keyCtp,keyClient;
	keyCtp.key.nFrontID		= pOrderAction->FrontID;
	keyCtp.key.nSessionID	= pOrderAction->SessionID;
	sprintf(keyCtp.key.szOrderRef, "%d", atoi(pOrderAction->OrderRef));
	keyCtp.strBrokerID = login.strBrokerID;
	keyCtp.strID = strAccountID;
	keyCtp.ForceCloseReason = '0';
	keyCtp.UserForceClose   = 0;

	BrokerAccountKey BAKey(pOrderAction->BrokerID, pOrderAction->InvestorID);
	if(!GetInputOrderKeyByAccountID(BAKey, keyCtp, keyClient))
	{//没有得到 orderref相关的客户端key信息
		OFFER_INFO("报单操作请求响应转换失败： AccountID:%s, FrontID:%d, SessionID:%d, nOrderRef:%d,  Accountid = %s\n", pOrderAction->InvestorID,pOrderAction->FrontID, pOrderAction->SessionID, pOrderAction->OrderRef, strAccountID.c_str());
		//PUSH_LOG_ERROR(FirstLevelError,true,true,"报单操作请求响应转换失败： AccountID:%s, FrontID:%d, SessionID:%d, nOrderRef:%d,  Accountid = %s\n", pOrderAction->InvestorID,pOrderAction->FrontID, pOrderAction->SessionID, pOrderAction->OrderRef, strAccountID.c_str());
		return;
	}
	pOrderAction->SessionID = keyClient.key.nSessionID;
	pOrderAction->FrontID   = keyClient.key.nFrontID;
	sprintf(pOrderAction->OrderRef, "%s", keyClient.key.szOrderRef); 
	strcpy(pOrderAction->InvestorID, keyClient.strID.c_str());		

	//OFFER_INFO("报单操作请求响应转换成功：AccountID:%s, FrontID:%d, SessionID:%d, nOrderRef:%d,  Accountid = %s", pOrderAction->InvestorID,pOrderAction->FrontID, pOrderAction->SessionID, pOrderAction->OrderRef, strAccountID.c_str());

	CInterface_SvrTradeData::getObj().PushErrRtnOrderAction(pOrderAction, pRspInfo);
}
///请求查询资金账户响应
void CUserTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	SLogin login;
	m_pCTPAccount->GetLogin(login);
	m_pCTPAccount->UpdateQryTime();
	if(bIsLast && m_pCTPAccount) 
		m_pCTPAccount->ClearbInQry();
	if(pTradingAccount == NULL)
	{//这种情况不知道怎么回事
		OFFER_INFO("请求查询资金出现无效值 %x, acountid = [%s]\n",  m_pCTPAccount, login.strUserID.c_str());		
		return;
	}
	PlatformStru_TradingAccountInfo TradingAccountField;
	TradingAccountField.clear();
	strcpy(TradingAccountField.AccountID, pTradingAccount->AccountID);
	strcpy(TradingAccountField.BrokerID, pTradingAccount->BrokerID);
	TradingAccountField.PreMortgage=pTradingAccount->PreMortgage;
	TradingAccountField.PreCredit=pTradingAccount->PreCredit;
	TradingAccountField.PreDeposit=pTradingAccount->PreDeposit;
	TradingAccountField.PreBalance=pTradingAccount->PreBalance;
	TradingAccountField.PreMargin=pTradingAccount->PreMargin;
	TradingAccountField.InterestBase=pTradingAccount->InterestBase;
	TradingAccountField.Interest=pTradingAccount->Interest;
	TradingAccountField.Deposit=pTradingAccount->Deposit;

	TradingAccountField.Withdraw=pTradingAccount->Withdraw;
	TradingAccountField.FrozenMargin=pTradingAccount->FrozenMargin;
	TradingAccountField.FrozenCommission=pTradingAccount->FrozenCommission;
	TradingAccountField.CurrMargin=pTradingAccount->CurrMargin;
	TradingAccountField.CashIn=pTradingAccount->CashIn;
	TradingAccountField.Commission=pTradingAccount->Commission;
	TradingAccountField.CloseProfit=pTradingAccount->CloseProfit;

	TradingAccountField.PositionProfit=pTradingAccount->PositionProfit;
	TradingAccountField.Balance=pTradingAccount->Balance;
	TradingAccountField.Available=pTradingAccount->Available;
	TradingAccountField.WithdrawQuota=pTradingAccount->WithdrawQuota;
	TradingAccountField.Reserve=pTradingAccount->Reserve;
	strcpy(TradingAccountField.TradingDay, pTradingAccount->TradingDay);
	TradingAccountField.SettlementID=pTradingAccount->SettlementID;
	TradingAccountField.Credit=pTradingAccount->Credit;
	TradingAccountField.Mortgage=pTradingAccount->Mortgage;
	TradingAccountField.ExchangeMargin=pTradingAccount->ExchangeMargin;
	TradingAccountField.DeliveryMargin=pTradingAccount->DeliveryMargin;
	TradingAccountField.ExchangeDeliveryMargin=pTradingAccount->ExchangeDeliveryMargin;
	CInterface_SvrTradeData::getObj().QryFundReach(TradingAccountField);
}
void CUserTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	SLogin login;
	m_pCTPAccount->GetLogin(login);
	m_pCTPAccount->UpdateQryTime();
	if(bIsLast && m_pCTPAccount) 
	{
		m_pCTPAccount->ClearbInQry();
	//	CInterface_SvrTradeData::getObj().EndUserQryPosition(login.strBrokerID, login.strUserID);
	}
	
	CThostFtdcRspInfoField RspInfo;
	int nErrID = 0;
	if(pRspInfo)
	{
		nErrID = pRspInfo->ErrorID;
		RspInfo = *pRspInfo;
	}
	PlatformStru_Position InvestorPositionField;
		

	TThostFtdcInstrumentIDType	QryInstrumentID;
	memset(&QryInstrumentID,0,sizeof(QryInstrumentID));
	if(!pInvestorPosition)
	{
		InvestorPositionField.Clear();
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{    		
		InvestorPositionField = *pInvestorPosition;
	}
	CInterface_SvrTradeData::getObj().QryPositionReach(InvestorPositionField, nErrID, bIsLast);
	if(bIsLast ) 
	{		
		CInterface_SvrTradeData::getObj().EndUserQryPosition(login.strBrokerID, login.strUserID);
	}
}
void CUserTraderSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	SLogin login;
	m_pCTPAccount->GetLogin(login);
	m_pCTPAccount->UpdateQryTime();
	if(bIsLast && m_pCTPAccount) 
	{
		m_pCTPAccount->ClearbInQry();
		//CInterface_SvrTradeData::getObj().EndUserQryPositionDetail(login.strBrokerID, login.strUserID);
	}
	PlatformStru_PositionDetail InvestorPositionDetailField;
	CThostFtdcRspInfoField RspInfo;
	int nErrID = 0;
	if(pRspInfo)
	{
		nErrID = pRspInfo->ErrorID;
		RspInfo = *pRspInfo;
	}
	TThostFtdcInstrumentIDType	QryInstrumentID;
	memset(&QryInstrumentID,0,sizeof(QryInstrumentID));
	if(!pInvestorPositionDetail)
	{
		InvestorPositionDetailField.Clear();
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{		
		InvestorPositionDetailField = *pInvestorPositionDetail;
	}
	CInterface_SvrTradeData::getObj().QryPositionDetailReach(InvestorPositionDetailField, nErrID, bIsLast);
	if(bIsLast ) 
	{	
		CInterface_SvrTradeData::getObj().EndUserQryPositionDetail(login.strBrokerID, login.strUserID);
	}
}































































void CUserTraderSpi::CopyInstrumentField( PlatformStru_InstrumentInfo& data, const CThostFtdcInstrumentField& instrumentField )
{
	strcpy(data.InstrumentID, instrumentField.InstrumentID);
	strcpy(data.ExchangeID, instrumentField.ExchangeID);
	strcpy(data.InstrumentName, instrumentField.InstrumentName);
	strcpy(data.ExchangeInstID, instrumentField.ExchangeInstID);
	strcpy(data.ProductID, instrumentField.ProductID);
	data.ProductClass = instrumentField.ProductClass;
	data.DeliveryYear = instrumentField.DeliveryYear;
	data.DeliveryMonth = instrumentField.DeliveryMonth;
	data.MaxMarketOrderVolume = instrumentField.MaxMarketOrderVolume;
	data.MinMarketOrderVolume = instrumentField.MinMarketOrderVolume;
	data.MaxLimitOrderVolume = instrumentField.MaxLimitOrderVolume;
	data.MinLimitOrderVolume = instrumentField.MinLimitOrderVolume;
	data.VolumeMultiple = instrumentField.VolumeMultiple;
	data.PriceTick = instrumentField.PriceTick;
	strcpy(data.CreateDate,instrumentField.CreateDate);
	strcpy(data.OpenDate,instrumentField.OpenDate);
	strcpy(data.ExpireDate, instrumentField.ExpireDate);
	strcpy(data.StartDelivDate, instrumentField.StartDelivDate);
	strcpy(data.EndDelivDate, instrumentField.EndDelivDate);
	data.InstLifePhase = instrumentField.InstLifePhase;
	data.IsTrading = instrumentField.IsTrading;
	data.PositionType = instrumentField.PositionType;
	data.PositionDateType = instrumentField.PositionType;
	data.LongMarginRatio = instrumentField.LongMarginRatio;
	data.ShortMarginRatio = instrumentField.ShortMarginRatio;
}


void CUserTraderSpi::CopyPositionField( PlatformStru_Position&data, const CThostFtdcInvestorPositionField& positionField )
{
	strncpy(data.InstrumentID,positionField.InstrumentID,sizeof(data.InstrumentID)-1);
	strncpy(data.BrokerID,positionField.BrokerID,sizeof(data.BrokerID)-1);
	strncpy(data.InvestorID,positionField.InvestorID,sizeof(data.InvestorID)-1);
	data.PosiDirection=positionField.PosiDirection;
	data.HedgeFlag=positionField.HedgeFlag;
	data.PositionDate=positionField.PositionDate;
	data.YdPosition=positionField.YdPosition;
	data.Position=positionField.Position;
	data.LongFrozen=positionField.LongFrozen;
	data.ShortFrozen=positionField.ShortFrozen;
	data.LongFrozenAmount=positionField.LongFrozenAmount;
	data.ShortFrozenAmount=positionField.ShortFrozenAmount;
	data.OpenVolume=positionField.OpenVolume;
	data.CloseVolume=positionField.CloseVolume;
	data.OpenAmount=positionField.OpenAmount;
	data.CloseAmount=positionField.CloseAmount;
	data.PositionCost=positionField.PositionCost;
	data.PreMargin=positionField.PreMargin;
	data.UseMargin=positionField.UseMargin;
	data.FrozenMargin=positionField.FrozenMargin;
	data.FrozenCash=positionField.FrozenCash;
	data.FrozenCommission=positionField.FrozenCommission;
	data.CashIn=positionField.CashIn;
	data.Commission=positionField.Commission;
	data.CloseProfit=positionField.CloseProfit;
	data.PositionProfit=positionField.PositionProfit;
	data.PreSettlementPrice=positionField.PreSettlementPrice;
	data.SettlementPrice=positionField.SettlementPrice;
	strncpy(data.TradingDay,positionField.TradingDay,sizeof(data.TradingDay)-1);
	data.SettlementID=positionField.SettlementID;
	data.OpenCost=positionField.OpenCost;
	data.ExchangeMargin=positionField.ExchangeMargin;
	data.CombPosition=positionField.CombPosition;
	data.CombLongFrozen=positionField.CombLongFrozen;
	data.CombShortFrozen=positionField.CombShortFrozen;
	data.CloseProfitByDate=positionField.CloseProfitByDate;
	data.CloseProfitByTrade=positionField.CloseProfitByTrade;
	data.TodayPosition=positionField.TodayPosition;
	data.MarginRateByMoney=positionField.MarginRateByMoney;
	data.MarginRateByVolume=positionField.MarginRateByVolume;

	strncpy(data.Account,positionField.InvestorID,sizeof(data.Account)-1);
}

void CUserTraderSpi::CopyPositionDetailField( PlatformStru_PositionDetail&data, const CThostFtdcInvestorPositionDetailField& positionField )
{
	strcpy( data.InstrumentID, positionField.InstrumentID);
	strcpy( data.BrokerID, positionField.BrokerID);
	strcpy( data.InvestorID, positionField.InvestorID);
	data.HedgeFlag = positionField.HedgeFlag;
	data.Direction = positionField.Direction;
	strcpy( data.OpenDate, positionField.OpenDate);
	strcpy( data.TradeID, positionField.TradeID);
	data.Volume = positionField.Volume;
	data.OpenPrice = positionField.OpenPrice;
	strcpy( data.TradingDay, positionField.TradingDay);
	data.SettlementID = positionField.SettlementID;
	data.TradeType = positionField.TradeType;
	strcpy( data.CombInstrumentID, positionField.CombInstrumentID);
	strcpy( data.ExchangeID, positionField.ExchangeID);
	data.CloseProfitByDate = positionField.CloseProfitByDate;
	data.CloseProfitByTrade = positionField.CloseProfitByTrade;
	data.PositionProfitByDate = positionField.PositionProfitByDate;
	data.PositionProfitByTrade = positionField.PositionProfitByTrade;
	data.Margin = positionField.Margin;
	data.ExchMargin = positionField.ExchMargin;
	data.MarginRateByMoney = positionField.MarginRateByMoney;
	data.MarginRateByVolume = positionField.MarginRateByVolume;
	data.LastSettlementPrice = positionField.LastSettlementPrice;
	data.SettlementPrice = positionField.SettlementPrice;
	data.CloseVolume = positionField.CloseVolume;
	data.CloseAmount = positionField.CloseAmount;

	strncpy(data.Account,positionField.InvestorID,sizeof(data.Account)-1);
}

void CUserTraderSpi::CopyPositionCombDetailField( PlatformStru_PositionDetailComb& data, const CThostFtdcInvestorPositionCombineDetailField& positionCombField)
{
	strncpy(data.TradingDay,positionCombField.TradingDay,sizeof(data.TradingDay)-1);
	strncpy(data.OpenDate,positionCombField.OpenDate,sizeof(data.OpenDate)-1);
	strncpy(data.ExchangeID,positionCombField.ExchangeID,sizeof(data.ExchangeID)-1);
	data.SettlementID=positionCombField.SettlementID;
	strncpy(data.BrokerID,positionCombField.BrokerID,sizeof(data.BrokerID)-1);
	strncpy(data.InvestorID,positionCombField.InvestorID,sizeof(data.InvestorID)-1);
	strncpy(data.ComTradeID,positionCombField.ComTradeID,sizeof(data.ComTradeID)-1);

	strncpy(data.TradeID,positionCombField.TradeID,sizeof(data.TradeID)-1);
	strncpy(data.InstrumentID,positionCombField.InstrumentID,sizeof(data.InstrumentID)-1);
	data.HedgeFlag=positionCombField.HedgeFlag;
	data.Direction=positionCombField.Direction;
	data.TotalAmt=positionCombField.TotalAmt;
	data.Margin=positionCombField.Margin;
	data.ExchMargin=positionCombField.ExchMargin;
	data.MarginRateByMoney=positionCombField.MarginRateByMoney;
	data.MarginRateByVolume=positionCombField.MarginRateByVolume;
	data.LegID=positionCombField.LegID;
	data.LegMultiple=positionCombField.LegMultiple;
	strncpy(data.CombInstrumentID,positionCombField.CombInstrumentID,sizeof(data.CombInstrumentID)-1);

	strncpy(data.Account,positionCombField.InvestorID,sizeof(data.Account)-1);
}

//void CUserTraderSpi::CopyPositionCombDetailField2( PlatformStru_PositionCombDetail2& data, const CThostFtdcInvestorPositionCombineDetailField& positionCombField)
//{
//    strncpy(data.TradingDay,positionCombField.TradingDay,sizeof(data.TradingDay)-1);
//    strncpy(data.OpenDate,positionCombField.OpenDate,sizeof(data.OpenDate)-1);
//    strncpy(data.ExchangeID,positionCombField.ExchangeID,sizeof(data.ExchangeID)-1);
//    data.SettlementID=positionCombField.SettlementID;
//    strncpy(data.BrokerID,positionCombField.BrokerID,sizeof(data.BrokerID)-1);
//    strncpy(data.InvestorID,positionCombField.InvestorID,sizeof(data.InvestorID)-1);
//    strncpy(data.ComTradeID,positionCombField.ComTradeID,sizeof(data.ComTradeID)-1);
//
//    strncpy(data.Leg1TradeID,positionCombField.TradeID,sizeof(data.Leg1TradeID)-1);
//    strncpy(data.Leg1InstrumentID,positionCombField.InstrumentID,sizeof(data.Leg1InstrumentID)-1);
//    data.HedgeFlag=positionCombField.HedgeFlag;
//    data.Direction=positionCombField.Direction;
//    data.TotalAmt=positionCombField.TotalAmt;
//    data.Leg1Margin=positionCombField.Margin;
//    data.Leg1ExchMargin=positionCombField.ExchMargin;
//    data.Leg1MarginRateByMoney=positionCombField.MarginRateByMoney;
//    data.Leg1MarginRateByVolume=positionCombField.MarginRateByVolume;
//    data.Leg1ID=positionCombField.LegID;
//    data.Leg1Multiple=positionCombField.LegMultiple;
//    strncpy(data.InstrumentID,positionCombField.CombInstrumentID,sizeof(data.InstrumentID)-1);
//
//    strncpy(data.szAccount,positionCombField.InvestorID,sizeof(data.szAccount)-1);
//}


