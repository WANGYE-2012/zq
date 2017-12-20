// MemoFile.h : interface of the CMemoFile class
//
/////////////////////////////////////////////////////////////////////////////
/*
�����:              �������� �.�.
����������:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_MEMOFILE_H__INCLUDED_)
#define ALX_MEMOFILE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBFDef.h"
#include "DBFException.h"


////////////////////////////////////////////////////////////////////////
// CMemoFile - a Memo File

class ALX_EXT_CLASS CMemoFile : public CFile
{

friend class CFPTMemoFile;
friend class CDBTMemoFile;
friend class CSMTMemoFile;

	DECLARE_DYNAMIC(CMemoFile)

// Constructors
public:
	CMemoFile();

// Attributes
public:
	DBT_HEAD*	m_pDBTHead;	// ��������� ���� ����

// Overridables
public:

// Operations
public:

// Implementation
public:
	~CMemoFile();

	virtual void Create(LPCTSTR lpszName);
	virtual void Open(LPCTSTR lpszName, UINT nOpenFlag);
	virtual void Close();
	// ������ �� ����
	virtual BOOL IsOpen() const;
	
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

	BOOL m_bOpen;				// ������ �� ����

	DBF_ULONG	m_nNextBlock;	// ����� ���������� �����
	DBF_LONG	m_nBlockSize;	// ������ �����
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_MEMOFILE_H__INCLUDED_)

