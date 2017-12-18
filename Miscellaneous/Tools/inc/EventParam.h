#ifndef EVENTPARAM_H_
#define EVENTPARAM_H_

//���岢ά��Event�Ĳ���
//1. ��֤�̰߳�ȫ
//2. ÿ��EventParam��һ��Ψһ��EventParamID����־��EventParam
//3. ÿ��EventParam����ʱ�����Ϊ8�룬��ʱɾ��
//4. ����ʼʱInit������˳�ʱRelease����������ReleaseҲ��
//5. ���Ǹ���ʵ����

#include <time.h>
#include <map>
#include <string>
#include <memory>
using std::auto_ptr;
using std::string;
#include "easymutex.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 

#ifndef MAXEVENTPARAM
#define MAXEVENTPARAM 100000
#endif

//ͳ�����ĵ�cpu tick��
#define CALCUCPUTIME_EVENTPARAM

class CEventParam
{
public:
	struct Stru_EventParam
	{
	public:
		Stru_EventParam(int& evtCnt,int& maxEvtCnt,int& curMemUsage,int& maxMemUsage)
		:EventParamID(0),
		 CreateTime(0),
		 EvtID(0),
         EvtStringLen(0),
		 EvtDataLen(0),
		 EvtDataLen2(0),
		 EvtUserInt(0),
		 EvtUserInt2(0),
         pdata(NULL),
		 EvtCnt(evtCnt),
		 MaxEvtCnt(maxEvtCnt),
		 CurMemUsage(curMemUsage),
		 MaxMemUsage(maxMemUsage)
		{
            CurMemUsage+=(int)sizeof(Stru_EventParam);
            if(MaxMemUsage<CurMemUsage) MaxMemUsage=CurMemUsage;
		}
		~Stru_EventParam()
		{
			clear();

            CurMemUsage-=(int)sizeof(Stru_EventParam);
		}
		void clear(void)
		{
			if(EventParamID!=0)
				EvtCnt--;

			EventParamID=0;
			CreateTime=0;
			EvtID=0;
            if(pdata&&pdata!=(char*)LittleEvtBuf) 
            {
                free(pdata);
                CurMemUsage-=EvtStringLen+EvtDataLen+EvtDataLen2;
            }
            pdata=NULL;
            EvtStringLen=0;
			EvtDataLen=0;
			EvtDataLen2=0;
			EvtUserInt=0;
			EvtUserInt2=0;
		}
		void set(unsigned int eventParamID,
			unsigned int eventID,
			const char* peventString,int eventStringLen,
			const void* peventData,int eventDataLen,const void* peventData2,int eventDataLen2,
			int eventUserInt,int eventUserInt2)
		{
			clear();
			set_internal(eventParamID,eventID,peventString,eventStringLen,peventData,eventDataLen,peventData2,eventDataLen2,eventUserInt,eventUserInt2);
			if(EventParamID!=0) 
			{
				EvtCnt++;
                if(MaxEvtCnt<EvtCnt) MaxEvtCnt=EvtCnt;
			}
		}

	private:
		//ע�⣬���û��clear()
		void set_internal(unsigned int eventParamID,
			unsigned int eventID,
			const char* peventString,int eventStringLen,
			const void* peventData,int eventDataLen,const void* peventData2,int eventDataLen2,
			int eventUserInt,int eventUserInt2)
		{
			EventParamID=eventParamID;
			CreateTime=time(NULL);
			EvtID=eventID;
			EvtUserInt=eventUserInt;
			EvtUserInt2=eventUserInt;

            pdata=NULL;
            EvtStringLen=0;
            EvtDataLen=0;
            EvtDataLen2=0;
            if(!peventString&&eventStringLen) eventStringLen=0;
			if(!peventData&&eventDataLen) eventDataLen=0;
			if(!peventData2&&eventDataLen2) eventDataLen2=0;
			if(eventStringLen>0||eventDataLen>0||eventDataLen2>0)
			{
				if(eventStringLen+eventDataLen+eventDataLen2<=(int)sizeof(LittleEvtBuf)) pdata=(char*)LittleEvtBuf;
                else 
                {
                    pdata=(char*)malloc(eventStringLen+eventDataLen+eventDataLen2);
                    CurMemUsage+=eventStringLen+eventDataLen+eventDataLen2;
                    if(MaxMemUsage<CurMemUsage) MaxMemUsage=CurMemUsage;
                }

                if(pdata)
				{
					if(eventStringLen>0)
					{
						EvtStringLen=eventStringLen;
                        memcpy(pdata,peventString,EvtStringLen);
					}
					if(eventDataLen>0)
					{
						EvtDataLen=eventDataLen;
                        memcpy(pdata+EvtStringLen,peventData,EvtDataLen);
					}
					if(eventDataLen2>0)
					{
						EvtDataLen2=eventDataLen2;
                        memcpy(pdata+EvtStringLen+EvtDataLen,peventData2,EvtDataLen2);
					}
				}
			}
		}

	public:
		unsigned int EventParamID;
		time_t CreateTime;
		unsigned int EvtID;
        int EvtStringLen;
		int EvtDataLen;
		int EvtDataLen2;
		int EvtUserInt;
		int EvtUserInt2;
        char* pdata;

		int& EvtCnt;
		int& MaxEvtCnt;
		int& CurMemUsage;
		int& MaxMemUsage;

	private:
		char LittleEvtBuf[1024];

    private:
        //����������������
		Stru_EventParam(const Stru_EventParam&);
		Stru_EventParam& operator=(const Stru_EventParam&);
	};


    //��ȡ/����Singleton��. ���캯��Ϊprivate, ��������Ϊpublic(Ϊ����auto_ptr����������)
    public: static CEventParam& getObj(void);
    private:static auto_ptr<CEventParam> m_pObj;
    private: CEventParam();
    public: virtual ~CEventParam();

public:

    //����һ��EventParam������RltEventParamID��ע�⣺peventString���ַ���
    bool CreateEventParam(unsigned int& RltEventParamID,unsigned int eventID,const char* peventString=NULL,const void* peventData=NULL,int eventDataLen=0,int eventUserInt=0,int eventUserInt2=0);
    bool CreateEventParam2(unsigned int& RltEventParamID,unsigned int eventID,const char* peventString=NULL,const void* peventData=NULL,int eventDataLen=0,const void* peventData2=NULL,int eventDataLen2=0,int eventUserInt=0,int eventUserInt2=0);
    //ɾ��һ��EventParma��Ӧ�ò�ȷ������ʹ�ú����ɾ��
    bool DeleteEventParam(unsigned int EventParamID);
    //��ȡ�¼����������pRltEventPtrΪ�գ�����ֻ��ȡpRltEventPtrLenֵ��pRltEventID��pRltEventString��pRltEventPtrΪ��ʱ��������Ӧֵ��
	bool GetEventParam(
        unsigned int EventParamID,
        unsigned int* pRltEventID=NULL,
        string* pRltEventString=NULL,
        void* pRltEventData=NULL,int* pRltEventDataLen=NULL,int MaxRltEventDataLen=0,
        int* pRltEventUserInt=NULL,
        int* pRltEventUserInt2=NULL);

	bool GetEventParam2(
        bool bDel,
        unsigned int EventParamID,
        unsigned int* pRltEventID=NULL,
        string* pRltEventString=NULL,
        void* pRltEventData=NULL,int* pRltEventDataLen=NULL,int MaxRltEventDataLen=0,
        void* pRltEventData2=NULL,int* pRltEventDataLen2=NULL,int MaxRltEventDataLen2=0,
        int* pRltEventUserInt=NULL,
        int* pRltEventUserInt2=NULL);

	bool GetEventParam3(
        unsigned int EventParamID,
        unsigned int* pRltEventID=NULL,
        string* pRltEventString=NULL,
        int* pRltEventDataLen=NULL,
        int* pRltEventDataLen2=NULL,
        int* pRltEventUserInt=NULL,
        int* pRltEventUserInt2=NULL);

private:
    Stru_EventParam *m_pEvtParams[MAXEVENTPARAM];       //����EvtParam���ϡ�EventParamID==0�ı�ʾ��Ч
    unsigned int m_FirstValidEvtParam;                  //��һ����ЧEvtParam

private:
	unsigned int EventParamBaseID;
	Ceasymutex m_mutex;

	//CEventParam();
	//virtual ~CEventParam();
	//static CEventParam* m_pObj;

public:
	int CPUTickCounter_CreateEventParam;
	int	CreateEventParamCnt;
	int CPUTickCounter_GetEventParam;
	int	GetEventParamCnt;
	int CPUTickCounter_DeleteEventParam;
	int	DeleteEventParamCnt;

    int EvtCnt;
    int MaxEvtCnt;
	int CurMemUsage;
	int MaxMemUsage;

};

#ifdef WIN32
#pragma managed(pop)
#endif 


#endif
