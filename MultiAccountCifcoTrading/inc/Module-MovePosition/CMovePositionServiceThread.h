#pragma once
#include "stdafx.h"
#include <vector>
#include "CfgMgr/CfgGlobal.h"

#include <wx/listctrl.h>


class CMovePositionServiceThread : public wxEvtHandler//, wxThread
{
public:
	// ����۸�����
	enum {	conPriceType_Market,		// �м�
			conPriceType_LastPrice,		// ���¼�
			conPriceType_BuySellPrice,	// ��һ��һ��
			conPriceType_Limit			// ָ����
	};

	// ��λƽ���ֳ�ʱ��Ļ 
	enum {	conTOutAction_Nothing,		// �޶���
			conTOutAction_LastPrice,	// �����¼۸����ĵ�
			conTOutAction_BuySellPrice,	// �������۸����ĵ�
			conTOutAction_Market,		// ���м۸ĵ�
			conTOutAction_CancelOrder	// ����
	};

	// �³�������״̬״̬
	enum {	conPosRegWaitingSend,		// δ����
			conPosRegOrderWaiting,		// �ҵ��ȴ�Ӧ��
			conPosRegOrderHasResponsed,	// ������Ӧ��
			conPosRegOrderFail,			// �ҵ�ʧ��
			conPosRegOrderCompleted,	// ȫ���ɽ�
			conPosRegCancelWaiting,		// �����ȴ�
			conPosRegCancelHasResponsed,	// ������Ӧ��
			conPosRegCancelFail,		// ����ʧ��
			conPosRegCancel				// �����ɹ�
	};

	// ��λ״̬
	enum {	conPositionPause,			// ��ͣ
			conPositionActive,			// ����
			//conPositionOrderWaiting,	// �ҵ��ȴ��ɽ�
			//conPositionOrderFail,		// �ҵ�ʧ��
			//conPositionCancelWaiting,	// �����ȴ�
			//conPositionCancelFail,	// ����ʧ��
			conPositionCancel,			// �ѳ����
			conPositionCompleted		// ȫ���ɽ�
	};

	// �Ʋ�״̬
	enum {	conMovePause,				// ��ͣ
			conMoveActive,				// ����
			conMoveCancel,				// �ѳ����
			conMoveCompleted			// ���
	};

	// ��λ����״̬
	enum {	conSwapPause,				// ��ͣ
			conSwapActive,				// ����
			conSwapCancel,				// �ѳ����
			conSwapCompleted			// ���
	};

	enum {	conActionPause,
			conActionMoving,			//1 ��ʼ�Ʋֻ��λ����
			conActionUserChanging,		//2 ִ����Χ�ĵ�
			conActionAutoChanging,		//3 ִ�г�ʱ�ĵ�
			conActionChanged,			//4 �ĵ����
			conActionCompleted,			//5 �Ʋ����
			conActionCancel,			//6 �û�����
			conActionCancel_TimeOut,	//7 ��ʱ����
			conActionCancel_Exciption,	//8 �쳣����
			conActionMoveCompleted,		//9 �Ʋֵ����
			conActionSwapCompleted		//10��λ�������
	};

	typedef struct {
		DWORD	dwDate;
		DWORD	dwTime;
		DWORD	dwMovePosNum;
		DWORD	dwSwapPosNum;
		DWORD	dwMovePosCount;
		DWORD	dwMovePosOffset;
		DWORD	dwMovePosSize;
		DWORD	dwSwapPosCount;
		DWORD	dwSwapPosOffset;
		DWORD	dwSwapPosSize;
		DWORD	dwBak1[24];
	} FILEHEAD, *PFILEHEAD;		// sizeof()=64


	// ��λ����
	typedef struct {
		InstrumentIDType	strInstrumentID;// ��Լ����
		double		fPriceTick;
		BOOL 		bDirection;			// ��������, TRUE ����, FALSE ����
		BOOL		bHedge;				// Ͷ����ֵ
		BOOL		bIsClose;			// �Ƿ�Ϊƽ��
		int			nPriceType;			// ����۸�����, 0 �м�, 1 ���¼�, 2 ��һ��һ��, 3 ָ����
		BOOL		bSimulate;			// ���ǵ�ͣ��ģ���м�
		double		fPrice;				// �۸�
		int			nPriceOffset;		// �����۲�, ��ʾΪ�۸�䶯��λ�ĳ���
		int			nVolume;			// �ֲ�����
		int			nTimeOutAction;		// ԭʼ��λƽ�ֳ�ʱ��Ļ, 0 �޶���, 1 �����¼۸����ĵ�, 2 �������۸����ĵ�, 3 ���м۸ĵ�, 4 ����, 5 �����۸��޸ĶԻ���
		int			nTimeOutPriceOffset;// ԭʼ��λ�����۲�

		int			nState;				// �״̬, �� ��λ״̬enum
		int			nRemainTime;		// ʣ��ʱ�䣬��λ����
		int			nRetryCount;		// �����Դ���

		BOOL		bIsAuto;			// ִ���Զ���
		int			nRegState;			// �³���״̬
		int			nRegNum;			// �����
		int			nCancelRegNum;		// ���������
		int			nRemainVolume;		// ʣ������
		char		strOrderSysID[32];	// ί�е����
		PlatformStru_InputOrder				req;			// ��λƽ��������
		PlatformStru_InputOrderAction		cancelAction;
		
		BOOL		bIsAuto_CloseToday;				// ִ���Զ���
		int			nRegState_CloseToday;			// �³���״̬
		int			nRegNum_CloseToday;				// �����
		int			nCancelRegNum_CloseToday;		// ���������
		int			nRemainVolume_CloseToday;		// ʣ������
		char		strOrderSysID_CloseToday[32];	// ί�е����
		PlatformStru_InputOrder				req_CloseToday;	// ��λƽ������
		PlatformStru_InputOrderAction		cancelAction_CloseToday;

	} POSITIONUNIT, *PPOSITIONUNIT;


	// �Ʋֽṹ��
	typedef struct {
		int			nNum;				// ���
		int			nState;				// �״̬, �� �Ʋ�״̬enum
		BOOL		bPauseTimer;		// ��ͣ��ʱ����(����ʱ)
		void*		pWnd;
		POSITIONUNIT	tSrcPos;		// ԭʼ��λ
		POSITIONUNIT	tDestPos;		// Ŀ���λ
		int			nActionMode;		// ����˳��, 0 ��ƽ��, 1 ͬʱ��ƽ
		int			nTimeOutInterval;	// ��ʱ�������λ����
		int			nRetryNum;			// ��ʱ���Դ���
		int			nRelateMode;		// ������ϵ, 0 �����޹���, 1 �ǲ�λ�������Ʋֵ�
		int			nRelateID;			// ������ID��, ���λ�����ĵ���
	} MOVEPOSITION, *PMOVEPOSITION;

	// ��λ�����ṹ��
	typedef struct {
		int			nNum;				// ���
		int			nState;				// �״̬, �� ��λ����״̬enum
		int			nMovePos1Num;		// ��λ1���
		int			nMovePos2Num;		// ��λ2���
		int			nActionMode;		// ����˳��, 0 ͬʱ����, 1 �ȴ���λ1�����, 2 �ȴ���λ2�����
	} SWAPPOSITION, *PSWAPPOSITION;

	typedef std::map<int, MOVEPOSITION> MAPMOVEPOSITION;
	typedef std::map<int, MOVEPOSITION>::iterator MAPMOVEPOSITIONITEM;
	typedef std::map<int, SWAPPOSITION> MAPSWAPPOSITION;
	typedef std::map<int, SWAPPOSITION>::iterator MAPSWAPPOSITIONITEM;

public:
	
	static CMovePositionServiceThread* GetInstance();
	static void ReleaseInstance();


	CMovePositionServiceThread();//wxWindow *parent, wxWindowID winid);
	~CMovePositionServiceThread();

	BOOL InitService();

	void OnTimer();
	//void OnTimer(wxTimerEvent& evt);
	void OnInitSubscribe(wxCommandEvent& evt);
	void OnRcvOrder(wxCommandEvent& evt);
	void OnRcvTrade(wxCommandEvent& evt);
	
	DECLARE_EVENT_TABLE()
public:
    static CMovePositionServiceThread* CreateThread();
	static BOOL MakeReq(wxString& strInstrumentID, BOOL bDirection, int nOCMode, 
					 double fPrice, int nVolume, BOOL bIsHedge, BOOL bIsMarketPrice, 
					 BOOL bSimulate, PlatformStru_InputOrder& req);
	//static BOOL MakeReq(wxString& strInstrumentID, BOOL bDirection, int nOCMode, 
	//				 double fPrice, int nVolume, BOOL bIsHedge, BOOL bIsMarketPrice, 
	//				 BOOL bSimulate, CThostFtdcInputOrderField& req);

	static int RspOrderInsertCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
	static int TradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);

	static int	sm_nMovePosNum;
	static int	sm_nSwapPosNum;

public:
	MAPMOVEPOSITION& GetMovePostionMap() {
		return m_mapMovePosition;
	};
	MAPSWAPPOSITION& GetSwapPositionMap() {
		return m_mapSwapPosition;
	};

	// ����һ���Ʋֵ�������ֵ: ����Ʋֵĵײ���
	int MovePositionAdd(MOVEPOSITION& tMove) {
		sm_nMovePosNum++;
		tMove.nNum = sm_nMovePosNum;
		tMove.nRelateMode = 0;
		tMove.nState = conMovePause;
		tMove.tSrcPos.nState = conPositionPause;
		tMove.tDestPos.nState = conPositionPause;
		m_mapMovePosition[tMove.nNum] = tMove;
		return tMove.nNum;
	};
	//// ���¸����ΪnNum���Ʋֵ���ֵ
	//BOOL MovePositionSet(int nNum, MOVEPOSITION& tMove) {
	//	tMove.nNum = nNum;
	//	m_mapMovePosition[nNum] = tMove;
	//	return TRUE;
	//};
	//// ��λ����ͬʱ�������Ʋֵ�
	//BOOL MovePositionAdd_SwapPosition(MOVEPOSITION& tMove1, MOVEPOSITION& tMove2) {
	//	sm_nMovePosNum++;
	//	tMove1.nNum = sm_nMovePosNum;
	//	//tMove1.nState = conPositionPause;

	//	sm_nMovePosNum++;
	//	tMove2.nNum = sm_nMovePosNum;
	//	//tMove2.nState = conPositionPause;
	//	
	//	m_mapMovePosition[tMove1.nNum] = tMove1;
	//	m_mapMovePosition[tMove2.nNum] = tMove2;
	//	return TRUE;
	//};
	// �������ΪnNum���Ʋֵ���ʹ�䴦����ֹ״̬
	BOOL MovePositionCancel(int nNum, int nState=conMoveCancel) {
		MAPMOVEPOSITIONITEM it = m_mapMovePosition.find(nNum);
		if(it==m_mapMovePosition.end()) 
			return FALSE;
		//if(it->second.nState!=conCondDelete)
		it->second.nState = nState;
		//it->second.nDeleteTime = ::wxGetUTCTime();
		return TRUE;
	};
	// �������ΪnNum���Ʋֵ���ʹ�䴦����ֹ״̬
	BOOL MovePositionPauseTimer(int nNum, BOOL nPauseTimer=TRUE) {
		MAPMOVEPOSITIONITEM it = m_mapMovePosition.find(nNum);
		if(it==m_mapMovePosition.end()) 
			return FALSE;
		//if(it->second.nState!=conCondDelete)
		it->second.bPauseTimer = nPauseTimer;
		//it->second.nDeleteTime = ::wxGetUTCTime();
		return TRUE;
	};
	// ������е��Ʋֵ�
	BOOL MovePositionClear() {
		m_mapMovePosition.clear();
	};
	// ִ�б��ΪnNum���Ʋֵ�
	BOOL MovePositionDoNow(int nNum);
	// ����ִ�б��ΪnNum���Ʋֵ�
	BOOL MovePositionRedo(int nNum, MOVEPOSITION& tTmpMove, BOOL& bIsEqual);
	// ĳ���Ʋֵ��Ƿ��иĵ����ڽ�����	
	BOOL MovePositionCheckAction(int nNum, int& nAction);	

	// ����һ����λ������������ֵ: �����λ�������ĵײ���
	//int SwapPositionAdd(SWAPPOSITION& tSwap) {
	//	sm_nSwapPosNum++;
	//	tSwap.nNum = sm_nSwapPosNum;
	//	m_mapSwapPosition[tSwap.nNum] = tSwap;
	//	return sm_nSwapPosNum;
	//};
	// ����һ����λ������������ֵ: �����λ�������ĵײ���
	int SwapPositionAdd(SWAPPOSITION& tSwap, MOVEPOSITION& tMove1, MOVEPOSITION& tMove2) {
		sm_nSwapPosNum++;
		tSwap.nNum = sm_nSwapPosNum;
		tSwap.nState = conSwapPause;

		sm_nMovePosNum++;
		tMove1.nNum = sm_nMovePosNum;
		tMove1.nRelateMode = 1;
		tMove1.nRelateID = tSwap.nNum;
		tMove1.nState = conPositionPause;
		tMove1.tSrcPos.nState = conPositionPause;
		tMove1.tDestPos.nState = conPositionPause;
		tSwap.nMovePos1Num = tMove1.nNum;

		sm_nMovePosNum++;
		tMove2.nNum = sm_nMovePosNum;
		tMove2.nRelateMode = 1;
		tMove2.nRelateID = tSwap.nNum;
		tMove2.nState = conPositionPause;
		tMove2.tSrcPos.nState = conPositionPause;
		tMove2.tDestPos.nState = conPositionPause;
		tSwap.nMovePos2Num = tMove2.nNum;
		
		m_mapMovePosition[tMove1.nNum] = tMove1;
		m_mapMovePosition[tMove2.nNum] = tMove2;

		m_mapSwapPosition[tSwap.nNum] = tSwap;
		return sm_nSwapPosNum;
	};
	//// ���¸����ΪnNum���Ʋֵ���ֵ
	//BOOL SwapPositionSet(int nNum, SWAPPOSITION& tSwap) {
	//	tSwap.nNum = nNum;
	//	m_mapSwapPosition[nNum] = tSwap;
	//	return TRUE;
	//};
	// �������ΪnNum���Ʋֵ���ʹ�䴦����ֹ״̬
	BOOL SwapPositionCancel(int nNum, int nState=conSwapCancel) {
		MAPSWAPPOSITIONITEM it = m_mapSwapPosition.find(nNum);
		if(it==m_mapSwapPosition.end()) 
			return FALSE;
		SWAPPOSITION& tSwap = it->second;
		tSwap.nState = nState;
		MovePositionCancel(tSwap.nMovePos1Num);
		MovePositionCancel(tSwap.nMovePos2Num);
		return TRUE;
	};
	// ������е��Ʋֵ�
	BOOL SwapPositionClear() {
		m_mapSwapPosition.clear();
	};
	// ִ�б��ΪnNum�Ĳ�λ������
	BOOL SwapPositionDoNow(int nNum);
	// ����ִ�б��ΪnNum�Ĳ�λ������
	BOOL SwapPositionRedo(int nNum, MOVEPOSITION& tMove1, MOVEPOSITION& tMove2, BOOL& bIsEqual);
	// ĳ���Ʋֵ��Ƿ��иĵ����ڽ�����	
	BOOL SwapPositionCheckAction(int nNum, int& nAction);	


	// LockObject()��UnloadObject()��������������ɶԵ���
	void LockObject() {
		m_oMutex.Lock();
	};
	void UnlockObject() {
		m_oMutex.Unlock();
	};


private:
	BOOL OrderInsert(POSITIONUNIT& tPosition, BOOL bIsCloseToday = FALSE);
	BOOL OrderCancel(POSITIONUNIT& tPosition, BOOL bIsCloseToday = FALSE);
	BOOL DoPositionOrder(POSITIONUNIT& tPosition, BOOL bIsTimeOut = FALSE);
	BOOL DoPositionOrder_ContinueTOut(POSITIONUNIT& tPosition, BOOL bIsCloseToday = FALSE);

	BOOL LoadDataFromFile();
	BOOL SaveDataToFile();

private:
	MAPMOVEPOSITION		m_mapMovePosition;			// �Ʋֵ�
	MAPSWAPPOSITION		m_mapSwapPosition;			// ��λ������

	wxMutex				m_oMutex;

	string				m_strInvestorID;

};
