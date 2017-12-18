#include "stdafx.h"
#include "doupdate.h"
#include "tmpstackbuf.h"
#include "Tools_Win32.h"
#include "VersionInfo2.h"
#include "IsProcessExist.h"
#include "EventParam.h"
#include "FileOpr.h"
#include <assert.h>


#define TimeOutSeconds      180                         //���ӳ�ʱʱ��
#define CMD_TcpEvt          1001

#define EvtID_ConnSucc      1002
#define EvtID_ConnFail      1003
#define EvtID_Disconn       1004
#define EvtID_Ontime        1005
#define EvtID_RcvData       1006

int             Cdoupdate::m_hThread=0;
unsigned long   Cdoupdate::m_hThreadID=0;
Cdoupdate*      Cdoupdate::m_pObj=NULL;;


Cdoupdate::Cdoupdate()
:   m_pTcpClient(NULL)
    ,m_RcvDataLen(0)
    ,m_pRcvData(NULL)
    ,m_step(Step_NotStart)
    ,m_UpdateXml(string(""))
    ,m_pmsgwin(NULL)
{
    m_pObj=this;

    m_bInMessageBox=false;
    m_bClosing=false;
    m_bStartLogin=false;

    m_hMainWnd_FastTrader=NULL;

    SendTime=time(NULL);
    RecvTime=time(NULL);

    m_pRcvData=(char*)malloc(m_RcvBufSize);

    //ȡ��������������ַ�����������ļ���
    GetParamInAutoUpdateFile_Internal();
    if(m_SvrAddr.IP==0||m_SvrAddr.Port==0||m_UpdateFileNameInSvr.empty()) 
        return;

    m_step=Step_NotStart;
}

Cdoupdate::~Cdoupdate()
{
    m_bClosing=true;

    if(m_bInMessageBox)
    {
        //�ر����ڴ򿪵�MessageBox
        HWND hWnd=FindWindow(NULL,MessageTitle);
        if(hWnd!=NULL)
        {
            PostMessage(hWnd,WM_CLOSE,0,0);
            PostMessage(hWnd,WM_COMMAND,IDCANCEL,0);
            PostMessage(hWnd,WM_COMMAND,IDNO,0);
        }
    }

    m_mutex.lock();
    if(m_pTcpClient) delete m_pTcpClient;
    if(m_pmsgwin) delete m_pmsgwin;
    m_mutex.unlock();

	if(m_hThreadID)
	{
		//Post WM_QUIT��GetMessage�˳�
		PostThreadMessage(m_hThreadID,WM_QUIT,0,0);
        m_hThreadID=0;
	}
	if(m_hThread)
	{
	    DWORD			ExitCode;
		WaitForSingleObject((HANDLE)m_hThread,8000);
		if(GetExitCodeThread((HANDLE)m_hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)m_hThread,0);
		CloseHandle((HANDLE)m_hThread);
        m_hThread=0;
	}

    if(m_pRcvData)
    {
        free(m_pRcvData);
        m_pRcvData=NULL;
    }

    m_pObj=NULL;

}
void Cdoupdate::start(void)
{
    m_StartSeconds=time(NULL);
    m_step=Step_GetUpdateFile;

	m_hThread=(int)CreateThread(NULL,65536,(LPTHREAD_START_ROUTINE)ThreadWorker,this,0,&m_hThreadID);
    CFileOpr::getObj().writelocallog("threadid","TID:\t% 8u\tCdoupdate::ThreadWorker",m_hThreadID);

    //������������������TcpClient����
    m_pTcpClient=new CTcpClientWorker(
        Stru_IPAddress(),
        m_SvrAddr,
		CB_RcvData,
		CB_Disconn,
		NULL,
		CB_ConnSucc,
		CB_ConnFail,
		CB_OnTime,
		1*1024*1024,
        10*1024*1024,
        0,
        0);

    if(!m_pTcpClient) 
    {
        m_step=Step_Stop;
    }
}

DWORD Cdoupdate::ThreadWorker(void *arg)
{
    Cdoupdate *pInst=(Cdoupdate*)arg;
    if(!pInst) return 0xffffffff;


	MSG				msg;
    SetMessageQueue(1024);
	while(GetMessage(&msg,NULL,0,0))
	{
        if(msg.message==WM_COMMAND)
        {
            if(LOWORD(msg.wParam)==CMD_TcpEvt&&pInst->m_step!=Step_Stop&&pInst->m_step!=Step_Complete)
            {
                unsigned int EventParamID=(unsigned int)msg.lParam;
                unsigned int EvtID;
                static char DataBuf[10*1024*1024];
                int DataLen=0;
                int hSocket;
                int UserData;

                if(CEventParam::getObj().GetEventParam2(false,EventParamID,&EvtID,NULL,DataBuf,&DataLen,sizeof(DataBuf),NULL,NULL,0,&hSocket,&UserData))
                {
                    switch(EvtID)
                    {
                        case EvtID_ConnSucc:      //���ӳɹ�
                        {
                            CeasymutexGuard guard(pInst->m_mutex);
                            if(pInst->m_step==Step_GetUpdateFile)
                                pInst->SendQryFilePkg_Internal(CMDID_QryFile, pInst->m_UpdateFileNameInSvr.c_str(), pInst->m_UpdateFileNameInSvr.size(), 0);
                            else if(pInst->m_step==Step_DownloadFiles)
                            {
                                assert(pInst->m_UpdateXml.m_replacefiles.size()>0);
                                if(pInst->m_pmsgwin) pInst->m_pmsgwin->AppendStr4("1/%d \t����%s......",pInst->m_UpdateXml.m_replacefiles.size(),pInst->m_UpdateXml.m_replacefiles.begin()->first.c_str());
                                pInst->SendQryFilePkg_Internal(CMDID_QryFile, pInst->m_UpdateXml.m_replacefiles.begin()->first.c_str(), pInst->m_UpdateXml.m_replacefiles.begin()->first.size(), 0);
                            }
                            break;
                        }
                        case EvtID_ConnFail:    //����ʧ��
                        {
                            CeasymutexGuard guard(pInst->m_mutex);
                            eupdatestep step=pInst->m_step;
                            pInst->stop_Internal();
                            if(step==Step_DownloadFiles)
                            {
                                pInst->m_bInMessageBox=true;
                                MessageBox(NULL,"���ӷ�����ʧ�ܣ������������ɹ���",MessageTitle,MB_OK);
                                pInst->m_bInMessageBox=false;
                            }
	                        if(pInst->m_hThreadID) PostThreadMessage(pInst->m_hThreadID,WM_QUIT,0,0);
                            break;
                        }
                        case EvtID_Disconn:     //�Ͽ�����
                        {
                            CeasymutexGuard guard(pInst->m_mutex);
                            bool bneedstop=false;
                            eupdatestep step=pInst->m_step;
                            if(step==Step_GetUpdateFile||step==Step_DownloadFiles)
                            {
                                pInst->stop_Internal();
                                bneedstop=true;
                            }
                            if(step==Step_DownloadFiles)
                            {
                                pInst->m_bInMessageBox=true;
                                MessageBox(NULL,"���ӷ�����ʧ�ܣ������������ɹ���",MessageTitle,MB_OK);
                                pInst->m_bInMessageBox=false;
                            }
                            if(bneedstop)
                            {
       	                        if(pInst->m_hThreadID) PostThreadMessage(pInst->m_hThreadID,WM_QUIT,0,0);
                            }

                            break;
                        }
                        case EvtID_Ontime:      //��ʱ�¼�
                        {
                            CeasymutexGuard guard(pInst->m_mutex);

                            //����������ļ��׶Σ�MsgWin���ر�(����ȡ����ť)���˳�����
                            if(pInst->m_step==Step_DownloadFiles&&
                                pInst->m_pmsgwin&&pInst->m_pmsgwin->m_bClickOKBtn)
                            {
                                pInst->stop_Internal();
	                            if(pInst->m_hThreadID) PostThreadMessage(pInst->m_hThreadID,WM_QUIT,0,0);
                            }
                            else if(pInst->m_step==Step_GetUpdateFile||pInst->m_step==Step_DownloadFiles)
                            {
                                time_t curtime=time(NULL);
                                if(curtime-pInst->SendTime>=60&&curtime-pInst->RecvTime>=60)
                                {
                                    pInst->stop_Internal();
                                    pInst->m_bInMessageBox=true;
                                    MessageBox(NULL,"���������ļ���ʱ�������������ɹ���",MessageTitle,MB_OK);
                                    pInst->m_bInMessageBox=false;
   	                                if(pInst->m_hThreadID) PostThreadMessage(pInst->m_hThreadID,WM_QUIT,0,0);
                                }
                            }
                            break;
                        }
                        case EvtID_RcvData:     //�յ�����
                        {
                            CeasymutexGuard guard(pInst->m_mutex);
                            pInst->RecvTime=time(NULL);

                            if(pInst->m_pRcvData&&pInst->m_RcvDataLen+DataLen<=m_RcvBufSize) 
	                        {
		                        memcpy(pInst->m_pRcvData+pInst->m_RcvDataLen,DataBuf,DataLen);
		                        pInst->m_RcvDataLen+=DataLen;

	                            //ѭ���������յ�������
		                        int off=0;
		                        while(true)
		                        {
			                        //���������ܳ��ֵ���Ч����
			                        int lastoff=off;
			                        while(off<pInst->m_RcvDataLen&&Stru_UniPkgHead::IsInvalidPkgHead(pInst->m_pRcvData+off,pInst->m_RcvDataLen-off)) off++;

			                        int LengthUsedByPkg;
			                        if(!Stru_UniPkgHead::IsValidPkg(pInst->m_pRcvData+off,pInst->m_RcvDataLen-off,&LengthUsedByPkg))
				                        break;

                                    //����һ����Ч���ݰ�
                                    pInst->ProcessPkg_Internal(*(Stru_UniPkgHead*)(pInst->m_pRcvData+off),pInst->m_pRcvData+off+sizeof(Stru_UniPkgHead),LengthUsedByPkg-sizeof(Stru_UniPkgHead)-4);

			                        off+=LengthUsedByPkg;

                                    if(pInst->m_bClosing) break;
		                        }

		                        if(off>0)
	                            {
			                        memmove(pInst->m_pRcvData,pInst->m_pRcvData+off,pInst->m_RcvDataLen-off);
		                            pInst->m_RcvDataLen -= off;
	                            }
	                        }
                            break;
                        }
                    }

                }
                CEventParam::getObj().DeleteEventParam(EventParamID);
            }
        }

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

    m_hThreadID=0;

    return 0;
}

//���ӳɹ�
void Cdoupdate::CB_ConnSucc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
    if(!m_hThreadID) return;
    unsigned int EventParamID;
    if(CEventParam::getObj().CreateEventParam(EventParamID,EvtID_ConnSucc,NULL,NULL,0,(int)hSocket,UserData))
        PostThreadMessage(m_hThreadID,WM_COMMAND,CMD_TcpEvt,EventParamID);
}

//����ʧ�ܣ�ֱ���˳�����
void Cdoupdate::CB_ConnFail(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
    if(!m_hThreadID) return;
    unsigned int EventParamID;
    if(CEventParam::getObj().CreateEventParam(EventParamID,EvtID_ConnFail,NULL,NULL,0,(int)hSocket,UserData))
        PostThreadMessage(m_hThreadID,WM_COMMAND,CMD_TcpEvt,EventParamID);
}

//�Ͽ�tcp���ӣ�ֱ���˳�����
void Cdoupdate::CB_Disconn(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
    if(!m_hThreadID) return;
    unsigned int EventParamID;
    if(CEventParam::getObj().CreateEventParam(EventParamID,EvtID_Disconn,NULL,NULL,0,(int)hSocket,UserData))
    {
        BOOL brlt=PostThreadMessage(m_hThreadID,WM_COMMAND,CMD_TcpEvt,EventParamID);
        if(!brlt) 
            brlt=FALSE;
    }
}

//���ڵ��õĻص�������һ��ÿ������һ��
void Cdoupdate::CB_OnTime(int UserData)
{
    if(!m_hThreadID) return;
    unsigned int EventParamID;
    if(CEventParam::getObj().CreateEventParam(EventParamID,EvtID_Ontime,NULL,NULL,0,0,UserData))
        PostThreadMessage(m_hThreadID,WM_COMMAND,CMD_TcpEvt,EventParamID);
}
//��tcp�õ����ݺ󣬵��ô˻ص��������д���
void Cdoupdate::CB_RcvData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
    if(!m_hThreadID) return;
    unsigned int EventParamID;
    if(CEventParam::getObj().CreateEventParam(EventParamID,EvtID_RcvData,NULL,pData,DataLen,(int)hSocket,UserData))
        PostThreadMessage(m_hThreadID,WM_COMMAND,CMD_TcpEvt,EventParamID);
}


void Cdoupdate::stop_Internal(void)
{
    if(m_pTcpClient) { delete m_pTcpClient;  m_pTcpClient=NULL; }
    if(m_pmsgwin) { delete m_pmsgwin;  m_pmsgwin=NULL; }

    m_step=Step_Stop;
}

//��AutoUpdate.xml��ȡ��������������ַ�����������ļ���
void Cdoupdate::GetParamInAutoUpdateFile_Internal()
{
    m_SvrAddr.IP=0;
    m_SvrAddr.Port=0;
    m_UpdateFileNameInSvr.clear();

    CMyXML xml(XMLName_AutoUpdate_InFastTrader);
    if(!xml.m_bvalid) return;
    string strIP;
    int Port;
	if(!xml.GetNodeContent(strIP,"update","ServerIP")) return;
	if(!xml.GetNodeContentInt(Port,"update","ServerPort")) return;
	if(!xml.GetNodeContent(m_UpdateFileNameInSvr,"update","updatefile")) return;
	xml.GetNodeContent(m_IgnoreVersion,"update","ignoreversion");

    CTools_Ansi::PerformWSAStartup();
    bool brlt=CTools_Ansi::GetIPFromHostName2(strIP.c_str(),m_SvrAddr.IP);
    m_SvrAddr.Port=(unsigned short)Port;
    CTools_Ansi::PerformWSACleanup();

    return;
}


//����һ����Ч���ݰ�
void Cdoupdate::ProcessPkg_Internal(const Stru_UniPkgHead& head,const void* pData,int len)
{
    if(head.cmdid==CMDID_RspQryFileFail)
    {
        //������������ļ�ʧ�ܣ��˳�����
        eupdatestep step=m_step;
        stop_Internal();

        if(step==Step_DownloadFiles)
        {
            m_bInMessageBox=true;
            MessageBox(NULL,"���������ļ�ʧ�ܣ��������ɹ���",MessageTitle,MB_OK);
            m_bInMessageBox=false;
        }
        if(m_hThreadID) PostThreadMessage(m_hThreadID,WM_QUIT,0,0);
        return;
    }

    if(head.cmdid!=CMDID_RspQryFile) return;

    //ǰ�����ļ���(\0��β)��
    int len_filenamelen=strnlen((const char*)pData,len);
    string strFileName((const char*)pData,len_filenamelen);
    string strPathFileName;
    if(len_filenamelen>=len||
        !WritePkgToFile_Internal(SubPathName_updatefiles,pData,len,strPathFileName))
    {
        eupdatestep step=m_step;
        stop_Internal();

        if(step==Step_DownloadFiles)
        {
            m_bInMessageBox=true;
            MessageBox(NULL,"���������ļ�ʧ�ܣ��������ɹ���",MessageTitle,MB_OK);
            m_bInMessageBox=false;
        }
        if(m_hThreadID) PostThreadMessage(m_hThreadID,WM_QUIT,0,0);
        return;
    }

    if(m_step==Step_GetUpdateFile)
    {
        //�������������ص�update����(��FastTrader_Update.xml�е�����)

        //�����ļ�����Ҫ��m_UpdateFileNameInSvr������һ�¡�������Ϊ���ݴ����˳���������
        if(strFileName!=m_UpdateFileNameInSvr)
        {
            stop_Internal();
            m_bInMessageBox=true;
            MessageBox(NULL,"�����ļ������������ɹ���",MessageTitle,MB_OK);
            m_bInMessageBox=false;
            if(m_hThreadID) PostThreadMessage(m_hThreadID,WM_QUIT,0,0);
            return;
        }

        //����update.xml�ļ�
        m_UpdateXml.Parse(strPathFileName);
        if(!m_UpdateXml.m_bvalid)
        {
            stop_Internal();
            m_bInMessageBox=true;
            MessageBox(NULL,"�����ļ����������������ɹ���",MessageTitle,MB_OK);
            m_bInMessageBox=false;
            if(m_hThreadID) PostThreadMessage(m_hThreadID,WM_QUIT,0,0);
            return;
        }

        //ȡ����FastTrader.exe�İ汾��
        char localFileName[512]={0};
        CTools_Win32::GetMainPath(localFileName,sizeof(localFileName));
        strcat_s(localFileName,sizeof(localFileName)-strlen(localFileName),"FastTrader.exe");
        string localversion=CVersionInfo2(localFileName).GetFileVersionAsString();


        //���������汾���Ƿ���ߣ��Լ�AutoUpdate.xml���Ƿ���Դ˰汾
        int icomparerlt=CParseUpdateXml::CompareVersion(localversion,m_UpdateXml.m_version);
        if(icomparerlt>=0||
            !m_IgnoreVersion.empty()&&CParseUpdateXml::CompareVersion(m_IgnoreVersion,m_UpdateXml.m_version)==0)
        {
            //����Ҫ����

            stop_Internal();
            if(m_hThreadID) PostThreadMessage(m_hThreadID,WM_QUIT,0,0);
            return;
        }

        //���û����Ҫ���µ��ļ����˳�����
        if(m_UpdateXml.m_replacefiles.size()==0)
        {
            stop_Internal();
            if(m_hThreadID) PostThreadMessage(m_hThreadID,WM_QUIT,0,0);
            return;
        }

        if(!m_pTcpClient)
        {
            stop_Internal();
            if(m_hThreadID) PostThreadMessage(m_hThreadID,WM_QUIT,0,0);
            return;
        }

        m_step=Step_WaitUserConfirmUpdate;

        //�Ͽ�Tcp����
        CSocketBase::CloseSocket(m_pTcpClient->getSocket());


        //������ذ汾�ŵ���Ҫ�����Ͱ汾�ţ������Զ�����
        if((icomparerlt=CParseUpdateXml::CompareVersion(localversion,m_UpdateXml.m_version_min))<0)
        {
            stop_Internal();
            m_bInMessageBox=true;
            MessageBox(NULL,"���ذ汾�Ź��ͣ������Զ����������������°汾���°�װ��",MessageTitle,MB_OK);
            m_bInMessageBox=false;
            if(m_hThreadID) PostThreadMessage(m_hThreadID,WM_QUIT,0,0);
            return;
        }


        //���û��Ƿ���Ҫ���´˰汾
        string strquestion=string("��⵽�и��µİ汾���Ƿ��������£�\n\n")+
                            string("�°汾��  ��")+CParseUpdateXml::FormatVersion(m_UpdateXml.m_version)+string("\n")+
                            string("���ذ汾�ţ�")+CParseUpdateXml::FormatVersion(localversion);
        if(!m_UpdateXml.m_note.empty()) strquestion+=string("\n\n����˵��  ��")+m_UpdateXml.m_note;
        m_bInMessageBox=true;
        int i=MessageBox(NULL,strquestion.c_str(),MessageTitle,MB_YESNO|MB_SYSTEMMODAL);
        m_bInMessageBox=false;
        if(m_bClosing) return;
        if(i==IDNO)
        {
            stop_Internal();
            m_bInMessageBox=true;
            i=MessageBox(NULL,"��ѡ���˲��������£��Ժ��Ƿ���Դ˰汾���£�",MessageTitle,MB_YESNO);
            m_bInMessageBox=false;
            if(i==IDYES)
            {
                CMyXML(XMLName_AutoUpdate_InFastTrader).ModifyOrAppendNode_save(CMyXML(string("ignoreversion"),m_UpdateXml.m_version),"update");
            }
            if(m_hThreadID) PostThreadMessage(m_hThreadID,WM_QUIT,0,0);
            return;
        }

        //��ʾ���ؽ��̴���
        CPopMsgWin::Stru_Param MsgWinParam;
        MsgWinParam.InitWid=480;
        MsgWinParam.InitHei=360;
        MsgWinParam.SetScreenCenter();
        MsgWinParam.bSysMenu=false;
        MsgWinParam.bMinMaxBox=false;
        MsgWinParam.bForceShow=CPopMsgWin::ForceShow;
        MsgWinParam.bDelOldContent=CPopMsgWin::NoDelOldContent;
        MsgWinParam.bAddCRLF=CPopMsgWin::AddCRLF;
        MsgWinParam.bInsertTime=CPopMsgWin::NoInsertTime;

        strncpy(MsgWinParam.szTitle,"����FastTrader",sizeof(MsgWinParam.szTitle)-1);
        m_pmsgwin=new CPopMsgWin(MsgWinParam);
        m_pmsgwin->WaitWndCreate(4000);
        m_pmsgwin->AppendStr3("��ʼ���������ļ�");

        //���濪ʼ���ظ����ļ�
        m_step=Step_DownloadFiles;

        //����tcp�ٴ�����
        m_pTcpClient->SetReconnect();
    }
    else if(m_step==Step_DownloadFiles)
    {
        //����������ļ��׶Σ�MsgWin���ر�(����ȡ����ť)���˳���������
        if(m_pmsgwin&&m_pmsgwin->m_bClickOKBtn)
        {
            stop_Internal();
            if(m_hThreadID) PostThreadMessage(m_hThreadID,WM_QUIT,0,0);
            return;
        }

        map<string,bool>::iterator it=m_UpdateXml.m_replacefiles.find(strFileName);
        if(it!=m_UpdateXml.m_replacefiles.end())
            m_UpdateXml.m_replacefiles[strFileName]=true;

        //��ʾ�Ѿ����ص��ļ���Ϣ
        int DownloadedCnt=0;
        for(it=m_UpdateXml.m_replacefiles.begin();it!=m_UpdateXml.m_replacefiles.end();it++)
            if(it->second==true) DownloadedCnt++;

        for(it=m_UpdateXml.m_replacefiles.begin();it!=m_UpdateXml.m_replacefiles.end();it++)
        {
            if(it->second==false) 
            {
                char buf[512];
                sprintf_s(buf,sizeof(buf),"%d/%d \t����%s......",DownloadedCnt+1,m_UpdateXml.m_replacefiles.size(),it->first.c_str());
                buf[sizeof(buf)-1]=0;
                m_pmsgwin->AppendStr3(buf);

                SendQryFilePkg_Internal(CMDID_QryFile, it->first.c_str(), it->first.size(), 0);
                break;
            }
        }

        if(it==m_UpdateXml.m_replacefiles.end())
        {
            //��ʾ��������ɣ�����Ҫ�����ˡ�
            m_pmsgwin->AppendStr3("�������");

            //�ر�TcpClient
            if(m_pTcpClient) { delete m_pTcpClient;  m_pTcpClient=NULL; }

            m_step=Step_WaitUserConfirmCloseFastTrader;

            //����Ѿ���ʼ��½����ʾ�û����رտͻ���
            if(m_bStartLogin)
            {
                m_bInMessageBox=true;
			    int irlt=MessageBox(NULL,"����������Ҫ�رս����ն������������\n\n���[ȷ��]���رս����նˣ����[ȡ��]��ȡ���˴�������",MessageTitle,MB_OKCANCEL);
                m_bInMessageBox=false;
                if(m_bClosing) return;
                if(irlt==IDCANCEL)
                {
                    m_bInMessageBox=true;
                    irlt=MessageBox(NULL,"ȷ��ȡ���˴������𣿡�",MessageTitle,MB_YESNO);
                    m_bInMessageBox=false;
                    if(irlt==IDYES)
                    {
                        stop_Internal();
                        if(m_hThreadID) PostThreadMessage(m_hThreadID,WM_QUIT,0,0);
                        return;
                    }
                }
            }

            //�������
            char mainpath[512]={0};
            CTools_Win32::GetMainPath(mainpath,sizeof(mainpath));
            char updateexename[512]={0};
            char updateexename2[512]={0};
            sprintf_s(updateexename,sizeof(updateexename)-1,"%s%s",mainpath,UpdateExeName);
            sprintf_s(updateexename2,sizeof(updateexename2)-1,"%s%s\\%s\\%s",mainpath,SubPathName_updatefiles,m_UpdateXml.m_version_svrpath.c_str(),UpdateExeName);

            //�Ƚ����ص�updateexe.exe��������
            HFILE hF=_lopen(updateexename2,OF_READ);
            if(hF!=HFILE_ERROR)
            {
                _lclose(hF);
                if(!CopyFile(updateexename2,updateexename,FALSE))
                {
                    stop_Internal();
                    m_bInMessageBox=true;
                    char buf[256]={0};
                    sprintf_s(buf,sizeof(buf)-1,"�ļ��������ɹ�(%s)������ʧ�ܡ�",UpdateExeName);
                    MessageBox(NULL,buf,MessageTitle,MB_OK);
                    m_bInMessageBox=false;
                    if(m_hThreadID) PostThreadMessage(m_hThreadID,WM_QUIT,0,0);
                    return;
                }
                unlink(updateexename2);
            }

            //����updateexe.exe�������
            m_step=Step_Complete;
            if(!CIsProcessExist(updateexename).m_bExist)
    	        WinExec(updateexename,SW_HIDE);

            //�ر�����
            for(int i=0;i<30;i++)
            {
                if(m_hMainWnd_FastTrader&&::IsWindow(m_hMainWnd_FastTrader))
                {
                    PostMessage(m_hMainWnd_FastTrader,WM_CLOSE,0x9821,0);
                    break;
                }
                Sleep(100);
            }
        }
    }
}

//��������������������д���ļ�������Ϊ�ļ���(\0��β) + �ļ����ݡ�psubpathΪд�����Ŀ¼��������б�ܡ�
bool Cdoupdate::WritePkgToFile_Internal(const char* subpath,const void* pData,int len,string& Out_PathFileName)
{
    //�з��ļ���ָ����ļ�����ָ��
    int len_filename=strnlen((const char*)pData,len);
    if(len_filename==0||len_filename>=len) return false;
    int len_content=len-len_filename-1;
    const char* pfilename=(const char*)pData;
    const char* pcontent=(const char*)pData+len_filename+1;

    //����ȷ��д���ļ���pathfilename   
    char mainpath[512];
    CTools_Win32::GetMainPath(mainpath,sizeof(mainpath));

    char pathfilename[512]={0};
    if(subpath&&(int)strlen(subpath)>0)
        sprintf_s(pathfilename,sizeof(pathfilename)-1,"%s%s\\%s",mainpath,subpath,pfilename);
    else sprintf_s(pathfilename,sizeof(pathfilename)-1,"%s%s",mainpath,pfilename);

    //д���ļ�
    HFILE hF=_lcreat(pathfilename,0);
    if(hF==HFILE_ERROR) 
    {
        //���Դ���Ŀ¼
        char ipath[512];
        if(CTools_Ansi::GetFilePathWithDiskSymbol(pathfilename,ipath,sizeof(ipath)))
        {
            CTools_Win32::CreateMultipleDirectory(ipath);
            hF=_lcreat(pathfilename,0);
        }
    }

    if(hF==HFILE_ERROR) return false;
    _lwrite(hF,pcontent,len_content);
    _lclose(hF);

    Out_PathFileName.assign(pathfilename);

    return true;
}


//���������ļ��İ�, ��sendData��һ����װ��update.xml���ü�ExtPath�������ļ���Ҫ����
bool Cdoupdate::SendQryFilePkg_Internal(unsigned int nCMDID, const void* pData, int nLen, unsigned int seq,
                                        unsigned int Subseq,unsigned int Userdata1,unsigned int Userdata2,unsigned int Userdata3,unsigned int Userdata4)
{
    if(m_pTcpClient==NULL||m_pTcpClient->getConnStatus()!=TcpClientConnStatus_Connected)
        return false;
	
    int PkgLen=sizeof(Stru_UniPkgHead)+nLen+4;
	AllocTmpStackBuf(tmpbuf,PkgLen);
	if(tmpbuf.m_pbuf==NULL)
        return false;

    Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)tmpbuf.m_pbuf;
	PkgHead.Set(nLen,nCMDID,0,seq,Subseq,Userdata1,Userdata2,Userdata3,Userdata4);
    int off=sizeof(Stru_UniPkgHead);
	if(pData&&nLen>0)
    {
		memcpy((char*)tmpbuf.m_pbuf+off,pData,nLen);
        off+=nLen;
    }
	*(unsigned int*)((char*)tmpbuf.m_pbuf+off)=TailID_UniPkg;
    off+=4;
	int sentlen = m_pTcpClient->SendData(tmpbuf.m_pbuf,PkgLen);
    SendTime=time(NULL);

	if(sentlen==PkgLen) return true;
    else return false;
}


