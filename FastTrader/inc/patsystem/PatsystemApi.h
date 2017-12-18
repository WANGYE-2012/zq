#pragma once

#include <list>
#include <string>
#include <map>
#include <vector>
using namespace std;
#include "patsystem.h"
#include "patsIntf.h"
#include "ISvr\PlatformStruct.h"

#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "ApiFunc.h"


#pragma warning (disable : 4251)
#pragma warning(disable:4996)
#define THOST_FTDC_OST_ERROE  'd'
typedef CHAR SafeFloatStr[128];
typedef struct _tagPatsPositionItem
{
    _tagPatsPositionItem(Array20 ac,Array10 ex,Array10 cn,Array50 cd)
    {
        memset(this, 0, sizeof(*this));
        strncpy(TraderAccount,ac,20);
        strncpy(ExchangeName,ex,20);
        strncpy(ContractName,cn,20);
        strncpy(ContractDate,cd,20);
    };
    bool operator<(const _tagPatsPositionItem  &b)const 
    {
        int ret;
        ret=strcmp(TraderAccount,b.TraderAccount);
        if(ret!=0) return ret<0;
        ret=strcmp(ExchangeName,b.ExchangeName);
        if(ret!=0) return ret<0;
        ret=strcmp(ContractName,b.ContractName);
        if(ret!=0) return ret<0;
        ret=strcmp(ContractDate,b.ContractDate);
        if(ret!=0) return ret<0;
        return false;
    }
    Array20 TraderAccount;
    Array10 ExchangeName;
    Array10 ContractName;
    Array50 ContractDate;
}PatsPositionItem;
class CPatsystemApi
{
public:
	CPatsystemApi(const char *pszFlowPath);
	~CPatsystemApi(void);
public:
	static CPatsystemApi *CreateFtdcApi(const char *pszFlowPath = "", const bool bIsUsingUdp=false);
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
    int GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& Data);
	///��ȡ�˻��ʽ��ı���Ϣ
	int GetAccountText(std::string& outData,int language);

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
	void DoQryInstrument();
	void FireLogon();
protected:
	 void CalcPositionCloseProfit(const std::string strAccount,PlatformStru_TradingAccountInfo& info);
	 void DoLogon(BOOL bOK,CThostFtdcReqUserLoginField *pReqUserLoginField=NULL, int nRequestID=0);
	 std::string MakeLongInstrumentID(std::string exchange, std::string name, std::string date);
	 void GetLastUpdateTime( char* pBuf, int nLen, const PriceDetailStruct& priceDetail );
	 std::string GetLogonStatusString(BYTE LogonStatus);
	 void initOrderTypeMap();
public:
	int GetAccountList(std::vector<string>& AcctList);	
	int GetExchangeIDs(vector<string>& outData);
	//����������
	void SetNewPwd( const std::string& strNewPwd );
	//��ʱ��
	friend VOID CALLBACK OnTimerProc(HWND   hwnd,UINT   uMsg,UINT_PTR   idEvent,DWORD   dwTime);
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
   //
   double GetExchangeRate(char* currency);
   bool GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi);
   bool GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi);
   bool GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi);

   bool GetPositionInfo( const std::string& strAccount, const std::string& strInstrumentID, PlatformStru_Position& field );
   void GetAllPositionInfo(std::vector<PlatformStru_Position>& vPosition);
   // ��ȡ��¼״̬ԭ��
   std::string GetLogonReason();
   //ע��ص�
   void RegisterCallback(bool reg);
	void GetAllContractPosition(std::vector<PlatformStru_Position>& vecPosition);
private:
	HANDLE m_hThread; 
	CPatsQuotSpi *m_pMdSpi;
	CPatsTraderSpi *m_pTraderSpi;
    std::list<TraderAcctStruct> m_AccountList;//�˻��б�
	std::list<OrderTypeStruct> m_OrderType;//���������б�
	//std::list<FillStruct> m_FillList;//�ɽ��б�
	std::list<OrderDetailStruct> m_OrderList;//�ɽ��б�
	std::list<CommodityStruct> m_Commodity;//��Ʒ�б�
	TraderStr m_accountName;
	std::map<std::string, ContractStruct> m_mapInstrumentDetail;
	std::map<std::string, PlatformStru_DepthMarketData*> m_mapInstrumentPrices;
	std::map<std::string, char> mapOrderType;
	std::set<std::string> m_SubscribedInstrument;
    std::set<PatsPositionItem> m_PatsPositionItems;
	INT iTimerID;
	PlatformStru_TradingAccountInfo m_FundAccount;
	std::string m_strNewPwd;
	LogonStatusStruct m_logonStatus;
	bool bFisrtReqUserLogin;
	bool bFirstLinkConnected;
public:
	int	m_HostLinkState;
	int	m_PriceLinkState;
	zqWriteLog* m_pWriteLog;
	ClientType m_ClientType;
public:
	//�����ǻص�����
	static void WINAPI onHostLinkStateChange(LinkStateStructPtr data);
	static void WINAPI onPriceLinkStateChange(LinkStateStructPtr data);
	static void WINAPI onLogonStatus();
	static void WINAPI onDataDLComplete();
	static void WINAPI onForcedLogout();
	static void WINAPI onContractAdded(ContractUpdStructPtr data);
	static void WINAPI onContractDeleted(ContractUpdStructPtr data);
	static void WINAPI onMessage(MsgIDPtr MsgID);
	static void WINAPI onOrder(OrderUpdStructPtr data);
	static void WINAPI onStatusChange(StatusUpdStructPtr data);
	static void WINAPI onFill(FillUpdStructPtr data);
	static void WINAPI onPriceUpdate(PriceUpdStructPtr data);
	static void WINAPI onTickerUpdate(TickerUpdStructPtr data);
	static void WINAPI onExchangeRate(ExchangeRateUpdStructPtr data);
	static void WINAPI onConnectivityStatus(ConnectivityStatusUpdStructPtr data);
	static void WINAPI onOrderCancelFailure(OrderUpdStructPtr data);
	static void WINAPI onAtBestUpdate(AtBestUpdStructPtr data);
	static void WINAPI onMemoryWarning();
	static void WINAPI onSubscriberDepthUpdate(SubscriberDepthUpdStructPtr data);
	static void WINAPI onDOMUpdate(DOMUpdStructPtr data);
	static void WINAPI onSettlementCallback(SettlementPriceStructPtr data);
	static void WINAPI onStrategyCreateSuccess(StrategyCreateSuccessStructPtr data);
	static void WINAPI onStrategyCreateFailure(StrategyCreateFailureStructPtr data);
	static void WINAPI onGenericPriceUpdate(GenericPriceStructPtr data);
	static void WINAPI onBlankPrice(BlankPriceStructPtr data);
	static void WINAPI onOrderSentFailure(OrderUpdStructPtr data);
	static void WINAPI onOrderQueuedFailure(OrderUpdStructPtr data);
	static void WINAPI onExchangeUpdate( ExchangeUpdStructPtr data);
	static void WINAPI onCommodityUpdate(CommodityUpdStructPtr data);
	static void WINAPI onPurgeCompleted( ExchangeUpdStructPtr data);
	static void WINAPI onOrderTypeUpdate(OrderTypeStructPtr data);
	static void InvalidateDepthMarketData( PlatformStru_DepthMarketData* pDepthMarketData );
};
VOID   CALLBACK   OnTimerProc(HWND   hwnd,UINT   uMsg,UINT_PTR   idEvent,DWORD   dwTime) ;