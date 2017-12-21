#pragma once
#include <windows.h>
#include "winbase.h"
#include "CommonPkg.h"
#include "fttcommdef.h"
#include "socketbase.h"
#define  MESSAGE_GETDATA    			WM_USER + 1018
#define  MESSAGE_GETSTATISITIC   		WM_USER + 1019
#define  MESSAGE_GETSTATISITIC_PUSH   	WM_USER + 1020
#define  MESSAGE_HEARTBEATRSP		    WM_USER + 1021



#define MONITOR_GETSTATISITIC			0x00001001  //����ͳ����������
#define MONITOR_GETSTATISITIC_Rsp		0x00001002  //�õ�ͳ����������
#define MONITOR_GETSTATISITIC_Push		0x00001003  //�õ�ͳ����������(ÿ����)


#define MONITOR_SENDCALCDATA_Rsp		0x00001100  //���ͼ������ݵ�����նˣ����У�

struct RecvData 
{
	Stru_UniPkgUDPHead head;	//��ͷ
	void* pData;				//���յ�����������
};
struct RecvDataMsg//�������Ϣ
{
	Stru_UniPkgUDPHead head;	//��ͷ
	void* pData;				//���յ�����������
	Stru_IPAddress RemoteAddr;
	SOCKET hSocket;
};

enum DataStatus
{
	UPRECEV		= 1,//1�����н���
	UPSEND		= 2,//2�����з���
	DOWNRECEV	= 3,//3�����н���
	DOWNSEND    = 4,//4�����з���
	
	UPSENDFAIL    = 5,// 5�����з���ʧ��
	DOWNSENDFAILE = 6,//6: ���з���ʧ��


};
//����˵ı���
struct ServerOrder
{//��Ӧ������ṹStru_UniPkgUDPHead
	unsigned int cmdid;		//FFT_API_START��ʼ�����У�FFT_SPI_START��ʼ������
	int			 len;		
	unsigned int seq;	
	unsigned int userdata1;	//ҵ������ID
	unsigned int userdata2;	//1�����н��գ�2�����з��ͣ�3�����н��գ�4�����з��ͣ�5�����з���ʧ��6�����з���ʧ��
	unsigned int userdata3; //��������seq
	unsigned int userdata4; //�ͻ���socket
	long		 nUTCTime;	//utc����
	long		 nMiliSecond;//������
};
/*
//�ͻ��˵ı���
struct ClientOrder 
{
	int nCmdID;				//������
	int nSeq;				//����Ψһ��
	char Direction;			// '0'���У���1������
	time_t tmSend;			//����ʱ��
	time_t tmRecv;			//����ʱ��
	time_t tmDay;			//����
	ClientOrder()
	{
		memset(this,0,sizeof(*this));
	}
};*/

//
struct StatisticsData
{
	time_t tmTime; //ÿ���ʱ��

	DWORD dwUpRecv;//���н�������
	DWORD dwUpSend;//��������

	DWORD dwDownRecv;//���н�������
	DWORD dwDownSend;//��������
	
	time_t dwUpDelay;//����ƽ���ӳ�
	time_t dwDownDelay;//����ƽ���ӳ�

	StatisticsData()
	{
		memset(this,0,sizeof(*this));
	}
	bool operator< (const StatisticsData& r) const
	{
		if(tmTime<r.tmTime)
			return true;
		if(tmTime>r.tmTime)
			return false;

		if(dwUpRecv<r.dwUpRecv)
			return true;
		if(dwUpRecv>r.dwUpRecv)
			return false;
		
		if(dwUpSend<r.dwUpSend)
			return true;
		if(dwUpSend>r.dwUpSend)
			return false;

		if(dwDownRecv<r.dwDownRecv)
			return true;
		if(dwDownRecv>r.dwDownRecv)
			return false;

		if(dwDownSend<r.dwDownSend)
			return true;
		if(dwDownSend>r.dwDownSend)
			return false;

		if(dwUpDelay<r.dwUpDelay)
			return true;
		if(dwUpDelay>r.dwUpDelay)
			return false;

		if(dwDownDelay<r.dwDownDelay)
			return true;
		if(dwDownDelay>r.dwDownDelay)
			return false;

		return false;
	}
};
#pragma pack(push)
#pragma pack(1)

//typedef struct {
//	Stru_UniPkgHead base;
//	int nUtcTime;
//	int nMS;
//} Stru_UDPPkgHead;

typedef struct {
	Stru_UniPkgHead base;
	int nServSeq;
	int nSocket;
} Stru_ThreadData;

typedef struct {
	int CpuUsage;		// ��ǰcpuռ���ʣ�ǧ����
	INT64 Memory;		// ��ǰ�ڴ�ʹ����
	INT64 DiskSpace;	// �����������Ӳ�̵�ǰ���ÿռ�
} Stru_Watch;

#pragma pack(pop)

struct StatData
{
	struct tm dwTime; //ÿ���ʱ��

	DWORD dwUpRecv;//���н�������
	DWORD dwUpSend;//��������

	DWORD dwDownRecv;//���н�������
	DWORD dwDownSend;//��������

	DWORD dwUpDelay;//�������ӳ�
	DWORD dwDownDelay;//�������ӳ�

	StatData()
	{
		memset(this,0,sizeof(*this));
	}
	bool operator< (const StatData& r) const
	{
		if(dwTime.tm_year > r.dwTime.tm_year)
			return true;
		else if(dwTime.tm_year < r.dwTime.tm_year)
			return false;

		if(dwTime.tm_mon > r.dwTime.tm_mon)
			return true;
		else if(dwTime.tm_mon < r.dwTime.tm_mon)
			return false;

		if(dwTime.tm_mday > r.dwTime.tm_mday)
			return true;
		else if(dwTime.tm_mday < r.dwTime.tm_mday)
			return false;

		if(dwTime.tm_hour > r.dwTime.tm_hour)
			return true;
		else if(dwTime.tm_hour < r.dwTime.tm_hour)
			return false;

		if(dwTime.tm_min > r.dwTime.tm_min)
			return true;
		else if(dwTime.tm_min < r.dwTime.tm_min)
			return false;

		if(dwTime.tm_sec > r.dwTime.tm_sec)
			return true;
		else if(dwTime.tm_sec < r.dwTime.tm_sec)
			return false;
		if(dwUpRecv<r.dwUpRecv)
			return true;
		if(dwUpRecv>r.dwUpRecv)
			return false;

		if(dwUpSend<r.dwUpSend)
			return true;
		if(dwUpSend>r.dwUpSend)
			return false;

		if(dwDownRecv<r.dwDownRecv)
			return true;
		if(dwDownRecv>r.dwDownRecv)
			return false;

		if(dwDownSend<r.dwDownSend)
			return true;
		if(dwDownSend>r.dwDownSend)
			return false;

		if(dwUpDelay<r.dwUpDelay)
			return true;
		if(dwUpDelay>r.dwUpDelay)
			return false;

		if(dwDownDelay<r.dwDownDelay)
			return true;
		if(dwDownDelay>r.dwDownDelay)
			return false;

		return false;
	}
};/*
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
  }
  };	// ��ʱͼ���ݽṹ*/
struct STItem
{
	int nItem;
	//STLine* pItem;
	MSG* pMsg; 
	bool bIsMouse;   //�����Ϣ
	bool bIsKeyBoard;//������Ϣ
	//CPoint wxPt;    //��굱ǰλ�ã�������k�����ƶ�����Ҫ�����������ڵ�����ƶ�λ�ã�x����һ���ģ�
	STItem()
	{
		nItem = -1;
		//pItem = NULL;
		pMsg  = NULL;
		bIsKeyBoard = false;
		bIsMouse    = false;	
	//	wxPt = CPoint(-1,-1);
	}
	STItem& operator = (STItem & other)
	{
		if(this == & other)
			return *this;

		nItem  = other.nItem;
		//	pItem     = other.pItem;
		pMsg      = other.pMsg;
		bIsKeyBoard = other.bIsMouse;
		bIsMouse    = other.bIsMouse;
	//	wxPt		= other.wxPt;
	}
};
#define MAX_VALUE_DEFAULT   100000000
#define MIN_VALUE_DEFAULT  -100000000
enum eMouseModes 
{ 
	MOUSE_NOTHING ,
	MOUSE_OVER_ITEM_DIVIDE,
	MOUSE_PREPARE_DRAGITEM, 	
};