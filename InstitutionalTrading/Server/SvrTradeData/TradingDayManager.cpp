#include "stdafx.h"
#include "TradingDayManager.h"
#include "SafeLock.h"
#include "..\SvrDBOpr\Interface_SvrDBOpr.h"
#include "time.h"
CTradingDayManager::CTradingDayManager()
{
	LoadHistoryTradingDay();
	LoadHistorySettmentedDay();
}
CTradingDayManager::~CTradingDayManager()
{
	CSafeLock mLock(mMutex);
	//msetAllTradingDay.erase(msetAllTradingDay.begin(),msetAllTradingDay.end());
	mmapAllTradingDay.clear();
	msetAllSettlementedDay.clear();
};

//װ����ʷ�Ľ�����
void CTradingDayManager::LoadHistoryTradingDay()
{
	CSafeLock mLock(mMutex);
	//װ�����ݿ�
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[100] = {0};	
	strcpy_s(szBuffer, "select * from TradingDay t");
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
			i++;
            eTradingDayInitStatus leInitStatus = static_cast<eTradingDayInitStatus>(vValue[i++].intVal);
			

			std::map<std::string,eTradingDayInitStatus>::iterator lIter = mmapAllTradingDay.find(szDay);
			if(lIter == mmapAllTradingDay.end() || 
				(lIter != mmapAllTradingDay.end() && lIter->second == TRADING_DAY_INIT_STATUS_NOTHING))
				mmapAllTradingDay[szDay] = leInitStatus;

		}
	}

	return;
}

//װ����ʷ�Ľ�����
void  CTradingDayManager::LoadHistorySettmentedDay()
{
	CSafeLock mLock(mMutex);
	//װ�����ݿ�
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[100] = {0};	
	strcpy_s(szBuffer, "select * from SETTLEMENTDAY t");
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


			msetAllSettlementedDay.insert(szDay);

		}
	}

	return;
}

//�ж��Ƿ������г�ʼ�����Ľ�����δ����
bool CTradingDayManager::HasInitTradingDayWithNoSettlement(std::string & nsOldestInitTradingDayWithNoSettlement)
{
	nsOldestInitTradingDayWithNoSettlement.clear();
	std::map<std::string,eTradingDayInitStatus>::iterator  lIter = mmapAllTradingDay.begin();
	while(lIter != mmapAllTradingDay.end())
	{
		if(lIter->second == TRADING_DAY_INIT_STATUS_INSTRUMENT && !IsSetttemnted(lIter->first))
		{
			nsOldestInitTradingDayWithNoSettlement = lIter->first;			
			break;
		}
		lIter++;
	}
	if(nsOldestInitTradingDayWithNoSettlement.empty())
		return false;
	else
		return true;
}

//��ȡ��һ��������,���һ��������δ��ʼ������ý���������Ч�Ľ�����
bool CTradingDayManager::GetLastTradingData(const std::string & nDay,std::string & nLastTradingDay)
{
	CSafeLock mLock(mMutex);

	if(nDay.empty())
		return false;

	std::map<std::string,eTradingDayInitStatus>::iterator  lIter = mmapAllTradingDay.find(nDay);
	while(lIter != mmapAllTradingDay.end())
	{
		std::map<std::string,eTradingDayInitStatus>::iterator lIterLast = --lIter;
		if(lIterLast != mmapAllTradingDay.end() && lIterLast->second == TRADING_DAY_INIT_STATUS_INSTRUMENT)
		{
			//nLastTradingDay == lIterLast->first;
			nLastTradingDay.assign(lIterLast->first);
			
			return true;
		}
		
	}
	return false;
	//nday ��ʷ������ֱ������ʷ�б��ҵ�Ϊֹ
	//time_t lTime;
	//std::string lsTempStr;
	//struct tm * lOldTimeinfo; 
	//time(&lTime);
	//lOldTimeinfo = localtime(&lTime);
	//sscanf_s(nDay.c_str(),"%04d-%02d-%02d",&lOldTimeinfo->tm_year,&lOldTimeinfo->tm_mon,&lOldTimeinfo->tm_mday);

	//lOldTimeinfo->tm_year -= 1900;
	//lOldTimeinfo->tm_mon -= 1;
	//time_t lOldTime = mktime(lOldTimeinfo);

	////���Ե���30�죬��ʷ��һ�����ڱ���һ���ǽ����գ����û����ʧ��
	//bool lbRet = false;
	//time_t lYesTime = lOldTime ;
	//int i = 0;
	//do 
	//{
	//	lYesTime = lYesTime - 24* 3600;
	//	struct tm * ltmYestorday = localtime(&lYesTime);
	//	char ltempBuf[11] = {0};
	//	sprintf_s(ltempBuf,"%04d-%02d-%02d",ltmYestorday->tm_year+1900,ltmYestorday->tm_mon+1,ltmYestorday->tm_mday);
	//	std::map<std::string,eTradingDayInitStatus>::iterator  lIter = mmapAllTradingDay.find(ltempBuf);
	//	if(lIter != mmapAllTradingDay.end() && lIter->second == TRADING_DAY_INIT_STATUS_INSTRUMENT)
	//	{
	//		nLastTradingDay = ltempBuf;
	//		lbRet = true;
	//		break;
	//	}
	//	i++;
 //      
	//} while (i <= 30);



	//return lbRet;
}

//���һ��������
bool CTradingDayManager::AddTradingDay(const std::string & nDay)
{
	CSafeLock mLock(mMutex);
	if(mmapAllTradingDay.find(nDay) == mmapAllTradingDay.end())
	{
		mmapAllTradingDay[nDay] = TRADING_DAY_INIT_STATUS_NOTHING;
		
		if(mbNeedSaveDB != false)
		{
			//���
			int nErrorCode;
			CInterface_SvrDBOpr::getObj().SaveOneTradingDay("TradingDay",nDay,TRADING_DAY_INIT_STATUS_NOTHING,nErrorCode);
		}
	}
	return true;
}


//����һ�������ճ�ʼ�������
bool CTradingDayManager::SetTradingDayInit(const std::string & nDay)
{
	CSafeLock mLock(mMutex);
	std::map<std::string,eTradingDayInitStatus>::iterator  lIter = mmapAllTradingDay.find(nDay);
	if(lIter != mmapAllTradingDay.end())
	{
		mmapAllTradingDay[nDay] = TRADING_DAY_INIT_STATUS_INSTRUMENT;
		
		if(mbNeedSaveDB != false)
		{
			//���
			int nErrorCode;
			CInterface_SvrDBOpr::getObj().UpdateTradingDayInitStatus("TradingDay",nDay,TRADING_DAY_INIT_STATUS_INSTRUMENT,nErrorCode);
		}
	}
	return true;
}
//���һ��������
bool CTradingDayManager::AddSettlementedDay(const std::string & nDay)
{
	CSafeLock mLock(mMutex);
	if(msetAllSettlementedDay.find(nDay) == msetAllSettlementedDay.end())
	{
		msetAllSettlementedDay.insert(nDay);
		
		if(mbNeedSaveDB != false)
		{
			//���
			int nErrorCode;
			CInterface_SvrDBOpr::getObj().SaveOneSettlementDay("SETTLEMENTDAY",nDay,nErrorCode);
		}
	}
	return true;
}

bool CTradingDayManager::IsTradingDay(const std::string & nsDay)
{
	CSafeLock mLock(mMutex);
	if(mmapAllTradingDay.find(nsDay) == mmapAllTradingDay.end())
		return false;

	return true;
}

//�ǲ���ϵͳ���еĵ�һ��
bool CTradingDayManager::IsFirstDay()
{
	CSafeLock mLock(mMutex);
	return mmapAllTradingDay.size() == 0 ? true : false;
}

//�Ƿ�����
bool CTradingDayManager::IsSetttemnted(const std::string & nsDay)
{
	CSafeLock mLock(mMutex);
	if(msetAllSettlementedDay.find(nsDay) == msetAllSettlementedDay.end())
		return false;

	return true;
}


//��ȡ���µ��ѽ���Ľ�����
bool CTradingDayManager::GetNewestSettlementDay( std::string & nDay)
{
	CSafeLock mLock(mMutex);
	std::set<std::string>::reverse_iterator lLast = msetAllSettlementedDay.rbegin();
	if(lLast == msetAllSettlementedDay.rend())
		return false;
	else
	{
		nDay.assign( *lLast);
		return true;
	}
}