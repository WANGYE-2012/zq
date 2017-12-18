#pragma once

// �������ӿ�ʹ�õ����ݽṹ

typedef char IAppIdType[64];
typedef char ILicenseType[64];
typedef char IHostNameType[64];
typedef char IUserNameType[32];
typedef char IPasswordType[32];
typedef char ITAType[32];
typedef DWORD ISessionIDType;
typedef char INamedPipeType[128];
typedef char IMessageType[256];

#define ISPLITCHAR			'\001'
#define ISPLITSTRING		"\001"
#define IPROTOCOLTITLE		"FastTraderV0.1"

#define IBUFSIZE			8192
#define IBUFMAXSIZE			16*1024*1024
#define IMAXSESSIOMNUM		20
#define IMAXREMAINTIME		180

#define IMASTERPIPENAME		"\\\\.\\pipe\\FastTrader\\v0_1\\Master"
#define IAPPPIPENAMEBODY	"\\pipe\\FastTrader\\v0_1\\App\\"

#define IYOUWELCOME			"You welcome to FastTrader, Protocol Version 0.1"

#define ISynIDNum			0xac34cd12
#define ITailNum			0x12342321

#define IMSGTYPE_A	1
#define IMSGTYPE_B	2
#define IMSGTYPE_C	3
#define IMSGTYPE_D	4
#define IMSGTYPE_E	5
#define IMSGTYPE_F	6
#define IMSGTYPE_G	7
#define IMSGTYPE_H	8

// MsgType A����������
#define ICMD_A_Login			1	//"Login"
#define ICMD_A_LoginReturn		2	//"LoginReturn"
#define ICMD_A_Logout			3	//"Logout"
#define ICMD_A_Hello			4	//"Hello"
#define ICMD_A_HeartBeat		6	//"HeartBeat"

// MsgType B����������
#define ICMD_B_IamFine			5	//"IamFine"
#define ICMD_B_rspHeartBeat		7	//"rspHeartBeat"

// MsgType C����������
#define ICMD_C_QueryAccountInfo				100	//"QueryAccountInfo"
#define ICMD_C_QueryContractIDInfo			102	//"QueryContractIDInfo"
#define ICMD_C_QueryOrderInfo				104	//"QueryOrderInfo"
#define ICMD_C_QueryPositionInfo			106	//"QueryPositionInfo"
#define ICMD_C_QueryExchangeDateTime		108	//"QueryExchangeDateTime"
#define ICMD_C_QueryDepthMarket				110	//"QueryDepthMarket"

// MsgType D����������
#define ICMD_D_ReturnAccountInfo			101	//"ReturnAccountInfo"
#define ICMD_D_ReturnContractIDInfo			103	//"ReturnContractIDInfo"
#define ICMD_D_ReturnOrderInfo				105	//"ReturnOrderInfo"
#define ICMD_D_ReturnPositionInfo			107	//"ReturnPositionInfo"
#define ICMD_D_ReturnExchangeDateTime		109	//"ReturnExchangeDateTime"
#define ICMD_D_ReturnDepthMarket			111	//"ReturnDepthMarket"

// MsgType E����������
#define ICMD_E_SubscribeDepthMarket			200	//"SubscribeDepthMarket"

// MsgType F����������
#define ICMD_F_ReturnDepthMarket			201	//"ReturnDepthMarket"
#define ICMD_F_rspDepthMarket				202	//"rspDepthMarket"

// MsgType G����������
#define ICMD_G_NewOrder						300	//"NewOrder"
#define ICMD_G_CancelOrder					303	//"CancelOrder"

// MsgType H����������
#define ICMD_H_rspNewOrderApiFail			301	//"rspNewOrderApiFail"
#define ICMD_H_rspNewOrderError				302	//"rspNewOrderError"
#define ICMD_H_rspCancelOrderApiFail		304	//"rspCancelOrderApiFail"
#define ICMD_H_rspCancelOrderError			305	//"rspCancelOrderError"
#define ICMD_H_rspOrderStatus				306	//"rspOrderStatus"
#define ICMD_H_rspTrade						307	//"rspTrade"


// ��ͷ���ݽṹ
typedef struct
{
	DWORD	SyncID;				//0xac34cd12
	ISessionIDType SessionID;	//sessionID
	DWORD	MsgType;			//��Ϣ����
	DWORD	CMDID;				//����ID
	DWORD	dwRspMsgNum;		//���ص���Ϣ���
	DWORD	Length;				//�����ȣ�������Э��ͷ��Э��β����sizeof(PackageHead + Data + PackageTail)
	DWORD	CRC;				//У�飬=0
} IPackageHeadStru;


// ��β��������
typedef DWORD PackageTailType;


// ��¼�ṹ��
typedef struct {
	IAppIdType		strAppID;		// AppID
	ILicenseType	strLicense;		// License
	IHostNameType 	strHostName;	// �ͻ��˼������
	IUserNameType 	strUserName;	// �û���
	IPasswordType 	strPassword;	// ����
	ITAType			strTraderAccount;	// �����˺�
	char 			chUseQuot;		// ʹ�����飬'1'��ʾʹ�ã�'0'��ʾ����
	char 			chUseOrder;		// ʹ���µ����ף�'1'��ʾʹ�ã�'0'��ʾ����
} ILOGINDATA, *PILOGINDATA;


// Session����ṹ��
typedef struct {
	ILOGINDATA		tLoginData;
	ISessionIDType	SessionID;
	INamedPipeType	strConnectAppName;
	INamedPipeType	strCreateAppName;
	DWORD			dwRemainTime;
} ISESSION, *PISESSION;


// ��¼�ɹ���ķ������ݽṹ��
typedef struct {
	IMessageType	strMessage;		// ������Ϣ�ַ���
	INamedPipeType	strAppPipeName;	// AppPipe��һ��names pipe
} ILOGINRETURN, *PILOGINRETURN;
