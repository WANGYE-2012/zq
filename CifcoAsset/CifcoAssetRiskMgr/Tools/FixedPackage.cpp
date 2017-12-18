// FixedPackage.cpp: implementation of the CFixedPackage class.
//
//	���ã�	���嶨�����Ļ�����
//	������	1. �������ڽ��������ݿ黺��
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

#include "FixedPackage.h"
#include "Tools_Ansi.h"
#include "Tools_Win32.h"



/**************************************************************************************************//**
*   @brief				CBlockCache��Ĺ��캯��        
*   @param[in] BlockSize	ÿ�����ݿ���ֽ���
*   @param[in] BlockCount	���ݿ���ܸ���
*   @param[in] Mode			����ģʽ:
*   - 0 ���̶߳�,���߳�д
*   - 3 ���̶߳�,���߳�д
*******************************************************************************************************/
CFixedPackage::CFixedPackage(int BlockSize, int BlockCount, int Mode)
{
	m_pData=NULL;
	m_BlockSize=0;
	m_BlockCount=0;
	m_GetOrd=0;
	m_SetOrd=0;

	m_Mode = Mode;
	m_pSem = NULL;

	if(m_Mode==3)
		m_pSem=CTools_Win32::AllocWinSem();

	m_BlockSize=BlockSize;
	if(m_BlockSize<0) m_BlockSize=0;
	m_BlockCount=BlockCount;
	if(m_BlockCount<0) m_BlockCount=0;

	if(m_BlockSize*m_BlockCount>0)
		m_pData=(char*)malloc(m_BlockSize*m_BlockCount);
}


//	�����������
CFixedPackage::~CFixedPackage()
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
bool CFixedPackage::SaveDataTo(void *pBlockData)
{
	bool bRlt;

	if(!pBlockData) return false;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	if(m_SetOrd>=m_GetOrd&&m_BlockCount-(m_SetOrd-m_GetOrd)<=1||
	   m_SetOrd<m_GetOrd&&m_GetOrd-m_SetOrd<=1)
	   bRlt=false;
	else bRlt=true;

	if(bRlt)
	{
		WriteData(m_SetOrd,(char*)pBlockData);
		m_SetOrd=(m_SetOrd+1)%m_BlockCount;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}


//	ȡ��ǰ���ݿ�ĸ���
int	CFixedPackage::GetBlockCount(void)
{
	int BlockCount;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	if(m_SetOrd==m_GetOrd) BlockCount=0;
	else if(m_GetOrd<m_SetOrd) BlockCount=m_SetOrd-m_GetOrd;
	else BlockCount=m_BlockCount-m_GetOrd+m_SetOrd;

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return BlockCount;
}


//	���ַ�������ȡ����ǰ���ݿ飬��ɾ��
bool CFixedPackage::PeekFrom(void *pData)
{
	bool bRlt;

	if(!m_pData) return false;
	if(!pData) return false;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	if(m_GetOrd==m_SetOrd) bRlt=false;
	else
	{
		ReadDate(m_GetOrd,(char*)pData);
		bRlt=true;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}



//	�ӻ�����ȡ����ǰ���ݿ飬Ȼ��ɾ��֮
bool CFixedPackage::GetFrom(void *pData)
{
	bool bRlt;

	if(!m_pData) return false;
	if(!pData) return false;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	if(m_GetOrd==m_SetOrd) bRlt=false;
	else
	{
		ReadDate(m_GetOrd,(char*)pData);
		m_GetOrd=(m_GetOrd+1)%m_BlockCount;
		bRlt=true;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}


//	ɾ������
void CFixedPackage::DeleteCurBlock(void)
{
	if(!m_pData) return;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	if(m_GetOrd!=m_SetOrd)
		m_GetOrd=(m_GetOrd+1)%m_BlockCount;

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
}


//��ջ�����
void CFixedPackage::Empty(void)
{
	if(!m_pData) return;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	m_GetOrd=m_SetOrd;

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);
}


bool CFixedPackage::IsEmpty(void)
{
	return m_GetOrd==m_SetOrd?true:false;
}


void CFixedPackage::GetPosition(int& GetOrd,int& SetOrd)
{
    GetOrd=m_GetOrd;
    SetOrd=m_SetOrd;
}



//ȡ��ĳߴ�
int CFixedPackage::GetBlockSize(void)
{
	return m_BlockSize;
}


//	��ָ��λ�ö�һ������
//	iOff�����λ��
//	���ض������ݿ����
bool CFixedPackage::ReadDate(int Ord,char*pData)
{
	if(!m_pData||Ord<0||Ord>=m_BlockCount)
		return false;

	memcpy(pData,m_pData+Ord*m_BlockSize,m_BlockSize);

	return true;
}


//	��ָ��λ��дһ������
//	iOff�����λ��
//	����д�����ݿ�ĸ���
bool CFixedPackage::WriteData(int Ord,char*pData)
{
	if(Ord<0||Ord>=m_BlockCount)					
		return false;			

	if(!m_pData) return false;

	memcpy(m_pData+Ord*m_BlockSize,pData,m_BlockSize);

	return true;
}







