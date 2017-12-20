#pragma once
#include "PlatformCTPQuot.h"
#include "Tools_Win32.h"
#include <queue>
using std::queue;

#define SAVE_TO_DB_INTERVAL 5*60//5 minute
#define LAST_SETTLE_TIME 15*60 //15minute
    

class CPlatformCTPQuotImpl :  public CPlatformCTPQuot
{
public:
    static CPlatformCTPQuotImpl& GetInstance(void);
    static void DeleteInstance();

public:
    CPlatformCTPQuotImpl(const Stru_CTPConnectionParam& ConnParam=Stru_CTPConnectionParam(),
                        bool bPushCache=true,
                        bool bUseComb=false,
                        bool bCalcuCombQuotOnLegQuot=false);
    virtual ~CPlatformCTPQuotImpl(void);

public:
    bool ReInitCtpConnect();

    int  SubscribeClientMarketData(const string& InstrumentID,int hSocket);
    int  UnSubscribeClientMarketData(const string& InstrumentID, int hSocket);
    void DeleteClientSocketFromMap(int nSocket);

    void Clear();

    void SetQuit(int nVal);
	bool GetQuit();
	bool GetQueueQuot(std::vector<PlatformStru_DepthMarketData>& vecData);
	bool GetQueueQuotForKLine( PlatformStru_DepthMarketData& outData, int& nRemainCount);
	void GetSubscribedInst2Socket(std::map<std::string,std::set<SOCKET>>& vec);
    void SetSubscribedInst2Socket(std::map<std::string,std::set<SOCKET>>& vec);
	void GetAppSubscribedInst2Socket(std::map<std::string,std::set<SOCKET>>& vec);
	void SetAppSubscribedInst2Socket(std::map<std::string,std::set<SOCKET>>& vec);

    void GetCTPConnectParam(BrokerInfo& brokerInfo,std::map<std::string, std::vector<std::string>>& mapBrokerCode2AddrParam);
    void SetCTPConnectParam(BrokerInfo& brokerInfo,std::map<std::string, std::vector<std::string>>& mapBrokerCode2AddrParam);
    ///��ȡ�ɹ����������б�
    vector<string> GetSubscribedInstrumentIDs(void) const;

	int IsValidUser(const std::string& strAccout, const std::string& strPwd);
	void SetValidSocket(int hSocket);
	bool IsValidSocket( int hSocket);
	bool IsValidInstrument( std::string strIns);
	int PushLastQuot(const string& InstrumentID,int hSocket);
	int UnSubscribeAppClientMarketData(const string& InstrumentID, int hSocket);
	time_t GetLoginQuotTime();
	void SetIns(map<std::string,PlatformStru_InstrumentInfo>& data);
	/*CThostFtdcTraderApi& GetSHSpi();*/

	void SetDataInTime(PlatformStru_DepthMarketData* pData);
	string GetTradingDay();


protected:
	///��¼������Ӧ
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    ///�������֪ͨ
    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
    ///��������Ӧ��
    void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnFrontDisconnected(int nReason);

private:
    void CopyDepthMarketData( PlatformStru_DepthMarketData& data, const CThostFtdcDepthMarketDataField& depthMarketData );
	void SetExchangeID(CThostFtdcDepthMarketDataField *pDepthMarketData);
	bool SaveSettlementPrice2DB(CThostFtdcDepthMarketDataField *pDepthMarketData);
	void VolatilityCalc(std::string strInstrumentID,string strTradingday);
	bool SaveVolatility2DB(Volatility* pVolatility);
	void GetCfgInfo();
	void split(const std::string& s, char c, std::vector<std::string>& v) ;
	
	BOOL SetInTimeData_KLine(PlatformStru_DepthMarketData* pData);
	//BOOL SetInTimeData_TLine(PlatformStru_DepthMarketData* pData);
	BOOL SetInTimeData_DiffSpan(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase,std::string exchangeID);
	BOOL SetInTimeData_MINS(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine, int nTimeSpan ,vector<struct tm>& vecTimeScales);
	BOOL SetInTimeData_Month(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine,vector<struct tm>& vecTimeScales);
	BOOL SetInTimeData_Week(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine,vector<struct tm>& vecTimeScales);
	BOOL SetInTimeData_Day(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine,vector<struct tm>& vecTimeScales);
	bool IsOneMonth(struct tm tm1, struct tm tm2);
	bool IsOneWeek(struct tm tm1, struct tm tm2);
	struct tm GetTimeInSpan(struct tm tmPara, int nSpan,vector<struct tm>& vecTimeScales);
	//BOOL SetInTimeData_DiffSpan_TLine(TLineBase& pTLine,std::string exchangeID);
	//struct tm GetTimeInSpan_TLine(struct tm tmPara, int nSpan,vector<struct tm>& vecTimeScales);
	int GetDayOfYear(struct tm tm1);
	BOOL SaveKline2DB(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase,std::string& exchangeID);
	//BOOL SaveKLineData(string strName,KLineBase& pKLine, int nErrorCode);
	BOOL UpdateKLineData(string& strInstrumentID, EnumPhrase phrase, KLineBase& pKLine, bool bInsert, int& nErrorCode);

	BOOL SaveKline2Cache(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase);


	

	


private:
    static CPlatformCTPQuotImpl* m_pInstance;

    Ceasymutex      m_mutexImpl;
	Ceasymutex      m_mutexKLine;//k�߼������
	Ceasymutex      m_mutexKLineM1,m_mutexKLineM5,m_mutexKLineM15,m_mutexKLineM30,m_mutexKLineM60,m_mutexKLineDay,m_mutexKLineWeek,m_mutexKLineMonth;

    volatile int    m_nQuit;
    queue<PlatformStru_DepthMarketData>      m_queueQuot; //���ﱣ��ÿ������������ݣ�ÿ10���Ӵ�һ�����ݿ�
	queue<PlatformStru_DepthMarketData>      m_queueForKline; //����K�߼����queue
    map<string, set<SOCKET>>    m_SubscribedInstrumentID2Socket;//��ǰ��������ĺ�Լ��socket��ϵ
    map<SOCKET, set<string>>    m_SendedSocket2Inst;//�����Ѿ����͹���Socket��������CTP�Ͽ��������Ӻ��ظ����͡�
	std::set<SOCKET> m_ValidSockets;
	vector<sLoginReq>  m_vecUsers;//�û���¼������Ϣ
	map<string, set<SOCKET>>    m_AppSubscribedInstrumentID2Socket;
	
	


    //�̲߳���
    HANDLE	m_hThread;
    DWORD	m_idThread; 
	HANDLE	m_hThreadKLine;
	DWORD	m_idThreadKLine; 

    BrokerInfo m_brokerInfo;
    std::map<std::string, std::vector<std::string>> m_mapBrokerCode2AddrParam; 
	map<std::string,PlatformStru_InstrumentInfo>   m_mapIns;
	std::map<std::string,std::vector<struct tm>> m_mapExchangeTime;			//������������ʱ�� С��������
	std::map<std::string,std::vector<TLineBase>>  m_mapTLine;
	std::map<std::string,std::vector<KLineBase>>		m_mapKLineMin1;         //���к�Լһ����K��
	std::map<std::string,std::vector<KLineBase>>		m_mapKLineMin5;         //���к�Լ5����K��
	std::map<std::string,std::vector<KLineBase>>		m_mapKLineMin15;         //���к�Լ15����K��
	std::map<std::string,std::vector<KLineBase>>		m_mapKLineMin30;         //���к�Լ30����K��
	std::map<std::string,std::vector<KLineBase>>		m_mapKLineMin60;         //���к�Լ60����K��
	std::map<std::string,std::vector<KLineBase>>		m_mapKLineDay;         //���к�Լ��K��
	std::map<std::string,std::vector<KLineBase>>		m_mapKLineWeek;         //���к�Լ��K��
	std::map<std::string,std::vector<KLineBase>>		m_mapKLineMonth;         //���к�Լ��K��
	//std::map<std::string,std::map<time_t, KLineBase>>		m_mapKLineMin1;         //���к�Լһ����K��
    std::map<std::string,KLineBase>			  m_mapKLineLast;
	std::map<std::string,TLineBase>			  m_mapTLineLast;				//���һ���ֵ�ʵʱ����
	map<std::string,KLineBase >                 m_maptmLast1;//��һ��K��
	map<std::string,KLineBase >                 m_maptmLast5;
	map<std::string,KLineBase >                 m_maptmLast15;
	map<std::string,KLineBase >                 m_maptmLast30;
	map<std::string,KLineBase >                 m_maptmLast60;
	map<std::string,KLineBase >                 m_maptmLastDay;
	map<std::string,KLineBase >                 m_maptmLastWeek;
	map<std::string,KLineBase >                 m_maptmLastMonth;

	std::set<std::string>					m_setTableName;		// �Ѵ���KLine�洢�������

	//��ʷ�ͽ�������	
	std::map<std::string,map<time_t,KLineBase>>		m_vmapKLineMin1;         //���к�Լһ����K��
	std::map<std::string,map<time_t,KLineBase>>		m_vmapKLineMin5;         //���к�Լ5����K��
	std::map<std::string,map<time_t,KLineBase>>		m_vmapKLineMin15;         //���к�Լ15����K��
	std::map<std::string,map<time_t,KLineBase>>		m_vmapKLineMin30;         //���к�Լ30����K��
	std::map<std::string,map<time_t,KLineBase>>		m_vmapKLineMin60;         //���к�Լ60����K��
	std::map<std::string,map<time_t,KLineBase>>		m_vmapKLineDay;         //���к�Լ��K��
	std::map<std::string,map<time_t,KLineBase>>		m_vmapKLineWeek;         //���к�Լ��K��
	std::map<std::string,map<time_t,KLineBase>>		m_vmapKLineMonth;         //���к�Լ��K��

    
public:
    void OnRtnDepthMarketDataSimulate(PlatformStru_DepthMarketData *pDepthMarketData); 
    void SetLastQuot(const PlatformStru_DepthMarketData& data);
    bool GetLastQuot(const string& InstrumentID, PlatformStru_DepthMarketData& outData);
	std::string MakeTableName(string& strInstrumentID, EnumPhrase phrase);

	bool RecalcWeekMonthKLine();

	//������ʷ����
	void SetKLineMin1(std::map<std::string,map<time_t,KLineBase>> mapKLine);
	void SetKLineMin5(std::map<std::string,map<time_t,KLineBase>> mapKLine);
	void SetKLineMin15(std::map<std::string,map<time_t,KLineBase>> mapKLine);
	void SetKLineMin30(std::map<std::string,map<time_t,KLineBase>> mapKLine);
	void SetKLineMin60(std::map<std::string,map<time_t,KLineBase>> mapKLine);
	void SetKLineDay(std::map<std::string,map<time_t,KLineBase>> mapKLine);
	void SetKLineWeek(std::map<std::string,map<time_t,KLineBase>> mapKLine);
	void SetKLineMonth(std::map<std::string,map<time_t,KLineBase>> mapKLine);

	bool GetKLineMin1(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);
	bool GetKLineMin5(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);
	bool GetKLineMin15(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);
	bool GetKLineMin30(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);
	bool GetKLineMin60(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);
	bool GetKLineDay(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);
	bool GetKLineWeek(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);
	bool GetKLineMonth(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);

private:
    map<string,PlatformStru_DepthMarketData> m_lastQuotImpl;	//���ﱣ��ÿ��ģ���Լ����������
	string            m_strTradingDay;//���潻����
	int               m_nTimeSpan;//�գ��ܡ���K�ߴ洢���
	struct           tm m_LastDBTimeDay;
	struct           tm m_LastDBTimeWeek;
	struct           tm m_LastDBTimeMonth;
	struct tm        m_tmCloseTime;
	bool			 m_bHasRecalcWeekMonthKLine;

public:
	time_t m_LoginQuotTime;
	/*static CThostFtdcTraderApi *m_pUserApi;*/
};
