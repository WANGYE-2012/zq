#pragma once
#include "CommonDataChangeEvent.h"
#include "Interface_SvrCommon.h"
#include <map>
#include <queue>
using namespace std;


#include "easymutex.h"
#include "windows.h"


//������
extern Ceasymutex			g_mutex;


//������
extern Ceasymutex			g_mutexEvent;

extern DWORD				g_idThread;

//�����ڲ��¼�֪ͨ���߳�map, cmdid -> idThread��
//�յ��ڲ��¼�֪ͨʱ����PostThreadMessage()���¼������͸����ĵ��̡߳�
//wParam=WndCmd_NotifyEventArrival
//lParam=EventParamID��EventParam��pEventData�ǰ����ݣ�Stru_UniPkgHead + ���� + ��β(4)��userint=hSocket
extern multimap<eEventType,unsigned int> g_mapDataType2Thread;

//��ѯ�ӿ�map
extern multimap<eAskType, CInterface_SvrCommon * const> g_mapDataType2Interface;



