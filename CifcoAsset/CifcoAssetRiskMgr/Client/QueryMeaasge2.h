#pragma once

//#ifndef _QueryMeaasge2_H_
//#define _QueryMeaasge2_H_

#include <string>
#include "CDataInfo.h"
using namespace std;

#define QUERY_SEND  0
#define QUERY_RECV   1
class QueryMeaasge2
{
public:
	QueryMeaasge2(void);
	~QueryMeaasge2(void);
	public:
	//�����¼�
	static void SubscribeEvt(unsigned int hWnd);
	//�˶��¼�
	static void UnsubscribeEvt(unsigned int hWnd);

	//�������������Ϣ֪ͨ
	void ReqQueryMessage(std::string strDate,int flag);

	bool ProcessMessageInfoEvtPkg(const char* pPkg,std::map<int, MessageInfo>& mapMessageInfo,int fieldcnt);
	bool ProcessMessageStatusEvtPkg(const char* pPkg,std::multimap<int, MsgSendStatus>& mapMsgSendStatus,int fieldcnt);

	public:
	//��������
	CDataInfo* m_pdataInfo;
	std::map <int,int>                  m_mapMessageID2RowID;
};
