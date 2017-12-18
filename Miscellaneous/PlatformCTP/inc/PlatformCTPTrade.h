/***************************************************************************//**
*   @file          PlatformCTPTrade.h
*   @brief        �ۺϽ���ƽ̨(CTP)���׽ӿ�
*
********************************************************************************/
#ifndef _PLATFORMCTPTRADE_H_
#define _PLATFORMCTPTRADE_H_

#include <string>
#include <set>
#include <map>
#include <vector>
using std::vector;
using std::string;
using std::map;
using std::set;

#include "VariablePackage.h"
#include "easymutex.h"

#include "PlatformCTP.h"


#pragma managed(push,off)



//���׽ӿ�
class CPlatformCTPTrade : public CThostFtdcTraderSpi, public CPlatformCTP
{

	//******************************************************************************************
	//	���涨���ѯ���д�����
	//******************************************************************************************
	class CQryQueue
	{
	public:
		//******************************************************************************************
		//	��ѯ��������ʹ�õĺ�
		//******************************************************************************************
		enum eQryID
		{
			QryID_ReqQryNotice=							1001,	//�����ѯ�ͻ�֪ͨ
			QryID_ReqQrySettlementInfoConfirm,                  //�����ѯ������Ϣȷ��
			QryID_ReqQrySettlementInfo,							//�����ѯ������Ϣ

			QryID_ReqQryInstrument,                             //��ѯ��Լ��Ϣ
			QryID_ReqQryOrder,                                  //��ѯ����
			QryID_ReqQryTrade,                                  //��ѯ�ɽ�
			QryID_ReqQryInvestorPosition,                       //��ѯ�ֲֻ���
			QryID_ReqQryInvestorPositionDetail,                 //��ѯ�ֲ���ϸ
			QryID_ReqQryInvestorPositionCombineDetail,          //�����ѯͶ���ֲ߳���ϸ(���)
			QryID_ReqQryTradingAccount,                         //��ѯ�ʽ�

			QryID_ReqQryInstrumentCommissionRate,               //��ѯ��Լ��������
			QryID_ReqQryInstrumentMarginRate                    //�����ѯ��Լ��֤����
		};


	public:
		CQryQueue(CPlatformCTPTrade& RefTrade);
		~CQryQueue(void);

		void ClearbInQry(void) { m_bInQry=false; }

		//�Ƿ񵽴��ѯʱ�̡�ctp�涨ÿ��ֻ����һ�β�ѯ 
		bool IsQryTime(void);
		bool IsCacheEmpty(void);
		//���Ͳ�ѯ���󣬷���ֵ��ʾ�����Ƿ�ɹ�
		//ReqRlt��ʾ��������˲�ѯ����ѯ����ֵ��<0��ʾ��ѯʧ��
		bool SendQryCmd(int& ReqRlt);
		//�̺߳���
		static unsigned int ThreadProc(void *arg);

	public:
		CPlatformCTPTrade&	m_RefTrade;

		CVariablePackage	m_ReqQueue;							//��ѯ���С����а��Ƚ��ȳ�˳���ѯ
		bool				m_bInQry;							//�Ƿ��ڲ�ѯ;��
		unsigned int		m_QryTime;							//��ѯʱ��(tickcount)

		bool				m_bNeedExit;                        //�߳��Ƿ���Ҫ�˳�
		int					m_hThread;                          //�߳̾��

		map<string,unsigned int> m_QryCommissionRateTime;		//ͬһ����Լ����ѯһ�κ󣬱����60s���ٴβ�ѯ
		map<string,int> m_QryCommissionRateCount;
		map<string,unsigned int> m_QryMarginRateTime;
		map<string,int> m_QryMarginRateCount;
	};

public:
    //******************************************************************************************
    //  ConnParam:              ���Ӳ���
    //  bPushCache:             �Ƿ���Ҫ��ctp���ص����ݷŵ�m_Cache�У��ȴ������̴߳���(������FastTrader����ʽ)
    //  bUserComb:              �Ƿ�ʹ����ϵ�����
    //  bCalcuCombQuotOnLegQuot:�����ϵ����Ƿ���ݵ��Ⱥ�Լ�����Զ�������Ϻ�Լ����
    //  Ver_String:             �汾�ţ���"1,6,0,13"
	//	bAutoReqSettlementInfoConfirmed:���ӹ������Ƿ��ѯ����ȷ����Ϣ
    //  bAutoConfirmSettlement: ���ӹ������Ƿ��Զ����н���ȷ��
	//	bAutoQryInstrumentInfo: ��¼���Զ���ѯ��Լ��Ϣ
    //******************************************************************************************
    CPlatformCTPTrade(const Stru_CTPConnectionParam& ConnParam=Stru_CTPConnectionParam(),
        bool bPushCache=true,bool bUseComb=true,bool bCalcuCombQuotOnLegQuot=true,
        const string& Ver_String=string(""),
		bool bAutoReqSettlementInfoConfirmed=true,
        bool bAutoConfirmSettlement=true,
		bool bAutoQryInstrumentInfo=true);

    virtual ~CPlatformCTPTrade();

    //******************************************************************************************
    //  pSpi:                   spi�ص���������ָ�롣һ��ΪNULL��ʹ�ñ��������Ĭ�ϴ����ɣ�
    //                          ���ʵ��������CPlatformCTPTrade�����࣬pMdSpiӦ�õ�����������ָ�룬
    //                          ʹ���������Խ��лص�����
    //******************************************************************************************
    virtual bool Login(const CThostFtdcTraderSpi* pSpi=NULL);
    virtual void Logout(void);



///****************************************************************************
/// ������ctpָ��ִ�к��������������ʾ�ɹ�����������ֵ��ʾʧ�ܡ�
///****************************************************************************

public:
	///�û������������
	int ReqUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate);
	///�ʽ��˻������������
	int ReqTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate);

	///����¼������. ����ʱpInputOrder�е�RequestID��BrokerID��InvestorID��UserID��OrderRef��ֵ���޸�Ϊ�ײ�ȷ����ֵ
	int ReqOrderInsert(CThostFtdcInputOrderField *pInputOrder);
	///������������
	int ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction);

	///Ͷ���߽�����ȷ��
	int ReqSettlementInfoConfirm() ;



	///�����ѯͶ���߽�����
	int ReqQrySettlementInfo(CThostFtdcQrySettlementInfoField *pQrySettlementInfo) ;
	///�����ѯ������Ϣȷ��
	int ReqQrySettlementInfoConfirm(CThostFtdcQrySettlementInfoConfirmField *pQrySettlementInfoConfirm) ;
	///�����ѯ�ͻ�֪ͨ
	int ReqQryNotice(CThostFtdcQryNoticeField *pQryNotice) ;

	///�����ѯ��Լ
	int ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument) ;
	///�����ѯ����
	int ReqQryOrder(CThostFtdcQryOrderField *pQryOrder) ;
	///�����ѯ�ɽ�
	int ReqQryTrade(CThostFtdcQryTradeField *pQryTrade) ;
	///�����ѯͶ���ֲ߳�
	int ReqQryInvestorPosition(CThostFtdcQryInvestorPositionField *pQryInvestorPosition) ;
	///�����ѯͶ���ֲ߳���ϸ
	int ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail) ;
	///�����ѯͶ������ϳֲ���ϸ
	int ReqQryInvestorPositionCombineDetail(CThostFtdcQryInvestorPositionCombineDetailField *pQryInvestorPositionCombineDetail) ;
	///�����ѯ�ʽ��˻�
	int ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount) ;

	///�����ѯ��Լ��֤����
	int ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate) ;
	///�����ѯ��Լ��������
	int ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate) ;





///****************************************************************************
/// ������ctp�ص�����
///****************************************************************************

protected:
    ///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();
	
	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);
		
	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);
#ifdef _NEED_AUTHENTICATE
	///�ͻ�����֤��Ӧ
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
#endif


	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    ///�����ѯ������Ϣȷ����Ӧ
	virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
	///Ͷ���߽�����ȷ����Ӧ
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�û��������������Ӧ
	virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�ʽ��˻��������������Ӧ
	virtual void OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///����¼��������Ӧ
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///��������������Ӧ
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;





	///�����ѯ������Ӧ
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�����ѯ�ɽ���Ӧ
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�����ѯͶ���ֲ߳���Ӧ
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�����ѯ�ʽ��˻���Ӧ
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�����ѯͶ������Ӧ
	virtual void OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�����ѯ��Լ��֤������Ӧ
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�����ѯ��Լ����������Ӧ
	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�����ѯ��Լ��Ӧ
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;


	///�����ѯͶ���߽�������Ӧ
	virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�����ѯͶ���ֲ߳���ϸ��Ӧ
	virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///�����ѯ�ͻ�֪ͨ��Ӧ
	virtual void OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;


	///�����ѯͶ���ֲ߳���ϸ��Ӧ
	virtual void OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;


	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///����֪ͨ
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder) ;

	///�ɽ�֪ͨ
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade) ;

	///����¼�����ر�
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) ;

	///������������ر�
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) ;

	///��Լ����״̬֪ͨ
	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) ;

	///����֪ͨ
	virtual void OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) ;




///****************************************************************************
/// �����ǳ�Ա����
///****************************************************************************

private:
	string					m_Ver_String;						//�汾�ַ�����������"1,6,0,13"
	bool					m_bAutoReqSettlementInfoConfirmed;	//�Ƿ��Զ���ѯ����ȷ����Ϣ
    bool                    m_bAutoConfirmSettlement;			//�Ƿ��Զ�ȷ�Ͻ�����
	bool					m_bAutoQryInstrumentInfo;			//��¼���Զ���ѯ��Լ��Ϣ

	int				        m_CurOrderRef;						//��ǰ�������ñ��

    CThostFtdcTraderApi*    m_pTradeApi;						//���׽ӿڶ������ۺϽ���ƽ̨�Ľӿ��ṩ
	CQryQueue*				m_pQryQueue;						//��ѯ���д������

	bool				    m_bSettlementInfoConfirmed;			//�Ƿ��Ѿ�����ȷ��

    CThostFtdcRspUserLoginField m_LoginInfo;					//��¼��Ϣ����¼��Ӧʱ�õ�



private:
    CPlatformCTPTrade(const CPlatformCTPTrade&);
	CPlatformCTPTrade& operator= (const CPlatformCTPTrade&);
};


#pragma managed(pop)


#endif

