
#pragma once
#include "CommonErrorCode.h"
#include "KernelStruct.h"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Rates.h"
#include "CommonStruct.h"



class IInterfaceAccountOp
{
public:	
	
	virtual CPlatformDataMgr_Rates* GetPlatFormRates()= 0;
public:

	//��������
	virtual void SetPlatFormQutos(CPlatformDataMgr_Quots * npQutos)= 0;

	//���ú�Լ
	virtual void SetPlatFormInstrument(CPlatformDataMgr_Instruments * npInstrument)= 0;
	///���ú�Լ��֤����
	virtual  void SetMarginRate(const std::string& InstrumentID,
		PlatformStru_InstrumentMarginRate& outData) = 0;

	virtual int GetMarginRate(const std::string& InstrumentID, 
		PlatformStru_InstrumentMarginRate& outData) = 0;

	///���ú�Լ��������
	virtual void SetCommissionRate(const std::string& InstrumentID, 
		PlatformStru_InstrumentCommissionRate& outData) = 0;

	virtual int GetCommissionRate(const std::string& InstrumentID, 
		PlatformStru_InstrumentCommissionRate& outData) = 0;

	virtual eCommissionMarginReady GetCommissonMarginReady() = 0;

	virtual void SetCommissionMarginReady(const eCommissionMarginReady & nReady) = 0;

public://�������
	virtual bool CalulateByNewQuto(const PlatformStru_DepthMarketData & nQutosData,bool nbSettlement) = 0;

public: //��������

	//��ѯ�����ĵ���
	virtual CF_ERROR QryOrderReach(PlatformStru_OrderInfo& OrderInfo,
		const int ErrID,
		const bool bIsLast) = 0;

	//׷��һ������
	virtual CF_ERROR PushOrder( PlatformStru_OrderInfo & nOrder) = 0;

	///��ȡָ������
	virtual CF_ERROR GetOrderByKey(const OrderKey &nOrderKey,
		PlatformStru_OrderInfo& outData) = 0;

	///��ȡȫ������
	virtual CF_ERROR GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////��ȡȫ���Ѵ����ı���
	virtual CF_ERROR GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////��ȡ�ѳɽ�����
	virtual CF_ERROR GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////��ȡ�ѳ����ʹ��󱨵�
	virtual CF_ERROR GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////��ȡδ�ɽ�����
	virtual CF_ERROR GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData) = 0;


	/////��ȡδ֪״̬�ı���
	virtual CF_ERROR GetUnkownOrders(std::vector<PlatformStru_OrderInfo>& outData) = 0;

public:// �ɽ�����
	//��ѯ�ɽ��ĵ���
	virtual CF_ERROR QryTradeReach(PlatformStru_TradeInfo& OrderInfo,
		const int ErrID,
		const bool bIsLast) = 0;

	//׷��һ���ɽ�
	virtual CF_ERROR PushTrader(PlatformStru_TradeInfo & nTrader) = 0;

	//��ȡȫ���ɽ��������������������
	virtual CF_ERROR GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData) = 0;


	//���Գɽ��Ƿ��Ѵ���
	virtual bool TestTradeInfoExist(const TradeKey &key) = 0;

public://�ֲֻ��ܲ���

	//�ճ���ѯ�ֲֵĵ���
	virtual CF_ERROR  QryPositionReach( 
		PlatformStru_Position& PositionInfo,
		int ErrID,
		bool bIsLast) = 0;
	//��ȡ�ֲֻ��ܣ����������������
	virtual CF_ERROR  GetPositions(
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData,
		bool bNeedZeroVolumePosition = false) = 0 ;


	//�ж��Ƿ��гֲ�
	virtual bool HasPositions() = 0;

public: //�ֲ���ϸ����

	//�ճ���ѯ�ֲ���ϸ�ĵ���
	virtual CF_ERROR  QryPositionDetailReach( 
		PlatformStru_PositionDetail& PositionDetailInfo,
		int ErrID,
		bool bIsLast) = 0;
	//��ȡ�ֲ���ϸ�����������������
	virtual CF_ERROR  GetPositionDetail(
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetail>& outData) = 0;

	//��ȡ�ֲ���ϸ�����������������
	virtual CF_ERROR  GetPositionDetail(const PositionDetailKey & nKey,
		PlatformStru_PositionDetail& outData) = 0;


	//��ȡĳ��ʱ��εĳֲ���ϸ
	virtual CF_ERROR  GetPositionDetail(int nLowtime,int nUpperTime,
		std::vector<PlatformStru_PositionDetail>& outData)  = 0;


	
public: //�ʽ����
	virtual CF_ERROR SetQryFundInfo(const PlatformStru_TradingAccountInfo& inData) =0 ;
	//��ȡ����Ա�ʽ���Ϣ
	virtual CF_ERROR  GetFundInfo(PlatformStru_TradingAccountInfo& outData) = 0;

public:

	//�����û���CTP����״̬
	virtual CF_ERROR SetAccountCTPStatus(const EnumAccountStatus & nStatus) = 0;

	//��ȡ�û���CTP����״̬
	//��ȡ�û���CTP����״̬
	//nDiffTime ���ؽ����״̬��ʱ��
	virtual EnumAccountStatus GetAccountCTPStatus(double & nDifftime) = 0; 	
	//��ȡ�û���ʼ��״̬
	virtual eUserInitStatus GetAccountInitStatus() = 0;

	//�����û���ʼ��״̬
	virtual void SetAccountInitStatus(eUserInitStatus neStatus) = 0;
	virtual void SetEqualAccountInitStatus(eUserInitStatus neStatus) = 0;

	virtual void clear() = 0;

public://���ܲ���
	//��ȡ���еĳֲ���ϸ
	virtual CF_ERROR  GetAllPositionDetail(
		std::vector<PlatformStru_PositionDetail> & outData) = 0;

	//��ȡ���еĳֲ�
	virtual CF_ERROR  GetAllPosition(
		std::vector<PlatformStru_Position>& outData) = 0;

public:
	//������н�������
	virtual void ClearAll() = 0;

	//����ֲ֣��ֲ���ϸ���ʽ�ͳɽ��Ľ�������
	virtual void ClearTraderAndPosition() = 0;


public:
	//����ʱ�����е�δ�ɽ������ֳɽ�������˵ı���ȫ����Ϊ�ѳ���
	virtual void CancelAllUnkonwnOrders() = 0;

	//��ѯ��ɺ󣬸������йҵ���Ӧ�ĳֲֶ�����
	virtual void UpdateQryWaitOrderPosition() = 0;

public:
	virtual void SetLoadOrderUpdateSeq(int nSeq)= 0;
	virtual void SetLoadTraderUpdateSeq(int nSeq)= 0;

	virtual void GetLoadOrderUpdateSeq(int& nSeq)= 0;
	virtual void GetLoadTraderUpdateSeq(int& nSeq)= 0;

	virtual void GetOrderAndTradeGeSeq( int nOrderSeq,
		int nTraderSeq,
		std::vector<PlatformStru_OrderInfo> & nvecOrder,
		std::vector<PlatformStru_TradeInfo> & nvecTrader) = 0;

	virtual bool IsSyncFinished()  = 0;
	virtual void SetSyncFinished(bool bVal) = 0;

};