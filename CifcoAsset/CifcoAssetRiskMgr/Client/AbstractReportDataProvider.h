#pragma once
#include <cliext/set>
#include <cliext/map>
#include <cliext/vector>
#include "Tools.h"
using namespace System;
using namespace System::Data;



namespace ReportDataProvider {

	//Ϊһ��int32�͵����� ���˺�ID�б�
	public ref class  CQueryConditions
	{
	public:
		array<Int32>^ marrayAccount;   //�����ʺ��б�
		DateTime   mstartTime;         //��ʼʱ��
		DateTime   mendTime;           //����ʱ��
		array<Int32>^ marrayInstructID;//��ԼID����
		
	};


	typedef cliext::set<String^> FilterAccontSetType;
    typedef cliext::map<String^,cliext::vector<Double>^> TimeStaticBalanceType;
	typedef cliext::map<String^,cliext::vector<Int32>^> TimeStaticHodeType;
	public ref class IReportDataProvider abstract
	{	

	private:
		System::Data::DataTable ^ mrDataTable;   //ͳһ���صĴ��
		FilterAccontSetType^ mFilterAccountSet;  //�����˺�
	    FilterAccontSetType^ mFilterInstrument;  //���˺�Լ
	public:
		
		IReportDataProvider()
		{
			mrDataTable = gcnew System::Data::DataTable();
			mFilterAccountSet = gcnew cliext::set<String^>();
			mFilterInstrument = gcnew cliext::set<String^>();
		}
		property DataTable ^ RealDataTable
		{
			DataTable ^ get() {return mrDataTable;}
			Void set(DataTable ^ nrTable) { mrDataTable = nrTable;}
		}
		/*property Boolean GroupQuery
		{
			Boolean  get() {return mbGroupQuery;}
			Void set(Boolean  nbVal) { mbGroupQuery = nbVal;}
		}  */
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
		bool FilterAccount(std::string nStr)
		{
			if(mFilterAccountSet->find(Tools::string2String(nStr.c_str())) == mFilterAccountSet->end())
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

			//���Һ�Լ������Ʒ��
			if(mFilterInstrument->find(Tools::string2String(nStr.c_str())) == mFilterInstrument->end())
			{
				//��ǰ��λƷ�ִ���		
				nStr.erase(2,nStr.length() - 2);
				if(mFilterInstrument->find(Tools::string2String(nStr.c_str())) == mFilterInstrument->end())
					return true;
			}
				
				
			return false;
		}
	//����ӿ�	
	public:	
		//�����¼�
		virtual void SubscribeEvt(CQueryConditions^ nCondition,IntPtr hWnd) abstract;
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