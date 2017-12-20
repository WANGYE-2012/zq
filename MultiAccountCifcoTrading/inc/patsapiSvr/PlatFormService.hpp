/***************************************************************************//**
*   @file          PlatFormService.hpp
*   @brief        �ۺϽ���ƽ̨(CTP)���׽ӿ�
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/09/13
********************************************************************************/
#ifndef _PLATFORMSERVICE_HPP_
#define _PLATFORMSERVICE_HPP_

#include <string>
#include <set>
#include <map>
#include <list>
#include <vector>


//#include "ctp/ThostFtdcTraderApi.h"
//#include "ctp/ThostFtdcMdApi.h"

#include "TraderSpi.hpp"
#include "QuotSpi.hpp"
#include "../inc/ISvr/BusinessData.hpp"
#include "../inc/ISvr/PlatformStruct.h"
#include "../inc/Patsystem/PatsystemApi.h"


class CPackageCache;

class zqWriteLog;


namespace Pats
{
	class CDataCenter;
	class CPlatFormParam
	{
	public:
		//���涨�彻��������״̬
		enum CONNSTATUS
		{    
			CONNSTATUS_Disconnected   =      0       ,            //δ����
			CONNSTATUS_Connecting     =      1       ,            //��������
			CONNSTATUS_Logining       =      2       ,            //���ڵ�¼
			CONNSTATUS_LoginFailure   =      3       ,            //��¼ʧ��
			CONNSTATUS_TradeSettlementInfoConfirming     =   4 ,  //���ڽ��н��׽�����ȷ��
			//CONNSTATUS_TradeSettlementInfoConfirmFailure =   5 ,  //���׽�����ȷ��ʧ��
			CONNSTATUS_Connected      =      6       ,            //���ӵ�¼�ɹ�
			CONNSTATUS_Disconnecting  =      7       ,            //���ڶϿ�����
			CONNSTATUS_UnInit         =      8                    //δ��ʼ��
		};

	public:
		CPlatFormParam()
		{
			TradeStatus=CONNSTATUS_Disconnected;
			QuotStatus =CONNSTATUS_Disconnected;
			HaveAllInstrumentIDs=false;
			RequestID=0;
		};

	public:
		std::vector<std::string>   TradeFrontAddrs; //���ƶ��"asp-sim2-front1.financial-trading-platform.com:26205"
		std::vector<std::string>   QuotFrontAddrs;   //���ƶ��"58.246.49.130:41213"
		std::string                BrokerID;
		std::string                UserID;
		std::string                UserPW;
		std::string                InvestorID;
		std::string                InvestorPW;
		std::string					m_strInvestorNewPWD;
		CONNSTATUS                 TradeStatus;
		CONNSTATUS                 QuotStatus;
		bool                       HaveAllInstrumentIDs;
		int                        RequestID;
		std::string                OneTimePassword;
		std::set<std::string>		Accounts;		// �����˺��б�
	};

	class CPlatFormService
	{
		friend class CTraderSpi;
		friend class CQuotSpi;
		friend class CTradeThread;
		friend class CQuotThread;
		friend class CPatsApiSvr;
		//���׽ӿ�
	public:
		///�û������������
		int ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate *pUserPasswordUpdate, int nRequestID=0);

		///�ʽ��˻������������
		int ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate *pTradingAccountPasswordUpdate, int nRequestID=0);

		///����¼������
		int ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nRequestID=0);

		///Ԥ��¼������
		int ReqParkedOrderInsert(PlatformStru_ParkedOrder *pParkedOrder, int nRequestID=0);

		///Ԥ�񳷵�¼������
		int ReqParkedOrderAction(PlatformStru_ParkedOrderAction *pParkedOrderAction, int nRequestID=0);

		///������������
		int ReqOrderAction(PlatformStru_InputOrderAction *pInputOrderAction, int nRequestID=0);

		///��ѯ��󱨵���������
		int ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume *pQueryMaxOrderVolume, int nRequestID=0);

		///Ͷ���߽�����ȷ��
		int ReqSettlementInfoConfirm() ;

		///����ɾ��Ԥ��
		int ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder *pRemoveParkedOrder, int nRequestID=0) ;

		///����ɾ��Ԥ�񳷵�
		int ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction *pRemoveParkedOrderAction, int nRequestID=0) ;

		///�����ѯ����
		int ReqQryOrder(PlatformStru_QryOrder *pQryOrder, int nRequestID=0) ;

		///�����ѯ�ɽ�
		int ReqQryTrade(PlatformStru_QryTrade *pQryTrade, int nRequestID=0) ;

		///�����ѯͶ���ֲ߳�
		int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition *pQryInvestorPosition, int nRequestID=0) ;

		///�����ѯ�ʽ��˻�
		int ReqQryTradingAccount(PlatformStru_QryTradingAccount *pQryTradingAccount, int nRequestID=0) ;

		///�����ѯͶ����
		int ReqQryInvestor(PlatformStru_QryInvestor *pQryInvestor, int nRequestID=0) ;

		///�����ѯ���ױ���
		int ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID=0) ;

		///�����ѯ��Լ��֤����
		int ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID=0) ;

		///�����ѯ��Լ��������
		int ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID=0) ;

		///�����ѯ������
		int ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID=0) ;

		///�����ѯ��Լ
		int ReqQryInstrument(PlatformStru_QryInstrument *pQryInstrument, int nRequestID=0) ;

		///�����ѯ����
		int ReqQryDepthMarketData(PlatformStru_QryDepthMarketData *pQryDepthMarketData, int nRequestID=0) ;

		///�����ѯͶ���߽�����
		int ReqQrySettlementInfo(PlatformStru_QrySettlementInfo *pQrySettlementInfo, int nRequestID=0) ;

		///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
		int ReqQryCFMMCTradingAccountKey(PlatformStru_QryCFMMCTradingAccountKey *pQryCFMMCTradingAccountKey, int nRequestID=0) ;

		///�����ѯת������
		int ReqQryTransferBank(PlatformStru_QryTransferBank *pQryTransferBank, int nRequestID=0) ;

		///�����ѯͶ���ֲ߳���ϸ
		int ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail *pQryInvestorPositionDetail, int nRequestID=0) ;

		///�����ѯ�ͻ�֪ͨ
		int ReqQryNotice(PlatformStru_QryNotice *pQryNotice, int nRequestID=0) ;

		///�����ѯ������Ϣȷ��
		int ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm *pQrySettlementInfoConfirm, int nRequestID=0) ;

		///�����ѯͶ���ֲ߳���ϸ
		int ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail *pQryInvestorPositionCombineDetail, int nRequestID=0) ;

		///�����ѯת����ˮ
		int ReqQryTransferSerial(PlatformStru_QryTransferSerial *pQryTransferSerial, int nRequestID=0) ;

		///�����ѯǩԼ����
		int ReqQryContractBank(PlatformStru_QryContractBank *pQryContractBank, int nRequestID=0) ;

		///�����ѯ����ǩԼ��ϵ
		int ReqQryAccountregister(PlatformStru_QryAccountRegister *pQryAccountregister, int nRequestID=0);

		///�����ѯԤ��
		int ReqQryParkedOrder(PlatformStru_QryParkedOrder *pQryParkedOrder, int nRequestID=0) ;

		///�����ѯԤ�񳷵�
		int ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction *pQryParkedOrderAction, int nRequestID=0) ;

		///�����ѯ����֪ͨ
		int ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID=0) ;

		///�����ѯ���͹�˾���ײ���
		int ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID=0) ;

		///�����ѯ���͹�˾�����㷨
		int ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID=0) ;

		///�ڻ����������ʽ�ת�ڻ�����
		int ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer *pReqTransfer, int nRequestID=0) ;

		///�ڻ������ڻ��ʽ�ת��������
		int ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer *pReqTransfer, int nRequestID=0) ;

		///�ڻ������ѯ�����������
		int ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount *pReqQueryAccount, int nRequestID=0) ;

		//����ӿ�
	public:
		///���ۺϽ���ƽ̨��������
		int SubscribeMarketData(const std::string& InstrumentID);

		///���ۺϽ���ƽ̨ȡ�����鶨��
		int UnSubscribeMarketData(const std::string& InstrumentID);

		//���ƹ���ӿ�    
	public:
		int LoginTrade(const std::string& InvestorID, const std::string& InvestorPW, const std::string& InvestorNewPwd);
		int LogoutTrade();
		int LoginQuot(const std::string& InvestorID, const std::string& InvestorPW);
		int LogoutQuot();
		void StopThreads(void);
		///�����Ƿ��ǵ�ǰ����
		///1���ǣ�����������
		int IsValidPW(const std::string& yourPW);
		int GetBrokerIDUserID(std::string& UserID, std::string& BrokerID,set<string>* accounts=NULL);
		int SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr);
		int SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr);
		int SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, const std::string& UserPW, const std::string& OneTimePassword);   

		//�Զ���ӿ�
	public:
		///����ҵ������
		int SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo=false);

		///ȡ��ҵ�����ݵĶ���
		int UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID);

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
		///��ȡָ����Լ��Ϣ
		int GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
		///����ָ����Լ��Ϣ
		void SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
		///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
		char GetProductClassType(const std::string& InstrumentID);
		//���ú�Լ����״̬
		void SetSubscribeStatus(const std::string& InstrumentID,int status,S_Status oper=S_STATUS_ADD);
		///��ȡ��ԼƷ�ִ���
		int GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID="");    

		///��ȡ��Լ��������
		int GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
		///���ú�Լ��������
		int SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);  
		///�������ڲ�ѯ�������ʵĺ�Լ
		void SetReqCommissionRateInstrumentID(const std::string& InstrumentID);
		///��ȡ��Լ��֤����
		int GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
		///��ȡ��Լ��֤����
		int SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);  
		///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
		int GetInstrumentMultiple(const std::string& InstrumentID);


		///��ȡָ����Լ����
		int GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
		///��ȡָ����Լ������
		int GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
		///��ȡָ����Լ���ּۣ�ʧ�ܷ���0.0
		double GetCurPrice(const std::string& InstrumentID);


		bool HavePositionDetail(const std::string& strInstrumentID);

		int GetPositions2(const std::string& strInstrument,
			std::set<long>& setFTID,
			std::vector<PlatformStru_Position>& outData,
			long& lastseq);
		int GetPositions3(const std::string& strInstrument,
			std::vector<PlatformStru_Position>& outData,
			long& lastseq);
		int GetPositionDetails3(const std::string& strInstrument,
			std::set<long>& setFTID,
			std::vector<PlatformStru_PositionDetail>& vecValue,
			long& lastseq);
		int GetPositionCombs2(const std::string& strTradeID, std::string& strInstrument,std::vector<PlatformStru_PositionComb>& outData);

		int GetPositions(std::vector<PlatformStru_Position>& outData);
		int GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq);
		int GetPositionCombs(std::vector<PlatformStru_PositionComb>& outData);


		//��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
		int GetPositionKeySet(std::set<PositionKey> &PositionKeySet);
		//��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
		int GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData);

		///���õ�ǰ��ʾ�ĳֲ����ݡ�1:�ֲ֣�2:�ֲ���ϸ��3:��ϳֲ�
		void SetCurrentPositionContent(int PositionContentMode);

		string GetTradingDay(const string& ExchangeID);
		bool IsTodayPosition( const char* pOpenData );

		///��ȡ������ʱ��
		unsigned long GetExchangeTime(const string& ExchangeID,string& time);
		///��ȡȫ��������
		int GetExchangeIDs(vector<string>& outData);
		///��ȡȫ������
		int GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData);
		///��ȡȫ������
		int GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
		///��ȡָ������
		bool GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData);

		///��ȡȫ���Ѵ����ı���
		int GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData);
		///��ȡ��Լ��ص��Ѵ����ı���
		int GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
		///��ȡָ���Ѵ����ı���
		bool GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

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

		///��ȡδ�ɽ�����
		int GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData);
		///��ȡδ�ɽ�����
		int GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
		///��ȡָ��δ�ɽ�����
		bool GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);


		int GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag=THOST_FTDC_HF_Speculation);
		//��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
		int GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag=THOST_FTDC_HF_Speculation);

		///��ȡȫ���ɽ���
		int GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData);
		///��ȡָ���ɽ���Ϣ
		bool GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData);
		///��ȡָ���ɽ���Ϣ
		int GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData);
		///��ȡȫ���ɽ�ͳ�Ƽ�¼
		int GetAllTradeTotals(std::vector<PlatformStru_TradeTotalInfo>& outData);
		///��ȡָ����Լ�ĳɽ�ͳ�Ƽ�¼�ɹ�����0��ʧ�ܷ���-1
		int GetTradeTotalOfInstrument(const std::string& strInstrument, std::vector<PlatformStru_TradeTotalInfo>& outData);
		///��ȡ�ɽ�ͳ��
		int GetAllTradeTotalDatas(vector<TotalInfo>& outData);
		///�ڳɽ�ͳ�Ʋ��Ҳ���rawData
		int FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData );

		///��ȡ�����ʽ��˻���Ϣ
		int GetTradingAccountWithdrawQuota(double& fWithdrawQuota);
		int GetTradingAccountAvailable(double& fAvailable);
		int GetTradingAccountID(char* AccountID);
		int GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& outData);
		///��ȡ�˻��ʽ��ı���Ϣ
		int GetAccountText(std::string& outData,int language);
		///���ý����ʽ��˻���Ϣ
		void SetTradingAccount(const PlatformStru_TradingAccountInfo& inData);

		///�����ѯͶ���߽�������Ӧ
		int GetLastSettlementInfo(std::string& outData);
		void GetMarketDataInstrumentID(std::set<std::string>& setMarketDataInstrumentID);
		void SetOrderRef(int orderref){m_CurOrderRef=orderref;}
		//��ȡ�ͻ�ǩԼ������Ϣ
		int GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData);

		CPlatFormParam::CONNSTATUS GetTradeStatus(void);


		CPlatFormParam::CONNSTATUS GetQuotStatus(void);


		std::string GetTradeBrokerID(void);


		std::string GetTradeInvestorID(void);

		//
		bool IsSettlementInfoConfirmed();
		//
		void GetErrorField(int& ErrorID){ErrorID=m_ErrorInfoField.ErrorID;}
		void GetErrorField(string& ErrMsg){ErrMsg=m_ErrorInfoField.ErrorMsg;}
		void SetReqID(int nOrderReqID) {
#ifndef __PLATFORM_IB_
			m_nOrderReqID = nOrderReqID;
#endif
		};

		long GetReqID() {
#ifdef __PLATFORM_IB_
			if(m_pTradeApi != NULL)
				return m_pTradeApi->GetOrderID();
			else
				return -1;
#else
			return m_nOrderReqID;
#endif
		};

		long GetNextReqID() {
#ifdef __PLATFORM_IB_
			if(m_pTradeApi != NULL)
				return m_pTradeApi->GetNextOrderID();
			else
				return -1;
#else
			return ++m_nOrderReqID;
#endif
		};
		SERVER_PARAM_CFG&  GetServerParamCfg();
		void SetModuleDataQry(bool bQry);
		bool GetModuleDataQry();

		//Ϊ���������½ӿ�
		//��ȡί�е��б�FTID
		int GetAllOrderFTIDList(std::vector<long>& vec);
		//��ȡί�е�����
		int GetAllOrderInfo(long lFTID, PlatformStru_OrderInfo& outData);
		int GetAllOrderInfo(OrderKey key, PlatformStru_OrderInfo& outData);

		//��ȡ�ֲ��б�FTID
		int GetPositionFTIDList(std::vector<long>& vec);
		//��ȡ�ֲ�����
		int GetPositionInfo(long lFTID, PlatformStru_Position& outData);
		int GetPositionInfo(PositionKey key, PlatformStru_Position& outData);

		//��ȡ��ϳֲ��б�FTID
		int GetPositionCombFTIDList(std::vector<long>& vec);
		//��ȡ��ϳֲ�����
		int GetPositionCombInfo(long lFTID, PlatformStru_PositionComb& outData);

	private:
		CPlatFormService(CPatsApiSvr& rServiceProxy);
		~CPlatFormService();    


		///�û���¼����
		int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID=0);	

		///�ǳ�����
		int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID=0);

		zqWriteLog*              m_pWriteLog;

		CPatsystemApi*			m_pTradeApi;    //���׽ӿڶ������ۺϽ���ƽ̨�Ľӿ��ṩ
		CTraderSpi*             m_pTradeSpi;    //���׽ӿڻص�����
		CPatsystemApi*			m_pQuotApi;     //����ӿڶ������ۺϽ���ƽ̨�Ľӿ��ṩ
		CQuotSpi*               m_pQuotSpi;     //����ӿڻص�����

		CPackageCache*          m_PCache;       //CTraderSpi��CQuotSpi���������ķ��ص����ݱ��浽��cache�У�
		//CDataCenter��ȡ��cache
		CDataCenter*            m_pDataCenter;  //�������ģ������ȡm_PCache�е�ҵ�����ݣ������ݶ�����Ϣ
		//���ö�Ӧ�Ļص�����
		int				 m_CurOrderRef;//��ǰ�������ñ��
		std::set<std::string> m_MarketDataInstrumentID;
		enum PLATFORM_STATUS
		{
			PLATFORM_STATUS_UNINIT,
			PLATFORM_STATUS_INITED
		};
		int                     m_status;       //ƽ̨�ĵ�ǰ״̬
		CPlatFormParam          m_PlatformParam;//ƽ̨������
		bool					m_bSettlementInfoConfirmed;
		CThostFtdcRspInfoField  m_ErrorInfoField;

		int						m_nOrderReqID;
		std::map<std::string, int> m_SubscribeMap;       //��������ĺ�Լ�б�Ͷ���״̬map

		CRITICAL_SECTION m_CS;
public:
	CPatsApiSvr& m_rServiceProxy;
	public:
		//pats����
		int  GetAccountList(std::vector<string>& AcctList);
		void FillQuotData( const PlatformStru_DepthMarketData& rawQuot, QuotFiled fieldID,
			std::vector<std::string>& vec );
		int GetTrader(std::vector<PlatformStru_Trader> &outData) ;
		int GetExchange(std::vector<PlatformStru_Exchange> &outData);
		int GetContract(std::vector<PlatformStru_Contract> &outData);
		int GetCommodity(std::vector<PlatformStru_Commodity> &outData);
		int GetOrderType(std::vector<PlatformStru_OrderType> &outData);
		bool GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi);
		bool GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,
			PlatformStru_AccountPosition* posi);
		bool GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,
			PlatformStru_AccountPosition* posi);
		std::string GetLogonReason();
		void GetLinkStatus(int * pHost,int * pPrice);
		int GetReports(map<DWORD, Report>& mapReport);
		double GetExchangeRate(char* currency);
		void GetAllPosition(std::vector<PlatformStru_Position>& vecPosition);
	};
}
#endif

