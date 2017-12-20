#pragma once
#include "..\CommonClasses\DataEntity.h"
#include "TestEntity.h"

using namespace std;


// ��CTestManager���ù���CTestEntity��
// ��Ҫ���ܣ�
// 1.  ������ʼ��CTestEntity��
// 2.  ִ�в��Բ��Թ��̡�
// 3.  ��ͣ����������ֹ���Բ��Թ��̡�


class CTestManager
{
protected:
	CTestManager(void);
	virtual ~CTestManager(void);

public:
	static CTestEntity* CreateTestEntity(const std::string& strSavePath, const std::string& strRunPath);

public:
	static CTestEntity* sm_pTestEntity;

};
