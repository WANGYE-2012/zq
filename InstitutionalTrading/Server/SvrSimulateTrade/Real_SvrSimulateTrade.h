#pragma once

#include "Interface_SvrSimulateTrade.h"
#include <map>
#include <set>

class CReal_SvrSimulateTrade : public CInterface_SvrSimulateTrade
{
	//---------------------------------------------------------------------------------
	//	���ݽӿ�
	//---------------------------------------------------------------------------------

	// ����ģ��ɽ�����
	bool Reinit(const std::string& strTradingDay);
	
	//���ý����գ��������йҵ�Ҳ����
	bool ResertTradingDay(const std::string& strTradingDay);

	//���ͱ���
	bool PushOrder(const CThostFtdcOrderField& order);

	//�ʺŵ�¼
	//����ֵ��true����¼�ɹ���false���ظ���¼��
	bool Login(const CThostFtdcRspUserLoginField& rspLogin);

	//�ʺŵǳ�
	//����ֵ��true���ǳ��ɹ���false���ǳ�ʧ�ܡ�
	bool Logout(CThostFtdcUserLogoutField& userLogout, int nRequestID);

	//�µ��ӿ�
	//����ֵ��0 �µ��ɹ���1 �û�δ��¼��2 �ظ��ı���
	int InsertOrder(const CThostFtdcInputOrderField &input, 
				int nRequestID, const std::string& strExchangeID, 
				std::vector<CThostFtdcOrderField>& vecOrder, 
				std::vector<CThostFtdcTradeField>& vecTrade);
	
	//�����ӿ�
	//����ֵ��0 �����ɹ���1 ����ʧ�ܣ��޶�Ӧ�Ĺҵ���2 ����״̬���ʺϳ���
	int CancelOrder(const CThostFtdcInputOrderActionField& action, int nRequestID, 
				std::vector<CThostFtdcOrderField>& vecOrder);
	
	//����ӿ�
	int Quot(const PlatformStru_DepthMarketData& quot, 
				std::vector<CThostFtdcOrderField>& vecOrder, 
				std::vector<CThostFtdcTradeField>& vecTrader);

public:
    CReal_SvrSimulateTrade();
    virtual ~ CReal_SvrSimulateTrade();
};







