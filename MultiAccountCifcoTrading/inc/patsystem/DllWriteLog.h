//*********************************************************************
//	ͳһ��־�ӿ�
//
//	1. ������ͳһ����־д��ӿ�
//	2. DLL
//	3. ���Թ�C++/Java�������
//
//	history 
//	20100113	lzq		�������ļ�
//*********************************************************************


#pragma once

//#include "toolpublic.h"


//#ifdef WRITELOG_EXPORTS
//#define WRITELOG_API __declspec(dllexport)
//#else
//#define WRITELOG_API __declspec(dllimport)
//#endif

//��־��¼ģʽ����
#define LOGMODE_DEFAULT					0					//ʹ��ȱʡ��־��¼ģʽ
#define LOGMODE_LOCALFILE				1					//ʹ�ñ����ļ���־��¼ģʽ


//��־������
#define LOGLEVEL_RUNNINGINFO			1					//������Ϣ
#define LOGLEVEL_DEBUGINFO				2					//������Ϣ
#define LOGLEVEL_ALERTINFO				-1					//һ��澯��Ϣ
#define LOGLEVEL_CRITICALINFO			-2					//���ظ澯��Ϣ
#define LOGLEVEL_RISKINFO				-3					//�����澯��Ϣ


//ͬ��������Mutex��
#define MUTEXNAME						"Mutex_CWriteLog"	//Mutex������


//��־д���ඨ��
class DllWriteLog
{
public:

	//���캯������ʼ����־ģʽΪDefault�������Ƽ��Ĺ��캯��
	DllWriteLog(void);

	//���캯����ָ����־ģʽ
	//Param:iLogMode��	��־ģʽ��������LOGMODE_xxxx
	//		pFileName:	����Ǳ�����־ģʽ��ָ�������ļ�������·����
	DllWriteLog(int iLogMode,char* pFileName=NULL);

	//�����������ͷ���Դ
	~DllWriteLog(void);




	//д��һ����־
	//Param:pLogName:	��־���ƣ�������־���ࡣÿ��ģ����Զ����Լ�����־����
	//		LogLevel:	��־���𡣿�����LOGLEVEL_xxxx������־�澯ʱ����־�����Ӧ��ͬ�ĸ澯��ʽ
	//		fmt����������:������sprintf()�Ĳ���
	void WriteLog_Fmt(char* pLogName,int LogLevel,char* fmt,...);


	//�Ƿ���дlog
	static void EnableWriteLog(bool bEnable);
private:
	static bool m_EnableLog;
	int m_LogMode;											//��־ģʽ
	char m_pLocalFileName[MAX_PATH];						//Log�ļ���·�����ļ���
	char m_pLocalFileDir[MAX_PATH];							//Log�ļ�Ŀ¼

	int m_DefaultLogMode;									//ȱʡ����־��¼ģʽ
	char m_pDefaultFileDir[MAX_PATH];						//ȱʡΪ�����ļ�ģʽʱ��Log�ļ�Ŀ¼



	//������־ģʽΪ�����ļ���־
	//Param:pFileName:	������־�ļ���
	void SetLogMode_LocalFile(char *pFileName);
	//
	bool GetPureFileName(char* pFileNameWithPath,char* pRltFileName,int RltSize);
	//��ȫ·���ļ�����ȡ���̷�����־��:
	bool GetDiskSymbol(char* pFileName,char* pRltDiskSymbol,int RltSize);

	//��ȫ·���ļ�����ȡ��·���������̷�
	bool GetFilePathWithoutDiskSymbol(char* pFileName,char* pRltFilePath,int RltSize);
	//д��һ����־
	//Param:pLogName:	��־���ƣ�������־���ࡣÿ��ģ����Զ����Լ�����־����
	//		LogLevel:	��־���𡣿�����LOGLEVEL_xxxx������־�澯ʱ����־�����Ӧ��ͬ�ĸ澯��ʽ
	//		pLog:		��־�ַ���
	void WriteLog(char* pLogName,int LogLevel,char *pLog);


public:
	DllWriteLog(const DllWriteLog&);									
	DllWriteLog& operator= (const DllWriteLog&);				

};


