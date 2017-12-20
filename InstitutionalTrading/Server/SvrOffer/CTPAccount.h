#pragma once
#include "MsgQueue.h"
class CToolThread;
class CTradeDataMsgQueue;

struct SMarginKey
{
	std::string strInstrument;	//��Լ����
	std::string strHedgeFlag;	//Ͷ���ױ���־
	
	bool operator< (const SMarginKey& r) const
	{
		int irlt;
		irlt=strncmp(strInstrument.c_str(),r.strInstrument.c_str(),sizeof(strInstrument));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(strHedgeFlag.c_str(),r.strHedgeFlag.c_str(),sizeof(strHedgeFlag));
		if(irlt<0) return true;
		if(irlt>0) return false;

		return false;
	}
};
struct STransfer//�µ���ʱ���õ�
{
	std::string strBrokerID;//���͹�˾��ֻ���˻���Ч���Խ���Ա��Ч
	std::string strID;//����ԱID�����û�ID
	InputOrderKey key;
	int						UserForceClose;	//�û�ǿƽ��־ ��ؿͻ���ǿƽʱ��Ϊ1����ȻΪ0
	char			        ForceCloseReason; //��������   ��ؿͻ���ǿƽʱ��ΪTHOST_FTDC_FCC_ForceClose,��ȻΪ��0��
	STransfer()
	{
		
		strBrokerID = "";
		strID = "";
		key.nFrontID = 0;
		key.nSessionID = 0;
		memset(key.szOrderRef, 0, sizeof(key.szOrderRef));
		ForceCloseReason = '0';
		UserForceClose   = 0;
	}
	bool operator< (const STransfer& r) const
	{
		int irlt;

		irlt=strncmp(strBrokerID.c_str(),r.strBrokerID.c_str(),sizeof(strBrokerID));
		if(irlt<0) return true;
		if(irlt>0) return false;


		irlt=strncmp(strID.c_str(),r.strID.c_str(),sizeof(strID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		if(UserForceClose < r.UserForceClose)
			return true;
		else if(UserForceClose > r.UserForceClose)
			return false;

		if(ForceCloseReason < r.ForceCloseReason)
			return true;
		else if(ForceCloseReason > r.ForceCloseReason)
			return false;

		if(key < r.key)
			return true;
		else return false;



		return false;
	}
};
struct SOrderRef//������ʱ���õ�
{
	std::string strBrokerID;//���͹�˾��ֻ���˻���Ч���Խ���Ա��Ч
	std::string strID;//����ԱID�����û�ID
	int  nOrderRef;	
	SOrderRef()
	{
		memset(this, 0, sizeof(SOrderRef));
	}
	bool operator< (const SOrderRef& r) const
	{
		int irlt;
		irlt=strncmp(strBrokerID.c_str(),r.strBrokerID.c_str(),sizeof(strBrokerID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(strID.c_str(),r.strID.c_str(),sizeof(strID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		if(nOrderRef < r.nOrderRef)
			return true;
		else if(nOrderRef > r.nOrderRef)
			return false;

	
		return false;
	}
};
struct SOrderSysIDRef//������ʱ���õ�
{
	std::string strExchanggeID;//������ID
	std::string strOrderSysID;//�������	
	SOrderSysIDRef()
	{
		memset(this, 0, sizeof(SOrderSysIDRef));
	}
	bool operator< (const SOrderSysIDRef& r) const
	{
		int irlt;
		irlt=strncmp(strExchanggeID.c_str(),r.strExchanggeID.c_str(),sizeof(strExchanggeID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(strOrderSysID.c_str(),r.strOrderSysID.c_str(),sizeof(strOrderSysID));
		if(irlt<0) return true;
		if(irlt>0) return false;

		return false;
	}
};
class CCTPAccount
{
public:
	CCTPAccount(void);
	~CCTPAccount(void);
	void Init(SLogin& login, int nAddrType, THOST_TE_RESUME_TYPE restartType);//nAddrType: 0 ��ͨ��ַ1 �����ַ
	void InitQueryThread();
	void QueryInstruments();
	void QueryCommission(std::string strInstrument);
	void QueryMargin(std::string strInstrument);
	void QueryOrder();
	void QueryTrade();
	void QueryFund();
	void QueryPosition();
	void QueryPositionDetail();

	bool ProcessResultFail(STradeQueryData& data, std::string strBrokerID, std::string strAccountID);
	bool ProcessResult(STradeQueryData& data, std::string strBrokerID, std::string strAccountID,bool bQueryCommission, bool bQueryMargin);
	
	void GetLogin(SLogin& login);
	void SetAccountBaseInfo(std::string	strBrokerID, std::string strUserID, std::string	 strPassword);
	void SetTrader(CThostFtdcTraderApi* pTraderApi, CUserTraderSpi* pTraderSpi);
	void SetCurrentOrderref(int nOrderRef);
	void SetFrontID(int nFrontID);
	void SetSessionID(int nSessionID);

	int GetCurrentOrderref();
	int GetFrontID();
	int GetSessionID();

	void GetAccountBaseInfo(std::string&	strBrokerID, std::string& strUserID, std::string& strPassword);
	CThostFtdcTraderApi* GetTraderApi();
	CUserTraderSpi*		 GetTraderSpi();

	void SetAccountStatus(EnumAccountStatus	enumAccountStatus);
	EnumAccountStatus GetAccountStatus();

	void Release();

	//���õ�¼����
	void SetLoginCount(int nLoginCount);
	//�õ���¼����
	int  GetLoginCount();

	//��֤���ʵĲ�ѯ
	void SetMarginQuery(std::map<std::string, int>& mapInstrument);
	//�õ���һ��Ҫ��ѯ��Լ��֤���ʵĺ�Լ����
	bool GetNextInstrumentOfMargin(std::string& strInstrument, std::string& strHedgeFlag);
	//��Լ�鵽
	bool FinishedQueryInstrumentMargin(std::string strInstrument, std::string strHedgeFlag);

	//�������ʵĲ�ѯ���к�Լmap
	void SetCommissionQuery(std::map<std::string, int>& mapInstrument);
	
	//�õ���һ��Ҫ��ѯ�������ʵĺ�Լ����	
	bool GetNextInstrumentOfCommission(std::string& strInstrument);

	//��Լ�鵽
	bool FinishedQueryInstrumentCommission(std::string strInstrument);

	//��Ʒ�ַ����������ʣ���һ��Ʒ�ֲ�ѯһ�ξ͹��ˣ�����ĺ�Լɾ��
	void DeleteInstrumrntOfComminssion(std::string& strInstrument);

	//����һ���ͻ��ˣ�FrontID,SessionID,OrderRef����ctp��FrontID,SessionID,OrderRef����ת����ϵ
	void InsertCleint2CTPKeyTransfer(STransfer keyClient, STransfer keyCTP);
    //���ݿͻ��˵�ID��FrontID,SessionID,OrderRef���ҵ���Ӧ�� CTP��صģ�FrontID,SessionID,OrderRef��
	bool GetCleint2CTPKeyTransfer(STransfer keyClient, STransfer& keyCTP);

	//����һ��ctp��FrontID,SessionID,OrderRef�����ͻ��ˣ�FrontID,SessionID,OrderRef����ת����ϵ
	void InsertCTP2CleintKeyTransfer(STransfer keyCTP, STransfer keyClient);
	//���� CTP��FrontID,SessionID,OrderRef���ҵ���Ӧ�� CTP�ͻ��˵�ID��صģ�FrontID,SessionID,OrderRef��
	bool GetCTP2CleintKeyTransfer(STransfer keyCTP, STransfer& keyClient);

	//����һ���ͻ���orderref��ctporderref֮��Ĺ�ϵ
	void InsertCtp2ClientOrderRef(SOrderRef CtpOrderRef, STransfer ClientOrderRef);

	bool GetClientOrderRef(SOrderRef CtpOrderRef, STransfer& ClientOrderRef);

	//����һ��ordersysid
	void Insertordersysid2ClientOrderRef(SOrderSysIDRef orderSysIDRef, STransfer ClientOrderRef);

	bool GetRefByOrderSysID(SOrderSysIDRef orderSysIDRef, STransfer& ClientOrderRef);

	//�Ƿ�ֻ�����ѯ����
	void SetIsQueryRatio(bool bOnlyQueryRatio);

	//�Ƿ�ֻ�����ѯ����
	bool GetIsQueryRatio();

	//�µ�
	int ReqOrderInsert_Account(PlatformStru_InputOrder *pInputOrder, int nTraderID, InputOrderKey key, int nRequestID);
	//����
	int ReqOrderAction_Account(CThostFtdcInputOrderActionField *pInputOrderAction, int nTraderID, InputOrderKey key, int nRequestID);

	//���ݽ���Ա�˺ŵõ�ί�н����˺�
	bool GetTradeAccountByUserID(int nTraderID, std::string& strBrokerID, std::string& strInvestID);



	//�Ѿ��鵽���������Ʒ�ַ��صĲ���ڶ���
	void InsertCommissionProductID(std::string strProductID);
	bool IsCommissionProductIDQuery(std::string strProductID);

	void InsertCommissionProductID_Main(BrokerAccountKey& BAKey, std::string strProductID);
	bool IsCommissionProductIDQuery_Main(BrokerAccountKey& BAKey, std::string strProductID);

public:
	#ifdef  SIMU//ģ�⽻����صĶ������������
		void Init_simu(SLogin& login);
		//ģ���µ�
		int ReqOrderInsert_SimuAccount(PlatformStru_InputOrder *pInputOrder, int nTraderID, InputOrderKey key, int nRequestID);
		//ģ�⳷��
		int ReqOrderAction_SimuAccount(CThostFtdcInputOrderActionField *pInputOrderAction, int nTraderID, InputOrderKey key, int nRequestID);

		void ProcessOrder(CThostFtdcOrderField& order);
		void ProcessTrade(CThostFtdcTradeField& trade);
		void OnRspQryOrder(CThostFtdcOrderField *pOrder);
		void OnRspQryTrade(CThostFtdcTradeField *pTrade);
	#endif

private:
	SLogin									m_login;
	// ��Ա����
	std::string								m_strBrokerID;
	// �����û�����		
	std::string								m_strUserID;
	// �����û�����
	std::string								m_strPassword;	

	CReadWriteLock							m_Mutex_BrokerID;

	//����API
	CThostFtdcTraderApi *					m_pTraderApi;   
	CReadWriteLock							m_Mutex_TraderApi;

	//����SPI
	CUserTraderSpi *						m_pTraderSpi;  
	CReadWriteLock							m_Mutex_TraderSpi;

	//��¼��״̬
	EnumAccountStatus						m_enumAccountStatus;
	CReadWriteLock							m_Mutex_AccountStatus;

	//��¼����
	int										m_nLoginInCount;
	CReadWriteLock							m_Mutex_loginInCount;

	//��ǰ���õ�OrderRef
	int										m_nCurrentOrderref;
	CReadWriteLock							m_Mutex_nCurrentOrderref;

	//CTP���ص�FrontID
	int										m_nCTPFrontID;           
	CReadWriteLock							m_Mutex_nCTPFrontID;

	//CTP���ص�SessionID
	int										m_nCTPSessionID;
	CReadWriteLock							m_Mutex_nCTPSessionID;
	
	//client �� ctp Keytransfer
	std::map<STransfer, STransfer>			m_Cleint2CTPKeyTransfer;	
	CReadWriteLock							m_Mutex_Cleint2CTPKeyTransfer;

	//ctp �� client  Keytransfer
	std::map<STransfer, STransfer>			m_CTP2ClientKeyTransfer;	
	CReadWriteLock							m_Mutex_CTP2ClientKeyTransfer;

	
	//ctp orderref to client orderref
	std::map<SOrderRef, STransfer>			m_CtpOrderref2Client;
	CReadWriteLock							m_Mutex_CtpOrderref2Client;

	//ctp ordersysid to client orderref
	std::map<SOrderSysIDRef, STransfer>			m_CtpOrderSysID2Client;
	CReadWriteLock							   m_Mutex_CtpOrderSysID2Client;


	//��Լ��֤���ʵĲ�ѯ�Ƿְ�
	std::map<SMarginKey, int>				m_mapInstrumentMargin;
	CReadWriteLock							m_Mutex_mapInstrumentMargin;

	//�������ʵĲ�ѯ�Ƿְ�
	std::map<std::string, int>				m_mapInstrumentCommission;
	CReadWriteLock							m_Mutex_mapInstrumentCommission;

	//��Լ��Ʒ�ַ��صģ�ֻ��һ�η���
	std::set<std::string>				    m_setCommissionProductID;
	CReadWriteLock							m_Mutex_setCommissionProductID;

	//
	CToolThread*		m_QueryMgrThread;
	static DWORD WINAPI QueryThread(PVOID pParam);
	

	CToolThread*		m_QueryRatioMgrThread;
	static DWORD WINAPI QueryRatioThread(PVOID pParam);

public:
	STradeQueryData		m_dataCurrent;     //���һ����ѯ������
	bool				m_bQueryCommission;//�в�ѯ����
	bool				m_bQueryMargin;    //�в�ѯ��֤��
	CCTPAccount*		m_LastAccount;     //���һ����ѯ���˻�
	CReadWriteLock		m_Mutex_dataCurrent;
	void SetCurrentQueryData(STradeQueryData	dataCurrent, CCTPAccount* pCtpAccount);
	void GetCurrentQueryData(STradeQueryData&	dataCurrent, CCTPAccount*& pCtpAccount, bool&	bQueryCommission, bool& bQueryMargin);

	void SetQueryConnectFail();//�Ͽ�����Ҫ��������ʧ��
public:		
	CTradeDataMsgQueue* m_pTradeDataMsgQueue;
	void				InitQueryRatioThread();
	DWORD				ProcessQueryCommission();
	DWORD				ProcessQueryMargin();
	bool				m_bOnlyQueryRatio;//���˻����ֻ�ǲ�����õģ��������˳�ctp����
	int					m_nQueryMargin;   // ��ѯ��֤���� 0�� ��ʼ����1,��ʼ��ѯ��2������
	CReadWriteLock		m_Mutex_QueryMargin;
	int					m_nQueryCommission;// ��ѯ��Լ�������� 0�� ��ʼ����1,��ʼ��ѯ��2������
	CReadWriteLock		m_Mutex_QueryCommission;
	int					GetQueryMargin();
	void				SetQueryMargin();
	int					GetQueryCommission();	
	void				SetQueryCommission();


	DWORD				m_LastQryTime;			//����ѯʱ��
	bool				m_bInQry;				//��û����;��ѯ
	bool				IsQryTime(void);		
	void				UpdateQryTime(void);
	void				ClearbInQry() ;
	bool				SendQryCmd(STradeQueryData& data, int& ReqRlt, CCTPAccount* pCtpMainAccount);


	
};