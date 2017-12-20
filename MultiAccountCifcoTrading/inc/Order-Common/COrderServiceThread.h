
#pragma once

#include "stdafx.h"
#include <vector>
#include "CfgMgr/CfgGlobal.h"


// ����״̬
enum {	conReqWaiting,			// �ȴ�����
		conReqSendOk,			// ���ͳɹ�
		conReqSendFail,			// ����ʧ��
		conReqEntrustOk,		// ί�е��ɹ�
		conReqEntrustFail,		// ί�е�ʧ��
		conReqDealFail,			// δ�ɽ�
		conReqDealClose			// �ѳɽ�
};

// ������״̬
enum {	conBatchPause,			// ��ͣ
		conBatchActive,			// ����
		conBatchDelete,			// ɾ��
		conBatchCompleted		// ȫ���ɽ�
};

// Ԥ��״̬
enum {	conParkedPause,			// ��ͣ
		conParkedActive,		// ����
		conParkedDelete,		// ɾ��
		conParkedConfirm		// �ύ����
};

// ������״̬
enum {	conCondPause,			// ��ͣ
		conCondActive,			// ����
		conCondDelete,			// ɾ��
		conCondConfirm,			// �ύ����
		conCondCancel,			// ����
		conCondCompleted		// ȫ���ɽ�
};

// ӯ��״̬
enum {	conPositionPause,		// ��ͣ
		conPositionActive,		// ����
		conPositionDelete,		// ɾ��
		//conPositionBaseCancel,	// ����������
		//conPositionBaseTrade,	// �������ɽ�
		conPositionConfirm,		// ִ��ֹӯֹ��
		conPositionCancel,		// ����ֹӯֹ��
		conPositionCompleted	// ȫ���ɽ�
};

// ��������ӯ�𵥵Ĺ�����ϵ��conCondPosRelation
enum {	conCPRNon,				// �޹�ϵ
		conCPRCondFromPos,		// ��������ӯ�𵥵�ֹӯֹ������
		conCPRCondNextPos		// ��������ɺ󼤻�ӯ��
};

class COrderServiceThread : public wxEvtHandler//, wxThread
{
public:

	typedef struct {
		DWORD	dwDate;
		DWORD	dwTime;
		DWORD	dwOrderReqNum;
		DWORD	dwBatchGroupNum;		// ��������
		DWORD	dwBatchOrderNum;		// �������ӵ���
		DWORD	dwParkedOrderNum;
		DWORD	dwCondOrderNum;
		DWORD	dwPositionOrderNum;
		DWORD	dwParkedCount;
		DWORD	dwParkedOffset;
		DWORD	dwParkedSize;
		DWORD	dwCondActiveCount;
		DWORD	dwCondActiveOffset;
		DWORD	dwCondActiveSize;
		DWORD	dwPositionCount;
		DWORD	dwPositionOffset;
		DWORD	dwPositionSize;
		DWORD	dwBatchSubCount;
		DWORD	dwBatchSubOffset;
		DWORD	dwBatchSubSize;
		DWORD	dwCondOverdueCount;
		DWORD	dwCondOverdueOffset;
		DWORD	dwCondOverdueSize;
		DWORD	dwPosOverdueCount;
		DWORD	dwPosOverdueOffset;
		DWORD	dwPosOverdueSize;
	} FILEHEAD, *PFILEHEAD;		// sizeof()=64


	// ������
	typedef struct {
		int		nNum;				//���
		int		nState;				//�״̬
		int		nOriVolume;			//ԭʼ�ύ����
		int		nPriceType;			//�۸����ͣ�1 ����ۣ�2 ���¼ۣ�3 ��һ��һ��
		int		nEachNum;			//ÿ������
		int		nTimeInterval;		//ʱ��������λ����
		int		nNextSubNum;		//��һ�����������ں�
		int		nTimePos;			//ʱ��㣬����ʱ��ʽ����ֵΪ"nTimeInterval"
		PlatformStru_InputOrder req;
	} BATCHORDER, *PBATCHORDER;

	// �������ӵ�
	typedef struct {
		int		nNum;				//���
		int		nGroupNum;			//��ţ�����������
		int		nSubNum;			//���ں�
		int		nRegNum;			//�����
		int		nEntrustNum;		//ί�е���
		int		nDealNum;			//�ɽ�����
		int		nRegState;			//ί��״̬
		PlatformStru_InputOrder req;
	} BATCHSUBORDER, *PBATCHSUBORDER;

	// ����Ԥ��
	typedef struct {
		int		nNum;				//���
		int		nState;				//�״̬
		int		nRegNum;			//�����
		int		nRegState;			//ί��״̬
		BOOL	bIsAuto;			//�Ƿ�Ϊ�Զ�ģʽ��FALSE �ֶ�������TRUE �Զ�
		DWORD	nCreateTime;		//Ԥ��ʱ��
		DWORD	nDeleteTime;		//ɾ��ʱ��
		PlatformStru_InputOrder req;
		TThostFtdcExchangeIDType	ExchangeID;	// ����������
		char	strErrorMsg[256];	//������Ϣ
	} PARKEDORDER, *PPARKEDORDER;

	// ����������
	typedef struct {
		int		nNum;				//���
		int		nState;				//�״̬
		int		nCondPosRelation;	//��ӯ�𵥵Ĺ�ϵ
		int		nPosNum;			//ӯ�𵥺�
		//int		nRelateCondNum;		//������������
		int		nRegNum;			//�����
		int		nEntrustNum;		//ί�е���
		int		nDealNum;			//�ɽ�����
		int		nRegState;			//ί��״̬
		BOOL	bHedge;				//Ͷ����ֵ
		int		nPriceType;			//�۸񴥷����ͣ�1 ���£�2 ��ۣ�3 ����
		int		nConditionType;		//�����������ͣ�1 С�ڵ���ָ���۸�2 ���ڵ���ָ���۸�
		double	fConditionPrice;	//ָ���Ĵ����۸�
		DWORD	nCreateTime;		//Ԥ��ʱ��
		DWORD	nDeleteTime;		//ɾ��ʱ��
		PlatformStru_InputOrder req;
		char	strErrorMsg[256];	//������Ϣ
	} CONDITIONORDER, *PCONDITIONORDER;

	// ӯ�𵥵Ĺ���
	// 1.  ����ָ����ί�п���
	// 2.  ͬʱ��һ��ֹӯ������
	// 3.  ͬʱ�ٿ�һ��ֹ��������

	// ����ӯ��
	typedef struct {
		int		nNum;				//���
		BOOL	bIsRunInServer;		//�Ƿ����ۺϽ���ƽ̨��ִ��ֹӯֹ��
		int		nState;				//�״̬
		int		nRegNum;			//�����
		int		nOpenNum;			//����ί�к�
		int		nDealNum;			//�ɽ�����
		int		nRegState;			//����ί��״̬
		int		nCondNum_StopGain;	//ֹӯ��������
		int		nCondNum_StopLose;	//ֹ����������
		BOOL	bHedge;				//Ͷ����ֵ
		int		nPriceOffsetType;	//�۸�ƫ���������ͣ�1 �ɽ��ۣ�2 ί�м�
		BOOL	bDoStopLose;		//ִ��ֹ��
		double	fStopLosePriceOffset;	//ֹ��۲�
		BOOL	bDoStopGain;		//ִ��ֹӯ
		double	fStopGainPriceOffset;	//ֹӯ�۲�
		int		nPriceType;			//�����۸����ͣ�1 ���¼ۣ�2 ������
		int		nCloseMode;			//ƽ�ַ�ʽ��1 �����ǵ�ͣ�ۣ�2 ����Ӽ���
		double	fClosePriceOffset;	//����Ӽ��۲�
		BOOL	bIsBuy;				//�Ƿ�Ϊ����
		BOOL	bIsMarket;			//�Ƿ�Ϊ�м�
		double	fRegPrice;			//ί�м�
		int		nRegVolume;			//ί������
		double	fOpenedPriceOffset;	//�ɽ���ƫ��
		double	fStopLosePrice;		//ֹ���
		double	fStopGainPrice;		//ֹӯ��
		double	fStopLoseForecast;	//ֹ��Ԥ��
		double	fStopGainForecast;	//ֹӯԤ��
		char	InstrumentID[32];	//��Լ����
		//CThostFtdcInputOrderField req;
	} POSITIONORDER, *PPOSITIONORDER;
	// ��Ҫʱ��������POSITIONORDER��ΪSTOPORDER������Ǹ������ֹӯֹ��ĸ���


	typedef std::map<int, BATCHORDER> MAPBATCHORDER;
	typedef std::map<int, BATCHORDER>::iterator MAPBATCHORDERITEM;
	typedef std::map<int, BATCHSUBORDER> MAPBATCHSUBORDER;
	typedef std::map<int, BATCHSUBORDER>::iterator MAPBATCHSUBORDERITEM;
	typedef std::map<int, PARKEDORDER> MAPPARKEDORDER;
	typedef std::map<int, PARKEDORDER>::iterator MAPPARKEDORDERITEM;
	typedef std::map<int, CONDITIONORDER> MAPCONDITIONORDER;
	typedef std::map<int, CONDITIONORDER>::iterator MAPCONDITIONORDERITEM;
	typedef std::map<int, POSITIONORDER> MAPPOSITIONORDER;
	typedef std::map<int, POSITIONORDER>::iterator MAPPOSITIONORDERITEM;

public:
	
	static COrderServiceThread* GetInstance();


	COrderServiceThread();//wxWindow *parent, wxWindowID winid);
	~COrderServiceThread(){}

	BOOL InitService();

    // thread execution starts here
    virtual void *Entry();

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit();
	void OnInitSubscribe(wxCommandEvent& evt);
	void OnRcvQuot(wxCommandEvent& evt);
	void OnRcvOrder(wxCommandEvent& evt);
	void OnRcvTrade(wxCommandEvent& evt);
	
	void OnPlatSvrStatusChange(wxCommandEvent& evt);
	void OnStopPlatform(wxCommandEvent& evt);
	DECLARE_EVENT_TABLE()
public:
    static COrderServiceThread* CreateThread();
	static int CallBack_InstrumentStatus(const GUIModuleID,const AbstractBusinessData &data);

//	static int	sm_nReqNum;
	static int	sm_nBatchOrderNum;
	static int	sm_nBatchSubOrderNum;
	static int	sm_nParkedOrderNum;
	static int	sm_nConditionOrderNum;
	static int	sm_nPositionOrderNum;

public:
	MAPBATCHORDER& GetOrderBatchMap() {
		return m_mapBatchOrder;
	};
	MAPBATCHSUBORDER& GetOrderBatchSubMap() {
		return m_mapBatchSubOrder;
	};
	MAPPARKEDORDER& GetOrderParkedMap() {
		return m_mapParkedOrder;
	};
	MAPCONDITIONORDER& GetOrderConditionMap() {
		return m_mapConditionOrder;
	};
	MAPCONDITIONORDER& GetOrderConditionOverdueMap() {
		return m_mapConditionOrder_Overdue;
	};
	MAPPOSITIONORDER& GetOrderPositionMap() {
		return m_mapPositionOrder;
	};
	MAPPOSITIONORDER& GetOrderPositionOverdueMap() {
		return m_mapPositionOrder_Overdue;
	};

	int OrderBatchAdd(BATCHORDER& tOrder, BOOL bIsLock=FALSE) {
		sm_nBatchOrderNum++;
		tOrder.nNum = sm_nBatchOrderNum;
		tOrder.nState = conBatchActive;
		m_mapBatchOrder[tOrder.nNum] = tOrder;
		//m_mapConditionOrder.insert(makepair(tOrder.nNum, tOrder));
		return sm_nBatchOrderNum;
	};
	BOOL OrderBatchDelete(int nNum, BOOL bIsLock=FALSE) {
		return m_mapBatchOrder.erase(nNum);
	};
	BOOL OrderBatchClear(BOOL bIsLock=FALSE) {
		m_mapBatchOrder.clear();
	};
	BOOL OrderBatchActive(int nNum, BOOL bActive, BOOL bIsLock=FALSE) {
		BATCHORDER tOrder={0};
		MAPBATCHORDERITEM it = m_mapBatchOrder.find(nNum);
		if(it != m_mapBatchOrder.end()) {
			tOrder = it->second;
			if(tOrder.nState >= conCondDelete) return FALSE;
			tOrder.nState = bActive ? conBatchActive : conBatchPause;
			m_mapBatchOrder[nNum] = tOrder;
			return TRUE;
		}
		else
			return FALSE;
	};


	int OrderBatchSubAdd(BATCHSUBORDER& tOrder) {
		sm_nBatchSubOrderNum++;
		tOrder.nNum = sm_nBatchSubOrderNum;
		m_mapBatchSubOrder[tOrder.nNum] = tOrder;
		return sm_nBatchSubOrderNum;
	};
	BOOL OrderBatchSubDelete(int nNum) {
		return m_mapBatchSubOrder.erase(nNum);
	};
	BOOL OrderBatchSubClear() {
		m_mapBatchSubOrder.clear();
	};

	int OrderParkedAdd(PARKEDORDER& tOrder) {
		sm_nParkedOrderNum++;
		tOrder.nNum = sm_nParkedOrderNum;
		tOrder.nCreateTime = ::wxGetUTCTime();
		m_mapParkedOrder[tOrder.nNum] = tOrder;
		return sm_nParkedOrderNum;
	};
	BOOL OrderParkedSet(int nNum, PARKEDORDER& tOrder) {
		m_mapParkedOrder[nNum] = tOrder;
		return TRUE;
	};
	BOOL OrderParkedDelete(int nNum) {
		MAPPARKEDORDERITEM it = m_mapParkedOrder.find(nNum);
		if(it==m_mapParkedOrder.end()) return FALSE;
		PARKEDORDER tOrder = m_mapParkedOrder[nNum];
		//if(tOrder.nState!=conCondDelete)
		tOrder.nState = conParkedDelete;
		tOrder.nDeleteTime = ::wxGetUTCTime();
		m_mapParkedOrder[nNum] = tOrder;
		return TRUE;
	};
	BOOL OrderParkedClear(BOOL bIsLock=FALSE) {
		m_mapParkedOrder.clear();
	};
	BOOL OrderParkedActive(int nNum, BOOL bActive) {
		PARKEDORDER tOrder={0};
		MAPPARKEDORDERITEM it = m_mapParkedOrder.find(nNum);
		if(it != m_mapParkedOrder.end()) {
			tOrder = it->second;
			if(tOrder.nState >= conParkedDelete) return FALSE;
			tOrder.nState = bActive ? conParkedActive : conParkedPause;
			m_mapParkedOrder[nNum] = tOrder;
			return TRUE;
		}
		else
			return FALSE;
	};
	BOOL OrderParkedDoNow(int nNum);

	int OrderConditionAdd(CONDITIONORDER& tOrder, BOOL bIsLock=FALSE) {
		sm_nConditionOrderNum++;
		tOrder.nNum = sm_nConditionOrderNum;
		tOrder.nCreateTime = ::wxGetUTCTime();
		m_mapConditionOrder[tOrder.nNum] = tOrder;
		PostEvent_ConditionStatus(sm_nConditionOrderNum);
		return sm_nConditionOrderNum;
	};
	BOOL OrderConditionSet(int nNum, CONDITIONORDER& tOrder, BOOL bIsLock=FALSE) {
		m_mapConditionOrder[nNum] = tOrder;
		PostEvent_ConditionStatus(nNum);
		return TRUE;
	};
	BOOL OrderConditionAdd_Position(CONDITIONORDER& tOrder1, CONDITIONORDER& tOrder2, BOOL bIsLock=FALSE) {
		sm_nConditionOrderNum++;
		tOrder1.nNum = sm_nConditionOrderNum;
		tOrder1.nState = conCondPause;

		sm_nConditionOrderNum++;
		tOrder2.nNum = sm_nConditionOrderNum;
		tOrder2.nState = conCondPause;
		
		//tOrder1.nRelateCondNum = tOrder2.nNum;
		//tOrder2.nRelateCondNum = tOrder1.nNum;
		
		m_mapConditionOrder[tOrder1.nNum] = tOrder1;
		m_mapConditionOrder[tOrder2.nNum] = tOrder2;
		return TRUE;
	};
	BOOL OrderConditionDelete(int nNum, int nState=conCondDelete, BOOL bIsLock=FALSE, BOOL bIsPostEvent=FALSE) {
		MAPCONDITIONORDERITEM it = m_mapConditionOrder.find(nNum);
		if(it==m_mapConditionOrder.end()) return FALSE;
		CONDITIONORDER tOrder = m_mapConditionOrder[nNum];
		if(nState==conCondDelete && !(tOrder.nState==conCondPause || tOrder.nState==conCondActive))
			return FALSE;
		tOrder.nState = nState;
		tOrder.nDeleteTime = ::wxGetUTCTime();
		m_mapConditionOrder_Overdue[nNum] = tOrder;
		m_mapConditionOrder.erase(nNum);
		if(bIsPostEvent)
			PostEvent_ConditionStatus(nNum);
		return TRUE;
	};
	BOOL OrderConditionClear(BOOL bIsLock=FALSE) {
		m_mapConditionOrder.clear();
	};
	BOOL OrderConditionActive(int nNum, BOOL bActive, BOOL bIsLock=FALSE) {
		CONDITIONORDER tOrder={0};
		MAPCONDITIONORDERITEM it = m_mapConditionOrder.find(nNum);
		if(it != m_mapConditionOrder.end()) {
			tOrder = it->second;
			if(tOrder.nState >= conCondDelete) return FALSE;
			tOrder.nState = bActive ? conCondActive : conCondPause;
			m_mapConditionOrder[nNum] = tOrder;
			return TRUE;
		}
		else
			return FALSE;
	};
	BOOL OrderConditionDoNow(int nNum, BOOL bIsLock=FALSE);

	int OrderPositionAdd(POSITIONORDER& tOrder, BOOL bIsLock=FALSE) {
		sm_nPositionOrderNum++;
		tOrder.nNum = sm_nPositionOrderNum;
		m_mapPositionOrder[tOrder.nNum] = tOrder;
		PostEvent_PositionStatus(sm_nPositionOrderNum);
		return sm_nPositionOrderNum;
	};
	BOOL OrderPositionSet(int nNum, POSITIONORDER& tOrder, BOOL bIsLock=FALSE) {
		m_mapPositionOrder[nNum] = tOrder;
		PostEvent_PositionStatus(nNum);
		return TRUE;
	};
	BOOL OrderPositionDelete(int nNum, int nState=conPositionDelete, BOOL bIsLock=FALSE, BOOL bIsPostEvent=FALSE) {
		MAPPOSITIONORDERITEM it = m_mapPositionOrder.find(nNum);
		if(it==m_mapPositionOrder.end()) return FALSE;

		COrderServiceThread::POSITIONORDER tPosOrder={0};
		tPosOrder = it->second;
		if(nState==conPositionDelete && !(tPosOrder.nState==conPositionPause || tPosOrder.nState==conPositionActive))
			return FALSE;
		tPosOrder.nState = nState;
		m_mapPositionOrder.erase(nNum);
		OrderConditionDelete(tPosOrder.nCondNum_StopLose);
		OrderConditionDelete(tPosOrder.nCondNum_StopGain);
		m_mapPositionOrder_Overdue[nNum]=tPosOrder;
		if(bIsPostEvent)
		{
			PostEvent_PositionStatus(nNum);
			// ��ʱ���÷����������Ϊ�յ��¼���ֻ�ı���صļ�¼ʱ��ɾ������÷�
			PostEvent_ConditionStatus(tPosOrder.nCondNum_StopLose);
		}
		return TRUE;
	};
	BOOL OrderPositionClear(BOOL bIsLock=FALSE) {
		m_mapPositionOrder.clear();
	};

	BOOL HasNoTouchLocalOrder();

	// LockObject()��UnloadObject()��������������ɶԵ���
	void LockObject() {
		m_oMutex.Lock();
	};
	void UnlockObject() {
		m_oMutex.Unlock();
	};

	std::map<string, char>& GetExchangeStatusMap() {
		return m_mapExchangeStatus;
	};

	void RegisterMsgWindow(void* pMsgWindow) {
		m_setMsgWindow.insert(pMsgWindow);
	};
	void UnregisterMsgWindow(void* pMsgWindow) {
		m_setMsgWindow.erase(pMsgWindow);
	};

private:
	void OrderCondition(CONDITIONORDER& tOrder);
	void OrderParked(PARKEDORDER& tOrder);
	void DoConditionOrder(PlatformStru_DepthMarketData& rawData);
	//void DoPositionOrder(CThostFtdcDepthMarketDataField& rawData);

	void PostEvent_ParkedStatus(int nNum);
	void PostEvent_ConditionStatus(int nNum);
	void PostEvent_PositionStatus(int nNum);
	BOOL LoadDataFromFile();
	BOOL SaveDataToFile();

private:
	MAPBATCHORDER		m_mapBatchOrder;			// ����������
	MAPBATCHSUBORDER	m_mapBatchSubOrder;			// �����������ӵ�
	MAPPARKEDORDER		m_mapParkedOrder;			// ����Ԥ��
	MAPCONDITIONORDER	m_mapConditionOrder;		// ����������
	MAPCONDITIONORDER	m_mapConditionOrder_Overdue;	// ����������
    MAPPOSITIONORDER	m_mapPositionOrder;			// ����ӯ��
    MAPPOSITIONORDER	m_mapPositionOrder_Overdue;		// ����ӯ��

	std::map<string, char> m_mapExchangeStatus;		// ����������״̬
	std::set<void*>		m_setMsgWindow;				// ������Ϣ�Ĵ��ڶ���ָ�뼯��

	wxMutex				m_oMutex;

	string				m_strInvestorID;

};