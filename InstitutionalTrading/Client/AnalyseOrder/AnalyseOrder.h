// AnalyseOrder.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAnalyseOrderApp:
// �йش����ʵ�֣������ AnalyseOrder.cpp
//

class CAnalyseOrderApp : public CWinApp
{
public:
	CAnalyseOrderApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAnalyseOrderApp theApp;