// DB_OO4O.h: interface for the CDB_OO4O class.
//
//	
//	FileName	:db_oo4o.h
//	Description	:1. ʵ��oo4o���ݿ����
//	Include		:oracl.h
//	Lib			:oraclm32.lib
//	Platform	:windows
//
//	Author		:lzq
//				 Copyright (C), 2008-2028
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DB_OO4O_H__1821CB9E_5237_4309_BADD_31B7F7D35F63__INCLUDED_)
#define AFX_DB_OO4O_H__1821CB9E_5237_4309_BADD_31B7F7D35F63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define				UseDB_OO4O


#ifdef				UseDB_OO4O

#include "Tools_Win32.h"


class CDB_OO4O  
{
public:
	static void		Regist(void);												//��ʹ�������֮ǰ����Ҫע��һ�£�ע�⣬���߳�ʹ��
	static void		Unregist(void);												//��ʹ���������֮ǰ����Ҫע��һ�£�ע�⣬���߳�ʹ��

	CDB_OO4O();
	CDB_OO4O(char *db_sSource,char *db_sUser,char *db_sPasswd);
	virtual ~CDB_OO4O();

	bool			ConnectDB(char *db_sSource,char *db_sUser,char *db_sPasswd);//�������ݿ⣬�õ������ݿ�������odb�� ����ʱʹ��ָ�������ݿ����ơ��û���������
	void			DisconnectDB(void);											//�Ͽ����ݿ�����
	bool			GetConnectOK(void);
	int				GetLastError(char* pRltErrText,int ErrTextSize);			//ȡ���Ĵ��󣬴����ı�����pRltErrText�У����ش����

	//ע�⣺����6������Ҫ�ڵ��߳���ʹ��
	char*			OpenRSet(char *pSql);										//��RecordSet�����ؼ�¼����ָ��
	void			CloseRSet(char *pRSet);										//�ر�RecordSet
	void			MoveNext(char *pRSet);										//�Ƶ���һ����¼
	bool			IsEOF(char *pRSet);											//�ж��Ƿ��˼�¼�������
	bool			GetFieldData(char *pRSet,int FieldNum,char **pRltBuf,int *RltSize);//ȡǰ��������ַ���ֵ ��odb ��ִ��sql��䣬ȡǰ���������¼����ֵ
	bool			GetFieldData(char *pRSet,char *pRltBuf1,int RltSize1,char *pRltBuf2=NULL,int RltSize2=0,char *pRltBuf3=NULL,int RltSize3=0,char *pRltBuf4=NULL,int RltSize4=0);//ȡǰ2������ַ���ֵ ��odb ��ִ��sql��䣬ȡǰ���������¼����ֵ


	bool			GetSqlFieldData(char *sql,int FieldNum,char **pRltBuf,int *RltSize);//ȡǰ��������ַ���ֵ ��odb ��ִ��sql��䣬ȡǰ���������¼����ֵ
	bool			GetSqlFieldData(char *sql,char *pRltBuf1,int RltSize1,char *pRltBuf2=NULL,int RltSize2=0,char *pRltBuf3=NULL,int RltSize3=0,char *pRltBuf4=NULL,int RltSize4=0);//ȡǰ2������ַ���ֵ ��odb ��ִ��sql��䣬ȡǰ���������¼����ֵ
	bool			ExecSql(char *sql);											//ִ��sql���



private:

	static char*	m_pDB_OO4OPool;												//����һ�����Ա���������ж�����

	int				m_PoolOrd;
	char			*m_pRSet;													//��¼����ָ��
	char			*m_pSem;

};


#endif


#endif // !defined(AFX_DB_OO4O_H__1821CB9E_5237_4309_BADD_31B7F7D35F63__INCLUDED_)
