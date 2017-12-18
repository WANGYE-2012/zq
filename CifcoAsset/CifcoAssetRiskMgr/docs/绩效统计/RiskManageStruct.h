#pragma once

#pragma pack(push)
#pragma pack(1)
#include <string>
typedef unsigned long RSDWORD;
typedef unsigned long RSCOLORREF;
typedef __int64       RSTIME;
typedef char ACCOUNTSTRING[21];
typedef char TRADEACCOUNTSTRING[32];
typedef char PASSWORDSTRING[33];
typedef char PERMISSIONNAMESTRING[21];
typedef char USERNAMESTRING[21];
typedef char CONTACTINFOSTRING[101];
typedef char IDCARDNOSTRING[21];
typedef char OTHERNAMESTRING[21];
typedef char INDICATORNAMESTRING[41];

typedef char DESCSTRING[101];

typedef  int  AssetMgmtOrgIDType;
typedef  int  RiskMgmtUserIDType;
typedef  int  FinancialProductIDType;
typedef  int  PermissionIDType;
typedef  int  StatusType;
typedef  int  AccountIDType;
typedef  int  TraderIDType;
typedef  int  RiskIndModuleType;
typedef  int  RiskLevelType;
typedef  int  PercentType;
typedef  char RiskItemDesc[101];
typedef	 char MsgTitle[41];
typedef  char MsgContent[1001];
typedef int	  RiskEventIDType;
typedef int	  RiskEventSubIDType;
typedef int   TradeAccountIDType;
typedef int   EventHandlingIDType;
typedef char InstrumentIDType[31];
typedef char CommentType[201];
typedef char DateType[11];

typedef double TFtdcMoneyType;
//////////////////////////////////////////////////////////////////////////
enum StatisticsIDType
{
	ST_PosProfit=1,//1.	�ֲ�ӯ������
	ST_InitFund,//2.	�˻���ʼȨ�����
	ST_Balance,//3.	�˻���ǰ��Ȩ�����
	ST_UseMargin,//4.	��Լ�ֱֲ�֤�����
	ST_Position,//5.	��Լ�ĳֲ�������
};
enum RiskIndicatorType
{
	RI_MaxPosition = 1,//�г��������
	RI_FundLoss,//�ֲ��޿�����
	RI_GappedMarket,//��ҹ���գ����ճ��� 
	RI_MarginRatio, //��֤���������
	RI_ProductMaxPositon, //Ʒ�����ֲַ���
	RI_FundNetValue,//����ֵ����

	RI_Count,//�ܹ�������Ŀ
};
//RiskIndicatorType ֵ��Ӧ���ַ����ƺ�
/*������ݴӻ������������ȡ

#define STR_RI_MaxPosition   "����Լ���ֲ�"
#define STR_RI_FundLoss		 "�ֲ��޿�����"
#define STR_RI_GappedMarket	 "��ҹ���գ����ճ���"
#define STR_RI_MarginRatio	 "��֤���������"
#define STR_RI_ProductMaxPositon	 "Ʒ�����ֲַ���"
*/
enum MsgStatusType
{
	MsgStatus_New,      //��������¼������
	MsgStatus_ChangeLevel,	//��������¼���ȼ����
	MsgStatus_Delete,	//��������¼����ʧ

	MsgStatus_Appoint,//�ֶ�������ί�ɸ�����Ա
	MsgStatus_Finished,//�ֶ������������¼������¼
	MsgStatus_ForceClose,//�ֶ�������ִ���ֶ�ǿƽ��¼
};
#define STR_MsgStatus_New			"�·���"
#define STR_MsgStatus_ChangeLevel	"�ȼ����"
#define STR_MsgStatus_Delete		"��ʧ"
#define STR_MsgStatus_Appoint		"�ֶ�������ί�ɸ�����Ա"
#define STR_MsgStatus_Finished		"�ֶ������������¼������¼"
#define STR_MsgStatus_ForceClose	"�ֶ�������ִ���ֶ�ǿƽ��¼"

enum RiskEventType
{
	RiskEvent_History,
	RiskEvent_Realtime,
};
enum EventHandlingActionType
{
	Action_ServerRecord,	//��������¼
	Action_ClientRecord,	//�ֶ���¼
	Action_ForceClose,		//�ֶ�ǿƽ
};
#define STR_Action_ServerRecord    "��������¼"
#define STR_Action_ClientRecord    "�ֶ���¼"
#define STR_Action_ForceClose	   "�ֶ�ǿƽ"
enum RoleType
{
	ROLE_TYPE_MANAGE,		//����Ա
	ROLE_TYPE_RISK_CONTROL,	//���Ա
};

enum IDCardType
{
	IDENTIFY_CARD,		//���֤
	MILITARY_ID,		//����֤
	OTHER_CARD			//����
};

#define STR_T_FD_In    "���"
#define STR_T_FD_Out    "����"

#define STR_T_AVAF_Invalid    "δȷ��"
#define STR_T_AVAF_Valid      "��Ч"
#define STR_T_AVAF_Repeal     "����"

#define STR_T_FIOT_FundIO      "�����"
#define STR_T_FIOT_Transfer     "����ת��"
#define  NULL_QUERY_RESULE    "��ѯ���Ϊ�գ����������ò�ѯ����"
#define  NULL_QUERY_CONDITION    "��ѯ����Ϊ�գ������ò�ѯ����"

//����Ա/���Ա�˻��ṹ
struct RiskMgmtUserInfo
{
	RiskMgmtUserIDType		nRiskMgmtUserID;
	ACCOUNTSTRING			szAccount;
	PASSWORDSTRING			szPassword;
	RoleType				nRoleType;				//�˻�����
	AssetMgmtOrgIDType		nAssetMgmtOrgID;
	FinancialProductIDType	nFinancialProductID;
	StatusType				nStatus;				//����״̬
	USERNAMESTRING			szName;
	CONTACTINFOSTRING		szContactInfo;
	PermissionIDType		nPermissionID;
};

//����Ա/���Ա�˻��ṹ��չ
struct RiskMgmtUserInfoEx
{
	RiskMgmtUserInfo		userInfo;
	OTHERNAMESTRING			szAssetMgmtOrgName;	//�ʹ�����
	OTHERNAMESTRING			szFinancialProductName;	//��Ʋ�Ʒ��
	PERMISSIONNAMESTRING	szPermissionName;	//Ȩ����
};

struct LoginReq
{
	ACCOUNTSTRING	szAccount;
	PASSWORDSTRING	szPassword;
	RoleType		nRoleType;
};



struct LoginRsp
{
	AccountIDType	   nAccountID;			//�˺�ID
	PermissionIDType   nPermissionID;		//Ȩ��
	AssetMgmtOrgIDType nAssetMgmtOrgID;	//�ʹ�����ID
	FinancialProductIDType	nFinancialProductID;	//��Ʋ�ƷID
};

struct ModifyPwd
{
	RiskMgmtUserIDType	nRiskMgmtUserID;
	PASSWORDSTRING		szPassword;
};

//ί�н����˻��ṹ
struct TrustTradeAccount
{
	AccountIDType nTradeAccountID;
	ACCOUNTSTRING szAccount;
	AssetMgmtOrgIDType nAssetMgmtOrgID;
	FinancialProductIDType nFinancialProductID;
	RiskIndModuleType	nRiskIndModuleID;
	TraderIDType		nTraderID;
	double				dProfit;
	char		szName[201];
	CONTACTINFOSTRING	szContactInfo;
	IDCardType			nIDCardType;
	IDCARDNOSTRING		szIDCardNo;
	PASSWORDSTRING	szPwd;
	bool operator<(const TrustTradeAccount item) const
	{ //STL �����õ�����
		if(nTradeAccountID < item.nTradeAccountID)
			return true;
		else  if(nTradeAccountID > item.nTradeAccountID)
			return false;

		if(nAssetMgmtOrgID < item.nAssetMgmtOrgID)
			return true;
		else if(nAssetMgmtOrgID > item.nAssetMgmtOrgID)
			return false;

		if(nRiskIndModuleID < item.nRiskIndModuleID)
			return true;
		else if(nRiskIndModuleID > item.nRiskIndModuleID)
			return false;

		if(nTraderID < item.nTraderID)
			return true;
		else if(nTraderID > item.nTraderID)
			return false;

		if(nIDCardType < item.nIDCardType)
			return true;
		else if(nIDCardType > item.nIDCardType)
			return false;


		int i = strcmp(szAccount, item.szAccount) < 0;			
		if(i>0)
			return true;
		else if(i<0)
			return false;


		i = strcmp(szName, item.szName) < 0;			
		if(i>0)
			return true;
		else if(i<0)
			return false;


		i = strcmp(szContactInfo, item.szContactInfo) < 0;			
		if(i>0)
			return true;
		else if(i<0)
			return false;


		i = strcmp(szIDCardNo, item.szIDCardNo) < 0;			
		if(i>0)
			return true;
		else if(i<0)
			return false;

		i = strcmp(szPwd, item.szPwd) < 0;			
		if(i>0)
			return true;
		else if(i<0)
			return false;


		return false;
	}

};

//ί�н����˻���չ
struct TrustTradeAccountEx
{
	TrustTradeAccount trustAccount;
	OTHERNAMESTRING szAssetMgmtOrgName;	//�ʹ�����
	OTHERNAMESTRING szFinancialProductName;	//��Ʋ�Ʒ��
	USERNAMESTRING szTraderName;	//����Ա��
	OTHERNAMESTRING szRiskIncModuleName;	//����ģ����
};

//����Ա��Ϣ�ṹ
struct TraderInfo
{
	TraderIDType	nTraderID;
	USERNAMESTRING	szName;
	AssetMgmtOrgIDType nAssetMgmtOrgID;
	FinancialProductIDType nFinancialProductID;
	char szTel[21];
	char szMobile[21];
	char szOtherContactInfo[21];
};

//����Ա��չ�ṹ
struct TraderInfoEx
{
	TraderInfo traderInfo;
	OTHERNAMESTRING szAssetMgmtOrgName;	//�ʹ�����
	OTHERNAMESTRING szFinancialProductName;	//��Ʋ�Ʒ��
};

//�ʹ���֯�ṹ
struct AssetMgmtOrganization
{
	AssetMgmtOrgIDType nAssetMgmtOrgID;
	OTHERNAMESTRING		szName;
	AssetMgmtOrgIDType nUpperLevelOrgID;
	int nScaleLevel;
	double dLongitude;
	double dLatitude;
};

//��Ʋ�Ʒ�ṹ
struct FinancialProduct
{
	FinancialProductIDType nFinancialProductID;
	OTHERNAMESTRING szName;
	DESCSTRING szScopeDesc;
};

//��Ʋ�Ʒ���ʹ���֯��ϵ
struct AssetOrgProRelation
{
	AssetMgmtOrgIDType nAssetMgmtOrgID;
	FinancialProductIDType nFinancialProductID;

	bool operator<(const AssetOrgProRelation&	src) const
	{
		if(nAssetMgmtOrgID<src.nAssetMgmtOrgID)
			return true;
		else if(nAssetMgmtOrgID==src.nAssetMgmtOrgID)
		{
			if(nFinancialProductID<src.nFinancialProductID)
				return true;
		}
		return false;
	}

};
//
struct EventLevelID
{
	RiskEventIDType EventID;
	RiskLevelType	LevelID;
};
//
struct	StatisticsEntry
{
	StatisticsIDType		nStatisticsType;
	AssetMgmtOrgIDType		nAssetMgmtOrgID;
	FinancialProductIDType	nFinancialProductID;	
	InstrumentIDType		InstrumentID;
	double					dblValue;
};
//����ָ��ģ�ͽṹ
struct RiskIndicatorModule
{
	RiskIndModuleType		nRiskIndModuleID;
	OTHERNAMESTRING			szName;
};

enum RiskWarningType
{
	WarningType_Beep,		//����
	WarningType_Vibrate,	//��
	WarningType_Both,		//���Ѽ���
};

//����Ԥ����¼�ṹ
struct RiskResponse
{
	RiskLevelType	  nRiskLevelID;
	float			  fThresholdValue;
	RiskWarningType	  nResponseType;
	RSCOLORREF		  nColor;

	bool operator!=(const RiskResponse&	src) const
	{
		return (memcmp(this,&src,sizeof(*this))!=0);
	}
};
struct RiskWarning
{
	RiskIndModuleType nRiskIndModuleID;
	RiskIndicatorType nRiskIndicatorID;
	RiskLevelType	  nRiskLevelID;
	float			  fThresholdValue;
	RiskWarningType	  nResponseType;
	RSCOLORREF		  nColor;
};
//���ռ������ýṹ
struct IndLevelWarning
{
	RiskIndicatorType nRiskIndicatorID;
	RiskLevelType	  nRiskLevelID;
	float			  fThresholdValue;
	RiskWarningType	  nResponseType;
	RSCOLORREF		  nColor;
};

//Ʒ�ַ��ռ�����ƽṹ
struct ProductRiskWarning
{
	TradeAccountIDType nTradeAccountID;
	InstrumentIDType  szProductID;	//Ʒ������
	RiskIndicatorType nRiskIndicatorID;
	RiskLevelType	  nRiskLevelID;
	float			  fThresholdValue;
	RiskWarningType	  nResponseType;
	RSCOLORREF		  nColor;
};

struct ProducrRiskWarningKey
{
	InstrumentIDType  szProductID;	//Ʒ������
	TraderIDType nTraderID;
};

//����ָ�궨��ṹ
struct RiskIndicator
{
	RiskIndicatorType	nRiskIndicatorID;
	INDICATORNAMESTRING	szName;
	RiskItemDesc		szAlgorithmDesc;
};

//�����ռ���
struct MaxRiskLevel
{
	RiskIndicatorType nRiskIndicatorID;
	RiskLevelType nMaxLevel;
};

//�¼���Ϣģ��ṹ
struct EventMessageTemplate
{
	RiskIndicatorType		nRiskIndicatorID;
	RiskLevelType			nRiskLevelID;
	MsgTitle				szTitle;
	MsgContent				szContent;
};

//�����¼����ݽṹ
struct RiskEvent
{
	RiskEventIDType			nRiskEventID;
	RiskEventSubIDType		nRiskEventSubID;
	RSTIME					lEventTime;			
	TradeAccountIDType		nTradeAccountID;
	RiskIndModuleType		nRiskIndModuleID;
	RiskIndicatorType		nRiskIndicatorID;
	InstrumentIDType		InstrumentID;
	RiskLevelType			nRiskLevelID;
	double					dblIndicatorValue;
	MsgStatusType			nMsgSendStatus;
	RiskEventType			nIsValid;		//0 ����ʷ�����¼��� 1 ʵʱ�����¼�
	bool operator<(const RiskEvent&	src) const
	{
		return lEventTime<src.lEventTime;
	}
};

struct RiskEventQueryKey
{
	TradeAccountIDType		nTradeAccountID;
	DateType szStartDate;	//��ʼʱ��
	DateType szEndDate;
};
//�������ݴ����û�����ṹ
struct RiskEventHandlingUserReq
{
	TradeAccountIDType		nTradeAccountID;
	RiskEventIDType			nRiskEventID;
	RiskEventSubIDType		nRiskEventSubID;
	USERNAMESTRING			szHandler;//������
	USERNAMESTRING			szAccepter;//������
	MsgStatusType			nProcessStatus;//�ֶ��������������
	CommentType				szContent;//��ע
};
//�������ݴ�������ݽṹ
struct RiskEventHandling
{
	EventHandlingIDType		nRiskEventHandlingID;
	RSTIME					lHandlingTime;
	TradeAccountIDType		nTradeAccountID;
	RiskEventIDType			nRiskEventID;
	RiskEventSubIDType		nRiskEventSubID;
	USERNAMESTRING			szHandler;//�ͻ��˼�¼��������
	USERNAMESTRING			szAccepter;//�ͻ��˼�¼��������
	EventHandlingActionType	nAction;//��������¼���ֶ���¼���ֶ�ǿƽ
	MsgStatusType			nProcessStatus;//�����������У���ʧ
	CommentType				szContent;//�ͻ��˼�¼����ע
	bool operator<(const RiskEventHandling&	src) const
	{
		return lHandlingTime<src.lHandlingTime;
	}
};

//�������ݴ�������ݽṹ��չ
struct RiskEventHandlingEx
{
	EventHandlingIDType		nRiskEventHandlingID;
	RSTIME					lHandlingTime;
	TradeAccountIDType		nTradeAccountID;
	RiskEventIDType			nRiskEventID;
	RiskEventSubIDType		nRiskEventSubID;
	USERNAMESTRING			szHandler;//�ͻ��˼�¼��������
	USERNAMESTRING			szAccepter;//�ͻ��˼�¼��������
	EventHandlingActionType	nAction;//��������¼���ֶ���¼���ֶ�ǿƽ
	MsgStatusType			nProcessStatus;//�����������У���ʧ
	CommentType				szContent;//�ͻ��˼�¼����ע
	RiskIndicatorType		nRiskIndicatorID;
	RiskLevelType			nRiskLevelID;
	double					dblIndicatorValue;
	InstrumentIDType		InstrumentID;
};

struct RiskEventHandlingQueryKey
{
	RiskEventIDType			nRiskEventID;
};

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

struct MessageQryCondition
{
	RiskMgmtUserIDType nRiskMgmtUserID;	//��Ϣ������ID
	DateType szCreateDate;				//����ʱ�䣬��ʽΪ YYYY-MM-DD
};

struct TargetAccount
{
	RiskMgmtUserIDType nRiskMgmtUserID;
	ACCOUNTSTRING szAccount;
};

struct MsgSendStatus
{
	int nMessageID;
	RiskMgmtUserIDType nRiskMgmtUserID;
	ACCOUNTSTRING szAccount;
	int nSendStatus;	// ��Ϣ����״̬ 0 δ���� 1 �ѷ���
};

struct PermissionName
{
	PermissionIDType nPermissionID;
	OTHERNAMESTRING szName;
};

struct PermissionNameEx
{
	PermissionIDType nPermissionID;
	OTHERNAMESTRING szName;
	int nPermissionType;
};

struct PermissionValue
{
	PermissionIDType nPermissionID;
	int nActionID;//��Ӧ���� PermissionActionType
	int nValue;
};

struct PermissionNode
{
	int nNodeID;
	OTHERNAMESTRING szName;
	int nNodeType;
};

struct PermissionAction
{
	int nActionID;
	int nNodeID;
	OTHERNAMESTRING szName;
};

struct HisTradeDataQueryKey
{
	char AccountID[13];
	DateType TradingDayFrom;	//��ʼʱ�� ��ʽΪ"YYYY-MM-DD"
	DateType TradingDayTo;		//����ʱ�� ��ʽΪ"YYYY-MM-DD"
};

//Ȩ�޶���
enum PermissionType
{
	PERMISSION_MANAGE,			//����Ա
	PERMISSION_RISK_CONTROL,	//���Ա
};

enum PermissionActionType
{
	//�����ն�Ȩ��
	ACCOUNT_ADD = 1,		//�����˻�
	ACCOUNT_MODIFY,			//�޸��˻�
	ACCOUNT_DEL,			//ɾ���˻�
	ACCOUNT_STOP_ACTIVE,	//����/ͣ���˻�
	ACCOUNT_BATCH_IMPORT,	//���������˻�
	ORGANIZATION_MANAGE,	//��֯�ܹ�����
	FIN_PRODUCT_MANAGE,		//��Ʒ����
	PERMISSION_SET,			//Ȩ������
	RISK_MANAGE,			//Ԥ��ģ������
	RISK_EVENT_MSG_MANAGE,	//������Ϣ����

	//����ն�Ȩ��
	FORMS_STATISTICS,		//����ͳ��
	MESSAGE_SEND,			//������Ϣ
	TODAY_OPEN_ORDER_QUERY,	//���ձ�����ѯ
	TODAY_TRADE_QUERY,		//���ճɽ���ѯ
	TODAY_POSITION_QUERY,	//���ճֲֲ�ѯ
	TODAY_ACCOUNT_QUERY,	//�����ʽ��ѯ
	TODAY_DEPOSIT_QUERY,	//���ճ�����ѯ
	INVESTOR_INFO_QUERY,	//Ͷ������Ϣ��ѯ
	UNREAD_MSG_QUERY,		//��Ϣ֪ͨ��ѯ
	RISK_EVENT_QUERY,		//�û��¼���ѯ
	FORCE_CLOSE_ORDER,		//ǿƽ����
};

//����ֵ��������
struct NetFundParam
{
	int nTradeAccountID;	//ί�н����˺�ID
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
//����ֵǿƽ��������
struct ForceCloseParam
{
	int nTradeAccountID;		//ί�н����˻�ID
	int nForceCloseInd;			//ǿƽָ��
	float fThreshold;			//������ֵ
	int nCloseStrategy;			//ƽ�ֲ��� 0��ȫ��ƽ��
	int nForceClosePriceOption;	//ǿƽ��λ 0���м�
};



//��������
/*
��֯������
szName:		 ��֯������
PreBalance:	 ����֯�����������˻���ʼȨ��� (SeqPreRiskAccountField::PreBalance)
Balance:	 ����֯�����������˻���ʼȨ��� (RiskSyncAccountField::Balance)	
OffBalance:  Balance - PreBalance
YearEarnRate:


MarginRate:  ����֯�����������˻���֤��ͺͶ�̬Ȩ��͵ı���(RiskSyncAccountField::CurrMargin/ RiskSyncAccountField::Balance)
*/
struct SGather
{
	int						nID;	         //��֯����ID,��Ʋ�ƷID,�˻�ID
	int						nIDType;		 //0 �����Ų�ѯ,1 ����Ʋ�Ʒ��ѯ,2 ��ĳһ�˻���ѯ
	TFtdcMoneyType			SumPreBalance;		 //��ʼȨ���
	TFtdcMoneyType			SumBalance;		 //��ǰȨ���
	TFtdcMoneyType			YearEarnRate;	 //�껯������
	TFtdcMoneyType		    MarginRate;	     //��֤�����
};
struct SGatherData
{
	int						nIDType;		 //0 �����Ų�ѯ,1 ����Ʋ�Ʒ��ѯ,2 ��ĳһ�˻���ѯ
	int						nID;	         //��֯����ID,��Ʋ�ƷID,�˻�ID	
	DateType                szUpdateDate;    //��ѯ�������-1����Ϊ���һ������֮ǰ��Ч�����մ洢������
	double					SumPreBalance;    //��ʼȨ���
	double					SumBalance;		 //��ǰȨ���
	double					OffBalance;		 //����Ȩ��仯]
	double					YearEarnRate;	 //�껯������
	double				    MarginRate;	     //��֤�����

	double					dbValue;		 //ָ��ֵ
	int						nDayCount;		 //�ܹ�������
};

#pragma pack(pop)