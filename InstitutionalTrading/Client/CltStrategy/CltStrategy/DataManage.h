#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include "CommonStruct.h"
#include "..\CommonClasses\IndicatorDataEx.h"
#include "..\CommonClasses\StrategyDataEx.h"
using namespace std;

class CDataManage
{
protected:
	CDataManage(void);
	~CDataManage(void);

public:
	static CDataManage* NewInstance();
	static void DestroyInstance();
	static CDataManage* m_pInstance;

public:
	void Clear();
 //   /*�½�ָ������������ʱ���������Ƿ��ظ�*/
	//bool CheckName(const std::string& name);
	bool CheckIndicatorName(const std::string& name);
	bool CheckStrategyName(const std::string& name);
	//static const char* IndiPara2String(IndicatorParaType eType);
	//static IndicatorParaType IndiPara2Enum( const string& eType);
	//*����һ��ָ��*/
     void AddIndcator(CIndicatorDataEx& data);
	void AddDownloadIndicator(CIndicatorDataEx& data);
	///*ɾ��һ��ָ��*/
     void DeleteIndcator( const string& name);
	 /*�õ�һ��ָ��*/
	 bool GetIndicator( const string& name,CIndicatorDataEx& data);
	 bool GetDownloadIndicator( const string& name,CIndicatorDataEx& data);
	 bool IsIndicatorExist(const string& name);
	 bool IsInStrategyExist(const string& name);
	//static IndicatorType IndiType2Enum( const string& eType);
	//static IndicatorFile IndiFile2Enum( const string& eType);
	///*�½�ָ��ʱ���ָ�����MAP*/
	//void ClearIndiPara();
	/*�õ�һ��ָ������в���*/
	bool GetIndiPara(const string& IndiName,std::map<std::string, IndicatorPara> &  lMapFiled);
	//static DisplayType DisplayType2Enum(const string& eType);
	//static StrategyParaType StrategyType2Enum(const string& eType);
	//�ж�������ַ����Ƿ���INT
	bool IsInteger(string& str);
	//static StrategyDirection StrategyDirection2Enum( const string& eType);
	/*bool CheckStrategyParaName(const std::string& name);*/
	//static const char* StrategyType2String(StrategyParaType eType);
	//static const char* DirectionType2String(StrategyDirection eType);
	//static const char* DisplayType2String(DisplayType eType);
	/*�õ�����ָ��*/
	void GetAllIndicator( std::map<std::string, CIndicatorDataEx> & lMapFiled);
	void GetAllIndicator_Download( std::map<std::string, CIndicatorDataEx> & lMapFiled, bool bOnlyMyself=false);
	/*ͨ��ָ�����õ�ָ����Ϣ*/
	bool GetIndicatorInfobyName(const string& IndiName,CIndicatorDataEx& data);
	//static const char* Bool2String( bool bData);
	/*����һ������*/
	void AddStrategy(CStrategyDataEx& data);
	void AddDownloadStrategy(CStrategyDataEx& data);
	/*ɾ��һ������*/
	void DeleteStrategy( const string& name);
	/*�õ�һ������*/
	bool GetStrategy( const string& name,CStrategyDataEx& data);
	bool GetDownloadStrategy( const string& name,CStrategyDataEx& data);
	/*�õ����еĲ���*/
	void GetAllStrategy(std::map<std::string, CStrategyDataEx> & lMapFiled);
	void GetAllStrategy_Download(std::map<std::string, CStrategyDataEx> & lMapFiled, bool bOnlyMyself=false);
	/*�õ�ĳ�������õ���ָ��*/
	void GetIndicator(const string& name,list<string>& data);
	void SetIndicatorDiaplay(list<string>& data,bool state);
	void indicatorDisplyMapClear();
	/*�ж�һ��ָ��˽�л��ǹ���*/
	int IsPrivate_Indicator(const std::string& name);
	/*�ж�һ������˽�л��ǹ���*/
	int IsPrivate_Strategy(const std::string& name);
	/*�Ƿ����ĳ��ָ��*/
	bool IsRelativeIndi(const string& name);

	/*�ж�һ��ָ���Ƿ�Ϊָ���û�����*/
	bool IsOwner_Indicator(const std::string& name, const std::string strUserID);
	/*�ж�һ�������Ƿ�Ϊָ���û�����*/
	bool IsOwner_Strategy(const std::string& name, const std::string strUserID);

protected:


	// 2014��4��3�գ��¶��Ĺ��ܷ�װ����
	//CIndicatorData		// 1.  ��װ���е�Indicator�Ľӿں�����
	//CIndicatorDataEx	// 2.  ����͵���indicator��xml
	//					// 3.  ���ɶ�Ӧ��c#���룬��ԭGenerateCodesת�ƹ���
	//CStrategyData		// 1.  ��װ���е�Strategy�Ľӿں�����
	//CStrategyDataEx		// 2.  ����͵���Strategy��xml
	//					// 3.  ���ɶ�Ӧ��c#���룬��ԭGenerateCodesת�ƹ���
	std::map<std::string, CIndicatorDataEx>		m_mapIndicatorInfo;// �������е�ָ�꣬��������˽�л��ǹ���
	std::map<std::string, CStrategyDataEx>		m_mapStrategyInfo;// �������еĲ��ԣ���������˽�л��ǹ���
	//// 1.  ����һ�����ѡ�в�����ص�ָ���б�����
	//// 2.  ����һ��Checkָ���Ƿ񱻲���ʹ�õĺ�����
	//// 3.  ���һ��ָ�����
	//// 4.  ���һ�����Զ���
	std::map<string,int>      m_mapIndicatorDiaplay;//ָ����������ʱָ�걻ʹ�õ��Ĵ���

	std::map<std::string, CIndicatorDataEx>		m_mapDownloadIndicators;
	std::map<std::string, CStrategyDataEx>		m_mapDownloadStrategies;

};
