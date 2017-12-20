#pragma  once
#include "CommonErrorCode.h"
#include "CommonDataChangeEvent.h"
//��ҵ��ģ��ӿ���Ĺ������ࡣ
//���Ǹ������࣬�ô��鷽���ķ�ʽ�����ҵ��ģ��ӿ������Ҫʵ�ֵķ���

class __declspec(dllexport) CInterface_SvrCommon
{
public:

	///************************************************************************/
	////����ResponseToAsk
	////����˵�����ú������������ģ���������ĳ�����ݲ������������������д�ú���������������أ����Ժ��Ըú���
	////ע�⣺    �ú����ڲ�ʵ�ֵ�ʱ��Ҫ���ܵ�������ģ��Ľӿں�����ֻ�ܵ����ڲ�������
	////������    neDataType   Ϊ���ݵĵ�����
	////          neChangeType Ϊ���ݵĲ���������
	////          nllDataID    Ϊ���ݵ�ΨһID
	////���أ�    �����ģ�鲻����ֹ�����ݵĲ���������CF_ERROR_SUCCESS
	////          �������ֹ�����ݵĲ�������������������
	///��ע��	  һƱ���
	///************************************************************************/
	virtual CF_ERROR ResponseToAsk(
		const eAskType& neDataType,
		const eAskOperationType& neOprType,
		const int nllDataID,
		const char * npDataBuf) 
	{
		return CF_ERROR_SUCCESS;
	}
	///************************************************************************/
	///��ע��	  һƱͨ����ͨ��ʱ����CF_ERROR_SUCCESS��pRlt�������������͵�ָ�룬����Ӧ���߷�������
	///************************************************************************/
	virtual CF_ERROR ResponseToAsk2(
		const eAskType& neDataType,
		const eAskOperationType& neOprType,
		const int nllDataID,
		const char * npDataBuf,
		void* pRlt=NULL) 
	{
		return CF_ERROR_FAILURE;
	}


	////���¶����ҵ��ģ�����Ҫʵ�ֵĽӿڷ���
	///************************************************************************/
	////����PreFilterPackage
	////����˵�����ú���ʹ����������������������ģ��֮ǰ�л����ȴ��������
	////ע�⣺    �ú����ڲ�ʵ�ֵ�ʱ��Ҫ���ܵ�������ģ��Ľӿں�����ֻ�ܵ����ڲ�������
	////������    npPackage Ϊ�����
	////          pConnection Ϊ�ô������õ������ݿ�����
	////          pStatament Ϊ�ô������õ������ݿ�����
	////���أ�    �ú����������ֹ�ð��������ݵ���һ��ģ�飬�뷵��һ��������
	////          ���������ֹ�ð����ݣ��뷵��CF_ERROR_SUCCESS
	///************************************************************************/
	//virtual CF_ERROR PreFilterPackage(void * npPackage,void* pConnection,void* pStatement)
	//{
	//	return CF_ERROR_SUCCESS;
	//}

	///************************************************************************/
	////����PostFilterPackage
	////����˵�����ú���ʹ����������������������ģ��֮���л����ȴ��������
	////ע�⣺    �ú����ڲ�ʵ�ֵ�ʱ��Ҫ���ܵ�������ģ��Ľӿں�����ֻ�ܵ����ڲ�������   
	////������    npPackage Ϊ�����
	////          pConnection Ϊ�ô������õ������ݿ�����
	////          pStatament Ϊ�ô������õ������ݿ�����
	////���أ�    �ú����������ֹ�ð��������ݵ���һ��ģ�飬�뷵��һ��������
	////          ���������ֹ�ð����ݣ��뷵��CF_ERROR_SUCCESS
	//virtual CF_ERROR PostFilterPackage(void * npPackage,void* pConnection,void* pStatement) 
	//{
	//	return CF_ERROR_SUCCESS;
	//}
};