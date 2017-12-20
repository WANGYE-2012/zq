#include "stdafx.h"
#include "CustomInterfaceSvrInstruments.h"
#include "SvrInstruments.h"
#include "easymutex.h"
#include "CommonPkg.h"

#include "..\\SvrDBOpr\\Interface_SvrDBOpr.h"
#pragma comment(lib, "SvrDBOpr.lib")
#include "CommonDef.h"
CCustomInterfaceSvrInstruments* CCustomInterfaceSvrInstruments::m_pObj=NULL;


CCustomInterfaceSvrInstruments::CCustomInterfaceSvrInstruments()
{
}

CCustomInterfaceSvrInstruments::~CCustomInterfaceSvrInstruments()
{
}

CCustomInterfaceSvrInstruments& CCustomInterfaceSvrInstruments::getObj(void)
{
	g_mutex.lock();
	if(!m_pObj)
		m_pObj=new CCustomInterfaceSvrInstruments();
	g_mutex.unlock();
	return *m_pObj;
}

//д���ݿ�
CF_ERROR CCustomInterfaceSvrInstruments::WriteSingleInstrumentToDB(const InstrumentField & nInstruments)
{
	char szBuffer[MAX_SQL_LENGTH] = {0};	
	sprintf_s(szBuffer,MAX_SQL_LENGTH, "insert into CFINSTRUEMENTS values( '%s','%s',\
					  '%s','%s','%s',%c,%d,%d,%d,%d,%d,%d,%d,'%f','%s','%s','%s','%s','%s',%c,%d,%c,%c,%f,%f   )",
					  nInstruments.InstrumentID,
					  nInstruments.ExchangeID,
					  nInstruments.InstrumentName,
					  nInstruments.ExchangeInstID,
					  nInstruments.ProductID,
					  nInstruments.ProductClass,
					  nInstruments.DeliveryYear,
					  nInstruments.DeliveryMonth,
					  nInstruments.MaxMarketOrderVolume,
					  nInstruments.MinMarketOrderVolume,
					  nInstruments.MaxLimitOrderVolume,
					  nInstruments.MinLimitOrderVolume,
					  nInstruments.VolumeMultiple,
					  nInstruments.PriceTick,
					  nInstruments.CreateDate,
					  nInstruments.OpenDate,
					  nInstruments.ExpireDate,
					  nInstruments.StartDelivDate,					  
					  nInstruments.EndDelivDate,
					  nInstruments.InstLifePhase,	
					  nInstruments.IsTrading,
					  nInstruments.PositionType,
					  nInstruments.PositionDateType	,				  
					  nInstruments.LongMarginRatio,
					  nInstruments.ShortMarginRatio  );

	int nErrorCode = 0;
    if ( !CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
		return (CF_ERROR)nErrorCode;

	return CF_ERROR_SUCCESS;
}

//---------------------------------------------------------------------------------
//	�������к�Լ
//---------------------------------------------------------------------------------
CF_ERROR CCustomInterfaceSvrInstruments::WriteAllInstruments(const _MapInstrumentType & nMapInstruments)
{
	if(nMapInstruments.size() == 0)
		return CF_ERROR_COMMON_INPUT_PARAM;

	g_mutex.lock();
	mMapInstruments.erase(mMapInstruments.begin(),mMapInstruments.end());
	mMapInstruments = nMapInstruments;
	
	g_mutex.unlock();

	return CF_ERROR_SUCCESS;
}


//---------------------------------------------------------------------------------
//	��ȡ���к�Լ
//---------------------------------------------------------------------------------
CF_ERROR CCustomInterfaceSvrInstruments::GetAllInstruments( _MapInstrumentType & nMapInstruments)
{
	CF_ERROR lError = CF_ERROR_NO_INSTRUMENTS;
	g_mutex.lock();
	if(mMapInstruments.size() != 0)		
	{
		 nMapInstruments = mMapInstruments;
		 lError = CF_ERROR_SUCCESS;
	}	
	g_mutex.unlock();
	return lError;
}

//---------------------------------------------------------------------------------
//	��ȡ������Լ�ľ�����Ϣ
//---------------------------------------------------------------------------------
CF_ERROR CCustomInterfaceSvrInstruments::GetInstrumentField( const std::string & nsInstrumentID,
									InstrumentField & nInstruments)
{
	CF_ERROR lError = CF_ERROR_NO_INSTRUMENTS;
	g_mutex.lock();
	
	_MapInstrumentType::iterator lIter = mMapInstruments.find(nsInstrumentID);
	if(lIter != mMapInstruments.end())
	{
		memcpy(&nInstruments,&lIter->second,sizeof(InstrumentField));
		lError = CF_ERROR_SUCCESS;
	}
	g_mutex.unlock();

	return lError;
}

//---------------------------------------------------------------------------------
//	׷�ӻ���µ�����Լ
//  ���أ� CF_ERROR_SUCCESS ����ɹ�
//         CF_ERROR_COMMON_MEMEORY �����ڴ����û����ӳɹ�
//---------------------------------------------------------------------------------
CF_ERROR CCustomInterfaceSvrInstruments::AppendInstrument( const InstrumentField & nInstruments)
{

	CF_ERROR lError = CF_ERROR_COMMON_MEMEORY;
	g_mutex.lock();
	_MapInstrumentType::iterator lIter = mMapInstruments.find(nInstruments.InstrumentID);
	if(lIter != mMapInstruments.end())
	{
		memcpy(&lIter->second ,&nInstruments,sizeof(InstrumentField));
		lError = CF_ERROR_SUCCESS;
	}
	else
	{
		
		mMapInstruments.insert(std::make_pair(nInstruments.InstrumentID,nInstruments));		
		lError = CF_ERROR_SUCCESS;
	}
	g_mutex.unlock();
	return lError;

}

