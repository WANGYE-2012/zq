// TSSPlotConfig.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTSSPlotConfigApp:
// �йش����ʵ�֣������ TSSPlotConfig.cpp
//

class CTSSPlotConfigApp : public CWinApp
{
public:
	CTSSPlotConfigApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTSSPlotConfigApp theApp;