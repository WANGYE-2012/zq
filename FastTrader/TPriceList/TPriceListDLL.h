// TPriceList.h : main header file for the TPriceList DLL
//

#pragma once

#include <windows.h>
#include <vector>
#include <string>
#include "KernelStruct.h"

#define Msg_PriceListT_Click WM_USER+999

#ifndef TPRICELISTDLL

//�����µ�T�ͱ��۴��ڣ����ش��ھ��
HWND __declspec(dllimport) CreateWnd(HWND hParent);

//������Ȩ��Լ
void __declspec(dllimport) SetOptionInstruments(const std::vector<PlatformStru_InstrumentInfo>& vecOptions);

//�����鵽��
void __declspec(dllimport) NewMarketData(const PlatformStru_DepthMarketData& NewMD);

//ͨ��EventParamID��ȡ�������������ȡ��ɾ����
// nClickMode �����ģʽ��0 ����/ 1 ˫��
// strInstrumentID ��ԼID
// bDirection ����TRUE ���룬FALSE ����
bool __declspec(dllimport) Get_Del_ClickParam(int EventParamID, int& nClickMode, 
											  std::string& strInstrumentID, BOOL& bDirection);

//������ɫģʽ��0Ϊǳ��ɫ��ɫ��1Ϊ���ɫ��ɫ
void __declspec(dllimport) SetColorMode(int colmode);

#endif
