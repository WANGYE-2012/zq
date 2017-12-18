/*
    ���ڱ����ά��CTP�ķ�������
    added by l. 20130328
*/


#pragma once

#include <map>
#include <string>

#include "easymutex.h"
#include "../Module-Misc2/KernelStruct.h"

#include "PlatformDataSetList.hpp"

#ifdef WIN32
#pragma managed(push,off)
#endif 


class CPlatformDataMgr_Rates
{
public:
    CPlatformDataMgr_Rates(void);
    ~CPlatformDataMgr_Rates(void);


    ///��ȡ��Լ��������, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
    int GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
    bool IsCommissionRateExist(const string& InstrumentID);
	std::map<std::string, PlatformStru_InstrumentCommissionRate> GetCommissionRateAll(void) { CeasymutexGuard guard(m_mutex);  return m_InstrumentCommissionRate; }
    ///���ú�Լ��������
    void SetCommissionRate(const std::string& InstrumentID, const PlatformStru_InstrumentCommissionRate& inData);
    ///�������ڲ�ѯ�������ʵĺ�Լ,���ڲ�ѯ����ʱ��Ӧ
    void SetReqCommissionRateInstrumentID(const std::string& InstrumentID);
    ///��ȡ���ڲ�ѯ�������ʵĺ�Լ
    void GetReqCommissionRateInstrumentID(std::string& outData);

    ///��ȡ��Լ��֤����, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
    int GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
    bool IsMarginRateExist(const string& InstrumentID);
	std::map<std::string, PlatformStru_InstrumentMarginRate> GetMarginRateAll(void) { CeasymutexGuard guard(m_mutex);  return m_InstrumentMarginRate; }
    ///��ȡ��Լ��֤���ʣ��ڲ����ã����ü���
    bool GetMarginRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
    ///���ú�Լ��֤����
    void SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);

    void IncrementReqMarginRateError(const std::string& InstrumentID);
    void IncrementReqCommissionRateError(const std::string& InstrumentID);

    bool IsNeedCheckCommissionRateAndMarginRate(void) { return m_bNeedCheckCommissionRateAndMarginRate;}
    void SetNeedCheckCommissionRateAndMarginRate(bool bNeedCheck) { m_bNeedCheckCommissionRateAndMarginRate=bNeedCheck;}

    void GetAllCommissionRates(vector<PlatformStru_InstrumentCommissionRate>& Rates);
    void GetAllMarginRates(vector<PlatformStru_InstrumentMarginRate>& Rates);

    //�����������
    void clear(void);
    //�ж��Ƿ�Ϊ��
    bool empty(void);

private:
    ///��ȡ��Լ�������ʣ��ڲ����ã����ü���
    bool GetCommissionRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);

private:
    Ceasymutex				m_mutex;
    bool m_bNeedCheckCommissionRateAndMarginRate;                           //�Ƿ���Ҫ����������ʺͱ�֤���ʣ������º�Լ�б�ʱ����ֵΪtrue����ѯ��󣬴�ֵΪfalse

    std::map<std::string, PlatformStru_InstrumentCommissionRate> m_InstrumentCommissionRate;//��������
    std::map<std::string, PlatformStru_InstrumentMarginRate> m_InstrumentMarginRate;        //��֤����
    std::string m_ReqCommissionRateInstrumentID;                            //���ڲ�ѯ�������ʵĺ�Լ

    std::map<std::string, int> m_reqMarginRateError;                        //����֤���ѯ�޼�¼����
	std::map<std::string, int> m_reqCommissionRateError;                    //���������Ѳ�ѯ�޼�¼����
};

#ifdef WIN32
#pragma managed(pop)
#endif 
