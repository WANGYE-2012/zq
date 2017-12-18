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

	string GetTradingDay();
    ///��ȡ������ʱ��
    unsigned long GetExchangeTime(const string& ExchangeID,string& time);
    ///��ȡȫ������
    int GetAllOrders(std::map<OrderKey,  PlatformStru_OrderInfo>& outData);
    //��ȡָ����Լ��ָ������ָ����ƽ�Ĺҵ�������������
	int GetWaitOrderVolume(const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag=THOST_FTDC_HF_Speculation);
    //��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
    int GetCloseOrderVolume(const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag=THOST_FTDC_HF_Speculation);

    ///��ȡȫ���ɽ���
    int GetAllTrades(std::map<TradeKey,  PlatformStru_TradeInfo>& outData);
    ///��ȡ�ɽ�ͳ��
    int GetAllTradeTotalDatas(vector<TotalInfo>& outData);
	///�ڳɽ�ͳ�Ʋ��Ҳ���rawData
	int FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData );

    ///��ȡ�����ʽ��˻���Ϣ
	int GetTradingAccountWithdrawQuota(double& fWithdrawQuota);
	int GetTradingAccountAvailable(double& fAvailable);
	int GetTradingAccountID(char* AccountID);
    int GetTradingAccount(PlatformStru_TradingAccountInfo& outData);
    ///���ý����ʽ��˻���Ϣ
    void SetTradingAccount(const PlatformStru_TradingAccountInfo& inData);

	///�����ѯͶ���߽�������Ӧ
    int GetLastSettlementInfo(std::string& outData);
    
    ///����
    int Start();

    ///ֹͣ,������Ϣ�������ٴ�����֮�󣬶�����Ȼ��Ч
    int Stop();

    ///��ȡ����ί�е���Ϣ
    int GetAllOrdersEx(std::map <AllOrderKey,  PlatformStru_OrderInfo> &AllOrdersMap);

    ///��ȡָ����Լ�ĳֲ���ϸ
    int GetPositionDetails(std::multimap<std::string, PositionDetailStru> &PositionDetailMap);
    int GetPositionDetails(const std::string& InstrumentID,std::multimap<std::string, PositionDetailStru> &PositionDetailMap);
	int GetPositionDetails(std::list<PositionDetailStru> &pPositionDetailList);
    //��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
    int GetPositionKeyList(std::list<PositionKey> &PositionKeyList);
    //��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
    int GetPositionKeySet(std::set<PositionKey> &PositionKeySet);
    //��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
    int GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PositionStru& PositionData);

	//������Ϻ�Լ�����������
	int MakeComboDepthMarketData(PlatformStru_DepthMarketData *pDepthMarketData,int index=0);
	int InitCombRelation();
    bool IsNeedCheckCommissionRateAndMarginRate(void);
    void SetNeedCheckCommissionRateAndMarginRate(bool bNeedCheck);

	SERVER_PARAM_CFG&  GetServerParamCfg();
private:
    int doProcessBusinessData(AbstractBusinessData& data);
private:
    CPackageCache&     m_dataSource;
    zqWriteLog*         m_pWriteLog;
    CDataCenterWorkThread* m_pWorkThread;
    wxMutex            m_mutex;



    CPlatformDataMgr* m_pDataMgr;
	CPlatFormService* m_PlatFormService;



};



#endif

