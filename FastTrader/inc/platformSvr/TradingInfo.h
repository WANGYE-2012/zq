/***************************************************************************//**
*   @file         TradingInfo.h
*   @brief        �������ջ���Ľ��ײ���������Ʒ�֡���Լ���������ʡ���֤����
*                   ��Щ���ݻ�����tradinginfo.xml/product.xml/instrument.xml/commissionrate.xml/marginrate.xml��
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/09/14
********************************************************************************/

#ifndef _TRADINGPARAM_H_
#define _TRADINGPARAM_H_

//#include <map>
//#include <set>
#include <string>
#include <vector>
//using std::map;
//using std::set;
using std::string;
using std::vector;

//#include "../Module-Misc2/BusinessData.hpp"
#include "easymutex.h"
#include "myxml.h"
#include "../Module-Misc2/KernelStruct.h"



//********************************************************************************************
//  ��ȡxml��Ϣ����
//
//  ��xml����ȡ������������ȡƷ�ֲ�������Լ�����������Ѳ�������֤�����
//  <Instruments>
//      <colname>InstrumentID,InvestorRange,BrokerID,InvestorID,OpenRatioByMoney,OpenRatioByVolume,CloseRatioByMoney,CloseRatioByVolume,CloseTodayRatioByMoney,CloseTodayRatioByVolume,StrikeRatioByMoney,StrikeRatioByVolume</colname>
//      <lines>
//          <line>...</line>
//          <line>...</line>
//      </lines>
//  </Instruments>
//
//����ʹ��malloc�ķ��������ڴ棬�����й����ڴ棬������stl������
//ԭ������debugģʽ�£������Լ������1400�����ң���stl���ʱ5-6��
//*********************************************************************************

class CTradingInfo
{
public:
    enum eDataStructType
    {
        eTradingParams=1,
        eProductInfos,
        eInstrumentInfos,
        eCommissionRates,
        eMarginRates,
    };

public:
    CTradingInfo(eDataStructType datatype,int SvrGrpID,const string& strAccount,const string& strTradingDay);
    ~CTradingInfo();

    //��xml�м������ݣ����зŵ�m_pLineContent�У�m_psubposs��m_psublens��¼ÿ��ÿ���ֶε�λ�úͳ���
    virtual void Load(void);

public:


    //��ȡ�����������ơ������Ƽ�����xml�ļ����ƣ�������������������tradingparam��product��instrument��commissionrate��marginrate
    static string GetDataTypeName(eDataStructType datatype);
    //��ȡxml�ļ����ƣ���·��
    static string GetXMLFileName(eDataStructType datatype);
    //��ȡ[SvrGrpIDxxx-Accountyyy]������
    static string GetAccountNodeName(int SvrGrpID,const string& strAccount);

private:
    //Loadʱ���ȼ��xml�Ƿ�Ϸ���1. ������ǵ�ǰ�����գ�����������ļ�; 2. ���û��<SvrGrpIDxxx-Accountyyy>�ڣ��򴴽�֮
    void CheckAfterLogin_Internal(CMyXML& myxml);


public:
    eDataStructType m_datatype;
    int             m_SvrGrpID;
    string          m_strAccount;
    string          m_strTradingDay;

    bool            m_bValid;
    vector<string>  m_vecColName;   //��ͷ����
    int             m_ColCnt;       //����
    int             m_LineCnt;      //����
    char**          m_pLineContent; //ÿһ��ԭʼ����
    int*            m_psubposs;     //ÿ��ÿ�ֶ�����ԭʼ�������λ��
    int*            m_psublens;     //ÿ��ÿ�ֶεĳ���

    static Ceasymutex      m_mutex;
private:
    //��ֹ��������͸�ֵ
    CTradingInfo(const CTradingInfo&);
    CTradingInfo& operator= (const CTradingInfo&);
};

//********************************************************************************************
//  ����xml��Ϣ����
//  ʹ��CTradingInfo::m_mutex
//  ������Ϊ: [root]-[SvrGrpIDxxx-Accountyyy]-[NodeName]
//  ��������<colname>��
//  ���ݷ���<lines>
//  bEraseOldInfo��ʾ�Ƿ�ɾ��ԭ�����ݡ�Ʒ�֡���Լ�����ײ���ʱһ���Ա��棬����ɾ��ԭ�����ݡ��������յ�һ����һ��������ɾ��ԭ������(�ڵ�¼�����֤���Ѿ�����������ɾ����)��
//  ���磺
//      <Instruments>
//          <colname>InstrumentID,InvestorRange,BrokerID,InvestorID,OpenRatioByMoney,OpenRatioByVolume,CloseRatioByMoney,CloseRatioByVolume,CloseTodayRatioByMoney,CloseTodayRatioByVolume,StrikeRatioByMoney,StrikeRatioByVolume</colname>
//          <lines>
//              <line>...</line>
//              <line>...</line>
//          </lines>
//      </Instruments>
//********************************************************************************************

class CSaveTradingInfo
{
public:
    CSaveTradingInfo(CTradingInfo::eDataStructType datatype,int SvrGrpID,const string& strAccount);
    ~CSaveTradingInfo();

    void OpenXML4Save(bool bEraseOldInfo);

protected:
    CTradingInfo::eDataStructType m_datatype;
    int                 m_SvrGrpID;
    string              m_strAccount;

    const static int    m_linebufsize=10240;
    char*               m_plinecontent;
    char*               m_pcolname;
    int                 m_linebuflen;
    int                 m_colnamelen;
    CMyXML*             m_pxml;
    CMyXML*             m_pnodexml;
    CMyXML*             m_pcolnamexml;
    CMyXML*             m_plinesxml;


private:
    //��ֹ��������͸�ֵ
    CSaveTradingInfo(const CSaveTradingInfo&);
    CSaveTradingInfo& operator= (const CSaveTradingInfo&);
};


//********************************************************************************************
//  �����������࣬����ʵ��xml���ݵļ��غͱ���
//********************************************************************************************
//Ʒ����Ϣ
class CTradingInfo_PlatformStru_ProductInfo : public CTradingInfo,CSaveTradingInfo
{
public:
    //����product.xml�е�Ʒ����Ϣ���������m_pLineData��
    CTradingInfo_PlatformStru_ProductInfo(int SvrGrpID,const string& strAccount,const string& strTradingDay);
    ~CTradingInfo_PlatformStru_ProductInfo();
    virtual void Load(void);
    void SaveXML(const vector<PlatformStru_ProductInfo>& Infos);

public:
    PlatformStru_ProductInfo*    m_pLineData;
};

//��Լ��Ϣ
class CTradingInfo_PlatformStru_InstrumentInfo : public CTradingInfo,CSaveTradingInfo
{
public:
    //����instrument.xml�еĺ�Լ��Ϣ���������m_pLineData��
    CTradingInfo_PlatformStru_InstrumentInfo(int SvrGrpID,const string& strAccount,const string& strTradingDay);
    ~CTradingInfo_PlatformStru_InstrumentInfo();
    virtual void Load(void);
    void SaveXML(const vector<PlatformStru_InstrumentInfo>& Infos);

public:
    PlatformStru_InstrumentInfo*    m_pLineData;
};

//����������Ϣ
class CTradingInfo_PlatformStru_InstrumentCommissionRate : public CTradingInfo,CSaveTradingInfo
{
public:
    //����commissionrate.xml�е�����������Ϣ���������m_pLineData��
    CTradingInfo_PlatformStru_InstrumentCommissionRate(int SvrGrpID,const string& strAccount,const string& strTradingDay);
    ~CTradingInfo_PlatformStru_InstrumentCommissionRate();
    virtual void Load(void);
    void SaveXML(const vector<PlatformStru_InstrumentCommissionRate>& Infos);

public:
    PlatformStru_InstrumentCommissionRate*    m_pLineData;
};

//��֤������Ϣ
class CTradingInfo_PlatformStru_InstrumentMarginRate : public CTradingInfo,CSaveTradingInfo
{
public:
    //����marginrate.xml�еı�֤������Ϣ���������m_pLineData��
    CTradingInfo_PlatformStru_InstrumentMarginRate(int SvrGrpID,const string& strAccount,const string& strTradingDay);
    ~CTradingInfo_PlatformStru_InstrumentMarginRate();
    virtual void Load(void);
    void SaveXML(const vector<PlatformStru_InstrumentMarginRate>& Infos);

public:
    PlatformStru_InstrumentMarginRate*    m_pLineData;
};

//���͹�˾���ײ�����Ϣ
class CTradingInfo_PlatformStru_BrokerTradingParams : public CTradingInfo,CSaveTradingInfo
{
public:
    //����tradingparam.xml�еĽ��ײ�����Ϣ���������m_pLineData��
    CTradingInfo_PlatformStru_BrokerTradingParams(int SvrGrpID,const string& strAccount,const string& strTradingDay);
    ~CTradingInfo_PlatformStru_BrokerTradingParams();
    virtual void Load(void);
    void SaveXML(const PlatformStru_BrokerTradingParams& Info);

public:
    PlatformStru_BrokerTradingParams*    m_pLineData;
};

#endif

