// DBTMemoFile.h : interface of the CDBTMemoFile class
//
/////////////////////////////////////////////////////////////////////////////
/*
�����:              �������� �.�.
����������:         alxsoft@gazinter.net
����������:
			��������� ������ � ���� ���� ������� dBASEIV
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_DBTMEMOFILE_H__INCLUDED_)
#define ALX_DBTMEMOFILE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MemoFile.h"


////////////////////////////////////////////////////////////////////////
// CDBTMemoFile - a Memo File

class ALX_EXT_CLASS CDBTMemoFile : public CMemoFile
{

	DECLARE_DYNAMIC(CDBTMemoFile)

// Constructors
public:
	CDBTMemoFile();

// Attributes
public:
	DBT_HEAD	m_DBTHead;	// ��������� ���� ����

// Overridables
public:

// Operations
public:

// Implementation
public:
	~CDBTMemoFile();

	virtual void Create(LPCTSTR lpszName);
	virtual void Open(LPCTSTR lpszName, UINT nOpenFlag);
	virtual void Close();

	// ��� �����
	virtual UINT GetMemoFileType();

	// ������ ���� ����
	virtual void ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, CString& strMemo); 
	virtual void ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, unsigned char** pMemoData);
	// ���������� ���� ����
	virtual void WriteMemoRecord(DBF_ULONG& lOffsetRec, DBF_LONG& nLen, CString& strMemo);

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
	void DeleteMemoRecord(DBF_ULONG lDelRec);
	// �������������� ���� ����
	BOOL UndeleteMemoRecord(DBF_ULONG& lOffsetRec, CString& strMemo);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_DBTMEMOFILE_H__INCLUDED_)

