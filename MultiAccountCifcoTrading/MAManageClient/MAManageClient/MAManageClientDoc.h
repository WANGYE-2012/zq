// MAManageClientDoc.h : CMAManageClientDoc ��Ľӿ�
//


#pragma once


class CMAManageClientDoc : public CDocument
{
protected: // �������л�����
	CMAManageClientDoc();
	DECLARE_DYNCREATE(CMAManageClientDoc)

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
	virtual ~CMAManageClientDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


