#pragma once
//�������ͷ�ļ��ж���ͻ��������������command ID

//ÿ���ͻ��˷�����������������Ҫ�ڰ�ͷ��userdata1�ĵط���дuserid����½�������

#define  MAKE_CMDID(_base,_offset) \
	((int)( (int)(_base) << 16 ) + (int)(_offset))



#define  CMDID_BASE_HIWORD_TCP						0x0000 //Tcp�����λCMDID
#define  CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT	0x0001 //���͹�˾�����λCMDID
#define  CMDID_BASE_HIWORD_DATA_USER_AND_ORG		0x0002 //�û���֯���������λCMDID
#define  CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT	0x0003 //��Ʋ�Ʒ�����λCMDID
#define  CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY		0x0004 //���ײ��Է����λCMDID
#define  CMDID_BASE_HIWORD_SESSION_LOGIN			0x0005 //��½�����λCMDID
#define  CMDID_BASE_HIWORD_MESSAGE_NOTIFY			0x0006 //��Ϣ�����λCMDID
#define  CMDID_BASE_HIWORD_BUSSINESS_OFFER			0x0007 //���̷����λCMDID
#define  CMDID_BASE_HIWORD_BUSSINESS_TRADER_DATA	0x0008 //�������ݷ����λCMDID
#define  CMDID_BASE_HIWORD_DATA_INSTRUMENTS			0x0009 //Ʒ�����ݷ����λCMDID
#define  CMDID_BASE_HIWORD_DATA_RATIO				0x000A //�������ݷ����λCMDID
#define  CMDID_BASE_HIWORD_DATA_SETTLEMENT			0x000B //��������λCMDID
#define  CMDID_BASE_HIWORD_TRADE_EXCUTE				0x000C //����ִ�и�λCMDID
#define  CMDID_BASE_HIWORD_TRADE_DATA_MANAGER		0x000D //�������ݹ����λCMDID
#define  CMDID_BASE_HIWORD_RISK						0x000E //��ظ�λCMDID
#define  CMDID_BASE_HIWORD_QUOT						0x000F //�����λCMDID
#define  CMDID_BASE_HIWORD_STATIC_PRESENTATION		0x0011 //ͳ��չ�ָ�λCMDID
#define  CMDID_BASE_HIWORD_DATA_ACCOUNT_AND_ORG		0x0012 //��֯����ί�н����˺Ÿ�λCMDID
#define  CMDID_BASE_STRATEDY						0x0013 //����
#define  CMDID_BASE_NotifyAndAsk					0x0014 //֪ͨ����ѯģ����Ϣ

//-----------------------------------------------------------------------------------
//	���涨�峣��
//-----------------------------------------------------------------------------------
//֪ͨ���ĵ��̣߳�����Ϣ���lParam=EventParamID��EventParam��pEventData�ǰ����ݣ�Stru_UniPkgHead + ���� + ��β, userint=hSocket
#ifndef WndCmd_YourPkgArrival2
#define WndCmd_YourPkgArrival2		0x3bbd
#endif

//֪ͨ���ĵ��̣߳�֪ͨ����ѯģ�鷢��֪ͨ������lParam=EventParamID��EventParam��pEventData��Stru_NotifyEvent����
#ifndef WndCmd_NotifyEventArrival
#define WndCmd_NotifyEventArrival		0x3bbe
#endif

#define MAX_SQL_LENGTH			4000	//SQL�����󳤶�
#define MAX_USABLE_SQL_LENGTH	3999

//-----------------------------------------------------------------------------------
//	���涨��������
//-----------------------------------------------------------------------------------
#define CMDID_SvrTcpDisconn				MAKE_CMDID(CMDID_BASE_HIWORD_TCP,0x2100)							//TcpServer: ��tcp���ӶϿ� pkghead��userdata1Ϊ�Ͽ���hSocket��userdata2Ϊ�ͻ���IP��userdata3Ϊ�ͻ���Port
#define CMDID_SvrTcpAccept			    MAKE_CMDID(CMDID_BASE_HIWORD_TCP,0x2111)							//TcpServer: ��tcp���� pkghead��userdata1Ϊ���ӵ�hSocket��userdata2Ϊ�ͻ���IP��userdata3Ϊ�ͻ���Port
#define CMDID_TcpClientConnect		    MAKE_CMDID(CMDID_BASE_HIWORD_TCP,0x2112)							//TcpClient: TcpClient���ӳɹ� pkghead��userdata1ΪhSocket��userdata2Ϊ������IP��userdata3Ϊ������Port
#define CMDID_TcpClientDisconn		    MAKE_CMDID(CMDID_BASE_HIWORD_TCP,0x2113)							//TcpClient: TcpClient�����ж� pkghead��userdata1ΪhSocket��userdata2Ϊ������IP��userdata3Ϊ������Port


//��½����
#define CMDID_Login_Req					MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x2000)				//��½����
#define CMDID_Login_Rsp					MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x2001)				//��½���󷵻�
#define CMDID_Trader_Login_Req			MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x3002)				//��½����
#define CMDID_Trader_Login_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x3003)				//��½���󷵻�
#define CMDID_Logoff_Req				MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x3000)				//ע������
#define CMDID_Logoff_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x3004)				//ע�����󷵻�
#define CMDID_ForceCloseClient_Push		MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4000)				//�û���ͣ�ã�ǿ�ƹرոÿͻ���

#define CMDID_GetALLError_Push		    MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x300b)				//��ȡϵͳ�Ĵ�������

//Slaver Req
#define CMDID_QueryMasterAllUserLoginStatus_Req    MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4001)				//��ȡmaster�����û���״̬
#define CMDID_QueryMasterAllUserLoginStatus_Rsp    MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4002)				//��ȡmaster�����û���״̬
#define CMDID_AddUserLoginStatusToMaster_Req       MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4003)				//�ӷ��������û�֪ͨ������
#define CMDID_AddUserLoginStatusToMaster_Rsp       MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4004)				//�ӷ��������û�֪ͨ������
#define CMDID_RemoveUserLoginStatusToMaster_Req    MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4005)				//�ӷ���ɾ���û�֪ͨ������
#define CMDID_RemoveUserLoginStatusToMaster_Rsp    MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4006)				//�ӷ��������û�֪ͨ������
#define CMDID_SubscriberAllUserLoginStatus_Req     MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4007)				//���Ļ�ȡ�����û�״̬
#define CMDID_SubscriberAllUserLoginStatus_Rsp     MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4008)				//���Ļ�ȡ�����û�״̬
#define CMDID_UnsubscriberAllUserLoginStatus_Req   MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4009)				//�˶���ȡ�����û�״̬
#define CMDID_UnsubscriberAllUserLoginStatus_Rsp   MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x400a)				//�˶���ȡ�����û�״̬
#define CMDID_AddUserLoginStatusToSlave_Req        MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x400b)				//�����������û�֪ͨ�ӷ���
#define CMDID_AddUserLoginStatusToSlave_Rsp        MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x400c)				//�����������û�֪ͨ�ӷ���
#define CMDID_RemoveUserLoginStatusToSlave_Req     MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x400d)				//������ɾ���û�֪ͨ�ӷ���
#define CMDID_RemoveUserLoginStatusToSlave_Rsp     MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x400e)				//������ɾ���û�֪ͨ�ӷ���
#define CMDID_AllUserLoginStatus_Push              MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x400f)				//�˶���ȡ�����û�״̬
#define CMDID_NotifySendToSlave_Req                MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4010)				//������ɾ���û�֪ͨ�ӷ���
#define CMDID_NotifySendToSlave_Rsp                MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4011)				//�˶���ȡ�����û�״̬
#define CMDID_NotifyStartSettlementToSlave_Push    MAKE_CMDID(CMDID_BASE_HIWORD_SESSION_LOGIN,0x4012)				//������ʼ����֪ͨ�ӷ���


//

//Ʒ�ֺͺ�Լ����
#define CMDID_QueryAllInstruments_Req   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_INSTRUMENTS,0x0001)            //�����ȡ���к�Լ
#define CMDID_QueryAllInstruments_Rsp   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_INSTRUMENTS,0x0002)            //��Ӧ��ȡ���к�Լ
#define CMDID_PushAllInstruments_Req    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_INSTRUMENTS,0x0003)            //�ͻ����������еĺ�Լ������
#define CMDID_PushAllInstruments_Rsp    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_INSTRUMENTS,0x0004)            //�ͻ����������еĺ�Լ����Ӧ

//Ʒ�ֺͺ�Լ����
#define CMDID_PushCommissionRate_Req                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_RATIO,0x0001)					//�ͻ�������һ��������������
#define CMDID_PushCommissionRate_Rsp                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_RATIO,0x0002)					//�ͻ�������һ������������Ӧ
#define CMDID_PushMarginRate_Req	                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_INSTRUMENTS,0x0003)            //�ͻ�������һ����֤���������
#define CMDID_PushMarginRate_Rsp                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_INSTRUMENTS,0x0004)            //�ͻ�������һ����֤�������Ӧ

#define BASE_YEAR 2013

//���͹�˾ģ���������ݴ������������
#define Cmd_QueryBrokerModule_Req			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x1001)	//���͹�˾ģ��������������
#define Cmd_QueryBrokerModule_Rsp			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x1002)	//���͹�˾ģ���������ݷ���


//ί�н����˺�
#define Cmd_AddTradeAccount_Req			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0001)	//���ί�н����˺�����
#define Cmd_AddTradeAccount_Rsp			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0002)	//���ί�н����˺ŷ���
#define Cmd_ModifyTradeAccount_Req		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0003)	//�޸�ί�н����˺�����
#define Cmd_ModifyTradeAccount_Rsp		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0004)	//�޸�ί�н����˺ŷ���
#define Cmd_DelTradeAccount_Req			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0005)	//ɾ��ί�н����˺�����
#define Cmd_DelTradeAccount_Rsp			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0006)	//ɾ��ί�н����˺ŷ���
#define Cmd_QueryTradeAccount_Req		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0007)	//��ѯί�н����˺�����
#define Cmd_QueryTradeAccount_Rsp		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0008)	//��ѯί�н����˺ŷ���

//���͹�˾
#define Cmd_AddBrokerInfo_Req			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0101)	//��Ӿ��͹�˾����
#define Cmd_AddBrokerInfo_Rsp			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0102)	//��Ӿ��͹�˾����
#define Cmd_ModifyBrokerInfo_Req			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0103)	//�޸ľ��͹�˾����
#define Cmd_ModifyBrokerInfo_Rsp			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0104)	//�޸ľ��͹�˾����
#define Cmd_DelBrokerInfo_Req			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0105)	//ɾ�����͹�˾����
#define Cmd_DelBrokerInfo_Rsp			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0106)	//ɾ�����͹�˾����
#define Cmd_QueryBrokerInfo_Req			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0107)	//��ѯ���͹�˾����
#define Cmd_QueryBrokerInfo_Rsp			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0108)	//��ѯ���͹�˾����

//��������
#define Cmd_AddServerGroup_Req			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0201)	//��ӷ�����������
#define Cmd_AddServerGroup_Rsp			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0202)	//��ӷ������鷵��
#define Cmd_ModifyServerGroup_Req		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0203)	//�޸ķ�����������
#define Cmd_ModifyServerGroup_Rsp		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0204)	//�޸ķ������鷵��
#define Cmd_DelServerGroup_Req			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0205)	//ɾ��������������
#define Cmd_DelServerGroup_Rsp			                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0206)	//ɾ���������鷵��
#define Cmd_QueryServerGroup_Req			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0207)	//��ѯ������������
#define Cmd_QueryServerGroup_Rsp			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0208)	//��ѯ�������鷵��

//��������ַ
#define Cmd_AddServerAddress_Req			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0301)	//��ӷ�������ַ����
#define Cmd_AddServerAddress_Rsp			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0302)	//��ӷ�������ַ����
#define Cmd_ModifyServerAddress_Req		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0303)	//�޸ķ�������ַ����
#define Cmd_ModifyServerAddress_Rsp		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0304)	//�޸ķ�������ַ����
#define Cmd_DelServerAddress_Req			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0305)	//ɾ����������ַ����
#define Cmd_DelServerAddress_Rsp			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0306)	//ɾ����������ַ����
#define Cmd_QueryServerAddress_Req		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0307)	//��ѯ��������ַ����
#define Cmd_QueryServerAddress_Rsp		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0308)	//��ѯ��������ַ����

//�����������ַ
#define Cmd_ModifyBrokerInfoQuot_Req		            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0401)	//�޸����龭�͹�˾����
#define Cmd_ModifyBrokerInfoQuot_Rsp		            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0402)	//�޸����龭�͹�˾����
#define Cmd_AddServerAddressQuot_Req		            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0403)	//��������������ַ����
#define Cmd_AddServerAddressQuot_Rsp		            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0404)	//��������������ַ����
#define Cmd_ModifyServerAddressQuot_Req	                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0405)	//�޸������������ַ����
#define Cmd_ModifyServerAddressQuot_Rsp	                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0406)	//�޸������������ַ����
#define Cmd_DelServerAddressQuot_Req		            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0407)	//ɾ�������������ַ����
#define Cmd_DelServerAddressQuot_Rsp		            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0408)	//ɾ�������������ַ����
#define Cmd_QueryServerAddressQuot_Req	                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x0409)	//��ѯ�����������ַ����
#define Cmd_QueryServerAddressQuot_Rsp	                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_BROKER_AND_ACCOUNT,0x040A)	//��ѯ�����������ַ����

//��֯����ģ���������ݴ������������
#define Cmd_QueryOrganizationModule_Req			        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x1001)	//��֯����ģ��������������
#define Cmd_QueryOrganizationModule_Rsp			        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x1002)	//��֯����ģ���������ݷ���

//��֯�ܹ�
#define Cmd_AddOrg_Req			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0001)	//�����֯�ܹ�����
#define Cmd_AddOrg_Rsp			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0002)	//�����֯�ܹ�����
#define Cmd_ModifyOrg_Req		                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0003)	//�޸���֯�ܹ�����
#define Cmd_ModifyOrg_Rsp		                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0004)	//�޸���֯�ܹ�����
#define Cmd_DelOrg_Req			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0005)	//ɾ����֯�ܹ�����
#define Cmd_DelOrg_Rsp			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0006)	//ɾ����֯�ܹ�����
#define Cmd_QueryOrg_Req			                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0007)	//��ѯ��֯�ܹ�����
#define Cmd_QueryOrg_Rsp			                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0008)	//��ѯ��֯�ܹ�����

//�û�
#define Cmd_AddUser_Req			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0101)	//����û��˺�����
#define Cmd_AddUser_Rsp			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0102)	//����û��˺ŷ���
#define Cmd_DelUser_Req			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0103)	//ɾ���û��˺�����
#define Cmd_DelUser_Rsp			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0104)	//ɾ���û��˺ŷ���
#define Cmd_ModifyUser_Req		                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0105)	//�޸��û��˻�������Ϣ����
#define Cmd_ModifyUser_Rsp		                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0106)	//�޸��û��˻�������Ϣ����
#define Cmd_QryUser_Req			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0107)	//��ѯ�����û��˺�����
#define Cmd_QryUser_Rsp			                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0108)	//��ѯ�����û��˺ŷ���
#define Cmd_ModifyUserPassword_Req	                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0109)	//�޸��û�(����Ա)��������
#define Cmd_ModifyUserPassword_Rsp	                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x010A)	//�޸��û�(����Ա)���뷵��
#define Cmd_UserStatus_Req		                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x010B)	//�Ƿ���������
#define Cmd_UserStatus_Rsp		                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x010C)   //�Ƿ����÷���

//�û����ɫ��ϵ
#define Cmd_SaveUserRoleRelation_Req		            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0201)	//�����û����ɫ��ϵ����
#define Cmd_SaveUserRoleRelation_Rsp		            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0202)	//�����û����ɫ��ϵ����
#define Cmd_QryUserRoleRelation_Req		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0203)	//��ѯ�û����ɫ��ϵ����
#define Cmd_QryUserRoleRelation_Rsp		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0204)	//��ѯ�û����ɫ��ϵ����

//�û�����֯������ϵ
#define Cmd_SaveUserOrgRelation_FromUser_Req		    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0301)	//���������û�ģ����û�����֯������ϵ���� 
#define Cmd_SaveUserOrgRelation_FromUser_Rsp		    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0302)	//���������û�ģ����û�����֯������ϵ����
#define Cmd_SaveUserOrgRelation_FromOrg_Req		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0303)	//�������Ի���ģ����û�����֯������ϵ����
#define Cmd_SaveUserOrgRelation_FromOrg_Rsp		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0304)	//�������Ի���ģ����û�����֯������ϵ����
#define Cmd_QryUserOrgRelation_Req		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0305)	//��ѯ�û�����֯������ϵ����
#define Cmd_QryUserOrgRelation_Rsp		                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0306)	//��ѯ�û�����֯������ϵ����


//�û���ί�н����˺Ź�ϵ
#define Cmd_SaveUserTradeAcctRelation_Req	            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0401)	//�����û���ί�н����˺Ź�ϵ����
#define Cmd_SaveUserTradeAcctRelation_Rsp	            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0402)	//�����û���ί�н����˺Ź�ϵ����
#define Cmd_QryUserTradeAcctRelation_Req		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0403)	//��ѯ�û���ί�н����˺Ź�ϵ����
#define Cmd_QryUserTradeAcctRelation_Rsp		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0404)	//��ѯ�û���ί�н����˺Ź�ϵ����

//��֯������ί�н����˺Ź�ϵ
#define Cmd_QryTradeAccountAndOrgRelation_Req		    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_ACCOUNT_AND_ORG,0x0001)	//��ѯί�н����˺ź���֯������ϵ����
#define Cmd_QryTradeAccountAndOrgRelation_Rsp		    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_ACCOUNT_AND_ORG,0x0002)	//��ѯί�н����˺ź���֯������ϵ����
#define Cmd_SaveTradeAccountAndOrgRelation_Req          MAKE_CMDID(CMDID_BASE_HIWORD_DATA_ACCOUNT_AND_ORG,0x0003)	//����ί�н����˺ź���֯������ϵ����
#define Cmd_SaveTradeAccountAndOrgRelation_Rsp          MAKE_CMDID(CMDID_BASE_HIWORD_DATA_ACCOUNT_AND_ORG,0x0004)	//����ί�н����˺ź���֯������ϵ����

//��ɫ
#define Cmd_AddRole_Req		                            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0501)	//��ӽ�ɫ����
#define Cmd_AddRole_Rsp		                            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0502)	//��ӽ�ɫ����
#define Cmd_DelRole_Req		                            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0503)	//ɾ����ɫ����
#define Cmd_DelRole_Rsp		                            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0504)	//ɾ����ɫ����
#define Cmd_ModifyRole_Req	                            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0505)	//�޸Ľ�ɫ����
#define Cmd_ModifyRole_Rsp	                            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0506)	//�޸Ľ�ɫ����
#define Cmd_QryRole_Req		                            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0507)	//��ѯ��ɫ����
#define Cmd_QryRole_Rsp		                            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0508)	//��ѯ��ɫ����

//Ȩ��
#define Cmd_QryPrivilege_Req		                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0601)	//��ѯ����Ȩ����������
#define Cmd_QryPrivilege_Rsp		                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0602)	//��ѯ����Ȩ�����ݷ���

//��ɫ��Ȩ�޵Ĺ�ϵ
#define Cmd_QueryRolePrivilegeRelation_Req		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0701)	//��ѯ��ɫ��Ȩ�޹�ϵ����
#define Cmd_QueryRolePrivilegeRelation_Rsp		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0702)	//��ѯ��ɫ��Ȩ�޹�ϵ����
#define Cmd_SaveRolePrivilegeRelation_Req		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0703)	//�����ɫ��Ȩ�޹�ϵ����
#define Cmd_SaveRolePrivilegeRelation_Rsp		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_USER_AND_ORG,0x0704)	//�����ɫ��Ȩ�޹�ϵ����

//��Ʋ�Ʒģ���������ݴ������������
#define Cmd_QueryProductModule_Req			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT,0x1001)	//��Ʋ�Ʒģ��������������
#define Cmd_QueryProductModule_Rsp			            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT,0x1002)	//��Ʋ�Ʒģ���������ݷ���

//��Ʋ�Ʒ
#define Cmd_AddFinicialProduct_Req                      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0001)//������Ʋ�Ʒ����
#define Cmd_AddFinicialProduct_Rsp                      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0002)//������Ʋ�Ʒ����
#define Cmd_DeleteFinicialProduct_Req                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0003)//ɾ����Ʋ�Ʒ����
#define Cmd_DeleteFinicialProduct_Rsp                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0003)//ɾ����Ʋ�Ʒ����
#define Cmd_ModifyFinicialProduct_Req                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0005)//�޸���Ʋ�Ʒ����
#define Cmd_ModifyFinicialProduct_Rsp                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0006)//�޸���Ʋ�Ʒ����
#define Cmd_QueryFinicialProduct_Req                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0007)//��ѯ������Ʋ�Ʒ����
#define Cmd_QueryFinicialProduct_Rsp                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0008)//��ѯ������Ʋ�Ʒ����

//��Ʋ�Ʒ�뽻��Ա��ϵ
#define Cmd_QueryProductTraderRelation_Req		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0101)	//��ѯ��Ʋ�Ʒ�뽻��Ա��ϵ����
#define Cmd_QueryProductTraderRelation_Rsp		        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0102)	//��ѯ��Ʋ�Ʒ�뽻��Ա��ϵ����
#define Cmd_SaveProductTraderRelation_FromUser_Req      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0103)//���������û�ģ�����Ʋ�Ʒ�뽻��Ա��ϵ����
#define Cmd_SaveProductTraderRelation_FromUser_Rsp      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0104)//���������û�ģ�����Ʋ�Ʒ�뽻��Ա��ϵ����
#define Cmd_SaveProductTraderRelation_FromProduct_Req   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0105)//����������Ʋ�Ʒģ�����Ʋ�Ʒ�뽻��Ա��ϵ����
#define Cmd_SaveProductTraderRelation_FromProduct_Rsp   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_FINANCIAL_PRODUCT, 0x0106)//����������Ʋ�Ʒģ�����Ʋ�Ʒ�뽻��Ա��ϵ����


//���ײ���
#define Cmd_AddTradeStrategy_Req                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0001)          //���ӽ��ײ�������
#define Cmd_AddTradeStrategy_Rsp                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0002)          //���ӽ��ײ��Է���
#define Cmd_DeleteTradeStrategy_Req                     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0003)          //ɾ�����ײ�������
#define Cmd_DeleteTradeStrategy_Rsp                     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0004)          //ɾ�����ײ��Է���   
#define Cmd_ModifyTradeStrategy_Req                     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0005)          //�޸Ľ��ײ�������
#define Cmd_ModifyTradeStrategy_Rsp                     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0006)          //�޸Ľ��ײ��Է���
#define Cmd_QueryTradeStrategy_Req                      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0007)          //��ѯ���н��ײ�������
#define Cmd_QueryTradeStrategy_Rsp                      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0008)          //��ѯ���н��ײ��Է���

//���ײ�������֯������ϵ
#define Cmd_SaveStrategyOrgRelation_Req	                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0101)	 //���ӽ��ײ�������֯������ϵ����
#define Cmd_SaveStrategyOrgRelation_Rsp	                MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0102)	 //���ӽ��ײ�������֯������ϵ����
#define Cmd_QueryStrategyOrgRelation_Req	            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0103)	 //��ѯ���ײ�������֯������ϵ����
#define Cmd_QueryStrategyOrgRelation_Rsp	            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_TRADER_STRATEGY, 0x0104)	 //��ѯ���ײ�������֯������ϵ����

//����ģ��
#define Cmd_Offer_Trade_Push						 MAKE_CMDID(CMDID_BASE_HIWORD_BUSSINESS_OFFER,0x0001)	//�ɽ�����			
#define Cmd_Offer_Order_Push						 MAKE_CMDID(CMDID_BASE_HIWORD_BUSSINESS_OFFER,0x0002)	//��������			
#define Cmd_Offer_InstrumentCommissionRate_Push      MAKE_CMDID(CMDID_BASE_HIWORD_BUSSINESS_OFFER,0x0003)	//��Լ��������		
#define Cmd_Offer_InstrumentMarginRate_Push          MAKE_CMDID(CMDID_BASE_HIWORD_BUSSINESS_OFFER,0x0004)	//��Լ��֤����


//����ģ��			
#define Cmd_Settlement_SettlementFinish_Notify             MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0001)  //������֪ͨ�ͻ��˽��������Ϣ		
#define Cmd_Settlement_SettlementFinish_Confirm            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0002)  //�ͻ��˽������ȷ����Ϣ����
#define Cmd_Settlement_QueryFund_Req                       MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0003)  //�ͻ��˲�ѯ�ʽ���Ϣ		
#define Cmd_Settlement_QueryFund_Rsp                       MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0004)  //�����������ʽ���Ϣ
#define Cmd_Settlement_QuerySettlementPrice_Req            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0005)  //�ͻ��˲�ѯ�������Ϣ		
#define Cmd_Settlement_QuerySettlementPrice_Rsp            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0006)  //���������ؽ������Ϣ
#define Cmd_Settlement_ModifySettlementPrice_Req           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0007)  //�ͻ����޸Ľ������Ϣ		
#define Cmd_Settlement_ModifySettlementPrice_Rsp           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0008)  //�������޸Ľ������Ϣ
#define Cmd_Settlement_QueryTrade_Req          	           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0009)	//�ͻ��˲�ѯ����Ա���˺ųɽ�������Ϣ
#define Cmd_Settlement_QueryTrade_Rsp                      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x000A)	//���������ؽ���Ա���˺ųɽ���Ϣ
#define Cmd_Settlement_AddTrade_Req          	           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x000B)	//�ͻ������ӽ���Ա���˺ųɽ���Ϣ
#define Cmd_Settlement_AddTrade_Rsp                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x000C)	//�������������ӽ���Ա���˺ųɽ���Ϣ
#define Cmd_Settlement_DeleteTrade_Req          	       MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x000D)	//�ͻ���ɾ������Ա���˺ųɽ���Ϣ
#define Cmd_Settlement_DeleteTrade_Rsp                     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x000E)	//����������ɾ������Ա���˺ųɽ���Ϣ
#define Cmd_Settlement_ModifyTrade_Req          	       MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x000F)	//�ͻ����޸Ľ���Ա���˺ųɽ���Ϣ
#define Cmd_Settlement_ModifyTrade_Rsp                     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0010)	//�����������޸Ľ���Ա���˺ųɽ���Ϣ
#define Cmd_Settlement_QueryPositionDetail_Req             MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0011)  //�ͻ��˲�ѯ����Ա���˺ųֲ���ϸ��Ϣ		
#define Cmd_Settlement_QueryPositionDetail_Rsp             MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0012)  //���������ؽ���Ա���˺ųֲ���ϸ��Ϣ
#define Cmd_Settlement_AddPositionDetail_Req               MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0013)  //�ͻ������ӽ���Ա���˺ųֲ���ϸ��Ϣ		
#define Cmd_Settlement_AddPositionDetail_Rsp               MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0014)  //�������������ӽ���Ա���˺ųֲ���ϸ��Ϣ
#define Cmd_Settlement_DeletePositionDetail_Req            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0015)  //�ͻ���ɾ������Ա���˺ųֲ���ϸ��Ϣ		
#define Cmd_Settlement_DeletePositionDetail_Rsp            MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0016)  //����������ɾ������Ա���˺ųֲ���ϸ��Ϣ
#define Cmd_Settlement_ModifyPositionDetail_Req	           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0017)	//�ͻ����޸Ľ���Ա���˺ųֲ���ϸ��Ϣ			
#define Cmd_Settlement_ModifyPositionDetail_Rsp	           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0018)	//�����������޸Ľ���Ա���˺ųֲ���ϸ��Ϣ
#define Cmd_Settlement_QueryPosition_Req                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0019)	//�ͻ���������ί���˺ųֲֻ��ܽ�����Ϣ
#define Cmd_Settlement_QueryPosition_Rsp                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x001A)	//���������ؽ���ί���˺ųֲֻ�����Ϣ
#define Cmd_Settlement_AddPosition_Req                     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x001B)	//�ͻ����������ӽ���ί���˺ųֲֻ�����Ϣ
#define Cmd_Settlement_AddPosition_Rsp                     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x001C)	//�������������ӽ���ί���˺ųֲֻ�����Ϣ
#define Cmd_Settlement_DeletePosition_Req                  MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x001D)	//�ͻ�������ɾ������ί���˺ųֲֻ�����Ϣ
#define Cmd_Settlement_DeletePosition_Rsp                  MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x001E)	//����������ɾ������ί���˺ųֲֻ�����Ϣ
#define Cmd_Settlement_ModifyPosition_Req                  MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x001F)	//�ͻ��������޸Ľ���ί���˺ųֲֻ�����Ϣ
#define Cmd_Settlement_ModifyPosition_Rsp                  MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0020)	//�����������޸Ľ���ί���˺ųֲֻ�����Ϣ
#define Cmd_Settlement_ModifyFund_Req	                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0021)	//�ͻ����޸��ʽ������Ϣ			
#define Cmd_Settlement_ModifyFund_Rsp	                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0022)	//������ȷ���޸��ʽ������Ϣ����
#define Cmd_Settlement_SetSettltmentTime_Req	           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0023)	//�ͻ������ý���ʱ������			
#define Cmd_Settlement_SetSettltmentTime_Rsp	           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0024)	//���������ý���ʱ�䷵��
#define Cmd_Settlement_ManualQueryData_Req	               MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0025)	//�ͻ����ֶ���ѯ�ɽ������������󣨱����ͳɽ���			
#define Cmd_Settlement_ManualQueryData_Rsp	               MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0026)	//�������ֶ���ѯ�ɽ��������ݷ���
#define Cmd_Settlement_ConfirmSettlementPrice_Req          MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0027)	//�����ȷ��
#define Cmd_Settlement_ConfirmSettlementPrice_Rsp          MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0028)	//�����ȷ�Ϸ���
#define Cmd_Settlement_ConfirmSettlementFund_Req           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0029)	//������ȷ��
#define Cmd_Settlement_ConfirmSettlementFund_Rsp           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x002A)	//������ȷ�Ϸ���
#define Cmd_Settlement_QueryCommissionMarginRate_Req       MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x002B)  //�ͻ��˲�ѯ��������Ϣ	
#define Cmd_Settlement_QueryCommissionMarginRate_Rsp       MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x002C)  //��������ѯ��������Ϣ����
#define Cmd_Settlement_ModifyCommissionMarginRate_Req      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x002D)  //�ͻ����޸���������Ϣ
#define Cmd_Settlement_ModifyCommissionMarginRate_Rsp      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x002E)  //�ͷ������޸���������Ϣ����
#define Cmd_Settlement_ConfirmCommissionMarginRate_Req     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x002F)  //�ͻ��˷�����Ϣȷ��
#define Cmd_Settlement_ConfirmCommissionMarginRate_Rsp     MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0030)  //������������Ϣȷ�Ϸ���
#define Cmd_Settlement_LoadData_Req                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0031)  //���ؽ�����������
#define Cmd_Settlement_LoadData_Rsp                        MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0032)  //���ؽ������ݷ���
#define Cmd_Settlement_ConfirmTrade_Req                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0033)	//�ɽ�ȷ��
#define Cmd_Settlement_ConfirmTrade_Rsp                    MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0034)	//�ɽ�ȷ�Ϸ���
#define Cmd_Settlement_StartSettlement_Req                 MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0035)	//��ʼ��������
#define Cmd_Settlement_StartSettlement_Rsp                 MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0036)	//��ʼ���㷵��
#define Cmd_Settlement_QueryTradingday_Req                 MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0037)	//�ͻ��������ѯ������
#define Cmd_Settlement_QueryTradingday_Rsp                 MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0038)	//��������ѯ�����շ���
#define Cmd_Settlement_TradeCaculate_Req                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0039)	//�ͻ�������ɽ��Լ���
#define Cmd_Settlement_TradeCaculate_Rsp                   MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x003A)	//�������ɽ��Լ��㷵��
#define Cmd_Settlement_SettleBill_Req                      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x003B)	//�ͻ���������㵥
#define Cmd_Settlement_SettleBill_Rsp                      MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x003C)	//���������㵥����
#define Cmd_Settlement_QuerySettltmentTime_Req             MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x003D)	//�ͻ��������Զ�����ʱ��
#define Cmd_Settlement_QuerySettltmentTime_Rsp             MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x003E)	//�������Զ�����ʱ�䷵��
#define Cmd_Settlement_SettlementWinColse_Notify           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x003F)  //�ͻ��˹رս��㴰��
#define Cmd_Settlement_QueryCTPLoadStatus_Req              MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0040)	//ÿ������Ա�Ƿ���سɹ�����
#define Cmd_Settlement_QueryCTPLoadStatus_Rsp              MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0041)	//ÿ������Ա�Ƿ���سɹ�����
#define Cmd_Settlement_LoadHistoryCommission_Req           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0042)	//������ʷ��������
#define Cmd_Settlement_LoadHistoryCommission_Rsp           MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0043)	//������ʷ���ʷ���
#define Cmd_Settlement_ImmediatelyAutoSettlement_Req       MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0044)	//�����Զ���������
#define Cmd_Settlement_ImmediatelyAutoSettlement_Rsp       MAKE_CMDID(CMDID_BASE_HIWORD_DATA_SETTLEMENT,0x0045)	//�����Զ����㷵��

//����ִ��
#define Cmd_TradeExcute_InputOrder_Req						MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0001)	//�µ�����
#define Cmd_TradeExcute_InputOrder_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0002)	//�µ�����
#define Cmd_TradeExcute_Trader_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0003)	//CTP���ر�����Ӧ
#define Cmd_TradeExcute_OrderStatus_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0004)	//���ͱ���״̬���ͻ���
#define Cmd_TradeExcute_VerifyOrder_Req						MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0005)	//������˿ͻ�������µ�����
#define Cmd_TradeExcute_VerifyOrder_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0006)	//�µ�������˽������
#define Cmd_TradeExcute_VerifyOrder_RspRsp					MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0007)	//��˽����Ӧ�ķ���
#define Cmd_TradeExcute_CancelOrder_Req						MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0101)	//��������
#define Cmd_TradeExcute_CancelOrder_Fail_RSP				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0103)	//����ʧ��
#define Cmd_TradeExcute_CancelOrder_Error_RSP				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0104)	//��������
#define Cmd_TradeExcute_QryApprovingOrder_Req				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0105)	//��ѯ����еı�������
#define Cmd_TradeExcute_QryApprovingOrder_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0106)	//��ѯ����еı�������
#define Cmd_TradeExcute_StategyInputOrder_Req				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0107)	//���Խ����µ�����
#define Cmd_TradeExcute_StategyInputOrder_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0108)	//���Խ����µ�����
#define Cmd_TradeExcute_StategyActionOrder_Req				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x0109)	//���Խ��̳�������
#define Cmd_TradeExcute_StategyActionOrder_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_EXCUTE,0x010a)	//���Խ��̳�������
// �û���Ϣ����
#define Cmd_UserMsg_SubsMsg_Req								MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0001)	//�û�������Ϣ����
#define Cmd_UserMsg_SubsMsg_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0002)	//�û�������Ϣ������Ӧ��
#define Cmd_UserMsg_UnSubsMsg_Req							MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0003)	//�û�ȡ��������Ϣ����
#define Cmd_UserMsg_UnSubsMsg_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0004)	//�û�ȡ��������Ϣ������Ӧ��
#define Cmd_UserMsg_PushMsg									MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0005)	//�û���Ϣ����
#define Cmd_UserMsg_Send_Req								MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0006)	//�û�������Ϣ����
#define Cmd_UserMsg_Send_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0007)	//�û�������Ϣ������Ӧ��
#define Cmd_UserMsg_Query_Send_Req							MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0008)	//�û���ѯ������Ϣ����
#define Cmd_UserMsg_Query_Send_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0009)	//�û���ѯ������Ϣ������Ӧ��
#define Cmd_UserMsg_Query_Recv_Req							MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x000a)	//�û���ѯ������Ϣ����
#define Cmd_UserMsg_Query_Recv_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x000b)	//�û���ѯ������Ϣ������Ӧ��
#define Cmd_UserMsg_QueryTargetMsgFromSlave_Req				MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x000c)	//slave��master��ѯ���е�δ���͵�Ŀ����Ϣ
#define Cmd_UserMsg_QueryTargetMsgFromSlave_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x000d)	//slave��master��ѯ���е�δ���͵�Ŀ����Ϣ��Ӧ��
#define Cmd_UserMsg_SynchrPushMsg							MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x000e)	//ͬ��slaveʱ��Ŀ����Ϣ����
#define Cmd_UserMsg_TurnSendReqToMaster_Req					MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x000f)	//�û�������Ϣ����ת����master
#define Cmd_UserMsg_TurnSendReqToMaster_Rsp					MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0010)	//�û�������Ϣ����ת����master��Ӧ��
//#define Cmd_UserMsg_SynchroTargetMsgToSlave_Req				MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0011)	//master��Ŀ����Ϣͬ����slave
//#define Cmd_UserMsg_SynchroTargetMsgToSlave_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0012)	//master��Ŀ����Ϣͬ����slave��Ӧ��
#define Cmd_UserMsg_SynchroSentStatusToSlave_Req			MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0013)	//master��Ŀ����Ϣ����״̬ͬ����slave
#define Cmd_UserMsg_SynchroSentStatusToSlave_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0014)	//master��Ŀ����Ϣ����״̬ͬ����slave��Ӧ��
#define Cmd_UserMsg_SynchroSentStatusToMaster_Req			MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0015)	//slave��Ŀ����Ϣ����״̬ͬ����master
#define Cmd_UserMsg_SynchroSentStatusToMaster_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_MESSAGE_NOTIFY,0x0016)	//slave��Ŀ����Ϣ����״̬ͬ����master��Ӧ��


//�������ݹ���ģ��
#define Cmd_TradeDataManager_QryInstrument_Req				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0001)	//��ѯ��Լ
#define Cmd_TradeDataManager_QryInstrument_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0002)	//��ѯ��Լ��Ӧ
#define Cmd_TradeDataManager_QryTraderAccount_Req		    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0003)	//��ѯ�ʽ��˻�
#define Cmd_TradeDataManager_QryTraderAccount_Rsp		    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0004)	//��ѯ�ʽ��˻���Ӧ
#define Cmd_TradeDataManager_QryTrade_Req					MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0005)	//��ѯ�ɽ�
#define Cmd_TradeDataManager_QryTrade_Rsp					MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0006)	//��ѯ�ɽ���Ӧ
#define Cmd_TradeDataManager_QryOrder_Req				    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0007)	//��ѯ����
#define Cmd_TradeDataManager_QryOrder_Rsp					MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0008)	//��ѯ������Ӧ
#define Cmd_TradeDataManager_QryPosition_Req				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0009)	//��ѯ�ֲ�
#define Cmd_TradeDataManager_QryPosition_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x000a)	//��ѯ�ֲ���Ӧ
#define Cmd_TradeDataManager_QryPositionDetail_Req			MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x000b)	//��ѯ�ֲ���ϸ
#define Cmd_TradeDataManager_QryPositionDetail_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x000c)	//��ѯ�ֲ���ϸ��Ӧ
#define Cmd_TradeDataManager_QryComPositionDetail_Req		MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x000d)	//��ѯ��ϳֲ���ϸ
#define Cmd_TradeDataManager_QryComPositionDetail_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x000e)	//��ѯ��ϳֲ���ϸ��Ӧ
#define Cmd_TradeDataManager_QryCommissionRat_Req			MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x000f)	//��ѯ��������
#define Cmd_TradeDataManager_QryCommissionRat_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0010)	//��ѯ����������Ӧ
#define Cmd_TradeDataManager_QryMarginRate_Req				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0011)	//��ѯ��֤����
#define Cmd_TradeDataManager_QryMarginRate_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0012)	//��ѯ��֤����Ӧ
#define Cmd_TradeDataManager_FundInOut_Req				    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0013)	//���������
#define Cmd_TradeDataManager_FundInOut_Rsp				    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0014)	//�������Ӧ
#define Cmd_TradeDataManager_QryFundInOut_Req				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0015)	//��ѯ���������
#define Cmd_TradeDataManager_QryFundInOut_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0016)	//��ѯ�������Ӧ
#define Cmd_TradeDataManager_QrySettlementConfirm_Req       MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0017)	//��ѯ����ȷ��
#define Cmd_TradeDataManager_QrySettlementConfirm_Rsp	    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0018)	//��ѯ����ȷ����Ӧ
#define Cmd_TradeDataManager_QrySettlementInfo_Req          MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0019)	//��ѯ���㵥��Ϣ
#define Cmd_TradeDataManager_QrySettlementInfo_Rsp	        MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x001a)	//��ѯ���㵥��Ϣ��Ӧ
#define Cmd_TradeDataManager_QryAccountStatus_Req           MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x001b)	//��ѯί�н����˺�״̬
#define Cmd_TradeDataManager_QryAccountStatus_Rsp	        MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x001c)	//��ѯί�н����˺�״̬��Ӧ
#define Cmd_TradeDataManager_LoginAccount_Req			    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x001d)	//��½��ί�н����˺ŵ�����
#define Cmd_TradeDataManager_LoginAccount_Rsp	            MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x001e)	//��½��ί�н����˺ŵ���Ӧ
#define Cmd_TradeDataManager_SettlementConfirm_Req 		    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0020)	//ȷ�Ͻ��㵥����
#define Cmd_TradeDataManager_SettlementConfirm_Rsp	        MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0021)	//ȷ�Ͻ��㵥��Ӧ
#define Cmd_TradeDataManager_QryUserInitStatus_Req 		    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0022)	//��ѯ�û���ʼ��״̬
#define Cmd_TradeDataManager_QryUserInitStatus_Rsp	        MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0023)	//��ѯ�û���ʼ��״̬��Ӧ
#define Cmd_TradeDataManager_InitUser_Req			        MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0024)	//��ʼ���û�����
#define Cmd_TradeDataManager_InitUser_Rsp	                MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0025)	//��ʼ���û�����
#define Cmd_TradeDataManager_ReInit_Req					    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0026)	//���³�ʼ������ģ��
#define Cmd_TradeDataManager_ReInit_Rsp					    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0027)	//���³�ʼ������ģ��
#define Cmd_TradeDataManager_QryReinitTime_Req              MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0028)	//�ͻ��������Զ����³�ʼ��ʱ��
#define Cmd_TradeDataManager_QryReinitTime_Rsp              MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0029)	//�������Զ����³�ʼ��ʱ��
#define Cmd_TradeDataManager_SetReinitTime_Req              MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x002a)	//�ͻ������������Զ����³�ʼ��ʱ��
#define Cmd_TradeDataManager_SetReinitTime_Rsp              MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x002b)	//�������Զ����³�ʼ��ʱ��
#define Cmd_TradeDataManager_QryStatus_Req                  MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x002c)	//�ͻ������������Զ����³�ʼ��ʱ��
#define Cmd_TradeDataManager_QryStatus_Rsp                  MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x002d)	//�������Զ����³�ʼ��ʱ��
#define Cmd_TradeDataManager_QryTradingTime_Req             MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x002e)	//��ѯ����ʱ��
#define Cmd_TradeDataManager_QryTradingTime_Rsp             MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x002f)	//��ѯ����ʱ��
#define Cmd_TradeDataManager_SetTradingTime_Req             MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0030)	//���ý���ʱ��
#define Cmd_TradeDataManager_SetTradingTime_Rsp             MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0031)	//���ý���ʱ��
#define Cmd_TradeDataManager_QryNotice_Req                  MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0032)	//�����ѯ�ͻ�֪ͨ
#define Cmd_TradeDataManager_QryNotice_Rsp                  MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0033)	//�����ѯ�ͻ�֪ͨ��Ӧ
#define Cmd_TradeDataManager_QryContractBank_Req            MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0034)	//�ڻ������ѯ�������֪ͨ
#define Cmd_TradeDataManager_QryContractBank_Rsp            MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0035)	//�ڻ������ѯ�������֪ͨ
#define Cmd_TradeDataManager_QryParkedOrder_Req             MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0036)	//�����ѯԤ��
#define Cmd_TradeDataManager_QryParkerOrder_Rsp             MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0037)	//�����ѯԤ��
#define Cmd_TradeDataManager_QryParkedOrderAction_Req       MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0038)	//�����ѯԤ�񳷵�
#define Cmd_TradeDataManager_QryParkerOrderAction_Rsp       MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0039)	//�����ѯԤ�񳷵�
#define Cmd_TradeDataManager_QryDepthMarket_Req             MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0040)	//�����ѯ�������
#define Cmd_TradeDataManager_QryDepthMarket_Rsp             MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0041)	//�����ѯ�������
#define Cmd_TradeDataManager_ReForceInit_Req				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0042)	//ǿ�����³�ʼ������ģ��
#define Cmd_TradeDataManager_ReForceInit_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0043)	//ǿ�����³�ʼ������ģ��

#define Cmd_TradeDataManager_SlaverQryTradingDay_Req	    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0050)	//�Ӳ�ѯ���Ľ�����
#define Cmd_TradeDataManager_SlaverQryTradingDay_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0051)	//�Ӳ�ѯ���Ľ�����
#define Cmd_TradeDataManager_SlaverQryOrderAndTraderBiggerFTID_Req	MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0059)	//�Ӳ�ѯ���ı���
#define Cmd_TradeDataManager_SlaverQryOrderAndTraderBiggerFTID_Rsp	MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x005a)	//�Ӳ�ѯ���ı���
#define Cmd_TradeDataManager_MasterOrder_Push				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x005b)	//������������
#define Cmd_TradeDataManager_MasterTrader_Push				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x005e)	//���ɽ�������
#define Cmd_TradeDataManager_MasterQuot_Req 			    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0062)	//����������
#define Cmd_TradeDataManager_MasterQuot_Push				MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x005f)	//�����������
#define Cmd_TradeDataManager_MasterCTPOrder_Push			MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0060)	//������������
#define Cmd_TradeDataManager_MasterCTPTrader_Push			MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0061)	//���ɽ�������

#define Cmd_TradeDataManager_MasterQryCommission_Push		MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0064)	//�����������
#define Cmd_TradeDataManager_MasterQryMargin_Push		    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0065)	//�����������
#define Cmd_TradeDataManager_MasterQryFund_Push			    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0066)	//�����������
#define Cmd_TradeDataManager_MasterQryPosition_Push		    MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0067)	//�����������
#define Cmd_TradeDataManager_MasterQryPositionDetail_Push	MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0068)	//�����������
#define Cmd_TradeDataManager_MasterUserInit_Push			MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0069)	//�����������
#define Cmd_TradeDataManager_MasterQryCTPOrder_Push			MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0070)	//�����������
#define Cmd_TradeDataManager_MasterQryCTPTrader_Push		MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0071)	//�����������
#define Cmd_TradeDataManager_MasterErrOrderAction_Push      MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0072)	//�����������
#define Cmd_TradeDataManager_MasterErrOrderInsert_Push      MAKE_CMDID(CMDID_BASE_HIWORD_TRADE_DATA_MANAGER,0x0073)	//�����������


//����
#define Cmd_SubscribeQuotEvent_Req						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0001)	//���������¼�����
#define Cmd_SubscribeQuotEvent_Rsp						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0002)	//���������¼�����
#define Cmd_UnSubscribeQuotEvent_Req						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0003)	//�˶������¼�����
#define Cmd_UnSubscribeQuotEvent_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0004)	//�˶������¼�����
#define Cmd_QuotEvent_Push								    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0005)	//�����¼�����
#define Cmd_ReInitQuotServerEvent_Req						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0006)	//���������¼�����
#define Cmd_ReInitQuotServerEvent_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0007)	//���������¼�����
#define Cmd_QuotServerTimeResetEvent_Req					MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0008)	//�������鶨ʱ���������¼�����
#define Cmd_QuotServerTimeResetEvent_Rsp					MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0009)	//�������鶨ʱ���������¼�����
#define Cmd_QryQuotServerTimeEvent_Req					    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x000A)	//��ѯ�������鶨ʱ���������¼�����
#define Cmd_QryQuotServerTimeEvent_Rsp					    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x000B)	//��ѯ�������鶨ʱ���������¼�����
#define Cmd_QuotSvr_Login_Req                               MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x000C)	//��¼�������������
#define Cmd_QuotSvr_Login_Rsp                               MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x000D)	//��¼�������������
#define Cmd_SubscribeQuotEvent2_Req						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x000E)	//��֤�û����������¼�����
#define Cmd_SubscribeQuotEvent2_Rsp						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x000F)	//��֤�û����������¼�����
#define Cmd_QueryQuotKLineData_Req						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0010)	//��ѯ����K���¼�����
#define Cmd_QueryQuotKLineData_Rsp						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0011)	//��ѯ����K���¼�����
#define Cmd_QueryQuotTimeLineData_Req						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0012)	//��ѯ�����ʱ���¼�����
#define Cmd_QueryQuotTimeLineData_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0013)	//��ѯ�����ʱ���¼�����
#define Cmd_UnSubscribeQuotEvent2_Req					    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0014)	//��֤�û��˶������¼�����
#define Cmd_UnSubscribeQuotEvent2_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0015)	//��֤�û��˶������¼�����
#define Cmd_QueryVolatilityData_Req                         MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0016)	//��ѯ����������
#define Cmd_QueryVolatilityData_Rsp                         MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0017)	//��ѯ�����ʷ���
#define Cmd_QuerySettlementPrice_Req                        MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0018)	//��ѯ���������
#define Cmd_QuerySettlementPrice_Rsp                        MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0019)	//��ѯ����۷���


//ģ���������������
#define Cmd_QrySimulateDay_Req					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0011)	//��ѯģ�⽻��������
#define Cmd_QrySimulateDay_Rsp					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0012)	//��ѯģ�⽻���շ���
#define Cmd_QrySimulatePlayMode_Req				            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0013)	//��ѯģ��طŷ�ʽ����
#define Cmd_QrySimulatePlayMode_Rsp				            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0014)	//��ѯģ��طŷ�ʽ����

#define Cmd_SimulatePlay_Req					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0015)	//��ʼģ������
#define Cmd_SimulatePlay_Rsp					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0016)	//��ʼģ�ⷵ��
#define Cmd_SimulatePause_Req					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0017)	//��ͣģ������
#define Cmd_SimulatePause_Rsp					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0018)	//��ͣģ�ⷵ��
#define Cmd_SimulateStop_Req					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0019)	//ֹͣģ������
#define Cmd_SimulateStop_Rsp					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x001A)	//ֹͣģ�ⷵ��
#define Cmd_SimulateApply_Req					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x001B)	//��ʼģ������Ӧ������
#define Cmd_SimulateApply_Rsp					            MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x001D)	//��ʼģ������Ӧ�÷���

#define Cmd_QueryQuotDataEvent_Req						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0020)	//��ѯ������ʷ�����¼�����
#define Cmd_QueryQuotDataEvent_Rsp						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0021)	//��ѯ������ʷ�����¼�����

//���·�����
#define Cmd_RM_Config_Min									MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0099)
#define Cmd_RM_Config_Max									MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0999)
#define Cmd_RM_QryOrganization_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0100)	//��ѯ���в�����Ϣ����
#define Cmd_RM_QryOrganization_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0102)	//�������в�����Ϣ

#define Cmd_RM_QryUser_Req								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0103)	//��ѯUser��Ϣ����
#define Cmd_RM_QryUser_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0104)	//��ѯUser��Ϣ����

#define Cmd_RM_QryUserAndOrgRelation_Req					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0105)	//��ѯ����Ա�Ͳ��ŵĹ�ϵ
#define Cmd_RM_QryUserAndOrgRelation_Rsp					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0106)	//��ѯ����Ա�Ͳ��ŵĹ�ϵ��Ϣ����

#define Cmd_RM_QryTraderAndTradeAccountRelation_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0107)	//����Ա��ί�н����˺ŵĹ�ϵ
#define Cmd_RM_QryTraderAndTradeAccountRelation_Rsp		    MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0108)	//����Ա��ί�н����˺ŵĹ�ϵ��Ϣ����

#define Cmd_RM_QryProducts_Req								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0109)	//��ѯ����Ʒ��
#define Cmd_RM_QryProducts_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x010a)	//��ѯ����Ʒ�ַ���
#define Cmd_RM_QryResponse_Req								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x010b)	//��ѯ��������
#define Cmd_RM_QryResponse_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x010c)	//��ѯ��������

#define Cmd_RM_SetLimitTrade_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x010d)	//�������ƽ�������
#define Cmd_RM_SetLimitTrade_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x010e)	//�������ƽ��׷���


#define Cmd_RM_SetManualVerify_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x010f)	//�����ֶ��������
#define Cmd_RM_SetManualVerify_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0111)	//�����ֶ���˷���



//��̨�ն��޸ĺ󾭹���ط��������͹���
#define Cmd_RM_SetLimitAndVerify_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0112)	//�������ƽ��׺��ֶ���˷���

#define Cmd_RM_AddRiskPlan_Req								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0135)	//���ӷ�ط���
#define Cmd_RM_AddRiskPlan_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0136)    //���ӷ�ط�������
#define Cmd_RM_UseRiskPlanAdd_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0137)	//�Ƿ����÷�ط���
#define Cmd_RM_UseRiskPlanAdd_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0138)    //�Ƿ����÷�ط�������
#define Cmd_RM_DeleteRiskPlan_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0139)	//ɾ����ط���
#define Cmd_RM_DeleteRiskPlan_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x013a)    //ɾ����ط�������//��ؿͻ����������
#define Cmd_RM_QueryRiskPlan_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x013b)	//��ѯ��ط���
#define Cmd_RM_QueryRiskPlan_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x013c)    //��ѯ��ط�������

#define Cmd_RM_CONTRACTSET_Req								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x013d)	 //��ͬ�������������
#define Cmd_RM_CONTRACTSET_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x013e)    //��ͬ������������󷵻�

#define Cmd_RM_CONTRACTSET_QUERY_Req						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x013f)	 //��ͬ��������������ѯ
#define Cmd_RM_CONTRACTSET_QUERY_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0141)    //��ͬ��������������ѯ����


#define Cmd_RM_ModifyPassword_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0125)	//�޸���������
#define Cmd_RM_ModifyPassword_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0126)	//�޸�������Ӧ
#define Cmd_RM_UnLockWindow_Req							    MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0127)	//������������
#define Cmd_RM_UnLockWindow_Rsp							    MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x0128)	//����������Ӧ


//����ָ����ؽӿ�
#define Cmd_RM_RiskIndicator_Min							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1001)
#define Cmd_RM_RiskIndicator_Max							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1020)

#define Cmd_RM_AddRiskIndicator_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1002)	//��ӷ���ָ������
#define Cmd_RM_AddRiskIndicator_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1003)	//��ӷ���ָ�귵��
#define Cmd_RM_DelRiskIndicator_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1004)	//ɾ������ָ������
#define Cmd_RM_DelRiskIndicator_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1005)	//ɾ������ָ�귵��
#define Cmd_RM_ModifyRiskIndicator_Req						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1006)	//�޸ķ���ָ������
#define Cmd_RM_ModifyRiskIndicator_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1007)	//�޸ķ���ָ�귵��
#define Cmd_RM_QryRiskIndicator_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1008)	//��ѯ����ָ������
#define Cmd_RM_QryRiskIndicator_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1009)	//��ѯ����ָ�귵��


//����¼�����
#define Cmd_RM_RiskEvent_Min								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1021)
#define Cmd_RM_RiskEvent_Max								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1040)

#define Cmd_RM_SubscribeRiskEvent_Req						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1022)	//���ķ���¼�����
#define Cmd_RM_SubscribeRiskEvent_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1023)	//���ķ���¼�����	
#define Cmd_RM_RiskEvent_Push								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1024)	//����¼�����	
#define Cmd_RM_UnSubscribeRiskEvent_Req						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1025)	//�˶�����¼�����
#define Cmd_RM_UnSubscribeRiskEvent_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1026)	//�˶�����¼�����
#define Cmd_RM_QryRiskEvent_Req								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1027)	//��ѯ�����¼�����
#define Cmd_RM_QryRiskEvent_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1028)	//��ѯ�����¼�����


/*
//��Ϣ��ؽӿ�
#define Cmd_RM_Message_Min									MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1041)
#define Cmd_RM_Message_Max									MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1060)

#define Cmd_RM_AddMessage_Req								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1042)	//�����Ϣ����
#define Cmd_RM_AddMessage_Rsp								MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1043)	//�����Ϣ��Ӧ
#define Cmd_RM_QrySendMessage_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1044)	//��ѯ��Ϣ���󣬲�ѯ�ѷ��͵���Ϣ
#define Cmd_RM_QrySendMessage_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1045)	//��ѯ��Ϣ����
#define Cmd_RM_QryMsgSendStatus_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1046)	//��ѯ��Ϣ����״̬����
#define Cmd_RM_QryMsgSendStatus_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1047)	//��ѯ��Ϣ����״̬����
#define Cmd_RM_QryUnReadMessage_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1048)	//��ѯδ�Ķ�����Ϣ
#define Cmd_RM_SendUnReadMessage_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1049)	//����δ�Ķ�����Ϣ
#define Cmd_RM_SendUnReadMessage_Push						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1050)	//����δ�Ķ�����Ϣ
#define Cmd_RM_QryRecvMessage_Req							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1051)	//��ѯ���յ���Ϣ����
#define Cmd_RM_QryRecvMessage_Rsp							MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1052)	//�������յ���Ϣ
*/

//����							
#define	Cmd_RM_SubscribeQuot_Min			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1061)		
#define	Cmd_RM_SubscribeQuot_Max			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1080)			
#define	Cmd_RM_SubscribeQuot_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1062)	//������������		
#define	Cmd_RM_SubscribeQuot_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1063)	//�������鷵��
#define Cmd_RM_Quot_Push					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1064)	//��������		
#define	Cmd_RM_UnSubscribeQuot_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1065)	//�˶���������		
#define	Cmd_RM_UnSubscribeQuot_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1066)	//�˶����鷵��									
//����� ������û�г����ĸ���
/*
#define	Cmd_RM_SubscribeDeposit_Min			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1081)			
#define	Cmd_RM_SubscribeDeposit_Max			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a0)			
#define	Cmd_RM_SubscribeDeposit_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1082)	//���ĳ��������		
#define	Cmd_RM_SubscribeDeposit_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1083)	//���ĳ���𷵻�		
#define	Cmd_RM_Deposit_Push					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1084)	//���������		
#define	Cmd_RM_UnSubscribeDeposit_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1085)	//�˶����������		
#define	Cmd_RM_UnSubscribeDeposit_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1086)	//�˶�����𷵻�		
#define	Cmd_RM_QryTraderDeposit_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1087)//��ѯ���������		
#define	Cmd_RM_QryTraderDeposit_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1088)//��ѯ����𷵻�								
*/
//�ɽ�							
#define	Cmd_RM_SubscribeTrade_Min			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a1)				
#define	Cmd_RM_SubscribeTrade_Max			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c0)				
#define	Cmd_RM_SubscribeTrade_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a2)		//���ĳɽ�����		
#define	Cmd_RM_SubscribeTrade_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a3)		//���ĳɽ�����		
#define	Cmd_RM_Trade_Push					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a4)		//�ɽ�����		
#define	Cmd_RM_UnSubscribeTrade_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a5)		//�˶��ɽ�����		
#define	Cmd_RM_UnSubscribeTrade_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a6)		//�˶��ɽ�����	
//#define Cmd_RM_QryHistroyTrade_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a7)		//��ѯ��ʷ�ɽ�����
//#define Cmd_RM_QryHistroyTrade_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10a8)		//��ѯ��ʷ�ɽ�����

//�ʽ�							
#define	Cmd_RM_SubscribeFund_Min			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c1)				
#define	Cmd_RM_SubscribeFund_Max			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e0)				
#define	Cmd_RM_SubscribeFund_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c2)		//�����ʽ�����		
#define	Cmd_RM_SubscribeFund_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c3)		//�����ʽ𷵻�		
#define	Cmd_RM_Fund_Push					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c4)		//�ʽ�����		
#define	Cmd_RM_UnSubscribeFund_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c5)		//�˶��ʽ�����		
#define	Cmd_RM_UnSubscribeFund_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c6)		//�˶��ʽ𷵻�
//#define Cmd_RM_QryHistroyRiskFund_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c7)		//��ѯ��ʷ�ʽ�����
//#define Cmd_RM_QryHistroyRiskFund_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10c8)		//��ѯ��ʷ�ʽ𷵻�

//�ֲ�							
#define	Cmd_RM_SubscribePosition_Min		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e1)			
#define	Cmd_RM_SubscribePosition_Max		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1100)			
#define	Cmd_RM_SubscribePosition_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e2)	//���ĳֲ�����		
#define	Cmd_RM_SubscribePosition_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e3)	//���ĳֲַ���		
#define	Cmd_RM_Position_Push				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e4)	//�ֲ�����		
#define	Cmd_RM_UnSubscribePosition_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e5)	//�˶��ֲ�����		
#define	Cmd_RM_UnSubscribePosition_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e6)	//�˶��ֲַ���	
//#define Cmd_RM_QryHistroyPosition_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e7)	//��ѯ��ʷ�ֲ�����
//#define Cmd_RM_QryHistroyPosition_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x10e8)	//��ѯ��ʷ�ֲַ���

//����							
#define	Cmd_RM_SubscribeOrder_Min			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1101)			
#define	Cmd_RM_SubscribeOrder_Max			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1120)		
#define	Cmd_RM_SubscribeOrder_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1102)	//���ı�������		
#define	Cmd_RM_SubscribeOrder_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1103)	//���ı�������		
#define	Cmd_RM_Order_Push					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1104)	//��������		
#define	Cmd_RM_UnSubscribeOrder_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1105)	//�˶���������		
#define	Cmd_RM_UnSubscribeOrder_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1106)	//�˶���������
#define Cmd_RM_QryHistroyOrder_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1107)	//��ѯ��ʷ��������
#define Cmd_RM_QryHistroyOrder_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1108)	//��ѯ��ʷ��������
#define Cmd_RM_VerifyOrder_Req				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1109)	//δ��˵���˽������
#define Cmd_RM_VerifyOrder_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x110a)	//δ��˵���˽������
#define Cmd_RM_QryVerifyOrder_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x110b)	//������˽����ѯ����
#define Cmd_RM_QryVerifyOrder_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x110c)	//������˽����ѯ����
//����ֵ
#define	Cmd_RM_FundNet_Min					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1121)			
#define	Cmd_RM_FundNet_Max					MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1140)
#define	Cmd_RM_AddFundNetParam_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1122)	//�������û���ֵ����
#define	Cmd_RM_AddFundNetParam_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1123)	//���û���ֵ��������
#define	Cmd_RM_QueryFundNetParam_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1124)	//�����ѯ���û���ֵ����������Ϊί�н����˺�ID
#define	Cmd_RM_QueryFundNetParam_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1125)	//��ѯ����ֵ��������
#define	Cmd_RM_QueryFundNetCalcResult_Req	MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1126)	//�����ѯ����ֵ������
#define	Cmd_RM_QueryFundNetCalcResult_Rsp	MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1127)	//��ѯ����ֵ����������
#define	Cmd_RM_FundNetValue_push			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1128)	//����ֵ����

#define	Cmd_RM_ForceCloseDo_Min				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1141)
#define	Cmd_RM_ForceCloseDo_Max				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1160)
#define	Cmd_RM_ForceCloseOrderInsert_Req	MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1142)	//����ǿƽ����¼��
#define	Cmd_RM_ForceCloseOrderInsert_Rsp	MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1143)	//ǿƽ����¼�뷵��
#define	Cmd_RM_QryForceCloseOrder_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1144)	//�����ѯǿƽ����¼��
#define	Cmd_RM_QryForceCloseOrder_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1145)	//ǿƽ��ѯ����¼�뷵��
#define	Cmd_RM_OrderAction_Req				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1146)	//���󳷵�¼��
#define	Cmd_RM_OrderAction_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1147)	//���󳷵�¼�뷵��


#define	Cmd_RM_QryAdmin_Min				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1161)    //�����ն˲�ѯ������ʼ
#define	Cmd_RM_QryAdmin_Max				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1180)	//�����ն˲�ѯ�������
#define	Cmd_RM_QryHistoryFundInfo_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1162)	//�����ѯ��ʷ�ʽ�
#define	Cmd_RM_QryHistoryFundInfo_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1163)	//ǿƽ��ѯ��ʷ�ʽ𷵻�
#define	Cmd_RM_QryHistoryPosition_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1164)	//�����ѯ��ʷ�ֲ�
#define	Cmd_RM_QryHistoryPosition_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1165)	//ǿƽ��ѯ��ʷ�ֲַ���
#define	Cmd_RM_QryHistoryPositionDetail_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1166)	//�����ѯ��ʷ�ֲ���ϸ�ֲ�
#define	Cmd_RM_QryHistoryPositionDetail_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1167)	//ǿƽ��ѯ��ʷ�ֲ���ϸ����
#define	Cmd_RM_QryHistoryTrade_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1168)	//�����ѯ��ʷ�ɽ�
#define	Cmd_RM_QryHistoryTrade_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1169)	//ǿƽ��ѯ��ʷ�ɽ�����
#define	Cmd_RM_QryHistoryOrders_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x116a)	//�����ѯ��ʷ����
#define	Cmd_RM_QryHistoryOrders_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x116b)	//ǿƽ��ѯ��ʷ��������
#define	Cmd_RM_QryHistoryFundInOut_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x116c)	//�����ѯ��ʷ�����
#define	Cmd_RM_QryHistoryFundInOut_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x116d)	//ǿƽ��ѯ��ʷ����𷵻�

#define	Cmd_RM_ToExecute_Min						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1300)    //����ִ��������ʼ
#define	Cmd_RM_ToExecute_Max						MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1380)	//����ִ���������
#define	Cmd_RM_ToExecute_OrderAction_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1301)	//������ִ�� ���󳷵�¼��
#define	Cmd_RM_ToExecute_OrderAction_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1302)	//������ִ�� ���󳷵�¼�뷵��
#define Cmd_RM_ToExecute_VerifyOrder_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1303)	//������ִ�� δ��˵���˽������
#define Cmd_RM_ToExecute_VerifyOrder_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1304)	//������ִ�� δ��˵���˽������
#define	Cmd_RM_ToExecute_ForceCloseOrderInsert_Req	MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1305)	//������ִ�� ����ǿƽ����¼��
#define	Cmd_RM_ToExecute_ForceCloseOrderInsert_Rsp	MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1306)	//������ִ�� ǿƽ����¼�뷵��
#define	Cmd_RM_ToExecute_RiskForce_Req				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1307)	//������ִ�� �µ�����
#define	Cmd_RM_ToExecute_RiskForce_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1308)	//������ִ�� �µ����Ʒ���

#define Cmd_RM_Master_SetLimitTrade_Req				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x1309)	//ת�������� �������ƽ�������
#define Cmd_RM_Master_SetLimitTrade_Rsp				MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x130a)	//ת�������� �������ƽ��׷���
#define Cmd_RM_Master_SetManualVerify_Req			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x130b)	//ת�������� �����ֶ��������
#define Cmd_RM_Master_SetManualVerify_Rsp			MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x130c)	//ת�������� �����ֶ���˷���
#define Cmd_RM_Master_ModifyUserPassword_Req		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x130d)	//ת�������� �޸��û���������
#define Cmd_RM_Master_ModifyUserPassword_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_RISK,0x130e)	//ת�������� �޸��û����뷵��

//ͳ��չ��ʹ��CMD
#define	Cmd_RM_SubscribeStatistics_Req		MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0001)	 //����ͳ������
#define	Cmd_RM_SubscribeStatistics_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0002)	 //����ͳ�Ʒ���
#define	Cmd_RM_Statistics_Push				MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0003)  //����ͳ������
#define	Cmd_RM_UnSubscribeStatistics_Req	MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0004)  //�˶�����
#define	Cmd_RM_UnSubscribeStatistics_Rsp	MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0005)  //�˶�����
#define	Cmd_RM_SubscribeUserFund_Req		MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0006)	 //�����ʽ�����		
#define	Cmd_RM_SubscribeUserFund_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0007)	 //�����ʽ𷵻�		
#define	Cmd_RM_UserFund_Push				MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0008)	 //�ʽ�����		
#define	Cmd_RM_UnSubscribeUserFund_Req		MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x0009)  //�˶�����
#define	Cmd_RM_UnSubscribeUserFund_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x000a)  //�˶�����
#define	Cmd_RM_QryAvilabeInstrument_Req		MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x000b)  //��ѯ���еĺ�Լ
#define	Cmd_RM_QryAvilabeInstrument_Rsp		MAKE_CMDID(CMDID_BASE_HIWORD_STATIC_PRESENTATION,0x000c)  //��ѯ���õĺ�Լ����

//������ص�CMD
#define CMD_STRTEGY_UPLOAD_Base_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0001)  //�ϴ�����
#define CMD_STRTEGY_UPLOAD_Base_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0002)  //�ϴ�����
//��Ӧ�ṹ��SStrategy


#define CMD_STRTEGY_UPLOAD_STRATEGY2INDEX_Base_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0003)  //�ϴ����Ժ�ָ���ϵ����
#define CMD_STRTEGY_UPLOAD_STRATEGY2INDEX_Base_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0004)  //�ϴ����Ժ�ָ���ϵ����
//�ϴ�һ��std::vector<SStrategy2Index>������

#define CMD_STRTEGY_UPLOAD_File_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0007)  //�ϴ�����
#define CMD_STRTEGY_UPLOAD_File_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0008)  //�ϴ�����
//�ڶ����ϴ����� int char szName[25]	char szUploader[25];	dll�ļ�����������; (int 2 ��ʾ�ϴ�dll)
//�������ϴ����� int char szName[25]	char szUploader[25];	zip�ļ�����������; (int 3 ��ʾ�ϴ�zip)

#define CMD_STRTEGY_DOWNLOAD_All_Base_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x00010)  //�������в�������
#define CMD_STRTEGY_DOWNLOAD_All_Base_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x00011)  //�������в��Է���
//���������char szUploader[25]   ���Ϊ��ָ�룬��������в���
//���ض�Ӧ�ṹ��vector<SStrategy>


#define CMD_STRTEGY_DOWNLOAD_Base_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x00012)  //�ϴ�����
#define CMD_STRTEGY_DOWNLOAD_Base_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x00013)  //�ϴ�����
//char szName[25]   ���ض�Ӧ�ṹ��SStrategy

#define CMD_STRTEGY_DOWNLOAD_Req				MAKE_CMDID(CMDID_BASE_STRATEDY,0x0015)  //��������
#define CMD_STRTEGY_DOWNLOAD_Rsp				MAKE_CMDID(CMDID_BASE_STRATEDY,0x0016)  //���ط���
//int char szName[25] char szUploader[25] int 2��ʾ �������dll��szNameΪ������
//int char szName[25] char szUploader[25] int 3��ʾ �������zip��szNameΪ������

//����  int  char szName[25]	char szUploader[25];	dll�ļ�����������; //(int 2 ��ʾ�ϴ�dll)(int 3 ��ʾ�ϴ�zip)

#define CMD_STRTEGY_DEL_Req				MAKE_CMDID(CMDID_BASE_STRATEDY,0x0027)  //ɾ����������
#define CMD_STRTEGY_DEL_Rsp				MAKE_CMDID(CMDID_BASE_STRATEDY,0x0028)  //ɾ�����Է���
//SDelStrategy


//ָ����ص�CMD
#define CMD_STRTEGY_UPLOADINDEX_Base_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0101)  //�ϴ�����
#define CMD_STRTEGY_UPLOADINDEX_Base_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0102)  //�ϴ�����
//SIndex

#define CMD_STRTEGY_UPLOADINDEX_File_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0103)  //�ϴ�����
#define CMD_STRTEGY_UPLOADINDEX_File_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0104)  //�ϴ�����
//�ڶ����ϴ����� int char szName[25]	char szUploader[25];	dll�ļ�����������; (int 2 ��ʾ�ϴ�dll)

#define CMD_STRTEGY_DOWNLOADINDEX_Base_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x00112)  //��������
#define CMD_STRTEGY_DOWNLOADINDEX_Base_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x00113)  //���ط���
//char szName[25] ���ض�Ӧ�ṹ��SIndex

#define CMD_STRTEGY_DOWNLOADINDEX_Req				MAKE_CMDID(CMDID_BASE_STRATEDY,0x0115)  //��������
#define CMD_STRTEGY_DOWNLOADINDEX_Rsp				MAKE_CMDID(CMDID_BASE_STRATEDY,0x0116)  //���ط���
//int char szName[25] char szUploader[25] int 2��ʾ �������dll��szNameΪ������

#define CMD_STRTEGY_DOWNLOAD_ALLINDEX_Base_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x00117)  //��������ָ����Ϣ����
#define CMD_STRTEGY_DOWNLOAD_ALLINDEX_Base_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x00118)  //��������ָ����Ϣ���󷵻�
//char szUploader[25]  ����ĳ������Ա��  ���Ϊ��ָ�룬���������ָ��
//���ض�Ӧ�ṹ��vector<SIndex>�����е��Լ���ָ��ͱ��˹����ָ��

#define CMD_STRTEGY_USE_Req						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0119)  //�������ú�ͣ������
#define CMD_STRTEGY_USE_Rsp						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0120)  //�������ú�ͣ�÷���
//SUseStrategy 


#define CMD_INSTANCE_ADD_Req						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0139)  //���Է�����������
#define CMD_INSTANCE_ADD_Rsp						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0130)  //���Է����������󷵻�
//SStrategyInstance

#define CMD_INSTANCE_Modify_Req						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0131)  //���Է����޸�����
#define CMD_INSTANCE_Modify_Rsp						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0132)  //���Է����޸����󷵻�
//SStrategyInstance

#define CMD_INSTANCE_DEL_Req						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0133)  //���Է���ɾ������
#define CMD_INSTANCE_DEL_Rsp						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0134)  //���Է���ɾ�����󷵻�
//SDelStrategy

#define CMD_INSTANCE_USE_Req						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0138)  //���Է������ú�ͣ������
#define CMD_INSTANCE_USE_Rsp						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0139)  //���Է������ú�ͣ�÷���
//SUseStrategy 

#define CMD_INSTANCE_AllDownlaod_Req				MAKE_CMDID(CMDID_BASE_STRATEDY,0x0140)  //�������в��Է�������
#define CMD_INSTANCE_AllDownlaod_Rsp				MAKE_CMDID(CMDID_BASE_STRATEDY,0x0141)  //�������в��Է�������
//����char Uploader[25];
//����std::vector<SStrategyInstance>& vecInstance

#define CMD_UPLOAD_START_Req						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0170)  //��ʼ�ϴ�����
#define CMD_UPLOAD_START_Rsp						MAKE_CMDID(CMDID_BASE_STRATEDY,0x0171)  //��ʼ�ϴ����󷵻�
//��������� UploadStart
//���أ���

#define CMD_UPLOAD_END_Req							MAKE_CMDID(CMDID_BASE_STRATEDY,0x0172)  //�ϴ���������
#define CMD_UPLOAD_END_Rsp							MAKE_CMDID(CMDID_BASE_STRATEDY,0x0173)  //�ϴ��������󷵻�
//��������� UploadEnd
//���أ���

/*
1.  �ϴ�һ��ָ�꣨�û�����ָ������ָ��dll��ָ��Դ����zip��˽��/����
2.  �ϴ�һ�����ԣ��û�����������������dll������Դ����zip��˽��/����
3.  ����(˽��/����)ָ���б�ָ�������飩
4.  ����(˽��/����)�����б����������飩
5.  ����һ��(˽��/����)ָ��dll
6.  ����һ��(˽��/����)����dll
7.  ����һ��˽��ָ��Դ����zip
7.  ����һ��˽�в���Դ����zip

*/

#define CMD_QUERY_COMMISSION_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0182)  //��ѯ��Լ��������
#define CMD_QUERY_COMMISSION_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0183)  //��ѯ��Լ�������ʷ���
//�������QueryCommission
//���أ�PlatformStru_InstrumentCommissionRate

#define CMD_QUERY_MARGINRATE_Req					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0184)  //��ѯ��Լ��֤����
#define CMD_QUERY_MARGINRATE_Rsp					MAKE_CMDID(CMDID_BASE_STRATEDY,0x0185)  //��ѯ��Լ��֤���ʷ���
//�������QueryMarginRate
//���أ�PlatformStru_InstrumentMarginRate


//֪ͨ����ѯģ��ʹ��CMD
#define  CMDID_NotifyAndAsk_BroadcastNotify			MAKE_CMDID(CMDID_BASE_NotifyAndAsk,0x0001)	//֪ͨ��Ϣ�㲥������������
