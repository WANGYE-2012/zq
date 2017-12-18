#include "stdafx.h"

//#include "wx/file.h"
//#include "wx/textfile.h"

#include <vector>
#include <string>
#include <map>

#include "ServiceProxy.hpp"
#include "PlatFormService.hpp"
#include "QryQueue.h"
#include "ReqOnRtnTrade.h"
#include "../inc/Module-Misc2/GlobalConfigMgr.h"
#pragma warning(disable:4996)


bool CReqOnRtnTrade::m_bInit=false;
CRITICAL_SECTION CReqOnRtnTrade::m_CS;
std::map<Stru_ReqKeyOnRtnTrade,Stru_ReqOnRtnTradeTime> CReqOnRtnTrade::m_mapReqTime;
int CReqOnRtnTrade::m_QryPrior=1;

void CReqOnRtnTrade::Init(void)
{
    InitializeCriticalSection(&m_CS);
    m_bInit=true;
}

//ע�⣺Ϊ�˱����ͷ���Դʱ�����̻߳�ʹ����m_CS�����ﲻ�ͷ�m_CS����Ӧ�ó������ʱ�Զ��ͷ�
void CReqOnRtnTrade::Release(void)
{
    if(!m_bInit)
        return;

    EnterCriticalSection(&m_CS);
    m_bInit=false;
    LeaveCriticalSection(&m_CS);
}




//�ɽ��ر������Ĵ���
//��ReqKey��ӵ�map�У�������ʱ�䣬�ȴ�OnTimeʱ����
void CReqOnRtnTrade::OnRtnTrade(const std::string& strBrokerID,
                                const std::string& strInvestorID,
                                const std::string& strInstrumentID,
                                bool bPositionComb,
                                int RecordCountOfPositionDetail)
{
    Stru_ReqOnRtnTradeTime ReqTime;

    if(!m_bInit) return;

    if(strInstrumentID.empty())
        return;

    Stru_ReqKeyOnRtnTrade ReqKey(strBrokerID,strInvestorID,strInstrumentID);

    EnterCriticalSection(&m_CS);

    std::map<Stru_ReqKeyOnRtnTrade,Stru_ReqOnRtnTradeTime>::iterator it;
    it=m_mapReqTime.find(ReqKey);
    if(it==m_mapReqTime.end())
    {
        //������ReqKey�����ڣ����һ��
        m_mapReqTime.insert(std::make_pair<Stru_ReqKeyOnRtnTrade,Stru_ReqOnRtnTradeTime>(ReqKey,ReqTime));
        it=m_mapReqTime.find(ReqKey);
    }

    if(it!=m_mapReqTime.end())
    {
        //��ReqKey��Ӧ��ʱ������Ϊ��ǰʱ��
        it->second.OnRtnTradeTime=GetTickCount();
        it->second.QryPositionDetailTime=0;
        it->second.QryPositionTime=0;
        it->second.QryAccountTime=0;
        it->second.QryPositionCombTime=0;
        it->second.bQryPositionComb=bPositionComb;
        it->second.RecordCountOfPositionDetail=RecordCountOfPositionDetail;
    }

    LeaveCriticalSection(&m_CS);

}


//���óֲֲ�ѯ���ȼ���1:�ֲ֣�2:�ֲ���ϸ��3:��ϳֲ�
void CReqOnRtnTrade::SetQryPrior(int QryPrior)
{
    if(!m_bInit) return;

    if(QryPrior==1||QryPrior==2||QryPrior==3)
        m_QryPrior=QryPrior;
}


//�ֲֲ�ѯ���ȼ���1:�ֲ֣�2:�ֲ���ϸ��3:��ϳֲ֡�
//��ѯ˳��(��m_QryPrior==1)�� �ֲ֡��ʽ𡢳ֲ���ϸ����ϳֲ�
//ע�⣬�ʽ��ѯһ���ڳֲ�֮��
void CReqOnRtnTrade::CheckOnTime(CServiceProxy& rServiceProxy)
{
    DWORD dwTime;

    if(!m_bInit) return;

    EnterCriticalSection(&m_CS);

    dwTime=GetTickCount();
    std::map<Stru_ReqKeyOnRtnTrade,Stru_ReqOnRtnTradeTime>::iterator it;
    for(it=m_mapReqTime.begin();it!=m_mapReqTime.end();)
    {
        if((dwTime<it->second.OnRtnTradeTime||
            (int)(dwTime-it->second.OnRtnTradeTime)>=GlobalConfigManager::GetDelayBefReq_OnRtnTrade()*1000)&&
            rServiceProxy.IsQryQueueEmpty())
        {
            if(m_QryPrior==1&&it->second.QryPositionTime==0)
            {
                ReqPosition_Internal(it->first,it->second, rServiceProxy);
                it++;
            }
            else if(m_QryPrior==2&&it->second.QryPositionDetailTime==0&&
                it->second.RecordCountOfPositionDetail<GlobalConfigManager::GetMaxRecordReqPositionDetail_OnRtnTrade())
            {
                ReqPositionDetail_Internal(it->first,it->second, rServiceProxy);
                it++;
            }
            else if(m_QryPrior==3&&it->second.QryPositionCombTime==0&&it->second.bQryPositionComb)
            {
                ReqPositionComb_Internal(it->first,it->second, rServiceProxy);
                it++;
            }
            else if(it->second.QryPositionTime==0)
            {
                ReqPosition_Internal(it->first,it->second, rServiceProxy);
                it++;
            }
            else if(it->second.QryAccountTime==0)
            {
                ReqTradingAccount_Internal(it->first,it->second, rServiceProxy);
                it++;
            }
            else if(it->second.QryPositionDetailTime==0&&
                    it->second.RecordCountOfPositionDetail<GlobalConfigManager::GetMaxRecordReqPositionDetail_OnRtnTrade())
            {
                ReqPositionDetail_Internal(it->first,it->second, rServiceProxy);
                it++;
            }
            else if(it->second.QryPositionCombTime==0&&it->second.bQryPositionComb)
            {
                ReqPositionComb_Internal(it->first,it->second, rServiceProxy);
                it++;
            }
            else
            {
                //��������
                it=m_mapReqTime.erase(it);
            }
        }
        else 
        {
            it++;
        }
    }
    
    LeaveCriticalSection(&m_CS);
}

//��ѯ�ֲ�
void CReqOnRtnTrade::ReqPosition_Internal(const Stru_ReqKeyOnRtnTrade& ReqKey,
										  Stru_ReqOnRtnTradeTime& ReqTime,
										  CServiceProxy& rServiceProxy)
{
    PlatformStru_QryInvestorPosition QryData;
    strncpy(QryData.BrokerID,ReqKey.strBrokerID.c_str(),sizeof(QryData.BrokerID)-1);
    strncpy(QryData.InvestorID,ReqKey.strInvestorID.c_str(),sizeof(QryData.InvestorID)-1);
    strncpy(QryData.InstrumentID,ReqKey.strInstrumentID.c_str(),sizeof(QryData.InstrumentID)-1);

    rServiceProxy.ReqQryInvestorPosition(QryData);
    ReqTime.QryPositionTime=GetTickCount();
}

//��ѯ�ֲ���ϸ
void CReqOnRtnTrade::ReqPositionDetail_Internal(const Stru_ReqKeyOnRtnTrade& ReqKey,
												Stru_ReqOnRtnTradeTime& ReqTime,
												CServiceProxy& rServiceProxy)
{
    PlatformStru_QryInvestorPositionDetail QryData;
    strncpy(QryData.BrokerID,ReqKey.strBrokerID.c_str(),sizeof(QryData.BrokerID)-1);
    strncpy(QryData.InvestorID,ReqKey.strInvestorID.c_str(),sizeof(QryData.InvestorID)-1);
    strncpy(QryData.InstrumentID,ReqKey.strInstrumentID.c_str(),sizeof(QryData.InstrumentID)-1);

	rServiceProxy.ReqQryInvestorPositionDetail(QryData);
    ReqTime.QryPositionDetailTime=GetTickCount();
}

//��ѯ��ϳֲ���ϸ
void CReqOnRtnTrade::ReqPositionComb_Internal(const Stru_ReqKeyOnRtnTrade& ReqKey,
											  Stru_ReqOnRtnTradeTime& ReqTime,
											  CServiceProxy& rServiceProxy)
{
    PlatformStru_QryInvestorPositionCombineDetail QryData;
    strncpy(QryData.BrokerID,ReqKey.strBrokerID.c_str(),sizeof(QryData.BrokerID)-1);
    strncpy(QryData.InvestorID,ReqKey.strInvestorID.c_str(),sizeof(QryData.InvestorID)-1);
    strncpy(QryData.CombInstrumentID,ReqKey.strInstrumentID.c_str(),sizeof(QryData.CombInstrumentID)-1);

    rServiceProxy.ReqQryInvestorPositionCombineDetail(QryData);
    ReqTime.QryPositionCombTime=GetTickCount();
}

//��ѯ�ʽ�
void CReqOnRtnTrade::ReqTradingAccount_Internal(const Stru_ReqKeyOnRtnTrade& ReqKey,
												Stru_ReqOnRtnTradeTime& ReqTime,
												CServiceProxy& rServiceProxy)
{
    PlatformStru_QryTradingAccount QryData;
    strncpy(QryData.BrokerID,ReqKey.strBrokerID.c_str(),sizeof(QryData.BrokerID)-1);
    strncpy(QryData.InvestorID,ReqKey.strInvestorID.c_str(),sizeof(QryData.InvestorID)-1);

    rServiceProxy.ReqQryTradingAccount(QryData);
    ReqTime.QryAccountTime=GetTickCount();
}
