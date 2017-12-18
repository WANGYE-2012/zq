#pragma once

#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>


class CLogFile
{
protected:

    CRITICAL_SECTION _csLock;
    char * _szFileName;
    HANDLE _hFile;

    bool OpenFile();
    DWORD Write(LPCVOID lpBuffer, DWORD dwLength);
    virtual void WriteLog( LPCVOID lpBuffer, DWORD dwLength);//д��־, ������չ�޸�

    void Lock();
    void Unlock();

public:

    CLogFile(const char *szFileName = "Log.log");   
    virtual ~CLogFile();

    const char * GetFileName();    
    void SetFileName(const char *szName);//�޸��ļ����� ͬʱ�ر���һ����־�ļ�

    bool IsOpen();
    void Close();
    void Log(LPCVOID lpBuffer, DWORD dwLength);//׷����־����
    void Log(const char *szText);
};
