// InternetGetFile.h: interface for the InternetGetFile namespace.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#if !defined(INTERNETGETFILE_H)
#define INTERNETGETFILE_H

namespace Internet
{
    enum
    {
        INTERNET_SUCCESS = 0,
        INTERNET_ERROR_OPENURL,
        INTERNET_ERROR_FILEOPEN,
        INTERNET_ERROR_READFILE,
        INTERNET_ERROR_OPEN
    };
    // �ϱ������������������Ϣ
    enum
    {
        NOTIFY_DOWNLOAD_INFO = 1001,		// ֪ͨҪ���ص��ļ�״��
        NOTIFY_DOWNLOADED_INFO,				// ֪ͨ�����ص��ļ�״��
        NOTIFY_DOWNLOAD_PROGRESS,			// ֪ͨ���ص����ļ�����
        NOTIFY_DOWNLOADING_FILENAME,		// ֪ͨ�������ص��ļ���
        NOTIFY_DOWNLOAD_FILE_FAIL,			// ֪ͨ�����ļ�ʧ��
        NOTIFY_VERIFY_FILE_FAIL,			// ֪ͨУ���ļ�ʧ��
        NOTIFY_UPDATE_FILE_FAIL,			// ֪ͨ�����ļ�ʧ��
        NOTIFY_FINISH_UPDATE,				// ֪ͨ���������Ϣ
    };

    // ���ص����ļ�������Ϣ�ṹ
    struct DOWNLOAD_PROGRESS_STRU
    {
        UINT iCurrentFileSize;				// ��ǰ�������ص��ļ����ֽ���
        UINT iCurrentFileFinishedSize;		// ��ǰ�ļ��������ֽ���
    };

    extern BOOL bInternetGetURLUserBreak;	// �ж����ر�ǣ���Ϊ�����ж������ļ�

    // Get URL form internet with inet API
    int InternetGetURL(const char* sURL, const char* sSaveFilename, const char* sHeader = NULL, HWND hProgressWindow = NULL);
};

#endif // !defined(INTERNETGETFILE_H)