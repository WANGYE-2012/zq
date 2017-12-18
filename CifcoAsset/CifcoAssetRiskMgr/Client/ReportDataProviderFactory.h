
#pragma once
#include "AbstractReportDataProvider.h"
#include "AccountHodeReportDP.h"
#include "AccountInterestReportDP.h"
#include "AccountTradesReportDP.h"
#include "Singleton.h"
namespace ReportDataProvider
{
	public interface class IReportDataProviderFactory
	{
	public:
		virtual IReportDataProvider^ CreateDataProvider() = 0;
		
	};



	//�û��ֲ�ͳ�ƹ���
	public ref class CAccountHodeReportDataProviderFactory :public IReportDataProviderFactory,
		Singleton<CAccountHodeReportDataProviderFactory>
	{

	public:
		virtual IReportDataProvider^ CreateDataProvider() 
		{
			return gcnew CAccountHodeReportDataProvider();
		}
	};

	//�û�Ȩ��ͳ�ƹ���
	public ref class CAccountInterestsDataProviderFactory :public IReportDataProviderFactory,
		Singleton<CAccountInterestsDataProviderFactory>
	{
	public:
		virtual IReportDataProvider^ CreateDataProvider() 
		{
			return gcnew CAccountInterestReportDataProvider();
		}
	};

	//�û�����ͳ�ƹ���
	public ref class CAccountTradesReportDataProviderFactory :public IReportDataProviderFactory,
		Singleton<CAccountTradesReportDataProviderFactory>
	{
	public:
		virtual IReportDataProvider^ CreateDataProvider() 
		{
			return gcnew CAccountTradesReportDataProvider();
		}
	};



}