#pragma once


#include <string>
#include "CDataInfo.h"
#include "UserApiStruct.h"


using namespace std;

#define       NoChoice           "" 
#define       FD_In              "���" 
#define       FD_Out             "����" 

class QueryTransfer2
{
public:
	//�����¼�
	static void SubscribeEvt(unsigned int hWnd);
	//�˶��¼�
	static void UnsubscribeEvt(unsigned int hWnd);

	//�����������Ͷ���߳����
    void ReqQueryTransfer(std::set<int> setAccntID);
	//����������Ϣ
    bool ProcessTransferEvtPkg(const char* pPkg,std::map<int, SeqDepositField>& mapSeqDeposit,int fieldcnt,string& Direction);
	// ����ַ���
	std::vector<std::string> split(const std::string &str);
	//doubleתstring
	string Double2String(double dbData);


    QueryTransfer2(void);
    virtual ~QueryTransfer2(void);
public:
	//��������
	CDataInfo* m_pdataInfo;
	MAPTrustTradeAccount	m_mapTrustTradeAccount;

};

