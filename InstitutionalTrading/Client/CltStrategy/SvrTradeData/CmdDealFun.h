#pragma once
#include "CommonPkg.h"


//��ѯ��Լ
bool QryInstrumentDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ѯ����
bool QryCommissionRateDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ѯ��֤����
bool QryMarginRateDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ѯ�ʽ�
bool QryUserAccountInfoDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ѯ�ֲ�
bool QryUserPositionDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ѯ�ֲ���ϸ
bool QryUserPositionDetailDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ѯ��ϳֲ���ϸ
bool QryUserComPositionDetailDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ѯ����
bool QryUserOrderDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ѯ�ɽ�
bool QryUserTraderDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//���������
bool ReqFundInOut(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ѯ��ʷ���������
bool ReqQryFundInOut(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ѯ����ȷ������
bool ReqQrySettlementConfirm(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ѯί�н����˺ŵ�״̬
bool ReqQryTraderAccountStatus(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//�ֶ�����ĳһ��ί�н����ʺŵĳ�ʼ��
bool ReqQryTraderAccountInit(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//����ȷ������
bool ReqSettlementConfirm(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ѯ�û���ʼ��״̬
bool ReqQryUserInitStatus(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ʼ���û�����
bool ReqInitNewUser(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);


//ǿ�Ƴ�ʼ��ģ��
bool ReForceInitModule(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ʼ��ģ��
bool ReInitModule(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ѯ��ʼ��ģ��ʱ��
bool QryReinitTime(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//���ó�ʼ��ģ��ʱ��
bool SetReinitTime(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ȡ��ǰ״̬
bool QryCurrentStatus(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ѯ��ʼ��ģ��ʱ��
bool QryTradingTime(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//���ó�ʼ��ģ��ʱ��
bool SetTradingTime(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);



//�����ѯ�ͻ�֪ͨ��Ӧ
bool QryNotice(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//�ڻ������ѯ�������֪ͨ
bool QryContractBank(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//�����ѯԤ��
bool QryParkedOrder(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//�����ѯԤ�񳷵�
bool QryParkedOrderAction(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//�����ѯ�������
bool QryDepthMarket(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);


//////��������Ӧ������
//�����ѯ������
bool SlaverQryTradingDay(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//����ͬ�������ͳɽ�
bool SlaverQrySyncReq(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��������
bool SlaverReqMasterQuotSend(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);


/////�ӷ�����Ӧ������
//�����ѯ��������Ӧ
bool SlaverQryTradingDayRsp(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);


//��Ӧ����push
bool SlaverRecMasterOrderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��Ӧ�ɽ�push
bool SlaverRecMasterTraderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��Ӧ���ʺű���push
bool SlaverRecMasterCTPOrderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��Ӧ���ʺųɽ�push
bool SlaverRecMasterCTPTraderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��Ӧ����push
bool SlaverRecMasterQuotPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);


//��Ӧ����push
bool SlaverRecMasterQryCommissionPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��Ӧ��֤��push
bool SlaverRecMasterQryMarginPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��Ӧ�ʽ�push
bool SlaverRecMasterQryFundPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��Ӧ�ֲ�push
bool SlaverRecMasterQryPositionPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��Ӧ�ֲ���ϸpush
bool SlaverRecMasterQryPositionDetailPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��Ӧ�û���ʼ��
bool SlaverRecUserInitPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��Ӧ���ʺŵĲ�ѯ����
bool SlaverRecMasterQryOrderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��Ӧ���ʺŵĲ�ѯ�ɽ�
bool SlaverRecMasterQryTraderPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//����ͬ�������ͳɽ�
bool SlaverQrySyncRsp(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);


//��Ӧ�µ�����
bool SlaverRecMasterErrOrderInsertPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��Ӧ��������
bool SlaverRecMasterErrOrderActionPush(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);