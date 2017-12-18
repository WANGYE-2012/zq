#pragma once
#include <string>
#include <set>
#include <map>
#include <list>
#include <vector>
#include "../inc/ISvr/BusinessData.hpp"
#include "../inc/ISvr/PlatformStruct.h"
#include "../inc/ISvr/IPlatformSvr.h"
#ifdef _DEBUG
	#pragma comment(lib, "CmeCoreD.lib")
#else
	#pragma comment(lib, "CmeCore.lib")
#endif
class CPlatFormService;
class CCmeApiSvr:public IPlatformSvr
{
public:
	CCmeApiSvr(void);
	~CCmeApiSvr(void);
public:
    ///����ƽ̨
    int Start();

    ///�ر�ƽ̨
    int Stop();

	///�����Ƿ��ǵ�ǰ����
	///1���ǣ�����������
	int IsValidPW(const std::string& yourPW);
	int GetBrokerIDUserID(std::string& UserID, std::string& BrokerID, set<string>* accounts=NULL);
    ///���ý���ǰ�û���ַ
    int SetTradeFrontAddr(const std::string& addr);
    int SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr);

    ///��������ǰ�û���ַ
    int SetQuotFrontAddr(const std::string& addr);
    int SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr);

    ///���þ��͹�˾������Ϣ
    int SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, const std::string& UserPW, const std::string& OneTimePassword);

    ///����
	int Login(const std::string& InvestorID, const std::string& InvestorPW, const std::string& InvestorNewPwd = "");

    ///�ǳ�
    int Logout();

    ///����ҵ������
    int SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo=false);

    ///ȡ��ҵ�����ݵĶ���
    int UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID);

    ///��ȡ�ײ����ĵ�ǰ״̬
    ServiceStatus GetCurrentStatus(int* pQuotStatus=NULL,int* pTradeStatus=NULL);

    ///��ȡ��Լ�б�
    int GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID="");
    int GetInstrumentList(std::vector<GroupInfo> &outData);
    ///���������Լ�б�
    int AddMainInstrumentList(std::string instrument);
    ///��ȡ������Լ�б�
    int GetMainInstrumentList(std::set<std::string> &outData);
	// ���ú�Լ�ݲ��б�
	void SetInstrumentVarietyMap(map<string, string>& inData);
	// ��ȡ��Լ�ݲ��б�
	int GetInstrumentVarietyMap(map<string, string>& outData);
    //���ú�Լ����״̬
	void SetSubscribeStatus(const std::string& InstrumentID,int status,S_Status oper=S_STATUS_ADD);
	//���º�Լ����״̬
	int UpdateSubscribeStatus();
    ///��ȡָ����Լ��Ϣ
    int GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
    ///����ָ����Լ��Ϣ
    void SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
    ///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
    char GetProductClassType(const std::string& InstrumentID);

    ///��ȡ��ԼƷ�ִ���
    int GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID="");    

    ///��ȡ��Լ��������
    int GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
    ///��ȡ��Լ�������ʣ����û�У��򴥷���ѯ
    int GetCommissionRate_TriggerReq(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
    ///���ú�Լ��������
    int SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);  
    ///��ȡ��Լ��֤����
    int GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
    ///��ȡ��Լ��֤���ʣ����û�У��򴥷���ѯ
    int GetMarginRate_TriggerReq(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
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
    ///��ȡָ����Լ���ּۣ�ʧ�ܷ���0.0
    double GetCurPrice(const std::string& InstrumentID);


    //��ȡ��������������
    string GetTradingDay();
    ///��ȡ������ʱ��
    unsigned long GetExchangeTime(const string& ExchangeID,string& time);
    ///��ȡȫ��������
    int GetExchangeIDs(vector<string>& outData);
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

	///��ȡ����ί�е���Ϣ
    int GetAllOrdersEx(std::map <AllOrderKey,  PlatformStru_OrderInfo> &AllOrdersMap);

    ///��ȡָ����Լ�ĳֲ���ϸ     add zhifa.gao	
    int GetPositionDetails(std::multimap<std::string, PositionDetailStru> &PositionDetailMap);
    int GetPositionDetails(const std::string& InstrumentID,std::multimap<std::string, PositionDetailStru> &PositionDetailMap);
    int GetPositionDetails(std::list<PositionDetailStru> &pPositionDetailList);
    //��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
    int GetPositionKeyList(std::list<PositionKey> &PositionKeyList);
    //��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
    int GetPositionKeySet(std::set<PositionKey> &PositionKeySet);
    //��ȡָ���˺š���Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
	int GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PositionStru& PositionData);

    //��ȡ�ҵ�����
    int  GetWaitOrderVolume(const std::string &strInstrumentID, const char Direction, const char CloseType,const char HedgeFlag=THOST_FTDC_HF_Speculation);

    //��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
    int GetCloseOrderVolume(const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag=THOST_FTDC_HF_Speculation);


    ///��ȡȫ������
    int GetAllOrders(std::map<OrderKey,  PlatformStru_OrderInfo>& outData);

    ///��ȡȫ���ɽ���
    int GetAllTrades(std::map<TradeKey,  PlatformStru_TradeInfo>& outData);
    ///��ȡ�ɽ�ͳ��
    int GetAllTradeTotalDatas(vector<TotalInfo>& outData);
	///�ڳɽ�ͳ�Ʋ��Ҳ���rawData
	int FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData );
	int InitCombRelation();
	bool IsSettlementInfoConfirmed();
	void GetErrorField(int& ErrorID);
	void GetErrorField(string& ErrMsg);

	void SetReqID(int nOrderReqID); 
	long GetReqID(); 
	long GetNextReqID();
	int  GetAccountList(std::vector<string>& AcctList);
	void FillQuotData( const PlatformStru_DepthMarketData& rawQuot, QuotFiled fieldID, std::vector<std::string>& vec );

	///������ID
	const string ExchangeID2String(const string& ExchangeID);
	//���ݺ�Լ���ý�����ID������Ƿ�ϵͳ��Լ���򷵻ؿ�
	const string ExchangeName2ID(const string& name);
	//
	SERVER_PARAM_CFG&  GetServerParamCfg();

	//�ж��б��ֶ��ڸ�ƽ̨�Ƿ���Ч
	bool IsFieldValid( int nModuleId, int nFieldId );
	//��ȡ��ǰ����ģ��ID����
	int  GetModuleValid(vector<int>& ModuleIds);
	bool IsModuleValid( int nModuleId );
	//�ж��Ƿ�֧�ֱ�ֵ
	bool HaveOrderType(UIOrderType type);
	//�ж��Ƿ�֧������ת��
	bool HaveBankFutureTransfer(){return false;}
	// ��ȡ��Լ�ļ�Ʊ�ṹ��so as "CU,ͭ,T"
	void GetContractShortNames(string& strShortNames);
	//
	string GetVersionIconString();
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
   //
   bool GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi);
   bool GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi);
   bool GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi);

   //�ж��Ƿ�֧�ֻ�ȡ�˺���Ϣ
    bool HaveGetTrader() {return true;}
   //�ж��Ƿ�֧��ȡ��������Ϣ
    bool HaveGetExchange() {return true;}
   //�ж��Ƿ�֧�ֻ�ȡ��Լ��Ϣ
    bool HaveGetContract() {return false;}
   //�ж��Ƿ�֧�ֻ�ȡ��Ʒ��Ϣ
    bool HaveGetCommodity() {return true;}
	//�ж��Ƿ�֧�ֻ�ȡ����������Ϣ
    bool HaveGetOrderType() {return true;}
	//�ж��Ƿ�֧�ֻ�ȡ�ֲ���Ϣ
	bool HaveGetPosition(){return true;}
	//�ж��Ƿ�֧�ֻ�ȡ������Ϣ
	bool HaveGetReport(){return true;}
	//�ж��Ƿ�֧�ֻ�ȡ������Ϣ
	bool HaveGetExchangeRate(){return true;}
	//�ж��Ƿ�֧���ڵ�¼ʱ�������޸�����
	bool HaveModifyPwdOnLongon() {return false;}
	bool HaveShowInstumentComboList() {return false;};
    bool HaveAccountText(){return false;}
       //��Լ���ͺ�ԼID�Ƿ���ȫһ��
    bool ContractNameIsID(){return true;}
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

    ///����¼������
    int ReqOrderInsert(PlatformStru_InputOrder &pInputOrder, int nRequestID=0);

    ///Ԥ��¼������
    int ReqParkedOrderInsert(PlatformStru_ParkedOrder &pParkedOrder, int nRequestID=0);

    ///Ԥ�񳷵�¼������
    int ReqParkedOrderAction(PlatformStru_ParkedOrderAction &pParkedOrderAction, int nRequestID=0);

    ///������������
    int ReqOrderAction(PlatformStru_InputOrderAction &pInputOrderAction, int nRequestID=0);

    ///��ѯ��󱨵���������
    int ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume &pQueryMaxOrderVolume, int nRequestID=0);

    ///Ͷ���߽�����ȷ��
    int ReqSettlementInfoConfirm() ;

    ///����ɾ��Ԥ��
    int ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder &pRemoveParkedOrder, int nRequestID=0) ;

    ///����ɾ��Ԥ�񳷵�
    int ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction &pRemoveParkedOrderAction, int nRequestID=0) ;

    ///�����ѯ����
    int ReqQryOrder(PlatformStru_QryOrder &pQryOrder, int nRequestID=0) ;

    ///�����ѯ�ɽ�
    int ReqQryTrade(PlatformStru_QryTrade &pQryTrade, int nRequestID=0) ;

    ///�����ѯͶ���ֲ߳�
    int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition &pQryInvestorPosition, int nRequestID=0) ;

    ///�����ѯ�ʽ��˻�
    int ReqQryTradingAccount(PlatformStru_QryTradingAccount &pQryTradingAccount, int nRequestID=0) ;
	//int  GetTradingAccount(PlatformStru_TradingAccountInfo& Data);

    ///�����ѯͶ����
    int ReqQryInvestor(PlatformStru_QryInvestor &pQryInvestor, int nRequestID=0) ;


    ///�����ѯ��Լ
    int ReqQryInstrument(PlatformStru_QryInstrument &pQryInstrument, int nRequestID=0) ;

    ///�����ѯ����
    int ReqQryDepthMarketData(PlatformStru_QryDepthMarketData &pQryDepthMarketData, int nRequestID=0) ;

    ///�����ѯͶ���߽�����
    int ReqQrySettlementInfo(PlatformStru_QrySettlementInfo &pQrySettlementInfo, int nRequestID=0) ;

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

    ///��������
    int SubscribeQuot(const std::string &InstrumentID);

    ///ȡ������
    int UnSubscribeQuot(const std::string &InstrumentID);
	//
	void _SetInitQryStep(int step){m_qrystep=step;}
	//��ȡ��ѯ����
	bool GetInitQryStatus(int status){return status<m_qrystep;}
    //��������
	void ReConnetServer();
	//��ȡ��¼״̬ԭ��
	std::string GetLogonReason();
	void GetLinkStatus(int* pHost,int* pPrice);
	//��ȡ����
	int GetReports(map<DWORD, Report>& mapReport);
	double GetExchangeRate(char* currency);
	bool IsSunGardSupported(){return false;}
	bool IsHistorySettlementSupported(){return false;}
	//��ȡ��Լ����Ϻ�Լ�Ĺ�ϵ��
	int GetCombineRelation(std::multimap<std::string,std::string>& mapCombineRelation){return 0;}
	//��ȡ��ϳֲ��б�
	virtual int GetComboPositions(std::map<pair<std::string, char>, CombinPositionStru>& outData){return 0;}
private:
    bool isLogin;
	int  m_qrystep;
	string	m_strContractShortName;
protected:
	CPlatFormService* m_PlatFormService;
};
