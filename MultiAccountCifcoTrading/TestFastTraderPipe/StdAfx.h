// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__DB763F8D_4DC5_4C9D_BA3F_96CB29EC3F57__INCLUDED_)
#define AFX_STDAFX_H__DB763F8D_4DC5_4C9D_BA3F_96CB29EC3F57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "..\inc\Module-Interface\interfaceDefines.h"
#define MASTERNAMEDPIPESTRING "\\\\.\\pipe\\FastTrader\\v0_1\\Master"

#include "..\inc\ctp\ThostFtdcMdApi.h"
#include "..\inc\ctp\ThostFtdcTraderApi.h"
#include "..\inc\ctp\ThostFtdcUserApiDataType.h"
#include "..\inc\ctp\ThostFtdcUserApiStruct.h"


#define ORDERCLASS_NewOrder 'N'
#define ORDERCLASS_ReplaceOrder 'R'

typedef char CommonOrderClassType;
typedef char CommonAccountType[32];
typedef char CommonLocalRequestIDType[32];
typedef char CommonLocalRefIDType[32];
typedef char CommonExchangeIDType[32];
typedef char CommonOrderRefType[32];

typedef char PatsystemAccountType[50];

typedef char InstrumentIDType[64];

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

    //FasttraderID,����ÿһ����¼��Key������һ��Ψһ���
    long FTID;         
    //ÿ���������ݷ����仯��UpdateSeq��һ
    long UpdateSeq;

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
    int         CloseOrderVolume;           //ƽ�֡�ǿƽ������ǿƽ�ҵ���
    int         CloseTodayOrderVolume;      //ƽ��ҵ���
    int         CloseYesterdayOrderVolume;  //ƽ��ҵ���
    int         CanCloseVolume;             //��ƽ��Position-TotalCloseOrderVolume
    int         CanCloseTodayVolume;        //��ƽ����TodayPosition-CloseTodayOrderVolume
    int         CanCloseydVolume;           //��ƽ����YdPosition-CloseYesterdayOrderVolume

    double      AveragePositionPrice;       //�ֲ־���(�ֲֳɱ�/�ֲ���)
    double      AverageOpenPrice;           //���־���((���տ��ֳɱ�+��ʷ���ֳɱ�)/�ܿ�����)

    int         TodayCloseYdVolume;         //��ƽ��ʷ����
	int         YdPositionRemain;	            //����ʣ���λ���ӳֲ���ϸ����

    double      CurPrice;                   //�ּ�

    //����ֲ�ӯ���õ��ּ�
    double      CurPriceUsedByCalcu;                   

    //FasttraderID,����ÿһ����¼��Key������һ��Ψһ���
    long FTID;                                                
    //ÿ���������ݷ����仯��UpdateSeq��һ
    long UpdateSeq;

};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DB763F8D_4DC5_4C9D_BA3F_96CB29EC3F57__INCLUDED_)
