// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#include <string>
#include <set>
#include <map>
#include <queue>
#include <vector>
#include "PlusinCommon/PlusinAPI.h"
#include "SequenceManager.h"
#include "ChangeOrderManager.h"
#include "OrderFunc.h"
#include "ViewStyle.h"

using namespace std;

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
	double	fPriceTick;
} PRICEGRIDINDEX, *PPRICEGRIDINDEX;


typedef std::map<std::string, std::string> MAPINSTCOMPTBL;


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

void SetInstComptblList(MAPINSTCOMPTBL& mapInstComptbl);
MAPINSTCOMPTBL& GetSetInstComptblList();

void MakeInstCompatibleList();
CString GetInstRealID(CString& strInstID);

std::set<CWnd*>& GetBookOrderDlgSet();
CSequenceManager& GetSequenceManager();
CChangeOrderManager& GetChangeOrderManager();

int GetColorStyle();
BOOKORDERDLGCOLOR& GetBookOrderDlgColor();
void SetBookOrderDlgColor(BOOKORDERDLGCOLOR& tDlgColor, int nColorStyle);

SIZE& GetBookDlgSize();
void SetBookDlgSize(SIZE& dlgSize);

CString GetRealPath();
CString GetIniFile();

void SaveConfig();
void LoadConfig();