#pragma once

#include <map>
#include <vector>
#include <set>
#include <string>
#include "CommonStruct.h"
#include "Interface_SvrTradeExcute.h"
#include "KernelStruct.h"
#include "easymutex.h"




class CSvrTradeExcuteImpl : public CInterface_SvrTradeExcute
{
public:
	static CSvrTradeExcuteImpl& getObj(void);


	//�����ǿƽ��
	virtual CF_ERROR RiskForceCloseOrder(PlatformStru_InputOrder & nInputOrder,
		const InputOrderKey& lKey,
		int nUserID,
		const std::string & nsForceReason) ;

	//��س��ҵ�
	virtual CF_ERROR RiskForceActionOrder( PlatformStru_InputOrderAction & nInputOrderAction,
		int nUserID,
		int nRequestID,
		const std::string & nsActionReson) ;

	//���ģ����ã�д������µ�����
	//��ص���
	virtual bool RiskControl(const std::string &nUserName, 
		std::multimap<RiskWarningType, SRiskControl>& vecRiskControl);


	//��˱���
	virtual  CF_ERROR SetVerifyOrder(OrderKey& orderKey, bool bPassVerify) ;
	////���ú�Լ����
	//void SetRiskInstrumentLimit(const std::map<std::string, std::set<std::string>>& mapRiskInstrumentLimit);

	////���÷���ָ����ʱ���ɵ��µ�����
	//void AddRiskIndOrderLimit(const std::string &nUserName,
	//	RiskLimit limit, 
	//	std::set<std::string> setLimitInstruments);

	//void DelRiskIndOrderLimit(const std::string &nUserName);

	//���ײ���ģ����ã�д������µ�����
	//void SetStrategyOrderLimit(const std::map<std::string, std::map<std::string, StrategyOrderLimit>>& mapStrategyOrderLimit);

	void SetOneStrategyOrderLimit(const std::string &nUserName, const std::string & nInstrumentID,StrategyOrderLimit nCtrl);	


    //Ԥ�����µ����ж��û����ã��û���Լ���ƣ��û��Ƿ���Ҫ�ֹ���� 
	CF_ERROR PreFilterOrder(int nUserId,
		const PlatformStru_OrderInfo& order, 
		bool& bNeedVerify);

	//�����ֿأ�
	CF_ERROR BaseFilterOrder(const std::string &nUserName,
		const PlatformStru_OrderInfo& order, 
		const bool nbNeedPushOrderInfoToTraderData);


	//�ֿغͽ��ײ���
	CF_ERROR AfterFilterOrder(const std::string &nUserName,
		const PlatformStru_OrderInfo& order, 
		const bool nbNeedPushOrderInfoToTraderData);



	//������ش���
	int CancelOrder(InputOrderKey key);
	//������Ӧ
	int CancelOrderReturn(InputOrderKey key);

	void AddVerifyingOrder( InputOrderKey key, PlatformStru_InputOrder order);
	bool GetInputOrder(InputOrderKey key, PlatformStru_InputOrder& order);
	void CancelOrderDeal(PlatformStru_OrderInfo & orderInfo,CF_ERROR nErrorCode,bool nbRiskEvent);
	void UpdateOrderDeal(PlatformStru_OrderInfo & orderInfo);
private:
	bool LoadDB();
	//�жϽ���Ա�Ƿ������µ�
	bool EnableOrder(int nTraderID, int nTradeAccountID);

	//�ж��Ƿ���Ҫ�ֹ����
	bool NeedVerify(int nTraderID, int nTradeAccountID);
	//�жϷ���ָ����ʱ���ɵ��µ�����
	bool RiskIndLimited(const std::string &nUserName,const PlatformStru_OrderInfo& order,CF_ERROR & nErrorCode);
	//�ж��µ���������
	bool StrategyLimited(const std::string &nUserName,std::string strInstrumentID, char Direction);

private:
	CSvrTradeExcuteImpl();
	virtual ~CSvrTradeExcuteImpl();
	static CSvrTradeExcuteImpl* m_pObj;

private:
	//�µ��ṹ
	
	//���ģ�����ݷ���ָ����ʱ���ɵ��µ�����	
	std::map<std::string, std::multimap<RiskWarningType, SRiskControl>> m_mapRiskLimited;
	Ceasymutex mMutexRisk;

	//���ײ����µ�����
	std::map<std::string, std::map<std::string, StrategyOrderLimit>> m_mapStrategyOrderLimit;
	Ceasymutex mMutexStategy;
};
