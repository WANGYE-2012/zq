#pragma once

#include <string>
#include <set>
#include <map>
using namespace std;

#include "UserApiStruct.h"

class ForceManualForm2
{
public:
    ForceManualForm2(void);
    ~ForceManualForm2(void);

    static ForceManualForm2* NewInstance() {return new ForceManualForm2();}
    static void DestroyInstance(ForceManualForm2* instance) {if(instance!=NULL) delete instance;}
public:
    //���ı�������
    static void SubscribeOrderRsp(unsigned int hWnd);
    //��������
    static void SubscribeOrderPush(unsigned int hWnd);
    //�˶���������
    static void UnsubscribeOrderRsp(unsigned int hWnd);

    //����������鷵��
    static void SubscribeQuotRsp(unsigned int hWnd);
    //�����������
    static void SubscribeQuotPush(unsigned int hWnd);
    //�˶�������鷵��
    static void UnsubscribeQuotRsp(unsigned int hWnd);

    //���ĳֲַ���
    static void SubscribePositionRsp(unsigned int hWnd);
    //�ֲ�����
    static void SubscribePositionPush(unsigned int hWnd);
    //�˶��ֲַ���
    static void UnsubscribePositionRsp(unsigned int hWnd);

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

    //����������ı�������
    static void ReqSubscribeOrder(int accountID); 
    //����������������������
    static void ReqSubscribeQuot(TInstrumentIDType szInstrumentID); 
    //��������˶������������
    static void UnreqSubscribeQuot(TInstrumentIDType szInstrumentID); 
    //����������ĳֲ�����
    static void ReqSubscribePosition(int accountID);  
    //������������ʽ�����
    static void ReqSubscribeFund(int accountID);
    //�����������ǿƽ����¼��
    static void ReqForceCloseOrderInsert(RiskForceCloseOrderField& oRiskForceCloseOrderField);

    //������������͵ķ����¼���Ϣ
    static bool ProcessRiskEvtPkg(const char* pPkg,string*pstr,int fieldcnt);

public:
    void SetOrderRefToId(std::string szOrderRef,int nCount);
    void GetOrderRefToId(std::map<std::string,int>& mapOrderRefToId);
    void DeleteOrderRefToId(std::string orderRef);
    void ResetOrderRefToId(int nId);
private:
    std::map<std::string,int> m_mapOrderRefToId;//�ҵ�Ref��Ӧ�к�
};