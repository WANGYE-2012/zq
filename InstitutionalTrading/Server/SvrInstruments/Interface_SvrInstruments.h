#pragma once

#include "Interface_SvrCommon.h"
#include "UserApiStruct.h"
#include <map>
#include <string>
#ifndef SVRINSTRUMENTS_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRTEMPLATE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRINSTRUMENTS_EXPORTS
#define SVRINSTRUMENTS_API __declspec(dllexport)
#else
#define SVRINSTRUMENTS_API __declspec(dllimport)
#endif
#endif


typedef std::map<std::string,InstrumentField> _MapInstrumentType;

class SVRINSTRUMENTS_API CInterface_SvrInstruments : public CInterface_SvrCommon
{
public:
    static CInterface_SvrInstruments& getObj(void);


	//---------------------------------------------------------------------------------
	//	�������к�Լ
	//  ���أ� CF_ERROR_SUCCESS ����ɹ�
	//         CF_ERROR_COMMON_INPUT_PARAM �����������
	//---------------------------------------------------------------------------------
	virtual CF_ERROR WriteAllInstruments(const _MapInstrumentType & nMapInstruments) = 0;


	//---------------------------------------------------------------------------------
	//	��ȡ���к�Լ
	//  ���أ� CF_ERROR_SUCCESS ����ɹ�
	//         CF_ERROR_NO_INSTRUMENTS ����û�к�Լ��ȡ
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetAllInstruments( _MapInstrumentType & nMapInstruments) = 0;

	//---------------------------------------------------------------------------------
	//	��ȡ������Լ�ľ�����Ϣ
	//  ���أ� CF_ERROR_SUCCESS ����ɹ�
	//         CF_ERROR_NO_INSTRUMENTS ����û�ж�Ӧ��Լ
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetInstrumentField( const std::string& nInstrumentID,
		InstrumentField & nVecInstruments) = 0;


	//---------------------------------------------------------------------------------
	//	׷�ӻ���µ�����Լ
	//  ���أ� CF_ERROR_SUCCESS ����ɹ�
	//         CF_ERROR_COMMON_MEMEORY �����ڴ����û����ӳɹ�
	//---------------------------------------------------------------------------------
	virtual CF_ERROR AppendInstrument( const InstrumentField & nVecInstruments) = 0;

};







