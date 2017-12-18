#ifndef _CONFIGERMANAGER_H_
#define _CONFIGERMANAGER_H_

#include "easymutex.h"

#include <string>
using namespace std;

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CConfigManager
{
public:
	static void Load();

	static bool IsLoaded(void) { return m_bLoaded; }
	static unsigned int GetUpdateCounter(void) { return m_UpdateCounter; }
	static bool GetSvrAddr(string& SvrIP,unsigned short& SvrPort);
	static bool GetLocalAddr(string& LocalIP,unsigned short& LocalPort);


public:
	//�Ƿ��Ѿ�����
	static bool m_bLoaded;

	//��������ַ
	static string m_SvrIP;
	static unsigned short m_SvrPort;

	//���ص�ַ�����Բ����ã�������Ϊ�㡣�ڶ�������������Ҫ����
	static string m_LocalIP;
	static unsigned short m_LocalPort;

private:
	static Ceasymutex m_mutex;
	static unsigned int m_UpdateCounter;
};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif


