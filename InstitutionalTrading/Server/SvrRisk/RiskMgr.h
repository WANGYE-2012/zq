#pragma once
#include "MsgQueue.h"
class CToolThread;
class CThreadSharedData;

class CRiskMgr
{
public:
	CRiskMgr(void);
	~CRiskMgr(void);
private:
	static CRiskMgr* m_pRiskMgr;
	std::string m_strTodayString;// �洢����orderref��ֵ���ļ�		
public:
	static void Init(void);
	static void Release(void);
	static DWORD WINAPI RiskThreadProcForceClose (PVOID pParam);	
	void doProcessBusinessDataForceClose();


	CToolThread*		  m_RiskMgrThread_AutoForceClose;
	const int			  m_IdleSleepTime;

	void SendForceCloseMessage(SForceClose& sForceClose);

	void GenerateInsertOrder( const std::vector<PlatformStru_Position>& vPosition, 
		std::vector<PlatformStru_InputOrder>& vInsertOrder );
	void GenerateOrderAction( const std::vector<PlatformStru_OrderInfo>& vOrder, 
		std::vector<PlatformStru_InputOrderAction>& vOrderAction );

	bool NewCloseOrder( const PlatformStru_Position &position, PlatformStru_InputOrder &inputOrder, 
		char Direction, int nCloseVolumn, char cCloseFlag );

	std::map<int, int>				m_mapInvestor2OrderRef;
	CReadWriteLock													m_Mutex_Investor2OrderRef;
	
	void	GetInputOrderKey(int nTradeInvestorID, InputOrderKey& lKey);

	void CloseAllPositon(SForceClose& sForceClose);//ǿƽ���гֲ�
	void CloseOneInstrument(SForceClose& sForceClose);//ǿƽ���гֲ�
	void CloseOnePositonDetail(SForceClose& sForceClose);//ǿƽĳһ���ֲ���ϸ

	void GenerateInsertOrder_PositionDetail(PlatformStru_PositionDetail& outData, std::vector<PlatformStru_InputOrder>& vInsertOrder);
};
