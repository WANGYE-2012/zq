
#include "stdafx.h"
#include "CFieldDetail.h"
#include "commonstruct.h"
#include "CommonStructEx.h"


const static char* __UNKOWN__="δ֪";

///��������
const char* CltPresentation::CFieldDetail::Direction2String(char param)
{
	const static char* Buy="��";
	const static char* Sell="��";
	switch(param)
	{
		case THOST_FTDC_D_Buy:return Buy;
		case THOST_FTDC_D_Sell:return Sell;
		default: return __UNKOWN__;
	}
}

///��ƽ��־
const char* CltPresentation::CFieldDetail::OffsetFlag2String(char param)
{
	const static char* Open="����";
	const static char* Close="ƽ��";
	const static char* ForceClose="ǿƽ";
	const static char* CloseToday="ƽ��";
	const static char* CloseYesterday="ƽ��";
	switch(param)
	{
		case THOST_FTDC_OF_Open:		return Open;
		case THOST_FTDC_OF_Close:		return Close;
		case THOST_FTDC_OF_ForceClose:	return ForceClose;
		case THOST_FTDC_OF_CloseToday:	return CloseToday;
		case THOST_FTDC_OF_CloseYesterday:return CloseYesterday;
		default: return __UNKOWN__;
	}
}

///����״̬
const char* CltPresentation::CFieldDetail::OrderStatus2String(char param)
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
		case THOST_FTDC_OST_AllTraded:				return AllTraded;
		case THOST_FTDC_OST_PartTradedQueueing:		return PartTradedQueueing;
		case THOST_FTDC_OST_PartTradedNotQueueing:	return PartTradedNotQueueing;
		case THOST_FTDC_OST_NoTradeQueueing:		return NoTradeQueueing;
		case THOST_FTDC_OST_NoTradeNotQueueing:		return NoTradeNotQueueing;
		case THOST_FTDC_OST_Canceled:				return Canceled;
		case THOST_FTDC_OST_Unknown:				return Unknown;
		case THOST_FTDC_OST_NotTouched:				return NotTouched;
		case THOST_FTDC_OST_Touched:				return Touched;
        //case T_OST_ERROE:				    return Error;
		default: return __UNKOWN__;
	}
}

///�����ύ״̬
const char* CltPresentation::CFieldDetail::OrderSubmitStatus2String(char param)
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
		case THOST_FTDC_OSS_InsertSubmitted:	return InsertSubmitted;
		case THOST_FTDC_OSS_CancelSubmitted:	return CancelSubmitted;
		case THOST_FTDC_OSS_ModifySubmitted:	return ModifySubmitted;
		case THOST_FTDC_OSS_Accepted:			return Accepted;
		case THOST_FTDC_OSS_InsertRejected:		return InsertRejected;
		case THOST_FTDC_OSS_CancelRejected:		return CancelRejected;
		case THOST_FTDC_OSS_ModifyRejected:		return ModifyRejected;
		default: return __UNKOWN__;
	}
}

///�����۸�����
const char* CltPresentation::CFieldDetail::OrderPriceType2String(char param)
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
		case THOST_FTDC_OPT_AnyPrice:		return AnyPrice;
		case THOST_FTDC_OPT_LimitPrice:		return LimitPrice;
		case THOST_FTDC_OPT_BestPrice:		return BestPrice;
		case THOST_FTDC_OPT_LastPrice:		return LastPrice;
		case THOST_FTDC_OPT_LastPricePlusOneTicks:		return LastPricePlusOneTicks;
		case THOST_FTDC_OPT_LastPricePlusTwoTicks:		return LastPricePlusTwoTicks;
		case THOST_FTDC_OPT_LastPricePlusThreeTicks:	return LastPricePlusThreeTicks;
		case THOST_FTDC_OPT_AskPrice1:					return AskPrice1;
		case THOST_FTDC_OPT_AskPrice1PlusOneTicks:		return AskPrice1PlusOneTicks;
		case THOST_FTDC_OPT_AskPrice1PlusTwoTicks:		return AskPrice1PlusTwoTicks;
		case THOST_FTDC_OPT_AskPrice1PlusThreeTicks:	return AskPrice1PlusThreeTicks;
		case THOST_FTDC_OPT_BidPrice1:					return BidPrice1;
		case THOST_FTDC_OPT_BidPrice1PlusOneTicks:		return BidPrice1PlusOneTicks;
		case THOST_FTDC_OPT_BidPrice1PlusTwoTicks:		return BidPrice1PlusTwoTicks;
		case THOST_FTDC_OPT_BidPrice1PlusThreeTicks:	return BidPrice1PlusThreeTicks;
		default: return __UNKOWN__;
	}
}

///Ͷ���ױ���־
const char* CltPresentation::CFieldDetail::HedgeFlag2String(char param)
{
	const static char* Speculation="Ͷ��";
	const static char* Hedge="�ױ�";
	switch(param)
	{
		case THOST_FTDC_HF_Speculation:return Speculation;
		case THOST_FTDC_HF_Hedge:return Hedge;
		default: return __UNKOWN__;
	}
}

///�ֲֶ�շ���
const char* CltPresentation::CFieldDetail::PosiDirection2String(char param)
{
	const static char* Net="��";
	const static char* Long="��ͷ";
	const static char* Short="��ͷ";
	switch(param)
	{
		case THOST_FTDC_PD_Net:return Net;
		case THOST_FTDC_PD_Long:return Long;
		case THOST_FTDC_PD_Short:return Short;
		default: return __UNKOWN__;
	}
}

///ǿƽԭ��
const char* CltPresentation::CFieldDetail::ForceCloseReason2String(char param)
{
	const static char* NotForceClose="��ǿƽ";
	const static char* LackDeposit="�ʽ���";
	const static char* CltPresentationOverPositionLimit="�ͻ�����";
	const static char* MemberOverPositionLimit="��Ա����";
	const static char* NotMultiple="�ֲַ�������";
	const static char* Violation="Υ��";
	const static char* Other="����";

	switch(param)
	{
		case THOST_FTDC_FCC_NotForceClose:	return NotForceClose;
		case THOST_FTDC_FCC_LackDeposit:	return LackDeposit;
		case THOST_FTDC_FCC_ClientOverPositionLimit:	return CltPresentationOverPositionLimit;
		case THOST_FTDC_FCC_MemberOverPositionLimit:	return MemberOverPositionLimit;
		case THOST_FTDC_FCC_NotMultiple:	return NotMultiple;
		case THOST_FTDC_FCC_Violation:		return Violation;
		case THOST_FTDC_FCC_Other:			return Other;
		default: return __UNKOWN__;
	}
}

///��������
const char* CltPresentation::CFieldDetail::OrderType2String(char param)
{
	const static char* Normal="����";
	const static char* DeriveFromQuote="��������";
	const static char* DeriveFromCombination="�������";
	const static char* Combination="��ϱ���";
	const static char* ConditionalOrder="������";
	const static char* Swap="������";

	switch(param)
	{
		case THOST_FTDC_ORDT_Normal:			return Normal;
		case THOST_FTDC_ORDT_DeriveFromQuote:	return DeriveFromQuote;
		case THOST_FTDC_ORDT_DeriveFromCombination:	return DeriveFromCombination;
		case THOST_FTDC_ORDT_Combination:		return Combination;
		case THOST_FTDC_ORDT_ConditionalOrder:	return ConditionalOrder;
		case THOST_FTDC_ORDT_Swap:				return Swap;
		default: return Normal;//return __UNKOWN__;
	}
}

///��Ч������
const char* CltPresentation::CFieldDetail::TimeCondition2String(char param)
{
	const static char* IOC="������ɣ�������";
	const static char* GFS="������Ч";
	const static char* GFD="������Ч";
	const static char* GTD="ָ������ǰ��Ч";
	const static char* GTC="����ǰ��Ч";
	const static char* GFA="���Ͼ�����Ч";

	switch(param)
	{
		case THOST_FTDC_TC_IOC:		return IOC;
		case THOST_FTDC_TC_GFS:		return GFS;
		case THOST_FTDC_TC_GFD:		return GFD;
		case THOST_FTDC_TC_GTD:		return GTD;
		case THOST_FTDC_TC_GTC:		return GTC;
		case THOST_FTDC_TC_GFA:		return GFA;
		default: return __UNKOWN__;
	}
}

///�ɽ�������
const char* CltPresentation::CFieldDetail::VolumeCondition2String(char param)
{
	const static char* AV="�κ�����";
	const static char* MV="��С����";
	const static char* CV="ȫ������";
	switch(param)
	{
		case THOST_FTDC_VC_AV:return AV;
		case THOST_FTDC_VC_MV:return MV;
		case THOST_FTDC_VC_CV:return CV;
		default: return __UNKOWN__;
	}
}

///ҵ�����������ؽ��
const char* CltPresentation::CFieldDetail::ReturnCode2String(int param)
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
const char* CltPresentation::CFieldDetail::ProductType2String(char param)
{
	const static char* Futures="�ڻ�";
	const static char* Options="��Ȩ";
	const static char* Combination="���";
	const static char* Spot="����";
	const static char* EFP="��ת��";


	switch(param)
	{
		case THOST_FTDC_PC_Futures:			return Futures;
		case THOST_FTDC_PC_Options:	        return Options;
		case THOST_FTDC_PC_Combination:  	return Combination;
		case THOST_FTDC_PC_Spot:		    return Spot;
		case THOST_FTDC_PC_EFP:	            return EFP;
		default: return __UNKOWN__;
	}
}



const char* CltPresentation::CFieldDetail::UserForceClose2String(int UserForceClose)
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
const char* CltPresentation::CFieldDetail::TradeType2String(char TradeType)
{
	const static char* Common="��ͨ�ɽ�";
	const static char* OptionsExecution="��Ȩִ��";
	const static char* OTC="OTC�ɽ�";
	const static char* EFPDerived="��ת�������ɽ�";
	const static char* CombinationDerived="��������ɽ�";

	switch(TradeType)
	{
		case THOST_FTDC_TRDT_Common:			return Common;
		case THOST_FTDC_TRDT_OptionsExecution:	return OptionsExecution;
		case THOST_FTDC_TRDT_OTC:  				return OTC;
		case THOST_FTDC_TRDT_EFPDerived:		return EFPDerived;
		case THOST_FTDC_TRDT_CombinationDerived:return CombinationDerived;
		case 0x00:								return Common;
		default: return __UNKOWN__;
	}
}

//���׽�ɫ
const char* CltPresentation::CFieldDetail::TraderRole2String(int param)
{
	const static char* TraderRoleBroker="����";
	const static char* TraderRoleHost="��Ӫ";
	const static char* TraderRoleMaker="������";
	switch(param)
	{
	case THOST_FTDC_ER_Broker:			return TraderRoleBroker;
	case THOST_FTDC_ER_Host:	        return TraderRoleHost;
	case THOST_FTDC_ER_Maker:  		    return TraderRoleMaker;
	default: return __UNKOWN__;
	}
}


//�ɽ���Դ
const char* CltPresentation::CFieldDetail::TradeSourceString(int param)
{
	const static char* TradeSourceNormal="��������ͨ�ر�";
	const static char* TradeSourceQuery="��ѯ";	
	switch(param)
	{
	case THOST_FTDC_TSRC_NORMAL :			return TradeSourceNormal;
	case THOST_FTDC_TSRC_QUERY:	        return TradeSourceQuery;
	default: return __UNKOWN__;
	}
}


//�ɽ�����Դ
const char* CltPresentation::CFieldDetail::PriceType2String(int param)
{
	const static char* PriceTypeLast="ǰ�ɽ���";
	const static char* PriceTypeBuy="��ί�м�";
	const static char* PriceTypeSell="��ί�м�";
	switch(param)
	{
	case THOST_FTDC_PSRC_LastPrice:			return PriceTypeLast;
	case THOST_FTDC_PSRC_Buy:	        return PriceTypeBuy;
	case THOST_FTDC_PSRC_Sell:  		    return PriceTypeSell;
	default: return __UNKOWN__;
	}
}

