#pragma once
#include <stdio.h>
#include <windows.h>
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcTraderApi.h"
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcUserApiDataType.h"
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcUserApiStruct.h"



//// ��Ա����
//TThostFtdcBrokerIDType g_chBrokerID;
//// �����û�����
//TThostFtdcUserIDType g_chUserID;
class SimpletradeSpi : public CThostFtdcTraderSpi
{
public:
	// ���캯������Ҫһ����Ч��ָ��CThostFtdcMduserApiʵ����ָ��
	SimpletradeSpi(CThostFtdcTraderApi *pUserApi);
	  ~SimpletradeSpi() {}

	  // ���ͻ����뽻���й�ϵͳ������ͨ�����ӣ��ͻ�����Ҫ���е�¼
	  virtual void OnFrontConnected();
	 

	  // ���ͻ����뽻���й�ϵͳͨ�����ӶϿ�ʱ���÷���������
	  virtual void OnFrontDisconnected(int nReason);
	  

	  // ���ͻ��˷�����¼����֮�󣬸÷����ᱻ���ã�֪ͨ�ͻ��˵�¼�Ƿ�ɹ�
	  virtual void OnRspUserLogin(CThostFtdcRspUserLoginField	*pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool	bIsLast);
	 

	  ///�����ѯ��Լ��Ӧ
	  virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	  void GetInstrument(map<std::string,PlatformStru_InstrumentInfo>& outData);


private:	
	bool SaveInstrument2DB(vector<PlatformStru_InstrumentInfo>& nVecInstruemnts);
	bool SaveTradingDay2DB(std::string strTradingday);
	  

private:
	// ָ��CThostFtdcMduserApiʵ����ָ��
	CThostFtdcTraderApi *m_pUserApi;
	vector<PlatformStru_InstrumentInfo>   m_vecInstruemnts;
	map<std::string,PlatformStru_InstrumentInfo>   m_mapInstruemnts;
	std::string m_BrokerID;
	std::string m_UserID;
	std::string m_CTPPassword;
	CThostFtdcReqUserLoginField m_reqUserLogin;
};
