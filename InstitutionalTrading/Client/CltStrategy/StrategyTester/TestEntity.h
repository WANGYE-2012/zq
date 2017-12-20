#pragma once

#include <queue>
#include "..\CommonClasses\DataEntity.h"
#include "..\CommonClasses\CMemCache.h"
#include "BaseBusinessSpi.h"
#include "TradeSimulation.h"
#include "..\SvrTradeData\Interface_SvrTradeData.h"

using namespace std;


#include "../SvrTradeData/Interface_SvrTradeData.h";
#pragma comment(lib, "SvrTradeData_Local.lib")	

// ��CTestEntity���ü������ݹ���ģ�顢ģ�⽻��ģ�飬�����г�ʼ������release�ȹ�����
// ��ȡ�����ļ�����ʵ���������ͣ�����RunSingleStrategy.exe��
// ��Ҫ���ܣ�
// 1.  ����CDataEntity�������������ɺ󱣴湤����
// 2.  ������������̡߳�
// 3.  ��ȡ��ģ�⽻���йص������ļ���
// 4.  ������ʼ�����ݹ���ģ�顣
// 5.  ������ʼ��ģ�⽻��ģ�顣
// 6.  �����ݹ���ģ�顢ģ�⽻��ģ���������顣
// 7.  �رղ����̣߳�ж�����ݹ���ģ�顢ģ�����ģ�顣
// 8.  �������������ݡ�
// 9.  ��ͣ����������ֹδ��ɵĲ��Թ��̡�


class CTestEntity : public CBaseBusinessSpi
{
public:
	enum {	conRunNone,			// δ����
			conRunning,			// ������
			conRunPause,		// ������ͣ
			conRunFinish		// �������
	};

	static DWORD WINAPI Proc_Thread(LPVOID lpParam);
	static void GetNextDateTime(int& nNextDate, int& nNextMinute);
	static int ConvTime(char* pcsTime);

public:
	CTestEntity(void);
	virtual ~CTestEntity(void);

	void LockObject() {
		::WaitForSingleObject(m_mutex, INFINITE);
	};
	void UnlockObject() {
		::ReleaseMutex(m_mutex);
	};

public:
	int  GetRunStatus() {
		return m_nRunStatus;
	};

	void SetDataEntity(CDataEntity* pDataEntity) {
		m_pDataEntity = pDataEntity;
	};
	CDataEntity* GetDataEntity() {
		return m_pDataEntity;
	};

	void SetInstrumentIDs(std::string& strIDs) {
		m_strInstrumentIDs = strIDs;
	};
	const std::string& GetInstrumentIDs() {
		return m_strInstrumentIDs;
	};
	void SetRunPath(const std::string& strPath) {
		m_strRunPath = strPath;
	};
	const std::string& GetRunPath() {
		return m_strRunPath;
	};
	void SetQuotFromFile(bool bHas) {
		m_bQuotFromFile = bHas;
	};
	bool GetQuotFromFile() {
		return m_bQuotFromFile;
	};
	void ResetQuotRange(int nStartDate, int nEndDate) {
		m_nStartDate = nStartDate;
		m_nEndDate = nEndDate;
	};
	void GetQuotRange(int& nStartDate, int& nEndDate) {
		nStartDate = m_nStartDate;
		nEndDate = m_nEndDate;
	};
	void SetQuotFile(const std::string& strFile) {
		m_strQuotFile = (std::string&)strFile;
	};
	const std::string& GetQuotFile() {
		return m_strQuotFile;
	};
	int GetCurrQuotPos() {
		return m_nCurrQuotPos;
	};

	bool Init(const std::vector<std::string>& vecInstrument, 
			CStrategyDataEx& strategy, const StrategyTestParams& testParams);
	void Start();
	void Pause();
	void Stop();
	void Release();


public:
	void OnQuotConnected();
	void OnQuotDisconnected();
	void OnTraderConnected();
	void OnTraderDisconnected();

	void OnQuotLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg);
	void OnTraderLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg);

	void OnInstrument(const std::vector<PlatformStru_InstrumentInfo>& vec, int nErrorNum);

	void OnDepthMarketData(const PlatformStru_DepthMarketData& rsp, int nErrorNum);
	void OnQueryQuotData(const vector<PlatformStru_DepthMarketData>& vec, int nErrorNum);
	
	void OnCommissionRate(const PlatformStru_InstrumentCommissionRate& rsp, int nErrorNum);
	void OnMarginRate(const PlatformStru_InstrumentMarginRate& rsp, int nErrorNum);


protected:
	bool loadXMLString(std::string& strStraID, std::string& strStraParams, 
						std::string& strInstruments, std::string& strStraRunPath, 
						StrategyTestParams& testParams, std::string& strXML);
	bool CreateRunStrategyExe();

	// ִ�в��Բ��Թ��̡�ֻ����Proc_Thread�����е��á�
	void Run();

	// ֻ����Run�����е��á�
	const bool GetNextQuotItem(PlatformStru_DepthMarketData& quot, bool& bEnd);
	// ֻ����Run�����е��á�
	void PushQuotToSimulate(const PlatformStru_DepthMarketData& quot);

protected:
	HANDLE			m_mutex;
	HANDLE			m_hThread;
	DWORD			m_nThreadID;
	HANDLE			m_hRunStrategyProcess;
	CMemCache		m_memCache;
	CMemCache		m_memSubCache;
	CDataEntity*	m_pDataEntity;
	CStrategyMdApi*	m_pMDApi;
	CTradeSimulation	m_oTradeSimu;
	CInterface_SvrTradeData*	m_pTradeData;

	HANDLE			m_hQuotFile;
	int				m_nRunStatus;
	std::string		m_strRunPath;
	std::string		m_strInstrumentIDs;
	bool			m_bQuotFromFile;
	int				m_nStartDate;
	int				m_nEndDate;
	int				m_nNextDate;
	int				m_nNextMinute;
	std::string		m_strQuotFile;

	bool			m_bIsConnectedQuot;
	int				m_nCurrQuotPos;
	int				m_nQuotUpdateTime;
	bool			m_bWaitQuotQuery;
	
	std::list<PlatformStru_DepthMarketData>::iterator 
					m_itDMQuotList;
	std::queue<PlatformStru_DepthMarketData>
					m_queueQuot;
	std::vector<std::string>	m_vecInstrument;
	CStrategyDataEx	m_strategy;
	StrategyTestParams m_testParams;

};
