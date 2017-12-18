// Tools_Ansi.h: interface for the CTools_Ansi class.
//
//	Author		:lzq
//				 Copyright (C), 2008-2028
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLS_ANSI_H__1215343F_84A5_4639_A71B_C83B93EE48B2__INCLUDED_)
#define AFX_TOOLS_ANSI_H__1215343F_84A5_4639_A71B_C83B93EE48B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma warning(disable : 4996)
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)

#include <time.h>
#include "public.h"


#ifdef WIN32
#pragma managed(push,off)
#endif 


#define Tools_Ansi_GetBit(pMap_uint32,Offset) 		(((((uint32*)(pMap_uint32))[(Offset)/32]) & ((uint32)1<<(uint32)((Offset)%32)))==0?0:1)
#define Tools_Ansi_SetBit(pMap_uint32,Offset)		((((uint32*)(pMap_uint32))[(Offset)/32]) |= ((uint32)1<<(uint32)((Offset)%32)))
#define Tools_Ansi_ClrBit(pMap_uint32,Offset) 		((((uint32*)(pMap_uint32))[(Offset)/32]) &= (~((uint32)1<<(uint32)((Offset)%32))))
#define Tools_Ansi_CMPDATA(p1,p2,Type)				( (*(Type*)(p1)==*(Type*)(p2)) ? 0 : ( (*(Type*)(p1)) > (*(Type*)(p2)) ? 1 : -1))
#define Tools_Ansi_IsDialPadKey(Key) ((Key)>=(int)'0'&&(Key)<=(int)'9'||(Key)==(int)'*'||(Key)==(int)'#'?1:0)

class CTools_Ansi  
{
public:
	CTools_Ansi();
	virtual ~CTools_Ansi();


public:

	//�滻���е�ȫ���Ӵ�
	static void ReplaceStr(char *Str,char *SubStr,char *NewSubStr);	
	//�滻���еĵ�һ���Ӵ�
	static void ReplaceStrOnce(char *Str,char *SubStr,char *NewSubStr);

	static void DeleteStrRightSpace(char *Str);
	static void DeleteStrLeftRightSpace(char *Str);
	static void DeleteStrLeftRightSpace_10_13(char *Str);
	static void DeleteStrLeftSpace(char *Str);

	//description:look for offset of the next digit bytes, current digit word will be ignore
	static int GetNextDigitOff(char *pStr,int Len=0,int StartOff=0);	
	//description	:look for offset of the next character bytes, Len can be empty
	static int GetNextCharOff(char Ch,char *pStr,int Len=0,int StartOff=0);
	//description	:look for offset of the next two character bytes, Len can be empty
	static int GetNextCharOff2(char Ch1,char Ch2,char *pStr,int Len=0,int StartOff=0);
	//description	:look for offset of the next char that is't space��tab��\r��\n, Len can be empty
	static int GetNextValidOff(char *pStr,int Len,int StartOff);		
	//description	:look for offset of the next char that is space��tab��\r��\n, Len can be empty
	static int GetNextInvalidOff(char *pStr,int Len,int StartOff);		
	//description	:��ǰ�������ҵ�һ����һ���ַ���Valid��λ��
	static int GetPrevInvalidOff(char *pStr,int StartOff,int MinOff=0);
	//description	:look for offset of the next char that is special character, or space��tab��\r��\n, Len can be empty
	static int GetNextCharOrInvalidOff(char Ch,char *pStr,int Len,int StartOff);	
	//description	:look for offset of the next field, seperated by Seperator��Len can be empty
	static int GetNextFieldOff(char *pStr,int Len,int StartOff,char Seperator1,char Seperator2,char Seperator3,char Seperator4);	
	//ȡָ��λ�ÿ�ʼ��ָ�������֮ǰ���ַ�����ѹ��ͷβ�Ŀո��tab
	static BOOL GetCurField(char *pStr,int Len,int StartOff,int *RltFieldStart,int *RltFieldEnd,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);
	static int GetFourField(char *pStr,int Len,int StartOff,int *RltFieldStart1,int *RltFieldEnd1,int *RltFieldStart2,int *RltFieldEnd2,int *RltFieldStart3,int *RltFieldEnd3,int *RltFieldStart4,int *RltFieldEnd4,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);
	//description	:get sub string in Yuankuohao
	static BOOL GetSubStrInYuankuohao(const char *pSour,char *pRlt,int RltSize);			


    //ִ��WSAStartup
    static void PerformWSAStartup(void);
    //ִ��WSACleanup
    static void PerformWSACleanup(void);
	//get local host name and local ip, ע�⣬���ô˺���ǰҪ����WSAStartup(). pHostName����ΪNULL
	static BOOL GetLocalHostName_IP(char *pHostName,int cbHostName,DWORD *pLocalIP=NULL,int MaxIPCount=1);			

	//function		:GetIPFromHostName
	//description	:1. get local host name and local ip
    //               2. �����ڽ���������ΪIP
	//return		:IP number in result
	static int GetIPFromHostName(char *pHostName,DWORD *pHostIP,int MaxIPCount);
	//���ַ�����ʽ��IP��ַת����DWORD
	static DWORD ConvertStrIP(const char *pIP);																			
	//��DWORD��ʽ��IP��ַת�����ַ���
	static char* ConvertdwIP(DWORD dwIP);
	//�ж��ǲ���ͬһ������
	static BOOL IsSameHost(DWORD* iHostIP,int IPCount,DWORD TargetIP);												

	//���ݷָ��������ַ�����ȡ����N���ֶ�
	static BOOL GetFieldFromStr(const char *pData,int FieldOrd,const char *pSeperator,char *RltBuf,int RltSize);	


	//	�ж��ַ������ʱ���Ƿ������ǰʱ��
	//	1. strDateTime���԰����������ʱ�䣬��ͬʱ�䲿���÷ֺŻ򶺺ŷָ�
	//	2. ÿ��ʱ�����date���ֺ�time����
	//	3. ���ÿ��ʱ����û��date����ʾdate�������Ϸ������û��time����ʾtime�������Ϸ�
	//	3. date���ֿ��԰������ʱ�䣬�ո�ָ���ȡֵ��Χ��
	//		�����ա���ĩ����1����2����3����4����5����6����7����0����1-��3
	//		1�š�2�š�......31�š�1��-10��
	//		1��1�ա�12��31�ա�1��1��-12��31��
	//	4. time���ֿɰ������ʱ�䣬�ո�ָ�����ʽ��
	//		hh:mm:ss
	//		hh:mm:ss-hh:mm:ss
	//		hh:mm
	//		hh:mm-hh:mm
	//	5. ʱ���ڲ����пո�
	static BOOL IsNowValidDateTimeStr(const char *strDateTime);


	//	�ж��ַ������ʱ���Ƿ������ǰʱ��
	//	1. strDateȡֵ��Χ��
	//		�����ա���ĩ����1����2����3����4����5����6����7����0����1-��3
	//		1�š�2�š�......31�š�1��-10��
	//		1��1�ա�12��31�ա�1��1��-12��31��
	//	2. ʱ���ڲ����пո�
	static BOOL IsNowValidDateStr(const char *strDate);


	//	�ж��ַ������ʱ���Ƿ������ǰʱ��
	//	1. strTime��ʽ��
	//		hh:mm:ss
	//		hh:mm:ss-hh:mm:ss
	//		hh:mm
	//		hh:mm-hh:mm
	//	2. ʱ���ڲ����пո�
	static BOOL IsNowValidTimeStr(const char *strTime);

	//	��NewFileָ���wave�ļ��ϲ���TargetFileָ���wave�ļ���
	//	NewFile��TargetFile�ļ��ĸ�ʽ����һ��
	static BOOL AppendWaveFileToAnother(const char *TargetFile,const char *NewFile);
	//	��Wave�ļ��У�ȡ��Ƶ����
	//	���س��ȡ����iSize�����󣬻�pContentΪ�գ���������Ҫ�ĳ���
	static int GetContentFromWaveFile(const char *FileName,char *pContent,DWORD iSize);
	//	��Wave�ļ��У�ɾ��ָ������
	//	pContent�ĳ���Ӧ�ô���16
	//	UnitBytesָһ������ȥ�����ֽڣ�16bits�������2�ֽ�
	static BOOL RemoveContentFromWaveFile(const char *FileName,const char *pContent,int Len,int UnitBytes);

	//	�Ӵ�·�����ļ�����ȡ��������ļ���
	//	PathFileName:��·�����ļ���
	//	RltFileName:��������Ĳ���·�����ļ���
	//	RltSize:���ߴ�
	static bool GetPureFileName(const char* pFileNameWithPath,char* pRltFileName,int RltSize);
	//	��ȫ·���ļ�����ȡ���̷�����־��:
	static bool GetDiskSymbol(const char* pFileName,char* pRltDiskSymbol,int RltSize);
	//	��ȫ·���ļ�����ȡ��·���������̷�
	static bool GetFilePathWithoutDiskSymbol(char* pFileName,char* pRltFilePath,int RltSize);
	//	��ȫ·���ļ�����ȡ��·�������̷�
	static bool GetFilePathWithDiskSymbol(char* pFileName,char* pRltFilePath,int RltSize);


	static int GetDeltaMS(time_t Endt1,int Endms1,time_t Startt2,int Startms2);
	static void MyTrim(char*pStr);



	//	ȡ��ǰ��ʱ�䣬����time_t��������int�ĺ�����
	static void GetComplexAnsiTime(time_t *pRltSecs,int *pRltMS);


	//	���ַ�����ȡ��time_tʱ��
	//	���ڸ�ʽΪ��xxxx��xx��xx�գ� xxxx/xx/xx�� xxxx-xx-xx
	//	ʱ���ʽΪ��xxʱxx��xx�룻 xx:xx:xx�� xx:xx
	static time_t	GetAnsiTimeFromString(char *pBuf);

	//	����һ��time_tʱ��
	static time_t MakeAnsiTime(int wYear,int wMon,int wDay,int wHour,int wMin,int wSec);

	//	��time_t��itimeMS��ʾ��ʱ��ת����yyyy-mm-dd hh:mm:ss.xxx���ַ���
	//	GetPartMap:Ҫȡ��ʱ�䲿�֣���λ��ʾ���ӵ�0λ��ʼ��ʾ���ꡢ�¡��ա�ʱ���֡��롢����
	static char *GetAnsiTimeString(char *pBuf,time_t itime,unsigned int itimeMS,unsigned int GetPartMap=0xffffffff);

    //  ���ַ�����ʾ��ʱ��ת����utc��time_tʱ��
    //  pDate:yyyymmdd
    //  pTime:hhmmss �� hh:mm:ss
    static time_t Get_time_t_utc_from_strTime(const char* pDate, const char* pTime);

	/// ��ʱ���ַ���תΪ����
	/// pTime:hhmmss �� hh:mm:ss
	static unsigned int CalculateTotalSeconds(const char* pTime);

    //  ��utc��time_tʱ��ת�����ַ�����ʾ��ʱ��
    //  pRltDate:yyyymmdd
    //  pRltTime:hh:mm:ss
    static void Get_strTime_from_utc(time_t iTime,char* pRltDate,char* pRltTime);

    //  ��utc��time_tʱ��ת���ɱ���ʱ��
    static void Get_DateTime_from_utc(time_t iTime,int* pRltYear=NULL,int* pRltMonth=NULL,int* pRltDay=NULL,int* pRltHour=NULL,int* pRltMinutes=NULL,int * pRltSecs=NULL,int* pRltWeekDay=NULL);
    //  ������ʱ��ת����utcʱ��
    static time_t Get_utc_from_DateTime(int year,int month,int day,int hour=0,int minutes=0,int secs=0);

    //  ���ַ�����ʾ��ʱ��ת���ɵ���00:00:00��ʼ������
    //  pTime:hhmmss �� hh:mm:ss
    static int Get_TodaySecs_from_strTime(char* pTime);


	//	��һ����ֵת���ɶ����Ʊ�ʾ���ַ�����bit0��pRlt[BitNum-1]
	static char *GetBinFormatString(char *pRlt,unsigned int Val,unsigned int BitNum);	
	//	��һ����ֵת���ɶ����Ʊ�ʾ���ַ���bit0��pRlt[0]
	static char *GetBinFormatString2(char *pRlt,unsigned int Val,unsigned int BitNum);


	//�ö��ַ���һ������Ļ����������һ���ַ���
	static int LookupInSequenceBuffer_Mem(char *SeqBuf,int UnitNum,int UnitSize,char *Str,int Len,int *pLastHead=NULL,int *pLastTail=NULL);
	//�ö��ַ���һ������Ļ����������һ��DWORD
	static int LookupInSequenceBuffer_DWORD(char *SeqBuf,int UnitNum,int UnitSize,unsigned int dwID,int *pLastHead=NULL,int *pLastTail=NULL);

    //��LookupInSequenceBuffer()���õ��ıȽϺ���
    typedef	int	(*COMPARE_LookupInSequenceBuffer)(const void* pdata1,const void* pdata2);
    //	�ö��ַ���һ������Ļ��������������
    //	�����Ǵ�С����
    //  �������RltBegin��RltEnd�У���ʾ��һ������������λ�ú͵�һ��������������λ�á�(������ȣ���ʾû�ҵ�)
    void LookupInSequenceBuffer(int& RltBegin,int& RltEnd,
                                 const void* pData,int UnitNum,int UnitSize,
                                 const void* pLookupContent,
                                 COMPARE_LookupInSequenceBuffer CompareMethod);

	//��ð�ݷ��������������Ǵ�С����
	static void SetSequenceInBubble(char *Buf,int UnitNum,int UnitSize,int CmpOff,int CmpLen);
																
	//	ȡָ����Χ�ڵ������
	static unsigned int GetRandomValue(unsigned int MinVal,unsigned int MaxVal);

};


#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_TOOLS_ANSI_H__1215343F_84A5_4639_A71B_C83B93EE48B2__INCLUDED_)
