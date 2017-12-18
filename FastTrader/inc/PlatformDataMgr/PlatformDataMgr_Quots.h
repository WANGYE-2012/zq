/*
    ���ڱ����ά��CTP����������
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <map>
#include <string>

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Instruments.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 


class CPlatformDataMgr_Quots
{
public:
    CPlatformDataMgr_Quots(CPlatformDataMgr_Instruments& refInstruments);
    ~CPlatformDataMgr_Quots(void);

    //��ȡָ����Լ����
    bool GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
    //����ָ����Լ���飬���õ�������£��򷵻�true�����򷵻�false����ԭ����������ϣ�
    bool SetQuotInfo(const PlatformStru_DepthMarketData& inData);
    //��ȡָ����Լ�����¼ۣ�ʧ�ܷ�����Чֵ
    double GetCurPrice (const std::string& InstrumentID);
    //��ȡָ����Լ������
    bool GetOldQuotInfo (const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);

    //�����������
    void clear(void);
    //�ж��Ƿ�Ϊ��
    bool empty(void);

private:
    Ceasymutex				        m_mutex;

    CPlatformDataMgr_Instruments    &m_RefInstruments;

    //�������ݼ�
    std::map<std::string,PlatformStru_DepthMarketData> m_lastQuot;    //��������
    std::map<std::string,PlatformStru_DepthMarketData> m_OldQuot;     //�ϴ�������
};

#ifdef WIN32
#pragma managed(pop)
#endif 
