#pragma once

#include "Interface_SvrCommon.h"

#ifndef SVRBROKERINFO_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRUSERORG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRBROKERINFO_EXPORTS
#define SVRBROKERINFO_API __declspec(dllexport)
#else
#define SVRBROKERINFO_API __declspec(dllimport)
#endif
#endif

#include <map>
#include <vector>
#include "CommonStruct.h"

class SVRBROKERINFO_API CInterface_SvrBrokerInfo : public CInterface_SvrCommon
{
public:

    //��ѯ����
    virtual CF_ERROR ResponseToAsk(const eAskType& neDataType,const eAskOperationType& neChangeType,const int nllDataID,const char * npDataBuf)=0;

    static CInterface_SvrBrokerInfo& getObj(void);

    //��ȡ���о��͹�˾��Ϣ
	virtual void GetBrokerInfos(std::vector<BrokerInfo>& vec) = 0;
    //ͨ�����͹�˾ID��ȡ����Ϣ
    virtual bool GetBrokerInfoByID(int brokerID,BrokerInfo& brokerInfo) = 0;
    //ͨ�����͹�˾�����ȡ����Ϣ
    virtual bool GetBrokerInfoByCode(std::string strCode,BrokerInfo& brokerInfo)=0;

    //��ȡ���з�������
	virtual void GetServerGroups(std::vector<ServerGroup>& vec) = 0;
    //��ȡһ�����͹�˾ID�µ����з���������Ϣ
	virtual void GetServerGroupsByBrokerID(int nBrokerID, std::vector<ServerGroup>& vec) = 0;
    //ͨ����������ID��ȡ����������Ϣ
	virtual bool GetServerGroupByID(int nGroupID, ServerGroup& group) = 0;

    //��ȡ���з��������ַ��Ϣ
	virtual void GetServerAddr(std::vector<ServerAddress>& vec) = 0;
    //��ȡһ����������ID�µ����з��������ַ��Ϣ
	virtual void GetServerAddrByGroupID(int nGroupID, std::vector<ServerAddress>& vec) = 0;

    //��ȡ����ί�н����˺���Ϣ
	virtual void GetTradeAccounts(std::vector<TradeAccount>& vec) = 0;
    //��ȡһ�����͹�˾ID�µ�����ί�н����˺���Ϣ
	virtual void GetTradeAccountsByBrokerID(int nBrokerID, std::vector<TradeAccount>& vec) = 0;
    //��ȡһ����������ID�µ�����ί�н����˺���Ϣ
	virtual void GetTradeAccountsByGroupID(int nGroupID, std::vector<TradeAccount>& vec) = 0;
    //ͨ��ί�н����˺�ID��ȡ����Ϣ
	virtual bool GetTradeAccount(int nTradeAccountID, TradeAccount& account) = 0;

    //�ж��Ƿ��н���������飨������������ã�
    virtual bool GetSettleServerGroup() = 0;


    //��ȡ����ί�н����˺ź���֯�����Ĺ�ϵ
    virtual void GetTradeAccountAndOrgRelation(std::vector<TradeAccountAndOrg>& vec) = 0;
    //ͨ��ί�н����˺�ID��ȡ��֯����ID
    virtual void GetOrgIDByTradeAccountID(const int& nTradeAccountID, int& nOrgID) = 0;
    //ͨ��ί�н����˺��ַ�����ȡ��֯����ID
    virtual void GetOrgIDByBrokerCodeAndTradeAccount(const std::string& szBrokerCode, const std::string& szTradeAccount, int& nOrgID) = 0;
    //ͨ����֯����ID�õ�ί�н����˻�
    virtual void GetTradeAccountByOrgID(const int& nOrgID, std::vector<TradeAccount>& vec) = 0;

};







