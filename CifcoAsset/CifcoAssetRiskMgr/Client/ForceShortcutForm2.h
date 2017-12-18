#pragma once

#include <set>
#include <string>
using namespace std;

#include "UserApiStruct.h"

enum CLOSE_TYPE
{
    CLOSE_TYPE_TODAY,
    CLOSE_TYPE_YESTERDAY
};

enum RISK_STATUS
{
    RISK_STATUS_HANDLING
};

enum FORCE_OPERATER
{
    FORCE_OPERATER_MANUAL
};
struct ForceShortcutFormStruct
{
    int nRiskID; 
    int nAccountID;
    int nHandleRiskPersonID;
    int nHandledRiskPersonID;
    RISK_STATUS nRiskStatus;
    FORCE_OPERATER nForceOperater;
    string sTime;
    string sRemark;
};

class ForceShortcutForm2
{
public:
    ForceShortcutForm2(void);
    ~ForceShortcutForm2(void);
public:
    //����������鷵��
    static void SubscribeQuotRsp(unsigned int hWnd);
    //�����������
    static void SubscribeQuotPush(unsigned int hWnd);
    //�˶�������鷵��
    static void UnsubscribeQuotRsp(unsigned int hWnd);

    //�����ʽ𷵻�
    static void SubscribeFundRsp(unsigned int hWnd);
    //�ʽ�����
    static void SubscribeFundPush(unsigned int hWnd);
    //�˶��ʽ𷵻�
    static void UnsubscribeFundRsp(unsigned int hWnd);

    //����ǿƽ����¼�뷵��
    static void SubscribeForceCloseOrderInsertRsp(unsigned int hWnd);

    //���ĳɽ�����
    static void SubscribeTradeRsp(unsigned int hWnd);
    //�ɽ�����
    static void SubscribeTradePush(unsigned int hWnd);
    //�˶��ɽ�����
    static void UnsubscribeTradeRsp(unsigned int hWnd);

    //�˶�ȫ���¼�
    static void UnsubscribeAllEvt(unsigned int hWnd);

    //����������������������
    static void ReqSubscribeQuot(TInstrumentIDType szInstrumentID); 
    //������������ʽ�����
    static void ReqSubscribeFund(int nAccount);
    //����������ĳɽ�����
    static void ReqSubscribeTrade(int nAccount);
    //�����������ǿƽ����¼��
    static void ReqForceCloseOrderInsert(RiskForceCloseOrderField& oRiskForceCloseOrderField);
    

    //������������͵ķ����¼���Ϣ
    static bool ProcessRiskEvtPkg(const char* pPkg,string*pstr,int fieldcnt);
};
