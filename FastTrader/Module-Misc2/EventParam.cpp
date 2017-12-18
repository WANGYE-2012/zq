#include "StdAfx.h"
#include "EventParam.h"

using namespace std;

#ifdef WIN32
#pragma unmanaged
#endif 


CRITICAL_SECTION CFTEventParam::m_CS;
DWORD CFTEventParam::m_BaseID=1;
map<DWORD,Stru_EventParam> CFTEventParam::m_mapEvtParam;


//����ʼʱInit
void CFTEventParam::Init(void)
{
    InitializeCriticalSection(&m_CS);
    m_BaseID=1;
    m_mapEvtParam.clear();
}

//�����˳�ʱRelease��Ҳ���Բ�Release���ò���ϵͳ�Զ��ͷ���Դ
void CFTEventParam::Release(void)
{
    EnterCriticalSection(&m_CS);
    while(m_mapEvtParam.begin()!=m_mapEvtParam.end())
    {
        char* ptmp=m_mapEvtParam.begin()->second.EvtPtr;
        m_mapEvtParam.erase(m_mapEvtParam.begin());
        if(ptmp!=NULL) free(ptmp);
    }
    LeaveCriticalSection(&m_CS);

    DeleteCriticalSection(&m_CS);
}

//����һ��EventParam������RltEventParamID
bool CFTEventParam::CreateEventParam(DWORD& RltEventParamID,DWORD* pEventID,string* pEventString,const void* pEventPtr,int EventPtrLen,int EventUserInt)
{
    EnterCriticalSection(&m_CS);

    time_t curtime;
    time(&curtime);

    //�����û�г�ʱ��ʱ�����
    map<DWORD,Stru_EventParam>::iterator it=m_mapEvtParam.begin();
    while(it!=m_mapEvtParam.end())
    {
        if(curtime<it->second.CreateTime||
            curtime>it->second.CreateTime&&curtime-it->second.CreateTime>=60)
        {
            it=m_mapEvtParam.erase(it);
        }
        else 
        {
            it++;
        }
    }

    //�����¼�����
    Stru_EventParam EvtParam;
    EvtParam.BaseID=m_BaseID++;
    time(&EvtParam.CreateTime);
    EvtParam.EvtID = (pEventID!=NULL ? (*pEventID) : 0);
    EvtParam.EvtString.clear();
    if(pEventString) EvtParam.EvtString=*pEventString;
    EvtParam.EvtPtr=NULL;
    EvtParam.EvtPtrLen=0;
    if(pEventPtr&&EventPtrLen>0)
    {
        EvtParam.EvtPtr=(char*)malloc(EventPtrLen);
        if(EvtParam.EvtPtr!=NULL)
        {
            memcpy((char*)EvtParam.EvtPtr,(char*)pEventPtr,EventPtrLen);
            EvtParam.EvtPtrLen=EventPtrLen;
        }
    }
    EvtParam.EvtUserInt=EventUserInt;

    RltEventParamID=EvtParam.BaseID;

    m_mapEvtParam.insert(pair<DWORD,Stru_EventParam>(EvtParam.BaseID,EvtParam));

    LeaveCriticalSection(&m_CS);

    return true;
}

//ɾ��һ��EventParma��Ӧ�ò�ȷ������ʹ�ú����ɾ��
bool CFTEventParam::DeleteEventParam(DWORD EventParamID)
{
    bool brlt=false;

    EnterCriticalSection(&m_CS);

    map<DWORD,Stru_EventParam>::iterator it=m_mapEvtParam.find(EventParamID);
    if(it!=m_mapEvtParam.end())
    {
        char *ptmp=it->second.EvtPtr;
        m_mapEvtParam.erase(it);
        if(ptmp) free(ptmp);
        brlt=true;
    }

    LeaveCriticalSection(&m_CS);

    return brlt;
}

//��ȡ�¼����������pRltEventPtrΪ�գ�����ֻ��ȡpRltEventPtrLenֵ��pRltEventID��pRltEventString��pRltEventPtrΪ��ʱ��������Ӧֵ
bool CFTEventParam::GetEventParam(DWORD EventParamID,DWORD* pRltEventID,string* pRltEventString,void* pRltEventPtr,int* pRltEventPtrLen,int MaxRltEventPtrLen,int* pRltEventUserInt)
{
    bool brlt=false;

    EnterCriticalSection(&m_CS);

    map<DWORD,Stru_EventParam>::iterator it=m_mapEvtParam.find(EventParamID);
    if(it!=m_mapEvtParam.end())
    {
        if(it->second.EvtPtr!=NULL&&pRltEventPtr!=NULL&&it->second.EvtPtrLen>MaxRltEventPtrLen)
        {
            brlt=false;
        }
        else
        {
            if(pRltEventID!=NULL)
                *pRltEventID=it->second.EvtID;
            if(pRltEventString!=NULL)
                *pRltEventString=it->second.EvtString;
            if(pRltEventPtrLen!=NULL)
                *pRltEventPtrLen=it->second.EvtPtrLen;
            if(it->second.EvtPtr!=NULL&&pRltEventPtr!=NULL&&it->second.EvtPtrLen<=MaxRltEventPtrLen)
                memcpy((char*)pRltEventPtr,it->second.EvtPtr,it->second.EvtPtrLen);
            if(pRltEventUserInt!=NULL)
                *pRltEventUserInt=it->second.EvtUserInt;
            brlt=true;
        }
    }
    else 
    {
        brlt=false;
    }

    LeaveCriticalSection(&m_CS);
    return brlt;
}
