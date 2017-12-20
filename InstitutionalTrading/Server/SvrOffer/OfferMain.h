#pragma once
#include "OfferMainInterface.h"
class CCTPAccount;
class CToolThread;

class COfferMain : public COfferMainInterface
{
public:
	COfferMain();
	virtual ~COfferMain(void);
	//��ʼ��
	virtual  void Init(bool bSettlement, int nMorning);//0 Ϊ���ף�1Ϊ����

	//�ͷ��ڴ�
	void Release();
	//�ж������˻��ǲ�����ɵ�¼�����εǲ���Ҳ����ɵ�¼
	bool IsAllAcountFinishLoginIn();

    //��ѯ���к�Լ
	void QueryInstruments();	
	void QueryCommission(BrokerAccountKey& BAKey);
	void QueryMargin(BrokerAccountKey& BAKey);
	void QueryOrder(BrokerAccountKey& BAKey);
	void QueryTrade(BrokerAccountKey& BAKey);
	void QueryFund(BrokerAccountKey& BAKey);
	void QueryPosition(BrokerAccountKey& BAKey);
	void QueryPositionDetail(BrokerAccountKey& BAKey);

	bool NeedQueryInstruments();
	void SetNeedQueryInstruments(bool bNeed);
	void StartQueryMargin(BrokerAccountKey& BAKey);
	void StartQueryCommission(BrokerAccountKey& BAKey);

	void RestartAccount(int nTradeAccount);

	//������������
	void QueryQuot(std::map<std::string, int>& mapInstrumentID);
	void QueryQuot();//���������
	//��ѯ�����ĺ�Լ����
	void AddInstrument(CThostFtdcInstrumentField *pInstrument, bool bIsLast);

	//�ӽ������ݹ��������ȡ����
	void AddInstrument();
	//�õ���һ��Ҫ��ѯ��Լ��֤���ʵĺ�Լ����
	bool GetNextQueryInstrumentOfMargin(std::string strBrokerID, std::string strUser, std::string& strInstrument, std::string& strHedgeFlag);	

	//�õ���һ��Ҫ��ѯ�������ʵĺ�Լ����	
	bool GetNextQueryInstrumentOfCommission(std::string strBrokerID, std::string strUser, std::string& strInstrument);

	//�õ���Ʒ�������еĺ�Լ
	void GetInstrumentsByProductID(std::string strProduct, std::vector<std::string>& vecInstruments);

	//�õ��ú�Լ��Ʒ��
	bool GetProductIDByInstruments(std::string strInstrument, std::string& strProduct);
	
	//�µ�
	int ReqOrderInsert_Main(PlatformStru_InputOrder *pInputOrder, int nTraderID, InputOrderKey key, int nRequestID);

	//����
	int ReqOrderAction_Main(CThostFtdcInputOrderActionField *pInputOrderAction, int nTraderID, InputOrderKey key, int nRequestID);	

	//�õ������˻�ָ��
	bool GetCTPAccount(BrokerAccountKey BAKey, CCTPAccount*& pCtpAccount);

	//�ж��ǲ��Ǻ�Լ�Ѿ�����
	bool GetFinishedQueryInstrument();



	//�Ͽ�ctp����
	void LogoffAccount(BrokerAccountKey BAKey);

	//��ʱ����˺�ctpǰ���Ƿ�����ʧ��
	void CheckAccountsConnect();

	//��������ϵĶϿ�
	bool IsCtpAccountTimerExit(CCTPAccount* pCtpAccount);
	void SetTimer2CTPAccount(UINT_PTR uPtr, CCTPAccount* pCtpAccount);
	bool GetCtpAccountByTime(UINT_PTR uPtr, CCTPAccount*& pCtpAccount);
	void RemoveCtpAccountByTime(UINT_PTR uPtr);
private:
	

	//��ѯ���ݿ�
	
	//void InsertOrderRef(BrokerAccountKey& BAKey, CCTPAccount*& pCtpAccount);

private:
//�����˺Ÿ���״̬��Ϣ
	std::map<BrokerAccountKey, CCTPAccount*>		m_mapUserID2CTPAccount;
	CReadWriteLock							m_mutex_mapUserID2CTPAccount;

	std::multimap<BrokerAccountKey, CCTPAccount*>		m_mapUserID2CTPAccount_Ratio;
	CReadWriteLock									m_mutex_mapUserID2CTPAccount_Ratio;

	//���к�Լ������
	std::map<std::string, int>				m_mapInstrumentID;
	bool									m_bFinishedQueryInstrument;
	CReadWriteLock							m_mutex_mapInstrument;

	//��Ʒ����Լ��ӳ�䣬��Ҫ���ͬƷ�ַ��ʲ��ظ���ѯ������
	std::multimap<std::string, std::string> m_mulmapProductID2InstrumentID;
	CReadWriteLock							m_mutex_mulmapProductID2InstrumentID;

	//��Լ����Ʒ��ӳ��
	std::map<std::string, std::string>		m_mapInstrumentID2ProductID;
	CReadWriteLock							m_mutex_mapInstrumentID2ProductID;

	//�������ݿ��������н����˻���ID��ת����ϵ
//	std::multimap<BrokerAccountKey, SOrderTransfer>	m_mapAccountID2OrderTransfer;
//	CReadWriteLock								m_mutex_mapAccountID2OrderTransfer;

	
	bool										m_bNeedQueryInstrument;
	CReadWriteLock								m_mutex_NeedQueryInstrument;

	

	std::map<UINT_PTR, CCTPAccount*>		   m_mapTimer2CTPAccount;
	CReadWriteLock							   m_mutex_mapTimer2CTPAccount; 



private:


};
