/*
 * msg.h
 *
 *  ����֪ͨ��Ϣ�����й�
 *	ֻ��Ӧ����Windows���
 *
 *  Created on: May 20, 2013
 *      Author: root
 */

#ifdef WIN32

#ifndef CLIENTTRADINGMSG_H_
#define CLIENTTRADINGMSG_H_

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)


#include <string>
using std::string;
#include <memory>
using std::auto_ptr;

#pragma managed(push,off)

class CPostMsgByEventParam
{
    //��ȡ/����Singleton��. ���캯��Ϊprivate, ��������Ϊpublic(Ϊ����auto_ptr����������)
    public: static CPostMsgByEventParam& getObj(void);
    private:static auto_ptr<CPostMsgByEventParam> m_pObj;
    private: CPostMsgByEventParam();
    public: virtual ~CPostMsgByEventParam();


public:
	void setMsgReceiver(int hwnd) { m_hwnd_MsgReceiver=hwnd; };

	bool postMsgStr(int msgID,unsigned int evtID,const string& msgstring,const void* pdata=NULL,int datalen=0,int evtUserInt1=0,int evtUserInt2=0);
	bool postMsgStr2(int hwnd,int msgID,unsigned int evtID,const string& msgstring,const void* pdata=NULL,int datalen=0,const void* pdata2=NULL,int datalen2=0,int evtUserInt1=0,int evtUserInt2=0);
	bool postMsgStrThread(unsigned long idThread,int msgID,unsigned int evtID,const string& msgstring,const void* pdata=NULL,int datalen=0,const void* pdata2=NULL,int datalen2=0,int evtUserInt1=0,int evtUserInt2=0);

private:
	int	m_hwnd_MsgReceiver;
};



#pragma managed(pop)

#endif /* CLIENTTRADINGMSG_H_ */

#endif /* WIN32 */