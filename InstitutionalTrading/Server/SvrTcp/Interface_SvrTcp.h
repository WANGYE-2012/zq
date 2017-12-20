#pragma once

#include "Interface_SvrCommon.h"

#ifndef SVRTCP_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRTCP_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRTCP_EXPORTS
#define SVRTCP_API __declspec(dllexport)
#else
#define SVRTCP_API __declspec(dllimport)
#endif
#endif

#include "CommonStruct.h"
#include "EventParam.h"
#include <map>
#include <set>
using std::map;
using std::set;
using std::pair;

class SVRTCP_API CInterface_SvrTcp : public CInterface_SvrCommon
{
public:
	//���ڻ�ȡ�ӿڶ���ͨ���ö���ʹ�ýӿڷ���
	static CInterface_SvrTcp& getObj(void);


	//---------------------------------------------------------------------------------
	//	���ݽӿ�
	//---------------------------------------------------------------------------------

	//ҵ��ģ�鶩����Ϣ����ָ����ϢID���߳�ID���յ����ݰ���Post����Ӧ�߳���
	virtual void SubscribePkg(unsigned int CmdID,unsigned int idThread)=0;
	//ģ���˶���Ϣ����ָ����ϢID���߳�ID
	virtual void UnsubscribePkg(unsigned int CmdID,unsigned int idThread)=0;
	//ģ���˶�ȫ����Ϣ����ָ����ϢID���߳�ID
	virtual void UnsubscribeAllPkg(unsigned int idThread)=0;

	//�ж�Socket����Ƿ���TcpSvr�ľ����true: ��һ��TcpSvr accept�õ�������socket; false: ��һ��TcpClient��socket
	virtual bool IsTcpSvr(int socket)=0;
	//��ȡSocket�����Ӧ��Զ�˵�ַ
	virtual bool GetRemoteAddr(int socket,unsigned int& outip,unsigned short& outport)=0;
	//��ȡ����TcpClient���ӵ�socket�����Զ�˵�ַ
	virtual void GetSocket_RemoteAddr_of_TcpClient(map<int,pair<unsigned int,unsigned short>>& outData)=0;
	//��ȡ����TcpSvr���ӵ�socket�����Զ�˵�ַ
	virtual void GetSocket_RemoteAddr_of_TcpServer(map<int,pair<unsigned int,unsigned short>>& outData)=0;

	//��ȡ������RequestID
	virtual unsigned int get_inc_RequestID(void)=0;

	//��������
	virtual bool SendData(int hsocket,const char* pData,int DataLen)=0;
	//��������, ��sendData��һ����װ
	virtual bool SendPkgData(int hsocket,unsigned int nCMDID, const void* pData, int nLen, unsigned int seq,
		unsigned int Subseq=0,unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0)=0;

	//�ر�һ���ͻ���socket
	virtual void Disconnect(int hsocket)=0;

	//��ȡEventParam�����ö�������ȡ�¼���������
	virtual CEventParam& getEventParamObj(void)=0;


	//*******************************************************************************
	//socket�ĵ�¼�����Ϣ
	//*******************************************************************************
	struct Stru_LoginInfo
	{
		Stru_LoginInfo()
		{
			memset(this,0,sizeof(*this));
		};
		Stru_LoginInfo(int hsocket)
		{
			memset(this,0,sizeof(*this));
			hSocket=hsocket;
		}
		Stru_LoginInfo(int hsocket,int frontID,int sessionID)
		{
			memset(this,0,sizeof(*this));
			hSocket=hsocket;
			FrontID=frontID;
			sessionID=SessionID;
		};

		int			hSocket;
		int			FrontID;
		int			SessionID;
		UserType	Usertype;
		int			UserID;
		char		UserName[32];
		bool		bLogin;
	};

	//����/��ȡSocket�ĵ�¼��Ϣ
	virtual bool SetLoginInfo(int hsocket,const Stru_LoginInfo& logininfo)=0;
	virtual bool GetLoginInfo(int hsocket,Stru_LoginInfo& logininfo)=0;

	//��ȡ�������ӵ�������������socket��������������Ǵӷ��񣬻�ȡ�������socket��������������������񣬻�ȡ�Ѿ���¼�ɹ������дӷ�����socket
	virtual set<int> GetAllServerSockets()=0;

};







