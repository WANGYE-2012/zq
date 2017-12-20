#pragma once

#include "FileOpr.h"
#include "ConfigMgr.h"

#if 0
#define LOG_INFO(fmt, ...)  ;
#else
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","CltServerMgr", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif

//���ļ���¼�����ն��û��Ĳ���
#define  MAKE_ERROR_CODE(_base,_offset) ((int)( (int)(_base) << 16 ) + (int)(_offset))

//��̨�����ն�
#define USER_OPR_SYSTEM				0x0010   //ϵͳ����
#define USER_OPR_ACCOUNT			0x0020   //�˻�����
#define USER_OPR_MANAGE				0x0030   //�������
#define USER_OPR_STRATEGY			0x0040   //���Բ���
#define USER_OPR_PRIVILEGE			0x0050   //Ȩ�޲���
#define USER_OPR_SETTLEMENT			0x0060   //�������
#define USER_OPR_QUERY			    0x0070   //��ѯ����
#define USER_OPR_MESSAGE			0x0080   //��Ϣ����
#define USER_OPR_CONTROL			0x0090   //���Ʋ���

//ϵͳ����
#define USER_OPR_SYSTEM_EXIT    MAKE_ERROR_CODE(USER_OPR_SYSTEM,0x0001)  //�˳�����

//�˻�����
#define USER_OPR_ACCOUNT_SYSTEMUSER_ADD             MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0001)  //����һ��ϵͳ�û�
#define USER_OPR_ACCOUNT_SYSTEMUSER_MODIFY          MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0002)  //�޸�һ��ϵͳ�û�
#define USER_OPR_ACCOUNT_SYSTEMUSER_DEL             MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0003)  //ɾ��һ��ϵͳ�û�
#define USER_OPR_ACCOUNT_SYSTEMUSER_USE             MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0004)  //��/ͣ��һ��ϵͳ�û�
#define USER_OPR_ACCOUNT_SYSTEMUSER_INIT            MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0005)  //��ʼ��һ��ϵͳ�û�
#define USER_OPR_ACCOUNT_SYSTEMUSER_ASSIGNORG       MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0006)  //һ��ϵͳ�û�������֯����
#define USER_OPR_ACCOUNT_SYSTEMUSER_ASSIGNPRODUCT   MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0007)  //һ��ϵͳ�û�������Ʋ�Ʒ
#define USER_OPR_ACCOUNT_SYSTEMUSER_ASSIGNACCOUNT   MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0008)  //һ��ϵͳ�û�ָ��ί�н����˺�

#define USER_OPR_ACCOUNT_INOUTFUND_QUERY    MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0011)  //��ѯ�����
#define USER_OPR_ACCOUNT_INOUTFUND_IN       MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0012)  //��һ���˻����
#define USER_OPR_ACCOUNT_INOUTFUND_OUT      MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0013)  //��һ���˻�����

#define USER_OPR_ACCOUNT_ACCOUNT_ADD             MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0021)  //����һ��ί�н����˺�
#define USER_OPR_ACCOUNT_ACCOUNT_MODIFY          MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0022)  //�޸�һ��ί�н����˺�
#define USER_OPR_ACCOUNT_ACCOUNT_DEL             MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0023)  //ɾ��һ��ί�н����˺�
#define USER_OPR_ACCOUNT_ACCOUNT_LOGIN           MAKE_ERROR_CODE(USER_OPR_ACCOUNT,0x0024)  //��¼һ��ί�н����˺�
//�������
#define USER_OPR_MANAGE_PRODUCT_ADD         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0001)  //����һ����Ʋ�Ʒ
#define USER_OPR_MANAGE_PRODUCT_MODIFY      MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0002)  //�޸�һ����Ʋ�Ʒ
#define USER_OPR_MANAGE_PRODUCT_DEL         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0003)  //ɾ��һ����Ʋ�Ʒ
#define USER_OPR_MANAGE_PRODUCT_SAVE        MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0004)  //����һ����Ʋ�Ʒ�ͽ���Ա�Ĺ�ϵ

#define USER_OPR_MANAGE_BROKER_ADD         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0011)  //����һ�����͹�˾
#define USER_OPR_MANAGE_BROKER_MODIFY      MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0012)  //�޸�һ�����͹�˾
#define USER_OPR_MANAGE_BROKER_DEL         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0013)  //ɾ��һ�����͹�˾
#define USER_OPR_MANAGE_SERVERGROUP_ADD         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0021)  //����һ����������
#define USER_OPR_MANAGE_SERVERGROUP_MODIFY      MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0022)  //�޸�һ����������
#define USER_OPR_MANAGE_SERVERGROUP_DEL         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0023)  //ɾ��һ����������
#define USER_OPR_MANAGE_SERVERADDR_ADD         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0031)  //����һ�����������ַ
#define USER_OPR_MANAGE_SERVERADDR_MODIFY      MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0032)  //�޸�һ�����������ַ
#define USER_OPR_MANAGE_SERVERADDR_DEL         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0033)  //ɾ��һ�����������ַ
#define USER_OPR_MANAGE_QUOTSERVER_CONFIG    MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0041)   //�������������
#define USER_OPR_MANAGE_QUOTADDR_ADD         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0051)  //����һ�������ַ
#define USER_OPR_MANAGE_QUOTADDR_MODIFY      MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0052)  //�޸�һ�������ַ
#define USER_OPR_MANAGE_QUOTADDR_DEL         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0053)  //ɾ��һ�������ַ

#define USER_OPR_MANAGE_ORGANIZE_ADD         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0061)  //����һ����֯����
#define USER_OPR_MANAGE_ORGANIZE_MODIFY      MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0062)  //�޸�һ����֯����
#define USER_OPR_MANAGE_ORGANIZE_DEL         MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0063)  //ɾ��һ����֯����
#define USER_OPR_MANAGE_ORGANIZE_SAVE        MAKE_ERROR_CODE(USER_OPR_MANAGE,0x0064)  //����һ����֯�����ͽ���Ա�Ĺ�ϵ
//���Բ���
#define USER_OPR_STRATEGY_ADD         MAKE_ERROR_CODE(USER_OPR_STRATEGY,0x0001)  //����һ�����ײ���
#define USER_OPR_STRATEGY_MODIFY      MAKE_ERROR_CODE(USER_OPR_STRATEGY,0x0002)  //�޸�һ�����ײ���
#define USER_OPR_STRATEGY_DEL         MAKE_ERROR_CODE(USER_OPR_STRATEGY,0x0003)  //ɾ��һ�����ײ���
#define USER_OPR_STRATEGY_SAVE        MAKE_ERROR_CODE(USER_OPR_STRATEGY,0x0004)  //����һ�����ײ��Ժ���֯�����Ĺ�ϵ
//Ȩ�޲���
#define USER_OPR_PRIVILEGE_ADD         MAKE_ERROR_CODE(USER_OPR_PRIVILEGE,0x0001)  //����һ����ɫ
#define USER_OPR_PRIVILEGE_MODIFY      MAKE_ERROR_CODE(USER_OPR_PRIVILEGE,0x0002)  //�޸�һ����ɫ
#define USER_OPR_PRIVILEGE_DEL         MAKE_ERROR_CODE(USER_OPR_PRIVILEGE,0x0003)  //ɾ��һ����ɫ
#define USER_OPR_PRIVILEGE_SAVE        MAKE_ERROR_CODE(USER_OPR_PRIVILEGE,0x0004)  //����һ����ɫ��Ȩ�޵Ĺ�ϵ
//�������

//��ѯ����

//��Ϣ����

//���Ʋ���
#define USER_OPR_CONTROL_TRADESERVERINIT            MAKE_ERROR_CODE(USER_OPR_CONTROL,0x0001)  //���׷�������ʼ��
#define USER_OPR_CONTROL_TRADETIMESET               MAKE_ERROR_CODE(USER_OPR_CONTROL,0x0002)  //����ʱ�������
#define USER_OPR_CONTROL_TRADESERVERTIMEAUTOINIT    MAKE_ERROR_CODE(USER_OPR_CONTROL,0x0003)  //���׷�������ʱ��ʼ��ʱ������
#define USER_OPR_CONTROL_QUOTSERVERINIT             MAKE_ERROR_CODE(USER_OPR_CONTROL,0x0004)  //�����������ʼ��
#define USER_OPR_CONTROL_QUOTSERVERTIMEAUTOINIT     MAKE_ERROR_CODE(USER_OPR_CONTROL,0x0005)  //�������������ʱ����ʱ������
#define USER_OPR_CONTROL_SIMULATEQUOTSERVERSETUP    MAKE_ERROR_CODE(USER_OPR_CONTROL,0x0006)  //ģ���������������

///////////////////////////////////////////////////////////////////////////////
//��ʽ���������Ϊ�����ַ���FormatErrorCode
//Input: nErrorCode Ϊ����Ĵ�����
//Output: ������򷵻ؾ���Ĵ����ַ���
//        ���û���ҵ���Ӧ�Ĵ����룬�򷵻ؿմ�
/////////////////////////////////////////////////////////////////////////////////
inline const char * FormatOprCode(const int nErrorCode)
{
    switch(nErrorCode)
    {
        //ϵͳ����
    case USER_OPR_SYSTEM_EXIT:                         return "�˳�����";

        //�˻�����
    case USER_OPR_ACCOUNT_SYSTEMUSER_ADD:              return  "����һ��ϵͳ�û�";
    case USER_OPR_ACCOUNT_SYSTEMUSER_MODIFY:           return  "�޸�һ��ϵͳ�û�";
    case USER_OPR_ACCOUNT_SYSTEMUSER_DEL:              return  "ɾ��һ��ϵͳ�û�";
    case USER_OPR_ACCOUNT_SYSTEMUSER_USE:              return  "��/ͣ��һ��ϵͳ�û�";
    case USER_OPR_ACCOUNT_SYSTEMUSER_INIT:             return  "��ʼ��һ��ϵͳ�û�";
    case USER_OPR_ACCOUNT_SYSTEMUSER_ASSIGNORG:        return  "һ��ϵͳ�û�������֯����";
    case USER_OPR_ACCOUNT_SYSTEMUSER_ASSIGNPRODUCT:    return  "һ��ϵͳ�û�������Ʋ�Ʒ";
    case USER_OPR_ACCOUNT_SYSTEMUSER_ASSIGNACCOUNT:    return  "һ��ϵͳ�û�ָ��ί�н����˺�";

    case USER_OPR_ACCOUNT_INOUTFUND_QUERY:             return  "��ѯ�����";
    case USER_OPR_ACCOUNT_INOUTFUND_IN:                return  "��һ���˻����";
    case USER_OPR_ACCOUNT_INOUTFUND_OUT:               return  "��һ���˻�����";

    case USER_OPR_ACCOUNT_ACCOUNT_ADD:                 return  "����һ��ί�н����˺�";
    case USER_OPR_ACCOUNT_ACCOUNT_MODIFY:              return  "�޸�һ��ί�н����˺�";
    case USER_OPR_ACCOUNT_ACCOUNT_DEL:                 return  "ɾ��һ��ί�н����˺�";
    case USER_OPR_ACCOUNT_ACCOUNT_LOGIN:               return  "��¼һ��ί�н����˺�";
        //�������
    case USER_OPR_MANAGE_PRODUCT_ADD:                  return  "����һ����Ʋ�Ʒ";
    case USER_OPR_MANAGE_PRODUCT_MODIFY:               return  "�޸�һ����Ʋ�Ʒ";
    case USER_OPR_MANAGE_PRODUCT_DEL:                  return  "ɾ��һ����Ʋ�Ʒ";
    case USER_OPR_MANAGE_PRODUCT_SAVE:                 return  "����һ����Ʋ�Ʒ�ͽ���Ա�Ĺ�ϵ";

    case USER_OPR_MANAGE_BROKER_ADD:                   return  "����һ�����͹�˾";
    case USER_OPR_MANAGE_BROKER_MODIFY:                return  "�޸�һ�����͹�˾";
    case USER_OPR_MANAGE_BROKER_DEL:                   return  "ɾ��һ�����͹�˾";
    case USER_OPR_MANAGE_SERVERGROUP_ADD:              return  "����һ����������";
    case USER_OPR_MANAGE_SERVERGROUP_MODIFY:           return  "�޸�һ����������";
    case USER_OPR_MANAGE_SERVERGROUP_DEL:              return  "ɾ��һ����������";
    case USER_OPR_MANAGE_SERVERADDR_ADD:               return  "����һ�����������ַ";
    case USER_OPR_MANAGE_SERVERADDR_MODIFY:            return  "�޸�һ�����������ַ";
    case USER_OPR_MANAGE_SERVERADDR_DEL:               return  "ɾ��һ�����������ַ";
    case USER_OPR_MANAGE_QUOTSERVER_CONFIG:            return   "�������������";
    case USER_OPR_MANAGE_QUOTADDR_ADD:                 return  "����һ�������ַ";
    case USER_OPR_MANAGE_QUOTADDR_MODIFY:              return  "�޸�һ�������ַ";
    case USER_OPR_MANAGE_QUOTADDR_DEL:                 return  "ɾ��һ�������ַ";

    case USER_OPR_MANAGE_ORGANIZE_ADD:                 return  "����һ����֯����";
    case USER_OPR_MANAGE_ORGANIZE_MODIFY:              return  "�޸�һ����֯����";
    case USER_OPR_MANAGE_ORGANIZE_DEL:                 return  "ɾ��һ����֯����";
    case USER_OPR_MANAGE_ORGANIZE_SAVE:                return  "����һ����֯�����ͽ���Ա�Ĺ�ϵ";
        //���Բ���
    case USER_OPR_STRATEGY_ADD:                        return  "����һ�����ײ���";
    case USER_OPR_STRATEGY_MODIFY:                     return  "�޸�һ�����ײ���";
    case USER_OPR_STRATEGY_DEL:                        return  "ɾ��һ�����ײ���";
    case USER_OPR_STRATEGY_SAVE:                       return  "����һ�����ײ��Ժ���֯�����Ĺ�ϵ";
        //Ȩ�޲���
    case USER_OPR_PRIVILEGE_ADD:                        return  "����һ����ɫ";
    case USER_OPR_PRIVILEGE_MODIFY:                     return  "�޸�һ����ɫ";
    case USER_OPR_PRIVILEGE_DEL:                        return  "ɾ��һ����ɫ";
    case USER_OPR_PRIVILEGE_SAVE:                       return  "����һ����ɫ��Ȩ�޵Ĺ�ϵ";
        //�������

        //��ѯ����

        //��Ϣ����

        //���Ʋ���
    case USER_OPR_CONTROL_TRADESERVERINIT:            return  "���׷�������ʼ��";
    case USER_OPR_CONTROL_TRADETIMESET:               return  "����ʱ�������";
    case USER_OPR_CONTROL_TRADESERVERTIMEAUTOINIT:    return  "���׷�������ʱ��ʼ��ʱ������";
    case USER_OPR_CONTROL_QUOTSERVERINIT:             return  "�����������ʼ��";
    case USER_OPR_CONTROL_QUOTSERVERTIMEAUTOINIT:     return  "�������������ʱ����ʱ������";
    case USER_OPR_CONTROL_SIMULATEQUOTSERVERSETUP:    return  "ģ���������������";
    default:	return "";
    }
}