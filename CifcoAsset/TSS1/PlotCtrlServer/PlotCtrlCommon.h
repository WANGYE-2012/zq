
#pragma once

///ʲô��������
#define OPC_DirectMode_Nothing '0'
///ֻ�ܿ���
#define OPC_DirectMode_OnlyLong '1'
///ֻ�ܿ���
#define OPC_DirectMode_OnlyShort '2'
///�ɿ���ɿ���
#define OPC_DirectMode_LongShort '3'

/////////////////////////////////////////////////////////////////////////
///TFtdcInstrumentIDType��һ����Լ��������
/////////////////////////////////////////////////////////////////////////
typedef char TInstrumentIDType[31];
/////////////////////////////////////////////////////////////////////////
///TPlotIDType��һ�����Դ�������
/////////////////////////////////////////////////////////////////////////
typedef char TPlotIDType[31];


// ���Է��������ĺ������µ���������
#define	Cmd_OPC_CtrlDirect_Min				0x200000			
#define	Cmd_OPC_CtrlDirect_MAX				0x2FFFFF			
#define	Cmd_OPC_SubscribeCtrlDirect_Req		0x200001	//���ı�����������
#define	Cmd_OPC_SubscribeCtrlDirect_Rsp		0x200002	//���ı������򷵻�
#define	Cmd_OPC_CtrlDirect_Push				0x200003	//������������
#define	Cmd_OPC_UnSubscribeCtrlDirect_Req	0x200004	//�˶�������������		
#define	Cmd_OPC_UnSubscribeCtrlDirect_Rsp	0x200005	//�˶��������򷵻�

#define CONFIGFILENAME "plotCtrl.dat"

//�����¼����ݽṹ
#pragma pack(push)
#pragma pack(1)

typedef struct {
	char				cDirectMode;
	TPlotIDType			plotID;
	TInstrumentIDType	inst;
} INSTRUMENTCTRLDIRECT, *PINSTRUMENTCTRLDIRECT;


// ���Ժ�Լ���ýṹ
typedef struct {
	double				maxLoseForDay;			// �����������
	int					forceCloseTime;			// ǿƽ����ʱ
	int					cancelForceTime;		// �ȹ�ʱ��
	int					fcPriceUp;				// ǿƽ����
	DWORD				dwLongColor;
	DWORD				dwShortColor;
	DWORD				dwNoneColor;
	DWORD				dwLimitAllColor;
	int					plotItemCount;			// ���ӵĲ��Ժ�Լ������
} PLOTCTRLHEADCFG, *PPLOTCTRLHEADCFG;


// ���Ժ�Լ���ýṹ
typedef struct {
	TPlotIDType			plotID;
	TInstrumentIDType	inst;
	int					stopGainPoint;
	int					stopLosePoint;
	int					maxPosiVol;
	int					singlePosiTime;
} PLOTCTRLITEMCFG, *PPLOTCTRLITEMCFG;

#pragma pack(pop)
