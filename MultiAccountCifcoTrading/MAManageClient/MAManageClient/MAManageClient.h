// MAManageClient.h : MAManageClient Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CMAManageClientApp:
// �йش����ʵ�֣������ MAManageClient.cpp
//

class CMAManageClientApp : public CWinApp
{
public:
	CMAManageClientApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMAManageClientApp theApp;