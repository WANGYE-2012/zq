
#pragma once

//#include "Module-Misc2\KernelStruct.h"

//��CTP��Api����������
enum {
	FFT_API_START = 0x18000000, 

	///�û���¼����
	FTTCMD_ReqUserLogin,			//����������
	FTTCMD_ReqUserLogin_Rsp,		//��Ӧ������

	///�ǳ�����
	FTTCMD_ReqUserLogout, 
	FTTCMD_ReqUserLogout_Rsp, 

	///�û������������
	FTTCMD_ReqUserPasswordUpdate, 
	FTTCMD_ReqUserPasswordUpdate_Rsp,

	///�ʽ��˻������������
	FTTCMD_ReqTradingAccountPasswordUpdate,
	FTTCMD_ReqTradingAccountPasswordUpdate_Rsp,

	///����¼������
	FTTCMD_ReqOrderInsert, 
	FTTCMD_ReqOrderInsert_Rsp, 

	///Ԥ��¼������
	FTTCMD_ReqParkedOrderInsert, 
	FTTCMD_ReqParkedOrderInsert_Rsp, 

	///Ԥ�񳷵�¼������
	FTTCMD_ReqParkedOrderAction, 
	FTTCMD_ReqParkedOrderAction_Rsp, 

	///������������
	FTTCMD_ReqOrderAction,
	FTTCMD_ReqOrderAction_Rsp,

	///��ѯ��󱨵���������
	FTTCMD_ReqQueryMaxOrderVolume,
	FTTCMD_ReqQueryMaxOrderVolume_Rsp,

	///Ͷ���߽�����ȷ��
	FTTCMD_ReqSettlementInfoConfirm,
	FTTCMD_ReqSettlementInfoConfirm_Rsp,

	///����ɾ��Ԥ��
	FTTCMD_ReqRemoveParkedOrder,
	FTTCMD_ReqRemoveParkedOrder_Rsp,

	///����ɾ��Ԥ�񳷵�
	FTTCMD_ReqRemoveParkedOrderAction,
	FTTCMD_ReqRemoveParkedOrderAction_Rsp,

	///ѯ��¼������
	FTTCMD_ReqForQuoteInsert,
	FTTCMD_ReqForQuoteInsert_Rsp,

	///ִ������¼������
	FTTCMD_ReqExecOrderInsert,
	FTTCMD_ReqExecOrderInsert_Rsp,

	///ִ�������������
	FTTCMD_ReqExecOrderAction,
	FTTCMD_ReqExecOrderAction_Rsp,

	///�����ѯ����
	FTTCMD_ReqQryOrder,
	FTTCMD_ReqQryOrder_Rsp,

	///�����ѯ�ɽ�
	FTTCMD_ReqQryTrade,
	FTTCMD_ReqQryTrade_Rsp,

	///�����ѯͶ���ֲ߳�
	FTTCMD_ReqQryInvestorPosition,
	FTTCMD_ReqQryInvestorPosition_Rsp,

	///�����ѯ�ʽ��˻�
	FTTCMD_ReqQryTradingAccount,
	FTTCMD_ReqQryTradingAccount_Rsp,

	///�����ѯͶ����
	FTTCMD_ReqQryInvestor,
	FTTCMD_ReqQryInvestor_Rsp,

	///�����ѯ��Լ��֤����
	FTTCMD_ReqQryInstrumentMarginRate,
	FTTCMD_ReqQryInstrumentMarginRate_Rsp,

	///�����ѯ��Լ��������
	FTTCMD_ReqQryInstrumentCommissionRate,
	FTTCMD_ReqQryInstrumentCommissionRate_Rsp,

	///�����ѯ������
	FTTCMD_ReqQryExchange,
	FTTCMD_ReqQryExchange_Rsp,

	///�����ѯ��Ʒ
	FTTCMD_ReqQryProduct,
	FTTCMD_ReqQryProduct_Rsp,

	///�����ѯ��Լ
	FTTCMD_ReqQryInstrument,
	FTTCMD_ReqQryInstrument_Rsp,

	///�����ѯͶ���߽�����
	FTTCMD_ReqQrySettlementInfo,
	FTTCMD_ReqQrySettlementInfo_Rsp,

	///�����ѯͶ���ֲ߳���ϸ
	FTTCMD_ReqQryInvestorPositionDetail,
	FTTCMD_ReqQryInvestorPositionDetail_Rsp,

	///�����ѯ�ͻ�֪ͨ
	FTTCMD_ReqQryNotice,
	FTTCMD_ReqQryNotice_Rsp,

	///�����ѯ������Ϣȷ��
	FTTCMD_ReqQrySettlementInfoConfirm,
	FTTCMD_ReqQrySettlementInfoConfirm_Rsp,

	///�����ѯͶ���ֲ߳���ϸ
	FTTCMD_ReqQryInvestorPositionCombineDetail,
	FTTCMD_ReqQryInvestorPositionCombineDetail_Rsp,

	///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
	FTTCMD_ReqQryCFMMCTradingAccountKey,
	FTTCMD_ReqQryCFMMCTradingAccountKey_Rsp,

	///�����ѯ��Ȩ���׳ɱ�
	FTTCMD_ReqQryOptionInstrTradeCost,
	FTTCMD_ReqQryOptionInstrTradeCost_Rsp,

	///�����ѯ��Ȩ��Լ������
	FTTCMD_ReqQryOptionInstrCommRate,
	FTTCMD_ReqQryOptionInstrCommRate_Rsp,

	///�����ѯִ������
	FTTCMD_ReqQryExecOrder,
	FTTCMD_ReqQryExecOrder_Rsp,

	///�����ѯת����ˮ
	FTTCMD_ReqQryTransferSerial,
	FTTCMD_ReqQryTransferSerial_Rsp,

	///�����ѯ����ǩԼ��ϵ
	FTTCMD_ReqQryAccountregister,
	FTTCMD_ReqQryAccountregister_Rsp,

	///�����ѯǩԼ����
	FTTCMD_ReqQryContractBank,
	FTTCMD_ReqQryContractBank_Rsp,

	///�����ѯԤ��
	FTTCMD_ReqQryParkedOrder,
	FTTCMD_ReqQryParkedOrder_Rsp,

	///�����ѯԤ�񳷵�
	FTTCMD_ReqQryParkedOrderAction,
	FTTCMD_ReqQryParkedOrderAction_Rsp,

	///�����ѯ����֪ͨ
	FTTCMD_ReqQryTradingNotice,
	FTTCMD_ReqQryTradingNotice_Rsp,

	///�����ѯ���͹�˾���ײ���
	FTTCMD_ReqQryBrokerTradingParams,
	FTTCMD_ReqQryBrokerTradingParams_Rsp,

	///�ڻ����������ʽ�ת�ڻ�����
	FTTCMD_ReqFromBankToFutureByFuture,
	FTTCMD_ReqFromBankToFutureByFuture_Rsp,

	///�ڻ������ڻ��ʽ�ת��������
	FTTCMD_ReqFromFutureToBankByFuture,
	FTTCMD_ReqFromFutureToBankByFuture_Rsp,

	///�ڻ������ѯ�����������
	FTTCMD_ReqQueryBankAccountMoneyByFuture,
	FTTCMD_ReqQueryBankAccountMoneyByFuture_Rsp,
};

//��CTP��Spi����������
enum {
	FFT_SPI_START = 0x18010000, 

	// ���׷��������ӳɹ���Ӧ
	FTTCMD_OnFrontConnected_RON,

	// ���׷��������ӶϿ���Ӧ
	FTTCMD_OnFrontDisconnected_RON,

	///��¼������Ӧ
	FTTCMD_OnRspUserLogin_RON,

	///�ǳ�������Ӧ
	FTTCMD_OnRspUserLogout_RON,

	///�û��������������Ӧ
	FTTCMD_OnRspUserPasswordUpdate_RON,

	///�ʽ��˻��������������Ӧ
	FTTCMD_OnRspTradingAccountPasswordUpdate_RON,

	///����¼��������Ӧ
	FTTCMD_OnRspOrderInsert_RON,

	///Ԥ��¼��������Ӧ
	FTTCMD_OnRspParkedOrderInsert_RON,

	///Ԥ�񳷵�¼��������Ӧ
	FTTCMD_OnRspParkedOrderAction_RON,

	///��������������Ӧ
	FTTCMD_OnRspOrderAction_RON,

	///��ѯ��󱨵�������Ӧ
	FTTCMD_OnRspQueryMaxOrderVolume_RON,

	///Ͷ���߽�����ȷ����Ӧ
	FTTCMD_OnRspSettlementInfoConfirm_RON,

	///ɾ��Ԥ����Ӧ
	FTTCMD_OnRspRemoveParkedOrder_RON,

	///ɾ��Ԥ�񳷵���Ӧ
	FTTCMD_OnRspRemoveParkedOrderAction_RON,

	///ִ������¼��������Ӧ
	FTTCMD_OnRspExecOrderInsert_RON,

	///ִ���������������Ӧ
	FTTCMD_OnRspExecOrderAction_RON,

	///�����ѯ������Ӧ
	FTTCMD_OnRspQryOrder_RON,

	///�����ѯ�ɽ���Ӧ
	FTTCMD_OnRspQryTrade_RON,

	///�����ѯͶ���ֲ߳���Ӧ
	FTTCMD_OnRspQryInvestorPosition_RON,

	///�����ѯ�ʽ��˻���Ӧ
	FTTCMD_OnRspQryTradingAccount_RON,

	///�����ѯͶ������Ӧ
	FTTCMD_OnRspQryInvestor_RON,

	///�����ѯ��Լ��֤������Ӧ
	FTTCMD_OnRspQryInstrumentMarginRate_RON,

	///�����ѯ��Լ����������Ӧ
	FTTCMD_OnRspQryInstrumentCommissionRate_RON,

	///�����ѯ��������Ӧ
	FTTCMD_OnRspQryExchange_RON,

	///�����ѯ��Ʒ��Ӧ
	FTTCMD_OnRspQryProduct_RON,

	///�����ѯ��Լ��Ӧ
	FTTCMD_OnRspQryInstrument_RON,

	///�����ѯͶ���߽�������Ӧ
	FTTCMD_OnRspQrySettlementInfo_RON,

	///�����ѯͶ���ֲ߳���ϸ��Ӧ
	FTTCMD_OnRspQryInvestorPositionDetail_RON,

	///�����ѯ�ͻ�֪ͨ��Ӧ
	FTTCMD_OnRspQryNotice_RON,

	///�����ѯ������Ϣȷ����Ӧ
	FTTCMD_OnRspQrySettlementInfoConfirm_RON,

	///�����ѯͶ���ֲ߳���ϸ��Ӧ
	FTTCMD_OnRspQryInvestorPositionCombineDetail_RON,

	///��ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ��Ӧ
	FTTCMD_OnRspQryCFMMCTradingAccountKey_RON,

	///�����ѯ��Ȩ���׳ɱ���Ӧ
	FTTCMD_OnRspQryOptionInstrTradeCost_RON,

	///�����ѯ��Ȩ��Լ��������Ӧ
	FTTCMD_OnRspQryOptionInstrCommRate_RON,

	///�����ѯִ��������Ӧ
	FTTCMD_OnRspQryExecOrder_RON,

	///�����ѯת����ˮ��Ӧ
	FTTCMD_OnRspQryTransferSerial_RON,

	///�����ѯ����ǩԼ��ϵ��Ӧ
	FTTCMD_OnRspQryAccountregister_RON,

	///����Ӧ��
	FTTCMD_OnRspError_RON,

	///����֪ͨ
	FTTCMD_OnRtnOrder_RON,

	///�ɽ�֪ͨ
	FTTCMD_OnRtnTrade_RON,

	///����¼�����ر�
	FTTCMD_OnErrRtnOrderInsert_RON,

	///������������ر�
	FTTCMD_OnErrRtnOrderAction_RON,

	///��Լ����״̬֪ͨ
	FTTCMD_OnRtnInstrumentStatus_RON,

	///����֪ͨ
	FTTCMD_OnRtnTradingNotice_RON,

	///ִ������֪ͨ
	FTTCMD_OnRtnExecOrder_RON,

	///ִ������¼�����ر�
	FTTCMD_OnErrRtnExecOrderInsert_RON,

	///ִ�������������ر�
	FTTCMD_OnErrRtnExecOrderAction_RON,

	///ѯ��¼��������Ӧ
	FTTCMD_OnRspForQuoteInsert_RON,

	///ѯ��¼�����ر�
	FTTCMD_OnErrRtnForQuoteInsert_RON,

	///�����ѯǩԼ������Ӧ
	FTTCMD_OnRspQryContractBank_RON,

	///�����ѯԤ����Ӧ
	FTTCMD_OnRspQryParkedOrder_RON,

	///�����ѯԤ�񳷵���Ӧ
	FTTCMD_OnRspQryParkedOrderAction_RON,

	///�����ѯ����֪ͨ��Ӧ
	FTTCMD_OnRspQryTradingNotice_RON,

	///�����ѯ���͹�˾���ײ�����Ӧ
	FTTCMD_OnRspQryBrokerTradingParams_RON,

	///�ڻ����������ʽ�ת�ڻ�֪ͨ
	FTTCMD_OnRtnFromBankToFutureByFuture_RON,

	///�ڻ������ڻ��ʽ�ת����֪ͨ
	FTTCMD_OnRtnFromFutureToBankByFuture_RON,

	///�ڻ������ѯ�������֪ͨ
	FTTCMD_OnRtnQueryBankBalanceByFuture_RON,

	///�ڻ����������ʽ�ת�ڻ�Ӧ��
	FTTCMD_OnRspFromBankToFutureByFuture_RON,

	///�ڻ������ڻ��ʽ�ת����Ӧ��
	FTTCMD_OnRspFromFutureToBankByFuture_RON,

	///�ڻ������ѯ�������Ӧ��
	FTTCMD_OnRspQueryBankAccountMoneyByFuture_RON,

};
