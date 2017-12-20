
#include "SimpletradeSpi.h"
#include "PlatformCTPQuotImpl.h"
#include "..\\SvrDBOpr\\Interface_SvrDBOpr.h"
#include "FileOpr.h"
#include "MyXML.h"


#if 0
#define LOG_INFO(fmt, ...)  ;
#else
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","PlatformCTPQuot", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif

//��¼����CTP event
HANDLE g_hEvent = CreateEvent(NULL, true, false, NULL);
//extern bool bLoginCTPTrade;


SimpletradeSpi::SimpletradeSpi(CThostFtdcTraderApi *pUserApi) :
m_pUserApi(pUserApi) 
{
	m_vecInstruemnts.clear();
	m_mapInstruemnts.clear();
}

void SimpletradeSpi::OnFrontConnected()
{
	 memset(&m_reqUserLogin,0,sizeof(m_reqUserLogin));

	string BrokerID=CMyXML("Account.xml").GetNodeContent2("","root","BrokerID");
	strcpy(m_reqUserLogin. BrokerID, BrokerID.c_str());

	string UserID=CMyXML("Account.xml").GetNodeContent2("","root","CTPUserID");
	strcpy(m_reqUserLogin.UserID, UserID.c_str());

	string CTPPassword=CMyXML("Account.xml").GetNodeContent2("","root","CTPPassword");
	strcpy(m_reqUserLogin.Password, CTPPassword.c_str());

	// ������½����
	m_pUserApi->ReqUserLogin(&m_reqUserLogin, 0);
		LOG_INFO(" SimpletradeSpi::OnFrontConnected()�����ӽ��׳ɹ� ������½����m_pUserApi->ReqUserLogin;");
}
void SimpletradeSpi::OnFrontDisconnected(int nReason)
{
	// ��������������API���Զ��������ӣ��ͻ��˿ɲ�������
	printf("OnFrontDisconnected.\n");
	LOG_INFO(" SimpletradeSpi::OnFrontDisconnected()�����ӽ���ʧ��;");
   
}
void SimpletradeSpi::OnRspUserLogin(CThostFtdcRspUserLoginField	*pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool	bIsLast)
{
	printf("OnRspUserLogin:\n");
	printf("ErrorCode=[%d], ErrorMsg=[%s]\n",
		pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	LOG_INFO(" SimpletradeSpi::OnRspUserLogin()��ErrorCode=[%d], ErrorMsg=[%s]\n;",
		pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	m_mapInstruemnts.clear();
	if (pRspInfo->ErrorID != 0) 
	{
		// �˵�ʧ�ܣ��ͻ�������д�����
		printf("Failed to login, errorcode=%d ,errormsg=%s,requestid=%d ,chain=%d", pRspInfo->ErrorID, pRspInfo->ErrorMsg,
			nRequestID, bIsLast);

		////֪ͨ���̵߳�¼����ʧ��
		//Stru_NotifyEvent lDataEventLoginCTPFail(EventLoginCTPFail,0);
		//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEventLoginCTPFail);


		////����һ��ÿ1���Ӵ���һ�εĶ�ʱ�� �������µ�¼����CTP 
		//if(!g_ReLoginTime)
		//    g_ReLoginTime = SetTimer(NULL,NULL,1*60*1000,(TIMERPROC)(NULL));
		SetEvent(g_hEvent);
		return;


		//if (m_mapInstruemnts.size()>0)
		//{
		//	LOG_INFO(" �ѱ��浱�պ�Լ������");
		//	SetEvent(g_hEvent);
		//    return;
		//}
		//else
		//{
		//	// �ٴη�����½����
		//	m_pUserApi->ReqUserLogin(&m_reqUserLogin, 0);
		//	LOG_INFO(" SimpletradeSpi::OnFrontConnected()�����ӽ��׳ɹ� �ٴη�����½����m_pUserApi->ReqUserLogin;");
		//}
		
	}

	// �˵ǳɹ�,����������

		SaveTradingDay2DB(pRspUserLogin->TradingDay);
	//......
	//�����ѯ��Լ
	CThostFtdcQryInstrumentField QryInstrument;
	memset(&QryInstrument, 0, sizeof(QryInstrument));	
	m_pUserApi->ReqQryInstrument(&QryInstrument, 0);
	LOG_INFO(" SimpletradeSpi::OnRspUserLogin()�������ѯ��Լ ReqQryInstrument");
}
///�����ѯ��Լ��Ӧ
void SimpletradeSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//LOG_INFO(" SimpletradeSpi::OnRspQryInstrument");
	if(pInstrument)
	{
		//LOG_INFO("SimpletradeSpi::OnRspQryInstrument ��ѯ����Լ�� %s\n", pInstrument->InstrumentID);
	
		m_vecInstruemnts.push_back(*pInstrument);
		m_mapInstruemnts[pInstrument->InstrumentID]=*pInstrument;
		if(pInstrument->ProductClass != THOST_FTDC_PC_Combination )
		CPlatformCTPQuotImpl::GetInstance().SubscribeMarketData( pInstrument->InstrumentID);



		if(bIsLast)
		{
			//SaveInstrument2DB(m_vecInstruemnts);

			/*std::map<std::string,std::set<SOCKET>> mapIns2Socket;
			CPlatformCTPQuotImpl::GetInstance().GetSubscribedInst2Socket(mapIns2Socket);
			vector<PlatformStru_InstrumentInfo>::iterator it = m_vecInstruemnts.begin();
			while(it != m_vecInstruemnts.end())
			{
				std::map<std::string,std::set<SOCKET>>::iterator its = find(*it);
				if (its == mapIns2Socket.end())
				{
					mapIns2Socket
				}

			}*/
			//�����Լ��SetEvent��ʹapiʵ���ܱ��ͷ�
			SetEvent(g_hEvent);
		}
	}
}
bool SimpletradeSpi::SaveInstrument2DB(vector<PlatformStru_InstrumentInfo>& nVecInstruemnts)
{
	char strName[128];
	char strTime[128];

	int nErrorCode=CF_ERROR_SUCCESS;
	std::string szTableName = "INSTRUMENT";
	std::string lsDeleteSql = "";

	SYSTEMTIME st;
	GetSystemTime(&st);
	memset(strName, 0, sizeof(strName));
	memset(strTime, 0, sizeof(strTime));
	sprintf(strTime,"%.4d%.2d%.2d",st.wYear,st.wMonth,st.wDay);
	sprintf(strName,"%s_%.4d%.2d%.2d",szTableName.c_str(),st.wYear,st.wMonth,st.wDay);

	if (CInterface_SvrDBOpr::getObj().CreatInsTable_PreDay(strName,nErrorCode ))
	{	
	    return CInterface_SvrDBOpr::getObj().SaveInstruments(strTime,nVecInstruemnts,nErrorCode,szTableName);
	}
	else
		return false;
}
void SimpletradeSpi::GetInstrument(map<std::string,PlatformStru_InstrumentInfo>& outData)
{
	outData=m_mapInstruemnts;
}
bool SimpletradeSpi::SaveTradingDay2DB(std::string strTradingday)
{
	int nErrorCode = CF_ERROR_SUCCESS;

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));

	/*sprintf(szBuffer, "select * from AQUOT_TRIGGER t where t.tradingday =\'%s\'", strTradingday.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer,"insert into AQUOT_TRIGGER values(\'%s\',sysdate)",strTradingday.c_str());
	    if (CInterface_SvrDBOpr::getObj().Excute( szBuffer,nErrorCode ))
	    {
			return true;
	    }
		else
			return false;
	    
		
	}
	else
	{      
		return true;
	}*/
	sprintf(szBuffer,"insert into AQUOT_TRIGGER values(\'%s\',sysdate)",strTradingday.c_str());
	if (CInterface_SvrDBOpr::getObj().Excute( szBuffer,nErrorCode ))
	{
		return true;
	}
	else
		return false;

}