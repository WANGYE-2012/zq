#include "StdAfx.h"
#include "TopException.h"
//���ļ�ֻ������Releaseģʽ
#include <dbghelp.h>
#pragma comment( lib, "dbghelp.lib" )
const int MAX_DebugBuffer = 4096;    // Ӧ�ó���͵�����֮�䴫��������ͨ��һ�� 4KB ��С�Ĺ����ڴ����ɵ�

typedef struct dbwin_buffer {
    DWORD   dwProcessId;
    char    szString[MAX_DebugBuffer-sizeof(DWORD)];
}DEBUGBUFFER,*PDEBUGBUFFER;
list<string> g_DebugBufferList;
void CreateMiniDump(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
    std::string filename="Exception.dmp";
    GlobalFunc::GetPath(filename);
    HANDLE hFile = CreateFile(filename.c_str(), GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
    if( INVALID_HANDLE_VALUE == hFile )
    {
        return;
    }

    MINIDUMP_EXCEPTION_INFORMATION mei;
    mei.ThreadId = GetCurrentThreadId();
    mei.ClientPointers = TRUE;
    mei.ExceptionPointers = ExceptionInfo;

    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithFullMemory, &mei, NULL, NULL);
    CloseHandle(hFile);
}

LONG WINAPI TopExceptionCallback(struct _EXCEPTION_POINTERS *pei)
{
    CreateMiniDump(pei);
	wxFile file;
	std::string filename="Exception.log";
	GlobalFunc::GetPath(filename);
    if(file.Open(filename.c_str(),wxFile::write_append))
	{
        string AllLog;
        list<string>::const_iterator it=g_DebugBufferList.begin();
        for(;it!=g_DebugBufferList.end();++it)
        {
            AllLog+=it->c_str();
        }
        file.Write(AllLog.c_str(),AllLog.length());
        file.Close();

	}
    return EXCEPTION_CONTINUE_SEARCH;
}
CTrackThread::CTrackThread():wxThread()
{

}
void * CTrackThread::Entry()
{
    HANDLE hMapping = NULL;  
    HANDLE hAckEvent = NULL;
    HANDLE hReadyEvent = NULL;
    PDEBUGBUFFER pdbBuffer = NULL;  
    // ���¼����    
    hAckEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("DBWIN_BUFFER_READY"));    
    if(hAckEvent == NULL)
    {
        CloseHandle(hAckEvent); 
        return NULL;
    }

    hReadyEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("DBWIN_DATA_READY"));  
    if(hReadyEvent == NULL)
    {
        CloseHandle(hReadyEvent); 
        return NULL;
    } 

    // �����ļ�ӳ��  
    hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MAX_DebugBuffer, TEXT("DBWIN_BUFFER"));   
    if(hMapping == NULL)
    {
        CloseHandle(hMapping); 
        return NULL;
    } 

    // ӳ����Ի�����  
    pdbBuffer = (PDEBUGBUFFER) MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);  
    DWORD pid=::GetCurrentProcessId();
    char   TimeString[MAX_DebugBuffer+20]={0};
    // ѭ��  
    while( !TestDestroy() )
    {             
        // �����¼�
        SetEvent(hAckEvent);  
        // �ȴ�����������
        DWORD dwError=WaitForSingleObject(hReadyEvent, 1000);
        if(dwError==WAIT_TIMEOUT) 
            continue; 
        if(dwError==WAIT_OBJECT_0)   
        {              
            if(pid== pdbBuffer->dwProcessId)
            {
                time_t timer=time(NULL);
                struct tm* ptm=localtime(&timer);
                sprintf_s(TimeString,sizeof(TimeString)-1,"%02d:%02d:%02d  %s",
                    ptm->tm_hour,ptm->tm_min,ptm->tm_sec,pdbBuffer->szString);
                if(TimeString[strlen(TimeString)-1]!='\n') 
                    strcat(TimeString,"\r\n");
                g_DebugBufferList.push_back(TimeString); 
                if(g_DebugBufferList.size()>1000)
                    g_DebugBufferList.pop_front();
            }
        }   
    } 
    // �ͷ�   
    if (pdbBuffer) 
    {      
        UnmapViewOfFile(pdbBuffer);   
    }   
    CloseHandle(hMapping);   
    CloseHandle(hReadyEvent);  
    CloseHandle(hAckEvent);
    return NULL;
}
void CTrackThread::OnExit()
{

}
CBeepThread::CBeepThread():wxThread()
{

}
void * CBeepThread::Entry()
{
	MSG msg;
	ZeroMemory(&msg,sizeof(MSG));
	UINT count=0;
	while(!TestDestroy())
	{
		count=0;
		while(PeekMessage(&msg,(HWND)-1, 0, 0, PM_REMOVE))
			count++;
		if(count>0&&msg.message==WM_BEEPMSG)//����Ϣ,MSG�������һ����Ϣ
		{
			switch(msg.wParam)
			{
			case 2:
				Beep(800, 300);
				Sleep(200);
				Beep(800, 300);
				break;
			case 1:
				Beep(800, 1200);
				break;
            default:
                Sleep(200);
                break;
			}
		}
        else Sleep(200);
	}
	return NULL;
}
void CBeepThread::OnExit()
{

}