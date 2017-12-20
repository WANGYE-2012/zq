#pragma once
#include "Interface_UserOp.h"
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

class CSubUserOp:public IInterfaceUserOp
{
public:
	CSubUserOp() 
	{
		mpPlatformInstruments = NULL;   //��Լ	
		mpPlatformQutos = NULL;         //����
		mpPlatformDataRates = NULL;           //����

		mpPlatformOrders = NULL;            //����
		mpPlatformTraders = NULL;           //�ɽ�
		mpPlatformTraderTotal = NULL; //�ɽ�����

		mpPlatformPositionDetailCombs = NULL; //��ϳֲֻ���
		mpPlatformPositionsCombs = NULL;            //��ϳֲ�
		mpPlatformPositions = NULL;                     //�ֲ�
		mpPlatformPositionDetail = NULL;          //�ֲ���ϸ

		mpPlatformTraderAccount = NULL;           //�ʽ�	
		SetUserCTPStatus(ACCOUNT_STATUS_UnInit);
		meUserInitStatus = USER_STATUS_UnInit;
		meSettlementConfirmedReady = DataReday_NoReady;
		mdbFirstFund = 0.0f;
		mbQryOrderLast = false;
		mbQryTradeLast = false;  

	
		mnLastSaveOrderUpdateSeq = 1;
		mnLastSaveTraderUpdateSeq= 1;
	
		mbaseFTID = mbaseUpdateSeq = 1;
		mbSynFinished = false;
	}
	~CSubUserOp();
public:
	virtual void SetPlatFormInstrument(CPlatformDataMgr_Instruments * npInstrument)
	{
		CSafeLock ls(mMutexData);
		mpPlatformInstruments = npInstrument;	
		SetUserInitStatus(USER_STATUS_InitQryInstrumentsReady);
	}
	virtual void SetPlatFormQutos(CPlatformDataMgr_Quots * npQutos)
	{
		CSafeLock ls(mMutexData);
		mpPlatformQutos = npQutos;
	}
	virtual void SetPlatFormRates(CPlatformDataMgr_Rates * npRates)
	{
		CSafeLock ls(mMutexData);
		SetUserInitStatus(USER_STATUS_InitQryRatesReady);
		mpPlatformDataRates = npRates;
		if(mpPlatformOrders != NULL)
		{			
			return;
		}

		//InterlockedExchange((volatile long*)(&meUserInitStatus),USER_STATUS_InitQryRatesReady);
		SetUserInitStatus(USER_STATUS_InitQryRatesReady);


		if(mpPlatformInstruments  && mpPlatformDataRates && mpPlatformQutos )
		{
			//��ʱ��Լ
			//mpPlatformQutos = new CPlatformDataMgr_Quots(*mpPlatformInstruments);

			mpPlatformOrders = new CPlatformDataMgr_Orders(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
				*mpPlatformDataRates,
				*mpPlatformQutos,3,3,true);
			mpPlatformTraders = new CPlatformDataMgr_Trades(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
				*mpPlatformDataRates);
			mpPlatformTraderTotal = new CPlatformDataMgr_TradeTotals(mbaseFTID,mbaseUpdateSeq,*mpPlatformTraders);


			mpPlatformPositionDetailCombs = new CPlatformDataMgr_PositionDetailCombs(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
				*mpPlatformQutos,			
				*mpPlatformTraders,
				*mpPlatformOrders);

			mpPlatformPositionDetail = new CPlatformDataMgr_PositionDetails(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
				*mpPlatformDataRates,
				*mpPlatformQutos,
				*mpPlatformTraders,
				*mpPlatformOrders,
				*mpPlatformPositionDetailCombs,3,0,1,0,0,1);

			mpPlatformPositions = new CPlatformDataMgr_Positions(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
				*mpPlatformDataRates,
				*mpPlatformQutos,
				*mpPlatformTraders,
				*mpPlatformOrders,
				*mpPlatformPositionDetail);

			mpPlatformPositionsCombs = new CPlatformDataMgr_PositionCombs(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
				*mpPlatformQutos,
				*mpPlatformTraders,
				*mpPlatformOrders,
				*mpPlatformPositionDetailCombs,
				*mpPlatformPositionDetail);

			mpPlatformTraderAccount = new CPlatformDataMgr_TradingAccount(*mpPlatformInstruments,
				*mpPlatformQutos,
				*mpPlatformTraders,	
				*mpPlatformOrders,	
				*mpPlatformPositionDetailCombs,
				*mpPlatformPositionDetail,
				*mpPlatformPositionsCombs,
				*mpPlatformPositions);
		}
	}

public:
	//���鵽������ļ���
	virtual bool CalulateByNewQuto(const  PlatformStru_DepthMarketData & nQutosData,bool nbSettlement);
public:
	virtual int GetMarginRate(const std::string& InstrumentID, 
		PlatformStru_InstrumentMarginRate& outData) ;

	virtual int GetCommissionRate(const std::string& InstrumentID, 
		PlatformStru_InstrumentCommissionRate& outData) ;

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

	//��鱨������Ƿ������ǵĽ��׿ͻ����µ�
	virtual bool CheckTradeOrderSysIDIsOwn(const std::string & nsOrderSysId) ;
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


	//��ȡ�ֲֻ��ܣ����������������
	virtual CF_ERROR  GetPositionVolume(
		const std::string & nInstrumentID,
		int& nVolume) ;

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

public: //��ϳֲֻ��ܲ���
	//��ȡ��ϳֲ֣����������������
	virtual CF_ERROR  GetPositionComb(
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData) ;

public: //��ϳֲ���ϸ����

	//��ȡ��ϳֲ���ϸ�����������������
	virtual CF_ERROR  GetPositionCombDetail(
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetailComb>& outData) ;

public: //�ʽ����
	virtual CF_ERROR SetQryFundInfo(const PlatformStru_TradingAccountInfo& inData) ;
	//��ȡ����Ա�ʽ���Ϣ
	virtual CF_ERROR  GetFundInfo(PlatformStru_TradingAccountInfo& outData) ;
	//�����
	virtual CF_ERROR UserFundInOut(const sFundInOut& nFundInOut) ;

public://���ܲ���

	//��ȡ���еĳֲ���ϸ
	virtual CF_ERROR  GetAllPositionDetail(
		std::vector<PlatformStru_PositionDetail> & outData) ;

	//��ȡ���еĳֲ�
	virtual CF_ERROR  GetAllPosition(
		std::vector<PlatformStru_Position>& outData) ;
public:
	//�����ʱ�����¼���ɽ�
	virtual CF_ERROR RecalculateTrade(const std::vector<PlatformStru_TradeInfo>& outData,
		bool bUserInputTrader ) ;
public:
	//�����û���CTP����״̬
	virtual CF_ERROR SetUserCTPStatus(const EnumAccountStatus & nStatus) 
	{
		InterlockedExchange((volatile long*)(&meUserCTPStatus),nStatus);
		mMutexTime.lock();		
		time(&mtimeCtp);	
		mMutexTime.unlock();
		return CF_ERROR_SUCCESS;
	}

	//��ȡ�û���CTP����״̬
	//nDiffTime ���ؽ����״̬��ʱ��
	virtual EnumAccountStatus GetUserCTPStatus(double & nDifftime) 
	{
		EnumAccountStatus leStatus = ACCOUNT_STATUS_UnInit;
		InterlockedExchange((volatile long*)(&leStatus),meUserCTPStatus);
		mMutexTime.lock();
		time_t lTime;
		time(&lTime);	
		nDifftime = difftime(lTime, mtimeCtp);
		mMutexTime.unlock();
		return leStatus;
	}

	//��ȡ�û���ʼ��״̬
	virtual eUserInitStatus GetUserInitStatus()
	{
		eUserInitStatus leStatus = USER_STATUS_UnInit;
		InterlockedExchange((volatile long*)(&leStatus),meUserInitStatus);
		return leStatus;
	}

	virtual void SetUserInitStatus(eUserInitStatus neStatus) 	
	{
		_InterlockedOr((volatile long*)(&meUserInitStatus),neStatus);
	}


	virtual void SetEqualUserInitStatus(eUserInitStatus neStatus) 
	{
		InterlockedExchange((volatile long*)(&meUserInitStatus),neStatus);		
	}
public:
	virtual void ClearAll();
	//����ֲ֣��ֲ���ϸ���ʽ�ͳɽ��Ľ�������
	virtual void ClearTraderAndPosition() ;
public:
	//��ȡ�ճ���������
	virtual CF_ERROR SetInitFund(const double & ndbVal) ;
	//��ȡ��ʼ��Ȩ��
	virtual CF_ERROR GetFirstFund(double & ndbVal);

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
	//���㵥�Ƿ�ȷ��
	virtual bool IsSettlementComfirmed(const std::string & nsValidateDate,
		std::string & nsConfirmedDate,
		std::string & nsConfirmedTime) 
	{
		CSafeLock ll(mMutexData);
		eDataReady leDataReady = DataReday_NoReady;
		InterlockedExchange((volatile long*)(&leDataReady),meSettlementConfirmedReady);

		
		nsConfirmedDate = msConfirmDate;
		nsConfirmedTime = msConfirmTime;

		return leDataReady == DataReady_Ready ;
	}

	//���ý��㵥�Ƿ�ȷ��
	virtual void SetSettlementConfirmed(bool  nbVal,
		const std::string & nsValidateDate,
		const std::string & nsConfirmedDate,
		const std::string & nsConfirmedTime) 
	{
		CSafeLock ll(mMutexData);
		eDataReady leDataReady = nbVal ? DataReady_Ready : DataReday_NoReady;
		InterlockedExchange((volatile long*)(&meSettlementConfirmedReady),leDataReady);	
		msConfirmDate = nsConfirmedDate;
		msConfirmTime = nsConfirmedTime;
	}

	bool IsSyncFinished() {
		bool lbVal = false;
		CSafeLock ll(mMutexData);
		lbVal = mbSynFinished;
		return lbVal;
	}
	void SetSyncFinished(bool bVal) {
		CSafeLock ll(mMutexData);
		mbSynFinished = bVal;
	}
public:
	//����ʱ�����е�δ�ɽ������ֳɽ�������˵ı���ȫ����Ϊ�ѳ���
	virtual void CancelAllUnkonwnOrders() ;


	//��ѯ��ɺ󣬸������йҵ���Ӧ�ĳֲֶ�����
	virtual void UpdateQryWaitOrderPosition() ;

private: //�����ⲿ�ĵ�ַ
	CPlatformDataMgr_Instruments *mpPlatformInstruments;   //��Լ	
	CPlatformDataMgr_Quots       *mpPlatformQutos;         //����
	CPlatformDataMgr_Rates *mpPlatformDataRates;           //����

	EnumAccountStatus     meUserCTPStatus;                    //�û��������˻����ӵ�CTP��״̬
	time_t                mtimeCtp;                           //�����״̬��ʱ��
	Ceasymutex			  mMutexTime;                         //����ʱ�����

	eDataReady            meSettlementConfirmedReady;         //���㵥�Ƿ�ȷ��
	std::string           msConfirmDate;                      //������
	std::string			  msConfirmTime;                      //��������


	eUserInitStatus     meUserInitStatus;                    //�û����ݳ�ʼ��״̬
	double              mdbFirstFund;                        //��ʼ��Ȩ�棬���и��˻��������ܺ�
	bool                mbQryOrderLast;                      //�Ƿ����һ����������
	bool                mbQryTradeLast;                      //�Ƿ����һ���ɽ�����
private:
	volatile long mbaseFTID;
	volatile long mbaseUpdateSeq;
	CPlatformDataMgr_Orders *mpPlatformOrders;            //����
	CPlatformDataMgr_Trades *mpPlatformTraders;           //�ɽ�
	CPlatformDataMgr_TradeTotals * mpPlatformTraderTotal; //�ɽ�����

	CPlatformDataMgr_PositionDetailCombs * mpPlatformPositionDetailCombs; //��ϳֲֻ���
	CPlatformDataMgr_PositionCombs * mpPlatformPositionsCombs;            //��ϳֲ�
	CPlatformDataMgr_Positions * mpPlatformPositions;                     //�ֲ�
	CPlatformDataMgr_PositionDetails * mpPlatformPositionDetail;          //�ֲ���ϸ

	CPlatformDataMgr_TradingAccount         *mpPlatformTraderAccount;           //�ʽ�

	Ceasymutex mMutexData;


private:
	bool mbSynFinished;	
	int mnLastSaveOrderUpdateSeq;
	int mnLastSaveTraderUpdateSeq;
	
};