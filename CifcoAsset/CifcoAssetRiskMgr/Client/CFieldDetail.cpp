
#include "stdafx.h"
#include "CFieldDetail.h"
#include "UserApiDataType.h"


const static char* __UNKOWN__="δ֪";

///��������
const char* Client::CFieldDetail::Direction2String(char param)
{
	const static char* Buy="��";
	const static char* Sell="��";
	switch(param)
	{
		case T_D_Buy:return Buy;
		case T_D_Sell:return Sell;
		default: return __UNKOWN__;
	}
}

///��ƽ��־
const char* Client::CFieldDetail::OffsetFlag2String(char param)
{
	const static char* Open="����";
	const static char* Close="ƽ��";
	const static char* ForceClose="ǿƽ";
	const static char* CloseToday="ƽ��";
	const static char* CloseYesterday="ƽ��";
	switch(param)
	{
		case T_OF_Open:		return Open;
		case T_OF_Close:		return Close;
		case T_OF_ForceClose:	return ForceClose;
		case T_OF_CloseToday:	return CloseToday;
		case T_OF_CloseYesterday:return CloseYesterday;
		default: return __UNKOWN__;
	}
}

///����״̬
const char* Client::CFieldDetail::OrderStatus2String(char param)
{
	const static char* AllTraded="ȫ���ɽ�";
	const static char* PartTradedQueueing="���ֳɽ����ڶ�����";
	const static char* PartTradedNotQueueing="���ֳɽ����ڶ�����";
	const static char* NoTradeQueueing="δ�ɽ����ڶ�����";
	const static char* NoTradeNotQueueing="δ�ɽ����ڶ�����";
	const static char* Canceled="����";
	const static char* Unknown="δ֪";
	const static char* NotTouched="����δ����";
	const static char* Touched="�����Ѵ���";
    const static char* Error="����";
	switch(param)
	{
		case T_OST_AllTraded:				return AllTraded;
		case T_OST_PartTradedQueueing:		return PartTradedQueueing;
		case T_OST_PartTradedNotQueueing:	return PartTradedNotQueueing;
		case T_OST_NoTradeQueueing:		return NoTradeQueueing;
		case T_OST_NoTradeNotQueueing:		return NoTradeNotQueueing;
		case T_OST_Canceled:				return Canceled;
		case T_OST_Unknown:				return Unknown;
		case T_OST_NotTouched:				return NotTouched;
		case T_OST_Touched:				return Touched;
        //case T_OST_ERROE:				    return Error;
		default: return __UNKOWN__;
	}
}

///�����ύ״̬
const char* Client::CFieldDetail::OrderSubmitStatus2String(char param)
{
	const static char* InsertSubmitted="�Ѿ��ύ";
	const static char* CancelSubmitted="�����Ѿ��ύ";
	const static char* ModifySubmitted="�޸��Ѿ��ύ";
	const static char* Accepted="�Ѿ�����";
	const static char* InsertRejected="�����Ѿ����ܾ�";
	const static char* CancelRejected="�����Ѿ����ܾ�";
	const static char* ModifyRejected="�ĵ��Ѿ����ܾ�";

	switch(param)
	{
		case T_OSS_InsertSubmitted:	return InsertSubmitted;
		case T_OSS_CancelSubmitted:	return CancelSubmitted;
		case T_OSS_ModifySubmitted:	return ModifySubmitted;
		case T_OSS_Accepted:			return Accepted;
		case T_OSS_InsertRejected:		return InsertRejected;
		case T_OSS_CancelRejected:		return CancelRejected;
		case T_OSS_ModifyRejected:		return ModifyRejected;
		default: return __UNKOWN__;
	}
}

///�����۸�����
const char* Client::CFieldDetail::OrderPriceType2String(char param)
{
	const static char* AnyPrice="�����";
	const static char* LimitPrice="�޼�";
	const static char* BestPrice="���ż�";
	const static char* LastPrice="���¼�";
	const static char* LastPricePlusOneTicks="���¼۸����ϸ�1��ticks";
	const static char* LastPricePlusTwoTicks="���¼۸����ϸ�2��ticks";
	const static char* LastPricePlusThreeTicks="���¼۸����ϸ�3��ticks";
	const static char* AskPrice1="��һ��";
	const static char* AskPrice1PlusOneTicks="��һ�۸����ϸ�1��ticks";
	const static char* AskPrice1PlusTwoTicks="��һ�۸����ϸ�2��ticks";
	const static char* AskPrice1PlusThreeTicks="��һ�۸����ϸ�3��ticks";
	const static char* BidPrice1="��һ��";
	const static char* BidPrice1PlusOneTicks="��һ�۸����ϸ�1��ticks";
	const static char* BidPrice1PlusTwoTicks="��һ�۸����ϸ�2��ticks";
	const static char* BidPrice1PlusThreeTicks="��һ�۸����ϸ�3��ticks";

	switch(param)
	{
		case T_OPT_AnyPrice:		return AnyPrice;
		case T_OPT_LimitPrice:		return LimitPrice;
		case T_OPT_BestPrice:		return BestPrice;
		case T_OPT_LastPrice:		return LastPrice;
		case T_OPT_LastPricePlusOneTicks:		return LastPricePlusOneTicks;
		case T_OPT_LastPricePlusTwoTicks:		return LastPricePlusTwoTicks;
		case T_OPT_LastPricePlusThreeTicks:	return LastPricePlusThreeTicks;
		case T_OPT_AskPrice1:					return AskPrice1;
		case T_OPT_AskPrice1PlusOneTicks:		return AskPrice1PlusOneTicks;
		case T_OPT_AskPrice1PlusTwoTicks:		return AskPrice1PlusTwoTicks;
		case T_OPT_AskPrice1PlusThreeTicks:	return AskPrice1PlusThreeTicks;
		case T_OPT_BidPrice1:					return BidPrice1;
		case T_OPT_BidPrice1PlusOneTicks:		return BidPrice1PlusOneTicks;
		case T_OPT_BidPrice1PlusTwoTicks:		return BidPrice1PlusTwoTicks;
		case T_OPT_BidPrice1PlusThreeTicks:	return BidPrice1PlusThreeTicks;
		default: return __UNKOWN__;
	}
}

///Ͷ���ױ���־
const char* Client::CFieldDetail::HedgeFlag2String(char param)
{
	const static char* Speculation="Ͷ��";
	const static char* Hedge="�ױ�";
	switch(param)
	{
		case T_HF_Speculation:return Speculation;
		case T_HF_Hedge:return Hedge;
		default: return __UNKOWN__;
	}
}

///�ֲֶ�շ���
const char* Client::CFieldDetail::PosiDirection2String(char param)
{
	const static char* Net="��";
	const static char* Long="��ͷ";
	const static char* Short="��ͷ";
	switch(param)
	{
		case T_PD_Net:return Net;
		case T_PD_Long:return Long;
		case T_PD_Short:return Short;
		default: return __UNKOWN__;
	}
}

///ǿƽԭ��
const char* Client::CFieldDetail::ForceCloseReason2String(char param)
{
	const static char* NotForceClose="��ǿƽ";
	const static char* LackDeposit="�ʽ���";
	const static char* ClientOverPositionLimit="�ͻ�����";
	const static char* MemberOverPositionLimit="��Ա����";
	const static char* NotMultiple="�ֲַ�������";
	const static char* Violation="Υ��";
	const static char* Other="����";

	switch(param)
	{
		case T_FCC_NotForceClose:	return NotForceClose;
		case T_FCC_LackDeposit:	return LackDeposit;
		case T_FCC_ClientOverPositionLimit:	return ClientOverPositionLimit;
		case T_FCC_MemberOverPositionLimit:	return MemberOverPositionLimit;
		case T_FCC_NotMultiple:	return NotMultiple;
		case T_FCC_Violation:		return Violation;
		case T_FCC_Other:			return Other;
		default: return __UNKOWN__;
	}
}

///��������
const char* Client::CFieldDetail::OrderType2String(char param)
{
	const static char* Normal="����";
	const static char* DeriveFromQuote="��������";
	const static char* DeriveFromCombination="�������";
	const static char* Combination="��ϱ���";
	const static char* ConditionalOrder="������";
	const static char* Swap="������";

	switch(param)
	{
		case T_ORDT_Normal:			return Normal;
		case T_ORDT_DeriveFromQuote:	return DeriveFromQuote;
		case T_ORDT_DeriveFromCombination:	return DeriveFromCombination;
		case T_ORDT_Combination:		return Combination;
		case T_ORDT_ConditionalOrder:	return ConditionalOrder;
		case T_ORDT_Swap:				return Swap;
		default: return Normal;//return __UNKOWN__;
	}
}

///��Ч������
const char* Client::CFieldDetail::TimeCondition2String(char param)
{
	const static char* IOC="������ɣ�������";
	const static char* GFS="������Ч";
	const static char* GFD="������Ч";
	const static char* GTD="ָ������ǰ��Ч";
	const static char* GTC="����ǰ��Ч";
	const static char* GFA="���Ͼ�����Ч";

	switch(param)
	{
		case T_TC_IOC:		return IOC;
		case T_TC_GFS:		return GFS;
		case T_TC_GFD:		return GFD;
		case T_TC_GTD:		return GTD;
		case T_TC_GTC:		return GTC;
		case T_TC_GFA:		return GFA;
		default: return __UNKOWN__;
	}
}

///�ɽ�������
const char* Client::CFieldDetail::VolumeCondition2String(char param)
{
	const static char* AV="�κ�����";
	const static char* MV="��С����";
	const static char* CV="ȫ������";
	switch(param)
	{
		case T_VC_AV:return AV;
		case T_VC_MV:return MV;
		case T_VC_CV:return CV;
		default: return __UNKOWN__;
	}
}

///ҵ�����������ؽ��
const char* Client::CFieldDetail::ReturnCode2String(int param)
{
	const static char* code0="�ɹ�";
	const static char* code1="��������ʧ��";
	const static char* code2="δ�������󳬹������";
	const static char* code3="ÿ�뷢�����������������";
	const static char* code98="�ʺ��޽���Ȩ��";
	const static char* code99="��Ч�Ľ����ʺ�";
	const static char* code999="����ԭ��ʧ��";
	switch(param)
	{
		case 0:		return code0;
		case -1:	return code1;
		case -2:	return code2;
		case -3:	return code3;
		case -98:	return code98;
		case -99:	return code99;
		case -999:	return code999;
		default:	return __UNKOWN__;
	}
}

///Ʒ������
const char* Client::CFieldDetail::ProductType2String(char param)
{
	const static char* Futures="�ڻ�";
	const static char* Options="��Ȩ";
	const static char* Combination="���";
	const static char* Spot="����";
	const static char* EFP="��ת��";


	switch(param)
	{
		case T_PC_Futures:			return Futures;
		case T_PC_Options:	        return Options;
		case T_PC_Combination:  	return Combination;
		case T_PC_Spot:		    return Spot;
		case T_PC_EFP:	            return EFP;
		default: return __UNKOWN__;
	}
}



const char* Client::CFieldDetail::UserForceClose2String(int UserForceClose)
{
	const static char* YES="��";
	const static char* NO="��";
	switch( UserForceClose)
	{
		case 0:return NO;
		case 1:return YES;
		default:return __UNKOWN__;
	}
}

///�ɽ�����
const char* Client::CFieldDetail::TradeType2String(char TradeType)
{
	const static char* Common="��ͨ�ɽ�";
	const static char* OptionsExecution="��Ȩִ��";
	const static char* OTC="OTC�ɽ�";
	const static char* EFPDerived="��ת�������ɽ�";
	const static char* CombinationDerived="��������ɽ�";

	switch(TradeType)
	{
		case T_TRDT_Common:			return Common;
		case T_TRDT_OptionsExecution:	return OptionsExecution;
		case T_TRDT_OTC:  				return OTC;
		case T_TRDT_EFPDerived:		return EFPDerived;
		case T_TRDT_CombinationDerived:return CombinationDerived;
		case 0x00:								return Common;
		default: return __UNKOWN__;
	}
}

//���׽�ɫ
const char* Client::CFieldDetail::TraderRole2String(int param)
{
	const static char* TraderRoleBroker="����";
	const static char* TraderRoleHost="��Ӫ";
	const static char* TraderRoleMaker="������";
	switch(param)
	{
	case T_ER_Broker:			return TraderRoleBroker;
	case T_ER_Host:	        return TraderRoleHost;
	case T_ER_Maker:  		    return TraderRoleMaker;
	default: return __UNKOWN__;
	}
}


//�ɽ���Դ
const char* Client::CFieldDetail::TradeSourceString(int param)
{
	const static char* TradeSourceNormal="��������ͨ�ر�";
	const static char* TradeSourceQuery="��ѯ";	
	switch(param)
	{
	case T_TSRC_NORMAL :			return TradeSourceNormal;
	case T_TSRC_QUERY:	        return TradeSourceQuery;
	default: return __UNKOWN__;
	}
}


//�ɽ�����Դ
const char* Client::CFieldDetail::PriceType2String(int param)
{
	const static char* PriceTypeLast="ǰ�ɽ���";
	const static char* PriceTypeBuy="��ί�м�";
	const static char* PriceTypeSell="��ί�м�";
	switch(param)
	{
	case T_PSRC_LastPrice:			return PriceTypeLast;
	case T_PSRC_Buy:	        return PriceTypeBuy;
	case T_PSRC_Sell:  		    return PriceTypeSell;
	default: return __UNKOWN__;
	}
}

