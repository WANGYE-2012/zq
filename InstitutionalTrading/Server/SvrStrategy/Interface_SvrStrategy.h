#pragma once

#include "Interface_SvrCommon.h"

#ifndef SvrStrategy_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SvrStrategy_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRSTRATEGY_EXPORTS
#define SVRSTRATEGY_API __declspec(dllexport)
#else
#define SVRSTRATEGY_API __declspec(dllimport)
#endif
#endif


class SVRSTRATEGY_API CInterface_SvrStrategy : public CInterface_SvrCommon
{
public:
    static CInterface_SvrStrategy& getObj(void);


	//---------------------------------------------------------------------------------
	//	���ݽӿ�
	//---------------------------------------------------------------------------------
	int getExampleValue(void);

private:
    CInterface_SvrStrategy();
    virtual ~ CInterface_SvrStrategy();
    static CInterface_SvrStrategy* m_pObj;
};







