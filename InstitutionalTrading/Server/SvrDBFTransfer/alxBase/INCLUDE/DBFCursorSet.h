// DBFCursorset.h : interface of the CDBFCursorset class
//
/////////////////////////////////////////////////////////////////////////////
/*
�����:              �������� �.�.
����������:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_DBFCURSORSET_H__INCLUDED_)
#define ALX_DBFCURSORSET_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DBFRecordset.h"

////////////////////////////////////////////////////////////////////////
// CDBFCursorset - the result of a query or base table browse

class ALX_EXT_CLASS CDBFCursorset : public CDBFRecordset
{
	DECLARE_DYNAMIC(CDBFCursorset)

// Constructor
public:
	CDBFCursorset(CDBFTableDef* pDBFTable = NULL);

	void Open(LPCTSTR lpszFileName, UINT nOpenFlag = ALX_DBF_USE_SHARED);
	void Close();

// Attributes
public:
	BOOL IsOpen() const;	// ������ �� �����
	BOOL IsBOF() const;		// ������ �� ������
	BOOL IsEOF() const;		// ��������� �� ������

	long GetRecordCount();	// ���������� �������

protected:
	long m_nCurCursorRecN;	// ������ ������� ������

	BOOL m_bBOF;			
	BOOL m_bEOF;

// Operations
public:
	// Cursor operations
	void MoveNext();		// ������� �� ��������� ������
	void MovePrev();		// ������� �� ���������� ������
	void MoveFirst();		// ������� �� ������ ������
	void MoveLast();		// ������� �� ��������� ������
	void Move(long lOffsetRec); // ������� �� ������ �� ��������

	// �����
	BOOL FindFirst(LPCTSTR lpszFilter);	// ����� ������ ������
	BOOL FindNext(LPCTSTR lpszFilter);	// ����� ��������� ������
	BOOL FindNext();					// ����� ��������� ������
	BOOL FindLast(LPCTSTR lpszFilter);	// ����� ��������� ������
	BOOL FindPrev(LPCTSTR lpszFilter);	// ����� ���������� ������
	BOOL FindPrev();					// ����� ���������� ������

	long GetAbsolutePosition();					// ���������� ������� ���. ������
	void SetAbsolutePosition(long lPosition);	// ������� �� ���������� ������� � ������

	// edit buffer operations
	virtual void AddNew();		// �������� ������
	virtual void Edit();		// �������� ������

	virtual void Update();		// ��������� ���������
	virtual void CancelUpdate();// �����

	virtual void Remove();		// ������� ������ �� ������

	// Recordset operations
	void Requery();				// �������� ����� �������

	// ���������� ���������� ��� ��� ���������� �� �������� ������
	void SetDelete(BOOL bHideDeleteRec = TRUE);
	BOOL GetDelete();
	// �������� ��������� �� ������� �������
	void SetFilter(BOOL (FAR * lpfnIsInclude) (CDBFRecordset* pSet) );
	void SetFilter(LPCTSTR lpszFilter);
	// ����������
	void SortByField(int nIndex, BOOL bDesc = FALSE);
	void SortByField(LPCTSTR lpszName, BOOL bDesc = FALSE);

// Overridables
public:


// Implementation
public:
	virtual ~CDBFCursorset();

	// Out-of-date functions kept for backward compatability

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	virtual void ThrowDBFException(long nError = NO_ALX_DBF_ERROR, LPCTSTR szDescription = NULL);

private:
	void SetRecord(long lRecNumber, const DBF_CHAR* lpBuffer);	// �������� ������

	void qsort (int from, int to);						// �������� ���������� �������
	int compare(const long elem1, const long elem2 );	// �������� ��������� �������

	CArray<long, long> m_RecPtr;// ������ ���������� �� ������ �������

	int m_nSortedField;			// ������ ���� �� �������� ������������� �������
	BOOL m_bDesc;				// ���� ����� ����������� �� �������� TRUE

	BOOL m_bShowDeletedRec;		// ���������� �������� �� ���������� �� �������� ������

	// ��������� �� ������� �������
	BOOL (FAR * m_lpfnIsInclude) (CDBFRecordset* pSet);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_DBFCURSORSET_H__INCLUDED_)
