#pragma once
#include "Interface_AccountOp.h"
#include "PlatformDataMgr_Orders.h"
#include "PlatformDataMgr_Trades.h"
#include "PlatformDataMgr_PositionCombs.h"
#include "PlatformDataMgr_PositionDetailCombs.h"
#include "PlatformDataMgr_Positions.h"
#include "PlatformDataMgr_PositionDetails.h"
#include "PlatformDataMgr_TradeTotals.h"
#include "PlatformDataMgr_TradingAccount.h"
#include "SafeLock.h"
#include "intrin.h"



class CSubAccountOp:public IInterfaceAccountOp
{
public:
	CSubAccountOp() 
	{
		
		mpPlatformDataRates = new CPlatformDataMgr_Rates();           //����	
		mpPlatformInstruments = NULL;   //��Լ	
		mpPlatformQutos = NULL;         //����
	
		mpPlatformOrders = NULL;            //����
		mpPlatformTraders = NULL;           //�ɽ�
		mpPlatformTraderTotal = NULL; //�ɽ�����

		mpPlatformPositionDetailCombs = NULL; //��ϳֲֻ���
		mpPlatformPositionsCombs = NULL;            //��ϳֲ�
		mpPlatformPositions = NULL;                     //�ֲ�
		mpPlatformPositionDetail = NULL;          //�ֲ���ϸ

		mpPlatformTraderAccount = NULL;           //�ʽ�	

		meAccountInitStatus = USER_STATUS_UnInit;
		SetAccountCTPStatus(ACCOUNT_STATUS_UnInit);
		ClearAll();
	
		mnLastSaveOrderUpdateSeq = 1;
		mnLastSaveTraderUpdateSeq= 1;
	
		mbaseFTID = mbaseUpdateSeq = 1;
		mbSynFinished = false;
	
	}
	~CSubAccountOp();
public:		
	virtual CPlatformDataMgr_Rates* GetPlatFormRates();
public:

	//���ú�Լ	
	virtual void SetPlatFormInstrument(CPlatformDataMgr_Instruments * npInstrument)
	{
		CSafeLock ls(mMutexData);
		mpPlatformInstruments = npInstrument;	
		SetAccountInitStatus(USER_STATUS_InitQryInstrumentsReady);
	}
	//��������
	virtual void SetPlatFormQutos(CPlatformDataMgr_Quots * npQutos)
	{
		CSafeLock ls(mMutexData);
		mpPlatformQutos = npQutos;
	}

public:
	///���ú�Լ��֤����
	virtual void SetMarginRate(const std::string& InstrumentID,
		PlatformStru_InstrumentMarginRate& outData) ;

	virtual int GetMarginRate(const std::string& InstrumentID, 
		PlatformStru_InstrumentMarginRate& outData) ;

	///���ú�Լ��������
	virtual void SetCommissionRate(const std::string& InstrumentID, 
		PlatformStru_InstrumentCommissionRate& outData) ;

	virtual int GetCommissionRate(const std::string& InstrumentID, 
		PlatformStru_InstrumentCommissionRate& outData) ;


	virtual eCommissionMarginReady GetCommissonMarginReady();

	virtual void SetCommissionMarginReady(const eCommissionMarginReady & nReady) ;

public://�������
	virtual bool CalulateByNewQuto(const PlatformStru_DepthMarketData & nQutosData,bool nbSettlement);

public: //��������

	//��ѯ�����ĵ���
	virtual CF_ERROR QryOrderReach(PlatformStru_OrderInfo& OrderInfo,
		const int ErrID,
		const bool bIsLast);

	//׷��һ������
	virtual CF_ERROR PushOrder( PlatformStru_OrderInfo & nOrder);
	//

	///��ȡָ������
	virtual CF_ERROR GetOrderByKey(const OrderKey &nOrderKey,
		PlatformStru_OrderInfo& outData) ;

	///��ȡȫ������
	virtual CF_ERROR GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData);

	/////��ȡȫ���Ѵ����ı���
	virtual CF_ERROR GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData);

	/////��ȡ�ѳɽ�����
	virtual CF_ERROR GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData);

	/////��ȡ�ѳ����ʹ��󱨵�
	virtual CF_ERROR GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData);

	/////��ȡδ�ɽ�����
	virtual CF_ERROR GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData);

	/////��ȡδ֪״̬�ı���
	virtual CF_ERROR GetUnkownOrders(std::vector<PlatformStru_OrderInfo>& outData) ;

public:// �ɽ�����
	//��ѯ�ɽ��ĵ���
	virtual CF_ERROR QryTradeReach(PlatformStru_TradeInfo& OrderInfo,
		const int ErrID,
		const bool bIsLast) ;
	//׷��һ���ɽ�
	virtual CF_ERROR PushTrader( PlatformStru_TradeInfo & nTrader);

	//��ȡȫ���ɽ��������������������
	virtual CF_ERROR GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData) ;


	//���Գɽ��Ƿ��Ѵ���
	virtual bool TestTradeInfoExist(const TradeKey &key) ;
public://�ֲֻ��ܲ���

	//�ճ���ѯ�ֲֵĵ���
	virtual CF_ERROR  QryPositionReach( 
		PlatformStru_Position& PositionInfo,
		int ErrID,
		bool bIsLast) ;
	//��ȡ�ֲֻ��ܣ����������������
	virtual CF_ERROR  GetPositions(
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData,
		bool bNeedZeroVolumePosition = false) ;



	//�ж��Ƿ��гֲ�
	virtual bool HasPositions() ;

public: //�ֲ���ϸ����

	//�ճ���ѯ�ֲ���ϸ�ĵ���
	virtual CF_ERROR  QryPositionDetailReach( 
		PlatformStru_PositionDetail& PositionDetailInfo,
		int ErrID,
		bool bIsLast) ;
	//��ȡ�ֲ���ϸ�����������������
	virtual CF_ERROR  GetPositionDetail(
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetail>& outData) ;

	//��ȡ�ֲ���ϸ�����������������
	virtual CF_ERROR  GetPositionDetail(const PositionDetailKey & nKey,
		PlatformStru_PositionDetail& outData) ;


	//��ȡĳ��ʱ��εĳֲ���ϸ
	virtual CF_ERROR  GetPositionDetail(int nLowtime,int nUpperTime,
		std::vector<PlatformStru_PositionDetail>& outData) ;



public: //�ʽ����
	virtual CF_ERROR SetQryFundInfo(const PlatformStru_TradingAccountInfo& inData) ;
	//��ȡ����Ա�ʽ���Ϣ
	virtual CF_ERROR  GetFundInfo(PlatformStru_TradingAccountInfo& outData) ;


public:

	virtual void SetLoadOrderUpdateSeq(int nSeq) {if(nSeq > mnLastSaveOrderUpdateSeq) mnLastSaveOrderUpdateSeq = nSeq;};
	virtual void SetLoadTraderUpdateSeq(int nSeq){if(nSeq > mnLastSaveTraderUpdateSeq) mnLastSaveTraderUpdateSeq = nSeq;};

	virtual void GetLoadOrderUpdateSeq(int& nSeq) {nSeq = mnLastSaveOrderUpdateSeq;};
	virtual void GetLoadTraderUpdateSeq(int& nSeq){nSeq = mnLastSaveTraderUpdateSeq;};
	

	virtual void GetOrderAndTradeGeSeq( int nOrderSeq,
		int nTraderSeq,
		std::vector<PlatformStru_OrderInfo> & nvecOrder,
		std::vector<PlatformStru_TradeInfo> & nvecTrader) ;

public: 

	//�����û���CTP����״̬
	virtual CF_ERROR SetAccountCTPStatus(const EnumAccountStatus & nStatus) ;

	//��ȡ�û���CTP����״̬
	//��ȡ�û���CTP����״̬
	//nDiffTime ���ؽ����״̬��ʱ��
	virtual EnumAccountStatus GetAccountCTPStatus(double & nDifftime) ; 	



	//��ȡ�û���ʼ��״̬
	virtual eUserInitStatus GetAccountInitStatus()
	{
		eUserInitStatus leStatus = USER_STATUS_UnInit;
		InterlockedExchange((volatile long*)(&leStatus),meAccountInitStatus);
		return leStatus;
	}

	virtual void SetAccountInitStatus(eUserInitStatus neStatus) 	
	{
		//_InterlockedOr((volatile long*)(&meAccountInitStatus),neStatus);
	}


	virtual void SetEqualAccountInitStatus(eUserInitStatus neStatus) 
	{
		InterlockedExchange((volatile long*)(&meAccountInitStatus),neStatus);		
	}
	virtual void clear() ;

public://���ܲ���
	//��ȡ���еĳֲ���ϸ
	virtual CF_ERROR  GetAllPositionDetail(
		std::vector<PlatformStru_PositionDetail> & outData) ;

	//��ȡ���еĳֲ�
	virtual CF_ERROR  GetAllPosition(
		std::vector<PlatformStru_Position>& outData) ;

public:
	//������н�������
	virtual void ClearAll() ;

	//����ֲ֣��ֲ���ϸ���ʽ�ͳɽ��Ľ�������
	virtual void ClearTraderAndPosition() ;


public:
	//����ʱ�����е�δ�ɽ������ֳɽ�������˵ı���ȫ����Ϊ�ѳ���
	virtual void CancelAllUnkonwnOrders() ;

	//��ѯ��ɺ󣬸������йҵ���Ӧ�ĳֲֶ�����
	virtual void UpdateQryWaitOrderPosition() ;
public:
	bool IsSyncFinished() {return mbSynFinished;}
	void SetSyncFinished(bool bVal) {mbSynFinished = bVal;}
private: //�����ⲿ�ĵ�ַ	
	CPlatformDataMgr_Instruments *mpPlatformInstruments;   //��Լ	
	CPlatformDataMgr_Quots       *mpPlatformQutos;         //����

	CPlatformDataMgr_Rates *mpPlatformDataRates;           //����

	EnumAccountStatus     meCTPStatus;                    //�û��������˻����ӵ�CTP��״̬
	time_t                mtimeCtp;                           //�����״̬��ʱ��
	Ceasymutex			  mMutexTime;                         //����ʱ�����

	eUserInitStatus meAccountInitStatus;

	eCommissionMarginReady meReady;                       //�����Ƿ�׼������

	CPlatformDataMgr_Orders *mpPlatformOrders;            //����
	CPlatformDataMgr_Trades *mpPlatformTraders;           //�ɽ�
	CPlatformDataMgr_TradeTotals * mpPlatformTraderTotal; //�ɽ�����

	CPlatformDataMgr_PositionDetailCombs * mpPlatformPositionDetailCombs; //��ϳֲֻ���
	CPlatformDataMgr_PositionCombs * mpPlatformPositionsCombs;            //��ϳֲ�
	CPlatformDataMgr_Positions * mpPlatformPositions;                     //�ֲ�
	CPlatformDataMgr_PositionDetails * mpPlatformPositionDetail;          //�ֲ���ϸ

	CPlatformDataMgr_TradingAccount         *mpPlatformTraderAccount;           //�ʽ�

	volatile long mbaseFTID;
	volatile long mbaseUpdateSeq;
private:


	Ceasymutex mMutexData;

private:
	bool mbSynFinished;  

	int mnLastSaveOrderUpdateSeq;
	int mnLastSaveTraderUpdateSeq;


};