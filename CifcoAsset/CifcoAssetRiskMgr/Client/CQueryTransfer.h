#ifndef _CQueryTransfer_H_
#define _CQueryTransfer_H_

#include <string>
using namespace std;

class CQueryTransfer
{
public:
	//�����¼�
	static void SubscribeEvt(unsigned int hWnd);
	//�˶��¼�
	static void UnsubscribeEvt(unsigned int hWnd);

	//�����������Ͷ���߳����
	static void ReqQueryTransfer(std::string strAccountID);



};

#endif
