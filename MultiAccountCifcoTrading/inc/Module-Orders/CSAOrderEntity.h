#pragma once
#include "stdafx.h"
#include "CfgMgr/CfgGlobal.h"
#include "COrderAtomEntity.h"

/*
1.����ԭʼ������Ϣ
2.ת����ʵ�ʱ�����Ϣ
3.һ��ԭʼ������Ӧһ��ʵ��
*/
// CSAOrderEntity����֧��replace��CancelThenOrder����Щ������COrderAtomEntity
class CSAOrderEntity
{
public:
	enum {	conMaxOrderReqNum = 10, conMaxOrderActionNum = 200 };
	enum {	conOperNoneAuto, conOperAuto };//, conOperReplace }; 
	enum {	conAutoCloseAll=1, conAutoCloseDefault, conAutoReverseOpen };
	enum {	conWaiting, conHasDone, conIsSuccess, conIsFail, conIsCancel };

	typedef struct {
		BOOL	bIsOpen;				// �Ƿ���Ҫ����
		DWORD	dwVolume_Open;			// ������
		double	fPrice_Open;			// ���ּ�
		BOOL	bIsCloseToday;			// �Ƿ���Ҫƽ��
		DWORD	dwVolume_CloseToday;	// ƽ����
		double	fPrice_CloseToday;		// ƽ���
		BOOL	bIsClose;				// �Ƿ���Ҫƽ��
		DWORD	dwVolume_Close;			// ƽ����
		double	fPrice_Close;			// ƽ�ּ�
		BOOL	bIsReverseOpen;			// �Ƿ���Ҫ���򿪲�
		DWORD	dwVolume_ReverseOpen;	// ���򿪲���
		double	fPrice_ReverseOpen;		// ���򿪲ּ�
		BOOL	bIsRevoke;				// �Ƿ�����ƽ��/ƽ��ҵ�
		DWORD	dwVolume_Revoke;		// ƽ��/ƽ��ҵ�
	} ORDERREQBREAK, *PORDERREQBREAK;

	// �����ʵ�Ǹ�CSAOrderEntity�õģ�Createʱ��Ϊһ����׼����ʹ��
	// ���⣬����Ǽ򵥵ĸĵ�����ҵ�����еĸĵ����ܣ���ʹ��CSAOrderEntity����ֱ����COrderAtomEntity����
	typedef struct {
		int		nOperType;				// ִ�����ͣ�0 ���Զ�����1 �Զ�����2 ���Զ��ĵ�
		int		nAutoMode;				// �Զ���ģʽ��0 ���ֲ���ȫƽ��1 ��Ĭ������ƽ�֣������򿪲�
										// 2 ��Ĭ������ƽ�֣��������ַ��򿪲�
		int		nDefaultVolume;			// Ĭ������
		BOOL	bNeedCancel;			// �Ƿ���Ҫ������ر���
		BOOL	bIsCanClose;			// �Ƿ񰴿�ƽ��������
		int		nCutMode;				// 0 ���ֲ𱨵���1 ��ָ�����ֲ�2 ����������󱨵����ֲ�
		int		nCutVolume;				// ָ���ķֲ�����
		BOOL	bSimulate;				// �Ƿ�����м�ģ��
		BOOL	bSimulate_Trigger;		// �Ƿ�����м�ģ��
		BOOL	bNoSendVolZoreOrder;	// ����������Ϊ0�ı���
	} OPERPARAM, *LPOPERPARAM;

	typedef struct  {
		PlatformStru_InputOrderAction action;	// �������
		PlatformStru_OrderInfo	orderRsp;		// ��������
		CThostFtdcRspInfoField	rspInfo;		// Ӧ��ṹ
	} CMBORDERACTION, *PCMBORDERACTION;

public:
	// bPreClose: �Ƿ񰴿�ƽ��������
	static CSAOrderEntity* CreateSAOrderEntity(
			COrderAtomEntity::STDORDERREQ& stdReq, OPERPARAM& operParam);

	static BOOL AnalyseOrderReq(
			COrderAtomEntity::STDORDERREQ& stdReq, OPERPARAM& operParam, 
			ORDERREQBREAK& tOrderReqBreak);

	// ��ֲ���
	static BOOL GetHoldingVolume(string& strUserID, string& strAccount, string& strCode, 
			BOOL bIsBuy, BOOL bIsHedge, int& nSumVolume, int& nSumTodayVolume);
	// ���ƽ����
	static BOOL GetCanCloseVolume(string& strUserID, string& strAccount, string& strCode, 
			BOOL bIsBuy, BOOL bIsHedge, int& nCanCloseVolume, int& nCanCloseTodayVolume);
	// ��ƽ�ֹҵ�����
	static BOOL GetPreCloseVolume(string& strUserID, string& strAccount, string& strCode, 
			BOOL bIsBuy, BOOL bIsHedge, int& nPreCloseVolume, int& nPreCloseTodayVolume);

	static BOOL HasTrade(const string& strUserID);

	~CSAOrderEntity();

protected:
	CSAOrderEntity(COrderAtomEntity::STDORDERREQ& stdReq, OPERPARAM& operParam, 
			ORDERREQBREAK& orderReqBreak);

//	BOOL InsertOrder(string& strUserID, string& strAccount, string& strInstrumentID, 
//			BOOL bIsBuy, int nOCMode, double fPrice, int nVolume, BOOL bIsHedge, 
//			BOOL bIsMarketPrice, BOOL bSimulate, BOOL bIsParketOrder);
	BOOL InsertOrder(COrderAtomEntity::STDORDERREQ stdReq, 
			BOOL bIsBuy, int nOCMode, int nVolume);

	BOOL MakeRevokeActions(string& strUserID, string& strAccount, 
			string& strInstrumentID, BOOL bIsBuy, int nOCMode);
//	BOOL MakeRevokeAction_Replace(OrderKey& orderKey);

public:

	//void SetReverseTimer(int nReverseTimer) {
	//	m_nReverseTimer = nReverseTimer;
	//};
	//int GetReverseTimer() {
	//	return m_nReverseTimer;
	//};
	//void ReverseTimerSubOne() {
	//	if(m_nReverseTimer>0)
	//		m_nReverseTimer--;
	//};
	//BOOL IsOverdue() {
	//	return m_nReverseTimer<=0;
	//};

	BOOL NeedCancelOrder() {
		return m_nOrderActionCount>0;
		//return m_operParam.bNeedCancel;
	};
	int Status_DoCancel() {
		return m_nDoCancelStatus;
	};
	int Status_DoInsert() {
		return m_nDoInsertStatus;
	};
	int GetRunStatus(int nOCMode);

	string GetUserID() {
		return string(m_stdOrderReq.csUserID);
	};
	
	OPERPARAM& GetOperParam() {
		return m_operParam;
	};
	int GetOrderReqCount() {
		return m_arrOrderEntity.size();
	};
	COrderAtomEntity* GetOrderReq(int nIndex) {
		if(nIndex<0 || nIndex>=GetOrderReqCount())
			return NULL;
		else
			return m_arrOrderEntity[nIndex];
	};
	int GetNeedCancelVolume() {
		return m_nNeedCancelVolume;
	};
	int GetOrderActionCount() {
		return m_nOrderActionCount;
	};
	CMBORDERACTION* GetOrderAction(int nIndex) {
		if(nIndex<0 || nIndex>=m_nOrderActionCount)
			return NULL;
		else
			return m_tOrderActions+nIndex;
	};
	BOOL IsRevokeAllOK() {
		BOOL bIsOK = TRUE;
		for(int i=0; i<m_nOrderActionCount; i++) 
			bIsOK = bIsOK && m_bOrderActionsState[i];
		return bIsOK;
	};
	UINT GetVolumeSum() {
		UINT nCount = 0;
		if(m_orderReqBreak.bIsOpen)
			nCount += m_orderReqBreak.dwVolume_Open;
		if(m_orderReqBreak.bIsReverseOpen)
			nCount += m_orderReqBreak.dwVolume_ReverseOpen;
		if(m_orderReqBreak.bIsCloseToday)
			nCount += m_orderReqBreak.dwVolume_CloseToday;
		if(m_orderReqBreak.bIsClose)
			nCount += m_orderReqBreak.dwVolume_Close;
		return nCount;
	};
	BOOL GetVolumeSum(int& nSumOpenVolume, int& nSumTodayVolume, int& nSumYdVolume) {
		if(m_orderReqBreak.bIsOpen)
			nSumOpenVolume += m_orderReqBreak.dwVolume_Open;
		if(m_orderReqBreak.bIsReverseOpen)
			nSumOpenVolume += m_orderReqBreak.dwVolume_ReverseOpen;
		if(m_orderReqBreak.bIsCloseToday)
			nSumTodayVolume += m_orderReqBreak.dwVolume_CloseToday;
		if(m_orderReqBreak.bIsClose)
			nSumYdVolume += m_orderReqBreak.dwVolume_Close;
		return TRUE;
	};
	BOOL GetVolumeSum_Trade(int& nOpenVolume, int& nTodayVolume, int& nYdVolume);
	BOOL GetCancelVolumeSum(int& nWaitVolume, int& nRealVolume);

	int SumCanCancel();

	BOOL DoOrderReq(int& nErrPos, string& strRetMsg, BOOL bNoSendZero=FALSE);
	BOOL DoRevokeAction(int& nErrPos, string& strRetMsg);
	BOOL Cancel(int& nCancelSum, string& strRetMsg);

	void ChangePrice(BOOL bIsMarket, double fPrice);
	void ChangeTriggerCond(int nPriceType, int nCondType, 
			BOOL bIsMarket, double fPrice, BOOL bNeedChangeOrderType=TRUE);


	BOOL ResetParkedStatus(DataRspParkedOrderInsert& field);
	BOOL ResetOrderStatus(DataRtnOrder& field);
	BOOL ResetOrderStatus(DataRspOrderInsert& field);
	BOOL ResetActionStatus(DataRspOrderAction1& field);
	BOOL ResetActionStatus(DataRspOrderAction2& field);


	BOOL MarkOrderActionStatus(DataRtnOrder& field);
	BOOL FindOrderAction(DataRspOrderAction2& act);

	std::string GetOrderDesciption();
	std::string GetRevokeOrderDesciption() {
		return m_strRevokeMsg;
	};

private:

	int					m_nDoCancelStatus;
	int					m_nDoInsertStatus;
	string				m_strRevokeMsg;

//	std::vector<COrderAtomEntity*>	m_arrCancelEntity;
	std::vector<COrderAtomEntity*>	m_arrOrderEntity;

	COrderAtomEntity::STDORDERREQ	m_stdOrderReq;
	OPERPARAM						m_operParam;
	ORDERREQBREAK					m_orderReqBreak;


	//int					m_nOrderReqCount;
	//CMBORDER			m_tOrderReqs[conMaxOrderReqNum];

	int					m_nNeedCancelVolume;
	int					m_nOrderActionCount;
	CMBORDERACTION		m_tOrderActions[conMaxOrderActionNum];
	BOOL				m_bOrderActionsState[conMaxOrderActionNum];	// TRUE, ��ʾ�����ɹ�

};
