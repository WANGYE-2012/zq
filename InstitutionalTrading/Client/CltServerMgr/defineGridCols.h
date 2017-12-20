#pragma once

namespace CltServerMgr {

	enum {
		conGridNone,					// 0, ������
		conGridQueryHistoryOrder,		// ʵʱ��ѯ����grid
		conGridQueryHistoryTrade,		// ʵʱ��ѯ�ɽ�grid 
		conGridQueryHistoryPositionDetail, 	// ʵʱ��ѯ�ֲ���ϸgrid
		conGridQueryHistoryPosition, 	// ʵʱ��ѯ�ֲ�grid
		conGridQueryHistoryFund,		// ʵʱ��ѯ�ʺ��ʽ�grid
		conGridQueryHistoryFundInOut,	// ʵʱ��ѯ�ʺ��ʽ����grid
		conGridQueryUserSendMsg,		// ��ѯ�û�������Ϣgrid
		conGridQueryUserRecvMsg,		// ��ѯ�û�������Ϣgrid 
	};

	// ��ѯ��ʷ����grid�ж���
	enum {
		// 0, ������
		conCol_HQO_None,
		///������
		conCol_HQO_TradingDay,
		///Ͷ���ߴ���
		conCol_HQO_InvestorID,
		///��Լ����
		conCol_HQO_InstrumentID,
		///��������
		conCol_HQO_OrderRef,
		///�����۸�����
		conCol_HQO_OrderPriceType,
		///��������
		conCol_HQO_Direction,
		///��Ͽ�ƽ��־
		conCol_HQO_CombOffsetFlag,
		///���Ͷ���ױ���־
		conCol_HQO_CombHedgeFlag,
		///�۸�
		conCol_HQO_LimitPrice,
		///����
		conCol_HQO_VolumeTotalOriginal,
		///��Ч������
		conCol_HQO_TimeCondition,
		///GTD����
		conCol_HQO_GTDDate,
		///�ɽ�������
		conCol_HQO_VolumeCondition,
		///��С�ɽ���
		conCol_HQO_MinVolume,
		///��������
		conCol_HQO_ContingentCondition,
		///ֹ���
		conCol_HQO_StopPrice,
		///ǿƽԭ��
		conCol_HQO_ForceCloseReason,
		///�Զ������־
		conCol_HQO_IsAutoSuspend,
		///ҵ��Ԫ
		conCol_HQO_BusinessUnit,
		///������
		conCol_HQO_RequestID,
		///���ر������
		conCol_HQO_OrderLocalID,
		///����������
		conCol_HQO_ExchangeID,
		///��Ա����
		conCol_HQO_ParticipantID,
		///�ͻ�����
		conCol_HQO_ClientID,
		///��Լ�ڽ������Ĵ���
		conCol_HQO_ExchangeInstID,
		///����������Ա����
		conCol_HQO_TraderID,
		///��װ���
		conCol_HQO_InstallID,
		///�����ύ״̬
		conCol_HQO_OrderSubmitStatus,
		///������ʾ���
		conCol_HQO_NotifySequence,
		///�������
		conCol_HQO_OrderSysID,
		///������Դ
		conCol_HQO_OrderSource,
		///����״̬
		conCol_HQO_OrderStatus,
		///��������
		conCol_HQO_OrderType,
		///��ɽ�����
		conCol_HQO_VolumeTraded,
		///ʣ������
		conCol_HQO_VolumeTotal,
		///��������
		conCol_HQO_InsertDate,
		///ί��ʱ��
		conCol_HQO_InsertTime,
		///����ʱ��
		conCol_HQO_ActiveTime,
		///����ʱ��
		conCol_HQO_SuspendTime,
		///����޸�ʱ��
		conCol_HQO_UpdateTime,
		///����ʱ��
		conCol_HQO_CancelTime,
		///����޸Ľ���������Ա����
		conCol_HQO_ActiveTraderID,
		///���
		conCol_HQO_SequenceNo,
		///ǰ�ñ��
		conCol_HQO_FrontID,
		///�Ự���
		conCol_HQO_SessionID,
		///�û��˲�Ʒ��Ϣ
		conCol_HQO_UserProductInfo,
		///״̬��Ϣ
		conCol_HQO_StatusMsg,
		///�û�ǿ����־
		conCol_HQO_UserForceClose,
		///�����û�����
		conCol_HQO_ActiveUserID,
		///���͹�˾�������
		conCol_HQO_BrokerOrderSeq,
		///��ر���
		conCol_HQO_RelativeOrderSysID,
	};

	// ��ѯ��ʷ�ɽ�grid�ж��� 
	enum {
		// 0, ������
		conCol_HQT_None,
		///������
		conCol_HQT_TradingDay,
		///Ͷ���ߴ���
		conCol_HQT_InvestorID,
		///��Լ����
		conCol_HQT_InstrumentID,
		///��������
		conCol_HQT_OrderRef,
		///����������
		conCol_HQT_ExchangeID,
		///�ɽ����
		conCol_HQT_TradeID,
		///��������
		conCol_HQT_Direction,
		///�������
		conCol_HQT_OrderSysID,
		///��ƽ��־
		conCol_HQT_OffsetFlag,
		///Ͷ���ױ���־
		conCol_HQT_HedgeFlag,
		///�۸�
		conCol_HQT_Price,
		///����
		conCol_HQT_Volume,
		///�ɽ�ʱ��
		conCol_HQT_TradeDate,
		///�ɽ�ʱ��
		conCol_HQT_TradeTime,
		///�ɽ�����
		conCol_HQT_TradeType,
		///�ɽ�����Դ
		conCol_HQT_PriceSource,
		///���ر������
		conCol_HQT_OrderLocalID,
		///���
		conCol_HQT_SequenceNo,
		///�ɽ���Դ
		conCol_HQT_TradeSource,
	};

	// ��ѯ��ʷ�ֲ���ϸgrid�ж���
	enum {
		// 0, ������
		conCol_HQPD_None,
		///������
		conCol_HQPD_TradingDay,
		///Ͷ���ߴ���
		conCol_HQPD_InvestorID,
		///��Լ����
		conCol_HQPD_InstrumentID,
		///Ͷ���ױ���־
		conCol_HQPD_HedgeFlag,
		///����
		conCol_HQPD_Direction,
		///��������
		conCol_HQPD_OpenDate,
		///�ɽ����
		conCol_HQPD_TradeID,
		///���� ���̾���λ
		conCol_HQPD_Volume,
		///���ּ�
		conCol_HQPD_OpenPrice,
		///�ɽ�����
		conCol_HQPD_TradeType,
		/////��Ϻ�Լ����
		//conCol_HQPD_CombInstrumentID,
		///����������
		conCol_HQPD_ExchangeID,
		///���ն���ƽ��ӯ��
		conCol_HQPD_CloseProfitByDate,
		///��ʶԳ�ƽ��ӯ��
		conCol_HQPD_CloseProfitByTrade,
		///���ն��гֲ�ӯ��
		conCol_HQPD_PositionProfitByDate,
		///��ʶԳ�ֲ�ӯ��
		conCol_HQPD_PositionProfitByTrade,
		///Ͷ���߱�֤��
		conCol_HQPD_Margin,
		///��֤����
		conCol_HQPD_MarginRateByMoney,
		///��֤����(������)
		conCol_HQPD_MarginRateByVolume,
		///������
		conCol_HQPD_LastSettlementPrice,
		///�����
		conCol_HQPD_SettlementPrice,
		///ƽ����
		conCol_HQPD_CloseVolume,
		///ƽ�ֽ��
		conCol_HQPD_CloseAmount,
	};

	// ��ѯ��ʷ�ֲ�grid�ж���
	enum {
		// 0, ������
		conCol_HQP_None,
		///������
		conCol_HQP_TradingDay,
		///�˻���
		conCol_HQP_InvestorID,
		///��Լ����
		conCol_HQP_InstrumentID,
		///�ֲֶ�շ���
		conCol_HQP_PosiDirection,
		///Ͷ���ױ���־
		conCol_HQP_HedgeFlag,
		///���ճֲ�
		conCol_HQP_YdPosition,
		///���ճֲ�
		conCol_HQP_Position,
		///������
		conCol_HQP_OpenVolume,
		///ƽ����
		conCol_HQP_CloseVolume,
		///���ֽ��
		conCol_HQP_OpenAmount,
		///ƽ�ֽ��
		conCol_HQP_CloseAmount,
		///�ֲֳɱ�
		conCol_HQP_PositionCost,
		///�ϴ�ռ�õı�֤��
		conCol_HQP_PreMargin,
		///ռ�õı�֤��
		conCol_HQP_UseMargin,
		///�ʽ���
		conCol_HQP_CashIn,
		///������
		conCol_HQP_Commission,
		///ƽ��ӯ��
		conCol_HQP_CloseProfit,
		///�ֲ�ӯ��
		conCol_HQP_PositionProfit,
		///�ϴν����
		conCol_HQP_PreSettlementPrice,
		///���ν����
		conCol_HQP_SettlementPrice,
		///���ֳɱ�
		conCol_HQP_OpenCost,
		/////��ϳɽ��γɵĳֲ�
		//conCol_HQP_CombPosition,
		/////��϶�ͷ����
		//conCol_HQP_CombLongFrozen,
		/////��Ͽ�ͷ����
		//conCol_HQP_CombShortFrozen,
		///���ն���ƽ��ӯ��
		conCol_HQP_CloseProfitByDate,
		///��ʶԳ�ƽ��ӯ��
		conCol_HQP_CloseProfitByTrade,
		///���ճֲ�
		conCol_HQP_TodayPosition,
		///��֤����
		conCol_HQP_MarginRateByMoney,
		///��֤����(������)
		conCol_HQP_MarginRateByVolume,
	};

	// ��ѯ��ʷ�ʺ��ʽ�grid�ж���
	enum {
		// 0, ������
		conCol_HQA_None,
		///������
		conCol_HQA_TradingDay,
		///�˻���
		conCol_HQA_InvestorID,
		/////�˻�����
		//conCol_HQA_InvestorName,
		///�ڳ����
		conCol_HQA_StaticProfit,
		///��ĩ���
		conCol_HQA_DynamicProfit,
		///�����
		conCol_HQA_Within,
		///������
		conCol_HQA_Without,
		///ռ�ñ�֤��
		conCol_HQA_CurrMargin,
		///�����ʽ�
		conCol_HQA_Available,
		///ƽ��ӯ��
		conCol_HQA_CloseProfit,
		///�ֲ�ӯ��
		conCol_HQA_PositionProfit,
		///������
		conCol_HQA_Commission,
		///���ն�
		conCol_HQA_RiskDegree,
	};

	// ��ѯ��ʷ�ʺ��ʽ����grid�ж���
	enum {
		// 0, ������
		conCol_HQFIN_None,
		///��������
		conCol_HQFIN_OperateDate,
		///�˻���
		conCol_HQFIN_InvestorID,
		/////�˻�����
		//conCol_HQFIN_InvestorName,
		///���������
		conCol_HQFIN_InOrOut,
		///������
		conCol_HQFIN_Volume,
		///����Ա
		conCol_HQFIN_OperateUser,
		///����ʱ��
		conCol_HQFIN_OperateTime,
		///��ע
		conCol_HQFIN_Remark,
	};

	// ��ѯ�û�������Ϣgrid�ж���
	enum {
		// 0, ������
		conCol_USM_None,
		///��ϢID
		conCol_USM_MSGID,
		///����
		conCol_USM_Title,
		///����
		conCol_USM_MSG,
		///ʱ��
		conCol_USM_CreateTime,
		///�ѽ�����
		conCol_USM_HasRecvUser,
		///δ������
		conCol_USM_NotRecvUser,
	};

	// ��ѯ�û�������Ϣgrid�ж���
	enum {
		// 0, ������
		conCol_URM_None,
		///��ϢID
		conCol_URM_TARGETMSGID,
		///����
		conCol_URM_Title,
		///����
		conCol_URM_MSG,
		///ʱ��
		conCol_URM_RecvTime,
		///������
		conCol_URM_SendUser,
	};
}
