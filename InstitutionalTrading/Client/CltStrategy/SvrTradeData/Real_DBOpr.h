
#pragma once
#include "interface_dbOpr.h"

class CRealDBOpr :public IInterfaceDBOpr
{
public:	

	//��������߷���
	//����������򽨱�ͽ��յķ���
	//�������ڣ����ҽ��յķ��������ڣ���׷�ӽ��յķ���
	virtual CF_ERROR CreateMyTableAndPartitions(const std::string & strDate);

	//װ��ĳ�����ڵĺ�Լ
	virtual CF_ERROR LoadTodayInstrument(const std::string & strTime, 
		std::vector<PlatformStru_InstrumentInfo> & nInfo) ;

	//д��Լ
	virtual CF_ERROR WriteIntrument(const std::string & strTime, 
		const std::vector<PlatformStru_InstrumentInfo> & nInfo) ;

	//װ���˻��ķ���
	virtual CF_ERROR LoadTodayCommission(const std::string & nsBrokerID,
		const std::string & nsAccountID,
		const std::string & strTime, 
		std::vector<PlatformStru_InstrumentCommissionRate> & nInfo) ;

	//д����
	virtual CF_ERROR WriteCommission(const std::string & nsBrokerID,
		const std::string & nsAccountID,
		const std::string & strTime, 
		const std::vector<PlatformStru_InstrumentCommissionRate> & nInfo) ;


	//ת���˻��ı�֤��
	virtual CF_ERROR LoadTodayMarginRate(const std::string & nsBrokerID,
		const std::string & nsAccountID,
		const std::string & strTime,
		std::vector<PlatformStru_InstrumentMarginRate> & nInfo) ;

	//д��֤��
	virtual CF_ERROR WriteMarginRate(const std::string & nsBrokerID,
		const std::string & nsAccountID,
		const std::string & strTime, 
		const std::vector<PlatformStru_InstrumentMarginRate> & nInfo) ;

	//д����
	virtual CF_ERROR WriteOrderInfos(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime,
		const std::vector<PlatformStru_OrderInfo> & nInfo,
		bool nbDelete = true) ;

	//д�ɽ�
	virtual CF_ERROR WriteTradeInfos(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime,
		const std::vector<PlatformStru_TradeInfo> & nInfo,
		bool nbDelete = true) ;

	//д�ֲ�
	virtual CF_ERROR WritePositionInfos(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime,
		const std::vector<PlatformStru_Position> & nInfo,
		bool nbDelete = true) ;

	//д�ֲ���ϸ
	virtual CF_ERROR WritePositionDetailInfos(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime,
		const std::vector<PlatformStru_PositionDetail> & nInfo,
		bool nbDelete = true) ;

	//д�ʽ�
	virtual CF_ERROR WriteFundInfos(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime,
		const PlatformStru_TradingAccountInfo & nInfo) ;


	//д�����
	virtual CF_ERROR WriteFundInOut(const sFundInOut & s) ;

	//д��ʼ�ʽ�
	virtual CF_ERROR WriteInitFund(const std::string & nsTableName,
		const std::string & nsUserName,
		const double & ndbVal);

	//д��������
	virtual CF_ERROR WriteQutoInfo(const std::string & nsTableName,
		const std::string & strTime,
		const std::vector<PlatformStru_DepthMarketData> & nInfo) ;


	//װ�ؽ�������
	virtual CF_ERROR LoadQutoInfo(const std::string & nsTableName,
		const std::string & strTime,
		std::vector<PlatformStru_DepthMarketData> & nInfo) ;

	//׷��һ����������
	virtual CF_ERROR AddQutoInfo(const std::string & nsTableName,
		const std::string & strTime,
		const PlatformStru_DepthMarketData & nInfo) ;


	//װ���û��ı���
	virtual CF_ERROR LoadTodayUserOrders(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime, 
		std::vector<PlatformStru_OrderInfo> & nInfo) ;

	//װ���û��ĳɽ�
	virtual CF_ERROR LoadTodayUserTrades(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime, 
		std::vector<PlatformStru_TradeInfo> & nInfo) ;

	//װ���û��ĳֲ�
	virtual CF_ERROR LoadTodayUserPositions(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime, 
		std::vector<PlatformStru_Position> & nInfo) ;

	//װ���û��ĳֲ���ϸ
	virtual CF_ERROR LoadTodayUserPositionDetails(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime, 
		std::vector<PlatformStru_PositionDetail> & nInfo) ;

	//װ���û����ʽ�
	virtual CF_ERROR LoadTodayUserFundInfo(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime, 
		PlatformStru_TradingAccountInfo & nInfo) ;


	//��ѯ�����
	virtual CF_ERROR LoadQryFundInOut(const sQryFundInOut & nQryFund,
		std::vector<sFundInOut> & nvecFund);


	//��ѯ��ʼ��Ȩ��
	virtual CF_ERROR LoadUserInitFund(const std::string & nsTableName,
		const std::string & nsUserName,
		double  & ndbVal);


	virtual CF_ERROR LoadUserSettlementConfirm(const std::string & strValidateTime,
		const std::string & nsUserName,
		std::string & nsConfirmDate,
		std::string & nsConfirmTime) ;

	virtual CF_ERROR AddUserSettlementConfirm(const std::string & strValidateTime,
		const std::string & nsUserName,
		const std::string & nsConfirmDate,
		const std::string & nsConfirmTime) ;


	//װ���û��ı���
	virtual CF_ERROR LoadTodayAccountOrders(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime, 
		std::vector<PlatformStru_OrderInfo> & nInfo) ;

	//װ���û��ĳɽ�
	virtual CF_ERROR LoadTodayAccountTrades(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime, 
		std::vector<PlatformStru_TradeInfo> & nInfo) ;

	//װ���û��ĳֲ�
	virtual CF_ERROR LoadTodayAccountPositions(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime, 
		std::vector<PlatformStru_Position> & nInfo) ;

	//װ���û��ĳֲ���ϸ
	virtual CF_ERROR LoadTodayAccountPositionDetails(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime, 
		std::vector<PlatformStru_PositionDetail> & nInfo) ;

	//װ���û����ʽ�
	virtual CF_ERROR LoadTodayAccountFundInfo(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime, 
		PlatformStru_TradingAccountInfo & nInfo) ;



	//д����
	virtual CF_ERROR WriteAccountOrderInfos(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime,
		const std::vector<PlatformStru_OrderInfo> & nInfo,
		bool nbDelete = true) ;

	//д�ɽ�
	virtual CF_ERROR WriteAccountTradeInfos(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime,
		const std::vector<PlatformStru_TradeInfo> & nInfo,
		bool nbDelete = true) ;

	//д�ֲ�
	virtual CF_ERROR WriteAccountPositionInfos(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime,
		const std::vector<PlatformStru_Position> & nInfo,
		bool nbDelete = true) ;

	//д�ֲ���ϸ
	virtual CF_ERROR WriteAccountPositionDetailInfos(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime,
		const std::vector<PlatformStru_PositionDetail> & nInfo,
		bool nbDelete = true) ;

	//д�ʽ�
	virtual CF_ERROR WriteAccountFundInfos(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime,
		const PlatformStru_TradingAccountInfo & nInfo) ;
};
