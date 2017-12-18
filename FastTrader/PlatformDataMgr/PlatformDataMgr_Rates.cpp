//�����ά��CTP�ĺ�Լ�����ʡ�������ʱ�䡢��������Ϣ������

#pragma warning(disable:4996)

#include <vector>
#include <string>
#include <map>
using namespace std;

#include "PlatformDataMgr_Rates.h"
#include "PlatformDataMgrMacro.h"
#include "FileOpr.h"


#ifdef WIN32
#pragma unmanaged
#endif 


//���죺
CPlatformDataMgr_Rates::CPlatformDataMgr_Rates(void)
{
}

CPlatformDataMgr_Rates::~CPlatformDataMgr_Rates(void)
{
    LOG_INFO("CPlatformDataMgr_Rates::~CPlatformDataMgr_Rates(����CPlatformDataMgr_Rates����)");
}



///��ȡ��Լ��������, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
int CPlatformDataMgr_Rates::GetCommissionRate(const string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    LOG_INFO("CPlatformDataMgr_Rates::GetCommissionRate");
    int ret;
    Lock_CPlatformDataMgr;
    bool brlt=GetCommissionRate_Internal(InstrumentID,outData);
    if(brlt) ret=0;
    else if(m_reqCommissionRateError[InstrumentID]>3) ret=CustomErrorID;
    else ret=-1;
    Unlock_CPlatformDataMgr;
    LOG_INFO("CPlatformDataMgr_Rates::GetCommissionRate-end");
    return ret;
}
bool CPlatformDataMgr_Rates::IsCommissionRateExist(const string& InstrumentID)
{
    LOG_INFO("CPlatformDataMgr_Rates::IsCommissionRateExist");
    bool bExist;
    Lock_CPlatformDataMgr;
    bExist=m_InstrumentCommissionRate.find(InstrumentID)!=m_InstrumentCommissionRate.end();
    Unlock_CPlatformDataMgr;
    LOG_INFO("CPlatformDataMgr_Rates::IsCommissionRateExist-end");
    return bExist;
}


///��ȡ��Լ�������ʣ��ڲ����ã����ü���
bool CPlatformDataMgr_Rates::GetCommissionRate_Internal(const string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    bool brlt=false;
    map<string, PlatformStru_InstrumentCommissionRate>::iterator it1;
    it1=m_InstrumentCommissionRate.find(InstrumentID);
    memset(&outData,0,sizeof(outData));
    if(it1!=m_InstrumentCommissionRate.end())
    {
        outData = it1->second;
        brlt=true;
    }
    return brlt;
}


///���ú�Լ��������
void CPlatformDataMgr_Rates::SetCommissionRate(const string& InstrumentID, const PlatformStru_InstrumentCommissionRate& inData)
{
    LOG_INFO("CPlatformDataMgr_Rates::SetCommissionRate");

    Lock_CPlatformDataMgr;

    m_InstrumentCommissionRate[InstrumentID]=inData;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Rates::SetCommissionRate-end");
}

///�������ڲ�ѯ�������ʵĺ�Լ,���ڲ�ѯ����ʱ��Ӧ
void CPlatformDataMgr_Rates::SetReqCommissionRateInstrumentID(const string& InstrumentID)
{
    LOG_INFO("CPlatformDataMgr_Rates::SetReqCommissionRateInstrumentID");

    Lock_CPlatformDataMgr;

    m_ReqCommissionRateInstrumentID=InstrumentID;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Rates::SetReqCommissionRateInstrumentID-end");
}

///��ȡ���ڲ�ѯ�������ʵĺ�Լ
void CPlatformDataMgr_Rates::GetReqCommissionRateInstrumentID(string& outData)
{
    LOG_INFO("CPlatformDataMgr_Rates::GetReqCommissionRateInstrumentID");

    Lock_CPlatformDataMgr;

    outData=m_ReqCommissionRateInstrumentID;

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Rates::GetReqCommissionRateInstrumentID-end");
}

///��ȡ��Լ��֤����, �ɹ�����0��ʧ�ܷ���-1����ѯ�������޷���CustomErrorID
int CPlatformDataMgr_Rates::GetMarginRate(const string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    LOG_INFO("CPlatformDataMgr_Rates::GetMarginRate");
    int ret;

    Lock_CPlatformDataMgr;
    bool brlt=GetMarginRate_Internal(InstrumentID,outData);
    if(brlt) ret=0;
    else if(m_reqMarginRateError[InstrumentID]>3) ret=CustomErrorID;
    else ret=-1;
	Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr_Rates::GetMarginRate-end");
    return ret; 
}
bool CPlatformDataMgr_Rates::IsMarginRateExist(const string& InstrumentID)
{
    LOG_INFO("CPlatformDataMgr_Rates::IsMarginRateExist");
    bool bExist;
    Lock_CPlatformDataMgr;
    bExist=m_InstrumentMarginRate.find(InstrumentID)!=m_InstrumentMarginRate.end();
    Unlock_CPlatformDataMgr;
    LOG_INFO("CPlatformDataMgr_Rates::IsMarginRateExist-end");
    return bExist;
}

///��ȡ��Լ��֤���ʣ��ڲ����ã����ü���
bool CPlatformDataMgr_Rates::GetMarginRate_Internal(const string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    LOG_INFO("CPlatformDataMgr_Rates::GetMarginRate_Internal");
    bool brlt=false;
    map<string, PlatformStru_InstrumentMarginRate>::iterator it;
    it=m_InstrumentMarginRate.find(InstrumentID);
    memset(&outData,0,sizeof(outData));
    if(it!=m_InstrumentMarginRate.end())
    {
        outData = it->second;
        brlt=true;
    }
    LOG_INFO("CPlatformDataMgr_Rates::GetMarginRate_Internal-end");
    return brlt; 
}

///���ú�Լ��֤����
void CPlatformDataMgr_Rates::SetMarginRate(const string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    //LOG_INFO("CPlatformDataMgr_Rates::SetMarginRate");

    Lock_CPlatformDataMgr;

    m_InstrumentMarginRate[InstrumentID]=outData;

    Unlock_CPlatformDataMgr;

    //LOG_INFO("CPlatformDataMgr_Rates::SetMarginRate-end");
}

void CPlatformDataMgr_Rates::IncrementReqMarginRateError(const std::string& InstrumentID)
{
    Lock_CPlatformDataMgr
	if(m_reqMarginRateError.find(InstrumentID)==m_reqMarginRateError.end())
		m_reqMarginRateError[InstrumentID]=0;
	else
		m_reqMarginRateError[InstrumentID]++;
	Unlock_CPlatformDataMgr
}
void CPlatformDataMgr_Rates::IncrementReqCommissionRateError(const std::string& InstrumentID)
{
    Lock_CPlatformDataMgr
	if(m_reqCommissionRateError.find(InstrumentID)==m_reqCommissionRateError.end())
		m_reqCommissionRateError[InstrumentID]=0;
	else
		m_reqCommissionRateError[InstrumentID]++;
	Unlock_CPlatformDataMgr
}

void CPlatformDataMgr_Rates::clear(void)
{
    Lock_CPlatformDataMgr
    
    m_bNeedCheckCommissionRateAndMarginRate=true;

    m_InstrumentCommissionRate.clear();
    m_InstrumentMarginRate.clear();
    m_ReqCommissionRateInstrumentID.clear();

    m_reqMarginRateError.clear();
	m_reqCommissionRateError.clear();

	Unlock_CPlatformDataMgr
}
bool CPlatformDataMgr_Rates::empty(void)
{
    Lock_CPlatformDataMgr;
    bool brlt = m_InstrumentCommissionRate.empty()&&m_InstrumentMarginRate.empty();
    Unlock_CPlatformDataMgr;
    return brlt;
}
void CPlatformDataMgr_Rates::GetAllCommissionRates(vector<PlatformStru_InstrumentCommissionRate>& Rates)
{
    Lock_CPlatformDataMgr;
    Rates.clear();
    for(map<string, PlatformStru_InstrumentCommissionRate>::const_iterator it=m_InstrumentCommissionRate.begin();it!=m_InstrumentCommissionRate.end();it++)
        Rates.push_back(it->second);
    Unlock_CPlatformDataMgr;
}
void CPlatformDataMgr_Rates::GetAllMarginRates(vector<PlatformStru_InstrumentMarginRate>& Rates)
{
    Lock_CPlatformDataMgr;
    Rates.clear();
    for(map<string, PlatformStru_InstrumentMarginRate>::const_iterator it=m_InstrumentMarginRate.begin();it!=m_InstrumentMarginRate.end();it++)
        Rates.push_back(it->second);
    Unlock_CPlatformDataMgr;
}
