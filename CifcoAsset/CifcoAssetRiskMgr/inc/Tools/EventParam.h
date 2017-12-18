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
using namespace std;

#ifdef WIN32
#pragma managed(push,off)
#endif 


struct Stru_EventParam
{
public:
	Stru_EventParam(unsigned int eventParamID,unsigned int eventID,const char* peventString,const void* peventData,int eventDataLen,int eventUserInt)
	:EventParamID(eventParamID),
	 CreateTime(time(NULL)),
	 EvtID(eventID),
	 EvtString(peventString?peventString:""),
	 EvtDataLen(0),
	 pEvtData(NULL),
	 EvtUserInt(eventUserInt)
	{
		if(peventData&&eventDataLen>0)
		{
			if(eventDataLen<=(int)sizeof(LittleEvtBuf))
			{
				pEvtData=LittleEvtBuf;
				EvtDataLen=eventDataLen;
			}
			else
			{
				pEvtData=malloc(eventDataLen);
				if(pEvtData)
					EvtDataLen=eventDataLen;
			}

			if(pEvtData&&EvtDataLen>0)
				memcpy((char*)pEvtData,peventData,EvtDataLen);
		}
	}
	Stru_EventParam(const Stru_EventParam& r)
	:EventParamID(0),
	 CreateTime(0),
	 EvtID(0),
	 EvtString(),
	 EvtDataLen(0),
	 pEvtData(NULL),
	 EvtUserInt(0)
	{
		*this=r;
	}
	~Stru_EventParam()
	{
		if(pEvtData&&(char*)pEvtData!=(char*)LittleEvtBuf)
		{
			free(pEvtData);
			pEvtData=NULL;
		}
	}
	virtual Stru_EventParam& operator=(const Stru_EventParam& r)
	{
		EventParamID=r.EventParamID;
		CreateTime=r.CreateTime;
		EvtID=r.EvtID;
		EvtString=r.EvtString;
		EvtUserInt=r.EvtUserInt;
		if(r.pEvtData&&r.EvtDataLen>0)
		{
			if(r.EvtDataLen<=(int)sizeof(LittleEvtBuf))
			{
				pEvtData=LittleEvtBuf;
				EvtDataLen=r.EvtDataLen;
			}
			else
			{
				pEvtData=malloc(r.EvtDataLen);
				if(pEvtData)
					EvtDataLen=r.EvtDataLen;
			}

			if(pEvtData&&EvtDataLen>0)
				memcpy((char*)pEvtData,r.pEvtData,EvtDataLen);
		}
		return *this;
	}

    unsigned int EventParamID;
    time_t CreateTime;
    unsigned int EvtID;
    string EvtString;
    int EvtDataLen;
    void *pEvtData;
    int EvtUserInt;

private:
	char LittleEvtBuf[4096];
};

class CEventParam
{
public:
	//ȡ���󣬱�Ҫʱ��������
	static CEventParam& getObj(void);

    //����һ��EventParam������RltEventParamID
    bool CreateEventParam(unsigned int& RltEventParamID,unsigned int eventID,const char* peventString=NULL,const void* peventData=NULL,int eventDataLen=0,int eventUserInt=0);
    //ɾ��һ��EventParma��Ӧ�ò�ȷ������ʹ�ú����ɾ��
    bool DeleteEventParam(unsigned int EventParamID);
    //��ȡ�¼����������pRltEventPtrΪ�գ�����ֻ��ȡpRltEventPtrLenֵ��pRltEventID��pRltEventString��pRltEventPtrΪ��ʱ��������Ӧֵ
    bool GetEventParam(unsigned int EventParamID,unsigned int* pRltEventID,string* pRltEventString,void* pRltEventData,int* pRltEventDataLen,int MaxRltEventDataLen,int* pRltEventUserInt);

private:
    map<unsigned int,Stru_EventParam> m_mapEvtParam;

	CEventParam();
	virtual ~CEventParam();
	static CEventParam* m_pObj;
};

#ifdef WIN32
#pragma managed(pop)
#endif 


#endif