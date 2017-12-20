#pragma once
#include "../MAServer/include/MAAccount/MultiAccountCmd.h"
#include "../MAServer/include/MAAccount/MultiAccountStruct.h"
#define MA_CORE()  (CMultiAccountCore::GetInstance())
typedef struct tagOrderInsertUIItem
{
	int						ID;
	CommonAccountType		Account;			//�ʺ�
	CommonInstrumentIDType  InstrumentID;		//��Լ
	bool                    IsBuy;				//����
	bool                    IsHedge;			//Ͷ��
	bool					IsMarket;			//�Ƿ�Ϊ�м�
	bool					IsMarket_Trigger;	//�������Ƿ�Ϊ�м�
	double					Price;				//�۸�
	double					TriggerPrice;		//�����۸�
	double					VolumeFlag;			//����flag
	int						Volume;				//����
	int						OCMode;				//��ƽ, 0 ����, 1 ƽ��, 2 ƽ��
}OrderInsertUIItem;
typedef enum tagStrategyItem
{
	Strategy_None=-1,
	Strategy_Assign,//�µ���ָ������
	Strategy_Base,//����������ʽ
	Strategy_Total,//�������䷽ʽ
	Strategy_FundDeviation,//���ʽ������ʻ�����
	Strategy_AccountSize,//���ʻ���ģ��������
	//Strategy_AccountAuto,//���ʻ��Զ���ƽ
	Strategy_UserDefine,//���԰��Զ���������ʽ
}StrategyItem;
typedef void (*SETUICALLBACK)(bool bAcc);
class CMultiAccountCore
{
public:
	static CMultiAccountCore* GetInstance();
	static void DestroyInstance();	
	static CMultiAccountCore* m_pInstance;
public:
	//�µ����Ի�ȡ
	StrategyItem GetStrategyNum();
	void SetStrategyNum(StrategyItem it);
	bool GetOrderInsertStrategy(vector<OrderInsertUIItem>& dest,
								const OrderInsertUIItem& src,
								StrategyItem stra=Strategy_None,
								map<string,bool>* accstatus=NULL);
	int	GetAccountStatus(map<string,bool>* accstatus);
	int	SetAccountStatus(map<string,bool>* accstatus);
	wxArrayString GetAccountArray();
public:
	int GetOrderStrategyData(OrderStrategy eStrategy, const std::string strAccount,
		std::string strInstrument, std::string strProduct);
public:

	void AddTradeAccount(CTPTradeAccount& trader);
	void EraseTradeAccount(wxString acc,bool bOnlyContent);
	void ClearAllTradeAccount();
	void SetTradeAccount(vector<CTPTradeAccount>& vec);
	void SetTradeAccount(int index,const CTPTradeAccount& pAcc);
	void SetTradeAccountUseQuot(int index,bool UseQuot);
	void SetTradeAccountStrategyValid(int index,bool b);
	void SetTradeAccountStatusText(int index,wxString text);
	int FindTradeAccount(wxString acc) const;
	int GetTradeAccountCount() const;
	const CTPTradeAccount* GetTradeAccount(int index) const;
	string GetTradeAccountName(int index) const;
	int GetNextTradeAccountId() const;
	void SetLoadTypeAuto(int b);
	void SetSaveTypeAuto(int b);
	void SetLoadTypeManual(int b);
	void SetSaveTypeManual(int b);
	int GetLoadTypeAuto();
	int GetSaveTypeAuto();
	int GetLoadTypeManual();
	int GetSaveTypeManual();
public:
	void SaveDataToSvr(bool HasPwd,wxWindow* win);
	void SaveDataToLocal(bool HasPwd,map<int,vector<TradeAccount>>* const pContent=NULL);
	void LoadFromSvr(wxWindow* win);
	void LoadFromLocal(map<int,vector<TradeAccount>>* pRtn=NULL);
	void DelLocal();
	void DelServer();
public:
    void SetUICallback(SETUICALLBACK func){m_uicallback=func;}
	set<int>			m_vecReqId;
	LoginRsp  m_pLoginRsp;

	vector<BrokerInfo>   m_vecBrokerInfo;
	multimap<int,ServerGroup> m_mapServerGroup;
	multimap<int,ServerAddr>	m_mapServerAddr;
protected:
	MULTIACCOUNT*  m_pMACfg;
	SETUICALLBACK m_uicallback;	
	vector<CTPTradeAccount> m_vecTradeAccount;
private:
	CMultiAccountCore(void);
	virtual ~CMultiAccountCore(void);
	double GetAccountAlignOrAccountSizeData( IPlatformSingleSvr* pSvr, int nFirstAccountAlign );
};
