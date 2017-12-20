#pragma once

#include "Interface_SvrDBOpr.h"

#include <comutil.h>
#include <vector>
#include "FileOpr.h"
#pragma comment(lib, "comsuppw.lib") 

#include "occi.h"

class CWriteLog;
#if 0
#define RISKDATA_LOG(fmt, ...)  ;
#else
#define RISKDATA_LOG(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","RiskDataTemporaryStorage", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif
struct ColumeData
{
	oracle::occi::Type eColumeType;
	int nColumeMaxLen;
	int nOffset;
};

struct ColumeDataEx
{
	oracle::occi::Type eColumeType;
	int nColumeMaxLen;
	int nOffset;
	std::string msColumeName;
};

struct DeleteStruct
{
	InstrumentIDType InstrumentID;
	TThostFtdcExchangeIDType ExchangeID;
};

class CSvrDBOprImpl : public CInterface_SvrDBOpr
{
public:
	static CSvrDBOprImpl& getObj(void);
	//static void deleteObj(void);

	void InitDB(const std::string& strUserName, const std::string& strPwd, const std::string& strDBName);
	bool Conncect();
	static int NotifyFn(oracle::occi::Environment *env, oracle::occi::Connection *conn, void *ctx,
		oracle::occi::Connection::FailOverType foType, 
		oracle::occi::Connection::FailOverEventType foEvent);

public:
	//ִ�и��¡�ɾ��SQL��䣬����Ӱ��ļ�¼����
	bool ExcuteUpdate( const char* pSql, int& nNum, int& nErrorCode );
	//ִ������SQL���
	bool Excute( const char* pSql, int& nErrorCode );
	//ִ�в�ѯSQL��䣬���ز�ѯ�ļ�¼����
	bool ExcuteSelect( const char* pSql, int& nRecordNum, int& nErrorCode );
	//����һ�����ݣ������ز����¼��ID
	bool InsertAndReturnID( const char* pSql, const char* pIDSql, int& nPKID, int& nErrorCode );
	//ִ�в�ѯSQL��䣬�����ؽ����
	bool QueryData(const char* pSql, std::vector<std::vector<_variant_t>>& vec, int& nErrorCode );

	//����Ȩ��
	bool SaveRolePrivilegeRelation(int nRoleID, std::vector<int>& vecPrivilegeID, int& nErrorCode);

	//�����û���ɫ
	bool SaveUserRole( int nProductID, std::vector<int>& vTraderID, int& nErrorCode );

    //������Ʋ�Ʒ�ͽ���Ա��ϵ
    bool SaveProductTraderRelation(int nRelationType, int nID, std::vector<int>& vID, int& nErrorCode );

    //���潻�ײ��Ժ���֯������ϵ
    bool SaveStrategyOrganizationRelation(int nStrategyID, std::vector<int>& vOrgID, int& nErrorCode );

    //�����û�����֯������ϵ
    bool SaveUserOrganizationRelation( int nOrgID, int nUserType, int nUserID, int nRelationType, int& nErrorCode);
    bool SaveUserOrganizationRelation( int nOrgID, int nUserType, std::vector<int>& vInID, std::vector<int>& vOutID, int nRelationType, int& nErrorCode);
    bool SaveUserOrganizationRelation( int nRiskID, int nAttachOrg, std::vector<int>& vMonitorOrg, int& nErrorCode);


	//������
	bool SaveSettlementPrice2DB(std::string strTime, std::vector<SettlementPriceField>& vSettlementPrice ,int& nErrorCode);

	//�����Լ
	bool SaveInstruments(
		const std::string& strTime, 
		const std::vector<PlatformStru_InstrumentInfo>& nInstruemnts ,
		int& nErrorCode ,
		const std::string& nsTableName);


	//�������
	virtual bool SaveAccountCommissionRate(
		const std::string& nsTableName,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_InstrumentCommissionRate>& nCommission ,
		int& nErrorCode ) ;


	//��֤����
	virtual bool SaveAccountMarginRate(
		const std::string& nsTableName,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_InstrumentMarginRate>& nMargins ,
		int& nErrorCode ) ;



	//����
	virtual bool SaveUserOrderInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsUserName,
		const std::vector<PlatformStru_OrderInfo>& nMargins ,
		int& nErrorCode ) ;
	//�ɽ�
	virtual bool SaveUserTraderInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsUserName,
		const std::vector<PlatformStru_TradeInfo>& nMargins ,
		int& nErrorCode ) ;

	//�ֲ�
	virtual bool SaveUserPositionInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsUserName,
		const std::vector<PlatformStru_Position>& nPositions ,
		int& nErrorCode );

	//�ֲ���ϸ
	virtual bool SaveUserPositionDetailInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsUserName,
		const std::vector<PlatformStru_PositionDetail>& nPositionDetails ,
		int& nErrorCode );

	//�ʽ�
	virtual bool SaveUserFundInfos(
		const std::string& nsTableName,
		const std::string& strTime,
		const std::string& nsUserName,
		const PlatformStru_TradingAccountInfo& nFundInfos,
		int& nErrorCode );



	//����
	virtual bool SaveAccountOrderInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_OrderInfo>& nMargins ,
		int& nErrorCode ) ;
	//�ɽ�
	virtual bool SaveAccountTraderInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_TradeInfo>& nMargins ,
		int& nErrorCode ) ;

	//�ֲ�
	virtual bool SaveAccountPositionInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_Position>& nPositions ,
		int& nErrorCode );

	//�ֲ���ϸ
	virtual bool SaveAccountPositionDetailInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_PositionDetail>& nPositionDetails ,
		int& nErrorCode );

	//�ʽ�
	virtual bool SaveAccountFundInfos(
		const std::string& nsTableName,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const PlatformStru_TradingAccountInfo& nFundInfos,
		int& nErrorCode );


	//д������¼
	virtual bool SaveFundInOut(const sFundInOut & s);

	//д������
	virtual bool SaveOneTradingDay(const std::string& nsTableName,
		const std::string& strTradingDay,
		int nInitStatus,
		int& nErrorCode );

	//���½�����״̬
	virtual bool UpdateTradingDayInitStatus(const std::string& nsTableName,
		const std::string& strTradingDay,
		int nInitStatus,
		int& nErrorCode ) ;

	//д������
	virtual bool SaveOneSettlementDay(const std::string& nsTableName,
		const std::string& strTradingDay,	
		int& nErrorCode ) ;

	virtual bool SaveDepthMarketDataWithDelete( const std::string& nsTableName,
		const std::string& strTime,
		const std::vector<PlatformStru_DepthMarketData>& vecData, 
		int& nErrorCode );


	//����
	virtual bool MergeOrderInfos(
		const std::string& nsTableName,
		const std::string& strTime,		
		const std::vector<PlatformStru_OrderInfo>& nOrders ,
		int& nErrorCode ) ;

public://����
	virtual bool SaveOrderTransfer(const std::vector<SOrderTransfer>& vecOrderTransfer,	int& nErrorCode );
	virtual bool GetOrderTransfer(std::string strTime,  std::vector<SOrderTransfer>& vecOrderTransfer); 
	virtual bool UpdateOrderTransfer(SOrderTransfer& sOrderTransfer);

public://���
	//�����¼���������Ϣ֪ͨ
	virtual bool QueryData( const char* pSql, std::vector<RiskIndicator>& vec, int& nErrorCode );
	virtual bool QueryData( const char* pSql, std::vector<RiskResponse>& vec, int& nErrorCode );
	virtual bool QueryData( const char* pSql, std::vector<RiskEvent>& vec, int& nErrorCode );
	virtual bool QueryData( const char* pSql, std::vector<MessageInfo>& vec, int& nErrorCode);
	virtual bool QueryData( const char* pSql, std::vector<MsgSendStatus>& vec, int& nErrorCode );
	virtual bool QueryData( const char* pSql, std::vector<NetFundCalcResult>& vec, int& nErrorCode ); 
	virtual bool AddMessage( const MessageInfo& msgInfo, const std::vector<TargetAccount> vAccount, int& nPKID, int& nErrorCode);
	virtual bool AddMessage( const MessageInfo& msgInfo, int& nPKID, int& nErrorCode );
	bool SaveMessageSendStatus( std::vector<MsgSendStatus> vMsgStatus );
	//ֻ��ͬһ����֯������ͬһ�ַ���
	bool InsertRiskPlan(std::vector<RiskPlan>& vecRiskPlan, int& nErrorCode);
	bool GetRiskPlan(const char* pSql, std::vector<RiskPlan>& vecRiskPlan, int& nErrorCode );
	bool QueryData( const char* pSql, std::vector<EventMessageTemplate>& vec );
	bool SaveMsgTemplate( int nIndicatorID, std::vector<EventMessageTemplate>& vec );	
	bool QueryData( const char* pSql, std::vector<NetFundParam>& vec );

	bool SaveRiskEvent( std::string strTime, std::vector<RiskEvent>& vRiskEvent, int& nErrorCode, bool deleteToday = true );
	bool SaveNetFundCalcResult(std::string strTime, std::vector<NetFundCalcResult>& vResult, int& nErrorCode);
	bool SaveDepthMarketData( const std::string& nsTableName,
		const std::string& strTime,
		const std::vector<PlatformStru_DepthMarketData>& vecData, int& nErrorCode );
    bool SaveDepthMarketData_PerDay( const std::string& nsTableName,
        const std::string& strTime,
        const std::vector<PlatformStru_DepthMarketData>& vecData, int& nErrorCode );

	//���ǿƽ�µ���¼ bForceCloseType:true�Զ�ǿƽ��false��ͷǿƽ
	bool SaveForceCloseOrder(PlatformStru_InputOrder& order, InputOrderKey& lKey, bool bForceCloseType, std::string strRiskName, int& nErrorCode);

	//���ǿƽ����
	bool SaveForceCloseOrderAction(PlatformStru_InputOrderAction& order, bool bForceCloseType, std::string strRiskName, int& nErrorCode);
	//д��ʼ�ʽ�
	virtual bool SaveInitFund(const std::string & nsTableName,
		const std::string & nsUserName,
		const double & ndbVal) ;

    //�������ݲ������
    bool WriteLogToDB( const LogBaseData& logData, int& nErrorCode );

	//�ж����ݿ�û��������
	bool IsConnected();
public://����
	void string_replace( std::string &strBig, const std::string &strsrc, const std::string &strdst );
	void GetFolderFileName(std::string& strFilePath,const std::string& folder);
	void ReadBlobToFile(oracle::occi::Blob& blob, std::string strFileName);
	void WriteFileToBlob(oracle::occi::Blob& blob, char *pData, int nLength);

	bool DB_GetUserStrategys(std::vector<SStrategy>& vecStrategy, std::string strUser ="");
	bool DB_AddStrategy(const SStrategy& strategy, int& nErrorCode );
	bool DB_DelStrategy(std::string strName, int& nErrorCode );
	bool DB_SetStragegyStatus(std::string strName, int nUse, int& nErrorCode );
	bool DB_WriteStragegyFile(std::string strName, int nType, char *pData, int nLength, int& nErrorCode);//nType:2д��dll�ļ��� nType��3 д��zip�ļ�

	bool DB_AddStrategy2IndexRelation(std::string strName, std::string strIndex, int& nErrorCode);
	bool DB_DelStrategy2IndexRelation(std::string strName, int& nErrorCode);
	bool DB_GetStrategy2IndexRelation(std::vector<SStrategy2Index>& vec);

	bool DB_GetUserInstance(std::vector<SStrategyInstance>& vecInstance, std::string strUser ="");
	bool DB_AddInstance(const SStrategyInstance& strategy, int& nErrorCode);
	bool DB_DelInstance(std::string strName, int& nErrorCode);
	bool DB_DelInstance_ByTrader(std::string strName, std::string strUploader, int& nErrorCode);	
	bool DB_SetInstanceStatus(std::string strName, std::string strUploader, int nUse, int& nErrorCode);

	bool DB_GetUserIndex(std::vector<SIndex>& vecIndex, std::string strUser ="");
	bool DB_AddIndex(const SIndex& strategy, int& nErrorCode );
	bool DB_DelIndex(std::string strName, int& nErrorCode);
	bool DB_WriteIndexFile(std::string strName, int nType, char *pData, int nLength, int& nErrorCode);
	
	bool DB_WriteLogToDB( const LogStrategy& logData, int& nErrorCode );

	//�������ݿ�Ͽ�����ʱ����Ҫ��������� �� �ļ�
	//�����¼�
	void SaveRiskEvent2File( std::vector<RiskEvent>& vRiskEvent);
	//����ֵ
	void SaveNetFundCalcResult2File(std::vector<NetFundCalcResult>& vResult);
	//���ǿƽ�µ���¼
	void SaveForceCloseOrder2File(PlatformStru_InputOrder& order, InputOrderKey& lKey, bool bForceCloseType, std::string strRiskName);
	//���ǿƽ����
	void SaveForceCloseOrderAction2File(PlatformStru_InputOrderAction& order, bool bForceCloseType, std::string strRiskName);
	//�������
	void SaveVertifyOrder2File(SVerisyOrder& order);
	std::string Int2String(int field);
	std::string Double2String(double field);
	//��̬����ÿ�պ�Լ��
	bool CreatInsTable_PreDay(const std::string& nsTableName,
		int& nErrorCode );
    //��̬����ÿ�������
	bool CreatDepthMarketTable_PerDay(const std::string& nsTableName);
	//��̬����K�߱�
	bool CreatKLineTable_PerDay(const std::string& strDate);

    
    bool SaveStockQuot( const int nType,
                        const std::string& nsTableName,
                        const std::string& strTime,
                        const vector< PlatformStru_DepthMarketData >& vData,
                        int& nErrorCode );
	

/*	bool IsStrategyExist(std::string strName);	
	bool UpdateStrategy(SStrategy& strategy, int nType);
	bool SelectStrategy(std::vector<SStrategy>& vecStrategy);
	
	void GetFolderFileName(std::string& strFilePath,const std::string& folder);
	void ReadBlobToFile(oracle::occi::Blob& blob, std::string strFileName);

	void WriteFileToBlob(oracle::occi::Blob& blob, char *pData, int nLength);

	//ָ��
	bool IsIndexExist(std::string strName);
	bool SaveIndex(SStrategyInstance& strategy, int& nErrorCode );
	bool UpdateIndex(SStrategyInstance& strategy, int nType);
	bool SelectIndex(std::vector<SStrategyInstance>& vecStrategy);
	*/
private:
	template <class T>
	bool BatchInsert(							
		const std::string& nsTableName,
		const std::string& nDeleteSql,
		const std::string& nValidateDate,
		std::vector<ColumeData>& nVecColume ,
		const std::vector<T>& nVecData ,
		int& nErrorCode );

    template <class T>
    bool BatchInsert_PerDay(							
        const std::string& nsTableName,
        const std::string& nDeleteSql,
        const std::string& nValidateDate,
        std::vector<ColumeData>& nVecColume ,
        const std::vector<T>& nVecData ,
        int& nErrorCode );

	template <class T>
	bool BatchMerge(							
		const std::string& nsTableName,
		const std::string& nsConditionStr,
		const std::string& nValidateDate,
		std::vector<ColumeDataEx>& nVecColume ,
		const std::vector<T>& nVecData ,
		int& nErrorCode );

	template <class T>
	bool BatchDelete(							
		const std::string& nsTableName,
		const std::string& nsConditionStr,
		std::vector<ColumeDataEx>& nVecColume ,
		const std::vector<T>& nVecData ,
		int& nErrorCode );
	
private:
	
	void DisConnected();
	void RollBack();
	void WriteLog(const std::string& strSql, const std::string& strError);
	int GetErrorCode(const std::string& strError );

private:
	oracle::occi::Environment* m_pEnvironment;
	oracle::occi::Connection * m_pCon;
	oracle::occi::Statement* m_pStmt;
	oracle::occi::ResultSet* m_pRes;

	std::string m_strUserName;
	std::string m_strPwd;
	std::string m_strDBName;
	volatile bool m_bConn;
	CWriteLog* m_pWriteLog;

private:
	CSvrDBOprImpl();
	virtual ~ CSvrDBOprImpl();
	static CSvrDBOprImpl* m_pObj;
	int m_nQuotSeq;
};
template<class T>
class CSafeArrayPtr
{
public:
	CSafeArrayPtr(T* p):m_ptr(p){}
	~CSafeArrayPtr(){if(m_ptr) delete[] m_ptr;}
private:
	T* m_ptr;
	
};