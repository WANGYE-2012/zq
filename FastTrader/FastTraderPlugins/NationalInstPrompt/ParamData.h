#pragma once

#include "IniFile.h"
#include "Tools_Win32.h"

#include <string>
using namespace std;


//������Լ��������ini�ļ���
#define LogFileName_NationalInstPrompt "NationalInstPrompt.log"
#define IniFileName_NationalInstPrompt "NationalInstPrompt.ini"
#define Node_NationalInstPrompt        "NationalInstPrompt"

class CParamData
{

public:
    CParamData(void);
    CParamData& operator=(CParamData& other);
    ~CParamData(void);

    //����㷨
    bool Calculate(double fLastQuotPrice);

    /*����*/
    void SetBandPrice(int& text);
    void WriteLog();

public:

    bool            TurnOn;                   //�����Ƿ�����

    //����
    double          Param_QuotPrice;          //�������¼�

    //����������
    string          Param_BandCode;           //��ȯ����
    string          Param_BandInstrument;     //��ȯ��Լ
    double          Param_IRR;                //IRR
    double          Param_InterestRates;      //Ʊ������
    double          Param_BandPrice;          //��ȯ�۸�
    double          Param_TransferFactor;     //ת������
    int             Param_YearInterestCount;  //�긶Ϣ����
    CTime           Param_PreDelivDate;       //��һ��Ϣ��
    CTime           Param_NextDelivDate;      //��һ��Ϣ��
    CTime           Param_DelivDate;          //������
    string          Param_PreDelivDateText;   //��һ��Ϣ��(����ʱʹ��)
    string          Param_NextDelivDateText;  //��һ��Ϣ��(����ʱʹ��)
    string          Param_DelivDateText;      //������(����ʱʹ��)


    //����������
    bool            Order_Alarm;              //�Ƿ񱨾�
    bool            Order_AutoOrder;          //�Ƿ��Զ��µ�
    string			Order_Instrument;	      //��Լ����
    int				Order_Direction;		  //����0:��1:��
    int				Order_OffsetID;			  //��ƽ��0:���֣�1:ƽ�֣�2:ƽ��
    int             Order_Count;              //����
    int             Order_Price;              //�۸�, 0:�м�



    //�����
    double          Interest;                 //Interest
    double          NeedInterest;             //Ӧ����Ϣ
    double          InvoicePrice;             //��Ʊ�۸�
    double          Result_BandTargetPrice;   //��ȯĿ��۸�

};
