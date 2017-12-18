#include "StdAfx.h"
#include "RiskmapData.h"

#include "TcpLayer.h"
#include "CDataInfo.h"
#include "RiskManageCmd.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"


//���һ��ͳ�����ݣ������ж��Ǹ��»�������
bool Client::CRiskMapData::PushStatics(Int32 nOrgID,  //��֯OID
		  StatisticsEntry* npStatics,DataTable ^ nOrgHZDataTable)  //ͳ������
		  
{
	if(npStatics == NULL)
		return false;

	bool lbUpdate = false;
	RiskMapStatisticMap::iterator lIteratorStart,lIteratorEnd;
	lIteratorStart = mmapStatics.lower_bound(nOrgID);
	lIteratorEnd = mmapStatics.upper_bound(nOrgID);	
	while(lIteratorStart != lIteratorEnd && lIteratorStart != mmapStatics.end())
	{
		if(memcmp(npStatics,&lIteratorStart->second,sizeof(StatisticsEntry)-sizeof(double))==0)
		{
			lbUpdate = true;
			lIteratorStart->second.dblValue = npStatics->dblValue;
			break;
		}
		lIteratorStart++;
	}
	if(lbUpdate == false)
	{
		mmapStatics.insert(std::make_pair(nOrgID,*npStatics));
	}
	

	return true;
}
//ɾ��һ����֯�����������
bool Client::CRiskMapData::RemoveOrgStatics(Int32 nOrgID)
{
	mmapStatics.erase(nOrgID);
	return true;
	//RiskMapStatisticMap::iterator lIteratorStart,lIteratorEnd;
	//lIteratorStart = mmapStatics.lower_bound(nOrgID);
	//lIteratorEnd = mmapStatics.upper_bound(nOrgID);	
	//while(lIteratorStart != lIteratorEnd && lIteratorStart != mmapStatics.end())
	//{
	//	mmapStatics.erase(nOrgID);
	//	lIteratorStart++;
	//}
}


//���һ��������Ʋ�Ʒ���ݣ������ж��Ǹ��»�������
bool Client::CRiskMapData::PushFinacialProductStatics(Int32 nOrgID,  //��֯OID
				 StatisticsEntry* npStatics)  //ͳ������	
{
	if(npStatics == NULL)
		return false;

	bool lbUpdate = false;
	RiskMapStatisticMap::iterator lIteratorStart,lIteratorEnd;
	lIteratorStart = mmapFinacialProducctStatics.lower_bound(nOrgID);
	lIteratorEnd = mmapFinacialProducctStatics.upper_bound(nOrgID);	
	while(lIteratorStart != lIteratorEnd && lIteratorStart != mmapFinacialProducctStatics.end())
	{
		if(memcmp(npStatics,&lIteratorStart->second,sizeof(StatisticsEntry)-sizeof(double))==0)
		{
			lbUpdate = true;
			lIteratorStart->second.dblValue = npStatics->dblValue;
			break;
		}
		lIteratorStart++;
	}
	if(lbUpdate == false)
	{
		mmapFinacialProducctStatics.insert(std::make_pair(nOrgID,*npStatics));
	}


	return true;
}