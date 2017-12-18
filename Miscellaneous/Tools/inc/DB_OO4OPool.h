// DB_OO4OPool.h: interface for the CDB_OO4OPool class.
//
//	
//	FileName	:db_oo4opool.h
//	Description	:1. ʵ��oo4o���ݿ����ӳز���
//	Include		:oracl.h
//	Lib			:oraclm32.lib
//	Platform	:windows
//
//	Author		:lzq
//				 Copyright (C), 2008-2028
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DB_OO4OPOOL_H__77FB0FA4_D29B_44F7_BC83_A47453B2642F__INCLUDED_)
#define AFX_DB_OO4OPOOL_H__77FB0FA4_D29B_44F7_BC83_A47453B2642F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define				UseDB_OO4OPOOL

#ifdef				UseDB_OO4OPOOL


#include "public.h"

#define OdbPoolStrSize	128														//odb��pool���ַ����ĳߴ�


struct PoolDataStr																//���ӳص�����
{
	char			*pOdb;
	int				RefCount;													//�Ա����ӵ����ô���
	char			Odb_Source[OdbPoolStrSize];
	char			Odb_User[OdbPoolStrSize];
	char			Odb_PW[OdbPoolStrSize];
	DWORD			Odb_OperateTime;											//�������ʱ��
	DWORD			Odb_CloseTime;												//�ر�ʱ��
};


class CDB_OO4OPool  
{
public:
	CDB_OO4OPool(int PoolCount=10,int IdleTimeOut=30);
	virtual ~CDB_OO4OPool();

	int				ConnectDB(char *db_sSource,char *db_sUser,char *db_sPasswd);//�������ݿ� �������ݿ����ӵ�ָ��
	bool			DisconnectDB(int PoolOrd);									//�Ͽ����ݿ����� ��OdbPool���ҵ���Ӧ��odb���������ùر�ʱ��
	bool			GetConnectOK(int PoolOrd);									//��ѯ���ݿ��Ƿ�����������״̬
	int				GetLastError(int PoolOrd,char* pRltErrText,int ErrTextSize);//ȡ���Ĵ��󣬴����ı�����pRltErrText�У����ش����

	//ע�⣺����5������Ҫ�ڵ��߳���ʹ��
	char*			OpenRSet(int PoolOrd,char* pSql);							//��RecordSet�����ؼ�¼����ָ��
	void			CloseRSet(char* pRSet);										//�ر�RecordSet
	void			MoveNext(char* pRSet);										//�Ƶ���һ����¼
	bool			IsEOF(char* pRSet);											//�ж��Ƿ��˼�¼�������
	bool			GetFieldData(char* pRSet,int FieldNum,char **pRltBuf,int *RltSize);//ȡǰ��������ַ���ֵ ��odb ��ִ��sql��䣬ȡǰ���������¼����ֵ

	bool			GetSqlFieldData(int PoolOrd,char *sql,int FieldNum,char **pRltBuf,int *RltSize);//ȡǰ��������ַ���ֵ ��odb ��ִ��sql��䣬ȡǰ���������¼����ֵ
	bool			ExecSql(int PoolOrd,char *sql);								//ִ��sql���



private:
	PoolDataStr		*m_pPool;

	int				m_PoolCount;
	int				m_IdleTimeOut;
	char			*m_pSem;

};



#endif

#endif // !defined(AFX_DB_OO4OPOOL_H__77FB0FA4_D29B_44F7_BC83_A47453B2642F__INCLUDED_)
