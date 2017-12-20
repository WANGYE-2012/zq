// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Real_SvrTradeData.h"
#include "SvrTradeData.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include "FactoryAccountOp.h"
#include "FactoryUserOp.h"
#include "SvrTradeData.h"
#include "CommonMacro.h"
#include "SafeLock.h"
#include "SafeRwLock.h"
//#include "..\SvrNotifyAndAsk\Interface_SvrNotifyAndAsk.h"
//#include "..\SvrUserOrg\Interface_SvrUserOrg.h"
//#include "..\SvrBrokerInfo\Interface_SvrBrokerInfo.h"
//#include "..\SvrLogin\Interface_SvrLogin.h"
//#include "..\SvrTcp\Interface_SvrTcp.h"
//#include "Real_DBOpr.h"
#include "..\StrategyTester\tinyxml\tinyxml.h"
#include "MyXML.h"

#define  TRADEDATA_KEY         "CFG"
#define  TRADEDATA_CONFIG_FILE "TradeDataCfg.txt"

CReal_SvrTradeData* CReal_SvrTradeData::m_pObj=NULL;

struct  CTraderSorter
{
public:
	// Return whether first element is less than the second   
	bool operator () (const PlatformStru_TradeInfo a,const PlatformStru_TradeInfo b) const  
	{   
		return strncmp(a.TradeTime,b.TradeTime,sizeof(TThostFtdcTimeType)) < 0;   
	}; 
}TraderSorterObject;


void  BufferStoreCallback()
{
	//PostThreadMessage(g_idThread,TMESSAGE_TRADING_BUFFER_STORE,0,0);
}
CReal_SvrTradeData& CReal_SvrTradeData::getObj(void)
{
	if(!m_pObj)
		m_pObj=new CReal_SvrTradeData();
	return *m_pObj;
}

CReal_SvrTradeData::CReal_SvrTradeData()
{
	mpPlatformDataInstrument = NULL;
	mpPlatformDataQuots = NULL;

	//mpDBOpr = new CRealDBOpr();

	meTraderDataStatus = TradeDataStatusInitialing;

	mpPlatformDataInstrument = new CPlatformDataMgr_Instruments();
	mpPlatformDataQuots = new CPlatformDataMgr_Quots(*mpPlatformDataInstrument);

	meExchangeStatus = ExchangeStatus_PreOpen;
	uv__rwlock_fallback_init(&mrwLockMapAccount);
	uv__rwlock_fallback_init(&mrwLockMapUserTrading);

	mbDirty = false;

	//m_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE,"TradeData.log");

	mMasterSocket = 0;

	//mpTradingDayManager.SetNeedSaveTODB(!CInterface_SvrLogin::getObj().IsSlaver());
	mbNeedCache = true;
    mpCache = NULL;
}

CReal_SvrTradeData::~CReal_SvrTradeData()
{
	ResetAll();

	if(mpPlatformDataInstrument)
		delete mpPlatformDataInstrument;
	if(mpPlatformDataQuots)
		delete mpPlatformDataQuots;

    uv__rwlock_fallback_destroy(&mrwLockMapUserTrading);
	uv__rwlock_fallback_destroy(&mrwLockMapAccount);

	/*if(m_pWriteLog!=NULL)
	{
		delete m_pWriteLog;
		m_pWriteLog = NULL;
	}*/

}
//��ȡ�������ݹ���ģ��״̬
eExchangeStatus CReal_SvrTradeData::GetExchangeStatus() 
{
	eExchangeStatus leRet = ExchangeStatus_PreOpen;
	InterlockedExchange((volatile long*)(&leRet),meExchangeStatus);
	return leRet;
}
//��ȡ�������ݹ���ģ��״̬
eTradeDataStatus CReal_SvrTradeData::GetTraderDataStatus()
{
	eTradeDataStatus leRet = TradeDataStatusInitialing;
	InterlockedExchange((volatile long*)(&leRet),meTraderDataStatus);
	return leRet;
}



//���ý������ݹ���ģ��״̬
void CReal_SvrTradeData::SetTraderDataStatus(eTradeDataStatus neStatus) 
{
	InterlockedExchange(reinterpret_cast<volatile long*>(&meTraderDataStatus),neStatus);
}
//���ý�����״̬
void CReal_SvrTradeData::SetIntrumentsStatus(const CThostFtdcInstrumentStatusField & nStatus)
{

	//TRADE_LOG("Instrument status is %c",nStatus.InstrumentStatus);

	if(nStatus.InstrumentStatus == THOST_FTDC_IS_Continous ||
		nStatus.InstrumentStatus == THOST_FTDC_IS_AuctionOrdering ||
		nStatus.InstrumentStatus == THOST_FTDC_IS_AuctionBalance ||
		nStatus.InstrumentStatus == THOST_FTDC_IS_AuctionMatch)
	{
		TRADE_LOG("Instrument can order ");
		InterlockedExchange((volatile long*)(&meExchangeStatus),ExchangeStatus_ContinueTrade);
		
		//���һ��������
		//mpTradingDayManager.AddTradingDay(msCurrentTradingDay);
	}
	if(nStatus.InstrumentStatus == THOST_FTDC_IS_NoTrading ||
		nStatus.InstrumentStatus == THOST_FTDC_IS_Closed)
	{
		TRADE_LOG("Instrument status change to notrading");
		InterlockedExchange((volatile long*)(&meExchangeStatus),ExchangeStatus_UnableTrade);
	}

	return;
}

//�ж��Ƿ��ǽ�����
//�ַ���Ϊ���ڸ�ʽ2013-08-09
bool  CReal_SvrTradeData::IsTradingDay(const std::string & nsDay)
{
	return false/*mpTradingDayManager.IsTradingDay(nsDay)*/;
}


//�жϽ������Ƿ����
//�ַ���Ϊ���ڸ�ʽ2013-08-09
bool  CReal_SvrTradeData::IsSettlemented(const std::string & nsDay) 
{
	return false/*mpTradingDayManager.IsSetttemnted(nsDay)*/;
}


//��ȡһ�����ڵ���һ��������
bool CReal_SvrTradeData::GetLastTrading(const std::string & nsDay, std::string & nsLastDay)
{
	return false/*mpTradingDayManager.GetLastTradingData(nsDay,nsLastDay)*/;
}
//���õ�ǰ������
void CReal_SvrTradeData::SetCurrentTradingDay(const std::string & nsCurrentTradingDay,bool nbSetByMySelf) 
{
	if(GetTraderDataStatus() != TradeDataStatusInitialing && GetTraderDataStatus() != TradeDataStatusSettlemtEnd)
		return;

	if(nsCurrentTradingDay.empty())
		return;

	CSafeLock lLock(mMutexTradingday);
	if(nbSetByMySelf)
	{
		msCurrentTradingDay = nsCurrentTradingDay;
		//return;
	}
	else
	{
		std::string lsTemp = nsCurrentTradingDay.substr(0,4) + "-" + nsCurrentTradingDay.substr(4,2) + "-" + nsCurrentTradingDay.substr(6,2);

		if(msCurrentTradingDay == lsTemp)
			return;

		msCurrentTradingDay = lsTemp;
	}
	
	
	//���һ��������
	//mpTradingDayManager.AddTradingDay(msCurrentTradingDay);

	//���ú�Լ����Ľ�����
	ExchangeTime lTime;
	lTime.CurTickCount = GetTickCount();
	std::string lsTempTradingDay = nsCurrentTradingDay.substr(0,4) + nsCurrentTradingDay.substr(5,2) + nsCurrentTradingDay.substr(8,2);
	//std::string lsTempTradingDay = nsCurrentTradingDay;
	strcpy(lTime.TradingDay,lsTempTradingDay.c_str());
	mpPlatformDataInstrument->SetExchangeTime(lTime);

	//�����ȡ�����պ�Ľ��׳�ʼ��
	//PostThreadMessage(g_idThread,TMESSAGE_TRADEINIT_AFTER_GET_TRADINGDAY,0,0);
	
}

//��ȡ��ǰ������
void CReal_SvrTradeData::GetCurrentTradingDay(std::string & nsCurrentTradingDay) 
{
	CSafeLock lLock(mMutexTradingday);
	nsCurrentTradingDay = msCurrentTradingDay;
}

//��½�ͳ�ʼ��һ��ί�н����˺�
CF_ERROR CReal_SvrTradeData::LoginAdnInitTraderAccount(const BrokerAccountKey & nKey,int nAccountID)
{
	CSafeRwLockWrite lLock(mrwLockMapAccount);
	_MapAccountData::iterator lIterAccount = mMapAccountData.find(nKey);
	if(lIterAccount == mMapAccountData.end())
	{		
		//����һ��
		IInterfaceAccountOp * lpTemp =  IFactoryAccountOp::GetFactory()->CreateAccountOp();
		lpTemp->SetPlatFormInstrument(mpPlatformDataInstrument);
		lpTemp->SetPlatFormQutos(mpPlatformDataQuots);
		mMapAccountData[nKey] = lpTemp;
	}


	if(mMapAccountData[nKey]->GetCommissonMarginReady() == CommissionMargin_ComissionAndMarinReady)
	{
		TRADE_LOG("Has Found %s commission,we need check ctp status ",nKey.AccountID);

		double ldbTime ;
		if(mMapAccountData[nKey]->GetAccountCTPStatus(ldbTime) != ACCOUNT_STATUS_Login)
		{
			TRADE_LOG(" %s ctp status is not login,we need relogin ",nKey.AccountID);
			//֪ͨ��½��ѯ����
			Stru_NotifyEvent lDataEvent(EventTypeNeedSingleLoginTrade,0);		
			lDataEvent.AddBufer((void*)&nAccountID,sizeof(int));	
			//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);

		}
		return CF_ERROR_SUCCESS;
	}
	TRADE_LOG("Has not Found %s commission,notify offer to login ",nKey.AccountID);

	//֪ͨ��½��ѯ����
	Stru_NotifyEvent lDataEvent(EventTypeNeedSingleLoginTrade,0);		
	lDataEvent.AddBufer((void*)&nAccountID,sizeof(int));	
	//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);

	//����һ����Ҫ��ѯ��Լ���ʵ��¼�
	Stru_NotifyEvent lDataEvent1(EventTypeNeedQryCommission,0);		
	lDataEvent1.AddBufer((void*)&nKey,sizeof(nKey));	
	//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent1);

	//����һ����Ҫ��ѯ��֤���ʵ��¼�
	Stru_NotifyEvent lDataEvent2(EventTypeNeedQryMargin,0);	
	lDataEvent2.AddBufer((void*)&nKey,sizeof(nKey));	
	//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent2);
	return CF_ERROR_SUCCESS;
}
//���̿��Թ�����
void CReal_SvrTradeData::TradeDataInit(CMemCache* pCache,string szUser,string szBrokerCode,string szTradeAccount,bool nbForce ) 
{
	//mbForceInit = nbForce;
 //   mpCache = pCache;

	//TRADE_LOG("TradeDataInit with %s",mbForceInit ? "force" : "noforce");

 //   //�����ڴ�
	//ResetAll();

	//msCurrentTradingDay.clear();
	//msInstrumentDay.clear();

	////Ϊÿ���û���һ���յ��û�����
	//InitUserMapWithEmpty(szUser,szBrokerCode,szTradeAccount,mMapUser2TradingData);  
}

//��������ÿ���˻�������CTP��״̬
void CReal_SvrTradeData::SetAccountStatus(const std::string & nsAccoutnID,
										  const std::string & nsBrokerID,
							  const EnumAccountStatus& neUserStatu) 
{

	//if(GetTraderDataStatus() != TradeDataStatusInitialing 
	//	&& GetTraderDataStatus() != TradeDataStatusSettlemtIniting)
	//{
	//	TRADE_LOG("not Init Status,discard this call");
	//	return ;
	//}
	CSafeRwLockRead lrlockAccount(mrwLockMapAccount);
	BrokerAccountKey lKey(nsBrokerID.c_str(),nsAccoutnID.c_str());
	_MapAccountData::iterator lIterAccount = mMapAccountData.find(lKey);
	if(lIterAccount != mMapAccountData.end())
	{
		lIterAccount->second->SetAccountCTPStatus(neUserStatu);
	}


	CSafeRwLockRead lrlock(mrwLockMapUserTrading);

	vector<UserInfo> lVecUser;
// 	CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(nsAccoutnID,nsBrokerID,lVecUser);	
// 	vector<UserInfo>::iterator lIter = lVecUser.begin();
// 	for(;lIter != lVecUser.end();lIter++)
// 	{	
// 		//CSafeLock ls(m_mutexMapUserTrading);
 		_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nsAccoutnID);
 		if(lIterUser != mMapUser2TradingData.end())
 			lIterUser->second->SetUserCTPStatus(neUserStatu);		
// 	}
	
}


//��ȡÿ���˻���ctp״̬
void CReal_SvrTradeData::GetAccountStatus(const std::string & nsAccoutnID,
							  const std::string & nsBrokerID,
							  EnumAccountStatus& neCTPStatus,
							  double & nStatusTime)
{
	CSafeRwLockRead lrlockAccount(mrwLockMapAccount);
	BrokerAccountKey lKey(nsBrokerID.c_str(),nsAccoutnID.c_str());
	_MapAccountData::iterator lIterAccount = mMapAccountData.find(lKey);
	if(lIterAccount != mMapAccountData.end())
	{		
		neCTPStatus = lIterAccount->second->GetAccountCTPStatus(nStatusTime);
	}
}

//��ȡ����ί�н����˺ŵ�״̬
void CReal_SvrTradeData::GetAllAccountStatus(std::vector<sAccountStatus> & nVecStatus)
{
	CSafeRwLockRead lrlockAccount(mrwLockMapAccount);	
	_MapAccountData::iterator lIterAccount = mMapAccountData.begin();
	for(;lIterAccount != mMapAccountData.end();lIterAccount++)
	{		
		sAccountStatus lsStatus;
		lsStatus.mBrokerAccount = lIterAccount->first;	
		double ldbTime = 0.0f;
		EnumAccountStatus leCTPStatus = lIterAccount->second->GetAccountCTPStatus(ldbTime);
		lsStatus.meStatus = leCTPStatus;
		lsStatus.ldbDuration = ldbTime;
		lsStatus.meCommissionAndMarginReady = lIterAccount->second->GetCommissonMarginReady();
		nVecStatus.push_back(lsStatus);
	}
}

//��ȡ�����û��ĳ�ʼ��״̬
void CReal_SvrTradeData::GetAlUserInitStatus(std::vector<sUserInitStatus> & lVecStatus)
{
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);

	_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.begin();
	for(;lIterUser != mMapUser2TradingData.end();lIterUser++)
	{
		sUserInitStatus ltemp;
		strcpy_s(ltemp.szUserAccount,lIterUser->first.c_str());
		ltemp.meStatus =  lIterUser->second->GetUserInitStatus();
		ltemp.meCTPStatus =  lIterUser->second->GetUserCTPStatus(ltemp.ldbDuration);
		lVecStatus.push_back(ltemp);
	}

}

bool CReal_SvrTradeData::IsUserSettlementConfirmed(const std::string & nsUserName,
														std::string & nsConfirmedDate,
														std::string & nsConfirmedTime) 
{
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);

	nsConfirmedDate.clear();
	nsConfirmedTime.clear();

	do 
	{		
// 		if(!msCurrentTradingDay.empty())
// 			mpDBOpr->LoadUserSettlementConfirm(msCurrentTradingDay,nsUserName,nsConfirmedDate,nsConfirmedTime);
	/*	_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nsUserName);
		if(lIterUser != mMapUser2TradingData.end())
			lbRet =  lIterUser->second->IsSettlementComfirmed(nsConfirmedDate,nsConfirmedTime);*/

		break;;

	} while (false);

	return !nsConfirmedDate.empty();
}


//�û�ȷ���˽��㵥
void CReal_SvrTradeData::SetUserSettlementConfirmed(const std::string & nsUserName,
										const std::string & nsConfirmedDate,
										const std::string & nsConfirmedTime) 
{
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
// 	if(!msCurrentTradingDay.empty())
// 		mpDBOpr->AddUserSettlementConfirm(msCurrentTradingDay,nsUserName,nsConfirmedDate,nsConfirmedTime);
	//_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nsUserName);
	//if(lIterUser != mMapUser2TradingData.end())
	//	lIterUser->second->SetSettlementConfirmed(true,nsConfirmedDate,nsConfirmedTime);	
	
}

//��ȡ�û���ctp����״̬
EnumAccountStatus  CReal_SvrTradeData::GetUserCTPStatus(int nUserID,double & nDifftime) 
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK)
	{
		TRADE_LOG("Init Status,discard this call");
		return ACCOUNT_STATUS_UnInit;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	EnumAccountStatus leRet = ACCOUNT_STATUS_UnInit;
	do 
	{
		UserInfo lUser;
// 		if(false == CInterface_SvrUserOrg::getObj().GetUserByID(nUserID,lUser))
// 			break;
// 
// 		_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(lUser.szAccount);
// 		if(lIterUser != mMapUser2TradingData.end())
// 			leRet =  lIterUser->second->GetUserCTPStatus(nDifftime);

	} while (false);

	return leRet;
	
}
//��ȡ�û���ctp����״̬
eUserInitStatus  CReal_SvrTradeData::GetUserInitStatus(int nUserID) 
{
	
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK)
	{
		TRADE_LOG("Init Status,discard this call");
		return USER_STATUS_UnInit;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	eUserInitStatus leRet = USER_STATUS_UnInit;
	do 
	{
// 		UserInfo lUser;
// 		if(false == CInterface_SvrUserOrg::getObj().GetUserByID(nUserID,lUser))
// 			break;
// 
// 		_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(lUser.szAccount);
// 		if(lIterUser != mMapUser2TradingData.end())
// 			leRet =  lIterUser->second->GetUserInitStatus();
	
	} while (false);

	return leRet;
}



//��ȡ�û���ctp����״̬
EnumAccountStatus CReal_SvrTradeData::GetUserCTPStatus(const std::string& nsUser,double & nDifftime) 
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK)
	{
		TRADE_LOG("Init Status,discard this call");
		return ACCOUNT_STATUS_UnInit;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	EnumAccountStatus leRet = ACCOUNT_STATUS_UnInit;
	do 
	{
		
		_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nsUser);
		if(lIterUser != mMapUser2TradingData.end())
			leRet =  lIterUser->second->GetUserCTPStatus(nDifftime);

	} while (false);

	return leRet;
}

//��ȡ�û��ĳ�ʼ��״̬
eUserInitStatus CReal_SvrTradeData::GetUserInitStatus(const std::string& nsUser) 
{
	
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK)
	{
		TRADE_LOG("Init Status,discard this call");
		return USER_STATUS_UnInit;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	eUserInitStatus leRet = USER_STATUS_UnInit;
	do 
	{		
		_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nsUser);
		if(lIterUser != mMapUser2TradingData.end())
			leRet =  lIterUser->second->GetUserInitStatus();

	} while (false);

	return leRet;
}

//��ȡ������
void CReal_SvrTradeData::GetExchangeData(ExchangeTime & nData) 
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing || 
		GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK||
		mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return ;
	}*/

	return;
}

///��ȡ��Լ�б�
int CReal_SvrTradeData::GetInstrumentList(std::set<std::string> &outData,
										  std::string ExchangeID)
{
	//CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}

	mpPlatformDataInstrument->GetInstrumentList(outData,ExchangeID);
	return outData.size();
}
//���������Լ�б�
void CReal_SvrTradeData::AddMainInstrumentList(std::string instrument)
{
	//CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return ;
	}
	mpPlatformDataInstrument->AddMainInstrumentList(instrument);
}
///��ȡ������Լ�б�
int CReal_SvrTradeData::GetMainInstrumentList(std::set<std::string> &outData)
{
	//CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}
	return mpPlatformDataInstrument->GetMainInstrumentList(outData);
}
///���ú�Լ�ݲ��б�
void CReal_SvrTradeData::SetInstrumentVarietyMap(map<string, string>& inData)
{
	//CSafeLock lLock(m_mutex);
	if(mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return ;
	}
	mpPlatformDataInstrument->SetInstrumentVarietyMap(inData);
}

// ��ȡ��Լ�ݲ��б�
int CReal_SvrTradeData::GetInstrumentVarietyMap(map<string, string>& outData)
{
	//CSafeLock lLock(m_mutex);
	if(mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}
	mpPlatformDataInstrument->GetInstrumentVarietyMap(outData);
	return outData.size();
}

///��ȡָ����Լ��Ϣ
int CReal_SvrTradeData::GetInstrumentInfo(const std::string& InstrumentID, 
										  PlatformStru_InstrumentInfo& outData)
{
	//CSafeLock lLock(m_mutex);
	if(mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}
	bool brlt=mpPlatformDataInstrument->GetInstrumentInfo(InstrumentID,outData);
	return brlt?0:-1;
}

///����ָ����Լ��Ϣ
void CReal_SvrTradeData::SetInstrumentInfo(PlatformStru_InstrumentInfo& nInstrumentInfo)
{
//	CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Not Trading Status,discard this call");
		return ;
	}
	TRADE_LOG("%s",nInstrumentInfo.InstrumentID);
	mpPlatformDataInstrument->SetInstrumentInfo(nInstrumentInfo.InstrumentID,nInstrumentInfo,false);
}

///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
char CReal_SvrTradeData::GetProductClassType(const std::string& InstrumentID)
{
//	CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}
	return mpPlatformDataInstrument->GetProductClassType(InstrumentID);
}

///��ȡ�����˵��Ⱥ�Լ����Ϻ�Լ�б�
bool CReal_SvrTradeData::GetCombInstrumentIDs_IncludeLeg(const std::string& LegInstrument,
														 std::vector<std::string>& vecCombInstruments)
{
//	CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}
	return mpPlatformDataInstrument->GetCombInstrumentIDs_IncludeLeg(LegInstrument,vecCombInstruments);
}

///��ȡ��ԼƷ�ִ���
int CReal_SvrTradeData::GetProductID(std::map<std::string, std::set<std::string> >& outData,
									 std::string ExchangeID)
{
//	CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}
	mpPlatformDataInstrument->GetProductID(outData,ExchangeID);
	return outData.size();
}



///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
int CReal_SvrTradeData::GetInstrumentMultiple(const std::string& InstrumentID)
{
//	CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}
	return mpPlatformDataInstrument->GetInstrumentMultiple(InstrumentID);
}

///��ȡָ��Ʒ�ֵĺ�Լ�б�,���غ�Լ����
int CReal_SvrTradeData::GetInstrumentListByProductID(const std::string& ProductID,
													 std::set<std::string> &outData)
{
//	CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}
	mpPlatformDataInstrument->GetInstrumentListByProductID(ProductID,outData);
	return outData.size();
}
//���̵��ã���ѯ��Լ���
void CReal_SvrTradeData::EndQryInstrument()
{	
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("EndQryInstrument Init Status,discard this call");
		return ;
	}
	//���ú�Լ����׼��״̬	//Ϊ�û��˻����ú�Լ
	if(DataReady_Ready != InterlockedExchange(reinterpret_cast<volatile long*>(&mbInstrumentsReady),DataReady_Ready))
	{
		SetPlatformObjectToAllAccount();
		SetPlatformObjectToAllUser(mMapUser2TradingData);	
	}
	
	//��ȡ���к�Լ
	std::vector<PlatformStru_InstrumentInfo> lVecInstruments;
	std::set<std::string> lInstrumentStringSet;
	CReal_SvrTradeData::getObj().GetInstrumentList(lInstrumentStringSet);
	TRADE_LOG("Instruments size %d",lInstrumentStringSet.size());
	std::set<std::string>::iterator lIterSet = lInstrumentStringSet.begin();
	while(lIterSet != lInstrumentStringSet.end())
	{
		PlatformStru_InstrumentInfo lTempInfo;
		if(0 == CReal_SvrTradeData::getObj().GetInstrumentInfo(*lIterSet,lTempInfo))
			lVecInstruments.push_back(lTempInfo);
		lIterSet++;
	}
	
    //д���ݿ� 
	std::string lsTempTradingDay;
	GetCurrentTradingDay(lsTempTradingDay);
	//mpDBOpr->WriteIntrument(lsTempTradingDay,lVecInstruments);

	//������Ľ����ճ�ʼ�����	
	//mpTradingDayManager.SetTradingDayInit(lsTempTradingDay);
	//PUSH_LOG_ERROR(Logger,false,true,"��Լ��ѯ���");

	//�����ں�Լ�ĳֲ�
	if(CheckInitDataHasExpiredInstruementPosition())
	{
		SetTraderDataStatus(TradeDataStatusInitFail);
		//PUSH_LOG_ERROR(Logger,false,true,"�����ճ��ֲ������й��ڵĺ�Լ�����÷�����Ϊ��ʼ��ʧ��״̬");
		//PUSH_LOG_ERROR(Logger,false,true,"-----------------��ʼ��ʧ��-----------------");
		//֪ͨ�̹߳رն�ʱ��
		//PostThreadMessage(g_idThread,TMESSAGE_TRADING_END_USER_CHECK,0,0);

		return;
	}

	//�����ѯ��֤��ͷ���
	SendQryCommissionAndMargin();
	
}

//��ȡ��Լ�����Ƿ����
eDataReady  CReal_SvrTradeData::GetInstrumentDataReady()
{	

	eDataReady lRet = DataReday_NoReady;
	InterlockedExchange(reinterpret_cast<volatile long*>(&lRet),mbInstrumentsReady);
	return lRet;

}
///��ȡָ����Լ����
int CReal_SvrTradeData::GetQuotInfo(const std::string& InstrumentID, 
									PlatformStru_DepthMarketData& outData)
{
	if(mpPlatformDataQuots == NULL)
	{
		TRADE_LOG("GetQuotInfo error Status,discard this call");
		return -1;
	}

//	CSafeLock lLock(m_mutex);
	return mpPlatformDataQuots->GetQuotInfo(InstrumentID,outData)?0:-1;
}


///��ȡָ����Լ������
int CReal_SvrTradeData::GetOldQuotInfo(const std::string& InstrumentID, 
									   PlatformStru_DepthMarketData& outData)
{
	if(mpPlatformDataQuots == NULL)
	{
		TRADE_LOG("GetOldQuotInfo error Status,discard this call");
		return -1;
	}

//	CSafeLock lLock(m_mutex);
	return mpPlatformDataQuots->GetOldQuotInfo(InstrumentID,outData)?0:-1;
}

///��ȡָ����Լ���ּۣ�ʧ�ܷ�����Чֵ
double CReal_SvrTradeData::GetCurPrice(const std::string& InstrumentID)
{
//	CSafeLock lLock(m_mutex);
	if(mpPlatformDataQuots == NULL)
	{
		TRADE_LOG("GetCurPrice error Status,discard this call");
		return util::GetDoubleInvalidValue();
	}

	return mpPlatformDataQuots->GetCurPrice(InstrumentID);
}

//����ָ����Լ���飬���õ�������£��򷵻�true�����򷵻�false����ԭ����������ϣ�
CF_ERROR CReal_SvrTradeData::SetQuotInfo( PlatformStru_DepthMarketData& inData 
									 ,bool bSettlementManul )
{	
    TRADE_LOG("�յ������������inst=%s",inData.InstrumentID);
	
	//����
	bool lbRet = mpPlatformDataQuots->SetQuotInfo(inData);
	if(lbRet)
	{

		CSafeRwLockRead lrlock(mrwLockMapUserTrading);
		//�������¼���
		_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.begin();
		while(lIterUser != mMapUser2TradingData.end())
		{
			if(lIterUser->second->CalulateByNewQuto(inData,bSettlementManul))
			{
				if(mbNeedCache)
				{
					//�����޸ĵĳֲ֣����ʽ�
					PlatformStru_TradingAccountInfo lFundInfo;
					lIterUser->second->GetFundInfo(lFundInfo);
					mpCache->CacheFund(lFundInfo);
                    TRADE_LOG("SetQuotInfo CacheFund AccountID=%s Available=%.4f DynamicProfit=%.4f StaticProfit=%.4f ",
                        lFundInfo.AccountID,lFundInfo.Available,lFundInfo.DynamicProfit,lFundInfo.StaticProfit);

					vector<PlatformStru_Position> lvscPos;
					lIterUser->second->GetPositions(inData.InstrumentID,lvscPos);
					for(int i = 0 ; i < lvscPos.size(); i++)
                    {
						mpCache->CachePosition(lvscPos[i]);
                        TRADE_LOG("SetQuotInfo CachePosition InstrumentID=%s CanCloseTodayVolume=%d CanCloseVolume=%d CanCloseydVolume=%d\
                            CloseProfit=%.4f CloseTodayOrderVolume=%d CloseOrderVolume=%d CloseVolume=%d InvestorID=%s CanCloseydVolume=%d\
                            OpenVolume=%d Position=%d TodayPosition=%d YdPosition=%d",
                            lvscPos[i].InstrumentID,lvscPos[i].CanCloseTodayVolume,lvscPos[i].CanCloseVolume,lvscPos[i].CanCloseydVolume,
                            lvscPos[i].CloseProfit,lvscPos[i].CloseTodayOrderVolume,lvscPos[i].CloseOrderVolume,lvscPos[i].CloseVolume,
                            lvscPos[i].InvestorID,lvscPos[i].OpenVolume,lvscPos[i].Position,lvscPos[i].TodayPosition,lvscPos[i].YdPosition);

                    }
				}
			}
			lIterUser++;
		}

		CSafeRwLockRead lrlockAccount(mrwLockMapAccount);
		//�������¼���
		_MapAccountData::iterator  lIterAccount = mMapAccountData.begin();
		while(lIterAccount != mMapAccountData.end())
		{
			lIterAccount->second->CalulateByNewQuto(inData,bSettlementManul);
			lIterAccount++;
		}
	}

	return CF_ERROR_SUCCESS;
}


/***************************************************************************************************************************************/

///��ȡĳ���˻���Լ��������, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
int CReal_SvrTradeData::GetCommissionRate(const string& nsBrokerID,const string& nsAccountID,
										  const string& InstrumentID, 
										  PlatformStru_InstrumentCommissionRate& outData)
{
	
	//if(GetTraderDataStatus() == TradeDataStatusInitialing
	//	|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	//{
	//	TRADE_LOG("error Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}
	CSafeRwLockRead lLock(mrwLockMapAccount);
	BrokerAccountKey lKey(nsBrokerID.c_str(),nsAccountID.c_str());

	//��ȡ״̬
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return -1;

	if(	!(lIter->second->GetCommissonMarginReady() & CommissionMargin_ComissionReady))
	{
		TRADE_LOG("Comission not ready,discard this call");
		return 0;
	}

	return  lIter->second->GetCommissionRate(InstrumentID,outData);

}
///��ȡĳ���û���Լ��������, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
//��Ϊһ���û���һ���˺ţ�����Ŀǰ���������ַ�ʽ�����ط���
int CReal_SvrTradeData::GetCommissionRate(const std::string& nsUserID,	
							  const std::string& InstrumentID, 
							  PlatformStru_InstrumentCommissionRate& outData) 
{
	
	//if(GetTraderDataStatus() == TradeDataStatusInitialing
	//	|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	//{
	//	TRADE_LOG("error Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}

	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nsUserID);
	if(mMapUser2TradingData.end() != lIter)
	{
		IInterfaceUserOp * lpTemp = lIter->second;	
		return lpTemp->GetCommissionRate(InstrumentID,outData);		
	}


	return -1;
}
///����ĳ���˻���Լ��������
void CReal_SvrTradeData::SetCommissionRate( PlatformStru_InstrumentCommissionRate& nCommissionRate)
{
	//if(GetTraderDataStatus() != TradeDataStatusInitialing &&
	//	GetTraderDataStatus() != TradeDataStatusSettlemtIniting && 
	//	GetTraderDataStatus() != TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Not Init Status,discard this call");
	//	return ;
	//}

	TRADE_LOG("SetCommissionRate BrokerID:%s AccountID %s",nCommissionRate.BrokerID,nCommissionRate.InvestorID);
// 	if(GetTraderDataStatus() == TradeDataStatusTrading && !CInterface_SvrLogin::getObj().IsSlaver())
// 	{
		//��������н���״̬��ѯ�����������͸����еĴӷ���
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a QryCommission to SLaver %d",lVecSocket[i]);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterQryCommission_Push,
// 				&nCommissionRate,
// 				sizeof(nCommissionRate),
// 				0,
// 				0,
// 				0,
// 				0,
// 				CF_ERROR_SUCCESS,0);
// 		}

// 	}

	CSafeRwLockRead lLock(mrwLockMapAccount);

	IInterfaceAccountOp * lpTemp = NULL;
	BrokerAccountKey lKey(nCommissionRate.BrokerID,nCommissionRate.InvestorID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
	{
		//TRADE_LOG("SetCommissionRate No Rate Object,discard this call");
		//return ;
		lpTemp =  IFactoryAccountOp::GetFactory()->CreateAccountOp();
		mMapAccountData[lKey] = lpTemp;
		lpTemp->SetPlatFormInstrument(mpPlatformDataInstrument);
		lpTemp->SetPlatFormQutos(mpPlatformDataQuots);
	}
	else
	{
		lpTemp = lIter->second;
	}

	std::set<std::string> InstrumentList;
	mpPlatformDataInstrument->GetInstrumentListByProductID(nCommissionRate.InstrumentID,InstrumentList);
	if((int)InstrumentList.size()> 0)
	{//��Ʒ�ִ���
		for(std::set<std::string>::iterator it2=InstrumentList.begin(); it2!=InstrumentList.end(); ++it2)
		{
			 strcpy_s(nCommissionRate.InstrumentID,sizeof(TThostFtdcInstrumentIDType),(*it2).c_str());
			 lpTemp->SetCommissionRate(*it2,nCommissionRate);
		}
	}
	else	
	  lpTemp->SetCommissionRate(nCommissionRate.InstrumentID,nCommissionRate);
}
//���̵��ã���ĳһ���û���Լ�ı�֤���ʲ�ѯ��ɺ�
void CReal_SvrTradeData::EndUserQryCommission(const std::string& nsBrokerID,
										  const std::string &nAccountID,
                                          const std::string &nUserID) 
{

	//if(GetTraderDataStatus() != TradeDataStatusInitialing &&
	//	GetTraderDataStatus() != TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Not Init Status,discard this call");
	//	return ;
	//}

	//CHECK instrument ready
	TRADE_LOG("EndUserQryCommission BrokerID:%s AccountID %s",nsBrokerID.c_str(),nAccountID.c_str());


	CSafeRwLockRead lLock(mrwLockMapAccount);
	IInterfaceAccountOp * lpRate = NULL;
	BrokerAccountKey lKey(nsBrokerID.c_str(),nAccountID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)	
	{		
		lpRate = lIter->second;
	}
	else
		return;

	//��commissionRate
	lIter->second->SetCommissionMarginReady(CommissionMargin_ComissionReady);	
	//������ʺͱ�֤���ʶ�����
// 	if(lIter->second->GetCommissonMarginReady() == CommissionMargin_ComissionAndMarinReady)
// 	{	
		//TRADE_LOG("EndUserQryCommission BrokerID:%s AccountID %s send qry trader",nsBrokerID.c_str(),nAccountID.c_str());
		//���÷��ʶ��󵽶�Ӧ���û�����
		//SetRatesObjectToAccountRelationUserObject(nsBrokerID,nAccountID,nUserID);


		////PUSH_LOG_ERROR(Logger,false,true,"���ʺͱ�֤���ʲ�ѯ��ϣ�װ��ί�н����ʺ�%s���ճ�����",nAccountID.c_str());

		////װ���ճ�������
		//DispathInitialDataToUserWithAccount(nsBrokerID,nAccountID,mMapUser2TradingData);

		////PUSH_LOG_ERROR(Logger,false,true,"����ί�н����ʺ�%s�Ĳ�ѯ�ʽ�����",nAccountID.c_str());


		////���Ͳ�ɽ�������
		//Stru_NotifyEvent lDataEvent(EventTypeNeedQryFund,0);
		//BrokerAccountKey lsBrokerAccount(nsBrokerID.c_str(),nAccountID.c_str());		
		//lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
		////CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 	}


	

	//д���ݿ�
// 	if(lpRate != NULL && !CInterface_SvrLogin::getObj().IsSlaver())	
// 	{
// 		std::vector<PlatformStru_InstrumentCommissionRate> lVecCommission;
// 		std::set<std::string> lInstrumentStringSet;
// 		CReal_SvrTradeData::getObj().GetInstrumentList(lInstrumentStringSet);
// 		std::set<std::string>::iterator lIterSet = lInstrumentStringSet.begin();
// 		while(lIterSet != lInstrumentStringSet.end())
// 		{
// 			PlatformStru_InstrumentCommissionRate lTempInfo;			
// 			if(0 == lpRate->GetCommissionRate(*lIterSet,lTempInfo))
// 				lVecCommission.push_back(lTempInfo);
// 			lIterSet++;
// 		}
// 
// 		//д���ݿ� 
// 		std::string lsTempTradingDay;
// 		GetCurrentTradingDay(lsTempTradingDay);
// 		mpDBOpr->WriteCommission(nsBrokerID,nAccountID,lsTempTradingDay,lVecCommission);
// 	}
	return;
}

///��ȡ��Լ��֤����, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
int CReal_SvrTradeData::GetMarginRate(const string& nsBrokerID,
									  const string& nsAccountID,
									  const string& InstrumentID, 
									  PlatformStru_InstrumentMarginRate& outData)
{
	
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	CSafeRwLockRead lLock(mrwLockMapAccount);
	BrokerAccountKey lKey(nsBrokerID.c_str(),nsAccountID.c_str());
	//��ȡ״̬	

	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return -1;

	if(!(lIter->second->GetCommissonMarginReady() & CommissionMargin_MarginReady))
	{
		TRADE_LOG("GetMarginRate Comission not ready,discard this call");
		return 0;
	}
	return lIter->second->GetMarginRate(InstrumentID,outData);
}


///��ȡ��Լ��֤����, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
int CReal_SvrTradeData::GetMarginRate(const string& nsUserID,	
						  const string& InstrumentID, 
						  PlatformStru_InstrumentMarginRate& outData) 
{
	

	//if(GetTraderDataStatus() == TradeDataStatusInitialing
	//	|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	//{
	//	TRADE_LOG("error Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}

	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nsUserID);
	if(mMapUser2TradingData.end() != lIter)
	{
		IInterfaceUserOp * lpTemp = lIter->second;	
		return lpTemp->GetMarginRate(InstrumentID,outData);		
	}


	return -1;
}

///���ú�Լ��֤����
void CReal_SvrTradeData::SetMarginRate( PlatformStru_InstrumentMarginRate& nMarginRate)
{
	
	//if(GetTraderDataStatus() != TradeDataStatusInitialing &&
	//	GetTraderDataStatus() != TradeDataStatusSettlemtIniting && 
	//	GetTraderDataStatus() != TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Not Init Status,discard this call");
	//	return ;
	//}
	TRADE_LOG("SetMarginRate BrokerID:%s AccountID %s",nMarginRate.BrokerID,nMarginRate.InvestorID);
// 	if(GetTraderDataStatus() == TradeDataStatusTrading && !CInterface_SvrLogin::getObj().IsSlaver())
// 	{
// 		//��������н���״̬��ѯ�����������͸����еĴӷ���
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a QryMargin to SLaver %d",lVecSocket[i]);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterQryMargin_Push,
// 				&nMarginRate,
// 				sizeof(nMarginRate),
// 				0,
// 				0,
// 				0,
// 				0,
// 				CF_ERROR_SUCCESS,0);
// 		}
// 
// 	}
	CSafeRwLockRead lLock(mrwLockMapAccount);
	IInterfaceAccountOp * lpTemp = NULL;
	BrokerAccountKey lKey(nMarginRate.BrokerID,nMarginRate.InvestorID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
	{
		TRADE_LOG("No Rate Object,discard this call");
		return ;
	}
	else
	{
		lpTemp = lIter->second;
	}

	return lpTemp->SetMarginRate(nMarginRate.InstrumentID,nMarginRate);

}
//���̵��ã���ĳһ���û���Լ�ı�֤���ʲ�ѯ��ɺ�
void CReal_SvrTradeData::EndUserQryMargin(const std::string& nsBrokerID,
													   const std::string &nAccountID) 
{
	/*if(GetTraderDataStatus() != TradeDataStatusInitialing &&
		GetTraderDataStatus() != TradeDataStatusTrading)
	{
		TRADE_LOG("Not Init Status,discard this call");
		return ;
	}*/


	TRADE_LOG("EndUserQryMargin BrokerID:%s AccountID %s",nsBrokerID.c_str(),nAccountID.c_str());
	CSafeRwLockRead lLock(mrwLockMapAccount);

	IInterfaceAccountOp * lpRate = NULL;
	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nAccountID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);	
	if(mMapAccountData.end() != lIter)	
	{		
		lpRate = lIter->second;
	}
	else
		return;
	//��commissionRate
	lIter->second->SetCommissionMarginReady(CommissionMargin_MarginReady);
	/*_MapAccountData::iterator lIterReady =  mMapAccountRateReady.find(lKey);
	if(mMapAccountRateReady.end() != lIterReady)
	{
		int lnTemp = (int)lIterReady->second;
		lnTemp |= CommissionMargin_MarginReady;
		lIterReady->second = (eCommissionMarginReady)lnTemp;
	}*/
	//������ʺͱ�֤���ʶ�����
	if(lIter->second->GetCommissonMarginReady() == CommissionMargin_ComissionAndMarinReady)
	{	
		TRADE_LOG("EndUserQryMargin BrokerID:%s AccountID %s send qry trader",nsBrokerID.c_str(),nAccountID.c_str());
		//���÷��ʶ��󵽶�Ӧ���û�����
		//SetRatesObjectToAccountRelationUserObject(nsBrokerID,nAccountID);

		

		////PUSH_LOG_ERROR(Logger,false,true,"��֤���ѯ��ϣ�װ��ί�н����ʺ�%s���ճ�����",nAccountID.c_str());

		////װ���ճ�������
		//DispathInitialDataToUserWithAccount(nsBrokerID,nAccountID,mMapUser2TradingData);


		////PUSH_LOG_ERROR(Logger,false,true,"����ί�н����ʺ�%s�Ĳ�ѯ�ʽ�����",nAccountID.c_str());

		////���Ͳ�ɽ�������
		//Stru_NotifyEvent lDataEvent(EventTypeNeedQryFund,0);
		//BrokerAccountKey lsBrokerAccount(nsBrokerID.c_str(),nAccountID.c_str());		
		//lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
		////CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
	}

	

	//д���ݿ�
	if(lpRate != NULL/* && !CInterface_SvrLogin::getObj().IsSlaver()*/)
	{
		std::vector<PlatformStru_InstrumentMarginRate> lVecMargin;
		std::set<std::string> lInstrumentStringSet;
		CReal_SvrTradeData::getObj().GetInstrumentList(lInstrumentStringSet);
		std::set<std::string>::iterator lIterSet = lInstrumentStringSet.begin();
		while(lIterSet != lInstrumentStringSet.end())
		{
			PlatformStru_InstrumentMarginRate lTempInfo;	
			memset(&lTempInfo,0,sizeof(PlatformStru_InstrumentMarginRate));
			if(0 == lpRate->GetMarginRate(*lIterSet,lTempInfo))	
				lVecMargin.push_back(lTempInfo);
			lIterSet++;
		}

		//д���ݿ� 
		std::string lsTempTradingDay;
		GetCurrentTradingDay(lsTempTradingDay);
		//mpDBOpr->WriteMarginRate(nsBrokerID,nAccountID,lsTempTradingDay,lVecMargin);
	}

	return;
}

//���̵��ã���ĳһ�����ʺ��ʽ��ѯ���
void CReal_SvrTradeData::QryFundReach(const PlatformStru_TradingAccountInfo& inData,const std::string & nUserID)
{
	TRADE_LOG("QryFundReach BrokerID:%s UserID %s",inData.BrokerID,inData.AccountID);

	CSafeRwLockRead lrlock(mrwLockMapAccount);

// 	if(GetTraderDataStatus() == TradeDataStatusTrading && !CInterface_SvrLogin::getObj().IsSlaver())
// 	{
// 		//��������н���״̬��ѯ�����������͸����еĴӷ���
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a QryFund to SLaver %d",lVecSocket[i]);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterQryFund_Push,
// 				(void*)&inData,
// 				sizeof(inData),
// 				0,
// 				0,
// 				0,
// 				0,
// 				CF_ERROR_SUCCESS,0);
// 		}

// 	}
	


	//���ò�ѯ��ϸ��ѯ������
	BrokerAccountKey lKey(inData.BrokerID,inData.AccountID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{

// 		if(1/*GetTraderDataStatus() == TradeDataStatusTrading && CInterface_SvrLogin::getObj().IsSlaver()*/)
// 		{
			lIter->second->SetCommissionMarginReady(CommissionMargin_ComissionAndMarinReady);
			//˵�����������ӵ�ί�н����ʺ�
			TRADE_LOG("SetRatesObjectToAccountRelationUserObject BrokerID:%s AccountID %s",inData.BrokerID,inData.AccountID);
			//���÷��ʶ��󵽶�Ӧ���û�����
			SetRatesObjectToAccountRelationUserObject(inData.BrokerID,inData.AccountID,nUserID);

			//װ���ճ�������
			DispathInitialDataToUserWithAccount(inData.BrokerID,inData.AccountID,mMapUser2TradingData,nUserID);

// 		}
// 		IInterfaceAccountOp * lpTemp = lIter->second;	
// 		if(CF_ERROR_SUCCESS == lpTemp->SetQryFundInfo(inData))
// 		{

			if(mbNeedCache)
            {
				mpCache->CacheFund(inData);
                TRADE_LOG("QryFundReach CacheFund AccountID=%s Available=%.4f DynamicProfit=%.4f StaticProfit=%.4f ",
                    inData.AccountID,inData.Available,inData.DynamicProfit,inData.StaticProfit);

            }

			//����һ����Ҫ��ѯ�������¼�
			//PUSH_LOG_ERROR(Logger,false,true,"����ί�н����ʺ�%s�Ĳ�ѯ�ɽ�����",inData.AccountID);
// 
// 			Stru_NotifyEvent lDataEvent(EventTypeNeedQryTrader,0);		
// 			lDataEvent.AddBufer((void*)&lKey,sizeof(lKey));	
			//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 		}
		
	}
}


//��ѯ��ĳһ�����ʺŵĳֲ�
void CReal_SvrTradeData::QryPositionReach( PlatformStru_Position& PositionInfo, int ErrID, bool bIsLast)
{
	//��ӵ��û�����ģ��ȥ
	TRADE_LOG("QryPositionReach BrokerID:%s UserID %s ErrID:%d,BLast:%d",PositionInfo.BrokerID,PositionInfo.InvestorID,ErrID,bIsLast);

	if(GetTraderDataStatus() == TradeDataStatusTrading /*&& !CInterface_SvrLogin::getObj().IsSlaver()*/)
	{
		//��������н���״̬��ѯ�����������͸����еĴӷ���
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a QryPosition to SLaver %d",lVecSocket[i]);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterQryPosition_Push,
// 				&PositionInfo,
// 				sizeof(PositionInfo),
// 				0,
// 				0,
// 				ErrID,
// 				bIsLast,
// 				CF_ERROR_SUCCESS,0);
// 		}

	}
	CSafeRwLockRead lrlock(mrwLockMapAccount);


	//���ò�ѯ��ϸ��ѯ������
	BrokerAccountKey lKey(PositionInfo.BrokerID,PositionInfo.InvestorID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{
		IInterfaceAccountOp * lpTemp = lIter->second;	
		if(CF_ERROR_SUCCESS == lpTemp->QryPositionReach(PositionInfo,ErrID,bIsLast) && ErrID == 0)
		{
			//��ӵ����洢������
			//mBufferDataStoreOrder.Push(OrderInfo,BufferStoreCallback);
			if(mbNeedCache)
            {
				mpCache->CachePosition(PositionInfo);
                TRADE_LOG("QryPositionReach CachePosition InstrumentID=%s CanCloseTodayVolume=%d CanCloseVolume=%d CanCloseydVolume=%d\
                          CloseProfit=%.4f CloseTodayOrderVolume=%d CloseOrderVolume=%d CloseVolume=%d InvestorID=%s CanCloseydVolume=%d\
                          OpenVolume=%d Position=%d TodayPosition=%d YdPosition=%d",
                          PositionInfo.InstrumentID,PositionInfo.CanCloseTodayVolume,PositionInfo.CanCloseVolume,PositionInfo.CanCloseydVolume,
                          PositionInfo.CloseProfit,PositionInfo.CloseTodayOrderVolume,PositionInfo.CloseOrderVolume,PositionInfo.CloseVolume,
                          PositionInfo.InvestorID,PositionInfo.OpenVolume,PositionInfo.Position,PositionInfo.TodayPosition,PositionInfo.YdPosition);
            }
		}	
	}
	return;
}


//���̵��ã���ĳһ���ֲֲ�ѯ��ɺ�
void CReal_SvrTradeData::EndUserQryPosition(const std::string& nsBrokerID,
						const std::string &nAccountID)
{
	TRADE_LOG("EndUserQryPosition BrokerID:%s AccountID %s",nsBrokerID.c_str(),nAccountID.c_str());
	CSafeRwLockRead lrlock(mrwLockMapAccount);


	//PUSH_LOG_ERROR(Logger,false,true,"ί�н����ʺ�%s�ֲֲ�ѯ���",nAccountID.c_str());


	//���ò�ѯ��ϸ��ѯ������
	BrokerAccountKey lKey(nsBrokerID.c_str(),nAccountID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{
		PlatformStru_Position lPositionInfo;
		lIter->second->QryPositionReach(lPositionInfo,-1,true);
		lIter->second->SetAccountInitStatus(USER_STATUS_InitQryPositionReady);

		if(lIter->second->GetAccountInitStatus() == USER_STATUS_Inited)
		{
			//�����û�����׼�����ˣ����Խ��з�������Ⱥ�������
		/*	Stru_NotifyEvent lsFund(EventTypeUserDataReady,0);
			lsFund.AddBufer((void*)lIter->first.c_str(),lIter->first.length() );	
			CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsFund);	*/
		}
	}

	return;
}



//��ѯ��ĳһ�����ʺŵĳֲ�
void CReal_SvrTradeData::QryPositionDetailReach( PlatformStru_PositionDetail& PositionDetailInfo, int ErrID, bool bIsLast)
{
	//��ӵ��û�����ģ��ȥ
	TRADE_LOG("QryPositionDetailReach BrokerID:%s UserID %s ErrID:%d,BLast:%d",PositionDetailInfo.BrokerID,PositionDetailInfo.InvestorID,ErrID,bIsLast);

// 	if(GetTraderDataStatus() == TradeDataStatusTrading && !CInterface_SvrLogin::getObj().IsSlaver())
// 	{
// 		//��������н���״̬��ѯ�����������͸����еĴӷ���
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a QryPositionDetail to SLaver %d",lVecSocket[i]);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterQryPositionDetail_Push,
// 				&PositionDetailInfo,
// 				sizeof(PositionDetailInfo),
// 				0,
// 				0,
// 				ErrID,
// 				bIsLast,
// 				CF_ERROR_SUCCESS,0);
// 		}

// 	}
	CSafeRwLockRead lrlock(mrwLockMapAccount);


	//���ò�ѯ��ϸ��ѯ������
	BrokerAccountKey lKey(PositionDetailInfo.BrokerID,PositionDetailInfo.InvestorID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{
		IInterfaceAccountOp * lpTemp = lIter->second;	
		if(CF_ERROR_SUCCESS == lpTemp->QryPositionDetailReach(PositionDetailInfo,ErrID,bIsLast) && ErrID == 0)
		{
			//��ӵ����洢������
			//mBufferDataStoreOrder.Push(OrderInfo,BufferStoreCallback);
			if(mbNeedCache)
				mpCache->CachePositionDetail(PositionDetailInfo);
		}	
	}

	return;
}


//���̵��ã���ĳһ���ֲ���ϸ��ѯ��ɺ�
void CReal_SvrTradeData::EndUserQryPositionDetail(const std::string& nsBrokerID,
							  const std::string &nAccountID) 
{

	TRADE_LOG("EndUserQryOrder BrokerID:%s AccountID %s",nsBrokerID.c_str(),nAccountID.c_str());
	CSafeRwLockRead lrlock(mrwLockMapAccount);


	//PUSH_LOG_ERROR(Logger,false,true,"ί�н����ʺ�%s�ֲ���ϸ��ѯ���",nAccountID.c_str());

	
	//���ò�ѯ��ϸ��ѯ������
	BrokerAccountKey lKey(nsBrokerID.c_str(),nAccountID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{
		PlatformStru_OrderInfo lOrderInfo;
		lIter->second->QryOrderReach(lOrderInfo,-1,true);
		lIter->second->SetAccountInitStatus(USER_STATUS_InitQryPositionDetailReady);

		//PUSH_LOG_ERROR(Logger,false,true,"����ί�н����ʺ�%s�Ĳ�ѯ�ֲ�����",nAccountID.c_str());

		//����һ����Ҫ��ѯ�������¼�
		Stru_NotifyEvent lDataEvent(EventTypeNeedQryPosition,0);
		BrokerAccountKey lsBrokerAccount(nsBrokerID.c_str(),nAccountID.c_str());		
		lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
		//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
	}

	return;
}






//��ѯ��������
CF_ERROR CReal_SvrTradeData::QryOrderReach(PlatformStru_OrderInfo& OrderInfo,
										   const int ErrID,
										   const bool bIsLast) 
{

	//��ӵ��û�����ģ��ȥ
	TRADE_LOG("QryOrderReach BrokerID:%s UserID %s ErrID:%d,BLast:%d",OrderInfo.BrokerID,OrderInfo.InvestorID,ErrID,bIsLast);
	
	////�ڽ����еĲ�ѯ������Ҫ���͸��ӷ���
	//if(!CInterface_SvrLogin::getObj().IsSlaver() && GetTraderDataStatus() == TradeDataStatusTrading)
	//{
	//	//��ӷ�����������
	//	vector<SOCKET> lVecSocket;
	//	CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
	//	for(unsigned int i = 0; i < lVecSocket.size() ; i++)
	//	{
	//		CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
	//			Cmd_TradeDataManager_MasterQryOrder_Push,
	//			&OrderInfo,
	//			sizeof(OrderInfo),
	//			0,
	//			0,
	//			ErrID,
	//			bIsLast,
	//			CF_ERROR_SUCCESS,0);
	//	}
	//}

	CSafeRwLockRead lrlock(mrwLockMapUserTrading);

	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(OrderInfo.InvestorID);
	if(mMapUser2TradingData.end() != lIter)
	{
		IInterfaceUserOp * lpTemp = lIter->second;	
		if( ErrID == 0)
		{	
			lpTemp->QryOrderReach(OrderInfo,ErrID,bIsLast) ;
			//��ӵ����洢������
// 			if(!CInterface_SvrLogin::getObj().IsSlaver())	
// 			{
// 				TRADE_LOG("Push order to bufferdate store BrokerID:%s UserID %s ErrID:%d,BLast:%d",OrderInfo.BrokerID,OrderInfo.InvestorID,ErrID,bIsLast);
// 				mBufferDataStoreOrder.Push(OrderInfo,BufferStoreCallback);
// 			}
			if(mbNeedCache)
            {
				mpCache->CacheOrder(OrderInfo);
                TRADE_LOG("QryOrderReach CacheOrder InvestorID=%s InstrumentID=%s CombHedgeFlag=%c CombOffsetFlag=%c\
                          Direction=%c LimitPrice=%.4f VolumeTotalOriginal=%d StatusMsg=%c",
                          OrderInfo.InvestorID,OrderInfo.InstrumentID,OrderInfo.CombHedgeFlag,OrderInfo.CombOffsetFlag,
                          OrderInfo.Direction,OrderInfo.LimitPrice,OrderInfo.VolumeTotalOriginal,OrderInfo.StatusMsg);
            }

		}
	}
	return CF_ERROR_SUCCESS;
}

//���̵��ã���ĳһ���˻��ı�����
void CReal_SvrTradeData::EndUserQryOrder(const std::string& nsBrokerID,
							 const std::string &nAccountID)
{


	TRADE_LOG("EndUserQryOrder BrokerID:%s AccountID %s",nsBrokerID.c_str(),nAccountID.c_str());
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);

	
// 	PUSH_LOG_ERROR(Logger,false,true,"ί�н����ʺ�%s������ѯ���",nAccountID.c_str());

	//���ø��˺���صĽ���Ա��ȫ��������ѯ����
	std::vector<UserInfo> lVecUser;
// 	CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(nAccountID,nsBrokerID,lVecUser);
// 	for(unsigned int i = 0; i < lVecUser.size() ;i++)
// 	{		
//         //���ò�ѯ������������ʼ�����̽���
// 		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(lVecUser[i].szAccount);
// 		if(mMapUser2TradingData.end() != lIter)
// 		{
// 			PlatformStru_OrderInfo lOrderInfo;
// 			lIter->second->QryOrderReach(lOrderInfo,-1,true);
// 			lIter->second->SetUserInitStatus(USER_STATUS_InitQryOrdersReady);
// 
// 
// 			//if(lIter->second->GetUserInitStatus() == USER_STATUS_Inited)
// 			//{
// 			//	//�����û�����׼�����ˣ����Խ��з�������Ⱥ�������
// 			//	Stru_NotifyEvent lsFund(EventTypeUserDataReady,0);
// 			//	lsFund.AddBufer((void*)lIter->first.c_str(),lIter->first.length() );	
// 			//	CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsFund);	
// 			//}
// 		}
// 		//
// 		////���ÿ��Ե�¼
// 		//CInterface_SvrLogin::getObj().SetUserDataReady(lVecUser[i].szAccount,true);
// 	}

	CSafeRwLockRead lrlockAccount(mrwLockMapAccount);
	BrokerAccountKey lKey(nsBrokerID.c_str(),nAccountID.c_str());
	_MapAccountData::iterator lIterAccount = mMapAccountData.find(lKey);
	if(lIterAccount != mMapAccountData.end())
	{
		lIterAccount->second->SetAccountInitStatus(USER_STATUS_InitQryOrdersReady);
	}

	//PUSH_LOG_ERROR(Logger,false,true,"����ί�н����ʺ�%s�Ĳ�ѯ�ֲ���ϸ����",nAccountID.c_str());
	//����һ����Ҫ��ѯ�������¼�
	Stru_NotifyEvent lDataEvent(EventTypeNeedQryPositionDetail,0);
	BrokerAccountKey lsBrokerAccount(nsBrokerID.c_str(),nAccountID.c_str());		
	lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
	//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
}


////���һ������
CF_ERROR CReal_SvrTradeData::PushOrder(PlatformStru_OrderInfo & nOrderInfo,bool nbNeedRiseEvent)
{
	if(GetTraderDataStatus() != TradeDataStatusTrading)
	{
		TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	TRADE_LOG("PushOrder BrokerID:%s UserID %s IId %s %c",nOrderInfo.BrokerID,nOrderInfo.InvestorID,nOrderInfo.InstrumentID,nOrderInfo.Direction);
	

	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	CF_ERROR lRet = CF_ERROR_TRADEDATA_NO_USER_DATA;
	//��ӵ��û�����ģ��ȥ	
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nOrderInfo.InvestorID);
	if(mMapUser2TradingData.end() != lIter)
	{
		IInterfaceUserOp * lpTemp = lIter->second;	
		lRet = lpTemp->PushOrder(nOrderInfo);	
		if(lRet != CF_ERROR_NOINITED && nbNeedRiseEvent)
		{
			//��ӵ����洢������
			if(lRet == CF_ERROR_SUCCESS /*&& !CInterface_SvrLogin::getObj().IsSlaver()*/)			
				mBufferDataStoreOrder.Push(nOrderInfo,&BufferStoreCallback);

			//cache
			if(mbNeedCache)
			{
				mpCache->CacheOrder(nOrderInfo);
				mpCache->PushOrder(nOrderInfo);

                TRADE_LOG("PushOrder CacheOrder InvestorID=%s InstrumentID=%s CombHedgeFlag=%c CombOffsetFlag=%c\
                          Direction=%c LimitPrice=%.4f VolumeTotalOriginal=%d StatusMsg=%c",
                          nOrderInfo.InvestorID,nOrderInfo.InstrumentID,nOrderInfo.CombHedgeFlag,nOrderInfo.CombOffsetFlag,
                          nOrderInfo.Direction,nOrderInfo.LimitPrice,nOrderInfo.VolumeTotalOriginal,nOrderInfo.StatusMsg);

                TRADE_LOG("PushOrder PushOrder InvestorID=%s InstrumentID=%s CombHedgeFlag=%c CombOffsetFlag=%c\
                          Direction=%c LimitPrice=%.4f VolumeTotalOriginal=%d StatusMsg=%c",
                          nOrderInfo.InvestorID,nOrderInfo.InstrumentID,nOrderInfo.CombHedgeFlag,nOrderInfo.CombOffsetFlag,
                          nOrderInfo.Direction,nOrderInfo.LimitPrice,nOrderInfo.VolumeTotalOriginal,nOrderInfo.StatusMsg);

			}

			//����ѳ�ʼ����ϣ��ŷ��ͱ����¼�
			Stru_NotifyEvent lEvent(EventTypeOrderRtn,0);
			lEvent.AddBufer(&nOrderInfo,sizeof(PlatformStru_OrderInfo));	
			//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lEvent);
		}
	}
	if(0/*!CInterface_SvrLogin::getObj().IsSlaver()*/)
	{
		//��ӷ�����������
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a order to SLaver %d ,order info %s %s Ftid:%d updateseq:%d",lVecSocket[i],nOrderInfo.InstrumentID,nOrderInfo.InvestorID,nOrderInfo.FTID,nOrderInfo.UpdateSeq);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterOrder_Push,
// 				&nOrderInfo,
// 				sizeof(nOrderInfo),
// 				0,
// 				0,
// 				0,
// 				0,
// 				CF_ERROR_SUCCESS,0);
// 		}
	}

	return lRet;
}
// ����¼��Ӧ��
CF_ERROR CReal_SvrTradeData::PushRspOrderInsert(int nFrontID,
												int nSessionID,
												CThostFtdcInputOrderField *pInputOrder, 
											    CThostFtdcRspInfoField *pRspInfo,
										        int nRequestID, 
											    bool bIsLast)
{
	//if(GetTraderDataStatus() != TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Not Trading Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}


	if(pInputOrder)
		TRADE_LOG("PushRspOrderInsert BrokerID:%s UserID %s",pInputOrder->BrokerID,pInputOrder->InvestorID);

	//�����¼�
	//Stru_NotifyEvent lEvent(EventTypeRspOrderInsert,0);
	PlatformStru_OrderInfo lOrder;
	GenerateOrderInfoFromCTPInputOrder(*pInputOrder,lOrder);
	lOrder.FrontID = nFrontID;
	lOrder.SessionID = nSessionID;
	
	if(pRspInfo)
		memcpy(lOrder.StatusMsg,pRspInfo->ErrorMsg,sizeof(TThostFtdcErrorMsgType));
	//lEvent.AddBufer(&lOrder,sizeof(PlatformStru_OrderInfo));
	//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lEvent);
	//TRADE_LOG("PushRspOrderInsert Fail");
	lOrder.troubleMoney = util::GetDoubleInvalidValue();	
	lOrder.freezeMargin = util::GetDoubleInvalidValue();
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	sprintf_s(lOrder.CancelTime, sizeof(lOrder.CancelTime), "%02d:%02d:%02d", 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	strncpy(lOrder.UpdateTime, lOrder.CancelTime, sizeof(lOrder.UpdateTime));

	lOrder.OrderStatus = THOST_FTDC_OST_Canceled;
	lOrder.ExStatus = PlatformStru_OrderInfo::ExSta_none;
	CInterface_SvrTradeData::getObj().PushOrder(lOrder,true);

	return CF_ERROR_SUCCESS;
}

///����¼�����ر�
CF_ERROR CReal_SvrTradeData::PushErrRtnOrderInsert(int nFrontID,
												   int nSessionID,
												   CThostFtdcInputOrderField *pInputOrder,
												   CThostFtdcRspInfoField *pRspInfo)
{
	//if(GetTraderDataStatus() != TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Not Trading Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}


	if(pInputOrder)
		TRADE_LOG("PushErrRtnOrderInsert BrokerID:%s UserID %s",pInputOrder->BrokerID,pInputOrder->InvestorID);

	//�����¼�
	//Stru_NotifyEvent lEvent(EventTypeErrOrderInsert,0);
	PlatformStru_OrderInfo lOrder;
	GenerateOrderInfoFromCTPInputOrder(*pInputOrder,lOrder);
	lOrder.FrontID = nFrontID;
	lOrder.SessionID = nSessionID;
	if(pRspInfo)
		memcpy(lOrder.StatusMsg,pRspInfo->ErrorMsg,sizeof(TThostFtdcErrorMsgType));
	lOrder.troubleMoney = util::GetDoubleInvalidValue();	
	lOrder.freezeMargin = util::GetDoubleInvalidValue();
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	sprintf_s(lOrder.CancelTime, sizeof(lOrder.CancelTime), "%02d:%02d:%02d", 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	strncpy(lOrder.UpdateTime, lOrder.CancelTime, sizeof(lOrder.UpdateTime));
	lOrder.OrderStatus = THOST_FTDC_OST_Canceled;
	lOrder.ExStatus = PlatformStru_OrderInfo::ExSta_none;
	
	CInterface_SvrTradeData::getObj().PushOrder(lOrder,true);

	//if(mbNeedCache)
	//	mCache.PushOrderInputError(lOrder);
// 	if(!CInterface_SvrLogin::getObj().IsSlaver())
// 	{
// 		//��ӷ�����������
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a Error order insert to SLaver %d ,order info %s %s Ftid:%d updateseq:%d",lVecSocket[i],lOrder.InstrumentID,lOrder.InvestorID,lOrder.FTID,lOrder.UpdateSeq);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterErrOrderInsert_Push,
// 				&lOrder,
// 				sizeof(PlatformStru_OrderInfo),
// 				0,
// 				0,
// 				0,
// 				0,
// 				CF_ERROR_SUCCESS,0);
// 		}
// 	}
	//��ⶳ�ʽ�
	return CF_ERROR_SUCCESS;

}
///��������������Ӧ
CF_ERROR CReal_SvrTradeData::PushRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, 
								 CThostFtdcRspInfoField *pRspInfo,
								 int nRequestID, 
								 bool bIsLast)
{
	//if(GetTraderDataStatus() != TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Not Trading Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}


	if(pInputOrderAction)
		TRADE_LOG("PushRspOrderAction BrokerID:%s UserID %s",pInputOrderAction->BrokerID,pInputOrderAction->InvestorID);

	//�����¼�
	Stru_NotifyEvent lEvent(EventTypeRspOrderAction,0);		
	lEvent.AddBufer(pInputOrderAction,sizeof(CThostFtdcInputOrderActionField));
	lEvent.AddBufer(pRspInfo,sizeof(CThostFtdcRspInfoField));
	lEvent.AddBufer(&nRequestID,sizeof(int));
	//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lEvent);

	//��ⶳ�ʽ�
	return CF_ERROR_SUCCESS;
}

///������������ر�
CF_ERROR CReal_SvrTradeData::PushErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction,
									CThostFtdcRspInfoField *pRspInfo)
{
	/*if(GetTraderDataStatus() != TradeDataStatusTrading)
	{
		TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/

	if(pOrderAction)
		TRADE_LOG("PushErrRtnOrderAction BrokerID:%s UserID %s",pOrderAction->BrokerID,pOrderAction->InvestorID);

	//�����¼�
	Stru_NotifyEvent lEvent(EventTypeErrOrderAction,0);
	lEvent.AddBufer(pOrderAction,sizeof(CThostFtdcOrderActionField));
	lEvent.AddBufer(pRspInfo,sizeof(CThostFtdcRspInfoField));
	//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lEvent);


// 	if(!CInterface_SvrLogin::getObj().IsSlaver())
// 	{
// 		//��ӷ�����������
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a error order action to SLaver %d ,order info %s %s ",lVecSocket[i],
// 				pOrderAction->InstrumentID,pOrderAction->InvestorID);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterErrOrderAction_Push,
// 				pOrderAction,
// 				sizeof(CThostFtdcOrderActionField),
// 				0,
// 				0,
// 				0,
// 				0,
// 				CF_ERROR_SUCCESS,0);
// 		}
// 	}
	/*if(mbNeedCache)
	{
		mCache.PushOrderActionError(*pOrderAction);
	}*/
	//��ⶳ�ʽ�
	return CF_ERROR_SUCCESS;
}

///��ȡָ������
CF_ERROR CReal_SvrTradeData::GetUserOrderByKey(const std::string & nUserID,
								   const OrderKey& nOrderKey,
								   PlatformStru_OrderInfo& outData) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("GetUserOrderByKey error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;


	return lpTemp->GetOrderByKey(nOrderKey,outData);
}
///��ȡȫ������
CF_ERROR CReal_SvrTradeData::GetUserOrders(const std::string & nUserID,
										   std::vector<PlatformStru_OrderInfo>& outData)
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);	
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;


	return lpTemp->GetAllOrders(outData);

}

/////��ȡȫ���Ѵ����ı���
CF_ERROR CReal_SvrTradeData::GetUserTriggerOrders(const std::string & nUserID,
												  std::vector<PlatformStru_OrderInfo>& outData)
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	//
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetTriggerOrders(outData);
}

/////��ȡ�ѳɽ�����
CF_ERROR CReal_SvrTradeData::GetUserTradedOrders(const std::string & nUserID,
												 std::vector<PlatformStru_OrderInfo>& outData)
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetTradedOrders(outData);

}

/////��ȡ�ѳ����ʹ��󱨵�
CF_ERROR CReal_SvrTradeData::GetUserCanceledOrders(const std::string & nUserID,
												   std::vector<PlatformStru_OrderInfo>& outData)
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	//
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetCanceledOrders(outData);
}

/////��ȡδ�ɽ�����
CF_ERROR CReal_SvrTradeData::GetUserWaitOrders(const std::string & nUserID,
											   std::vector<PlatformStru_OrderInfo>& outData)
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetWaitOrders(outData);

}

///��ȡδ֪״̬�ı���
CF_ERROR CReal_SvrTradeData::GetUserUnkownOrders(const std::string & nUserID,
									 std::vector<PlatformStru_OrderInfo>& outData)
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetUnkownOrders(outData);
}


//
///��ȡָ������
CF_ERROR CReal_SvrTradeData::GetAccountOrderByKey(const std::string & nsBrokerID,
									  const std::string & nInvestorID,
									  const OrderKey& nOrderKey,
									  PlatformStru_OrderInfo& outData)
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetOrderByKey(nOrderKey,outData);
}


///��ȡȫ������
CF_ERROR CReal_SvrTradeData::GetAccountOrders(const std::string & nsBrokerID,
								  const std::string & nInvestorID,
								  std::vector<PlatformStru_OrderInfo>& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetAllOrders(outData);
}


/////��ȡȫ���Ѵ����ı���
CF_ERROR CReal_SvrTradeData::GetAccountTriggerOrders(const std::string & nsBrokerID,
										 const std::string & nInvestorID,
										 std::vector<PlatformStru_OrderInfo>& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetTriggerOrders(outData);
}

/////��ȡ�ѳɽ�����
CF_ERROR CReal_SvrTradeData::GetAccountTradedOrders(const std::string & nsBrokerID,
										const std::string & nInvestorID,
										std::vector<PlatformStru_OrderInfo>& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetTradedOrders(outData);
}

/////��ȡ�ѳ����ʹ��󱨵�
CF_ERROR CReal_SvrTradeData::GetAccountCanceledOrders(const std::string & nsBrokerID,
										  const std::string & nInvestorID,
										  std::vector<PlatformStru_OrderInfo>& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetCanceledOrders(outData);
}

/////��ȡδ�ɽ�����
CF_ERROR CReal_SvrTradeData::GetAccountWaitOrders(const std::string & nsBrokerID,
									  const std::string & nInvestorID,
									  std::vector<PlatformStru_OrderInfo>& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetWaitOrders(outData);
}

///��ȡδ֪״̬�ı���
CF_ERROR CReal_SvrTradeData::GetAccountUnkownOrders(const std::string & nsBrokerID,
										const std::string & nInvestorID,
										std::vector<PlatformStru_OrderInfo>& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetUnkownOrders(outData);
}
//��ѯ�ĳɽ��ر�����
CF_ERROR CReal_SvrTradeData::QryTradeReach( PlatformStru_TradeInfo& TradeInfo,
										   int ErrID,
										   bool bIsLast)
{
	//��ӵ��û�����ģ��ȥ
	TRADE_LOG("QryTradeReach BrokerID:%s UserID %s ErrID:%d,BLast:%d",TradeInfo.BrokerID,TradeInfo.InvestorID,ErrID,bIsLast);
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	if(GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("QryTradeReach Settlement Status,dispatch this call to settlement data with ctp");
		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(TradeInfo.InvestorID);
		if(mMapUser2TradingData.end() != lIter)
		{
			IInterfaceUserOp * lpTemp = lIter->second;	
			lpTemp->QryTradeReach(TradeInfo,ErrID,bIsLast);		
			
		}
		return CF_ERROR_SUCCESS;
	}
	else if(GetTraderDataStatus() == TradeDataStatusInitialing || GetTraderDataStatus() == TradeDataStatusTrading)
	{
		TRADE_LOG("QryTradeReach Trading Status,dispatch this call to trading data");
		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(TradeInfo.InvestorID);
		if(mMapUser2TradingData.end() != lIter)
		{
			IInterfaceUserOp * lpTemp = lIter->second;			
			if(ErrID == 0)
			{
				lpTemp->QryTradeReach(TradeInfo,ErrID,bIsLast);
				//��ӵ����洢������
// 				if(CInterface_SvrLogin::getObj().IsSlaver())
// 				{
// 					TRADE_LOG("QryTradeReach Trading Status,push this trade to buffer store data");
// 					mBufferDataStoreTrade.Push(TradeInfo,BufferStoreCallback);
// 				}
			}
		}
		return CF_ERROR_SUCCESS;
	}
	
	return CF_ERROR_NOINITED;
}


//���̵��ã���ĳһ���˻��ĳɽ���
void CReal_SvrTradeData::EndUserQryTrade(const std::string& nsBrokerID,
							 const std::string &nAccountID)
{
	//���ø��˺���صĽ���Ա��ȫ��������ѯ�ɽ�
	//���Ͳ�ѯ���˻��ı�������
	TRADE_LOG("EndUserQryTrade BrokerID:%s AccountID %s",nsBrokerID.c_str(),nAccountID.c_str());
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	if(GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("EndUserQryTrade Settlement Status,dispatch this call to settlement data with ctp");
		std::vector<UserInfo> lVecUser;
// 		CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(nAccountID,nsBrokerID,lVecUser);
// 		for(unsigned int i = 0; i < lVecUser.size() ;i++)
// 		{
// 			_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(lVecUser[i].szAccount);
// 			if(mMapUser2TradingData.end() != lIter)
// 			{
// 				PlatformStru_TradeInfo lTraderInfo;
// 				lIter->second->QryTradeReach(lTraderInfo,-1,true);
// 				
// 			}
// 		}
		
	}
	else if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusTrading)
	{
		TRADE_LOG("EndUserQryTrade Trading Status,dispatch this call to trading data");
		std::vector<UserInfo> lVecUser;
// 		CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(nAccountID,nsBrokerID,lVecUser);
// 		for(unsigned int i = 0; i < lVecUser.size() ;i++)
// 		{
// 			_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(lVecUser[i].szAccount);
// 			if(mMapUser2TradingData.end() != lIter)
// 			{
// 				PlatformStru_TradeInfo lTraderInfo;
// 				lIter->second->QryTradeReach(lTraderInfo,-1,true);				
// 			}
// 		}

		CSafeRwLockRead lrlockAccount(mrwLockMapAccount);
		BrokerAccountKey lKey(nsBrokerID.c_str(),nAccountID.c_str());
		_MapAccountData::iterator lIterAccount = mMapAccountData.find(lKey);
		if(lIterAccount != mMapAccountData.end())
		{
			lIterAccount->second->SetAccountInitStatus(USER_STATUS_InitQryTradesReady);
		}

	
		//PUSH_LOG_ERROR(Logger,false,true,"�ɽ���ѯ��ɣ�����ί�н����ʺ�%s�Ĳ�ѯ��������",nAccountID.c_str());

		//����һ����Ҫ��ѯ�������¼�
		Stru_NotifyEvent lDataEvent(EventTypeNeedQryOrder,0);
		BrokerAccountKey lsBrokerAccount(nsBrokerID.c_str(),nAccountID.c_str());		
		lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
		//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
		
	}

	

}
//�µĳɽ��ر�����
CF_ERROR CReal_SvrTradeData::PushTrade(PlatformStru_TradeInfo& nTradeInfo)
{

	if(GetTraderDataStatus() != TradeDataStatusTrading)
	{
		TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}

	TRADE_LOG("���ͳɽ�PushTrade OrderRef=%s,  TradeID=%s,  OrderSysID=%s,Direction=%c, OffsetFlag=%c,Price=%f,Volume=%d,InstrumentID=%s,TradeTime=%s",nTradeInfo.OrderRef,nTradeInfo.TradeID,nTradeInfo.OrderSysID,
		nTradeInfo.Direction,nTradeInfo.OffsetFlag,nTradeInfo.Price,nTradeInfo.Volume,nTradeInfo.InstrumentID,nTradeInfo.TradeTime);
	
	

	CSafeRwLockRead lrlock(mrwLockMapUserTrading);

	//��ӵ��û�����ģ��ȥ
	CF_ERROR lRet = CF_ERROR_TRADEDATA_NO_USER_DATA;
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nTradeInfo.InvestorID);
	if(mMapUser2TradingData.end() != lIter)
	{
		
		IInterfaceUserOp * lpTemp = lIter->second;	
		
		TradeKey lkey(nTradeInfo);
		if(!lpTemp->TestTradeInfoExist(lkey))
		{

			lRet = lpTemp->PushTrader(nTradeInfo);	
			if(lRet == CF_ERROR_SUCCESS)
			{

				//��ӵ����洢������
// 				if(CInterface_SvrLogin::getObj().IsSlaver())
// 					mBufferDataStoreTrade.Push(nTradeInfo,BufferStoreCallback);
// 
// 				//����ѳ�ʼ����ϣ��ŷ��ͱ����¼�
// 				Stru_NotifyEvent lEvent(EventTypeTradeRtn,0);
// 				lEvent.AddBufer(&nTradeInfo,sizeof(PlatformStru_TradeInfo));	
// 				CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lEvent);

				////�����ֱֲ仯���ʽ�仯�¼�		
				//Stru_NotifyEvent lsTrader(EventTypeTrade,0);
				//lsTrader.AddBufer((void*)lIter->first.c_str(),lIter->first.length() );	
				//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsTrader);

				//�����ֱֲ仯���ʽ�仯�¼�		
				Stru_NotifyEvent ls(EventTypePosition,0);
				ls.AddBufer((void*)lIter->first.c_str(),lIter->first.length() );	
				//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(ls);

				Stru_NotifyEvent lsFund(EventTypeFund,0);
				lsFund.AddBufer((void*)lIter->first.c_str(),lIter->first.length() );	
				//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsFund);

				//cache
				if(mbNeedCache)
				{
					mpCache->PushTrader(nTradeInfo);
                    //TRADE_LOG("PushTrade PushTrader InvestorID=%s InstrumentID=%s CombHedgeFlag=%c CombOffsetFlag=%c\
                    //          Direction=%c LimitPrice=%.4f VolumeTotalOriginal=%d StatusMsg=%c",
                    //          nTradeInfo.InvestorID,nTradeInfo.InstrumentID,nTradeInfo.CombHedgeFlag,nTradeInfo.CombOffsetFlag,
                    //          nTradeInfo.Direction,nTradeInfo.LimitPrice,nTradeInfo.VolumeTotalOriginal,nTradeInfo.StatusMsg);


					//�����޸ĵĳֲ֣����ʽ�
					PlatformStru_TradingAccountInfo lFundInfo;
					lpTemp->GetFundInfo(lFundInfo);
					mpCache->CacheFund(lFundInfo);

					vector<PlatformStru_Position> lvscPos;
					lpTemp->GetPositions(nTradeInfo.InstrumentID,lvscPos,true);
					for(int i = 0 ; i < lvscPos.size(); i++)
                    {
						mpCache->CachePosition(lvscPos[i]);
                        TRADE_LOG("PushTrade CachePosition InstrumentID=%s CanCloseTodayVolume=%d CanCloseVolume=%d CanCloseydVolume=%d\
                                  CloseProfit=%.4f CloseTodayOrderVolume=%d CloseOrderVolume=%d CloseVolume=%d InvestorID=%s CanCloseydVolume=%d\
                                  OpenVolume=%d Position=%d TodayPosition=%d YdPosition=%d",
                                  lvscPos[i].InstrumentID,lvscPos[i].CanCloseTodayVolume,lvscPos[i].CanCloseVolume,lvscPos[i].CanCloseydVolume,
                                  lvscPos[i].CloseProfit,lvscPos[i].CloseTodayOrderVolume,lvscPos[i].CloseOrderVolume,lvscPos[i].CloseVolume,
                                  lvscPos[i].InvestorID,lvscPos[i].OpenVolume,lvscPos[i].Position,lvscPos[i].TodayPosition,lvscPos[i].YdPosition);

                    }
				}
			}
		}
		else
		{
			TRADE_LOG("Push an Exsit Trader");
		}

	}
// 	if(!CInterface_SvrLogin::getObj().IsSlaver())
// 	{
// 		//��ӷ�����������
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a trader to SLaver %d,trader info %s %s %d %d",lVecSocket[i],nTradeInfo.InstrumentID,nTradeInfo.InvestorID,nTradeInfo.FTID,nTradeInfo.UpdateSeq);
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterTrader_Push,
// 				&nTradeInfo,
// 				sizeof(nTradeInfo),
// 				0,
// 				0,
// 				0,
// 				0,
// 				CF_ERROR_SUCCESS,0);
// 		}
// 	}
	return lRet;
}

//���CTP�ɽ�����ı�������ǲ������ǵı������
bool CReal_SvrTradeData::CheckTradeOrderSysIDIsOwn(const std::string& nsUserID,
									   const std::string& nsOrderSysID) 
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return false;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nsUserID);
	if(mMapUser2TradingData.end() == lIter)
		return false;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->CheckTradeOrderSysIDIsOwn(nsOrderSysID);
}

//��ȡȫ���ɽ��������������������
CF_ERROR  CReal_SvrTradeData::GetUserTradeInfo(const std::string & nUserID,											  
											   std::vector<PlatformStru_TradeInfo>& outData)
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
	//	TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetAllTrades(outData);

	return CF_ERROR_SUCCESS;

}

//��ȡȫ���ɽ��������������������
CF_ERROR  CReal_SvrTradeData::GetAccountTradeInfo(const std::string & nsBrokerID,
									  const std::string & nInvestorID, 
									  std::vector<PlatformStru_TradeInfo>& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetAllTrades(outData);
}									


//��ȡ���˻������ɽ�����Seq
CF_ERROR  CReal_SvrTradeData::GetAccountOrderAndTraderGESeq(const std::string & nsBrokerID,
												const std::string & nInvestorID,
												int nOrderSeq,
												int nTraderSeq,
												std::vector<PlatformStru_OrderInfo> & nvecOrder,
												std::vector<PlatformStru_TradeInfo>& nvecTrader) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;
	lpTemp->GetOrderAndTradeGeSeq(nOrderSeq,nTraderSeq,nvecOrder,nvecTrader );
	return CF_ERROR_SUCCESS;
}


//��ȡ���˻������ɽ�����Seq
CF_ERROR  CReal_SvrTradeData::GetUserOrderAndTraderGESeq(const std::string & nInvestorID,
											 int nOrderSeq,
											 int nTraderSeq,
											 std::vector<PlatformStru_OrderInfo> & nvecOrder,
											 std::vector<PlatformStru_TradeInfo>& nvecTrader) 
{

	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nInvestorID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	lpTemp->GetOrderAndTradeGeSeq(nOrderSeq,nTraderSeq,nvecOrder,nvecTrader );
	return CF_ERROR_SUCCESS;
}
//��ȡ�ֲֻ��ܣ����������������
CF_ERROR  CReal_SvrTradeData::GetUserPositions(const std::string & nUserID,
											   const std::string & nInstrumentID,
											   std::vector<PlatformStru_Position>& outData,
											   bool bNeedZeroVolumePosition ) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		//TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetPositions(nInstrumentID,outData, bNeedZeroVolumePosition );
	
}


//��ȡ�ֲܳ���
CF_ERROR CReal_SvrTradeData::GetUserPositionVolume(const std::string & nUserID,
									   const std::string & nInstrumentID,
									   int & nVolume) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		//TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetPositionVolume(nInstrumentID,nVolume);
}

//��ȡ�ֲֻ��ܣ�
CF_ERROR  CReal_SvrTradeData::GetAccountPositions(const std::string & nsBrokerID,
									  const std::string & nInvestorID,
									  const std::string & nInstrumentID,
									  std::vector<PlatformStru_Position>& outData,
									  bool bNeedZeroVolumePosition ) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetPositions(nInstrumentID,outData, bNeedZeroVolumePosition );
}		


//��ȡ�ֲ���ϸ�����������������
CF_ERROR   CReal_SvrTradeData::GetUserPositionDetail(const std::string & nUserID,
													 const std::string & nInstrumentID,
													 std::vector<PlatformStru_PositionDetail>& outData) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		//TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetPositionDetail(nInstrumentID,outData);

	
}

//��ȡ�ֲ���ϸ�����ض�Ӧ��key������
CF_ERROR   CReal_SvrTradeData::GetUserPositionDetail(const std::string & nUserID,
										const PositionDetailKey & nKey,
										PlatformStru_PositionDetail& outData) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
	//	TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetPositionDetail(nKey,outData);
}
//��ȡ�ֲ���ϸ
//�ɹ�����CF_ERROR_SUCCESS
CF_ERROR  CReal_SvrTradeData::GetAccountPositionDetail(const std::string & nsBrokerID,
										   const std::string & nInvestorID,
										   const std::string & nInstrumentID,
										   std::vector<PlatformStru_PositionDetail>& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetPositionDetail(nInstrumentID,outData);
}		


//��ȡ�ֲ���ϸ�����ض�Ӧ��key�ĳֲ���ϸ
//�ɹ�����CF_ERROR_SUCCESS
CF_ERROR  CReal_SvrTradeData::GetAccountPositionDetail(const std::string & nsBrokerID,
										   const std::string & nInvestorID,
										   const PositionDetailKey & nKey,
										   PlatformStru_PositionDetail& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetPositionDetail(nKey,outData);
}		





//��ȡ��ϳֲ֣����������������
CF_ERROR  CReal_SvrTradeData::GetUserPositionComb(const std::string & nUserID,
												  const std::string & nInstrumentID,
												  std::vector<PlatformStru_Position>& outData)
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
	//	TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetPositionComb(nInstrumentID,outData);


}






//��ȡ��ϳֲ���ϸ�����������������
CF_ERROR  CReal_SvrTradeData::GetUserPositionCombDetail(const std::string & nUserID,
														const std::string & nInstrumentID,
														std::vector<PlatformStru_PositionDetailComb>& outData)
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
	//	TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetPositionCombDetail(nInstrumentID,outData);

	
}

//��ȡ����Ա�ʽ���Ϣ
CF_ERROR  CReal_SvrTradeData::GetUserFundInfo(const std::string & nUserID
											  ,PlatformStru_TradingAccountInfo& outData) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
	//	TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetFundInfo(outData);
}

//��ȡί�н����ʺ��ʽ���Ϣ
CF_ERROR   CReal_SvrTradeData::GetAccountFundInfo(const std::string & nsBrokerID,
									 const std::string & nInvestorID,
									 PlatformStru_TradingAccountInfo& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;
	return lpTemp->GetFundInfo(outData);
}		

CF_ERROR  CReal_SvrTradeData::GetUserInitFund(const std::string & nUserID,
						  double& outData) 
{

	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
	//	TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetFirstFund(outData);
	
}

//��ȡ��ǰ�����յĺ�Լ
CF_ERROR  CReal_SvrTradeData::GetCurrentDayAlInstrumentInfo(
	std::vector<PlatformStru_InstrumentInfo> & outData) 
{
	//if(GetTraderDataStatus() == TradeDataStatusInitialing
	//	|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	//{
	//	TRADE_LOG("error Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}
	//��ȡ��ǰ�����ն����ݿ�
	std::string lsTradingDay;
	GetCurrentTradingDay(lsTradingDay);
	if(lsTradingDay.empty())
	{
		TRADE_LOG("no tradingday,discard this call");
		return CF_ERROR_TRADEDATA_NOTRADINGDAY;
	}
	//װ�ؽ�������

	return 0/*mpDBOpr->LoadTodayInstrument(lsTradingDay,outData)*/;
	
}


//��ȡ����ί�н����˺ŵ���������
CF_ERROR CReal_SvrTradeData::GetAllCommissionRate(
	std::vector<PlatformStru_InstrumentCommissionRate> & outData)
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting
		|| GetTraderDataStatus() == TradeDataStatusTrading)
	{
		TRADE_LOG("Init Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	//m_mutexMapAccount.lock();
	CSafeRwLockRead lLock(mrwLockMapAccount);
	outData.clear();
	_MapAccountData::iterator lIter =  mMapAccountData.begin();

	while(mMapAccountData.end() != lIter)	
	{		
		std::set<std::string> lInstrumentStringSet;
		CReal_SvrTradeData::getObj().GetInstrumentList(lInstrumentStringSet);
		std::set<std::string>::iterator lIterSet = lInstrumentStringSet.begin();
		while(lIterSet != lInstrumentStringSet.end())
		{		
			PlatformStru_InstrumentCommissionRate lTempInfo;			
			if(lIter->second != NULL && 0 == lIter->second->GetCommissionRate(*lIterSet,lTempInfo))
				outData.push_back(lTempInfo);
			lIterSet++;
		}
		lIter++;
	}
	

	return CF_ERROR_SUCCESS;
}

//��ȡ����ί�н����˺ŵı�֤����
CF_ERROR CReal_SvrTradeData::GetAllMarginRate(
	std::vector<PlatformStru_InstrumentMarginRate>  & outData)
{

	//if(GetTraderDataStatus() == TradeDataStatusInitialing 
	//	|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK
	//	|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting
	//	|| GetTraderDataStatus() == TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Init Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lLock(mrwLockMapAccount);

	outData.clear();

	_MapAccountData::iterator lIter =  mMapAccountData.begin();

	while(mMapAccountData.end() != lIter)	
	{		
		std::set<std::string> lInstrumentStringSet;
		CReal_SvrTradeData::getObj().GetInstrumentList(lInstrumentStringSet);
		std::set<std::string>::iterator lIterSet = lInstrumentStringSet.begin();
		while(lIterSet != lInstrumentStringSet.end())
		{
			PlatformStru_InstrumentMarginRate lTempInfo;			
			if(lIter->second != NULL && 0 == lIter->second->GetMarginRate(*lIterSet,lTempInfo))
				outData.push_back(lTempInfo);
			lIterSet++;
		}
		lIter++;
	}

	return CF_ERROR_SUCCESS;
}
//��ȡ���еĽ����
CF_ERROR  CReal_SvrTradeData::GetAllSettlementPrice(
	std::map<std::string,PlatformStru_DepthMarketData> & outData) 
{
	//if(GetTraderDataStatus() == TradeDataStatusInitialing 
	//	|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK
	//	|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting
	//	|| GetTraderDataStatus() == TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Init Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeLock lLock(mMutextSettlementQuot);
	outData.clear();
	outData = mmapSettlementQuot;
	return CF_ERROR_SUCCESS;
}

//��ȡ��ǰ�����յĽ����
CF_ERROR  CReal_SvrTradeData::GetCurrentDayAllSettlementPrice(
	std::map<std::string,PlatformStru_DepthMarketData> & outData) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("GetCurrentDayAllSettlementPrice error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	//��ȡ��ǰ�����ն����ݿ�
	std::string lsTradingDay;
	GetCurrentTradingDay(lsTradingDay);
	if(lsTradingDay.empty())
	{
		TRADE_LOG("GetCurrentDayAllSettlementPrice no tradingday,discard this call");
		return CF_ERROR_TRADEDATA_NOTRADINGDAY;
	}
	//װ�ؽ�������
	vector<PlatformStru_DepthMarketData> lVecQuto;	
// 	if(CF_ERROR_SUCCESS == mpDBOpr->LoadQutoInfo("TRADEDATA_Quto",lsTradingDay,lVecQuto))
// 	{
// 		TRADE_LOG("GetCurrentDayAllSettlementPrice LoadTodaySettlementQuto From DataBase Success  %s",lsTradingDay.c_str());
// 		vector<PlatformStru_DepthMarketData>::iterator lIte = lVecQuto.begin();
// 		for(;lIte != lVecQuto.end();lIte++)
// 		{				
// 			//���뵽��������map��
// 			outData[lIte->InstrumentID] = *lIte;		
// 		}		
// 	}
	return CF_ERROR_SUCCESS;

}
//��ȡ���еĳɽ�
CF_ERROR  CReal_SvrTradeData::GetAllTradeInfo(
	std::map<std::string,std::vector<PlatformStru_TradeInfo>> & outData) 
{

	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		//TRADE_LOG("GetAllTradeInfo error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting
		|| GetTraderDataStatus() == TradeDataStatusTrading)
	{
		TRADE_LOG("Init Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/

	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	//_MapUserTradeData * lpMap = GetMapUserDataWithOption(nOption);
	if(mMapUser2TradingData.size() == 0)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;


	outData.clear();
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();

	while(mMapUser2TradingData.end() != lIter)	
	{		
		vector<PlatformStru_TradeInfo> lVec;
		if(lIter->second != NULL && 
			CF_ERROR_SUCCESS == lIter->second->GetAllTrades(lVec))
		{
			//if(lVec.size() > 0)
			{
				//���ɽ�ʱ������
				//std::sort(lVec.begin(),lVec.end(),TraderSorterObject);
				outData[lIter->first] = lVec;
				//outData.insert(std::make_pair(lIter->first,lVec));
			}
		}
		
		lIter++;
	}
	return CF_ERROR_SUCCESS;
}

//��ȡ���еĳֲ���ϸ
CF_ERROR  CReal_SvrTradeData::GetAllPositionDetail(
	std::map<std::string,std::vector<PlatformStru_PositionDetail>> & outData) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		//TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting
		|| GetTraderDataStatus() == TradeDataStatusTrading)
	{
		TRADE_LOG("Init Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	if(mMapUser2TradingData.size() == 0)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;


	outData.clear();

	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();

	while(mMapUser2TradingData.end() != lIter)	
	{		
		vector<PlatformStru_PositionDetail> lVec;
		if(lIter->second != NULL && 
			CF_ERROR_SUCCESS == lIter->second->GetAllPositionDetail(lVec))
		{
			//if(lVec.size() > 0)
			outData[lIter->first] = lVec;
				//outData.insert(std::make_pair(lIter->first,lVec));
		}

		lIter++;
	}
	return CF_ERROR_SUCCESS;
}

//��ȡ���еĳֲ�
CF_ERROR  CReal_SvrTradeData::GetAllPosition(
	std::map<std::string,std::vector<PlatformStru_Position>> & outData) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		//TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting
		|| GetTraderDataStatus() == TradeDataStatusTrading)
	{
		TRADE_LOG("Init Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	if(mMapUser2TradingData.size() == 0)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;


	outData.clear();

	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();

	while(mMapUser2TradingData.end() != lIter)	
	{		
		vector<PlatformStru_Position> lVec;
		if(lIter->second != NULL && 
			CF_ERROR_SUCCESS == lIter->second->GetAllPosition(lVec))
		{
			//if(lVec.size() > 0)
			outData[lIter->first] = lVec;
				//outData.insert(std::make_pair(lIter->first,lVec));
		}

		lIter++;
	}
	return CF_ERROR_SUCCESS;
}

//��ȡ���е��ʽ�
CF_ERROR  CReal_SvrTradeData::GetAllFundInfo(
	std::map<std::string,PlatformStru_TradingAccountInfo> & outData) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		//TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	//if(GetTraderDataStatus() == TradeDataStatusInitialing 
	//	|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK
	//	|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting
	//	|| GetTraderDataStatus() == TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Init Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}

	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	if(mMapUser2TradingData.size() == 0)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;


	outData.clear();

	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();

	while(mMapUser2TradingData.end() != lIter)	
	{		
		PlatformStru_TradingAccountInfo lTemp;
		if(lIter->second != NULL &&
			CF_ERROR_SUCCESS == lIter->second->GetFundInfo(lTemp))
		{
			//outData.insert(std::make_pair(lIter->first,lTemp));
			outData[lIter->first] = lTemp;
		}

		lIter++;
	}
	return CF_ERROR_SUCCESS;
}

//��ʼ����
CF_ERROR  CReal_SvrTradeData::StartSettlement(eSettlementLoadOption nOption,
											  const std::string & nsDay ,
											  bool nbHistorySettlement) 
{

	TRADE_LOG("StartSettlement %s %d %s",nsDay.c_str(),nOption,nbHistorySettlement?"History":"Today");

	//PUSH_LOG_ERROR(Logger,false,true,"��ʼ����%s",nsDay.c_str());
	eTradeDataStatus  leStaus = GetTraderDataStatus();

	if(leStaus == TradeDataStatusInitialing)
	{
		//TRADE_LOG("this status ,no settlement");

		TRADE_LOG("StartSettlement this status ,no settlement");

		return CF_ERROR_TRADEDATA_SETTMENT_NOINIT;
	}
		
	


	//����ǽ���ļ���
	if(nbHistorySettlement == false)
	{
		if(GetTraderDataStatus() == TradeDataStatusNoTradingDay)
		{
			//TRADE_LOG("this status ,just history settlement");

			TRADE_LOG("StartSettlement this status ,just history settlement");
			//PUSH_LOG_ERROR(Logger,false,true,"û�н�����ֻ������ʷ����");
			return CF_ERROR_TRADEDATA_SETTMENT_NOINIT;
		}

		////��������״̬�������н���
		//if(GetExchangeStatus() == ExchangeStatus_ContinueTrade )
		//{
		//	TRADE_LOG("EXCHANGE_ISTRADE,no settlement");
		//	return CF_ERROR_TRADEDATA_EXCHANGE_ISTRADE;
		//}
		//û�н����գ�ֻ�ܽ�����ʷ����
		std::string lsTempTradingDay;
		GetCurrentTradingDay(lsTempTradingDay);
		if(lsTempTradingDay.empty())
		{
			TRADE_LOG("StartSettlement No TradingDay");
			//PUSH_LOG_ERROR(Logger,false,true,"û�л�ȡ��������ֻ������ʷ����");
			return CF_ERROR_TRADEDATA_NOTRADINGDAY;
		}

		if(GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK)
		{
			TRADE_LOG("StartSettlement TradeDataStatusLastDaySettlemtNotOK");
			mbDirty = true;
		}
	}
	else
	{		

		//�������������״̬�����ܽ���
		/*if(GetExchangeStatus() == ExchangeStatus_ContinueTrade &&
				GetTraderDataStatus() == TradeDataStatusTrading)
		{
			TRADE_LOG("EXCHANGE_ISTRADE,no settlement");
			return CF_ERROR_TRADEDATA_EXCHANGE_ISTRADE;
		}*/
		if(!IsTradingDay(nsDay))
		{
			TRADE_LOG("StartSettlement History Settlement No traydingDay");
			//PUSH_LOG_ERROR(Logger,false,true,"���ղ��ǽ����գ����ܽ��н���");
			return CF_ERROR_TRADEDATA_NOTRADINGDAY;
		}

	}
	
	
	//װ�سɹ�������Ϊ����״̬		
	SetTraderDataStatus(TradeDataStatusSettlemtIniting);

    //��ӷ������ͽ����¼�
//     vector<SOCKET> lVecSocket;
//     CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
//     for(unsigned int i = 0; i < lVecSocket.size() ; i++)
//     {
//         TRADE_LOG("Master push a StartSettlement Msg to SLaver %d",lVecSocket[i]);
//         CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
//             CMDID_NotifyStartSettlementToSlave_Push,
//             NULL,
//             0,
//             0,
//             0,
//             0,
//             0,
//             CF_ERROR_SUCCESS,0);
//     }


	//����ͬ��֪ͨ����ģ��ֹͣ����
	Stru_NotifyEvent lDataEvent(EventTypeStartSettlement,0);
	//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);

	//�ǽ�����������ݽ��㣬
	if(nOption == SettlementLoadOption_TradingDataWithTodayDB 
		&& nbHistorySettlement == false)
	{	

		//�����ǰ�ǽ���״̬�������Ǹɾ��ģ���ֱ�ӷ���ʹ��
		if(mbDirty == false )
		{		
			TRADE_LOG("StartSettlement Today ettlement with Clean Data no need to load");
			//PUSH_LOG_ERROR(Logger,false,true,"ֱ��ʹ�ý�����ڴ����ݽ��н��㣬�������������״̬");
			TRADE_LOG("��������δ֪��δ�ɽ�����");
			CReal_SvrTradeData::getObj().CancelAllUnkonwnOrder();
			CReal_SvrTradeData::getObj().SaveAllTradingData();

			SetTraderDataStatus(TradeDataStatusSettlemting);
			return CF_ERROR_SUCCESS;
		}
	}
	//�����ڴ�
	ResetAll();	

	TRADE_LOG("StartSettlement Settlement Check End");

	//�������ݲ��ɾ�
	mbDirty = true;

	//���������ݿ���ת������
	CF_ERROR lRet = SettlementInitTradeDataWithTradingDB(nOption,nsDay);
	if(lRet == CF_ERROR_SUCCESS)
	{
		TRADE_LOG("StartSettlement Settlement Load End");
		//PUSH_LOG_ERROR(Logger,false,true,"���ؽ���������ɣ��������������״̬");
		//װ�سɹ�������Ϊ����״̬		
		SetTraderDataStatus(TradeDataStatusSettlemting);
	}
	else
	{
		TRADE_LOG("StartSettlement Settlement Load Faild");
		//PUSH_LOG_ERROR(Logger,false,true,"�����ݿ���ؽ�������ʧ��");
		//װ��ʧ�ܣ�����Ϊԭ����״̬
		SetTraderDataStatus(leStaus);
	}
	//if(nOption == SettlementLoadOption_TradingDataWithHistory ||
	//	nOption == SettlementLoadOption_TradingDataWithTodayDB)
	//{
	//	if(lRet == CF_ERROR_SUCCESS)
	//	{
	//		//װ�سɹ�������Ϊ����״̬		
	//		SetTraderDataStatus(TradeDataStatusSettlemting);
	//	}
	//	else
	//	{
	//		//װ��ʧ�ܣ�����Ϊԭ����״̬
	//		SetTraderDataStatus(leStaus);
	//	}
	//}
	//else
	//{
	//	if(lRet != CF_ERROR_SUCCESS)
	//	{
	//		//װ��ʧ�ܣ�����Ϊԭ����״̬
	//		SetTraderDataStatus(leStaus);
	//	}
	//}

	return lRet;
}



//���������ݿ��ʼ����������
CF_ERROR CReal_SvrTradeData::SettlementInitTradeDataWithTradingDB(eSettlementLoadOption nOption,
																  const std::string & nsDay) 
{
	if(GetTraderDataStatus() != TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("SettlementInitTradeDataWithTradingDB Not Settlemnt Status,discard this call");
		
		return CF_ERROR_NOINITED;
	}

	//CSafeLock lLock(m_mutexMapUserTraded);

	//��ʼ�������û��ĳ�ʼ������
// 	InitUserMapWithEmpty(mMapUser2TradingData); 

	//��ȡ��һ��������
	std::string lsLastTradingDay;
	//mpTradingDayManager.GetLastTradingData(nsDay,lsLastTradingDay);

// 	if(!lsLastTradingDay.empty() &&
// 		!mpTradingDayManager.IsSetttemnted(lsLastTradingDay))
// 	{
// 		////����Ϊ��һ�������ս��㲻��ȫ״̬����Ҫ���½�����һ�������յ�����	
// 		TRADE_LOG("LoadInitData Failed need resettlement");
// 		return CF_ERROR_TRADEDATA_SETTMENT_NOINIT;
// 	}
// 	else
// 	{
// 		//װ���ճ�����
// 		LoadInitData(lsLastTradingDay);
// 		////����Ϊ��һ�������ս��㲻��ȫ״̬����Ҫ���½�����һ�������յ�����	
// 		TRADE_LOG("LoadInitData load day is %s",lsLastTradingDay.c_str() );		
// 	}


	//�����Լ
	//������ݿ��и��յĺ�Լ�����ȡ���ݿ�
	//���ú�Լ�ͷ��ʵ�����
	
	vector<PlatformStru_InstrumentInfo> lVecInstrument;	
// 	if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayInstrument(nsDay,lVecInstrument))
// 	{
// 		TRADE_LOG("LoadTodayInstrument From DataBase Success  %s",nsDay.c_str());
// 		vector<PlatformStru_InstrumentInfo>::iterator lIte = lVecInstrument.begin();
// 		for(;lIte != lVecInstrument.end();lIte++)
// 		{
// 			SetInstrumentInfo(*lIte);
// 		}
// 		msInstrumentDay = nsDay;
// 		//���������û���		
// 		InterlockedExchange(reinterpret_cast<volatile long*>(&mbInstrumentsReady),DataReady_Ready);
// 
// 		//���ú�Լ����Ľ�����
// 		ExchangeTime lTime;
// 		lTime.CurTickCount = GetTickCount();
// 		std::string lsTempTradingDay = msInstrumentDay.substr(0,4) + msInstrumentDay.substr(5,2) + msInstrumentDay.substr(8,2);
// 		strcpy(lTime.TradingDay,lsTempTradingDay.c_str());
// 		mpPlatformDataInstrument->SetExchangeTime(lTime);
// 
// 	}
// 	else
// 	{
// 		return CF_ERROR_TRADEDATA_SETTMENT_NOTRAING;
// 	}

	//װ�ؽ�������
	vector<PlatformStru_DepthMarketData> lVecQuto;	
// 	if(CF_ERROR_SUCCESS == mpDBOpr->LoadQutoInfo("TRADEDATA_Quto",nsDay,lVecQuto))
// 	{
// 		TRADE_LOG("LoadTodaySettlementQuto From DataBase Success  %s",nsDay.c_str());
// 		vector<PlatformStru_DepthMarketData>::iterator lIte = lVecQuto.begin();
// 		for(;lIte != lVecQuto.end();lIte++)
// 		{
// 			mMutextSettlementQuot.lock();			
// 			//���뵽��������map��
// 			mmapSettlementQuot[lIte->InstrumentID] = *lIte;
// 			mMutextSettlementQuot.unlock();
// 
// 			mpPlatformDataQuots->SetQuotInfo(*lIte);
// 		}		
// 	}

	//������������
	//������ݿ��н��������˻����������ʣ����ȡ���ݿ�

	//����֤����
	//������ݿ��н��������˻��ı�֤���ʣ����ȡ���ݿ�

	{
		vector<TradeAccount> lVecAccount;
// 		CInterface_SvrBrokerInfo::getObj().GetTradeAccounts(lVecAccount);
// 		vector<TradeAccount>::iterator lIter = lVecAccount.begin();
// 		for(;lIter != lVecAccount.end();lIter++)
// 		{
// 			//���ó�ʼ������
// 			bool lbCommisstionLoadSuccess = false;
// 			bool lbMarginRatesLoadSuccess = false;
// 			BrokerInfo lBroker;
// 			CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lIter->nBrokerID,lBroker);
// 			BrokerAccountKey lKey(lBroker.szBrokerCode,lIter->szTradeAccount);			
// 
// 			IInterfaceAccountOp * lpTemp = NULL;
// 			if(mMapAccountData.find(lKey) == mMapAccountData.end())
// 			{
// 				lpTemp = IFactoryAccountOp::GetFactory()->CreateAccountOp();
// 				mMapAccountData.insert(std::make_pair(lKey,lpTemp));
// 			}
// 			else
// 			{
// 				lpTemp = mMapAccountData.find(lKey)->second;
// 			}
// 
// 			vector<PlatformStru_InstrumentCommissionRate> lVecCommissionRate;	
// 			//���װ�óɹ������Һ�Լ�ĸ�����Ϊ0 ���ҷ��ʵĸ����ͺ�Լ�ĸ�����ȵ�����²���ɹ�
// 			if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayCommission(lBroker.szBrokerCode, lIter->szTradeAccount,nsDay,lVecCommissionRate) )
// 			{
// 				TRADE_LOG("LoadTodayCommission From DataBase Success:%s AccountID %s",
// 					lIter->szTradeAccount,nsDay.c_str());
// 				lbCommisstionLoadSuccess = true;
// 				vector<PlatformStru_InstrumentCommissionRate>::iterator lIte = lVecCommissionRate.begin();
// 				for(;lIte != lVecCommissionRate.end();lIte++)
// 				{
// 					SetCommissionRate(*lIte);
// 				}		
// 				//����״̬
// 				_MapAccountData::iterator lIterReady =  mMapAccountData.find(lKey);
// 				if(mMapAccountData.end() != lIterReady)
// 				{
// 					lIterReady->second->SetCommissionMarginReady(CommissionMargin_ComissionReady);
// 				/*	int lnTemp = (int)lIterReady->second;
// 					lnTemp |= CommissionMargin_ComissionReady;
// 					lIterReady->second = (eCommissionMarginReady)lnTemp;*/
// 				}
// 			}
// 			else
// 			{
// 				TRADE_LOG("LoadTodayCommission From DataBase Faild %s AccountID %s",
// 					lIter->szTradeAccount,nsDay.c_str());
// 			}
// 
// 
// 			//ȥװ�ر�֤����
// 			//���װ�óɹ������Һ�Լ�ĸ�����Ϊ0 ���ҷ��ʵĸ����ͺ�Լ�ĸ�����ȵ�����²���ɹ�
// 			vector<PlatformStru_InstrumentMarginRate> lVecMarginRate;	
// 			if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayMarginRate(lBroker.szBrokerCode,lIter->szTradeAccount,nsDay,lVecMarginRate) )
// 			{
// 				TRADE_LOG("LoadTodayMarginRate From DataBase Success:%s AccountID %s",lIter->szTradeAccount,nsDay.c_str());
// 				lbMarginRatesLoadSuccess = true;
// 				vector<PlatformStru_InstrumentMarginRate>::iterator lIte = lVecMarginRate.begin();
// 				for(;lIte != lVecMarginRate.end();lIte++)
// 				{
// 					SetMarginRate(*lIte);
// 				}	
// 				//����״̬
// 				_MapAccountData::iterator lIterReady =  mMapAccountData.find(lKey);
// 				if(mMapAccountData.end() != lIterReady)
// 				{
// 					lIterReady->second->SetCommissionMarginReady(CommissionMargin_MarginReady);
// 				/*	int lnTemp = (int)lIterReady->second;
// 					lnTemp |= CommissionMargin_MarginReady;
// 					lIterReady->second = (eCommissionMarginReady)lnTemp;*/
// 				}
// 				
// 			}
// 			else
// 			{
// 				TRADE_LOG("LoadTodayCommission From DataBase Faild %s AccountID %s",
// 					lIter->szTradeAccount,nsDay.c_str());
// 			}			
// 
// 		}
	}

	//���ɺ�Լ�ͷ��ʶ���ÿ���û�������
	{
		//���ú�Լ�����ʺŶ�����
		SetPlatformObjectToAllAccount();

		//���ú�Լ����������û��Ķ���
		SetPlatformObjectToAllUser(mMapUser2TradingData);

		//ָ���ʺŶ�Ӧ�ķ��ʶ��������û��Ķ���
		DispathRateToAllUser(mMapUser2TradingData);

	}

	//�����ճ����ݵ�ÿ���û�������
	{
		
// 		vector<TradeAccount> lVecAccount;
// 		CInterface_SvrBrokerInfo::getObj().GetTradeAccounts(lVecAccount);
// 		vector<TradeAccount>::iterator lIterAcc = lVecAccount.begin();
// 		for(;lIterAcc != lVecAccount.end();lIterAcc++)
// 		{
// 			BrokerInfo lBrokeInfo;
// 			CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lIterAcc->nBrokerID, lBrokeInfo);		
// 			DispathInitialDataToUserWithAccount(lBrokeInfo.szBrokerCode,lIterAcc->szTradeAccount,mMapUser2TradingData);
// 		
// 		}	
	}

	


	//���ɵ���ĳ�����û�����
	//DispathFundInOut(nsDay,mMapUser2TradingData);

	//�������е����ݵ�ÿ���û�������
// 	if(nOption != SettlementLoadOption_TradingDataWithCTPSett)
// 	{
// 		CSafeRwLockRead lrlock(mrwLockMapUserTrading);
// 
// 		vector<UserInfo> lVecUser;
// 		CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUser);
// 		vector<UserInfo>::iterator lIter = lVecUser.begin();
// 		for(;lIter != lVecUser.end();lIter++)
// 		{
// 			if(lIter->nUserType == USER_TYPE_TRADER)
// 			{
// 				_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(lIter->szAccount);
// 				if(lIterUser != mMapUser2TradingData.end())
// 				{
// 					//װ�سɽ�
// 					vector<PlatformStru_TradeInfo> lTradeVec;
// 					mpDBOpr->LoadTodayUserTrades("TRADEDATA_TRADES",lIter->szAccount,nsDay,lTradeVec);
// 					if(lTradeVec.size() == 0)
// 					{
// 						PlatformStru_TradeInfo lTemp;
// 						lIterUser->second->QryTradeReach(lTemp,-1,true);
// 					}
// 					else
// 					{
// 						bool bLast = false;
// 						for(unsigned int i = 0;i < lTradeVec.size();i++ )
// 						{	
// 							if(i == lTradeVec.size() - 1) bLast = true;
// 							strcpy(lTradeVec[i].Account,lTradeVec[i].InvestorID);
// 							lIterUser->second->QryTradeReach(lTradeVec[i],0,bLast);
// 						}
// 					}
// 					////װ�ر���
// 					//vector<PlatformStru_OrderInfo> lOrderVec;
// 					//mpDBOpr->LoadTodayUserOrders("TRADEDATA_ORDERS",lIter->szAccount,nsDay,lOrderVec);
// 					//if(lOrderVec.size() == 0)
// 					//{
// 					//	PlatformStru_OrderInfo lTemp;
// 					//	lIterUser->second->QryOrderReach(lTemp,-1,true);
// 					//}
// 					//else
// 					//{
// 					//	bool bLast = false;
// 					//	for(unsigned int i = 0;i < lOrderVec.size();i++ )
// 					//	{	
// 					//		if(i == lOrderVec.size() - 1) bLast = true;
// 					//		strcpy(lOrderVec[i].szAccount,lOrderVec[i].InvestorID);
// 					//		lIterUser->second->QryOrderReach(lOrderVec[i],0,bLast);
// 					//	}
// 					//}//else
// 
// 				}//if(lIterUser != mMapUser2TradedData.end())
// 			}//if(lIter->nUserType == USER_TYPE_TRADER)
// 		}//for(;lIter != lVecUser.end();lIter++)
// 	}//if(nOption != SettlementLoadOption_TradingDataWithCTPSett)
// 	else
// 	{	
// 		//��ctp��������ݾ�����
// 		mbDirty = true;
// 
// 		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();
// 		for(;lIter != mMapUser2TradingData.end();lIter++)
// 		{
// 			lIter->second->SetUserCTPStatus(ACCOUNT_STATUS_UnInit);
// 			//lIter->second->SetEqualUserInitStatus(USER_STATUS_UnInit);
// 		}
// 		//���͵�¼�������������
// 		Stru_NotifyEvent lDataEvent(EventTypeNeedAllLoginSettlement,0);		
// 		CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 
// 		vector<TradeAccount> lVecAccount;
// 		CInterface_SvrBrokerInfo::getObj().GetTradeAccounts(lVecAccount);
// 		vector<TradeAccount>::iterator lIterAcc = lVecAccount.begin();
// 		for(;lIterAcc != lVecAccount.end();lIterAcc++)
// 		{
// 			BrokerInfo lBrokeInfo;
// 			CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lIterAcc->nBrokerID, lBrokeInfo);		
// 
// 			//���Ͳ�ɽ�������
// 			Stru_NotifyEvent lDataEvent(EventTypeNeedQryTrader,0);
// 			BrokerAccountKey lsBrokerAccount(lBrokeInfo.szBrokerCode,lIterAcc->szTradeAccount);		
// 			lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
// 			CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 			
// 		}	
// 		//ctp��ɽ�����������ö�ʱcheck״̬
// 		int lnCount = 0;
// 		
// 		do 
// 		{
// 			int lnFinishNum = 0;
// 			Sleep(1000);
// 			_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();
// 			for(;lIter != mMapUser2TradingData.end();lIter++)
// 			{
// 				int nUserID = -1;
// 				vector<UserInfo> lVecUeser;
// 				CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUeser);
// 				for(unsigned int i = 0;i < lVecUeser.size();i++)
// 				{
// 					if(strcmp(lVecUeser[i].szAccount,lIter->first.c_str()) == 0)
// 					{
// 						nUserID = lVecUeser[i].nUserID;
// 						break;
// 					}
// 				}
// 				TradeAccount tradeAccount;
// 				if(!CInterface_SvrUserOrg::getObj().GetTradeAccountByUserID(nUserID,tradeAccount))			
// 				{
// 					TRADE_LOG("SettlementInitTradeDataWithTradingDB User %s Finished with Empty Trader Account",lIter->first.c_str());
// 					lnFinishNum++;
// 				}
// 				else if(lIter->second->GetUserInitStatus() & USER_STATUS_InitQryTradesReady)
// 				{
// 					TRADE_LOG("SettlementInitTradeDataWithTradingDB Uesr %s qry trader success,we need success it",lIter->first.c_str());
// 					//һ���û��Ĳ�����
// 					lnFinishNum++;
// 					PushEventToAccountWithUser(EventTypeNeedLogoffSettlement,lIter->first);					
// 				}
// 				else
// 				{
// 					//���κ�һ��״̬ͣ��20s���ϣ�����Ϊ����
// 					double ldifftime = 0.0f;
// 					EnumAccountStatus lAcStatus = lIter->second->GetUserCTPStatus(ldifftime);
// 					//����3��״̬��ͣ��20s��ʱ�䣬����Ϊ�ǹ̶�״̬
// 					if( (lAcStatus== ACCOUNT_STATUS_CannotConnect ||
// 						lAcStatus == ACCOUNT_STATUS_LoginFailure ||
// 						lAcStatus == ACCOUNT_STATUS_Disconnected) &&
// 						ldifftime > 20)
// 					{
// 						TRADE_LOG("SettlementInitTradeDataWithTradingDB Uesr %s sit this status %d too long,we need failed it",lIter->first.c_str(),lAcStatus);
// 						lnFinishNum++;
// 						PushEventToAccountWithUser(EventTypeNeedLogoffSettlement,lIter->first);		
// 					}
// 					/*if( ldifftime > 20)
// 					{		
// 						lnFinishNum++;
// 						PushEventToAccountWithUser(EventTypeNeedLogoffSettlement,lIter->first);			
// 					}*/
// 				
// 				}
// 			}
// 
// 			//���е��û���������
// 			if(lnFinishNum == mMapUser2TradingData.size())
// 				break;
// 
// 			lnCount++;
// 		} while (lnCount <= 30*60);
// 
// 	}

	

	//meUserTradedDataWithDBReady = DataReady_Ready;
	return CF_ERROR_SUCCESS;
}

//��������
CF_ERROR  CReal_SvrTradeData::EndSettlement(const std::string & nsDay)
{
	if(GetTraderDataStatus() != TradeDataStatusSettlemting)
	{
		TRADE_LOG("SettlementInitTradeDataWithTradingDB Not Settlemnt Status,discard this call");
		return CF_ERROR_NOINITED;
	}

	/*_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();
	for(;lIter != mMapUser2TradingData.end();lIter++)
		lIter->second->;*/

	//PUSH_LOG_ERROR(Logger,false,true,"������ɣ�����������������״̬");
	SetTraderDataStatus(TradeDataStatusSettlemtEnd);

	//�����������
// 	if(!nsDay.empty())
// 		mpTradingDayManager.AddSettlementedDay(nsDay);
// 	
// 
// 	//����ͬ��֪ͨ����ģ��ֹͣ����
// 	Stru_NotifyEvent lDataEvent(EventTypeEndSettlement,0);
// 	CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 
// 	////����Ƿ���δ����Ľ�����
// 	std::string lsOldestInitDayWithNoSett;
// 	mpTradingDayManager.HasInitTradingDayWithNoSettlement(lsOldestInitDayWithNoSett);
// 	if(!lsOldestInitDayWithNoSett.empty())
// 	{		
// 		SetCurrentTradingDay(lsOldestInitDayWithNoSett,true);
// 		PUSH_LOG_ERROR(Logger,false,true,"������ɣ���Ȼ�д�����Ľ�����");
// 	//	SetTraderDataStatus(TradeDataStatusLastDaySettlemtNotOK);
// 		TRADE_LOG("EndSettlement with no settlement is %s, set trading status to TradeDataStatusLastDaySettlemtNotOK ",\
// 			lsOldestInitDayWithNoSett.c_str());
// 	}
// 	else
// 	{
// 		std::string lsNewestSettlementedDay;
// 		mpTradingDayManager.GetNewestSettlementDay(lsNewestSettlementedDay);
// 		SetCurrentTradingDay(lsNewestSettlementedDay,true);
// 		//	SetTraderDataStatus(TradeDataStatusLastDaySettlemtNotOK);
// 		TRADE_LOG("EndSettlement with no day need to settlement");
// 	}


	return CF_ERROR_SUCCESS;
}

//�����޸ĺ���������ʺͱ�֤���ʣ��������ɽ��ͳֲֺ��ʽ�ļ���
CF_ERROR  CReal_SvrTradeData::SetModifiedCommissionAndMargin( std::vector<PlatformStru_InstrumentCommissionRate> & inCommssion,
												  std::vector<PlatformStru_InstrumentMarginRate> & inMargin) 
{
	if(GetTraderDataStatus() != TradeDataStatusSettlemting)
	{
		TRADE_LOG("Not Settlemnt Status,discard this call");
		return CF_ERROR_NOINITED;
	}

	//�����޸��˷��ʺ����ݾ�����
	mbDirty = true;

	CSafeRwLockRead lwlock(mrwLockMapAccount);
	//�����¼�޸ĵ�ί�н����˺�
	std::set<BrokerAccountKey> lModifySet;
	//�������÷���
	for(unsigned int i = 0; i < inCommssion.size();i++)
	{
		BrokerAccountKey lKey(inCommssion[i].BrokerID,inCommssion[i].InvestorID);
		_MapAccountData::iterator lIter = mMapAccountData.find(lKey);
		if(lIter != mMapAccountData.end())
			lIter->second->SetCommissionRate(inCommssion[i].InstrumentID,inCommssion[i]);
		lModifySet.insert(lKey);
	}
	for(unsigned int i = 0; i < inMargin.size();i++)
	{
		BrokerAccountKey lKey(inMargin[i].BrokerID,inMargin[i].InvestorID);
		_MapAccountData::iterator lIter = mMapAccountData.find(lKey);
		if(lIter != mMapAccountData.end())
			lIter->second->SetMarginRate(inMargin[i].InstrumentID,inMargin[i]);
		lModifySet.insert(lKey);
	}

	////���¼����޸ĵ�ί�н����˺ŵĶ�Ӧ���û��ĳɽ����ݣ���һ��ȫ��ȫ��Ķ���
	//std::set<BrokerAccountKey>::iterator lIterAccount = lModifySet.begin();
	//for(;lIterAccount != lModifySet.end();lIterAccount++)
	//{
	//	vector<UserInfo> lvecUser;
	//	CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(lIterAccount->AccountID,
	//		lIterAccount->BrokerID,lvecUser);
	//	vector<UserInfo>::iterator lIter = lvecUser.begin();
	//	for(;lIter != lvecUser.end();lIter++)
	//	{
	//		_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(lIter->szAccount);
	//		if(lIterUser != mMapUser2TradingData.end())
	//		{
	//			std::vector<PlatformStru_TradeInfo> lTemp;
	//			lIterUser->second->RecalculateTrade(lTemp,false);
	//		}
	//	}	

	//}


	return CF_ERROR_SUCCESS;
}

//�����޸ĺ�ĳɽ����������ɽ��ͳֲֺ��ʽ�ļ���
CF_ERROR  CReal_SvrTradeData::SeUserNewTrades(const std::string & nUserName,
											  const std::vector<PlatformStru_TradeInfo> & inTradeInfo)
{
	if(GetTraderDataStatus() != TradeDataStatusSettlemting)
	{
		TRADE_LOG("SeUserNewTrades Not Settlemnt Status,discard this call");
		return CF_ERROR_NOINITED;
	}

	//�����޸��˳ɽ������ݾ�����
	mbDirty = true;
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);

	_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nUserName);
	if(lIterUser != mMapUser2TradingData.end())
	{
		//������û��ĳֲ���Ϣ
		lIterUser->second->ClearTraderAndPosition();
	
		//���¼����ճ��ֲ�
		std::string lsTempTradingDay = msInstrumentDay.substr(0,4) + msInstrumentDay.substr(5,2) + msInstrumentDay.substr(8,2);	
        DispathInitialDataToUser(lIterUser->first,lsTempTradingDay,mMapUser2TradingData);

		//���¼���ɽ�
		lIterUser->second->RecalculateTrade(inTradeInfo,true);
	}

	return CF_ERROR_SUCCESS;
}




//�µ��õĺ�������Ҫ�ڽ���״̬��ִ��
CF_ERROR CReal_SvrTradeData::CheckUserInstrumentCommissionAndMaginExist(const std::string & nUserName,
															const std::string & nIstrumentID) 
{
	if(GetTraderDataStatus() != TradeDataStatusTrading )
	{
		//TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}

	int lnRet = CF_ERROR_TRADEDATA_NO_USER_DATA;
	do 
	{
		CSafeRwLockRead lrlock(mrwLockMapUserTrading);
		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserName);
		if(lIter ==  mMapUser2TradingData.end())
			break;
		
		PlatformStru_InstrumentCommissionRate lTempCom;
		if(0 != lIter->second->GetCommissionRate(nIstrumentID,lTempCom))
		{
			lnRet = CF_ERROR_TRADEDATA_NO_COMMISSION;
			break;
		}

		PlatformStru_InstrumentMarginRate lTempMar;
		if(0 != lIter->second->GetMarginRate(nIstrumentID,lTempMar))
		{
			lnRet = CF_ERROR_TRADEDATA_NO_MARGIN;
			break;
		}

		lnRet = CF_ERROR_SUCCESS;

		
	} while (false);
	
	return lnRet;
}

//�ж��Ƿ�ɿ���
CF_ERROR  CReal_SvrTradeData::CanOrderOpen(const PlatformStru_OrderInfo & nOrder) 
{
	if(GetTraderDataStatus() != TradeDataStatusTrading )
	{
	//	TRADE_LOG("Init Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	IInterfaceUserOp * lpUserOp = NULL;
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nOrder.InvestorID);
	if(mMapUser2TradingData.end() != lIter)
	{	
		lpUserOp = lIter->second;
	}

	if(lpUserOp == NULL)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;		

	//����ǿ��֣��жϿ����ʽ�
	PlatformStru_InstrumentInfo lInstruemntInfo;
	if(mpPlatformDataInstrument == NULL ||
		false == mpPlatformDataInstrument->GetInstrumentInfo(nOrder.InstrumentID,lInstruemntInfo))
		return CF_ERROR_TRADEDATA_NO_INSTRUMENTS;

	if(lInstruemntInfo.VolumeMultiple == 0 )
		return CF_ERROR_TRADEDATA_NO_INSTRUMENTS;

	//��ȡ��������
	PlatformStru_InstrumentCommissionRate lCommission;
	if(	0 != lpUserOp->GetCommissionRate(nOrder.InstrumentID,lCommission))
		return CF_ERROR_TRADEDATA_NO_COMMISSION;

	if(util::isInvalidValue(lCommission.OpenRatioByMoney) ||
		util::isInvalidValue(lCommission.OpenRatioByVolume))
		return CF_ERROR_TRADEDATA_NO_COMMISSION;

	//��ȡ��֤����
	PlatformStru_InstrumentMarginRate lMargin;
	if(0 != lpUserOp->GetMarginRate(nOrder.InstrumentID,lMargin))
		return CF_ERROR_TRADEDATA_NO_MARGIN;

	if(util::isInvalidValue(lMargin.LongMarginRatioByMoney) ||
		util::isInvalidValue(lMargin.LongMarginRatioByVolume) ||
		util::isInvalidValue(lMargin.ShortMarginRatioByMoney) ||
		util::isInvalidValue(lMargin.ShortMarginRatioByVolume) )
		return CF_ERROR_TRADEDATA_NO_MARGIN;

	//������мۣ�ȡ����
	double ldbOrderPrice = 0.0;
	if(nOrder.OrderPriceType == THOST_FTDC_OPT_AnyPrice)
	{
		PlatformStru_DepthMarketData lQuto;
		GetQuotInfo(nOrder.InstrumentID,lQuto);
		if(util::isInvalidValue(lQuto.LastPrice))
			return CF_ERROR_TRADEDATA_NO_QUTO;

		ldbOrderPrice = lQuto.LastPrice;
	}
	else
		ldbOrderPrice = nOrder.LimitPrice;

	//�ж϶�ͷ�Ϳ�ͷ
	double ldbNeedMoney = 0.0;
	if(nOrder.Direction == THOST_FTDC_D_Sell)
	{
		ldbNeedMoney = ldbOrderPrice * lInstruemntInfo.VolumeMultiple * lMargin.ShortMarginRatioByMoney+
			lMargin.ShortMarginRatioByVolume +
			ldbOrderPrice * lInstruemntInfo.VolumeMultiple * lCommission.OpenRatioByMoney +
			lCommission.OpenRatioByVolume;


	}
	else
	{
		ldbNeedMoney = ldbOrderPrice * lInstruemntInfo.VolumeMultiple * lMargin.LongMarginRatioByMoney+
			lMargin.LongMarginRatioByVolume +
			ldbOrderPrice * lInstruemntInfo.VolumeMultiple * lCommission.OpenRatioByMoney +
			lCommission.OpenRatioByVolume;

	}
	ldbNeedMoney *= nOrder.VolumeTotalOriginal;


	//ȡ�����ʽ�
	PlatformStru_TradingAccountInfo lAccountInfo;
    lpUserOp->GetFundInfo(lAccountInfo); 
	if(util::isInvalidValue(lAccountInfo.Available) ||
		lAccountInfo.Available <= 0.0 ||
		lAccountInfo.Available < ldbNeedMoney)
		return CF_ERROR_TRADEDATA_NOT_ENOUGH_FUND;

	return CF_ERROR_SUCCESS;
}

//�ж��Ƿ��ƽ��
CF_ERROR  CReal_SvrTradeData::CanOrderColseToday(const PlatformStru_OrderInfo & nOrder)
{
	if(GetTraderDataStatus() != TradeDataStatusTrading )
	{
		//TRADE_LOG("Init Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	IInterfaceUserOp * lpUserOp = NULL;
	_MapUserTradeData::iterator lIterUser =  mMapUser2TradingData.find(nOrder.InvestorID);
	if(mMapUser2TradingData.end() != lIterUser)
	{	
		lpUserOp = lIterUser->second;
	}

	if(lpUserOp == NULL)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;	

	std::vector<PlatformStru_Position> lVecPosition;
	TThostFtdcVolumeType lVol = 0;
	lpUserOp->GetPositions(nOrder.InstrumentID,lVecPosition);
	std::vector<PlatformStru_Position>::iterator lIter = lVecPosition.begin();
	while(lIter != lVecPosition.end())
	{
		if( lIter->HedgeFlag == nOrder.CombHedgeFlag[0] &&
			( ( lIter->PosiDirection == THOST_FTDC_PD_Long && nOrder.Direction == THOST_FTDC_D_Sell) ||
			( lIter->PosiDirection == THOST_FTDC_PD_Short && nOrder.Direction == THOST_FTDC_D_Buy)) )
		{
			lVol = lIter->CanCloseTodayVolume ;
			break;
		}
		lIter++;
	}

	if(lVol < nOrder.VolumeTotalOriginal)
		return CF_ERROR_TRADEDATA_NOT_ENOUGH_VOLOMN; //

	return CF_ERROR_SUCCESS;

}

//�ж��Ƿ��ƽ��
CF_ERROR  CReal_SvrTradeData::CanOrderColse(const PlatformStru_OrderInfo & nOrder)
{
	if(GetTraderDataStatus() != TradeDataStatusTrading )
	{
		//TRADE_LOG("Init Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	IInterfaceUserOp * lpUserOp = NULL;
	_MapUserTradeData::iterator lIterUser =  mMapUser2TradingData.find(nOrder.InvestorID);
	if(mMapUser2TradingData.end() != lIterUser)
	{	
		lpUserOp = lIterUser->second;
	}

	if(lpUserOp == NULL)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;	

	std::vector<PlatformStru_Position> lVecPosition;
	TThostFtdcVolumeType lVol = 0;
	lpUserOp->GetPositions(nOrder.InstrumentID,lVecPosition);
	std::vector<PlatformStru_Position>::iterator lIter = lVecPosition.begin();
	while(lIter != lVecPosition.end())
	{
		if( lIter->HedgeFlag == nOrder.CombHedgeFlag[0] &&
			( ( lIter->PosiDirection == THOST_FTDC_PD_Long && nOrder.Direction == THOST_FTDC_D_Sell) ||
			( lIter->PosiDirection == THOST_FTDC_PD_Short && nOrder.Direction == THOST_FTDC_D_Buy)) )
		{
			lVol = lIter->CanCloseVolume ;
			break;
		}
		lIter++;
	}

	if(lVol < nOrder.VolumeTotalOriginal)
		return CF_ERROR_TRADEDATA_NOT_ENOUGH_VOLOMN; //

	return CF_ERROR_SUCCESS;
}




//�����
CF_ERROR CReal_SvrTradeData::FundInOut(const sFundInOut& nFundInOut) 
{
	if(GetTraderDataStatus() != TradeDataStatusTrading )
	{
	//	TRADE_LOG("error Status,discard this call");
		return CF_ERROR_TRADEDATA_FUNDINOUT_DENY;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nFundInOut.mUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_FUNDIN_DENY_NOINIT;

	IInterfaceUserOp * lpTemp = lIter->second;
	CF_ERROR lRet = lpTemp->UserFundInOut(nFundInOut);
// 	if(lRet == CF_ERROR_SUCCESS && mpDBOpr)
// 	{
// 
// 		//�����ʽ�仯���¼�
// 		Stru_NotifyEvent lsFund(EventTypeFund,0);
// 		lsFund.AddBufer((void*)lIter->first.c_str(),lIter->first.length() );	
// 		CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsFund);			
// 
// 		if(!CInterface_SvrLogin::getObj().IsSlaver())
// 		{
// 			double ldbVal = 0.0f;
// 			lpTemp->GetFirstFund(ldbVal);
// 			//���û��ĳ�ʼ�ʽ�
// 			mpDBOpr->WriteInitFund("TradeData_InitFund",lIter->first,ldbVal);
// 
// 			//������
// 			mpDBOpr->WriteFundInOut(nFundInOut);
// 			//���ʽ�		
// 			PlatformStru_TradingAccountInfo lTempTrade;
// 			if(CF_ERROR_SUCCESS == lpTemp->GetFundInfo(lTempTrade))
// 			{			
// 				std::string lsTempTradingDay;
// 				GetCurrentTradingDay(lsTempTradingDay);
// 				mpDBOpr->WriteFundInfos("TradeData_FundInfo",lIter->first,lsTempTradingDay,lTempTrade);
// 			}
// 		}


// 	}
	return lRet;


}
//��ѯ��ʷ�����
CF_ERROR CReal_SvrTradeData::QryHistroyFundInOut(const sQryFundInOut & nQryFund,
									 std::vector<sFundInOut> & nvecFund) 
{
	return 0/*mpDBOpr->LoadQryFundInOut(nQryFund,nvecFund)*/;
}
//�����û���ʼ�ʽ� ���Ա��ص��� 
CF_ERROR CReal_SvrTradeData::SetUserInitFund(const double dInitFund)
{
    _MapUserTradeData::iterator lIterUser =  mMapUser2TradingData.begin();
    for(;lIterUser != mMapUser2TradingData.end();lIterUser++)
    {
        lIterUser->second->SetInitFund(dInitFund);
    }

    return CF_ERROR_SUCCESS;
}


//��ѯ����
//check �û��Ƿ��ܹ����е�¼��ע��
CF_ERROR CReal_SvrTradeData::ResponseToAsk(const eAskType& neDataType,
								 const eAskOperationType& neChangeType,
								 const int nllDataID,
								 const char * npDataBuf)
{
	CF_ERROR lRet = CF_ERROR_LOGIN_USER_DATA_NOTREADY;
	
// 	if(neDataType == AskType_Login)
// 	{
// 		//��¼�¼�
// 		if(neChangeType == AskOperationTypeAdd)
// 		{  
// 			int lnUserID = (int)npDataBuf;
// 			if(lnUserID == SLAVE_USER_ID)
// 			{
// 				//�ӷ�������½����������������ڽ���״̬
// 				if(GetTraderDataStatus() != TradeDataStatusTrading )
// 					return CF_ERROR_LOGIN_TADRE_STATUS_INIT;
// 
// 				return CF_ERROR_SUCCESS;
// 			}
// 
// 			UserInfo lUserInfo;
// 			if(false == CInterface_SvrUserOrg::getObj().GetUserByID(lnUserID,lUserInfo) )
// 				return CF_ERROR_LOGIN_INVALIDE_USERID;
// 			if(lUserInfo.nUserType == USER_TYPE_TRADER)
// 			{				
// 
// 				if(GetTraderDataStatus() == TradeDataStatusInitialing ||
// 					GetTraderDataStatus() == TradeDataStatusNoTradingDay)
// 					return CF_ERROR_LOGIN_TADRE_STATUS_INIT;
// 				if(GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK )
// 					return CF_ERROR_LOGIN_TADRE_STATUS_NOSETT;
// 				if(GetTraderDataStatus() == TradeDataStatusSettlemting ||
// 					GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
// 					return CF_ERROR_LOGIN_TADRE_STATUS_SETT;
// 
// 				if(GetTraderDataStatus() == TradeDataStatusSettlemtEnd)
// 					return CF_ERROR_LOGIN_TADRE_STATUS_END;
// 
// 				if(GetTraderDataStatus() == TradeDataStatusInitFail)
// 					return CF_ERROR_LOGIN_TADRE_STATUS_INITFAIL;
// 
// 				if(npDataBuf)
// 				{					
// 					if(GetUserInitStatus(lnUserID) == USER_STATUS_Inited)
// 					{
// 						lRet = CF_ERROR_SUCCESS;
// 					}
// 				}
// 			}
// 			else if(lUserInfo.nUserType == USER_TYPE_MANAGE)
// 			{
// 				if(GetTraderDataStatus() == TradeDataStatusInitialing)
// 					return CF_ERROR_LOGIN_TADRE_STATUS_INIT;
// 
// 
// 				lRet = CF_ERROR_SUCCESS;
// 			}
// 			else if(lUserInfo.nUserType == USER_TYPE_RISK_CONTROL ||
// 				lUserInfo.nUserType == USER_TYPE_VIEWER)
// 			{
// 				if(GetTraderDataStatus() == TradeDataStatusInitialing)
// 					return CF_ERROR_LOGIN_TADRE_STATUS_INIT;
// 
// 				lRet = CF_ERROR_SUCCESS;
// 			}
// 			
// 		}

// 	}	
// 	else if(neDataType == AskType_UserAndTradeAccount)
// 	{
// 		//ɾ�����޸�һ�����˻�
// 		if(neChangeType == AskOperationTypeDelete || neChangeType == AskOperationTypeUpdate)
// 		{
// 			UserAndTradeAccountRelation * ltempRelation = (UserAndTradeAccountRelation*)(npDataBuf);
// 			if(GetTraderDataStatus() != TradeDataStatusSettlemtEnd && 
// 				GetTraderDataStatus() != TradeDataStatusNoTradingDay &&
// 				GetTraderDataStatus() != TradeDataStatusInitFail &&
// 				GetTraderDataStatus() != TradeDataStatusLastDaySettlemtNotOK)
// 				return CF_ERROR_TRADEDATA_NOTMATCH_STATUS;
// 			else 
// 			{
				//������гֲֲ��ܸ�
// 				UserInfo lInfo;
// 				CInterface_SvrUserOrg::getObj().GetUserByID(ltempRelation->nUserID,lInfo);
// 				if(TestUserHasPosition(lInfo.szAccount))
// 					return CF_ERROR_TRADEDATA_HAS_POSITION;
// 				return CF_ERROR_SUCCESS;
// 			}
// 		}
// 		if(neChangeType == AskOperationTypeAdd)
// 		{
// 			UserAndTradeAccountRelation * ltempRelation = (UserAndTradeAccountRelation*)( npDataBuf);
// 			if(ltempRelation->nTradeAccountID == -1)
// 				return CF_ERROR_SUCCESS;
// 			//��ʼ��״̬�²�������
// 			if(GetTraderDataStatus() == TradeDataStatusInitialing ||
//                GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
// 					return CF_ERROR_TRADEDATA_INITING;
// 
// 			if(GetTraderDataStatus() == TradeDataStatusSettlemting)
// 				return CF_ERROR_LOGIN_TADRE_STATUS_SETT;
// 
// 			lRet = CF_ERROR_SUCCESS;
// 		}
// 	
// 	}
// 	else if(neDataType == AskType_OrgAndStrategy || 
// 			neDataType == AskType_TraderAndProduct ||
// 			neDataType == AskType_UserAndOrg ||
// 			neDataType == AskType_BrokerAndServerGroup ||
// 			neDataType == AskType_BrokerAndTradeAccount ||
// 			neDataType == AskType_BrokerAndServerGroupAddress ||
// 			neDataType == AskType_User ||
// 			neDataType == AskType_Orgnazation ||
// 			neDataType == AskType_Broker ||
// 			neDataType == AskType_ServerGroup ||
// 			neDataType == AskType_ServerAddress ||
// 			neDataType == AskType_FinacialProduct ||
// 			neDataType == AskType_TradeStrategy)
// 	{
// 		//ɾ�����޸�һ�����˻�
// 		if(neChangeType == AskOperationTypeDelete || neChangeType == AskOperationTypeUpdate)
// 		{
// 			
// 			if(GetTraderDataStatus() != TradeDataStatusSettlemtEnd && 
// 				GetTraderDataStatus() != TradeDataStatusNoTradingDay &&
// 				GetTraderDataStatus() != TradeDataStatusInitFail &&
// 				GetTraderDataStatus() != TradeDataStatusLastDaySettlemtNotOK)
// 				return CF_ERROR_TRADEDATA_NOTMATCH_STATUS;
// 			return CF_ERROR_SUCCESS;			
// 		}
// 		if(neChangeType == AskOperationTypeAdd)
// 		{			
// 			//��ʼ��״̬�²�������
// 			if(GetTraderDataStatus() == TradeDataStatusInitialing ||
// 				GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
// 				return CF_ERROR_TRADEDATA_INITING;
// 
// 			if(GetTraderDataStatus() == TradeDataStatusSettlemting)
// 				return CF_ERROR_LOGIN_TADRE_STATUS_SETT;
// 
// 			lRet = CF_ERROR_SUCCESS;
// 		}
// 
// 	}
// 	else if (neDataType == AskType_Account)
// 	{
// 		//ɾ�����޸�һ�����˻�
// 		if(neChangeType == AskOperationTypeDelete || neChangeType == AskOperationTypeUpdate)
// 		{
// 			//int lAccountID = nllDataID;		
			//TradeAccount lAccountInfo;
			//CInterface_SvrBrokerInfo::getObj().GetTradeAccount(lAccountID,lAccountInfo);

			//BrokerInfo lBroker;
			//CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lAccountInfo.nBrokerID,lBroker);

			//BrokerAccountKey  lpKey(lBroker.szBrokerCode,lAccountInfo.szTradeAccount);
			//EnumAccountStatus lsStatus = ACCOUNT_STATUS_UnInit;
			//double ldbTime;
			////ֻ�и��˻���½ʧ�ܻ��߲������ӵ�ʱ�����ɾ�����޸�
			//GetAccountStatus(lAccountInfo.szTradeAccount,lBroker.szBrokerCode,lsStatus,ldbTime);
			//if(lsStatus == ACCOUNT_STATUS_Login )
			//	return CF_ERROR_TRADEDATA_ACCOUNT_ISLOGIN;		
			//else
			//	return CF_ERROR_SUCCESS;
// 		}
// 		if(neChangeType == AskOperationTypeAdd)
// 		{			
// 			//��ʼ��״̬�²�������
// 			if(GetTraderDataStatus() == TradeDataStatusInitialing ||
// 				GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
// 				return CF_ERROR_TRADEDATA_INITING;
// 
// 			if(GetTraderDataStatus() == TradeDataStatusSettlemting)
// 				return CF_ERROR_LOGIN_TADRE_STATUS_SETT;
// 
// 			lRet = CF_ERROR_SUCCESS;
// 		}
// 
// 	}
// 	else if(AskType_UserLoginCheck == neDataType)
// 	{

		/*lRet = CF_ERROR_TRADEDATA_USER_LOGIN;
		int lUserID = nllDataID;
		std::vector<SOCKET> lvecSocket;
		CInterface_SvrLogin::getObj().GetUserSockets(lUserID,lvecSocket);
		if(lvecSocket.size() == 0)
			lRet = CF_ERROR_SUCCESS;*/
// 	}
	return lRet;

}

//��ȡ��ʼ��ʱ��
CF_ERROR CReal_SvrTradeData::GetReinitTime(sReinitTime & nsReinitTime)
{
	char lsPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,lsPath,MAX_PATH);
	*(strrchr(lsPath,'\\') + 1)= NULL;
	strcat(lsPath,TRADEDATA_CONFIG_FILE);
	
	GetPrivateProfileString(TRADEDATA_KEY,"NightTime","20:30:30",nsReinitTime.mNightTime,sizeof(nsReinitTime.mNightTime),lsPath);
	GetPrivateProfileString(TRADEDATA_KEY,"DayTime","08:30:30",nsReinitTime.mDayTime,sizeof(nsReinitTime.mNightTime),lsPath);
	return CF_ERROR_SUCCESS;
}

//��ȡ��ʼ��ʱ��
CF_ERROR CReal_SvrTradeData::GetSaveTime(sSaveTime & nsSaveTime)
{
	char lsPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,lsPath,MAX_PATH);
	*(strrchr(lsPath,'\\') + 1)= NULL;
	strcat(lsPath,TRADEDATA_CONFIG_FILE);

	GetPrivateProfileString(TRADEDATA_KEY,"SaveTime1","11:30:30",nsSaveTime.mSaveTime1,sizeof(nsSaveTime.mSaveTime1),lsPath);
	GetPrivateProfileString(TRADEDATA_KEY,"SaveTime2","15:20:30",nsSaveTime.mSaveTime2,sizeof(nsSaveTime.mSaveTime2),lsPath);
	return CF_ERROR_SUCCESS;
}

//���ó�ʼ��ʱ��
CF_ERROR CReal_SvrTradeData::SetReinitTime(const sReinitTime & nsReinitTime)
{

	std::string lsNightTime = nsReinitTime.mNightTime;
	std::string lsDayTime = nsReinitTime.mDayTime;
	char lsPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,lsPath,MAX_PATH);
	*(strrchr(lsPath,'\\') + 1)= NULL;
	strcat(lsPath,TRADEDATA_CONFIG_FILE);

	WritePrivateProfileString(TRADEDATA_KEY,"NightTime",lsNightTime.c_str(),lsPath);
	WritePrivateProfileString(TRADEDATA_KEY,"DayTime",lsDayTime.c_str(),lsPath);
	return CF_ERROR_SUCCESS;
}

//��ȡ����ʱ��
CF_ERROR CReal_SvrTradeData::GetTradingTime(sTradingTime & nsTradingTime) 
{
	char lsPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,lsPath,MAX_PATH);
	*(strrchr(lsPath,'\\') + 1)= NULL;
	strcat(lsPath,TRADEDATA_CONFIG_FILE);

	GetPrivateProfileString(TRADEDATA_KEY,"TradingDayStart","08:55:00",nsTradingTime.mTradingDayStart,sizeof(nsTradingTime.mTradingDayStart),lsPath);
	GetPrivateProfileString(TRADEDATA_KEY,"TradingDayEnd","15:15:00",nsTradingTime.mTradingDayEnd,sizeof(nsTradingTime.mTradingDayEnd),lsPath);

	GetPrivateProfileString(TRADEDATA_KEY,"TradingNigthStart","20:00:00",nsTradingTime.mTradingNightStart,sizeof(nsTradingTime.mTradingNightStart),lsPath);
	GetPrivateProfileString(TRADEDATA_KEY,"TradingNightEnd","02:00:00",nsTradingTime.mTradingNightEnd,sizeof(nsTradingTime.mTradingNightEnd),lsPath);
	
	
	return CF_ERROR_SUCCESS;
}


//���ó�ʼ��ʱ��
CF_ERROR CReal_SvrTradeData::SetTradingTime(const sTradingTime & nsTradingTime)
{

	std::string lsTradingDayStart = nsTradingTime.mTradingDayStart;
	std::string lsTradingDayEnd = nsTradingTime.mTradingDayEnd;
	std::string lsTradingNightStart = nsTradingTime.mTradingNightStart;
	std::string lsTradingNightEnd = nsTradingTime.mTradingNightEnd;	
	
	char lsPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,lsPath,MAX_PATH);
	*(strrchr(lsPath,'\\') + 1)= NULL;
	strcat(lsPath,TRADEDATA_CONFIG_FILE);

	WritePrivateProfileString(TRADEDATA_KEY,"TradingDayStart",lsTradingDayStart.c_str(),lsPath);
	WritePrivateProfileString(TRADEDATA_KEY,"TradingDayEnd",lsTradingDayEnd.c_str(),lsPath);
	WritePrivateProfileString(TRADEDATA_KEY,"TradingNigthStart",lsTradingNightStart.c_str(),lsPath);
	WritePrivateProfileString(TRADEDATA_KEY,"TradingNightEnd",lsTradingNightEnd.c_str(),lsPath);

	return CF_ERROR_SUCCESS;
}

////���һ��CTP����
CF_ERROR CReal_SvrTradeData::QryCTPOrder(PlatformStru_OrderInfo & nOrderInfo,const int ErrID,const bool bIsLast) 
{
	if(GetTraderDataStatus() != TradeDataStatusInitialing)
	{
		//TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	//if(!CInterface_SvrLogin::getObj().IsSlaver() && GetTraderDataStatus() == TradeDataStatusTrading)
	//{
	//	//��ӷ�����������
	//	vector<SOCKET> lVecSocket;
	//	CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
	//	for(unsigned int i = 0; i < lVecSocket.size() ; i++)
	//	{
	//		TRADE_LOG("Master push a QryCTPOrder to SLaver %d",lVecSocket[i]);
	//		CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
	//			Cmd_TradeDataManager_MasterQryCTPOrder_Push,
	//			&nOrderInfo,
	//			sizeof(nOrderInfo),
	//			0,
	//			0,
	//			ErrID,
	//			bIsLast,
	//			CF_ERROR_SUCCESS,0);
	//	}
	//}
	CSafeRwLockRead lrlock(mrwLockMapAccount);

	//��ӵ��û�����ģ��ȥ
	CF_ERROR lRet = CF_ERROR_TRADEDATA_NO_USER_DATA;
	BrokerAccountKey lKey(nOrderInfo.BrokerID,nOrderInfo.InvestorID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{
		IInterfaceAccountOp * lpTemp = lIter->second;	
		lRet = lpTemp->QryOrderReach(nOrderInfo,ErrID,bIsLast);	
	}

	/*if(CInterface_SvrLogin::getObj().IsSlaver())
		mBufferDataStoreCTPOrder.Push(nOrderInfo,BufferStoreCallback);*/
	return CF_ERROR_SUCCESS;
}

//���һ��CTP�ɽ�
CF_ERROR CReal_SvrTradeData::QryCTPTrader(PlatformStru_TradeInfo & nTraderInfo,const int ErrID,const bool bIsLast) 
{
	if(GetTraderDataStatus() != TradeDataStatusInitialing)
	{
		//TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}

	//if(!CInterface_SvrLogin::getObj().IsSlaver() && GetTraderDataStatus() == TradeDataStatusTrading)
	//{
	//	//��ӷ�����������
	//	vector<SOCKET> lVecSocket;
	//	CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
	//	for(unsigned int i = 0; i < lVecSocket.size() ; i++)
	//	{
	//		TRADE_LOG("Master push a QryCTPTrader to SLaver %d",lVecSocket[i]);
	//		CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
	//			Cmd_TradeDataManager_MasterQryCTPTrader_Push,
	//			&nTraderInfo,
	//			sizeof(nTraderInfo),
	//			0,
	//			0,
	//			ErrID,
	//			bIsLast,
	//			CF_ERROR_SUCCESS,0);
	//	}
	//}

	CSafeRwLockRead lrlock(mrwLockMapAccount);

	//��ӵ��û�����ģ��ȥ
	CF_ERROR lRet = CF_ERROR_TRADEDATA_NO_USER_DATA;
	BrokerAccountKey lKey(nTraderInfo.BrokerID,nTraderInfo.InvestorID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{
		IInterfaceAccountOp * lpTemp = lIter->second;	
		lRet = lpTemp->QryTradeReach(nTraderInfo,ErrID,bIsLast);	
	}


	/*if(!CInterface_SvrLogin::getObj().IsSlaver())
		mBufferDataStoreCTPTrade.Push(nTraderInfo,BufferStoreCallback);*/
	return CF_ERROR_SUCCESS;
}


////���һ��CTP����
CF_ERROR CReal_SvrTradeData::PushCTPOrder(PlatformStru_OrderInfo & nOrderInfo) 
{
	if(GetTraderDataStatus() != TradeDataStatusTrading)
	{
		//TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	

	//��ӵ��û�����ģ��ȥ
	CF_ERROR lRet = CF_ERROR_TRADEDATA_NO_USER_DATA;
	BrokerAccountKey lKey(nOrderInfo.BrokerID,nOrderInfo.InvestorID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{
		IInterfaceAccountOp * lpTemp = lIter->second;	
		lRet = lpTemp->PushOrder(nOrderInfo);	
	}
	//if(CInterface_SvrLogin::getObj().IsSlaver())
	//	mBufferDataStoreCTPOrder.Push(nOrderInfo,BufferStoreCallback);
	//if(!CInterface_SvrLogin::getObj().IsSlaver() && GetTraderDataStatus() == TradeDataStatusTrading)
	//{
	//	//��ӷ�����������
	//	vector<SOCKET> lVecSocket;
	//	CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
	//	for(unsigned int i = 0; i < lVecSocket.size() ; i++)
	//	{
	//		TRADE_LOG("Master push a CTPOrder to SLaver %d",lVecSocket[i]);

	//		CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
	//			Cmd_TradeDataManager_MasterCTPOrder_Push,
	//			&nOrderInfo,
	//			sizeof(nOrderInfo),
	//			0,
	//			0,
	//			0,
	//			0,
	//			CF_ERROR_SUCCESS,0);
	//	}
	//}

	return CF_ERROR_SUCCESS;
}

//���һ��CTP�ɽ�
CF_ERROR CReal_SvrTradeData::PushCTPTrader(PlatformStru_TradeInfo & nTraderInfo) 
{
	if(GetTraderDataStatus() != TradeDataStatusTrading)
	{
		//TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	
	CSafeRwLockRead lrlock(mrwLockMapAccount);

	//��ӵ��û�����ģ��ȥ
	CF_ERROR lRet = CF_ERROR_TRADEDATA_NO_USER_DATA;
	BrokerAccountKey lKey(nTraderInfo.BrokerID,nTraderInfo.InvestorID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{
		IInterfaceAccountOp * lpTemp = lIter->second;	
		lRet = lpTemp->PushTrader(nTraderInfo);	
	}

	//if(!CInterface_SvrLogin::getObj().IsSlaver())
	//	mBufferDataStoreCTPTrade.Push(nTraderInfo,BufferStoreCallback);

	//if(!CInterface_SvrLogin::getObj().IsSlaver() && GetTraderDataStatus() == TradeDataStatusTrading)
	//{
	//	//��ӷ�����������
	//	vector<SOCKET> lVecSocket;
	//	CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
	//	for(unsigned int i = 0; i < lVecSocket.size() ; i++)
	//	{
	//		TRADE_LOG("Master push a CTPTrader to SLaver %d",lVecSocket[i]);

	//		CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
	//			Cmd_TradeDataManager_MasterCTPTrader_Push,
	//			&nTraderInfo,
	//			sizeof(nTraderInfo),
	//			0,
	//			0,
	//			0,
	//			0,
	//			CF_ERROR_SUCCESS,0);
	//	}
	//}
	return CF_ERROR_SUCCESS;
}


////���һ��ExecuteIn����
CF_ERROR CReal_SvrTradeData::PushExecuteInOrder(PlatformStru_OrderInfo & nOrderInfo) 
{
	if(GetTraderDataStatus() != TradeDataStatusTrading)
	{
		//TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	/*if(CInterface_SvrLogin::getObj().IsSlaver())
		mBufferDataStoreExecuteInOrder.Push(nOrderInfo,BufferStoreCallback);*/
	return CF_ERROR_SUCCESS;
}

////���һ��ExecuteOut����
CF_ERROR CReal_SvrTradeData::PushExecuteOutOrder(PlatformStru_OrderInfo & nOrderInfo) 
{
	if(GetTraderDataStatus() != TradeDataStatusTrading)
	{
		//TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	/*if(CInterface_SvrLogin::getObj().IsSlaver())
		mBufferDataStoreExecuteOutOrder.Push(nOrderInfo,BufferStoreCallback);*/
	return CF_ERROR_SUCCESS;
}


//��ȡ�������ݿ�Ľӿ�
//IInterfaceDBOpr * CReal_SvrTradeData::GetInterfaceDBOpr()
//{
//	return mpDBOpr;
//}
//���̲�ѯ���ɹ�����
void CReal_SvrTradeData::QryFailed(const BrokerAccountKey & nBrokerAccount,eEventType nEvent)
{
	TRADE_LOG("QryFailed event is %d",(int)nEvent);
	if(nEvent == EventTypeNeedQryInstrument)
	{
		Stru_NotifyEvent lDataEvent(EventTypeNeedQryInstrument,0);
		//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
	}
	else
	{
		Stru_NotifyEvent lDataEvent(nEvent,0);			
		BrokerAccountKey lsBrokerAccount(nBrokerAccount.BrokerID,nBrokerAccount.AccountID);		
		lDataEvent.AddBufer((void*)&nBrokerAccount,sizeof(nBrokerAccount));	
		//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
	}
	return;
}

//��ȡ��Ӧ�û���Ӧʱ��εĳֲ���ϸ
CF_ERROR CReal_SvrTradeData::GetPositionDetail_MaxTime(const  std::vector<std::string> nvecAccount, 
										   int LowerOpenTime,int UpperOpenTime,
										   std::map<std::string, std::vector<PlatformStru_PositionDetail>>& mapDetail) 
{
	if(GetTraderDataStatus() != TradeDataStatusTrading)
	{
		//TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	for(unsigned int i = 0; i < nvecAccount.size();i++)
	{		
		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nvecAccount[i]);
		if(mMapUser2TradingData.end() != lIter)
		{
			std::vector<PlatformStru_PositionDetail> lvecTemp;
			IInterfaceUserOp * lpTemp = lIter->second;
			lpTemp->GetPositionDetail(LowerOpenTime,UpperOpenTime,lvecTemp);
			if(lvecTemp.size() != 0)
				mapDetail[nvecAccount[i]] = lvecTemp;
		}	
	}
	return CF_ERROR_SUCCESS;

}
///***********************************�ڲ�����******************************************************************************/
//
///***********************************�ڲ�����******************************************************************************/
//�����Ͳ�ѯ���ʺͱ�֤����
void CReal_SvrTradeData::SendQryCommissionAndMargin()
{
//������������
	//������ݿ��н��������˻����������ʣ����ȡ���ݿ�
	//��������Щ�˻�����û��׼���ã����ͷ��ʲ�ѯ�¼�

	//����֤����
	//������ݿ��н��������˻��ı�֤���ʣ����ȡ���ݿ�
	//��������Щ�˻�����û��׼���ã����ͷ��ʲ�ѯ�¼�
	std::string lsTempTradingDay;
	GetCurrentTradingDay(lsTempTradingDay);
	TRADE_LOG("SendQryCommissionAndMargin %s",lsTempTradingDay.c_str());

	//ȥ�������ظ��ĺ�Լ
	std::set<string> lSetInstrumentId;
	mpPlatformDataInstrument->GetInstrumentList(lSetInstrumentId);
// 
// 	vector<TradeAccount> lVecAccount;
// 	CInterface_SvrBrokerInfo::getObj().GetTradeAccounts(lVecAccount);
// 	vector<TradeAccount>::iterator lIter = lVecAccount.begin();
// 	for(;lIter != lVecAccount.end();lIter++)
// 	{
// 		//���ó�ʼ������
// 		bool lbCommisstionLoadSuccess = false;
// 		bool lbMarginRatesLoadSuccess = false;
// 		BrokerInfo lBroker;
// 		CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lIter->nBrokerID,lBroker);
// 		BrokerAccountKey lKey(lBroker.szBrokerCode,lIter->szTradeAccount);
// 		
// 
// 		vector<PlatformStru_InstrumentCommissionRate> lVecCommissionRate;	
// 		//���װ�óɹ������Һ�Լ�ĸ�����Ϊ0 ���ҷ��ʵĸ����ͺ�Լ�ĸ�����ȵ�����²���ɹ�
// 		if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayCommission(lBroker.szBrokerCode,lIter->szTradeAccount,lsTempTradingDay,lVecCommissionRate) &&
// 			lSetInstrumentId.size() != 0 &&
// 			lVecCommissionRate.size() == lSetInstrumentId.size() &&
// 			mbForceInit == false )
// 		{
// 			TRADE_LOG("LoadTodayCommission From DataBase Success:%s AccountID %s",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 		
// 			PUSH_LOG_ERROR(Logger,false,true,"�����ݿ��м���ί�н����ʺ�%s����Ϊ%s�ķ��ʳɹ�",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 
// 			
// 			lbCommisstionLoadSuccess = true;
// 			vector<PlatformStru_InstrumentCommissionRate>::iterator lIte = lVecCommissionRate.begin();
// 			for(;lIte != lVecCommissionRate.end();lIte++)
// 			{
// 				SetCommissionRate(*lIte);
// 			}		
// 			//����״̬
// 			_MapAccountData::iterator lIterReady =  mMapAccountData.find(lKey);
// 			if(mMapAccountData.end() != lIterReady)
// 			{
// 				lIterReady->second->SetCommissionMarginReady(CommissionMargin_ComissionReady);
// 		/*		int lnTemp = (int)lIterReady->second;
// 				lnTemp |= CommissionMargin_ComissionReady;
// 				lIterReady->second = (eCommissionMarginReady)lnTemp;*/
// 			}
// 		}
// 		
// 		
// 
// 		//ȥװ�ر�֤����
// 		//���װ�óɹ������Һ�Լ�ĸ�����Ϊ0 ���ҷ��ʵĸ����ͺ�Լ�ĸ�����ȵ�����²���ɹ�
// 		vector<PlatformStru_InstrumentMarginRate> lVecMarginRate;	
// 		if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayMarginRate(lBroker.szBrokerCode,lIter->szTradeAccount,lsTempTradingDay,lVecMarginRate) &&
// 			lSetInstrumentId.size() != 0 &&
// 			lVecMarginRate.size() == lSetInstrumentId.size() &&
// 			mbForceInit == false )
// 		{
// 			TRADE_LOG("LoadTodayMarginRate From DataBase Success:%s AccountID %s",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 		
// 			PUSH_LOG_ERROR(Logger,false,true,"�����ݿ��м���ί�н����ʺ�%s����Ϊ%s�ı�֤���ʳɹ�",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 
// 			lbMarginRatesLoadSuccess = true;
// 			vector<PlatformStru_InstrumentMarginRate>::iterator lIte = lVecMarginRate.begin();
// 			for(;lIte != lVecMarginRate.end();lIte++)
// 			{
// 				SetMarginRate(*lIte);
// 			}	
// 			//����״̬
// 			_MapAccountData::iterator lIterReady =  mMapAccountData.find(lKey);
// 			if(mMapAccountData.end() != lIterReady)
// 			{
// 				lIterReady->second->SetCommissionMarginReady(CommissionMargin_MarginReady);
// 				/*int lnTemp = (int)lIterReady->second;
// 				lnTemp |= CommissionMargin_MarginReady;
// 				lIterReady->second = (eCommissionMarginReady)lnTemp;*/
// 			}
// 		}
// 		//else
// 		
// 
// 		//�����֤����װ�óɹ�����Ҫ�����ʶ���ָ�ɵ��û�������ȥ
// 		if(lbMarginRatesLoadSuccess && lbCommisstionLoadSuccess)
// 		{		
// 			TRADE_LOG("LoadToday Commission And Margin From DataBase Success,and send Qry Trade Cmd:%s AccountID %s",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 			BrokerInfo lBrokeInfo;
// 			CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lIter->nBrokerID, lBrokeInfo);
// 
// 
// 			//���÷��ʶ��󵽶�Ӧ���û�����
// 			SetRatesObjectToAccountRelationUserObject(lBrokeInfo.szBrokerCode,lIter->szTradeAccount);
// 
// 
// 			PUSH_LOG_ERROR(Logger,false,true,"װ��ί�н����ʺ�%s���ճ�����",lIter->szTradeAccount);
// 
// 			//װ���ճ�������
// 			DispathInitialDataToUserWithAccount(lBrokeInfo.szBrokerCode,lIter->szTradeAccount,mMapUser2TradingData);
// 
// 
// 			PUSH_LOG_ERROR(Logger,false,true,"����ί�н����ʺ�%s�Ĳ�ѯ�ʽ�����",lIter->szTradeAccount);
// 
// 			//���Ͳ�ѯ�ɽ����¼�
// 			//���Ͳ�ɽ�������
// 			Stru_NotifyEvent lDataEvent(EventTypeNeedQryFund,0);
// 			BrokerAccountKey lsBrokerAccount(lBrokeInfo.szBrokerCode,lIter->szTradeAccount);		
// 			lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
// 			CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 		}
// 
// 		if(!lbCommisstionLoadSuccess)
// 		{
// 			TRADE_LOG("LoadTodayCommission From DataBase Faild send Qry Commission Cmd:%s AccountID %s",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 			PUSH_LOG_ERROR(Logger,false,true,"�����ݿ��м���ί�н����ʺ�%s����Ϊ%s�ķ���ʧ�ܣ����Ͳ�ѯ��ί�н����ʺŵķ��ʲ�ѯָ��",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 
// 			BrokerInfo lBrokeInfo;
// 			CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lIter->nBrokerID, lBrokeInfo);
// 
// 			//����һ����Ҫ��ѯ��Լ���ʵ��¼�
// 			Stru_NotifyEvent lDataEvent(EventTypeNeedQryCommission,0);			
// 			BrokerAccountKey lsBrokerAccount(lBrokeInfo.szBrokerCode,lIter->szTradeAccount);		
// 			lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
// 			CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 
// 		}
// 		if(!lbMarginRatesLoadSuccess)
// 		{
// 			//����һ����Ҫ��ѯ��Լ��֤���ʵ��¼�			
// 			TRADE_LOG("LoadTodayMarginRate From DataBase Faild send Qry Margin Cmd:%s AccountID %s",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 			PUSH_LOG_ERROR(Logger,false,true,"�����ݿ��м���ί�н����ʺ�%s����Ϊ%s�ı�֤����ʧ�ܣ����Ͳ�ѯ��ί�н����ʺŵı�֤���ʲ�ѯָ��",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 
// 			BrokerInfo lBrokeInfo;
// 			CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lIter->nBrokerID, lBrokeInfo);
// 
// 			//����һ����Ҫ��ѯ��Լ���ʵ��¼�
// 			Stru_NotifyEvent lDataEvent(EventTypeNeedQryMargin,0);	
// 			BrokerAccountKey lsBrokerAccount(lBrokeInfo.szBrokerCode,lIter->szTradeAccount);		
// 			lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
// 			CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 
// 		}
// 
// 
// 
// 	}
}
_MapUserTradeData* CReal_SvrTradeData::GetMapUserDataWithOption(eSettlementLoadOption nOption)
{
	_MapUserTradeData * lRet = NULL;
//	switch(nOption)
//	{
////	case SettlementLoadOption_TradingDataFormMorning : { lRet = & mMapUser2TradingData;break;}
//	case SettlementLoadOption_TradingDataWithTodayDB : { lRet = & mMapUser2TradedDataWithDB;break;}
//	case SettlementLoadOption_TradingDataWithCTPSett : { lRet = & mMapUser2TradedDataWithCTP;break;}
//	//case SettlementLoadOption_TradingDataWithInit : lRet = & mMapUserInitialData;break;
//	}

	return lRet;
}

void CReal_SvrTradeData::GenerateOrderInfoFromCTPInputOrder(const CThostFtdcInputOrderField& inputOrder,
											PlatformStru_OrderInfo & orderInfo)
{
	memset(&orderInfo, 0, sizeof(orderInfo));
	strcpy(orderInfo.BrokerID, inputOrder.BrokerID);
	strcpy(orderInfo.InvestorID, inputOrder.InvestorID);
	strcpy(orderInfo.InstrumentID, inputOrder.InstrumentID);
	strcpy(orderInfo.OrderRef, inputOrder.OrderRef);
	strcpy(orderInfo.UserID, inputOrder.UserID);
	orderInfo.OrderPriceType = inputOrder.OrderPriceType;
	orderInfo.Direction = inputOrder.Direction;
	strcpy(orderInfo.CombOffsetFlag, inputOrder.CombOffsetFlag);
	strcpy(orderInfo.CombHedgeFlag, inputOrder.CombHedgeFlag);
	orderInfo.LimitPrice = inputOrder.LimitPrice;
	orderInfo.VolumeTotalOriginal = inputOrder.VolumeTotalOriginal;
	orderInfo.TimeCondition = inputOrder.TimeCondition;
	strcpy(orderInfo.GTDDate, inputOrder.GTDDate);
	orderInfo.VolumeCondition = inputOrder.VolumeCondition;
	orderInfo.MinVolume = inputOrder.MinVolume;
	orderInfo.ContingentCondition = inputOrder.ContingentCondition;
	orderInfo.StopPrice = inputOrder.StopPrice;
	orderInfo.ForceCloseReason = inputOrder.ForceCloseReason;
	orderInfo.IsAutoSuspend = inputOrder.IsAutoSuspend;
	strcpy(orderInfo.BusinessUnit, inputOrder.BusinessUnit);
	orderInfo.MinVolume = inputOrder.MinVolume;
	strcpy(orderInfo.Account, inputOrder.InvestorID);
	orderInfo.RequestID = inputOrder.RequestID;
	orderInfo.UserForceClose = inputOrder.UserForceClose;
	
	orderInfo.OrderSubmitStatus = '0';
	orderInfo.bInvalid = false;

}

void CReal_SvrTradeData::GenerateOrderInfo(const PlatformStru_InputOrder& inputOrder,PlatformStru_OrderInfo & orderInfo)
{
	memset(&orderInfo, 0, sizeof(orderInfo));
	strcpy(orderInfo.BrokerID, inputOrder.BrokerID);
	strcpy(orderInfo.InvestorID, inputOrder.InvestorID);
	strcpy(orderInfo.InstrumentID, inputOrder.InstrumentID);
	strcpy(orderInfo.OrderRef, inputOrder.OrderRef);
	strcpy(orderInfo.UserID, inputOrder.UserID);
	orderInfo.OrderPriceType = inputOrder.OrderPriceType;
	orderInfo.Direction = inputOrder.Direction;
	strcpy(orderInfo.CombOffsetFlag, inputOrder.CombOffsetFlag);
	strcpy(orderInfo.CombHedgeFlag, inputOrder.CombHedgeFlag);
	orderInfo.LimitPrice = inputOrder.LimitPrice;
	orderInfo.VolumeTotalOriginal = inputOrder.VolumeTotalOriginal;
	orderInfo.TimeCondition = inputOrder.TimeCondition;
	strcpy(orderInfo.GTDDate, inputOrder.GTDDate);
	orderInfo.VolumeCondition = inputOrder.VolumeCondition;
	orderInfo.MinVolume = inputOrder.MinVolume;
	orderInfo.ContingentCondition = inputOrder.ContingentCondition;
	orderInfo.StopPrice = inputOrder.StopPrice;
	orderInfo.ForceCloseReason = inputOrder.ForceCloseReason;
	orderInfo.UserForceClose = inputOrder.UserForceClose;
	orderInfo.IsAutoSuspend = inputOrder.IsAutoSuspend;
	strcpy(orderInfo.BusinessUnit, inputOrder.BusinessUnit);
	orderInfo.MinVolume = inputOrder.MinVolume;
	strcpy(orderInfo.Account, inputOrder.InvestorID);
	orderInfo.RequestID = inputOrder.RequestID;
	orderInfo.OrderSubmitStatus = '0';
	orderInfo.bInvalid = false;
	orderInfo.VolumeTotal = inputOrder.VolumeTotalOriginal;
	orderInfo.troubleMoney = util::GetDoubleInvalidValue();	
	orderInfo.freezeMargin = util::GetDoubleInvalidValue();
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	sprintf_s(orderInfo.UpdateTime, sizeof(orderInfo.UpdateTime), "%02d:%02d:%02d", 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
}

void CReal_SvrTradeData::GenerateInputOrder(const PlatformStru_OrderInfo& orderInfo,PlatformStru_InputOrder & inputOrder)
{
	memset(&inputOrder, 0, sizeof(inputOrder));
	strcpy(inputOrder.BrokerID, orderInfo.BrokerID);
	strcpy(inputOrder.InvestorID, orderInfo.InvestorID);
	strcpy(inputOrder.InstrumentID, orderInfo.InstrumentID);
	strcpy(inputOrder.OrderRef, orderInfo.OrderRef);
	strcpy(inputOrder.UserID, orderInfo.UserID);
	inputOrder.OrderPriceType = orderInfo.OrderPriceType;
	inputOrder.Direction = orderInfo.Direction;
	strcpy(inputOrder.CombOffsetFlag, orderInfo.CombOffsetFlag);
	strcpy(inputOrder.CombHedgeFlag, orderInfo.CombHedgeFlag);
	inputOrder.LimitPrice = orderInfo.LimitPrice;
	inputOrder.VolumeTotalOriginal = orderInfo.VolumeTotalOriginal;
	inputOrder.TimeCondition = orderInfo.TimeCondition;
	strcpy(inputOrder.GTDDate, orderInfo.GTDDate);
	inputOrder.VolumeCondition = orderInfo.VolumeCondition;
	inputOrder.MinVolume = orderInfo.MinVolume;
	inputOrder.ContingentCondition = orderInfo.ContingentCondition;
	inputOrder.StopPrice = orderInfo.StopPrice;
	inputOrder.ForceCloseReason = orderInfo.ForceCloseReason;
	inputOrder.IsAutoSuspend = orderInfo.IsAutoSuspend;
	inputOrder.RequestID = orderInfo.RequestID;
	inputOrder.UserForceClose = orderInfo.UserForceClose;
	strcpy(inputOrder.BusinessUnit, orderInfo.BusinessUnit);
	inputOrder.MinVolume = orderInfo.MinVolume;
	strcpy(inputOrder.strAccount, orderInfo.InvestorID);
	
}
//�����յ��Ӷ��󲢼���map
void CReal_SvrTradeData::CreateEmptyAccountOpAndAddMap(const string & nUserId,_MapUserTradeData & mapUserTradeData)
{
	//CSafeRwLockWrite lwlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mapUserTradeData.find(nUserId);
	if(mapUserTradeData.end() == lIter)
	{
		IInterfaceUserOp * lp = IFactoryUserOp::GetFactory()->CreateUserOp();
		mapUserTradeData.insert(std::make_pair(nUserId,lp));
        lp->SetEqualUserInitStatus(USER_STATUS_Inited);
	}
}

void CReal_SvrTradeData::SetPlatformObjectToAllUser(_MapUserTradeData & mapUserTradeData)
{

	//Ϊ�û��˻����ú�Լ
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mapUserTradeData.begin();
	for(;lIter != mapUserTradeData.end();lIter++)
	{
		lIter->second->SetPlatFormInstrument(mpPlatformDataInstrument);
		lIter->second->SetPlatFormQutos(mpPlatformDataQuots);
	}

}
//����Լ������ɸ��������˻��Ķ���
void CReal_SvrTradeData::SetPlatformObjectToAllAccount()
{

	//Ϊ�û��˻����ú�Լ
	CSafeRwLockRead lrlock(mrwLockMapAccount);
	_MapAccountData::iterator lIter =  mMapAccountData.begin();
	for(;lIter != mMapAccountData.end();lIter++)
	{
		lIter->second->SetPlatFormInstrument(mpPlatformDataInstrument);
		lIter->second->SetPlatFormQutos(mpPlatformDataQuots);
	}

}

//����Լ������ɸ������û��Ķ���
void CReal_SvrTradeData::DispathRateToAllUser(_MapUserTradeData & mapUserTradeData)
{
	
	CPlatformDataMgr_Rates * lpRate = NULL;
	CSafeRwLockRead lrlockAccount(mrwLockMapAccount);
	_MapAccountData::iterator lIter =  mMapAccountData.begin();	
	while(mMapAccountData.end() != lIter)	
	{		
		lpRate = lIter->second->GetPlatFormRates();
		if(lpRate != NULL)
		{
			//�������nBrokerID �� nAccountID���û�		//			
			vector<UserInfo> lVecUser;
// 			CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(lIter->first.AccountID,lIter->first.BrokerID,lVecUser);	
// 			vector<UserInfo>::iterator lIter = lVecUser.begin();
// 			for(;lIter != lVecUser.end();lIter++)
// 			{
// 				CSafeRwLockRead lrlock(mrwLockMapUserTrading);
// 				_MapUserTradeData::iterator  lIterUser = mapUserTradeData.find(lIter->szAccount);
// 				if(lIterUser != mapUserTradeData.end())
// 					lIterUser->second->SetPlatFormRates(lpRate);
// 			}		

		}
		lIter++;
	}


	
}
//���ɽ���ĳ������û�����
void CReal_SvrTradeData::DispathFundInOut(const std::string & nsDate,_MapUserTradeData & nmapUserTradeData)
{
	//װ�س����
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIteFundInOut = nmapUserTradeData.begin();
// 	while(lIteFundInOut != nmapUserTradeData.end())
// 	{
// 		TRADE_LOG("LoadQryFundInOut From DataBase CMD:day %s",nsDate.c_str());
// 		vector<sFundInOut> lvecFundInOut;
// 		sQryFundInOut ls;
// 		strcpy(ls.msStartDay,nsDate.c_str());
// 		strcpy(ls.msEndDay,nsDate.c_str());
// 		strcpy(ls.mUserID,lIteFundInOut->first.c_str());
// 		mpDBOpr->LoadQryFundInOut(ls,lvecFundInOut);
// 		for(unsigned int i = 0; i < lvecFundInOut.size();i++)
// 		{
// 			lIteFundInOut->second->UserFundInOut(lvecFundInOut[i]);
// 		}
// 		lIteFundInOut++;
// 	}


}


//�����û��ĳ�ʼ���ݵ�����������
void CReal_SvrTradeData::DispathInitialDataToUser(const std::string & nUserId, 
												  const std::string & nsInstruemntDay,
												  _MapUserTradeData & nmapUserTradeData)
{
	CSafeLock lLock(m_mutexInitData);

	TRADE_LOG("DispathInitialDataToUser %s",nUserId.c_str());
	_MapUserTradeData::iterator  lIterUser = nmapUserTradeData.find(nUserId);
	if(lIterUser != nmapUserTradeData.end())
	{


		//��ȡ���û��ĳ�ʼȨ��				
		std::map<std::string,double>::iterator lIteInitFund = mUserInitInitFund.find(nUserId);
		if(lIteInitFund != mUserInitInitFund.end())
		{
			lIterUser->second->SetInitFund(lIteInitFund->second);
		}

		//��ȡ���û����ʽ�				
		std::map<std::string,PlatformStru_TradingAccountInfo>::iterator lIteFund = mUserIniFundInfo.find(nUserId);
		if(lIteFund != mUserIniFundInfo.end())
		{
			strcpy(lIteFund->second.TradingDay,nsInstruemntDay.c_str());
			lIterUser->second->SetQryFundInfo(lIteFund->second);
		}
		else
		{
			//���ÿ��ʽ��accountid
			PlatformStru_TradingAccountInfo lFundInfo;
			memset(&lFundInfo,0,sizeof(lFundInfo));
			strcpy(lFundInfo.BrokerID,"0000");
			strcpy(lFundInfo.AccountID,lIterUser->first.c_str());	
			strcpy(lFundInfo.TradingDay,nsInstruemntDay.c_str());
			lIterUser->second->SetQryFundInfo(lFundInfo);
		}

		//����ճ����ݵ����ں��������ݵ�������һ�죬���ü��س����
		if(msInstrumentDay != msUserInitDay)
		{
			//��ȡ�û�����ĳ����
			vector<sFundInOut> lvecFundInOut;
			sQryFundInOut ls;
			strcpy(ls.msStartDay,msInstrumentDay.c_str());
			strcpy(ls.msEndDay,msInstrumentDay.c_str());
			strcpy(ls.mUserID,lIterUser->first.c_str());
// 			mpDBOpr->LoadQryFundInOut(ls,lvecFundInOut);
// 			for(unsigned int i = 0; i < lvecFundInOut.size();i++)
// 			{
// 				lIterUser->second->UserFundInOut(lvecFundInOut[i]);
// 			}
		}


		//��ȡ���û��ĳֲ�
		std::map<std::string,vector<PlatformStru_Position>>::iterator lIteVecPos = mUserInitPosition.find(nUserId);
		if(lIteVecPos != mUserInitPosition.end())
		{

			if(lIteVecPos->second.size() == 0)
			{
				PlatformStru_Position lTemp;
				//lIterUser->second->QryPositionReach(lTemp,-1,true);
			}
			else
			{
				bool bLast = false;
				for(unsigned int i = 0;i < lIteVecPos->second.size();i++ )
				{	
					strcpy(lIteVecPos->second[i].TradingDay,nsInstruemntDay.c_str());
					PlatformStru_InstrumentMarginRate lMargin;
					lMargin.clear();
					if(0 == lIterUser->second->GetMarginRate(lIteVecPos->second[i].InstrumentID,lMargin))
					{
						lIteVecPos->second[i].MarginRateByMoney = 
							lIteVecPos->second[i].PosiDirection == THOST_FTDC_PD_Long ? lMargin.LongMarginRatioByMoney : lMargin.ShortMarginRatioByMoney;
						lIteVecPos->second[i].MarginRateByMoney = 
							  lIteVecPos->second[i].PosiDirection == THOST_FTDC_PD_Long ? lMargin.LongMarginRatioByVolume : lMargin.ShortMarginRatioByVolume;

					}
					//lIteVecPos->second[i].MarginRateByMoney = lIterUser->
					//lIteVecPos->second[i].MarginRateByMoney

					if(i == lIteVecPos->second.size() - 1) 
						bLast = true;
					//�жϸóֲֵĺ�Լ�Ƿ����
					 PlatformStru_InstrumentInfo InstrumentInfo;
					if(mpPlatformDataInstrument != NULL &&
						false == mpPlatformDataInstrument->GetInstrumentInfo(lIteVecPos->second[i].InstrumentID,InstrumentInfo))
					{
						sErrorInfo lsError;
						memset(&lsError,0,sizeof(lsError));
						lsError.meErrorType = FirstLevelError;
						sprintf(lsError.msError,
							ERROR_POSITION_FORMAT,
							lIteVecPos->second[i].InvestorID,
							lIteVecPos->second[i].InstrumentID,
							lIteVecPos->second[i].PosiDirection == THOST_FTDC_PD_Long ? "��ͷ" : "��ͷ",
							lIteVecPos->second[i].Position);
						//TRADE_LOG(lsError.msError);
						//CInterface_SvrNotifyAndAsk::getObj().PushErrorInfo(lsError);
					}
					strcpy(lIteVecPos->second[i].Account,lIteVecPos->second[i].InvestorID);
					lIterUser->second->QryPositionReach(lIteVecPos->second[i],0,bLast);
				}
			}				
		}
		else
		{
			//����һ������ĳֲ�
			PlatformStru_Position lTemp;
			memset(&lTemp,0,sizeof(lTemp));
			lIterUser->second->QryPositionReach(lTemp,-1,true);
		}

		//��ȡ���û��ĳֲ���ϸ
		std::map<std::string,vector<PlatformStru_PositionDetail>>::iterator lIteVecPosDetail = mUserInitPositionDeatil.find(nUserId);
		if(lIteVecPosDetail != mUserInitPositionDeatil.end())
		{

			if(lIteVecPosDetail->second.size() == 0)
			{
				PlatformStru_PositionDetail lTemp;
				memset(&lTemp,0,sizeof(lTemp));
				//lIterUser->second->QryPositionDetailReach(lTemp,-1,true);
			}
			else
			{
				bool bLast = false;
				for(unsigned int i = 0;i < lIteVecPosDetail->second.size();i++ )
				{	
					strcpy(lIteVecPosDetail->second[i].TradingDay,nsInstruemntDay.c_str());
					PlatformStru_InstrumentMarginRate lMargin;
					lMargin.clear();
					if(0 == lIterUser->second->GetMarginRate(lIteVecPosDetail->second[i].InstrumentID,lMargin))
					{
						lIteVecPosDetail->second[i].MarginRateByMoney = 
							lIteVecPosDetail->second[i].Direction == THOST_FTDC_D_Buy ? lMargin.LongMarginRatioByMoney : lMargin.ShortMarginRatioByMoney;
						lIteVecPosDetail->second[i].MarginRateByMoney = 
						    lIteVecPosDetail->second[i].Direction == THOST_FTDC_D_Buy ? lMargin.LongMarginRatioByVolume : lMargin.ShortMarginRatioByVolume;

					}
					if(i == lIteVecPosDetail->second.size() - 1) bLast = true;
					strcpy(lIteVecPosDetail->second[i].Account,lIteVecPosDetail->second[i].InvestorID);
					lIterUser->second->QryPositionDetailReach(lIteVecPosDetail->second[i],0,bLast);
				}
			}				
		}
		else
		{
			//����һ������ĳֲ���ϸ
			PlatformStru_PositionDetail lTemp;
			memset(&lTemp,0,sizeof(lTemp));
			lIterUser->second->QryPositionDetailReach(lTemp,-1,true);
		}
	}			


	return;

}
//�����û��ĳ�ʼ���ݵ�����������
void CReal_SvrTradeData::DispathInitialDataToUserWithAccount(const std::string & nBrokerID, 
									  const std::string & nAccountID,
									  _MapUserTradeData & nmapUserTradeData,
                                      const std::string & nUserID)
{

    std::string lsTempTradingDay = "20140618";//msInstrumentDay.substr(0,4) + msInstrumentDay.substr(5,2) + msInstrumentDay.substr(8,2);
    DispathInitialDataToUser(nUserID,lsTempTradingDay,nmapUserTradeData);	

	//CSafeLock lLock(m_mutexInitData);
	//����ʽ�
	//CPlatformDataMgr_Rates * lpRate = NULL;
	////m_mutexMapAccount.lock();
	//BrokerAccountKey lKey(nBrokerID.c_str(),nAccountID.c_str());
	//_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);	
	//if(mMapAccountData.end() != lIter)	
	//{		
	//	lpRate = lIter->second;
	//}
	////m_mutexMapAccount.unlock();

	//if(lpRate != NULL)
// 	{
		//�������nBrokerID �� nAccountID���û�		//	
// 		std::string lsTempTradingDay = msInstrumentDay.substr(0,4) + msInstrumentDay.substr(5,2) + msInstrumentDay.substr(8,2);
// 		vector<UserInfo> lVecUser;
//  		CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(nAccountID,nBrokerID,lVecUser);	
//  		vector<UserInfo>::iterator lIter = lVecUser.begin();
//  		for(;lIter != lVecUser.end();lIter++)
//  		{
//  			DispathInitialDataToUser(nUserID,lsTempTradingDay,nmapUserTradeData);	
//  		}	

// 	}
	
	return;

}

void CReal_SvrTradeData::SetRatesObjectToAccountRelationUserObject(const std::string & nsBrokerId,
													   const std::string & nsAccountID,
                                                       const std::string & nUserID)
{

	

	CPlatformDataMgr_Rates * lpRate = NULL;

	BrokerAccountKey lKey(nsBrokerId.c_str(),nsAccountID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);	
	if(mMapAccountData.end() != lIter)	
	{		
		lpRate = lIter->second->GetPlatFormRates();
	}
	

	if(lpRate != NULL)
	{
		//�������nBrokerID �� nAccountID���û�		//		
		//vector<UserInfo> lVecUser;
// 		CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(nsAccountID,nsBrokerId,lVecUser);	
// 		vector<UserInfo>::iterator lIter = lVecUser.begin();
// 		for(;lIter != lVecUser.end();lIter++)
// 		{
 			_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nUserID);
 			if(lIterUser != mMapUser2TradingData.end())
 				lIterUser->second->SetPlatFormRates(lpRate);
// 		}
		

	}
}


//װ��ĳһ�������û��ĵ��ճ�����
CF_ERROR CReal_SvrTradeData::LoadInitData(PlatformStru_TradingAccountInfo& inData, const std::string & nsTime,const std::string & szUser)
{
	if(GetTraderDataStatus() != TradeDataStatusInitialing &&
		GetTraderDataStatus() != TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("Not Initing,discard this call");
		return CF_ERROR_NOINITED;
	}
	
	/*if(nsTime.empty())
		return CF_ERROR_SUCCESS;*/

	bool lbLoad = true;
	CSafeLock lLock(m_mutexInitData);

	msUserInitDay = nsTime;
	//��ȡ�û��б�
	//vector<UserInfo> lVecUser;
// 	CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUser);
// 	vector<UserInfo>::iterator lIter = lVecUser.begin();
// 	for(;lIter != lVecUser.end();lIter++)
// 	{
// 		if(lIter->nUserType == USER_TYPE_TRADER)
// 		{
 			vector<PlatformStru_PositionDetail> lVecPosionDeatil;
 			vector<PlatformStru_Position> lVecPosition;
 			//PlatformStru_TradingAccountInfo lFundInfo;
 			//lFundInfo.clear();
 			
 			
 			
 			//if(CF_ERROR_SUCCESS == LoadInitialPositionFromDB(lIter->szAccount,nsTime,lVecPosition))
 			//	lbLoad = true;
 
 			//if(CF_ERROR_SUCCESS == LoadInitialPositionDetailFromDB(lIter->szAccount,nsTime,lVecPosionDeatil))
 			//	lbLoad = true;
 
 			//if(CF_ERROR_SUCCESS == LoadInitialFundFromDB(lIter->szAccount,nsTime,lFundInfo))
 			//	lbLoad = true;
 
 			//double ldbVal = inData.Available;
 			//LoadInitialInitFundFromDB(lIter->szAccount,ldbVal);
 
 			//strcpy(inData.AccountID,szUser.c_str());
 			mUserIniFundInfo.insert(std::make_pair(szUser,inData));
 			mUserInitPosition.insert(std::make_pair(szUser,lVecPosition));
 			mUserInitPositionDeatil.insert(std::make_pair(szUser,lVecPosionDeatil));
 			mUserInitInitFund[szUser] = inData.Available;
// 		}
// 	}
	if(lbLoad )
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_DATABASE_OTHER_ERROR;
}
//�ӽ������ݿ�����װ���ճ��ĳֲ�
CF_ERROR CReal_SvrTradeData::LoadInitialPositionFromDB(const std::string & nUserName,
													   const std::string & nsTime,
													   vector<PlatformStru_Position> & nVec)
{
	
// 	if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayUserPositions("SETTLEMENT_POSITIONINIT",
// 		nUserName,
// 		nsTime,
// 		nVec))
// 	{
// 		return CF_ERROR_SUCCESS;
// 	}
	return CF_ERROR_DATABASE_OTHER_ERROR;
}

//�ӽ������ݿ�����װ���ճ��ĳֲ���ϸ
CF_ERROR CReal_SvrTradeData::LoadInitialPositionDetailFromDB(const std::string & nUserName,
															 const std::string & nsTime,
															 vector<PlatformStru_PositionDetail>& nVec)
{
	
// 	if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayUserPositionDetails("SETTLEMENT_POSITIONDETAILINIT",
// 		nUserName,
// 		nsTime,
// 		nVec))
// 	{
// 		return CF_ERROR_SUCCESS;
// 	}
	return CF_ERROR_DATABASE_OTHER_ERROR;
}

//�ӽ������ݿ�����װ���ճ����ʽ�
CF_ERROR CReal_SvrTradeData::LoadInitialFundFromDB(const std::string & nUserName,
												   const std::string & nsTime,
												   PlatformStru_TradingAccountInfo & nFundIndo)
{

	//PlatformStru_TradingAccountInfo lInfo;
	memset(&nFundIndo,0,sizeof(nFundIndo));
// 	if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayUserFundInfo("SETTLEMENT_FUNDINIT",
// 		nUserName,
// 		nsTime,
// 		nFundIndo))
// 	{
// 		//�޸�ÿ�յĳ����Ϊ0
// 		nFundIndo.Withdraw = 0.0;
// 		nFundIndo.Deposit = 0.0;
// 		return CF_ERROR_SUCCESS;
// 	}
	return CF_ERROR_DATABASE_OTHER_ERROR;	
}

//�ӳ�ʼȨ������������ת�س�ʼ�ʽ�
CF_ERROR CReal_SvrTradeData::LoadInitialInitFundFromDB(const std::string & nUserName,		
										   double & nFundIndo)
{
	
// 	if(CF_ERROR_SUCCESS == mpDBOpr->LoadUserInitFund("TradeData_InitFund",
// 		nUserName,		
// 		nFundIndo))
// 	{
// 		
// 		return CF_ERROR_SUCCESS;
// 	}
	return CF_ERROR_DATABASE_OTHER_ERROR;	
}
//��ʼ���û�����
CF_ERROR CReal_SvrTradeData::InitUserMapWithEmpty(string szUser,string szBrokerCode,string szTradeAccount,_MapUserTradeData & nmapUserData)
{

	{
		CSafeRwLockWrite lwlockAccount(mrwLockMapAccount);

        BrokerAccountKey lKey(szBrokerCode.c_str(),szTradeAccount.c_str());

        IInterfaceAccountOp * lpTemp = NULL;
		if(mMapAccountData.find(lKey) == mMapAccountData.end())
		{
			lpTemp = IFactoryAccountOp::GetFactory()->CreateAccountOp();
			mMapAccountData.insert(std::make_pair(lKey,lpTemp));
		}
	}

	{

		//��ȡ�û��б�
		CSafeRwLockWrite lwlock(mrwLockMapUserTrading);
        if(nmapUserData.find(szUser) == nmapUserData.end())
            CreateEmptyAccountOpAndAddMap(szUser,nmapUserData);
	}

	return CF_ERROR_SUCCESS;
}
//ȫ����������
//��ʼ��״̬�µĺ���������״̬����Ч
CF_ERROR CReal_SvrTradeData::ResetAll()
{
	//������������
	mMutextSettlementQuot.lock();
	mmapSettlementQuot.clear();
	mMutextSettlementQuot.unlock();

	//����ճ��û���Ϣ
	{
		m_mutexInitData.lock();		
		mUserInitPosition.erase(mUserInitPosition.begin(),mUserInitPosition.end());
		mUserInitPositionDeatil.erase(mUserInitPositionDeatil.begin(),mUserInitPositionDeatil.end());
		mUserIniFundInfo.erase(mUserIniFundInfo.begin(),mUserIniFundInfo.end());
		m_mutexInitData.unlock();

	}	
	
	//����û���Ϣ
	{
		CSafeRwLockWrite lwlock(mrwLockMapUserTrading);
		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();	
		while(mMapUser2TradingData.end() != lIter)	
		{		
			if(lIter->second != NULL)
			{
				lIter->second->ClearAll();					
			}
			lIter++;
		}		

	}

	//���ί�н����˺���صķ��ʱ�֤����
	{
		//m_mutexMapAccount.lock();
		CSafeRwLockWrite lwlock(mrwLockMapAccount);
		_MapAccountData::iterator lIter =  mMapAccountData.begin();	
		while(mMapAccountData.end() != lIter)	
		{		
			if(lIter->second != NULL)
			{
				lIter->second->ClearAll();
			}
			lIter++;
		}
		//mMapAccountData.erase(mMapAccountData.begin(),mMapAccountData.end());
		//mMapAccountRateReady.erase(mMapAccountRateReady.begin(),mMapAccountRateReady.end());
		//m_mutexMapAccount.unlock();
	}


	//�����Լ
	InterlockedExchange(reinterpret_cast<volatile long*>(&mbInstrumentsReady),DataReday_NoReady);
	if(mpPlatformDataQuots)
	{
		mpPlatformDataQuots->clear();	
	}

	if(mpPlatformDataInstrument != NULL)
	{
		mpPlatformDataInstrument->clear();	
	}

	return CF_ERROR_SUCCESS;
}


//��ʱ�洢
CF_ERROR  CReal_SvrTradeData::TimerSaveTradingData(bool bTimer)
{
// 	if(CInterface_SvrLogin::getObj().IsSlaver())
// 		return CF_ERROR_SUCCESS;

	if(GetTraderDataStatus() != TradeDataStatusTrading && GetTraderDataStatus() != TradeDataStatusInitialing)
		return CF_ERROR_NOINITED;

	std::string lsTempTradingDay;
	GetCurrentTradingDay(lsTempTradingDay);
	//if(bTimer != false || mBufferDataStoreCTPOrder.Size() >= LIST_MAX_SIZE)
	//	mBufferDataStoreCTPOrder.SaveToDB(mpDBOpr,&IInterfaceDBOpr::WriteOrderInfos,"CTP_ORDERS","",lsTempTradingDay);

	//if(bTimer != false || mBufferDataStoreCTPTrade.Size() >= LIST_MAX_SIZE)
	//	mBufferDataStoreCTPTrade.SaveToDB(mpDBOpr,&IInterfaceDBOpr::WriteTradeInfos,"CTP_TRADES","",lsTempTradingDay);

	//if(bTimer != false || mBufferDataStoreOrder.Size() >= LIST_MAX_SIZE)
	//	mBufferDataStoreOrder.SaveToDB(mpDBOpr,&IInterfaceDBOpr::WriteOrderInfos,"TRADEDATA_ORDERS","",lsTempTradingDay);

	//if(bTimer != false || mBufferDataStoreTrade.Size() >= LIST_MAX_SIZE)
	//	mBufferDataStoreTrade.SaveToDB(mpDBOpr,&IInterfaceDBOpr::WriteTradeInfos,"TRADEDATA_TRADES","",lsTempTradingDay);

	//if(bTimer != false || mBufferDataStoreExecuteInOrder.Size() >= LIST_MAX_SIZE)
	//	mBufferDataStoreExecuteInOrder.SaveToDB(mpDBOpr,&IInterfaceDBOpr::WriteOrderInfos,"EXECUTEIN_ORDERS","",lsTempTradingDay,false);
	//	
	//if(bTimer != false || mBufferDataStoreExecuteOutOrder.Size() >= LIST_MAX_SIZE)
	//	mBufferDataStoreExecuteOutOrder.SaveToDB(mpDBOpr,&IInterfaceDBOpr::WriteOrderInfos,"EXECUTEOUT_ORDERS","",lsTempTradingDay,false);

	//��ȡ�����޸ĵı������ɽ����޸ĵĳֲ֣��ֲ���ϸ����ʱsave

	return CF_ERROR_SUCCESS;
}


//�����������
CF_ERROR  CReal_SvrTradeData::SaveAllTradingData(bool nbInitFirst)
{
// 	if(CInterface_SvrLogin::getObj().IsSlaver())
// 		return CF_ERROR_SUCCESS;

	//
	//CSafeLock lLock(m_mutexMapUserTrading);

	////����������
	//mMutextSettlementQuot.lock();
	//vector<PlatformStru_DepthMarketData> lVecQuto;
	//std::map<std::string,PlatformStru_DepthMarketData>::iterator lIterQuto =  mmapSettlementQuot.begin();
	//while(lIterQuto != mmapSettlementQuot.end())
	//{
	//	lVecQuto.push_back(lIterQuto->second);
	//	lIterQuto++;
	//}
	//mMutextSettlementQuot.unlock();
	//if(lVecQuto.size() > 0)
	//	mpDBOpr->WriteQutoInfo("TradeData_Quto",msCurrentTradingDay,lVecQuto);

	//	
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	std::string lsTempTradingDay;
	GetCurrentTradingDay(lsTempTradingDay);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();

	while(mMapUser2TradingData.end() != lIter && !lsTempTradingDay.empty())	
	{			
// 		if(lIter->second != NULL && lIter->second->GetUserInitStatus() == USER_STATUS_Inited)
// 		{
// 			//��������еı����ͳɽ�
// 			mBufferDataStoreOrder.Clear();
// 			mBufferDataStoreTrade.Clear();
// 			////�汨��
// 			std::vector<PlatformStru_OrderInfo> lVecOrder;
// 			if(CF_ERROR_SUCCESS == lIter->second->GetAllOrders(lVecOrder) && lVecOrder.size() != 0)
// 				mpDBOpr->WriteOrderInfos("TradeData_Orders",lIter->first,lsTempTradingDay,lVecOrder);
// 
// 			//��ɽ�
// 			std::vector<PlatformStru_TradeInfo> lVecTrade;
// 			if(CF_ERROR_SUCCESS == lIter->second->GetAllTrades(lVecTrade) && lVecTrade.size() != 0)
// 				mpDBOpr->WriteTradeInfos("TradeData_Trades",lIter->first,lsTempTradingDay,lVecTrade);
// 
// 			//��ֲ�
// 			std::vector<PlatformStru_Position> lVecPosition;
// 			if(CF_ERROR_SUCCESS == lIter->second->GetAllPosition(lVecPosition) && lVecPosition.size() != 0)
// 				mpDBOpr->WritePositionInfos("TradeData_Position",lIter->first,lsTempTradingDay,lVecPosition);
// 
// 			//��ֲ�
// 			std::vector<PlatformStru_PositionDetail> lVecPositionDetail;
// 			if(CF_ERROR_SUCCESS == lIter->second->GetAllPositionDetail(lVecPositionDetail) && lVecPositionDetail.size() != 0)
// 				mpDBOpr->WritePositionDetailInfos("TradeData_PositionDetail",lIter->first,lsTempTradingDay,lVecPositionDetail);
// 	
// 			//���ʽ�		
// 			PlatformStru_TradingAccountInfo lTempTrade;
// 			if(CF_ERROR_SUCCESS == lIter->second->GetFundInfo(lTempTrade))
// 			{				
// 				mpDBOpr->WriteFundInfos("TradeData_FundInfo",lIter->first,lsTempTradingDay,lTempTrade);
// 			}
// 
// 
// 		}

		lIter++;
	}

	CSafeRwLockRead lrlockAccount(mrwLockMapAccount);
	_MapAccountData::iterator lIterAccount =  mMapAccountData.begin();

	while(mMapAccountData.end() != lIterAccount && !lsTempTradingDay.empty())	
	{			
		if(lIterAccount->second != NULL && lIterAccount->second->GetAccountInitStatus() == USER_STATUS_Inited)
		{
		
			////�汨��
// 			std::vector<PlatformStru_OrderInfo> lVecOrder;
// 			if(CF_ERROR_SUCCESS == lIterAccount->second->GetAllOrders(lVecOrder) && lVecOrder.size() != 0)
// 			{
// 				if(nbInitFirst)
// 					mpDBOpr->WriteAccountOrderInfos("TRADEDATA_A_O_INIT",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lVecOrder);
// 
// 				mpDBOpr->WriteAccountOrderInfos("TRADEDATA_A_ORDERS",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lVecOrder);
// 			}
// 
// 			//��ɽ�
// 			std::vector<PlatformStru_TradeInfo> lVecTrade;
// 			if(CF_ERROR_SUCCESS == lIterAccount->second->GetAllTrades(lVecTrade) && lVecTrade.size() != 0)
// 			{
// 				if(nbInitFirst)
// 					mpDBOpr->WriteAccountTradeInfos("TRADEDATA_A_T_INIT",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lVecTrade);
// 
// 				mpDBOpr->WriteAccountTradeInfos("TRADEDATA_A_TRADES",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lVecTrade);
// 			}
// 
// 			//��ֲ�
// 			std::vector<PlatformStru_Position> lVecPosition;
// 			if(CF_ERROR_SUCCESS == lIterAccount->second->GetAllPosition(lVecPosition) && lVecPosition.size() != 0)
// 			{
// 				if(nbInitFirst)
// 					mpDBOpr->WriteAccountPositionInfos("TradeData_A_P_INIT",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lVecPosition);
// 
// 				mpDBOpr->WriteAccountPositionInfos("TradeData_A_P",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lVecPosition);
// 			}
// 
// 			//��ֲ�
// 			std::vector<PlatformStru_PositionDetail> lVecPositionDetail;
// 			if(CF_ERROR_SUCCESS == lIterAccount->second->GetAllPositionDetail(lVecPositionDetail) && lVecPositionDetail.size() != 0)
// 			{
// 				if(nbInitFirst)
// 					mpDBOpr->WriteAccountPositionDetailInfos("TradeData_A_PD_INIT",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lVecPositionDetail);
// 
// 				mpDBOpr->WriteAccountPositionDetailInfos("TradeData_A_PD",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lVecPositionDetail);
// 			}
// 
// 			//���ʽ�		
// 			PlatformStru_TradingAccountInfo lTempTrade;
// 			if(CF_ERROR_SUCCESS == lIterAccount->second->GetFundInfo(lTempTrade))
// 			{		
// 				if(nbInitFirst)
// 						mpDBOpr->WriteAccountFundInfos("TRADEDATA_A_FUNDINIT",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lTempTrade);
// 
// 				mpDBOpr->WriteAccountFundInfos("TRADEDATA_A_FUNDINFO",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lTempTrade);
// 			}


		}

		lIterAccount++;
	}
	
	return CF_ERROR_SUCCESS;
}


//����һ���¼��û���ص��˻�Ϊ����
CF_ERROR CReal_SvrTradeData::PushEventToAccountWithUser(eEventType nEve,const std::string & nUserName)
{
	int nUserID = -1;
	vector<UserInfo> lVecUeser;
// 	CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUeser);
// 	for(unsigned int i = 0;i < lVecUeser.size();i++)
// 	{
// 		if(strcmp(lVecUeser[i].szAccount,nUserName.c_str()) == 0)
// 		{
// 			nUserID = lVecUeser[i].nUserID;
// 			break;
// 		}
// 	}
// 	if(nUserID != -1)
// 	{
// 		TradeAccount tradeAccount;
// 		if(!CInterface_SvrUserOrg::getObj().GetTradeAccountByUserID(nUserID,tradeAccount))
// 		{
// 			BrokerInfo lBroker;							
// 			CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(tradeAccount.nBrokerID,lBroker);
// 			//����logoff���¼�					
// 			Stru_NotifyEvent lDataEvent(nEve,0);
// 			BrokerAccountKey lsBrokerAccount(lBroker.szBrokerCode,tradeAccount.szTradeAccount);		
// 			lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
// 			CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 			return CF_ERROR_SUCCESS;
// 		}
// 	}
	return CF_ERROR_TRADEDATA_NO_USER_DATA;
	
}

void CReal_SvrTradeData::TradeDataInitAfterNoTradingDay()
{
	//û�л�ȡ��������
	//
	TRADE_LOG("TradeDataInitAfterNoTradingDay");
	
	std::string lsOldestInitDayWithNoSett;
// 	mpTradingDayManager.HasInitTradingDayWithNoSettlement(lsOldestInitDayWithNoSett);
// 
// 	if(lsOldestInitDayWithNoSett.empty())
// 	{
// 		PUSH_LOG_ERROR(Logger,false,true,"û�н����գ�Ҳû�д�����Ľ����գ����÷�����Ϊ�޽�����״̬");
// 		PUSH_LOG_ERROR(Logger,false,true,"-----------------��ʼ�����-----------------");
// 		TRADE_LOG("can not find a inited trading day with no settlemnt,we do nothing");
// 		SetTraderDataStatus(TradeDataStatusNoTradingDay);
// 	}
// 	else
// 	{
// 		PUSH_LOG_ERROR(Logger,false,true,"�д�����Ľ����գ����÷�����Ϊ�д����㽻����״̬");
// 		PUSH_LOG_ERROR(Logger,false,true,"-----------------��ʼ�����-----------------");
// 
// 		SetCurrentTradingDay(lsOldestInitDayWithNoSett,true);
// 		SetTraderDataStatus(TradeDataStatusLastDaySettlemtNotOK);
// 		TRADE_LOG("Inittradingday with no settlement is %s, set trading status to TradeDataStatusLastDaySettlemtNotOK ",\
// 			lsOldestInitDayWithNoSett.c_str());
// 	
// 	}
	return;
}


//��ȡ�����պ󣬽��еĽ��׳�ʼ������
void CReal_SvrTradeData::TradeDataInitAfterGetTradingDay() 
{
	std::string lsTempTradingDay;
	GetCurrentTradingDay(lsTempTradingDay);
	TRADE_LOG("TradeDataInitAfterGetTradingDay %s",lsTempTradingDay.c_str());
	//�鿴�����Ƿ�����,ֱ������Ϊ�������״̬
// 	if(mpTradingDayManager.IsSetttemnted(lsTempTradingDay))
// 	{		
// 		TRADE_LOG("Today %s has settlemented,only need to set status to EndSettlemented",lsTempTradingDay.c_str());	
// 		PUSH_LOG_ERROR(Logger,false,true,"�����ѽ��㣬���÷���������������״̬");
// 		CReal_SvrTradeData::getObj().SetTraderDataStatus(TradeDataStatusSettlemtEnd);
// 		return;
// 	}
// 	else
// 	{
// 		std::string lsLastTradingDay;
// 		mpTradingDayManager.GetLastTradingData(lsTempTradingDay,lsLastTradingDay);
// 
// 		//�����һ���������Ƿ����
// 		if(!lsLastTradingDay.empty () &&
// 			!mpTradingDayManager.IsSetttemnted(lsLastTradingDay))
// 		{
// 			TRADE_LOG("LoadInitData Failed need resettlement day is %s",lsLastTradingDay.c_str());
// 			PUSH_LOG_ERROR(Logger,false,true,"��һ��������δ����");
// 			TradeDataInitAfterNoTradingDay();
// 			return;				
// 		}
// 		else
// 		{
// 			PUSH_LOG_ERROR(Logger,false,true,"�����ݿ��м����ճ�����");
// 			TRADE_LOG("LoadInitData Load last day %s",lsLastTradingDay.c_str());
// 			LoadInitData(lsLastTradingDay);
// 		}
// 
// 
// 	}


	//֪ͨģ���̲߳�ͣ��check �û���״̬��
	//PostThreadMessage(g_idThread,TMESSAGE_TRADEINIT_CHECK_USER_STATUS,0,0);


	//���װ���ճ����ݳɹ�������Ϊ����״̬
	//SetTraderDataStatus(TradeDataStatusTrading);



	//���ú�Լ�ͷ��ʵ�����
	msInstrumentDay = lsTempTradingDay;

	//�����Լ
	//������ݿ��н���ĺ�Լ�����ȡ���ݿ�
	//������ݿ���û�н���ĺ�Լ�������̷��Ͳ�ѯ��Լ���¼�
	vector<PlatformStru_InstrumentInfo> lVecInstrument;	
	std::set<std::string> lSetInstrumentId;
// 	if(mbForceInit == false && CF_ERROR_SUCCESS == mpDBOpr->LoadTodayInstrument(lsTempTradingDay,lVecInstrument))
// 	{
// 		TRADE_LOG("LoadTodayInstrument From DataBase Success Send Get Instrument CMD:AccountID %s",lsTempTradingDay.c_str());
// 		vector<PlatformStru_InstrumentInfo>::iterator lIte = lVecInstrument.begin();
// 		for(;lIte != lVecInstrument.end();lIte++)
// 		{
// 			SetInstrumentInfo(*lIte);
// 		}
// 		//ȥ�������ظ��ĺ�Լ
// 		mpPlatformDataInstrument->GetInstrumentList(lSetInstrumentId);
// 
// 		//���������û���		
// 		InterlockedExchange(reinterpret_cast<volatile long*>(&mbInstrumentsReady),DataReady_Ready);
// 
// 		//Ϊ���ʺ����ú�Լ
// 		SetPlatformObjectToAllAccount();
// 		//Ϊ�û��˻����ú�Լ
// 		SetPlatformObjectToAllUser(mMapUser2TradingData);
// 
// 		//������Ľ����ճ�ʼ�����	
// 		mpTradingDayManager.SetTradingDayInit(lsTempTradingDay);
// 
// 		
// 		//֪ͨ��������ȡ��Լ
// 		Stru_NotifyEvent lDataEvent(EventTypeNeedGetInstrument,0);
// 		CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 		PUSH_LOG_ERROR(Logger,false,true,"�����ݿ���غ�Լ��Ϣ�ɹ������ͻ�ȡ��Լָ��");
// 
// 		//�����ں�Լ�ĳֲ�
// 		if(CheckInitDataHasExpiredInstruementPosition())
// 		{
// 			SetTraderDataStatus(TradeDataStatusInitFail);
// 			PUSH_LOG_ERROR(Logger,false,true,"�����ճ��ֲ������й��ڵĺ�Լ����������ʼ��ʧ��");
// 			//֪ͨ�̹߳رն�ʱ��
// 			PostThreadMessage(g_idThread,TMESSAGE_TRADING_END_USER_CHECK,0,0);
// 			return;
// 		}
// 
// 		
// 
// 		//�����ѯ��֤��ͷ���
// 		SendQryCommissionAndMargin();
// 	}
// 	else
// 	{
// 		TRADE_LOG("LoadTodayInstrument From DataBase Faild Send Qry Instrument CMD:date is %s",lsTempTradingDay.c_str());
// 		PUSH_LOG_ERROR(Logger,false,true,"���Ͳ�ѯ��Լָ��");
// 		//����һ����Ҫ��ѯ��Լ���¼�
// 		Stru_NotifyEvent lDataEvent(EventTypeNeedQryInstrument,0);
// 		CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 	}
	

}
//���ÿ���û�ctp��½���
bool CReal_SvrTradeData::CheckAllUserFinishedCTP(int & nLoginCount)
{
	bool lbRet = false;
	nLoginCount = 0;
	do 
	{
		TRADE_LOG("Start Check All User CTP")
        CSafeRwLockRead lrlock(mrwLockMapUserTrading);
		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();
        int lnFinishCount = 0;
		while(mMapUser2TradingData.end() != lIter)	
		{		
			//�ж��û��Ƿ��ǿ��û���û�а�ί�н����˺�
			int nUserID = -1;
			vector<UserInfo> lVecUeser;
// 			CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUeser);
// 			for(unsigned int i = 0;i < lVecUeser.size();i++)
// 			{
// 				if(strcmp(lVecUeser[i].szAccount,lIter->first.c_str()) == 0)
// 				{
// 					nUserID = lVecUeser[i].nUserID;
// 					break;
// 				}
// 			}
// 			TradeAccount tradeAccount;
// 			if(!CInterface_SvrUserOrg::getObj().GetTradeAccountByUserID(nUserID,tradeAccount))
// 			{
// 				TRADE_LOG("User %s Finished CTP with Empty Trader Account",lIter->first.c_str());
// 				lnFinishCount++;
// 			}
// 			else
// 			{
// 				double lnDifferTime;
// 				EnumAccountStatus  lnCtpStatus = lIter->second->GetUserCTPStatus(lnDifferTime);
// 
// 				if(lnCtpStatus == ACCOUNT_STATUS_Login ||
// 					lnCtpStatus == ACCOUNT_STATUS_CannotConnect ||
// 					lnCtpStatus == ACCOUNT_STATUS_LoginFailure)
// 				{
// 					TRADE_LOG("User %s Finished with Account CTP Status %d",lIter->first.c_str(),lnCtpStatus);
// 					//nLoginCount++;
// 					lnFinishCount++;
// 				}
// 				////���� �Ͽ�120s��ʱ�䣬����Ϊ�ǹ̶�״̬
// 				else if(lnCtpStatus ==  ACCOUNT_STATUS_Disconnected && lnDifferTime > 120)
// 				{
// 					TRADE_LOG("User %s Finished with Account CTP Status %d",lIter->first.c_str(),lnCtpStatus);
// 					lnFinishCount++;
// 				}
// 			
// 			}
			lIter++;
		}

		if(lnFinishCount == mMapUser2TradingData.size())
		{
			lbRet = true;
			TRADE_LOG("Exit Check All User CTP Finished")
	

			break;
		}

		TRADE_LOG("End Check All User CTP  Finished")
		break;
		//Sleep(1000);
	} while (true);


	return lbRet;

}
//���ÿ���û��Ƿ������˶��������뵽����״̬
bool CReal_SvrTradeData::CheckAllUserFinishedInit()
{
	bool lbRet = false;
	do 
	{
		TRADE_LOG("Start Check All User Finished")
     
        CSafeRwLockRead lrlock(mrwLockMapUserTrading);
		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();
        int lnFinishCount = 0;
		while(mMapUser2TradingData.end() != lIter)	
		{		
			//�ж��û��Ƿ��ǿ��û���û�а�ί�н����˺�
			int nUserID = -1;
			vector<UserInfo> lVecUeser;
// 			CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUeser);
// 			for(unsigned int i = 0;i < lVecUeser.size();i++)
// 			{
// 				if(strcmp(lVecUeser[i].szAccount,lIter->first.c_str()) == 0)
// 				{
// 					nUserID = lVecUeser[i].nUserID;
// 					break;
// 				}
// 			}
// 			TradeAccount tradeAccount;
// 			if(!CInterface_SvrUserOrg::getObj().GetTradeAccountByUserID(nUserID,tradeAccount))			
// 			{
// 				TRADE_LOG("User %s Finished with Empty Trader Account",lIter->first.c_str());
// 				lnFinishCount++;
// 			}
// 			else
// 			{
// 				if( lIter->second->GetUserInitStatus() == USER_STATUS_Inited )
// 				{
// 					TRADE_LOG("User %s Finished with Qry End",lIter->first.c_str());
// 					lnFinishCount++;
// 				}
// 				else 			 
// 				{
// 					double lnDifferTime;
// 					EnumAccountStatus  lnCtpStatus = lIter->second->GetUserCTPStatus(lnDifferTime);
// 
// 					//����3��״̬��ͣ��8s��ʱ�䣬����Ϊ�ǹ̶�״̬
// 					if( (lnCtpStatus== ACCOUNT_STATUS_CannotConnect ||
// 						lnCtpStatus == ACCOUNT_STATUS_LoginFailure ||
// 						lnCtpStatus == ACCOUNT_STATUS_Disconnected) &&
// 						lnDifferTime > 8)
// 					{
// 						TRADE_LOG("User %s Finished with Account CTP Status %d",lIter->first.c_str(),lnCtpStatus);
// 						lnFinishCount++;
// 					}
// 				}
// 			}
			lIter++;
		}

		if(lnFinishCount == mMapUser2TradingData.size())
		{
			lbRet = true;
			TRADE_LOG("Exit Check All User Finished")
			break;
		}

		TRADE_LOG("End Check All User Finished with %d/%d",lnFinishCount,mMapUser2TradingData.size())
		break;
		//Sleep(1000);
	} while (true);


	return lbRet;

}


//����һ���û��Ƿ��гֲ�
bool CReal_SvrTradeData::TestUserHasPosition(const std::string & nUserName)
{
	bool lbRet = false;
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nUserName);
	if(lIterUser != mMapUser2TradingData.end())
	{	
		lbRet = lIterUser->second->HasPositions();
	}
	return lbRet;
}


//����һ���û��Ƿ��иóɽ�
bool CReal_SvrTradeData::TestUserTraderExist(const PlatformStru_TradeInfo & nTraderInfo)
{
	bool lbRet = false;
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nTraderInfo.InvestorID);
	if(lIterUser != mMapUser2TradingData.end())
	{	
		TradeKey lKey(nTraderInfo);
		lbRet = lIterUser->second->TestTradeInfoExist(lKey);
	}
	return lbRet;
}
//����������һ������Ա����ί�н����˺�Ҳ�Ѳ������
CF_ERROR CReal_SvrTradeData::AddTraderUserWhenTrading(const std::string & nUserName,const std::string & nsAccountName,const std::string & nsBrokerCode)
{

	if(nUserName.empty() || nsAccountName.empty() || nsBrokerCode.empty())
		return CF_ERROR_COMMON_INPUT_PARAM;

	TRADE_LOG("param is %s ",nUserName.c_str());
	//����һ��������ڴ�
	CSafeRwLockWrite lwlock(mrwLockMapUserTrading);
	
	CreateEmptyAccountOpAndAddMap(nUserName,mMapUser2TradingData);

	//�����Լ���������
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserName);
	if(lIter != mMapUser2TradingData.end())
	{
		lIter->second->SetPlatFormInstrument(mpPlatformDataInstrument);
		lIter->second->SetPlatFormQutos(mpPlatformDataQuots);
	}
	else
	{
		TRADE_LOG("No Data %s",nUserName.c_str());
		return CF_ERROR_TRADEDATA_NO_USER_DATA;
	}

	//�鿴�Ƿ��ѳ�ʼ�����
	if(lIter->second->GetUserInitStatus() == USER_STATUS_Inited)
	{
		TRADE_LOG("Has Init %s",nUserName.c_str());
		return CF_ERROR_TRADEDATA_HAS_INITED;
	}
	

	//�鿴�Ƿ��з��ʶ��������ֱ�Ӹ�ֵ�����û��֪ͨ���̲�ѯ���ʺͱ�֤����
	CSafeRwLockWrite lwlock2(mrwLockMapAccount);
	BrokerAccountKey lKey(nsBrokerCode.c_str(),nsAccountName.c_str());
	_MapAccountData::iterator lIterAccount= mMapAccountData.find(lKey);
    if(lIterAccount == mMapAccountData.end())
	{
		return CF_ERROR_TRADEDATA_INIT_ACCOUNT_NOLOGIN; 
	}

	if(	lIterAccount->second->GetCommissonMarginReady() != CommissionMargin_ComissionAndMarinReady)
	{
		return CF_ERROR_TRADEDATA_INIT_ACCOUNT_NOCOMM;
	}


	TRADE_LOG("Has Found %s commission ",nsAccountName.c_str());

	lIter->second->SetPlatFormRates(lIterAccount->second->GetPlatFormRates());

	//���û�з����ճ����ݣ�����ɿյ��ճ�����

	if(!(lIter->second->GetUserInitStatus() & USER_STATUS_InitQryPositionReady) )
	{
		TRADE_LOG("DispathInitialDataToUser %s  ",nUserName.c_str());
		std::string lsTempTradingDay = msInstrumentDay.substr(0,4) + msInstrumentDay.substr(5,2) + msInstrumentDay.substr(8,2);	
		DispathInitialDataToUser(nUserName,lsTempTradingDay,mMapUser2TradingData);
	}

	//���ɿյı����ͳɽ�
	lIter->second->SetEqualUserInitStatus(USER_STATUS_Inited);

    //�����û�ͬ����ɣ����򱨵����ɽ�����ʱ�޷����͸��½��û�
    CReal_SvrTradeData::getObj().SetUserSynFinished(nUserName);

	
	return CF_ERROR_SUCCESS;

	//else
	//{
	//	CPlatformDataMgr_Rates * lpTemp = new CPlatformDataMgr_Rates();
	//	mMapAccountData.insert(std::make_pair(nsAccountName,lpTemp));

	//	TRADE_LOG("Has not Found %s commission %s",nsAccountName.c_str,lsTempTradingDay.c_str());
	//
	//	//֪ͨ��½��ѯ����
	//	Stru_NotifyEvent lDataEvent(EventTypeNeedSingleLoginTrade,0);			
	//	BrokerAccountKey lsBrokerAccount(nsBrokerCode.c_str(),nsAccountName.c_str());		
	//	lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
	//	CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);

	//	//����һ����Ҫ��ѯ��Լ���ʵ��¼�
	//	Stru_NotifyEvent lDataEvent(EventTypeNeedQryCommission,0);			
	//	BrokerAccountKey lsBrokerAccount(nsBrokerCode.c_str(),nsAccountName.c_str());		
	//	lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
	//	CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
	//
	//	//����һ����Ҫ��ѯ��֤���ʵ��¼�
	//	Stru_NotifyEvent lDataEvent(EventTypeNeedQryMargin,0);			
	//	BrokerAccountKey lsBrokerAccount(nsBrokerCode.c_str(),nsAccountName.c_str());		
	//	lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
	//	CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
	//}	

	
}
//����������һ������Ա����ί�н����˺�Ҳ�Ѳ������
CF_ERROR CReal_SvrTradeData::AddInitTraderUser(const std::string & nUserName,
											   const std::string & nsAccountName,
											   const std::string & nsBrokerCode)
{

	if(nUserName.empty() )
		return CF_ERROR_COMMON_INPUT_PARAM;

	TRADE_LOG("AddInitTraderUser param is %s ",nUserName.c_str());
	//����һ��������ڴ�
	CSafeRwLockWrite lwlock(mrwLockMapUserTrading);

	CreateEmptyAccountOpAndAddMap(nUserName,mMapUser2TradingData);

	//�����Լ���������
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserName);
	if(lIter != mMapUser2TradingData.end())
	{
		lIter->second->SetPlatFormInstrument(mpPlatformDataInstrument);
		lIter->second->SetPlatFormQutos(mpPlatformDataQuots);
	}
	else
	{
		TRADE_LOG("No instrument Data %s",nUserName.c_str());
		return CF_ERROR_TRADEDATA_NO_USER_DATA;
	}


	//�鿴�Ƿ��з��ʶ��������ֱ�Ӹ�ֵ�����û��֪ͨ���̲�ѯ���ʺͱ�֤����
	CSafeRwLockWrite lwlock2(mrwLockMapAccount);
	BrokerAccountKey lKey(nsBrokerCode.c_str(),nsAccountName.c_str());
	_MapAccountData::iterator lIterAccount= mMapAccountData.find(lKey);
	if(lIterAccount == mMapAccountData.end())
	{
		return CF_ERROR_TRADEDATA_INIT_ACCOUNT_NOLOGIN; 
	}

	if(	lIterAccount->second->GetCommissonMarginReady() != CommissionMargin_ComissionAndMarinReady)
	{
		return CF_ERROR_TRADEDATA_INIT_ACCOUNT_NOCOMM;
	}


	TRADE_LOG("Has Found %s commission ",nsAccountName.c_str());

	lIter->second->SetPlatFormRates(lIterAccount->second->GetPlatFormRates());

	//���û�з����ճ����ݣ�����ɿյ��ճ�����

	if(!(lIter->second->GetUserInitStatus() & USER_STATUS_InitQryPositionReady) )
	{
		TRADE_LOG("DispathInitialDataToUser %s  ",nUserName.c_str());
		std::string lsTempTradingDay = msInstrumentDay.substr(0,4) + msInstrumentDay.substr(5,2) + msInstrumentDay.substr(8,2);	
		DispathInitialDataToUser(nUserName,lsTempTradingDay,mMapUser2TradingData);
	}

	//���ɿյı����ͳɽ�
	lIter->second->SetEqualUserInitStatus(USER_STATUS_Inited);

	
	return CF_ERROR_SUCCESS;

	


}

bool CReal_SvrTradeData::CheckInitDataHasExpiredInstruementPosition()
{
	CSafeLock lLock(m_mutexInitData);
    bool lbRet = false;
	//��ȡ���û��ĳֲ�
	std::map<std::string,vector<PlatformStru_Position>>::iterator lIteVecPos = mUserInitPosition.begin();
	for(;lIteVecPos != mUserInitPosition.end();lIteVecPos++)
	{		

		for(unsigned int i = 0;i < lIteVecPos->second.size();i++ )
		{						
			//�жϸóֲֵĺ�Լ�Ƿ����
			PlatformStru_InstrumentInfo InstrumentInfo;
			if(mpPlatformDataInstrument != NULL &&
				false == mpPlatformDataInstrument->GetInstrumentInfo(lIteVecPos->second[i].InstrumentID,InstrumentInfo))
			{
				sErrorInfo lsError;
				memset(&lsError,0,sizeof(lsError));
				lsError.meErrorType = FirstLevelError;
				sprintf(lsError.msError,
					ERROR_POSITION_FORMAT,
					lIteVecPos->second[i].InvestorID,
					lIteVecPos->second[i].InstrumentID,
					lIteVecPos->second[i].PosiDirection == THOST_FTDC_PD_Long ? "��ͷ" : "��ͷ",
					lIteVecPos->second[i].Position);
				//TRADE_LOG(lsError.msError);
				//CInterface_SvrNotifyAndAsk::getObj().PushErrorInfo(lsError);
				lbRet = true;
			}				

		}
	}	
	return lbRet;

}


//����ʱ�����е�δ�ɽ������ֳɽ�������˵ı���ȫ����Ϊ�ѳ���
CF_ERROR CReal_SvrTradeData::CancelAllUnkonwnOrder()
{
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);	
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();

	for(;mMapUser2TradingData.end() != lIter ;lIter++)	
	{
		//lIter->second->CancelAllUnkonwnOrders();
		//���������pushorder���Ա����͵��ӷ�����
		std::vector<PlatformStru_OrderInfo> lvecOrder;
		lIter->second->GetAllOrders(lvecOrder);
		for(unsigned int i = 0; i < lvecOrder.size(); i++)
		{
			if(lvecOrder[i].OrderStatus == THOST_FTDC_OST_PartTradedQueueing ||
				lvecOrder[i].OrderStatus == THOST_FTDC_OST_NoTradeQueueing||
				lvecOrder[i].OrderStatus == THOST_FTDC_OST_PartTradedNotQueueing||
				lvecOrder[i].OrderStatus == THOST_FTDC_OST_NoTradeNotQueueing ||
				lvecOrder[i].OrderStatus == THOST_FTDC_OST_Unknown )
			{
				lvecOrder[i].OrderStatus = THOST_FTDC_OST_Canceled;
				lvecOrder[i].ExStatus = PlatformStru_OrderInfo::ExSta_none;
				lvecOrder[i].freezeMargin = util::GetDoubleInvalidValue();
				lvecOrder[i].freezeExchMargin = util::GetDoubleInvalidValue();
				lvecOrder[i].troubleMoney = util::GetDoubleInvalidValue();
				lvecOrder[i].freezeExchMargin = util::GetDoubleInvalidValue();
				lvecOrder[i].Leg1FreezeMargin = util::GetDoubleInvalidValue();       //��Ϻ�Լ�����ĵ�һ�ȶ��ᱣ֤��
				lvecOrder[i].Leg1FreezeExchMargin = util::GetDoubleInvalidValue();   //��Ϻ�Լ�����ĵ�һ�ȶ��ύ������֤��
				lvecOrder[i].Leg1TroubleMoney = util::GetDoubleInvalidValue();       //��Ϻ�Լ�����ĵ�һ�ȶ���������
				lvecOrder[i].Leg2FreezeMargin = util::GetDoubleInvalidValue();      //��Ϻ�Լ�����ĵڶ��ȶ��ᱣ֤��
				lvecOrder[i].Leg2FreezeExchMargin = util::GetDoubleInvalidValue();   //��Ϻ�Լ�����ĵڶ��ȶ��ύ������֤��
				lvecOrder[i].Leg2TroubleMoney = util::GetDoubleInvalidValue();      //��Ϻ�Լ�����ĵڶ��ȶ���������
				SYSTEMTIME sysTime;
				::GetLocalTime(&sysTime);
				sprintf_s(lvecOrder[i].CancelTime, sizeof(lvecOrder[i].CancelTime), "%02d:%02d:%02d", 
					sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
				strncpy(lvecOrder[i].UpdateTime, lvecOrder[i].CancelTime, sizeof(lvecOrder[i].UpdateTime));
				strcpy(lvecOrder[i].StatusMsg,"����������");
				PushOrder(lvecOrder[i]);
			}
		}
		
	}
	return CF_ERROR_SUCCESS;
}

void CReal_SvrTradeData::SetUserSynFinished(const std::string & nsUserName)
{
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);	
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nsUserName);
	if(lIter != mMapUser2TradingData.end())
		lIter->second->SetSyncFinished(true);

}
bool CReal_SvrTradeData::GetUserSynFinished(const std::string & nsUserName)
{
	bool lbRet = false;
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);	
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nsUserName);
	if(lIter != mMapUser2TradingData.end())
		lbRet = lIter->second->IsSyncFinished();

	return lbRet;

}
void CReal_SvrTradeData::SetAccountSynFinished(const BrokerAccountKey & nKey)
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	_MapAccountData::iterator lIter =  mMapAccountData.find(nKey);
	if(lIter != mMapAccountData.end())
		lIter->second->SetSyncFinished(true);
}
bool  CReal_SvrTradeData::SlaverInitAfterGetTradingDay(std::string szUser,std::string szBrokerCode,
                                                       std::string szTradeAccount,CDataEntity* pDataEntity, 
                                                       CMemCache* pCmdCache, double dFund )
{
    mpCache = pCmdCache;

    //�������
    mpCache->FlushAll();

    InitUserMapWithEmpty(szUser,szBrokerCode,szTradeAccount,mMapUser2TradingData);  

    std::string szTradingDay = "20140613";
    SetCurrentTradingDay(szTradingDay);

    //ת���ճ�����
    std::string lsTempTradingDay;
    std::string lsLastTradingDay;
    GetCurrentTradingDay(lsTempTradingDay);
    TRADE_LOG("SlaverInitAfterGetTradingDay %s",lsTempTradingDay.c_str());

    mpTradingDayManager.LoadHistoryTradingDay();
    mpTradingDayManager.LoadHistorySettmentedDay();

    mpTradingDayManager.GetLastTradingData(lsTempTradingDay,lsLastTradingDay);
    //���ú�Լ�ͷ��ʵ�����
    msInstrumentDay = lsTempTradingDay;

    //�����ݿ�ת�غ�Լ
    //�����Լ
    //������ݿ��н���ĺ�Լ�����ȡ���ݿ�
    //������ݿ���û�н���ĺ�Լ�������̷��Ͳ�ѯ��Լ���¼�
    vector<PlatformStru_InstrumentInfo> lVecInstrument;	
    std::set<std::string> lSetInstrumentId;
    TRADE_LOG("LoadTodayInstrument From DataBase Faild Send Qry Instrument CMD:date is %s",lsTempTradingDay.c_str());
          
    std::map<std::string, PlatformStru_InstrumentInfo>::const_iterator itInstr;
    const std::map<std::string, PlatformStru_InstrumentInfo>& mapInstrument = pDataEntity->GetInstrument();
    for(itInstr = mapInstrument.begin(); itInstr != mapInstrument.end(); itInstr++) 
    {
        CReal_SvrTradeData::getObj().SetInstrumentInfo((PlatformStru_InstrumentInfo&)itInstr->second);
    }

    //ȥ�������ظ��ĺ�Լ
    mpPlatformDataInstrument->GetInstrumentList(lSetInstrumentId);

    //���������û���		
    InterlockedExchange(reinterpret_cast<volatile long*>(&mbInstrumentsReady),DataReady_Ready);

    //Ϊ���ʺ����ú�Լ
    SetPlatformObjectToAllAccount();
    //Ϊ�û��˻����ú�Լ
    SetPlatformObjectToAllUser(mMapUser2TradingData);
  	

    //�����ݿ�ת�ر�֤��
    //�����ݿ�ת�ط���
    //ȥ�������ظ��ĺ�Լ
    bool lbCommisstionLoadSuccess = false;
    bool lbMarginRatesLoadSuccess = false;

    BrokerAccountKey lKey(szBrokerCode.c_str(),szTradeAccount.c_str());	
    _MapAccountData::iterator lIterReady =  mMapAccountData.find(lKey);

    lbCommisstionLoadSuccess = true;
    std::map<std::string, PlatformStru_InstrumentCommissionRate>::iterator itCS;
    std::map<std::string, PlatformStru_InstrumentCommissionRate> mapCS = pDataEntity->GetCommissionRate();
    for(itCS = mapCS.begin(); itCS != mapCS.end(); itCS++)
    {
        strncpy(itCS->second.BrokerID, szBrokerCode.c_str(), sizeof(itCS->second.BrokerID)-1);
        strncpy(itCS->second.InvestorID, szTradeAccount.c_str(), sizeof(itCS->second.InvestorID)-1);
        SetCommissionRate((PlatformStru_InstrumentCommissionRate&)itCS->second);
    }

    if(mMapAccountData.end() != lIterReady)
    {
        lIterReady->second->SetCommissionMarginReady(CommissionMargin_ComissionReady);	
    }


    lbMarginRatesLoadSuccess = true;
    std::map<std::string, PlatformStru_InstrumentMarginRate>::iterator itMG;
    std::map<std::string, PlatformStru_InstrumentMarginRate> mapMG = pDataEntity->GetMarginRate();
    for(itMG = mapMG.begin(); itMG != mapMG.end(); itMG++) 
    {
        strncpy(itMG->second.BrokerID, szBrokerCode.c_str(), sizeof(itMG->second.BrokerID)-1);
        strncpy(itMG->second.InvestorID, szTradeAccount.c_str(), sizeof(itMG->second.InvestorID)-1);
        SetMarginRate((PlatformStru_InstrumentMarginRate&)itMG->second);
    }

    //����״̬		
    if(mMapAccountData.end() != lIterReady)
    {
        lIterReady->second->SetCommissionMarginReady(CommissionMargin_MarginReady);			
    }


    if(lbMarginRatesLoadSuccess && lbCommisstionLoadSuccess)
    {		
        TRADE_LOG("LoadToday Commission And Margin From DataBase Success,and send Qry Trade Cmd:%s AccountID %s",szTradeAccount,lsTempTradingDay.c_str());

        //���÷��ʶ��󵽶�Ӧ���û�����
        SetRatesObjectToAccountRelationUserObject(szBrokerCode,szTradeAccount,szUser);
    }

    //װ�����ʺŵ��ʽ�
    PlatformStru_TradingAccountInfo lAccountInfo;
    memset(&lAccountInfo,0,sizeof(PlatformStru_TradingAccountInfo));
    lIterReady->second->SetQryFundInfo(lAccountInfo);

    //װ�����ʺŵĳֲ�
    vector<PlatformStru_Position> lvecPosition;
    if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayAccountPositions("TRADEDATA_A_P_INIT",lBroker.szBrokerCode,lIter->szTradeAccount,lsTempTradingDay,lvecPosition)*/)
    {
        //����״̬
        if(mMapAccountData.end() != lIterReady)
        {
            vector<PlatformStru_Position>::iterator lIte = lvecPosition.begin();
            for(;lIte != lvecPosition.end();lIte++)
            {		
                strcpy(lIte->Account,lIte->InvestorID);
                lIterReady->second->QryPositionReach(*lIte,0,false);	
            }	
            PlatformStru_Position lPostion;
            lIterReady->second->QryPositionReach(lPostion,-100,true);
        }			
    }
    else
    {
        //PUSH_LOG_ERROR(Logger,false,true,"���ݿ���û�и����ʺ�%s�ĳֲ�",szTradeAccount);
        if(mMapAccountData.end() != lIterReady)
        {			
            PlatformStru_Position lPostion;
            lIterReady->second->QryPositionReach(lPostion,-100,true);
        }		
    }

    //װ�����ʺŵĳֲ���ϸ
    vector<PlatformStru_PositionDetail> lvecPositionDetail;
    if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayAccountPositionDetails("TRADEDATA_A_PD_INIT",lBroker.szBrokerCode,lIter->szTradeAccount,lsTempTradingDay,lvecPositionDetail)*/)
    {
        //����״̬
        if(mMapAccountData.end() != lIterReady)
        {
            vector<PlatformStru_PositionDetail>::iterator lIte = lvecPositionDetail.begin();
            for(;lIte != lvecPositionDetail.end();lIte++)
            {			
                strcpy(lIte->Account,lIte->InvestorID);
                lIterReady->second->QryPositionDetailReach(*lIte,0,false);	
            }	
            PlatformStru_PositionDetail lPostionDetail;
            lIterReady->second->QryPositionDetailReach(lPostionDetail,-100,true);
        }			
    }
    else
    {
        //PUSH_LOG_ERROR(Logger,false,true,"���ݿ���û�и����ʺ�%s�ĳֲ���ϸ",szTradeAccount);
        if(mMapAccountData.end() != lIterReady)
        {			
            PlatformStru_PositionDetail lPostionDetail;
            lIterReady->second->QryPositionDetailReach(lPostionDetail,-100,true);
        }		
    }


    //װ�����ʺŵĳɽ�
    vector<PlatformStru_TradeInfo> lvecTrader;
    if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayAccountTrades("TRADEDATA_A_T_INIT",lBroker.szBrokerCode,lIter->szTradeAccount,lsTempTradingDay,lvecTrader)*/)
    {
        //����״̬
        if(mMapAccountData.end() != lIterReady)
        {
            vector<PlatformStru_TradeInfo>::iterator lIte = lvecTrader.begin();
            for(;lIte != lvecTrader.end();lIte++)
            {				
                strcpy(lIte->Account,lIte->InvestorID);
                lIterReady->second->QryTradeReach(*lIte,0,false);	
                lIterReady->second->SetLoadTraderUpdateSeq(lIte->UpdateSeq);
            }	
            PlatformStru_TradeInfo lTrader;
            lIterReady->second->QryTradeReach(lTrader,-100,true);	

        }			
    }
    else
    {
        //PUSH_LOG_ERROR(Logger,false,true,"���ݿ���û�и����ʺ�%s�ĳɽ�",szTradeAccount);
        if(mMapAccountData.end() != lIterReady)
        {			
            PlatformStru_TradeInfo lTrader;
            lIterReady->second->QryTradeReach(lTrader,-100,true);	
        }			
    }

    //װ�����ʺŵı���
    vector<PlatformStru_OrderInfo> lvecOrder;
    if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayAccountOrders("TRADEDATA_A_O_INIT",lBroker.szBrokerCode,lIter->szTradeAccount,lsTempTradingDay,lvecOrder)*/)
    {
        //����״̬
        if(mMapAccountData.end() != lIterReady)
        {
            vector<PlatformStru_OrderInfo>::iterator lIte = lvecOrder.begin();
            for(;lIte != lvecOrder.end();lIte++)
            {		
                strcpy(lIte->Account,lIte->InvestorID);			
                lIte->troubleMoney = util::GetDoubleInvalidValue();	
                lIte->freezeMargin = util::GetDoubleInvalidValue();
                lIterReady->second->QryOrderReach(*lIte,0,false);
                lIterReady->second->SetLoadOrderUpdateSeq(lIte->UpdateSeq);
            }	
            PlatformStru_OrderInfo lOrder;
            lIterReady->second->QryOrderReach(lOrder,-100,true);
        }			
    }
    else
    {
        //PUSH_LOG_ERROR(Logger,false,true,"���ݿ���û�и����ʺ�%s�ı���",szTradeAccount);
        if(mMapAccountData.end() != lIterReady)
        {			
            PlatformStru_OrderInfo lOrder;
            lIterReady->second->QryOrderReach(lOrder,-100,true);
        }			
    }

//     //װ�����ʺŵı���
//     vector<PlatformStru_OrderInfo> lvecSecondOrder;
//     if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayAccountOrders("CTP_ORDERS",lBroker.szBrokerCode,lIter->szTradeAccount,lsTempTradingDay,lvecSecondOrder))
//     {
//         //����״̬		
//         if(mMapAccountData.end() != lIterReady)
//         {	
//             //����ǰ��ת�ص�updateseq�ı����Żᱻpush
//             int lnLastSeq=0;
//             lIterReady->second->GetLoadOrderUpdateSeq(lnLastSeq);
//             vector<PlatformStru_OrderInfo>::iterator lIte = lvecSecondOrder.begin();
//             for(;lIte != lvecSecondOrder.end();lIte++)
//             {	
// 
//                 if(lIte->UpdateSeq > lnLastSeq)
//                 {
//                     lIte->troubleMoney = util::GetDoubleInvalidValue();	
//                     lIte->freezeMargin = util::GetDoubleInvalidValue();
//                     lIterReady->second->PushOrder(*lIte);
//                     lIterReady->second->SetLoadOrderUpdateSeq(lIte->UpdateSeq);
//                 }
//             }	
// 
//         }	
//     }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //װ�����ʺŵ�����
    std::string lsTempDay = msCurrentTradingDay.substr(0,4) + msCurrentTradingDay.substr(5,2) + msCurrentTradingDay.substr(8,2);	
    _MapUserTradeData::iterator lIterUser =  mMapUser2TradingData.begin();
    for(;lIterUser != mMapUser2TradingData.end();lIterUser++)
    {
        //װ�����ʺŵ��ʽ�
        PlatformStru_TradingAccountInfo lAccountInfo;	
        if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayUserFundInfo("SETTLEMENT_FUNDINIT",lIterUser->first,lsLastTradingDay,lAccountInfo)*/)
        {
            //����״̬
            lIterUser->second->SetQryFundInfo(lAccountInfo);
            if(mbNeedCache)
                pCmdCache->CacheFund(lAccountInfo);
        }
        else
        {
            //PUSH_LOG_ERROR(Logger,false,true,"���ݿ���û�и��û�%s���ʽ�",lIterUser->first.c_str());

            //���ÿ��ʽ��accountid
            PlatformStru_TradingAccountInfo lFundInfo;    
            lFundInfo.clear();
            strcpy(lFundInfo.BrokerID,"0000");
            strcpy(lFundInfo.AccountID,lIterUser->first.c_str());	
            strcpy(lFundInfo.TradingDay,lsTempDay.c_str());
            //lFundInfo.Available = dFund;
            //lFundInfo.StaticProfit = dFund;
            //lFundInfo.DynamicProfit = dFund;
            lFundInfo.PreBalance = dFund;
            lIterUser->second->SetQryFundInfo(lFundInfo);
            if(mbNeedCache)
                pCmdCache->CacheFund(lAccountInfo);
        }


        //���ó�ʼ�ʽ�
        double ldbVal = dFund;
        LoadInitialInitFundFromDB(lIterUser->first,ldbVal);
        lIterUser->second->SetInitFund(ldbVal);

        //װ�س����
        //��ȡ�û�����ĳ����
        vector<sFundInOut> lvecFundInOut;        
        sQryFundInOut ls;
        strcpy(ls.msStartDay,msInstrumentDay.c_str());
        strcpy(ls.msEndDay,msInstrumentDay.c_str());
        strcpy(ls.mUserID,lIterUser->first.c_str());
//         sFundInOut inout;
//         strcpy(inout.mUserID,szUser.c_str());     //����Ա�˻�
//         eInOut       meInOut;     //������
//         double       mdbVolume;   //���
//         char         msDesc[200]; //��ע
//         _UserIDType  mOpAdminID;  //�����Ĺ���Ա�˻�
//         char		 msDay[11];   //��������
//         char		 msTime[9];   //����ʱ��
//         lvecFundInOut.push_back(ls);
        //mpDBOpr->LoadQryFundInOut(ls,lvecFundInOut);
        for(unsigned int i = 0; i < lvecFundInOut.size();i++)
        {
            lIterUser->second->UserFundInOut(lvecFundInOut[i]);
        }

        //�����ʽ�
        if(mbNeedCache)
        {
            PlatformStru_TradingAccountInfo lFundInfo;
            lIterUser->second->GetFundInfo(lFundInfo);
            pCmdCache->CacheFund(lAccountInfo);
        }

        //װ�����ʺŵĳֲ�
        vector<PlatformStru_Position> lvecPosition;
        if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayUserPositions("SETTLEMENT_POSITIONINIT",lIterUser->first,lsLastTradingDay,lvecPosition)*/)
        {
            //����״̬
            vector<PlatformStru_Position>::iterator lIte = lvecPosition.begin();
            for(;lIte != lvecPosition.end();lIte++)
            {	
                strcpy(lIte->TradingDay,lsTempDay.c_str());
                PlatformStru_InstrumentMarginRate lMargin;
                lMargin.clear();
                if(0 == lIterUser->second->GetMarginRate(lIte->InstrumentID,lMargin))
                {
                    lIte->MarginRateByMoney = 
                        lIte->PosiDirection == THOST_FTDC_PD_Long ? lMargin.LongMarginRatioByMoney : lMargin.ShortMarginRatioByMoney;
                    lIte->MarginRateByMoney = 
                        lIte->PosiDirection == THOST_FTDC_PD_Long ? lMargin.LongMarginRatioByVolume : lMargin.ShortMarginRatioByVolume;

                }			


                strcpy(lIte->Account,lIte->InvestorID);
                lIterUser->second->QryPositionReach(*lIte,0,false);	
                if(mbNeedCache)
                {
                    pCmdCache->CachePosition(*lIte);
                    TRADE_LOG("PushTrade CachePosition InstrumentID=%s CanCloseTodayVolume=%d CanCloseVolume=%d CanCloseydVolume=%d\
                              CloseProfit=%.4f CloseTodayOrderVolume=%d CloseOrderVolume=%d CloseVolume=%d InvestorID=%s CanCloseydVolume=%d\
                              OpenVolume=%d Position=%d TodayPosition=%d YdPosition=%d",
                              lIte->InstrumentID,lIte->CanCloseTodayVolume,lIte->CanCloseVolume,lIte->CanCloseydVolume,
                              lIte->CloseProfit,lIte->CloseTodayOrderVolume,lIte->CloseOrderVolume,lIte->CloseVolume,
                              lIte->InvestorID,lIte->OpenVolume,lIte->Position,lIte->TodayPosition,lIte->YdPosition);

                }
            }	
            PlatformStru_Position lPostion;
            lIterUser->second->QryPositionReach(lPostion,-100,true);
        }
        else
        {
            //PUSH_LOG_ERROR(Logger,false,true,"���ݿ���û�и��û�%s�ĳֲ�",lIterUser->first.c_str());
            PlatformStru_Position lTemp;
            lIterUser->second->QryPositionReach(lTemp,-100,true);
        }

        //װ�����ʺŵĳֲ���ϸ
        vector<PlatformStru_PositionDetail> lvecPositionDetail;
        if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayUserPositionDetails("SETTLEMENT_POSITIONDETAILINIT",lIterUser->first,lsLastTradingDay,lvecPositionDetail)*/)
        {
            //����״̬
            vector<PlatformStru_PositionDetail>::iterator lIte = lvecPositionDetail.begin();
            for(;lIte != lvecPositionDetail.end();lIte++)
            {				
                strcpy(lIte->Account,lIte->InvestorID);
                strcpy(lIte->TradingDay,lsTempDay.c_str());
                PlatformStru_InstrumentMarginRate lMargin;
                lMargin.clear();
                if(0 == lIterUser->second->GetMarginRate(lIte->InstrumentID,lMargin))
                {
                    lIte->MarginRateByMoney = 
                        lIte->Direction == THOST_FTDC_D_Buy ? lMargin.LongMarginRatioByMoney : lMargin.ShortMarginRatioByMoney;
                    lIte->MarginRateByMoney = 
                        lIte->Direction == THOST_FTDC_D_Buy ? lMargin.LongMarginRatioByVolume : lMargin.ShortMarginRatioByVolume;
                }			

                lIterUser->second->QryPositionDetailReach(*lIte,0,false);
                if(mbNeedCache)
                    pCmdCache->CachePositionDetail(*lIte);
            }	
            PlatformStru_PositionDetail lPostionDetail;		
            lIterUser->second->QryPositionDetailReach(lPostionDetail,-100,true);

        }
        else
        {
            //PUSH_LOG_ERROR(Logger,false,true,"���ݿ���û�и��û�%s�ĳֲ���ϸ",lIterUser->first.c_str());
            PlatformStru_PositionDetail lPostionDetail;		
            lIterUser->second->QryPositionDetailReach(lPostionDetail,-100,true);
        }


        //װ�����ʺŵĳɽ�
        vector<PlatformStru_TradeInfo> lvecTrader;
        if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayUserTrades("TRADEDATA_TRADES",lIterUser->first,lsTempTradingDay,lvecTrader)*/)
        {
            //����״̬
            vector<PlatformStru_TradeInfo>::iterator lIte = lvecTrader.begin();
            for(;lIte != lvecTrader.end();lIte++)
            {
                strcpy(lIte->Account,lIte->InvestorID);
                lIterUser->second->SetLoadTraderUpdateSeq(lIte->UpdateSeq);				
                lIterUser->second->QryTradeReach(*lIte,0,false);	

            }	
            PlatformStru_TradeInfo lTrader;
            lIterUser->second->QryTradeReach(lTrader,-100,true);			
        }
        else
        {
            //PUSH_LOG_ERROR(Logger,false,true,"���ݿ���û�и��û�%s�ĳɽ�",lIterUser->first.c_str());
            PlatformStru_TradeInfo lTrader;
            lIterUser->second->QryTradeReach(lTrader,-100,true);	
            //continue;
        }

        //װ�����ʺŵı���
        vector<PlatformStru_OrderInfo> lvecOrder;
        if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayUserOrders("TRADEDATA_ORDERS",lIterUser->first,lsTempTradingDay,lvecOrder)*/)
        {
            //����״̬
            vector<PlatformStru_OrderInfo>::iterator lIte = lvecOrder.begin();
            for(;lIte != lvecOrder.end();lIte++)
            {		
                strcpy(lIte->Account,lIte->InvestorID);			
                lIte->troubleMoney = util::GetDoubleInvalidValue();	
                lIte->freezeMargin = util::GetDoubleInvalidValue();
                lIterUser->second->SetLoadOrderUpdateSeq(lIte->UpdateSeq);
                lIterUser->second->QryOrderReach(*lIte,0,false);

                if(mbNeedCache)
                    pCmdCache->CacheOrder(*lIte);
            }	
            PlatformStru_OrderInfo lOrder;
            lIterUser->second->QryOrderReach(lOrder,-100,true);

        }
        else
        {
            //PUSH_LOG_ERROR(Logger,false,true,"���ݿ���û�и��û�%s�ı���",lIterUser->first.c_str());
            PlatformStru_OrderInfo lOrder;
            lIterUser->second->QryOrderReach(lOrder,-100,true);
        }
    }

    SetTraderDataStatus(TradeDataStatusTrading);

    return true;
}

//���ÿ���û����������Ƿ�ͬ�����
bool CReal_SvrTradeData::CheckAllUserAccountSyn()
{
	bool lbRet = false;
	do 
	{
		TRADE_LOG("Start Check All User Finished")

// 		CSafeRwLockRead lrlock(mrwLockMapUserTrading);
// 		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();
// 		int lnFinishCount = 0;
// 		while(mMapUser2TradingData.end() != lIter)	
// 		{		
// 			//�ж��û��Ƿ��ǿ��û���û�а�ί�н����˺�
// 			int nUserID = -1;
// 			vector<UserInfo> lVecUeser;
// 			CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUeser);
// 			for(unsigned int i = 0;i < lVecUeser.size();i++)
// 			{
// 				if(strcmp(lVecUeser[i].szAccount,lIter->first.c_str()) == 0)
// 				{
// 					nUserID = lVecUeser[i].nUserID;
// 					break;
// 				}
// 			}
// 			TradeAccount tradeAccount;
// 			if(!CInterface_SvrUserOrg::getObj().GetTradeAccountByUserID(nUserID,tradeAccount))			
// 			{
// 				TRADE_LOG("User %s Finished with Empty Trader Account",lIter->first.c_str());
// 				lnFinishCount++;
// 			}
// 			else
// 			{
// 				if( lIter->second->IsSyncFinished()  )
// 				{
// 					TRADE_LOG("User %s Finished with Qry End",lIter->first.c_str());
// 					lnFinishCount++;
// 				}
// 				
// 			}
// 			lIter++;
// 		}
// 
// 		if(lnFinishCount == mMapUser2TradingData.size())
// 		{
// 			lbRet = true;
// 			TRADE_LOG("Exit Check All User Finished")
// 				break;
// 		}

		//TRADE_LOG("End Check All User Finished with %d/%d",lnFinishCount,mMapUser2TradingData.size())
			break;
		//Sleep(1000);
	} while (true);


	return lbRet;

}

void CReal_SvrTradeData::AddCommand(const std::string & nsCommand)
{
	CSafeLock ls(mCommandMutex);
	if(!nsCommand.empty())
	{
		mCommandList.push(nsCommand);	
		//PostThreadMessage(g_idThread,TMESSAGE_START_DEAL_CACHE_CHANNEL_COMMAND,0,0);
	}
}

void CReal_SvrTradeData::DealCommand()
{
	CSafeLock ls(mCommandMutex);
	while(mCommandList.size() != 0)
	{
		std::string lCommandStr = mCommandList.front();
		//if(lCommandStr is orderInput)
		//  send master a orderinput 
		//if(lCommandStr is orderAction)
		//  send master a orderaction
		//if(lCommandStr is heartbeat)
		// update user:strategy updatetime
		 std::size_t found = std::string::npos;
		 if( (found = lCommandStr.find(ORDERINPUT_CHANNEL_NAME)) != std::string::npos)
		 {
			 //��ȡ���ݿ��ȡ����updateseq
// 			 CInterface_SvrTcp::getObj().SendPkgData(mMasterSocket,
// 				 Cmd_TradeExcute_StategyInputOrder_Req,
// 				 (void*)lCommandStr.c_str(),
// 				 lCommandStr.size()+1,
// 				 0,
// 				 0,
// 				 0,
// 				 0,
// 				 0,0);
		 }
		 else if((found = lCommandStr.find(ORDERACTION_CHANNEL_NAME)) != std::string::npos) 
		 {
			 //��ȡ���ݿ��ȡ����updateseq
// 			 CInterface_SvrTcp::getObj().SendPkgData(mMasterSocket,
// 				 Cmd_TradeExcute_StategyActionOrder_Req,
// 				 (void*)lCommandStr.c_str(),
// 				 lCommandStr.size()+1,
// 				 0,
// 				 0,
// 				 0,
// 				 0,
// 				 0,0);
		 }
		 else  if((found = lCommandStr.find(HEARTBEAT_CHANNEL_NAME)) != std::string::npos) 
		 {
			 //set heartbeat updatetime
			/* time_t curtime=time(NULL);
			 CSafeLock llock(mUpdateTimeMapMutex);
			 bool lbRet = false;
			 std::string lsKey;
			 mStrategyInstanceUpdateTimeMap[lsKey] = curtime;*/
		 }
		mCommandList.pop();
	}
	return;
}

void CReal_SvrTradeData::AddCommandCallback(const std::string & nsCommand)
{
	CReal_SvrTradeData::getObj().AddCommand(nsCommand);
}
bool CReal_SvrTradeData::GetStategyUserInstanceUpdateTime(const std::string & nsUserName,
											  const std::string & nsStategyName,
											  time_t & nsUpdateTime) 
{
	CSafeLock llock(mUpdateTimeMapMutex);
	bool lbRet = false;
	std::string lsKey = nsUserName + ":"+nsStategyName;
	std::map<std::string ,time_t>::iterator lIter = mStrategyInstanceUpdateTimeMap.find(lsKey);
	if(lIter != mStrategyInstanceUpdateTimeMap.end())
	{
		nsUpdateTime = lIter->second;
		lbRet = true;
	}
	return lbRet;		
}