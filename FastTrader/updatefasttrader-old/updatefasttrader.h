#if !defined(UPDATE_FASTTRADER_H)
#define UPDATE_FASTTRADER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma once

#include "stdafx.h" 
//#include "checkversion.h"

/////////////////////////////////////////////////////////////////////////////
// CUpdateThread thread
// ���ر�������������ļ���
#define UPDATE_CONFIG_FILENAME "UpdateConfig.ini"

// ���������ļ��е�������
#define SECTION_UPDATE "UPDATE"
#define SECTION_COMMON "COMMON"

// ���������ļ�״̬��Ϣ�ṹ
struct DOWNLOAD_INFO_STRU
{
	UINT iFileCount;					// �ϼ��ļ���
	UINT iFileSize;						// �ϼ��ֽ���
};

// ���ص����ļ�������Ϣ�ṹ
struct DOWNLOAD_PROGRESS_STRU
{
	UINT iCurrentFileSize;				// ��ǰ�������ص��ļ����ֽ���
	UINT iCurrentFileFinishedSize;		// ��ǰ�ļ��������ֽ���
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



/////////////////////////////////////////////////////////////////////////////
// CAutoUpdateApp:
// See AutoUpdate.cpp for the implementation of this class
//

class CUpdateFasttraderApp : public wxApp
{
public:
	virtual bool OnInit();
 //   CUpdateFasttraderApp();
	//~CUpdateFasttraderApp();
  //  wxString m_pszExeName;

	//BOOL DoUpdate();					// ִ������

	//DECLARE_MESSAGE_MAP()

private:

private:
 //   void CUpdateFasttraderApp::GetPath(std::string &filePath);
 //   size_t GetFileSize(const char* sFilename);
  //  char* GetFilename( char* sFilePath);
};
DECLARE_APP(CUpdateFasttraderApp)
#endif // 
