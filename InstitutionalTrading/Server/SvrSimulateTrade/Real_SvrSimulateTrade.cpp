// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#include "Real_SvrSimulateTrade.h"
#include "SvrSimulateTrade.h"
#include "COrderCenter.h"


extern COrderCenter g_orderCenter;


CReal_SvrSimulateTrade::CReal_SvrSimulateTrade()
{
}

CReal_SvrSimulateTrade::~CReal_SvrSimulateTrade()
{
}

// ����ģ��ɽ�����
bool CReal_SvrSimulateTrade::Reinit(const std::string& strTradingDay)
{
	return g_orderCenter.Reinit(strTradingDay);
}

//���ý����գ��������йҵ�Ҳ����
bool CReal_SvrSimulateTrade::ResertTradingDay(const std::string& strTradingDay)
{
	return g_orderCenter.ResertTradingDay(strTradingDay);
}

//���ͱ���
bool CReal_SvrSimulateTrade::PushOrder(const CThostFtdcOrderField& order)
{
	return g_orderCenter.PushOrder(order);
}

//�ʺŵ�¼
//����ֵ��true����¼�ɹ���false���ظ���¼��
bool CReal_SvrSimulateTrade::Login(const CThostFtdcRspUserLoginField& rspLogin)
{
	return g_orderCenter.Login(rspLogin);
}

//�ʺŵǳ�
//����ֵ��true���ǳ��ɹ���false���ǳ�ʧ�ܡ�
bool CReal_SvrSimulateTrade::Logout(CThostFtdcUserLogoutField& userLogout, int nRequestID)
{
	return g_orderCenter.Logout(userLogout, nRequestID);
}

//�µ��ӿ�
//����ֵ��0 �µ��ɹ���1 �û�δ��¼��2 �ظ��ı���
int CReal_SvrSimulateTrade::InsertOrder(const CThostFtdcInputOrderField &input, 
										int nRequestID, const std::string& strExchangeID, 
										std::vector<CThostFtdcOrderField>& vecOrder, 
										std::vector<CThostFtdcTradeField>& vecTrade)
{
	return g_orderCenter.InsertOrder(input, nRequestID, strExchangeID, 
								   vecOrder, vecTrade);
}

//�����ӿ�
//����ֵ��0 �����ɹ���1 ����ʧ�ܣ��޶�Ӧ�Ĺҵ���2 ����״̬���ʺϳ���
int CReal_SvrSimulateTrade::CancelOrder(const CThostFtdcInputOrderActionField& action, int nRequestID, 
										std::vector<CThostFtdcOrderField>& vecOrder)
{
	return g_orderCenter.CancelOrder(action, nRequestID, vecOrder);
}

//����ӿ�
int CReal_SvrSimulateTrade::Quot(const PlatformStru_DepthMarketData& quot, 
								 std::vector<CThostFtdcOrderField>& vecOrder, 
								 std::vector<CThostFtdcTradeField>& vecTrader)
{
	return g_orderCenter.Quot(quot, vecOrder, vecTrader);
}
