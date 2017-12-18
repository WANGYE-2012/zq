#ifndef _CONFIGERMANAGER_H_
#define _CONFIGERMANAGER_H_

#include "inc\easymutex.h"

#include <string>
#include <map>
#include <vector>

using namespace std;

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CConfigManager
{
public:
	enum {	conMaxPlotCtrlItemNum = 100 };

public:
	static void Load();
	static void LoadExchangeTime();

	static bool IsLoaded(void) { return m_bLoaded; }
	static unsigned int GetUpdateCounter(void) { return m_UpdateCounter; }
	static bool GetSvrAddr(string& SvrIP,unsigned short& SvrPort);
	static bool GetLocalAddr(string& LocalIP,unsigned short& LocalPort);

	static map<string, vector<EXCHANGERESTTIME>>& GetExchangeRestTime() {
		return m_mapRestTime;
	};

	static PLOTCTRLHEADCFG& GetPlotCtrlHead() {
		return m_plotCtrlHead;
	};
	static int GetPlotCtrlItemCount() {
		return m_plotCtrlHead.plotItemCount;
	};
	static PLOTCTRLITEMCFG& GetPlotCtrlItem(int nIndex) {
		return m_plotCtrlItems[nIndex];
	};

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
	static map<string, vector<EXCHANGERESTTIME>> m_mapRestTime;

	static PLOTCTRLHEADCFG	m_plotCtrlHead;
	static PLOTCTRLITEMCFG	m_plotCtrlItems[conMaxPlotCtrlItemNum];
};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif


