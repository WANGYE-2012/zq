// RiskManage.h : RiskManage Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CRiskManageApp:
// �йش����ʵ�֣������ RiskManage.cpp
//

class CRiskManageApp : public CWinApp
{
public:
	CRiskManageApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRiskManageApp theApp;