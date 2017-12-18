#pragma once
#include "CommPublic.h"
#include "KernelStruct.h"
class StrategyDecisionMaking
{
public:
	StrategyDecisionMaking(void);
	~StrategyDecisionMaking(void);

public:
	//*************************************************************************************************
	//	��ȡ���Խ��鱨��
	//	strategyID:					                [in]	    ����ID
	//	strUnderlyingInstrumentID: 	            	[in]	    ������ԼID
	//  vector<PlatformStru_InstrumentInfo>         [in]        ��ǰ������Ȩ�ڻ���Լ
	//	map<string,PlatformStru_DepthMarketData>    [in]        ��ǰ������Ȩ�ڻ�������
	//	outOrders:					               [out]	    ���鱨���б�
	//*************************************************************************************************
	bool GetOptionStrategyOrders(eStrategyID     strategyID,
								 const string      strUnderlyingInstrumentID,
		vector<PlatformStru_InstrumentInfo>  vecOptionIns,
		map<string,PlatformStru_DepthMarketData>   maplastQuot,

		vector<PlatformStru_OrderInfo>&      outOrders);

private:
	void GetMiniPriceIns(vector<PlatformStru_InstrumentInfo>& vecTempIns,PlatformStru_InstrumentInfo& outData);
	void GetMaxPriceIns(vector<PlatformStru_InstrumentInfo>& vecTempIns,PlatformStru_InstrumentInfo& outData);

};
