// Module-BookOrder.h : Module-BookOrder Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CModuleBookOrderApp:
// �йش����ʵ�֣������ Module-BookOrder.cpp
//

class CModuleBookOrderApp : public CWinApp
{
public:
	CModuleBookOrderApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CModuleBookOrderApp theApp;