// TestorProxy.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestorProxyApp:
// �йش����ʵ�֣������ TestorProxy.cpp
//

class CTestorProxyApp : public CWinApp
{
public:
	CTestorProxyApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CTestorProxyApp theApp;