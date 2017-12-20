#pragma once
#include "interface_svrquotserver.h"
#include "../SvrDBOpr/Interface_SvrDBOpr.h"
#include "ThreadManage.h"
#include "ThreadPoolSelfDefine.h"


// enum WEEKDAY{Sunday=0, Monday,Tuesday,Wednesday,Thursday,Friday,Saturday};

class CSvrQuotServerImpl : public CInterface_SvrQuotServer
{
public:
    static CSvrQuotServerImpl& getObj(void);
    static void deleteObj();


public:
    bool LoadConnectCtpParamFromDB(BrokerInfo& brokerInfo,std::map<std::string,std::vector<std::string>>& mapBrokerCode2AddrParam);
    bool LoadInstrumentsFromDB(const TThostFtdcDateType pTradeDay,std::set<std::string>& setInstrument);

    CF_ERROR GetReinitTime(sReinitTime& sTime);
    CF_ERROR SetReinitTime(const sReinitTime& sTime);
 
    void CreateDeleteThread();
    //���2��������ǰ����������
    bool DeleteTradingDayData_ex();
	////ɾ��QuotTrigger���о�����
	//bool DeleteQuotTriggerData_ex();
	//���ʼ��ʱ�����̳߳ش�������K�������߳�
	void CreateLoadKLineDataThread();

    //��ѯ��ĳ��ĳ��ĳ��ĳʱĳ�֡���ʷ����ӿ�
    //nErrorCode -100 ʱ���ʽ����ȷ
    bool QueryQuotData( const std::string strDateTime, 
        std::vector<PlatformStru_DepthMarketData_EX>& vMarketData ,
        int& nErrorCode);
	bool QueryKlineData(KLineQuery* pData ,int&lError,int nsocket,int nSeq);
	bool QuerySettlementPrice(KLineQuery* pData, int& lError, vector<SettlementPrice>& outData);
	static DWORD  WINAPI QueryKlineDBData(LPVOID lp);
	static DWORD  WINAPI QueryTimelineDBData(LPVOID lp);
	static DWORD  WINAPI LoadKLineDataThreadProc(void* lpParam);
	static DWORD  WINAPI LoadKLineData();

	CF_ERROR QueryVolatilityData(Volatility& pData);

	int GetRestLoadkLineDataThread();
private:
    //���ط������Զ�����ʱ��
    void LoadServerReinitTime();

    //����������ʷ���ݱ����ֹʱ��
    void LoadQuotDataSaveTime();

	static int GetParamFromString(std::string Str, std::vector<std::string>& StringVec, char Delim = ';');
	static void RespKlineData(int nsocket,int nSeq,std::vector<KLineBase>& vecKlineData);
	static void RespTlineData(int nsocket,int nSeq,std::vector<TLineBase>& vecTlineData);
	bool  GetKLineData(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);
private:
    CSvrQuotServerImpl(void);
    virtual ~CSvrQuotServerImpl(void);
    static CSvrQuotServerImpl* m_pObj;

private:
    sReinitTime m_sReinitTime;
    _ExchangDateType m_HistoryDataSaveTime;

    //�̲߳���
    HANDLE	m_hThread;
    DWORD	m_idThread;

	HANDLE	m_hLoadKLineDataThread;
	DWORD	m_idLoadKLineDataThread;
	

private:
	CThreadManage* manage;
public:
	/*static std::vector<KLineBase> m_vecKlineData;
	static std::vector<TLineBase> m_vecTlineData;*/
	//������
	//Ceasymutex			m_mutex;
	//std::vector<KLineData>    m_vKlineData;
	bool	m_bKLineDataThreadLoading;//������ʷK����ʷ���ݵ��߳��Ƿ����
};
