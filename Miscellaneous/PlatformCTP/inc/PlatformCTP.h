/***************************************************************************//**
*   @file          PlatformCTP.h
*   @brief        �ۺϽ���ƽ̨(CTP)���׽ӿ�
*
********************************************************************************/
#ifndef _PLATFORMCTP_H_
#define _PLATFORMCTP_H_

#include <string>
#include <vector>
#include <map>
using std::vector;
using std::string;
using std::multimap;


#include "VariablePackage.h"
#include "easymutex.h"
#include "easytradingtimespan.hpp"


#pragma managed(push,off)

#include "ctp/ThostFtdcTraderApi.h"
#include "ctp/ThostFtdcMdApi.h"


//���涨�彻��������״̬
enum eCTPCONNSTATUS
{    
    CTPCONNSTATUS_Disconnected   =      0       ,           //δ����
    CTPCONNSTATUS_Connecting     =      1       ,           //��������
    CTPCONNSTATUS_Logining       =      2       ,           //���ڵ�¼
    CTPCONNSTATUS_LoginFailure   =      3       ,           //��¼ʧ��
    CTPCONNSTATUS_Connected      =      6       ,           //���ӵ�¼�ɹ�
    CTPCONNSTATUS_Disconnecting  =      7       ,           //���ڶϿ�����
    CTPCONNSTATUS_UnInit         =      8                   //δ��ʼ��
};


//��������
enum ePkgCmd
{
    CmdID_Quot_FrontConnected           =13000,			    //�������̨ͨ�����ӳɹ�
    CmdID_Quot_FrontDisconnected,                           //�������̨ͨ�����ӶϿ�
    CmdID_Quot_RspUserLogin,                                //��¼������Ӧ��
    CmdID_Quot_RspUserLogout,                               //�ǳ�������Ӧ��
    CmdID_Quot_RtnDepthMarketData,                          //��������

    CmdID_Trade_FrontConnected          =13200,             //�뽻��ctpͨ�����ӳɹ�
    CmdID_Trade_FrontDisconnected,                          //�뽻��ctpͨ�����ӶϿ�
    CmdID_Trade_RspUserLogin,                               //��¼������Ӧ��
    CmdID_Trade_RspUserLogout,                              //�ǳ�������Ӧ��
    CmdID_Trade_RspQrySettlementInfoConfirm,                //�����ѯ������Ϣȷ����Ӧ
    CmdID_Trade_RspSettlementInfoConfirm,                   //���������Ϣȷ����Ӧ

    CmdID_Trade_RspUserPasswordUpdate,                      //�û��������������Ӧ
    CmdID_Trade_RspTradingAccountPasswordUpdate,            //�ʽ��˻��������������Ӧ
    CmdID_Trade_RspOrderInsert,                             //����¼��������Ӧ
    CmdID_Trade_RspOrderAction,                             //��������������Ӧ
    CmdID_Trade_RspParkedOrderInsert,                       //Ԥ��¼��������Ӧ
    CmdID_Trade_RspParkedOrderAction,                       //Ԥ�񳷵�¼��������Ӧ
    CmdID_Trade_RspRemoveParkedOrder,                       //ɾ��Ԥ����Ӧ    
    CmdID_Trade_RspRemoveParkedOrderAction,                 //ɾ��Ԥ�񳷵���Ӧ
    CmdID_Trade_RspQryParkedOrder,                          //�����ѯԤ����Ӧ
    CmdID_Trade_RspQryParkedOrderAction,                    //�����ѯԤ�񳷵���Ӧ
    CmdID_Trade_RspQryOrder,                                //�����ѯ������Ӧ
    CmdID_Trade_RspQryTrade,                                //�����ѯ�ɽ���Ӧ
    CmdID_Trade_RspQryTradingAccount,                       //�����ѯ�ʽ��˻���Ӧ
    CmdID_Trade_RspQryInvestor,                             //�����ѯͶ������Ӧ
    CmdID_Trade_RspQryInstrumentMarginRate,                 //�����ѯ��Լ��֤������Ӧ   
    CmdID_Trade_RspQryInstrumentCommissionRate,             //�����ѯ��Լ����������Ӧ
    CmdID_Trade_RspQryInstrument,                           //�����ѯ��Լ��Ӧ
    CmdID_Trade_RspQrySettlementInfo,                       //�����ѯͶ���߽�������Ӧ
    CmdID_Trade_RspQryInvestorPositionDetail,               //�����ѯͶ���ֲ߳���ϸ��Ӧ
    CmdID_Trade_RspQryInvestorPosition,                     //�����ѯͶ���ֲ߳���Ӧ
    CmdID_Trade_RspQryInvestorPositionCombDetail,           //�����ѯͶ���ֲ߳���ϸ��Ӧ
    CmdID_Trade_RspQryNotice,                               //�����ѯ�ͻ�֪ͨ��Ӧ
    CmdID_Trade_RtnOrder,                                   //�����ر�(����¼���뱨��������������ûر�)
    CmdID_Trade_RtnTrade,                                   //�ɽ��ر�
    CmdID_Trade_ErrRtnOrderInsert,                          //����¼���ִ((�����ִ����������Ϊ¼����Ч)
    CmdID_Trade_ErrRtnOrderAction,                          //����������ִ(�����ִ����������Ϊ������Ч)
    CmdID_Trade_RtnInstrumentStatus,                        //��Լ����״̬֪ͨ
    CmdID_Trade_RtnTradingNotice                            //����֪ͨ
};



//CTP���Ӳ���
struct Stru_CTPConnectionParam
{
public:
    Stru_CTPConnectionParam()
    {
    };
    Stru_CTPConnectionParam(const vector<string>& frontAddrs,const string& brokerID,const string& userID,const string& userPW)
    {
        FrontAddrs=frontAddrs;
        BrokerID=brokerID;
        UserID=userID;
        UserPW=userPW;
		InvestorID=userID;
    };
    Stru_CTPConnectionParam(const Stru_CTPConnectionParam& r)
    {
        *this=r;
    };
	Stru_CTPConnectionParam& operator= (const Stru_CTPConnectionParam& r)
    {
        ProxyConnectStr=r.ProxyConnectStr;
        FrontAddrs=r.FrontAddrs;
        BrokerID=r.BrokerID;
        UserID=r.UserID;
        UserPW=r.UserPW;
        InvestorID=r.InvestorID;
        OneTimePassword=r.OneTimePassword;
        return *this;
    };
    
    string          ProxyConnectStr;                        //�����������ַ
    vector<string>  FrontAddrs;                             //����ǰ�û���ַ�飬���ƶ��"58.246.49.130:41213"
    string          BrokerID;
    string          UserID;
    string          UserPW;
    string          InvestorID;
	string          OneTimePassword;                        //�ͻ�����ǿ��֤ʱ�ĵ�������
};


//CTP�����ӿ�
class CPlatformCTP
{
public:
	enum eMsgID
	{
		QuotConnMsg=1500,		//Quot����״̬�ı�, lParamΪEventID, EventParam��stringΪ: Quot: ConnStr, UserIntΪeCTPCONNSTATUS
		TradeConnMsg,			//Trade����״̬�ı�, lParamΪEventID, EventParam��stringΪ: Trade: ConnStr, UserIntΪeCTPCONNSTATUS
		DataArrivedMsg			//����(�����顢�ɽ���)����, lParamΪEventParamID, EvtIDΪ��������(ePkgCmd), EventParam��pEventData��pEventData2Ϊ������, userInt1��userInt2ΪRequestID��bLast
	};

public:
    //ConnParam: ���Ӳ���
    //bPushCache: �Ƿ���Ҫ��ctp���ص����ݷŵ�m_Cache�У��ȴ������̴߳���(������FastTrader����ʽ)
    //bUserComb: �Ƿ�ʹ����ϵ�����
    //bCalcuCombQuotOnLegQuot: �����ϵ����Ƿ���ݵ��Ⱥ�Լ�����Զ�������Ϻ�Լ����
    CPlatformCTP(const Stru_CTPConnectionParam& ConnParam=Stru_CTPConnectionParam(),bool bPushCache=true,bool bUseComb=false,bool bCalcuCombQuotOnLegQuot=false);

	//��ȡ/��������״̬
	static char* GetConnStatusName(eCTPCONNSTATUS sta);
    eCTPCONNSTATUS GetConnStatus(void) const { return m_ConnStatus; }
	const char* GetConnStatus2(void) const;
	time_t GetConnStatusTime(void) const { return m_ConnStatusTime; }
    void SetConnStatus(eCTPCONNSTATUS connsta,bool bQuot);

	//��ȡ/�������Ӳ���
    Stru_CTPConnectionParam GetConnParam(void) const { CeasymutexGuard guard(m_mutex);  return m_ConnParam; }
    void SetConnParam(Stru_CTPConnectionParam& ConnParam) { CeasymutexGuard guard(m_mutex);  m_ConnParam=ConnParam; }

    //��Cache�л�ȡ���ݰ�
    bool IsEmptyCache(void) { CeasymutexGuard guard(m_mutex);  return m_Cache.IsEmpty(); }
	CVariablePackage::HeadStru PeekFromCache(void* pData1,int DataSize1,void* pData2=NULL,int DataSize2=0) { CeasymutexGuard guard(m_mutex);  return m_Cache.PeekFrom(pData1,DataSize1,pData2,DataSize2); }
    void DeleteCurPackageFromCache() { CeasymutexGuard guard(m_mutex);  m_Cache.DeleteCurPackage(); }
    bool IsPushCache(void) { return m_bPushCache; }

	//ȡָ���������Ľ���ʱ��
	static CEasyTradingTimespan GetTradingTimespan(const string& strExchangeID)
	{
		CEasyTimespan<int> ts_IntraDay;
		if(strExchangeID==string("CFFEX"))
		{	
			//�н�����Լ 9:15-11:30 13:00-15:15, total 4.5 hours (2h15'+2h15')
			ts_IntraDay.insert(9*3600+15*60,11*3600+30*60);
			ts_IntraDay.insert(13*3600,15*3600+15*60);
		}
		else 
		{
			//��Ʒ�ڻ� 9:00-10:15 10:30-11:30 13:00-15:00, total 4h15'(1h15'+1h+2h)
			ts_IntraDay.insert(9*3600,10*3600+15*60);
			ts_IntraDay.insert(10*3600+30*60,11*3600+30*60);
			ts_IntraDay.insert(13*3600+30*60,15*3600);
		}
		CEasyTradingTimespan tts(true,ts_IntraDay);
		return tts;
	}


protected:
    //���������ַ���
    //���������tcp://...
    //ʹ�ô���socks5:// asp-sim2-front1.financial-trading-platform.com:26205/user:pass@127.0.0.1:10001
    string CPlatformCTP::MakeConnectStr(const string& addr,const string& ProxyStr);

public:
    Ceasymutex              m_mutex;

protected:
    Stru_CTPConnectionParam m_ConnParam;

    CVariablePackage        m_Cache;                        //�������ķ��ص����ݱ��浽��cache�У�
    bool                    m_bPushCache;                   //ctp���ص������Ƿ���Ҫpush��cache��

	bool                    m_bUseComb;						//�Ƿ�����Ϻ�Լ������
	bool                    m_bCalcuCombQuotOnLegQuot;		//�Ƿ���ݷ������������Ϻ�Լ������

private:
    eCTPCONNSTATUS          m_ConnStatus;					//����״̬
	time_t					m_ConnStatusTime;				//����״̬��ʱ��

public:
    int                     m_RequestID;                    //һ��������ID

protected:
	multimap<ePkgCmd,int>	m_subscribePkg;					//����ָ����Ϣ�Ĵ��ھ��
public:
	void SubscribePkg(ePkgCmd pkgID,int hWnd);
	void UnsubscribePkg(ePkgCmd pkgID,int hWnd);
	void UnsubscribePkg(int hWnd);
	void UnsubscribePkgAll();

protected:
	//���ֻ������������ʹ�ã���ʹ���߸������
	void PostPkg_Internal(ePkgCmd pkgID,void* pdata,int datalen,void* pdata2=NULL,int datalen2=0,int userInt1=0,int userInt2=0);



private:
    CPlatformCTP(const CPlatformCTP&);
	CPlatformCTP& operator= (const CPlatformCTP&);
};


#pragma managed(pop)

#endif

