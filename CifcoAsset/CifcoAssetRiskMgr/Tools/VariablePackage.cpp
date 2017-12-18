// VariablePackage.cpp: implementation of the CVariablePackage class.
//
//	���ã�	���岻�������Ļ�����
//	������	1. �������ڽ����������ݿ黺��
//			2. ���ݽṹΪ��Package = PackageHead + Data + Additional(1-3) + Tail
//			3. Dataռ�õĿռ䰴4�ֽ����㲹��������Additional���֡�ÿ��package����4�ֽڶ����
//			4. ��ѭ�Ƚ��ȳ���ԭ��
//			5. ��Ҫ�ⲿ�ӿڣ�
//				SaveDataTo()	����һ�����ݴ��뻺��
//				GetDataFrom()	���ӻ�����ȡ����ǰ���ݿ�
//				GetCurDataLen()	��ȡ��ǰ���ݿ�ĳ���
//				IsEmpty()		�������Ƿ�Ϊ��
//			6. m_bEmptyID
//				����Ҫ����Ϊ���水���λ�����������Ҫͨ��m_bEmptyID�ж��Ƿ�Ϊ��
//			7. �̰߳�ȫ��
//				�������ݺͶ�ȡ���ݵĲ����̰߳�ȫ�����Զ���߳�ͬʱ��ȡ��ͬʱ���档
//			8. ��������ָ�룺GetOff��SetOff��GetOff����SetOff��ʾΪ�գ�GetOff����׷SetOff�������ܳ�������SetOff����׷��GetOff
//			9. ע�⣬�������������ʱ�����̲߳���ȫ�ġ����߳�Ҫע�Ᵽ�� 
//
//	history 
//	20100119	lzq		�������ļ�
//
//////////////////////////////////////////////////////////////////////


#include "VariablePackage.h"
#include "Tools_Ansi.h"
#include "Tools_Win32.h"




/**************************************************************************************************//**
*   @brief				CVariablePackage        
*   @param[in] CacheSize	���ֽ���
*   @param[in] Mode			����ģʽ:
*   - 0 ���̶߳�,���߳�д
*   - 3 ���̶߳�,���߳�д
*******************************************************************************************************/
CVariablePackage::CVariablePackage(int CacheSize, int Mode)
{
	m_pCache=NULL;
	m_CacheSize=0;
	m_GetOff=0;
	m_SetOff=0;
	m_Seq=0;

	m_Mode = Mode;
	m_pSem = NULL;

	if(m_Mode==3)
		m_pSem=CTools_Win32::AllocWinSem();

	m_CacheSize=(CacheSize/4+(CacheSize%4==0?0:1))*4;
	if(m_CacheSize<0) m_CacheSize=0;

	if(m_CacheSize>0)
		m_pCache=(char*)malloc(m_CacheSize);
}

CVariablePackage::~CVariablePackage()
{
	if(m_pCache) 
	{
		free(m_pCache);
		m_pCache=NULL;
	}

	if(m_pSem)
	{
		CTools_Win32::ReleaseWinSem(m_pSem);
		m_pSem=NULL;
	}
}


//	��һ�����ݴ��뱾�ػ����ļ���PackageHeadָ��İ�ͷ��������ݺͰ�β
//	������ݿ���ڻ������ߴ磬����ʧ��
//	��������ԭ��������
//	ע�⣺pDataռ�Ŀռ����DataLen��4�ֽ�ȡ��
bool CVariablePackage::SaveDataTo(VariablePackageHeadStru& PackageHead, const void* pData)
{
	int PackageLen,AdditionalLen;
	DWORD dwTail=TAIL_VARIABLEPACKAGE;
	bool bRlt;

	if(PackageHead.DataLen>0&&!pData) return false;
	if(!m_pCache) return false;

    PackageHead.Prefix=PREFIX_VARIABLEPACKAGE;
	PackageHead.dwCRC=0;
	PackageHead.dwSeq=m_Seq++;

	AdditionalLen = PackageHead.DataLen%4==0 ? 0 : 4-PackageHead.DataLen%4;
	PackageLen=sizeof(PackageHead) + PackageHead.DataLen + AdditionalLen + 4;


	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	if( m_SetOff>=m_GetOff&&m_CacheSize-(m_SetOff-m_GetOff)<=PackageLen||
	    m_SetOff<m_GetOff&&m_GetOff-m_SetOff<=PackageLen)
		bRlt=false;
	else bRlt=true;

	if(bRlt)
	{
		WriteData(m_SetOff,(char*)&PackageHead,sizeof(PackageHead));
		if(PackageHead.DataLen>0)
			WriteData((m_SetOff+sizeof(PackageHead))%m_CacheSize,(char*)pData,PackageHead.DataLen);

		WriteData((m_SetOff+sizeof(PackageHead)+PackageHead.DataLen+AdditionalLen)%m_CacheSize,(char*)&dwTail,sizeof(dwTail));
		m_SetOff=(m_SetOff+PackageLen)%m_CacheSize;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}

//��һ�����ݴ��뻺��
bool CVariablePackage::SaveDataTo(const void* pData,int DataLen,DWORD UserData1,DWORD UserData2,DWORD UserData3,DWORD UserData4)
{
    VariablePackageHeadStru PackageHead;

    memset(&PackageHead,0,sizeof(PackageHead));
    PackageHead.DataLen=DataLen;
    PackageHead.UserData1=UserData1;
    PackageHead.UserData2=UserData2;
    PackageHead.UserData3=UserData3;
    PackageHead.UserData4=UserData4;

    return SaveDataTo(PackageHead,pData);
}


//ȡ��ǰ�������ݳ���
int CVariablePackage::GetCurPackageDataLen(void)                                 
{
	VariablePackageHeadStru	PackageHead;
	int DataLen;

	if(!m_pCache) return 0;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	IgnoreInvalidPackage();

	if(m_GetOff==m_SetOff) DataLen=0;
	else
	{
															//��ʱ��֤��һ���Ϸ���
		PackageHead.DataLen=0;
		ReadData(m_GetOff,(char*)&PackageHead,sizeof(PackageHead));
		DataLen=PackageHead.DataLen;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

    return DataLen;
}


//	���ַ�������ȡ����ǰ���ݿ飬��ɾ�������DataSize����������ʧ��
//	���pData==NULL��ֻȡ��ͷ
bool CVariablePackage::PeekFrom(VariablePackageHeadStru& PackageHead,void* pData,int DataSize)
{
	bool bRlt;

	if(!m_pCache) return false;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	IgnoreInvalidPackage();

	if(m_GetOff==m_SetOff) bRlt=false;
	else bRlt=true;
															//��ʱ��֤��һ���Ϸ���
	ReadData(m_GetOff,(char*)&PackageHead,sizeof(PackageHead));
	if(PackageHead.DataLen>0&&pData&&PackageHead.DataLen>DataSize)
	{
		bRlt=false;											//�������ߴ粻�㣬ֱ�ӷ���false
	}
	else if(PackageHead.DataLen>0&&pData) 
	{
		ReadData((m_GetOff+sizeof(PackageHead))%m_CacheSize,(char*)pData,PackageHead.DataLen);
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}



//���ַ�������ȡ����ǰ���ݿ飬��ɾ�������DataSize����������ʧ��.���pData==NULL��ֻȡ��ͷ
bool CVariablePackage::PeekFrom(void* pData,int DataSize,DWORD* pUserData1,DWORD* pUserData2,DWORD* pUserData3,DWORD* pUserData4)
{
    VariablePackageHeadStru PackageHead;
    bool bRlt;

    bRlt=PeekFrom(PackageHead,pData,DataSize);
    if(!bRlt) return false;

    if(pUserData1) *pUserData1=PackageHead.UserData1;
    if(pUserData2) *pUserData2=PackageHead.UserData2;
    if(pUserData3) *pUserData3=PackageHead.UserData3;
    if(pUserData4) *pUserData4=PackageHead.UserData4;

    return true;
}


//	�ӻ�����ȡ����ǰ���ݿ飬Ȼ��ɾ��֮�����DataSize����������ʧ��
bool CVariablePackage::GetFrom(VariablePackageHeadStru& PackageHead,void* pData,int DataSize)
{
	bool bRlt;
	int AdditionalLen;
	int PackageLen;

	if(!m_pCache) return false;

	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	IgnoreInvalidPackage();

	if(m_GetOff==m_SetOff) bRlt=false;
	else bRlt=true;
															//��ʱ��֤��һ���Ϸ���
	ReadData(m_GetOff,(char*)&PackageHead,sizeof(PackageHead));
	if(PackageHead.DataLen>0&&pData&&PackageHead.DataLen>DataSize)
	{
		bRlt=false;											//�������ߴ粻�㣬ֱ�ӷ���false
	}
	else if(PackageHead.DataLen>0&&pData) 
	{
		ReadData((m_GetOff+sizeof(PackageHead))%m_CacheSize,(char*)pData,PackageHead.DataLen);
	}

	if(bRlt)
	{
		AdditionalLen = PackageHead.DataLen%4==0? 0 : 4-PackageHead.DataLen%4;
		PackageLen=sizeof(PackageHead)+PackageHead.DataLen+AdditionalLen+4;
		m_GetOff=(m_GetOff+PackageLen)%m_CacheSize;
	}

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}


//	�ӻ�����ȡ����ǰ���ݿ飬Ȼ��ɾ��֮�����DataSize����������ʧ��
bool CVariablePackage::GetFrom(void* pData,int DataSize,DWORD* pUserData1,DWORD* pUserData2,DWORD* pUserData3,DWORD* pUserData4)
{
    VariablePackageHeadStru PackageHead;
    bool bRlt;

    bRlt=GetFrom(PackageHead,pData,DataSize);
    if(!bRlt) return false;

    if(pUserData1) *pUserData1=PackageHead.UserData1;
    if(pUserData2) *pUserData2=PackageHead.UserData2;
    if(pUserData3) *pUserData3=PackageHead.UserData3;
    if(pUserData4) *pUserData4=PackageHead.UserData4;

    return true;
}


//	ɾ������
void CVariablePackage::DeleteCurPackage(void)
{
	VariablePackageHeadStru PackageHead;
	GetFrom(PackageHead,NULL,0);
}



//��ջ�����
void CVariablePackage::Empty(void)
{
	if(m_pSem) CTools_Win32::TakeWinSem(m_pSem);

	m_GetOff=m_SetOff=0;

	if(m_pSem) CTools_Win32::GiveWinSem(m_pSem);

}

//��ȡ�������ߴ�
int	CVariablePackage::GetCacheSize(void)
{
	return m_CacheSize;
}


bool CVariablePackage::IsEmpty(void)
{
	return m_GetOff==m_SetOff?true:false;
}

void CVariablePackage::GetPosition(int& GetOff,int& SetOff)
{
    GetOff=m_GetOff;
    SetOff=m_SetOff;
}


//	��ָ��λ�ö�һ�����ݣ��������ݻ����۷�����У�����ݳ���
//	Off�����λ��
//	���ض����ֽڸ���
int CVariablePackage::ReadData(int Off,char* pData,int ReadLen) const
{
	int iLen;

	if(!m_pCache||Off<0||Off>=m_CacheSize||ReadLen>m_CacheSize)
		return 0;

	if(Off+ReadLen>m_CacheSize) 
	{
		iLen=m_CacheSize-Off;
		memcpy(pData,m_pCache+Off,iLen);
		memcpy(pData+iLen,m_pCache,ReadLen-iLen);
	}
	else 
	{
		memcpy(pData,m_pCache+Off,ReadLen);
	}

	return ReadLen;
}


//	��ָ��λ��дһ�����ݣ��������ݻ����۷�����У�����ݳ���
//	Off�����λ��
//	����д�����ݿ�ĸ���
int	CVariablePackage::WriteData(int Off, const char* pData,int WriteLen)
{
	int iLen;

	if(!m_pCache||Off<0||Off>=m_CacheSize||WriteLen>m_CacheSize)
		return 0;

	if(Off+WriteLen>m_CacheSize) 
	{
		iLen=m_CacheSize-Off;
		memcpy(m_pCache+Off,pData,iLen);
		memcpy(m_pCache,pData+iLen,WriteLen-iLen);
	}
	else 
	{
		memcpy(m_pCache+Off,pData,WriteLen);
	}

	return WriteLen;
}





//	������Ч������
//	ִ�����Ҫôm_GetOff==m_SetOff,Ҫô��һ���Ϸ���
void CVariablePackage::IgnoreInvalidPackage(void)
{
	VariablePackageHeadStru	PackageHead;
	int AdditionalLen,PackageLen,ReadLen;
	DWORD dwTail;

	if(!m_pCache) return ;
	if(m_GetOff==m_SetOff) return ;

	while(1)
	{
		ReadLen=ReadData(m_GetOff,(char*)&PackageHead,sizeof(PackageHead));

		if(ReadLen==sizeof(PackageHead)&&PackageHead.Prefix==PREFIX_VARIABLEPACKAGE)
		{
			AdditionalLen = PackageHead.DataLen%4==0 ? 0 : 4-PackageHead.DataLen%4;
			PackageLen=sizeof(PackageHead)+PackageHead.DataLen+AdditionalLen+4;
			dwTail=0;
			ReadData((m_GetOff+PackageLen-4)%m_CacheSize,(char*)&dwTail,4);
		
			if( dwTail==TAIL_VARIABLEPACKAGE&&
				(m_GetOff<m_SetOff && m_GetOff+PackageLen<=m_SetOff ||
				 m_GetOff>m_GetOff && (m_GetOff+PackageLen<m_CacheSize||m_GetOff+PackageLen-m_CacheSize<=m_SetOff))  )
			{
				break;
			}
		}


		m_GetOff+=4;
		if(m_GetOff>=m_CacheSize) m_GetOff=0;

		if(m_GetOff==m_SetOff) break;
	}

	return ;
}




