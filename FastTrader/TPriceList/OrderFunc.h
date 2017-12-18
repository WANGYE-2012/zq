// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once


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

BOOL ConvertComboInstrumentID(const CString& strComboID, CString& strInstrumentID1, 
							  CString& strInstrumentID2);
BOOL ParseOptionFormat(const CString& strOptionInstr, const CString& strExchangeID, 
					   CString& strTargetID, BOOL& bDirect, CString& strPrice);
BOOL ConvOptionFormat(const CString& strOptionInstr, const CString& strExchangeID, 
					  CString& strNewOptionInstr);
