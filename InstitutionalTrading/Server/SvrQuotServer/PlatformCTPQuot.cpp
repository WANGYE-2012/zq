//#include "stdafx.h"
//
#include <windows.h>
#include "PlatformCTPQuot.h"
#include "FileOpr.h"
#include "tools_util.h"

using std::make_pair;

#pragma unmanaged


#if 0
#define LOG_INFO(fmt, ...)  ;
#else
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","PlatformCTPQuot", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif




//*************************************************************************
//  CPlatformCTPQuot��ʵ��
//*************************************************************************
CPlatformCTPQuot::CPlatformCTPQuot(const Stru_CTPConnectionParam& ConnParam,bool bPushCache,bool bUseComb,bool bCalcuCombQuotOnLegQuot)
:   CPlatformCTP(ConnParam,bPushCache,bUseComb,bCalcuCombQuotOnLegQuot),
	m_pQuotApi(NULL)
{    
}
CPlatformCTPQuot::~CPlatformCTPQuot()
{    
    Clear(); 

    if(m_pQuotApi)
        Logout();
}

void CPlatformCTPQuot::Clear()
{
    CeasymutexGuard guard(m_mutex);

    m_SubscribedInstrumentID.clear();
    m_LegInstrument2CombSubscribed.clear();
    m_lastQuot.clear();

    memset(m_TradeDay,0,sizeof(TThostFtdcDateType));
}

void CPlatformCTPQuot::GetTradeDay(TThostFtdcDateType szTradeDay)
{
    CeasymutexGuard guard(m_mutex);
    memcpy(szTradeDay,m_TradeDay,sizeof(TThostFtdcDateType));
}

bool CPlatformCTPQuot::Login(const CThostFtdcMdSpi* pSpi)
{
    CeasymutexGuard guard(m_mutex);

    if(m_pQuotApi) 
    {
        LOG_INFO("Login()�ɹ����Ѿ�����Чm_pQuotApi(%x)",m_pQuotApi);
        return true;
    }
    if(m_ConnParam.BrokerID.empty()||m_ConnParam.UserID.empty()||m_ConnParam.UserPW.empty())
    {
        LOG_INFO("Login()ʧ�ܣ���¼����(BrokerID��UserID��UserPW)Ϊ��");
        return false;
    }

	SetConnStatus(CTPCONNSTATUS_Connecting,true);

    //����Api����
    m_pQuotApi = CThostFtdcMdApi::CreateFtdcMdApi();

    //ע��ص�
    m_pQuotApi->RegisterSpi(static_cast<CThostFtdcMdSpi*>(pSpi?const_cast<CThostFtdcMdSpi*>(pSpi):this));

    //ע��ǰ�û���ַ
    string addrstr;
    for(vector<string>::iterator it=m_ConnParam.FrontAddrs.begin();it!=m_ConnParam.FrontAddrs.end();++it)
    {
        addrstr=MakeConnectStr(*it,m_ConnParam.ProxyConnectStr);
		m_pQuotApi->RegisterFront(const_cast<char*>(addrstr.c_str()));
    }

    //��ʼ��������ǰ�û��ӿ�
	m_pQuotApi->Init();     

    LOG_INFO("Login CTP��m_pQuotApi(%x)",m_pQuotApi);

    return true;
}
void CPlatformCTPQuot::Logout(void)
{
	m_mutex.lock();
    if(!m_pQuotApi) 
    {
        LOG_INFO("Logout()�ɹ���m_pQuotApiΪNULL");
        return;
    }
	SetConnStatus(CTPCONNSTATUS_Disconnecting,true);
	m_mutex.unlock();

	//ע�⣬Release() �Ὣʣ��δ����������ûص������������ط������Ļ�������ص����������������
    m_pQuotApi->Release();

	m_mutex.lock();
    m_pQuotApi=NULL;
	SetConnStatus(CTPCONNSTATUS_Disconnected,true);
    LOG_INFO("Logout()�ɹ�");
	m_mutex.unlock();
}

int CPlatformCTPQuot::SubscribeMarketData(const string& InstrumentID)
{
    CeasymutexGuard guard(m_mutex);

    int ret = -999;
	if(InstrumentID.empty()) return ret;

    map<string,int>::iterator it=m_SubscribedInstrumentID.find(InstrumentID);
    if(it==m_SubscribedInstrumentID.end())
    {
        m_SubscribedInstrumentID.insert(make_pair(InstrumentID,1));

        string Leg1InstrumentID,Leg2InstrumentID;
        if(IsCombInstrument(InstrumentID,Leg1InstrumentID,Leg2InstrumentID))
        {
            m_LegInstrument2CombSubscribed.insert(make_pair(Leg1InstrumentID,InstrumentID));
            m_LegInstrument2CombSubscribed.insert(make_pair(Leg2InstrumentID,InstrumentID));
        }

		it=m_SubscribedInstrumentID.find(InstrumentID);
    }
	else it->second++;

    if(it!=m_SubscribedInstrumentID.end())
	{
		if(it->second==1&&m_pQuotApi && GetConnStatus()==CTPCONNSTATUS_Connected)
		{
			char *pInstrumentID = const_cast<char*>(InstrumentID.c_str());
            //Sleep(1000);
			ret=m_pQuotApi->SubscribeMarketData(&pInstrumentID,1);
			LOG_INFO("SubscribeMarketData(����ָ����Լ����):ret=[%d],InstrumentID=[%s]",ret, pInstrumentID);
		}
	}

    return 0;
}
int CPlatformCTPQuot::UnSubscribeMarketData(const string& InstrumentID)
{
    CeasymutexGuard guard(m_mutex);

    int ret = -999;

	if(InstrumentID.empty()) return ret;

    map<string,int>::iterator it=m_SubscribedInstrumentID.find(InstrumentID);
    if(it!=m_SubscribedInstrumentID.end())
    {		
		
/*		LOG_INFO("UnSubscribeMarketData(�����鶩���б���ɾ����Լ):InstrumentID=[%s],��Լ���ļ���=[%d]",InstrumentID.c_str(),it->second);

        if(it->second==0)
        {
            if(m_pQuotApi && GetConnStatus()==CTPCONNSTATUS_Connected)
            {
                char *pInstrumentID = const_cast<char*>(it->first.c_str());
                ret=m_pQuotApi->UnSubscribeMarketData(&pInstrumentID,1);
                LOG_INFO("UnSubscribeMarketData(ȡ�����鶩��):ret=[%d],InstrumentID=[%s]",ret, pInstrumentID);
            }

            if(IsCombInstrument2(it->first))
            {
                for(multimap<string,string>::iterator it2=m_LegInstrument2CombSubscribed.begin();it2!=m_LegInstrument2CombSubscribed.end();)
                {
                    if(it2->second==it->first) it2=m_LegInstrument2CombSubscribed.erase(it2);
                    else it2++;
                }
            }
            m_SubscribedInstrumentID.erase(it);
        }
        */ 
        
        //����Լ����Ϊ1ʱ���������˶�ȡ����Լ�ˣ�����һ���Ͽ�ctp�ٴ����Ӻ���޷��յ����������ĵ�������
        int nCount = it->second;
        it->second--;
        if(it->second==0)
        {
            if(IsCombInstrument2(it->first))
            {
                for(multimap<string,string>::iterator it2=m_LegInstrument2CombSubscribed.begin();it2!=m_LegInstrument2CombSubscribed.end();)
                {
                    if(it2->second==it->first) it2=m_LegInstrument2CombSubscribed.erase(it2);
                    else it2++;
                }
            }
        }
        
		if(nCount>1 && m_pQuotApi && GetConnStatus()==CTPCONNSTATUS_Connected)
		{
			char *pInstrumentID = const_cast<char*>(it->first.c_str());
			ret=m_pQuotApi->UnSubscribeMarketData(&pInstrumentID,1);
            LOG_INFO("UnSubscribeMarketData(�˶�����):InstrumentID=[%s] ��Լ���ļ���=[%d]",pInstrumentID,--nCount);
            
            m_SubscribedInstrumentID.erase(it);
		}
    }
    return ret;
}
int CPlatformCTPQuot::UnSubscribeAllMarketData()
{
    CeasymutexGuard guard(m_mutex);

	LOG_INFO("UnSubscribeAllMarketData(ɾ��ȫ�����鶩���б�)");

    int ret = -999;

	if(m_pQuotApi && GetConnStatus()==CTPCONNSTATUS_Connected)
	{
		for(map<string,int>::iterator it=m_SubscribedInstrumentID.begin();it!=m_SubscribedInstrumentID.end();it++)
		{
			char *pInstrumentID = const_cast<char*>(it->first.c_str());
            ret=m_pQuotApi->UnSubscribeMarketData(&pInstrumentID,1);
            LOG_INFO("UnSubscribeAllMarketData(ȡ�����鶩��):ret=[%d],InstrumentID=[%s]",ret, pInstrumentID);
		}
	}
	m_SubscribedInstrumentID.clear();
	m_LegInstrument2CombSubscribed.clear();
    return ret;
}

vector<string> CPlatformCTPQuot::GetSubscribedInstrumentIDs(void) const 
{ 
    CeasymutexGuard guard(m_mutex);  
    vector<string> rlt;
    for(map<string,int>::const_iterator it=m_SubscribedInstrumentID.begin();it!=m_SubscribedInstrumentID.end();it++)
        rlt.push_back(it->first);
    return rlt; 
}

bool CPlatformCTPQuot::GetLastQuotMap(map<string,CThostFtdcDepthMarketDataField>& mapLastQuot)
{
    mapLastQuot = m_lastQuot;
    return m_lastQuot.size()>0;
};
///****************************************************************************
/// ������ctp�ص�����
///****************************************************************************

///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CPlatformCTPQuot::OnFrontConnected()
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Quot_FrontConnected, NULL, 0);

	SetConnStatus(CTPCONNSTATUS_Logining,true);

    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strncpy(req.BrokerID, m_ConnParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
    strncpy(req.UserID, m_ConnParam.UserID.c_str(),sizeof(req.UserID)-1);
    strncpy(req.Password, m_ConnParam.UserPW.c_str(),sizeof(req.Password)-1);
	strncpy(req.OneTimePassword,m_ConnParam.OneTimePassword.c_str(),sizeof(req.OneTimePassword)-1);
    int RequestID=m_RequestID++;
    int iRlt = m_pQuotApi->ReqUserLogin(&req, RequestID);

    LOG_INFO("CPlatformCTPQuot::OnFrontConnected(�뽻�����ɹ�����TCP����):\n"
        "\t\t\t\t\t\t\t\t\t --->ReqUserLogin(�û���¼����):ret=[%d],iRequestID=[%d],BrokerID=[%s],UserID=[%s]\n",
        iRlt, RequestID,req.BrokerID, req.UserID);
}

///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void CPlatformCTPQuot::OnFrontDisconnected(int nReason)
{ 
    CeasymutexGuard guard(m_mutex);

	SetConnStatus(CTPCONNSTATUS_Disconnected,true);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Quot_FrontDisconnected,NULL,0,NULL,0,nReason);

    LOG_INFO("CPlatformCTPQuot::OnFrontDisconnected(�뽻����ʧȥTCP����):nReason=[%#x]",nReason);
}
    
///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
void CPlatformCTPQuot::OnHeartBeatWarning(int nTimeLapse)
{
	LOG_INFO("CPlatformCTPQuot::OnHeartBeatWarning:nTimeLapse=[%x]",nTimeLapse);
}

///��¼������Ӧ
void CPlatformCTPQuot::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

    if(pRspUserLogin)
    {
        DWORD tickcount=GetTickCount();
        if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Quot_RspUserLogin, pRspUserLogin,sizeof(*pRspUserLogin),pRspInfo,pRspInfo?sizeof(*pRspInfo):0,(unsigned int)nRequestID,(unsigned int)bIsLast,tickcount);

        LOG_INFO("CPlatformCTPQuot::OnRspUserLogin(��¼������Ӧ):\n"
                        "\t\t\t\t\t\t\t\t\t ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]\n"
                        "\t\t\t\t\t\t\t\t\t TradingDay=[%s],\t LoginTime=[%s],\t BrokerID=[%s],\t UserID=[%s],\t SystemName=[%s]\n"
                        "\t\t\t\t\t\t\t\t\t FrontID=[%d],\t SessionID=[%#x],\t MaxOrderRef[%s],\t SHFETime=[%s],\t DCETime=[%s],\n"
                        "\t\t\t\t\t\t\t\t\t CZCETime[%s],\t FFEXTime=[%s],\t CurTickCount=[%ul]",
                        pRspInfo?pRspInfo->ErrorID:-1, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast,
                        pRspUserLogin->TradingDay,pRspUserLogin->LoginTime,pRspUserLogin->BrokerID   , pRspUserLogin->UserID,pRspUserLogin->SystemName, 
                        pRspUserLogin->FrontID  , pRspUserLogin->SessionID ,pRspUserLogin->MaxOrderRef, pRspUserLogin->SHFETime , pRspUserLogin->DCETime, 
                        pRspUserLogin->CZCETime   , pRspUserLogin->FFEXTime,tickcount);
    }
    else
    {
        LOG_INFO("CPlatformCTPQuot::OnRspUserLogin(��¼������Ӧ),pRspUserLogin is NULL");
    }

    if(pRspUserLogin&&(!pRspInfo||pRspInfo->ErrorID==0))
    {
		SetConnStatus(CTPCONNSTATUS_Connected,true); 

        //todo ���ݺ�Լ��������  ����Ͽ���Ҫ���¶���һ�£���������ղ�������
        for(map<string,int>::const_iterator it=m_SubscribedInstrumentID.begin();it!=m_SubscribedInstrumentID.end();it++)
        {
            char *pInstrumentID = const_cast<char*>(it->first.c_str());
            m_pQuotApi->SubscribeMarketData(&pInstrumentID,1);
			LOG_INFO("CPlatformCTPQuot::OnRspUserLogin(��¼������Ӧ),���¶������飬InstrumentID = %s",pInstrumentID);
        }
    }
    else
    {
		SetConnStatus(CTPCONNSTATUS_LoginFailure,true);
    }
    
}

///����Ӧ��
void CPlatformCTPQuot::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pRspInfo)
		LOG_INFO("CPlatformCTPQuot::OnRspError : ErrCode=[%d] ErrMsg=[%s],RequestID=[%d],IsLast=[%d]",
				 pRspInfo->ErrorID,pRspInfo->ErrorMsg,nRequestID,(int)bIsLast);
}

///��������Ӧ��
void CPlatformCTPQuot::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pSpecificInstrument&&(!pRspInfo||pRspInfo->ErrorID==0))
	{
		LOG_INFO("CPlatformCTPQuot::OnRspSubMarketData(��������Ӧ��,Unimplemented) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],InstrumentID=[%s],nRequestID=[%d],bIsLast=[%d]",
			pRspInfo,pRspInfo?pRspInfo->ErrorID:0,pRspInfo?pRspInfo->ErrorMsg:"",
			pSpecificInstrument->InstrumentID,nRequestID,(int)bIsLast);
	}
	else
	{
        LOG_INFO("CPlatformCTPQuot::OnRspSubMarketData(��������Ӧ��,Unimplemented),pSpecificInstrument is NULL");
	}
}

///ȡ����������Ӧ��
void CPlatformCTPQuot::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pSpecificInstrument&&(!pRspInfo||pRspInfo->ErrorID==0))
	{
		LOG_INFO("CPlatformCTPQuot::OnRspUnSubMarketData(ȡ����������Ӧ��,Unimplemented) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],InstrumentID=[%s],nRequestID=[%d],bIsLast=[%d]",
			pRspInfo,pRspInfo?pRspInfo->ErrorID:0,pRspInfo?pRspInfo->ErrorMsg:"",
			pSpecificInstrument->InstrumentID,nRequestID,(int)bIsLast);
	}
    else
	{
        LOG_INFO("CPlatformCTPQuot::OnRspUnSubMarketData(ȡ����������Ӧ��,Unimplemented),pSpecificInstrument is NULL");
	}
}


///�������֪ͨ
void CPlatformCTPQuot::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
  
    CeasymutexGuard guard(m_mutex);
  
    if(!pDepthMarketData)
        return;

    static bool blogID=false;
    if(blogID)
    {
        LOG_INFO("CPlatformCTPQuot::OnRtnDepthMarketData:InstrumentID=%s,LastPrice=%g,UpdateTime=%s",
            pDepthMarketData->InstrumentID,pDepthMarketData->LastPrice,pDepthMarketData->UpdateTime);
    }

    CThostFtdcDepthMarketDataField MD=*pDepthMarketData;

    if(m_bUseComb&&
        IsCombInstrument2(MD.InstrumentID))
    {
        if(MD.PreSettlementPrice==util::GetDoubleInvalidValue()||
            MD.LastPrice==util::GetDoubleInvalidValue()||
            MD.LowerLimitPrice==0||
            MD.UpperLimitPrice==0)
        {
            if(m_bCalcuCombQuotOnLegQuot)
            {
                //���м��������ۡ����¼ۡ��ǵ�ͣ��
                string Leg1InstrumentID,Leg2InstrumentID;
                CThostFtdcDepthMarketDataField Leg1Quot,Leg2Quot;
                if(IsCombInstrument(MD.InstrumentID,Leg1InstrumentID,Leg2InstrumentID)&&
                    GetQuotInfo_Internal(Leg1InstrumentID,Leg1Quot)&&
                    GetQuotInfo_Internal(Leg2InstrumentID,Leg2Quot))
                {
                    if(Leg1Quot.LastPrice!=util::GetDoubleInvalidValue()&&Leg2Quot.LastPrice!=util::GetDoubleInvalidValue())
                    {
                        MD.LastPrice=Leg1Quot.LastPrice-Leg2Quot.LastPrice;
                    }
                    if(Leg1Quot.AskPrice1!=util::GetDoubleInvalidValue()&&Leg2Quot.AskPrice1!=util::GetDoubleInvalidValue())
                    {
                        MD.AskPrice1=Leg1Quot.AskPrice1-Leg2Quot.BidPrice1;
                    }
                    if(Leg1Quot.AskVolume1!=util::GetIntInvalidValue()&&Leg2Quot.BidVolume1!=util::GetIntInvalidValue())
                    {
                        MD.AskVolume1 = Leg1Quot.AskVolume1 <= Leg2Quot.BidVolume1 ? Leg1Quot.AskVolume1 : Leg2Quot.BidVolume1;
                    }
                    if(Leg1Quot.BidPrice1!=util::GetDoubleInvalidValue()&&Leg2Quot.BidPrice1!=util::GetDoubleInvalidValue())
                    {
                        MD.BidPrice1=Leg1Quot.BidPrice1-Leg2Quot.AskPrice1;
                    }
                    if(Leg1Quot.BidVolume1!=util::GetIntInvalidValue()&&Leg2Quot.AskVolume1!=util::GetIntInvalidValue())
                    {
                        MD.BidVolume1 = Leg1Quot.BidVolume1 <= Leg2Quot.AskVolume1 ? Leg1Quot.BidVolume1 : Leg2Quot.AskVolume1;
                    }
                    if(MD.LowerLimitPrice==util::GetDoubleInvalidValue()&&Leg1Quot.LowerLimitPrice!=util::GetDoubleInvalidValue()&&Leg2Quot.UpperLimitPrice!=util::GetDoubleInvalidValue())
                    {
                        MD.LowerLimitPrice=Leg1Quot.LowerLimitPrice-Leg2Quot.UpperLimitPrice;
                    }
                    if(MD.UpperLimitPrice==util::GetDoubleInvalidValue()&&Leg1Quot.UpperLimitPrice!=util::GetDoubleInvalidValue()&&Leg2Quot.LowerLimitPrice!=util::GetDoubleInvalidValue())
                    {
                        MD.UpperLimitPrice=Leg1Quot.UpperLimitPrice-Leg2Quot.LowerLimitPrice;
                    }
                    if(MD.PreSettlementPrice==util::GetDoubleInvalidValue()&&
                        Leg1Quot.PreSettlementPrice!=util::GetDoubleInvalidValue()&&
                        Leg2Quot.PreSettlementPrice!=util::GetDoubleInvalidValue())
                    {
                        MD.PreSettlementPrice=Leg1Quot.PreSettlementPrice-Leg2Quot.PreSettlementPrice;
                    }
                    //������Ϊ������������Сֵ
                    if(Leg1Quot.Volume!=util::GetIntInvalidValue()&&Leg2Quot.Volume!=util::GetIntInvalidValue())
                    {
                        MD.Volume = Leg1Quot.Volume <= Leg2Quot.Volume ? Leg1Quot.Volume : Leg2Quot.Volume;
                    }

                }
            }
        }
        if(MD.HighestPrice==0) 
            MD.HighestPrice=util::GetDoubleInvalidValue();
        if(MD.LowestPrice==0)
            MD.LowestPrice=util::GetDoubleInvalidValue();
        if(MD.Volume==0)
            MD.Volume=util::GetIntInvalidValue();
        if(MD.OpenPrice==0)
            MD.OpenPrice=util::GetDoubleInvalidValue();
        if(MD.ClosePrice==0)
            MD.ClosePrice=util::GetDoubleInvalidValue();
        if(MD.PreClosePrice==0)
            MD.PreClosePrice=util::GetDoubleInvalidValue();
    }

	if(m_bPushCache)
		m_Cache.SaveDataTo2(CmdID_Quot_RtnDepthMarketData, &MD, sizeof(MD), NULL, 0, GetTickCount()); 
	PostPkg_Internal(CmdID_Quot_RtnDepthMarketData,&MD,sizeof(MD));

    SetQuotInfo_Internal(MD);

    if(m_bUseComb&&m_bCalcuCombQuotOnLegQuot)
    {
        string ThisInstrumentID(pDepthMarketData->InstrumentID);
        vector<string> CombInstrumentIDs;
        string Leg1InstrumentID,Leg2InstrumentID;
        CThostFtdcDepthMarketDataField Leg1Quot,Leg2Quot,CombQuot;

        //���ڷ���ϵ���������ĳЩ��ϵ��ĵ��ȵ�������д���
        if(!IsCombInstrument2(ThisInstrumentID)&&
            GetCombInstrumentIDs_IncludeLeg_Subscribed_Internal(ThisInstrumentID,CombInstrumentIDs))
        {
            //��ʱCombInstrumentIDs����ź͵����йص���Ϻ�Լ
            //���ÿһ����Ϻ�Լ����������
            for(int i=0;i<(int)CombInstrumentIDs.size();i++)
            {
                if(IsCombInstrument(CombInstrumentIDs[i],Leg1InstrumentID,Leg2InstrumentID))
                {
                    //��ȡLeg1Quot��Leg2Quot
                    if(Leg1InstrumentID==ThisInstrumentID)
                    {
                        Leg1Quot=MD;
                        if(!GetQuotInfo_Internal(Leg2InstrumentID, Leg2Quot))
                            continue;
                    }
                    else if(Leg2InstrumentID==ThisInstrumentID)
                    {
                        if(!GetQuotInfo_Internal(Leg1InstrumentID, Leg1Quot))
                            continue;
                        Leg2Quot=MD;
                    }
                    else continue;

                    //��ȡCombQuot
                    if(!GetQuotInfo_Internal(CombInstrumentIDs[i], CombQuot))
                    {
                        memset(&CombQuot,0,sizeof(CombQuot));
                        strncpy(CombQuot.TradingDay,MD.TradingDay,sizeof(CombQuot.TradingDay)-1);
                        strncpy(CombQuot.InstrumentID,CombInstrumentIDs[i].c_str(),sizeof(CombQuot.InstrumentID)-1);
                        strncpy(CombQuot.ExchangeID,MD.ExchangeID,sizeof(CombQuot.ExchangeID)-1);
                        strncpy(CombQuot.ExchangeInstID,CombInstrumentIDs[i].c_str(),sizeof(CombQuot.ExchangeInstID)-1);
                        strncpy(CombQuot.UpdateTime,MD.UpdateTime,sizeof(CombQuot.UpdateTime)-1);
                        CombQuot.UpdateMillisec=MD.UpdateMillisec;
                        CombQuot.PreSettlementPrice=util::GetDoubleInvalidValue();
                    }

                    //������Leg1Quot��Leg2Quot��������ϵ�������
                    //�������¼ۡ������ۡ����������ǵ�ͣ�ۡ��������ʱ��
                    bool bUpdate=false;
                    if(Leg1Quot.LastPrice!=util::GetDoubleInvalidValue()&&Leg2Quot.LastPrice!=util::GetDoubleInvalidValue())
                    {
                        CombQuot.LastPrice=Leg1Quot.LastPrice-Leg2Quot.LastPrice;
                        bUpdate=true;
                    }
                    if(Leg1Quot.AskPrice1!=util::GetDoubleInvalidValue()&&Leg2Quot.AskPrice1!=util::GetDoubleInvalidValue())
                    {
                        CombQuot.AskPrice1=Leg1Quot.AskPrice1-Leg2Quot.BidPrice1;
                        bUpdate=true;
                    }
                    if(Leg1Quot.AskVolume1!=util::GetIntInvalidValue()&&Leg2Quot.BidVolume1!=util::GetIntInvalidValue())
                    {
                        CombQuot.AskVolume1 = Leg1Quot.AskVolume1 <= Leg2Quot.BidVolume1 ? Leg1Quot.AskVolume1 : Leg2Quot.BidVolume1;
                        bUpdate=true;
                    }
                    if(Leg1Quot.BidPrice1!=util::GetDoubleInvalidValue()&&Leg2Quot.BidPrice1!=util::GetDoubleInvalidValue())
                    {
                        CombQuot.BidPrice1=Leg1Quot.BidPrice1-Leg2Quot.AskPrice1;
                        bUpdate=true;
                    }
                    if(Leg1Quot.BidVolume1!=util::GetIntInvalidValue()&&Leg2Quot.AskVolume1!=util::GetIntInvalidValue())
                    {
                        CombQuot.BidVolume1 = Leg1Quot.BidVolume1 <= Leg2Quot.AskVolume1 ? Leg1Quot.BidVolume1 : Leg2Quot.AskVolume1;
                        bUpdate=true;
                    }
                    if(CombQuot.LowerLimitPrice==util::GetDoubleInvalidValue()&&Leg1Quot.LowerLimitPrice!=util::GetDoubleInvalidValue()&&Leg2Quot.UpperLimitPrice!=util::GetDoubleInvalidValue())
                    {
                        CombQuot.LowerLimitPrice=Leg1Quot.LowerLimitPrice-Leg2Quot.UpperLimitPrice;
                        bUpdate=true;
                    }
                    if(CombQuot.UpperLimitPrice==util::GetDoubleInvalidValue()&&Leg1Quot.UpperLimitPrice!=util::GetDoubleInvalidValue()&&Leg2Quot.LowerLimitPrice!=util::GetDoubleInvalidValue())
                    {
                        CombQuot.UpperLimitPrice=Leg1Quot.UpperLimitPrice-Leg2Quot.LowerLimitPrice;
                        bUpdate=true;
                    }
                    if(CombQuot.PreSettlementPrice==util::GetDoubleInvalidValue()&&
                        Leg1Quot.PreSettlementPrice!=util::GetDoubleInvalidValue()&&
                        Leg2Quot.PreSettlementPrice!=util::GetDoubleInvalidValue())
                    {
                        CombQuot.PreSettlementPrice=Leg1Quot.PreSettlementPrice-Leg2Quot.PreSettlementPrice;
                        bUpdate=true;
                    }
                    //������Ϊ������������Сֵ
                    if(Leg1Quot.Volume!=util::GetIntInvalidValue()&&Leg2Quot.Volume!=util::GetIntInvalidValue())
                    {
                        CombQuot.Volume = Leg1Quot.Volume <= Leg2Quot.Volume ? Leg1Quot.Volume : Leg2Quot.Volume;
                        bUpdate=true;
                    }


                    if(bUpdate)
                    {
                        //��������ϵ�������
                        int cmprlt=memcmp(MD.UpdateTime,CombQuot.UpdateTime,sizeof(CombQuot.UpdateTime));
                        if(cmprlt>=0)
                        {
                            if(cmprlt>0)
                                strncpy(CombQuot.UpdateTime,MD.UpdateTime,sizeof(CombQuot.UpdateTime)-1);
                            if(cmprlt==0&&MD.UpdateMillisec>CombQuot.UpdateMillisec)
                                CombQuot.UpdateMillisec=MD.UpdateMillisec;
                        }

                        SetQuotInfo_Internal(CombQuot);
						if(m_bPushCache)
							m_Cache.SaveDataTo2(CmdID_Quot_RtnDepthMarketData, &CombQuot, sizeof(CombQuot), NULL, 0, GetTickCount()); 
						PostPkg_Internal(CmdID_Quot_RtnDepthMarketData,&CombQuot,sizeof(CombQuot));
                    }
                }
            }
        }
    }
}
///��ȡ�����˵��Ⱥ�Լ����Ϻ�Լ�б�
bool CPlatformCTPQuot::GetCombInstrumentIDs_IncludeLeg_Subscribed_Internal(const string& LegInstrument,vector<string>& vecCombInstruments)
{
    bool ret=false;
    multimap<string,string>::iterator itlower=m_LegInstrument2CombSubscribed.lower_bound(LegInstrument);
    multimap<string,string>::iterator itupper=m_LegInstrument2CombSubscribed.upper_bound(LegInstrument);
    vecCombInstruments.clear();
    while(itlower!=itupper)
    {
        vecCombInstruments.push_back(itlower->second);
        itlower++;
    }
    return vecCombInstruments.size()==0?false:true;
}


//����ָ����Լ���飬���õ�������£��򷵻�true�����򷵻�false����ԭ����������ϣ�
//������OnRtnDepthMarketData()��
bool CPlatformCTPQuot::SetQuotInfo_Internal(const CThostFtdcDepthMarketDataField& inData)
{
    bool brlt=true;

    map<string,CThostFtdcDepthMarketDataField>::iterator it = m_lastQuot.find(string(inData.InstrumentID));
    m_lastQuot[string(inData.InstrumentID)] = inData;

    return brlt;
}

//��ȡָ����Լ���顣������OnRtnDepthMarketData()��
bool CPlatformCTPQuot::GetQuotInfo_Internal(const string& InstrumentID, CThostFtdcDepthMarketDataField& outData)
{
    bool brlt=false;

    map<string,CThostFtdcDepthMarketDataField>::iterator it =  m_lastQuot.find(InstrumentID);
    if(it==m_lastQuot.end())
    {
        memset(&outData,0,sizeof(outData));
    }
    else
    {
        outData = it->second;
        brlt=true;
    }
    return brlt;
}

//�ж�һ����Լ�Ƿ�����ϵ���Լ������ǣ����ص��Ⱥ�Լ���ơ�
//Ŀǰ�����жϴ��̺�֣�̵���Ϻ�Լ
//�ж�������ǰ�пո����&����SP c1305&c1309
bool CPlatformCTPQuot::IsCombInstrument(const string& strInstrument,string& LegInstrument1,string& LegInstrument2)
{
    int len=strInstrument.length();
    int pos1=strInstrument.find(" ");
    if(pos1==-1) return false;
    int pos2=strInstrument.find("&",pos1);
    if(pos2==-1) return false;
    LegInstrument1=strInstrument.substr(pos1+1,pos2-pos1-1);
    LegInstrument2=strInstrument.substr(pos2+1,len-pos2-1);
    return true;
}
//�ж�һ����Լ�Ƿ�����ϵ���Լ��
//Ŀǰ�����жϴ��̺�֣�̵���Ϻ�Լ
//�ж�������ǰ�пո����&����SP c1305&c1309
bool CPlatformCTPQuot::IsCombInstrument2(const string& strInstrument)
{
    int len=strInstrument.length();
    int pos1=strInstrument.find(" ");
    if(pos1==-1) return false;
    int pos2=strInstrument.find("&",pos1);
    if(pos2==-1) return false;
    return true;
}


