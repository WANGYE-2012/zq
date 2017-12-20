#pragma once
#include "stdafx.h"
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p);  (p)=NULL;} }
#endif
#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&); \
	void operator=(const TypeName&)
#endif
template<class T>
class CMsgQueue
{
public:
	CMsgQueue(void)
	{
		InitializeCriticalSection(&m_CS);
	}

	virtual ~CMsgQueue(void)
	{
		DeleteCriticalSection(&m_CS);
	}
	void DelMsg()
	{
		EnterCriticalSection(&m_CS);
		m_DataQueue.clear();
		LeaveCriticalSection(&m_CS);
	}
	void AddMsg(const T& pParam)
	{
		EnterCriticalSection(&m_CS);		
		m_DataQueue.push(pParam);
		LeaveCriticalSection(&m_CS);
	}
	bool GetMsg(T& pParam)
	{
		bool bRet=false;
		EnterCriticalSection(&m_CS);
		int count=m_DataQueue.size();		
		if(count>0)
		{			
			T& data=m_DataQueue.front();
			memcpy(&pParam,&data,sizeof(T));
			m_DataQueue.pop();			
			bRet=true;
		}
		LeaveCriticalSection(&m_CS);
		return bRet;
	}
	bool GetMsgVector(std::vector<T>& pParamVector)
	{
		bool bRet=false;
		EnterCriticalSection(&m_CS);
		int count=m_DataQueue.size();		
		while(count>0)
		{		
			T pParam;
			T& data=m_DataQueue.front();
			memcpy(&pParam,&data,sizeof(T));
			m_DataQueue.pop();			
			pParamVector.push_back(pParam);
			count=m_DataQueue.size();	
		}
		if(pParamVector.size() >0)
			bRet = true;
		LeaveCriticalSection(&m_CS);
		return bRet;
	}
protected:
	std::queue<T> m_DataQueue;
	CRITICAL_SECTION m_CS;
private:
	DISALLOW_COPY_AND_ASSIGN(CMsgQueue);
};
enum EnumQueryContent
{
	QUERYCOTENT_NULL = 1,
	QUERYCOTENT_INSTRUMENT,//��Լ
	QUERYCOTENT_ORDER,     //����
	QUERYCOTENT_TRADE,     //�ɽ�	
	QUERYCOTENT_COMMISSION,//��Լ��������
	QUERYCOTENT_FUND,		//�ʽ�
	QUERYCOTENT_POSITION,   //�ֲ�
	QUERYCOTENT_POSITIONDETAIL,//�ֲ���ϸ

	QUERYCOTENT_MARGIN,    //��֤����
};
enum EnumQueryStatus
{
	QUERYSTATUS_INIT = 1,//��ʼ����ѯ״̬
	QUERYSTATUS_BEGIN,   //��ѯ��ʼ
	QUERYSTATUS_FAILURE, //��ѯʧ��
	QUERYSTATUS_SUCCESS, //��ѯ�ɹ�

};
struct STradeQueryData 
{
	EnumQueryContent			enumQueryContent;	  //��ѯ������
	std::string					strInstrument;		  //��ѯ�ĺ�Լ
	std::string					strHedgeFlag;		 //Ͷ���ױ���־����ѯ��֤���ʱ���õ�
	DWORD						dwBeginTickCount;    //��ʼʱ�� 
	int							nRepeatCount;		 //���ԵĴ��� Ŀǰ�ظ�3��
	EnumQueryStatus				enumStatus;			  //��ѯ״̬	
	STradeQueryData()
	{
		enumQueryContent = QUERYCOTENT_NULL;
		strInstrument    = "";
		strHedgeFlag	 = THOST_FTDC_HF_Speculation;
		dwBeginTickCount = GetTickCount();
		nRepeatCount     = 0;
		enumStatus		 = QUERYSTATUS_INIT;
	}
	STradeQueryData& operator= (const STradeQueryData& r)
	{
		enumQueryContent = r.enumQueryContent;
		strInstrument	 = r.strInstrument;
		strHedgeFlag	 = r.strHedgeFlag;
		dwBeginTickCount = r.dwBeginTickCount;
		nRepeatCount     = r.nRepeatCount;
		enumStatus		 = r.enumStatus;	

		return *this;
	}	
};
class  CTradeDataMsgQueue:
	public CMsgQueue<STradeQueryData>
{
public:
	CTradeDataMsgQueue(void){}
	virtual ~CTradeDataMsgQueue(void){}
	void AddMsg(STradeQueryData& sData)
	{
		EnterCriticalSection(&m_CS);
		m_DataQueue.push(sData);
		LeaveCriticalSection(&m_CS);
	}
	int  GetSize()
	{
		int nSize = 0;
		EnterCriticalSection(&m_CS);
		nSize = m_DataQueue.size();
		LeaveCriticalSection(&m_CS);

		return nSize;
	}	
};
