#pragma once
#include <map>
#include <string>
#include <set>
#include <vector>
#include "CommonStruct.h"
class CUserStatic
{
private:
	double mdbPositionProfit; //�ֲ�ӯ������
	double mdbInitFund;       //�û���ʼȨ��
	double mdbStaticBalance;  //��ǰ��Ȩ�����
	std::map<std::string,double>  mmapInstrumentMargin;//����Լ�ֱֲ�֤��
	std::map<std::string,double>  mmapInstrmentVolume;//����Լ�ֲ���
	std::set<std::string> msetInstrument;

public:
	
	CUserStatic()
	{
		clear();
	}
	~CUserStatic()
	{
		clear();
	}

	void clear()
	{
		msetInstrument.clear();
		mdbStaticBalance = mdbInitFund = mdbPositionProfit = 0.0f;
		mmapInstrmentVolume.clear();
		mmapInstrumentMargin.clear();
	}
	double GetPositionProfit()
	{
		return mdbPositionProfit;
	}

	void SetPositionProfit(double ndbVal)
	{
		mdbPositionProfit = ndbVal;
	}

	double GetInitFund()
	{
		return mdbInitFund;
	}

	void SetInitFund(double ndbVal)
	{
		mdbInitFund = ndbVal;
	}

	double GetStaticBalance()
	{
		return mdbStaticBalance;
	}

	void SetStaticBalance(double ndbVal)
	{
		mdbStaticBalance = ndbVal;
	}

	void PushInstrumentUserMargin(const std::string& nsInstrumentID,double ndbVal)
	{
		msetInstrument.insert(nsInstrumentID);
		mmapInstrumentMargin[nsInstrumentID] += ndbVal;
	}

	double GetInstruemntUserMargin(const  std::string& nsInstrumentID)
	{
		if(mmapInstrumentMargin.find(nsInstrumentID) != mmapInstrumentMargin.end())
			return mmapInstrumentMargin[nsInstrumentID];
		else
			return 0.0f;
	}

	void PushInstrumentVolumn(const  std::string& nsInstrumentID,double ndbVal)
	{
		msetInstrument.insert(nsInstrumentID);
		mmapInstrmentVolume[nsInstrumentID] += ndbVal;
	}

	double GetInstruemntUserVolumn(const std::string& nsInstumentId)
	{
		if(mmapInstrmentVolume.find(nsInstumentId) != mmapInstrmentVolume.end())
			return mmapInstrmentVolume[nsInstumentId];
		else
			return 0.0f;
	}

	void GetInsturments(std::set<std::string>& lsetInstrument)
	{
		lsetInstrument = msetInstrument;
	}

	void GetStatisticsEntryList(const AssetOrgProRelation& org,std::vector<StatisticsEntry>& vec);

};