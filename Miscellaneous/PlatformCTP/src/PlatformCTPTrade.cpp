//#include "stdafx.h"
//
#include <windows.h>
#include "PlatformCTPTrade.h"
#include "FileOpr.h"
#include "Tools_Ansi.h"
#include "Tools_Win32.h"


#pragma unmanaged


#if 0
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","PlatformCTPTrade", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__)
#endif

#define strncpy_ctptrade(dest,sour)	{ memset((dest),0,sizeof(dest));  strncpy((dest),(sour),sizeof(dest)-1); }


//*************************************************************************
//  CQryQueue��ʵ��
//*************************************************************************

CPlatformCTPTrade::CQryQueue::CQryQueue(CPlatformCTPTrade& RefTrade)
:	m_RefTrade(RefTrade),
	m_ReqQueue(20*1024*1024,0),
	m_bInQry(false),
	m_QryTime(0),
	m_bNeedExit(false),
	m_hThread(0)
{
    DWORD threadId;
    m_hThread=(int)CreateThread(NULL,2*1024*1024,(LPTHREAD_START_ROUTINE)ThreadProc,this,0,&threadId);
}
CPlatformCTPTrade::CQryQueue::~CQryQueue(void)
{
	m_bNeedExit=true;
	CTools_Win32::StopThread((HANDLE)m_hThread);
	m_hThread=NULL;
}
//�Ƿ񵽴��ѯʱ�̡�ctp�涨ÿ��ֻ����һ�β�ѯ 
bool CPlatformCTPTrade::CQryQueue::IsQryTime(void)
{
    DWORD CurTickCount=GetTickCount();
    if(!m_bInQry)
    {                                                       //û����;��ѯ
        if(CurTickCount>=m_QryTime && CurTickCount-m_QryTime > 1000 ||
            CurTickCount<m_QryTime && CurTickCount+((DWORD)0xffffffff-m_QryTime) > 1000)
            return true;
    }
    else
    {                                                       //����;��ѯ����ʱΪ8s
        if(CurTickCount>=m_QryTime && CurTickCount-m_QryTime > 8000 ||
            CurTickCount<m_QryTime && CurTickCount+((DWORD)0xffffffff-m_QryTime) > 8000)
            return true;
    }
    return false;
}
bool CPlatformCTPTrade::CQryQueue::IsCacheEmpty(void)
{
	return m_ReqQueue.IsEmpty();
}
//���Ͳ�ѯ���󣬷���ֵ��ʾ�Ƿ�ɹ���������
//ReqRlt��ʾ��������˲�ѯ����ѯ����ֵ��<0��ʾ��ѯʧ��
bool CPlatformCTPTrade::CQryQueue::SendQryCmd(int& ReqRlt)
{
	if(m_RefTrade.GetConnStatus()!=CTPCONNSTATUS_Connected) return false;
	if(!m_RefTrade.m_pTradeApi) return false;

	char Buf[10240];
	CVariablePackage::HeadStru PackageHead;

	ReqRlt=0;
	bool bProcessQueue=false;

	while(!m_ReqQueue.IsEmpty())
	{
		if(m_ReqQueue.GetCurPackageDataLen()>sizeof(Buf))
		{
			m_ReqQueue.DeleteCurPackage();
			continue;
		}

		PackageHead=m_ReqQueue.PeekFrom(Buf,sizeof(Buf));

		switch(PackageHead.CmdID)
		{
			case QryID_ReqQryNotice:						//�����ѯ�ͻ�֪ͨ
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryNoticeField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryNotice((CThostFtdcQryNoticeField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ͻ�֪ͨ)ReqQryNotice RequestID=%d Rlt=%d",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQrySettlementInfoConfirm:         //�����ѯ������Ϣȷ��
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQrySettlementInfoConfirmField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQrySettlementInfoConfirm((CThostFtdcQrySettlementInfoConfirmField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (��ѯ������Ϣȷ��)ReqQrySettlementInfoConfirm RequestID=%d Rlt=%d",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQrySettlementInfo:				//��ѯͶ���߽�����
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQrySettlementInfoField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQrySettlementInfo((CThostFtdcQrySettlementInfoField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (��ѯͶ���߽�����)ReqQrySettlementInfo RequestID=%d Rlt=%d",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryInstrument:                    //��ѯ��Լ��Ϣ
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryInstrumentField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryInstrument((CThostFtdcQryInstrumentField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (��ѯ��Լ��Ϣ)ReqQryInstrument : InstrumentID=%s RequestID=%d Rlt=%d",((CThostFtdcQryInstrumentField*)Buf)->InstrumentID,PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryOrder:                         //��ѯ����
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryOrderField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryOrder((CThostFtdcQryOrderField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (��ѯ������Ϣ)QryID_ReqQryOrder RequestID=%d Rlt=%d",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryTrade:                         //��ѯ�ɽ�
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryTradeField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryTrade((CThostFtdcQryTradeField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ɽ���Ϣ)ReqQryTrade RequestID=%d Rlt=%d ",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryInvestorPosition:              //��ѯ�ֲ�
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryInvestorPositionField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryInvestorPosition((CThostFtdcQryInvestorPositionField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ֲֻ���)ReqQryInvestorPosition RequestID=%d Rlt=%d",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryInvestorPositionDetail:        //��ѯ�ֲ���ϸ
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryInvestorPositionDetailField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryInvestorPositionDetail((CThostFtdcQryInvestorPositionDetailField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ֲ���ϸ)ReqQryInvestorPositionDetail RequestID=%d Rlt=%d",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryInvestorPositionCombineDetail:              //�����ѯͶ���ֲ߳���ϸ(���)
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryInvestorPositionCombineDetailField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryInvestorPositionCombineDetail((CThostFtdcQryInvestorPositionCombineDetailField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (��ѯͶ���ֲ߳���ϸ(���))ReqQryInvestorPositionCombineDetail RequestID=%d Rlt=%d",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryTradingAccount:                //��ѯ�ʽ�
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryTradingAccountField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryTradingAccount((CThostFtdcQryTradingAccountField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (��ѯ�ʽ���Ϣ)ReqQryTradingAccount RequestID=%d Rlt=%d",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryInstrumentCommissionRate:      //��ѯ��Լ��������

				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryInstrumentCommissionRateField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryInstrumentCommissionRate((CThostFtdcQryInstrumentCommissionRateField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (��ѯ��Լ��������)ReqQryInstrumentCommissionRate��InstrumentID=%s RequestID=%d Rlt=%d",((CThostFtdcQryInstrumentCommissionRateField*)Buf)->InstrumentID,PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryInstrumentMarginRate:          //�����ѯ��Լ��֤����
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryInstrumentMarginRateField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryInstrumentMarginRate((CThostFtdcQryInstrumentMarginRateField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (��ѯ��Լ��֤����)ReqQryInstrumentCommissionRate��InstrumentID=%s RequestID=%d Rlt=%d",((CThostFtdcQryInstrumentMarginRateField*)Buf)->InstrumentID,PackageHead.UserData1,ReqRlt);
				break;
		}

		if(!bProcessQueue)
		{
			m_ReqQueue.DeleteCurPackage();
			continue;
		}

        if(ReqRlt==0) 
        { 
            //�ɹ�������ѯ����
            m_bInQry=true;
			m_QryTime=GetTickCount();
			m_ReqQueue.DeleteCurPackage();
            return true; 
        }                                                               
        else if(ReqRlt==-2)
        {
            //��������ʧ�ܣ�δ�������󳬹��������Ҳ������һ�ε�����û�����ꡣ
            //����m_bInQry��������true���Ը��·��������ʱ�䣬����Ƶ������
            m_bInQry=true;
			m_QryTime=GetTickCount();
            return true; 
        }
        else
        {
            return false;         
        }
    }
    return false;
}
//�̺߳���
unsigned int CPlatformCTPTrade::CQryQueue::ThreadProc(void *arg)
{
    CPlatformCTPTrade::CQryQueue   *pQryQueue=(CQryQueue*)arg;
    int ReqRlt;

    while(!pQryQueue->m_bNeedExit)
    {
		int sleepms=50;

        if(pQryQueue->m_RefTrade.GetConnStatus()!=CTPCONNSTATUS_Connected) 
			sleepms=1000;
        else if( !pQryQueue->IsQryTime()) 
			sleepms=50;
        else 
        {
			CeasymutexGuard guard(pQryQueue->m_RefTrade.m_mutex);

            if(pQryQueue->SendQryCmd(ReqRlt)) sleepms=1000;
            else
            {
                if(pQryQueue->IsCacheEmpty()) sleepms=100;
                else if(ReqRlt<0) sleepms=200;
            }
        }
		if(sleepms>0) Sleep(sleepms);
    }
    return 0;
}




//*************************************************************************
//  CPlatformCTPTrade��ʵ��
//*************************************************************************
CPlatformCTPTrade::CPlatformCTPTrade(const Stru_CTPConnectionParam& ConnParam,
                                     bool bPushCache,bool bUseComb,bool bCalcuCombQuotOnLegQuot,
                                     const string& Ver_String,
									 bool bAutoReqSettlementInfoConfirmed,
									 bool bAutoConfirmSettlement,
									 bool bAutoQryInstrumentInfo)
:	CPlatformCTP(ConnParam,bPushCache,bUseComb,bCalcuCombQuotOnLegQuot),
	m_Ver_String(Ver_String),
	m_bAutoReqSettlementInfoConfirmed(bAutoReqSettlementInfoConfirmed),
    m_bAutoConfirmSettlement(bAutoConfirmSettlement),
	m_bAutoQryInstrumentInfo(bAutoQryInstrumentInfo),
    m_CurOrderRef(0),
    m_pTradeApi(NULL),
	m_pQryQueue(NULL),
	m_bSettlementInfoConfirmed(false)
{
    memset(&m_LoginInfo,0,sizeof(m_LoginInfo));
	m_pQryQueue=new CQryQueue(*this);
}

CPlatformCTPTrade::~CPlatformCTPTrade()
{
    CeasymutexGuard guard(m_mutex);

	if(m_pQryQueue)
	{
		delete m_pQryQueue;
		m_pQryQueue=NULL;
	}

    if(m_pTradeApi)
        Logout();
}
bool CPlatformCTPTrade::Login(const CThostFtdcTraderSpi* pSpi)
{
    CeasymutexGuard guard(m_mutex);

    if(m_pTradeApi) 
    {
        LOG_INFO("Login()�ɹ����Ѿ�����Чm_pTradeApi(%x)",m_pTradeApi);
        return true;
    }
    if(m_ConnParam.BrokerID.empty()||m_ConnParam.UserID.empty()||m_ConnParam.UserPW.empty())
    {
        LOG_INFO("Login()ʧ�ܣ���¼����(BrokerID��UserID��UserPW)Ϊ��");
        return false;
    }

	SetConnStatus(CTPCONNSTATUS_Connecting,false);
 
	memset(&m_LoginInfo,0,sizeof(m_LoginInfo));

    //����Api����
    m_pTradeApi = CThostFtdcTraderApi::CreateFtdcTraderApi();

    //ע��ص�
    m_pTradeApi->RegisterSpi(static_cast<CThostFtdcTraderSpi*>(pSpi?const_cast<CThostFtdcTraderSpi*>(pSpi):this));

    //���Ĺ�������˽����
    m_pTradeApi->SubscribePublicTopic(THOST_TERT_QUICK);
    m_pTradeApi->SubscribePrivateTopic(THOST_TERT_QUICK);

    //ע��ǰ�û���ַ
    string addrstr;
    for(vector<string>::iterator it=m_ConnParam.FrontAddrs.begin();it!=m_ConnParam.FrontAddrs.end();++it)
    {
        addrstr=MakeConnectStr(*it,m_ConnParam.ProxyConnectStr);
		m_pTradeApi->RegisterFront(const_cast<char*>(addrstr.c_str()));
    }

    //��ʼ��������ǰ�û��ӿ�
	m_pTradeApi->Init();     


    LOG_INFO("Login()�ɹ���m_pTradeApi(%x)",m_pTradeApi);
    return true;
}
void CPlatformCTPTrade::Logout(void)
{
	m_mutex.lock();
    if(!m_pTradeApi) 
    {
        LOG_INFO("Logout()�ɹ���m_pTradeApiΪNULL");
		m_mutex.unlock();
        return;
    }
	SetConnStatus(CTPCONNSTATUS_Disconnecting,false);
	m_mutex.unlock();

	//ע�⣬Release() �Ὣʣ��δ����������ûص������������ط������Ļ����ص����������������
	m_pTradeApi->RegisterSpi(NULL);
    m_pTradeApi->Release();
	m_pTradeApi=NULL;

	m_mutex.lock();
    m_pTradeApi=NULL;
	SetConnStatus(CTPCONNSTATUS_Disconnected,false);
	m_Ver_String=string("");
	m_bSettlementInfoConfirmed=false;
    LOG_INFO("Logout()�ɹ�");
	m_mutex.unlock();
}



///****************************************************************************
/// ������ctpָ��ִ�к�����
///	һ�������   return  :   0:		�ɹ�
///							-1:     ��������ʧ��
///							-2:     δ�������󳬹������
///							-3:     ÿ�뷢�����������������
///							-999:   ����ԭ��ʧ��
///****************************************************************************

//������CPlatformCTPTrade�ĳ�Ա����ʹ��
#define CHECK_TRADE_STATUS() \
    if(!m_pTradeApi || GetConnStatus()!=CTPCONNSTATUS_Connected)\
    {\
        LOG_INFO("����API��δ׼�����������ܵ���δ�ɹ�����������");\
        return -999;\
    }


///�û������������
int CPlatformCTPTrade::ReqUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();
    strncpy_ctptrade(pUserPasswordUpdate->BrokerID, m_ConnParam.BrokerID.c_str());	///���͹�˾����       
    strncpy_ctptrade(pUserPasswordUpdate->UserID, m_ConnParam.UserID.c_str());		///�û�����,����Ͷ���ߴ���
    int ret = m_pTradeApi->ReqUserPasswordUpdate(pUserPasswordUpdate,++m_RequestID); 
    LOG_INFO("ReqUserPasswordUpdate():ret=[%d],RequestID=[%d]  \t BrokerID=[%s],UserID=[%s]",
		ret, m_RequestID,pUserPasswordUpdate->BrokerID,pUserPasswordUpdate->UserID);
    return ret;
}
///�ʽ��˻������������
int CPlatformCTPTrade::ReqTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();
    strncpy_ctptrade(pTradingAccountPasswordUpdate->BrokerID, m_ConnParam.BrokerID.c_str()); ///���͹�˾����       
    strncpy_ctptrade(pTradingAccountPasswordUpdate->AccountID, m_ConnParam.UserID.c_str());  ///�ʽ��˺�,�����û�����,Ҳ����Ͷ���ߴ���
    int ret = m_pTradeApi->ReqTradingAccountPasswordUpdate(pTradingAccountPasswordUpdate, ++m_RequestID);  
    LOG_INFO("ReqTradingAccountPasswordUpdate():ret=[%d],RequestID=[%d], \t BrokerID=[%s],AccountID=[%s]",
		ret, m_RequestID,pTradingAccountPasswordUpdate->BrokerID,pTradingAccountPasswordUpdate->AccountID);
    return ret;
}

///����¼������. ����ʱpInputOrder�е�RrequestID��BrokerID��InvestorID��UserID��OrderRef��ֵ���޸�Ϊ�ײ�ȷ����ֵ
int CPlatformCTPTrade::ReqOrderInsert(CThostFtdcInputOrderField *pInputOrder)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    pInputOrder->RequestID = ++m_RequestID;
    strncpy_ctptrade(pInputOrder->BrokerID, m_ConnParam.BrokerID.c_str()); ///���͹�˾����       
    strncpy_ctptrade(pInputOrder->InvestorID, m_ConnParam.InvestorID.c_str());  ///Ͷ���ߴ���
    strncpy_ctptrade(pInputOrder->UserID, m_ConnParam.UserID.c_str());  ///�û�����
    pInputOrder->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	sprintf(pInputOrder->OrderRef,"%12d",++m_CurOrderRef);

    int ret = m_pTradeApi->ReqOrderInsert(pInputOrder, m_RequestID);  

    LOG_INFO("ReqOrderInsert(����¼������):ret=[%d],RequestID=[%d]\n"
			"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
             "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
             "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
             "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
             "\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
             ret, m_CurOrderRef,
             pInputOrder->BrokerID, pInputOrder->InvestorID, pInputOrder->InstrumentID, pInputOrder->OrderRef,pInputOrder->UserID,
             pInputOrder->OrderPriceType, pInputOrder->Direction, pInputOrder->CombOffsetFlag, pInputOrder->CombHedgeFlag,pInputOrder->LimitPrice,
             pInputOrder->VolumeTotalOriginal, pInputOrder->TimeCondition, pInputOrder->GTDDate, pInputOrder->VolumeCondition,pInputOrder->MinVolume,
             pInputOrder->ContingentCondition, pInputOrder->StopPrice, pInputOrder->ForceCloseReason, pInputOrder->IsAutoSuspend,pInputOrder->BusinessUnit,
             pInputOrder->RequestID, pInputOrder->UserForceClose);
    return ret;
}
///������������
///
int CPlatformCTPTrade::ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    pInputOrderAction->RequestID = ++m_RequestID;
    strncpy_ctptrade(pInputOrderAction->BrokerID, m_ConnParam.BrokerID.c_str()); ///���͹�˾����       
    strncpy_ctptrade(pInputOrderAction->InvestorID, m_ConnParam.InvestorID.c_str());  ///Ͷ���ߴ���
    pInputOrderAction->ActionFlag = THOST_FTDC_AF_Delete;

    int ret = m_pTradeApi->ReqOrderAction(pInputOrderAction, m_RequestID); 

    LOG_INFO("ReqOrderAction(������������):ret=[%d],RequestID=[%d]\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
                     "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
                     "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
                     ret, m_RequestID,
                     pInputOrderAction->BrokerID, pInputOrderAction->InvestorID, pInputOrderAction->OrderActionRef, pInputOrderAction->OrderRef,pInputOrderAction->RequestID,
                     pInputOrderAction->FrontID, pInputOrderAction->SessionID, pInputOrderAction->ExchangeID, pInputOrderAction->OrderSysID,pInputOrderAction->ActionFlag,
                     pInputOrderAction->LimitPrice, pInputOrderAction->VolumeChange, pInputOrderAction->UserID, pInputOrderAction->InstrumentID);
    return ret;
}




///Ͷ���߽�����ȷ��
int CPlatformCTPTrade::ReqSettlementInfoConfirm()
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strncpy_ctptrade(req.BrokerID, m_ConnParam.BrokerID.c_str());
	strncpy_ctptrade(req.InvestorID, m_ConnParam.InvestorID.c_str());

    int ret = m_pTradeApi->ReqSettlementInfoConfirm(&req, ++m_RequestID);

    LOG_INFO("ReqSettlementInfoConfirm(Ͷ���߽�����ȷ��):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ConfirmDate=[%s],\t ConfirmTime=[%s]",
                     ret, m_RequestID, 
                     req.BrokerID,
                     req.InvestorID,
                     req.ConfirmDate,
                     req.ConfirmTime);
    return ret;
}

///�����ѯͶ���߽�����
int CPlatformCTPTrade::ReqQrySettlementInfo(CThostFtdcQrySettlementInfoField *pQrySettlementInfo)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    strncpy_ctptrade(pQrySettlementInfo->BrokerID, m_ConnParam.BrokerID.c_str());			///���͹�˾����       
    strncpy_ctptrade(pQrySettlementInfo->InvestorID, m_ConnParam.InvestorID.c_str());		///Ͷ���ߴ���

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQrySettlementInfo,pQrySettlementInfo,sizeof(*pQrySettlementInfo),NULL,0,++m_RequestID);

	LOG_INFO("Push into QryQueue: ReqQrySettlementInfo(�����ѯͶ���߽�����):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t TradingDay=[%s]",
                     brlt, m_RequestID, 
                     pQrySettlementInfo->BrokerID,
                     pQrySettlementInfo->InvestorID,
                     pQrySettlementInfo->TradingDay);
    return 0;
}
///�����ѯ������Ϣȷ��
int CPlatformCTPTrade::ReqQrySettlementInfoConfirm(CThostFtdcQrySettlementInfoConfirmField *pQrySettlementInfoConfirm)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

	strncpy_ctptrade(pQrySettlementInfoConfirm->BrokerID, m_ConnParam.BrokerID.c_str());		///���͹�˾����    
	strncpy_ctptrade(pQrySettlementInfoConfirm->InvestorID, m_ConnParam.InvestorID.c_str());    ///Ͷ���ߴ���

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQrySettlementInfoConfirm,pQrySettlementInfoConfirm,sizeof(*pQrySettlementInfoConfirm),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQrySettlementInfoConfirm(�����ѯ������Ϣȷ��):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
                     brlt, m_RequestID,
                     pQrySettlementInfoConfirm->BrokerID,
                     pQrySettlementInfoConfirm->InvestorID);
    return 0;
}
///�����ѯ�ͻ�֪ͨ
int CPlatformCTPTrade::ReqQryNotice(CThostFtdcQryNoticeField *pQryNotice)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

	strncpy_ctptrade(pQryNotice->BrokerID, m_ConnParam.BrokerID.c_str()); ///���͹�˾����    

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryNotice,pQryNotice,sizeof(*pQryNotice),NULL,0,++m_RequestID);
    return 0;
}

///�����ѯ��Լ
int CPlatformCTPTrade::ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryInstrument,pQryInstrument,sizeof(*pQryInstrument),NULL,0,++m_RequestID);

	LOG_INFO("Push into QryQueue: ReqQryInstrument(�����ѯ��Լ):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t InstrumentID=[%s],\t ExchangeID=[%s],\t ExchangeInstID=[%s],\tProductID=[%s]",
                     brlt, m_RequestID, 
                     pQryInstrument->InstrumentID,
                     pQryInstrument->ExchangeID,
                     pQryInstrument->ExchangeInstID,
                     pQryInstrument->ProductID);

    return 0;
}

///�����ѯ����
int CPlatformCTPTrade::ReqQryOrder(CThostFtdcQryOrderField *pQryOrder)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    strncpy_ctptrade(pQryOrder->BrokerID, m_ConnParam.BrokerID.c_str());			///���͹�˾����       
    strncpy_ctptrade(pQryOrder->InvestorID, m_ConnParam.InvestorID.c_str());		///Ͷ���ߴ���

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryOrder,pQryOrder,sizeof(*pQryOrder),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQryOrder(�����ѯ����):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s],\t OrderSysID=[%s],\n"
                     "\t\t\t InsertTimeStart=[%s],\t InsertTimeEnd=[%s]",
                     brlt, m_RequestID,
                     pQryOrder->BrokerID,
                     pQryOrder->InvestorID,
                     pQryOrder->InstrumentID,
                     pQryOrder->ExchangeID,
                     pQryOrder->OrderSysID,
                     pQryOrder->InsertTimeStart,
                     pQryOrder->InsertTimeEnd);

    return 0;
}

///�����ѯ�ɽ�
int CPlatformCTPTrade::ReqQryTrade(CThostFtdcQryTradeField *pQryTrade)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();
	
    strncpy_ctptrade(pQryTrade->BrokerID, m_ConnParam.BrokerID.c_str());		///���͹�˾����       
    strncpy_ctptrade(pQryTrade->InvestorID, m_ConnParam.InvestorID.c_str());  ///Ͷ���ߴ���

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryTrade,pQryTrade,sizeof(*pQryTrade),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQryTrade(�����ѯ�ɽ�):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s],\t TradeID=[%s],\n"
                     "\t\t\t TradeTimeStart=[%s],\t TradeTimeEnd=[%s]",
                     brlt, m_RequestID,
                     pQryTrade->BrokerID,
                     pQryTrade->InvestorID,
                     pQryTrade->InstrumentID,
                     pQryTrade->ExchangeID,
                     pQryTrade->TradeID,
                     pQryTrade->TradeTimeStart,
                     pQryTrade->TradeTimeEnd);
    return 0;
}

///�����ѯͶ���ֲ߳�
int CPlatformCTPTrade::ReqQryInvestorPosition(CThostFtdcQryInvestorPositionField *pQryInvestorPosition)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

	strncpy_ctptrade(pQryInvestorPosition->BrokerID, m_ConnParam.BrokerID.c_str());
	strncpy_ctptrade(pQryInvestorPosition->InvestorID, m_ConnParam.InvestorID.c_str());

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryInvestorPosition,pQryInvestorPosition,sizeof(*pQryInvestorPosition),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQryInvestorPosition(�����ѯͶ���ֲ߳�):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
                     brlt, m_RequestID,
                     pQryInvestorPosition->BrokerID,
                     pQryInvestorPosition->InvestorID,
                     pQryInvestorPosition->InstrumentID);
    return 0;
}
///�����ѯͶ���ֲ߳���ϸ
int CPlatformCTPTrade::ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    strncpy_ctptrade(pQryInvestorPositionDetail->BrokerID,m_ConnParam.BrokerID.c_str());
    strncpy_ctptrade(pQryInvestorPositionDetail->InvestorID,m_ConnParam.InvestorID.c_str());

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryInvestorPositionDetail,pQryInvestorPositionDetail,sizeof(*pQryInvestorPositionDetail),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQryInvestorPositionDetail(�����ѯͶ���ֲ߳���ϸ):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
                     brlt, m_RequestID,
                     pQryInvestorPositionDetail->BrokerID,
                     pQryInvestorPositionDetail->InvestorID,
                     pQryInvestorPositionDetail->InstrumentID);
    return 0;
}
///�����ѯͶ���ֲ߳���ϸ
int CPlatformCTPTrade::ReqQryInvestorPositionCombineDetail(CThostFtdcQryInvestorPositionCombineDetailField *pQryInvestorPositionCombineDetail)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    strncpy_ctptrade(pQryInvestorPositionCombineDetail->BrokerID,m_ConnParam.BrokerID.c_str());
    strncpy_ctptrade(pQryInvestorPositionCombineDetail->InvestorID,m_ConnParam.InvestorID.c_str());

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryInvestorPositionCombineDetail,pQryInvestorPositionCombineDetail,sizeof(*pQryInvestorPositionCombineDetail),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQryInvestorPositionCombineDetail(�����ѯͶ������ϳֲ���ϸ):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t CombInstrumentID=[%s]",
                     brlt, m_RequestID,
                     pQryInvestorPositionCombineDetail->BrokerID,
                     pQryInvestorPositionCombineDetail->InvestorID,
                     pQryInvestorPositionCombineDetail->CombInstrumentID);
    return 0;
}



///�����ѯ�ʽ��˻�
int CPlatformCTPTrade::ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    strncpy_ctptrade(pQryTradingAccount->BrokerID,m_ConnParam.BrokerID.c_str());
    strncpy_ctptrade(pQryTradingAccount->InvestorID,m_ConnParam.InvestorID.c_str());

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryTradingAccount,pQryTradingAccount,sizeof(*pQryTradingAccount),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQryTradingAccount(�����ѯ�ʽ��˻�):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
                     brlt, m_RequestID,
                     pQryTradingAccount->BrokerID,
                     pQryTradingAccount->InvestorID);
    return 0;
}


///�����ѯ��Լ��֤����
int CPlatformCTPTrade::ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    strncpy_ctptrade(pQryInstrumentMarginRate->BrokerID, m_ConnParam.BrokerID.c_str());			///���͹�˾����    
	strncpy_ctptrade(pQryInstrumentMarginRate->InvestorID, m_ConnParam.InvestorID.c_str());		///Ͷ���ߴ���
    if(pQryInstrumentMarginRate->HedgeFlag==0) pQryInstrumentMarginRate->HedgeFlag=THOST_FTDC_HF_Speculation;

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryInstrumentMarginRate,pQryInstrumentMarginRate,sizeof(*pQryInstrumentMarginRate),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQryInstrumentMarginRate(�����ѯ��Լ��֤����):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t HedgeFlag=[%d]",
                     brlt, m_RequestID,
                     pQryInstrumentMarginRate->BrokerID,
                     pQryInstrumentMarginRate->InvestorID,
                     pQryInstrumentMarginRate->InstrumentID,
                     pQryInstrumentMarginRate->HedgeFlag);
    return 0;
}

///�����ѯ��Լ��������
int CPlatformCTPTrade::ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    strncpy_ctptrade(pQryInstrumentCommissionRate->BrokerID, m_ConnParam.BrokerID.c_str());			///���͹�˾����    
	strncpy_ctptrade(pQryInstrumentCommissionRate->InvestorID, m_ConnParam.InvestorID.c_str());		///Ͷ���ߴ���

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryInstrumentCommissionRate,pQryInstrumentCommissionRate,sizeof(*pQryInstrumentCommissionRate),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQryInstrumentCommissionRate(�����ѯ��Լ��������):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
                     brlt, m_RequestID,
                     pQryInstrumentCommissionRate->BrokerID,
                     pQryInstrumentCommissionRate->InvestorID,
                     pQryInstrumentCommissionRate->InstrumentID);
    return 0;
}












///****************************************************************************
/// ������ctp�ص�����
///****************************************************************************

#ifndef _NEED_AUTHENTICATE
///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CPlatformCTPTrade::OnFrontConnected()
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2((unsigned int)CmdID_Trade_FrontConnected, NULL,0);

    //�����¼����
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strncpy(req.BrokerID, m_ConnParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
    strncpy(req.UserID, m_ConnParam.UserID.c_str(),sizeof(req.UserID)-1);
    strncpy(req.Password, m_ConnParam.UserPW.c_str(),sizeof(req.Password)-1);
    vector<string> vervec;
	vector<int> vecpos=CTools_Ansi::GetCharPos(m_Ver_String.c_str(),',');
    if((int)vecpos.size()==3)
    {
        char tmp[256]={0};
        sprintf(tmp,"ft%02d%02d%02d%02d",
            atoi(m_Ver_String.c_str()),
            atoi(m_Ver_String.c_str()+vecpos[0]),
            atoi(m_Ver_String.c_str()+vecpos[1]),
            atoi(m_Ver_String.c_str()+vecpos[2]));
	    strncpy(req.UserProductInfo,tmp,sizeof(req.UserProductInfo)-1);
    }
    else
    {
	    strncpy(req.UserProductInfo, "ft00000000",sizeof(req.UserProductInfo)-1);
    }
	strncpy(req.OneTimePassword,m_ConnParam.OneTimePassword.c_str(),sizeof(req.OneTimePassword)-1);

	SetConnStatus(CTPCONNSTATUS_Logining,false);

    int RequestID=++m_RequestID;
    int iRlt = m_pTradeApi->ReqUserLogin(&req, RequestID);

    LOG_INFO("OnFrontConnected(�뽻�����ɹ�����TCP����)\n"
                     "\t\t\t --->ReqUserLogin(�û���¼����):ret=[%d],iRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t UserID=[%s],\t UserProductInfo=[%s]",
                     iRlt, RequestID,
                     req.BrokerID, req.UserID,req.UserProductInfo);
}
#else
///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
void CPlatformCTPTrade::OnFrontConnected()
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Trade_FrontConnected, NULL,0);

    //������֤����
    GetLocalTime(&m_ConnectTime);//�������ʱ��

    CThostFtdcReqAuthenticateField req;
    memset(&req, 0, sizeof(req));
    strncpy(req.BrokerID, m_ConnParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
    strncpy(req.UserID, m_ConnParam.UserID.c_str(),sizeof(req.UserID)-1);
    vector<string> vervec;
    if(SplitString(m_Ver_String,vervec)==4)
    {
        char tmp[256]={0};
        sprintf(tmp,"ft%02d%02d%02d%02d",
            atoi(vervec[0].c_str()),
            atoi(vervec[1].c_str()),
            atoi(vervec[2].c_str()),
            atoi(vervec[3].c_str()));
	    strncpy(req.UserProductInfo,tmp,sizeof(req.UserProductInfo)-1);
    }
    else
    {
	    strncpy(req.UserProductInfo, "ft00000000",sizeof(req.UserProductInfo)-1);
    }

	m_ConnStatus=CTPCONNSTATUS_Logining;

    int RequestID=++m_RequestID;
    int iRlt = m_pTradeApi->ReqAuthenticate(&req, RequestID);

    LOG_INFO("OnFrontConnected(�뽻�����ɹ�����TCP����)\n"
                     "\t\t\t --->ReqAuthenticate(�ͻ�����֤����):ret=[%d],iRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t UserID=[%s],\t UserProductInfo=[%s]",
                     iRlt, RequestID,
                     req.BrokerID, req.UserID,req.UserProductInfo);
}
//�ͻ�����֤��Ӧ
void CPlatformCTPTrade::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(pRspInfo&&pRspInfo->ErrorID==0)//����ɹ�
	{
		CThostFtdcReqUserLoginField req;
		memset(&req, 0, sizeof(req));
		strncpy(req.BrokerID, m_ConnParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
		strncpy(req.UserID, m_ConnParam.UserID.c_str(),sizeof(req.UserID)-1);
		strncpy(req.Password, m_ConnParam.UserPW.c_str(),sizeof(req.Password)-1);
        vector<string> vervec;
        if(SplitString(m_Ver_String,vervec)==4)
        {
            char tmp[256]={0};
            sprintf(tmp,"ft%02d%02d%02d%02d",
                atoi(vervec[0].c_str()),
                atoi(vervec[1].c_str()),
                atoi(vervec[2].c_str()),
                atoi(vervec[3].c_str()));
	        strncpy(req.UserProductInfo,tmp,sizeof(req.UserProductInfo)-1);
        }
        else
        {
	        strncpy(req.UserProductInfo, "ft00000000",sizeof(req.UserProductInfo)-1);
        }
		strncpy(req.OneTimePassword,m_ConnParam.OneTimePassword.c_str(),sizeof(req.OneTimePassword)-1);

        int RequestID=++m_RequestID;
		int iRlt = m_pTradeApi->ReqUserLogin(&req, RequestID);

		LOG_INFO("OnRspAuthenticate(�ͻ�����֤��Ӧ)\n"
			"\t\t\t --->ReqUserLogin(�û���¼����):ret=[%d],RequestID=[%d],\n"
			"\t\t\t BrokerID=[%s],\t UserID=[%s],\t UserProductInfo=[%s]",
			iRlt, RequestID,
			req.BrokerID, req.UserID,req.UserProductInfo);
	}
	else
	{
        m_ConnStatus=CTPCONNSTATUS_LoginFailure;
	}
}
#endif
///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á�
///��������������API���Զ��������ӣ��ͻ��˿ɲ�������
///@param nReason ����ԭ��
///        0x1001 �����ʧ��
///        0x1002 ����дʧ��
///        0x2001 ����������ʱ
///        0x2002 ��������ʧ��
///        0x2003 �յ�������
void CPlatformCTPTrade::OnFrontDisconnected(int nReason)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Trade_FrontDisconnected, NULL,0,NULL,0,(DWORD)nReason);
	SetConnStatus(CTPCONNSTATUS_Disconnected,false);

    LOG_INFO("OnFrontDisconnected(�뽻����ʧȥTCP����), Reason=[%#x]", nReason);
}
    
///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
///�ۺϽ���ƽ̨��API�Ѿ����δ˽ӿ�
void CPlatformCTPTrade::OnHeartBeatWarning(int nTimeLapse)
{
    LOG_INFO("OnHeartBeatWarning:nTimeLapse=[%d]", nTimeLapse);
}


///��¼������Ӧ
void CPlatformCTPTrade::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

    if(pRspUserLogin&&(!pRspInfo||pRspInfo->ErrorID==0))
    {
        memcpy(&m_LoginInfo,pRspUserLogin,sizeof(m_LoginInfo));
        m_CurOrderRef=atoi(pRspUserLogin->MaxOrderRef);
    }
    else
    {
		SetConnStatus(CTPCONNSTATUS_LoginFailure,false);
    }
    
    if(m_bPushCache) 
    {
        m_Cache.SaveDataTo2(CmdID_Trade_RspUserLogin,
            pRspUserLogin,pRspUserLogin?sizeof(*pRspUserLogin):0,
            pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
            (unsigned int)nRequestID, (unsigned int)bIsLast,
			(unsigned int)GetConnStatus());
    }

        
    if(pRspUserLogin)
    {
        LOG_INFO("OnRspUserLogin(��¼������Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]\n"
                        "\t\t\t TradingDay=[%s],\t LoginTime=[%s],\t BrokerID=[%s],\t UserID=[%s],\t SystemName=[%s]\n"
                        "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t MaxOrderRef[%s],\t SHFETime=[%s],\t DCETime=[%s],\n"
                        "\t\t\t CZCETime[%s],\t FFEXTime=[%s]",
						pRspInfo,pRspInfo?pRspInfo->ErrorID:0,pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast,
                        pRspUserLogin->TradingDay,pRspUserLogin->LoginTime,pRspUserLogin->BrokerID   , pRspUserLogin->UserID,pRspUserLogin->SystemName, 
                        pRspUserLogin->FrontID  , pRspUserLogin->SessionID ,pRspUserLogin->MaxOrderRef, pRspUserLogin->SHFETime , pRspUserLogin->DCETime, 
                        pRspUserLogin->CZCETime   , pRspUserLogin->FFEXTime);
    }
    else 
    {
        LOG_INFO("OnRspUserLogin(��¼������Ӧ) : pRspUserLogin=[%x] pRspInfo=[%x] ErrorID=%d",
            pRspUserLogin,pRspInfo,pRspInfo?pRspInfo->ErrorID:0);
    }

    if(pRspUserLogin&&(!pRspInfo||pRspInfo->ErrorID==0))
    {
		SetConnStatus(CTPCONNSTATUS_Connected,false);

        //��ѯ������ȷ����Ϣ
		if(m_bAutoReqSettlementInfoConfirmed)
		{
			CThostFtdcQrySettlementInfoConfirmField req;
			memset(&req, 0, sizeof(req));
			strncpy_ctptrade(req.BrokerID, m_ConnParam.BrokerID.c_str());
			strncpy_ctptrade(req.InvestorID, m_ConnParam.InvestorID.c_str());

			int iRlt = ReqQrySettlementInfoConfirm(&req);
			LOG_INFO("OnRspUserLogin--->ReqQrySettlementInfoConfirm(�����ѯͶ���߽�����ȷ��):ret=[%d]",iRlt); 
		}

		//��ѯ��Լ��Ϣ
		if(m_bAutoQryInstrumentInfo)
		{
			CThostFtdcQryInstrumentField req;
			memset(&req,0,sizeof(req));
			int iRlt=ReqQryInstrument(&req) ;
			LOG_INFO("OnRspUserLogin--->ReqQryInstrument(�����ѯ��Լ��Ϣ):ret=[%d]",iRlt); 
		}
    }
}

///�����ѯ������Ϣȷ����Ӧ������ָ��ѯ���Ƿ��Ѿ����н���ȷ�ϡ�
void CPlatformCTPTrade::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

	if(pSettlementInfoConfirm&&(!pRspInfo||pRspInfo->ErrorID==0))
		m_bSettlementInfoConfirmed=true;

    if(m_bPushCache) 
    {
        m_Cache.SaveDataTo2(CmdID_Trade_RspQrySettlementInfoConfirm,
            pSettlementInfoConfirm,pSettlementInfoConfirm?sizeof(*pSettlementInfoConfirm):0,
            pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
            (unsigned int)nRequestID,(unsigned int)bIsLast,
			m_bSettlementInfoConfirmed);
    }

    LOG_INFO("OnRspQrySettlementInfoConfirm(�����ѯͶ���߽�����ȷ����Ӧ)��pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]\n"
		"\t\t\t pSettlementInfoConfirm=[%x],m_bSettlementInfoConfirmed=%d\n"
        "\t\t\t BrokerID=%s InvestorID=%s ConfirmDate=%s ConfirmTime=%s",
        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast,
        pSettlementInfoConfirm,m_bSettlementInfoConfirmed,
        pSettlementInfoConfirm?pSettlementInfoConfirm->BrokerID:"",
        pSettlementInfoConfirm?pSettlementInfoConfirm->InvestorID:"",
        pSettlementInfoConfirm?pSettlementInfoConfirm->ConfirmDate:"",
        pSettlementInfoConfirm?pSettlementInfoConfirm->ConfirmTime:"");

    //�Զ����н���ȷ��
    if(!m_bSettlementInfoConfirmed&&m_bAutoConfirmSettlement)
    {
	    CThostFtdcSettlementInfoConfirmField req;
	    memset(&req, 0, sizeof(req));
	    strncpy_ctptrade(req.BrokerID, m_ConnParam.BrokerID.c_str());
	    strncpy_ctptrade(req.InvestorID, m_ConnParam.InvestorID.c_str());

        int ret = m_pTradeApi->ReqSettlementInfoConfirm(&req, ++m_RequestID);

        LOG_INFO("ReqSettlementInfoConfirm(Ͷ���߽�����ȷ��):ret=[%d],RequestID=[%d],\n"
                         "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ConfirmDate=[%s],\t ConfirmTime=[%s]",
                         ret, m_RequestID, 
                         req.BrokerID,
                         req.InvestorID,
                         req.ConfirmDate,
                         req.ConfirmTime);
    }
}

///Ͷ���߽�����ȷ����Ӧ������ָ����ȷ�ϵ���Ӧ
void CPlatformCTPTrade::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

    if(pSettlementInfoConfirm&&(!pRspInfo||pRspInfo->ErrorID==0))
	    m_bSettlementInfoConfirmed=true;

    if(m_bPushCache) 
    {
        m_Cache.SaveDataTo2(CmdID_Trade_RspSettlementInfoConfirm,
            pSettlementInfoConfirm,pSettlementInfoConfirm?sizeof(*pSettlementInfoConfirm):0,
            pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast,
            m_bSettlementInfoConfirmed);
    }

    LOG_INFO("OnRspSettlementInfoConfirm(Ͷ���߽�����ȷ����Ӧ)��pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]\n"
		"\t\t\t pSettlementInfoConfirm=[%x] m_bSettlementInfoConfirmed=%d\n"
        "\t\t\t BrokerID=%s InvestorID=%s ConfirmDate=%s ConfirmTime=%s",
        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast,
        pSettlementInfoConfirm,m_bSettlementInfoConfirmed,
        pSettlementInfoConfirm?pSettlementInfoConfirm->BrokerID:"",
        pSettlementInfoConfirm?pSettlementInfoConfirm->InvestorID:"",
        pSettlementInfoConfirm?pSettlementInfoConfirm->ConfirmDate:"",
        pSettlementInfoConfirm?pSettlementInfoConfirm->ConfirmTime:"");

}







///�û��������������Ӧ
void CPlatformCTPTrade::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(pUserPasswordUpdate&&(!pRspInfo||pRspInfo->ErrorID==0))
		m_ConnParam.UserPW=pUserPasswordUpdate->NewPassword;

    if(m_bPushCache) 
    {
        m_Cache.SaveDataTo2(CmdID_Trade_RspUserPasswordUpdate, 
            pUserPasswordUpdate,pUserPasswordUpdate?sizeof(*pUserPasswordUpdate):0,
            pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
    }

    if(pUserPasswordUpdate)
    {
        LOG_INFO("OnRspUserPasswordUpdate(�û��������������Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t UserID=[%s],\t OldPassword=[******],\t NewPassword=[******]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                        pUserPasswordUpdate->BrokerID, pUserPasswordUpdate->UserID);
    }
    else
    {
        LOG_INFO("OnRspUserPasswordUpdate(�û��������������Ӧ) : pUserPasswordUpdate=[%x] pRspInfo=[%x] ErrorID=%d",
            pUserPasswordUpdate,pRspInfo,pRspInfo?pRspInfo->ErrorID:0);
    }
}

///�ʽ��˻��������������Ӧ
void CPlatformCTPTrade::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache)
    {
        m_Cache.SaveDataTo2(CmdID_Trade_RspTradingAccountPasswordUpdate, 
            pTradingAccountPasswordUpdate,pTradingAccountPasswordUpdate?sizeof(*pTradingAccountPasswordUpdate):0,
            pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
    }

    if(pTradingAccountPasswordUpdate)
    {
        LOG_INFO("OnRspTradingAccountPasswordUpdate(�ʽ��˻��������������Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],BrokerID=[%s] AccountID=[%s],RequestID=[%d],bIsLast=[%d]",
			    pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", 
			    pTradingAccountPasswordUpdate->BrokerID, pTradingAccountPasswordUpdate->AccountID, nRequestID,bIsLast);
    }
    else
    {
        LOG_INFO("OnRspTradingAccountPasswordUpdate(�ʽ��˻��������������Ӧ) : pTradingAccountPasswordUpdate=[%x] pRspInfo=[%x] ErrorID=%d",
            pTradingAccountPasswordUpdate,pRspInfo,pRspInfo?pRspInfo->ErrorID:0);
    }
}

///����¼��������Ӧ
void CPlatformCTPTrade::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspOrderInsert,
			pInputOrder,pInputOrder?sizeof(*pInputOrder):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast); 
	}

    if(pInputOrder)
    {
        LOG_INFO("OnRspOrderInsert(����¼��������Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                 "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                 "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
                 "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
                 "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
                 "\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
                 pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                 pInputOrder->BrokerID, pInputOrder->InvestorID, pInputOrder->InstrumentID, pInputOrder->OrderRef,pInputOrder->UserID,
                 pInputOrder->OrderPriceType, pInputOrder->Direction, pInputOrder->CombOffsetFlag, pInputOrder->CombHedgeFlag,pInputOrder->LimitPrice,
                 pInputOrder->VolumeTotalOriginal, pInputOrder->TimeCondition, pInputOrder->GTDDate, pInputOrder->VolumeCondition,pInputOrder->MinVolume,
                 pInputOrder->ContingentCondition, pInputOrder->StopPrice, pInputOrder->ForceCloseReason, pInputOrder->IsAutoSuspend,pInputOrder->BusinessUnit,
                 pInputOrder->RequestID, pInputOrder->UserForceClose);
    }
    else
    {
        LOG_INFO("OnRspOrderInsert(����¼��������Ӧ) : pInputOrder=[%x] pRspInfo=[%x] ErrorID=%d",
            pInputOrder,pRspInfo,pRspInfo?pRspInfo->ErrorID:0);
    }
}

///��������������Ӧ
void CPlatformCTPTrade::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspOrderAction,
			pInputOrderAction,pInputOrderAction?sizeof(*pInputOrderAction):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);  
	}

    if(pInputOrderAction)
    {
        LOG_INFO("OnRspOrderAction(��������������Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                         "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
                         "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
                         "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
                         pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                         pInputOrderAction->BrokerID, pInputOrderAction->InvestorID, pInputOrderAction->OrderActionRef, pInputOrderAction->OrderRef,pInputOrderAction->RequestID,
                         pInputOrderAction->FrontID, pInputOrderAction->SessionID, pInputOrderAction->ExchangeID, pInputOrderAction->OrderSysID,pInputOrderAction->ActionFlag,
                         pInputOrderAction->LimitPrice, pInputOrderAction->VolumeChange, pInputOrderAction->UserID, pInputOrderAction->InstrumentID);
    }
    else
    {
        LOG_INFO("OnRspOrderAction(��������������Ӧ) : pInputOrderAction=[%x] pRspInfo=[%x] ErrorID=%d",
            pInputOrderAction,pRspInfo,pRspInfo?pRspInfo->ErrorID:0);
    }
}





///�����ѯ������Ӧ
void CPlatformCTPTrade::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryOrder,
			pOrder,pOrder?sizeof(*pOrder):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);  
	}

    if(pOrder&&pRspInfo)
    {
        LOG_INFO("OnRspQryOrder(�����ѯ������Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                        "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
                        "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
                        "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
                        "\t\t\t RequestID=[%d],\t OrderLocalID=[%s],\t ExchangeID=[%s],\t ParticipantID=[%s],\t ClientID=[%s],\n"
                        "\t\t\t ExchangeInstID=[%s],\t TraderID=[%s],\t InstallID=[%d],\t OrderSubmitStatus=[%d],\t NotifySequence=[%d],\n"
                        "\t\t\t TradingDay=[%s],\t SettlementID=[%d],\t OrderSysID=[%s],\t OrderSource=[%d],\t OrderStatus=[%d],\n"
                        "\t\t\t OrderType=[%d],\t VolumeTraded=[%d],\t VolumeTotal=[%d],\t InsertDate=[%s],\t InsertTime=[%s],\n"
                        "\t\t\t ActiveTime=[%s],\t SuspendTime=[%s],\t UpdateTime=[%s],\t CancelTime=[%s],\t ActiveTraderID=[%s]\n"
                        "\t\t\t ClearingPartID=[%s],\t SequenceNo=[%d],\t FrontID=[%d],\t SessionID=[%#x],\t UserProductInfo=[%s],\n"
                        "\t\t\t StatusMsg=[%s],\t UserForceClose=[%d],\t ActiveUserID=[%s],\t BrokerOrderSeq=[%d],RelativeOrderSysID=[%s]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast,
                        pOrder->BrokerID,pOrder->InvestorID,pOrder->InstrumentID,pOrder->OrderRef,pOrder->UserID,
                        pOrder->OrderPriceType,pOrder->Direction,pOrder->CombOffsetFlag,pOrder->CombHedgeFlag,pOrder->LimitPrice,
                        pOrder->VolumeTotalOriginal,pOrder->TimeCondition,pOrder->GTDDate,pOrder->VolumeCondition,pOrder->MinVolume,
                        pOrder->ContingentCondition,pOrder->StopPrice,pOrder->ForceCloseReason,pOrder->IsAutoSuspend,pOrder->BusinessUnit,
						pOrder->RequestID,pOrder->OrderLocalID,pOrder->ExchangeID,pOrder->ParticipantID,pOrder->ClientID,
                        pOrder->ExchangeInstID,pOrder->TraderID,pOrder->InstallID,pOrder->OrderSubmitStatus,pOrder->NotifySequence,
                        pOrder->TradingDay,pOrder->SettlementID,pOrder->OrderSysID,pOrder->OrderSource,pOrder->OrderStatus,
                        pOrder->OrderType,pOrder->VolumeTraded,pOrder->VolumeTotal,pOrder->InsertDate,pOrder->InsertTime,
                        pOrder->ActiveTime,pOrder->SuspendTime,pOrder->UpdateTime,pOrder->CancelTime,pOrder->ActiveTraderID,
                        pOrder->ClearingPartID,pOrder->SequenceNo,pOrder->FrontID,pOrder->SessionID,pOrder->UserProductInfo,
                        pOrder->StatusMsg,pOrder->UserForceClose,pOrder->ActiveUserID,pOrder->BrokerOrderSeq,pOrder->RelativeOrderSysID
                    );
    }
    else
    {
        LOG_INFO("OnRspQryOrder(�����ѯ������Ӧ) : pOrder=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
                         pOrder,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}

///�����ѯ�ɽ���Ӧ
void CPlatformCTPTrade::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryTrade,
			pTrade,pTrade?sizeof(*pTrade):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pTrade)
    {
        LOG_INFO("OnRspQryTrade(�����ѯ�ɽ���Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                        "\t\t\t ExchangeID=[%s],\t TradeID=[%s],\t Direction=[%d],\t OrderSysID=[%s],\t ParticipantID=[%s],\n"
                        "\t\t\t ClientID=[%s],\t TradingRole=[%d],\t ExchangeInstID=[%s],\t OffsetFlag=[%d],\t HedgeFlag=[%d],\n"
                        "\t\t\t Price=[%g],\t Volume=[%d],\t TradeDate=[%s],\t TradeTime=[%s],\t TradeType=[%d],\n"
                        "\t\t\t PriceSource=[%d],\t TraderID=[%s],\t OrderLocalID=[%s],\t ClearingPartID=[%s],\t BusinessUnit=[%s],\n"
                        "\t\t\t SequenceNo=[%d],\t TradingDay=[%s],\t SettlementID=[%d],\t BrokerOrderSeq=[%d],\t TradeSource=[%c]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast,
                        pTrade->BrokerID,pTrade->InvestorID,pTrade->InstrumentID,pTrade->OrderRef,pTrade->UserID,
                        pTrade->ExchangeID,pTrade->TradeID,pTrade->Direction,pTrade->OrderSysID,pTrade->ParticipantID,
                        pTrade->ClientID,pTrade->TradingRole,pTrade->ExchangeInstID,pTrade->OffsetFlag,pTrade->HedgeFlag,
                        pTrade->Price,pTrade->Volume,pTrade->TradeDate,pTrade->TradeTime,pTrade->TradeType,
                        pTrade->PriceSource,pTrade->TraderID,pTrade->OrderLocalID,pTrade->ClearingPartID,pTrade->BusinessUnit,
                        pTrade->SequenceNo,pTrade->TradingDay,pTrade->SettlementID,pTrade->BrokerOrderSeq, pTrade->TradeSource);
    }
    else
    {
        LOG_INFO("OnRspQryTrade(�����ѯ�ɽ���Ӧ) : pTrade=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
                         pTrade,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}

///�����ѯ�ʽ��˻���Ӧ
void CPlatformCTPTrade::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryTradingAccount,
			pTradingAccount,pTradingAccount?sizeof(*pTradingAccount):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pTradingAccount)
    {
        LOG_INFO("OnRspQryTradingAccount(�����ѯ�ʽ��˻���Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t AccountID=[%s],\t PreMortgage=[%g],\t PreCredit=[%g],\t PreDeposit=[%g],\n"
                        "\t\t\t PreBalance=[%g],\t PreMargin=[%g],\t InterestBase=[%g],\t Interest=[%g],\t Deposit=[%g],\n "
                        "\t\t\t Withdraw=[%g],\t FrozenMargin=[%g],\t FrozenCash=[%g],\t FrozenCommission=[%g],\t CurrMargin=[%g],\n "
                        "\t\t\t CashIn=[%g],\t Commission=[%g],\t CloseProfit=[%g],\t PositionProfit=[%g],\t Balance=[%g],\n "
                        "\t\t\t Available=[%g],\t WithdrawQuota=[%g],\t Reserve=[%g],\t TradingDay=[%s],\t SettlementID=[%d],\n "
                        "\t\t\t Credit=[%g],\t Mortgage=[%g],\t ExchangeMargin=[%g],\t DeliveryMargin=[%g],\t ExchangeDeliveryMargin=[%g]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                        pTradingAccount->BrokerID,pTradingAccount->AccountID,pTradingAccount->PreMortgage,pTradingAccount->PreCredit,pTradingAccount->PreDeposit, 
                        pTradingAccount->PreBalance,pTradingAccount->PreMargin,pTradingAccount->InterestBase,pTradingAccount->Interest,pTradingAccount->Deposit, 
                        pTradingAccount->Withdraw,pTradingAccount->FrozenMargin,pTradingAccount->FrozenCash,pTradingAccount->FrozenCommission,pTradingAccount->CurrMargin, 
                        pTradingAccount->CashIn,pTradingAccount->Commission,pTradingAccount->CloseProfit,pTradingAccount->PositionProfit,pTradingAccount->Balance, 
                        pTradingAccount->Available,pTradingAccount->WithdrawQuota,pTradingAccount->Reserve,pTradingAccount->TradingDay,pTradingAccount->SettlementID, 
                        pTradingAccount->Credit,pTradingAccount->Mortgage,pTradingAccount->ExchangeMargin,pTradingAccount->DeliveryMargin,pTradingAccount->ExchangeDeliveryMargin
                        );
    }
    else
    {
        LOG_INFO("OnRspQryTrade(�����ѯ�ʽ��˻���Ӧ) : pTradingAccount=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
                         pTradingAccount,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}

///�����ѯͶ������Ӧ
void CPlatformCTPTrade::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryInvestor,
			pInvestor,pInvestor?sizeof(*pInvestor):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pInvestor)
    {
        LOG_INFO("OnRspQryInvestor(�����ѯͶ������Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InvestorID=[%s],\t BrokerID=[%s],\t InvestorGroupID=[%s],\t InvestorName=[%s],\t IdentifiedCardType=[%d],\n"
                        "\t\t\t IdentifiedCardNo=[%s],\t IsActive=[%d],\t Telephone=[%s],\t Address=[%s]",
                        pRspInfo,pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID,bIsLast,
                        pInvestor->InvestorID, pInvestor->BrokerID,pInvestor->InvestorGroupID,pInvestor->InvestorName,pInvestor->IdentifiedCardType,
                        pInvestor->IdentifiedCardNo, pInvestor->IsActive,pInvestor->Telephone,pInvestor->Address);
    }
    else
    {
        LOG_INFO("OnRspQryInvestor(�����ѯͶ������Ӧ) : pInvestor=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
            pInvestor,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}


///�����ѯ��Լ��֤������Ӧ
void CPlatformCTPTrade::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryInstrumentMarginRate,
			pInstrumentMarginRate,pInstrumentMarginRate?sizeof(*pInstrumentMarginRate):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pInstrumentMarginRate)
    {
        LOG_INFO("OnRspQryInstrumentMarginRate(�����ѯ��Լ��֤������Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InstrumentID=[%s],\t InvestorRange=[%d],\t BrokerID=[%s],\t InvestorID=[%s],\t HedgeFlag=[%d],\n"
                        "\t\t\t LongMarginRatioByMoney=[%g],\t LongMarginRatioByVolume=[%g],\t ShortMarginRatioByMoney=[%g],\t ShortMarginRatioByVolume=[%g],\t IsRelative=[%d]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                        pInstrumentMarginRate->InstrumentID,
                        pInstrumentMarginRate->InvestorRange,
                        pInstrumentMarginRate->BrokerID,
                        pInstrumentMarginRate->InvestorID,
                        pInstrumentMarginRate->HedgeFlag,
                        pInstrumentMarginRate->LongMarginRatioByMoney,
                        pInstrumentMarginRate->LongMarginRatioByVolume,
                        pInstrumentMarginRate->ShortMarginRatioByMoney,
                        pInstrumentMarginRate->ShortMarginRatioByVolume,
                        pInstrumentMarginRate->IsRelative);
    }
    else
    {
        LOG_INFO("OnRspQryInstrumentMarginRate(�����ѯ��Լ��֤������Ӧ) : pInstrumentMarginRate=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],",
            pInstrumentMarginRate,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}

///�����ѯ��Լ����������Ӧ
void CPlatformCTPTrade::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryInstrumentCommissionRate,
			pInstrumentCommissionRate,pInstrumentCommissionRate?sizeof(*pInstrumentCommissionRate):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pInstrumentCommissionRate)
    {
        LOG_INFO("OnRspQryInstrumentCommissionRate(�����ѯ��Լ����������Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InstrumentID=[%s],\t InvestorRange=[%d],\t BrokerID=[%s],\t InvestorID=[%s],\t OpenRatioByMoney=[%g],\n"
                        "\t\t\t OpenRatioByVolume=[%g],\t CloseRatioByMoney=[%g],\t CloseRatioByVolume=[%g],\t CloseTodayRatioByMoney=[%g],\t CloseTodayRatioByVolume=[%g]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                        pInstrumentCommissionRate->InstrumentID,
                        pInstrumentCommissionRate->InvestorRange,
                        pInstrumentCommissionRate->BrokerID,
                        pInstrumentCommissionRate->InvestorID,
                        pInstrumentCommissionRate->OpenRatioByMoney,
                        pInstrumentCommissionRate->OpenRatioByVolume,
                        pInstrumentCommissionRate->CloseRatioByMoney,
                        pInstrumentCommissionRate->CloseRatioByVolume,
                        pInstrumentCommissionRate->CloseTodayRatioByMoney,
                        pInstrumentCommissionRate->CloseTodayRatioByVolume);
    }
    else
    {
        LOG_INFO("OnRspQryInstrumentCommissionRate(�����ѯ��Լ����������Ӧ) : pInstrumentCommissionRate=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
                         pInstrumentCommissionRate,pRspInfo,
						 pRspInfo?pRspInfo->ErrorID:0, 
                         pRspInfo?pRspInfo->ErrorMsg:"",
                         nRequestID,bIsLast);
    }
}


///�����ѯ��Լ��Ӧ
void CPlatformCTPTrade::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    //�޸������� ��� ��Լ����Ϊ0������
	if(pInstrument&&
        strcmp(pInstrument->ExchangeID,"DCE")==0&&
		pInstrument->ProductClass==THOST_FTDC_PC_Combination&&
		pInstrument->VolumeMultiple==0)
	{
		pInstrument->VolumeMultiple=1;
	}

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryInstrument,
			pInstrument,pInstrument?sizeof(*pInstrument):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pInstrument)
    {
        LOG_INFO("OnRspQryInstrument(�����ѯ��Լ��Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
			     "\t\t\t InstrumentID=[%s],\t ExchangeID=[%s],\t InstrumentName=[%s],\t ExchangeInstID=[%s],\t ProductID=[%s], \n"
			     "\t\t\t ProductClass=[%c],\t DeliveryYear=[%u],\t DeliveryMonth=[%u],\t MaxMarketOrderVolume=[%d],\t MinMarketOrderVolume=[%d], \n"
			     "\t\t\t MaxLimitOrderVolume=[%d],\t MinLimitOrderVolume=[%d],\t VolumeMultiple=[%d],\t PriceTick=[%g],\t CreateDate=[%s], \n"
			     "\t\t\t OpenDate=[%s],\t ExpireDate=[%s],\t StartDelivDate=[%s],\t EndDelivDate=[%s],\t InstLifePhase=[%c], \n"
			     "\t\t\t IsTrading=[%d],\t PositionType=[%c],\t PositionDateType=[%c],\t LongMarginRatio=[%g],\t ShortMarginRatio=[%g]\n", 
			     pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID, bIsLast,
			     pInstrument->InstrumentID, pInstrument->ExchangeID, pInstrument->InstrumentName, pInstrument->ExchangeInstID, pInstrument->ProductID, 
			     pInstrument->ProductClass, pInstrument->DeliveryYear, pInstrument->DeliveryMonth, pInstrument->MaxMarketOrderVolume, pInstrument->MinMarketOrderVolume, 
			     pInstrument->MaxLimitOrderVolume, pInstrument->MinLimitOrderVolume, pInstrument->VolumeMultiple, pInstrument->PriceTick, pInstrument->CreateDate, 
			     pInstrument->OpenDate, pInstrument->ExpireDate, pInstrument->StartDelivDate, pInstrument->EndDelivDate, pInstrument->InstLifePhase, 
			     pInstrument->IsTrading, pInstrument->PositionType, pInstrument->PositionDateType, pInstrument->LongMarginRatio, pInstrument->ShortMarginRatio
			     );
    }
    else
    {
        LOG_INFO("OnRspQryInstrument(�����ѯ��Լ��Ӧ) : pInstrument=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
                         pInstrument,pRspInfo,
						 pRspInfo?pRspInfo->ErrorID:0, 
                         pRspInfo?pRspInfo->ErrorMsg:"",
                         nRequestID,bIsLast);
    }

}


///�����ѯͶ���߽�������Ӧ
void CPlatformCTPTrade::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQrySettlementInfo,
			pSettlementInfo,pSettlementInfo?sizeof(*pSettlementInfo):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pSettlementInfo)
    {
        LOG_INFO("OnRspQrySettlementInfo(�����ѯͶ���߽�������Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t TradingDay=[%s],\t SettlementID=[%d],\t BrokerID=[%s],\t InvestorID=[%s],\t SequenceNo=[%d],\n"
                        "\t\t\t Content=[%s]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                        pSettlementInfo->TradingDay, pSettlementInfo->SettlementID,pSettlementInfo->BrokerID,pSettlementInfo->InvestorID,pSettlementInfo->SequenceNo,
                        pSettlementInfo->Content);
    }
    else
    {
        LOG_INFO("OnRspQrySettlementInfo(�����ѯͶ���߽�������Ӧ) : pSettlementInfo=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],",
                         pSettlementInfo,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}


///�����ѯͶ���ֲ߳���ϸ��Ӧ
void CPlatformCTPTrade::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryInvestorPositionDetail,
			pInvestorPositionDetail,pInvestorPositionDetail?sizeof(*pInvestorPositionDetail):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pInvestorPositionDetail)
    {
        LOG_INFO("OnRspQryInvestorPositionDetail(�����ѯͶ���ֲ߳���ϸ��Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InstrumentID=[%s],\t BrokerID=[%s],\t InvestorID=[%s],\t HedgeFlag=[%d],\t Direction=[%d],\n"
                        "\t\t\t OpenDate=[%s],\t TradeID=[%s],\t Volume=[%d],\t OpenPrice=[%g],\t TradingDay=[%s],\n"
                        "\t\t\t SettlementID=[%d],\t  TradeType=[%d],\t CombInstrumentID=[%s],\t ExchangeID=[%s],\t CloseProfitByDate=[%g],\n"
                        "\t\t\t CloseProfitByTrade=[%g],\t PositionProfitByDate=[%g],\t PositionProfitByTrade=[%g],\t Margin=[%g],\t ExchMargin=[%g],\n"
                        "\t\t\t MarginRateByMoney=[%g],\t MarginRateByVolume=[%g],\t LastSettlementPrice=[%g],\t SettlementPrice=[%g],CloseVolume=[%d],\n"
                        "\t\t\t CloseAmount=[%g]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                        pInvestorPositionDetail->InstrumentID, pInvestorPositionDetail->BrokerID,pInvestorPositionDetail->InvestorID,pInvestorPositionDetail->HedgeFlag,pInvestorPositionDetail->Direction,
                        pInvestorPositionDetail->OpenDate, pInvestorPositionDetail->TradeID,pInvestorPositionDetail->Volume,pInvestorPositionDetail->OpenPrice,pInvestorPositionDetail->TradingDay,
                        pInvestorPositionDetail->SettlementID, pInvestorPositionDetail->TradeType,pInvestorPositionDetail->CombInstrumentID,pInvestorPositionDetail->ExchangeID,pInvestorPositionDetail->CloseProfitByDate,
                        pInvestorPositionDetail->CloseProfitByTrade, pInvestorPositionDetail->PositionProfitByDate,pInvestorPositionDetail->PositionProfitByTrade,pInvestorPositionDetail->Margin,pInvestorPositionDetail->ExchMargin,
                        pInvestorPositionDetail->MarginRateByMoney, pInvestorPositionDetail->MarginRateByVolume,pInvestorPositionDetail->LastSettlementPrice,pInvestorPositionDetail->SettlementPrice,pInvestorPositionDetail->CloseVolume,
                        pInvestorPositionDetail->CloseAmount);
    }
    else
    {
        LOG_INFO("OnRspQryInvestorPositionDetail(�����ѯͶ���ֲ߳���ϸ��Ӧ) : pInvestorPositionDetail=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
                         pInvestorPositionDetail,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}

///�����ѯͶ���ֲ߳���Ӧ
void CPlatformCTPTrade::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryInvestorPosition,
			pInvestorPosition,pInvestorPosition?sizeof(*pInvestorPosition):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pInvestorPosition)
    {
        LOG_INFO("OnRspQryInvestorPosition(�����ѯͶ���ֲ߳���Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InstrumentID=[%s],\t BrokerID=[%s],\t InvestorID=[%s],\t PosiDirection=[%d],\t HedgeFlag=[%d],\n"
                        "\t\t\t PositionDate=[%d],\t YdPosition=[%d],\t Position=[%d],\t LongFrozen=[%d],\t ShortFrozen=[%d],\n"
                        "\t\t\t LongFrozenAmount=[%g],\t  ShortFrozenAmount=[%g],\t OpenVolume=[%d],\t CloseVolume=[%d],\t OpenAmount=[%g],\n"
                        "\t\t\t CloseAmount=[%g],\t PositionCost=[%g],\t PreMargin=[%g],\t UseMargin=[%g],\t FrozenMargin=[%g],\n"
                        "\t\t\t FrozenCash=[%g],\t FrozenCommission=[%g],\t CashIn=[%g],\t Commission=[%g],CloseProfit=[%g],\n"
                        "\t\t\t PositionProfit=[%g],\t PreSettlementPrice=[%g],\t SettlementPrice=[%g],\t TradingDay=[%s],\t SettlementID=[%d],\n"
                        "\t\t\t OpenCost=[%g],\t ExchangeMargin=[%g],\t CombPosition=[%d],\t CombLongFrozen=[%d],\t CombShortFrozen=[%d],\n"
                        "\t\t\t CloseProfitByDate=[%g],\t CloseProfitByTrade=[%g],\t TodayPosition=[%d],\t MarginRateByMoney=[%g],\t MarginRateByVolume=[%g]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                        pInvestorPosition->InstrumentID, pInvestorPosition->BrokerID,pInvestorPosition->InvestorID,pInvestorPosition->PosiDirection,pInvestorPosition->HedgeFlag,
                        pInvestorPosition->PositionDate, pInvestorPosition->YdPosition,pInvestorPosition->Position,pInvestorPosition->LongFrozen,pInvestorPosition->ShortFrozen,
                        pInvestorPosition->LongFrozenAmount, pInvestorPosition->ShortFrozenAmount,pInvestorPosition->OpenVolume,pInvestorPosition->CloseVolume,pInvestorPosition->OpenAmount,
                        pInvestorPosition->CloseAmount, pInvestorPosition->PositionCost,pInvestorPosition->PreMargin,pInvestorPosition->UseMargin,pInvestorPosition->FrozenMargin,
                        pInvestorPosition->FrozenCash, pInvestorPosition->FrozenCommission,pInvestorPosition->CashIn,pInvestorPosition->Commission,pInvestorPosition->CloseProfit,
                        pInvestorPosition->PositionProfit, pInvestorPosition->PreSettlementPrice,pInvestorPosition->SettlementPrice,pInvestorPosition->TradingDay,pInvestorPosition->SettlementID,
                        pInvestorPosition->OpenCost, pInvestorPosition->ExchangeMargin,pInvestorPosition->CombPosition,pInvestorPosition->CombLongFrozen,pInvestorPosition->CombShortFrozen,
                        pInvestorPosition->CloseProfitByDate, pInvestorPosition->CloseProfitByTrade,pInvestorPosition->TodayPosition,pInvestorPosition->MarginRateByMoney,pInvestorPosition->MarginRateByVolume
                        );
    }
    else
    {
        LOG_INFO("OnRspQryInvestorPosition(�����ѯͶ���ֲ߳���Ӧ) : pInvestorPosition=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
                         pInvestorPosition,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }


}


///�����ѯͶ������ϳֲ���ϸ��Ӧ
void CPlatformCTPTrade::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);


	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryInvestorPositionCombDetail,
			pDetail,pDetail?sizeof(*pDetail):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pDetail)
    {
		LOG_INFO("OnRspQryInvestorPositionCombineDetail(�����ѯͶ���ֲ߳���Ϻ�Լ��ϸ��Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
				 "\t\t\t TradingDay=[%s],\t OpenDate=[%s],\t ExchangeID=[%s],\t SettlementID=[%d],\t BrokerID=[%s], \n"
				 "\t\t\t InvestorID=[%s],\t ComTradeID=[%s],\t TradeID=[%s],\t InstrumentID=[%s],\t HedgeFlag=[%c], \n"
				 "\t\t\t Direction=[%c],\t TotalAmt=[%d],\t Margin=[%g],\t ExchMargin=[%g],\t MarginRateByMoney=[%g], \n"
				 "\t\t\t MarginRateByVolume=[%g],\t LegID=[%d],\t LegMultiple=[%d],\t CombInstrumentID=[%s]", 
				 pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
				 pDetail->TradingDay, pDetail->OpenDate, pDetail->ExchangeID, pDetail->SettlementID, pDetail->BrokerID, 
				 pDetail->InvestorID, pDetail->ComTradeID, pDetail->TradeID, pDetail->InstrumentID, pDetail->HedgeFlag, 
				 pDetail->Direction, pDetail->TotalAmt, pDetail->Margin, pDetail->ExchMargin, pDetail->MarginRateByMoney, 
				 pDetail->MarginRateByVolume, pDetail->LegID, pDetail->LegMultiple, pDetail->CombInstrumentID);
    }
    else
    {
        LOG_INFO("OnRspQryInvestorPositionCombineDetail(�����ѯͶ���ֲ߳���Ϻ�Լ��ϸ��Ӧ) : pDetail=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
			pDetail,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}



///�����ѯ�ͻ�֪ͨ��Ӧ
void CPlatformCTPTrade::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryNotice,
			pNotice,pNotice?sizeof(*pNotice):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pNotice)
    {
        LOG_INFO("OnRspQryNotice(�����ѯ�ͻ�֪ͨ��Ӧ) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
				 "\t\t\t BrokerID=%s Content=%s SequenceLabel=%s",
				 pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                 pNotice->BrokerID,pNotice->Content,pNotice->SequenceLabel);
    }
    else
    {
        LOG_INFO("OnRspQryNotice(�����ѯ�ͻ�֪ͨ��Ӧ) : pNotice=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
			pNotice,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}


///����Ӧ��
void CPlatformCTPTrade::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LOG_INFO("CPlatformCTPTrade::OnRspError,pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
		pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID, bIsLast);
}

///����֪ͨ
void CPlatformCTPTrade::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Trade_RtnOrder,pOrder,pOrder?sizeof(*pOrder):0);

    if(pOrder)
    {
        LOG_INFO("OnRtnOrder(����֪ͨ) : \n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                        "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
                        "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
                        "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
                        "\t\t\t RequestID=[%d],\t OrderLocalID=[%s],\t ExchangeID=[%s],\t ParticipantID=[%s],\t ClientID=[%s],\n"
                        "\t\t\t ExchangeInstID=[%s],\t TraderID=[%s],\t InstallID=[%d],\t OrderSubmitStatus=[%d],\t NotifySequence=[%d],\n"
                        "\t\t\t TradingDay=[%s],\t SettlementID=[%d],\t OrderSysID=[%s],\t OrderSource=[%d],\t OrderStatus=[%d],\n"
                        "\t\t\t OrderType=[%d],\t VolumeTraded=[%d],\t VolumeTotal=[%d],\t InsertDate=[%s],\t InsertTime=[%s],\n"
                        "\t\t\t ActiveTime=[%s],\t SuspendTime=[%s],\t UpdateTime=[%s],\t CancelTime=[%s],\t ActiveTraderID=[%s]\n"
                        "\t\t\t ClearingPartID=[%s],\t SequenceNo=[%d],\t FrontID=[%d],\t SessionID=[%#x],\t UserProductInfo=[%s],\n"
                        "\t\t\t StatusMsg=[%s],\t UserForceClose=[%d],\t ActiveUserID=[%s],\t BrokerOrderSeq=[%d],RelativeOrderSysID=[%s]",
                        pOrder->BrokerID,pOrder->InvestorID,pOrder->InstrumentID,pOrder->OrderRef,pOrder->UserID,
                        pOrder->OrderPriceType,pOrder->Direction,pOrder->CombOffsetFlag,pOrder->CombHedgeFlag,pOrder->LimitPrice,
                        pOrder->VolumeTotalOriginal,pOrder->TimeCondition,pOrder->GTDDate,pOrder->VolumeCondition,pOrder->MinVolume,
                        pOrder->ContingentCondition,pOrder->StopPrice,pOrder->ForceCloseReason,pOrder->IsAutoSuspend,pOrder->BusinessUnit,
                        pOrder->RequestID,pOrder->OrderLocalID,pOrder->ExchangeID,pOrder->ParticipantID,pOrder->ClientID,
                        pOrder->ExchangeInstID,pOrder->TraderID,pOrder->InstallID,pOrder->OrderSubmitStatus,pOrder->NotifySequence,
                        pOrder->TradingDay,pOrder->SettlementID,pOrder->OrderSysID,pOrder->OrderSource,pOrder->OrderStatus,
                        pOrder->OrderType,pOrder->VolumeTraded,pOrder->VolumeTotal,pOrder->InsertDate,pOrder->InsertTime,
                        pOrder->ActiveTime,pOrder->SuspendTime,pOrder->UpdateTime,pOrder->CancelTime,pOrder->ActiveTraderID,
                        pOrder->ClearingPartID,pOrder->SequenceNo,pOrder->FrontID,pOrder->SessionID,pOrder->UserProductInfo,
                        pOrder->StatusMsg,pOrder->UserForceClose,pOrder->ActiveUserID,pOrder->BrokerOrderSeq,pOrder->RelativeOrderSysID
                        );
    }
    else 
    {
        LOG_INFO("OnRtnOrder(����֪ͨ),pOrder is NULL");
    }
}

///�ɽ�֪ͨ
void CPlatformCTPTrade::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Trade_RtnTrade,pTrade,pTrade?sizeof(*pTrade):0);

    if(pTrade)
    {
        LOG_INFO("OnRtnTrade(�ɽ�֪ͨ) : \n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                        "\t\t\t ExchangeID=[%s],\t TradeID=[%s],\t Direction=[%d],\t OrderSysID=[%s],\t ParticipantID=[%s],\n"
                        "\t\t\t ClientID=[%s],\t TradingRole=[%d],\t ExchangeInstID=[%s],\t OffsetFlag=[%d],\t HedgeFlag=[%d],\n"
                        "\t\t\t Price=[%g],\t Volume=[%d],\t TradeDate=[%s],\t TradeTime=[%s],\t TradeType=[%d],\n"
                        "\t\t\t PriceSource=[%d],\t TraderID=[%s],\t OrderLocalID=[%s],\t ClearingPartID=[%s],\t BusinessUnit=[%s],\n"
                        "\t\t\t SequenceNo=[%d],\t TradingDay=[%s],\t SettlementID=[%d],\t BrokerOrderSeq=[%d]",
                        pTrade->BrokerID,pTrade->InvestorID,pTrade->InstrumentID,pTrade->OrderRef,pTrade->UserID,
                        pTrade->ExchangeID,pTrade->TradeID,pTrade->Direction,pTrade->OrderSysID,pTrade->ParticipantID,
                        pTrade->ClientID,pTrade->TradingRole,pTrade->ExchangeInstID,pTrade->OffsetFlag,pTrade->HedgeFlag,
                        pTrade->Price,pTrade->Volume,pTrade->TradeDate,pTrade->TradeTime,pTrade->TradeType,
                        pTrade->PriceSource,pTrade->TraderID,pTrade->OrderLocalID,pTrade->ClearingPartID,pTrade->BusinessUnit,
                        pTrade->SequenceNo,pTrade->TradingDay,pTrade->SettlementID,pTrade->BrokerOrderSeq,pTrade->TradeSource
                        );
    }
    else
    {
        LOG_INFO("OnRtnTrade(�ɽ�֪ͨ),pTrade is NULL");
    }
}

///����¼�����ر�
void CPlatformCTPTrade::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Trade_ErrRtnOrderInsert,pInputOrder,pInputOrder?sizeof(*pInputOrder):0,pRspInfo,pRspInfo?sizeof(*pRspInfo):0);  

    if(pInputOrder)
    {
        LOG_INFO("OnErrRtnOrderInsert(����¼�����ر�) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],\n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                        "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
                        "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
                        "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
                        "\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", 
                        pInputOrder->BrokerID, pInputOrder->InvestorID, pInputOrder->InstrumentID, pInputOrder->OrderRef,pInputOrder->UserID,
                        pInputOrder->OrderPriceType, pInputOrder->Direction, pInputOrder->CombOffsetFlag, pInputOrder->CombHedgeFlag,pInputOrder->LimitPrice,
                        pInputOrder->VolumeTotalOriginal, pInputOrder->TimeCondition, pInputOrder->GTDDate, pInputOrder->VolumeCondition,pInputOrder->MinVolume,
                        pInputOrder->ContingentCondition, pInputOrder->StopPrice, pInputOrder->ForceCloseReason, pInputOrder->IsAutoSuspend,pInputOrder->BusinessUnit,
                        pInputOrder->RequestID, pInputOrder->UserForceClose);
    }
    else
    {
        LOG_INFO("OnRspOrderInsert(����¼�����ر�),pInputOrder=[%x] pRspInfo=[%x]",pInputOrder,pRspInfo);
    }
}

///������������ر�
void CPlatformCTPTrade::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Trade_ErrRtnOrderAction,pOrderAction,pOrderAction?sizeof(*pOrderAction):0,pRspInfo,pRspInfo?sizeof(*pRspInfo):0);  

    if(pOrderAction)
    {
        LOG_INFO("OnErrRtnOrderAction(������������ر�) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],\n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
                        "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
                        "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t ActionDate=[%s],\t ActionTime=[%s],\t TraderID=[%s],\n"
                        "\t\t\t InstallID=[%d],\t OrderLocalID=[%s],\t ActionLocalID=[%s],\t ParticipantID=[%s],\t ClientID=[%s],\n"
                        "\t\t\t BusinessUnit=[%s],\t OrderActionStatus=[%d],\t UserID=[%s],\t StatusMsg=[%s],\t InstrumentID=[%s]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",
                        pOrderAction->BrokerID, pOrderAction->InvestorID, pOrderAction->OrderActionRef, pOrderAction->OrderRef,pOrderAction->RequestID,
                        pOrderAction->FrontID, pOrderAction->SessionID, pOrderAction->ExchangeID, pOrderAction->OrderSysID,pOrderAction->ActionFlag,
                        pOrderAction->LimitPrice, pOrderAction->VolumeChange, pOrderAction->ActionDate, pOrderAction->ActionTime,pOrderAction->TraderID,
                        pOrderAction->InstallID, pOrderAction->OrderLocalID, pOrderAction->ActionLocalID, pOrderAction->ParticipantID,pOrderAction->ClientID,
                        pOrderAction->BusinessUnit, pOrderAction->OrderActionStatus, pOrderAction->UserID, pOrderAction->StatusMsg,pOrderAction->InstrumentID
    			    );
    }
    else
    {
        LOG_INFO("OnErrRtnOrderAction(������������ر�) : pOrderAction=[%x] pRspInfo=[%x]",pOrderAction,pRspInfo);
    }
 }

///��Լ����״̬֪ͨ
void CPlatformCTPTrade::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Trade_RtnInstrumentStatus,pInstrumentStatus,pInstrumentStatus?sizeof(*pInstrumentStatus):0);  

    if(pInstrumentStatus)
    {
        LOG_INFO("OnRtnInstrumentStatus(��Լ����״̬֪ͨ):\n"
                         "\t\t\t ExchangeID=[%s],\t ExchangeInstID=[%s],\t SettlementGroupID=[%s],\t InstrumentID=[%s],\t InstrumentStatus=[%d],\n"
                         "\t\t\t TradingSegmentSN=[%d],\t EnterTime=[%s],\t EnterReason=[%d]",
                         pInstrumentStatus->ExchangeID,
                         pInstrumentStatus->ExchangeInstID,
                         pInstrumentStatus->SettlementGroupID,
                         pInstrumentStatus->InstrumentID,
                         pInstrumentStatus->InstrumentStatus,
                         pInstrumentStatus->TradingSegmentSN,
                         pInstrumentStatus->EnterTime,
                         pInstrumentStatus->EnterReason);
    }
    else
    {
        LOG_INFO("OnRtnInstrumentStatus(��Լ����״̬֪ͨ),pInstrumentStatus is NULL");
    }
}

///����֪ͨ
void CPlatformCTPTrade::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Trade_RtnTradingNotice,pTradingNoticeInfo,pTradingNoticeInfo?sizeof(*pTradingNoticeInfo):0);  

    if(pTradingNoticeInfo)
    {
        LOG_INFO("OnRtnTradingNotice(����֪ͨ):\n"
               "\t\t\t BrokerID=[%s],\tInvestorID=[%s],SendTime=[%s],\n"
               "\t\t\t FieldContent=[%s],\n"
               "\t\t\t SequenceSeries=[%d],SequenceNo=[%d]",
                 pTradingNoticeInfo->BrokerID,
                 pTradingNoticeInfo->InvestorID,
                 pTradingNoticeInfo->SendTime,
                 pTradingNoticeInfo->FieldContent,
                 pTradingNoticeInfo->SequenceSeries,
                 pTradingNoticeInfo->SequenceNo);
    }
    else
    {
        LOG_INFO("OnRtnTradingNotice(����֪ͨ),pTradingNoticeInfo is NULL");
    }
}
