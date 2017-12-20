#pragma once
#include <vector>
#define  ERROR_PROMPT            "��ʾ"
#define  ERROR_CANNOTSCREENING   "�����ѯ����δɸѡ���κ��ʺ�"
#define  ERROR_QUERY_TIMEOUT     "��ѯ��ʱ"
#define  ERROR_QUERY_NONDATA     "��ѯ�޷�������"
#define  ERROR_QUERY_DATEPROMPT  "��ʼ���ڱ���С�ڵ��ڽ�������"
class CQueryData
{
public:
	CQueryData(void);
public:
	static CQueryData* NewInstance();
	static void DestroyInstance();
	static CQueryData* m_pInstance;

	//��ѯSEQ��¼
	void SetSendSeq(int nSeq);
	void GetSendSeq(std::vector<int>&   vecSeq);
	void ReceiveSeq(int nSeq);
	bool IsEmpty();
	void ClearSendSeq();

private:
	 std::vector<int>   m_vecSeq;

};
