#pragma once

#ifndef SVRTRADEDATA_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRTRADEDATA_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRTRADEDATA_EXPORTS
#define SVRTRADEDATA_API __declspec(dllexport)
#else
#define SVRTRADEDATA_API __declspec(dllimport)
#endif
#endif


#include "easymutex.h"


//������
extern Ceasymutex			g_mutex;
extern DWORD				g_idThread;
extern DWORD				g_idThreadSubcribe;

//�¼�������֪ͨ�����̵߳���ϢID ����
#define TMESSAGE_TRADEINIT_AFTER_GET_TRADINGDAY 0x0402

//�¼�������֪ͨ�����̵߳���ϢID ����
#define TMESSAGE_TRADEINIT_CHECK_USER_STATUS 0x0403

//�¼�������֪ͨ�����̵߳���ϢID ����
#define TMESSAGE_TRADEINIT_CHECK_USER_CTP_STATUS 0x0404

//�¼�������֪ͨ�����̵߳���ϢID ����
#define TMESSAGE_TRADING_BUFFER_STORE 0x0405

//�¼�������֪ͨ�����̵߳���ϢID ����
#define TMESSAGE_TRADING_END_USER_CHECK 0x0406

//�¼�������֪ͨ�����̵߳���ϢID ����
#define TMESSAGE_TRADEINIT_CHECK_USER_ACCOUNT_SYNC 0x0407


//�¼�������֪ͨ�����̵߳���ϢID ����
#define TMESSAGE_START_SUBCRIBE_CACHE_CHANNEL 0x0408

//�¼�������֪ͨ�����̵߳���ϢID ����
#define TMESSAGE_START_DEAL_CACHE_CHANNEL_COMMAND 0x0409

#define  ERROR_POSITION_FORMAT "�û�%s��һ�����ڵ�%s��Լ�ĳֲ�,�ֲַ���Ϊ%s,�ֲ�����Ϊ%d,���ڽ���ʱ��Ӹú�Լ�ֲֵķ���ɽ���ȥ���óֲ֣���Ȼ�������޷���ʼ���ɹ�"