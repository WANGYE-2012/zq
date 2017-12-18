
#pragma once


#define CONSOLE_MAGIC_NUMBER  0x4946


#define CONSOLE_INPUT_ORDER 1		// �µ�
#define CONSOLE_INPUT_ACTION 2		// ����
#define CONSOLE_INPUT_RANDAM 3		// ���


enum {
	CONSOLE_CMD_INPUTSIMPLE,		// �����µ�ָ��
	CONSOLE_CMD_EXITFASTTRADER,		// �˳�FastTrader����
};

enum {
	CONSOLE_RSP_HWND,		//���ؽ��׿ͻ��˵������ھ��
	CONSOLE_RSP_DOORDER,	//���ؽ��׿ͻ��˵��µ�/����ִ�н��
};


// console֪ͨ���µ�
typedef struct {
	int		nSeqNum;			// �����Ӧ��SeqNum
	int		nInputType;			// �µ����ͣ��µ������������
	int		nQty;				// �µ�������-1 �����>=0 ָ������
	int		nMaxQty;			// �µ��������ޣ�-1 �����>=0 ָ������
	char 	OpenClose;			// ctp�Ŀ�ƽ�֣�0 �����'0' ���֣�'1' ƽ��
	char 	Direction;			// ctp�ķ���0 �����'0' ��'1' ��
	char	OrderPrice;			// �ҵ�ģʽ��0 �����'0' Ҫ��ɽ���'1' Ҫ��ҵ�
	char	CancelMode;			// ����ģʽ��0 �����'0' ��һ�ʹҵ���'1' �����йҵ�
	char	InstrumentID[32];	// ctp��ԼID
} CMD_INPUTSIMPLE;


// ��FastTrader���ظ�Console��ͨѶ���
typedef struct {
	DWORD	dwProcID;	// FastTrader����ID
	HWND	hWnd;		// FastTrader�����ھ��
} RSP_FASTTRADERHWND;


// ��FastTrader���ظ�Console���µ��������
typedef struct {
	DWORD	nSeqNum;	// �����Ӧ��SeqNum
	int		nResult;	// FastTrader��������0 ���գ�1 �·�api��2 �ܾ��·�api
} RSP_DOORDER;

