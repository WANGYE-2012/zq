// RiskManageDoc.h : CRiskManageDoc ��Ľӿ�
//


#pragma once


class CRiskManageDoc : public CDocument
{
protected: // �������л�����
	CRiskManageDoc();
	DECLARE_DYNCREATE(CRiskManageDoc)

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
	virtual ~CRiskManageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


