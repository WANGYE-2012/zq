#pragma  once

#include "CommonStruct.h"
#include <map>
#include "SocketBase.h"

struct sSocketInfo
{

	int mnUserID;
	UserType meUserType;
	_USERSTRING msUserName;
	int    mnFrontID;                   //ǰ��ID
	int    mnSessionID;                 //�ỰID
	int    mnMinOrderref;               //��С��Orderref	

	SOCKET mhSocket;
	//time_t mtimeBreakheart;             //���һ���յ������ĸ���ʱ��
	//eSocketStatus meSocketStatus;

	sSocketInfo()
	{
		meUserType = USER_TYPE_INVALID;
		mnUserID = mhSocket =  mnFrontID = mnSessionID = mnMinOrderref = -1;
//		meSocketStatus = SocketStatus_Exception;
		memset(msUserName,0,sizeof(msUserName));
//		memset(&mtimeBreakheart,0,sizeof(mtimeBreakheart));
	}
	sSocketInfo(const int nUserID,
		const UserType neUserTye,
		const _USERSTRING nsUserName,
		const int nFrontID,
		const int nSessionID,
		const int nMinOrderRef,
		const SOCKET nhSocket):
			 mnUserID(nUserID),
			 meUserType(neUserTye),
			 mnFrontID(nFrontID),
			 mnSessionID(nSessionID),
			 mnMinOrderref(nMinOrderRef),
			 mhSocket(nhSocket)
    {
		strcpy(msUserName,nsUserName);
		//time(&mtimeBreakheart);
	}
	/*void UpdateTime()
	{
		time(&mtimeBreakheart);
	}*/
};



///��ʾһ��������¼
struct FrontAndSessionIdKey
{
	FrontAndSessionIdKey()
	{
		FrontID=-1;
		SessionID=-1;
	};

	FrontAndSessionIdKey(int fid, int sid)
	{		
		FrontID=fid;
		SessionID=sid;
	
	};



	FrontAndSessionIdKey& operator= (const FrontAndSessionIdKey& other)
	{
		if (this == &other)
			return *this;

		memcpy((char*)this,(char*)&other,sizeof(*this));
		return *this;
	};

	bool operator< (const FrontAndSessionIdKey& r) const
	{
		
		if(FrontID<r.FrontID)
			return true;
		if(FrontID>r.FrontID)
			return false;

		if(SessionID<r.SessionID)
			return true;
		if(SessionID>r.SessionID)
			return false;


		return false;
	}

	bool operator== (const FrontAndSessionIdKey& r) const
	{
	
		if(FrontID!=r.FrontID) return false;

		if(SessionID!=r.SessionID) return false;	

		return true;
	}

	bool operator> (const FrontAndSessionIdKey& r) const
	{
		return !((*this) < r || (*this) == r);
	}


	int                     FrontID;            //ǰ�û�����࣬ͨ����ID��ǲ�ͬ��ǰ�û�
	int                     SessionID;          //ֻҪ���µ��룬�ͻ�仯��
};
typedef std::map<SOCKET,sSocketInfo>      _MapSocketAndSocketInfo;
typedef std::multimap<int,sSocketInfo>    _MultiMapUserIDAndSocketInfo;
typedef std::map<FrontAndSessionIdKey,sSocketInfo>      _MapFrontIDAndSessionAndSocketInfo;
typedef std::multimap<std::string,sSocketInfo>    _MultiMapUserNameAndSocketInfo;
//�������ά���ӷ���SocketID�͵�¼�ӷ���Ĳ���Ա��TraderID��
typedef std::map<SOCKET,std::vector<int>>   _MapSocketAndTraderIDs;
