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
using std::multimap;
using std::map;
using std::string;
using std::set;
using std::vector;


#include "ctp/ThostFtdcTraderApi.h"
#include "ctp/ThostFtdcMdApi.h"

#include "TraderSpi.hpp"
#include "QuotSpi.hpp"
#include "../Module-Misc2/BusinessData.hpp"
#include "platformSvr/ServiceProxy.hpp"
//#include "../Module-Misc2/PlatformStruct.h"



class CPackageCache;
class CDataCenter;
class zqWriteLog;


//���涨�彻��������״̬
enum CTPCONNSTATUS
{    
    CTPCONNSTATUS_Disconnected   =      0       ,            //δ����
    CTPCONNSTATUS_Connecting     =      1       ,            //��������
    CTPCONNSTATUS_Logining       =      2       ,            //���ڵ�¼
    CTPCONNSTATUS_LoginFailure   =      3       ,            //��¼ʧ��
    CTPCONNSTATUS_TradeSettlementInfoConfirming     =   4 ,  //���ڽ��н��׽�����ȷ��
    CTPCONNSTATUS_Connected      =      6       ,            //���ӵ�¼�ɹ�
    CTPCONNSTATUS_Disconnecting  =      7       ,            //���ڶϿ�����
    CTPCONNSTATUS_UnInit         =      8                    //δ��ʼ��
};

class CPlatFormParam
{
public:
    
public:
    CPlatFormParam()
    {
        TradeStatus=CTPCONNSTATUS_Disconnected;
        QuotStatus =CTPCONNSTATUS_Disconnected;
        RequestID=0;
		//PlatFormID = 0;
    };
    
public:
    std::vector<std::string>    TradeFrontAddrs;    //���ƶ��"asp-sim2-front1.financial-trading-platform.com:26205"
    std::vector<std::string>    QuotFrontAddrs;     //���ƶ��"58.246.49.130:41213"
    std::string                 BrokerID;
    std::string                 UserID;
    std::string                 UserPW;
    std::string                 InvestorID;      
    std::string                 InvestorPW;
    CTPCONNSTATUS               TradeStatus;
    CTPCONNSTATUS               QuotStatus;
    int                         RequestID;
	std::string                 OneTimePassword;
    Stru_ProxyServerParam       ProxyParam;
	//int						    PlatFormID;
};

class CPlatFormService
{
friend class CTraderSpi;
friend class CQuotSpi;
friend class CTradeThread;
friend class CQuotThread;
friend class CServiceProxy;

//���׽ӿ�
public:
	///�û������������
	int ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate *pUserPasswordUpdate, int nRequestID=0);

	///�ʽ��˻������������
	int ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate *pTradingAccountPasswordUpdate, int nRequestID=0);

	///����¼������. ����ʱpInputOrder�е�RrequestID��BrokerID��InvestorID��UserID��OrderRef��ֵ���޸�Ϊ�ײ�ȷ����ֵ
	int ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nRequestID=0);

	///Ԥ��¼������. ����ʱpInputOrder�е�RrequestID��BrokerID��InvestorID��UserID��OrderRef��ֵ���޸�Ϊ�ײ�ȷ����ֵ
	int ReqParkedOrderInsert(PlatformStru_ParkedOrder *pParkedOrder, int nRequestID=0);

	///Ԥ�񳷵�¼������
	int ReqParkedOrderAction(PlatformStru_ParkedOrderAction *pParkedOrderAction, int nRequestID=0);

	///������������(������)
	int ReqOrderAction(PlatformStru_InputOrderAction *pInputOrderAction, int nRequestID=0);
	///������������(����)
	int ReqCancelOrder(const OrderKey& orderkey, int nRequestID=0);

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

#ifdef CTP060300
	///�����ѯ��Ȩ��Լ������
	int ReqQryOptionInstrCommRate(CThostFtdcQryOptionInstrCommRateField *pQryOptionInstrCommRate, int nRequestID);
    ///�����ѯ��Ȩ���׳ɱ�
	int ReqQryOptionInstrTradeCost(CThostFtdcQryOptionInstrTradeCostField *pQryOptionInstrTradeCost, int nRequestID);
#endif
    ///����ѯ��֪ͨ��OptionInstrumentIDsΪ������ȫ����Ȩ��Լ��ѯ��֪ͨ
    int  SubscribeForQuoteRsp(const vector<string>& OptionInstrumentIDs);
    ///�˶�ѯ��֪ͨ��OptionInstrumentIDsΪ�����˶�ȫ����Լ��ѯ��֪ͨ
    int UnSubscribeForQuoteRsp(const vector<string>& OptionInstrumentIDs);
    ///����ѯ��¼��
    int ReqForQuoteInsert(PlatformStru_InputForQuoteField& InputForQuote, int nRequestID);
    ///ִ������¼������
	int ReqExecOrderInsert(PlatformStru_InputExecOrderField& InputExecOrder, int nRequestID);
    ///ִ�������������
	int ReqExecOrderAction(PlatformStru_InputExecOrderActionField& InputExecOrderAction, int nRequestID);
    ///�����ѯִ������
	int ReqQryExecOrder(PlatformStru_QryExecOrderField& QryExecOrder, int nRequestID);

	///�����ѯ������
	int ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID=0) ;

	///�����ѯƷ��
	int ReqQryProduct(PlatformStru_QryProduct *pQryProduct, int nRequestID=0) ;

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

    ///�����ѯ����
    int ReqQryExchangeRate(PlatformStru_QryExchangeRate *pQryExchangeRate, int nRequestID);

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
	bool IsValidInvestorPW(const string& yourPW);
	bool IsValidUserPW(const string& yourPW);
	int GetBrokerIDUserID(std::string& UserID, std::string& BrokerID,set<string>* accounts=NULL);
    int SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr);
    int SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr);
    int SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, const std::string& UserPW, const std::string& OneTimePassword);   
	int SetProxyParam(const Stru_ProxyServerParam& ProxyParam);
//�Զ���ӿ�
public:
    ///����ҵ������
    int SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo=false);

    ///ȡ��ҵ�����ݵĶ���
    int UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID);

    ///��ȡ��Լ�б�
    int GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID="");
    int GetInstrumentList(vector<string> &outData,string ExchangeID="");
	int GetInstrumentList(std::vector<InstrumentGroupInfo> &outData);
    ///���������Լ�б�
    void AddMainInstrumentList(std::string instrument);
    ///��ȡ������Լ�б�
    int GetMainInstrumentList(std::set<std::string> &outData);
	///���ú�Լ�ݲ��б�
	void SetInstrumentVarietyMap(map<string, string>& inData);
	// ��ȡ��Լ�ݲ��б�
	int GetInstrumentVarietyMap(map<string, string>& outData);
	///��ȡָ��Ʒ�ֵĺ�Լ�б�,���غ�Լ����
	int GetInstrumentListByProductID(const string& ProductID,set<string> &outData);
	int GetInstrumentListByProductID(const string& ProductID,vector<string> &outData);
    ///��ȡָ����Լ��Ϣ
    int GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
    bool GetLegInstrumentID(const string& strComInstrumentID,
        string& strLeg1InstrumentID,
        string& strLeg2InstrumentID);
    ///����ָ����Լ��Ϣ
    void SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData,bool bLast);
    ///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
    char GetProductClassType(const std::string& InstrumentID);
    ///��ȡ��Լ��ProductID
    string GetProductID(const string& strInstrumentID);
    string GetExchangeID(const string& strInstrumentID);

    ///����ָ��Ʒ����Ϣ
    void SetProductInfo(const string& ProductID, PlatformStru_ProductInfo& outData);
    ///��ȡƷ���б�
    int GetProductList(vector<string> &outData,const string& ExchangeID="");

    ///��ȡ�����˵��Ⱥ�Լ����Ϻ�Լ�б�
    bool GetCombInstrumentIDs_IncludeLeg(const std::string& LegInstrument,std::vector<std::string>& vecCombInstruments);
    //���ú�Լ����״̬
	void SetSubscribeStatus(const std::string& InstrumentID,int GID,SubscribeMarketDataOper oper=eSubscribeMarketData);
	//�˶�GID��Ӧ��ȫ����Լ����
	void UnsubscribeMarketDataOfGID(int GID);
	//���GID�Ƿ�����ָ����Լ������
	bool IsSubscribeMarketDataOfGID(const string& InstrumentID,int GID);
    ///��ȡָ��������������Ʒ��-��Լ
    int GetProductID_InstrumentIDsByExchangeID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID="");    
    ///��ȡȫ����������Ʒ�ֺ�ԼID vector<pair<ExchangeID,vector<pair<ProductID,vector<InstrumentID>>>>>
    void GetExchangeID_ProductID_InstrumentIDs(vector<pair<string,vector<pair<string,vector<string>>>>>& outData);
    ///��ȡȫ����������Ʒ����Ϣ vector<pair<ExchangeID,vector<PlatformStru_ProductInfo>>>
    void GetExchangeID_ProductInfos(vector<pair<string,vector<PlatformStru_ProductInfo>>>& outData);
    bool GetProductInfo(const string& strInstrumentID,PlatformStru_ProductInfo& outData);
    bool GetProductInfo2(const string& strProductID,PlatformStru_ProductInfo& outData);
    //��ȡָ������������Ȩ������ڻ�Ʒ��
    int GetOptionProductIDsByExchangeID(const string& ExchangeID,vector<string>& outProductIDs);
    //��ȡָ��������ڻ���Լ��Ӧ����Ȩ��ԼID������Ŀ����Ȩ��Լ��������UnderlyingInstrumentIDΪ���򷵻�ȫ����Ȩ��Լ
    int GetOptionInstrumentIDsByUnderlyingInstrumentID(const string& UnderlyingInstrumentID,vector<string>& outOptionInstrumentIDs);
    //��ȡָ��������ڻ���Լ��Ӧ�Ŀ�����Ȩ��ԼID����Ϳ�����Ȩ��ԼID���顣UnderlyingInstrumentIDΪ���򷵻�ȫ����Ȩ��Լ
    void GetOptionInstrumentIDsByUnderlyingInstrumentID(const string& UnderlyingInstrumentID,vector<string>& outCallOptionInstrumentIDs,vector<string>& outPutOptionInstrumentIDs);
    //��ȡ���б�����ڻ���Լ
    int GetAllOptionUnderlyingInstrumentIDs(vector<string>& outUnderlyingInstrumentIDs);
    int GetAllOptionUnderlyingInstrumentIDs(set<string>& outUnderlyingInstrumentIDs);
    //�ж��Ƿ�����Ȩ������ڻ���Լ
    bool IsUnderlyingInstrumentID(const string& InstrumentID);

    ///��ȡ��Լ��������
    int GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
    bool IsCommissionRateExist(const string& InstrumentID);
    ///���ú�Լ��������
    int SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);  
    ///�������ڲ�ѯ�������ʵĺ�Լ
    void SetReqCommissionRateInstrumentID(const std::string& InstrumentID);
    ///��ȡ��Լ��֤����
    int GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
    bool IsMarginRateExist(const string& InstrumentID);
    ///��ȡ��Լ��֤����
    int SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);  
    ///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
    int GetInstrumentMultiple(const std::string& InstrumentID);


    ///��ȡָ����Լ����
    int GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
    ///��ȡָ����Լ������
    int GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
    ///��ȡָ����Լ���ּۣ�ʧ�ܷ�����Чֵ
    double GetCurPrice(const std::string& InstrumentID);


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

    ///���õ�ǰ��ʾ�ĳֲ����ݡ�1:�ֲ֣�2:�ֲ���ϸ��3:��ϳֲ�
    void SetCurrentPositionContent(int PositionContentMode);

	string GetTradingDay();
	bool IsTodayPosition( const char* pOpenData );
	
    ///��ȡ������ʱ��
    unsigned long GetExchangeTime(const string& ExchangeID,string& time);
    ///��ȡ���������ڽ���ʱ��, ���ؽ���ʱ����ʼʱ��Ե����飬��{pair("0915","1130"),pair("1300","1515")}��
    vector<pair<string,string>> GetTradingTimespan(const string& ExchangeID);
    ///��ȡȫ��������
    int GetExchangeIDs(vector<string>& outData);
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


    int GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag=THOST_FTDC_HF_Speculation);
    //��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
    int GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag=THOST_FTDC_HF_Speculation);

    ///��ȡȫ���ɽ���
    int GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData);
    ///��ȡָ���ɽ���Ϣ
    bool GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData);
    ///��ȡָ���ɽ���Ϣ
    int GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData);
    int GetTradeBriefsOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfoBrief>& outData);
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
	int GetTradingAccountID(char* AccountID,int rltsize);
    int GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& outData);
	///��ȡ�˻��ʽ��ı���Ϣ
    int GetAccountText(std::string& outData,int language);

    ///�����ѯͶ���߽�������Ӧ
    int GetLastSettlementInfo(std::string& outData);
    void GetMarketDataInstrumentID(std::set<std::string>& setMarketDataInstrumentID);
	void SetOrderRef(int orderref){m_CurOrderRef=orderref;}
    //��ȡ�ͻ�ǩԼ������Ϣ
    int GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData);

    //��ȡ������Ϣ
    vector<string> GetCurrencys(const string& BankName,bool bAddUSD=true,bool bAddCNY=true);
    vector<string> GetCurrencys(bool bAddUSD=true,bool bAddCNY=true);

    CTPCONNSTATUS GetTradeStatus(void);


    CTPCONNSTATUS GetQuotStatus(void);


    std::string GetTradeBrokerID(void);
    std::string GetTradeInvestorID(void);
	//int GetPlatFormID();

    //�ж��Ƿ���Ҫ��ѯ�º�Լ�ķ���
    bool IsNeedCheckCommissionRateAndMarginRate(void);

    //�����Ҫ��ѯ�º�Լ���ʵı�־
    void SetNeedCheckCommissionRateAndMarginRate(bool bNeedCheck);
	//

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
    //void SetModuleDataQry(bool bQry);
    //bool GetModuleDataQry();

    //UI��֪ͨ�ײ㣬��ʼ����ѯ��ʼ�����
    void NotifyInitQryStart();
    void NotifyInitQrySucc();

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

    //��ȡ�������ӵ�FrontID��SessionID��������ֵ�ڽ��׵�¼ʱ��ctp����
    const PlatformStru_LoginInfo& GetTradingLoginInfo();

    ///DataCenter�Ƿ��Ѿ��͵ײ�ͬ���ϣ����Կ�ʼ��ʼ����ѯ��
    bool IsDataCenterReady(void);

private:
    CPlatFormService(CServiceProxy& rServiceProxy);
    ~CPlatFormService();    


    ///�û���¼����
	int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID=0);	

	///�ǳ�����
	int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID=0);

    zqWriteLog*              m_pWriteLog;

    CThostFtdcTraderApi*    m_pTradeApi;    //���׽ӿڶ������ۺϽ���ƽ̨�Ľӿ��ṩ
    CTraderSpi*             m_pTradeSpi;    //���׽ӿڻص�����
    CThostFtdcMdApi*        m_pQuotApi;     //����ӿڶ������ۺϽ���ƽ̨�Ľӿ��ṩ
    CQuotSpi*               m_pQuotSpi;     //����ӿڻص�����
    CTradeThread*           m_pTradeWorkThread; //���׹����߳�
    CQuotThread*            m_pQuotWorkThread;  //���鹤���߳�
    CPackageCache*          m_PCache;       //CTraderSpi��CQuotSpi���������ķ��ص����ݱ��浽��cache�У�
                                            //CDataCenter��ȡ��cache
    CDataCenter*            m_pDataCenter;  //�������ģ������ȡm_PCache�е�ҵ�����ݣ������ݶ�����Ϣ
                                            //���ö�Ӧ�Ļص�����
	int				        m_CurOrderRef;//��ǰ�������ñ��

    int                     m_CurForQuoteRef;           //��Ȩ��ѯ��Ref�ĵ�ǰ���
    int                     m_CurExecOrderRef;          //��Ȩ��ִ������Ref�ĵ�ǰ���
    int                     m_CurExecOrderActionRef;    //��Ȩ��ִ���������Ref�ĵ�ǰ���

    set<string>             m_setOptionInstrumentIDs_SubscribeForQuoteRsp;//��Ȩ����ǰ������ѯ��֪ͨ����Ȩ��ԼID

    std::set<std::string> m_MarketDataInstrumentID;
    enum PLATFORM_STATUS
    {
        PLATFORM_STATUS_UNINIT,
        PLATFORM_STATUS_INITED
    };
    int                     m_status;       //ƽ̨�ĵ�ǰ״̬

	CThostFtdcRspInfoField  m_ErrorInfoField;
	
	int						m_nOrderReqID;
	multimap<string, int>   m_SubscribeMap;       //���ĸú�Լ�����GID

    CRITICAL_SECTION m_CS;

public:
    CPlatFormParam          m_PlatformParam;//ƽ̨������
	bool					m_bSettlementInfoConfirmed;

public:
    bool                    m_bInQry;             //�Ƿ��ڲ�ѯ;��

public:
	CServiceProxy& m_rServiceProxy;


};


#endif

