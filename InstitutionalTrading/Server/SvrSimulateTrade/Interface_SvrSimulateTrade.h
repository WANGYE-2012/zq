#pragma once

#include "Interface_SvrCommon.h"
#include <string>
#include <vector>
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcUserApiStruct.h"
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"


#ifndef SIMULATETRADE_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRSIMULATETRADE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRSIMULATETRADE_EXPORTS
#define SIMULATETRADE_API __declspec(dllexport)
#else
#define SIMULATETRADE_API __declspec(dllimport)
#endif
#endif


class SIMULATETRADE_API CInterface_SvrSimulateTrade : public CInterface_SvrCommon
{
public:
   static CInterface_SvrSimulateTrade& getObj(void);

public:

	// ����ģ��ɽ�����
	virtual bool Reinit(const std::string& strTradingDay) = 0;
	
	//���ý����գ��������йҵ�Ҳ����
	virtual bool ResertTradingDay(const std::string& strTradingDay) = 0;

	//���ͱ���
	virtual bool PushOrder(const CThostFtdcOrderField& order) = 0;

	//�ʺŵ�¼�����������ʺŵ�frontID��sessionID
	//����ֵ��true����¼�ɹ���false���ظ���¼��
	virtual bool Login(const CThostFtdcRspUserLoginField& rspLogin) = 0;

	//�ʺŵǳ�
	//����ֵ��true���ǳ��ɹ���false���ǳ�ʧ�ܡ�
	virtual bool Logout(CThostFtdcUserLogoutField& userLogout, int nRequestID) = 0;

	//�µ��ӿ�
	//����: vecOrder�����ر���
	//����: vecTrade�����سɽ�
	//����ֵ��0 �µ��ɹ���1 �û�δ��¼��2 �ظ��ı���
	virtual int InsertOrder(const CThostFtdcInputOrderField &input, 
				int nRequestID, const std::string& strExchangeID, 
				std::vector<CThostFtdcOrderField>& vecOrder, 
				std::vector<CThostFtdcTradeField>& vecTrade) = 0;
	
	//�����ӿ�
	//����: vecOrder�����ر���
	//����ֵ��0 �����ɹ���1 ����ʧ�ܣ��޶�Ӧ�Ĺҵ���2 ����״̬���ʺϳ���
	virtual int CancelOrder(const CThostFtdcInputOrderActionField& action, int nRequestID, 
				std::vector<CThostFtdcOrderField>& vecOrder) = 0;
	
	//����ӿ�
	//����: vecOrder�����ر���
	//����: vecTrade�����سɽ�
	//����ֵ��0 �������óɹ���2 ��Ч�����¼�
	virtual int Quot(const PlatformStru_DepthMarketData& quot, 
				std::vector<CThostFtdcOrderField>& vecOrder, 
				std::vector<CThostFtdcTradeField>& vecTrader) = 0;

};
