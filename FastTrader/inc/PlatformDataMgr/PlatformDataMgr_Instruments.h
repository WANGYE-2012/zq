/*
    ���ڱ����ά��CTP�ĺ�Լ��������ʱ�䡢��������Ϣ������
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <map>
#include <set>
#include <string>
using std::string;
using std::map;
using std::multimap;
using std::set;

#include "easymutex.h"
#include "../Module-Misc2/KernelStruct.h"
#include "PlatformDataSetList.hpp"


#ifdef WIN32
#pragma managed(push,off)
#endif 

class CPlatformDataMgr_Instruments
{
public:
    CPlatformDataMgr_Instruments(bool bUseComb=false);
    ~CPlatformDataMgr_Instruments(void);



    ///ctp���Ի����£��Ӻ�Լ���ƽ�����������ID
    const string Instrument2ExchangeID(const string& strInstrument);
    //����ָ����Լ��Ϣ
    void SetInstrumentInfo(const string& strInstrumentID, const PlatformStru_InstrumentInfo& inData, bool bCTPSimulate, bool bLast);
    ///��ȡָ����Լ��Ϣ
    bool GetInstrumentInfo(const string& strInstrumentID, PlatformStru_InstrumentInfo& outData);
    void GetInstrumentInfos(vector<PlatformStru_InstrumentInfo> &outData,const string ExchangeID="");

    ///ָ����Լ�Ƿ�Ϸ�(����)
    bool InstrumentIsValid(const string& strInstrumentID);
    ///��ȡ�����˵��Ⱥ�Լ����Ϻ�Լ�б�
    bool GetCombInstrumentIDs_IncludeLeg(const string& LegInstrument,std::vector<string>& vecCombInstruments);
    ///��ȡ��Լ�Ĳ�Ʒ���ͣ�ʧ�ܷ���-1
    char GetProductClassType(const string& InstrumentID);
    ///��ȡ��Լ��ProductID
    string GetProductID(const string& strInstrumentID);
    ///��ȡ��Լ�Ľ�����
    bool GetExchangeID(const string& InstrumentID,string& outExchangeID);
    string GetExchangeID(const string& InstrumentID);
    ///��ȡ��Լ�б�
    void GetInstrumentList(std::vector<InstrumentGroupInfo> &outData);
    ///��ȡ��Լ�б�
    void GetInstrumentList(std::set<string> &outData,const string ExchangeID="");
    void GetInstrumentList(vector<string> &outData,const string ExchangeID="");
    ///��ȡ��Լ�б�
    int GetMainInstrumentList(std::set<string> &outData);
    //���������Լ�б�
    void AddMainInstrumentList(string instrument);
    ///���ú�Լ�ݲ��б�
    void SetInstrumentVarietyMap(const std::map<string,string>& inData);
    // ��ȡ��Լ�ݲ��б�
    void GetInstrumentVarietyMap(std::map<string,string>& outData);
    ///��ȡָ��Ʒ�ֵĺ�Լ�б�
    void GetInstrumentListByProductID(const string& ProductID,set<string> &outData);
    void GetInstrumentListByProductID(const string& ProductID,vector<string> &outData);
    ///��ȡָ���������ĺ�Լ-Ʒ��
    void GetProductID_InstrumentIDsByExchangeID(std::map<string,std::set<string>>& outData,const string ExchangeID="");
    ///��ȡȫ����������Ʒ�ֺ�ԼID vector<pair<ExchangeID,vector<pair<ProductID,vector<InstrumentID>>>>>
    void GetExchangeID_ProductID_InstrumentIDs(vector<pair<string,vector<pair<string,vector<string>>>>>& outData);
    ///��ȡȫ����������Ʒ����Ϣ vector<pair<ExchangeID,vector<PlatformStru_ProductInfo>>>
    void GetExchangeID_ProductInfos(vector<pair<string,vector<PlatformStru_ProductInfo>>>& outData);
    ///��ȡ��Լ�������ɹ����غ�Լ������ʧ�ܷ���-1
    int GetInstrumentMultiple(const string& InstrumentID);
    //���������Ⱥ�Լ�ϳ���Ϻ�Լ�����سɹ���񡣺ϳ�ʱ�����Լ�б����Ƿ��и���Ϻ�Լ
	bool GetComboInstrumentID(  const string strInstrumentID1, 
                                const string strInstrumentID2, 
                                string& strComInstrumentID,
                                string& strLeg1InstrumentID,
                                string& strLeg2InstrumentID);
	bool GetLegInstrumentID(    const string& strComInstrumentID,
                                string& strLeg1InstrumentID,
                                string& strLeg2InstrumentID);

    //����ָ����Ʒ��Ϣ
    void SetProductInfo(const string& strProductID, const PlatformStru_ProductInfo& inData);
    //�Ӳ�Ʒ�б���ɾ��û�к�Լ�Ĳ�Ʒ. ��ѯ��Լ������ִ��һ��. ʵ���﷢������������������й��ڵĲ�Ʒ
    void DeleteEmptyProduct();
    ///��ȡƷ���б�
    int GetProductList(vector<string> &outData,const string ExchangeID="");
    bool GetProductInfo(const string& strInstrumentID,PlatformStru_ProductInfo& outData);
    bool GetProductInfo2(const string& strProductID,PlatformStru_ProductInfo& outData);
    void GetProductInfos(vector<PlatformStru_ProductInfo> &outData,const string ExchangeID="");


    //��ȡ�������Ľ�����
    string GetTradingDay(void);
    //�ж�һ���ַ��������Ƿ��ǵ���
    bool IsToday(const char *OpenDate);
    //��ȡ������ʱ����Ϣ
    unsigned long GetExchangeTime(const string& ExchangeID,string& time);
    //��ȡ������ʱ��
    int GetExchangeTime(ExchangeTime& outData);
    //���ý�����ʱ����Ϣ
    void SetExchangeTime(const ExchangeTime& inData);
    //��ȡȫ��������
    int GetExchangeIDs(std::vector<string>& outData);

    //�����������
    void clear(void);
    //�ж��Ƿ�Ϊ��
    bool empty(void);

    //��ȡָ������������Ȩ������ڻ�Ʒ�֣�����Ŀ��Ʒ��ID������
    int GetOptionProductIDsByExchangeID(const string& ExchangeID,vector<string>& outProductIDs);
    //��ȡָ��������ڻ���Լ��Ӧ����Ȩ��ԼID������Ŀ����Ȩ��Լ��������UnderlyingInstrumentIDΪ���򷵻�ȫ����Ȩ��Լ
    int GetOptionInstrumentIDsByUnderlyingInstrumentID(const string& UnderlyingInstrumentID,vector<string>& outOptionInstrumentIDs);
    //��ȡָ��������ڻ���Լ��Ӧ�Ŀ�����Ȩ��ԼID����Ϳ�����Ȩ��ԼID���顣UnderlyingInstrumentIDΪ���򷵻�ȫ����Ȩ��Լ
    void GetOptionInstrumentIDsByUnderlyingInstrumentID(const string& UnderlyingInstrumentID,vector<string>& outCallOptionInstrumentIDs,vector<string>& outPutOptionInstrumentIDs);
    //��ȡ���б�����ڻ���Լ
    int GetAllOptionUnderlyingInstrumentIDs(vector<string>& outUnderlyingInstrumentIDs);
    //��ȡ���б�����ڻ���Լ
    int GetAllOptionUnderlyingInstrumentIDs(set<string>& outUnderlyingInstrumentIDs);
    //�ж��Ƿ�����Ȩ������ڻ���Լ
    bool IsUnderlyingInstrumentID(const string& InstrumentID);

private:
    Ceasymutex				                    m_mutex;
    bool                                        m_bUseComb;                     //�Ƿ�ʹ����ϵ�����

    //��Ʒ��Ϣ
    map<string, PlatformStru_ProductInfo>       m_allProducts;                  //ProductID -> ProductInfo

    //������ID->Ʒ��IDs
    map<string,set<string>>                     m_ProductIDsByExchangeID;       //ExchangeID -> set of ProductIDs

    //������ID->��Ȩ�����Ʒ��ID
    multimap<string,string>                     m_OptionUnderlyingProductIDsByExchangeID;       //ExchangeID -> Underlying ProductIDs of option
    //������ڻ���Լ->��Ȩ��Լ
    multimap<string,string>                     m_OptionInstrumentIDsByUnderlyingInstrumentID;  //UnderlyingInstrumentID -> OptionInstrumentIDs
    multimap<string,string>                     m_CallOptionInstrumentIDsByUnderlyingInstrumentID;  //UnderlyingInstrumentID -> CallOptionInstrumentIDs
    multimap<string,string>                     m_PutOptionInstrumentIDsByUnderlyingInstrumentID;   //UnderlyingInstrumentID -> PutOptionInstrumentIDs

    //��Լ��Ϣ�������������ݼ��ڲ�ѯ��Լ��Ϣ����еõ���������gui����
    map<string, PlatformStru_InstrumentInfo>    m_allInstruments;               //��ǰ���к�Լ��Ϣ��ϵͳ��ʼ��ʱ��ȡ. InstrumentID -> InsrumentInfo
    set<string>                                 m_allInstrumentList;            //��ǰ���к�Լ�б�. set<InstrumentID>
    set<string>                                 m_allInstrumentListWithoutComb; //��ǰ���к�Լ�б��ų���Ϻ�Լ��. set<InstrumentID>
    map<string, set<string>>                    m_allInstrumentIDsByProductID;  //���в�Ʒ����. ProductID -> set<InstrumentID>
    multimap<string,string>                     m_Leg2Comb;                     //���Ⱥ�Լ����Ϻ�Լ�Ķ�Ӧ��. LegInstrumentID -> CombInstrumentID
    map<string,string>                          m_Legs2Comb;                    //Leg1&Leg2 -> ComInstrumentID����c1309&c1401 -> SP c1309&c1401

    set<string>                                 m_allMainInstrumentList;        //��ǰ������Լ�б�. set<InstrumentID>

	std::map<string, string>                    m_allInstrumentVarietyMap;      //���ڶ��û�����ĺ�Լ��������ݴ���ת��

    ExchangeTime                                m_exchangeTime;                 //������ʱ��

};

#ifdef WIN32
#pragma managed(pop)
#endif 
