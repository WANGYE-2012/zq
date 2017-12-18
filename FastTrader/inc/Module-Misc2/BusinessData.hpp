#ifndef _BUSINESSDATA_HPP_
#define _BUSINESSDATA_HPP_

#include "KernelStruct.h" 
#include "globalDefines.h"
#include "Tools_Ansi.h"
#include <string>

#ifdef WIN32
#pragma managed(push,off)
#endif 





#define PATSYSTEM_FTDC_OPT_Market              'G'
#define PATSYSTEM_FTDC_OPT_Limit               'H'
#define PATSYSTEM_FTDC_OPT_LimitFAK            'I'
#define PATSYSTEM_FTDC_OPT_LimitFOK            'J'
#define PATSYSTEM_FTDC_OPT_Stop                'K'
#define PATSYSTEM_FTDC_OPT_SynthStop           'L'
#define PATSYSTEM_FTDC_OPT_SynthStopLimit      'M'
#define PATSYSTEM_FTDC_OPT_MIT                 'N'
#define PATSYSTEM_FTDC_OPT_SynthMIT            'O'
#define PATSYSTEM_FTDC_OPT_MarketFOK           'P'
#define PATSYSTEM_FTDC_OPT_MOO                 'Q'
#define PATSYSTEM_FTDC_OPT_CrossingBatchType   'R'
#define PATSYSTEM_FTDC_OPT_BasisBatchType      'S'
#define PATSYSTEM_FTDC_OPT_BlockBatchType      'T'
#define PATSYSTEM_FTDC_OPT_AABatchType         'U'
#define PATSYSTEM_FTDC_OPT_IOC                 'V'
#define PATSYSTEM_FTDC_OPT_StopRise            'W'
#define PATSYSTEM_FTDC_OPT_StopFall            'X'
#define PATSYSTEM_FTDC_OPT_RFQ                 'Y'
#define PATSYSTEM_FTDC_OPT_StopLoss            'Z'

enum BusinessID
{
    BID_MIN = 0,
    BID_RspUserLogin = BID_MIN,     //��¼������Ӧ,��Ϊ���׵���,�������
    BID_RspUserLogout,              //�ǳ�������Ӧ,��Ϊ���׵ǳ�,����ǳ�
    BID_RspUserPasswordUpdate,      //�û��������������Ӧ
    BID_RspTradingAccountPasswordUpdate,  //�ʽ��˻��������������Ӧ
    BID_RspOrderInsert,            //����¼��������Ӧ(�����ִ)(OnRspOrderInsert ���ݡ��ۺϽ���ƽ̨API����FAQ��:CTP�ڼ��ί�з��ִ���ʱ���������ί�е�Ͷ���߷���OnRspInsertOrder��ͬʱ����OnErrRtnOrder����صĽ���Ա�����ԣ���ΪͶ���߿��Բ�����OnErrRtnOrder)
    BID_ErrRtnOrderInsert,			//BID_RspOrderInsert2, //����¼���ִ((�����ִ2����������Ϊ¼����Ч)(OnErrRtnOrder)
    BID_RspOrderAction,            //��������������Ӧ
    BID_ErrRtnOrderAction,          //����������ִ(�����ִ2����������Ϊ������Ч)
    BID_RtnOrder,                   //�����ر�(����¼���뱨��������������ûر�)
    BID_RtnTrade,                   //�ɽ��ر�
    BID_RtnDepthMarketData,         //�������
    BID_RspQryDepthMarketData,      //��ѯ���������Ӧ
    BID_RspParkedOrderInsert,       //Ԥ��¼��������Ӧ
    BID_RspParkedOrderAction,       //Ԥ�񳷵�¼��������Ӧ
    BID_RspRemoveParkedOrder,       //ɾ��Ԥ����Ӧ    
    BID_RspRemoveParkedOrderAction, //ɾ��Ԥ�񳷵���Ӧ
    BID_RspQryParkedOrder,          //�����ѯԤ����Ӧ
    BID_RspQryParkedOrderAction,    //�����ѯԤ�񳷵���Ӧ
    BID_RspQueryMaxOrderVolume,     //��ѯ��󱨵�������Ӧ
    BID_RspSettlementInfoConfirm,   //Ͷ���߽�����ȷ����Ӧ
    BID_RspQryOrder,                //�����ѯ������Ӧ
    BID_RspQryTrade,                //�����ѯ�ɽ���Ӧ
    BID_RspQryInvestorPosition,     //�����ѯͶ���ֲ߳���Ӧ
    BID_RspQryTradingAccount,       //�����ѯ�ʽ��˻���Ӧ
    BID_RspQryInvestor,             //�����ѯͶ������Ӧ
    BID_RspQryTradingCode,          //�����ѯ���ױ�����Ӧ
    BID_RspQryInstrumentMarginRate, //�����ѯ��Լ��֤������Ӧ   
    BID_RspQryInstrumentCommissionRate,//�����ѯ��Լ����������Ӧ
    BID_RspQryOptionInstrCommRate,  //�����ѯ��Ȩ��Լ��������Ӧ
    BID_RspQryOptionInstrTradeCost, //�����ѯ��Ȩ���׳ɱ���Ӧ
    BID_RspQryExchange,             //�����ѯ��������Ӧ
    BID_RspQryProduct,              //�����ѯ��Ʒ��Ӧ
    BID_RspQryInstrument,           //�����ѯ��Լ��Ӧ
    BID_RspQrySettlementInfo,       //�����ѯͶ���߽�������Ӧ
    BID_RspQryTransferBank,         //�����ѯת��������Ӧ
    BID_RspQryInvestorPositionDetail,//�����ѯͶ���ֲ߳���ϸ��Ӧ
    BID_RspQryNotice,               //�����ѯ�ͻ�֪ͨ��Ӧ
    BID_RspQrySettlementInfoConfirm,//�����ѯ������Ϣȷ����Ӧ
    BID_RspQryInvestorPositionCombDetail, //�����ѯͶ���ֲ߳���ϸ��Ӧ
    BID_RtnInstrumentStatus,                 //��Լ����״̬֪ͨ
    BID_RtnTradingNotice,                    //����֪ͨ
    BID_RspQryContractBank,                  //�����ѯǩԼ������Ӧ
    BID_RspQryTradingNotice,                 //�����ѯ����֪ͨ��Ӧ
    BID_RspQryBrokerTradingParams,           //�����ѯ���͹�˾���ײ�����Ӧ
    BID_RspQryBrokerTradingAlgos,            //�����ѯ���͹�˾�����㷨��Ӧ

    BID_RspFromBankToFutureByFuture,        //�ڻ����������ʽ�ת�ڻ�Ӧ��
    BID_RspFromFutureToBankByFuture,        //�ڻ������ڻ��ʽ�ת����Ӧ��
    BID_RspQueryBankAccountMoneyByFuture,   //�ڻ������ѯ�������Ӧ��
    BID_RspQryTransferSerial,               //�����ѯת����ˮ��Ӧ
    BID_RspQryAccountregister,              //�����ѯ����ǩԼ��ϵ��Ӧ

    BID_RtnFromBankToFutureByFuture,        //�ڻ����������ʽ�ת�ڻ��ر�
    BID_RtnFromFutureToBankByFuture,        //�ڻ������ڻ��ʽ�ת���лر�
    BID_RtnQueryBankAccountMoneyByFuture,   //�ڻ������ѯ�������ر�

    BID_RspQryExchangeRate,                 //�����ѯ���ʷ���

    BID_RspForQuoteInsert,                  //ѯ��¼��������Ӧ
    BID_RspExecOrderInsert,                 //ִ������¼��������Ӧ
    BID_RspExecOrderAction,                 //ִ���������������Ӧ
    BID_RspQryExecOrder,                    //�����ѯִ��������Ӧ
    BID_ErrRtnForQuoteInsert,               //ѯ��¼�����ر�
    BID_ErrRtnExecOrderInsert,              //ִ������¼�����ر�
    BID_ErrRtnExecOrderAction,              //ִ�������������ر�
    BID_RtnExecOrder,                       //ִ������֪ͨ
    BID_RspSubForQuoteRsp,                  //����ѯ��Ӧ��
    BID_RspUnSubForQuoteRsp,                //�˶�ѯ��Ӧ��
    BID_RtnForQuoteRsp,                     //ѯ��֪ͨ

    BID_FrontDisconnected,                  //�뽻�׺�̨ͨ�����ӶϿ�
    BID_FrontConnected,					    //�뽻�׺�̨ͨ�����ӳɹ�

    BID_QryStart,                           //��ѯ��ʼ������Ϊstring����ʾ��ѯ����
    BID_ConnectivityStatus,
    BID_AlertMessage,			            //Pats Alert Message
    BID_ExchangeRate,		
    BID_MAX
};

struct AbstractBusinessData
{
    AbstractBusinessData()
    :   BID(BID_MIN),
        PlatformID(0),
        Size(0)
    {
    };

    AbstractBusinessData(BusinessID bID,unsigned int size)
    :   BID(bID),
        PlatformID(0),
        Size(size)
    {
    };

    BusinessID                 BID;
	int						   PlatformID;
    unsigned int               Size;//�϶����ڵ���sizeof(AbstractBusinessData)
};

typedef int (*GuiCallBackFunc)(const GUIModuleID,const AbstractBusinessData&);

//����������Ӧ
struct DataRspUserLogin
{
    DataRspUserLogin(bool btrader,int requestID,bool bisLast,unsigned long curTickCount,const CThostFtdcRspUserLoginField& userLoginField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspUserLogin,(unsigned int)sizeof(*this)),
        bTrader(btrader),
        nRequestID(requestID),
        bIsLast(bisLast),
        CurTickCount(curTickCount),
        UserLoginField(userLoginField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    bool                      bTrader;//�Ƿ���
    int                       nRequestID;
    bool                      bIsLast;
    unsigned long             CurTickCount;//���ص��Կ�����ĺ�������
    CThostFtdcRspUserLoginField UserLoginField;
    CThostFtdcRspInfoField    RspInfoField; 
};

//�ǳ�������Ӧ
struct DataRspUserLogout
{
    DataRspUserLogout(int type,int requestID,bool bisLast,const CThostFtdcUserLogoutField& userLogoutField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspUserLogout,(unsigned int)sizeof(*this)),
        Type(type),
        nRequestID(requestID),
        bIsLast(bisLast),
        UserLogoutField(userLogoutField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       Type;//0:����, 1:����
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcUserLogoutField UserLogoutField;
    CThostFtdcRspInfoField    RspInfoField; 
};


//�û��������������Ӧ
struct DataRspUserPasswordUpdate
{
    DataRspUserPasswordUpdate()
    :   Head(BID_RspUserPasswordUpdate,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&UserPasswordUpdateField,0,sizeof(UserPasswordUpdateField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspUserPasswordUpdate(int requestID,bool bisLast,const CThostFtdcUserPasswordUpdateField& userPasswordUpdateField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspUserPasswordUpdate,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        UserPasswordUpdateField(userPasswordUpdateField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcUserPasswordUpdateField UserPasswordUpdateField;
    CThostFtdcRspInfoField    RspInfoField; 
};

//�ʽ��˻��������������Ӧ
struct DataRspTradingAccountPasswordUpdate
{
    DataRspTradingAccountPasswordUpdate()
    :   Head(BID_RspTradingAccountPasswordUpdate,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&TradingAccountPasswordUpdateField,0,sizeof(TradingAccountPasswordUpdateField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspTradingAccountPasswordUpdate(int requestID,bool bisLast,const CThostFtdcTradingAccountPasswordUpdateField& tradingAccountPasswordUpdateField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspTradingAccountPasswordUpdate,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        TradingAccountPasswordUpdateField(tradingAccountPasswordUpdateField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcTradingAccountPasswordUpdateField TradingAccountPasswordUpdateField;
    CThostFtdcRspInfoField    RspInfoField; 
};

//Ͷ���߽�����ȷ����Ӧ
struct DataRspSettlementInfoConfirm
{
    DataRspSettlementInfoConfirm()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspSettlementInfoConfirm;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcSettlementInfoConfirmField      SettlementInfoConfirmField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//Ԥ��¼���ִ(��Ϊ�ɹ���ִ������ִ)
struct DataRspParkedOrderInsert
{
    DataRspParkedOrderInsert()
    :   Head(BID_RspParkedOrderInsert,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&ParkedOrder,0,sizeof(ParkedOrder));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspParkedOrderInsert(int requestID,bool bisLast,const CThostFtdcParkedOrderField& parkedOrder,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspParkedOrderInsert,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ParkedOrder(parkedOrder),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcParkedOrderField ParkedOrder;
    CThostFtdcRspInfoField    RspInfoField;    
};

//Ԥ�񳷵�¼��������Ӧ
struct DataRspParkedOrderAction
{
    DataRspParkedOrderAction()
    :   Head(BID_RspParkedOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&ParkedOrderAction,0,sizeof(ParkedOrderAction));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspParkedOrderAction(int requestID,bool bisLast,const CThostFtdcParkedOrderActionField& parkedOrderAction,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspParkedOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ParkedOrderAction(parkedOrderAction),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcParkedOrderActionField ParkedOrderAction;
    CThostFtdcRspInfoField     RspInfoField;    
};


//ɾ��Ԥ����Ӧ
struct DataRspRemoveParkedOrder
{
    DataRspRemoveParkedOrder()
    :   Head(BID_RspRemoveParkedOrder,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&RemoveParkedOrder,0,sizeof(RemoveParkedOrder));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspRemoveParkedOrder(int requestID,bool bisLast,const CThostFtdcRemoveParkedOrderField& removeParkedOrder,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspRemoveParkedOrder,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        RemoveParkedOrder(removeParkedOrder),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcRemoveParkedOrderField RemoveParkedOrder;
    CThostFtdcRspInfoField     RspInfoField;    
};

//ɾ��Ԥ�񳷵���Ӧ
struct DataRspRemoveParkedOrderAction
{
    DataRspRemoveParkedOrderAction()
    :   Head(BID_RspRemoveParkedOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&RemoveParkedOrderAction,0,sizeof(RemoveParkedOrderAction));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspRemoveParkedOrderAction(int requestID,bool bisLast,const CThostFtdcRemoveParkedOrderActionField& removeParkedOrderAction,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspRemoveParkedOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        RemoveParkedOrderAction(removeParkedOrderAction),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcRemoveParkedOrderActionField RemoveParkedOrderAction;
    CThostFtdcRspInfoField     RspInfoField;    
};

//�����ѯԤ����Ӧ
struct DataRspQryParkedOrder
{
    DataRspQryParkedOrder()
    :   Head(BID_RspQryParkedOrder,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&ParkedOrder,0,sizeof(ParkedOrder));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryParkedOrder(int requestID,bool bisLast,const CThostFtdcParkedOrderField& parkedOrder,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryParkedOrder,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ParkedOrder(parkedOrder),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcParkedOrderField      ParkedOrder;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�����ѯԤ�񳷵���Ӧ
struct DataRspQryParkedOrderAction
{
    DataRspQryParkedOrderAction()
    :   Head(BID_RspQryParkedOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&ParkedOrderAction,0,sizeof(ParkedOrderAction));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryParkedOrderAction(int requestID,bool bisLast,const CThostFtdcParkedOrderActionField& parkedOrderAction,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryParkedOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ParkedOrderAction(parkedOrderAction),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcParkedOrderActionField      ParkedOrderAction;
    CThostFtdcRspInfoField    RspInfoField;    
};


//����¼���ִ(��Ϊ�ɹ���ִ������ִ)
struct DataRspOrderInsert
{
    DataRspOrderInsert()
    :   Head(BID_RspOrderInsert,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InputOrderField,0,sizeof(InputOrderField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspOrderInsert(int requestID,bool bisLast,const CThostFtdcInputOrderField& OrderField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspOrderInsert,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InputOrderField(OrderField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcInputOrderField InputOrderField;
    CThostFtdcRspInfoField    RspInfoField;    
};
struct DataErrRtnOrderInsert
{
	DataErrRtnOrderInsert()
    :   Head(BID_ErrRtnOrderInsert,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
	{
        memset(&InputOrderField,0,sizeof(InputOrderField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataErrRtnOrderInsert(int requestID,bool bisLast,const CThostFtdcInputOrderField& OrderField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_ErrRtnOrderInsert,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InputOrderField(OrderField),
        RspInfoField(RspInfo)
	{
	};

	AbstractBusinessData      Head;
	int                       nRequestID;
	bool                      bIsLast;
	CThostFtdcInputOrderField InputOrderField;
	CThostFtdcRspInfoField    RspInfoField;    
};


//����������ִ(�����ִ1���ۺϽ���ƽ̨��Ϊ������Ч)
struct DataRspOrderAction
{
    DataRspOrderAction()
    :   Head(BID_RspOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InputOrderActionField,0,sizeof(InputOrderActionField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspOrderAction(int requestID,bool bisLast,const CThostFtdcInputOrderActionField& OrderActionField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InputOrderActionField(OrderActionField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcInputOrderActionField InputOrderActionField;
    CThostFtdcRspInfoField     RspInfoField;    
};

//����������ִ(�����ִ2����������Ϊ������Ч)
struct DataErrRtnOrderAction
{
    DataErrRtnOrderAction()
    :   Head(BID_ErrRtnOrderAction,(unsigned int)sizeof(*this))
    {
        memset(&OrderActionField,0,sizeof(OrderActionField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataErrRtnOrderAction(const CThostFtdcOrderActionField& orderActionField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_ErrRtnOrderAction,(unsigned int)sizeof(*this)),
        OrderActionField(orderActionField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    CThostFtdcOrderActionField OrderActionField;
    CThostFtdcRspInfoField     RspInfoField;    
};

//�����ر�(�µ����󣬻��������Ϣ����������Ҳ���������Ϣ)
struct DataRtnOrder
{
    DataRtnOrder()
    :   Head(BID_RtnOrder,(unsigned int)sizeof(*this))
    {
        memset(&OrderField,0,sizeof(OrderField));
    };
    DataRtnOrder(const PlatformStru_OrderInfo& orderField)
    :   Head(BID_RtnOrder,(unsigned int)sizeof(*this)),
        OrderField(orderField)
    {
    };

    AbstractBusinessData      Head;
    PlatformStru_OrderInfo      OrderField; 
};

//�ɽ��ر�
struct DataRtnTrade
{
    DataRtnTrade()
    :   Head(BID_RtnTrade,(unsigned int)sizeof(*this))
    {
        memset(&TradeField,0,sizeof(TradeField));
    };
    DataRtnTrade(const PlatformStru_TradeInfo& tradeField)
    :   Head(BID_RtnTrade,(unsigned int)sizeof(*this)),
        TradeField(tradeField)
    {
    };

    AbstractBusinessData      Head;
    PlatformStru_TradeInfo      TradeField; 
};

//�������֪ͨ
struct DataRtnDepthMarketData
{
    DataRtnDepthMarketData()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RtnDepthMarketData;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData                      Head;
    PlatformStru_DepthMarketData    DepthMarketDataField; 
    bool                            bHavePosition;  //��ʾ�Ƿ�����Ч�ֲ�
};

//�����ѯͶ���ֲ߳���Ӧ
struct DataRspQryInvestorPosition
{
    DataRspQryInvestorPosition()
    :   Head(BID_RspQryInvestorPosition,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InvestorPositionField,0,sizeof(InvestorPositionField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
		memset(QryInstrumentID,0,sizeof(QryInstrumentID));
    };
    DataRspQryInvestorPosition(int requestID,bool bisLast,const PlatformStru_Position& investorPositionField,const CThostFtdcRspInfoField& RspInfo,const TThostFtdcInstrumentIDType& qryInstrumentID)
    :   Head(BID_RspQryInvestorPosition,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InvestorPositionField(investorPositionField),
        RspInfoField(RspInfo)
    {
		memcpy(QryInstrumentID,qryInstrumentID,sizeof(QryInstrumentID));
    };

    AbstractBusinessData        Head;
    int                         nRequestID;
    bool                        bIsLast;
    PlatformStru_Position       InvestorPositionField;
    CThostFtdcRspInfoField      RspInfoField;    
  	TThostFtdcInstrumentIDType	QryInstrumentID;        //��ѯ�ĺ�ԼID
};


//�����ѯͶ���ֲ߳���ϸ��Ӧ
struct DataRspQryInvestorPositionDetail
{
    DataRspQryInvestorPositionDetail()
    :   Head(BID_RspQryInvestorPositionDetail,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InvestorPositionDetailField,0,sizeof(InvestorPositionDetailField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
		memset(QryInstrumentID,0,sizeof(QryInstrumentID));
    };
    DataRspQryInvestorPositionDetail(int requestID,bool bisLast,const PlatformStru_PositionDetail& investorPositionDetailField,const CThostFtdcRspInfoField& RspInfo,const TThostFtdcInstrumentIDType& qryInstrumentID)
    :   Head(BID_RspQryInvestorPositionDetail,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InvestorPositionDetailField(investorPositionDetailField),
        RspInfoField(RspInfo)
    {
		memcpy(QryInstrumentID,qryInstrumentID,sizeof(QryInstrumentID));
    };

    AbstractBusinessData		Head;
    int							nRequestID;
    bool						bIsLast;
    PlatformStru_PositionDetail	InvestorPositionDetailField;
    CThostFtdcRspInfoField		RspInfoField;    
  	TThostFtdcInstrumentIDType	QryInstrumentID;        //��ѯ�ĺ�ԼID
};

//�����ѯͶ���ֲ߳���Ϻ�Լ��ϸ��Ӧ
struct DataRspQryInvestorPositionCombDetail
{
    DataRspQryInvestorPositionCombDetail()
    :   Head(BID_RspQryInvestorPositionCombDetail,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InvestorPositionCombineDetailField,0,sizeof(InvestorPositionCombineDetailField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
		memset(QryInstrumentID,0,sizeof(QryInstrumentID));
    };
    DataRspQryInvestorPositionCombDetail(int requestID,bool bisLast,const PlatformStru_PositionDetailComb& investorPositionCombineDetailField,const CThostFtdcRspInfoField& RspInfo,const TThostFtdcInstrumentIDType& qryInstrumentID)
    :   Head(BID_RspQryInvestorPositionCombDetail,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InvestorPositionCombineDetailField(investorPositionCombineDetailField),
        RspInfoField(RspInfo)
    {
		memcpy(QryInstrumentID,qryInstrumentID,sizeof(QryInstrumentID));
    };

    AbstractBusinessData		Head;
    int							nRequestID;
    bool						bIsLast;
    PlatformStru_PositionDetailComb	InvestorPositionCombineDetailField;
    CThostFtdcRspInfoField		RspInfoField;    
  	TThostFtdcInstrumentIDType	QryInstrumentID;        //��ѯ�ĺ�ԼID
};

//�����ѯ�ʽ��˻���Ӧ
struct DataRspQryTradingAccount
{
    DataRspQryTradingAccount()
    :   Head(BID_RspQryTradingAccount,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&TradingAccountField,0,sizeof(TradingAccountField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryTradingAccount(int requestID,bool bisLast,const PlatformStru_TradingAccountInfo& tradingAccountField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryTradingAccount,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        TradingAccountField(tradingAccountField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_TradingAccountInfo TradingAccountField;
    CThostFtdcRspInfoField     RspInfoField;    
};

struct DataRspQryTradingAccountEx
{
	DataRspQryTradingAccountEx()
	{
		memset(this, 0, sizeof(*this));
		Head.BID = BID_RspQryTradingAccount;
		Head.Size = sizeof(*this);
	};
	AbstractBusinessData      Head;
	int                       nRequestID;
	bool                      bIsLast;
	CThostFtdcTradingAccountFieldEx TradingAccountField;
	CThostFtdcRspInfoField     RspInfoField;    
};

//�����ѯͶ������Ӧ
struct DataRspQryInvestor
{
    DataRspQryInvestor()
    :   Head(BID_RspQryInvestor,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InvestorField,0,sizeof(InvestorField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryInvestor(int requestID,bool bisLast,const CThostFtdcInvestorField& investorField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryInvestor,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InvestorField(investorField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcInvestorField   InvestorField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�����ѯ���ױ�����Ӧ
struct DataRspQryTradingCode
{
    DataRspQryTradingCode()
    :   Head(BID_RspQryTradingCode,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&TradingCodeField,0,sizeof(TradingCodeField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryTradingCode(int requestID,bool bisLast,const CThostFtdcTradingCodeField& tradingCodeField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryTradingCode,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        TradingCodeField(tradingCodeField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcTradingCodeField   TradingCodeField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�����ѯ��Լ��֤������Ӧ
struct DataRspQryInstrumentMarginRate
{
    DataRspQryInstrumentMarginRate()
    :   Head(BID_RspQryInstrumentMarginRate,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InstrumentMarginRateField,0,sizeof(InstrumentMarginRateField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryInstrumentMarginRate(int requestID,bool bisLast,const PlatformStru_InstrumentMarginRate& instrumentMarginRateField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryInstrumentMarginRate,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InstrumentMarginRateField(instrumentMarginRateField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_InstrumentMarginRate   InstrumentMarginRateField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�����ѯ��Լ����������Ӧ
struct DataRspQryInstrumentCommissionRate
{
    DataRspQryInstrumentCommissionRate()
    :   Head(BID_RspQryInstrumentCommissionRate,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InstrumentCommissionRateField,0,sizeof(InstrumentCommissionRateField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryInstrumentCommissionRate(int requestID,bool bisLast,const PlatformStru_InstrumentCommissionRate& instrumentCommissionRateField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryInstrumentCommissionRate,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InstrumentCommissionRateField(instrumentCommissionRateField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_InstrumentCommissionRate   InstrumentCommissionRateField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�����ѯ��Ȩ��Լ����������Ӧ
struct DataRspQryOptionInstrCommRate
{
    DataRspQryOptionInstrCommRate()
    :   Head(BID_RspQryOptionInstrCommRate,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryOptionInstrCommRate(int requestID,bool bisLast,const PlatformStru_InstrumentCommissionRate& instrumentCommissionRateField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryOptionInstrCommRate,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InstrumentCommissionRateField(instrumentCommissionRateField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_InstrumentCommissionRate   InstrumentCommissionRateField;
    CThostFtdcRspInfoField    RspInfoField;    
};
//�����ѯ��Ȩ���׳ɱ���Ӧ
struct DataRspQryOptionInstrTradeCost
{
    DataRspQryOptionInstrTradeCost()
    :   Head(BID_RspQryOptionInstrTradeCost,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryOptionInstrTradeCost(int requestID,bool bisLast,const PlatformStru_InstrumentMarginRate& instrumentMarginRateField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryOptionInstrTradeCost,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InstrumentMarginRateField(instrumentMarginRateField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_InstrumentMarginRate   InstrumentMarginRateField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�����ѯ��������Ӧ
struct DataRspQryExchange
{
    DataRspQryExchange(int requestID,bool bisLast,const CThostFtdcExchangeField& exchangeField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryExchange,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ExchangeField(exchangeField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcExchangeField   ExchangeField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�����ѯ���͹�˾���ײ�����Ӧ
struct DataRspQryBrokerTradingParams
{
    DataRspQryBrokerTradingParams(int requestID,bool bisLast,const PlatformStru_BrokerTradingParams& brokerTradingParams,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryBrokerTradingParams,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        BrokerTradingParams(brokerTradingParams),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData                Head;
    int                                 nRequestID;
    bool                                bIsLast;
    PlatformStru_BrokerTradingParams    BrokerTradingParams;
    CThostFtdcRspInfoField              RspInfoField;    
};

//�����ѯ��Ʒ��Ӧ
struct DataRspQryProduct
{
    DataRspQryProduct(int requestID,bool bisLast,const PlatformStru_ProductInfo& productField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryProduct,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ProductField(productField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    int                         nRequestID;
    bool                        bIsLast;
    PlatformStru_ProductInfo    ProductField;
    CThostFtdcRspInfoField      RspInfoField;    
};

//�����ѯ��Լ��Ӧ
struct DataRspQryInstrument
{
    DataRspQryInstrument(int requestID,bool bisLast,const PlatformStru_InstrumentInfo& instrumentField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryInstrument,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InstrumentField(instrumentField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_InstrumentInfo InstrumentField;
    CThostFtdcRspInfoField     RspInfoField;    
};
//�����ѯ������Ӧ
struct DataRspQryDepthMarketData
{
    DataRspQryDepthMarketData()
    :   Head(BID_RspQryDepthMarketData,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&DepthMarketDataField,0,sizeof(DepthMarketDataField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryDepthMarketData(int requestID,bool bisLast,const PlatformStru_DepthMarketData& depthMarketDataField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryDepthMarketData,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        DepthMarketDataField(depthMarketDataField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_DepthMarketData DepthMarketDataField;
    CThostFtdcRspInfoField     RspInfoField;    
};


//�����ѯ������Ӧ
struct DataRspQryOrder
{
    DataRspQryOrder(int requestID,bool bisLast,const PlatformStru_OrderInfo& orderField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryOrder,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        OrderField(orderField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_OrderInfo    OrderField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�����ѯ�ɽ���Ӧ
struct DataRspQryTrade
{
    DataRspQryTrade(int requestID,bool bisLast,const PlatformStru_TradeInfo& tradeField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryTrade,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        TradeField(tradeField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_TradeInfo    TradeField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�����ѯͶ���߽�������Ӧ
struct DataRspQrySettlementInfo
{
    DataRspQrySettlementInfo()
    :   Head(BID_RspQrySettlementInfo,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&SettlementInfoField,0,sizeof(SettlementInfoField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQrySettlementInfo(int requestID,bool bisLast,const CThostFtdcSettlementInfoField& settlementInfoField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQrySettlementInfo,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        SettlementInfoField(settlementInfoField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcSettlementInfoField      SettlementInfoField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�ڻ����������ʽ�ת�ڻ�Ӧ��
struct DataRspFromBankToFutureByFuture
{
    DataRspFromBankToFutureByFuture()
    :   Head(BID_RspFromBankToFutureByFuture,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&TransferField,0,sizeof(TransferField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspFromBankToFutureByFuture(int requestID,bool bisLast,const CThostFtdcReqTransferField& transferField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspFromBankToFutureByFuture,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        TransferField(transferField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcReqTransferField      TransferField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�ڻ������ڻ��ʽ�ת����Ӧ��
struct DataRspFromFutureToBankByFuture
{
    DataRspFromFutureToBankByFuture()
    :   Head(BID_RspFromFutureToBankByFuture,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&TransferField,0,sizeof(TransferField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspFromFutureToBankByFuture(int requestID,bool bisLast,const CThostFtdcReqTransferField& transferField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspFromFutureToBankByFuture,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        TransferField(transferField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcReqTransferField      TransferField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�ڻ������ѯ�������Ӧ��
struct DataRspQueryBankAccountMoneyByFuture
{
    DataRspQueryBankAccountMoneyByFuture()
    :   Head(BID_RspQueryBankAccountMoneyByFuture,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&QueryAccountField,0,sizeof(QueryAccountField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQueryBankAccountMoneyByFuture(int requestID,bool bisLast,const CThostFtdcReqQueryAccountField& queryAccountField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQueryBankAccountMoneyByFuture,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        QueryAccountField(queryAccountField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcReqQueryAccountField      QueryAccountField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�����ѯת����ˮ��Ӧ
struct DataRspQryTransferSerial
{
    DataRspQryTransferSerial()
    :   Head(BID_RspQryTransferSerial,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&TransferSerialField,0,sizeof(TransferSerialField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryTransferSerial(int requestID,bool bisLast,const CThostFtdcTransferSerialField& transferSerialField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryTransferSerial,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        TransferSerialField(transferSerialField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcTransferSerialField      TransferSerialField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�����ѯ����ǩԼ��ϵ��Ӧ
struct DataRspQryAccountregister
{
    DataRspQryAccountregister(int requestID,bool bisLast,const CThostFtdcAccountregisterField& accountregister,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryAccountregister,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        Accountregister(accountregister),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcAccountregisterField      Accountregister;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�ڻ����������ʽ�ת�ڻ��ر�
struct DataRtnFromBankToFutureByFuture
{
    DataRtnFromBankToFutureByFuture()
    :   Head(BID_RtnFromBankToFutureByFuture,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&RspTransferField,0,sizeof(RspTransferField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRtnFromBankToFutureByFuture(int requestID,bool bisLast,const CThostFtdcRspTransferField& rspTransferField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RtnFromBankToFutureByFuture,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        RspTransferField(rspTransferField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcRspTransferField      RspTransferField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�ڻ������ڻ��ʽ�ת���лر�
struct DataRtnFromFutureToBankByFuture
{
    DataRtnFromFutureToBankByFuture()
    :   Head(BID_RtnFromFutureToBankByFuture,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&RspTransferField,0,sizeof(RspTransferField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRtnFromFutureToBankByFuture(int requestID,bool bisLast,const CThostFtdcRspTransferField& rspTransferField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RtnFromFutureToBankByFuture,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        RspTransferField(rspTransferField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcRspTransferField      RspTransferField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�ڻ������ѯ�������ر�
struct DataRtnQueryBankAccountMoneyByFuture
{
    DataRtnQueryBankAccountMoneyByFuture()
    :   Head(BID_RtnQueryBankAccountMoneyByFuture,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&NotifyQueryAccountField,0,sizeof(NotifyQueryAccountField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRtnQueryBankAccountMoneyByFuture(int requestID,bool bisLast,const CThostFtdcNotifyQueryAccountField& notifyQueryAccountField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RtnQueryBankAccountMoneyByFuture,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        NotifyQueryAccountField(notifyQueryAccountField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcNotifyQueryAccountField      NotifyQueryAccountField;
    CThostFtdcRspInfoField    RspInfoField;    
};
//�����ѯǩԼ������Ӧ
struct DataRspQryContractBank
{
    DataRspQryContractBank(int requestID,bool bisLast,const CThostFtdcContractBankField& contractBankField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryContractBank,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ContractBankField(contractBankField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcContractBankField      ContractBankField;
    CThostFtdcRspInfoField    RspInfoField;    
};
//�����ѯ������Ӧ
struct DataRspQryExchangeRate
{
    DataRspQryExchangeRate(int requestID,bool bisLast,const CThostFtdcExchangeRateField& exchangeRateField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryExchangeRate,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ExchangeRateField(exchangeRateField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    int                         nRequestID;
    bool                        bIsLast;
    CThostFtdcExchangeRateField ExchangeRateField;
    CThostFtdcRspInfoField      RspInfoField;    
};

//ѯ��¼��������Ӧ
struct DataRspForQuoteInsert
{
	DataRspForQuoteInsert(void) {
		memset(this, 0, sizeof(*this));
	};
    DataRspForQuoteInsert(int requestID,bool bisLast,const PlatformStru_InputForQuoteField& inputForQuoteField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspForQuoteInsert,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InputForQuoteField(inputForQuoteField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    int                         nRequestID;
    bool                        bIsLast;
    PlatformStru_InputForQuoteField InputForQuoteField;
    CThostFtdcRspInfoField      RspInfoField;    
};
//ִ������¼��������Ӧ
struct DataRspExecOrderInsert
{
	DataRspExecOrderInsert(void)
	:   Head(BID_RspExecOrderInsert,(unsigned int)sizeof(*this)) {
		nRequestID = 0;
		bIsLast = false;
		memset(&InputExecOrderField, 0, sizeof(InputExecOrderField));
		memset(&RspInfoField, 0, sizeof(RspInfoField));
	};
    DataRspExecOrderInsert(int requestID,bool bisLast,const PlatformStru_InputExecOrderField& inputExecOrderField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspExecOrderInsert,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InputExecOrderField(inputExecOrderField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    int                         nRequestID;
    bool                        bIsLast;
    PlatformStru_InputExecOrderField InputExecOrderField;
    CThostFtdcRspInfoField      RspInfoField;    
};
//ִ���������������Ӧ
struct DataRspExecOrderAction
{
	DataRspExecOrderAction(void) 
    :   Head(BID_RspExecOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(false)
	{
		memset(&RspInfoField, 0, sizeof(RspInfoField));
	};
    DataRspExecOrderAction(int requestID,bool bisLast,const PlatformStru_InputExecOrderActionField& inputExecOrderActionField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspExecOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InputExecOrderActionField(inputExecOrderActionField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    int                         nRequestID;
    bool                        bIsLast;
    PlatformStru_InputExecOrderActionField InputExecOrderActionField;
    CThostFtdcRspInfoField      RspInfoField;    
};
//�����ѯִ��������Ӧ
struct DataRspQryExecOrder
{
	DataRspQryExecOrder(void) 
	:   Head(BID_RspQryExecOrder,(unsigned int)sizeof(*this))
	{
        nRequestID = 0;
        bIsLast = false;
        memset(&RspInfoField, 0, sizeof(RspInfoField));
	};
    DataRspQryExecOrder(int requestID,bool bisLast,const PlatformStru_ExecOrderField& execOrderField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryExecOrder,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ExecOrderField(execOrderField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    int                         nRequestID;
    bool                        bIsLast;
    PlatformStru_ExecOrderField ExecOrderField;
    CThostFtdcRspInfoField      RspInfoField;    
};
//ѯ��¼�����ر�
struct DataErrRtnForQuoteInsert
{
	DataErrRtnForQuoteInsert(void) {
		memset(this, 0, sizeof(*this));
	};
    DataErrRtnForQuoteInsert(const PlatformStru_InputForQuoteField& inputForQuoteField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_ErrRtnForQuoteInsert,(unsigned int)sizeof(*this)),
        InputForQuoteField(inputForQuoteField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    PlatformStru_InputForQuoteField InputForQuoteField;
    CThostFtdcRspInfoField      RspInfoField;    
};
//ִ������¼�����ر�
struct DataErrRtnExecOrderInsert
{
	DataErrRtnExecOrderInsert(void) {
		memset(this, 0, sizeof(*this));
	};
	DataErrRtnExecOrderInsert(const PlatformStru_InputExecOrderField& inputExecOrderField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_ErrRtnExecOrderInsert,(unsigned int)sizeof(*this)),
        InputExecOrderField(inputExecOrderField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    PlatformStru_InputExecOrderField InputExecOrderField;
    CThostFtdcRspInfoField      RspInfoField;    
};
//ִ�������������ر�
struct DataErrRtnExecOrderAction
{
	DataErrRtnExecOrderAction(void)
    :   Head(BID_ErrRtnExecOrderAction,(unsigned int)sizeof(*this))
	{
		memset(&RspInfoField, 0, sizeof(RspInfoField));
	};
    DataErrRtnExecOrderAction(const PlatformStru_ExecOrderActionField& execOrderActionField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_ErrRtnExecOrderAction,(unsigned int)sizeof(*this)),
        ExecOrderActionField(execOrderActionField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    PlatformStru_ExecOrderActionField ExecOrderActionField;
    CThostFtdcRspInfoField      RspInfoField;    
};
//ִ������֪ͨ
struct DataRtnExecOrder
{
    DataRtnExecOrder()
    :   Head(BID_RtnExecOrder,(unsigned int)sizeof(*this))
    {
    };
    DataRtnExecOrder(const PlatformStru_ExecOrderField& execOrderField)
    :   Head(BID_RtnExecOrder,(unsigned int)sizeof(*this)),
        ExecOrderField(execOrderField)
    {
    };

    AbstractBusinessData        Head;
    PlatformStru_ExecOrderField ExecOrderField;
};
//����ѯ��Ӧ��
struct DataRspSubForQuoteRsp
{
	DataRspSubForQuoteRsp(void) 
	:	Head(BID_RspSubForQuoteRsp,(unsigned int)sizeof(*this)) {
		memset(&RspInfoField, 0, sizeof(RspInfoField));
	};
    DataRspSubForQuoteRsp(const char* instrumentID,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspSubForQuoteRsp,(unsigned int)sizeof(*this)),
        RspInfoField(RspInfo)
    {
        memset(InstrumentID,0,sizeof(InstrumentID));
        strncpy(InstrumentID,instrumentID,sizeof(InstrumentID)-1);
    };

    AbstractBusinessData        Head;
    CommonInstrumentIDType      InstrumentID;
    CThostFtdcRspInfoField      RspInfoField;    
};
//�˶�ѯ��Ӧ��
struct DataRspUnSubForQuoteRsp
{
	DataRspUnSubForQuoteRsp(void) 
	:   Head(BID_RspUnSubForQuoteRsp,(unsigned int)sizeof(*this)) {
		memset(&RspInfoField, 0, sizeof(RspInfoField));
	};
    DataRspUnSubForQuoteRsp(const char* instrumentID,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspUnSubForQuoteRsp,(unsigned int)sizeof(*this)),
        RspInfoField(RspInfo)
    {
        strncpy(InstrumentID,instrumentID,sizeof(InstrumentID)-1);
    };

    AbstractBusinessData        Head;
    CommonInstrumentIDType      InstrumentID;
    CThostFtdcRspInfoField      RspInfoField;    
};
//ѯ��֪ͨ
struct DataRtnForQuoteRsp
{
    DataRtnForQuoteRsp()
    :   Head(BID_RtnForQuoteRsp,(unsigned int)sizeof(*this))
    {
    };
    DataRtnForQuoteRsp(const PlatformStru_ForQuoteRspField& forQuoteRsp)
    :   Head(BID_RtnForQuoteRsp,(unsigned int)sizeof(*this)),
        ForQuoteRsp(forQuoteRsp)
    {
    };

    AbstractBusinessData        Head;
    PlatformStru_ForQuoteRspField ForQuoteRsp;
};


///��Լ����״̬
struct DataInstrumentStatus
{
    DataInstrumentStatus()
    :   Head(BID_RtnInstrumentStatus,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InstrumentStatusField,0,sizeof(InstrumentStatusField));
    };
    DataInstrumentStatus(int requestID,bool bisLast,const CThostFtdcInstrumentStatusField& instrumentStatusField)
    :   Head(BID_RtnInstrumentStatus,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InstrumentStatusField(instrumentStatusField)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcInstrumentStatusField      InstrumentStatusField; 
};
//�뽻�׺�̨ͨ�����ӳɹ�
struct DataFrontConnected
{
    DataFrontConnected(bool btrader)
    :   Head(BID_FrontConnected,(unsigned int)sizeof(*this)),
        bTrader(btrader)
    {
    };

    AbstractBusinessData      Head;
	bool                      bTrader;
};
//�뽻�׺�̨ͨ�����ӶϿ�
struct DataFrontDisconnected
{
    DataFrontDisconnected(bool btrader,int nreason)
    :   Head(BID_FrontDisconnected,(unsigned int)sizeof(*this)),
        bTrader(btrader),
        nReason(nreason)
    {
    };

    AbstractBusinessData      Head;
	bool                      bTrader;
    int                       nReason; 
};
struct DataRspQrySettlementInfoConfirm
{
	DataRspQrySettlementInfoConfirm(int requestID,bool bisLast,const CThostFtdcSettlementInfoConfirmField& settlementInfoConfirm,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQrySettlementInfoConfirm,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        SettlementInfoConfirm(settlementInfoConfirm),
        RspInfoField(RspInfo)
	{
	};

    AbstractBusinessData      Head;
	int                       nRequestID;
	bool                      bIsLast;
	CThostFtdcSettlementInfoConfirmField      SettlementInfoConfirm;
	CThostFtdcRspInfoField    RspInfoField;    
};
struct DataTradingNoticeInfoField
{
    DataTradingNoticeInfoField(const CThostFtdcTradingNoticeInfoField& tradingNoticeInfoField)
    :   Head(BID_RtnTradingNotice,(unsigned int)sizeof(*this)),
        TradingNoticeInfoField(tradingNoticeInfoField)
    {
    };

    AbstractBusinessData      Head;
    CThostFtdcTradingNoticeInfoField      TradingNoticeInfoField; 
};

struct DataRspQryTradingNotice
{
    DataRspQryTradingNotice()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryTradingNotice;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    CThostFtdcTradingNoticeField      TradingNoticeInfoField; 
};
struct DataConnectivityStatus
{
    DataConnectivityStatus()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_ConnectivityStatus;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    ConnectivityStatus      StatusField; 
};
struct DataMessageStruct
{
    DataMessageStruct()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_AlertMessage;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    PlatformStru_MessageStruct  MessageStructField; 
};
struct DataExchangeRate
{
    DataExchangeRate()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_ExchangeRate;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    char			RateField[128]; 
};
///��ѯ�û�֪ͨ��Ӧ
struct DataRspQryNotice
{
    DataRspQryNotice()
    :   Head(BID_RspQryNotice,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&NoticeField,0,sizeof(NoticeField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryNotice(int requestID,bool bisLast,const CThostFtdcNoticeField& noticeField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryNotice,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        NoticeField(noticeField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcNoticeField     NoticeField; 
    CThostFtdcRspInfoField    RspInfoField;   
};
// �ɽ�ͳ�ƽṹ
typedef struct
{
	std::string Instrument;
	std::string EcChangeID;
	double TradeCommission;
	std::string BuySell;
	std::string OpenClose;
	double price;
	int volume;
} TotalInfo;
typedef enum  _tagSubscribeMarketDataOper
{
	eSubscribeMarketData,
	eUnsubscribeMarketData,
}SubscribeMarketDataOper;

//��ʾ���ĸ�ģ�鶩�ĵ�����
typedef enum  _tagSPanel
{
	S_STATUS_TEMP=(1<<2),
	S_STATUS_QUOT=(1<<3),
	S_STATUS_ORDER=(1<<4),
	S_STATUS_ORDERINSERT=(1<<5),
	S_STATUS_POS=(1<<6),
	S_STATUS_PLUS=(1<<7),
    S_STATUS_PlatformSvr=(1<<8),
    S_STATUS_MovePosition=(1<<9)
}S_Panel;


#ifdef WIN32
#pragma managed(pop)
#endif 





#endif

