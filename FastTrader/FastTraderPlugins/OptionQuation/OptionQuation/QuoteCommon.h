#pragma once

#include <windows.h>
#include <time.h>
#include <fstream>
#include <map>
#include <vector>
#define MAX_VALUE_DEFAULT   100000000
#define MIN_VALUE_DEFAULT  -100000000
#define COLOR_MAX   9
#define	 TIMER_2			1
#define	 TIMER_1			0

#define  MESSAGE_LEFTDBCLICK		WM_USER + 1011
#define  MESSAGE_RETURN     		WM_USER + 1012
#define  MESSAGE_QutoInTime    		WM_USER + 1018
#define  MESSAGE_CONNECTSUCCESS   	WM_USER + 1019
#define  MESSAGE_CONNECTFAIL   		WM_USER + 1020
#define  MESSAGE_QutoTLineHistory   WM_USER + 1021
#define  MESSAGE_QutoKLineHistory   WM_USER + 1023

#define  MESSAGE_CHANGEVIEW   		WM_USER + 1026
#define  MESSAGE_CHANGETimeSPAN		WM_USER + 1027

#define  MESSAGE_INDEX_EXPRESS		WM_USER + 1037
#define  MESSAGE_INDEX_CONFIG		WM_USER + 1038

#define  MESSAGE_CONFIG_CHANGE		WM_USER + 1039
enum eMouseModes 
{ 
	MOUSE_NOTHING ,
	MOUSE_OVER_ITEM_DIVIDE,
	MOUSE_PREPARE_DRAGITEM, 	
};
enum EnumPhrase
{
	PHRASE_1MIN		= 1,
	PHRASE_5MIN		= 5,
	PHRASE_15MIN	= 15,
	PHRASE_30MIN	= 30,
	PHRASE_60MIN	= 60,
	PHRASE_DAY,
	PHRASE_WEEK,
	PHRASE_MONTH,
	PHRASE_USERDEFINE
}; 
struct SKLine   //k�����ݽṹ��1���� �� ���߶���������ݽṹ
{
	struct tm  dwTime;              //utcʱ�� ��������ʱ�䡣 ���ʱ������ʵʱ���1900������: dwTime.tm_year = 111;����111+1900 = 2011�� (������ṹ����������dword�����濪���鷳�˺ö�)
	double fOpenPrice;				// ���̼�
	double fHighPrice;				// ��߼�
	double fLowPrice;				// ��ͼ�
	double fClosePrice;				// ���̼�
	double fSumTradeVolume;			//�ɽ����
	DWORD dwVolume;					//�ɽ���
	DWORD dwHoldVolume;				//�ֲ���
	double dJieSuan;                //�����
	SKLine& operator=(const SKLine& other)
	{
		dwTime.tm_hour = other.dwTime.tm_hour;
		dwTime.tm_isdst = other.dwTime.tm_isdst;
		dwTime.tm_mday  = other.dwTime.tm_mday;
		dwTime.tm_min   = other.dwTime.tm_min;
		dwTime.tm_mon   = other.dwTime.tm_mon;
		dwTime.tm_sec   = other.dwTime.tm_sec;
		dwTime.tm_wday  = other.dwTime.tm_wday;
		dwTime.tm_yday  = other.dwTime.tm_yday;
		dwTime.tm_year  = other.dwTime.tm_year;

		fOpenPrice      = other.fOpenPrice;
		fHighPrice		= other.fHighPrice;		
		fLowPrice		= other.fLowPrice;			
		fClosePrice		= other.fClosePrice;				
		fSumTradeVolume	= other.fSumTradeVolume;		
		dwVolume		= other.dwVolume;					
		dwHoldVolume	= other.dwHoldVolume;				
		dJieSuan		= other.dJieSuan;      
		return *this;
	}
	bool operator<(const SKLine&	src) const
	{//�Ӵ�С����
		if(dwTime.tm_year > src.dwTime.tm_year)
			return true;
		else if(dwTime.tm_year < src.dwTime.tm_year)
			return false;

		if(dwTime.tm_mon > src.dwTime.tm_mon)
			return true;
		else if(dwTime.tm_mon < src.dwTime.tm_mon)
			return false;

		if(dwTime.tm_mday > src.dwTime.tm_mday)
			return true;
		else if(dwTime.tm_mday < src.dwTime.tm_mday)
			return false;

		if(dwTime.tm_hour > src.dwTime.tm_hour)
			return true;
		else if(dwTime.tm_hour < src.dwTime.tm_hour)
			return false;

		if(dwTime.tm_min > src.dwTime.tm_min)
			return true;
		else if(dwTime.tm_min < src.dwTime.tm_min)
			return false;

		if(dwTime.tm_sec > src.dwTime.tm_sec)
			return true;
		else if(dwTime.tm_sec < src.dwTime.tm_sec)
			return false;

		return false;
	}
} ;	// K��ͼ���ݽṹ  


struct STLine //��ʱ���ݽṹ
{
	struct tm  dwTime;               //ʱ��
	double fLastPrice;				 //�۸�
	DWORD dwVolume;					 //�ɽ���
	double dwVolumeMoney;			 //�ɽ����
	DWORD dwHoldVolume;				 //�ֲ���

	STLine& operator=(const STLine& other)
	{
		dwTime.tm_hour = other.dwTime.tm_hour;
		dwTime.tm_isdst = other.dwTime.tm_isdst;
		dwTime.tm_mday  = other.dwTime.tm_mday;
		dwTime.tm_min   = other.dwTime.tm_min;
		dwTime.tm_mon   = other.dwTime.tm_mon;
		dwTime.tm_sec   = other.dwTime.tm_sec;
		dwTime.tm_wday  = other.dwTime.tm_wday;
		dwTime.tm_yday  = other.dwTime.tm_yday;
		dwTime.tm_year  = other.dwTime.tm_year;

		fLastPrice      = other.fLastPrice;
		dwVolume		= other.dwVolume;
		dwVolumeMoney   = other.dwVolumeMoney;
		dwHoldVolume	= other.dwHoldVolume;
		return *this;
	}
	bool operator<(const STLine&	src) const
	{//�Ӵ�С����
		if(dwTime.tm_year > src.dwTime.tm_year)
			return true;
		else if(dwTime.tm_year < src.dwTime.tm_year)
			return false;

		if(dwTime.tm_mon > src.dwTime.tm_mon)
			return true;
		else if(dwTime.tm_mon < src.dwTime.tm_mon)
			return false;

		if(dwTime.tm_mday > src.dwTime.tm_mday)
			return true;
		else if(dwTime.tm_mday < src.dwTime.tm_mday)
			return false;

		if(dwTime.tm_hour > src.dwTime.tm_hour)
			return true;
		else if(dwTime.tm_hour < src.dwTime.tm_hour)
			return false;

		if(dwTime.tm_min > src.dwTime.tm_min)
			return true;
		else if(dwTime.tm_min < src.dwTime.tm_min)
			return false;

		if(dwTime.tm_sec > src.dwTime.tm_sec)
			return true;
		else if(dwTime.tm_sec < src.dwTime.tm_sec)
			return false;

		return false;
	}/*
	bool operator<(const STLine&	src) const
	{
		if(dwTime.tm_year < src.dwTime.tm_year)
			return true;
		else if(dwTime.tm_year > src.dwTime.tm_year)
			return false;
		
		if(dwTime.tm_mon < src.dwTime.tm_mon)
			return true;
		else if(dwTime.tm_mon > src.dwTime.tm_mon)
			return false;

		if(dwTime.tm_mday < src.dwTime.tm_mday)
			return true;
		else if(dwTime.tm_mday > src.dwTime.tm_mday)
			return false;

		if(dwTime.tm_hour < src.dwTime.tm_hour)
			return true;
		else if(dwTime.tm_hour > src.dwTime.tm_hour)
			return false;

		if(dwTime.tm_min < src.dwTime.tm_min)
			return true;
		else if(dwTime.tm_min > src.dwTime.tm_min)
			return false;

		if(dwTime.tm_sec < src.dwTime.tm_sec)
			return true;
		else if(dwTime.tm_sec > src.dwTime.tm_sec)
			return false;

		return false;
	}*/
};	// ��ʱͼ���ݽṹ

struct STItem
{
	int nItem;
	STLine* pItem;
	MSG* pMsg; 
	bool bIsMouse;   //�����Ϣ
	bool bIsKeyBoard;//������Ϣ
	CPoint wxPt;    //��굱ǰλ�ã�������k�����ƶ�����Ҫ�����������ڵ�����ƶ�λ�ã�x����һ���ģ�
	STItem()
	{
		nItem = -1;
		pItem = NULL;
		pMsg  = NULL;
		bIsKeyBoard = false;
		bIsMouse    = false;	
		wxPt = CPoint(-1,-1);
	}
	STItem& operator = (STItem & other)
	{
		if(this == & other)
			return *this;

		nItem  = other.nItem;
		pItem     = other.pItem;
		pMsg      = other.pMsg;
		bIsKeyBoard = other.bIsMouse;
		bIsMouse    = other.bIsMouse;
		wxPt		= other.wxPt;
	}
};
struct STechCalcPara
{//���㼼��ָ��ʱ���õ��Ĳ�����ʱ���������ڶ�ȡ���ش�Ĳ�ͬ���ڵĲ�������Լ�������ڳɽ�������֮��Ĺ�ϵ
	EnumPhrase enumPhrase;		//ʱ������
	int		m_VolumeMultiple;	//��Լ����
};