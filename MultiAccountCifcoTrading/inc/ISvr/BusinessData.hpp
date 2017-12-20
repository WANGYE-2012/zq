#ifndef _BUSINESSDATA_HPP_
#define _BUSINESSDATA_HPP_

#include "..\\ctp\\ThostFtdcUserApiStruct.h" //����CTP�����ݽṹ
#include "../inc/Module-Misc/globalDefines.h"
#include "../inc/Module-Misc/tools_util.h"
#include "PlatformStruct.h"
#include "../inc/Module-Misc/tools_tools.h"
#include <string>


//��ִ��һЩ��ѯ����ʱ�����ܲ�ѯ���Ϊ�գ�
//���ۺϽ���ƽ̨��API��Ȼ��Ϊ�˲�ѯ�ɹ���
//���ǽ��������"����"Ϊ��ѯʧ�ܣ����Ը�GUI
//��ģ���账����������ErrorID
#define CustomErrorID 123456789
#define CustomErrorMsg "��ѯ�޼�¼"
#define THOST_FTDC_OST_ERROE  'd'
#define ORDER_INSERT_FRONTID  -1
#define ORDER_INSERT_SESSIONID -1
#define ORDER_INSERT_ORDERSTATUS 'N'

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
    BID_RspOrderInsert,             //����¼���ִ(�����ĵ���һ���Ǵ����ִ)
    BID_RspOrderAction1,            //����������ִ(�����ִ1���ۺϽ���ƽ̨��Ϊ������Ч)
    BID_RspOrderAction2,            //����������ִ(�����ִ2����������Ϊ������Ч)
    BID_RtnOrder,                   //�����ر�(����¼���뱨��������������ûر�)
    BID_RtnTrade,                   //�ɽ��ر�
    BID_RtnDepthMarketData,         //�������
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
    BID_RspQryExchange,             //�����ѯ��������Ӧ
    BID_RspQryInstrument,           //�����ѯ��Լ��Ӧ
    BID_RspQryDepthMarketData,      //�����ѯ������Ӧ
    BID_RspQrySettlementInfo,       //�����ѯͶ���߽�������Ӧ
    BID_RspQryTransferBank,         //�����ѯת��������Ӧ
    BID_RspQryInvestorPositionDetail,//�����ѯͶ���ֲ߳���ϸ��Ӧ
    BID_RspQryNotice,               //�����ѯ�ͻ�֪ͨ��Ӧ
    BID_RspQrySettlementInfoConfirm,//�����ѯ������Ϣȷ����Ӧ
    BID_RspQryInvestorPositionComb, //�����ѯͶ���ֲ߳���ϸ��Ӧ
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

    //ADD NEW
    BID_FrontDisconnected,                  //�뽻�׺�̨ͨ�����ӶϿ�
    BID_FrontConnected,					//�뽻�׺�̨ͨ�����ӳɹ�


    BID_QryStart,                           //��ѯ��ʼ������Ϊstring����ʾ��ѯ����
    BID_ConnectivityStatus,
    BID_AlertMessage,			//Pats Alert Message
    BID_ExchangeRate,		
    BID_MAX
};

///��ʾһ��������¼
struct OrderKey
{
    OrderKey()
    {
        memset(this,0,sizeof(*this));
        FrontID=-1;
        SessionID=-1;
    };

    OrderKey(const char* account,const char* instrumentID,int fid, int sid, const char* orderRef)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),account);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        FrontID=fid;
        SessionID=sid;
        CTools::mymemcpy_trim(OrderRef,sizeof(OrderRef),orderRef);
    };

    OrderKey(const PlatformStru_OrderInfo& Order)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),Order.szAccount);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Order.InstrumentID);
        FrontID=Order.FrontID;
        SessionID=Order.SessionID;
        CTools::mymemcpy_trim(OrderRef,sizeof(OrderRef),Order.OrderRef);
    };

    OrderKey& operator= (const OrderKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));
        return *this;
    };

    bool operator< (const OrderKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(FrontID<r.FrontID)
            return true;
        if(FrontID>r.FrontID)
            return false;

        if(SessionID<r.SessionID)
            return true;
        if(SessionID>r.SessionID)
            return false;

        irlt=strncmp(OrderRef,r.OrderRef,sizeof(OrderRef));
        if(irlt<0) return true;
        if(irlt>0) return false;

        return false;
    }

    bool operator== (const OrderKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        if(FrontID!=r.FrontID) return false;

        if(SessionID!=r.SessionID) return false;

        irlt=strncmp(OrderRef,r.OrderRef,sizeof(OrderRef));
        if(irlt!=0) return false;

        return true;
    }

    bool operator> (const OrderKey& r) const
    {
        return !((*this) < r || (*this) == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=0,ilen;
        prlt[len]=0;

        ilen=strlen(Account);
        if(len+ilen+1<rltsize)        { strcat(prlt,Account);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(InstrumentID);
        if(len+ilen+1<rltsize)   { strcat(prlt,InstrumentID);  strcat(prlt,",");  len+=ilen+1; }

        char buf[256];
        sprintf(buf,"Front%d,SessionD%d",FrontID,SessionID);
        ilen=strlen(buf);
        if(len+ilen<rltsize)             { strcat(prlt,buf);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(OrderRef);
        if(len+ilen+1<rltsize)       { strcat(prlt,OrderRef);  strcat(prlt,",");  len+=ilen+1; }
        return prlt;
    }
    
    CommonAccountType       Account;
    CommonInstrumentIDType  InstrumentID;       //��ԼID
    int                     FrontID;            //ǰ�û�����࣬ͨ����ID��ǲ�ͬ��ǰ�û�
    int                     SessionID;          //ֻҪ���µ��룬�ͻ�仯��
    CommonOrderRefType      OrderRef;           //(����Ϊ12���ַ����Ҷ��룬��߲��ո�)������GUI�е�ID,
                                                //ֻҪFrontID��SessionID�����Ψһ��
                                                //OrderRef�����ظ�ʹ��    
};

///��ʾһ���ɽ���¼
struct TradeKey
{
//    TradeKey()
//    {
//    };

    TradeKey(const char* account, 
                const char* instrumentID, 
                const char* tradeID, 
                const char* tradeDate, 
                const char* ordersysID, 
                const char direction)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),account);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        CTools::mymemcpy_trim(TradeDate,sizeof(TradeDate),tradeDate);
        CTools::mymemcpy_trim(TradeID,sizeof(TradeID),tradeID);
        CTools::mymemcpy_trim(OrderSysID,sizeof(OrderSysID),ordersysID);
        Direction=direction;
    };

    TradeKey(const PlatformStru_TradeInfo& Trade)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),Trade.szAccount);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Trade.InstrumentID);
        CTools::mymemcpy_trim(TradeDate,sizeof(TradeDate),Trade.TradeDate);
        CTools::mymemcpy_trim(TradeID,sizeof(TradeID),Trade.TradeID);
        CTools::mymemcpy_trim(OrderSysID,sizeof(OrderSysID),Trade.OrderSysID);
        Direction=Trade.Direction;
    };

    TradeKey& operator= (const TradeKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));

        return *this;
    };

    //ע�⣬ͨ��TradeDate��TradeID�ıȽ�˳�򣬱�֤set<TradeKey>����ͬ�����ļ�¼����ʱ������
    bool operator< (const TradeKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(TradeDate,r.TradeDate,sizeof(TradeDate));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(TradeID,r.TradeID,sizeof(TradeID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(OrderSysID,r.OrderSysID,sizeof(OrderSysID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(Direction<r.Direction) return true;
        if(Direction>r.Direction) return false;
        
        return false;
    }

    bool operator== (const TradeKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        irlt=strncmp(TradeDate,r.TradeDate,sizeof(TradeDate));
        if(irlt!=0) return false;

        irlt=strncmp(TradeID,r.TradeID,sizeof(TradeID));
        if(irlt!=0) return false;

        irlt=strncmp(OrderSysID,r.OrderSysID,sizeof(OrderSysID));
        if(irlt!=0) return false;

        if(Direction!=r.Direction)
            return false;

        return true;
    }
    
    bool operator> (const TradeKey& r) const
    {
        return !(*this < r ||*this == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=0,ilen;

        prlt[len]=0;

        ilen=strlen(Account);
        if(len+ilen+1<rltsize)   { strcat(prlt,Account);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(InstrumentID);
        if(len+ilen+1<rltsize)   { strcat(prlt,InstrumentID);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(TradeDate);
        if(len+ilen+1<rltsize)   { strcat(prlt,TradeDate);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(TradeID);
        if(len+ilen+1<rltsize)   { strcat(prlt,TradeID);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(OrderSysID);
        if(len+ilen+1<rltsize)   { strcat(prlt,OrderSysID);  strcat(prlt,",");  len+=ilen+1; }

        char buf[256];
        sprintf(buf,"Direct%d",Direction);
        ilen=strlen(buf);
        if(len+ilen<rltsize)      { strcat(prlt,buf);  len+=ilen; }
        return prlt;
    }

    CommonAccountType       Account;
    CommonInstrumentIDType  InstrumentID;      //��ԼID
    CommonDateType          TradeDate;         //�ɽ�����(����Ϊ8���ַ�����:20101023)
    CommonTradeIDType       TradeID;           //�ɽ�ID(����Ϊ20���ַ����Ҷ��룬��߲��ո�)
    CommonOrderSysIDType    OrderSysID;        //����ID(����Ϊ20���ַ����Ҷ��룬��߲��ո�)
	char		            Direction;         //����ģ�����Գɽ���������Ϸ���
};

///��ʾһ���ɽ�ͳ�Ƽ�¼
struct TradeTotalKey
{
//    TradeTotalKey()
//    {
//    };

    TradeTotalKey(const char* account,
                    const char* exchangeID, 
                    const char* instrumentID, 
                    const char direction, 
                    const char offsetFlag)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),account);
        CTools::mymemcpy_trim(ExchangeID,sizeof(ExchangeID),exchangeID);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        Direction=direction;
        OffsetFlag=offsetFlag;
    };

    TradeTotalKey(const PlatformStru_TradeTotalInfo& TradeTotal)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),TradeTotal.szAccount);
        CTools::mymemcpy_trim(ExchangeID,sizeof(ExchangeID),TradeTotal.ExchangeID);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),TradeTotal.InstrumentID);
        Direction=TradeTotal.Direction;
        OffsetFlag=TradeTotal.OffsetFlag;
    };

	TradeTotalKey(const PlatformStru_TradeInfo& TradeInfo)
	{
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),TradeInfo.szAccount);
        CTools::mymemcpy_trim(ExchangeID,sizeof(ExchangeID),TradeInfo.ExchangeID);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),TradeInfo.InstrumentID);
        Direction=TradeInfo.Direction;
        OffsetFlag=TradeInfo.OffsetFlag;
    }


    TradeTotalKey& operator= (const TradeTotalKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));

        return *this;
    };

    bool operator< (const TradeTotalKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(ExchangeID,r.ExchangeID,sizeof(ExchangeID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(Direction<r.Direction)
            return true;
        if(Direction>r.Direction)
            return false;

        if(OffsetFlag<r.OffsetFlag)
            return true;
        if(OffsetFlag>r.OffsetFlag)
            return false;

        return false;
    }

    bool operator== (const TradeTotalKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(ExchangeID,r.ExchangeID,sizeof(ExchangeID));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        if(Direction!=r.Direction)
            return false;

        if(OffsetFlag!=r.OffsetFlag)
            return false;

        return true;
    }
    
    bool operator> (const TradeTotalKey& r) const
    {
        return !(*this < r ||*this == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=0,ilen;
        prlt[len]=0;

        ilen=strlen(Account);
        if(len+ilen+1<rltsize)   { strcat(prlt,Account);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(ExchangeID);
        if(len+ilen+1<rltsize)   { strcat(prlt,ExchangeID);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(InstrumentID);
        if(len+ilen+1<rltsize)   { strcat(prlt,InstrumentID);  strcat(prlt,",");  len+=ilen+1; }

        char buf[256];
        sprintf(buf,"Direct%d,Offset%d",Direction,OffsetFlag);
        ilen=strlen(buf);
        if(len+ilen<rltsize)             { strcat(prlt,buf);  len+=ilen; }
        return prlt;
    }

    CommonAccountType           Account;
    CommonExchangeIDType        ExchangeID;
    CommonInstrumentIDType      InstrumentID;      //��ԼID
	char			            Direction;
	char			            OffsetFlag;
};


///��ʾһ���ֲ���ϸ��¼����InstrumentID��OpenDate��TradeID��Direction��hedgeFlag��TradeType����Ψһ�ĳֲ���ϸ��¼
struct PositionDetailKey
{
	PositionDetailKey(const char* account, 
                        const char* instrumentID, 
                        const char* openDate, 
                        const char* tradeID, 
                        const char& direction, 
                        const char& hedgeFlag, 
                        const char& tradeType)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),account);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        CTools::mymemcpy_trim(OpenDate,sizeof(OpenDate),openDate);
        CTools::mymemcpy_trim(TradeID,sizeof(TradeID),tradeID);

        Direction=direction;
        HedgeFlag=hedgeFlag;
        TradeType=tradeType;
    };

    PositionDetailKey(const PlatformStru_PositionDetail& PositionDetail)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),PositionDetail.szAccount);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),PositionDetail.InstrumentID);
        CTools::mymemcpy_trim(OpenDate,sizeof(OpenDate),PositionDetail.OpenDate);
        CTools::mymemcpy_trim(TradeID,sizeof(TradeID),PositionDetail.TradeID);

        Direction=PositionDetail.Direction;
        HedgeFlag=PositionDetail.HedgeFlag;
        TradeType=PositionDetail.TradeType;
    };

    PositionDetailKey& operator= (const PositionDetailKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));

        return *this;
    };


    //ע�⣬ͨ��OpenDate��TradeID�ıȽ�˳�򣬱�֤set<PositionDetailKey>����ͬ�����ĳֲ���ϸ����ʱ������
    bool operator< (const PositionDetailKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(OpenDate,r.OpenDate,sizeof(OpenDate));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=CTools::mycompare2_trynumeric(TradeID,r.TradeID);
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(Direction<r.Direction)
			return true;
		if(Direction>r.Direction)
			return false;

        if(HedgeFlag<r.HedgeFlag)
			return true;
		if(HedgeFlag>r.HedgeFlag) 
			return false;

        if(TradeType<r.TradeType)
			return true;
		if(TradeType>r.TradeType) 
			return false;


        return false;
    }

    bool operator== (const PositionDetailKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        irlt=strncmp(OpenDate,r.OpenDate,sizeof(OpenDate));
        if(irlt!=0) return false;

        irlt=strncmp(TradeID,r.TradeID,sizeof(TradeID));
        if(irlt!=0) return false;

        if(Direction!=r.Direction)
            return false;
        if(HedgeFlag!=r.HedgeFlag)
            return false;
        if(TradeType!=r.TradeType)
            return false;

        return true;
    }
    
    bool operator!= (const PositionDetailKey& r) const
    {
        return !(*this == r);
    }
    
    bool operator> (const PositionDetailKey& r) const
    {
        return !(*this < r ||*this == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=0,ilen;
        prlt[len]=0;

        ilen=strlen(Account);
        if(len+ilen+1<rltsize)   { strcat(prlt,Account);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(InstrumentID);
        if(len+ilen+1<rltsize)   { strcat(prlt,InstrumentID);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(OpenDate);
        if(len+ilen+1<rltsize)   { strcat(prlt,OpenDate);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(TradeID);
        if(len+ilen+1<rltsize)   { strcat(prlt,TradeID);  strcat(prlt,",");  len+=ilen+1; }

        char buf[256];
        sprintf(buf,"Direct%d,Hedge%d,TradeType%d",Direction,HedgeFlag,TradeType);
        ilen=strlen(buf);
        if(len+ilen<rltsize)     { strcat(prlt,buf);  len+=ilen; }
        return prlt;
    }


	CommonAccountType       Account;		    //�˺�
    CommonInstrumentIDType  InstrumentID;       //��ԼID
    CommonDateType          OpenDate;           //��������
    CommonTradeIDType       TradeID;            //�ɽ�ID(����Ϊ20���ַ����Ҷ��룬��߲��ո�)
    char                    Direction;          //��������
    char                    HedgeFlag;          //Ͷ���ױ���־
    char                    TradeType;          //�ɽ����ͣ�'0'/0x0: ��ͨ�ɽ��� '4': ��������ɽ�
};


///��ʾһ���ֲּ�¼����InstrumentID��Direction��HedgeFlag����Ψһ�ĳֲ���ϸ��¼
struct PositionKey
{
//    PositionKey()
//    {
//    };

	PositionKey(const char* account, const char* instrumentID, const char& posiDirection, const char& hedgeFlag)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),account);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        PosiDirection=posiDirection;
        HedgeFlag=hedgeFlag;
    };

    PositionKey(const PlatformStru_Position& Position)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),Position.szAccount);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Position.InstrumentID);
        PosiDirection=Position.PosiDirection;
        HedgeFlag=Position.HedgeFlag;
    };

	PositionKey(const PlatformStru_OrderInfo& Order)
	{
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),Order.szAccount);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Order.InstrumentID);
		if(Order.CombOffsetFlag[0]==THOST_FTDC_OF_Open)
			PosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
		else if(Order.CombOffsetFlag[0]==THOST_FTDC_OF_Close||
				Order.CombOffsetFlag[0]==THOST_FTDC_OF_ForceClose||
				Order.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday||
				Order.CombOffsetFlag[0]==THOST_FTDC_OF_CloseYesterday||
				Order.CombOffsetFlag[0]==THOST_FTDC_OF_LocalForceClose)
			PosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Short : THOST_FTDC_PD_Long;
		else 
			PosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
		HedgeFlag=Order.CombHedgeFlag[0];
	};

    PositionKey& operator= (const PositionKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));
        return *this;
    };


    bool operator< (const PositionKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(PosiDirection<r.PosiDirection)
			return true;
		if(PosiDirection>r.PosiDirection)
			return false;

        if(HedgeFlag<r.HedgeFlag)
			return true;
		if(HedgeFlag>r.HedgeFlag) 
			return false;

        return false;
    }

    bool operator== (const PositionKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        if(PosiDirection!=r.PosiDirection)
            return false;

        if(HedgeFlag!=r.HedgeFlag)
            return false;

        return true;
    }
    
    bool operator!= (const PositionKey& r) const
    {
        return !(*this == r);
    }
    
    bool operator> (const PositionKey& r) const
    {
        return !(*this < r ||*this == r);
    }
    bool IsSamePositionKey(const PlatformStru_PositionDetail& PositionDetail) const
    {
        int off,len;

        CTools::mygettrimpos(PositionDetail.szAccount,off,len);
        if(strlen(Account)!=len||memcmp(Account,PositionDetail.szAccount,len)!=0)
            return false;

        CTools::mygettrimpos(PositionDetail.InstrumentID,off,len);
        if(strlen(InstrumentID)!=len||memcmp(InstrumentID,PositionDetail.InstrumentID,len)!=0)
            return false;

        if( HedgeFlag != PositionDetail.HedgeFlag)
            return false;

        if( PosiDirection==THOST_FTDC_PD_Long && PositionDetail.Direction!=THOST_FTDC_D_Buy ||
            PosiDirection==THOST_FTDC_PD_Short && PositionDetail.Direction!=THOST_FTDC_D_Sell )
            return false;

        return true;
    }
    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=0,ilen;
        prlt[len]=0;

        ilen=strlen(Account);
        if(len+ilen+1<rltsize)   { strcat(prlt,Account);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(InstrumentID);
        if(len+ilen+1<rltsize)   { strcat(prlt,InstrumentID);  strcat(prlt,",");  len+=ilen+1; }

        char buf[256];
        sprintf(buf,"Direct%d,Hedge%d",PosiDirection,HedgeFlag);
        ilen=strlen(buf);
        if(len+ilen<rltsize)     { strcat(prlt,buf);  len+=ilen; }
        return prlt;
    }

	CommonAccountType       Account;			//�˺�
    CommonInstrumentIDType  InstrumentID;      //��ԼID
    char                    PosiDirection;     //��������
    char                    HedgeFlag;         //Ͷ���ױ���־
};

///��ʾһ����ϳֲ���ϸ��¼
struct PositionCombKey
{
    PositionCombKey(const char* account, 
                    const char* instrumentID, 
                    const char* openDate, 
                    const char* leg1TradeID, 
                    const char* leg2TradeID, 
                    const char& direction, 
                    const char& hedgeFlag)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),account);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        CTools::mymemcpy_trim(OpenDate,sizeof(OpenDate),openDate);
        CTools::mymemcpy_trim(Leg1TradeID,sizeof(Leg1TradeID),leg1TradeID);
        CTools::mymemcpy_trim(Leg2TradeID,sizeof(Leg2TradeID),leg2TradeID);

        Direction=direction;
        HedgeFlag=hedgeFlag;
    };

    PositionCombKey(const PlatformStru_PositionComb& PositionComb)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),PositionComb.szAccount);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),PositionComb.InstrumentID);
        CTools::mymemcpy_trim(OpenDate,sizeof(OpenDate),PositionComb.OpenDate);
        CTools::mymemcpy_trim(Leg1TradeID,sizeof(Leg1TradeID),PositionComb.Leg1TradeID);
        CTools::mymemcpy_trim(Leg2TradeID,sizeof(Leg2TradeID),PositionComb.Leg2TradeID);

        Direction=PositionComb.Direction;
        HedgeFlag=PositionComb.HedgeFlag;
    };

    PositionCombKey& operator= (const PositionCombKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));

        return *this;
    };

    //ע�⣬ͨ��OpenDate��TradeID�ıȽ�˳�򣬱�֤set<PositionCombKey>����ͬ�����ļ�¼����ʱ������
    bool operator< (const PositionCombKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(OpenDate,r.OpenDate,sizeof(OpenDate));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=CTools::mycompare2_trynumeric(Leg1TradeID,r.Leg1TradeID);
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=CTools::mycompare2_trynumeric(Leg2TradeID,r.Leg2TradeID);
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(Direction<r.Direction)
		    return true;
	    else if(Direction>r.Direction)
		    return false;

        if(HedgeFlag<r.HedgeFlag)
		    return true;
	    else if(HedgeFlag>r.HedgeFlag) 
		    return false;
	     
		return false;
    }

    bool operator== (const PositionCombKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        irlt=strncmp(OpenDate,r.OpenDate,sizeof(OpenDate));
        if(irlt!=0) return false;

        irlt=strncmp(Leg1TradeID,r.Leg1TradeID,sizeof(Leg1TradeID));
        if(irlt!=0) return false;

        irlt=strncmp(Leg2TradeID,r.Leg2TradeID,sizeof(Leg2TradeID));
        if(irlt!=0) return false;

        if(Direction!=r.Direction)
            return false;

        if(HedgeFlag!=r.HedgeFlag)
            return false;

        return true;
    }
    
    bool operator!= (const PositionCombKey& r) const
    {
        return !(*this == r);
    }
    
    bool operator> (const PositionCombKey& r) const
    {
        return !(*this < r ||*this == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=0,ilen;
        prlt[len]=0;

        ilen=strlen(Account);
        if(len+ilen+1<rltsize)   { strcat(prlt,Account);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(InstrumentID);
        if(len+ilen+1<rltsize)   { strcat(prlt,InstrumentID);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(OpenDate);
        if(len+ilen+1<rltsize)   { strcat(prlt,OpenDate);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(Leg1TradeID);
        if(len+ilen+1<rltsize)   { strcat(prlt,Leg1TradeID);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(Leg2TradeID);
        if(len+ilen+1<rltsize)   { strcat(prlt,Leg2TradeID);  strcat(prlt,",");  len+=ilen+1; }

        char buf[256];
        sprintf(buf,"Direct%d,Hedge%d",Direction,HedgeFlag);
        ilen=strlen(buf);
        if(len+ilen<rltsize)     { strcat(prlt,buf);  len+=ilen; }

        return prlt;
    }

	CommonAccountType       Account;			//�˺�
    CommonInstrumentIDType  InstrumentID;      //��Ϻ�ԼID
    CommonDateType          OpenDate;          //��������
    CommonTradeIDType       Leg1TradeID;       //Leg1�ĳɽ����
    CommonTradeIDType       Leg2TradeID;       //Leg2�ĳɽ����
    char                    Direction;         //��������
    char                    HedgeFlag;         //Ͷ���ױ���־
};






//���涨��һ����Լ��ͬ�ҵ��������ݽṹ
struct WaitOrderVolumeStru
{
    int         OpenVolume_Buy_Speculation;                 //����-��-Ͷ��
    int         CloseVolume_Buy_Speculation;                //ƽ��-��-Ͷ��
    int         CloseTodayVolume_Buy_Speculation;           //ƽ��-��-Ͷ��
    int         CloseYesterdayVolume_Buy_Speculation;       //ƽ��-��-Ͷ��

    int         OpenVolume_Sell_Speculation;                //����-��-Ͷ��
    int         CloseVolume_Sell_Speculation;               //ƽ��-��-Ͷ��
    int         CloseTodayVolume_Sell_Speculation;          //ƽ��-��-Ͷ��
    int         CloseYesterdayVolume_Sell_Speculation;      //ƽ��-��-Ͷ��

    int         OpenVolume_Buy_Hedge;                       //����-��-�ױ�
    int         CloseVolume_Buy_Hedge;                      //ƽ��-��-�ױ�
    int         CloseTodayVolume_Buy_Hedge;                 //ƽ��-��-�ױ�
    int         CloseYesterdayVolume_Buy_Hedge;             //ƽ��-��-�ױ�

    int         OpenVolume_Sell_Hedge;                      //����-��-�ױ�
    int         CloseVolume_Sell_Hedge;                     //ƽ��-��-�ױ�
    int         CloseTodayVolume_Sell_Hedge;                //ƽ��-��-�ױ�
    int         CloseYesterdayVolume_Sell_Hedge;            //ƽ��-��-�ױ�
};

struct ExchangeTime
{
    ExchangeTime()
    {
        CurTickCount=0;
        memset(&TradingDay, 0, sizeof(TradingDay));
        memset(&SHFETime, 0, sizeof(SHFETime));
        memset(&DCETime, 0, sizeof(DCETime));
        memset(&CZCETime, 0, sizeof(CZCETime));
        memset(&FFEXTime, 0, sizeof(FFEXTime));
    };
    
    unsigned long             CurTickCount;//���ص��Կ�����ĺ�������
    TThostFtdcDateType	TradingDay;
    ///������ʱ��
	TThostFtdcTimeType	SHFETime;
	///������ʱ��
	TThostFtdcTimeType	DCETime;
	///֣����ʱ��
	TThostFtdcTimeType	CZCETime;
	///�н���ʱ��
	TThostFtdcTimeType	FFEXTime;
};

struct AbstractBusinessData
{
    BusinessID                 BID;
	void*					   pPlatformSvr;
    unsigned int               Size;//�϶����ڵ���sizeof(AbstractBusinessData)
};

typedef int (*GuiCallBackFunc)(const GUIModuleID,const AbstractBusinessData&);

//����������Ӧ
struct DataRspUserLogin
{
    DataRspUserLogin()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspUserLogin;
        Head.Size = sizeof(*this);
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
    DataRspUserLogout()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspUserLogout;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspUserPasswordUpdate;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspTradingAccountPasswordUpdate;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspParkedOrderInsert;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspParkedOrderAction;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspRemoveParkedOrder;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspRemoveParkedOrderAction;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryParkedOrder;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryParkedOrderAction;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspOrderInsert;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcInputOrderField InputOrderField;
    CThostFtdcRspInfoField    RspInfoField;    
};



//����������ִ(�����ִ1���ۺϽ���ƽ̨��Ϊ������Ч)
struct DataRspOrderAction1
{
    DataRspOrderAction1()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspOrderAction1;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcInputOrderActionField InputOrderActionField;
    CThostFtdcRspInfoField     RspInfoField;    
};

//����������ִ(�����ִ2����������Ϊ������Ч)
struct DataRspOrderAction2
{
    DataRspOrderAction2()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspOrderAction2;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    CThostFtdcOrderActionField OrderActionField;
    CThostFtdcRspInfoField     RspInfoField;    
};

//�����ر�(�µ����󣬻��������Ϣ����������Ҳ���������Ϣ)
struct DataRtnOrder
{
    DataRtnOrder()
    {
        //memset(this, 0, sizeof(*this));PlatformStru_OrderInfo��Ĭ�Ϲ��캯��
        Head.BID = BID_RtnOrder;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    PlatformStru_OrderInfo      OrderField; 
};

//�ɽ��ر�
struct DataRtnTrade
{
    DataRtnTrade()
    {
        //memset(this, 0, sizeof(*this));PlatformStru_TradeInfo��Ĭ�Ϲ��캯��
        Head.BID = BID_RtnTrade;
        Head.Size = sizeof(*this);
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
    bool                            bHavePosition;  //��ʾ�Ƿ��гֲ�
};

//�����ѯͶ���ֲ߳���Ӧ
struct DataRspQryInvestorPosition
{
    DataRspQryInvestorPosition()
    {
        Head.BID = BID_RspQryInvestorPosition;
        Head.Size = sizeof(*this);
        nRequestID=0;
        bIsLast=false;
        InvestorPositionField.Clear();
        memset(&RspInfoField, 0, sizeof(CThostFtdcRspInfoField));
        memset(&QryInstrumentID, 0, sizeof(TThostFtdcInstrumentIDType));
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
    {
        Head.BID = BID_RspQryInvestorPositionDetail;
        Head.Size = sizeof(*this);
        nRequestID=0;
        bIsLast=false;
        InvestorPositionDetailField.Clear();
        memset(&RspInfoField, 0, sizeof(CThostFtdcRspInfoField));
        memset(&QryInstrumentID, 0, sizeof(TThostFtdcInstrumentIDType));
    };

    AbstractBusinessData		Head;
    int							nRequestID;
    bool						bIsLast;
    PlatformStru_PositionDetail	InvestorPositionDetailField;
    CThostFtdcRspInfoField		RspInfoField;    
  	TThostFtdcInstrumentIDType	QryInstrumentID;        //��ѯ�ĺ�ԼID
};

//�����ѯͶ���ֲ߳���Ϻ�Լ��ϸ��Ӧ
struct DataRspQryInvestorPositionComb
{
    DataRspQryInvestorPositionComb()
    {
        Head.BID = BID_RspQryInvestorPositionComb;
        Head.Size = sizeof(*this);
        nRequestID=0;
        bIsLast=false;
        InvestorPositionCombineDetailField.Clear();
        memset(&RspInfoField, 0, sizeof(CThostFtdcRspInfoField));
        memset(&QryInstrumentID, 0, sizeof(TThostFtdcInstrumentIDType));
    };

    AbstractBusinessData		Head;
    int							nRequestID;
    bool						bIsLast;
    PlatformStru_PositionComb	InvestorPositionCombineDetailField;
    CThostFtdcRspInfoField		RspInfoField;    
  	TThostFtdcInstrumentIDType	QryInstrumentID;        //��ѯ�ĺ�ԼID
};

//�����ѯ�ʽ��˻���Ӧ
struct DataRspQryTradingAccount
{
    DataRspQryTradingAccount()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryTradingAccount;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryInvestor;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryTradingCode;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryInstrumentMarginRate;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryInstrumentCommissionRate;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_InstrumentCommissionRate   InstrumentCommissionRateField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�����ѯ��������Ӧ
struct DataRspQryExchange
{
    DataRspQryExchange()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryExchange;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcExchangeField   ExchangeField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//�����ѯ��Լ��Ӧ
struct DataRspQryInstrument
{
    DataRspQryInstrument()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryInstrument;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_InstrumentInfo InstrumentField;
    CThostFtdcRspInfoField     RspInfoField;    
};


//�����ѯ������Ӧ
struct DataRspQryOrder
{
    DataRspQryOrder()
    {
        Head.BID = BID_RspQryOrder;
        Head.Size = sizeof(*this);
        nRequestID=0;
        bIsLast=false;
        OrderField.Clear();
        memset(&RspInfoField, 0, sizeof(CThostFtdcRspInfoField));
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
    DataRspQryTrade()
    {
        Head.BID = BID_RspQryTrade;
        Head.Size = sizeof(*this);
        nRequestID=0;
        bIsLast=false;
        TradeField.Clear();
        memset(&RspInfoField, 0, sizeof(CThostFtdcRspInfoField));  
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQrySettlementInfo;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspFromBankToFutureByFuture;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspFromFutureToBankByFuture;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQueryBankAccountMoneyByFuture;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryTransferSerial;
        Head.Size = sizeof(*this);
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
    DataRspQryAccountregister()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryAccountregister;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RtnFromBankToFutureByFuture;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RtnFromFutureToBankByFuture;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RtnQueryBankAccountMoneyByFuture;
        Head.Size = sizeof(*this);
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
    DataRspQryContractBank()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryContractBank;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcContractBankField      ContractBankField;
    CThostFtdcRspInfoField    RspInfoField;    
};

///��Լ����״̬
struct DataInstrumentStatus
{
    DataInstrumentStatus()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RtnInstrumentStatus;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcInstrumentStatusField      InstrumentStatusField; 
};
//�뽻�׺�̨ͨ�����ӳɹ�
struct DataFrontConnected
{
    DataFrontConnected()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_FrontDisconnected;
        Head.Size = sizeof(*this);
    };
    AbstractBusinessData      Head;
	bool                      bTrader;
};
//�뽻�׺�̨ͨ�����ӶϿ�
struct DataFrontDisconnected
{
    DataFrontDisconnected()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_FrontDisconnected;
        Head.Size = sizeof(*this);
    };
    AbstractBusinessData      Head;
	bool                      bTrader;
    int                       nReason; 
};
struct DataRspQrySettlementInfoConfirm
{
	DataRspQrySettlementInfoConfirm()
	{
		memset(this, 0, sizeof(*this));
		Head.BID = BID_RspQrySettlementInfoConfirm;
		Head.Size = sizeof(*this);
	};
	AbstractBusinessData      Head;
	int                       nRequestID;
	bool                      bIsLast;
	CThostFtdcSettlementInfoConfirmField      SettlementInfoConfirm;
	CThostFtdcRspInfoField    RspInfoField;    
};
//
struct DataTradingNoticeInfoField
{
    DataTradingNoticeInfoField()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RtnTradingNotice;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryNotice;
        Head.Size = sizeof(*this);
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
typedef enum  _tagSStatus
{
	S_STATUS_ADD,
	S_STATUS_DEL,
	S_STATUS_EQU	
}S_Status;
typedef enum  _tagSPanel
{
	S_STATUS_TEMP=(1<<2),
	S_STATUS_QUOT=(1<<3),
	S_STATUS_ORDER=(1<<4),
	S_STATUS_TRADE=(1<<5),
	S_STATUS_POS=(1<<6),
	S_STATUS_PLUS=(1<<7)
}S_Panel;
/************����������**begin********************************************************************************************************/
typedef struct tagServerParamCfgItem
{
	std::vector<std::string> TradeAddr;		//��ȡ����ǰ�û���ַ
	std::vector<std::string> QuotAddr;		//��ȡ����ǰ�û���ַ
	std::string BrokerID;					//��ȡ���͹�˾ID
	std::string InvestorID;					//��¼���˻� 
	std::string ServerName;					//����
	int CTPSimulate;						//�������Ƿ�CTPģ�������, ���ǣ�����Ҫ����Լ��Ӧ��ExchangeID��ԭΪ��ʵ�Ľ�����ID(CTP����ΪSHFE)
}SERVER_PARAM_CFG_ITEM, *LPSERVER_PARAM_CFG_ITEM;

typedef struct tagServerParamCfg
{
	std::vector<SERVER_PARAM_CFG_ITEM> vec;
	std::string UniserverAddr;
	std::string VersionStr;
	int LanguageId;
	int current;
	//��������������������
	std::string ProxyType;//socks4,socks5
	std::string ProxyAddr;
	std::string ProxyPort;
	std::string ProxyUser;
	std::string ProxyPwd;
}SERVER_PARAM_CFG,* LPSERVER_PARAM_CFG;
/************����������**end********************************************************************************************************/








#endif

