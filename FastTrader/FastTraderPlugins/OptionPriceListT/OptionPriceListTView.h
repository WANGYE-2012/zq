// OptionPriceListTView.h : COptionPriceListTView ��Ľӿ�
//


#pragma once


class COptionPriceListTView : public CView
{
protected: // �������л�����
	COptionPriceListTView();
	DECLARE_DYNCREATE(COptionPriceListTView)

// ����
public:
	COptionPriceListTDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~COptionPriceListTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // OptionPriceListTView.cpp �еĵ��԰汾
inline COptionPriceListTDoc* COptionPriceListTView::GetDocument() const
   { return reinterpret_cast<COptionPriceListTDoc*>(m_pDocument); }
#endif

