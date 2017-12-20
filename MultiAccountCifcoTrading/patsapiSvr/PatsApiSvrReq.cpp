#include "StdAfx.h"

#include "PatsApiSvr.h"
#include "PlatFormService.hpp"
#include "Module-Misc/globalDefines.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
namespace Pats
{


#define CHECK_LOGIN_STATUS()    \
	if(!m_isLogin)  return -999

	const static string __UNKOWN__ = "δ֪";
#define LOG_INFO(fmt, ...) \
	do{\
	if(m_pWriteLog)\
	{\
	m_pWriteLog->WriteLog_Fmt("CPatsApiSvr", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
	}\
	}while(0)


	/*--------------------------------------------------------------------------------------------------------
	����ҵ���������첽�ģ��趨�ƾ���ҵ�����ݣ������յ�������
	---------------------------------------------------------------------------------------------------------*/
	///�û������������
	int CPatsApiSvr::ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate &UserPasswordUpdate, int nRequestID)
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqUserPasswordUpdate(&UserPasswordUpdate, nRequestID);
	}

	///�ʽ��˻������������
	int CPatsApiSvr::ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate &TradingAccountPasswordUpdate, int nRequestID)
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqTradingAccountPasswordUpdate(&TradingAccountPasswordUpdate, nRequestID);
	}

	///����¼������
	int CPatsApiSvr::ReqOrderInsert(PlatformStru_InputOrder &InputOrder, int nRequestID)
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqOrderInsert(&InputOrder, nRequestID);
	}

	///Ԥ��¼������
	int CPatsApiSvr::ReqParkedOrderInsert(PlatformStru_ParkedOrder &ParkedOrder, int nRequestID)
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqParkedOrderInsert(&ParkedOrder, nRequestID);
	}

	///Ԥ�񳷵�¼������
	int CPatsApiSvr::ReqParkedOrderAction(PlatformStru_ParkedOrderAction &ParkedOrderAction, int nRequestID)
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqParkedOrderAction(&ParkedOrderAction, nRequestID);
	}

	///������������
	int CPatsApiSvr::ReqOrderAction(PlatformStru_InputOrderAction &InputOrderAction, int nRequestID)
	{
		CHECK_LOGIN_STATUS();


		return m_PlatFormService->ReqOrderAction(&InputOrderAction, nRequestID);
	}

	///��ѯ��󱨵���������
	int CPatsApiSvr::ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume &QueryMaxOrderVolume, int nRequestID)
	{
		CHECK_LOGIN_STATUS();


		return m_PlatFormService->ReqQueryMaxOrderVolume(&QueryMaxOrderVolume, nRequestID);
	}

	///Ͷ���߽�����ȷ��
	int CPatsApiSvr::ReqSettlementInfoConfirm() 
	{
		CHECK_LOGIN_STATUS();


		return m_PlatFormService->ReqSettlementInfoConfirm();
	}

	///����ɾ��Ԥ��
	int CPatsApiSvr::ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder &RemoveParkedOrder, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();


		return m_PlatFormService->ReqRemoveParkedOrder(&RemoveParkedOrder, nRequestID);
	}

	///����ɾ��Ԥ�񳷵�
	int CPatsApiSvr::ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction &RemoveParkedOrderAction, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();


		return m_PlatFormService->ReqRemoveParkedOrderAction(&RemoveParkedOrderAction, nRequestID);
	}

	///�����ѯ����
	int CPatsApiSvr::ReqQryOrder(PlatformStru_QryOrder &QryOrder, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();


		return m_PlatFormService->ReqQryOrder(&QryOrder,nRequestID);
	}

	///�����ѯ�ɽ�
	int CPatsApiSvr::ReqQryTrade(PlatformStru_QryTrade &QryTrade, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQryTrade(&QryTrade,nRequestID);
	}

	///�����ѯͶ���ֲ߳�
	int CPatsApiSvr::ReqQryInvestorPosition(PlatformStru_QryInvestorPosition &QryInvestorPosition, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return  m_PlatFormService->ReqQryInvestorPosition(&QryInvestorPosition,nRequestID);
	}

	///�����ѯ�ʽ��˻�
	int CPatsApiSvr::ReqQryTradingAccount(PlatformStru_QryTradingAccount &QryTradingAccount, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQryTradingAccount(&QryTradingAccount,nRequestID);
	}

	///�����ѯͶ����
	int CPatsApiSvr::ReqQryInvestor(PlatformStru_QryInvestor &QryInvestor, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();


		return m_PlatFormService->ReqQryInvestor(&QryInvestor, nRequestID);
	}


	///�����ѯ��Լ
	int CPatsApiSvr::ReqQryInstrument(PlatformStru_QryInstrument &QryInstrument, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQryInstrument(&QryInstrument,nRequestID);
	}

	///�����ѯ����
	int CPatsApiSvr::ReqQryDepthMarketData(PlatformStru_QryDepthMarketData &QryDepthMarketData, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQryDepthMarketData(&QryDepthMarketData,nRequestID);

	}

	///�����ѯͶ���߽�����
	int CPatsApiSvr::ReqQrySettlementInfo(PlatformStru_QrySettlementInfo &QrySettlementInfo, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQrySettlementInfo(&QrySettlementInfo,nRequestID);
	}

	///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
	int CPatsApiSvr::ReqQryCFMMCTradingAccountKey(PlatformStru_QryCFMMCTradingAccountKey &QryCFMMCTradingAccountKey, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQryCFMMCTradingAccountKey(&QryCFMMCTradingAccountKey,nRequestID);
	}


	///�����ѯת������
	int CPatsApiSvr::ReqQryTransferBank(PlatformStru_QryTransferBank &QryTransferBank, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQryTransferBank(&QryTransferBank,nRequestID);
	}

	///�����ѯͶ���ֲ߳���ϸ
	int CPatsApiSvr::ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail &QryInvestorPositionDetail, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQryInvestorPositionDetail(&QryInvestorPositionDetail,nRequestID);
	}

	///�����ѯ�ͻ�֪ͨ
	int CPatsApiSvr::ReqQryNotice(PlatformStru_QryNotice &QryNotice, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQryNotice(&QryNotice,nRequestID);
	}

	///�����ѯ������Ϣȷ��
	int CPatsApiSvr::ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm &QrySettlementInfoConfirm, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQrySettlementInfoConfirm(&QrySettlementInfoConfirm,nRequestID);
	}

	///�����ѯͶ���ֲ߳���ϸ
	int CPatsApiSvr::ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail &QryInvestorPositionCombineDetail, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQryInvestorPositionCombineDetail(&QryInvestorPositionCombineDetail,nRequestID);
	}

	///�����ѯת����ˮ
	int CPatsApiSvr::ReqQryTransferSerial(PlatformStru_QryTransferSerial &QryTransferSerial, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQryTransferSerial(&QryTransferSerial,nRequestID);
	}

	///�����ѯǩԼ����
	int CPatsApiSvr::ReqQryContractBank(PlatformStru_QryContractBank &QryContractBank, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQryContractBank(&QryContractBank,nRequestID);

	}

	///�����ѯ����ǩԼ��ϵ
	int CPatsApiSvr::ReqQryAccountregister(PlatformStru_QryAccountRegister &pQryAccountregister, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQryAccountregister(&pQryAccountregister,nRequestID);
	}

	///�����ѯԤ��
	int CPatsApiSvr::ReqQryParkedOrder(PlatformStru_QryParkedOrder &QryParkedOrder, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQryParkedOrder(&QryParkedOrder,nRequestID);
	}

	///�����ѯԤ�񳷵�
	int CPatsApiSvr::ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction &QryParkedOrderAction, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQryParkedOrderAction(&QryParkedOrderAction,nRequestID);
	}




	///�ڻ����������ʽ�ת�ڻ�����
	int CPatsApiSvr::ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer &ReqTransfer, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();


		return m_PlatFormService->ReqFromBankToFutureByFuture(&ReqTransfer, nRequestID);
	}

	///�ڻ������ڻ��ʽ�ת��������
	int CPatsApiSvr::ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer &ReqTransfer, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();


		return m_PlatFormService->ReqFromFutureToBankByFuture(&ReqTransfer, nRequestID);
	}

	///�ڻ������ѯ�����������
	int CPatsApiSvr::ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount &ReqQueryAccount, int nRequestID) 
	{
		CHECK_LOGIN_STATUS();

		return m_PlatFormService->ReqQueryBankAccountMoneyByFuture(&ReqQueryAccount,nRequestID);
	}
}