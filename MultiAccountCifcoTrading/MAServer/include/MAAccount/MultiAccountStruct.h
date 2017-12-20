#pragma once

#pragma pack(push)
#pragma pack(1)

typedef char ACCOUNTSTRING[21];
typedef char BROKERCODESTRING[21];
typedef char TRADEACCOUNTSTRING[32];
typedef char PASSWORDSTRING[33];
typedef char PERMISSIONNAMESTRING[21];
typedef char IDCARDNAMESTRING[21];

struct AccountInfo
{
	int nAccountID;
	ACCOUNTSTRING szAccount;
	PASSWORDSTRING szPassword;
	int nAccountType;
	int nOwnerAccountID;
	char szName[21];
	char szContactInfo[101];
	int nIDCardType;
	char szIDCardNo[21];
	int nAccountStatus;
	int nLogonStatus;
	int nViewPermission1;
	int nViewPermission2;
	int nPermissionID;
	char szDate[11];
};

struct AccountInfoEx
{
	AccountInfo accountInfo;
	//�����ֶ�
	ACCOUNTSTRING szOwnerAccount;
	PERMISSIONNAMESTRING szPermissionName;
	IDCARDNAMESTRING szIDCardName;
};

struct LoginReq
{
	ACCOUNTSTRING szAccount;
	PASSWORDSTRING szPassword;
	int nAccountType;
};

enum AccountType
{
	ACCOUNT_TYPE_MANAGE,
	ACCOUNT_TYPE_LOGON,
};

struct LoginRsp
{
	int nAccountID;
	int nPermissionID;
};

struct ModifyPwd
{
	int nAccountID;
	PASSWORDSTRING szPassword;
};

struct BrokerInfo
{
	int nBrokerID;
	BROKERCODESTRING szCode;
	char szCompany[65];
	int nShareWithLowerLever;
	int nOwnerAccountID;
};

struct ServerGroup
{
	int nServerNo;
	char szServerName[21];
	int nBrokerID;
	int nShare;
};

struct ServerAddr
{
	int nAddrID;
	int nServerNo;
	char szServerAddress[129];
	int nPort;
	int nServerType;
};

struct IDCardInfo
{
	int nIDCardType;
	IDCARDNAMESTRING szIDCardName;
};

struct TradeAccount
{
	TRADEACCOUNTSTRING szTradeAccount;
	PASSWORDSTRING szPassword;
	int nOwnerAccountID;
	int nBrokerID;
	int nServerNo;
	char szNickName[21];
	int nID;
	int nRowNo;
};
struct CTPTradeAccount
{
	TradeAccount t;
	PASSWORDSTRING DynamicPassword;
	bool		UseQuot;
	bool		StrategyValid;
	char        StatusText[64]; 
};
struct TradeAccountKey
{
	TRADEACCOUNTSTRING szTradeAccount;
	int nOwnerAccountID;
};

struct PermissionModule
{
	int nModuleID;
	char szModuleName[21];
	int nCommonOperators;
	int nAdditionalOperators;
	int nModuleType;
};

struct Permission
{
	int nPermissionID;
	int nOwnerAccountID;
	PERMISSIONNAMESTRING szName;
	int nPermissionType;
	int nPermissionDataSize;
	char permissionData[120];
};

struct PermissionSimple
{
	int nPermissionID;
	int nOwnerAccountID;
	PERMISSIONNAMESTRING szName;
	int nPermissionType;
};

struct PermissionData
{
	int nModuleID;
	int nCommonOperators;
	int nAdditionalOperators;
};

struct MessageInfo
{
	int nMessageID;
	char szTitle[41];
	char szContent[2001];
	char szDate[11];
	int nOwner;
	int nType;
};

struct TargetAccount
{
	int nAccountID;
	ACCOUNTSTRING szAccount;
};

//��Ϣ���Ͷ���
enum MessageType
{
	MESSAGE_COMMON,
	MESSAGE_PERSONAL,
};

//Ȩ�޶���
enum PermissionType
{
	PERMISSION_BKG,
	PERMISSION_FORE,
};


//Ȩ��ģ�鶨��
enum PermissionModuleType
{
	MODULE_FORE_ACCOUNT_MANAGE = 1,	//ǰ̨�˺Ź���
	MODULE_BKG_ACCOUNT_MANAGE,		//�����˺Ź���
	MODULE_BROKER_MANAGE,			//���͹�˾����
	MODULE_SERVER_MANAGE,			//����������
	MODULE_PERMISSION_MANAGE,		//Ȩ������
	MODULE_FORE_CLIENT_MANAGE,		//ǰ̨�ͻ���
};

//��̨ͨ��Ȩ��
#define PERMISSION_BKG_COMMON_ADD	0x1		//����
#define PERMISSION_BKG_COMMON_MODIFY	0x2	//�޸�
#define PERMISSION_BKG_COMMON_DEL	0x4		//ɾ��
#define PERMISSION_BKG_COMMON_QUERY	0x8		//�鿴
#define PERMISSION_BKG_COMMON_CHANGE_STATUS	0x10	//״̬�ı�
#define PERMISSION_BKG_COMMON_ADD_MODIFY	0x20	//����/�޸�

//ǰ̨Ȩ��
#define PERMISSION_FORE_LOGON	0x1	//��¼
#define PERMISSION_FORE_ADD_TRADE_ACCOUNT	0x2	//��ӽ����˺�

#define PERMISSION_STRING_ADD		"����"
#define PERMISSION_STRING_MODIFY	"�޸�"	
#define PERMISSION_STRING_DEL		"ɾ��"
#define PERMISSION_STRING_QUERY		"�鿴"
#define PERMISSION_STRING_CHANGE_STATUS	"״̬�ı�"
#define PERMISSION_STRING_ADD_MODIFY		"����/�޸�"
#define PERMISSION_STRING_LOGON		"��¼"
#define PERMISSION_STRING_ADD_TRADE_ACCOUNT	"��ӽ����˺�"

#pragma pack(pop)