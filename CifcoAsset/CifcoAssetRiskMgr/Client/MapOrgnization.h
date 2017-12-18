
#pragma once

#include <cliext/map>
using namespace System;
using namespace System::Data;

using namespace CFMap::WindowsForms;
using namespace CFMap::CoreType;
//��Ϊ��֯IDΨһ,��ʹ��mapʵ��
namespace RiskMap {


ref class CSingleOrgnization
{
private:	
	Int32  mnOrgId;                  //��֯ID
	PointLatLng mPointLatLng;        //��γ��
	String^ msName;                  //��֯����
	Int32 mnOrgLevel;                //��֯���� 1��10��
	RiskLevel mLevel;                //���ռ���
	String^ msRiskDes;               //��������
	CSingleOrgnization ^ mParentOrg; //������
	CMarkerRisk ^ mrMaker;           //���ڵ�ͼ��ʾ�ķ��ձ��
	Int32 mnRiskCount;               //����������
    Int32 mnSingalIntrusmentMaxHode; //����Լ���ֲ�Ԥ������
    Int32 mnLimitedLost;             //�޿�Ԥ������
	Int32 mnHodeDanger;              //�ֲַ��ճ���Ԥ������
    Int32 mnScaleLevel;              //���ż���

public:
	CSingleOrgnization(Int32 nID,PointLatLng np,String^ nsName,Int32 nOrgLevel,RiskLevel nLevel,String^ nRiskDes,Int32 nScaleLevel,CSingleOrgnization ^ nrParent):
	      mnOrgId(nID),
		  mPointLatLng(np),
		  msName(nsName),
		  mnOrgLevel(nOrgLevel),
		  mLevel(nLevel),
		  msRiskDes(nRiskDes),
		  mnScaleLevel(nScaleLevel),
		  mParentOrg(nrParent)
	{
        mrMaker = nullptr;
		mnSingalIntrusmentMaxHode = 10;
		mnLimitedLost = 5;
		mnHodeDanger = 9;
		mnRiskCount = 24;
	}

    CSingleOrgnization()
	{
		msRiskDes = nullptr;
		mParentOrg = nullptr;
		msName = nullptr;
		mnOrgId = -1;
		mLevel = RiskLevel::RiskLevel_none;
		mrMaker = nullptr;
	}

	property CMarkerRisk ^ MakerRef
	{
		CMarkerRisk^ get() { return mrMaker; }
		void set(CMarkerRisk^ nm) { mrMaker = nm; }
	}
	
	property Int32  ID
	{
		Int32 get() { return mnOrgId; }
		void set(Int32 nm) { mnOrgId = nm; }
	}

	property String ^ Name
	{
		String^ get() { return msName; }
		void set(String^ nm) { msName = nm; }
	}

	property PointLatLng  LatLng
	{
		PointLatLng get() { return mPointLatLng; }
		void set(PointLatLng nm) { mPointLatLng = nm; }
	}
	property Int32  OrgLevel
	{
		Int32 get() { return mnOrgLevel; }
		void set(Int32 nm) { mnOrgLevel = nm; }
	}

	property RiskLevel  OrgRiskLevel
	{
		RiskLevel get() { return mLevel; }
		void set(RiskLevel nm) { mLevel = nm; }
	}

	property CSingleOrgnization ^  ParentOrg
	{
		CSingleOrgnization ^ get() { return mParentOrg; }
		void set(CSingleOrgnization ^ nm) { mParentOrg = nm; }
	}

	property String^   RiskDesc
	{
		String^  get() { return msRiskDes; }
		void set(String^  nm) { msRiskDes = nm; }
	}


	property Int32  RiskCount
	{
		Int32 get() { return mnRiskCount; }
		void set(Int32 nm) { mnRiskCount = nm; }
	}
	property Int32  SingalIntrusmentMaxHodeCount
	{
		Int32 get() { return mnSingalIntrusmentMaxHode; }
		void set(Int32 nm) { mnSingalIntrusmentMaxHode = nm; }
	}
	property Int32  LimitedLostCount
	{
		Int32 get() { return mnLimitedLost; }
		void set(Int32 nm) { mnLimitedLost = nm; }
	}
	property Int32  HoldDangerCount
	{
		Int32 get() { return mnHodeDanger; }
		void set(Int32 nm) { mnHodeDanger = nm; }
	}
	property Int32  ScaleLevel
	{
		Int32 get() { return mnScaleLevel; }
		void set(Int32 nm) { mnScaleLevel = nm; }
	}
};


typedef cliext::map<Int32, CSingleOrgnization^> TOrgnizationMap;
//typedef TOrgnizationMap::pair_iter_iter PairiIterator;
ref class COrgnization
{
private:
	TOrgnizationMap^ mmapOrg;
	Int32 mnDeep;
	Int32 mnMaxScaleLevel;
	
public:
	static Int32 INVALID_PARENT_ID=0;
	static Int32 MaxDeep = 10;
	static Int32 MinDeep = 1;
public:
	COrgnization()
	{
		mmapOrg = gcnew TOrgnizationMap();	

	}
	property Int32 Deep
	{
		Int32 get() { return mnDeep; }
		void set(Int32 nDeep) { mnDeep = nDeep; }
	}
	property Int32 MaxScaleLevel
	{
		Int32 get() { return mnMaxScaleLevel; }
		void set(Int32 nVal) { mnMaxScaleLevel = nVal; }
	}

	property TOrgnizationMap^ OrgMap
	{
		TOrgnizationMap^ get() { return mmapOrg; }
		void set(TOrgnizationMap^ nmap) { mmapOrg = nmap; }
	}
	//reset all to a risklevel
	void ResetAllToRiskLevel(RiskLevel nRiskLevel)
	{
		for each (TOrgnizationMap::value_type empl in mmapOrg)
		{
			empl->second->OrgRiskLevel = nRiskLevel;
		}
	}

	//����һ����֯�ṹ�ķ���״̬,���޸������еĸ���״̬,���״̬Ϊnormal ��ִ�ж���
	//�������״̬���ڵ�ǰ״̬����ֹͣ
	void UpdateOrgMapStatus(Int32 nID,RiskLevel nRiskLevel,String^ nRiskDes)
	{
		if(nRiskLevel == RiskLevel::RiskLevel_normal || nRiskLevel == RiskLevel::RiskLevel_none)
			return;
		//�ȸ���ID�ҵ���֯
		TOrgnizationMap::iterator lIte = mmapOrg->find(nID);
		if(lIte != mmapOrg->end())
		{
			lIte->second->OrgRiskLevel  = nRiskLevel;
			lIte->second->RiskDesc  = nRiskDes;
			CSingleOrgnization^ lParent = lIte->second->ParentOrg;
			while(lParent != nullptr)
			{
				if(lParent->OrgRiskLevel >= nRiskLevel)
				{
					break;
				}
				else
					lParent->OrgRiskLevel = nRiskLevel;

				lParent = lParent->ParentOrg;
			}

		}
	}
	//����ID������
	CSingleOrgnization ^ Find(int nID)
	{
		TOrgnizationMap::iterator lIte = mmapOrg->find(nID);
		if(lIte != mmapOrg->end())
		{
			return lIte->second;
		}
       
		return nullptr;
	}

	//����һ����֯
	bool Insert(Int32 nID,PointLatLng np,String^ nOrgName,Int32 nOrgLevel,RiskLevel nRiskLevel,String^ nRiskDes,Int32 nScaleLevel,Int32 nParentID)
	{
		////�쿴�ǲ���root node
		if(nParentID == INVALID_PARENT_ID) 
		{
			CSingleOrgnization ^ lrOrg = gcnew CSingleOrgnization(nID,np,nOrgName,nOrgLevel,nRiskLevel,nRiskDes,nScaleLevel,nullptr);
			mmapOrg->insert(TOrgnizationMap::make_value(nID,lrOrg));
			//������֯���
			Deep = nOrgLevel;
			mnMaxScaleLevel = nScaleLevel;
			return true;
		}

		//Find Parent
		CSingleOrgnization ^ lParent = Find(nParentID);
		if(lParent != nullptr)
		{
			CSingleOrgnization ^ lrOrg = gcnew CSingleOrgnization(nID,np,nOrgName,nOrgLevel,nRiskLevel,nRiskDes,nScaleLevel,lParent);
			mmapOrg->insert(TOrgnizationMap::make_value(nID,lrOrg));
			//������֯���
			if(nOrgLevel > Deep)
				Deep = nOrgLevel;
			if(nScaleLevel > mnMaxScaleLevel)
				MaxScaleLevel = nScaleLevel;
			return true;
		}

		return false;
	}

	//ɾ��һ����֯
	bool Remove(Int32 nID)
	{
		TOrgnizationMap::iterator lIte = mmapOrg->find(nID);
		if(lIte != mmapOrg->end())
		{
			mmapOrg->erase(lIte);
			return true;
		}
		return false;
	}

	//�Ƿ����ӽ��,����У����ض��ٸ�����û�з���0
	int HasChild(Int32 nID)
	{
		TOrgnizationMap::iterator lIre = mmapOrg->begin();
		Int32 lnCount = 0;
		for(;lIre != mmapOrg->end();lIre++)
		{
			if(lIre->second->ParentOrg != nullptr && lIre->second->ParentOrg->ID == nID)
				++lnCount;
		}
		return lnCount;
	}

	Int32 Size()
	{
		return mmapOrg->size();
	}

	//��ȡ��һ������֯
	CSingleOrgnization ^ GetFirstChildOrg(Int32 nParentID)
	{
		TOrgnizationMap::iterator lIre = mmapOrg->begin();	
		if(nParentID == INVALID_PARENT_ID && lIre != mmapOrg->end())
			return lIre->second;
			
		for(;lIre != mmapOrg->end();lIre++)
		{
			if(lIre->second->ParentOrg != nullptr && lIre->second->ParentOrg->ID == nParentID)
			{
				return lIre->second;
			}
		}
		return nullptr;
	}

	//��ȡ��һ������֯
	CSingleOrgnization ^ GetNextChildOrg(Int32 nParentID,CSingleOrgnization ^ nrPreChildOrg)
	{
		if(nrPreChildOrg == nullptr)
			return nullptr;

		TOrgnizationMap::iterator lIre = mmapOrg->find(nrPreChildOrg->ID);	
		lIre++;
		for(;lIre != mmapOrg->end();lIre++)
		{
			if(lIre->second->ParentOrg != nullptr && lIre->second->ParentOrg->ID == nParentID)
			{
				return lIre->second;
			}
		}
		return nullptr;
	}

	//��ȡһ����εĵ�һ���ڵ�
	CSingleOrgnization^ GetFirstOrgOfThisLevel(Int32 nOrgLevel)
	{
		TOrgnizationMap::iterator lIre = mmapOrg->begin();		

		for(;lIre != mmapOrg->end();lIre++)
		{
			if( lIre->second->OrgLevel == nOrgLevel)
			{
				return lIre->second;
			}
		}
		return nullptr;
	}

	//��ȡһ����ε���һ���ڵ�
	CSingleOrgnization ^ GetNextOrgOfThisLevel(Int32 nOrgLevel,CSingleOrgnization ^ nrPreOrg)
	{
		if(nrPreOrg == nullptr)
			return nullptr;

		TOrgnizationMap::iterator lIre = mmapOrg->find(nrPreOrg->ID);	
		lIre++;
		for(;lIre != mmapOrg->end();lIre++)
		{
			if( lIre->second->OrgLevel == nOrgLevel)
			{
				return lIre->second;
			}
		}
		return nullptr;
	}

	//�õ�һ�������Org�������ӽ�� ˭�����ĵ����
	CSingleOrgnization ^ CheckWhoCloserCenter(Int32 nOrgLevel,PointLatLng npCenter)
	{
		if(nOrgLevel > MaxDeep || nOrgLevel < MinDeep)
			return nullptr;
		TOrgnizationMap::iterator lIre = mmapOrg->begin();		
		TOrgnizationMap::iterator lIterCloser = lIre;
		double lnMin = 10000000000000.0;
		for(;lIre != mmapOrg->end();lIre++)
		{
			if( lIre->second->OrgLevel == nOrgLevel && HasChild(lIre->second->ID))
			{
				double ldbTemp = npCenter.LengthTwoPoint(lIre->second->LatLng);
				if(lnMin > ldbTemp)
				{
					lIterCloser = lIre;
					lnMin = ldbTemp;
				}
			}
		}
		return lIterCloser->second;
	}

};

}