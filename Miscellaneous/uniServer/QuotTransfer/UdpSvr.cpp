#include "QuotTransfer.h"
#include "easytime.h"



#define PREFIX_PACKAGECACHE		0xa973b71c					//Package��ʼ��־
#define TAIL_PACKAGECACHE		0x5a7b9c2d					//Package������־

#define CMDID_QuotInfo_ctp		4001


#pragma pack(push)
#pragma pack(4)

//���涨��Package��ͷ�ṹ
typedef struct
{
	DWORD	Prefix;						                    //ǰ����־�����ڱ�־һ��package�Ŀ�ʼ
	int		CmdID;						                    //Package������ID��������CMDID_xxxx
	int		PlatformID;					                    //����ƽ̨ID��������PLATFORM_xxxx
	int		ExchangeID;					                    //������ID��������EXCHANGE_xxxx

	int		DataLen;					                    //���ݳ��ȣ�ָ���ݵĳ��ȡ�ע���������ݵĴ�С�ǰ�DataLen�������㲹����ʹ֮Ϊ4�ֽڱ߽硣
	DWORD	dwCRC;						                    //���ݵ������ͣ����ݰ���PackageHead��Data��Tail
	DWORD	dwSeq;						                    //ͬһ��Package�����к�
}PackageHeadStru;

/***********************************************************
*   name    :   Stru_QuotAdditionalInfo
*   brief   :   ������Ϣ�ĸ��Ӳ���
***********************************************************/
typedef struct
{
    time_t  RecvTime_utc;                                   //���յ�����������Ϣ��ʱ���UTC����
    int     RecvTime_ms;                                    //���յ�����������Ϣ��ʱ��ĺ�����
    time_t  UpdateTime_utc;                                 //����������Ϣ���µ�ʱ���UTC����������ǰ����ֵ����Ϊ��
    int     UpdateTime_ms;                                  //����������Ϣ���µ�ʱ��ĺ�����
    int     Seq;
}Stru_QuotAdditionalInfo;

#pragma pack(pop)


void SendNewQuotData(Stru_Quot2& QuotData);



//��udp simple�õ����ݺ󣬵��ô˻ص��������д���
QUOTTRANSFER_API void UdpRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
    PackageHeadStru* pPackageHead;                          //��ͷָ��
    CThostFtdcDepthMarketDataField* pThostQuotData;         //��������ָ��
    Stru_QuotAdditionalInfo* pAdditionalInfo;               //������Ϣָ�룬�����ʱ�䡢����ʱ���

	if(DataLen<sizeof(PackageHeadStru)+sizeof(CThostFtdcDepthMarketDataField)+sizeof(Stru_QuotAdditionalInfo))
		return;

    pPackageHead=(PackageHeadStru*)pData;
    pThostQuotData=(CThostFtdcDepthMarketDataField*)((char*)pData+sizeof(PackageHeadStru));
    pAdditionalInfo=(Stru_QuotAdditionalInfo*)((char*)pData+sizeof(PackageHeadStru)+sizeof(CThostFtdcDepthMarketDataField));

	if(pPackageHead->Prefix!=PREFIX_PACKAGECACHE) 
		return;
	if(pPackageHead->CmdID!=CMDID_QuotInfo_ctp)
		return;

	Stru_Quot2 TmpQuot;
	std::string strInstrumentID;

	memset(&TmpQuot,0,sizeof(TmpQuot));
	TmpQuot.Head=PACKAGEHEAD;
	TmpQuot.PackageLen=sizeof(TmpQuot);
	TmpQuot.CRCValue=0;
	TmpQuot.Cmd=0;
	strncpy(TmpQuot.InstrumentID,pThostQuotData->InstrumentID,sizeof(TmpQuot.InstrumentID)-1);
	TmpQuot.OpenPrice=pThostQuotData->OpenPrice;
	TmpQuot.LastPrice=pThostQuotData->LastPrice;
	TmpQuot.Volume=pThostQuotData->Volume;
	TmpQuot.OpenInterest=(int)pThostQuotData->OpenInterest;			//�ֲ���
	TmpQuot.PreSettlementPrice=pThostQuotData->PreSettlementPrice;	//������
	TmpQuot.PreClosePrice=pThostQuotData->PreClosePrice;			//�����̼�
    TmpQuot.UpdateTime_utc=(DWORD)CEasyTime(NULL,pThostQuotData->UpdateTime).getutc();			//����ʱ��,utc��ʽ����λΪ��
    TmpQuot.UpdateTimeMS=pThostQuotData->UpdateMillisec;			//����ʱ�������
//	TmpQuot.UpdateTime_utc=(DWORD)pAdditionalInfo->UpdateTime_utc;			//����ʱ��,utc��ʽ����λΪ��
//	TmpQuot.UpdateTimeMS=pAdditionalInfo->UpdateTime_ms;			//����ʱ�������
	TmpQuot.Tail=PACKAGETAIL;										//��β
	
	strInstrumentID=TmpQuot.InstrumentID;

	g_Quots[strInstrumentID]=TmpQuot;

	SendNewQuotData(TmpQuot);

	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	if(!CWinService::IsRunInServiceMode())
	{
		printf("%02d:%02d:%02d.%03d �����鵽�� %s\n",CurTime.wHour,CurTime.wMinute,CurTime.wSecond,CurTime.wMilliseconds%1000,
			TmpQuot.InstrumentID);
	}
}
