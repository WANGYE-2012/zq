#pragma once
#include "Interface_SvrSettlement.h"
class CSettlementCalc
{
public:
	CSettlementCalc(void);
	~CSettlementCalc(void);
 //   /*�����ʽ����*/
	//CF_ERROR FundCalc(void);
	///*�����������*/
	//CF_ERROR SaveData(void);
	const string ExchangeID2String(const string& ExchangeID);
	const char* TradeDirection2String(char param);
	const char* HedgeFlag2String(char param);
	const char* OffsetFlag2String(char param);
	const char* Direction2String(char param);
};
