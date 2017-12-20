#pragma once

#include "Interface_SvrMsg.h"
#include <map>
#include <set>


using namespace std;

class CReal_SvrMsg : public CInterface_SvrMsg
{
	//---------------------------------------------------------------------------------
	//	���ݽӿ�
	//---------------------------------------------------------------------------------
	bool SendMsg(int nOwnerID, int nTargetUserID, int nMsgType, 
						 int nExpiredDate, const string& strTitle, const string& strMsg);
	bool SendMsg(int nOwnerID, const set<int>& setTargetUser, int nMsgType, 
						 int nExpiredDate, const string& strTitle, const string& strMsg);
public:
    CReal_SvrMsg();
    virtual ~ CReal_SvrMsg();
};







