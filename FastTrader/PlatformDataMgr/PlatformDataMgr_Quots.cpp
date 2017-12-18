//ά���ײ���������

#pragma warning(disable:4996)

#include <vector>
#include <string>
#include <map>
using namespace std;

#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgrMacro.h"
#include "FileOpr.h"
#include "Module-Misc2/GlobalFunc.h"


#ifdef WIN32
#pragma unmanaged
#endif 


//���죺
CPlatformDataMgr_Quots::CPlatformDataMgr_Quots(CPlatformDataMgr_Instruments& refInstruments)
: m_RefInstruments(refInstruments)
{
}

CPlatformDataMgr_Quots::~CPlatformDataMgr_Quots(void)
{
    LOG_INFO("CPlatformDataMgr_Quots::~CPlatformDataMgr_Quots(����CPlatformDataMgr_Quots����)");
}


//��ȡָ����Լ����
bool CPlatformDataMgr_Quots::GetQuotInfo(const string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    bool brlt=false;

    Lock_CPlatformDataMgr;

    map<string,PlatformStru_DepthMarketData>::iterator it =  m_lastQuot.find(InstrumentID);
    if(it==m_lastQuot.end())
    {
        memset(&outData,0,sizeof(outData));
    }
    else
    {
        outData = it->second;
        brlt=true;
    }

    Unlock_CPlatformDataMgr;

    return brlt;
}

//����ָ����Լ���飬���õ�������£��򷵻�true�����򷵻�false����ԭ����������ϣ�
//������Ϻ�Լ���������
bool CPlatformDataMgr_Quots::SetQuotInfo(const PlatformStru_DepthMarketData& inData)
{
	if(inData.InstrumentID[0]==0) return false;

    Lock_CPlatformDataMgr;

    map<string,PlatformStru_DepthMarketData>::iterator it = m_lastQuot.find(string(inData.InstrumentID));
    if(it==m_lastQuot.end())
    {
        m_lastQuot[string(inData.InstrumentID)] = inData;
    }
    else
    {
        PlatformStru_DepthMarketData& OldBusiness= it->second;
        m_OldQuot[string(inData.InstrumentID)]=m_lastQuot[string(inData.InstrumentID)];
        m_lastQuot[string(inData.InstrumentID)] = inData;
    }

    Unlock_CPlatformDataMgr;

    return true;
}

//��ȡָ����Լ�����¼�
double CPlatformDataMgr_Quots::GetCurPrice (const string& InstrumentID)
{
    double ret=util::GetDoubleInvalidValue();
	//��Ϻ�Լ
	char clstype=m_RefInstruments.GetProductClassType(InstrumentID);
	if(clstype==THOST_FTDC_PC_Combination)
	{
		string id1,id2;
        if(GlobalFunc::IsCombInstrument(InstrumentID,id1,id2))
        {
		    double prc1=GetCurPrice(id1);
		    double prc2=GetCurPrice(id2);
            if(prc1!=util::GetDoubleInvalidValue()&&prc2!=util::GetDoubleInvalidValue())
		        ret= prc1-prc2;
        }
        else
        {
            map<string,PlatformStru_DepthMarketData>::iterator it =  m_lastQuot.find(InstrumentID);
		    if(it!=m_lastQuot.end())
		    {
                if(it->second.Volume>0) ret= it->second.LastPrice;
                else ret= it->second.PreSettlementPrice;
		    }
        }
	}
	else if(clstype==THOST_FTDC_PC_Futures)
	{
        Lock_CPlatformDataMgr;
		
        map<string,PlatformStru_DepthMarketData>::iterator it =  m_lastQuot.find(InstrumentID);
		if(it!=m_lastQuot.end())
		{
            if(it->second.Volume>0) ret= it->second.LastPrice;
            else ret= it->second.PreSettlementPrice;
		}

        Unlock_CPlatformDataMgr;
	}
    return ret;
}

//��ȡָ����Լ������
bool CPlatformDataMgr_Quots::GetOldQuotInfo(const string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    bool ret=false;

    Lock_CPlatformDataMgr;

    map<string,PlatformStru_DepthMarketData>::iterator it =  m_OldQuot.find(InstrumentID);
    if(it==m_OldQuot.end())
    {
        memset(&outData,0,sizeof(outData));
    }
    else
    {
        outData = it->second;
        ret=true;
    }

    Unlock_CPlatformDataMgr;

    return ret;
}

void CPlatformDataMgr_Quots::clear(void)
{
    Lock_CPlatformDataMgr;
    m_lastQuot.clear();
    m_OldQuot.clear();
    Unlock_CPlatformDataMgr;
}
bool CPlatformDataMgr_Quots::empty(void)
{
    Lock_CPlatformDataMgr;
    bool brlt=m_lastQuot.empty();
    Unlock_CPlatformDataMgr;
    return brlt;
}
