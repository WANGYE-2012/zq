#pragma once

#define Cmd_MA_UnLogon_Rsp			0x100000	//δ��¼���󷵻�
#define Cmd_MA_Login_Req			0x100001	//��¼����
#define Cmd_MA_Login_Rsp			0x100002	//��¼����
#define Cmd_MA_Connected_Rsp		0x100003	//���ӳɹ�����
//�˺���ؽӿ�
#define Cmd_MA_Account_Min			0x100003
#define Cmd_MA_Account_Max			0x1FFFFF
#define Cmd_MA_AddAccount_Req		0x100003	//����˺�����
#define Cmd_MA_AddAccount_Rsp		0x100004	//����˺ŷ���

#define Cmd_MA_DelAccount_Req		0x100005	//ɾ���˺�����
#define Cmd_MA_DelAccount_Rsp		0x100006	//ɾ���˺ŷ���

#define Cmd_MA_ModifyPwd_Req		0x100007	//�޸���������
#define Cmd_MA_ModifyPwd_Rsp		0x100008	//�޸����뷵��
#define Cmd_MA_ModifyAccount_Req	0x100009	//�޸��˻�������Ϣ����
#define Cmd_MA_ModifyAccount_Rsp	0x10000A	//�޸��˻�������Ϣ����

#define Cmd_MA_QryALLAccount_Req		0x10000B	//��ѯ�����˺�����
#define Cmd_MA_QryAccountByOwner_Req	0x10000C	//�˺Ų�ѯ���󣬲���Ϊ�������˺�
#define Cmd_MA_QryAccount_Rsp		0x10000D	//�˺Ų�ѯ����

#define Cmd_MA_ForceCloseClient_Rsp	0x10000E	//�˺ű�ͣ��ʱ��ǿ�ƹرտͻ���
#define Cmd_MA_DelAccountCaseCade_Rsp		0x10000F	//����ɾ���˺ŷ���

//IDCard��ؽӿ�
#define Cmd_MA_IDCard_Min			0x200000
#define Cmd_MA_IDCard_Max			0x2FFFFF
#define Cmd_MA_AddIDCard_Req		0x200001	//���IDCard��Ϣ����
#define Cmd_MA_AddIDCard_Rsp		0x200002	//���IDCard��Ϣ����

#define Cmd_MA_DelIDCard_Req		0x200003	//ɾ��IDCard��Ϣ����
#define Cmd_MA_DelIDCard_Rsp		0x200004	//ɾ��IDCard��Ϣ����

#define Cmd_MA_ModifyIDCard_Req		0x200005	//�޸�IDCard��Ϣ����
#define Cmd_MA_ModifyIDCard_Rsp		0x200006	//�޸�IDCard��Ϣ����

#define Cmd_MA_QryIDCard_Req		0x200007	//��ѯIDCard��Ϣ����
#define Cmd_MA_QryIDCard_Rsp		0x200008	//��ѯIDCard��Ϣ����

//��������ؽӿ�
#define Cmd_MA_Server_Min			0x300000
#define Cmd_MA_Server_Max			0x3FFFFF
#define Cmd_MA_AddServerGroup_Req		0x300001	//��ӷ���������Ϣ����
#define Cmd_MA_AddServerGroup_Rsp		0x300002	//��ӷ���������Ϣ����

#define Cmd_MA_DelServerGroup_Req		0x300003	//ɾ������������Ϣ����
#define Cmd_MA_DelServerGroup_Rsp		0x300004	//ɾ������������Ϣ����

#define Cmd_MA_ModifyServerGroup_Req	0x300005	//�޸ķ���������Ϣ����
#define Cmd_MA_ModifyServerGroup_Rsp	0x300006	//�޸ķ���������Ϣ����

#define Cmd_MA_QryServerGroup_Req		0x300007	//��ѯ����������Ϣ����
#define Cmd_MA_QryServerGroup_Rsp		0x300008	//��ѯ����������Ϣ����

#define Cmd_MA_AddServerAddr_Req		0x300009	//��ӷ�������ַ��Ϣ����
#define Cmd_MA_AddServerAddr_Rsp		0x30000A	//��ӷ�������ַϢ����

#define Cmd_MA_DelServerAddr_Req		0x30000B	//ɾ����������ַ��Ϣ����
#define Cmd_MA_DelServerAddr_Rsp		0x30000C	//ɾ����������ַ��Ϣ����

#define Cmd_MA_ModifyServerAddr_Req		0x30000D	//�޸ķ�������ַ��Ϣ����
#define Cmd_MA_ModifyServerAddr_Rsp		0x30000E	//�޸ķ�������ַ��Ϣ����

#define Cmd_MA_QryServerAddr_Req		0x30000F	//��ѯ��������ַ��Ϣ����
#define Cmd_MA_QryServerAddr_Rsp		0x300010	//��ѯ��������ַ��Ϣ����

//���͹�˾��ؽӿ�
#define Cmd_MA_Broker_Min			0x400000
#define Cmd_MA_Broker_Max			0x4FFFFF
#define Cmd_MA_AddBroker_Req		0x400001	//��Ӿ��͹�˾����
#define Cmd_MA_AddBroker_Rsp		0x400002	//��Ӿ��͹�˾����

#define Cmd_MA_DelBroker_Req		0x400003	//ɾ�����͹�˾����
#define Cmd_MA_DelBroker_Rsp		0x400004	//ɾ�����͹�˾����

#define Cmd_MA_ModifyBroker_Req		0x400005	//�޸ľ��͹�˾��Ϣ����
#define Cmd_MA_ModifyBroker_Rsp		0x400006	//�޸ľ��͹�˾��Ϣ����

#define Cmd_MA_QryBroker_Req		0x400007	//��ѯ���͹�˾��Ϣ���󣬲���Ϊ������ID
#define Cmd_MA_QryBroker_Rsp		0x400008	//��ѯ���͹�˾��Ϣ����

#define Cmd_MA_QryUsableBroker_Req		0x400009	//��ѯ���õľ��͹�˾��Ϣ���󣬲���Ϊǰ̨�˻�ID
#define Cmd_MA_QryUsableBroker_Rsp		0x40000A	//���ؿ��õľ��͹�˾��Ϣ����

//�����˺���ؽӿ�
#define Cmd_MA_TradeAccount_Min			0x500000
#define Cmd_MA_TradeAccount_Max			0x5FFFFF
#define Cmd_MA_AddTradeAccount_Req		0x500001	//��ӽ����˺�����
#define Cmd_MA_AddTradeAccount_Rsp		0x500002	//��ӽ����˺ŷ���

#define Cmd_MA_DelTradeAccount_Req		0x500003	//ɾ�������˺�����
#define Cmd_MA_DelTradeAccount_Rsp		0x500004	//ɾ�������˺ŷ���

#define Cmd_MA_ModifyTradeAccount_Req	0x500005	//�޸Ľ����˺�����
#define Cmd_MA_ModifyTradeAccount_Rsp	0x500006	//�޸Ľ����˺ŷ���

#define Cmd_MA_QryTradeAccount_Req		0x500007	//��ѯ�����˺�����
#define Cmd_MA_QryTradeAccount_Rsp		0x500008	//��ѯ�����˺ŷ���

#define Cmd_MA_SaveTradeAccount_Req		0x500009	//�����˺�����
#define Cmd_MA_SaveTradeAccount_Rsp		0x50000A	//�����˺ŷ���

//Ȩ��ģ����ؽӿ�
#define Cmd_MA_PermissionModule_Min			0x600000
#define Cmd_MA_PermissionModule_Max			0x699999
#define Cmd_MA_AddPermissionModule_Req		0x600001	//���Ȩ��ģ������
#define Cmd_MA_AddPermissionModule_Rsp		0x600002	//���Ȩ��ģ�鷵��

#define Cmd_MA_DelPermissionModule_Req		0x600003	//ɾ��Ȩ��ģ������
#define Cmd_MA_DelPermissionModule_Rsp		0x600004	//ɾ��Ȩ��ģ�鷵��

#define Cmd_MA_ModifyPermissionModule_Req	0x600005	//�޸�Ȩ��ģ������
#define Cmd_MA_ModifyPermissionModule_Rsp	0x600006	//�޸�Ȩ��ģ�鷵��

#define Cmd_MA_QryPermissionModule_Req		0x600007	//��ѯȨ��ģ������
#define Cmd_MA_QryPermissionModule_Rsp		0x600008	//��ѯȨ��ģ�鷵��

//Ȩ����ؽӿ�
#define Cmd_MA_Permission_Min			0x700000
#define Cmd_MA_Permission_Max			0x7FFFFF
#define Cmd_MA_AddPermission_Req		0x700001	//���Ȩ������
#define Cmd_MA_AddPermission_Rsp		0x700002	//���Ȩ�޷���

#define Cmd_MA_DelPermission_Req		0x700003	//ɾ��Ȩ������
#define Cmd_MA_DelPermission_Rsp		0x700004	//ɾ��Ȩ�޷���

#define Cmd_MA_ModifyPermission_Req		0x700005	//�޸�Ȩ������
#define Cmd_MA_ModifyPermission_Rsp		0x700006	//�޸�Ȩ�޷���

#define Cmd_MA_QryPermissionByOwner_Req		0x700007	//��ѯȨ�����󣬲���Ϊ������ID
#define Cmd_MA_QryPermissionByOwner_Rsp		0x700008	//��ѯȨ�޷��أ�����Ϊ������ID

#define Cmd_MA_QryPermissionByID_Req		0x700009	//��ѯȨ�����󣬲���ΪȨ��ID
#define Cmd_MA_QryPermissionByID_Rsp		0x70000A	//��ѯȨ�޷��أ�����ΪȨ��ID

//��Ϣ��ؽӿ�
#define Cmd_MA_Message_Min				0x800000
#define Cmd_MA_Message_Max				0x8FFFFF
#define Cmd_MA_AddMessage_Req			0x800001	//�����Ϣ����
#define Cmd_MA_AddMessage_Rsp			0x800002	//�����Ϣ��Ӧ
#define Cmd_MA_QryMessage_Req			0x800003	//��ѯ��Ϣ����
#define Cmd_MA_QryMessage_Rsp			0x800004	//��ѯ��Ϣ����
#define Cmd_MA_QryTargetAccount_Req		0x800005	//��ѯ��Ϣ��Ŀ���˻��б�
#define Cmd_MA_QryTargetAccount_Rsp		0x800006	//������Ϣ��Ŀ���˻��б�
#define Cmd_MA_QryUnReadMessage_Req		0x800007	//��ѯδ�Ķ�����Ϣ
#define Cmd_MA_SendUnReadMessage_Rsp	0x800008	//����δ�Ķ�����Ϣ
#define Cmd_MA_QryWaitForSendAccount_Req	0x800009	//��ѯδ�ʹ��˻��б�
#define Cmd_MA_QryWaitForSendAccount_Rsp	0x80000A	//����δ�ʹ��˻��б�

#define OPERATOR_SUCCESS	1		//����ɹ�
#define OPERATOR_FAILED		0		//����ʧ��