#ifndef _RiskEvt2_H_
#define _RiskEvt2_H_

#include <string>
using namespace std;

class RiskEvt2
{
public:
	//�����¼�
	static void SubscribeEvt(unsigned int hWnd);
	//�˶��¼�
	static void UnsubscribeEvt(unsigned int hWnd);

	//���������������¼�
	static void ReqRiskEvt(void);

	//������������͵ķ����¼���Ϣ
	static bool ProcessRiskEvtPkg(const char* pPkg,string*pstr,int fieldcnt);

};

#endif


