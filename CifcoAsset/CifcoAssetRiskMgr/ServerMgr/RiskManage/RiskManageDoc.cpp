// RiskManageDoc.cpp : CRiskManageDoc ���ʵ��
//

#include "stdafx.h"
#include "RiskManage.h"

#include "RiskManageDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRiskManageDoc

IMPLEMENT_DYNCREATE(CRiskManageDoc, CDocument)

BEGIN_MESSAGE_MAP(CRiskManageDoc, CDocument)
END_MESSAGE_MAP()


// CRiskManageDoc ����/����

CRiskManageDoc::CRiskManageDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CRiskManageDoc::~CRiskManageDoc()
{
}

BOOL CRiskManageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CRiskManageDoc ���л�

void CRiskManageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CRiskManageDoc ���

#ifdef _DEBUG
void CRiskManageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRiskManageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CRiskManageDoc ����
