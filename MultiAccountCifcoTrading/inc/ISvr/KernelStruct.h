#pragma once

#define DOUBLEINVALIDVALUE	(1.7976931348623158e+308)
#define INTINVALIDVALUE		(0xfeeefeee)

#define ORDERCLASS_NewOrder 'N'
#define ORDERCLASS_ReplaceOrder 'R'

typedef char CommonAccountIDType[13];

typedef char CommonOrderClassType;
typedef char CommonUserIDType[32];                  //�û���ID����Ӧ�ۺϽ���ƽ̨Ͷ����ID��Pats��UserName
typedef char CommonAccountType[32];                 //�˺ţ���ӦPats��Account
typedef char CommonLocalRequestIDType[32];
typedef char CommonLocalRefIDType[32];
typedef char CommonExchangeIDType[32];
typedef char CommonExchangeNameType[32];            //����������
typedef char CommonCurrencyType[16];                //��������
typedef char CommonOrderRefType[32];                //������OrderRef
typedef char CommonOrderSysIDType[32];              //������OrderSysID
typedef char CommonInstrumentIDType[32];            //��ԼID
typedef char CommonDateType[16];                    //����
typedef char CommonTradeIDType[32];                 //�ɽ�ID

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

///��Լ��Ϣ
struct PlatformStru_InstrumentInfo
{
	///��Լ����
	InstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ����
	InstrumentIDType	InstrumentName;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///��Ʒ����
	InstrumentIDType	ProductID;
	///��Ʒ����
	TThostFtdcProductClassType	ProductClass;
	///�������
	TThostFtdcYearType	DeliveryYear;
	///������
	TThostFtdcMonthType	DeliveryMonth;
	///�м۵�����µ���
	TThostFtdcVolumeType	MaxMarketOrderVolume;
	///�м۵���С�µ���
	TThostFtdcVolumeType	MinMarketOrderVolume;
	///�޼۵�����µ���
	TThostFtdcVolumeType	MaxLimitOrderVolume;
	///�޼۵���С�µ���
	TThostFtdcVolumeType	MinLimitOrderVolume;
	///��Լ��������
	TThostFtdcVolumeMultipleType	VolumeMultiple;
	///��С�䶯��λ
	TThostFtdcPriceType	PriceTick;
	///������
	TThostFtdcDateType	CreateDate;
	///������
	TThostFtdcDateType	OpenDate;
	///������
	TThostFtdcDateType	ExpireDate;
	///��ʼ������
	TThostFtdcDateType	StartDelivDate;
	///����������
	TThostFtdcDateType	EndDelivDate;
	///��Լ��������״̬
	TThostFtdcInstLifePhaseType	InstLifePhase;
	///��ǰ�Ƿ���
	TThostFtdcBoolType	IsTrading;
	///�ֲ�����
	TThostFtdcPositionTypeType	PositionType;
	///�ֲ���������
	TThostFtdcPositionDateTypeType	PositionDateType;
	///��ͷ��֤����
	TThostFtdcRatioType	LongMarginRatio;
	///��ͷ��֤����
	TThostFtdcRatioType	ShortMarginRatio;

	char Currency [11];
	int  TicksPerPoint;
	char TickSize[10];
};

///��������Ϣ
struct PlatformStru_InstrumentCommissionRate
{
	//CCifcoInstrumentCommissionRateField Thost;
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
};

///��֤����Ϣ
struct PlatformStru_InstrumentMarginRate
{
	// CCifcoInstrumentMarginRateField Thost;
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
};

///�����˺���Ϣ
struct PlatformStru_TradingAccountInfo
{
	// CCifcoTradingAccountFieldEx Thost;
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
	CommonAccountType szAccount;

	//Ӷ��
	double TradeCommission;

    double                  PriceTick;                      //��InstrumentInfo��ȡ��

	//FasttraderID,����ÿһ����¼��Key������һ��Ψһ���
	long FTID;                                                
	//ÿ���������ݷ����仯��UpdateSeq��һ
	long UpdateSeq;


	PlatformStru_TradeInfo()
	{
		Clear();
	}
	void Clear(void)
	{
		memset(this,0,sizeof(*this));
		TradeCommission=DOUBLEINVALIDVALUE;
		FTID=0;
		UpdateSeq=0;
	}
	char* tostring(char*prlt,int rltsize) const;

};

// �ɽ�ͳ�ƽṹ
struct PlatformStru_TradeTotalInfo
{
	//�˺�
	CommonAccountType szAccount;
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
	char* tostring(char*prlt,int rltsize) const;
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
	CommonLocalRequestIDType	strLocalRequestID;			// ���ص�������
	CommonAccountType		szAccount;					// �����˺�
	CommonExchangeIDType		strExchange;				// ����������

	TThostFtdcPriceType      freezeMargin;              //���ᱣ֤��
	TThostFtdcPriceType      troubleMoney;              //����������

    double                  PriceTick;                      //��InstrumentInfo��ȡ��

	//FasttraderID,����ÿһ����¼��Key������һ��Ψһ���
	long FTID;         
	//ÿ���������ݷ����仯��UpdateSeq��һ
	long UpdateSeq;

	bool bInvalid;      //��ʾ�Ƿ���Ч
	static PlatformStru_OrderInfo InvalidOrderInfo;

	PlatformStru_OrderInfo()
	{
		Clear();
	}
	PlatformStru_OrderInfo(bool binvalid)
	{
		Clear();
		bInvalid=binvalid;
	}
	void Clear(void)
	{
		memset(this,0,sizeof(*this));
		freezeMargin=DOUBLEINVALIDVALUE;
		troubleMoney=DOUBLEINVALIDVALUE;

		FTID=0;
		UpdateSeq=0;
		bInvalid=false;
	}
	static PlatformStru_OrderInfo& GetInvalidRecord(void)
	{
		return InvalidOrderInfo;
	}
	bool IsInvalidRecord(void) const
	{
		return bInvalid;
	}

	char* tostring(char*prlt,int rltsize) const;
};

///������Ϣ
struct PlatformStru_DepthMarketData
{
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

///�����û�������Ϣ
struct CCifcoUserPasswordUpdateField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///ԭ���Ŀ���
	TThostFtdcPasswordType	OldPassword;
	///�µĿ���
	TThostFtdcPasswordType	NewPassword;
};
struct PlatformStru_UserPasswordUpdate
{
	CCifcoUserPasswordUpdateField Thost;
};
///�����û��ʽ��˻�������Ϣ
struct CCifcoTradingAccountPasswordUpdateField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///ԭ���Ŀ���
	TThostFtdcPasswordType	OldPassword;
	///�µĿ���
	TThostFtdcPasswordType	NewPassword;
};
struct PlatformStru_TradingAccountPasswordUpdate
{
	CCifcoTradingAccountPasswordUpdateField Thost;
};
///¼�뱨����Ϣ
struct PlatformStru_InputOrder
{
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

	CommonAccountType		strAccount;					// �����˺�
	CommonLocalRequestIDType	strLocalRequestID;			// ���ص�������
	CommonLocalRefIDType		strLocalRefID;				// ���ص����úű��
	CommonExchangeIDType		strExchange;				// ����������
};

///¼��Ԥ����Ϣ
struct PlatformStru_ParkedOrder
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

///¼��Ԥ�񳷵���Ϣ
struct CCifcoParkedOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������������
	TThostFtdcOrderActionRefType	OrderActionRef;
	///��������
	TThostFtdcOrderRefType	OrderRef;
	///������
	TThostFtdcRequestIDType	RequestID;
	///ǰ�ñ��
	TThostFtdcFrontIDType	FrontID;
	///�Ự���
	TThostFtdcSessionIDType	SessionID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///������־
	TThostFtdcActionFlagType	ActionFlag;
	///�۸�
	TThostFtdcPriceType	LimitPrice;
	///�����仯
	TThostFtdcVolumeType	VolumeChange;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///Ԥ�񳷵������
	TThostFtdcParkedOrderActionIDType	ParkedOrderActionID;
	///�û�����
	TThostFtdcUserTypeType	UserType;
	///Ԥ�񳷵�״̬
	TThostFtdcParkedOrderStatusType	Status;
	///�������
	TThostFtdcErrorIDType	ErrorID;
	///������Ϣ
	TThostFtdcErrorMsgType	ErrorMsg;
};
struct PlatformStru_ParkedOrderAction
{
	CCifcoParkedOrderActionField Thost;
};
///����������Ϣ
struct CCifcoInputOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������������
	TThostFtdcOrderActionRefType	OrderActionRef;
	///��������
	TThostFtdcOrderRefType	OrderRef;
	///������
	TThostFtdcRequestIDType	RequestID;
	///ǰ�ñ��
	TThostFtdcFrontIDType	FrontID;
	///�Ự���
	TThostFtdcSessionIDType	SessionID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///������־
	TThostFtdcActionFlagType	ActionFlag;
	///�۸�
	TThostFtdcPriceType	LimitPrice;
	///�����仯
	TThostFtdcVolumeType	VolumeChange;
	///�û�����
	TThostFtdcUserIDType	UserID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};
struct PlatformStru_InputOrderAction
{
	CCifcoInputOrderActionField Thost;
};
///��ѯ��󱨵�����
struct CCifcoQueryMaxOrderVolumeField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///��������
	TThostFtdcDirectionType	Direction;
	///��ƽ��־
	TThostFtdcOffsetFlagType	OffsetFlag;
	///Ͷ���ױ���־
	TThostFtdcHedgeFlagType	HedgeFlag;
	///�������������
	TThostFtdcVolumeType	MaxVolume;
};
struct PlatformStru_QueryMaxOrderVolume
{
	CCifcoQueryMaxOrderVolumeField Thost;
};
///����ɾ��Ԥ��
struct CCifcoRemoveParkedOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///Ԥ�񱨵����
	TThostFtdcParkedOrderIDType	ParkedOrderID;
};
struct PlatformStru_RemoveParkedOrder
{
	CCifcoRemoveParkedOrderField Thost;
};
///����ɾ��Ԥ�񳷵�
struct CCifcoRemoveParkedOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///Ԥ�񳷵����
	TThostFtdcParkedOrderActionIDType	ParkedOrderActionID;
};
struct PlatformStru_RemoveParkedOrderAction
{
	CCifcoRemoveParkedOrderActionField Thost;
};
///�����ѯ����
struct CCifcoQryOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///��ʼʱ��
	TThostFtdcTimeType	InsertTimeStart;
	///����ʱ��
	TThostFtdcTimeType	InsertTimeEnd;
};
struct PlatformStru_QryOrder
{
	CCifcoQryOrderField Thost;
};
///�����ѯ�ɽ�
struct CCifcoQryTradeField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�ɽ����
	TThostFtdcTradeIDType	TradeID;
	///��ʼʱ��
	TThostFtdcTimeType	TradeTimeStart;
	///����ʱ��
	TThostFtdcTimeType	TradeTimeEnd;
};
struct PlatformStru_QryTrade
{
	PlatformStru_QryTrade()
	{
		memset(this,0,sizeof(*this));
	};
	CCifcoQryTradeField Thost;
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
	CommonAccountType szAccount;

	double      CurPrice;                   //�ּ�


	//����ֲ�ӯ���õ��ּ�
	double      CurPriceUsedByCalcu;                   

    double                  PriceTick;                      //��InstrumentInfo��ȡ��
    bool                    bToday;                         //�Ƿ���ճֲ���ϸ

	//FasttraderID,����ÿһ����¼��Key������һ��Ψһ���
	long FTID;                                                
	//ÿ���������ݷ����仯��UpdateSeq��һ
	long UpdateSeq;



	bool bInvalid;      //��ʾ�Ƿ���Ч
	static PlatformStru_PositionDetail InvalidPositionDetail;

	PlatformStru_PositionDetail()
	{
		Clear();
	}
	PlatformStru_PositionDetail(bool binvalid)
	{
		Clear();
		bInvalid=binvalid;
	}

	void Clear()
	{
		ZeroMemory(this, sizeof(PlatformStru_PositionDetail));
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
		CurPriceUsedByCalcu = dInvalidDouble;
		FTID=0;
		UpdateSeq=0;
		bInvalid=false;
	}
	static PlatformStru_PositionDetail& GetInvalidRecord(void)
	{
		return InvalidPositionDetail;
	}
	bool IsInvalidRecord(void) const
	{
		return bInvalid;
	}
	char* tostring(char*prlt,int rltsize) const;
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
	TThostFtdcVolumeType	CombPosition;
	///��϶�ͷ����
	TThostFtdcVolumeType	CombLongFrozen;
	///��Ͽ�ͷ����
	TThostFtdcVolumeType	CombShortFrozen;

	CommonAccountType szAccount;		//��Ӧ�˺�
	TThostFtdcExchangeIDType   ExchangeID;  //����������
	double PositionProfitByTrade;			//����ӯ�������гֲ���ϸ��ӯ֮�ͣ�������ʶԳ�ĳֲ�ӯ��
	double TotalPositionProfitByDate;		//������ӯ��������ƽ��ӯ���ͳֲ�ӯ��֮��

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

	double      CurPrice;                   //�ּ�

	//����ֲ�ӯ���õ��ּ�
	double      CurPriceUsedByCalcu;  

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
	static PlatformStru_Position InvalidPosition;

	PlatformStru_Position()
	{
		Clear();
	}
	PlatformStru_Position(bool binvalid)
	{
		Clear();
		bInvalid=binvalid;
	}

	void Clear()
	{
		ZeroMemory(this, sizeof(PlatformStru_Position));
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
		CurPriceUsedByCalcu=dInvalidDouble;
		FTID=0;
		UpdateSeq=0;
		bInvalid=false;
	}
	static PlatformStru_Position& GetInvalidRecord(void)
	{
		return InvalidPosition;
	}
	bool IsInvalidRecord(void) const
	{
		return bInvalid;
	}
	char* tostring(char*prlt,int rltsize) const;
};



//��ϳֲ���ϸ
struct PlatformStru_PositionComb
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

	///Ͷ���ױ���־
	TThostFtdcHedgeFlagType	HedgeFlag;
	///����
	TThostFtdcDirectionType	Direction;
	//���ּ�
	TThostFtdcPriceType	OpenPrice;
	///�ֲ���
	TThostFtdcVolumeType	TotalAmt;

	//����1����Ϣ
	///��ϱ��
	TThostFtdcTradeIDType	Leg1TradeID;
	///��Լ����
	TThostFtdcInstrumentIDType	Leg1InstrumentID;
	///Ͷ���߱�֤��
	TThostFtdcMoneyType	Leg1Margin;
	///��������֤��
	TThostFtdcMoneyType	Leg1ExchMargin;
	///��֤����
	TThostFtdcRatioType	Leg1MarginRateByMoney;
	///��֤����(������)
	TThostFtdcRatioType	Leg1MarginRateByVolume;
	///���ȱ��
	TThostFtdcLegIDType	Leg1ID;
	///���ȳ���
	TThostFtdcLegMultipleType	Leg1Multiple;

	//����2����Ϣ
	///��ϱ��
	TThostFtdcTradeIDType	Leg2TradeID;
	///��Լ����
	TThostFtdcInstrumentIDType	Leg2InstrumentID;
	///Ͷ���߱�֤��
	TThostFtdcMoneyType	Leg2Margin;
	///��������֤��
	TThostFtdcMoneyType	Leg2ExchMargin;
	///��֤����
	TThostFtdcRatioType	Leg2MarginRateByMoney;
	///��֤����(������)
	TThostFtdcRatioType	Leg2MarginRateByVolume;
	///���ȱ��
	TThostFtdcLegIDType	Leg2ID;
	///���ȳ���
	TThostFtdcLegMultipleType	Leg2Multiple;

	///��ϳֲֺ�Լ����
	TThostFtdcInstrumentIDType	InstrumentID;

	CommonAccountType szAccount;	//��Ӧ�˺�

	//FasttraderID,����ÿһ����¼��Key������һ��Ψһ���
	long FTID;                                                
	//ÿ���������ݷ����仯��UpdateSeq��һ
	long UpdateSeq;


	bool bInvalid;      //��ʾ�Ƿ���Ч
	static PlatformStru_PositionComb InvalidPositionDetailComb;

	PlatformStru_PositionComb()
	{
		Clear();
	}
	PlatformStru_PositionComb(bool binvalid)
	{
		Clear();
		bInvalid=binvalid;
	}

	void Clear()
	{
		ZeroMemory(this, sizeof(PlatformStru_PositionComb));
		int nInvalidInt = INTINVALIDVALUE;
		double dInvalidDouble = DOUBLEINVALIDVALUE;
		TotalAmt = nInvalidInt;

		Leg1Margin = dInvalidDouble;
		Leg1ExchMargin = dInvalidDouble;
		Leg1MarginRateByMoney = dInvalidDouble;
		Leg1MarginRateByVolume = dInvalidDouble;
		Leg1Multiple = nInvalidInt;

		Leg2Margin = dInvalidDouble;
		Leg2ExchMargin = dInvalidDouble;
		Leg2MarginRateByMoney = dInvalidDouble;
		Leg2MarginRateByVolume = dInvalidDouble;
		Leg2Multiple = nInvalidInt;

		FTID=0;
		UpdateSeq=0;
		bInvalid=false;
	}
	static PlatformStru_PositionComb& GetInvalidRecord(void)
	{
		return InvalidPositionDetailComb;
	}
	bool IsInvalidRecord(void) const
	{
		return bInvalid;
	}
	char* tostring(char*prlt,int rltsize) const;
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

///�����ѯͶ����
struct CCifcoQryInvestorField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
};
struct PlatformStru_QryInvestor
{
	CCifcoQryInvestorField Thost;
};
///�����ѯ��Լ
struct CCifcoQryInstrumentField
{
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///��Լ�ڽ������Ĵ���
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///��Ʒ����
	TThostFtdcInstrumentIDType	ProductID;
};
struct PlatformStru_QryInstrument
{
	CCifcoQryInstrumentField Thost;
};
///�����ѯ����
struct CCifcoQryDepthMarketDataField
{
	TThostFtdcInstrumentIDType	InstrumentID;
};
struct PlatformStru_QryDepthMarketData
{
	CCifcoQryDepthMarketDataField Thost;
};
///�����ѯͶ���߽�����
struct CCifcoQrySettlementInfoField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///������
	TThostFtdcDateType	TradingDay;
};
struct PlatformStru_QrySettlementInfo
{
	CCifcoQrySettlementInfoField Thost;
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

///�����ѯ�ͻ�֪ͨ
struct CCifcoQryNoticeField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
};
struct PlatformStru_QryNotice
{
	CCifcoQryNoticeField Thost;
};
///�����ѯ������Ϣȷ��
struct CCifcoQrySettlementInfoConfirmField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
};
struct PlatformStru_QrySettlementInfoConfirm
{
	CCifcoQrySettlementInfoConfirmField Thost;
};

///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
struct CCifcoQryCFMMCTradingAccountKeyField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
};
///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
struct PlatformStru_QryCFMMCTradingAccountKey
{
	CCifcoQryCFMMCTradingAccountKeyField Thost;
};
///�����ѯת������
struct CCifcoQryTransferBankField
{
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з����Ĵ���
	TThostFtdcBankBrchIDType	BankBrchID;
};
struct PlatformStru_QryTransferBank
{
	CCifcoQryTransferBankField Thost;
};
///�����ѯת����ˮ
struct CCifcoQryTransferSerialField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///���б���
	TThostFtdcBankIDType	BankID;
};
struct PlatformStru_QryTransferSerial
{
	CCifcoQryTransferSerialField Thost;
};
///�����ѯǩԼ����
struct CCifcoQryContractBankField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з����Ĵ���
	TThostFtdcBankBrchIDType	BankBrchID;
};
struct PlatformStru_QryContractBank
{
	CCifcoQryContractBankField Thost;
};
///�����ѯ����ǩԼ��ϵ
struct CCifcoQryAccountregisterField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///���б���
	TThostFtdcBankIDType	BankID;
};
struct PlatformStru_QryAccountRegister
{
	CCifcoQryAccountregisterField Thost;
};
///�����ѯԤ��
struct CCifcoQryParkedOrderField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
};
///�����ѯԤ��
struct PlatformStru_QryParkedOrder
{
	CCifcoQryParkedOrderField Thost;
};
///�����ѯԤ�񳷵�
struct CCifcoQryParkedOrderActionField
{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
};
struct PlatformStru_QryParkedOrderAction
{
	CCifcoQryParkedOrderActionField Thost;
};
///�ڻ������ѯ�����������
struct CCifcoReqQueryAccountField
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	TThostFtdcSerialType	PlateSerial;
	///����Ƭ��־
	TThostFtdcLastFragmentType	LastFragment;
	///�Ự��
	TThostFtdcSessionIDType	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	TThostFtdcIdCardTypeType	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�ͻ�����
	TThostFtdcCustTypeType	CustType;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///�ڻ���˾��ˮ��
	TThostFtdcFutureSerialType	FutureSerial;
	///��װ���
	TThostFtdcInstallIDType	InstallID;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///��֤�ͻ�֤�������־
	TThostFtdcYesNoIndicatorType	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�����ʺ�����
	TThostFtdcBankAccTypeType	BankAccType;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���λ�ʺ�����
	TThostFtdcBankAccTypeType	BankSecuAccType;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///�ڻ���λ�ʺ�
	TThostFtdcBankAccountType	BankSecuAcc;
	///���������־
	TThostFtdcPwdFlagType	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	TThostFtdcPwdFlagType	SecuPwdFlag;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	TThostFtdcRequestIDType	RequestID;
	///����ID
	TThostFtdcTIDType	TID;
};

struct PlatformStru_ReqQueryAccount
{
	CCifcoReqQueryAccountField Thost;
};
///�ڻ����������ʽ�ת�ڻ�����
struct  CCifcoReqTransferField 
{
	///ҵ������
	TThostFtdcTradeCodeType	TradeCode;
	///���д���
	TThostFtdcBankIDType	BankID;
	///���з�֧��������
	TThostFtdcBankBrchIDType	BankBranchID;
	///���̴���
	TThostFtdcBrokerIDType	BrokerID;
	///���̷�֧��������
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///��������
	TThostFtdcTradeDateType	TradeDate;
	///����ʱ��
	TThostFtdcTradeTimeType	TradeTime;
	///������ˮ��
	TThostFtdcBankSerialType	BankSerial;
	///����ϵͳ���� 
	TThostFtdcTradeDateType	TradingDay;
	///����ƽ̨��Ϣ��ˮ��
	TThostFtdcSerialType	PlateSerial;
	///����Ƭ��־
	TThostFtdcLastFragmentType	LastFragment;
	///�Ự��
	TThostFtdcSessionIDType	SessionID;
	///�ͻ�����
	TThostFtdcIndividualNameType	CustomerName;
	///֤������
	TThostFtdcIdCardTypeType	IdCardType;
	///֤������
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///�ͻ�����
	TThostFtdcCustTypeType	CustType;
	///�����ʺ�
	TThostFtdcBankAccountType	BankAccount;
	///��������
	TThostFtdcPasswordType	BankPassWord;
	///Ͷ�����ʺ�
	TThostFtdcAccountIDType	AccountID;
	///�ڻ�����
	TThostFtdcPasswordType	Password;
	///��װ���
	TThostFtdcInstallIDType	InstallID;
	///�ڻ���˾��ˮ��
	TThostFtdcFutureSerialType	FutureSerial;
	///�û���ʶ
	TThostFtdcUserIDType	UserID;
	///��֤�ͻ�֤�������־
	TThostFtdcYesNoIndicatorType	VerifyCertNoFlag;
	///���ִ���
	TThostFtdcCurrencyIDType	CurrencyID;
	///ת�ʽ��
	TThostFtdcTradeAmountType	TradeAmount;
	///�ڻ���ȡ���
	TThostFtdcTradeAmountType	FutureFetchAmount;
	///����֧����־
	TThostFtdcFeePayFlagType	FeePayFlag;
	///Ӧ�տͻ�����
	TThostFtdcCustFeeType	CustFee;
	///Ӧ���ڻ���˾����
	TThostFtdcFutureFeeType	BrokerFee;
	///���ͷ������շ�����Ϣ
	TThostFtdcAddInfoType	Message;
	///ժҪ
	TThostFtdcDigestType	Digest;
	///�����ʺ�����
	TThostFtdcBankAccTypeType	BankAccType;
	///������־
	TThostFtdcDeviceIDType	DeviceID;
	///�ڻ���λ�ʺ�����
	TThostFtdcBankAccTypeType	BankSecuAccType;
	///�ڻ���˾���б���
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///�ڻ���λ�ʺ�
	TThostFtdcBankAccountType	BankSecuAcc;
	///���������־
	TThostFtdcPwdFlagType	BankPwdFlag;
	///�ڻ��ʽ�����˶Ա�־
	TThostFtdcPwdFlagType	SecuPwdFlag;
	///���׹�Ա
	TThostFtdcOperNoType	OperNo;
	///������
	TThostFtdcRequestIDType	RequestID;
	///����ID
	TThostFtdcTIDType	TID;
	///ת�˽���״̬
	TThostFtdcTransferStatusType	TransferStatus;
};
struct PlatformStru_ReqTransfer
{
	CCifcoReqTransferField Thost;
};
//�˺���Ϣ
struct PlatformStru_Trader
{
	char TraderAccount[21];
	char BackOfficeID[21];
	CHAR Tradable;
	INT LossLimit;
};
//��������Ϣ
struct PlatformStru_Exchange
{
	char Name[11];
	CHAR QueryEnabled;
	CHAR AmendEnabled;
	INT Strategy;
	CHAR CustomDecs;
	INT Decimals;
	CHAR TicketType;
	CHAR RFQA;
	CHAR RFQT;
	CHAR EnableBlock;
	CHAR EnableBasis;
	CHAR EnableAA;
	CHAR EnableCross;
	INT GTStatus;

};
//��Լ��Ϣ
typedef CHAR Array10[11];
typedef Array10 LegStruct[5];
struct PlatformStru_Contract
{
	char  ContractName[11];
	char ContractDate[51];
	char ExchangeName[11];
	char ExpiryDate[9];
	char LastTradeDate[9];
	INT NumberOfLegs;
	INT TicksPerPoint;
	char TickSize[11];
	CHAR Tradable;
	INT GTStatus;
	char Margin[21];
	CHAR ESATemplate;
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
	INT TicksPerPoint;
	Array10 TickSize;
	INT GTStatus;
};
//��������
struct PlatformStru_OrderType
{
	Array10 OrderType;
	Array10 ExchangeName;
	INT OrderTypeID;
	BYTE NumPricesReqd;
	BYTE NumVolumesReqd;
	BYTE NumDatesReqd;
	CHAR AutoCreated;
	CHAR TimeTriggered;
	CHAR RealSynthetic;
	CHAR GTCFlag;
	char TicketType[3];
	CHAR PatsOrderType;
	INT AmendOTCount;
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

#pragma pack(push)
#pragma pack(1)
struct PlatformStru_AccountPosition
{
	CHAR Profit[21];
	INT Buys;
	INT Sells;
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

///��Ӧ��Ϣ
struct CCifcoRspInfoField
{
    ///�������
    TThostFtdcErrorIDType	ErrorID;
    ///������Ϣ
    TThostFtdcErrorMsgType	ErrorMsg;
};

struct PlatformStru_RspOrderAction1
{
	CCifcoInputOrderActionField InputOrderActionField;
	CCifcoRspInfoField     RspInfoField; 
};

struct PlatformStru_RspOrderAction2
{
	CCifcoInputOrderActionField OrderActionField;
	CCifcoRspInfoField     RspInfoField;  
};

struct PlatformStru_RspOrderInsert
{
	CCifcoInputOrderActionField InputOrderInsertField;
	CCifcoRspInfoField     RspInfoField; 
};