//ά���ײ�ɽ�ͳ�Ƽ�¼��Ϣ

#pragma warning(disable:4996)

#include <vector>
#include <string>
#include <map>
using namespace std;

#include "PlatformDataMgr_TradeTotals.h"
#include "PlatformDataMgrMacro.h"
#include "FileOpr.h"


#ifdef WIN32
#pragma unmanaged
#endif 



//���죺
CPlatformDataMgr_TradeTotals::CPlatformDataMgr_TradeTotals(
    volatile long& baseFTID,
    volatile long& baseUpdateSeq,
    CPlatformDataMgr_Trades& refTrades)
: m_RefTrades(refTrades),
  m_TradeTotalData(baseFTID,baseUpdateSeq)
{
}

CPlatformDataMgr_TradeTotals::~CPlatformDataMgr_TradeTotals(void)
{
    LOG_INFO("CPlatformDataMgr_TradeTotals::~CPlatformDataMgr_TradeTotals(����CPlatformDataMgr_TradeTotals����)");
}



//��ѯ�ĳɽ��ر���ȫ����ʼ����ɽ�ͳ��
void CPlatformDataMgr_TradeTotals::QryTradeTotalReach(bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr_TradeTotals::QryTradeTotalReach");

    Lock_CPlatformDataMgr;

    if(bIsLast)
    {
		//�����ԭ�м�¼
        PlatformStru_TradeTotalInfo BlankData;
        m_TradeTotalData.WillQry_Internal(string(""));

        m_TradeTotalData.QryRltReach_Internal(BlankData,CustomErrorID,true);

        //�ӳɽ���¼���ɳɽ�ͳ��
        set<TradeTotalKey> setTradeTotalKey;
        m_RefTrades.GetTradeTotalKeysByTrades(setTradeTotalKey);

        set<TradeTotalKey>::iterator it_set;
		PlatformStru_TradeTotalInfo TradeTotal;
        for(it_set=setTradeTotalKey.begin();it_set!=setTradeTotalKey.end();it_set++)
        {
			if(!m_RefTrades.GetTradeTotalValue(*it_set,TradeTotal))
				continue;

			m_TradeTotalData.NewReach_Internal(TradeTotal);
        }
    }

    Unlock_CPlatformDataMgr;


    LOG_INFO("CPlatformDataMgr_TradeTotals::QryTradeReach-end");
}


//�������������Ӧʱ��������Ӧ��Լ�ĳɽ�ͳ�ơ����ظ������
bool CPlatformDataMgr_TradeTotals::UpdateTradeTotal_RspQryCommissionRate(const string& strInstrument)
{
	bool brlt=false;

    Lock_CPlatformDataMgr;

	
    map<string,set<long>>::iterator it_Index2;
    set<long>::iterator itFTID;
	map<long,PlatformStru_TradeTotalInfo*>::iterator it_tradetotal2;
	PlatformStru_TradeTotalInfo TradeTotal;

    it_Index2=m_TradeTotalData.m_allIndex2.find(strInstrument);
    if(it_Index2!=m_TradeTotalData.m_allIndex2.end()&&!it_Index2->second.empty())
    {
        for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
        {
		    it_tradetotal2=m_TradeTotalData.m_all2.find(*itFTID);
    		
		    if(it_tradetotal2!=m_TradeTotalData.m_all2.end()&&
                it_tradetotal2->second!=NULL)
		    {
                TradeTotalKey tmpkey(*(it_tradetotal2->second));
                if(m_RefTrades.GetTradeTotalValue(tmpkey,TradeTotal)&&
			       TradeTotal.TradeCommission!=it_tradetotal2->second->TradeCommission)
			    {
				    it_tradetotal2->second->TradeCommission=TradeTotal.TradeCommission;
                    it_tradetotal2->second->UpdateSeq=m_TradeTotalData.NewUpdateSeq(it_tradetotal2->second->FTID);
				    brlt=true;
			    }
		    }
        }
	}

    Unlock_CPlatformDataMgr;

	return brlt;
}

//�ɽ��ر�ʱ��������Ӧ��Լ�ĳɽ�ͳ��
void CPlatformDataMgr_TradeTotals::UpdateTradeTotal_RtnTrade(const PlatformStru_TradeInfo& TradeInfo)
{
    Lock_CPlatformDataMgr;

    TradeTotalKey tmpKey(TradeInfo);

    map<TradeTotalKey,PlatformStru_TradeTotalInfo*>::iterator it=m_TradeTotalData.m_all.find(tmpKey);
    if(it==m_TradeTotalData.m_all.end()||it->second==NULL)
    {
        PlatformStru_TradeTotalInfo TradeTotal;

        strncpy(TradeTotal.Account,TradeInfo.Account,sizeof(TradeTotal.Account)-1);
        strncpy(TradeTotal.ExchangeID,TradeInfo.ExchangeID,sizeof(TradeTotal.ExchangeID)-1);
        strncpy(TradeTotal.InstrumentID,TradeInfo.InstrumentID,sizeof(TradeTotal.InstrumentID)-1);
        TradeTotal.Direction=TradeInfo.Direction;
        TradeTotal.OffsetFlag=TradeInfo.OffsetFlag;
        TradeTotal.price = TradeInfo.Price;
        TradeTotal.volume = TradeInfo.Volume;
	    TradeTotal.TradeCommission = TradeInfo.TradeCommission;

        m_TradeTotalData.NewReach_Internal(TradeTotal);
    }
    else
    {
        if(it->second->volume+TradeInfo.Volume==0) it->second->price=0;
        else it->second->price = (it->second->price*it->second->volume+TradeInfo.Price*TradeInfo.Volume)/(it->second->volume+TradeInfo.Volume);
        it->second->volume += TradeInfo.Volume;
        if(it->second->TradeCommission!=util::GetDoubleInvalidValue())
        {
	        if(util::isInvalidValue(TradeInfo.TradeCommission))
		        it->second->TradeCommission=util::GetDoubleInvalidValue();
	        else
		        it->second->TradeCommission+=TradeInfo.TradeCommission;
        }

        it->second->UpdateSeq=m_TradeTotalData.NewUpdateSeq(it->second->FTID);
    }

    Unlock_CPlatformDataMgr;

}

///��ȡȫ���ɽ�ͳ�Ƽ�¼
int CPlatformDataMgr_TradeTotals::GetAllTradeTotals(vector<PlatformStru_TradeTotalInfo>& outData)
{
    int irlt;

    Lock_CPlatformDataMgr;

    irlt=m_TradeTotalData.GetAll2_Internal(outData);

    Unlock_CPlatformDataMgr;

    return irlt;
}

///��ȡָ����Լ�ĳɽ�ͳ�Ƽ�¼�����������������
int CPlatformDataMgr_TradeTotals::GetTradeTotalOfInstrument(const string& strInstrument,vector<PlatformStru_TradeTotalInfo>& outData)
{
    int irlt;

    Lock_CPlatformDataMgr;

    irlt=m_TradeTotalData.GetAllAboutInstrument2_Internal(strInstrument,outData);

    Unlock_CPlatformDataMgr;

    return irlt;
}

void CPlatformDataMgr_TradeTotals::clear(void)
{
    Lock_CPlatformDataMgr;

    m_TradeTotalData.Clear();

    Unlock_CPlatformDataMgr;
}
bool CPlatformDataMgr_TradeTotals::empty(void)
{
    Lock_CPlatformDataMgr;
    bool brlt = m_TradeTotalData.empty();
    Unlock_CPlatformDataMgr;
    return brlt;
}


