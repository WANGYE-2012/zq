#ifndef _CONFIGERMANAGER_H_
#define _CONFIGERMANAGER_H_

class GlobalConfigManager
{
public:
	static void Load();

    static std::string GetstrValueByTitle(void* root,const char* DefValue,
                                            const char* pTitle1,
                                            const char* pTitle2=NULL,
                                            const char* pTitle3=NULL,
                                            const char* pTitle4=NULL);
    static int GetnValueByTitle(void* proot,const int DefValue,
                                            const char* pTitle1,
                                            const char* pTitle2=NULL,
                                            const char* pTitle3=NULL,
                                            const char* pTitle4=NULL);


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

    //���˺��Ƿ������µ�
    static bool IsDisorderWhenInsertOrder_MultiAccount();


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

    //���˺��Ƿ������µ�
    static bool m_bDisorderWhenInsertOrder;

};

#endif


