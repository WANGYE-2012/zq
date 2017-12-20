#pragma once

#ifndef SVRQUOTSERVER_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRQUOTSERVER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRQUOTSERVER_EXPORTS
#define SVRQUOTSERVER_API __declspec(dllexport)
#else
#define SVRQUOTSERVER_API __declspec(dllimport)
#endif
#endif


#include "easymutex.h"
#include "CommonDef.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "..\\SvrDBOpr\\Interface_SvrDBOpr.h"
#include "..\\..\\..\\FastTrader\\inc\\Module-Misc2\\KernelStruct.h"


//������
extern Ceasymutex			g_mutex;
extern Ceasymutex			g_mutexImpl;


#include "FileOpr.h"
#include "tools_util.h"

#if 0
#define LOG_INFO(fmt, ...)  ;
#else
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","PlatformCTPQuot", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif

#define CONFIG_FILE     "quotfunction.ini"
#define SIMULATE_FILE   "simulate.ini"
#define QUOTTABLENAME   "AQUOTINFO"
#define DATETIMESTYLE   "YYYYMMDD HH:MM:SS"

#include "easytime.h"
#include "easytimespan.hpp"

/********************************��˵��******************************************
���������Ϊ��һ����,�������û��뽻����������, 
ͨ���˷���������
1.������գ�������ʵʱ���飩
2.����ת���������û����ĵ����飩
3.���鴦������K�ߣ�
4.������⣨�������ݿ⣩

_SIMULATE_			ģ�����鿪��
_Stock_				֤ȯҵ�񿪹أ�����SvrDBFTransfer����
_DeleteQuot_		ɾ�����������߳̿��أ����ǵ����ش��̵Ĵ洢�ռ䣩
_LoadKlineData_		������ʷK�������߳̿���

ע��
	��ȻbinĿ¼�µ�function.ini�м�����������DLL����������������ϵĺ꣬�ú��Ӧ�Ĺ���Ҳ������ڡ�
*********************************************************************************/