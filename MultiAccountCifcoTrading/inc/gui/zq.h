#ifndef __MYAPP_H__
#define __MYAPP_H__


//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
class zqWriteLog;
class MainFrame;
class MyApp :	public wxApp
{
public:
	MyApp(void);
	~MyApp(void);
    /***********************************************************
    *   brief   :   Ĭ�ϳ����ʼ������
    *   param   :   nothing
    *   return  :   �ɹ� true ʧ�� false
    ***********************************************************/    
	virtual bool OnInit();
    virtual int OnExit();
    virtual void CleanUp();
    void Release(bool retval=true);
    bool StartMainFrame(bool bInit=false);
    void GetFileName(std::string &filename,bool bWithExt=true);
	bool UnZipXMLFile(string rcid,string strname);
    void StartLogSystem();
    void StopLogSystem();

	//ȥ����ǰĿ¼��ֻ������
	BOOL IsDirWriteable();

	BOOL IsFileWriteable( const std::string &strApiFile );
	void DeleteOldLogFile(int days);
private:
    //��������С�ߴ�
    wxSize m_frame_minsize;
    MainFrame* m_frame;
    zqWriteLog*           m_pWriteLog;
};
// Implements MyApp& wxGetApp()
DECLARE_APP(MyApp)

#endif
