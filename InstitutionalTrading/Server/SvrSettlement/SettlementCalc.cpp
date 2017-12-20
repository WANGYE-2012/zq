
#include "SettlementCalc.h"
const static char* __UNKOWN__="δ֪";
CSettlementCalc::CSettlementCalc(void)
{
}

CSettlementCalc::~CSettlementCalc(void)
{
}
//CF_ERROR CSettlementCalc::FundCalc(void)
//{
//return CF_ERROR_SUCCESS;
//}
//CF_ERROR CSettlementCalc::SaveData(void)
//{
//return CF_ERROR_SUCCESS;
//}
const string CSettlementCalc::ExchangeID2String(const string& ExchangeID)
{
	const string SHFE="������";
	const string CZCE="֣����";
	const string DCE="������";
	const string CFFEX="�н���";

	if(ExchangeID=="SHFE")
		return SHFE;
	else if(ExchangeID=="CZCE")
		return CZCE;
	else if(ExchangeID=="DCE")
		return DCE;
	else if(ExchangeID=="CFFEX")
		return CFFEX;
	else
		return __UNKOWN__;
}
const char* CSettlementCalc::TradeDirection2String(char param)
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
///Ͷ���ױ���־
const char* CSettlementCalc::HedgeFlag2String(char param)
{
	const static char* Speculation="Ͷ��";
	const static char* Arbitrage="����";
	const static char* Hedge="�ױ�";
	switch(param)
	{
	case THOST_FTDC_HF_Speculation:return Speculation;
	case THOST_FTDC_HF_Arbitrage:return Arbitrage;
	case THOST_FTDC_HF_Hedge:return Hedge;
	default: return __UNKOWN__;
	}
}
///��ƽ��־
const char* CSettlementCalc::OffsetFlag2String(char param)
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
const char* CSettlementCalc::Direction2String(char param)
{
	const static char* Net="��";
	const static char* Buy="��ͷ";
	const static char* Sell="��ͷ";
	switch(param)
	{
	case THOST_FTDC_PD_Net:return Net;
	case THOST_FTDC_PD_Long:return Buy;
	case THOST_FTDC_PD_Short:return Sell;
	default: return __UNKOWN__;
	}
}