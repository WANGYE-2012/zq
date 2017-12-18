/*
    ���ڱ����ά�����׵�¼���������
    added by l. 20130329
*/


#pragma once

#include <string>

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "..\\ctp\\ThostFtdcUserApiStruct.h"
#include "..\\Module-Misc2\\KernelStruct.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CPlatformDataMgr_LoginInfo
{
public:
    CPlatformDataMgr_LoginInfo(void);
    ~CPlatformDataMgr_LoginInfo(void);

    //���ý������ӵ�FrontID��SessionID
    void SetTradingLoginInfo(const PlatformStru_LoginInfo& LoginInfo);
    //��ȡ�������ӵ�FrontID��SessionID
    const PlatformStru_LoginInfo& GetTradingLoginInfo(void);


    //�����������
    void clear(void);

private:
	//Ceasymutex				m_mutex;

    PlatformStru_LoginInfo m_LoginInfo;

    //int m_TradingFrontID;
    //int m_TradingSessionID;
    //char m_UserID[32];
};

#ifdef WIN32
#pragma managed(pop)
#endif 
