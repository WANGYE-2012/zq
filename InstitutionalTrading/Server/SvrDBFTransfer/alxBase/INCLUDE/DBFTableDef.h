// DBFTableDef.h : interface of the CDBFTableDef class
//
/////////////////////////////////////////////////////////////////////////////
/*
�����:              �������� �.�.
����������:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_DBFTABLEDEF_H__INCLUDED_)
#define ALX_DBFTABLEDEF_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBFDef.h"
#include "DBFFields.h"
#include "DBTMemoFile.h"
#include "FPTMemoFile.h"
#include "SMTMemoFile.h"
#include "DBFException.h"

// ����� ��������������� ������
enum TextConvertDirection
{
	GetText,
	SetText
};

// ������� ������ ���������� � ����� �����
CString alxChangeFileExt(CString& strFileName, CString strNewFileExt);

// ������� ������� � ���������� ��������� �� ����� ���� �����
CMemoFile* alxNewMemoFile(UINT nMemoType);

// ������� ���������� ���������� ��� ����� ���� �����
LPCTSTR alxMemoFileExt(UINT nMemoType);

// ������� ���������� �������� ���� � ����� ������
void alxSetFieldValue(DBF_CHAR* pRec, FIELD_REC* pFieldRec, const COleVariant& varValue,
					  void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection));
// ������� ���������� �������� ���� �� ������ ������
long alxGetFieldValue(DBF_CHAR* pRec, FIELD_REC* pFieldRec, COleVariant& varValue,
					  void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection));

////////////////////////////////////////////////////////////////////////
// CDBFTableDef - a DBF TableDef

class ALX_EXT_CLASS CDBFTableDef : public CObject
{

friend class CDBFRecordset;
friend class CDBFCursorset;
friend class CDBFMemCursorset;

	DECLARE_DYNAMIC(CDBFTableDef)

// Constructors
public:
	CDBFTableDef();

	void Open(LPCTSTR lpszName, UINT nOpenFlag = ALX_DBF_USE_EXCLUSIVE);
	void Close();

// Attributes
public:
	DBF_HEAD	m_DBFHead;	// ��������� �������

	BOOL IsOpen() const;	// ������ �� ����� �������

	CString GetName();		// ���������� ��� ������
	CString GetDBFilePath();// ���������� ������ ���� � DBF �������
	CString GetMemoName();	// ���������� ��� ���� �����
	CString GetMemoFilePath();// ���������� ������ ���� � ���� �����

	CTime GetDateCreated();		// ���� �������� �������
	CTime GetDateLastUpdated();	// ���� ��������� �����������

	long GetRecordCount();	// ���-�� ������� � �������

// Overridables
public:

// Operations
public:
	BOOL CreateTable(LPCTSTR lpszName);	// ������� �������
	void ModifyTable();					// ������������
	void UpdateTable();					// ��������� ���������
	void CancelUpdateTable();			// �����

	void AddField(FIELD_REC* pFieldRec);					// ��������� ����
	void InsertField(short nIndex, FIELD_REC* pFieldRec);	// ��������� ����
	void MoveField(short nIndex, short nNewIndex);			// ���������� ���� �� �������...
	void ModifyField(short nIndex, FIELD_REC* pFieldRec);	// �������� ����
	void DeleteField(short nIndex);							// ������� ����

	short GetFieldCount();	// ���-�� �����

// Implementation
public:
	~CDBFTableDef();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CDBFFields m_DBFFields; // ��������� �����

protected:
	BOOL ChangeOpenFlag(UINT nNewFlag);
	BOOL m_bOpen;				// ������� �� �������
	BOOL m_bNew;				// ������� �� �������
	UINT m_nOpenFlag;			// ������ �������� �����

	CFile m_DBFile;				// DBF �������
	CMemoFile* m_pMemoFile;		// ���� � ���� ������

	enum EditMode
	{
		noMode,
		modify,
		create
	};
	UINT m_nEditMode;

	BOOL ReadHeader();	// ������ ���������
	void WriteHeader(); // ���������� ���������

	void ReadRecord(long lRecNumber, void* lpBuffer);	// ������ ������
	void WriteRecord(long lRecNumber, void* lpBuffer);	// ���������� ������

	// ������ ���� ����
	void ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, CString& strMemo); 
	void ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, unsigned char** pMemoData);
	// ���������� ���� ����
	void WriteMemoRecord(DBF_ULONG& lOffsetRec, DBF_LONG& nLen, CString& strMemo);

	// ���������� �������� ����
	long GetFieldValue(DBF_CHAR* pRec, FIELD_REC* pFieldRec, COleVariant& varValue,
					  void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection));
	// ������� ��������� �������� ����
	void SetFieldValue(DBF_CHAR* pRec, FIELD_REC* pFieldRec, const COleVariant& varValue,
					  void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection));
private:
	// ��������� ������������ ��� ����������� �������
	DBF_HEAD*	m_pOldDBFHead;		// ��������� �� ������ ��������� �������
	CDBFFields* m_pOldDBFFields;	// ��������� ��������� �����
	DBF_INT*	m_pSwapFld;			// ������ ����������� ������������ ������ � ����� �����

	DBF_UCHAR*	m_pData;	// ����� ��� �������� �������� ������ (���� Picture, Binary, General)
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_DBFTABLEDEF_H__INCLUDED_)

