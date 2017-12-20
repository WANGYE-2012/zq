#pragma once
#include <string>
#include <set>
#include <map>
#include <list>
#include <vector>
using namespace std;
#include "platformID.h"
#include "BusinessData.hpp"
#include "PlatformStruct.h"
enum ServiceStatus
{
    SS_NOLOGIN,            //��δ����
    SS_LOGINING,           //���ڵ�����
    SS_LOGINFAILED,        //����ʧ��
    SS_LOGINED,            //����ɹ�
    SS_LOSTCONN//,           //ʧȥ���ӣ��ڲ����Զ�����
    //SS_LOGOUT              //�ѵǳ�
};
enum UIOrderType
{
	UIOT_STANDARD,
	UIOT_BATCH,
	UIOT_CONDITION,
	UIOT_PROFIT,
	UIOT_HEDGE,
	UIOT_STOPLIMIT,
	UIOT_REPLACEORDER,
	UIOT_SIMULATEMARKET,
	UIOT_OPENCLOSE, 
	UIOT_ACCOUNT
};
class IPlatformSingleSvr
{
public:
	virtual PTYPE GetPlatformType()=0;
   ///����ƽ̨
    virtual int Start()=0;

    ///�ر�ƽ̨
    virtual int Stop()=0;
    ///�ر�ƽ̨�е��߳�
    virtual int StopThread()=0;
	///�����Ƿ��ǵ�ǰ����
	///1���ǣ�����������
	virtual int IsValidPW(const std::string& yourPW)=0;
	virtual int GetBrokerIDUserID(std::string& UserID, std::string& BrokerID, set<string>* accounts=NULL)=0;
    virtual string GetAccount()=0;
	///���ý���ǰ�û���ַ
    virtual int SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr)=0;

    ///��������ǰ�û���ַ
    virtual int SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr)=0;

    ///���þ��͹�˾������Ϣ
    virtual int SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, const std::string& UserPW, const std::string& OneTimePassword)=0;
	//���ô���������ַ���
	virtual int SetProxyConnectStr(const std::string& str)=0;

    ///����
    virtual int Login(const std::string& InvestorID, const std::string& InvestorPW, const std::string& InvestorNewPwd="")=0;

    ///�ǳ�
    virtual int Logout()=0;

    ///����ҵ������
    virtual int SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo=false)=0;

    ///ȡ��ҵ�����ݵĶ���
    virtual int UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID)=0;

    ///��ȡ�ײ����ĵ�ǰ״̬
    virtual ServiceStatus GetCurrentStatus(int* pQuotStatus=NULL,int* pTradeStatus=NULL)=0;

    ///��ȡ��Լ�б�
    virtual int GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID="")=0;
	virtual int GetInstrumentList(std::vector<GroupInfo> &outData)=0;
    ///���������Լ�б�
    virtual int AddMainInstrumentList(std::string instrument)=0;
    ///��ȡ������Լ�б�
    virtual int GetMainInstrumentList(std::set<std::string> &outData)=0;
	// ���ú�Լ�ݲ��б�
	virtual void SetInstrumentVarietyMap(map<string, string>& inData)=0;
	// ��ȡ��Լ�ݲ��б�
	virtual int GetInstrumentVarietyMap(map<string, string>& outData)=0;
    //���ú�Լ����״̬
	virtual void SetSubscribeStatus(const std::string& InstrumentID,int status,S_Status oper=S_STATUS_ADD)=0;
    ///��ȡָ����Լ��Ϣ
    virtual int GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)=0;
    ///����ָ����Լ��Ϣ
    virtual void SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)=0;
    ///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
    virtual char GetProductClassType(const std::string& InstrumentID)=0;

    ///��ȡ��ԼƷ�ִ���
    virtual int GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID="")=0;    

    ///��ȡ��Լ��������
    virtual int GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)=0;
    ///��ȡ��Լ�������ʣ����û�У��򴥷���ѯ
    virtual int GetCommissionRate_TriggerReq(const std::string& InstrumentID)=0;
    ///���ú�Լ��������
    virtual int SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)=0;  
    ///��ȡ��Լ��֤����
    virtual int GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)=0;
    ///��ȡ��Լ��֤���ʣ����û�У��򴥷���ѯ
    virtual int GetMarginRate_TriggerReq(const std::string& InstrumentID)=0;
    ///���ú�Լ��֤����
    virtual int SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)=0;     
    ///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
    virtual int GetInstrumentMultiple(const std::string& InstrumentID)=0;
	///��ȡָ��Ʒ�ֵĺ�Լ�б�,���غ�Լ����
	virtual int GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData)=0;    
    
    ///��ȡָ����Լ����
    virtual int GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)=0;
    ///��ȡָ����Լ������
    virtual int GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)=0;
    ///��ȡָ����Լ���ּۣ�ʧ�ܷ���0.0
    virtual double GetCurPrice(const std::string& InstrumentID)=0;

    //��ȡ��������������
    virtual string GetTradingDay(const string& ExchangeID)=0;
	virtual bool IsTodayPosition( const char* pOpenData ) = 0;
    ///��ȡ������ʱ��
    virtual unsigned long GetExchangeTime(const string& ExchangeID,string& time)=0;
    ///��ȡȫ��������
    virtual int GetExchangeIDs(vector<string>& outData)=0;
    ///��ȡ�����ʽ��˻���Ϣ
	virtual int GetTradingAccountWithdrawQuota(double& fWithdrawQuota)=0;
	virtual int GetTradingAccountAvailable(double& fAvailable)=0;
	virtual int GetTradingAccountID(char* AccountID)=0;
	virtual int GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& outData)=0;
	///��ȡ�˻��ʽ��ı���Ϣ
	virtual int GetAccountText(std::string& outData,int language)=0;
    ///���ý����ʽ��˻���Ϣ
    virtual void SetTradingAccount(const PlatformStru_TradingAccountInfo& inData)=0;
    ///�����ѯͶ���߽�������Ӧ
    virtual int GetLastSettlementInfo(std::string& outData)=0;


    ///��ȡָ����Լ�ĳֲ���ϸ     add zhifa.gao	
    virtual bool HavePositionDetail(const std::string& strInstrumentID)=0;

    virtual int GetPositions2(const std::string& strInstrument,
                                std::set<long>& setFTID,
                                std::vector<PlatformStru_Position>& outData,
                                long& lastseq)=0;
    virtual int GetPositions3(const std::string& strInstrument,
                                std::vector<PlatformStru_Position>& outData,
                                long& lastseq)=0;
    virtual int GetPositionDetails3(const std::string& strInstrument,
                                      std::set<long>& setFTID,
                                      std::vector<PlatformStru_PositionDetail>& vecValue,
                                      long& lastseq)=0;
	virtual int GetPositionCombs2(const std::string& strTradeID, std::string& strInstrument,std::vector<PlatformStru_PositionComb>& outData)=0;

    virtual int GetPositions(std::vector<PlatformStru_Position>& outData)=0;
    virtual int GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq)=0;
    virtual int GetPositionCombs(std::vector<PlatformStru_PositionComb>& outData)=0;

    //��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
    virtual int GetPositionKeySet(std::set<PositionKey> &PositionKeySet)=0;
    //��ȡָ���˺š���Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
    virtual int GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData)=0;

    //��ȡ�ҵ�����
    virtual int  GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, const char CloseType,const char HedgeFlag)=0;

    //��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
    virtual int GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)=0;

    ///���õ�ǰ��ʾ�ĳֲ����ݡ�1:�ֲ֣�2:�ֲ���ϸ��3:��ϳֲ�
    virtual void SetCurrentPositionContent(int PositionContentMode)=0;


    ///��ȡȫ������
    virtual int GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///��ȡȫ������
    virtual int GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///��ȡָ������
    virtual bool GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData)=0;

    ///��ȡȫ���Ѵ����ı���
    virtual int GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///��ȡ��Լ��ص��Ѵ����ı���
    virtual int GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///��ȡָ���Ѵ����ı���
    virtual bool GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)=0;

    ///��ȡ�ѳɽ�����
    virtual int GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///��ȡ�ѳɽ�����
    virtual int GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///��ȡָ���ѳɽ�����
    virtual bool GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)=0;

    ///��ȡ�ѳ����ʹ��󱨵�
    virtual int GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///��ȡ�ѳ����ʹ��󱨵�
    virtual int GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///��ȡָ���ѳ����ʹ��󱨵�
    virtual bool GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)=0;

    ///��ȡδ�ɽ�����
    virtual int GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///��ȡδ�ɽ�����
    virtual int GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///��ȡָ��δ�ɽ�����
    virtual bool GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)=0;

    ///��ȡȫ���ɽ���
    virtual int GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData)=0;
    ///��ȡָ���ɽ���Ϣ
    virtual bool GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData)=0;
    ///��ȡָ���ɽ���Ϣ
    virtual int GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData)=0;
    ///��ȡȫ���ɽ�ͳ�Ƽ�¼
    virtual int GetAllTradeTotals(std::vector<PlatformStru_TradeTotalInfo>& outData)=0;
    ///��ȡָ����Լ�ĳɽ�ͳ�Ƽ�¼�ɹ�����0��ʧ�ܷ���-1
    virtual int GetTradeTotalOfInstrument(const std::string& strInstrument, std::vector<PlatformStru_TradeTotalInfo>& outData)=0;
    ///��ȡ�ɽ�ͳ��
    virtual int GetAllTradeTotalDatas(vector<TotalInfo>& outData)=0;
	///�ڳɽ�ͳ�Ʋ��Ҳ���rawData
	virtual int FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData )=0;


	virtual bool IsSettlementInfoConfirmed()=0;
	virtual void GetErrorField(int& ErrorID)=0;
	virtual void GetErrorField(string& ErrMsg)=0;
	virtual void SetReqID(int nOrderReqID)=0; 
	virtual long GetReqID()=0; 
	virtual long GetNextReqID()=0;
	virtual int  GetAccountList(std::vector<string>& AcctList)=0;
	virtual void FillQuotData( const PlatformStru_DepthMarketData& rawQuot, QuotFiled fieldID, std::vector<std::string>& vec )=0;

	///������ID
	virtual const string ExchangeID2String(const string& ExchangeID) = 0;
	//���ݺ�Լ���ý�����ID������Ƿ�ϵͳ��Լ���򷵻ؿ�
	virtual const string ExchangeName2ID(const string& name) = 0;
	//��ȡ����������
	virtual SERVER_PARAM_CFG& GetServerParamCfg()=0;
	//�ж��б��ֶ��ڸ�ƽ̨�Ƿ���Ч
	virtual bool IsFieldValid( int nModuleId, int nFieldId ) = 0;
	virtual bool IsModuleValid( int nModuleId )=0;
	//�ж��Ƿ�֧�ֱ�ֵ
	virtual bool HaveOrderType(UIOrderType type)=0;
	//�ж��Ƿ�֧������ת��
	virtual bool HaveBankFutureTransfer()=0;
	
	// ��ȡ��Լ�ļ�Ʊ�ṹ��so as "CU,ͭ,T"
	virtual void GetContractShortNames(string& strShortNames)=0;
	//
	virtual string GetVersionIconString()=0;
	//
	virtual bool SupportPlugins()=0;
	//�Ƿ�֧�ֶ��˺�
	virtual bool IsMultipleAccounts()=0;
	///��ȡ�˺���Ϣ
   virtual  int GetTrader(std::vector<PlatformStru_Trader> &outData) =0;
    ///��ȡ��������Ϣ
   virtual  int GetExchange(std::vector<PlatformStru_Exchange> &outData) =0;
    ///��ȡ��Լ��Ϣ
   virtual  int GetContract(std::vector<PlatformStru_Contract> &outData) =0;
    ///��ȡ��Ʒ��Ϣ
   virtual  int GetCommodity(std::vector<PlatformStru_Commodity> &outData) =0;
   ///��ȡ����������Ϣ
   virtual  int GetOrderType(std::vector<PlatformStru_OrderType> &outData) =0;
   //��ȡ�˻��ֲ�
   virtual bool GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi)=0;
   virtual bool GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi)=0;
   virtual bool GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi)=0;
   //�ж��Ƿ�֧�ֻ�ȡ�˺���Ϣ
   virtual bool HaveGetTrader()=0;
   //�ж��Ƿ�֧��ȡ��������Ϣ
   virtual bool HaveGetExchange()=0;
   //�ж��Ƿ�֧�ֻ�ȡ��Լ��Ϣ
   virtual bool HaveGetContract()=0;
   //�ж��Ƿ�֧�ֻ�ȡ��Ʒ��Ϣ
   virtual bool HaveGetCommodity()=0;
   //�ж��Ƿ�֧�ֻ�ȡ����������Ϣ
   virtual bool HaveGetOrderType()=0;
   //�ж��Ƿ�֧�ֻ�ȡ�ֲ���Ϣ
   virtual bool HaveGetPosition()=0;
   //�ж��Ƿ�֧�ֻ�ȡ������Ϣ
   virtual bool HaveGetReport()=0;
   //�ж��Ƿ�֧�ֻ�ȡ������Ϣ
   virtual bool HaveGetExchangeRate()=0;
	//�ж��Ƿ�֧���ڵ�¼ʱ�������޸�����
   virtual bool HaveModifyPwdOnLongon()=0;
	//�ж��Ƿ�֧��
	virtual bool HaveShowInstumentComboList()=0;
   //��ȡ��¼״̬ԭ��
   virtual std::string GetLogonReason()=0;
   //��ȡPats����״̬
   virtual void GetLinkStatus(int* pHost,int* pPrice)=0;
   //�Ƿ�֧�ֽ�ʿ������
   virtual bool IsSunGardSupported() = 0;
   //�Ƿ�֧��
   virtual bool IsHistorySettlementSupported() = 0;
   //�Ƿ����ʽ��ѯ����
   virtual bool HaveAccountText()=0;
   //�Ƿ����ʽ��ѯ����
   virtual bool HaveFileCache()=0;
   //��Լ���ͺ�ԼID�Ƿ���ȫһ��
   virtual bool ContractNameIsID()=0;
   //��֤���Ƿ��ں�Լ��Ϣ��ؽṹ����
   virtual bool MarginRatioInInstrumentInfo()=0;
   //��ȡ�ͻ�ǩԼ������Ϣ
   virtual int GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData)=0;
public:

    /*--------------------------------------------------------------------------------------------------------
               ����ҵ���������첽�ģ��趨�ƾ���ҵ�����ݣ������յ�������
      ---------------------------------------------------------------------------------------------------------*/
    ///�û������������
   virtual  int ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate &pUserPasswordUpdate, int nRequestID=0)=0;

    ///�ʽ��˻������������
   virtual  int ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate &pTradingAccountPasswordUpdate, int nRequestID=0)=0;

    ///����¼������
   virtual  int ReqOrderInsert(PlatformStru_InputOrder &pInputOrder, int nRequestID=0)=0;

    ///Ԥ��¼������
   virtual  int ReqParkedOrderInsert(PlatformStru_ParkedOrder &pParkedOrder, int nRequestID=0)=0;

    ///Ԥ�񳷵�¼������
   virtual  int ReqParkedOrderAction(PlatformStru_ParkedOrderAction &pParkedOrderAction, int nRequestID=0)=0;

    ///������������
   virtual  int ReqOrderAction(PlatformStru_InputOrderAction &pInputOrderAction, int nRequestID=0)=0;

    ///��ѯ��󱨵���������
   virtual  int ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume &pQueryMaxOrderVolume, int nRequestID=0)=0;

    ///Ͷ���߽�����ȷ��
   virtual  int ReqSettlementInfoConfirm() =0;

    ///����ɾ��Ԥ��
   virtual  int ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder &pRemoveParkedOrder, int nRequestID=0) =0;

    ///����ɾ��Ԥ�񳷵�
   virtual  int ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction &pRemoveParkedOrderAction, int nRequestID=0) =0;

    //��ѯ�����Ƿ�Ϊ��
    virtual bool IsQryQueueEmpty(void)=0;

    ///�����ѯ����
   virtual  int ReqQryOrder(PlatformStru_QryOrder &pQryOrder, int nRequestID=0) =0;

    ///�����ѯ�ɽ�
   virtual  int ReqQryTrade(PlatformStru_QryTrade &pQryTrade, int nRequestID=0) =0;

    ///�����ѯͶ���ֲ߳�
   virtual  int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition &pQryInvestorPosition, int nRequestID=0) =0;

    ///�����ѯ�ʽ��˻�
   virtual  int ReqQryTradingAccount(PlatformStru_QryTradingAccount &pQryTradingAccount, int nRequestID=0) =0;
  // virtual int  GetTradingAccount(PlatformStru_TradingAccountInfo& Data)=0;

    ///�����ѯͶ����
   virtual  int ReqQryInvestor(PlatformStru_QryInvestor &pQryInvestor, int nRequestID=0) =0;


    ///�����ѯ��Լ
   virtual  int ReqQryInstrument(PlatformStru_QryInstrument &pQryInstrument, int nRequestID=0) =0;

    ///�����ѯ����
   virtual  int ReqQryDepthMarketData(PlatformStru_QryDepthMarketData &pQryDepthMarketData, int nRequestID=0) =0;

    ///�����ѯͶ���߽�����
   virtual  int ReqQrySettlementInfo(PlatformStru_QrySettlementInfo &pQrySettlementInfo, int nRequestID=0) =0;

    ///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
   virtual  int ReqQryCFMMCTradingAccountKey(PlatformStru_QryCFMMCTradingAccountKey &QryCFMMCTradingAccountKey, int nRequestID=0)=0; 

    ///�����ѯת������
   virtual  int ReqQryTransferBank(PlatformStru_QryTransferBank &pQryTransferBank, int nRequestID=0) =0;

    ///�����ѯͶ���ֲ߳���ϸ
   virtual  int ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail &pQryInvestorPositionDetail, int nRequestID=0) =0;

    ///�����ѯ�ͻ�֪ͨ
   virtual  int ReqQryNotice(PlatformStru_QryNotice &pQryNotice, int nRequestID=0) =0;

    ///�����ѯ������Ϣȷ��
   virtual  int ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm &pQrySettlementInfoConfirm, int nRequestID=0) =0;

    ///�����ѯͶ���ֲ߳���ϸ
   virtual  int ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail &pQryInvestorPositionCombineDetail, int nRequestID=0) =0;

    ///�����ѯת����ˮ
   virtual  int ReqQryTransferSerial(PlatformStru_QryTransferSerial &pQryTransferSerial, int nRequestID=0) =0;

    ///�����ѯǩԼ����
   virtual  int ReqQryContractBank(PlatformStru_QryContractBank &pQryContractBank, int nRequestID=0) =0;
        
    ///�����ѯ����ǩԼ��ϵ
	virtual  int ReqQryAccountregister(PlatformStru_QryAccountRegister &pQryAccountregister, int nRequestID=0)=0;

    ///�����ѯԤ��
   virtual  int ReqQryParkedOrder(PlatformStru_QryParkedOrder &pQryParkedOrder, int nRequestID=0) =0;

    ///�����ѯԤ�񳷵�
   virtual  int ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction &pQryParkedOrderAction, int nRequestID=0) =0;


    ///�ڻ����������ʽ�ת�ڻ�����
   virtual  int ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer &pReqTransfer, int nRequestID=0) =0;

    ///�ڻ������ڻ��ʽ�ת��������
   virtual  int ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer &pReqTransfer, int nRequestID=0) =0;

    ///�ڻ������ѯ�����������
   virtual  int ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount &pReqQueryAccount, int nRequestID=0) =0;
	virtual void SetModuleDataQry(bool bQry)=0;
	virtual bool GetModuleDataQry()=0;
    //��������
	virtual void ReConnetServer()=0;
	//��ȡ����
	virtual int GetReports(map<DWORD, Report>& mapReport)=0;
	//��ȡ���һ���
	virtual double GetExchangeRate(char* currency)=0;

    virtual void NotifyStartStopInitQry(bool bStart)=0;

	//Ϊ���������½ӿ�
	//��ȡί�е��б�FTID
	virtual int GetAllOrderFTIDList(std::vector<long>& vec)=0;
	//��ȡί�е�����
	virtual int GetAllOrderInfo(long lFTID, PlatformStru_OrderInfo& outData)=0;
	virtual int GetAllOrderInfo(OrderKey key, PlatformStru_OrderInfo& outData)=0;

	//��ȡ�ֲ��б�FTID
	virtual int GetPositionFTIDList(std::vector<long>& vec)=0;
	//��ȡ�ֲ�����
	virtual int GetPositionInfo(long lFTID, PlatformStru_Position& outData)=0;
	virtual int GetPositionInfo(PositionKey key, PlatformStru_Position& outData)=0;

	//��ȡ��ϳֲ��б�FTID
	virtual int GetPositionCombFTIDList(std::vector<long>& vec)=0;
	//��ȡ��ϳֲ�����
	virtual int GetPositionCombInfo(long lFTID, PlatformStru_PositionComb& outData)=0;
	//virtual int GetPositionCombInfo(PositionCombKey key, PlatformStru_PositionComb& outData)=0;
	//��ȡƽ̨һ����Լ�����ĺ�Լ����
	virtual int GetGroupMaxContractNum()=0;
	//��Լ�������ʲ���Ҫ�ر���
	virtual bool ContractCountIsFit()=0;
	//
	virtual bool GetMACurrentAccount()=0;
	virtual void SetMACurrentAccount(bool bCur)=0;

	virtual bool GetQryFinished()=0;
	virtual void SetQryFinished(bool bFinished)=0;
public:
    static IPlatformSingleSvr* Create(PTYPE type);
	static void Release(IPlatformSingleSvr* pSingleSvr);
};