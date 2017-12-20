#pragma once

// ͣ����Fasttrader�����е�λ�ö���
#define NEED_NO_POS_NO_BIND		0	// ����Ҫͣ�������󶨴���
#define POS_ANY			1	// ��Ҫͣ��������λ����Ҫ��
#define POS_LEFT		2	// ͣ���ڴ������
#define POS_RIGHT		3	// ͣ���ڴ����ұ�
#define POS_TOP			4	// ͣ���ڴ��ڶ���
#define POS_BOTTOM		5	// ͣ���ڴ��ڵ׶�
#define POS_QUOTE		6	// �ڱ��۱���λ����ʾ
#define NEED_NO_POS_BUT_BIND	7	//����Ҫͣ������Ҫ�󶨴���

#define DEBUG_ON	0	//���Կ���
#define HEARTBEAT_RATE	60	//����ʱ��������λΪ��
// CMDID����
// ������Ϣ������
#define CMDID_CONSOLE_MIN							0x1000
#define CMDID_CONSOLE_MAX							0x1FFF
#define CMDID_CONSOLE_HEARTBEAT						0x1001	//��������
#define CMDID_CONSOLE_HIDE							0x1003	//����
#define CMDID_CONSOLE_LOGON_REQUEST					0x1004	//��¼����
#define CMDID_CONSOLE_LOGON_RSP						0x1005	//��¼��Ӧ
#define CMDID_CONSOLE_CLOSE							0x1006	//Ҫ�����˳�����
#define CMDID_CONSOLE_FASTTRADER_TO_PLUSIN_SHOW		0x1007	//FastTrader�����������Ϣ��Ҫ����������ʾ
#define CMDID_CONSOLE_FASTTRADER_TO_PLUSIN_HIDE		0x1008	//FastTrader�����������Ϣ��Ҫ������������
#define CMDID_CONSOLE_CHANGE_SIZE					0x1009	//�����������FastTrader������Ϣ��Ҫ��ı������ڴ�С״̬
#define CMDID_CONSOLE_WND_SIZE_STATUS_QUERY			0x100A	//��ȡ��ǰ���ڴ�С״̬
#define CMDID_CONSOLE_WND_SIZE_STATUS_RSP			0x100B	//���ص�ǰ���ڴ�С״̬
#define CMDID_CONSOLE_WND_SIZE_STATUS_CHANGED		0x100C	//FastTrade�������������ʹ���״̬�ı���Ϣ

// �˻���Ϣ������
#define CMDID_ACCOUNT_MIN							0x2000
#define CMDID_ACCOUNT_MAX							0x2FFF
#define CMDID_ACCOUNT_SUB_ACCOUNT_LIST_QUERY		0x2001	//��ѯ�������˻�
#define CMDID_ACCOUNT_SUB_ACCOUNT_LIST_RSP			0x2002	//���˻���Ϣ����
#define CMDID_ACCOUNT_SUB_ACCOUNT_INFO_QUERY		0x2003	//��ѯĳ�����˻��ʽ���Ϣ
#define CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP			0x2004	//����ĳ�����˻��ʽ���Ϣ
#define CMDID_ACCOUNT_CURRENT_ACCOUNT_QUERY			0x2005	//��ȡ��ǰ�˺�
#define CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP			0x2006	//���ص�ǰ�˺�
#define CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_QUERY		0x2007	//��ȡ��ǰ���˺�
#define CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_RSP		0x2008	//���ص�ǰ���˺�
#define CMDID_ACCOUNT_REQUERY						0x2009	//FastTrader����������������ʽ����²�ѯ�¼�

// ��Լ��Ϣ������
#define CMDID_INSTRUMENT_MIN						0x3000
#define CMDID_INSTRUMENT_MAX						0x3FFF
#define CMDID_INSTRUMENT_LIST_QUERY					0x3001	//��ѯ�ض��������ĺ�Լ�б�
#define CMDID_INSTRUMENT_LIST_RSP					0x3002	//��ѯ�ض��������ĺ�Լ�б���Ӧ
#define CMDID_INSTRUMENT_INFO_QUERY					0x3003	//��ѯ������Լ����
#define CMDID_INSTRUMENT_INFO_RSP					0x3004	//��ѯ������Լ������Ӧ
#define CMDID_INSTRUMENT_SHORTNAME_QUERY			0x3005	//��ȡ��Լ��Ʊ�
#define CMDID_INSTRUMENT_SHORTNAME_RSP				0x3006	//���غ�Լ��Ʊ�

// ί�е���Ϣ������
#define CMDID_ALL_ORDER_MIN							0x4000
#define CMDID_ALL_ORDER_MAX							0x4FFF
#define CMDID_ALL_ORDER_LIST_QUERY					0x4001	//��ѯί�е��б�
#define CMDID_ALL_ORDER_LIST_RSP					0x4002	//��ѯί�е��б���Ӧ
#define CMDID_ALL_ORDER_INFO_QUERY1					0x4003	//��ѯ����ί�е����飬����ΪFTID
#define CMDID_ALL_ORDER_INFO_QUERY2					0x4004	//��ѯ����ί�е����飬����ΪAccount��InstrumentID��SessionID��FrontID��OrderRef
#define CMDID_ALL_ORDER_INFO_RSP					0x4005	//��ѯ����ί�е�������Ӧ
#define CMDID_ALL_ORDER_CURRENT_SEL					0x4006	//FastTrader���������������ί�е�ģ�鵱ǰѡ�е���
#define CMDID_ALL_ORDER_WAIT_ORDER_NUM_QUERY		0x4007	//��ѯδ�ɽ��ҵ�����������Ϊ�û��������ʺš���ԼID��LimitPrice
#define CMDID_ALL_ORDER_WAIT_ORDER_NUM_RSP			0x4008	//����δ�ɽ��ҵ�����
#define CMDID_ALL_ORDER_REQUERY						0x4009	//FastTrader���������������ί�е����²�ѯ�¼�
#define CMDID_ALL_ORDER_WAIT_ORDER_PRICE_QUERY		0x4010	//����ָ����Լδ�ɽ��ҵ�����߹ҵ��۸����͹ҵ��۸�
#define CMDID_ALL_ORDER_WAIT_ORDER_PRICE_RSP		0x4011	//����ָ����Լδ�ɽ��ҵ�����߹ҵ��۸����͹ҵ��۸�

// �ֲ���Ϣ������
#define CMDID_POSITION_MIN							0x5000
#define CMDID_POSITION_MAX							0x5FFF
#define CMDID_POSITION_LIST_QUERY					0x5101	//��ѯ�ֲ��б�
#define CMDID_POSITION_LIST_RSP						0x5102	//��ѯ�ֲ��б���Ӧ
#define CMDID_POSITION_INFO_QUERY1					0x5103	//��ѯ�����ֲ����飬����ΪFTID
#define CMDID_POSITION_INFO_QUERY2					0x5104	//��ѯ�����ֲ����飬����Ϊ��Լ���������״̬
#define CMDID_POSITION_INFO_RSP						0x5105	//��ѯ�����ֲ�������Ӧ
#define CMDID_POSITION_INFO_LIST_QUERY				0x5106	//��ѯ�ֲ����飬����Ϊ�˻������˻�����ԼID
#define CMDID_POSITION_INFO_LIST_RSP				0x5107	//��ѯ�����ֲ�������Ӧ

#define CMDID_POSITION_COMBO_LIST_QUERY				0x5501	//��ѯ��ϳֲ��б�
#define CMDID_POSITION_COMBO_LIST_RSP				0x5502	//��ѯ��ϳֲ��б���Ӧ
#define CMDID_POSITION_COMBO_INFO_QUERY				0x5503	//��ѯ������ϳֲ�����
#define CMDID_POSITION_COMBO_INFO_RSP				0x5504	//��ѯ������ϳֲ�������Ӧ

#define CMDID_POSITION_CURRENT_SEL					0x5A01	//FastTrader��������������ͳֲ�ģ�鵱ǰѡ�е���
#define CMDID_POSITION_DETAIL_CURRENT_SEL			0x5A02	//FastTrader��������������ͳֲ�ģ�鵱ǰѡ�е���
#define CMDID_POSITION_COMBO_CURRENT_SEL			0x5A03	//FastTrader��������������ͳֲ�ģ�鵱ǰѡ�е���
#define CMDID_POSITION_REQUERY						0x5A04	//FastTrader��������������ͳֲ����²�ѯ�¼�
#define CMDID_POSITION_COMBO_REQUERY				0x5A05	//FastTrader�����������������ϳֲ����²�ѯ�¼�

// ���������������
#define CMDID_EXCHANGE_MIN							0x6000
#define CMDID_EXCHANGE_MAX							0x6FFF
#define CMDID_EXCHANGE_LIST_QUERY					0x6001	//��ѯ�������б�
#define CMDID_EXCHANGE_LIST_RSP						0x6002	//��ѯ�������б���Ӧ
#define CMDID_EXCHANGE_CURRENT_TIME_QUERY			0x6003	//��ѯ��������ǰʱ��
#define CMDID_EXCHANGE_CURRENT_TIME_RSP				0x6004	//��ѯ��������ǰʱ����Ӧ
#define CMDID_EXCHANGE_TRADE_TIME_QUERY				0x6005	//��ѯ����������ʱ��
#define CMDID_EXCHANGE_TRADE_TIME_RSP				0x6006	//��ѯ����������ʱ����Ӧ

// �������������
#define CMDID_QUOTE_MIN								0x7000
#define CMDID_QUOTE_MAX								0x7FFF
#define CMDID_QUOTE_SUBSCRIBE						0x7001	//��������
#define CMDID_QUOTE_SUBSCRIBE_RSP					0x7002	//�������鷵��
#define CMDID_QUOTE_SEND_QUOTE						0x7003	//��������
#define CMDID_QUOTE_UNSUBSCRIBE						0x7004	//�����˶�
#define CMDID_QUOTE_CURRENT_GROUP_INST_LIST			0x7006	//��ǰѡ�еĺ�Լ�鷢���仯ʱ,FastTrader�������͵�ǰ��Լ��ĺ�Լ�б�
#define CMDID_QUOTE_CURRENT_SEL_QUERY				0x7007	//���󱨼۱�ǰѡ����
#define CMDID_QUOTE_CURRENT_SEL						0x7008	//FastTrader��������������ͱ��۱�ǰѡ�е���
#define CMDID_QUOTE_SNAPSHOT_QUERY					0x7009	//��ȡ�������
#define CMDID_QUOTE_SNAPSHOT_RSP					0x700A	//������շ���
#define CMDID_QUOTE_PLUSIN_CURRENT_SEL				0x700B	//����ı䵱ǰ��ʾ�ĺ�Լ����FastTrader������Ϣ��Ҫ�󱨼۱�ı䵱ǰѡ�еĺ�Լ
#define CMDID_QUOTE_IS_SIMULATE_QUERY				0x700C	//��ȡFastTrader��ǰ��¼����ģ���̻���ʵ��
#define CMDID_QUOTE_IS_SIMULATE_RSP					0x700D	//����FastTrader��ǰ��¼����ģ���̻���ʵ�̣�1��ģ���� 0��ʵ��
#define CMDID_QUOTE_ADD_TO_SELFGROUP_REQ			0x700E	//����һ����Լ���뵽FastTrader��ѡ��Լ����
#define CMDID_QUOTE_ADD_TO_SELFGROUP_RSP			0x700F	//���ؽ�һ����Լ���뵽FastTrader��ѡ��Լ���еĲ������

// �µ����������
#define CMDID_ORDER_INSERT_MIN						0x8000
#define CMDID_ORDER_INSERT_MAX						0x8FFF
#define CMDID_ORDER_INSERT_ORDER					0x8001	//�µ�����
#define CMDID_ORDER_INSERT_ORDER_RSP				0x8002	//�µ�������Ӧ
#define CMDID_ORDER_CANCEL_ORDER					0x8004	//��������
#define CMDID_ORDER_CANCEL_ORDER_RSP				0x8005	//����������Ӧ
#define CMDID_ORDER_ORDER_STATUS_CHANGE				0x8007	//ί�е�״̬�ı�ʱ������Ϣ
#define CMDID_ORDER_TRADE_RTN						0x8008	//�ɽ��ر�����ʱ����
#define CMDID_ORDER_CANCEL_ORDER_BY_INST			0x8009	//ָ����Լ��������
#define CMDID_ORDER_CANCEL_ORDER_BY_INSTLMTPRICE	0x8010	//ָ����Լ���۸񳷵�����
#define CMDID_ORDER_CURRENT_INSTRUENT_QUERY			0x8011	//��ѯ�µ��嵱ǰ��Լ
#define CMDID_ORDER_CURRENT_INSTRUENT_RSP			0x8012	//�����µ��嵱ǰ��Լ
#define CMDID_ORDER_CANCEL_ORDER_BY_INSTLMTDIRPRICE	0x8013	//ָ����Լ�����򡢼۸񳷵�����
#define CMDID_ORDER_RSP_ORDER_ACTION1				0x8014	//�������󷵻�1
#define CMDID_ORDER_RSP_ORDER_ACTION2				0x8015	//�������󷵻�2
#define CMDID_ORDER_RSP_ORDER_INSERT				0x8016	//�µ����󷵻�

#define CMDID_CONFIG_MIN							0x9000
#define CMDID_CONFIG_MAX							0x9FFF
#define CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_QUERY	0x9001	//��ѯ����������������Լ���롱
#define CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_RSP	0x9002	//���ء���������������Լ���롱
#define CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_QUERY	0x9003	//��ѯ��������������볣��������
#define CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_RSP		0x9004	//���ء�������������볣��������
#define CMDID_CONFIG_INPUT_VOLUMES_QUERY			0x9005	//��ѯ������������9������������ֵ��
#define CMDID_CONFIG_INPUT_VOLUMES_RSP				0x9006	//���ء�����������9������������ֵ��
#define CMDID_CONFIG_CHANGE							0x9007	//FastTrader������������������øı���Ϣ

//�����붨��
#define SUCCESS			0		//��ѯ�ɹ�
#define ERROR_ONE		1		//�������1
#define UNKOWN_ERROR	999		//δ֪����

struct LogonStu
{
	char szAppID[100];
	char szLicense[100];
	char szAppPath[256];
	HWND hPlusinTopWnd;
	int nPos;
	DWORD dwVersion;
};