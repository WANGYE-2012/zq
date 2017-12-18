// zq.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "zq.h"
#include "UserLoginDlg.h"
#include "ConfirmationDlg.h"
#include "MainFrame.h"
#include "cfgMgr/CfgMgr.h"
#include <wx/file.h> 
#include "wx/dir.h"
#include <fstream>
#include "TopException.h"
#include "updatelib/doupdate.h"
#include "../inc/Module-Misc2/GlobalConfigMgr.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
#include "../inc/Plusin/TCPDataService.h"
#include "Tools_Ansi.h"

#if 1
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("MyApp", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif

LPTOP_LEVEL_EXCEPTION_FILTER g_pFilter=NULL;

IMPLEMENT_APP(MyApp)


unsigned long g_MainThreadID=0;



MyApp::MyApp(void):
m_frame_minsize(20,10)
,m_frame(NULL)
,m_pWriteLog(NULL)
{
    CFTEventParam::Init();
    m_pWriteLog=new zqWriteLog();
	GlobalConfigManager::Load();

    new Cdoupdate();
    if(Cdoupdate::m_pObj) Cdoupdate::m_pObj->start();

    //// for test
    //vector<pair<string,pair<int,int>>> test;
    //test.push_back(make_pair(string("a"),make_pair(1,2)));
    //pair<string,pair<int,int>>& tt=test.back();
    //tt.second.second=3;
    //tt.second.first=4;
    //tt.second.second=5;

    //PlatformStru_InstrumentCommissionRate test;
    //string tidn(typeid(test).name());
    //bool bt = typeid(test)==typeid(PlatformStru_InstrumentCommissionRate);
    //bt = typeid(test)==typeid(PlatformStru_InstrumentMarginRate);
    //memset(&test,0,sizeof(test));

    //// for test
    //multimap<int,int> test;
    //multimap<int,int>::iterator ittest;
    //ittest=test.insert(make_pair(1,11));
    //ittest=test.insert(make_pair(3,33));
    //ittest=test.insert(make_pair(2,22));
    //ittest->second=100;
    //ittest->second=200;
}
MyApp::~MyApp()
{
    LOG_INFO("MyApp::~MyApp()");
    SAFE_DELETE(m_pWriteLog);
    SAFE_DELETE(Cdoupdate::m_pObj);
}
int MyApp::OnExit()
{
	Release();
	int irlt=wxApp::OnExit();

    CFTEventParam::Release();
	//GlobalFunc::ReleaseUserOperWriteLog();
	LOG_INFO("MyApp::OnExit()");
    return irlt;
}
void MyApp::CleanUp()
{
    LOG_INFO("MyApp::CleanUp() START");
    wxApp::CleanUp();
    LOG_INFO("MyApp::CleanUp() END");
}
void MyApp::Release()
{
	::SetUnhandledExceptionFilter(g_pFilter);

	//�ͷ�TCP/IP����
	CTCPDataService::DestroyInstance();

	CfgMgr* pMgr = CfgMgr::GetInstance();
	if ( NULL != pMgr )
	{
		pMgr->Save();
		CfgMgr::DestroyInstance();
	}

    //�ͷŵײ����  
    if(DEFAULT_SVR()) DEFAULT_SVR()->StopThread();
	if(DEFAULT_SVR()) DEFAULT_SVR()->Stop();
	if(DEFAULT_SVR()) g_pPlatformMgr->ReleaseAccount(DEFAULT_SVR());
	PlatformSvrMgr::DestroyInstance();

    LOG_INFO("MyApp::Release()");
}

bool MyApp::UnZipXMLFile(string rcid,string strname)
{
	DWORD lpSize=0;
	void* pFileBuffer=GlobalFunc::GetXmlFileResource(rcid.c_str(),_T("XML"),&lpSize);
	if(pFileBuffer==NULL)
		return false;
	string strPath;
	GlobalFunc::GetPath(strPath);
	strPath+=strname;//;
	wxFile file;
	if(file.Open(strPath.c_str(),wxFile::write))
	{
		file.Write(pFileBuffer,lpSize);
		file.Close();
		return true;
	}
	return false;
}
int MyAllocHook( int nAllocType, void *pvData,  size_t nSize, int nBlockUse, long lRequest,  const unsigned char * szFileName, int nLine )
{
	//_CRT_BLOCK ������ C ����ʱ�⺯���ڲ����е��ڴ����,������ʽ�غ���
	if( nBlockUse == _CRT_BLOCK  )
		return TRUE;
	if( nAllocType == _HOOK_ALLOC )
	{
		::OutputDebugString( "_HOOK_ALLOC" );
	}
	else if( nAllocType == _HOOK_REALLOC )
	{

		::OutputDebugString( "_HOOK_REALLOC" );
	}
	else if( nAllocType == _HOOK_FREE )
	{
		::OutputDebugString( "_HOOK_FREE" );
	}
	return TRUE;
}

bool MyApp::OnInit()
{
    LOG_INFO("MyApp::OnInit");
	g_pFilter=::SetUnhandledExceptionFilter(TopExceptionCallback);
	DeleteOldLogFile(7);
	//::_CrtSetAllocHook( MyAllocHook );
    WXLOG_INFO("MyApp::OnInit");
    //if(!wxApp::OnInit()) return false;

	// �����û�������־Ŀ¼
	//GlobalFunc::MakeUserOperDirectory();

	srand(::GetTickCount());

	wxString strUserName;
	wxString strPassword;

	SetConsoleStart(argc==5 && stricmp(argv[1], "ConsoleStart")==0);
	//��ÿ���̨����
    for(int i=2;i<argc;i++) {
        switch(i) {
        case 2:
            SetConsoleHWnd((HWND)atoi(argv[i]));
            break;
        case 3:
            strUserName = argv[i];
            break;
        case 4:
            strPassword = argv[i];
            break;
        };
    }

	//�����ײ����
	PlatformSvrMgr::CreateInstance();

	g_pPlatformMgr->CreateAccount();
    if(!DEFAULT_SVR()) 
        return false;

    DEFAULT_SVR()->Start();

    //��ȡ������Ϣ
	CfgMgr* pMgr = CfgMgr::GetInstance();
	if(pMgr == NULL)
	{
        DEFAULT_SVR()->Stop();
        g_pPlatformMgr->ReleaseAccount(DEFAULT_SVR());
        PlatformSvrMgr::DestroyInstance();
		return false;
	}
	int bResult=pMgr->Init();
	switch (bResult)
	{
	    case 0:
		    wxMessageBox( LOADSTRING(GUI_LOAD_CONFIG_ERROR));
		    break;
	    case -1:
	    {
		    TCHAR* buffer;
		    DWORD error = ERROR_OPEN_FAILED;
		    ::FormatMessage(
			    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			    NULL,
			    error,
			    0,
			    ( LPTSTR )&buffer,
			    0,
			    NULL );
		    wxMessageBox(wxString("language.csv\n")+buffer);
		    LocalFree( buffer );  
	        break;
	    }
	}
    if(bResult==0||bResult==-1)
    {
        CfgMgr::DestroyInstance();
        DEFAULT_SVR()->Stop();
        g_pPlatformMgr->ReleaseAccount(DEFAULT_SVR());
        PlatformSvrMgr::DestroyInstance();
        return false;
    }
	if( !IsDirWriteable())
	{
		wxMessageBox(LOADSTRING(INSTALL_DIR_READ_ONLY) ,LOADSTRING(USERLOGINDLG_INFORMATION));
        CfgMgr::DestroyInstance();
        DEFAULT_SVR()->Stop();
        g_pPlatformMgr->ReleaseAccount(DEFAULT_SVR());
        PlatformSvrMgr::DestroyInstance();
		return false;
	}
    LPMAIN_WINDOW_CFG pMainWindowCfg=pMgr->GetMainWindowCfg();
    //if(pMainWindowCfg)
    //    DEFAULT_SVR()->SetModuleDataQry(pMainWindowCfg->bModuleIsQry);

    if(!UserLoginDlg::LoadSaveServerParam(true))
	{
		wxMessageBox(LOADSTRING(GUI_GET_SERVER_ADDRESS_ERROR),LOADSTRING(MAINFRAME_INITIALIZE_ERROR_CAPTION),wxOK|wxICON_ERROR);
		return false;	
	}
    //if(wxGetApp().argc==1&&UpdateApp::DoUpdate())
    //    return false;

	//��ʼ��TCP/IP����
	if ( DEFAULT_SVR() && DEFAULT_SVR()->SupportPlugins())
	{
		std::string strAddr = DEFAULT_SVR()->GetServerParamCfg().UniserverAddr;
		int nPos = strAddr.find(':');
		if ( nPos != -1 )
		{
			std::string strIP = strAddr.substr(0, nPos);
			int nPort = atoi(strAddr.substr(nPos+1, strAddr.length()).c_str());
			CTCPDataService::GetInstance()->Connect(strIP, nPort);
		}
	}

    //����������
    StartMainFrame(false);
#if 1//1:���õײ����0:��ֹ�ײ����
	{
		UserLoginDlg logindlg(NULL);
        if(Cdoupdate::m_pObj) Cdoupdate::m_pObj->m_hMainWnd_FastTrader=(HWND)logindlg.GetHandle();
		if(HasConsoleStart())
			logindlg.ResetAutoLogin(strUserName, strPassword);
		if(logindlg.ShowModal()!=wxID_OK)
		{
            if(m_frame) { delete m_frame;  m_frame=NULL; }
			Release();
			return false;
		}
	}
	if(DEFAULT_SVR()&&!(DEFAULT_SVR()->IsSettlementInfoConfirmed()))
	{
		ConfirmationDlg confirmationdlg(NULL);
        if(Cdoupdate::m_pObj) Cdoupdate::m_pObj->m_hMainWnd_FastTrader=(HWND)confirmationdlg.GetHandle();
		if(confirmationdlg.ShowModal()==wxID_OK)
		{
			DEFAULT_SVR()->ReqSettlementInfoConfirm();
		}
		else
		{
            if(m_frame) { delete m_frame;  m_frame=NULL; }
			Release();
			return false;
		}
	}
#endif 
    //����Logϵͳ
    StartLogSystem();
    //��ʼ��������
	if(!StartMainFrame(true))
	{
        if(m_frame) { delete m_frame;  m_frame=NULL; }
		Release();
		return false;
	}
    return true;
}
bool MyApp::StartMainFrame(bool bInit)
{
    if(!bInit)
    {
        m_frame=new MainFrame();
        SetTopWindow(m_frame);
	    m_frame->Show(true);
    }
    else
    {
		if(GlobalConfigManager::IsCifcoServicesHall_ExtraFunction())
			m_frame->LoadCifcoServicesHallApp();
        bool ret_init=m_frame->Initialize(NULL,wxID_ANY,wxEmptyString,wxDefaultPosition,m_frame_minsize);
        if(Cdoupdate::m_pObj) Cdoupdate::m_pObj->m_hMainWnd_FastTrader=(HWND)m_frame->GetHandle();
        if(!ret_init)
        {
            wxMessageBox(LOADSTRING(MAINFRAME_INITIALIZE_ERROR_MSG),
                LOADSTRING(MAINFRAME_INITIALIZE_ERROR_CAPTION),
                wxOK|wxCENTRE|wxICON_ERROR );
            return false;
        }

		m_frame->SetMinSize(m_frame_minsize);
		CfgMgr* pMgr = CfgMgr::GetInstance();
		if(pMgr == NULL)
		{
			m_frame->Center();
			m_frame->Restore();
		}
		else
		{
			//���������0����ʾ��һ�ε�¼
			if (pMgr->GetWidth() ==0 && pMgr->GetHeight() == 0)
			{
				m_frame->SetSize(wxSize(800, 600));
				m_frame->Center();
				m_frame->Restore();
			}
			else
			{
				m_frame->SetPosition(wxPoint(pMgr->GetPosX(), pMgr->GetPosY()));
				m_frame->SetSize(pMgr->GetWidth(), pMgr->GetHeight());
				if ( pMgr->GetStatus() == 1) // 0 ������ʾ 1 ��� 2 ��С�� 
				{
					m_frame->Maximize(true);
				}
				else
				{
					m_frame->Restore();
				}
			}
		}

        m_frame->Show();
    }
    return true;
}
void MyApp::GetFileName(std::string &filename,bool bWithExt)
{
	char localPath[256];
	memset(localPath, 0, 256);
	GetModuleFileName( NULL, localPath, 256);
    filename=localPath;
    size_t splitpos=filename.find_last_of('\\');
    filename=filename.substr(splitpos+1);
	if(!bWithExt)
	{
		splitpos=filename.find_last_of('.');
		filename=filename.substr(0,splitpos);
	}
}
void MyApp::StartLogSystem()
{
#ifdef _DEBUG
    //wxLog::SetActiveTarget(new wxLogWindow(NULL,"Log"));
#endif
}
void MyApp::StopLogSystem()
{
#ifdef _DEBUG
    //delete wxLog::SetActiveTarget(NULL);
#endif
}

BOOL MyApp::IsDirWriteable()
{
	//std::string strSysPath = GlobalFunc::GetSysPath();
	std::string strApiFile = "DialogRsp.con";
	GlobalFunc::GetPath(strApiFile);
	if(!IsFileWriteable(strApiFile))
	{
		return FALSE;
	}

	strApiFile = "Private.con";
	GlobalFunc::GetPath(strApiFile);
	if(!IsFileWriteable(strApiFile))
	{
		return FALSE;
	}

	strApiFile = "Public.con";
	GlobalFunc::GetPath(strApiFile);
	if(!IsFileWriteable(strApiFile))
	{
		return FALSE;
	}

	strApiFile = "QueryRsp.con";
	GlobalFunc::GetPath(strApiFile);
	if(!IsFileWriteable(strApiFile))
	{
		return FALSE;
	}

	strApiFile = "TradingDay.con";
	GlobalFunc::GetPath(strApiFile);
	if(!IsFileWriteable(strApiFile))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL MyApp::IsFileWriteable( const std::string &strApiFile )
{
	if ( GlobalFunc::IsFileExists(strApiFile))
	{
		DWORD dwAttribute = ::GetFileAttributes(strApiFile.c_str());
		if(dwAttribute & FILE_ATTRIBUTE_READONLY) 
		{
			dwAttribute &= ~FILE_ATTRIBUTE_READONLY; 
			if( !SetFileAttributes(strApiFile.c_str(),dwAttribute))
			{
				return FALSE;
			}
		}
	}
	else
	{
		//�����ļ�
		locale loc = locale::global(locale("")); 
		ofstream file(strApiFile.c_str());
		if(!file.is_open())
		{
			locale::global(loc);
			return FALSE;
		}
		locale::global(loc);
		file.close();
	}

	return TRUE;
}

void MyApp::DeleteOldLogFile(int days)
{
	wxDateTime datetime=wxDateTime::Now().Subtract(wxDateSpan::Days(days-1));
	int limit=datetime.GetYear()*1000000+(datetime.GetMonth()+1)*10000+datetime.GetDay()*100;
	string strPath="log_",strName;
	GlobalFunc::GetPath(strPath);
	GlobalFunc::GetFileName(strName);
	strPath+=strName+"\\";
	wxArrayString strFiles;
	if(wxDirExists(strPath.c_str()))
	{
		wxDir dir(strPath.c_str());
		if(dir.IsOpened())
		{
			wxString filename;
			bool cont = dir.GetFirst(&filename,"*.log", wxDIR_FILES ) ;
			while(cont)
			{
				if(filename.Length()>strlen(".log"))
				{
					if(atoi(filename)<limit)
						strFiles.Add(strPath+filename);
				}
				cont = dir.GetNext(&filename);
			}
		}
	}	
	for (int i=strFiles.GetCount()-1;i>=0;--i)
	{
		::DeleteFile(strFiles[i]);
	}
}
