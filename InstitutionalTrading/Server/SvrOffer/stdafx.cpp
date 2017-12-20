// stdafx.cpp : source file that includes just the standard includes
// testtcpsvr.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
void CopyOrderField(PlatformStru_OrderInfo& data,const CThostFtdcOrderField& orderField)
{
	///���͹�˾����
	strcpy_s(data.BrokerID, sizeof(orderField.BrokerID), orderField.BrokerID);
	///Ͷ���ߴ���
	strcpy_s(data.InvestorID, sizeof(orderField.InvestorID), orderField.InvestorID);
	///��Լ����
	strcpy_s(data.InstrumentID, sizeof(orderField.InstrumentID), orderField.InstrumentID);
	///��������
	strcpy_s(data.OrderRef, sizeof(orderField.OrderRef), orderField.OrderRef);
	///�û�����
	strcpy_s(data.UserID, sizeof(orderField.UserID), orderField.UserID);
	///�����۸�����
	data.OrderPriceType = orderField.OrderPriceType;
	///��������
	data.Direction = orderField.Direction;
	///��Ͽ�ƽ��־
	data.CombOffsetFlag[0] = orderField.CombOffsetFlag[0];
	///���Ͷ���ױ���־
	strcpy_s(data.CombHedgeFlag, sizeof(orderField.CombHedgeFlag), orderField.CombHedgeFlag);
	///�۸�
	data.LimitPrice = orderField.LimitPrice;
	///����
	data.VolumeTotalOriginal = orderField.VolumeTotalOriginal;
	///��Ч������
	data.TimeCondition = orderField.TimeCondition;
	///GTD����
	strcpy_s(data.GTDDate, sizeof(orderField.GTDDate), orderField.GTDDate);
	///�ɽ�������
	data.VolumeCondition = orderField.VolumeCondition;
	///��С�ɽ���
	data.MinVolume = orderField.MinVolume;
	///��������
	data.ContingentCondition = orderField.ContingentCondition;
	///ֹ���
	data.StopPrice = orderField.StopPrice;
	///ǿƽԭ��
	data.ForceCloseReason = orderField.ForceCloseReason;
	///�Զ������־
	data.IsAutoSuspend = orderField.IsAutoSuspend;
	///ҵ��Ԫ
	strcpy_s(data.BusinessUnit, sizeof(orderField.BusinessUnit), orderField.BusinessUnit);
	///������
	data.RequestID = orderField.RequestID;
	///���ر������
	strcpy_s(data.OrderLocalID, sizeof(orderField.OrderLocalID), orderField.OrderLocalID);
	///����������
	strcpy_s(data.ExchangeID, sizeof(orderField.ExchangeID), orderField.ExchangeID);
	///��Ա����
	strcpy_s(data.ParticipantID, sizeof(orderField.ParticipantID), orderField.ParticipantID);
	///�ͻ�����
	strcpy_s(data.ClientID, sizeof(orderField.ClientID), orderField.ClientID);
	///��Լ�ڽ������Ĵ���
	strcpy_s(data.ExchangeInstID, sizeof(orderField.ExchangeInstID), orderField.ExchangeInstID);
	///����������Ա����
	strcpy_s(data.TraderID, sizeof(orderField.TraderID), orderField.TraderID);
	///��װ���
	data.InstallID = orderField.InstallID;
	///�����ύ״̬
	data.OrderSubmitStatus = orderField.OrderSubmitStatus;
	///������ʾ���
	data.NotifySequence = orderField.NotifySequence;
	///������
	strcpy_s(data.TradingDay, sizeof(orderField.TradingDay), orderField.TradingDay);
	///������
	data.SettlementID = orderField.SettlementID;
	///�������
	strcpy_s(data.OrderSysID, sizeof(orderField.OrderSysID), orderField.OrderSysID);
	///������Դ
	data.OrderSource = orderField.OrderSource;
	///����״̬
	data.OrderStatus = orderField.OrderStatus;
	data.ExStatus = PlatformStru_OrderInfo::ExSta_none;
	///��������
	data.OrderType = orderField.OrderType;
	///��ɽ�����
	data.VolumeTraded = orderField.VolumeTraded;
	///ʣ������
	data.VolumeTotal = orderField.VolumeTotal;
	///��������
	strcpy_s(data.InsertDate, sizeof(orderField.InsertDate), orderField.InsertDate);
	///ί��ʱ��
	strcpy_s(data.InsertTime, sizeof(orderField.InsertTime), orderField.InsertTime);
	///����ʱ��
	strcpy_s(data.ActiveTime, sizeof(orderField.ActiveTime), orderField.ActiveTime);
	///����ʱ��
	strcpy_s(data.SuspendTime, sizeof(orderField.SuspendTime), orderField.SuspendTime);
	///����޸�ʱ��
	strcpy_s(data.UpdateTime, sizeof(orderField.UpdateTime), orderField.UpdateTime);
	///����ʱ��
	strcpy_s(data.CancelTime, sizeof(orderField.CancelTime), orderField.CancelTime);
	///����޸Ľ���������Ա����
	strcpy_s(data.ActiveTraderID, sizeof(orderField.ActiveTraderID), orderField.ActiveTraderID);
	///�����Ա���
	strcpy_s(data.ClearingPartID, sizeof(orderField.ClearingPartID), orderField.ClearingPartID);
	///���
	data.SequenceNo = orderField.SequenceNo;
	///ǰ�ñ��
	data.FrontID = orderField.FrontID;
	///�Ự���
	data.SessionID = orderField.SessionID;
	///�û��˲�Ʒ��Ϣ
	strcpy_s(data.UserProductInfo, sizeof(orderField.UserProductInfo), orderField.UserProductInfo);
	///״̬��Ϣ
	strcpy_s(data.StatusMsg, sizeof(orderField.StatusMsg), orderField.StatusMsg);
	///�û�ǿ����־
	data.UserForceClose = orderField.UserForceClose;
	///�����û�����
	strcpy_s(data.ActiveUserID, sizeof(orderField.ActiveUserID), orderField.ActiveUserID);
	///���͹�˾�������
	data.BrokerOrderSeq = orderField.BrokerOrderSeq;
	///��ر���
	strcpy_s(data.RelativeOrderSysID, sizeof(orderField.RelativeOrderSysID), orderField.RelativeOrderSysID);


	strncpy(data.Account,orderField.InvestorID,sizeof(data.Account)-1);

}
void CopyCTPOrderField(CThostFtdcOrderField& data, PlatformStru_OrderInfo& orderField)
{
	///���͹�˾����
	strcpy_s(data.BrokerID, sizeof(orderField.BrokerID), orderField.BrokerID);
	///Ͷ���ߴ���
	strcpy_s(data.InvestorID, sizeof(orderField.InvestorID), orderField.InvestorID);
	///��Լ����
	strcpy_s(data.InstrumentID, sizeof(orderField.InstrumentID), orderField.InstrumentID);
	///��������
	strcpy_s(data.OrderRef, sizeof(orderField.OrderRef), orderField.OrderRef);
	///�û�����
	strcpy_s(data.UserID, sizeof(orderField.UserID), orderField.UserID);
	///�����۸�����
	data.OrderPriceType = orderField.OrderPriceType;
	///��������
	data.Direction = orderField.Direction;
	///��Ͽ�ƽ��־
	data.CombOffsetFlag[0] = orderField.CombOffsetFlag[0];
	///���Ͷ���ױ���־
	strcpy_s(data.CombHedgeFlag, sizeof(orderField.CombHedgeFlag), orderField.CombHedgeFlag);
	///�۸�
	data.LimitPrice = orderField.LimitPrice;
	///����
	data.VolumeTotalOriginal = orderField.VolumeTotalOriginal;
	///��Ч������
	data.TimeCondition = orderField.TimeCondition;
	///GTD����
	strcpy_s(data.GTDDate, sizeof(orderField.GTDDate), orderField.GTDDate);
	///�ɽ�������
	data.VolumeCondition = orderField.VolumeCondition;
	///��С�ɽ���
	data.MinVolume = orderField.MinVolume;
	///��������
	data.ContingentCondition = orderField.ContingentCondition;
	///ֹ���
	data.StopPrice = orderField.StopPrice;
	///ǿƽԭ��
	data.ForceCloseReason = orderField.ForceCloseReason;
	///�Զ������־
	data.IsAutoSuspend = orderField.IsAutoSuspend;
	///ҵ��Ԫ
	strcpy_s(data.BusinessUnit, sizeof(orderField.BusinessUnit), orderField.BusinessUnit);
	///������
	data.RequestID = orderField.RequestID;
	///���ر������
	strcpy_s(data.OrderLocalID, sizeof(orderField.OrderLocalID), orderField.OrderLocalID);
	///����������
	strcpy_s(data.ExchangeID, sizeof(orderField.ExchangeID), orderField.ExchangeID);
	///��Ա����
	strcpy_s(data.ParticipantID, sizeof(orderField.ParticipantID), orderField.ParticipantID);
	///�ͻ�����
	strcpy_s(data.ClientID, sizeof(orderField.ClientID), orderField.ClientID);
	///��Լ�ڽ������Ĵ���
	strcpy_s(data.ExchangeInstID, sizeof(orderField.ExchangeInstID), orderField.ExchangeInstID);
	///����������Ա����
	strcpy_s(data.TraderID, sizeof(orderField.TraderID), orderField.TraderID);
	///��װ���
	data.InstallID = orderField.InstallID;
	///�����ύ״̬
	data.OrderSubmitStatus = orderField.OrderSubmitStatus;
	///������ʾ���
	data.NotifySequence = orderField.NotifySequence;
	///������
	strcpy_s(data.TradingDay, sizeof(orderField.TradingDay), orderField.TradingDay);
	///������
	data.SettlementID = orderField.SettlementID;
	///�������
	strcpy_s(data.OrderSysID, sizeof(orderField.OrderSysID), orderField.OrderSysID);
	///������Դ
	data.OrderSource = orderField.OrderSource;
	///����״̬
	data.OrderStatus = orderField.OrderStatus;
//	data.ExStatus = PlatformStru_OrderInfo::ExSta_none;
	///��������
	data.OrderType = orderField.OrderType;
	///��ɽ�����
	data.VolumeTraded = orderField.VolumeTraded;
	///ʣ������
	data.VolumeTotal = orderField.VolumeTotal;
	///��������
	strcpy_s(data.InsertDate, sizeof(orderField.InsertDate), orderField.InsertDate);
	///ί��ʱ��
	strcpy_s(data.InsertTime, sizeof(orderField.InsertTime), orderField.InsertTime);
	///����ʱ��
	strcpy_s(data.ActiveTime, sizeof(orderField.ActiveTime), orderField.ActiveTime);
	///����ʱ��
	strcpy_s(data.SuspendTime, sizeof(orderField.SuspendTime), orderField.SuspendTime);
	///����޸�ʱ��
	strcpy_s(data.UpdateTime, sizeof(orderField.UpdateTime), orderField.UpdateTime);
	///����ʱ��
	strcpy_s(data.CancelTime, sizeof(orderField.CancelTime), orderField.CancelTime);
	///����޸Ľ���������Ա����
	strcpy_s(data.ActiveTraderID, sizeof(orderField.ActiveTraderID), orderField.ActiveTraderID);
	///�����Ա���
	strcpy_s(data.ClearingPartID, sizeof(orderField.ClearingPartID), orderField.ClearingPartID);
	///���
	data.SequenceNo = orderField.SequenceNo;
	///ǰ�ñ��
	data.FrontID = orderField.FrontID;
	///�Ự���
	data.SessionID = orderField.SessionID;
	///�û��˲�Ʒ��Ϣ
	strcpy_s(data.UserProductInfo, sizeof(orderField.UserProductInfo), orderField.UserProductInfo);
	///״̬��Ϣ
	strcpy_s(data.StatusMsg, sizeof(orderField.StatusMsg), orderField.StatusMsg);
	///�û�ǿ����־
	data.UserForceClose = orderField.UserForceClose;
	///�����û�����
	strcpy_s(data.ActiveUserID, sizeof(orderField.ActiveUserID), orderField.ActiveUserID);
	///���͹�˾�������
	data.BrokerOrderSeq = orderField.BrokerOrderSeq;
	///��ر���
	strcpy_s(data.RelativeOrderSysID, sizeof(orderField.RelativeOrderSysID), orderField.RelativeOrderSysID);

//
//	strncpy(data.szAccount,orderField.InvestorID,sizeof(data.szAccount)-1);

}

void CopyTradeRecordField( PlatformStru_TradeInfo& data, const CThostFtdcTradeField& tradeField )
{
	strcpy(data.BrokerID, tradeField.BrokerID);
	strcpy(data.InvestorID, tradeField.InvestorID);
	strcpy(data.InstrumentID, tradeField.InstrumentID);
	strcpy(data.OrderRef, tradeField.OrderRef);
	strcpy(data.UserID, tradeField.UserID);
	strcpy(data.ExchangeID, tradeField.ExchangeID);
	strcpy(data.TradeID, tradeField.TradeID);
	data.Direction = tradeField.Direction;
	strcpy(data.OrderSysID, tradeField.OrderSysID);
	strcpy(data.ParticipantID, tradeField.ParticipantID);
	strcpy(data.ClientID, tradeField.ClientID);
	data.TradingRole = tradeField.TradingRole;
	strcpy(data.ExchangeInstID, tradeField.ExchangeInstID);
	data.OffsetFlag = tradeField.OffsetFlag;
	data.HedgeFlag = tradeField.HedgeFlag;
	data.Price = tradeField.Price;
	data.Volume = tradeField.Volume;
	strcpy(data.TradeDate, tradeField.TradeDate);
	strcpy(data.TradeTime, tradeField.TradeTime);
	data.TradeType = tradeField.TradeType;
	data.PriceSource = tradeField.PriceSource;
	strcpy(data.TraderID, tradeField.TraderID);
	strcpy(data.OrderLocalID, tradeField.OrderLocalID);
	strcpy(data.ClearingPartID, tradeField.ClearingPartID);
	strcpy(data.BusinessUnit, data.BusinessUnit);
	data.SequenceNo = tradeField.SequenceNo;
	strcpy(data.TradingDay, tradeField.TradingDay);
	data.SettlementID = tradeField.SettlementID;
	data.BrokerOrderSeq = tradeField.BrokerOrderSeq;
	data.TradeSource = tradeField.TradeSource;

	strncpy(data.Account,tradeField.InvestorID,sizeof(data.Account)-1);
}
void CopyCTPTradeRecordField( CThostFtdcTradeField& data,  PlatformStru_TradeInfo& tradeField )
{
	strcpy(data.BrokerID, tradeField.BrokerID);
	strcpy(data.InvestorID, tradeField.InvestorID);
	strcpy(data.InstrumentID, tradeField.InstrumentID);
	strcpy(data.OrderRef, tradeField.OrderRef);
	strcpy(data.UserID, tradeField.UserID);
	strcpy(data.ExchangeID, tradeField.ExchangeID);
	strcpy(data.TradeID, tradeField.TradeID);
	data.Direction = tradeField.Direction;
	strcpy(data.OrderSysID, tradeField.OrderSysID);
	strcpy(data.ParticipantID, tradeField.ParticipantID);
	strcpy(data.ClientID, tradeField.ClientID);
	data.TradingRole = tradeField.TradingRole;
	strcpy(data.ExchangeInstID, tradeField.ExchangeInstID);
	data.OffsetFlag = tradeField.OffsetFlag;
	data.HedgeFlag = tradeField.HedgeFlag;
	data.Price = tradeField.Price;
	data.Volume = tradeField.Volume;
	strcpy(data.TradeDate, tradeField.TradeDate);
	strcpy(data.TradeTime, tradeField.TradeTime);
	data.TradeType = tradeField.TradeType;
	data.PriceSource = tradeField.PriceSource;
	strcpy(data.TraderID, tradeField.TraderID);
	strcpy(data.OrderLocalID, tradeField.OrderLocalID);
	strcpy(data.ClearingPartID, tradeField.ClearingPartID);
	strcpy(data.BusinessUnit, data.BusinessUnit);
	data.SequenceNo = tradeField.SequenceNo;
	strcpy(data.TradingDay, tradeField.TradingDay);
	data.SettlementID = tradeField.SettlementID;
	data.BrokerOrderSeq = tradeField.BrokerOrderSeq;
	data.TradeSource = tradeField.TradeSource;

	//strncpy(data.szAccount,tradeField.InvestorID,sizeof(data.szAccount)-1);
}