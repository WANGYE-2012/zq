#pragma once
#include "stdafx.h"
#include "CfgMgr/CfgGlobal.h"

/*
1.����ԭʼ������Ϣ
2.ת����ʵ�ʱ�����Ϣ
3.һ��ԭʼ������Ӧһ��ʵ��
*/
class COrderInsertEntity
{
public:
	enum {	conMaxRspOrderReqNum = 10, conMaxFtdcOrderActionNum = 1000 };

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

	typedef struct {
		int		nOrderType;
		TThostFtdcInstrumentIDType	strInstrumentID;
		char	strAccount[32];
		BOOL	bIsParketOrder;
		BOOL	bDirection;
		int		nOCMode;                //0����; 1ƽ��; 2ƽ��; -1δ֪
		double	fPrice;
		int		nVolume;
		int		nHedgeStatus;
		BOOL	bIsMarketPrice;
		BOOL	bSimulate;				// �Ƿ����м�ģ��
		BOOL	bIsAuto;
		int		nAutoMode;
		BOOL	bIsPreClose;			// �Ƿ񰴿�ƽ��������
		int		nDefaultVolume;
		int		nMinVolume;
		//int		nPriceOffset;		// �۸�䶯��λ���۸�:=fPriceX+nPriceOffset*fPriceTick
		//double	fPriceTick;
		//double	fAskPrice1;			// ��һ��
		//double	fLastPrice;			// ���¼�
		//double	fBidPrice1;			// ��һ��
	} ORIORDERREQ, *PORIORDERREQ;

	typedef struct 
    {
		BOOL					bIsParked;	// �Ƿ�ΪԤ�񵥻���������
		union 
        {
			PlatformStru_ParkedOrder	parked;	// Ԥ����
			PlatformStru_InputOrder		insert;	// ��������
		} order;
		CThostFtdcRspInfoField	rspInfo;	// Ӧ��ṹ
	} CMBORDER, *PCMBORDER;

	typedef struct  {
		//CThostFtdcInputOrderActionField action;	// �������
		PlatformStru_InputOrderAction action;	// �������
		CThostFtdcRspInfoField	rspInfo;	// Ӧ��ṹ
	} CMBORDERACTION, *PCMBORDERACTION;

public:
	// bPreClose: �Ƿ񰴿�ƽ��������
	static COrderInsertEntity* CreateOrderInsert(ORIORDERREQ& tOriReq);

	static BOOL GetHoldingVolume(string strCode, BOOL bIsBuy, int nHedgeStatus, int& nSumVolume, int& nSumTodayVolume, string strAccount);
	static BOOL GetAskCloseVolume(string strCode, BOOL bIsBuy, int nHedgeStatus, int& nAskVolume, int& nAskTodayVolume, string strAccount);
	static BOOL GetPreVolume(string strCode, BOOL bIsBuy, int nHedgeStatus, int& nAskVolume, int& nAskTodayVolume, string strAccount);//��ƽ����

	static BOOL AnalyseOrderReq(ORIORDERREQ& tOriReq, ORDERREQBREAK& tOrderReqBreak);

	~COrderInsertEntity();

protected:
	COrderInsertEntity();

	BOOL InsertOrder(wxString& strInstrumentID, wxString& strAccount, 
			BOOL bDirection, int nOCMode, double fPrice, int nVolume, int nHedgeStatus, 
			BOOL bIsMarketPrice, BOOL bSimulate, BOOL bIsParketOrder, 
			int nOrderType, int nMinVolume);

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
	
	int GetOrderReqCount() {
		return m_nRspOrderReqCount;
	};
	CMBORDER& GetOrderReq(int nIndex) {
		return m_tRspOrderReqs[nIndex];
	};
	int GetFtdcOrderActionCount() {
		return m_nFtdcOrderActionCount;
	};
//	CThostFtdcInputOrderActionField& GetFtdcOrderAction(int nIndex) {
//		return m_tFtdcOrderActions[nIndex];
//	};
	CMBORDERACTION& GetFtdcOrderAction(int nIndex) {
		return m_tFtdcOrderActions[nIndex];
	};
	BOOL IsRevokeAllOK() {
		BOOL bIsOK = TRUE;
		for(int i=0; i<m_nFtdcOrderActionCount; i++) 
			bIsOK = bIsOK && m_bFtdcOrderActionsState[i];
		return bIsOK;
	};
	BOOL MarkRevokeItemState(DataRtnOrder& field);
	BOOL FindRspOrderAction(DataErrRtnOrderAction& act);

	//int GetOrderVolumeTotal() {
	//	int nVolume=0;
	//	for(int i=0; i<m_nRspOrderReqCount;i++) {
	//		nVolume += m_tRspOrderReqs[i].InputOrderField.VolumeTotalOriginal;
	//	}
	//	return nVolume;
	//};

	BOOL DoOrder(int& nErrPos, wxString& strRetMsg);
	BOOL RevokeOrder(wxString& strInstrumentID, BOOL bDirection, int nOCMode, wxString& strRetMsg);
	BOOL DoRevokeOrder(int& nErrPos, wxString& strRetMsg);
	wxString GetOrderDesciption(int nViewStyle=3);
	wxString GetRevokeOrderDesciption(int nViewStyle=3) {
		return m_strRevokeMsg;
	};

	void LockForSetChange() {
		m_bIsLockForSetChange = TRUE;
	};
	void UnlockForSetChange() {
		m_bIsLockForSetChange = FALSE;
	};

private:

	BOOL				m_bIsAuto;
	BOOL				m_bIsParkedOrder;
	BOOL				m_bIsLockForSetChange;

	//int					m_nReverseTimer;

	int					m_nRspOrderReqCount;

	CMBORDER			m_tRspOrderReqs[conMaxRspOrderReqNum];

	wxString			m_strRevokeMsg;
	int					m_nFtdcOrderActionCount;
//	CThostFtdcInputOrderActionField		m_tFtdcOrderActions[conMaxFtdcOrderActionNum];
	CMBORDERACTION		m_tFtdcOrderActions[conMaxFtdcOrderActionNum];
	BOOL				m_bFtdcOrderActionsState[conMaxFtdcOrderActionNum];	// TRUE, ��ʾ�����ɹ�

};
