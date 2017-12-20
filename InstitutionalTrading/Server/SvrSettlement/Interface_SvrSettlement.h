#pragma once

#include "Interface_SvrCommon.h"
#include "CommonErrorCode.h"
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
//#include "../Module-Misc2/KernelStruct.h"
//#include "../Module-Misc2/BusinessData.hpp"
#include "CommonStruct.h" 
#include <vector>
#include <map>
#include <set>
using namespace std;
#ifndef SVRSETTLEMENT_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRSETTLEMENT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRSETTLEMENT_EXPORTS
#define SVRSETTLEMENT_API __declspec(dllexport)
#else
#define SVRSETTLEMENT_API __declspec(dllimport)
#endif
#endif


class SVRSETTLEMENT_API CInterface_SvrSettlement : public CInterface_SvrCommon
{
public:
    static CInterface_SvrSettlement& getObj(void);
	static void deleteObj(void);


	//---------------------------------------------------------------------------------
	//	���ݽӿ�
	//---------------------------------------------------------------------------------
	int getExampleValue(void);

	////---------------------------------------------------------------------------------
	////	��ʼ����
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////         CF_ERROR_NOT_SETTLEMENTTIME δ������ʱ��
	////         CF_ERROR_NO_SETTLEMENTPRICE û�н����
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR StratSettlement() = 0;

	////---------------------------------------------------------------------------------
	////	��ɽ��㣬�������
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////         
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR FinishSettlement() = 0;

	////---------------------------------------------------------------------------------
	////	��ѯ�������Ϣ
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////         CF_ERROR_NO_SETTLEMENTPRICE û�н����   
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR QuerySettlementPrice(std::vector<SettlementPriceField>& outData) = 0;

	////---------------------------------------------------------------------------------
	////	�޸Ľ������Ϣ
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////            
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR ModifySettlementPrice(const SettlementPriceField* indata) = 0;

	////---------------------------------------------------------------------------------
	////	��ѯ�ɽ�
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////         CF_ERROR_NO_TRADEDATA     ����û�гɽ���¼
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR QueryTrade(std::map<TradeKey,PlatformStru_TradeInfo>& outData) = 0;

	////---------------------------------------------------------------------------------
	////	����һ���ɽ�
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////         CF_ERROR_REDUPLICATE_TRADE �������ݿ������д����ɽ���¼   
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR AddTrade(PlatformStru_TradeInfo* indata) = 0;

	////---------------------------------------------------------------------------------
	////	ɾ��һ���ɽ�
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////           
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR DeleteTrade(PlatformStru_TradeInfo* indata) = 0;

	////---------------------------------------------------------------------------------
	////	�޸�һ���ɽ�
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////           
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR ModifyTrade(PlatformStru_TradeInfo* indata) = 0;

	////---------------------------------------------------------------------------------
	////	��ѯ�ֲ���ϸ
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////         CF_ERROR_NO_POSITIONDETAIL ����û�гֲ���ϸ
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR QueryPositionDetail(std::map<PositionDetailKey,PlatformStru_PositionDetail>& outData) = 0;

	////---------------------------------------------------------------------------------
	////	����һ���ֲ���ϸ
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////         CF_ERROR_REDUPLICATE_POSITIONDETAIL �������ݿ������д����ֲ���ϸ��¼   
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR AddPositionDetail(PlatformStru_PositionDetail* indata) = 0;

	////---------------------------------------------------------------------------------
	////	ɾ��һ���ֲ���ϸ
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////           
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR DeletePositionDetail(PlatformStru_PositionDetail* indata) = 0;

	////---------------------------------------------------------------------------------
	////	�޸�һ���ֲ���ϸ
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////           
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR ModifyPositionDetail(PlatformStru_PositionDetail* indata) = 0;

	////---------------------------------------------------------------------------------
	////	��ѯ�ֲ�
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////         CF_ERROR_NO_POSITION ����û�гֲ�
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR QueryPosition(std::map<PositionKey,PlatformStru_Position>& outData) = 0;

	////---------------------------------------------------------------------------------
	////	����һ���ֲ�
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////         CF_ERROR_REDUPLICATE_POSITION �������ݿ������д����ֲּ�¼   
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR AddPosition(PlatformStru_Position* indata) = 0;

	////---------------------------------------------------------------------------------
	////	ɾ��һ���ֲ�
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////           
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR DeletePosition(PlatformStru_Position* indata) = 0;

	////---------------------------------------------------------------------------------
	////	�޸�һ���ֲ�
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////           
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR ModifyPosition(PlatformStru_Position* indata) = 0;

	////---------------------------------------------------------------------------------
	////	���ý���ʱ��
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////           
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR SetSettltmentTime(TThostFtdcTimeType* SettltmentTime) = 0;

	////---------------------------------------------------------------------------------
	////	�ͻ����޸��ʽ���Ϣ
	////  ���أ� CF_ERROR_SUCCESS ����ɹ�
	////           
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR ModifyFund(PlatformStru_TradingAccountInfo* indata) = 0;

    CInterface_SvrSettlement();
    virtual ~ CInterface_SvrSettlement();
    static CInterface_SvrSettlement* m_pObj;
};







