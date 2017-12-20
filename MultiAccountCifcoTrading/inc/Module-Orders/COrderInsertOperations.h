#pragma once
#include "stdafx.h"
#include <vector>
#include "../inc/Module-Misc/TradeInfoDlg.h" //������Ϣ��ʾ�Ի���
#include "CfgMgr/CfgGlobal.h"

#include "../inc/Order-Common/COrderServiceThread.h"
#include "COrderInsertEntity.h"
#include "COrderChangeEntity.h"
#include "COrderInsertView.h"
#include "CMAOrderEntity.h"
#include "CMAOrderConfirmDlg.h"
#include "COrderAtomEntity.h"

#include "../inc/Module-Misc/SimpleWriteLog.h"

class CMAOrderConfirmDlg;

class COrderInsertOperations : public COrderInsertView
{
public:

	COrderInsertOperations(wxWindow *parent,
			wxWindowID winid = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style =wxScrolledWindowStyle | wxTAB_TRAVERSAL | wxNO_BORDER,
			const wxString& name = wxPanelNameStr);
	~COrderInsertOperations();

	void SetMultiplyVolume(int nVolume) {
		m_nMultiplyVolume = nVolume;
	};

//DECLARE_EVENT_TABLE()

protected:

	void		FillInputOrderField(PlatformStru_InputOrder& req);
	
	int			GetLogonAccounts(map<string,bool>& accsLogon, BOOL bIsAll=FALSE);
	void		FilterNoPositionAccount(BOOL bNeedCancel, 
					COrderAtomEntity::STDORDERREQ& stdReq, map<string, bool>& mapAccounts);

	double		AnalysePrice(double fPrice, wxString& strInstrumentID);
	BOOL		DoMaOrderInsert(CSAOrderEntity::OPERPARAM& operParam, 
					COrderAtomEntity::STDORDERREQ& stdReq, 
					CMAOrderEntity::MAAUTOOPER& maOper, 
					map<string, bool>& accsLogon);
	BOOL		DoOrderInsert(COrderInsertEntity::ORIORDERREQ& tOriReq, int nAction, BOOL& bHasOrderInsert, BOOL bIsAsyncView=FALSE);
	BOOL		DoOrderChange(PlatformStru_OrderInfo& ftdcField, BOOL bIsAsyncView=FALSE);
	void		DoBackHand(wxString strAccount, wxString InstrumentID, char PosiDirection, char Hedge, int Volume, BOOL bOnlyOpen=FALSE);
	void		DoKeyOrder(wxString& strCode, KeyOrderInfo& keyOrder);
	void		AsyncViewData(BOOL bIsBuy, int nOCMode, int nVolume, double fPrice, int nAction);
	
	//���ݱ����ر���ʾ ������Ϣ
	void		ShowTradeInfoDlg(const string& Title, const CThostFtdcOrderField& OrderInfo, BOOL bIsShow);
	//���ݱ�����ִ��ʾ ������Ϣ
	void		ShowTradeInfoDlg(const string& Title, const DataRspOrderInsert& OrderInfo, BOOL bIsShow);
	//���ݱ�����ִ��ʾ ������Ϣ
	void		ShowTradeInfoDlg(const string& Title, const COrderChangeEntity::CMBORDERINSERT& OrderInfo, BOOL bIsShow);
	//���ݱ�����ִ��ʾ ������Ϣ
	void		ShowTradeInfoDlg(const string& Title, const COrderInsertEntity::CMBORDER& OrderInfo, BOOL bIsShow);
	//����ErrRtnOrderAction��ʾ ����ʧ����Ϣ
	void		ShowTradeInfoDlg(const string& Title, const CThostFtdcOrderActionField& OrderActionInfo, BOOL bIsShow);
	//����OnRspOrderAction��ʾ ����ʧ����Ϣ
	void		ShowTradeInfoDlg(const string& Title, const DataRspOrderAction1& InputOrderActionInfo, BOOL bIsShow);
    //��ʾ������Ϣ
    void		ShowTradeInfoDlg(const string& Title, const string& errormessage, BOOL bIsShow);

	BOOL		NeedConfirm();							// �ж��µ�ǰ�Ƿ���Ҫȷ��
	BOOL		NeedAutoOpenClose(int& nAutoMode);		// �ж��Ƿ���Ҫ�Զ���ƽ
	BOOL		NeedAlwaysOpen();
	BOOL		NeedAutoOCHideRadio();					// ʹ���Զ�ƽ��ʱ��ȡ����ƽ��ѡ��
	BOOL		NeedContinueTrackPrice();				// �ڸ���״̬ �µ� ���������
	BOOL		NeedSimulateMarketPrice();
	BOOL		NeedReturnQuotList();
	BOOL		NeedOrderSuccessConfirm();
	BOOL		NeedOrderSuccessSound();
	BOOL		NeedOrderFailConfirm();
	BOOL		NeedOrderFailSound();
	BOOL		NeedOrderTradeConfirm();
	BOOL		NeedOrderTradeSound();
	void		SaveFivePriceList(BOOL bHas);

	int			GetAfterFocus();
	int			GetActionAfterOrder();
	int			GetRaskVolumeLimit();
	int			GetRaskDifferPrice();
	int			GetDefaultVolume(wxString& wxCode);
	int			GetDefaultVolume(wxString& wxCode, int& nDefaultVolume, int& nMultiplyVolume);

public:
	static BOOL		NeedFivePriceList();

	// LockObject()��UnloadObject()��������������ɶԵ���
	void LockObject() {
		m_oMutex.Lock();
	};
	void UnlockObject() {
		m_oMutex.Unlock();
	};

protected:

	double		m_currPriceTick;
	double		m_currUpperLimitPrice;
	double		m_currLowerLimitPrice;
	int			m_nMultiplyVolume;
    bool		m_NeedUpdatePrice;
	BOOL		m_bNoKeyChangeText;
	BOOL		m_bIsGridChangeInstrumentID;

	wxMutex		m_oMutex;

    std::set<std::string> m_InstrumentIDArray;
	COrderServiceThread* m_poOrderServiceThread;
	volatile bool m_bWaitingRsp_OrderInsert;//�ȴ��µ�����Ӧ
	volatile bool m_bWaitingRsp_OrderDelete;//�ȴ���������Ӧ
	//int m_ReqID;//����µ�������ID��
	bool m_bShowTradeInfo;//���������ļ����Ƿ���ʾ������Ϣ
	std::map<std::string, int> m_KeyMultiplyVolumeMap;
	std::map<COrderInsertEntity*, int> m_mapOrderInsertWaitForCancel;	// ������Ҫ�ȴ�������ɺ�����µ���ʵ��ļ���
	std::map<COrderAtomEntity*, int> m_mapCancelThenOrder;	// ������Ҫ�ȴ�������ɺ�����µ���ʵ��ļ���
	std::map<CMAOrderEntity*, CMAOrderConfirmDlg*> m_mapMAOrderMonitoring;	// ������Ҫ���ӵ�MAEntity�ļ���

	zqWriteLog* m_pWriteLog;

};
