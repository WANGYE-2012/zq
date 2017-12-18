// StreamCache.cpp: implementation of the CMemoryCache class.
//
//	���ã�	�����ַ���������
//	������	1. �������ڽ��ַ�������
//			2. ��ѭ�Ƚ��ȳ���ԭ��
//			3. ��Ҫ�ⲿ�ӿڣ�
//				SaveDataTo()	����һ�����ݴ��뻺��
//				GetDataFrom()	���ӻ�����ȡ����ǰ���ݿ�
//				GetCurDataLen()	��ȡ��ǰ���ݿ�ĳ���
//				IsEmpty()		�������Ƿ�Ϊ��
//			4. m_bEmptyID
//				����Ҫ����Ϊ���水���λ�����������Ҫͨ��m_bEmptyID�ж��Ƿ�Ϊ��
//			5. �̰߳�ȫ��
//				�������ݺͶ�ȡ���ݵĲ����̰߳�ȫ�����Զ���߳�ͬʱ��ȡ��ͬʱ���档
//			6. ��������ָ�룺GetOff��SetOff��GetOff����SetOff��ʾΪ�գ�GetOff����׷SetOff�������ܳ�������SetOff����׷��GetOff
//			7. ע�⣬�������������ʱ�����̲߳���ȫ�ġ����߳�Ҫע�Ᵽ�� 
//
//	history 
//	20100119	lzq		�������ļ�
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "MemoryCache.h"
#include "Tools_Ansi.h"
#include "Tools_Win32.h"


#pragma unmanaged

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**************************************************************************************************//**
*   @brief				CMemoryCache��Ĺ��캯��        
*   @param[in] CacheSize	��������С
*   @param[in] Mode			����ģʽ:
*   - 0 ���̶߳�,���߳�д
*   - 3 ���̶߳�,���߳�д
*******************************************************************************************************/
CMemoryCache::CMemoryCache(int CacheSize,int Mode)
{
	m_pData=NULL;
	m_CacheSize=CacheSize;
	if(m_CacheSize<0) m_CacheSize=0;
	m_GetOff=0;
	m_SetOff=0;

	m_Mode = Mode;
	m_pSem = NULL;

	if(m_Mode==3)
		m_pSem=CTools_Win32::AllocWinSem();

	if(m_CacheSize>0)
	{
		m_pData=(char*)malloc(m_CacheSize*2+1);
		if(m_pData)
			memset(m_pData,0,m_CacheSize*2+1);
	}

}


CMemoryCache::~CMemoryCache()
{
	if(m_pData) 
	{
		free(m_pData);
		m_pData=NULL;
	}

	if(m_pSem)
	{
		CTools_Win32::ReleaseWinSem(m_pSem);
		m_pSem=NULL;
	}

}


//	��һ�����ݴ��뱾�ػ����ļ�
//	������ݿ���ڻ������ߴ磬����ʧ��
//	��������ԭ��������
bool CMemoryCache::SaveDataTo(void *pData,int DataLen)
{
	bool bRlt;
	int	CmpOff;														//���ڱȽϵ����ݿ�дλ��
															
	if(!pData) return false;
	if(DataLen>m_CacheSize) return false;															//����̫���ˣ��Ų���
	if(DataLen<0) return false;
	if(DataLen==0) return true;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	if(m_SetOff<m_GetOff) CmpOff=m_GetOff;
	else CmpOff=m_GetOff+m_CacheSize;
	if(m_SetOff+DataLen>=CmpOff) bRlt=false;
	else
	{
		WriteData_Internal(m_SetOff,pData,DataLen);
		m_SetOff=(m_SetOff+DataLen)%m_CacheSize;
		bRlt=true;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}

//ȡ��ǰ�������ݳ���
int CMemoryCache::GetLen(void)
{
	int Len=0;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	Len=GetLen_Internal();

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

    return Len;
}


//ȡ�Ӵ���λ�ã����뵱ǰλ�õľ��룬�Ӵ���0��β
//���û���Ӵ�������-1
int	CMemoryCache::GetPos_SubStr(const char* pSubStr)
{
	int Pos=0,SubLen,ContentLen;
	char *pp;

	if(!m_pData) return -1;
	SubLen=strlen(pSubStr);
	if(SubLen>m_CacheSize) return -1;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	pp=strstr(m_pData+m_GetOff,pSubStr);
	if(!pp) Pos=-1;
	else Pos=pp-(m_pData+m_GetOff);

	ContentLen=GetLen_Internal();

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	if(Pos==-1) return -1;
	if(Pos+SubLen>ContentLen) return -1;
	return Pos;
}


//�ӱ��ػ�����ȡ��ָ�����ȵ����ݣ���ɾ��
bool CMemoryCache::PeekFrom(void *pData,int PeekLen)
{
	bool	bRlt=false;
	int		CurLen;

	if(!m_pData) return false;
	if(!pData) return false;
	if(PeekLen==0) return true;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	CurLen=GetLen_Internal();
	if(PeekLen>CurLen) bRlt=false;
	else 
	{
		ReadDate_Internal(m_GetOff,pData,PeekLen);
		bRlt=true;
	}


	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}


//�ӱ��ػ�����ȡ��ָ�����ȵ����ݣ����ӻ�����ɾ��
bool CMemoryCache::GetFrom(void *pData,int GetLen)
{
	bool	bRlt=false;
	int		CurLen;

	if(!m_pData) return false;
	if(!pData) return false;
	if(GetLen==0) return true;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	CurLen=GetLen_Internal();
	if(GetLen>CurLen) bRlt=false;
	else 
	{
		ReadDate_Internal(m_GetOff,pData,GetLen);
		m_GetOff=(m_GetOff+GetLen)%m_CacheSize;
		bRlt=true;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}

//ɾ��ָ�����ȵ�����
void CMemoryCache::DeleteData(int DeleteLen)
{
	bool	bRlt=false;
	int		CurLen;

	if(DeleteLen<=0) return;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	CurLen=GetLen_Internal();
	if(DeleteLen<0) DeleteLen=0;
	else if(DeleteLen>CurLen) DeleteLen=CurLen;

	m_GetOff=(m_GetOff+DeleteLen)%m_CacheSize;

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
}


//��ջ��������ɶ�ȡ�̵߳���
void CMemoryCache::Empty(void)
{
	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);
	m_GetOff=m_SetOff;
	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
}


bool CMemoryCache::IsEmpty(void)
{
	return m_GetOff==m_SetOff?true:false;
}


void CMemoryCache::GetPosition(int& GetOrd,int& SetOrd)
{
    GetOrd=m_GetOff;
    SetOrd=m_SetOff;
}



//ȡCache�ĳߴ�
int CMemoryCache::GetCacheSize(void)
{
	return m_CacheSize;
}


//ȡ��ǰ�������ݳ��ȣ��ڲ�����
int CMemoryCache::GetLen_Internal(void)
{
	if(m_SetOff==m_GetOff) return 0;
	else if(m_GetOff<m_SetOff) return m_SetOff-m_GetOff;
	else return m_CacheSize-m_GetOff+m_SetOff;
}



//	��ָ��λ�ö�һ������
//	iOff�����λ��
//	���ض��������ݳ���
//	����д����ʱд���������ݣ����Զ�����ʱ���ÿ��ǻ���
int	CMemoryCache::ReadDate_Internal(int iOff,void*pData,int ReadLen)
{
	if(!m_pData||iOff<0||iOff>=m_CacheSize||ReadLen<0||ReadLen>m_CacheSize)			//�ߴ����
		return 0;

	memcpy(pData,m_pData+iOff,ReadLen);
	return ReadLen;
}


//	��ָ��λ��дһ������
//	iOff�����λ��
//	����д�����ݵĳ���
//	��Ҫд��������
int	CMemoryCache::WriteData_Internal(int iOff,void*pData,int WriteLen)
{
	if(iOff<0||iOff>=m_CacheSize||WriteLen<0||WriteLen>m_CacheSize)					
		return 0;			

	if(!m_pData) return 0;
	if(iOff+WriteLen<=m_CacheSize)												//û�дﵽ��������ߴ�
	{
		memcpy(m_pData+iOff,pData,WriteLen);
		memcpy(m_pData+m_CacheSize+iOff,pData,WriteLen);
	}
	else
	{																			//�ﵽ�˻�������߽磬���δ���
		memcpy(m_pData+iOff,pData,m_CacheSize-iOff);
		memcpy(m_pData,(char*)pData+m_CacheSize-iOff,WriteLen-(m_CacheSize-iOff));

		memcpy(m_pData+m_CacheSize+iOff,pData,m_CacheSize-iOff);
		memcpy(m_pData+m_CacheSize,(char*)pData+m_CacheSize-iOff,WriteLen-(m_CacheSize-iOff));
	}
	return WriteLen;
}

