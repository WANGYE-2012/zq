#include "StdAfx.h"
#include "CmeCoreApi.h"
#include <string>
#define GET_EXCHANGEID(ExchID,InstID)\
do{\
    map<string,EPCID>::const_iterator iter=m_ContractName2ID.find(InstID);\
    if(iter!=m_ContractName2ID.end())\
    {\
        map<int,Stru_Base_ExchangeInfo>::const_iterator exch_iter=m_ExchangeInfos.find(iter->second.ExchangeID);\
        if(exch_iter!=m_ExchangeInfos.end())\
        {\
            strcpy_s(ExchID,sizeof(ExchID)-1,exch_iter->second.ExchangeCode);\
        }\
     }\
}while(0)
#define STRUCT_SWAP_BEGIN(src)   switch(src){
#define STRUCT_SWAP_END()        default:break;}
#define STRUCT_SWAP_ITEM(dest,srcV,destV)  case srcV:dest=destV;break;
static uint32 DataConvert(const char* pStr)
{
    return 0;
}
static uint32 TimeConvert(const char* pStr)
{
    return 0;
}
//���PlatformStru_InstrumentInfo�ṹ��
void CCmeCoreApi::FillInstrumentInfo(const int ExchangeID,const Stru_Base_ContractInfo& ContractInfo,PlatformStru_InstrumentInfo& Instrument)
{
    memset(&Instrument,0,sizeof(Instrument));
    Stru_Base_ProductInfo&  ProductInfo=m_ProductInfos[ExchangeID][ContractInfo.ProductID];
    Stru_Base_ExchangeInfo& ExchangeInfo=m_ExchangeInfos[ExchangeID];
    //
    MakeGUIContractID(ExchangeInfo,ProductInfo,ContractInfo,Instrument.InstrumentID);
    strcpy_s(Instrument.ExchangeID,sizeof(Instrument.ExchangeID)-1,ExchangeInfo.ExchangeCode);
    strcpy_s(Instrument.InstrumentName,sizeof(Instrument.InstrumentName)-1,Instrument.InstrumentID);
    strcpy_s(Instrument.ExchangeInstID,sizeof(Instrument.ExchangeInstID)-1,Instrument.InstrumentID);
    strcpy_s(Instrument.ProductID,sizeof(Instrument.ProductID)-1,ProductInfo.ProductName);
    Instrument.ProductClass=THOST_FTDC_PC_Futures;
    struct tm* UTCtime=localtime((time_t *)&ContractInfo.ExpiryDate);
    Instrument.DeliveryYear=UTCtime->tm_year+1900;
    Instrument.DeliveryMonth=UTCtime->tm_mon;

    Instrument.VolumeMultiple=ProductInfo.OnePoint;
    Instrument.PriceTick=ProductInfo.TickSize;
    map<int,Stru_Base_CurrencyInfo>::iterator curr_it=m_CurrencyInfos.find(ProductInfo.CurrencyID);
    if(curr_it!=m_CurrencyInfos.end())
        sprintf_s(Instrument.Currency,sizeof(Instrument.Currency)-1,curr_it->second.CurrencyName);
    Instrument.TicksPerPoint=ProductInfo.TicksPerPoint;
	sprintf_s(Instrument.TickSize,sizeof(Instrument.TickSize)-1,"%f",ProductInfo.TickSize);
}
void CCmeCoreApi::FillDepthMarketData(const Stru_Base_Market_SnapshotData& md,CThostFtdcDepthMarketDataField& field)
{
//    ///������
//    {
//        struct tm* UTCtime=localtime((time_t *)&md.TradingDay);
//        sprintf_s(field.TradingDay,sizeof(field.TradingDay)-1,"%04d%02d%02d",UTCtime->tm_year+1900,UTCtime->tm_mon,UTCtime->tm_mday);
//    }
//    ///��Լ����,��Լ�ڽ������Ĵ���
//    {
//        map<int,string>::const_iterator iter=m_ID2ContractName.find(md.ContractID);
//        if(iter!=m_ID2ContractName.end())
//        {
//            strcpy_s(field.InstrumentID,sizeof(field.InstrumentID)-1,iter->second.c_str());
//            strcpy_s(field.ExchangeInstID,sizeof(field.ExchangeInstID)-1,iter->second.c_str());
//        }
//    }
//    ///����������
//    {
//        map<string,EPCID>::const_iterator iter=m_ContractName2ID.find(field.InstrumentID);
//        if(iter!=m_ContractName2ID.end())
//        {
//            map<int,Stru_Base_ExchangeInfo>::const_iterator exch_iter=m_ExchangeInfos.find(iter->second.ExchangeID);
//            if(exch_iter!=m_ExchangeInfos.end())
//            {
//                strcpy_s(field.ExchangeID,sizeof(field.ExchangeID)-1,exch_iter->second.ExchangeCode);
//            }
//        }
//    }
//	///���¼�
//    field.LastPrice=md.LastPrice;
//	///�ϴν����
//    field.PreSettlementPrice=md.PreSettlementPrice;
//	///������
//    field.PreClosePrice=md.PreClosePrice;
// 	///��ֲ���
//    field.PreOpenInterest=md.PreOpenInterest;
//	///����
//    field.OpenPrice=md.OpenPrice;
//	///��߼�
//    field.HighestPrice=md.HighestPrice;
//	///��ͼ�
//    field.LowestPrice=md.LowestPrice;   
//	///����
//    field.Volume=md.Volume;
//	///�ɽ����
//    field.Turnover=md.Trunover;
//	///�ֲ���
//    field.OpenInterest=md.OpenInterest;
//	///������
//    field.ClosePrice=md.ClosePrice;
//	///���ν����
//    field.SettlementPrice=md.SettlementPrice;
//	///��ͣ���
//    field.UpperLimitPrice=md.UpperLimitPrice;
//	///��ͣ���
//    field.LowerLimitPrice=md.LowerLimitPrice;
//	///����ʵ��
//	field.PreDelta=0;
//	///����ʵ��
//	field.CurrDelta=0;
//	///����޸�ʱ��
//    {
//        struct tm* UTCtime=localtime((time_t *)&md.UpdateTime);
//        sprintf_s(field.UpdateTime,sizeof(field.UpdateTime)-1,"%04d%02d%02d",UTCtime->tm_year+1900,UTCtime->tm_mon,UTCtime->tm_mday);
//    }
//	///����޸ĺ���
//	field.UpdateMillisec=GetTickCount()%1000;
//#define BIDASK_N(index)\
//do{\
//    field.BidPrice##index=md.BidPrice[index-1];\
//    field.BidVolume##index=md.BidVolume[index-1];\
//    field.AskPrice##index=md.AskPrice[index-1];\
//    field.AskVolume##index=md.AskVolume[index-1];\
//}while(0)
//    BIDASK_N(1);
//    BIDASK_N(2);
//    BIDASK_N(3);
//    BIDASK_N(4);
//    BIDASK_N(5);
}
void CCmeCoreApi::FillDepthMarketData(const Stru_Client_NewMarketData& md,PlatformStru_DepthMarketData& field)
{
}
void CCmeCoreApi::FillOrderInfo(const Stru_Base_OrderInfo& Info,PlatformStru_OrderInfo& Order)
{
    ///���͹�˾����
	Order.BrokerID;
	///Ͷ���ߴ���
	sprintf_s(Order.InvestorID,sizeof(Order.InvestorID)-1,"%d",Info.AccountID);
	///��Լ����
    {
        map<int,string>::const_iterator iter=m_ID2ContractName.find(Info.ContractID);
        if(iter!=m_ID2ContractName.end())
        {
            strcpy_s(Order.InstrumentID,sizeof(Order.InstrumentID)-1,iter->second.c_str());
            strcpy_s(Order.ExchangeInstID,sizeof(Order.ExchangeInstID)-1,iter->second.c_str());
        }
    }
	///��������
	Order.OrderRef;
	///�û�����
	Order.UserID;
	///�����۸�����
	Order.OrderPriceType;
	///��������
	Order.Direction;
	///��Ͽ�ƽ��־
	Order.CombOffsetFlag;
	///���Ͷ���ױ���־
	Order.CombHedgeFlag;
	///�۸�
	Order.LimitPrice;
	///����
	Order.VolumeTotalOriginal;
	///��Ч������
	Order.TimeCondition;
	///GTD����
	Order.GTDDate;
	///�ɽ�������
	Order.VolumeCondition;
	///��С�ɽ���
	Order.MinVolume;
	///��������
	Order.ContingentCondition;
	///ֹ���
	Order.StopPrice;
	///ǿƽԭ��
	Order.ForceCloseReason;
	///�Զ������־
	Order.IsAutoSuspend;
	///ҵ��Ԫ
	Order.BusinessUnit;
	///������
	Order.RequestID;
	///���ر������
	Order.OrderLocalID;
	///����������
	Order.ExchangeID;
	///��Ա����
	Order.ParticipantID;
	///�ͻ�����
	Order.ClientID;
	///����������Ա����
	Order.TraderID;
	///��װ���
	Order.InstallID;
	///�����ύ״̬
	Order.OrderSubmitStatus;
	///������ʾ���
	Order.NotifySequence;
	///������
	Order.TradingDay;
	///������
	Order.SettlementID;
	///�������
	Order.OrderSysID;
	///������Դ
	Order.OrderSource;
	///����״̬
	Order.OrderStatus;
	///��������
	Order.OrderType;
	///��ɽ�����
	Order.VolumeTraded;
	///ʣ������
	Order.VolumeTotal;
	///��������
	Order.InsertDate;
	///ί��ʱ��
	Order.InsertTime;
	///����ʱ��
	Order.ActiveTime;
	///����ʱ��
	Order.SuspendTime;
	///����޸�ʱ��
	Order.UpdateTime;
	///����ʱ��
	Order.CancelTime;
	///����޸Ľ���������Ա����
	Order.ActiveTraderID;
	///�����Ա���
	Order.ClearingPartID;
	///���
	Order.SequenceNo;
	///ǰ�ñ��
	Order.FrontID;
	///�Ự���
	Order.SessionID;
	///�û��˲�Ʒ��Ϣ
	Order.UserProductInfo;
	///״̬��Ϣ
	Order.StatusMsg;
	///�û�ǿ����־
	Order.UserForceClose;
	///�����û�����
	Order.ActiveUserID;
	///���͹�˾�������
	Order.BrokerOrderSeq;
	///��ر���
	Order.RelativeOrderSysID;
    //�ɽ�����
	Order.AvgPrice;
	//�۸�2
	Order.Price2;  
}
//��ѯ�ɽ�����
void CCmeCoreApi::FillTradeInfo(const Stru_Base_TradeInfo& src,PlatformStru_TradeInfo& info)
{
}
//��ѯ�ֲַ���
void CCmeCoreApi::FillPositionInfo(const Stru_Base_PositionInfo& src,PlatformStru_Position& pos)
{
}
//��ѯ�ʽ𷵻�
void CCmeCoreApi::FillTradingAccount(const Stru_Base_EquityInfo& src,CThostFtdcTradingAccountField& field)
{

}
//��ѯ����𷵻�
void CCmeCoreApi::FillTradingAccount(const Stru_Client_DepositWithdrawDynaUpdate& src,CThostFtdcTradingAccountField& field)
{
    switch(src.UpdateFlag)
    {
    case UpdateFlag_new: 
    case UpdateFlag_modify:
        FillTradingAccount(src.Info,field);
        break;
    case UpdateFlag_remove:
        memset(&field,0,sizeof(field));
        break;
    }
}
//��ѯ����𷵻�
void CCmeCoreApi::FillTradingAccount(const Stru_Base_DepositWithdrawDyna& src,CThostFtdcTradingAccountField& field)
{
    sprintf_s(field.AccountID,sizeof(field.AccountID)-1,"%d",src.AccountID);
    field.Deposit=src.Deposit;;
    field.Withdraw=src.Withdraw;
    struct tm* UTCtime=localtime((time_t *)&src.TradingDay);
    sprintf_s(field.TradingDay,sizeof(field.TradingDay)-1,"%04d%02d%02d",UTCtime->tm_year+1900,UTCtime->tm_mon,UTCtime->tm_mday);
}
//������������
void CCmeCoreApi::FillOrderCmdInfo(const PlatformStru_InputOrder& src,Stru_Base_OrderCmdInfo& field)
{
    field.OrderCommandID=atoi(src.OrderRef);
    field.OrderCmdFlag=OrderCmdFlag_New;
    field.OrderID=atoi(src.OrderRef);
    field.AccountID=atoi(src.InvestorID);
    map<string,EPCID>::const_iterator thisEPCID=m_ContractName2ID.find(src.InstrumentID);
    if(thisEPCID!=m_ContractName2ID.end())
        field.ContractID=thisEPCID->second.ContractID;
    if(src.CombHedgeFlag[0]==THOST_FTDC_HF_Speculation)field.HedgeFlag=HedgeFlag_Speculation;
    else if(src.CombHedgeFlag[0]==THOST_FTDC_HF_Hedge)  field.HedgeFlag=HedgeFlag_Hedge;
    if(src.Direction==THOST_FTDC_D_Buy)  field.DirectionFlag=DirectionFlag_Long;
    else if(src.Direction==THOST_FTDC_D_Sell)  field.DirectionFlag=DirectionFlag_Short;
    STRUCT_SWAP_BEGIN(src.OrderPriceType)
        STRUCT_SWAP_ITEM(field.OrderType,THOST_FTDC_OPT_LimitPrice,OrderType_Limit)
        STRUCT_SWAP_ITEM(field.OrderType,THOST_FTDC_OPT_AnyPrice,OrderType_Market)
    STRUCT_SWAP_END()
    field.LimitPrice=src.LimitPrice;
    STRUCT_SWAP_BEGIN(src.ForceCloseReason)
        STRUCT_SWAP_ITEM(field.ForceCloseReason,THOST_FTDC_FCC_NotForceClose,EnumForceCloseReason_NotForceClose)          
        STRUCT_SWAP_ITEM(field.ForceCloseReason, THOST_FTDC_FCC_LackDeposit,EnumForceCloseReason_LackDeposit)         
        STRUCT_SWAP_ITEM(field.ForceCloseReason, THOST_FTDC_FCC_ClientOverPositionLimit,EnumForceCloseReason_ClientOverPositionLimit)
        STRUCT_SWAP_ITEM(field.ForceCloseReason,THOST_FTDC_FCC_MemberOverPositionLimit,EnumForceCloseReason_MemberOverPositionLimit)
        STRUCT_SWAP_ITEM(field.ForceCloseReason,  THOST_FTDC_FCC_NotMultiple,EnumForceCloseReason_NotMultiple)    
        STRUCT_SWAP_ITEM(field.ForceCloseReason,   THOST_FTDC_FCC_Violation,EnumForceCloseReason_Violation)                      
        STRUCT_SWAP_ITEM(field.ForceCloseReason,    THOST_FTDC_FCC_Other,EnumForceCloseReason_Other)
    STRUCT_SWAP_END()
    if(src.ForceCloseReason==THOST_FTDC_FCC_NotForceClose) field.ForceCloseFlag=ForceCloseFlag_Not;
    else field.ForceCloseFlag=ForceCloseFlag_CGT;
    if(src.ContingentCondition==THOST_FTDC_CC_Immediately) field.TriggerFlag=TriggerFlag_Immediately;
    else if(src.ContingentCondition==THOST_FTDC_CC_ParkedOrder)field.TriggerFlag=TriggerFlag_Time;

    switch(src.TimeCondition){
    case THOST_FTDC_TC_IOC:field.ExpiryFlag=OrderExpiryFlag_Common;break;
    case THOST_FTDC_TC_GTD:field.ExpiryFlag=OrderExpiryFlag_GTD;break;
    case THOST_FTDC_TC_GTC:field.ExpiryFlag=OrderExpiryFlag_GTC;break;
    default:break;
    }
    field.TriggerPriceForStop=src.StopPrice;
    field.TradingDay=DataConvert(src.GTDDate);
    field.VolumeMin=src.MinVolume;
    field.Volume=src.VolumeTotalOriginal;
    field.GTDTime=TimeConvert(src.GTDDate);
}
//����������Ϊ
void CCmeCoreApi::FillOrderCmdInfo(const CThostFtdcInputOrderActionField& src,Stru_Base_OrderCmdInfo& field)
{
    field.OrderCommandID=atoi(src.OrderRef);
    if(src.ActionFlag==THOST_FTDC_AF_Delete) field.OrderCmdFlag=OrderCmdFlag_Cancel;
    else if(src.ActionFlag==THOST_FTDC_AF_Modify) field.OrderCmdFlag=OrderCmdFlag_Amend;
    field.OrderID=atoi(src.OrderRef);
    field.AccountID=atoi(src.InvestorID);
    field.LoginSessionID=src.SessionID;
    map<string,EPCID>::const_iterator thisEPCID=m_ContractName2ID.find(src.InstrumentID);
    if(thisEPCID!=m_ContractName2ID.end())
        field.ContractID=thisEPCID->second.ContractID;
    field.TriggerFlag=TriggerFlag_Immediately;
    field.Volume=src.VolumeChange;
}
//����Ԥ�񵥲���
void CCmeCoreApi::FillOrderCmdInfo(const CThostFtdcParkedOrderField& src,Stru_Base_OrderCmdInfo& field)
{
    //�ݲ�ʵ��
}
//����Ԥ����Ϊ
void CCmeCoreApi::FillOrderCmdInfo(const CThostFtdcParkedOrderActionField& src,Stru_Base_OrderCmdInfo& field)
{
    //�ݲ�ʵ��
}
//�������뷵��
void CCmeCoreApi::FillOrderCmdInfo(const EnumOrderCmdFlag flag,const Stru_Base_OrderInfo src,CThostFtdcInputOrderField& field)
{
    //�ݲ�ʵ��
}
//������Ϊ����
void CCmeCoreApi::FillOrderCmdInfo(const EnumOrderCmdFlag flag,const Stru_Base_OrderInfo src,CThostFtdcInputOrderActionField& field)
{
    string strInstrument=m_ID2ContractName[src.ContractID];
    ///���͹�˾���� BrokerID;
	///Ͷ���ߴ��� InvestorID	
    sprintf_s(field.InvestorID,sizeof(field.InvestorID)-1,"%d",src.AccountID);
	///������������OrderActionRef;
	field.OrderActionRef=src.OrderID;	
	///��������OrderRef;
    sprintf_s(field.OrderRef,sizeof(field.OrderRef)-1,"%d",src.OrderID);
	///������ RequestID;
	///ǰ�ñ�� FrontID;
	///�Ự��� SessionID;
    field.SessionID=src.LoginSessionID;
	///���������� ExchangeID;
    GET_EXCHANGEID(field.ExchangeID,strInstrument);
	///�������
    sprintf_s(field.OrderSysID,sizeof(field.OrderSysID)-1,"%d",src.OrderID);;
	///������־
	if(flag==OrderCmdFlag_Amend) field.ActionFlag=THOST_FTDC_AF_Modify;
    else if(flag==OrderCmdFlag_Cancel) field.ActionFlag=THOST_FTDC_AF_Delete;
	///�۸�
    field.LimitPrice=src.LimitPrice;
	///�����仯
    field.VolumeChange=src.VolumeTraded;
	///�û�����
    sprintf_s(field.UserID,sizeof(field.UserID)-1,"%d",src.AccountID);
	///��Լ����
    strcpy_s(field.InstrumentID,sizeof(field.InstrumentID)-1,strInstrument.c_str());
}
//Ԥ�񵥲��뷵��
void CCmeCoreApi::FillOrderCmdInfo(const EnumOrderCmdFlag flag,const Stru_Base_OrderInfo src,CThostFtdcParkedOrderField& field)
{
    //�ݲ�ʵ��
}
//Ԥ����Ϊ����
void CCmeCoreApi::FillOrderCmdInfo(const EnumOrderCmdFlag flag,const Stru_Base_OrderInfo src,CThostFtdcParkedOrderActionField& field)
{
    //�ݲ�ʵ��
}
//����״̬����
void CCmeCoreApi::FillRtnOrder(const Stru_Base_OrderInfo& src,PlatformStru_OrderInfo& field)
{
    ///���͹�˾���� BrokerID;
	///Ͷ���ߴ��� InvestorID	
    sprintf_s(field.InvestorID,sizeof(field.InvestorID)-1,"%d",src.AccountID);
	///��Լ����InstrumentID;
    string strInstrument=m_ID2ContractName[src.ContractID];
    strcpy_s(field.InstrumentID,sizeof(field.InstrumentID)-1,strInstrument.c_str());
	///��������OrderRef;
    sprintf_s(field.OrderRef,sizeof(field.OrderRef)-1,"%d",src.OrderID);
    ///�û�����UserID
    sprintf_s(field.UserID,sizeof(field.UserID)-1,"%d",src.AccountID);
 	///�����۸�����OrderPriceType;
    switch(src.OrderType){
    case OrderType_Market:field.OrderPriceType=THOST_FTDC_OPT_AnyPrice;break;
    case OrderType_Limit:field.OrderPriceType=THOST_FTDC_OPT_LimitPrice;break;
    default:break;
    }
	///��������Direction;
    switch(src.DirectionFlag){
    case DirectionFlag_Long:field.Direction=THOST_FTDC_D_Buy;break;                      //long, also means buy
    case DirectionFlag_Short:field.Direction=THOST_FTDC_D_Sell;break;                      //short, also means sell
    default:break;
    }
	///��Ͽ�ƽ��־CombOffsetFlag,����Ϊ��ƽ��Ϣ
    field.CombHedgeFlag[0]=0;
	///���Ͷ���ױ���־CombHedgeFlag;
    switch(src.HedgeFlag){
    case HedgeFlag_Speculation:field.CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;break;
    case HedgeFlag_Hedge:field.CombHedgeFlag[0]=THOST_FTDC_HF_Hedge;break;
    default:break;
    }
	///�۸�LimitPrice;
    field.LimitPrice=src.LimitPrice;
	///����VolumeTotalOriginal;
    field.VolumeTotalOriginal=src.Volume;
	///��Ч������TimeCondition;
    switch(src.ExpiryFlag){
    case OrderExpiryFlag_Common:field.TimeCondition=THOST_FTDC_TC_IOC;break;
    case OrderExpiryFlag_GTD:field.TimeCondition=THOST_FTDC_TC_GTD;break;
    case OrderExpiryFlag_GTC:field.TimeCondition=THOST_FTDC_TC_GTC;break;
    default:break;
    }
	///GTD����GTDDate;
    {
        struct tm* UTCtime=localtime((time_t *)&src.GTDTime);
        sprintf_s(field.GTDDate,sizeof(field.GTDDate)-1,"%04d%02d%02d",UTCtime->tm_year+1900,UTCtime->tm_mon,UTCtime->tm_mday);
    }
    ///�ɽ�������VolumeCondition;
    field.VolumeCondition= THOST_FTDC_VC_AV;
	///��С�ɽ��� MinVolume;
    field.MinVolume=src.VolumeMin;
	///�������� ContingentCondition;
    switch(src.TriggerFlag){
        case TriggerFlag_Immediately:field.ContingentCondition=THOST_FTDC_CC_Immediately;break;
        case TriggerFlag_Time:field.ContingentCondition=THOST_FTDC_CC_ParkedOrder;break;
        default:break;
    }
    //ֹ���StopPrice;
    field.StopPrice=src.TriggerPriceForStop;
    //ǿƽԭ��ForceCloseReason;
    STRUCT_SWAP_BEGIN(src.ForceCloseReason)
        STRUCT_SWAP_ITEM(field.ForceCloseReason,EnumForceCloseReason_NotForceClose,THOST_FTDC_FCC_NotForceClose)          
        STRUCT_SWAP_ITEM(field.ForceCloseReason,EnumForceCloseReason_LackDeposit, THOST_FTDC_FCC_LackDeposit)         
        STRUCT_SWAP_ITEM(field.ForceCloseReason,EnumForceCloseReason_ClientOverPositionLimit, THOST_FTDC_FCC_ClientOverPositionLimit)
        STRUCT_SWAP_ITEM(field.ForceCloseReason,EnumForceCloseReason_MemberOverPositionLimit,THOST_FTDC_FCC_MemberOverPositionLimit)
        STRUCT_SWAP_ITEM(field.ForceCloseReason,EnumForceCloseReason_NotMultiple,  THOST_FTDC_FCC_NotMultiple)    
        STRUCT_SWAP_ITEM(field.ForceCloseReason,EnumForceCloseReason_Violation,   THOST_FTDC_FCC_Violation)                      
        STRUCT_SWAP_ITEM(field.ForceCloseReason,EnumForceCloseReason_Other,    THOST_FTDC_FCC_Other)
    STRUCT_SWAP_END()
	///�Զ������־ IsAutoSuspend;
    field.IsAutoSuspend=0;
	///ҵ��ԪBusinessUnit;
	///������	RequestID;
	///���ر������OrderLocalID;
    sprintf_s(field.OrderLocalID,sizeof(field.OrderLocalID)-1,"%d",src.OrderID);
	///���������� ExchangeID;
    GET_EXCHANGEID(field.ExchangeID,strInstrument);
	///��Ա����ParticipantID;
	///�ͻ�����ClientID;
    sprintf_s(field.ClientID,sizeof(field.ClientID)-1,"%d",src.AccountID);
	///��Լ�ڽ������Ĵ��� ExchangeInstID;
    GET_EXCHANGEID(field.ExchangeInstID,strInstrument);
	///����������Ա���� TraderID;
	///��װ��� nstallID;
	///�����ύ״̬	OrderSubmitStatus
	///������ʾ���NotifySequence;
	///������ TradingDay;
    {
        struct tm* UTCtime=localtime((time_t *)&src.TradingDay);
        sprintf_s(field.TradingDay,sizeof(field.TradingDay)-1,"%04d%02d%02d",UTCtime->tm_year+1900,UTCtime->tm_mon,UTCtime->tm_mday);
    }
	///������SettlementID;
	///������� OrderSysID;
    strcpy_s(field.OrderSysID,sizeof(field.OrderSysID)-1,src.OrderExchangeCode);
	///������Դ OrderSource;
    field.OrderSource=THOST_FTDC_OSRC_Participant;
	///����״̬ OrderStatus;
    STRUCT_SWAP_BEGIN(src.OrderStatus)
        STRUCT_SWAP_ITEM(field.OrderStatus,OrderStatus_Unknown,THOST_FTDC_OST_Unknown)
        STRUCT_SWAP_ITEM(field.OrderStatus,OrderStatus_NotTrigged,THOST_FTDC_OST_NotTouched)
        STRUCT_SWAP_ITEM(field.OrderStatus,OrderStatus_AllTraded,THOST_FTDC_OST_AllTraded)
        STRUCT_SWAP_ITEM(field.OrderStatus,OrderStatus_NoTradeQueueing,THOST_FTDC_OST_NoTradeQueueing)
        STRUCT_SWAP_ITEM(field.OrderStatus,OrderStatus_NoTradeNotQueueing,THOST_FTDC_OST_PartTradedNotQueueing)
        STRUCT_SWAP_ITEM(field.OrderStatus,OrderStatus_PartTradedQueueing,THOST_FTDC_OST_PartTradedQueueing)
        STRUCT_SWAP_ITEM(field.OrderStatus,OrderStatus_PartTradedNotQueueing,THOST_FTDC_OST_NoTradeNotQueueing)
        STRUCT_SWAP_ITEM(field.OrderStatus,OrderStatus_Canceled,THOST_FTDC_OST_Canceled)
    STRUCT_SWAP_END()
	///��������OrderType;
	///��ɽ�����VolumeTraded;
    field.VolumeTraded=src.VolumeTraded;
	///ʣ������VolumeTotal;
    field.VolumeTotal=src.Volume-src.VolumeTraded;
	///�������� InsertDate; ί��ʱ�� InsertTime;   
    {
        struct tm* UTCtime=localtime((time_t *)&src.OrderInsertTime);
        sprintf_s(field.InsertDate,sizeof(field.InsertDate)-1,"%04d%02d%02d",UTCtime->tm_year+1900,UTCtime->tm_mon,UTCtime->tm_mday);
        sprintf_s(field.InsertTime,sizeof(field.InsertTime)-1,"%02d:%02d:%02d",UTCtime->tm_hour,UTCtime->tm_min,UTCtime->tm_sec);
    }
	///����ʱ�� ActiveTime;
	///����ʱ�� SuspendTime;
	///����޸�ʱ�� UpdateTime;
    {
        struct tm* UTCtime=localtime((time_t *)&src.UpdateTime);
        sprintf_s(field.UpdateTime,sizeof(field.UpdateTime)-1,"%02d:%02d:%02d",UTCtime->tm_hour,UTCtime->tm_min,UTCtime->tm_sec);
    }
	///����ʱ�� CancelTime;
    {
        struct tm* UTCtime=localtime((time_t *)&src.OrderCancelTime);
        sprintf_s(field.CancelTime,sizeof(field.CancelTime)-1,"%02d:%02d:%02d",UTCtime->tm_hour,UTCtime->tm_min,UTCtime->tm_sec);
    }
	///����޸Ľ���������Ա���� ActiveTraderID;
	///�����Ա��� ClearingPartID;
	///��� SequenceNo;
	///ǰ�ñ�� FrontID;
	///�Ự��� SessionID;
    field.SessionID=src.LoginSessionID;
	///�û��˲�Ʒ��Ϣ UserProductInfo;
    strcpy_s(field.UserProductInfo,sizeof(field.UserProductInfo)-1,src.OrderClientCode);
	///״̬��Ϣ StatusMsg;
	///�û�ǿ����־
    if(src.ForceCloseFlag==ForceCloseFlag_Not)  field.UserForceClose=0;
    else if(src.ForceCloseFlag==ForceCloseFlag_CGT)  field.UserForceClose=1;
	///�����û����� ActiveUserID;
	///���͹�˾������� BrokerOrderSeq;
	///��ر��� RelativeOrderSysID;
	///�����û����� ActiveUserID;
	///���͹�˾������� BrokerOrderSeq;
	///��ر��� RelativeOrderSysID;
    //�ɽ����� AvgPrice;
	//�۸�2 Price2;
    // ���ص������� strLocalRequestID;	
    // �����˺�	strAccount;	
    sprintf_s(field.strAccount,sizeof(field.strAccount)-1,"%d",src.AccountID);		
    // ����������strExchange;	
    GET_EXCHANGEID(field.strExchange,strInstrument);
    //���ᱣ֤�� freezeMargin;              
    //���������� troubleMoney;              
}
//�����ѷ���
void CCmeCoreApi::FillCommissionMarginInfo(const Stru_Base_CustomerCommissionInfo& src,PlatformStru_InstrumentCommissionRate& cr)
{
    string strInstrument=m_ID2ContractName[src.ContractID];
    strcpy_s(cr.InstrumentID,sizeof(cr.InstrumentID)-1,strInstrument.c_str());
    sprintf_s(cr.InvestorID,sizeof(cr.InvestorID)-1,"%d",src.CustomerID);
    if(src.AmountOrNumFlag==AmountOrNumFlag_Amount)
    {
        cr.OpenRatioByMoney=src.OpenSpecCommission; 
        cr.CloseRatioByMoney=src.CloseSpecCommission; 
        cr.CloseTodayRatioByMoney=src.CloseTodaySpecCommission;
    }
    else if(src.AmountOrNumFlag==AmountOrNumFlag_Num)
    {
        cr.OpenRatioByVolume=src.OpenSpecCommission;
        cr.CloseRatioByVolume=src.CloseSpecCommission;
        cr.CloseTodayRatioByVolume=src.CloseTodaySpecCommission;
    }
}
//��֤�𷵻�
void CCmeCoreApi::FillCommissionMarginInfo(const Stru_Base_CustomerMarginInfo& src,PlatformStru_InstrumentMarginRate&  mr)
{
    string strInstrument=m_ID2ContractName[src.ContractID];
    strcpy_s(mr.InstrumentID,sizeof(mr.InstrumentID)-1,strInstrument.c_str());
    sprintf_s(mr.InvestorID,sizeof(mr.InvestorID)-1,"%d",src.CustomerID);
    if(src.AmountOrNumFlag==AmountOrNumFlag_Amount)
    {
        mr.LongMarginRatioByMoney=src.LongSpecMaintainMargin;
        mr.ShortMarginRatioByMoney=src.ShortSpecMaintainMargin;
    }
    else if(src.AmountOrNumFlag==AmountOrNumFlag_Num)
    {  
        mr.LongMarginRatioByVolume=src.LongSpecMaintainMargin;;
        mr.ShortMarginRatioByVolume=src.ShortSpecMaintainMargin;
    }
    mr.HedgeFlag=THOST_FTDC_HF_Speculation;
}