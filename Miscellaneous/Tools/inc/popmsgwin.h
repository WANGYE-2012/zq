//*******************************************************************************************
//	���ã�	���嵯����Ϣ������
//	������	1. ���ഴ��һ���̣߳��߳��д���һ������edit���ڣ�Ӧ�ó��������ô��ڷ��͵�����Ϣ����ʾ�ڴ�����
//			2. �����Ӧ����Win32������
//			3. ע�⣬Ҫ��������������ڣ�ʹ��������CPopMsgWin���󼴿ɡ��в�����DestroyWindow�ķ�ʽ���٣�����ᵼ���ڴ�й¶
//			4. ����ഴ��Edit���ڣ�Edit���ڹ�����ϵͳ��WorkerThread��ʵ�֡�
//			5. ���ڡ�ϵͳ��Edit�����������������ɣ�������ShowWindow��AppendStr�Ĳ���ʱҲ������������������ǰ�᲻���Ͻ����������������û���⡣
//
//	Author		:lzq
//				 Copyright (C), 2008-2028
//*******************************************************************************************

#ifndef POPMSGWIN_H_
#define POPMSGWIN_H_

#ifdef WIN32
#pragma managed(push,off)
#endif 


typedef BOOL (*OPENSERVICEDESKTOP)(HANDLE &hOriDesk,HANDLE &hVisibleDesk);		//�ڷ����У������滷���Ļص�����
typedef BOOL (*CLOSESERVICEDESKTOP)(HANDLE &hOriDesk,HANDLE &hVisibleDesk);		//�ڷ����У��ر����滷���Ļص�����


#include "easymutex.h"
#include "Tools_Win32.h"
#include <string>
using std::string;


//	���Դ�����
class CPopMsgWin
{
public:
	enum eForceShow
	{
		ForceShow,		//�������ʱǿ����ʾ����
		NoForceShow		//��ǿ����ʾ����
	};
	enum eDelOldContent
	{
		DelOldContent,	//�������ʱɾ���ɵ�����
		NoDelOldContent	//��ɾ���ɵ�����
	};
	enum eAddCRLF
	{
		AddCRLF,		//�������ʱ���ӻس�����
		NoAddCRLF		//��ǿ����ʾ����
	};
	enum eInsertTime
	{
		InsertTime,		//�������ʱ����ʱ��
		NoInsertTime	//������ʱ��
	};

    struct Stru_Param
    {
        Stru_Param()
        :   bInServiceID(false),
	        bCreateOKBtn(true),
	        bCreateCancelBtn(false),
	        bClearAsClose(true),
            bToolWindow(false),
            bTopMost(false),
            bMinMaxBox(true),
            bSysMenu(true),
			bDispMessageBorder(true),
            bInitVisible(true),
            InitPox(100),
            InitPoy(100),
            InitWid(320),
            InitHei(240),
            MaxMsgLen(0),
	        TextColor(0),
	        BackgroundColor(0xf8f8f8),
            bForceShow(ForceShow),
            bDelOldContent(NoDelOldContent),
            bAddCRLF(AddCRLF),
            bInsertTime(InsertTime),
			bDestroyWhenClose(false),
			hWnd_PostCloseMsg(0),
			wParam_PostCloseMsg(1000)
        {
            memset(szTitle,0,sizeof(szTitle));
            memset(szTxt_OKBtn,0,sizeof(szTxt_OKBtn));
            strncpy(szTxt_OKBtn,"ȷ��",sizeof(szTxt_OKBtn)-1);
            strncpy(szTxt_CancelBtn,"ȡ��",sizeof(szTxt_CancelBtn)-1);
        };
        void SetScreenCenter(void)
        {
            int ScnWid,ScnHei;
            CTools_Win32::GetWorkAreaSize(ScnWid,ScnHei);
            InitPox=(ScnWid-InitWid)/2;
            InitPoy=(ScnHei-InitHei)/2;
        };

	    bool			bInServiceID;					//�Ƿ��ڷ���ģʽ������
	    bool			bCreateOKBtn;
	    bool			bCreateCancelBtn;
	    bool			bClearAsClose;
        bool            bToolWindow;                    //�Ƿ���ToolWindowģʽ
        bool            bTopMost;                       //�Ƿ���TopMostģʽ
        bool            bMinMaxBox;                     //�Ƿ�������С����ť
        bool            bSysMenu;                       //�Ƿ��SYSMENU
		bool			bDispMessageBorder;				//�Ƿ���ʾ��Ϣ�ı߿�

        bool            bInitVisible;
        int             InitPox;
        int             InitPoy;
        int             InitWid;
        int             InitHei;

        int				MaxMsgLen;						//��Ϣ�ı�����󳤶�

	    unsigned int	TextColor;
	    unsigned int	BackgroundColor;
        char            szTitle[128];                   //��Ϣ����
        char            szTxt_OKBtn[128];               //OK��ť���ı�
        char            szTxt_CancelBtn[128];           //Cancel��ť���ı�

        eForceShow      bForceShow;
        eDelOldContent  bDelOldContent;
        eAddCRLF        bAddCRLF;
        eInsertTime     bInsertTime;

		//�رմ��ڼ����ٶ�������á����OK��Cancel�����Ͻǲ�涼���Թرմ���
		bool			bDestroyWhenClose;				//�رմ���ʱ�Ƿ����ٶ���Ĭ�ϲ����٣�ֻ����
		int				hWnd_PostCloseMsg;				//���ٶ���ʱ���Ƿ���ָ������Post WM_COMMAND��Ϣ��Ĭ��Ϊ0,��Post
		int				wParam_PostCloseMsg;			//���ٶ���ʱ����ָ������Post WM_COMMAND��Ϣ��wParamֵ��lParamΪ�Ƿ�����OK��ť


	};

	CPopMsgWin(const Stru_Param& Param=Stru_Param());
	virtual ~CPopMsgWin();

	void Show(void);
	void Hide(void);

	//	�����������
	void Clear(void);
	//	��һ���ַ�����ӵ�������
	//	bForceShow: ������������صģ�����ʾ����
	//	bAddCRLF: ��ӻس����е���β
	//	bInsertTime: ���ʱ�䵽����
	//	DelOldContent: �Ƿ�ɾ��ԭ��������
	void AppendStr(eForceShow bForceShow,eDelOldContent bDelOldContent,eAddCRLF bAddCRLF,eInsertTime bInsertTime,const char *pStr);
	void AppendStr2(eForceShow bForceShow,eDelOldContent bDelOldContent,eAddCRLF bAddCRLF,eInsertTime bInsertTime,const char* fmt,...);
	void AppendStr3(const char *pStr);
	void AppendStr4(const char* fmt,...);

	//	���ô��ڱ�������
	void SetTitle(const char *pStr);

    //  �жϴ����Ƿ��Ѿ�����
    bool IsWndCreated(void) { return m_hWnd!=NULL&&m_hEdit!=NULL; }

    //  �ȴ����ڴ����ɹ�
    bool WaitWndCreate(int TimeoutMS);


private:
	int GethWnd(void) { return m_hWnd; }					//��ȡ���ھ��

	void CreateWindow_Internal(void);						//��������

	void AppendStr_Internal(eForceShow bForceShow,eDelOldContent bDelOldContent,eAddCRLF bAddCRLF,eInsertTime bInsertTime,const char *pStr);


	OPENSERVICEDESKTOP  m_OpenDesktopFunc;
	CLOSESERVICEDESKTOP m_CloseDesktopFunc;
	HANDLE			    m_hOriDesk;								//�̵߳�ԭʼ������
	HANDLE			    m_hVisibleDesk;							//����������

	//	�����߳�
	static unsigned int WorkerThread(void *arg);

	void OpenDisplayInService_Internal(void);
	void CloseDisplayInService_Internal(void);

private:
    Ceasymutex      m_mutex;

    Stru_Param      m_Param;

	int				m_hFont;
	int				m_hBkBrush;
	int				m_hWnd;									//�����ھ��
	int				m_hEdit;								//Edit���ھ��

	int				m_hThread;								//�߳�
	unsigned long	m_hThreadID;

	int				m_hWinSta_Visible;						//���ӵĽ��̹���վ���
	int				m_hWinSta_Ori;							//ԭʼ�Ľ��̹���վ���
	int				m_hDesktop_Ori;
	int				m_hDesktop_Visible;

public:
    bool            m_bClickOKBtn;                          //�Ƿ����OK��ť

public:
	//���溯������MessageBox���رռ����١����ھ��С�hWnd_PostCloseMsg;���ٶ���ʱ���Ƿ���ָ������Post WM_COMMAND��Ϣ��Ĭ��Ϊ0,��Post��wParam_PostCloseMsg;wParamֵ��lParamΪ�Ƿ�����OK��ť
	static void	CreateMessageBox(const char* content,const char* caption,int wid=320,int hei=240,bool btopmost=true);
	static void	CreateMessageBoxOKCancel(const char* content,const char* caption,int hWnd_PostCloseMsg,int wParam_PostCloseMsg=1000,int wid=320,int hei=240,bool btopmost=true);
	static void	CreateMessageBoxYESNO(const char* content,const char* caption,int hWnd_PostCloseMsg,int wParam_PostCloseMsg=1000,int wid=320,int hei=240,bool btopmost=true);

private:
	string			m_TitleBeforeWndCreated;				//���ڴ�����ǰ��SetTitle���õĴ��ڱ���
	string			m_ContentBeforeWndCreated;				//���ڴ�����ǰ���õĴ�������
};

#ifdef WIN32
#pragma managed(pop)
#endif 


#endif