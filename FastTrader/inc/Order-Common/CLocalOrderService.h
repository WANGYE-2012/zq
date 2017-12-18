
#pragma once

#include "../Module-Misc2/BusinessData.hpp"

	///��ʾһ��Local��¼
#define _MakeKeyDefine(name, order) \
	struct name##Key \
	{ \
		name##Key() \
		{ \
			ZeroMemory(this, sizeof(*this)); \
		};\
\
		name##Key(const long	FTID) \
		{ \
			ZeroMemory(this, sizeof(*this)); \
			this->FTID = FTID; \
		}; \
\
		name##Key(const order& Order) \
		{ \
			ZeroMemory(this, sizeof(*this)); \
			FTID = Order.FTID; \
		}; \
\
		name##Key& operator= (const name##Key& other) \
		{ \
			if (this == &other) \
				return *this; \
\
			memcpy((char*)this,(char*)&other,sizeof(*this)); \
			return *this; \
		}; \
\
		bool operator< (const name##Key& r) const \
		{ \
			if(FTID<r.FTID) \
				return true; \
			if(FTID>r.FTID) \
				return false; \
\
			return false; \
		} \
\
		bool operator== (const name##Key& r) const \
		{ \
			if(FTID!=r.FTID) \
				return false; \
			return true; \
		}\
\
		bool operator> (const name##Key& r) const \
		{ \
			return !((*this) < r || (*this) == r); \
		} \
\
		char* tostring(char* prlt, int rltsize) const \
		{ \
			if(!prlt||!rltsize) return NULL; \
			int len=0,ilen; \
			prlt[len]=0; \
\
			char buf[256]; \
			sprintf(buf, "FTID[%d]", FTID); \
			ilen=strlen(buf); \
			if(len+ilen<rltsize) \
			{ \
				strcat(prlt, buf); \
				len=ilen; \
			} \
\
			return prlt; \
		}\
\
		long	FTID; \
	};


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

//// ӯ��״̬
//enum {	conPLPause,				// ��ͣ
//		conPLActive,			// ����
//		conPLDelete,			// ɾ��
//		conPLConfirm,			// ִ��ֹӯֹ��
//		conPLCancel,			// ����ֹӯֹ��
//		conPLCompleted			// ȫ���ɽ�
//};
// ӯ��״̬
enum {	conPLPause,				// ��ͣ
		conPLActive,			// ����µ��ȴ�
		conPLDelete,			// ɾ��
		conPLNoTrade,			// δ�ɽ�
		conPLPartTraded,		// ���ֳɽ�
		conPLAllTraded,			// ȫ���ɽ�
		conPLCancelNoTrade,		// δ�ɽ��ѳ���
		conPLCancelPartTraded,	// ���ֳɽ��ѳ���
		conPLFail				// �µ�ʧ��
};

// �ֲ�ֹӯֹ��״̬
enum {	conPosiPLPause,			// ��ͣ
		conPosiPLActive,		// ����
		conPosiPLDelete,		// ɾ��
		conPosiPLConfirm,		// ִ��ֹӯֹ��
		conPosiPLCancel,		// ����ֹӯֹ��
		conPosiPLCompleted		// ֹӯֹ�����
};

// ӯ����ϸ״̬
enum {	conPLDetailPause,		// ��ͣ
		conPLDetailActive,		// ����
		conPLDetailDelete,		// ɾ��
		conPLDetailConfirm,		// ִ��ֹӯֹ��
		conPLDetailCancel,		// ����ֹӯֹ��
		conPLDetailCompleted	// ȫ���ɽ�
};

// ��������ӯ�𵥵Ĺ�����ϵ��conCondPosRelation
enum {	conCPRNon,				// �޹�ϵ
		conCPRCondFromPL,		// ��������ӯ����ϸ��ֹӯֹ������
		conCPRCondNextPL		// ��������ɺ󼤻�ӯ��
};

// �������۸񴥷�����
enum {	conCndPTypeLast=1,		// 1 ����
		conCndPTypeBid,			// 2 ���
		conCndPTypeAsk			// 3 ����
};

// ������������������
enum {  conCndTypeLessEqual=1, 	//1 С�ڵ���ָ���۸�
		conCndTypeGreaterEqual, //2 ���ڵ���ָ���۸�
		conCndTypeLess, 		//3 С��ָ���۸�
		conCndTypeGreater		//4 ����ָ���۸�
};

// ӯ�𵥼۸�ƫ����������
enum {	conPLPRTradePrice=1,	// 1 �ɽ���
		conPLPRReqPrice			// 2 ί�м�
};

// ӯ�𵥼۸񴥷�����
enum {	conPLPTypeLast=1,		// 1 ����
		conPLPTypeBidAsk		// 2 ������
};

// ӯ��ƽ�ַ�ʽ
enum {	conPLCMBound=1,			// 1 �����ǵ�ͣ��
		conPLCMOffer,			// 2 ����Ӽ���
		conPLCMMarket			// 3 �м�
};


class CLocalOrderService : public wxEvtHandler//, wxThread
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
		DWORD	dwPLOrderNum;
		DWORD	dwPLDetailOrderNum;
		DWORD	dwParkedCount;
		DWORD	dwParkedOffset;
		DWORD	dwParkedSize;
		DWORD	dwCondActiveCount;
		DWORD	dwCondActiveOffset;
		DWORD	dwCondActiveSize;
		DWORD	dwPLCount;
		DWORD	dwPLOffset;
		DWORD	dwPLSize;
		DWORD	dwPLDetailCount;
		DWORD	dwPLDetailOffset;
		DWORD	dwPLDetailSize;
		DWORD	dwBatchSubCount;
		DWORD	dwBatchSubOffset;
		DWORD	dwBatchSubSize;
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


	// �뱨������豣�����ֶ�
	// �����ؼ�����
	typedef	struct {
		// ���ر��
		long						FTID;
		///ǰ�ñ��
		TThostFtdcFrontIDType		FrontID;
		///�Ự���
		TThostFtdcSessionIDType		SessionID;
		///������
		TThostFtdcRequestIDType		RequestID;
		///�����ύ״̬
		TThostFtdcOrderSubmitStatusType	OrderSubmitStatus;
		///����״̬
		TThostFtdcOrderStatusType	OrderStatus;
		///��������
		TThostFtdcOrderRefType		OrderRef;
		///����������
		TThostFtdcExchangeIDType	ExchangeID;
		///�������
		TThostFtdcOrderSysIDType	OrderSysID;
		///�ɽ����
		TThostFtdcTradeIDType		TradeID;
		// �����˺�
		CommonAccountType			Account;
	} ORDERREF, *PORDERREF;


	// �û��µ�����
	typedef struct {
		int		nOrderType;			//�µ�����
		int		nOCMode;			//��ƽ��ʽ      //0����; 1ƽ��; 2ƽ��; -1δ֪
		BOOL	bIsBuy;				//�Ƿ�Ϊ����
		BOOL	bIsMarket;			//�Ƿ�Ϊ�м�
		int	    nHedgeStatus;	    //0/1/2��ӦͶ��/����/��ֵ
		double	fPrice;				//ί�м�
		int		nVolume;			//ί������
		InstrumentIDType	InstrumentID;	//��Լ����
	} ORDERPARAM, *PORDERPARAM;


	// �������ò���
	typedef struct {
		int		nPriceType;			//�۸񴥷����ͣ�1 ���£�2 ��ۣ�3 ����
		int		nConditionType;		//�����������ͣ�1 С�ڵ���ָ���۸�2 ���ڵ���ָ���۸�
		double	fConditionPrice;	//ָ���Ĵ����۸�
	} CONDPARAM, *PCONDPARAM;


	// ӯ�����ò���
	typedef struct {
		int		nPriceOffsetType;	//�۸�ƫ���������ͣ�1 �ɽ��ۣ�2 ί�м�
		BOOL	bDoStopLose;		//ִ��ֹ��
		double	fStopLosePriceOffset;	//ֹ��۲�
		BOOL	bDoStopGain;		//ִ��ֹӯ
		double	fStopGainPriceOffset;	//ֹӯ�۲�
		int		nPriceType;			//�����۸����ͣ�1 ���¼ۣ�2 ������
		int		nCloseMode;			//ƽ�ַ�ʽ��1 �����ǵ�ͣ�ۣ�2 ����Ӽ���
		double	fClosePriceOffset;	//����Ӽ��۲�
	} PLPARAM, *PPLPARAM;


	// ����Ԥ��
	typedef struct {
		int		nNum;				//���
		int		nState;				//�״̬
		int		nRegNum;			//�����
		int		nRegState;			//ί��״̬
		BOOL	bIsAuto;			//�Ƿ�Ϊ�Զ�ģʽ��FALSE �ֶ�������TRUE �Զ�
		DWORD	nTradingDay;		//������
		DWORD	nCreateTime;		//Ԥ��ʱ��
		DWORD	nDeleteTime;		//ɾ��ʱ��
		PlatformStru_InputOrder req;
		TThostFtdcExchangeIDType	ExchangeID;	// ����������
		char	strErrorMsg[256];	//������Ϣ
	} PARKEDORDER, *PPARKEDORDER;


	// ����������
	typedef struct {
		//FasttraderID,����ÿһ����¼��Key������һ��Ψһ���
		long	FTID;                                                
		//ÿ���������ݷ����仯��UpdateSeq��һ
		long	UpdateSeq;
		InstrumentIDType	InstrumentID;	//��Լ����

		//BOOL	bIsRunInServer;		//�Ƿ����ۺϽ���ƽ̨��ִ��ֹӯֹ��
		int		nState;				//�״̬
		int		nCondPLRelation;	//��ӯ�𵥵Ĺ�ϵ
		int		nPLNum;				//ӯ�𵥺�
		int		nPLDetailNum;		//ӯ�𵥺�
		//int		nRelateCondNum;		//������������
		int		nRegState;			//ί��״̬
		ORDERREF	ref;
		ORDERPARAM	orderSrc;
		CONDPARAM	cond;
		DWORD	nTradingDay;		//������
		DWORD	nCreateTime;		//Ԥ��ʱ��
		DWORD	nDeleteTime;		//ɾ��ʱ��
		char	strErrorMsg[256];	//������Ϣ
	} CONDITIONORDER, *PCONDITIONORDER;


	// 2012.7.11�������
	// ӯ�𵥵Ĺ���
	// 1.  ����ָ����ί�п���
	// 2.  ���ɱ���ӯ�𵥲����м���
	// 3.  ���гɽ�ʱ������ӯ����ϸ��������
	// 4.  ӯ����ϸ����ֹӯ��ֹ�����������µ�������ɽ������
	// 5.  ӯ���������༭�󣬲��޸��Ѿ��¹�����������ֻ���µĳɽ��µ�������ʹ��
	// 6.  ӯ�𵥱�ɾ����ֻɾ��ӯ�𵥱�������δ��ɳɽ��Ľ��г���
	// 7.  ӯ����ϸ�������༭��ֻ�޸��������������֮ƥ��ı���������
	// 8.  ӯ����ϸ��ɾ����ɾ��ӯ����ϸ������ص�������


	// ����ӯ��
	typedef struct {
		//FasttraderID,����ÿһ����¼��Key������һ��Ψһ���
		long	FTID;
		//ÿ���������ݷ����仯��UpdateSeq��һ
		long	UpdateSeq;
		InstrumentIDType	InstrumentID;	//��Լ����

		BOOL	bIsRunInServer;		//�Ƿ����ۺϽ���ƽ̨��ִ��ֹӯֹ��
		int		nState;				//�״̬

		int		nRegState;			//����ί��״̬
		ORDERREF	ref;

		PLPARAM		plparam;
		//int		nPriceOffsetType;	//�۸�ƫ���������ͣ�1 �ɽ��ۣ�2 ί�м�
		//BOOL	bDoStopLose;		//ִ��ֹ��
		//double	fStopLosePriceOffset;	//ֹ��۲�
		//BOOL	bDoStopGain;		//ִ��ֹӯ
		//double	fStopGainPriceOffset;	//ֹӯ�۲�
		//int		nPriceType;			//�����۸����ͣ�1 ���¼ۣ�2 ������
		//int		nCloseMode;			//ƽ�ַ�ʽ��1 �����ǵ�ͣ�ۣ�2 ����Ӽ���
		//double	fClosePriceOffset;	//����Ӽ��۲�
		ORDERPARAM	orderSrc;
		//int		nOCMode;			//��ƽ��ʽ
		//BOOL	bIsBuy;				//�Ƿ�Ϊ����
		//BOOL	bIsMarket;			//�Ƿ�Ϊ�м�
		//BOOL	bHedge;				//Ͷ����ֵ
		//double	fRegPrice;			//ί�м�
		//int		nRegVolume;			//ί������
		DWORD	nTradingDay;		//������
		DWORD	nCreateTime;		//Ԥ��ʱ��
		DWORD	nDeleteTime;		//ɾ��ʱ��
	} PLORDER, *PPLORDER;


	// ����ӯ�𵥳ɽ���Ӧ��ϸ
	typedef struct {
		//FasttraderID,����ÿһ����¼��Key������һ��Ψһ���
		long	FTID;
		//ÿ���������ݷ����仯��UpdateSeq��һ
		long	UpdateSeq;
		InstrumentIDType	InstrumentID;	//��Լ����

		BOOL	bIsRunInServer;		//�Ƿ����ۺϽ���ƽ̨��ִ��ֹӯֹ��
		int		nState;				//�״̬
		long	nPLNum;				//ӯ�𵥱��

		ORDERREF ref;

		ORDERPARAM	orderSrc;
		PLPARAM		plparam;

		double	fOpenedPriceOffset;	//�ɽ���ƫ��
		double	fStopLosePrice;		//ֹ���
		double	fStopGainPrice;		//ֹӯ��
		double	fStopLoseForecast;	//ֹ��Ԥ��
		double	fStopGainForecast;	//ֹӯԤ��

		BOOL	bGainIsServ;		//ֹӯ�������Ƿ��ڷ�����������
		BOOL	bLoseIsServ;		//ֹ���������Ƿ��ڷ�����������
		ORDERREF	refGain;		//ֹӯ��������
		ORDERREF	refLose;		//ֹ����������
		int		nGainRegState;		//ֹӯ������ί��״̬
		int		nLoseRegState;		//ֹ��������ί��״̬
		DWORD	nTradingDay;		//������
		DWORD	nCreateTime;		//Ԥ��ʱ��
		DWORD	nDeleteTime;		//ɾ��ʱ��
	} PLDETAILORDER, *PPLDETAILORDER;


	// ����ȴ�ɾ��������Ӧ��ķ������������Ľṹ��
	typedef struct {
		long		nPosiPLNum;		// ֹӯֹ����
		BOOL		bIsLose;		// �Ƿ�Ϊֹ��
		DWORD		dwStarTime;		// ִ��ɾ��������ʱ�䣬���ڳ�ʱ����
		ORDERREF	oldCondRef;		// ��ɾ�������µķ�������������ref�ؼ�����
		ORDERREF	newCondRef;		// ��ɾ�������µķ�������������ref�ؼ�����
		PLPARAM		plparam;		// ӯ������
		ORDERPARAM	newOrder;		// ���µ��µ��ṹ
	} WaitDelOrNew_ServCond, *LPWaitDelOrNew_ServCond;


	_MakeKeyDefine(CondOrder, CONDITIONORDER)
	_MakeKeyDefine(PLOrder, PLORDER)
	_MakeKeyDefine(PLDetailOrder, PLDETAILORDER)


	typedef std::map<long, BATCHORDER> MAPBATCHORDER;
	typedef std::map<long, BATCHORDER>::iterator MAPBATCHORDERITEM;
	typedef std::map<long, BATCHSUBORDER> MAPBATCHSUBORDER;
	typedef std::map<long, BATCHSUBORDER>::iterator MAPBATCHSUBORDERITEM;
	typedef std::map<long, PARKEDORDER> MAPPARKEDORDER;
	typedef std::map<long, PARKEDORDER>::iterator MAPPARKEDORDERITEM;
	typedef std::map<long, CONDITIONORDER> MAPCONDITIONORDER;
	typedef std::map<long, CONDITIONORDER>::iterator MAPCONDITIONORDERITEM;
	typedef std::map<long, PLORDER> MAPPLORDER;
	typedef std::map<long, PLORDER>::iterator MAPPLORDERITEM;
	typedef std::map<long, PLDETAILORDER> MAPPLDETAILORDER;
	typedef std::map<long, PLDETAILORDER>::iterator MAPPLDETAILORDERITEM;

public:
	
	static CLocalOrderService* GetInstance();
    static CLocalOrderService* CreateThread();


	CLocalOrderService();//wxWindow *parent, wxWindowID winid);
	~CLocalOrderService();

	BOOL InitService();

	void OnRcvLogon(wxCommandEvent& evt);
	void OnInitSubscribe(wxCommandEvent& evt);
	void OnRcvQuot(wxCommandEvent& evt);
	void OnRcvOrder(wxCommandEvent& evt);
	void OnRcvTrade(wxCommandEvent& evt);
	
	void OnPlatSvrStatusChange(wxCommandEvent& evt);
	void OnStopPlatform(wxCommandEvent& evt);
    void OnTimer();
	DECLARE_EVENT_TABLE()
public:
	static int CallBack_UserLogin(const GUIModuleID,const AbstractBusinessData &data);
	static int CallBack_InstrumentStatus(const GUIModuleID,const AbstractBusinessData &data);
	static int CallBack_DepthMarketCallBack(const GUIModuleID,const AbstractBusinessData& data);
	static int CallBack_RspOrderInsertCallBackFunc(const GUIModuleID,const AbstractBusinessData& data);
	static int CallBack_TradeCallBackFunc(const GUIModuleID,const AbstractBusinessData &data);

	//static BOOL MakeLocalCondOrder_PLDetail(PLORDER& tPLOrder, PLDETAILORDER& tPLDetail, 
	//			CONDITIONORDER& tCondGain, CONDITIONORDER& tCondLose);
	static BOOL MakeLocalCondOrder_PLDetail(PLDETAILORDER& tPLDetail, 
				CONDITIONORDER& tCondGain, CONDITIONORDER& tCondLose);
	static BOOL ConvertPLD2Cond(ORDERPARAM& orderPL, BOOL bIsLose, 
								PLPARAM& plparam, CONDPARAM& cond, ORDERPARAM& orderNew);
	static BOOL ConvertLocal2ReqOrder(CONDITIONORDER& tCondOrder, PlatformStru_InputOrder& req);
	static BOOL ConvertLocal2ServCond(CONDITIONORDER& tCondOrder, PlatformStru_InputOrder& req);
	static BOOL ConvertServ2LocalCond(CThostFtdcInputOrderField& inputOrder, CONDITIONORDER& tCondOrder);
	static BOOL CopyOrderRef(ORDERREF& ref, PlatformStru_OrderInfo& order);

//	static int	sm_nReqNum;
	static int	sm_nBatchOrderNum;
	static int	sm_nBatchSubOrderNum;
	static int	sm_nParkedOrderNum;
	static int	sm_nConditionOrderNum;
	static int	sm_nPLOrderNum;
	static int	sm_nPLDetailOrderNum;

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
	MAPPLORDER& GetOrderPLMap() {
		return m_mapPLOrder;
	};
	MAPPLDETAILORDER& GetOrderPLDetailMap() {
		return m_mapPLDetailOrder;
	};

	long OrderBatchAdd(BATCHORDER& tOrder);
	BOOL OrderBatchDelete(long nNum);
	void OrderBatchClear();
	BOOL OrderBatchActive(long nNum, BOOL bActive);

	long OrderBatchSubAdd(BATCHSUBORDER& tOrder);
	BOOL OrderBatchSubDelete(long nNum);
	void OrderBatchSubClear();

	long OrderParkedAdd(PARKEDORDER& tOrder);
	BOOL OrderParkedSet(long nNum, PARKEDORDER& tOrder);
	BOOL OrderParkedDelete(long nNum);
	void OrderParkedClear();
	BOOL OrderParkedActive(long nNum, BOOL bActive);
	BOOL OrderParkedDoNow(long nNum);

	long OrderConditionAdd(CONDITIONORDER& tOrder);
	BOOL OrderConditionAdd_PLDetail(CONDITIONORDER& tOrder1, CONDITIONORDER& tOrder2);
	BOOL OrderConditionSet(long nNum, CONDITIONORDER& tOrder);
	BOOL OrderConditionDelete(long nNum, int nState=conCondDelete, BOOL bIsPostEvent=FALSE);

	void OrderConditionClear();
	BOOL OrderConditionActive(long nNum, BOOL bActive);
	BOOL OrderConditionDoNow(long nNum);

	long OrderPLAdd(PLORDER& tOrder);
	BOOL OrderPLSet(long nNum, PLORDER& tOrder);
	BOOL OrderPLDelete(long nNum, int nState=conPLDelete, BOOL bIsPostEvent=FALSE);
	void OrderPLClear();

	long OrderPLDetailAdd(PLDETAILORDER& tOrder);
	BOOL OrderPLDetailSet(long nNum, PLDETAILORDER& tOrder);
	BOOL OrderPLDetailDelete(long nNum, int nState=conPLDetailDelete, BOOL bIsPostEvent=FALSE);
	BOOL OrderPLDetailDeleteAll(int nState=conPLDetailDelete, BOOL bIsPostEvent=FALSE);
	BOOL OrderPLDetailChange(int nNum, int nSubVolume);
	void OrderPLDetailClear();
	BOOL ConvertServ2Local_PLDetail(long nNum, BOOL bIsLose);
	BOOL AddPosiPLAndRun(PLDETAILORDER& tPLDetail);

	BOOL HasNoTouchLocalOrder();

	void CheckLocalOrder();
	int GetCondOrderVolume(BOOL bIsServ, ORDERREF& condRef, std::string& strInstruemtID);
	int	GetPosiPLVolume(PLDETAILORDER& posiPLOrder);
	// nModeȡֵ, 0 ������, 1 ƽ��, 2 ƽ��
	int GetPosiPLVolume(std::string& strAccount, std::string& strInstrumentID, 
						BOOL bIsBuy, int nHedgeStatus, int nMode=0);
	BOOL AsyncVolumePosiAndPL(PlatformStru_Position& posiInfo);
	void AsyncPosiPLOrderVolume();

	// LockObject()��UnloadObject()��������������ɶԵ���
	void LockObject() {
		m_oMutex.Lock();
	};
	void UnlockObject() {
		m_oMutex.Unlock();
	};

	std::map<std::string, char>& GetExchangeStatusMap() {
		return m_mapExchangeStatus;
	};

	void RegisterMsgWindow(void* pMsgWindow) {
		m_setMsgWindow.insert(pMsgWindow);
	};
	void UnregisterMsgWindow(void* pMsgWindow) {
		m_setMsgWindow.erase(pMsgWindow);
	};

private:
	BOOL MakePLDetailAndRun(PLORDER& tPLOrder, PlatformStru_TradeInfo& traderInfo);
	void OrderCondition(CONDITIONORDER& tOrder, BOOL bIsServCond=FALSE);
	void OrderParked(PARKEDORDER& tOrder);
	void DoConditionOrder(PlatformStru_DepthMarketData& rawData);
	//void DoPositionOrder(CThostFtdcDepthMarketDataField& rawData);
	int  CancelCondServer(ORDERREF& ref, char* InstrumentID);
	BOOL OrderCondServDelete_PLDetail(PLDETAILORDER plDetail, BOOL bIsPostEvent=FALSE); 
	BOOL CheckOverTradingDay();

	void PostEvent_ParkedStatus(long nNum);
	void PostEvent_ConditionStatus(long nNum);
	// nMode, 0 ������ pInputOrder��Ч��1 ֹӯ, 2 ֹ�� nNum��Ч ��PLDetail�ı��
	void PostEvent_CondServFail(CThostFtdcInputOrderField* pInputOrder, long nNum, int nMode);
	void PostEvent_PLStatus(long nNum);
	void PostEvent_PLDetailStatus(long nNum, long nLPNum);
	BOOL LoadDataFromFile();
	BOOL SaveDataToFile();

private:
	MAPBATCHORDER		m_mapBatchOrder;			// ����������
	MAPBATCHSUBORDER	m_mapBatchSubOrder;			// �����������ӵ�
	MAPPARKEDORDER		m_mapParkedOrder;			// ����Ԥ��
	MAPCONDITIONORDER	m_mapConditionOrder;		// ����������
    MAPPLORDER			m_mapPLOrder;				// ����ӯ��
    MAPPLDETAILORDER	m_mapPLDetailOrder;			// ����ӯ����ϸ

	// �ȴ�ɾ��Ӧ��ķ�������������������ֹӯֹ��ĸĵ�
	std::map<long, WaitDelOrNew_ServCond>	m_mapWaitDelServCond;
	// �ȴ��µ�Ӧ��ķ�������������������ֹӯֹ��ĸĵ�
	std::map<long, WaitDelOrNew_ServCond>	m_mapWaitNewServCond;

	std::map<std::string, char> m_mapExchangeStatus;		// ����������״̬
	std::set<void*>		m_setMsgWindow;				// ������Ϣ�Ĵ��ڶ���ָ�뼯��

	wxMutex				m_oMutex;
    //wxTimer				m_timer;

	std::string			m_strInvestorID;
	DWORD				m_nTradingDay;

};