#pragma once

#include "PlatformCTPQuotImpl.h"

#define QUOTTABLENAME   "AQUOTINFO"
enum WEEKDAY{Sunday=0, Monday,Tuesday,Wednesday,Thursday,Friday,Saturday};


class CSimulateQuotMgr
{
public:
    static CSimulateQuotMgr& getObj(void);
    static void deleteObj();   

public:
    CSimulateQuotMgr(void);
    ~CSimulateQuotMgr(void);

    void SetSimulateQuit(int nVal);
    bool GetSimulateQuit();

    void SetSimulatePause(int nVal);
    bool GetSimulatePause();

    bool GetSimulateData(eSimulateDayType& nType,vector<string>& vDayTime);
    int  GetSimulateQuotSize();

    void SetSimulateQuot(const PlatformStru_DepthMarketData& pDepthMarketData);
    void GetSimulateQuot(std::vector<PlatformStru_DepthMarketData>& vecdepthMarketData);

    //һ���ֶ���ʼ�طŲ���ʱ���Զ��ľ�ʧЧ��ֱ���ֶ�ֹͣ�طŲ������Զ�����Ч
    //�ֶ����
    //ģ������ʱ�佻����
    void LoadSimulateDay();
    CF_ERROR GetSimulateDay(sSimulateDay& sDay);
    CF_ERROR SetSimulateDay(const sSimulateDay& sDay);

    //ģ������طŷ�ʽ
    void LoadSimulatePlayMode();
    CF_ERROR GetSimulatePlayMode(eSimulatePlayMode& eMode);
    CF_ERROR SetSimulatePlayMode(const eSimulatePlayMode& sMode);

    //ģ�����
    bool SimulateQuotPlay();
    bool SimulateQuotPause();
    bool SimulateQuotStop();

    //�Զ����
    //ģ��ʱ��㣨�Զ�ģ��ʱ�� ������+ʱ��Σ�
    void LoadSimulateTime();
    CF_ERROR GetSimulateTime(sSimulateTime& sTime);

public:
    static CSimulateQuotMgr* m_pObj;
    static Ceasymutex	m_mutexSimulate;

    HANDLE  m_hReadStopEvent;
    HANDLE  m_hSendStopEvent;
private: 
    sSimulateDay    m_sSimulateDay;   
    sSimulateTime   m_sSimulateTime;
    eSimulatePlayMode m_eSimulatePlayMode;

    volatile int m_bSimulateQuit;        //
    volatile int m_bSimulatePause;       //

    //�ֶ�
    HANDLE m_hReadThread_manual;
    HANDLE m_hSendThread_manual;

    DWORD m_idReadThread_manual;
    DWORD m_idSendThread_manual;

    queue<PlatformStru_DepthMarketData>  m_queueSimulateQuot; //��������ݿ��ж�ȡ��ÿ�������������
};
