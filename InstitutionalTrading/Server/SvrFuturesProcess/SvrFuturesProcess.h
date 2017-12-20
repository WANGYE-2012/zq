#pragma once

#ifndef SVRFUTURESPROCESS_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRQUOTSERVER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRFUTURESPROCESS_EXPORTS
#define SVRFUTURESPROCESS_API __declspec(dllexport)
#else
#define SVRFUTURESPROCESS_API __declspec(dllimport)
#endif
#endif

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#pragma warning(disable : 4101)


#include <queue>
#include <map>
#include <vector>
using namespace std;

#include "CommonPkg.h"
#include "CommonMacro.h"
#include "CommonStruct.h"
#include "MyXML.h"
#include "easymutex.h"
#include "Tools_Ansi.h"
#include "..\\SvrDBOpr\\Interface_SvrDBOpr.h"

#ifdef _DEBUG
#pragma comment(lib, "Tools4dllD.lib")	
#pragma comment(lib, "SvrDBOpr.lib")										
#else 
#pragma comment(lib, "Tools4dll.lib")	
#pragma comment(lib, "SvrDBOpr.lib")
#endif

#include "FileOpr.h"
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","PlatformCTPQuot", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","EndOfFile")

//����ṹ��
struct STU
{
	char snum[10];
	char sname[10];
	char ssex[2];
	int  sage;
	char smajor[20];
};
//KLine�ṹ��
struct KLine
{
	string		szExchange;
	string		szInstrumentID;
	EnumPhrase	ePhrase;
	KLineBase	sKLine;
};


void	LoadConfig(void);
bool	GetQuotFromQueue(PlatformStru_DepthMarketData& outData);
bool	GetKLineFromQueue(vector<KLine>& vData);
bool	RecalcWeekMonthKLine(void);
void	SetDataInTime(PlatformStru_DepthMarketData* pData);
bool	SetInTimeData_KLine(PlatformStru_DepthMarketData* pData);
bool	SetInTimeData_DiffSpan(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase,string exchangeID);
bool	SetInTimeData_MINS(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine, int nTimeSpan ,vector<struct tm>& vecTimeScales);
bool    SetInTimeData_Day(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine,vector<struct tm>& vecTimeScales);
bool	SetInTimeData_Week(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine,vector<struct tm>& vecTimeScales);
bool	SetInTimeData_Month(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine,vector<struct tm>& vecTimeScales);

bool	SaveKline2Cache(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase);
bool	SaveKline2DB(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase,string& exchangeID);

bool	UpdateKLineData(string& strInstrumentID, EnumPhrase phrase, KLineBase& pKLine, bool bInsert, int& nErrorCode);

string MakeTableName(string& strInstrumentID, EnumPhrase phrase);

struct	tm GetTimeInSpan(struct tm tmPara, int nSpan,vector<struct tm>& vecTimeScales);
bool	IsOneWeek(struct tm tm1, struct tm tm2);
bool	IsOneMonth(struct tm tm1, struct tm tm2);
int		GetDayOfYear(struct tm tm1);


//��ȡSQLServer DB �߳� 
HANDLE	hReadSQLDBThread=NULL;	
DWORD	idReadSQLDBThread=0;
DWORD	ReadSQLDBThreadProc(void);

//����KLine�߳�
HANDLE	hCaculateKLineThread=NULL;	
DWORD	idCaculateKLineThread=0;
DWORD	CaculateKLineThreadProc(void);

//��K�����ݱ��ؿ��߳�
HANDLE	hSaveDBThread=NULL;	
DWORD	idSaveDBThread=0;
DWORD	SaveDBThreadProc(void);

//�����߳��˳���ʶ
bool	bQuit = false;

//������ �ֱ�����������
Ceasymutex	mutexQuot;
Ceasymutex	mutexKLine;
Ceasymutex  m_mutexKLineM1,m_mutexKLineM5,m_mutexKLineM15,m_mutexKLineM30,m_mutexKLineM60,m_mutexKLineDay,m_mutexKLineWeek,m_mutexKLineMonth;

//�������зֱ�洢��ѯ������ͼ�����K��
queue<PlatformStru_DepthMarketData>			queueQuot;
queue<KLine>								queueKLine;

//K��
string		m_strTradingDay;		//���潻����
int			m_nTimeSpan;			//�գ��ܡ���K�ߴ洢���
struct		tm m_tmCloseTime;
struct		tm m_LastDBTimeDay;
struct		tm m_LastDBTimeWeek;
struct		tm m_LastDBTimeMonth;

set<string>							m_setTableName;		// �Ѵ���KLine�洢�������

map<string,vector<struct tm>>		m_mapExchangeTime;
map<string,KLineBase>				m_mapKLineLast;

map<string,KLineBase >				m_maptmLast1;	//��һ��K��
map<string,KLineBase >				m_maptmLast5;
map<string,KLineBase >				m_maptmLast15;
map<string,KLineBase >				m_maptmLast30;
map<string,KLineBase >				m_maptmLast60;
map<string,KLineBase >				m_maptmLastDay;
map<string,KLineBase >				m_maptmLastWeek;
map<string,KLineBase >				m_maptmLastMonth;


map<string,vector<KLineBase>>		m_mapKLineMin1;		//���к�Լ1����K��
map<string,vector<KLineBase>>		m_mapKLineMin5;		//���к�Լ5����K��
map<string,vector<KLineBase>>		m_mapKLineMin15;	//���к�Լ15����K��
map<string,vector<KLineBase>>		m_mapKLineMin30;	//���к�Լ30����K��
map<string,vector<KLineBase>>		m_mapKLineMin60;	//���к�Լ60����K��
map<string,vector<KLineBase>>		m_mapKLineDay;		//���к�Լ��K��
map<string,vector<KLineBase>>		m_mapKLineWeek;		//���к�Լ��K��
map<string,vector<KLineBase>>		m_mapKLineMonth;	//���к�Լ��K��

map<string,map<time_t,KLineBase>>		m_vmapKLineMin1;	//���к�Լ1����K��
map<string,map<time_t,KLineBase>>		m_vmapKLineMin5;	//���к�Լ5����K��
map<string,map<time_t,KLineBase>>		m_vmapKLineMin15;	//���к�Լ15����K��
map<string,map<time_t,KLineBase>>		m_vmapKLineMin30;	//���к�Լ30����K��
map<string,map<time_t,KLineBase>>		m_vmapKLineMin60;	//���к�Լ60����K��
map<string,map<time_t,KLineBase>>		m_vmapKLineDay;		//���к�Լ��K��
map<string,map<time_t,KLineBase>>		m_vmapKLineWeek;	//���к�Լ��K��
map<string,map<time_t,KLineBase>>		m_vmapKLineMonth;	//���к�Լ��K��


