#pragma once
#include "Interface_SvrInstruments.h"

class CCustomInterfaceSvrInstruments : public CInterface_SvrInstruments
{
	//����getObj()������/����������m_pObj���ڶ��嵥ʵ����
public:
	//���ڻ�ȡ�ӿڶ���ͨ���ö���ʹ�ýӿڷ���
	static CCustomInterfaceSvrInstruments& getObj(void);
private:
	CCustomInterfaceSvrInstruments();
	virtual ~ CCustomInterfaceSvrInstruments();
	static CCustomInterfaceSvrInstruments* m_pObj;
private:
	_MapInstrumentType mMapInstruments;
public:
	//---------------------------------------------------------------------------------
	//	�������к�Լ
	//---------------------------------------------------------------------------------
	virtual CF_ERROR WriteAllInstruments(const _MapInstrumentType & nMapInstruments);


	//---------------------------------------------------------------------------------
	//	��ȡ���к�Լ
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetAllInstruments( _MapInstrumentType & nMapInstruments);


	//---------------------------------------------------------------------------------
	//	��ȡ������Լ�ľ�����Ϣ
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetInstrumentField( const std::string& nInstrumentID,
		InstrumentField & nVecInstruments) ;

	//---------------------------------------------------------------------------------
	//	׷�ӻ���µ�����Լ
	//  ���أ� CF_ERROR_SUCCESS ����ɹ�
	//         CF_ERROR_COMMON_MEMEORY �����ڴ����û����ӳɹ�
	//---------------------------------------------------------------------------------
	virtual CF_ERROR AppendInstrument( const InstrumentField & nVecInstruments) ;

private:
	virtual CF_ERROR WriteSingleInstrumentToDB(const InstrumentField & nInstruments);
};