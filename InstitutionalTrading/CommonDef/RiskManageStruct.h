#pragma once
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
#include <map>
#include <set>
#include <vector>
#include <iostream>
#pragma pack(push)
#pragma pack(1)
typedef int RiskLevelType;
typedef unsigned long RSCOLORREF;
typedef char RiskProductID[300];
typedef int RiskEventIDType;
typedef int RiskEventSubIDType;
typedef __int64 RSTIME;
typedef int TradeInvestorIDType;
typedef int EventHandlingIDType;
typedef char USERNAMESTRING[21];
typedef char CommentType[201];
typedef  char MsgContent[1001];
typedef char ACCOUNTSTRING[21];
typedef	 char MsgTitle[41];
typedef char INDICATORNAMESTRING[41];
typedef char RESPONSENAMESTRING[41];
typedef  char RiskItemDesc[101];
typedef int   TradeAccountIDType;
typedef  char MsgContent[1001];
typedef char DateType[21];
typedef char ACCOUNTSTRING[21];
typedef int RiskMgmtUserIDType;
typedef char TInstrumentIDType[31];
typedef char BrokerIDType[11];
typedef char RiskTime[6];
typedef char TraderAccount[13];
///��
#define T_PD_Net '1'
///��ͷ
#define T_PD_Long '2'
///��ͷ
#define T_PD_Short '3'
//����ָ������
enum RiskIndicatorType 
{
	RI_MaxPosition = 1,			//��Լ���ֲ�;   ��Լ�ֲ�����������������ܣ�
	RI_MarketShock,				//�г��������;   ��Լ�ֲ�����������������ܣ�ռ�г��ֲܳ����ٷֱ�     
	RI_AccountLoss,				//�˻��޿�		  �����ն�̬Ȩ�� -- �˻���ʼȨ�棩/�˻���ʼȨ��
	RI_TodayLoss,				//�����޿�		  �˻����տ���ռ�ճ�Ȩ��İٷֱ�
	RI_GappedMarket,			//��ҹ����        �˻�����Լ�ֲ��ۺϺ����������кͣ���Ԥ��ڶ���������տ���ռ�˻���ʼȨ��ٷֱ�
	RI_MarginRatio,				//��֤���������  ��֤��Ͷ�̬Ȩ��ı�ֵ�����նȣ�
	RI_SingleHoldTime,          //���ʳֲ�ʱ��    
	RI_FundNetValue,			//����ֵ����	
	RI_TRADE_FORBID,			//��ͬ����أ����׺�Լ���ƣ�Ҳ�п������Ƶ���Ʒ�֣�
	RI_MARGIN_FORBID,			//��ͬ����أ���֤��ʹ������
	RI_LOSS_FORBID,			    //��ͬ����أ���������


	RI_ONEDAY_LARGESTLOSS,      //��˾����أ�����������
	RI_LOSSMAXVALUE,			//��˾����أ��������ֵ
	RI_MAXRETRACE,				//��˾����أ����س�ֵ
	RI_CONTRACTS_VALUES,        //��˾����أ��˻��ֲֺ�Լ��ֵ
	RI_BULLBEAR_VALUES,         //��˾����أ���������Լ��ֵ
	RI_LOSS_CONTINUEDAYS,       //��˾����أ�������������
	RI_LOSS_PERCENT,			//��˾����أ��������������


	RI_Count,					//�ܹ�������Ŀ
};

enum RiskWarningType
{	
	WarningType_None                    = 0x0000000,
	WarningType_ForbidOpen_Single		= 0x0000001,			//���ƿ���(����Լ)
	WarningType_ForbidOpen				= 0x0000002,			//���ƿ���
	WarningType_ForbidOrder_Single		= 0x0000004,			//�����µ�(����Լ)
	WarningType_ForbidOrder				= 0x0000010,			//�����µ�
	WarningType_ForceClose_Single		= 0x0000020,			//�м�ǿƽ������Լ��
	WarningType_ForceClose				= 0x0000040,			//�м�ǿƽ
	
	WarningType_Beep					= 0x0000080,			//����
	WarningType_Vibrate					= 0x0000100,			//��
	WarningType_Both					= 0x0000200,			//���Ѽ���
    WarningType_ShowMsg					= 0x0000400,			//��Ϣ��ʾ(������Ϣ��)
	WarningType_ForceClose_SingleOpen	= 0x0000800,			//���ʳֲ�ǿƽ
};

//����ֵ��������
struct NetFundParam
{
	int nOrgID;				//����ID
	double dInnerVolumn;	//���ڷݶ�
	double dOuterVolumn;	//����ݶ�
	double dOuterNetAsset;	//�����ʲ���ֵ
	int nInnerNetAssetOption;	//�����ʲ���ֵѡ��
};
//����ֵÿ�վ�ֵ���
struct NetFundCalcResult
{
	int nTradeAccountID;
	double dInnerVolumn;
	double dOuterVolumn;
	double dInnerNetAsset;
	double dOuterNetAsset;
	double dInnerPerNet;
	double dOuterPerNet;
	double dTotalNetAsset;
	DateType szUpdateDate;
};

struct AccountLossDay
{
	BrokerIDType	BrokerID;
	TraderAccount	AccountID;	
	double dynamicProfit;	
	int    nlossdaycount;
	DateType szLastDate;
	double maxdynamicProfit;//�˻�һֱ��������߶�̬��ֵ
	AccountLossDay& operator=(const AccountLossDay& r) 
	{
		strcpy(BrokerID, r.BrokerID);
		strcpy(AccountID, r.AccountID);
		dynamicProfit = r.dynamicProfit;
		nlossdaycount = r.nlossdaycount;
		strcpy(szLastDate, r.szLastDate);
		maxdynamicProfit = r.maxdynamicProfit;
		return *this;
	}
};

//������յȼ�
struct RiskWaring
{
	int					nOrgID;				//��֯����ID	
	char				cTimeBegin[6];		//00:00
	char				cTimeEnd[6];		//24:00
	RiskIndicatorType	nRiskIndicatorID;	//����ָ������

	RiskLevelType	    nRiskLevelID;			//���յȼ�
	float			    fThresholdValue;      //����ֵ 
	int				    nResponseType;        //����

	RSCOLORREF		    nColor;               //��ɫ
	RiskWaring()
	{
		memset(this,0,sizeof(*this));
		strcpy(cTimeBegin, "00:00");
		strcpy(cTimeEnd, "24:00");
	}
};
//��֯�����ͷ�ط����Ĺ�ϵ
struct RiskOrgIDPlanRelation
{
	int					nOrgID;				//��֯����ID
	int					nRiskType;			//0����ͬ��� 1����˾���
	char				cTimeBegin[6];		//00:00
	char				cTimeEnd[6];		//24:00
	RiskIndicatorType	nRiskIndicatorID;	//����ָ������
	
	RiskProductID		ProductID;			//Ʒ������
	bool				bUse;				//�Ƿ����ø÷�ط���	

	//��ͬ�������Ҫ��������Ҫ�����ProductID��
	char				cInstruments[300];  //���׺�Լ���ƣ������Ʒ�����ƿ������ڸ�ֵ��Ʒ�֡�
	double				dbMarginDividDynamic;//��֤��/��̬Ȩ��
	double				dbMarginUse;		 //ʹ�ñ�֤��	
	double				dbLossAmount;		//�����ܶ�
	double				dbLossPercent;      //�������
	char				szPlan[32];			//��ط�������


	//ԭ����صı�������ȫ��Ʒ�ֻ��� �ض��ļ���Ʒ�֣�ProductID��
	bool				bAllProduct;		//ȫ��Ʒ��
	RiskOrgIDPlanRelation()
	{
		memset(this,0,sizeof(*this));
		//Ĭ�ϵķ���ָ��24Сʱ��Ч
		strcpy(cTimeBegin, "00:00");
		strcpy(cTimeEnd, "24:00");
	}
};
//��֯������ĳһ������
struct RiskPlan
{
	RiskOrgIDPlanRelation		OrgIDPlanRelation;
	RiskWaring					WaringLevel;
	NetFundParam				netFundParam;//ֻ������ֵ����ָ������ʹ��
	RiskPlan()
	{
		memset(this,0,sizeof(*this));
		strcpy(OrgIDPlanRelation.cTimeBegin, "00:00");
		strcpy(OrgIDPlanRelation.cTimeEnd, "24:00");
		strcpy(WaringLevel.cTimeBegin, "00:00");
		strcpy(WaringLevel.cTimeEnd, "24:00");
	}
	bool operator<(const RiskPlan&	r) const
	{		
		if(WaringLevel.nOrgID < r.WaringLevel.nOrgID)
			return true;
		else if(WaringLevel.nOrgID >r.WaringLevel.nOrgID)
			return false;

		if(WaringLevel.nRiskIndicatorID<r.WaringLevel.nRiskIndicatorID)
			return true;
		else if(WaringLevel.nRiskIndicatorID>r.WaringLevel.nRiskIndicatorID)
			return false;

		if(WaringLevel.nRiskLevelID<r.WaringLevel.nRiskLevelID)
			return true;
		else if(WaringLevel.nRiskLevelID>r.WaringLevel.nRiskLevelID)
			return false;

		return false;
	}
};
//����ָ�����ã�ͣ�ã�����ɾ���õ������ݽṹ
struct RiskChange
{
	int					nOrgID;				//��֯����ID
	RiskIndicatorType	nRiskIndicatorID;	//����ָ������
	bool				bUse;				//true:Ϊ������falseΪͣ��
};
struct SResponse
{	
	int nRiskEventID;						//�����¼�ID
	std::map<int,RiskPlan>  mapResponse;   //���յȼ�
	std::set<std::string>   setProducts;  //��Ҫ������ֲ֣��г�������գ��޶���Ʒ������
	std::set<std::string>   setInstruments;//���ͬʱ��Ʒ�ֺͺ�Լ���Ƶ�����������ͬ��أ����ƽ��������е�����Լ�����
	SResponse()
	{
		nRiskEventID = 0;
	}
};
struct SRiskKey
{//ĳһ�˻�ĳ�ַ��յ�Ψһ�Ա�ʶ
	RiskIndicatorType	nRiskIndicatorID;	//����ָ������
	std::string			strInstrument;      //��Լ���ƣ���Ҫ��������ֲ�֮���
	PositionDetailKey   PostionDetail; //
	SRiskKey()
	{
		memset(this,0,sizeof(*this));
		nRiskIndicatorID = RI_Count;
		strInstrument.empty();		
	}
	~SRiskKey()
	{
		
	}
/**	SRiskKey(RiskIndicatorType	nID, std::string& str)
	{
		nRiskIndicatorID = nID;
		strInstrument    = str;		
	}*/
	SRiskKey(RiskIndicatorType	nID, std::string& str, PositionDetailKey& p)
	{
		nRiskIndicatorID = nID;
		strInstrument    = str;
		PostionDetail = p;			
	}
	bool operator< (const SRiskKey& r) const
	{
		int irlt;
		if(nRiskIndicatorID < r.nRiskIndicatorID)
			return true;
		else if(nRiskIndicatorID > r.nRiskIndicatorID)
			return false;

		irlt=strncmp(strInstrument.c_str(),r.strInstrument.c_str(),sizeof(strInstrument));
		if(irlt<0) return true;
		if(irlt>0) return false;

		if(PostionDetail < r.PostionDetail)
			return true;

		
		return false;
	}

};

struct SProduct
{
	InstrumentIDType ProductID;//Ʒ��ID
};
struct SLimitTrade
{
	int		nUserID;    //����ԱID
	bool	bLimitTrade;//�Ƿ����ƽ���
};
struct SManualVerify
{
	int		nUserID;    //����ԱID
	bool	bManualVerify;//�Ƿ��ֶ����
};
enum MsgStatusType
{
	MsgStatus_New,				//��������¼������
	MsgStatus_ChangeLevel,		//��������¼���ȼ����
	MsgStatus_Delete,			//��������¼����ʧ

	MsgStatus_Appoint,			//�ֶ�������ί�ɸ�����Ա
	MsgStatus_Finished,			//�ֶ������������¼������¼
	MsgStatus_ForceClose,		//�ֶ�������ִ���ֶ�ǿƽ��¼
};
enum RiskEventType
{
	RiskEvent_History,
	RiskEvent_Realtime,
};
struct RiskEventQueryKey
{
	TradeAccountIDType		nTradeAccountID;
	DateType szStartDate;	//��ʼʱ��
	DateType szEndDate;
};
//�����¼����ݽṹ
struct RiskEvent
{
	RiskEventIDType			nRiskEventID;
	RiskEventSubIDType		nRiskEventSubID;
	RiskTime				cTimeBegin;		//�÷���¼���Ч����ֹʱ�䣻�����¼���nRiskIndicatorID��cTimeBegin��cTimeEnd��ΪΨһ��ʶ
	RiskTime				cTimeEnd;		// 
	RSTIME					lEventTime;			
	BrokerIDType			BrokerID;			//�����˺ŷ��ʱ���������������
	TradeInvestorIDType		nTradeInvestorID;	
	RiskIndicatorType		nRiskIndicatorID;
	InstrumentIDType		InstrumentID;
	RiskLevelType			nRiskLevelID;
	double					dblIndicatorValue;			//����ֵ
	double					dblIndicatorCurrentValue;	//��ǰֵ
	MsgStatusType			nMsgSendStatus;
	RiskEventType			nIsValid;					//0 ����ʷ�����¼��� 1 ʵʱ�����¼�
	int						nResponseType;				//���ն�������Ҫ�ͻ�������������

	double					dblIndicatorValue2;		 // ��Щָ����Ҫ��������ֵ��(���籣֤������,����ֶδ���ʵ�ʱ�֤��;���ƿ�������"�����ܶ�"
	double					dblIndicatorCurrentValue2;//��ǰֵ

	bool operator<(const RiskEvent&	src) const
	{
		return lEventTime<src.lEventTime;
	}
};
//������ֵ����
struct RiskEventUpdate
{
	RiskEventIDType			nRiskEventID;
	RiskEventSubIDType		nRiskEventSubID;
	double					dblIndicatorCurrentValue;//��ǰֵ
};

enum EventHandlingActionType
{
	Action_ServerRecord,	//��������¼
	Action_ClientRecord,	//�ֶ���¼
	Action_ForceClose,		//�ֶ�ǿƽ
};
//�������ݴ�������ݽṹ
struct RiskEventHandling
{
	EventHandlingIDType		nRiskEventHandlingID;
	RSTIME					lHandlingTime;
	TradeInvestorIDType		nTradeInvestorID;
	RiskEventIDType			nRiskEventID;
	RiskEventSubIDType		nRiskEventSubID;
	USERNAMESTRING			szHandler;			//�ͻ��˼�¼��������
	USERNAMESTRING			szAccepter;			//�ͻ��˼�¼��������
	EventHandlingActionType	nAction;			//��������¼���ֶ���¼���ֶ�ǿƽ
	MsgStatusType			nProcessStatus;		//�����������У���ʧ
	CommentType				szContent;			//�ͻ��˼�¼����ע
	bool operator<(const RiskEventHandling&	src) const
	{
		return lHandlingTime<src.lHandlingTime;
	}
};

struct EventLevelID
{
	RiskEventIDType EventID;
	RiskLevelType	LevelID;
};
struct RiskEventLevelID
{
	RiskEventIDType EventID;
	RiskLevelType	LevelID;
	double			RiskVal;
};
//�¼���Ϣģ��ṹ
struct EventMessageTemplate
{
	RiskIndicatorType		nRiskIndicatorID;
	RiskLevelType			nRiskLevelID;
	MsgTitle				szTitle;
	MsgContent				szContent;
};
//����ָ�궨��ṹ
struct RiskIndicator
{
	RiskIndicatorType	nRiskIndicatorID;
	INDICATORNAMESTRING	szName;
	RiskItemDesc		szAlgorithmDesc;
	char				szTypeName[40];//��˾����أ���ͬ������������
};

//��������ṹ
struct RiskResponse
{
	RiskWarningType		nResponseType;//ĳһ�����嶯����ID���������ѣ���
	RESPONSENAMESTRING	szName;       //��������˵��
	RiskItemDesc		szDesc;
};
////////////////////////////////////////////////////////////////////////////
//����õ��Ľṹ
//��Ϣ�ṹ����

struct MessageInfo
{
	int nMessageID;
	char szTitle[41];
	MsgContent szContent;
	DateType szExpiredDate;
	char szCreateDate[20];	//��ʽ"yyyy-mm-dd HH:MM:SS"
	int nOwner;
	ACCOUNTSTRING szOwner;	//�������û���
};
struct MsgSendStatus
{
	int nMessageID;
	RiskMgmtUserIDType nRiskMgmtUserID;
	ACCOUNTSTRING szAccount;
	int nSendStatus;	// ��Ϣ����״̬ 0 δ���� 1 �ѷ���
};

struct TargetAccount
{
	RiskMgmtUserIDType nRiskMgmtUserID;
	ACCOUNTSTRING szAccount;
};
struct MessageQryCondition
{
	RiskMgmtUserIDType nRiskMgmtUserID;	//��Ϣ������ID
	DateType szCreateDate;				//����ʱ�䣬��ʽΪ YYYY-MM-DD
};


//����ֵǿƽ��������
struct ForceCloseParam
{
	int nTradeAccountID;		//����ԱID
	int nForceCloseInd;			//ǿƽָ��
	float fThreshold;			//������ֵ
	int nCloseStrategy;			//ƽ�ֲ��� 0��ȫ��ƽ��
	int nForceClosePriceOption;	//ǿƽ��λ 0���м�
};

struct SRiskControl
{
	RiskWarningType			nResponseType;//��������
	std::set<std::string>	setInstrument; //�ö������Ƶĺ�Լ
	PositionDetailKey   PostionDetail;

	int					nEventID;//�����¼�ID
	RiskIndicatorType	nRiskIndicatorID;		//�����ķ���ָ������
	RiskLevelType	    nRiskLevelID;			//�����ķ��յȼ�
	SRiskControl()
	{
		nResponseType = WarningType_None;
	}
};
//δ��˵���˽ṹ
struct  SVerisyOrder
{
	OrderKey					orderKey;							//δ��˵�
	int							nVerifyUser;						//���ԱID����Ӧ�þ��Ƿ��Ա��		
	int							nResult;							//��˽�� 1ͨ����0ûͨ��;-1δ���
};
//������˲�ѯ����
struct SQueryOrder
{
	SQueryOrder()
	{
		memset(this, 0 ,sizeof(*this));
	}
	DateType				szVerifyDate;		//��ʽ2013-08-06			����գ���ȫ��
	CommonAccountType       Account;			//����Ա�˻�				����գ���ȫ��
	bool					bIsProduct;			//true:����Ʒ�֣� false:�����Լ
	CommonInstrumentIDType  InstrumentID;       //Ʒ��ID ���� ��ԼID		����գ���ȫ��
	int						nResult;            //��˽�� 1ͨ����0ûͨ����-1�����;-2ȫ��
};
//������˲�ѯ���
struct SQueryOrderResult
{
	PlatformStru_OrderInfo order;				//�����ṹ
	int					   nVerifyUser;	        //���ԱID
	CommonAccountType	   VerifyUserName;		//���Ա���֣���Ӧ�þ��Ƿ��Ա��
	char			       szVerifyDate[25];    //������� 
	int					   nResult;             //��˽��
};
//ǿƽ��¼��ѯ����
struct ForceCloseCondition
{
	ForceCloseCondition()
	{
		memset(this, 0 ,sizeof(*this));
	}
	DateType				szInsertDate;		//��ʽ2013-08-06			����գ���ȫ��
	CommonAccountType       Account;			//����Ա�˻�				����գ���ȫ��
	bool					bIsProduct;			//true:����Ʒ�֣� false:�����Լ
	CommonInstrumentIDType  InstrumentID;       //Ʒ��ID ���� ��ԼID		����գ���ȫ��
	int						nForceCloseType;    //1Ϊ�Զ�ǿƽ��0Ϊ�ֶ�  -2Ϊȫ��
	CommonAccountType		TradeInvestorName; //���Ա�˻�				����գ���ȫ��

};
//ǿƽ��¼��ѯ�������ؽṹ
struct ForceCloseResult
{
	ForceCloseResult()
	{
		memset(this, 0 ,sizeof(*this));
	}
	PlatformStru_OrderInfo  order;
//	PlatformStru_InputOrder order;            //ǿƽ�µ��ṹ
//	int						nFrontID;		  //
//	int						nSessionID;
//	char					szOrderRef[13];
	bool					bForceCloseType;  //trueΪ�Զ�ǿƽ��falseΪ�ֶ�
	CommonAccountType		RiskerName;	  //���Ա����
	char					szInsertDate[22];		//��ʽ2013-08-06 11:08:08	


};
/*ʾ������
int nCount = 2;
SAdminQuery *sAdminQuery = (SAdminQuery*)malloc(sizeof(SAdminQuery)+ sizeof(CommonAccountType)*nCount) ;
strcpy(sAdminQuery->szDateBegin, "2011-01-01");
strcpy(sAdminQuery->szDateEnd, "2013-08-13");
sAdminQuery->nCount = nCount;
strcpy(sAdminQuery->strAccount[0], "0001");
strcpy(sAdminQuery->strAccount[1], "0002");	

if( !CTCPDataService::GetInstance()->SendData(Cmd_RM_QryHistoryFundInfo_Req, (void*)sAdminQuery, sizeof(SAdminQuery)+ sizeof(CommonAccountType)*2, 0, false))
{
std::string strError = CTCPDataService::GetInstance()->GetLastError();
MessageBox(strError.c_str(), "��ʾ", MB_ICONERROR);
return;
}
*/
//�����ն˲�ѯ����ʷ�ʽ���ʷ�ֲ֣���ʷ��������ʷ�����
struct SAdminQuery
{	
	DateType				szDateBegin;	  //��ʽ2013-08-06	
	DateType				szDateEnd;			
	int						nCount;				 //����Ա�˻���Ŀ
	CommonAccountType		strAccount[1];       //����Ա�˺�ID���Էֺŷָ�	
};
//�ֶ�ǿƽ�ṹ
struct SManualForceClose
{
	PlatformStru_InputOrder  nInputOrder;//InvestorID �ֶ���д�Ľ���Ա�ַ�������
    int nFrontID;//���Ա��Frontid�����Ա��ID����session�� orderref(�Զ���������ؿͻ���ά��)
    int nSessionID;
    char szOrderRef[13];
	char szReason[256];//ǿƽ��ԭ���ĸ����Ա���ֶ�ǿƽָ�
	char szRiskerName[256];//���Ա������
};
//��ؿͻ��˳�������ṹ
struct SRiskInputOrderAction
{
	PlatformStru_InputOrderAction  nInputOrderAction; 
	int nRequestID;//��ʱû���õ�	
	char nsActionReson[256];//����ԭ��
	char szRiskerName[256];//���Ա������
};
//�µ����ƽṹ������ͨѶ������ִ��
struct SRiskControl2Execute
{	
	CommonAccountType		Investor;	  //����ԱID
	RiskWarningType			nResponseType;//��������
	CommonInstrumentIDType	Instrument; //�ö������Ƶĺ�Լ
	PositionDetailKey		PostionDetail;

	int						nEventID;//�����¼�ID
	RiskIndicatorType		nRiskIndicatorID;		//�����ķ���ָ������
	RiskLevelType			nRiskLevelID;			//�����ķ��յȼ�
};
//////////////////////////////////////////////////////////////////////////////
//�˻����׺�Լ����
struct  STradeForbidRequest
{	
	RiskIndicatorType		rType;
	CommonAccountType       Account;	
	char					szRiskerName[256];//���Ա������
	char					szInsertDate[22];		//��ʽ2013-08-06 11:08:08	

	RiskProductID			ProductID;			//Ʒ������	
	char					cInstruments[300];  //���׺�Լ���ƣ������Ʒ�����ƿ������ڸ�ֵ��Ʒ�֡�
	double					dbMarginDividDynamic;//��֤��/��̬Ȩ��
	double					dbMarginUse;		 //ʹ�ñ�֤��
	double					dbLossAmount;		//�����ܶ�
	double					dbLossPercent;      //�������

	STradeForbidRequest()
	{
		memset(this,0,sizeof(*this));
	}
};
//��ѯ�˻����������
struct SQueryTradeRistrict
{	
	DateType				szDateBegin;	  //��ʽ2013-08-06	
	DateType				szDateEnd;			
	CommonAccountType		strAccount;       //����Ա�˺�ID
};
#pragma pack(pop)