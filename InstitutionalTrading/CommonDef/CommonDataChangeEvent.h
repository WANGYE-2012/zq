#pragma once
#include "string.h"
//�������ͷ�ļ��ж�������������ݱ仯���¼��ṹ����¼�ID
#define  DATA_CHANGE_EVENT_INSTRUMENT_LENGTH 21

#define DATA_CHANGE_ENENT_DATA_LEN  1008
enum eAskType
{
    AskType_Broker,              //���͹�˾
    AskType_ServerGroup,         //��������
    AskType_ServerAddress,       //���������ַ 
	AskType_Account,             //ί�н����ʺ�
	AskType_User,                //�û�
    AskType_UserAndTradeAccount, //�û���ί�н����˺Ź�ϵ
	AskType_Orgnazation,         //��֯����
	AskType_FinacialProduct,     //��Ʋ�Ʒ
    AskType_TradeStrategy,       //���ײ���
	AskType_Login,				  //�û���¼ע����ADDΪ��¼��deleteΪע����
    AskType_Role,                //��ɫ
    AskType_QueryPrivilege,      //��ѯȨ��
    AskType_OrgAndStrategy,      //��֯�����뽻�ײ��ԵĹ�ϵ
    AskType_RoleAndPrivelege,    //��ɫ��Ȩ�޵Ĺ�ϵ
    AskType_TraderAndProduct,    //����Ա����Ʋ�Ʒ�Ĺ�ϵ
    AskType_UserAndRole,         //�û����ɫ�Ĺ�ϵ
    AskType_UserAndOrg,          //�û�����֯�����Ĺ�ϵ
    AskType_BrokerAndServerGroup,//���͹�˾���������Ĺ�ϵ
    AskType_BrokerAndTradeAccount,//���͹�˾��ί�н����˺ŵĹ�ϵ
    AskType_BrokerAndServerGroupAddress,//���͹�˾����������ַ�Ĺ�ϵ
	AskType_UserLoginCheck,  //�û��Ƿ��ڵ�½��
};

enum eEventType
{
	EventTypeInvalideType = 0, //��Ч���¼�����
	
	EventTypeUserDataReady,    //֪ͨ�û�����׼������
	EventTypeQuto,			   //֪ͨ�µ�һ������
	EventTypeTrade,			   //�ɽ��仯 
	EventTypeFund,			   //�ʽ�仯 
	EventTypePosition,		   //�ֱֲ仯

	EventTypeNeedAllLoginTrade  ,        //֪ͨ������Ҫ��¼����ί�н����˺ŵ�����IP
	EventTypeNeedSingleLoginTrade  ,     //֪ͨ������Ҫ��¼���в�����ί�н����˺�
	EventTypeNeedAllLoginSettlement  ,   //֪ͨ������Ҫ��¼����ί�н����˺ŵ�����IP
    EventTypeNeedGetInstrument,          //֪ͨ���̴ӽ������ݹ���ģ���ȡ��Լ
	EventTypeNeedQryInstrument,          //֪ͨ������Ҫ��ѯ��Լ
	EventTypeNeedQryCommissionAndMargin, //֪ͨ�������ѯ������
	EventTypeNeedQryCommission,          //֪ͨ�������ѯ������
	EventTypeNeedQryMargin,				 //֪ͨ�������ѯ������
	EventTypeNeedQryOrder,				 //֪ͨ�������ѯ����
	EventTypeNeedQryTrader,              //֪ͨ�������ѯ�ɽ�
	EventTypeNeedQryFund,				 //֪ͨ�������ѯ����
	EventTypeNeedQryPosition,            //֪ͨ�������ѯ�ֲ�
	EventTypeNeedQryPositionDetail,      //֪ͨ�������ѯ�ֲ���ϸ
	EventTypeNeedLogoffSettlement,       //֪ͨ���̰Ѳ����ڵ�ί���˺ŵ�ctp���ӶϿ�
	EventTypeOrderRtn,		    //֪ͨ����ִ��,�µ�һ���ɽ�
	EventTypeTradeRtn,		    //֪ͨ����ִ��,�µ�һ������
	EventTypeRspOrderInsert,	//֪ͨ����ִ�У�����¼��Ӧ��
	EventTypeErrOrderInsert,	//֪ͨ����ִ�У�����¼�����
	EventTypeRspOrderAction,	//֪ͨ����ִ�У���������Ӧ��
	EventTypeErrOrderAction,	//֪ͨ����ִ�У�������������	
	
	EventTypeUserLogin,        //�û���½�¼� LPARAMΪUserID
	EventTypeUserLogoff,       //�û�ע���¼� LPARAMΪUserID
	EventTypeUserDiscon,       //�û��Ͽ��¼� LPARAMΪUserID

	EventTypeStartSettlement,  //��ʼ����
	EventTypeEndSettlement,    //��������
	EventTypeStartInit,        //��ʼ��ʼ��
	EventTypeInitFinish,        //��ʼ�����
	EventTypeUserStop,         //ͣ��һ���û�


    //�������ݸı��Ĺ㲥�¼�	
    EventTypeAccountAdd ,      //����һ��ί�н����˺�  LPARAMΪAccountID
	EventTypeAccountDelete,    //ɾ��һ��ί�н����˺�
	EventTypeAccountUpdate,    //����һ��ί�н����˺�
    EventTypeUserAdd ,         //����һ���û�
	EventTypeUserDelete,       //ɾ��һ���û�
	EventTypeUserUpdate,       //����һ���û�
    EventTypeUserLimitAndVerifyUpdate,       //����һ���û����µ����ֹ����Ȩ��
    EventTypeUserAndOrgRelation,        //�û�����֯������ϵ�ı�
    EventTypeUserAndRoleRelation,       //�û��ͽ�ɫ��ϵ�ı�
    EventTypeUserAndTradeAcctRelation,  //�û���ί�н����˺Ź�ϵ�ı�
    EventTypeRoleAdd,                   //����һ����ɫ
    EventTypeRoleDelete,                //ɾ��һ����ɫ
    EventTypeRoleUpdate,                //����һ����ɫ
    EventTypeRoleAndPrivelegeRelation,  //��ɫ��Ȩ�޹�ϵ�ı�
    EventTypeOrgAdd,                   //����һ����֯����
    EventTypeOrgDelete,                //ɾ��һ����֯����
    EventTypeOrgUpdate,                //����һ����֯����
    EventTypeTradeStrategyAdd,                   //����һ�����ײ���
    EventTypeTradeStrategyDelete,                //ɾ��һ�����ײ���
    EventTypeTradeStrategyUpdate,                //����һ�����ײ���
    EventTypeTradeStrategyAndOrgRelation,  //���ײ��Ժ���֯������ϵ�ı�
    EventTypeFinicialProductAdd,                   //����һ����Ʋ�Ʒ
    EventTypeFinicialProductDelete,                //ɾ��һ����Ʋ�Ʒ
    EventTypeFinicialProductUpdate,                //����һ����Ʋ�Ʒ
    EventTypeFinicialProductAndUserRelation,  //��Ʋ�Ʒ���û���ϵ�ı�
    EventTypeBrokerInfoAdd,                   //����һ�����͹�˾
    EventTypeBrokerInfoDelete,                //ɾ��һ�����͹�˾
    EventTypeBrokerInfoUpdate,                //����һ�����͹�˾
    EventTypeServerGroupAdd,                   //����һ����������
    EventTypeServerGroupDelete,                //ɾ��һ����������
    EventTypeServerGroupUpdate,                //����һ����������
    EventTypeServerAddressAdd,                   //����һ�����������ַ
    EventTypeServerAddressDelete,                //ɾ��һ�����������ַ
    EventTypeServerAddressUpdate,                //����һ�����������ַ
    EventTypeBrokerInfoQuotUpdate,                //����һ�����龭�͹�˾
    EventTypeServerAddressQuotAdd,                   //����һ��������������ַ
    EventTypeServerAddressQuotDelete,                //ɾ��һ��������������ַ
    EventTypeServerAddressQuotUpdate,                //����һ��������������ַ
	EventTypeErrorCome,                               //һ�����󵽴�

	EventTypeSlaverLoginedMaster,                   //�ӷ�������¼����������
	
};


//���ݲ�����ѯʱ��ѯ�ʵ����ݲ�������
enum eAskOperationType
{
	AskOperationTypeAdd = 0, //����һ����������
	AskOperationTypeDelete,  //ɾ��һ����������
	AskOperationTypeUpdate,  //����һ����������
	AskOperationTypeQuery	 //��ѯ����
};

typedef struct _sDataChangeEvent
{
	eEventType       meEventType;           //�¼�����
	int              mhEvent;               //�¼�����ͬ�����
	int              mnDataID;             //�¼�����������ID
	//char            msInstrumentName[DATA_CHANGE_EVENT_INSTRUMENT_LENGTH];  //��ѡ���Լ��
	int             mnValidateLen;           //��Ч�����ݳ���
	unsigned char   marrDataByte[DATA_CHANGE_ENENT_DATA_LEN];       //��Ч�����ݻ���
public:
	_sDataChangeEvent()
	{
		memset(this,0,sizeof(*this));
		meEventType = EventTypeInvalideType;
		mnValidateLen = 0;
	}
	_sDataChangeEvent(eEventType nType,int nId)
	{
		memset(this,0,sizeof(*this));
		mnValidateLen = 0;
		meEventType = nType;
		mnDataID = nId;
		mhEvent = 0;
	}
	virtual _sDataChangeEvent& operator=(const _sDataChangeEvent& t1) //�������أ�
	{ 	
		
		if(this == &t1)
			return *this;

		//this->meDataChangeType = t1.meDataChangeType;
		this->meEventType = t1.meEventType;
		this->mnDataID = t1.mnDataID;
		this->mhEvent = t1.mhEvent;
		//memcpy(msInstrumentName,t1.msInstrumentName,DATA_CHANGE_EVENT_INSTRUMENT_LENGTH);
		this->mnValidateLen = t1.mnValidateLen;
		memcpy(marrDataByte,t1.marrDataByte,DATA_CHANGE_ENENT_DATA_LEN);
		return *this;
	} 
	bool AddBufer(void* lpBuf,const int nLen)
	{
		if(DATA_CHANGE_ENENT_DATA_LEN - mnValidateLen < nLen || lpBuf == NULL || nLen == 0)
			return false;
		memcpy(marrDataByte+mnValidateLen,lpBuf,nLen);
		mnValidateLen += nLen;
		return true;

	}
}Stru_NotifyEvent;




