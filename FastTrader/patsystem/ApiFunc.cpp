#include "StdAfx.h"
#include "ApiFunc.h"

#define LOADFUNCTION(funcname)\
	if(NULL==(funcname=(lp_##funcname)::GetProcAddress(m_hInst_PATSAPIDLL,#funcname))) bRet=false
HINSTANCE ApiFunc::m_hInst_PATSAPIDLL=NULL;
lp_ptRegisterCallback                                   ApiFunc::ptRegisterCallback=NULL;
lp_ptRegisterLinkStateCallback                          ApiFunc::ptRegisterLinkStateCallback=NULL;
lp_ptRegisterMsgCallback                                ApiFunc::ptRegisterMsgCallback=NULL;
lp_ptRegisterOrderCallback                              ApiFunc::ptRegisterOrderCallback=NULL;
lp_ptRegisterOrderTypeUpdateCallback                    ApiFunc::ptRegisterOrderTypeUpdateCallback=NULL;
lp_ptRegisterFillCallback                               ApiFunc::ptRegisterFillCallback=NULL;
lp_ptRegisterPriceCallback                              ApiFunc::ptRegisterPriceCallback=NULL;
lp_ptRegisterBlankPriceCallback                         ApiFunc::ptRegisterBlankPriceCallback=NULL;
lp_ptRegisterGenericPriceCallback                       ApiFunc::ptRegisterGenericPriceCallback=NULL;
lp_ptRegisterDOMCallback                                ApiFunc::ptRegisterDOMCallback=NULL;
lp_ptRegisterContractCallback                           ApiFunc::ptRegisterContractCallback=NULL;
lp_ptRegisterCommodityCallback                          ApiFunc::ptRegisterCommodityCallback=NULL;
lp_ptRegisterExchangeCallback                           ApiFunc::ptRegisterExchangeCallback=NULL;
lp_ptRegisterStatusCallback                             ApiFunc::ptRegisterStatusCallback=NULL;
lp_ptRegisterExchangeRateCallback                       ApiFunc::ptRegisterExchangeRateCallback=NULL;
lp_ptRegisterConStatusCallback                          ApiFunc::ptRegisterConStatusCallback=NULL;
lp_ptRegisterOrderCancelFailureCallback                 ApiFunc::ptRegisterOrderCancelFailureCallback=NULL;
lp_ptRegisterOrderSentFailureCallback                   ApiFunc::ptRegisterOrderSentFailureCallback=NULL;
lp_ptRegisterOrderQueuedFailureCallback                 ApiFunc::ptRegisterOrderQueuedFailureCallback=NULL;
lp_ptRegisterAtBestCallback                             ApiFunc::ptRegisterAtBestCallback=NULL;
lp_ptRegisterTickerCallback                             ApiFunc::ptRegisterTickerCallback=NULL;
lp_ptRegisterAmendFailureCallback                       ApiFunc::ptRegisterAmendFailureCallback=NULL;
lp_ptRegisterSubscriberDepthCallback                    ApiFunc::ptRegisterSubscriberDepthCallback=NULL;
lp_ptRegisterSettlementCallback                         ApiFunc::ptRegisterSettlementCallback=NULL;
lp_ptRegisterStrategyCreateSuccess                      ApiFunc::ptRegisterStrategyCreateSuccess=NULL;
lp_ptRegisterStrategyCreateFailure                      ApiFunc::ptRegisterStrategyCreateFailure=NULL;
lp_ptRegisterOrderBookReset                             ApiFunc::ptRegisterOrderBookReset=NULL;
lp_ptRegisterTraderAdded                                ApiFunc::ptRegisterTraderAdded=NULL;
lp_ptCountExchanges                                     ApiFunc::ptCountExchanges=NULL;
lp_ptGetExchange                                        ApiFunc::ptGetExchange=NULL;
lp_ptGetExchangeByName                                  ApiFunc::ptGetExchangeByName=NULL;
lp_ptExchangeExists                                     ApiFunc::ptExchangeExists=NULL;
lp_ptCountTraders                                       ApiFunc::ptCountTraders=NULL;
lp_ptGetTrader                                          ApiFunc::ptGetTrader=NULL;
lp_ptGetTraderByName                                    ApiFunc::ptGetTraderByName=NULL;
lp_ptTraderExists                                       ApiFunc::ptTraderExists=NULL;
lp_ptCountOrderTypes                                    ApiFunc::ptCountOrderTypes=NULL;
lp_ptGetOrderType                                       ApiFunc::ptGetOrderType=NULL;
lp_ptGetExchangeRate                                    ApiFunc::ptGetExchangeRate=NULL;
lp_ptCountReportTypes                                   ApiFunc::ptCountReportTypes=NULL;
lp_ptGetReportType                                      ApiFunc::ptGetReportType=NULL;
lp_ptReportTypeExists                                   ApiFunc::ptReportTypeExists=NULL;
lp_ptGetReportSize                                      ApiFunc::ptGetReportSize=NULL;
lp_ptGetReport                                          ApiFunc::ptGetReport=NULL;
lp_ptCountCommodities                                   ApiFunc::ptCountCommodities=NULL;
lp_ptGetCommodity                                       ApiFunc::ptGetCommodity=NULL;
lp_ptCommodityExists                                    ApiFunc::ptCommodityExists=NULL;
lp_ptGetCommodityByName                                 ApiFunc::ptGetCommodityByName=NULL;
lp_ptCountContracts                                     ApiFunc::ptCountContracts=NULL;
lp_ptGetContract                                        ApiFunc::ptGetContract=NULL;
lp_ptGetContractByName                                  ApiFunc::ptGetContractByName=NULL;
lp_ptGetContractByExternalID                            ApiFunc::ptGetContractByExternalID=NULL;
lp_ptGetExtendedContract                                ApiFunc::ptGetExtendedContract=NULL;
lp_ptGetExtendedContractByName                          ApiFunc::ptGetExtendedContractByName=NULL;
lp_ptContractExists                                     ApiFunc::ptContractExists=NULL;
lp_ptInitialise                                         ApiFunc::ptInitialise=NULL;
lp_ptGetAPIBuildVersion                                 ApiFunc::ptGetAPIBuildVersion=NULL;
lp_ptReady                                              ApiFunc::ptReady=NULL;
lp_ptPurge                                              ApiFunc::ptPurge=NULL;
lp_ptDisconnect                                         ApiFunc::ptDisconnect=NULL;
lp_ptSetHostAddress                                     ApiFunc::ptSetHostAddress=NULL;
lp_ptSetPriceAddress                                    ApiFunc::ptSetPriceAddress=NULL;
lp_ptEnable                                             ApiFunc::ptEnable=NULL;
lp_ptDisable                                            ApiFunc::ptDisable=NULL;
lp_ptLogString                                          ApiFunc::ptLogString=NULL;
lp_ptOMIEnabled                                         ApiFunc::ptOMIEnabled=NULL;
lp_ptNotifyAllMessages                                  ApiFunc::ptNotifyAllMessages=NULL;
lp_ptSetHostReconnect                                   ApiFunc::ptSetHostReconnect=NULL;
lp_ptSetPriceReconnect                                  ApiFunc::ptSetPriceReconnect=NULL;
lp_ptSetPriceAgeCounter                                 ApiFunc::ptSetPriceAgeCounter=NULL;
lp_ptSubscribePrice                                     ApiFunc::ptSubscribePrice=NULL;
lp_ptSubscribeToMarket                                  ApiFunc::ptSubscribeToMarket=NULL;
lp_ptUnsubscribePrice                                   ApiFunc::ptUnsubscribePrice=NULL;
lp_ptUnsubscribeToMarket                                ApiFunc::ptUnsubscribeToMarket=NULL;
lp_ptPriceSnapshot                                      ApiFunc::ptPriceSnapshot=NULL;
lp_ptSetEncryptionCode                                  ApiFunc::ptSetEncryptionCode=NULL;
lp_ptSetHandShakePeriod                                 ApiFunc::ptSetHandShakePeriod=NULL;
lp_ptSetHostHandShake                                   ApiFunc::ptSetHostHandShake=NULL;
lp_ptSetPriceHandShake                                  ApiFunc::ptSetPriceHandShake=NULL;
lp_ptSetInternetUser                                    ApiFunc::ptSetInternetUser=NULL;
lp_ptSetClientPath                                      ApiFunc::ptSetClientPath=NULL;
lp_ptGetErrorMessage          ApiFunc::ptGetErrorMessage=NULL;
lp_ptDumpLastError                                      ApiFunc::ptDumpLastError=NULL;
lp_ptSnapdragonEnabled                                  ApiFunc::ptSnapdragonEnabled=NULL;
lp_ptSubscribeBroadcast                                 ApiFunc::ptSubscribeBroadcast=NULL;
lp_ptUnsubscribeBroadcast                               ApiFunc::ptUnsubscribeBroadcast=NULL;
lp_ptCountFills                                         ApiFunc::ptCountFills=NULL;
lp_ptGetFill                                            ApiFunc::ptGetFill=NULL;
lp_ptGetFillByID                                        ApiFunc::ptGetFillByID=NULL;
lp_ptGetContractPosition                                ApiFunc::ptGetContractPosition=NULL;
lp_ptGetOpenPosition                                    ApiFunc::ptGetOpenPosition=NULL;
lp_ptGetTotalPosition                                   ApiFunc::ptGetTotalPosition=NULL;
lp_ptGetAveragePrice                                    ApiFunc::ptGetAveragePrice=NULL;
lp_ptCountOrders                                        ApiFunc::ptCountOrders=NULL;
lp_ptGetOrder                                           ApiFunc::ptGetOrder=NULL;
lp_ptGetOrderByID                                       ApiFunc::ptGetOrderByID=NULL;
lp_ptGetOrderIndex                                      ApiFunc::ptGetOrderIndex=NULL;
lp_ptBlankPrices                                        ApiFunc::ptBlankPrices=NULL;
lp_ptGetPrice                                           ApiFunc::ptGetPrice=NULL;
lp_ptGetPriceForContract                                ApiFunc::ptGetPriceForContract=NULL;
lp_ptGetGenericPrice                                    ApiFunc::ptGetGenericPrice=NULL;
lp_ptAddOrder                                           ApiFunc::ptAddOrder=NULL;
lp_ptAddAlgoOrder                                       ApiFunc::ptAddAlgoOrder=NULL;
lp_ptGetOrderEx                                         ApiFunc::ptGetOrderEx=NULL;
lp_ptGetOrderByIDEx                                     ApiFunc::ptGetOrderByIDEx=NULL;
lp_ptGetOrderHistoryEx                                  ApiFunc::ptGetOrderHistoryEx=NULL;
lp_ptAddProtectionOrder                                 ApiFunc::ptAddProtectionOrder=NULL;
lp_ptAddBatchOrder                                      ApiFunc::ptAddBatchOrder=NULL;
lp_ptAddAggregateOrder                                  ApiFunc::ptAddAggregateOrder=NULL;
lp_ptAddCustRequest                                     ApiFunc::ptAddCustRequest=NULL;
lp_ptReParent                                           ApiFunc::ptReParent=NULL;
lp_ptDoneForDay                                         ApiFunc::ptDoneForDay=NULL;
lp_ptAddCrossingOrder                                   ApiFunc::ptAddCrossingOrder=NULL;
lp_ptAddBlockOrder                                      ApiFunc::ptAddBlockOrder=NULL;
lp_ptAddBasisOrder                                      ApiFunc::ptAddBasisOrder=NULL;
lp_ptAddAAOrder                                         ApiFunc::ptAddAAOrder=NULL;
lp_ptGetConsolidatedPosition                            ApiFunc::ptGetConsolidatedPosition=NULL;
lp_ptAmendOrder                                         ApiFunc::ptAmendOrder=NULL;
lp_ptAmendAlgoOrder                                     ApiFunc::ptAmendAlgoOrder=NULL;
lp_ptCancelOrder                                        ApiFunc::ptCancelOrder=NULL;
lp_ptActivateOrder                                      ApiFunc::ptActivateOrder=NULL;
lp_ptDeactivateOrder                                    ApiFunc::ptDeactivateOrder=NULL;
lp_ptCancelBuys                                         ApiFunc::ptCancelBuys=NULL;
lp_ptCancelSells                                        ApiFunc::ptCancelSells=NULL;
lp_ptCancelOrders                                       ApiFunc::ptCancelOrders=NULL;
lp_ptCancelAll                                          ApiFunc::ptCancelAll=NULL;
lp_ptOrderChecked                                       ApiFunc::ptOrderChecked=NULL;
lp_ptQueryOrderStatus                                   ApiFunc::ptQueryOrderStatus=NULL;
lp_ptCountOrderHistory                                  ApiFunc::ptCountOrderHistory=NULL;
lp_ptGetOrderHistory                                    ApiFunc::ptGetOrderHistory=NULL;
lp_ptSetUserIDFilter                                    ApiFunc::ptSetUserIDFilter=NULL;
lp_ptNextOrderSequence                                  ApiFunc::ptNextOrderSequence=NULL;
lp_ptLogOn                                              ApiFunc::ptLogOn=NULL;
lp_ptLogOff                                             ApiFunc::ptLogOff=NULL;
lp_ptGetLogonStatus                                     ApiFunc::ptGetLogonStatus=NULL;
lp_ptDOMEnabled                                         ApiFunc::ptDOMEnabled=NULL;
lp_ptPostTradeAmendEnabled                              ApiFunc::ptPostTradeAmendEnabled=NULL;
lp_ptEnabledFunctionality                               ApiFunc::ptEnabledFunctionality=NULL;
lp_ptCountUsrMsgs                                       ApiFunc::ptCountUsrMsgs=NULL;
lp_ptGetUsrMsg                                          ApiFunc::ptGetUsrMsg=NULL;
lp_ptGetUsrMsgByID                                      ApiFunc::ptGetUsrMsgByID=NULL;
lp_ptAcknowledgeUsrMsg                                  ApiFunc::ptAcknowledgeUsrMsg=NULL;
lp_ptPriceStep                                          ApiFunc::ptPriceStep=NULL;
lp_ptPLBurnRate                                         ApiFunc::ptPLBurnRate=NULL;
lp_ptOpenPositionExposure                               ApiFunc::ptOpenPositionExposure=NULL;
lp_ptBuyingPowerRemaining                               ApiFunc::ptBuyingPowerRemaining=NULL;
lp_ptBuyingPowerUsed                                    ApiFunc::ptBuyingPowerUsed=NULL;
lp_ptMarginForTrade                                     ApiFunc::ptMarginForTrade=NULL;
lp_ptTotalMarginPaid                                    ApiFunc::ptTotalMarginPaid=NULL;
lp_ptGetMarginPerLot                                    ApiFunc::ptGetMarginPerLot=NULL;
lp_ptSetOrderCancelFailureDelay                         ApiFunc::ptSetOrderCancelFailureDelay=NULL;
lp_ptSetOrderSentFailureDelay                           ApiFunc::ptSetOrderSentFailureDelay=NULL;
lp_ptSetOrderQueuedFailureDelay                         ApiFunc::ptSetOrderQueuedFailureDelay=NULL;
lp_ptCountContractAtBest                                ApiFunc::ptCountContractAtBest=NULL;
lp_ptGetContractAtBest                                  ApiFunc::ptGetContractAtBest=NULL;
lp_ptGetContractAtBestPrices                            ApiFunc::ptGetContractAtBestPrices=NULL;
lp_ptCountContractSubscriberDepth                       ApiFunc::ptCountContractSubscriberDepth=NULL;
lp_ptGetContractSubscriberDepth                         ApiFunc::ptGetContractSubscriberDepth=NULL;
lp_ptSuperTASEnabled                                    ApiFunc::ptSuperTASEnabled=NULL;
lp_ptSetSSL                                             ApiFunc::ptSetSSL=NULL;
lp_ptSetPDDSSL                                          ApiFunc::ptSetPDDSSL=NULL;
lp_ptSetMDSToken                                        ApiFunc::ptSetMDSToken=NULL;
lp_ptSetSSLCertificateName                              ApiFunc::ptSetSSLCertificateName=NULL;
lp_ptSetPDDSSLCertificateName                           ApiFunc::ptSetPDDSSLCertificateName=NULL;
lp_ptSetSSLClientAuthName                               ApiFunc::ptSetSSLClientAuthName=NULL;
lp_ptSetPDDSSLClientAuthName                            ApiFunc::ptSetPDDSSLClientAuthName=NULL;
lp_ptSetSuperTAS                                        ApiFunc::ptSetSuperTAS=NULL;
lp_ptSetMemoryWarning                                   ApiFunc::ptSetMemoryWarning=NULL;
lp_ptCreateStrategy                                     ApiFunc::ptCreateStrategy=NULL;
lp_ptGetOptionPremium                                   ApiFunc::ptGetOptionPremium=NULL;
lp_ptLockUpdates                                        ApiFunc::ptLockUpdates=NULL;
lp_ptUnlockUpdates                                      ApiFunc::ptUnlockUpdates=NULL;
lp_ptAddOrderEx                                         ApiFunc::ptAddOrderEx=NULL;
lp_ptAmendOrderEx                                       ApiFunc::ptAmendOrderEx=NULL;
lp_ptCancelOrderEx                                      ApiFunc::ptCancelOrderEx=NULL;
lp_ptCancelBuysEx                                       ApiFunc::ptCancelBuysEx=NULL;
lp_ptCancelSellsEx                                      ApiFunc::ptCancelSellsEx=NULL;
ApiFunc::ApiFunc(ClientType  type)
{
if(type==DemoClient)
	m_hInst_PATSAPIDLL = ::LoadLibrary("DEMOAPI.dll");
else
	m_hInst_PATSAPIDLL = ::LoadLibrary("PATSAPI.dll");
	if(m_hInst_PATSAPIDLL)
		GetFunctionAddrOfPats();
}

ApiFunc::~ApiFunc(void)
{
	if(m_hInst_PATSAPIDLL)
	{
		::FreeLibrary(m_hInst_PATSAPIDLL);
        m_hInst_PATSAPIDLL=NULL;
	}
}
//��ȡpatsapi�и�����������ڵ�ַ
bool ApiFunc::GetFunctionAddrOfPats(void)
{
	bool bRet=true;
	LOADFUNCTION(ptRegisterCallback);
	LOADFUNCTION(ptRegisterLinkStateCallback);
	LOADFUNCTION(ptRegisterMsgCallback);
	LOADFUNCTION(ptRegisterOrderCallback);
	LOADFUNCTION(ptRegisterOrderTypeUpdateCallback);
	LOADFUNCTION(ptRegisterFillCallback);
	LOADFUNCTION(ptRegisterPriceCallback);
	LOADFUNCTION(ptRegisterBlankPriceCallback);
	LOADFUNCTION(ptRegisterGenericPriceCallback);
	LOADFUNCTION(ptRegisterDOMCallback);
	LOADFUNCTION(ptRegisterContractCallback);
	LOADFUNCTION(ptRegisterCommodityCallback);
	LOADFUNCTION(ptRegisterExchangeCallback);
	LOADFUNCTION(ptRegisterStatusCallback);
	LOADFUNCTION(ptRegisterExchangeRateCallback);
	LOADFUNCTION(ptRegisterConStatusCallback);
	LOADFUNCTION(ptRegisterOrderCancelFailureCallback);
	LOADFUNCTION(ptRegisterOrderSentFailureCallback);
	LOADFUNCTION(ptRegisterOrderQueuedFailureCallback);
	LOADFUNCTION(ptRegisterAtBestCallback);
	LOADFUNCTION(ptRegisterTickerCallback);
	LOADFUNCTION(ptRegisterAmendFailureCallback);
	LOADFUNCTION(ptRegisterSubscriberDepthCallback);
	LOADFUNCTION(ptRegisterSettlementCallback);
	LOADFUNCTION(ptRegisterStrategyCreateSuccess);
	LOADFUNCTION(ptRegisterStrategyCreateFailure);
	LOADFUNCTION(ptRegisterOrderBookReset);
	//LOADFUNCTION(ptRegisterTraderAdded);
	LOADFUNCTION(ptCountExchanges);
	LOADFUNCTION(ptGetExchange);
	LOADFUNCTION(ptGetExchangeByName);
	LOADFUNCTION(ptExchangeExists);
	LOADFUNCTION(ptCountTraders);
	LOADFUNCTION(ptGetTrader);
	LOADFUNCTION(ptGetTraderByName);
	LOADFUNCTION(ptTraderExists);
	LOADFUNCTION(ptCountOrderTypes);
	LOADFUNCTION(ptGetOrderType);
	LOADFUNCTION(ptGetExchangeRate);
	LOADFUNCTION(ptCountReportTypes);
	LOADFUNCTION(ptGetReportType);
	LOADFUNCTION(ptReportTypeExists);
	LOADFUNCTION(ptGetReportSize);
	LOADFUNCTION(ptGetReport);
	LOADFUNCTION(ptCountCommodities);
	LOADFUNCTION(ptGetCommodity);
	LOADFUNCTION(ptCommodityExists);
	LOADFUNCTION(ptGetCommodityByName);
	LOADFUNCTION(ptCountContracts);
	LOADFUNCTION(ptGetContract);
	LOADFUNCTION(ptGetContractByName);
	LOADFUNCTION(ptGetContractByExternalID);
	LOADFUNCTION(ptGetExtendedContract);
	LOADFUNCTION(ptGetExtendedContractByName);
	LOADFUNCTION(ptContractExists);
	LOADFUNCTION(ptInitialise);
	LOADFUNCTION(ptGetAPIBuildVersion);
	LOADFUNCTION(ptReady);
	LOADFUNCTION(ptPurge);
	LOADFUNCTION(ptDisconnect);
	LOADFUNCTION(ptSetHostAddress);
	LOADFUNCTION(ptSetPriceAddress);
	LOADFUNCTION(ptEnable);
	LOADFUNCTION(ptDisable);
	LOADFUNCTION(ptLogString);
	LOADFUNCTION(ptOMIEnabled);
	LOADFUNCTION(ptNotifyAllMessages);
	LOADFUNCTION(ptSetHostReconnect);
	LOADFUNCTION(ptSetPriceReconnect);
	LOADFUNCTION(ptSetPriceAgeCounter);
	//INT ptSubscribePrice(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate);
	//ptSubscribePrice���ļ۸�Ҫ��۸�������ṩָ����Լ�ļ۸񣬼۸������ptPriceUpdate֪ͨ
	LOADFUNCTION(ptSubscribePrice);
    //INT  ptSubscribeToMarket(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate);
	//ptSubscribeToMarket����һ��������ȫ����Լ�۸񣬻���һ�����������ض���Լ��ͬʱ���ȫ���۸�
	LOADFUNCTION(ptSubscribeToMarket);
    //INT ptUnsubscribePrice(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate);
	//ptUnsubscribePrice����ȡ�����ļ۸�Ҫ��۸������ֹͣ���ͼ۸���Ϣ����ptSubscribePrice��Ӧ
	LOADFUNCTION(ptUnsubscribePrice);
    //INT ptUnsubscribeToMarket(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate);
	//ptUnsubscribeToMarket����ȡ�����ļ۸�Ҫ��۸������ֹͣ���ͼ۸���Ϣ����ptSubscribeToMarket��Ӧ
	LOADFUNCTION(ptUnsubscribeToMarket);
    //INT ptPriceSnapshot(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, INT Wait);
    //ptPriceSnapshot����Ҫ��۸�������ṩ������Լ�ļ۸񣬿���ֱ�ӵȴ����غ��������أ�������ߣ��۸���ͨ��ptPriceUpdate����
	LOADFUNCTION(ptPriceSnapshot);
	LOADFUNCTION(ptSetEncryptionCode);
	LOADFUNCTION(ptSetHandShakePeriod);
	LOADFUNCTION(ptSetHostHandShake);
	LOADFUNCTION(ptSetPriceHandShake);
	LOADFUNCTION(ptSetInternetUser);
	LOADFUNCTION(ptSetClientPath);
	LOADFUNCTION(ptGetErrorMessage);
	LOADFUNCTION(ptDumpLastError);
	LOADFUNCTION(ptSnapdragonEnabled);
	LOADFUNCTION(ptSubscribeBroadcast);
	LOADFUNCTION(ptUnsubscribeBroadcast);
	LOADFUNCTION(ptCountFills);
	LOADFUNCTION(ptGetFill);
	LOADFUNCTION(ptGetFillByID);
	LOADFUNCTION(ptGetContractPosition);
	LOADFUNCTION(ptGetOpenPosition);
	LOADFUNCTION(ptGetTotalPosition);
    //INT ptGetAveragePrice(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, TraderPtr TraderAccount, PricePtr Price);
	//ptGetAveragePrice�������ؽ����û�һ����Լ��ȫ���ҵ���ƽ���۸�
	LOADFUNCTION(ptGetAveragePrice);
	LOADFUNCTION(ptCountOrders);
	LOADFUNCTION(ptGetOrder);
	LOADFUNCTION(ptGetOrderByID);
	LOADFUNCTION(ptGetOrderIndex);
	//IN ptBlankPrices()
	//ptBlankPrices����ͨ�����ڿͻ��˺ͼ۸������ʧȥ������ʱ���API�洢�ļ۸����ݣ�ʹptGetPriceȫ������0
	LOADFUNCTION(ptBlankPrices);
	//INT ptGetPrice(INT Index, PriceStructPtr CurrentPrice);
	//ptGetPrice��������һ����Լ�ĵ�ǰ�۸�indexָ���Ǹ���Լ�۸񱻷��أ�ֵ��0��ptCountContracts �C 1.
	LOADFUNCTION(ptGetPrice);
    //INT  ptGetPriceForContract(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, PriceStructPtr CurrentPrice);
	//ptGetPriceForContract��������һ����Լ�ĵ�ǰ�۸񣬲���ExchangeName��ContractName��ContractDateȷ��һ����Լ
	LOADFUNCTION(ptGetPriceForContract);
	//INT  ptGetGenericPrice(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, INT PriceType, INT Side, PriceStructPtr Price);
	//ptGetGenericPrice���������û���ȡPriceStruct���ض��ļ۸���
	LOADFUNCTION(ptGetGenericPrice);
	//INT  ptAddOrder(NewOrderStructPtr NewOrder, OrderIDPtr OrderID);
	//ptAddOrder�����ύһ���±��������׷�����������״̬ͨ��ptOrder�ص���������.���ݷ��صı���IDʹ��ptGetOrderByID��ȡ��ϸ��Ϣ
	LOADFUNCTION(ptAddOrder);
	//INT  ptAddAlgoOrder(NewOrderStructPtr NewOrder, INT BuffSize, AlgoBuff AlgoBuffer, OrderIDPtr OrderID);
	//ptAddAlgoOrder������ʽͬptAddOrder��ֻ�Ǹ�����һ����ALGOʹ�õ�XML�ַ���
	LOADFUNCTION(ptAddAlgoOrder);
    //INT ptGetOrderEx(INT Index, OrderDetailStructPtr OrderDetail, AlgoBuffPtr AlgoDetail, PINT AlgoSize);
	LOADFUNCTION(ptGetOrderEx);
	LOADFUNCTION(ptGetOrderByIDEx);
	LOADFUNCTION(ptAddProtectionOrder);
	LOADFUNCTION(ptAddBatchOrder);
	LOADFUNCTION(ptAddAggregateOrder);
	LOADFUNCTION(ptAddCustRequest);
	LOADFUNCTION(ptReParent);
	LOADFUNCTION(ptDoneForDay);
	LOADFUNCTION(ptAddCrossingOrder);
	LOADFUNCTION(ptAddBlockOrder);
	LOADFUNCTION(ptAddBasisOrder);
	LOADFUNCTION(ptAddAAOrder);
	//LOADFUNCTION(ptGetConsolidatedPosition);
	//�ĵ�
	LOADFUNCTION(ptAmendOrder);
	//�ĵ���ʹ��Algo XML
	LOADFUNCTION(ptAmendAlgoOrder);
	//����һ��ָ������
	LOADFUNCTION(ptCancelOrder);
	LOADFUNCTION(ptActivateOrder);
	LOADFUNCTION(ptDeactivateOrder);
	//�����ض������˻���ȫ����
	LOADFUNCTION(ptCancelBuys);
	//�����ض������˻���ȫ������
	LOADFUNCTION(ptCancelSells);
    //����һ���ض��˻���ĳ��������ĳ����Լ��ȫ������
	LOADFUNCTION(ptCancelOrders);
	//�����ض������˻���ȫ������
	LOADFUNCTION(ptCancelAll);
	LOADFUNCTION(ptOrderChecked);
	LOADFUNCTION(ptQueryOrderStatus);
	//INT  ptCountOrderHistory(INT Index, PINT Count);
	//ptCountOrderHistory��������Ϊ�����������еı�����ʷ��¼������
	LOADFUNCTION(ptCountOrderHistory);
	//INT  ptGetOrderHistory(INT  Index, INT Position, OrderDetailStructPtr OrderDetail);
	//ptGetOrderHistory�������ر�����ϸ�������ǰ���ʱ����򷵻�
	LOADFUNCTION(ptGetOrderHistory);
	//INT ptGetOrderHistoryEx(INT  Index, INT Position, OrderDetailStructPtr OrderDetail, AlgoBuffPtr AlgoDetail, PINT AlgoSize);
	//ptGetOrderHistoryEx����������ptGetOrderHistory���ر�����ϸ�⣬������һ��Algo XML���ݺ�������ݵĴ�С
	LOADFUNCTION(ptGetOrderHistoryEx);
	LOADFUNCTION(ptSetUserIDFilter);
	LOADFUNCTION(ptNextOrderSequence);
	LOADFUNCTION(ptLogOn);
	LOADFUNCTION(ptLogOff);
	LOADFUNCTION(ptGetLogonStatus);
	LOADFUNCTION(ptDOMEnabled);
	LOADFUNCTION(ptPostTradeAmendEnabled);
	LOADFUNCTION(ptEnabledFunctionality);
	LOADFUNCTION(ptCountUsrMsgs);
	LOADFUNCTION(ptGetUsrMsg);
	LOADFUNCTION(ptGetUsrMsgByID);
	LOADFUNCTION(ptAcknowledgeUsrMsg);
	//INT ptPriceStep(double Price, double TickSize, INT NumSteps, INT TicksPerPoint);
	//ptPriceStep�Ǽ۸���㺯����return Price+TickSize*NumSteps*TicksPerPoint
	LOADFUNCTION(ptPriceStep);
	LOADFUNCTION(ptPLBurnRate);
	LOADFUNCTION(ptOpenPositionExposure);
	LOADFUNCTION(ptBuyingPowerRemaining);
	LOADFUNCTION(ptBuyingPowerUsed);
	LOADFUNCTION(ptMarginForTrade);
	LOADFUNCTION(ptTotalMarginPaid);
	LOADFUNCTION(ptGetMarginPerLot);
	LOADFUNCTION(ptSetOrderCancelFailureDelay);
	LOADFUNCTION(ptSetOrderSentFailureDelay);
	LOADFUNCTION(ptSetOrderQueuedFailureDelay);
	LOADFUNCTION(ptCountContractAtBest);
	LOADFUNCTION(ptGetContractAtBest);
	LOADFUNCTION(ptGetContractAtBestPrices);
	LOADFUNCTION(ptCountContractSubscriberDepth);
	LOADFUNCTION(ptGetContractSubscriberDepth);
	LOADFUNCTION(ptSuperTASEnabled);
	LOADFUNCTION(ptSetSSL);
	LOADFUNCTION(ptSetPDDSSL);
	LOADFUNCTION(ptSetMDSToken);
	LOADFUNCTION(ptSetSSLCertificateName);
	LOADFUNCTION(ptSetPDDSSLCertificateName);
	LOADFUNCTION(ptSetSSLClientAuthName);
	LOADFUNCTION(ptSetPDDSSLClientAuthName);
	LOADFUNCTION(ptSetSuperTAS);
	LOADFUNCTION(ptSetMemoryWarning);
	LOADFUNCTION(ptCreateStrategy);
	LOADFUNCTION(ptGetOptionPremium);
	LOADFUNCTION(ptLockUpdates);
	LOADFUNCTION(ptUnlockUpdates);
/*���׺���***************************************************************************************/
    //INT ptAddOrderEx(NewOrderStructPtr NewOrder, OrderIDPtr OrderID, UserNameStrPtr UserName);
	//�������ͬptAddOrder��ֻ�Ǹ�����һ����¼�û��������ڶ��û����س���
	LOADFUNCTION(ptAddOrderEx);
	//�ĵ��������û���
	LOADFUNCTION(ptAmendOrderEx);
	//����һ��ָ�������������û���
	LOADFUNCTION(ptCancelOrderEx);
	//�����ض������˻���ȫ���򵥣������û���
	LOADFUNCTION(ptCancelBuysEx);
	//�����ض������˻���ȫ�������������û���
	LOADFUNCTION(ptCancelSellsEx);
	return bRet; 
}