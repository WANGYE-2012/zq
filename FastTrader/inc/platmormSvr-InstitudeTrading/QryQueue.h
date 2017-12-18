#pragma once


#include <map>
#include <list>
#include <set>
#include <string>
#include "Module-Misc2/BusinessData.hpp"
#include "Module-Misc2/packagecache.h"
//#include "Module-Misc2/platformstruct.h"


class CPackageCache;
class zqWriteLog;


//���涨��ÿ�ֲ�ѯ�ĺ�
#define QryID_ReqQryInstrument                      1001            //��ѯ��Լ��Ϣ
#define QryID_ReqQryTradingAccount                  1002            //��ѯ�ʽ�
#define QryID_ReqQryTrade                           1003            //��ѯ�ɽ�
#define QryID_ReqQryOrder                           1004            //��ѯ����
#define QryID_ReqQryInvestorPositionDetail          1005            //��ѯ�ֲ���ϸ
#define QryID_ReqQryInvestorPosition                1006            //��ѯ�ֲֻ���
#define QryID_ReqQryInstrumentCommissionRate        1007            //��ѯ��Լ��������
#define QryID_ReqQryInstrumentMarginRate            1008            //�����ѯ��Լ��֤����
#define QryID_ReqQrySettlementInfo                  1009            //�����ѯͶ���߽�����
#define QryID_ReqQryDepthMarketData                 1010            //�����ѯ����

#define QryID_ReqQryInvestorPositionCombineDetail   1011            //�����ѯͶ���ֲ߳���ϸ(���)
#define QryID_ReqQryNotice                          1012            //�����ѯ�ͻ�֪ͨ
#define QryID_ReqQrySettlementInfoConfirm           1013            //�����ѯ������Ϣȷ��
#define QryID_ReqQryTransferBank                    1014            //�����ѯת������
#define QryID_ReqQryTransferSerial                  1015            //�����ѯת����ˮ
#define QryID_ReqQryContractBank                    1016            //�����ѯǩԼ����
#define QryID_ReqQryAccountregister                 1017            //�����ѯ����ǩԼ��ϵ
#define QryID_ReqQryParkedOrder                     1018            //�����ѯԤ��
#define QryID_ReqQryParkedOrderAction               1019            //�����ѯԤ�񳷵�
#define QryID_ReqQueryBankAccountMoneyByFuture      1020            //�ڻ������ѯ�����������
#define QryID_ReqQryExchangeRate                    1021            //�����ѯ����

#define QryID_ReqQryCFMMCTradingAccountKey          1030            //�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ

class CPlatFormService;
class CPlatFormParam;
class CQryQueue
{
public:
    CQryQueue();
    ~CQryQueue(void);
    void StopThread();
	void SetPlatFormService(CPlatFormService* pPlatFormService){m_pPlatFormService=pPlatFormService;}
    //�Ƿ���Ҫ�߳��˳�
    bool IsThreadNeedExit(void);                                  

    //�Ƿ񵽴��ѯʱ�̡�ctp�涨ÿ��ֻ����һ�β�ѯ 
    bool IsQryTime(void);
	//
	bool TradeStatusIsConnected(void);
    //���²�ѯʱ��
    void UpdateQryTime(void);

    //���Ͳ�ѯ���󣬷���ֵ��ʾ�����Ƿ�ɹ�
    //ReqRlt��ʾ��������˲�ѯ����ѯ����ֵ��<0��ʾ��ѯʧ��
    bool SendQryCmd(int& ReqRlt);

    //���ReqChache�Ƿ�Ϊ��
    bool IsCacheEmpty(void);
    bool IsCache2Empty(void);
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

    //�����ѯ���������ָ�����͵Ĳ�ѯ��������TCP��������ز�ǰ�Ĵ���
    //ReqCmdID:1:�ʽ��ѯ��2:�ɽ���ѯ��4:������ѯ��8:�ֲ���ϸ��ѯ��16:�ֲֲ�ѯ��32:��ϳֲ���ϸ��ѯ
    //ReqCmdID���Ի���
    void ClearReqInQryQueue(int ReqCmdID);


    static void ClearbInQry(void) { m_bInQry=false; }
    //static void NotifyStartStopInitQry(bool bStart) { m_bDoingInitQry=bStart?true:false; }


private:

    CPackageCache *m_pReqQueue;                             //���Ȳ�ѯ���С����а��Ƚ��ȳ�˳���ѯ
    CPackageCache *m_pReqQueue2;                            //�μ���ѯ���У��ŷ��ʲ�ѯ���󡣶��а��Ƚ��ȳ�˳���ѯ    
    bool    m_bNeedExit;                                    //�߳��Ƿ���Ҫ�˳�
    DWORD   m_LastQryTime;                                  //�ϴβ�ѯ��ʱ��
    HANDLE  m_hThread;                                      //�߳̾��
    
    //�̺߳���
    static DWORD ThreadProc(void *arg);

    CPlatFormService *m_pPlatFormService;

    static bool m_bInQry;                                   //�Ƿ��ڲ�ѯ;��
    //static bool m_bDoingInitQry;                            //���ڽ��г�ʼ����ѯ����ʱ���ܲ����

    zqWriteLog*           m_pWriteLog;
    std::map<std::string,unsigned int> m_QryCommissionRateTime;
    std::map<std::string,int> m_QryCommissionRateCount;
    std::map<std::string,unsigned int> m_QryMarginRateTime;
    std::map<std::string,int> m_QryMarginRateCount;

};

