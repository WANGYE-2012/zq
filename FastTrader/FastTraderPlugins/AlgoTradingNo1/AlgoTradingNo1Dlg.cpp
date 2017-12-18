// AlgoTradingNo1Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AlgoTradingNo1.h"
#include "AlgoTradingNo1Dlg.h"
#include "IniFile.h"
#include "Tools_Win32.h"
#include "CommonDef.h"
#include "PlusinAPI.h"
#include "easytime.h"
#include "easytradingtimespan.hpp"
#include <math.h>
#include <string>
#include <sstream>
using namespace std;


#ifdef _DEBUG
	#pragma comment(lib,"PlusinAPID.lib")
#else
	#pragma comment(lib,"PlusinAPI.lib")
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAlgoTradingNo1Dlg �Ի���

CAlgoTradingNo1Dlg* CAlgoTradingNo1Dlg::m_pDlg=NULL;


CAlgoTradingNo1Dlg::CAlgoTradingNo1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlgoTradingNo1Dlg::IDD, pParent)
{
	m_pDlg=this;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bTrading=false;
	m_bLogon=false;
}
CAlgoTradingNo1Dlg::~CAlgoTradingNo1Dlg()
{
	m_pDlg=NULL;
}


void CAlgoTradingNo1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab_result);
	DDX_Control(pDX, IDC_EDIT1, m_InstrumentName);
	DDX_Control(pDX, IDC_EDIT2, m_S0);
	DDX_Control(pDX, IDC_EDIT3, m_X0);
	DDX_Control(pDX, IDC_COMBO1, m_Direction);
	DDX_Control(pDX, IDC_COMBO2, m_Offset);
	DDX_Control(pDX, IDC_EDIT4, m_T);
	DDX_Control(pDX, IDC_EDIT5, m_N);
	DDX_Control(pDX, IDC_EDIT7, m_Sigma);
	DDX_Control(pDX, IDC_EDIT8, m_Alpha);
	DDX_Control(pDX, IDC_EDIT9, m_lambda);
	DDX_Control(pDX, IDC_EDIT10, m_Epsilon);
	DDX_Control(pDX, IDC_EDIT11, m_Eta);
	DDX_Control(pDX, IDC_EDIT12, m_Gamma);
	DDX_Control(pDX, IDC_EDIT6, m_tau);
	DDX_Control(pDX, IDC_EDIT14, m_xbar);
	DDX_Control(pDX, IDC_EDIT13, m_EtaBar);
	DDX_Control(pDX, IDC_EDIT15, m_EX);
	DDX_Control(pDX, IDC_EDIT16, m_VX);
	DDX_Control(pDX, IDC_EDIT17, m_kT);
	DDX_Control(pDX, IDC_EDIT18, m_ktau);
	DDX_Control(pDX, IDC_EDIT19, m_k2);
	DDX_Control(pDX, IDC_EDIT20, m_k);
	DDX_Control(pDX, IDC_COMBO3, m_TradeTrend);
	DDX_Control(pDX, IDC_BUTTON3, m_btnCalculate);
	DDX_Control(pDX, IDC_BUTTON1, m_btnTrade);
	DDX_Control(pDX, IDC_BUTTON2, m_btnCancelTrading);
}

BEGIN_MESSAGE_MAP(CAlgoTradingNo1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, &CAlgoTradingNo1Dlg::OnTcnSelchangingTab1)
	ON_WM_SIZE()
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON3, &CAlgoTradingNo1Dlg::OnBnClickedButton3)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT1, &CAlgoTradingNo1Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CAlgoTradingNo1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAlgoTradingNo1Dlg::OnBnClickedButton2)
	ON_WM_TIMER()
//	ON_EN_UPDATE(IDC_EDIT1, &CAlgoTradingNo1Dlg::OnEnUpdateEdit1)
END_MESSAGE_MAP()


// CAlgoTradingNo1Dlg ��Ϣ�������

BOOL CAlgoTradingNo1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��


	if(m_wndStatusBar.Create(WS_CHILD|WS_VISIBLE|CCS_BOTTOM, CRect(0,0,0,0), this, 0))
	{
		int strPartDim[2]={300,-1};
		m_wndStatusBar.SetParts(2,strPartDim);
		m_wndStatusBar.SetText("δ��¼�����ն�",0,0);
	}


	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_brush.CreateSolidBrush(RGB(240,240,240));


	//==========================================================
	//��ʼ��TAB
	TCITEM tc1,tc2;
	tc1.mask = TCIF_TEXT;
	tc1.pszText = "����б�";
	tc2.mask = TCIF_TEXT;
	tc2.pszText = "��״ͼ";

	m_tab_result.InsertItem(0, &tc1);
	m_tab_result.InsertItem(1, &tc2); //������ֻ�����������tabҳ

	//������ҳ��
	m_PageListCtrl.Create(IDD_DIALOG1, GetDlgItem(IDC_TAB1));

	m_PageChart.Create(IDD_DIALOG2, GetDlgItem(IDC_TAB1));

	AdjustChildWindowSize();

	//��ʾ��ҳ��
	m_PageListCtrl.ShowWindow(SW_HIDE);
	m_PageChart.ShowWindow(SW_SHOW);
	m_tab_result.SetCurSel(1);

	//===============================================================

	if(g_hWnd_FT!=NULL)
	{
		PlusinAPI* pAPI=PlusinAPI::GetInstance();
		if(pAPI)
		{
			pAPI->Init("AlgoTradingNo1",
						"AlgoTradingNo1",
						&CAlgoTradingNo1Dlg::PluginsAPICallBack,
						g_hWnd_FT,
						NEED_NO_POS_NO_BIND,
						GetSafeHwnd());
			pAPI->Logon();

			if(m_wndStatusBar.GetSafeHwnd()!=NULL)
				m_wndStatusBar.SetText("���ڵ�¼�����ն�......",0,0);
		}
	}

	m_btnTrade.EnableWindow(FALSE);
	m_btnCancelTrading.EnableWindow(FALSE);


	//��ini��������
	ReadIni();


	//���ö�ʱ�������ڽ���ִ��
	SetTimer(1002,100,NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


//��ȡini�ļ���������Ϣ,���õ��ؼ���ȥ
void CAlgoTradingNo1Dlg::ReadIni()
{
	char PathFileName[512];
	bool bIniExist;

	//��ȡ����·��
	memset(PathFileName,0,sizeof(PathFileName));
	CTools_Win32::GetMainPath(PathFileName,sizeof(PathFileName)-1-strlen(IniFileName_AlgoNo1));
	strcat(PathFileName,IniFileName_AlgoNo1);
	HFILE hF=_lopen(PathFileName,OF_READ);
	if(hF!=HFILE_ERROR)
	{
		bIniExist=true;
		_lclose(hF);
	}
	else bIniExist=false;
	
    CIniFile *pIniFile=NULL;
	pIniFile=new CIniFile(PathFileName);
	if(!pIniFile) return;



	//����������
	//�Ƿ�����ִ�н���
	m_bTrading=pIniFile->GetIniKeyVal("Trading","m_bTrading",0)==0?false:true;
	//��ʼִ�е�ʱ��
	time(&m_StartTradingUTC);
    m_StartTradingUTC=(time_t)pIniFile->GetIniKeyVal("Trading","m_StartTradingUTC",(int)m_StartTradingUTC);
	//��ǰִ�еĽڣ�0 ... N-1
    m_TradingSession=pIniFile->GetIniKeyVal("Trading","m_TradingSession",0);

	if(pIniFile)
	{
		delete pIniFile;
		pIniFile=NULL;
	}

	if(m_algo.LoadFromIni())
	{
		if(bIniExist)
		{
			SetAlgoParamToWnd();

			if(m_algo.N>0&&(int)m_algo.vecnj.size()==m_algo.N&&(int)m_algo.vecnj2.size()==m_algo.N&&m_algo.vecTradedVolume.size()==m_algo.N)
			{
				//����������õ����������
				m_PageListCtrl.setcontent(m_algo);
				m_PageChart.m_ChartCtl.setcontent(m_algo,m_StartTradingUTC);
			}

			if(m_bTrading)
			{
				if(g_hWnd_FT!=NULL)
				{
					m_btnCalculate.EnableWindow(FALSE);
					m_btnTrade.EnableWindow(FALSE);
					m_btnCancelTrading.EnableWindow(TRUE);
				}
				else
				{
					m_btnCalculate.EnableWindow(TRUE);
					m_btnTrade.EnableWindow(FALSE);
					m_btnCancelTrading.EnableWindow(FALSE);
				}
			}
		}
		else if(m_bTrading)
			MessageBox("���������ļ������޷�����ִ�н��ף������½��д���");
	}
	else
	{
		if(m_bTrading)
			MessageBox("���������ļ������޷�����ִ�н��ף������½��д���");
	}
}

//����������Ϣ��ini�ļ�
void CAlgoTradingNo1Dlg::SaveIni(void)
{
	char PathFileName[512];

	//��ȡ����·��
	memset(PathFileName,0,sizeof(PathFileName));
	CTools_Win32::GetMainPath(PathFileName,sizeof(PathFileName)-1-strlen(IniFileName_AlgoNo1));
	strcat(PathFileName,IniFileName_AlgoNo1);
    CIniFile *pIniFile=NULL;
	pIniFile=new CIniFile(PathFileName);
	if(!pIniFile) return;

	//���潻������
	//�Ƿ�����ִ�н���
    pIniFile->UpdateKeyVal("Trading","m_bTrading",(int)m_bTrading);
	//��ʼִ�е�ʱ��
    pIniFile->UpdatedwordKeyVal("Trading","m_StartTradingUTC",(unsigned int)m_StartTradingUTC);
	//��ǰִ�еĽڣ�0 ... N-1
    pIniFile->UpdateKeyVal("Trading","m_TradingSession",m_TradingSession);


	if(pIniFile)
	{
		delete pIniFile;
		pIniFile=NULL;
	}

	if(!m_bTrading)
		GetAlgoParamFromWnd();
	m_algo.SaveIni();

}


void CAlgoTradingNo1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAlgoTradingNo1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
//		CDialog::OnPaint();

		CPaintDC dc(this); // device context for painting
		CRect rc;

		CWnd * pW = this->GetDlgItem(IDC_STATIC_Epsilon);
		pW->GetClientRect(&rc);
		pW->ClientToScreen(&rc);
		this->ScreenToClient(&rc);
		rc.top+=8;

		dc.FillSolidRect(&rc,RGB(255,255,255));
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAlgoTradingNo1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


HBRUSH CAlgoTradingNo1Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor ==CTLCOLOR_DLG)
		return m_brush;
	else if(nCtlColor==CTLCOLOR_STATIC)
	{
		SetBkMode(pDC->m_hDC,TRANSPARENT);

		CRect rc_EpsilonGroupbox;
		CWnd * pW = this->GetDlgItem(IDC_STATIC_Epsilon);
		if(pW)
		{
			pW->GetWindowRect(&rc_EpsilonGroupbox);
			CRect rc_this;
			pWnd->GetWindowRect(&rc_this);
			POINT pt;
			pt.x=rc_this.left;
			pt.y=rc_this.top;
			if(rc_EpsilonGroupbox.PtInRect(pt)) 
				return (HBRUSH)::GetStockObject(WHITE_BRUSH);
		}
		

		return m_brush;
	}
	return hbr;
}


void CAlgoTradingNo1Dlg::OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
// TODO: Add your control notification handler code here
	//ע�⣬��ʱ��CurSel�����ϵ�
    switch (m_tab_result.GetCurSel())
    {
      case 0:
            m_PageListCtrl.ShowWindow(SW_HIDE);
            m_PageChart.ShowWindow(SW_SHOW);
            break;
       case 1:
            m_PageListCtrl.ShowWindow(SW_SHOW);
            m_PageChart.ShowWindow(SW_HIDE);
            break;
      default:
            break;
    }
	*pResult = 0;

}

void CAlgoTradingNo1Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	AdjustChildWindowSize();
}


//��������������м���
void CAlgoTradingNo1Dlg::OnBnClickedButton3()
{
	m_algo.clear();

	GetAlgoParamFromWnd();

	//����Լ����
	if(m_algo.strInstrumentName.empty())
	{
		MessageBox("�����ú�Լ���ƣ�");
		return;
	}

	//����
	string errstr;
	bool brlt=m_algo.calculate(errstr);
	if(!brlt)
	{
		if(!errstr.empty())
			errstr+="��";
		errstr+="���������ã�";
		MessageBox(errstr.c_str());
		return;
	}


	//��������õ�������
	SetAlgoParamToWnd();

	
	//����������õ����������
	m_PageListCtrl.setcontent(m_algo);
	time(&m_StartTradingUTC);
	m_PageChart.m_ChartCtl.setcontent(m_algo,m_StartTradingUTC);

	if(g_hWnd_FT!=NULL)
	{
		m_btnTrade.EnableWindow(TRUE);
		m_btnCancelTrading.EnableWindow(FALSE);
	}
	else
	{
		m_btnTrade.EnableWindow(FALSE);
		m_btnCancelTrading.EnableWindow(FALSE);
	}

}


//ִ�н���
void CAlgoTradingNo1Dlg::OnBnClickedButton1()
{
	//�ӵ�ǰʱ�俪ʼ������Chart��ʱ��
	time(&m_StartTradingUTC);
	m_PageChart.m_ChartCtl.setcontent(m_algo,m_StartTradingUTC);

	//ʹ��ִֹ�еİ�ť����ʹ��
	m_btnCancelTrading.EnableWindow(TRUE);

	//ʹ��ʼִ�кͼ���İ�ť�һ�
	m_btnTrade.EnableWindow(FALSE);
	m_btnCalculate.EnableWindow(FALSE);

	//�������пؼ�
	EnableAllControls(FALSE);

	m_bTrading=true;
	m_TradingSession=-1;

}

//��ֹ����
void CAlgoTradingNo1Dlg::OnBnClickedButton2()
{
	StopTrading();
}

void CAlgoTradingNo1Dlg::OnClose()
{
	int irlt;

	if(!m_bTrading)
	{
		irlt=MessageBox("�Ƿ񱣴浱ǰ���ã�","AlgoTrading",MB_YESNO);
		if(irlt==IDYES)
			SaveIni();
	}
	else
	{
		irlt=MessageBox("����ִ�н��ף�����˳������׽�ֹͣ��(����������������Զ��ָ�����)��\n�Ƿ��˳�(�����浱ǰ����)��",NULL,MB_YESNO);
		if(irlt==IDNO)
			return;
		SaveIni();
	}


	if(g_hWnd_FT!=NULL)
	{
		PlusinAPI* pAPI=PlusinAPI::GetInstance();
		if(pAPI)
			pAPI->Logout();
		m_bLogon=false;
		PlusinAPI::DestroyInstance();
	}

	CDialog::OnClose();
}

void CAlgoTradingNo1Dlg::OnEnChangeEdit1()
{
	if(g_hWnd_FT!=NULL)
	{
		PlusinAPI* pAPI=PlusinAPI::GetInstance();
		if(pAPI)
		{
			InstrumentIDType InstName;
			memset(InstName,0,sizeof(InstName));
			m_InstrumentName.GetWindowTextA(InstName,sizeof(InstName)-1);
			if(string(InstName)!=m_SubscribedInstrumentName)
			{
				//�˶�������
				memset(InstName,0,sizeof(InstName));
				strncpy(InstName,m_SubscribedInstrumentName.c_str(),sizeof(InstName)-1);
//				pAPI->Send(CMDID_QUOTE_UNSUBSCRIBE,InstName,sizeof(InstName));


				memset(InstName,0,sizeof(InstName));
				m_InstrumentName.GetWindowTextA(InstName,sizeof(InstName)-1);
				//���浱ǰ��������ĺ�Լ����
				m_SubscribedInstrumentName=string(InstName);

				//����������
//				pAPI->Send(CMDID_QUOTE_SUBSCRIBE,InstName,sizeof(InstName));
			}
		}
	}
}

void CAlgoTradingNo1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	//�������ִ�н��ף�����ʱ�䣬���м۵�
	if(m_bTrading&&m_bLogon&&!m_Account.empty())
	{
		time_t curtime;
		time(&curtime);

		if(m_PageChart.m_ChartCtl.isovertime(curtime))
		{
			//�Ѿ����ｻ�׽���ʱ����
			StopTrading();
		}
		else
		{
			int pox=m_PageChart.m_ChartCtl.getxPos(curtime);
			if(pox>=0&&pox<m_PageChart.m_ChartCtl.getTimeWndWid())
			{
				//����Ч�ؽ���ʱ��
				int nSession=pox*m_algo.N/m_PageChart.m_ChartCtl.getTimeWndWid();
				if(nSession>=0&&nSession<m_algo.N&&nSession>m_TradingSession)
				{
					//�����µ�һ�ڽ���
					m_TradingSession=nSession;
					SendInsertOrder();
				}
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}


int CAlgoTradingNo1Dlg::PluginsAPICallBack(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId)
{
	switch(nCmdId)
	{
		case CMDID_CONSOLE_LOGON_RSP:
		{
			//��¼����
			if(nErrorCode==SUCCESS&&m_pDlg!=NULL)
			{
				m_pDlg->m_bLogon=true;

				if(m_pDlg->m_wndStatusBar.GetSafeHwnd()!=NULL)
					m_pDlg->m_wndStatusBar.SetText("�Ѿ���¼�����նˣ����ڽ��г�ʼ����ѯ......",0,0);

				//��ѯ�˺�
				PlusinAPI* pAPI=PlusinAPI::GetInstance();
				if(pAPI)
				{
					pAPI->Send(CMDID_ACCOUNT_CURRENT_ACCOUNT_QUERY,NULL,0);
				}
			}
			else 
			{
				if(m_pDlg->m_wndStatusBar.GetSafeHwnd()!=NULL)
					m_pDlg->m_wndStatusBar.SetText("��¼�����ն�ʧ��",0,0);
			}
			break;
		}
		case CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP:
		{
			//��ѯ�˺ŷ���
			if(nErrorCode==SUCCESS&&nDataLen==(int)sizeof(CommonAccountIDType)&&m_pDlg!=NULL)
			{
				CommonAccountIDType& pAccount=*(CommonAccountIDType*)pData;
				if(pAccount[sizeof(CommonAccountIDType)-1]!=0)
					pAccount[sizeof(CommonAccountIDType)-1]=0;
				m_pDlg->m_Account=string(pAccount);

				if(m_pDlg->m_wndStatusBar.GetSafeHwnd()!=NULL)
				{
					char buf[256];
					memset(buf,0,sizeof(buf));
					sprintf_s(buf,sizeof(buf)-1,"��¼�����ն˳ɹ��������˺�Ϊ:%s",pAccount);
					m_pDlg->m_wndStatusBar.SetText(buf,0,0);
				}
			}
			break;
		}
		case CMDID_QUOTE_SUBSCRIBE_RSP:
		{
			//��������Ľ������
			if(nErrorCode==SUCCESS&&nDataLen==sizeof(InstrumentIDType)&&m_pDlg!=NULL)
			{
				InstrumentIDType InstName;
				memcpy(InstName,pData,nDataLen);
				InstName[sizeof(InstName)-1]=0;
			}
			break;
		}
		case CMDID_QUOTE_SEND_QUOTE:
		{
			//���յ���������
			if(nErrorCode==SUCCESS&&nDataLen==sizeof(PlatformStru_DepthMarketData)&&m_pDlg!=NULL)
			{
				PlatformStru_DepthMarketData* pMarketData=(PlatformStru_DepthMarketData*)pData;

				InstrumentIDType InstName;
				memset(InstName,0,sizeof(InstName));
				m_pDlg->m_InstrumentName.GetWindowTextA(InstName,sizeof(InstName)-1);

				if(pMarketData->InstrumentID[sizeof(pMarketData->InstrumentID)-1]!=0)
					pMarketData->InstrumentID[sizeof(pMarketData->InstrumentID)-1]=0;

				if(strcmp(InstName,pMarketData->InstrumentID)==0)
				{
					CTools_AlgoTrading::SetDoubleToCEdit(m_pDlg->m_S0,pMarketData->LastPrice);
				}
			}
			break;
		}
		case CMDID_ORDER_TRADE_RTN:
		{
			//�ɽ��ر�
			if(nDataLen==sizeof(PlatformStru_TradeInfo)&&m_pDlg!=NULL)
			{
				m_pDlg->TradeInfoReach(*(PlatformStru_TradeInfo*)pData);
			}
			break;
		}
		case CMDID_ORDER_ORDER_STATUS_CHANGE:
		{
			//����״̬�ر�
			if(nDataLen==sizeof(PlatformStru_OrderInfo)&&m_pDlg!=NULL)
			{
				m_pDlg->OnOrderRtn(*(PlatformStru_OrderInfo*)pData);
			}
			break;
		}
	}
	return 0;
}

//ʹ�ܻ�������пؼ�
void CAlgoTradingNo1Dlg::EnableAllControls(BOOL bEnable)
{
	m_InstrumentName.EnableWindow(bEnable);
	m_S0.EnableWindow(bEnable);
	m_X0.EnableWindow(bEnable);
	m_Direction.EnableWindow(bEnable);
	m_Offset.EnableWindow(bEnable);
	m_TradeTrend.EnableWindow(bEnable);
	m_T.EnableWindow(bEnable);
	m_N.EnableWindow(bEnable);
	m_Sigma.EnableWindow(bEnable);
	m_Alpha.EnableWindow(bEnable);
	m_lambda.EnableWindow(bEnable);
	m_Epsilon.EnableWindow(bEnable);
	m_Eta.EnableWindow(bEnable);
	m_Gamma.EnableWindow(bEnable);
}

//����һ���µ�,����m_algo��m_TradingSession��VolumeΪ-1ʱ���Զ������µ���
void CAlgoTradingNo1Dlg::SendInsertOrder(int Volume)
{
	if(!m_bTrading)
		return;
	if(m_TradingSession<0||m_TradingSession>=m_algo.N)
		return;
	if((int)m_algo.vecnj2.size()!=m_algo.N||(int)m_algo.vecnj.size()!=m_algo.N)
		return;

	if(Volume==-1)
	{
		if(m_algo.bTradeTrend) Volume = (int)m_algo.vecnj2[m_TradingSession];
		else Volume = (int)m_algo.vecnj[m_TradingSession];
		if(Volume<0) Volume*=-1;
		Volume-=m_algo.vecTradedVolume[m_TradingSession];
	}
	if(Volume<=0) return;

	PlatformStru_InputOrder OrderInsert;
	memset(&OrderInsert,0,sizeof(OrderInsert));

	strncpy_s(OrderInsert.InvestorID,sizeof(OrderInsert.InvestorID),m_Account.c_str(),sizeof(OrderInsert.InvestorID)-1);
	strncpy_s(OrderInsert.InstrumentID,sizeof(OrderInsert.InstrumentID),m_algo.strInstrumentName.c_str(),sizeof(OrderInsert.InstrumentID)-1);
	OrderInsert.OrderPriceType='2';								//�м� THOST_FTDC_OPT_AnyPrice='1' �޼� THOST_FTDC_OPT_LimitPrice='2';
	OrderInsert.Direction=m_algo.direction==0?'0':'1';			//��������0-'0'=��1-'1'=��
	if(m_algo.OffsetID==0) OrderInsert.CombOffsetFlag[0]='0';	//��ƽ��־��0-'0'=����
	else if(m_algo.OffsetID==1) OrderInsert.CombOffsetFlag[0]='1';//��ƽ��־��1-'1'=ƽ��
	else OrderInsert.CombOffsetFlag[0]='3';						//��ƽ��־��2-'3'=ƽ��
	OrderInsert.CombHedgeFlag[0]='1';							//Ͷ���ױ���'1'=Ͷ����'3'=�ױ�
	OrderInsert.LimitPrice=m_algo.S;							//�۸����м۵������Լ۸�
	OrderInsert.VolumeTotalOriginal=Volume;						//����
	OrderInsert.TimeCondition='3';								//��Ч�����ͣ��м۵���Ӧ��������ɣ�������='1'; �޼۵���Ӧ��������Ч='3'
	OrderInsert.VolumeCondition='1';							//�ɽ������ͣ�'1'��ʾ�κ�����
	OrderInsert.MinVolume=1;
	OrderInsert.ContingentCondition='1';						//��������
	OrderInsert.ForceCloseReason='0';							//ǿƽԭ��'0'��ǿƽ
	OrderInsert.OrderClass='N';									//�±���
	strncpy_s(OrderInsert.strAccount,sizeof(OrderInsert.strAccount),m_Account.c_str(),sizeof(OrderInsert.strAccount)-1);

	PlusinAPI* pAPI=PlusinAPI::GetInstance();
	if(pAPI) 
		pAPI->Send(CMDID_ORDER_INSERT_ORDER,&OrderInsert,sizeof(OrderInsert));
}

//����ɽ��ر�������ֻ���򵥴���ֻҪ��Լ����/����/��ƽ/Ͷ��һ�£����ۼ��ѳɽ�������
void CAlgoTradingNo1Dlg::TradeInfoReach(PlatformStru_TradeInfo& TradeInfo)
{
	if(!m_bTrading)
		return;
	if(m_TradingSession<0||m_TradingSession>=m_algo.N)
		return;
	if((int)m_algo.vecTradedVolume.size()!=m_algo.N)
		return;
	if(TradeInfo.InstrumentID[sizeof(TradeInfo.InstrumentID)-1]!=0)
		TradeInfo.InstrumentID[sizeof(TradeInfo.InstrumentID)-1]=0;
	if(string(TradeInfo.InstrumentID)!=m_algo.strInstrumentName)
		return;
	char Direction=m_algo.direction==0?'0':'1';
	if(TradeInfo.Direction!=Direction)
		return;
	if(m_algo.OffsetID==0&&TradeInfo.OffsetFlag!='0'||
		m_algo.OffsetID!=0&&TradeInfo.OffsetFlag=='0') 
		return;
	char HedgeFlag='1';
	if(TradeInfo.HedgeFlag!=HedgeFlag)
		return;

	m_algo.vecTradedVolume[m_TradingSession]+=TradeInfo.Volume;

	m_PageListCtrl.RefreshTradedVolume(m_TradingSession,m_algo.vecTradedVolume[m_TradingSession]);
	m_PageChart.m_ChartCtl.RefreshTradedVolume(m_TradingSession,m_algo.vecTradedVolume[m_TradingSession]);
}

//�������ر���
void CAlgoTradingNo1Dlg::OnOrderRtn(PlatformStru_OrderInfo& OrderInfo)
{
	if(!m_bTrading)
		return;
	if(m_TradingSession<0||m_TradingSession>=m_algo.N)
		return;
	if((int)m_algo.vecTradedVolume.size()!=m_algo.N)
		return;
	if(OrderInfo.InstrumentID[sizeof(OrderInfo.InstrumentID)-1]!=0)
		OrderInfo.InstrumentID[sizeof(OrderInfo.InstrumentID)-1]=0;
	if(string(OrderInfo.InstrumentID)!=m_algo.strInstrumentName)
		return;
	char Direction=m_algo.direction==0?'0':'1';
	if(OrderInfo.Direction!=Direction)
		return;
	if(m_algo.OffsetID==0&&OrderInfo.CombOffsetFlag[0]!='0'||
		m_algo.OffsetID!=0&&OrderInfo.CombOffsetFlag[0]=='0') 
		return;
	char HedgeFlag='1';
	if(OrderInfo.CombHedgeFlag[0]!=HedgeFlag)
		return;

	//���״̬Ϊ�Ѿ���������ʣ�����������µ�
	if(OrderInfo.OrderStatus=='5'&&
		OrderInfo.VolumeTotal>0)
	{
		SendInsertOrder(OrderInfo.VolumeTotal);
	}
}

//ֹͣ����
void CAlgoTradingNo1Dlg::StopTrading()
{
	//ʹ��ʼִ�к���ִֹ�еİ�ť�һ�
	m_btnTrade.EnableWindow(FALSE);
	m_btnCancelTrading.EnableWindow(FALSE);

	//ʹ����İ�ť����ʹ��
	m_btnCalculate.EnableWindow(TRUE);

	//ʹ�����пؼ�
	EnableAllControls(TRUE);

	m_bTrading=false;
}


//�����Ӵ��ڴ�С
void CAlgoTradingNo1Dlg::AdjustChildWindowSize(void)
{
	RECT ClientRect;
	GetClientRect(&ClientRect);
	int cx=ClientRect.right-ClientRect.left;
	int cy=ClientRect.bottom-ClientRect.top;

	int StatusHei=0;
	if(m_wndStatusBar.GetSafeHwnd()!=NULL)
	{
		RECT Rect;
		m_wndStatusBar.GetWindowRect(&Rect);
		int wid=cx;
		int hei=Rect.bottom-Rect.top;
		StatusHei=hei;
		m_wndStatusBar.MoveWindow(0,cy-hei,wid,hei);
	}
	if(m_tab_result.GetSafeHwnd()!=NULL)
	{
		RECT Rect;
		m_tab_result.GetWindowRect(&Rect);
		ScreenToClient(&Rect);
		int wid=cx-Rect.left*2;
		int hei=cy-StatusHei-8-Rect.top;
		m_tab_result.MoveWindow(Rect.left,Rect.top,wid,hei);

		//���TAB�ؼ�������
		CRect rect=m_tab_result.GetPageRect();
		//����ҳ���ƶ���ָ����λ��
		if(m_PageListCtrl.GetSafeHwnd())
			m_PageListCtrl.MoveWindow(&rect);
		if(m_PageChart.GetSafeHwnd())
			m_PageChart.MoveWindow(&rect);
	}
}

//���ؼ��е�ֵ�ŵ�m_algo��ȥ
void CAlgoTradingNo1Dlg::GetAlgoParamFromWnd(void)
{
	//ȡ��Լ����
	m_algo.strInstrumentName=CTools_AlgoTrading::GetStringFromCEdit(m_InstrumentName);
	//��ƽ����
	if(CTools_AlgoTrading::GetStringFromCComboBox(m_Offset)=="����") m_algo.OffsetID=0;
	else if(CTools_AlgoTrading::GetStringFromCComboBox(m_Offset)=="ƽ��") m_algo.OffsetID=1;
	else m_algo.OffsetID=2;	//ƽ��
	//������������
	m_algo.bTradeTrend = CTools_AlgoTrading::GetStringFromCComboBox(m_TradeTrend)=="������"?true:false;
	//���׷�������
	m_algo.direction = CTools_AlgoTrading::GetStringFromCComboBox(m_Direction)=="��"?0:1;
	
	//�۸�
	m_algo.S=(int)CTools_AlgoTrading::GetDoubleFromCEdit(m_S0);
	//����
	m_algo.X=(int)CTools_AlgoTrading::GetDoubleFromCEdit(m_X0);
	//ʱ��
	m_algo.T=CTools_AlgoTrading::GetDoubleFromCEdit(m_T);
	//�ֶ���
	m_algo.N=(int)CTools_AlgoTrading::GetDoubleFromCEdit(m_N);
	//����ƫ�æ�
	m_algo.lambda=CTools_AlgoTrading::GetDoubleFromCEdit(m_lambda);
	//�ղ����ʦ�
	m_algo.sigma=CTools_AlgoTrading::GetDoubleFromCEdit(m_Sigma);
	//�������
	m_algo.alpha=CTools_AlgoTrading::GetDoubleFromCEdit(m_Alpha);
	//���ڳ���ɱ�������
	m_algo.eta=CTools_AlgoTrading::GetDoubleFromCEdit(m_Eta);
	//���ڳ���ɱ�������
	m_algo.epsilon=CTools_AlgoTrading::GetDoubleFromCEdit(m_Epsilon);
	//���ڳ���ɱ�������
	m_algo.gamma=CTools_AlgoTrading::GetDoubleFromCEdit(m_Gamma);

	//���ݷ�������X0�ͦǵķ���
	if(m_algo.direction==1)
	{
		if(m_algo.X<0) m_algo.X*=-1;
		if(m_algo.eta<0) m_algo.eta*=-1;
	}
	else
	{
		if(m_algo.X>0) m_algo.X*=-1;
		if(m_algo.eta>0) m_algo.eta*=-1;
	}
}

//��m_algo�е�ֵ���õ��ؼ���ȥ
void CAlgoTradingNo1Dlg::SetAlgoParamToWnd(void)
{
	CTools_AlgoTrading::SetStringToCEdit(m_InstrumentName,m_algo.strInstrumentName.c_str());
	m_Direction.SetCurSel(m_algo.direction==0?0:1);
	if(m_algo.OffsetID==0) m_Offset.SetCurSel(0);
	else if(m_algo.OffsetID==1) m_Offset.SetCurSel(1);
	else m_Offset.SetCurSel(2);
	m_TradeTrend.SetCurSel(m_algo.bTradeTrend?1:0);

	CTools_AlgoTrading::SetDoubleToCEdit(m_S0,m_algo.S);
	CTools_AlgoTrading::SetIntToCEdit(m_X0,m_algo.X);
	CTools_AlgoTrading::SetDoubleToCEdit(m_T,m_algo.T);
	CTools_AlgoTrading::SetIntToCEdit(m_N,m_algo.N);
	CTools_AlgoTrading::SetDoubleToCEdit(m_lambda,m_algo.lambda);
	CTools_AlgoTrading::SetDoubleToCEdit(m_Sigma,m_algo.sigma);
	CTools_AlgoTrading::SetDoubleToCEdit(m_Alpha,m_algo.alpha);
	CTools_AlgoTrading::SetDoubleToCEdit(m_Eta,m_algo.eta);
	CTools_AlgoTrading::SetDoubleToCEdit(m_Epsilon,m_algo.epsilon);
	CTools_AlgoTrading::SetDoubleToCEdit(m_Gamma,m_algo.gamma);

	CTools_AlgoTrading::SetDoubleToCEdit(m_tau,m_algo.tau);
	CTools_AlgoTrading::SetDoubleToCEdit(m_xbar,m_algo.xbar);
	CTools_AlgoTrading::SetDoubleToCEdit(m_EtaBar,m_algo.etabar);
	CTools_AlgoTrading::SetDoubleToCEdit(m_k2,m_algo.ksquare);
	CTools_AlgoTrading::SetDoubleToCEdit(m_k,m_algo.k);
	CTools_AlgoTrading::SetDoubleToCEdit(m_kT,m_algo.k*m_algo.T);
	CTools_AlgoTrading::SetDoubleToCEdit(m_ktau,m_algo.k*m_algo.tau);

	//���ݺ�Լ���ƣ�����ÿ�ս���ʱ��
    CEasyTimespan<int> 		Timespan_IntraDay;
	//vector<Stru_TimeLineWin_Period> vecperiodEachDay;

	//��ʽ����ϵͳ
	if(m_algo.strInstrumentName.size()==6&&
		isupper(m_algo.strInstrumentName.c_str()[0])&&
		isupper(m_algo.strInstrumentName.c_str()[1])&&
		isdigit(m_algo.strInstrumentName.c_str()[2]))
	{
		//�н�����Լ 9:15-11:30 13:00-15:15, total 4.5 hours (2h15'+2h15')
        Timespan_IntraDay.insert(9*3600+15*60,11*3600+30*60);
        Timespan_IntraDay.insert(13*3600,15*3600+15*60);
//		vecperiodEachDay.push_back(Stru_TimeLineWin_Period(9*3600+15*60,11*3600+30*60));
//		vecperiodEachDay.push_back(Stru_TimeLineWin_Period(13*3600,15*3600+15*60));
	}
	else 
	{
		//��Ʒ�ڻ� 9:00-10:15 10:30-11:30 13:00-15:00, total 4h15'(1h15'+1h+2h)
        Timespan_IntraDay.insert(9*3600,10*3600+15*60);
        Timespan_IntraDay.insert(10*3600+30*60,11*3600+30*60);
        Timespan_IntraDay.insert(13*3600+30*60,15*3600);
//		vecperiodEachDay.push_back(Stru_TimeLineWin_Period(9*3600,10*3600+15*60));
//		vecperiodEachDay.push_back(Stru_TimeLineWin_Period(10*3600+30*60,11*3600+30*60));
//		vecperiodEachDay.push_back(Stru_TimeLineWin_Period(13*3600+30*60,15*3600));
	}

	if(1)
	{
		//ģ�⽻��ϵͳ 15:15 - 18:00��������
        Timespan_IntraDay.insert(15*3600+15*60,18*3600);
//		vecperiodEachDay.push_back(Stru_TimeLineWin_Period(15*3600+15*60,18*3600));
	}

//	m_PageChart.m_ChartCtl.m_TimeLineWin.setvecPeriodEachDay(vecperiodEachDay);
    CEasyTradingTimespan TradingTimespans(true,
                                            Timespan_IntraDay,
                                            CEasyTimespan<time_t>(),
                                            true,
											Stru_TimezoneDst());
	m_PageChart.m_ChartCtl.setTradingTimespan(TradingTimespans);
	m_PageChart.m_ChartCtl.refreshTimeLineWin();
}



