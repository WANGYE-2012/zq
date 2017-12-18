#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include <set>
#include <map>
#include <queue>
#include <string>
#include "CDataInfo.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"
#include "easymutex.h"

//using namespace std;
namespace Client {

	typedef std::multimap<AssetMgmtOrgIDType, StatisticsEntry> RiskMapStatisticMap;
	ref class TotalStatiscData
	{
	public:
		double mdbCCYK; //
		double mdbCCQY;
		double mdbDQQY;
		double mdbBZJ;
		double mdbCCL;

	public:
		TotalStatiscData()
		{
			mdbCCL = mdbBZJ = mdbCCQY = mdbCCYK = mdbDQQY = 0.0;
		}
		void Add(StatisticsIDType nType,double ndbVal)
		{
			if(nType == ST_PosProfit)
				mdbCCYK += ndbVal;
			else if(nType == ST_InitFund)
				mdbCCQY += ndbVal;
			else if(nType == ST_Balance)
				mdbDQQY += ndbVal;
			else if(nType == ST_UseMargin)
				mdbBZJ += ndbVal; 
			else if(nType == ST_Position)
				mdbCCL += ndbVal; 
		}
		static Double SafeDivMulti100(Double nDbNumerator,Double nDbDenominator)
		{
			if(Math::Abs(nDbDenominator) < 0.000001)
				return 0;
			else
				return (nDbNumerator / nDbDenominator)*100  ;
			
		}
		static Double SafeDiv(Double nDbNumerator,Double nDbDenominator)
		{
			if(Math::Abs(nDbDenominator) < 0.000001)
				return 0.0;
			else
				return nDbNumerator / nDbDenominator;
		}
		static Double GetNearestTen(Double ndbVal)
		{
           double ldbCount = 1;
		   double ldbTemp = Math::Abs(ndbVal);
		   while(ldbTemp > 1)
		   {
			  ldbTemp /= 10;
			  ldbCount *= 10;
		   }
		   return  ldbCount;

		}
	};
	class CRiskMapData {
	public:
		static CRiskMapData* NewInstance() {
			return new CRiskMapData();
		}
		static void ReleaseInstance(CRiskMapData* instance) {
			if(instance!=NULL) 
				delete instance;
		}

	private:
		CRiskMapData(void) {
			
		}
		virtual ~CRiskMapData(void) {
			//��������
			mStaticsAccountVector.clear();
			mStaticsFinacalProductVector.clear();
			mStaticsVector.clear();
			mmapStatics.clear();
			mmapFinacialProducctStatics.clear();

		}

	public:
		// LockObject()��UnloadObject()��������������ɶԵ���
		void LockObject() {
			oMutex.lock();
		}
		void UnlockObject() {
			oMutex.unlock();
		}


		//���һ��ͳ�����ݣ������ж��Ǹ��»�������
		bool PushStatics(Int32 nOrgID,  //��֯OID
			StatisticsEntry* ns,
			DataTable ^ nOrgHZDataTable);  //ͳ������

		//ɾ��һ����֯�����������
		bool RemoveOrgStatics(Int32 nOrgID);
		
		//���һ��������Ʋ�Ʒ���ݣ������ж��Ǹ��»�������
		bool PushFinacialProductStatics(Int32 nOrgID,  //��֯OID
			StatisticsEntry* ns);  //ͳ������	
		
		RiskMapStatisticMap mmapStatics;                                //��������
		RiskMapStatisticMap mmapFinacialProducctStatics;                //ĳ������Ʋ�Ʒ��������

		std::vector<AssetOrgProRelation> mStaticsVector;                //���ĵĻ�������
		std::vector<AssetOrgProRelation> mStaticsFinacalProductVector;  //���ĵĵ�ǰ���ŵ���Ʋ�Ʒ������
	
		std::vector<int> mStaticsAccountVector;                          //


		MAPFinancialProduct mmapFinanProduct;                            //��ƷID�б�
	private:		
		Ceasymutex oMutex;
		
		
	};
}
