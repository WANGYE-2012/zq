// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TOOLS_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TOOLS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#pragma once

//#include "toolpublic.h"

// This class is exported from the tools.dll
class CTools 
{
public:
	CTools(void);

	//��ȫ·���ļ�����ȡ���������ļ���
	static bool GetPureFileName(char* pFileNameWithPath,char* pRltFileName,int RltSize);

	//��ȫ·���ļ�����ȡ���̷�����־��:
	static bool GetDiskSymbol(char* pFileName,char* pRltDiskSymbol,int RltSize);

	//��ȫ·���ļ�����ȡ��·���������̷�
	static bool GetFilePathWithoutDiskSymbol(char* pFileName,char* pRltFilePath,int RltSize);

	//��ȫ·���ļ�����ȡ��·�������̷�
	static bool GetFilePath(char* pFileName,char* pRltFilePath,int RltSize);

	//ɾ���ַ���ͷβ�Ŀո��Tab��\n��\r
	static void Trim_Space(char* pString);

	//ɾ���ַ���������б�ܺ����ע������
	static void Trim_Note(char* pString);

    //��Name=Value��ʽ���ַ�����ȡ��Name��Value, ���Value��˫�����������Ļ���ɾ��˫����
	static bool GetName_ValueFromString(char* pString,char* pRltName,char* pRltValue);

	//�ж�һ��ʱ���Ƿ���ָ����ʱ����ڡ�ʱ����hhmm�����α�ʾ
	static bool IsInTimePeriod(int iMins,int StartMins,int EndMins);

	//ȡ��ǰ�������ͺ�����
	static void GetTime(time_t *pSec,int *pMS);

	//	�ö��ַ���һ������Ļ����������һ���ַ���
	//	���ҵ����ַ������������ڻ�����������
	//	���Ҳ���������-1��pLastHead��pLastTail�ǰ���ָ���ַ�����������ֵ
	//	�����Ǵ�С����
	//	��Сд�޹�
	static int LookupStringInSequenceBuffer_memicmp(char *SeqBuf,int UnitNum,int UnitSize,char *pMem,int Len,int *pLastHead=NULL,int *pLastTail=NULL);

	//	�ö��ַ���һ������Ļ����������һ���ַ���
	//	���ҵ����ַ������������ڻ�����������
	//	���Ҳ���������-1��pLastHead��pLastTail�ǰ���ָ���ַ�����������ֵ
	//	�����Ǵ�С����
	//	��Сд���
	static int LookupStringInSequenceBuffer_memcmp(char *SeqBuf,int UnitNum,int UnitSize,char *pMem,int Len,int *pLastHead=NULL,int *pLastTail=NULL);

	//	��ð�ݷ��������������Ǵ�С����
	//	Buf��Ҫ����Ļ���������������UnitNum����Ԫ��ɣ�ÿ����Ԫ�ĳ���ΪUnitSize
	//	CmpOffΪҪ�ȽϵĴ��ڵ�Ԫ���λ�ã�CmpLenΪ�Ƚϵĳ���
	//	��Сд�޹�
	static void SetSequenceInBubble_memicmp(char *Buf,int UnitNum,int UnitSize,int CmpOff,int CmpLen);

	//	��ð�ݷ��������������Ǵ�С����
	//	Buf��Ҫ����Ļ���������������UnitNum����Ԫ��ɣ�ÿ����Ԫ�ĳ���ΪUnitSize
	//	CmpOffΪҪ�ȽϵĴ��ڵ�Ԫ���λ�ã�CmpLenΪ�Ƚϵĳ���
	//	��Сд���
	static void SetSequenceInBubble_memcmp(char *Buf,int UnitNum,int UnitSize,int CmpOff,int CmpLen);

	//	��4�ֽڶ���ķ�ʽȡһ���ڴ��DWORD������
	static DWORD GetCRC_dw(DWORD* pdw,int dwCount);

    //  ���ַ�����ʾ��ʱ��ת����utc��time_tʱ��
    //  pDate:yyyymmdd
    //  pTime:hhmmss �� hh:mm:ss
    //static time_t Get_time_t_utc_from_strTime(const char* pDate, const char* pTime);

	/// ��ʱ���ַ���תΪ����
	/// pTime:hhmmss �� hh:mm:ss
	static unsigned int CalculateTotalSeconds(const char* pTime);

    //  ��utc��time_tʱ��ת�����ַ�����ʾ��ʱ��
    //  pRltDate:yyyymmdd
    //  pRltTime:hh:mm:ss
    static void Get_strTime_from_utc(time_t iTime,char* pRltDate,char* pRltTime);

    //  ��utc��time_tʱ��ת���ɱ���ʱ��
    static void Get_DateTime_from_utc(time_t iTime,int* pRltYear=NULL,int* pRltMonth=NULL,int* pRltDay=NULL,int* pRltHour=NULL,int* pRltMinutes=NULL,int * pRltSecs=NULL);


    //  ���ַ�����ʾ��ʱ��ת���ɵ���00:00:00��ʼ������
    //  pTime:hhmmss �� hh:mm:ss
    static int Get_TodaySecs_from_strTime(char* pTime);


    //  ��������ӵ���β, ������ӵĳ���
    static int AppendIntToString(int iVal,char *pStr,int StrSize,char *pHeadStr,char *pTailStr);

    //  ��double��ӵ���β, ������ӵĳ���
    static int AppendDoubleToString(double dVal,char *pStr,int StrSize,char *pHeadStr,char *pTailStr);

    //  ��char��ӵ���β, ������ӵĳ���
    static int AppendCharToString(char cVal,char *pStr,int StrSize,char *pHeadStr,char *pTailStr);

    //  ��string��ӵ���β, ������ӵĳ���
    static int AppendStringToString(char *pSourStr,char *pStr,int StrSize,char *pHeadStr,char *pTailStr);

    //  ��string��ǰ����ӿո���string�ĳ���Ϊָ������
    static void AppendSpaceToString(char *pStr,int FinalLen);

	//  ��string��ǰ�����'0'��ʹstring�ĳ���Ϊָ������
	static void AppendZeroToString(char *pStr,int FinalLen);

    //  ��ip:port��ip port��ʽ�ĵ�ַ���ָ�Ϊ������IP����Port��
    static bool SplitIPAddrStr(const char* pAddr,char* pRltIP,int RltIPSize,char* pRltPort,int RltPortSize);

    //  �Ƚ��ڴ�飬���Ժ���Ŀո�
    static int MyMemcmpIgnoreTailSpace(const char* pMem1,const char* pMem2,int MemLen,int* pRealCmpLen=NULL);

    static void mytrimleft(std::string& str);
    static void mytrimright(std::string& str);
    static void mytrim(std::string& str);

    //��ȡһ���ַ���trim�����Ч��ʼλ�ú���Ч����
    static void mygettrimpos(const char* pstr,int&off,int&len);

    static void mymemcpy_trim(char* prlt,int rltsize,const char* psrc);

    //�Ƚ�������������1,0,-1
    static int mycompare_trynumeric(const wxString& str1,const wxString& str2);
    static int mycompare2_trynumeric(const char* str1,const char* str2);
    static int mycompare(const std::string& str1,const std::string& str2);
    static int mycompare2(const wxString& str1,const wxString& str2);

public:
	CTools(const CTools&);									
	CTools& operator= (const CTools&);				

};

