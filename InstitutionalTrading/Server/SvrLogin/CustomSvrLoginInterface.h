#pragma  once
#include "stdafx.h"
#include "Interface_SvrLogin.h"
#include "SocketInfoMgr.h"

class CSlaveInfo
{
private:
	std::string msSlaverUserName;
	std::string msSlaverUserPass;
	std::set<std::string> msSetSlaverIp;
public:
	void SetUserAnsPass(const std::string& nUserName,const std::string& nUserPass)
	{
		msSlaverUserName = nUserName;
		msSlaverUserPass = nUserPass;
	}

	void GetUserAndPass(std::string& nUserName,std::string& nUserPass)
	{
		nUserName = msSlaverUserName;
		nUserPass = msSlaverUserPass;
	}
	void AddIP(const std::string& nIP)
	{
		msSetSlaverIp.insert(nIP);
	}
	CF_ERROR ValidateSlaverUser(const std::string& nUserName,const std::string& nUserPass,const std::string& nIP,int& nUserID)
	{
		if(nUserName != msSlaverUserName)
			return CF_ERROR_LOGIN_USER_NOT_EXIST;

		if(nUserPass != msSlaverUserPass)
			return CF_ERROR_LOGIN_PASS_INVALIDATE;

		if(msSetSlaverIp.find(nIP) == msSetSlaverIp.end())
			return CF_ERROR_LOGIN_IP_INVALIDATE;

		nUserID = SLAVE_USER_ID;
		return CF_ERROR_SUCCESS;
	}
};

class CCunstomSvrLoginInterface :public CInterface_SvrLogin
{
	//����getObj()������/����������m_pObj���ڶ��嵥ʵ����
public:
	//���ڻ�ȡ�ӿڶ���ͨ���ö���ʹ�ýӿڷ���
	static CCunstomSvrLoginInterface& getObj(void);
private:
	CCunstomSvrLoginInterface();
	virtual ~ CCunstomSvrLoginInterface();
	static CCunstomSvrLoginInterface* m_pObj;
	int mnCurrentAvailableFrontID;
	int mnCurrentAvailableSessionID;
private:

	std::string msTodayString;
	sExchangeTimeInfo mExchangeTimeInfo;
	_MapSocketAndSocketInfo mmapScoketAndSocketInfo;
	_MultiMapUserIDAndSocketInfo mmapUserIdAndSocketInfo;
	_MapFrontIDAndSessionAndSocketInfo mmapFrontIdAndSocketInfo;
	_MultiMapUserNameAndSocketInfo mmapUserNameAndSocketInfo;
	std::map<std::string,bool> mmapUserDataReady; 
    _MapSocketAndTraderIDs mmapSocketAndTraderIDs;
	
	bool mbSlave;
	CSlaveInfo mSlaver;
	std::set<UserType> mAllowLoginUserType;
	
	//��Ҫ����ͬ����OnlineUser
	std::map<int,int> mmapOnlineUser;
	std::set<SOCKET>  msetSubsribleSocket;
    
public:
	bool InsertSocketInfo(const sSocketInfo & nSocketInfo);
	bool RemoveSocket(const SOCKET & nSocketHandle,int & nUserId);

	//ά���ӷ�������mmapSocketAndTraderIDs
	bool InsertSocketInfo2(SOCKET nSocket,int nTraderID);
	bool RemoveSocket2(const SOCKET & nSocketHandle,const int & nTraderID);


	bool GetTimeInfo(sExchangeTimeInfo & nTimeInfo);
	bool GetUserDataReady(const std::string &nUserName);

	bool CloseAllTraderSocket();
	bool CloseUserSocket(int  nUserId);
	bool CloseAllSocket();

	bool GetOnlineUser(std::set<int> & nVecUsreID);

	void GetUserAndPass(std::string& nUserName,std::string& nUserPass){mSlaver.GetUserAndPass(nUserName,nUserPass);}

public:
	void AddSubscribSocket(SOCKET nSocket) { msetSubsribleSocket.insert(nSocket);}
	void RemoveSubscribSocket(SOCKET nSocket) { msetSubsribleSocket.erase(nSocket);}
	void InitAddOnlineUser(int nUserId,int nCount)
	{
		mmapOnlineUser[nUserId] = nCount;
	}
	void AddOnlineUser(int nUserId) 
	{ 
#ifdef _DEBUG
        LOGIN_LOG("AddOnlineUser before    ");
        for (std::map<int,int>::iterator it = mmapOnlineUser.begin();it!=mmapOnlineUser.end();it++)
        {  
            LOGIN_LOG("nUserID=%d nCount=%d",it->first,it->second);
        }
#endif

		if(mmapOnlineUser.find(nUserId) == mmapOnlineUser.end())
			mmapOnlineUser[nUserId] = 1;
		else
		{
			int n = mmapOnlineUser[nUserId] ;
			mmapOnlineUser[nUserId] = ++n;
		}

#ifdef _DEBUG
        LOGIN_LOG("AddOnlineUser after    ");
        for (std::map<int,int>::iterator it = mmapOnlineUser.begin();it!=mmapOnlineUser.end();it++)
        {  
            LOGIN_LOG("AddOnlineUser after    nUserID=%d nCount=%d",it->first,it->second);
        }
#endif
		PushSubscribeSocketAllUserStatus();
	}
	void RemoveOnlineUser(int nUserId)
	{
#ifdef _DEBUG
        LOGIN_LOG("RemoveOnlineUser before  ");    
        for (std::map<int,int>::iterator it = mmapOnlineUser.begin();it!=mmapOnlineUser.end();it++)
        {  
            LOGIN_LOG("nUserID=%d nCount=%d",it->first,it->second);
        }
#endif

        if(mmapOnlineUser.find(nUserId) != mmapOnlineUser.end())		
		{
			int n = mmapOnlineUser[nUserId] ;
			if(n > 1)
				mmapOnlineUser[nUserId] = --n;
			else
				mmapOnlineUser.erase(nUserId);
		}
#ifdef _DEBUG
        LOGIN_LOG("RemoveOnlineUser after   ");    
        for (std::map<int,int>::iterator it = mmapOnlineUser.begin();it!=mmapOnlineUser.end();it++)
        {  
            LOGIN_LOG("nUserID=%d nCount=%d",it->first,it->second);
        }
#endif
		PushSubscribeSocketAllUserStatus();
	}
	void GetAllOnlineUserStatus(std::vector<sUserOnlineInfo>& nvecInfo)
	{
		std::map<int,int>::iterator lIter = mmapOnlineUser.begin();
		for(;lIter != mmapOnlineUser.end(); lIter++)
		{
			sUserOnlineInfo s;
			s.mUserID = lIter->first;
			s.mUserOnlineCount = lIter->second;
			nvecInfo.push_back(s);
		}
	}

	void GetOnlineUserStatus(int nUserId ,sUserOnlineInfo& nInfo)
	{
		nInfo.mUserID = nUserId;
		nInfo.mUserOnlineCount = 0;
		std::map<int,int>::iterator lIter = mmapOnlineUser.find(nUserId);
		if(lIter != mmapOnlineUser.end())
		{			
			nInfo.mUserID = lIter->first;
			nInfo.mUserOnlineCount = lIter->second;			
		}

	}
	void PushSubscribeSocketAllUserStatus();
	

	CF_ERROR IsAllowedLoginUserType(UserType nType)
	{
		if(mAllowLoginUserType.find(nType) != mAllowLoginUserType.end())
			return CF_ERROR_SUCCESS;
		else
			return CF_ERROR_LOGIN_USERTYPE_INVALIDATE;
	}
public:
	//---------------------------------------------------------------------------------
		//����Ψһ��FrontID��SessionID
	//---------------------------------------------------------------------------------
	virtual void GenerateUniqueFrontAndSessionID(int & nFrontID,int & nSessionID);


	////---------------------------------------------------------------------------------
	////  ���ý�����ʱ��
	////---------------------------------------------------------------------------------
	virtual void  SetTimeInfo(const sExchangeTimeInfo& nsTimeInfo) ;

	//---------------------------------------------------------------------------------
	//	��ȡsocket�Ķ�Ӧ��UserID,FrontID,SessionID
	//  �ɹ�����CF_ERROR_SUCCESS
	//  ʧ�ܷ���CF_ERROR_LOGIN_SOCKET_NOT_EXIST
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetSocketUserIDAndFrontSessionID(const SOCKET&,int & nUsrID,int & nFrontID,int& nSessionID) ;


	//---------------------------------------------------------------------------------
	//	��ȡFrontID,SessionID�Ķ�Ӧ��socket
	//  �ɹ�����CF_ERROR_SUCCESS
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetSocketHandleWithFrontAndSessionID(const int & nFrontID,const int& nSessionID,SOCKET & nhShocketHandle) ;



	//---------------------------------------------------------------------------------
	//	��ȡUserID�Ķ�Ӧ��socket�б�
	//  �ɹ�����CF_ERROR_SUCCESS
	//  �ɹ�����CF_ERROR_SUCCESS
	//  ʧ�ܷ���CF_ERROR_LOGIN_USER_NOT_EXIST
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetUserSockets(const int & nUserID, std::vector<SOCKET> & nVecSocket);

	//---------------------------------------------------------------------------------
	//	��ȡUserName�Ķ�Ӧ��socket�б�
	//  �ɹ�����CF_ERROR_SUCCESS
	//  �ɹ�����CF_ERROR_SUCCESS
	//  ʧ�ܷ���CF_ERROR_LOGIN_USER_NOT_EXIST
	//---------------------------------------------------------------------------------
	virtual CF_ERROR GetUserSockets(const std::string & nUserName, std::vector<SOCKET> & nVecSocket, int & nUserID) ;


	//---------------------------------------------------------------------------------
	//	�����û��Ľ��������Ƿ�׼������	
	//---------------------------------------------------------------------------------
	virtual void SetUserDataReady(const std::string & nUserName,bool nbReady) ;



	//---------------------------------------------------------------------------------
	//	�ж�����
	//---------------------------------------------------------------------------------
	virtual bool IsSlaver() ;

	//---------------------------------------------------------------------------------
	//	�жϴӷ����û����ݵ���Ч��
	//---------------------------------------------------------------------------------
	virtual CF_ERROR IsValidSlaveUser(const std::string& strUser, const std::string& strPwd,const std::string& strIP, int& nUserID) ;

	virtual bool GetUserTypeSocket(UserType  nUserType,std::vector<SOCKET> & nVecSocket);

	
};