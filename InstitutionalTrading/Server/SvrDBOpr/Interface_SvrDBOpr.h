#pragma once

#include "Interface_SvrCommon.h"
#include <comutil.h>
#include <vector>
#include "CommonStruct.h"
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
#include "RiskManageStruct.h"
#pragma comment(lib, "comsuppw.lib") 

#ifndef SVRDBOPR_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRDBOPR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRDBOPR_EXPORTS
#define SVRDBOPR_API __declspec(dllexport)
#else
#define SVRDBOPR_API __declspec(dllimport)
#endif
#endif

class SVRDBOPR_API CInterface_SvrDBOpr : public CInterface_SvrCommon
{
public:
    static CInterface_SvrDBOpr& getObj(void);

	virtual void InitDB(const std::string& strUserName, const std::string& strPwd, const std::string& strDBName) = 0;
	virtual bool Conncect() = 0;

	//ִ�и��¡�ɾ��SQL��䣬����Ӱ��ļ�¼����
	virtual bool ExcuteUpdate( const char* pSql, int& nNum, int& nErrorCode ) = 0;
	//ִ������SQL���
	virtual bool Excute( const char* pSql, int& nErrorCode ) = 0;
	//ִ�в�ѯSQL��䣬���ز�ѯ�ļ�¼����
	virtual bool ExcuteSelect( const char* pSql, int& nRecordNum, int& nErrorCode ) = 0;
	//����һ�����ݣ������ز����¼��ID
	virtual bool InsertAndReturnID( const char* pSql, const char* pIDSql, int& nPKID, int& nErrorCode ) = 0;
	//ִ�в�ѯSQL��䣬�����ؽ����
	virtual bool QueryData(const char* pSql, std::vector<std::vector<_variant_t>>& vec, int& nErrorCode ) = 0;

	//����Ȩ��
	virtual bool SaveRolePrivilegeRelation(int nRoleID, std::vector<int>& vecPrivilegeID, int& nErrorCode) = 0;
	
	//�����û���ɫ
	virtual bool SaveUserRole( int nUserID, std::vector<int>& vRoleID, int& nErrorCode ) = 0;

    //������Ʋ�Ʒ�ͽ���Ա��ϵ
    virtual bool SaveProductTraderRelation( int nRelationType, int nID, std::vector<int>& vID, int& nErrorCode ) = 0;

    //���潻�ײ��Ժ���֯������ϵ
    virtual bool SaveStrategyOrganizationRelation( int nStrategyID, std::vector<int>& vOrgID, int& nErrorCode ) = 0;

    //�����û�����֯������ϵ
    virtual bool SaveUserOrganizationRelation( int nOrgID, int nUserType, int nUserID, int nRelationType, int& nErrorCode) = 0;
    virtual bool SaveUserOrganizationRelation( int nOrgID, int nUserType, std::vector<int>& vInID, std::vector<int>& vOutID, int nRelationType, int& nErrorCode) = 0;
    virtual bool SaveUserOrganizationRelation( int nRiskID, int nAttachOrg, std::vector<int>& vMonitorOrg, int& nErrorCode) = 0;

	//������
	virtual bool SaveSettlementPrice2DB(std::string strTime, std::vector<SettlementPriceField>& vSettlementPrice ,int& nErrorCode)=0;

	//�����Լ
	virtual bool SaveInstruments(const std::string& strTime, 
		const std::vector<PlatformStru_InstrumentInfo>& nInstruemnts ,
		int& nErrorCode ,
		const std::string& nsTableName ="TRADEDATA_INSTRUEMENTS") = 0;
	//��̬����ÿ�պ�Լ��
	virtual bool CreatInsTable_PreDay(const std::string& nsTableName,
		int& nErrorCode ) = 0;

	//�������
	virtual bool SaveAccountCommissionRate(
		const std::string& nsTableName,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_InstrumentCommissionRate>& nCommission ,
		int& nErrorCode ) = 0 ;


	//��֤����
	virtual bool SaveAccountMarginRate(
		const std::string& nsTableName,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_InstrumentMarginRate>& nMargins ,
		int& nErrorCode ) = 0;

	//����
	virtual bool SaveUserOrderInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsUserName,
		const std::vector<PlatformStru_OrderInfo>& nMargins ,
		int& nErrorCode ) = 0;
	//�ɽ�
	virtual bool SaveUserTraderInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsUserName,
		const std::vector<PlatformStru_TradeInfo>& nMargins ,
		int& nErrorCode ) = 0;

	//�ֲ�
	virtual bool SaveUserPositionInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsUserName,
		const std::vector<PlatformStru_Position>& nPositions ,
		int& nErrorCode )= 0;

	//�ֲ���ϸ
	virtual bool SaveUserPositionDetailInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsUserName,
		const std::vector<PlatformStru_PositionDetail>& nPositionDetails ,
		int& nErrorCode )= 0;

	//�ʽ�
	virtual bool SaveUserFundInfos(
		const std::string& nsTableName,
		const std::string& strTime,
		const std::string& nsUserName,
		const PlatformStru_TradingAccountInfo& nFundInfos,
		int& nErrorCode )= 0;


	//����
	virtual bool SaveAccountOrderInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_OrderInfo>& nMargins ,
		int& nErrorCode ) = 0;
	//�ɽ�
	virtual bool SaveAccountTraderInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_TradeInfo>& nMargins ,
		int& nErrorCode ) = 0;

	//�ֲ�
	virtual bool SaveAccountPositionInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_Position>& nPositions ,
		int& nErrorCode )= 0;

	//�ֲ���ϸ
	virtual bool SaveAccountPositionDetailInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_PositionDetail>& nPositionDetails ,
		int& nErrorCode )= 0;

	//�ʽ�
	virtual bool SaveAccountFundInfos(
		const std::string& nsTableName,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const PlatformStru_TradingAccountInfo& nFundInfos,
		int& nErrorCode )= 0;

	//д������¼
	virtual bool SaveFundInOut(const sFundInOut & s) = 0;

	//д������
	virtual bool SaveOneTradingDay(const std::string& nsTableName,
		const std::string& strTradingDay,
		int nInitStatus,
		int& nErrorCode ) = 0;

	//���½�����״̬
	virtual bool UpdateTradingDayInitStatus(const std::string& nsTableName,
		const std::string& strTradingDay,
		int nInitStatus,
		int& nErrorCode ) = 0;

	//д������
	virtual bool SaveOneSettlementDay(const std::string& nsTableName,
		const std::string& strTradingDay,	
		int& nErrorCode ) = 0;

	//����orderfrefת��
	virtual bool SaveOrderTransfer(const std::vector<SOrderTransfer>& vecOrderTransfer ,
		int& nErrorCode ) = 0;

	virtual bool GetOrderTransfer(std::string strTime, std::vector<SOrderTransfer>& vecOrderTransfer) = 0;

	virtual bool UpdateOrderTransfer(SOrderTransfer& sOrderTransfer) = 0;

	//�����¼���������Ϣ֪ͨ
	virtual bool QueryData( const char* pSql, std::vector<RiskIndicator>& vec, int& nErrorCode ) =0;
	virtual bool QueryData( const char* pSql, std::vector<RiskResponse>& vec, int& nErrorCode ) =0;
	virtual bool QueryData( const char* pSql, std::vector<RiskEvent>& vec, int& nErrorCode ) =0;
	virtual bool QueryData( const char* pSql, std::vector<MessageInfo>& vec, int& nErrorCode ) =0;
	virtual bool QueryData( const char* pSql, std::vector<MsgSendStatus>& vec, int& nErrorCode ) =0;
	virtual bool QueryData( const char* pSql, std::vector<NetFundCalcResult>& vec, int& nErrorCode ) =0;

	virtual bool AddMessage( const MessageInfo& msgInfo, const std::vector<TargetAccount> vAccount, int& nPKID, int& nErrorCode) = 0;
	virtual bool AddMessage( const MessageInfo& msgInfo, int& nPKID, int& nErrorCode ) = 0;
    virtual bool SaveMessageSendStatus( std::vector<MsgSendStatus> vMsgStatus ) = 0;

	virtual bool InsertRiskPlan(std::vector<RiskPlan>& vecRiskPlan, int& nErrorCode) = 0;
	virtual bool GetRiskPlan(const char* pSql,std::vector<RiskPlan>& vecRiskPlan, int& nErrorCode ) = 0;
	virtual bool SaveMsgTemplate( int nIndicatorID, std::vector<EventMessageTemplate>& vec ) =0;
	virtual bool QueryData( const char* pSql, std::vector<EventMessageTemplate>& vec ) =0;
	virtual bool QueryData( const char* pSql, std::vector<NetFundParam>& vec ) =0;

	virtual bool SaveRiskEvent( std::string strTime, std::vector<RiskEvent>& vRiskEvent, int& nErrorCode , bool deleteToday = true ) = 0;
	virtual bool SaveNetFundCalcResult(std::string strTime, std::vector<NetFundCalcResult>& vResult, int& nErrorCode)=0;
	//�õ�����ָ������
	
	//���ǿƽ�µ���¼
	virtual bool SaveForceCloseOrder(PlatformStru_InputOrder& order, InputOrderKey& lKey, bool bForceCloseType, std::string strRiskName, int& nErrorCode) = 0;
	//���ǿƽ����
	virtual bool SaveForceCloseOrderAction(PlatformStru_InputOrderAction& order, bool bForceCloseType, std::string strRiskName, int& nErrorCode) =0;

public://����
	virtual	bool DB_GetUserStrategys(std::vector<SStrategy>& vecStrategy, std::string strUser ="") = 0;
	virtual	bool DB_AddStrategy(const SStrategy& strategy, int& nErrorCode ) = 0;
	virtual	bool DB_DelStrategy(std::string strName, int& nErrorCode) = 0;
	virtual	bool DB_SetStragegyStatus(std::string strName, int nUse, int& nErrorCode ) = 0;
	virtual bool DB_WriteStragegyFile(std::string strName, int nType, char *pData, int nLength, int& nErrorCode)=0;//nType:2д��dll�ļ��� nType��3 д��zip�ļ�
	virtual bool DB_AddStrategy2IndexRelation(std::string strName, std::string strIndex, int& nErrorCode) = 0;
	virtual bool DB_DelStrategy2IndexRelation(std::string strName, int& nErrorCode) = 0;
	virtual bool DB_GetStrategy2IndexRelation(std::vector<SStrategy2Index>& vec) = 0;

	virtual	bool DB_GetUserInstance(std::vector<SStrategyInstance>& vecInstance, std::string strUser ="") = 0;
	virtual	bool DB_AddInstance(const SStrategyInstance& strategy, int& nErrorCode) = 0;
	virtual	bool DB_DelInstance(std::string strName, int& nErrorCode) = 0;
	virtual	bool DB_DelInstance_ByTrader(std::string strName, std::string strUploader, int& nErrorCode) = 0;
	virtual	bool DB_SetInstanceStatus(std::string strName, std::string strUploader, int nUse, int& nErrorCode) = 0;

	virtual	bool DB_GetUserIndex(std::vector<SIndex>& vecIndex, std::string strUser ="") = 0;
	virtual	bool DB_AddIndex(const SIndex& strategy, int& nErrorCode ) = 0;
	virtual	bool DB_DelIndex(std::string strName, int& nErrorCode) = 0;
	virtual bool DB_WriteIndexFile(std::string strName, int nType, char *pData, int nLength, int& nErrorCode)=0;

	//�������ݲ������
	virtual bool DB_WriteLogToDB( const LogStrategy& logData, int& nErrorCode ) = 0;


    //���浱������
	virtual bool SaveDepthMarketData( const std::string& nsTableName,
		const std::string& strTime,
		const std::vector<PlatformStru_DepthMarketData>& vecData, int& nErrorCode ) = 0;    

    //���浱������
    virtual bool SaveDepthMarketData_PerDay( const std::string& nsTableName,
        const std::string& strTime,
        const std::vector<PlatformStru_DepthMarketData>& vecData, int& nErrorCode ) = 0;  

	//д��ʼ�ʽ�
	virtual bool SaveInitFund(const std::string & nsTableName,
		const std::string & nsUserName,
		const double & ndbVal) = 0;


	//���������
	virtual bool SaveDepthMarketDataWithDelete( const std::string& nsTableName,
		const std::string& strTime,
		const std::vector<PlatformStru_DepthMarketData>& vecData, 
		int& nErrorCode ) = 0;

    //�������ݲ������
    virtual bool WriteLogToDB( const LogBaseData& logData, int& nErrorCode ) = 0;

	//�ж����ݿ�û��������
	virtual bool IsConnected() = 0;

	//�������ݿ�Ͽ�����ʱ����Ҫ��������� �� �ļ�
	//�����¼�
	virtual void  SaveRiskEvent2File( std::vector<RiskEvent>& vRiskEvent) = 0;
	//����ֵ
	virtual void  SaveNetFundCalcResult2File( std::vector<NetFundCalcResult>& vResult)=0;
	//���ǿƽ�µ���¼
	virtual void  SaveForceCloseOrder2File(PlatformStru_InputOrder& order, InputOrderKey& lKey, bool bForceCloseType, std::string strRiskName) = 0;
	//���ǿƽ����
	virtual void  SaveForceCloseOrderAction2File(PlatformStru_InputOrderAction& order, bool bForceCloseType, std::string strRiskName) =0;
	//�������
	virtual void  SaveVertifyOrder2File(SVerisyOrder& order) =0;
	//��̬����ÿ�������
	virtual bool CreatDepthMarketTable_PerDay(const std::string& nsTableName)=0;
	//��̬����K�߱�
	virtual bool CreatKLineTable_PerDay(const std::string& strDate)=0;


    //��Ʊ��⣬Ŀǰֻ�浱ǰ����Ľ����ա���Ʊ���롢�߿����ռ۸�
    //nType 0���Ͻ���   1�����
    virtual bool SaveStockQuot( const int nType,
                                const std::string& nsTableName,
                                const std::string& strTime,
                                const vector< PlatformStru_DepthMarketData >& vData,
                                int& nErrorCode )=0;
};







