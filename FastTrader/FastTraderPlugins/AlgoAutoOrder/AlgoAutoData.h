#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "IniFile.h"
#include "Tools_Win32.h"
#include "Tools_AlgoTrading.h"

#include <math.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

//������Լ��������ini�ļ���
#define IniFileName_AlgoAutoData "AlgoAutoData.ini"

typedef struct ORDERLATEST
{
    PlatformStru_OrderInfo    orderInfo; 
    time_t                    time;  

}ORDERLATEST,*pORDERLATEST;

typedef struct ORDERWAITCANCEL
{
    BOOL    bSucess; 
    int     nOrderRef;     

    ORDERWAITCANCEL()
    {
        bSucess = FALSE;
        nOrderRef = -1;
    }

}ORDERWAITCANCEL,*pORDERWAITCANCEL;

struct ORDERPARAM
{
    string value;
    string valueFrom;
    string valueTo;
    int   type;
};
//�µ���Ϣ��װ
struct ORDERINFO 
{
    string			name;	            //��Լ����
    int				direction;			//����0:��1:��
    int				offsetID;		    //��ƽ��0:���� 1:ƽ�� 2:ƽ��
    double          price;	            //�۸�
    int             count;              //����
    int             index;              //����
};

//�µ������Ӧ��װ
struct ORDERRSP 
{
    bool            finish;             //�Ƿ�ɽ�
    string 		    orderRef;	        //��Լ����
    double          price;	            //�۸�
    int             count;              //����
    int             realCount;    

    ORDERLATEST     status;             //����״̬
    bool            cancle;             //�Ƿ�Ҫ��
    bool            bStatus;            //״̬�Ƿ�ı� 
    bool            bRspArrive;         //�µ��Ƿ��Ӧ

    ORDERRSP()
    {
        finish = false;
        cancle = false;
        bStatus = false;
        bRspArrive = false;
        price = 0.0f;
        count = 0;
    }

};

typedef struct 
{
    double low;
    double high;
    int index; 
}section;

typedef struct 
{
    int index;//0 ��λ 1 ����
    vector<string> data;
}WDDATA_PRICESPREAD;

typedef struct 
{
    int index;//0 �ٷֱ� 1 ����
    vector<string> data;
}WDDATA_ORDERCOUNT;

//�Ż�������Ӧ�Ĳ���
typedef struct SINGLEOPT
{
    int             Index;                          //����
    int             BackTime;                       //����ʱ������
    ORDERPARAM      PriceChange;                    //�۸�䶯��Χ
    string          Multiple;                       //���ٱ���
    int             OptMarketDegreed;               //�г������            
    WDDATA_PRICESPREAD OptSlidePriceSpread;       //�����۲�

    string Range;                                  //Detil list ��Χ��
    string Option;                                 //Detil list �Ż�������
    string History;                                //Detil list ��ʷ�۸���

}SINGLEOPT,pSINGLEOPT;
//�Ż�������Ϣ��װ
typedef struct OPTPARAM
{
    int             Index;                           //�Ż�����
    string          Name;                            //����
    vector<SINGLEOPT*> vecSingle;                    //�������еĲ���
    vector<section> vecSection;                      //�������еļ۸�����

    OPTPARAM()
    {
        Index = -1;
        vecSingle.clear();
        vecSection.clear();
    }

    ~OPTPARAM()
    {
        vector<SINGLEOPT*>::iterator iter = vecSingle.begin();
        for(;iter!=vecSingle.end();iter++)
        {
            SINGLEOPT* pSingle = (*iter);
            if (pSingle != NULL)
            {
                delete pSingle;
                pSingle = NULL;
            }
        }

        vecSingle.clear();
    }
}OPTPARAM,*POPTPARAM;


class CAlgoAutoData
{
public:
    CAlgoAutoData(void);
    CAlgoAutoData(const CAlgoAutoData& r);
    ~CAlgoAutoData(void);

    CAlgoAutoData& operator= (const CAlgoAutoData& r);

    bool ReadIni(string& strInstrumentID); 
    void SaveIni(void);
    BOOL AutoOrderCalculate();//�µ�����


    int             Volume;                         //ÿ���µ��ĳɽ���
    int             WaitVolume;                     //ÿ���µ��Ĺҵ���
    double			AverageQuan;				    //ƽ����Ȩ
    int             TotalVolume;                    //�ܳɽ���(ÿ���µ��ɽ���֮��)
    int             RetryCountSave;                 //��¼���Դ���
    int             RetryTotalTimeSave;             //��¼������ʱ��


    //��״ͼ���
    int				TradingTime;			        //���״���
    int				TradingMinute;					//����ʱ�η�Χ�����ķ�����
    vector<int>		vecTradedVolumePerMinute;       //��¼ÿ���ӳɽ�����
    vector<int>		vecTradedVolumePerTime;         //��¼ÿ���µ��ɽ�����

    //������
    string			InstrumentName;	                //��Լ����
    int				InstrumentDirection;			//����0:��1:��
    int				InstrumentOffsetID;		    	//��ƽ��0:���� 1:ƽ�� 2��ƽ��
    string          InstrumentCount;	            //����������
    string          LimitPrice;                     //�޼�               
    CTime           StartTime;                      //���׿�ʼʱ��
    CTime           EndTime;                        //���׽���ʱ��
    int             MarketDegreed;                  //�г������
    ORDERPARAM      OrderCount;                     //ÿ���µ�����
//     ORDERPARAM      ResetCount;                     //ÿ����������
    ORDERPARAM      BasicPrice;                     //���̼۸�
    ORDERPARAM      IdleTime;                       //����ʱ��
    ORDERPARAM      ExecuteTime;                    //ִ��ʱ��
    ORDERPARAM      RetryTotalTime;                 //��ʱ������ʱ��
    string          RetryCount;                     //��ʱ���Դ���
    bool            bActiceOverTime;                //�Ƿ�������ʱ���Դ���
    bool            bActiceTotalTime;               //�Ƿ�������ʱ������ʱ��
    bool            bActiceOpt;                     //�Ƿ������Ż�
    int             OptimizeIndex;                  //�Ż�����

//     bool            bMultiPrice;                    //�Ƿ����ö൵�۸�
//     bool            bPatchOrder;                    //�Ƿ��ǲ���
    WDDATA_PRICESPREAD  WDDATA_PriceSpread;         //�����۲�
//     WDDATA_ORDERCOUNT   WDDATA_OrderCount;          //��Ӧ�µ���


    //������
    int             NewVolume;                      //�ֽ�����
    int             BidVolume;                      //����
    int             AskVolume;                      //����
    double          LatestPrice;                    //���¼�
    double          PriceTick;                      //��С�䶯��λ
   
    PlatformStru_Position m_tBuyPosition;             // ��ֲ���Ϣ
    PlatformStru_Position m_tSellPosition;             // ���ֲ���Ϣ

    //������
    int             OrderCountValue;                //ÿ���µ��������ڰٷֱ�ʱ��
    int             PatchOrderCountValue;           //����ʵ���µ���
//     int             ResetCountValue;             //ÿ�����õ��������ڰٷֱ�ʱ��
    BOOL            bIdleTime;
    BOOL            bExecuteTime;
    int             IdleTimeValue;                  //��
    int             ExecuteTimeValue;               //��
    int             RetryTotalTimeValue;            //��
    // ��¼�µ�����
    int m_nOrderCount;

    vector<ORDERINFO>    vecOrderInfo;              //������µ�
    vector<ORDERINFO>    vecOrderActiveInfo;        //�Ż��������µ�
    vector<OPTPARAM*>    vecOptInfo;                //������Ż�����

    //������Ӧ
    vector<ORDERRSP>            vecOrderRSP;        //��¼�����µ��ı���

private:       
    void AnalysisData(vector<string>& vec, char *pBuf);//��ȡ�����е�����
    int  DfxRound(double dValue);//��������
};
