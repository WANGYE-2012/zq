#pragma once

#include "Interface_SvrPresentation.h"
#include "UserStatic.h"
#include "commonstruct.h"
#include <vector>
#include <map>
#include <set>
#include "SocketBase.h"
#include "KernelStruct.h"
#include "CommonStructEx.h"

class CRealSvrPresentation : public CInterface_SvrPresentation
{
public:
	static CRealSvrPresentation& getObj(void);


	virtual bool clear();

	virtual bool OnTimerStatisc();

	virtual bool GenerateEmptyEnrtyList(const AssetOrgProRelation& org, std::vector<StatisticsEntry>& vec);

	virtual bool GetStatisticsEntryList(const AssetOrgProRelation& org, std::vector<StatisticsEntry>& vec);

	void SubscribeStatistics(const AssetOrgProRelation& org,SOCKET nSocket)
	{
		if(m_SubscribeStatistics.find(org) == m_SubscribeStatistics.end())
		{
			std::set<SOCKET> lSetSocket;
			lSetSocket.insert(nSocket);
			m_SubscribeStatistics[org] = lSetSocket;
		}
		else
		{
			m_SubscribeStatistics[org].insert(nSocket);
		}	
	}
	void UnSubscribeStatistics(const AssetOrgProRelation& org,SOCKET nSocket)
	{
		if(m_SubscribeStatistics.find(org) != m_SubscribeStatistics.end())
		{			
			m_SubscribeStatistics[org].erase(nSocket);
			if(m_SubscribeStatistics[org].size() == 0)
				m_SubscribeStatistics.erase(org);
		}	
	}

	void SubscribeFund(const int org,SOCKET nSocket)
	{
		if(m_SubscribeFund.find(org) == m_SubscribeFund.end())
		{
			std::set<SOCKET> lSetSocket;
			lSetSocket.insert(nSocket);
			m_SubscribeFund[org] = lSetSocket;
		}
		else
		{
			m_SubscribeFund[org].insert(nSocket);
		}	
	}
	void UnSubscribeFund(const int org,SOCKET nSocket)
	{
		if(m_SubscribeFund.find(org) != m_SubscribeFund.end())
		{			
			m_SubscribeFund[org].erase(nSocket);
			if(m_SubscribeFund[org].size() == 0)
				m_SubscribeFund.erase(org);
		}	
	}

	void GetFundList(int nUserID,std::vector<sFundInfoEx>& vec);
private:


	//����
	std::map<AssetOrgProRelation,std::set<SOCKET>> m_SubscribeStatistics;
	std::map<int,std::set<SOCKET>> m_SubscribeFund;  //�����ʽ�仯

	//ͳ��
	std::map<int,sFundInfoEx>	mmapFundItemUser;   //�����û����ʽ���
	std::map<int,CUserStatic>			mmapStatisticsItemUser;   //�����û���ͳ�ƽ��
	std::map<int,CUserStatic>			mmapStatisticsItemOrg;   //������֯������ͳ�ƽ��
	std::map<int,CUserStatic>			mmapStatisticsItemFinicial;   //������Ʋ�Ʒ��ͳ�ƽ��

	
	CRealSvrPresentation();
	virtual ~ CRealSvrPresentation();
	static CRealSvrPresentation* m_pObj;
};