// Global.cpp : ֻ������׼�����ļ���Դ�ļ�
// Module-BookOrder.pch ����ΪԤ����ͷ
// Global.obj ������Ԥ����������Ϣ

#include "stdafx.h"
#include "OrderFunc.h"
#include "Module-Misc2/tools_util.h"


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

CString Number2String(int nData)
{
	char str[512]="-";
	if(nData!=-1) sprintf_s(str, "%d", nData);
	return str;
}

CString Price2String(double dbData, double dbTick)
{
	char strPriceText[512]={0},strFormatText[512]={0};
	ZeroMemory(strPriceText, sizeof(strPriceText));
	ZeroMemory(strFormatText, sizeof(strFormatText));
	strcpy_s(strPriceText, "-");
	if(util::isInvalidValue(dbData)||util::isInvalidValue(dbTick)) 
		return strPriceText;
	sprintf_s(strFormatText,"%%0.%df", GetNumDigits(dbTick));
	sprintf_s(strPriceText, strFormatText, dbData);
	return strPriceText;
}

CString Double2String(double dbData)
{
	char strPriceText[512]={0};
	ZeroMemory(strPriceText, sizeof(strPriceText));
	strcpy_s(strPriceText, "-");
	if(util::isInvalidValue(dbData)) 
		return strPriceText;
	sprintf_s(strPriceText, "%f", dbData);
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

//��ȡ���ҵķֶ���ʾ��ʽ
CString Money2String(double dblAmount, int precision)
{
	char szBuf[256];
    char szFormat[16];
    int i,Len,ipt;

    if(precision<0)
		precision=0;

    ZeroMemory(szBuf, sizeof(szBuf));
	sprintf_s(szFormat, "%%.%df", precision);
    sprintf_s(szBuf, szFormat, dblAmount);
    szBuf[sizeof(szBuf)-1]='\000';

    Len=strlen(szBuf);
    for(ipt=0;ipt<Len&&szBuf[ipt]!='.';ipt++);
    while(ipt>3&&
            szBuf[ipt-3-1]>='0'&&szBuf[ipt-3-1]<='9'&&
            Len<sizeof(szBuf)-10)
    {
        i=ipt-3;
        memmove(szBuf+i+1,szBuf+i,Len-i+1);
        szBuf[i]=',';
        Len++;
        ipt-=3;
    }
    return CString(szBuf);
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

BOOL ConvertComboInstrumentID(const CString& strComboID, CString& strInstrumentID1, CString& strInstrumentID2)
{
	char strTemp[128];
	char strCode1[128];
	char strCode2[128];
	ZeroMemory(strCode1, 128);
	ZeroMemory(strCode2, 128);
	sscanf_s(strComboID, "%s %[^&]&%s", strTemp, sizeof(strTemp)-1, 
			strCode1, sizeof(strCode1)-1, strCode2, sizeof(strCode2)-1);
	strInstrumentID1 = strCode1;
	strInstrumentID2 = strCode2;
	return TRUE;
}

BOOL MakeReq_Order(PlatformStru_InputOrder& req, CString& strInstrumentID, CString& strAccount, 
				   int nOrderType, BOOL bDirection, int nOCMode, int nVolume, 
				   double fPrice, double fTriggerPrice, BOOL bIsHedge, BOOL bIsMarketPrice, 
				   BOOL bIsReplaceOrder)
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
	default:
		req.ContingentCondition = THOST_FTDC_CC_Immediately;		// ����ģʽ
		break;
	};

	// �����ʺ�
	strncpy_s(req.strAccount, strAccount, 
			sizeof(req.strAccount)-1 < strAccount.GetLength() ? 
			sizeof(req.strAccount)-1 : strAccount.GetLength());

	//��Լ����
	strncpy_s(req.InstrumentID, strInstrumentID, 
			sizeof(req.InstrumentID)-1 < strInstrumentID.GetLength() ? 
				sizeof(req.InstrumentID)-1 : strInstrumentID.GetLength());

	//��������
	req.Direction = (bDirection ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell);

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
		req.TimeCondition=THOST_FTDC_TC_GFD;
	}

	//��ƽ��־
	switch(nOCMode) {
	case 0:
		req.CombOffsetFlag[0]='\000';					// δ֪
		break;
	case 1:
		req.CombOffsetFlag[0]=THOST_FTDC_OF_Open;		// ����
		break;
	case 2: 
		req.CombOffsetFlag[0]=THOST_FTDC_OF_CloseToday;	//ƽ��
		break;
	case 3:
		req.CombOffsetFlag[0]=THOST_FTDC_OF_Close;		//ƽ��
		break;
	};

	//���Ͷ���ױ���־��ֻ�е�һ���ֽ���Ч
	if(!bIsHedge)
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;	// Ͷ��
	else
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Hedge;		// ��ֵ

	//����
	req.VolumeTotalOriginal=nVolume;
	//�ɽ�������
	req.VolumeCondition = THOST_FTDC_VC_AV; 
	//��С�ɽ���
	req.MinVolume=1;

	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;

	return TRUE;
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
	strncpy_s(csItem, nItemLen+8, csProductSName, nItemLen);

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
