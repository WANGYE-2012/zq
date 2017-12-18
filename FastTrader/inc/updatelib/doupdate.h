#pragma once


#include "stdafx.h"
#include "MyXML.h"
#include "TcpClientWorker.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include "parseupdatexml.h"
#include "VersionInfo2.h"
#include "popmsgwin.h"

#include <string>
using std::string;

#define XMLName_AutoUpdate_InFastTrader     "AutoUpdate.xml"
#define SubPathName_updatefiles             "update"        //ԭʼ�����ļ���FastTrader�µ���Ŀ¼��
#define UpdateExeName                       "updateexe.exe"
#define MessageTitle                        "FastTrader������ʾ"



class Cdoupdate
{
public:
    //MainThreadID: �����˳�FastTrader
    Cdoupdate();
    ~Cdoupdate();
    void start(void);

public:
    //��tcp�õ����ݺ󣬵��ô˻ص��������д���
    static void CB_RcvData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
    //�Ͽ�tcp���Ӻ󣬵��ô˻ص��������д���
    static void CB_Disconn(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
    //���ӳɹ��󣬵��ô˻ص��������д���
    static void CB_ConnSucc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
    //����ʧ�ܺ󣬵��ô˻ص��������д���
    static void CB_ConnFail(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
    //���ڵ��õĻص�������һ��ÿ������һ�Σ��������ڼ��������ʱ��
    static void CB_OnTime(int UserData);


private:
    void stop_Internal(void);

    //��AutoUpdate.xml��ȡ��������������ַ�����������ļ���
    void GetParamInAutoUpdateFile_Internal();

    //���������ļ��İ�, ��sendData��һ����װ��
    bool SendQryFilePkg_Internal(unsigned int nCMDID, const void* pData, int nLen, unsigned int seq,
        unsigned int Subseq=0,unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0);
    
    //��������������������д���ļ�������Ϊ�ļ���(\0��β) + �ļ����ݡ�psubpathΪд�����Ŀ¼��������б�ܡ�
    bool WritePkgToFile_Internal(const char* subpath,const void* pData,int len,string& Out_PathFileName);

    //����һ����Ч���ݰ�
    void ProcessPkg_Internal(const Stru_UniPkgHead& head,const void* pData,int len);

	static DWORD ThreadWorker(void *arg);

public:
    //FastTrader�������ھ���������˳�
    HWND m_hMainWnd_FastTrader;
    //�Ƿ��Ѿ���ʼ��½������Ѿ���ʼ��½������ǰ��ʾ�û�ȷ��
    bool m_bStartLogin;

    //doupdate��ʵ��
    static Cdoupdate* m_pObj;


protected:
    Ceasymutex m_mutex;

    //������������ַ
    Stru_IPAddress m_SvrAddr;
    //(�������ϵ�)���������ļ�������FastTrader_Update.xml
    string m_UpdateFileNameInSvr;
    //Ҫ���Եİ汾��
    string m_IgnoreVersion;

    CTcpClientWorker *m_pTcpClient;
    static const int m_RcvBufSize=50*1024*1024;
    int m_RcvDataLen;
    char* m_pRcvData;


    enum eupdatestep
    {
        Step_NotStart,
        Step_GetUpdateFile,
        Step_WaitUserConfirmUpdate,
        Step_DownloadFiles,
        Step_WaitUserConfirmCloseFastTrader,
        Step_Complete,
        Step_Stop
    };

    eupdatestep m_step;

    //���������ص�update.xml�Ľ�������
    CParseUpdateXml m_UpdateXml;

    //��ʾ�������ݵĴ���
    CPopMsgWin *m_pmsgwin;

    //����ʱ�䣬����3������Ϊ��ʱ���˳�����
    time_t      m_StartSeconds;

    static int             m_hThread;
    static unsigned long   m_hThreadID;

    //�Ƿ���MessageBox�С�����ʱ��
    bool m_bInMessageBox;
    //�Ƿ��ڹر���
    bool m_bClosing;

    //�������ݽ������ݵ�ʱ�䡣�����ж����糬ʱ����ʱʱ�����趨Ϊ60s
    time_t SendTime;
    time_t RecvTime;


    //����������������
	Cdoupdate(const Cdoupdate&);
	Cdoupdate& operator= (const Cdoupdate&);
};
