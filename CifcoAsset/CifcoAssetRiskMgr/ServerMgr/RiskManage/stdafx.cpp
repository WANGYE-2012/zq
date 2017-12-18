// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// RiskManage.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

//��ȡ���ҵķֶ���ʾ��ʽ
CString GetAccountFormatString(double dAmount, int nPrecision)
{
	char szBuf[256]={0};
	char szFormat[16]={0};
	int i,Len,ipt;

	if(nPrecision<0&&nPrecision!=-1) nPrecision=2;

	if(nPrecision==-1)
	{
		_snprintf(szBuf,255,"%f",dAmount);
		szBuf[sizeof(szBuf)-1]=0;
		Len=strlen(szBuf);
		for(i=0;i<Len&&szBuf[i]!='.';i++);
		if(i<Len)
		{
			//���˷���������Ч����
			while(Len>0&&szBuf[Len-1]=='0') 
			{
				szBuf[Len-1]=0;
				Len--;
			}
		}
		Len=strlen(szBuf);
		if(Len>0&&szBuf[Len-1]=='.')
			szBuf[Len-1]=0;
	}
	else
	{
		_snprintf(szFormat,15,"%%.%df",nPrecision);
		_snprintf(szBuf,255,szFormat,dAmount);
		szBuf[sizeof(szBuf)-1]=0;
	}

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
	return szBuf;
}