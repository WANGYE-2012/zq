#pragma once

#include "Interface_SvrCommon.h"
#include "CommonStruct.h"
#include "socketBase.h"
#include <vector>

#ifndef SVRLOGIN_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRLOGIN_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRLOGIN_EXPORTS
#define SVRLOGIN_API __declspec(dllexport)
#else
#define SVRLOGIN_API __declspec(dllimport)
#endif
#endif


class SVRLOGIN_API CInterface_SvrLogin : public CInterface_SvrCommon
{
public:
    static CInterface_SvrLogin& getObj(void);

	//---------------------------------------------------------------------------------
	//����Ψһ��FrontID��SessionID
	//---------------------------------------------------------------------------------
	virtual void GenerateUniqueFrontAndSessionID(int & nFrontID,int & nSessionID) = 0;


	////---------------------------------------------------------------------------------
	////  ���ý�����ʱ��
	////---------------------------------------------------------------------------------
	virtual void  SetTimeInfo(const sExchangeTimeInfo& nsTimeInfo) = 0;



	//---------------------------------------------------------------------------------
	//	��ȡsocket�Ķ�Ӧ��UserID,FrontID,SessionID
	//  �ɹ�����CF_ERROR_SUCCESS
	//  ʧ�ܷ���CF_ERROR_LOGIN_SOCKET_NOT_EXIST
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetSocketUserIDAndFrontSessionID(const SOCKET&,int & nUsrID,int & nFrontID,int& nSessionID) = 0;

	//---------------------------------------------------------------------------------
	//	��ȡFrontID,SessionID�Ķ�Ӧ��socket
	//  �ɹ�����CF_ERROR_SUCCESS
	//  �ɹ�����CF_ERROR_SUCCESS
	//  ʧ�ܷ���CF_ERROR_LOGIN_SOCKET_NOT_EXIST
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetSocketHandleWithFrontAndSessionID(const int & nFrontID,const int& nSessionID,SOCKET & nhShocketHandle) = 0;



	//---------------------------------------------------------------------------------
	//	��ȡUserID�Ķ�Ӧ��socket�б�
	//  �ɹ�����CF_ERROR_SUCCESS
	//  �ɹ�����CF_ERROR_SUCCESS
	//  ʧ�ܷ���CF_ERROR_LOGIN_USER_NOT_EXIST
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetUserSockets(const int & nUserID, std::vector<SOCKET> & nVecSocket) = 0;



	//---------------------------------------------------------------------------------
	//	��ȡUserName�Ķ�Ӧ��socket�б�
	//  �ɹ�����CF_ERROR_SUCCESS
	//  �ɹ�����CF_ERROR_SUCCESS
	//  ʧ�ܷ���CF_ERROR_LOGIN_USER_NOT_EXIST
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetUserSockets(const std::string & nUserName, std::vector<SOCKET> & nVecSocket,int & nUserID) = 0;


	//---------------------------------------------------------------------------------
	//	�����û��Ľ��������Ƿ�׼������	
	//---------------------------------------------------------------------------------
	virtual void SetUserDataReady(const std::string & nUserName,bool nbReady) = 0;



	//---------------------------------------------------------------------------------
	//	�ж�����
	//---------------------------------------------------------------------------------
	virtual bool IsSlaver() = 0;



	//---------------------------------------------------------------------------------
	//	�жϴӷ����û����ݵ���Ч��
	//---------------------------------------------------------------------------------
	virtual CF_ERROR IsValidSlaveUser(const std::string& strUser, const std::string& strPwd,const std::string& strIP, int& nUserID) = 0;


	//---------------------------------------------------------------------------------
	//	��ȡĳһ���û���socket
	//---------------------------------------------------------------------------------
	virtual bool GetUserTypeSocket(UserType  nUserType,std::vector<SOCKET> & nVecSocket) = 0;


	//---------------------------------------------------------------------------------
	//	��ȡĳһ���û����������
	//---------------------------------------------------------------------------------
	virtual void GetAllOnlineUserStatus(std::vector<sUserOnlineInfo>& nvecInfo)= 0;

	virtual void GetOnlineUserStatus(int nUserId ,sUserOnlineInfo& nInfo)= 0;

};







