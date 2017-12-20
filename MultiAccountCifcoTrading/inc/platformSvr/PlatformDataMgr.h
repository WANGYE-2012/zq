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
#include "../inc/ISvr/BusinessData.hpp"
#include "../inc/Module-Misc/packagecache.h"
#include "PlatformDataSetList.hpp"
#include "ReqOnRtnTrade.h"


class CPackageCache;

class zqWriteLog;

#if 1
#define LogLock1
#define LogLock2 
#define LogUnlock 
#else
#define LogLock1 \
{ \
	char minfun[1024],buf[1024];  \
	strcpy_s(minfun,__FUNCDNAME__);\
	for (size_t i=0;i<strlen(minfun);++i){	if(minfun[i]=='@') {minfun[i]=0;break;}}\
	sprintf(buf,"lock_Begin:0x%08X,%s(%d)\n",&m_CS,minfun,__LINE__);\
	OutputDebugString(buf); \
}
#define LogLock2 \
{ \
	char minfun[1024],buf[1024];  \
	strcpy_s(minfun,__FUNCDNAME__);\
	for (size_t i=0;i<strlen(minfun);++i){	if(minfun[i]=='@') {minfun[i]=0;break;}}\
	sprintf(buf,"lock_end:0x%08X,%s(%d)\n",&m_CS,minfun,__LINE__);\
	OutputDebugString(buf); \
}
#define LogUnlock \
{ \
	char minfun[1024],buf[1024];  \
	strcpy_s(minfun,__FUNCDNAME__);\
	for (size_t i=0;i<strlen(minfun);++i){	if(minfun[i]=='@') {minfun[i]=0;break;}}\
	sprintf(buf,"LogUnlock:0x%08X,%s(%d)\n",&m_CS,minfun,__LINE__);\
	OutputDebugString(buf); \
}
#endif


#define Lock_CPlatformDataMgr \
                { \
                    LogLock1  \
                    EnterCriticalSection(&m_CS);  \
                }

#define Unlock_CPlatformDataMgr \
                { \
                    LogUnlock  \
                    LeaveCriticalSection(&m_CS); \
                }

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
	///��ȡ�˻��ʽ��ı���Ϣ
    int GetAccountText(std::string& outData,int language);
    ///��̬�����ʽ��˻��еĶ����ʽ𣬲�ѯ�������ء���ѯ���ʷ���
    void DynamicUpdateTradingAccount_Freeze();
    ///��̬�����ʽ��˻��еĶ����ʽ𣬱����ر�ʱ����
    void DynamicUpdateTradingAccount_Freeze_RtnOrder(const PlatformStru_OrderInfo& Order, 
                                                     const PlatformStru_OrderInfo& OldOrderInfo);
    ///�������ʱ����̬�����ʽ��˻���Ϣ
    void DynamicUpdateTradingAccount_RtnDepthMarketData(const std::string InstrumentID);
    ///�����ر�ʱ����̬�����ʽ��˻���Ϣ
    void DynamicUpdateTradingAccount_RtnOrder(const PlatformStru_OrderInfo& Order, 
                                              const PlatformStru_OrderInfo& OldOrderInfo);
    ///�ɽ��ر�ʱ����̬�����ʽ��˻���Ϣ
    void DynamicUpdateTradingAccount_RtnTrade(void);
    ///��ѯ�ֲ���Ӧʱ����̬�����ʽ��˻���Ϣ
    void DynamicUpdateTradingAccount_RspQryInvestorPositionLast(void);
    ///��ѯ�ɽ���Ӧʱ����̬�����ʽ��˻���Ϣ
    void DynamicUpdateTradingAccount_RspQryTrade(void);
    ///��ѯ������Ӧʱ����̬�����ʽ��˻���Ϣ
    void DynamicUpdateTradingAccount_RspQryOrder(void);
    ///��ѯ��֤������Ӧʱ����̬�����ʽ��˻���Ϣ
    void DynamicUpdateTradingAccount_RspQryRate(void);
    ///��ѯ����������Ӧʱ����̬�����ʽ��˻���Ϣ
    void DynamicUpdateTradingAccount_RspQryInstrumentCommissionRate(void);

    
    void SetAccountField(DataRspQryTradingAccountEx& TradingAccountField, const CThostFtdcTradingAccountFieldEx& field );

	void SetAccountField(DataRspQryTradingAccount& field );

    //�����ʽ������еĶ�̬Ȩ��ֵ
    void RefreshDynamicProficOfTradingAccount(PlatformStru_TradingAccountInfo& TradingAccountField);
    //�����ʽ������еķ��ն�ֵ
    void RefreshRiskDegreeOfTradingAccount(PlatformStru_TradingAccountInfo& TradingAccountField);
    //�����ʽ������еĿ����ʽ�
    void RefreshAvailOfTradingAccount(PlatformStru_TradingAccountInfo& TradingAccountField);

	const string Instrument2ExchangeID(const string& Instrument);
    ///��ȡָ����Լ��Ϣ
    void SetInstrumentInfo(const std::string& InstrumentID, const PlatformStru_InstrumentInfo& inData);
    ///��ȡָ����Լ��Ϣ,0�ɹ���-1ʧ��
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
	///��Ӳ�ѯ�޼�¼�������
	void IncrementReqMarginRateError(const std::string& InstrumentID);
	///��Ӳ�ѯ�޼�¼�������
	void IncrementReqCommissionRateError(const std::string& InstrumentID);

    ///���õ�ǰ��ʾ�ĳֲ����ݡ�1:�ֲ֣�2:�ֲ���ϸ��3:��ϳֲ�
    void SetCurrentPositionContent(int PositionContentMode);




    //��ѯǰ��ո���map
    void ClearMapQryRltOrders();
    void ClearMapQryRltTrades();
    void ClearMapQryRltPositionDetails(std::string& strQryInstrument);
    void ClearMapQryRltPositions(std::string& strQryInstrument);
    void ClearMapQryRltPositionCombs(std::string& strQryInstrument);
    


    //----------�����Ǳ����ӿ�----------
    //��ѯ�ı�������
    void QryOrderReach(PlatformStru_OrderInfo& OrderInfo,const int ErrID,const bool bIsLast);
	//�±�������
    void NewOrderReady(PlatformStru_OrderInfo& OrderInfo, PlatformStru_OrderInfo& OldOrderInfo, const PlatformStru_InstrumentInfo& InstrumentInfo);
    //�������б����еĶ��ᱣ֤�𣬷��غ�Լ����Ƿ��б����������Ƿ���Ҫ�����ʽ���塣��ѯ��֤������Ӧʱ����
    bool UpdateOrderFreezeMargin_RspQryMarginRate(std::string&	strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo);
    //�����б����еĶ��������ѣ����غ�Լ����Ƿ��б����������Ƿ���Ҫ�����ʽ���塣��ѯ����������Ӧʱ����
    bool UpdateOrderFreezeCommission_RspQryCommissionRate(std::string&	strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo);
    //���㱨����Ϣ�еĶ��ᱣ֤��
    void CalcuOrderFreezeMargin_Internal(PlatformStru_OrderInfo& OrderInfo,const PlatformStru_InstrumentInfo& InstrumentInfo);
    //���㱨����Ϣ�еĶ���������
    void CalcuOrderFreezeCommission_Internal(PlatformStru_OrderInfo& OrderInfo,const PlatformStru_InstrumentInfo& InstrumentInfo);

    ///��ȡָ������
    bool GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData);
    ///��ȡȫ������
    int GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///��ȡȫ������
    int GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);

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
    int GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, const char CloseType,const char HedgeFlag=THOST_FTDC_HF_Speculation);
    //��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
    int GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag=THOST_FTDC_HF_Speculation);
    //��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ڲ����ã����������ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
	int  GetCloseOrderVolume_Internal(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag);
    //ȡȫ���ҵ��Ķ��������ѺͶ��ᱣ֤���ڲ����ã��������������û�з��ʣ���0����
    void GetFreezeMargin_Commission_Internal(double& FreezeMargin,double& FreezeCommission);
    


    //----------�����ǳɽ���¼�ӿ�----------
    //ԭʼ�ĳɽ��ر�����
    void QryTradeReach( PlatformStru_TradeInfo& TradeInfo,const int ErrID,const bool bIsLast);
    //�µĳɽ��ر�����
    void NewTradeReady( PlatformStru_TradeInfo& TradeInfo,const PlatformStru_InstrumentInfo& InstrumentInfo);
    //��ȡ�������ʺ󣬸������гɽ��е������ѣ����غ�Լ����Ƿ��гɽ�
    bool UpdateTradeCommission_RspQryCommissionRate(std::string& strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo);
    //����ɽ�������
    void CalcuTradeCommission_Internal(PlatformStru_TradeInfo& TradeInfo,const PlatformStru_InstrumentInfo& InstrumentInfo);
    ///��ȡȫ���ɽ���
    int GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData);
    ///��ȡָ���ɽ���Ϣ
    bool GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData);
    ///��ȡȫ���ɽ���
    int GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData);

    //�ӳɽ���ȡ�����ѡ����û�з��ʣ���0����
    void GetAllTradeCommission_Internal(double& TotalCommission);
    ///��ȡ�ɽ�ͳ��
	double GetShouXuFei(const PlatformStru_TradeInfo& TradeInfo);


    //----------�����ǳɽ�ͳ�ƽӿ�----------
    void QryTradeTotalReach(const bool bIsLast);
	//�ɽ��ر�ʱ�����³ɽ�ͳ��
    void UpdateTradeTotal_RtnTrade(const PlatformStru_TradeInfo& TradeInfo);
	//�������������Ӧʱ��������Ӧ��Լ�ĳɽ�ͳ��
	bool UpdateTradeTotal_RspQryCommissionRate(const std::string& strInstrument);
    void UpdateTradeTotalByTrade_Internal(const PlatformStru_TradeInfo& TradeInfo);
    bool GetTradeTotalDataValue_Internal(const TradeTotalKey& TotalKey,PlatformStru_TradeTotalInfo& TradeTotalInfo);
    ///���ݳɽ����³ɽ�ͳ��
    bool UpdateTradeTotalDataByTrade_Internal(const PlatformStru_TradeInfo& TradeInfo,PlatformStru_TradeTotalInfo& TradeTotalInfo);
    ///��ȡȫ���ɽ�ͳ�Ƽ�¼
    int GetAllTradeTotals(std::vector<PlatformStru_TradeTotalInfo>& outData);
    ///��ȡָ����Լ�ĳɽ�ͳ�Ƽ�¼�ɹ�����0��ʧ�ܷ���-1
    int GetTradeTotalOfInstrument(const std::string& strInstrument, std::vector<PlatformStru_TradeTotalInfo>& outData);


    //----------�����ǳֲ���ϸ�ӿ�----------
    //��ѯ�ĳֲ���ϸ��¼����
    void QryPositionDetailReach(PlatformStru_PositionDetail& PositionDetailInfo,int ErrID,const bool bIsLast);
    //ȡ���ֲ���ϸ�еļ�¼����
    int GetRecordCountOfPositionDetail(void);
    //�ӳɽ���¼���³ֲ���ϸ������ƽ���ĳֲ���ϸ�ļ�¼�������ں������㡣ModifiedPositionDetails�����ܵ�Ӱ��ĳֲ���ϸ��¼��n*2+0��ԭ�������ݣ�n*2+1��������
    void UpdatePositionDetail_RtnTrade(PlatformStru_TradeInfo& TradeInfo,
                                        std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                        const PlatformStru_InstrumentInfo& InstrumentInfo);
    //�ӿ��ֳɽ���¼�����ֲ���ϸ
    void CreatePositionDetailFromOpenTrade_Internal(PlatformStru_PositionDetail& PositionDetailInfo,PlatformStru_TradeInfo& TradeInfo);
    //��ƽ�ֳɽ���¼���³ֲ���ϸ��ModifiedPositionDetails�����ܵ�Ӱ��ĳֲ���ϸ��¼��n*2+0��ԭ�������ݣ�n*2+1��������
    void UpdatePositionDetailFromCloseTrade_Internal(PlatformStru_TradeInfo& TradeInfo,
                                                        std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                                        const PlatformStru_InstrumentInfo& InstrumentInfo);
//	//�ҿ���ʱ������ͳɽ������С�ĳֲ���ϸ��¼
//	bool FindEarlierOpenPosition( std::string strInstrument, 
//		PlatformStru_TradeInfo &TradeInfo, 
//		std::map<PositionDetailKey,PlatformStru_PositionDetail>::iterator& it_prePosition,
//		std::map<PositionDetailKey,PlatformStru_PositionDetail>::iterator& it_minall );
    //�жϳֲ���ϸ�ǲ���(ƽ��)�ɽ��Ķ��ֲֳ�
    bool IsOpponentPositionDetailOfTrade_IgnoreTradeType(PlatformStru_PositionDetail& PositionDetail,PlatformStru_TradeInfo& TradeInfo);
    //��ȡĳһ��(���)�ֲ���ϸ�Ķ��ֲֳ���ϸ��¼��ע�⣬ֻ���������Խ�ֻ����
    //���ڳɽ����³ֲ���ϸʱ��ɢ��ϳֲֵļ��㡣��ȡʱ�õ���ϳֲ���ϸ������
    PlatformStru_PositionDetail& GetOpponentPositionDetailComb(const PlatformStru_PositionDetail& OriPositionDetail);
    //��ͨ�ɽ�(ƽ��)��Ҫƽ��ϵ�ʱ����ɢ���ֲֳ���ϸ
    bool BreakupPositionDetailComb(PlatformStru_PositionDetail&OppoPositionDetail,int ClosedVolume,
                                    std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails);


	int CloseSinglePosition_Internal( int CloseVolume, PlatformStru_PositionDetail &positionDetail, 
                             int nVolumeMultiple, double dClosePrice);

	//��̬���³ֲ���ϸ��¼�ĳֲ�ӯ���͸���ӯ�����õ�������ʱ��һ�顣���������bForceCalculateΪfalse����ʾ���ڼ۸�䶯ʱ���£��������Ϊtrue
    void UpdatePositionDetailByQuot(PlatformStru_DepthMarketData& QuotData,const PlatformStru_InstrumentInfo& InstrumentInfo,bool bForceCalculate);
    //��̬���³ֲ���ϸ��¼�ĳֲ�ӯ���͸���ӯ��
    void UpdatePositionDetailByQuot_Internal(PlatformStru_DepthMarketData& QuotData,
                                                PlatformStru_PositionDetail& PositionDetail,
                                                const PlatformStru_InstrumentInfo InstrumentInfo,
                                                bool bForceCalculate);
    //��̬���³ֲ���ϸ��¼�ı�֤�𡢱�֤���ʣ���ѯ��֤������Ӧʱ����
    bool UpdatePositionDetailByMarginRate_RspQryRate(PlatformStru_InstrumentMarginRate& MarginRate,
                                            const PlatformStru_InstrumentInfo& InstrumentInfo);
    //��̬���³ֲ���ϸ��¼�ı�֤�𡢱�֤����
    void UpdatePositionDetailByMarginRate_Internal(PlatformStru_InstrumentMarginRate& MarginRate,
                                                    PlatformStru_PositionDetail& PositionDetail,
                                                    const PlatformStru_InstrumentInfo& InstrumentInfo);

    //��ȡ�ֲ���ϸ��¼
    int GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq);
    int GetPositionDetails3(const std::string& strInstrument,
                              std::set<long>& setFTID,
                              std::vector<PlatformStru_PositionDetail>& vecValue,
                              long& lastseq);
    bool HavePositionDetail(const std::string& strInstrumentID);
    //��ȡ��ѯ�ĺ�ԼID
    void GetQryPositionDetailInstrument(std::string& outQryInstrument);



    //----------�����ǳֲֽӿ�----------
	//��ѯ�ֲ�ʱ�ϲ�ͬһ��Լͬһ�������ʷ�ֲֺͽ��ճֲּ�¼
	void MergePositions_RspQryPosition( PlatformStru_Position& firstPositionInfo, PlatformStru_Position secondPositionInfo );
    //��ѯ�ĳֲּ�¼����
    void QryPositionReach(PlatformStru_Position& PositionInfo,const int ErrID,const bool bIsLast);
    //�ӳɽ���¼���³ֲ֡��˺����ӳֲ���ϸ�����´���������ֲ֡������ڳֲ���ϸ�������ڳֲ���ϸ��������ٸ��³ֲ�
    void UpdatePosition_RtnTrade(PlatformStru_TradeInfo& TradeInfo, 
                                    std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                    const PlatformStru_InstrumentInfo& InstrumentInfo);
	//���³ֲ������ж��Ƶ���ָ������CTP���ص���
	void UpdatePositionFieldsWithoutCTPSupported_QryPositionReach_Internal(PlatformStru_Position& PositionData,
                                                                            const PlatformStru_InstrumentInfo& InstrumentInfo);
    //��̬���³ֲּ�¼�ĳֲ�ӯ�����۸�ֵ���õ�������ʱ��һ��
    void UpdatePositionByQuot(PlatformStru_DepthMarketData& QuotData,
                                const PlatformStru_InstrumentInfo& InstrumentInfo,
                                bool bForceCalculate);
    //��̬���³ֲּ�¼�ĳֲ�ӯ��(����)������ӯ��(���)�Ͷ�����ӯ��������ۺ������ۣ���������
    void UpdatePositionByQuot_Internal(PlatformStru_DepthMarketData& QuotData,
                                        PlatformStru_Position& PositionInfo,
                                        const PlatformStru_InstrumentInfo& InstrumentInfo,
                                        bool bForceCalculate);
    //��̬���³ֲּ�¼�ı�֤�𡢱�֤���ʣ���ѯ��֤������Ӧʱ����
    bool UpdatePositionByMarginRate_RspQryRate(PlatformStru_InstrumentMarginRate& MarginRate);
    //��̬���³ֲּ�¼�ı�֤�𡢱�֤���ʣ���ѯ��֤������Ӧ�͵õ��״�����ʱ����
    void UpdatePositionByMarginRate_RspQryRate_FirstQuot_Internal(PlatformStru_InstrumentMarginRate& MarginRate,PlatformStru_Position& PositionInfo,bool bForce);
    //��̬���³ֲּ�¼�ı�֤�𡢱�֤���ʣ��ɽ��޸ĳֲ�ʱ����
    void UpdatePositionByMarginRate_RtnTrade_Internal(PlatformStru_Position& PositionInfo,
                                                        std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails); 
    //��̬���³ֲּ�¼�������ѣ���ѯ����������Ӧʱ����
    bool UpdatePositionByCommissionRate_RspQryRate(PlatformStru_InstrumentCommissionRate& CommissionRate);
    //��̬���³ֲּ�¼�������ѣ���ѯ����������Ӧʱ����
    void UpdatePositionByCommissionRate_RspQryRate_Internal(PlatformStru_Position& PositionInfo);
	//���ʲ�ѯ��Ӧʱ�����³ֲ���صĹҵ��Ķ��������ѺͶ��ᱣ֤��
	void CPlatformDataMgr::UpdatePositionFreezeFieldsByOrders_RspQryRate(const std::string& strInstrument);
	//�����ر�ʱ�����³ֲ���صĹҵ��Ķ��������ѺͶ��ᱣ֤��
	void UpdatePositionFreezeFields_RtnOrder(const PlatformStru_OrderInfo& Order, 
                                                const PlatformStru_OrderInfo& OldOrderInfo);
	//ȡ�ֲ���صĹҵ��Ķ��������ѺͶ��ᱣ֤��
	void UpdatePositionFreezeFieldsByAllOrders_Internal(PlatformStru_Position& PositionData);
	//ȡ�ֲ���صĹҵ��Ķ��������ѺͶ��ᱣ֤�𣬸���һ�������ı仯��
	void UpdatePositionFreezeFieldsByOneOrder_Internal(PlatformStru_Position& PositionData, 
                                                        const PlatformStru_OrderInfo& Order, 
                                                        const PlatformStru_OrderInfo& OldOrderInfo);

    //�ڳֲ������У�����ƽ�����Ϳ�ƽ����, (�������йҵ�)����ȫ��
    void CalcuCloseValueInPositionByAllOrders_Internal(PlatformStru_Position& PositionData,
                                                        const int CloseVolume,
                                                        const int CloseTodayVolume,
                                                        const int CloseYesterdayVolume);
    //�ڳֲ������У�����ƽ�����Ϳ�ƽ����, (����һ���ҵ�)������������
    //Deltaxxx������
    void CalcuCloseValueInPositionByOneOrder_Internal(PlatformStru_Position& PositionData,
                                                        const int DeltaCloseVolume,
                                                        const int DeltaCloseTodayVolume,
                                                        const int DeltaCloseYesterdayVolume);

    //��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
    int GetPositionKeySet(std::set<PositionKey> &PositionKeySet);
    //��ȡָ����Լ������Ͷ���ĳֲ����ݣ��ɹ�����0��ʧ�ܷ���-1
    int GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData);
    int GetPositions(std::vector<PlatformStru_Position>& outData);
    int GetPositions2(const std::string& strInstrument,
                        std::set<long>& setFTID,
                        std::vector<PlatformStru_Position>& outData,
                        long& lastseq);
    int GetPositions3(const std::string& strInstrument,
                        std::vector<PlatformStru_Position>& outData,
                        long& lastseq);
    //��ȡ��ѯ�ĺ�ԼID
    void GetQryPositionInstrument(std::string& outQryInstrument);



    //----------��������ϳֲ���ϸ�ӿ�----------
    //��ѯ��ϳֲּ�¼�Ľ��������ע�⣬�����ǲ�ѯָ����Լ����ϳֲ�
    void QryPositionCombReach(PlatformStru_PositionComb& PositionCombInfo,const int ErrID,const bool bIsLast);
    //��ȡ�����ϳֲּ�¼
    int GetPositionCombs(std::vector< PlatformStru_PositionComb > &PositionCombMap);
	int GetPositionCombs2(const std::string& strTradeID, std::string& strInstrument,std::vector<PlatformStru_PositionComb>& outData);
    //��ȡ�ɽ�ID��Ӧ����ϳֲ���ϸ��¼
    PlatformStru_PositionComb& GetPositionComb3_Internal(const std::string& strTradeID);
    //��ȡ��ѯ�ĺ�ԼID
    void GetQryPositionCombInstrument(std::string& outQryInstrument);
	//�ϲ���ʼ����ѯ�õ��ĵ��ȳֲ�Ϊһ����ϳֲ�
	bool MergeOriSingleLegPosition( const PlatformStru_PositionComb &PositionCombo, PlatformStru_PositionComb& newComboPosition );
	//��ϵ�����
	void CreatePositionDetailCombFromOpenTrade_Internal( PlatformStru_TradeInfo &TradeInfo );





    //��ȡָ����Լ���飬�ɹ�����0��ʧ�ܷ���-1
    int GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
    //����ָ����Լ���飬���õ�������£��򷵻�true�����򷵻�false
    bool SetQuotInfo(const PlatformStru_DepthMarketData& inData,bool& bFirstQuot);
    //���������Ⱥ�Լ�ϳ���Ϻ�Լ�����سɹ���񡣺ϳ�ʱ�����Լ�б����Ƿ��и���Ϻ�Լ
	bool GetComboInstrumentID_Internal(const std::string strInstrumentID1, 
                                        const std::string strInstrumentID2, 
                                        std::string& strComInstrumentID,
                                        std::string& strLeg1InstrumentID,
                                        std::string& strLeg2InstrumentID);
    //��ȡָ����Լ�����¼�
    double GetCurPrice (const std::string& InstrumentID);
    //��ȡָ����Լ�����飬�ɹ�����0��ʧ�ܷ���-1
    int GetOldQuotInfo (const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);

    //����Ͷ������Ϣ
    void SetInvestorInfo(const DataRspQryInvestor& inData);

    string GetTradingDay(const string& ExchangeID);
    //��ȡ������ʱ����Ϣ
    unsigned long GetExchangeTime(const string& ExchangeID,string& time);
    //���ý�����ʱ����Ϣ
    int GetExchangeTime(ExchangeTime& outData);
    //���ý�����ʱ����Ϣ
    void SetExchangeTime(const ExchangeTime& inData);
	//��ȡȫ��������
	int GetExchangeIDs(vector<string>& outData);

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

		//ҵ���ƣ��ɹ�����0��ʧ�ܷ���-1
    int SubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID, const GuiCallBackFunc& callback);
    ///ȡ��ҵ�����ݵĶ��ƣ��ɹ�����0��ʧ�ܷ���-1
    int UnSubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID);
    ///��ȡҵ������Ϣ���ɹ�����0��ʧ�ܷ���-1
    int GetSubscribeBusinessData(const BusinessID BID, std::map<GUIModuleID,GuiCallBackFunc>& outData);

	//
	SERVER_PARAM_CFG&  GetServerParamCfg();
    void SetModuleDataQry(bool bQry);
    bool GetModuleDataQry();

    ///��̬�����ʽ��˻��еĽ���Ӷ�𣬲�ѯ�ɽ����ء��ɽ��ر�����ѯ���ʷ���
    void DynamicUpdateTradingAccount_TradeCommission_Internal();

	//�ж�һ���ַ��������Ƿ��ǵ���
	bool IsTodayPosition(const char *OpenDate);
    bool IsTodayPositionDetail(const PlatformStru_PositionDetail& PositionDetail);

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

private:

    //----------�����Ǳ������ݼ�----------
    CPlatformDataSetList<OrderKey,PlatformStru_OrderInfo> m_OrderData;

    //----------�����ǳɽ����ݼ�----------
    CPlatformDataSetList<TradeKey,PlatformStru_TradeInfo> m_TradeData;

    //----------�����ǳɽ�ͳ�����ݼ�----------
    CPlatformDataSetList<TradeTotalKey,PlatformStru_TradeTotalInfo> m_TradeTotalData;

    //----------�����ǳֲ���ϸ���ݼ�----------
    CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail> m_PositionDetailData;

    //----------�����ǳֲ����ݼ�----------
    CPlatformDataSetList<PositionKey,PlatformStru_Position> m_PositionData;

    //----------��������ϳֲ����ݼ�----------
    CPlatformDataSetList<PositionCombKey,PlatformStru_PositionComb> m_PositionCombData;




    //��Լ��Ϣ�������������ݼ��ڲ�ѯ��Լ��Ϣ����еõ���������gui����
    std::map<std::string,  PlatformStru_InstrumentInfo> m_allInstruments; //��ǰ���к�Լ��Ϣ��ϵͳ��ʼ��ʱ��ȡ
    std::set<std::string>  m_allInstrumentList;                         //��ǰ���к�Լ�б�
    std::set<std::string>  m_allInstrumentListWithoutComb;              //��ǰ���к�Լ�б��ų���Ϻ�Լ��
    std::map<std::string, std::set<std::string> > m_allProductID;       //���в�Ʒ����

    std::set<std::string>  m_allMainInstrumentList;                         //��ǰ������Լ�б�

	std::map<string, string> m_allInstrumentVarietyMap;
    bool m_bNeedCheckCommissionRateAndMarginRate;                       //�Ƿ���Ҫ����������ʺͱ�֤���ʣ������º�Լ�б�ʱ����ֵΪtrue����ѯ��󣬴�ֵΪfalse

	bool m_bHavePostionDetail;
    std::map<std::string, PlatformStru_InstrumentCommissionRate> m_InstrumentCommissionRate;//��������
    std::string m_ReqCommissionRateInstrumentID;                 //���ڲ�ѯ�������ʵĺ�Լ
    std::map<std::string, PlatformStru_InstrumentMarginRate> m_InstrumentMarginRate;//��֤����


    //�ʽ��˻���Ϣ���ʽ��˻���ϵͳ����ǰ��ѯһ�Σ�Ȼ����ݱ����ر��ͳɽ��ر���̬����
    DataRspQryTradingAccount m_TradingAccount;                          //�ʽ��˻���Ϣ


    //�������ݼ�
    std::map<std::string,  PlatformStru_DepthMarketData> m_lastQuot;  //��������
    std::map<std::string,  PlatformStru_DepthMarketData> m_OldQuot;  //�ϴ�������


    //�������ݼ�
    DataRspQryInvestor m_InvestorInfo;//Ͷ���߸�����Ϣ
    std::string m_LastSettlementInfo;
    bool m_bSettlementInfoIsReady;
    std::map<BusinessID, std::map<GUIModuleID, GuiCallBackFunc> >  m_subscribeInfo;
	////��Ϲ�ϵ<ID1,<ALL,ID2>>,<ID2,<ALL,ID1>>
	//std::multimap<std::string,std::vector<std::string>> m_CombRelation;
//	std::multimap<std::string,std::string> m_mapCombineRelation;
//	std::map<pair<std::string, std::string>, std::string> m_ComboInstruments;
	//����ƥ����ϳֲ��еļ������ȵ����ݼ�
	std::map<std::string, PlatformStru_PositionComb> m_combinLegs;
	//��ʼ�ֲ���ϸ�����ڲ�ѯ��Ϻ�Լ�ɽ���
	std::map<std::string, PlatformStru_PositionDetail> m_tempOriPositionDetail;	
	//������Ϻ�Լ�е�һ�����ȣ�����ƥ������һ������
	std::map<std::string, PlatformStru_TradeInfo> m_tempTradeCombinLegs;

    ExchangeTime m_exchangeTime; //������ʱ��

    zqWriteLog*           m_pWriteLog;

    CRITICAL_SECTION m_CS;

    //ר����Ժ�Լ��Ϣ��������m_allInstruments��m_allInstrumentList��m_allProductID
    CRITICAL_SECTION m_CS_InstrumentInfo;

    //ר������������
    CRITICAL_SECTION m_CS_Quot;


	SERVER_PARAM_CFG m_ServerParamCfg;
    //��һ���ɽ���ϸ���µ�m_allPositionDetails��
    //���أ��ɹ����
    //ע�⣺m_allPositionDetails�е�ͬһ��Լ�ĳֲ���ϸ��¼�������Ⱥ�˳������
    //bool AssignTradeInfoToPositionDetail(PlatformStru_TradeInfo& TradeInfo);



    ///ָ����Լ�Ƿ�Ϸ�(����)
    bool InstrumentIsValid(const std::string& InstrumentID);
    ///��ȡ��Լ��֤���ʣ��ڲ����ã����ü���
    int GetMarginRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
    ///��ȡ��Լ�������ʣ��ڲ����ã����ü���
    int GetCommissionRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);

    //ȡȫ���ֲֵ�ռ�ñ�֤��ͳֲ�ӯ��(���ն���)�����û�з��ʣ���0����
    void GetAllMargin_CloseProfitByDate_Internal(double& TotalMargin,double&TotalCloseProfitByDate);
    //ȡȫ���ֲֵ�ռ�ñ�֤��ͳֲ�ӯ��(���ն���)�����û�з��ʣ���0����
    void GetAllPositionProfitByDate_Internal(double&TotalPositionProfitByDate);
    //ȡȫ���ֲֵ�ƽ��ӯ��(���ն���)
    void GetAllCloseProfitByDate_Internal(double&TotalCloseProfitByDate);

    void TotalCloseProfitByDate_Internal(double& TotalCloseProfitByDate);
	std::map<std::string,  PlatformStru_ContractBank> m_ContractBankInfo;
	std::map<std::string,  CThostFtdcContractBankField> m_BankName;
	std::map<std::string, int> m_reqMarginRateError;//����֤���ѯ�޼�¼����
	std::map<std::string, int> m_reqCommissionRateError;//���������Ѳ�ѯ�޼�¼����
    bool m_ModuleDataQry;

protected:
	CPlatFormService* m_PlatFormService;


};

