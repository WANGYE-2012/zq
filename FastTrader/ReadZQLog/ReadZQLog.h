// ReadZQLog.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CReadZQLogApp:
// �йش����ʵ�֣������ ReadZQLog.cpp
//

class CReadZQLogApp : public CWinApp
{
public:
	CReadZQLogApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CReadZQLogApp theApp;