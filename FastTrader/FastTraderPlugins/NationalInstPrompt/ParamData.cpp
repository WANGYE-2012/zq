#include "StdAfx.h"
#include "ParamData.h"
#include "LogFile.h"

CParamData::CParamData(void)
{
    TurnOn = false;

    //���������� 
    Param_QuotPrice = 0.0f;
    Param_IRR = 0.0f;
    Param_InterestRates = 0.0f;
    Param_BandPrice = 0.0f;
    Param_TransferFactor = 0.0f;
    Param_YearInterestCount = 0;
    Param_PreDelivDate = CTime::GetCurrentTime();
    Param_NextDelivDate = CTime::GetCurrentTime();
    Param_DelivDate = CTime::GetCurrentTime();

    //����������
    Order_Alarm = false;
    Order_AutoOrder = false;
    Order_Direction = 0;
    Order_OffsetID = 0;
    Order_Price = 0;
    Order_Count = 0;

    //�����
    Interest = 0.0f;
    NeedInterest = 0.0f;
    InvoicePrice = 0.0f;
    Result_BandTargetPrice = 0.0f;
}

CParamData& CParamData::operator=(CParamData& other)
{
    //����������
    Param_BandCode = other.Param_BandCode;
    Param_BandInstrument = other.Param_BandInstrument;
    Param_QuotPrice = other.Param_QuotPrice;
    Param_IRR = other.Param_IRR;
    Param_InterestRates = other.Param_InterestRates;
    Param_BandPrice = other.Param_BandPrice;
    Param_TransferFactor = other.Param_TransferFactor;
    Param_YearInterestCount = other.Param_YearInterestCount;
    Param_PreDelivDate = other.Param_PreDelivDate;
    Param_NextDelivDate = other.Param_NextDelivDate;
    Param_DelivDate = other.Param_DelivDate;


    //����������
    Order_Alarm = other.Order_Alarm;
    Order_AutoOrder = other.Order_AutoOrder;
    Order_Instrument = other.Order_Instrument;
    Order_Direction = other.Order_Direction;
    Order_OffsetID = other.Order_OffsetID;
    Order_Price = other.Order_Price;
    Order_Count = other.Order_Count;

    //�����
    Result_BandTargetPrice = other.Result_BandTargetPrice;

    return *this;
}

CParamData::~CParamData(void)
{
}

bool CParamData::Calculate(double fLastQuotPrice)
{
    Param_QuotPrice = fLastQuotPrice;

    CTime curDelivDate = CTime::GetCurrentTime();
  
    Interest = ((Param_DelivDate - curDelivDate).GetDays() + 1)*Param_InterestRates*100.0/365;
    NeedInterest = ((curDelivDate - Param_PreDelivDate).GetDays())*Param_InterestRates*100.0/365;
    InvoicePrice = Param_QuotPrice*Param_TransferFactor + Interest;

    double factor1 = Param_InterestRates*100.0/Param_YearInterestCount;
    int factor2 = (int)(Param_DelivDate - Param_NextDelivDate).GetDays() + 1;
    int factor3 = (int)(Param_DelivDate - curDelivDate).GetDays() + 1;

    //�������ս��
    if(Param_DelivDate - Param_NextDelivDate < 0)
        Result_BandTargetPrice = 365*InvoicePrice / (Param_IRR*factor3 + 365) - NeedInterest;
    else 
        Result_BandTargetPrice = (365*(InvoicePrice + factor1) + Param_IRR*factor1*factor2)/(Param_IRR*factor3 + 365) - NeedInterest;

    //WriteLog();

    bool bTrggle = ( Param_BandPrice - Result_BandTargetPrice > 0.00000001 );
    return !bTrggle;
}

/*����*/
void CParamData::SetBandPrice(int& text)
{
    Param_BandPrice += text;
    text = 0;
}

void CParamData::WriteLog()
{
    //��ȡ����·��
    char PathFileName[512];
    memset(PathFileName,0,sizeof(PathFileName));
    CTools_Win32::GetMainPath(PathFileName,sizeof(PathFileName)-1-strlen(LogFileName_NationalInstPrompt));
    strcat(PathFileName,LogFileName_NationalInstPrompt);    

    char buf[256];
    memset(buf,0,sizeof(buf));
    sprintf(buf,"��ȯ����=%s, Interest=%.6f, Ӧ����Ϣ=%.6f, ��Ʊ�۸�=%.6f, ��ȯĿ��۸�=%.6f, �����=%.6f, ��ȯ�۸�=%.6f\r\n",
            Param_BandCode.c_str(),Interest,NeedInterest,InvoicePrice,Result_BandTargetPrice,Param_QuotPrice,Param_BandPrice);

    CLogFile log(PathFileName);
    log.Log(buf);
}

