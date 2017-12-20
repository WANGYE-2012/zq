//ά���ײ���ϳֲ���Ϣ

#include "stdafx.h"

#include "wx/file.h"
#include "wx/textfile.h"

#include <vector>
#include <string>
#include <map>

#include "ServiceProxy.hpp"
#include "PlatFormService.hpp"
#include "QryQueue.h"
#include "PlatformDataMgr.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "../inc/Module-Misc/tools_util.h"
#include "../inc/Module-Misc/orderCommonFunc.h"
#pragma warning(disable:4996)

#if 1
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
    char logbuf[256];\
    logbuf[0]=1;\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CPlatformDataMgr", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif


//��ѯ��ϳֲּ�¼�Ľ��������ע�⣬�����ǲ�ѯָ����Լ����ϳֲ�
void CPlatformDataMgr::QryPositionCombReach(PlatformStru_PositionComb& PositionCombInfo,const int ErrID,const bool bIsLast)
{
    LOG_INFO("CPlatformDataMgr::QryPositionCombReach:InstrumentID=%s",PositionCombInfo.InstrumentID);

#ifndef _USE_COMBOINSTRUMENT
    return;
#endif

    Lock_CPlatformDataMgr;

	PlatformStru_PositionComb newComboPosition;
	if ( MergeOriSingleLegPosition(PositionCombInfo, newComboPosition))
		m_PositionCombData.QryRltReach_Internal(newComboPosition, ErrID, bIsLast);
    else if(bIsLast)
		m_PositionCombData.QryRltReach_Internal(newComboPosition, -1, bIsLast);

	if ( bIsLast )
	{
		m_tempOriPositionDetail.clear();
	}

    Unlock_CPlatformDataMgr;

    LOG_INFO("CPlatformDataMgr::QryPositionCombReach-end");

}

//�ϲ���ʼ����ѯ�õ��ĵ��ȳֲ�Ϊһ����ϳֲ�
bool CPlatformDataMgr::MergeOriSingleLegPosition( const PlatformStru_PositionComb &PositionCombo, PlatformStru_PositionComb& newComboPosition )
{
	//�����ȵĳɽ��ر�������ʱ����ӻ����һ����ϳֲּ�¼
	std::map<std::string, PlatformStru_PositionComb>::iterator it_comTradeID = m_combinLegs.find(PositionCombo.ComTradeID);
	if ( it_comTradeID == m_combinLegs.end())
	{
		m_combinLegs[PositionCombo.ComTradeID] = PositionCombo;
		return false;
	}
	else
	{
		strcpy(newComboPosition.TradingDay, PositionCombo.TradingDay);
		strcpy(newComboPosition.OpenDate, PositionCombo.OpenDate);
		strcpy(newComboPosition.ExchangeID, PositionCombo.ExchangeID);
		newComboPosition.SettlementID = PositionCombo.SettlementID;
		strcpy(newComboPosition.BrokerID, PositionCombo.BrokerID);
		strcpy(newComboPosition.InvestorID, PositionCombo.InvestorID);
		strcpy(newComboPosition.ComTradeID, PositionCombo.ComTradeID);
		strcpy(newComboPosition.szAccount, PositionCombo.szAccount);
		strcpy(newComboPosition.InstrumentID, PositionCombo.InstrumentID);

		double dPrice1, dPrice2;
		std::map<std::string, PlatformStru_PositionDetail>::iterator it_price1 = m_tempOriPositionDetail.find(PositionCombo.Leg1TradeID);
		if ( it_price1 != m_tempOriPositionDetail.end())
		{
			dPrice1 = it_price1->second.OpenPrice;
		}
		else
		{
			return false;
		}

		std::map<std::string, PlatformStru_PositionDetail>::iterator it_price2 = m_tempOriPositionDetail.find(it_comTradeID->second.Leg1TradeID);
		if ( it_price2 != m_tempOriPositionDetail.end())
		{
			dPrice2 = it_price2->second.OpenPrice;
		}
		else
		{
			return false;
		}

		//�жϷ�����Ϻ�Լ�ķ�������ں�Լ��һ��
		if ( 0 == PositionCombo.Leg1ID )
		{
			newComboPosition.Direction = PositionCombo.Direction;
			newComboPosition.OpenPrice = dPrice1 - dPrice2;
			strcpy(newComboPosition.Leg1TradeID, PositionCombo.Leg1TradeID);
			strcpy(newComboPosition.Leg1InstrumentID, PositionCombo.Leg1InstrumentID);
			newComboPosition.Leg1Margin = PositionCombo.Leg1Margin;
			newComboPosition.Leg1ExchMargin = PositionCombo.Leg1ExchMargin;
			newComboPosition.Leg1MarginRateByMoney = PositionCombo.Leg1MarginRateByMoney;
			newComboPosition.Leg1MarginRateByVolume = PositionCombo.Leg1MarginRateByVolume;
			newComboPosition.Leg1ID = PositionCombo.Leg1ID;
			newComboPosition.Leg1Multiple = PositionCombo.Leg1Multiple;

			strcpy(newComboPosition.Leg2TradeID, it_comTradeID->second.Leg1TradeID);
			strcpy(newComboPosition.Leg2InstrumentID, it_comTradeID->second.Leg1InstrumentID);
			newComboPosition.Leg2Margin = it_comTradeID->second.Leg1Margin;
			newComboPosition.Leg2ExchMargin = it_comTradeID->second.Leg1ExchMargin;
			newComboPosition.Leg2MarginRateByMoney = it_comTradeID->second.Leg1MarginRateByMoney;
			newComboPosition.Leg2MarginRateByVolume = it_comTradeID->second.Leg1MarginRateByVolume;
			newComboPosition.Leg2ID = it_comTradeID->second.Leg1ID;
			newComboPosition.Leg2Multiple = it_comTradeID->second.Leg1Multiple;
		}
		else if ( 1 == PositionCombo.Leg1ID )
		{
			newComboPosition.Direction = it_comTradeID->second.Direction;
			newComboPosition.OpenPrice = dPrice2 - dPrice1;
			strcpy(newComboPosition.Leg2TradeID, PositionCombo.Leg1TradeID);
			strcpy(newComboPosition.Leg2InstrumentID, PositionCombo.Leg1InstrumentID);
			newComboPosition.Leg2Margin = PositionCombo.Leg1Margin;
			newComboPosition.Leg2ExchMargin = PositionCombo.Leg1ExchMargin;
			newComboPosition.Leg2MarginRateByMoney = PositionCombo.Leg1MarginRateByMoney;
			newComboPosition.Leg2MarginRateByVolume = PositionCombo.Leg1MarginRateByVolume;
			newComboPosition.Leg2ID = PositionCombo.Leg1ID;
			newComboPosition.Leg2Multiple = PositionCombo.Leg1Multiple;

			strcpy(newComboPosition.Leg1TradeID, it_comTradeID->second.Leg1TradeID);
			strcpy(newComboPosition.Leg1InstrumentID, it_comTradeID->second.Leg1InstrumentID);
			newComboPosition.Leg1Margin = it_comTradeID->second.Leg1Margin;
			newComboPosition.Leg1ExchMargin = it_comTradeID->second.Leg1ExchMargin;
			newComboPosition.Leg1MarginRateByMoney = it_comTradeID->second.Leg1MarginRateByMoney;
			newComboPosition.Leg1MarginRateByVolume = it_comTradeID->second.Leg1MarginRateByVolume;
			newComboPosition.Leg1ID = it_comTradeID->second.Leg1ID;
			newComboPosition.Leg1Multiple = it_comTradeID->second.Leg1Multiple;
		}
		else
		{
			return false;
		}

		newComboPosition.TotalAmt = PositionCombo.TotalAmt;
		newComboPosition.HedgeFlag = PositionCombo.HedgeFlag;
		m_combinLegs.erase(it_comTradeID);
		return true;
	}
}

void CPlatformDataMgr::CreatePositionDetailCombFromOpenTrade_Internal( PlatformStru_TradeInfo &TradeInfo )
{
	//�����ȵĳɽ��ر�������ʱ����ӻ����һ����ϳֲּ�¼
	std::map<std::string, PlatformStru_TradeInfo>::iterator it_orderSys = m_tempTradeCombinLegs.find(TradeInfo.OrderSysID);
	if ( it_orderSys == m_tempTradeCombinLegs.end())
	{
        //����һ�ȳɽ��浽map�У��ȴ��ڶ��ȳɽ�
		m_tempTradeCombinLegs[TradeInfo.OrderSysID] = TradeInfo;
        LOG_INFO("CPlatformDataMgr::CreatePositionDetailCombFromOpenTrade_Internal(�յ���ϳֲ���ϸ�ĵ�һ�ȳɽ��ر�):%s",TradeInfo.tostring(logbuf,sizeof(logbuf)));
	}
	else
	{
        //���ȳɽ��������ˣ����к�������
        LOG_INFO("CPlatformDataMgr::CreatePositionDetailCombFromOpenTrade_Internal(�յ���ϳֲ���ϸ�ĵڶ��ȳɽ��ر�):%s",TradeInfo.tostring(logbuf,sizeof(logbuf)));
		PlatformStru_PositionComb newComboPosition;
		string strComInstrument;
        string strLeg1InstrumentID,strLeg2InstrumentID;

        //�������Ⱥ�Լ����ȡ��ϵ��ĺ�Լ����
		if ( !GetComboInstrumentID_Internal(it_orderSys->second.InstrumentID, TradeInfo.InstrumentID, 
                                            strComInstrument,strLeg1InstrumentID,strLeg2InstrumentID))
        {
            LOG_INFO("CPlatformDataMgr::CreatePositionDetailCombFromOpenTrade_Internal(��ȡ��Ϻ�ԼIDʧ��)");
            return;
        }

		strncpy(newComboPosition.szAccount, TradeInfo.szAccount, sizeof(newComboPosition.szAccount)-1);
	    strncpy(newComboPosition.InstrumentID, strComInstrument.c_str(), sizeof(newComboPosition.InstrumentID)-1);

		strncpy(newComboPosition.TradingDay, TradeInfo.TradingDay, sizeof(newComboPosition.TradingDay)-1);
		strncpy(newComboPosition.OpenDate, TradeInfo.TradeDate, sizeof(newComboPosition.OpenDate)-1);
		strncpy(newComboPosition.ExchangeID, TradeInfo.ExchangeID, sizeof(newComboPosition.ExchangeID)-1);
		newComboPosition.SettlementID = TradeInfo.SettlementID;
		strncpy(newComboPosition.BrokerID, TradeInfo.BrokerID, sizeof(newComboPosition.BrokerID)-1);
		strncpy(newComboPosition.InvestorID, TradeInfo.InvestorID, sizeof(newComboPosition.InvestorID)-1);

        newComboPosition.HedgeFlag = TradeInfo.HedgeFlag;
        newComboPosition.TotalAmt = TradeInfo.Volume;

        //�жϷ�����Ϻ�Լ�ķ�������ں�Լ��һ��
        if(strLeg1InstrumentID==std::string(it_orderSys->second.InstrumentID))
        {
            newComboPosition.Direction = it_orderSys->second.Direction;
			newComboPosition.OpenPrice = it_orderSys->second.Price - TradeInfo.Price;

            strncpy(newComboPosition.Leg1TradeID, it_orderSys->second.TradeID, sizeof(newComboPosition.Leg1TradeID)-1 );
            strncpy(newComboPosition.Leg1InstrumentID, it_orderSys->second.InstrumentID, sizeof(newComboPosition.Leg1InstrumentID)-1 );
            newComboPosition.Leg1ID=0;
            newComboPosition.Leg1Multiple=1;

            strncpy(newComboPosition.Leg2TradeID, TradeInfo.TradeID, sizeof(newComboPosition.Leg2TradeID)-1 );
            strncpy(newComboPosition.Leg2InstrumentID, TradeInfo.InstrumentID, sizeof(newComboPosition.Leg2InstrumentID)-1 );
            newComboPosition.Leg2ID=1;
            newComboPosition.Leg2Multiple=1;
        }
        else
        {
            newComboPosition.Direction = TradeInfo.Direction;
			newComboPosition.OpenPrice = TradeInfo.Price - it_orderSys->second.Price ;

            strncpy(newComboPosition.Leg1TradeID, TradeInfo.TradeID, sizeof(newComboPosition.Leg1TradeID)-1 );
            strncpy(newComboPosition.Leg1InstrumentID, TradeInfo.InstrumentID, sizeof(newComboPosition.Leg1InstrumentID)-1 );
            newComboPosition.Leg1ID=0;
            newComboPosition.Leg1Multiple=1;

            strncpy(newComboPosition.Leg2TradeID, it_orderSys->second.TradeID, sizeof(newComboPosition.Leg2TradeID)-1 );
            strncpy(newComboPosition.Leg2InstrumentID, it_orderSys->second.InstrumentID, sizeof(newComboPosition.Leg2InstrumentID)-1 );
            newComboPosition.Leg2ID=1;
            newComboPosition.Leg2Multiple=1;
        }

		m_tempTradeCombinLegs.erase(it_orderSys);
		m_PositionCombData.NewReach_Internal(newComboPosition);
        LOG_INFO("CPlatformDataMgr::CreatePositionDetailCombFromOpenTrade_Internal(������ϳֲ���ϸ��¼):%s",newComboPosition.tostring(logbuf,sizeof(logbuf)));
	}
}

int CPlatformDataMgr::GetPositionCombs(std::vector<PlatformStru_PositionComb>& outData)
{
	outData.clear();

    Lock_CPlatformDataMgr;

	std::map<PositionCombKey,PlatformStru_PositionComb*>::iterator it = m_PositionCombData.m_all.begin();
	for ( ; it != m_PositionCombData.m_all.end(); it++ )
	{
        if ( it->second!=NULL && it->second->TotalAmt > 0 )
		{
            outData.push_back(*(it->second));
		}
	}

    Unlock_CPlatformDataMgr;

    return outData.size();
}



int CPlatformDataMgr::GetPositionCombs2(const std::string& strTradeID, std::string& strInstrument,std::vector<PlatformStru_PositionComb>& outData)
{
	outData.clear();

    Lock_CPlatformDataMgr;

    std::map<PositionCombKey,PlatformStru_PositionComb*>::iterator it = m_PositionCombData.m_all.begin();
	for ( ; it != m_PositionCombData.m_all.end(); it++ )
	{
        if ( it->second!=NULL && 
            (strTradeID == it->second->Leg1TradeID || 
             strTradeID == it->second->Leg2TradeID ))
		{
			strInstrument = it->second->InstrumentID;
			if ( it->second->TotalAmt > 0)
			{
                outData.push_back(*(it->second));
			}
		}
	}

    Unlock_CPlatformDataMgr;

    return outData.size();
}

PlatformStru_PositionComb& CPlatformDataMgr::GetPositionComb3_Internal(const std::string& strTradeID)
{
    std::map<PositionCombKey,PlatformStru_PositionComb*>::iterator it = m_PositionCombData.m_all.begin();
	for ( ; it != m_PositionCombData.m_all.end(); it++ )
	{
        if ( it->second!=NULL &&
             (strTradeID == it->second->Leg1TradeID || 
              strTradeID == it->second->Leg2TradeID ))
		{
            return *(it->second);
		}
	}
    return PlatformStru_PositionComb::GetInvalidRecord();
}



//��ղ�ѯ��ϳֲֵ�map
void CPlatformDataMgr::ClearMapQryRltPositionCombs(std::string& strQryInstrument)
{
    Lock_CPlatformDataMgr;

    m_PositionCombData.WillQry_Internal(strQryInstrument);

    Unlock_CPlatformDataMgr;

}


//��ȡ��ѯ�ĺ�ԼID
void CPlatformDataMgr::GetQryPositionCombInstrument(std::string& outQryInstrument)
{
    Lock_CPlatformDataMgr;

    m_PositionCombData.GetQryInstrument(outQryInstrument);

    Unlock_CPlatformDataMgr;

}

int CPlatformDataMgr::GetPositionCombFTIDList(std::vector<long>& vec)
{
	vec.clear();
	Lock_CPlatformDataMgr;
	std::set<long>::iterator it_FTID = m_PositionCombData.m_allFTID.begin();
	for ( ; it_FTID != m_PositionCombData.m_allFTID.end(); ++it_FTID )
	{
		vec.push_back(*it_FTID);
	}
	Unlock_CPlatformDataMgr;

	return vec.size();
}

int CPlatformDataMgr::GetPositionCombInfo(long lFTID, PlatformStru_PositionComb& outData)
{
	int nRet = 0;
	Lock_CPlatformDataMgr;
	std::map<long, PlatformStru_PositionComb*>::iterator it_FTID = m_PositionCombData.m_all2.find(lFTID);
	if ( it_FTID != m_PositionCombData.m_all2.end())
	{
		outData = *(it_FTID->second);
	}
	else
	{
		nRet = -1;
	}
	Unlock_CPlatformDataMgr;

	return nRet;
}