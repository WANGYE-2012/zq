
/////////////////////////////////////////////////////////////////////////
///@system ��һ��������ϵͳ
///@company �Ϻ��ڻ���Ϣ�������޹�˾
///@file FtdcStruct.h
///@brief �����˿ͻ��˽ӿ�ʹ�õ�ҵ�����ݽṹ
///@history 
///20070227	zhangjie		�������ļ�
/////////////////////////////////////////////////////////////////////////

#if !defined(_USERAPISTRUCT_H)
#define _USERAPISTRUCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <math.h>
#include <string.h>
#include "UserApiDataType.h"

#ifndef SMALL_DOUBLE
#define SMALL_DOUBLE	0.0000001
#endif

#ifndef EQUALDOUBLE
#define EQUALDOUBLE(x,y)	(fabs((x)-(y)) < SMALL_DOUBLE)
#endif


///��Ϣ�ַ�
struct DisseminationField
{
	///����ϵ�к�
	TSequenceSeriesType	SequenceSeries;
	///���к�
	TSequenceNoType	SequenceNo;

bool operator==(const DisseminationField &ct) const
{
	bool ret = true;
	ret = ret && (SequenceSeries == ct.SequenceSeries);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;

	return ret;
}
};

///�û���¼����
struct ReqUserLoginField
{
	///������
	TDateType	TradingDay;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///����
	TPasswordType	Password;
	///�û��˲�Ʒ��Ϣ
	TProductInfoType	UserProductInfo;
	///�ӿڶ˲�Ʒ��Ϣ
	TProductInfoType	InterfaceProductInfo;
	///Э����Ϣ
	TProtocolInfoType	ProtocolInfo;

bool operator==(const ReqUserLoginField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserProductInfo, ct.UserProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InterfaceProductInfo, ct.InterfaceProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProtocolInfo, ct.ProtocolInfo) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�û���¼Ӧ��
struct RspUserLoginField
{
	///������
	TDateType	TradingDay;
	///��¼�ɹ�ʱ��
	TTimeType	LoginTime;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///����ϵͳ����
	TSystemNameType	SystemName;
	///ǰ�ñ��
	TFrontIDType	FrontID;
	///�Ự���
	TSessionIDType	SessionID;
	///��󱨵�����
	TOrderRefType	MaxOrderRef;
	///������ʱ��
	TTimeType	SHFETime;
	///������ʱ��
	TTimeType	DCETime;
	///֣����ʱ��
	TTimeType	CZCETime;
	///�н���ʱ��
	TTimeType	FFEXTime;

bool operator==(const RspUserLoginField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LoginTime, ct.LoginTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SystemName, ct.SystemName) == 0);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(MaxOrderRef, ct.MaxOrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SHFETime, ct.SHFETime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(DCETime, ct.DCETime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CZCETime, ct.CZCETime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FFEXTime, ct.FFEXTime) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�û��ǳ�����
struct UserLogoutField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;

bool operator==(const UserLogoutField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///ǿ�ƽ���Ա�˳�
struct ForceUserLogoutField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;

bool operator==(const ForceUserLogoutField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///����ת�ʱ���ͷ
struct TransferHeaderField
{
	///�汾�ţ�������1.0
	TVersionType	Version;
	///���״��룬����
	TTradeCodeType	TradeCode;
	///�������ڣ������ʽ��yyyymmdd
	TTradeDateType	TradeDate;
	///����ʱ�䣬�����ʽ��hhmmss
	TTradeTimeType	TradeTime;
	///������ˮ�ţ�N/A
	TTradeSerialType	TradeSerial;
	///�ڻ���˾���룬����
	TFutureIDType	FutureID;
	///���д��룬���ݲ�ѯ���еõ�������
	TBankIDType	BankID;
	///���з����Ĵ��룬���ݲ�ѯ���еõ�������
	TBankBrchIDType	BankBrchID;
	///����Ա��N/A
	TOperNoType	OperNo;
	///�����豸���ͣ�N/A
	TDeviceIDType	DeviceID;
	///��¼����N/A
	TRecordNumType	RecordNum;
	///�Ự��ţ�N/A
	TSessionIDType	SessionID;
	///�����ţ�N/A
	TRequestIDType	RequestID;

bool operator==(const TransferHeaderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(Version, ct.Version) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeCode, ct.TradeCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeDate, ct.TradeDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeTime, ct.TradeTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeSerial, ct.TradeSerial) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureID, ct.FutureID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankID, ct.BankID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankBrchID, ct.BankBrchID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OperNo, ct.OperNo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(DeviceID, ct.DeviceID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(RecordNum, ct.RecordNum) == 0);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;

	return ret;
}
};

///�����ʽ�ת�ڻ�����TradeCode=202001
struct TransferBankToFutureReqField
{
	///�ڻ��ʽ��˻�
	TAccountIDType	FutureAccount;
	///�����־
	TFuturePwdFlagType	FuturePwdFlag;
	///����
	TFutureAccPwdType	FutureAccPwd;
	///ת�˽��
	TMoneyType	TradeAmt;
	///�ͻ�������
	TMoneyType	CustFee;
	///���֣�RMB-����� USD-��Բ HKD-��Ԫ
	TCurrencyCodeType	CurrencyCode;

bool operator==(const TransferBankToFutureReqField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(FutureAccount, ct.FutureAccount) == 0);
	if (!ret) return ret;
	ret = ret && (FuturePwdFlag == ct.FuturePwdFlag);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureAccPwd, ct.FutureAccPwd) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(TradeAmt, ct.TradeAmt));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CustFee, ct.CustFee));
	if (!ret) return ret;
	ret = ret && (strcmp(CurrencyCode, ct.CurrencyCode) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�����ʽ�ת�ڻ�������Ӧ
struct TransferBankToFutureRspField
{
	///��Ӧ����
	TRetCodeType	RetCode;
	///��Ӧ��Ϣ
	TRetInfoType	RetInfo;
	///�ʽ��˻�
	TAccountIDType	FutureAccount;
	///ת�ʽ��
	TMoneyType	TradeAmt;
	///Ӧ�տͻ�������
	TMoneyType	CustFee;
	///����
	TCurrencyCodeType	CurrencyCode;

bool operator==(const TransferBankToFutureRspField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(RetCode, ct.RetCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(RetInfo, ct.RetInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureAccount, ct.FutureAccount) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(TradeAmt, ct.TradeAmt));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CustFee, ct.CustFee));
	if (!ret) return ret;
	ret = ret && (strcmp(CurrencyCode, ct.CurrencyCode) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�ڻ��ʽ�ת��������TradeCode=202002
struct TransferFutureToBankReqField
{
	///�ڻ��ʽ��˻�
	TAccountIDType	FutureAccount;
	///�����־
	TFuturePwdFlagType	FuturePwdFlag;
	///����
	TFutureAccPwdType	FutureAccPwd;
	///ת�˽��
	TMoneyType	TradeAmt;
	///�ͻ�������
	TMoneyType	CustFee;
	///���֣�RMB-����� USD-��Բ HKD-��Ԫ
	TCurrencyCodeType	CurrencyCode;

bool operator==(const TransferFutureToBankReqField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(FutureAccount, ct.FutureAccount) == 0);
	if (!ret) return ret;
	ret = ret && (FuturePwdFlag == ct.FuturePwdFlag);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureAccPwd, ct.FutureAccPwd) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(TradeAmt, ct.TradeAmt));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CustFee, ct.CustFee));
	if (!ret) return ret;
	ret = ret && (strcmp(CurrencyCode, ct.CurrencyCode) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�ڻ��ʽ�ת����������Ӧ
struct TransferFutureToBankRspField
{
	///��Ӧ����
	TRetCodeType	RetCode;
	///��Ӧ��Ϣ
	TRetInfoType	RetInfo;
	///�ʽ��˻�
	TAccountIDType	FutureAccount;
	///ת�ʽ��
	TMoneyType	TradeAmt;
	///Ӧ�տͻ�������
	TMoneyType	CustFee;
	///����
	TCurrencyCodeType	CurrencyCode;

bool operator==(const TransferFutureToBankRspField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(RetCode, ct.RetCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(RetInfo, ct.RetInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureAccount, ct.FutureAccount) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(TradeAmt, ct.TradeAmt));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CustFee, ct.CustFee));
	if (!ret) return ret;
	ret = ret && (strcmp(CurrencyCode, ct.CurrencyCode) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ�����ʽ�����TradeCode=204002
struct TransferQryBankReqField
{
	///�ڻ��ʽ��˻�
	TAccountIDType	FutureAccount;
	///�����־
	TFuturePwdFlagType	FuturePwdFlag;
	///����
	TFutureAccPwdType	FutureAccPwd;
	///���֣�RMB-����� USD-��Բ HKD-��Ԫ
	TCurrencyCodeType	CurrencyCode;

bool operator==(const TransferQryBankReqField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(FutureAccount, ct.FutureAccount) == 0);
	if (!ret) return ret;
	ret = ret && (FuturePwdFlag == ct.FuturePwdFlag);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureAccPwd, ct.FutureAccPwd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CurrencyCode, ct.CurrencyCode) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ�����ʽ�������Ӧ
struct TransferQryBankRspField
{
	///��Ӧ����
	TRetCodeType	RetCode;
	///��Ӧ��Ϣ
	TRetInfoType	RetInfo;
	///�ʽ��˻�
	TAccountIDType	FutureAccount;
	///�������
	TMoneyType	TradeAmt;
	///���п������
	TMoneyType	UseAmt;
	///���п�ȡ���
	TMoneyType	FetchAmt;
	///����
	TCurrencyCodeType	CurrencyCode;

bool operator==(const TransferQryBankRspField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(RetCode, ct.RetCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(RetInfo, ct.RetInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureAccount, ct.FutureAccount) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(TradeAmt, ct.TradeAmt));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UseAmt, ct.UseAmt));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FetchAmt, ct.FetchAmt));
	if (!ret) return ret;
	ret = ret && (strcmp(CurrencyCode, ct.CurrencyCode) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���н�����ϸ����TradeCode=204999
struct TransferQryDetailReqField
{
	///�ڻ��ʽ��˻�
	TAccountIDType	FutureAccount;

bool operator==(const TransferQryDetailReqField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(FutureAccount, ct.FutureAccount) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���н�����ϸ������Ӧ
struct TransferQryDetailRspField
{
	///��������
	TDateType	TradeDate;
	///����ʱ��
	TTradeTimeType	TradeTime;
	///���״���
	TTradeCodeType	TradeCode;
	///�ڻ���ˮ��
	TTradeSerialNoType	FutureSerial;
	///�ڻ���˾����
	TFutureIDType	FutureID;
	///�ʽ��ʺ�
	TFutureAccountType	FutureAccount;
	///������ˮ��
	TTradeSerialNoType	BankSerial;
	///���д���
	TBankIDType	BankID;
	///���з����Ĵ���
	TBankBrchIDType	BankBrchID;
	///�����˺�
	TBankAccountType	BankAccount;
	///֤������
	TCertCodeType	CertCode;
	///���Ҵ���
	TCurrencyCodeType	CurrencyCode;
	///�������
	TMoneyType	TxAmount;
	///��Ч��־
	TTransferValidFlagType	Flag;

bool operator==(const TransferQryDetailRspField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradeDate, ct.TradeDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeTime, ct.TradeTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeCode, ct.TradeCode) == 0);
	if (!ret) return ret;
	ret = ret && (FutureSerial == ct.FutureSerial);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureID, ct.FutureID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureAccount, ct.FutureAccount) == 0);
	if (!ret) return ret;
	ret = ret && (BankSerial == ct.BankSerial);
	if (!ret) return ret;
	ret = ret && (strcmp(BankID, ct.BankID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankBrchID, ct.BankBrchID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankAccount, ct.BankAccount) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CertCode, ct.CertCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CurrencyCode, ct.CurrencyCode) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(TxAmount, ct.TxAmount));
	if (!ret) return ret;
	ret = ret && (Flag == ct.Flag);
	if (!ret) return ret;

	return ret;
}
};

///��Ӧ��Ϣ
struct RspInfoField
{
	///�������
	TErrorIDType	ErrorID;
	///������Ϣ
	TErrorMsgType	ErrorMsg;

bool operator==(const RspInfoField &ct) const
{
	bool ret = true;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///������
struct ExchangeField
{
	///����������
	TExchangeIDType	ExchangeID;
	///����������
	TExchangeNameType	ExchangeName;
	///����������
	TExchangePropertyType	ExchangeProperty;

bool operator==(const ExchangeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeName, ct.ExchangeName) == 0);
	if (!ret) return ret;
	ret = ret && (ExchangeProperty == ct.ExchangeProperty);
	if (!ret) return ret;

	return ret;
}
};

///��Ʒ
struct ProductField
{
	///��Ʒ����
	TInstrumentIDType	ProductID;
	///��Ʒ����
	TProductNameType	ProductName;
	///����������
	TExchangeIDType	ExchangeID;
	///��Ʒ����
	TProductClassType	ProductClass;
	///��Լ��������
	TVolumeMultipleType	VolumeMultiple;
	///��С�䶯��λ
	TPriceType	PriceTick;
	///�м۵�����µ���
	TVolumeType	MaxMarketOrderVolume;
	///�м۵���С�µ���
	TVolumeType	MinMarketOrderVolume;
	///�޼۵�����µ���
	TVolumeType	MaxLimitOrderVolume;
	///�޼۵���С�µ���
	TVolumeType	MinLimitOrderVolume;
	///�ֲ�����
	TPositionTypeType	PositionType;
	///�ֲ���������
	TPositionDateTypeType	PositionDateType;
	///ƽ�ִ�������
	TCloseDealTypeType	CloseDealType;

bool operator==(const ProductField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductName, ct.ProductName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (ProductClass == ct.ProductClass);
	if (!ret) return ret;
	ret = ret && (VolumeMultiple == ct.VolumeMultiple);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PriceTick, ct.PriceTick));
	if (!ret) return ret;
	ret = ret && (MaxMarketOrderVolume == ct.MaxMarketOrderVolume);
	if (!ret) return ret;
	ret = ret && (MinMarketOrderVolume == ct.MinMarketOrderVolume);
	if (!ret) return ret;
	ret = ret && (MaxLimitOrderVolume == ct.MaxLimitOrderVolume);
	if (!ret) return ret;
	ret = ret && (MinLimitOrderVolume == ct.MinLimitOrderVolume);
	if (!ret) return ret;
	ret = ret && (PositionType == ct.PositionType);
	if (!ret) return ret;
	ret = ret && (PositionDateType == ct.PositionDateType);
	if (!ret) return ret;
	ret = ret && (CloseDealType == ct.CloseDealType);
	if (!ret) return ret;

	return ret;
}
};

///��Լ
struct InstrumentField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///����������
	TExchangeIDType	ExchangeID;
	///��Լ����
	TInstrumentNameType	InstrumentName;
	///��Լ�ڽ������Ĵ���
	TExchangeInstIDType	ExchangeInstID;
	///��Ʒ����
	TInstrumentIDType	ProductID;
	///��Ʒ����
	TProductClassType	ProductClass;
	///�������
	TYearType	DeliveryYear;
	///������
	TMonthType	DeliveryMonth;
	///�м۵�����µ���
	TVolumeType	MaxMarketOrderVolume;
	///�м۵���С�µ���
	TVolumeType	MinMarketOrderVolume;
	///�޼۵�����µ���
	TVolumeType	MaxLimitOrderVolume;
	///�޼۵���С�µ���
	TVolumeType	MinLimitOrderVolume;
	///��Լ��������
	TVolumeMultipleType	VolumeMultiple;
	///��С�䶯��λ
	TPriceType	PriceTick;
	///������
	TDateType	CreateDate;
	///������
	TDateType	OpenDate;
	///������
	TDateType	ExpireDate;
	///��ʼ������
	TDateType	StartDelivDate;
	///����������
	TDateType	EndDelivDate;
	///��Լ��������״̬
	TInstLifePhaseType	InstLifePhase;
	///��ǰ�Ƿ���
	TBoolType	IsTrading;
	///�ֲ�����
	TPositionTypeType	PositionType;
	///�ֲ���������
	TPositionDateTypeType	PositionDateType;
	///��ͷ��֤����
	TRatioType	LongMarginRatio;
	///��ͷ��֤����
	TRatioType	ShortMarginRatio;

bool operator==(const InstrumentField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentName, ct.InstrumentName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;
	ret = ret && (ProductClass == ct.ProductClass);
	if (!ret) return ret;
	ret = ret && (DeliveryYear == ct.DeliveryYear);
	if (!ret) return ret;
	ret = ret && (DeliveryMonth == ct.DeliveryMonth);
	if (!ret) return ret;
	ret = ret && (MaxMarketOrderVolume == ct.MaxMarketOrderVolume);
	if (!ret) return ret;
	ret = ret && (MinMarketOrderVolume == ct.MinMarketOrderVolume);
	if (!ret) return ret;
	ret = ret && (MaxLimitOrderVolume == ct.MaxLimitOrderVolume);
	if (!ret) return ret;
	ret = ret && (MinLimitOrderVolume == ct.MinLimitOrderVolume);
	if (!ret) return ret;
	ret = ret && (VolumeMultiple == ct.VolumeMultiple);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PriceTick, ct.PriceTick));
	if (!ret) return ret;
	ret = ret && (strcmp(CreateDate, ct.CreateDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenDate, ct.OpenDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExpireDate, ct.ExpireDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StartDelivDate, ct.StartDelivDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EndDelivDate, ct.EndDelivDate) == 0);
	if (!ret) return ret;
	ret = ret && (InstLifePhase == ct.InstLifePhase);
	if (!ret) return ret;
	ret = ret && (IsTrading == ct.IsTrading);
	if (!ret) return ret;
	ret = ret && (PositionType == ct.PositionType);
	if (!ret) return ret;
	ret = ret && (PositionDateType == ct.PositionDateType);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatio, ct.LongMarginRatio));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatio, ct.ShortMarginRatio));
	if (!ret) return ret;

	return ret;
}
};

///���͹�˾
struct BrokerField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///���͹�˾���
	TBrokerAbbrType	BrokerAbbr;
	///���͹�˾����
	TBrokerNameType	BrokerName;
	///�Ƿ��Ծ
	TBoolType	IsActive;

bool operator==(const BrokerField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerAbbr, ct.BrokerAbbr) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerName, ct.BrokerName) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;

	return ret;
}
};

///����������Ա
struct TraderField
{
	///����������
	TExchangeIDType	ExchangeID;
	///����������Ա����
	TTraderIDType	TraderID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///����
	TPasswordType	Password;
	///��װ����
	TInstallCountType	InstallCount;
	///���͹�˾����
	TBrokerIDType	BrokerID;

bool operator==(const TraderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;
	ret = ret && (InstallCount == ct.InstallCount);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ����
struct InvestorField
{
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���߷������
	TInvestorIDType	InvestorGroupID;
	///Ͷ��������
	TPartyNameType	InvestorName;
	///֤������
	TIdCardTypeType	IdentifiedCardType;
	///֤������
	TIdentifiedCardNoType	IdentifiedCardNo;
	///�Ƿ��Ծ
	TBoolType	IsActive;
	///��ϵ�绰
	TTelephoneType	Telephone;
	///ͨѶ��ַ
	TAddressType	Address;
	///��������
	TDateType	OpenDate;
	///�ֻ�
	TMobileType	Mobile;
	///��������ģ�����
	TInvestorIDType	CommModelID;

bool operator==(const InvestorField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupID, ct.InvestorGroupID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorName, ct.InvestorName) == 0);
	if (!ret) return ret;
	ret = ret && (IdentifiedCardType == ct.IdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(IdentifiedCardNo, ct.IdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (strcmp(Telephone, ct.Telephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Address, ct.Address) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenDate, ct.OpenDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Mobile, ct.Mobile) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CommModelID, ct.CommModelID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///���ױ���
struct TradingCodeField
{
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///����������
	TExchangeIDType	ExchangeID;
	///�ͻ�����
	TClientIDType	ClientID;
	///�Ƿ��Ծ
	TBoolType	IsActive;
	///���ױ�������
	TClientIDTypeType	ClientIDType;

bool operator==(const TradingCodeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (ClientIDType == ct.ClientIDType);
	if (!ret) return ret;

	return ret;
}
};

///��Ա����;��͹�˾������ձ�
struct PartBrokerField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///����������
	TExchangeIDType	ExchangeID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///�Ƿ��Ծ
	TBoolType	IsActive;

bool operator==(const PartBrokerField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;

	return ret;
}
};

///�����û�
struct SuperUserField
{
	///�û�����
	TUserIDType	UserID;
	///�û�����
	TUserNameType	UserName;
	///����
	TPasswordType	Password;
	///�Ƿ��Ծ
	TBoolType	IsActive;

bool operator==(const SuperUserField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserName, ct.UserName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;

	return ret;
}
};

///�����û�����Ȩ��
struct SuperUserFunctionField
{
	///�û�����
	TUserIDType	UserID;
	///���ܴ���
	TFunctionCodeType	FunctionCode;

bool operator==(const SuperUserFunctionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (FunctionCode == ct.FunctionCode);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ������
struct InvestorGroupField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���߷������
	TInvestorIDType	InvestorGroupID;
	///Ͷ���߷�������
	TInvestorGroupNameType	InvestorGroupName;

bool operator==(const InvestorGroupField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupID, ct.InvestorGroupID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupName, ct.InvestorGroupName) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�ʽ��˻�
struct TradingAccountField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TAccountIDType	AccountID;
	///�ϴ���Ѻ���
	TMoneyType	PreMortgage;
	///�ϴ����ö��
	TMoneyType	PreCredit;
	///�ϴδ���
	TMoneyType	PreDeposit;
	///�ϴν���׼����
	TMoneyType	PreBalance;
	///�ϴ�ռ�õı�֤��
	TMoneyType	PreMargin;
	///��Ϣ����
	TMoneyType	InterestBase;
	///��Ϣ����
	TMoneyType	Interest;
	///�����
	TMoneyType	Deposit;
	///������
	TMoneyType	Withdraw;
	///����ı�֤��
	TMoneyType	FrozenMargin;
	///������ʽ�
	TMoneyType	FrozenCash;
	///�����������
	TMoneyType	FrozenCommission;
	///��ǰ��֤���ܶ�
	TMoneyType	CurrMargin;
	///�ʽ���
	TMoneyType	CashIn;
	///������
	TMoneyType	Commission;
	///ƽ��ӯ��
	TMoneyType	CloseProfit;
	///�ֲ�ӯ��
	TMoneyType	PositionProfit;
	///�ڻ�����׼����
	TMoneyType	Balance;
	///�����ʽ�
	TMoneyType	Available;
	///��ȡ�ʽ�
	TMoneyType	WithdrawQuota;
	///����׼����
	TMoneyType	Reserve;
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;
	///���ö��
	TMoneyType	Credit;
	///��Ѻ���
	TMoneyType	Mortgage;
	///��������֤��
	TMoneyType	ExchangeMargin;
	///Ͷ���߽��֤��
	TMoneyType	DeliveryMargin;
	///���������֤��
	TMoneyType	ExchangeDeliveryMargin;

bool operator==(const TradingAccountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(AccountID, ct.AccountID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMortgage, ct.PreMortgage));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreCredit, ct.PreCredit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreDeposit, ct.PreDeposit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreBalance, ct.PreBalance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMargin, ct.PreMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(InterestBase, ct.InterestBase));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Interest, ct.Interest));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Deposit, ct.Deposit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Withdraw, ct.Withdraw));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCash, ct.FrozenCash));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCommission, ct.FrozenCommission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrMargin, ct.CurrMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CashIn, ct.CashIn));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Commission, ct.Commission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfit, ct.PositionProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance, ct.Balance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Available, ct.Available));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(WithdrawQuota, ct.WithdrawQuota));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Reserve, ct.Reserve));
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Credit, ct.Credit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Mortgage, ct.Mortgage));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeMargin, ct.ExchangeMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(DeliveryMargin, ct.DeliveryMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeDeliveryMargin, ct.ExchangeDeliveryMargin));
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���ֲ߳�
struct InvestorPositionField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�ֲֶ�շ���
	TPosiDirectionType	PosiDirection;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///�ֲ�����
	TPositionDateType	PositionDate;
	///���ճֲ�
	TVolumeType	YdPosition;
	///���ճֲ�
	TVolumeType	Position;
	///��ͷ����
	TVolumeType	LongFrozen;
	///��ͷ����
	TVolumeType	ShortFrozen;
	///���ֶ�����
	TMoneyType	LongFrozenAmount;
	///���ֶ�����
	TMoneyType	ShortFrozenAmount;
	///������
	TVolumeType	OpenVolume;
	///ƽ����
	TVolumeType	CloseVolume;
	///���ֽ��
	TMoneyType	OpenAmount;
	///ƽ�ֽ��
	TMoneyType	CloseAmount;
	///�ֲֳɱ�
	TMoneyType	PositionCost;
	///�ϴ�ռ�õı�֤��
	TMoneyType	PreMargin;
	///ռ�õı�֤��
	TMoneyType	UseMargin;
	///����ı�֤��
	TMoneyType	FrozenMargin;
	///������ʽ�
	TMoneyType	FrozenCash;
	///�����������
	TMoneyType	FrozenCommission;
	///�ʽ���
	TMoneyType	CashIn;
	///������
	TMoneyType	Commission;
	///ƽ��ӯ��
	TMoneyType	CloseProfit;
	///�ֲ�ӯ��
	TMoneyType	PositionProfit;
	///�ϴν����
	TPriceType	PreSettlementPrice;
	///���ν����
	TPriceType	SettlementPrice;
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;
	///���ֳɱ�
	TMoneyType	OpenCost;
	///��������֤��
	TMoneyType	ExchangeMargin;
	///��ϳɽ��γɵĳֲ�
	TVolumeType	CombPosition;
	///��϶�ͷ����
	TVolumeType	CombLongFrozen;
	///��Ͽ�ͷ����
	TVolumeType	CombShortFrozen;
	///���ն���ƽ��ӯ��
	TMoneyType	CloseProfitByDate;
	///��ʶԳ�ƽ��ӯ��
	TMoneyType	CloseProfitByTrade;
	///���ճֲ�
	TVolumeType	TodayPosition;
	///��֤����
	TRatioType	MarginRateByMoney;
	///��֤����(������)
	TRatioType	MarginRateByVolume;

bool operator==(const InvestorPositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (PosiDirection == ct.PosiDirection);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (PositionDate == ct.PositionDate);
	if (!ret) return ret;
	ret = ret && (YdPosition == ct.YdPosition);
	if (!ret) return ret;
	ret = ret && (Position == ct.Position);
	if (!ret) return ret;
	ret = ret && (LongFrozen == ct.LongFrozen);
	if (!ret) return ret;
	ret = ret && (ShortFrozen == ct.ShortFrozen);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongFrozenAmount, ct.LongFrozenAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortFrozenAmount, ct.ShortFrozenAmount));
	if (!ret) return ret;
	ret = ret && (OpenVolume == ct.OpenVolume);
	if (!ret) return ret;
	ret = ret && (CloseVolume == ct.CloseVolume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenAmount, ct.OpenAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseAmount, ct.CloseAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionCost, ct.PositionCost));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMargin, ct.PreMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UseMargin, ct.UseMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCash, ct.FrozenCash));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCommission, ct.FrozenCommission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CashIn, ct.CashIn));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Commission, ct.Commission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfit, ct.PositionProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreSettlementPrice, ct.PreSettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SettlementPrice, ct.SettlementPrice));
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenCost, ct.OpenCost));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeMargin, ct.ExchangeMargin));
	if (!ret) return ret;
	ret = ret && (CombPosition == ct.CombPosition);
	if (!ret) return ret;
	ret = ret && (CombLongFrozen == ct.CombLongFrozen);
	if (!ret) return ret;
	ret = ret && (CombShortFrozen == ct.CombShortFrozen);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByDate, ct.CloseProfitByDate));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByTrade, ct.CloseProfitByTrade));
	if (!ret) return ret;
	ret = ret && (TodayPosition == ct.TodayPosition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByMoney, ct.MarginRateByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByVolume, ct.MarginRateByVolume));
	if (!ret) return ret;

	return ret;
}
};

///��Լ��֤����
struct InstrumentMarginRateField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	TInvestorRangeType	InvestorRange;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///��ͷ��֤����
	TRatioType	LongMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	LongMarginRatioByVolume;
	///��ͷ��֤����
	TRatioType	ShortMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	ShortMarginRatioByVolume;
	///�Ƿ���Խ�������ȡ
	TBoolType	IsRelative;

bool operator==(const InstrumentMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (IsRelative == ct.IsRelative);
	if (!ret) return ret;

	return ret;
}
};

///��Լ��������
struct InstrumentCommissionRateField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	TInvestorRangeType	InvestorRange;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///������������
	TRatioType	OpenRatioByMoney;
	///����������
	TRatioType	OpenRatioByVolume;
	///ƽ����������
	TRatioType	CloseRatioByMoney;
	///ƽ��������
	TRatioType	CloseRatioByVolume;
	///ƽ����������
	TRatioType	CloseTodayRatioByMoney;
	///ƽ��������
	TRatioType	CloseTodayRatioByVolume;

bool operator==(const InstrumentCommissionRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenRatioByMoney, ct.OpenRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenRatioByVolume, ct.OpenRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseRatioByMoney, ct.CloseRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseRatioByVolume, ct.CloseRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseTodayRatioByMoney, ct.CloseTodayRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseTodayRatioByVolume, ct.CloseTodayRatioByVolume));
	if (!ret) return ret;

	return ret;
}
};

///�������
struct DepthMarketDataField
{
	///������
	TDateType	TradingDay;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///����������
	TExchangeIDType	ExchangeID;
	///��Լ�ڽ������Ĵ���
	TExchangeInstIDType	ExchangeInstID;
	///���¼�
	TPriceType	LastPrice;
	///�ϴν����
	TPriceType	PreSettlementPrice;
	///������
	TPriceType	PreClosePrice;
	///��ֲ���
	TLargeVolumeType	PreOpenInterest;
	///����
	TPriceType	OpenPrice;
	///��߼�
	TPriceType	HighestPrice;
	///��ͼ�
	TPriceType	LowestPrice;
	///����
	TVolumeType	Volume;
	///�ɽ����
	TMoneyType	Turnover;
	///�ֲ���
	TLargeVolumeType	OpenInterest;
	///������
	TPriceType	ClosePrice;
	///���ν����
	TPriceType	SettlementPrice;
	///��ͣ���
	TPriceType	UpperLimitPrice;
	///��ͣ���
	TPriceType	LowerLimitPrice;
	///����ʵ��
	TRatioType	PreDelta;
	///����ʵ��
	TRatioType	CurrDelta;
	///����޸�ʱ��
	TTimeType	UpdateTime;
	///����޸ĺ���
	TMillisecType	UpdateMillisec;
	///�����һ
	TPriceType	BidPrice1;
	///������һ
	TVolumeType	BidVolume1;
	///������һ
	TPriceType	AskPrice1;
	///������һ
	TVolumeType	AskVolume1;
	///����۶�
	TPriceType	BidPrice2;
	///��������
	TVolumeType	BidVolume2;
	///�����۶�
	TPriceType	AskPrice2;
	///��������
	TVolumeType	AskVolume2;
	///�������
	TPriceType	BidPrice3;
	///��������
	TVolumeType	BidVolume3;
	///��������
	TPriceType	AskPrice3;
	///��������
	TVolumeType	AskVolume3;
	///�������
	TPriceType	BidPrice4;
	///��������
	TVolumeType	BidVolume4;
	///��������
	TPriceType	AskPrice4;
	///��������
	TVolumeType	AskVolume4;
	///�������
	TPriceType	BidPrice5;
	///��������
	TVolumeType	BidVolume5;
	///��������
	TPriceType	AskPrice5;
	///��������
	TVolumeType	AskVolume5;
	///���վ���
	TPriceType	AveragePrice;

bool operator==(const DepthMarketDataField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LastPrice, ct.LastPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreSettlementPrice, ct.PreSettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreClosePrice, ct.PreClosePrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreOpenInterest, ct.PreOpenInterest));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenPrice, ct.OpenPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(HighestPrice, ct.HighestPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LowestPrice, ct.LowestPrice));
	if (!ret) return ret;
	ret = ret && (Volume == ct.Volume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Turnover, ct.Turnover));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenInterest, ct.OpenInterest));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ClosePrice, ct.ClosePrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SettlementPrice, ct.SettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UpperLimitPrice, ct.UpperLimitPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LowerLimitPrice, ct.LowerLimitPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreDelta, ct.PreDelta));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrDelta, ct.CurrDelta));
	if (!ret) return ret;
	ret = ret && (strcmp(UpdateTime, ct.UpdateTime) == 0);
	if (!ret) return ret;
	ret = ret && (UpdateMillisec == ct.UpdateMillisec);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BidPrice1, ct.BidPrice1));
	if (!ret) return ret;
	ret = ret && (BidVolume1 == ct.BidVolume1);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AskPrice1, ct.AskPrice1));
	if (!ret) return ret;
	ret = ret && (AskVolume1 == ct.AskVolume1);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BidPrice2, ct.BidPrice2));
	if (!ret) return ret;
	ret = ret && (BidVolume2 == ct.BidVolume2);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AskPrice2, ct.AskPrice2));
	if (!ret) return ret;
	ret = ret && (AskVolume2 == ct.AskVolume2);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BidPrice3, ct.BidPrice3));
	if (!ret) return ret;
	ret = ret && (BidVolume3 == ct.BidVolume3);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AskPrice3, ct.AskPrice3));
	if (!ret) return ret;
	ret = ret && (AskVolume3 == ct.AskVolume3);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BidPrice4, ct.BidPrice4));
	if (!ret) return ret;
	ret = ret && (BidVolume4 == ct.BidVolume4);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AskPrice4, ct.AskPrice4));
	if (!ret) return ret;
	ret = ret && (AskVolume4 == ct.AskVolume4);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BidPrice5, ct.BidPrice5));
	if (!ret) return ret;
	ret = ret && (BidVolume5 == ct.BidVolume5);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AskPrice5, ct.AskPrice5));
	if (!ret) return ret;
	ret = ret && (AskVolume5 == ct.AskVolume5);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AveragePrice, ct.AveragePrice));
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���ߺ�Լ����Ȩ��
struct InstrumentTradingRightField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	TInvestorRangeType	InvestorRange;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///����Ȩ��
	TTradingRightType	TradingRight;

bool operator==(const InstrumentTradingRightField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (TradingRight == ct.TradingRight);
	if (!ret) return ret;

	return ret;
}
};

///���͹�˾�û�
struct BrokerUserField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///�û�����
	TUserNameType	UserName;
	///�û�����
	TUserTypeType	UserType;
	///�Ƿ��Ծ
	TBoolType	IsActive;
	///�Ƿ�ʹ������
	TBoolType	IsUsingOTP;

bool operator==(const BrokerUserField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserName, ct.UserName) == 0);
	if (!ret) return ret;
	ret = ret && (UserType == ct.UserType);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (IsUsingOTP == ct.IsUsingOTP);
	if (!ret) return ret;

	return ret;
}
};

///���͹�˾�û�����
struct BrokerUserPasswordField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///����
	TPasswordType	Password;

bool operator==(const BrokerUserPasswordField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;

	return ret;
}
};

///���͹�˾�û�����Ȩ��
struct BrokerUserFunctionField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///���͹�˾���ܴ���
	TBrokerFunctionCodeType	BrokerFunctionCode;

bool operator==(const BrokerUserFunctionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (BrokerFunctionCode == ct.BrokerFunctionCode);
	if (!ret) return ret;

	return ret;
}
};

///����������Ա���̻�
struct TraderOfferField
{
	///����������
	TExchangeIDType	ExchangeID;
	///����������Ա����
	TTraderIDType	TraderID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///����
	TPasswordType	Password;
	///��װ���
	TInstallIDType	InstallID;
	///���ر������
	TOrderLocalIDType	OrderLocalID;
	///����������Ա����״̬
	TTraderConnectStatusType	TraderConnectStatus;
	///�����������������
	TDateType	ConnectRequestDate;
	///�������������ʱ��
	TTimeType	ConnectRequestTime;
	///�ϴα�������
	TDateType	LastReportDate;
	///�ϴα���ʱ��
	TTimeType	LastReportTime;
	///�����������
	TDateType	ConnectDate;
	///�������ʱ��
	TTimeType	ConnectTime;
	///��������
	TDateType	StartDate;
	///����ʱ��
	TTimeType	StartTime;
	///������
	TDateType	TradingDay;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��ϯλ���ɽ����
	TTradeIDType	MaxTradeID;
	///��ϯλ��󱨵�����
	TReturnCodeType	MaxOrderMessageReference;

bool operator==(const TraderOfferField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (TraderConnectStatus == ct.TraderConnectStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(ConnectRequestDate, ct.ConnectRequestDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ConnectRequestTime, ct.ConnectRequestTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LastReportDate, ct.LastReportDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LastReportTime, ct.LastReportTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ConnectDate, ct.ConnectDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ConnectTime, ct.ConnectTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StartDate, ct.StartDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StartTime, ct.StartTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(MaxTradeID, ct.MaxTradeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(MaxOrderMessageReference, ct.MaxOrderMessageReference) == 0);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���߽�����
struct SettlementInfoField
{
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///���
	TSequenceNoType	SequenceNo;
	///��Ϣ����
	TContentType	Content;

bool operator==(const SettlementInfoField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(Content, ct.Content) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��Լ��֤���ʵ���
struct InstrumentMarginRateAdjustField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	TInvestorRangeType	InvestorRange;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///��ͷ��֤����
	TRatioType	LongMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	LongMarginRatioByVolume;
	///��ͷ��֤����
	TRatioType	ShortMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	ShortMarginRatioByVolume;
	///�Ƿ���Խ�������ȡ
	TBoolType	IsRelative;

bool operator==(const InstrumentMarginRateAdjustField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (IsRelative == ct.IsRelative);
	if (!ret) return ret;

	return ret;
}
};

///��������֤����
struct ExchangeMarginRateField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///��ͷ��֤����
	TRatioType	LongMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	LongMarginRatioByVolume;
	///��ͷ��֤����
	TRatioType	ShortMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	ShortMarginRatioByVolume;

bool operator==(const ExchangeMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;

	return ret;
}
};

///��������֤���ʵ���
struct ExchangeMarginRateAdjustField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///���潻����Ͷ���߶�ͷ��֤����
	TRatioType	LongMarginRatioByMoney;
	///���潻����Ͷ���߶�ͷ��֤���
	TMoneyType	LongMarginRatioByVolume;
	///���潻����Ͷ���߿�ͷ��֤����
	TRatioType	ShortMarginRatioByMoney;
	///���潻����Ͷ���߿�ͷ��֤���
	TMoneyType	ShortMarginRatioByVolume;
	///��������ͷ��֤����
	TRatioType	ExchLongMarginRatioByMoney;
	///��������ͷ��֤���
	TMoneyType	ExchLongMarginRatioByVolume;
	///��������ͷ��֤����
	TRatioType	ExchShortMarginRatioByMoney;
	///��������ͷ��֤���
	TMoneyType	ExchShortMarginRatioByVolume;
	///�����潻����Ͷ���߶�ͷ��֤����
	TRatioType	NoLongMarginRatioByMoney;
	///�����潻����Ͷ���߶�ͷ��֤���
	TMoneyType	NoLongMarginRatioByVolume;
	///�����潻����Ͷ���߿�ͷ��֤����
	TRatioType	NoShortMarginRatioByMoney;
	///�����潻����Ͷ���߿�ͷ��֤���
	TMoneyType	NoShortMarginRatioByVolume;

bool operator==(const ExchangeMarginRateAdjustField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchLongMarginRatioByMoney, ct.ExchLongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchLongMarginRatioByVolume, ct.ExchLongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchShortMarginRatioByMoney, ct.ExchShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchShortMarginRatioByVolume, ct.ExchShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NoLongMarginRatioByMoney, ct.NoLongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NoLongMarginRatioByVolume, ct.NoLongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NoShortMarginRatioByMoney, ct.NoShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NoShortMarginRatioByVolume, ct.NoShortMarginRatioByVolume));
	if (!ret) return ret;

	return ret;
}
};

///��������
struct SettlementRefField
{
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;

bool operator==(const SettlementRefField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;

	return ret;
}
};

///��ǰʱ��
struct CurrentTimeField
{
	///��ǰ����
	TDateType	CurrDate;
	///��ǰʱ��
	TTimeType	CurrTime;
	///��ǰʱ�䣨���룩
	TMillisecType	CurrMillisec;

bool operator==(const CurrentTimeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(CurrDate, ct.CurrDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CurrTime, ct.CurrTime) == 0);
	if (!ret) return ret;
	ret = ret && (CurrMillisec == ct.CurrMillisec);
	if (!ret) return ret;

	return ret;
}
};

///ͨѶ�׶�
struct CommPhaseField
{
	///������
	TDateType	TradingDay;
	///ͨѶʱ�α��
	TCommPhaseNoType	CommPhaseNo;

bool operator==(const CommPhaseField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (CommPhaseNo == ct.CommPhaseNo);
	if (!ret) return ret;

	return ret;
}
};

///��¼��Ϣ
struct LoginInfoField
{
	///ǰ�ñ��
	TFrontIDType	FrontID;
	///�Ự���
	TSessionIDType	SessionID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///��¼����
	TDateType	LoginDate;
	///��¼ʱ��
	TTimeType	LoginTime;
	///IP��ַ
	TIPAddressType	IPAddress;
	///�û��˲�Ʒ��Ϣ
	TProductInfoType	UserProductInfo;
	///�ӿڶ˲�Ʒ��Ϣ
	TProductInfoType	InterfaceProductInfo;
	///Э����Ϣ
	TProtocolInfoType	ProtocolInfo;
	///ϵͳ����
	TSystemNameType	SystemName;
	///����
	TPasswordType	Password;
	///��󱨵�����
	TOrderRefType	MaxOrderRef;
	///������ʱ��
	TTimeType	SHFETime;
	///������ʱ��
	TTimeType	DCETime;
	///֣����ʱ��
	TTimeType	CZCETime;
	///�н���ʱ��
	TTimeType	FFEXTime;
	///Mac��ַ
	TMacAddressType	MacAddress;
	///��̬����
	TPasswordType	OneTimePassword;

bool operator==(const LoginInfoField &ct) const
{
	bool ret = true;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LoginDate, ct.LoginDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LoginTime, ct.LoginTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(IPAddress, ct.IPAddress) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserProductInfo, ct.UserProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InterfaceProductInfo, ct.InterfaceProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProtocolInfo, ct.ProtocolInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SystemName, ct.SystemName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(MaxOrderRef, ct.MaxOrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SHFETime, ct.SHFETime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(DCETime, ct.DCETime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CZCETime, ct.CZCETime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FFEXTime, ct.FFEXTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(MacAddress, ct.MacAddress) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OneTimePassword, ct.OneTimePassword) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��¼��Ϣ
struct LogoutAllField
{
	///ǰ�ñ��
	TFrontIDType	FrontID;
	///�Ự���
	TSessionIDType	SessionID;
	///ϵͳ����
	TSystemNameType	SystemName;

bool operator==(const LogoutAllField &ct) const
{
	bool ret = true;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(SystemName, ct.SystemName) == 0);
	if (!ret) return ret;

	return ret;
}
};

///ǰ��״̬
struct FrontStatusField
{
	///ǰ�ñ��
	TFrontIDType	FrontID;
	///�ϴα�������
	TDateType	LastReportDate;
	///�ϴα���ʱ��
	TTimeType	LastReportTime;
	///�Ƿ��Ծ
	TBoolType	IsActive;

bool operator==(const FrontStatusField &ct) const
{
	bool ret = true;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (strcmp(LastReportDate, ct.LastReportDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LastReportTime, ct.LastReportTime) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;

	return ret;
}
};

///�û�������
struct UserPasswordUpdateField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///ԭ���Ŀ���
	TPasswordType	OldPassword;
	///�µĿ���
	TPasswordType	NewPassword;

bool operator==(const UserPasswordUpdateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OldPassword, ct.OldPassword) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(NewPassword, ct.NewPassword) == 0);
	if (!ret) return ret;

	return ret;
}
};

///���뱨��
struct InputOrderField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///��������
	TOrderRefType	OrderRef;
	///�û�����
	TUserIDType	UserID;
	///�����۸�����
	TOrderPriceTypeType	OrderPriceType;
	///��������
	TDirectionType	Direction;
	///��Ͽ�ƽ��־
	TCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	TPriceType	LimitPrice;
	///����
	TVolumeType	VolumeTotalOriginal;
	///��Ч������
	TTimeConditionType	TimeCondition;
	///GTD����
	TDateType	GTDDate;
	///�ɽ�������
	TVolumeConditionType	VolumeCondition;
	///��С�ɽ���
	TVolumeType	MinVolume;
	///��������
	TContingentConditionType	ContingentCondition;
	///ֹ���
	TPriceType	StopPrice;
	///ǿƽԭ��
	TForceCloseReasonType	ForceCloseReason;
	///�Զ������־
	TBoolType	IsAutoSuspend;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///������
	TRequestIDType	RequestID;
	///�û�ǿ����־
	TBoolType	UserForceClose;

bool operator==(const InputOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;

	return ret;
}
};

///����
struct OrderField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///��������
	TOrderRefType	OrderRef;
	///�û�����
	TUserIDType	UserID;
	///�����۸�����
	TOrderPriceTypeType	OrderPriceType;
	///��������
	TDirectionType	Direction;
	///��Ͽ�ƽ��־
	TCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	TPriceType	LimitPrice;
	///����
	TVolumeType	VolumeTotalOriginal;
	///��Ч������
	TTimeConditionType	TimeCondition;
	///GTD����
	TDateType	GTDDate;
	///�ɽ�������
	TVolumeConditionType	VolumeCondition;
	///��С�ɽ���
	TVolumeType	MinVolume;
	///��������
	TContingentConditionType	ContingentCondition;
	///ֹ���
	TPriceType	StopPrice;
	///ǿƽԭ��
	TForceCloseReasonType	ForceCloseReason;
	///�Զ������־
	TBoolType	IsAutoSuspend;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///������
	TRequestIDType	RequestID;
	///���ر������
	TOrderLocalIDType	OrderLocalID;
	///����������
	TExchangeIDType	ExchangeID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///�ͻ�����
	TClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TTraderIDType	TraderID;
	///��װ���
	TInstallIDType	InstallID;
	///�����ύ״̬
	TOrderSubmitStatusType	OrderSubmitStatus;
	///������ʾ���
	TSequenceNoType	NotifySequence;
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;
	///�������
	TOrderSysIDType	OrderSysID;
	///������Դ
	TOrderSourceType	OrderSource;
	///����״̬
	TOrderStatusType	OrderStatus;
	///��������
	TOrderTypeType	OrderType;
	///��ɽ�����
	TVolumeType	VolumeTraded;
	///ʣ������
	TVolumeType	VolumeTotal;
	///��������
	TDateType	InsertDate;
	///ί��ʱ��
	TTimeType	InsertTime;
	///����ʱ��
	TTimeType	ActiveTime;
	///����ʱ��
	TTimeType	SuspendTime;
	///����޸�ʱ��
	TTimeType	UpdateTime;
	///����ʱ��
	TTimeType	CancelTime;
	///����޸Ľ���������Ա����
	TTraderIDType	ActiveTraderID;
	///�����Ա���
	TParticipantIDType	ClearingPartID;
	///���
	TSequenceNoType	SequenceNo;
	///ǰ�ñ��
	TFrontIDType	FrontID;
	///�Ự���
	TSessionIDType	SessionID;
	///�û��˲�Ʒ��Ϣ
	TProductInfoType	UserProductInfo;
	///״̬��Ϣ
	TErrorMsgType	StatusMsg;
	///�û�ǿ����־
	TBoolType	UserForceClose;
	///�����û�����
	TUserIDType	ActiveUserID;
	///���͹�˾�������
	TSequenceNoType	BrokerOrderSeq;
	///��ر���
	TOrderSysIDType	RelativeOrderSysID;
	///֣�����ɽ�����
	TVolumeType	ZCETotalTradedVolume;

bool operator==(const OrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (OrderSubmitStatus == ct.OrderSubmitStatus);
	if (!ret) return ret;
	ret = ret && (NotifySequence == ct.NotifySequence);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderSource == ct.OrderSource);
	if (!ret) return ret;
	ret = ret && (OrderStatus == ct.OrderStatus);
	if (!ret) return ret;
	ret = ret && (OrderType == ct.OrderType);
	if (!ret) return ret;
	ret = ret && (VolumeTraded == ct.VolumeTraded);
	if (!ret) return ret;
	ret = ret && (VolumeTotal == ct.VolumeTotal);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertDate, ct.InsertDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertTime, ct.InsertTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveTime, ct.ActiveTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SuspendTime, ct.SuspendTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UpdateTime, ct.UpdateTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CancelTime, ct.CancelTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveTraderID, ct.ActiveTraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClearingPartID, ct.ClearingPartID) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(UserProductInfo, ct.UserProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StatusMsg, ct.StatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveUserID, ct.ActiveUserID) == 0);
	if (!ret) return ret;
	ret = ret && (BrokerOrderSeq == ct.BrokerOrderSeq);
	if (!ret) return ret;
	ret = ret && (strcmp(RelativeOrderSysID, ct.RelativeOrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (ZCETotalTradedVolume == ct.ZCETotalTradedVolume);
	if (!ret) return ret;

	return ret;
}
};

///����������
struct ExchangeOrderField
{
	///�����۸�����
	TOrderPriceTypeType	OrderPriceType;
	///��������
	TDirectionType	Direction;
	///��Ͽ�ƽ��־
	TCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	TPriceType	LimitPrice;
	///����
	TVolumeType	VolumeTotalOriginal;
	///��Ч������
	TTimeConditionType	TimeCondition;
	///GTD����
	TDateType	GTDDate;
	///�ɽ�������
	TVolumeConditionType	VolumeCondition;
	///��С�ɽ���
	TVolumeType	MinVolume;
	///��������
	TContingentConditionType	ContingentCondition;
	///ֹ���
	TPriceType	StopPrice;
	///ǿƽԭ��
	TForceCloseReasonType	ForceCloseReason;
	///�Զ������־
	TBoolType	IsAutoSuspend;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///������
	TRequestIDType	RequestID;
	///���ر������
	TOrderLocalIDType	OrderLocalID;
	///����������
	TExchangeIDType	ExchangeID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///�ͻ�����
	TClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TTraderIDType	TraderID;
	///��װ���
	TInstallIDType	InstallID;
	///�����ύ״̬
	TOrderSubmitStatusType	OrderSubmitStatus;
	///������ʾ���
	TSequenceNoType	NotifySequence;
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;
	///�������
	TOrderSysIDType	OrderSysID;
	///������Դ
	TOrderSourceType	OrderSource;
	///����״̬
	TOrderStatusType	OrderStatus;
	///��������
	TOrderTypeType	OrderType;
	///��ɽ�����
	TVolumeType	VolumeTraded;
	///ʣ������
	TVolumeType	VolumeTotal;
	///��������
	TDateType	InsertDate;
	///ί��ʱ��
	TTimeType	InsertTime;
	///����ʱ��
	TTimeType	ActiveTime;
	///����ʱ��
	TTimeType	SuspendTime;
	///����޸�ʱ��
	TTimeType	UpdateTime;
	///����ʱ��
	TTimeType	CancelTime;
	///����޸Ľ���������Ա����
	TTraderIDType	ActiveTraderID;
	///�����Ա���
	TParticipantIDType	ClearingPartID;
	///���
	TSequenceNoType	SequenceNo;

bool operator==(const ExchangeOrderField &ct) const
{
	bool ret = true;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (OrderSubmitStatus == ct.OrderSubmitStatus);
	if (!ret) return ret;
	ret = ret && (NotifySequence == ct.NotifySequence);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderSource == ct.OrderSource);
	if (!ret) return ret;
	ret = ret && (OrderStatus == ct.OrderStatus);
	if (!ret) return ret;
	ret = ret && (OrderType == ct.OrderType);
	if (!ret) return ret;
	ret = ret && (VolumeTraded == ct.VolumeTraded);
	if (!ret) return ret;
	ret = ret && (VolumeTotal == ct.VolumeTotal);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertDate, ct.InsertDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertTime, ct.InsertTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveTime, ct.ActiveTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SuspendTime, ct.SuspendTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UpdateTime, ct.UpdateTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CancelTime, ct.CancelTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveTraderID, ct.ActiveTraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClearingPartID, ct.ClearingPartID) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;

	return ret;
}
};

///��������������ʧ��
struct ExchangeOrderInsertErrorField
{
	///����������
	TExchangeIDType	ExchangeID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///����������Ա����
	TTraderIDType	TraderID;
	///��װ���
	TInstallIDType	InstallID;
	///���ر������
	TOrderLocalIDType	OrderLocalID;
	///�������
	TErrorIDType	ErrorID;
	///������Ϣ
	TErrorMsgType	ErrorMsg;

bool operator==(const ExchangeOrderInsertErrorField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///���뱨������
struct InputOrderActionField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///������������
	TOrderActionRefType	OrderActionRef;
	///��������
	TOrderRefType	OrderRef;
	///������
	TRequestIDType	RequestID;
	///ǰ�ñ��
	TFrontIDType	FrontID;
	///�Ự���
	TSessionIDType	SessionID;
	///����������
	TExchangeIDType	ExchangeID;
	///�������
	TOrderSysIDType	OrderSysID;
	///������־
	TActionFlagType	ActionFlag;
	///�۸�
	TPriceType	LimitPrice;
	///�����仯
	TVolumeType	VolumeChange;
	///�û�����
	TUserIDType	UserID;
	///��Լ����
	TInstrumentIDType	InstrumentID;

bool operator==(const InputOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderActionRef == ct.OrderActionRef);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (ActionFlag == ct.ActionFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeChange == ct.VolumeChange);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��������
struct OrderActionField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///������������
	TOrderActionRefType	OrderActionRef;
	///��������
	TOrderRefType	OrderRef;
	///������
	TRequestIDType	RequestID;
	///ǰ�ñ��
	TFrontIDType	FrontID;
	///�Ự���
	TSessionIDType	SessionID;
	///����������
	TExchangeIDType	ExchangeID;
	///�������
	TOrderSysIDType	OrderSysID;
	///������־
	TActionFlagType	ActionFlag;
	///�۸�
	TPriceType	LimitPrice;
	///�����仯
	TVolumeType	VolumeChange;
	///��������
	TDateType	ActionDate;
	///����ʱ��
	TTimeType	ActionTime;
	///����������Ա����
	TTraderIDType	TraderID;
	///��װ���
	TInstallIDType	InstallID;
	///���ر������
	TOrderLocalIDType	OrderLocalID;
	///�������ر��
	TOrderLocalIDType	ActionLocalID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///�ͻ�����
	TClientIDType	ClientID;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///��������״̬
	TOrderActionStatusType	OrderActionStatus;
	///�û�����
	TUserIDType	UserID;
	///״̬��Ϣ
	TErrorMsgType	StatusMsg;
	///��Լ����
	TInstrumentIDType	InstrumentID;

bool operator==(const OrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderActionRef == ct.OrderActionRef);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (ActionFlag == ct.ActionFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeChange == ct.VolumeChange);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionDate, ct.ActionDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionTime, ct.ActionTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionLocalID, ct.ActionLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (OrderActionStatus == ct.OrderActionStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StatusMsg, ct.StatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��������������
struct ExchangeOrderActionField
{
	///����������
	TExchangeIDType	ExchangeID;
	///�������
	TOrderSysIDType	OrderSysID;
	///������־
	TActionFlagType	ActionFlag;
	///�۸�
	TPriceType	LimitPrice;
	///�����仯
	TVolumeType	VolumeChange;
	///��������
	TDateType	ActionDate;
	///����ʱ��
	TTimeType	ActionTime;
	///����������Ա����
	TTraderIDType	TraderID;
	///��װ���
	TInstallIDType	InstallID;
	///���ر������
	TOrderLocalIDType	OrderLocalID;
	///�������ر��
	TOrderLocalIDType	ActionLocalID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///�ͻ�����
	TClientIDType	ClientID;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///��������״̬
	TOrderActionStatusType	OrderActionStatus;
	///�û�����
	TUserIDType	UserID;

bool operator==(const ExchangeOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (ActionFlag == ct.ActionFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeChange == ct.VolumeChange);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionDate, ct.ActionDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionTime, ct.ActionTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionLocalID, ct.ActionLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (OrderActionStatus == ct.OrderActionStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��������������ʧ��
struct ExchangeOrderActionErrorField
{
	///����������
	TExchangeIDType	ExchangeID;
	///�������
	TOrderSysIDType	OrderSysID;
	///����������Ա����
	TTraderIDType	TraderID;
	///��װ���
	TInstallIDType	InstallID;
	///���ر������
	TOrderLocalIDType	OrderLocalID;
	///�������ر��
	TOrderLocalIDType	ActionLocalID;
	///�������
	TErrorIDType	ErrorID;
	///������Ϣ
	TErrorMsgType	ErrorMsg;

bool operator==(const ExchangeOrderActionErrorField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionLocalID, ct.ActionLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�������ɽ�
struct ExchangeTradeField
{
	///����������
	TExchangeIDType	ExchangeID;
	///�ɽ����
	TTradeIDType	TradeID;
	///��������
	TDirectionType	Direction;
	///�������
	TOrderSysIDType	OrderSysID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///�ͻ�����
	TClientIDType	ClientID;
	///���׽�ɫ
	TTradingRoleType	TradingRole;
	///��Լ�ڽ������Ĵ���
	TExchangeInstIDType	ExchangeInstID;
	///��ƽ��־
	TOffsetFlagType	OffsetFlag;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///�۸�
	TPriceType	Price;
	///����
	TVolumeType	Volume;
	///�ɽ�ʱ��
	TDateType	TradeDate;
	///�ɽ�ʱ��
	TTimeType	TradeTime;
	///�ɽ�����
	TTradeTypeType	TradeType;
	///�ɽ�����Դ
	TPriceSourceType	PriceSource;
	///����������Ա����
	TTraderIDType	TraderID;
	///���ر������
	TOrderLocalIDType	OrderLocalID;
	///�����Ա���
	TParticipantIDType	ClearingPartID;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///���
	TSequenceNoType	SequenceNo;
	///�ɽ���Դ
	TTradeSourceType	TradeSource;

bool operator==(const ExchangeTradeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeID, ct.TradeID) == 0);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (TradingRole == ct.TradingRole);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (OffsetFlag == ct.OffsetFlag);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price, ct.Price));
	if (!ret) return ret;
	ret = ret && (Volume == ct.Volume);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeDate, ct.TradeDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeTime, ct.TradeTime) == 0);
	if (!ret) return ret;
	ret = ret && (TradeType == ct.TradeType);
	if (!ret) return ret;
	ret = ret && (PriceSource == ct.PriceSource);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClearingPartID, ct.ClearingPartID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (TradeSource == ct.TradeSource);
	if (!ret) return ret;

	return ret;
}
};

///�ɽ�
struct TradeField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///��������
	TOrderRefType	OrderRef;
	///�û�����
	TUserIDType	UserID;
	///����������
	TExchangeIDType	ExchangeID;
	///�ɽ����
	TTradeIDType	TradeID;
	///��������
	TDirectionType	Direction;
	///�������
	TOrderSysIDType	OrderSysID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///�ͻ�����
	TClientIDType	ClientID;
	///���׽�ɫ
	TTradingRoleType	TradingRole;
	///��Լ�ڽ������Ĵ���
	TExchangeInstIDType	ExchangeInstID;
	///��ƽ��־
	TOffsetFlagType	OffsetFlag;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///�۸�
	TPriceType	Price;
	///����
	TVolumeType	Volume;
	///�ɽ�ʱ��
	TDateType	TradeDate;
	///�ɽ�ʱ��
	TTimeType	TradeTime;
	///�ɽ�����
	TTradeTypeType	TradeType;
	///�ɽ�����Դ
	TPriceSourceType	PriceSource;
	///����������Ա����
	TTraderIDType	TraderID;
	///���ر������
	TOrderLocalIDType	OrderLocalID;
	///�����Ա���
	TParticipantIDType	ClearingPartID;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///���
	TSequenceNoType	SequenceNo;
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;
	///���͹�˾�������
	TSequenceNoType	BrokerOrderSeq;
	///�ɽ���Դ
	TTradeSourceType	TradeSource;

bool operator==(const TradeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeID, ct.TradeID) == 0);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (TradingRole == ct.TradingRole);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (OffsetFlag == ct.OffsetFlag);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price, ct.Price));
	if (!ret) return ret;
	ret = ret && (Volume == ct.Volume);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeDate, ct.TradeDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeTime, ct.TradeTime) == 0);
	if (!ret) return ret;
	ret = ret && (TradeType == ct.TradeType);
	if (!ret) return ret;
	ret = ret && (PriceSource == ct.PriceSource);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClearingPartID, ct.ClearingPartID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (BrokerOrderSeq == ct.BrokerOrderSeq);
	if (!ret) return ret;
	ret = ret && (TradeSource == ct.TradeSource);
	if (!ret) return ret;

	return ret;
}
};

///�û��Ự
struct UserSessionField
{
	///ǰ�ñ��
	TFrontIDType	FrontID;
	///�Ự���
	TSessionIDType	SessionID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///��¼����
	TDateType	LoginDate;
	///��¼ʱ��
	TTimeType	LoginTime;
	///IP��ַ
	TIPAddressType	IPAddress;
	///�û��˲�Ʒ��Ϣ
	TProductInfoType	UserProductInfo;
	///�ӿڶ˲�Ʒ��Ϣ
	TProductInfoType	InterfaceProductInfo;
	///Э����Ϣ
	TProtocolInfoType	ProtocolInfo;
	///Mac��ַ
	TMacAddressType	MacAddress;

bool operator==(const UserSessionField &ct) const
{
	bool ret = true;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LoginDate, ct.LoginDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LoginTime, ct.LoginTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(IPAddress, ct.IPAddress) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserProductInfo, ct.UserProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InterfaceProductInfo, ct.InterfaceProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProtocolInfo, ct.ProtocolInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(MacAddress, ct.MacAddress) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ��󱨵�����
struct QueryMaxOrderVolumeField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///��������
	TDirectionType	Direction;
	///��ƽ��־
	TOffsetFlagType	OffsetFlag;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///�������������
	TVolumeType	MaxVolume;

bool operator==(const QueryMaxOrderVolumeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (OffsetFlag == ct.OffsetFlag);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (MaxVolume == ct.MaxVolume);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���߽�����ȷ����Ϣ
struct SettlementInfoConfirmField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///ȷ������
	TDateType	ConfirmDate;
	///ȷ��ʱ��
	TTimeType	ConfirmTime;

bool operator==(const SettlementInfoConfirmField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ConfirmDate, ct.ConfirmDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ConfirmTime, ct.ConfirmTime) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�����ͬ��
struct SyncDepositField
{
	///�������ˮ��
	TDepositSeqNoType	DepositSeqNo;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�����
	TMoneyType	Deposit;
	///�Ƿ�ǿ�ƽ���
	TBoolType	IsForce;

bool operator==(const SyncDepositField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(DepositSeqNo, ct.DepositSeqNo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Deposit, ct.Deposit));
	if (!ret) return ret;
	ret = ret && (IsForce == ct.IsForce);
	if (!ret) return ret;

	return ret;
}
};

///���͹�˾ͬ��
struct BrokerSyncField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;

bool operator==(const BrokerSyncField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///����ͬ���е�Ͷ����
struct SyncingInvestorField
{
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���߷������
	TInvestorIDType	InvestorGroupID;
	///Ͷ��������
	TPartyNameType	InvestorName;
	///֤������
	TIdCardTypeType	IdentifiedCardType;
	///֤������
	TIdentifiedCardNoType	IdentifiedCardNo;
	///�Ƿ��Ծ
	TBoolType	IsActive;
	///��ϵ�绰
	TTelephoneType	Telephone;
	///ͨѶ��ַ
	TAddressType	Address;
	///��������
	TDateType	OpenDate;
	///�ֻ�
	TMobileType	Mobile;
	///��������ģ�����
	TInvestorIDType	CommModelID;

bool operator==(const SyncingInvestorField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupID, ct.InvestorGroupID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorName, ct.InvestorName) == 0);
	if (!ret) return ret;
	ret = ret && (IdentifiedCardType == ct.IdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(IdentifiedCardNo, ct.IdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (strcmp(Telephone, ct.Telephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Address, ct.Address) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenDate, ct.OpenDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Mobile, ct.Mobile) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CommModelID, ct.CommModelID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///����ͬ���еĽ��״���
struct SyncingTradingCodeField
{
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///����������
	TExchangeIDType	ExchangeID;
	///�ͻ�����
	TClientIDType	ClientID;
	///�Ƿ��Ծ
	TBoolType	IsActive;
	///���ױ�������
	TClientIDTypeType	ClientIDType;

bool operator==(const SyncingTradingCodeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (ClientIDType == ct.ClientIDType);
	if (!ret) return ret;

	return ret;
}
};

///����ͬ���е�Ͷ���߷���
struct SyncingInvestorGroupField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���߷������
	TInvestorIDType	InvestorGroupID;
	///Ͷ���߷�������
	TInvestorGroupNameType	InvestorGroupName;

bool operator==(const SyncingInvestorGroupField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupID, ct.InvestorGroupID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupName, ct.InvestorGroupName) == 0);
	if (!ret) return ret;

	return ret;
}
};

///����ͬ���еĽ����˺�
struct SyncingTradingAccountField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TAccountIDType	AccountID;
	///�ϴ���Ѻ���
	TMoneyType	PreMortgage;
	///�ϴ����ö��
	TMoneyType	PreCredit;
	///�ϴδ���
	TMoneyType	PreDeposit;
	///�ϴν���׼����
	TMoneyType	PreBalance;
	///�ϴ�ռ�õı�֤��
	TMoneyType	PreMargin;
	///��Ϣ����
	TMoneyType	InterestBase;
	///��Ϣ����
	TMoneyType	Interest;
	///�����
	TMoneyType	Deposit;
	///������
	TMoneyType	Withdraw;
	///����ı�֤��
	TMoneyType	FrozenMargin;
	///������ʽ�
	TMoneyType	FrozenCash;
	///�����������
	TMoneyType	FrozenCommission;
	///��ǰ��֤���ܶ�
	TMoneyType	CurrMargin;
	///�ʽ���
	TMoneyType	CashIn;
	///������
	TMoneyType	Commission;
	///ƽ��ӯ��
	TMoneyType	CloseProfit;
	///�ֲ�ӯ��
	TMoneyType	PositionProfit;
	///�ڻ�����׼����
	TMoneyType	Balance;
	///�����ʽ�
	TMoneyType	Available;
	///��ȡ�ʽ�
	TMoneyType	WithdrawQuota;
	///����׼����
	TMoneyType	Reserve;
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;
	///���ö��
	TMoneyType	Credit;
	///��Ѻ���
	TMoneyType	Mortgage;
	///��������֤��
	TMoneyType	ExchangeMargin;
	///Ͷ���߽��֤��
	TMoneyType	DeliveryMargin;
	///���������֤��
	TMoneyType	ExchangeDeliveryMargin;

bool operator==(const SyncingTradingAccountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(AccountID, ct.AccountID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMortgage, ct.PreMortgage));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreCredit, ct.PreCredit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreDeposit, ct.PreDeposit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreBalance, ct.PreBalance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMargin, ct.PreMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(InterestBase, ct.InterestBase));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Interest, ct.Interest));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Deposit, ct.Deposit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Withdraw, ct.Withdraw));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCash, ct.FrozenCash));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCommission, ct.FrozenCommission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrMargin, ct.CurrMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CashIn, ct.CashIn));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Commission, ct.Commission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfit, ct.PositionProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance, ct.Balance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Available, ct.Available));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(WithdrawQuota, ct.WithdrawQuota));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Reserve, ct.Reserve));
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Credit, ct.Credit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Mortgage, ct.Mortgage));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeMargin, ct.ExchangeMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(DeliveryMargin, ct.DeliveryMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeDeliveryMargin, ct.ExchangeDeliveryMargin));
	if (!ret) return ret;

	return ret;
}
};

///����ͬ���е�Ͷ���ֲ߳�
struct SyncingInvestorPositionField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�ֲֶ�շ���
	TPosiDirectionType	PosiDirection;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///�ֲ�����
	TPositionDateType	PositionDate;
	///���ճֲ�
	TVolumeType	YdPosition;
	///���ճֲ�
	TVolumeType	Position;
	///��ͷ����
	TVolumeType	LongFrozen;
	///��ͷ����
	TVolumeType	ShortFrozen;
	///���ֶ�����
	TMoneyType	LongFrozenAmount;
	///���ֶ�����
	TMoneyType	ShortFrozenAmount;
	///������
	TVolumeType	OpenVolume;
	///ƽ����
	TVolumeType	CloseVolume;
	///���ֽ��
	TMoneyType	OpenAmount;
	///ƽ�ֽ��
	TMoneyType	CloseAmount;
	///�ֲֳɱ�
	TMoneyType	PositionCost;
	///�ϴ�ռ�õı�֤��
	TMoneyType	PreMargin;
	///ռ�õı�֤��
	TMoneyType	UseMargin;
	///����ı�֤��
	TMoneyType	FrozenMargin;
	///������ʽ�
	TMoneyType	FrozenCash;
	///�����������
	TMoneyType	FrozenCommission;
	///�ʽ���
	TMoneyType	CashIn;
	///������
	TMoneyType	Commission;
	///ƽ��ӯ��
	TMoneyType	CloseProfit;
	///�ֲ�ӯ��
	TMoneyType	PositionProfit;
	///�ϴν����
	TPriceType	PreSettlementPrice;
	///���ν����
	TPriceType	SettlementPrice;
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;
	///���ֳɱ�
	TMoneyType	OpenCost;
	///��������֤��
	TMoneyType	ExchangeMargin;
	///��ϳɽ��γɵĳֲ�
	TVolumeType	CombPosition;
	///��϶�ͷ����
	TVolumeType	CombLongFrozen;
	///��Ͽ�ͷ����
	TVolumeType	CombShortFrozen;
	///���ն���ƽ��ӯ��
	TMoneyType	CloseProfitByDate;
	///��ʶԳ�ƽ��ӯ��
	TMoneyType	CloseProfitByTrade;
	///���ճֲ�
	TVolumeType	TodayPosition;
	///��֤����
	TRatioType	MarginRateByMoney;
	///��֤����(������)
	TRatioType	MarginRateByVolume;

bool operator==(const SyncingInvestorPositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (PosiDirection == ct.PosiDirection);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (PositionDate == ct.PositionDate);
	if (!ret) return ret;
	ret = ret && (YdPosition == ct.YdPosition);
	if (!ret) return ret;
	ret = ret && (Position == ct.Position);
	if (!ret) return ret;
	ret = ret && (LongFrozen == ct.LongFrozen);
	if (!ret) return ret;
	ret = ret && (ShortFrozen == ct.ShortFrozen);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongFrozenAmount, ct.LongFrozenAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortFrozenAmount, ct.ShortFrozenAmount));
	if (!ret) return ret;
	ret = ret && (OpenVolume == ct.OpenVolume);
	if (!ret) return ret;
	ret = ret && (CloseVolume == ct.CloseVolume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenAmount, ct.OpenAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseAmount, ct.CloseAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionCost, ct.PositionCost));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMargin, ct.PreMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UseMargin, ct.UseMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCash, ct.FrozenCash));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCommission, ct.FrozenCommission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CashIn, ct.CashIn));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Commission, ct.Commission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfit, ct.PositionProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreSettlementPrice, ct.PreSettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SettlementPrice, ct.SettlementPrice));
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenCost, ct.OpenCost));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeMargin, ct.ExchangeMargin));
	if (!ret) return ret;
	ret = ret && (CombPosition == ct.CombPosition);
	if (!ret) return ret;
	ret = ret && (CombLongFrozen == ct.CombLongFrozen);
	if (!ret) return ret;
	ret = ret && (CombShortFrozen == ct.CombShortFrozen);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByDate, ct.CloseProfitByDate));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByTrade, ct.CloseProfitByTrade));
	if (!ret) return ret;
	ret = ret && (TodayPosition == ct.TodayPosition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByMoney, ct.MarginRateByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByVolume, ct.MarginRateByVolume));
	if (!ret) return ret;

	return ret;
}
};

///����ͬ���еĺ�Լ��֤����
struct SyncingInstrumentMarginRateField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	TInvestorRangeType	InvestorRange;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///��ͷ��֤����
	TRatioType	LongMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	LongMarginRatioByVolume;
	///��ͷ��֤����
	TRatioType	ShortMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	ShortMarginRatioByVolume;
	///�Ƿ���Խ�������ȡ
	TBoolType	IsRelative;

bool operator==(const SyncingInstrumentMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (IsRelative == ct.IsRelative);
	if (!ret) return ret;

	return ret;
}
};

///����ͬ���еĺ�Լ��������
struct SyncingInstrumentCommissionRateField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	TInvestorRangeType	InvestorRange;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///������������
	TRatioType	OpenRatioByMoney;
	///����������
	TRatioType	OpenRatioByVolume;
	///ƽ����������
	TRatioType	CloseRatioByMoney;
	///ƽ��������
	TRatioType	CloseRatioByVolume;
	///ƽ����������
	TRatioType	CloseTodayRatioByMoney;
	///ƽ��������
	TRatioType	CloseTodayRatioByVolume;

bool operator==(const SyncingInstrumentCommissionRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenRatioByMoney, ct.OpenRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenRatioByVolume, ct.OpenRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseRatioByMoney, ct.CloseRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseRatioByVolume, ct.CloseRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseTodayRatioByMoney, ct.CloseTodayRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseTodayRatioByVolume, ct.CloseTodayRatioByVolume));
	if (!ret) return ret;

	return ret;
}
};

///����ͬ���еĺ�Լ����Ȩ��
struct SyncingInstrumentTradingRightField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	TInvestorRangeType	InvestorRange;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///����Ȩ��
	TTradingRightType	TradingRight;

bool operator==(const SyncingInstrumentTradingRightField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (TradingRight == ct.TradingRight);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ����
struct QryOrderField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///����������
	TExchangeIDType	ExchangeID;
	///�������
	TOrderSysIDType	OrderSysID;
	///��ʼʱ��
	TTimeType	InsertTimeStart;
	///����ʱ��
	TTimeType	InsertTimeEnd;

bool operator==(const QryOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertTimeStart, ct.InsertTimeStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertTimeEnd, ct.InsertTimeEnd) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ�ɽ�
struct QryTradeField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///����������
	TExchangeIDType	ExchangeID;
	///�ɽ����
	TTradeIDType	TradeID;
	///��ʼʱ��
	TTimeType	TradeTimeStart;
	///����ʱ��
	TTimeType	TradeTimeEnd;

bool operator==(const QryTradeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeID, ct.TradeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeTimeStart, ct.TradeTimeStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeTimeEnd, ct.TradeTimeEnd) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯͶ���ֲ߳�
struct QryInvestorPositionField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;

bool operator==(const QryInvestorPositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ�ʽ��˻�
struct QryTradingAccountField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;

bool operator==(const QryTradingAccountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯͶ����
struct QryInvestorField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;

bool operator==(const QryInvestorField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���ױ���
struct QryTradingCodeField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///����������
	TExchangeIDType	ExchangeID;
	///�ͻ�����
	TClientIDType	ClientID;
	///���ױ�������
	TClientIDTypeType	ClientIDType;

bool operator==(const QryTradingCodeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (ClientIDType == ct.ClientIDType);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���ױ���
struct QryInvestorGroupField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;

bool operator==(const QryInvestorGroupField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���ױ���
struct QryInstrumentMarginRateField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;

bool operator==(const QryInstrumentMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���ױ���
struct QryInstrumentCommissionRateField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;

bool operator==(const QryInstrumentCommissionRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���ױ���
struct QryInstrumentTradingRightField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;

bool operator==(const QryInstrumentTradingRightField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���͹�˾
struct QryBrokerField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;

bool operator==(const QryBrokerField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ����Ա
struct QryTraderField
{
	///����������
	TExchangeIDType	ExchangeID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///����������Ա����
	TTraderIDType	TraderID;

bool operator==(const QryTraderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���͹�˾��Ա����
struct QryPartBrokerField
{
	///����������
	TExchangeIDType	ExchangeID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��Ա����
	TParticipantIDType	ParticipantID;

bool operator==(const QryPartBrokerField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ�����û�����Ȩ��
struct QrySuperUserFunctionField
{
	///�û�����
	TUserIDType	UserID;

bool operator==(const QrySuperUserFunctionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ�û��Ự
struct QryUserSessionField
{
	///ǰ�ñ��
	TFrontIDType	FrontID;
	///�Ự���
	TSessionIDType	SessionID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;

bool operator==(const QryUserSessionField &ct) const
{
	bool ret = true;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯǰ��״̬
struct QryFrontStatusField
{
	///ǰ�ñ��
	TFrontIDType	FrontID;

bool operator==(const QryFrontStatusField &ct) const
{
	bool ret = true;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ����������
struct QryExchangeOrderField
{
	///��Ա����
	TParticipantIDType	ParticipantID;
	///�ͻ�����
	TClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TExchangeInstIDType	ExchangeInstID;
	///����������
	TExchangeIDType	ExchangeID;
	///����������Ա����
	TTraderIDType	TraderID;

bool operator==(const QryExchangeOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ��������
struct QryOrderActionField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///����������
	TExchangeIDType	ExchangeID;

bool operator==(const QryOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ��������������
struct QryExchangeOrderActionField
{
	///��Ա����
	TParticipantIDType	ParticipantID;
	///�ͻ�����
	TClientIDType	ClientID;
	///����������
	TExchangeIDType	ExchangeID;
	///����������Ա����
	TTraderIDType	TraderID;

bool operator==(const QryExchangeOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ�����û�
struct QrySuperUserField
{
	///�û�����
	TUserIDType	UserID;

bool operator==(const QrySuperUserField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ������
struct QryExchangeField
{
	///����������
	TExchangeIDType	ExchangeID;

bool operator==(const QryExchangeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ��Ʒ
struct QryProductField
{
	///��Ʒ����
	TInstrumentIDType	ProductID;

bool operator==(const QryProductField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ��Լ
struct QryInstrumentField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///����������
	TExchangeIDType	ExchangeID;
	///��Լ�ڽ������Ĵ���
	TExchangeInstIDType	ExchangeInstID;
	///��Ʒ����
	TInstrumentIDType	ProductID;

bool operator==(const QryInstrumentField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ����
struct QryDepthMarketDataField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;

bool operator==(const QryDepthMarketDataField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���͹�˾�û�
struct QryBrokerUserField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;

bool operator==(const QryBrokerUserField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���͹�˾�û�Ȩ��
struct QryBrokerUserFunctionField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;

bool operator==(const QryBrokerUserFunctionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ����Ա���̻�
struct QryTraderOfferField
{
	///����������
	TExchangeIDType	ExchangeID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///����������Ա����
	TTraderIDType	TraderID;

bool operator==(const QryTraderOfferField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ�������ˮ
struct QrySyncDepositField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�������ˮ��
	TDepositSeqNoType	DepositSeqNo;

bool operator==(const QrySyncDepositField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(DepositSeqNo, ct.DepositSeqNo) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯͶ���߽�����
struct QrySettlementInfoField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///������
	TDateType	TradingDay;

bool operator==(const QrySettlementInfoField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ����
struct QryHisOrderField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///����������
	TExchangeIDType	ExchangeID;
	///�������
	TOrderSysIDType	OrderSysID;
	///��ʼʱ��
	TTimeType	InsertTimeStart;
	///����ʱ��
	TTimeType	InsertTimeEnd;
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;

bool operator==(const QryHisOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertTimeStart, ct.InsertTimeStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertTimeEnd, ct.InsertTimeEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;

	return ret;
}
};

///��֯�ܹ�Ͷ���߶�Ӧ��ϵ��ƽ��
struct InvestorDepartmentFlatField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��֯�ܹ�����
	TInvestorIDType	DepartmentID;

bool operator==(const InvestorDepartmentFlatField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(DepartmentID, ct.DepartmentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///����Ա��֯�ܹ���ϵ
struct DepartmentUserField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///Ͷ���߷�Χ
	TDepartmentRangeType	InvestorRange;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;

bool operator==(const DepartmentUserField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��֯�ܹ�
struct DepartmentField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��֯�ܹ�����
	TInvestorIDType	DepartmentID;
	///��֯�ܹ�����
	TPartyNameType	DepartmentName;

bool operator==(const DepartmentField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(DepartmentID, ct.DepartmentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(DepartmentName, ct.DepartmentName) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���͹�˾�ʽ�
struct QueryBrokerDepositField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///����������
	TExchangeIDType	ExchangeID;

bool operator==(const QueryBrokerDepositField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///���͹�˾�ʽ�
struct BrokerDepositField
{
	///��������
	TTradeDateType	TradingDay;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///����������
	TExchangeIDType	ExchangeID;
	///�ϴν���׼����
	TMoneyType	PreBalance;
	///��ǰ��֤���ܶ�
	TMoneyType	CurrMargin;
	///ƽ��ӯ��
	TMoneyType	CloseProfit;
	///�ڻ�����׼����
	TMoneyType	Balance;
	///�����
	TMoneyType	Deposit;
	///������
	TMoneyType	Withdraw;
	///�����ʽ�
	TMoneyType	Available;
	///����׼����
	TMoneyType	Reserve;
	///����ı�֤��
	TMoneyType	FrozenMargin;

bool operator==(const BrokerDepositField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreBalance, ct.PreBalance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrMargin, ct.CurrMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance, ct.Balance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Deposit, ct.Deposit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Withdraw, ct.Withdraw));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Available, ct.Available));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Reserve, ct.Reserve));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;

	return ret;
}
};

///�г�����
struct MarketDataField
{
	///������
	TDateType	TradingDay;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///����������
	TExchangeIDType	ExchangeID;
	///��Լ�ڽ������Ĵ���
	TExchangeInstIDType	ExchangeInstID;
	///���¼�
	TPriceType	LastPrice;
	///�ϴν����
	TPriceType	PreSettlementPrice;
	///������
	TPriceType	PreClosePrice;
	///��ֲ���
	TLargeVolumeType	PreOpenInterest;
	///����
	TPriceType	OpenPrice;
	///��߼�
	TPriceType	HighestPrice;
	///��ͼ�
	TPriceType	LowestPrice;
	///����
	TVolumeType	Volume;
	///�ɽ����
	TMoneyType	Turnover;
	///�ֲ���
	TLargeVolumeType	OpenInterest;
	///������
	TPriceType	ClosePrice;
	///���ν����
	TPriceType	SettlementPrice;
	///��ͣ���
	TPriceType	UpperLimitPrice;
	///��ͣ���
	TPriceType	LowerLimitPrice;
	///����ʵ��
	TRatioType	PreDelta;
	///����ʵ��
	TRatioType	CurrDelta;
	///����޸�ʱ��
	TTimeType	UpdateTime;
	///����޸ĺ���
	TMillisecType	UpdateMillisec;

bool operator==(const MarketDataField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LastPrice, ct.LastPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreSettlementPrice, ct.PreSettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreClosePrice, ct.PreClosePrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreOpenInterest, ct.PreOpenInterest));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenPrice, ct.OpenPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(HighestPrice, ct.HighestPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LowestPrice, ct.LowestPrice));
	if (!ret) return ret;
	ret = ret && (Volume == ct.Volume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Turnover, ct.Turnover));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenInterest, ct.OpenInterest));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ClosePrice, ct.ClosePrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SettlementPrice, ct.SettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UpperLimitPrice, ct.UpperLimitPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LowerLimitPrice, ct.LowerLimitPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreDelta, ct.PreDelta));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrDelta, ct.CurrDelta));
	if (!ret) return ret;
	ret = ret && (strcmp(UpdateTime, ct.UpdateTime) == 0);
	if (!ret) return ret;
	ret = ret && (UpdateMillisec == ct.UpdateMillisec);
	if (!ret) return ret;

	return ret;
}
};

///�����������
struct MarketDataBaseField
{
	///������
	TDateType	TradingDay;
	///�ϴν����
	TPriceType	PreSettlementPrice;
	///������
	TPriceType	PreClosePrice;
	///��ֲ���
	TLargeVolumeType	PreOpenInterest;
	///����ʵ��
	TRatioType	PreDelta;

bool operator==(const MarketDataBaseField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreSettlementPrice, ct.PreSettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreClosePrice, ct.PreClosePrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreOpenInterest, ct.PreOpenInterest));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreDelta, ct.PreDelta));
	if (!ret) return ret;

	return ret;
}
};

///���龲̬����
struct MarketDataStaticField
{
	///����
	TPriceType	OpenPrice;
	///��߼�
	TPriceType	HighestPrice;
	///��ͼ�
	TPriceType	LowestPrice;
	///������
	TPriceType	ClosePrice;
	///��ͣ���
	TPriceType	UpperLimitPrice;
	///��ͣ���
	TPriceType	LowerLimitPrice;
	///���ν����
	TPriceType	SettlementPrice;
	///����ʵ��
	TRatioType	CurrDelta;

bool operator==(const MarketDataStaticField &ct) const
{
	bool ret = true;
	ret = ret && (EQUALDOUBLE(OpenPrice, ct.OpenPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(HighestPrice, ct.HighestPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LowestPrice, ct.LowestPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ClosePrice, ct.ClosePrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UpperLimitPrice, ct.UpperLimitPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LowerLimitPrice, ct.LowerLimitPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SettlementPrice, ct.SettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrDelta, ct.CurrDelta));
	if (!ret) return ret;

	return ret;
}
};

///�������³ɽ�����
struct MarketDataLastMatchField
{
	///���¼�
	TPriceType	LastPrice;
	///����
	TVolumeType	Volume;
	///�ɽ����
	TMoneyType	Turnover;
	///�ֲ���
	TLargeVolumeType	OpenInterest;

bool operator==(const MarketDataLastMatchField &ct) const
{
	bool ret = true;
	ret = ret && (EQUALDOUBLE(LastPrice, ct.LastPrice));
	if (!ret) return ret;
	ret = ret && (Volume == ct.Volume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Turnover, ct.Turnover));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenInterest, ct.OpenInterest));
	if (!ret) return ret;

	return ret;
}
};

///�������ż�����
struct MarketDataBestPriceField
{
	///�����һ
	TPriceType	BidPrice1;
	///������һ
	TVolumeType	BidVolume1;
	///������һ
	TPriceType	AskPrice1;
	///������һ
	TVolumeType	AskVolume1;

bool operator==(const MarketDataBestPriceField &ct) const
{
	bool ret = true;
	ret = ret && (EQUALDOUBLE(BidPrice1, ct.BidPrice1));
	if (!ret) return ret;
	ret = ret && (BidVolume1 == ct.BidVolume1);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AskPrice1, ct.AskPrice1));
	if (!ret) return ret;
	ret = ret && (AskVolume1 == ct.AskVolume1);
	if (!ret) return ret;

	return ret;
}
};

///�����������������
struct MarketDataBid23Field
{
	///����۶�
	TPriceType	BidPrice2;
	///��������
	TVolumeType	BidVolume2;
	///�������
	TPriceType	BidPrice3;
	///��������
	TVolumeType	BidVolume3;

bool operator==(const MarketDataBid23Field &ct) const
{
	bool ret = true;
	ret = ret && (EQUALDOUBLE(BidPrice2, ct.BidPrice2));
	if (!ret) return ret;
	ret = ret && (BidVolume2 == ct.BidVolume2);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BidPrice3, ct.BidPrice3));
	if (!ret) return ret;
	ret = ret && (BidVolume3 == ct.BidVolume3);
	if (!ret) return ret;

	return ret;
}
};

///������������������
struct MarketDataAsk23Field
{
	///�����۶�
	TPriceType	AskPrice2;
	///��������
	TVolumeType	AskVolume2;
	///��������
	TPriceType	AskPrice3;
	///��������
	TVolumeType	AskVolume3;

bool operator==(const MarketDataAsk23Field &ct) const
{
	bool ret = true;
	ret = ret && (EQUALDOUBLE(AskPrice2, ct.AskPrice2));
	if (!ret) return ret;
	ret = ret && (AskVolume2 == ct.AskVolume2);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AskPrice3, ct.AskPrice3));
	if (!ret) return ret;
	ret = ret && (AskVolume3 == ct.AskVolume3);
	if (!ret) return ret;

	return ret;
}
};

///���������ġ�������
struct MarketDataBid45Field
{
	///�������
	TPriceType	BidPrice4;
	///��������
	TVolumeType	BidVolume4;
	///�������
	TPriceType	BidPrice5;
	///��������
	TVolumeType	BidVolume5;

bool operator==(const MarketDataBid45Field &ct) const
{
	bool ret = true;
	ret = ret && (EQUALDOUBLE(BidPrice4, ct.BidPrice4));
	if (!ret) return ret;
	ret = ret && (BidVolume4 == ct.BidVolume4);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BidPrice5, ct.BidPrice5));
	if (!ret) return ret;
	ret = ret && (BidVolume5 == ct.BidVolume5);
	if (!ret) return ret;

	return ret;
}
};

///���������ġ�������
struct MarketDataAsk45Field
{
	///��������
	TPriceType	AskPrice4;
	///��������
	TVolumeType	AskVolume4;
	///��������
	TPriceType	AskPrice5;
	///��������
	TVolumeType	AskVolume5;

bool operator==(const MarketDataAsk45Field &ct) const
{
	bool ret = true;
	ret = ret && (EQUALDOUBLE(AskPrice4, ct.AskPrice4));
	if (!ret) return ret;
	ret = ret && (AskVolume4 == ct.AskVolume4);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AskPrice5, ct.AskPrice5));
	if (!ret) return ret;
	ret = ret && (AskVolume5 == ct.AskVolume5);
	if (!ret) return ret;

	return ret;
}
};

///�������ʱ������
struct MarketDataUpdateTimeField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///����޸�ʱ��
	TTimeType	UpdateTime;
	///����޸ĺ���
	TMillisecType	UpdateMillisec;

bool operator==(const MarketDataUpdateTimeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UpdateTime, ct.UpdateTime) == 0);
	if (!ret) return ret;
	ret = ret && (UpdateMillisec == ct.UpdateMillisec);
	if (!ret) return ret;

	return ret;
}
};

///ָ���ĺ�Լ
struct SpecificInstrumentField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;

bool operator==(const SpecificInstrumentField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��Լ״̬
struct InstrumentStatusField
{
	///����������
	TExchangeIDType	ExchangeID;
	///��Լ�ڽ������Ĵ���
	TExchangeInstIDType	ExchangeInstID;
	///���������
	TSettlementGroupIDType	SettlementGroupID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///��Լ����״̬
	TInstrumentStatusType	InstrumentStatus;
	///���׽׶α��
	TTradingSegmentSNType	TradingSegmentSN;
	///���뱾״̬ʱ��
	TTimeType	EnterTime;
	///���뱾״̬ԭ��
	TInstStatusEnterReasonType	EnterReason;

bool operator==(const InstrumentStatusField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlementGroupID, ct.SettlementGroupID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InstrumentStatus == ct.InstrumentStatus);
	if (!ret) return ret;
	ret = ret && (TradingSegmentSN == ct.TradingSegmentSN);
	if (!ret) return ret;
	ret = ret && (strcmp(EnterTime, ct.EnterTime) == 0);
	if (!ret) return ret;
	ret = ret && (EnterReason == ct.EnterReason);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ��Լ״̬
struct QryInstrumentStatusField
{
	///����������
	TExchangeIDType	ExchangeID;
	///��Լ�ڽ������Ĵ���
	TExchangeInstIDType	ExchangeInstID;

bool operator==(const QryInstrumentStatusField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ�����˻�
struct InvestorAccountField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///Ͷ�����ʺ�
	TAccountIDType	AccountID;

bool operator==(const InvestorAccountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(AccountID, ct.AccountID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///����ӯ���㷨
struct PositionProfitAlgorithmField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TAccountIDType	AccountID;
	///ӯ���㷨
	TAlgorithmType	Algorithm;
	///��ע
	TMemoType	Memo;

bool operator==(const PositionProfitAlgorithmField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(AccountID, ct.AccountID) == 0);
	if (!ret) return ret;
	ret = ret && (Algorithm == ct.Algorithm);
	if (!ret) return ret;
	ret = ret && (strcmp(Memo, ct.Memo) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��Ա�ʽ��ۿ�
struct DiscountField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���߷�Χ
	TInvestorRangeType	InvestorRange;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�ʽ��ۿ۱���
	TRatioType	Discount;

bool operator==(const DiscountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Discount, ct.Discount));
	if (!ret) return ret;

	return ret;
}
};

///��ѯת������
struct QryTransferBankField
{
	///���д���
	TBankIDType	BankID;
	///���з����Ĵ���
	TBankBrchIDType	BankBrchID;

bool operator==(const QryTransferBankField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BankID, ct.BankID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankBrchID, ct.BankBrchID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///ת������
struct TransferBankField
{
	///���д���
	TBankIDType	BankID;
	///���з����Ĵ���
	TBankBrchIDType	BankBrchID;
	///��������
	TBankNameType	BankName;
	///�Ƿ��Ծ
	TBoolType	IsActive;

bool operator==(const TransferBankField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BankID, ct.BankID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankBrchID, ct.BankBrchID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankName, ct.BankName) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;

	return ret;
}
};

///��ѯͶ���ֲ߳���ϸ
struct QryInvestorPositionDetailField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;

bool operator==(const QryInvestorPositionDetailField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���ֲ߳���ϸ
struct InvestorPositionDetailField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///����
	TDirectionType	Direction;
	///��������
	TDateType	OpenDate;
	///�ɽ����
	TTradeIDType	TradeID;
	///����
	TVolumeType	Volume;
	///���ּ�
	TPriceType	OpenPrice;
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;
	///�ɽ�����
	TTradeTypeType	TradeType;
	///��Ϻ�Լ����
	TInstrumentIDType	CombInstrumentID;
	///����������
	TExchangeIDType	ExchangeID;
	///���ն���ƽ��ӯ��
	TMoneyType	CloseProfitByDate;
	///��ʶԳ�ƽ��ӯ��
	TMoneyType	CloseProfitByTrade;
	///���ն��гֲ�ӯ��
	TMoneyType	PositionProfitByDate;
	///��ʶԳ�ֲ�ӯ��
	TMoneyType	PositionProfitByTrade;
	///Ͷ���߱�֤��
	TMoneyType	Margin;
	///��������֤��
	TMoneyType	ExchMargin;
	///��֤����
	TRatioType	MarginRateByMoney;
	///��֤����(������)
	TRatioType	MarginRateByVolume;
	///������
	TPriceType	LastSettlementPrice;
	///�����
	TPriceType	SettlementPrice;
	///ƽ����
	TVolumeType	CloseVolume;
	///ƽ�ֽ��
	TMoneyType	CloseAmount;

bool operator==(const InvestorPositionDetailField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenDate, ct.OpenDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeID, ct.TradeID) == 0);
	if (!ret) return ret;
	ret = ret && (Volume == ct.Volume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenPrice, ct.OpenPrice));
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (TradeType == ct.TradeType);
	if (!ret) return ret;
	ret = ret && (strcmp(CombInstrumentID, ct.CombInstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByDate, ct.CloseProfitByDate));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByTrade, ct.CloseProfitByTrade));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfitByDate, ct.PositionProfitByDate));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfitByTrade, ct.PositionProfitByTrade));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Margin, ct.Margin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchMargin, ct.ExchMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByMoney, ct.MarginRateByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByVolume, ct.MarginRateByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LastSettlementPrice, ct.LastSettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SettlementPrice, ct.SettlementPrice));
	if (!ret) return ret;
	ret = ret && (CloseVolume == ct.CloseVolume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseAmount, ct.CloseAmount));
	if (!ret) return ret;

	return ret;
}
};

///�ʽ��˻�������
struct TradingAccountPasswordField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TAccountIDType	AccountID;
	///����
	TPasswordType	Password;

bool operator==(const TradingAccountPasswordField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(AccountID, ct.AccountID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;

	return ret;
}
};

///���������鱨�̻�
struct MDTraderOfferField
{
	///����������
	TExchangeIDType	ExchangeID;
	///����������Ա����
	TTraderIDType	TraderID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///����
	TPasswordType	Password;
	///��װ���
	TInstallIDType	InstallID;
	///���ر������
	TOrderLocalIDType	OrderLocalID;
	///����������Ա����״̬
	TTraderConnectStatusType	TraderConnectStatus;
	///�����������������
	TDateType	ConnectRequestDate;
	///�������������ʱ��
	TTimeType	ConnectRequestTime;
	///�ϴα�������
	TDateType	LastReportDate;
	///�ϴα���ʱ��
	TTimeType	LastReportTime;
	///�����������
	TDateType	ConnectDate;
	///�������ʱ��
	TTimeType	ConnectTime;
	///��������
	TDateType	StartDate;
	///����ʱ��
	TTimeType	StartTime;
	///������
	TDateType	TradingDay;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��ϯλ���ɽ����
	TTradeIDType	MaxTradeID;
	///��ϯλ��󱨵�����
	TReturnCodeType	MaxOrderMessageReference;

bool operator==(const MDTraderOfferField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (TraderConnectStatus == ct.TraderConnectStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(ConnectRequestDate, ct.ConnectRequestDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ConnectRequestTime, ct.ConnectRequestTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LastReportDate, ct.LastReportDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LastReportTime, ct.LastReportTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ConnectDate, ct.ConnectDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ConnectTime, ct.ConnectTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StartDate, ct.StartDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StartTime, ct.StartTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(MaxTradeID, ct.MaxTradeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(MaxOrderMessageReference, ct.MaxOrderMessageReference) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���鱨�̻�
struct QryMDTraderOfferField
{
	///����������
	TExchangeIDType	ExchangeID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///����������Ա����
	TTraderIDType	TraderID;

bool operator==(const QryMDTraderOfferField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ�ͻ�֪ͨ
struct QryNoticeField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;

bool operator==(const QryNoticeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�ͻ�֪ͨ
struct NoticeField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��Ϣ����
	TContentType	Content;
	///���͹�˾֪ͨ�������к�
	TSequenceLabelType	SequenceLabel;

bool operator==(const NoticeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Content, ct.Content) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SequenceLabel, ct.SequenceLabel) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�û�Ȩ��
struct UserRightField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///�ͻ�Ȩ������
	TUserRightTypeType	UserRightType;
	///�Ƿ��ֹ
	TBoolType	IsForbidden;

bool operator==(const UserRightField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (UserRightType == ct.UserRightType);
	if (!ret) return ret;
	ret = ret && (IsForbidden == ct.IsForbidden);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ������Ϣȷ����
struct QrySettlementInfoConfirmField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;

bool operator==(const QrySettlementInfoConfirmField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///װ�ؽ�����Ϣ
struct LoadSettlementInfoField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;

bool operator==(const LoadSettlementInfoField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///���͹�˾�����ʽ��㷨��
struct BrokerWithdrawAlgorithmField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�����ʽ��㷨
	TAlgorithmType	WithdrawAlgorithm;
	///�ʽ�ʹ����
	TRatioType	UsingRatio;
	///�����Ƿ����ƽ��ӯ��
	TIncludeCloseProfitType	IncludeCloseProfit;
	///�����޲����޳ɽ��ͻ��Ƿ��ܿ����������
	TAllWithoutTradeType	AllWithoutTrade;
	///�����Ƿ����ƽ��ӯ��
	TIncludeCloseProfitType	AvailIncludeCloseProfit;
	///�Ƿ������û��¼�
	TBoolType	IsBrokerUserEvent;

bool operator==(const BrokerWithdrawAlgorithmField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (WithdrawAlgorithm == ct.WithdrawAlgorithm);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UsingRatio, ct.UsingRatio));
	if (!ret) return ret;
	ret = ret && (IncludeCloseProfit == ct.IncludeCloseProfit);
	if (!ret) return ret;
	ret = ret && (AllWithoutTrade == ct.AllWithoutTrade);
	if (!ret) return ret;
	ret = ret && (AvailIncludeCloseProfit == ct.AvailIncludeCloseProfit);
	if (!ret) return ret;
	ret = ret && (IsBrokerUserEvent == ct.IsBrokerUserEvent);
	if (!ret) return ret;

	return ret;
}
};

///�ʽ��˻���������
struct TradingAccountPasswordUpdateV1Field
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///ԭ���Ŀ���
	TPasswordType	OldPassword;
	///�µĿ���
	TPasswordType	NewPassword;

bool operator==(const TradingAccountPasswordUpdateV1Field &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OldPassword, ct.OldPassword) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(NewPassword, ct.NewPassword) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�ʽ��˻���������
struct TradingAccountPasswordUpdateField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ�����ʺ�
	TAccountIDType	AccountID;
	///ԭ���Ŀ���
	TPasswordType	OldPassword;
	///�µĿ���
	TPasswordType	NewPassword;

bool operator==(const TradingAccountPasswordUpdateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(AccountID, ct.AccountID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OldPassword, ct.OldPassword) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(NewPassword, ct.NewPassword) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ��Ϻ�Լ����
struct QryCombinationLegField
{
	///��Ϻ�Լ����
	TInstrumentIDType	CombInstrumentID;
	///���ȱ��
	TLegIDType	LegID;
	///���Ⱥ�Լ����
	TInstrumentIDType	LegInstrumentID;

bool operator==(const QryCombinationLegField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(CombInstrumentID, ct.CombInstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (LegID == ct.LegID);
	if (!ret) return ret;
	ret = ret && (strcmp(LegInstrumentID, ct.LegInstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ��Ϻ�Լ����
struct QrySyncStatusField
{
	///������
	TDateType	TradingDay;

bool operator==(const QrySyncStatusField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��Ͻ��׺�Լ�ĵ���
struct CombinationLegField
{
	///��Ϻ�Լ����
	TInstrumentIDType	CombInstrumentID;
	///���ȱ��
	TLegIDType	LegID;
	///���Ⱥ�Լ����
	TInstrumentIDType	LegInstrumentID;
	///��������
	TDirectionType	Direction;
	///���ȳ���
	TLegMultipleType	LegMultiple;
	///��������
	TImplyLevelType	ImplyLevel;

bool operator==(const CombinationLegField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(CombInstrumentID, ct.CombInstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (LegID == ct.LegID);
	if (!ret) return ret;
	ret = ret && (strcmp(LegInstrumentID, ct.LegInstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (LegMultiple == ct.LegMultiple);
	if (!ret) return ret;
	ret = ret && (ImplyLevel == ct.ImplyLevel);
	if (!ret) return ret;

	return ret;
}
};

///����ͬ��״̬
struct SyncStatusField
{
	///������
	TDateType	TradingDay;
	///����ͬ��״̬
	TDataSyncStatusType	DataSyncStatus;

bool operator==(const SyncStatusField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (DataSyncStatus == ct.DataSyncStatus);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ��ϵ��
struct QryLinkManField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;

bool operator==(const QryLinkManField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ϵ��
struct LinkManField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��ϵ������
	TPersonTypeType	PersonType;
	///֤������
	TIdCardTypeType	IdentifiedCardType;
	///֤������
	TIdentifiedCardNoType	IdentifiedCardNo;
	///����
	TPartyNameType	PersonName;
	///��ϵ�绰
	TTelephoneType	Telephone;
	///ͨѶ��ַ
	TAddressType	Address;
	///��������
	TZipCodeType	ZipCode;
	///���ȼ�
	TPriorityType	Priority;

bool operator==(const LinkManField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (PersonType == ct.PersonType);
	if (!ret) return ret;
	ret = ret && (IdentifiedCardType == ct.IdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(IdentifiedCardNo, ct.IdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(PersonName, ct.PersonName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Telephone, ct.Telephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Address, ct.Address) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ZipCode, ct.ZipCode) == 0);
	if (!ret) return ret;
	ret = ret && (Priority == ct.Priority);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���͹�˾�û��¼�
struct QryBrokerUserEventField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///�û��¼�����
	TUserEventTypeType	UserEventType;

bool operator==(const QryBrokerUserEventField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (UserEventType == ct.UserEventType);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���͹�˾�û��¼�
struct BrokerUserEventField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///�û��¼�����
	TUserEventTypeType	UserEventType;
	///�û��¼����
	TSequenceNoType	EventSequenceNo;
	///�¼���������
	TDateType	EventDate;
	///�¼�����ʱ��
	TTimeType	EventTime;
	///�û��¼���Ϣ
	TUserEventInfoType	UserEventInfo;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;

bool operator==(const BrokerUserEventField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (UserEventType == ct.UserEventType);
	if (!ret) return ret;
	ret = ret && (EventSequenceNo == ct.EventSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserEventInfo, ct.UserEventInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯǩԼ��������
struct QryContractBankField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///���д���
	TBankIDType	BankID;
	///���з����Ĵ���
	TBankBrchIDType	BankBrchID;

bool operator==(const QryContractBankField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankID, ct.BankID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankBrchID, ct.BankBrchID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯǩԼ������Ӧ
struct ContractBankField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///���д���
	TBankIDType	BankID;
	///���з����Ĵ���
	TBankBrchIDType	BankBrchID;
	///��������
	TBankNameType	BankName;

bool operator==(const ContractBankField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankID, ct.BankID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankBrchID, ct.BankBrchID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankName, ct.BankName) == 0);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ������ϳֲ���ϸ
struct InvestorPositionCombineDetailField
{
	///������
	TDateType	TradingDay;
	///��������
	TDateType	OpenDate;
	///����������
	TExchangeIDType	ExchangeID;
	///������
	TSettlementIDType	SettlementID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��ϱ��
	TTradeIDType	ComTradeID;
	///��ϱ��
	TTradeIDType	TradeID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///����
	TDirectionType	Direction;
	///�ֲ���
	TVolumeType	TotalAmt;
	///Ͷ���߱�֤��
	TMoneyType	Margin;
	///��������֤��
	TMoneyType	ExchMargin;
	///��֤����
	TRatioType	MarginRateByMoney;
	///��֤����(������)
	TRatioType	MarginRateByVolume;
	///���ȱ��
	TLegIDType	LegID;
	///���ȳ���
	TLegMultipleType	LegMultiple;
	///��ϳֲֺ�Լ����
	TInstrumentIDType	CombInstrumentID;

bool operator==(const InvestorPositionCombineDetailField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenDate, ct.OpenDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ComTradeID, ct.ComTradeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeID, ct.TradeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (TotalAmt == ct.TotalAmt);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Margin, ct.Margin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchMargin, ct.ExchMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByMoney, ct.MarginRateByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByVolume, ct.MarginRateByVolume));
	if (!ret) return ret;
	ret = ret && (LegID == ct.LegID);
	if (!ret) return ret;
	ret = ret && (LegMultiple == ct.LegMultiple);
	if (!ret) return ret;
	ret = ret && (strcmp(CombInstrumentID, ct.CombInstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///Ԥ��
struct ParkedOrderField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///��������
	TOrderRefType	OrderRef;
	///�û�����
	TUserIDType	UserID;
	///�����۸�����
	TOrderPriceTypeType	OrderPriceType;
	///��������
	TDirectionType	Direction;
	///��Ͽ�ƽ��־
	TCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	TPriceType	LimitPrice;
	///����
	TVolumeType	VolumeTotalOriginal;
	///��Ч������
	TTimeConditionType	TimeCondition;
	///GTD����
	TDateType	GTDDate;
	///�ɽ�������
	TVolumeConditionType	VolumeCondition;
	///��С�ɽ���
	TVolumeType	MinVolume;
	///��������
	TContingentConditionType	ContingentCondition;
	///ֹ���
	TPriceType	StopPrice;
	///ǿƽԭ��
	TForceCloseReasonType	ForceCloseReason;
	///�Զ������־
	TBoolType	IsAutoSuspend;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///������
	TRequestIDType	RequestID;
	///�û�ǿ����־
	TBoolType	UserForceClose;
	///����������
	TExchangeIDType	ExchangeID;
	///Ԥ�񱨵����
	TParkedOrderIDType	ParkedOrderID;
	///�û�����
	TUserTypeType	UserType;
	///Ԥ��״̬
	TParkedOrderStatusType	Status;
	///�������
	TErrorIDType	ErrorID;
	///������Ϣ
	TErrorMsgType	ErrorMsg;

bool operator==(const ParkedOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParkedOrderID, ct.ParkedOrderID) == 0);
	if (!ret) return ret;
	ret = ret && (UserType == ct.UserType);
	if (!ret) return ret;
	ret = ret && (Status == ct.Status);
	if (!ret) return ret;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///����Ԥ�񵥲���
struct ParkedOrderActionField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///������������
	TOrderActionRefType	OrderActionRef;
	///��������
	TOrderRefType	OrderRef;
	///������
	TRequestIDType	RequestID;
	///ǰ�ñ��
	TFrontIDType	FrontID;
	///�Ự���
	TSessionIDType	SessionID;
	///����������
	TExchangeIDType	ExchangeID;
	///�������
	TOrderSysIDType	OrderSysID;
	///������־
	TActionFlagType	ActionFlag;
	///�۸�
	TPriceType	LimitPrice;
	///�����仯
	TVolumeType	VolumeChange;
	///�û�����
	TUserIDType	UserID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ԥ�񳷵������
	TParkedOrderActionIDType	ParkedOrderActionID;
	///�û�����
	TUserTypeType	UserType;
	///Ԥ�񳷵�״̬
	TParkedOrderStatusType	Status;
	///�������
	TErrorIDType	ErrorID;
	///������Ϣ
	TErrorMsgType	ErrorMsg;

bool operator==(const ParkedOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderActionRef == ct.OrderActionRef);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (ActionFlag == ct.ActionFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeChange == ct.VolumeChange);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParkedOrderActionID, ct.ParkedOrderActionID) == 0);
	if (!ret) return ret;
	ret = ret && (UserType == ct.UserType);
	if (!ret) return ret;
	ret = ret && (Status == ct.Status);
	if (!ret) return ret;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯԤ��
struct QryParkedOrderField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///����������
	TExchangeIDType	ExchangeID;

bool operator==(const QryParkedOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯԤ�񳷵�
struct QryParkedOrderActionField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///����������
	TExchangeIDType	ExchangeID;

bool operator==(const QryParkedOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///ɾ��Ԥ��
struct RemoveParkedOrderField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///Ԥ�񱨵����
	TParkedOrderIDType	ParkedOrderID;

bool operator==(const RemoveParkedOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParkedOrderID, ct.ParkedOrderID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///ɾ��Ԥ�񳷵�
struct RemoveParkedOrderActionField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///Ԥ�񳷵����
	TParkedOrderActionIDType	ParkedOrderActionID;

bool operator==(const RemoveParkedOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParkedOrderActionID, ct.ParkedOrderActionID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///���͹�˾�����ʽ��㷨��
struct InvestorWithdrawAlgorithmField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���߷�Χ
	TInvestorRangeType	InvestorRange;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�����ʽ����
	TRatioType	UsingRatio;

bool operator==(const InvestorWithdrawAlgorithmField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UsingRatio, ct.UsingRatio));
	if (!ret) return ret;

	return ret;
}
};

///��ѯ��ϳֲ���ϸ
struct QryInvestorPositionCombineDetailField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��ϳֲֺ�Լ����
	TInstrumentIDType	CombInstrumentID;

bool operator==(const QryInvestorPositionCombineDetailField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CombInstrumentID, ct.CombInstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�ɽ�����
struct MarketDataAveragePriceField
{
	///���վ���
	TPriceType	AveragePrice;

bool operator==(const MarketDataAveragePriceField &ct) const
{
	bool ret = true;
	ret = ret && (EQUALDOUBLE(AveragePrice, ct.AveragePrice));
	if (!ret) return ret;

	return ret;
}
};

///У��Ͷ��������
struct VerifyInvestorPasswordField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///����
	TPasswordType	Password;

bool operator==(const VerifyInvestorPasswordField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�û�IP
struct UserIPField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///IP��ַ
	TIPAddressType	IPAddress;
	///IP��ַ����
	TIPAddressType	IPMask;
	///Mac��ַ
	TMacAddressType	MacAddress;

bool operator==(const UserIPField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(IPAddress, ct.IPAddress) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(IPMask, ct.IPMask) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(MacAddress, ct.MacAddress) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�û��¼�֪ͨ��Ϣ
struct TradingNoticeInfoField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///����ʱ��
	TTimeType	SendTime;
	///��Ϣ����
	TContentType	FieldContent;
	///����ϵ�к�
	TSequenceSeriesType	SequenceSeries;
	///���к�
	TSequenceNoType	SequenceNo;

bool operator==(const TradingNoticeInfoField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SendTime, ct.SendTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FieldContent, ct.FieldContent) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceSeries == ct.SequenceSeries);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;

	return ret;
}
};

///�û��¼�֪ͨ
struct TradingNoticeField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���߷�Χ
	TInvestorRangeType	InvestorRange;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///����ϵ�к�
	TSequenceSeriesType	SequenceSeries;
	///�û�����
	TUserIDType	UserID;
	///����ʱ��
	TTimeType	SendTime;
	///���к�
	TSequenceNoType	SequenceNo;
	///��Ϣ����
	TContentType	FieldContent;

bool operator==(const TradingNoticeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceSeries == ct.SequenceSeries);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SendTime, ct.SendTime) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(FieldContent, ct.FieldContent) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ�����¼�֪ͨ
struct QryTradingNoticeField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;

bool operator==(const QryTradingNoticeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���󱨵�
struct QryErrOrderField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;

bool operator==(const QryErrOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///���󱨵�
struct ErrOrderField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///��������
	TOrderRefType	OrderRef;
	///�û�����
	TUserIDType	UserID;
	///�����۸�����
	TOrderPriceTypeType	OrderPriceType;
	///��������
	TDirectionType	Direction;
	///��Ͽ�ƽ��־
	TCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	TPriceType	LimitPrice;
	///����
	TVolumeType	VolumeTotalOriginal;
	///��Ч������
	TTimeConditionType	TimeCondition;
	///GTD����
	TDateType	GTDDate;
	///�ɽ�������
	TVolumeConditionType	VolumeCondition;
	///��С�ɽ���
	TVolumeType	MinVolume;
	///��������
	TContingentConditionType	ContingentCondition;
	///ֹ���
	TPriceType	StopPrice;
	///ǿƽԭ��
	TForceCloseReasonType	ForceCloseReason;
	///�Զ������־
	TBoolType	IsAutoSuspend;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///������
	TRequestIDType	RequestID;
	///�û�ǿ����־
	TBoolType	UserForceClose;
	///�������
	TErrorIDType	ErrorID;
	///������Ϣ
	TErrorMsgType	ErrorMsg;

bool operator==(const ErrOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ���󱨵�����
struct QryErrOrderActionField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;

bool operator==(const QryErrOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///���󱨵�����
struct ErrOrderActionField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///������������
	TOrderActionRefType	OrderActionRef;
	///��������
	TOrderRefType	OrderRef;
	///������
	TRequestIDType	RequestID;
	///ǰ�ñ��
	TFrontIDType	FrontID;
	///�Ự���
	TSessionIDType	SessionID;
	///����������
	TExchangeIDType	ExchangeID;
	///�������
	TOrderSysIDType	OrderSysID;
	///������־
	TActionFlagType	ActionFlag;
	///�۸�
	TPriceType	LimitPrice;
	///�����仯
	TVolumeType	VolumeChange;
	///��������
	TDateType	ActionDate;
	///����ʱ��
	TTimeType	ActionTime;
	///����������Ա����
	TTraderIDType	TraderID;
	///��װ���
	TInstallIDType	InstallID;
	///���ر������
	TOrderLocalIDType	OrderLocalID;
	///�������ر��
	TOrderLocalIDType	ActionLocalID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///�ͻ�����
	TClientIDType	ClientID;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///��������״̬
	TOrderActionStatusType	OrderActionStatus;
	///�û�����
	TUserIDType	UserID;
	///״̬��Ϣ
	TErrorMsgType	StatusMsg;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///�������
	TErrorIDType	ErrorID;
	///������Ϣ
	TErrorMsgType	ErrorMsg;

bool operator==(const ErrOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderActionRef == ct.OrderActionRef);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (ActionFlag == ct.ActionFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeChange == ct.VolumeChange);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionDate, ct.ActionDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionTime, ct.ActionTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionLocalID, ct.ActionLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (OrderActionStatus == ct.OrderActionStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StatusMsg, ct.StatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ������ϢժҪ
struct InvestorSumInfoField
{
	///Ͷ��������
	TVolumeType	Count;
	///MD5У����1
	TMD5Type	md5_1;
	///MD5У����2
	TMD5Type	md5_2;
	///MD5У����3
	TMD5Type	md5_3;
	///MD5У����4
	TMD5Type	md5_4;
	///MD5У����5
	TMD5Type	md5_5;
	///MD5У����6
	TMD5Type	md5_6;
	///MD5У����7
	TMD5Type	md5_7;
	///MD5У����8
	TMD5Type	md5_8;
	///MD5У����9
	TMD5Type	md5_9;
	///MD5У����10
	TMD5Type	md5_10;
	///MD5У����11
	TMD5Type	md5_11;
	///MD5У����12
	TMD5Type	md5_12;
	///MD5У����13
	TMD5Type	md5_13;
	///MD5У����14
	TMD5Type	md5_14;
	///MD5У����15
	TMD5Type	md5_15;
	///MD5У����16
	TMD5Type	md5_16;

bool operator==(const InvestorSumInfoField &ct) const
{
	bool ret = true;
	ret = ret && (Count == ct.Count);
	if (!ret) return ret;
	ret = ret && (md5_1 == ct.md5_1);
	if (!ret) return ret;
	ret = ret && (md5_2 == ct.md5_2);
	if (!ret) return ret;
	ret = ret && (md5_3 == ct.md5_3);
	if (!ret) return ret;
	ret = ret && (md5_4 == ct.md5_4);
	if (!ret) return ret;
	ret = ret && (md5_5 == ct.md5_5);
	if (!ret) return ret;
	ret = ret && (md5_6 == ct.md5_6);
	if (!ret) return ret;
	ret = ret && (md5_7 == ct.md5_7);
	if (!ret) return ret;
	ret = ret && (md5_8 == ct.md5_8);
	if (!ret) return ret;
	ret = ret && (md5_9 == ct.md5_9);
	if (!ret) return ret;
	ret = ret && (md5_10 == ct.md5_10);
	if (!ret) return ret;
	ret = ret && (md5_11 == ct.md5_11);
	if (!ret) return ret;
	ret = ret && (md5_12 == ct.md5_12);
	if (!ret) return ret;
	ret = ret && (md5_13 == ct.md5_13);
	if (!ret) return ret;
	ret = ret && (md5_14 == ct.md5_14);
	if (!ret) return ret;
	ret = ret && (md5_15 == ct.md5_15);
	if (!ret) return ret;
	ret = ret && (md5_16 == ct.md5_16);
	if (!ret) return ret;

	return ret;
}
};

///����Ự
struct SettlementSessionField
{
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;

bool operator==(const SettlementSessionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;

	return ret;
}
};

///�û���¼����
struct ReqRiskUserLoginField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///����
	TPasswordType	Password;
	///�ͻ��˰汾,20091230��Version=1,֮ǰ�汾Version=0
	TVolumeType	Version;
	///����ǰ���пͻ������ӵ�SessionID
	TSessionIDType	LocalSessionID;

bool operator==(const ReqRiskUserLoginField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;
	ret = ret && (Version == ct.Version);
	if (!ret) return ret;
	ret = ret && (LocalSessionID == ct.LocalSessionID);
	if (!ret) return ret;

	return ret;
}
};

///�û���¼Ӧ��
struct RspRiskUserLoginField
{
	///������
	TDateType	TradingDay;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///�ͻ��˰汾,20091230��Version=1,֮ǰ�汾Version=0
	TVolumeType	Version;
	///ǰ������
	TFrontTypeType	FrontType;

bool operator==(const RspRiskUserLoginField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (Version == ct.Version);
	if (!ret) return ret;
	ret = ret && (FrontType == ct.FrontType);
	if (!ret) return ret;

	return ret;
}
};

///�ض���ΧͶ������ϢժҪ����
struct InvestorRangeSumInfoField
{
	///Ͷ������ʼ�±�
	TVolumeType	startindex;
	///Ͷ���߽����±�
	TVolumeType	endindex;
	///���͹�˾����
	TBrokerIDType	BrokerID;

bool operator==(const InvestorRangeSumInfoField &ct) const
{
	bool ret = true;
	ret = ret && (startindex == ct.startindex);
	if (!ret) return ret;
	ret = ret && (endindex == ct.endindex);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ�����ʽ��ѯ����
struct ReqInvestorAccountField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��ʼ��������
	TDateType	TradingDayStart;
	///������������
	TDateType	TradingDayEnd;
	///��ʼͶ���ߴ���
	TInvestorIDType	InvestorIDStart;
	///����Ͷ���ߴ���
	TInvestorIDType	InvestorIDEnd;
	///�ʽ����򷽷�
	TAccountSortTypeType	SortType;
	///�����������ؽ��
	TVolumeType	ResultCount;
	///���������ؽ��
	TRatioType	ResultRatio;

bool operator==(const ReqInvestorAccountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDayStart, ct.TradingDayStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDayEnd, ct.TradingDayEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDStart, ct.InvestorIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDEnd, ct.InvestorIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (SortType == ct.SortType);
	if (!ret) return ret;
	ret = ret && (ResultCount == ct.ResultCount);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ResultRatio, ct.ResultRatio));
	if (!ret) return ret;

	return ret;
}
};

///Ͷ�����ʽ��ѯ��Ӧ
struct InvestorRiskAccountField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///Ͷ�����ʺ�
	TAccountIDType	AccountID;
	///�ϴ���Ѻ���
	TMoneyType	PreMortgage;
	///�ϴ����ö��
	TMoneyType	PreCredit;
	///�ϴδ���
	TMoneyType	PreDeposit;
	///�ϴ���Ȩ��
	TMoneyType	PreBalance;
	///�ϴ�ռ�õı�֤��
	TMoneyType	PreMargin;
	///�������
	TMoneyType	Withdraw;
	///��ǰ��֤���ܶ�
	TMoneyType	CurrMargin;
	///������
	TMoneyType	Commission;
	///ƽ��ӯ��
	TMoneyType	CloseProfit;
	///�ֲ�ӯ��
	TMoneyType	PositionProfit;
	///��Ȩ��
	TMoneyType	Balance;
	///�����ʽ�
	TMoneyType	Available;
	///��Ѻ���
	TMoneyType	Mortgage;
	///��������֤��
	TMoneyType	ExchangeMargin;
	///����׼����
	TMoneyType	Reserve;
	///��ȡ�ʽ�
	TMoneyType	WithdrawQuota;
	///���ö��
	TMoneyType	Credit;
	///�ϴν�������֤��
	TMoneyType	PreExchMargin;
	///��ʷǿƽ����
	TVolumeType	ForceCloseStat;
	///Ͷ���߽��֤��
	TMoneyType	DeliveryMargin;
	///���������֤��
	TMoneyType	ExchangeDeliveryMargin;
	///��֤�𶳽�
	TMoneyType	FrozenMargin;
	///�����Ѷ���
	TMoneyType	FrozenCommission;

bool operator==(const InvestorRiskAccountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(AccountID, ct.AccountID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMortgage, ct.PreMortgage));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreCredit, ct.PreCredit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreDeposit, ct.PreDeposit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreBalance, ct.PreBalance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMargin, ct.PreMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Withdraw, ct.Withdraw));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrMargin, ct.CurrMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Commission, ct.Commission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfit, ct.PositionProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance, ct.Balance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Available, ct.Available));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Mortgage, ct.Mortgage));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeMargin, ct.ExchangeMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Reserve, ct.Reserve));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(WithdrawQuota, ct.WithdrawQuota));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Credit, ct.Credit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreExchMargin, ct.PreExchMargin));
	if (!ret) return ret;
	ret = ret && (ForceCloseStat == ct.ForceCloseStat);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(DeliveryMargin, ct.DeliveryMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeDeliveryMargin, ct.ExchangeDeliveryMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCommission, ct.FrozenCommission));
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���ֲֲ߳�ѯ����
struct ReqInvestorPositionField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��ʼ��������
	TDateType	TradingDayStart;
	///������������
	TDateType	TradingDayEnd;
	///��ʼͶ���ߴ���
	TInvestorIDType	InvestorIDStart;
	///����Ͷ���ߴ���
	TInvestorIDType	InvestorIDEnd;
	///��ʼ��Լ����
	TInstrumentIDType	InstIDStart;
	///������Լ����
	TInstrumentIDType	InstIDEnd;
	///��ʼ��Ʒ����
	TInstrumentIDType	ProductIDStart;
	///������Ʒ����
	TInstrumentIDType	ProductIDEnd;
	///�ֲ����򷽷�
	TPositionSortTypeType	SortType;
	///�����������ؽ��
	TVolumeType	ResultCount;
	///���������ؽ��
	TRatioType	ResultRatio;

bool operator==(const ReqInvestorPositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDayStart, ct.TradingDayStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDayEnd, ct.TradingDayEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDStart, ct.InvestorIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDEnd, ct.InvestorIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstIDStart, ct.InstIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstIDEnd, ct.InstIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductIDStart, ct.ProductIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductIDEnd, ct.ProductIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (SortType == ct.SortType);
	if (!ret) return ret;
	ret = ret && (ResultCount == ct.ResultCount);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ResultRatio, ct.ResultRatio));
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���ֲֲ߳�ѯ��Ӧ
struct RspInvestorPositionField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�ֲֶ�շ���
	TPosiDirectionType	PosiDirection;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///�ֲ�����
	TPositionDateType	PositionDate;
	///���ճֲ�
	TVolumeType	YdPosition;
	///���ճֲ�
	TVolumeType	Position;
	///��ͷ����
	TVolumeType	LongFrozen;
	///��ͷ����
	TVolumeType	ShortFrozen;
	///���ֶ�����
	TMoneyType	LongFrozenAmount;
	///���ֶ�����
	TMoneyType	ShortFrozenAmount;
	///������
	TVolumeType	OpenVolume;
	///ƽ����
	TVolumeType	CloseVolume;
	///���ֽ��
	TMoneyType	OpenAmount;
	///ƽ�ֽ��
	TMoneyType	CloseAmount;
	///�ֲֳɱ�
	TMoneyType	PositionCost;
	///�ϴ�ռ�õı�֤��
	TMoneyType	PreMargin;
	///ռ�õı�֤��
	TMoneyType	UseMargin;
	///����ı�֤��
	TMoneyType	FrozenMargin;
	///������ʽ�
	TMoneyType	FrozenCash;
	///�����������
	TMoneyType	FrozenCommission;
	///�ʽ���
	TMoneyType	CashIn;
	///������
	TMoneyType	Commission;
	///ƽ��ӯ��
	TMoneyType	CloseProfit;
	///�ֲ�ӯ��
	TMoneyType	PositionProfit;
	///�ϴν����
	TPriceType	PreSettlementPrice;
	///���ν����
	TPriceType	SettlementPrice;
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;
	///���ֳɱ�
	TMoneyType	OpenCost;
	///��������֤��
	TMoneyType	ExchangeMargin;
	///��ϳɽ��γɵĳֲ�
	TVolumeType	CombPosition;
	///��϶�ͷ����
	TVolumeType	CombLongFrozen;
	///��Ͽ�ͷ����
	TVolumeType	CombShortFrozen;
	///���ն���ƽ��ӯ��
	TMoneyType	CloseProfitByDate;
	///��ʶԳ�ƽ��ӯ��
	TMoneyType	CloseProfitByTrade;
	///���ճֲ�
	TVolumeType	TodayPosition;
	///��֤����
	TRatioType	MarginRateByMoney;
	///��֤����(������)
	TRatioType	MarginRateByVolume;

bool operator==(const RspInvestorPositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (PosiDirection == ct.PosiDirection);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (PositionDate == ct.PositionDate);
	if (!ret) return ret;
	ret = ret && (YdPosition == ct.YdPosition);
	if (!ret) return ret;
	ret = ret && (Position == ct.Position);
	if (!ret) return ret;
	ret = ret && (LongFrozen == ct.LongFrozen);
	if (!ret) return ret;
	ret = ret && (ShortFrozen == ct.ShortFrozen);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongFrozenAmount, ct.LongFrozenAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortFrozenAmount, ct.ShortFrozenAmount));
	if (!ret) return ret;
	ret = ret && (OpenVolume == ct.OpenVolume);
	if (!ret) return ret;
	ret = ret && (CloseVolume == ct.CloseVolume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenAmount, ct.OpenAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseAmount, ct.CloseAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionCost, ct.PositionCost));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMargin, ct.PreMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UseMargin, ct.UseMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCash, ct.FrozenCash));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCommission, ct.FrozenCommission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CashIn, ct.CashIn));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Commission, ct.Commission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfit, ct.PositionProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreSettlementPrice, ct.PreSettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SettlementPrice, ct.SettlementPrice));
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenCost, ct.OpenCost));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeMargin, ct.ExchangeMargin));
	if (!ret) return ret;
	ret = ret && (CombPosition == ct.CombPosition);
	if (!ret) return ret;
	ret = ret && (CombLongFrozen == ct.CombLongFrozen);
	if (!ret) return ret;
	ret = ret && (CombShortFrozen == ct.CombShortFrozen);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByDate, ct.CloseProfitByDate));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByTrade, ct.CloseProfitByTrade));
	if (!ret) return ret;
	ret = ret && (TodayPosition == ct.TodayPosition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByMoney, ct.MarginRateByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByVolume, ct.MarginRateByVolume));
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���߽��ײ�ѯ����
struct ReqInvestorTradeField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��ʼ��������
	TDateType	TradingDayStart;
	///������������
	TDateType	TradingDayEnd;
	///��ʼͶ���ߴ���
	TInvestorIDType	InvestorIDStart;
	///����Ͷ���ߴ���
	TInvestorIDType	InvestorIDEnd;
	///��ʼ��Լ����
	TInstrumentIDType	InstIDStart;
	///������Լ����
	TInstrumentIDType	InstIDEnd;
	///��ʼ��Ʒ����
	TInstrumentIDType	ProductIDStart;
	///������Ʒ����
	TInstrumentIDType	ProductIDEnd;
	///�������򷽷�
	TTradeSortTypeType	SortType;
	///�����������ؽ��
	TVolumeType	ResultCount;
	///���������ؽ��
	TRatioType	ResultRatio;

bool operator==(const ReqInvestorTradeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDayStart, ct.TradingDayStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDayEnd, ct.TradingDayEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDStart, ct.InvestorIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDEnd, ct.InvestorIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstIDStart, ct.InstIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstIDEnd, ct.InstIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductIDStart, ct.ProductIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductIDEnd, ct.ProductIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (SortType == ct.SortType);
	if (!ret) return ret;
	ret = ret && (ResultCount == ct.ResultCount);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ResultRatio, ct.ResultRatio));
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���߽��ײ�ѯ��Ӧ
struct RspInvestorTradeField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///��������
	TOrderRefType	OrderRef;
	///�û�����
	TUserIDType	UserID;
	///����������
	TExchangeIDType	ExchangeID;
	///�ɽ����
	TTradeIDType	TradeID;
	///��������
	TDirectionType	Direction;
	///�������
	TOrderSysIDType	OrderSysID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///�ͻ�����
	TClientIDType	ClientID;
	///���׽�ɫ
	TTradingRoleType	TradingRole;
	///��Լ�ڽ������Ĵ���
	TExchangeInstIDType	ExchangeInstID;
	///��ƽ��־
	TOffsetFlagType	OffsetFlag;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///�۸�
	TPriceType	Price;
	///����
	TVolumeType	Volume;
	///�ɽ�ʱ��
	TDateType	TradeDate;
	///�ɽ�ʱ��
	TTimeType	TradeTime;
	///�ɽ�����
	TTradeTypeType	TradeType;
	///�ɽ�����Դ
	TPriceSourceType	PriceSource;
	///����������Ա����
	TTraderIDType	TraderID;
	///���ر������
	TOrderLocalIDType	OrderLocalID;
	///�����Ա���
	TParticipantIDType	ClearingPartID;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///���
	TSequenceNoType	SequenceNo;
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;
	///���͹�˾�������
	TSequenceNoType	BrokerOrderSeq;
	///�ɽ���Դ
	TTradeSourceType	TradeSource;

bool operator==(const RspInvestorTradeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeID, ct.TradeID) == 0);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (TradingRole == ct.TradingRole);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (OffsetFlag == ct.OffsetFlag);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price, ct.Price));
	if (!ret) return ret;
	ret = ret && (Volume == ct.Volume);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeDate, ct.TradeDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeTime, ct.TradeTime) == 0);
	if (!ret) return ret;
	ret = ret && (TradeType == ct.TradeType);
	if (!ret) return ret;
	ret = ret && (PriceSource == ct.PriceSource);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClearingPartID, ct.ClearingPartID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (BrokerOrderSeq == ct.BrokerOrderSeq);
	if (!ret) return ret;
	ret = ret && (TradeSource == ct.TradeSource);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���߱�����ѯ����
struct ReqInvestorOrderField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��������
	TDateType	TradingDay;
	///��ʼͶ���ߴ���
	TInvestorIDType	InvestorIDStart;
	///����Ͷ���ߴ���
	TInvestorIDType	InvestorIDEnd;
	///��ʼ��Լ����
	TInstrumentIDType	InstIDStart;
	///������Լ����
	TInstrumentIDType	InstIDEnd;
	///��ʼ��Ʒ����
	TInstrumentIDType	ProductIDStart;
	///������Ʒ����
	TInstrumentIDType	ProductIDEnd;

bool operator==(const ReqInvestorOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDStart, ct.InvestorIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDEnd, ct.InvestorIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstIDStart, ct.InstIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstIDEnd, ct.InstIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductIDStart, ct.ProductIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductIDEnd, ct.ProductIDEnd) == 0);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���߱�����ѯ��Ӧ
struct RspInvestorOrderField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///��������
	TOrderRefType	OrderRef;
	///�û�����
	TUserIDType	UserID;
	///�����۸�����
	TOrderPriceTypeType	OrderPriceType;
	///��������
	TDirectionType	Direction;
	///��Ͽ�ƽ��־
	TCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	TPriceType	LimitPrice;
	///����
	TVolumeType	VolumeTotalOriginal;
	///��Ч������
	TTimeConditionType	TimeCondition;
	///GTD����
	TDateType	GTDDate;
	///�ɽ�������
	TVolumeConditionType	VolumeCondition;
	///��С�ɽ���
	TVolumeType	MinVolume;
	///��������
	TContingentConditionType	ContingentCondition;
	///ֹ���
	TPriceType	StopPrice;
	///ǿƽԭ��
	TForceCloseReasonType	ForceCloseReason;
	///�Զ������־
	TBoolType	IsAutoSuspend;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///������
	TRequestIDType	RequestID;
	///���ر������
	TOrderLocalIDType	OrderLocalID;
	///����������
	TExchangeIDType	ExchangeID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///�ͻ�����
	TClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TTraderIDType	TraderID;
	///��װ���
	TInstallIDType	InstallID;
	///�����ύ״̬
	TOrderSubmitStatusType	OrderSubmitStatus;
	///������ʾ���
	TSequenceNoType	NotifySequence;
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;
	///�������
	TOrderSysIDType	OrderSysID;
	///������Դ
	TOrderSourceType	OrderSource;
	///����״̬
	TOrderStatusType	OrderStatus;
	///��������
	TOrderTypeType	OrderType;
	///��ɽ�����
	TVolumeType	VolumeTraded;
	///ʣ������
	TVolumeType	VolumeTotal;
	///��������
	TDateType	InsertDate;
	///ί��ʱ��
	TTimeType	InsertTime;
	///����ʱ��
	TTimeType	ActiveTime;
	///����ʱ��
	TTimeType	SuspendTime;
	///����޸�ʱ��
	TTimeType	UpdateTime;
	///����ʱ��
	TTimeType	CancelTime;
	///����޸Ľ���������Ա����
	TTraderIDType	ActiveTraderID;
	///�����Ա���
	TParticipantIDType	ClearingPartID;
	///���
	TSequenceNoType	SequenceNo;
	///ǰ�ñ��
	TFrontIDType	FrontID;
	///�Ự���
	TSessionIDType	SessionID;
	///�û��˲�Ʒ��Ϣ
	TProductInfoType	UserProductInfo;
	///״̬��Ϣ
	TErrorMsgType	StatusMsg;
	///�û�ǿ����־
	TBoolType	UserForceClose;
	///�����û�����
	TUserIDType	ActiveUserID;
	///���͹�˾�������
	TSequenceNoType	BrokerOrderSeq;
	///��ر���
	TOrderSysIDType	RelativeOrderSysID;
	///֣�����ɽ�����
	TVolumeType	ZCETotalTradedVolume;

bool operator==(const RspInvestorOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (OrderSubmitStatus == ct.OrderSubmitStatus);
	if (!ret) return ret;
	ret = ret && (NotifySequence == ct.NotifySequence);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderSource == ct.OrderSource);
	if (!ret) return ret;
	ret = ret && (OrderStatus == ct.OrderStatus);
	if (!ret) return ret;
	ret = ret && (OrderType == ct.OrderType);
	if (!ret) return ret;
	ret = ret && (VolumeTraded == ct.VolumeTraded);
	if (!ret) return ret;
	ret = ret && (VolumeTotal == ct.VolumeTotal);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertDate, ct.InsertDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertTime, ct.InsertTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveTime, ct.ActiveTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SuspendTime, ct.SuspendTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UpdateTime, ct.UpdateTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CancelTime, ct.CancelTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveTraderID, ct.ActiveTraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClearingPartID, ct.ClearingPartID) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(UserProductInfo, ct.UserProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StatusMsg, ct.StatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveUserID, ct.ActiveUserID) == 0);
	if (!ret) return ret;
	ret = ret && (BrokerOrderSeq == ct.BrokerOrderSeq);
	if (!ret) return ret;
	ret = ret && (strcmp(RelativeOrderSysID, ct.RelativeOrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (ZCETotalTradedVolume == ct.ZCETotalTradedVolume);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���ֲ߳�ͳ�Ʋ�ѯ��Ӧ
struct InvestorPositionStaticField
{
	///��Ʒ����
	TInstrumentIDType	ProductID;
	///��Լ���
	TInstrumentIDType	InstrumentID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��ֲ�
	TVolumeType	YdPosition;
	///�ֲܳ�
	TVolumeType	Position;
	///��ͷ�ֲ�
	TVolumeType	LongPosition;
	///��ͷ�ֲ�
	TVolumeType	ShortPosition;
	///���ֲ�
	TVolumeType	NetPosition;
	///Ͷ����ͷ�ֲ�
	TVolumeType	SpecuLongPosi;
	///Ͷ����ͷ�ֲ�
	TVolumeType	SpecuShortPosi;
	///��ֵ��ͷ�ֲ�
	TVolumeType	HedgeLongPosi;
	///��ֵ��ͷ�ֲ�
	TVolumeType	HedgeShortPosi;
	///���
	TVolumeType	TodayPosition;
	///�ֲֳܳɱ�
	TMoneyType	PositionCost;

bool operator==(const InvestorPositionStaticField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (YdPosition == ct.YdPosition);
	if (!ret) return ret;
	ret = ret && (Position == ct.Position);
	if (!ret) return ret;
	ret = ret && (LongPosition == ct.LongPosition);
	if (!ret) return ret;
	ret = ret && (ShortPosition == ct.ShortPosition);
	if (!ret) return ret;
	ret = ret && (NetPosition == ct.NetPosition);
	if (!ret) return ret;
	ret = ret && (SpecuLongPosi == ct.SpecuLongPosi);
	if (!ret) return ret;
	ret = ret && (SpecuShortPosi == ct.SpecuShortPosi);
	if (!ret) return ret;
	ret = ret && (HedgeLongPosi == ct.HedgeLongPosi);
	if (!ret) return ret;
	ret = ret && (HedgeShortPosi == ct.HedgeShortPosi);
	if (!ret) return ret;
	ret = ret && (TodayPosition == ct.TodayPosition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionCost, ct.PositionCost));
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���߳ɽ�ͳ�Ʋ�ѯ��Ӧ
struct InvestorTradeStaticField
{
	///��Ʒ����
	TInstrumentIDType	ProductID;
	///��Լ���
	TInstrumentIDType	InstrumentID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�ܳɽ���
	TVolumeType	TotalVol;
	///�ܳɽ���
	TMoneyType	TotalAmt;
	///��ɽ���
	TVolumeType	BuyVol;
	///��ɽ���
	TMoneyType	BuyAmt;
	///���ɽ���
	TVolumeType	SellVol;
	///���ɽ���
	TMoneyType	SellAmt;
	///������ɽ���
	TVolumeType	NetVol;
	///������ɽ���
	TMoneyType	NetAmt;

bool operator==(const InvestorTradeStaticField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (TotalVol == ct.TotalVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(TotalAmt, ct.TotalAmt));
	if (!ret) return ret;
	ret = ret && (BuyVol == ct.BuyVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BuyAmt, ct.BuyAmt));
	if (!ret) return ret;
	ret = ret && (SellVol == ct.SellVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SellAmt, ct.SellAmt));
	if (!ret) return ret;
	ret = ret && (NetVol == ct.NetVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NetAmt, ct.NetAmt));
	if (!ret) return ret;

	return ret;
}
};

///����ʵʱ����
struct SubMarketDataField
{
	///��Լ���
	TInstrumentIDType	InstrumentID;

bool operator==(const SubMarketDataField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��Լ�ֱֲ�����ѯ
struct QryInstPositionRateField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��ʼ��Լ����
	TInstrumentIDType	InstIDStart;
	///������Լ����
	TInstrumentIDType	InstIDEnd;
	///ɢ����ֵ����ֲ�������
	TLargeVolumeType	hbtotal_little;
	///�л���ֵ����ֲ�������
	TLargeVolumeType	hbtotal_medium;
	///ɢ����ֵ�����ֲ�������
	TLargeVolumeType	hstotal_little;
	///�л���ֵ�����ֲ�������
	TLargeVolumeType	hstotal_medium;
	///ɢ����ֵ�ֲ�������
	TLargeVolumeType	htotal_little;
	///�л���ֵ�ֲ�������
	TLargeVolumeType	htotal_medium;
	///ɢ��Ͷ������ֲ�������
	TLargeVolumeType	sbtotal_little;
	///�л�Ͷ������ֲ�������
	TLargeVolumeType	sbtotal_medium;
	///ɢ��Ͷ�������ֲ�������
	TLargeVolumeType	sstotal_little;
	///�л�Ͷ�������ֲ�������
	TLargeVolumeType	sstotal_medium;
	///ɢ��Ͷ���ֲ�������
	TLargeVolumeType	stotal_little;
	///�л�Ͷ���ֲ�������
	TLargeVolumeType	stotal_medium;
	///ɢ������ֲ�������
	TLargeVolumeType	buytotal_little;
	///�л�����ֲ�������
	TLargeVolumeType	buytotal_medium;
	///ɢ�������ֲ�������
	TLargeVolumeType	selltotal_little;
	///�л������ֲ�������
	TLargeVolumeType	selltotal_medium;
	///ɢ���ֲܳ�������
	TLargeVolumeType	total_little;
	///�л��ֲܳ�������
	TLargeVolumeType	total_medium;
	///ȡֵ��ʽ
	TValueModeType	ValueMode;

bool operator==(const QryInstPositionRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstIDStart, ct.InstIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstIDEnd, ct.InstIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(hbtotal_little, ct.hbtotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(hbtotal_medium, ct.hbtotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(hstotal_little, ct.hstotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(hstotal_medium, ct.hstotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(htotal_little, ct.htotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(htotal_medium, ct.htotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(sbtotal_little, ct.sbtotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(sbtotal_medium, ct.sbtotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(sstotal_little, ct.sstotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(sstotal_medium, ct.sstotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(stotal_little, ct.stotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(stotal_medium, ct.stotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(buytotal_little, ct.buytotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(buytotal_medium, ct.buytotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(selltotal_little, ct.selltotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(selltotal_medium, ct.selltotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(total_little, ct.total_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(total_medium, ct.total_medium));
	if (!ret) return ret;
	ret = ret && (ValueMode == ct.ValueMode);
	if (!ret) return ret;

	return ret;
}
};

///��Լ�ֱֲ���Ӧ��
struct RspInstPositionRateField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///ɢ����ֵ����ֲ���
	TVolumeType	hbtotal_amt_little;
	///�л���ֵ����ֲ���
	TVolumeType	hbtotal_amt_medium;
	///�󻧱�ֵ����ֲ���
	TVolumeType	hbtotal_amt_large;
	///ɢ����ֵ�����ֲ���
	TVolumeType	hstotal_amt_little;
	///�л���ֵ�����ֲ���
	TVolumeType	hstotal_amt_medium;
	///�󻧱�ֵ�����ֲ���
	TVolumeType	hstotal_amt_large;
	///ɢ����ֵ�ֲ���
	TVolumeType	htotal_amt_little;
	///�л���ֵ�ֲ���
	TVolumeType	htotal_amt_medium;
	///�󻧱�ֵ�ֲ���
	TVolumeType	htotal_amt_large;
	///ɢ��Ͷ������ֲ���
	TVolumeType	sbtotal_amt_little;
	///�л�Ͷ������ֲ���
	TVolumeType	sbtotal_amt_medium;
	///��Ͷ������ֲ���
	TVolumeType	sbtotal_amt_large;
	///ɢ��Ͷ�������ֲ���
	TVolumeType	sstotal_amt_little;
	///�л�Ͷ�������ֲ���
	TVolumeType	sstotal_amt_medium;
	///��Ͷ�������ֲ���
	TVolumeType	sstotal_amt_large;
	///ɢ��Ͷ���ֲ���
	TVolumeType	stotal_amt_little;
	///�л�Ͷ���ֲ���
	TVolumeType	stotal_amt_medium;
	///��Ͷ���ֲ���
	TVolumeType	stotal_amt_large;
	///ɢ������ֲ���
	TVolumeType	buytotal_amt_little;
	///�л�����ֲ���
	TVolumeType	buytotal_amt_medium;
	///������ֲ���
	TVolumeType	buytotal_amt_large;
	///ɢ�������ֲ���
	TVolumeType	selltotal_amt_little;
	///�л������ֲ���
	TVolumeType	selltotal_amt_medium;
	///�������ֲ���
	TVolumeType	selltotal_amt_large;
	///ɢ���ֲܳ���
	TVolumeType	total_amt_little;
	///�л��ֲܳ���
	TVolumeType	total_amt_medium;
	///���ֲܳ���
	TVolumeType	total_amt_large;

bool operator==(const RspInstPositionRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (hbtotal_amt_little == ct.hbtotal_amt_little);
	if (!ret) return ret;
	ret = ret && (hbtotal_amt_medium == ct.hbtotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (hbtotal_amt_large == ct.hbtotal_amt_large);
	if (!ret) return ret;
	ret = ret && (hstotal_amt_little == ct.hstotal_amt_little);
	if (!ret) return ret;
	ret = ret && (hstotal_amt_medium == ct.hstotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (hstotal_amt_large == ct.hstotal_amt_large);
	if (!ret) return ret;
	ret = ret && (htotal_amt_little == ct.htotal_amt_little);
	if (!ret) return ret;
	ret = ret && (htotal_amt_medium == ct.htotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (htotal_amt_large == ct.htotal_amt_large);
	if (!ret) return ret;
	ret = ret && (sbtotal_amt_little == ct.sbtotal_amt_little);
	if (!ret) return ret;
	ret = ret && (sbtotal_amt_medium == ct.sbtotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (sbtotal_amt_large == ct.sbtotal_amt_large);
	if (!ret) return ret;
	ret = ret && (sstotal_amt_little == ct.sstotal_amt_little);
	if (!ret) return ret;
	ret = ret && (sstotal_amt_medium == ct.sstotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (sstotal_amt_large == ct.sstotal_amt_large);
	if (!ret) return ret;
	ret = ret && (stotal_amt_little == ct.stotal_amt_little);
	if (!ret) return ret;
	ret = ret && (stotal_amt_medium == ct.stotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (stotal_amt_large == ct.stotal_amt_large);
	if (!ret) return ret;
	ret = ret && (buytotal_amt_little == ct.buytotal_amt_little);
	if (!ret) return ret;
	ret = ret && (buytotal_amt_medium == ct.buytotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (buytotal_amt_large == ct.buytotal_amt_large);
	if (!ret) return ret;
	ret = ret && (selltotal_amt_little == ct.selltotal_amt_little);
	if (!ret) return ret;
	ret = ret && (selltotal_amt_medium == ct.selltotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (selltotal_amt_large == ct.selltotal_amt_large);
	if (!ret) return ret;
	ret = ret && (total_amt_little == ct.total_amt_little);
	if (!ret) return ret;
	ret = ret && (total_amt_medium == ct.total_amt_medium);
	if (!ret) return ret;
	ret = ret && (total_amt_large == ct.total_amt_large);
	if (!ret) return ret;

	return ret;
}
};

///��Ʒ�ֱֲ�����ѯ
struct QryProductPositionRateField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��Ʒ����
	TInstrumentIDType	ProductID;
	///ɢ����ֵ����ֲ�������
	TLargeVolumeType	hbtotal_little;
	///�л���ֵ����ֲ�������
	TLargeVolumeType	hbtotal_medium;
	///ɢ����ֵ�����ֲ�������
	TLargeVolumeType	hstotal_little;
	///�л���ֵ�����ֲ�������
	TLargeVolumeType	hstotal_medium;
	///ɢ����ֵ�ֲ�������
	TLargeVolumeType	htotal_little;
	///�л���ֵ�ֲ�������
	TLargeVolumeType	htotal_medium;
	///ɢ��Ͷ������ֲ�������
	TLargeVolumeType	sbtotal_little;
	///�л�Ͷ������ֲ�������
	TLargeVolumeType	sbtotal_medium;
	///ɢ��Ͷ�������ֲ�������
	TLargeVolumeType	sstotal_little;
	///�л�Ͷ�������ֲ�������
	TLargeVolumeType	sstotal_medium;
	///ɢ��Ͷ���ֲ�������
	TLargeVolumeType	stotal_little;
	///�л�Ͷ���ֲ�������
	TLargeVolumeType	stotal_medium;
	///ɢ������ֲ�������
	TLargeVolumeType	buytotal_little;
	///�л�����ֲ�������
	TLargeVolumeType	buytotal_medium;
	///ɢ�������ֲ�������
	TLargeVolumeType	selltotal_little;
	///�л������ֲ�������
	TLargeVolumeType	selltotal_medium;
	///ɢ���ֲܳ�������
	TLargeVolumeType	total_little;
	///�л��ֲܳ�������
	TLargeVolumeType	total_medium;
	///ȡֵ��ʽ
	TValueModeType	ValueMode;

bool operator==(const QryProductPositionRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(hbtotal_little, ct.hbtotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(hbtotal_medium, ct.hbtotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(hstotal_little, ct.hstotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(hstotal_medium, ct.hstotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(htotal_little, ct.htotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(htotal_medium, ct.htotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(sbtotal_little, ct.sbtotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(sbtotal_medium, ct.sbtotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(sstotal_little, ct.sstotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(sstotal_medium, ct.sstotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(stotal_little, ct.stotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(stotal_medium, ct.stotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(buytotal_little, ct.buytotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(buytotal_medium, ct.buytotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(selltotal_little, ct.selltotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(selltotal_medium, ct.selltotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(total_little, ct.total_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(total_medium, ct.total_medium));
	if (!ret) return ret;
	ret = ret && (ValueMode == ct.ValueMode);
	if (!ret) return ret;

	return ret;
}
};

///��Ʒ�ֱֲ���
struct RspProductPositionRateField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��Ʒ����
	TInstrumentIDType	ProductID;
	///ɢ����ֵ����ֲ���
	TVolumeType	hbtotal_amt_little;
	///�л���ֵ����ֲ���
	TVolumeType	hbtotal_amt_medium;
	///�󻧱�ֵ����ֲ���
	TVolumeType	hbtotal_amt_large;
	///ɢ����ֵ�����ֲ���
	TVolumeType	hstotal_amt_little;
	///�л���ֵ�����ֲ���
	TVolumeType	hstotal_amt_medium;
	///�󻧱�ֵ�����ֲ���
	TVolumeType	hstotal_amt_large;
	///ɢ����ֵ�ֲ���
	TVolumeType	htotal_amt_little;
	///�л���ֵ�ֲ���
	TVolumeType	htotal_amt_medium;
	///�󻧱�ֵ�ֲ���
	TVolumeType	htotal_amt_large;
	///ɢ��Ͷ������ֲ���
	TVolumeType	sbtotal_amt_little;
	///�л�Ͷ������ֲ���
	TVolumeType	sbtotal_amt_medium;
	///��Ͷ������ֲ���
	TVolumeType	sbtotal_amt_large;
	///ɢ��Ͷ�������ֲ���
	TVolumeType	sstotal_amt_little;
	///�л�Ͷ�������ֲ���
	TVolumeType	sstotal_amt_medium;
	///��Ͷ�������ֲ���
	TVolumeType	sstotal_amt_large;
	///ɢ��Ͷ���ֲ���
	TVolumeType	stotal_amt_little;
	///�л�Ͷ���ֲ���
	TVolumeType	stotal_amt_medium;
	///��Ͷ���ֲ���
	TVolumeType	stotal_amt_large;
	///ɢ������ֲ���
	TVolumeType	buytotal_amt_little;
	///�л�����ֲ���
	TVolumeType	buytotal_amt_medium;
	///������ֲ���
	TVolumeType	buytotal_amt_large;
	///ɢ�������ֲ���
	TVolumeType	selltotal_amt_little;
	///�л������ֲ���
	TVolumeType	selltotal_amt_medium;
	///�������ֲ���
	TVolumeType	selltotal_amt_large;
	///ɢ���ֲܳ���
	TVolumeType	total_amt_little;
	///�л��ֲܳ���
	TVolumeType	total_amt_medium;
	///���ֲܳ���
	TVolumeType	total_amt_large;

bool operator==(const RspProductPositionRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;
	ret = ret && (hbtotal_amt_little == ct.hbtotal_amt_little);
	if (!ret) return ret;
	ret = ret && (hbtotal_amt_medium == ct.hbtotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (hbtotal_amt_large == ct.hbtotal_amt_large);
	if (!ret) return ret;
	ret = ret && (hstotal_amt_little == ct.hstotal_amt_little);
	if (!ret) return ret;
	ret = ret && (hstotal_amt_medium == ct.hstotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (hstotal_amt_large == ct.hstotal_amt_large);
	if (!ret) return ret;
	ret = ret && (htotal_amt_little == ct.htotal_amt_little);
	if (!ret) return ret;
	ret = ret && (htotal_amt_medium == ct.htotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (htotal_amt_large == ct.htotal_amt_large);
	if (!ret) return ret;
	ret = ret && (sbtotal_amt_little == ct.sbtotal_amt_little);
	if (!ret) return ret;
	ret = ret && (sbtotal_amt_medium == ct.sbtotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (sbtotal_amt_large == ct.sbtotal_amt_large);
	if (!ret) return ret;
	ret = ret && (sstotal_amt_little == ct.sstotal_amt_little);
	if (!ret) return ret;
	ret = ret && (sstotal_amt_medium == ct.sstotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (sstotal_amt_large == ct.sstotal_amt_large);
	if (!ret) return ret;
	ret = ret && (stotal_amt_little == ct.stotal_amt_little);
	if (!ret) return ret;
	ret = ret && (stotal_amt_medium == ct.stotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (stotal_amt_large == ct.stotal_amt_large);
	if (!ret) return ret;
	ret = ret && (buytotal_amt_little == ct.buytotal_amt_little);
	if (!ret) return ret;
	ret = ret && (buytotal_amt_medium == ct.buytotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (buytotal_amt_large == ct.buytotal_amt_large);
	if (!ret) return ret;
	ret = ret && (selltotal_amt_little == ct.selltotal_amt_little);
	if (!ret) return ret;
	ret = ret && (selltotal_amt_medium == ct.selltotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (selltotal_amt_large == ct.selltotal_amt_large);
	if (!ret) return ret;
	ret = ret && (total_amt_little == ct.total_amt_little);
	if (!ret) return ret;
	ret = ret && (total_amt_medium == ct.total_amt_medium);
	if (!ret) return ret;
	ret = ret && (total_amt_large == ct.total_amt_large);
	if (!ret) return ret;

	return ret;
}
};

///��ʷģ�ⷨѹ������ֻ����һ�ν��
struct IsWriteOnceField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�Ƿ�ֻ����һ�ν��
	TBoolType	IsWriteOnce;

bool operator==(const IsWriteOnceField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (IsWriteOnce == ct.IsWriteOnce);
	if (!ret) return ret;

	return ret;
}
};

///ѹ�����Խ���۸�(�򸡶�)
struct STSettlePriceField
{
	///�ڼ���
	TDayNumberType	Day;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///����۸�����(Day=1ʱ��Ч)
	TSTPriceTypeType	PriceType;
	///����۸�(�򸡶�)
	TPriceType	Price;

bool operator==(const STSettlePriceField &ct) const
{
	bool ret = true;
	ret = ret && (Day == ct.Day);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (PriceType == ct.PriceType);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price, ct.Price));
	if (!ret) return ret;

	return ret;
}
};

///ѹ������Ͷ���߱�֤����
struct STInstrumentMarginRateField
{
	///�ڼ���
	TDayNumberType	Day;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	TInvestorRangeType	InvestorRange;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///��ͷ��֤����
	TRatioType	LongMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	LongMarginRatioByVolume;
	///��ͷ��֤����
	TRatioType	ShortMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	ShortMarginRatioByVolume;
	///�Ƿ���Խ�������ȡ
	TBoolType	IsRelative;

bool operator==(const STInstrumentMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (Day == ct.Day);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (IsRelative == ct.IsRelative);
	if (!ret) return ret;

	return ret;
}
};

///ѹ������Ͷ���߱�֤���ʵ���
struct STInstrumentMarginRateAdjustField
{
	///�ڼ���
	TDayNumberType	Day;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	TInvestorRangeType	InvestorRange;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///��ͷ��֤����
	TRatioType	LongMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	LongMarginRatioByVolume;
	///��ͷ��֤����
	TRatioType	ShortMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	ShortMarginRatioByVolume;
	///�Ƿ���Խ�������ȡ
	TBoolType	IsRelative;

bool operator==(const STInstrumentMarginRateAdjustField &ct) const
{
	bool ret = true;
	ret = ret && (Day == ct.Day);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (IsRelative == ct.IsRelative);
	if (!ret) return ret;

	return ret;
}
};

///ѹ�����Խ�������֤����
struct STExchangeMarginRateField
{
	///�ڼ���
	TDayNumberType	Day;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///��ͷ��֤����
	TRatioType	LongMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	LongMarginRatioByVolume;
	///��ͷ��֤����
	TRatioType	ShortMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	ShortMarginRatioByVolume;

bool operator==(const STExchangeMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (Day == ct.Day);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;

	return ret;
}
};

///ѹ�����Խ�������֤���ʵ���
struct STExchangeMarginRateAdjustField
{
	///�ڼ���
	TDayNumberType	Day;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///���潻����Ͷ���߶�ͷ��֤����
	TRatioType	LongMarginRatioByMoney;
	///���潻����Ͷ���߶�ͷ��֤���
	TMoneyType	LongMarginRatioByVolume;
	///���潻����Ͷ���߿�ͷ��֤����
	TRatioType	ShortMarginRatioByMoney;
	///���潻����Ͷ���߿�ͷ��֤���
	TMoneyType	ShortMarginRatioByVolume;
	///��������ͷ��֤����
	TRatioType	ExchLongMarginRatioByMoney;
	///��������ͷ��֤���
	TMoneyType	ExchLongMarginRatioByVolume;
	///��������ͷ��֤����
	TRatioType	ExchShortMarginRatioByMoney;
	///��������ͷ��֤���
	TMoneyType	ExchShortMarginRatioByVolume;
	///�����潻����Ͷ���߶�ͷ��֤����
	TRatioType	NoLongMarginRatioByMoney;
	///�����潻����Ͷ���߶�ͷ��֤���
	TMoneyType	NoLongMarginRatioByVolume;
	///�����潻����Ͷ���߿�ͷ��֤����
	TRatioType	NoShortMarginRatioByMoney;
	///�����潻����Ͷ���߿�ͷ��֤���
	TMoneyType	NoShortMarginRatioByVolume;

bool operator==(const STExchangeMarginRateAdjustField &ct) const
{
	bool ret = true;
	ret = ret && (Day == ct.Day);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchLongMarginRatioByMoney, ct.ExchLongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchLongMarginRatioByVolume, ct.ExchLongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchShortMarginRatioByMoney, ct.ExchShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchShortMarginRatioByVolume, ct.ExchShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NoLongMarginRatioByMoney, ct.NoLongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NoLongMarginRatioByVolume, ct.NoLongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NoShortMarginRatioByMoney, ct.NoShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NoShortMarginRatioByVolume, ct.NoShortMarginRatioByVolume));
	if (!ret) return ret;

	return ret;
}
};

///Ͷ��������ϵ����Ϣ
struct InvestorLinkManField
{
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���߷������
	TInvestorIDType	InvestorGroupID;
	///Ͷ��������
	TPartyNameType	InvestorName;
	///֤������
	TIdCardTypeType	IdentifiedCardType;
	///֤������
	TIdentifiedCardNoType	IdentifiedCardNo;
	///�Ƿ��Ծ
	TBoolType	IsActive;
	///��ϵ�绰
	TTelephoneType	Telephone;
	///ͨѶ��ַ
	TAddressType	Address;
	///ָ���µ���֤������
	TIdCardTypeType	OrderManIdentifiedCardType;
	///ָ���µ���֤������
	TIdentifiedCardNoType	OrderManIdentifiedCardNo;
	///ָ���µ�������
	TPartyNameType	OrderManPersonName;
	///ָ���µ�����ϵ�绰
	TTelephoneType	OrderManTelephone;
	///ָ���µ���ͨѶ��ַ
	TAddressType	OrderManAddress;
	///ָ���µ�����������
	TZipCodeType	OrderManZipCode;
	///�ʽ������֤������
	TIdCardTypeType	FundManIdentifiedCardType;
	///�ʽ������֤������
	TIdentifiedCardNoType	FundManIdentifiedCardNo;
	///�ʽ����������
	TPartyNameType	FundManPersonName;
	///�ʽ��������ϵ�绰
	TTelephoneType	FundManTelephone;
	///�ʽ������ͨѶ��ַ
	TAddressType	FundManAddress;
	///�ʽ��������������
	TZipCodeType	FundManZipCode;
	///������Ȩ��֤������
	TIdCardTypeType	OpenManIdentifiedCardType;
	///������Ȩ��֤������
	TIdentifiedCardNoType	OpenManIdentifiedCardNo;
	///������Ȩ������
	TPartyNameType	OpenManPersonName;
	///������Ȩ����ϵ�绰
	TTelephoneType	OpenManTelephone;
	///������Ȩ��ͨѶ��ַ
	TAddressType	OpenManAddress;
	///������Ȩ����������
	TZipCodeType	OpenManZipCode;
	///���㵥ȷ����֤������
	TIdCardTypeType	SettlementManIdentifiedCardType;
	///���㵥ȷ����֤������
	TIdentifiedCardNoType	SettlementManIdentifiedCardNo;
	///���㵥ȷ��������
	TPartyNameType	SettlementManPersonName;
	///���㵥ȷ������ϵ�绰
	TTelephoneType	SettlementManTelephone;
	///���㵥ȷ����ͨѶ��ַ
	TAddressType	SettlementManAddress;
	///���㵥ȷ������������
	TZipCodeType	SettlementManZipCode;
	///��������
	TDateType	OpenDate;
	///�ֻ�
	TMobileType	Mobile;
	///�����ʼ�
	TEMailType	EMail;
	///Ͷ��������
	TInvestorTypeType	InvestorType;
	///���Ҵ���
	TUOACountryCodeType	PhoneCountryCode;
	///����
	TAreaCodeType	PhoneAreaCode;
	///������Ȩ�˹��Ҵ���
	TUOACountryCodeType	OpenPhoneCountryCode;
	///������Ȩ������
	TAreaCodeType	OpenPhoneAreaCode;
	///ָ���µ��˹��Ҵ���
	TUOACountryCodeType	OrderPhoneCountryCode;
	///ָ���µ�������
	TAreaCodeType	OrderPhoneAreaCode;
	///�ʽ�����˹��Ҵ���
	TUOACountryCodeType	FundPhoneCountryCode;
	///�ʽ����������
	TAreaCodeType	FundPhoneAreaCode;
	///���㵥ȷ���˹��Ҵ���
	TUOACountryCodeType	SettlePhoneCountryCode;
	///���㵥ȷ��������
	TAreaCodeType	SettlePhoneAreaCode;

bool operator==(const InvestorLinkManField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupID, ct.InvestorGroupID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorName, ct.InvestorName) == 0);
	if (!ret) return ret;
	ret = ret && (IdentifiedCardType == ct.IdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(IdentifiedCardNo, ct.IdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (strcmp(Telephone, ct.Telephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Address, ct.Address) == 0);
	if (!ret) return ret;
	ret = ret && (OrderManIdentifiedCardType == ct.OrderManIdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderManIdentifiedCardNo, ct.OrderManIdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderManPersonName, ct.OrderManPersonName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderManTelephone, ct.OrderManTelephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderManAddress, ct.OrderManAddress) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderManZipCode, ct.OrderManZipCode) == 0);
	if (!ret) return ret;
	ret = ret && (FundManIdentifiedCardType == ct.FundManIdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(FundManIdentifiedCardNo, ct.FundManIdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FundManPersonName, ct.FundManPersonName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FundManTelephone, ct.FundManTelephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FundManAddress, ct.FundManAddress) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FundManZipCode, ct.FundManZipCode) == 0);
	if (!ret) return ret;
	ret = ret && (OpenManIdentifiedCardType == ct.OpenManIdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenManIdentifiedCardNo, ct.OpenManIdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenManPersonName, ct.OpenManPersonName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenManTelephone, ct.OpenManTelephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenManAddress, ct.OpenManAddress) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenManZipCode, ct.OpenManZipCode) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementManIdentifiedCardType == ct.SettlementManIdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlementManIdentifiedCardNo, ct.SettlementManIdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlementManPersonName, ct.SettlementManPersonName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlementManTelephone, ct.SettlementManTelephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlementManAddress, ct.SettlementManAddress) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlementManZipCode, ct.SettlementManZipCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenDate, ct.OpenDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Mobile, ct.Mobile) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EMail, ct.EMail) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorType == ct.InvestorType);
	if (!ret) return ret;
	ret = ret && (strcmp(PhoneCountryCode, ct.PhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(PhoneAreaCode, ct.PhoneAreaCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenPhoneCountryCode, ct.OpenPhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenPhoneAreaCode, ct.OpenPhoneAreaCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderPhoneCountryCode, ct.OrderPhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderPhoneAreaCode, ct.OrderPhoneAreaCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FundPhoneCountryCode, ct.FundPhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FundPhoneAreaCode, ct.FundPhoneAreaCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlePhoneCountryCode, ct.SettlePhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlePhoneAreaCode, ct.SettlePhoneAreaCode) == 0);
	if (!ret) return ret;

	return ret;
}
};

///����Ͷ���߳ɽ�
struct SubInvestorTradeField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;

bool operator==(const SubInvestorTradeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///����Ͷ���ֲ߳�
struct SubInvestorPositionField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;

bool operator==(const SubInvestorPositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///���к�
struct NotifySequenceField
{
	///���
	TSequenceNoType	SequenceNo;

bool operator==(const NotifySequenceField &ct) const
{
	bool ret = true;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;

	return ret;
}
};

///����ĳɽ�
struct SequencialTradeField
{
	///����Ψһ�����к�
	TSequenceNoType	UniqSequenceNo;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///��������
	TOrderRefType	OrderRef;
	///�û�����
	TUserIDType	UserID;
	///����������
	TExchangeIDType	ExchangeID;
	///�ɽ����
	TTradeIDType	TradeID;
	///��������
	TDirectionType	Direction;
	///�������
	TOrderSysIDType	OrderSysID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///�ͻ�����
	TClientIDType	ClientID;
	///���׽�ɫ
	TTradingRoleType	TradingRole;
	///��Լ�ڽ������Ĵ���
	TExchangeInstIDType	ExchangeInstID;
	///��ƽ��־
	TOffsetFlagType	OffsetFlag;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///�۸�
	TPriceType	Price;
	///����
	TVolumeType	Volume;
	///�ɽ�ʱ��
	TDateType	TradeDate;
	///�ɽ�ʱ��
	TTimeType	TradeTime;
	///�ɽ�����
	TTradeTypeType	TradeType;
	///�ɽ�����Դ
	TPriceSourceType	PriceSource;
	///����������Ա����
	TTraderIDType	TraderID;
	///���ر������
	TOrderLocalIDType	OrderLocalID;
	///�����Ա���
	TParticipantIDType	ClearingPartID;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///���
	TSequenceNoType	SequenceNo;
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;
	///���͹�˾�������
	TSequenceNoType	BrokerOrderSeq;
	///�ɽ���Դ
	TTradeSourceType	TradeSource;

bool operator==(const SequencialTradeField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeID, ct.TradeID) == 0);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (TradingRole == ct.TradingRole);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (OffsetFlag == ct.OffsetFlag);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price, ct.Price));
	if (!ret) return ret;
	ret = ret && (Volume == ct.Volume);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeDate, ct.TradeDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeTime, ct.TradeTime) == 0);
	if (!ret) return ret;
	ret = ret && (TradeType == ct.TradeType);
	if (!ret) return ret;
	ret = ret && (PriceSource == ct.PriceSource);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClearingPartID, ct.ClearingPartID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (BrokerOrderSeq == ct.BrokerOrderSeq);
	if (!ret) return ret;
	ret = ret && (TradeSource == ct.TradeSource);
	if (!ret) return ret;

	return ret;
}
};

///����ı���
struct SequencialOrderField
{
	///����Ψһ�����к�
	TSequenceNoType	UniqSequenceNo;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///��������
	TOrderRefType	OrderRef;
	///�û�����
	TUserIDType	UserID;
	///�����۸�����
	TOrderPriceTypeType	OrderPriceType;
	///��������
	TDirectionType	Direction;
	///��Ͽ�ƽ��־
	TCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	TPriceType	LimitPrice;
	///����
	TVolumeType	VolumeTotalOriginal;
	///��Ч������
	TTimeConditionType	TimeCondition;
	///GTD����
	TDateType	GTDDate;
	///�ɽ�������
	TVolumeConditionType	VolumeCondition;
	///��С�ɽ���
	TVolumeType	MinVolume;
	///��������
	TContingentConditionType	ContingentCondition;
	///ֹ���
	TPriceType	StopPrice;
	///ǿƽԭ��
	TForceCloseReasonType	ForceCloseReason;
	///�Զ������־
	TBoolType	IsAutoSuspend;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///������
	TRequestIDType	RequestID;
	///���ر������
	TOrderLocalIDType	OrderLocalID;
	///����������
	TExchangeIDType	ExchangeID;
	///��Ա����
	TParticipantIDType	ParticipantID;
	///�ͻ�����
	TClientIDType	ClientID;
	///��Լ�ڽ������Ĵ���
	TExchangeInstIDType	ExchangeInstID;
	///����������Ա����
	TTraderIDType	TraderID;
	///��װ���
	TInstallIDType	InstallID;
	///�����ύ״̬
	TOrderSubmitStatusType	OrderSubmitStatus;
	///������ʾ���
	TSequenceNoType	NotifySequence;
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;
	///�������
	TOrderSysIDType	OrderSysID;
	///������Դ
	TOrderSourceType	OrderSource;
	///����״̬
	TOrderStatusType	OrderStatus;
	///��������
	TOrderTypeType	OrderType;
	///��ɽ�����
	TVolumeType	VolumeTraded;
	///ʣ������
	TVolumeType	VolumeTotal;
	///��������
	TDateType	InsertDate;
	///ί��ʱ��
	TTimeType	InsertTime;
	///����ʱ��
	TTimeType	ActiveTime;
	///����ʱ��
	TTimeType	SuspendTime;
	///����޸�ʱ��
	TTimeType	UpdateTime;
	///����ʱ��
	TTimeType	CancelTime;
	///����޸Ľ���������Ա����
	TTraderIDType	ActiveTraderID;
	///�����Ա���
	TParticipantIDType	ClearingPartID;
	///���
	TSequenceNoType	SequenceNo;
	///ǰ�ñ��
	TFrontIDType	FrontID;
	///�Ự���
	TSessionIDType	SessionID;
	///�û��˲�Ʒ��Ϣ
	TProductInfoType	UserProductInfo;
	///״̬��Ϣ
	TErrorMsgType	StatusMsg;
	///�û�ǿ����־
	TBoolType	UserForceClose;
	///�����û�����
	TUserIDType	ActiveUserID;
	///���͹�˾�������
	TSequenceNoType	BrokerOrderSeq;
	///��ر���
	TOrderSysIDType	RelativeOrderSysID;
	///֣�����ɽ�����
	TVolumeType	ZCETotalTradedVolume;

bool operator==(const SequencialOrderField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (OrderSubmitStatus == ct.OrderSubmitStatus);
	if (!ret) return ret;
	ret = ret && (NotifySequence == ct.NotifySequence);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderSource == ct.OrderSource);
	if (!ret) return ret;
	ret = ret && (OrderStatus == ct.OrderStatus);
	if (!ret) return ret;
	ret = ret && (OrderType == ct.OrderType);
	if (!ret) return ret;
	ret = ret && (VolumeTraded == ct.VolumeTraded);
	if (!ret) return ret;
	ret = ret && (VolumeTotal == ct.VolumeTotal);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertDate, ct.InsertDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertTime, ct.InsertTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveTime, ct.ActiveTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SuspendTime, ct.SuspendTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UpdateTime, ct.UpdateTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CancelTime, ct.CancelTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveTraderID, ct.ActiveTraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClearingPartID, ct.ClearingPartID) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(UserProductInfo, ct.UserProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StatusMsg, ct.StatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveUserID, ct.ActiveUserID) == 0);
	if (!ret) return ret;
	ret = ret && (BrokerOrderSeq == ct.BrokerOrderSeq);
	if (!ret) return ret;
	ret = ret && (strcmp(RelativeOrderSysID, ct.RelativeOrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (ZCETotalTradedVolume == ct.ZCETotalTradedVolume);
	if (!ret) return ret;

	return ret;
}
};

///����ľ��͹�˾�û��¼�
struct SequencialBrokerUserEventField
{
	///����Ψһ�����к�
	TSequenceNoType	UniqSequenceNo;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///�û��¼�����
	TUserEventTypeType	UserEventType;
	///�û��¼����
	TSequenceNoType	EventSequenceNo;
	///�¼���������
	TDateType	EventDate;
	///�¼�����ʱ��
	TTimeType	EventTime;
	///�û��¼���Ϣ
	TUserEventInfoType	UserEventInfo;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;

bool operator==(const SequencialBrokerUserEventField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (UserEventType == ct.UserEventType);
	if (!ret) return ret;
	ret = ret && (EventSequenceNo == ct.EventSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserEventInfo, ct.UserEventInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///����ĳֲ�
struct SequencialPositionField
{
	///����Ψһ�����к�
	TSequenceNoType	UniqSequenceNo;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�ֲֶ�շ���
	TPosiDirectionType	PosiDirection;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///�ֲ�����
	TPositionDateType	PositionDate;
	///���ճֲ�
	TVolumeType	YdPosition;
	///���ճֲ�
	TVolumeType	Position;
	///��ͷ����
	TVolumeType	LongFrozen;
	///��ͷ����
	TVolumeType	ShortFrozen;
	///���ֶ�����
	TMoneyType	LongFrozenAmount;
	///���ֶ�����
	TMoneyType	ShortFrozenAmount;
	///������
	TVolumeType	OpenVolume;
	///ƽ����
	TVolumeType	CloseVolume;
	///���ֽ��
	TMoneyType	OpenAmount;
	///ƽ�ֽ��
	TMoneyType	CloseAmount;
	///�ֲֳɱ�
	TMoneyType	PositionCost;
	///�ϴ�ռ�õı�֤��
	TMoneyType	PreMargin;
	///ռ�õı�֤��
	TMoneyType	UseMargin;
	///����ı�֤��
	TMoneyType	FrozenMargin;
	///������ʽ�
	TMoneyType	FrozenCash;
	///�����������
	TMoneyType	FrozenCommission;
	///�ʽ���
	TMoneyType	CashIn;
	///������
	TMoneyType	Commission;
	///ƽ��ӯ��
	TMoneyType	CloseProfit;
	///�ֲ�ӯ��
	TMoneyType	PositionProfit;
	///�ϴν����
	TPriceType	PreSettlementPrice;
	///���ν����
	TPriceType	SettlementPrice;
	///������
	TDateType	TradingDay;
	///������
	TSettlementIDType	SettlementID;
	///���ֳɱ�
	TMoneyType	OpenCost;
	///��������֤��
	TMoneyType	ExchangeMargin;
	///��ϳɽ��γɵĳֲ�
	TVolumeType	CombPosition;
	///��϶�ͷ����
	TVolumeType	CombLongFrozen;
	///��Ͽ�ͷ����
	TVolumeType	CombShortFrozen;
	///���ն���ƽ��ӯ��
	TMoneyType	CloseProfitByDate;
	///��ʶԳ�ƽ��ӯ��
	TMoneyType	CloseProfitByTrade;
	///���ճֲ�
	TVolumeType	TodayPosition;
	///��֤����
	TRatioType	MarginRateByMoney;
	///��֤����(������)
	TRatioType	MarginRateByVolume;

bool operator==(const SequencialPositionField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (PosiDirection == ct.PosiDirection);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (PositionDate == ct.PositionDate);
	if (!ret) return ret;
	ret = ret && (YdPosition == ct.YdPosition);
	if (!ret) return ret;
	ret = ret && (Position == ct.Position);
	if (!ret) return ret;
	ret = ret && (LongFrozen == ct.LongFrozen);
	if (!ret) return ret;
	ret = ret && (ShortFrozen == ct.ShortFrozen);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongFrozenAmount, ct.LongFrozenAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortFrozenAmount, ct.ShortFrozenAmount));
	if (!ret) return ret;
	ret = ret && (OpenVolume == ct.OpenVolume);
	if (!ret) return ret;
	ret = ret && (CloseVolume == ct.CloseVolume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenAmount, ct.OpenAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseAmount, ct.CloseAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionCost, ct.PositionCost));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMargin, ct.PreMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UseMargin, ct.UseMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCash, ct.FrozenCash));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCommission, ct.FrozenCommission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CashIn, ct.CashIn));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Commission, ct.Commission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfit, ct.PositionProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreSettlementPrice, ct.PreSettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SettlementPrice, ct.SettlementPrice));
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenCost, ct.OpenCost));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeMargin, ct.ExchangeMargin));
	if (!ret) return ret;
	ret = ret && (CombPosition == ct.CombPosition);
	if (!ret) return ret;
	ret = ret && (CombLongFrozen == ct.CombLongFrozen);
	if (!ret) return ret;
	ret = ret && (CombShortFrozen == ct.CombShortFrozen);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByDate, ct.CloseProfitByDate));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByTrade, ct.CloseProfitByTrade));
	if (!ret) return ret;
	ret = ret && (TodayPosition == ct.TodayPosition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByMoney, ct.MarginRateByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByVolume, ct.MarginRateByVolume));
	if (!ret) return ret;

	return ret;
}
};

///��ѯ�ͻ�����֪ͨģ��
struct QryRiskNotifyPatternField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///�����ֶ�
	TCommentType	Reserve;

bool operator==(const QryRiskNotifyPatternField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Reserve, ct.Reserve) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�ͻ�����֪ͨģ��
struct RiskNotifyPatternField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û����룬�����޸�ģ��ʱ��Ч
	TUserIDType	UserID;
	///֪ͨ����
	TNotifyClassType	NotifyClass;
	///�Ƿ�����
	TBoolType	IsActive;
	///�Ƿ��Զ�ϵͳ֪ͨ
	TBoolType	IsAutoSystem;
	///�Ƿ��Զ�����֪ͨ
	TBoolType	IsAutoSMS;
	///�Ƿ��Զ��ʼ�֪ͨ
	TBoolType	IsAutoEmail;
	///ϵͳ֪ͨģ������
	TRiskNofityInfoType	SystemPattern;
	///����֪ͨģ������
	TRiskNofityInfoType	SMSPattern;
	///�ʼ�֪ͨģ������
	TRiskNofityInfoType	EmailPattern;
	///Ԥ������(��ʱֻ����warnlevel)
	TAddressType	Reserve;

bool operator==(const RiskNotifyPatternField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (IsAutoSystem == ct.IsAutoSystem);
	if (!ret) return ret;
	ret = ret && (IsAutoSMS == ct.IsAutoSMS);
	if (!ret) return ret;
	ret = ret && (IsAutoEmail == ct.IsAutoEmail);
	if (!ret) return ret;
	ret = ret && (strcmp(SystemPattern, ct.SystemPattern) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SMSPattern, ct.SMSPattern) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EmailPattern, ct.EmailPattern) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Reserve, ct.Reserve) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ�ͻ�����֪ͨģ�����Զ��滻�ֶ�
struct QryRiskNotifyTokenField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;

bool operator==(const QryRiskNotifyTokenField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�ͻ�����֪ͨģ�����Զ��滻�ֶ�
struct RiskNotifyTokenField
{
	///�Զ��滻�ֶ�
	TExchangeNameType	Token;
	///�Զ��滻�ֶε�����
	TBrokerNameType	Description;

bool operator==(const RiskNotifyTokenField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(Token, ct.Token) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Description, ct.Description) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�����Ϳͻ�����֪ͨ������
struct RiskNotifyCommandField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///֪ͨ����
	TNotifyClassType	NotifyClass;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�Ƿ���ϵͳ֪ͨ
	TBoolType	IsAutoSystem;
	///�Ƿ��Ͷ���֪ͨ
	TBoolType	IsAutoSMS;
	///�Ƿ����ʼ�֪ͨ
	TBoolType	IsAutoEmail;
	///�����ֶ�
	TCommentType	Reserve;
	///֪ͨģ������
	TRiskNofityInfoType	Pattern;
	///�Ƿ�����������֪ͨ
	TBoolType	IsNormal;
	///�Ƿ������;�ʾ֪ͨ
	TBoolType	IsWarn;

bool operator==(const RiskNotifyCommandField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (IsAutoSystem == ct.IsAutoSystem);
	if (!ret) return ret;
	ret = ret && (IsAutoSMS == ct.IsAutoSMS);
	if (!ret) return ret;
	ret = ret && (IsAutoEmail == ct.IsAutoEmail);
	if (!ret) return ret;
	ret = ret && (strcmp(Reserve, ct.Reserve) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Pattern, ct.Pattern) == 0);
	if (!ret) return ret;
	ret = ret && (IsNormal == ct.IsNormal);
	if (!ret) return ret;
	ret = ret && (IsWarn == ct.IsWarn);
	if (!ret) return ret;

	return ret;
}
};

///����Ŀͻ�����֪ͨ�¼�
struct SequencialRiskNotifyField
{
	///����Ψһ�����к�
	TSequenceNoType	UniqSequenceNo;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�����͸÷���֪ͨ���û�����
	TUserIDType	UserID;
	///���û�����ķ���֪ͨ�¼��ڵ�������
	TSequenceNoType	SequenceNo;
	///�¼���������
	TDateType	EventDate;
	///�¼�����ʱ��
	TTimeType	EventTime;
	///֪ͨ����
	TNotifyClassType	NotifyClass;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�Ƿ���ϵͳ֪ͨ
	TBoolType	IsAutoSystem;
	///�Ƿ��Ͷ���֪ͨ
	TBoolType	IsAutoSMS;
	///�Ƿ����ʼ�֪ͨ
	TBoolType	IsAutoEmail;
	///ϵͳ֪ͨ����
	TRiskNofityInfoType	SystemMsg;
	///����֪ͨ����
	TRiskNofityInfoType	SMSMsg;
	///�ʼ�֪ͨ����
	TRiskNofityInfoType	EmailMsg;

bool operator==(const SequencialRiskNotifyField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (IsAutoSystem == ct.IsAutoSystem);
	if (!ret) return ret;
	ret = ret && (IsAutoSMS == ct.IsAutoSMS);
	if (!ret) return ret;
	ret = ret && (IsAutoEmail == ct.IsAutoEmail);
	if (!ret) return ret;
	ret = ret && (strcmp(SystemMsg, ct.SystemMsg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SMSMsg, ct.SMSMsg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EmailMsg, ct.EmailMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�ͻ�����֪ͨ�¼�
struct RiskNotifyField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�����͸÷���֪ͨ���û�����
	TUserIDType	UserID;
	///���û�����ķ���֪ͨ�¼��ڵ�������
	TSequenceNoType	SequenceNo;
	///�¼���������
	TDateType	EventDate;
	///�¼�����ʱ��
	TTimeType	EventTime;
	///֪ͨ����
	TNotifyClassType	NotifyClass;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�Ƿ���ϵͳ֪ͨ
	TBoolType	IsAutoSystem;
	///�Ƿ��Ͷ���֪ͨ
	TBoolType	IsAutoSMS;
	///�Ƿ����ʼ�֪ͨ
	TBoolType	IsAutoEmail;
	///ϵͳ֪ͨ����
	TRiskNofityInfoType	SystemMsg;
	///����֪ͨ����
	TRiskNofityInfoType	SMSMsg;
	///�ʼ�֪ͨ����
	TRiskNofityInfoType	EmailMsg;

bool operator==(const RiskNotifyField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (IsAutoSystem == ct.IsAutoSystem);
	if (!ret) return ret;
	ret = ret && (IsAutoSMS == ct.IsAutoSMS);
	if (!ret) return ret;
	ret = ret && (IsAutoEmail == ct.IsAutoEmail);
	if (!ret) return ret;
	ret = ret && (strcmp(SystemMsg, ct.SystemMsg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SMSMsg, ct.SMSMsg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EmailMsg, ct.EmailMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///ǿƽ��׼
struct ForceCloseStandardField
{
	///ǿƽ��׼
	TForceCloseLevelType	ForceCloseLevel;
	///ǿƽ�ʽ��ͷű�׼
	TForceCloseReleaseType	ForceCloseRelease;
	///��ͣ�巽��ֲ�����
	TBoolType	FCNonLimitFirst;

bool operator==(const ForceCloseStandardField &ct) const
{
	bool ret = true;
	ret = ret && (ForceCloseLevel == ct.ForceCloseLevel);
	if (!ret) return ret;
	ret = ret && (ForceCloseRelease == ct.ForceCloseRelease);
	if (!ret) return ret;
	ret = ret && (FCNonLimitFirst == ct.FCNonLimitFirst);
	if (!ret) return ret;

	return ret;
}
};

///ǿƽ�ֲֹ���
struct ForceClosePositionRuleField
{
	///��Ʒ���Լ����
	TInstrumentIDType	ProductInstrumentID;
	///ǿƽ�ֲַ���
	TForceClosePosiDirectionType	FCPosiDirection;
	///ǿƽͶ���ױ���־
	TForceCloseHedgeFlagType	FCHedgeFlag;
	///ǿƽ��ϳֱֲ�־
	TForceCloseCombPosiFlagType	FCCombPosiFlag;
	///ǿƽ��ʷ�ֲ�˳��
	TForceCloseHistoryPosiOrderType	FCHistoryPosiOrder;
	///ǿƽ�۸�����
	TForceClosePriceTypeType	FCPrice;
	///�޼۵�������
	TVolumeType	PriceTick;
	///����ǿƽ����������ȼ�
	TFCRulePriorityType	FCRulePriority;

bool operator==(const ForceClosePositionRuleField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ProductInstrumentID, ct.ProductInstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (FCPosiDirection == ct.FCPosiDirection);
	if (!ret) return ret;
	ret = ret && (FCHedgeFlag == ct.FCHedgeFlag);
	if (!ret) return ret;
	ret = ret && (FCCombPosiFlag == ct.FCCombPosiFlag);
	if (!ret) return ret;
	ret = ret && (FCHistoryPosiOrder == ct.FCHistoryPosiOrder);
	if (!ret) return ret;
	ret = ret && (FCPrice == ct.FCPrice);
	if (!ret) return ret;
	ret = ret && (PriceTick == ct.PriceTick);
	if (!ret) return ret;
	ret = ret && (strcmp(FCRulePriority, ct.FCRulePriority) == 0);
	if (!ret) return ret;

	return ret;
}
};

///ǿƽ����
struct ForceCloseListField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;

bool operator==(const ForceCloseListField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ����ǿƽ�ֲ�
struct ForceClosePositionField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�ֲֶ�շ���
	TPosiDirectionType	PosiDirection;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///�ֲ�����
	TPositionDateType	PositionDate;
	///��ǰ�ֲ�����
	TVolumeType	Position;
	///��ǿƽ�ֲ�����
	TVolumeType	FCPosition;
	///ǿƽ�۸�����
	TForceClosePriceTypeType	FCPriceType;
	///�޼۵�������
	TVolumeType	PriceTick;
	///ǿƽ�۸�
	TPriceType	FCPrice;
	///ƽ���ͷŵı�֤��
	TMoneyType	ReleaseMargin;
	///ƽ��ӯ��
	TMoneyType	CloseProfit;
	///ƽ���ͷŵĽ�������֤��
	TMoneyType	ExchReleaseMargin;

bool operator==(const ForceClosePositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (PosiDirection == ct.PosiDirection);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (PositionDate == ct.PositionDate);
	if (!ret) return ret;
	ret = ret && (Position == ct.Position);
	if (!ret) return ret;
	ret = ret && (FCPosition == ct.FCPosition);
	if (!ret) return ret;
	ret = ret && (FCPriceType == ct.FCPriceType);
	if (!ret) return ret;
	ret = ret && (PriceTick == ct.PriceTick);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FCPrice, ct.FCPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ReleaseMargin, ct.ReleaseMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchReleaseMargin, ct.ExchReleaseMargin));
	if (!ret) return ret;

	return ret;
}
};

///ǿƽӦ��
struct RspForceClosePositionField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�ֲֶ�շ���
	TPosiDirectionType	PosiDirection;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///�ֲ�����
	TPositionDateType	PositionDate;
	///��ǰ�ֲ�����
	TVolumeType	Position;
	///��ǿƽ�ֲ�����
	TVolumeType	FCPosition;
	///ǿƽ�۸�����
	TForceClosePriceTypeType	FCPriceType;
	///�޼۵�������
	TVolumeType	PriceTick;
	///ǿƽ�۸�
	TPriceType	FCPrice;
	///ƽ���ͷŵı�֤��
	TMoneyType	ReleaseMargin;
	///ƽ��ӯ��
	TMoneyType	CloseProfit;
	///ǿƽ�������
	TForceCloseSceneIdType	FCID;
	///����ǿƽ��������ʱ��
	TTimeType	Time;
	///��ǰʱ�䣨���룩
	TMillisecType	CurrMillisec;
	///ƽ���ͷŵĽ�������֤��
	TMoneyType	ExchReleaseMargin;

bool operator==(const RspForceClosePositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (PosiDirection == ct.PosiDirection);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (PositionDate == ct.PositionDate);
	if (!ret) return ret;
	ret = ret && (Position == ct.Position);
	if (!ret) return ret;
	ret = ret && (FCPosition == ct.FCPosition);
	if (!ret) return ret;
	ret = ret && (FCPriceType == ct.FCPriceType);
	if (!ret) return ret;
	ret = ret && (PriceTick == ct.PriceTick);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FCPrice, ct.FCPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ReleaseMargin, ct.ReleaseMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (strcmp(FCID, ct.FCID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Time, ct.Time) == 0);
	if (!ret) return ret;
	ret = ret && (CurrMillisec == ct.CurrMillisec);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchReleaseMargin, ct.ExchReleaseMargin));
	if (!ret) return ret;

	return ret;
}
};

///���ǿƽ��������
struct RiskForceCloseOrderField
{
	///���ǿƽ����
	TForceCloseTypeType	FCType;
	///����ǿƽ��������ʱ��
	TTimeType	Time1;
	///����ǿƽ��������ʱ�䣨���룩
	TMillisecType	Millisec1;
	///ǿƽ�����ύʱ��
	TTimeType	Time2;
	///ǿƽ�����ύʱ�䣨���룩
	TMillisecType	Millisec2;
	///ǿƽ�������
	TForceCloseSceneIdType	FCSceneId;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///��������
	TOrderRefType	OrderRef;
	///�û�����
	TUserIDType	UserID;
	///�����۸�����
	TOrderPriceTypeType	OrderPriceType;
	///��������
	TDirectionType	Direction;
	///��Ͽ�ƽ��־
	TCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	TPriceType	LimitPrice;
	///����
	TVolumeType	VolumeTotalOriginal;
	///��Ч������
	TTimeConditionType	TimeCondition;
	///GTD����
	TDateType	GTDDate;
	///�ɽ�������
	TVolumeConditionType	VolumeCondition;
	///��С�ɽ���
	TVolumeType	MinVolume;
	///��������
	TContingentConditionType	ContingentCondition;
	///ֹ���
	TPriceType	StopPrice;
	///ǿƽԭ��
	TForceCloseReasonType	ForceCloseReason;
	///�Զ������־
	TBoolType	IsAutoSuspend;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///������
	TRequestIDType	RequestID;
	///�û�ǿ����־
	TBoolType	UserForceClose;
	///ǰ�ñ��
	TFrontIDType	FrontID;
	///�Ự���
	TSessionIDType	SessionID;

bool operator==(const RiskForceCloseOrderField &ct) const
{
	bool ret = true;
	ret = ret && (FCType == ct.FCType);
	if (!ret) return ret;
	ret = ret && (strcmp(Time1, ct.Time1) == 0);
	if (!ret) return ret;
	ret = ret && (Millisec1 == ct.Millisec1);
	if (!ret) return ret;
	ret = ret && (strcmp(Time2, ct.Time2) == 0);
	if (!ret) return ret;
	ret = ret && (Millisec2 == ct.Millisec2);
	if (!ret) return ret;
	ret = ret && (strcmp(FCSceneId, ct.FCSceneId) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;

	return ret;
}
};

///ǰ��״̬����
struct FrontStatusReportField
{
	///ǰ��time(NULL)
	TSessionIDType	Seconds;

bool operator==(const FrontStatusReportField &ct) const
{
	bool ret = true;
	ret = ret && (Seconds == ct.Seconds);
	if (!ret) return ret;

	return ret;
}
};

///���ֱֲ�֤��ָ�����
struct IndexNPPField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Ʒ����
	TInstrumentIDsType	ProductIDs;
	///����ֵ(�ڲ������Ǿ�ʾֵ,��ָ��FullIndexNPP����ָ��ֵ)
	TRatioType	WarnLevel;

bool operator==(const IndexNPPField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductIDs, ct.ProductIDs) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(WarnLevel, ct.WarnLevel));
	if (!ret) return ret;

	return ret;
}
};

///���ֱֲ�֤��ָ��ֵ
struct FullIndexNPPField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Ʒ����
	TInstrumentIDsType	ProductIDs;
	///����ֵ(�ڲ������Ǿ�ʾֵ,��ָ��FullIndexNPP����ָ��ֵ)
	TRatioType	WarnLevel;
	///��ͷ��֤��
	TMoneyType	LongMargin;
	///��ͷ��֤��
	TMoneyType	ShortMargin;
	///��Ȩ��
	TMoneyType	Balance;
	///������ֵ
	TRatioType	RealWarnLevel;

bool operator==(const FullIndexNPPField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductIDs, ct.ProductIDs) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(WarnLevel, ct.WarnLevel));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMargin, ct.LongMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMargin, ct.ShortMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance, ct.Balance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(RealWarnLevel, ct.RealWarnLevel));
	if (!ret) return ret;

	return ret;
}
};

///һ���ѯ����
struct NormalRiskQueryField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;

bool operator==(const NormalRiskQueryField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ��ȫ�۸񲨶���Χ����
struct QrySafePriceRangeField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ�۸񲨶�����
	TPriceVaryAlgoType	PriceVaryAlgo;
	///�۸񲨶��Ļ�׼������(v5.1.2֮�����)
	TPriceTypeType	BasePriceType;
	///��������
	TNotifyClassType	RiskLevel;
	///����Լ˳�򲨶�ʱ���������ͣ�����
	TVolumeType	MaxLimitDay;

bool operator==(const QrySafePriceRangeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (PriceVaryAlgo == ct.PriceVaryAlgo);
	if (!ret) return ret;
	ret = ret && (BasePriceType == ct.BasePriceType);
	if (!ret) return ret;
	ret = ret && (RiskLevel == ct.RiskLevel);
	if (!ret) return ret;
	ret = ret && (MaxLimitDay == ct.MaxLimitDay);
	if (!ret) return ret;

	return ret;
}
};

///�۸񲨶�����
struct PriceVaryParamField
{
	///��Լ���
	TInstrumentIDType	InstrumentID;
	///�۸񲨶�����
	TPriceVaryDirType	Direction;
	///�۸񲨶�����(>=0��Ч)
	TRatioType	Pecent;
	///�۸񲨶��Ļ�׼������
	TPriceTypeType	BasePriceType;
	///�Զ����׼��
	TPriceType	BasePrice;

bool operator==(const PriceVaryParamField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Pecent, ct.Pecent));
	if (!ret) return ret;
	ret = ret && (BasePriceType == ct.BasePriceType);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BasePrice, ct.BasePrice));
	if (!ret) return ret;

	return ret;
}
};

///��ѯ��ȫ�۸񲨶���ΧӦ��
struct RspSafePriceRangeField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///��׼�۸�
	TPriceType	BasePrice;
	///�����۸�
	TPriceType	LimitPrice;
	///�ܲ�������
	TRatioType	VaryPecent;
	///ͣ�����
	TVolumeType	LimitDayCnt;
	///�ܲ�������
	TVolumeType	VaryTickCnt;
	///ͣ�����
	TVolumeType	LimitTickCnt;
	///�Ƿ��в�������
	TBoolType	HasLimit;
	///�ǵ�ͣ
	TRatioType	PecentPerLimit;
	///ͣ�岨��
	TRatioType	InLimitVaryPct;
	///ͣ���Ⲩ��
	TRatioType	OutLimitVaryPct;
	///�����
	TVolumeType	LongVol;
	///������
	TVolumeType	ShortVol;
	///��������
	TPriceVaryDirType	Direction;
	///��������
	TPriceVaryDirType	BadDir;

bool operator==(const RspSafePriceRangeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BasePrice, ct.BasePrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(VaryPecent, ct.VaryPecent));
	if (!ret) return ret;
	ret = ret && (LimitDayCnt == ct.LimitDayCnt);
	if (!ret) return ret;
	ret = ret && (VaryTickCnt == ct.VaryTickCnt);
	if (!ret) return ret;
	ret = ret && (LimitTickCnt == ct.LimitTickCnt);
	if (!ret) return ret;
	ret = ret && (HasLimit == ct.HasLimit);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PecentPerLimit, ct.PecentPerLimit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(InLimitVaryPct, ct.InLimitVaryPct));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OutLimitVaryPct, ct.OutLimitVaryPct));
	if (!ret) return ret;
	ret = ret && (LongVol == ct.LongVol);
	if (!ret) return ret;
	ret = ret && (ShortVol == ct.ShortVol);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (BadDir == ct.BadDir);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���߲�ֲ�
struct FullInvestorGroupField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���߷������
	TInvestorIDType	InvestorGroupID;
	///Ͷ���߷�������
	TInvestorGroupNameType	InvestorGroupName;

bool operator==(const FullInvestorGroupField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupID, ct.InvestorGroupID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupName, ct.InvestorGroupName) == 0);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���߲�ѯ����
struct InvestorGroupMapField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///Ͷ���߷������
	TInvestorIDType	InvestorGroupID;

bool operator==(const InvestorGroupMapField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupID, ct.InvestorGroupID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ�۸񲨶���Ͷ���߷��յ�Ӱ��
struct QryPriceVaryEffectField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��������
	TNotifyClassType	RiskLevel;

bool operator==(const QryPriceVaryEffectField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (RiskLevel == ct.RiskLevel);
	if (!ret) return ret;

	return ret;
}
};

///���͹�˾Ͷ����
struct BrokerInvestorField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;

bool operator==(const BrokerInvestorField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///����ϵͳ����
struct TradeParamField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///��������
	TTradeParamIDType	TradeParamID;
	///��������ֵ
	TSettlementParamValueType	TradeParamValue;
	///��ע
	TMemoType	Memo;

bool operator==(const TradeParamField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (TradeParamID == ct.TradeParamID);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeParamValue, ct.TradeParamValue) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Memo, ct.Memo) == 0);
	if (!ret) return ret;

	return ret;
}
};

///���Ԥ��
struct RiskParkedOrderField
{
	///Ԥ�񱨵����
	TParkedOrderIDType	ParkedOrderID;
	///Ԥ�񵥱��ر��
	TParkedOrderIDType	LocalID;
	///����û�����
	TRiskUserTypeType	UserType;
	///Ԥ��״̬
	TRiskParkedOrderStatusType	Status;
	///Ԥ��״̬��Ϣ
	TErrorMsgType	StatusMsg;
	///��������
	TOrderTriggerTypeType	TriggerType;
	///���׽׶�
	TVolumeType	TradeSegment;
	///����������
	TExchangeIDType	ExchangeID;
	///���ǿƽ����
	TForceCloseTypeType	FCType;
	///����ǿƽ��������ʱ��
	TTimeType	Time1;
	///����ǿƽ��������ʱ�䣨���룩
	TMillisecType	Millisec1;
	///ǿƽ�����ύʱ��
	TTimeType	Time2;
	///ǿƽ�����ύʱ�䣨���룩
	TMillisecType	Millisec2;
	///ǿƽ�������
	TForceCloseSceneIdType	FCSceneId;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///��������
	TOrderRefType	OrderRef;
	///�û�����
	TUserIDType	UserID;
	///�����۸�����
	TOrderPriceTypeType	OrderPriceType;
	///��������
	TDirectionType	Direction;
	///��Ͽ�ƽ��־
	TCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	TPriceType	LimitPrice;
	///����
	TVolumeType	VolumeTotalOriginal;
	///��Ч������
	TTimeConditionType	TimeCondition;
	///GTD����
	TDateType	GTDDate;
	///�ɽ�������
	TVolumeConditionType	VolumeCondition;
	///��С�ɽ���
	TVolumeType	MinVolume;
	///��������
	TContingentConditionType	ContingentCondition;
	///ֹ���
	TPriceType	StopPrice;
	///ǿƽԭ��
	TForceCloseReasonType	ForceCloseReason;
	///�Զ������־
	TBoolType	IsAutoSuspend;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///������
	TRequestIDType	RequestID;
	///�û�ǿ����־
	TBoolType	UserForceClose;
	///�����ύ״̬
	TOrderSubmitStatusType	OrderSubmitStatus;
	///����״̬
	TOrderStatusType	OrderStatus;
	///����״̬��Ϣ
	TErrorMsgType	OrderStatusMsg;
	///�������
	TErrorIDType	ErrorID;
	///������Ϣ
	TErrorMsgType	ErrorMsg;
	///Ԥ��ʱ��
	TTimeType	ParkedTime;
	///Ԥ����
	TVolumeType	OriginalParkedVol;
	///Ԥ��ʱ��ƽ��
	TVolumeType	MaxCloseVol1;
	///����ʱ��ƽ��
	TVolumeType	MaxCloseVol2;
	///Ԥ��ʱ׷��
	TMoneyType	Call1;
	///����ʱ׷��
	TMoneyType	Call2;
	///Ԥ��ʱ�����
	TMoneyType	MoneyIO1;
	///����ʱ�����
	TMoneyType	MoneyIO2;
	///ɾ��ԭ��
	TCommentType	DeleteReason;
	///ǿƽ�ʽ��ͷű�׼
	TForceCloseReleaseType	ForceCloseRelease;

bool operator==(const RiskParkedOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ParkedOrderID, ct.ParkedOrderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LocalID, ct.LocalID) == 0);
	if (!ret) return ret;
	ret = ret && (UserType == ct.UserType);
	if (!ret) return ret;
	ret = ret && (Status == ct.Status);
	if (!ret) return ret;
	ret = ret && (strcmp(StatusMsg, ct.StatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (TriggerType == ct.TriggerType);
	if (!ret) return ret;
	ret = ret && (TradeSegment == ct.TradeSegment);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (FCType == ct.FCType);
	if (!ret) return ret;
	ret = ret && (strcmp(Time1, ct.Time1) == 0);
	if (!ret) return ret;
	ret = ret && (Millisec1 == ct.Millisec1);
	if (!ret) return ret;
	ret = ret && (strcmp(Time2, ct.Time2) == 0);
	if (!ret) return ret;
	ret = ret && (Millisec2 == ct.Millisec2);
	if (!ret) return ret;
	ret = ret && (strcmp(FCSceneId, ct.FCSceneId) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;
	ret = ret && (OrderSubmitStatus == ct.OrderSubmitStatus);
	if (!ret) return ret;
	ret = ret && (OrderStatus == ct.OrderStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderStatusMsg, ct.OrderStatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParkedTime, ct.ParkedTime) == 0);
	if (!ret) return ret;
	ret = ret && (OriginalParkedVol == ct.OriginalParkedVol);
	if (!ret) return ret;
	ret = ret && (MaxCloseVol1 == ct.MaxCloseVol1);
	if (!ret) return ret;
	ret = ret && (MaxCloseVol2 == ct.MaxCloseVol2);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Call1, ct.Call1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Call2, ct.Call2));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MoneyIO1, ct.MoneyIO1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MoneyIO2, ct.MoneyIO2));
	if (!ret) return ret;
	ret = ret && (strcmp(DeleteReason, ct.DeleteReason) == 0);
	if (!ret) return ret;
	ret = ret && (ForceCloseRelease == ct.ForceCloseRelease);
	if (!ret) return ret;

	return ret;
}
};

///ɾ�����Ԥ��
struct RemoveRiskParkedOrderField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///Ԥ�񱨵����
	TParkedOrderIDType	ParkedOrderID;

bool operator==(const RemoveRiskParkedOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParkedOrderID, ct.ParkedOrderID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///����ķ��Ԥ��
struct SeqRiskParkedOrderField
{
	///����Ψһ�����к�
	TSequenceNoType	UniqSequenceNo;
	///Ԥ�񱨵����
	TParkedOrderIDType	ParkedOrderID;
	///Ԥ�񵥱��ر��
	TParkedOrderIDType	LocalID;
	///����û�����
	TRiskUserTypeType	UserType;
	///Ԥ��״̬
	TRiskParkedOrderStatusType	Status;
	///Ԥ��״̬��Ϣ
	TErrorMsgType	StatusMsg;
	///��������
	TOrderTriggerTypeType	TriggerType;
	///���׽׶�
	TVolumeType	TradeSegment;
	///����������
	TExchangeIDType	ExchangeID;
	///���ǿƽ����
	TForceCloseTypeType	FCType;
	///����ǿƽ��������ʱ��
	TTimeType	Time1;
	///����ǿƽ��������ʱ�䣨���룩
	TMillisecType	Millisec1;
	///ǿƽ�����ύʱ��
	TTimeType	Time2;
	///ǿƽ�����ύʱ�䣨���룩
	TMillisecType	Millisec2;
	///ǿƽ�������
	TForceCloseSceneIdType	FCSceneId;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///��������
	TOrderRefType	OrderRef;
	///�û�����
	TUserIDType	UserID;
	///�����۸�����
	TOrderPriceTypeType	OrderPriceType;
	///��������
	TDirectionType	Direction;
	///��Ͽ�ƽ��־
	TCombOffsetFlagType	CombOffsetFlag;
	///���Ͷ���ױ���־
	TCombHedgeFlagType	CombHedgeFlag;
	///�۸�
	TPriceType	LimitPrice;
	///����
	TVolumeType	VolumeTotalOriginal;
	///��Ч������
	TTimeConditionType	TimeCondition;
	///GTD����
	TDateType	GTDDate;
	///�ɽ�������
	TVolumeConditionType	VolumeCondition;
	///��С�ɽ���
	TVolumeType	MinVolume;
	///��������
	TContingentConditionType	ContingentCondition;
	///ֹ���
	TPriceType	StopPrice;
	///ǿƽԭ��
	TForceCloseReasonType	ForceCloseReason;
	///�Զ������־
	TBoolType	IsAutoSuspend;
	///ҵ��Ԫ
	TBusinessUnitType	BusinessUnit;
	///������
	TRequestIDType	RequestID;
	///�û�ǿ����־
	TBoolType	UserForceClose;
	///�����ύ״̬
	TOrderSubmitStatusType	OrderSubmitStatus;
	///����״̬
	TOrderStatusType	OrderStatus;
	///����״̬��Ϣ
	TErrorMsgType	OrderStatusMsg;
	///�������
	TErrorIDType	ErrorID;
	///������Ϣ
	TErrorMsgType	ErrorMsg;
	///Ԥ��ʱ��
	TTimeType	ParkedTime;
	///Ԥ����
	TVolumeType	OriginalParkedVol;
	///Ԥ��ʱ��ƽ��
	TVolumeType	MaxCloseVol1;
	///����ʱ��ƽ��
	TVolumeType	MaxCloseVol2;
	///Ԥ��ʱ׷��
	TMoneyType	Call1;
	///����ʱ׷��
	TMoneyType	Call2;
	///Ԥ��ʱ�����
	TMoneyType	MoneyIO1;
	///����ʱ�����
	TMoneyType	MoneyIO2;
	///ɾ��ԭ��
	TCommentType	DeleteReason;
	///ǿƽ�ʽ��ͷű�׼
	TForceCloseReleaseType	ForceCloseRelease;

bool operator==(const SeqRiskParkedOrderField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(ParkedOrderID, ct.ParkedOrderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LocalID, ct.LocalID) == 0);
	if (!ret) return ret;
	ret = ret && (UserType == ct.UserType);
	if (!ret) return ret;
	ret = ret && (Status == ct.Status);
	if (!ret) return ret;
	ret = ret && (strcmp(StatusMsg, ct.StatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (TriggerType == ct.TriggerType);
	if (!ret) return ret;
	ret = ret && (TradeSegment == ct.TradeSegment);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (FCType == ct.FCType);
	if (!ret) return ret;
	ret = ret && (strcmp(Time1, ct.Time1) == 0);
	if (!ret) return ret;
	ret = ret && (Millisec1 == ct.Millisec1);
	if (!ret) return ret;
	ret = ret && (strcmp(Time2, ct.Time2) == 0);
	if (!ret) return ret;
	ret = ret && (Millisec2 == ct.Millisec2);
	if (!ret) return ret;
	ret = ret && (strcmp(FCSceneId, ct.FCSceneId) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;
	ret = ret && (OrderSubmitStatus == ct.OrderSubmitStatus);
	if (!ret) return ret;
	ret = ret && (OrderStatus == ct.OrderStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderStatusMsg, ct.OrderStatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParkedTime, ct.ParkedTime) == 0);
	if (!ret) return ret;
	ret = ret && (OriginalParkedVol == ct.OriginalParkedVol);
	if (!ret) return ret;
	ret = ret && (MaxCloseVol1 == ct.MaxCloseVol1);
	if (!ret) return ret;
	ret = ret && (MaxCloseVol2 == ct.MaxCloseVol2);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Call1, ct.Call1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Call2, ct.Call2));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MoneyIO1, ct.MoneyIO1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MoneyIO2, ct.MoneyIO2));
	if (!ret) return ret;
	ret = ret && (strcmp(DeleteReason, ct.DeleteReason) == 0);
	if (!ret) return ret;
	ret = ret && (ForceCloseRelease == ct.ForceCloseRelease);
	if (!ret) return ret;

	return ret;
}
};

///�����Ͷ������Ϣ
struct RiskInvestorInfoField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�ֻ�
	TTelephoneType	Mobile;
	///�����ʼ�
	TEMailType	EMail;
	///�Ƿ��������ʼ�
	TBoolType	IsEMail;
	///�Ƿ��������ʼ�
	TBoolType	IsSMS;
	///Ͷ��������
	TInvestorTypeType	InvestorType;
	///���Ҵ���
	TUOACountryCodeType	PhoneCountryCode;
	///����
	TAreaCodeType	PhoneAreaCode;
	///������Ȩ�˹��Ҵ���
	TUOACountryCodeType	OpenPhoneCountryCode;
	///������Ȩ������
	TAreaCodeType	OpenPhoneAreaCode;
	///ָ���µ��˹��Ҵ���
	TUOACountryCodeType	OrderPhoneCountryCode;
	///ָ���µ�������
	TAreaCodeType	OrderPhoneAreaCode;
	///�ʽ�����˹��Ҵ���
	TUOACountryCodeType	FundPhoneCountryCode;
	///�ʽ����������
	TAreaCodeType	FundPhoneAreaCode;
	///���㵥ȷ���˹��Ҵ���
	TUOACountryCodeType	SettlePhoneCountryCode;
	///���㵥ȷ��������
	TAreaCodeType	SettlePhoneAreaCode;

bool operator==(const RiskInvestorInfoField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Mobile, ct.Mobile) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EMail, ct.EMail) == 0);
	if (!ret) return ret;
	ret = ret && (IsEMail == ct.IsEMail);
	if (!ret) return ret;
	ret = ret && (IsSMS == ct.IsSMS);
	if (!ret) return ret;
	ret = ret && (InvestorType == ct.InvestorType);
	if (!ret) return ret;
	ret = ret && (strcmp(PhoneCountryCode, ct.PhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(PhoneAreaCode, ct.PhoneAreaCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenPhoneCountryCode, ct.OpenPhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenPhoneAreaCode, ct.OpenPhoneAreaCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderPhoneCountryCode, ct.OrderPhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderPhoneAreaCode, ct.OrderPhoneAreaCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FundPhoneCountryCode, ct.FundPhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FundPhoneAreaCode, ct.FundPhoneAreaCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlePhoneCountryCode, ct.SettlePhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlePhoneAreaCode, ct.SettlePhoneAreaCode) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�ͻ�����֪ͨ�汾A
struct RiskNotifyAField
{
	///����֪ͨ�¼��ڵ�������
	TSequenceNoType	SequenceNo;
	///�¼���������
	TDateType	EventDate;
	///�¼�����ʱ��
	TTimeType	EventTime;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�����͸÷���֪ͨ���û�����
	TUserIDType	UserID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///֪ͨ����
	TNotifyClassType	NotifyClass;
	///����֪ͨ;��
	TRiskNotifyMethodType	NotifyMethod;
	///����֪ͨ״̬
	TRiskNotifyStatusType	NotifyStatus;
	///֪ͨ����
	TRiskNofityInfoType	Message;
	///Ԥ���ֶ�(�˹�֪ͨ-֪ͨ����Ա������Ϊ����ԭ��)
	TCommentType	Reserve;

bool operator==(const RiskNotifyAField &ct) const
{
	bool ret = true;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (NotifyMethod == ct.NotifyMethod);
	if (!ret) return ret;
	ret = ret && (NotifyStatus == ct.NotifyStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(Message, ct.Message) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Reserve, ct.Reserve) == 0);
	if (!ret) return ret;

	return ret;
}
};

///����Ŀͻ�����֪ͨ�汾A
struct SeqRiskNotifyAField
{
	///����Ψһ�����к�
	TSequenceNoType	UniqSequenceNo;
	///����֪ͨ�¼��ڵ�������
	TSequenceNoType	SequenceNo;
	///�¼���������
	TDateType	EventDate;
	///�¼�����ʱ��
	TTimeType	EventTime;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�����͸÷���֪ͨ���û�����
	TUserIDType	UserID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///֪ͨ����
	TNotifyClassType	NotifyClass;
	///����֪ͨ;��
	TRiskNotifyMethodType	NotifyMethod;
	///����֪ͨ״̬
	TRiskNotifyStatusType	NotifyStatus;
	///֪ͨ����
	TRiskNofityInfoType	Message;
	///Ԥ���ֶ�(�˹�֪ͨ-֪ͨ����Ա������Ϊ����ԭ��)
	TCommentType	Reserve;

bool operator==(const SeqRiskNotifyAField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (NotifyMethod == ct.NotifyMethod);
	if (!ret) return ret;
	ret = ret && (NotifyStatus == ct.NotifyStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(Message, ct.Message) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Reserve, ct.Reserve) == 0);
	if (!ret) return ret;

	return ret;
}
};

///����ĳ����
struct SeqDepositField
{
	///����Ψһ�����к�
	TSequenceNoType	UniqSequenceNo;
	///���������
	TFundIOTypeType	Type;
	///��Ч��־-��Ч�����
	TAvailabilityFlagType	Flag;
	///�������
	TFundDirectionType	Direction;
	///������
	TDateType	TradingDay;
	///��ˮ��
	TDepositSeqNoType	SequenceNo;
	///ʱ��
	TTimeType	Time;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�������
	TMoneyType	Amount;

bool operator==(const SeqDepositField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (Type == ct.Type);
	if (!ret) return ret;
	ret = ret && (Flag == ct.Flag);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SequenceNo, ct.SequenceNo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Time, ct.Time) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Amount, ct.Amount));
	if (!ret) return ret;

	return ret;
}
};

///����Ľ��ױ���
struct SeqTradingCodeField
{
	///����Ψһ�����к�
	TSequenceNoType	UniqSequenceNo;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///����������
	TExchangeIDType	ExchangeID;
	///�ͻ�����
	TClientIDType	ClientID;
	///�Ƿ��Ծ
	TBoolType	IsActive;
	///������־(�޸Ļ�ɾ��)
	TActionFlagType	Action;
	///���ױ�������
	TClientIDTypeType	ClientIDType;

bool operator==(const SeqTradingCodeField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (Action == ct.Action);
	if (!ret) return ret;
	ret = ret && (ClientIDType == ct.ClientIDType);
	if (!ret) return ret;

	return ret;
}
};

///����û������¼�
struct RiskUserEventField
{
	///�û��¼���������
	TSequenceNoType	SequenceNo;
	///�¼���������
	TDateType	EventDate;
	///�¼�����ʱ��
	TTimeType	EventTime;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///�¼�����
	TRiskUserEventType	EventType;
	///�¼���Ϣ
	TUserEventInfoType	EventInfo;

bool operator==(const RiskUserEventField &ct) const
{
	bool ret = true;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (EventType == ct.EventType);
	if (!ret) return ret;
	ret = ret && (strcmp(EventInfo, ct.EventInfo) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ض������к�
struct RiskNtfSequenceField
{
	///���
	TSequenceNoType	SequenceNo;
	///ҵ����������
	TBusinessUnitType	DataType;

bool operator==(const RiskNtfSequenceField &ct) const
{
	bool ret = true;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(DataType, ct.DataType) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�ͻ�����֪ͨģ��汾A
struct RiskNotifyPatternAField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û����룬�����޸�ģ��ʱ��Ч
	TUserIDType	UserID;
	///֪ͨ����
	TNotifyClassType	NotifyClass;
	///����֪ͨ;��
	TRiskNotifyMethodType	NotifyMethod;
	///�Ƿ�����
	TBoolType	IsActive;
	///֪ͨģ������
	TRiskNofityInfoType	Pattern;
	///Ԥ������(��ʱֻ����warnlevel)
	TCommentType	Reserve;

bool operator==(const RiskNotifyPatternAField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (NotifyMethod == ct.NotifyMethod);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (strcmp(Pattern, ct.Pattern) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Reserve, ct.Reserve) == 0);
	if (!ret) return ret;

	return ret;
}
};

///Ʒ��ͣ���ǵ���
struct ProductLimitsField
{
	///Ʒ�ֻ��Լ����
	TInstrumentIDType	ProductID;
	///D1�ǵ���
	TRatioType	Limit1;
	///D2�ǵ���
	TRatioType	Limit2;
	///D3�ǵ���
	TRatioType	Limit3;
	///D4�ǵ���
	TRatioType	Limit4;
	///���֤����
	TRatioType	MaxMarginRate1;
	///�����
	TPriceType	Price;

bool operator==(const ProductLimitsField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Limit1, ct.Limit1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Limit2, ct.Limit2));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Limit3, ct.Limit3));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Limit4, ct.Limit4));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MaxMarginRate1, ct.MaxMarginRate1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price, ct.Price));
	if (!ret) return ret;

	return ret;
}
};

///����Ԥ���ʽ��˻�
struct PredictRiskAccountField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��ʼȨ��
	TMoneyType	Balance0;
	///D1����
	TMoneyType	Lose1;
	///D1Ȩ��
	TMoneyType	Balance1;
	///D2����
	TMoneyType	Lose2;
	///D2Ȩ��
	TMoneyType	Balance2;
	///D3����
	TMoneyType	Lose3;
	///D3Ȩ��
	TMoneyType	Balance3;
	///D4����
	TMoneyType	Lose4;
	///D4Ȩ��
	TMoneyType	Balance4;

bool operator==(const PredictRiskAccountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance0, ct.Balance0));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Lose1, ct.Lose1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance1, ct.Balance1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Lose2, ct.Lose2));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance2, ct.Balance2));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Lose3, ct.Lose3));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance3, ct.Balance3));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Lose4, ct.Lose4));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance4, ct.Balance4));
	if (!ret) return ret;

	return ret;
}
};

///����Ԥ��ֲ�
struct PredictRiskPositionField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///��ͷ�ֲ�
	TVolumeType	LongVol;
	///��ͷ�ֲ�
	TVolumeType	ShortVol;
	///���ֲ�
	TVolumeType	NetVol;
	///��ʼ�۸�
	TPriceType	Price0;
	///D1�����
	TPriceType	Price1;
	///D1�ǵ���
	TRatioType	Limit1;
	///D2�ǵ���
	TRatioType	Limit2;
	///D3�ǵ���
	TRatioType	Limit3;
	///D4�ǵ���
	TRatioType	Limit4;
	///��Լ����
	TVolumeMultipleType	Multiple;
	///D1����
	TMoneyType	Lose1;
	///D2����
	TMoneyType	Lose2;
	///D3����
	TMoneyType	Lose3;
	///D4����
	TMoneyType	Lose4;
	///��������
	TVolumeType	LeftVol;
	///D1Ӧǿƽ
	TVolumeType	ShouldClose;
	///�������ǿƽ
	TVolumeType	CanClose;
	///��ǰ��֤����
	TRatioType	CurrMarginRate;
	///������֤����
	TRatioType	MarginRate1;
	///���ֱֲ�֤��
	TMoneyType	NetMargin1;
	///D1����
	TMoneyType	Available1;
	///1�ֱ�֤��
	TMoneyType	Margin1PerVol;

bool operator==(const PredictRiskPositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (LongVol == ct.LongVol);
	if (!ret) return ret;
	ret = ret && (ShortVol == ct.ShortVol);
	if (!ret) return ret;
	ret = ret && (NetVol == ct.NetVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price0, ct.Price0));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price1, ct.Price1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Limit1, ct.Limit1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Limit2, ct.Limit2));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Limit3, ct.Limit3));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Limit4, ct.Limit4));
	if (!ret) return ret;
	ret = ret && (Multiple == ct.Multiple);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Lose1, ct.Lose1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Lose2, ct.Lose2));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Lose3, ct.Lose3));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Lose4, ct.Lose4));
	if (!ret) return ret;
	ret = ret && (LeftVol == ct.LeftVol);
	if (!ret) return ret;
	ret = ret && (ShouldClose == ct.ShouldClose);
	if (!ret) return ret;
	ret = ret && (CanClose == ct.CanClose);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrMarginRate, ct.CurrMarginRate));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRate1, ct.MarginRate1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NetMargin1, ct.NetMargin1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Available1, ct.Available1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Margin1PerVol, ct.Margin1PerVol));
	if (!ret) return ret;

	return ret;
}
};

///����Ԥ�����
struct PredictRiskParamField
{
	///D1����
	TVolumeType	D1;
	///�Ƿ������Ч��¼
	TBoolType	IsFilter;

bool operator==(const PredictRiskParamField &ct) const
{
	bool ret = true;
	ret = ret && (D1 == ct.D1);
	if (!ret) return ret;
	ret = ret && (IsFilter == ct.IsFilter);
	if (!ret) return ret;

	return ret;
}
};

///�����ͬ��Ͷ����
struct RiskSyncInvestorField
{
	///������־(�޸Ļ�ɾ��)
	TActionFlagType	Action;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���߷������
	TInvestorIDType	InvestorGroupID;
	///Ͷ��������
	TPartyNameType	InvestorName;
	///֤������
	TIdCardTypeType	IdentifiedCardType;
	///֤������
	TIdentifiedCardNoType	IdentifiedCardNo;
	///�Ƿ��Ծ
	TBoolType	IsActive;
	///��ϵ�绰
	TTelephoneType	Telephone;
	///ͨѶ��ַ
	TAddressType	Address;
	///��������
	TDateType	OpenDate;
	///�ֻ�
	TMobileType	Mobile;

bool operator==(const RiskSyncInvestorField &ct) const
{
	bool ret = true;
	ret = ret && (Action == ct.Action);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupID, ct.InvestorGroupID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorName, ct.InvestorName) == 0);
	if (!ret) return ret;
	ret = ret && (IdentifiedCardType == ct.IdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(IdentifiedCardNo, ct.IdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (strcmp(Telephone, ct.Telephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Address, ct.Address) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenDate, ct.OpenDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Mobile, ct.Mobile) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��Լ�۸�
struct InstrumentPriceField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///�۸�����
	TPriceTypeType	PriceType;
	///�۸�
	TPriceType	Price;

bool operator==(const InstrumentPriceField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (PriceType == ct.PriceType);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price, ct.Price));
	if (!ret) return ret;

	return ret;
}
};

///Ͷ��������ϵ����Ϣ��Hashֵ
struct InvestorHashField
{
	///��������ҵ�����ͣ�����������MD5ֵ
	TFullMD5Type	MD5;
	///Ͷ���ߴ���(Ϊ�մ�������Ͷ���ߴ���)
	TInvestorIDType	InvestorIDBeg;
	///Ͷ���ߴ���(Ϊ�մ�������Ͷ���ߴ���)
	TInvestorIDType	InvestorIDEnd;

bool operator==(const InvestorHashField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(MD5, ct.MD5) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDBeg, ct.InvestorIDBeg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDEnd, ct.InvestorIDEnd) == 0);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���ߴ���
struct InvestorIDRangeField
{
	///Ͷ���ߴ���(Ϊ�մ�������Ͷ���ߴ���)
	TInvestorIDType	InvestorIDBeg;
	///Ͷ���ߴ���(Ϊ�մ�������Ͷ���ߴ���)
	TInvestorIDType	InvestorIDEnd;

bool operator==(const InvestorIDRangeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InvestorIDBeg, ct.InvestorIDBeg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDEnd, ct.InvestorIDEnd) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ͨѶʱ�α�ŵ������
struct CommPhaseSequenceField
{
	///ͨѶʱ�α��
	TCommPhaseNoType	CommPhaseNo;
	///����ϵ�к�
	TSequenceSeriesType	SequenceSeries;
	///���
	TSequenceNoType	SequenceNo;

bool operator==(const CommPhaseSequenceField &ct) const
{
	bool ret = true;
	ret = ret && (CommPhaseNo == ct.CommPhaseNo);
	if (!ret) return ret;
	ret = ret && (SequenceSeries == ct.SequenceSeries);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;

	return ret;
}
};

///����û�Ȩ��
struct RiskUserFunctionField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///Ȩ������
	TFunctionIDType	Function;

bool operator==(const RiskUserFunctionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Function, ct.Function) == 0);
	if (!ret) return ret;

	return ret;
}
};

///����״̬
struct LinkStatusField
{
	///����״̬
	TLinkStatusType	LinkStatus;

bool operator==(const LinkStatusField &ct) const
{
	bool ret = true;
	ret = ret && (LinkStatus == ct.LinkStatus);
	if (!ret) return ret;

	return ret;
}
};

///���ͬ��Ͷ�����ʽ��˻�
struct RiskSyncAccountField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�������
	TMoneyType	Withdraw;
	///��ǰ��֤���ܶ�
	TMoneyType	CurrMargin;
	///������
	TMoneyType	Commission;
	///ƽ��ӯ��
	TMoneyType	CloseProfit;
	///�ֲ�ӯ��
	TMoneyType	PositionProfit;
	///��Ȩ��
	TMoneyType	Balance;
	///�����ʽ�
	TMoneyType	Available;
	///��Ѻ���
	TMoneyType	Mortgage;
	///��������֤��
	TMoneyType	ExchangeMargin;
	///��ȡ�ʽ�
	TMoneyType	WithdrawQuota;
	///���ö��
	TMoneyType	Credit;
	///��֤�𶳽�
	TMoneyType	FrozenMargin;
	///�����Ѷ���
	TMoneyType	FrozenCommission;
	///����ʱ��
	TTimeType RecvTime;

bool operator==(const RiskSyncAccountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Withdraw, ct.Withdraw));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrMargin, ct.CurrMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Commission, ct.Commission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfit, ct.PositionProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance, ct.Balance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Available, ct.Available));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Mortgage, ct.Mortgage));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeMargin, ct.ExchangeMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(WithdrawQuota, ct.WithdrawQuota));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Credit, ct.Credit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCommission, ct.FrozenCommission));
	if (!ret) return ret;

	return ret;
}
};

///����ʽ��˻�������Ϣ
struct SeqPreRiskAccountField
{
	///����Ψһ�����к�
	TSequenceNoType	UniqSequenceNo;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///Ͷ�����ʺ�
	TAccountIDType	AccountID;
	///�ϴ���Ѻ���
	TMoneyType	PreMortgage;
	///�ϴ����ö��
	TMoneyType	PreCredit;
	///�ϴδ���
	TMoneyType	PreDeposit;
	///�ϴ���Ȩ��
	TMoneyType	PreBalance;
	///�ϴ�ռ�õı�֤��
	TMoneyType	PreMargin;
	///����׼����
	TMoneyType	Reserve;
	///�ϴν�������֤��
	TMoneyType	PreExchMargin;
	///��ʷǿƽ����
	TVolumeType	ForceCloseStat;
	///Ͷ���߽��֤��
	TMoneyType	DeliveryMargin;
	///���������֤��
	TMoneyType	ExchangeDeliveryMargin;
	///����ʱ��
	TTimeType RecvTime;

bool operator==(const SeqPreRiskAccountField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(AccountID, ct.AccountID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMortgage, ct.PreMortgage));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreCredit, ct.PreCredit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreDeposit, ct.PreDeposit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreBalance, ct.PreBalance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMargin, ct.PreMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Reserve, ct.Reserve));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreExchMargin, ct.PreExchMargin));
	if (!ret) return ret;
	ret = ret && (ForceCloseStat == ct.ForceCloseStat);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(DeliveryMargin, ct.DeliveryMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeDeliveryMargin, ct.ExchangeDeliveryMargin));
	if (!ret) return ret;

	return ret;
}
};
struct RiskAllAccountField
{
	//����
	SeqPreRiskAccountField pre;
	//����
	RiskSyncAccountField   cur;
};
///��ѯ��������¼Ӧ��
struct RspLocalFrontLoginField
{
	///ͨѶʱ�α��
	TCommPhaseNoType	CommPhaseNo;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///ǰ�ñ��
	TFrontIDType	FrontID;
	///���ǰ�������ӵ�SessionID
	TSessionIDType	FrontSessionID;

bool operator==(const RspLocalFrontLoginField &ct) const
{
	bool ret = true;
	ret = ret && (CommPhaseNo == ct.CommPhaseNo);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (FrontSessionID == ct.FrontSessionID);
	if (!ret) return ret;

	return ret;
}
};

///ģ���滻�ֶ�
struct NoticeTokenField
{
	///�Զ��滻�ֶ�
	TExchangeNameType	Token;
	///�Զ��滻�ֶε�����
	TBrokerNameType	Description;

bool operator==(const NoticeTokenField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(Token, ct.Token) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Description, ct.Description) == 0);
	if (!ret) return ret;

	return ret;
}
};

///֪ͨģ��
struct NoticePatternField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///ҵ������
	TParamNameType	BizType;
	///֪ͨ;��
	TRiskNotifyMethodType	Method;
	///ҵ������
	TParamNameType	BizName;
	///����޸�ģ����û�����
	TUserIDType	UserID;
	///�Ƿ�����
	TBoolType	IsActive;
	///֪ͨģ������
	TContentType	Pattern;

bool operator==(const NoticePatternField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BizType, ct.BizType) == 0);
	if (!ret) return ret;
	ret = ret && (Method == ct.Method);
	if (!ret) return ret;
	ret = ret && (strcmp(BizName, ct.BizName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (strcmp(Pattern, ct.Pattern) == 0);
	if (!ret) return ret;

	return ret;
}
};

///ҵ��֪ͨ
struct BizNoticeField
{
	///�¼���������
	TDateType	TradingDay;
	///֪ͨ�¼��ڵ�������
	TSequenceNoType	SequenceNo;
	///����֪ͨ;��
	TRiskNotifyMethodType	Method;
	///�¼�����ʱ��
	TTimeType	EventTime;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///ҵ������
	TParamNameType	BizType;
	///֪ͨ״̬
	TRiskNotifyStatusType	Status;
	///֪ͨ����
	TContentType	Message;
	///������Ϣ
	TErrorMsgType	ErrorMsg;

bool operator==(const BizNoticeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (Method == ct.Method);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BizType, ct.BizType) == 0);
	if (!ret) return ret;
	ret = ret && (Status == ct.Status);
	if (!ret) return ret;
	ret = ret && (strcmp(Message, ct.Message) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���߲���
struct RiskInvestorParamField
{
	///��������
	TParamIDType	ParamID;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///����ֵ
	TParamValueType	ParamValue;

bool operator==(const RiskInvestorParamField &ct) const
{
	bool ret = true;
	ret = ret && (ParamID == ct.ParamID);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParamValue, ct.ParamValue) == 0);
	if (!ret) return ret;

	return ret;
}
};

///�仯����
struct VaryMarketDataField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///���¼�
	TPriceType	LastPrice;

bool operator==(const VaryMarketDataField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LastPrice, ct.LastPrice));
	if (!ret) return ret;

	return ret;
}
};

///��Լ�۸�����
struct PriceRangeField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///�۸�1
	TPriceType	Price1;
	///�۸�2
	TPriceType	Price2;

bool operator==(const PriceRangeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price1, ct.Price1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price2, ct.Price2));
	if (!ret) return ret;

	return ret;
}
};

///�����ҵ��֪ͨ
struct SeqBizNoticeField
{
	///����Ψһ�����к�
	TSequenceNoType	UniqSequenceNo;
	///�¼���������
	TDateType	TradingDay;
	///֪ͨ�¼��ڵ�������
	TSequenceNoType	SequenceNo;
	///����֪ͨ;��
	TRiskNotifyMethodType	Method;
	///�¼�����ʱ��
	TTimeType	EventTime;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///ҵ������
	TParamNameType	BizType;
	///֪ͨ״̬
	TRiskNotifyStatusType	Status;
	///֪ͨ����
	TContentType	Message;
	///������Ϣ
	TErrorMsgType	ErrorMsg;

bool operator==(const SeqBizNoticeField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (Method == ct.Method);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BizType, ct.BizType) == 0);
	if (!ret) return ret;
	ret = ret && (Status == ct.Status);
	if (!ret) return ret;
	ret = ret && (strcmp(Message, ct.Message) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ز�����Ϣ
struct RiskParamInfoField
{
	///��������
	TParamIDType	ParamID;
	///����˵��
	TDescriptionType	Description;

bool operator==(const RiskParamInfoField &ct) const
{
	bool ret = true;
	ret = ret && (ParamID == ct.ParamID);
	if (!ret) return ret;
	ret = ret && (strcmp(Description, ct.Description) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ص�¼��Ϣ
struct RiskLoginInfoField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�û�����
	TUserIDType	UserID;
	///����ǰ�ûỰ���
	TSessionIDType	LocalSessionID;
	///���ǰ�ûỰ���
	TSessionIDType	SessionID;
	///���ǰ�ñ��
	TFrontIDType	FrontID;

bool operator==(const RiskLoginInfoField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (LocalSessionID == ct.LocalSessionID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;

	return ret;
}
};

///���֪ͨģ��
struct RiskPatternField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///ҵ������
	TParamNameType	BizType;
	///ģ�����
	TParamIDType	PatternID;
	///ģ������
	TParamNameType	PatternName;
	///֪ͨģ������
	TContentType	Pattern;

bool operator==(const RiskPatternField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BizType, ct.BizType) == 0);
	if (!ret) return ret;
	ret = ret && (PatternID == ct.PatternID);
	if (!ret) return ret;
	ret = ret && (strcmp(PatternName, ct.PatternName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Pattern, ct.Pattern) == 0);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ����֪ͨģ��
struct InvestorPatternField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///ҵ������
	TParamNameType	BizType;
	///֪ͨ;��
	TRiskNotifyMethodType	Method;
	///ģ�����
	TParamIDType	PatternID;
	///�Ƿ�����
	TBoolType	IsActive;

bool operator==(const InvestorPatternField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BizType, ct.BizType) == 0);
	if (!ret) return ret;
	ret = ret && (Method == ct.Method);
	if (!ret) return ret;
	ret = ret && (PatternID == ct.PatternID);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;

	return ret;
}
};

///�ͻ�����֪ͨ�汾B
struct RiskNotifyBField
{
	///����֪ͨ�¼��ڵ�������
	TSequenceNoType	SequenceNo;
	///�¼���������
	TDateType	EventDate;
	///�¼�����ʱ��
	TTimeType	EventTime;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�����͸÷���֪ͨ���û�����
	TUserIDType	UserID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///֪ͨ����
	TNotifyClassType	NotifyClass;
	///����֪ͨ;��
	TRiskNotifyMethodType	NotifyMethod;
	///����֪ͨ״̬
	TRiskNotifyStatusType	NotifyStatus;
	///֪ͨ����
	TContentType	Message;
	///Ԥ���ֶ�(�˹�֪ͨ-֪ͨ����Ա������Ϊ����ԭ��)
	TCommentType	Reserve;

bool operator==(const RiskNotifyBField &ct) const
{
	bool ret = true;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (NotifyMethod == ct.NotifyMethod);
	if (!ret) return ret;
	ret = ret && (NotifyStatus == ct.NotifyStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(Message, ct.Message) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Reserve, ct.Reserve) == 0);
	if (!ret) return ret;

	return ret;
}
};

///����Ŀͻ�����֪ͨ�汾B
struct SeqRiskNotifyBField
{
	///����Ψһ�����к�
	TSequenceNoType	UniqSequenceNo;
	///����֪ͨ�¼��ڵ�������
	TSequenceNoType	SequenceNo;
	///�¼���������
	TDateType	EventDate;
	///�¼�����ʱ��
	TTimeType	EventTime;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///�����͸÷���֪ͨ���û�����
	TUserIDType	UserID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///֪ͨ����
	TNotifyClassType	NotifyClass;
	///����֪ͨ;��
	TRiskNotifyMethodType	NotifyMethod;
	///����֪ͨ״̬
	TRiskNotifyStatusType	NotifyStatus;
	///֪ͨ����
	TContentType	Message;
	///Ԥ���ֶ�(�˹�֪ͨ-֪ͨ����Ա������Ϊ����ԭ��)
	TCommentType	Reserve;

bool operator==(const SeqRiskNotifyBField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (NotifyMethod == ct.NotifyMethod);
	if (!ret) return ret;
	ret = ret && (NotifyStatus == ct.NotifyStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(Message, ct.Message) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Reserve, ct.Reserve) == 0);
	if (!ret) return ret;

	return ret;
}
};

///��ѯ�ֲֳɽ��������Ĳ���
struct QryStatField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///������Ʒ�ֺ�Լ�������(����������ʹ��ǰ׺e:����ʽ��cu,e:SHFE,cu1105)
	TString100Type	ExchangeProductInstID;
	///��������
	TStatSortTypeType	SortType;
	///�����������ؽ��
	TVolumeType	ResultCount;
	///���������ؽ��
	TRatioType	ResultRatio;

bool operator==(const QryStatField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeProductInstID, ct.ExchangeProductInstID) == 0);
	if (!ret) return ret;
	ret = ret && (SortType == ct.SortType);
	if (!ret) return ret;
	ret = ret && (ResultCount == ct.ResultCount);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ResultRatio, ct.ResultRatio));
	if (!ret) return ret;

	return ret;
}
};

///�ֲ�����ͳ��
struct PositionStatField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///������Ʒ�ֺ�Լ�������(����������ʹ��ǰ׺e:����ʽ��cu,e:SHFE,cu1105)
	TString100Type	ExchangeProductInstID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��ֲ�
	TVolumeType	YdPosition;
	///�ֲܳ�
	TVolumeType	Position;
	///��ͷ�ֲ�
	TVolumeType	LongPosition;
	///��ͷ�ֲ�
	TVolumeType	ShortPosition;
	///���ֲ�
	TVolumeType	NetPosition;
	///Ͷ����ͷ�ֲ�
	TVolumeType	SpecuLongPosi;
	///Ͷ����ͷ�ֲ�
	TVolumeType	SpecuShortPosi;
	///��ֵ��ͷ�ֲ�
	TVolumeType	HedgeLongPosi;
	///��ֵ��ͷ�ֲ�
	TVolumeType	HedgeShortPosi;
	///���
	TVolumeType	TodayPosition;
	///�ֲֳܳɱ�
	TMoneyType	PositionCost;

bool operator==(const PositionStatField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeProductInstID, ct.ExchangeProductInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (YdPosition == ct.YdPosition);
	if (!ret) return ret;
	ret = ret && (Position == ct.Position);
	if (!ret) return ret;
	ret = ret && (LongPosition == ct.LongPosition);
	if (!ret) return ret;
	ret = ret && (ShortPosition == ct.ShortPosition);
	if (!ret) return ret;
	ret = ret && (NetPosition == ct.NetPosition);
	if (!ret) return ret;
	ret = ret && (SpecuLongPosi == ct.SpecuLongPosi);
	if (!ret) return ret;
	ret = ret && (SpecuShortPosi == ct.SpecuShortPosi);
	if (!ret) return ret;
	ret = ret && (HedgeLongPosi == ct.HedgeLongPosi);
	if (!ret) return ret;
	ret = ret && (HedgeShortPosi == ct.HedgeShortPosi);
	if (!ret) return ret;
	ret = ret && (TodayPosition == ct.TodayPosition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionCost, ct.PositionCost));
	if (!ret) return ret;

	return ret;
}
};

///�ɽ�����ͳ��
struct TradeStatField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///������Ʒ�ֺ�Լ�������(����������ʹ��ǰ׺e:����ʽ��cu,e:SHFE,cu1105)
	TString100Type	ExchangeProductInstID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�ܳɽ���
	TVolumeType	TotalVol;
	///�ܳɽ���
	TMoneyType	TotalAmt;
	///��ɽ���
	TVolumeType	BuyVol;
	///��ɽ���
	TMoneyType	BuyAmt;
	///���ɽ���
	TVolumeType	SellVol;
	///���ɽ���
	TMoneyType	SellAmt;
	///������ɽ���
	TVolumeType	NetVol;
	///������ɽ���
	TMoneyType	NetAmt;

bool operator==(const TradeStatField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeProductInstID, ct.ExchangeProductInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (TotalVol == ct.TotalVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(TotalAmt, ct.TotalAmt));
	if (!ret) return ret;
	ret = ret && (BuyVol == ct.BuyVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BuyAmt, ct.BuyAmt));
	if (!ret) return ret;
	ret = ret && (SellVol == ct.SellVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SellAmt, ct.SellAmt));
	if (!ret) return ret;
	ret = ret && (NetVol == ct.NetVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NetAmt, ct.NetAmt));
	if (!ret) return ret;

	return ret;
}
};

///��������ͳ��
struct OrderStatField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///������Ʒ�ֺ�Լ�������(����������ʹ��ǰ׺e:����ʽ��cu,e:SHFE,cu1105)
	TString100Type	ExchangeProductInstID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///�ܱ�����
	TVolumeType	TotalVol;
	///�򱨵���
	TVolumeType	BuyVol;
	///��������
	TVolumeType	SellVol;
	///�����뱨����
	TVolumeType	NetVol;
	///�гɽ�������
	TVolumeType	TradeCount;
	///�ܱ�����
	TVolumeType	TotalCount;

bool operator==(const OrderStatField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeProductInstID, ct.ExchangeProductInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (TotalVol == ct.TotalVol);
	if (!ret) return ret;
	ret = ret && (BuyVol == ct.BuyVol);
	if (!ret) return ret;
	ret = ret && (SellVol == ct.SellVol);
	if (!ret) return ret;
	ret = ret && (NetVol == ct.NetVol);
	if (!ret) return ret;
	ret = ret && (TradeCount == ct.TradeCount);
	if (!ret) return ret;
	ret = ret && (TotalCount == ct.TotalCount);
	if (!ret) return ret;

	return ret;
}
};

///�ֱ���ϵͳ���
struct DRSysIDField
{
	///�ֱ���ϵͳ���(ÿ����ϵͳ����һ�����)
	TDRSysIDType	DRSysID;

bool operator==(const DRSysIDField &ct) const
{
	bool ret = true;
	ret = ret && (DRSysID == ct.DRSysID);
	if (!ret) return ret;

	return ret;
}
};

///��ѯͶ���߱�֤����
struct QryInvestorMarginRateField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;

bool operator==(const QryInvestorMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;

	return ret;
}
};

///Ͷ���߱�֤����
struct InvestorMarginRateField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	TInvestorRangeType	InvestorRange;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///��ͷ��֤����
	TRatioType	LongMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	LongMarginRatioByVolume;
	///��ͷ��֤����
	TRatioType	ShortMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	ShortMarginRatioByVolume;

bool operator==(const InvestorMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;

	return ret;
}
};

///ѹ�����Խ���۸�
struct STPriceField
{
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���߷�Χ
	TInvestorRangeType	InvestorRange;
	///Ͷ���ߴ����ģ�����
	TInvestorIDType	InvestorID;
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///�۸�����
	TPriceTypeType	PriceType;
	///�۸�
	TPriceType	Price;

bool operator==(const STPriceField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (PriceType == ct.PriceType);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price, ct.Price));
	if (!ret) return ret;

	return ret;
}
};

///ѹ�����Ա�֤����
struct STMarginRateField
{
	///��Լ����
	TInstrumentIDType	InstrumentID;
	///Ͷ���߷�Χ
	TInvestorRangeType	InvestorRange;
	///���͹�˾����
	TBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TInvestorIDType	InvestorID;
	///Ͷ���ױ���־
	THedgeFlagType	HedgeFlag;
	///��ͷ��֤����
	TRatioType	LongMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	LongMarginRatioByVolume;
	///��ͷ��֤����
	TRatioType	ShortMarginRatioByMoney;
	///��ͷ��֤���
	TMoneyType	ShortMarginRatioByVolume;

bool operator==(const STMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;

	return ret;
}
};



#endif
