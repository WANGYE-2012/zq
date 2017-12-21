// stdafx.cpp : source file that includes just the standard includes
// MonitorClient.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"



CString Double2String(double dbValue, int nNumDigits, bool bUseMax /*= false*/)
{
	CString str;

	CString strFormat;
	if(bUseMax )
	{
		if(dbValue>1000)
			str.Format(_T("%.0f"), dbValue);
		else 
		{
			int n = 2;
			if(nNumDigits>2)
				n = nNumDigits;
			strFormat.Format(_T("%%.%df"), n);
			str.Format(strFormat, dbValue);	
		}
	}
	else
	{
		strFormat.Format(_T("%%.%df"), nNumDigits);
		str.Format(strFormat, dbValue);	
	}

	return str;
}
bool IsDoubleSame(double db1, double db2)
{
	if(db1 - db2 > -0.00001 && db1 - db2 < 0.00001)
		return true;
	return false;
}
CString Double2String_Tip(double dbValue)
{
	CString str;
	if(dbValue>=10000)
		str.Format(_T("%.0f"), dbValue);
	else if(dbValue>=1000)
		str.Format(_T("%.2f"), dbValue);
	else
	{
		str.Format(_T("%.3f"), dbValue);
	}
	return str;
}
bool CmdID2String(int nCmdID, std::string& str)
{
	bool bFind = true;
	switch (nCmdID)
	{
	case FFT_API_START:
		str = "";
		break;
		///�û���¼����
	case FTTCMD_ReqUserLogin:
		str = "�û���¼����";
		break;
	case FTTCMD_ReqUserLogin_Rsp:
		str = "�û���¼����";
		break;		//��Ӧ������

		///�ǳ�����
	case FTTCMD_ReqUserLogout:
		str = "�ǳ�����";
		break; 
	case FTTCMD_ReqUserLogout_Rsp: 
		str = "�ǳ�����";
		break; 

		///�û������������
	case FTTCMD_ReqUserPasswordUpdate: 
		str = "�û������������";
		break; 
	case FTTCMD_ReqUserPasswordUpdate_Rsp: 
		str = "�û������������";
		break;

		///�ʽ��˻������������
	case FTTCMD_ReqTradingAccountPasswordUpdate: 
		str = "�ʽ��˻������������";
		break;
	case FTTCMD_ReqTradingAccountPasswordUpdate_Rsp:
		str = "�ʽ��˻������������";
		break;

		///����¼������
	case FTTCMD_ReqOrderInsert: 
		str = "����¼������";
		break; 
	case FTTCMD_ReqOrderInsert_Rsp: 
		str = "����¼������";
		break; 

		///Ԥ��¼������
	case FTTCMD_ReqParkedOrderInsert: 
		str = "Ԥ��¼������";
		break; 
	case FTTCMD_ReqParkedOrderInsert_Rsp: 
		str = "Ԥ��¼������";
		break; 

		///Ԥ�񳷵�¼������
	case FTTCMD_ReqParkedOrderAction: 
		str = "Ԥ�񳷵�¼������";
		break; 
	case FTTCMD_ReqParkedOrderAction_Rsp: 
		str = "Ԥ�񳷵�¼������";
		break; 

		///������������
	case FTTCMD_ReqOrderAction: 
		str = "������������";
		break;
	case FTTCMD_ReqOrderAction_Rsp:
		str = "������������";
		break;

		///��ѯ��󱨵���������
	case FTTCMD_ReqQueryMaxOrderVolume: 
		str = "��ѯ��󱨵���������";
		break;
	case FTTCMD_ReqQueryMaxOrderVolume_Rsp: 
		str = "��ѯ��󱨵���������";
		break;

		///Ͷ���߽�����ȷ��
	case FTTCMD_ReqSettlementInfoConfirm: 
		str = "Ͷ���߽�����ȷ��";
		break;
	case FTTCMD_ReqSettlementInfoConfirm_Rsp: 
		str = "Ͷ���߽�����ȷ��";
		break;

		///����ɾ��Ԥ��
	case FTTCMD_ReqRemoveParkedOrder: 
		str = "����ɾ��Ԥ��";
		break;
	case FTTCMD_ReqRemoveParkedOrder_Rsp: 
		str = "����ɾ��Ԥ��";
		break;

		///����ɾ��Ԥ�񳷵�
	case FTTCMD_ReqRemoveParkedOrderAction: 
		str = "����ɾ��Ԥ�񳷵�";
		break;
	case FTTCMD_ReqRemoveParkedOrderAction_Rsp: 
		str = "����ɾ��Ԥ�񳷵�";
		break;

		///ѯ��¼������
	case FTTCMD_ReqForQuoteInsert: 
		str = "ѯ��¼������";
		break;
	case FTTCMD_ReqForQuoteInsert_Rsp: 
		str = "ѯ��¼������";
		break;

		///ִ������¼������
	case FTTCMD_ReqExecOrderInsert: 
		str = "ִ������¼������";
		break;
	case FTTCMD_ReqExecOrderInsert_Rsp: 
		str = "ִ������¼������";
		break;

		///ִ�������������
	case FTTCMD_ReqExecOrderAction: 
		str = "ִ�������������";
		break;
	case FTTCMD_ReqExecOrderAction_Rsp: 
		str = "ִ�������������";
		break;

		///�����ѯ����
	case FTTCMD_ReqQryOrder: 
		str = "�����ѯ����";
		break;
	case FTTCMD_ReqQryOrder_Rsp: 
		str = "�����ѯ����";
		break;

		///�����ѯ�ɽ�
	case FTTCMD_ReqQryTrade: 
		str = "�����ѯ�ɽ�";
		break;
	case FTTCMD_ReqQryTrade_Rsp: 
		str = "�����ѯ�ɽ�";
		break;

		///�����ѯͶ���ֲ߳�
	case FTTCMD_ReqQryInvestorPosition: 
		str = "�����ѯͶ���ֲ߳�";
		break;
	case FTTCMD_ReqQryInvestorPosition_Rsp: 
		str = "�����ѯͶ���ֲ߳�";
		break;

		///�����ѯ�ʽ��˻�
	case FTTCMD_ReqQryTradingAccount:
		str = "�����ѯ�ʽ��˻�";
		break;
	case FTTCMD_ReqQryTradingAccount_Rsp: 
		str = "�����ѯ�ʽ��˻�";
		break;

		///�����ѯͶ����
	case FTTCMD_ReqQryInvestor:
		str = "�����ѯͶ����";
		break;
	case FTTCMD_ReqQryInvestor_Rsp: 
		str = "�����ѯͶ����";
		break;

		///�����ѯ��Լ��֤����
	case FTTCMD_ReqQryInstrumentMarginRate: 
		str = "�����ѯ��Լ��֤����";
		break;
	case FTTCMD_ReqQryInstrumentMarginRate_Rsp: 
		str = "�����ѯ��Լ��֤����";
		break;

		///�����ѯ��Լ��������
	case FTTCMD_ReqQryInstrumentCommissionRate: 
		str = "�����ѯ��Լ��������";
		break;
	case FTTCMD_ReqQryInstrumentCommissionRate_Rsp: 
		str = "�����ѯ��Լ��������";
		break;

		///�����ѯ������
	case FTTCMD_ReqQryExchange: 
		str = "�����ѯ������";
		break;
	case FTTCMD_ReqQryExchange_Rsp: 
		str = "�����ѯ������";
		break;

		///�����ѯ��Ʒ
	case FTTCMD_ReqQryProduct: 
		str = "�����ѯ��Ʒ";
		break;
	case FTTCMD_ReqQryProduct_Rsp: 
		str = "�����ѯ��Ʒ";
		break;

		///�����ѯ��Լ
	case FTTCMD_ReqQryInstrument: 
		str = "�����ѯ��Լ";
		break;
	case FTTCMD_ReqQryInstrument_Rsp: 
		str = "�����ѯ��Լ";
		break;

		///�����ѯͶ���߽�����
	case FTTCMD_ReqQrySettlementInfo: 
		str = "�����ѯͶ���߽�����";
		break;
	case FTTCMD_ReqQrySettlementInfo_Rsp: 
		str = "�����ѯͶ���߽�����";
		break;

		///�����ѯͶ���ֲ߳���ϸ
	case FTTCMD_ReqQryInvestorPositionDetail: 
		str = "�����ѯͶ���ֲ߳���ϸ";
		break;
	case FTTCMD_ReqQryInvestorPositionDetail_Rsp: 
		str = "�����ѯͶ���ֲ߳���ϸ";
		break;

		///�����ѯ�ͻ�֪ͨ
	case FTTCMD_ReqQryNotice: 
		str = "�����ѯ�ͻ�֪ͨ";
		break;
	case FTTCMD_ReqQryNotice_Rsp: 
		str = "�����ѯ�ͻ�֪ͨ";
		break;

		///�����ѯ������Ϣȷ��
	case FTTCMD_ReqQrySettlementInfoConfirm: 
		str = "�����ѯ������Ϣȷ��";
		break;
	case FTTCMD_ReqQrySettlementInfoConfirm_Rsp: 
		str = "�����ѯ������Ϣȷ��";
		break;

		///�����ѯͶ���ֲ߳���ϸ
	case FTTCMD_ReqQryInvestorPositionCombineDetail: 
		str = "�����ѯͶ���ֲ߳���ϸ";
		break;
	case FTTCMD_ReqQryInvestorPositionCombineDetail_Rsp: 
		str = "�����ѯͶ���ֲ߳���ϸ";
		break;

		///�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ
	case FTTCMD_ReqQryCFMMCTradingAccountKey: 
		str = "�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ";
		break;
	case FTTCMD_ReqQryCFMMCTradingAccountKey_Rsp:
		str = "�����ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ";
		break;

		///�����ѯ��Ȩ���׳ɱ�
	case FTTCMD_ReqQryOptionInstrTradeCost:
		str = "�����ѯ��Ȩ���׳ɱ�";
		break;
	case FTTCMD_ReqQryOptionInstrTradeCost_Rsp:
		str = "�����ѯ��Ȩ���׳ɱ�";
		break;

		///�����ѯ��Ȩ��Լ������
	case FTTCMD_ReqQryOptionInstrCommRate:
		str = "�����ѯ��Ȩ��Լ������";
		break;
	case FTTCMD_ReqQryOptionInstrCommRate_Rsp:
		str = "�����ѯ��Ȩ��Լ������";
		break;

		///�����ѯִ������
	case FTTCMD_ReqQryExecOrder:
		str = "�����ѯִ������";
		break;
	case FTTCMD_ReqQryExecOrder_Rsp:
		str = "�����ѯִ������";
		break;

		///�����ѯת����ˮ
	case FTTCMD_ReqQryTransferSerial:
		str = "�����ѯת����ˮ";
		break;
	case FTTCMD_ReqQryTransferSerial_Rsp:
		str = "�����ѯת����ˮ";
		break;

		///�����ѯ����ǩԼ��ϵ
	case FTTCMD_ReqQryAccountregister:
		str = "�����ѯ����ǩԼ��ϵ";
		break;
	case FTTCMD_ReqQryAccountregister_Rsp:
		str = "�����ѯ����ǩԼ��ϵ";
		break;

		///�����ѯǩԼ����
	case FTTCMD_ReqQryContractBank:
		str = "�����ѯǩԼ����";
		break;
	case FTTCMD_ReqQryContractBank_Rsp:
		str = "�����ѯǩԼ����";
		break;

		///�����ѯԤ��
	case FTTCMD_ReqQryParkedOrder:
		str = "�����ѯԤ��";
		break;
	case FTTCMD_ReqQryParkedOrder_Rsp:
		str = "�����ѯԤ��";
		break;

		///�����ѯԤ�񳷵�
	case FTTCMD_ReqQryParkedOrderAction:
		str = "�����ѯԤ�񳷵�";
		break;
	case FTTCMD_ReqQryParkedOrderAction_Rsp:
		str = "�����ѯԤ�񳷵�";
		break;

		///�����ѯ����֪ͨ
	case FTTCMD_ReqQryTradingNotice:
		str = "�����ѯ����֪ͨ";
		break;
	case FTTCMD_ReqQryTradingNotice_Rsp:
		str = "�����ѯ����֪ͨ";
		break;

		///�����ѯ���͹�˾���ײ���
	case FTTCMD_ReqQryBrokerTradingParams:
		str = "�����ѯ���͹�˾���ײ���";
		break;
	case FTTCMD_ReqQryBrokerTradingParams_Rsp:
		str = "�����ѯ���͹�˾���ײ���";
		break;

		///�ڻ����������ʽ�ת�ڻ�����
	case FTTCMD_ReqFromBankToFutureByFuture:
		str = "�ڻ����������ʽ�ת�ڻ�����";
		break;
	case FTTCMD_ReqFromBankToFutureByFuture_Rsp:
		str = "�ڻ����������ʽ�ת�ڻ�����";
		break;

		///�ڻ������ڻ��ʽ�ת��������
	case FTTCMD_ReqFromFutureToBankByFuture:
		str = "�ڻ������ڻ��ʽ�ת��������";
		break;
	case FTTCMD_ReqFromFutureToBankByFuture_Rsp:
		str = "�ڻ������ڻ��ʽ�ת��������";
		break;

		///�ڻ������ѯ�����������
	case FTTCMD_ReqQueryBankAccountMoneyByFuture:
		str = "�ڻ������ѯ�����������";
		break;
	case FTTCMD_ReqQueryBankAccountMoneyByFuture_Rsp:
		str = "�ڻ������ѯ�����������";
		break;


		//��CTP��Spi����������

	case FFT_SPI_START:
		str = "";break; 

		// ���׷��������ӳɹ���Ӧ
	case FTTCMD_OnFrontConnected_RON:
		str = "���׷��������ӳɹ���Ӧ";
		break;

			// ���׷��������ӶϿ���Ӧ
	case FTTCMD_OnFrontDisconnected_RON:
		str = "���׷��������ӶϿ���Ӧ";
		break;

		///��¼������Ӧ
	case FTTCMD_OnRspUserLogin_RON:
		str = "��¼������Ӧ";
		break;

		///�ǳ�������Ӧ
	case FTTCMD_OnRspUserLogout_RON:
		str = "�ǳ�������Ӧ";
		break;

		///�û��������������Ӧ
	case FTTCMD_OnRspUserPasswordUpdate_RON:		
		str = "�û��������������Ӧ";
		break;

		///�ʽ��˻��������������Ӧ
	case FTTCMD_OnRspTradingAccountPasswordUpdate_RON:
		str = "�ʽ��˻��������������Ӧ";
		break;

		///����¼��������Ӧ
	case FTTCMD_OnRspOrderInsert_RON: 
		str = "����¼��������Ӧ";
		break;

		///Ԥ��¼��������Ӧ
	case FTTCMD_OnRspParkedOrderInsert_RON: 
		str = "Ԥ��¼��������Ӧ";break;

		///Ԥ�񳷵�¼��������Ӧ
	case FTTCMD_OnRspParkedOrderAction_RON: 
		str = "Ԥ�񳷵�¼��������Ӧ";break;

		///��������������Ӧ
	case FTTCMD_OnRspOrderAction_RON: 
		str = "��������������Ӧ";break;

		///
	case FTTCMD_OnRspQueryMaxOrderVolume_RON: 
		str = "��ѯ��󱨵�������Ӧ";break;

		///Ͷ���߽�����ȷ����Ӧ
	case FTTCMD_OnRspSettlementInfoConfirm_RON: 
		str = "Ͷ���߽�����ȷ����Ӧ";break;

		///ɾ��Ԥ����Ӧ
	case FTTCMD_OnRspRemoveParkedOrder_RON: 
		str = "ɾ��Ԥ����Ӧ";break;

		///ɾ��Ԥ�񳷵���Ӧ
	case FTTCMD_OnRspRemoveParkedOrderAction_RON: 
		str = "ɾ��Ԥ�񳷵���Ӧ";break;

		///ִ������¼��������Ӧ
	case FTTCMD_OnRspExecOrderInsert_RON: 
		str = "ִ������¼��������Ӧ";break;

		///ִ���������������Ӧ
	case FTTCMD_OnRspExecOrderAction_RON: 
		str = "ִ���������������Ӧ";break;

		///�����ѯ������Ӧ
	case FTTCMD_OnRspQryOrder_RON: 
		str = "�����ѯ������Ӧ";break;

		///�����ѯ�ɽ���Ӧ
	case FTTCMD_OnRspQryTrade_RON: 
		str = "�����ѯ�ɽ���Ӧ";break;

		///�����ѯͶ���ֲ߳���Ӧ
	case FTTCMD_OnRspQryInvestorPosition_RON: 
		str = "�����ѯͶ���ֲ߳���Ӧ";break;

		///�����ѯ�ʽ��˻���Ӧ
	case FTTCMD_OnRspQryTradingAccount_RON: 
		str = "�����ѯ�ʽ��˻���Ӧ";break;

		///�����ѯͶ������Ӧ
	case FTTCMD_OnRspQryInvestor_RON: 
		str = "�����ѯͶ������Ӧ";break;

		///�����ѯ��Լ��֤������Ӧ
	case FTTCMD_OnRspQryInstrumentMarginRate_RON: 
		str = "�����ѯ��Լ��֤������Ӧ";break;

		///�����ѯ��Լ����������Ӧ
	case FTTCMD_OnRspQryInstrumentCommissionRate_RON: 
		str = "�����ѯ��Լ����������Ӧ";break;

		///�����ѯ��������Ӧ
	case FTTCMD_OnRspQryExchange_RON: 
		str = "�����ѯ��������Ӧ";break;

		///�����ѯ��Ʒ��Ӧ
	case FTTCMD_OnRspQryProduct_RON:
		str = "�����ѯ��Ʒ��Ӧ";break;

		///�����ѯ��Լ��Ӧ
	case FTTCMD_OnRspQryInstrument_RON: 
		str = "�����ѯ��Լ��Ӧ";break;

		///�����ѯͶ���߽�������Ӧ
	case FTTCMD_OnRspQrySettlementInfo_RON:
		str = "�����ѯͶ���߽�������Ӧ";break;

		///�����ѯͶ���ֲ߳���ϸ��Ӧ
	case FTTCMD_OnRspQryInvestorPositionDetail_RON:
		str = "�����ѯͶ���ֲ߳���ϸ��Ӧ";break;

		///�����ѯ�ͻ�֪ͨ��Ӧ
	case FTTCMD_OnRspQryNotice_RON: 
		str = "�����ѯ�ͻ�֪ͨ��Ӧ";break;

		///�����ѯ������Ϣȷ����Ӧ
	case FTTCMD_OnRspQrySettlementInfoConfirm_RON:
		str = "�����ѯ������Ϣȷ����Ӧ";break;

		///�����ѯͶ���ֲ߳���ϸ��Ӧ
	case FTTCMD_OnRspQryInvestorPositionCombineDetail_RON:
		str = "�����ѯͶ���ֲ߳���ϸ��Ӧ";break;

		///��ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ��Ӧ
	case FTTCMD_OnRspQryCFMMCTradingAccountKey_RON:
		str = "��ѯ��֤����ϵͳ���͹�˾�ʽ��˻���Կ��Ӧ";break;

		///�����ѯ��Ȩ���׳ɱ���Ӧ
	case FTTCMD_OnRspQryOptionInstrTradeCost_RON:
		str = "�����ѯ��Ȩ���׳ɱ���Ӧ";break;

		///�����ѯ��Ȩ��Լ��������Ӧ
	case FTTCMD_OnRspQryOptionInstrCommRate_RON: 
		str = "�����ѯ��Ȩ��Լ��������Ӧ";break;

		///�����ѯִ��������Ӧ
	case FTTCMD_OnRspQryExecOrder_RON: 
		str = "�����ѯִ��������Ӧ";break;

		///�����ѯת����ˮ��Ӧ
	case FTTCMD_OnRspQryTransferSerial_RON: 
		str = "�����ѯת����ˮ��Ӧ";break;

		///�����ѯ����ǩԼ��ϵ��Ӧ
	case FTTCMD_OnRspQryAccountregister_RON: 
		str = "�����ѯ����ǩԼ��ϵ��Ӧ";break;

		///����Ӧ��
	case FTTCMD_OnRspError_RON: 
		str = "����Ӧ��";break;

		///����֪ͨ
	case FTTCMD_OnRtnOrder_RON: 
		str = "����֪ͨ";break;

		///�ɽ�֪ͨ
	case FTTCMD_OnRtnTrade_RON:
		str = "�ɽ�֪ͨ";break;

		///����¼�����ر�
	case FTTCMD_OnErrRtnOrderInsert_RON:
		str = "����¼�����ر�";break;

		///������������ر�
	case FTTCMD_OnErrRtnOrderAction_RON: 
		str = "������������ر�";break;

		///��Լ����״̬֪ͨ
	case FTTCMD_OnRtnInstrumentStatus_RON:
		str = "��Լ����״̬֪ͨ";break;

		///����֪ͨ
	case FTTCMD_OnRtnTradingNotice_RON:
		str = "����֪ͨ";break;

		///ִ������֪ͨ
	case FTTCMD_OnRtnExecOrder_RON:
		str = "ִ������֪ͨ";break;

		///ִ������¼�����ر�
	case FTTCMD_OnErrRtnExecOrderInsert_RON:
		str = "ִ������¼�����ر�";break;

		///ִ�������������ر�
	case FTTCMD_OnErrRtnExecOrderAction_RON: 
		str = "ִ�������������ر�";break;

		///ѯ��¼��������Ӧ
	case FTTCMD_OnRspForQuoteInsert_RON: 
		str = "ѯ��¼��������Ӧ";break;

		///ѯ��¼�����ر�
	case FTTCMD_OnErrRtnForQuoteInsert_RON:
		str = "ѯ��¼�����ر�";break;

		///�����ѯǩԼ������Ӧ
	case FTTCMD_OnRspQryContractBank_RON:
		str = "�����ѯǩԼ������Ӧ";break;

		///�����ѯԤ����Ӧ
	case FTTCMD_OnRspQryParkedOrder_RON: 
		str = "�����ѯԤ����Ӧ";break;

		///�����ѯԤ�񳷵���Ӧ
	case FTTCMD_OnRspQryParkedOrderAction_RON: 
		str = "�����ѯԤ�񳷵���Ӧ";break;

		///�����ѯ����֪ͨ��Ӧ
	case FTTCMD_OnRspQryTradingNotice_RON: 
		str = "�����ѯ����֪ͨ��Ӧ";break;

		///�����ѯ���͹�˾���ײ�����Ӧ
	case FTTCMD_OnRspQryBrokerTradingParams_RON: 
		str = "�����ѯ���͹�˾���ײ�����Ӧ";break;

		///�ڻ����������ʽ�ת�ڻ�֪ͨ
	case FTTCMD_OnRtnFromBankToFutureByFuture_RON: 
		str = "�ڻ����������ʽ�ת�ڻ�֪ͨ";break;

		///�ڻ������ڻ��ʽ�ת����֪ͨ
	case FTTCMD_OnRtnFromFutureToBankByFuture_RON: 
		str = "�ڻ������ڻ��ʽ�ת����֪ͨ";break;

		///�ڻ������ѯ�������֪ͨ
	case FTTCMD_OnRtnQueryBankBalanceByFuture_RON: 
		str = "�ڻ������ѯ�������֪ͨ";break;

		///�ڻ����������ʽ�ת�ڻ�Ӧ��
	case FTTCMD_OnRspFromBankToFutureByFuture_RON:
		str = "�ڻ����������ʽ�ת�ڻ�Ӧ��";break;

		///�ڻ������ڻ��ʽ�ת����Ӧ��
	case FTTCMD_OnRspFromFutureToBankByFuture_RON:
		str = "�ڻ������ڻ��ʽ�ת����Ӧ��";break;

		///�ڻ������ѯ�������Ӧ��
	case FTTCMD_OnRspQueryBankAccountMoneyByFuture_RON: 
		str = "�ڻ������ѯ�������Ӧ��";break;

	default:
		bFind = false;
		str = "";break;
	}

	return bFind;
}
void split(const string& s, char c, vector<string>& v) 
{
	string::size_type i = 0;
	string::size_type j = s.find(c);
	if(j == -1 && s.length() != 0)
	{
		v.push_back(s);
		return;
	}
	while (j != string::npos) 
	{
		v.push_back(s.substr(i, j-i));
		i = ++j;
		j = s.find(c, j);

		if (j == string::npos)
			v.push_back(s.substr(i, s.length( )));
	}

}