#ifndef _CONFIGERMANAGER_H_
#define _CONFIGERMANAGER_H_

#ifdef WIN32
#pragma managed(push,off)
#endif 

class GlobalConfigManager
{
public:
	static void Load();

	static bool IsCalculate_OnRtnTrade();

    //��ȡ�ɽ��ر����Ƿ񴥷��Ĳ���
    static bool IsReq_OnRtnTrade(void);

    //��ȡ�ɽ��ر��󴥷���ѯ���ӳ�����
    static int GetDelayBefReq_OnRtnTrade(void);

    //��ȡ�ɽ�������ѯ�ֲ���ϸ������¼������ֵ���������ֵ�Ͳ��Զ���ѯ�ֲ���ϸ
    static int GetMaxRecordReqPositionDetail_OnRtnTrade(void);

    //�б���Զ�����ʱ������������������������ֹͣ�Զ���������Ӱ���ٶ�
    static int GetMaxItemCountForAutoResort_ListCtrl(void);

    //�Ƿ�����CifcoServicesHall����
	static bool IsCifcoServicesHall_ExtraFunction();
	

    //��״̬Ϊδ֪�ı���(���ֹ���˵�)���Ƿ�ִ���ʽ𶳽�
    static bool IsFreezeOrderUnknownStatus();

    // ��׼�µ����
	static bool IsOrderType_UIOT_PANE_STANDARD();
	// �����µ����
    static bool IsOrderType_UIOT_PANE_BATCH();
	// �������µ����
    static bool IsOrderType_UIOT_PANE_CONDITION();
	// Fak/fok�µ����
    static bool IsOrderType_UIOT_PANE_FAKFOK();
	// ӯ���µ����
    static bool IsOrderType_UIOT_PANE_PROFIT();
	// ֹͣ���µ����
    static bool IsOrderType_UIOT_PANE_STOPLIMIT();
	// ģ���м۹���
    static bool IsOrderType_UIOT_CMD_SIMULATEMARKET();
	// Ԥ�񵥰�ť
    static bool IsOrderType_UIOT_CMD_PARKED();
	// pats�ĵ��˵���
    static bool IsOrderType_UIOT_CMD_REPLACEORDER();
	// �µ��屣ֵ����
    static bool IsOrderType_UIOT_ITEM_HEDGE();
	// �µ��忪ƽ��
    static bool IsOrderType_UIOT_ITEM_OPENCLOSE();
	// �µ����ʺ���
    static bool IsOrderType_UIOT_ITEM_ACCOUNT();
	// ����������������
    static bool IsOrderType_UIOT_ITEM_CONDITION_LOCAL();
	// ������������������
    static bool IsOrderType_UIOT_ITEM_CONDITION_SERVER();
	// ����Ԥ�񵥿�����
    static bool IsOrderType_UIOT_ITEM_PARKED_LOCAL();
	// ������Ԥ�񵥿�����
    static bool IsOrderType_UIOT_ITEM_PARKED_SERVER();
	// ����Ԥ�񵥿�����
    static bool IsOrderType_UIOT_ITEM_PROFIT_LOCAL();
	// ������Ԥ�񵥿�����
    static bool IsOrderType_UIOT_ITEM_PROFIT_SERVER();

	// �Ƿ񱣴淢�͡����ձ������ر��ı���ʱ��
    static bool HasSaveOrderTime();

    //�Ƿ�֧��ָ����Ʒ����
    enum eProductClassType
    {
        PC_All,         ///ȫ��
        PC_Futures,     ///��Ȩ
        PC_Options,     ///��Ȩ
        PC_Combination, ///���
        PC_Spot,        ///����
        PC_EFP,         ///��ת��
    };
    static bool IsSupportProductClassType(eProductClassType ProductClassType);


private:
    //�Ƿ��Ѿ�����
    static bool m_bLoad;

    //�ɽ��ر�ʱ�Ƿ���㲢���³ֲ֡��ֲ���ϸ���ʽ�
	static bool m_bCalculate_OnRtnTrade;

    //�ɽ��ر����Ƿ񴥷��ֲ֡��ֲ���ϸ���ʽ�Ĳ�ѯ
    static bool m_bReq_OnRtnTrade;

    //�ɽ��ر����ӳٲ�ѯ�ֲ֡��ֲ���ϸ���ʽ��ʱ�䡣��λΪ��
    static int m_nDelayBefReq_OnRtnTrade;

    //�ɽ�������ѯ�ֲ���ϸ������¼������ֵ���������ֵ�Ͳ��Զ���ѯ�ֲ���ϸ
    static int m_nMaxRecordReqPositionDetail_OnRtnTrade;


    //�б���Զ�����ʱ������������������������ֹͣ�Զ���������Ӱ���ٶ�
    static int m_nMaxItemCountForAutoResort_ListCtrl;

    //�Ƿ�����CifcoServicesHall����
    static bool m_bCifcoServicesHall_ExtraFunction;

public:
    //�Ƿ�ʹ����ϵ�����
    static bool m_bUseComb;

	//��ϵ���֤���Żݲ���, 0-���Żݣ�1-��һ���Żݣ�2-�ڶ����Żݣ�3-����������и�����ȡ��4-���߱�֤����ȡ(����)�����ݺ�Լ��Ϣ�е�MaxMarginSideAlgorithm
    //ע�⣬ѡȡ���ʱ������������֤�����
	static int m_DiscountCZCE;
	static int m_DiscountDCE;
	static int m_DiscountSHFE;
	static int m_DiscountCFFEX;

    //֣���������Ƿ�֤���Żݡ�֣�������������ͨ�ֲ֣���ͬ��Լ�෴����ĳֲ֣�ֻ�յ��߱�֤��
    static bool m_LockPositionDiscountCZCE;

    //ƽ��˳��: 0: ͳһ�ȿ���ƽ��1: �ȵ�һ������ȿ���ƽ
    static int m_UnwindPrioritiesCZCE;
    static int m_UnwindPrioritiesDCE;
    static int m_UnwindPrioritiesSHFE;
    static int m_UnwindPrioritiesCFFEX;
    //�����ֱ�֤��ʹ�õļ۸�: 0: ���ɽ��ۼ���(ȱʡ)��1: �������ۼ���
    static int m_PriceForMarginOfTodayPositionCZCE;
    static int m_PriceForMarginOfTodayPositionDCE;
    static int m_PriceForMarginOfTodayPositionSHFE;
    static int m_PriceForMarginOfTodayPositionCFFEX;

	//�Ƿ���ݵ�����������������顣1���ɣ�0������
	static bool m_bCalcuCombQuotOnLegQuot_LastPrice;            //�Ƿ��Զ��������¼�
	static bool m_bCalcuCombQuotOnLegQuot_Volume;               //�Ƿ��Զ���������
	static bool m_bCalcuCombQuotOnLegQuot_AskBid;               //�Ƿ��Զ����������ۺ�������
	static bool m_bCalcuCombQuotOnLegQuot_PreSettlementPrice;   //�Ƿ��Զ�����������
	static bool m_bCalcuCombQuotOnLegQuot_PreClosePrice;        //�Ƿ��Զ����������̼�
	static bool m_bCalcuCombQuotOnLegQuot_LimitPrice;           //�Ƿ��Զ������ǵ�ͣ��

private:
    //��״̬Ϊδ֪�ı���(���ֹ���˵�)���Ƿ�ִ���ʽ𶳽�
    static bool m_bFreezeOrderUnknownStatus;


    // ��׼�µ����
	static bool m_bOrderType_UIOT_PANE_STANDARD;
	// �����µ����
    static bool m_bOrderType_UIOT_PANE_BATCH;
	// �������µ����
    static bool m_bOrderType_UIOT_PANE_CONDITION;
	// Fak/fok�µ����
    static bool m_bOrderType_UIOT_PANE_FAKFOK;
	// ӯ���µ����
    static bool m_bOrderType_UIOT_PANE_PROFIT;
	// ֹͣ���µ����
    static bool m_bOrderType_UIOT_PANE_STOPLIMIT;
	// ģ���м۹���
    static bool m_bOrderType_UIOT_CMD_SIMULATEMARKET;
	// Ԥ�񵥰�ť
    static bool m_bOrderType_UIOT_CMD_PARKED;
	// pats�ĵ��˵���
    static bool m_bOrderType_UIOT_CMD_REPLACEORDER;
	// �µ��屣ֵ����
    static bool m_bOrderType_UIOT_ITEM_HEDGE;
	// �µ��忪ƽ��
    static bool m_bOrderType_UIOT_ITEM_OPENCLOSE;
	// �µ����ʺ���
    static bool m_bOrderType_UIOT_ITEM_ACCOUNT;
	// ����������������
    static bool m_bOrderType_UIOT_ITEM_CONDITION_LOCAL;
	// ������������������
    static bool m_bOrderType_UIOT_ITEM_CONDITION_SERVER;
	// ����Ԥ�񵥿�����
    static bool m_bOrderType_UIOT_ITEM_PARKED_LOCAL;
	// ������Ԥ�񵥿�����
    static bool m_bOrderType_UIOT_ITEM_PARKED_SERVER;
	// ����Ԥ�񵥿�����
    static bool m_bOrderType_UIOT_ITEM_PROFIT_LOCAL;
	// ������Ԥ�񵥿�����
    static bool m_bOrderType_UIOT_ITEM_PROFIT_SERVER;

	// �Ƿ���ڷ��͡����ձ������ر��ı���ʱ��
    static bool m_bHasSaveOrderTime;

    ////�Ƿ�֧�ֲ�Ʒ����
    static bool m_bSupport_PC_All;
    static bool m_bSupport_PC_Futures;
    static bool m_bSupport_PC_Options;
    static bool m_bSupport_PC_Combination;
    static bool m_bSupport_PC_Spot;
    static bool m_bSupport_PC_EFP;

public:
    //ϵͳ�������������Լ������ȱʡΪ20
    static int MaxSubscribeCount;
    //����������������Լ������MaxSubscribeCountInPlugins<=MaxSubscribeCount��ȱʡΪ10
    static int MaxSubscribeCountInPlugins;

    //����������ƽ��ָ���֧�֣�0:��֧��(��ctp���ܻ�ת����ƽ��)��1:֧��
    static int CloseTodaySupport_SHFE;
    static int CloseTodaySupport_CZCE;
    static int CloseTodaySupport_DCE;
    static int CloseTodaySupport_CFFEX;

    //����������ƽ��ָ���֧�֣�0:��֧�֣�1:֧�֣�����ƽ��ֺͽ�֣�2:֧�֣�ֻ��ƽ���
    static int CloseSupport_SHFE;
    static int CloseSupport_CZCE;
    static int CloseSupport_DCE;
    static int CloseSupport_CFFEX;

    //���˺Ų���
public:
    //�Ƿ��Ƕ��˺������
    static bool m_bNetVersion_MultiAccount;
    //�Ƿ��Ƕ��˺ŵ�����
    static bool m_bLocalVersion_MultiAccount;
    //���˺��Ƿ������µ�
    static bool m_bDisorderWhenInsertOrder_MultiAccount;

    //�Ƿ������Զ������˻��������ԡ�1����0������
	static bool m_bAutoDeriveOrderByFirstAccount_MultiAccount;
	//�Զ������˻�����ʱʹ�õļ۸�(��δ��)��0�����ۣ�1���¼ۣ�2�ǵ�ͣ��ۣ�3�м�
	static int m_UsePrice_AutoDeriveOrderByFirstAccount_MultiAccount;
	//�Ƿ��Զ���ֲ����򿪲�
	static bool m_AutoBreak_AutoDeriveOrderByFirstAccount_MultiAccount;

public:
    //�Ƿ��¼ҵ�����businessdata.rec�У����ڲ��Իط�
    static bool m_Test_bSaveBusinessData;
    //�Ƿ���в��Իطţ��ط�businessdata.rec�е�ҵ���
    static bool m_Test_bPlayBusinessData;
    //�Ƿ��¼�߳�ID���߳����Ƶ�threadid.log�ļ���
    static bool m_Test_bLogThreadID;

public:
    //����ģʽ��1: ���ڻ���2: �ڻ�+��Ȩ��3: ��Ȩ��������ڻ���Ĭ��Ϊ1
    static int m_RunMode;
    //��Ȩ���Ƿ�֧�ֽ���ѯ��֪ͨ�Ĺ��ܣ�Ĭ�ϲ�֧��
    static bool m_CanReceiveForQuote;

public:
    //����н���۵Ļ����Ƿ�ʹ�ý���ۼ���ӯ���ͱ�֤��
    static bool m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible;

};


#ifdef WIN32
#pragma managed(pop)
#endif 

#endif


