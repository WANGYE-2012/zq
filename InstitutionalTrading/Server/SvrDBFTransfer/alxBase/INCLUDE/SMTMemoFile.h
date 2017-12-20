// SMTMemoFile.h : interface of the CSMTMemoFile class
//
/////////////////////////////////////////////////////////////////////////////
/*
�����:              �������� �.�.
����������:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_SMTMEMOFILE_H__INCLUDED_)
#define ALX_SMTMEMOFILE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MemoFile.h"


////////////////////////////////////////////////////////////////////////
// CSMTMemoFile - a Memo File

class ALX_EXT_CLASS CSMTMemoFile : public CMemoFile
{

	DECLARE_DYNAMIC(CSMTMemoFile)

// Constructors
public:
	CSMTMemoFile();

// Attributes
public:
	SMT_HEAD	m_SMTHead;	// ��������� ���� ����

// Overridables
public:

// Operations
public:

// Implementation
public:
	~CSMTMemoFile();

	virtual void Create(LPCTSTR lpszName);
	virtual void Open(LPCTSTR lpszName, UINT nOpenFlag);
	virtual void Close();

	// ��� �����
	virtual UINT GetMemoFileType();

	// ������ ���� ����
	virtual void ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, CString& strMemo); 
	virtual void ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, unsigned char** pMemoData);
	// ���������� ���� ����
	virtual void WriteMemoRecord(DBF_ULONG& lOffsetRec, DBF_LONG& lLenMemo, CString& strMemo);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	BOOL ReadHeader();	// ������ ���������
	BOOL WriteHeader();	// ���������� ���������

private:
	// ��������� ���� ����
	DBF_LONG AddMemoRecord(CString& strMemo);
	// ��������� ���� ����
	void UpdateMemoRecord(DBF_ULONG lOffsetRec, CString& strMemo);
	// ������� ���� ����
	void DeleteMemoRecord(DBF_ULONG lDelRec, DBF_UINT nDelBlockCnt);
	// �������������� ���� ����
	BOOL UndeleteMemoRecord(DBF_ULONG& lOffsetRec, CString& strMemo);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_MEMOFILE_H__INCLUDED_)

