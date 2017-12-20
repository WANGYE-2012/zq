#pragma once
#include "stdafx.h"
#include "CfgMgr/CfgGlobal.h"

/*
1. ����һ��ԭ�Ӳ�����ԭʼ������Ϣ����ת����Ҳֻ��Ӧһ���������󣬲����ڷֽ��ˣ�
2. ת����ʵ�ʱ�����Ϣ
3. һ��ԭ�ӱ�����Ӧһ��ʵ��
*/

// �ṩһ�»�������
// 1.  ��ָ����������OrderKeyָ����صı����ҵ�
// 2.  ��ָ���������µ�����OrderKeyָ����صı����ҵ�
// 3.  �µ�
// 4.  �����±���
// 5.  �����±������޸ļ۸��µ�
// 6.  �����±������޸������µ�
// 7.  �����±������޸ļ۸�������µ�
// 8.  ������ɺ��Զ���ָ������

class COrderAtomEntity
{
public:
	enum {	conModeNewOrder, conModeCancelOnly, conModeReplace, conModeCancelThenOrder };
	enum {	conPriceNone, conPriceMarket, conPriceLimit };//, conPriceUpperLimit, conPriceLowerLimit, 
			//conPriceLast, conPriceAsk1, conPriceBid1 };
	enum {	conVolumeNone, conVolumeCustom, conVolumeRemain, conVolumeRemainIncrement };
	enum {	conNothing, conWaiting, conIsSuccess, conIsFail, 
					conCancelling, conIsCancelSuccess, conIsCancelFail };
	enum {	conStepNothing, conStepOrderSource, conStepOrderChange };


	// ����Ǳ�׼���µ��ṹ�壬��ƽ̨�����
	typedef struct {
		//BOOL	bIsParketOrder;			// �Ƿ�ΪԤ��
		int		nOrderType;				// ��������
		BOOL	bIsBuy;
		int		nOCMode;				// ��ƽ��ģʽ, 0 δ֪��1 ���֣�2 ƽ��3 ƽ��(ƽ��)
		BOOL	bIsHedge;				// FALSE Ͷ����TRUE �ױ�
		BOOL	bIsMarket;				// �Ƿ�Ϊ�м�
		double	fPrice;
		int		nVolume;
		int		nTrigger_PriceType;		// �����۸����ͣ��Ѱ�����nOrderType���ˣ�����ֻ�������������õ�
		int		nTrigger_Condition;		// �����������Ѱ�����nOrderType���ˣ�����ֻ�������������õ�
		BOOL	bTrigger_IsMarket;		// �Ƿ�Ϊ�м�
		double	fTrigger_Price;
		InstrumentIDType		csInstrumentID;		// ��Լ
		CommonExchangeIDType	csExchangeID;		// ������
		CommonUserIDType		csUserID;			// Ͷ����ID��Pats���û���
		CommonAccountType		csAccount;			// ���ʺ�
	} STDORDERREQ, *LPSTDORDERREQ;

public:
	// nMode��0 ��ͨ�µ���1 �ĵ�(ֱ��ʹ������Թҵ������޸�)��2 ��ָ�������µ�
	static COrderAtomEntity* CreateOrderAtomEntity(STDORDERREQ& tOriReq, 
								int nMode, LPVOID pOrderInfo=NULL, 
								BOOL bNoSendVolZoreOrder=FALSE);
	static BOOL ConvertCondToOrderType(int nPriceType, int nCondType, int& nOrderType);

	static BOOL ConvertToAction(PlatformStru_InputOrderAction& actionReq, 
								PlatformStru_OrderInfo& orderInfo);
	static BOOL ConvertToAction(PlatformStru_RemoveParkedOrder& actionReq, 
								CThostFtdcParkedOrderField& orderInfo);
	static wxString MakeRevokeDesciption(PlatformStru_OrderInfo& field);
	
	static BOOL IsParketOrder(STDORDERREQ& stdReq) {
		return stdReq.nOrderType==OrderType_FT_ParkedOrder;
	};
	
	~COrderAtomEntity();

protected:
	COrderAtomEntity(STDORDERREQ& stdReq, int nMode, LPVOID pOrderInfo=NULL, 
			BOOL bNoSendVolZoreOrder=FALSE);

	BOOL SendSrcOrder(BOOL bIsReplace);
	BOOL ChangeOrder(int nPriceMode, double fPrice, 
					int nPriceTriggerMode, double fPriceTrigger, 
					int nVolumeMode, int nVolume, BOOL bIsReplaceOrder);
	BOOL CancelOrder(int nNextStep=conStepNothing);

public:

	void SetStartTime(DWORD dwStartTime) {
		m_dwStartTime = dwStartTime;
	};
	int GetStartTime() {
		return m_dwStartTime;
	};
	BOOL IsOverdue(DWORD dwCurrTime, DWORD dwTimeoutNum=2000) {
		return (dwCurrTime-m_dwStartTime)>=dwTimeoutNum;
	};

	int GetRunStatus() {
		return m_nRunStatus;
	};
	int GetNextStep() {
		return m_nNextStep;
	}

	STDORDERREQ& GetStdOrderReq() {
		return m_stdReq;
	};
	string GetUserID() {
		return string(m_stdReq.csUserID);
	};

	CThostFtdcRspInfoField&	GetLastRspInfo() {
		return m_lastRspInfo;
	};

	BOOL IsCanCancel() {
		if(m_nArrayIndex<0)
			return FALSE;
		return m_arrayOrderRsp[m_nArrayIndex].OrderStatus==THOST_FTDC_OST_PartTradedQueueing ||
				m_arrayOrderRsp[m_nArrayIndex].OrderStatus==THOST_FTDC_OST_NoTradeQueueing;
	};

	int GetTradeVolume() {
		if(IsParketOrder(m_stdReq))
			return 0;
		if(m_nRunStatus == conWaiting)
			return 0;
		if(m_nArrayIndex<0)
			return 0;
		return m_arrayOrderRsp[m_nArrayIndex].VolumeTraded;
	};
	
	// ��һ��ִ���µ���ʹ�ã�ֻ��״̬ΪconNothing��Ч
	BOOL Launch(BOOL bNothingOrder=FALSE);
	// �����ɴ˶����µı�������״̬ΪconIsSuccess��Ч
	BOOL Cancel();
	// �ĵ���ֱ��ʹ�øĵ������֣���״̬ΪconIsSuccess��Ч
	// conVolumeNone��������
	BOOL Replace(int nPriceMode, double fPrice, 
			int nPriceTriggerMode, double fPriceTrigger, int nVolumeMode, int nVolume);
	// �����������µ�����״̬ΪconIsSuccess��Ч
	// conVolumeNoneʹ��ʣ�������µ���ͬconVolumeRemain
	BOOL CancelThenOrder(int nPriceMode, double fPrice, 
			int nPriceTriggerMode, double fPriceTrigger, int nVolumeMode, int nVolume);

	void ChangePrice(BOOL bIsMarket, double fPrice);
	void ChangeTriggerCond(int nPriceType, int nCondType, 
			BOOL bIsMarket, double fPrice, BOOL bNeedChangeOrderType=TRUE);

	BOOL ParseParkedStatus(DataRspParkedOrderInsert& field);
	BOOL ParseOrderStatus(DataRtnOrder& field);
	BOOL ParseOrderStatus(DataRspOrderInsert& field);
	BOOL ParseActionStatus(DataRspOrderAction1& field);
	BOOL ParseActionStatus(DataRspOrderAction2& field);

	wxString GetOrderDesciption();
	wxString GetRevokeDesciption() {
		return m_strRevokeMsg;
	};

private:

	DWORD				m_dwStartTime;//��ֵ=0xFFFFFFFF;

	// ÿ��ʱ��ֻ����һ��״̬�������ݲ�ͬ��״̬���в�ͬ�Ļ
	int					m_nRunStatus;

	BOOL				m_bNeedNewOrderStatus;
	UINT				m_nLastOrderReqID;
	UINT				m_nLastCancelReqID;
	int					m_nArrayIndex;

	std::vector<CThostFtdcParkedOrderField>
						m_arrayParkedRsp;
	std::vector<PlatformStru_OrderInfo>
						m_arrayOrderRsp;

	int					m_nMode;
	BOOL				m_bNoSendVolZoreOrder;
	STDORDERREQ			m_stdReq;

	int					m_nNextStep;
	int					m_nPriceMode;
	double				m_fPrice;
	int					m_nPriceTriggerMode;
	double				m_fPriceTrigger;
	int					m_nVolumeMode;
	int					m_nVolume;

	wxString			m_strRevokeMsg;

	CThostFtdcRspInfoField	
						m_lastRspInfo;	// Ӧ��ṹ

};
