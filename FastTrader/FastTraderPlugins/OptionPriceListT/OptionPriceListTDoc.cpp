// OptionPriceListTDoc.cpp : COptionPriceListTDoc ���ʵ��
//

#include "stdafx.h"
#include "OptionPriceListT.h"

#include "OptionPriceListTDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COptionPriceListTDoc

IMPLEMENT_DYNCREATE(COptionPriceListTDoc, CDocument)

BEGIN_MESSAGE_MAP(COptionPriceListTDoc, CDocument)
END_MESSAGE_MAP()


// COptionPriceListTDoc ����/����

COptionPriceListTDoc::COptionPriceListTDoc()
{
	// TODO: �ڴ����һ���Թ������

}

COptionPriceListTDoc::~COptionPriceListTDoc()
{
}

BOOL COptionPriceListTDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// COptionPriceListTDoc ���л�

void COptionPriceListTDoc::Serialize(CArchive& ar)
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


// COptionPriceListTDoc ���

#ifdef _DEBUG
void COptionPriceListTDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COptionPriceListTDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COptionPriceListTDoc ����
