#pragma once

#include <string>
#include <map>
#include <set>
#include <vector>
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcUserApiStruct.h"
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
#include "easymutex.h"

using namespace std;

class COrderCenter
{
public:

	typedef struct {
		UINT nStart;
		UINT nEnd;
		UINT nSubNum;
	} EXCHANGETIME;

	typedef std::map<std::string, CThostFtdcRspUserLoginField>
															MAPUSERSESSION;
	typedef std::map<std::string, PlatformStru_DepthMarketData>
															MAPQUOT;
	typedef std::map<std::string, CThostFtdcOrderField>		MAPORDER;
	typedef std::map<std::string, std::set<std::string>>	MAPINSTID2ORDER;
	typedef std::map<std::string, std::vector<EXCHANGETIME>>			MAPEXCHANGETIME;

public:
	static void split(const std::string& s, char c, std::vector<std::string>& v); 
	static bool IsInvalidValue(double fVal);
	static bool ConvInput2Order(const CThostFtdcInputOrderField &input, 
				const std::string& strExchangeID, CThostFtdcOrderField& order);
	static bool ConvOrder2Trade(const CThostFtdcOrderField &order, 
				CThostFtdcTradeField& trade);

public:
	COrderCenter(void);
	~COrderCenter(void);

public:

	void LockObject() {
		m_mutex.lock();
	};
	void UnlockObject() {
		m_mutex.unlock();
	};

	// ����ģ��ɽ�����
	bool Reinit(const std::string& strTradingDay);
	
	//���ý����գ��������йҵ�Ҳ����
	bool ResertTradingDay(const std::string& strTradingDay);

	bool PushOrder(const CThostFtdcOrderField& order);

	//�ʺŵ�¼
	bool Login(const CThostFtdcRspUserLoginField& rspLogin);

	//�ʺŵǳ�
	bool Logout(CThostFtdcUserLogoutField& userLogout, int nRequestID);

	//�µ��ӿ�
	int InsertOrder(const CThostFtdcInputOrderField &input, 
				int nRequestID, const std::string& strExchangeID, 
				std::vector<CThostFtdcOrderField>& vecOrder, 
				std::vector<CThostFtdcTradeField>& vecTrade);
	
	//�����ӿ�
	int CancelOrder(const CThostFtdcInputOrderActionField& action, int nRequestID, 
				std::vector<CThostFtdcOrderField>& vecOrder);
	
	//����ӿ�
	int Quot(const PlatformStru_DepthMarketData& quot, 
				std::vector<CThostFtdcOrderField>& vecOrder, 
				std::vector<CThostFtdcTradeField>& vecTrader);

protected:
	int GetNextSessionID() {
		n_nSessionID++;
		return n_nSessionID;
	};
	int GetNextSeqID() {
		m_nSeqID++;
		return m_nSeqID;
	};
	int GetNextOrderID() {
		m_nOrderID++;
		return m_nOrderID;
	};
	int GetNextTradeID() {
		m_nTradeID++;
		return m_nTradeID;
	};
	bool GetUserSession(const std::string& strBrokerID, const std::string& strDealerID, 
				int& nFrontID, int& nSessionID);
	bool MarkOrderBase(CThostFtdcOrderField& order);
	bool MarkOrderSys(CThostFtdcOrderField& order);
	bool MarkTrade(CThostFtdcTradeField& trade, double fPrice);
	void LoadExchangeTime();
	bool IsInTradingTime(const std::string& strExchangeID, int nTime);
	void SaveDataToIni();

protected:

	//������
	Ceasymutex			m_mutex;

	std::string			m_strTradingDay;

	MAPUSERSESSION		m_mapUserSession;
	MAPQUOT				m_mapQuot;
	MAPORDER			m_mapOrder;
	MAPINSTID2ORDER		m_mapInst2Order;

	MAPEXCHANGETIME		m_mapTradingTime;

	int					n_nSessionID;
	int					m_nSeqID;
	int					m_nOrderID;
	int					m_nTradeID;
};

