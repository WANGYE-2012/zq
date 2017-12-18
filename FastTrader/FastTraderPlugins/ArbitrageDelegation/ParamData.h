#pragma once

#include "KernelStruct.h"
#include "IniFile.h"
#include "Tools_Win32.h"

#include <string>
using namespace std;


//������Լ��������ini�ļ���
#define LogFileName_ArbitrageDelegation "ArbitrageDelegation.log"
#define IniFileName_ArbitrageDelegation "ArbitrageDelegation.ini"
#define Node_ArbitrageDelegation        "ArbitrageDelegation"

class CParamData
{

public:
    CParamData(void);
    CParamData& operator =(CParamData& other);
    ~CParamData(void);


    /*����*/
    void SetBandPrice(int& text);
    void WriteLog();

public:    

    bool            Param_RevokeFlag;         //�Ƿ񳷵�
    bool            Param_TriggerFlag;        //�Ƿ񴥷�

    int             Param_ID;                 //��ˮ��
    int				Param_OffsetID;			  //��ƽ��0:���֣�1:ƽ�֣�2:ƽ
    int				Param_ArbitrageType;	  //�������ͣ�0:�۲���С����1:�۲���������
    double          Param_TriggerPrice;       //�����۲�
    double          Param_DeviatePrice;       //ƫ����
    CTime           Param_ValidateDay;        //��Ч��
    string          Param_ValidateDayText;    //��Ч��(����ת��)
    CTime           Param_ValidateCreateDay;  //��������������

    //��Լ1
    string			Order_Instrument1;	      //��Լ����
    int				Order_Direction1;		  //����0:��1:��
    int             Order_Count1;             //����
    double          Order_Price1;             //�µ�ί�м۸�
    double          Order_DelegatePrice1;     //ί�м۲�
    double          Order_UpperPrice1;        //��ͣ��
    double          Order_LowestPrice1;       //��ͣ��

    //��Լ2
    string			Order_Instrument2;	      //��Լ����
    int				Order_Direction2;		  //����0:��1:��
    int             Order_Count2;             //����
    double          Order_Price2;             //�µ�ί�м۸�
    double          Order_DelegatePrice2;     //ί�м۲�
    double          Order_UpperPrice2;        //��ͣ��
    double          Order_LowestPrice2;       //��ͣ��

    //Orderkey1
    string          OrderKey_Account1;
    string          OrderKey_Instrument1;
    int             OrderKey_FrontID1;
    int             OrderKey_SessionID1;
    string          OrderKey_OrderRef1;

    //Orderkey2
    string          OrderKey_Account2;
    string          OrderKey_Instrument2;
    int             OrderKey_FrontID2;
    int             OrderKey_SessionID2;
    string          OrderKey_OrderRef2;

};