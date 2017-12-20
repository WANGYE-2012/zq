/*
    ���ڱ����ά���ײ�����
    ��������Լ���ݡ��������ݡ��ֲ���ϸ���ݡ��������ݡ��ɽ������ݡ��ʽ����ݵ�
    added by l. 20110216
*/


#pragma once

#include <vector>
#include <map>
#include <list>
#include <set>
#include <string>
#include <hash_map>
#include "../inc/ISvr/BusinessData.hpp"
#include "../inc/Module-Misc/packagecache.h"
#include "../inc/platformSvr/PlatformDataSetList.hpp"


class CPackageCache;

class zqWriteLog;


#if 1
#define LogLock1 
#else
#define LogLock1 \
                { \
                    char buf[256];  \
                    const char*pf=__FUNCDNAME__;  \
                    const char*p=strchr(pf,'@');  \
                    int i=p!=NULL?(p-pf):0;\
                    if(i>100) i=100;  \
                    memcpy(buf,"..lock1: ",9);  \
                    if(i>0) memcpy(buf+9,pf,i);  \
                    buf[9+i]='\n';  \
                    buf[9+i+1]=0;  \
                    OutputDebugString(buf); \
                }
#endif

#if 1
#define LogLock2 
#else
#define LogLock2 \
                { \
                    char buf[256];  \
                    const char*pf=__FUNCDNAME__;  \
                    const char*p=strchr(pf,'@');  \
                    int i=p!=NULL?(p-pf):0;\
                    if(i>100) i=100;  \
                    memcpy(buf,"..lock2: ",9);  \
                    if(i>0) memcpy(buf+9,pf,i);  \
                    buf[9+i]='\n';  \
                    buf[9+i+1]=0;  \
                    OutputDebugString(buf); \
                }
#endif

#if 1
#define LogUnlock 
#else
#define LogUnlock \
                { \
                    char buf[256];  \
                    const char*pf=__FUNCDNAME__;  \
                    const char*p=strchr(pf,'@');  \
                    int i=p!=NULL?(p-pf):0;\
                    if(i>100) i=100;  \
                    memcpy(buf,".unlock: ",9);  \
                    if(i>0) memcpy(buf+9,pf,i);  \
                    buf[9+i]='\n';  \
                    buf[9+i+1]=0;  \
                    OutputDebugString(buf); \
                }
#endif

#define Lock_CPlatformDataMgr \
                { \
                    LogLock1  \
                    EnterCriticalSection(&m_CS);  \
                    LogLock2 \
                }

#define Unlock_CPlatformDataMgr \
                { \
                    LogUnlock  \
                    LeaveCriticalSection(&m_CS); \
                }

namespace Pats
{
	class CPlatformDataMgr
	{
	public:
		CPlatformDataMgr(CPlatFormService* pHost);
		~CPlatformDataMgr(void);
		//----------�������ʽ��˻��ӿ�----------
		//PATS��ʵʱͨ��API�ӿڲ�ѯ��ȡ�ʽ���Ϣ
		//����Ҫ����ͻ����ʽ���Ϣ
		//---------------------------------------
		///���ý����ʽ��˻���Ϣ
		void SetTradingAccount(const PlatformStru_TradingAccountInfo& inData);
		///���ý����ʽ��˻���Ϣ
		void SetTradingAccount(const DataRspQryTradingAccount& outData);
		///��ȡ�����ʽ��˻���Ϣ
		int GetTradingAccountWithdrawQuota(double& fWithdrawQuota);
		int GetTradingAccountAvailable(double& fAvailable);
		int GetTradingAccountID(char* AccountID);


		//----------�����Ǻ�Լ��Ϣ�ӿ�----------
		///��ȡָ����Լ��Ϣ
		void SetInstrumentInfo(const std::string& InstrumentID, const PlatformStru_InstrumentInfo& inData);
		///��ȡָ����Լ��Ϣ
		int GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
		///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
		char GetProductClassType(const std::string& InstrumentID);
		///��ȡ��Լ�б�
		int GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID="");
		int GetInstrumentList(std::vector<GroupInfo> &outData);
		///���������Լ�б�
		int AddMainInstrumentList(std::string instrument);
		///��ȡ������Լ�б�
		int GetMainInstrumentList(std::set<std::string> &outData);
		///���ú�Լ�ݲ��б�
		void SetInstrumentVarietyMap(map<string, string>& inData);
		// ��ȡ��Լ�ݲ��б�
		int GetInstrumentVarietyMap(map<string, string>& outData);

		///��ȡָ��Ʒ�ֵĺ�Լ�б�,���غ�Լ����
		int GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData);
		///��ȡ��ԼƷ�ִ���
		int GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID="");
		///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
		int GetInstrumentMultiple(const std::string& InstrumentID);
		///��ȡ��Լ��������
		int GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
		///���ú�Լ��������
		void SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
		///�������ڲ�ѯ�������ʵĺ�Լ
		void SetReqCommissionRateInstrumentID(const std::string& InstrumentID);
		///��ȡ���ڲ�ѯ�������ʵĺ�Լ
		void GetReqCommissionRateInstrumentID(std::string& outData);

		///��ȡ��Լ��֤����
		int GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
		///���ú�Լ��֤����
		void SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
	private:
		//�ж�һ���ַ��������Ƿ��ǵ���
		bool IsTodayPosition(const char *OpenDate);
		bool IsTodayPositionDetail(const PlatformStru_PositionDetail& PositionDetail);
		///��ȡָ����Լ��Ϣ���ڲ����ã����ü���
		int GetInstrumentInfo_Internal(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
		///��ȡ��Լ��֤���ʣ��ڲ����ã����ü���
		int GetMarginRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
		///��ȡ��Լ�������ʣ��ڲ����ã����ü���
		int GetCommissionRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
		//�ӳɽ���ȡ�����ѡ����û�з��ʣ���0����
		void GetAllCommission_Internal(double& TotalCommission);
	public:
		//----------����������ӿ�----------
		string GetTradingDay(const string& ExchangeID);
		//��ȡ������ʱ����Ϣ
		unsigned long GetExchangeTime(const string& ExchangeID,string& time);
		//���ý�����ʱ����Ϣ
		int GetExchangeTime(ExchangeTime& outData);
		//���ý�����ʱ����Ϣ
		void SetExchangeTime(const ExchangeTime& inData);

		//��ȡ������Ϣ���ɹ�����0��ʧ�ܷ���-1
		int GetLastSettlementInfo(std::string& outData);
		//���������Ϣ
		void ClearLastSettlementInfo(void);
		//��ӽ�����Ϣ
		void AppendLastSettlementInfo(const std::string& inData,bool bLast);
		//----------����������ӿ�----------

		//��ȡָ����Լ���飬�ɹ�����0��ʧ�ܷ���-1
		int GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
		int GetQuotInfo_Internal(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
		//����ָ����Լ���飬���õ�������£��򷵻�true�����򷵻�false
		bool SetQuotInfo(const PlatformStru_DepthMarketData& inData);
		void ConvertComboInstrumentID(const std::string strComboID, std::string& strInstrumentID1, std::string& strInstrumentID2);
		//��ȡָ����Լ�����¼�
		double GetCurPrice (const std::string& InstrumentID);
		//��ȡָ����Լ�����飬�ɹ�����0��ʧ�ܷ���-1
		int GetOldQuotInfo (const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);


		//ҵ���ƣ��ɹ�����0��ʧ�ܷ���-1
		int SubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID, const GuiCallBackFunc& callback);
		///ȡ��ҵ�����ݵĶ��ƣ��ɹ�����0��ʧ�ܷ���-1
		int UnSubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID);
		///��ȡҵ������Ϣ���ɹ�����0��ʧ�ܷ���-1
		int GetSubscribeBusinessData(const BusinessID BID, std::map<GUIModuleID,GuiCallBackFunc>& outData);



		//��ѯǰ��ո���map
		void ClearMapQryRltOrders();
		void ClearMapQryRltTrades();
		void ClearMapQryRltPositionDetails(std::string& strQryInstrument);
		void ClearMapQryRltPositions(std::string& strQryInstrument);
		void ClearMapQryRltPositionCombs(std::string& strQryInstrument);


	public:  
		//----------�����Ǳ����ӿ�----------
		void QryOrderReach(const PlatformStru_OrderInfo& OrderInfo,const int ErrID,const bool bIsLast);
		//�±�������
		void NewOrderReady(PlatformStru_OrderInfo& OrderInfo, 
			PlatformStru_OrderInfo& OldOrderInfo,
			const PlatformStru_InstrumentInfo& InstrumentInfo);
		//�������б����еĶ��ᱣ֤�𣬷��غ�Լ����Ƿ��б����������Ƿ���Ҫ�����ʽ���塣��ѯ��֤������Ӧʱ����
		bool UpdateOrderFreezeMargin_RspQryMarginRate(std::string&	strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo);
		//�����б����еĶ��������ѣ����غ�Լ����Ƿ��б����������Ƿ���Ҫ�����ʽ���塣��ѯ����������Ӧʱ����
		bool UpdateOrderFreezeCommission_RspQryCommissionRate(std::string&	strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo);
		//���㱨����Ϣ�еĶ��ᱣ֤��
		void CalcuOrderFreezeMargin_Internal(PlatformStru_OrderInfo& OrderInfo,const PlatformStru_InstrumentInfo& InstrumentInfo);
		//���㱨����Ϣ�еĶ���������
		void CalcuOrderFreezeCommission_Internal(PlatformStru_OrderInfo& OrderInfo,const PlatformStru_InstrumentInfo& InstrumentInfo);
		int GetAllOrderFTIDList(std::vector<long>& vec);
		///��ȡȫ������
		int GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData);
		int GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
		///��ȡָ������
		bool GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData);
		///��ȡȫ���Ѵ����ı���
		int GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData);
		///��ȡ��Լ��ص��Ѵ����ı���
		int GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
		///��ȡָ���Ѵ����ı���
		bool GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

		///��ȡδ�ɽ�����
		int GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData);
		///��ȡδ�ɽ�����
		int GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
		///��ȡָ����δ�ɽ�����
		bool GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

		///��ȡ�ѳɽ�����
		int GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData);
		///��ȡ�ѳɽ�����
		int GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
		///��ȡָ���ѳɽ�����
		bool GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

		///��ȡ�ѳ����ʹ��󱨵�
		int GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData);
		///��ȡ�ѳ����ʹ��󱨵�
		int GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
		///��ȡָ���ѳ����ʹ��󱨵�
		bool GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

		//��ȡָ����Լ��ָ������ָ����ƽ�Ĺҵ�������������
		int GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, const char CloseType,const char HedgeFlag=THOST_FTDC_HF_Speculation);
		//��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
		int GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag=THOST_FTDC_HF_Speculation);
		//��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ڲ����ã����������ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
		int  GetCloseOrderVolume_Internal(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag);
	public:  
		//----------�����ǳɽ���¼�ӿ�----------
		//ԭʼ�ĳɽ��ر�����
		void QryTradeReach( const PlatformStru_TradeInfo& TradeInfo,const int ErrID,const bool bIsLast);
		//�µĳɽ��ر�����
		void NewTradeReady( PlatformStru_TradeInfo& TradeInfo,const PlatformStru_InstrumentInfo& InstrumentInfo);
		//����ɽ�������
		int GetAllOrderInfo(long lFTID, PlatformStru_OrderInfo& outData);
		int GetAllOrderInfo(OrderKey key, PlatformStru_OrderInfo& outData);    
		///��ȡȫ���ɽ���
		int GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData);
		///��ȡָ���ɽ���Ϣ
		bool GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData);
		///��ȡȫ���ɽ���
		int GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData);
		//�ӳɽ���ȡ�����ѡ����û�з��ʣ���0����]
	public:
		//----------�����ǳֲֽӿ�----------
		//��ѯ�ĳֲּ�¼����
		void QryPositionReach(PlatformStru_Position& PositionInfo,const int ErrID,const bool bIsLast);

		//�ӳɽ���¼���³ֲ֡��˺����ӳֲ���ϸ�����´���������ֲ֡������ڳֲ���ϸ�������ڳֲ���ϸ��������ٸ��³ֲ�
		void UpdatePosition_RtnTrade();

		//��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
		int GetPositionKeySet(std::set<PositionKey> &PositionKeySet);

		int GetPositions(std::vector<PlatformStru_Position>& outData);
		int GetPositions2(const std::string& strInstrument,std::set<long>& setFTID,std::vector<PlatformStru_Position>& outData,long& lastseq);
		int GetPositions3(const std::string& strInstrument3,std::vector<PlatformStru_Position>& outData,long& lastseq);
		//��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
		int GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData);
		//��ȡ�ֲ��б�FTID
		int GetPositionFTIDList(std::vector<long>& vec);
		//��ȡ�ֲ�����
		int GetPositionInfo(long lFTID, PlatformStru_Position& outData);
		int GetPositionInfo(PositionKey key, PlatformStru_Position& outData);
		//��ȡ��ϳֲ��б�FTID
		int GetPositionCombFTIDList(std::vector<long>& vec);
		//��ȡ��ϳֲ�����
		int GetPositionCombInfo(long lFTID, PlatformStru_PositionComb& outData);
		SERVER_PARAM_CFG&  GetServerParamCfg();
		bool HavePositionDetail(const std::string& strInstrumentID);
	private:

		std::map<BusinessID, std::map<GUIModuleID, GuiCallBackFunc> >  m_subscribeInfo;

		//��Լ��Ϣ�������������ݼ��ڲ�ѯ��Լ��Ϣ����еõ���������gui����
		std::map<std::string,  PlatformStru_InstrumentInfo> m_allInstruments; //��ǰ���к�Լ��Ϣ��ϵͳ��ʼ��ʱ��ȡ
		std::set<std::string>  m_allInstrumentList;                         //��ǰ���к�Լ�б�
		std::map<std::string, std::set<std::string> > m_allProductID;       //���в�Ʒ����
		std::vector<GroupInfo> m_allGroupInfo;

		//�������ݼ�
		std::map<std::string,  PlatformStru_DepthMarketData> m_lastQuot;  //��������
		std::map<std::string,  PlatformStru_DepthMarketData> m_OldQuot;  //�ϴ�������


		//----------�����Ǳ������ݼ�----------
		CPlatformDataSetList<OrderKey,PlatformStru_OrderInfo> m_OrderData;
		//----------�����ǳɽ����ݼ�----------
		CPlatformDataSetList<TradeKey,PlatformStru_TradeInfo> m_TradeData;
		//----------�����ǳֲ����ݼ�----------
		CPlatformDataSetList<PositionKey,PlatformStru_Position> m_PositionData;

		zqWriteLog*           m_pWriteLog;
		CRITICAL_SECTION m_CS;
		SERVER_PARAM_CFG m_ServerParamCfg;
	protected:
		CPlatFormService* m_PlatFormService;

	};

}