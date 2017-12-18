/*
    ���ڱ����ά��CTP�ı�������
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <map>
#include <set>
#include <string>
using std::vector;
using std::map;
using std::multimap;
using std::set;
using std::string;

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Rates.h"
#include "PlatformDataMgr_Quots.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 


class CPlatformDataMgr_Orders
{
public:
    //DiscountDCEComb   :��������ϵ��Żݲ��ԣ�0-���Żݣ�1-��һ���Żݣ�2-�ڶ����Ż�(ȱʡ)
    //DiscountCZCEComb  :֣������ϵ��Żݲ��ԣ�0-���Żݣ�1-��һ���Żݣ�2-�ڶ����Ż�(ȱʡ)
    //bFreezeOrderUnknownStatus: ��״̬Ϊδ֪�ı���(���ֹ���˵�)���Ƿ�ִ���ʽ𶳽�
    CPlatformDataMgr_Orders(
        volatile long& baseFTID,
        volatile long& baseUpdateSeq,
        CPlatformDataMgr_Instruments& refInstruments,
        CPlatformDataMgr_Rates& refRates,
        CPlatformDataMgr_Quots& refQuots,
        int DiscountCZCE_Comb=3,
        int DiscountDCE_Comb=3,
        bool bFreezeOrderUnknownStatus=false);
    ~CPlatformDataMgr_Orders(void);


    //----------�������޸ı����ӿ�----------
public:
    //��ѯ�ı�������
    void QryOrderReach(PlatformStru_OrderInfo& OrderInfo,int ErrID,const bool bIsLast);

	//�±�������
    void NewOrderReady(PlatformStru_OrderInfo& OrderInfo, PlatformStru_OrderInfo& OldOrderInfo, const PlatformStru_InstrumentInfo& InstrumentInfo);

    //�������б����еĶ��ᱣ֤�𣬷��غ�Լ����Ƿ��б����������Ƿ���Ҫ�����ʽ���塣��ѯ��֤������Ӧʱ����
    bool UpdateOrderFreezeMargin_RspQryMarginRate_RspQryDepthMarketData(std::string& strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo);

    //�����б����еĶ��������ѣ����غ�Լ����Ƿ��б����������Ƿ���Ҫ�����ʽ���塣��ѯ����������Ӧʱ����
    bool UpdateOrderFreezeCommission_RspQryCommissionRate_RspQryDepthMarketData(const std::string& strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo);

    void ClearMapQryRltOrders();
    //�����������
    void clear(void);
    //�жϱ����Ƿ��ڹ���״̬�����ҵ�״̬�������(δ֪״̬)
    static bool IsOrderWorking(const PlatformStru_OrderInfo& OrderInfo);
    //�жϱ����Ƿ��ڹҵ�״̬
    static bool IsOrderWaiting(const PlatformStru_OrderInfo& OrderInfo);
    //�жϱ����Ƿ��ڴ���״̬
    bool IsOrderTrigger(const PlatformStru_OrderInfo& OrderInfo);
    //�жϱ����Ƿ��ѳɽ��������ֳɽ���ȫ���ɽ�
    bool IsOrderTraded(const PlatformStru_OrderInfo& OrderInfo);
    //�жϱ����Ƿ��ǳ���/��
    bool IsOrderCanceled(const PlatformStru_OrderInfo& OrderInfo);


private:
    //���㱨����Ϣ�еĶ��ᱣ֤�𡣺�δ֪״̬�ı���
    void CalcuOrderFreezeMargin_Internal(
        PlatformStru_OrderInfo& OrderInfo,
        bool& bUpdate,
        const PlatformStru_InstrumentInfo& InstrumentInfo);

    //���㵥��Լ��Ȩ������Ϣ�еĶ��ᱣ֤��. ��δ֪״̬�ı�����δ���������Ȩ֮���
    //��Ҫ�õ������е�������
    //ÿ���������ױ�֤�� = MAX(Ȩ���� + Ͷ������Ȩ��Լ��֤�𲻱䲿��, Ͷ������Ȩ��Լ��С��֤��)
    //Ȩ���� = �۸񣨶���ʹ�������ۣ�*��Ȩ��Լ����
    void CalcuOrderFreezeMargin_Option_Internal(
        PlatformStru_OrderInfo& OrderInfo,
        bool& bUpdate,
        const PlatformStru_InstrumentInfo& InstrumentInfo);

    //������Ϻ�Լ�����еĶ��ᱣ֤�𡣺�δ֪״̬�ı���
    //���ڱ�����û�з��ȱ��ۣ��������ۼ�����ȶ��ᱣ֤��
    void CalcuOrderFreezeMarginComb_Internal(
        PlatformStru_OrderInfo& OrderInfo,
        bool& bUpdate,
        const PlatformStru_InstrumentInfo& Leg1InstrumentInfo,
        const PlatformStru_InstrumentInfo& Leg2InstrumentInfo);

    //���㱨����Ϣ�еĶ��������ѡ���δ֪״̬�ı���
    void CalcuOrderFreezeCommission_Internal(
        PlatformStru_OrderInfo& OrderInfo,
        bool& bUpdate,
        const PlatformStru_InstrumentInfo& InstrumentInfo);

    //������Ϻ�Լ�����еĶ��������ѡ���Ҫ��˫�߷ֱ���㡣��δ֪״̬�ı���
    //���ڱ�����û�з��ȱ��ۣ��������ۼ�����ȶ���������
    void CalcuOrderFreezeCommissionComb_Internal(
        PlatformStru_OrderInfo& OrderInfo,
        bool& bUpdate,
        const PlatformStru_InstrumentInfo& Leg1InstrumentInfo,
        const PlatformStru_InstrumentInfo& Leg2InstrumentInfo);

    //----------�����ǻ�ȡ�����ӿ�----------
public:
    ///��ȡָ������
    bool GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData);
    ///��ȡָ������
    bool GetOrder2(const std::string& strOrderSysID,PlatformStru_OrderInfo& outData);
    ///��ȡȫ������
    int GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///��ȡȫ������
    int GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);

    ///��ȡȫ���Ѵ����ı���
    int GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///��ȡ��Լ��ص��Ѵ����ı�������Լ�ǵ���Լ�������Ƕ�Ӧ������������ú�Լ����ϱ���
    int GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///��ȡָ���Ѵ����ı���
    bool GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

    ///��ȡ�ѳɽ�����
    int GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///��ȡ�ѳɽ���������Լ�ǵ���Լ�������Ƕ�Ӧ������������ú�Լ����ϱ���
    int GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///��ȡָ���ѳɽ�����
    bool GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

    ///��ȡ�ѳ����ʹ��󱨵�
    int GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///��ȡ�ѳ����ʹ��󱨵�
    int GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///��ȡָ���ѳ����ʹ��󱨵�
    bool GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

    ///��ȡδ�ɽ�����, �������ֹ�����еı���
    int GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///��ȡδ�ɽ�����, �������ֹ�����еı���
    int GetWaitOrders3(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///��ȡָ����Լ��ص�δ�ɽ�����, �������ֹ�����еı�������Լ�ǵ���Լ�������Ƕ�Ӧ������������ú�Լ����ϱ���
    int GetWaitOrders2(const string& strInstrument,vector<PlatformStru_OrderInfo>& outData);
    ///��ȡָ��δ�ɽ�����, �������ֹ�����еı���
    bool GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

    ///��ȡ״̬Ϊδ֪�ı���, ���ֹ�����б���
    int GetUnknownStatusOrders(std::vector<PlatformStru_OrderInfo>& outData);

    //��ȡָ����Լ��ָ������ָ����ƽ�Ĺҵ�������������, �������ֹ�����еı���
    int GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, char Direction, char CloseType,char HedgeFlag=THOST_FTDC_HF_Speculation);
    //��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
    int GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, char Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,char HedgeFlag=THOST_FTDC_HF_Speculation);
    //��ȡָ����Լָ�������ƽ�ֹҵ���ƽ����������0��ʾ�ɹ���<0��ʾʧ�ܡ��ڲ����ã����������ɹ�ʱCloseVolume����ƽ�ֵ�(��ǿƽ�ͱ���ǿƽ)��������CloseTodayVolume����ƽ��������CloseYesterdayVolume��ʾƽ������
	int GetCloseOrderVolume_Internal(const std::string& strAccount,const std::string &strInstrumentID, char Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,char HedgeFlag=THOST_FTDC_HF_Speculation);
    //ȡȫ���ҵ��Ķ��������ѺͶ��ᱣ֤�����û�з��ʣ�Ϊ��Чֵ����״̬δ֪����
    void GetFreezeMargin_Commission(double& FreezeMargin,double& FreezeCommission);
    //��ȡָ����Լָ������ĵĶ��������ͬ����Ŀ��ֵ��ͷ������ƽ�ֵ�����״̬δ֪������(������ϵ��Ե��Ⱥ�Լ��Ӱ��)
    //strInstrumentIDΪ���Ⱥ�Լ
    void GetFreezeMargin_Commission2(const std::string& strAccount,const std::string &strInstrumentID, char Direction,char HedgeFlag,
        double& FreezeMargin,double& FreezeCommission);

	//Ϊ���������½ӿ�
	//��ȡί�е��б�FTID
	int GetAllOrderFTIDList(std::vector<long>& vec);
	//��ȡί�е�����
	int GetAllOrderInfo(long lFTID, PlatformStru_OrderInfo& outData);
	int GetAllOrderInfo(OrderKey key, PlatformStru_OrderInfo& outData);

    //ȡ���йҵ��ĺ�Լ�б�
    void GetInstrumentIDs_Working(set<string>& setInstrumentID);
    //ȡ�ȴ�������ж��ᱣ֤��������Ѽ���ĺ�Լ��
    void GetInstrumentIDs_WaitQuot(set<string>& setInstrumentID);

    //�ж��Ƿ�Ϊ��
    bool empty(void);

    ///��ȡ���ڵ���MinFTID��ȫ�����ݣ������������������������������С�m_bUpdateSeq2FTIDΪtrueʱ��Ч
    int GetAll_GE_UpdateSeq(long MinUpdateSeq,vector<PlatformStru_OrderInfo>& outDatas);

private:
    //��ȡ�ͺ�Լ��صı���FTID���ϣ���������Ϊ�ú�Լ����ϱ���
    void GetFTIDSetOfInstrument2_Internal(const string& strInstrument,set<long>&FTIDs);

private:

    Ceasymutex				        m_mutex;

    CPlatformDataMgr_Instruments    &m_RefInstruments;
    CPlatformDataMgr_Rates          &m_RefRates;
    CPlatformDataMgr_Quots          &m_RefQuots;

    //��ϵ��Żݲ��ԣ�0-���Żݣ�1-��һ���Żݣ�2-�ڶ����Żݣ�3-����֤��Ƚϸߵ�һ����ȡ(ȱʡ)
    int                             m_DiscountDCE_Comb;      
    int                             m_DiscountCZCE_Comb;

    static bool                     m_bFreezeOrderUnknownStatus;//����״̬Ϊδ֪�ı������Ƿ���Ҫ����

    //----------�����Ǳ������ݼ�----------
    CPlatformDataSetList<OrderKey,PlatformStru_OrderInfo> m_OrderData;

	//OrderSysID �� OrderKey ��ӳ���ϵ����Ҫ����ͨ��OrderSysID���Ҷ�Ӧ��OrderKey
    map<string, OrderKey> m_mapOrderSysID2OrderKey;

    multimap<string,long> m_mapLegID12CombOrderFTID;//��һ�Ⱥ�Լ����ϱ���FTID�Ķ�Ӧ��ϵ
    multimap<string,long> m_mapLegID22CombOrderFTID;//�ڶ��Ⱥ�Լ����ϱ���FTID�Ķ�Ӧ��ϵ

    //�����������ݽṹ��ͨ�����ڼ��㱣֤�𣬿������ڴ����Ż�
    struct Stru_OrderCategory
    {
        Stru_OrderCategory(){};
        Stru_OrderCategory(const string& account,const string& instrumentorproduct,TThostFtdcHedgeFlagType hedgeFlag,TThostFtdcDirectionType direction)
        {
            set(account,instrumentorproduct,hedgeFlag,direction);
        };
        Stru_OrderCategory& operator= (const Stru_OrderCategory& r)
        {
            Account=r.Account;
            InstrumentOrProduct=r.InstrumentOrProduct;
            HedgeFlag=r.HedgeFlag;
            Direction=r.Direction;
            return *this;
        };
        void set(const string& account,const string& instrumentorproduct,TThostFtdcHedgeFlagType hedgeFlag,TThostFtdcDirectionType direction)
        {
            Account=account;
            InstrumentOrProduct=instrumentorproduct;
            HedgeFlag=hedgeFlag;
            Direction=direction;
        };
        bool operator< (const Stru_OrderCategory& r) const
        {
            if(Account<r.Account) return true;
            else if(Account>r.Account) return false;
            if(InstrumentOrProduct<r.InstrumentOrProduct) return true;
            else if(InstrumentOrProduct>r.InstrumentOrProduct) return false;
            if(HedgeFlag<r.HedgeFlag) return true;
            else if(HedgeFlag>r.HedgeFlag) return false;
            if(Direction<r.Direction) return true;
            else if(Direction>r.Direction) return false;
            return false;
        }
        bool operator== (const Stru_OrderCategory& r) const
        {
            return Account==r.Account&&InstrumentOrProduct==r.InstrumentOrProduct&&HedgeFlag==r.HedgeFlag&&Direction==r.Direction ? true : false;
        }
        bool operator> (const Stru_OrderCategory& r) const
        {
            return !((*this) < r || (*this) == r);
        }


        string                  Account;            //Ͷ�����˺�
        string                  InstrumentOrProduct;//����Լ����򰴲�Ʒ����
	    TThostFtdcHedgeFlagType	HedgeFlag;          //Ͷ��
	    TThostFtdcDirectionType	Direction;          //����
    };
    //����Լ����ı�֤��m_TotalMargin��Ч�����������Ч��m_mapInstrumentExchMargin�ǽ�������֤�������ڴ����Ż�ʱ�������
    map<Stru_OrderCategory,double> m_mapInstrumentMargin;
    map<Stru_OrderCategory,double> m_mapInstrumentExchMargin;
    bool                        m_bmapInstrumentMarginOK;
    double                      m_TotalMargin;
    double                      m_TotalCommission;

    //����ȫ�����ᱣ֤��
    void UpdateAllMargin_Internal(void);
    //����ȫ������������
    void UpdateAllCommission_Internal(void);
    //���ݱ����仯��������֤��
    void UpdateMarginByOrder_Internal(const PlatformStru_OrderInfo& Order,const PlatformStru_OrderInfo& OldOrder);
    //���ݱ����仯������������
    void UpdateCommissionByOrder_Internal(const PlatformStru_OrderInfo& Order,const PlatformStru_OrderInfo& OldOrder);

    //�����ж��ᱣ֤�𰴺�Լ���������ͳ��
    void ClassifyAllMarginByInstrumentID_Internal(void);
    //���ݱ�֤��������ݣ������ܶ��ᱣ֤��
    void CalcuTotalMargin_Internal(void);


};

#ifdef WIN32
#pragma managed(pop)
#endif 
