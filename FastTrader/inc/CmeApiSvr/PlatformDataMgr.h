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


class CPackageCache;

class zqWriteLog;



class CPlatformDataMgr
{
public:
    CPlatformDataMgr(CPlatFormService* pHost);
    ~CPlatformDataMgr(void);

    ///���ý����ʽ��˻���Ϣ
    void SetTradingAccount(const PlatformStru_TradingAccountInfo& inData);
    ///���ý����ʽ��˻���Ϣ
    void SetTradingAccount(const DataRspQryTradingAccount& outData);
    ///��ȡ�����ʽ��˻���Ϣ
	int GetTradingAccountWithdrawQuota(double& fWithdrawQuota);
	int GetTradingAccountAvailable(double& fAvailable);
	int GetTradingAccountID(char* AccountID);
    int GetTradingAccount(PlatformStru_TradingAccountInfo& outData);
    ///��̬�����ʽ��˻���Ϣ�����ݳɽ��ر������顢�����ر������ʲ�ѯ�ر�����Լ��ѯ�ر�
    void DynamicUpdateTradingAccount(void);
    ///��̬�����ʽ��˻���Ϣ����������
    void DynamicUpdateTradingAccount_NewQuot(const std::string InstrumentID);
	void SetAccountField(DataRspQryTradingAccountEx& TradingAccountField, const CThostFtdcTradingAccountFieldEx& field );

	void SetAccountField(DataRspQryTradingAccountEx& TradingAccountField, const CThostFtdcTradingAccountField& field );

	void Assign(DataRspQryTradingAccountEx& TradingAccountField,const DataRspQryTradingAccount& data );


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



    bool IsNeedCheckCommissionRateAndMarginRate(void) { return m_bNeedCheckCommissionRateAndMarginRate;}
    void SetNeedCheckCommissionRateAndMarginRate(bool bNeedCheck) { m_bNeedCheckCommissionRateAndMarginRate=bNeedCheck;}



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



    //ԭʼ�ĳɽ��ر�����
    void OriginTradeReady( const PlatformStru_TradeInfo TradeInfo,const int ErrID,const bool bIsLast);
    //�µĳɽ��ر�����
    void NewTradeReady( const PlatformStru_TradeInfo TradeInfo);
    ///��ȡȫ���ɽ���
    int GetAllTrades(std::map<TradeKey,  PlatformStru_TradeInfo>& outData);
    ///��ȡ�ɽ�ͳ��
    int GetAllTradeTotalDatas(vector<TotalInfo>& outData);
	///�ڳɽ�ͳ�Ʋ��Ҳ���rawData
	int FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData );


    //ԭʼ��������
    void OriginOrderReady(const PlatformStru_OrderInfo& OrderInfo,const int ErrID,const bool bIsLast);
    //�±�������
    void NewOrderReady(const PlatformStru_OrderInfo& OrderInfo);
    ///��ȡȫ������
    int GetAllOrders(std::map<OrderKey,  PlatformStru_OrderInfo>& outData);
    //��ȡָ����Լ��ָ������ָ����ƽ�Ĺҵ�������������
    int GetWaitOrderVolume(const std::string &strInstrumentID, const char Direction, const char CloseType,const char HedgeFlag=THOST_FTDC_HF_Speculation);
    //��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
    int GetCloseOrderVolume(const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag=THOST_FTDC_HF_Speculation);
    
	//�±�������,��������ί�е�
	void DynamicUpdateAllOrderPanel(const PlatformStru_OrderInfo& OrderInfo);
	///��ȡ����ί�е���Ϣ
    int GetAllOrdersEx(std::map <AllOrderKey,  PlatformStru_OrderInfo> &AllOrdersMap);
	//����¼��ر�����������ί�е�
	void UpdateOrdersforOrderInsert(CThostFtdcInputOrderField &InputOrderField,CThostFtdcRspInfoField &RspInfoField);


    //ԭʼ�ֲ���ϸ��¼����
    void OriginPositionDetailReady(PlatformStru_Position PositionDetailInfo,const int ErrID,const bool bIsLast);
    //���¶�̬�ֲ���ϸ��¼�е�ӯ����ֵ���õ�������ʱ��һ��
    void UpdateProfitInDynamicPositionDetail(PlatformStru_DepthMarketData& QuotData);

    //��̬���³ֲ���ϸ��¼��ÿ�γɽ��ر��͵õ���֤����ʱ��һ��
    //����m_TradesListForUpdatePositionDetail������m_allPositionDetail
    void ProcessNewTradeList(void);
    //��ȡ�ֲ���ϸ��¼
    int GetPositionDetails(std::multimap<std::string, PositionDetailStru > &PositionDetailMap);
    int GetPositionDetails(const std::string& InstrumentID,std::multimap<std::string, PositionDetailStru > &PositionDetailMap);
    int GetPositionDetails(std::list<PositionDetailStru> &PositionDetailList);

    //��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
    int GetPositionKeyList(std::list<PositionKey> &PositionKeyList);
    //��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
    int GetPositionKeySet(std::set<PositionKey> &PositionKeySet);
    //��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
    int GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PositionStru& PositionData);





    //��ȡָ����Լ���飬�ɹ�����0��ʧ�ܷ���-1
    int GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
    //����ָ����Լ���飬���õ�������£��򷵻�true�����򷵻�false
    bool SetQuotInfo(const PlatformStru_DepthMarketData& inData);
	void ConvertComboInstrumentID(const std::string strComboID, std::string& strInstrumentID1, std::string& strInstrumentID2);
    //��ȡָ����Լ�����¼�
    double GetCurPrice (const std::string& InstrumentID);
    //��ȡָ����Լ�����飬�ɹ�����0��ʧ�ܷ���-1
    int GetOldQuotInfo (const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);

    //����Ͷ������Ϣ
    void SetInvestorInfo(const DataRspQryInvestor& inData);

    string GetTradingDay();
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


    //ҵ���ƣ��ɹ�����0��ʧ�ܷ���-1
    int SubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID, const GuiCallBackFunc& callback);
    ///ȡ��ҵ�����ݵĶ��ƣ��ɹ�����0��ʧ�ܷ���-1
    int UnSubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID);
    ///��ȡҵ������Ϣ���ɹ�����0��ʧ�ܷ���-1
    int GetSubscribeBusinessData(const BusinessID BID, std::list<GuiCallBackFunc>& outData);
    //��ȡ��Լ�б�󣬳�ʼ����Ϲ�ϵmap
	int InitCombRelation(std::vector<DataRtnDepthMarketData>& vec);
	//������Ϻ�Լ�����������
	int MakeComboDepthMarketData(PlatformStru_DepthMarketData *pDepthMarketData,int index=0);
	// ˢ�³ֲ���Ϣ
	void RefreshPositionData();
	//
	SERVER_PARAM_CFG&  GetServerParamCfg();
private:


    //��Լ��Ϣ�������������ݼ��ڲ�ѯ��Լ��Ϣ����еõ���������gui����
    stdext::hash_map<std::string,  PlatformStru_InstrumentInfo> m_allInstruments; //��ǰ���к�Լ��Ϣ��ϵͳ��ʼ��ʱ��ȡ
    std::set<std::string>  m_allInstrumentList;                         //��ǰ���к�Լ�б�
    std::set<std::string>  m_allMainInstrumentList;                         //��ǰ������Լ�б�
    std::map<std::string, std::set<std::string> > m_allProductID;       //���в�Ʒ����
	std::vector<GroupInfo> m_allGroupInfo;
	std::map<string, string> m_allInstrumentVarietyMap;
    bool m_bNeedCheckCommissionRateAndMarginRate;                       //�Ƿ���Ҫ����������ʺͱ�֤���ʣ������º�Լ�б�ʱ����ֵΪtrue����ѯ��󣬴�ֵΪfalse
    std::map<std::string, PlatformStru_InstrumentCommissionRate> m_InstrumentCommissionRate;//��������
    std::string m_ReqCommissionRateInstrumentID;                 //���ڲ�ѯ�������ʵĺ�Լ
    std::map<std::string, PlatformStru_InstrumentMarginRate> m_InstrumentMarginRate;//��֤����


    //�ʽ��˻���Ϣ���ʽ��˻���ϵͳ����ǰ��ѯһ�Σ�Ȼ����ݱ����ر��ͳɽ��ر���̬����
    DataRspQryTradingAccount m_TradingAccount;                          //�ʽ��˻���Ϣ


    //�ɽ���¼��Ϣ���ɽ���¼���ݼ���ϵͳ����ǰ��ѯһ�Σ�Ȼ����ݳɽ��ر���̬����
    std::map<TradeKey,  PlatformStru_TradeInfo> m_originTrades;           //��ʼ�ɽ�����ϵͳ����ʱ��ѯ�ĳɽ�����Ȼ���ٸ��¡�
    std::map<TradeKey,  PlatformStru_TradeInfo> m_allTrades;              //���гɽ�����ϵͳ����ʱ��ѯ�������ݳɽ��ر���̬����
    std::multimap<std::string,TradeKey> m_allTradeIndex;              //ȫ���ɽ���������
    std::list<PlatformStru_TradeInfo> m_TradesListWaitForAppend;          //�ȴ���ӵĳɽ����б�ͨ��OnRtnTrade�����ĳɽ���������ȴ�ԭʼ�ɽ�����ԭʼ�ֲ���ϸ�õ���������
    bool m_bGetOriginTradeData;                                       //�Ƿ��Ѿ���ȫ��ȡ����ʼ�ĳɽ���Ϣ

	vector<TotalInfo>	m_TradeTotalData;	                          // �ɽ�ͳ������


    //������¼���ݼ�
    std::map<OrderKey,  PlatformStru_OrderInfo> m_originOrders;           //��ʼί�е���ϵͳ����ʱ��ѯ�ı�����Ȼ���ٸ��¡�
    std::map<OrderKey,  PlatformStru_OrderInfo> m_allOrders;              //����ί�е���ϵͳ����ʱ��ѯ��Ȼ���汨���ر����¡������ҵ��еģ��ѳɽ��ģ��ѳ�����
    std::multimap<std::string,OrderKey> m_allOrderIndex;              //ȫ��ί�е�������
    std::list<PlatformStru_OrderInfo> m_OrdersListWaitForAppend;          //�ȴ���ӵı����б�ͨ��OnRtnOrder�����ı���������ȴ�ԭʼ�����õ���������
    bool m_bGetOriginOrderData;                                       //�Ƿ��Ѿ���ȫ��ȡ����ʼ�ı�����Ϣ


    //�ֲ����ݼ�
    std::map<PositionDetailKey,PositionDetailStru> m_allPosition;          //ȫ���ֲ���ϸ��¼��ÿ�εõ��ɽ��ر�ʱ�����ԭʼ�ֲ���ϸ�ͳɽ��ر���ϸ��̬���´˼�¼��ÿ�εõ�����ͷ���ʱ������Ӧ��ӯ������ֵ
    std::multimap<std::string,PositionDetailKey> m_allPositionIndex;       //ȫ���ֲ���ϸ������
    bool m_bGetOriginPositionDetailData;                                        //�Ƿ��Ѿ���ȫ��ȡ����ʼ�ĳֲ���ϸ��Ϣ
    long m_MaxPositionDetailID;
    
	//����ί�е����ݼ�
	std::map <AllOrderKey,  PlatformStru_OrderInfo> m_OrdersPanel;  

    //�������ݼ�
    std::map<std::string,  PlatformStru_DepthMarketData> m_lastQuot;  //��������
    std::map<std::string,  PlatformStru_DepthMarketData> m_OldQuot;  //�ϴ�������


    //�������ݼ�
    DataRspQryInvestor m_InvestorInfo;//Ͷ���߸�����Ϣ
    std::string m_LastSettlementInfo;
    bool m_bSettlementInfoIsReady;
    std::map<BusinessID, std::map<GUIModuleID, GuiCallBackFunc> >  m_subscribeInfo;
	//��Ϲ�ϵ<ID1,<ALL,ID2>>,<ID2,<ALL,ID1>>
	std::multimap<std::string,std::vector<std::string>> m_CombRelation;

    ExchangeTime m_exchangeTime; //������ʱ��

    zqWriteLog*           m_pWriteLog;

    CRITICAL_SECTION m_CS;

	SERVER_PARAM_CFG m_ServerParamCfg;
    //�ж�һ���ַ��������Ƿ��ǵ���
    bool IsTodayPosition(const char *OpenDate);

    ///��ȡָ����Լ��Ϣ���ڲ����ã����ü���
    int GetInstrumentInfo_Internal(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
    ///��ȡ��Լ��֤���ʣ��ڲ����ã����ü���
    int GetMarginRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
    ///��ȡ��Լ�������ʣ��ڲ����ã����ü���
    int GetCommissionRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);

    //ȡȫ���ҵ��Ķ��������ѺͶ��ᱣ֤���ڲ����ã��������������û�з��ʣ���0����
    void GetFreezeMargin_Commission_Internal(double& FreezeMargin,double& FreezeCommission);
    //��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ڲ����ã�������
    //�ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
    int  GetCloseOrderVolume_Internal(const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag);
    //��ȡָ����Լ��ͬ����Ĺҵ������ڲ����ã���������������Ͷ��
    void GetWaitOrderVolume_Internal(std::map<std::string,WaitOrderVolumeStru>& WaitOrderVolume);
    //�ӳɽ���ȡ�����ѡ����û�з��ʣ���0����
    void GetAllCommission_Internal(double& TotalCommission);
    //ȡȫ���ֲֵ�ռ�ñ�֤��ͳֲ�ӯ��(���ն���)�����û�з��ʣ���0����
    void GetAllMargin_PositionProfitByDate_Internal(double& TotalMargin,double&TotalPositionProfitByDate,double&TotalCloseProfitByDate);
	
	void SumTradeData(const PlatformStru_TradeInfo& rawData);
	double GetShouXuFei(const PlatformStru_TradeInfo& raw);
protected:
	CPlatFormService* m_PlatFormService;
};

