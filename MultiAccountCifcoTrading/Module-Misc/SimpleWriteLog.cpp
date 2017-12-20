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

#include "StdAfx.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "SimpleWriteLog.h"
#include "tools_tools.h"


#pragma warning(disable:4996)



bool zqWriteLog::m_EnableLog=true;//Ĭ���ǿ���ʹ��zqWriteLog���log��
void zqWriteLog::EnableWriteLog(bool bEnable)
{
	m_EnableLog=bEnable;
}
zqWriteLog::zqWriteLog(void)
{
	char Buf[MAX_PATH];
	char ProgramDiskSymbol[MAX_PATH];
	char ProgramPath[MAX_PATH];
	char ProgramName[MAX_PATH];
	char ProgramName_WithoutExt[MAX_PATH];
    char DefaultFileName[MAX_PATH];
	SYSTEMTIME CurTime;
	HFILE hF=NULL;
    int i;
    HANDLE hMutex;





	m_LogMode=LOGMODE_DEFAULT;
	memset(m_pLocalFileName,0,sizeof(m_pLocalFileName));
	memset(m_pLocalFileDir,0,sizeof(m_pLocalFileDir));



	m_DefaultLogMode=LOGMODE_LOCALFILE;						//Ŀǰȱʡ����־��¼ģʽΪ������־ģʽ���������Ը�Ϊ������
	memset(DefaultFileName,0,sizeof(DefaultFileName));
	memset(m_pDefaultFileDir,0,sizeof(m_pDefaultFileDir));


	GetModuleFileName(NULL,Buf,sizeof(Buf)-1);

    memset(ProgramDiskSymbol,0,sizeof(ProgramDiskSymbol));
	memset(ProgramPath,0,sizeof(ProgramPath));
	memset(ProgramName,0,sizeof(ProgramName));
    memset(ProgramName_WithoutExt,0,sizeof(ProgramName_WithoutExt));
	CTools::GetDiskSymbol(Buf,ProgramDiskSymbol,sizeof(ProgramDiskSymbol));
	CTools::GetFilePathWithoutDiskSymbol(Buf,ProgramPath,sizeof(ProgramPath));
	CTools::GetPureFileName(Buf,ProgramName,sizeof(ProgramName));
    for(i=strlen(ProgramName);i>0;i--)
        if(ProgramName[i-1]=='.') break;
    if(i>0&&i<sizeof(ProgramName_WithoutExt))
        memcpy(ProgramName_WithoutExt,ProgramName,i);
    else strncpy(ProgramName_WithoutExt,ProgramName,sizeof(ProgramName_WithoutExt)-1);


                                                            //ȡȱʡ����־�ļ������ļ�Ŀ¼
	strncpy(m_pDefaultFileDir,ProgramDiskSymbol,sizeof(m_pDefaultFileDir)-1);
	strncpy(m_pDefaultFileDir+strlen(m_pDefaultFileDir),ProgramPath,sizeof(m_pDefaultFileDir)-strlen(m_pDefaultFileDir)-1);
	strncpy(m_pDefaultFileDir+strlen(m_pDefaultFileDir),"log_",sizeof(m_pDefaultFileDir)-strlen(m_pDefaultFileDir)-1);
	strncpy(m_pDefaultFileDir+strlen(m_pDefaultFileDir),ProgramName_WithoutExt,sizeof(m_pDefaultFileDir)-strlen(m_pDefaultFileDir)-1);
	strncpy(m_pDefaultFileDir+strlen(m_pDefaultFileDir),"\\",sizeof(m_pDefaultFileDir)-strlen(m_pDefaultFileDir)-1);
	
	strncpy(DefaultFileName,m_pDefaultFileDir,sizeof(DefaultFileName)-1);
	GetLocalTime(&CurTime);
    sprintf(Buf,"%04d%02d%02d%02d.log",CurTime.wYear,CurTime.wMonth,CurTime.wDay,CurTime.wHour);
	strncpy(DefaultFileName+strlen(DefaultFileName),Buf,sizeof(DefaultFileName)-strlen(DefaultFileName)-1);

	hMutex=CreateMutex(NULL,FALSE,MUTEXNAME);
    if(hMutex==NULL) hMutex=OpenMutex(NULL,FALSE,MUTEXNAME);
	if(hMutex!=NULL) WaitForSingleObject(hMutex,1000);

															//����ȱʡ��־ģʽ��д�����־�ļ�
	hF=_lopen(DefaultFileName,OF_READWRITE);
	if(hF==HFILE_ERROR)
		hF=_lcreat(DefaultFileName,0);
	if(hF==HFILE_ERROR&&m_pDefaultFileDir[0]!=0)
	{
		CreateDirectory(m_pDefaultFileDir,NULL);
		hF=_lcreat(DefaultFileName,0);
	}
	if(hF!=HFILE_ERROR)
		_lclose(hF);

	if(hMutex!=NULL)
    {
		ReleaseMutex(hMutex);
        CloseHandle(hMutex);
    }

}


zqWriteLog::zqWriteLog(int iLogMode,char* pFileName)
{
	new(this) zqWriteLog();

	if(iLogMode==LOGMODE_LOCALFILE)
		SetLogMode_LocalFile(pFileName);
}


zqWriteLog::~zqWriteLog(void)
{
}


//������־ģʽΪ�����ļ�ģʽ
void zqWriteLog::SetLogMode_LocalFile(char *pFileName)
{
	char ProgramFileName[MAX_PATH];
	char ProgramPath[MAX_PATH];								//�������ڵ�·�������̷�
	char ProgramDiskSymbol[MAX_PATH];						//�������ڵ��̷�
	char FilePath[MAX_PATH];
	char FileDiskSymbol[MAX_PATH];
	HFILE hF;
    HANDLE hMutex;

	if(pFileName==NULL) return;


	memset(m_pLocalFileName,0,sizeof(m_pLocalFileName));
	memset(m_pLocalFileDir,0,sizeof(m_pLocalFileDir));


															//��ȡ���������ڵ�·�����̷�
	memset(ProgramFileName,0,sizeof(ProgramFileName));
	memset(ProgramPath,0,sizeof(ProgramPath));
	memset(ProgramDiskSymbol,0,sizeof(ProgramDiskSymbol));
	GetModuleFileName(NULL,ProgramFileName,sizeof(ProgramFileName)-1);
	CTools::GetDiskSymbol(ProgramFileName,ProgramDiskSymbol,sizeof(ProgramDiskSymbol));
	CTools::GetFilePathWithoutDiskSymbol(ProgramFileName,ProgramPath,sizeof(ProgramPath));

															//ȡָ����·�����̷�
	memset(FilePath,0,sizeof(FilePath));
	memset(FileDiskSymbol,0,sizeof(FileDiskSymbol));
	CTools::GetDiskSymbol(pFileName,FileDiskSymbol,sizeof(FileDiskSymbol));
	CTools::GetFilePathWithoutDiskSymbol(pFileName,FilePath,sizeof(FilePath));


	if(FileDiskSymbol[0]!=0)
	{														//ָ�����̷�����ΪҲ��ָ����·��
		strncpy(m_pLocalFileName,pFileName,sizeof(m_pLocalFileName)-1);
		sprintf(m_pLocalFileDir,"%s%s",FileDiskSymbol,FilePath);
	}
	else if(FilePath[0]=='\\')
	{														//δָ���̷���ָ���˾���·����ʹ�ó������ڵ��̷���ָ���ľ���·�� 
		strncpy(m_pLocalFileName,ProgramDiskSymbol,sizeof(m_pLocalFileName)-1);
		strncpy(m_pLocalFileName+strlen(m_pLocalFileName),pFileName,sizeof(m_pLocalFileName)-strlen(m_pLocalFileName)-1);
		strncpy(m_pLocalFileDir,ProgramDiskSymbol,sizeof(m_pLocalFileDir)-1);
		strncpy(m_pLocalFileDir+strlen(m_pLocalFileDir),FilePath,sizeof(m_pLocalFileDir)-strlen(m_pLocalFileDir)-1);
	}
	else
	{														//δָ���̷���δָ������·����ʹ�ó������ڵ��̷���·�������·��
		strncpy(m_pLocalFileName,ProgramDiskSymbol,sizeof(m_pLocalFileName)-1);
		strncpy(m_pLocalFileName+strlen(m_pLocalFileName),ProgramPath,sizeof(m_pLocalFileName)-strlen(m_pLocalFileName)-1);
		strncpy(m_pLocalFileName+strlen(m_pLocalFileName),pFileName,sizeof(m_pLocalFileName)-strlen(m_pLocalFileName)-1);
		strncpy(m_pLocalFileDir,ProgramDiskSymbol,sizeof(m_pLocalFileDir)-1);
		strncpy(m_pLocalFileDir+strlen(m_pLocalFileDir),ProgramPath,sizeof(m_pLocalFileDir)-strlen(m_pLocalFileDir)-1);
		strncpy(m_pLocalFileDir+strlen(m_pLocalFileDir),FilePath,sizeof(m_pLocalFileDir)-strlen(m_pLocalFileDir)-1);
	}


	m_LogMode=LOGMODE_LOCALFILE;


	hMutex=CreateMutex(NULL,FALSE,MUTEXNAME);
    if(hMutex==NULL) hMutex=OpenMutex(NULL,FALSE,MUTEXNAME);
	if(hMutex!=NULL) WaitForSingleObject(hMutex,1000);

															//������־�ļ�
	hF=_lopen(m_pLocalFileName,OF_READWRITE);
	if(hF==HFILE_ERROR)
		hF=_lcreat(m_pLocalFileName,0);
	if(hF==HFILE_ERROR&&m_pLocalFileDir[0]!=0)
	{
		CreateDirectory(m_pLocalFileDir,NULL);
		hF=_lcreat(m_pLocalFileName,0);
	}
	if(hF!=HFILE_ERROR)
		_lclose(hF);


	if(hMutex!=NULL)
    {
		ReleaseMutex(hMutex);
        CloseHandle(hMutex);
    }

}

//д��һ����־
//Param:pLogName:	��־���ƣ�������־���ࡣÿ��ģ����Զ����Լ�����־����
//		LogLevel:	��־���𡣿�����LOGLEVEL_xxxx������־�澯ʱ����־�����Ӧ��ͬ�ĸ澯��ʽ
//		pLog:		��־�ַ���
void zqWriteLog::WriteLog(char* pLogName,int LogLevel,char *pLog)
{
	if(!m_EnableLog) return;
	char DateBuf[256];
	char LogLevelBuf[256];
    char Buf[256];
    char FileName[MAX_PATH];
	HFILE hF;
	SYSTEMTIME CurTime;


	GetLocalTime(&CurTime);
	sprintf(DateBuf,"%04d-%02d-%02d %02d:%02d:%02d.%03d : ",
			CurTime.wYear,CurTime.wMonth,CurTime.wDay,
			CurTime.wHour,CurTime.wMinute,CurTime.wSecond,CurTime.wMilliseconds);
	sprintf(LogLevelBuf,"%02d,",LogLevel);


	if(m_LogMode==LOGMODE_DEFAULT)
	{
		if(m_DefaultLogMode==LOGMODE_LOCALFILE)
		{

            memset(FileName,0,sizeof(FileName));
	        strncpy(FileName,m_pDefaultFileDir,sizeof(FileName)-1);
	        sprintf(Buf,"%04d%02d%02d%02d.log",CurTime.wYear,CurTime.wMonth,CurTime.wDay,CurTime.wHour);
	        strncpy(FileName+strlen(FileName),Buf,sizeof(FileName)-strlen(FileName)-1);

			hF=_lopen(FileName,OF_READWRITE);
	        if(hF==HFILE_ERROR)
		        hF=_lcreat(FileName,0);
	        if(hF==HFILE_ERROR&&m_pDefaultFileDir[0]!=0)
	        {
		        CreateDirectory(m_pDefaultFileDir,NULL);
		        hF=_lcreat(FileName,0);
	        }
			if(hF!=HFILE_ERROR)
			{
				_llseek(hF,0,2);
				_lwrite(hF,DateBuf,strlen(DateBuf));
				if(pLogName!=NULL)
				{
					_lwrite(hF,pLogName,strlen(pLogName));
					_lwrite(hF,",",1);
				}
				_lwrite(hF,LogLevelBuf,strlen(LogLevelBuf));
				_lwrite(hF,pLog,strlen(pLog));
				_lwrite(hF,"\r\n",2);
				_lclose(hF);
			}
		}
	}
	else if(m_LogMode==LOGMODE_LOCALFILE)
	{
		hF=_lopen(m_pLocalFileName,OF_READWRITE);
	    if(hF==HFILE_ERROR)
		    hF=_lcreat(m_pLocalFileName,0);
	    if(hF==HFILE_ERROR&&m_pLocalFileDir[0]!=0)
	    {
		    CreateDirectory(m_pLocalFileDir,NULL);
		    hF=_lcreat(m_pLocalFileName,0);
	    }
		if(hF!=HFILE_ERROR)
		{
			_llseek(hF,0,2);
			_lwrite(hF,DateBuf,strlen(DateBuf));
			if(pLogName!=NULL)
			{
				_lwrite(hF,pLogName,strlen(pLogName));
				_lwrite(hF,",",1);
			}
			_lwrite(hF,LogLevelBuf,strlen(LogLevelBuf));
			_lwrite(hF,pLog,strlen(pLog));
			_lwrite(hF,"\r\n",2);
			_lclose(hF);
		}
	}
}


//д��һ����־
//Param:pLogName:	��־���ƣ�������־���ࡣÿ��ģ����Զ����Լ�����־����
//		LogLevel:	��־���𡣿�����LOGLEVEL_xxxx������־�澯ʱ����־�����Ӧ��ͬ�ĸ澯��ʽ
//		fmt����������:������sprintf()�Ĳ���
//ע�� :��������־��ҪС��512�ֽ�
void zqWriteLog::WriteLog_Fmt(char* pLogName,int LogLevel,char* fmt,...)
{
	if(!m_EnableLog) return;
	va_list			argp;
	static char		Buf[20480];
	int				nRlt;
    HANDLE          hMutex;


	hMutex=CreateMutex(NULL,FALSE,MUTEXNAME);
    if(hMutex==NULL) hMutex=OpenMutex(NULL,FALSE,MUTEXNAME);
	if(hMutex!=NULL) WaitForSingleObject(hMutex,1000);


	va_start(argp,fmt);
	nRlt=vsnprintf(Buf,sizeof(Buf)-1,fmt,argp);
	if(nRlt>=sizeof(Buf)) nRlt=sizeof(Buf)-1;
	Buf[nRlt]=0;
	va_end(argp);


	WriteLog(pLogName,LogLevel,Buf);

	if(hMutex!=NULL)
    {
		ReleaseMutex(hMutex);
        CloseHandle(hMutex);
    }

}