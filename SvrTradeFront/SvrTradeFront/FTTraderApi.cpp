#include "FTTraderApi.h"
#include "SvrTradeFront.h"

#include "FileOpr.h"
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","SvrTradefrontModule", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);



extern set<string>			  g_setTradeFrontAdds;



CFTTraderApi::CFTTraderApi(void)
:m_pTradeApi(NULL)
,m_TradeStatus(CTPCONNSTATUS_Disconnected)
{
}

CFTTraderApi::~CFTTraderApi(void)
{	
}

void CFTTraderApi::Init(int nSocket)
{
	m_pTradeApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	m_TradeSpi.SetSocket(nSocket);
	m_TradeSpi.SetCTPTraderApi(m_pTradeApi);
	m_pTradeApi->RegisterSpi(&m_TradeSpi);

	m_pTradeApi->SubscribePublicTopic(THOST_TERT_QUICK);
	m_pTradeApi->SubscribePrivateTopic(THOST_TERT_QUICK);

	string addrstr;
	for(std::set<std::string>::iterator it=g_setTradeFrontAdds.begin();it!=g_setTradeFrontAdds.end();++it)
	{		
		addrstr = "tcp://" + *it;//addrstr=string("tcp://180.168.102.193:41205");
		m_pTradeApi->RegisterFront(const_cast<char*>(addrstr.c_str()));// ����ǰ�û�

	}
	m_pTradeApi->Init();
}

void CFTTraderApi::Init(LPVOID pLinkEntity)
{
	m_pTradeApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	m_TradeSpi.SetLinkEntity(pLinkEntity);
	m_TradeSpi.SetCTPTraderApi(m_pTradeApi);
	m_pTradeApi->RegisterSpi(&m_TradeSpi);

	m_pTradeApi->SubscribePublicTopic(THOST_TERT_QUICK);
	m_pTradeApi->SubscribePrivateTopic(THOST_TERT_QUICK);

	string addrstr;
	for(std::set<std::string>::iterator it=g_setTradeFrontAdds.begin();it!=g_setTradeFrontAdds.end();++it)
	{		
		addrstr = "tcp://" + *it;//addrstr=string("tcp://180.168.102.193:41205");
		m_pTradeApi->RegisterFront(const_cast<char*>(addrstr.c_str()));// ����ǰ�û�

	}
	m_pTradeApi->Init();
}

void CFTTraderApi::Release()
{
	m_pTradeApi->RegisterSpi(NULL);
	m_pTradeApi->Release();
	m_pTradeApi=NULL;
}

//�����ڱ�ģ��ʹ��
#define CHECK_TRADE_STATUS() \
	if(!m_pTradeApi || m_TradeStatus!=CTPCONNSTATUS_Connected)\
{\
	LOG_INFO("����API��δ׼������, ���ܵ���δ�ɹ�����������");\
	return ret;\
}
/***********************************************************
*   return  :   0:�ɹ�
*               -1:     ��������ʧ��
*               -2:     δ�������󳬹������
*               -3:     ÿ�뷢�����������������
*               -999:   ����ԭ��ʧ��
***********************************************************/
///�û���¼����
int CFTTraderApi::ReqUserLogin(CThostFtdcReqUserLoginField *pData, int nRequestID)
{	
	int ret = 0;	
	
	m_TradeSpi.LoginCtp(pData, nRequestID);
	ret = m_pTradeApi->ReqUserLogin(pData, nRequestID);
	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqUserLogin(�û���¼����):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t TradingDay=[%s],\t BrokerID=[%s],\t UserID=[%s],\t Password=[******],\t UserProductInfo=[%s],\n"
		"\t\t\t InterfaceProductInfo=[%s],\t ProtocolInfo=[%s]",
		ret, nRequestID,
		pData->TradingDay,
		pData->BrokerID,
		pData->UserID,
		/*pData->Password,*/
		pData->UserProductInfo,
		pData->InterfaceProductInfo,
		pData->ProtocolInfo);

	return ret;
}

///�ǳ�����
int CFTTraderApi::ReqUserLogout(CThostFtdcUserLogoutField *pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqUserLogout(pData, nRequestID);	
	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqUserLogout(�ǳ�����):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],UserID=[%s]",
		ret, nRequestID,
		pData->BrokerID,
		pData->UserID);

	return ret;
}

///�û������������
int CFTTraderApi::ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate* pData, int nRequestID)
{
	int ret = -999;

	CThostFtdcUserPasswordUpdateField req={0};
	strncpy(req.BrokerID, pData->BrokerID, sizeof(req.BrokerID)-1);
	strncpy(req.UserID, pData->UserID, sizeof(req.UserID)-1);
	strncpy(req.NewPassword, pData->NewPassword, sizeof(req.NewPassword)-1);
	strncpy(req.OldPassword, pData->OldPassword, sizeof(req.OldPassword)-1);

	ret = m_pTradeApi->ReqUserPasswordUpdate(&req, nRequestID);
	if(ret==0)
		_GetForwarding().SendPackageForApi(&req, sizeof(req), nRequestID);

	LOG_INFO("ReqUserPasswordUpdate(�û������������):ret=[%d],nRequestID=[%d],\n"
			"\t\t\t BrokerID=[%s],UserID=[%s],OldPassword=[******],NewPassword=[******]",
			ret, nRequestID,
			req.BrokerID,
			req.UserID);

	return ret;
}

///�ʽ��˻������������
int CFTTraderApi::ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate* pData, int nRequestID)
{
	int ret = -999;

	CThostFtdcTradingAccountPasswordUpdateField req={0};
	strncpy(req.BrokerID, pData->BrokerID, sizeof(req.BrokerID)-1); 
	strncpy(req.AccountID, pData->AccountID, sizeof(req.AccountID)-1);
	strncpy(req.OldPassword, pData->OldPassword, sizeof(req.OldPassword)-1);
	strncpy(req.NewPassword, pData->NewPassword, sizeof(req.NewPassword)-1);
	strncpy(req.CurrencyID, pData->CurrencyID, sizeof(req.CurrencyID)-1);

	ret = m_pTradeApi->ReqTradingAccountPasswordUpdate(&req, nRequestID);  
	if(ret==0)
		_GetForwarding().SendPackageForApi(&req, sizeof(req), nRequestID);

	LOG_INFO("ReqTradingAccountPasswordUpdate(�ʽ��˻������������):ret=[%d],nRequestID=[%d],\n"
				"\t\t\t BrokerID=[%s],AccountID=[%s],OldPassword=[******],NewPassword=[******],CurrencyID=[%s]",
				ret, nRequestID,
				req.BrokerID,
				req.AccountID,
				req.CurrencyID);

	return ret;
}

///����¼������
int CFTTraderApi::ReqOrderInsert(PlatformStru_InputOrder* pData, int nRequestID)
{
	int ret = -999;

	CThostFtdcInputOrderField tField = {0};
	strcpy(tField.BrokerID, pData->BrokerID);					///���͹�˾����
	strcpy(tField.InvestorID, pData->InvestorID);				///Ͷ���ߴ���
	strcpy(tField.InstrumentID, pData->InstrumentID);			///��Լ����
	strcpy(tField.OrderRef, pData->OrderRef);					///��������
	strcpy(tField.UserID, pData->UserID);						///�û�����
	tField.OrderPriceType = pData->OrderPriceType;				///�����۸�����
	tField.Direction = pData->Direction;						///��������
	strcpy(tField.CombOffsetFlag, pData->CombOffsetFlag);		///��Ͽ�ƽ��־
	strcpy(tField.CombHedgeFlag, pData->CombHedgeFlag);			///���Ͷ���ױ���־
	tField.LimitPrice = pData->LimitPrice;						///�۸�
	tField.VolumeTotalOriginal = pData->VolumeTotalOriginal;	///����
	tField.TimeCondition = pData->TimeCondition;				///��Ч������
	strcpy(tField.GTDDate, pData->GTDDate);						///GTD����
	tField.VolumeCondition = pData->VolumeCondition;			///�ɽ�������
	tField.MinVolume = pData->MinVolume;						///��С�ɽ���
	tField.ContingentCondition = pData->ContingentCondition;	///��������
	tField.StopPrice = pData->StopPrice;						///ֹ���
	tField.ForceCloseReason = pData->ForceCloseReason;			///ǿƽԭ��
	tField.IsAutoSuspend = pData->IsAutoSuspend;				///�Զ������־
	strcpy(tField.BusinessUnit, pData->BusinessUnit);			///ҵ��Ԫ
	tField.RequestID = pData->RequestID;						///������
	tField.UserForceClose = pData->UserForceClose;				///�û�ǿ����־

	ret = m_pTradeApi->ReqOrderInsert(&tField, nRequestID);  
	if(ret==0)
		_GetForwarding().SendPackageForApi(&tField, sizeof(tField), nRequestID);

	LOG_INFO("ReqOrderInsert(����¼������):ret=[%d],nRequestID=[%d]\n"
			"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
			"\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
			"\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
			"\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
			"\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
			ret, nRequestID,
			pData->BrokerID, pData->InvestorID, pData->InstrumentID, pData->OrderRef,pData->UserID,
			pData->OrderPriceType, pData->Direction, pData->CombOffsetFlag, pData->CombHedgeFlag,pData->LimitPrice,
			pData->VolumeTotalOriginal, pData->TimeCondition, pData->GTDDate, pData->VolumeCondition,pData->MinVolume,
			pData->ContingentCondition, pData->StopPrice, pData->ForceCloseReason, pData->IsAutoSuspend,pData->BusinessUnit,
			pData->RequestID, pData->UserForceClose);

	return ret;

}

///Ԥ��¼������
int CFTTraderApi::ReqParkedOrderInsert(PlatformStru_ParkedOrder* pData, int nRequestID)
{
	int ret = -999;

	///Ԥ��
	CThostFtdcParkedOrderField req={0};
	strncpy(req.BrokerID, pData->BrokerID, sizeof(req.BrokerID));						///���͹�˾����
	strncpy(req.InvestorID, pData->InvestorID, sizeof(req.InvestorID));					///Ͷ���ߴ���
	strncpy(req.InstrumentID, pData->InstrumentID, sizeof(req.InstrumentID));			///��Լ����
	strncpy(req.OrderRef, pData->OrderRef, sizeof(req.OrderRef));						///��������
	strncpy(req.UserID, pData->UserID, sizeof(req.UserID));								///�û�����
	req.OrderPriceType = pData->OrderPriceType;											///�����۸�����
	req.Direction = pData->Direction;													///��������
	strncpy(req.CombOffsetFlag, pData->CombOffsetFlag, sizeof(req.CombOffsetFlag));		///��Ͽ�ƽ��־
	strncpy(req.CombHedgeFlag, pData->CombHedgeFlag, sizeof(req.CombHedgeFlag));		///���Ͷ���ױ���־
	req.LimitPrice = pData->LimitPrice;													///�۸�
	req.VolumeTotalOriginal = pData->VolumeTotalOriginal;								///����
	req.TimeCondition = pData->TimeCondition;											///��Ч������
	strncpy(req.GTDDate, pData->GTDDate, sizeof(req.GTDDate));							///GTD����
	req.VolumeCondition = pData->VolumeCondition;										///�ɽ�������
	req.MinVolume = pData->MinVolume;													///��С�ɽ���
	req.ContingentCondition = pData->ContingentCondition;								///��������
	req.StopPrice = pData->StopPrice;													///ֹ���
	req.ForceCloseReason = pData->ForceCloseReason;										///ǿƽԭ��
	req.IsAutoSuspend = pData->IsAutoSuspend;											///�Զ������־
	strncpy(req.BusinessUnit, pData->BusinessUnit, sizeof(req.BusinessUnit));			///ҵ��Ԫ
	req.RequestID = pData->RequestID;													///������
	req.UserForceClose = pData->UserForceClose;											///�û�ǿ����־
	strncpy(req.ExchangeID, pData->ExchangeID, sizeof(req.ExchangeID));					///����������
	strncpy(req.ParkedOrderID, pData->ParkedOrderID, sizeof(req.ParkedOrderID));		///Ԥ�񱨵����
	req.UserType = pData->UserType;														///�û�����
	req.Status = pData->Status;															///Ԥ��״̬
	req.ErrorID = pData->ErrorID;														///�������
	strncpy(req.ErrorMsg, pData->ErrorMsg, sizeof(req.ErrorMsg));						///������Ϣ	

	ret = m_pTradeApi->ReqParkedOrderInsert(&req, nRequestID); 
	if(ret==0)
		_GetForwarding().SendPackageForApi(&req, sizeof(req), nRequestID);

	LOG_INFO("ReqOrderInsert(Ԥ�񱨵�¼������):ret=[%d],nRequestID=[%d]\n"
			"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
			"\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
			"\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
			"\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
			"\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
			ret, nRequestID,
			pData->BrokerID, pData->InvestorID, pData->InstrumentID, pData->OrderRef,pData->UserID,
			pData->OrderPriceType, pData->Direction, pData->CombOffsetFlag, pData->CombHedgeFlag,pData->LimitPrice,
			pData->VolumeTotalOriginal, pData->TimeCondition, pData->GTDDate, pData->VolumeCondition,pData->MinVolume,
			pData->ContingentCondition, pData->StopPrice, pData->ForceCloseReason, pData->IsAutoSuspend,pData->BusinessUnit,
			pData->RequestID, pData->UserForceClose);

	return ret;
}

///Ԥ�񳷵�¼������
int CFTTraderApi::ReqParkedOrderAction(PlatformStru_ParkedOrderAction* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqParkedOrderAction(&pData->Thost, nRequestID); 
	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqParkedOrderAction(Ԥ�񳷵�¼������):ret=[%d],nRequestID=[%d]\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
		"\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
		"\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID, pData->Thost.InvestorID, pData->Thost.OrderActionRef, pData->Thost.OrderRef,pData->Thost.RequestID,
		pData->Thost.FrontID, pData->Thost.SessionID, pData->Thost.ExchangeID, pData->Thost.OrderSysID,pData->Thost.ActionFlag,
		pData->Thost.LimitPrice, pData->Thost.VolumeChange, pData->Thost.UserID, pData->Thost.InstrumentID);

	return ret;
}

///������������
int CFTTraderApi::ReqOrderAction(PlatformStru_InputOrderAction* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqOrderAction(&pData->Thost, nRequestID); 
	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);
	
	LOG_INFO("ReqOrderAction(������������):ret=[%d],nRequestID=[%d]\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
		"\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
		"\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID, pData->Thost.InvestorID, pData->Thost.OrderActionRef, pData->Thost.OrderRef,pData->Thost.RequestID,
		pData->Thost.FrontID, pData->Thost.SessionID, pData->Thost.ExchangeID, pData->Thost.OrderSysID,pData->Thost.ActionFlag,
		pData->Thost.LimitPrice, pData->Thost.VolumeChange, pData->Thost.UserID, pData->Thost.InstrumentID);

	return ret;
}

///��ѯ��󱨵���������
int CFTTraderApi::ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqQueryMaxOrderVolume(&pData->Thost, nRequestID);  
	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQueryMaxOrderVolume(��ѯ��󱨵���������):ret=[%d],nRequestID=[%d]\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t Direction=[%d],\n"
		"\t\t\t HedgeFlag=[%d],\t MaxVolume=[%d],\t OffsetFlag=[%d]",
		ret, nRequestID,
		pData->Thost.BrokerID, pData->Thost.InvestorID, pData->Thost.InstrumentID, pData->Thost.Direction,
		pData->Thost.HedgeFlag, pData->Thost.MaxVolume, pData->Thost.OffsetFlag);

	return ret;
}

///Ͷ���߽�����ȷ��
int CFTTraderApi::ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqSettlementInfoConfirm(pData, nRequestID);
	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqSettlementInfoConfirm(Ͷ���߽�����ȷ��):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ConfirmDate=[%s],\t ConfirmTime=[%s]",
		ret, nRequestID, 
		pData->BrokerID,
		pData->InvestorID,
		pData->ConfirmDate,
		pData->ConfirmTime);

	return ret;
}

///����ɾ��Ԥ��
int CFTTraderApi::ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqRemoveParkedOrder(&pData->Thost, nRequestID);  
	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqRemoveParkedOrder(����ɾ��Ԥ��):ret=[%d],nRequestID=[%d]\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ParkedOrderID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID, pData->Thost.InvestorID, pData->Thost.ParkedOrderID);

	return ret;
}

///����ɾ��Ԥ�񳷵�
int CFTTraderApi::ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqRemoveParkedOrderAction(&pData->Thost, nRequestID);  
	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqRemoveParkedOrderAction(����ɾ��Ԥ�񳷵�):ret=[%d],nRequestID=[%d]\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ParkedOrderActionID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID, 
		pData->Thost.InvestorID, 
		pData->Thost.ParkedOrderActionID);

	return ret;
}

///ѯ��¼������
int CFTTraderApi::ReqForQuoteInsert(PlatformStru_InputForQuoteField* pData, int nRequestID)
{
	int ret = -999;	

    CThostFtdcInputForQuoteField thost;
    pData->ToThost(thost);

    ret = m_pTradeApi->ReqForQuoteInsert(&thost, nRequestID);  
	if(ret==0)
 		_GetForwarding().SendPackageForApi(&thost, sizeof(thost), nRequestID);
    LOG_INFO("ReqForQuoteInsert(����ѯ��¼��):ret=[%d],nRequestID=[%d]\n%s",
             ret, nRequestID,pData->tostring().c_str());

	return ret;
}

///ִ������¼������
int CFTTraderApi::ReqExecOrderInsert(PlatformStru_InputExecOrderField* pData, int nRequestID)
{
	int ret = -999;

	CThostFtdcInputExecOrderField thost;
	pData->ToThost(thost);

	ret = m_pTradeApi->ReqExecOrderInsert(&thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&thost, sizeof(thost), nRequestID);

	LOG_INFO("ReqExecOrderInsert(ִ������¼������):ret=[%d],nRequestID=[%d]\n%s",
		ret, nRequestID,pData->tostring().c_str());

	return ret;
}

///ִ�������������
int CFTTraderApi::ReqExecOrderAction(PlatformStru_InputExecOrderActionField* pData, int nRequestID)
{
	int ret = -999;

	CThostFtdcInputExecOrderActionField thost;
	pData->ToThost(thost);

	ret = m_pTradeApi->ReqExecOrderAction(&thost, nRequestID);  
	if(ret==0)
	 	_GetForwarding().SendPackageForApi(&thost, sizeof(thost), nRequestID);
	
	LOG_INFO("ReqExecOrderAction(ִ�������������):ret=[%d],nRequestID=[%d]\n%s",
		ret, nRequestID,pData->tostring().c_str());

	return ret;
}

///�����ѯ����
int CFTTraderApi::ReqQryOrder(PlatformStru_QryOrder* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqQryOrder(&pData->Thost, nRequestID);
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQryOrder(�����ѯ����):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s],\t OrderSysID=[%s],\n"
		"\t\t\t InsertTimeStart=[%s],\t InsertTimeEnd=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.InvestorID,
		pData->Thost.InstrumentID,
		pData->Thost.ExchangeID,
		pData->Thost.OrderSysID,
		pData->Thost.InsertTimeStart,
		pData->Thost.InsertTimeEnd);

	return ret;
}

///�����ѯ�ɽ�
int CFTTraderApi::ReqQryTrade(PlatformStru_QryTrade* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqQryTrade(&pData->Thost, nRequestID);
	if(ret==0)
 		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQryTrade(�����ѯ�ɽ�):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s],\t TradeID=[%s],\n"
		"\t\t\t TradeTimeStart=[%s],\t TradeTimeEnd=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.InvestorID,
		pData->Thost.InstrumentID,
		pData->Thost.ExchangeID,
		pData->Thost.TradeID,
		pData->Thost.TradeTimeStart,
		pData->Thost.TradeTimeEnd);

	return ret;
}

///�����ѯͶ���ֲ߳�
int CFTTraderApi::ReqQryInvestorPosition(PlatformStru_QryInvestorPosition* pData, int nRequestID)
{
	int ret = -999;

	CThostFtdcQryInvestorPositionField field;
	memset( &field, 0, sizeof(field));
	strncpy(field.BrokerID, pData->BrokerID,sizeof(field.BrokerID)-1);
	strncpy(field.InstrumentID, pData->InstrumentID,sizeof(field.InstrumentID)-1);
	strncpy(field.InvestorID, pData->InvestorID,sizeof(field.InvestorID)-1);

	ret = m_pTradeApi->ReqQryInvestorPosition(&field, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&field, sizeof(field), nRequestID);

	LOG_INFO("ReqQryInvestorPosition(�����ѯͶ���ֲ߳�):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID,
		pData->InstrumentID);

	return ret;
}

///�����ѯ�ʽ��˻�
int CFTTraderApi::ReqQryTradingAccount(PlatformStru_QryTradingAccount* pData, int nRequestID)
{
	int ret = -999;

	CThostFtdcQryTradingAccountField QryTradingAccount;
	memset(&QryTradingAccount,0,sizeof(QryTradingAccount));
	strncpy(QryTradingAccount.BrokerID,pData->BrokerID,sizeof(QryTradingAccount.BrokerID)-1);
	strncpy(QryTradingAccount.InvestorID,pData->InvestorID,sizeof(QryTradingAccount.InvestorID)-1);

	ret = m_pTradeApi->ReqQryTradingAccount(&QryTradingAccount, nRequestID); 
	if(ret==0)
 		_GetForwarding().SendPackageForApi(&QryTradingAccount, sizeof(QryTradingAccount), nRequestID);

	LOG_INFO("ReqQryTradingAccount(�����ѯ�ʽ��˻�):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID);

	return ret;
}

///�����ѯͶ����
int CFTTraderApi::ReqQryInvestor(PlatformStru_QryInvestor* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryInvestor(&pData->Thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQryInvestor(�����ѯͶ����):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.InvestorID);

	return ret;
}

///�����ѯ��Լ��֤����
int CFTTraderApi::ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqQryInstrumentMarginRate(pData, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqQryInstrumentMarginRate(�����ѯ��Լ��֤����):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t HedgeFlag=[%d]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID,
		pData->InstrumentID,
		pData->HedgeFlag);

	return ret;
}

///�����ѯ��Լ��������
int CFTTraderApi::ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqQryInstrumentCommissionRate(pData, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqQryInstrumentCommissionRate(�����ѯ��Լ��������):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID,
		pData->InstrumentID);

	return ret;
}

///�����ѯ������
int CFTTraderApi::ReqQryExchange(CThostFtdcQryExchangeField* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqQryExchange(pData, nRequestID);
 	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqQryExchange(�����ѯ������):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t ExchangeID=[%s]",
		ret, nRequestID,
		pData->ExchangeID);

	return ret;
}

///�����ѯ��Ʒ
int CFTTraderApi::ReqQryProduct(PlatformStru_QryProduct* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryProduct(&pData->Thost, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQryProduct(�����ѯƷ��):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t ProductID=[%s],\t ProductClass=[%d]",
		ret, nRequestID, 
		pData->Thost.ProductID,
		pData->Thost.ProductClass);

	return ret;
}

///�����ѯ��Լ
int CFTTraderApi::ReqQryInstrument(PlatformStru_QryInstrument* pData, int nRequestID)
{
	int ret = -999;
	
	ret = m_pTradeApi->ReqQryInstrument(&pData->Thost, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQryInstrument(�����ѯ��Լ):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t InstrumentID=[%s],\t ExchangeID=[%s],\t ExchangeInstID=[%s],\tProductID=[%s]",
		ret, nRequestID, 
		pData->Thost.InstrumentID,
		pData->Thost.ExchangeID,
		pData->Thost.ExchangeInstID,
		pData->Thost.ProductID);

	return ret;
}

///�����ѯͶ���߽�����
int CFTTraderApi::ReqQrySettlementInfo(PlatformStru_QrySettlementInfo* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQrySettlementInfo(&pData->Thost, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQrySettlementInfo(�����ѯͶ���߽�����):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t TradingDay=[%s]",
		ret, nRequestID, 
		pData->Thost.BrokerID,
		pData->Thost.InvestorID,
		pData->Thost.TradingDay);
	
	return ret;
}

///�����ѯͶ���ֲ߳���ϸ
int CFTTraderApi::ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail* pData, int nRequestID)
{
	int ret = -999;
	

	CThostFtdcQryInvestorPositionDetailField QryInvestorPositionDetail;
	memset(&QryInvestorPositionDetail,0,sizeof(QryInvestorPositionDetail));
	strncpy(QryInvestorPositionDetail.BrokerID,pData->BrokerID,sizeof(QryInvestorPositionDetail.BrokerID)-1);
	strncpy(QryInvestorPositionDetail.InvestorID,pData->InvestorID,sizeof(QryInvestorPositionDetail.InvestorID)-1);
	strncpy(QryInvestorPositionDetail.InstrumentID,pData->InstrumentID,sizeof(QryInvestorPositionDetail.InstrumentID)-1);

	ret = m_pTradeApi->ReqQryInvestorPositionDetail(&QryInvestorPositionDetail, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&QryInvestorPositionDetail, sizeof(QryInvestorPositionDetail), nRequestID);

	LOG_INFO("ReqQryInvestorPositionDetail(�����ѯͶ���ֲ߳���ϸ):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID,
		pData->InstrumentID);

	return ret;
}

///�����ѯ�ͻ�֪ͨ
int CFTTraderApi::ReqQryNotice(PlatformStru_QryNotice* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryNotice(&pData->Thost, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQryNotice(�����ѯ�ͻ�֪ͨ):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID);

	return ret;
}

///�����ѯ������Ϣȷ��
int CFTTraderApi::ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQrySettlementInfoConfirm(&pData->Thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQrySettlementInfoConfirm(�����ѯ������Ϣȷ��):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.InvestorID);

	return ret;
}

///�����ѯͶ���ֲ߳���ϸ
int CFTTraderApi::ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail* pData, int nRequestID)
{
	int ret = -999;	

	CThostFtdcQryInvestorPositionCombineDetailField QryInvestorPositionCombineDetail;
	memset(&QryInvestorPositionCombineDetail,0,sizeof(QryInvestorPositionCombineDetail));
	strncpy(QryInvestorPositionCombineDetail.BrokerID,pData->BrokerID,sizeof(QryInvestorPositionCombineDetail.BrokerID)-1);
	strncpy(QryInvestorPositionCombineDetail.InvestorID,pData->InvestorID,sizeof(QryInvestorPositionCombineDetail.InvestorID)-1);
	strncpy(QryInvestorPositionCombineDetail.CombInstrumentID,pData->CombInstrumentID,sizeof(QryInvestorPositionCombineDetail.CombInstrumentID)-1);

	ret = m_pTradeApi->ReqQryInvestorPositionCombineDetail(&QryInvestorPositionCombineDetail, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&QryInvestorPositionCombineDetail, sizeof(QryInvestorPositionCombineDetail), nRequestID);

	LOG_INFO("ReqQryInvestorPositionCombineDetail(�����ѯͶ������ϳֲ���ϸ):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t CombInstrumentID=[%s]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID,
		pData->CombInstrumentID);
	
	return ret;
}

///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
int CFTTraderApi::ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryCFMMCTradingAccountKey(pData, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);
	
	LOG_INFO("ReqQryCFMMCTradingAccountKey(�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
		ret, nRequestID, 
		pData->BrokerID,
		pData->InvestorID);

	return ret;
}

///�����ѯ��Ȩ���׳ɱ�
int CFTTraderApi::ReqQryOptionInstrTradeCost(CThostFtdcQryOptionInstrTradeCostField* pData, int nRequestID)
{
	int ret = -999;
	
	ret = m_pTradeApi->ReqQryOptionInstrTradeCost(pData, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqQryOptionInstrTradeCost(�����ѯ��Ȩ���׳ɱ�):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t HedgeFlag=[%d],\t InputPrice=[%g],\t UnderlyingPrice=[%g]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID,
		pData->InstrumentID,
		pData->HedgeFlag,
		pData->InputPrice,
		pData->UnderlyingPrice
		);

	return ret;
}

///�����ѯ��Ȩ��Լ������
int CFTTraderApi::ReqQryOptionInstrCommRate(CThostFtdcQryOptionInstrCommRateField* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryOptionInstrCommRate(pData, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqQryOptionInstrCommRate(�����ѯ��Ȩ��Լ��������):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID,
		pData->InstrumentID);
	
	return ret;
}

///�����ѯִ������
int CFTTraderApi::ReqQryExecOrder(PlatformStru_QryExecOrderField* pData, int nRequestID)
{
	int ret = -999;

	CThostFtdcQryExecOrderField thost;
	pData->ToThost(thost);

	ret = m_pTradeApi->ReqQryExecOrder(&thost, nRequestID);
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&thost, sizeof(thost), nRequestID);

	LOG_INFO("ReqQryExecOrder(�����ѯִ������):ret=[%d],nRequestID=[%d]\n%s",
		ret, nRequestID,pData->tostring().c_str());

	return ret;
}

///�����ѯת����ˮ
int CFTTraderApi::ReqQryTransferSerial(PlatformStru_QryTransferSerial* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryTransferSerial(&pData->Thost, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQryTransferSerial(�����ѯת����ˮ) : ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t AccountID=[%s],\t BankID=[%s]",
		ret,nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.AccountID,
		pData->Thost.BankID);

	return ret;
}

///�����ѯ����ǩԼ��ϵ
int CFTTraderApi::ReqQryAccountregister(PlatformStru_QryAccountRegister* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryAccountregister(&pData->Thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQryAccountregister(�����ѯ����ǩԼ��ϵ) : ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t AccountID=[%s],\t BankID=[%s]",
		ret,nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.AccountID,
		pData->Thost.BankID);
	
	return ret;
}

///�����ѯǩԼ����
int CFTTraderApi::ReqQryContractBank(PlatformStru_QryContractBank* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryContractBank(&pData->Thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);
	
	LOG_INFO("ReqQryContractBank(�����ѯǩԼ����) : ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t BankID=[%s],\t BankBrchID=[%s]",
		ret,nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.BankID,
		pData->Thost.BankBrchID);
	
	return ret;
}

///�����ѯԤ��
int CFTTraderApi::ReqQryParkedOrder(PlatformStru_QryParkedOrder* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryParkedOrder(&pData->Thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);
	
	LOG_INFO("ReqQryParkedOrder(�����ѯԤ��):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.InvestorID,
		pData->Thost.InstrumentID,
		pData->Thost.ExchangeID);
	
	return ret;
}

///�����ѯԤ�񳷵�
int CFTTraderApi::ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryParkedOrderAction(&pData->Thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);
	
	LOG_INFO("ReqQryParkedOrderAction(�����ѯԤ�񳷵�):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s]",
		ret, nRequestID,
		pData->Thost.BrokerID,
		pData->Thost.InvestorID,
		pData->Thost.InstrumentID,
		pData->Thost.ExchangeID);
	
	return ret;
}

///�����ѯ����֪ͨ
int CFTTraderApi::ReqQryTradingNotice(CThostFtdcQryTradingNoticeField* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryTradingNotice(pData, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqQryTradingNotice(�����ѯ����֪ͨ):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
		ret, nRequestID,
		pData->BrokerID,
		pData->InvestorID);

	return ret;
}

///�����ѯ���͹�˾���ײ���
int CFTTraderApi::ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqQryBrokerTradingParams(pData, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(pData, sizeof(*pData), nRequestID);

	LOG_INFO("ReqQryBrokerTradingParams(�����ѯ���͹�˾���ײ���):ret=[%d],nRequestID=[%d],\n"
		"\t\t\t CurrencyID=[%s],\t BrokerID=[%s],\t InvestorID=[%s]",
		ret, nRequestID,
		pData->CurrencyID,
		pData->BrokerID,
		pData->InvestorID);

	return ret;
}

///�ڻ����������ʽ�ת�ڻ�����
int CFTTraderApi::ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer* pData, int nRequestID)
{
	int ret = -999;
	
	ret = m_pTradeApi->ReqFromBankToFutureByFuture(&pData->Thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqFromBankToFutureByFuture(�ڻ����������ʽ�ת�ڻ�����) : ret=[%d],nRequestID=[%d],\n"
	                    "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
	                    "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
	                    "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
	                    "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[******],\n"
	                    "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
	                    "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
	                    "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
	                    "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
	                    "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d]",
	                    ret,nRequestID,
	                    pData->Thost.TradeCode, pData->Thost.BankID,pData->Thost.BankBranchID,pData->Thost.BrokerID,pData->Thost.BrokerBranchID,
	                    pData->Thost.TradeDate, pData->Thost.TradeTime,pData->Thost.BankSerial,pData->Thost.TradingDay,pData->Thost.PlateSerial,
	                    pData->Thost.LastFragment, pData->Thost.SessionID,pData->Thost.CustomerName,pData->Thost.IdCardType,pData->Thost.IdentifiedCardNo,
	                    pData->Thost.CustType, pData->Thost.BankAccount,pData->Thost.BankPassWord,pData->Thost.AccountID,/*pData->Thost.Password,*/
	                    pData->Thost.InstallID,pData->Thost.FutureSerial, pData->Thost.UserID,pData->Thost.VerifyCertNoFlag,pData->Thost.CurrencyID,
	                    pData->Thost.TradeAmount,pData->Thost.FutureFetchAmount, pData->Thost.FeePayFlag,pData->Thost.CustFee,pData->Thost.BrokerFee,
	                    pData->Thost.Message, pData->Thost.Digest, pData->Thost.BankAccType,pData->Thost.DeviceID,pData->Thost.BankSecuAccType,
	                    pData->Thost.BrokerIDByBank, pData->Thost.BankSecuAcc, pData->Thost.BankPwdFlag,pData->Thost.SecuPwdFlag,pData->Thost.OperNo,
	                    pData->Thost.RequestID, pData->Thost.TID, pData->Thost.TransferStatus
	                    );

	return ret;
}

///�ڻ������ڻ��ʽ�ת��������
int CFTTraderApi::ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer* pData, int nRequestID)
{
	int ret = -999;	

	ret = m_pTradeApi->ReqFromFutureToBankByFuture(&pData->Thost, nRequestID);  
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqFromFutureToBankByFuture(�ڻ������ڻ��ʽ�ת��������) : ret=[%d],nRequestID=[%d],\n"
	                    "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
	                    "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
	                    "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
	                    "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[******],\n"
	                    "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
	                    "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
	                    "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
	                    "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
	                    "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d]",
	                    ret,nRequestID,
	                    pData->Thost.TradeCode, pData->Thost.BankID,pData->Thost.BankBranchID,pData->Thost.BrokerID,pData->Thost.BrokerBranchID,
	                    pData->Thost.TradeDate, pData->Thost.TradeTime,pData->Thost.BankSerial,pData->Thost.TradingDay,pData->Thost.PlateSerial,
	                    pData->Thost.LastFragment, pData->Thost.SessionID,pData->Thost.CustomerName,pData->Thost.IdCardType,pData->Thost.IdentifiedCardNo,
	                    pData->Thost.CustType, pData->Thost.BankAccount,pData->Thost.BankPassWord,pData->Thost.AccountID,/*pData->Thost.Password,*/
	                    pData->Thost.InstallID,pData->Thost.FutureSerial, pData->Thost.UserID,pData->Thost.VerifyCertNoFlag,pData->Thost.CurrencyID,
	                    pData->Thost.TradeAmount,pData->Thost.FutureFetchAmount, pData->Thost.FeePayFlag,pData->Thost.CustFee,pData->Thost.BrokerFee,
	                    pData->Thost.Message, pData->Thost.Digest, pData->Thost.BankAccType,pData->Thost.DeviceID,pData->Thost.BankSecuAccType,
	                    pData->Thost.BrokerIDByBank, pData->Thost.BankSecuAcc, pData->Thost.BankPwdFlag,pData->Thost.SecuPwdFlag,pData->Thost.OperNo,
	                    pData->Thost.RequestID, pData->Thost.TID, pData->Thost.TransferStatus
	                    );

	return ret;
}

///�ڻ������ѯ�����������
int CFTTraderApi::ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount* pData, int nRequestID)
{
	int ret = -999;

	ret = m_pTradeApi->ReqQueryBankAccountMoneyByFuture(&pData->Thost, nRequestID); 
 	if(ret==0)
		_GetForwarding().SendPackageForApi(&pData->Thost, sizeof(pData->Thost), nRequestID);

	LOG_INFO("ReqQueryBankAccountMoneyByFuture(�ڻ������ѯ�����������) : ret=[%d],nRequestID=[%d],\n"
		"\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
		"\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
		"\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
		"\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[******],\t AccountID=[%s],\t Password=[******],\n"
		"\t\t\t FutureSerial=[%d],\t InstallID=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
		"\t\t\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],BrokerIDByBank=[%s],\n"
		"\t\t\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],RequestID=[%d],\n"
		"\t\t\t TID=[%d]",
		ret,nRequestID,
		pData->Thost.TradeCode, pData->Thost.BankID,pData->Thost.BankBranchID,pData->Thost.BrokerID,pData->Thost.BrokerBranchID,
		pData->Thost.TradeDate, pData->Thost.TradeTime,pData->Thost.BankSerial,pData->Thost.TradingDay,pData->Thost.PlateSerial,
		pData->Thost.LastFragment, pData->Thost.SessionID,pData->Thost.CustomerName,pData->Thost.IdCardType,pData->Thost.IdentifiedCardNo,
		pData->Thost.CustType, pData->Thost.BankAccount,pData->Thost.AccountID,
		pData->Thost.FutureSerial, pData->Thost.InstallID,pData->Thost.UserID,pData->Thost.VerifyCertNoFlag,pData->Thost.CurrencyID,
		pData->Thost.Digest, pData->Thost.BankAccType,pData->Thost.DeviceID,pData->Thost.BankSecuAccType,pData->Thost.BrokerIDByBank,
		pData->Thost.BankSecuAcc, pData->Thost.BankPwdFlag,pData->Thost.SecuPwdFlag,pData->Thost.OperNo,pData->Thost.RequestID,
		pData->Thost.TID);		

	return ret;
}
