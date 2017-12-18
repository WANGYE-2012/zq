#pragma once

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include "tools_util.h"
#include "Tools_Ansi.h"
#include <sys/timeb.h>

#include <string>
#include <vector>

#ifdef WIN32
#pragma managed(push,off)
#endif 

#include "../ctp/ThostFtdcUserApiStruct.h"

#ifndef CTP060300
#define CTP060300
#endif

///ǿƽ(���������ֶ�ǿƽ���Զ�ǿƽ)
#define THOST_FTDC_FCC_ForceClose 'Z'

#define DOUBLEINVALIDVALUE	(util::GetDoubleInvalidValue())
#define INTINVALIDVALUE		(util::GetIntInvalidValue())

//��ִ��һЩ��ѯ����ʱ�����ܲ�ѯ���Ϊ�գ�
//���ۺϽ���ƽ̨��API��Ȼ��Ϊ�˲�ѯ�ɹ���
//���ǽ��������"����"Ϊ��ѯʧ�ܣ����Ը�GUI
//��ģ���账����������ErrorID
#define CustomErrorID 123456789
#define CustomErrorMsg "��ѯ�޼�¼"
#define ORDER_INSERT_FRONTID  -1
#define ORDER_INSERT_SESSIONID -1
#define ORDER_INSERT_ORDERSTATUS 'N'

#define ORDERCLASS_NewOrder 'N'
#define ORDERCLASS_ReplaceOrder 'R'

#define THOST_FTDC_OST_ERROE  'd'

typedef char CommonAccountIDType[13];

typedef char CommonOrderClassType;
typedef char CommonUserIDType[32];                  //�û���ID����Ӧ�ۺϽ���ƽ̨Ͷ����ID��Pats��UserName
typedef char CommonAccountType[32];                 //�˺�
typedef char CommonLocalRequestIDType[32];
typedef char CommonLocalRefIDType[32];
typedef char CommonExchangeIDType[32];
typedef char CommonExchangeNameType[32];            //����������
typedef char CommonCurrencyType[16];                //��������
typedef char CommonOrderRefType[32];                //������OrderRef
typedef char CommonOrderSysIDType[32];              //������OrderSysID
typedef char CommonInstrumentIDType[32];            //��ԼID
typedef char CommonDateType[16];                    //����
typedef char CommonTradeIDType[21];                 //�ɽ�ID

typedef char ContractShortNamesType[4096];			//��Լ��Ʊ�
typedef char PatsystemAccountType[50];

typedef char InstrumentIDType[64];

typedef char TThostFtdcBrokerIDType[11];
typedef char TThostFtdcAccountIDType[13];
typedef double TThostFtdcMoneyType;
typedef char TThostFtdcDateType[9];
typedef int TThostFtdcSettlementIDType;
typedef char TThostFtdcExchangeIDType[9];
typedef char InstrumentIDType[64];
typedef char TThostFtdcExchangeInstIDType[31];
typedef char TThostFtdcProductClassType;
typedef int TThostFtdcYearType;
typedef int TThostFtdcMonthType;
typedef int TThostFtdcVolumeType;
typedef int TThostFtdcVolumeMultipleType;
typedef double TThostFtdcPriceType;
typedef char TThostFtdcInstLifePhaseType;
typedef int TThostFtdcBoolType;
typedef char TThostFtdcPositionTypeType;
typedef char TThostFtdcPositionDateTypeType;
typedef double TThostFtdcRatioType;
typedef char TThostFtdcInstrumentIDType[31];
typedef char TThostFtdcInvestorRangeType;
typedef char TThostFtdcInvestorIDType[13];
typedef char TThostFtdcHedgeFlagType;
typedef char TThostFtdcOrderRefType[13];
typedef char TThostFtdcUserIDType[16];
typedef char TThostFtdcTradeIDType[21];
typedef char TThostFtdcDirectionType;
typedef char TThostFtdcOrderSysIDType[21];
typedef char TThostFtdcParticipantIDType[11];
typedef char TThostFtdcClientIDType[11];
typedef char TThostFtdcTradingRoleType;
typedef char TThostFtdcOffsetFlagType;
typedef char TThostFtdcTimeType[9];
typedef char TThostFtdcTradeTypeType;
typedef char TThostFtdcPriceSourceType;
typedef char TThostFtdcTraderIDType[21];
typedef char TThostFtdcOrderLocalIDType[13];
typedef char TThostFtdcBusinessUnitType[21];
typedef int TThostFtdcSequenceNoType;
typedef char TThostFtdcTradeSourceType;
typedef char TThostFtdcOrderPriceTypeType;
typedef char TThostFtdcCombOffsetFlagType[5];
typedef char TThostFtdcCombHedgeFlagType[5];
typedef char TThostFtdcTimeConditionType;
typedef char TThostFtdcVolumeConditionType;
typedef char TThostFtdcContingentConditionType;
typedef char TThostFtdcForceCloseReasonType;
typedef int TThostFtdcRequestIDType;
typedef int TThostFtdcInstallIDType;
typedef char TThostFtdcOrderSubmitStatusType;
typedef char TThostFtdcOrderSourceType;
typedef char TThostFtdcOrderStatusType;
typedef char TThostFtdcOrderTypeType;
typedef int TThostFtdcSessionIDType;
typedef int TThostFtdcFrontIDType;
typedef char TThostFtdcProductInfoType[11];
typedef char TThostFtdcErrorMsgType[81];
typedef double TThostFtdcLargeVolumeType;
typedef int TThostFtdcMillisecType;
typedef char TThostFtdcPasswordType[41];
typedef char TThostFtdcParkedOrderIDType[13];
typedef char TThostFtdcUserTypeType;
typedef char TThostFtdcParkedOrderStatusType;
typedef int TThostFtdcErrorIDType;
typedef int TThostFtdcOrderActionRefType;
typedef char TThostFtdcActionFlagType;
typedef char TThostFtdcParkedOrderActionIDType[13];
typedef char TThostFtdcBankIDType[4];
typedef char TThostFtdcBankNameType[101];
typedef char TThostFtdcBankAccountType[41];
typedef char TThostFtdcCurrencyIDType[4];
typedef char TThostFtdcBankBrchIDType[5];
typedef int TThostFtdcLegIDType;
typedef int TThostFtdcLegMultipleType;
typedef char TThostFtdcPosiDirectionType;
typedef char TThostFtdcPositionDateType;
typedef char TThostFtdcTradeCodeType[7];
typedef char TThostFtdcFutureBranchIDType[31];
typedef char TThostFtdcTradeDateType[9];
typedef char TThostFtdcTradeTimeType[9];
typedef char TThostFtdcBankSerialType[13];
typedef int TThostFtdcSerialType;
typedef char TThostFtdcLastFragmentType;
typedef char TThostFtdcIndividualNameType[51];
typedef char TThostFtdcIdCardTypeType;
typedef char TThostFtdcIdentifiedCardNoType[51];
typedef char TThostFtdcGenderType;
typedef char TThostFtdcCountryCodeType[21];
typedef char TThostFtdcCustTypeType;
typedef double TThostFtdcTradeAmountType;
typedef char TThostFtdcFeePayFlagType;
typedef double TThostFtdcCustFeeType;
typedef double TThostFtdcFutureFeeType;
typedef char TThostFtdcAddInfoType[129];
typedef int TThostFtdcFutureSerialType;
typedef char TThostFtdcYesNoIndicatorType;
typedef char TThostFtdcDigestType[36];
typedef char TThostFtdcBankAccTypeType;
typedef char TThostFtdcDeviceIDType[3];
typedef char TThostFtdcBankAccTypeType;
typedef char TThostFtdcBankCodingForFutureType[33];
typedef char TThostFtdcPwdFlagType;
typedef char TThostFtdcOperNoType[17];
typedef int TThostFtdcTIDType;
typedef char TThostFtdcTransferStatusType;
typedef double TThostFtdcUnderlyingMultipleType;


///////////////////////////////////////////////////////////////////////////////////
struct CThostFtdcTradingAccountFieldEx
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ϴ���Ѻ���
	TThostFtdcMoneyType	PreMortgage;
	///�ϴ����ö��
	TThostFtdcMoneyType	PreCredit;
	///�ϴδ���
	TThostFtdcMoneyType	PreDeposit;
	///�ϴν���׼����
	TThostFtdcMoneyType	PreBalance;
	///�ϴ�ռ�õı�֤��
	TThostFtdcMoneyType	PreMargin;
	///��Ϣ����
	TThostFtdcMoneyType	InterestBase;
	///��Ϣ����
	TThostFtdcMoneyType	Interest;
	///�����
	TThostFtdcMoneyType	Deposit;
	///������
	TThostFtdcMoneyType	Withdraw;
	///����ı�֤��
	TThostFtdcMoneyType	FrozenMargin;
	///������ʽ�
	TThostFtdcMoneyType	FrozenCash;
	///�����������
	TThostFtdcMoneyType	FrozenCommission;
	///��ǰ��֤���ܶ�
	TThostFtdcMoneyType	CurrMargin;
	///�ʽ���
	TThostFtdcMoneyType	CashIn;
	///������
	TThostFtdcMoneyType	Commission;
	///ƽ��ӯ��
	TThostFtdcMoneyType	CloseProfit;
	///�ֲ�ӯ��
	TThostFtdcMoneyType	PositionProfit;
	///�ڻ�����׼����
	TThostFtdcMoneyType	Balance;
	///�����ʽ�
	TThostFtdcMoneyType	Available;
	///��ȡ�ʽ�
	TThostFtdcMoneyType	WithdrawQuota;
	///����׼����
	TThostFtdcMoneyType	Reserve;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	TThostFtdcSettlementIDType	SettlementID;
	///���ö��
	TThostFtdcMoneyType	Credit;
	///��Ѻ���
	TThostFtdcMoneyType	Mortgage;
	///��������֤��
	TThostFtdcMoneyType	ExchangeMargin;
	///Ͷ���߽��֤��
	TThostFtdcMoneyType	DeliveryMargin;
	///���������֤��
	TThostFtdcMoneyType	ExchangeDeliveryMargin;
	///��̬Ȩ��
	TThostFtdcMoneyType StaticProfit;
	///��̬Ȩ��
	TThostFtdcMoneyType DynamicProfit;
	///���ն�
	TThostFtdcMoneyType RiskDegree;
};


///��¼��Ϣ
struct PlatformStru_LoginInfo
{
	///������
	TThostFtdcDateType	TradingDay;
	///��¼�ɹ�ʱ��
	TThostFtdcTimeType	LoginTime;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///����ϵͳ����
	TThostFtdcSystemNameType	SystemName;
	///ǰ�ñ��
	TThostFtdcFrontIDType	FrontID;
	///�Ự���
	TThostFtdcSessionIDType	SessionID;
	///��󱨵�����
	TThostFtdcOrderRefType	MaxOrderRef;
	///������ʱ��
	TThostFtdcTimeType	SHFETime;
	///������ʱ��
	TThostFtdcTimeType	DCETime;
	///֣����ʱ��
	TThostFtdcTimeType	CZCETime;
	///�н���ʱ��
	TThostFtdcTimeType	FFEXTime;
	///��Դ����ʱ��
	TThostFtdcTimeType	INETime;

    //��¼�ɹ�ʱ�ı���ʱ�䣬Ҳ�������ô˽ṹ���ݵ�ʱ��
    struct timeb        LoginTimeLocal;


    PlatformStru_LoginInfo()
    {
        clear();
    }
    PlatformStru_LoginInfo(const CThostFtdcRspUserLoginField& r)
    {
        clear();

        strncpy(TradingDay,r.TradingDay,sizeof(TradingDay));
        strncpy(LoginTime,r.LoginTime,sizeof(LoginTime));
        strncpy(BrokerID,r.BrokerID,sizeof(BrokerID));
        strncpy(UserID,r.UserID,sizeof(UserID));
        strncpy(SystemName,r.SystemName,sizeof(SystemName));
        FrontID=r.FrontID;
        SessionID=r.SessionID;
        strncpy(MaxOrderRef,r.MaxOrderRef,sizeof(MaxOrderRef));
        strncpy(SHFETime,r.SHFETime,sizeof(SHFETime));
        strncpy(DCETime,r.DCETime,sizeof(DCETime));
        strncpy(CZCETime,r.CZCETime,sizeof(CZCETime));
        strncpy(FFEXTime,r.FFEXTime,sizeof(FFEXTime));
        strncpy(INETime,r.INETime,sizeof(INETime));

        ftime(&LoginTimeLocal);
    }
    void clear()
    {
        memset(this,0,sizeof(*this));
    }
};

///���͹�˾���ײ�����Ϣ
struct PlatformStru_BrokerTradingParams
{
	PlatformStru_BrokerTradingParams(void)
	{
		clear();
	}
	PlatformStru_BrokerTradingParams(const CThostFtdcBrokerTradingParamsField& r)
	{
		clear();

        strncpy(BrokerID,r.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,r.InvestorID,sizeof(InvestorID)-1);
        MarginPriceType=r.MarginPriceType;
        Algorithm=r.Algorithm;
        AvailIncludeCloseProfit=r.AvailIncludeCloseProfit;
        strncpy(CurrencyID,r.CurrencyID,sizeof(CurrencyID)-1);
#ifdef CTP060300
        OptionRoyaltyPriceType=r.OptionRoyaltyPriceType;
#endif
	}
    void clear(void)
    {
        memset(this,0,sizeof(*this));
    };

	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��֤��۸�����
	TThostFtdcMarginPriceTypeType	MarginPriceType;
	///ӯ���㷨
	TThostFtdcAlgorithmType	Algorithm;
	///�����Ƿ����ƽ��ӯ��
	TThostFtdcIncludeCloseProfitType	AvailIncludeCloseProfit;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///��ȨȨ����۸�����
	TThostFtdcOptionRoyaltyPriceTypeType	OptionRoyaltyPriceType;
};


///��Ʒ��Ϣ
struct PlatformStru_ProductInfo
{
	PlatformStru_ProductInfo(void)
	{
		clear();
	}
	PlatformStru_ProductInfo(const CThostFtdcProductField& r)
	{
		clear();

        strncpy(ProductID,r.ProductID,sizeof(ProductID)-1);
        strncpy(ProductName,r.ProductName,sizeof(ProductName)-1);
        strncpy(ExchangeID,r.ExchangeID,sizeof(ExchangeID)-1);
        ProductClass=r.ProductClass;
        VolumeMultiple=r.VolumeMultiple;
        PriceTick=r.PriceTick;
        MaxMarketOrderVolume=r.MaxMarketOrderVolume;
        MinMarketOrderVolume=r.MinMarketOrderVolume;
        MaxLimitOrderVolume=r.MaxLimitOrderVolume;
        MinLimitOrderVolume=r.MinLimitOrderVolume;
        PositionType=r.PositionType;
        PositionDateType=r.PositionDateType;
        CloseDealType=r.CloseDealType;
        strncpy(TradeCurrencyID,r.TradeCurrencyID,sizeof(TradeCurrencyID)-1);
        MortgageFundUseRange=r.MortgageFundUseRange;
#ifdef CTP060300
        strncpy(ExchangeProductID,r.ExchangeProductID,sizeof(ExchangeProductID)-1);
        UnderlyingMultiple=r.UnderlyingMultiple;
#endif
        strncpy(ProductShortName,r.ProductName,sizeof(ProductShortName)-1);
	}
    void clear(void)
    {
        memset(this,0,sizeof(*this));
    };

    ///��Ʒ����
	TThostFtdcInstrumentIDType	ProductID;
	///��Ʒ����
	TThostFtdcProductNameType	ProductName;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ʒ����
	TThostFtdcProductClassType	ProductClass;
	///��Լ��������
	TThostFtdcVolumeMultipleType	VolumeMultiple;
	///��С�䶯��λ
	TThostFtdcPriceType	PriceTick;
	///�м۵�����µ���
	TThostFtdcVolumeType	MaxMarketOrderVolume;
	///�м۵���С�µ���
	TThostFtdcVolumeType	MinMarketOrderVolume;
	///�޼۵�����µ���
	TThostFtdcVolumeType	MaxLimitOrderVolume;
	///�޼۵���С�µ���
	TThostFtdcVolumeType	MinLimitOrderVolume;
	///�ֲ�����
	TThostFtdcPositionTypeType	PositionType;
	///�ֲ���������
	TThostFtdcPositionDateTypeType	PositionDateType;
	///ƽ�ִ�������
	TThostFtdcCloseDealTypeType	CloseDealType;
	///���ױ�������
	TThostFtdcCurrencyIDType	TradeCurrencyID;
	///��Ѻ�ʽ���÷�Χ
	TThostFtdcMortgageFundUseRangeType	MortgageFundUseRange;
	///��������Ʒ����
	TThostFtdcInstrumentIDType	ExchangeProductID;
	///��Լ������Ʒ����
	TThostFtdcUnderlyingMultipleType	UnderlyingMultiple;

    ///��Ʒ��ƣ�ȱʡ����µ��ڲ�Ʒ���ƣ���Ʒ����̫��ʱ�Զ���
    TThostFtdcProductNameType   ProductShortName;
};

///��Լ��Ϣ
struct PlatformStru_InstrumentInfo
{
	PlatformStru_InstrumentInfo(void)
	{
		clear();
	}
	PlatformStru_InstrumentInfo(const CThostFtdcInstrumentField& r)
	{
		clear();

		strncpy(InstrumentID, r.InstrumentID,sizeof(InstrumentID)-1);
		strncpy(ExchangeID, r.ExchangeID,sizeof(ExchangeID)-1);
		strncpy(InstrumentName, r.InstrumentName,sizeof(InstrumentName)-1);
		strncpy(ExchangeInstID, r.ExchangeInstID,sizeof(ExchangeInstID)-1);
		strncpy(ProductID, r.ProductID,sizeof(ProductID)-1);
		ProductClass = r.ProductClass;
		DeliveryYear = r.DeliveryYear;
		DeliveryMonth = r.DeliveryMonth;
		MaxMarketOrderVolume = r.MaxMarketOrderVolume;
		MinMarketOrderVolume = r.MinMarketOrderVolume;
		MaxLimitOrderVolume = r.MaxLimitOrderVolume;
		MinLimitOrderVolume = r.MinLimitOrderVolume;
		VolumeMultiple = r.VolumeMultiple;
		PriceTick = r.PriceTick;
		strncpy(CreateDate,r.CreateDate,sizeof(CreateDate)-1);
		strncpy(OpenDate,r.OpenDate,sizeof(OpenDate)-1);
		strncpy(ExpireDate, r.ExpireDate,sizeof(ExpireDate)-1);
		strncpy(StartDelivDate, r.StartDelivDate,sizeof(StartDelivDate)-1);
		strncpy(EndDelivDate, r.EndDelivDate,sizeof(EndDelivDate)-1);
		InstLifePhase = r.InstLifePhase;
		IsTrading = r.IsTrading;
		PositionType = r.PositionType;
		PositionDateType = r.PositionType;
		LongMarginRatio = r.LongMarginRatio;
		ShortMarginRatio = r.ShortMarginRatio;
        MaxMarginSideAlgorithm=r.MaxMarginSideAlgorithm;

#ifdef CTP060300
		strncpy(UnderlyingInstrID, r.UnderlyingInstrID,sizeof(UnderlyingInstrID)-1);
		StrikePrice = r.StrikePrice;
		OptionsType = r.OptionsType;
		UnderlyingMultiple = r.UnderlyingMultiple;
		CombinationType = r.CombinationType;
#endif

	}
    void clear(void)
    {
        memset(this,0,sizeof(*this));
    };

	///��Լ����
	InstrumentIDType	        InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ����
	InstrumentIDType	        InstrumentName;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///��Ʒ����
	InstrumentIDType	        ProductID;
	///��Ʒ����
	TThostFtdcProductClassType	ProductClass;
	///�������
	TThostFtdcYearType	        DeliveryYear;
	///������
	TThostFtdcMonthType	        DeliveryMonth;
	///�м۵�����µ���
	TThostFtdcVolumeType	    MaxMarketOrderVolume;
	///�м۵���С�µ���
	TThostFtdcVolumeType	    MinMarketOrderVolume;
	///�޼۵�����µ���
	TThostFtdcVolumeType	    MaxLimitOrderVolume;
	///�޼۵���С�µ���
	TThostFtdcVolumeType	    MinLimitOrderVolume;
	///��Լ��������
	TThostFtdcVolumeMultipleType	VolumeMultiple;
	///��С�䶯��λ
	TThostFtdcPriceType	        PriceTick;
	///������
	TThostFtdcDateType	        CreateDate;
	///������
	TThostFtdcDateType	        OpenDate;
	///������
	TThostFtdcDateType	        ExpireDate;
	///��ʼ������
	TThostFtdcDateType	        StartDelivDate;
	///����������
	TThostFtdcDateType	        EndDelivDate;
	///��Լ��������״̬
	TThostFtdcInstLifePhaseType	InstLifePhase;
	///�����Ƿ���
	TThostFtdcBoolType	        IsTrading;
	///�ֲ�����
	TThostFtdcPositionTypeType	PositionType;
	///�ֲ���������
	TThostFtdcPositionDateTypeType	PositionDateType;
	///��ͷ��֤����
	TThostFtdcRatioType	        LongMarginRatio;
	///��ͷ��֤����
	TThostFtdcRatioType	        ShortMarginRatio;
    ///�Ƿ�ʹ�ô��߱�֤���㷨(��������������ͬһ�ͻ���ͬһ��Ա����ͬƷ��˫��ֲ�ֱ�Ӱ��ձ�֤����ϴ��һ����ȡ���ױ�֤�𣻶Խ����·ݺ�Լ�����5�������ղ��μӵ������۵�)
   	TThostFtdcMaxMarginSideAlgorithmType	MaxMarginSideAlgorithm;

    ///������ctp API 6.3.0���������
    ///������Ʒ����
	TThostFtdcInstrumentIDType	UnderlyingInstrID;
	///ִ�м�
	TThostFtdcPriceType	StrikePrice;
	///��Ȩ����
	TThostFtdcOptionsTypeType	OptionsType;
	///��Լ������Ʒ����
	TThostFtdcUnderlyingMultipleType	UnderlyingMultiple;
	///�������
	TThostFtdcCombinationTypeType	CombinationType;



    ///�������Լ������������ɵ���
    ///��ϱ�֤���Żݷ�����0-���Ż�(ȱʡ)��1-��һ���Żݣ�2-�ڶ����Żݣ�3-����������и�����ȡ��4-���߱�֤����ȡ(����)
    int                         CombMarginDiscountMode;
    ///ƽ��˳��: 0: ͳһ�ȿ���ƽ��1: �ȵ�һ������ȿ���ƽ
    int                         UnwindPriorities;
    ///�����ֱ�֤��ʹ�õļ۸�: 0: ���ɽ��ۼ���(ȱʡ)��1: �������ۼ���
    int                         PriceForMarginOfTodayPosition;
    ///��ƽ��ָ���֧�֣�0:��֧��(��ctp���ܻ�ת����ƽ��)��1:֧��
    int                         CloseTodayInstructionSupport;
    ///��ƽ��ָ���֧�֣�0:��֧�֣�1:֧�֣�����ƽ��ֺͽ�֣�2:֧�֣�ֻ��ƽ���
    int                         CloseInstructionSupport;

	char Currency [11];
	int  TicksPerPoint;
	char TickSize[10];
};

///��������Ϣ
struct PlatformStru_InstrumentCommissionRate
{
    PlatformStru_InstrumentCommissionRate()
    {
        clear();
    };
    PlatformStru_InstrumentCommissionRate(const CThostFtdcInstrumentCommissionRateField& r)
    {
        clear();

        strncpy(InstrumentID,r.InstrumentID,sizeof(InstrumentID)-1);
        InvestorRange=r.InvestorRange;
        strncpy(BrokerID,r.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,r.InvestorID,sizeof(InvestorID)-1);
        OpenRatioByMoney=r.OpenRatioByMoney;
        OpenRatioByVolume=r.OpenRatioByVolume;
        CloseRatioByMoney=r.CloseRatioByMoney;
        CloseRatioByVolume=r.CloseRatioByVolume;
        CloseTodayRatioByMoney=r.CloseTodayRatioByMoney;
        CloseTodayRatioByVolume=r.CloseTodayRatioByVolume;
    };
#ifdef CTP060300
    PlatformStru_InstrumentCommissionRate(const CThostFtdcOptionInstrCommRateField& r)
    {
        clear();

        strncpy(InstrumentID,r.InstrumentID,sizeof(InstrumentID)-1);
        InvestorRange=r.InvestorRange;
        strncpy(BrokerID,r.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,r.InvestorID,sizeof(InvestorID)-1);
        OpenRatioByMoney=r.OpenRatioByMoney;
        OpenRatioByVolume=r.OpenRatioByVolume;
        CloseRatioByMoney=r.CloseRatioByMoney;
        CloseRatioByVolume=r.CloseRatioByVolume;
        CloseTodayRatioByMoney=r.CloseTodayRatioByMoney;
        CloseTodayRatioByVolume=r.CloseTodayRatioByVolume;
        StrikeRatioByMoney=r.StrikeRatioByMoney;
        StrikeRatioByVolume=r.StrikeRatioByVolume;
    };
#endif
    void clear(void)
    {
        memset(this,0,sizeof(*this));
        OpenRatioByMoney=util::GetDoubleInvalidValue();
        OpenRatioByVolume=util::GetDoubleInvalidValue();
        CloseRatioByMoney=util::GetDoubleInvalidValue();
        CloseRatioByVolume=util::GetDoubleInvalidValue();
        CloseTodayRatioByMoney=util::GetDoubleInvalidValue();
        CloseTodayRatioByVolume=util::GetDoubleInvalidValue();
        StrikeRatioByMoney=util::GetDoubleInvalidValue();
        StrikeRatioByVolume=util::GetDoubleInvalidValue();
    };

	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	TThostFtdcInvestorRangeType	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������������
	TThostFtdcRatioType	OpenRatioByMoney;
	///����������
	TThostFtdcRatioType	OpenRatioByVolume;
	///ƽ����������
	TThostFtdcRatioType	CloseRatioByMoney;
	///ƽ��������
	TThostFtdcRatioType	CloseRatioByVolume;
	///ƽ����������
	TThostFtdcRatioType	CloseTodayRatioByMoney;
	///ƽ��������
	TThostFtdcRatioType	CloseTodayRatioByVolume;
    ///ִ����������(��Ȩ)
	TThostFtdcRatioType	StrikeRatioByMoney;
	///ִ��������(��Ȩ)
	TThostFtdcRatioType	StrikeRatioByVolume;
};

///��֤����Ϣ
struct PlatformStru_InstrumentMarginRate
{
    PlatformStru_InstrumentMarginRate()
    {
        clear();
    };
    PlatformStru_InstrumentMarginRate(const CThostFtdcInstrumentMarginRateField& r)
    {
        clear();

        strncpy(InstrumentID,r.InstrumentID,sizeof(InstrumentID)-1);
        InvestorRange=r.InvestorRange;
        strncpy(BrokerID,r.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,r.InvestorID,sizeof(InvestorID)-1);
        HedgeFlag=r.HedgeFlag;
        LongMarginRatioByMoney=r.LongMarginRatioByMoney;
        LongMarginRatioByVolume=r.LongMarginRatioByVolume;
        ShortMarginRatioByMoney=r.ShortMarginRatioByMoney;
        ShortMarginRatioByVolume=r.ShortMarginRatioByVolume;
        IsRelative=r.IsRelative;
    };
#ifdef CTP060300
    PlatformStru_InstrumentMarginRate(const CThostFtdcOptionInstrTradeCostField& r)
    {
        clear();

        strncpy(InstrumentID,r.InstrumentID,sizeof(InstrumentID)-1);
        strncpy(BrokerID,r.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,r.InvestorID,sizeof(InvestorID)-1);
        HedgeFlag=r.HedgeFlag;
        FixedMargin=r.FixedMargin;
        MiniMargin=r.MiniMargin;
        Royalty=r.Royalty;
        ExchFixedMargin=r.ExchFixedMargin;
        ExchMiniMargin=r.ExchMiniMargin;
    };
#endif
    void clear(void)
    {
        memset(this,0,sizeof(*this));
        LongMarginRatioByMoney=util::GetDoubleInvalidValue();
        LongMarginRatioByVolume=util::GetDoubleInvalidValue();
        ShortMarginRatioByMoney=util::GetDoubleInvalidValue();
        ShortMarginRatioByVolume=util::GetDoubleInvalidValue();
        FixedMargin=util::GetDoubleInvalidValue();
        MiniMargin=util::GetDoubleInvalidValue();
        Royalty=util::GetDoubleInvalidValue();
        ExchFixedMargin=util::GetDoubleInvalidValue();
        ExchMiniMargin=util::GetDoubleInvalidValue();
    };

	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	TThostFtdcInvestorRangeType	InvestorRange;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///Ͷ���ױ���־
	TThostFtdcHedgeFlagType	HedgeFlag;
	///��ͷ��֤����
	TThostFtdcRatioType	LongMarginRatioByMoney;
	///��ͷ��֤���
	TThostFtdcMoneyType	LongMarginRatioByVolume;
	///��ͷ��֤����
	TThostFtdcRatioType	ShortMarginRatioByMoney;
	///��ͷ��֤���
	TThostFtdcMoneyType	ShortMarginRatioByVolume;
	///�Ƿ���Խ�������ȡ
	TThostFtdcBoolType	IsRelative;

    ///����������Ȩ��֤�����
	///��Ȩ��Լ��֤�𲻱䲿��
	TThostFtdcMoneyType	FixedMargin;
	///��Ȩ��Լ��С��֤��
	TThostFtdcMoneyType	MiniMargin;
	///��Ȩ��ԼȨ����
	TThostFtdcMoneyType	Royalty;
	///��������Ȩ��Լ��֤�𲻱䲿��
	TThostFtdcMoneyType	ExchFixedMargin;
	///��������Ȩ��Լ��С��֤��
	TThostFtdcMoneyType	ExchMiniMargin;
};

///��Ȩ���׳ɱ�
struct PlatformStru_OptionInstrTradeCost
{
    PlatformStru_OptionInstrTradeCost()
    {
        clear();
    };
#ifdef CTP060300
    PlatformStru_OptionInstrTradeCost(const CThostFtdcOptionInstrTradeCostField& r)
    {
        strncpy(BrokerID,r.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,r.InvestorID,sizeof(InvestorID)-1);
        strncpy(InstrumentID,r.InstrumentID,sizeof(InstrumentID)-1);
        HedgeFlag=r.HedgeFlag;
        FixedMargin=r.FixedMargin;
        MiniMargin=r.MiniMargin;
        Royalty=r.Royalty;
        ExchFixedMargin=r.ExchFixedMargin;
        ExchMiniMargin=r.ExchMiniMargin;
    };
#endif
    void clear(void)
    {
        memset(this,0,sizeof(*this));
        FixedMargin=util::GetDoubleInvalidValue();
        MiniMargin=util::GetDoubleInvalidValue();
        Royalty=util::GetDoubleInvalidValue();
        ExchFixedMargin=util::GetDoubleInvalidValue();
        ExchMiniMargin=util::GetDoubleInvalidValue();
    };

	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	TThostFtdcHedgeFlagType	HedgeFlag;
	///��Ȩ��Լ��֤�𲻱䲿��
	TThostFtdcMoneyType	FixedMargin;
	///��Ȩ��Լ��С��֤��
	TThostFtdcMoneyType	MiniMargin;
	///��Ȩ��ԼȨ����
	TThostFtdcMoneyType	Royalty;
	///��������Ȩ��Լ��֤�𲻱䲿��
	TThostFtdcMoneyType	ExchFixedMargin;
	///��������Ȩ��Լ��С��֤��
	TThostFtdcMoneyType	ExchMiniMargin;
};

///�����˺���Ϣ
struct PlatformStru_TradingAccountInfo
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ϴ���Ѻ���
	TThostFtdcMoneyType	PreMortgage;
	///�ϴ����ö��
	TThostFtdcMoneyType	PreCredit;
	///�ϴδ���
	TThostFtdcMoneyType	PreDeposit;
	///�ϴν���׼����
	TThostFtdcMoneyType	PreBalance;
	///�ϴ�ռ�õı�֤��
	TThostFtdcMoneyType	PreMargin;
	///��Ϣ����
	TThostFtdcMoneyType	InterestBase;
	///��Ϣ����
	TThostFtdcMoneyType	Interest;
	///�����
	TThostFtdcMoneyType	Deposit;
	///������
	TThostFtdcMoneyType	Withdraw;
	///����ı�֤��
	TThostFtdcMoneyType	FrozenMargin;
	///������ʽ�
	TThostFtdcMoneyType	FrozenCash;
	///�����������
	TThostFtdcMoneyType	FrozenCommission;
	///��ǰ��֤���ܶ�
	TThostFtdcMoneyType	CurrMargin;
	///�ʽ���
	TThostFtdcMoneyType	CashIn;
	///������
	TThostFtdcMoneyType	Commission;
	///ƽ��ӯ��
	TThostFtdcMoneyType	CloseProfit;
	///�ֲ�ӯ��
	TThostFtdcMoneyType	PositionProfit;
	///�ڻ�����׼����
	TThostFtdcMoneyType	Balance;
	///�����ʽ�
	TThostFtdcMoneyType	Available;
	///��ȡ�ʽ�
	TThostFtdcMoneyType	WithdrawQuota;
	///����׼����
	TThostFtdcMoneyType	Reserve;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	TThostFtdcSettlementIDType	SettlementID;
	///���ö��
	TThostFtdcMoneyType	Credit;
	///��Ѻ���
	TThostFtdcMoneyType	Mortgage;
	///��������֤��
	TThostFtdcMoneyType	ExchangeMargin;
	///Ͷ���߽��֤��
	TThostFtdcMoneyType	DeliveryMargin;
	///���������֤��
	TThostFtdcMoneyType	ExchangeDeliveryMargin;
	///��̬Ȩ��
	TThostFtdcMoneyType StaticProfit;
	///��̬Ȩ��
	TThostFtdcMoneyType DynamicProfit;
	///���ն�
	TThostFtdcMoneyType RiskDegree;


	/*patsystem*/
	PatsystemAccountType Account;
	//ʣ�๺����
	PatsystemAccountType BPRemaining;
	//��Լռ�õ��ʽ�
	PatsystemAccountType BPUsed;	
	//��Լռ���ʽ����
	PatsystemAccountType Exposure;
	//��Լ��ӯ������
	PatsystemAccountType BurnRate;
	PatsystemAccountType MarginPaid;

    PlatformStru_TradingAccountInfo()
    {
        clear();

		const double dInvalidDouble = util::GetDoubleInvalidValue();

	    PreMortgage = dInvalidDouble;
	    PreCredit = dInvalidDouble;
	    PreDeposit = dInvalidDouble;
	    PreBalance = dInvalidDouble;
	    PreMargin = dInvalidDouble;
	    InterestBase = dInvalidDouble;
	    Interest = dInvalidDouble;
	    Deposit = dInvalidDouble;
	    Withdraw = dInvalidDouble;
	    FrozenMargin = dInvalidDouble;
	    FrozenCash = dInvalidDouble;
	    FrozenCommission = dInvalidDouble;
	    CurrMargin = dInvalidDouble;
	    CashIn = dInvalidDouble;
	    Commission = dInvalidDouble;
	    CloseProfit = dInvalidDouble;
	    PositionProfit = dInvalidDouble;
	    Balance = dInvalidDouble;
	    Available = dInvalidDouble;
	    WithdrawQuota = dInvalidDouble;
	    Reserve = dInvalidDouble;

	    Credit = dInvalidDouble;
	    Mortgage = dInvalidDouble;
	    ExchangeMargin = dInvalidDouble;
	    DeliveryMargin = dInvalidDouble;
	    ExchangeDeliveryMargin = dInvalidDouble;
	    StaticProfit = dInvalidDouble;
	    DynamicProfit = dInvalidDouble;
	    RiskDegree = dInvalidDouble;
    };
    void clear(void)
    {
        memset(this,0,sizeof(*this));
    };
};

///�ɽ���Ϣ
struct PlatformStru_TradeInfo
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	TThostFtdcOrderRefType	OrderRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�ɽ����
	TThostFtdcTradeIDType	TradeID;
	///��������
	TThostFtdcDirectionType	Direction;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///���׽�ɫ
	TThostFtdcTradingRoleType	TradingRole;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///��ƽ��־
	TThostFtdcOffsetFlagType	OffsetFlag;
	///Ͷ���ױ���־
	TThostFtdcHedgeFlagType	HedgeFlag;
	///�۸�
	TThostFtdcPriceType	Price;
	///����
	TThostFtdcVolumeType	Volume;
	///�ɽ�ʱ��
	TThostFtdcDateType	TradeDate;
	///�ɽ�ʱ��
	TThostFtdcTimeType	TradeTime;
	///�ɽ�����
	TThostFtdcTradeTypeType	TradeType;
	///�ɽ�����Դ
	TThostFtdcPriceSourceType	PriceSource;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///���ر������
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///�����Ա���
	TThostFtdcParticipantIDType	ClearingPartID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///���
	TThostFtdcSequenceNoType	SequenceNo;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	TThostFtdcSettlementIDType	SettlementID;
	///���͹�˾�������
	TThostFtdcSequenceNoType	BrokerOrderSeq;
	///�ɽ���Դ
	TThostFtdcTradeSourceType	TradeSource;


	//�˺�
	CommonAccountType       Account;


    double                  PriceTick;                      //��InstrumentInfo��ȡ��

    //ƽ������ƽ�ֳɽ���Ч����Ҫ���ݳֲ���ϸ�����㡣�ɽ��ر�����ѯ�ɽ���������ѯ�ֲ���ϸ��������(�ڳֲ���ϸ��)���Լ���ƽ��������������Ͳ�ѯ���ʽ�������������ѣ�
    int                     CloseTodayVolume;
	//�����ѡ�����ƽ���Żݵ����⣬��Ҫ��ȷƽ��������ܼ���������
	double                  TradeCommission;

    ///���ն���ƽ��ӯ�������ڲ�ѯ�õ��ĳɽ���¼��ƽ��ӯ��Ϊ��Чֵ�����ڳɽ��ر��õ��ĳɽ���¼��ƽ��ӯ���ڸ��³ֲ���ϸ��õ������ڻ�����������˽���
	double                  CloseProfitByDate;
	///��ʶԳ�ƽ��ӯ����ע������ͬ��
	double                  CloseProfitByTrade;


	//FasttraderID,����ÿһ����¼��Key������һ��Ψһ���
	long FTID;                                                
	//ÿ���������ݷ����仯��UpdateSeq��һ
	long UpdateSeq;


	PlatformStru_TradeInfo()
	{
		Clear();
	}
	PlatformStru_TradeInfo(const CThostFtdcTradeField& r)
	{
		Clear();

		strncpy(BrokerID, r.BrokerID,sizeof(BrokerID)-1);
		strncpy(InvestorID, r.InvestorID,sizeof(InvestorID)-1);
		strncpy(InstrumentID, r.InstrumentID,sizeof(InstrumentID)-1);
		strncpy(OrderRef, r.OrderRef,sizeof(OrderRef)-1);
		strncpy(UserID, r.UserID,sizeof(UserID)-1);
		strncpy(ExchangeID, r.ExchangeID,sizeof(ExchangeID)-1);

        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),r.TradeID);

        Direction = r.Direction;

        strncpy(OrderSysID,r.OrderSysID,sizeof(OrderSysID)-1);

		strncpy(ParticipantID, r.ParticipantID,sizeof(ParticipantID)-1);
		strncpy(ClientID, r.ClientID,sizeof(ClientID)-1);
		TradingRole = r.TradingRole;
		strncpy(ExchangeInstID, r.ExchangeInstID,sizeof(ExchangeInstID)-1);
		OffsetFlag = r.OffsetFlag;
		HedgeFlag = r.HedgeFlag;
		Price = r.Price;
		Volume = r.Volume;
		strncpy(TradeDate, r.TradeDate,sizeof(TradeDate)-1);
		strncpy(TradeTime, r.TradeTime,sizeof(TradeTime)-1);
		TradeType = r.TradeType;
		PriceSource = r.PriceSource;
		strncpy(TraderID, r.TraderID,sizeof(TraderID)-1);
		strncpy(OrderLocalID, r.OrderLocalID,sizeof(OrderLocalID)-1);
		strncpy(ClearingPartID, r.ClearingPartID,sizeof(ClearingPartID)-1);
		strncpy(BusinessUnit, BusinessUnit,sizeof(BusinessUnit)-1);
		SequenceNo = r.SequenceNo;
		strncpy(TradingDay, r.TradingDay,sizeof(TradingDay)-1);
		SettlementID = r.SettlementID;
		BrokerOrderSeq = r.BrokerOrderSeq;
		TradeSource = r.TradeSource;

		strncpy(Account,r.InvestorID,sizeof(Account)-1);

        if(OffsetFlag==THOST_FTDC_OF_Open) CloseTodayVolume=0;
        else if(OffsetFlag==THOST_FTDC_OF_CloseToday) CloseTodayVolume=Volume;
        else CloseTodayVolume=util::GetIntInvalidValue();
	}
	void Clear(void)
	{
		memset(this,0,sizeof(*this));
		TradeCommission=DOUBLEINVALIDVALUE;
        CloseTodayVolume=util::GetIntInvalidValue();
        CloseProfitByDate=DOUBLEINVALIDVALUE;
        CloseProfitByTrade=DOUBLEINVALIDVALUE;
		FTID=0;
		UpdateSeq=0;
	}
    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,%s,%s,%s,Direct%d",
            Account,InstrumentID,TradeDate,TradeID,OrderSysID,Direction);
        prlt[len]=0;
        return prlt;
    }

};

///�ɽ���Ϣ
struct PlatformStru_TradeInfoBrief
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///�ɽ����
	TThostFtdcTradeIDType	TradeID;
	///��������
	TThostFtdcDirectionType	Direction;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///��ƽ��־
	TThostFtdcOffsetFlagType	OffsetFlag;
	///Ͷ���ױ���־
	TThostFtdcHedgeFlagType	HedgeFlag;
	///�۸�
	TThostFtdcPriceType	Price;
	///����
	TThostFtdcVolumeType	Volume;
	///�ɽ�ʱ��
	TThostFtdcDateType	TradeDate;
	///�ɽ�ʱ��
	TThostFtdcTimeType	TradeTime;
	///�ɽ�����
	TThostFtdcTradeTypeType	TradeType;
	///������
	TThostFtdcDateType	TradingDay;
	//�˺�
	CommonAccountType       Account;

	PlatformStru_TradeInfoBrief()
	{
		Clear();
	}
	PlatformStru_TradeInfoBrief(const PlatformStru_TradeInfo& r)
	{
		Clear();

		strncpy(InstrumentID, r.InstrumentID,sizeof(InstrumentID)-1);
        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),r.TradeID);
        Direction = r.Direction;
        strncpy(OrderSysID,r.OrderSysID,sizeof(OrderSysID)-1);
		OffsetFlag = r.OffsetFlag;
		HedgeFlag = r.HedgeFlag;
		Price = r.Price;
		Volume = r.Volume;
		strncpy(TradeDate, r.TradeDate,sizeof(TradeDate)-1);
		strncpy(TradeTime, r.TradeTime,sizeof(TradeTime)-1);
		TradeType = r.TradeType;
		strncpy(TradingDay, r.TradingDay,sizeof(TradingDay)-1);
		strncpy(Account,r.InvestorID,sizeof(Account)-1);
	}
	void Clear(void)
	{
		memset(this,0,sizeof(*this));
	}
    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,%s,%s,%s,Direct%d",
            Account,InstrumentID,TradeDate,TradeID,OrderSysID,Direction);
        prlt[len]=0;
        return prlt;
    }
};


// �ɽ�ͳ�ƽṹ
struct PlatformStru_TradeTotalInfo
{
	//�˺�
	CommonAccountType           Account;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	TThostFtdcDirectionType		Direction;
	///��ƽ��־
	TThostFtdcOffsetFlagType	OffsetFlag;

	double price;
	int volume;
	//Ӷ��
	double TradeCommission;

	//FasttraderID,����ÿһ����¼��Key������һ��Ψһ���
	long FTID;                                                
	//ÿ���������ݷ����仯��UpdateSeq��һ
	long UpdateSeq;

	PlatformStru_TradeTotalInfo()
	{
		Clear();
	}
	void Clear(void)
	{
		memset(this,0,sizeof(*this));
		FTID=0;
		UpdateSeq=0;
	}
	char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,%s,Direct%d,Offset%d",
            Account,ExchangeID,InstrumentID,Direction,OffsetFlag);
        prlt[len]=0;
        return prlt;
    }
};


///������Ϣ
struct PlatformStru_OrderInfo
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	TThostFtdcOrderRefType	OrderRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�����۸�����
	TThostFtdcOrderPriceTypeType	OrderPriceType;
	///��������
	TThostFtdcDirectionType	Direction;
	///��Ͽ�ƽ��־
	TThostFtdcCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TThostFtdcCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	TThostFtdcPriceType	LimitPrice;
	///����
	TThostFtdcVolumeType	VolumeTotalOriginal;
	///��Ч������
	TThostFtdcTimeConditionType	TimeCondition;
	///GTD����
	TThostFtdcDateType	GTDDate;
	///�ɽ�������
	TThostFtdcVolumeConditionType	VolumeCondition;
	///��С�ɽ���
	TThostFtdcVolumeType	MinVolume;
	///��������
	TThostFtdcContingentConditionType	ContingentCondition;
	///ֹ���
	TThostFtdcPriceType	StopPrice;
	///ǿƽԭ��
	TThostFtdcForceCloseReasonType	ForceCloseReason;
	///�Զ������־
	TThostFtdcBoolType	IsAutoSuspend;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
	TThostFtdcRequestIDType	RequestID;
	///���ر������
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	TThostFtdcInstallIDType	InstallID;
	///�����ύ״̬
	TThostFtdcOrderSubmitStatusType	OrderSubmitStatus;
	///������ʾ���
	TThostFtdcSequenceNoType	NotifySequence;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	TThostFtdcSettlementIDType	SettlementID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///������Դ
	TThostFtdcOrderSourceType	OrderSource;
	///����״̬
	TThostFtdcOrderStatusType	OrderStatus;
	///��������
	TThostFtdcOrderTypeType	OrderType;
	///��ɽ�����
	TThostFtdcVolumeType	VolumeTraded;
	///ʣ������
	TThostFtdcVolumeType	VolumeTotal;
	///��������
	TThostFtdcDateType	InsertDate;
	///ί��ʱ��
	TThostFtdcTimeType	InsertTime;
	///����ʱ��
	TThostFtdcTimeType	ActiveTime;
	///����ʱ��
	TThostFtdcTimeType	SuspendTime;
	///����޸�ʱ��
	TThostFtdcTimeType	UpdateTime;
	///����ʱ��
	TThostFtdcTimeType	CancelTime;
	///����޸Ľ���������Ա����
	TThostFtdcTraderIDType	ActiveTraderID;
	///�����Ա���
	TThostFtdcParticipantIDType	ClearingPartID;
	///���
	TThostFtdcSequenceNoType	SequenceNo;
	///ǰ�ñ��
	TThostFtdcFrontIDType	FrontID;
	///�Ự���
	TThostFtdcSessionIDType	SessionID;
	///�û��˲�Ʒ��Ϣ
	TThostFtdcProductInfoType	UserProductInfo;
	///״̬��Ϣ
	TThostFtdcErrorMsgType	StatusMsg;
	///�û�ǿ����־
	TThostFtdcBoolType	UserForceClose;
	///�����û�����
	TThostFtdcUserIDType	ActiveUserID;
	///���͹�˾�������
	TThostFtdcSequenceNoType	BrokerOrderSeq;
	///��ر���
	TThostFtdcOrderSysIDType	RelativeOrderSysID;
	//�ɽ�����
	TThostFtdcPriceType       AvgPrice;
	//�۸�2
	PatsystemAccountType       Price2;

	// ͨ����չ��
	CommonLocalRequestIDType	strLocalRequestID;		// ���ص�������
	CommonAccountType		    Account;				// �����˺�
	CommonExchangeIDType		strExchange;			// ����������

	double         freezeMargin;           //���ᱣ֤��
	double         freezeExchMargin;       //���������ᱣ֤�����ڼ���������֤���Ż�
	double         troubleMoney;           //����������

	double         Leg1FreezeMargin;       //��Ϻ�Լ�����ĵ�һ�ȶ��ᱣ֤��
	double         Leg1FreezeExchMargin;   //��Ϻ�Լ�����ĵ�һ�ȶ��ύ������֤��
	double         Leg1TroubleMoney;       //��Ϻ�Լ�����ĵ�һ�ȶ���������
	double         Leg2FreezeMargin;       //��Ϻ�Լ�����ĵڶ��ȶ��ᱣ֤��
	double         Leg2FreezeExchMargin;   //��Ϻ�Լ�����ĵڶ��ȶ��ύ������֤��
	double         Leg2TroubleMoney;       //��Ϻ�Լ�����ĵڶ��ȶ���������

    double                      PriceTick;              //��InstrumentInfo��ȡ��

	//FasttraderID,����ÿһ����¼��Key������һ��Ψһ���
	long FTID;         
	//ÿ���������ݷ����仯��UpdateSeq��һ
	long UpdateSeq;

	bool bInvalid;      //��ʾ�Ƿ���Ч
	//static PlatformStru_OrderInfo InvalidOrderInfo;


    enum EnumExStatus
    {
        ExSta_none=0,
        ExSta_approving=1       //���������ֹ�����У���ʱOrderStatus����"δ֪"
    };
    EnumExStatus ExStatus;      //�����ʹ�õ�״̬


	PlatformStru_OrderInfo()
	{
		Clear();
		bInvalid=true;
	}
	PlatformStru_OrderInfo(bool binvalid)
	{
		Clear();
		bInvalid=binvalid;
	}
	PlatformStru_OrderInfo(const CThostFtdcOrderField& r)
	{
		Clear();

		strncpy(BrokerID, r.BrokerID,sizeof(BrokerID)-1);
		strncpy(InvestorID, r.InvestorID,sizeof(InvestorID)-1);
		strncpy(InstrumentID, r.InstrumentID,sizeof(InstrumentID)-1);
		strncpy(OrderRef, r.OrderRef,sizeof(OrderRef)-1);
		strncpy(UserID, r.UserID,sizeof(UserID)-1);
		OrderPriceType = r.OrderPriceType;
		Direction = r.Direction;
		CombOffsetFlag[0] = r.CombOffsetFlag[0];
		strncpy(CombHedgeFlag, r.CombHedgeFlag, sizeof(CombHedgeFlag)-1);
		LimitPrice = r.LimitPrice;
		VolumeTotalOriginal = r.VolumeTotalOriginal;
		TimeCondition = r.TimeCondition;
		strncpy(GTDDate, r.GTDDate, sizeof(GTDDate)-1);
		VolumeCondition = r.VolumeCondition;
		MinVolume = r.MinVolume;
		ContingentCondition = r.ContingentCondition;
		StopPrice = r.StopPrice;
		ForceCloseReason = r.ForceCloseReason;
		IsAutoSuspend = r.IsAutoSuspend;
		strncpy(BusinessUnit, r.BusinessUnit, sizeof(BusinessUnit)-1);
		RequestID = r.RequestID;
		strncpy(OrderLocalID, r.OrderLocalID, sizeof(OrderLocalID)-1);
		strncpy(ExchangeID, r.ExchangeID, sizeof(ExchangeID)-1);
		strncpy(ParticipantID, r.ParticipantID, sizeof(ParticipantID)-1);
		strncpy(ClientID, r.ClientID, sizeof(ClientID)-1);
		strncpy(ExchangeInstID, r.ExchangeInstID, sizeof(ExchangeInstID)-1);
		strncpy(TraderID, r.TraderID, sizeof(TraderID)-1);
		InstallID = r.InstallID;
		OrderSubmitStatus = r.OrderSubmitStatus;
		NotifySequence = r.NotifySequence;
		strncpy(TradingDay, r.TradingDay, sizeof(TradingDay)-1);
		SettlementID = r.SettlementID;
        strncpy(OrderSysID,r.OrderSysID,sizeof(OrderSysID)-1);
		OrderSource = r.OrderSource;
		OrderStatus = r.OrderStatus;
		ExStatus = PlatformStru_OrderInfo::ExSta_none;
		OrderType = r.OrderType;
		VolumeTraded = r.VolumeTraded;
		VolumeTotal = r.VolumeTotal;
		strncpy(InsertDate, r.InsertDate, sizeof(InsertDate)-1);
		strncpy(InsertTime, r.InsertTime, sizeof(InsertTime)-1);
		strncpy(ActiveTime, r.ActiveTime, sizeof(ActiveTime)-1);
		strncpy(SuspendTime, r.SuspendTime, sizeof(SuspendTime)-1);
		strncpy(UpdateTime, r.UpdateTime, sizeof(UpdateTime)-1);
		strncpy(CancelTime, r.CancelTime, sizeof(CancelTime)-1);
		strncpy(ActiveTraderID, r.ActiveTraderID, sizeof(ActiveTraderID)-1);
		strncpy(ClearingPartID, r.ClearingPartID, sizeof(ClearingPartID)-1);
		SequenceNo = r.SequenceNo;
		FrontID = r.FrontID;
		SessionID = r.SessionID;
		strncpy(UserProductInfo, r.UserProductInfo, sizeof(UserProductInfo)-1);
		strncpy(StatusMsg, r.StatusMsg, sizeof(StatusMsg)-1);
		UserForceClose = r.UserForceClose;
		strncpy(ActiveUserID, r.ActiveUserID, sizeof(ActiveUserID)-1);
		BrokerOrderSeq = r.BrokerOrderSeq;
		strncpy(RelativeOrderSysID, r.RelativeOrderSysID, sizeof(RelativeOrderSysID)-1);

		strncpy(Account,r.InvestorID,sizeof(Account)-1);
        bInvalid=false;
	}
	void Clear(void)
	{
		memset(this,0,sizeof(*this));
		freezeMargin=DOUBLEINVALIDVALUE;
		freezeExchMargin=DOUBLEINVALIDVALUE;
		troubleMoney=DOUBLEINVALIDVALUE;
        Leg1FreezeMargin=DOUBLEINVALIDVALUE;
        Leg1FreezeExchMargin=DOUBLEINVALIDVALUE;
        Leg1TroubleMoney=DOUBLEINVALIDVALUE;
        Leg2FreezeMargin=DOUBLEINVALIDVALUE;
        Leg2FreezeExchMargin=DOUBLEINVALIDVALUE;
        Leg2TroubleMoney=DOUBLEINVALIDVALUE;

		FTID=0;
		UpdateSeq=0;
		bInvalid=true;
	}
	bool IsInvalidRecord(void) const
	{
		return bInvalid;
	}

	char* tostring(char*prlt,int rltsize) const
    {
        if(bInvalid&&prlt&&rltsize>0) 
        {
            strncpy(prlt,"Invalid",rltsize);
            prlt[rltsize-1]=0;
            return prlt;
        }

        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,FrontID%d,SessionID%d,%s",
            Account,InstrumentID,FrontID,SessionID,OrderRef);
        prlt[len]=0;
        return prlt;
    }
};

///������Ϣ
struct PlatformStru_DepthMarketData
{
    PlatformStru_DepthMarketData()
    {
        clear();
    }
	PlatformStru_DepthMarketData(const CThostFtdcDepthMarketDataField& r)
	{
		clear();

		strncpy(TradingDay, r.TradingDay,sizeof(TradingDay)-1);
		strncpy(InstrumentID, r.InstrumentID,sizeof(InstrumentID)-1);
		strncpy(ExchangeID, r.ExchangeID,sizeof(ExchangeID)-1);
		strncpy(ExchangeInstID, r.ExchangeInstID,sizeof(ExchangeInstID)-1);
		LastPrice = r.LastPrice;
		PreSettlementPrice = r.PreSettlementPrice;
		PreClosePrice = r.PreClosePrice;
		PreOpenInterest = r.PreOpenInterest;
		OpenPrice = r.OpenPrice;
		HighestPrice = r.HighestPrice;
		LowestPrice = r.LowestPrice;
		Volume = r.Volume;
		Turnover = r.Turnover;
		OpenInterest = r.OpenInterest;
		ClosePrice = r.ClosePrice;
		SettlementPrice = r.SettlementPrice;
		UpperLimitPrice = r.UpperLimitPrice;
		LowerLimitPrice = r.LowerLimitPrice;
		PreDelta = r.PreDelta;
		CurrDelta = r.CurrDelta;
		strcpy(UpdateTime, r.UpdateTime);
		UpdateMillisec = r.UpdateMillisec;
		BidPrice1 = r.BidPrice1;
		BidVolume1 = r.BidVolume1;
		AskPrice1 = r.AskPrice1;
		AskVolume1 = r.AskVolume1;
		BidPrice2 = r.BidPrice2;
		BidVolume2 = r.BidVolume2;
		AskPrice2 = r.AskPrice2;
		AskVolume2 = r.AskVolume2;
		BidPrice3 = r.BidPrice3;
		BidVolume3 = r.BidVolume3;
		AskPrice3 = r.AskPrice3;
		AskVolume3 = r.AskVolume3;
		BidPrice4 = r.BidPrice4;
		BidVolume4 = r.BidVolume4;
		AskPrice4 = r.AskPrice4;
		AskVolume4 = r.AskVolume4;
		BidPrice5 = r.BidPrice5;
		BidVolume5 = r.BidVolume5;
		AskPrice5 = r.AskPrice5;
		AskVolume5 = r.AskVolume5;
		AveragePrice = r.AveragePrice;
	}
    void clear(void)
    {
        memset(this,0,sizeof(*this));

        LastPrice=util::GetDoubleInvalidValue();
        PreSettlementPrice=util::GetDoubleInvalidValue();
        PreClosePrice=util::GetDoubleInvalidValue();
        PreOpenInterest=util::GetDoubleInvalidValue();
        OpenPrice=util::GetDoubleInvalidValue();
        HighestPrice=util::GetDoubleInvalidValue();
        LowestPrice=util::GetDoubleInvalidValue();
        Volume=util::GetIntInvalidValue();
        Turnover=util::GetDoubleInvalidValue();
        OpenInterest=util::GetDoubleInvalidValue();
        ClosePrice=util::GetDoubleInvalidValue();
        SettlementPrice=util::GetDoubleInvalidValue();
        UpperLimitPrice=util::GetDoubleInvalidValue();
        LowerLimitPrice=util::GetDoubleInvalidValue();
        PreDelta=util::GetDoubleInvalidValue();
        CurrDelta=util::GetDoubleInvalidValue();
        BidPrice1=util::GetDoubleInvalidValue();
        BidVolume1=util::GetIntInvalidValue();
        AskPrice1=util::GetDoubleInvalidValue();
        AskVolume1=util::GetIntInvalidValue();
        BidPrice2=util::GetDoubleInvalidValue();
        BidVolume2=util::GetIntInvalidValue();
        AskPrice2=util::GetDoubleInvalidValue();
        AskVolume2=util::GetIntInvalidValue();
        BidPrice3=util::GetDoubleInvalidValue();
        BidVolume3=util::GetIntInvalidValue();
        AskPrice3=util::GetDoubleInvalidValue();
        AskVolume3=util::GetIntInvalidValue();
        BidPrice4=util::GetDoubleInvalidValue();
        BidVolume4=util::GetIntInvalidValue();
        AskPrice4=util::GetDoubleInvalidValue();
        AskVolume4=util::GetIntInvalidValue();
        BidPrice5=util::GetDoubleInvalidValue();
        BidVolume5=util::GetIntInvalidValue();
        AskPrice5=util::GetDoubleInvalidValue();
        AskVolume5=util::GetIntInvalidValue();
        AveragePrice=util::GetDoubleInvalidValue();
        BidPrice6=util::GetDoubleInvalidValue();
        BidVolume6=util::GetIntInvalidValue();
        AskPrice6=util::GetDoubleInvalidValue();
        AskVolume6=util::GetIntInvalidValue();
        BidPrice7=util::GetDoubleInvalidValue();
        BidVolume7=util::GetIntInvalidValue();
        AskPrice7=util::GetDoubleInvalidValue();
        AskVolume7=util::GetIntInvalidValue();
        BidPrice8=util::GetDoubleInvalidValue();
        BidVolume8=util::GetIntInvalidValue();
        AskPrice8=util::GetDoubleInvalidValue();
        AskVolume8=util::GetIntInvalidValue();
        BidPrice9=util::GetDoubleInvalidValue();
        BidVolume9=util::GetIntInvalidValue();
        AskPrice9=util::GetDoubleInvalidValue();
        AskVolume9=util::GetIntInvalidValue();
        BidPrice10=util::GetDoubleInvalidValue();
        BidVolume10=util::GetIntInvalidValue();
        AskPrice10=util::GetDoubleInvalidValue();
        AskVolume10=util::GetIntInvalidValue();
        NewVolume=util::GetIntInvalidValue();
    }

	///������
	TThostFtdcDateType	TradingDay;
	///��Լ����
	InstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///���¼�
	TThostFtdcPriceType	LastPrice;
	///�ϴν����
	TThostFtdcPriceType	PreSettlementPrice;
	///������
	TThostFtdcPriceType	PreClosePrice;
	///��ֲ���
	TThostFtdcLargeVolumeType	PreOpenInterest;
	///����
	TThostFtdcPriceType	OpenPrice;
	///��߼�
	TThostFtdcPriceType	HighestPrice;
	///��ͼ�
	TThostFtdcPriceType	LowestPrice;
	///����
	TThostFtdcVolumeType	Volume;
	///�ɽ����
	TThostFtdcMoneyType	Turnover;
	///�ֲ���
	TThostFtdcLargeVolumeType	OpenInterest;
	///������
	TThostFtdcPriceType	ClosePrice;
	///���ν����
	TThostFtdcPriceType	SettlementPrice;
	///��ͣ���
	TThostFtdcPriceType	UpperLimitPrice;
	///��ͣ���
	TThostFtdcPriceType	LowerLimitPrice;
	///����ʵ��
	TThostFtdcRatioType	PreDelta;
	///����ʵ��
	TThostFtdcRatioType	CurrDelta;
	///����޸�ʱ��
	TThostFtdcTimeType	UpdateTime;
	///����޸ĺ���
	TThostFtdcMillisecType	UpdateMillisec;
	///�����һ
	TThostFtdcPriceType	BidPrice1;
	///������һ
	TThostFtdcVolumeType	BidVolume1;
	///������һ
	TThostFtdcPriceType	AskPrice1;
	///������һ
	TThostFtdcVolumeType	AskVolume1;
	///����۶�
	TThostFtdcPriceType	BidPrice2;
	///��������
	TThostFtdcVolumeType	BidVolume2;
	///�����۶�
	TThostFtdcPriceType	AskPrice2;
	///��������
	TThostFtdcVolumeType	AskVolume2;
	///�������
	TThostFtdcPriceType	BidPrice3;
	///��������
	TThostFtdcVolumeType	BidVolume3;
	///��������
	TThostFtdcPriceType	AskPrice3;
	///��������
	TThostFtdcVolumeType	AskVolume3;
	///�������
	TThostFtdcPriceType	BidPrice4;
	///��������
	TThostFtdcVolumeType	BidVolume4;
	///��������
	TThostFtdcPriceType	AskPrice4;
	///��������
	TThostFtdcVolumeType	AskVolume4;
	///�������
	TThostFtdcPriceType	BidPrice5;
	///��������
	TThostFtdcVolumeType	BidVolume5;
	///��������
	TThostFtdcPriceType	AskPrice5;
	///��������
	TThostFtdcVolumeType	AskVolume5;
	///���վ���
	TThostFtdcPriceType	AveragePrice;

	///�������
	TThostFtdcPriceType	BidPrice6;
	///��������
	TThostFtdcVolumeType	BidVolume6;
	///��������
	TThostFtdcPriceType	AskPrice6;
	///��������
	TThostFtdcVolumeType	AskVolume6;
	///�������
	TThostFtdcPriceType	BidPrice7;
	///��������
	TThostFtdcVolumeType	BidVolume7;
	///��������
	TThostFtdcPriceType	AskPrice7;
	///��������
	TThostFtdcVolumeType	AskVolume7;
	///����۰�
	TThostFtdcPriceType	BidPrice8;
	///��������
	TThostFtdcVolumeType	BidVolume8;
	///�����۰�
	TThostFtdcPriceType	AskPrice8;
	///��������
	TThostFtdcVolumeType	AskVolume8;
	///����۾�
	TThostFtdcPriceType	BidPrice9;
	///��������
	TThostFtdcVolumeType	BidVolume9;
	///�����۾�
	TThostFtdcPriceType	AskPrice9;
	///��������
	TThostFtdcVolumeType	AskVolume9;
	///�����ʮ
	TThostFtdcPriceType	BidPrice10;
	///������ʮ
	TThostFtdcVolumeType	BidVolume10;
	///������ʮ
	TThostFtdcPriceType	AskPrice10;
	///������ʮ
	TThostFtdcVolumeType	AskVolume10;
	//������
	TThostFtdcVolumeType   NewVolume;
};

struct PlatformStru_UserPasswordUpdate
{
    PlatformStru_UserPasswordUpdate()
    {
        memset(this,0,sizeof(*this));
    }; 

	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///ԭ���Ŀ���
	TThostFtdcPasswordType	OldPassword;
	///�µĿ���
	TThostFtdcPasswordType	NewPassword;
};
struct PlatformStru_TradingAccountPasswordUpdate
{
    PlatformStru_TradingAccountPasswordUpdate()
    {
        memset(this,0,sizeof(*this));
    };

	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///ԭ���Ŀ���
	TThostFtdcPasswordType	OldPassword;
	///�µĿ���
	TThostFtdcPasswordType	NewPassword;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
};
///¼�뱨����Ϣ
struct PlatformStru_InputOrder
{
    //PlatformStru_InputOrder() { memset(this,0,sizeof(*this)); }

	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	InstrumentIDType	InstrumentID;
	///��������
	TThostFtdcOrderRefType	OrderRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�����۸�����
	TThostFtdcOrderPriceTypeType	OrderPriceType;
	///��������
	TThostFtdcDirectionType	Direction;
	///��Ͽ�ƽ��־
	TThostFtdcCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TThostFtdcCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	TThostFtdcPriceType	LimitPrice;
	///����
	TThostFtdcVolumeType	VolumeTotalOriginal;
	///��Ч������
	TThostFtdcTimeConditionType	TimeCondition;
	///GTD����
	TThostFtdcDateType	GTDDate;
	///�ɽ�������
	TThostFtdcVolumeConditionType	VolumeCondition;
	///��С�ɽ���
	TThostFtdcVolumeType	MinVolume;
	///��������
	TThostFtdcContingentConditionType	ContingentCondition;
	///ֹ���
	TThostFtdcPriceType	StopPrice;
	///ǿƽԭ��
	TThostFtdcForceCloseReasonType	ForceCloseReason;
	///�Զ������־
	TThostFtdcBoolType	IsAutoSuspend;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
	TThostFtdcRequestIDType	RequestID;
	///�û�ǿ����־
	TThostFtdcBoolType	UserForceClose;

	// ͨ����չ��
	CommonOrderClassType		OrderClass;					// ���ݷ��ࣺ'\000'/'N' New order, 'R' Replace

	CommonAccountType		    strAccount;					// �����˺�
	CommonLocalRequestIDType	strLocalRequestID;			// ���ص�������
	CommonLocalRefIDType		strLocalRefID;				// ���ص����úű��
	CommonExchangeIDType		strExchange;				// ����������
};

///¼��Ԥ����Ϣ
struct PlatformStru_ParkedOrder
{
    //PlatformStru_ParkedOrder() { memset(this,0,sizeof(*this)); }

	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	TThostFtdcOrderRefType	OrderRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///�����۸�����
	TThostFtdcOrderPriceTypeType	OrderPriceType;
	///��������
	TThostFtdcDirectionType	Direction;
	///��Ͽ�ƽ��־
	TThostFtdcCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TThostFtdcCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	TThostFtdcPriceType	LimitPrice;
	///����
	TThostFtdcVolumeType	VolumeTotalOriginal;
	///��Ч������
	TThostFtdcTimeConditionType	TimeCondition;
	///GTD����
	TThostFtdcDateType	GTDDate;
	///�ɽ�������
	TThostFtdcVolumeConditionType	VolumeCondition;
	///��С�ɽ���
	TThostFtdcVolumeType	MinVolume;
	///��������
	TThostFtdcContingentConditionType	ContingentCondition;
	///ֹ���
	TThostFtdcPriceType	StopPrice;
	///ǿƽԭ��
	TThostFtdcForceCloseReasonType	ForceCloseReason;
	///�Զ������־
	TThostFtdcBoolType	IsAutoSuspend;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
	TThostFtdcRequestIDType	RequestID;
	///�û�ǿ����־
	TThostFtdcBoolType	UserForceClose;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///Ԥ�񱨵����
	TThostFtdcParkedOrderIDType	ParkedOrderID;
	///�û�����
	TThostFtdcUserTypeType	UserType;
	///Ԥ��״̬
	TThostFtdcParkedOrderStatusType	Status;
	///�������
	TThostFtdcErrorIDType	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;

	// ͨ����չ��
	CommonOrderClassType		OrderClass;					// ���ݷ��ࣺ'\000'/'N' New order, 'R' Replace

	CommonAccountType		strAccount;					// �����˺�
};

struct PlatformStru_ParkedOrderAction
{
	CThostFtdcParkedOrderActionField Thost;
};
struct PlatformStru_InputOrderAction
{
    PlatformStru_InputOrderAction()
    {
        memset(this,0,sizeof(*this));
    }

	CThostFtdcInputOrderActionField Thost;
};
struct PlatformStru_QueryMaxOrderVolume
{
    PlatformStru_QueryMaxOrderVolume()
    {
        memset(this,0,sizeof(*this));
    };

	CThostFtdcQueryMaxOrderVolumeField Thost;
};
struct PlatformStru_RemoveParkedOrder
{
    PlatformStru_RemoveParkedOrder()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcRemoveParkedOrderField Thost;
};
struct PlatformStru_RemoveParkedOrderAction
{
    PlatformStru_RemoveParkedOrderAction()
    {
        memset(this,0,sizeof(*this));
    };

	CThostFtdcRemoveParkedOrderActionField Thost;
};
struct PlatformStru_QryOrder
{
    PlatformStru_QryOrder()
    {
        memset(this,0,sizeof(*this));
    }
	CThostFtdcQryOrderField Thost;
};
struct PlatformStru_QryTrade
{
	PlatformStru_QryTrade()
	{
		memset(this,0,sizeof(*this));
	};
	CThostFtdcQryTradeField Thost;
};
///�����ѯͶ���ֲ߳�
struct PlatformStru_QryInvestorPosition
{
	PlatformStru_QryInvestorPosition()
	{
		memset(this,0,sizeof(*this));
	}

	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};


//�ֲ���ϸ
struct PlatformStru_PositionDetail
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///Ͷ���ױ���־
	TThostFtdcHedgeFlagType	HedgeFlag;
	///����
	TThostFtdcDirectionType	Direction;
	///��������
	TThostFtdcDateType	OpenDate;
	///�ɽ����
	TThostFtdcTradeIDType	TradeID;
	///���� ���̾���λ
	TThostFtdcVolumeType	Volume;
	///���ּ�
	TThostFtdcPriceType	OpenPrice;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	TThostFtdcSettlementIDType	SettlementID;
	///�ɽ�����
	TThostFtdcTradeTypeType	TradeType;
	///��Ϻ�Լ����
	TThostFtdcInstrumentIDType	CombInstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///���ն���ƽ��ӯ��
	TThostFtdcMoneyType	CloseProfitByDate;
	///��ʶԳ�ƽ��ӯ��
	TThostFtdcMoneyType	CloseProfitByTrade;
	///���ն��гֲ�ӯ��
	TThostFtdcMoneyType	PositionProfitByDate;
	///��ʶԳ�ֲ�ӯ��
	TThostFtdcMoneyType	PositionProfitByTrade;
	///Ͷ���߱�֤��
	TThostFtdcMoneyType	Margin;
	///��������֤��
	TThostFtdcMoneyType	ExchMargin;
	///��֤����
	TThostFtdcRatioType	MarginRateByMoney;
	///��֤����(������)
	TThostFtdcRatioType	MarginRateByVolume;
	///������
	TThostFtdcPriceType	LastSettlementPrice;
	///�����
	TThostFtdcPriceType	SettlementPrice;
	///ƽ����
	TThostFtdcVolumeType	CloseVolume;
	///ƽ�ֽ��
	TThostFtdcMoneyType	CloseAmount;



	//����
	int nBuyVolumn;
	//����
	int nSellVolumn;
	//���ֲ־���
	double dOpenAvgPrice;
	//���ֲ�����
	double dOpenProfit;
	//������
	double dTotalProfit;
	//��֤��ÿ����
	double dMarginPerLot;
	//�ʽ�������
	double dPLBurnRate;
	//δƽ�ֺ�Լ����
	double dOpenPositionExposure;
	//ʣ�๺����
	double dBuyPowerRemain;
	//�����ʽ�
	double dBuyPowerUsed;
	//������֤��
	double dTotalMarginPaid;
	//�˺�
	CommonAccountType   Account;

	double      CurPrice;                   //�ּۣ����ڼ���ӯ��������ʹ�õļ۸�˳��Ϊ������� > ���¼� > ������

    double              PriceTick;          //��InstrumentInfo��ȡ��
    bool                bToday;             //�Ƿ���ճֲ���ϸ
    TThostFtdcTimeType	OpenTime;           //����ʱ�䡣���Ϊ00:00:00�����Ϊ��Ӧ�Ŀ��ֳɽ�ʱ��
    int                 OpenTimeUTC;        //����ʱ�䣬utc���������Ϊ��������ʱ��ʱ�䣬���Ϊ��ȷʱ��

	//FasttraderID,����ÿһ����¼��Key������һ��Ψһ���
	long FTID;                                                
	//ÿ���������ݷ����仯��UpdateSeq��һ
	long UpdateSeq;



	bool bInvalid;      //��ʾ�Ƿ���Ч

	PlatformStru_PositionDetail()
	{
		Clear();
	}
	PlatformStru_PositionDetail(bool binvalid)
	{
		Clear();
		bInvalid=binvalid;
	}
	PlatformStru_PositionDetail(const CThostFtdcInvestorPositionDetailField& r)
	{
		Clear();

		strncpy(Account,r.InvestorID,sizeof(Account)-1);
		strncpy(InstrumentID, r.InstrumentID, sizeof(InstrumentID)-1);
		strncpy(BrokerID, r.BrokerID, sizeof(BrokerID)-1);
		strncpy(InvestorID, r.InvestorID, sizeof(InvestorID)-1);

        HedgeFlag = r.HedgeFlag;
		Direction = r.Direction;
        strncpy(OpenDate, r.OpenDate, sizeof(OpenDate)-1);

        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),r.TradeID);

        Volume = r.Volume;
		OpenPrice = r.OpenPrice;
		strncpy(TradingDay, r.TradingDay, sizeof(TradingDay)-1);
		SettlementID = r.SettlementID;
		TradeType = r.TradeType;
		strncpy(CombInstrumentID, r.CombInstrumentID, sizeof(CombInstrumentID)-1);
		strncpy(ExchangeID, r.ExchangeID, sizeof(ExchangeID)-1);

		CloseProfitByDate = r.CloseProfitByDate;
		CloseProfitByTrade = r.CloseProfitByTrade;
		PositionProfitByDate = r.PositionProfitByDate;
		PositionProfitByTrade = r.PositionProfitByTrade;

		Margin = r.Margin;
		ExchMargin = r.ExchMargin;
		MarginRateByMoney = r.MarginRateByMoney;
		MarginRateByVolume = r.MarginRateByVolume;

		LastSettlementPrice = r.LastSettlementPrice;
		SettlementPrice = r.SettlementPrice;
		CloseVolume = r.CloseVolume;
		CloseAmount = r.CloseAmount;

		bInvalid=false;
	}
    //���ֳɽ������ֲ���ϸ
    PlatformStru_PositionDetail(const PlatformStru_TradeInfo& TradeInfo)
    {
        Clear();

	    strncpy(Account,TradeInfo.Account,sizeof(Account)-1);
        strncpy(InstrumentID,TradeInfo.InstrumentID,sizeof(InstrumentID)-1);
        strncpy(BrokerID,TradeInfo.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,TradeInfo.InvestorID,sizeof(InvestorID)-1);

        HedgeFlag=TradeInfo.HedgeFlag;
        Direction=TradeInfo.Direction;
        strncpy(OpenDate,TradeInfo.TradeDate,sizeof(OpenDate)-1);
        strncpy(TradeID,TradeInfo.TradeID,sizeof(TradeID)-1);

        Volume=TradeInfo.Volume;
        OpenPrice=TradeInfo.Price;
        strncpy(TradingDay,TradeInfo.TradingDay,sizeof(TradingDay)-1);
        SettlementID=TradeInfo.SettlementID;
        TradeType=TradeInfo.TradeType;
        strncpy(ExchangeID,TradeInfo.ExchangeID,sizeof(ExchangeID)-1);

        CloseProfitByDate=0;
        CloseProfitByTrade=0;
        PositionProfitByDate=0;
        PositionProfitByTrade=0;

        Margin=util::GetDoubleInvalidValue();
        MarginRateByMoney=util::GetDoubleInvalidValue();
        MarginRateByVolume=util::GetDoubleInvalidValue();
        ExchMargin=util::GetDoubleInvalidValue();

        LastSettlementPrice=util::GetDoubleInvalidValue();
        SettlementPrice=util::GetDoubleInvalidValue();
        CloseVolume=0;
        CloseAmount=0;

        CurPrice=util::GetDoubleInvalidValue();
        PriceTick=TradeInfo.PriceTick;
        bToday=true;
        strncpy(OpenTime,TradeInfo.TradeTime,sizeof(OpenTime)-1);
        OpenTimeUTC=(int)CTools_Ansi::Get_time_t_utc_from_strTime(OpenDate,OpenTime);

        bInvalid=false;
    }
	void Clear()
	{
        memset(this,0,sizeof(*this));
		int nInvalidInt = INTINVALIDVALUE;
		double dInvalidDouble = DOUBLEINVALIDVALUE;
		Volume = nInvalidInt;
		OpenPrice = dInvalidDouble;
		SettlementID = nInvalidInt;
		CloseProfitByDate = dInvalidDouble;
		CloseProfitByTrade = dInvalidDouble;
		PositionProfitByDate = dInvalidDouble;
		PositionProfitByTrade = dInvalidDouble;
		Margin = dInvalidDouble;
		ExchMargin = dInvalidDouble;
		MarginRateByMoney = dInvalidDouble;
		MarginRateByVolume = dInvalidDouble;
		LastSettlementPrice = dInvalidDouble;
		SettlementPrice = dInvalidDouble;
		CloseVolume = nInvalidInt;
		CloseAmount = nInvalidInt;

		nBuyVolumn = nInvalidInt;
		nSellVolumn = nInvalidInt;
		dOpenAvgPrice = dInvalidDouble;
		dOpenProfit = dInvalidDouble;
		dTotalProfit = dInvalidDouble;
		dMarginPerLot = dInvalidDouble;
		dPLBurnRate = dInvalidDouble;
		dOpenPositionExposure = dInvalidDouble;
		dBuyPowerRemain = dInvalidDouble;
		dBuyPowerUsed = dInvalidDouble;
		dTotalMarginPaid = dInvalidDouble;
		CurPrice = dInvalidDouble;
		FTID=0;
		UpdateSeq=0;
		bInvalid=false;
	}
	bool IsInvalidRecord(void) const
	{
		return bInvalid;
	}
	char* tostring(char*prlt,int rltsize) const
    {
        if(bInvalid&&prlt&&rltsize>0) 
        {
            strncpy(prlt,"Invalid",rltsize);
            prlt[rltsize-1]=0;
            return prlt;
        }

        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,%s,%s,Direct%d,Hedge%d,TradeType%d,Volume=%d",
            Account,InstrumentID,OpenDate,TradeID,Direction,HedgeFlag,TradeType,Volume);
        prlt[len]=0;
        return prlt;
    }
};

//��ϳֲ���ϸ
struct PlatformStru_PositionDetailComb
{
	///������
	TThostFtdcDateType	TradingDay;
	///��������
	TThostFtdcDateType	OpenDate;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///������
	TThostFtdcSettlementIDType	SettlementID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��ϱ��
	TThostFtdcTradeIDType	ComTradeID;
	///��ϱ��
	TThostFtdcTradeIDType	TradeID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	TThostFtdcHedgeFlagType	HedgeFlag;
	///����
	TThostFtdcDirectionType	Direction;
	///�ֲ���
	TThostFtdcVolumeType	TotalAmt;
	///Ͷ���߱�֤��
	TThostFtdcMoneyType	Margin;
	///��������֤��
	TThostFtdcMoneyType	ExchMargin;
	///��֤����
	TThostFtdcRatioType	MarginRateByMoney;
	///��֤����(������)
	TThostFtdcRatioType	MarginRateByVolume;
    ///���ȱ��(����ֵ��0/1)
	TThostFtdcLegIDType	LegID;
    ///���ȳ���(1)
	TThostFtdcLegMultipleType	LegMultiple;
	///��ϳֲֺ�Լ����
	TThostFtdcInstrumentIDType	CombInstrumentID;

	CommonAccountType   Account;	//��Ӧ�˺�
    bool                bToday;             //�Ƿ������ϳֲ���ϸ

	//FasttraderID,����ÿһ����¼��Key������һ��Ψһ���
	long FTID;                                                
	//ÿ���������ݷ����仯��UpdateSeq��һ
	long UpdateSeq;


	bool bInvalid;      //��ʾ�Ƿ���Ч
	//static PlatformStru_PositionDetailComb InvalidPositionDetailComb;

	PlatformStru_PositionDetailComb()
	{
		Clear();
	}
	PlatformStru_PositionDetailComb(bool binvalid)
	{
		Clear();
		bInvalid=binvalid;
	}
    //��һ�����ȳɽ�������һ����ϳɽ���ϸ
    PlatformStru_PositionDetailComb(const PlatformStru_TradeInfo &TradeInfo,bool btoday,const std::string& comTradeID,const std::string& strCombInstrumentID,int legID)
	{
		Clear();

	    strncpy(Account, TradeInfo.Account, sizeof(Account)-1);
	    strncpy(InstrumentID, TradeInfo.InstrumentID, sizeof(InstrumentID)-1);

	    strncpy(TradingDay, TradeInfo.TradingDay, sizeof(TradingDay)-1);
	    strncpy(OpenDate, TradeInfo.TradingDay, sizeof(OpenDate)-1);
	    strncpy(ExchangeID, TradeInfo.ExchangeID, sizeof(ExchangeID)-1);
	    SettlementID = TradeInfo.SettlementID;
	    strncpy(BrokerID, TradeInfo.BrokerID, sizeof(BrokerID)-1);
	    strncpy(InvestorID, TradeInfo.InvestorID, sizeof(InvestorID)-1);

        strncpy(ComTradeID, comTradeID.c_str(), sizeof(ComTradeID)-1);
        strncpy(TradeID, TradeInfo.TradeID, sizeof(TradeID)-1);

        HedgeFlag = TradeInfo.HedgeFlag;
        Direction = TradeInfo.Direction;
        TotalAmt = TradeInfo.Volume;

        Margin = util::GetDoubleInvalidValue();
        ExchMargin = util::GetDoubleInvalidValue();
        MarginRateByMoney = util::GetDoubleInvalidValue();
        MarginRateByVolume = util::GetDoubleInvalidValue();

        LegID = legID;
        LegMultiple = 1;
        strncpy(CombInstrumentID, strCombInstrumentID.c_str(), sizeof(CombInstrumentID)-1);

        bToday=btoday;

        bInvalid=false;
	}
	PlatformStru_PositionDetailComb(const CThostFtdcInvestorPositionCombineDetailField& r)
	{
		Clear();

		strncpy(TradingDay,r.TradingDay,sizeof(TradingDay)-1);
		strncpy(OpenDate,r.OpenDate,sizeof(OpenDate)-1);
		strncpy(ExchangeID,r.ExchangeID,sizeof(ExchangeID)-1);
		SettlementID=r.SettlementID;
		strncpy(BrokerID,r.BrokerID,sizeof(BrokerID)-1);
		strncpy(InvestorID,r.InvestorID,sizeof(InvestorID)-1);

        CTools_Ansi::mymemcpy_trim(ComTradeID,sizeof(ComTradeID),r.ComTradeID);
        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),r.TradeID);

        strncpy(InstrumentID,r.InstrumentID,sizeof(InstrumentID)-1);
		HedgeFlag=r.HedgeFlag;
		Direction=r.Direction;
		TotalAmt=r.TotalAmt;
		Margin=r.Margin;
		ExchMargin=r.ExchMargin;
		MarginRateByMoney=r.MarginRateByMoney;
		MarginRateByVolume=r.MarginRateByVolume;
		LegID=r.LegID;
		LegMultiple=r.LegMultiple;
		strncpy(CombInstrumentID,r.CombInstrumentID,sizeof(CombInstrumentID)-1);

		strncpy(Account,r.InvestorID,sizeof(Account)-1);
		bInvalid=false;
	}

	void Clear()
	{
		memset(this, 0,sizeof(*this));
		int nInvalidInt = INTINVALIDVALUE;
		double dInvalidDouble = DOUBLEINVALIDVALUE;
		
        TotalAmt = nInvalidInt;
		Margin = dInvalidDouble;
		ExchMargin = dInvalidDouble;
		MarginRateByMoney = dInvalidDouble;
		MarginRateByVolume = dInvalidDouble;
        LegID = nInvalidInt;
		LegMultiple = nInvalidInt;

        bToday=false;

		FTID=0;
		UpdateSeq=0;
		bInvalid=false;
	}
	bool IsInvalidRecord(void) const
	{
		return bInvalid;
	}
	char* tostring(char*prlt,int rltsize) const
    {
        if(bInvalid&&prlt&&rltsize>0) 
        {
            strncpy(prlt,"Invalid",rltsize);
            prlt[rltsize-1]=0;
            return prlt;
        }

        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,%s,%s,Direct%d,Hedge%d,TotalAmt=%d",
            Account,InstrumentID,OpenDate,TradeID,Direction,HedgeFlag,TotalAmt);
        prlt[len]=0;
        return prlt;
    }
};


//�ֲ�
struct PlatformStru_Position
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///�ֲֶ�շ���
	TThostFtdcPosiDirectionType	PosiDirection;
	///Ͷ���ױ���־
	TThostFtdcHedgeFlagType	HedgeFlag;
	///�ֲ���������
	TThostFtdcPositionDateType	PositionDate;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	TThostFtdcSettlementIDType	SettlementID;

	///��ǰ���ֲ�
	TThostFtdcVolumeType	Position;
	///���ճֲ�(ָ���վ��ֲ�)
	TThostFtdcVolumeType	TodayPosition;
	///���ճֲ�(ָ��ʷ�ֲ��ܺͣ���������ƽ���Ĳ���)
	TThostFtdcVolumeType	YdPosition;

	///������(ָ���տ�����)
	TThostFtdcVolumeType	OpenVolume;
	///ƽ����(ָ����ƽ����)
	TThostFtdcVolumeType	CloseVolume;
	///���ֽ��(ָ�񿪲ֽ��)
	TThostFtdcMoneyType	OpenAmount;
	///ƽ�ֽ��(ָ��ƽ�ֽ��)
	TThostFtdcMoneyType	CloseAmount;
	///�ֲֳɱ�(��ǰ���ֲֵĳֲֳɱ�)
	TThostFtdcMoneyType	PositionCost;
	///���ֳɱ�(δƽ�ֲ���ϸ�Ŀ��ֽ����ܺ�(ֻ����δƽ����))
	TThostFtdcMoneyType	OpenCost;

	///��ͷ����(��ͷ�ֲ�: ���򶳽᣻��ͷ�ֲ֣�ƽ�򶳽�)
	TThostFtdcVolumeType	LongFrozen;
	///��ͷ����(��ͷ�ֲ�: ƽ�����᣻��ͷ�ֲ֣���������)
	TThostFtdcVolumeType	ShortFrozen;
	///��ͷ������
	TThostFtdcMoneyType	LongFrozenAmount;
	///��ͷ������
	TThostFtdcMoneyType	ShortFrozenAmount;
	///����ı�֤��
	TThostFtdcMoneyType	FrozenMargin;
	///�����������
	TThostFtdcMoneyType	FrozenCommission;
	///������ʽ�
	TThostFtdcMoneyType	FrozenCash;

	///������
	TThostFtdcMoneyType	Commission;
	///�ϴ�ռ�õı�֤��
	TThostFtdcMoneyType	PreMargin;
	///ռ�õı�֤��
	TThostFtdcMoneyType	UseMargin;
	///��������֤��
	TThostFtdcMoneyType	ExchangeMargin;
	///��֤����
	TThostFtdcRatioType	MarginRateByMoney;
	///��֤����(������)
	TThostFtdcRatioType	MarginRateByVolume;

	///�ʽ���
	TThostFtdcMoneyType	CashIn;

	///�ֲ�ӯ��(����)
	TThostFtdcMoneyType	PositionProfit;
	///ƽ��ӯ��(����)
	TThostFtdcMoneyType	CloseProfit;
	///���ն���ƽ��ӯ��
	TThostFtdcMoneyType	CloseProfitByDate;
	///��ʶԳ�ƽ��ӯ��
	TThostFtdcMoneyType	CloseProfitByTrade;

	///�ϴν����
	TThostFtdcPriceType	PreSettlementPrice;
	///���ν����
	TThostFtdcPriceType	SettlementPrice;

	///��ϳɽ��γɵĳֲ�
	TThostFtdcVolumeType	    CombPosition;
	///��϶�ͷ����
	TThostFtdcVolumeType	    CombLongFrozen;
	///��Ͽ�ͷ����
	TThostFtdcVolumeType	    CombShortFrozen;

	CommonAccountType           Account;		            //��Ӧ�˺�
	TThostFtdcExchangeIDType    ExchangeID;                 //����������
	double                      PositionProfitByTrade;		//����ӯ�������гֲ���ϸ��ӯ֮�ͣ�������ʶԳ�ĳֲ�ӯ��
	double                      TotalPositionProfitByDate;	//������ӯ��������ƽ��ӯ���ͳֲ�ӯ��֮��

	int         TotalCloseOrderVolume;      //��ƽ�ֹҵ���
	int         CloseOrderVolume;           //ƽ�ֹҵ�����ָƽ�ֵ���������ƽ���ƽ��
    int         CloseOrderVolumeTdPart;     //ƽ�ֹҵ����Ľ�ֲ���
    int         CloseOrderVolumeYdPart;     //ƽ�ֹҵ�������ֲ���
	int         CloseTodayOrderVolume;      //ƽ��ҵ���
	int         CloseYesterdayOrderVolume;  //ƽ��ҵ���
	int         CanCloseVolume;             //��ƽ��Position-TotalCloseOrderVolume
	int         CanCloseTodayVolume;        //��ƽ����TodayPosition-CloseTodayOrderVolume
	int         CanCloseydVolume;           //��ƽ����YdPosition-CloseYesterdayOrderVolume

	double      AveragePositionPrice;       //�ֲ־���(�ֲֳɱ�/�ֲ���)
	double      AverageOpenPrice;           //���־���((���տ��ֳɱ�+��ʷ���ֳɱ�)/�ܿ�����)

	int         TodayCloseYdVolume;         //��ƽ��ʷ����
	int         YdPositionRemain;	        //����ʣ���λ���ӳֲ���ϸ����

	double      CurPrice;                   //��ǰ���ڼ���ӯ���ļ۸� �����/���¼�/������

    double                  PriceTick;                      //��InstrumentInfo��ȡ��
    CommonCurrencyType      Currency;                       //��������


	// ������Pats���е���
	//����
	int pats_BuyVolumn;
	//����
	int pats_SellVolumn;
	//���ֲ־���
	double pats_OpenAvgPrice;
	//���ֲ�����
	double pats_OpenProfit;
	//������
	double pats_TotalProfit;
	//��֤��ÿ����
	double pats_MarginPerLot;
	//�ʽ�������
	double pats_PLBurnRate;
	//δƽ�ֺ�Լ����
	double pats_OpenPositionExposure;
	//ʣ�๺����
	double pats_BuyPowerRemain;
	//�����ʽ�
	double pats_BuyPowerUsed;
	//������֤��
	double pats_TotalMarginPaid;
	// Pats���Ϊֹ


	//FasttraderID,����ÿһ����¼��Key������һ��Ψһ���
	long FTID;                                                
	//ÿ���������ݷ����仯��UpdateSeq��һ
	long UpdateSeq;


	bool bInvalid;      //��ʾ�Ƿ���Ч
	//static PlatformStru_Position InvalidPosition;

	PlatformStru_Position()
	{
		Clear();
	}
	PlatformStru_Position(bool binvalid)
	{
		Clear();
		bInvalid=binvalid;
	}
	PlatformStru_Position(const CThostFtdcInvestorPositionField& r)
	{
		Clear();

		strncpy(InstrumentID,r.InstrumentID,sizeof(InstrumentID)-1);
		strncpy(BrokerID,r.BrokerID,sizeof(BrokerID)-1);
		strncpy(InvestorID,r.InvestorID,sizeof(InvestorID)-1);
		PosiDirection=r.PosiDirection;
		HedgeFlag=r.HedgeFlag;
		PositionDate=r.PositionDate;
		YdPosition=r.YdPosition;
		Position=r.Position;
		LongFrozen=r.LongFrozen;
		ShortFrozen=r.ShortFrozen;
		LongFrozenAmount=r.LongFrozenAmount;
		ShortFrozenAmount=r.ShortFrozenAmount;
		OpenVolume=r.OpenVolume;
		CloseVolume=r.CloseVolume;
		OpenAmount=r.OpenAmount;
		CloseAmount=r.CloseAmount;
		PositionCost=r.PositionCost;
		PreMargin=r.PreMargin;
		UseMargin=r.UseMargin;
		FrozenMargin=r.FrozenMargin;
		FrozenCash=r.FrozenCash;
		FrozenCommission=r.FrozenCommission;
		CashIn=r.CashIn;
		Commission=r.Commission;
		CloseProfit=r.CloseProfit;
		PositionProfit=r.PositionProfit;
		PreSettlementPrice=r.PreSettlementPrice;
		SettlementPrice=r.SettlementPrice;
		strncpy(TradingDay,r.TradingDay,sizeof(TradingDay)-1);
		SettlementID=r.SettlementID;
		OpenCost=r.OpenCost;
		ExchangeMargin=r.ExchangeMargin;
		CombPosition=r.CombPosition;
		CombLongFrozen=r.CombLongFrozen;
		CombShortFrozen=r.CombShortFrozen;
		CloseProfitByDate=r.CloseProfitByDate;
		CloseProfitByTrade=r.CloseProfitByTrade;
		TodayPosition=r.TodayPosition;
		MarginRateByMoney=r.MarginRateByMoney;
		MarginRateByVolume=r.MarginRateByVolume;

		strncpy(Account,r.InvestorID,sizeof(Account)-1);
		bInvalid=false;
	}

	void Clear()
	{
		memset(this, 0, sizeof(*this));
		int nInvalidInt = INTINVALIDVALUE;
		double dInvalidDouble = DOUBLEINVALIDVALUE;

		YdPosition=nInvalidInt;
		Position=nInvalidInt;
		LongFrozen=nInvalidInt;
		ShortFrozen=nInvalidInt;
		LongFrozenAmount=dInvalidDouble;
		ShortFrozenAmount=dInvalidDouble;
		OpenVolume=nInvalidInt;
		CloseVolume=nInvalidInt;
		OpenAmount=dInvalidDouble;
		CloseAmount=dInvalidDouble;
		PositionCost=dInvalidDouble;
		PreMargin=dInvalidDouble;
		UseMargin=dInvalidDouble;
		FrozenMargin=dInvalidDouble;
		FrozenCash=dInvalidDouble;
		FrozenCommission=dInvalidDouble;
		CashIn=dInvalidDouble;
		Commission=dInvalidDouble;
		CloseProfit=dInvalidDouble;
		PositionProfit=dInvalidDouble;
		PreSettlementPrice=dInvalidDouble;
		SettlementPrice=dInvalidDouble;
		SettlementID=nInvalidInt;
		OpenCost=dInvalidDouble;
		ExchangeMargin=dInvalidDouble;
		CombPosition=nInvalidInt;
		CombLongFrozen=nInvalidInt;
		CombShortFrozen=nInvalidInt;
		CloseProfitByDate=dInvalidDouble;
		CloseProfitByTrade=dInvalidDouble;
		TodayPosition=nInvalidInt;
		MarginRateByMoney=dInvalidDouble;
		MarginRateByVolume=dInvalidDouble;

		PositionProfitByTrade=dInvalidDouble;
		TotalPositionProfitByDate=dInvalidDouble;
		TotalCloseOrderVolume=nInvalidInt;
		CloseOrderVolume=nInvalidInt;
		CloseOrderVolumeTdPart=nInvalidInt;
		CloseOrderVolumeYdPart=nInvalidInt;
		CloseTodayOrderVolume=nInvalidInt;
		CloseYesterdayOrderVolume=nInvalidInt;
		CanCloseVolume=nInvalidInt;
		CanCloseTodayVolume=nInvalidInt;
		CanCloseydVolume=nInvalidInt;
		AveragePositionPrice=dInvalidDouble;
		AverageOpenPrice=dInvalidDouble;
		TodayCloseYdVolume=nInvalidInt;
		YdPositionRemain = nInvalidInt;
		CurPrice=dInvalidDouble;
		FTID=0;
		UpdateSeq=0;
		bInvalid=false;
	}
	bool IsInvalidRecord(void) const
	{
		return bInvalid;
	}
	char* tostring(char*prlt,int rltsize) const
    {
        if(bInvalid&&prlt&&rltsize>0) 
        {
            strncpy(prlt,"Invalid",rltsize);
            prlt[rltsize-1]=0;
            return prlt;
        }

        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,Direct%d,Hedge%d,Position=%d",
            Account,InstrumentID,PosiDirection,HedgeFlag,Position);
        prlt[len]=0;
        return prlt;
    }
};




///�����ѯ�ʽ��˻���Ϣ
struct PlatformStru_QryTradingAccount
{
	PlatformStru_QryTradingAccount()
	{
		memset(this,0,sizeof(*this));
	};

	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
};

struct PlatformStru_QryInvestor
{
    PlatformStru_QryInvestor()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcQryInvestorField Thost;
};
struct PlatformStru_QryProduct
{
    PlatformStru_QryProduct()
    {
        memset(this,0,sizeof(*this));
    }
	CThostFtdcQryProductField Thost;
};
struct PlatformStru_QryInstrument
{
    PlatformStru_QryInstrument()
    {
        memset(this,0,sizeof(*this));
    }
	CThostFtdcQryInstrumentField Thost;
};
struct PlatformStru_QryDepthMarketData
{
    PlatformStru_QryDepthMarketData() { memset(this,0,sizeof(*this)); };
	CThostFtdcQryDepthMarketDataField Thost;
};
struct PlatformStru_QrySettlementInfo
{
    PlatformStru_QrySettlementInfo()
    {
        memset(this,0,sizeof(*this));
    }

	CThostFtdcQrySettlementInfoField Thost;
};
///�����ѯͶ���ֲ߳���ϸ
struct PlatformStru_QryInvestorPositionDetail
{
	PlatformStru_QryInvestorPositionDetail()
	{
		memset(this,0,sizeof(*this));
	}

	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///�����ѯͶ���ֲ߳���ϸ(���)
struct PlatformStru_QryInvestorPositionCombineDetail
{
	PlatformStru_QryInvestorPositionCombineDetail()
	{
		memset(this,0,sizeof(*this));
	}

	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��ϳֲֺ�Լ����
	TThostFtdcInstrumentIDType	CombInstrumentID;
};

struct PlatformStru_QryNotice
{
    PlatformStru_QryNotice()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcQryNoticeField Thost;
};
struct PlatformStru_QrySettlementInfoConfirm
{
    PlatformStru_QrySettlementInfoConfirm()
    {
        memset(this,0,sizeof(*this));
    };

	CThostFtdcQrySettlementInfoConfirmField Thost;
};

///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
struct PlatformStru_QryCFMMCTradingAccountKey
{
    PlatformStru_QryCFMMCTradingAccountKey()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcQryCFMMCTradingAccountKeyField Thost;
};
struct PlatformStru_QryTransferBank
{
    PlatformStru_QryTransferBank()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcQryTransferBankField Thost;
};
struct PlatformStru_QryTransferSerial
{
    PlatformStru_QryTransferSerial()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcQryTransferSerialField Thost;
};
struct PlatformStru_QryContractBank
{
    PlatformStru_QryContractBank()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcQryContractBankField Thost;
};
struct PlatformStru_QryAccountRegister
{
    PlatformStru_QryAccountRegister()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcQryAccountregisterField Thost;
};
///�����ѯ����
struct PlatformStru_QryExchangeRate
{
    PlatformStru_QryExchangeRate()
    {
        memset(this,0,sizeof(*this));
    }
    CThostFtdcQryExchangeRateField Thost;
};
///�����ѯԤ��
struct PlatformStru_QryParkedOrder
{
    PlatformStru_QryParkedOrder()
    {
        memset(this,0,sizeof(*this));
    }
	CThostFtdcQryParkedOrderField Thost;
};
struct PlatformStru_QryParkedOrderAction
{
    PlatformStru_QryParkedOrderAction()
    {
        memset(this,0,sizeof(*this));
    }
	CThostFtdcQryParkedOrderActionField Thost;
};

struct PlatformStru_ReqQueryAccount
{
    PlatformStru_ReqQueryAccount()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcReqQueryAccountField Thost;
};
struct PlatformStru_ReqTransfer
{
    PlatformStru_ReqTransfer()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcReqTransferField Thost;
};
//�˺���Ϣ
struct PlatformStru_Trader
{
	char TraderAccount[21];
	char BackOfficeID[21];
	char Tradable;
	int LossLimit;
};
//��������Ϣ
struct PlatformStru_Exchange
{
	char Name[11];
	char QueryEnabled;
	char AmendEnabled;
	int Strategy;
	char CustomDecs;
	int Decimals;
	char TicketType;
	char RFQA;
	char RFQT;
	char EnableBlock;
	char EnableBasis;
	char EnableAA;
	char EnableCross;
	int GTStatus;

};
//��Լ��Ϣ
typedef char Array10[11];
typedef Array10 LegStruct[5];
struct PlatformStru_Contract
{
	char  ContractName[11];
	char ContractDate[51];
	char ExchangeName[11];
	char ExpiryDate[9];
	char LastTradeDate[9];
	int NumberOfLegs;
	int TicksPerPoint;
	char TickSize[11];
	char Tradable;
	int GTStatus;
	char Margin[21];
	char ESATemplate;
	char MarketRef[17];
	char lnExchangeName[11];
	char lnContractName[11];
	char lnContractDate[51];
	LegStruct ExternalID[33];
};
//��Ʒ��Ϣ
struct PlatformStru_Commodity
{
	Array10 ExchangeName;
	Array10 ContractName;
	Array10 Currency;
	Array10 Group;
	Array10 OnePoint;
	int TicksPerPoint;
	Array10 TickSize;
	int GTStatus;
};
//��������
struct PlatformStru_OrderType
{
	Array10 OrderType;
	Array10 ExchangeName;
	int OrderTypeID;
	unsigned char NumPricesReqd;
	unsigned char NumVolumesReqd;
	unsigned char NumDatesReqd;
	char AutoCreated;
	char TimeTriggered;
	char RealSynthetic;
	char GTCFlag;
	char TicketType[3];
	char PatsOrderType;
	int AmendOTCount;
	char AlgoXML[51];
	char AmendTypesArray[501];
};
//ǩԼ������Ϣ
struct PlatformStru_ContractBank
{
	///���б���
	TThostFtdcBankIDType	BankID;
	///��������
	TThostFtdcBankNameType	BankName;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///�ڻ���˾����
	TThostFtdcBrokerIDType	BrokerID;
};
struct PlatformStru_ExchangeRate
{
    PlatformStru_ExchangeRate() { memset(this,0,sizeof(*this)); }

	///���͹�˾����
	TThostFtdcBrokerIDType	    BrokerID;
	///Դ����
	TThostFtdcCurrencyIDType	FromCurrencyID;
	///Դ���ֵ�λ����
	TThostFtdcCurrencyUnitType	FromCurrencyUnit;
	///Ŀ�����
	TThostFtdcCurrencyIDType	ToCurrencyID;
	///����
	TThostFtdcExchangeRateType	ExchangeRate;
};

///�����ѯ��
struct PlatformStru_InputForQuoteField
{
    PlatformStru_InputForQuoteField()
    {
        memset(this,0,sizeof(*this));
    };
#ifdef CTP060300
    PlatformStru_InputForQuoteField(const CThostFtdcInputForQuoteField& thost)
    {
        memset(this,0,sizeof(*this));
        strncpy(BrokerID,thost.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,thost.InvestorID,sizeof(InvestorID)-1);
        strncpy(InstrumentID,thost.InstrumentID,sizeof(InstrumentID)-1);
        strncpy(ForQuoteRef,thost.ForQuoteRef,sizeof(ForQuoteRef)-1);
        strncpy(UserID,thost.UserID,sizeof(UserID)-1);
    };
    void ToThost(CThostFtdcInputForQuoteField& thost)
    {
        memset(&thost,0,sizeof(thost));
        strncpy(thost.BrokerID,BrokerID,sizeof(thost.BrokerID)-1);
        strncpy(thost.InvestorID,InvestorID,sizeof(thost.InvestorID)-1);
        strncpy(thost.InstrumentID,InstrumentID,sizeof(thost.InstrumentID)-1);
        strncpy(thost.ForQuoteRef,ForQuoteRef,sizeof(thost.ForQuoteRef)-1);
        strncpy(thost.UserID,UserID,sizeof(thost.UserID)-1);
    };
#endif
    string tostring(void)
    {
        char buf[1024]={0};
        sprintf_s(buf,sizeof(buf)-1,"BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ForQuoteRef=[%s],\t UserID=[%s]",
            BrokerID,InvestorID,InstrumentID,ForQuoteRef,UserID);
        return string(buf);
    };
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///ѯ������
	TThostFtdcOrderRefType	ForQuoteRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
};

///�����ִ������
struct PlatformStru_InputExecOrderField
{
    PlatformStru_InputExecOrderField()
    {
        memset(this,0,sizeof(*this));
    };
#ifdef CTP060300
    PlatformStru_InputExecOrderField(const CThostFtdcInputExecOrderField& thost)
    {
        memset(this,0,sizeof(*this));
        strncpy(BrokerID,thost.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,thost.InvestorID,sizeof(InvestorID)-1);
        strncpy(InstrumentID,thost.InstrumentID,sizeof(InstrumentID)-1);
        strncpy(ExecOrderRef,thost.ExecOrderRef,sizeof(ExecOrderRef)-1);
        strncpy(UserID,thost.UserID,sizeof(UserID)-1);
        Volume=thost.Volume;
        RequestID=thost.RequestID;
        strncpy(BusinessUnit,thost.BusinessUnit,sizeof(BusinessUnit)-1);
        OffsetFlag=thost.OffsetFlag;
        HedgeFlag=thost.HedgeFlag;
        ActionType=thost.ActionType;
        PosiDirection=thost.PosiDirection;
        ReservePositionFlag=thost.ReservePositionFlag;
        CloseFlag=thost.CloseFlag;
    };
    void ToThost(CThostFtdcInputExecOrderField& thost)
    {
        memset(&thost,0,sizeof(thost));
        strncpy(thost.BrokerID,BrokerID,sizeof(thost.BrokerID)-1);
        strncpy(thost.InvestorID,InvestorID,sizeof(thost.InvestorID)-1);
        strncpy(thost.InstrumentID,InstrumentID,sizeof(thost.InstrumentID)-1);
        strncpy(thost.ExecOrderRef,ExecOrderRef,sizeof(thost.ExecOrderRef)-1);
        strncpy(thost.UserID,UserID,sizeof(thost.UserID)-1);
        thost.Volume=Volume;
        thost.RequestID=RequestID;
        strncpy(thost.BusinessUnit,BusinessUnit,sizeof(thost.BusinessUnit)-1);
        thost.OffsetFlag=OffsetFlag;
        thost.HedgeFlag=HedgeFlag;
        thost.ActionType=ActionType;
        thost.PosiDirection=PosiDirection;
        thost.ReservePositionFlag=ReservePositionFlag;
        thost.CloseFlag=CloseFlag;
    };
#endif
    string tostring(void)
    {
        char buf[1024]={0};
        sprintf_s(buf,sizeof(buf)-1,
            "BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExecOrderRef=[%s],\t UserID=[%s]"
            ",\t Volume=[%d],\t RequestID=[%d],\t BusinessUnit=[%s],\t OffsetFlag=[%d],\t HedgeFlag=[%d]"
            ",\t ActionType=[%d],\t PosiDirection=[%d],\t ReservePositionFlag=[%d],\t CloseFlag=[%d]",
             BrokerID, InvestorID, InstrumentID, ExecOrderRef,UserID,
             Volume, RequestID, BusinessUnit, OffsetFlag,HedgeFlag,
             ActionType, PosiDirection, ReservePositionFlag, CloseFlag);
        return string(buf);
    };

	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///ִ����������
	TThostFtdcOrderRefType	ExecOrderRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///����
	TThostFtdcVolumeType	Volume;
	///������
	TThostFtdcRequestIDType	RequestID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///��ƽ��־
	TThostFtdcOffsetFlagType	OffsetFlag;
	///Ͷ���ױ���־
	TThostFtdcHedgeFlagType	HedgeFlag;
	///ִ������
	char	                    ActionType;         //TThostFtdcActionTypeType
	///����ͷ������ĳֲַ���
	char	                    PosiDirection;      //TThostFtdcPosiDirectionType
	///��Ȩ��Ȩ���Ƿ����ڻ�ͷ��ı��
	char	                    ReservePositionFlag;//TThostFtdcExecOrderPositionFlagType
	///��Ȩ��Ȩ�����ɵ�ͷ���Ƿ��Զ�ƽ��
	char	                    CloseFlag;          //TThostFtdcExecOrderCloseFlagType
};
///����ִ���������
struct PlatformStru_InputExecOrderActionField
{
    PlatformStru_InputExecOrderActionField()
    {
        memset(this,0,sizeof(*this));
    };
#ifdef CTP060300
    PlatformStru_InputExecOrderActionField(const CThostFtdcInputExecOrderActionField& thost)
    {
        memset(this,0,sizeof(*this));
        strncpy(BrokerID,thost.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,thost.InvestorID,sizeof(InvestorID)-1);
        ExecOrderActionRef=thost.ExecOrderActionRef;
        strncpy(ExecOrderRef,thost.ExecOrderRef,sizeof(ExecOrderRef)-1);
        RequestID=thost.RequestID;
        FrontID=thost.FrontID;
        SessionID=thost.SessionID;
        strncpy(ExchangeID,thost.ExchangeID,sizeof(ExchangeID)-1);
        strncpy(ExecOrderSysID,thost.ExecOrderSysID,sizeof(ExecOrderSysID)-1);
        ActionFlag=thost.ActionFlag;
        strncpy(UserID,thost.UserID,sizeof(UserID)-1);
        strncpy(InstrumentID,thost.InstrumentID,sizeof(InstrumentID)-1);
    };
    void ToThost(CThostFtdcInputExecOrderActionField& thost)
    {
        memset(&thost,0,sizeof(thost));
        strncpy(thost.BrokerID,BrokerID,sizeof(thost.BrokerID)-1);
        strncpy(thost.InvestorID,InvestorID,sizeof(thost.InvestorID)-1);
        thost.ExecOrderActionRef=ExecOrderActionRef;
        strncpy(thost.ExecOrderRef,ExecOrderRef,sizeof(thost.ExecOrderRef)-1);
        thost.RequestID=RequestID;
        thost.FrontID=FrontID;
        thost.SessionID=SessionID;
        strncpy(thost.ExchangeID,ExchangeID,sizeof(thost.ExchangeID)-1);
        strncpy(thost.ExecOrderSysID,ExecOrderSysID,sizeof(thost.ExecOrderSysID)-1);
        thost.ActionFlag=ActionFlag;
        strncpy(thost.UserID,UserID,sizeof(thost.UserID)-1);
        strncpy(thost.InstrumentID,InstrumentID,sizeof(thost.InstrumentID)-1);
    };
#endif
    string tostring(void)
    {
        char buf[1024]={0};
        sprintf_s(buf,sizeof(buf)-1,
             "BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExecOrderActionRef=[%d],\t ExecOrderRef=[%s],\t UserID=[%s]"
             ",\t RequestID=[%d],\t FrontID=[%d],\t SessionID=[%d],\t ExchangeID=[%s],\t ExecOrderSysID=[%s],\t ActionFlag=[%d]",
             BrokerID, InvestorID, InstrumentID, ExecOrderActionRef, ExecOrderRef, UserID,
             RequestID, FrontID, SessionID, ExchangeID, ExecOrderSysID, ActionFlag);
        return string(buf);
    };

    ///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///ִ�������������
	int	                    ExecOrderActionRef;     //TThostFtdcOrderActionRefType
	///ִ����������
	TThostFtdcOrderRefType	ExecOrderRef;
	///������
	TThostFtdcRequestIDType	RequestID;
	///ǰ�ñ��
	TThostFtdcFrontIDType	FrontID;
	///�Ự���
	TThostFtdcSessionIDType	SessionID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///ִ������������
	char	                ExecOrderSysID[21];     //TThostFtdcExecOrderSysIDType
	///������־
	TThostFtdcActionFlagType	ActionFlag;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};
///ִ�������ѯ
struct PlatformStru_QryExecOrderField
{
    PlatformStru_QryExecOrderField()
    {
        memset(this,0,sizeof(*this));
    };
#ifdef CTP060300
    PlatformStru_QryExecOrderField(const CThostFtdcQryExecOrderField& thost)
    {
        memset(this,0,sizeof(*this));
        strncpy(BrokerID,thost.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,thost.InvestorID,sizeof(InvestorID)-1);
        strncpy(InstrumentID,thost.InstrumentID,sizeof(InstrumentID)-1);
        strncpy(ExchangeID,thost.ExchangeID,sizeof(ExchangeID)-1);
        strncpy(ExecOrderSysID,thost.ExecOrderSysID,sizeof(ExecOrderSysID)-1);
        strncpy(InsertTimeStart,thost.InsertTimeStart,sizeof(InsertTimeStart)-1);
        strncpy(InsertTimeEnd,thost.InsertTimeEnd,sizeof(InsertTimeEnd)-1);
    };
    void ToThost(CThostFtdcQryExecOrderField& thost)
    {
        memset(&thost,0,sizeof(thost));
        strncpy(thost.BrokerID,BrokerID,sizeof(thost.BrokerID)-1);
        strncpy(thost.InvestorID,InvestorID,sizeof(thost.InvestorID)-1);
        strncpy(thost.InstrumentID,InstrumentID,sizeof(thost.InstrumentID)-1);
        strncpy(thost.ExchangeID,ExchangeID,sizeof(thost.ExchangeID)-1);
        strncpy(thost.ExecOrderSysID,ExecOrderSysID,sizeof(thost.ExecOrderSysID)-1);
        strncpy(thost.InsertTimeStart,InsertTimeStart,sizeof(thost.InsertTimeStart)-1);
        strncpy(thost.InsertTimeEnd,InsertTimeEnd,sizeof(thost.InsertTimeEnd)-1);
    };
#endif
    string tostring(void)
    {
        char buf[1024]={0};
        sprintf_s(buf,sizeof(buf)-1,
             "BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s],\t ExecOrderSysID=[%s]"
             ",\t InsertTimeStart=[%s],\t InsertTimeEnd=[%s]",
             BrokerID, InvestorID, InstrumentID, ExchangeID,ExecOrderSysID,
             InsertTimeStart,InsertTimeEnd);
        return string(buf);
    };

    ///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///ִ��������
	char	ExecOrderSysID[21];         //TThostFtdcExecOrderSysIDType
	///��ʼʱ��
	TThostFtdcTimeType	InsertTimeStart;
	///����ʱ��
	TThostFtdcTimeType	InsertTimeEnd;
};
///ִ���������
struct PlatformStru_ExecOrderActionField
{
    PlatformStru_ExecOrderActionField()
    {
        memset(this,0,sizeof(*this));
    };
#ifdef CTP060300
    PlatformStru_ExecOrderActionField(const CThostFtdcExecOrderActionField& thost)
    {
        memset(this,0,sizeof(*this));
        strncpy(BrokerID,thost.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,thost.InvestorID,sizeof(InvestorID)-1);
        ExecOrderActionRef=thost.ExecOrderActionRef;
        strncpy(ExecOrderRef,thost.ExecOrderRef,sizeof(ExecOrderRef)-1);
        RequestID=thost.RequestID;
        FrontID=thost.FrontID;
        SessionID=thost.SessionID;
        strncpy(ExchangeID,thost.ExchangeID,sizeof(ExchangeID)-1);
        strncpy(ExecOrderSysID,thost.ExecOrderSysID,sizeof(ExecOrderSysID)-1);
        ActionFlag=thost.ActionFlag;
        strncpy(ActionDate,thost.ActionDate,sizeof(ActionDate)-1);
        strncpy(ActionTime,thost.ActionTime,sizeof(ActionTime)-1);
        strncpy(TraderID,thost.TraderID,sizeof(TraderID)-1);
        InstallID=thost.InstallID;
        strncpy(ExecOrderLocalID,thost.ExecOrderLocalID,sizeof(ExecOrderLocalID)-1);
        strncpy(ActionLocalID,thost.ActionLocalID,sizeof(ActionLocalID)-1);
        strncpy(ParticipantID,thost.ParticipantID,sizeof(ParticipantID)-1);
        strncpy(ClientID,thost.ClientID,sizeof(ClientID)-1);
        strncpy(BusinessUnit,thost.BusinessUnit,sizeof(BusinessUnit)-1);
        OrderActionStatus=thost.OrderActionStatus;
        strncpy(UserID,thost.UserID,sizeof(UserID)-1);
        ActionType=thost.ActionType;
        strncpy(StatusMsg,thost.StatusMsg,sizeof(StatusMsg)-1);
        strncpy(InstrumentID,thost.InstrumentID,sizeof(InstrumentID)-1);
    };
    void ToThost(CThostFtdcExecOrderActionField& thost)
    {
        memset(&thost,0,sizeof(thost));
        strncpy(thost.BrokerID,BrokerID,sizeof(thost.BrokerID)-1);
        strncpy(thost.InvestorID,InvestorID,sizeof(thost.InvestorID)-1);
        thost.ExecOrderActionRef=ExecOrderActionRef;
        strncpy(thost.ExecOrderRef,ExecOrderRef,sizeof(thost.ExecOrderRef)-1);
        thost.RequestID=RequestID;
        thost.FrontID=FrontID;
        thost.SessionID=SessionID;
        strncpy(thost.ExchangeID,ExchangeID,sizeof(thost.ExchangeID)-1);
        strncpy(thost.ExecOrderSysID,ExecOrderSysID,sizeof(thost.ExecOrderSysID)-1);
        thost.ActionFlag=ActionFlag;
        strncpy(thost.ActionDate,ActionDate,sizeof(thost.ActionDate)-1);
        strncpy(thost.ActionTime,ActionTime,sizeof(thost.ActionTime)-1);
        strncpy(thost.TraderID,TraderID,sizeof(thost.TraderID)-1);
        thost.InstallID=InstallID;
        strncpy(thost.ExecOrderLocalID,ExecOrderLocalID,sizeof(thost.ExecOrderLocalID)-1);
        strncpy(thost.ActionLocalID,ActionLocalID,sizeof(thost.ActionLocalID)-1);
        strncpy(thost.ParticipantID,ParticipantID,sizeof(thost.ParticipantID)-1);
        strncpy(thost.ClientID,ClientID,sizeof(thost.ClientID)-1);
        strncpy(thost.BusinessUnit,BusinessUnit,sizeof(thost.BusinessUnit)-1);
        thost.OrderActionStatus=OrderActionStatus;
        strncpy(thost.UserID,UserID,sizeof(thost.UserID)-1);
        thost.ActionType=ActionType;
        strncpy(thost.StatusMsg,StatusMsg,sizeof(thost.StatusMsg)-1);
        strncpy(thost.InstrumentID,InstrumentID,sizeof(thost.InstrumentID)-1);
    };
#endif
    string tostring(void)
    {
        char buf[1024]={0};
        sprintf_s(buf,sizeof(buf)-1,
            "BrokerID=[%s],\t InvestorID=[%s],\t TraderID=[%s],\t ParticipantID=[%s],\t ClientID=[%s],\t UserID=[%s]"
            ",\t InstrumentID=[%s],\t ExchangeID=[%s],\t OrderActionStatus=[%d],\t StatusMsg=[%s]"
            ",\t ActionType=[%d],\t ActionFlag=[%d],\t ActionDate=[%s],\t ActionTime=[%s]"
            ",\t ExecOrderLocalID=[%s],\t ExecOrderSysID=[%s],\t ExecOrderActionRef=[%d],\t ExecOrderRef=[%s],\t ActionLocalID=[%s]"
            ",\t RequestID=[%d],\t FrontID=[%d],\t SessionID=[%d],\t InstallID=[%d],\t BusinessUnit=[%s]",
            BrokerID,InvestorID,TraderID,ParticipantID,ClientID,UserID,
            InstrumentID,ExchangeID,OrderActionStatus,StatusMsg,
            ActionType,ActionFlag,ActionDate,ActionTime,
            ExecOrderLocalID,ExecOrderSysID,ExecOrderActionRef,ExecOrderRef,ActionLocalID,
            RequestID,FrontID,SessionID,InstallID,BusinessUnit);
        return string(buf);
    };

    ///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///ִ�������������
	TThostFtdcOrderActionRefType	ExecOrderActionRef;
	///ִ����������
	TThostFtdcOrderRefType	ExecOrderRef;
	///������
	TThostFtdcRequestIDType	RequestID;
	///ǰ�ñ��
	TThostFtdcFrontIDType	FrontID;
	///�Ự���
	TThostFtdcSessionIDType	SessionID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///ִ������������
	TThostFtdcExecOrderSysIDType	ExecOrderSysID;
	///������־
	TThostFtdcActionFlagType	ActionFlag;
	///��������
	TThostFtdcDateType	ActionDate;
	///����ʱ��
	TThostFtdcTimeType	ActionTime;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	TThostFtdcInstallIDType	InstallID;
	///����ִ��������
	TThostFtdcOrderLocalIDType	ExecOrderLocalID;
	///�������ر��
	TThostFtdcOrderLocalIDType	ActionLocalID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///��������״̬
	TThostFtdcOrderActionStatusType	OrderActionStatus;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///ִ������
	TThostFtdcActionTypeType	ActionType;
	///״̬��Ϣ
	TThostFtdcErrorMsgType	StatusMsg;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};

///ִ������
struct PlatformStru_ExecOrderField
{
    PlatformStru_ExecOrderField()
    {
        memset(this,0,sizeof(*this));
    };
#ifdef CTP060300
    PlatformStru_ExecOrderField(const CThostFtdcExecOrderField& thost)
    {
        memset(this,0,sizeof(*this));
        strncpy(BrokerID,thost.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,thost.InvestorID,sizeof(InvestorID)-1);
        strncpy(InstrumentID,thost.InstrumentID,sizeof(InstrumentID)-1);
        strncpy(ExecOrderRef,thost.ExecOrderRef,sizeof(ExecOrderRef)-1);
        strncpy(UserID,thost.UserID,sizeof(UserID)-1);
        Volume=thost.Volume;
        RequestID=thost.RequestID;
        strncpy(BusinessUnit,thost.BusinessUnit,sizeof(BusinessUnit)-1);
        OffsetFlag=thost.OffsetFlag;
        HedgeFlag=thost.HedgeFlag;
        ActionType=thost.ActionType;
        PosiDirection=thost.PosiDirection;
        ReservePositionFlag=thost.ReservePositionFlag;
        CloseFlag=thost.CloseFlag;
        strncpy(ExecOrderLocalID,thost.ExecOrderLocalID,sizeof(ExecOrderLocalID)-1);
        strncpy(ExchangeID,thost.ExchangeID,sizeof(ExchangeID)-1);
        strncpy(ParticipantID,thost.ParticipantID,sizeof(ParticipantID)-1);
        strncpy(ClientID,thost.ClientID,sizeof(ClientID)-1);
        strncpy(ExchangeInstID,thost.ExchangeInstID,sizeof(ExchangeInstID)-1);
        strncpy(TraderID,thost.TraderID,sizeof(TraderID)-1);
        InstallID=thost.InstallID;
        OrderSubmitStatus=thost.OrderSubmitStatus;
        NotifySequence=thost.NotifySequence;
        strncpy(TradingDay,thost.TradingDay,sizeof(TradingDay)-1);
        SettlementID=thost.SettlementID;
        strncpy(ExecOrderSysID,thost.ExecOrderSysID,sizeof(ExecOrderSysID)-1);
        strncpy(InsertDate,thost.InsertDate,sizeof(InsertDate)-1);
        strncpy(InsertTime,thost.InsertTime,sizeof(InsertTime)-1);
        strncpy(CancelTime,thost.CancelTime,sizeof(CancelTime)-1);
        ExecResult=thost.ExecResult;
        strncpy(ClearingPartID,thost.ClearingPartID,sizeof(ClearingPartID)-1);
        SequenceNo=thost.SequenceNo;
        FrontID=thost.FrontID;
        SessionID=thost.SessionID;
        strncpy(UserProductInfo,thost.UserProductInfo,sizeof(UserProductInfo)-1);
        strncpy(StatusMsg,thost.StatusMsg,sizeof(StatusMsg)-1);
        strncpy(ActiveUserID,thost.ActiveUserID,sizeof(ActiveUserID)-1);
        BrokerExecOrderSeq=thost.BrokerExecOrderSeq;

        strncpy(Account,InvestorID,sizeof(Account)-1);
    };
    void ToThost(CThostFtdcExecOrderField& thost)
    {
        memset(&thost,0,sizeof(thost));
        strncpy(thost.BrokerID,BrokerID,sizeof(thost.BrokerID)-1);
        strncpy(thost.InvestorID,InvestorID,sizeof(thost.InvestorID)-1);
        strncpy(thost.InstrumentID,InstrumentID,sizeof(thost.InstrumentID)-1);
        strncpy(thost.ExecOrderRef,ExecOrderRef,sizeof(thost.ExecOrderRef)-1);
        strncpy(thost.UserID,UserID,sizeof(thost.UserID)-1);
        thost.Volume=Volume;
        thost.RequestID=RequestID;
        strncpy(thost.BusinessUnit,BusinessUnit,sizeof(thost.BusinessUnit)-1);
        thost.OffsetFlag=OffsetFlag;
        thost.HedgeFlag=HedgeFlag;
        thost.ActionType=ActionType;
        thost.PosiDirection=PosiDirection;
        thost.ReservePositionFlag=ReservePositionFlag;
        thost.CloseFlag=CloseFlag;
        strncpy(thost.ExecOrderLocalID,ExecOrderLocalID,sizeof(thost.ExecOrderLocalID)-1);
        strncpy(thost.ExchangeID,ExchangeID,sizeof(thost.ExchangeID)-1);
        strncpy(thost.ParticipantID,ParticipantID,sizeof(thost.ParticipantID)-1);
        strncpy(thost.ClientID,ClientID,sizeof(thost.ClientID)-1);
        strncpy(thost.ExchangeInstID,ExchangeInstID,sizeof(thost.ExchangeInstID)-1);
        strncpy(thost.TraderID,TraderID,sizeof(thost.TraderID)-1);
        thost.InstallID=InstallID;
        thost.OrderSubmitStatus=OrderSubmitStatus;
        thost.NotifySequence=NotifySequence;
        strncpy(thost.TradingDay,TradingDay,sizeof(thost.TradingDay)-1);
        thost.SettlementID=SettlementID;
        strncpy(thost.ExecOrderSysID,ExecOrderSysID,sizeof(thost.ExecOrderSysID)-1);
        strncpy(thost.InsertDate,InsertDate,sizeof(thost.InsertDate)-1);
        strncpy(thost.InsertTime,InsertTime,sizeof(thost.InsertTime)-1);
        strncpy(thost.CancelTime,CancelTime,sizeof(thost.CancelTime)-1);
        thost.ExecResult=ExecResult;
        strncpy(thost.ClearingPartID,ClearingPartID,sizeof(thost.ClearingPartID)-1);
        thost.SequenceNo=SequenceNo;
        thost.FrontID=FrontID;
        thost.SessionID=SessionID;
        strncpy(thost.UserProductInfo,UserProductInfo,sizeof(thost.UserProductInfo)-1);
        strncpy(thost.StatusMsg,StatusMsg,sizeof(thost.StatusMsg)-1);
        strncpy(thost.ActiveUserID,ActiveUserID,sizeof(thost.ActiveUserID)-1);
        thost.BrokerExecOrderSeq=BrokerExecOrderSeq;
    };
#endif
    string tostring(void)
    {
        char buf[1024]={0};
        sprintf_s(buf,sizeof(buf)-1,
            "BrokerID=[%s],\t InvestorID=[%s],\t TraderID=[%s],\t ParticipantID=[%s],\t ClientID=[%s],\t UserID=[%s],\t ActiveUserID=[%s]"
            ",\t InstrumentID=[%s],\t ExchangeID=[%s],\t ExchangeInstID=[%s],\t Volume=[%d],\t OffsetFlag=[%d],\t HedgeFlag=[%d]"
            ",\t ExecResult=[%d],\t OrderSubmitStatus=[%d],\t NotifySequence=[%d],\t StatusMsg=[%s]"
            ",\t ActionType=[%d],\t InsertDate=[%s],\t InsertTime=[%s],\t CancelTime=[%s],\t TradingDay=[%s]"
            ",\t PosiDirection=[%d],\t ReservePositionFlag=[%d],\t CloseFlag=[%d]"
            ",\t ExecOrderLocalID=[%s],\t ExecOrderSysID=[%s],\t ExecOrderRef=[%s],\t BrokerExecOrderSeq=[%d]"
            ",\t RequestID=[%d],\t FrontID=[%d],\t SessionID=[%d],\t InstallID=[%d],\t BusinessUnit=[%s],\t SettlementID=[%d]"
            ",\t ClearingPartID=[%s],\t SequenceNo=[%d],\t UserProductInfo=[%s]",
            BrokerID,InvestorID,TraderID,ParticipantID,ClientID,UserID,ActiveUserID,
            InstrumentID,ExchangeID,ExchangeInstID,Volume,OffsetFlag,HedgeFlag,
            ExecResult,OrderSubmitStatus,NotifySequence,StatusMsg,
            ActionType,InsertDate,InsertTime,CancelTime,TradingDay,
            PosiDirection,ReservePositionFlag,CloseFlag,
            ExecOrderLocalID,ExecOrderSysID,ExecOrderRef,BrokerExecOrderSeq,
            RequestID,FrontID,SessionID,InstallID,BusinessUnit,SettlementID,
            ClearingPartID,SequenceNo,UserProductInfo);
        return string(buf);
    };

	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///ִ����������
	TThostFtdcOrderRefType	ExecOrderRef;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///����
	TThostFtdcVolumeType	Volume;
	///������
	TThostFtdcRequestIDType	RequestID;
	///ҵ��Ԫ
	TThostFtdcBusinessUnitType	BusinessUnit;
	///��ƽ��־
	TThostFtdcOffsetFlagType	OffsetFlag;
	///Ͷ���ױ���־
	TThostFtdcHedgeFlagType	HedgeFlag;
	///ִ������
	char	ActionType;                             //TThostFtdcActionTypeType
	///����ͷ������ĳֲַ���
	TThostFtdcPosiDirectionType	PosiDirection;
	///��Ȩ��Ȩ���Ƿ����ڻ�ͷ��ı��
	char	ReservePositionFlag;                    //TThostFtdcExecOrderPositionFlagType
	///��Ȩ��Ȩ�����ɵ�ͷ���Ƿ��Զ�ƽ��
	char	CloseFlag;                              //TThostFtdcExecOrderCloseFlagType
	///����ִ��������
	TThostFtdcOrderLocalIDType	ExecOrderLocalID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Ա����
	TThostFtdcParticipantIDType	ParticipantID;
	///�ͻ�����
	TThostFtdcClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TThostFtdcTraderIDType	TraderID;
	///��װ���
	TThostFtdcInstallIDType	InstallID;
	///ִ�������ύ״̬
	TThostFtdcOrderSubmitStatusType	OrderSubmitStatus;
	///������ʾ���
	TThostFtdcSequenceNoType	NotifySequence;
	///������
	TThostFtdcDateType	TradingDay;
	///������
	TThostFtdcSettlementIDType	SettlementID;
	///ִ��������
	char	ExecOrderSysID[21];             //TThostFtdcExecOrderSysIDType
	///��������
	TThostFtdcDateType	InsertDate;
	///����ʱ��
	TThostFtdcTimeType	InsertTime;
	///����ʱ��
	TThostFtdcTimeType	CancelTime;
	///ִ�н��
	char	ExecResult;                     //TThostFtdcExecResultType
	///�����Ա���
	TThostFtdcParticipantIDType	ClearingPartID;
	///���
	TThostFtdcSequenceNoType	SequenceNo;
	///ǰ�ñ��
	TThostFtdcFrontIDType	FrontID;
	///�Ự���
	TThostFtdcSessionIDType	SessionID;
	///�û��˲�Ʒ��Ϣ
	TThostFtdcProductInfoType	UserProductInfo;
	///״̬��Ϣ
	TThostFtdcErrorMsgType	StatusMsg;
	///�����û�����
	TThostFtdcUserIDType	ActiveUserID;
	///���͹�˾�������
	TThostFtdcSequenceNoType	BrokerExecOrderSeq;

    //��Ӧ�˺�
	CommonAccountType           Account;		            
	//FasttraderID,����ÿһ����¼��Key������һ��Ψһ���
	long FTID;                                                
	//ÿ���������ݷ����仯��UpdateSeq��һ
	long UpdateSeq;

};
///ѯ��֪ͨ
struct PlatformStru_ForQuoteRspField
{
    PlatformStru_ForQuoteRspField()
    {
        memset(this,0,sizeof(*this));
    };
#ifdef CTP060300
    PlatformStru_ForQuoteRspField(const CThostFtdcForQuoteRspField& thost)
    {
        memset(this,0,sizeof(*this));
        strncpy(TradingDay,thost.TradingDay,sizeof(TradingDay)-1);
        strncpy(InstrumentID,thost.InstrumentID,sizeof(InstrumentID)-1);
        strncpy(ForQuoteSysID,thost.ForQuoteSysID,sizeof(ForQuoteSysID)-1);
        strncpy(ForQuoteTime,thost.ForQuoteTime,sizeof(ForQuoteTime)-1);
        strncpy(ActionDay,thost.ActionDay,sizeof(ActionDay)-1);
    };
#endif
    string tostring(void)
    {
        char buf[1024]={0};
        sprintf_s(buf,sizeof(buf)-1,
            "TradingDay=[%s],\t InstrumentID=[%s],\t ForQuoteSysID=[%s],\t ForQuoteTime=[%s],\t ActionDay=[%s]",
            TradingDay,InstrumentID,ForQuoteSysID,ForQuoteTime,ActionDay);
        return string(buf);
    };

	///������
	TThostFtdcDateType	TradingDay;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///ѯ�۱��
	TThostFtdcOrderSysIDType	ForQuoteSysID;
	///ѯ��ʱ��
	TThostFtdcTimeType	ForQuoteTime;
	///ҵ������
	TThostFtdcDateType	ActionDay;
};


#pragma pack(push)
#pragma pack(1)
struct PlatformStru_AccountPosition
{
	char Profit[21];
	int Buys;
	int Sells;
};
struct PlatformStru_MessageStruct
{    
	char MsgID[11];
	char MsgText[501];
	char IsAlert;
	char Status;
};
#pragma pack(pop)
struct ConnectivityStatus
{
	char DeviceType[3];
	char ExchangeID[20];
	char DeviceName[20];
	char Commentary[255];
	char Status[3];
	char Severity[3];
};

struct PlusinAcctInstrumentKey
{
	CommonAccountType szAccountID;			//�˺�
	CommonAccountType szSubAccountID;		//���˺�
	CommonInstrumentIDType szInstrumentID;	//��ԼID
};

struct PlusinAcctInstrumentLmtPriceKey
{
	CommonAccountType szAccountID;			//�˺�
	CommonAccountType szSubAccountID;		//���˺�
	CommonInstrumentIDType szInstrumentID;	//��ԼID
	double dLimitPrice;						//�޼�
};

struct PlusinOpenOrderKey
{
	CommonAccountType szAccountID;			//�˺�
	CommonAccountType szSubAccountID;		//���˺�
	CommonInstrumentIDType szInstrumentID;	//��ԼID
	char cDirection;						//����
	double dLimitPrice;						//�޼�
};

struct PlusinOpenOrderNumRsp
{
	PlusinOpenOrderKey queryCondition;
	int nNum;
};

struct PlusinAcctOpenOrderNumRsp
{
	PlusinAcctInstrumentKey queryCondition;
	int nNum;
};

struct ExchangeCurTime
{
	DWORD dwTickCount;
	char szExchangeTime[32];
};

struct WaitOrderPrice
{
	double dHighPrice;
	double dLowPrice;

	WaitOrderPrice()
	{
		dHighPrice = dLowPrice = DOUBLEINVALIDVALUE;
	}
};


struct PlatformStru_RspOrderAction1
{
	CThostFtdcInputOrderActionField InputOrderActionField;
	CThostFtdcRspInfoField     RspInfoField; 
};

struct PlatformStru_RspOrderAction2
{
	CThostFtdcInputOrderActionField OrderActionField;
	CThostFtdcRspInfoField     RspInfoField;  
};

struct PlatformStru_RspOrderInsert1
{
	CThostFtdcInputOrderActionField InputOrderInsertField;
	CThostFtdcRspInfoField     RspInfoField; 
};

struct PlatformStru_RspOrderInsert2
{
	CThostFtdcInputOrderActionField OrderInsertField;
	CThostFtdcRspInfoField     RspInfoField;  
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

//�򵥵ĺ�Լ��Ϣ
struct SimpleInstrumentInfo
{
	SimpleInstrumentInfo(std::string sid="",std::string sname=""):id(sid),name(sname){}
    SimpleInstrumentInfo(const SimpleInstrumentInfo& r) { *this=r; }
    SimpleInstrumentInfo& operator= (const SimpleInstrumentInfo& r) { id=r.id;  name=r.name;  return *this; }

	std::string id;
	std::string name;
} ;

struct InstrumentGroupInfo
{
    InstrumentGroupInfo() {};
    InstrumentGroupInfo(const InstrumentGroupInfo& r) { *this=r; }
    InstrumentGroupInfo& operator= (const InstrumentGroupInfo& r) { GroupName=r.GroupName;  InstrVec=r.InstrVec;  return *this; }

	std::string GroupName;
	std::vector<SimpleInstrumentInfo> InstrVec;
};

struct CTPReport
{
	std::string strReportType;
	int nSize;
	void* pReport;

	CTPReport()
	{
		strReportType = "";
		nSize = 0;
		pReport = NULL;
	}

	DWORD GetDate()
	{
		int nPos = strReportType.find( ' ');
		if ( nPos == -1 )
		{
			return 0;
		}

		std::string strTemp = strReportType.substr( nPos+1, strReportType.length());
		nPos = strTemp.find( '-' );
		if ( nPos == -1 )
		{
			return 0;
		}

		std::string strDay = strTemp.substr( 0, nPos );
		strTemp = strTemp.substr( nPos+1, strTemp.length());
		nPos = strTemp.find( '-' );
		if ( nPos == -1 )
		{
			return 0;
		}

		std::string strMonth = strTemp.substr( 0, nPos );
		std::string strYear = strTemp.substr( nPos+1, strTemp.length());
		
		DWORD dwDate = atoi(strYear.c_str())*10000 + GetMonth(strMonth)*100 + atoi(strDay.c_str());
		return dwDate;
	}

	DWORD GetMonth( const std::string& month )
	{
		std::string strMonth = month;
		transform( strMonth.begin(), strMonth.end(), strMonth.begin(), toupper);
		if ( strMonth == "JAN" )
		{
			return 1;
		}
		else if ( strMonth == "FEB")
		{
			return 2;
		}
		else if ( strMonth == "MAR")
		{
			return 3;
		}
		else if ( strMonth == "APR")
		{
			return 4;
		}
		else if ( strMonth == "MAY")
		{
			return 5;
		}
		else if ( strMonth == "JUN")
		{
			return 6;
		}
		else if ( strMonth == "JUL")
		{
			return 7;
		}
		else if ( strMonth == "AUG")
		{
			return 8;
		}
		else if ( strMonth == "SEPT")
		{
			return 9;
		}
		else if ( strMonth == "OCT")
		{
			return 10;
		}
		else if ( strMonth == "NOV")
		{
			return 11;
		}
		else if ( strMonth == "DEC")
		{
			return 12;
		}
		else
		{
			return 0;
		}
	}

    //��ֹʹ�ÿ�������
    CTPReport(const CTPReport& r);
    CTPReport& operator= (const CTPReport& r);
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

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        FrontID=fid;
        SessionID=sid;
        CTools_Ansi::mymemcpy_trim(OrderRef,sizeof(OrderRef),orderRef);
    };

    OrderKey(const PlatformStru_OrderInfo& Order)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),Order.Account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Order.InstrumentID);
        FrontID=Order.FrontID;
        SessionID=Order.SessionID;
        CTools_Ansi::mymemcpy_trim(OrderRef,sizeof(OrderRef),Order.OrderRef);
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
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,FrontID%d,SessionID%d,%s",
            Account,InstrumentID,FrontID,SessionID,OrderRef);
        prlt[len]=0;
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
    TradeKey()
    {
        memset(this,0,sizeof(*this));
    };

    TradeKey(const char* account, 
                const char* instrumentID, 
                const char* tradeID, 
                const char* tradeDate, 
                const char direction,
                const char hedgeFlag)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        CTools_Ansi::mymemcpy_trim(TradeDate,sizeof(TradeDate),tradeDate);
        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),tradeID);
        Direction=direction;
        HedgeFlag=hedgeFlag;
    };

    TradeKey(const PlatformStru_TradeInfo& Trade)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),Trade.Account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Trade.InstrumentID);
        CTools_Ansi::mymemcpy_trim(TradeDate,sizeof(TradeDate),Trade.TradeDate);
        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),Trade.TradeID);
        Direction=Trade.Direction;
        HedgeFlag=Trade.HedgeFlag;
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

        if(HedgeFlag<r.HedgeFlag) return true;
        if(HedgeFlag>r.HedgeFlag) return false;
        
        if(Direction<r.Direction) return true;
        if(Direction>r.Direction) return false;
        
        irlt=strncmp(TradeID,r.TradeID,sizeof(TradeID));
        if(irlt<0) return true;
        if(irlt>0) return false;

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

        if(HedgeFlag!=r.HedgeFlag)
            return false;
        if(Direction!=r.Direction)
            return false;

        irlt=strncmp(TradeID,r.TradeID,sizeof(TradeID));
        if(irlt!=0) return false;

        return true;
    }
    bool operator!= (const TradeKey& r) const
    {
        return !(*this==r);
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

        char buf[256];
        sprintf(buf,"Direct%d HedgeFlag%d",Direction,HedgeFlag);
        ilen=strlen(buf);
        if(len+ilen<rltsize)      { strcat(prlt,buf);  len+=ilen; }
        return prlt;
    }

    CommonAccountType       Account;
    CommonInstrumentIDType  InstrumentID;      //��ԼID
    CommonDateType          TradeDate;         //�ɽ�����(����Ϊ8���ַ�����:20101023)
    CommonTradeIDType       TradeID;           //�ɽ�ID(����Ϊ20���ַ����Ҷ��룬��߲��ո�)
	char		            Direction;         //����ģ�����Գɽ���������Ϸ���
    char                    HedgeFlag;
};


///��ʾһ����Ȩ��Ȩ������¼
struct ExecOrderKey
{
    ExecOrderKey()
    {
        memset(this,0,sizeof(*this));
        FrontID=-1;
        SessionID=-1;
    };

    ExecOrderKey(const char* account,const char* instrumentID,int fid, int sid, const char* orderRef)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        FrontID=fid;
        SessionID=sid;
        CTools_Ansi::mymemcpy_trim(OrderRef,sizeof(OrderRef),orderRef);
    };

    ExecOrderKey(const PlatformStru_ExecOrderField& Order)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),Order.Account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Order.InstrumentID);
        FrontID=Order.FrontID;
        SessionID=Order.SessionID;
        CTools_Ansi::mymemcpy_trim(OrderRef,sizeof(OrderRef),Order.ExecOrderRef);
    };

    ExecOrderKey& operator= (const ExecOrderKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));
        return *this;
    };

    bool operator< (const ExecOrderKey& r) const
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

    bool operator== (const ExecOrderKey& r) const
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

    bool operator> (const ExecOrderKey& r) const
    {
        return !((*this) < r || (*this) == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,FrontID%d,SessionID%d,%s",
            Account,InstrumentID,FrontID,SessionID,OrderRef);
        prlt[len]=0;
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


///��ʾһ���ɽ�ͳ�Ƽ�¼
struct TradeTotalKey
{
    TradeTotalKey(const char* account,
                    const char* exchangeID, 
                    const char* instrumentID, 
                    const char direction, 
                    const char offsetFlag)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),account);
        CTools_Ansi::mymemcpy_trim(ExchangeID,sizeof(ExchangeID),exchangeID);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        Direction=direction;
        OffsetFlag=offsetFlag;
    };

    TradeTotalKey(const PlatformStru_TradeTotalInfo& TradeTotal)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),TradeTotal.Account);
        CTools_Ansi::mymemcpy_trim(ExchangeID,sizeof(ExchangeID),TradeTotal.ExchangeID);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),TradeTotal.InstrumentID);
        Direction=TradeTotal.Direction;
        OffsetFlag=TradeTotal.OffsetFlag;
    };

	TradeTotalKey(const PlatformStru_TradeInfo& TradeInfo)
	{
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),TradeInfo.Account);
        CTools_Ansi::mymemcpy_trim(ExchangeID,sizeof(ExchangeID),TradeInfo.ExchangeID);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),TradeInfo.InstrumentID);
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
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,%s,Direct%d,Offset%d",
            Account,ExchangeID,InstrumentID,Direction,OffsetFlag);
        prlt[len]=0;
        return prlt;
    }

    CommonAccountType           Account;
    CommonExchangeIDType        ExchangeID;
    CommonInstrumentIDType      InstrumentID;      //��ԼID
	char			            Direction;
	char			            OffsetFlag;
};


///��ʾһ���ֲ���ϸ��¼����InstrumentID��OpenDate��TradeID��Direction��TradeType����Ψһ�ĳֲ���ϸ��¼
struct PositionDetailKey
{
    PositionDetailKey()
    {
        memset(this,0,sizeof(*this));
    }
	PositionDetailKey(const char* account, 
                        const char* instrumentID, 
                        const char* openDate, 
                        const char* tradeID, 
                        char direction, 
                        char tradeType,
                        char hedgeFlag)
    {
	    set(account,instrumentID,openDate,tradeID,direction,tradeType,hedgeFlag);
    };

    PositionDetailKey(const PlatformStru_PositionDetail& PositionDetail)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),PositionDetail.Account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),PositionDetail.InstrumentID);
        CTools_Ansi::mymemcpy_trim(OpenDate,sizeof(OpenDate),PositionDetail.OpenDate);
        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),PositionDetail.TradeID);

        Direction=PositionDetail.Direction;
        TradeType=PositionDetail.TradeType;
        HedgeFlag=PositionDetail.HedgeFlag;
    };

    PositionDetailKey& operator= (const PositionDetailKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));

        return *this;
    };
	void set(const char* account,
        const char* instrumentID,
        const char* openDate,
        const char* tradeID,
        char direction,
        char tradeType,
        char hedgeFlag)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        CTools_Ansi::mymemcpy_trim(OpenDate,sizeof(OpenDate),openDate);
        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),tradeID);

        Direction=direction;
        TradeType=tradeType;
        HedgeFlag=hedgeFlag;
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

        irlt=CTools_Ansi::mycompare_trynumeric(TradeID,r.TradeID);
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(Direction<r.Direction)
			return true;
		if(Direction>r.Direction)
			return false;

        if(TradeType<r.TradeType)
			return true;
		if(TradeType>r.TradeType) 
			return false;

        if(HedgeFlag<r.HedgeFlag)
			return true;
		if(HedgeFlag>r.HedgeFlag) 
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
        if(TradeType!=r.TradeType)
            return false;
        if(HedgeFlag!=r.HedgeFlag)
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
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,%s,%s,Direct%d,TradeType%d,HedgeFlag%d",
            Account,InstrumentID,OpenDate,TradeID,Direction,TradeType,HedgeFlag);
        prlt[len]=0;
        return prlt;
    }


	CommonAccountType       Account;		    //�˺�
    CommonInstrumentIDType  InstrumentID;       //��ԼID
    CommonDateType          OpenDate;           //��������
    CommonTradeIDType       TradeID;            //�ɽ�ID(����Ϊ20���ַ����Ҷ��룬��߲��ո�)
    char                    Direction;          //��������
    char                    TradeType;          //�ɽ����ͣ�'0'/0x0: ��ͨ�ɽ��� '4': ��������ɽ�
    char                    HedgeFlag;          //Ͷ���ױ���־
};


///��ʾһ���ֲּ�¼����InstrumentID��Direction��HedgeFlag����Ψһ�ĳֲ���ϸ��¼
struct PositionKey
{
    PositionKey()
    {
        memset(this,0,sizeof(*this));
    };
	PositionKey(const char* account, const char* instrumentID, const char& posiDirection, const char& hedgeFlag)
    {
        set(account, instrumentID, posiDirection, hedgeFlag);
    }
	void set(const char* account, const char* instrumentID, const char& posiDirection, const char& hedgeFlag)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        PosiDirection=posiDirection;
        HedgeFlag=hedgeFlag;
    };

    PositionKey(const PlatformStru_Position& Position)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),Position.Account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Position.InstrumentID);
        PosiDirection=Position.PosiDirection;
        HedgeFlag=Position.HedgeFlag;
    };

	PositionKey(const PlatformStru_OrderInfo& Order)
	{
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),Order.Account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Order.InstrumentID);
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
	PositionKey(const PlatformStru_TradeInfo& Trade)
	{
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),Trade.Account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Trade.InstrumentID);
		if(Trade.OffsetFlag==THOST_FTDC_OF_Open)
            PosiDirection = Trade.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
		else if(Trade.OffsetFlag==THOST_FTDC_OF_Close||
				Trade.OffsetFlag==THOST_FTDC_OF_ForceClose||
				Trade.OffsetFlag==THOST_FTDC_OF_CloseToday||
				Trade.OffsetFlag==THOST_FTDC_OF_CloseYesterday||
				Trade.OffsetFlag==THOST_FTDC_OF_LocalForceClose)
			PosiDirection = Trade.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Short : THOST_FTDC_PD_Long;
		else 
			PosiDirection = Trade.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
		HedgeFlag=Trade.HedgeFlag;
	};
	PositionKey(const PlatformStru_PositionDetail& PositionDetail)
	{
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),PositionDetail.Account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),PositionDetail.InstrumentID);
        PosiDirection = PositionDetail.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
		HedgeFlag=PositionDetail.HedgeFlag;
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

        CTools_Ansi::mygettrimpos(PositionDetail.Account,off,len);
        if(strlen(Account)!=len||memcmp(Account,PositionDetail.Account,len)!=0)
            return false;

        CTools_Ansi::mygettrimpos(PositionDetail.InstrumentID,off,len);
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
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,Direct%d,Hedge%d",
            Account,InstrumentID,PosiDirection,HedgeFlag);
        prlt[len]=0;
        return prlt;
    }

	CommonAccountType       Account;			//�˺�
    CommonInstrumentIDType  InstrumentID;      //��ԼID
    char                    PosiDirection;     //��������
    char                    HedgeFlag;         //Ͷ���ױ���־
};

///��ʾһ����ϳֲ���ϸ��¼����Account��InstrumentID��OpenDate��TradeID��ComTradeID, Direction��hedgeFlag����Ψһ����ϳֲ���ϸ��¼
struct PositionDetailCombKey
{
    PositionDetailCombKey()
    {
        memset(this,0,sizeof(*this));
    }
	PositionDetailCombKey(const char* account,
        const char* instrumentID,
        const char* openDate,
        const char* tradeID,
        const char* comTradeID,
        char direction,
        char hedgeFlag)
    {
        set(account,instrumentID,openDate,tradeID,comTradeID,direction,hedgeFlag);
    };

    PositionDetailCombKey(const PlatformStru_PositionDetailComb& r)
    {
        new(this) PositionDetailCombKey(r.Account,r.InstrumentID,r.OpenDate,r.TradeID,r.ComTradeID,r.Direction,r.HedgeFlag);
    };

	void set(const char* account,
        const char* instrumentID,
        const char* openDate,
        const char* tradeID,
        const char* comTradeID,
        char direction,
        char hedgeFlag)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        CTools_Ansi::mymemcpy_trim(OpenDate,sizeof(OpenDate),openDate);
        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),tradeID);
        CTools_Ansi::mymemcpy_trim(ComTradeID,sizeof(ComTradeID),comTradeID);

        Direction=direction;
        HedgeFlag=hedgeFlag;
    };

    PositionDetailCombKey& operator= (const PositionDetailCombKey& r)
    {
        memcpy((char*)this,(char*)&r,sizeof(*this));

        return *this;
    };


    //ע�⣬ͨ��OpenDate��TradeID�ıȽ�˳�򣬱�֤set<PositionDetailCombKey>����ͬ�����ĳֲ���ϸ����ʱ������
    bool operator< (const PositionDetailCombKey& r) const
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

        if(HedgeFlag<r.HedgeFlag)
			return true;
		if(HedgeFlag>r.HedgeFlag)
			return false;

        if(Direction<r.Direction)
			return true;
		if(Direction>r.Direction)
			return false;

        irlt=CTools_Ansi::mycompare_trynumeric(TradeID,r.TradeID);
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=CTools_Ansi::mycompare_trynumeric(ComTradeID,r.ComTradeID);
        if(irlt<0) return true;
        if(irlt>0) return false;

        return false;
    }

    bool operator== (const PositionDetailCombKey& r) const
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

        irlt=strncmp(ComTradeID,r.ComTradeID,sizeof(ComTradeID));
        if(irlt!=0) return false;

        if(Direction!=r.Direction)
            return false;
        if(HedgeFlag!=r.HedgeFlag)
            return false;

        return true;
    }
    
    bool operator!= (const PositionDetailCombKey& r) const
    {
        return !(*this == r);
    }
    
    bool operator> (const PositionDetailCombKey& r) const
    {
        return !(*this < r ||*this == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,%s,%s,%s,Direct%d,HedgeFlag%d",
            Account,InstrumentID,OpenDate,TradeID,ComTradeID,Direction,HedgeFlag);
        prlt[len]=0;
        return prlt;
    }


	CommonAccountType       Account;		    //�˺�
    CommonInstrumentIDType  InstrumentID;       //��ԼID
    CommonDateType          OpenDate;           //��������
    CommonTradeIDType       TradeID;            //�ɽ�ID(����Ϊ20���ַ����Ҷ��룬��߲��ո�)
	CommonTradeIDType       ComTradeID;         //��ϳɽ�ID�����ڲ�ѯ��������ϳֲ���ϸ����ctp�����������гɽ������ģ���OrderSysID_xxxx��ʱ���壬һ��OrderSysID���ܶ�Ӧ�����ϳֲ���ϸ��
    char                    Direction;          //��������
    char                    HedgeFlag;          //Ͷ���ױ���־
};

//��Account��InstrumentID������Keyֵ
struct Account_InstrumentKey
{
    Account_InstrumentKey()
    {
        memset(this,0,sizeof(*this));
    }
    Account_InstrumentKey(const char* account,const char* instrumentID)
    {
        memset(this,0,sizeof(*this));

        if(account) CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),account);
        if(instrumentID) CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
    };
    Account_InstrumentKey& operator= (const Account_InstrumentKey& r)
    {
        if (this == &r) return *this;
        memcpy((char*)this,(char*)&r,sizeof(*this));
        return *this;
    };

    bool operator< (const Account_InstrumentKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        return false;
    }

    bool operator== (const Account_InstrumentKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        return true;
    }
    bool operator!= (const Account_InstrumentKey& r) const
    {
        return !(*this==r);
    }
    
    bool operator> (const Account_InstrumentKey& r) const
    {
        return !(*this < r ||*this == r);
    }

    CommonAccountType       Account;
    CommonInstrumentIDType  InstrumentID;      //��ԼID
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
	int CurSvrGrpID;    //��ǰ�ķ�������ID
	//��������������������
	std::string ProxyType;//socks4,socks5,http
	std::string ProxyAddr;
	std::string ProxyPort;
	std::string ProxyUser;
	std::string ProxyPwd;
}SERVER_PARAM_CFG,* LPSERVER_PARAM_CFG;
/************����������**end********************************************************************************************************/


#ifdef WIN32
#pragma managed(pop)
#endif 
