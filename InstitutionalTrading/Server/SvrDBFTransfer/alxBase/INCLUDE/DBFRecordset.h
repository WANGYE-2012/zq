// DBFRecordset.h : interface of the CDBFRecordset class
//
/////////////////////////////////////////////////////////////////////////////
/*
����?              �������� ??
����������:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_DBFRECORDSET_H__INCLUDED_)
#define ALX_DBFRECORDSET_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBFTableDef.h"
#include "ALXParser.h"

// ����?����������
enum LockModeEnum
{
	LockPessimistic	= 1,
	LockOptimistic	= 2
};
// ��?(��������) ����������
enum LockTypeEnum2
{
	NoLock		= 0,
	LockDBase	= 1,
	LockClipper	= 2,
	LockFoxpro	= 3
};
// ����?������������?
enum EditModeEnum2
{
	noMode,
	edit,
	addnew
};

// ������� ������������ ����?OEM -> ANSI ��?ANSI -> OEM
void FAR alxOemToAnsiConvert(CString& strConvertText, TextConvertDirection enDirection);
// ������� ������������ ����?ANSI -> OEM ��?OEM -> ANSI
void FAR alxAnsiToOemConvert(CString& strConvertText, TextConvertDirection enDirection);

// ������� - ������
BOOL FAR alxFilter(CDBFRecordset* pSet);

// ���������� ������?������?�� ����?
void FAR alxFetchEmptyParserVariable(CDBFFields& DBFFields, CALXParser* pParser);
void FAR alxFetchParserVariable(CDBFRecordset* pSet, CALXParser* pParser);

////////////////////////////////////////////////////////////////////////
// CDBFRecordset - the result of a query or base table browse

class ALX_EXT_CLASS CDBFRecordset : public CObject
{
	DECLARE_DYNAMIC(CDBFRecordset)

friend BOOL FAR alxFilter(CDBFRecordset* pSet);

// Constructor
public:
	CDBFRecordset(CDBFTableDef* pDBFTable = NULL);

	void Open(LPCTSTR lpszFileName, UINT nOpenFlag = ALX_DBF_USE_SHARED);
	void Close();

// Attributes
public:

	BOOL IsOpen() const;	// ������ �� ����?������?
	BOOL IsBOF() const;		// ���� ��? ������? ������ ����?������
	BOOL IsEOF() const;		// ���� ��? ������? ������ ����?��������?
	BOOL IsDeleted() const;	// ���� ���� �������� �� ��������

	CString GetName();			// ���������� ��� ����?
	CString GetDBFilePath();	// ���������� ������ ���� ?DBF ������?

	CTime GetDateCreated();		// ���������� ���� ������? ������?
	CTime GetDateLastUpdated();	// ���������� ���� ��������?����������?

	long GetRecordCount();		// ��?�� ������??������?

// Operations
public:
	// create/modify table operations
	BOOL CreateTable(LPCTSTR lpszFileName);	// ������?������?
	void ModifyTable();						// ������������
	void UpdateTable();						// ��������� ���������
	void CancelUpdateTable();				// ����?

	// create/modify field operations
	void AddField(FIELD_REC* pFieldRec);					// ��������� ����
	void InsertField(short nIndex, FIELD_REC* pFieldRec);	// ��������� ����
	void MoveField(short nIndex, short nNewIndex);			// ���������� ���� �� ������?..
	void ModifyField(short nIndex, FIELD_REC* pFieldRec);	// ����?�� ����
	void DeleteField(short nIndex);							// ������� ����

	// recordset operations
	void MoveNext();			// ������?�� ��������?������
	void MovePrev();			// ������?�� ����������?������
	void MoveFirst();			// ������?�� ������ ������
	void MoveLast();			// ������?�� ��������?������
	void Move(long lRecNumber);	// ������?�� ������ �� ��������

	// ����?
	BOOL FindFirst(LPCTSTR lpszFilter);	// ����?������ ������
	BOOL FindNext(LPCTSTR lpszFilter);	// ����?��������?������
	BOOL FindNext();					// ����?��������?������
	BOOL FindLast(LPCTSTR lpszFilter);	// ����?��������?������
	BOOL FindPrev(LPCTSTR lpszFilter);	// ����?���������� ������
	BOOL FindPrev();					// ����?���������� ������

	long GetAbsolutePosition();					// �������� ���������� ������?��? ������
	void SetAbsolutePosition(long lOffsetRec);	// ������ ��? ������?������ ��? ������� ������?������? ��? ������?

	// edit buffer operations
	virtual void AddNew();		// ��������� ����?������
	virtual void Edit();		// ����?�� ������?������
	virtual void Update();		// ��������� ���������
	virtual void CancelUpdate();// ����?
	virtual void Delete();		// �������� ������ �� ��������
	virtual void Undelete();	// ������?������?

	// locking
	BOOL LockRecord();		// ��������?������?������
	BOOL UnlockRecord();	// ������������ ������?������

	// field operations
	virtual COleVariant GetFieldValue(LPCTSTR lpszName);
	virtual COleVariant GetFieldValue(int nIndex);

	virtual long GetFieldValue(LPCTSTR lpszName, COleVariant& varValue);
	virtual long GetFieldValue(int nIndex, COleVariant& varValue);

	virtual void SetFieldValue(LPCTSTR lpszName, const COleVariant& varValue);
	virtual void SetFieldValue(int nIndex, const COleVariant& varValue);

	// set text convert function
	void SetTextConvertFunction(void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection));

	short GetFieldCount();	// ��?�� ����?

// Overridables
public:
	virtual CString GetDefaultDBName();

	// for recordset field exchange
//	virtual void DoFieldExchange(CDBFFieldExchange* pFX);

// Implementation
public:
	BOOL ChangeOpenFlag(UINT nNewFlag);
	virtual ~CDBFRecordset();


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CDBFTableDef*	m_pTableDef;	// ��������?�� ������?
	CDBFFields		m_DBFFields;	// ��������?����?


protected:
	int m_nOpenType;		// ������ ������? ������?

	BOOL m_bBOF;			
	BOOL m_bEOF;

	long m_nCurTableRecN;	// ������ ������?������ ?������?

	UINT m_nLockMode;		// ����?����������
	UINT m_nLockType;		// ��?(��������) ����������

	UINT m_nEditMode;		// ����?������������?

	// ��������?�� ������?����������?������
	void (FAR * m_lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection);

	CALXParser* m_pFindParser;		// ������ ��������?������
	CALXParser* m_pFilterParser;	// ������ ��������� ������?
	
	virtual void ThrowDBFException(long nError = NO_ALX_DBF_ERROR, LPCTSTR szDescription = NULL);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_DBFRECORDSET_H__INCLUDED_)
