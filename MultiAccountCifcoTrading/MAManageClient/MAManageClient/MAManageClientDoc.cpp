// MAManageClientDoc.cpp : CMAManageClientDoc ���ʵ��
//

#include "stdafx.h"
#include "MAManageClient.h"

#include "MAManageClientDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMAManageClientDoc

IMPLEMENT_DYNCREATE(CMAManageClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CMAManageClientDoc, CDocument)
END_MESSAGE_MAP()


// CMAManageClientDoc ����/����

CMAManageClientDoc::CMAManageClientDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CMAManageClientDoc::~CMAManageClientDoc()
{
}

BOOL CMAManageClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CMAManageClientDoc ���л�

void CMAManageClientDoc::Serialize(CArchive& ar)
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


// CMAManageClientDoc ���

#ifdef _DEBUG
void CMAManageClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMAManageClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMAManageClientDoc ����
