#pragma once

//���岢ά��Event�Ĳ���
//1. ��֤�̰߳�ȫ
//2. ÿ��EventParam��һ��Ψһ��EventParamID����־��EventParam
//3. ÿ��EventParam����ʱ�����Ϊ8�룬��ʱɾ��
//4. ����ʼʱInit������˳�ʱRelease����������ReleaseҲ��

#include <windows.h>
#include <time.h>
#include <string>
#include <map>

#ifdef WIN32
#pragma managed(push,off)
#endif 

struct Stru_EventParam
{
    DWORD BaseID;
    time_t CreateTime;
    DWORD EvtID;
    std::string EvtString;
    int EvtPtrLen;
    char *EvtPtr;
    int EvtUserInt;
};

class CFTEventParam
{
public:
    //����ʼʱInit
    static void Init(void);         

    //�����˳�ʱRelease��Ҳ���Բ�Release���ò���ϵͳ�Զ��ͷ���Դ
    static void Release(void);             

    //����һ��EventParam������RltEventParamID
    static bool CreateEventParam(DWORD& RltEventParamID,DWORD* pEventID,std::string* pEventString,const void* pEventPtr,int EventPtrLen,int EventUserInt);
    //ɾ��һ��EventParma��Ӧ�ò�ȷ������ʹ�ú����ɾ��
    static bool DeleteEventParam(DWORD EventParamID);
    //��ȡ�¼����������pRltEventPtrΪ�գ�����ֻ��ȡpRltEventPtrLenֵ��pRltEventID��pRltEventString��pRltEventPtrΪ��ʱ��������Ӧֵ
    static bool GetEventParam(DWORD EventParamID,DWORD* pRltEventID,std::string* pRltEventString,void* pRltEventPtr,int* pRltEventPtrLen,int MaxRltEventPtrLen,int* pRltEventUserInt);

private:
    static CRITICAL_SECTION m_CS;
    static DWORD m_BaseID;
    static std::map<DWORD,Stru_EventParam> m_mapEvtParam;

};

#ifdef WIN32
#pragma managed(pop)
#endif 
