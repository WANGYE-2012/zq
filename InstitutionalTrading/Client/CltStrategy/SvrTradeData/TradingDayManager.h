
#pragma once
#include <set>
#include <map>
#include "easymutex.h"
#include <string>
enum eTradingDayInitStatus
{
	TRADING_DAY_INIT_STATUS_NOTHING = 0,       
	TRADING_DAY_INIT_STATUS_INSTRUMENT = 1
};

class CTradingDayManager
{
	
public:
	CTradingDayManager();
	~CTradingDayManager();

	void SetNeedSaveTODB(bool nbNeed) { mbNeedSaveDB = nbNeed;}
	//װ����ʷ�Ľ�����
	void LoadHistoryTradingDay();

	//װ����ʷ�Ľ�����
	void LoadHistorySettmentedDay();

	//�ж��Ƿ������г�ʼ�����Ľ�����δ����
	bool HasInitTradingDayWithNoSettlement(std::string & nsOldestInitTradingDayWithNoSettlement);

	//��ȡ��һ��������
	bool GetLastTradingData(const std::string & nDay,std::string & nLastTradingDay);

	//���һ��������
	bool AddTradingDay(const std::string & nDay);

	//����һ�������ճ�ʼ�������
	bool SetTradingDayInit(const std::string & nDay);

	//�ǲ���ϵͳ���еĵ�һ��
	bool IsFirstDay();

	//�Ƿ��ǽ�����
	bool IsTradingDay(const std::string & nsDay);

	//�Ƿ�����
	bool IsSetttemnted(const std::string & nsDay);


	//���һ��������
	bool AddSettlementedDay(const std::string & nDay); 

	//��ȡ���µ��ѽ���Ľ�����
	bool GetNewestSettlementDay( std::string & nDay); 
private:
	Ceasymutex mMutex;
	std::map<std::string,eTradingDayInitStatus> mmapAllTradingDay;//���еĽ�����

	std::set<std::string> msetAllSettlementedDay;//�����������Ľ�����

	bool mbNeedSaveDB;
};