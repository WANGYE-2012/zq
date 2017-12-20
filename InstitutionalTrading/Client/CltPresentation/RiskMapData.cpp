#include "StdAfx.h"
#include "RiskmapData.h"

#include "TcpLayer.h"
#include "CDataInfo.h"
#include "CommonDef.h"
#include "CommonPkg.h"


//���һ��ͳ�����ݣ������ж��Ǹ��»�������
bool CltPresentation::CRiskMapData::PushStatics(Int32 nOrgID,  //��֯OID
		  StatisticsEntry* npStatics)  //ͳ������
		  
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
			lIteratorStart->second.mdblValue = npStatics->mdblValue;
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
bool CltPresentation::CRiskMapData::RemoveOrgStatics(Int32 nOrgID)
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
bool CltPresentation::CRiskMapData::PushFinacialProductStatics(Int32 nFinancial,  //��֯OID
				 StatisticsEntry* npStatics)  //ͳ������	
{
	if(npStatics == NULL)
		return false;

	bool lbUpdate = false;
	RiskMapStatisticMap::iterator lIteratorStart,lIteratorEnd;
	lIteratorStart = mmapFinacialProducctStatics.lower_bound(nFinancial);
	lIteratorEnd = mmapFinacialProducctStatics.upper_bound(nFinancial);	
	while(lIteratorStart != lIteratorEnd && lIteratorStart != mmapFinacialProducctStatics.end())
	{
		if(memcmp(npStatics,&lIteratorStart->second,sizeof(StatisticsEntry)-sizeof(double))==0)
		{
			lbUpdate = true;
			lIteratorStart->second.mdblValue = npStatics->mdblValue;
			break;
		}
		lIteratorStart++;
	}
	if(lbUpdate == false)
	{
		mmapFinacialProducctStatics.insert(std::make_pair(nFinancial,*npStatics));
	}
	//if(npStatics == NULL)
	//	return false;

	//bool lbUpdate = false;
	//RiskMapStatisticMap::iterator lIteratorStart,lIteratorEnd;
	//lIteratorStart = mmapFinacialProducctStatics.lower_bound(nOrgID);
	//lIteratorEnd = mmapFinacialProducctStatics.upper_bound(nOrgID);	
	//while(lIteratorStart != lIteratorEnd && lIteratorStart != mmapFinacialProducctStatics.end())
	//{
	//	if(memcmp(npStatics,&lIteratorStart->second,sizeof(StatisticsEntry)-sizeof(double))==0)
	//	{
	//		lbUpdate = true;
	//		lIteratorStart->second.mdblValue = npStatics->mdblValue;
	//		break;
	//	}
	//	lIteratorStart++;
	//}
	//if(lbUpdate == false)
	//{
	//	mmapFinacialProducctStatics.insert(std::make_pair(nOrgID,*npStatics));
	//}


	return true;
}