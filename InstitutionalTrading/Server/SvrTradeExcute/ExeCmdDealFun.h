
#pragma once
#include "CommonPkg.h"
//
//void GenerateOrderInfo(const PlatformStru_InputOrder& inputOrder,PlatformStru_OrderInfo & orderInfo);
//void GenerateInputOrder(const PlatformStru_OrderInfo& orderInfo,PlatformStru_InputOrder & inputOrder);

//�㲥
bool BroadcastBufToClient(const int nRspID,const char* nUserId,PVOID buf,int nBufLen,int nRequestID = 0);
//�µ�
bool InputOrderReqDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//����
bool ActionOrderReqDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//����������˽��
bool VerifyedOrderDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//��ѯ�÷��Ա�����Ľ���Ա�Ĵ���˱���
bool QryApprovingOrderInfo(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//�µ�
bool StragegyInputOrderReqDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);

//����
bool StragegyActionOrderReqDeal(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);