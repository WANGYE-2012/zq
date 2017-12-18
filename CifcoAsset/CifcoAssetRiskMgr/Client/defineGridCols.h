#pragma once

namespace Client {

	enum {
		conGridNone,					// 0, ������
		conGridRealTimeQueryOrder,		// ʵʱ��ѯ����grid
		conGridRealTimeQueryTrade,		// ʵʱ��ѯ�ɽ�grid 
		conGridRealTimeQueryPosition, 	// ʵʱ��ѯ�ֲ�grid
		conGridRealTimeQueryFund		// ʵʱ��ѯ�ʺ��ʽ�grid
	};

	// ʵʱ��ѯ����grid�ж���
	enum {
		// 0, ������
		conCol_RTQO_None,
		///Ͷ���ߴ���
		conCol_RTQO_InvestorID,
		// �˺������ʹ�
		conCol_RTQO_AssertMgmtOrgan,
		// �˺�������Ʋ�Ʒ
		conCol_RTQO_FinanProduct,
		/////����Ψһ�����к�
		//conCol_RTQO_UniqSequenceNo,
		///���͹�˾����
		conCol_RTQO_BrokerID,
		///��Լ����
		conCol_RTQO_InstrumentID,
		///��������
		conCol_RTQO_OrderRef,
		///�û�����
		conCol_RTQO_UserID,
		///�����۸�����
		conCol_RTQO_OrderPriceType,
		///��������
		conCol_RTQO_Direction,
		///��Ͽ�ƽ��־
		conCol_RTQO_CombOffsetFlag,
		///���Ͷ���ױ���־
		conCol_RTQO_CombHedgeFlag,
		///�۸�
		conCol_RTQO_LimitPrice,
		///����
		conCol_RTQO_VolumeTotalOriginal,
		///��Ч������
		conCol_RTQO_TimeCondition,
		///GTD����
		conCol_RTQO_GTDDate,
		///�ɽ�������
		conCol_RTQO_VolumeCondition,
		///��С�ɽ���
		conCol_RTQO_MinVolume,
		///��������
		conCol_RTQO_ContingentCondition,
		///ֹ���
		conCol_RTQO_StopPrice,
		///ǿƽԭ��
		conCol_RTQO_ForceCloseReason,
		///�Զ������־
		conCol_RTQO_IsAutoSuspend,
		///ҵ��Ԫ
		conCol_RTQO_BusinessUnit,
		///������
		conCol_RTQO_RequestID,
		///���ر������
		conCol_RTQO_OrderLocalID,
		///����������
		conCol_RTQO_ExchangeID,
		///��Ա����
		conCol_RTQO_ParticipantID,
		///�ͻ�����
		conCol_RTQO_ClientID,
		///��Լ�ڽ������Ĵ���
		conCol_RTQO_ExchangeInstID,
		///����������Ա����
		conCol_RTQO_TraderID,
		///��װ���
		conCol_RTQO_InstallID,
		///�����ύ״̬
		conCol_RTQO_OrderSubmitStatus,
		///������ʾ���
		conCol_RTQO_NotifySequence,
		///������
		conCol_RTQO_TradingDay,
		///������
		conCol_RTQO_SettlementID,
		///�������
		conCol_RTQO_OrderSysID,
		///������Դ
		conCol_RTQO_OrderSource,
		///����״̬
		conCol_RTQO_OrderStatus,
		///��������
		conCol_RTQO_OrderType,
		///��ɽ�����
		conCol_RTQO_VolumeTraded,
		///ʣ������
		conCol_RTQO_VolumeTotal,
		///��������
		conCol_RTQO_InsertDate,
		///ί��ʱ��
		conCol_RTQO_InsertTime,
		///����ʱ��
		conCol_RTQO_ActiveTime,
		///����ʱ��
		conCol_RTQO_SuspendTime,
		///����޸�ʱ��
		conCol_RTQO_UpdateTime,
		///����ʱ��
		conCol_RTQO_CancelTime,
		///����޸Ľ���������Ա����
		conCol_RTQO_ActiveTraderID,
		///�����Ա���
		conCol_RTQO_ClearingPartID,
		///���
		conCol_RTQO_SequenceNo,
		///ǰ�ñ��
		conCol_RTQO_FrontID,
		///�Ự���
		conCol_RTQO_SessionID,
		///�û��˲�Ʒ��Ϣ
		conCol_RTQO_UserProductInfo,
		///״̬��Ϣ
		conCol_RTQO_StatusMsg,
		///�û�ǿ����־
		conCol_RTQO_UserForceClose,
		///�����û�����
		conCol_RTQO_ActiveUserID,
		///���͹�˾�������
		conCol_RTQO_BrokerOrderSeq,
		///��ر���
		conCol_RTQO_RelativeOrderSysID,
		///֣�����ɽ�����
		conCol_RTQO_ZCETotalTradedVolume,
	};

	// ʵʱ��ѯ�ɽ�grid�ж��� 
	enum {
		// 0, ������
		conCol_RTQT_None,
		///Ͷ���ߴ���
		conCol_RTQT_InvestorID,
		// �˺������ʹ�
		conCol_RTQT_AssertMgmtOrgan,
		// �˺�������Ʋ�Ʒ
		conCol_RTQT_FinanProduct,
		/////����Ψһ�����к�
		//conCol_RTQT_UniqSequenceNo,
		///���͹�˾����
		conCol_RTQT_BrokerID,
		///��Լ����
		conCol_RTQT_InstrumentID,
		///��������
		conCol_RTQT_OrderRef,
		///�û�����
		conCol_RTQT_UserID,
		///����������
		conCol_RTQT_ExchangeID,
		///�ɽ����
		conCol_RTQT_TradeID,
		///��������
		conCol_RTQT_Direction,
		///�������
		conCol_RTQT_OrderSysID,
		///��Ա����
		conCol_RTQT_ParticipantID,
		///�ͻ�����
		conCol_RTQT_ClientID,
		///���׽�ɫ
		conCol_RTQT_TradingRole,
		///��Լ�ڽ������Ĵ���
		conCol_RTQT_ExchangeInstID,
		///��ƽ��־
		conCol_RTQT_OffsetFlag,
		///Ͷ���ױ���־
		conCol_RTQT_HedgeFlag,
		///�۸�
		conCol_RTQT_Price,
		///����
		conCol_RTQT_Volume,
		///�ɽ�ʱ��
		conCol_RTQT_TradeDate,
		///�ɽ�ʱ��
		conCol_RTQT_TradeTime,
		///�ɽ�����
		conCol_RTQT_TradeType,
		///�ɽ�����Դ
		conCol_RTQT_PriceSource,
		///����������Ա����
		conCol_RTQT_TraderID,
		///���ر������
		conCol_RTQT_OrderLocalID,
		///�����Ա���
		conCol_RTQT_ClearingPartID,
		///ҵ��Ԫ
		conCol_RTQT_BusinessUnit,
		///���
		conCol_RTQT_SequenceNo,
		///������
		conCol_RTQT_TradingDay,
		///������
		conCol_RTQT_SettlementID,
		///���͹�˾�������
		conCol_RTQT_BrokerOrderSeq,
		///�ɽ���Դ
		conCol_RTQT_TradeSource,
	};

	// ʵʱ��ѯ�ֲ�grid�ж���
	enum {
		// 0, ������
		conCol_RTQP_None,
		///Ͷ���ߴ���
		conCol_RTQP_InvestorID,
		// �˺������ʹ�
		conCol_RTQP_AssertMgmtOrgan,
		// �˺�������Ʋ�Ʒ
		conCol_RTQP_FinanProduct,
		/////����Ψһ�����к�
		//conCol_RTQP_UniqSequenceNo,
		///���͹�˾����
		conCol_RTQP_BrokerID,
		///��Լ����
		conCol_RTQP_InstrumentID,
		///�ֲֶ�շ���
		conCol_RTQP_PosiDirection,
		///Ͷ���ױ���־
		conCol_RTQP_HedgeFlag,
		///�ֲ�����
		conCol_RTQP_PositionDate,
		///���ճֲ�
		conCol_RTQP_YdPosition,
		///���ճֲ�
		conCol_RTQP_Position,
		///��ͷ����
		conCol_RTQP_LongFrozen,
		///��ͷ����
		conCol_RTQP_ShortFrozen,
		///���ֶ�����
		conCol_RTQP_LongFrozenAmount,
		///���ֶ�����
		conCol_RTQP_ShortFrozenAmount,
		///������
		conCol_RTQP_OpenVolume,
		///ƽ����
		conCol_RTQP_CloseVolume,
		///���ֽ��
		conCol_RTQP_OpenAmount,
		///ƽ�ֽ��
		conCol_RTQP_CloseAmount,
		///�ֲֳɱ�
		conCol_RTQP_PositionCost,
		///�ϴ�ռ�õı�֤��
		conCol_RTQP_PreMargin,
		///ռ�õı�֤��
		conCol_RTQP_UseMargin,
		///����ı�֤��
		conCol_RTQP_FrozenMargin,
		///������ʽ�
		conCol_RTQP_FrozenCash,
		///�����������
		conCol_RTQP_FrozenCommission,
		///�ʽ���
		conCol_RTQP_CashIn,
		///������
		conCol_RTQP_Commission,
		///ƽ��ӯ��
		conCol_RTQP_CloseProfit,
		///�ֲ�ӯ��
		conCol_RTQP_PositionProfit,
		///�ϴν����
		conCol_RTQP_PreSettlementPrice,
		///���ν����
		conCol_RTQP_SettlementPrice,
		///������
		conCol_RTQP_TradingDay,
		///������
		conCol_RTQP_SettlementID,
		///���ֳɱ�
		conCol_RTQP_OpenCost,
		///��������֤��
		conCol_RTQP_ExchangeMargin,
		///��ϳɽ��γɵĳֲ�
		conCol_RTQP_CombPosition,
		///��϶�ͷ����
		conCol_RTQP_CombLongFrozen,
		///��Ͽ�ͷ����
		conCol_RTQP_CombShortFrozen,
		///���ն���ƽ��ӯ��
		conCol_RTQP_CloseProfitByDate,
		///��ʶԳ�ƽ��ӯ��
		conCol_RTQP_CloseProfitByTrade,
		///���ճֲ�
		conCol_RTQP_TodayPosition,
		///��֤����
		conCol_RTQP_MarginRateByMoney,
		///��֤����(������)
		conCol_RTQP_MarginRateByVolume,
	};

	// ʵʱ��ѯ�ʺ��ʽ�grid�ж���
	enum {
		// 0, ������
		conCol_RTQA_None,
		///Ͷ���ߴ���
		conCol_RTQA_InvestorID,
		// �˺������ʹ�
		conCol_RTQA_AssertMgmtOrgan,
		// �˺�������Ʋ�Ʒ
		conCol_RTQA_FinanProduct,
		///���͹�˾����
		conCol_RTQA_BrokerID,
		///�������
		conCol_RTQA_Withdraw,
		///��ǰ��֤���ܶ�
		conCol_RTQA_CurrMargin,
		///������
		conCol_RTQA_Commission,
		///ƽ��ӯ��
		conCol_RTQA_CloseProfit,
		///�ֲ�ӯ��
		conCol_RTQA_PositionProfit,
		///��Ȩ��
		conCol_RTQA_Balance,
		///�����ʽ�
		conCol_RTQA_Available,
		///��Ѻ���
		conCol_RTQA_Mortgage,
		///��������֤��
		conCol_RTQA_ExchangeMargin,
		///��ȡ�ʽ�
		conCol_RTQA_WithdrawQuota,
		///���ö��
		conCol_RTQA_Credit,
		///��֤�𶳽�
		conCol_RTQA_FrozenMargin,
		///�����Ѷ���
		conCol_RTQA_FrozenCommission,

		// �����������ʽ��ʺ���ص�����

		/////����Ψһ�����к�
		//conCol_RTQA_UniqSequenceNo,
		/////���͹�˾����
		//conCol_RTQA_BrokerID,
		///Ͷ�����ʺ�
		conCol_RTQA_AccountID,
		///�ϴ���Ѻ���
		conCol_RTQA_PreMortgage,
		///�ϴ����ö��
		conCol_RTQA_PreCredit,
		///�ϴδ���
		conCol_RTQA_PreDeposit,
		///�ϴ���Ȩ��
		conCol_RTQA_PreBalance,
		///�ϴ�ռ�õı�֤��
		conCol_RTQA_PreMargin,
		///����׼����
		conCol_RTQA_Reserve,
		///�ϴν�������֤��
		conCol_RTQA_PreExchMargin,
		///��ʷǿƽ����
		conCol_RTQA_ForceCloseStat,
		///Ͷ���߽��֤��
		conCol_RTQA_DeliveryMargin,
		///���������֤��
		conCol_RTQA_ExchangeDeliveryMargin,
		//����ֵ
		conCol_RTQA_NETVALUE,
	};

}
