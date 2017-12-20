#pragma once

#include "Interface_SvrCommon.h"

#ifndef SVRRISK_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRRISK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRRISK_EXPORTS
#define SVRRISK_API __declspec(dllexport)
#else
#define SVRRISK_API __declspec(dllimport)
#endif
#endif


class SVRRISK_API CInterface_SvrRisk : public CInterface_SvrCommon
{
public:
    static CInterface_SvrRisk& getObj(void);


	//---------------------------------------------------------------------------------
	//	���ݽӿ�
	//---------------------------------------------------------------------------------
	int getExampleValue(void);

private:
    CInterface_SvrRisk();
    virtual ~ CInterface_SvrRisk();
    static CInterface_SvrRisk* m_pObj;
};







