
#pragma once

/////ʲô��������
//#define OPC_DirectMode_Nothing '0'
/////ֻ�ܿ���
//#define OPC_DirectMode_OnlyLong '1'
/////ֻ�ܿ���
//#define OPC_DirectMode_OnlyShort '2'
/////�ɿ���ɿ���
//#define OPC_DirectMode_LongShort '3'

/////////////////////////////////////////////////////////////////////////
///TFtdcInstrumentIDType��һ����Լ��������
/////////////////////////////////////////////////////////////////////////
typedef char TInstrumentIDType[31];
/////////////////////////////////////////////////////////////////////////
///TPlotIDType��һ�����Դ�������
/////////////////////////////////////////////////////////////////////////
typedef char TPlotIDType[31];
/////////////////////////////////////////////////////////////////////////
///TPLOTDIRECTType��һ�����Կ��Ʒ�������
/////////////////////////////////////////////////////////////////////////
typedef char TPlotDirectType[31];


// �����ڲ�����ṹ
typedef struct {
	int					nDirectMode;		// 0 ˫�ߣ�1 �࣬-1 ��
	TPlotIDType			plotID;
	TInstrumentIDType	instID;
} INSTPLOTDIRECT, *PINSTPLOTDIRECT;

// ���Եĵ���ṹ
typedef struct {
	TPlotIDType			plotID;
	TInstrumentIDType	instID;
	TPlotDirectType		direct;				// Sell��ForbidOpenSell��Donotcare��ForbidOpenBuy��Buy
} IMPORTPLOTDIRECT, *PIMPORTPLOTDIRECT;

static int g_nPlotDirectCount = 5;
static char* g_pcsPlotDirect[] = { "sell", "forbidopensell", "donotcare", "forbidopenbuy", "buy" };