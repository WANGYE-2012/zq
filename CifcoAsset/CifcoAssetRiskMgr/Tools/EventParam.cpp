#include "StdAfx.h"
#include "EventParam.h"
#include "easymutex.h"

#pragma unmanaged

static unsigned int EventParamBaseID=1;
static Ceasymutex emutex;
CEventParam* CEventParam::m_pObj=NULL;


CEventParam::CEventParam()
{
}

CEventParam::~CEventParam()
{
}

CEventParam& CEventParam::getObj(void)
{
	emutex.lock();
    if(!m_pObj)
        m_pObj=new CEventParam();
    emutex.unlock();
    return *m_pObj;
}


//����һ��EventParam������RltEventParamID
bool CEventParam::CreateEventParam(unsigned int& RltEventParamID,unsigned int eventID,const char* peventString,const void* peventData,int eventDataLen,int eventUserInt)
{
	emutex.lock();

    time_t curtime;
    time(&curtime);

    //�����û�г�ʱ��ʱ�����
    map<unsigned int,Stru_EventParam>::iterator it=m_mapEvtParam.begin();
    while(it!=m_mapEvtParam.end())
    {
        if(curtime<it->second.CreateTime||curtime>it->second.CreateTime&&curtime-it->second.CreateTime>=60)
            it=m_mapEvtParam.erase(it);
        else it++;
    }

    //�����¼�����
	RltEventParamID=++EventParamBaseID;
    Stru_EventParam EvtParam(RltEventParamID,eventID,peventString,peventData,eventDataLen,eventUserInt);

	m_mapEvtParam.insert(pair<unsigned int,Stru_EventParam>(EvtParam.EventParamID,EvtParam));

    emutex.unlock();

    return true;
}

//ɾ��һ��EventParma��Ӧ�ò�ȷ������ʹ�ú����ɾ��
bool CEventParam::DeleteEventParam(unsigned int EventParamID)
{
    bool brlt=false;

    emutex.lock();

    map<unsigned int,Stru_EventParam>::iterator it=m_mapEvtParam.find(EventParamID);
    if(it!=m_mapEvtParam.end())
    {
        m_mapEvtParam.erase(it);
        brlt=true;
    }

    emutex.unlock();

    return brlt;
}

//��ȡ�¼����������pRltEventPtrΪ�գ�����ֻ��ȡpRltEventPtrLenֵ��pRltEventID��pRltEventString��pRltEventPtrΪ��ʱ��������Ӧֵ
bool CEventParam::GetEventParam(unsigned int EventParamID,unsigned int* pRltEventID,string* pRltEventString,void* pRltEventData,int* pRltEventDataLen,int MaxRltEventDataLen,int* pRltEventUserInt)
{
    bool brlt=false;

    emutex.lock();

    map<unsigned int,Stru_EventParam>::iterator it=m_mapEvtParam.find(EventParamID);
    if(it!=m_mapEvtParam.end())
    {
        if(it->second.pEvtData!=NULL&&pRltEventData!=NULL&&it->second.EvtDataLen>MaxRltEventDataLen)
        {
            brlt=false;
        }
        else
        {
            if(pRltEventID!=NULL)
                *pRltEventID=it->second.EvtID;
            if(pRltEventString!=NULL)
                *pRltEventString=it->second.EvtString;
            if(pRltEventDataLen!=NULL)
                *pRltEventDataLen=it->second.EvtDataLen;
            if(it->second.pEvtData!=NULL&&pRltEventData!=NULL&&it->second.EvtDataLen<=MaxRltEventDataLen)
                memcpy((char*)pRltEventData,it->second.pEvtData,it->second.EvtDataLen);
            if(pRltEventUserInt!=NULL)
                *pRltEventUserInt=it->second.EvtUserInt;
            brlt=true;
        }
    }
    else 
    {
        brlt=false;
    }

    emutex.unlock();
    return brlt;
}
