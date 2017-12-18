#pragma once

#include <string>
#include <vector>
#include "../Module-Misc2/KernelStruct.h"

using namespace std;


enum {	conMaxRltCodeNum = 32 };
enum 
{	
    conOCUnknow=-1, 
    conOCOpen=0, 
    conOCClodeToday=1, 
    conOCClose=2 
};

typedef struct {
	int	 nCmbCodeCount;
	char cType;
	char strCodeName[24];	// ��Լ���
	char strCodeNameJP[24];	// ��Լ��Ƶ�ƴ������ĸ
	char strCodeHeads[conMaxRltCodeNum][24];	// ��Լ����ͷ
} CODEHEADDATA, *PCODEHEADDATA;


size_t SplitString(const string strInput,vector<string>& outData,const char separator=',',bool bTailAppend=true);
int GetNumDigits(double dbTick);
string Number2String(int nData);
// �۸���ʾת������������һ���۸��ַ���
// dbData, �۸�
// dbTick����С�䶯��λ
string Price2String(double dbData, double dbTick);
//������ת��Ϊ�ַ���������С��λ����=��С�䶯��λС��λ+1����С����λ
string AvgPrice2String(double dbData, double dbTick);
string Double2String(double dbData);

BOOL CheckSumPrice(double& fPrice, double fPriceTick, int nStep=0);
double AnalysePrice(double fPrice, double fUpperLimitPrice, double fLowerLimitPrice);

BOOL ConvertComboInstrumentID(const string& strComboID, string& strInstrumentID1, string& strInstrumentID2);

BOOL IsMarket(const string& strPrice);

BOOL MakeReq_Order(PlatformStru_InputOrder& req, string strInstrumentID, string strAccount, 
				   int nOrderType, BOOL bDirection, int nOCMode, int nVolume, 
				   double fPrice, double fTriggerPrice, int nHedgeStatus, BOOL bIsMarketPrice, 
				   BOOL bIsReplaceOrder, int minVolume=1);

BOOL MakeReq_Parked(PlatformStru_ParkedOrder& req, string strInstrumentID, string strAccount, 
					int nOrderType, BOOL bDirection, int nOCMode, int nVolume, 
					double fPrice, double fTriggerPrice, int nHedgeStatus, BOOL bIsMarketPrice, 
					BOOL bIsReplaceOrder, int minVolume=1);

string Printf_StdOrderReq(const PlatformStru_InputOrder& req, string strAccountTitle, 
						  string strOrderTypeTitle, string strInstrumentIDTitle, string strDirectionTitle, 
						  string strOpenCloseTitle, string strQtyTitle, string strPriceTitle, 
						  string strTriggerPriceTitle, string strHedgeTitle, double fPriceTick=-1.0, 
						  BOOL bShowAccountItem=FALSE, BOOL bShowOpenCloseItem=TRUE, BOOL bShowHedgeItem=TRUE);

char* GetProductSNameItem(char* csProductSName, CODEHEADDATA& codeHead, BOOL& bRetHasData);
BOOL GetCodesItem(char* csCodes, CODEHEADDATA& codeHead);

bool InitOwner(std::string& strAccount, std::string& strTradingDay);
bool AppendOwner(int nFrontID, int nSessionID);
bool HasOwner(int nFrontID, int nSessionID);
bool HasOwnerTrade(PlatformStru_TradeInfo& trade);

int VerifyOrder(PlatformStru_InputOrder& req);
int VerifyOrder(PlatformStru_ParkedOrder& req);

//strName as std::string
//bIsBuy as BOOL
//strPriceText as std::string
//bIsMarketPrice as BOOL
//fPrice as double
#define AUTOMARKETPRICE(strName, bIsBuy, strPriceText, bIsMarketPrice, fPrice) \
	{\
		if(stricmp(strPriceText.c_str(), LOADSTRING(OLS_MARKET))==0) \
        {\
			PlatformStru_InstrumentInfo contInfo;\
			memset(&contInfo,0,sizeof(contInfo));\
			IPlatformSingleSvr* pSvr=DEFAULT_SVR();\
			if(pSvr!=NULL) \
            {\
				pSvr->GetInstrumentInfo(strName.c_str(), contInfo);\
			    if(NEEDSIMULATEMARKET(contInfo.ExchangeID)) \
                {\
				    bIsMarketPrice = FALSE;\
				    PlatformStru_DepthMarketData depthMarket;\
				    memset(&depthMarket,0,sizeof(depthMarket));\
				    pSvr->GetQuotInfo(strName.c_str(), depthMarket);\
				    if(bIsBuy)  fPrice = depthMarket.UpperLimitPrice;\
				    else  fPrice = depthMarket.LowerLimitPrice;\
				}\
			    else  bIsMarketPrice = TRUE;\
			}\
		}\
		else \
        {\
			bIsMarketPrice=FALSE;\
			fPrice=atof(strPriceText.c_str());\
		}\
	}

//strName as std::string
//bIsBuy as BOOL
//bIsMarketPrice as BOOL
//fPrice as double
#define AUTOMARKETPRICE2(strName, bIsBuy, bIsMarketPrice, fPrice) \
	{\
		if(bIsMarketPrice) \
        {\
			PlatformStru_InstrumentInfo contInfo;\
			memset(&contInfo,0,sizeof(contInfo));\
			if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(strName.c_str(), contInfo);\
			IPlatformSingleSvr* pSvr=DEFAULT_SVR();\
			if(pSvr!=NULL&&NEEDSIMULATEMARKET(contInfo.ExchangeID)) \
            {\
				bIsMarketPrice = FALSE;\
				PlatformStru_DepthMarketData depthMarket;\
				memset(&depthMarket,0,sizeof(depthMarket));\
				pSvr->GetQuotInfo(strName.c_str(), depthMarket);\
				if(bIsBuy) fPrice = depthMarket.UpperLimitPrice;\
				else fPrice = depthMarket.LowerLimitPrice;\
			}\
		}\
	}
