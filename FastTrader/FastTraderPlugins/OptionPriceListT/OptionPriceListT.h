// OptionPriceListT.h : OptionPriceListT Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// COptionPriceListTApp:
// �йش����ʵ�֣������ OptionPriceListT.cpp
//

class COptionPriceListTApp : public CWinApp
{
public:
	COptionPriceListTApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern COptionPriceListTApp theApp;