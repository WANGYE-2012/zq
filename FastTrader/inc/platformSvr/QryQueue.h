#pragma once


#include <map>
#include <list>
#include <vector>
using std::map;
using std::string;
using std::vector;

#include "../Module-Misc2/BusinessData.hpp"
#include "../Module-Misc2/packagecache.h"
//#include "../Module-Misc2/platformstruct.h"


class CPackageCache;
class zqWriteLog;


//���涨��ÿ�ֲ�ѯ�ĺ�
enum
{
    QryID_ReqBrokerTradingParam=1001,           //��ѯ���͹�˾���ײ���
    QryID_ReqQryProduct,                        //��ѯ��Լ��Ϣ
    QryID_ReqQryInstrument,                     //��ѯ��Լ��Ϣ
    QryID_ReqQryTradingAccount,                 //��ѯ�ʽ�
    QryID_ReqQryTrade,                          //��ѯ�ɽ�
    QryID_ReqQryOrder,                          //��ѯ����
    QryID_ReqQryInvestorPositionDetail,         //��ѯ�ֲ���ϸ
    QryID_ReqQryInvestorPosition,               //��ѯ�ֲֻ���
    QryID_ReqQryInstrumentCommissionRate,       //��ѯ��Լ��������
    QryID_ReqQryInstrumentMarginRate,           //�����ѯ��Լ��֤����
    QryID_ReqQrySettlementInfo,                 //�����ѯͶ���߽�����
    QryID_ReqQryDepthMarketData,                //�����ѯ����

    QryID_ReqQryInvestorPositionCombineDetail,  //�����ѯͶ���ֲ߳���ϸ(���)
    QryID_ReqQryNotice,                         //�����ѯ�ͻ�֪ͨ
    QryID_ReqQrySettlementInfoConfirm,          //�����ѯ������Ϣȷ��
    QryID_ReqQryTransferBank,                   //�����ѯת������
    QryID_ReqQryTransferSerial,                 //�����ѯת����ˮ
    QryID_ReqQryContractBank,                   //�����ѯǩԼ����
    QryID_ReqQryAccountregister,                //�����ѯ����ǩԼ��ϵ
    QryID_ReqQryParkedOrder,                    //�����ѯԤ��
    QryID_ReqQryParkedOrderAction,              //�����ѯԤ�񳷵�
    QryID_ReqQueryBankAccountMoneyByFuture,     //�ڻ������ѯ�����������
    QryID_ReqQryExchangeRate,                   //�����ѯ����
    QryID_ReqQryExecOrder,                      //�����ѯִ������
    QryID_ReqQueryMaxOrderVolume,               //�����ѯ��󱨵���������

    QryID_ReqQryCFMMCTradingAccountKey,         //�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
};

class CPlatFormService;
class CPlatFormParam;
class CQryQueue
{
public:
    CQryQueue(CPlatFormService& PlatFormService);
    ~CQryQueue(void);
    void StopThread();
    //�Ƿ���Ҫ�߳��˳�
    bool IsThreadNeedExit(void);                                  

    //�Ƿ񵽴��ѯʱ�̡�ctp�涨ÿ��ֻ����һ�β�ѯ 
    bool IsQryTime(void);
	//
	bool TradeStatusIsConnected(void);
    //���²�ѯʱ��
    void UpdateQryTime(void);

    //���Ͳ�ѯ���󣬷���ֵ��ʾ�����Ƿ�ɹ�
    bool SendQryCmd();

    //���ReqChache�Ƿ�Ϊ��
    bool IsCacheEmpty(void);
    bool IsCache2Empty(void);

    //�����ѯ���͹�˾���ײ���
    void ReqQryBrokerTradingParams(int nRequestID);

    //�����ѯƷ��
    int ReqQryProduct(PlatformStru_QryProduct &QryProduct, int nRequestID);

    //�����ѯ��Լ
    int ReqQryInstrument(PlatformStru_QryInstrument &QryInstrument, int nRequestID);

    //�����ѯ�ʽ��˻�
    int ReqQryTradingAccount(PlatformStru_QryTradingAccount &QryTradingAccount, int nRequestID);

    //�����ѯ�ɽ�
    int ReqQryTrade(PlatformStru_QryTrade &QryTrade, int nRequestID);

    //�����ѯ����
    int ReqQryOrder(PlatformStru_QryOrder &QryOrder, int nRequestID);

    //�����ѯͶ���ֲ߳���ϸ
    int ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail &QryInvestorPositionDetail, int nRequestID);

    //�����ѯͶ���ֲ߳�
    int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition &QryInvestorPosition, int nRequestID);

    //�����ѯ��Լ��������
    int ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField &QryInstrumentCommissionRate, int nRequestID);

    //�����ѯ��Լ��֤����
    int ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField &QryInstrumentMarginRate, int nRequestID) ;

    //�����ѯͶ���߽�����
    int ReqQrySettlementInfo(PlatformStru_QrySettlementInfo &QrySettlementInfo, int nRequestID);

    //�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
    int ReqQryCFMMCTradingAccountKey(PlatformStru_QryCFMMCTradingAccountKey &QryCFMMCTradingAccountKey, int nRequestID);

    //�����ѯ����
    int ReqQryDepthMarketData(PlatformStru_QryDepthMarketData &QryDepthMarketData, int nRequestID);

    ///�����ѯͶ���ֲ߳���ϸ(���)
    int ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail &pQryInvestorPositionCombineDetail, int nRequestID=0) ;

    ///�����ѯ�ͻ�֪ͨ
    int ReqQryNotice(PlatformStru_QryNotice &pQryNotice, int nRequestID=0) ;

    ///�����ѯ������Ϣȷ��
    int ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm &pQrySettlementInfoConfirm, int nRequestID=0) ;

    ///�����ѯת������
    int ReqQryTransferBank(PlatformStru_QryTransferBank &pQryTransferBank, int nRequestID=0) ;

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

    ///�ڻ������ѯ�����������
    int ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount &pReqQueryAccount, int nRequestID=0) ;

    //�����ѯִ������
	int ReqQryExecOrder(PlatformStru_QryExecOrderField& QryExecOrder, int nRequestID);

    //��ѯ��󱨵���������
    int ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume &QueryMaxOrderVolume, int nRequestID);

    //�����ѯ���������ָ�����͵Ĳ�ѯ��������TCP��������ز�ǰ�Ĵ���
    //ReqCmdID:1:�ʽ��ѯ��2:�ɽ���ѯ��4:������ѯ��8:�ֲ���ϸ��ѯ��16:�ֲֲ�ѯ��32:��ϳֲ���ϸ��ѯ
    //ReqCmdID���Ի���
    void ClearReqInQryQueue(int ReqCmdID);



private:
    //�ж�Cache�е������ѯ�����ݰ��Ƿ���ȷ
    bool IsReqQryPackageInCacheValid(const PackageHeadStru& PackageHead,const void* pPackageData);



private:

    CPackageCache *m_pReqQueue;                             //���Ȳ�ѯ���С����а��Ƚ��ȳ�˳���ѯ
    CPackageCache *m_pReqQueue2;                            //�μ���ѯ���У��ŷ��ʲ�ѯ���󡣶��а��Ƚ��ȳ�˳���ѯ    
    bool    m_bNeedExit;                                    //�߳��Ƿ���Ҫ�˳�
    DWORD   m_LastQryTime;                                  //�ϴβ�ѯ��ʱ��
    HANDLE  m_hThread;                                      //�߳̾��
    
    //�̺߳���
    static DWORD ThreadProc(void *arg);

    CPlatFormService& m_RefPlatFormService;


    zqWriteLog*             m_pWriteLog;
    map<string,time_t>      m_QryCommissionRateTime;
    map<string,int>         m_QryCommissionRateCount;
    map<string,time_t>      m_QryMarginRateTime;
    map<string,int>         m_QryMarginRateCount;

    bool                    m_bStartQryRate;                //�Ƿ��Ѿ���ʼ��ѯ���ʡ���m_rServiceProxy.m_bInitQrySuccΪtrue֮���ٿ�ʼ��ѯ����
    vector<string>          m_InstrumentIDsNeedQry;         //��Ҫ��ѯ���ʵĺ�Լ����ѯʱ����˳����С�
    vector<string>          m_InstrumentIDsNeedQry2;        //��Ҫ��ѯ���ʵĺ�Լ�����ȼ��ϵ͡����ĳ��Լ��ѯ�޷��ʣ���ͬƷ��������Լ�Ƶ��˶��У�����ٲ顣
    int                     m_QryRateCount;                 //��ѯ���ʵĴ�����ͷ4�Σ���InstrumentIDΪ�յķ�ʽ��ѯ�������ʡ���֤���ʡ���Ȩ�������ʡ���Ȩ���׳ɱ����Եõ��гֲֵķ�������

};

