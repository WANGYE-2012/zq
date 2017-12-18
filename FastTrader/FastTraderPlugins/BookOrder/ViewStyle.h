// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once


// ��Ԫ����ɫ�ṹ��
typedef struct {
	DWORD dwBackColor;			// ����ɫ
	DWORD dwTextColor;			// �ı�ɫ
} CELLCOLOR, *PCELLCOLOR;

// �нṹ��ɫ�ṹ��
typedef struct {
	CELLCOLOR tFixedRow;		// �̶���
	CELLCOLOR tFixedEvenRow;	// �̶���ż����
	CELLCOLOR tRow;				// һ���������
	CELLCOLOR tEvenRow;			// ż����
} ROWCOLOR, *PROWCOLOR;

// PriceGrid��ɫ�ṹ��
typedef struct {
	//ROWCOLOR	tTradeBuyCol;	// ����ҵ����е�����ɫ����
	//ROWCOLOR	tConditionBuyCol;	// ����ҵ����е�����ɫ����
	//ROWCOLOR	tWaitBuyCol;	// ����ҵ����е�����ɫ����
	ROWCOLOR	tAskCol;		// �������е�����ɫ����
	ROWCOLOR	tPriceCol;		// ��۸��е�����ɫ����
	ROWCOLOR	tBidCol;		// �������е�����ɫ����
	//ROWCOLOR	tWaitSellCol;	// �����ҵ����е�����ɫ����
	//ROWCOLOR	tConditionSellCol;	// �����ҵ����е�����ɫ����
	//ROWCOLOR	tTradeSellCol;	// �����ҵ����е�����ɫ����
	ROWCOLOR	tOtherCol;	// �����ҵ����е�����ɫ����

	CELLCOLOR	tCellLastPrice;	// ���¼۵�Ԫ����ɫ����
	CELLCOLOR	tCellRisePrice;	// ������ɫ����
	CELLCOLOR	tCellFallPrice;	// �µ���ɫ����
} PRICEGRIDCOLOR, *PPRICEGRIDCOLOR;

// BookOrder�Ի������ɫ�ṹ��
typedef struct {
	int			nViewMode;				// ������ʽ
	DWORD		dwDlgBackColor;			// �Ի��򱳾�ɫ
	DWORD		dwDlgButtonBackColor;	// �Ի���ǰ��ɫ����Ҫ���ڿؼ��ı���ɫ
	DWORD		dwDlgEditBackColor;		// �Ի���ǰ��ɫ����Ҫ���ڿؼ��ı���ɫ
	ROWCOLOR	tAccount;
	ROWCOLOR	tPosition;
	PRICEGRIDCOLOR tPrice;
} BOOKORDERDLGCOLOR, *PBOOKORDERDLGCOLOR;

BOOL GetColorStyle(int nStyle, BOOKORDERDLGCOLOR& colorStyle);