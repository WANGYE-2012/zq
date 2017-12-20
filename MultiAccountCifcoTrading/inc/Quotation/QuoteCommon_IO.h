#pragma once
/*/////////////////////////////////////////////////////////////////////////
���ͷ�ļ�������Ҫ���ڽ���IO����ʱ���õ����ݽṹ����ʡ�洢�Լ���ȡ�ļ���ʱ��

/////////////////////////////////////////////////////////////////////////*/

struct SKLine_IO   //k�����ݽṹ��1���� �� ���߶���������ݽṹ
{
	DWORD  dwTime;                  //utcʱ�� ��������ʱ�䡣 ���ʱ������ʵʱ���1900������: dwTime.tm_year = 111;����111+1900 = 2011�� (������ṹ����������dword�����濪���鷳�˺ö�)
	double fOpenPrice;				// ���̼�
	double fHighPrice;				// ��߼�
	double fLowPrice;				// ��ͼ�
	double fClosePrice;				// ���̼�
	double fSumTradeVolume;			//�ɽ����
	DWORD dwVolume;					//�ɽ���
	DWORD dwHoldVolume;				//�ֲ���
	double dJieSuan;                //�����
	SKLine_IO& operator=(const SKLine_IO& other)
	{
		dwTime			 = other.dwTime;
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
	
} ;	

struct STLine_IO 
{
	DWORD dwTime;               //ʱ��
	double fLastPrice;				 //�۸�
	DWORD dwVolume;					 //�ɽ���
	double dwVolumeMoney;			 //�ɽ����
	DWORD dwHoldVolume;				 //�ֲ���
	
	STLine_IO& operator=(const STLine_IO& other)
	{
		dwTime			= other.dwTime;
		fLastPrice      = other.fLastPrice;
		dwVolume		= other.dwVolume;
		dwVolumeMoney   = other.dwVolumeMoney;
		dwHoldVolume	= other.dwHoldVolume;

		return *this;
	}
};	
struct SQIntime_IO                      //ʵʱ��������
{
	DWORD	dwTime;							//ʱ��
	double	fLastPrice;						    //�۸�
	DWORD	dwVolume;						    //�ɽ���
	DWORD	dwOpenVolume;					    //����
	DWORD	dwCloseVolume;					    //ƽ��
	int     nDesc;                              //0:�տ�; 1:�࿪; 2:��ƽ; 3:��ƽ; 4:�໻; 5:�ջ�;


	SQIntime_IO& operator=(const SQIntime_IO& other)
	{
		dwTime			= other.dwTime;
		fLastPrice      = other.fLastPrice;
		dwVolume		= other.dwVolume;
		dwOpenVolume	= other.dwOpenVolume;
		dwCloseVolume	= other.dwCloseVolume;
		nDesc         = other.nDesc;
		return *this;
	}
};