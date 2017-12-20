#include "stdafx.h"

#include "wx/file.h"
#include "wx/textfile.h"

#include <vector>
#include <string>
#include <map>

#include "PatsApiSvr.h"
#include "PlatFormService.hpp"
#include "PlatformDataMgr.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "../inc/Module-Misc/tools_util.h"
#pragma warning(disable:4996)
#if 1
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
	do{\
	if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CPlatformDataMgr", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif
namespace Pats
{
	CPlatformDataMgr::CPlatformDataMgr(CPlatFormService* pHost):
	m_PlatFormService(pHost)
	{
		InitializeCriticalSection(&m_CS);
		m_pWriteLog=new zqWriteLog(LOGMODE_LOCALFILE,"PlatformDataMgr.log");
	}

	CPlatformDataMgr::~CPlatformDataMgr(void)
	{
		if(m_pWriteLog)
		{
			delete m_pWriteLog;
			m_pWriteLog=NULL;
		}
		DeleteCriticalSection(&m_CS);
	}


	///���ý����ʽ��˻���Ϣ
	void CPlatformDataMgr::SetTradingAccount(const PlatformStru_TradingAccountInfo& inData)
	{
		ASSERT("The method is not implemented in pats.");
	}
	///���ý����ʽ��˻���Ϣ
	void CPlatformDataMgr::SetTradingAccount(const DataRspQryTradingAccount& outData)
	{
		ASSERT("The method is not implemented in pats.");
	}
	int CPlatformDataMgr::GetTradingAccountAvailable(double& fAvailable)
	{
		ASSERT("The method is not implemented in pats.");
		return 0; 
	}
	int CPlatformDataMgr::GetTradingAccountWithdrawQuota(double& fWithdrawQuota)
	{
		ASSERT("The method is not implemented in pats.");
		return 0;
	}
	int CPlatformDataMgr::GetTradingAccountID(char* AccountID)
	{
		ASSERT("The method is not implemented in pats.");
		return 0;  
	}
	///����ָ����Լ��Ϣ
	void CPlatformDataMgr::SetInstrumentInfo(const std::string& InstrumentID, const PlatformStru_InstrumentInfo& inData)
	{
		LOG_INFO("CPlatformDataMgr::SetInstrumentInfo");
		EnterCriticalSection(&m_CS);
		m_allInstruments[InstrumentID] = inData;
		m_allInstrumentList.insert(InstrumentID);
		m_allProductID[inData.ProductID].insert(InstrumentID);
		LeaveCriticalSection(&m_CS);
		LOG_INFO("CPlatformDataMgr::SetInstrumentInfo-end");
	}
	///��ȡָ����Լ��Ϣ
	int CPlatformDataMgr::GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
	{
		LOG_INFO("CPlatformDataMgr::GetInstrumentInfo");
		int ret;
		EnterCriticalSection(&m_CS);
		ret=GetInstrumentInfo_Internal(InstrumentID,outData);
		LeaveCriticalSection(&m_CS);
		LOG_INFO("CPlatformDataMgr::GetInstrumentInfo-end");
		return 0;
	}

	///��ȡָ����Լ��Ϣ���ڲ����ã����ü���
	int CPlatformDataMgr::GetInstrumentInfo_Internal(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
	{
		LOG_INFO("CPlatformDataMgr::GetInstrumentInfo_Internal");
		int ret;
		std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it;
		it =  m_allInstruments.find(InstrumentID);
		memset(&outData,0,sizeof(outData));
		if(it==m_allInstruments.end())
		{
			ret=-1;
		}
		else 
		{
			outData = it->second;
			ret=0;
		}
		LOG_INFO("CPlatformDataMgr::GetInstrumentInfo_Internal-end");
		return ret;
	}

	///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
	char CPlatformDataMgr::GetProductClassType(const std::string& InstrumentID)
	{
		//LOG_INFO("CPlatformDataMgr::GetProductClassType");
		char ProductClassType;
		std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it;
		EnterCriticalSection(&m_CS);
		it =  m_allInstruments.find(InstrumentID);
		if(it==m_allInstruments.end())
			ProductClassType=-1;
		else 
			ProductClassType = it->second.ProductClass;
		LeaveCriticalSection(&m_CS);
		//LOG_INFO("CPlatformDataMgr::GetProductClassType-end");
		return ProductClassType;
	}
	///��ȡ��Լ�б�
	int CPlatformDataMgr::GetInstrumentList(std::vector<GroupInfo> &outData)
	{
		if(m_allGroupInfo.empty())
		{
			std::map<std::string,int> GroupName2ID;
			std::map<std::string,int>::iterator GroupName2ID_it;
			PlatformStru_InstrumentInfo instInfo;
			std::set<std::string>::iterator it=m_allInstrumentList.begin();
			for(it;it!=m_allInstrumentList.end();++it)
			{
				if(GetInstrumentInfo_Internal(*it,instInfo)==0)
				{
					GroupName2ID_it=GroupName2ID.find(instInfo.ExchangeID);
					if(GroupName2ID_it==GroupName2ID.end())
					{
						GroupInfo ginfo;
						ginfo.GroupName=instInfo.ExchangeID;
						ginfo.InstrVec.push_back(InstrumentInfo(instInfo.InstrumentID,instInfo.InstrumentName));
						m_allGroupInfo.push_back(ginfo);
						GroupName2ID[instInfo.ExchangeID]=m_allGroupInfo.size()-1;
					}
					else
					{
						m_allGroupInfo[GroupName2ID_it->second].InstrVec.push_back(InstrumentInfo(instInfo.InstrumentID,instInfo.InstrumentName));
					}
				}
			}
		}
		outData=m_allGroupInfo;
		return 0;
	}
	///��ȡ��Լ�б�
	int CPlatformDataMgr::GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID)
	{
		LOG_INFO("CPlatformDataMgr::GetInstrumentList");
		EnterCriticalSection(&m_CS);
		if(ExchangeID.empty())
			outData = m_allInstrumentList;
		else
		{
			outData.clear();
			std::set<std::string>::iterator it=m_allInstrumentList.begin();
			PlatformStru_InstrumentInfo instInfo;
			for(it;it!=m_allInstrumentList.end();++it)
			{
				if(GetInstrumentInfo_Internal(*it,instInfo)==0&&ExchangeID==instInfo.ExchangeID)
				{
					outData.insert(*it);
				}
			}
		}
		LeaveCriticalSection(&m_CS);
		LOG_INFO("CPlatformDataMgr::GetInstrumentList-end");
		return outData.size();
	}
	//���������Լ�б�
	int CPlatformDataMgr::AddMainInstrumentList(std::string instrument)
	{
		ASSERT("The method is not implemented in pats.");
		return 0;
	}
	///��ȡ��Լ�б�
	int CPlatformDataMgr::GetMainInstrumentList(std::set<std::string> &outData)
	{
		LOG_INFO("CPlatformDataMgr::GetMainInstrumentList");
		EnterCriticalSection(&m_CS);
		outData=m_allInstrumentList;
		LeaveCriticalSection(&m_CS);
		LOG_INFO("CPlatformDataMgr::GetMainInstrumentList-end");
		return outData.size();
	}
	///���ú�Լ�ݲ��б�
	void CPlatformDataMgr::SetInstrumentVarietyMap(map<string, string>& inData)
	{
		ASSERT("The method is not implemented in pats.");
	}

	// ��ȡ��Լ�ݲ��б�
	int CPlatformDataMgr::GetInstrumentVarietyMap(map<string, string>& outData)
	{
		outData.clear();
		return 0;
	}

	///��ȡָ��Ʒ�ֵĺ�Լ�б�,���غ�Լ����
	int CPlatformDataMgr::GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData)
	{
		LOG_INFO("CPlatformDataMgr::GetInstrumentListByProductID");
		int ret;
		std::map<std::string, std::set<std::string> >::iterator it;
		EnterCriticalSection(&m_CS);
		it=m_allProductID.find(ProductID);
		if(it==m_allProductID.end()) 
			ret=-1;
		else
		{
			outData = it->second;
			ret=outData.size();
		}
		LeaveCriticalSection(&m_CS);
		LOG_INFO("CPlatformDataMgr::GetInstrumentListByProductID-end");
		return ret;
	}

	///��ȡ��ԼƷ�ִ���
	int CPlatformDataMgr::GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID)
	{
		LOG_INFO("CPlatformDataMgr::GetProductID");
		EnterCriticalSection(&m_CS);
		outData=m_allProductID;
		if(!ExchangeID.empty())
		{
			PlatformStru_InstrumentInfo instInfo;
			std::map<std::string, std::set<std::string> >::iterator it;
			for(it=outData.begin();it!=outData.end();)
			{
				if(it->second.size()==0||
					GetInstrumentInfo_Internal(*(it->second.begin()),instInfo)!=0||
					ExchangeID!=instInfo.ExchangeID)
				{
					outData.erase(it++);
				}
				else
					++it;
			}
		}
		LeaveCriticalSection(&m_CS);
		return outData.size(); 
		LOG_INFO("CPlatformDataMgr::GetProductID-end");
	}


	///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
	int CPlatformDataMgr::GetInstrumentMultiple(const std::string& InstrumentID)
	{
		LOG_INFO("CPlatformDataMgr::GetInstrumentMultiple");
		int Multiple;
		std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it;
		EnterCriticalSection(&m_CS);
		it =  m_allInstruments.find(InstrumentID);
		if(it==m_allInstruments.end())
			Multiple=-1;
		else 
			Multiple= it->second.VolumeMultiple;
		LeaveCriticalSection(&m_CS);
		LOG_INFO("CPlatformDataMgr::GetInstrumentMultiple-end");
		return Multiple;
	}

	///��ȡ��Լ��������
	int CPlatformDataMgr::GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
	{
		LOG_INFO("CPlatformDataMgr::GetCommissionRate");
		int ret;
		EnterCriticalSection(&m_CS);
		ret=GetCommissionRate_Internal(InstrumentID,outData);
		LeaveCriticalSection(&m_CS);
		LOG_INFO("CPlatformDataMgr::GetCommissionRate-end");
		return ret;
	}

	///��ȡ��Լ�������ʣ��ڲ����ã����ü���
	int CPlatformDataMgr::GetCommissionRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
	{
		LOG_INFO("CPlatformDataMgr::GetCommissionRate_Internal");
		int ret=0;
		ZeroMemory(&outData,sizeof(PlatformStru_InstrumentCommissionRate));
		strcpy(outData.InstrumentID,InstrumentID.c_str());
		LOG_INFO("CPlatformDataMgr::GetCommissionRate_Internal-end");
		return ret;
	}

	///���ú�Լ��������
	void CPlatformDataMgr::SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
	{
		ASSERT("The method is not implemented in pats.");
	}

	///�������ڲ�ѯ�������ʵĺ�Լ,���ڲ�ѯ����ʱ��Ӧ
	void CPlatformDataMgr::SetReqCommissionRateInstrumentID(const std::string& InstrumentID)
	{
		ASSERT("The method is not implemented in pats.");
	}

	///��ȡ���ڲ�ѯ�������ʵĺ�Լ
	void CPlatformDataMgr::GetReqCommissionRateInstrumentID(std::string& outData)
	{
		ASSERT("The method is not implemented in pats.");
	}

	///��ȡ��Լ��֤����
	int CPlatformDataMgr::GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
	{
		LOG_INFO("CPlatformDataMgr::GetMarginRate");
		int ret;
		EnterCriticalSection(&m_CS);
		ret=GetMarginRate_Internal(InstrumentID,outData);
		LeaveCriticalSection(&m_CS);
		LOG_INFO("CPlatformDataMgr::GetMarginRate-end");
		return ret; 
	}

	///��ȡ��Լ��֤���ʣ��ڲ����ã����ü���
	int CPlatformDataMgr::GetMarginRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
	{
		LOG_INFO("CPlatformDataMgr::GetMarginRate_Internal");
		int ret=0;
		ZeroMemory(&outData,sizeof(PlatformStru_InstrumentMarginRate));
		strcpy(outData.InstrumentID,InstrumentID.c_str());
		LOG_INFO("CPlatformDataMgr::GetMarginRate_Internal-end");
		return ret; 
	}

	///���ú�Լ��֤����
	void CPlatformDataMgr::SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
	{
		ASSERT("The method is not implemented in pats.");
	}



	//ԭʼ�ĳɽ��ر�����
	void CPlatformDataMgr::QryTradeReach( const PlatformStru_TradeInfo& TradeInfo,const int ErrID,const bool bIsLast)
	{

		LOG_INFO("CPlatformDataMgr::QryTradeReach");

		EnterCriticalSection(&m_CS);
		m_TradeData.QryRltReach_Internal(TradeInfo,ErrID,bIsLast);
		LeaveCriticalSection(&m_CS);


		LOG_INFO("CPlatformDataMgr::QryTradeReach-end");
	}

	//�µĳɽ��ر����������ɽ����ŵ�m_TradesListWaitForAppend�У�Ȼ�����ProcessNewTradeList����
	void CPlatformDataMgr::NewTradeReady( PlatformStru_TradeInfo& TradeInfo,const PlatformStru_InstrumentInfo& InstrumentInfo)
	{
		LOG_INFO("CPlatformDataMgr::NewTradeReady");

		Lock_CPlatformDataMgr;
		m_TradeData.NewReach_Internal(TradeInfo);
		Unlock_CPlatformDataMgr;
		LOG_INFO("CPlatformDataMgr::NewTradeReady-end");
	}
	bool CPlatformDataMgr::GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData)
	{
		bool brlt;
		EnterCriticalSection(&m_CS);
		PlatformStru_TradeInfo *pTrade;
		if(m_TradeData.GetValue_Internal(pTrade,tradekey))
		{
			outData=*pTrade;
			brlt=true;
		}
		else
		{
			brlt=false;
		}
		LeaveCriticalSection(&m_CS);
		return brlt;
	}
	///��ȡȫ���ɽ���
	int CPlatformDataMgr::GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData)
	{
		int irlt;
		Lock_CPlatformDataMgr;

		irlt=m_TradeData.GetAllAboutInstrument2_Internal(strInstrument,outData);

		Unlock_CPlatformDataMgr;

		return irlt;
	}
	int CPlatformDataMgr::GetAllOrderInfo(long lFTID, PlatformStru_OrderInfo& outData)
	{
		int nRet = 0;
		Lock_CPlatformDataMgr;
		std::map<long, PlatformStru_OrderInfo*>::iterator it_FTID = m_OrderData.m_all2.find(lFTID);
		if ( it_FTID != m_OrderData.m_all2.end())
		{
			outData = *(it_FTID->second);
		}
		else
		{
			nRet = -1;
		}
		Unlock_CPlatformDataMgr;

		return nRet;
	}
	int CPlatformDataMgr::GetAllOrderInfo(OrderKey key, PlatformStru_OrderInfo& outData)
	{
		int nRet = 0;
		Lock_CPlatformDataMgr;
		std::map<OrderKey, PlatformStru_OrderInfo*>::iterator it_all = m_OrderData.m_all.find(key);
		if ( it_all != m_OrderData.m_all.end())
		{
			outData = *(it_all->second);
		}
		else
		{
			nRet = -1;
		}
		Unlock_CPlatformDataMgr;

		return nRet;
	}
	///��ȡȫ���ɽ���
	int CPlatformDataMgr::GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData)
	{
		int irlt;
		EnterCriticalSection(&m_CS);
		irlt=m_TradeData.GetAll2_Internal(outData);
		LeaveCriticalSection(&m_CS);
		return irlt;
	}
	//�ӳɽ���ȡ�����ѡ����û�з��ʣ���0����
	void CPlatformDataMgr::GetAllCommission_Internal(double& TotalCommission)
	{
		ASSERT("The method is not implemented in pats.");
	}
	//ԭʼ����������������ѯ��Ӧʱ����
	void CPlatformDataMgr::QryOrderReach(const PlatformStru_OrderInfo& OrderInfo,const int ErrID,const bool bIsLast)
	{
		LOG_INFO("CPlatformDataMgr::QryOrderReach");

		std::set<string> setInstruments;

		Lock_CPlatformDataMgr;

		m_OrderData.QryRltReach_Internal(OrderInfo,ErrID,bIsLast);
		Unlock_CPlatformDataMgr;
		LOG_INFO("CPlatformDataMgr::QryOrderReach-end");
	}
	//�±�������,ע�⣬������ԭ�б�������״̬
	void CPlatformDataMgr::NewOrderReady(PlatformStru_OrderInfo& OrderInfo, 
		PlatformStru_OrderInfo& OldOrderInfo,
		const PlatformStru_InstrumentInfo& InstrumentInfo)
	{
		LOG_INFO("CPlatformDataMgr::NewOrderReady");

		PlatformStru_OrderInfo& RltOldOrderInfo=PlatformStru_OrderInfo::GetInvalidRecord();

		Lock_CPlatformDataMgr;

		PlatformStru_OrderInfo *pOrder;

		OrderKey OldOrderKey(OrderInfo);
		if(m_OrderData.GetValue_Internal(pOrder,OldOrderKey))
			OldOrderInfo=*pOrder;
		else OldOrderInfo=PlatformStru_OrderInfo::GetInvalidRecord();

		//CalcuOrderFreezeMargin_Internal(OrderInfo,InstrumentInfo);
		//CalcuOrderFreezeCommission_Internal(OrderInfo,InstrumentInfo);

		m_OrderData.NewReach_Internal(OrderInfo);

		Unlock_CPlatformDataMgr;

		LOG_INFO("CPlatformDataMgr::NewOrderReady-end");
	}

	//�������б����еĶ��ᱣ֤�𣬷��غ�Լ����Ƿ��б����������Ƿ���Ҫ�����ʽ���塣��ѯ��֤������Ӧʱ����
	bool CPlatformDataMgr::UpdateOrderFreezeMargin_RspQryMarginRate(std::string&	strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo)
	{
		ASSERT("The method is not implemented in pats.");
		return false;
	}

	//�������б����еĶ��������ѣ����غ�Լ����Ƿ��б����������Ƿ���Ҫ�����ʽ���塣��ѯ����������Ӧʱ����
	bool CPlatformDataMgr::UpdateOrderFreezeCommission_RspQryCommissionRate(std::string& strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo)
	{
		ASSERT("The method is not implemented in pats.");
		return false;
	}

	//���㱨����Ϣ�еĶ��ᱣ֤��
	void CPlatformDataMgr::CalcuOrderFreezeMargin_Internal(PlatformStru_OrderInfo& OrderInfo,
		const PlatformStru_InstrumentInfo& InstrumentInfo)
	{
		ASSERT("The method is not implemented in pats.");
	}

	//���㱨����Ϣ�еĶ���������
	void CPlatformDataMgr::CalcuOrderFreezeCommission_Internal(PlatformStru_OrderInfo& OrderInfo,
		const PlatformStru_InstrumentInfo& InstrumentInfo)
	{
		ASSERT("The method is not implemented in pats.");
	}
	int CPlatformDataMgr::GetAllOrderFTIDList(std::vector<long>& vec)
	{
		vec.clear();
		Lock_CPlatformDataMgr;
		std::set<long>::iterator it_FTID = m_OrderData.m_allFTID.begin();
		for ( ; it_FTID != m_OrderData.m_allFTID.end(); ++it_FTID )
		{
			vec.push_back(*it_FTID);
		}
		Unlock_CPlatformDataMgr;

		return vec.size();
	}
	///��ȡȫ������
	int CPlatformDataMgr::GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData)
	{
		int irlt;

		Lock_CPlatformDataMgr;

		irlt=m_OrderData.GetAll2_Internal(outData);

		Unlock_CPlatformDataMgr;

		return irlt;
	}
	int CPlatformDataMgr::GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
	{
		int irlt;
		EnterCriticalSection(&m_CS);
		irlt=m_OrderData.GetAllAboutInstrument2_Internal(strInstrument,outData);
		LeaveCriticalSection(&m_CS);
		return irlt;
	}
	///��ȡָ������
	bool CPlatformDataMgr::GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData)
	{
		bool brlt;

		Lock_CPlatformDataMgr;

		PlatformStru_OrderInfo *pOrder;
		if(m_OrderData.GetValue_Internal(pOrder,inOrderKey))
		{
			outData=*pOrder;
			brlt=true;
		}
		else
		{
			outData=PlatformStru_OrderInfo::GetInvalidRecord();
			brlt=false;
		}

		Unlock_CPlatformDataMgr;

		return brlt;
	}
	///��ȡȫ���Ѵ����ı���
	int CPlatformDataMgr::GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData)
	{
		Lock_CPlatformDataMgr;

		std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it;
		outData.clear();
		for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
		{
			if(it->second==NULL) continue;
			if( it->second->ContingentCondition == THOST_FTDC_CC_Immediately ||
				it->second->ContingentCondition == THOST_FTDC_CC_Touch ||
				it->second->ContingentCondition == THOST_FTDC_CC_TouchProfit)
			{
				outData.push_back(*(it->second));
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}

	///��ȡ��Լ��ص��Ѵ����ı���
	int CPlatformDataMgr::GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
	{
		Lock_CPlatformDataMgr;

		outData.clear();

		std::set<long>* pFTIDSet;
		std::set<long>::iterator it;
		PlatformStru_OrderInfo *pOrderInfo;
		long FTID;

		if(m_OrderData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
		{
			for(it=pFTIDSet->begin();it!=pFTIDSet->end();it++)
			{
				FTID=*it;
				if(m_OrderData.GetValue2_Internal(pOrderInfo,FTID)&&
					(pOrderInfo->ContingentCondition == THOST_FTDC_CC_Immediately ||
					pOrderInfo->ContingentCondition == THOST_FTDC_CC_Touch ||
					pOrderInfo->ContingentCondition == THOST_FTDC_CC_TouchProfit))
				{
					outData.push_back(*pOrderInfo);
				}
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}

	///��ȡָ���Ѵ����ı���
	bool CPlatformDataMgr::GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
	{
		bool brlt=false;

		Lock_CPlatformDataMgr;

		std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it=m_OrderData.m_all.find(orderkey);
		if(it!=m_OrderData.m_all.end()&&
			it->second!=NULL&&
			(   it->second->ContingentCondition == THOST_FTDC_CC_Immediately ||
			it->second->ContingentCondition == THOST_FTDC_CC_Touch ||
			it->second->ContingentCondition == THOST_FTDC_CC_TouchProfit))
		{
			outData=*(it->second);
			brlt=true;
		}

		Unlock_CPlatformDataMgr;

		return brlt;
	}

	///��ȡδ�ɽ�����
	int CPlatformDataMgr::GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData)
	{
		Lock_CPlatformDataMgr;

		std::map<OrderKey,  PlatformStru_OrderInfo*>::iterator it;
		outData.clear();
		for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
		{
			if(it->second==NULL) continue;
			if( it->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
				it->second->OrderStatus==THOST_FTDC_OST_NoTradeQueueing)
			{
				outData.push_back(*(it->second));
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}
	///��ȡδ�ɽ�����
	int CPlatformDataMgr::GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
	{
		Lock_CPlatformDataMgr;

		outData.clear();

		std::set<long>* pFTIDSet;
		std::set<long>::iterator it;
		PlatformStru_OrderInfo *pOrderInfo;
		long FTID;

		if(m_OrderData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
		{
			for(it=pFTIDSet->begin();it!=pFTIDSet->end();it++)
			{
				FTID=*it;
				if(m_OrderData.GetValue2_Internal(pOrderInfo,FTID)&&
					(pOrderInfo->ContingentCondition == THOST_FTDC_OST_PartTradedQueueing ||
					pOrderInfo->ContingentCondition == THOST_FTDC_OST_NoTradeQueueing))
				{
					outData.push_back(*pOrderInfo);
				}
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}

	///��ȡָ����δ�ɽ�����
	bool CPlatformDataMgr::GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
	{
		bool brlt=false;

		Lock_CPlatformDataMgr;

		std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it=m_OrderData.m_all.find(orderkey);
		if(it!=m_OrderData.m_all.end()&&
			it->second!=NULL&&
			(   it->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
			it->second->OrderStatus==THOST_FTDC_OST_NoTradeQueueing))
		{
			outData=*(it->second);
			brlt=true;
		}

		Unlock_CPlatformDataMgr;

		return brlt;
	}
	///��ȡ�ѳɽ�����
	int CPlatformDataMgr::GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData)
	{
		Lock_CPlatformDataMgr;

		std::map<OrderKey,  PlatformStru_OrderInfo*>::iterator it;
		outData.clear();
		for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
		{
			if(it->second==NULL) continue;
			if( it->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
				it->second->OrderStatus==THOST_FTDC_OST_AllTraded)
			{
				outData.push_back(*(it->second));
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}
	///��ȡ�ѳɽ�����
	int CPlatformDataMgr::GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
	{
		Lock_CPlatformDataMgr;

		outData.clear();

		std::set<long>* pFTIDSet;
		std::set<long>::iterator it;
		PlatformStru_OrderInfo *pOrderInfo;
		long FTID;

		if(m_OrderData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
		{
			for(it=pFTIDSet->begin();it!=pFTIDSet->end();it++)
			{
				FTID=*it;
				if(m_OrderData.GetValue2_Internal(pOrderInfo,FTID)&&
					(pOrderInfo->ContingentCondition == THOST_FTDC_OST_PartTradedQueueing ||
					pOrderInfo->ContingentCondition == THOST_FTDC_OST_AllTraded))
				{
					outData.push_back(*pOrderInfo);
				}
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}
	///��ȡָ���ѳɽ�����
	bool CPlatformDataMgr::GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
	{
		bool brlt=false;

		Lock_CPlatformDataMgr;

		std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it=m_OrderData.m_all.find(orderkey);
		if(it!=m_OrderData.m_all.end()&&
			it->second!=NULL&&
			(   it->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
			it->second->OrderStatus==THOST_FTDC_OST_AllTraded))
		{
			outData=*(it->second);
			brlt=true;
		}

		Unlock_CPlatformDataMgr;

		return brlt;
	}
	///��ȡ�ѳ����ʹ��󱨵�
	int CPlatformDataMgr::GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData)
	{
		Lock_CPlatformDataMgr;

		std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it;
		outData.clear();
		for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
		{
			if(it->second==NULL) continue;
			if( it->second->OrderStatus==THOST_FTDC_OST_Canceled||
				it->second->OrderStatus==THOST_FTDC_OST_ERROE)
			{
				outData.push_back(*(it->second));
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}
	///��ȡ�ѳ����ʹ��󱨵�
	int CPlatformDataMgr::GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
	{
		Lock_CPlatformDataMgr;

		outData.clear();

		std::set<long>* pFTIDSet;
		std::set<long>::iterator it;
		PlatformStru_OrderInfo *pOrderInfo;
		long FTID;

		if(m_OrderData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
		{
			for(it=pFTIDSet->begin();it!=pFTIDSet->end();it++)
			{
				FTID=*it;
				if(m_OrderData.GetValue2_Internal(pOrderInfo,FTID)&&
					(pOrderInfo->ContingentCondition == THOST_FTDC_OST_Canceled ||
					pOrderInfo->ContingentCondition == THOST_FTDC_OST_ERROE))
				{
					outData.push_back(*pOrderInfo);
				}
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}
	///��ȡָ���ѳ����ʹ��󱨵�
	bool CPlatformDataMgr::GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
	{
		bool brlt=false;

		Lock_CPlatformDataMgr;

		std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it=m_OrderData.m_all.find(orderkey);
		if(it!=m_OrderData.m_all.end()&&
			it->second!=NULL&&
			(   it->second->OrderStatus==THOST_FTDC_OST_Canceled||
			it->second->OrderStatus==THOST_FTDC_OST_ERROE))
		{
			outData=*(it->second);
			brlt=true;
		}

		Unlock_CPlatformDataMgr;

		return brlt;
	}

	//��ȡָ����Լ��ָ������ָ����ƽ�Ĺҵ�������������
	int  CPlatformDataMgr::GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, const char CloseType, const char HedgeFlag)
	{
		int tnVolume = 0;
		std::map<long,  PlatformStru_OrderInfo*>::iterator iter;
		std::set<long>::iterator itFTID;
		std::map<std::string,std::set<long>>::iterator it_Index2;

		if(strInstrumentID.empty())
			return 0;

		Lock_CPlatformDataMgr;

		it_Index2=m_OrderData.m_allIndex2.find(strInstrumentID);
		if(it_Index2!=m_OrderData.m_allIndex2.end()&&!it_Index2->second.empty())
		{
			for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
			{
				iter=m_OrderData.m_all2.find(*itFTID);
				if(iter!=m_OrderData.m_all2.end()&&
					iter->second!=NULL&&
					iter->second->Direction == Direction &&
					iter->second->CombOffsetFlag[0] == CloseType&&
					iter->second->CombHedgeFlag[0]==HedgeFlag&&
					(iter->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
					iter->second->OrderStatus==THOST_FTDC_OST_NoTradeQueueing)&&
					strAccount==std::string(iter->second->szAccount))
					tnVolume += iter->second->VolumeTotal;
			}
		}

		Unlock_CPlatformDataMgr;

		return tnVolume;
	}



	//��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ��
	//�ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
	int  CPlatformDataMgr::GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
	{
		LOG_INFO("CPlatformDataMgr::GetCloseOrderVolume");
		int ret;

		Lock_CPlatformDataMgr;

		ret=GetCloseOrderVolume_Internal(strAccount,strInstrumentID,Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,HedgeFlag);

		Unlock_CPlatformDataMgr;

		LOG_INFO("CPlatformDataMgr::GetCloseOrderVolume-end");
		return ret;
	}

	//��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ڲ����ã�������
	//�ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
	int  CPlatformDataMgr::GetCloseOrderVolume_Internal(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
	{
		CloseVolume=CloseTodayVolume=CloseYesterdayVolume=0;

		std::map<std::string,std::set<long>>::iterator it_Index2;
		std::set<long>::iterator itFTID;
		std::map<long,  PlatformStru_OrderInfo*>::iterator iter;

		if(strInstrumentID.empty()) return -1;

		it_Index2=m_OrderData.m_allIndex2.find(strInstrumentID);
		if(it_Index2!=m_OrderData.m_allIndex2.end()&&!it_Index2->second.empty())
		{
			for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
			{
				iter=m_OrderData.m_all2.find(*itFTID);
				if(iter!=m_OrderData.m_all2.end()&&
					iter->second!=NULL&&
					iter->second->Direction == Direction &&
					iter->second->CombHedgeFlag[0]==HedgeFlag&&
					(iter->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
					iter->second->OrderStatus==THOST_FTDC_OST_NoTradeQueueing)&&
					strAccount==std::string(iter->second->szAccount))
				{
					if(iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_Close||
						iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
						iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
						CloseVolume += iter->second->VolumeTotal;
					if(iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
						CloseTodayVolume += iter->second->VolumeTotal;
					if(iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
						CloseYesterdayVolume += iter->second->VolumeTotal;
				}
			}
		}
		return 0;
	}
	int CPlatformDataMgr::GetPositions(std::vector<PlatformStru_Position>& outData)
	{
		outData.clear();

		Lock_CPlatformDataMgr;

		std::map<PositionKey,PlatformStru_Position*>::iterator it = m_PositionData.m_all.begin();
		for ( ; it != m_PositionData.m_all.end(); ++it )
		{
			if ( it->second!=NULL &&
				it->second->Position > 0 )
			{
				outData.push_back(*(it->second));
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}
	int CPlatformDataMgr::GetPositions3(const std::string& strInstrument,std::vector<PlatformStru_Position>& outData,long& lastseq)
	{
		int rlt;

		outData.clear();

		Lock_CPlatformDataMgr;

		if(lastseq==m_PositionData.GetUpdateSeq())
		{
			rlt=1;
		}
		else
		{
			std::set<long>* pFTIDSet;
			if(m_PositionData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
			{
				PlatformStru_Position *pData;
				std::set<long>::iterator itFTID;
				for(itFTID=pFTIDSet->begin();itFTID!=pFTIDSet->end();itFTID++)
				{
					if(m_PositionData.GetValue2_Internal(pData,(*itFTID))&&
						pData->Position>0)
					{
						outData.push_back(*pData);
					}
				}
			}

			lastseq=m_PositionData.GetUpdateSeq();
			rlt=0;
		}

		Unlock_CPlatformDataMgr;

		return rlt;
	}
	int CPlatformDataMgr::GetPositions2(const std::string& strInstrument,std::set<long>& setFTID,std::vector<PlatformStru_Position>& outData,long& lastseq)
	{
		int rlt;

		outData.clear();

		Lock_CPlatformDataMgr;

		if(lastseq==m_PositionData.GetUpdateSeq())
		{
			rlt=1;
		}
		else
		{
			std::set<long>* pFTIDSet=NULL;
			if(m_PositionData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
			{
				setFTID=(*pFTIDSet);

				PlatformStru_Position *pData=NULL;
				std::set<long>::iterator itFTID;
				for(itFTID=pFTIDSet->begin();itFTID!=pFTIDSet->end();itFTID++)
				{
					if(m_PositionData.GetValue2_Internal(pData,(*itFTID))&&
						pData->Position>0)
					{
						outData.push_back(*pData);
					}
					else
					{
						setFTID.erase(*itFTID);
					}
				}
			}
			lastseq=m_PositionData.GetUpdateSeq();
			rlt=0;
		}

		Unlock_CPlatformDataMgr;

		return rlt;
	}
	//��ѯ�ĳֲּ�¼����
	void CPlatformDataMgr::QryPositionReach(PlatformStru_Position& PositionInfo,const int ErrID,const bool bIsLast)
	{
		LOG_INFO("CPlatformDataMgr::QryPositionReach:InstrumentID=%s",PositionInfo.InstrumentID)
			Lock_CPlatformDataMgr;
		m_PositionData.QryRltReach_Internal(PositionInfo,ErrID,bIsLast);
		Unlock_CPlatformDataMgr;

		LOG_INFO("CPlatformDataMgr::QryPositionReach-end");
	}
	void CPlatformDataMgr::UpdatePosition_RtnTrade()
	{
		LOG_INFO("CPlatformDataMgr::QryPositionReach:InstrumentID=%s",PositionInfo.InstrumentID)
			Lock_CPlatformDataMgr;
		std::vector<PlatformStru_Position> vecPosition;
		m_PlatFormService->GetAllPosition(vecPosition);
		//
		std::set<PositionKey> newset;
		for (size_t i=0;i<vecPosition.size();++i)
		{
			PositionKey thisKey(vecPosition[i]);
			newset.insert(thisKey);
		}
		std::map<PositionKey,PlatformStru_Position*>::iterator it;
		for(it=m_PositionData.m_all.begin();it!=m_PositionData.m_all.end();++it)
		{
			if(newset.find(it->first)==newset.end())
			{
				//һ��������һ���ֲּ�¼
				m_PositionData.DeleteRecord2_Internal(it->first);
				break;
			}
		}
		for (size_t i=0;i<vecPosition.size();++i)
		{
			m_PositionData.UpdateRecord_Internal(vecPosition[i]);
		}
		m_PositionData.NewUpdateSeq();
		Unlock_CPlatformDataMgr;

		LOG_INFO("CPlatformDataMgr::QryPositionReach-end");
	}
	bool CPlatformDataMgr::IsTodayPositionDetail(const PlatformStru_PositionDetail& PositionDetail)
	{
		ASSERT("The method is not implemented in pats.");
		return false;
	}
	bool CPlatformDataMgr::IsTodayPosition(const char *OpenDate)
	{
		ASSERT("The method is not implemented in pats.");
		return false;
	}


	//��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
	int CPlatformDataMgr::GetPositionKeySet(std::set<PositionKey> &PositionKeySet)
	{
		LOG_INFO("CPlatformDataMgr::GetPositionKeySet");
		PositionKeySet.clear();

		std::map<PositionKey,PlatformStru_Position*>::iterator it;

		EnterCriticalSection(&m_CS);

		it=m_PositionData.m_all.begin();
		for(;it!=m_PositionData.m_all.end();it++)
		{
			if(it->second->Position>0)
			{
				PositionKey tmpKey(it->first);
				PositionKeySet.insert(tmpKey);
			}
		}

		LeaveCriticalSection(&m_CS);

		LOG_INFO("CPlatformDataMgr::GetPositionKeySet-end");
		return PositionKeySet.size();
	}


	//��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
	int CPlatformDataMgr::GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData)
	{
		LOG_INFO("CPlatformDataMgr::GetPositionData");
		EnterCriticalSection(&m_CS);

		int bRet = 0;
		PositionKey posKey(strAccount.c_str(), InstrumentID.c_str(), Direction, HedgeFlag);
		std::map<PositionKey, PlatformStru_Position*>::iterator it_pos = m_PositionData.m_all.find(posKey);
		if(it_pos != m_PositionData.m_all.end())
		{
			PositionData = *(it_pos->second);
			bRet = 0;
		}
		else 
		{
			bRet = -1;
		}
		LeaveCriticalSection(&m_CS);

		LOG_INFO("CPlatformDataMgr::GetPositionData-end");
		return bRet;
	}

	//��ȡָ����Լ���飬�ɹ�����0��ʧ�ܷ���-1
	int CPlatformDataMgr::GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
	{
		int ret;

		Lock_CPlatformDataMgr;

		ret=GetQuotInfo_Internal(InstrumentID,outData);

		Unlock_CPlatformDataMgr;

		return ret;
	}

	//��ȡָ����Լ���飬�ɹ�����0��ʧ�ܷ���-1
	int CPlatformDataMgr::GetQuotInfo_Internal(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
	{
		int ret;

		std::map<std::string,  PlatformStru_DepthMarketData>::iterator it =  m_lastQuot.find(InstrumentID);
		if(it==m_lastQuot.end())
		{
			memset(&outData,0,sizeof(outData));
			ret=-1;
		}
		else
		{
			outData = it->second;
			ret=0;
		}

		return ret;
	}

	//����ָ����Լ���飬���õ�������£��򷵻�true�����򷵻�false
	//������Ϻ�Լ���������
	bool CPlatformDataMgr::SetQuotInfo(const PlatformStru_DepthMarketData& inData)
	{
		std::map<std::string,  PlatformStru_DepthMarketData>::iterator it = m_lastQuot.find(std::string(inData.InstrumentID));
		bool toBeContinue=true;

		EnterCriticalSection(&m_CS);

		if(it==m_lastQuot.end())
		{
			m_lastQuot[std::string(inData.InstrumentID)] = inData;
		}
		else
		{
			PlatformStru_DepthMarketData& OldBusiness= it->second;
			//�Ƚ������ʱ�䣬����������
			if (memcmp(inData.UpdateTime, OldBusiness.UpdateTime, sizeof(OldBusiness.UpdateTime))<0)
				toBeContinue=false;
			if(toBeContinue)
			{
				m_OldQuot[std::string(inData.InstrumentID)]=m_lastQuot[std::string(inData.InstrumentID)];
				m_lastQuot[std::string(inData.InstrumentID)] = inData;
			}
			else
			{
				LOG_INFO("CDataCenter::doProcessBusinessData:Old BID_RtnDepthMarketData InstrumentID=[%s],OldUpdateTime=[%s],NewUpdateTime=[%s]",
					inData.InstrumentID,
					OldBusiness.UpdateTime,
					inData.UpdateTime);
			}
		}
		LeaveCriticalSection(&m_CS);

		return toBeContinue;

	}
	void CPlatformDataMgr::ConvertComboInstrumentID(const std::string strComboID, std::string& strInstrumentID1, std::string& strInstrumentID2)
	{
		char strTemp[128];
		char strCode1[128];
		char strCode2[128];
		ZeroMemory(strCode1, 128);
		ZeroMemory(strCode2, 128);
		sscanf_s(strComboID.c_str(), "%s %[^&]&%s", strTemp, sizeof(strTemp)-1, strCode1, sizeof(strCode1)-1, strCode2, sizeof(strCode2)-1);
		strInstrumentID1 = strCode1;
		strInstrumentID2 = strCode2;
	}
	//��ȡָ����Լ�����¼�
	double CPlatformDataMgr::GetCurPrice (const std::string& InstrumentID)
	{
		double ret=0.0;
		//��Ϻ�Լ
		char clstype=GetProductClassType(InstrumentID);
		if(clstype==THOST_FTDC_PC_Combination)
		{
			std::string id1,id2;
			ConvertComboInstrumentID(InstrumentID,id1,id2);
			double prc1=GetCurPrice(id1);
			double prc2=GetCurPrice(id2);
			ret= prc1-prc2;
		}
		else if(clstype==THOST_FTDC_PC_Futures)
		{
			EnterCriticalSection(&m_CS);
			std::map<std::string,  PlatformStru_DepthMarketData>::iterator it =  m_lastQuot.find(InstrumentID);
			if(it!=m_lastQuot.end())
			{
				if(it->second.Volume>0) ret= it->second.LastPrice;
				else ret= it->second.PreSettlementPrice;
			}
			LeaveCriticalSection(&m_CS);
		}
		return ret;
	}

	//��ȡָ����Լ�����飬�ɹ�����0��ʧ�ܷ���-1
	int CPlatformDataMgr::GetOldQuotInfo (const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
	{
		int ret;

		EnterCriticalSection(&m_CS);

		std::map<std::string,  PlatformStru_DepthMarketData>::iterator it =  m_OldQuot.find(InstrumentID);
		if(it==m_OldQuot.end())
		{
			memset(&outData,0,sizeof(outData));
			ret=-1;
		}
		else
		{
			outData = it->second;
			ret=0;
		}

		LeaveCriticalSection(&m_CS);

		return ret;
	}
	string CPlatformDataMgr::GetTradingDay(const string& ExchangeID)
	{
		ASSERT("The method is not implemented in pats.");
		return string("");
	}

	//��ȡ������ʱ����Ϣ
	unsigned long CPlatformDataMgr::GetExchangeTime(const string& ExchangeID,string& time)
	{
		ASSERT("The method is not implemented in pats.");
		return 0;
	}
	int CPlatformDataMgr::GetExchangeTime(ExchangeTime& outData)
	{
		ASSERT("The method is not implemented in pats.");
		return 0;
	}
	//���ý�����ʱ����Ϣ
	void CPlatformDataMgr::SetExchangeTime(const ExchangeTime& inData)
	{
		ASSERT("The method is not implemented in pats.");
	}



	//��ȡ������Ϣ���ɹ�����0��ʧ�ܷ���-1
	int CPlatformDataMgr::GetLastSettlementInfo(std::string& outData)
	{
		ASSERT("The method is not implemented in pats.");
		return 0;
	}

	//���������Ϣ
	void CPlatformDataMgr::ClearLastSettlementInfo(void)
	{
		ASSERT("The method is not implemented in pats.");
	}

	//��ӽ�����Ϣ
	void CPlatformDataMgr::AppendLastSettlementInfo(const std::string& inData,bool bLast)
	{
		ASSERT("The method is not implemented in pats.");
	}


	//ҵ���ƣ��ɹ�����0��ʧ�ܷ���-1
	int CPlatformDataMgr::SubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID, const GuiCallBackFunc& callback)
	{
		int ret;

		if(BID>=BID_MAX || GID>=GID_MAX || callback==NULL)
			return -1;


		EnterCriticalSection(&m_CS);

		ret=0;
		try
		{
			m_subscribeInfo[BID][GID]=callback;
		}
		catch(...)
		{
			ret=-2;
		}
		LeaveCriticalSection(&m_CS);

		return ret;
	}


	///ȡ��ҵ�����ݵĶ��ƣ��ɹ�����0��ʧ�ܷ���-1
	int CPlatformDataMgr::UnSubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID)
	{
		int ret;

		if(BID>=BID_MAX || GID>=GID_MAX)
			return -1;

		EnterCriticalSection(&m_CS);

		ret=0;

		std::map<BusinessID, std::map<GUIModuleID, GuiCallBackFunc> >::iterator it1=m_subscribeInfo.find(BID);
		if(it1==m_subscribeInfo.end())
			ret=-3;
		else
		{
			std::map<GUIModuleID, GuiCallBackFunc>::iterator it2 = it1->second.find(GID);
			if(it2==it1->second.end())
				ret=-3;
			else
			{
				it1->second.erase(it2);
				if(it1->second.empty())
					m_subscribeInfo.erase(it1);
				ret=0;
			}
		}
		LeaveCriticalSection(&m_CS);

		return ret;
	}


	///��ȡҵ������Ϣ���ɹ�����0��ʧ�ܷ���-1
	int CPlatformDataMgr::GetSubscribeBusinessData(const BusinessID BID, std::map<GUIModuleID,GuiCallBackFunc>& outData)
	{
		int ret=0;
		outData.clear();

		EnterCriticalSection(&m_CS);

		std::map<BusinessID, std::map<GUIModuleID, GuiCallBackFunc> >::iterator it1=m_subscribeInfo.find(BID);
		if(it1==m_subscribeInfo.end())
			ret=-1;
		else
		{
			outData=it1->second;
			ret=0;
		}
		LeaveCriticalSection(&m_CS);

		return ret;
	}

	SERVER_PARAM_CFG&  CPlatformDataMgr::GetServerParamCfg()
	{
		return m_ServerParamCfg;
	}

	void CPlatformDataMgr::ClearMapQryRltOrders()
	{
		Lock_CPlatformDataMgr;
		m_OrderData.WillQry_Internal(std::string(""));
		Unlock_CPlatformDataMgr;
	}

	//��ղ�ѯ�ɽ���map
	void CPlatformDataMgr::ClearMapQryRltTrades()
	{
		Lock_CPlatformDataMgr;
		m_TradeData.WillQry_Internal(std::string(""));
		Unlock_CPlatformDataMgr;
	}

	//��ղ�ѯ�ֲ���ϸ��map
	void CPlatformDataMgr::ClearMapQryRltPositionDetails(std::string& strQryInstrument)
	{
		ASSERT("The method is not implemented in pats.");
	}

	//��ղ�ѯ�ֲֵ�map
	void CPlatformDataMgr::ClearMapQryRltPositions(std::string& strQryInstrument)
	{
		Lock_CPlatformDataMgr;
		m_PositionData.WillQry_Internal(strQryInstrument);
		Unlock_CPlatformDataMgr;
	}

	//��ղ�ѯ��ϳֲֵ�map
	void CPlatformDataMgr::ClearMapQryRltPositionCombs(std::string& strQryInstrument)
	{
		ASSERT("The method is not implemented in pats.");
	}
	int CPlatformDataMgr::GetPositionFTIDList(std::vector<long>& vec)
	{
		vec.clear();
		Lock_CPlatformDataMgr;
		std::set<long>::iterator it_FTID = m_PositionData.m_allFTID.begin();
		for ( ; it_FTID != m_PositionData.m_allFTID.end(); ++it_FTID )
		{
			vec.push_back(*it_FTID);
		}
		Unlock_CPlatformDataMgr;

		return vec.size();
	}

	int CPlatformDataMgr::GetPositionInfo(long lFTID, PlatformStru_Position& outData)
	{
		int nRet = 0;
		Lock_CPlatformDataMgr;
		std::map<long, PlatformStru_Position*>::iterator it_FTID = m_PositionData.m_all2.find(lFTID);
		if ( it_FTID != m_PositionData.m_all2.end())
		{
			outData = *(it_FTID->second);
		}
		else
		{
			nRet = -1;
		}
		Unlock_CPlatformDataMgr;

		return nRet;
	}

	int CPlatformDataMgr::GetPositionInfo(PositionKey key, PlatformStru_Position& outData)
	{
		int nRet = 0;
		Lock_CPlatformDataMgr;
		std::map<PositionKey, PlatformStru_Position*>::iterator it_all = m_PositionData.m_all.find(key);
		if ( it_all != m_PositionData.m_all.end())
		{
			outData = *(it_all->second);
		}
		else
		{
			nRet = -1;
		}
		Unlock_CPlatformDataMgr;

		return nRet;
	}
	int CPlatformDataMgr::GetPositionCombFTIDList(std::vector<long>& vec)
	{
		ASSERT("The method is not implemented in pats.");
		return 0;
	}

	int CPlatformDataMgr::GetPositionCombInfo(long lFTID, PlatformStru_PositionComb& outData)
	{
		ASSERT("The method is not implemented in pats.");
		return 0;
	}
	bool CPlatformDataMgr::HavePositionDetail(const std::string& strInstrumentID)
	{
		return true;
	}
}