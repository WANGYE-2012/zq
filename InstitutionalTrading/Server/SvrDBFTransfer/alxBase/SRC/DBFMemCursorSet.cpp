// DBFMemCursorset.cpp : implementation of the CDBFMemCursorset class
//
/////////////////////////////////////////////////////////////////////////////
/*
����?              �������� ??
����������:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBFMemCursorset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CDBFMemCursorset
IMPLEMENT_DYNAMIC(CDBFMemCursorset, CDBFRecordset )

/* ����������?*/
CDBFMemCursorset::CDBFMemCursorset(CDBFTableDef* pDBFTable)
{
	m_bDesc = FALSE;

	m_bShowDeletedRec = TRUE;

	m_pTableData = NULL;

	m_pEditRec = NULL;

	m_lLoadRecord = 0;

	m_lpfnIsInclude = NULL;

	m_nCurCursorRecN = -1;

	m_bBOF = TRUE;			
	m_bEOF = TRUE;
}

/* ���������� */
CDBFMemCursorset::~CDBFMemCursorset()
{
	if (IsOpen())
		Close();
}

/* �������� ������?*/
void CDBFMemCursorset::Open(LPCTSTR lpszFileName, UINT nOpenFlag)
{
	ASSERT_VALID(this);

	CString strFileName;

	// �������� ��� ����?
	if(lpszFileName == NULL)
		strFileName = GetDefaultDBName();
	else
		strFileName = lpszFileName;

	ASSERT(strFileName.GetLength() > 4);

	// Re-Opening is invalid.
	if (IsOpen())
	{
		ASSERT(FALSE);
		return;
	}

	m_pTableDef = new CDBFTableDef;

	// ��������?������?
	m_pTableDef->Open(strFileName, nOpenFlag);

	if(!IsOpen())
		return;

	// ��������� ��������?��������??����?
	m_DBFFields = m_pTableDef->m_DBFFields;
	m_DBFHead = m_pTableDef->m_DBFHead;

	// ��������?�� ��?�� ��� �������� ������?
	UINT lFileSize = m_pTableDef->m_DBFile.GetLength();
	m_pTableData = new DBF_UCHAR[lFileSize];

	// ��������?���� ?��?��
	m_pTableDef->m_DBFile.Seek(0, CFile::begin);

#if _MSC_VER > 1200
	DBF_CHECK_ERROR(
		m_pTableDef->m_DBFile.Read(m_pTableData, lFileSize) == lFileSize, ALX_DBF_ERROR_DBFILE_MEMORYLOAD);
#else
	DBF_CHECK_ERROR(
		m_pTableDef->m_DBFile.ReadHuge(m_pTableData, lFileSize) == lFileSize, ALX_DBF_ERROR_DBFILE_MEMORYLOAD);
#endif // _MSC_VER > 1200

	// �������� ���������� ����������?������?
	m_lLoadRecord = m_pTableDef->GetRecordCount();

	// ��������� ������
	Requery();
}

/* ��������?����?������?*/
void CDBFMemCursorset::Close()
{
	CDBFRecordset::Close();

	m_bDesc = FALSE;
	m_bShowDeletedRec = TRUE;

	m_nCurCursorRecN = -1;

	m_bBOF = TRUE;			
	m_bEOF = TRUE;

	// ����������?������?

	if(m_pTableData != NULL) { delete[] m_pTableData; m_pTableData = NULL; }

	m_lLoadRecord = 0;

	if(m_pEditRec != NULL) { delete[] m_pEditRec; m_pEditRec = NULL; }


	m_RecPtr.RemoveAll();
	for(int i = 0; i < 	m_NewRecPtr.GetSize(); i++)
	{
		DBF_UCHAR* pNewRec = (DBF_UCHAR*)m_NewRecPtr[i];
		delete[] pNewRec;
	}
	m_NewRecPtr.RemoveAll();
}

/* ������� ���������� TRUE ���� ��? ���?��? ����? ������ */
BOOL CDBFMemCursorset::IsBOF() const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return (m_nCurCursorRecN == 0);
}

/* ������� ���������� TRUE ���� ��? ���?��? ������?� ������ */
BOOL CDBFMemCursorset::IsEOF() const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return (m_nCurCursorRecN >= m_RecPtr.GetSize() - 1);
}

/* ������� ���������� ���������� ������??������ */
long CDBFMemCursorset::GetRecordCount()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_RecPtr.GetSize();
}

/* ������� ���������� ���������� ������?������ ?������ */
long CDBFMemCursorset::GetAbsolutePosition()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_nCurCursorRecN;
}

/* ������� ������������?���������� ������?������ ?������ */
void CDBFMemCursorset::SetAbsolutePosition(long lPosition)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(lPosition >= 0 && lPosition < GetRecordCount());

	DBF_LONG lCursorRecN = lPosition; 

	// Call Move.
	m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
	m_nCurCursorRecN = lCursorRecN;
	m_bBOF = m_bEOF = FALSE;

}

/* ������?�� ��������?������ */
void CDBFMemCursorset::MoveNext()
{
	ASSERT_VALID(this);
	ASSERT(!IsEOF());

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = m_nCurCursorRecN;
	
	lCursorRecN++;

	if(lCursorRecN < GetRecordCount())
	{
		// Call Move.
		m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
		m_nCurCursorRecN = lCursorRecN;
		m_bBOF = m_bEOF = FALSE;
	}
	else
	{
		m_bEOF = TRUE;
		m_bBOF = GetRecordCount() > 0; 
	}
}

/* ������?�� ���������� ������ */
void CDBFMemCursorset::MovePrev()
{
	ASSERT_VALID(this);
	ASSERT(!IsBOF());

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = m_nCurCursorRecN; 

	lCursorRecN--;

	if(lCursorRecN >= 0)
	{
		// Call Move.
		m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
		m_nCurCursorRecN = lCursorRecN;
		m_bBOF = m_bEOF = FALSE;
	}
	else
	{
		m_bBOF = TRUE;
		m_bEOF = GetRecordCount() > 0; 
	}
}

/* ������?�� ������ ������ ?������ */
void CDBFMemCursorset::MoveFirst()
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = 0; 

	// Call Move.
	m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
	m_nCurCursorRecN = lCursorRecN;
	m_bBOF = m_bEOF = FALSE;

}

/* ������?�� ��������?������ ?������ */
void CDBFMemCursorset::MoveLast()
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = GetRecordCount() - 1; 

	// Call Move.
	m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
	m_nCurCursorRecN = lCursorRecN;
	m_bBOF = m_bEOF = FALSE;

}

/* ������?�� ������ �� �������� ������������ ������?������ */
void CDBFMemCursorset::Move(long lOffsetRec)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = m_nCurCursorRecN + lOffsetRec;
	
	// ������??���������� ����??
	if(lOffsetRec < 0)
	{
		if(lCursorRecN < 0)
		{
			lCursorRecN = 0;
			m_bBOF = TRUE;
		}
		else
			m_bBOF = FALSE;

		m_bEOF = FALSE; 
	}
	// ������??��������?����??
	else if(lOffsetRec > 0)
	{
		if(lCursorRecN >= GetRecordCount())
		{
			lCursorRecN = GetRecordCount() - 1;
			m_bEOF = TRUE;
		}
		else
			m_bEOF = FALSE;

		m_bBOF = FALSE; 
	}

	// Call Move.
	m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
	m_nCurCursorRecN = lCursorRecN;
}

/* ����?������ ������ */
BOOL CDBFMemCursorset::FindFirst(LPCTSTR lpszFilter)
{
	m_nCurCursorRecN = -1;
	return FindNext(lpszFilter);
}

/* ����?��������?������ */
BOOL CDBFMemCursorset::FindNext(LPCTSTR lpszFilter)
{
	if(m_pFindParser != NULL)	delete m_pFindParser;
	m_pFindParser = new CALXParser;

	alxFetchEmptyParserVariable(m_DBFFields, m_pFindParser);

	try
	{
		m_nCurCursorRecN++;

		m_pFindParser->Parse(lpszFilter);
		m_pFindParser->DelUnusedVariables();

		m_nCurCursorRecN--;

		return FindNext();
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}
	return FALSE;
}

/* ����?��������?������ */
BOOL CDBFMemCursorset::FindNext()
{
	ASSERT(m_pFindParser != NULL);

	BOOL bResult = FALSE;
	VARIANT* pvarExprResult = NULL;

	try
	{
		m_nCurCursorRecN++;

		if(m_nCurCursorRecN >= GetRecordCount())
			return FALSE;

		// ���������� ������
		for(; m_nCurCursorRecN < GetRecordCount(); m_nCurCursorRecN++)
		{
			m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);
		
			alxFetchParserVariable(this, m_pFindParser);

			// ��������� ��������?
			pvarExprResult = m_pFindParser->Execute();

			// ���� �� ���������� ��������?
			if(V_VT(pvarExprResult) != VT_BOOL)
				break;

			// ���� ����?
			if(V_BOOL(pvarExprResult) != FALSE)
			{
				bResult = TRUE;
				break;
			}
		}
	
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}

	if(m_nCurCursorRecN >= GetRecordCount())
	{
		m_nCurCursorRecN = GetRecordCount() - 1;
		m_bEOF = TRUE;
	}
	else
		m_bEOF = FALSE;

	m_bBOF = FALSE;

	if(V_VT(pvarExprResult) != VT_BOOL)
		ALXThrowDBFException(ALX_DBF_ERROR_NOT_LOGIC_EXPR);

	return bResult;
}
/* ����?��������?������ */
BOOL CDBFMemCursorset::FindLast(LPCTSTR lpszFilter)
{
	m_nCurCursorRecN = m_RecPtr.GetSize();
	return FindPrev(lpszFilter);
}

/* ����?���������� ������ */
BOOL CDBFMemCursorset::FindPrev(LPCTSTR lpszFilter)
{
	if(m_pFindParser != NULL)	delete m_pFindParser;
	m_pFindParser = new CALXParser;

	alxFetchEmptyParserVariable(m_DBFFields, m_pFindParser);

	try
	{
		m_nCurCursorRecN--;

		m_pFindParser->Parse(lpszFilter);
		m_pFindParser->DelUnusedVariables();

		m_nCurCursorRecN++;

		return FindPrev();
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}
	return FALSE;
}

/* ����?���������� ������ */
BOOL CDBFMemCursorset::FindPrev()
{
	ASSERT(m_pFindParser != NULL);

	BOOL bResult = FALSE;
	VARIANT* pvarExprResult = NULL;

	try
	{
		m_nCurCursorRecN--;

		if(m_nCurCursorRecN < 0)
			return FALSE;

		// ���������� ������
		for(; m_nCurCursorRecN >= 0; m_nCurCursorRecN--)
		{
			m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);
		
			alxFetchParserVariable(this, m_pFindParser);

			// ��������� ��������?
			pvarExprResult = m_pFindParser->Execute();

			// ���� �� ���������� ��������?
			if(V_VT(pvarExprResult) != VT_BOOL)
				break;

			// ���� ����?
			if(V_BOOL(pvarExprResult) != FALSE)
			{
				bResult = TRUE;
				break;
			}
		}
	
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}

	if(m_nCurCursorRecN < 0)
	{
		m_nCurCursorRecN = 0;
		m_bBOF = TRUE;
	}
	else
		m_bBOF = FALSE;

	m_bEOF = FALSE;

	if(V_VT(pvarExprResult) != VT_BOOL)
		ALXThrowDBFException(ALX_DBF_ERROR_NOT_LOGIC_EXPR);

	return bResult;
}

/* ����?�� ��������? ��������??������ */
void CDBFMemCursorset::CancelUpdate()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode != noMode);
	ASSERT(m_pEditRec != NULL);

	m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);

	if(m_pEditRec != NULL)
		delete[] m_pEditRec;

	m_nEditMode = noMode;
}

/* ���������� ����?������. ��� ��������? ��������?����������
   ������?������?- Update(), ��� ������ - CancelUpdate()      */ 
void CDBFMemCursorset::AddNew()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);
	ASSERT(m_pEditRec == NULL);

	m_nEditMode = addnew;

	if(m_pEditRec != NULL)
		delete[] m_pEditRec;

	// ����?�� ��?�� ��� ����?������
	m_pEditRec = new DBF_CHAR[m_DBFHead.rec_size];

	memset(m_pEditRec, 0, m_DBFHead.rec_size);

	COleVariant varEmpty;
	varEmpty.Clear();
	// ����������?���� ��??������ ��������
	for(short i = 0; i < m_DBFFields.GetFieldCount(); i++)
	{
		FIELD_REC* pFieldRec = m_DBFFields.GetFieldRec(i);
		alxSetFieldValue(m_pEditRec, pFieldRec, varEmpty, m_lpfnTextConvert);
	}

	m_pEditRec[0] = REC_FLAG_NORMAL;

	m_DBFFields.m_pCurRec = m_pEditRec;
}

/* ��������?������. ��� ��������? ��������?����������
   ������?������?- Update(), ��� ������ - CancelUpdate() */ 
void CDBFMemCursorset::Edit()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);
	ASSERT(m_pEditRec == NULL);

	m_nEditMode = edit;

	if(m_pEditRec != NULL)
		delete[] m_pEditRec;

	// ����?�� ��?�� ��� ����?������
	m_pEditRec = new DBF_CHAR[m_DBFHead.rec_size];

	// �������� ������
	memcpy(m_pEditRec, m_DBFFields.m_pCurRec, m_DBFHead.rec_size);

	m_DBFFields.m_pCurRec = m_pEditRec;
}

/* ���������� ���������� ������ */
void CDBFMemCursorset::Update()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode != noMode);
	ASSERT(m_pEditRec != NULL);

	switch(m_nEditMode)
	{
	case edit:
		{
			SetRecord(m_nCurCursorRecN, m_pEditRec);
			delete [] m_pEditRec;
			break;
		}
	case addnew:
		{
			m_DBFHead.last_rec++;
			m_NewRecPtr.Add(m_pEditRec);
			m_RecPtr.Add(m_pEditRec);
			m_nCurCursorRecN = max(0,GetRecordCount() - 1);
			SetRecord(m_nCurCursorRecN, m_pEditRec);
			m_bBOF = m_bEOF = FALSE;
			break;
		}
	}
	
	m_pEditRec = NULL;

	m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);

	m_nEditMode = noMode;
}

/* ������ ��������? �� �������� */
void CDBFMemCursorset::Delete()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_DBFFields.m_pCurRec[0] = REC_FLAG_DELETED;

	SetRecord(m_nCurCursorRecN, m_DBFFields.m_pCurRec);
}

/* ������� ���������� TRUE ���� ������ ���� �������� ��� ������? */
BOOL CDBFMemCursorset::IsDeleted()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return (m_DBFFields.m_pCurRec[0] == REC_FLAG_DELETED);
}

/* ������?������?�� �������� */
void CDBFMemCursorset::Undelete()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_DBFFields.m_pCurRec[0] = REC_FLAG_NORMAL;

	SetRecord(m_nCurCursorRecN, m_DBFFields.m_pCurRec);
}

/* ������� ��������?�� ������. ����?������������?������ ���� ������?
   ���������� ������?������?Save(). ������ ����?������������ ����?Requery() */
void CDBFMemCursorset::Remove()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_DBFHead.last_rec--;
	m_RecPtr.RemoveAt(m_nCurCursorRecN);

	if(m_nCurCursorRecN >= GetRecordCount() && m_nCurCursorRecN > 0)
		m_nCurCursorRecN--;

	m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);
}

/* ������� ���������� TRUE ���� ����?������?��?������?������ */
BOOL CDBFMemCursorset::IsOpen() const
{
	ASSERT_VALID(this);
	return (m_pTableDef == NULL ? FALSE : m_pTableDef->IsOpen());
}

/* ������� ����������?���������� */
void CDBFMemCursorset::ThrowDBFException(long nALXDBFError, LPCTSTR szDescription)
{
	ASSERT_VALID(this);

	ALXThrowDBFException(nALXDBFError, szDescription);
}

#ifdef _DEBUG
void CDBFMemCursorset::AssertValid() const
{
	CDBFRecordset::AssertValid();
}

void CDBFMemCursorset::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CDBFRecordset::Dump(dc);

	dc << "\n";
}
#endif //_DEBUG

/* ������� ���������� ��������?�� ������ */
DBF_CHAR* CDBFMemCursorset::GetRecord(long lRecNumber)
{
	ASSERT(lRecNumber < m_RecPtr.GetSize());
	return (DBF_CHAR*)m_RecPtr[lRecNumber];
}

/* ������� ���������� ��������?�� ������ */
void CDBFMemCursorset::GetRecord(long lRecNumber, DBF_CHAR* lpBuffer)
{
	ASSERT(lRecNumber < m_RecPtr.GetSize());
}

/* ������� �������� ������ �� ���������� ������ ?����?������
   ����������? ��� ��������� ��������??����?DBF ������?   */
void CDBFMemCursorset::SetRecord(long lRecNumber, const DBF_CHAR* lpBuffer)
{
	ASSERT(lRecNumber < m_RecPtr.GetSize());
	memcpy(m_RecPtr[lRecNumber],lpBuffer,m_DBFHead.rec_size);
}

/* ���������� �� ���������� ���� */
void CDBFMemCursorset::SortByField(LPCTSTR lpszName, BOOL bDesc /* = FALSE */)
{
    short nIndex;
	for(short nIndex = 0;
		strcmp(m_DBFFields.m_pFields[nIndex].field_name, lpszName) != 0 &&
		nIndex <= m_DBFFields.m_nFieldCount;
		nIndex++)
	
	ASSERT(nIndex < m_DBFFields.m_nFieldCount);

	SortByField(nIndex, bDesc);
}

/* ���������� �� ���������� ���� */
void CDBFMemCursorset::SortByField(int nIndex, BOOL bDesc /* = FALSE */)
{
	m_nSortedField = nIndex;

	m_bDesc = bDesc;

	if(GetRecordCount() > 1)
	{
		FIELD_REC* pFieldRec = m_DBFFields.GetFieldRec(nIndex);

		// �� �������� ��??�� ��������?
		switch(pFieldRec->field_type)
		{
		case FLD_TYPE_DOUBLE: // �������� FLD_TYPE_BINARY
			// ���� FLD_TYPE_DOUBLE
			if(pFieldRec->len_info.num_size.len == sizeof(DBF_DOUBLE))
				break;
		case FLD_TYPE_GENERAL:
		case FLD_TYPE_PICTURE:
			return;
		}

		qsort( 0, m_RecPtr.GetSize() - 1 );

		m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);
	}
}

/* ������� ��������� ���� ������?(������ ��? �� ��������
   ��������?��? ��������?������? m_nSortedField)       */
int CDBFMemCursorset::compare(const void *elem1, const void *elem2)
{
	// if elem1 < elem2 return TRUE else return FALSE
	COleVariant var1, var2;
	BOOL bResult;

	// ���� �� ��������
	if(m_bDesc)
	{
		// �������� ������? ����?(������ ��?�� �� ������?
		m_DBFFields.m_pCurRec = (DBF_CHAR*)elem1;
		var2 = GetFieldValue(m_nSortedField);
		m_DBFFields.m_pCurRec = (DBF_CHAR*)elem2;
		var1 = GetFieldValue(m_nSortedField);
	}
	else
	{
		// �������� ������? ����?
		m_DBFFields.m_pCurRec = (DBF_CHAR*)elem1;
		var1 = GetFieldValue(m_nSortedField);
		m_DBFFields.m_pCurRec = (DBF_CHAR*)elem2;
		var2 = GetFieldValue(m_nSortedField);
	}
	
	// ���������� ������? ����?
	if(var1 == var2)
		bResult = FALSE;
	else
	{
		if(var1.vt == VT_EMPTY && var2.vt != VT_EMPTY)
			bResult = TRUE;
		else if (var2.vt == VT_EMPTY && var1.vt != VT_EMPTY)
			bResult = FALSE;
		else
		{
			switch(var1.vt)
			{
			case VT_BSTR:
				{
					CString str1 = var1.bstrVal;
					CString str2 = var2.bstrVal;
					bResult = (str1 < str2);
					break;
				}
			case VT_R8:
				{
					bResult = (var1.dblVal < var2.dblVal);
					break;
				}
			case VT_I4:
				{
					bResult = (var1.lVal < var2.lVal);
					break;
				}
			case VT_CY:
				{
					COleCurrency cur1, cur2;
					cur1.m_cur = var1.cyVal;
					cur2.m_cur = var2.cyVal;
					bResult = (cur1 < cur2);
					break;
				}
			case VT_DATE:
				{
					bResult = (var1.date < var2.date);
					break;
				}
			case VT_BOOL:
				{
					bResult = (var1.boolVal < var2.boolVal);
					break;
				}
			}
		}
	}

	return bResult;
}

/* �������� ���������� ����?*/
void CDBFMemCursorset::qsort(int from, int to)
{
    int i, j;
	void *x, *tmp;

//	int from	= 0;					/* ����?��������?������ */
//	int to		= m_RecPtr.GetSize();	/* ������ �������� ������ */

	if( from >= to ) return;
	/* ����?��������?<= 1 */

	i = from; j = to;
	x = m_RecPtr[ (i+j) / 2 ];     /* �������� �� �������� */

	do
	{
		/* ������?������ */
		while( compare(m_RecPtr[i], x) ) i++ ;	// m_RecPtr[i] < x

		/* ������?����?*/
		while( compare(x, m_RecPtr[j]) ) j--;	// x < m_RecPtr[j] 

		if( i <= j )
		{   /* ����?�� */
			tmp = m_RecPtr[i]; m_RecPtr[i] = m_RecPtr[j] ; m_RecPtr[j] = tmp;
			i++;  j--;
		}
	} while( i <= j );

            /* ������ ��?����?������??����?����?
             * ����?����?����?= j - from + 1
             *       ������      = to - i   + 1
             * ��?����??����?����?������ ���� ����??������.
             * ������ ���� ������ ������������?������ ����??����������?
             * ������?��������?����?�������� (��� �������� ��?��
             * ?����?). ������?:
             */
            if( (j - from)  <  (to - i) ){
                    qsort( from, j );
                    qsort( i, to   );
            } else {
                    qsort( i,   to );
                    qsort( from, j );
            }
}

/* ������������?������ ������ ����?(����������
   ��?��?���������� �� �������� ������)        */
void CDBFMemCursorset::SetDelete(BOOL bHideDeleteRec /* = TRUE */)
{
	m_bShowDeletedRec = !bHideDeleteRec;
}

/* ������?�� ������ ������ ����?(����������?
   ��?��?���������� �� �������� ������)        */
BOOL CDBFMemCursorset::GetDelete()
{
	return !m_bShowDeletedRec;
}

/* ��������� ������ */
void CDBFMemCursorset::Requery()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	m_nEditMode = noMode;

	m_RecPtr.SetSize(m_lLoadRecord + m_NewRecPtr.GetSize());

	long nRec = 0;
	// ���� ���������� ���������� �� �������� ������
	if(m_bShowDeletedRec)
	{
		// ���������� ��?����������?������
		for(long i = 0; i < m_lLoadRecord; i++)
		{
			m_RecPtr[nRec] =  m_pTableData + m_DBFHead.data_offset + m_DBFHead.rec_size * i;
			// ���� ������?������� ������?
			if(m_lpfnIsInclude != NULL)
			{
				// ���� ������ ������������� ������?������?
				if((*m_lpfnIsInclude)(this))
					nRec++;
			}
			else
				nRec++;
		}
		// ���������� ����������?����?�������� ������
		for(long j = 0; j < m_NewRecPtr.GetSize(); j++)
		{
			m_RecPtr[nRec] = m_NewRecPtr[j];
			// ���� ������?������� ������?
			if(m_lpfnIsInclude != NULL) 
			{
				// ���� ������ ������������� ������?������?
				if((*m_lpfnIsInclude)(this))
					nRec++;
			}
			else
				nRec++;
		}
	}
	else
	{
		// ���������� ��?����������?������
		for(long i = 0; i < m_lLoadRecord; i++)
		{
			if(REC_FLAG_DELETED != *(DBF_CHAR*)(m_pTableData + m_DBFHead.data_offset + m_DBFHead.rec_size * i))
			{
				m_RecPtr[nRec] =  m_pTableData + m_DBFHead.data_offset + m_DBFHead.rec_size * i;
				// ���� ������?������� ������?
				if(m_lpfnIsInclude != NULL) 
				{
					// ���� ������ ������������� ������?������?
					if((*m_lpfnIsInclude)(this))
						nRec++;
				}
				else
					nRec++;
			}
		}
		// ���������� ����������?����?�������� ������
        long i;
		for(i = 0; i < m_NewRecPtr.GetSize(); i++)
		{
			if(REC_FLAG_DELETED != *(DBF_CHAR*)(m_NewRecPtr[i]))
			{
				m_RecPtr[nRec] = m_NewRecPtr[i];
				// ���� ������?������� ������?
				if(m_lpfnIsInclude != NULL) 
				{
					// ���� ������ ������������� ������?������?
					if((*m_lpfnIsInclude)(this))
						nRec++;
				}
				else
					nRec++;
			}
		}
	}

	m_RecPtr.SetSize(nRec);
	
	m_DBFHead.last_rec = m_RecPtr.GetSize();

	if(GetRecordCount() > 0)
		MoveFirst();
	else
		m_nCurCursorRecN = 0;
}

// ������ ��������??���� */
BOOL CDBFMemCursorset::Save()
{
	CFile* pDBFile = &m_pTableDef->m_DBFile;
/*	if (!SaveFile.Open(lpszPathName, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareDenyNone))
	{
//		ReportSaveLoadException(lpszPathName, &fe,
//			TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}
*/
	DBF_CHAR* pRec;
	pDBFile->Seek(m_DBFHead.data_offset, CFile::begin);
	// ��������� ������ �� ������?
    long i,j;
	for(long i = 0; i < m_lLoadRecord; i++)
	{
		pRec = (DBF_CHAR*)(m_pTableData + m_DBFHead.data_offset + m_DBFHead.rec_size * i);
		pDBFile->Write(pRec, m_DBFHead.rec_size);
	}
	// ��������� ����?������
	for(long j = 0; j < m_NewRecPtr.GetSize(); j++)
	{
		pRec = (DBF_CHAR*) m_NewRecPtr[j];
		pDBFile->Write(pRec, m_DBFHead.rec_size);
	}

	char eof = REC_EOF;
	// ���������� ������?����?DBF ����?
	pDBFile->Write(&eof, 1);

	// ����?�� ��������?��������?
	COleDateTime curDate = COleDateTime::GetCurrentTime();
	if(m_DBFHead.dbf_id == 0x30) // Visual FoxPro
		m_DBFHead.last_update[0] = atoi(curDate.Format("%y"));
	else
		m_DBFHead.last_update[0] = curDate.GetYear() - ALX_DBF_CENTURY;

	m_DBFHead.last_update[1] = curDate.GetMonth();
	m_DBFHead.last_update[2] = curDate.GetDay();
	m_DBFHead.last_rec = i + j;

	pDBFile->Seek(0, CFile::begin);
	// ���������� ��������?
	pDBFile->Write(&m_DBFHead, sizeof(DBF_HEAD));
	// ���������� ������ ����?
	for(i = 0; i < m_DBFFields.GetFieldCount(); i++)
		pDBFile->Write(m_DBFFields.GetFieldRec(i), sizeof(FIELD_REC));
	char end = 0x0D;
	pDBFile->Write(&end, 1);
	// �������� ������ ����?
	pDBFile->SetLength(m_DBFHead.data_offset + m_DBFHead.last_rec * m_DBFHead.rec_size + 1);

	m_pTableDef->m_DBFHead.last_rec = m_DBFHead.last_rec;

	return TRUE;
}

/* �������� ��������?�� ������?������? ����?������ ������ ����?�������� NULL */
void CDBFMemCursorset::SetFilter(BOOL (FAR * lpfnIsInclude) (CDBFRecordset* pSet) )
{
	m_lpfnIsInclude = lpfnIsInclude;
}

void CDBFMemCursorset::SetFilter(LPCTSTR lpszFilter)
{
	if(lpszFilter == NULL)
	{
		m_lpfnIsInclude = NULL;
		return;
	}

	if(m_pFilterParser != NULL)	delete m_pFilterParser;

	m_pFilterParser = new CALXParser;

	alxFetchEmptyParserVariable(m_DBFFields, m_pFilterParser);

	try
	{
		m_pFilterParser->Parse(lpszFilter);
		m_pFilterParser->DelUnusedVariables();

		m_lpfnIsInclude = alxFilter;
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}
}