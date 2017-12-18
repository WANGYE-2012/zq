/*
    ���ڱ����ά���ײ�����
    ��������Լ���ݡ��������ݡ��ֲ���ϸ���ݡ��������ݡ��ɽ������ݡ��ʽ����ݵ�
    added by l. 20110216
*/


#pragma once

#include "easymutex.h"

#include "PlatformDataMgr_LoginInfo.h"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Rates.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Trades.h"
#include "PlatformDataMgr_TradeTotals.h"
#include "PlatformDataMgr_Orders.h"
#include "PlatformDataMgr_PositionDetailCombs.h"
#include "PlatformDataMgr_PositionCombs.h"
#include "PlatformDataMgr_PositionDetails.h"
#include "PlatformDataMgr_Positions.h"
#include "PlatformDataMgr_TradingAccount.h"


#ifdef WIN32
#pragma managed(push,off)
#endif 



class CPlatformDataMgr
{
public:
    //bUseComb                              :�Ƿ�ʹ����ϵ�����
    //DiscountDCE_Comb                      :��������ϵ��Żݲ��ԣ�0-���Żݣ�1-��һ���Żݣ�2-�ڶ����Żݣ�3-����֤��Ƚϸߵ�һ����ȡ(ȱʡ)
    //DiscountCZCE_Comb                     :֣������ϵ��Żݲ��ԣ�
    //UnwindPrioritiesCZCE_Comb             :ƽ��˳��: 0: ͳһ�ȿ���ƽ��1: �ȵ�һ������ȿ���ƽ
    //UnwindPrioritiesDCE_Comb              :
    //PriceForMarginOfTodayPositionCZCE_Comb:�����ֱ�֤��ʹ�õļ۸�: 0: ���ɽ��ۼ���(ȱʡ)��1: �������ۼ���
    //PriceForMarginOfTodayPositionDCE_Comb :
    //bFreezeOrderUnknownStatus             :��״̬Ϊδ֪�ı���(���ֹ���˵�)���Ƿ�ִ���ʽ𶳽�
    //PlatformID                            :ƽ̨ID���Զ��˺źͻ�������������ã����ڽ�FTID����
    //MaxPlatformID                         :ƽ̨ID�����ֵ�����ڶ��˺źͻ������������FTID����
    CPlatformDataMgr(
        bool bCalculate_OnRtnTrade=true,
        bool bUseComb=false,
        int DiscountCZCE_Comb=3,
        int DiscountDCE_Comb=3,
        int UnwindPrioritiesCZCE_Comb=1,
        int UnwindPrioritiesDCE_Comb=0,
        int PriceForMarginOfTodayPositionCZCE_Comb=0,
        int PriceForMarginOfTodayPositionDCE_Comb=1,
        bool bFreezeOrderUnknownStatus=false,
        int PlatformID=0,
        int MaxPlatformID=1,
        bool bUseSettlementPriceToCalculateProfitAndMarginAsPossible=false);
    ~CPlatformDataMgr(void);


    //����Ͷ������Ϣ
    void SetInvestorInfo(const CThostFtdcInvestorField& inData);


    //��ȡ������Ϣ���ɹ�����0��ʧ�ܷ���-1
    int GetLastSettlementInfo(std::string& outData);
    //���������Ϣ
    void ClearLastSettlementInfo(void);
    //��ӽ�����Ϣ
    void AppendLastSettlementInfo(const std::string& inData,bool bLast);

    //���¿ͻ�ǩԼ������Ϣ
	void UpdateContractBank(CThostFtdcAccountregisterField& inData);
	//��ȡ�ͻ�ǩԼ������Ϣ
	int GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData);
	//�洢ǩԼ��������
	void SaveBankName(CThostFtdcContractBankField& inData);

    //���������Ϣ
    void SaveExchangeRate(const PlatformStru_ExchangeRate& ExchangeRate);
    //��ȡ������Ϣ
    vector<string> GetCurrencys(const string& BankName,bool bAddUSD=true,bool bAddCNY=true);
    vector<string> GetCurrencys(bool bAddUSD=true,bool bAddCNY=true);

    //����/��ȡ���͹�˾���ײ���
    void SetBrokerTradingParam(const PlatformStru_BrokerTradingParams& BrokerTradingParam);
    const PlatformStru_BrokerTradingParams& GetBrokerTradingParam(void);

	SERVER_PARAM_CFG&  GetServerParamCfg();

	//�����������
    void clear(bool bAlwaysClearInstruments_Rates=true);


    //-------------------------------------------------
    //-------------------------------------------------
    //�����������
    void OnRtnDepthMarketData(const PlatformStru_DepthMarketData& MarketData);
    //�����ѯ������Ӧ������Ϻ�Լ������Ҫ�����������ʱ���ᴥ����ѯ���Ⱥ�Լ������
    void OnRspQryDepthMarketData(const PlatformStru_DepthMarketData& MarketData);
    //�����ر�
    void OnRtnOrder(PlatformStru_OrderInfo& OrderInfo);
    //�ɽ��ر�
    void OnRtnTrade(PlatformStru_TradeInfo& TradeInfo);
    //�����ѯ��Ʒ��Ӧ
    void OnRspQryProduct(const PlatformStru_ProductInfo& ProductInfo);
    //�����ѯ��Լ��Ӧ
    void OnRspQryInstrument(PlatformStru_InstrumentInfo& InstrumentInfo,bool bCTPSimulate,bool bLast);
    //�����ѯͶ���ֲ߳���Ӧ��ErrorID==0��ʾ������ȷ��ErrorID==123456789��ʾ��ѯ�޼�¼
    void OnRspQryInvestorPosition(PlatformStru_Position& PositionInfo,int ErrorID,bool bIsLast);
    //�����ѯͶ���ֲ߳���ϸ��Ӧ
    void OnRspQryInvestorPositionDetail(PlatformStru_PositionDetail& PositionDetailInfo,int ErrorID,bool bIsLast);
    //�����ѯͶ������ϳֲ���ϸ��Ӧ
    void OnRspQryInvestorPositionCombDetail(PlatformStru_PositionDetailComb& PositionDetailCombInfo,int ErrorID,bool bIsLast);
    //�����ѯ�ɽ���Ӧ
    void OnRspQryTrade(PlatformStru_TradeInfo& TradeInfo,int ErrorID,bool bIsLast);
    //�����ѯ������Ӧ
    void OnRspQryOrder(PlatformStru_OrderInfo& OrderInfo,int ErrorID,bool bIsLast);
    //��ѯ�����ʽ��˻���Ϣ��Ӧ
    void OnRspQryTradingAccount(PlatformStru_TradingAccountInfo& TradingAccount);
    //�����ѯ��Լ��֤������Ӧ
    void OnRspQryInstrumentMarginRate(PlatformStru_InstrumentMarginRate& MarginRate,int ErrorID);
    //�����ѯ��Լ����������Ӧ
    void OnRspQryInstrumentCommissionRate(PlatformStru_InstrumentCommissionRate& CommissionRate,int ErrorID);


    //�Ƿ��о��͹�˾���ײ�����Ϣ
    bool IsHaveBrokerTradingParam(void) { return m_bHaveBrokerTradingParam; }
public:
	SERVER_PARAM_CFG m_ServerParamCfg;


private:
    const int                   m_PlatformID;
    const int                   m_MaxPlatformID;

    //Ϊ�˼��ݶ��˺Ű汾������ʹ��static���Ա����ڶ��˺���ʹ��ͳһ��FTID��UpdateSeq, �������ڲ�ͬ�˺ŵ�FTID��ͬ����UI�����쳣
    //ÿ����¼���ݵ�FTID�Ĳ���ģ���˶��˺���ʹ��ͬһ��baseFTID��������long�����õ����⡣
    //UpdateSeq�ڶ��˺�����ܳ���long�����õ����⣬��7fffffff������0������UpdateSeqֻ�����Ƿ���ȵ��жϣ������д���С�ڵ��жϣ�����۷�Ϊ0�������Ӱ��ϵͳ����
    static volatile long        m_baseFTID;                              //FTID��׼ֵ��ÿ��ʹ�ú����
    static volatile long        m_baseUpdateSeq;                         //UpdateSeq��׼ֵ��ÿ��ʹ�ú����

private:
    //�������ݼ�
    CThostFtdcInvestorField     m_InvestorInfo;//Ͷ���߸�����Ϣ
    bool                        m_bInvestorInfoValid;

    std::string m_LastSettlementInfo;
    bool m_bSettlementInfoIsReady;

    Ceasymutex				m_mutex;

	std::map<std::string,  PlatformStru_ContractBank> m_ContractBankInfo;
	std::map<std::string,  CThostFtdcContractBankField> m_BankName;
    //������Ϣ
    map<pair<string,string>,PlatformStru_ExchangeRate> m_ExchangeRates;
    //����֧�ֵı��֣�������ѯ����ǩԼ��ϵʱ�鵽�ı���
    map<string,vector<string>> m_BankCurrency;

    //�ɽ�ʱ�Ƿ񴥷�����
    bool m_bCalculate_OnRtnTrade;
    //�Ƿ�ʹ����ϵ�����
    bool m_bUseComb;
	//��ϵ���֤���Żݲ���, 0-���Żݣ�1-��һ���Żݣ�2-�ڶ����Żݣ�3-����֤��Ƚϸߵ�һ����ȡ(ȱʡ)
	int m_DiscountCZCE_Comb;
	int m_DiscountDCE_Comb;
    //ƽ��˳��: 0: ͳһ�ȿ���ƽ��1: �ȵ�һ������ȿ���ƽ
    int m_UnwindPrioritiesCZCE_Comb;
    int m_UnwindPrioritiesDCE_Comb;
    //�����ֱ�֤��ʹ�õļ۸�: 0: ���ɽ��ۼ���(ȱʡ)��1: �������ۼ���
    int m_PriceForMarginOfTodayPositionCZCE_Comb;
    int m_PriceForMarginOfTodayPositionDCE_Comb;

    //���͹�˾���ײ��������е���ȨȨ����۸������ڼ���Ȩ����ʱ�õ�
    PlatformStru_BrokerTradingParams m_BrokerTradingParam;
    bool                             m_bHaveBrokerTradingParam;

public:
	//�����Ǹ������ݶ���Ҫע�����ǵ��Ⱥ�˳�򣬹���ʱҪ�����˳��������
    CPlatformDataMgr_LoginInfo              m_LoginInfo;
    CPlatformDataMgr_Instruments            m_Instruments;
    CPlatformDataMgr_Rates                  m_Rates;
    CPlatformDataMgr_Quots                  m_Quots;
    CPlatformDataMgr_Trades                 m_Trades;
    CPlatformDataMgr_TradeTotals            m_TradeTotals;
    CPlatformDataMgr_Orders                 m_Orders;
    CPlatformDataMgr_PositionDetailCombs    m_PositionDetailCombs;
    CPlatformDataMgr_PositionDetails        m_PositionDetails;
    CPlatformDataMgr_PositionCombs          m_PositionCombs;
    CPlatformDataMgr_Positions              m_Positions;
    CPlatformDataMgr_TradingAccount         m_TradingAccount;

private:

};

#ifdef WIN32
#pragma managed(pop)
#endif 
