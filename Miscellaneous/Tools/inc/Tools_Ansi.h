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
#include <map>
#include <set>
#include <vector>
#include <string>
using std::map;
using std::set;
using std::multimap;
using std::vector;
using std::pair;
using std::string;
#include "public.h"


#ifdef WIN32
#pragma managed(push,off)
#endif 



#define Tools_Ansi_GetBit(pMap_uint32,Offset) 		(((((uint32*)(pMap_uint32))[(Offset)/32]) & ((uint32)1<<(uint32)((Offset)%32)))==0?0:1)
#define Tools_Ansi_SetBit(pMap_uint32,Offset)		((((uint32*)(pMap_uint32))[(Offset)/32]) |= ((uint32)1<<(uint32)((Offset)%32)))
#define Tools_Ansi_ClrBit(pMap_uint32,Offset) 		((((uint32*)(pMap_uint32))[(Offset)/32]) &= (~((uint32)1<<(uint32)((Offset)%32))))
#define Tools_Ansi_CMPDATA(p1,p2,Type)				( (*(Type*)(p1)==*(Type*)(p2)) ? 0 : ( (*(Type*)(p1)) > (*(Type*)(p2)) ? 1 : -1))
#define Tools_Ansi_IsDialPadKey(Key) ((Key)>=(int)'0'&&(Key)<=(int)'9'||(Key)==(int)'*'||(Key)==(int)'#'?1:0)

#ifndef Tools_Ansi_ArraySize
  #define Tools_Ansi_ArraySize(arrayvariable) ((int)sizeof(arrayvariable)/(int)sizeof(arrayvariable[0]))
#endif

#ifndef Tools_Ansi_OffsetOfStru
  #ifndef WIN32
    #define Tools_Ansi_OffsetOfStru(stru,membername) offsetof(stru,membername)
  #else
    #define Tools_Ansi_OffsetOfStru(stru,membername) ((int)(&(((stru*)0)->membername)))
  #endif
#endif

#ifndef Tools_Ansi_MemberArraySize
  #ifndef WIN32
    #define Tools_Ansi_MemberArraySize(stru,arraymember) ((int)sizeof(stru::arraymember)/(int)sizeof(stru::arraymember[0]))
  #else
    #define Tools_Ansi_MemberArraySize(stru,arraymember) ((int)sizeof(((stru*)0)->arraymember)/(int)sizeof(((stru*)0)->arraymember[0]))
  #endif
#endif

#ifndef Tools_Ansi_StructMemberSize
  #ifndef WIN32
    #define Tools_Ansi_StructMemberSize(stru,membername) ((int)sizeof(stru::membername))
  #else
    #define Tools_Ansi_StructMemberSize(stru,membername) ((int)sizeof(((stru*)0)->membername))
  #endif
#endif


class CTools_Ansi  
{
public:
	CTools_Ansi();
	virtual ~CTools_Ansi();


public:

	//�滻���е�ȫ���Ӵ�
	static void ReplaceStr(char *Str,const char *SubStr,const char *NewSubStr);	
	//�滻���еĵ�һ���Ӵ�
	static void ReplaceStrOnce(char *Str,const char *SubStr,const char *NewSubStr);

	static void DeleteStrRightSpace(char *Str);
	static void DeleteStrRightSpace_10_13(char *Str);
	static void DeleteStrLeftRightSpace(char *Str);
	static void DeleteStrLeftRightSpace_10_13(char *Str);
	static string DeleteStrLeftRightSpace_10_13(const string& Str);
	static void DeleteStrLeftSpace(char *Str);
	static void DeleteStrRightDigit(char *Str);

    //���˵�˫б����������ע�ͣ������˵���β�Ŀո�Tab���س����з�
    static string FilterLineComment(const string& Str);

	//description:look for offset of the next digit bytes, current digit word will be ignore
	static int GetNextDigitOff(const char *pStr,int Len=0,int StartOff=0);	
	//description	:look for offset of the next character bytes, Len can be empty
	static int GetNextCharOff(char Ch,const char *pStr,int Len=0,int StartOff=0);
	//description	:look for offset of the next two character bytes, Len can be empty
	static int GetNextCharOff2(char Ch1,char Ch2,const char *pStr,int Len=0,int StartOff=0);
	//description	:look for offset of the next char that is't space��tab��\r��\n, Len can be empty
	static int GetNextValidOff(const char *pStr,int Len,int StartOff);		
	//description	:look for offset of the next char that is space��tab��\r��\n, Len can be empty
	static int GetNextInvalidOff(const char *pStr,int Len,int StartOff);		
	//description	:��ǰ�������ҵ�һ����һ���ַ���Valid��λ��
	static int GetPrevInvalidOff(const char *pStr,int StartOff,int MinOff=0);
	//description	:look for offset of the next char that is special character, or space��tab��\r��\n, Len can be empty
	static int GetNextCharOrInvalidOff(char Ch,const char *pStr,int Len,int StartOff);	

    //description	:look for offset of the next field, seperated by Seperator��Len can be empty
	static int GetNextFieldOff(const char *pStr,int Len,int StartOff,char Seperator1,char Seperator2,char Seperator3,char Seperator4);	

    //ȡָ��λ�ÿ�ʼ��ָ�������֮ǰ���ַ��������˵�ǰ�ĵĿո�tab�ͼ����
	static BOOL GetCurField(const char *pStr,int Len,int StartOff,int *RltFieldStart,int *RltFieldEnd,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);
	static BOOL GetCurField_putDest(char *pDest,int MaxDestLen,const char* pStr,int Len,int StartOff,int *RltFieldStart,int *RltFieldEnd,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);
    //ȡָ����Χ���ü�����������������
    static bool GetCurField(const char *pStr,int StartOff,int EndOff,int& FieldStart,int& FieldEnd,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);
	//ȡָ��λ�ÿ�ʼ��ָ�������֮ǰ���ַ����������˵�ǰ�ĵĿո�tab�ͼ���������س���
	static int GetCurField2(const char *pStr,int Len,int StartOff,int& RltFieldStart,int& RltFieldEnd,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);
	static int GetCurField2_putDest(char *pDest,int MaxDestLen,const char* pStr,int Len,int StartOff,int& RltFieldStart,int& RltFieldEnd,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);

	static int GetFourField(const char *pStr,int Len,int StartOff,int *RltFieldStart1,int *RltFieldEnd1,int *RltFieldStart2,int *RltFieldEnd2,int *RltFieldStart3,int *RltFieldEnd3,int *RltFieldStart4,int *RltFieldEnd4,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);
	static int GetFourFieldInt(const char *pStr,int Len,int StartOff,int *RltValue1,int *RltValue2,int *RltValue3,int *RltValue4,char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);
    static int GetFourField(const char *pStr,int StartOff,int EndOff,
        int& FieldStart1,int& FieldEnd1,int& FieldStart2,int& FieldEnd2,int& FieldStart3,int& FieldEnd3,int& FieldStart4,int& FieldEnd4,
		char Seperator1,char Seperator2=0,char Seperator3=0,char Seperator4=0);

    //��Str��ȡ��LeadingStr����ֱ��EndingStr֮��Ĵ�
    static string GetSubStr(const string& Str,const string& LeadingStr,const string& EndingStr);

    //get content in a string that follow prefix.
    static bool GetContentFollowPrefix(const char* pStr,int StartOff,int EndOff,const char* pPrefix,char* pContent,int ContentSize);
    static bool GetContentFollowPrefix(const char* pStr,int StartOff,int EndOff,const char* pPrefix,string rltContent);

	//description	:get sub string in Yuankuohao
	static BOOL GetSubStrInYuankuohao(const char *pSour,char *pRlt,int RltSize);
	//����һ������ָ���ַ��ĸ���
	static int GetCountOfCharInStr(const char* pStr,char Ch);
	//ȡһ������ָ���ַ���λ��
	static std::vector<int> GetCharPos(const char* pStr,char Ch);
	//��һ�������ָ������ֳɶ���Ӵ�
	static std::vector<std::string> SplitString(const char* pStr,char splitchar1,char splitchar2=0);
	static void SplitString(vector<int>& rltsubpos,vector<int>& rltsublen,const char* pStr,char splitchar1,char splitchar2=0);
	static bool SplitString(int* prltsubpos,int* prltsublen,int& rltsubcount,int maxsubcount,const char* pStr,char splitchar1,char splitchar2=0);
	//�зִ�������ʽΪ��str11-str12;str21-str22;str31-str32������[;]Ϊsplitchar1��[-]Ϊsplitchar2�����˷ָ�����β�Ŀո�tab���س�������
	static vector<pair<string,string>> SplitString2(const char* pStr,char splitchar1,char splitchar2);


    //ִ��WSAStartup
    static void PerformWSAStartup(void);
    //ִ��WSACleanup
    static void PerformWSACleanup(void);
	//get local host name and local ip, ע�⣬���ô˺���ǰҪ����WSAStartup(). pHostName����ΪNULL
	static BOOL GetLocalHostName_IP(char *pHostName,int cbHostName,DWORD *pLocalIP=NULL,int MaxIPCount=1);			

	//function		:GetIPFromHostName
    //               GetIPFromHostName2(ȡ��һ��IP)
	//description	:1. get local host name and local ip
    //               2. �����ڽ���������ΪIP
	//return		:IP number in result
	static int GetIPFromHostName(const char *pHostName,DWORD *pHostIP,int MaxIPCount);
	static bool GetIPFromHostName2(const char *pHostName,unsigned int& HostIP);
	//���ַ�����ʽ��IP��ַת����DWORD
	static DWORD ConvertStrIP(const char *pIP);																			
	//��DWORD��ʽ��IP��ַת�����ַ���
	static char* ConvertdwIP(DWORD dwIP);
	static char* ConvertdwIP2(DWORD dwIP,char*buf,int bufsize);
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
	static string GetPureFileName(const string& FileNameWithPath);
	//	��ȫ·���ļ�����ȡ���̷�����־��:
	static bool GetDiskSymbol(const char* pFileName,char* pRltDiskSymbol,int RltSize);
	//	��ȫ·���ļ�����ȡ��·���������̷�
	static bool GetFilePathWithoutDiskSymbol(char* pFileName,char* pRltFilePath,int RltSize);
	//	��ȫ·���ļ�����ȡ��·�������̷�
	static bool GetFilePathWithDiskSymbol(char* pFileName,char* pRltFilePath,int RltSize);


	static int GetDeltaMS(time_t Endt1,int Endms1,time_t Startt2,int Startms2);
	static void MyTrim(char*pStr);
	static void mytrimleft(std::string& str);
    static void mytrimright(std::string& str);	//ɾ�����ұߵĿո�
    static void mytrimright2(std::string& str);	//ɾ�����ұߵĿո�tab��\r��\n
	static void mytrimrightdight(std::string& str);//ɾ�����ұߵĿո�tab��\r��\n������
    static void mytrim(std::string& str);
    //��ȡһ���ַ���trim�����Ч��ʼλ�ú���Ч����
    static void mygettrimpos(const char* pstr,int&off,int&len);
    static void mymemcpy_trim(char* prlt,int rltsize,const char* psrc);
    //�Ƚ�������������1,0,-1
    static int mycompare_trynumeric(const char* str1,const char* str2);
    static int mycompare(const std::string& str1,const std::string& str2);
    static int mycompare2(const char* str1,const char* str2);

	//�жϴ��Ƿ�ȫ��������
	static bool isdigitstring(const char* str,int len=0);



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
    static void Get_strTime_from_utc(time_t iTime,char* pRltDate,char* pRltTime=NULL);
    //  utcת��Ϊhh:mm:ss
    static string Get_strTime_from_utc2(time_t iTime);
    //  utcת��ΪSun, 5 Jan 2014 10:22:33 GMT
    static string Get_strTime_from_utc3(time_t iTime);
    //  ��utc��time_tʱ��ת�����ַ�����ʾ��ʱ��, ��ʽΪyyyy-mm-dd
    static void Get_strDate_from_utc(time_t iTime,char* pRltDate);
	//	��utc��time_tʱ�������ת��Ϊ������, ��ʽΪyyyymmdd����20140511
	static int Get_intDate_from_utc(time_t iTime);

    //  ��utc��time_tʱ��ת���ɱ���ʱ��
    static void Get_DateTime_from_utc(time_t iTime,int* pRltYear=NULL,int* pRltMonth=NULL,int* pRltDay=NULL,int* pRltHour=NULL,int* pRltMinutes=NULL,int * pRltSecs=NULL,int* pRltWeekDay=NULL);
    //  ������ʱ��ת����utcʱ��. ʧ�ܷ���-1
    static time_t Get_utc_from_DateTime(int year,int month,int day,int hour=0,int minutes=0,int secs=0);

    //  ���ַ�����ʾ��ʱ��ת���ɵ���00:00:00��ʼ������
    //  pTime:hhmmss �� hh:mm:ss
    static int Get_TodaySecs_from_strTime(char* pTime);

	//��ָ��λ���Ĵ�ת��Ϊdouble
	inline static double myatod(const char*p,int len,char* buf,int bufsize)
	{
		memcpy(buf,p,len<bufsize?len:bufsize-1);
		buf[len<bufsize?len:bufsize-1]=0;
		return atof(buf);
	}
	//��ָ��λ���Ĵ�ת��Ϊfloat
	inline static float myatof(const char*p,int len,char* buf,int bufsize)
	{
		memcpy(buf,p,len<bufsize?len:bufsize-1);
		buf[len<bufsize?len:bufsize-1]=0;
		return (float)atof(buf);
	}
	//��ָ��λ���Ĵ�ת��Ϊint
	inline static int myatoi(const char*p,int len,char* buf,int bufsize)
	{
		memcpy(buf,p,len<bufsize?len:bufsize-1);
		buf[len<bufsize?len:bufsize-1]=0;
		return atoi(buf);
	}
	inline static string myitoa(int val,int radix=10)
	{
		char buf[64];
		itoa(val,buf,radix);
		return string(buf);
	}



	//	��һ����ֵת���ɶ����Ʊ�ʾ���ַ�����bit0��pRlt[BitNum-1]
	static char *GetBinFormatString(char *pRlt,unsigned int Val,unsigned int BitNum);	
	//	��һ����ֵת���ɶ����Ʊ�ʾ���ַ���bit0��pRlt[0]
	static char *GetBinFormatString2(char *pRlt,unsigned int Val,unsigned int BitNum);

    //  ��һ���ڴ�ת����Hex�ַ���
    static string GetHexStringFromMem(const void* pMem,int MemLen);
    //  ��Hex�ַ���ת���ڴ档���ؽ���ֽڳ���
    static int GetMemFromHexString(void* pMem,int MemSize,const string& strHex);
    static int GetMemFromHexString(void* pMem,int MemSize,const char* pszHex,int HexLen);
    //  ��doubleת����Hex�ַ���
    static string GetHexStringFromDouble(double Val);
    //  ��Hex�ַ���ת��double�����ؽ���ֽڳ���
    static double GetDoubleFromHexString(const string& strHex);
    static double GetDoubleFromHexString(const char* pszHex,int HexLen);
    //  ������ת����ʮ�����ַ���
    static string GetStringFromInteger(int Val);
    //  ʮ�����ַ���ת��Ϊ����
    static int GetIntegerFromString(const string& strVal);
    static int GetIntegerFromString(const char* pszVal);
    //  ��λʮ�������ַ���ת��Ϊ����
    static unsigned char GetByteFromHexString(const string& strHex);

	//�ö��ַ���һ������Ļ����������һ���ַ���
	static int LookupInSequenceBuffer_Mem(char *SeqBuf,int UnitNum,int UnitSize,char *Str,int Len,int *pLastHead=NULL,int *pLastTail=NULL);
	//�ö��ַ���һ������Ļ����������һ��DWORD
	static int LookupInSequenceBuffer_DWORD(char *SeqBuf,int UnitNum,int UnitSize,unsigned int dwID,int *pLastHead=NULL,int *pLastTail=NULL);

    //��LookupInSequenceBuffer()���õ��ıȽϺ���
    typedef	int	(*COMPARE_LookupInSequenceBuffer)(const void* pdata1,const void* pdata2);
    //	�ö��ַ���һ������Ļ��������������
    //	�����Ǵ�С����
    //  �������RltBegin��RltEnd�У���ʾ��һ������������λ�ú͵�һ��������������λ�á�(������ȣ���ʾû�ҵ�)
    static void LookupInSequenceBuffer(int& RltBegin,int& RltEnd,
		const void* pData,int UnitNum,int UnitSize,
		const void* pLookupContent,
		COMPARE_LookupInSequenceBuffer CompareMethod);

	//��ð�ݷ��������������Ǵ�С����
	static void SetSequenceInBubble(char *Buf,int UnitNum,int UnitSize,int CmpOff,int CmpLen);
																
	//	ȡָ����Χ�ڵ������
	static unsigned int GetRandomValue(unsigned int MinVal,unsigned int MaxVal);

    //  ȡ���ִ���ÿ�����ֵ�����ĸ��ɵĴ������ֱ�����GB2312����
    static string GetFirstAlphasOfChineseStrGBK(const string& GBKStr);

	template <typename KeyType,typename ValueType>
	static bool IsKeyValueInMultimap(multimap<KeyType,ValueType>& mmap,const KeyType& ikey,const ValueType& ivalue)
	{
		typename multimap<KeyType,ValueType>::iterator it_upper,it_lower;
		it_lower=mmap.lower_bound(ikey);
		it_upper=mmap.upper_bound(ikey);
		while(it_lower!=it_upper)
		{
			if(it_lower->second==ivalue)
				return true;
			it_lower++;
		}
		return false;
	};
	template <typename KeyType,typename ValueType>
    static bool GetValueInMultimap(const multimap<KeyType,ValueType>& mmap,const KeyType& ikey,ValueType& outvalue)
	{
		typename multimap<KeyType,ValueType>::const_iterator it_upper,it_lower;
		it_lower=mmap.lower_bound(ikey);
		it_upper=mmap.upper_bound(ikey);
		if(it_lower!=it_upper)
		{
            outvalue=it_lower->second;
            return true;
		}
        else return false;
	};
	template <typename KeyType,typename ValueType>
    static void GetValuesInMultimap(const multimap<KeyType,ValueType>& mmap,const KeyType& ikey,vector<ValueType>& outvalue)
	{
        outvalue.clear();
		typename multimap<KeyType,ValueType>::const_iterator it_upper,it_lower;
		it_lower=mmap.lower_bound(ikey);
		it_upper=mmap.upper_bound(ikey);
		while(it_lower!=it_upper)
		{
            outvalue.push_back(it_lower->second);
			it_lower++;
		}
	};
	template <typename KeyType,typename ValueType>
    static vector<ValueType> GetValuesInMultimap(const multimap<KeyType,ValueType>& mmap,const KeyType& ikey)
	{
		vector<ValueType> outvalue;
		typename multimap<KeyType,ValueType>::const_iterator it_upper,it_lower;
		it_lower=mmap.lower_bound(ikey);
		it_upper=mmap.upper_bound(ikey);
		while(it_lower!=it_upper)
		{
            outvalue.push_back(it_lower->second);
			it_lower++;
		}
		return outvalue;
	};
	template <typename KeyType,typename ValueType>
    static void GetValuesInMultimap(const multimap<KeyType,ValueType>& mmap,const KeyType& ikey,set<ValueType>& outvalue)
	{
        outvalue.clear();
		typename multimap<KeyType,ValueType>::const_iterator it_upper,it_lower;
		it_lower=mmap.lower_bound(ikey);
		it_upper=mmap.upper_bound(ikey);
		while(it_lower!=it_upper)
		{
            if(outvalue.find(it_lower->second)==outvalue.end())
                outvalue.insert(it_lower->second);
			it_lower++;
		}
	};
	template <typename KeyType,typename ValueType>
    static void GetAllValuesInMultimap(const multimap<KeyType,ValueType>& mmap,set<ValueType>& outvalue)
	{
        outvalue.clear();
		typename multimap<KeyType,ValueType>::const_iterator it;
		for(it=mmap.begin();it!=mmap.end();it++)
            if(outvalue.find(it->second)==outvalue.end()) outvalue.insert(it->second);
	};
	template <typename KeyType,typename ValueType>
    static void GetAllValuesInMultimap(const multimap<KeyType,ValueType>& mmap,vector<ValueType>& outvalue)
	{
        typename set<ValueType> setvalues;
        GetAllValuesInMultimap<KeyType,ValueType>(mmap,setvalues);
        outvalue.clear();
        typename set<ValueType>::const_iterator itset;
        for(itset=setvalues.begin();itset!=setvalues.end();itset++)
            outvalue.push_back(*itset);
	};
	template <typename KeyType,typename ValueType>
    static void GetAllKeysInMultimap(const multimap<KeyType,ValueType>& mmap,set<KeyType>& outvalue)
	{
        outvalue.clear();
		typename multimap<KeyType,ValueType>::const_iterator it;
		for(it=mmap.begin();it!=mmap.end();it++)
            if(outvalue.find(it->first)==outvalue.end()) outvalue.insert(it->first);
	};
	template <typename KeyType,typename ValueType>
    static void GetAllKeysInMultimap(const multimap<KeyType,ValueType>& mmap,vector<KeyType>& outvalue)
	{
        typename set<ValueType> setvalues;
        GetAllKeysInMultimap<KeyType,ValueType>(mmap,setvalues);
        outvalue.clear();
        typename set<ValueType>::const_iterator itset;
        for(itset=setvalues.begin();itset!=setvalues.end();itset++)
            outvalue.push_back(*itset);
	};

	template <typename KeyType,typename ValueType>
    static void GetKeysInMultimap_MatchValue(const multimap<KeyType,ValueType>& mmap,const ValueType& ivalue,vector<KeyType>& outkeys)
	{
        outkeys.clear();
		for(typename multimap<KeyType,ValueType>::const_iterator it=mmap.begin();it!=mmap.end();it++)
		{
			if(it->second==ivalue)
                outkeys.push_back(it->first);
		}
	};

	template <typename KeyType,typename ValueType>
	static bool EraseKeyValueInMultimap(multimap<KeyType,ValueType>& mmap,const KeyType& ikey,const ValueType& ivalue)
	{
		bool brlt=false;
		typename multimap<KeyType,ValueType>::iterator it_upper,it_lower;
		it_lower=mmap.lower_bound(ikey);
		it_upper=mmap.upper_bound(ikey);
		while(it_lower!=it_upper)
		{
			if(it_lower->second==ivalue)
			{
				it_lower=mmap.erase(it_lower);
				if(!brlt) brlt=true;
			}
			else 
				it_lower++;
		}
		return brlt;
	};

	template <typename KeyType,typename ValueType>
	static bool EraseKeyInMultimap(multimap<KeyType,ValueType>& mmap,const KeyType& ikey)
	{
		bool brlt=false;
		typename multimap<KeyType,ValueType>::iterator it_upper,it_lower;
		it_lower=mmap.lower_bound(ikey);
		it_upper=mmap.upper_bound(ikey);
		if(it_lower!=it_upper)
		{
			brlt=true;
			while(it_lower!=it_upper)
				it_lower=mmap.erase(it_lower);
		}
		return brlt;
	};

	template <typename KeyType,typename ValueType>
	static bool EraseValueInMultimap(multimap<KeyType,ValueType>& mmap,const ValueType& ivalue)
	{
		bool brlt=false;
		typename multimap<KeyType,ValueType>::iterator it;
		for(it=mmap.begin();it!=mmap.end();)
		{
			if(it->second==ivalue) 
			{
				it=mmap.erase(it);
				if(!brlt) brlt=true;
			}
			else it++;
		}
		return brlt;
	};
	template <typename KeyType,typename ValueType>
	static void AddKeyValueToMultimap(multimap<KeyType,ValueType>& mmap,const KeyType& ikey,const ValueType& ivalue)
	{
		typename multimap<KeyType,ValueType>::iterator it_upper,it_lower;
		it_lower=mmap.lower_bound(ikey);
		it_upper=mmap.upper_bound(ikey);
		while(it_lower!=it_upper&&it_lower->second!=ivalue)
			it_lower++;
        if(it_lower==it_upper)
            mmap.insert(pair<KeyType,ValueType>(ikey,ivalue));
	};

	template <typename DataType>
	static bool IsSameVector(const vector<DataType>& vec1,const vector<DataType>& vec2)
	{
		if(vec1.size()!=vec2.size()) return false;
		for(int i=0;i<(int)vec1.size();i++)
			if(vec1[i]!=vec2[i]) return false;
		return true;
	}
	template <typename DataType>
	static bool IsValueInVector(const vector<DataType>& vec,const DataType& value)
	{
		for(int i=0;i<(int)vec.size();i++)
            if(vec[i]==value) return true;
		return false;
	}

	template <typename KeyType,typename ValueType>
	static bool EraseInMap(map<KeyType,ValueType>& imap,const KeyType& ikey)
    {
		typename map<KeyType,ValueType>::iterator it=imap.find(ikey);
        if(it!=imap.end())
        {
            imap.erase(it);
            return true;
        }
        return false;
    }

	template <typename KeyType,typename ValueType>
	static bool GetValueInMap(const map<KeyType,ValueType>& imap,const KeyType& ikey,ValueType& outValue)
    {
		typename map<KeyType,ValueType>::const_iterator it=imap.find(ikey);
        if(it!=imap.end())
        {
			outValue=it->second;
            return true;
        }
        return false;
    }
	template <typename KeyType,typename ValueType>
	static ValueType GetValueInMap(const map<KeyType,ValueType>& imap,const KeyType& ikey)
    {
		ValueType rlt;
		typename map<KeyType,ValueType>::const_iterator it=imap.find(ikey);
        if(it!=imap.end())
        {
			return it->second;
        }
        return rlt;
    }

	template <typename KeyType,typename ValueType>
	static bool GetKeyInMap(const map<KeyType,ValueType>& imap,const ValueType& iValue,KeyType& outkey)
    {
		typename map<KeyType,ValueType>::const_iterator it;
		for(it=imap.begin();it!=imap.end();it++)
		{
			if(it->second==iValue)
			{
				outkey=it->first;
				return true;
			}
		}
		return false;
    }

	template <typename KeyType,typename ValueType>
	static vector<ValueType> GetValuesInMap(const map<KeyType,ValueType>& imap)
    {
		vector<ValueType> outValues;
		typename map<KeyType,ValueType>::const_iterator it;
		for(it=imap.begin();it!=imap.end();it++)
			outValues.push_back(it->second);
        return outValues;
    }

	template <typename DataType>
	static void ConvertVector2Set(const vector<DataType>& sourvec,set<DataType>& rltset)
	{
		rltset.clear();
		for(vector<DataType>::const_iterator it=sourvec.begin();it!=sourvec.end();it++)
		{
			if(rltset.find(*it)==rltset.end())
				rltset.insert(*it);
		}
	}

	template <typename DataType>
	static void ConvertSet2Vector(const set<DataType>& sourset,vector<DataType>& rltvec)
	{
        rltvec.clear();
		for(set<DataType>::const_iterator it=sourset.begin();it!=sourset.end();it++)
            rltvec.push_back(*it);
	}

    template <typename DataType>
	static bool EraseInSet(set<DataType>& sset,const DataType& value)
	{
		set<DataType>::iterator it=sset.find(value);
		if(it==sset.end()) return false;
		sset.erase(it);
		return true;
	}

};


#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_TOOLS_ANSI_H__1215343F_84A5_4639_A71B_C83B93EE48B2__INCLUDED_)
