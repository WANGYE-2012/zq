#pragma once
#include "CommPublic.h"
#include "PLProbability.h"

class CDataInfo
{
public:
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

    void    SetExchangeFlag(bool bArrived);
    void    SetProductFlag(bool bArrived);
    void    SetInstrumentFlag(bool bArrived);

    bool    GetExchangeFlag();
    bool    GetProductFlag();
    bool    GetInstrumentFlag();

    void    SetVolatilityFlag(bool bArrived);
    void    SetLastPriceFlag(bool bArrived);

    bool    GetCaculateFlag();

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
    bool m_bExchangeFlag;
    bool m_bProductFlag;    
    bool m_bInstrumentFlag; 
    bool m_bCaculateFlag;
    bool m_LastPriceFlag;                                           //���¼��Ƿ��ѯ��
    bool m_VolatilityFlag;                                          //�������Ƿ��ѯ��
    PlatformStru_InstrumentInfo     m_InstrumentInfo;                 //��ǰ�������Ϣ
    PlatformStru_DepthMarketData    m_InstrumentDepthData;            //��ǰ���������

    CString m_strNote;//������Ϣ
    CString GetNoteText();
    void SetAccount(string sAccount);
    void SetTradingDay(string sDay);
    void InsertMapExchangeID2ProductID(const string& sExchangeID, const set<string>& setProductID);
    void SetProductId2InstID(const string& sProductID, const set<string>& setInstID);
    void ClearRelaInstrumentInfo(void);
    void SetSubscribeInstrument(const string& sInstID);
    void InsertSetRelaInstrumentInfo(const string& sInstID);
    string GetSubscribedInstrument(void);
    void GetVecRelaInstrumentInfoReq(vector<PlatformStru_InstrumentInfo>& vecRelaInstrumetInfoReq);
    void GetSetRelaInstrumentInfoReq(set<string>& setRelaInstrumetInfoReq);
};
