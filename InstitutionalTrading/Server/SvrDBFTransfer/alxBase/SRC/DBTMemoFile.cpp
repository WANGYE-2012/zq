// DBTMemoFile.cpp : implementation of the CDBTMemoFile class
//
/////////////////////////////////////////////////////////////////////////////
/*
�����:              �������� �.�.
����������:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DBTMemoFile.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CDBTMemoFile
IMPLEMENT_DYNAMIC(CDBTMemoFile, CMemoFile)

/* ����������� */
CDBTMemoFile::CDBTMemoFile()
{
	m_nBlockSize = 256;
}

/* ���������� */
CDBTMemoFile::~CDBTMemoFile()
{
	Close();
}

/* ������� ���� ���� */
void CDBTMemoFile::Create(LPCTSTR lpszName)
{
	ASSERT_VALID(this);
	ASSERT(lpszName != NULL);

	// Re-open is invalid
	if (IsOpen())
	{
		ASSERT(FALSE);
		return;
	}

TRY
{
	// ��������� ���� ����
	DBF_CHECK_ERROR(
		CFile::Open(lpszName, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive), ALX_DBF_ERROR_MEMO_CREATE);

	DBF_UCHAR* pFirstBlock = new DBF_UCHAR[512];
	memset(pFirstBlock, 0, 512);
	Write(pFirstBlock, 512);
	delete[] pFirstBlock;

	memset(&m_DBTHead, 0, sizeof(DBT_HEAD));
	strncpy(m_DBTHead.dbf_file_name, GetFileTitle(), 8);
	m_DBTHead.block_size = (DBF_UINT)m_nBlockSize;
	m_DBTHead.next_block = (DBF_ULONG) ceil((double)512/(double)m_DBTHead.block_size);;

	WriteHeader();

	m_bOpen = TRUE;
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();
	Close();
}
END_CATCH

}

/* ��������� ���� ���� */
void CDBTMemoFile::Open(LPCTSTR lpszName, UINT nOpenFlag)
{
	ASSERT_VALID(this);
	ASSERT(lpszName != NULL);

	// ��������� ��� �������� ����
	nOpenFlag = nOpenFlag | CFile::typeBinary;

	// Re-open is invalid
	if (IsOpen())
	{
		ASSERT(FALSE);
		return;
	}


TRY
{

	// ��������� ���� ����
	DBF_CHECK_ERROR(
		CFile::Open(lpszName, nOpenFlag), ALX_DBF_ERROR_MEMO_OPEN);

	// ������ ���������
	DBF_CHECK_ERROR(
		ReadHeader(),ALX_DBF_ERROR_MEMO_HEAD_READ);

	// ��������� ������ �����
	DBF_CHECK_ERROR(m_DBTHead.block_size > 0,ALX_DBF_ERROR_MEMO_BLOCK_SIZE);

	m_bOpen = TRUE;
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();
	Close();
}
END_CATCH

}

/* ��������� ������� */
void CDBTMemoFile::Close()
{
	if(m_bOpen)
		CMemoFile::Close();
}

UINT CDBTMemoFile::GetMemoFileType()
{
	return MEMO_TYPE_DBASEIV;
}

/* ������ ��������� memo ����� */
BOOL CDBTMemoFile::ReadHeader()
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != NULL);

TRY
{
	Seek(0, CFile::begin);

	// ������ ��������� ���� �����
	if(Read(&m_DBTHead, sizeof(DBT_HEAD)) != sizeof(DBT_HEAD))
		return FALSE;

	return TRUE;
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

} END_CATCH

	return FALSE;
}

/* ���������� ��������� ������� */
BOOL CDBTMemoFile::WriteHeader()
{
	ASSERT_VALID(this);
	ASSERT(m_hFile != NULL);

TRY
{
	Seek(0, CFile::begin);

	// ���������� ��������� ���� �����
	Write(&m_DBTHead, sizeof(DBT_HEAD));

	return TRUE;
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

} END_CATCH

	return FALSE;
}

/* ������ ���� ���� */
void CDBTMemoFile::ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, CString& strMemo)
{
	ASSERT_VALID(this);
	ASSERT(lOffsetRec > 0);

	strMemo = "";

	if(!IsOpen())
		return;

TRY
{
	DBF_LONG  nLenMemo = 0;

	Seek(lOffsetRec * m_DBTHead.block_size + sizeof(DBF_LONG),CFile::begin);
	// �������� ����� ����
	DBF_CHECK_ERROR(
		Read(&nLenMemo, sizeof(nLenMemo)) == sizeof(DBF_LONG),
		ALX_DBF_ERROR_MEMO_FIELD_READ);
	
	// �������� ����� ������
	nLenMemo -= sizeof(DBF_LONG) * 2;

	// ��������� ����� ����
	DBF_CHECK_ERROR(nLenMemo > 0,ALX_DBF_ERROR_MEMO_FIELD_READ);

	char* buffer = strMemo.GetBuffer(nLenMemo + 1);

	// ������ ���� ����
	DBF_CHECK_ERROR(
		Read(buffer, nLenMemo) == (UINT)nLenMemo,
		ALX_DBF_ERROR_MEMO_FIELD_READ);

	buffer[nLenMemo] = '\0';
	strMemo.ReleaseBuffer(nLenMemo + 1);

	nLen = nLenMemo;

}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

	ALXThrowDBFException(ALX_DBF_ERROR_MEMO_FIELD_READ);

} END_CATCH

}

/* ������ ���� ���� */
void CDBTMemoFile::ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, unsigned char** pMemoData)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(lOffsetRec > 0);

	*pMemoData = NULL;

	if(!IsOpen())
		return;

TRY
{
	DBF_LONG  nLenMemo = 0;

	Seek(lOffsetRec * m_DBTHead.block_size + sizeof(DBF_LONG),CFile::begin);
	// �������� ����� ����
	DBF_CHECK_ERROR(
		Read(&nLenMemo, sizeof(nLenMemo)) == sizeof(DBF_LONG),
		ALX_DBF_ERROR_MEMO_FIELD_READ);
	
	// �������� ����� ������
	nLenMemo -= sizeof(DBF_LONG) * 2;

	// ��������� ����� ����
	DBF_CHECK_ERROR(nLenMemo > 0,ALX_DBF_ERROR_MEMO_FIELD_READ);

	*pMemoData = new unsigned char[nLenMemo];

	// ������ ���� ����
	DBF_CHECK_ERROR(
		Read(*pMemoData, nLenMemo) == (UINT)nLenMemo,
		ALX_DBF_ERROR_MEMO_FIELD_READ);

	nLen = nLenMemo;

}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

	ALXThrowDBFException(ALX_DBF_ERROR_MEMO_FIELD_READ);

} END_CATCH

}

/* ���������� � ���� ���� */
void CDBTMemoFile::WriteMemoRecord(DBF_ULONG& lOffsetRec, DBF_LONG& nLen, CString& strMemo)
{
	ASSERT_VALID(this);

	if(!IsOpen())
		return;

TRY
{
	DBF_LONG  nLenMemo = 0;

	// ���� ������ ����������
	if(lOffsetRec > 0)
	{
		// ����������� ���-�� ������
		DBF_UINT nNeedBlockCnt = (DBF_UINT) ceil(((double)(strMemo.GetLength() + sizeof(DBF_ULONG) + sizeof(DBF_LONG)))/((double)m_DBTHead.block_size));

		if(strMemo.IsEmpty())
		{
			// ������� ������
			DeleteMemoRecord(lOffsetRec);
			lOffsetRec = 0;
		}
		else
		{
			Seek(lOffsetRec * m_DBTHead.block_size + sizeof(DBF_LONG),CFile::begin);

			// �������� ����� ����
			DBF_CHECK_ERROR(
			Read(&nLenMemo, sizeof(nLenMemo)) == sizeof(DBF_LONG),
			ALX_DBF_ERROR_MEMO_FIELD_READ);
			
			// ���. ���-�� ������ ���������� ���� �����
			DBF_UINT nBlockCnt = (DBF_UINT) ceil(((double)(nLenMemo))/((double)m_DBTHead.block_size));

			// ���� ������ �� �������� �� ��������
			if(nBlockCnt != nNeedBlockCnt)
			{
				// ������� ������
				DeleteMemoRecord(lOffsetRec);
				lOffsetRec = 0;

				// ���� �� ������� ����������� �����-������ ������
				if(!UndeleteMemoRecord(lOffsetRec, strMemo))
					lOffsetRec = AddMemoRecord(strMemo);
			}
			else
				// ��������� ������
				UpdateMemoRecord(lOffsetRec, strMemo);
		}
	}
	else
	{
		// ���� �� ������� ����������� �����-������ ������
		if(!UndeleteMemoRecord(lOffsetRec, strMemo))
			// ��������� ����� ������
			lOffsetRec = AddMemoRecord(strMemo);
	}
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

	ALXThrowDBFException(ALX_DBF_ERROR_MEMO_FIELD_WRITE);

} END_CATCH

}

/* ��������� memo ���� */
DBF_LONG CDBTMemoFile::AddMemoRecord(CString& strMemo)
{
	// ����� ������
	DBF_ULONG lOffsetRec = 0;

TRY
{
	// ���������
	DBF_LONG nSign = MF_USEDREC;
	DBF_LONG nLenMemo = strMemo.GetLength() + sizeof(DBF_LONG) * 2;

	lOffsetRec = (DBF_ULONG) ceil((double)GetLength() / (double)m_DBTHead.block_size);

	Seek(lOffsetRec * m_DBTHead.block_size,CFile::begin);
		
	// ���������� ��������� �����
	Write(&nSign, sizeof(nSign));

	// ���������� ����� ������
	Write(&nLenMemo, sizeof(nLenMemo));
		
	// ���������� ������
	Write((LPCSTR)strMemo, strMemo.GetLength());

	ReadHeader();

	if(m_DBTHead.next_block == lOffsetRec)
	{
		m_DBTHead.next_block = (DBF_ULONG) ceil((double)GetLength() / (double)m_DBTHead.block_size);
		WriteHeader();
	}

	// ����������� �� ������� �����
	SetLength((DBF_ULONG) ceil((double)GetLength() / (double)m_DBTHead.block_size) * m_DBTHead.block_size);
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

} END_CATCH

	return lOffsetRec;
}

/* �������������� memo ���� */
void CDBTMemoFile::UpdateMemoRecord(DBF_ULONG lOffsetRec, CString& strMemo)
{

TRY
{
	// ����� ����
	DBF_LONG nLenMemo = strMemo.GetLength() + sizeof(DBF_LONG) * 2;

	Seek(lOffsetRec * m_DBTHead.block_size + sizeof(DBF_LONG),CFile::begin);
		
	// ���������� ����� ������
	Write(&nLenMemo, sizeof(nLenMemo));
		
	// ���������� ������
	Write((LPCSTR)strMemo, strMemo.GetLength());

}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

} END_CATCH

}

/* ������� memo ���� */
void CDBTMemoFile::DeleteMemoRecord(DBF_ULONG lDelRec)
{

TRY
{
	DBF_LONG  nLenMemo = 0;
	// ������ ������
	DBF_UINT nBlockSize = m_DBTHead.block_size;

	Seek(lDelRec * nBlockSize + sizeof(DBF_ULONG),CFile::begin);
		
	// �������� ����� ����
	DBF_CHECK_ERROR(
		Read(&nLenMemo, sizeof(nLenMemo)) == sizeof(DBF_LONG),
		ALX_DBF_ERROR_MEMO_FIELD_READ);

	// �������� ���-�� ������ ���������� ���� �����
	DBF_LONG nDelBlockCnt = (DBF_LONG) ceil(((double)nLenMemo)/((double)nBlockSize));

	ReadHeader();

	// ���������� ������ � ������ ��������� �������
	DBF_ULONG lPrevFreeRec = 0;
	// ������� ������ � ������ ��������� �������
	DBF_ULONG lCurFreeRec = 0;
	// �������� ����� ������ ������ � ������ ��������� �������
	DBF_ULONG lNextFreeRec = m_DBTHead.next_block;
	// ������ ������ �� �������� �����
	DBF_ULONG lEOFRec = (DBF_ULONG) ceil((double)GetLength() / (double)nBlockSize);

	// ���������� ��� ��������� ������� � ������
	while(lNextFreeRec != lEOFRec && lNextFreeRec != MF_USEDREC)
	{
		DBF_LONG nCurFreeBlockCnt = 1;

		lPrevFreeRec = lCurFreeRec;
		lCurFreeRec = lNextFreeRec;

		Seek(lCurFreeRec * nBlockSize,CFile::begin);

		// �������� ��������� �� ��������� ��������� ������
		DBF_CHECK_ERROR(
			Read(&lNextFreeRec, sizeof(lNextFreeRec)) == sizeof(DBF_ULONG),
			ALX_DBF_ERROR_MEMO_FIELD_READ);

		// �������� ���-�� ����. ������� ������
		DBF_CHECK_ERROR(
			Read(&nCurFreeBlockCnt, sizeof(nCurFreeBlockCnt)) == sizeof(DBF_LONG),
			ALX_DBF_ERROR_MEMO_FIELD_READ);

		// ���� ������� ��. ������ ������� � ���������
		if(lCurFreeRec + nCurFreeBlockCnt == lDelRec)
		{
			Seek(lCurFreeRec * nBlockSize + sizeof(DBF_LONG),CFile::begin);

			// ����������� ���������� ������� ������
			nCurFreeBlockCnt += nDelBlockCnt;
			Write(&nCurFreeBlockCnt, sizeof(nCurFreeBlockCnt));

			return;
		}
		// ���� ������� ��. ������ ������� � ���������
		else if(lDelRec + nDelBlockCnt == lCurFreeRec)
		{
			Seek(lDelRec * nBlockSize,CFile::begin);

			// ���������� ��������� �� ����. ��. ������
			Write(&lNextFreeRec, sizeof(lNextFreeRec));
			// ���������� ���������� ������� ������
			nDelBlockCnt += nCurFreeBlockCnt;
			Write(&nDelBlockCnt, sizeof(nDelBlockCnt));

			return;
		}
	}

	// ���� ������ ��������� ������� �� ������
	if(lPrevFreeRec > 0)
	{
		Seek(lPrevFreeRec * nBlockSize,CFile::begin);

		// ���������� ��������� �� ��������� ������
		Write(&lDelRec, sizeof(lDelRec));

	}
	// ������ ������ ��������� �������
	else
	{
		// ��������� �� ������ ��������� ����
		m_DBTHead.next_block = lDelRec;

		WriteHeader();
	}

	// �������� ���� ��� ���������
	Seek(lDelRec * nBlockSize,CFile::begin);

	// ���������� ��������� �� ������ ���� �� �������� �����
	Write(&lEOFRec, sizeof(lEOFRec));

	// ���������� ���������� ������� ������
	Write(&nDelBlockCnt, sizeof(nDelBlockCnt));

}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

} END_CATCH

}

/* �������� �������� ����� ���� ���� ������ ���������� */
BOOL CDBTMemoFile::UndeleteMemoRecord(DBF_ULONG& lOffsetRec, CString& strMemo)
{

TRY
{
	// ����� ������
	DBF_LONG nLenData = strMemo.GetLength();
	// ����� ���� ���� (� ������ ������� ��������� ����������)
	DBF_LONG nLenMemo = nLenData + sizeof(DBF_LONG) * 2;
	// ������ ������
	DBF_UINT nBlockSize = m_DBTHead.block_size;
	// �������� ����������� ���-�� ������ ��� ������ ���� ����
	DBF_LONG nNeedBlockCnt = (DBF_LONG) ceil(((double)(nLenMemo))/((double)nBlockSize));

	ReadHeader();

	// ���������� ������ � ������ ��������� �������
	DBF_ULONG lPrevFreeRec = 0;
	// ������� ������ � ������ ��������� �������
	DBF_ULONG lCurFreeRec = 0;
	// �������� ����� ������ ������ � ������ ��������� �������
	DBF_ULONG lNextFreeRec = m_DBTHead.next_block;
	// ������ ������ �� �������� �����
	DBF_ULONG lEOFRec = (DBF_ULONG) ceil((double)GetLength() / (double)nBlockSize);

	// ���������� ��� ��������� ������ � ������
	while(lNextFreeRec != lEOFRec && lNextFreeRec != MF_USEDREC)
	{
		DBF_LONG nCurFreeBlockCnt = 1;

		lPrevFreeRec = lCurFreeRec;
		lCurFreeRec = lNextFreeRec;

		Seek(lCurFreeRec * nBlockSize,CFile::begin);

		// �������� ��������� �� ��������� ��������� ������
		DBF_CHECK_ERROR(
			Read(&lNextFreeRec, sizeof(lNextFreeRec)) == sizeof(DBF_ULONG),
			ALX_DBF_ERROR_MEMO_FIELD_READ);

		// �������� ���-�� ����. ������� ������
		DBF_CHECK_ERROR(
			Read(&nCurFreeBlockCnt, sizeof(nCurFreeBlockCnt)) == sizeof(DBF_LONG),
			ALX_DBF_ERROR_MEMO_FIELD_READ);

		// ���� ������� ��. ������ �������� �� ������� (������ ��� �����)
		if(nNeedBlockCnt < nCurFreeBlockCnt)
		{
			lOffsetRec = (lCurFreeRec + nCurFreeBlockCnt - nNeedBlockCnt);

			Seek(lOffsetRec * nBlockSize,CFile::begin);

			// ���������
			DBF_LONG nSign = MF_USEDREC;

			// ���������� ��������� �����
			Write(&nSign, sizeof(nSign));

			// ���������� ����� ������
			Write(&nLenMemo, sizeof(nLenMemo));
		
			// ���������� ������
			Write((LPCSTR)strMemo, strMemo.GetLength());

			Seek(lCurFreeRec * nBlockSize + sizeof(DBF_ULONG),CFile::begin);

			// ��������� ���������� ������� ������
			nCurFreeBlockCnt -= nNeedBlockCnt;
			Write(&nCurFreeBlockCnt, sizeof(nCurFreeBlockCnt));

			return TRUE;
		}
		// ���� ������� ��. ������ �������� �� �������
		else if(nNeedBlockCnt == nCurFreeBlockCnt)
		{
			lOffsetRec = lCurFreeRec;

			Seek(lOffsetRec * nBlockSize,CFile::begin);

			// ���������
			DBF_LONG nSign = MF_USEDREC;

			// ���������� ��������� �����
			Write(&nSign, sizeof(nSign));

			// ���������� ����� ������
			Write(&nLenMemo, sizeof(nLenMemo));
		
			// ���������� ������
			Write((LPCSTR)strMemo, strMemo.GetLength());

			Seek(lPrevFreeRec * nBlockSize,CFile::begin);

			// ��������� ���������� ��. �������
			Write(&lNextFreeRec, sizeof(lNextFreeRec));

			return TRUE;
		}
	}
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

} END_CATCH

	return FALSE;
}

#ifdef _DEBUG
void CDBTMemoFile::AssertValid() const
{
	CMemoFile::AssertValid();
}

void CDBTMemoFile::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CMemoFile::Dump(dc);

	dc << "m_DBTHead.next_block = " << m_DBTHead.next_block;
	dc << "m_DBTHead.dbf_file_name = " << m_DBTHead.dbf_file_name;
	dc << "m_DBTHead.block_size = " << m_DBTHead.block_size;

	dc << "\n";
}
#endif //_DEBUG
