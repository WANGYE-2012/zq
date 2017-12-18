// DB_OO4OPool.cpp: implementation of the CDB_OO4OPool class.
//
//	
//	FileName	:db_oo4opool.h
//	Description	:1. ʵ��oo4o���ݿ����ӳز���
//	Include		:oracl.h
//	Lib			:oraclm32.lib
//	Platform	:windows
//	Author		:lzq
//				 Copyright (C), 2008-2028
//////////////////////////////////////////////////////////////////////

#include "DB_OO4OPool.h"

#ifdef				UseDB_OO4OPOOL

#include <windows.h>
#include "oracl.h"
#include "Tools_Win32.h"


#pragma comment( lib, "oraclm32.lib" )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDB_OO4OPool::CDB_OO4OPool(int PoolCount,int IdleTimeOut)
{
	int				i;

	m_PoolCount=PoolCount;
	m_IdleTimeOut=IdleTimeOut;
	if(m_PoolCount>10) m_PoolCount=10;

	m_pSem=CTools_Win32::AllocWinSem();


	OStartup(OSTARTUP_MULTITHREADED);

	m_pPool=(PoolDataStr*)malloc(m_PoolCount*sizeof(PoolDataStr));
	if(m_pPool)
	{
		memset((char*)m_pPool,0,m_PoolCount*sizeof(PoolDataStr));

		for(i=0;i<m_PoolCount;i++)
		{
			try	
			{	
				m_pPool[i].pOdb=(char*) new ODatabase;
			}
			catch(OException E) 
			{	
				m_pPool[i].pOdb=NULL;
			}
		}

	}

}

CDB_OO4OPool::~CDB_OO4OPool()
{
	int				i;

	if(m_pPool)
	{
		for(i=0;i<m_PoolCount;i++)
		{
			if(m_pPool[i].pOdb)
			{
				try
				{
					if(((ODatabase*)(m_pPool[i].pOdb))->GetConnectionOK())
						((ODatabase*)(m_pPool[i].pOdb))->Close();
				}
				catch(OException E)
				{
				}

				delete (ODatabase*)(m_pPool[i].pOdb);
				m_pPool[i].pOdb=NULL;
			}
		}

		free(m_pPool);
		m_pPool=NULL;
	}

	OShutdown();

	if(m_pSem)
	{
		CTools_Win32::ReleaseWinSem(m_pSem);
		m_pSem=NULL;
	}

}


//	�������ݿ�
//	�������ݿ����ӵ�PoolOrd
int CDB_OO4OPool::ConnectDB(char *db_sSource,char *db_sUser,char *db_sPasswd)
{
	oresult			oRlt;
	int				PoolOrd=-1,i;


	if(!m_pPool) return -1;
	if(!db_sSource||!db_sUser||!db_sPasswd) return -1;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);


																				//***�����ӳ�����û�п��õ�����
	for(i=0;i<m_PoolCount;i++)
	{																			
		if(	m_pPool[i].pOdb&&
			strncmp(m_pPool[i].Odb_Source,db_sSource,sizeof(m_pPool[i].Odb_Source)-1)==0&&
			strncmp(m_pPool[i].Odb_User,db_sUser,sizeof(m_pPool[i].Odb_User)-1)==0&&
			strncmp(m_pPool[i].Odb_PW,db_sPasswd,sizeof(m_pPool[i].Odb_PW)-1)==0)
			break;
	}
	if(i<m_PoolCount)
	{																			//�����ԭ�ȵ�odb�����Ƿ񻹱���������
		try 
		{ 
			if(((ODatabase*)(m_pPool[i].pOdb))->GetConnectionOK())
				PoolOrd=i;
		}
		catch(OException E) 
		{ 
		}
	}


																				//***��һ�����õ�Poolλ��
	if(PoolOrd==-1)
	{
		for(i=0;i<m_PoolCount;i++)
		{
			if(m_pPool[i].pOdb&&!m_pPool[i].Odb_Source[0]) 
			{
				PoolOrd=i;
				break;
			}
		}

		if(PoolOrd>=0)															//***����һ��������
		{
			try	
			{	
				oRlt=((ODatabase*)(m_pPool[PoolOrd].pOdb))->Open(db_sSource,db_sUser,db_sPasswd,ODATABASE_DEFAULT);
				if(oRlt!=OSUCCESS)
				{
					long ErrNumber;

					ErrNumber=((ODatabase*)(m_pPool[PoolOrd].pOdb))->ErrorNumber();
					PoolOrd=-1;
				}
			}
			catch(OException E) 
			{	
				PoolOrd=-1;
			}

			if(PoolOrd>=0)
			{
				strncpy(m_pPool[PoolOrd].Odb_Source,db_sSource,sizeof(m_pPool[PoolOrd].Odb_Source)-1);
				strncpy(m_pPool[PoolOrd].Odb_User,db_sUser,sizeof(m_pPool[PoolOrd].Odb_User)-1);
				strncpy(m_pPool[PoolOrd].Odb_PW,db_sPasswd,sizeof(m_pPool[PoolOrd].Odb_PW)-1);
			}
		}

	}

	if(PoolOrd>=0)
	{
		m_pPool[PoolOrd].Odb_OperateTime=GetTickCount();
		m_pPool[PoolOrd].RefCount++;
	}


	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return PoolOrd;
}


//	�Ͽ����ݿ�����
//	��OdbPool���ҵ���Ӧ��odb���������ùر�ʱ��
bool CDB_OO4OPool::DisconnectDB(int PoolOrd)
{
	if(PoolOrd<0||PoolOrd>=m_PoolCount||!m_pPool[PoolOrd].pOdb) return false;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);


	m_pPool[PoolOrd].Odb_CloseTime=GetTickCount();
	m_pPool[PoolOrd].RefCount--;

	if(m_pPool[PoolOrd].RefCount<=0)
	{
		try
		{
			((ODatabase*)(m_pPool[PoolOrd].pOdb))->Close();
		}
		catch(OException E) 
		{
		}
		memset((char*)m_pPool[PoolOrd].Odb_Source,0,sizeof(m_pPool[PoolOrd].Odb_Source));
		memset((char*)m_pPool[PoolOrd].Odb_User,0,sizeof(m_pPool[PoolOrd].Odb_User));
		memset((char*)m_pPool[PoolOrd].Odb_PW,0,sizeof(m_pPool[PoolOrd].Odb_PW));
	}

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return true;
}


//	��ѯ���ݿ��Ƿ�����������״̬
bool CDB_OO4OPool::GetConnectOK(int PoolOrd)
{
	bool			bRlt=false;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	try
	{
		if(	PoolOrd>=0&&PoolOrd<m_PoolCount&&m_pPool[PoolOrd].pOdb&&
			((ODatabase*)(m_pPool[PoolOrd].pOdb))->GetConnectionOK())
			bRlt=true;
	}
	catch(OException E)
	{
		bRlt=false;
	}


	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}



//ȡ���Ĵ��󣬴����ı�����pRltErrText�У����ش����
int CDB_OO4OPool::GetLastError(int PoolOrd,char* pRltErrText,int ErrTextSize)
{
	long ErrNumber=0;

	if(PoolOrd<0||PoolOrd>=m_PoolCount||!m_pPool[PoolOrd].pOdb) return 0;

	if(pRltErrText&&ErrTextSize>0)
		memset(pRltErrText,0,ErrTextSize);

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	ErrNumber=((ODatabase*)(m_pPool[PoolOrd].pOdb))->ErrorNumber();
	if(ErrNumber!=OERROR_NONE&&pRltErrText&&ErrTextSize>0)
	{
		try
		{
			strncpy(pRltErrText,((ODatabase*)(m_pPool[PoolOrd].pOdb))->GetErrorText(),ErrTextSize-1);
		}
		catch(OException E)
		{
		}
	}

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return (int)ErrNumber;
}



//	��RecordSet�����ؼ�¼����ָ��
char* CDB_OO4OPool::OpenRSet(int PoolOrd,char *pSql)
{
	ODynaset		*pOdyn=NULL;

	if(PoolOrd<0||PoolOrd>=m_PoolCount||!m_pPool[PoolOrd].pOdb||!pSql) return NULL;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	try
	{
		pOdyn=new ODynaset;
		if(pOdyn)
		{
			if(pOdyn->Open(*((ODatabase*)(m_pPool[PoolOrd].pOdb)),pSql)!=OSUCCESS)
			{
				delete pOdyn;
				pOdyn=NULL;
			}
		}
	}
	catch(OException E)
	{
		pOdyn=NULL;
	}

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return (char*)pOdyn;
}

//	�ر�RecordSet
void CDB_OO4OPool::CloseRSet(char *pRSet)
{
	if(!pRSet) return;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	try
	{
		((ODynaset*)pRSet)->Close();
		delete (ODynaset*)pRSet;
	}
	catch(OException E)
	{
	}

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);
}

//	�Ƶ���һ����¼
void CDB_OO4OPool::MoveNext(char *pRSet)
{
	if(!pRSet) return;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	try
	{
		((ODynaset*)pRSet)->MoveNext();
	}
	catch(OException E)
	{
	}

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);
}

//	�ж��Ƿ��˼�¼�������
bool CDB_OO4OPool::IsEOF(char *pRSet)
{
	bool			bRlt;

	if(!pRSet) return false;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	try
	{
		bRlt=((ODynaset*)pRSet)->IsEOF()==TRUE?true:false;
	}
	catch(OException E)
	{
		bRlt=false;
	}

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}


//	ȡǰ��������ַ���ֵ
//	��odb ��ִ��sql��䣬ȡǰ���������¼����ֵ
bool CDB_OO4OPool::GetFieldData(char* pRSet,int FieldNum,char **pRltBuf,int *RltSize)
{
	bool			bRlt=false;
	int				i;

	if(!pRltBuf||!RltSize) return false;

	for(i=0;i<FieldNum;i++)
	{
		if(pRltBuf[i])
		{
			if(RltSize[i]>0)
				memset(pRltBuf[i],0,RltSize[i]);
		}
	}

	if(!pRSet) return false;


	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	try
	{
		for(i=0;i<FieldNum;i++)
		{
			if(RltSize[i]>0)
				((ODynaset*)pRSet)->GetFieldValue(i,pRltBuf[i],(unsigned short)RltSize[i]-1);
		}
		bRlt=true;
	}
	catch(OException E)
	{
		bRlt=false;
	}

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}


//	ȡǰ��������ַ���ֵ
//	��odb ��ִ��sql��䣬ȡǰ���������¼����ֵ
bool CDB_OO4OPool::GetSqlFieldData(int PoolOrd,char *sql,int FieldNum,char **pRltBuf,int *RltSize)
{
	bool			bRlt=false;
    ODynaset		*podyn=NULL;

	if(PoolOrd<0||PoolOrd>=m_PoolCount||!m_pPool[PoolOrd].pOdb) return false;
	if(!sql||!pRltBuf) return false;


	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	try
	{
		podyn=new ODynaset;
		if(podyn)
		{
			if(podyn->Open(*((ODatabase*)(m_pPool[PoolOrd].pOdb)),sql)==OSUCCESS)
			{
				if(!podyn->IsEOF())
				{
					bRlt=GetFieldData((char*)podyn,FieldNum,pRltBuf,RltSize);
				}
				podyn->Close();
			}
			delete podyn;
			podyn=NULL;
		}
	}
	catch(OException E)
	{
	}

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}


//	ִ��sql���
bool CDB_OO4OPool::ExecSql(int PoolOrd,char *sql)
{
	bool			bRlt=false;

	if(PoolOrd<0||PoolOrd>=m_PoolCount||!m_pPool[PoolOrd].pOdb) return false;
	if(!sql) return false;

	if(m_pSem)
		CTools_Win32::TakeWinSem(m_pSem);

	try
	{
		if(((ODatabase*)(m_pPool[PoolOrd].pOdb))->ExecuteSQL(sql)==OSUCCESS)
			bRlt=true;
		else
		{
			int ErrNumber;
			ErrNumber=((ODatabase*)(m_pPool[PoolOrd].pOdb))->ErrorNumber();
			ErrNumber=0;
		}

	}
	catch(OException E)
	{
		char		Buf[1024];
		memset(Buf,0,sizeof(Buf));
		strncpy(Buf,E.GetErrorText(),sizeof(Buf)-1);
		Buf[sizeof(Buf)-1]=0;
	}

	if(m_pSem)
		CTools_Win32::GiveWinSem(m_pSem);

	return bRlt;
}



#endif