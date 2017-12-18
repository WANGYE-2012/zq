// tools.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <windows.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <stdlib.h>
#include "tools_tools.h"
#include "BusinessData.hpp"
#include <string>
#include <map>

#pragma warning(disable:4996)


CTools::CTools()
{
	return;
}


//	��ȫ·���ļ�����ȡ���������ļ���
bool CTools::GetPureFileName(char* pFileNameWithPath,char* pRltFileName,int RltSize)
{
	int Len,Off;

	if(!pFileNameWithPath||!pRltFileName) return FALSE;

	pRltFileName[0]=0;

	Len=strlen(pFileNameWithPath);
	Off=Len;
	while(Off>0&&pFileNameWithPath[Off-1]!='\\'&&pFileNameWithPath[Off-1]!=':') 
		Off--;

	if(RltSize>0&&Len-Off>=RltSize)
		return FALSE;

	memcpy(pRltFileName,pFileNameWithPath+Off,Len-Off);
	pRltFileName[Len-Off]=0;

	return TRUE;
}

//	��ȫ·���ļ�����ȡ���̷�����־��:
bool CTools::GetDiskSymbol(char* pFileName,char* pRltDiskSymbol,int RltSize)
{
	char *pTemp;
	int Len;

	if(!pFileName||!pRltDiskSymbol) return FALSE;

	pRltDiskSymbol[0]=0;

	pTemp=strstr(pFileName,":");
	if(!pTemp) return FALSE;

	Len=pTemp-pFileName+1;
	if(RltSize>0&&Len>=RltSize) return FALSE;
	
	memcpy(pRltDiskSymbol,pFileName,Len);
	pRltDiskSymbol[Len]=0;
	return TRUE;
}

//	��ȫ·���ļ�����ȡ��·���������̷�
bool CTools::GetFilePathWithoutDiskSymbol(char* pFileName,char* pRltFilePath,int RltSize)
{
	char *pTempHead;
	int OffHead;
	int Len;

	if(!pFileName||!pRltFilePath) return FALSE;

	pRltFilePath[0]=0;

	pTempHead=strstr(pFileName,":");
	OffHead=0;
	if(pTempHead!=NULL)
		OffHead=pTempHead-pFileName+1;
	

	Len=strlen(pFileName);
	while(Len>0&&pFileName[Len-1]!='\\'&&pFileName[Len-1]!=':') 
		Len--;

	if(RltSize>0&&Len-OffHead>=RltSize)
		return FALSE;

	memcpy(pRltFilePath,pFileName+OffHead,Len-OffHead);
	pRltFilePath[Len-OffHead]=0;

	if(Len-OffHead>0&&pRltFilePath[Len-OffHead-1]!='\\'&&(RltSize<=0||Len-OffHead<RltSize-1))
	{
		pRltFilePath[Len-OffHead]='\\';
		pRltFilePath[Len-OffHead+1]=0;
	}

	return TRUE;
}

//	��ȫ·���ļ�����ȡ��·�������̷�
bool CTools::GetFilePath(char* pFileName,char* pRltFilePath,int RltSize)
{
	int Len;

	if(!pFileName||!pRltFilePath) return FALSE;

	pRltFilePath[0]=0;

	Len=strlen(pFileName);
	while(Len>0&&pFileName[Len-1]!='\\'&&pFileName[Len-1]!=':') 
		Len--;

	if(RltSize>0&&Len>=RltSize)
		return FALSE;

	memcpy(pRltFilePath,pFileName,Len);
	pRltFilePath[Len]=0;

	if(pRltFilePath[Len-1]!='\\'&&(RltSize<=0||Len<RltSize-1))
	{
		pRltFilePath[Len]='\\';
		pRltFilePath[Len+1]=0;
	}

	return TRUE;
}


//	ɾ���ַ���ͷβ�Ŀո�Tab��\r��\n
void CTools::Trim_Space(char* pString)
{
	int Len;
	int i;

	Len=strlen(pString);
	while(Len>0&&(pString[Len-1]==' '||pString[Len-1]=='\t'||pString[Len-1]=='\r'||pString[Len-1]=='\n')) Len--;
	pString[Len]=0;

	i=0;
	while(i<Len&&(pString[i]==' '||pString[i]=='\t'||pString[i]=='\r'||pString[i]=='\n')) i++;
	if(i>0) 
		memmove(pString,pString+i,Len-i+1);

}


//	ɾ���ַ���β����//��ͷ��ע��
void CTools::Trim_Note(char* pString)
{
	int Len;
	int i;
	int iInquotationMark;									//�Ƿ���˫���ŷ�Χ��

	Len=strlen(pString);
	iInquotationMark=0;

	for(i=0;i<Len;i++)
	{
		if(pString[i]=='"')
			iInquotationMark=1-iInquotationMark;
		if(iInquotationMark==0&&pString[i]=='/'&&pString[i+1]=='/')
			break;
	}
	pString[i]=0;
}

//��Name=Value��ʽ���ַ�����ȡ��Name��Value, ���Value��˫�����������Ļ���ɾ��˫����
bool CTools::GetName_ValueFromString(char* pString,char* pRltName,char* pRltValue)
{
    char *pTemp;
    int Len,NameLen,ValueLen;

    if(!pString||!pRltName||!pRltValue) return false;

    try
    {
        pRltName[0]=0;
        pRltValue[0]=0;

        Len=strlen(pString);

        Trim_Note(pString);
        pTemp=strstr(pString,"=");
        if(!pTemp) return false;
        
        NameLen=pTemp-pString;
        ValueLen=Len-NameLen-1;

        if(ValueLen<1||pTemp[1]=='=') return false;

        memcpy(pRltName,pString,NameLen);
        pRltName[NameLen]=0;
        memcpy(pRltValue,pTemp+1,ValueLen);
        pRltValue[ValueLen]=0;

        Trim_Space(pRltName);
        Trim_Space(pRltValue);

        ValueLen=strlen(pRltValue);
        if(ValueLen>=2&&pRltValue[0]=='"'&&pRltValue[ValueLen-1]=='"')
        {
            memmove(pRltValue,pRltValue+1,ValueLen-2);
            ValueLen-=2;
            pRltValue[ValueLen]=0;
            Trim_Space(pRltValue);
        }

        ValueLen=strlen(pRltValue);
        if(ValueLen>=2&&pRltValue[0]=='\''&&pRltValue[ValueLen-1]=='\'')
        {
            memmove(pRltValue,pRltValue+1,ValueLen-2);
            ValueLen-=2;
            pRltValue[ValueLen]=0;
            Trim_Space(pRltValue);
        }

        if(strlen(pRltName)>0&&strlen(pRltValue)>0) return true;
        else return false;
    }
    catch(...)
    {
        pRltName[0]=0;
        pRltValue[0]=0;
        return false;
    }
}


//	�ж�һ��ʱ���Ƿ���ָ����ʱ����ڡ�ʱ����һ��ķ����������α�ʾ
bool CTools::IsInTimePeriod(int iMins,int StartMins,int EndMins)
{
	iMins=iMins%(24*60);
	StartMins=StartMins%(24*60);
	EndMins=EndMins%(24*60);
	if(EndMins>=StartMins&&iMins>=StartMins&&iMins<EndMins||
		EndMins<StartMins&&(iMins>=StartMins||iMins<EndMins))
		return TRUE;
	else return FALSE;
}


//	ȡ��ǰ�������ͺ�����
void CTools::GetTime(time_t *pSec,int *pMS)
{
	struct _timeb itime;

	_ftime(&itime);
	if(pSec) *pSec=itime.time;
	if(pMS) *pMS=itime.millitm;
}


//	�ö��ַ���һ������Ļ����������һ���ַ���
//	���ҵ����ַ������������ڻ�����������
//	���Ҳ���������-1��pLastHead��pLastTail�ǰ���ָ���ַ�����������ֵ
//	�����Ǵ�С����
//	��Сд�޹�
int CTools::LookupStringInSequenceBuffer_memicmp(char *SeqBuf,int UnitNum,int UnitSize,char *pMem,int Len,int *pLastHead,int *pLastTail)
{
	int			Head,Tail,Mid,Rlt;

	if(Len>UnitSize) return -1;

	Head=0;
	Tail=UnitNum;
	Mid=(Head+Tail)/2;
	while(1)
	{
		Rlt=_memicmp(pMem,SeqBuf+Mid*UnitSize,Len);
		if(Rlt>0&&Head!=Mid) { Head=Mid;  Mid=(Head+Tail)/2; }
		else if(Rlt<0&&Tail!=Mid) { Tail=Mid;  Mid=(Head+Tail)/2; }
		else break;
	}
	if(pLastHead) *pLastHead=Head;
	if(pLastTail) *pLastTail=Tail;
	if(!Rlt) return Mid;
	else return -1;
}


//	�ö��ַ���һ������Ļ����������һ���ַ���
//	���ҵ����ַ������������ڻ�����������
//	���Ҳ���������-1��pLastHead��pLastTail�ǰ���ָ���ַ�����������ֵ
//	�����Ǵ�С����
//	��Сд���
int CTools::LookupStringInSequenceBuffer_memcmp(char *SeqBuf,int UnitNum,int UnitSize,char *pMem,int Len,int *pLastHead,int *pLastTail)
{
	int			Head,Tail,Mid,Rlt;

	if(Len>UnitSize) return -1;

	Head=0;
	Tail=UnitNum;
	Mid=(Head+Tail)/2;
	while(1)
	{
		Rlt=memcmp(pMem,SeqBuf+Mid*UnitSize,Len);
		if(Rlt>0&&Head!=Mid) { Head=Mid;  Mid=(Head+Tail)/2; }
		else if(Rlt<0&&Tail!=Mid) { Tail=Mid;  Mid=(Head+Tail)/2; }
		else break;
	}
	if(pLastHead) *pLastHead=Head;
	if(pLastTail) *pLastTail=Tail;
	if(!Rlt) return Mid;
	else return -1;
}




//	��ð�ݷ��������������Ǵ�С����
//	Buf��Ҫ����Ļ���������������UnitNum����Ԫ��ɣ�ÿ����Ԫ�ĳ���ΪUnitSize
//	CmpOffΪҪ�ȽϵĴ��ڵ�Ԫ���λ�ã�CmpLenΪ�Ƚϵĳ���
//	��Сд�޹�
void CTools::SetSequenceInBubble_memicmp(char *Buf,int UnitNum,int UnitSize,int CmpOff,int CmpLen)
{
	int			i,j,AdjustNum,Rlt;
	char		Tmp[10240];

	if(UnitSize>sizeof(Tmp)) return;
	for(i=0;i<UnitNum-1;i++)
	{
		AdjustNum=0;
		for(j=UnitNum-1;j>i;j--)
		{
			Rlt=_memicmp(Buf+CmpOff+(j-1)*UnitSize,Buf+CmpOff+j*UnitSize,CmpLen);
			if(Rlt>0)
			{
				memcpy(Tmp,Buf+(j-1)*UnitSize,UnitSize);
				memcpy(Buf+(j-1)*UnitSize,Buf+j*UnitSize,UnitSize);
				memcpy(Buf+j*UnitSize,Tmp,UnitSize);
				AdjustNum++;
			}
		}
		if(!AdjustNum) break;								//û��Ҫ����������Ѿ��ź�����
	}
}


//	��ð�ݷ��������������Ǵ�С����
//	Buf��Ҫ����Ļ���������������UnitNum����Ԫ��ɣ�ÿ����Ԫ�ĳ���ΪUnitSize
//	CmpOffΪҪ�ȽϵĴ��ڵ�Ԫ���λ�ã�CmpLenΪ�Ƚϵĳ���
//	��Сд���
void CTools::SetSequenceInBubble_memcmp(char *Buf,int UnitNum,int UnitSize,int CmpOff,int CmpLen)
{
	int			i,j,AdjustNum,Rlt;
	char		Tmp[10240];

	if(UnitSize>sizeof(Tmp)) return;
	for(i=0;i<UnitNum-1;i++)
	{
		AdjustNum=0;
		for(j=UnitNum-1;j>i;j--)
		{
			Rlt=memcmp(Buf+CmpOff+(j-1)*UnitSize,Buf+CmpOff+j*UnitSize,CmpLen);
			if(Rlt>0)
			{
				memcpy(Tmp,Buf+(j-1)*UnitSize,UnitSize);
				memcpy(Buf+(j-1)*UnitSize,Buf+j*UnitSize,UnitSize);
				memcpy(Buf+j*UnitSize,Tmp,UnitSize);
				AdjustNum++;
			}
		}
		if(!AdjustNum) break;								//û��Ҫ����������Ѿ��ź�����
	}
}


//	��4�ֽڶ���ķ�ʽȡһ���ڴ��DWORD������
DWORD CTools::GetCRC_dw(DWORD* pdw,int dwCount)
{
	int i;
	DWORD dwRlt=0;
	for(i=0;i<dwCount;i++)
		dwRlt+=pdw[i];
	return dwRlt;
}

//  ���ַ�����ʾ��ʱ��ת����utc��time_tʱ��
//  pDate:yyyymmdd
//  pTime:hh:mm:ss �� hhmmss
//time_t CTools::Get_time_t_utc_from_strTime(const char* pDate, const char* pTime)
//{
//    int iYear,iMon,iDay,iHour,iMin,iSec;
//    struct tm itm;
//    time_t itime;
//
//    if(!pDate&&!pTime) return 0;
//
//    if(!pDate||!pDate[0]) 
//    {
//        SYSTEMTIME CurTime;
//        GetLocalTime(&CurTime);
//        iYear=CurTime.wYear;
//        iMon=CurTime.wMonth;
//        iDay=CurTime.wDay;
//    }
//    else 
//    {
//        sscanf_s(pDate,"%04d%02d%02d",&iYear,&iMon,&iDay);
//    }
//
//    if(!pTime||!pTime[0]) 
//        iHour=iMin=iSec=0;
//    else if(pTime[2]==':'&&pTime[5]==':')
//        sscanf_s(pTime,"%02d:%02d:%02d",&iHour,&iMin,&iSec);
//    else
//        sscanf_s(pTime,"%02d%02d%02d",&iHour,&iMin,&iSec);
//
//    if(iYear<1970||iYear>2030||iMon<1||iMon>12||iDay<1||iDay>31||iHour<0||iHour>23||iMin<0||iMin>59||iSec<0||iSec>59) 
//        return 0;
//
//    memset(&itm,0,sizeof(itm));
//    itm.tm_year=iYear-1900;
//    itm.tm_mon=iMon-1;
//    itm.tm_mday=iDay;
//    itm.tm_hour=iHour;
//    itm.tm_min=iMin;
//    itm.tm_sec=iSec;
//
//    itime=mktime(&itm);                                     //�õ�utcʱ��
//
//    return itime;
//}

/// ��ʱ���ַ���תΪ����
/// pTime:hhmmss �� hh:mm:ss
unsigned int CTools::CalculateTotalSeconds(const char* pTime)
{
	if(pTime==NULL || pTime[0]=='\0')
		return 0;

	int iHour=0;
	int iMin=0;
	int iSec=0;
	unsigned int len=strlen(pTime);
	if(len==6)
	{
		sscanf_s(pTime,"%02d%02d%02d",&iHour,&iMin,&iSec);
	}
	else if(len==8 && pTime[2]==':'&&pTime[5]==':')
	{
		sscanf_s(pTime,"%02d:%02d:%02d",&iHour,&iMin,&iSec);
	}

	if(iHour<0||iHour>23||iMin<0||iMin>59||iSec<0||iSec>59)
		return 0;

	return (iHour*3600+iMin*60+iSec);

}


//  ��utc��time_tʱ��ת�����ַ�����ʾ��ʱ��
//  pRltDate:yyyymmdd��Ҫ��֤�㹻�Ŀռ�
//  pRltTime:hh:mm:ss��Ҫ��֤�㹻�Ŀռ�
void CTools::Get_strTime_from_utc(time_t iTime,char* pRltDate,char* pRltTime)
{
    struct tm *ptm;
    ptm=localtime(&iTime);
    if(!ptm) return;

    if(pRltDate)
        sprintf(pRltDate,"%04d%02d%02d",ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday);
    if(pRltTime)
        sprintf(pRltTime,"%02d:%02d:%02d",ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
}


//  ��utc��time_tʱ��ת���ɱ���ʱ��
void CTools::Get_DateTime_from_utc(time_t iTime,int* pRltYear,int* pRltMonth,int* pRltDay,int* pRltHour,int* pRltMinutes,int * pRltSecs)
{
    struct tm *ptm;

    if(pRltYear) *pRltYear=0;
    if(pRltMonth) *pRltMonth=0;
    if(pRltDay) *pRltDay=0;
    if(pRltHour) *pRltHour=0;
    if(pRltMinutes) *pRltMinutes=0;
    if(pRltSecs) *pRltSecs=0;

    ptm=localtime(&iTime);
    if(!ptm) return;

    if(pRltYear) *pRltYear=ptm->tm_year+1900;
    if(pRltMonth) *pRltMonth=ptm->tm_mon+1;
    if(pRltDay) *pRltDay=ptm->tm_mday;
    if(pRltHour) *pRltHour=ptm->tm_hour;
    if(pRltMinutes) *pRltMinutes=ptm->tm_min;
    if(pRltSecs) *pRltSecs=ptm->tm_sec;
}


//  ���ַ�����ʾ��ʱ��ת���ɵ���00:00:00��ʼ������
//  pTime:hhmmss �� hh:mm:ss ��hh:mm �� h:mm �� h:m �� hh:m
int CTools::Get_TodaySecs_from_strTime(char* pTime)
{
    int iHour=0,iMin=0,iSec=0;

    if(!pTime) return 0;

    if(pTime[2]==':'&&pTime[5]==':')
        sscanf_s(pTime,"%02d:%02d:%02d",&iHour,&iMin,&iSec);
    else if(strlen(pTime)==6)
        sscanf_s(pTime,"%02d%02d%02d",&iHour,&iMin,&iSec);
    else if(strlen(pTime)==5&&pTime[2]==':')
        sscanf_s(pTime,"%02d:%02d",&iHour,&iMin);
    else if(strlen(pTime)==4&&pTime[1]==':')
        sscanf_s(pTime,"%01d:%02d",&iHour,&iMin);
    else if(strlen(pTime)==3&&pTime[1]==':')
        sscanf_s(pTime,"%01d:%01d",&iHour,&iMin);
    else if(strlen(pTime)==4&&pTime[2]==':')
        sscanf_s(pTime,"%02d:%01d",&iHour,&iMin);

    if(iHour<0||iHour>23||iMin<0||iMin>59||iSec<0||iSec>59) 
        return 0;

    return iHour*3600+iMin*60+iSec;
}

//  ��������ӵ���β
//  ������ӵĳ���
int CTools::AppendIntToString(int iVal,char *pStr,int StrSize,char *pHeadStr,char *pTailStr)
{
    char Buf[256];
    int BufLen,HeadLen,TailLen;

    if(StrSize<=0) return 0; 

    itoa(iVal,Buf,10);
    BufLen=strlen(Buf);

    HeadLen = pHeadStr!=NULL ? strlen(pHeadStr) : 0;
    TailLen = pTailStr!=NULL ? strlen(pTailStr) : 0;

    if(HeadLen+BufLen+TailLen<StrSize)
    {
        if(HeadLen>0) memcpy(pStr,pHeadStr,HeadLen);
        if(BufLen>0) memcpy(pStr+HeadLen,Buf,BufLen);
        if(TailLen>0) memcpy(pStr+HeadLen+BufLen,pTailStr,TailLen);
        pStr[HeadLen+BufLen+TailLen]=0;
        return HeadLen+BufLen+TailLen;
    }
    else return 0;
}

//  ��double��ӵ���β
//  ������ӵĳ���
int CTools::AppendDoubleToString(double dVal,char *pStr,int StrSize,char *pHeadStr,char *pTailStr)
{
    char Buf[256];
    int BufLen,HeadLen,TailLen;

    if(StrSize<=0) return 0; 
    //ouyang������Buf�����
    //sprintf(Buf,"%f",dVal);
    sprintf(Buf,"%g",dVal);
    BufLen=strlen(Buf);

    HeadLen = pHeadStr!=NULL ? strlen(pHeadStr) : 0;
    TailLen = pTailStr!=NULL ? strlen(pTailStr) : 0;

    if(HeadLen+BufLen+TailLen<StrSize)
    {
        if(HeadLen>0) memcpy(pStr,pHeadStr,HeadLen);
        if(BufLen>0) memcpy(pStr+HeadLen,Buf,BufLen);
        if(TailLen>0) memcpy(pStr+HeadLen+BufLen,pTailStr,TailLen);
        pStr[HeadLen+BufLen+TailLen]=0;
        return HeadLen+BufLen+TailLen;
    }
    else return 0;
}

//  ��char��ӵ���β
//  ������ӵĳ���
int CTools::AppendCharToString(char cVal,char *pStr,int StrSize,char *pHeadStr,char *pTailStr)
{
    int BufLen,HeadLen,TailLen;

    if(StrSize<=0) return 0; 

    BufLen = cVal!=0 ? 1 : 0;
    HeadLen = pHeadStr!=NULL ? strlen(pHeadStr) : 0;
    TailLen = pTailStr!=NULL ? strlen(pTailStr) : 0;

    if(HeadLen+BufLen+TailLen<StrSize)
    {
        if(HeadLen>0) memcpy(pStr,pHeadStr,HeadLen);
        if(BufLen>0) pStr[HeadLen]=cVal;
        if(TailLen>0) memcpy(pStr+HeadLen+BufLen,pTailStr,TailLen);
        pStr[HeadLen+BufLen+TailLen]=0;
        return HeadLen+BufLen+TailLen;
    }
    else return 0;
}


//  ��string��ӵ���β, ������ӵĳ���
//  ������ӵĳ���
int CTools::AppendStringToString(char *pSourStr,char *pStr,int StrSize,char *pHeadStr,char *pTailStr)
{
    int BufLen,HeadLen,TailLen;

    if(StrSize<=0) return 0;

    BufLen = pSourStr!=NULL ? strlen(pSourStr) : 0;
    HeadLen = pHeadStr!=NULL ? strlen(pHeadStr) : 0;
    TailLen = pTailStr!=NULL ? strlen(pTailStr) : 0;

    if(HeadLen+BufLen+TailLen<StrSize)
    {
        if(HeadLen>0) memcpy(pStr,pHeadStr,HeadLen);
        if(BufLen>0) memcpy(pStr+HeadLen,pSourStr,BufLen);
        if(TailLen>0) memcpy(pStr+HeadLen+BufLen,pTailStr,TailLen);
        pStr[HeadLen+BufLen+TailLen]=0;
        return HeadLen+BufLen+TailLen;
    }
    else return 0;
}


//  ��string��ǰ����ӿո�ʹstring�ĳ���Ϊָ������
void CTools::AppendSpaceToString(char *pStr,int FinalLen)
{
    try
    {
        int Len=strlen(pStr);
        if(Len<FinalLen)
        {
            memmove(pStr+FinalLen-Len,pStr,Len);
            memset(pStr,' ',FinalLen-Len);
            pStr[FinalLen]=0;
        }
    }
    catch(...)
    {
    }
}

//  ��string��ǰ�����'0'��ʹstring�ĳ���Ϊָ������
void CTools::AppendZeroToString(char *pStr,int FinalLen)
{
    try
    {
        int Len=strlen(pStr);
        if(Len<FinalLen)
        {
            memmove(pStr+FinalLen-Len,pStr,Len);
            memset(pStr,'0',FinalLen-Len);
            pStr[FinalLen]=0;
        }
    }
    catch(...)
    {
    }
}

//  ��ip:port��ip port��ʽ�ĵ�ַ���ָ�Ϊ������IP����Port��
bool CTools::SplitIPAddrStr(const char* pAddr,char* pRltIP,int RltIPSize,char* pRltPort,int RltPortSize)
{
    char *pPort;
    int AddrLen,IPLen,PortLen;

    if(RltIPSize>0&&pRltIP) memset(pRltIP,0,RltIPSize);
    if(RltPortSize>0&&pRltPort) memset(pRltPort,0,RltPortSize);

    if(!pAddr) return false;

    try
    {
        AddrLen=strlen(pAddr);
        pPort=strstr((char*)pAddr,":");
        if(!pPort) pPort=strstr((char*)pAddr," ");
        if(!pPort)
        {
            IPLen=AddrLen;
            PortLen=0;
        }
        else 
        {
            IPLen=pPort-pAddr;
            PortLen=AddrLen-1-IPLen;
        }

        if(IPLen>0&&IPLen<RltIPSize&&pRltIP)
        {
            memcpy(pRltIP,pAddr,IPLen);
            pRltIP[IPLen]=0;
        }
        if(PortLen>0&&PortLen<RltPortSize&&pRltPort)
        {
            memcpy(pRltPort,pPort+1,PortLen);
            pRltPort[PortLen]=0;
        }
    }
    catch(...)
    {
        return false;
    }

    return true;

}

//  �Ƚ��ڴ�飬���Ժ���Ŀո�
int CTools::MyMemcmpIgnoreTailSpace(const char* pMem1,const char* pMem2,int MemLen,int* pRealCmpLen)
{
    int Len1,Len2,CmpLen;

    for(Len1=MemLen;Len1>0&&(pMem1[Len1-1]==0||pMem1[Len1-1]==' ');Len1--);
    for(Len2=MemLen;Len2>0&&(pMem2[Len2-1]==0||pMem2[Len2-1]==' ');Len2--);
    if(Len1>Len2) CmpLen=Len1;
    else CmpLen=Len2;

    if(pRealCmpLen) *pRealCmpLen=CmpLen;

    return memcmp(pMem1,pMem2,CmpLen);
}

//void CTools::mytrimleft(std::string& str)
//{
//    str.erase(0,str.find_first_not_of(" "));
//}
//
//void CTools::mytrimright(std::string& str)
//{
//    str.erase(str.find_last_not_of(" ") + 1);
//}
//
//void CTools::mytrim(std::string& str)
//{
//    str.erase(0,str.find_first_not_of(" "));
//    str.erase(str.find_last_not_of(" ") + 1);
//}

////��ȡһ���ַ���trim�����Ч��ʼλ�ú���Ч����
//void CTools::mygettrimpos(const char* pstr,int&off,int&len)
//{
//    if(!pstr) { off=len=0;  return; }
//
//    len=strlen(pstr);
//    while(len>0&&pstr[len-1]==' ') len--;
//    off=0;
//    while(off<len&&pstr[off]==' ') off++;
//    len-=off;
//}

//void CTools::mymemcpy_trim(char* prlt,int rltsize,const char* psrc)
//{
//    if(!prlt||rltsize<=0||!psrc)
//        return;
//
//    int len,off;
//    mygettrimpos(psrc,off,len);
//    if(len>=rltsize) len=rltsize-1;
//
//    if(len>0)
//        memcpy(prlt,psrc+off,len);
//    prlt[len]=0;
//}

////�Ƚ�������������1,0,-1��
//int CTools::mycompare_trynumeric(const wxString& str1,const wxString& str2)
//{
//    const wxString& Str1=str1;
//    const wxString& Str2=str2;
//
////    Str1.Trim(false).Trim(true);
////    Str2.Trim(false).Trim(true);
//
//    bool bdouble1,bdouble2;
//    double dValue1,dValue2;
//    bdouble1=Str1.ToDouble(&dValue1);
//    bdouble2=Str2.ToDouble(&dValue2);
//
//    if(bdouble1&&bdouble2)
//    {
//        if(dValue1==dValue2) return 0;
//        else return (dValue1>dValue2?1:-1);
//    }
//    else
//    {
//        bool bll1,bll2;
//        wxLongLong_t llValue1,llValue2;
//        bll1=Str1.ToLongLong(&llValue1);
//        bll2=Str2.ToLongLong(&llValue2);
//        if(bll1&&bll2)
//        {
//            if(llValue1==llValue2) return 0;
//            else return (llValue1>llValue2?1:-1);
//        }
//        else
//        {
//            int irlt=Str1.CompareTo(Str2);
//            if(irlt==0) return 0;
//            else if(irlt>0) return 1;
//            else return -1;
//        }
//    }
//}

////�Ƚ�������������1,0,-1��
//int CTools::mycompare_trynumeric(const char* str1,const char* str2)
//{
//    if(!str1||!str2) return -1;
//
//    char *endptr1=NULL,*endptr2=NULL;
//
//    //�ȳ���ת����double���бȽ�
//    double d1=strtod(str1,&endptr1);
//    double d2=strtod(str2,&endptr2);
//    if((endptr1==NULL||endptr1[0]==0)&&(endptr2==NULL||endptr2[0]==0))
//    {
//        //��ʾת����double�ɹ�
//        if(d1==d2) return 0;
//        else return (d1>d2?1:-1);
//    }
//
//    //�ٳ���ת����long���бȽ�
//    endptr1=endptr2=NULL;
//    long l1=strtol(str1,&endptr1,0);
//    long l2=strtol(str2,&endptr2,0);
//    if((endptr1==NULL||endptr1[0]==0)&&(endptr2==NULL||endptr2[0]==0))
//    {
//        //��ʾת����long�ɹ�
//        if(l1==l2) return 0;
//        else return (l1>l2?1:-1);
//    }
//
//    //����ַ����Ƚ�
//    int irlt=strcmp(str1,str2);
//    if(irlt==0) return 0;
//    else if(irlt>0) return 1;
//    else return -1;
//
//
//    //const wxString Str1(str1);
//    //const wxString Str2(str2);
//
//    //bool bdouble1,bdouble2;
//    //double dValue1,dValue2;
//    //bdouble1=Str1.ToDouble(&dValue1);
//    //bdouble2=Str2.ToDouble(&dValue2);
//
//    //if(bdouble1&&bdouble2)
//    //{
//    //    if(dValue1==dValue2) return 0;
//    //    else return (dValue1>dValue2?1:-1);
//    //}
//    //else
//    //{
//    //    bool bll1,bll2;
//    //    wxLongLong_t llValue1,llValue2;
//    //    bll1=Str1.ToLongLong(&llValue1);
//    //    bll2=Str2.ToLongLong(&llValue2);
//    //    if(bll1&&bll2)
//    //    {
//    //        if(llValue1==llValue2) return 0;
//    //        else return (llValue1>llValue2?1:-1);
//    //    }
//    //    else
//    //    {
//    //        int irlt=strcmp(str1,str2);
//    //        if(irlt==0) return 0;
//    //        else if(irlt>0) return 1;
//    //        else return -1;
//    //    }
//    //}
//}



////�Ƚ�������������1,0,-1��
//int CTools::mycompare(const std::string& str1,const std::string& str2)
//{
//    int irlt=str1.compare(str2);
//    if(irlt==0) return 0;
//    else if(irlt>0) return 1;
//    else return -1;
//}

////�Ƚ�������������1,0,-1��
//int CTools::mycompare2(const char* str1,const char* str2)
//{
//    if(!str1||!str2) return -1;
//
//    int irlt=strcmp(str1,str2);
//    if(irlt==0) return 0;
//    else if(irlt>0) return 1;
//    else return -1;
//
//    //int irlt=str1.CompareTo(str2);
//    //if(irlt==0) return 0;
//    //else if(irlt>0) return 1;
//    //else return -1;
//}

//BusinessIDת�����ַ���
char* CTools::BusinessID2String(int bid)
{
    switch(bid)
    {
        case BID_RspUserLogin:   return "BID_RspUserLogin";  break;
        case BID_RspUserLogout:   return "BID_RspUserLogout";  break;
        case BID_RspUserPasswordUpdate:   return "BID_RspUserPasswordUpdate";  break;
        case BID_RspTradingAccountPasswordUpdate:   return "BID_RspTradingAccountPasswordUpdate";  break;
        case BID_RspOrderInsert:   return "BID_RspOrderInsert";  break;
        case BID_ErrRtnOrderInsert:   return "BID_ErrRtnOrderInsert";  break;
        case BID_RspOrderAction:   return "BID_RspOrderAction";  break;
        case BID_ErrRtnOrderAction:   return "BID_ErrRtnOrderAction";  break;
        case BID_RtnOrder:   return "BID_RtnOrder";  break;
        case BID_RtnTrade:   return "BID_RtnTrade";  break;
        case BID_RtnDepthMarketData:   return "BID_RtnDepthMarketData";  break;
        case BID_RspQryDepthMarketData:   return "BID_RspQryDepthMarketData";  break;
        case BID_RspParkedOrderInsert:   return "BID_RspParkedOrderInsert";  break;
        case BID_RspParkedOrderAction:   return "BID_RspParkedOrderAction";  break;
        case BID_RspRemoveParkedOrder:   return "BID_RspRemoveParkedOrder";  break;
        case BID_RspRemoveParkedOrderAction:   return "BID_RspRemoveParkedOrderAction";  break;
        case BID_RspQryParkedOrder:   return "BID_RspQryParkedOrder";  break;
        case BID_RspQryParkedOrderAction:   return "BID_RspQryParkedOrderAction";  break;
        case BID_RspQueryMaxOrderVolume:   return "BID_RspQueryMaxOrderVolume";  break;
        case BID_RspSettlementInfoConfirm:   return "BID_RspSettlementInfoConfirm";  break;
        case BID_RspQryOrder:   return "BID_RspQryOrder";  break;
        case BID_RspQryTrade:   return "BID_RspQryTrade";  break;
        case BID_RspQryInvestorPosition:   return "BID_RspQryInvestorPosition";  break;
        case BID_RspQryTradingAccount:   return "BID_RspQryTradingAccount";  break;
        case BID_RspQryInvestor:   return "BID_RspQryInvestor";  break;
        case BID_RspQryTradingCode:   return "BID_RspQryTradingCode";  break;
        case BID_RspQryInstrumentMarginRate:   return "BID_RspQryInstrumentMarginRate";  break;
        case BID_RspQryInstrumentCommissionRate:   return "BID_RspQryInstrumentCommissionRate";  break;
        case BID_RspQryOptionInstrCommRate:   return "BID_RspQryOptionInstrCommRate";  break;
        case BID_RspQryOptionInstrTradeCost:   return "BID_RspQryOptionInstrTradeCost";  break;
        case BID_RspQryExchange:   return "BID_RspQryExchange";  break;
        case BID_RspQryProduct:   return "BID_RspQryProduct";  break;
        case BID_RspQryInstrument:   return "BID_RspQryInstrument";  break;
        case BID_RspQrySettlementInfo:   return "BID_RspQrySettlementInfo";  break;
        case BID_RspQryTransferBank:   return "BID_RspQryTransferBank";  break;
        case BID_RspQryInvestorPositionDetail:   return "BID_RspQryInvestorPositionDetail";  break;
        case BID_RspQryNotice:   return "BID_RspQryNotice";  break;
        case BID_RspQrySettlementInfoConfirm:   return "BID_RspQrySettlementInfoConfirm";  break;
        case BID_RspQryInvestorPositionCombDetail:   return "BID_RspQryInvestorPositionCombDetail";  break;
        case BID_RtnInstrumentStatus:   return "BID_RtnInstrumentStatus";  break;
        case BID_RtnTradingNotice:   return "BID_RtnTradingNotice";  break;
        case BID_RspQryContractBank:   return "BID_RspQryContractBank";  break;
        case BID_RspQryTradingNotice:   return "BID_RspQryTradingNotice";  break;
        case BID_RspQryBrokerTradingParams:   return "BID_RspQryBrokerTradingParams";  break;
        case BID_RspQryBrokerTradingAlgos:   return "BID_RspQryBrokerTradingAlgos";  break;

        case BID_RspFromBankToFutureByFuture:   return "BID_RspFromBankToFutureByFuture";  break;
        case BID_RspFromFutureToBankByFuture:   return "BID_RspFromFutureToBankByFuture";  break;
        case BID_RspQueryBankAccountMoneyByFuture:   return "BID_RspQueryBankAccountMoneyByFuture";  break;
        case BID_RspQryTransferSerial:   return "BID_RspQryTransferSerial";  break;
        case BID_RspQryAccountregister:   return "BID_RspQryAccountregister";  break;

        case BID_RtnFromBankToFutureByFuture:   return "BID_RtnFromBankToFutureByFuture";  break;
        case BID_RtnFromFutureToBankByFuture:   return "BID_RtnFromFutureToBankByFuture";  break;
        case BID_RtnQueryBankAccountMoneyByFuture:   return "BID_RtnQueryBankAccountMoneyByFuture";  break;

        case BID_RspQryExchangeRate:   return "BID_RspQryExchangeRate";  break;

        case BID_FrontDisconnected:   return "BID_FrontDisconnected";  break;
        case BID_FrontConnected:   return "BID_FrontConnected";  break;

        case BID_QryStart:   return "BID_QryStart";  break;
        case BID_ConnectivityStatus:   return "BID_ConnectivityStatus";  break;
        case BID_AlertMessage:   return "BID_AlertMessage";  break;
        case BID_ExchangeRate:   return "BID_ExchangeRate";  break;
        default: return "unknown";  break;
    }
}

