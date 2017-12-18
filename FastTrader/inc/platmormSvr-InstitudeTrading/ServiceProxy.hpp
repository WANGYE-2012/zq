#pragma once
#include <string>
#include <set>
#include <map>
#include <list>
#include <vector>
#include "Module-Misc2/BusinessData.hpp"
//#include "Module-Misc2/PlatformStruct.h"
#include "ISvr/IPlatformSingleSvr.h"
#include "Module-Misc2/GlobalConfigMgr.h"
#include "QryQueue.h"
///////////////////////////////////////////////////////////////////////////
//����API����ֵ0��ʾ�ɹ���������ʾʧ��
class CPlatFormService;
class zqWriteLog;
class CServiceProxy :public IPlatformSingleSvr
{

public:
    ///����ƽ̨
    int Start();

    ///�ر�ƽ̨
    int Stop();
    //�ر�ƽ̨�е��߳�
    int StopThread();
	///�����Ƿ��ǵ�ǰ����
	bool IsValidInvestorPW(const std::string& yourPW);
	bool IsValidUserPW(const std::string& yourPW);
	int GetBrokerIDUserID(std::string& UserID, std::string& BrokerID, set<string>* accounts=NULL);
    string GetAccount();
    ///���ý���ǰ�û���ַ
    int SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr);

    ///��������ǰ�û���ַ
    int SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr);

    ///���þ��͹�˾������Ϣ
    int SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, const std::string& UserPW, const std::string& OneTimePassword);
	//���ô������������
	int SetProxyParam(const Stru_ProxyServerParam& ProxyParam);
    ///����
    int Login(const std::string& InvestorID, const std::string& InvestorPW, const std::string& InvestorNewPWD="");

    ///�ǳ�
    int Logout();

    ///����ҵ������
    int SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo=false);

    ///ȡ��ҵ�����ݵĶ���
    int UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID);

    ///��ȡ�ײ����ĵ�ǰ״̬
    ServiceStatus GetCurrentStatus(int* pQuotStatus=NULL,int* pTradeStatus=NULL);
    ///DataCenter�Ƿ��Ѿ��͵ײ�ͬ���ϣ����Կ�ʼ��ʼ����ѯ��
    bool IsDataCenterReady(void);

    ///��ȡ��Լ�б�
    int GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID="");
	int GetInstrumentList(std::vector<InstrumentGroupInfo> &outData);
    ///���������Լ�б�
    void AddMainInstrumentList(std::string instrument);
    ///��ȡ������Լ�б�
    int GetMainInstrumentList(std::set<std::string> &outData);
	// ���ú�Լ�ݲ��б�
	void SetInstrumentVarietyMap(map<string, string>& inData);
	// ��ȡ��Լ�ݲ��б�
	int GetInstrumentVarietyMap(map<string, string>& outData);
    //���ú�Լ����״̬
	void SetSubscribeStatus(const std::string& InstrumentID,int GID,SubscribeMarketDataOper oper=eSubscribeMarketData);
	//�˶�GID��Ӧ��ȫ����Լ����
	void UnsubscribeMarketDataOfGID(int GID);
	//���GID�Ƿ�����ָ����Լ������
	bool IsSubscribeMarketDataOfGID(const string& InstrumentID,int GID);
    ///��ȡָ����Լ��Ϣ
    int GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
	bool GetLegInstrumentID(const string& strComInstrumentID,
                                    string& strLeg1InstrumentID,
                                    string& strLeg2InstrumentID);
    ///����ָ����Լ��Ϣ
    void SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
    ///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
    char GetProductClassType(const std::string& InstrumentID);

    ///��ȡ��ԼƷ�ִ���
    int GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID="");    

    ///��ȡ��Լ��������
    int GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
    ///��ȡ��Լ�������ʣ����û�У��򴥷���ѯ
    int GetCommissionRate_TriggerReq(const std::string& InstrumentID);
    ///���ú�Լ��������
    int SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);  
    ///��ȡ��Լ��֤����
    int GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
    ///��ȡ��Լ��֤���ʣ����û�У��򴥷���ѯ
    int GetMarginRate_TriggerReq(const std::string& InstrumentID);
    ///���ú�Լ��֤����
    int SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);     
    ///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
    int GetInstrumentMultiple(const std::string& InstrumentID);
	///��ȡָ��Ʒ�ֵĺ�Լ�б�,���غ�Լ����
	int GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData);    
    
    ///��ȡָ����Լ����
    int GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
    ///��ȡָ����Լ������
    int GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
    ///��ȡָ����Լ���ּۣ�ʧ�ܷ�����Чֵ
    double GetCurPrice(const std::string& InstrumentID);


    //��ȡ��������������
    string GetTradingDay(const string& ExchangeID);
	bool IsTodayPosition( const char* pOpenData );
    ///��ȡ������ʱ��
    unsigned long GetExchangeTime(const string& ExchangeID,string& time);
    ///��ȡȫ��������
    int GetExchangeIDs(vector<string>& outData);
    ///��ȡ�����ʽ��˻���Ϣ
	int GetTradingAccountWithdrawQuota(double& fWithdrawQuota);
	int GetTradingAccountAvailable(double& fAvailable);
	int GetTradingAccountID(char* AccountID,int rltsize);
    int GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& outData);
	///��ȡ�˻��ʽ��ı���Ϣ
	int GetAccountText(std::string& outData,int language);

    ///�����ѯͶ���߽�������Ӧ
    int GetLastSettlementInfo(std::string& outData);


    ///��ȡָ����Լ�ĳֲ���ϸ     add zhifa.gao	
    bool HavePositionDetail(const std::string& strInstrumentID);
    bool HaveCombPositionDetail();
    void GetDerivedInstrumentID_OnCloseTrade(set<string>& InstrumentIDs);

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
    int GetPositionCombs2(const std::string& strInstrument,std::vector<PlatformStru_Position>& outData);

	int GetPositions(std::vector<PlatformStru_Position>& outData);
    int GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq);
    int GetPositionCombs(std::vector<PlatformStru_Position>& outData);


    //��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
    int GetPositionKeySet(std::set<PositionKey> &PositionKeySet);
    //��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
    int GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData);

    //��ȡ�ҵ�����
    int  GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, const char CloseType,const char HedgeFlag=THOST_FTDC_HF_Speculation);

    //��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
    int GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag=THOST_FTDC_HF_Speculation);

    ///���õ�ǰ��ʾ�ĳֲ����ݡ�1:�ֲ֣�2:�ֲ���ϸ��3:��ϳֲ�
    void SetCurrentPositionContent(int PositionContentMode);



    ///��ȡȫ������
    int GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///��ȡȫ������
    int GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///��ȡָ������
    bool GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData);
    bool GetOrder2(const string& strOrderSysID,PlatformStru_OrderInfo& outData);

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
    int GetWaitOrders3(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///��ȡָ����Լ��ص�δ�ɽ�����, �������ֹ�����еı�������Լ�ǵ���Լ�������Ƕ�Ӧ������������ú�Լ����ϱ���
    int GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///��ȡָ��δ�ɽ�����
    bool GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);


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

	bool IsSettlementInfoConfirmed();
	void GetErrorField(int& ErrorID);
	void GetErrorField(string& ErrMsg);
    //��ȡ�ͻ�ǩԼ������Ϣ
    int GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData);

	void SetReqID(int nOrderReqID); 
	long GetReqID(); 
	long GetNextReqID();
	int  GetAccountList(std::vector<string>& AcctList);
	void FillQuotData( const PlatformStru_DepthMarketData& rawQuot, QuotFiled fieldID, std::vector<std::string>& vec );

	///������ID
	const string ExchangeID2String(const string& ExchangeID);
	///���ݺ�ԼID��ý�����ID
	//���ݺ�Լ���ý�����ID������Ƿ�ϵͳ��Լ���򷵻ؿ�
	const string ExchangeName2ID(const string& name);
    //��ȡ����������
	SERVER_PARAM_CFG&  GetServerParamCfg();

	//�ж��б��ֶ��ڸ�ƽ̨�Ƿ���Ч
	bool IsFieldValid( int nModuleId, int nFieldId );
	bool IsModuleValid( int nModuleId );
	//�ж��Ƿ�֧�ֱ�ֵ
	bool HaveOrderType(UIOrderType type);
	//�ж��Ƿ�֧������ת��
	bool HaveBankFutureTransfer(){return false;}

    //��ȡ������Ϣ
    vector<string> GetCurrencys(const string& BankName);

	// ��ȡ��Լ�ļ�Ʊ�ṹ��so as "CU,ͭ,t"
	void GetContractShortNames(string& strShortNames);
	//
	string GetVersionIconString();
	//�Ƿ�֧�ֲ��
	bool SupportPlugins();
	//bool IsMultipleAccounts();
    ///��ȡ�˺���Ϣ
      int GetTrader(std::vector<PlatformStru_Trader> &outData);
    ///��ȡ��������Ϣ
     int GetExchange(std::vector<PlatformStru_Exchange> &outData);
    ///��ȡ��Լ��Ϣ
     int GetContract(std::vector<PlatformStru_Contract> &outData);
    ///��ȡ��Ʒ��Ϣ
     int GetCommodity(std::vector<PlatformStru_Commodity> &outData);
	 ///��ȡ����������Ϣ
     int GetOrderType(std::vector<PlatformStru_OrderType> &outData);
	 //��ȡ�˻��ֲ�
	 bool GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi);
	 bool GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi);
	 bool GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi);
   //�ж��Ƿ�֧�ֻ�ȡ�˺���Ϣ
    bool HaveGetTrader() {return false;}
   //�ж��Ƿ�֧��ȡ��������Ϣ
    bool HaveGetExchange() {return false;}
   //�ж��Ƿ�֧�ֻ�ȡ��Լ��Ϣ
    bool HaveGetContract() {return false;}
   //�ж��Ƿ�֧�ֻ�ȡ��Ʒ��Ϣ
    bool HaveGetCommodity() {return false;}
	//�ж��Ƿ�֧�ֻ�ȡ����������Ϣ
    bool HaveGetOrderType() {return false;}
	//�ж��Ƿ�֧�ֻ�ȡ�ֲ���Ϣ
	bool HaveGetPosition() {return false;}
	//�ж��Ƿ�֧�ֻ�ȡ������Ϣ
	bool HaveGetReport() {return false;}
	//�ж��Ƿ�֧�ֻ�ȡ������Ϣ
	bool HaveGetExchangeRate(){return false;}
	bool HaveShowInstumentComboList() {return true;};
	//�ж��Ƿ�֧���ڵ�¼ʱ�������޸�����
    bool HaveModifyPwdOnLongon(){return false;}
    bool HaveAccountText(){return true;}
       //��Լ���ͺ�ԼID�Ƿ���ȫһ��
    bool ContractNameIsID(){return false;}
       //��֤���Ƿ��ں�Լ��Ϣ��ؽṹ����
    bool MarginRatioInInstrumentInfo(){return false;}
public:

    /*--------------------------------------------------------------------------------------------------------
               ����ҵ���������첽�ģ��趨�ƾ���ҵ�����ݣ������յ�������
      ---------------------------------------------------------------------------------------------------------*/
    ///�û������������
    int ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate &pUserPasswordUpdate, int nRequestID=0);

    ///�ʽ��˻������������
    int ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate &pTradingAccountPasswordUpdate, int nRequestID=0);

    ///����¼������. ����ʱpInputOrder�е�RrequestID��BrokerID��InvestorID��UserID��OrderRef��ֵ���޸�Ϊ�ײ�ȷ����ֵ
    int ReqOrderInsert(PlatformStru_InputOrder &pInputOrder, int nRequestID=0);

    ///Ԥ��¼������. ����ʱpInputOrder�е�RrequestID��BrokerID��InvestorID��UserID��OrderRef��ֵ���޸�Ϊ�ײ�ȷ����ֵ
    int ReqParkedOrderInsert(PlatformStru_ParkedOrder &pParkedOrder, int nRequestID=0);

    ///Ԥ�񳷵�¼������
    int ReqParkedOrderAction(PlatformStru_ParkedOrderAction &pParkedOrderAction, int nRequestID=0);

	///������������(������)
    int ReqOrderAction(PlatformStru_InputOrderAction &pInputOrderAction, int nRequestID=0);
	///������������(����)
	virtual  int ReqCancelOrder(const OrderKey& orderkey, int nRequestID=0);

    ///��ѯ��󱨵���������
    int ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume &pQueryMaxOrderVolume, int nRequestID=0);

    ///Ͷ���߽�����ȷ��
    int ReqSettlementInfoConfirm() ;

    ///����ɾ��Ԥ��
    int ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder &pRemoveParkedOrder, int nRequestID=0) ;

    ///����ɾ��Ԥ�񳷵�
    int ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction &pRemoveParkedOrderAction, int nRequestID=0) ;

    //��ѯ�����Ƿ�Ϊ��
    bool IsQryQueueEmpty(void);


    ///�����ѯ����
    int ReqQryOrder(PlatformStru_QryOrder &pQryOrder, int nRequestID=0) ;

    ///�����ѯ�ɽ�
    int ReqQryTrade(PlatformStru_QryTrade &pQryTrade, int nRequestID=0) ;

    ///�����ѯͶ���ֲ߳�
    int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition &pQryInvestorPosition, int nRequestID=0) ;

    ///�����ѯ�ʽ��˻�
    int ReqQryTradingAccount(PlatformStru_QryTradingAccount &pQryTradingAccount, int nRequestID=0) ;

    //�����ѯ���������ָ�����͵Ĳ�ѯ��������TCP��������ز�ǰ�Ĵ���
    //ReqCmdID:1:�ʽ��ѯ��2:�ɽ���ѯ��4:������ѯ��8:�ֲ���ϸ��ѯ��16:�ֲֲ�ѯ��32:��ϳֲ���ϸ��ѯ
    //ReqCmdID���Ի���
    void ClearReqInQryQueue(int ReqCmdID);

    ///�����ѯͶ����
    int ReqQryInvestor(PlatformStru_QryInvestor &pQryInvestor, int nRequestID=0) ;


    ///�����ѯ��Լ
    int ReqQryInstrument(PlatformStru_QryInstrument &pQryInstrument, int nRequestID=0) ;

    ///�����ѯ����
    int ReqQryDepthMarketData(PlatformStru_QryDepthMarketData &pQryDepthMarketData, int nRequestID=0) ;

    ///�����ѯͶ���߽�����
    int ReqQrySettlementInfo(PlatformStru_QrySettlementInfo &pQrySettlementInfo, int nRequestID=0) ;

    ///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
    int ReqQryCFMMCTradingAccountKey(PlatformStru_QryCFMMCTradingAccountKey &QryCFMMCTradingAccountKey, int nRequestID=0); 

    ///�����ѯת������
    int ReqQryTransferBank(PlatformStru_QryTransferBank &pQryTransferBank, int nRequestID=0) ;

    ///�����ѯͶ���ֲ߳���ϸ
    int ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail &pQryInvestorPositionDetail, int nRequestID=0) ;

    ///�����ѯ�ͻ�֪ͨ
    int ReqQryNotice(PlatformStru_QryNotice &pQryNotice, int nRequestID=0) ;

    ///�����ѯ������Ϣȷ��
    int ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm &pQrySettlementInfoConfirm, int nRequestID=0) ;

    ///�����ѯͶ���ֲ߳���ϸ
    int ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail &pQryInvestorPositionCombineDetail, int nRequestID=0) ;

    ///�����ѯת����ˮ
    int ReqQryTransferSerial(PlatformStru_QryTransferSerial &pQryTransferSerial, int nRequestID=0) ;

    ///�����ѯǩԼ����
    int ReqQryContractBank(PlatformStru_QryContractBank &pQryContractBank, int nRequestID=0) ;
        
    ///�����ѯ����ǩԼ��ϵ
	int ReqQryAccountregister(PlatformStru_QryAccountRegister &pQryAccountregister, int nRequestID=0);

    ///�����ѯ����
    int ReqQryExchangeRate(PlatformStru_QryExchangeRate &QryExchangeRate, int nRequestID=0);

    ///�����ѯԤ��
    int ReqQryParkedOrder(PlatformStru_QryParkedOrder &pQryParkedOrder, int nRequestID=0) ;

    ///�����ѯԤ�񳷵�
    int ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction &pQryParkedOrderAction, int nRequestID=0) ;


    ///�ڻ����������ʽ�ת�ڻ�����
    int ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer &pReqTransfer, int nRequestID=0) ;

    ///�ڻ������ڻ��ʽ�ת��������
    int ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer &pReqTransfer, int nRequestID=0) ;

    ///�ڻ������ѯ�����������
    int ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount &pReqQueryAccount, int nRequestID=0) ;

	//void SetModuleDataQry(bool bQry);
    //bool GetModuleDataQry();
    //��������
	void ReConnetServer();
	//��ȡ��¼״̬ԭ��
	std::string GetLogonReason();
	void GetLinkStatus(int * pHost,int * pPrice);
	//��ȡ����
	int GetReports(map<DWORD, CTPReport>& mapReport);
	double GetExchangeRate(char* currency);
	bool IsSunGardSupported(){return true;}
	bool IsHistorySettlementSupported(){return true;}
	//CTP֧���ļ������Լ�������ѣ���֤��
	bool HaveFileCache(){return true;}
    //֪ͨ: ��ʼ����ѯ��ʼ/�ɹ�/ʧ��
    void NotifyInitQryStart();
    void NotifyInitQrySucc();
    void NotifyInitQryFail();
    bool IsInitQrySucc();


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
	int GetPositionCombInfo(long lFTID, PlatformStru_Position& outData);
	//int GetPositionCombInfo(PositionCombDetailKey2 key, PlatformStru_PositionCombDetail2& outData);

    //��ȡ�������ӵ�FrontID��SessionID��������ֵ�ڽ��׵�¼ʱ��ctp����
    void GetTradingLoginInfo(int& FrontID,int& SessionID,string& UserID);

	int GetGroupMaxContractNum();
	//��Լ�������ʲ���Ҫ�ر���
	bool ContractCountIsFit();

	//bool IsCurrentAccount();
	//void SetCurrentAccount(bool bCurrent);

	int  GetPlatformType();
	//bool GetQryFinished();
	//void SetQryFinished(bool bFinished);

public:
 //   static CServiceProxy& GetSingleInstance();
	//static void Release();
    ~CServiceProxy();

    CServiceProxy(const string& strAccount,int PlatformID,int MaxPlatformID);
private:
    //static CServiceProxy* s_mInstance;
    bool m_isLogin;
	bool  m_QryFinished;
	int  m_qrystep;
    zqWriteLog*           m_pWriteLog;
    CQryQueue* m_pQryQueue;
	//bool m_bCurrentAccount;

public:
    bool m_bInitQryStart;                            //���ڽ��г�ʼ����ѯ
    bool m_bInitQrySucc;                             //��ʼ����ѯ�ɹ�
    bool m_bInitQryFail;                             //��ʼ����ѯʧ��

protected:
	CPlatFormService* m_PlatFormService;
};

