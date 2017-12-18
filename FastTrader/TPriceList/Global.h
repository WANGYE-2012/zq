// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#include "OrderFunc.h"
#include "KernelStruct.h"

//���۱�
enum QuotTable
{
	QUOT_InstrumentID,
	QUOT_LastPrice,
	//QUOT_ChangeValue,
	//QUOT_ChangePercentage,
	QUOT_BidPrice1,
	//QUOT_BidVolume1,
	QUOT_AskPrice1,
	//QUOT_AskVolume1,
	//QUOT_OpenInterest,
	//QUOT_OpenPrice,
	//QUOT_PreSettlementPrice,
	QUOT_LastVolume,
	//QUOT_HighestPrice,
	//QUOT_LowestPrice,
	//QUOT_ClosePrice,
	QUOT_Volume,
	//QUOT_Turnover,
	//QUOT_UpperLimitPrice,
	//QUOT_LowerLimitPrice,
	//QUOT_SettlementPrice,
	//QUOT_PreClosePrice,
	//QUOT_PreOpenInterest,
	//QUOT_UpdateTime,
	QUOT_KLineView,
	QUOT_COUNT,						// �����ֶ�����
	QUOT_OptionPrice,				// ��Ȩִ�м�
	QUOT_HEAD_REVERSE = 100,		// �����ſ�ʼ
};

#define QUOTNEWHEAD QUOT_HEAD_REVERSE


typedef struct {
	int nSeqNum;
	int nCmd;
	int nErrorCode;
	int nDataLen;
	void* pData;
} ITFCMDATA, *PITFCMDATA;


BOOL LockGlobalObject();
BOOL UnlockGlobalObject();
void ReleaseGlobalObject();

UINT GetColSetChangedMSGID();

std::map<std::string, PlatformStru_InstrumentInfo>& GetInstrumentInfos();

void ParseOptionInstrument(std::map<std::string, PlatformStru_InstrumentInfo>& mapInstrument);
std::map<std::string, std::map<std::string, std::set<std::string>>>& GetOptionTargetRelate();
BOOL FindTargetID(const CString& strOptionID, CString& strTargetID); 

void InitGridMap();
// ����ȷʵ����ʾ��
void ResetDefaultShowCol();
const std::map<int, std::string>& GetTargetColMap();
const std::map<int, std::string>& GetOptionColMap();

const std::vector<int>& GetTargetShowCol();
void SetTargetShowCol(const std::vector<int>& vecCol);
const std::vector<int>& GetOptionShowCol();
void SetOptionShowCol(const std::vector<int>& vecCol);

CString GetIniFile();
CString GetXmlFile();

void SaveConfig();
void LoadConfig();
