// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once


enum {	conMaxRltCodeNum = 32 };

typedef struct {
	int	 nCmbCodeCount;
	char cType;
	char strCodeName[24];	// ��Լ���
	char strCodeNameJP[24];	// ��Լ��Ƶ�ƴ������ĸ
	char strCodeHeads[conMaxRltCodeNum][24];	// ��Լ����ͷ
} CODEHEADDATA, *PCODEHEADDATA;


int GetNumDigits(double dbTick);
CString Number2String(int nData);
// �۸���ʾת������������һ���۸��ַ���
// dbData, �۸�
// dbTick����С�䶯��λ
CString Price2String(double dbData, double dbTick);
CString Double2String(double dbData);
CString Money2String(double dblAmount, int precision=0);

BOOL CheckSumPrice(double& fPrice, double fPriceTick, int nStep=0);
double AnalysePrice(double fPrice, double fUpperLimitPrice, double fLowerLimitPrice);

BOOL ConvertComboInstrumentID(const CString& strComboID, CString& strInstrumentID1, CString& strInstrumentID2);

BOOL MakeReq_Order(PlatformStru_InputOrder& req, CString& strInstrumentID, CString& strAccount, 
				   int nOrderType, BOOL bDirection, int nOCMode, int nVolume, 
				   double fPrice, double fTriggerPrice, BOOL bIsHedge, BOOL bIsMarketPrice, 
				   BOOL bIsReplaceOrder);

char* GetProductSNameItem(char* csProductSName, CODEHEADDATA& codeHead, BOOL& bRetHasData);
BOOL GetCodesItem(char* csCodes, CODEHEADDATA& codeHead);
