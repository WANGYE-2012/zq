#include "stdafx.h"
#include "SocketBase.h"
#include "tools_ansi.h"
#include "SvrSettlement.h"
#include "SvrSettlementBusiness.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include "..\SvrNotifyAndAsk\Interface_SvrNotifyAndAsk.h"
#include "CommonMacro.h"
#include "../SvrDBOpr/Interface_SvrDBOpr.h"
#include "../SvrTradeData/Interface_SvrTradeData.h"
#include "..\SvrMsg\Interface_SvrMsg.h"
#include "..\SvrUserOrg\Interface_SvrUserOrg.h"
#include "..\SvrBrokerInfo\Interface_SvrBrokerInfo.h"
#include "CommonErrorCode.h"
#include "CommonDef.h"
#include "SimpleWriteLog.h"
#include <time.h>
#include <vector>
#include <iostream>
#include <fstream>



#define OPENTIME     "0:00"
#define CLOSETIME    "15:00"
#define FIELD_SETTLEMENT     "�����"
#define FIELD_CLOSERATIOBYMONEY     "ƽ����������"
#define FIELD_CLOSERATIOBYVOLUME     "ƽ��������"
#define FIELD_OPENRATIOBYMONEY     "������������"
#define FIELD_OPENRATIOBYVOLUME     "����������"
#define FIELD_CLOSETODAYRATIOBYMONEY     "ƽ����������"
#define FIELD_CLOSETODAYRATIOBYVOLUME     "ƽ��������"
#define FIELD_LONGMARGINRATIOBYMONEY     "��ͷ��֤����"
#define FIELD_LONGMARGINRATIOBYVOLUME     "��ͷ��֤���"
#define FIELD_SHORTMARGINRATIOBYMONEY     "��ͷ��֤����"
#define FIELD_SHORTMARGINRATIOBYVOLUME     "��ͷ��֤���"
#define FIELD_STATICPROFIT                  "��̬Ȩ��"
#define FIELD_WITHDRAW     "����"
#define FIELD_DEPOSIT     "���"
#define FIELD_DYNAMICPROFIT     "��̬Ȩ��"
#define FIELD_AVAILABLE     "�����ʽ�"
#define FIELD_CLOSEPROFIT     "ƽ��ӯ��"
#define FIELD_POSITIONPROFIT     "�ֲ�ӯ��"
#define FIELD_COMMISSION     "������"
#define FIELD_CURRMARGIN     "��֤��"

#define __CUSTOM_FILE__    (std::string(__FILE__).substr(std::string(__FILE__).rfind('\\')+1).c_str())
#define LOG_INFO(fmt, ...) \
	do{\
	if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("Settlement", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)

SvrSettlementBusiness* SvrSettlementBusiness::m_pObj=NULL;
SvrSettlementBusiness::SvrSettlementBusiness(void)
{

	//m_bHaveSettlePrice = false;
	m_pSettlementCalc = new CSettlementCalc();
	InvalideValue=util::GetDoubleInvalidValue();
	m_SettlementStatus = No_Settlementing;
	CInterface_SvrTradeData::getObj().GetCurrentTradingDay(msTodayString);
	m_bSettlement =false;
	m_bModifiedTrade=false;
	m_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE,"Settlement.log");
	m_SettleSocket = INVALID_SOCKET;
}

SvrSettlementBusiness::~SvrSettlementBusiness(void)
{
	if(m_pWriteLog!=NULL)
	{
		delete m_pWriteLog;
		m_pWriteLog = NULL;
	}
}
SvrSettlementBusiness& SvrSettlementBusiness::getObj(void)
{
	//g_mutex.lock();
	if (NULL == m_pObj)    
		m_pObj = new SvrSettlementBusiness();
	//g_mutex.unlock();
	return *m_pObj;
}

void SvrSettlementBusiness::deleteObj()
{
	//g_mutex.lock();
	if(NULL != m_pObj)
	{
		delete m_pObj;
		m_pObj = NULL;
	}	

	//g_mutex.unlock();
}
//CF_ERROR SvrSettlementBusiness::StratSettlement(void)
//{
//	
//	time_t cursecs=time(NULL);
//	SYSTEMTIME lp;
//	GetLocalTime(&lp); 
//	int miniTime = lp.wHour*60+lp.wMinute;
//	if(IsInTimePeriod(miniTime,time2Mins(OPENTIME),time2Mins(CLOSETIME)))
//		return CF_ERROR_NOT_SETTLEMENTTIME;
//	
//	else
//	/*m_pSettlementCalc->FundCalc();*/
//	
//	return SettlementCalc();
//
//}

//��ȡ����ģ��״̬
eSettlementStatus SvrSettlementBusiness::GetSettlementStatus()
{
	eSettlementStatus leRet = No_Settlementing;
	InterlockedExchange((volatile long*)(&leRet),m_SettlementStatus);
	return leRet;
}
//���ý���ģ��״̬
void SvrSettlementBusiness::SetSettlementStatus(eSettlementStatus eStatus) 
{
	InterlockedExchange(reinterpret_cast<volatile long*>(&m_SettlementStatus),eStatus);
}
CF_ERROR SvrSettlementBusiness::SettlementCalc(void)
{
	//int nErrorCode = loadData();


	if ( PositionDetailCalc() && PositionCalc() && FundCalc() )
		return CF_ERROR_SUCCESS;

	/*PositionDetailCalc();
	PositionCalc();	
	FundCalc();

	return nErrorCode;*/
	return true;
}
bool SvrSettlementBusiness::getSettlementPrice( const string instrumentId, double& price )
{
	std::map<std::string,PlatformStru_DepthMarketData>::iterator it = m_mapSettlementPrice.find(instrumentId);
	if(it == m_mapSettlementPrice.end())
		return false;

	price = it->second.SettlementPrice;
	return true;
}
bool SvrSettlementBusiness::GetMarginRate( const string userId, const string instrumentId, CommissionMarginRate& rate )
{
	if(m_mapCommissionMarginRate.empty())
		return false ;
	TradeAccount tradeAccount;
	CInterface_SvrUserOrg::getObj().GetTradeAccountByUserID(atoi(userId.c_str()), tradeAccount);
	char strAccount[13];
	sprintf(strAccount,"%d",tradeAccount.nTradeAccountID);
	char strBrokerID[11];
	sprintf(strBrokerID,"%d",tradeAccount.nBrokerID);
	RateKey key(strAccount,instrumentId.c_str(),strBrokerID);
	std::map<RateKey,CommissionMarginRate>::iterator it = m_mapCommissionMarginRate.find(key);
	if ( it != m_mapCommissionMarginRate.end() )
	{		
		rate = it->second;

	}
	return true;

}
CF_ERROR SvrSettlementBusiness::PositionDetailCalc(void)
{
	std::map<std::string,std::vector<PlatformStru_PositionDetail>>::iterator ituser=m_mapPositionDetail.begin();
	for (ituser; ituser!=m_mapPositionDetail.end(); ituser++)
	{
		std::vector<PlatformStru_PositionDetail>::iterator it = ituser->second.begin();
		for (it; it != ituser->second.end(); it++)
		{

			//����۸�ֵ
			if( !getSettlementPrice( it->InstrumentID, it->SettlementPrice ))
				return CF_ERROR_NO_SETTLEMENTPRICE;

			PlatformStru_InstrumentInfo InstrumentInfo;
			if( !CInterface_SvrTradeData::getObj().GetInstrumentInfo( it->InstrumentID, InstrumentInfo))
				return CF_ERROR_NO_INSTRUMENTS;

			//�������ն��гֲ�ӯ��
			bool bTodayPositionDetail = it->bToday;

			if(bTodayPositionDetail)
			{
				if(it->Direction==THOST_FTDC_D_Buy)
					it->PositionProfitByDate=(it->SettlementPrice - it->OpenPrice)*it->Volume*InstrumentInfo.VolumeMultiple;
				else
					it->PositionProfitByDate=-1*(it->SettlementPrice-it->OpenPrice)*it->Volume*InstrumentInfo.VolumeMultiple;
			}
			else 
			{
				if(it->Direction==THOST_FTDC_D_Buy)
					it->PositionProfitByDate=(it->SettlementPrice-it->LastSettlementPrice)*it->Volume*InstrumentInfo.VolumeMultiple;   //���ն��гֲ�ӯ��
				else
					it->PositionProfitByDate=-1*(it->SettlementPrice-it->LastSettlementPrice)*it->Volume*InstrumentInfo.VolumeMultiple;   //���ն��гֲ�ӯ��
			}
			if(it->PositionProfitByDate==-0) it->PositionProfitByDate=0;


			//���㴥�ʶԳ�ֲ�ӯ��(����ӯ��)
			if(it->Direction==THOST_FTDC_D_Buy)
				it->PositionProfitByTrade=(it->SettlementPrice-it->OpenPrice)*it->Volume*InstrumentInfo.VolumeMultiple;
			else
				it->PositionProfitByTrade=-1*(it->SettlementPrice-it->OpenPrice)*it->Volume*InstrumentInfo.VolumeMultiple;
			if(it->PositionProfitByTrade==-0) it->PositionProfitByTrade=0;

			//���㱣֤��

			CommissionMarginRate MarginRate;
			if( !GetMarginRate(ituser->first,it->InstrumentID, MarginRate))
				return CF_ERROR_NO_MARGIN;


			//�ж���ϳֲ���ϸ��֤���Ż����---��һ�ڲ�����

			double dPrice=it->SettlementPrice;

			if(it->Direction==THOST_FTDC_D_Buy)
			{
				it->Margin=  it->Volume*
					InstrumentInfo.VolumeMultiple*
					dPrice*
					MarginRate.LongMarginRatioByMoney+
					it->Volume*
					MarginRate.LongMarginRatioByVolume;
			}
			else
			{
				it->Margin=  it->Volume*
					InstrumentInfo.VolumeMultiple*
					dPrice*
					MarginRate.ShortMarginRatioByMoney+
					it->Volume*
					MarginRate.ShortMarginRatioByVolume;

			}





		}
	}

	return CF_ERROR_SUCCESS;

}
CF_ERROR SvrSettlementBusiness::PositionCalc(void)
{
	std::map<std::string,std::vector<PlatformStru_Position>>::iterator ituser=m_mapPosition.begin();
	for (ituser; ituser!=m_mapPosition.end(); ituser++)
	{
		std::vector<PlatformStru_Position>::iterator it = ituser->second.begin();
		for (it; it != ituser->second.end(); it++)
		{

			//����۸�ֵ
			if( !getSettlementPrice( it->InstrumentID, it->SettlementPrice ))
				return CF_ERROR_NO_SETTLEMENTPRICE;

			PlatformStru_InstrumentInfo InstrumentInfo;
			CInterface_SvrTradeData::getObj().GetInstrumentInfo( it->InstrumentID, InstrumentInfo);

			//����ֲ�ӯ��������ӯ����������ӯ��
			double CurCost;
			CurCost=it->SettlementPrice*it->Position*InstrumentInfo.VolumeMultiple;

			if(it->PosiDirection==THOST_FTDC_PD_Long)
			{
				it->PositionProfit=CurCost-it->PositionCost;
				it->PositionProfitByTrade=CurCost-it->OpenCost;
			}
			else 
			{
				it->PositionProfit=it->PositionCost-CurCost;
				it->PositionProfitByTrade=it->OpenCost-CurCost;
			}
			it->TotalPositionProfitByDate=it->PositionProfit+it->CloseProfitByDate;



			//����ָ����Լ�ı�֤��
			CommissionMarginRate MarginRate;
			if( !GetMarginRate(ituser->first,it->InstrumentID, MarginRate))
				return CF_ERROR_NO_MARGIN;

			double dPrice=it->SettlementPrice;

			if(it->PosiDirection==THOST_FTDC_PD_Long)
			{
				it->UseMargin=  it->Position*
					InstrumentInfo.VolumeMultiple*
					dPrice*
					MarginRate.LongMarginRatioByMoney+
					it->Position*
					MarginRate.LongMarginRatioByVolume;
			}
			else
			{
				it->UseMargin=  it->Position*
					InstrumentInfo.VolumeMultiple*
					dPrice*
					MarginRate.ShortMarginRatioByMoney+
					it->Position*
					MarginRate.ShortMarginRatioByVolume;

			}
		}
	}
	return CF_ERROR_SUCCESS;
}

CF_ERROR SvrSettlementBusiness::FundCalc(void)
{

	std::map<std::string,PlatformStru_TradingAccountInfo>::iterator ituser=m_mapFundData.begin();
	for (ituser; ituser!=m_mapFundData.end(); ituser++)
	{


		std::map<std::string,std::vector<PlatformStru_Position>>::iterator itPosition=m_mapPosition.find(ituser->first);
		if(itPosition != m_mapPosition.end())
		{
			std::vector<PlatformStru_Position>::iterator it = itPosition->second.begin();
			for (it; it != itPosition->second.end(); it++)
			{
				// ����ֲ�ӯ��
				if (it->PositionProfit!=InvalideValue)
				{
					ituser->second.PositionProfit += it->PositionProfit;
				}
				// ����ռ�ñ�֤�𣨲�������ϳֲ�)
				if(it->UseMargin != InvalideValue)
				{
					ituser->second.CurrMargin += it->UseMargin;
				}

			}


		}
		else
			return CF_ERROR_COMMON_MEMEORY;

		// ������Ȩ��=��̬ӯ��=��ĩ���=�ڶ����ڳ����
		ituser->second.DynamicProfit =  ituser->second.StaticProfit
			+ ituser->second.CloseProfit 
			+ ituser->second.PositionProfit
			- ituser->second.Commission
			-ituser->second.Withdraw 
			+ ituser->second.Deposit;
		//�����ʽ�=��ĩ��� - ��֤��ռ��
		ituser->second.Available = ituser->second.DynamicProfit - ituser->second.CurrMargin;

		//������

	}
	return CF_ERROR_SUCCESS;
}

////��ȡ�������Ľ�����
//string SvrSettlementBusiness::GetTradingDay(void)
//{
//	string ret;
//
//	ExchangeTime exchangeTime;
//	CInterface_SvrTradeData::getObj().getExchangeTime(exchangeTime))
//	if(exchangeTime.CurTickCount>0)
//		ret=string(exchangeTime.TradingDay);
//	else ret=string("");
//
//	return ret;
//}
//�ж�һ���ַ��������Ƿ��ǵ���
bool SvrSettlementBusiness::IsToday(const string& OpenDate)
{
	time_t lTime;
	struct tm * lTimeinfo; 
	time(&lTime);
	lTimeinfo = localtime(&lTime);
	_SettlementDate lcBuf = {0};
	sprintf_s(lcBuf,"%04d-%02d-%02d",1900+lTimeinfo->tm_year,1+lTimeinfo->tm_mon,lTimeinfo->tm_mday);

	string Tradingday=lcBuf;

	return (Tradingday==OpenDate);
}
CF_ERROR SvrSettlementBusiness::SetSettltmentTime(_SettlementDate * indata, int Flag)
{
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];	
	memset(szBuffer, 0, sizeof(szBuffer));

	sprintf(szBuffer, "delete from SETTLEMENT_AUTOTIME");
	CInterface_SvrDBOpr::getObj().Excute( szBuffer,nErrorCode );

	if (!nErrorCode && Flag == 1)
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer,"insert into SETTLEMENT_AUTOTIME values(\'%s\',sysdate)",indata);
	}

	CInterface_SvrDBOpr::getObj().Excute( szBuffer,nErrorCode );

	return  nErrorCode;


}

bool SvrSettlementBusiness::LoadSettltTime(string& szSettleTime)
{
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[100] = {0};	
	strcpy_s(szBuffer, "select * from SETTLEMENT_AUTOTIME t");
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return false;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			std::vector<_variant_t> vValue = vNode[j];
			int i = 0;
			szSettleTime=vValue[i++].operator _bstr_t();			


		}
		return true;
	}
}
CF_ERROR SvrSettlementBusiness::loadTradeData()
{
	m_mapTradeData.clear();
	int nErrorCode=CInterface_SvrTradeData::getObj().GetAllTradeInfo(m_mapTradeData);
	return  nErrorCode;
}

CF_ERROR SvrSettlementBusiness::QueryTradingday(std::string &pData)
{
	int nErrorCode= CF_ERROR_SUCCESS;
	CInterface_SvrTradeData::getObj().GetCurrentTradingDay(pData);
	if (pData == "")
	{
		nErrorCode=CF_ERROR_TRADINGDAY_ERROR;
		return nErrorCode;
	}
	return nErrorCode;
}
CF_ERROR SvrSettlementBusiness::GetCTPLoadStatus(std::string &pData)
{
	vector<UserInfo> lVecUser;
	CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUser);
	vector<UserInfo>::iterator lIter = lVecUser.begin();
	for(;lIter != lVecUser.end();lIter++)
	{
		if(lIter->nUserType == USER_TYPE_TRADER)
		{
			char buf[64];
			memset(buf, 0,sizeof(buf));
			if(CInterface_SvrTradeData::getObj().GetUserInitStatus(lIter->nUserID) & USER_STATUS_InitQryTradesReady)
			{

				sprintf( buf,"�û� %s ��ѯ�ɽ��ɹ�", lIter->szAccount);			
			}
			else
			{
				//���κ�һ��״̬ͣ��20s���ϣ�����Ϊ����
				double ldifftime = 0.0f;
				EnumAccountStatus lAcStatus = CInterface_SvrTradeData::getObj().GetUserCTPStatus(lIter->nUserID,ldifftime);
				//����3��״̬��ͣ��20s��ʱ�䣬����Ϊ�ǹ̶�״̬
				if(lAcStatus== ACCOUNT_STATUS_UnInit )
				{
					continue;
				}
					std::string szStatus = "δ֪";
					switch(lAcStatus)
					{

					case ACCOUNT_STATUS_CannotConnect:
						szStatus = "���Ӳ���CTP";
						break;

					case ACCOUNT_STATUS_Disconnected:
						szStatus = "��CTP�Ͽ�����";
						break;

					case ACCOUNT_STATUS_LoginFailure:
						szStatus = "�û���¼CTP��¼ʧ��";
						break;
					case ACCOUNT_STATUS_Login:
						szStatus = "�û���¼CTP��¼�ɹ�";
						break;
					case ACCOUNT_STATUS_TradeSettlementInfoConfirming:
						szStatus = "CTP���ڽ��н��׽�����ȷ��";
						break;

					}   
					sprintf( buf,"�û� %s ��ѯ�ɽ�ʧ�ܣ��û���CTP����״̬��%s", lIter->szAccount,szStatus.c_str());	

			}
			pData += buf ;
			pData +="\r\n" ;
		}

	}
	return CF_ERROR_SUCCESS;
}
CF_ERROR SvrSettlementBusiness::loadData(eSettlementLoadOption nOption,const string& strDate,int nbHistorySettlement)
{
	int nErrorCode= CF_ERROR_SUCCESS;
	if(GetSettlementStatus() != No_Settlementing)
	{
		LOG_INFO("SvrSettlementBusiness::loadData �ֶ�����ʧ�ܣ����������ڽ���״̬ ");
		return CF_ERROR_MANULSETTLEMENT_FAILD;
	}

	if ( nOption == SettlementLoadOption_TradingDataWithCTPSett )
	{

		if (!CInterface_SvrBrokerInfo::getObj().GetSettleServerGroup())
		{
			return CF_ERROR_NOSERVICEGROUP_FAILD;
		}
		
	}
    std::string  nsCurrentTradingDay="";
	//�Ƿ��ǵ��ս���
	if (!nbHistorySettlement)
	{
		
		CInterface_SvrTradeData::getObj().GetCurrentTradingDay(nsCurrentTradingDay);
		
		
	}
	else
	{
        nsCurrentTradingDay =strDate ;
	}

    //���ý�����
    SetSettlementDay(nsCurrentTradingDay);

	//֪ͨ���ݹ���ģ�鿪ʼ����

	nErrorCode= CInterface_SvrTradeData::getObj().StartSettlement(nOption,nsCurrentTradingDay,nbHistorySettlement);

	if(nErrorCode == CF_ERROR_SUCCESS)
	{
		SetSettlementStatus(Manul_Settlementing);
		m_bModifiedTrade=false;
		Mapclear();
		LOG_INFO("SvrSettlementBusiness::loadData  ����%s�ս������� ",strDate.c_str());
	}

	return nErrorCode;

}
void SvrSettlementBusiness::Mapclear(void)
{
	/*m_mapSettlementPrice.clear();
	m_mapSettlePriceInternal.clear();*/
	m_mapTradeData.clear();
	m_mapTradeID2TradeInfo.clear();
	m_mapPositionDetail.clear();
	m_mapPosition.clear();
	m_mapFundData.clear();
	m_mapCommissionMarginRate.clear();
	m_vecModifideCommissionRate.clear();
	m_vecModifideMarginRate.clear();

}
//CF_ERROR SvrSettlementBusiness::FinishSettlement(void)
//{
//	
//    /*return m_pSettlementCalc->SaveData();*/
//	return SaveData();
//}
void SvrSettlementBusiness::AutoSettlement()
{
	LOG_INFO("SvrSettlementBusiness::AutoSettlement ��ʼ�Զ����� ");
	//�Զ�����ʧ���跢��Ϣ��ʾ����ͻ���
	
	MessageInfo MsgInfo;
	memset(&MsgInfo,0,sizeof(MessageInfo));
	safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),"�Զ�����ʧ����ʾ");

	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);		
	int nExpiredDate = sysTime.wYear*10000 + sysTime.wMonth*100 + sysTime.wDay;

	
	char lcBuf[64] = {0};
	sprintf_s(lcBuf,"%04d-%02d-%02d %2d:%2d:%2d",sysTime.wYear, sysTime.wMonth,sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	std::vector<UserInfo> vec;
	CInterface_SvrUserOrg::getObj().GetUserInfos(vec);
	set<int> setRealUsers;
	for(int i =0; i < (int)vec.size(); i++)
	{
		UserInfo& userInfo = vec[i];
		if(userInfo.nUserType != USER_TYPE_MANAGE)
			continue;
		setRealUsers.insert(userInfo.nUserID);
	}
	
	if(GetSettlementStatus()  != No_Settlementing  )
	{
		PUSH_LOG_ERROR(Logger,false,true,"�Զ�����ʧ�ܣ����������ڽ��������");
		LOG_INFO("SvrSettlementBusiness::AutoSettlement �Զ�����ʧ�ܣ����������ڽ�������� ");
		sprintf(MsgInfo.szContent," ������ʱ��%s�Զ�����ʧ��\r\n ʧ��ԭ�򣺷��������ڽ��������",lcBuf);
		CInterface_SvrMsg::getObj().SendMsg(1, setRealUsers, conAutoMsg, nExpiredDate, 
			MsgInfo.szTitle, MsgInfo.szContent);
		return;
	}


	//��Ϊ�Զ�����״̬
	SetSettlementStatus(Auto_Settlementing);

	//�ж��Ƿ��ǽ����� 
	CInterface_SvrTradeData::getObj().GetCurrentTradingDay(msTodayString);
	if ( msTodayString == "")
	{

		PUSH_LOG_ERROR(Logger,false,true,"�Զ�����ʧ�ܣ�û��ȡ�������գ��޷�����");
		LOG_INFO("SvrSettlementBusiness::AutoSettlement û��ȡ�������գ��޷����� ");
		SetSettlementStatus(No_Settlementing);
		return;
	}

	//�жϽ������Ƿ�Ϊ����
	if(!IsToday(msTodayString))
	{
		PUSH_LOG_ERROR(Logger,false,true,"�Զ�����ʧ�ܣ��������Ľ�����Ϊ%s �ǵ���",  msTodayString.c_str());
		LOG_INFO("SvrSettlementBusiness::AutoSettlement �Զ�����ʧ�ܣ��������Ľ�����Ϊ%s �ǵ���",  msTodayString.c_str());
		sprintf(MsgInfo.szContent," ������ʱ��%s�Զ�����ʧ��\r\n ʧ��ԭ�򣺷������Ľ����� %s �ǵ��� ", lcBuf, msTodayString.c_str());
		CInterface_SvrMsg::getObj().SendMsg(1, setRealUsers, conAutoMsg, nExpiredDate, 
			MsgInfo.szTitle, MsgInfo.szContent);
		SetSettlementStatus(No_Settlementing);
		return;
	}
	//eTradeDataStatus status = CInterface_SvrTradeData::getObj().GetTraderDataStatus();
	//if (status != TradeDataStatusTrading )
	//{
	//	LOG_INFO("SvrSettlementBusiness::AutoSettlement ���ݹ���ģ��Ϊ�ǽ���״̬���޷����� ");
	//	return;
	//}

	//�жϽ������Ƿ����

	if(CInterface_SvrTradeData::getObj().IsSettlemented(msTodayString))
	{
		PUSH_LOG_ERROR(Logger,false,true,"�Զ�����ʧ�ܣ������ѽ�����������Զ�����");

		LOG_INFO("SvrSettlementBusiness::AutoSettlement �����ѽ�����������Զ����� ");
		sprintf(MsgInfo.szContent," ������ʱ��%s�Զ�����ʧ��\r\n ʧ��ԭ�򣺽���%s �ѽ�����������Զ����� ",  lcBuf,msTodayString.c_str());
		CInterface_SvrMsg::getObj().SendMsg(1, setRealUsers, conAutoMsg, nExpiredDate, 
			MsgInfo.szTitle, MsgInfo.szContent);
		SetSettlementStatus(No_Settlementing);
		return;
	}
	eTradeDataStatus status = CInterface_SvrTradeData::getObj().GetTraderDataStatus();
	if (status != TradeDataStatusTrading )
	{
		PUSH_LOG_ERROR(Logger,false,true,"�Զ�����ʧ�ܣ����ݹ���ģ��Ϊ�ǽ���״̬");
		LOG_INFO("SvrSettlementBusiness::AutoSettlement ���ݹ���ģ��Ϊ�ǽ���״̬���Զ�����ʧ�� ");
		sprintf(MsgInfo.szContent," ������ʱ��%s�Զ�����ʧ��\r\n ʧ��ԭ�򣺷�����Ϊ�ǽ���״̬",lcBuf);
		CInterface_SvrMsg::getObj().SendMsg(1, setRealUsers, conAutoMsg, nExpiredDate, 
			MsgInfo.szTitle, MsgInfo.szContent);
		SetSettlementStatus(No_Settlementing);
		return;
	}
	//�ȿ������ȫ��ȫ
	std::vector<SettlementPriceField> outData;
	QuerySettlementPrice(outData,0);

	CInterface_SvrTradeData::getObj().GetAllSettlementPrice( m_mapSettlementPrice);
	if ( m_mapSettlementPrice.size() == 0 || m_mapSettlePriceInternal.size() != m_mapSettlementPrice.size() )
	{
		PUSH_LOG_ERROR(Logger,false,true,"�Զ�����ʧ�ܣ�����۲�ȫ");
		LOG_INFO("SvrSettlementBusiness::AutoSettlement ����۲�ȫ���޷��Զ����� ");
		sprintf(MsgInfo.szContent," ������ʱ��%s�Զ�����ʧ��\r\n ʧ��ԭ�򣺽���۲�ȫ���޷��Զ�����\r\n �����գ�%s",lcBuf,msTodayString.c_str());
		CInterface_SvrMsg::getObj().SendMsg(1, setRealUsers, conAutoMsg, nExpiredDate, 
			MsgInfo.szTitle, MsgInfo.szContent);
		SetSettlementStatus(No_Settlementing);
		return ;
	}

	//֪ͨ���ݹ���ģ�鿪ʼ����
	int nErrorCode = CInterface_SvrTradeData::getObj().StartSettlement(SettlementLoadOption_TradingDataWithTodayDB,msTodayString,false);
	if( nErrorCode)
	{
		PUSH_LOG_ERROR(Logger,false,true,"�Զ�����ʧ�ܣ�%s  ",FormatErrorCode(nErrorCode));
		LOG_INFO("SvrSettlementBusiness::AutoSettlement �޷��Զ����㣺 %s  ",FormatErrorCode(nErrorCode));
		sprintf(MsgInfo.szContent," ������ʱ��%s�Զ�����ʧ��\r\n ʧ��ԭ�� %s\r\n �����գ�%s", lcBuf, FormatErrorCode(nErrorCode),msTodayString.c_str());
		CInterface_SvrMsg::getObj().SendMsg(1, setRealUsers, conAutoMsg, nExpiredDate, 
			MsgInfo.szTitle, MsgInfo.szContent);
		SetSettlementStatus(No_Settlementing);
		return ;
	}

	// ����ڴ�
	Mapclear();

	//��ʼ��������	

	
	ConfirmSettlementPrice();


	nErrorCode = CInterface_SvrTradeData::getObj().GetAllTradeInfo(m_mapTradeData);

	//������ɽ�
	ConfirmTrade();


	nErrorCode = CInterface_SvrTradeData::getObj().GetAllFundInfo(m_mapFundData);
	if( nErrorCode)
	{
		PUSH_LOG_ERROR(Logger,false,true,"�Զ�����ʧ�ܣ�%s  ",FormatErrorCode(nErrorCode));
		LOG_INFO("SvrSettlementBusiness::AutoSettlement �޷��Զ�����: %s  ",FormatErrorCode(nErrorCode));
		sprintf(MsgInfo.szContent," ������ʱ��%s�Զ�����ʧ��\r\n ʧ��ԭ�� %s \r\n �����գ�%s ",  lcBuf,FormatErrorCode(nErrorCode),msTodayString.c_str());
		CInterface_SvrMsg::getObj().SendMsg(1, setRealUsers, conAutoMsg, nExpiredDate, 
			MsgInfo.szTitle, MsgInfo.szContent);
		SetSettlementStatus(No_Settlementing);
		return ;
	}
	//���
	SaveData();
    SetSettlementStatus(No_Settlementing);
	LOG_INFO("SvrSettlementBusiness::AutoSettlement msTodayString=%s���Զ�������� ",msTodayString.c_str());
	PUSH_LOG_ERROR(Logger,false,true,"%s���Զ��������",msTodayString.c_str());
}
CF_ERROR SvrSettlementBusiness::ImmediatelyAutoSettlement()
{
	LOG_INFO("SvrSettlementBusiness::ImmediatelyAutoSettlement ��ʼ�����Զ����� ");

	int nErrorCode = CF_ERROR_SUCCESS;
	if(GetSettlementStatus()  != No_Settlementing  )
	{
		LOG_INFO("SvrSettlementBusiness::ImmediatelyAutoSettlement �Զ�����ʧ�ܣ����������ڽ�������� ");
		return CF_ERROR_MANULSETTLEMENT_FAILD;
	}
	//��ȡ������
	std::string  nsCurrentTradingDay="";
	CInterface_SvrTradeData::getObj().GetCurrentTradingDay(nsCurrentTradingDay);

	//���ý�����
	SetSettlementDay(nsCurrentTradingDay);
	
	//�жϽ������Ƿ����
	if(CInterface_SvrTradeData::getObj().IsSettlemented(msTodayString))
	{
		LOG_INFO("SvrSettlementBusiness::ImmediatelyAutoSettlement �����ѽ�����������Զ����� ");
		return CF_ERROR_TRADEDATA_TODAYHAD;
	}
	eTradeDataStatus status = CInterface_SvrTradeData::getObj().GetTraderDataStatus();
	/*if (status != TradeDataStatusTrading )
	{
		LOG_INFO("SvrSettlementBusiness::ImmediatelyAutoSettlement ���ݹ���ģ��Ϊ�ǽ���״̬���Զ�����ʧ�� ");
		return CF_ERROR_TRADEDATASTATUSERROR_FAILD;
	}*/

	//֪ͨ���ݹ���ģ�鿪ʼ����
	CInterface_SvrTradeData::getObj().StartSettlement(SettlementLoadOption_TradingDataWithTodayDB,nsCurrentTradingDay,0);
	//��Ϊ�Զ�����״̬
	SetSettlementStatus(Auto_Settlementing);
	//�����
	std::vector<SettlementPriceField> outData;
	QuerySettlementPrice(outData,0);

	// ����ڴ�
	Mapclear();

	//��ʼ��������	
	ConfirmSettlementPrice();

    CInterface_SvrTradeData::getObj().GetAllTradeInfo(m_mapTradeData);

	//������ɽ�
	ConfirmTrade();

	CInterface_SvrTradeData::getObj().GetAllFundInfo(m_mapFundData);
	
	//���
	nErrorCode=SaveData();
	SetSettlementStatus(No_Settlementing);
	LOG_INFO("SvrSettlementBusiness::ImmediatelyAutoSettlement msTodayString=%s���Զ�������� ",msTodayString.c_str());
	return nErrorCode;
}
bool SvrSettlementBusiness::IsSettled()
{
	return m_bSettlement;
}

CF_ERROR SvrSettlementBusiness::LoadLastSettlementPrice(const std::string & strTime, std::map<std::string,SettlementPriceField>  & nInfo) 
{
	int nErrorCode = CF_ERROR_SUCCESS;

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from SETTLEMENT_SETTLEMENTPRICE t where to_char(t.VALIDATEDATE,'YYYY-MM-DD') = \'%s\'",strTime.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			SettlementPriceField lTempInfp;
			memset(&lTempInfp, 0, sizeof(lTempInfp));

			std::vector<_variant_t> vValue = vNode[j];

			int i = 0;
			strcpy(lTempInfp.InstrumentID, vValue[i++].operator _bstr_t());			
			strcpy(lTempInfp.ExchangeID, vValue[i++].operator _bstr_t());
			strcpy(lTempInfp.ProductID, vValue[i++].operator _bstr_t());			
			lTempInfp.SettlementPrice = vValue[i++].dblVal;
			lTempInfp.LastSettlementPrice = vValue[i++].dblVal;
			strcpy(lTempInfp.SettlementDate, vValue[i++].operator _bstr_t());

			nInfo.insert(std::make_pair(lTempInfp.InstrumentID, lTempInfp));
		}
	}



	return CF_ERROR_SUCCESS;

}
CF_ERROR SvrSettlementBusiness::QuerySettlementPrice( std::vector<SettlementPriceField>& outData,int nbHistorySettlement)
{
	outData.clear();
    m_mapSettlePriceInternal.clear();
	
	if (nbHistorySettlement)//��ʷ����
	{
		//��ȡ��Լ�б�
		std::set<std::string> InstrumentList;
		CInterface_SvrTradeData::getObj().GetInstrumentList(InstrumentList);
		std::set<std::string>::iterator itIns = InstrumentList.begin();

		while ( itIns != InstrumentList.end())
		{
			SettlementPriceField settle;
			memset(&settle,0,sizeof(settle));
			strncpy(settle.InstrumentID ,(*itIns).c_str(),sizeof(settle.InstrumentID)-1);
			PlatformStru_InstrumentInfo Ins;
			CInterface_SvrTradeData::getObj().GetInstrumentInfo(*itIns, Ins);
			strncpy(settle.ExchangeID , Ins.ExchangeID,sizeof(settle.ExchangeID)-1);
			strncpy(settle.ProductID , Ins.ProductID,sizeof(settle.ProductID)-1);
			settle.SettlementPrice =InvalideValue;
			m_mapSettlePriceInternal[settle.InstrumentID] = settle;

			itIns++;
		}
	}
	else //���ս���
	{

		std::vector<PlatformStru_InstrumentInfo>  vecIns;		
		CInterface_SvrTradeData::getObj().GetCurrentDayAlInstrumentInfo(vecIns);
		if (vecIns.size() == 0)
		{
			return CF_ERROR_NO_INSTRUMENTS;
		}
        std::vector<PlatformStru_InstrumentInfo> ::iterator itIns =vecIns.begin();
		while (itIns != vecIns.end())
		{
			SettlementPriceField settle;
			memset(&settle,0,sizeof(settle));
			strncpy(settle.InstrumentID ,itIns->InstrumentID,sizeof(settle.InstrumentID)-1);			
			strncpy(settle.ExchangeID , itIns->ExchangeID,sizeof(settle.ExchangeID)-1);
			strncpy(settle.ProductID , itIns->ProductID,sizeof(settle.ProductID)-1);
			settle.SettlementPrice =InvalideValue;
			m_mapSettlePriceInternal[settle.InstrumentID] = settle;

			itIns++;
		}

	}
	
	//m_mapSettlePriceInternal["CVX"].SettlementPrice= 1;

	//�������еõ������
	m_mapSettlementPrice.clear();
	if (nbHistorySettlement)
	{
		CInterface_SvrTradeData::getObj().GetAllSettlementPrice( m_mapSettlementPrice);
	}
	else
	{

		CInterface_SvrTradeData::getObj().GetCurrentDayAllSettlementPrice(m_mapSettlementPrice);
	}
	

	//��ȡ������
	std::map<std::string,SettlementPriceField>  LastSettlementPrice;
	std::string  nsCurrentTradingDay="";
	if (nbHistorySettlement)
	{
		nsCurrentTradingDay =msTodayString;
	}
	else
	{
		CInterface_SvrTradeData::getObj().GetCurrentTradingDay(nsCurrentTradingDay);
	}
    //û�л�ȡ�������ʱ�������ݿ����������
	if ( m_mapSettlementPrice.size() == 0 )  
	{			
		std::string  nsLastDay="";
		CInterface_SvrTradeData::getObj().GetLastTrading(nsCurrentTradingDay, nsLastDay);
		LoadLastSettlementPrice(nsLastDay, LastSettlementPrice);

	}

	if (nsCurrentTradingDay != "")
	{
		nsCurrentTradingDay.erase (nsCurrentTradingDay.begin()+4);
		nsCurrentTradingDay.erase (nsCurrentTradingDay.begin()+6);
	}

	std::map<std::string,SettlementPriceField>::iterator itPrice = m_mapSettlePriceInternal.begin();
	while (itPrice != m_mapSettlePriceInternal.end())
	{
		std::map<std::string,PlatformStru_DepthMarketData>::iterator it = m_mapSettlementPrice.find(itPrice->first);
		if (it != m_mapSettlementPrice.end())
		{
			//�������еõ�����۵Ļ�������ۣ������ۣ��������ڸ�ֵ
			itPrice->second.SettlementPrice = it->second.SettlementPrice;	
			itPrice->second.LastSettlementPrice = it->second.PreSettlementPrice;
			strncpy(itPrice->second.SettlementDate,it->second.TradingDay,sizeof(itPrice->second.SettlementDate)-1);
		}

		else //û�л�ȡ�������ʱ�������ݿ����������
		{
			std::map<std::string,SettlementPriceField>::iterator it2 =  LastSettlementPrice.find(itPrice->first);
			if (it2 != LastSettlementPrice.end())
			{
				itPrice->second.LastSettlementPrice = it2->second.SettlementPrice;
			}

			strncpy(itPrice->second.SettlementDate,nsCurrentTradingDay.c_str(),sizeof(itPrice->second.SettlementDate)-1);
		}



		outData.push_back(itPrice->second);
		itPrice++;
	}

	//g_mutex.unlock();

	return CF_ERROR_SUCCESS;
}
CF_ERROR SvrSettlementBusiness::ConfirmForCaculate(void)
{
	//      //ȷ�Ϸ��ʺ����سֲ֡��ֲ���ϸ���ʽ�����
	//int nErrorCode = loadData();
	//if( !nErrorCode )
	//	return nErrorCode;
	////���سɹ��������㿪ʼ
	////return SettlementCalc();
	return CF_ERROR_SUCCESS;

}

void SvrSettlementBusiness::SetSettlementDay(const string SetDate)
{
	msTodayString =  SetDate;
}

CF_ERROR SvrSettlementBusiness::SaveData(void)
{
	int nErrorCode = CF_ERROR_SUCCESS;

	//g_mutex.lock();

	//������

	std::vector<SettlementPriceField> vSettlementPrice;
	std::map<std::string,SettlementPriceField>::iterator itPrice = m_mapSettlePriceInternal.begin();
	while (itPrice != m_mapSettlePriceInternal.end())
	{
		vSettlementPrice.push_back(itPrice->second);
		itPrice++;
	}
	CInterface_SvrDBOpr::getObj().SaveSettlementPrice2DB(msTodayString, vSettlementPrice , nErrorCode);
	if (nErrorCode != CF_ERROR_SUCCESS)
		return nErrorCode;


	//�����
	std::vector<PlatformStru_InstrumentCommissionRate> vecCommissionRate;
	if(!CInterface_SvrTradeData::getObj().GetAllCommissionRate( vecCommissionRate) && vecCommissionRate.size() != 0 )
	{
		CInterface_SvrDBOpr::getObj().SaveAccountCommissionRate("SETTLEMENT_COMMISSION",msTodayString,"*","*",vecCommissionRate,nErrorCode);
		if (nErrorCode != CF_ERROR_SUCCESS)
			return nErrorCode;

	}

	std::vector<PlatformStru_InstrumentMarginRate> vecMarginRate;
	if(!CInterface_SvrTradeData::getObj().GetAllMarginRate( vecMarginRate) && vecMarginRate.size() != 0 )
	{
		CInterface_SvrDBOpr::getObj().SaveAccountMarginRate("SETTLEMENT_MARGINRATE",msTodayString,"*","*",vecMarginRate,nErrorCode);
		if (nErrorCode != CF_ERROR_SUCCESS)
			return nErrorCode;

	}


	//��ɽ�
	m_mapTradeData.clear();
	CInterface_SvrTradeData::getObj().GetAllTradeInfo(m_mapTradeData);
	std::map<std::string,std::vector<PlatformStru_TradeInfo>>::iterator lIter = m_mapTradeData.begin();
	while(lIter != m_mapTradeData.end())
	{
		if( lIter->second.size() != 0)
		{
			CInterface_SvrDBOpr::getObj().SaveUserTraderInfos("SETTLEMENT_TRADE",true,msTodayString,lIter->first,lIter->second,nErrorCode);
			if (nErrorCode != CF_ERROR_SUCCESS)
				return nErrorCode;
		}
		lIter++;
	}

	//���سֲ�

		std::vector<PlatformStru_Position>  PositionData;
		QueryPosition(PositionData);

	
	//��ֲ�
	std::map<std::string,std::vector<PlatformStru_Position>>::iterator lIter2 = m_mapPosition.begin();
	while(lIter2 != m_mapPosition.end())
	{
		//if( lIter2->second.size() != 0)
		CInterface_SvrDBOpr::getObj().SaveUserPositionInfos("SETTLEMENT_POSITION",true,msTodayString,lIter2->first,lIter2->second,nErrorCode);
		if (nErrorCode != CF_ERROR_SUCCESS)
			return nErrorCode;
		if (lIter2->second.size() != 0 )			
		{
			std::vector<PlatformStru_Position>::iterator it=lIter2->second.begin();
			while (it!=lIter2->second.end())
			{
				(*it).YdPosition = (*it).Position;///���ճֲ�(ָ��ʷ�ֲ��ܺͣ���������ƽ���Ĳ���)
				(*it).TodayPosition = 0;///���ճֲ�(ָ���վ��ֲ�)
				(*it).OpenVolume =0;
				(*it).CloseVolume =0;
				(*it).OpenAmount =0;
				(*it).CloseAmount =0;

				(*it).LongFrozen =0;
				(*it).ShortFrozen =0;
				(*it).LongFrozenAmount =0;
				(*it).ShortFrozenAmount =0;
				(*it).FrozenMargin =0;
				(*it).FrozenCommission =0;
				(*it).FrozenCash =0;

				(*it).PreMargin = (*it).UseMargin;
				(*it).PositionProfit = 0;
				(*it).CloseProfitByDate = 0;
				(*it).CloseProfit = 0;
				(*it).CloseProfitByTrade = 0;
				(*it).Commission =0;
				(*it).TotalPositionProfitByDate =0;
				(*it).PreSettlementPrice = (*it).SettlementPrice;
				(*it).SettlementPrice =InvalideValue;
				it++;
			}		
		}
		CInterface_SvrDBOpr::getObj().SaveUserPositionInfos("SETTLEMENT_POSITIONINIT",true,msTodayString,lIter2->first,lIter2->second,nErrorCode);
		if (nErrorCode != CF_ERROR_SUCCESS)
			return nErrorCode;
		lIter2++;
	}

	//���سֲ���ϸ
	
		std::vector<PlatformStru_PositionDetail> PositionDetailData;
	    QueryPositionDetail(PositionDetailData);

	
	//��ֲ���ϸ
	std::map<std::string,std::vector<PlatformStru_PositionDetail>>::iterator lIter3 = m_mapPositionDetail.begin();
	while(lIter3 != m_mapPositionDetail.end())
	{
		//if( lIter3->second.size() != 0)
		CInterface_SvrDBOpr::getObj().SaveUserPositionDetailInfos("SETTLEMENT_POSITIONDETAIL",true,msTodayString,lIter3->first,lIter3->second,nErrorCode);
		if (nErrorCode != CF_ERROR_SUCCESS)
			return nErrorCode;
		if (lIter3->second.size() != 0 )
		{

			std::vector<PlatformStru_PositionDetail>::iterator it=lIter3->second.begin();
			while (it!=lIter3->second.end())
			{
				(*it).CloseVolume =0;
				(*it).CloseAmount =0;
				(*it).CloseProfitByTrade =0;
				(*it).PositionProfitByDate = 0;
				(*it).CloseProfitByDate = 0;
				(*it).LastSettlementPrice = (*it).SettlementPrice;
				(*it).SettlementPrice =InvalideValue;

				it++;
			}			
		}
		CInterface_SvrDBOpr::getObj().SaveUserPositionDetailInfos("SETTLEMENT_POSITIONDETAILINIT",true,msTodayString,lIter3->first,lIter3->second,nErrorCode);
		if (nErrorCode != CF_ERROR_SUCCESS)
			return nErrorCode;
		lIter3++;
	}

	//���ʽ�
	std::map<std::string,PlatformStru_TradingAccountInfo>::iterator lIter4 = m_mapFundData.begin();
	while(lIter4 != m_mapFundData.end())
	{

		CInterface_SvrDBOpr::getObj().SaveUserFundInfos("SETTLEMENT_FUNDINFO",msTodayString,lIter4->first,lIter4->second,nErrorCode);
		if (nErrorCode != CF_ERROR_SUCCESS)
			return nErrorCode;
		if (nErrorCode ==CF_ERROR_SUCCESS )
		{

			lIter4->second.Deposit = 0;
			lIter4->second.Withdraw = 0;
			lIter4->second.PositionProfit = 0;
			lIter4->second.CloseProfit = 0;
			lIter4->second.Commission = 0;
			lIter4->second.PreBalance = lIter4->second.DynamicProfit;
			CInterface_SvrDBOpr::getObj().SaveUserFundInfos("SETTLEMENT_FUNDINIT",msTodayString,lIter4->first,lIter4->second,nErrorCode);
			if (nErrorCode != CF_ERROR_SUCCESS)
				return nErrorCode;

		}
		lIter4++;
	}

	nErrorCode = CInterface_SvrTradeData::getObj().EndSettlement(msTodayString);
	if (nErrorCode == CF_ERROR_SUCCESS)
	{
		m_bSettlement = true;
		
		
	}

	LOG_INFO("SvrSettlementBusiness::SaveData msTodayString  %s�ս������ ",msTodayString.c_str());
	//g_mutex.unlock();
	return nErrorCode;
}
CF_ERROR SvrSettlementBusiness::SaveAction2DB(SettlementOperation& inData)
{
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH] = {0};

	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer,"insert into SETTLEMENT_OPRATIONRECORD values(%d,%d,'%s','%s',%d,%d,'%s',%f,%f,sysdate)",inData.nUserID,\
		inData.nUserID,inData.szTradingDay,inData.szInstrumentID,inData.nBusinessType,inData.nActionType,inData.szOperationItem,\
		inData.dOriginalContent,inData.dNewContent);

	CInterface_SvrDBOpr::getObj().Excute( szBuffer,nErrorCode );
	return nErrorCode;
}
CF_ERROR SvrSettlementBusiness::ModifySettlementPrice(const SettlementPriceField& indata, int userId)
{

	int nErrorCode = CF_ERROR_COMMON_MEMEORY;


	std::map<std::string,SettlementPriceField>::iterator it=  m_mapSettlePriceInternal.find(indata.InstrumentID);

	if ( it != m_mapSettlePriceInternal.end() )
	{
		//�޸Ľ���۶������
		SettlementOperation  operate;
		memset(&operate,0,sizeof(operate));
		operate.nUserID = userId;
		strncpy(operate.szTradingDay,msTodayString.c_str(),sizeof(operate.szTradingDay)-1);
		operate.nBusinessType = Business_SettlementPrice;
		operate.nActionType = Action_ModifyRecord;
		strncpy(operate.szInstrumentID , indata.InstrumentID,sizeof(operate.szInstrumentID)-1);
		strncpy(operate.szOperationItem , FIELD_SETTLEMENT,sizeof(operate.szOperationItem)-1);
		if(it->second.SettlementPrice == InvalideValue)
		{
			operate.dOriginalContent =-1;
		}			
		else
		{
            operate.dOriginalContent =it->second.SettlementPrice;
		}
		
		operate.dNewContent = indata.SettlementPrice;
        SaveAction2DB(operate);

		//�޸�map�н����
		it->second.SettlementPrice = indata.SettlementPrice;
        nErrorCode = CF_ERROR_SUCCESS;

	}


	return nErrorCode;
}
CF_ERROR SvrSettlementBusiness::ConfirmSettlementPrice()
{
	int nErrorCode = CF_ERROR_SUCCESS;
	//g_mutex.lock();

	std::map<std::string,SettlementPriceField>::iterator it=  m_mapSettlePriceInternal.begin();
	while (it!=m_mapSettlePriceInternal.end())
	{
		//֪ͨ���ݹ���ģ���޸Ľ����
		PlatformStru_DepthMarketData quot;
		memset( &quot,0,sizeof(PlatformStru_DepthMarketData));
		std::map<std::string,PlatformStru_DepthMarketData>::iterator itQuot=  m_mapSettlementPrice.find(it->first);
		if (itQuot != m_mapSettlementPrice.end())
		{
			quot =itQuot->second;
		}
		strncpy(quot.InstrumentID , it->first.c_str() ,sizeof(quot.InstrumentID)-1);
		quot.LastPrice = it->second.SettlementPrice;
		quot.SettlementPrice =it->second.SettlementPrice;
		quot.PreSettlementPrice = it->second.LastSettlementPrice;
		quot.Volume = 1;

		nErrorCode = CInterface_SvrTradeData::getObj().SetQuotInfo(quot,true);
		if (nErrorCode)
			break;
		it++;
	}

	//g_mutex.unlock();
	return nErrorCode;
}

CF_ERROR SvrSettlementBusiness::QueryCommissionRate( std::map<RateKey,CommissionMarginRate>& outData)
{
	outData.clear();
	//int nErrorCode = CF_ERROR_SUCCESS;

	//��ǰ���سɽ�����
	 GetTrade();
	
	 //��ȡ��Լ�б�
	 std::set<std::string> InstrumentList;
	 CInterface_SvrTradeData::getObj().GetInstrumentList(InstrumentList);

	//��ȡ����ί�н����˺�
	vector<TradeAccount> lVecAccount;
	std::set<BrokerAccountKey> lvecUser;
	CInterface_SvrBrokerInfo::getObj().GetTradeAccounts(lVecAccount);
	vector<TradeAccount>::iterator lIter = lVecAccount.begin();
	for(;lIter != lVecAccount.end();lIter++)
	{
		BrokerInfo lBroker;
		CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lIter->nBrokerID,lBroker);
		BrokerAccountKey lKey(lBroker.szBrokerCode,lIter->szTradeAccount);	
		lvecUser.insert(lKey);
	}
    //�õ�һ���յķ���MAP
	m_mapCommissionMarginRate.clear();
	std::set<BrokerAccountKey>::iterator liteUser = lvecUser.begin();
	for(;liteUser != lvecUser.end(); liteUser++)
	{
		std::set<std::string>::iterator itIns = InstrumentList.begin();
		while ( itIns != InstrumentList.end())
		{
			RateKey key((*liteUser).AccountID,(*itIns).c_str(),(*liteUser).BrokerID);
			CommissionMarginRate field;
			//memset(&field,0,sizeof(field));
			strncpy(field.AccountID,(*liteUser).AccountID,sizeof(field.AccountID)-1);
			strncpy(field.InstrumentID,(*itIns).c_str(),sizeof(field.InstrumentID)-1);
			strncpy(field.BrokerID,(*liteUser).BrokerID,sizeof(field.BrokerID)-1);
			m_mapCommissionMarginRate[key] =field;
			itIns++;
		}

	}


	std::vector<PlatformStru_InstrumentCommissionRate> vecCommissionRate;
	CInterface_SvrTradeData::getObj().GetAllCommissionRate( vecCommissionRate);
	std::vector<PlatformStru_InstrumentCommissionRate>::iterator it = vecCommissionRate.begin();
	for( it; it != vecCommissionRate.end(); it++ )
	{
		RateKey key((*it).InvestorID,(*it).InstrumentID,(*it).BrokerID);
		std::map<RateKey,CommissionMarginRate>::iterator it3 = m_mapCommissionMarginRate.find(key);
		if (it3 != m_mapCommissionMarginRate.end())
		{
			it3->second.CloseRatioByMoney = (*it).CloseRatioByMoney;
			it3->second.CloseRatioByVolume = (*it).CloseRatioByVolume;
			it3->second.CloseTodayRatioByMoney = (*it).CloseTodayRatioByMoney;
			it3->second.CloseTodayRatioByVolume = (*it).CloseTodayRatioByVolume;
			it3->second.OpenRatioByMoney = (*it).OpenRatioByMoney;
			it3->second.OpenRatioByVolume = (*it).OpenRatioByVolume;
		}
	}
	std::vector<PlatformStru_InstrumentMarginRate> vecMarginRate;	
	CInterface_SvrTradeData::getObj().GetAllMarginRate( vecMarginRate);

	std::vector<PlatformStru_InstrumentMarginRate>::iterator it2 = vecMarginRate.begin();
	for( it2; it2 != vecMarginRate.end(); it2++ )
	{
		RateKey key2((*it2).InvestorID,(*it2).InstrumentID,(*it2).BrokerID);
		std::map<RateKey,CommissionMarginRate>::iterator it3 = m_mapCommissionMarginRate.find(key2);
		if (it3 != m_mapCommissionMarginRate.end())
		{
			it3->second.LongMarginRatioByMoney = (*it2).LongMarginRatioByMoney;
			it3->second.LongMarginRatioByVolume = (*it2).LongMarginRatioByVolume;
			it3->second.ShortMarginRatioByMoney = (*it2).ShortMarginRatioByMoney;
			it3->second.ShortMarginRatioByVolume = (*it2).ShortMarginRatioByVolume;
		}
	}


	outData = m_mapCommissionMarginRate;

	//g_mutex.unlock();
	return CF_ERROR_SUCCESS;
}
CF_ERROR SvrSettlementBusiness::LoadHistoryRate(const string& strDate,RateKey& key,PlatformStru_InstrumentCommissionRate& Commission,PlatformStru_InstrumentMarginRate& MarginRate)
{
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from SETTLEMENT_COMMISSION t where t.ValidateDate = \'%s\'AND BrokerID = \'%s\' AND InvestorID = \'%s\' AND instrumentid =\'%s\'",
		strDate.c_str(),key.BrokerID,key.AccountID,key.InstrumentID);
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode)|| vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{

			std::vector<_variant_t> vValue = vNode[j];

			int i = 0;
			strcpy(Commission.InstrumentID, vValue[i++].operator _bstr_t());			
			Commission.InvestorRange = vValue[i++].cVal;
			strcpy(Commission.BrokerID, vValue[i++].operator _bstr_t());
			strcpy(Commission.InvestorID, vValue[i++].operator _bstr_t());					
			Commission.OpenRatioByMoney = vValue[i++].dblVal;
			Commission.OpenRatioByVolume = vValue[i++].dblVal;
			Commission.CloseRatioByMoney = vValue[i++].dblVal;
			Commission.CloseRatioByVolume = vValue[i++].dblVal;
			Commission.CloseTodayRatioByMoney = vValue[i++].dblVal;
			Commission.CloseTodayRatioByVolume = vValue[i++].dblVal;
		}
	}

	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from SETTLEMENT_MARGINRATE t where t.ValidateDate = \'%s\' AND BrokerID = \'%s\' AND InvestorID = \'%s\' AND instrumentid =\'%s\'",
		strDate.c_str(),key.BrokerID,key.AccountID,key.InstrumentID);
	std::vector<std::vector<_variant_t>> vNodeM;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNodeM, nErrorCode) || vNodeM.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}	
	else
	{
		for ( UINT j = 0; j < vNodeM.size(); j++ )
		{

			std::vector<_variant_t> vValue = vNodeM[j];

			int i = 0;
			strcpy(MarginRate.InstrumentID, vValue[i++].operator _bstr_t());			
			MarginRate.InvestorRange = vValue[i++].cVal;
			strcpy(MarginRate.BrokerID, vValue[i++].operator _bstr_t());
			strcpy(MarginRate.InvestorID, vValue[i++].operator _bstr_t());					
			MarginRate.HedgeFlag = vValue[i++].cVal;
			MarginRate.LongMarginRatioByMoney = vValue[i++].dblVal;
			MarginRate.LongMarginRatioByVolume = vValue[i++].dblVal;
			MarginRate.ShortMarginRatioByMoney = vValue[i++].dblVal;
			MarginRate.ShortMarginRatioByVolume = vValue[i++].dblVal;
			MarginRate.IsRelative = vValue[i++].intVal;
		}
	}
   return nErrorCode;
}
CF_ERROR SvrSettlementBusiness::QueryHistoryCommissionRate(const string& strDate,std::vector<RateKey>& vecRate,std::map<RateKey,CommissionMarginRate>& outData)
{
	outData.clear();
	//m_mapCommissionMarginRate.clear();
	std::map<RateKey,CommissionMarginRate> mapTempRate;
	int nErrorCode = CF_ERROR_SUCCESS;

	std::vector<PlatformStru_InstrumentCommissionRate> vecCommissionRate;
	std::vector<PlatformStru_InstrumentMarginRate> vecMarginRate;
	std::vector<RateKey>::iterator itRate= vecRate.begin();
	while ( itRate != vecRate.end())
	{
		PlatformStru_InstrumentCommissionRate Commission;
		memset(&Commission,0,sizeof(Commission));
		PlatformStru_InstrumentMarginRate MarginRate;
		memset(&MarginRate,0,sizeof(MarginRate));
		if( LoadHistoryRate(strDate,*itRate,Commission,MarginRate) == CF_ERROR_SUCCESS)
		{
			vecCommissionRate.push_back(Commission);
			vecMarginRate.push_back(MarginRate);
			m_vecModifideCommissionRate.push_back( Commission);
			m_vecModifideMarginRate.push_back( MarginRate);

		}
		/*else
		{
			nErrorCode= CF_ERROR_LOADHISTORYCOMMISSION_FAILD;
			break;
		}*/
		itRate++;
	}


	std::vector<PlatformStru_InstrumentMarginRate>::iterator it2 = vecMarginRate.begin();
	for( it2; it2 != vecMarginRate.end(); it2++ )
	{
		RateKey key2((*it2).InvestorID,(*it2).InstrumentID,(*it2).BrokerID);
		CommissionMarginRate field;
		strncpy(field.AccountID,(*it2).InvestorID,sizeof(field.AccountID)-1);
		strncpy(field.InstrumentID,(*it2).InstrumentID,sizeof(field.InstrumentID)-1);
		strncpy(field.BrokerID,(*it2).BrokerID,sizeof(field.BrokerID)-1);
		field.LongMarginRatioByMoney = (*it2).LongMarginRatioByMoney;
		field.LongMarginRatioByVolume = (*it2).LongMarginRatioByVolume;
		field.ShortMarginRatioByMoney = (*it2).ShortMarginRatioByMoney;
		field.ShortMarginRatioByVolume = (*it2).ShortMarginRatioByVolume;
        mapTempRate[key2] =field;

	}

	std::vector<PlatformStru_InstrumentCommissionRate>::iterator it = vecCommissionRate.begin();
	for( it; it != vecCommissionRate.end(); it++ )
	{
		RateKey key((*it).InvestorID,(*it).InstrumentID,(*it).BrokerID);
		std::map<RateKey,CommissionMarginRate>::iterator it3 = mapTempRate.find(key);
		if (it3 != mapTempRate.end())
		{
		it3->second.CloseRatioByMoney = (*it).CloseRatioByMoney;
		it3->second.CloseRatioByVolume = (*it).CloseRatioByVolume;
		it3->second.CloseTodayRatioByMoney = (*it).CloseTodayRatioByMoney;
		it3->second.CloseTodayRatioByVolume = (*it).CloseTodayRatioByVolume;
		it3->second.OpenRatioByMoney = (*it).OpenRatioByMoney;
		it3->second.OpenRatioByVolume = (*it).OpenRatioByVolume;
		}
	}
	

   if(mapTempRate.size() == 0)
   {
	   nErrorCode= CF_ERROR_LOADHISTORYCOMMISSION_FAILD;
   }
	outData = mapTempRate;


	return nErrorCode;

}
void SvrSettlementBusiness::ModifiedRateClear()
{
	m_vecModifideCommissionRate.clear();
	m_vecModifideMarginRate.clear();

}
CF_ERROR SvrSettlementBusiness::ModifyCommissionRate( CommissionMarginRate& outData,int userId)
{
	int nErrorCode = CF_ERROR_COMMON_MEMEORY;



	//�������ݹ���ģ���е��޸ķ��ʺ������������¼���ɽ��ֲ��ʽ�
	PlatformStru_InstrumentCommissionRate CommissionRate;
	memset(&CommissionRate,0,sizeof(CommissionRate));
	strncpy(CommissionRate.InvestorID,outData.AccountID,sizeof(CommissionRate.InvestorID)-1);
	strncpy(CommissionRate.InstrumentID,outData.InstrumentID,sizeof(CommissionRate.InstrumentID)-1);
	strncpy(CommissionRate.BrokerID,outData.BrokerID,sizeof(CommissionRate.BrokerID)-1);
	CommissionRate.CloseRatioByMoney = outData.CloseRatioByMoney;
	CommissionRate.CloseRatioByVolume = outData.CloseRatioByVolume;
	CommissionRate.CloseTodayRatioByMoney = outData.CloseTodayRatioByMoney;
	CommissionRate.CloseTodayRatioByVolume = outData.CloseTodayRatioByVolume;
	CommissionRate.OpenRatioByMoney = outData.OpenRatioByMoney;
	CommissionRate.OpenRatioByVolume = outData.OpenRatioByVolume;
	m_vecModifideCommissionRate.push_back( CommissionRate);

	PlatformStru_InstrumentMarginRate MarginRate;
	memset(&MarginRate,0,sizeof(MarginRate));
	strncpy(MarginRate.InvestorID,outData.AccountID,sizeof(MarginRate.InvestorID)-1);
	strncpy(MarginRate.InstrumentID,outData.InstrumentID,sizeof(MarginRate.InstrumentID)-1);
	strncpy(MarginRate.BrokerID,outData.BrokerID,sizeof(MarginRate.BrokerID)-1);
	MarginRate.LongMarginRatioByMoney = outData.LongMarginRatioByMoney;
	MarginRate.LongMarginRatioByVolume = outData.LongMarginRatioByVolume;
	MarginRate.ShortMarginRatioByMoney = outData.ShortMarginRatioByMoney;
	MarginRate.ShortMarginRatioByVolume = outData.ShortMarginRatioByVolume;
	m_vecModifideMarginRate.push_back( MarginRate);


	//�޸ķ��ʶ������
	//g_mutex.lock();
	RateKey key(outData.AccountID,outData.InstrumentID,outData.BrokerID);
	std::map<RateKey,CommissionMarginRate>::iterator it2 =m_mapCommissionMarginRate.find(key);
	if( it2 != m_mapCommissionMarginRate.end() )
	{

		SettlementOperation  operate;
		memset(&operate,0,sizeof(operate));
		operate.nUserID = userId;
		strncpy(operate.szTradingDay,msTodayString.c_str(),sizeof(operate.szTradingDay)-1);
        operate.nBusinessType = Business_CommissionRate;
		operate.nActionType = Action_ModifyRecord;
		strncpy( operate.szInstrumentID ,outData.InstrumentID,sizeof(operate.szInstrumentID)-1);

		if (it2->second.CloseRatioByMoney != outData.CloseRatioByMoney)
		{			
			strncpy(operate.szOperationItem , FIELD_CLOSERATIOBYMONEY,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it2->second.CloseRatioByMoney;
			operate.dNewContent = outData.CloseRatioByMoney;
            SaveAction2DB(operate);
			
		}

		if (it2->second.CloseRatioByVolume != outData.CloseRatioByVolume)
		{
		
			strncpy(operate.szOperationItem , FIELD_CLOSERATIOBYVOLUME,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it2->second.CloseRatioByVolume;
			operate.dNewContent = outData.CloseRatioByVolume;
            SaveAction2DB(operate);		
		}
		if (it2->second.OpenRatioByMoney != outData.OpenRatioByMoney)
		{
			
			strncpy(operate.szOperationItem , FIELD_OPENRATIOBYMONEY,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it2->second.OpenRatioByMoney;
			operate.dNewContent = outData.OpenRatioByMoney;
            SaveAction2DB(operate);		
		}
		if (it2->second.OpenRatioByVolume != outData.OpenRatioByVolume)
		{
			
			strncpy(operate.szOperationItem ,  FIELD_OPENRATIOBYVOLUME,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it2->second.OpenRatioByVolume;
			operate.dNewContent = outData.OpenRatioByVolume;
            SaveAction2DB(operate);
		}
		if (it2->second.CloseTodayRatioByMoney != outData.CloseTodayRatioByMoney)
		{
		
			strncpy(operate.szOperationItem ,  FIELD_CLOSETODAYRATIOBYMONEY,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it2->second.CloseTodayRatioByMoney;
			operate.dNewContent = outData.CloseTodayRatioByMoney;
            SaveAction2DB(operate);			
		}
		if (it2->second.CloseTodayRatioByVolume != outData.CloseTodayRatioByVolume)
		{
			
			strncpy(operate.szOperationItem ,  FIELD_CLOSETODAYRATIOBYVOLUME,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it2->second.CloseTodayRatioByVolume;
			operate.dNewContent = outData.CloseTodayRatioByVolume;
            SaveAction2DB(operate);			
		}

		//�޸ı�֤�������
		if (it2->second.LongMarginRatioByMoney != outData.LongMarginRatioByMoney)
		{
			
			operate.nBusinessType = Business_MarginRate;
			
			strncpy(operate.szOperationItem ,  FIELD_LONGMARGINRATIOBYMONEY,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it2->second.LongMarginRatioByMoney;
			operate.dNewContent = outData.LongMarginRatioByMoney;
			SaveAction2DB(operate);
		}

		if (it2->second.LongMarginRatioByVolume != outData.LongMarginRatioByVolume)
		{
			
			operate.nBusinessType = Business_MarginRate;
			
			strncpy(operate.szOperationItem ,  FIELD_LONGMARGINRATIOBYVOLUME,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it2->second.LongMarginRatioByVolume;
			operate.dNewContent = outData.LongMarginRatioByVolume;
            SaveAction2DB(operate);
		}
		if (it2->second.ShortMarginRatioByMoney != outData.ShortMarginRatioByMoney)
		{
			
			operate.nBusinessType = Business_MarginRate;
			
			strncpy(operate.szOperationItem ,  FIELD_SHORTMARGINRATIOBYMONEY,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it2->second.ShortMarginRatioByMoney;
			operate.dNewContent = outData.ShortMarginRatioByMoney;
            SaveAction2DB(operate);
			
		}
		if (it2->second.ShortMarginRatioByVolume != outData.ShortMarginRatioByVolume)
		{

			operate.nBusinessType = Business_MarginRate;

			strncpy(operate.szOperationItem ,  FIELD_SHORTMARGINRATIOBYVOLUME,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it2->second.ShortMarginRatioByVolume;
			operate.dNewContent = outData.ShortMarginRatioByVolume;
            SaveAction2DB(operate);
		}


		
		//�޸�map�з���
		m_mapCommissionMarginRate[key]=outData;
		nErrorCode = CF_ERROR_SUCCESS;


	}
	//g_mutex.unlock();

	return nErrorCode;

}
CF_ERROR SvrSettlementBusiness::SetModifiedRate()
{

	int nErrorCode = CInterface_SvrTradeData::getObj().SetModifiedCommissionAndMargin(m_vecModifideCommissionRate,m_vecModifideMarginRate);
	return nErrorCode;
}

//CF_ERROR SvrSettlementBusiness::QueryMarginRate( std::map<std::string,std::map<std::string,PlatformStru_InstrumentMarginRate>>& outData)
//{
//	outData.clear();
//	//g_mutex.lock();
//	//std::map<std::string,std::map<std::string,PlatformStru_InstrumentMarginRate>> m_mapMarginRate;
//	if(!CInterface_SvrTradeData::getObj().GetAllMarginRate( m_mapMarginRate) || m_mapMarginRate.size() == 0 )
//		return CF_ERROR_NO_MARGIN;
//
//	outData = m_mapMarginRate;
//
//	//g_mutex.unlock();
//	return CF_ERROR_SUCCESS;
//}
//CF_ERROR SvrSettlementBusiness::ModifyMarginRate( PlatformStru_InstrumentMarginRate& outData,int userId )
//{
//	//g_mutex.lock();
//	std::map<std::string,std::map<std::string,PlatformStru_InstrumentMarginRate>>::iterator it = m_mapMarginRate.find(outData.InvestorID);
//	if( it != m_mapMarginRate.end() )
//	{
//
//		std::map<std::string,PlatformStru_InstrumentMarginRate> lMapField;
//		memset(&lMapField,0,sizeof(lMapField));
//		lMapField = it->second;
//		std::map<std::string,PlatformStru_InstrumentMarginRate>::iterator lIter = lMapField.find(outData.InstrumentID);
//
//		if ( lIter != lMapField.end() )
//		{
//			//�޸ı�֤�������
//			if (lIter->second.LongMarginRatioByMoney != outData.LongMarginRatioByMoney)
//			{
//				SettlementOperation  operate;
//				memset(&operate,0,sizeof(operate));
//				operate.nUserID = userId;
//				operate.nBusinessType = Business_MarginRate;
//				operate.nActionType = Action_ModifyRecord;
//				strncpy(operate.szInstrumentID , outData.InstrumentID,sizeof(operate.szInstrumentID));
//				strncpy(operate.szOperationItem ,  FIELD_LONGMARGINRATIOBYMONEY,sizeof(operate.szOperationItem));
//				operate.dOriginalContent =lIter->second.LongMarginRatioByMoney;
//				operate.dNewContent = outData.LongMarginRatioByMoney;
//
//				SaveAction2DB(operate);
//			}
//
//			if (lIter->second.LongMarginRatioByVolume != outData.LongMarginRatioByVolume)
//			{
//				SettlementOperation  operate;
//				memset(&operate,0,sizeof(operate));
//				operate.nUserID = userId;
//				operate.nBusinessType = Business_MarginRate;
//				operate.nActionType = Action_ModifyRecord;
//				strncpy(operate.szInstrumentID , outData.InstrumentID,sizeof(operate.szInstrumentID));
//				strncpy(operate.szOperationItem ,  FIELD_LONGMARGINRATIOBYMONEY,sizeof(operate.szOperationItem));
//				operate.dOriginalContent =lIter->second.LongMarginRatioByVolume;
//				operate.dNewContent = outData.LongMarginRatioByVolume;
//
//				SaveAction2DB(operate);
//			}
//			if (lIter->second.ShortMarginRatioByMoney != outData.ShortMarginRatioByMoney)
//			{
//				SettlementOperation  operate;
//				memset(&operate,0,sizeof(operate));
//				operate.nUserID = userId;
//				operate.nBusinessType = Business_MarginRate;
//				operate.nActionType = Action_ModifyRecord;
//				strncpy(operate.szInstrumentID , outData.InstrumentID,sizeof(operate.szInstrumentID));
//				strncpy(operate.szOperationItem ,  FIELD_SHORTMARGINRATIOBYMONEY,sizeof(operate.szOperationItem));
//				operate.dOriginalContent =lIter->second.ShortMarginRatioByMoney;
//				operate.dNewContent = outData.ShortMarginRatioByMoney;
//
//				SaveAction2DB(operate);
//			}
//			if (lIter->second.ShortMarginRatioByVolume != outData.ShortMarginRatioByVolume)
//			{
//				SettlementOperation  operate;
//				memset(&operate,0,sizeof(operate));
//				operate.nUserID = userId;
//				operate.nBusinessType = Business_MarginRate;
//				operate.nActionType = Action_ModifyRecord;
//				strncpy(operate.szInstrumentID , outData.InstrumentID,sizeof(operate.szInstrumentID));
//				strncpy(operate.szOperationItem ,  FIELD_SHORTMARGINRATIOBYVOLUME,sizeof(operate.szOperationItem));
//				operate.dOriginalContent =lIter->second.ShortMarginRatioByVolume;
//				operate.dNewContent = outData.ShortMarginRatioByVolume;
//
//				SaveAction2DB(operate);
//			}
//
//			//�޸�map�б�֤��
//			lMapField[outData.InstrumentID] =outData;
//			m_mapMarginRate[outData.InvestorID]=lMapField;
//		}
//
//	}
//	//g_mutex.unlock();
//
//	return CF_ERROR_SUCCESS;
//}
void SvrSettlementBusiness::GetTrade()
{
	m_mapTradeData.clear();
	CInterface_SvrTradeData::getObj().GetAllTradeInfo(m_mapTradeData);

}
CF_ERROR SvrSettlementBusiness::QueryTrade(std::vector<PlatformStru_TradeInfo>& outData) 
{
	int nErrorCode = CF_ERROR_SUCCESS;
	outData.clear();
	////��ѯ�ɽ���Ϣ

	std::map<std::string,std::vector<PlatformStru_TradeInfo>>::iterator lIter = m_mapTradeData.begin();

	while(lIter != m_mapTradeData.end())
	{
		std::vector<PlatformStru_TradeInfo>::iterator it =lIter->second.begin();
		while (it !=lIter->second.end())
		{
			TradeKey key(*it);
			m_mapTradeID2TradeInfo[key]=*it;
			outData.push_back(*it);
			it++;
		}
		lIter++;
	}

	if ( outData.size() == 0)
		nErrorCode= CF_ERROR_NO_TRADEDATA;

	return nErrorCode;
}

CF_ERROR SvrSettlementBusiness::CheckCloseVolume(PlatformStru_TradeInfo&  indata) 
{

	int nErrorCode = CF_ERROR_ADDTRADE_FAILD;
	m_mapPosition.clear();
	CInterface_SvrTradeData::getObj().GetAllPosition(m_mapPosition);
	std::map<std::string,std::vector<PlatformStru_Position>>::iterator lIter = m_mapPosition.find(indata.InvestorID);
	if (lIter != m_mapPosition.end())
	{
		std::vector<PlatformStru_Position>::iterator itposition= lIter->second.begin();
		while (itposition != lIter->second.end())
		{
			if (strncmp(itposition->InstrumentID,indata.InstrumentID,sizeof(indata.InstrumentID)) == 0 &&
				itposition->HedgeFlag == indata.HedgeFlag &&
				( ( itposition->PosiDirection == THOST_FTDC_PD_Long && indata.Direction == THOST_FTDC_D_Sell) ||
				( itposition->PosiDirection == THOST_FTDC_PD_Short && indata.Direction == THOST_FTDC_D_Buy)))
			{
				switch(indata.OffsetFlag)
				{
				case THOST_FTDC_OF_Close:
				case THOST_FTDC_OF_ForceClose:
					{
						if (itposition->CanCloseVolume>= indata.Volume)
						{
							/*itposition->CanCloseVolume -= indata.Volume;
							if (itposition->CanCloseydVolume > indata.Volume)
							{
							itposition->CanCloseydVolume -= indata.Volume;
							}
							else
							{
							itposition->CanCloseTodayVolume -= indata.Volume - itposition->CanCloseydVolume;
							itposition->CanCloseydVolume =0;

							}*/
							nErrorCode = CF_ERROR_SUCCESS;
						}

					}

					break;
				case THOST_FTDC_OF_CloseToday:
					{
						if (itposition->CanCloseTodayVolume>= indata.Volume)
						{
							/*    itposition->CanCloseTodayVolume -= indata.Volume;
							itposition->CanCloseVolume -= indata.Volume;*/
							nErrorCode = CF_ERROR_SUCCESS;
						}

					}
					break;
				case THOST_FTDC_OF_CloseYesterday:
					{
						if (itposition->CanCloseydVolume>= indata.Volume)
						{
							/*itposition->CanCloseydVolume -= indata.Volume;
							itposition->CanCloseVolume -= indata.Volume;*/
							nErrorCode = CF_ERROR_SUCCESS;
						}

					}
					break;
				}
			}
			itposition++;
		}

	}
	m_mapPosition.clear();
	return nErrorCode;
}
CF_ERROR SvrSettlementBusiness::AddTrade(PlatformStru_TradeInfo&  indata,int userId) 
{
	int nErrorCode = CF_ERROR_SUCCESS;
	//g_mutex.lock();	
	bool bCloseTrade =false;
	//������һ��ƽ�ּ�¼�����ý���Ա�Ƿ��гֲ�
	if(indata.OffsetFlag == THOST_FTDC_OF_Close || indata.OffsetFlag == THOST_FTDC_OF_ForceClose
		||indata.OffsetFlag == THOST_FTDC_OF_CloseToday ||indata.OffsetFlag == THOST_FTDC_OF_CloseYesterday )
	{
		nErrorCode = CheckCloseVolume(indata);



		if (nErrorCode)
		{
			//g_mutex.unlock();
			return nErrorCode;
		}
	}
	TradeKey key(indata);
	m_mapTradeID2TradeInfo[key] = indata;
	std::map<std::string,std::vector<PlatformStru_TradeInfo>>::iterator it=m_mapTradeData.find(indata.InvestorID);
	if (it!= m_mapTradeData.end())
	{		
		it->second.push_back(indata);
	}
	else
	{
		std::vector<PlatformStru_TradeInfo> vecTrade;
		vecTrade.push_back(indata);
		m_mapTradeData[indata.InvestorID] = vecTrade;
	}
	
	//��¼�����ۼ�

	SettlementOperation  operate;
	memset(&operate,0,sizeof(operate));
	operate.nUserID = userId;
	strncpy(operate.szTraderID ,indata.InvestorID,sizeof(operate.szTraderID)-1);
	strncpy(operate.szTradingDay,msTodayString.c_str(),sizeof(operate.szTradingDay)-1);
	operate.nBusinessType = Business_Trade;
	operate.nActionType = Action_AddRecord;
	strncpy(operate.szOperationItem , indata.TradeID,sizeof(operate.szOperationItem)-1);
	strncpy(operate.szInstrumentID , indata.InstrumentID,sizeof(operate.szInstrumentID)-1);
	
	SaveAction2DB(operate);

	//����ɽ�
	ConfirmTrade();
	m_bModifiedTrade=true;
	return nErrorCode;
}

CF_ERROR SvrSettlementBusiness::DeleteTrade(PlatformStru_TradeInfo&  indata,int userId) 
{
	int nErrorCode = CF_ERROR_SUCCESS;

	//g_mutex.lock();

	TradeKey key(indata);
	m_mapTradeID2TradeInfo.erase(key);
	std::map<std::string,std::vector<PlatformStru_TradeInfo>>::iterator it=m_mapTradeData.find(indata.InvestorID);
	if (it!= m_mapTradeData.end())
	{		
		std::vector<PlatformStru_TradeInfo>::iterator it2 = it->second.begin();
		while ( it2 != it->second.end())
		{
			if (strcmp((*it2).TradeID ,indata.TradeID) == 0)
			{
				it2=it->second.erase(it2);
			}
			else
				++it2;
		}
	}
	
	//��¼�����ۼ�

	SettlementOperation  operate;
	memset(&operate,0,sizeof(operate));
	operate.nUserID = userId;
	strncpy(operate.szTraderID ,indata.InvestorID,sizeof(operate.szTraderID)-1);
	strncpy(operate.szTradingDay,msTodayString.c_str(),sizeof(operate.szTradingDay)-1);
	operate.nBusinessType = Business_Trade;
	operate.nActionType = Action_DeleteRecord;
	strncpy(operate.szOperationItem , indata.TradeID,sizeof(operate.szOperationItem)-1);
	strncpy(operate.szInstrumentID , indata.InstrumentID,sizeof(operate.szInstrumentID)-1);

	SaveAction2DB(operate);

	//����ɽ�
	ConfirmTrade();
	m_bModifiedTrade=true;
	return nErrorCode;
}
CF_ERROR SvrSettlementBusiness::ModifyTrade(PlatformStru_TradeInfo&  originaldata, PlatformStru_TradeInfo&  newdata,int userId)
{
	int nErrorCode = CF_ERROR_SUCCESS;

	//g_mutex.lock();



	TradeKey keyoriginal(originaldata);
	TradeKey keynew(newdata);
	m_mapTradeID2TradeInfo.erase(originaldata);
	m_mapTradeID2TradeInfo[keynew] = newdata;

	//ɾ���ɵĳɽ���¼
	std::map<std::string,std::vector<PlatformStru_TradeInfo>>::iterator it=m_mapTradeData.find(originaldata.InvestorID);
	if (it!= m_mapTradeData.end())
	{		
		std::vector<PlatformStru_TradeInfo>::iterator it2 = it->second.begin();
		while ( it2 != it->second.end())
		{
			TradeKey keytemp(*it2);
			if (keytemp == keyoriginal)
			{
				it2=it->second.erase(it2);
			}
			else
				++it2;
		}
	}
	//g_mutex.unlock();

	//����ɽ�
	ConfirmTrade();

	//g_mutex.lock();

	//����Ƿ��ƽ��
	if(newdata.OffsetFlag == THOST_FTDC_OF_Close || newdata.OffsetFlag == THOST_FTDC_OF_ForceClose
		||newdata.OffsetFlag == THOST_FTDC_OF_CloseToday ||newdata.OffsetFlag == THOST_FTDC_OF_CloseYesterday )
	{
		nErrorCode = CheckCloseVolume(newdata);

		if (nErrorCode)
		{
			//����޸�ƽ�ֳɽ�ʧ�ܣ���ɾ���ĳɽ���ӻ���
			std::map<std::string,std::vector<PlatformStru_TradeInfo>>::iterator it=m_mapTradeData.find(originaldata.InvestorID);
			if (it!= m_mapTradeData.end())
			{		
				it->second.push_back(originaldata);
			}
			else
			{
				std::vector<PlatformStru_TradeInfo> vecTrade;
				vecTrade.push_back(originaldata);
				m_mapTradeData[originaldata.InvestorID] = vecTrade;
			}
			//g_mutex.unlock();
			return nErrorCode;
		}

	}

	//�����޸ĺ���³ɽ�
	std::map<std::string,std::vector<PlatformStru_TradeInfo>>::iterator it3=m_mapTradeData.find(newdata.InvestorID);
	if (it3!= m_mapTradeData.end())
	{		
		it3->second.push_back(newdata);
	}
	else
	{
		std::vector<PlatformStru_TradeInfo> vecTrade;
		vecTrade.push_back(newdata);
		m_mapTradeData[newdata.InvestorID] = vecTrade;
	}

	//��¼�����ۼ�

	SettlementOperation  operate;
	memset(&operate,0,sizeof(operate));
	operate.nUserID = userId;
	strncpy(operate.szTraderID ,newdata.InvestorID,sizeof(operate.szTraderID)-1);
	strncpy(operate.szTradingDay,msTodayString.c_str(),sizeof(operate.szTradingDay)-1);
	operate.nBusinessType = Business_Trade;
	operate.nActionType = Action_ModifyRecord;
	strncpy(operate.szOperationItem , newdata.TradeID,sizeof(operate.szOperationItem)-1);
	strncpy(operate.szInstrumentID , newdata.InstrumentID,sizeof(operate.szInstrumentID)-1);

	SaveAction2DB(operate);

	m_bModifiedTrade=true;
	//����ɽ�
	ConfirmTrade();
	//g_mutex.unlock();
	return nErrorCode;
}

CF_ERROR SvrSettlementBusiness::ConfirmTrade()
{
	int nErrorCode = CF_ERROR_SUCCESS;
    //g_mutex.lock();
	m_mapPositionDetail.clear();
	m_mapPosition.clear();
	std::vector<UserInfo> vec;
	CInterface_SvrUserOrg::getObj().GetUserInfos(vec);
	std::vector<UserInfo>::iterator it= vec.begin();
	while (it!=vec.end())
	{
		std::vector<PlatformStru_TradeInfo> vecTrade;
		if(m_mapTradeData.end() != m_mapTradeData.find((*it).szAccount))
		{
			vecTrade = m_mapTradeData.find((*it).szAccount)->second;
		}
		nErrorCode =CInterface_SvrTradeData::getObj().SeUserNewTrades((*it).szAccount,vecTrade) ;

		it++;
	}

    //g_mutex.unlock();
	return nErrorCode;
}
CF_ERROR SvrSettlementBusiness::QueryPositionDetail(std::vector<PlatformStru_PositionDetail>& outData)
{
	int nErrorCode = CF_ERROR_SUCCESS;
	outData.clear();

	m_mapPositionDetail.clear();
	nErrorCode = CInterface_SvrTradeData::getObj().GetAllPositionDetail(m_mapPositionDetail);

	std::map<std::string,std::vector<PlatformStru_PositionDetail>>::iterator lIter = m_mapPositionDetail.begin();
	while(lIter != m_mapPositionDetail.end())
	{

		//����۸�ֵ
		std::vector<PlatformStru_PositionDetail>::iterator it2 =lIter->second.begin();
		while (it2!=lIter->second.end())
		{
			std::map<std::string,SettlementPriceField>::iterator itSettle=m_mapSettlePriceInternal.find(it2->InstrumentID);
			if (itSettle!=m_mapSettlePriceInternal.end())
			{
				it2->SettlementPrice = itSettle->second.SettlementPrice;
				it2->LastSettlementPrice = itSettle->second.LastSettlementPrice;
			}
			it2++;
		}
		std::vector<PlatformStru_PositionDetail>::iterator it =outData.end();
		outData.insert(it,lIter->second.begin(),lIter->second.end());
		lIter++;
	}
	if ( outData.size() == 0)
		nErrorCode= CF_ERROR_NO_POSITIONDETAIL;

	return nErrorCode;
}

CF_ERROR SvrSettlementBusiness::AddPositionDetail(PlatformStru_PositionDetail&  indata) 
{
	int nErrorCode = CF_ERROR_SUCCESS;

	//g_mutex.lock();
	std::map<std::string,std::vector<PlatformStru_PositionDetail>>::iterator it = m_mapPositionDetail.find(indata.InvestorID);
	if( it != m_mapPositionDetail.end())
	{
		it->second.push_back(indata);
	}
	else

	{
		//nErrorCode =CF_ERROR_COMMON_MEMEORY;
		//�޴˽���Ա�ֲ�ʱ
		vector<PlatformStru_PositionDetail> vecField;
		vecField.push_back(indata);
		m_mapPositionDetail[indata.InvestorID] = vecField;

	}

	//g_mutex.unlock();
	return nErrorCode;
}

CF_ERROR SvrSettlementBusiness::DeletePositionDetail(PlatformStru_PositionDetail&  indata) 
{
	int nErrorCode = CF_ERROR_SUCCESS;

	//g_mutex.lock();
	std::map<std::string,std::vector<PlatformStru_PositionDetail>>::iterator it = m_mapPositionDetail.find(indata.InvestorID);
	if( it != m_mapPositionDetail.end() )
	{
		std::vector<PlatformStru_PositionDetail>::iterator it2 = it->second.begin();
		while (it2!=it->second.end())
		{
			if((*it2).TradeID == indata.TradeID)
				it2=it->second.erase(it2);
			else
				++it2;
		}
	}
	else

	{
		nErrorCode =CF_ERROR_COMMON_MEMEORY;

	}
	//g_mutex.unlock();
	return nErrorCode;
}
CF_ERROR SvrSettlementBusiness::ModifyPositionDetail(PlatformStru_PositionDetail&  indata)
{
	int nErrorCode = CF_ERROR_SUCCESS;
	//g_mutex.lock();
	std::map<std::string,std::vector<PlatformStru_PositionDetail>>::iterator it = m_mapPositionDetail.find(indata.InvestorID);
	if( it != m_mapPositionDetail.end())
	{
		std::vector<PlatformStru_PositionDetail>::iterator it2 = it->second.begin();
		while (it2!=it->second.end())
		{
			if((*it2).TradeID == indata.TradeID)
			{
				it2=it->second.erase(it2);
				it->second.push_back(indata);
			}
			else
				it2++;
		}
	}
	else

	{
		nErrorCode =CF_ERROR_COMMON_MEMEORY;

	}
	//g_mutex.unlock();
	return nErrorCode;
}

CF_ERROR SvrSettlementBusiness::QueryPosition(std::vector<PlatformStru_Position>&  outData)
{
	int nErrorCode = CF_ERROR_SUCCESS;
	outData.clear();

	m_mapPosition.clear();
	nErrorCode = CInterface_SvrTradeData::getObj().GetAllPosition(m_mapPosition);

	std::map<std::string,std::vector<PlatformStru_Position>>::iterator lIter = m_mapPosition.begin();
	while(lIter != m_mapPosition.end())
	{

		//����۸�ֵ
		std::vector<PlatformStru_Position>::iterator it2 =lIter->second.begin();
		while (it2!=lIter->second.end())
		{
			std::map<std::string,SettlementPriceField>::iterator itSettle=m_mapSettlePriceInternal.find(it2->InstrumentID);
			if (itSettle!=m_mapSettlePriceInternal.end())
			{
				it2->SettlementPrice = itSettle->second.SettlementPrice;
			}
			//����ֲֳɱ�
			PlatformStru_InstrumentInfo ins;
			memset(&ins,0,sizeof(ins));
			CInterface_SvrTradeData::getObj().GetInstrumentInfo(it2->InstrumentID, ins);
			it2->PositionCost = it2->Position *it2->SettlementPrice * ins.VolumeMultiple;

			it2++;

		} 
		std::vector<PlatformStru_Position>::iterator it =outData.end();
		outData.insert(it,lIter->second.begin(),lIter->second.end());

		lIter++;
	}

	if ( outData.size() == 0)
		nErrorCode=  CF_ERROR_NO_POSITION;

	return nErrorCode;
}
//
//CF_ERROR SvrSettlementBusiness::AddPosition(PlatformStru_Position&  indata,int userId) 
//{
//	int nErrorCode = CF_ERROR_SUCCESS;
//
//	PositionDetailKey key(indata);
//
//	string strInstrumentID(indata.InstrumentID);
//	PlatformStru_InstrumentInfo InstrumentInfo;
//	if(CInterface_SvrTradeData::getObj().GetInstrumentInfo(strInstrumentID,InstrumentInfo))
//		nErrorCode = CF_ERROR_NO_INSTRUMENTS;
//	//���ö�Ӧ��PriceTick
//	indata.PriceTick=InstrumentInfo.PriceTick;
//
//	//�����Ƿ���
//	//PositionDetailInfo.bToday=m_RefInstruments.IsToday(PositionDetailInfo.OpenDate);
//
//	//g_mutex.lock();
//	std::map<PositionKey,PlatformStru_Position>::iterator it = m_Position.find(key);
//	if( it != m_Position.end() && it->second != NULL )
//	{
//		nErrorCode = CF_ERROR_REDUPLICATE_POSITIONDETAIL;
//	}
//	else
//
//	{
//		m_Position.insert(make_pair(key,indata));
//
//	}
//
//	//g_mutex.unlock();
//	return nErrorCode;
//}
//
//CF_ERROR SvrSettlementBusiness::DeletePosition(PlatformStru_Position&  indata,int userId) 
//{
//	int nErrorCode = CF_ERROR_SUCCESS;
//
//	PositionKey key(indata);
//
//	//g_mutex.lock();
//	std::map<PositionKey,PlatformStru_Position>::iterator it = m_Position.find(key);
//	if( it != m_Position.end() && it->second != NULL )
//	{
//		m_Position.erase(key);
//	}
//
//	//g_mutex.unlock();
//	return nErrorCode;
//}
//CF_ERROR SvrSettlementBusiness::ModifyPosition(PlatformStru_Position&  indata,int userId)
//{
//	int nErrorCode = CF_ERROR_SUCCESS;
//
//	PositionKey key(indata);
//
//	//g_mutex.lock();
//	std::map<PositionKey,PlatformStru_Position>::iterator it = m_Position.find(key);
//	if( it != m_Position.end() && it->second != NULL )
//	{
//		m_Position[key] = indata;
//	}
//
//	//g_mutex.unlock();
//	return nErrorCode;
//}
//
//CF_ERROR SvrSettlementBusiness::SetsettlementTime(TThostFtdcTimeType&  settlementTime)
//{
//	std::string time(settlementTime);
//	m_strautosettlementTime = time;
//
//	return CF_ERROR_SUCCESS;
//
//}
//

void SvrSettlementBusiness::LoadFund()
{
	m_mapFundData.clear();
	CInterface_SvrTradeData::getObj().GetAllFundInfo(m_mapFundData);

};
CF_ERROR SvrSettlementBusiness::QueryFund(std::vector<PlatformStru_TradingAccountInfo>& outData)
{

	int nErrorCode = CF_ERROR_SUCCESS;
	outData.clear();		

	std::map<std::string,PlatformStru_TradingAccountInfo>::iterator lIter = m_mapFundData.begin();
	while(lIter != m_mapFundData.end())
	{
		outData.push_back(lIter->second);
		lIter++;
	}

	if ( outData.size() == 0)
		nErrorCode= CF_ERROR_NO_FUND;

	return nErrorCode;
}
CF_ERROR SvrSettlementBusiness::ModifyFund(PlatformStru_TradingAccountInfo&  inData,int userId)
{

	int nErrorCode = CF_ERROR_SUCCESS;

	//g_mutex.lock();
	std::map<std::string,PlatformStru_TradingAccountInfo>::iterator it = m_mapFundData.find(inData.AccountID);
	if( it != m_mapFundData.end() )
	{
		

		//��¼�����ۼ�

		SettlementOperation  operate;
		memset(&operate,0,sizeof(operate));
		operate.nUserID = userId;
		strncpy(operate.szTraderID ,inData.AccountID,sizeof(operate.szTraderID)-1);
		strncpy(operate.szTradingDay,msTodayString.c_str(),sizeof(operate.szTradingDay)-1);
		operate.nBusinessType = Business_Fund;
		operate.nActionType = Action_ModifyRecord;
		if (it->second.StaticProfit != inData.StaticProfit)
		{			
			strncpy(operate.szOperationItem , FIELD_STATICPROFIT,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it->second.StaticProfit;
			operate.dNewContent = inData.StaticProfit;
			SaveAction2DB(operate);
		}
		if (it->second.Withdraw != inData.Withdraw)
		{			
			strncpy(operate.szOperationItem , FIELD_WITHDRAW,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it->second.Withdraw;
			operate.dNewContent = inData.Withdraw;
            SaveAction2DB(operate);
		}
		if (it->second.Deposit != inData.Deposit)
		{			
			strncpy(operate.szOperationItem , FIELD_DEPOSIT,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it->second.Deposit;
			operate.dNewContent = inData.Deposit;
			SaveAction2DB(operate);
		}
		if (it->second.DynamicProfit != inData.DynamicProfit)
		{			
			strncpy(operate.szOperationItem , FIELD_DYNAMICPROFIT,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it->second.DynamicProfit;
			operate.dNewContent = inData.DynamicProfit;
			SaveAction2DB(operate);
		}
		if (it->second.Available != inData.Available)
		{			
			strncpy(operate.szOperationItem , FIELD_AVAILABLE,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it->second.Available;
			operate.dNewContent = inData.Available;
			SaveAction2DB(operate);
		}
		if (it->second.CloseProfit != inData.CloseProfit)
		{			
			strncpy(operate.szOperationItem , FIELD_CLOSEPROFIT,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it->second.CloseProfit;
			operate.dNewContent = inData.CloseProfit;
			SaveAction2DB(operate);
		}
		if (it->second.PositionProfit != inData.PositionProfit)
		{			
			strncpy(operate.szOperationItem , FIELD_POSITIONPROFIT,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it->second.PositionProfit;
			operate.dNewContent = inData.PositionProfit;
			SaveAction2DB(operate);
		}
		if (it->second.Commission != inData.Commission)
		{			
			strncpy(operate.szOperationItem , FIELD_COMMISSION,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it->second.Commission;
			operate.dNewContent = inData.Commission;
			SaveAction2DB(operate);
		}
		if (it->second.CurrMargin != inData.CurrMargin)
		{			
			strncpy(operate.szOperationItem , FIELD_CURRMARGIN,sizeof(operate.szOperationItem)-1);
			operate.dOriginalContent =it->second.CurrMargin;
			operate.dNewContent = inData.CurrMargin;
			SaveAction2DB(operate);
		}

		
		
		
		
		m_mapFundData[inData.AccountID] = inData;
	}
	else
		nErrorCode = CF_ERROR_COMMON_MEMEORY;

	//g_mutex.unlock();
	return nErrorCode;
}
void SvrSettlementBusiness::LoadLatestSettlementDay(std::string& strSettlementDay)
{
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[200] = {0};	
	//strcpy_s(szBuffer, "select * from SETTLEMENTDAY t where rownum <= 1 order by DATEDIFF(d, to_char(t.SETTLEMENTDAY,'YYYY-MM-DD'),GETDATE())");
	//strcpy_s(szBuffer, "select * from SETTLEMENTDAY t where rownum <= 1 order by DATEDIFF(d, t.SETTLEMENTDAY,GETDATE())");
	strcpy_s(szBuffer, "select * from (select * from SETTLEMENTDAY t order by t.SETTLEMENTDAY DESC) where rownum <= 1 ");

	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return ;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			char szDay[11] = {0};
			std::vector<_variant_t> vValue = vNode[j];
			int i = 0;
			strcpy(szDay, vValue[i++].operator _bstr_t());			


			strSettlementDay = szDay;

		}
	} 
	return ;

}
CF_ERROR SvrSettlementBusiness::QuerySettlementInfo(PlatformStru_QrySettlementInfo&  inData,string&  outData)
{
	int nErrorCode = CF_ERROR_SUCCESS;

	string strSettlementDay = inData.Thost.TradingDay;

	if (strSettlementDay.empty())
	{
		LoadLatestSettlementDay(strSettlementDay);
	}
	else
	{

		strSettlementDay.insert(4,"-");
		strSettlementDay.insert(7,"-");
	}

	IInterfaceDBOpr * mpDBOpr = CInterface_SvrTradeData::getObj().GetInterfaceDBOpr();
	//װ�ؽ�������
	vector<PlatformStru_TradeInfo> lTradeVec;
	mpDBOpr->LoadTodayUserTrades("SETTLEMENT_TRADE",inData.Thost.InvestorID,strSettlementDay,lTradeVec);
	vector<PlatformStru_Position> lVecPosition;
	mpDBOpr->LoadTodayUserPositions("SETTLEMENT_POSITION",inData.Thost.InvestorID,strSettlementDay,lVecPosition);
	vector<PlatformStru_PositionDetail> lVecPosionDeatil;
	mpDBOpr->LoadTodayUserPositionDetails("SETTLEMENT_POSITIONDETAIL",inData.Thost.InvestorID,strSettlementDay,lVecPosionDeatil);
	PlatformStru_TradingAccountInfo lFundInfo;
	if( CF_ERROR_SUCCESS != mpDBOpr->LoadTodayUserFundInfo("SETTLEMENT_FUNDINFO",inData.Thost.InvestorID,strSettlementDay,lFundInfo))
		return CF_ERROR_NO_SETTLEMENTINFO;

	std::string strContent="";
	std::string strSpace=" ";
	std::string strTitle ="�й������ڻ����޹�˾\r\n";
	std::string segmentation="";
	std::string strSeparator="|";
	std::string strnewline = "\r\n";
	std::string strSep = "  ";
	string strdDirection="  ";
	for(int i=0;i< 120;i++)
	{
		segmentation+="-";
	}
	for(int i=0;i< 20;i++)
	{
		strContent+=strSpace;
	}
	strContent+=strTitle;
	for(int i=0;i< 40;i++)
	{
		strContent+=strSpace;
	}

	strContent+= "�Ʊ�ʱ��"+strSettlementDay+strnewline+segmentation+strnewline;

	strContent+= "����Ա��";
	strContent+=inData.Thost.InvestorID;
	strContent+=strnewline;
	strContent+= "�������ڣ�"+strSettlementDay+strnewline+segmentation+"\r\n\r\n\r\n";
	strContent+="                                        �ʽ�״��  ���֣������";
	strContent+=strnewline+segmentation+strnewline;

	std::string strFund="";

	strFund+="�ڳ���棺    ";
	strFund+=fundDouble2String(lFundInfo.StaticProfit);
	strFund+="    ���֤��";
	strFund+=fundDouble2String(lFundInfo.DeliveryMargin);
	strFund+="    ��Ѻ�仯��";
	strFund+=fundDouble2String(lFundInfo.Mortgage-lFundInfo.PreMortgage)+strnewline;
	strFund+="�� �� ��    ";
	strFund+=fundDouble2String(lFundInfo.Deposit -lFundInfo.Withdraw);
	strFund+="    ��ĩ��棺  ";
	strFund+=fundDouble2String(lFundInfo.DynamicProfit);
	strFund+="    �����ʽ�  ";
	strFund+=fundDouble2String(lFundInfo.Available)+strnewline;
	strFund+="ƽ��ӯ����    ";
	strFund+=fundDouble2String(lFundInfo.CloseProfit);
	strFund+="    �� Ѻ ��  ";
	strFund+=fundDouble2String(lFundInfo.Mortgage);
	strFund+="    �� �� �ȣ�                ";
	strFund+=Risk2String(lFundInfo.RiskDegree);
	strFund+=strnewline;
	strFund+="�ֲֶ���ӯ����";
	strFund+=fundDouble2String(lFundInfo.PositionProfit);
	strFund+="    �ͻ�Ȩ�棺  ";
	strFund+=fundDouble2String(lFundInfo.DynamicProfit);
	strFund+="    �� �� �ѣ�   ";
	strFund+=fundDouble2String(lFundInfo.Commission)+strnewline;
	strFund+="��֤��ռ�ã�  ";
	strFund+=fundDouble2String(lFundInfo.CurrMargin)+"\r\n\r\n\r\n";


	std::string strTrade="";
	if (lTradeVec.size()>0)
	{



		strTrade+="                                       �ɽ���¼";
		strTrade+=strnewline+segmentation+strnewline;
		strTrade+="| �ɽ�����|������| ��Լ |����|Ͷ��|     �ɽ��� | ���� |        �ɽ��� |��ƽ|          ������ |       ƽ��ӯ�� |   �ɽ����  |";
		strTrade+=strnewline+segmentation+strnewline;
		int sumVolume =0;
		double sumTradeMoney=0;
		double sumCommission=0;
		double sumCloseProfit=0;

		for (UINT i = 0; i < lTradeVec.size(); i++)
		{
			strTrade+=strSeparator;
			strTrade+=lTradeVec[i].TradeDate;
			strTrade+=strSeparator;
			strTrade+=m_pSettlementCalc->ExchangeID2String(lTradeVec[i].ExchangeID);
			strTrade+=strSeparator;
			strTrade+=lTradeVec[i].InstrumentID;
			strTrade+=strSeparator;
			strdDirection = m_pSettlementCalc->TradeDirection2String(lTradeVec[i].Direction);
			if (strdDirection == "��")
			{
				strTrade+=strdDirection + strSep;
			}
			else if(strdDirection == "��")
			{
				strTrade+= strSep + strdDirection  ;
			}

			strTrade+=strSeparator;
			strTrade+=m_pSettlementCalc->HedgeFlag2String(lTradeVec[i].HedgeFlag);
			strTrade+=strSeparator;
			strTrade+=Price2String(lTradeVec[i].Price);
			strTrade+=strSeparator+strSep;
			strTrade+=Volume2String(lTradeVec[i].Volume);
			strTrade+=strSeparator;
			strTrade+=Double2String(lTradeVec[i].Volume*lTradeVec[i].Price);
			strTrade+=strSeparator;
			strTrade+=m_pSettlementCalc->OffsetFlag2String(lTradeVec[i].OffsetFlag);
			strTrade+=strSeparator;
			strTrade+=Double2String(lTradeVec[i].TradeCommission);
			strTrade+=strSeparator;
			strTrade+=Double2String(lTradeVec[i].CloseProfitByDate);
			strTrade+=strSeparator;
			strTrade+=lTradeVec[i].TradeID;
			strTrade+=strSeparator+strnewline;
			sumVolume+=lTradeVec[i].Volume;
			sumTradeMoney+=lTradeVec[i].Volume*lTradeVec[i].Price;
			sumCommission+=lTradeVec[i].TradeCommission;
			sumCloseProfit+=lTradeVec[i].CloseProfitByDate;

		}
		strTrade+=segmentation+strnewline;
		strTrade+=strSeparator;
		strTrade+="��"+Volume2String(lTradeVec.size())+"��";
		strTrade+="|      |      |    |    |              |";
		strTrade+=Volume2String(sumVolume)+strSeparator;
		strTrade+=Double2String(sumTradeMoney)+"|    |";
		strTrade+=Double2String(sumCommission)+strSeparator;
		strTrade+=Double2String(sumCloseProfit)+"|            |";
		strTrade+=strnewline+segmentation+"\r\n\r\n\r\n";
	}


	std::string strPosionDeatil="";
	if (lVecPosionDeatil.size()>0)
	{



		strPosionDeatil+="                                       �ֲ���ϸ";
		strPosionDeatil+=strnewline+segmentation+strnewline;
		strPosionDeatil+="|������|  ��Լ|��������|Ͷ/��|��/��| �ֲ���|     ���ּ� |     ����� |      ����� |        ƽ��ӯ�� |        ����ӯ��|        ��֤��  |";
		strPosionDeatil+=strnewline+segmentation+strnewline;
		int sumVolume =0;
		double sumMargin=0;
		double sumPositionProfit=0;
		double sumCloseProfit=0;
		for (UINT i = 0; i < lVecPosionDeatil.size(); i++)
		{

			strPosionDeatil+=strSeparator;
			strPosionDeatil+=m_pSettlementCalc->ExchangeID2String(lVecPosionDeatil[i].ExchangeID);
			strPosionDeatil+=strSeparator;
			strPosionDeatil+=lVecPosionDeatil[i].InstrumentID;
			strPosionDeatil+=strSeparator;
			strPosionDeatil+=lVecPosionDeatil[i].OpenDate;			
			strPosionDeatil+=strSeparator;
			strPosionDeatil+=m_pSettlementCalc->HedgeFlag2String(lVecPosionDeatil[i].HedgeFlag);
			strPosionDeatil+=strSeparator;
			strdDirection = m_pSettlementCalc->TradeDirection2String(lVecPosionDeatil[i].Direction);
			if (strdDirection == "��")
			{
				strPosionDeatil+=strdDirection + strSep;
			}
			else if(strdDirection == "��")
			{
				strPosionDeatil+= strSep + strdDirection  ;
			}

			strPosionDeatil+=strSeparator+strSep+strSep;			
			strPosionDeatil+="  "+Volume2String(lVecPosionDeatil[i].Volume);
			strPosionDeatil+=strSeparator;
			strPosionDeatil+=Price2String(lVecPosionDeatil[i].OpenPrice);
			strPosionDeatil+=strSeparator;
			strPosionDeatil+=Price2String(lVecPosionDeatil[i].LastSettlementPrice);
			strPosionDeatil+=strSeparator;
			strPosionDeatil+=Price2String(lVecPosionDeatil[i].SettlementPrice);
			strPosionDeatil+=strSeparator;
			strPosionDeatil+=Double2String(lVecPosionDeatil[i].CloseProfitByDate);
			strPosionDeatil+=strSeparator;
			strPosionDeatil+=Double2String(lVecPosionDeatil[i].PositionProfitByDate);
			strPosionDeatil+=strSeparator;
			strPosionDeatil+=Double2String(lVecPosionDeatil[i].Margin);

			strPosionDeatil+=strSeparator+strnewline;
			sumVolume+=lVecPosionDeatil[i].Volume;
			sumMargin+=lVecPosionDeatil[i].Margin;
			sumPositionProfit+=lVecPosionDeatil[i].PositionProfitByDate;
			sumCloseProfit+=lVecPosionDeatil[i].CloseProfitByDate;

		}
		strPosionDeatil+=segmentation+strnewline;
		strPosionDeatil+=strSeparator;
		strPosionDeatil+="��"+Volume2String(lVecPosionDeatil.size())+"��";
		strPosionDeatil+="|    |        |     |     |";
		strPosionDeatil+=Volume2String(sumVolume)+"|              |            |           | ";
		strPosionDeatil+=Double2String(sumCloseProfit)+strSeparator;
		strPosionDeatil+=Double2String(sumPositionProfit)+strSeparator;
		strPosionDeatil+=Double2String(sumMargin)+strSeparator;
		strPosionDeatil+=strnewline+segmentation+"\r\n\r\n\r\n";
	}


	std::string strPosition="";
	if (lVecPosition.size()>0)
	{



		strPosition+="                                       �ֲֻ���";
		strPosition+=strnewline+segmentation+strnewline;
		strPosition+="|  ��Լ  | ��/��  | �ֲ��� |   �����  |     �����  |     �ֲֶ���ӯ��|     ��֤��ռ�� |        ƽ��ӯ�� |Ͷ��|";
		strPosition+=strnewline+segmentation+strnewline;
		int sumVolume =0;
		double sumMargin=0;
		double sumPositionProfit=0;
		double sumCloseProfit=0;
		for (UINT i = 0; i < lVecPosition.size(); i++)
		{

			strPosition+=strSeparator;
			strPosition+=lVecPosition[i].InstrumentID;
			strPosition+=strSep+strSeparator;
			strPosition+=m_pSettlementCalc->Direction2String(lVecPosition[i].PosiDirection);			
			strPosition+=strSep+strSep+strSeparator;			
			strPosition+=Volume2String(lVecPosition[i].Position);
			strPosition+=strSep+strSep+strSeparator;
			strPosition+=Price2String(lVecPosition[i].PreSettlementPrice);
			strPosition+=strSeparator;
			strPosition+=Price2String(lVecPosition[i].SettlementPrice);			
			strPosition+=strSeparator;
			strPosition+=Double2String(lVecPosition[i].PositionProfit);
			strPosition+=strSeparator;
			strPosition+=Double2String(lVecPosition[i].UseMargin);
			strPosition+=strSeparator;
			strPosition+=Double2String(lVecPosition[i].CloseProfit);
			strPosition+=strSeparator;
			strPosition+=m_pSettlementCalc->HedgeFlag2String(lVecPosition[i].HedgeFlag);
			strPosition+=strSeparator+strnewline;
			sumVolume+=lVecPosition[i].Position;
			sumMargin+=lVecPosition[i].UseMargin;
			sumPositionProfit+=lVecPosition[i].PositionProfit;
			sumCloseProfit+=lVecPosition[i].CloseProfit;

		}
		strPosition+=segmentation+strnewline;
		strPosition+=strSeparator;
		strPosition+="��"+Volume2String(lVecPosition.size())+"��";
		strPosition+="|          |";
		strPosition+=Volume2String(sumVolume)+"|              |               |";
		strPosition+=Double2String(sumPositionProfit)+strSeparator;
		strPosition+=Double2String(sumMargin)+strSeparator;
		strPosition+=Double2String(sumCloseProfit)+strSeparator;		
		strPosition+=strnewline+segmentation+"\r\n\r\n\r\n";
	}

	strContent+= strFund + strTrade +strPosionDeatil +strPosition;
	outData = strContent;

	//const char filename[] = "mytext.txt";
	//ofstream o_file;
	//o_file.open(filename);

	//o_file << outData;
	//o_file.close();



	return nErrorCode;

}
string SvrSettlementBusiness::fundDouble2String(double field)
{
	if (field != InvalideValue )
	{
		char strVolume[128];
		//memset(strVolume,0,strlen(strVolume)+1);
		if (field > pow(10.0, 20))
		{
			sprintf(strVolume, "%.2f",field);
		}
		else
		{
			sprintf(strVolume, "%20.2f",field);
		}
		return strVolume;
	}
	else
		return "";

}
string SvrSettlementBusiness::Risk2String(double field)
{
	if (field != InvalideValue )
	{
		char strVolume[64];
		//memset(strVolume,0,strlen(strVolume)+1);
		sprintf(strVolume, "%.2f%%",field);
		return strVolume;
	}
	else
		return "";

}
string SvrSettlementBusiness::Double2String(double field)
{
	if (field != InvalideValue)
	{
		char strVolume[64];
		//memset(strVolume,0,strlen(strVolume)+1);
		if (field > pow(10.0, 16))
		{
			sprintf(strVolume, "%.2f",field);
		}
		else
		{
		    sprintf(strVolume, "%16.2f",field);
		}
		return strVolume;
	}
	else
		return "";
}
string SvrSettlementBusiness::Price2String(double field)
{
	if (field != InvalideValue)
	{
		char strVolume[64];
		//memset(strVolume,0,strlen(strVolume)+1);
		if (field > pow(10.0, 4))
		{
			sprintf(strVolume, "%.2f",field);
		}
		else
		{
		    sprintf(strVolume, "%12.3f",field);
		}
		return strVolume;
	}
	else
		return "";
}
string SvrSettlementBusiness::Volume2String(int field)
{
	if (field != InvalideValue)
	{
		char strVolume[64];
		//memset(strVolume,0,strlen(strVolume)+1);
		sprintf(strVolume, "%d",field);
		return strVolume;
	}
	else
		return "";
}

int SvrSettlementBusiness::time2Mins(char* strTime)
{
	char* pTemp;
	int iTime=-1;
	int iHours=-1;
	int iMinutes=-1;
	pTemp=strstr(strTime,":");
	if(pTemp!=NULL)
	{

		char sTime[5];
		strncpy(sTime,strTime,strlen(strTime)-strlen(pTemp));
		//*pTemp='\0';
		iHours=atoi(sTime)%24;
		iMinutes=atoi(pTemp+1)%60;
	}
	else 
	{
		iHours=atoi(strTime)%100%24;
		iMinutes=atoi(strTime)/100%60;
	}
	iTime=iHours*60+iMinutes;
	return iTime;
}

//	�ж�һ��ʱ���Ƿ���ָ����ʱ����ڡ�ʱ����һ��ķ����������α�ʾ
bool SvrSettlementBusiness::IsInTimePeriod(int iMins,int StartMins,int EndMins)
{
	iMins=iMins%(24*60);
	StartMins=StartMins%(24*60);
	EndMins=EndMins%(24*60);
	if(EndMins>=StartMins&&iMins>=StartMins&&iMins<EndMins||
		EndMins<StartMins&&(iMins>=StartMins||iMins<EndMins))
		return TRUE;
	else return FALSE;
}

void SvrSettlementBusiness::SetSettleSocket(int nSocket)
{
	m_SettleSocket=nSocket;
}
void SvrSettlementBusiness::RemoveSettleSocket(int nSocket)
{
	if (m_SettleSocket == nSocket)
	{
		m_SettleSocket=INVALID_SOCKET;
		SetSettlementStatus(No_Settlementing);
	}
	
}
int SvrSettlementBusiness::GetSettleSocket()
{	
	return m_SettleSocket;
}