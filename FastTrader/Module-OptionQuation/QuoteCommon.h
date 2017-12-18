#pragma once
#pragma pack(push)
#pragma pack(1)
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
	PHRASE_DAY		= 24*60,
	PHRASE_WEEK		= 7*24*60,
	PHRASE_MONTH	= 31*24*60,
	PHRASE_USERDEFINE = 0
}; 
struct SKLine   //k�����ݽṹ��1���� �� ���߶���������ݽṹ
{
	struct tm  dwTime;              //utcʱ�� ��������ʱ�䡣 ���ʱ������ʵʱ���1900;����: dwTime.tm_year = 111;����111+1900 = 2011�� (������ṹ����������dword�����濪���鷳�˺ö�)
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
	{//�Ӵ�С����;
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
	{//�Ӵ�С����;
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
{//���㼼��ָ��ʱ���õ��Ĳ���:ʱ���������ڶ�ȡ���ش�Ĳ�ͬ���ڵĲ���;��Լ�������ڳɽ�������֮��Ĺ�ϵ
	EnumPhrase enumPhrase;		//ʱ������
	int		m_VolumeMultiple;	//��Լ����
};
























//////////////////////////////////////////////////////////////////////////
//һ�����Ի���������Ķ��壬�������Ǹ�Ŀ¼���ļ�
//InstitutionalTrading\CommonDef\CommonStruct.h

/*
enum  KLineDataCycle
{
	Minute_1 = 1,
	Minute_3 =3,
	Minute_5=5,
	Minute_10=10,
	Minute_15=15,
	Minute_30=30,
	Minute_60=60,
	Day_1,
	Week_1,
	Month_1,
};*/
typedef char _InstrumentID[31];/*
//K��
struct KLineData
{
	KLineDataCycle	eCycle;           //K������
	char			szDate[16];			   // ����yyyymmdd
	char			szTime[9];			  // ʱ��hh:mm:ss
	_InstrumentID	InstrumentID;     //��Լ����
	double          OpenPrice;			  //���̼�	
	double	        HighestPrice;         ///��߼�	
	double	        LowestPrice;          ///��ͼ�
	double	        ClosePrice;           /// ���̼�
	int         	IncreaseVolume;              ///�ɽ�������
	int         	TotalVolume;              ///�ɽ�������
	double          Turnover;			//�ɽ��������
	int            OpenInterest;				//�ֲ�������
	double         SettlementPrice;                //�����

};
///��ʱ��
struct TimeLineData
{
	char			szDate[16];			   // ����
	char			szTime[9];			  // ʱ��
	_InstrumentID	InstrumentID;     //��Լ����
	double	        ClosePrice;           /// ���̼�
	int         	Volume;              ///�ɽ�������
	double          Turnover;			//�ɽ��������
	int             OpenInterest;		//�ֲ�������

};*/
//��ѯK�ߡ���ʱ�߽ṹ
struct KLineQuery
{	
	time_t			        szStartTime;		// 
	time_t			        szEndTime;		//
	_InstrumentID	        InstrumentID;			//��Լ����
	EnumPhrase				nCycle;					//K������	
	int                     nDataType;				//1-K�����ݣ�2-����������
};
struct KLineBase   //k�����ݽṹ��1���� �� ���߶���������ݽṹ
{
	time_t dwTime;						//ʱ�䣬��1900��1��1�տ�ʼ������
	double fOpenPrice;					// ���̼�
	double fHighPrice;					// ��߼�
	double fLowPrice;					// ��ͼ�
	double fClosePrice;					// ���̼�
	double fTurnover;					//�ɽ����
	DWORD dwVolume;						//�ɽ���
	DWORD dwOpenInterest;				//�ֲ���
	KLineBase& operator=(const KLineBase& other)
	{
		dwTime			= other.dwTime;

		fOpenPrice      = other.fOpenPrice;
		fHighPrice		= other.fHighPrice;		
		fLowPrice		= other.fLowPrice;			
		fClosePrice		= other.fClosePrice;				
		fTurnover		= other.fTurnover;		
		dwVolume		= other.dwVolume;					
		dwOpenInterest	= other.dwOpenInterest;				
		//dJieSuan		= other.dJieSuan;      
		return *this;
	}
	bool operator<(const KLineBase&	src) const
	{//�Ӵ�С����;
		return dwTime > src.dwTime;
	}
} ; 

struct TLineBase //��ʱ���ݽṹ
{
	time_t dwTime;						//ʱ�䣬��1900��1��1�տ�ʼ������
	double fLastPrice;					// ���̼�
	double fTurnover;					//�ɽ����
	DWORD dwVolume;						//�ɽ���
	DWORD dwOpenInterest;				//�ֲ���
	TLineBase& operator=(const TLineBase& other)
	{
		dwTime			= other.dwTime;

		fLastPrice		= other.fLastPrice;				
		fTurnover		= other.fTurnover;		
		dwVolume		= other.dwVolume;					
		dwOpenInterest	= other.dwOpenInterest;				
		return *this;
	}
	bool operator<(const TLineBase&	src) const
	{//�Ӵ�С����;
		return dwTime > src.dwTime;
	}
};	
//////////////////////////////////////////////////////////////////////////
//�������ID���Ժ�ȥ��
#define  MAKE_CMDID(_base,_offset) \
	((int)( (int)(_base) << 16 ) + (int)(_offset))
#define  CMDID_BASE_HIWORD_QUOT						0x000F //�����λCMDID
#define Cmd_SubscribeQuotEvent_Req						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0001)	//���������¼�����
#define Cmd_SubscribeQuotEvent_Rsp						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0002)	//���������¼�����
#define Cmd_UnSubscribeQuotEvent_Req						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0003)	//�˶������¼�����
#define Cmd_UnSubscribeQuotEvent_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0004)	//�˶������¼�����
#define Cmd_QuotEvent_Push								    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0005)	//�����¼�����

#define Cmd_QueryQuotKLineData_Req						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0010)	//��ѯ����K���¼�����
#define Cmd_QueryQuotKLineData_Rsp						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0011)	//��ѯ����K���¼�����
#define Cmd_QueryQuotTimeLineData_Req						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0012)	//��ѯ�����ʱ���¼�����
#define Cmd_QueryQuotTimeLineData_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0013)	//��ѯ�����ʱ���¼�����

#define CF_ERROR_SUCCESS							0
