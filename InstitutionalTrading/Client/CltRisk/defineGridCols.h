#pragma once

namespace CltRisk {

	enum {
		conGridNone,					// 0, ������
		conGridQueryUserSendMsg,		// ��ѯ�û�������Ϣgrid
		conGridQueryUserRecvMsg,		// ��ѯ�û�������Ϣgrid 
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

    // �������grid�ж���
    enum {
        // 0, ������
        conCol_DepthMarket_None,
        ///��ԼID
        conCol_DepthMarket_InstrumentID,
        ///���¼�
        conCol_DepthMarket_LastPrice,
        ///�ǵ�
        conCol_DepthMarket_Mixed,
        ///�ǵ�����
        conCol_DepthMarket_MixedRange,
        ///���
        conCol_DepthMarket_BidPrice1,
        ///����
        conCol_DepthMarket_BidVolume1,
        // ����
        conCol_DepthMarket_AskPrice1,
        ///����
        conCol_DepthMarket_AskVolume1,
        ///�ɽ���
        conCol_DepthMarket_Volume,
        ///�ֲ���
        conCol_DepthMarket_OpenInterest,
        ///����
        conCol_DepthMarket_CurVolume,
        ///����
        conCol_DepthMarket_OpenPrice,
        ///�����
        conCol_DepthMarket_SettlementPrice,
        // ��߼�
        conCol_DepthMarket_HighestPrice,
        ///��ͼ�
        conCol_DepthMarket_LowestPrice,
        ///��ͣ��
        conCol_DepthMarket_UpperLimitPrice,
        ///��ͣ��
        conCol_DepthMarket_LowerLimitPrice,
        ///����ʱ��
        conCol_DepthMarket_UpdateTime,        
    };

    // �����¼�grid�ж���
    enum {
        // 0, ������
        conCol_RiskEvent_None,
        ///�����¼�ID
        conCol_RiskEvent_RiskEventID,
        ///����ԱID
        conCol_RiskEvent_TradeInvestorID,
        ///����Ա����
        conCol_RiskEvent_TraderName,
        ///����ָ������
        conCol_RiskEvent_IndicatorName,
        ///��������
        conCol_RiskEvent_RiskDesc,
        ///����ʱ��
        conCol_RiskEvent_EventTime,
        // �����¼�״̬
        conCol_RiskEvent_EventStatus,       
    };

    // �˻����ʽ�grid�ж���
    enum {
        // 0, ������
        conCol_TradingAccount_None,
        ///����Ա
        conCol_TradingAccount_Account,
        ///��������
        conCol_TradingAccount_LimitTade,
        ///�ֶ����
        conCol_TradingAccount_ManuVerify,
        ///��¼״̬
        conCol_TradingAccount_LoginStatus,
        ///���ն�(%)
        conCol_TradingAccount_RiskDegree,
        ///��̬Ȩ��
        conCol_TradingAccount_DynamicProfit,
        // �ֲ�ӯ��
        conCol_TradingAccount_PositionProfit,       
        ///ƽ��ӯ��
        conCol_TradingAccount_CloseProfit,
        ///ռ�ñ�֤��
        conCol_TradingAccount_CurrMargin,
        ///������
        conCol_TradingAccount_Commission,
        ///���ᱣ֤��
        conCol_TradingAccount_FrozenMargin,
        // ����������
        conCol_TradingAccount_FrozenCommission,   
    };

    // ί�е�grid�ж���
    enum {
        // 0, ������
        conCol_AllOrder_None,
        ///����Ա
        conCol_AllOrder_InvestorID,
        ///�������
        conCol_AllOrder_OrderSysID,
        ///��Լ
        conCol_AllOrder_InstrumentID,
        ///����
        conCol_AllOrder_Direction,
        ///��ƽ
        conCol_AllOrder_CombOffsetFlag,
        ///�ҵ�״̬
        conCol_AllOrder_OrderStatus,
        // �����۸�
        conCol_AllOrder_LimitPrice,
        ///��������
        conCol_AllOrder_VolumeTotalOriginal,
        ///δ�ɽ�����
        conCol_AllOrder_VolumeTotal,
        ///�ɽ�����
        conCol_AllOrder_Volume,
        ///��ϸ״̬
        conCol_AllOrder_StatusMsg,
        ///����ʱ��
        conCol_AllOrder_InsertTime,
        ///���ᱣ֤��
        conCol_AllOrder_freezeMargin,
        // ����������
        conCol_AllOrder_troubleMoney,
        ///Ͷ��
        conCol_AllOrder_HedgeFlag,
        ///������
        conCol_AllOrder_ExchangeID,
        ///ǰ�û�
        conCol_AllOrder_FrontID,
        ///�Ự��
        conCol_AllOrder_SessionID,   
        ///��������
        conCol_AllOrder_OrderRef,
        ///״̬
        conCol_AllOrder_ExStatus,  
    };

    // �ɽ���grid�ж���
    enum {
        // 0, ������
        conCol_Trade_None,
        ///����Ա
        conCol_Trade_InvestorID,
        ///��Լ
        conCol_Trade_InstrumentID,
        ///����
        conCol_Trade_Direction,
        ///��ƽ
        conCol_Trade_OffsetFlag,
        ///�ɽ��۸�
        conCol_Trade_Price,
        ///�ɽ�����
        conCol_Trade_Volume,
        // ������
        conCol_Trade_TradeCommission,
        ///�ɽ�ʱ��
        conCol_Trade_TradeTime,
        ///�ɽ�����
        conCol_Trade_TradeType,
        ///Ͷ��
        conCol_Trade_HedgeFlag,
        ///�ɽ����
        conCol_Trade_TradeID,
        ///�������
        conCol_Trade_OrderSysID,
        ///������
        conCol_Trade_ExchangeID, 
    };

    // �ֲ�grid�ж���
    enum {
        // 0, ������
        conCol_Position_None,
        ///����Ա
        conCol_Position_InvestorID,
        ///��Լ
        conCol_Position_InstrumentID,
        ///����
        conCol_Position_Direction,
        ///Ͷ��
        conCol_Position_HedgeFlag,
        ///�ֲܳ�
        conCol_Position_Position,
        ///��ƽ��
        conCol_Position_CanCloseVolume,
        // �ּ�
        conCol_Position_LastPrice,
        ///�ֲ־���
        conCol_Position_AveragePositionPrice,
        ///���־���
        conCol_Position_AverageOpenPrice,
        ///�ֲ�ӯ��
        conCol_Position_PositionProfit,
        ///����ӯ��
        conCol_Position_PositionProfitByTrade,
        ///������ӯ��
        conCol_Position_TotalPositionProfitByDate,
        ///ռ�ñ�֤��
        conCol_Position_UseMargin,
        // �ܶ��
        conCol_Position_MultiBuyPosition,
        ///�ܿղ�
        conCol_Position_MultiSellPosition,
        ///����
        conCol_Position_TodayBuyPosition,
        ///��ղ�
        conCol_Position_TodaySellPosition,
        ///���
        conCol_Position_YesterdayPosition,   
        ///���
        conCol_Position_TodayPosition,
        ///�񿪲���
        conCol_Position_OpenVolume,  
        // ��ƽ����
        conCol_Position_CloseVolume,
        ///�ܿ�ƽ����
        conCol_Position_TotalCanCloseTodayVolume,
        ///�ܿ�ƽ����
        conCol_Position_TotalCanCloseYesterdayVolume,
        ///ƽ��ҵ���
        conCol_Position_CloseTodayOrderVolume,
        ///ƽ�ֹҵ���
        conCol_Position_CloseOrderVolume,
        ///��ƽ����
        conCol_Position_CanCloseTodayVolume,
        ///��ƽ����
        conCol_Position_CanCloseYesterdayVolume,
        // ������
        conCol_Position_ExchangeID, 
    };
}
