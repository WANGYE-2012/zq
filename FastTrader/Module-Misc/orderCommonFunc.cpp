
#include "stdafx.h"
#include "constants.h"
#include "orderDefines.h"
#include "orderCommonFunc.h"
#include "COwnerManager.h"
#include "../inc/Module-Misc2/tools_util.h"
#include "FieldValueTool.h"
#include "../inc/Module-Misc2/globalDefines.h"
#include "..\inc\cfgmgr\CfgMgr.h"
#include "../inc/Plusin/Plusin.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "Module-Misc2/GlobalConfigMgr.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
extern CPlusin g_Plusin;
size_t SplitString(const string strInput,vector<string>& outData,const char separator,bool bTailAppend)
{
    outData.clear();
    if (strInput.empty())
        return 0;
    std::string strSrc=strInput;
    size_t pos=0,count=0;
    while(1)
    {
        count=strSrc.find(separator,0);
        if(count!=std::string::npos)
            outData.push_back(strSrc.substr(0,count));
        else
        {
            if(bTailAppend)
                outData.push_back(strSrc);
            break;
        }
        strSrc=strSrc.substr(count+1);
    }
    return outData.size();
}
int GetNumDigits(double dbTick)
{
	int pos,nNumDigits=0;
	char strPriceText[512]={0};
	ZeroMemory(strPriceText, sizeof(strPriceText));
	sprintf_s(strPriceText,512, "%f", dbTick);
	for(pos=strlen(strPriceText)-1;pos>=0;pos--)
	{
		if(strPriceText[pos]=='0')
			strPriceText[pos]=0;
		else
			break;
	}
	for(pos=strlen(strPriceText)-1;pos>=0;pos--)
	{
		if(strPriceText[pos]!='.')
			nNumDigits++;
		else
			break;
	}
	return nNumDigits;

}

string Number2String(int nData)
{
	char str[512]="-";
    if(nData!=-1&&!util::isInvalidValue(nData)) sprintf(str, "%d", nData);
	return str;
}
string Price2String(double dbData, double dbTick)
{
	char strPriceText[512]={0},strFormatText[512]={0};
	ZeroMemory(strPriceText, sizeof(strPriceText));
	ZeroMemory(strFormatText, sizeof(strFormatText));
	strcpy(strPriceText, "-");
	if(util::isInvalidValue(dbData)||util::isInvalidValue(dbTick)) 
		return strPriceText;
	sprintf(strFormatText,"%%0.%df", GetNumDigits(dbTick));
	sprintf(strPriceText, strFormatText, dbData);
	return strPriceText;
}
//������ת��Ϊ�ַ���������С��λ����=��С�䶯��λС��λ+1����С����λ
string AvgPrice2String(double dbData, double dbTick)
{
	char strPriceText[512]={0},strFormatText[512]={0};
	ZeroMemory(strPriceText, sizeof(strPriceText));
	ZeroMemory(strFormatText, sizeof(strFormatText));
	strcpy(strPriceText, "-");
	if(util::isInvalidValue(dbData)||util::isInvalidValue(dbTick)) 
		return strPriceText;
    int DigitsNum=GetNumDigits(dbTick)+1;
    if(DigitsNum<2) DigitsNum=2;
	sprintf(strFormatText,"%%0.%df", DigitsNum);
	sprintf(strPriceText, strFormatText, dbData);
	return strPriceText;
}
string Double2String(double dbData)
{
	char strPriceText[512]={0};
	ZeroMemory(strPriceText, sizeof(strPriceText));
	strcpy(strPriceText, "-");
	if(util::isInvalidValue(dbData)) 
		return strPriceText;
	sprintf(strPriceText, "%f", dbData);
	for(int i=strlen(strPriceText)-1; i>=0; i--) 
	{
		if(strPriceText[i]=='0')
			strPriceText[i]=0;
		else
		{
			if(strPriceText[i]=='.')
				strPriceText[i]=0;
			break;
		}
	}
	return strPriceText;
}

BOOL CheckSumPrice(double& fPrice, double fPriceTick, int nStep)
{
	if(fPriceTick < 0.000001)
		return FALSE;

	double fNewPrice = fPrice;
	long nNewPrice = 0;

	if(nStep==0) {
		if(fNewPrice>0.0)
			nNewPrice=(long)((fPrice+fPriceTick/2.0+0.0000000009)/fPriceTick);
		else
			nNewPrice=(long)((fPrice-fPriceTick/2.0-0.0000000009)/fPriceTick);
		fNewPrice=(double)nNewPrice*fPriceTick;
	}
	else {
		if(fNewPrice>0.0)
			nNewPrice=(long)(fNewPrice/fPriceTick+0.0000000009);
		else
			nNewPrice=(long)(fNewPrice/fPriceTick-0.0000000009);
		fNewPrice=(double)nNewPrice*fPriceTick;
		if(abs(fPrice - fNewPrice)<0.0000000009) {
			if(nStep>0) {
				fNewPrice+=fPriceTick;
			}
			else {
				fNewPrice-=fPriceTick;
			}
		}
		else {
			if(nStep>0 && fPrice>0.0) {
				fNewPrice+=fPriceTick;
			}
			else if(nStep<0 && fPrice<0.0) {
				fNewPrice-=fPriceTick;
			}
		}
	}

	BOOL bIsEqual = abs(fPrice - fNewPrice)<0.0000000009;
	fPrice = fNewPrice;
	return !bIsEqual;
}

double AnalysePrice(double fPrice, double fUpperLimitPrice, double fLowerLimitPrice)
{
	if(util::isInvalidValue(fPrice))
		return 0.0;
	else if(fUpperLimitPrice==0.0 
			&& fLowerLimitPrice==0.0)
		return fPrice;
	else if(util::isInvalidValue(fUpperLimitPrice) 
			|| util::isInvalidValue(fLowerLimitPrice)) 
		return fPrice;
	else if(fPrice>fUpperLimitPrice) 
		return fUpperLimitPrice;
	else if(fPrice<fLowerLimitPrice)
		return fLowerLimitPrice;
	else 
		return fPrice;
}

BOOL ConvertComboInstrumentID(const string& strComboID, string& strInstrumentID1, string& strInstrumentID2)
{
	char strTemp[128];
	char strCode1[128];
	char strCode2[128];
	ZeroMemory(strCode1, 128);
	ZeroMemory(strCode2, 128);
	sscanf_s(strComboID.c_str(), "%s %[^&]&%s", strTemp, sizeof(strTemp)-1, 
			strCode1, sizeof(strCode1)-1, strCode2, sizeof(strCode2)-1);
	strInstrumentID1 = strCode1;
	strInstrumentID2 = strCode2;
//	vector<string> outData;
//	if(SplitString(strText, outData)<3) 
//		return FALSE;
//	strInstrumentID1 = outData[0];
//	strInstrumentID2 = outData[1];
	return TRUE;
}

BOOL IsMarket(const string& strPrice)
{
	return stricmp(strPrice.c_str(), LOADSTRING(OLS_MARKET))==0;
}


BOOL MakeReq_Order(PlatformStru_InputOrder& req, string strInstrumentID, string strAccount, 
				   int nOrderType, BOOL bDirection, int nOCMode, int nVolume, 
				   double fPrice, double fTriggerPrice, int nHedgeStatus, BOOL bIsMarketPrice, 
				   BOOL bIsReplaceOrder, int minVolume)
{
	// ����Ϊ���̸ĵ�����
	if(bIsReplaceOrder)
		req.OrderClass=ORDERCLASS_ReplaceOrder;
	else
		req.OrderClass=ORDERCLASS_NewOrder;

	switch(nOrderType) {
	case OrderType_FT_Touch:
		req.ContingentCondition = THOST_FTDC_CC_Touch;				// ֹ��
		req.StopPrice = fTriggerPrice;								// ͬʱдֹ�𴥷���
		break;
	case OrderType_FT_TouchProfit:
		req.ContingentCondition = THOST_FTDC_CC_TouchProfit;		// ֹӮ
		req.StopPrice = fTriggerPrice;								// ͬʱдֹӯ������
		break;
	case OrderType_FT_ParkedOrder:
		req.ContingentCondition = THOST_FTDC_CC_ParkedOrder;		// Ԥ��
		break;
	case OrderType_FT_LastPriceGreaterThanStopPrice:
		///���¼۴���������
		req.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterThanStopPrice;
		req.StopPrice = fTriggerPrice;								// ͬʱд������
		break;
	case OrderType_FT_LastPriceGreaterEqualStopPrice:
		///���¼۴��ڵ���������
		req.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterEqualStopPrice;
		req.StopPrice = fTriggerPrice;								// ͬʱд������
		break;
	case OrderType_FT_LastPriceLesserThanStopPrice:
		///���¼�С��������
		req.ContingentCondition =  THOST_FTDC_CC_LastPriceLesserThanStopPrice;
		req.StopPrice = fTriggerPrice;								// ͬʱд������
		break;
	case OrderType_FT_LastPriceLesserEqualStopPrice:
		///���¼�С�ڵ���������
		req.ContingentCondition = THOST_FTDC_CC_LastPriceLesserEqualStopPrice;
		req.StopPrice = fTriggerPrice;								// ͬʱд������
		break;
	case OrderType_FT_AskPriceGreaterThanStopPrice:
		///��һ�۴���������
		req.ContingentCondition = THOST_FTDC_CC_AskPriceGreaterThanStopPrice;
		req.StopPrice = fTriggerPrice;								// ͬʱд������
		break;
	case OrderType_FT_AskPriceGreaterEqualStopPrice:
		///��һ�۴��ڵ���������
		req.ContingentCondition = THOST_FTDC_CC_AskPriceGreaterEqualStopPrice;
		req.StopPrice = fTriggerPrice;								// ͬʱд������
		break;
	case OrderType_FT_AskPriceLesserThanStopPrice:
		///��һ��С��������
		req.ContingentCondition = THOST_FTDC_CC_AskPriceLesserThanStopPrice;
		req.StopPrice = fTriggerPrice;								// ͬʱд������
		break;
	case OrderType_FT_AskPriceLesserEqualStopPrice:
		///��һ��С�ڵ���������
		req.ContingentCondition = THOST_FTDC_CC_AskPriceLesserEqualStopPrice;
		req.StopPrice = fTriggerPrice;								// ͬʱд������
		break;
	case OrderType_FT_BidPriceGreaterThanStopPrice:
		///��һ�۴���������
		req.ContingentCondition = THOST_FTDC_CC_BidPriceGreaterThanStopPrice;
		req.StopPrice = fTriggerPrice;								// ͬʱд������
		break;
	case OrderType_FT_BidPriceGreaterEqualStopPrice:
		///��һ�۴��ڵ���������
		req.ContingentCondition = THOST_FTDC_CC_BidPriceGreaterEqualStopPrice;
		req.StopPrice = fTriggerPrice;								// ͬʱд������
		break;
	case OrderType_FT_BidPriceLesserThanStopPrice:
		///��һ��С��������
		req.ContingentCondition = THOST_FTDC_CC_BidPriceLesserThanStopPrice;
		req.StopPrice = fTriggerPrice;								// ͬʱд������
		break;
	case OrderType_FT_BidPriceLesserEqualStopPrice:
		///��һ��С�ڵ���������
		req.ContingentCondition = THOST_FTDC_CC_BidPriceLesserEqualStopPrice;
		req.StopPrice = fTriggerPrice;								// ͬʱд������
		break;
	case OrderType_FT_Immediately:
	case OrderType_FT_ImmeFAK:							// ����FAK
	case OrderType_FT_ImmeFOK:							// ����FOK
	default:
		req.ContingentCondition = THOST_FTDC_CC_Immediately;		// ����ģʽ
		break;
	};

	// �����ʺ�
	strncpy(req.strAccount, strAccount.c_str(), 
			sizeof(req.strAccount)-1 < strAccount.size() ? 
			sizeof(req.strAccount)-1 : strAccount.size());

	//��Լ����
	strncpy(req.InstrumentID, strInstrumentID.c_str(), 
			sizeof(req.InstrumentID)-1 < strInstrumentID.size() ? 
				sizeof(req.InstrumentID)-1 : strInstrumentID.size());

	//��������
	req.Direction = (bDirection ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell);

	//if(bIsMarketPrice && !(nOrderType==OrderType_FT_ImmeFAK || nOrderType==OrderType_FT_ImmeFOK)) {
	if(bIsMarketPrice) {
		//�۸�����
		req.OrderPriceType=THOST_FTDC_OPT_AnyPrice;
		//�۸�
		req.LimitPrice=0;
		//��Ч������
		req.TimeCondition=THOST_FTDC_TC_IOC;
	}
	else {
		//�۸�����
		req.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
		//�޼ۼ۸�
		req.LimitPrice=fPrice;
		//��Ч������
		if(nOrderType==OrderType_FT_ImmeFAK || nOrderType==OrderType_FT_ImmeFOK)
			req.TimeCondition=THOST_FTDC_TC_IOC;
		else
			req.TimeCondition=THOST_FTDC_TC_GFD;
	}

	//��ƽ��־
	switch(nOCMode) {
	case conOCUnknow:
		req.CombOffsetFlag[0]='\000';					// δ֪
		break;
	case conOCOpen:
		req.CombOffsetFlag[0]=THOST_FTDC_OF_Open;		// ����
		break;
	case conOCClodeToday: 
		req.CombOffsetFlag[0]=THOST_FTDC_OF_CloseToday;	//ƽ��
		break;
	case conOCClose:
		req.CombOffsetFlag[0]=THOST_FTDC_OF_Close;		//ƽ��
		break;
	};

	//���Ͷ���ױ���־��ֻ�е�һ���ֽ���Ч
	switch(nHedgeStatus) {
	case 0:
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;	// Ͷ��
		break;
	case 1:
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Arbitrage;	// ����
		break;
	case 2:
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Hedge;		// ��ֵ
		break;
	};

	//����
	req.VolumeTotalOriginal=nVolume;
	//�ɽ�������
	//if(minVolume<1)
	//	minVolume=1;
	if(nOrderType==OrderType_FT_ImmeFOK)
		req.VolumeCondition = THOST_FTDC_VC_CV; 
	else if(nOrderType==OrderType_FT_ImmeFAK && minVolume > 1)
		req.VolumeCondition = THOST_FTDC_VC_MV; 
	else
		req.VolumeCondition = THOST_FTDC_VC_AV; 
	
	//��С�ɽ���
	if(nOrderType==OrderType_FT_ImmeFAK)
		req.MinVolume=minVolume;
	else
		req.MinVolume=1;

	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;

	return TRUE;
}

BOOL MakeReq_Parked(PlatformStru_ParkedOrder& req, string strInstrumentID, string strAccount, 
				   int nOrderType, BOOL bDirection, int nOCMode, int nVolume, 
				   double fPrice, double fTriggerPrice, int nHedgeStatus, BOOL bIsMarketPrice, 
				   BOOL bIsReplaceOrder, int minVolume)
{
	// ����Ϊ���̸ĵ�����
	if(bIsReplaceOrder)
		req.OrderClass=ORDERCLASS_ReplaceOrder;
	else
		req.OrderClass=ORDERCLASS_NewOrder;

	//�����ʺ�
	strncpy(req.strAccount, strAccount.c_str(), 
			sizeof(req.strAccount)-1<strAccount.size() ? sizeof(req.strAccount)-1 : strAccount.size());

	req.ContingentCondition = THOST_FTDC_CC_Immediately;		// ����ģʽ

	// �����ʺ�
	strncpy(req.strAccount, strAccount.c_str(), 
			sizeof(req.strAccount)-1 < strAccount.size() ? 
			sizeof(req.strAccount)-1 : strAccount.size());

	//��Լ����
	strncpy(req.InstrumentID, strInstrumentID.c_str(), 
			sizeof(req.InstrumentID)-1 < strInstrumentID.size() ? 
				sizeof(req.InstrumentID)-1 : strInstrumentID.size());

	//��������
	req.Direction = (bDirection ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell);

	//if(bIsMarketPrice && !(nOrderType==OrderType_FT_ImmeFAK || nOrderType==OrderType_FT_ImmeFOK)) {
	if(bIsMarketPrice) {
		//�۸�����
		req.OrderPriceType=THOST_FTDC_OPT_AnyPrice;
		//�۸�
		req.LimitPrice=0;
		//��Ч������
		req.TimeCondition=THOST_FTDC_TC_IOC;
	}
	else {
		//�۸�����
		req.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
		//�޼ۼ۸�
		req.LimitPrice=fPrice;
		//��Ч������
		if(nOrderType==OrderType_FT_ImmeFAK || nOrderType==OrderType_FT_ImmeFOK)
			req.TimeCondition=THOST_FTDC_TC_IOC;
		else
			req.TimeCondition=THOST_FTDC_TC_GFD;
	}

	//��ƽ��־
	switch(nOCMode) {
	case conOCUnknow:
		req.CombOffsetFlag[0]='\000';					// Ϊֹ
		break;
	case conOCOpen:
		req.CombOffsetFlag[0]=THOST_FTDC_OF_Open;		// ����
		break;
	case conOCClodeToday: 
		req.CombOffsetFlag[0]=THOST_FTDC_OF_CloseToday;	//ƽ��
		break;
	case conOCClose:
		req.CombOffsetFlag[0]=THOST_FTDC_OF_Close;		//ƽ��
		break;
	};

	//���Ͷ���ױ���־��ֻ�е�һ���ֽ���Ч
	switch(nHedgeStatus) {
	case 0:
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;	// Ͷ��
		break;
	case 1:
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Arbitrage;	// ����
		break;
	case 2:
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Hedge;		// ��ֵ
		break;
	};

	//����
	req.VolumeTotalOriginal=nVolume;
	//�ɽ�������
//	if(minVolume<1)
//		minVolume=1;
	if(nOrderType==OrderType_FT_ImmeFOK)
		req.VolumeCondition = THOST_FTDC_VC_CV; 
	else if(nOrderType==OrderType_FT_ImmeFAK && minVolume > 1)
		req.VolumeCondition = THOST_FTDC_VC_MV; 
	else
		req.VolumeCondition = THOST_FTDC_VC_AV; 
	
	//��С�ɽ���
	if(nOrderType==OrderType_FT_ImmeFAK)
		req.MinVolume=minVolume;
	else
		req.MinVolume=1;

	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;

	return TRUE;
}

string Printf_StdOrderReq(const PlatformStru_InputOrder& req, string strAccountTitle, 
						  string strOrderTypeTitle, string strInstrumentIDTitle, string strDirectionTitle, 
						  string strOpenCloseTitle, string strQtyTitle, string strPriceTitle, 
						  string strTriggerPriceTitle, string strHedgeTitle, double fPriceTick, 
						  BOOL bShowAccountItem, BOOL bShowOpenCloseItem, BOOL bShowHedgeItem)
{
	string strRet;
	string strOrderType;
	BOOL bShowPrice = FALSE;
	BOOL bShowTriggerPrice = FALSE;
	char strText[256];

	ZeroMemory(strText, sizeof(strText));
	if(req.ContingentCondition==THOST_FTDC_CC_Immediately && req.OrderPriceType==THOST_FTDC_OPT_AnyPrice) {
		bShowPrice = FALSE;
		bShowTriggerPrice = FALSE;
		strOrderType = LOADSTRING(ORDER_MARKET_TYPE);
	}
	else if(req.ContingentCondition==THOST_FTDC_CC_Immediately && req.OrderPriceType==THOST_FTDC_OPT_LimitPrice) {
		bShowPrice = TRUE;
		bShowTriggerPrice = FALSE;
		strOrderType = LOADSTRING(ORDER_LIMIT_TYPE);
	}
	else if(req.ContingentCondition==THOST_FTDC_CC_Touch && req.OrderPriceType==THOST_FTDC_OPT_AnyPrice) {
		bShowPrice = FALSE;
		bShowTriggerPrice = TRUE;
		strOrderType = LOADSTRING(ORDER_STOP_TYPE);
	}
	else if(req.ContingentCondition==THOST_FTDC_CC_Touch && req.OrderPriceType==THOST_FTDC_OPT_LimitPrice) {
		bShowPrice = TRUE;
		bShowTriggerPrice = TRUE;
		strOrderType = LOADSTRING(ORDER_STOPLIMIT_TYPE);
	}
	
	// �µ�����
	sprintf_s(strText, sizeof(strText), "%s: %s\n\000", strOrderTypeTitle.c_str(), strOrderType.c_str());
	strRet+=strText;
	// �ʺ�
	sprintf_s(strText, sizeof(strText), " %s: %s,\000", strAccountTitle.c_str(), req.strAccount);
	strRet+=strText;
	// ��Լ
	sprintf_s(strText, sizeof(strText), " %s: %s,\000", strInstrumentIDTitle.c_str(), req.InstrumentID);
	strRet+=strText;
	// ����
	sprintf_s(strText, sizeof(strText), " %s: %s,\000", strDirectionTitle.c_str(), 
			CFieldValueTool::Direction2String(req.Direction,SVR_LANGUAGE).c_str());
	strRet+=strText;
	// ��ƽ
	if(bShowOpenCloseItem) {
		sprintf_s(strText, sizeof(strText), " %s: %s,\000", strOpenCloseTitle.c_str(), 
				CFieldValueTool::OffsetFlag2String(req.CombOffsetFlag[0], SVR_LANGUAGE).c_str());
		strRet+=strText;
	}
	// �ױ�
	if(bShowHedgeItem) {
		sprintf_s(strText, sizeof(strText), " %s: %s,\000", strHedgeTitle.c_str(), 
				CFieldValueTool::HedgeFlag2String(req.CombHedgeFlag[0], SVR_LANGUAGE).c_str());
		strRet+=strText;
	}
	// ������
	if(bShowTriggerPrice) {
		sprintf_s(strText, sizeof(strText), " %s: %s,\000", strTriggerPriceTitle.c_str(), 
				Price2String(req.StopPrice, fPriceTick).c_str());
		strRet+=strText;
	}
	// �۸�
	if(bShowPrice) {
		sprintf_s(strText, sizeof(strText), " %s: %s,\000", strPriceTitle.c_str(), 
				Price2String(req.LimitPrice, fPriceTick).c_str());
		strRet+=strText;
	}
	// ����
	sprintf_s(strText, sizeof(strText), " %s: %s,\000", strQtyTitle.c_str(), 
			Number2String(req.VolumeTotalOriginal).c_str());
	strRet+=strText;
	strRet+=wxT("\n");
	return strRet;
}

char* GetProductSNameItem(char* csProductSName, CODEHEADDATA& codeHead, BOOL& bRetHasData)
{
	if(csProductSName==NULL)
		return NULL;

	int nItemLen = 0;
	char* pCurrItem = NULL;
	char* pNextItem = NULL;

	pCurrItem = csProductSName;
	do 
	{
		while(pCurrItem[nItemLen]!='\r' && pCurrItem[nItemLen]!='\000')
		{
			nItemLen++;
		}
		if(pCurrItem[nItemLen]=='\000')
			break;
		if(nItemLen>=7)
			break;
		pCurrItem = pCurrItem+nItemLen+2;
	} while(pCurrItem[nItemLen]!='\000');

	if(nItemLen<7)
	{
		bRetHasData = FALSE;
		return NULL;
	}

	if(pCurrItem[nItemLen]=='\r')
		pNextItem = pCurrItem+nItemLen+2;

	char* csItem = new char[nItemLen+8];
	char csCodes[128];

	ZeroMemory(csItem, nItemLen+8);
	ZeroMemory(csCodes, sizeof(csCodes));
	strncpy(csItem, csProductSName, nItemLen);

	sscanf_s(csItem, "%[^,],%[^,],%[^,],%c", 
			codeHead.strCodeName, sizeof(codeHead.strCodeName)-1, 
			csCodes, sizeof(csCodes)-1, 
			codeHead.strCodeNameJP, sizeof(codeHead.strCodeNameJP)-1,
			&codeHead.cType);
	bRetHasData = GetCodesItem(csCodes, codeHead);
	delete[] csItem;
	
	return pNextItem;
}

BOOL GetCodesItem(char* csCodes, CODEHEADDATA& codeHead)
{
	if(csCodes==NULL) 
		return FALSE;

	char seps[] = "|";
	char* pSplit = NULL;
	char* pNextString = NULL;
	pSplit = strtok_s(csCodes, seps, &pNextString);
	while(pSplit!=NULL)
	{
		strcpy_s(codeHead.strCodeHeads[codeHead.nCmbCodeCount],
				23, pSplit);
		codeHead.nCmbCodeCount++;
		if(codeHead.nCmbCodeCount>=conMaxRltCodeNum)
		{
			codeHead.nCmbCodeCount=conMaxRltCodeNum;
			break;
		}
		pSplit = strtok_s(NULL, seps, &pNextString);
	}
	return codeHead.nCmbCodeCount>0;
}

COwnerManager g_oWonerMng;

bool InitOwner(std::string& strAccount, std::string& strTradingDay)
{
	return g_oWonerMng.Init(strAccount, strTradingDay);
}

bool AppendOwner(int nFrontID, int nSessionID)
{
	return g_oWonerMng.AppendOwner(nFrontID, nSessionID);
}

bool HasOwner(int nFrontID, int nSessionID)
{
	return g_oWonerMng.HasOwner(nFrontID, nSessionID);
}

bool HasOwnerTrade(PlatformStru_TradeInfo& trade)
{
	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return FALSE;
	IPlatformSingleSvr* pSvr = NULL;
	if(IS_MULTIACCOUNT_VERSION)
		pSvr = g_pPlatformMgr->GetPlatformSvr(std::string(trade.InvestorID));
	else
		pSvr = DEFAULT_SVR();
	if(pSvr==NULL) 
		return FALSE;

	PlatformStru_OrderInfo outData;
	if(!pSvr->GetOrder2(string(trade.OrderSysID), outData))
		return FALSE;
	return HasOwner(outData.FrontID, outData.SessionID);
}

int VerifyOrder(PlatformStru_InputOrder& req)
{
	int nRet = 0;
#if _FORCEVERIFYORDER
	if(g_Plusin.GetOrderVerifyStatus()) {
		g_Plusin.SendNeedVerifyOrder(req);
	}
	else 
#else
	{
#if _TEMPNONEOPENLONG

	if(req.CombOffsetFlag[0]==THOST_FTDC_OF_Open && 
			req.Direction==THOST_FTDC_D_Buy) {
		wxEvtHandler* pFrame=(wxEvtHandler*)GETTOPWINDOW();
		wxCommandEvent evt(wxEVT_MASSAGE_NOTICE);
		evt.SetString("�˰汾��ֹ�������롣");
		pFrame->AddPendingEvent(evt);
		//wxMessageBox("�˰汾��ֹ�������룬���ȷ���������");
		return 0;
	}

#endif
		if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
			return nRet;
		IPlatformSingleSvr* pSvr = NULL;
		if(IS_MULTIACCOUNT_VERSION)
			pSvr = g_pPlatformMgr->GetPlatformSvr(std::string(req.InvestorID));
		else
			pSvr = DEFAULT_SVR();
		if(pSvr==NULL) 
			return nRet;

		nRet = pSvr->ReqOrderInsert(req, DEFAULT_SVR()->GetNextReqID());
	}
#endif
	return nRet;
}

int VerifyOrder(PlatformStru_ParkedOrder& req)
{
	return 0;
}
