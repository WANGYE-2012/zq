// TSSCtrlPlugin.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTSSCtrlPluginApp:
// �йش����ʵ�֣������ TSSCtrlPlugin.cpp
//

class CTSSCtrlPluginApp : public CWinApp
{
public:
	CTSSCtrlPluginApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CTSSCtrlPluginApp theApp;