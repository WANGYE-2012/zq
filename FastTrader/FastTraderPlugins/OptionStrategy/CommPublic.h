#pragma once
#include <math.h>
#include <algorithm> 
#include <vector>
#include <map>
#include <set>
using namespace std;

#include "KernelStruct.h"
#include "orderDefines.h"
#include "OrderFunc.h"
#include "LogFile.h"
#include "easymutex.h"
#include "easytime.h"
#include "easytimespan.hpp"


//#define _LOG
#define LogFileName_OptionStrategy "OptionStrategy.log"

#define  MAKE_CMDID(_base,_offset) \
    ((int)( (int)(_base) << 16 ) + (int)(_offset))

#define  CMDID_BASE_HIWORD_QUOT						        0x000F //�����λCMDID
#define Cmd_QueryVolatilityData_Req                         MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0016)	//��ѯ����������
#define Cmd_QueryVolatilityData_Rsp                         MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0017)	//��ѯ�����ʷ���

#pragma pack(push)
#pragma pack(1)
struct Volatility 
{
    ///������
    TThostFtdcDateType	TradingDay;
    ///��Լ����
    InstrumentIDType	InstrumentID;
    ///20�첨����
    TThostFtdcPriceType	VolatilityTwenty;
    ///60�첨����
    TThostFtdcPriceType	VolatilitySixty;
};

#include "PLProbability.h"

//User message define
#define WM_USER_QUOT_UPDATE         WM_USER+600
#define WM_USER_VOLATILITY_UPDATE   WM_USER+700
#define WM_VOLATILITY_RESET         WM_USER+800
#define WM_USER_POSITION_UPDATE     WM_USER+900
#define WM_USER_EXPIREDATE_UPDATE   WM_USER+1000

enum eStrategyID
{
    StrategyID_Rose,					//����
    StrategyID_Crash,					//���
    StrategyID_Peakback,				//��������
    StrategyID_Bottomout,				//���׻���
    StrategyID_Breakout_Straddle,			//ͻ��-�����ʽ����
    StrategyID_Breakout_Strangle,			//ͻ��-������ʽ����
    StrategyID_Consolidation_Straddle,		//����-������ʽ����
    StrategyID_Consolidation_Strangle,		//����-������ʽ����
    StrategyID_ConsolidationUp_Call,		//����Call
    StrategyID_ConsolidationUp_Put,		//����Put
    StrategyID_ConsolidationDown_Call,		//�̵�Call
    StrategyID_ConsolidationDown_Put,		//�̵�Put
};
//�������CLineChart�ؼ������ݽṹ
/*
    Buy         ���
    Sell        ����
    Bearish     ����
    Bullish     ����
*/
enum eOrderStyle{eNone,BuyBullish,BuyBearish,SellBullish,SellBearish,eBullish,eBearish};
enum CHART_TYPE{eProbaChart,eProfitChart};

inline const int FormatStrategyID(const char *nStrategyName)
{
    int nStrategyID=-1;
    if(0==strcmp(nStrategyName,"����"))
        nStrategyID = StrategyID_Rose;
    else if(0==strcmp(nStrategyName,"���"))
        nStrategyID = StrategyID_Crash;
    else if(0==strcmp(nStrategyName,"��������"))
        nStrategyID = StrategyID_Peakback;
    else if(0==strcmp(nStrategyName,"���׻���"))
        nStrategyID = StrategyID_Bottomout;
    else if(0==strcmp(nStrategyName,"ͻ��-�����ʽ"))
        nStrategyID = StrategyID_Breakout_Straddle;
    else if(0==strcmp(nStrategyName,"ͻ��-������ʽ"))
        nStrategyID = StrategyID_Breakout_Strangle;
    else if(0==strcmp(nStrategyName,"����-������ʽ"))
        nStrategyID = StrategyID_Consolidation_Straddle;
    else if(0==strcmp(nStrategyName,"����-������ʽ"))
        nStrategyID = StrategyID_Consolidation_Strangle;
    else if(0==strcmp(nStrategyName,"����-Call"))
        nStrategyID = StrategyID_ConsolidationUp_Call;
    else if(0==strcmp(nStrategyName,"����-Put"))
        nStrategyID = StrategyID_ConsolidationUp_Put;
    else if(0==strcmp(nStrategyName,"�̵�-Call"))
        nStrategyID = StrategyID_ConsolidationDown_Call;
    else if(0==strcmp(nStrategyName,"�̵�-Put"))
        nStrategyID = StrategyID_ConsolidationDown_Put;

    return nStrategyID;
}

struct sStrategyData  
{
    sStrategyData()
    {
        eStyle = eNone;
        dOrderPrice           = 50.0;
        dUnderlyingMultiple   = 100.0;
    }

    eOrderStyle eStyle;                     //��������
    double      dUnderlyingMultiple;        //��Լ����
    int         nOrderCount;                //��������
    double      dOrderPrice;                //�����۸�
    double      dStrikePrice;               //��Ȩ��
};

//////////////////////////////////////////////////////////////////////////
/* �������νṹ */ 
struct _POINT 
{ 
    double x; 
    double y; 
    _POINT(double a=0, double b=0) { x=a; y=b;} //constructor 

    bool operator==(const _POINT& pt) const 
    { 
        bool bx = fabs(x-pt.x)<0.000001;
        bool by = fabs(y-pt.y)<0.000001;

        //TRACE("x = %f , pt.x = %f ,fabs(%f) = %f\n",x,pt.x,x-pt.x,fabs(x-pt.x));
        //TRACE("y = %f , pt.y = %f ,fabs(%f) = %f\n",y,pt.y,y-pt.y,fabs(y-pt.y));
        //TRACE("bx = %s, by = %s\n",bx?"true":"false",by?"true":"false");
        return (bx && by); 
    } 
}; 
struct _LINESEG 
{ 
    _POINT s; 
    _POINT e; 
    _LINESEG(_POINT a, _POINT b) { s=a; e=b;} 
    _LINESEG() { } 
}; 
struct _LINE           // ֱ�ߵĽ������� a*x+b*y+c=0  Ϊͳһ��ʾ��Լ�� a >= 0 
{ 
    double a; 
    double b; 
    double c; 
    _LINE(double d1=1, double d2=-1, double d3=0) {a=d1; b=d2; c=d3;} 
}; 


typedef _POINT sTurnPt;
typedef _POINT sBalancePt;


struct sPoint
{
    eOrderStyle eStyle;
    sTurnPt     ptTurn;
    sBalancePt  ptBalance;
};

class CDataInfo
{
private:
    CDataInfo();
    ~CDataInfo();

public:
    static CDataInfo* NewInstance();
    static void DestroyInstance();

private:
    static CDataInfo* m_pInstance;


public:
    void SetBalancePoint(const vector<sBalancePt>& vBalancePt);
    void GetBalancePoint(vector<sBalancePt>& vBalancePt);

    void SetMouseIn(bool bMouseIn);
    bool GetMouseIn();

    void    SetMousePt(CPoint sMousePt);
    CPoint  GetMousePt();


private:
    vector<sBalancePt> m_vBalancePos;       //��������ƽ����߼�����

    bool        m_bMouseIn;                 //�Ƿ����κ�һ��������
    CPoint      m_ptMouse;                  //�κ�һ�������е��������


public:

    void    SetTradingDayFlag(bool bTradingDay);
    void    SetExchangeFlag(bool bArrived);
    void    SetProductFlag(bool bArrived);
    void    SetInstrumentFlag(bool bArrived);

    bool    GetTradingDayFlag();
    bool    GetExchangeFlag();
    bool    GetProductFlag();
    bool    GetInstrumentFlag();
    
    void    SetVolatilityFlag(bool bArrived);
    void    SetLastPriceFlag(bool bArrived);

    bool    GetCaculateFlag();

    void    SetVolatilityTestFlag(bool bTest);
    bool    GetVolatilityTestFlag();

    void    SetInit(bool bInit);
    bool    GetInit();


    string  GetAccount();

    void    SetVolatility(const double& dVolatility);
    void    SetVolatilityTest(const double& dVolatilityTest);
    double  GetVolatility();

    void    GetProfitDlgParams(CTime& tExpireDate,string& strRestDays);

    void    SetStrategyData(const vector <sStrategyData>& vData);
    void    GetStrategyData(vector <sStrategyData>& vData);

    void    SetRelaInstCount(const int& nCount);
    int     GetRelaInstCount();

    void    SetInstrumentInfo(const PlatformStru_InstrumentInfo& info);
    double  GetInstPriceTick();                     //�������С������λ
    double  GetInstPriceTick(const string& sInstID);//�����Ȩ��С������λ
    double  GetInstVolumeMultiple();                     //������Լ����
    double  GetInstVolumeMultiple(const string& sInstID);//�����Ȩ��Լ����
    string  GetExpireDate();
    

    void    SetInstrumentDepthData(const PlatformStru_DepthMarketData& depthData);  
    double  GetLastPrice();
    double  GetInstUpperPrice();
    double  GetInstLowerPrice();
    double  GetInstUpperPrice(const string& sInstID);
    double  GetInstLowerPrice(const string& sInstID);


    
private:

    CEasyTime *m_time1,*m_time2;
    /// ���Ա������
    bool        m_bInit;
    double      m_dQuotLastPrice;                   //���������۸�
    double      m_dVolatility;                      //�����۸��껯������(20��) ���Ա������Ϣ
    double      m_dVolatilityTest;                  //�����۸��껯������(20��) ����ӯ������

    vector <sStrategyData> m_vStrategyData;         //���Ա���

    
    //��ͼ�������////////////////////////////////////////////////////////////////////////
public:  
    void    SetChartInfo(CHART_TYPE eType, const CRect& rc);   
    void    Calculate();
    void    CalculateTest();
    void    SetXAxisTick();
    void    GetXAxisInfo(vector<double>& vXAxisPos, double& dPriceMin, double& nSpaceValue,double& dXPosValueMin,double& dXValueMax);
    void    SetYAxisTick();
    

    static bool CompareOrderByStrikePriceASC(const sStrategyData& l, const sStrategyData& r){return l.dStrikePrice<r.dStrikePrice;}
    static bool ComparePointByXASC(const _POINT& l, const _POINT& r)
    {
        if(fabs(l.x-r.x)<=0.000001)
            return l.y < r.y;

        return l.x<r.x;
    }

    void    SetPos();
    bool    GetPos(vector<_POINT>& vPos);

    void    GetStrategyPos(vector<_POINT>& _vPoint);
    void    MergeStrategyPos(vector<_POINT>& _vPoint);
    double  GetYPosValue(const double& dXPosValue);
    void    GetYPosValue(CHART_TYPE  eType, double& dYPosValueMin, double& dYPosValueMax);
    bool    GetBalancePos(vector<_POINT>& _vPoints, vector<_LINESEG>& _vLineSeg);

    double  GetProbaYPosValue(double dXPosValue);
    double  GetArea(double dXPosValue);            

private:
    CPLProbability*     m_PLProbability;
    CHART_TYPE          m_ChartType;                                //���ڱ�ʶ
    CRect               m_ProbaRect, m_ProfitRect;                  //��������
    int                 m_nRelaInstCount;                           //��ر�������,����x�����Ữ��
    double              m_dValueMin, m_dValueMax;                   //ȷ������X�����߼���ֵ
    double              m_dXAxisPosValueMin, m_dXAxisPosValueMax;   //x�߼��������ֵ����Сֵ(��Ȩ��) 

    double              m_dYAxisPosValueMin1, m_dYAxisPosValueMax1;   //Proba���ڵ�Y�߼��������ֵ����Сֵ(�������Ȩ����)
    double              m_dYAxisPosValueMin2, m_dYAxisPosValueMax2;   //Profit���ڵ�Y�߼��������ֵ����Сֵ(�������Ȩ����)

    vector<double>      m_vecXAxisPos;                              //X������̶ȵ�

    double              m_dPriceMin;
    double              m_dPriceSpace;


    bool                m_bBalance;                                 //�Ƿ���ƽ����ʶ
    vector<_POINT>      m_vPoint;                                   //ÿ�����Ա�����ת�ۺ�ƽ���

public:    
    void    GetExchangeID2ProductID(map<string,set<string>>& mapData);
    void    GetProductID2InstID(map<string,set<string>>& mapData);

    string GetProductByInstID(const string& szCurrInstrumentID);
    string GetExchangeByProductID(const string& sProductName);

public:
    Ceasymutex      m_mutexLastQuot;
    Ceasymutex      m_mutexRelaInst;
    Ceasymutex      m_mutexCaculate;

    CLogFile*   m_pLog;                                     //log
    bool m_bLogon;                                          // �Ƿ��¼��CTP
    string m_szAccount;                                     //��ǰ���˺�
    set<std::string> m_setExchangeID;                       //������
    string      m_TradingDay;                               //������
    map<string,set<string>> m_mapExchangeID2ProductID;      //�������µĲ�ƷƷ��
    map<string,set<string>> m_mapProductID2InstID;          //��ƷƷ���µĺ�Լ

    string  m_SubscribedInstrument;                         //��ǰ������Լ
    set<string> m_setRelaInstrumentInfoReq;                 //��ر�����Լ���� ��T�ͱ�����һ��ȫ��������ʼ������Ϊ�첽��������Ҫ��¼�������غ�Լ�Ƿ񶼵���
    static bool upSort(const PlatformStru_InstrumentInfo& l, const PlatformStru_InstrumentInfo& r){return l.StrikePrice < r.StrikePrice;}

    void    SetLastQuotInfo(const PlatformStru_DepthMarketData& data);
    void    GetLastQuotInfo(map<string,PlatformStru_DepthMarketData>& outData);
    double  GetLimitPrice(const string& sInstID, bool bDirect);
    void    GetRelaInstrumentInfo(vector<PlatformStru_InstrumentInfo>& vData);

    map<string,PlatformStru_DepthMarketData> m_mapLastQuot;             //����ÿ����Լ����������
    vector<PlatformStru_InstrumentInfo> m_vecRelaInstrumentInfoReq;     //���������Ȩ��Լ��Ϣ

    //��Լ��Ϣ���
    bool m_bTradingDayFlag;         
    bool m_bExchangeFlag;
    bool m_bProductFlag;    
    bool m_bInstrumentFlag; 
    bool m_bCaculateFlag;
    bool m_LastPriceFlag;                                           //���¼��Ƿ��ѯ��
    bool m_VolatilityFlag;                                          //�������Ƿ��ѯ��
    bool m_bVolatilityTestFlag;                                      //�Ƿ����Զ�����������״̬
    PlatformStru_InstrumentInfo     m_InstrumentInfo;                 //��ǰ�������Ϣ
    PlatformStru_DepthMarketData    m_InstrumentDepthData;            //��ǰ���������

    CString m_strNote;//������Ϣ
    CString GetNoteText();
    void    SetNoteText(CString strNote);
};
