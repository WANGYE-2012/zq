// TestTPriceList.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestTPriceListApp:
// �йش����ʵ�֣������ TestTPriceList.cpp
//

class CTestTPriceListApp : public CWinApp
{
public:
	CTestTPriceListApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestTPriceListApp theApp;