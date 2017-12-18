#include "StdAfx.h"
#include "ForceShortcutForm2.h"

#include <windows.h>
#include "RiskManageCmd.h"
#include "RiskManageStruct.h"
#include "CommonPkg.h"
#include "TcpLayer.h"


ForceShortcutForm2::ForceShortcutForm2(void)
{
}

ForceShortcutForm2::~ForceShortcutForm2(void)
{
}

//����������鷵��
void ForceShortcutForm2::SubscribeQuotRsp(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_SubscribeQuot_Rsp,(int)hWnd);
}
//�����������
void ForceShortcutForm2::SubscribeQuotPush(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_Quot_Push,(int)hWnd);
}
//�˶�������鷵��
void ForceShortcutForm2::UnsubscribeQuotRsp(unsigned int hWnd)
{
    CTcpLayer::UnsubscribePkg(Cmd_RM_UnSubscribeQuot_Rsp,(int)hWnd);
}
//�����ʽ𷵻�
void ForceShortcutForm2::SubscribeFundRsp(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_SubscribeFund_Rsp,(int)hWnd);
}
//�ʽ�����
void ForceShortcutForm2::SubscribeFundPush(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_Fund_Push,(int)hWnd);
}
//�˶��ʽ𷵻�
void ForceShortcutForm2::UnsubscribeFundRsp(unsigned int hWnd)
{
    CTcpLayer::UnsubscribePkg(Cmd_RM_UnSubscribeFund_Rsp,(int)hWnd);
}
//����ǿƽ����¼�뷵��
void ForceShortcutForm2::SubscribeForceCloseOrderInsertRsp(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_ForceCloseOrderInsert_Rsp,(int)hWnd);
}
//���ĳɽ�����
void ForceShortcutForm2::SubscribeTradeRsp(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_SubscribeTrade_Rsp,(int)hWnd);
}
//�ɽ�����
void ForceShortcutForm2::SubscribeTradePush(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_Trade_Push,(int)hWnd);
}
//�˶��ɽ�����
void ForceShortcutForm2::UnsubscribeTradeRsp(unsigned int hWnd)
{
    CTcpLayer::UnsubscribePkg(Cmd_RM_UnSubscribeTrade_Rsp,(int)hWnd);
}
//�˶�ȫ���¼�
void ForceShortcutForm2::UnsubscribeAllEvt(unsigned int hWnd)
{
    CTcpLayer::UnsubscribeAllPkg((int)hWnd);
}
//����������������������
void ForceShortcutForm2::ReqSubscribeQuot(TInstrumentIDType szInstrumentID)
{   
   CTcpLayer::SendData(Cmd_RM_SubscribeQuot_Req, (void*)szInstrumentID, sizeof(TInstrumentIDType), 0);
}
//������������ʽ�����
void ForceShortcutForm2::ReqSubscribeFund(int nAccount)
{
    CTcpLayer::SendData(Cmd_RM_SubscribeFund_Req, (void*)&nAccount, sizeof(int), 0);
}
//����������ĳɽ�����
void ForceShortcutForm2::ReqSubscribeTrade(int nAccount)
{
    CTcpLayer::SendData(Cmd_RM_SubscribeTrade_Req, (void*)&nAccount, sizeof(int), 0);
}
//�����������ǿƽ����¼��
void ForceShortcutForm2::ReqForceCloseOrderInsert(RiskForceCloseOrderField& oRiskForceCloseOrderField)
{
    CTcpLayer::SendData(Cmd_RM_ForceCloseOrderInsert_Req, (void*)&oRiskForceCloseOrderField, sizeof(RiskForceCloseOrderField), 0);
}
//������������͵ķ����¼���Ϣ
bool ForceShortcutForm2::ProcessRiskEvtPkg(const char* pPkg,string*pstr,int fieldcnt)
{
    for(int i=0;i<fieldcnt;i++)
    {
        char buf[256];
        itoa(i,buf,10);
        pstr[i].assign(buf);
    }
    return true;
}
