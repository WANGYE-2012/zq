#pragma once

#include <list>
#include <string>
#include <map>
#include <vector>
using namespace std;
#include "CmeCore.h"
#include "ISvr\PlatformStruct.h"

#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "DataWorker.h"
#include "../inc/Tools/UniPkg.h"
#include "../inc/tools/client_stru.h"
#include "../inc/tools/client_cmdid.h"
#pragma warning (disable : 4251)
#pragma warning(disable:4996)
#define THOST_FTDC_OST_ERROE  'd'
struct EPCID
{
    int ExchangeID;
    int ProductID;
    int ContractID;
    EPCID(int e=0,int p=0,int c=0)
    {
        ExchangeID=e;
        ProductID=p;
        ContractID=c;
    }
    bool operator <(const EPCID& rhs) const 
    {
        if(ExchangeID!=rhs.ExchangeID) 
            return (ExchangeID<rhs.ExchangeID);
        if(ProductID!=rhs.ProductID) 
            return (ProductID<rhs.ProductID);
        return (ContractID<rhs.ContractID);
    }
};
class PATSYSTEM_API CCmeCoreApi
{
public:
	CCmeCoreApi(void);
	~CCmeCoreApi(void);
public:
	static CCmeCoreApi *CreateFtdcApi(const char *pszFlowPath = "", const bool bIsUsingUdp=false);
	///ɾ���ӿڶ�����
	///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
	virtual void Release();
	
	///��ʼ��
	///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
	virtual void Init();
	
	///�ȴ��ӿ��߳̽�������
	///@return �߳��˳�����
	virtual int Join();
	
	///��ȡ��ǰ������
	///@retrun ��ȡ���Ľ�����
	///@remark ֻ�е�¼�ɹ���,���ܵõ���ȷ�Ľ�����
	virtual const char *GetTradingDay();
	
	///ע��ǰ�û������ַ
	///@param pszFrontAddress��ǰ�û������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	virtual void RegisterFront(char *pszFrontAddress);
	
	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	virtual void RegisterSpi(CPatsQuotSpi *pSpi);
	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	virtual void RegisterSpi(CPatsTraderSpi *pSpi);
	//---------------------------------------------------------------------------------------------
	///�������顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	virtual int SubscribeMarketData(char *ppInstrumentID[], int nCount);

	///�˶����顣
	///@param ppInstrumentID ��ԼID  
	///@param nCount Ҫ����/�˶�����ĺ�Լ����
	///@remark 
	virtual int UnSubscribeMarketData(char *ppInstrumentID[], int nCount);
	//---------------------------------------------------------------------------------------------
	///����˽������
	///@param nResumeType ˽�����ش���ʽ  
	///        THOST_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        THOST_TERT_RESUME:���ϴ��յ�������
	///        THOST_TERT_QUICK:ֻ���͵�¼��˽����������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ�˽���������ݡ�
	virtual void SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType) ;
	
	///���Ĺ�������
	///@param nResumeType �������ش���ʽ  
	///        THOST_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        THOST_TERT_RESUME:���ϴ��յ�������
	///        THOST_TERT_QUICK:ֻ���͵�¼�󹫹���������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ������������ݡ�
	virtual void SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType) ;

	///�ͻ�����֤����
	virtual int ReqAuthenticate(CThostFtdcReqAuthenticateField *pReqAuthenticateField, int nRequestID) ;

	///�û���¼����
	virtual int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID) ;
	

	///�ǳ�����
	virtual int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID) ;

	///�û������������
	virtual int ReqUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID) ;

	///�ʽ��˻������������
	virtual int ReqTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, int nRequestID) ;

	///����¼������
	virtual int ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nRequestID) ;

	///Ԥ��¼������
	virtual int ReqParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, int nRequestID) ;

	///Ԥ�񳷵�¼������
	virtual int ReqParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, int nRequestID) ;

	///������������
	virtual int ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, int nRequestID) ;

	///��ѯ��󱨵���������
	virtual int ReqQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, int nRequestID) ;

	///Ͷ���߽�����ȷ��
	virtual int ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID) ;

	///����ɾ��Ԥ��
	virtual int ReqRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, int nRequestID) ;

	///����ɾ��Ԥ�񳷵�
	virtual int ReqRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, int nRequestID) ;

	///�����ѯ����
	virtual int ReqQryOrder(CThostFtdcQryOrderField *pQryOrder, int nRequestID) ;

	///�����ѯ�ɽ�
	virtual int ReqQryTrade(CThostFtdcQryTradeField *pQryTrade, int nRequestID) ;

	///�����ѯͶ���ֲ߳�
	virtual int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition *pQryInvestorPosition, int nRequestID) ;

	///�����ѯ�ʽ��˻�
	virtual int ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID) ;

	///�����ѯͶ����
	virtual int ReqQryInvestor(CThostFtdcQryInvestorField *pQryInvestor, int nRequestID) ;

	///�����ѯ���ױ���
	virtual int ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID) ;

	///�����ѯ��Լ��֤����
	virtual int ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID) ;

	///�����ѯ��Լ��������
	virtual int ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID) ;

	///�����ѯ������
	virtual int ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID) ;

	///�����ѯ��Լ
	virtual int ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID) ;

	///�����ѯ����
	virtual int ReqQryDepthMarketData(CThostFtdcQryDepthMarketDataField *pQryDepthMarketData, int nRequestID) ;

	///�����ѯͶ���߽�����
	virtual int ReqQrySettlementInfo(CThostFtdcQrySettlementInfoField *pQrySettlementInfo, int nRequestID) ;

	///�����ѯת������
	virtual int ReqQryTransferBank(CThostFtdcQryTransferBankField *pQryTransferBank, int nRequestID) ;

	///�����ѯͶ���ֲ߳���ϸ
	virtual int ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail, int nRequestID) ;

	///�����ѯ�ͻ�֪ͨ
	virtual int ReqQryNotice(CThostFtdcQryNoticeField *pQryNotice, int nRequestID) ;

	///�����ѯ������Ϣȷ��
	virtual int ReqQrySettlementInfoConfirm(CThostFtdcQrySettlementInfoConfirmField *pQrySettlementInfoConfirm, int nRequestID) ;

	///�����ѯͶ���ֲ߳���ϸ
	virtual int ReqQryInvestorPositionCombineDetail(CThostFtdcQryInvestorPositionCombineDetailField *pQryInvestorPositionCombineDetail, int nRequestID) ;

	///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
	virtual int ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField *pQryCFMMCTradingAccountKey, int nRequestID) ;

	///�����ѯ�ֵ��۵���Ϣ
	virtual int ReqQryEWarrantOffset(CThostFtdcQryEWarrantOffsetField *pQryEWarrantOffset, int nRequestID) ;

	///�����ѯת����ˮ
	virtual int ReqQryTransferSerial(CThostFtdcQryTransferSerialField *pQryTransferSerial, int nRequestID) ;

	///�����ѯ����ǩԼ��ϵ
	virtual int ReqQryAccountregister(CThostFtdcQryAccountregisterField *pQryAccountregister, int nRequestID) ;

	///�����ѯǩԼ����
	virtual int ReqQryContractBank(CThostFtdcQryContractBankField *pQryContractBank, int nRequestID) ;

	///�����ѯԤ��
	virtual int ReqQryParkedOrder(CThostFtdcQryParkedOrderField *pQryParkedOrder, int nRequestID) ;

	///�����ѯԤ�񳷵�
	virtual int ReqQryParkedOrderAction(CThostFtdcQryParkedOrderActionField *pQryParkedOrderAction, int nRequestID) ;

	///�����ѯ����֪ͨ
	virtual int ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID) ;

	///�����ѯ���͹�˾���ײ���
	virtual int ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID) ;

	///�����ѯ���͹�˾�����㷨
	virtual int ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID) ;

	///�ڻ����������ʽ�ת�ڻ�����
	virtual int ReqFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID) ;

	///�ڻ������ڻ��ʽ�ת��������
	virtual int ReqFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID) ;

	///�ڻ������ѯ�����������
	virtual int ReqQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, int nRequestID) ;
	//---------------------------------------------------------------------------------------------
public:
    int GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& Data);
    ///��ȡ�˻��ʽ��ı���Ϣ
    int GetAccountText(std::string& outData,int language);
    //
    int GetAccountList(std::vector<string>& AcctList);	
    bool GetPositionInfo( const std::string& strAccount, 
        const std::string& strInstrumentID, PlatformStru_Position& field );
    void GetAllContractPosition(std::vector<PlatformStru_Position>& vecPosition);
    void SetNewPwd( const std::string& strNewPwd );
    int GetExchangeIDs(vector<string>& outData);
	///��ȡ�˺���Ϣ
	int GetTrader(std::vector<PlatformStru_Trader> &outData);
    ///��ȡ��������Ϣ
   int GetExchange(std::vector<PlatformStru_Exchange> &outData);
    ///��ȡ��Լ��Ϣ
   int GetContract(std::vector<PlatformStru_Contract> &outData);
    ///��ȡ��Ʒ��Ϣ
   int GetCommodity(std::vector<PlatformStru_Commodity> &outData);
   ///��ȡ����������Ϣ
   int GetOrderType(std::vector<PlatformStru_OrderType>& outData);
   // ��ȡ����
   int GetReports(map<DWORD, Report>& mapReport);
   bool GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi);
   bool GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi);
   bool GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi);
   // ��ȡ��¼״̬ԭ��
   std::string GetLogonReason();
   void GetLinkStatus(int* pHost,int* pPrice);
   double GetExchangeRate(char* currency);
public:
    void	trade_rcvdata(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);	//��tcp�õ����ݺ󣬵��ô˻ص��������д���
    void	trade_disconn(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);		//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
    void	trade_startconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);										//��ʼ����connect()�󣬵��ô˺���֪ͨӦ�ò�
    void	trade_connsuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);									//���ӳɹ��󣬵��ô˻ص��������д���
    void	trade_connfailure(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);									//����ʧ�ܺ󣬵��ô˻ص��������д���
    void    trade_ontime(void);
    void	quot_rcvdata(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);	//��tcp�õ����ݺ󣬵��ô˻ص��������д���
    void	quot_disconn(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);		//�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
    void	quot_startconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);										//��ʼ����connect()�󣬵��ô˺���֪ͨӦ�ò�
    void	quot_connsuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);									//���ӳɹ��󣬵��ô˻ص��������д���
    void	quot_connfailure(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);									//����ʧ�ܺ󣬵��ô˻ص��������д���
    void    quot_ontime(void);
public:
    void ParseRcvData(const char *pData,int DataLen,bool bTrader);
    void OnRspUserLogin(const char *pData,int DataLen,bool bTrader);
    void OnRspUserLogout(const char *pData,int DataLen,bool bTrader);
    void OnRspQrySettlementInfoConfirm(const char *pData,int DataLen,bool bTrader);
    void OnRspQrySettlementInfo(const char *pData,int DataLen,bool bTrader);
    void OnRspSettlementInfoConfirm(const char *pData,int DataLen,bool bTrader);
    void OnRspGetCurrencyInfo(const char *pData,int DataLen,bool bTrader);
    void OnCurrencyInfoUpdate(const char *pData,int DataLen,bool bTrader);
    void OnRspGetAllExchangeInfo(const char *pData,int DataLen,bool bTrader);
    void OnExchangeInfoUpdate(const char *pData,int DataLen,bool bTrader);
    void OnRtnExchangeStatus(const char *pData,int DataLen,bool bTrader);
    void OnRspGetExchangeProductGroupInfo(const char *pData,int DataLen,bool bTrader);
    void OnProductGroupInfoUpdate(const char *pData,int DataLen,bool bTrader);
    void OnRspGetExchangeProductInfo(const char *pData,int DataLen,bool bTrader);
    void OnProductInfoUpdate(const char *pData,int DataLen,bool bTrader);
    void OnContractInfoUpdate(const char *pData,int DataLen,bool bTrader);
    void GetExchangeContractInfo(const char *pData,int DataLen,bool bTrader);
    void OnRspQryInstrument(int RequestID);
    void FillInstrumentInfo(const int ExchangeID,const Stru_Base_ContractInfo& ContractInfo,
        PlatformStru_InstrumentInfo& Instrument);
    void MakeGUIContractID(const Stru_Base_ExchangeInfo& ExchangeInfo,
                                    const Stru_Base_ProductInfo& ProductInfo,
                                    const Stru_Base_ContractInfo& ContractInfo,char* strContract);
    void OnRspQryTradingAccount(const char *pData,int DataLen,bool bTrader);
    void OnRspGetDepositWithdrawInfo(const char *pData,int DataLen,bool bTrader);
    void OnDepositWithdrawInfoUpdate(const char *pData,int DataLen,bool bTrader);
    void OnRspQryTrade(const char *pData,int DataLen,bool bTrader);
    void OnRtnTrade(const char *pData,int DataLen,bool bTrader);
    void OnRtnPositionDetail(const char *pData,int DataLen,bool bTrader);
    void OnRspQryInvestorPositionDetail(const char *pData,int DataLen,bool bTrader);
    void OnRspQryOrder(const char *pData,int DataLen,bool bTrader);
    void OnRspOrderCmd(const char *pData,int DataLen,bool bTrader);
    void OnRspOrderStatusUpdate(const char *pData,int DataLen,bool bTrader);
    void OnRspSubscribeMarketData(const char *pData,int DataLen,bool bTrader);
    void OnRspUnsubscribeMarketData(const char *pData,int DataLen,bool bTrader);
    void OnRspGetMarketDataSnapshot(const char *pData,int DataLen,bool bTrader);
    void OnNewMarketData(const char *pData,int DataLen,bool bTrader);
    void OnRspUserPasswordUpdate(const char *pData,int DataLen,bool bTrader);
    void SendHeartbeat(bool bTrader);
    void OnRspGetExchangeCommissionMarginInfo(const char *pData,int DataLen,bool bTrader);
    void OnCommissionMarginInfoUpdate(const char *pData,int DataLen,bool bTrader);
    void FillDepthMarketData(const Stru_Base_Market_SnapshotData& md,CThostFtdcDepthMarketDataField& field);
    void FillDepthMarketData(const Stru_Client_NewMarketData& md,PlatformStru_DepthMarketData& field);
    void FillOrderInfo(const Stru_Base_OrderInfo& Info,PlatformStru_OrderInfo& Order);
    void FillTradeInfo(const Stru_Base_TradeInfo& src,PlatformStru_TradeInfo& info);
    void FillPositionInfo(const Stru_Base_PositionInfo& src,PlatformStru_Position& pos);
    void FillTradingAccount(const Stru_Base_EquityInfo& src,CThostFtdcTradingAccountField& field);
    void FillTradingAccount(const Stru_Client_DepositWithdrawDynaUpdate& src,CThostFtdcTradingAccountField& field);
    void FillTradingAccount(const Stru_Base_DepositWithdrawDyna& src,CThostFtdcTradingAccountField& field);
    void FillOrderCmdInfo(const PlatformStru_InputOrder& src,Stru_Base_OrderCmdInfo& field);
    void FillOrderCmdInfo(const CThostFtdcInputOrderActionField& src,Stru_Base_OrderCmdInfo& field);
    void FillOrderCmdInfo(const CThostFtdcParkedOrderField& src,Stru_Base_OrderCmdInfo& field);
    void FillOrderCmdInfo(const CThostFtdcParkedOrderActionField& src,Stru_Base_OrderCmdInfo& field);

    void FillOrderCmdInfo(const EnumOrderCmdFlag flag,const Stru_Base_OrderInfo src,CThostFtdcInputOrderField& field);
    void FillOrderCmdInfo(const EnumOrderCmdFlag flag,const Stru_Base_OrderInfo src,CThostFtdcInputOrderActionField& field);
    void FillOrderCmdInfo(const EnumOrderCmdFlag flag,const Stru_Base_OrderInfo src,CThostFtdcParkedOrderField& field);
    void FillOrderCmdInfo(const EnumOrderCmdFlag flag,const Stru_Base_OrderInfo src,CThostFtdcParkedOrderActionField& field);
    void FillRtnOrder(const Stru_Base_OrderInfo& src,PlatformStru_OrderInfo& field);
    void FillCommissionMarginInfo(const Stru_Base_CustomerCommissionInfo& src,PlatformStru_InstrumentCommissionRate& cr);
    void FillCommissionMarginInfo(const Stru_Base_CustomerMarginInfo& src,PlatformStru_InstrumentMarginRate&  mr);
protected:
    void ParseAddress(char *pszFrontAddress,Stru_IPAddress& TradeFrontAddr,Stru_IPAddress& QuotFrontAddr);
    CTradeDataWorker*   m_pTradeDataWorker;
    CQuotDataWorker*    m_pQuotDataWorker;
    Stru_IPAddress      m_TradeFrontAddr;
    Stru_IPAddress      m_QuotFrontAddr;
    CPatsQuotSpi*       m_pMdSpi;
    CPatsTraderSpi*     m_pTraderSpi;
private:
    map<int,Stru_Base_CurrencyInfo>                 m_CurrencyInfos;//<CurrencyID,Stru_Base_CurrencyInfo>
    map<int,Stru_Base_ExchangeInfo>::const_iterator m_QueryExchangeInfoIterator;//<ExhangeID,Stru_Base_ExchangeInfo>  
    map<int,Stru_Base_ExchangeInfo>                 m_ExchangeInfos;//<ExhangeID,Stru_Base_ExchangeInfo>  
    map<int,map<int,Stru_Base_ProductGroupInfo>>     m_ProductGroupInfos;//<ExhangeID,<ProductGroupID,Stru_Base_ProductGroupInfo>>
    map<int,map<int,Stru_Base_ProductInfo>>         m_ProductInfos;//<ExhangeID,<ProductID,Stru_Base_ProductInfo>>
    map<int,map<int,Stru_Base_ContractInfo>>         m_ContractInfos;//<ExhangeID,<ContractID,Stru_Base_ContractInfo>>
    vector<Stru_Base_SettlementReport>               m_SettlementReportInfos;//
    map<string,EPCID>                                m_ContractName2ID;//<ContractGUIName,ContractID>
    map<int,string>                                  m_ID2ContractName;//<ContractID,ContractGUIName>
    map<int,EnumOrderCmdFlag>                       m_RequestID2OrderCmdFlag;//<RequestID,OrderCmdFlag>
    int                                           m_TradeOverTimeCount,m_QuotOverTimeCount;//��ʱ����
    CThostFtdcTradingAccountField                  m_TradingAccount;//��ǰ�ʽ���Ϣ
    set<int>                                       m_ReqCommissionMarginInfo;//<ExchangeID>
/////////////////////////////////////////////////////////////////////////////////////////////////////
    int SubscribeUnsubscribeID;
    map<int,string>                                 m_SubscribeUnsubscribeMarketMap;
};
