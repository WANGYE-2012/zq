#pragma  once
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"

//�������ͷ�ļ��ж���������˺Ϳͻ��˹��õ����ݽṹ��
#pragma pack(push)
#pragma pack(1)

struct sFundInfoEx
{
	double mdbInitFund;
	PlatformStru_TradingAccountInfo mTradingAccountInfo;
};

#pragma pack(pop)