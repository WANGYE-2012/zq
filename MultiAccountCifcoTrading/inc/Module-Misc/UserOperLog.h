// UserOperLog.h: interface for the CUserOperLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSEROPERLOG_H__460757FC_6393_42D9_B321_3DF7258F3672__INCLUDED_)
#define AFX_CUSEROPERLOG_H__460757FC_6393_42D9_B321_3DF7258F3672__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma warning(disable : 4996)


#include <windows.h>
#include <queue>
#include <string>


class CUserOperLog  
{
public:
	/******************************************************************************************************
	*   @brief					���캯����ָ����־ģʽ        
	*				pFileName	����Ǳ�����־ģʽ��ָ�������ļ�������·����
	*******************************************************************************************************/
	CUserOperLog(std::string& strFileName);

	/******************************************************************************************************
	*   @brief					�����������ͷ���Դ        
	*******************************************************************************************************/
	virtual ~CUserOperLog();

	/******************************************************************************************************
	*   @brief					д��һ����־        
	*   @param[in]	pLogName	��־���ƣ�������־���ࡣÿ��ģ����Զ����Լ�����־����
	*				LogLevel	��־���𡣿�����LOGLEVEL_xxxx������־�澯ʱ����־�����Ӧ��ͬ�ĸ澯��ʽ
	*				fmt������	������sprintf()�Ĳ���
	*******************************************************************************************************/
	void WriteLog_Fmt(char* fmt,...);

private:
	HANDLE			m_hMutex;
	std::string		m_strFileName;		//Log�ļ���·�����ļ���
	char			*m_pTmpBuf;			//��ʱ������
	int				m_TmpBufSize;		//��ʱ�������ĳߴ�

	/******************************************************************************************************
	*   @brief					��һ���ַ���д��һ����־���ڲ����ã�������        
	*   @param[in]	pLogName	��־���ƣ�������־���ࡣÿ��ģ����Զ����Լ�����־����
	*				LogLevel	��־���𡣿�����LOGLEVEL_xxxx������־�澯ʱ����־�����Ӧ��ͬ�ĸ澯��ʽ
	*				pLog		��־�ַ���
	*******************************************************************************************************/
	void WriteLog_Internal(char *pLog);

	BOOL CryptString(char* pString);

	// LockObject()��UnloadObject()��������������ɶԵ���
	void LockObject() {
		::WaitForSingleObject(m_hMutex, INFINITE);
	};
	void UnlockObject() {
		::ReleaseMutex(m_hMutex);
	};


public:
	//����������������������ʵ�����ǣ�Ŀ���ǽ�ֹ��������͸�ֵ
	CUserOperLog(const CUserOperLog&);									
	CUserOperLog& operator= (const CUserOperLog&);
};

#endif // !defined(AFX_CUSEROPERLOG_H__460757FC_6393_42D9_B321_3DF7258F3672__INCLUDED_)
