#pragma warning(disable:4996)

#include "PlatformDataMgr.h"

#include <string>
#include <map>
using namespace std;

#include "FileOpr.h"
#include "Module-Misc2/tools_util.h"
#include "Module-Misc2/GlobalFunc.h"
#include "PlatformDataMgrMacro.h"



#ifdef WIN32
#pragma unmanaged
#endif 


volatile long CPlatformDataMgr::m_baseFTID=1;                              //FTID��׼ֵ��ÿ��ʹ�ú����
volatile long CPlatformDataMgr::m_baseUpdateSeq=1;                         //UpdateSeq��׼ֵ��ÿ��ʹ�ú����


CPlatformDataMgr::CPlatformDataMgr( bool bCalculate_OnRtnTrade,
                                    bool bUseComb,
                                    int DiscountCZCE_Comb,
                                    int DiscountDCE_Comb,
                                    int UnwindPrioritiesCZCE_Comb,
                                    int UnwindPrioritiesDCE_Comb,
                                    int PriceForMarginOfTodayPositionCZCE_Comb,
                                    int PriceForMarginOfTodayPositionDCE_Comb,
                                    bool bFreezeOrderUnknownStatus,
                                    int PlatformID,
                                    int MaxPlatformID,
                                    bool bUseSettlementPriceToCalculateProfitAndMarginAsPossible)
:   m_PlatformID(PlatformID),
    m_MaxPlatformID(MaxPlatformID),
    m_bInvestorInfoValid(false),
    m_bCalculate_OnRtnTrade(bCalculate_OnRtnTrade),
    m_bUseComb(bUseComb),
    m_DiscountCZCE_Comb(DiscountCZCE_Comb),
    m_DiscountDCE_Comb(DiscountDCE_Comb),
    m_UnwindPrioritiesCZCE_Comb(UnwindPrioritiesCZCE_Comb),
    m_UnwindPrioritiesDCE_Comb(UnwindPrioritiesDCE_Comb),
    m_PriceForMarginOfTodayPositionCZCE_Comb(PriceForMarginOfTodayPositionCZCE_Comb),
    m_PriceForMarginOfTodayPositionDCE_Comb(PriceForMarginOfTodayPositionDCE_Comb),
    m_bHaveBrokerTradingParam(false),
    m_LoginInfo(),
    m_Instruments(m_bUseComb),
    m_Rates(),
    m_Quots(m_Instruments),
    m_Trades(m_baseFTID,m_baseUpdateSeq,m_Instruments,m_Rates),
    m_TradeTotals(m_baseFTID,m_baseUpdateSeq,m_Trades),
    m_Orders(m_baseFTID,m_baseUpdateSeq,m_Instruments,m_Rates,m_Quots,m_DiscountCZCE_Comb,m_DiscountDCE_Comb,bFreezeOrderUnknownStatus),
    m_PositionDetailCombs(m_baseFTID,m_baseUpdateSeq,m_Instruments,m_Quots,m_Trades,m_Orders),
    m_PositionDetails(m_baseFTID,m_baseUpdateSeq,m_Instruments,m_Rates,m_Quots,m_Trades,m_Orders,m_PositionDetailCombs,m_DiscountCZCE_Comb,m_DiscountDCE_Comb,m_UnwindPrioritiesCZCE_Comb,m_UnwindPrioritiesDCE_Comb,m_PriceForMarginOfTodayPositionCZCE_Comb,m_PriceForMarginOfTodayPositionDCE_Comb,bUseSettlementPriceToCalculateProfitAndMarginAsPossible),
    m_PositionCombs(m_baseFTID,m_baseUpdateSeq,m_Instruments,m_Quots,m_Trades,m_Orders,m_PositionDetailCombs,m_PositionDetails),
    m_Positions(m_baseFTID,m_baseUpdateSeq,m_Instruments,m_Rates,m_Quots,m_Trades,m_Orders,m_PositionDetails,bUseSettlementPriceToCalculateProfitAndMarginAsPossible),
    m_TradingAccount(m_Instruments,m_Quots,m_Trades,m_Orders,m_PositionDetailCombs,m_PositionDetails,m_PositionCombs,m_Positions)
{
    memset(&m_BrokerTradingParam,0,sizeof(m_BrokerTradingParam));
    memset(&m_InvestorInfo,0,sizeof(m_InvestorInfo));
    m_bSettlementInfoIsReady=false;
}

CPlatformDataMgr::~CPlatformDataMgr(void)
{
    LOG_INFO("CPlatformDataMgr::~CPlatformDataMgr(����CPlatformDataMgr����)");
}






//����Ͷ������Ϣ
void CPlatformDataMgr::SetInvestorInfo(const CThostFtdcInvestorField& inData)
{
    Lock_CPlatformDataMgr;

    m_InvestorInfo=inData;
    m_bInvestorInfoValid=true;

    Unlock_CPlatformDataMgr;

}



//��ȡ������Ϣ���ɹ�����0��ʧ�ܷ���-1
int CPlatformDataMgr::GetLastSettlementInfo(std::string& outData)
{
    int ret;

    Lock_CPlatformDataMgr;

    if(m_bSettlementInfoIsReady)
    {
        outData=m_LastSettlementInfo;
        ret=0;
    }
    else
        ret=-1;

    Unlock_CPlatformDataMgr;

    return ret;
}

//���������Ϣ
void CPlatformDataMgr::ClearLastSettlementInfo(void)
{
    Lock_CPlatformDataMgr;

    m_LastSettlementInfo.clear();
    m_bSettlementInfoIsReady=false;

    Unlock_CPlatformDataMgr;

}

//��ӽ�����Ϣ
void CPlatformDataMgr::AppendLastSettlementInfo(const std::string& inData,bool bLast)
{
    Lock_CPlatformDataMgr;

    if(m_bSettlementInfoIsReady)
    {
        m_LastSettlementInfo.clear();
        m_bSettlementInfoIsReady=false;
    }

    m_LastSettlementInfo.append(inData);
    if(bLast)
        m_bSettlementInfoIsReady=true;

    Unlock_CPlatformDataMgr;

}



SERVER_PARAM_CFG&  CPlatformDataMgr::GetServerParamCfg()
{
	return m_ServerParamCfg;
}

//�����������
void CPlatformDataMgr::clear(bool bAlwaysClearInstruments_Rates)
{
    Lock_CPlatformDataMgr;

	m_LoginInfo.clear();
	if(bAlwaysClearInstruments_Rates)
	{
		m_Instruments.clear();
		m_Rates.clear();
	}
	m_Quots.clear();
	m_Trades.clear();
	m_TradeTotals.clear();
	m_Orders.clear();
	m_PositionDetailCombs.clear();
	m_PositionCombs.clear();
	m_PositionDetails.clear();
	m_Positions.clear();
	m_TradingAccount.clear();

    Unlock_CPlatformDataMgr;
}


//�洢�ͻ�ǩԼ��������
void CPlatformDataMgr::SaveBankName(CThostFtdcContractBankField& inData)
{
    m_BankName[inData.BankName]=inData;
}

//���������Ϣ
void CPlatformDataMgr::SaveExchangeRate(const PlatformStru_ExchangeRate& ExchangeRate)
{
    m_ExchangeRates[pair<string,string>(ExchangeRate.FromCurrencyID,ExchangeRate.ToCurrencyID)]=ExchangeRate;
}
//���¿ͻ�ǩԼ������Ϣ
void CPlatformDataMgr:: UpdateContractBank(CThostFtdcAccountregisterField& inData)
{
	LOG_INFO("CPlatformDataMgr::UpdateContractBank");
    PlatformStru_ContractBank   ContractBank;
	memset(&ContractBank,0,sizeof(PlatformStru_ContractBank));
    strcpy(ContractBank.BankID,inData.BankID);
	strcpy(ContractBank.BankAccount,inData.BankAccount);
	strcpy(ContractBank.CurrencyID,inData.CurrencyID);
	strcpy(ContractBank.BankBranchID,inData.BankBranchID);
	strcpy(ContractBank.BrokerID,inData.BrokerID);


	std::map<std::string,  CThostFtdcContractBankField>::iterator it=m_BankName.begin();
	for(; it!=m_BankName.end(); ++it)
	{
		if(!strcmp(inData.BankID, it->second.BankID))
		{
			strcpy(ContractBank.BankName,it->second.BankName);
			strcpy(ContractBank.BankBranchID,it->second.BankBrchID);
			break;

		}

	}

    Lock_CPlatformDataMgr;

    string strBankName(ContractBank.BankName);
	m_ContractBankInfo[strBankName]=ContractBank;

    //�������ж�Ӧ�ı�����Ϣ
    string strCurrency(ContractBank.CurrencyID);
    map<string,vector<string>>::iterator itCurrency=m_BankCurrency.find(strBankName);
    if(itCurrency==m_BankCurrency.end())
    {
        m_BankCurrency[strBankName]=vector<string>();
        itCurrency=m_BankCurrency.find(strBankName);
    }
    if(!CTools_Ansi::IsValueInVector<string>(itCurrency->second,strCurrency))
        itCurrency->second.push_back(strCurrency);


    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::UpdateContractBank-end");
	//m_Accountregister.push_back(inData.BankID);
	//m_timer.Start(1000,true);
}

//��ȡ�ͻ�ǩԼ������Ϣ
int CPlatformDataMgr::GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData)
{
	LOG_INFO("CPlatformDataMgr::GetContractBank");

    Lock_CPlatformDataMgr;

	outData=m_ContractBankInfo;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::GetContractBank-end");
	return outData.size();

}

//�����������
void CPlatformDataMgr::OnRtnDepthMarketData(const PlatformStru_DepthMarketData& MarketData)
{
    string strInstrument(MarketData.InstrumentID);
    if(strInstrument.empty()) return;

    //ȡ��Լ��Ϣ
    PlatformStru_InstrumentInfo InstrumentInfo;
    if(!m_Instruments.GetInstrumentInfo(strInstrument,InstrumentInfo)) return;

    m_Quots.SetQuotInfo(MarketData);

    static bool b1=true,b2=true,b3=true;
    //���³ֲ���ϸ��¼�ĳֲ�ӯ��������ӯ��������ۡ������ۡ��ּ�
    if(b1) m_PositionDetails.UpdatePositionDetail_RtnDepthMarketData(MarketData,InstrumentInfo);
    //���³ֲּ�¼�ĳֲ�ӯ��(����)������ӯ��(���)��������ӯ��������ۡ�������
    if(b2) m_Positions.UpdatePosition_RtnQuot(MarketData,InstrumentInfo,false);
    //�����ʽ��˻���Ϣ
    if(b3) m_TradingAccount.DynamicUpdateTradingAccount_RtnDepthMarketData(strInstrument);
}

//�����ѯ������Ӧ������Ϻ�Լ������Ҫ�����������ʱ���ᴥ����ѯ���Ⱥ�Լ������
void CPlatformDataMgr::OnRspQryDepthMarketData(const PlatformStru_DepthMarketData& MarketData)
{
    string strInstrument(MarketData.InstrumentID);
    if(strInstrument.empty()) return;

    //ȡ��Լ��Ϣ
    PlatformStru_InstrumentInfo InstrumentInfo;
    if(!m_Instruments.GetInstrumentInfo(strInstrument,InstrumentInfo)) return;

    m_Quots.SetQuotInfo(MarketData);

    //���±������ᱣ֤��Ͷ���������
    bool bUpdateMargin=m_Orders.UpdateOrderFreezeMargin_RspQryMarginRate_RspQryDepthMarketData(strInstrument,InstrumentInfo);
    bool bUpdateCommission=m_Orders.UpdateOrderFreezeCommission_RspQryCommissionRate_RspQryDepthMarketData(strInstrument,InstrumentInfo);
    if(bUpdateMargin||bUpdateCommission)
    {
        //�����ʽ��еĶ�������
        m_TradingAccount.UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData();
    }
}

//�����ر�
void CPlatformDataMgr::OnRtnOrder(PlatformStru_OrderInfo& OrderInfo)
{
    string strInstrumentID(OrderInfo.InstrumentID);
    if(strInstrumentID.empty()) return;

    //ȡ��Լ��Ϣ
    PlatformStru_InstrumentInfo InstrumentInfo;
    if(!m_Instruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo)) return;

    PlatformStru_OrderInfo OldOrderInfo;

    //���±�������
    m_Orders.NewOrderReady(OrderInfo,OldOrderInfo,InstrumentInfo);

    //���µ���Լ�ֲֶ����ƽ������
    m_Positions.UpdatePositionFreezeFields_RtnOrder(OrderInfo, OldOrderInfo, InstrumentInfo);

    if(1)
    {
        PlatformStru_OrderInfo tmpOrderInfo;
        if(m_Orders.GetOrder(OrderKey(OrderInfo),tmpOrderInfo))
        {
            char buf[1024];
            sprintf(buf,"BID_RtnOrder: %s/%s/%g/%g  ",tmpOrderInfo.InstrumentID,tmpOrderInfo.OrderSysID,tmpOrderInfo.freezeMargin,tmpOrderInfo.troubleMoney);
            CFileOpr::getObj().writelocallog("�����������",buf);
        }
    }

    //�����ʽ�����
    m_TradingAccount.UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData();
}

//�ɽ��ر�
void CPlatformDataMgr::OnRtnTrade(PlatformStru_TradeInfo& TradeInfo)
{
    string strInstrumentID(TradeInfo.InstrumentID);
    if(strInstrumentID.empty()) return;

    //ȡ��Լ��Ϣ
    PlatformStru_InstrumentInfo InstrumentInfo;
    if(!m_Instruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo)) return;

    //���³ɽ���¼
    m_Trades.NewTradeReady(TradeInfo,InstrumentInfo);

    //���³ɽ�ͳ��
	m_TradeTotals.UpdateTradeTotal_RtnTrade(TradeInfo);

	if(m_bCalculate_OnRtnTrade)
	{
        //���ɽ��޸ĵĳֲ���ϸ��¼
        map<PositionDetailKey,PlatformStru_PositionDetail> ModifiedPositionDetails;
        //���޸ĵ���ϳֲ���ϸ��¼
        map<PositionDetailCombKey,PlatformStru_PositionDetailComb> ModifiedPositionDetailCombs;
        //ƽ���������λʱ����ƽ������ϳɽ�ID�Ͷ�Ӧƽ�����Ĺ�ϵ
        map<string,int> ComTradeID2CloseVolume;

        //���³ֲ���ϸ
		m_PositionDetails.UpdatePositionDetail_RtnTrade(
            TradeInfo,
            InstrumentInfo,
            ModifiedPositionDetails,
            ComTradeID2CloseVolume);

        //���³ɽ���¼�е�ƽ������ƽ��ӯ�������������ѡ�
        m_Trades.UpdateTradeCloseTodayVolume_CloseProfit_RecalcuCommission(
            TradeInfo.FTID,
            InstrumentInfo,
            TradeInfo.CloseTodayVolume,
            TradeInfo.CloseProfitByDate,
            TradeInfo.CloseProfitByTrade,
            TradeInfo.TradeCommission);

        
        //������ϳֲ���ϸ��
        map<PositionDetailKey,int> mapCloseDeltaVolume;
        if(TradeInfo.OffsetFlag!=THOST_FTDC_OF_Open)
            m_PositionDetails.GetCloseDeltaVolume(ModifiedPositionDetails,mapCloseDeltaVolume);
        m_PositionDetailCombs.Update_RtnTrade(
            TradeInfo,
            mapCloseDeltaVolume,
            ComTradeID2CloseVolume,
            ModifiedPositionDetailCombs);

        //����б仯�ĳֲ���ϸ������֤���������õ���ϳֲ���ϸ�С���ʱδ�����Ż�
        m_PositionDetails.SetMarginOfPositionDetailComb(ModifiedPositionDetails);

        //����б仯�ĳֲ���ϸ��������ϳֲ���ϸ�ı�֤���Żݡ���Ҫ�ڸ��³ֲ���ϸ�͸�����ϳֲ���ϸ����С����ݱ�֤���Ż����ý��м��㣬������Ӧ�ֲ���ϸ����ϳֲ���ϸ�ı�֤���ֶΣ�(Ϊ��֤��©�������ȶ��㣬�е��ظ�)
        //�Ὣ��֤���������õ��ֲ���ϸ����ϳֲ���ϸ��ȥ
        m_PositionDetails.CalcuPositionDetailMarginDiscount_RtnTrade_RspQryRate_OnSettlement(ModifiedPositionDetails,ModifiedPositionDetailCombs);

        //���ֳɽ�ʱ����Ϻ�ԼID���õ���Ӧ�ĳֲ���ϸ�С�������ϳֲ���ϸ�ı仯
        if(TradeInfo.TradeType==THOST_FTDC_TRDT_CombinationDerived&&TradeInfo.OffsetFlag==THOST_FTDC_OF_Open)
            m_PositionDetails.SetCombInstrumentID_OpenTrade(ModifiedPositionDetailCombs);

        //���³ֲ�
        m_Positions.UpdatePosition_RtnTrade(TradeInfo,ModifiedPositionDetails,InstrumentInfo);

        //������ϳֲ�
        m_PositionCombs.Update_RtnTrade(TradeInfo,ModifiedPositionDetailCombs);

        //�����ʽ�
		m_TradingAccount.UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted();
	}
}

//�����ѯ��Ʒ��Ӧ
void CPlatformDataMgr::OnRspQryProduct(const PlatformStru_ProductInfo& ProductInfo)
{
	if(ProductInfo.ProductID[0]==0) return;
    m_Instruments.SetProductInfo(string(ProductInfo.ProductID),ProductInfo);
}

//�����ѯ��Լ��Ӧ
void CPlatformDataMgr::OnRspQryInstrument(PlatformStru_InstrumentInfo& InstrumentInfo,bool bCTPSimulate,bool bLast)
{
	if(InstrumentInfo.InstrumentID[0]==0) return;

    m_Instruments.SetInstrumentInfo(InstrumentInfo.InstrumentID,InstrumentInfo,bCTPSimulate,bLast);
    m_Rates.SetNeedCheckCommissionRateAndMarginRate(true);
}

//�����ѯͶ���ֲ߳���Ӧ��ErrorID==0��ʾ������ȷ��ErrorID==123456789��ʾ��ѯ�޼�¼
void CPlatformDataMgr::OnRspQryInvestorPosition(PlatformStru_Position& PositionInfo,int ErrorID,bool bIsLast)
{
    string strInstrumentID(PositionInfo.InstrumentID);
    if(strInstrumentID.empty()) return;
    //ȡ��Լ��Ϣ
    PlatformStru_InstrumentInfo InstrumentInfo;
    if(!m_Instruments.GetInstrumentInfo(strInstrumentID,InstrumentInfo)) return;

    //����Ƿ�����Ϻ�Լ�ֲ�
    bool bComb = InstrumentInfo.ProductClass==THOST_FTDC_PC_Combination;
    
    //�����ѯ�����������͵����ݼ���
    if(!bComb) m_Positions.QryPositionReach(PositionInfo,ErrorID,bIsLast);
    else if(bIsLast)  m_Positions.QryPositionReach(PositionInfo,-1,bIsLast);

    if(bComb) m_PositionCombs.QryPositionCombReach(PositionInfo,ErrorID,bIsLast);
    else if(bIsLast)  m_PositionCombs.QryPositionCombReach(PositionInfo,-1,bIsLast);

    //�����ʽ���Ϣ
	if(bIsLast)
		m_TradingAccount.UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted();
}

//�����ѯͶ���ֲ߳���ϸ��Ӧ
void CPlatformDataMgr::OnRspQryInvestorPositionDetail(PlatformStru_PositionDetail& PositionDetailInfo,int ErrorID,bool bIsLast)
{
    if(PositionDetailInfo.InstrumentID[0]==0) return;

    //�����ѯ�����������͵����ݼ���
    m_PositionDetails.QryPositionDetailReach(PositionDetailInfo,ErrorID,bIsLast);
}

//�����ѯͶ������ϳֲ���ϸ��Ӧ
void CPlatformDataMgr::OnRspQryInvestorPositionCombDetail(PlatformStru_PositionDetailComb& PositionDetailCombInfo,int ErrorID,bool bIsLast)
{
    if(PositionDetailCombInfo.InstrumentID[0]==0) return;

	//�����ϳֲ���ϸ��¼
	m_PositionDetailCombs.QryPositionDetailCombReach(PositionDetailCombInfo,ErrorID,bIsLast);

    if(bIsLast)
    {
        //ȡ��������ϳֲ���ϸ��key, ����Ϻ�Լ���õ���Ӧ�ĳֲ���ϸ��
        vector<PositionDetailCombKey> combdetailkeys;
        m_PositionDetailCombs.GetAllKeys(combdetailkeys);
        m_PositionDetails.SetCombInstrumentIDs(combdetailkeys);
    }
}

//�����ѯ�ɽ���Ӧ
void CPlatformDataMgr::OnRspQryTrade(PlatformStru_TradeInfo& TradeInfo,int ErrorID,bool bIsLast)
{
    if(TradeInfo.InstrumentID[0]==0) return;

    //�����ѯ�����������͵����ݼ���
    m_Trades.QryTradeReach(TradeInfo,ErrorID,bIsLast);

    if(bIsLast)
	{
		//�ɽ���ȫ�󣬼���ɽ�ͳ��
		m_TradeTotals.QryTradeTotalReach(true);
		//�����ʽ�����
        m_TradingAccount.UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted();
	}
}

//�����ѯ������Ӧ
void CPlatformDataMgr::OnRspQryOrder(PlatformStru_OrderInfo& OrderInfo,int ErrorID,bool bIsLast)
{
    if(OrderInfo.InstrumentID[0]==0) return;

    //�����ѯ�����������͵����ݼ���
    m_Orders.QryOrderReach(OrderInfo,ErrorID,bIsLast);
    if(bIsLast)
    {
	    //�����ʽ�����
        m_TradingAccount.UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData();
    }
}

//��ѯ�����ʽ��˻���Ϣ��Ӧ
void CPlatformDataMgr::OnRspQryTradingAccount(PlatformStru_TradingAccountInfo& TradingAccount)
{
    m_TradingAccount.SetTradingAccount(TradingAccount);
}

//�����ѯ��Լ��֤������Ӧ
void CPlatformDataMgr::OnRspQryInstrumentMarginRate(PlatformStru_InstrumentMarginRate& MarginRate,int ErrorID)
{
    string strInstrument(MarginRate.InstrumentID);
    if(strInstrument.empty()) return;

    PlatformStru_InstrumentInfo InstrumentInfo;
    if(!m_Instruments.GetInstrumentInfo(strInstrument,InstrumentInfo)) return;

    if(ErrorID==0)
    {
        m_Rates.SetMarginRate(strInstrument,MarginRate);

        //���±������ᱣ֤������
        if(m_Orders.UpdateOrderFreezeMargin_RspQryMarginRate_RspQryDepthMarketData(strInstrument,InstrumentInfo))
        {
            //�����ʽ��еĶ�������
            m_TradingAccount.UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData();
        }

        //���³ֲ���ϸ��֤������, ͬʱ������ϱ�֤���Żݡ���ϳֲ���ϸ�ı�֤��
        m_PositionDetails.UpdatePositionDetailByMarginRate_RspQryRate(MarginRate,InstrumentInfo);

        //���³ֲ��ж����ʽ�����
		m_Positions.UpdatePositionFreezeFieldsByOrders_RspQryRate(string(MarginRate.InstrumentID));

        //���³ֲ��еı�֤������
        bool bUpdatePosition=m_Positions.UpdatePositionByMarginRate_RspQryRate(MarginRate);

        //������ϳֲ��еı�֤������
        bool bUpdatePositionComb=m_PositionCombs.UpdatePositionByMarginRate_RspQryRate(MarginRate);

        if(bUpdatePosition||bUpdatePositionComb)
        {
            //��Ҫʱ�����ʽ��еı�֤������
            m_TradingAccount.DynamicUpdateTradingAccount_RspQryRate();
        }
    }
	else if(ErrorID==CustomErrorID)
	{
		//���޼�¼
		m_Rates.IncrementReqMarginRateError(strInstrument);
	}
}

//�����ѯ��Լ����������Ӧ��������Ȩ����������
void CPlatformDataMgr::OnRspQryInstrumentCommissionRate(PlatformStru_InstrumentCommissionRate& CommissionRate,int ErrorID)
{
    string strInstrument(CommissionRate.InstrumentID);
    if(strInstrument.empty()) return;

    if(ErrorID==0)
    {
        //m_allInstrumentIDsByProductID��ʱ�Ѿ�������(��ѯ��Լ��Ϣ������ ��ѯ��������֮ǰ)
        bool bHaveOrder=false;
        bool bHaveTrade=false;
        bool bHavePosition=false;

        //�Ǻ�ԼID
        PlatformStru_InstrumentInfo InstrumentInfo;
        if(!m_Instruments.GetInstrumentInfo(strInstrument,InstrumentInfo))
            return;

        m_Rates.SetCommissionRate(strInstrument,CommissionRate);

		//���±��������ж���������
        if(m_Orders.UpdateOrderFreezeCommission_RspQryCommissionRate_RspQryDepthMarketData(strInstrument,InstrumentInfo)&&!bHaveOrder)
            bHaveOrder=true;

		//���³ɽ���ϸ������������
        if(m_Trades.UpdateTradeCommission_RspQryCommissionRate(strInstrument,InstrumentInfo,CommissionRate)&&!bHaveTrade)
            bHaveTrade=true;

		//���³ɽ�ͳ��
		m_TradeTotals.UpdateTradeTotal_RspQryCommissionRate(strInstrument);

		//���³ֲ��еĶ�����
		m_Positions.UpdatePositionFreezeFieldsByOrders_RspQryRate(strInstrument);

        //���³ֲ��е�����������
        if(m_Positions.UpdatePositionByCommissionRate_RspQryRate(CommissionRate)&&!bHavePosition)
            bHavePosition=true;

        //�����ʽ��еĶ�������
        if(bHaveOrder)
            m_TradingAccount.UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData();

        //�����ʽ��е�����������
        if(bHaveTrade||bHavePosition)
            m_TradingAccount.DynamicUpdateTradingAccount_RspQryInstrumentCommissionRate();
    }
    else if(ErrorID==CustomErrorID)
    {
		//���޼�¼
		m_Rates.IncrementReqCommissionRateError(strInstrument);
    }
}



//��ȡ������Ϣ
//1. ���ȸ�������ǩԼ��ϵ��ȡ���ж�Ӧ�ı���
//2. �������USD��RMB(CNY)
vector<string> CPlatformDataMgr::GetCurrencys(const string& BankName,bool bAddUSD,bool bAddCNY) 
{ 
    vector<string> vecrlt;
    map<string,vector<string>>::const_iterator it=m_BankCurrency.find(BankName);
    if(it!=m_BankCurrency.end())
    {
        vecrlt=it->second;
    }
    if(bAddUSD&&!CTools_Ansi::IsValueInVector<string>(vecrlt,string("USD")))
        vecrlt.push_back(string("USD"));
    if(bAddCNY&&!CTools_Ansi::IsValueInVector<string>(vecrlt,string("CNY"))&&!CTools_Ansi::IsValueInVector<string>(vecrlt,string("RMB")))
    {
        vecrlt.push_back(string("CNY"));
        vecrlt.push_back(string("RMB"));
    }
    return vecrlt;
}
vector<string> CPlatformDataMgr::GetCurrencys(bool bAddUSD,bool bAddCNY) 
{ 
    vector<string> vecrlt;

    set<string> setCurrency;
    for(map<string,vector<string>>::const_iterator it=m_BankCurrency.begin();it!=m_BankCurrency.end();it++)
    {
        for(int i=0;i<(int)(it->second.size());i++)
            if(setCurrency.find(it->second[i])==setCurrency.end())
                setCurrency.insert(it->second[i]);
    }
    if(bAddUSD&&setCurrency.find("USD")==setCurrency.end())
        setCurrency.insert(string("USD"));
    if(bAddCNY&&setCurrency.find("CNY")==setCurrency.end()&&setCurrency.find("RMB")==setCurrency.end())
    {
        setCurrency.insert(string("CNY"));
        setCurrency.insert(string("RMB"));
    }

    CTools_Ansi::ConvertSet2Vector<string>(setCurrency,vecrlt);
    return vecrlt;
}

//����/��ȡ���͹�˾���ײ���
void CPlatformDataMgr::SetBrokerTradingParam(const PlatformStru_BrokerTradingParams& BrokerTradingParam)
{
    m_BrokerTradingParam=BrokerTradingParam;
    m_PositionDetails.SetPriceForRoyalty_Option(m_BrokerTradingParam.OptionRoyaltyPriceType==THOST_FTDC_ORPT_PreSettlementPrice?1:0);
    m_bHaveBrokerTradingParam=true;
}
const PlatformStru_BrokerTradingParams& CPlatformDataMgr::GetBrokerTradingParam(void)
{
    return m_BrokerTradingParam;
}
