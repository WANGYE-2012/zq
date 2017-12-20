#pragma once

#include "Interface_SvrCommon.h"
#include <map>
#include <set>


using namespace std;

#ifndef SVRMSG_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRMSG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRMSG_EXPORTS
#define SVRMSG_API __declspec(dllexport)
#else
#define SVRMSG_API __declspec(dllimport)
#endif
#endif


class SVRMSG_API CInterface_SvrMsg : public CInterface_SvrCommon
{
public:
    static CInterface_SvrMsg& getObj(void);


	//---------------------------------------------------------------------------------
	//	���ݽӿ�
	//---------------------------------------------------------------------------------

	// �򵥸�Ŀ���û�������Ϣ
	// nOwnerID����Ϣ������
	// nTargetUserID��Ŀ���û�
	// nMsgType����Ϣ����
	// nExpiredDate����Ч����
	// strTitle����Ϣ����
	// strMsg����Ϣ����
	// ����ֵ��true����Ϣ�����棻false����Ϣ�޷����淢�͡�
	virtual bool SendMsg(int nOwnerID, int nTargetUserID, int nMsgType, 
						 int nExpiredDate, const string& strTitle, const string& strMsg) = 0;
	
	// ����Ŀ���û�������Ϣ
	// nOwnerID����Ϣ������
	// setTargetUser��Ŀ���û��ļ���
	// nMsgType����Ϣ����
	// nExpiredDate����Ч����
	// strTitle����Ϣ����
	// strMsg����Ϣ����
	// ����ֵ��true����Ϣ�����棻false����Ϣ�޷����淢�͡�
	virtual bool SendMsg(int nOwnerID, const set<int>& setTargetUser, int nMsgType, 
						 int nExpiredDate, const string& strTitle, const string& strMsg) = 0;
private:
    static CInterface_SvrMsg* m_pObj;
};







