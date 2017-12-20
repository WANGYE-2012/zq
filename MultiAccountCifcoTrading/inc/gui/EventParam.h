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
using namespace std;

struct Stru_EventParam
{
    DWORD BaseID;
    time_t CreateTime;
    DWORD EvtID;
    string EvtString;
    int EvtPtrLen;
    char *EvtPtr;
    int EvtUserInt;
};

class CEventParam
{
public:
    //����ʼʱInit
    static void Init(void);         

    //�����˳�ʱRelease��Ҳ���Բ�Release���ò���ϵͳ�Զ��ͷ���Դ
    static void Release(void);             

    //����һ��EventParam������RltEventParamID
    static bool CreateEventParam(DWORD& RltEventParamID,DWORD* pEventID,string* pEventString,const void* pEventPtr,int EventPtrLen,int EventUserInt);
    //ɾ��һ��EventParma��Ӧ�ò�ȷ������ʹ�ú����ɾ��
    static bool DeleteEventParam(DWORD EventParamID);
    //��ȡ�¼����������pRltEventPtrΪ�գ�����ֻ��ȡpRltEventPtrLenֵ��pRltEventID��pRltEventString��pRltEventPtrΪ��ʱ��������Ӧֵ
    static bool GetEventParam(DWORD EventParamID,DWORD* pRltEventID,string* pRltEventString,void* pRltEventPtr,int* pRltEventPtrLen,int MaxRltEventPtrLen,int* pRltEventUserInt);

private:
    static CRITICAL_SECTION m_CS;
    static DWORD m_BaseID;
    static map<DWORD,Stru_EventParam> m_mapEvtParam;

};
