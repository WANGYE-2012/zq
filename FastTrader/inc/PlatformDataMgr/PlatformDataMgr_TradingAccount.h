/*
    ���ڱ����ά��CTP���˻��ʽ�����
    added by l. 20130204
*/


#pragma once

#include <string>

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Trades.h"
#include "PlatformDataMgr_Orders.h"
#include "PlatformDataMgr_PositionDetails.h"
#include "PlatformDataMgr_Positions.h"
#include "PlatformDataMgr_PositionDetailCombs.h"
#include "PlatformDataMgr_PositionCombs.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 


class CPlatformDataMgr_TradingAccount
{
public:
    CPlatformDataMgr_TradingAccount(CPlatformDataMgr_Instruments& refInstruments,
        CPlatformDataMgr_Quots& refQuots,
        CPlatformDataMgr_Trades& refTrades,
        CPlatformDataMgr_Orders& refOrders,
        CPlatformDataMgr_PositionDetailCombs& refPositionDetailCombs,
        CPlatformDataMgr_PositionDetails& refPositionDetails,
        CPlatformDataMgr_PositionCombs& refPositionCombs,
        CPlatformDataMgr_Positions& refPositions);
    ~CPlatformDataMgr_TradingAccount(void);

    ///���ý����ʽ��˻���Ϣ
    void SetTradingAccount(const PlatformStru_TradingAccountInfo& inData);
    int GetTradingAccountAvailable(double& fAvailable);
	double GetTradingAccountDynamicProfit(void);
    int GetTradingAccountWithdrawQuota(double& fWithdrawQuota);
    int GetTradingAccountID(char* AccountID,int rltsize);
    int GetTradingAccount(PlatformStru_TradingAccountInfo& outData);

    ///��ȡ�˻��ʽ��ı���Ϣ
    int GetAccountText(std::string& outData,int language);
    ///��ѯ����\��֤������Ӧʱ����̬�����ʽ��˻���Ϣ
    void UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData(void);
    ///�������ʱ����̬�����ʽ��˻���Ϣ
    void DynamicUpdateTradingAccount_RtnDepthMarketData(const std::string InstrumentID);
    ///�ɽ��ر�ʱ����̬�����ʽ��˻���Ϣ
    void UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted(void);
    ///��ѯ��֤������Ӧʱ����̬�����ʽ��˻���Ϣ
    void DynamicUpdateTradingAccount_RspQryRate(void);
    ///��ѯ����������Ӧʱ����̬�����ʽ��˻���Ϣ
    void DynamicUpdateTradingAccount_RspQryInstrumentCommissionRate(void);

    //�����������
    void clear(void);
    bool IsValid(void);

private:
    //�����ʽ������еľ�̬Ȩ��ֵ
    void CalcuStaticProfic_Internal();
    //�����ʽ������еĶ�̬Ȩ��ֵ
    void CalcuDynamicProfic_Internal();
    //�����ʽ������еĿ����ʽ�
    void CalcuAvailable_Interanl();
    //�����ʽ������еķ��ն�ֵ
    void CalcuRiskDegree_Internal();

private:
    Ceasymutex				            m_mutex;

    CPlatformDataMgr_Instruments        &m_RefInstruments;
    CPlatformDataMgr_Quots              &m_RefQuots;
    CPlatformDataMgr_Trades             &m_RefTrades;
    CPlatformDataMgr_Orders             &m_RefOrders;
    CPlatformDataMgr_PositionDetailCombs&m_RefPositionDetailCombs;
    CPlatformDataMgr_PositionDetails    &m_RefPositionDetails;
    CPlatformDataMgr_PositionCombs      &m_RefPositionCombs;
    CPlatformDataMgr_Positions          &m_RefPositions;


    //�ʽ��˻���Ϣ���ʽ��˻���ϵͳ����ǰ��ѯһ�Σ�Ȼ����ݱ����ر��ͳɽ��ر���̬����
    PlatformStru_TradingAccountInfo     m_TradingAccount;   //�ʽ��˻���Ϣ
    bool                                m_bValid;           //���ó�ʼ�ʽ�������Ч��

};

#ifdef WIN32
#pragma managed(pop)
#endif 
