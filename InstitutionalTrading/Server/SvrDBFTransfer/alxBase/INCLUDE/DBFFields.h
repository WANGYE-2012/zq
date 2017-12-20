/////////////////////////////////////////////////////////////////////////////
/*
�����:              �������� �.�.
����������:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_DBFFIELDS_H__INCLUDED_)
#define ALX_DBFFIELDS_H__INCLUDED_

#include "DBFDef.h"

////////////////////////////////////////////////////////////////////////
// CDBFFieldInfo

typedef struct DBFField
{
	CString			m_strName;	// ��� ����
	TCHAR			m_chType;	// ��� ����
	unsigned short	m_nLen;		// ������ ����
	unsigned char	m_nDec;		// ������� �����
} Field;

////////////////////////////////////////////////////////////////////////
// CDBFFields

class ALX_EXT_CLASS CDBFFields
{

friend class CDBFTableDef;
friend class CDBFRecordset;
friend class CDBFCursorset;
friend class CDBFMemCursorset;

public:
	CDBFFields();
	~CDBFFields();

	short GetFieldCount();							// ���-�� ����� � ���������
	FIELD_REC* GetFieldRec(int nIndexFld)  const;	// ���������� ��������� �� ��������� ����

	Field& operator[](int nIndexFld);
	Field& operator[](CString strNameFld);

private:

	FIELD_REC*	m_pFields;		// ��������� �� ��������� �����
	Field		m_CurField;		// ������� ����
	short		m_nFieldCount;	// ���-�� �����
	DBF_CHAR*	m_pCurRec;		// ��������� �� ������� ������
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_DBFFIELDS_H__INCLUDED_)

