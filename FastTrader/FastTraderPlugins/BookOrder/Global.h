// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#include "PlusinCommon/PlusinAPI.h"
#include "SequenceManager.h"
#include "ChangeOrderManager.h"
#include "OrderFunc.h"
#include "ViewStyle.h"
#include <vector>
#include <map>
#include <string>


enum {
	conKeyNone, conKeyShift, conKeyCtrl, conKeyAlt
};

enum {
	conOrderByMarket, conOrderByLastPrice, conOrderByAskBidPrice
};


typedef struct {
	int nSeqNum;
	int nCmd;
	int nErrorCode;
	int nDataLen;
	void* pData;
} ITFCMDATA, *PITFCMDATA;


typedef struct {
	BOOL	bHasData;
	BOOL	bFixedPrice;
	int		nGridRowCount;
	int		nLastPriceRowIndex;
	double	fTopPrice;
	double	fLastPrice;
	double	fPrevSettlePrice;
	double	fPriceTick;
} PRICEGRIDINDEX, *PPRICEGRIDINDEX;


typedef struct {
	int		nAutoOrderKey;
	int		nOpenOrderKey;
	int		nCloseOrderKey;
	int		nCondOrderPriceMode;
	int		nCondOrderOffset;
} ORDERSETTING, *LPORDERSETTING;


typedef struct {
	UINT	nID;
	char	AccountID[64];
	char	SubAccountID[64];
	char	InstrumentID[64];
	double	PriceTick;
	double	TriggerPrice;		// �����۸�
	int		OrderPriceMode;
	BOOL	Direction;			// ���� 0 ���룬1 ����
	int		OrderOffset;		// ����ƫ��
	int		OpenClose;			// ��ƽ��1 ����, 2 ƽ��, 3 ƽ��
	int		Qty;				// �µ�����
} LOCALTRIGGERORDER, *LPLOCALTRIGGERORDER;


// ���������� map<ID, ��������>
typedef std::map<int, LOCALTRIGGERORDER> MAPPRICETRIGGERITEM;
// ��Լ������������ map<��Լ+����, ����������>
// ��Լ����"IF1409"��"IF1409"
typedef std::map<std::string, MAPPRICETRIGGERITEM> MAPLOCALTRIGGERORDER;


BOOL LockGlobalObject();
BOOL UnlockGlobalObject();
void ReleaseGlobalObject();

BOOL GetInitStatus();
void SetInitStatus(BOOL bIsSuccess);

void SetTargetHWnd(HWND hTargetWnd);
HWND GetTargetHWnd();

void SetPlusinAPI(PlusinAPI* poPlusinApi);
PlusinAPI* GetPlusinAPI();

UINT GetBookOrderCloseMSGID();

void SetIsConfirmRaskInfo(BOOL bConfirm);
BOOL GetIsConfirmRaskInfo();

void SetProductShortNames(CString& strProductShortNames);
CString& GetProductShortNames();

void SetExchangeIDList(std::set<std::string>& setExchangeID);
std::set<std::string>& GetExchangeIDList();

void SetInstrumentIDList(std::set<std::string>& setInstrumentID);
std::set<std::string>& GetInstrumentIDList();

std::map<std::string, std::string>& GetProduct2ExchangeList();

std::set<CWnd*>& GetBookOrderDlgSet();
CSequenceManager& GetSequenceManager();
CChangeOrderManager& GetChangeOrderManager();

int GetColorStyle();
BOOKORDERDLGCOLOR& GetBookOrderDlgColor();
void SetBookOrderDlgColor(BOOKORDERDLGCOLOR& tDlgColor, int nColorStyle);

SIZE& GetBookDlgSize();
void SetBookDlgSize(SIZE& dlgSize);

void SetOrderSetting(const ORDERSETTING& setting);
const ORDERSETTING& GetOrderSetting();

BOOL ResetPosition(PlatformStru_Position* pPosi, int nCount);

int GetNextLocalCondID();
const MAPLOCALTRIGGERORDER& GetLocalConds();
int AppendLocalCond(LOCALTRIGGERORDER& localCond);
BOOL DeleteLocalCondByID(const std::string& strInstrumentID, int nID);
BOOL DeleteLocalCondByPrice(const std::string& strInstrumentID, BOOL bDirection, double fTriggerPrice);
void ClearLocalConds();
void OnQuoteForLocalConds(const PlatformStru_DepthMarketData& quote);


CString GetIniFile();

void SaveConfig();
void LoadConfig();