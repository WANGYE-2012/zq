#pragma once
//�������ͷ�ļ��ж���ͻ��������������command ID

//ÿ���ͻ��˷�����������������Ҫ�ڰ�ͷ��userdata1�ĵط���дuserid����½�������

#define  MAKE_CMDID(_base,_offset) \
	((int)( (int)(_base) << 16 ) + (int)(_offset))



#define  CMDID_RM_BASE_HIWORD_SERVER_TCP              0x0000 //ServerTcp�����λCMDID

//-----------------------------------------------------------------------------------
//	���涨�峣��
//-----------------------------------------------------------------------------------
//֪ͨ���ĵ��̣߳�����Ϣ���lParam=EventParamID��EventParam��pEventData�ǰ����ݣ�Stru_UniPkgHead + ���� + ��β, userint=hSocket
#ifndef WndCmd_YourPkgArrival2
#define WndCmd_YourPkgArrival2		0x3bbd
#endif

#ifndef WndCmd_YourEventArrival2
#define WndCmd_YourEventArrival2		0x3bbe
#endif

#ifndef WM_YourEventArrival2
#define WM_YourEventArrival2		0x8f01
#endif

#define MAX_SQL_LENGTH			4000	//SQL�����󳤶�
#define MAX_USABLE_SQL_LENGTH	3999

//-----------------------------------------------------------------------------------
//	���涨��������
//-----------------------------------------------------------------------------------
#define CMDID_Heartbeat					MAKE_CMDID(CMDID_RM_BASE_HIWORD_SERVER_TCP,0x2000)				//�ͻ���ÿ��20s���������������������Ϊ��
#define CMDID_HeartbeatRsp				MAKE_CMDID(CMDID_RM_BASE_HIWORD_SERVER_TCP,0x2001)				//��������ͻ��˷���������Ӧ������Ϊ��

#define CMDID_SvrTcpDisconn				MAKE_CMDID(CMDID_RM_BASE_HIWORD_SERVER_TCP,0x2100)				//��tcp���ӶϿ� pkghead��userdata1Ϊ�Ͽ���hSocket
#define CMDID_SvrTcpCon				    MAKE_CMDID(CMDID_RM_BASE_HIWORD_SERVER_TCP,0x2111)				//��tcp���� pkghead��userdata1Ϊ�Ͽ���hSocket


#define CMDID_SvrGet					MAKE_CMDID(CMDID_RM_BASE_HIWORD_SERVER_TCP,0x3100)				//��ȡ�ļ�
#define CMDID_SvrGetRsp				    MAKE_CMDID(CMDID_RM_BASE_HIWORD_SERVER_TCP,0x3111)				//��ȡ�ļ�����


struct sGet
{
	char mPath[256];
};