#pragma once

//������ΪDWORD���ͣ����У���λ��ʾ�����ַ��࣬��λ��ʾ�����ֱ��

#define CMDID_HEARTBEAT				0x000001	//��������
#define CMDID_HEARTBEATRSP			0x000002	//������

//��¼���������
#define Cmd_RM_Login_Min			0x10000
#define Cmd_RM_Login_Max			0x1FFFF
#define Cmd_RM_Login_Req			0x10001	//��¼����
#define Cmd_RM_Login_Rsp			0x10002	//��¼����
#define Cmd_RM_ForceCloseClient_Push	0x10004	//�˺ű�ͣ��ʱ��ǿ�ƹرտͻ���

//����Ա/���Ա�˺���ؽӿ�
#define Cmd_RM_Account_Min			0x20000
#define Cmd_RM_Account_Max			0x2FFFF
#define Cmd_RM_AddAccount_Req		0x20001	//����˺�����
#define Cmd_RM_AddAccount_Rsp		0x20002	//����˺ŷ���

#define Cmd_RM_DelAccount_Req		0x20003	//ɾ���˺�����
#define Cmd_RM_DelAccount_Rsp		0x20004	//ɾ���˺ŷ���

#define Cmd_RM_ModifyPwd_Req		0x20005	//�޸���������
#define Cmd_RM_ModifyPwd_Rsp		0x20006	//�޸����뷵��
#define Cmd_RM_ModifyAccount_Req	0x20007	//�޸��˻�������Ϣ����
#define Cmd_RM_ModifyAccount_Rsp	0x20008	//�޸��˻�������Ϣ����

#define Cmd_RM_QryAccountByOrgID_Req	0x20009	//��ѯĳ�ʹ���֯���������˺����󣬲���Ϊ�ʹ���֯ID
#define Cmd_RM_QryAccountByOrgID_Rsp	0x2000A	//�˺Ų�ѯ����

//ί�н����˺���ؽӿ�
#define Cmd_RM_TradeAccount_Min			0x30000
#define Cmd_RM_TradeAccount_Max			0x3FFFF
#define Cmd_RM_AddTradeAccount_Req		0x30001	//���ί�н����˺�����
#define Cmd_RM_AddTradeAccount_Rsp		0x30002	//���ί�н����˺ŷ���

#define Cmd_RM_DelTradeAccount_Req		0x30003	//ɾ��ί�н����˺�����
#define Cmd_RM_DelTradeAccount_Rsp		0x30004	//ɾ��ί�н����˺ŷ���

#define Cmd_RM_ModifyTradeAccount_Req	0x30005	//�޸�ί�н����˻�������Ϣ����
#define Cmd_RM_ModifyTradeAccount_Rsp	0x30006	//�޸�ί�н����˻�������Ϣ����

#define Cmd_RM_QryTradeAccountByOrgID_Req		0x30007	//��ѯί�н����˻�������Ϣ���󣬲���Ϊ�ʹ���֯ID
#define Cmd_RM_QryTradeAccountByOrgID_Rsp		0x30008	//��ѯί�н����˻�������Ϣ����

//����Ա��ؽӿ�
#define Cmd_RM_Trader_Min			0x40000
#define Cmd_RM_Trader_Max			0x4FFFF
#define Cmd_RM_AddTrader_Req		0x40001	//���Trader��Ϣ����
#define Cmd_RM_AddTrader_Rsp		0x40002	//���Trader��Ϣ����

#define Cmd_RM_DelTrader_Req		0x40003	//ɾ��Trader��Ϣ����
#define Cmd_RM_DelTrader_Rsp		0x40004	//ɾ��Trader��Ϣ����

#define Cmd_RM_ModifyTrader_Req		0x40005	//�޸�Trader��Ϣ����
#define Cmd_RM_ModifyTrader_Rsp		0x40006	//�޸�Trader��Ϣ����

#define Cmd_RM_QryTraderByOrgID_Req		0x40007	//��ѯTrader��Ϣ���󣬲���Ϊ�ʹ���֯ID
#define Cmd_RM_QryTraderByOrgID_Rsp		0x40008	//��ѯTrader��Ϣ����

//�ʹ���֯��ؽӿ�
#define Cmd_RM_AssetMgmtOrg_Min			0x50000
#define Cmd_RM_AssetMgmtOrg_Max			0x5FFFF
#define Cmd_RM_AddAssetMgmtOrg_Req		0x50001	//���AssetMgmtOrg��Ϣ����
#define Cmd_RM_AddAssetMgmtOrg_Rsp		0x50002	//���AssetMgmtOrg��Ϣ����

#define Cmd_RM_DelAssetMgmtOrg_Req		0x50003	//ɾ��AssetMgmtOrg��Ϣ����
#define Cmd_RM_DelAssetMgmtOrg_Rsp		0x50004	//ɾ��AssetMgmtOrg��Ϣ����

#define Cmd_RM_ModifyAssetMgmtOrg_Req	0x50005	//�޸�AssetMgmtOrg��Ϣ����
#define Cmd_RM_ModifyAssetMgmtOrg_Rsp	0x50006	//�޸�AssetMgmtOrg��Ϣ����

#define Cmd_RM_QryAssetMgmtOrgByOrgID_Req		0x50007	//��ѯĳ�ʹ���֯��������AssetMgmtOrg��Ϣ����
#define Cmd_RM_QryAssetMgmtOrgByOrgID_Rsp		0x50008	//����ĳ�ʹ���֯��������AssetMgmtOrg��Ϣ

#define Cmd_RM_QryAssetMgmtOrg_Req		0x50009	//��ѯ����AssetMgmtOrg��Ϣ����
#define Cmd_RM_QryAssetMgmtOrg_Rsp		0x5000A	//����AssetMgmtOrg��Ϣ

//��Ʋ�Ʒ��ؽӿ�
#define Cmd_RM_FinancialProduct_Min			0x60000
#define Cmd_RM_FinancialProduct_Max			0x6FFFF
#define Cmd_RM_AddFinancialProduct_Req		0x60001	//�����Ʋ�Ʒ����
#define Cmd_RM_AddFinancialProduct_Rsp		0x60002	//�����Ʋ�Ʒ����

#define Cmd_RM_DelFinancialProduct_Req		0x60003	//ɾ����Ʋ�Ʒ����
#define Cmd_RM_DelFinancialProduct_Rsp		0x60004	//ɾ����Ʋ�Ʒ����

#define Cmd_RM_ModifyFinancialProduct_Req	0x60005	//�޸���Ʋ�Ʒ��Ϣ����
#define Cmd_RM_ModifyFinancialProduct_Rsp	0x60006	//�޸���Ʋ�Ʒ��Ϣ����

#define Cmd_RM_QryFinancialProduct_Req		0x60007	//��ѯ��Ʋ�Ʒ��Ϣ����
#define Cmd_RM_QryFinancialProduct_Rsp		0x60008	//��ѯ��Ʋ�Ʒ��Ϣ����

#define Cmd_RM_QryAssetOrgProRelation_Req	0x60009	//��ѯ�ʹ���֯����Ʋ�Ʒ��ϵ
#define Cmd_RM_QryAssetOrgProRelation_Rsp	0x6000A	//�����ʹ���֯����Ʋ�Ʒ��ϵ

#define Cmd_RM_SaveAssetOrgProRelation_Req	0x6000B	//�����ʹ���֯����Ʋ�Ʒ��ϵ����
#define Cmd_RM_SaveAssetOrgProRelation_Rsp	0x6000C	//�����ʹ���֯����Ʋ�Ʒ��ϵ����

//����ָ��ģ����ؽӿ�
#define Cmd_RM_RiskIndModule_Min			0x70000
#define Cmd_RM_RiskIndModule_Max			0x7FFFF
#define Cmd_RM_AddRiskIndModule_Req			0x70001	//��ӷ���ָ��ģ������
#define Cmd_RM_AddRiskIndModule_Rsp			0x70002	//��ӷ���ָ��ģ�ͷ���

#define Cmd_RM_DelRiskIndModule_Req			0x70003	//ɾ������ָ��ģ������
#define Cmd_RM_DelRiskIndModule_Rsp			0x70004	//ɾ������ָ��ģ�ͷ���

#define Cmd_RM_ModifyRiskIndModule_Req		0x70005	//�޸ķ���ָ��ģ������
#define Cmd_RM_ModifyRiskIndModule_Rsp		0x70006	//�޸ķ���ָ��ģ�ͷ���

#define Cmd_RM_QryRiskIndModule_Req			0x70007	//��ѯ����ָ��ģ������
#define Cmd_RM_QryRiskIndModule_Rsp			0x70008	//��ѯ����ָ��ģ�ͷ���

#define Cmd_RM_QryValidRiskIndModule_Req	0x70009	//��ѯ��Ч�ķ���ָ��ģ������
#define Cmd_RM_QryValidRiskIndModule_Rsp	0x7000A	//��ѯ��Ч�ķ���ָ��ģ�ͷ���

//����Ԥ����ؽӿ�
#define Cmd_RM_RiskWarning_Min			0x80000
#define Cmd_RM_RiskWarning_Max			0x8FFFF
#define Cmd_RM_QryRiskWarning_Req			0x80001	//��ѯ����Ԥ������
#define Cmd_RM_QryRiskWarning_Rsp			0x80002	//��ѯ����Ԥ������

#define Cmd_RM_SaveRiskWarning_Req			0x80003	//�������Ԥ������
#define Cmd_RM_SaveRiskWarning_Rsp			0x80004	//�������Ԥ������

#define Cmd_RM_QryMaxRiskLevel_Req			0x80005	//��ѯÿ������ָ��������ռ�������
#define Cmd_RM_QryMaxRiskLevel_Rsp			0x80006	//����ÿ������ָ��������ռ���

#define Cmd_RM_QryProductRiskWarning_Req	0x80007	//��Ʒ�ֲ�ѯ����Ԥ������
#define Cmd_RM_QryProductRiskWarning_Rsp	0x80008	//��Ʒ�ֲ�ѯ����Ԥ������

#define Cmd_RM_SaveProductRiskWarning_Req	0x80009	//��Ʒ�ֱ������Ԥ������
#define Cmd_RM_SaveProductRiskWarning_Rsp	0x8000A	//��Ʒ�ֱ������Ԥ������

#define Cmd_RM_DelProductRiskWarning_Req	0x8000B	//��Ʒ��ɾ������Ԥ������
#define Cmd_RM_DelProductRiskWarning_Rsp	0x8000C	//��Ʒ��ɾ������Ԥ������

//����ָ����ؽӿ�
#define Cmd_RM_RiskIndicator_Min			0x90000
#define Cmd_RM_RiskIndicator_Max			0x9FFFF
#define Cmd_RM_AddRiskIndicator_Req			0x90001	//��ӷ���ָ������
#define Cmd_RM_AddRiskIndicator_Rsp			0x90002	//��ӷ���ָ�귵��

#define Cmd_RM_DelRiskIndicator_Req			0x90003	//ɾ������ָ������
#define Cmd_RM_DelRiskIndicator_Rsp			0x90004	//ɾ������ָ�귵��

#define Cmd_RM_ModifyRiskIndicator_Req		0x90005	//�޸ķ���ָ������
#define Cmd_RM_ModifyRiskIndicator_Rsp		0x90006	//�޸ķ���ָ�귵��

#define Cmd_RM_QryRiskIndicator_Req			0x90007	//��ѯ����ָ������
#define Cmd_RM_QryRiskIndicator_Rsp			0x90008	//��ѯ����ָ�귵��

//�¼���Ϣģ����ؽӿ�
#define Cmd_RM_EvtMsgTemplate_Min			0xA0000
#define Cmd_RM_EvtMsgTemplate_Max			0xAFFFF
#define Cmd_RM_QryEvtMsgTemplate_Req		0xA0001	//��ѯ�¼���Ϣ����
#define Cmd_RM_QryEvtMsgTemplate_Rsp		0xA0002	//��ѯ�¼���Ϣ����

#define Cmd_RM_SaveEvtMsgTemplate_Req		0xA0003	//�����¼���Ϣ����
#define Cmd_RM_SaveEvtMsgTemplate_Rsp		0xA0004	//�����¼���Ϣ����

//�����¼���ؽӿ�
#define Cmd_RM_RiskEvent_Min			0xB0000
#define Cmd_RM_RiskEvent_Max			0xBFFFF
#define	Cmd_RM_SubscribeRiskEvent_Req			0xB0001	//���ķ���¼�����		
#define	Cmd_RM_SubscribeRiskEvent_Rsp			0xB0002	//���ķ���¼�����		
#define	Cmd_RM_RiskEvent_Push					0xB0003	//����¼�����		
#define	Cmd_RM_UnSubscribeRiskEvent_Req			0xB0004	//�˶�����¼�����		
#define	Cmd_RM_UnSubscribeRiskEvent_Rsp			0xB0005	//�˶�����¼�����	

#define Cmd_RM_QryRiskEvent_Req			0xB0006	//��ѯ�����¼�����
#define Cmd_RM_QryRiskEvent_Rsp			0xB0007	//��ѯ�����¼�����


//�����¼�������ؽӿ�
#define Cmd_RM_RiskEvtHandling_Min			0xC0000
#define Cmd_RM_RiskEvtHandling_Max			0xCFFFF
#define Cmd_RM_AddRiskEvtHandling_Req		0xC0001	//��ӷ����¼���������
#define Cmd_RM_AddRiskEvtHandling_Rsp		0xC0002	//��ӷ����¼�������

#define Cmd_RM_QryRiskEvtHandling_Req		0xC0007	//��ѯ�����¼���������
#define Cmd_RM_QryRiskEvtHandling_Rsp		0xC0008	//��ѯ�����¼�������

#define	Cmd_RM_SubscribeRiskEvtHandling_Req			0xC0009	//���ķ���¼���������		
#define	Cmd_RM_SubscribeRiskEvtHandling_Rsp			0xC000A	//���ķ���¼�������		
#define	Cmd_RM_RiskEvtHandling_Push					0xC000B	//����¼���������		
#define	Cmd_RM_UnSubscribeRiskEvtHandling_Req		0xC000C	//�˶�����¼���������		
#define	Cmd_RM_UnSubscribeRiskEvtHandling_Rsp		0xC000D	//�˶�����¼�������	

//Ȩ����ؽӿ�
#define Cmd_RM_Permission_Min				0xD0000
#define Cmd_RM_Permission_Max				0xD9999
#define Cmd_RM_QryPermissionNode_Req		0xD0001	//��ѯȨ�޽ڵ�����
#define Cmd_RM_QryPermissionNode_Rsp		0xD0002	//��ѯȨ�޽ڵ㷵��

#define Cmd_RM_QryPermissionAction_Req		0xD0003	//��ѯȨ�޶�������
#define Cmd_RM_QryPermissionAction_Rsp		0xD0004	//��ѯȨ�޶�������

#define Cmd_RM_AddPermissionName_Req		0xD0005	//���Ȩ��������
#define Cmd_RM_AddPermissionName_Rsp		0xD0006	//���Ȩ��������

#define Cmd_RM_DelPermissionName_Req		0xD0007	//ɾ��Ȩ��������
#define Cmd_RM_DelPermissionName_Rsp		0xD0008	//ɾ��Ȩ��������

#define Cmd_RM_ModifyPermissionName_Req		0xD0009	//�޸�Ȩ��������
#define Cmd_RM_ModifyPermissionName_Rsp		0xD000A	//�޸�Ȩ��������

#define Cmd_RM_QryPermissionName_Req		0xD000B	//��ѯȨ��������
#define Cmd_RM_QryPermissionName_Rsp		0xD000C	//��ѯȨ��������

#define Cmd_RM_QryPermissionValue_Req		0xD000D	//��ѯ����Ȩ����������
#define Cmd_RM_QryPermissionValue_Rsp		0xD000E	//��ѯ����Ȩ�����ݷ���

#define Cmd_RM_QryPermissionValueByID_Req	0xD000F	//��ѯȨ���������󣬲���ΪȨ��ID
#define Cmd_RM_QryPermissionValueByID_Rsp	0xD0010	//��ѯȨ�����ݷ��أ�����ΪȨ��ID

#define Cmd_RM_SavePermissionValue_Req		0xD0011	//����Ȩ����������
#define Cmd_RM_SavePermissionValue_Rsp		0xD0012	//����Ȩ�����ݷ���

#define Cmd_RM_QryPermissionNameEx_Req		0xD0013	//��ѯȨ������չ�ṹ����
#define Cmd_RM_QryPermissionNameEx_Rsp		0xD0014	//��ѯȨ������չ�ṹ����

//��Ϣ��ؽӿ�
#define Cmd_RM_Message_Min					0xE0000
#define Cmd_RM_Message_Max					0xEFFFF
#define Cmd_RM_AddMessage_Req				0xE0001	//�����Ϣ����
#define Cmd_RM_AddMessage_Rsp				0xE0002	//�����Ϣ��Ӧ
#define Cmd_RM_QrySendMessage_Req			0xE0003	//��ѯ��Ϣ���󣬲�ѯ�ѷ��͵���Ϣ
#define Cmd_RM_QrySendMessage_Rsp			0xE0004	//��ѯ��Ϣ����
#define Cmd_RM_QryMsgSendStatus_Req			0xE0005	//��ѯ��Ϣ����״̬����
#define Cmd_RM_QryMsgSendStatus_Rsp			0xE0006	//��ѯ��Ϣ����״̬����
#define Cmd_RM_QryUnReadMessage_Req			0xE0007	//��ѯδ�Ķ�����Ϣ
#define Cmd_RM_SendUnReadMessage_Rsp		0xE0008	//����δ�Ķ�����Ϣ
#define Cmd_RM_SendUnReadMessage_Push		0xE0009	//����δ�Ķ�����Ϣ
#define Cmd_RM_QryRecvMessage_Req			0xE000A	//��ѯ���յ���Ϣ����
#define Cmd_RM_QryRecvMessage_Rsp			0xE000B	//�������յ���Ϣ

//��Լ��Ϣ							
#define	Cmd_RM_Instrument_Min			0xF0000			
#define	Cmd_RM_Instrument_Max			0xFFFFF	
#define	Cmd_RM_InstrumentIDList_Req		0xF0001
#define	Cmd_RM_InstrumentIDList_Rsp		0xF0002
#define	Cmd_RM_InstrumentInfo_Req		0xF0003
#define	Cmd_RM_InstrumentInfo_Rsp		0xF0004
#define	Cmd_RM_InstrumentInfoList_Req	0xF0005
#define	Cmd_RM_InstrumentInfoList_Rsp	0xF0006
#define	Cmd_RM_ProductIDList_Req		0xF0007
#define Cmd_RM_ProductIDList_Rsp		0xF0008 
//����							
#define	Cmd_RM_SubscribeQuot_Min			0x100000			
#define	Cmd_RM_SubscribeQuot_Max			0x10FFFF			
#define	Cmd_RM_SubscribeQuot_Req			0x100001	//������������		
#define	Cmd_RM_SubscribeQuot_Rsp			0x100002	//�������鷵��		
#define	Cmd_RM_Quot_Push					0x100003	//��������		
#define	Cmd_RM_UnSubscribeQuot_Req			0x100004	//�˶���������		
#define	Cmd_RM_UnSubscribeQuot_Rsp			0x100005	//�˶����鷵��									
//�����							
#define	Cmd_RM_SubscribeDeposit_Min			0x110000			
#define	Cmd_RM_SubscribeDeposit_Max			0x11FFFF			
#define	Cmd_RM_SubscribeDeposit_Req			0x110001	//���ĳ��������		
#define	Cmd_RM_SubscribeDeposit_Rsp			0x110002	//���ĳ���𷵻�		
#define	Cmd_RM_Deposit_Push					0x110003	//���������		
#define	Cmd_RM_UnSubscribeDeposit_Req		0x110004	//�˶����������		
#define	Cmd_RM_UnSubscribeDeposit_Rsp		0x110005	//�˶�����𷵻�		
#define	Cmd_RM_QryTraderDeposit_Req		0x110006	//��ѯ���������		
#define	Cmd_RM_QryTraderDeposit_Rsp		0x110007	//��ѯ����𷵻�								
//�ɽ�							
#define	Cmd_RM_SubscribeTrade_Min			0x120000			
#define	Cmd_RM_SubscribeTrade_Max			0x12FFFF			
#define	Cmd_RM_SubscribeTrade_Req			0x120001	//���ĳɽ�����		
#define	Cmd_RM_SubscribeTrade_Rsp			0x120002	//���ĳɽ�����		
#define	Cmd_RM_Trade_Push					0x120003	//�ɽ�����		
#define	Cmd_RM_UnSubscribeTrade_Req			0x120004	//�˶��ɽ�����		
#define	Cmd_RM_UnSubscribeTrade_Rsp			0x120005	//�˶��ɽ�����	
#define Cmd_RM_QryHistroyTrade_Req			0x120006	//��ѯ��ʷ�ɽ�����
#define Cmd_RM_QryHistroyTrade_Rsp			0x120007	//��ѯ��ʷ�ɽ�����
							
//�ʽ�							
#define	Cmd_RM_SubscribeFund_Min			0x130000			
#define	Cmd_RM_SubscribeFund_Max			0x13FFFF			
#define	Cmd_RM_SubscribeFund_Req			0x130001	//�����ʽ�����		
#define	Cmd_RM_SubscribeFund_Rsp			0x130002	//�����ʽ𷵻�		
#define	Cmd_RM_Fund_Push					0x130003	//�ʽ�����		
#define	Cmd_RM_UnSubscribeFund_Req			0x130004	//�˶��ʽ�����		
#define	Cmd_RM_UnSubscribeFund_Rsp			0x130005	//�˶��ʽ𷵻�
#define Cmd_RM_QryHistroyRiskFund_Req		0x130006	//��ѯ��ʷ�ʽ�����
#define Cmd_RM_QryHistroyRiskFund_Rsp		0x130007	//��ѯ��ʷ�ʽ𷵻�
							
//�ֲ�							
#define	Cmd_RM_SubscribePosition_Min			0x140000			
#define	Cmd_RM_SubscribePosition_Max			0x14FFFF			
#define	Cmd_RM_SubscribePosition_Req			0x140001	//���ĳֲ�����		
#define	Cmd_RM_SubscribePosition_Rsp			0x140002	//���ĳֲַ���		
#define	Cmd_RM_Position_Push					0x140003	//�ֲ�����		
#define	Cmd_RM_UnSubscribePosition_Req			0x140004	//�˶��ֲ�����		
#define	Cmd_RM_UnSubscribePosition_Rsp			0x140005	//�˶��ֲַ���	
#define Cmd_RM_QryHistroyPosition_Req			0x140006	//��ѯ��ʷ�ֲ�����
#define Cmd_RM_QryHistroyPosition_Rsp			0x140007	//��ѯ��ʷ�ֲַ���
							
//����							
#define	Cmd_RM_SubscribeOrder_Min			0x150000			
#define	Cmd_RM_SubscribeOrder_Max			0x15FFFF			
#define	Cmd_RM_SubscribeOrder_Req			0x150001	//���ı�������		
#define	Cmd_RM_SubscribeOrder_Rsp			0x150002	//���ı�������		
#define	Cmd_RM_Order_Push					0x150003	//��������		
#define	Cmd_RM_UnSubscribeOrder_Req			0x150004	//�˶���������		
#define	Cmd_RM_UnSubscribeOrder_Rsp			0x150005	//�˶���������
#define Cmd_RM_QryHistroyOrder_Req			0x150006	//��ѯ��ʷ��������
#define Cmd_RM_QryHistroyOrder_Rsp			0x150007	//��ѯ��ʷ��������

//ǿƽ����
#define	Cmd_RM_ForceCloseSolution_Min			0x160000
#define	Cmd_RM_ForceCloseSolution_Max			0x16FFFF	
#define	Cmd_RM_ForceCloseSolution_Req			0x160001	//����ǿƽ��������
#define	Cmd_RM_ForceCloseSolution_Rsp			0x160002	//ǿƽ�������鷵��
#define	Cmd_RM_DelForceCloseSolution_Req		0x160003	//����ɾ��ǿƽ����
#define	Cmd_RM_DelForceCloseSolution_Rsp		0x160004	//ɾ��ǿƽ��������
#define	Cmd_RM_EditForceCloseSolution_Req		0x160005	//�����޸ķ�������
#define	Cmd_RM_EditForceCloseSolution_Rsp		0x160006	//�޸ķ������Ʒ���
#define	Cmd_RM_DefaultForceCloseSolution_Req	0x160007	//�����޸�Ĭ��ǿƽ����
#define	Cmd_RM_DefaultForceCloseSolution_Rsp	0x160008	//�޸�Ĭ��ǿƽ��������
#define	Cmd_RM_AddForceCloseSolution_Req		0x160009	//�������ǿƽ����
#define	Cmd_RM_AddForceCloseSolution_Rsp		0x160010	//���ǿƽ��������

//ǿƽ����
#define	Cmd_RM_ForceCloseRule_Min				0x170000
#define	Cmd_RM_ForceCloseRule_Max				0x17FFFF
#define	Cmd_RM_ForceCloseRule_Req				0x170001	//����ǿƽ��������
#define	Cmd_RM_ForceCloseRule_Rsp				0x170002	//ǿƽ�������鷵��
#define	Cmd_RM_DelForceCloseRule_Req			0x170003	//����ɾ��ǿƽ����
#define	Cmd_RM_DelForceCloseRule_Rsp			0x170004	//ɾ��ǿƽ���򷵻�
#define	Cmd_RM_EditForceCloseRule_Req			0x170005	//�����޸�ǿƽ����
#define	Cmd_RM_EditForceCloseRule_Rsp			0x170006	//�޸�ǿƽ���򷵻�
#define	Cmd_RM_AddForceCloseRule_Req			0x170007	//�������ǿƽ����
#define	Cmd_RM_AddForceCloseRule_Rsp			0x170008	//���ǿƽ���򷵻�

//ǿƽ������µ�
#define	Cmd_RM_ForceCloseDo_Min				0x180000
#define	Cmd_RM_ForceCloseDo_Max				0x18FFFF
#define	Cmd_RM_ForceCloseCalc_Req			0x180001	//����ǿƽ����
#define	Cmd_RM_ForceCloseCalc_Rsp			0x180002	//ǿƽ���㷵��
#define	Cmd_RM_ForceCloseOrderInsert_Req	0x180003	//����ǿƽ����¼��
#define	Cmd_RM_ForceCloseOrderInsert_Rsp	0x180004	//ǿƽ����¼�뷵��

#define	Cmd_RM_AddFundNetParam_Req				0x180009	//�������û���ֵ����
#define	Cmd_RM_AddFundNetParam_Rsp				0x18000A	//���û���ֵ��������
#define	Cmd_RM_QueryFundNetParam_Req			0x18000B	//�����ѯ���û���ֵ����������Ϊί�н����˺�ID
#define	Cmd_RM_QueryFundNetParam_Rsp			0x18000C	//��ѯ����ֵ��������

#define	Cmd_RM_QueryFundNetCalcResult_Req		0x18000F	//�����ѯ����ֵ������
#define	Cmd_RM_QueryFundNetCalcResult_Rsp		0x180010	//��ѯ����ֵ����������

#define	Cmd_RM_AddForceCloseParam_Req			0x180011	//�����Զ�ǿƽ����
#define	Cmd_RM_AddForceCloseParam_Rsp			0x180012	//�����Զ�ǿƽ����
#define	Cmd_RM_QueryForceCloseParam_Req			0x180013	//�����ѯ�Զ�ǿƽ����
#define	Cmd_RM_QueryForceCloseParam_Rsp			0x180014	//��ѯ�Զ�ǿƽ��������

#define	Cmd_RM_FundNetValue_push				0x180015	//����ֵ����

//ͳ�Ƽ���
#define	Cmd_RM_Statistics_Min				0x190000
#define	Cmd_RM_Statistics_Max				0x19FFFF
#define	Cmd_RM_SubscribeStatistics_Req		0x190001	//����ͳ������		
#define	Cmd_RM_SubscribeStatistics_Rsp		0x190002	//����ͳ�Ʒ���		
#define	Cmd_RM_Statistics_Push				0x190003	//ͳ������		
#define	Cmd_RM_UnSubscribeStatistics_Req	0x190004	//�˶�ͳ������		
#define	Cmd_RM_UnSubscribeStatistics_Rsp	0x190005	//�˶�ͳ�Ʒ���

//���ܼ���
#define	Cmd_RM_Gather_Min								0x200000
#define	Cmd_RM_Gather_Max								0x20FFFF
#define	Cmd_RM_Gather_ByOrgID_Req						0x200001	//���ģ�ĳ���ţ��������ʻ�������Ϣ����
#define	Cmd_RM_Gather_ByOrgID_Rsp						0x200002	//���ģ�ĳ���ţ��������ʻ�������Ϣ����	

#define	Cmd_RM_Gather_ByFinancialProductID_Req			0x200003	//���ģ�ĳ��Ʋ�Ʒ���������ʻ�������Ϣ����	
#define	Cmd_RM_Gather_ByFinancialProductID_Rsp			0x200004	//���ģ�ĳ��Ʋ�Ʒ���������ʻ�������Ϣ����	

#define	Cmd_RM_Gather_ByAccountID_Req					0x200005	//���ģ�ĳ�����˺ţ��������ʻ�������Ϣ����		
#define	Cmd_RM_Gather_ByAccountID_Rsp					0x200006	//���ģ�ĳ�����˺ţ��������ʻ�������Ϣ����	

#define Cmd_RM_Rank_ByAccount_Req						0x200007	//���������ʻ�����Top10���а�����	
#define Cmd_RM_Rank_ByAccount_Rsp						0x200008	//���������ʻ�����Top10���а񷵻�	

#define Cmd_RM_Rank_ByFinancialProductID_Req			0x200009	//���������껯��Ʋ�Ʒ����Top10���а�����	
#define Cmd_RM_Rank_ByFinancialProductID_Rsp			0x20000a	//���������껯��Ʋ�Ʒ����Top10���а񷵻�	

#define Cmd_RM_AnnualRate_ByAccount_Req					0x20000d	//�����껯�ʻ�Top10���а�����	
#define Cmd_RM_AnnualRate_ByAccount_Rsp					0x20000e	//�����껯�ʻ�Top10���а񷵻�	

#define Cmd_RM_AnnualRate_ByFinancialProductID_Req		0x20000f	//�����껯��Ʋ�ƷTop10���а�����	
#define Cmd_RM_AnnualRate_ByFinancialProductID_Rsp		0x200011	//�����껯��Ʋ�ƷTop10���а񷵻�	

#define Cmd_RM_AnnualRate_ByFinancialProductID_Worst_Req		0x200012	//�����껯��Ʋ�Ʒ���Top10���а�����	
#define Cmd_RM_AnnualRate_ByFinancialProductID_Worst_Rsp		0x200013	//�����껯��Ʋ�Ʒ���Top10���а񷵻�

#define Cmd_RM_GetAccountList_ByFinancialProductID_Req		0x200014	//������Ʋ�ƷID��ȡ���е�ί�н����˺�����	
#define Cmd_RM_GetAccountList_ByFinancialProductID_Rsp		0x200015	//������Ʋ�ƷID��ȡ���е�ί�н����˺ŷ���

#define Cmd_Rm_GetProfitCurve_Req		0x200016	//��������ͼ����ӿ�����
#define Cmd_Rm_GetProfitCurve_Rsp		0x200017    //��������ͼ����ӿڷ���

#define OPERATOR_SUCCESS	1		//����ɹ�
#define OPERATOR_FAILED		0		//����ʧ��