// OptionPriceListTDoc.h : COptionPriceListTDoc ��Ľӿ�
//


#pragma once


class COptionPriceListTDoc : public CDocument
{
protected: // �������л�����
	COptionPriceListTDoc();
	DECLARE_DYNCREATE(COptionPriceListTDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~COptionPriceListTDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


