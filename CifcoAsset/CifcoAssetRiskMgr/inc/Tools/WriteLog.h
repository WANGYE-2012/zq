// WriteLog.h: interface for the CWriteLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITELOG_H__460757FC_6393_42D9_B321_3DF7258F3672__INCLUDED_)
#define AFX_WRITELOG_H__460757FC_6393_42D9_B321_3DF7258F3672__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma warning(disable : 4996)
#pragma warning(disable : 4793)


#include "easymutex.h"


//��־��¼ģʽ����
enum TYPE_WriteLogMode
{
	WriteLogMode_LOCALFILE				= 1					//ʹ�ñ����ļ���־��¼ģʽ
};



//��־������
enum TYPE_WriteLogLevel
{
	WriteLogLevel_RUNNINGINFO			= 1,				//������Ϣ
	WriteLogLevel_DEBUGINFO				= 2,				//������Ϣ
	WriteLogLevel_ALERTINFO				=-1,				//һ��澯��Ϣ
	WriteLogLevel_CRITICALINFO			=-2,				//���ظ澯��Ϣ
	WriteLogLevel_RISKINFO				=-3					//�����澯��Ϣ
};

#define MAX_PATH_WriteLog 1024
class CWriteLog  
{
public:
	/******************************************************************************************************
	*   @brief					���캯����ָ����־ģʽ        
	*   @param[in]	LogMode		��־ģʽ��������LOGMODE_xxxx
	*				pFileName	����Ǳ�����־ģʽ��ָ�������ļ�������·����
	*******************************************************************************************************/
	CWriteLog(TYPE_WriteLogMode LogMode=WriteLogMode_LOCALFILE,char* pFileName=NULL);

	/******************************************************************************************************
	*   @brief					�����������ͷ���Դ        
	*******************************************************************************************************/
	virtual ~CWriteLog();


	/******************************************************************************************************
	*   @brief					д��һ����־        
	*   @param[in]	pLogName	��־���ƣ�������־���ࡣÿ��ģ����Զ����Լ�����־����
	*				LogLevel	��־���𡣿�����LOGLEVEL_xxxx������־�澯ʱ����־�����Ӧ��ͬ�ĸ澯��ʽ
	*				fmt������	������sprintf()�Ĳ���
	*******************************************************************************************************/
	void WriteLog_Fmt(char* pLogName,int LogLevel,char* fmt,...);




private:
	TYPE_WriteLogMode	m_LogMode;							//��־ģʽ
	char	m_pLocalFileName[MAX_PATH_WriteLog];			//Log�ļ���·�����ļ���
	char	m_pLocalFileDir[MAX_PATH_WriteLog];				//Log�ļ�Ŀ¼
	char	*m_pTmpBuf;										//��ʱ������
	int		m_TmpBufSize;									//��ʱ�������ĳߴ�

	Ceasymutex	m_Sem;										//��д������



	/******************************************************************************************************
	*   @brief					������־ģʽΪ�����ļ���־
	*   @param[in]	pFileName	������־�ļ���
	*******************************************************************************************************/
	void SetLogMode_LocalFile(char *pFileName=NULL);


	/******************************************************************************************************
	*   @brief					��һ���ַ���д��һ����־���ڲ����ã�������        
	*   @param[in]	pLogName	��־���ƣ�������־���ࡣÿ��ģ����Զ����Լ�����־����
	*				LogLevel	��־���𡣿�����LOGLEVEL_xxxx������־�澯ʱ����־�����Ӧ��ͬ�ĸ澯��ʽ
	*				pLog		��־�ַ���
	*******************************************************************************************************/
	void WriteLog_Internal(char* pLogName,int LogLevel,char *pLog);




public:
															//����������������������ʵ�����ǣ�Ŀ���ǽ�ֹ��������͸�ֵ
	CWriteLog(const CWriteLog&);									
	CWriteLog& operator= (const CWriteLog&);				


};

#endif // !defined(AFX_WRITELOG_H__460757FC_6393_42D9_B321_3DF7258F3672__INCLUDED_)
