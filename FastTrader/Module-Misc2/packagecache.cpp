//*******************************************************************************************
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
//			6. ����ӿڣ�
//				SetCacheParam()	�����û�������������ļ����棬�����ļ��������ߴ磻�����ڴ滺�棬�������ߴ�
//			7. m_bEmptyID
//				����Ҫ����Ϊ���水���λ�����������Ҫͨ��m_bEmptyID�ж��Ƿ�Ϊ��
//			8. ���̰߳�ȫ��
//				�������ݺͶ�ȡ���ݵĲ������Էֱ���������߳�������ܶ���߳�ͬʱ��ȡ��ͬʱ���档
//			9. ��������ָ�룺GetOff��SetOff��GetOff����SetOff��ʾΪ�գ�GetOff����׷SetOff�������ܳ�������SetOff����׷��GetOff
//
//	history 
//	20100119	lzq		�������ļ�
//
//*******************************************************************************************
#include <stdafx.h>
#include <time.h>
#include <stdlib.h>

//using namespace std;
#include "packagecache.h"


#define GuardRead()  MutexLockGuradEx lock(m_pReadMutexLock)											//������
#define GuardWrite() MutexLockGuradEx lock(m_pWriteMutexLock)											//д����

//	��Ĺ��캯��
CPackageCache::CPackageCache(int CacheSize, int Mode)
{
	m_pCache=NULL;
	m_CacheSize=0;
	m_GetOff=0;
	m_SetOff=0;

	m_Mode = Mode;
	m_pReadMutexLock=NULL;
	m_pWriteMutexLock=NULL;

	switch(m_Mode)
	{
		case 0://���̶߳�,���߳�д
			break;
		case 1://���̶߳�,���߳�д
			m_pWriteMutexLock = new MutexLock();
			break;
		case 2://���̶߳�,���߳�д
			m_pReadMutexLock = new MutexLock();
			break;
		case 3://���̶߳�,���߳�д
		default:
            //next gragment fixed by liao, 20101011
			m_pReadMutexLock = new MutexLock();
            m_pWriteMutexLock = m_pReadMutexLock;
//			m_pReadMutexLock = new MutexLock();
//			m_pWriteMutexLock = new MutexLock();
			break;
	}

	SetCacheParam(CacheSize);
}


//	�����������
CPackageCache::~CPackageCache()
{
	if(m_pCache) 
	{
		free(m_pCache);
		m_pCache=NULL;
	}

    //next fragment added by liao, 20101011
    if(m_pReadMutexLock&&m_pReadMutexLock==m_pWriteMutexLock)
    {
		delete m_pReadMutexLock;
		m_pReadMutexLock=m_pWriteMutexLock=NULL;
    }

	if(m_pReadMutexLock)
	{
		delete m_pReadMutexLock;
		m_pReadMutexLock=NULL;
	}

	if(m_pWriteMutexLock)
	{
		delete m_pWriteMutexLock;
		m_pWriteMutexLock=NULL;
	}
}

//	���BlockSize��BlockCountΪ�㣬��ʾҪ��ʼ���ڴ���
void CPackageCache::SetCacheParam(int CacheSize)		
{
	if(CacheSize>0)
		m_CacheSize=(CacheSize/4+(CacheSize%4==0?0:1))*4;

	m_GetOff=0;
	m_SetOff=0;

	if(m_pCache) 
	{
		free(m_pCache);
		m_pCache=NULL;
	}

	if(m_CacheSize>0)
		m_pCache=(char*)malloc(m_CacheSize);
}




bool CPackageCache::IsEmpty(void)
{

	GuardRead();

	return m_GetOff==m_SetOff?true:false;
}



//	��ָ��λ�ö�һ�����ݣ��������ݻ����۷�����У�����ݳ���
//	Off�����λ��
//	���ض����ֽڸ���
int CPackageCache::ReadData(int Off,char* pData,int ReadLen) const
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
int	CPackageCache::WriteData(int Off, const char* pData,int WriteLen)
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





//	��һ�����ݴ��뱾�ػ����ļ���pPackageָ��İ�ͷ��������ݺͰ�Χ
//	������ݿ���ڻ������ߴ磬����ʧ��
//	��������ԭ��������
//	ע�⣺DataLen��Ҫ��4�ֽ�ȡ��
bool CPackageCache::SaveDataTo(PackageHeadStru* pPackageHead, const void* pData,int* pOriSetOff,int* pNewSetOff)
{
	int PackageLen,i,AdditionalLen;
	DWORD dwTail=TAIL_PACKAGECACHE;

	if(!pPackageHead) return false;
	if(pPackageHead->DataLen>0&&!pData) return false;

    pPackageHead->Prefix=PREFIX_PACKAGECACHE;
	AdditionalLen=4-(pPackageHead->DataLen)%4;
	if(AdditionalLen==4) AdditionalLen=0;

	PackageLen=sizeof(PackageHeadStru) + pPackageHead->DataLen+AdditionalLen + 4;


	GuardWrite();
	if(m_SetOff>=m_GetOff&&m_CacheSize-(m_SetOff-m_GetOff)<=PackageLen) return false;
	if(m_SetOff<m_GetOff&&m_GetOff-m_SetOff<=PackageLen) return false;

    if(pOriSetOff) (*pOriSetOff)=m_SetOff;

	WriteData(m_SetOff,(char*)pPackageHead,sizeof(PackageHeadStru));
	if(pPackageHead->DataLen>0)
	{
		WriteData((m_SetOff+sizeof(PackageHeadStru))%m_CacheSize,(char*)pData,pPackageHead->DataLen);
		if(AdditionalLen>0)
		{													//��4�ֽ�ȡ��
			i=0;
			WriteData((m_SetOff+sizeof(PackageHeadStru)+pPackageHead->DataLen)%m_CacheSize,(char*)&i,AdditionalLen);
		}
	}

	WriteData((m_SetOff+sizeof(PackageHeadStru)+pPackageHead->DataLen+AdditionalLen)%m_CacheSize,(char*)&dwTail,sizeof(dwTail));
	m_SetOff=(m_SetOff+PackageLen)%m_CacheSize;

    if(pNewSetOff) (*pNewSetOff)=m_SetOff;

	return true;
}

//��һ�����ݴ��뻺��
bool CPackageCache::SaveDataTo(int CmdID,int PlatformID,int ExchangeID,DWORD dwSeq,int DataLen, const void* pData,int* pOriSetOff,int* pNewSetOff)
{
    PackageHeadStru PackageHead;

    memset(&PackageHead,0,sizeof(PackageHead));
    PackageHead.CmdID=CmdID;
    PackageHead.PlatformID=PlatformID;
    PackageHead.ExchangeID=ExchangeID;
    PackageHead.DataLen=DataLen;
    PackageHead.dwSeq=dwSeq;

    return SaveDataTo(&PackageHead,pData,pOriSetOff,pNewSetOff);
}

//	������Ч������
//	ִ�����Ҫôm_GetOff==m_SetOff,Ҫô��һ���Ϸ���
void CPackageCache::IgnoreInvalidPackage(void)
{
	PackageHeadStru	PackageHead;
	int AdditionalLen,PackageLen,ReadLen;
	DWORD dwTail;

	if(!m_pCache) return ;
	if(m_GetOff==m_SetOff) return ;

	while(1)
	{
		ReadLen=ReadData(m_GetOff,(char*)&PackageHead,sizeof(PackageHead));

		if(ReadLen==sizeof(PackageHead)&&PackageHead.Prefix==PREFIX_PACKAGECACHE)
		{
			AdditionalLen=4-(PackageHead.DataLen%4);
			if(AdditionalLen==4) AdditionalLen=0;
			PackageLen=sizeof(PackageHeadStru)+PackageHead.DataLen+AdditionalLen+4;
			dwTail=0;
			ReadData((m_GetOff+PackageLen-4)%m_CacheSize,(char*)&dwTail,4);
		
			if( dwTail==TAIL_PACKAGECACHE&&
				(m_GetOff<m_SetOff && m_GetOff+PackageLen<=m_SetOff ||
				 m_GetOff>m_SetOff && (m_GetOff+PackageLen<m_CacheSize||m_GetOff+PackageLen-m_CacheSize<=m_SetOff))  )
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

//ȡ��ǰ�������ݳ���
int CPackageCache::GetCurPackageDataLen(void)                                 
{
	GuardRead();

	PackageHeadStru	PackageHead;

	if(!m_pCache) return 0;

	IgnoreInvalidPackage();

	if(m_GetOff==m_SetOff) return 0;

															//��ʱ��֤��һ���Ϸ���
    PackageHead.DataLen=0;
	ReadData(m_GetOff,(char*)&PackageHead,sizeof(PackageHead));
    return PackageHead.DataLen;
}

//���ַ�������ȡ����ǰ���ݿ飬��ɾ�������DataSize����������ʧ��.���pData==NULL��ֻȡ��ͷ
bool CPackageCache::PeekFrom(int&CmdID,int&PlatformID,int& ExchangeID,DWORD& dwSeq,int& DataLen,void* pData,int DataSize,int* pGetOff)
{
    PackageHeadStru PackageHead;
    bool bRlt;

    bRlt=PeekFrom(&PackageHead,pData,DataSize,pGetOff);
    if(!bRlt) return false;

    CmdID=PackageHead.CmdID;
    PlatformID=PackageHead.PlatformID;
    ExchangeID=PackageHead.ExchangeID;
    dwSeq=PackageHead.dwSeq;

    return true;
}

//	���ַ�������ȡ����ǰ���ݿ飬��ɾ�������DataSize����������ʧ��
//	���pData==NULL��ֻȡ��ͷ
bool CPackageCache::PeekFrom(PackageHeadStru* pPackageHead,void* pData,int DataSize,int* pGetOff)
{
	GuardRead();

	PackageHeadStru	PackageHead;

	if(!m_pCache) return false;

	IgnoreInvalidPackage();

	if(m_GetOff==m_SetOff) return false;

															//��ʱ��֤��һ���Ϸ���
	ReadData(m_GetOff,(char*)&PackageHead,sizeof(PackageHead));
	if(pPackageHead)
		memcpy((char*)pPackageHead,(char*)&PackageHead,sizeof(PackageHeadStru));

	if(PackageHead.DataLen>0&&pData&&PackageHead.DataLen>DataSize)
	{
		return false;										//�������ߴ粻�㣬ֱ�ӷ���false
	}


	if(PackageHead.DataLen>0&&pData) 
		ReadData((m_GetOff+sizeof(PackageHeadStru))%m_CacheSize,(char*)pData,PackageHead.DataLen);

    if(pGetOff) (*pGetOff)=m_GetOff;

	return true;
}


//	�ӻ�����ȡ����ǰ���ݿ飬Ȼ��ɾ��֮�����DataSize����������ʧ��
bool CPackageCache::GetFrom(PackageHeadStru* pPackageHead,void* pData,int DataSize,int* pOriGetOff,int* pNewGetOff)
{
	bool bRlt;
	int AdditionalLen,PackageLen;
    PackageHeadStru PackageHead;

    if(!pPackageHead) pPackageHead=&PackageHead;

	GuardRead();

    if(pOriGetOff) (*pOriGetOff)=m_GetOff;

#if 0//���ܴ򿪣���������
	bRlt=PeekFrom(pPackageHead,pData,DataSize);
#else
	do
	{//copy form CPackageCache::PeekFrom(PackageHeadStru* pPackageHead,void* pData,int DataSize)
		PackageHeadStru	PackageHead;


		if(!m_pCache)
		{
			bRlt=false;
			break;
		}

		IgnoreInvalidPackage();

		if(m_GetOff==m_SetOff)
		{
			bRlt=false;
			break;
		}

																//��ʱ��֤��һ���Ϸ���
		ReadData(m_GetOff,(char*)&PackageHead,sizeof(PackageHead));
		if(pPackageHead)
			memcpy((char*)pPackageHead,(char*)&PackageHead,sizeof(PackageHeadStru));

		if(PackageHead.DataLen>0&&pData&&PackageHead.DataLen>DataSize)
		{
			bRlt=false;										//�������ߴ粻�㣬ֱ�ӷ���false
			break;
		}

		if(PackageHead.DataLen>0&&pData) 
			ReadData((m_GetOff+sizeof(PackageHeadStru))%m_CacheSize,(char*)pData,PackageHead.DataLen);

		bRlt=true;
	}while(0);
#endif

	if(bRlt)
	{
		AdditionalLen=4-(pPackageHead->DataLen%4);
		if(AdditionalLen==4) AdditionalLen=0;
		PackageLen=sizeof(PackageHeadStru)+pPackageHead->DataLen+AdditionalLen+4;
		m_GetOff=(m_GetOff+PackageLen)%m_CacheSize;
	}

    if(pNewGetOff) (*pNewGetOff)=m_GetOff;
	return bRlt;
}


//	ɾ������
void CPackageCache::DeleteCurPackage(int* pOriGetOff,int* pNewGetOff)
{
	GetFrom(NULL,NULL,0,pOriGetOff,pNewGetOff);
}



//��ջ�����
void CPackageCache::Empty(void)
{
	GuardRead();

	m_GetOff=m_SetOff;
}


void CPackageCache::GetPosition(int& GetOff,int& SetOff)
{

	GuardRead();

    GetOff=m_GetOff;
    SetOff=m_SetOff;
}

//�ڻ������У�������CmdID����ָ��ֵ�İ���CmdID��Ϊ��(Ŀ����ʹ֮��Ч)
void CPackageCache::ClearCmdIDInPackageHead(int CmdID)
{
    PackageHeadStru PackageHead;

    if(CmdID==0) return;

	GuardWrite();

    int Off=0;
    int Len=(m_SetOff+m_CacheSize-m_GetOff)%m_CacheSize;
    int AdditionalLen,PackageLen;
    while(Off+(int)sizeof(PackageHead)<=Len)
    {
	    if(ReadData((Off+m_GetOff)%m_CacheSize,(char*)&PackageHead,sizeof(PackageHead))!=(int)sizeof(PackageHead))
            break;
        if(PackageHead.CmdID==CmdID)
        {
            PackageHead.CmdID=0;
            WriteData((Off+m_GetOff)%m_CacheSize,(char*)&PackageHead,sizeof(PackageHead));
        }

        AdditionalLen=4-(PackageHead.DataLen%4);
		if(AdditionalLen==4) AdditionalLen=0;
		PackageLen=sizeof(PackageHeadStru)+PackageHead.DataLen+AdditionalLen+4;

        Off+=PackageLen;
    }
}
