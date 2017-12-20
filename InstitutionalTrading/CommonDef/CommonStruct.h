#pragma  once
#include "string.h"
#include "..\..\..\FastTrader\inc\Module-Misc2\tools_util.h"
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
#pragma warning(disable : 4996)
//�������ͷ�ļ��ж���������˺Ϳͻ��˹��õ����ݽṹ��
#pragma pack(push)
#pragma pack(1)


//��Ÿ�ģ�鹲�е����ݽṹ��
typedef char _USERSTRING[21];
typedef char _PASSWORDSTRING[33];
typedef char _ExchangDateTimeType[18];
typedef char _ExchangTimeType[9];
typedef char _ExchangDateType[9];
typedef char _BrokerIDType[11];
typedef char _TraderAccount[13];
typedef char _SystemName[41];
typedef char _UserIDType[21];
typedef char _InstrumentID[31];
typedef char _SettlementDate[256];
typedef char _TradeIDType[21];

enum eBROKERTYPE 
{
    BROKER_TYPE_TRADE,//����
    BROKER_TYPE_QUOT  //����
};

//ɾ�����͹�˾ʱ���������顢ί�н����˺ŵ�brokeridΪ-1
#define TRADEACCOUNT_BROKERIDORSERVERID_NULL  -1 

//���͹�˾
struct BrokerInfo 
{
	int nBrokerID;
	_BrokerIDType szBrokerCode;
	char szCompany[65];

    int nType;  //0 ����ʹ�� 1 ����ʹ��
    char	szAccount[21];
    _PASSWORDSTRING	szPassword;  

    bool operator== (const BrokerInfo& r) const
    {
        int irlt;

        irlt=strncmp(szBrokerCode,r.szBrokerCode,sizeof(szBrokerCode));
        if(irlt!=0) return false;

        irlt=strncmp(szCompany,r.szCompany,sizeof(szCompany));
        if(irlt!=0) return false;

        irlt=strncmp(szAccount,r.szAccount,sizeof(szAccount));
        if(irlt!=0) return false;

        irlt=strncmp(szPassword,r.szPassword,sizeof(szPassword));
        if(irlt!=0) return false;

        if(nType!=r.nType)
            return false;

        return true;
    }
};

//��������
struct ServerGroup
{
	int nServerID;
	char szServerName[21];
	int nBrokerID;
	int nIsSettle;

    bool operator== (const ServerGroup& r) const
    {
        int irlt;

        irlt=strncmp(szServerName,r.szServerName,sizeof(szServerName));
        if(irlt!=0) return false;

        if(nBrokerID!=r.nBrokerID)
            return false;

        if(nIsSettle!=r.nIsSettle)
            return false;

        return true;
    }
};

//��������ַ
struct ServerAddress
{
    int nAddrID;	
    int nServerID;
	char szIP[128];
	int nPort;

    bool operator== (const ServerAddress& r) const
    {
        int irlt;

        irlt=strncmp(szIP,r.szIP,sizeof(szIP));
        if(irlt!=0) return false;

        if(nPort!=r.nPort)
            return false;

        if(nServerID!=r.nServerID)
            return false;

        return true;
    }
};

//�����������ַ
struct ServerAddressQuot
{
    int nAddrID;
    _BrokerIDType szBrokerCode;
    char szIP[128];
    int nPort;

    bool operator== (const ServerAddressQuot& r) const
    {
        int irlt;

        irlt=strncmp(szBrokerCode,r.szBrokerCode,sizeof(szBrokerCode));
        if(irlt!=0) return false;

        irlt=strncmp(szIP,r.szIP,sizeof(szIP));
        if(irlt!=0) return false;

        if(nPort!=r.nPort)
            return false;

        return true;
    }
};


//ί�н����˺�
struct TradeAccount
{
	int nTradeAccountID;
	_TraderAccount szTradeAccount;
	_PASSWORDSTRING szPwd;
	int nBrokerID;
	int nServerGroupID;
    double dProfit;//û��
	char szAlias[21];
	char szRealName[21];
    char szContact[41]; 
	int  nIDCardType;
	char szIDCardNo[21];	

    bool operator== (const TradeAccount& r) const
    {
        int irlt;

        irlt=strncmp(szTradeAccount,r.szTradeAccount,sizeof(szTradeAccount));
        if(irlt!=0) return false;

        irlt=strncmp(szPwd,r.szPwd,sizeof(szPwd));
        if(irlt!=0) return false;

        irlt=strncmp(szAlias,r.szAlias,sizeof(szAlias));
        if(irlt!=0) return false;

        irlt=strncmp(szRealName,r.szRealName,sizeof(szRealName));
        if(irlt!=0) return false;

        irlt=strncmp(szContact,r.szContact,sizeof(szContact));
        if(irlt!=0) return false;

        irlt=strncmp(szIDCardNo,r.szIDCardNo,sizeof(szIDCardNo));
        if(irlt!=0) return false;

        if(nBrokerID!=r.nBrokerID)
            return false;

        if(nServerGroupID!=r.nServerGroupID)
            return false;

        if(dProfit!=r.dProfit)
            return false;

        if(nIDCardType!=r.nIDCardType)
            return false;

        return true;
    }
};

//ί�н����˺�����������ϵ��
struct TradeAccountAndServerGroup
{
	int nTradeAccountID;		//�˺�ID
	int nServerGroupID;	        //��������ID
};

//ί�н����˺�����֯������ϵ��
struct TradeAccountAndOrg
{
    int nTradeAccountID;		//�˺�ID
    int nOrgID;	                //��֯����ID
};

enum eUSERANDORGRELATIONTYPE
{
    USER_ORG_RELATION_ATTACH,   //����
    USER_ORG_RELATION_MONITOR,  //���
    USER_ORG_RELATION_ALL       //�����ͼ��

};

#define ORGNIZATION_ROOT 10846209
//��֯����
struct Organization
{
	int nAssetMgmtOrgID;
	char szName[21];
	int nUpperLevelOrgID;
	int nScaleLevel;
	double dLongitude;
	double dLatitude;

    bool operator== (const Organization& r) const
    {
        int irlt;

        irlt=strncmp(szName,r.szName,sizeof(szName));
        if(irlt!=0) return false;

        if(nUpperLevelOrgID!=r.nUpperLevelOrgID)
            return false;

        if(nScaleLevel!=r.nScaleLevel)
            return false;

        if(dLongitude!=r.dLongitude)
            return false;

        if(dLatitude!=r.dLatitude)
            return false;

        return true;
    }
};

enum UserType
{	
	USER_TYPE_MANAGE,		//����Ա
	USER_TYPE_RISK_CONTROL,	//���Ա
	USER_TYPE_TRADER,		//����Ա
	USER_TYPE_VIEWER,		//չ��Ա
	USER_TYPE_SLAVESERVER,  //�ӷ������û�
	USER_TYPE_MASTERSERVER,	//��������
	USER_TYPE_INVALID		//��Ч���û�����
};
#define  SLAVE_USER_ID 1
// enum eUSERASSIGN
// {
//     UNASSIGN,  //������֯����
//     ASSIGN //δ������֯����
// };

//�������ݱ�����Ƿ�ɾ������
enum eDELETE
{
    UNDELETE_FLAG, //δɾ��
    DELETE_FLAG    //ɾ��
};

#define USER_ADMIN 10846209
//�û��ṹ
struct UserInfo
{
	int		nUserID;
	char	szAccount[21];
	_PASSWORDSTRING	szPassword;    
	UserType nUserType;				//�˻�����
	int		nStatus;				//����״̬
	char	szAlias[21];
	char	szContactInfo[101];
    int     nAssign;                //����֯�����У�0 δ���� 1 ����


    bool operator== (const UserInfo& r) const
    {
        int irlt;

        irlt=strncmp(szAccount,r.szAccount,sizeof(szAccount));
        if(irlt!=0) return false;

        irlt=strncmp(szPassword,r.szPassword,sizeof(szPassword));
        if(irlt!=0) return false;

        irlt=strncmp(szAlias,r.szAlias,sizeof(szAlias));
        if(irlt!=0) return false;

        irlt=strncmp(szContactInfo,r.szContactInfo,sizeof(szContactInfo));
        if(irlt!=0) return false;

        if(nUserType!=r.nUserType)
            return false;

        if(nStatus!=r.nStatus)
            return false;

        if(nAssign!=r.nAssign)
            return false;

        return true;
    }
};

//�û��޸�����
struct UserPassword
{
    int     nUserID;
    _PASSWORDSTRING szOldPassword; //������ 
    _PASSWORDSTRING	szNewPassword; //������ 
};

//�û�״̬����
struct UserStatus
{
    int nUserID;
    int nStatus; //0 ͣ�� 1 ����
};

//�û����ɫ��ϵ
struct UserRole
{
    int nUserID;
    int nRoleID;
};

//�����ݿ�ID��Ӧ
enum ePrivilege
{
    //����ԱȨ��
	ACCOUNT                                             =1, //�˻�
        ACCOUNT_MANAGE                                  ,//ί�н����˺Ź���
            ACCOUNT_ADD                                 ,//�����˺�
            ACCOUNT_MODIFY                              ,//�޸��˺�
            ACCOUNT_DEL                                 ,//ɾ���˺�
            ACCOUNT_LOGIN                               ,//��¼
        USER_MANAGE                                     =7,//ϵͳ�û�����
            USER_ADD                                    ,//�����û�
            USER_MODIFY                                 ,//�޸��û�
            USER_DEL                                    ,//ɾ���û�
            USER_START                                  ,//����Ա����
            USER_INIT                                   ,//����Ա��ʼ��
            USER_TRADER_ASSIGN_ORGANIZATION             ,//����Ա������֯�ܹ�
            USER_TRADER_ASSIGN_PRODUCT                  ,//����Ա������Ʋ�Ʒ
            USER_TRADER_ASSIGN_ACCOUNT                  ,//����Ա����ί�н����˺�
            USER_RISKER_SETUP_ORGANIZATION              ,//���Ա��֯��������
        FUND_MANAGE                                     =17,//����Ա��������
            FUND_OUT                                    ,//����
            FUND_IN                                     ,//���
    MANAGE                                              =20, //����
        ORGANIZATION_MANAGE                             ,//��֯��������
            ORGANIZATION_ADD                            ,//������֯�ܹ�
            ORGANIZATION_MODIFY                         ,//�޸���֯�ܹ�
            ORGANIZATION_DEL                            ,//ɾ����֯�ܹ�
            ORGANIZATION_ASSIGN_TRADER                  ,//��֯�������佻��Ա
        FIN_PRODUCT_MANAGE                              =26,//��Ʋ�Ʒ����
            FIN_PRODUCT_ADD                             ,//������Ʋ�Ʒ
            FIN_PRODUCT_MODIFY                          ,//�޸���Ʋ�Ʒ
            FIN_PRODUCT_DEL                             ,//ɾ����Ʋ�Ʒ
            FIN_PRODUCT_ASSIGN_TRADER                   ,//��Ʋ�Ʒ���佻��Ա
        BROKER_SERVER_MANAGE                            =31,//���͹�˾�����������
            BROKER_ADD                                  ,//���Ӿ��͹�˾
            BROKER_MODIFY                               ,//�޸ľ��͹�˾
            BROKER_DEL                                  ,//ɾ�����͹�˾
            SERVER_ADD                                  ,//���ӷ�������
            SERVER_MODIFY                               ,//�޸ķ�������
            SERVER_DEL                                  ,//ɾ����������
            SERVERADDR_ADD                              ,//���ӷ�������ַ
            SERVERADDR_MODIFY                           ,//�޸ķ�������ַ
            SERVERADDR_DEL                              ,//ɾ����������ַ
            BROKER_QUOT_MODIFY                          ,//�޸����龭�͹�˾
            SERVERADDR_QUOT_ADD                         ,//���������������ַ
            SERVERADDR_QUOT_MODIFY                      ,//�޸������������ַ
            SERVERADDR_QUOT_DEL                         ,//ɾ�������������ַ
    STRATEGY                                            =45, //����
        STRATEGY_ADD                                    ,//���ӽ��ײ���
        STRATEGY_MODIFY                                 ,//�޸Ľ��ײ���
        STRATEGY_DEL                                    ,//ɾ�����ײ���
        STRATEGY_ASSIGN_ORGANIZATION                    ,//���ײ��Է�����֯����
    PRIVILEGE                                           =50, //Ȩ��
        PRIVILEGE_ROLE_ADD                              ,//���ӽ�ɫ
        PRIVILEGE_ROLE_MODIFY                           ,//�޸Ľ�ɫ
        PRIVILEGE_ROLE_DEL                              ,//ɾ����ɫ
        PRIVILEGE_ROLE_ASSIGN_PRIVILEGE                 ,//��ɫ����Ȩ��
    SETTLEMENT                                          =55, //����
        SETTLEMENT_MANUAL                                ,//�������ݽ���
        SETTLEMENT_HISTORY                               ,//��ʷ����
        SETTLEMENT_AUTO_SETUP                            ,//�Զ�����ʱ������        
    QUERY                                               =59, //��ѯ
        QUERY_HISTORY_FUND                              ,//��ѯ��ʷ����
        QUERY_HISTORY_POSITION                          ,//��ѯ��ʷ�ֲ�
        QUERY_HISTORY_POSITION_DETAIL                   ,//��ѯ��ʷ�ֲ���ϸ
        QUERY_HISTORY_TRADE                             ,//��ѯ��ʷ�ɽ�
        QUERY_HISTORY_ORDER                             ,//��ѯ��ʷ����  
    CONTROL                                             =65, //����
        CONTROL_SERVER_RESET                            ,//����������
        CONTROL_TIME_SETUP                              ,//��ʱ����ʱ������
        CONTROL_SERVER_QUOT_RESET                       ,//�������������
        CONTROL_TIME_QUOT_SETUP                         ,//�����������ʱ����ʱ������

    //���ԱȨ��
	RISK_RISK                                           =70,//���
        RISK_MANUAL_VERIFY                              ,//�ֶ����
        RISK_LIMIT_TRADE                                ,//���ƽ���
        RISK_FORCE_CLOSE                                ,//ǿƽ
        RISK_ADD                                        ,//������֯������ط���
        RISK_MODIFY                                     ,//�޸���֯������ط���
        RISK_DEL                                        ,//ɾ����֯������ط���
	    RISK_SWITCH                                     ,//���ÿ���
    RISK_QUERY                                          =78,//��ѯ
        RISK_HISTORY_RISK_EVENT_QUERY                   ,//��ʷ�����¼���ѯ
        RISK_HISTROY_FUND_SUTTLE_QUERY                  ,//��ʷ����ֵ��ѯ
        RISK_ORDER_VERIFY_RECORD_QUERY                  ,//������˼�¼��ѯ
        RISK_FORCE_CLOSE_RECORD_QUERY                   ,//ǿƽ��¼��ѯ
        RISK_TRADER_BASE_INFO_QUERY                     ,//����Ա������Ϣ��ѯ

    // �û���ϢȨ��	
    MESSAGE                                             =84, //��Ϣ
        MESSAGE_QUERY                                   ,//��Ϣ��ѯ
        MESSAGE_SEND                                    ,//��Ϣ����

    //�������
    SETTLEMENT_AUTO_IMMEDIATE                        ,//�����Զ�����
};

//��ɫ
struct RoleInfo
{
    int nRoleID;
    char szName[21];
    int nUserTypeID;

    bool operator== (const RoleInfo& r) const
    {
        int irlt;

        irlt=strncmp(szName,r.szName,sizeof(szName));
        if(irlt!=0) return false;

        if(nUserTypeID!=r.nUserTypeID)
            return false;

        return true;
    }
};
//�û��������ɫ��ϵ
struct UserTypeAndRoleRelation
{
    int nUserTypeID;
    int nRoleID;
};

//Ȩ��
struct Privilege
{
	int nPrivilegeID;
	char szName[31];
    int nUserTypeID;    //�û�����
    int nParentID;      //���ڵ�    
    int nNodeLevel;     //�����ڵ�

    bool operator== (const Privilege& r) const
    {
        int irlt;

        irlt=strncmp(szName,r.szName,sizeof(szName));
        if(irlt!=0) return false;

        if(nUserTypeID!=r.nUserTypeID)
            return false;

        if(nParentID!=r.nParentID)
            return false;

        if(nNodeLevel!=r.nNodeLevel)
            return false;

        return true;
    }
};

//��ɫ��Ȩ�޵Ĺ�ϵ
struct RoleAndPrivilegeRelation
{
    int nRoleID;
    int nPrivilegeID;
};

struct UserAndTradeAccountRelation
{
	int nUserID;
	int nTradeAccountID;
	int nCanInputOrder;	    //�Ƿ������µ�
	int nNeedVerify;		//�µ��Ƿ���Ҫ��֤
};

struct UserAndOrgRelation
{
	int nUserID;
	int nOrgID;
	int nRelationType;	// 0 ����(��֯�ܹ�) 1 ��ع���(�û���)
    int nUserType;
};

//��Ʋ�Ʒ�ṹ
struct 	FinancialProduct
{
    int			nID;			                //��Ʋ�ƷID
    char		szName[20];					    //��Ʋ�Ʒ����    
    char 		szDesc[128];				    //��Ʋ�Ʒ����

    bool operator== (const FinancialProduct& r) const
    {
        int irlt;

        irlt=strncmp(szName,r.szName,sizeof(szName));
        if(irlt!=0) return false;

        irlt=strncmp(szDesc,r.szDesc,sizeof(szDesc));
        if(irlt!=0) return false;

        return true;
    }
};

enum eTraderProRelationType
{
    TRADER_ASSGIN_PRODUCT_TYPE,   //����Աָ����Ʋ�Ʒ
    PRODUCT_ASSGIN_TRADER_TYPE   //��Ʋ�Ʒָ������Ա
};
//��Ʋ�Ʒ�뽻��Ա��ϵ�ṹ
struct 	TraderProRelation
{
    int 			nFinancialProductID;	//��Ʋ�ƷID
    int 			nTraderID;				//����ԱID 
};

//���ײ��Խṹ
struct TradeStrategy
{
    int 		nID;			    //����ID
    char 	    szName[21];	        //��������
    int         nFrequency;         //���Զ�ȡƵ��
    char	    szAddr[256];	    //�����ļ�����·��

    bool operator== (const TradeStrategy& r) const
    {
        int irlt;

        irlt=strncmp(szName,r.szName,sizeof(szName));
        if(irlt!=0) return false;

        irlt=strncmp(szAddr,r.szAddr,sizeof(szAddr));
        if(irlt!=0) return false;

        if(nFrequency!=r.nFrequency)
            return false;

        return true;
    }
};

//���ײ�������֯������ϵ�ṹ
struct AssetOrgStrategyRelation
{
    int 		nStrategyID;			//����ID
    int		    nAssetOrgID;		    //�����ڵ�ID

    bool operator<(const AssetOrgStrategyRelation&	src) const
    {
        if(nAssetOrgID<src.nAssetOrgID)
            return true;
        else if(nAssetOrgID==src.nAssetOrgID)
        {
            if(nStrategyID<src.nStrategyID)
                return true;
        }
        return false;
    }
};

// �û���Ϣ����
enum enmMsgType 
{
	conManualMsg, conAutoMsg, conStrategyMsg
};

// �û�Send��Ϣtcp����ṹ��
struct UserSendMsgStru
{
	int		OwnerUserID;			// ��Ϣ������
	int		ExpiredDate;			// ��Ϣ������
	int		TargetUserCount;			// ��Ϣ������
	char	Title[32];				// ��Ϣ����
	char	Msg[512];				// ��Ϣ����
	int		TargetUsers[1];			// Ŀ���û�ID��
};

// �û�Recv��Ϣtcp����ṹ��
struct UserMsgStru
{
	int		MsgID;					// ��ϢID
	int		TargetMsgID;			// Ŀ����ϢID
	int		MsgType;				// ��Ϣ����
	int		TargetUserID;			// ��Ϣ������
	int		OwnerUserID;			// ��Ϣ�����ˣ������ϵͳ�Զ���Ϣ����Ϊ0
	int		ExpiredDate;			// ��Ϣ������
	char	CreateDateTime[20];		// ��Ϣ��������ʱ�䣬��ʽ"yyyy-mm-dd HH:MM:SS"
	char	SendDateTime[20];		// ��Ϣ��������ʱ�䣬��ʽ"yyyy-mm-dd HH:MM:SS"
	char	Title[32];				// ��Ϣ����
	char	Msg[512];				// ��Ϣ����
};

// �û���Ϣ�ṹ
struct UserMsgMemStru 
{
	bool	IsOrigMsg;				// ��ԭ��Ϣ����Ŀ����Ϣ�����������������ĸ�����
									// ԭ��Ϣ������ԭ��Ϣ���У�Ŀ����Ϣ������Ŀ����Ϣ����
	bool	HasSend;				// �Ƿ��Ѿ�����
	bool	HasRead;				// �Ƿ��Ѿ��Ķ���������ʱ����
	UserMsgStru msg;				// ��Ϣ�ṹ��
};

// �û���Ϣ״̬�ṹ
struct UserMsgStatusStru 
{
	int		TargetUserID;			// �û�ID
	int		TargetMsgID;			// Ŀ����ϢID
};


///���õ�½ʱ����������Ϣ
struct sExchangeTimeInfo
{
	///��ȡ����ʱ����Ϣ��ʱ��
	_ExchangTimeType	GetTime;
	///������
	_ExchangDateType	TradingDay;	
	///������ʱ��
	_ExchangTimeType	SHFETime;
	///������ʱ��
	_ExchangTimeType	DCETime;
	///֣����ʱ��
	_ExchangTimeType	CZCETime;
	///�н���ʱ��
	_ExchangTimeType	FFEXTime;
};
//��½����
enum eClientType
{
	CLIENT_TYPE_ADMIN = 0, //����ͻ���
	CLIENT_TYPE_TRADE,     //���׿ͻ���
	CLIENT_TYPE_RISK,      //��ؿͻ���
	CLIENT_TYPE_SHOW       //չ�ֿͻ���
};
struct sLoginReq
{
	_USERSTRING msUserName;         //�û���
	_PASSWORDSTRING msPassWord;     //�û�����
	UserType meUserType;            //�û�����
	eClientType meClientType;       //�ͻ�������
};



//���׿ͻ��˵�½����
struct sTraderLoginRsp
{
	int		mnUserID;                //�û�ID   
	int     mnFrontID;               //ǰ��ID
	int     mnSessionID;             //�ỰID
	int     mnMinOrderRef;           //��С��Orderrefֵ		
	_SystemName	        mSystemName; ///����ϵͳ����
	_BrokerIDType       mBrokerID;   //����ʹ�ã����
	_ExchangTimeType	mLoginTime;  ///��¼�ɹ�ʱ��
	_ExchangDateType	mTradingDay; ///������	
	_ExchangTimeType	mSHFETime;   ///������ʱ��
	_ExchangTimeType	mDCETime;    ///������ʱ��
	_ExchangTimeType	mCZCETime;	 ///֣����ʱ��	
	_ExchangTimeType	mFFEXTime;   ///�н���ʱ��

};
//��½����
struct sLoginRsp
{
	int		mnUserID;                //�û�ID   
	int     mnFrontID;               //ǰ��ID
	int     mnSessionID;             //�ỰID
	int     mnMinOrderRef;           //��С��Orderrefֵ
	int     mnPrivilegeLen;          //Ȩ�޼��ϵĳ���
	int     mnPemission[1];          //Ȩ�޼���

};
enum eSocketStatus
{
	SocketStatus_Validated = 0,     //��Ч��socket
	SocketStatus_Connected,         //�������ӣ���û��ͨ����֤
	SocketStatus_Exception,         //�쳣�Ͽ�
	SocketStatus_Timeout,           //��ʱ
	SocketStatus_Logoff             //ע��

};

enum RiskLimitOpt
{
	NO_FORBID,		//������
	FORBID_ORDER,	//��ֹ����
	FORBID_OPEN_ORDER,	//��ֹ����
};

enum RiskLimitDirection
{
	DIRECTION_NO_FORBID,		//������
	DIRECTION_FORBID_BUY,		//��ֹ��
	DIRECTION_FORBID_SELL,		//��ֹ��
	DIRECTION_FORBID_BOTH_SIDE,	//˫�򶼽�ֹ
};

enum RiskLimitRange
{
	FORBID_NONE,				//������
	FORBID_ALL_PRODUCT,			//����Ʒ������
	FORBID_SINGLE_PRODUCT,		//��Ʒ������
	FORBID_SINGLE_INSTRUMENT,	//����Լ����
};

//�����¼������Ľ�������
struct RiskLimit
{
	RiskLimitOpt opt;
	RiskLimitDirection direction;
	RiskLimitRange range;
};

enum StrategyOrderLimit
{
	STRATEGY_NO_FORBID,			//������
	STRATEGY_FORBID_BUY,		//��ֹ��
	STRATEGY_FORBID_SELL,		//��ֹ��
	STRATEGY_FORBID_BOTH_SIDE,	//˫�򶼽�ֹ
};

struct InputOrderKey
{
	int nFrontID;
	int nSessionID;
	char szOrderRef[13];

	bool operator< (const InputOrderKey& r) const
	{
		int irlt;		
		if(nFrontID<r.nFrontID)
			return true;
		else if(nFrontID>r.nFrontID)
			return false;

		if(nSessionID<r.nSessionID)
			return true;
		else if(nSessionID>r.nSessionID)
			return false;

		irlt=strncmp(szOrderRef,r.szOrderRef,sizeof(szOrderRef));
		if(irlt<0) return true;
		if(irlt>0) return false;

		return false;
	}
};

//struct VerifyOrderRtn
//{
//	OrderKey orderKey;
//	bool bRet;
//};

//enum InputOrderStatus
//{
//	InputOrder_Receive,	//���ձ���ָ��
//	InputOrder_Verifying,	//�ֶ������
//	InputOrder_Verify_End,	//������
//	InputOrder_CTP_Received,	//ctp�������ѽ��ձ���
//	InputOrder_Part_Transacted,	//���ֳɽ�
//	InputOrder_Complete_Transacted,	//ȫ���ɽ�
//};

struct SettleTradeKey
{
	SettleTradeKey()
	{
		memset(this,0,sizeof(*this));		
	};

	SettleTradeKey(const char* nInvestorID,const char* nTradeID)
	{
		memset(this,0,sizeof(*this));

		if(nInvestorID != NULL && nTradeID != NULL)
		{
			strcpy_s(InvestorID,sizeof(InvestorID),nInvestorID);
			strcpy_s(TradeID,sizeof(TradeID),nTradeID);

		}


	};


	//SettleTradeKey& operator= (const SettleTradeKey& other)
	//{
	//	if (this == &other)
	//		return *this;

	//	memcpy((char*)this,(char*)&other,sizeof(*this));
	//	return *this;
	//};

	bool operator< (const SettleTradeKey& r) const
	{
		int irlt;
		irlt=strncmp(InvestorID,r.InvestorID,sizeof(InvestorID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(TradeID,r.TradeID,sizeof(TradeID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		return false;
	}

	bool operator== (const SettleTradeKey& r) const
	{
		int irlt;
		irlt=strncmp(InvestorID,r.InvestorID,sizeof(InvestorID));
		if(irlt!=0) return false;	

		irlt=strncmp(TradeID,r.TradeID,sizeof(TradeID));
		if(irlt!=0) return false;

		return true;
	}

	bool operator> (const SettleTradeKey& r) const
	{
		return !((*this) < r || (*this) == r);
	}		
	///Ͷ���ߴ���
	_TraderAccount	InvestorID;
	///�ɽ����
	_TradeIDType	TradeID;
};

struct RateKey
{
	RateKey()
	{
		memset(this,0,sizeof(*this));		
	};

	RateKey(const char* nAccountID,const char* nInstrumentID,const char* nBrokerID)
	{
		memset(this,0,sizeof(*this));

		if(nAccountID != NULL && nInstrumentID != NULL && nBrokerID != NULL)
		{
			strcpy_s(AccountID,sizeof(AccountID),nAccountID);
			strcpy_s(InstrumentID,sizeof(InstrumentID),nInstrumentID);
			strcpy_s(BrokerID,sizeof(BrokerID),nBrokerID);
		}


	};


	RateKey& operator= (const RateKey& other)
	{
		if (this == &other)
			return *this;

		memcpy((char*)this,(char*)&other,sizeof(*this));
		return *this;
	};

	bool operator< (const RateKey& r) const
	{
		int irlt;
		irlt=strncmp(AccountID,r.AccountID,sizeof(AccountID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(BrokerID,r.BrokerID,sizeof(BrokerID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		return false;
	}

	bool operator== (const RateKey& r) const
	{
		int irlt;
		irlt=strncmp(AccountID,r.AccountID,sizeof(AccountID));
		if(irlt!=0) return false;	

		irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
		if(irlt!=0) return false;

		irlt=strncmp(BrokerID,r.BrokerID,sizeof(BrokerID));
		if(irlt!=0) return false;

		return true;
	}

	bool operator> (const RateKey& r) const
	{
		return !((*this) < r || (*this) == r);
	}		
	///��Լ����
	_InstrumentID	InstrumentID;
	///Ͷ���ߴ���
	_TraderAccount	AccountID;	
	///���͹�˾����
	_BrokerIDType	BrokerID;

};
struct QryHisRateCondition
{
	RateKey conditionKey;
	char loadDate[11];
};
struct CommissionMarginRate
{
	CommissionMarginRate()
	{
		memset(this,0,sizeof(*this));
		LongMarginRatioByMoney=util::GetDoubleInvalidValue();
		LongMarginRatioByVolume=util::GetDoubleInvalidValue();
		ShortMarginRatioByMoney=util::GetDoubleInvalidValue();
		ShortMarginRatioByVolume=util::GetDoubleInvalidValue();
		OpenRatioByMoney=util::GetDoubleInvalidValue();
		OpenRatioByVolume=util::GetDoubleInvalidValue();
		CloseRatioByMoney=util::GetDoubleInvalidValue();
		CloseRatioByVolume=util::GetDoubleInvalidValue();
		CloseTodayRatioByMoney=util::GetDoubleInvalidValue();
		CloseTodayRatioByVolume=util::GetDoubleInvalidValue();

	}
	///���͹�˾����
	_BrokerIDType	BrokerID;
	///Ͷ���ߴ���
	_TraderAccount	AccountID;	
	///��Լ����
	_InstrumentID	InstrumentID;
	///��ͷ��֤����
	double	LongMarginRatioByMoney;
	///��ͷ��֤���
	double	LongMarginRatioByVolume;
	///��ͷ��֤����
	double	ShortMarginRatioByMoney;
	///��ͷ��֤���
	double	ShortMarginRatioByVolume;
	///������������
	double	OpenRatioByMoney;
	///����������
	double	OpenRatioByVolume;
	///ƽ����������
	double	CloseRatioByMoney;
	///ƽ��������
	double	CloseRatioByVolume;
	///ƽ����������
	double	CloseTodayRatioByMoney;
	///ƽ��������
	double	CloseTodayRatioByVolume;
};
struct SettlementPriceField
{
	///��Լ����
	_InstrumentID	InstrumentID;
	///����������
	char	ExchangeID[9];
	///��Ʒ����
	char	ProductID[64];
	///�����
	double	SettlementPrice;
	///������
	double	LastSettlementPrice;
	//��������
	char	SettlementDate[9];
};

enum SettlementOperationActionType
{
	Action_AddRecord,	    //���Ӽ�¼
	Action_ModifyRecord,	//�޸ļ�¼
	Action_DeleteRecord,	//ɾ����¼
};
enum SettlementBusinessType
{

	Business_SettlementPrice,
	Business_CommissionRate,
	Business_MarginRate,
	Business_Trade,
	Business_PositionDetail,
	Business_Position,
	Business_Fund,
};
//����Ա����������¼
struct SettlementOperation
{
	int		                      nUserID;//����ԱID
	char 	                      szTraderID[13];//����ԱID	
	char	                      szTradingDay[11];//��������
	char                          szInstrumentID[31];//��ԼID
	SettlementBusinessType        nBusinessType;//����ҵ������
	SettlementOperationActionType           nActionType;//��������
	char                          szOperationItem[21];//������
	double                        dOriginalContent;//ԭ��������
	double                        dNewContent;//�޸ĺ������
};

enum eSettlementStatus
{
	No_Settlementing = 0, //�ǽ���״̬
	Manul_Settlementing,  //�����ֶ�����
	Auto_Settlementing    //�����Զ�����
};

enum EnumAccountStatus
{    
	ACCOUNT_STATUS_UnInit							=      0,            //δ��ʼ��
	ACCOUNT_STATUS_CannotConnect					=      1,            //���Ӳ���
	ACCOUNT_STATUS_Connected						=      2,            //������
	ACCOUNT_STATUS_Disconnected						=      3,            //�Ͽ�����	
	ACCOUNT_STATUS_Login							=      4,            //��¼�ɹ�
	ACCOUNT_STATUS_LoginFailure						=      5,            //��¼ʧ��
	ACCOUNT_STATUS_TradeSettlementInfoConfirming    =	   6,			 //���ڽ��н��׽�����ȷ��
	
};
enum eUserInitStatus
{
	USER_STATUS_UnInit=0,                                        //δ��ʼ��
	USER_STATUS_InitQryInstrumentsReady=1,					     //��Լ���ݲ�ѯ���
	USER_STATUS_InitQryRatesReady =2,				        	 //���ʺͱ�֤���ʲ�ѯ���
	USER_STATUS_InitQryTradesReady =4,				        	 //�ɽ���ѯ���	
	USER_STATUS_InitQryOrdersReady =8,				        	 //������ѯ���
	USER_STATUS_InitQryFundInfoReady =16,				         //�ճ����ʽ��ѯ���
	USER_STATUS_InitQryPositionReady =32,				         //�ճ��ĳֲֲ�ѯ���
	USER_STATUS_InitQryPositionDetailReady =64,				     //�ճ��ĳֲ���ϸ��ѯ���
	USER_STATUS_Inited=127,				        	             //��ʼ�����
	USER_STATUS_InitFailed=128,			        	             //��ʼ��ʧ��

};

//������״̬
enum eExchangeStatus
{
	ExchangeStatus_PreOpen = 0,  //����ǰ
	ExchangeStatus_ContinueTrade,//��������
	ExchangeStatus_UnableTrade   //�ǽ���

};
enum eTradeDataStatus
{
	TradeDataStatusInitialing = 0,      //ģ���ʼ��״̬�����ж���ӿھ�������
	TradeDataStatusLastDaySettlemtNotOK,//û��ȡ�������գ��������ݿ����д�����Ľ����գ������ý����յ����н������ʷ����
    TradeDataStatusNoTradingDay,        //û��ȡ�������գ����ݿ���Ҳû�д�����Ľ����ա�ֻ������ʷ����
	TradeDataStatusInitFail,            //��ʼ��ʧ��״̬����������й��ں�Լ��ԭ���µĳ�ʼ��ʧ��
	TradeDataStatusTrading,             //�������ݹ���ģ�������ڽ���״̬ ����Ĭ�Ͻ��뽻��״̬
	TradeDataStatusSettlemtIniting,     //�����ʼ��״̬
	TradeDataStatusSettlemting,         //�����ʼ����ɺ󣬽������״̬
	TradeDataStatusSettlemtEnd          //�������
};

enum eSettlementLoadOption
{
	SettlementLoadOption_TradingDataWithTodayDB = 0, //���������ݿ��е�ԭʼ���ݿ�ʼ����
	SettlementLoadOption_TradingDataWithCTPSett,   	 //���������ݿ��е�ԭʼ��Լ���� + CTP����ɽ�����
	SettlementLoadOption_TradingDataWithHistory   	 //�����ݿ��ж���ʷ���ݽ��н���

};

enum eDataReady
{
	DataReday_NoReady = 0,
	DataReady_Ready
};

enum eCommissionMarginReady
{
	CommissionMargin_NoReady = 0,
	CommissionMargin_ComissionReady = 0x0001,
	CommissionMargin_MarginReady = 0x0002,
	CommissionMargin_ComissionAndMarinReady = 0x0003
};

//struct sBrokerAccount
//{
//	///���͹�˾����
//	_BrokerIDType	BrokerID;
//	///ί�н����˺Ŵ���
//	_TraderAccount	AccountID;	
//};
///��ʾһ��������¼
struct BrokerAccountKey
{
	BrokerAccountKey()
	{
		memset(this,0,sizeof(*this));		
	};

	BrokerAccountKey(const char* npsBrokerID,const char* npsAccountID)
	{
		memset(this,0,sizeof(*this));

		if(npsAccountID != NULL && npsBrokerID != NULL)
		{
			//strcpy_s(BrokerID,sizeof(BrokerID),npsBrokerID);
			//strcpy_s(AccountID,sizeof(AccountID),npsAccountID);
			//���ȳ���������
			strncpy_s(BrokerID, sizeof(BrokerID),npsBrokerID, sizeof(BrokerID)-1);
			strncpy_s(AccountID, sizeof(AccountID),npsAccountID, sizeof(AccountID)-1);
		}
		

	};


	BrokerAccountKey& operator= (const BrokerAccountKey& other)
	{
		if (this == &other)
			return *this;

		memcpy((char*)this,(char*)&other,sizeof(*this));
		return *this;
	};

	bool operator< (const BrokerAccountKey& r) const
	{
		int irlt;

		irlt=strncmp(BrokerID,r.BrokerID,sizeof(BrokerID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(AccountID,r.AccountID,sizeof(AccountID));
		if(irlt<0) return true;
		if(irlt>0) return false;


		return false;
	}

	bool operator== (const BrokerAccountKey& r) const
	{
		int irlt;

		irlt=strncmp(BrokerID,r.BrokerID,sizeof(BrokerID));
		if(irlt!=0) return false;

		irlt=strncmp(AccountID,r.AccountID,sizeof(AccountID));
		if(irlt!=0) return false;		

		return true;
	}

	bool operator> (const BrokerAccountKey& r) const
	{
		return !((*this) < r || (*this) == r);
	}		
	///���͹�˾����
	_BrokerIDType	BrokerID;
	///Ͷ���ߴ���
	_TraderAccount	AccountID;	

};

struct sAccountStatus
{
	BrokerAccountKey mBrokerAccount;
	eCommissionMarginReady meCommissionAndMarginReady; //�����Ƿ�׼����
	EnumAccountStatus meStatus;                        //CTP����״̬
	double ldbDuration;                                //����״̬��ʱ�䳤
};
struct sUserInitStatus
{
	_UserIDType	szUserAccount;              
	eUserInitStatus meStatus;                //�û��ĳ�ʼ��״̬
	EnumAccountStatus meCTPStatus;           //���û�������ί�н����˺ŵ�ctp״̬
	double ldbDuration;                      //����״̬��ʱ�䳤
};



//���̴�ȡ���ݿ������ݽṹ
typedef char OTACCOUNTTYPE[16];
typedef char DateType[21];
typedef char ORDERSYSID[21];
typedef char TExchangeIDType[9];
struct SOrderTransfer
{
	OTACCOUNTTYPE szInvestorID;//����ԱID
	int  nFrontID;
	int	 nSessionID;
	int  nOrderRef;
	
	OTACCOUNTTYPE szBrokerID;//�˺ž��͹�˾ID
	OTACCOUNTTYPE szAccountID;//�˺�ID
	int	 nCTPOrderRef;
	int  nCTPFrontID;
	int  nCTPSessionID;
	DateType                szUpdateDate;
	ORDERSYSID  orderSysID;   ///�������
	TExchangeIDType	ExchangeID; //������ID

	int						UserForceClose;	//�û�ǿƽ��־ ��ؿͻ���ǿƽʱ��Ϊ1����ȻΪ0
	char			        ForceCloseReason; //��������   ��ؿͻ���ǿƽʱ��ΪTHOST_FTDC_FCC_ForceClose,��ȻΪ��0��
	SOrderTransfer()
	{
		memset(this,0,sizeof(*this));		
	}
	SOrderTransfer& operator= (const SOrderTransfer& order)
	{
		strcpy(szInvestorID, order.szInvestorID);
		nFrontID = order.nFrontID;
		nSessionID = order.nSessionID;
		nOrderRef = order.nOrderRef;
		strcpy(szBrokerID, order.szBrokerID);
		strcpy(szAccountID, order.szAccountID);
		nCTPOrderRef = order.nCTPOrderRef;
		nCTPFrontID = order.nCTPFrontID;
		nCTPSessionID = order.nCTPSessionID;
		strcpy(szUpdateDate, order.szUpdateDate);
		strcpy(orderSysID, order.orderSysID);
		strcpy(ExchangeID, order.ExchangeID);
		UserForceClose		= order.UserForceClose;
		ForceCloseReason	= order.ForceCloseReason;

		return *this;
	}
	bool operator<(const SOrderTransfer item) const
	{

		int i = strcmp(szInvestorID, item.szInvestorID);			
		if(i<0)
			return true;
		else if(i>0)
			return false;

		if(nFrontID < item.nFrontID)
			return true;
		else if(nFrontID > item.nFrontID)
			return false;

		if(nSessionID < item.nSessionID)
			return true;
		else if(nSessionID > item.nSessionID)
			return false;

		if(nOrderRef < item.nOrderRef)
			return true;
		else if(nOrderRef > item.nOrderRef)
			return false;

		i = strcmp(szBrokerID, item.szBrokerID);			
		if(i<0)
			return true;
		else if(i>0)
			return false;

		i = strcmp(szAccountID, item.szAccountID);			
		if(i<0)
			return true;
		else if(i>0)
			return false;

		if(nCTPOrderRef < item.nCTPOrderRef)
			return true;
		else if(nCTPOrderRef > item.nCTPOrderRef)
			return false;

		if(nCTPFrontID < item.nCTPFrontID)
			return true;
		else if(nCTPFrontID > item.nCTPFrontID)
			return false;

		if(nCTPSessionID < item.nCTPSessionID)
			return true;
		else if(nCTPSessionID > item.nCTPSessionID)
			return false;

		i = strcmp(szUpdateDate, item.szUpdateDate);			
		if(i<0)
			return true;
		else if(i>0)
			return false;

		i = strcmp(orderSysID, item.orderSysID);			
		if(i<0)
			return true;
		else if(i>0)
			return false;

		i = strcmp(ExchangeID, item.ExchangeID);			
		if(i<0)
			return true;
		else if(i>0)
			return false;

		if(UserForceClose < item.UserForceClose)
			return true;
		else if(UserForceClose > item.UserForceClose)
			return false;

		if(ForceCloseReason < item.ForceCloseReason)
			return true;
		else if(ForceCloseReason > item.ForceCloseReason)
			return false;
		return false;
	}
};

enum eInOut
{
	FundIN = 0, //���
	FundOut     //����
};
//�����
struct sFundInOut
{
	_UserIDType  mUserID;     //����Ա�˻�
	eInOut       meInOut;     //������
	double       mdbVolume;   //���
	char         msDesc[200]; //��ע
	_UserIDType  mOpAdminID;  //�����Ĺ���Ա�˻�
	char		 msDay[11];   //��������
	char		 msTime[9];   //����ʱ��
};

struct sQryFundInOut
{
	_UserIDType  mUserID;           //����Ա�˻� ��������е�������*
	char msStartDay[11];    //��ʼ��������
	char msEndDay[11];      //��ʼ��������
};


struct sReinitTime
{
	_ExchangTimeType mNightTime; //ҹ������ʱ��
	_ExchangTimeType mDayTime;  //��������ʱ��
};
struct sSaveTime
{
	_ExchangTimeType mSaveTime1; //���б�������
	_ExchangTimeType mSaveTime2;   //��β����ʱ��
};
struct sTradingTime
{
	_ExchangTimeType mTradingDayStart; //���̿�ʼʱ��
	_ExchangTimeType mTradingDayEnd ;  //���̽���ʱ��
	_ExchangTimeType mTradingNightStart; //ҹ�̿�ʼʱ��
	_ExchangTimeType mTradingNightEnd ;  //ҹ�̽���ʱ��
};

///////////////////////////���///////////////////////////////////////////////
//�޸�����
struct sModifyPassword
{
    char szOldPassword[21];
    char szNewPassword1[21];
    char szNewPassword2[21];
};

//��������
struct sUnLockPassword
{
    int nRiskerID;
    char szUnLockPassword[21];
};


//չ��ͳ��ʹ�õĽṹ��
//��Ʋ�Ʒ���ʹ���֯��ϵ
struct AssetOrgProRelation
{
	int mnAssetMgmtOrgID;     //��֯����ID
	int mnFinancialProductID; //��Ʋ�ƷID

	bool operator<(const AssetOrgProRelation&	src) const
	{
		if(mnAssetMgmtOrgID<src.mnAssetMgmtOrgID)
			return true;
		else if(mnAssetMgmtOrgID==src.mnAssetMgmtOrgID)
		{
			if(mnFinancialProductID<src.mnFinancialProductID)
				return true;
		}
		return false;
	}

};
enum eStatisticsIDType
{
	ST_PosProfit=1,//1.	�ֲ�ӯ������
	ST_InitFund,//2.	�˻���ʼȨ�����
	ST_Balance,//3.	�˻���ǰ��Ȩ�����
	ST_UseMargin,//4.	��Լ�ֱֲ�֤�����
	ST_Position,//5.	��Լ�ĳֲ�������
};
struct	StatisticsEntry
{
	eStatisticsIDType		mnStatisticsType;
	int						mnAssetMgmtOrgID;
	int						mnFinancialProductID;	
	_InstrumentID			mInstrumentID;
	double					mdblValue;
};

//////////////////////////////////////////////////////////////////////////
//����������־�ṹ

enum OperType
{
    Oper_Add,
    Oper_Delete,
    Oper_Modify,
    Oper_Save,
    Oper_Add_Trade,
    Oper_Add_Quot,
    Oper_Modify_Trade,
    Oper_Modify_Quot    
};

struct LogBaseData
{
    char     logTable[41];   // Ҫ�������ݿ�ı�
    char	 logDateTime[20];// ����ʱ�䣬��ʽ"yyyy-mm-dd HH:MM:SS"
    int      logOperatorID;  // ������ID
    int      logTargetorID;  // ��������ID
    OperType logAction;      // ��������
    char     logContent[256];// ��������
    bool     logResult;      // �������
};

//ϵͳ������־����
enum eErrorType
{
	FirstLevelError,  //1������Ӱ��ϵͳ���еĴ���������
	SecondLevelError, //2�����󣬲�Ӱ��ϵͳ���еĴ���������
	ThirdLevelError,  //3�����󣬲�Ӱ��ϵͳ���У�������
	Logger,			  //ϵͳ������־ ����ѡ����
    OperationLogNeedSend,			  //ϵͳ������־�����͵������Ŀͻ���������־
};
enum eSendBit
{
    eSendManager = 1, //���͸������ն�
    eSendRisk = 2,    //���͸����з���ն�
    eSendPresentation = 4, //���͸�չ���ն�
    eSendTrader = 8,       //���͸������ն�
    eSendSpecialUserClient = 16,//���͸��ض����û�ID
};
#define  MAX_SEND_SPECIAL_USER_SIZE 50
struct sErrorInfo
{
	sErrorInfo()
	{
		memset(msError,0,512);
        mhSocket = 0;
        mnSendBit = 0;
		ClearArray();
	
	}
	sErrorInfo(eErrorType nErrorType,char* npMsg)
	{
		meErrorType = nErrorType;
		memset(msError,0,512);
	    ClearArray();
		if(npMsg)
			strcpy(msError,npMsg);
	}

    sErrorInfo(eErrorType nErrorType,char* npMsg,int nSocket,int nSendBit)
    {
        meErrorType = nErrorType;
        memset(msError,0,512);
        if(npMsg)
            strcpy(msError,npMsg);
        mnSendBit = nSendBit;
        mhSocket = nSocket;
		ClearArray();
    }
	void ClearArray()
	{
		for(int i = 0; i < MAX_SEND_SPECIAL_USER_SIZE; i++)
			mnSpecialUserIDArray[i] = 0;
	}
	eErrorType meErrorType;  //��������
	char       msError[512];   // ������־
    int        mhSocket;      //����socket
    int        mnSendBit;     //���͵Ŀͻ������λ ��
	int	       mnSpecialUserIDArray[MAX_SEND_SPECIAL_USER_SIZE]; //���͵��ض��û�ID����
};


struct sUserOnlineInfo
{
	int mUserID;           //�û�iD
	int mUserOnlineCount;  //�û�������
};

//////////////////////////////////////////////////////////////////////////
enum eServerRunMode 
{
    ServerRunMode_Master,//������
    ServerRunMode_Slave  //�ӷ���
};


//#ifdef _SIMULATE_
//ģ��������������
enum eSimulateDayType
{
    SimulateDayType_Special, //���������
    SimulateDayType_Nearest //ָ��������
};
//����ط�����
enum eSimulatePlayMode
{
    SimulatePlayMode_Normal, //ʵ�������ط�
    SimulatePlayMode_NoDelay //��ʱ���ӳٻط�
};
struct sSimulateDay
{
    eSimulateDayType mType;     //ģ����������ѡ������ 1�����һ�������գ�Ĭ���������Ч��0��ָ�������գ�����������Ч��
    _ExchangDateType mStartDay; //��ʼ������
    _ExchangDateType mEndDay;   //����������
};
struct sSimulateTime
{
    _ExchangTimeType mStartTime; //����ʱ���
    _ExchangTimeType mEndTime;  //ֹͣʱ���
};
//#endif

enum QryPropertyAccount
{
	QryIsAccount = 0,
	QryIsUser
};
enum QryPropertyTrader
{
	QryIsTrader = 0,
	QryIsOrder
};
enum QryPropertyEnd
{
	QryIsNotEnd = 0,
	QryIsEnd
};


//����Ϊ����ƽ̨����


// ָ������
enum IndicatorType
{
    BarIndicator = 0,   //K��ָ��
	MDIndicator         //����ָ��

};

//ָ���ļ���
enum IndicatorFile
{
	PriceIndicatorFile = 0,    //�۸�ָ��
	TradingVolumeFile,          //������ָ��
	IndicatorSampleFile         //ָ������


};
//ָ��
struct IndicatorInfo
{
	//int              nIndicatorID;       //ָ��ID
	char             szName[32];         //ָ������
	IndicatorType    eIndiType;          // ָ������
	IndicatorFile    eIndiFile;          //ָ���ļ���
	char             szDescribe[256];     //ָ������

	bool operator== (const IndicatorInfo& r) const
	{
		int irlt;

		irlt=strncmp(szName,r.szName,sizeof(szName));
		if(irlt!=0) return false;
		irlt=strncmp(szDescribe,r.szDescribe,sizeof(szDescribe));
		if(irlt!=0) return false;

		if(eIndiType!=r.eIndiType)
			return false;
		if(eIndiFile!=r.eIndiFile)
			return false;

		return true;
	}
};

//ָ���������
enum  IndicatorParaType
{
	IntegerType = 0,
	DoubleType,
	BooleanType      

};
//ָ�����
struct  IndicatorPara
{
	char               szName[32];                //��������
	IndicatorParaType  eType;                     //ָ���������
	char               szDefault[128];             // Ĭ��ֵ

	bool operator== (const IndicatorPara& r) const
	{
		int irlt;

		irlt=strncmp(szName,r.szName,sizeof(szName));
		if(irlt!=0) return false;

		irlt=strncmp(szDefault,r.szDefault,sizeof(szDefault));
		if(irlt!=0) return false;

		if(eType!=r.eType)
			return false;

		return true;
	}

};
//

typedef char StrategyName[21]; 
typedef char ACCOUNTSTRING[21];
typedef char VERSIONSTRING[11];
typedef char TInstrumentID[255];
typedef char TParameterType[2550];
typedef char TCommentType[255];

typedef char INDEXSTRING[21];

typedef char UPLOADSTRING[2000];
typedef char UPLOADEND[50];
//����
struct SStrategy
{
	StrategyName		strategyName;		//������
	StrategyName		strategyNickName;	//����������
	VERSIONSTRING		Version;			//���԰汾��
	ACCOUNTSTRING		strTraderName;		//�ϴ���
	int					nPrivate;			//1:˽�У�2����
	TParameterType      strPara;			//���ò����ַ���
	TCommentType		strComment;			//��ע

	int					nUse;				//1:���ã� 
};

//���Է���
struct SStrategyInstance
{
	StrategyName		strategyName;			//������
	ACCOUNTSTRING		strTraderName;			//�ϴ���
	TInstrumentID		Instruments;			//��Լ	
	int					nRunAfterLoad;			//1:���غ��������У�  0�����غ�����
	int					nRunAfterOffline;		//1:���ߺ���Ȼ���У�  0�����ߺ�����
	int					nOrderActionBeforeStop; //1:ֹͣǰ������      0��ֹͣǰ������
	TParameterType      strPara;				//���ò����ַ���
	TCommentType		strComment;				//��ע
	int					nStart;					//1:���У�����Ӧ�ľ���SUseStrategy��1:���ã�0:ͣ�ã�

	bool				bStartTimer;			//�Ƿ�������ʱ��
	int					nTimerSpan;				//��ʱ��ʱ����
};
#define UPLOAD_STRATEGY_DLL 2
#define UPLOAD_STRATEGY_ZIP 3
#define UPLOAD_INDEX_DLL 2
#define UPLOAD_INDEX_ZIP 3
struct SIndex
{
	INDEXSTRING		IndexName;				//ָ����
	int				nType;
	ACCOUNTSTRING		strTraderName;		//����Ա
	int					nPrivate;			//1:˽�У�2����
	TParameterType      strPara;				//���ò����ַ���
	TCommentType		strComment;			//��ע
};
struct SStrategy2Index//һ�����Զ�Ӧ����ָ����
{
	StrategyName		strategyName;			//������
	INDEXSTRING			IndexName;				//ָ����
};
struct SUseStrategy //ͣ�����ò���;���Է���
{
	int					nUserType;				//1:�����նˣ�2��Ϊ�����ն�
	ACCOUNTSTRING		strTraderName;			//����Ա
	StrategyName		strategyName;			//������
	int					nStatus;				//0:ͣ�ã�1:����
};
struct SDelStrategy //ɾ������;���Է���
{
	int					nUserType;				//1:�����նˣ�2��Ϊ�����ն�
	ACCOUNTSTRING		strTraderName;			//����Ա
	StrategyName		strategyName;			//������	
};
struct LogStrategy
{
	char     logTable[41];   // Ҫ�������ݿ�ı�
	char	 logDateTime[20];// ����ʱ�䣬��ʽ"yyyy-mm-dd HH:MM:SS"
	char      logOperatorID[25];  // ������ID
	char      logTargetorID[25];  // ��������ID
	OperType logAction;      // ��������
	char     logContent[256];// ��������
	bool     logResult;      // �������
};

struct UploadStart 
{
	UPLOADSTRING uploadstring;//����ַ�����һ��XML��ʽ
};
/*XML��ʽ
	<uploadid>ID</uploadid>						//�ϴ�Ψһ��ID		
	<traderid>taderid</traderid>				//����ԱID
	<stratregyname>test1;test2</stratregyname>  //���в��������÷ֺŸ���
	<indexname>in1;in2</indexname>              //����ָ�������÷ֺŸ���

*/
struct UploadEnd 
{
	UPLOADEND uploadend;//����ַ�����uploadstring�����uploadid
	ACCOUNTSTRING		strTraderName;			//����Ա
};
struct QueryCommission
{	
	///�˺�
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};
struct QueryMarginRate
{
	///�˺�
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
};
//////////////////////////////////////////////////////////////////////////

//���Բ�������
enum  StrategyParaType
{
	StrategyIntegerType = 0,
	StrategyDoubleType =1,
	StrategyBooleanType,
	StrategyStringType,
	StrategyDateTimeType,
	StrategyCycleType,
	SubStrategyType
};
enum  StrategyDirection
{
	DirectionIN = 0,
	DirectionOUT =1,
	DirectionINOUT      

};
//��ʾ��ʽ
enum  DisplayType
{
	EditBox,//�ı���
	RadioBox,//��ѡ��
	DropDownBox,//��ѡ��
	BOOLBox,//����
	CYCLEBox,//����
	DirectionBox,//�������� 
	OffsetFlagBox,//��ƽ����
	TimeBox,//ʱ�������
	//DateBox//���������
};
//���Բ���
struct StrategyPara 
{
	char               szTitle[32];                //��������
	char               szName[32];                //��������
	StrategyParaType   eType;                     //���Բ�������
	StrategyDirection  eDirection;                 //���Բ�������
	char               szDefault[128];             // Ĭ��ֵ
	DisplayType        eDisplayType;                //��ʾ��ʽ
	bool               bReadOnly;                    //ֻ��


	bool operator== (const StrategyPara& r) const
	{
		int irlt;

		irlt=strncmp(szName,r.szName,sizeof(szName));
		if(irlt!=0) return false;
		irlt=strncmp(szTitle,r.szTitle,sizeof(szTitle));
		if(irlt!=0) return false;

		irlt=strncmp(szDefault,r.szDefault,sizeof(szDefault));
		if(irlt!=0) return false;

		if(eDirection!=r.eDirection)
			return false;
		if(eType!=r.eType)
			return false;
		if(eDisplayType!=r.eDisplayType)
			return false;
		if(bReadOnly!=r.bReadOnly)
			return false;

		return true;
	}
};
//����ָ������
struct StrategyIndicator 
{
	char               szTitle[32];                //����ָ�꣬Ϊkeyֵ
	char               szName[32];                //ָ������
	char               szPara[256];                //ָ�����
	bool               bOutput;                    //�Ƿ����
	int                nInstrument;           //��ѡ��Լ


	bool operator== (const StrategyIndicator& r) const
	{
		int irlt;

		irlt=strncmp(szName,r.szName,sizeof(szName));
		if(irlt!=0) return false;
		irlt=strncmp(szTitle,r.szTitle,sizeof(szTitle));
		if(irlt!=0) return false;

		irlt=strncmp(szPara,r.szPara,sizeof(szPara));
		if(irlt!=0) return false;
		
		if(nInstrument!=r.nInstrument)
			return false;
		if(bOutput!=r.bOutput)
			return false;

		return true;
	}
};
struct StrategyPrarSubItem 
{
	char               szName[32];                 //����ѡ������
	char               szValue[32];                //����ѡ��ֵ
	bool               bDefault;                   //�Ƿ���Ĭ��ֵ
};

//���Բ��Խ��ָ��
struct TestIndicator 
{
	//��������õ����ֶ�
	double         NetProfit;            //������
	double         AvgProfit;            //ƽ��ӯ��
	//double         PositionTimeRatio;    //�ֲ�ʱ�����
	double         TotalProfit;          //��ӯ��
	double         TotalLoss;            //�ܿ���
	double         AvgLoss;              //ƽ������
	double         InitialProfit;;       //��ʼȨ��
	double         MiniDynamicProfit;    //��СȨ��
	double         MaxDynamicProfit;     //���Ȩ��
	double         GrossProfit;          //ë����
	double         AvgNetProfit;         //ƽ������
	double         TotalCommission;      //��������
	double         MaxUsedFund;          //���ʹ���ʽ�
	double         FinishProfit;         //������Ȩ��
	int            OpenOrderCnt;         //δ�ɽ�����
	double         WinRate;              //ʤ��
	double         YieldRate;            //������
	double         EffectiveYield;       //��Ч������
	double         Annualyield;          //��������
	double         MonthlyAvgProfit;     //��ƽ������
	double         AnnualProfit;         //�껯����
	int            MaxContinuousProfit;  //�������ӯ������
	int            MaxContinuousLoss;    //��������������
	int            TotalTradeCnt;        //�ܽ��״���
	int            MaxPosition;          //���ֲ���
	double         ConfidenceLevel;      //���Ŷ�
	int            TotalOrderCnt;        //�ܽ���ί����
	//int            TotalPositionTime;    //�ֲܳ�ʱ��
	double         MaxProfit;            //���ӯ��
	double         MaxLoss;              //������
	char		   szStartTime[24];		// ���Կ�ʼʱ��yyyy-mm-dd hh:mm:ss
	char		   szEndTime[24];		// ���Խ���ʱ��yyyy-mm-dd hh:mm:ss
	char		   szCreatTime[24];		// ���Դ���ʱ��yyyy-mm-dd hh:mm:ss
	char		   szModifyTime[24];	// �����޸�ʱ��yyyy-mm-dd hh:mm:ss
	char		   szQuoteSource[_MAX_PATH];	// ������Դ

	//���������ʹ�õ����ֶ�

	int           TotalTradingTime;       //�ܽ�������
	int           TotalProfitCnt;         //��ӯ������
	int           TotalLossCnt;           //�ܿ������
	bool          bIsProfit;              //�Ƿ�ӯ��









};

struct TestSimpleData
{
	char			szName[64];				// ����������
	char			szDate[16];				// ��������
	char			szTime[16];				// ����ʱ��
	double         NetProfit;				//������
	int            TotalTradeCnt;			//�ܽ��״���
};



//redis ͨ������
#define ORDERINPUT_CHANNEL_NAME "OrderInput"
#define ORDERACTION_CHANNEL_NAME "OrderAction"
#define HEARTBEAT_CHANNEL_NAME "HeartBeat"


///������Ϣ��չ ֻΪ���ز��Ե���ʹ��
struct PlatformStru_DepthMarketData_EX
{
     PlatformStru_DepthMarketData_EX()
     {
         clear();
     }
     PlatformStru_DepthMarketData_EX(const CThostFtdcDepthMarketDataField& r)
     {
         clear();
 
         strncpy(TradingDay, r.TradingDay,sizeof(TradingDay)-1);
         strncpy(InstrumentID, r.InstrumentID,sizeof(InstrumentID)-1);
         strncpy(ExchangeID, r.ExchangeID,sizeof(ExchangeID)-1);
         LastPrice = r.LastPrice;
         PreSettlementPrice = r.PreSettlementPrice;
         PreClosePrice = r.PreClosePrice;
         PreOpenInterest = r.PreOpenInterest;
         OpenPrice = r.OpenPrice;
         HighestPrice = r.HighestPrice;
         LowestPrice = r.LowestPrice;
         Volume = r.Volume;
         Turnover = r.Turnover;
         OpenInterest = r.OpenInterest;
         ClosePrice = r.ClosePrice;
         SettlementPrice = r.SettlementPrice;
         UpperLimitPrice = r.UpperLimitPrice;
         LowerLimitPrice = r.LowerLimitPrice;
         strcpy(UpdateTime, r.UpdateTime);
         UpdateMillisec = r.UpdateMillisec;
         BidPrice1 = r.BidPrice1;
         BidVolume1 = r.BidVolume1;
         AskPrice1 = r.AskPrice1;
         AskVolume1 = r.AskVolume1;
         AveragePrice = r.AveragePrice;
     }
     void clear(void)
     {
         memset(this,0,sizeof(*this));
 
         LastPrice=util::GetDoubleInvalidValue();
         PreSettlementPrice=util::GetDoubleInvalidValue();
         PreClosePrice=util::GetDoubleInvalidValue();
         PreOpenInterest=util::GetDoubleInvalidValue();
         OpenPrice=util::GetDoubleInvalidValue();
         HighestPrice=util::GetDoubleInvalidValue();
         LowestPrice=util::GetDoubleInvalidValue();
         Volume=util::GetIntInvalidValue();
         Turnover=util::GetDoubleInvalidValue();
         OpenInterest=util::GetDoubleInvalidValue();
         ClosePrice=util::GetDoubleInvalidValue();
         SettlementPrice=util::GetDoubleInvalidValue();
         UpperLimitPrice=util::GetDoubleInvalidValue();
         LowerLimitPrice=util::GetDoubleInvalidValue();
         BidPrice1=util::GetDoubleInvalidValue();
         BidVolume1=util::GetIntInvalidValue();
         AskPrice1=util::GetDoubleInvalidValue();
         AskVolume1=util::GetIntInvalidValue();
         AveragePrice=util::GetDoubleInvalidValue();
     }
 
    ///������
    TThostFtdcDateType	TradingDay;
    ///��Լ����
    InstrumentIDType	InstrumentID;
    ///����������
    TThostFtdcExchangeIDType	ExchangeID;
    ///���¼�
    TThostFtdcPriceType	LastPrice;
    ///�ϴν����
    TThostFtdcPriceType	PreSettlementPrice;
    ///������
    TThostFtdcPriceType	PreClosePrice;
    ///��ֲ���
    TThostFtdcLargeVolumeType	PreOpenInterest;
    ///����
    TThostFtdcPriceType	OpenPrice;
    ///��߼�
    TThostFtdcPriceType	HighestPrice;
    ///��ͼ�
    TThostFtdcPriceType	LowestPrice;
    ///����
    TThostFtdcVolumeType	Volume;
    ///�ɽ����
    TThostFtdcMoneyType	Turnover;
    ///�ֲ���
    TThostFtdcLargeVolumeType	OpenInterest;
    ///������
    TThostFtdcPriceType	ClosePrice;
    ///���ν����
    TThostFtdcPriceType	SettlementPrice;
    ///��ͣ���
    TThostFtdcPriceType	UpperLimitPrice;
    ///��ͣ���
    TThostFtdcPriceType	LowerLimitPrice;
    ///����޸�ʱ��
    TThostFtdcTimeType	UpdateTime;
    ///����޸ĺ���
    TThostFtdcMillisecType	UpdateMillisec;
    ///�����һ
    TThostFtdcPriceType	BidPrice1;
    ///������һ
    TThostFtdcVolumeType	BidVolume1;
    ///������һ
    TThostFtdcPriceType	AskPrice1;
    ///������һ
    TThostFtdcVolumeType	AskVolume1;
    ///���վ���
    TThostFtdcPriceType	AveragePrice;
};

enum  KLineDataCycle
{
	Minute_1 = 1,
	Minute_3 =3,
	Minute_5=5,
	Minute_10=10,
	Minute_15=15,
	Minute_30=30,
	Minute_60=60,
	Day_1,
	Week_1,
	Month_1,
};

//K��
struct KLineData
{
	KLineDataCycle	eCycle;           //K������
	char			szDate[16];			   // ����yyyymmdd
	char			szUpdateTime[9];			  // ������ʱ�����K��hh:mm:ss����k �� yyyymm  ��K�� ��K��yyyy
	_InstrumentID	InstrumentID;     //��Լ����
	double          OpenPrice;			  //���̼�	
	double	        HighestPrice;         ///��߼�	
	double	        LowestPrice;          ///��ͼ�
    double	        ClosePrice;           /// ���̼�
	int         	IncreaseVolume;              ///�ɽ�������
	int         	TotalVolume;              ///�ɽ�������
	double          Turnover;			//�ɽ��������
	int            OpenInterest;				//�ֲ�������
	double         SettlementPrice;                //�����

};
///��ʱ��
struct TimeLineData
{
	char			szDate[16];			   // ����
	char			szTime[9];			  // ʱ��
	_InstrumentID	InstrumentID;     //��Լ����
	double	        ClosePrice;           /// ���̼�
	int         	Volume;              ///�ɽ�������
	double          Turnover;			//�ɽ��������
	int             OpenInterest;		//�ֲ�������

};
enum EnumPhrase
{
	PHRASE_1MIN		= 1,
	PHRASE_5MIN		= 5,
	PHRASE_15MIN	= 15,
	PHRASE_30MIN	= 30,
	PHRASE_60MIN	= 60,
	PHRASE_DAY		= 24*60,
	PHRASE_WEEK		= 7*24*60,
	PHRASE_MONTH	= 31*24*60,
	PHRASE_USERDEFINE = 0
};
struct KLineBase   //k�����ݽṹ��1���� �� ���߶���������ݽṹ
{
	time_t dwTime;						//ʱ�䣬��1900��1��1�տ�ʼ������
	double fOpenPrice;					// ���̼�
	double fHighPrice;					// ��߼�
	double fLowPrice;					// ��ͼ�
	double fClosePrice;					// ���̼�
	double fTurnover;					//�ɽ����
	DWORD dwVolume;						//�ɽ���
	DWORD dwOpenInterest;				//�ֲ���
	KLineBase& operator=(const KLineBase& other)
	{
		dwTime			= other.dwTime;

		fOpenPrice      = other.fOpenPrice;
		fHighPrice		= other.fHighPrice;		
		fLowPrice		= other.fLowPrice;			
		fClosePrice		= other.fClosePrice;				
		fTurnover		= other.fTurnover;		
		dwVolume		= other.dwVolume;					
		dwOpenInterest	= other.dwOpenInterest;				
		//dJieSuan		= other.dJieSuan;      
		return *this;
	}
	bool operator<(const KLineBase&	src) const
	{//�Ӵ�С����
		return dwTime > src.dwTime;
	}
} ; 

struct TLineBase //��ʱ���ݽṹ
{
	time_t dwTime;						//ʱ�䣬��1900��1��1�տ�ʼ������
	double fLastPrice;					// ���̼�
	double fTurnover;					//�ɽ����
	DWORD dwVolume;						//�ɽ���
	DWORD dwOpenInterest;				//�ֲ���
	TLineBase& operator=(const TLineBase& other)
	{
		dwTime			= other.dwTime;

		fLastPrice		= other.fLastPrice;				
		fTurnover		= other.fTurnover;		
		dwVolume		= other.dwVolume;					
		dwOpenInterest	= other.dwOpenInterest;				
		return *this;
	}
	bool operator<(const TLineBase&	src) const
	{//�Ӵ�С����
		return dwTime > src.dwTime;
	}
};	
//��ѯK�ߡ���ʱ�߽ṹ
struct KLineQuery
{	
	time_t			        szStartTime;		// 
	time_t			        szEndTime;		//
	_InstrumentID	        InstrumentID;     //��Լ����
	EnumPhrase				nCycle;           //K������	
	int                     nDataType;         //1-K�����ݣ�2-����������, 3-��ѯ�����
};

struct QueryPara 
{
	char szBuffer[4000]; 
	//std::vector<std::string> vecSql;
	//KLineQuery query;
	int  nSocket;
	int  nSeq;
};
struct SimpleDepthMarketData
{

	///������
	TThostFtdcDateType	TradingDay;
	///��Լ����
	InstrumentIDType	InstrumentID;
	///���¼�
	TThostFtdcPriceType	LastPrice;
	///����
	TThostFtdcPriceType	OpenPrice;
	///��߼�
	TThostFtdcPriceType	HighestPrice;
	///��ͼ�
	TThostFtdcPriceType	LowestPrice;
	///������
	TThostFtdcPriceType	ClosePrice;
	///�ɽ����
	TThostFtdcMoneyType	Turnover;
	///�ֲ���
	TThostFtdcLargeVolumeType	OpenInterest;
	///�ϴν����
	TThostFtdcPriceType	PreSettlementPrice;
	///����
	TThostFtdcVolumeType	Volume;
	///����޸�ʱ��
	TThostFtdcTimeType	UpdateTime;
	///�����һ
	TThostFtdcPriceType	BidPrice1;
	///������һ
	TThostFtdcVolumeType	BidVolume1;
	///������һ
	TThostFtdcPriceType	AskPrice1;
	///������һ
	TThostFtdcVolumeType	AskVolume1;

};
struct SettlementPriceCalc
{
	///������
	TThostFtdcDateType	TradingDay;
	///��Լ����
	InstrumentIDType	InstrumentID;
	///�����
	TThostFtdcPriceType	SettlementPrice;
	///�۸�仯
	TThostFtdcPriceType	PriceChange;
	///��׼��
	TThostFtdcPriceType	StandardDeviation;

};
struct Volatility 
{
	///������
	TThostFtdcDateType	TradingDay;
	///��Լ����
	InstrumentIDType	InstrumentID;
	///20�첨����
	TThostFtdcPriceType	VolatilityTwenty;
	///60�첨����
	TThostFtdcPriceType	VolatilitySixty;
};

struct SettlementPrice
{
	///������
	int	TradingDay;
	///�����
	double	Price;
};

#pragma pack(pop)