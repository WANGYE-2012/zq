// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// OptionPriceListT.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

int g_nRunMode = 0;

void _SetRunMode(int nMode)
{
	g_nRunMode = nMode;
}

int  _GetRunMode()
{
	return g_nRunMode;
}

char g_cNewOptionType = '2';

void _SetNewOptionType(char cType)
{
	g_cNewOptionType = cType;
}

char _GetNewOptionType()
{
	return g_cNewOptionType;
}
