// OptionPriceListTView.cpp : COptionPriceListTView ���ʵ��
//

#include "stdafx.h"
#include "OptionPriceListT.h"

#include "OptionPriceListTDoc.h"
#include "OptionPriceListTView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COptionPriceListTView

IMPLEMENT_DYNCREATE(COptionPriceListTView, CView)

BEGIN_MESSAGE_MAP(COptionPriceListTView, CView)
END_MESSAGE_MAP()

// COptionPriceListTView ����/����

COptionPriceListTView::COptionPriceListTView()
{
	// TODO: �ڴ˴���ӹ������

}

COptionPriceListTView::~COptionPriceListTView()
{
}

BOOL COptionPriceListTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// COptionPriceListTView ����

void COptionPriceListTView::OnDraw(CDC* /*pDC*/)
{
	COptionPriceListTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// COptionPriceListTView ���

#ifdef _DEBUG
void COptionPriceListTView::AssertValid() const
{
	CView::AssertValid();
}

void COptionPriceListTView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COptionPriceListTDoc* COptionPriceListTView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COptionPriceListTDoc)));
	return (COptionPriceListTDoc*)m_pDocument;
}
#endif //_DEBUG


// COptionPriceListTView ��Ϣ�������
