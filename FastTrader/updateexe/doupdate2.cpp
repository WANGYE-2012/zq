#include "stdafx.h"
#include "doupdate2.h"
#include "updateexe.h"
#include "tmpstackbuf.h"
#include "Tools_Win32.h"
#include "IsProcessExist.h"

char Cdoupdate2::FastTraderFileName[512];

Cdoupdate2::Cdoupdate2()
:   Cdoupdate(),
    m_bUpdateSucc(false)
{
    //ȡ��ǰĿ¼
    char mainpath[512]={0};
    CTools_Win32::GetMainPath(mainpath,sizeof(mainpath));

    char xmlfilename[512]={0};
    HFILE hF;

    //���update���Ƿ���FastTrader_Update.xml
    sprintf_s(xmlfilename,sizeof(xmlfilename)-1,"%s%s\\%s",mainpath,SubPathName_updatefiles,Cdoupdate::m_UpdateFileNameInSvr.c_str());
    hF=_lopen(xmlfilename,OF_READ);
    if(hF==HFILE_ERROR)
    {
        PostThreadMessage(g_idMainThread,WM_QUIT,0,0);
        return;
    }
    _lclose(hF);

    //����FastTrader_Update.xml
    m_UpdateXml.Parse(string(xmlfilename));
    if(!m_UpdateXml.m_bvalid)
    {
        PostThreadMessage(g_idMainThread,WM_QUIT,0,0);
        return;
    }

    //���ȫ�������ļ��Ƿ����
    map<string,bool>::iterator it;
    for(it=m_UpdateXml.m_replacefiles.begin();it!=m_UpdateXml.m_replacefiles.end();it++)
    {
        char sourfile[512]={0},purefilename[512]={0};

        sprintf_s(sourfile,sizeof(sourfile)-1,"%s%s\\%s",mainpath,SubPathName_updatefiles,it->first.c_str());

        //updateexe.exe��FastTrader������
        CTools_Ansi::GetPureFileName(sourfile,purefilename,sizeof(purefilename)-1);
        if(string(purefilename)==string(UpdateExeName))
            continue;               

        hF=_lopen(sourfile,OF_READ);
        if(hF==HFILE_ERROR)
        {
            PostThreadMessage(g_idMainThread,WM_QUIT,0,0);
            return;
        }
        _lclose(hF);
    }

    //ÿ��200ms�ж�FastTrader.exe�����Ƿ���ڣ�����20��(4s)
    memset(FastTraderFileName,0,sizeof(FastTraderFileName));
    sprintf_s(FastTraderFileName,sizeof(FastTraderFileName)-1,"%sFastTrader.exe",mainpath);

    //��ʱ���͵Ľ����ն˽����Ƿ��Ѿ�ֹͣ
    SetTimer(NULL,1001,100,TimerCBFunc);

    //FastTrader.exe��δ�˳�����ʾ�û�
    while(CIsProcessExist(string(FastTraderFileName)).m_bExist)
    {
        int irlt=MessageBox(
            NULL,
            "���ڵȴ������ն��˳���������������Ժ�......\n\n"
            "���[ȡ��]��ȡ���˴�����",
            MessageTitle,
            MB_OKCANCEL);
        if(irlt==IDCANCEL)
        {
            PostThreadMessage(g_idMainThread,WM_QUIT,0,0);
            return;
        }
    }

    KillTimer(NULL,1001);

    //��updateĿ¼�µ������ļ����Ƶ���ǰĿ¼�£���ɾ��ԭ���ļ�
    for(it=m_UpdateXml.m_replacefiles.begin();it!=m_UpdateXml.m_replacefiles.end();it++)
    {
        char sourfile[512]={0},destfile[512]={0},purefilename[512]={0};

        sprintf_s(sourfile,sizeof(sourfile)-1,"%s%s\\%s",mainpath,SubPathName_updatefiles,it->first.c_str());
        sprintf_s(destfile,sizeof(destfile)-1,"%s%s",mainpath,it->first.substr(m_UpdateXml.m_version_svrpath.size()+1).c_str());

        //updateexe.exe��FastTrader������
        CTools_Ansi::GetPureFileName(sourfile,purefilename,sizeof(purefilename)-1);
        if(string(purefilename)==string(UpdateExeName))
            continue;               

        if(!CopyFile(sourfile,destfile,FALSE))
        {
            char buf[256]={0};
            sprintf_s(buf,sizeof(buf)-1,"�ļ��������ɹ�(%s)������ʧ�ܡ�",purefilename);

            MessageBox(NULL,buf,MessageTitle,MB_OK);
            PostThreadMessage(g_idMainThread,WM_QUIT,0,0);
            return;
        }
        unlink(sourfile);
    }

    m_bUpdateSucc=true;

    char buf[256]={0};
    sprintf_s(buf,sizeof(buf)-1,"FastTrader�����ɹ�����ǰ�汾��Ϊ%s��\n\n���[ȷ��]��Ϊ�����������նˡ�",m_UpdateXml.m_version.c_str());
    MessageBox(NULL,buf,MessageTitle,MB_OK);

    //����FastTrader.exe
    char pathfilename[512]={0};
    sprintf_s(pathfilename,sizeof(pathfilename)-1,"%s%s",mainpath,"FastTrader.exe");
    WinExec(pathfilename,SW_SHOW);

    //�˳���������
    PostThreadMessage(g_idMainThread,WM_QUIT,0,0);
}
Cdoupdate2::~Cdoupdate2()
{
}

//��ʱ���͵Ľ����ն˽����Ƿ��Ѿ�ֹͣ
void CALLBACK Cdoupdate2::TimerCBFunc( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
    if(!CIsProcessExist(string(FastTraderFileName)).m_bExist)
    {
        HWND hWnd=FindWindow(NULL,MessageTitle);
        if(hWnd!=NULL)
            PostMessage(hWnd,WM_COMMAND,IDOK,0);
    }
}

