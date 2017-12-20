#pragma once
#include "WriteLog.h"
#include <string>
#include <map>
#include <set>
#include <vector>
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcUserApiStruct.h"
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
#include "easymutex.h"
#include "..\CommonClasses\CMemCache.h"
#include "..\CommonClasses\DataEntity.h"

using namespace std;


#include "../SvrTradeData/Interface_SvrTradeData.h";
#pragma comment(lib, "SvrTradeData_Local.lib")		


class CTradeSimulation
{
public:
	CTradeSimulation(void);
	~CTradeSimulation(void);
	bool init(std::string& nUserID, CDataEntity*	pDataEntity ,int ntradeRule, CMemCache* SubCache);
	bool Release();
	//����ӿ�
	CF_ERROR Quot(const PlatformStru_DepthMarketData& quot);
	static DWORD ThreadWorkerSubscribe(LPVOID pParameter);
	static void AddCommandCallback(const std::string & nsCommand);
	bool SubscribeCacheChannel();
	void DealCommand();
	void AddCommand(const std::string & nsCommand);

	typedef std::map<std::string, PlatformStru_DepthMarketData>   MAPQUOT;

public: static CTradeSimulation& getObj(void);
//private:static auto_ptr<CTradeSimulation> m_pObj;
       static CTradeSimulation* m_pObj;


private:

	static void ConvInput2Order(const PlatformStru_InputOrder &input,PlatformStru_OrderInfo& order);
	static void ConvOrder2Trade(const PlatformStru_OrderInfo &order, 
		PlatformStru_TradeInfo& trade);
    bool  IsInvalidValue(double fVal);  
	void GenerateCancelFail(const PlatformStru_InputOrderAction& action, PlatformStru_OrderInfo& order);
	bool Orderaction2String(std::string strRedis, PlatformStru_InputOrderAction& action);
	bool OrderInput2String(std::string strRedis,PlatformStru_InputOrder& orderInput);

	void LockObject();
	void UnlockObject();

    //���ͱ���
	void PushOrder(PlatformStru_OrderInfo& order);
	//���ͳɽ�
	void PushTrade(PlatformStru_TradeInfo& trade);
	void CancelOrderDeal(PlatformStru_OrderInfo & orderInfo,CF_ERROR nErrorCode);
	

	//�µ��ӿ�
	//����ֵ��0 �µ��ɹ���1 �û�δ��¼��2 �ظ��ı���
	CF_ERROR InsertOrder(const PlatformStru_InputOrder &input);

	//�����ӿ�
	CF_ERROR CancelOrder(const PlatformStru_InputOrderAction& action);

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
	bool MarkOrderBase(PlatformStru_OrderInfo& order);
	bool MarkOrderSys(PlatformStru_OrderInfo& order);
	bool MarkTrade(PlatformStru_TradeInfo& trade,  const PlatformStru_DepthMarketData& quot,int tradeRule);





protected:

	CDataEntity*	m_pDataEntity;//��¼��������
	std::string     m_nUserID;//����ʹ�õĽ���Ա�˺�
	int             m_nTradeRule;//�ɽ�����1-���¼ۣ�2-�����ۣ�3-�������۴����������İ��������ɽ�

	//������
	Ceasymutex			m_mutex;

	std::string			m_strTradingDay;
	std::string			m_strRspOrderTime;

	MAPQUOT				m_mapQuot;


	int					n_nSessionID;
	int					m_nSeqID;
	int					m_nOrderID;
	int					m_nTradeID;

	//CMemCache           mCache;
	
	std::queue<std::string> mCommandList;


	//�̲߳���
	HANDLE				m_hThreadSubscribe;



	CMemCache*           mSubCache;

	double               m_preDynamicProfit;
	int                  m_TotalTradingTime;
	
};
