/***************************************************************************//**
*   @file          QuotSpi.cpp
*   @brief        �ۺϽ���ƽ̨(CTP)������صĻص�����
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/09/14
********************************************************************************/
#include "stdafx.h"

#include "QuotSpi.hpp"
#include "PlatFormService.hpp"
#include "../inc/ISvr/BusinessData.hpp"

#include "../inc/Module-Misc/packagecache.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"

#define LOG_INFO(fmt, ...) \
	do{\
	if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CQuotSpi", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
namespace Pats
{
	CQuotSpi::CQuotSpi(CPlatFormService& RefCPatsPlatFormService)
		:m_RefMdApi(*(RefCPatsPlatFormService.m_pQuotApi)), 
		m_RefPackageCache(*(RefCPatsPlatFormService.m_PCache)),
		m_PlatformParam(RefCPatsPlatFormService.m_PlatformParam),
		m_RefCPatsPlatFormService(RefCPatsPlatFormService)
	{
		m_pWriteLog=new zqWriteLog();
		LOG_INFO("CQuotSpi::CQuotSpi(����CQuotSpi����)");
	}

	CQuotSpi::~CQuotSpi()
	{

		LOG_INFO("CQuotSpi::~CQuotSpi(����CQuotSpi����)");
		delete m_pWriteLog;
	}

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	void CQuotSpi::OnFrontConnected()
	{
		//GetLocalTime(&m_ConnectTime);//�������ʱ��

		DataFrontConnected tmp;
		tmp.Head.BID=BID_FrontConnected;
		tmp.Head.Size=sizeof(DataFrontConnected);
		tmp.bTrader=false;
		m_RefPackageCache.SaveDataTo(BID_FrontConnected, 0, 0, 0, sizeof(tmp), &tmp);

		CThostFtdcReqUserLoginField req;
		memset(&req, 0, sizeof(req));
		strncpy(req.BrokerID, m_PlatformParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
		strncpy(req.UserID, m_PlatformParam.UserID.c_str(),sizeof(req.UserID)-1);
		strncpy(req.Password, m_PlatformParam.UserPW.c_str(),sizeof(req.Password)-1);
		strncpy(req.OneTimePassword,m_PlatformParam.OneTimePassword.c_str(),sizeof(req.OneTimePassword)-1);

		m_PlatformParam.QuotStatus=CPlatFormParam::CONNSTATUS_Logining;

		int iRequestID=m_PlatformParam.RequestID++;
		int iRlt = m_RefMdApi.ReqUserLogin(&req, iRequestID);

		LOG_INFO("CQuotSpi::OnFrontConnected(�뽻�����ɹ�����TCP����):\n"
			"\t\t\t --->ReqUserLogin(�û���¼����):ret=[%d],iRequestID=[%d],\n"
			"\t\t\t BrokerID=[%s],\t UserID=[%s],",
			iRlt, iRequestID,
			req.BrokerID, req.UserID);
	}

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	void CQuotSpi::OnFrontDisconnected(int nReason)
	{ 
		LOG_INFO("CQuotSpi::OnFrontDisconnected(�뽻����ʧȥTCP����):nReason=[%#x]",nReason);
		DataFrontDisconnected tmp;
		tmp.Head.BID=BID_FrontDisconnected;
		tmp.Head.Size=sizeof(DataFrontDisconnected);
		tmp.bTrader=false;
		tmp.nReason=nReason;
		m_RefPackageCache.SaveDataTo(BID_FrontDisconnected, 0, 0, 0, sizeof(tmp), &tmp);
		m_PlatformParam.QuotStatus=CPlatFormParam::CONNSTATUS_Disconnected;
	}

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	void CQuotSpi::OnHeartBeatWarning(int nTimeLapse)
	{
		LOG_INFO("CQuotSpi::OnHeartBeatWarning:nTimeLapse=[%x]",nTimeLapse);
	}

	///��¼������Ӧ
	void CQuotSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		if(pRspUserLogin)
		{
			DataRspUserLogin tmp;
			tmp.bTrader       = false;
			tmp.nRequestID = nRequestID;
			tmp.bIsLast    = bIsLast;
			tmp.CurTickCount = GetTickCount();
			tmp.UserLoginField = *pRspUserLogin;
			if(pRspInfo)
				tmp.RspInfoField = *pRspInfo;

			m_RefPackageCache.SaveDataTo(BID_RspUserLogin, 0, 0, 0, sizeof(tmp), &tmp);
			LOG_INFO("CQuotSpi::OnRspUserLogin(��¼������Ӧ) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]\n"
				"\t\t\t TradingDay=[%s],\t LoginTime=[%s],\t BrokerID=[%s],\t UserID=[%s],\t SystemName=[%s]\n"
				"\t\t\t FrontID=[%d],\t SessionID=[%#x],\t MaxOrderRef[%s],\t SHFETime=[%s],\t DCETime=[%s],\n"
				"\t\t\t CZCETime[%s],\t FFEXTime=[%s],\t CurTickCount=[%ul]",
				tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
				pRspUserLogin->TradingDay,pRspUserLogin->LoginTime,pRspUserLogin->BrokerID   , pRspUserLogin->UserID,pRspUserLogin->SystemName, 
				pRspUserLogin->FrontID  , pRspUserLogin->SessionID ,pRspUserLogin->MaxOrderRef, pRspUserLogin->SHFETime , pRspUserLogin->DCETime, 
				pRspUserLogin->CZCETime   , pRspUserLogin->FFEXTime,tmp.CurTickCount);
		}
		else
		{
			LOG_INFO("CQuotSpi::OnRspUserLogin(��¼������Ӧ),pRspUserLogin is NULL");
		}

		if(pRspInfo->ErrorID==0)
		{
			m_PlatformParam.QuotStatus=CPlatFormParam::CONNSTATUS_Connected;
			//todo ���ݺ�Լ��������
			std::set<std::string> setMarketDataInstrumentID;
			m_RefCPatsPlatFormService.GetMarketDataInstrumentID(setMarketDataInstrumentID);
			std::set<std::string>::iterator iter=setMarketDataInstrumentID.begin();
			for(iter;iter!=setMarketDataInstrumentID.end();++iter)
			{
				char *pInstrumentID = const_cast<char*>(iter->c_str());
				m_RefMdApi.SubscribeMarketData(&pInstrumentID,1);
			}
		}
		else
		{
			m_PlatformParam.QuotStatus=CPlatFormParam::CONNSTATUS_LoginFailure;
			if(pRspInfo) memcpy(&m_RefCPatsPlatFormService.m_ErrorInfoField,pRspInfo,sizeof(CThostFtdcRspInfoField));
		}

	}

	///�ǳ�������Ӧ
	void CQuotSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		if(!pUserLogout)
		{
			LOG_INFO("CQuotSpi::OnRspUserLogout(�ǳ�������Ӧ),pUserLogout is NULL");
			return;
		}

		DataRspUserLogout tmp;
		tmp.Type       = 1;
		tmp.nRequestID = nRequestID;
		tmp.bIsLast    = bIsLast;
		tmp.UserLogoutField = *pUserLogout;
		if(pRspInfo)
			tmp.RspInfoField = *pRspInfo;

		m_RefPackageCache.SaveDataTo(BID_RspUserLogout, 0, 0, 0, sizeof(tmp), &tmp);
		LOG_INFO("CQuotSpi::OnRspUserLogout(�ǳ�������Ӧ) : ErrCode=[%d] ErrMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t BrokerID=[%s],\t UserID=[%s]",
			tmp.RspInfoField.ErrorID,tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
			pUserLogout->BrokerID,pUserLogout->UserID);
	}

	///����Ӧ��
	void CQuotSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		LOG_INFO("CQuotSpi::OnRspError : ErrCode=[%d] ErrMsg=[%s],RequestID=[%d],IsLast=[%d]",
			pRspInfo->ErrorID,pRspInfo->ErrorMsg,nRequestID,(int)bIsLast);
	}

	///��������Ӧ��
	void CQuotSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		if(!pSpecificInstrument)
		{
			LOG_INFO("CQuotSpi::OnRspSubMarketData(��������Ӧ��,Unimplemented),pSpecificInstrument is NULL");
			return;
		}

		LOG_INFO("CQuotSpi::OnRspSubMarketData(��������Ӧ��,Unimplemented) : ErrorID=[%d],ErrorMsg=[%s],InstrumentID=[%s],nRequestID=[%d],bIsLast=[%d]",
			pRspInfo->ErrorID,pRspInfo->ErrorMsg,
			pSpecificInstrument->InstrumentID,nRequestID,(int)bIsLast);
	}

	///ȡ����������Ӧ��
	void CQuotSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		if(!pSpecificInstrument)
		{
			LOG_INFO("CQuotSpi::OnRspUnSubMarketData(ȡ����������Ӧ��,Unimplemented),pSpecificInstrument is NULL");
			return;
		}

		LOG_INFO("CQuotSpi::OnRspUnSubMarketData(ȡ����������Ӧ��,Unimplemented) : ErrorID=[%d],ErrorMsg=[%s],InstrumentID=[%s],nRequestID=[%d],bIsLast=[%d]",
			pRspInfo->ErrorID,pRspInfo->ErrorMsg,   
			pSpecificInstrument->InstrumentID,nRequestID,(int)bIsLast);
	}

	///�������֪ͨ
	void CQuotSpi::OnRtnDepthMarketData(PlatformStru_DepthMarketData *pDepthMarketData)
	{
		if(!pDepthMarketData)
			return;

		static int Count=0;
		Count++;

		static bool blogID=false;
		if(blogID)
		{
			LOG_INFO("CQuotSpi::OnRtnDepthMarketData:InstrumentID=%s,LastPrice=%g,UpdateTime=%s",
				pDepthMarketData->InstrumentID,pDepthMarketData->LastPrice,pDepthMarketData->UpdateTime);
		}

		bool ret;
		DataRtnDepthMarketData tmp;
		tmp.DepthMarketDataField = *pDepthMarketData;//ԭʼ����
		ret=m_RefPackageCache.SaveDataTo(BID_RtnDepthMarketData, 0, 0, 0, sizeof(tmp), &tmp); 
		if(!ret)
		{
			LOG_INFO("CQuotSpi::OnRtnDepthMarketData,Failed to SaveDataTo m_RefPackageCache:"
				"InstrumentID=%s,LastPrice=%g",
				tmp.DepthMarketDataField.InstrumentID,tmp.DepthMarketDataField.LastPrice);
		}


	}
}

