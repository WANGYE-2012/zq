// DBFMemCursorset.h : interface of the CDBFMemCursorset class
//
/////////////////////////////////////////////////////////////////////////////
/*
�����:              �������� �.�.
����������:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_DBFMEMCURSORSET_H__INCLUDED_)
#define ALX_DBFMEMCURSORSET_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBFRecordset.h"

////////////////////////////////////////////////////////////////////////
// CDBFMemCursorset - the result of a query or base table browse

class ALX_EXT_CLASS CDBFMemCursorset : public CDBFRecordset
{
	DECLARE_DYNAMIC(CDBFMemCursorset)

// Constructor
public:
	CDBFMemCursorset(CDBFTableDef* pDBFTable = NULL);

	void Open(LPCTSTR lpszFileName, UINT nOpenFlag = ALX_DBF_USE_SHARED);
	void Close();

// Attributes
public:
	BOOL IsOpen() const;	// ������ �� �����
	BOOL IsBOF() const;		// ������ �� ������
	BOOL IsEOF() const;		// ��������� �� ������

	BOOL IsDeleted();		// ���� ���� �������� �� ��������

	long GetRecordCount();	// ���������� �������

protected:
	DBF_HEAD	m_DBFHead;	// ���������

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

	virtual void Delete();		// �������� �� ��������
	virtual void Undelete();	// ����� ������� �� ��������
	virtual void Remove();		// ������� ������ �� ������

	// Recordset operations
	void Requery();				// �������� ����� �������
	BOOL Save();				// �������� ��������� � ����
	// �������

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
	virtual ~CDBFMemCursorset();

	// Out-of-date functions kept for backward compatability

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	virtual void ThrowDBFException(long nError = NO_ALX_DBF_ERROR, LPCTSTR szDescription = NULL);

	// ������� - ������ �� ���� ����� ������
	BOOL FAR fnFilter(DBF_CHAR* pRec, CDBFFields* pDBFFields);
private:

	DBF_CHAR* GetRecord(long lRecNumber);						// �������� ������ 
	void GetRecord(long lRecNumber, DBF_CHAR* lpBuffer);		// �������� ������
	void SetRecord(long lRecNumber, const DBF_CHAR* lpBuffer);	// �������� ������

	void qsort (int from, int to);						// �������� ���������� �������
	int compare(const void *elem1, const void *elem2 );	// �������� ��������� �������

	DBF_UCHAR*	m_pTableData;	// ��������� �� ������� ������ ��� �������� DBF �������
	CPtrArray	m_RecPtr;		// ������ ���������� �� ������ �������
	CPtrArray	m_NewRecPtr;	// ������ ���������� �� ����� ������ ����������� � �������

	int m_nSortedField;			// ������ ���� �� �������� ������������� �������
	BOOL m_bDesc;				// ���� ����� ����������� �� �������� TRUE

	BOOL m_bShowDeletedRec;		// ���������� �������� �� ���������� �� �������� ������

	DBF_CHAR*	m_pEditRec;		// ��������� �� ����� ������������� ������
	DBF_LONG	m_lLoadRecord;	// ���-�� ������� ����������� �� DBF �������

	// ��������� �� ������� �������
	BOOL (FAR * m_lpfnIsInclude) (CDBFRecordset* pSet);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_DBFMEMCURSORSET_H__INCLUDED_)
