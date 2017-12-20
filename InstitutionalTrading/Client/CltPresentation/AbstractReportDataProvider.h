#pragma once
#include <cliext/set>
#include <cliext/map>
#include <cliext/vector>
#include "Tools.h"
#include "CDataInfo.h"
#include <string>
using namespace System;
using namespace System::Data;



namespace ReportDataProvider {

	//Ϊһ��int32�͵����� ���˺�ID�б�
	public ref class  CQueryConditions
	{
	public:
		CQueryConditions(int nSize ,int nOtherSize )
		{
			marrayAccount = gcnew array<String^>(nSize);
			marrayOtherName = gcnew array<String^>(nOtherSize);
		}
	public:
		int             mnQueryMode;
		array<String^>^ marrayAccount;   //�����ʺ��б�
		DateTime   mstartTime;         //��ʼʱ��
		DateTime   mendTime;           //����ʱ��
		array<String^>^ marrayOtherName;//��ԼID����
		
	};


	typedef cliext::set<String^> FilterAccontSetType;
    typedef cliext::map<String^,cliext::vector<Double>^> TimeStaticBalanceType;
	typedef cliext::map<String^,cliext::vector<Int32>^> TimeStaticHodeType;
	typedef cliext::map<Int32,String^> QueryTimeSeq;
	public ref class IReportDataProvider abstract
	{	

	private:
		System::Data::DataTable ^ mrDataTable;   //ͳһ���صĴ��
		System::Data::DataTable ^ mrStatisticDataTable;   //ͳ��
		FilterAccontSetType^ mFilterAccountSet;  //�����˺�
	    FilterAccontSetType^ mFilterInstrument;  //���˺�Լ
		FilterAccontSetType^ mFilterOther;       //��������
		Int32 mnQueryMode;
		QueryTimeSeq^ mQuerySeq;
		Int32 mnQueryCount;
		bool mbUserCancel;
	public:
		
		IReportDataProvider()
		{
			mrDataTable = gcnew System::Data::DataTable();
			mrStatisticDataTable = gcnew System::Data::DataTable();
			mFilterAccountSet = gcnew cliext::set<String^>();
			mFilterInstrument = gcnew cliext::set<String^>();
			mFilterOther = gcnew cliext::set<String^>();
			mQuerySeq = gcnew QueryTimeSeq();
			mnQueryMode = -1;
			mbUserCancel = false;
		}
		property DataTable ^ RealDataTable
		{
			DataTable ^ get() {return mrDataTable;}
			Void set(DataTable ^ nrTable) { mrDataTable = nrTable;}
		}
		property DataTable ^ StatisticDataTable
		{
			DataTable ^ get() {return mrStatisticDataTable;}
			Void set(DataTable ^ nrTable) { mrStatisticDataTable = nrTable;}
		}

		property Int32 QueryMode
		{
			Int32  get() {return mnQueryMode;}
			Void set(Int32  nbVal) { mnQueryMode = nbVal;}
		}  
		void ClearFilterInstrument()
		{
			mFilterInstrument->clear();
		}
		void ClearFilterAccount()
		{
			mFilterAccountSet->clear();
		}

		void SetFilterAccountSet(std::set<std::string> & nSet)
		{
			mFilterAccountSet->clear();
			std::set<std::string>::iterator lite = nSet.begin();
			while(lite != nSet.end())
			{
				mFilterAccountSet->insert(Tools::string2String((*lite).c_str()));
				lite++;
			}
		}
		void SetFilterInstrument(std::set<std::string> & nSet)
		{
			mFilterInstrument->clear();
			std::set<std::string>::iterator lite = nSet.begin();
			while(lite != nSet.end())
			{
				mFilterInstrument->insert(Tools::string2String((*lite).c_str()));
				lite++;
			}
		}

		Object ^ ConvertDouble(double nb)
		{
			if(nb == util::GetDoubleInvalidValue())
				return  0.0f;
			else
				return nb;

		}
		void SetFilterOther(std::set<std::string> & nSet)
		{
			mFilterOther->clear();
			std::set<std::string>::iterator lite = nSet.begin();
			while(lite != nSet.end())
			{
				mFilterOther->insert(Tools::string2String((*lite).c_str()));
				lite++;
			}
		}

		bool FilterAccount(std::string nStr)
		{
			if(mFilterAccountSet->find(Tools::string2String(nStr.c_str())) == mFilterAccountSet->end())
				return true;
			return false;
		}

		bool FilterOther(std::string nStr)
		{
			if(mFilterOther->find(Tools::string2String(nStr.c_str()))== mFilterOther->end())
				return true;
			return false;
		}
		bool FilterInstrument(std::string nStr)
		{
			//���û�����ù��˺�Լ����ȫ��������
			if(mFilterInstrument->size() == 0)
				return false; 

			if(nStr.empty())
				return true;

			String ^ lsIID = Tools::string2String(nStr.c_str());
			String ^ lsPid;
			for(int i = 0; i < lsIID->Length;i++)
			{
				if(Char::IsLetter(lsIID,i))
				{
					lsPid += lsIID->Substring(i,1);
				}
				else
					break;
			}

			if(mFilterInstrument->find(lsPid) == mFilterInstrument->end())
				return true;
							
			return false;
		}

		array<String ^>^ GetStaticKey(String^ lsAccountVal,String ^ lsInstrument)
		{
			if(mnQueryMode == 0)
			{
				//
				if(lsAccountVal == nullptr)
					return nullptr;

				int nUserID = 0;
				nUserID = CDataInfo::NewInstance()->GetUserIDByAccount(Tools::String2string(lsAccountVal));
			   //������
				std::string lsOrgName = CDataInfo::NewInstance()->GetOrgNameByUserID(nUserID);
				array<String ^>^ lsRet = gcnew array<String ^>(1);
				lsRet[0] = Tools::string2String(lsOrgName.c_str());

				return lsRet;
			}
			else if(mnQueryMode == 1)
			{
				//�����
				if(lsAccountVal == nullptr)
					return nullptr;

				int nUserID = 0;
				nUserID = CDataInfo::NewInstance()->GetUserIDByAccount(Tools::String2string(lsAccountVal));
				//������
				std::set<int> vecProduct;
				CDataInfo::NewInstance()->GetProductByTraderID(nUserID,vecProduct);
				array<String ^>^ lsRet = gcnew array<String ^>(vecProduct.size());
				std::set<int>::iterator it = vecProduct.begin();	
				int i = 0;
				for(; it != vecProduct.end(); it++,i++)
				{
					std::string lsVal = CDataInfo::NewInstance()->GetFinancialProductNameByID(*it);
					lsRet[i] = (Tools::string2String(lsVal.c_str()));
				}
				return lsRet;

			}
			else if(mnQueryMode == 2)
			{
				//��Ʒ��
				if(lsInstrument == nullptr)
					return nullptr;

				//ָ���û���ѯ
				array<String ^>^ lsRet = gcnew array<String ^>(1);
				String ^ lsStr = "";
				for(int i = 0; i < lsInstrument->Length;i++)
				{
					if(Char::IsLetter(lsInstrument,i))
					{
						lsStr += lsInstrument->Substring(i,1);
					}
					else
						break;
				}

				lsRet[0] = lsStr;
				return lsRet;
			}
			else
			{
				if(lsAccountVal == nullptr)
					return nullptr;
				//ָ���û���ѯ
				array<String ^>^ lsRet = gcnew array<String ^>(1);
				lsRet[0] = lsAccountVal;

				return lsRet;
			}

		}

		void ClearQuerySeq()
		{
			mQuerySeq->clear();
		}
		void AddQuerySeq(DateTime^ nTime,int nSeq)
		{
			mQuerySeq[nSeq] = nTime->ToString("yyyy-MM-dd");
		
		}
		void FinishedSingleQuey(int nSeq)
		{
			mQuerySeq->erase(nSeq);
		}

		bool QueryIsFinished()
		{
			return mQuerySeq->size() == 0;
		}

		Int32 GetRemindQueryCount()
		{
			return mQuerySeq->size();
		}
		Int32 GetTotalQueryCount()
		{
			return mnQueryCount;
		}
		void SetTotalQueryCount(Int32 nVal)
		{
			mnQueryCount = nVal;
		}

		String^ GetCurrentQuery(int & nSeq)
		{
			String^ lRetString= "";
			if(mQuerySeq->size() == 0)
				return lRetString;
			else
			{
				QueryTimeSeq::iterator  lIter = mQuerySeq->begin();
				lRetString = lIter->second;	
				nSeq = lIter->first;
				return lRetString;				
			}
		}
		void SetUserCancel(bool nbVal)
		{
			mbUserCancel = nbVal;
		}

		bool IsUserCancel()
		{
			return mbUserCancel;
		}
	//����ӿ�	
	public:	
		//�����¼�
		virtual void SubscribeEvt(CQueryConditions^ nCondition,IntPtr hWnd,Int32 nUserID) abstract;
		//�˶��¼�
		virtual void UnsubscribeEvt(unsigned int hWnd) abstract;

		//ִ��
		virtual bool ProcessData(char*  npRespond) abstract;

		//���ݽ��սӿ�
		virtual bool OnReponseDataReciver() abstract;

		//���ɱ�ͷ
		virtual bool GenerateGridViewHeader() abstract;

		//��GridView
		virtual bool GenerateDataTable(CQueryConditions^ nCondition) abstract;

		//���chart
		virtual bool FillChart(System::Windows::Forms::FlowLayoutPanel^ nrChartContainer) abstract;
	

	
	};

}