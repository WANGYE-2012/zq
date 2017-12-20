/***************************************************************************//**
*   @file          DataCenter.hpp
*   @brief        �������ģ��洢�ۺϽ���ƽ̨��������ҵ������
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/09/14
********************************************************************************/

#ifndef _DATACENTER_HPP_
#define _DATACENTER_HPP_

#include <map>
#include <list>
#include <set>
#include <string>
#include "../inc/ISvr/BusinessData.hpp"
#include "wx/thread.h"
#include "PlatformDataMgr.h"


//#include "InvestorPositionDetailMgr.h"

class CPackageCache;
class zqWriteLog;
class CPlatFormService;
class CDataCenter
{
friend class CDataCenterWorkThread;
public:
    explicit CDataCenter(CPackageCache& dataSource,CPlatFormService* pHost);
    ~CDataCenter();

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
	void SetInstrumentVarietyMap(map<string, string>& outData);
	// ��ȡ��Լ�ݲ��б�
	int GetInstrumentVarietyMap(map<string, string>& outData);
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
    ///���ú�Լ��������
    int SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);  
    ///�������ڲ�ѯ�������ʵĺ�Լ
    void SetReqCommissionRateInstrumentID(const std::string& InstrumentID);
    ///��ȡ��Լ��֤����
    int GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
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

    ///���õ�ǰ��ʾ�ĳֲ����ݡ�1:�ֲ֣�2:�ֲ���ϸ��3:��ϳֲ�
    void SetCurrentPositionContent(int PositionContentMode);

	string GetTradingDay(const string& ExchangeID);
	bool IsTodayPosition( const char* pOpenData );
    ///��ȡ������ʱ��
    unsigned long GetExchangeTime(const string& ExchangeID,string& time);
	//��ȡȫ��������
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


    //��ȡָ����Լ��ָ������ָ����ƽ�Ĺҵ�������������
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
    int GetTradingAccount(PlatformStru_TradingAccountInfo& outData);
	///��ȡ�˻��ʽ��ı���Ϣ
    int GetAccountText(std::string& outData,int language);
    ///���ý����ʽ��˻���Ϣ
    void SetTradingAccount(const PlatformStru_TradingAccountInfo& inData);

	///�����ѯͶ���߽�������Ӧ
    int GetLastSettlementInfo(std::string& outData);
	 //��ȡ�ͻ�ǩԼ������Ϣ
    int GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData);
    
    ///����
    int Start();

    ///ֹͣ,������Ϣ�������ٴ�����֮�󣬶�����Ȼ��Ч
    int Stop();


    ///��ȡָ����Լ�ĳֲ���ϸ
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


    bool IsNeedCheckCommissionRateAndMarginRate(void) { return m_pDataMgr->IsNeedCheckCommissionRateAndMarginRate();}
    void SetNeedCheckCommissionRateAndMarginRate(bool bNeedCheck) { m_pDataMgr->SetNeedCheckCommissionRateAndMarginRate(bNeedCheck);}
	SERVER_PARAM_CFG&  GetServerParamCfg();
    void SetModuleDataQry(bool bQry);
    bool GetModuleDataQry();

    void ClearMapQryRltOrders();
    void ClearMapQryRltTrades();
    void ClearMapQryRltPositionDetails(std::string& strQryInstrument);
    void ClearMapQryRltPositions(std::string& strQryInstrument);
    void ClearMapQryRltPositionCombs(std::string& strQryInstrument);

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
	static DWORD DataCenterThreadFunc( void* pData );
private:
    int doProcessBusinessData(AbstractBusinessData& data);
private:
    CPackageCache&     m_dataSource;
    zqWriteLog*         m_pWriteLog;
	HANDLE				m_hWorkerThread;
	bool				m_bWorkerThreadExit;

    wxMutex            m_mutex;



    CPlatformDataMgr* m_pDataMgr;
	CPlatFormService* m_PlatFormService;



};



#endif

