// TestorProxyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestorProxy.h"
#include "TestorProxyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
enum eOrderType
{
	OrderType_OrderInput,
	OrderType_OrderAction,
	OrderType_Random,
};
enum eOpenCloseType
{
	OpenCloseType_Open,
	OpenCloseType_Close,
	OpenCloseType_Random,
};

struct sTrader
{
	char msName[30];
	char msPass[30];
};

struct sOrderAction
{
	char mUserName[30];
	int  mnIndex;
	int  mnSocket;
	eOrderType mnOrder; 
	char mProduct[30];
	eOpenCloseType  mnOpenClose;
};

//��½����
#define CMDID_REINIT					1				//���³�ʼ��
#define CMDID_LOGIN					    2				//��½
#define CMDID_LOGOFF			        3				//ע��
#define CMDID_ORDER			            4				//�µ�
//#include "..\Client\Misc\ConfigMgr.h"
//#include "..\Client\Misc\TcpLayer.h"
#include "TCPDataService.h"
#ifdef _DEBUG
#pragma comment(lib, "Toolsd.lib")
#else
#pragma comment(lib, "Tools.lib")
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


// CTestorProxyDlg �Ի���




CTestorProxyDlg::CTestorProxyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestorProxyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestorProxyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRIDSTATUS, m_gridStatus);             // associate the grid window with a C++ object
}

BEGIN_MESSAGE_MAP(CTestorProxyDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &CTestorProxyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTestorProxyDlg::OnBnClickedCancel)
	ON_MESSAGE(RECV_DATA_MSG, &CTestorProxyDlg::OnRecvMessage)
END_MESSAGE_MAP()


LRESULT CTestorProxyDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	switch(message) {
	case WM_COPYDATA:
		{
			COPYDATASTRUCT* pCpyData=(COPYDATASTRUCT*)lParam;	
			if(wParam==CONSOLE_MAGIC_NUMBER && pCpyData!=NULL) {
				if(pCpyData->dwData==CONSOLE_RSP_HWND
						&&pCpyData->cbData==sizeof(RSP_FASTTRADERHWND)) {
					RSP_FASTTRADERHWND* pRsp = (RSP_FASTTRADERHWND*)pCpyData->lpData;
					m_fastTraderManager.ReseHwnd(pRsp->dwProcID, pRsp->hWnd);
				}
				else if(pCpyData->dwData==CONSOLE_RSP_DOORDER
						&&pCpyData->cbData==sizeof(RSP_DOORDER)) {
					RSP_DOORDER* pRsp = (RSP_DOORDER*)pCpyData->lpData;
					map<int, ITEMNUM>::iterator it;
					if((it = m_mapAccountSum.find(pRsp->nSeqNum)) != m_mapAccountSum.end()) {
						switch(pRsp->nResult) {
						case 0:
							it->second.nMsgRecvNum++;
							UpdateGrid_MsgRecvNum(pRsp->nSeqNum, it->second.nMsgRecvNum);
							break;
						case 1:
							it->second.nInsertNum++;
							UpdateGrid_InsertNum(pRsp->nSeqNum, it->second.nInsertNum);
							break;
						case 2:
							it->second.nRejectNum++;
							UpdateGrid_RejectNum(pRsp->nSeqNum, it->second.nRejectNum);
							break;

						}
					}
				}
			}
		}
		break;
	};

	return CDialog::WindowProc(message, wParam, lParam);
}

// CTestorProxyDlg ��Ϣ�������

LRESULT CTestorProxyDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == CMDID_REINIT )
	{		
		//MessageBox("Reinit", "��ʾ", MB_ICONERROR);
		m_fastTraderManager.Clear();
	}
	else if ( pRecvData->head.cmdid == CMDID_LOGIN )
	{
	
		//if ( pRecvData->head.userdata1 == 0 )
		{
			DWORD dwProcID = 0;
			int nFolderNum = -1;
			sTrader* lp = (sTrader*)pRecvData->pData	;
			char lpBuf[512] = {0};

			sprintf(lpBuf,"��½�û� %s, Socket Is %d,index is %d",lp,pRecvData->head.userdata1,pRecvData->head.userdata2);
			//MessageBox(lpBuf, "��ʾ", MB_ICONERROR);
			if(m_fastTraderManager.RegisterFastTrader(pRecvData->head.userdata2, GetSafeHwnd(), 
					CString(lp->msName), CString(lp->msPass), dwProcID, nFolderNum)) {
				ITEMNUM item = {0};
				item.nFolderNum = nFolderNum;
				m_mapAccountSum.insert(make_pair(pRecvData->head.userdata2, item));
				UpdateGrid_ID(nFolderNum, pRecvData->head.userdata2, CString(lp->msName));
			}
		}
	
	}
	else if ( pRecvData->head.cmdid == CMDID_LOGOFF )
	{
		//if ( pRecvData->head.userdata1 == 0 )
		{
			char * lp = (char*)pRecvData->pData	;
			char lpBuf[512] = {0};
			sprintf(lpBuf,"ע���û� %s, Socket Is %d,index is %d",lp,pRecvData->head.userdata1,pRecvData->head.userdata2);
			//MessageBox(lpBuf, "��ʾ", MB_ICONERROR);
			if(m_fastTraderManager.UnregisterFastTrader(pRecvData->head.userdata2)) {
				map<int, ITEMNUM>::iterator it = m_mapAccountSum.find(pRecvData->head.userdata2);
				if(it != m_mapAccountSum.end()) {
					UpdateGrid_ID(it->second.nFolderNum, -1, CString(""));
					m_mapAccountSum.erase(it);
				}
			}
		}
	
	}
	else if ( pRecvData->head.cmdid == CMDID_ORDER )
	{
		if ( pRecvData->head.userdata1 == 0 )
		{
			sOrderAction * lp = (sOrderAction*)pRecvData->pData;
			
			if(lp != NULL) {

				BOOL bRet = FALSE;
				CMD_INPUTSIMPLE input = {0};

				input.nSeqNum = lp->mnIndex;

				//strncpy(input.InstrumentID, lp->mProduct, sizeof(input.InstrumentID)-1);

				if(lp->mnOrder==OrderType_Random)
					input.nInputType = 0;
				else
					input.nInputType = lp->mnOrder+1;
				if(lp->mnOpenClose==OpenCloseType_Random)
					input.OpenClose = 0;
				else
					input.OpenClose = lp->mnOpenClose+'0';
				input.nQty = 1;
				input.nMaxQty = -1;
				input.OrderPrice = '0';
				bRet = m_fastTraderManager.DoCommand(lp->mnIndex, input);

				map<int, ITEMNUM>::iterator it;
				if((it = m_mapAccountSum.find(lp->mnIndex)) != m_mapAccountSum.end()) {
					it->second.nRecvNum++;
					UpdateGrid_RecvNum(lp->mnIndex, it->second.nRecvNum);
					if(bRet) {
						it->second.nSendNum++;
						UpdateGrid_SendNum(lp->mnIndex, it->second.nSendNum);
					}
				}
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			//MessageBox(csMsg, "��ʾ", MB_ICONERROR);
		}
	}
	return 0;
}
BOOL CTestorProxyDlg::OnInitDialog()
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


	m_gridStatus.SetDefCellHeight(20);
	m_gridStatus.SetDefCellMargin(3);
	m_gridStatus.SetEditable(FALSE);
	m_gridStatus.EnableSelection(TRUE);
	m_gridStatus.SetFixedColumnSelection(FALSE);
	m_gridStatus.EnableDragAndDrop(FALSE);
	m_gridStatus.SetColumnResize(TRUE);
	m_gridStatus.SetHeaderSort(FALSE);
	m_gridStatus.SetTrackFocusCell(TRUE);
	m_gridStatus.SetBkColor(RGB(255, 255, 255));
	
	m_gridStatus.SetColumnCount(7);
	m_gridStatus.SetRowCount(CFastTraderManager::conMaxFolderNum+1);
	m_gridStatus.SetFixedRowCount(1);
	m_gridStatus.SetItemText(0, conColUserSeqNum, "�ʺ����");
	m_gridStatus.SetItemText(0, conColUserName, "�ʺ�ID");
	m_gridStatus.SetItemText(0, conColRecvNum, "����tcp");
	m_gridStatus.SetItemText(0, conColSendNum, "ת��tcp");
	m_gridStatus.SetItemText(0, conColMsgRecvNum, "Msg�յ�");
	m_gridStatus.SetItemText(0, conColInsertNum, "�·�api");
	m_gridStatus.SetItemText(0, conColRejectNum, "�ܾ���");
	m_gridStatus.ExpandColumnsToFit();
	m_gridStatus.Invalidate();

	char szLocalPath[256];
	memset(szLocalPath, 0, 256);
	GetModuleFileName( NULL, szLocalPath, sizeof(szLocalPath)-1 );
	*(strrchr(szLocalPath,'\\') + 1) = 0;
	strcat(szLocalPath,"TestProxy.ini");


	char strBuf[100] = {0};
	GetPrivateProfileStringA("TCP","SvrIP","",strBuf,sizeof(strBuf),szLocalPath);
	int	nPort = GetPrivateProfileIntA("TCP","Port",8000,szLocalPath);
	CTCPDataService::GetInstance()->Connect(strBuf, nPort);
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTestorProxyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestorProxyDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void CTestorProxyDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	m_fastTraderManager.Clear();

	CDialog::OnClose();
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�


void CTestorProxyDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTestorProxyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestorProxyDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CTestorProxyDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

int CTestorProxyDlg::FindRow(int nSeqNum)
{
	for(int i=1; i<m_gridStatus.GetRowCount(); i++) {
		if((int)m_gridStatus.GetItemData(i, 0)==nSeqNum)
			return i;
	}
	return -1;
}

void CTestorProxyDlg::UpdateGrid_ID(int nRow, int nSeqNum, CString& strAccount)
{
	if(nRow<1 || nRow>m_gridStatus.GetRowCount())
		return;

	CString strText;

	m_gridStatus.SetItemData(nRow, 0, (LPARAM)nSeqNum);
	
	strText.Format("%d", nSeqNum);
	m_gridStatus.SetItemText(nRow, conColUserSeqNum, strText);
	m_gridStatus.SetItemText(nRow, conColUserName, strAccount);
	m_gridStatus.Invalidate();
}

void CTestorProxyDlg::UpdateGrid_RecvNum(int nSeqNum, int nNum)
{
	int nRow = -1;
	if((nRow=FindRow(nSeqNum))<1)
		return;

	CString strText;
	
	strText.Format("%d", nNum);
	m_gridStatus.SetItemText(nRow, conColRecvNum, strText);
	m_gridStatus.Invalidate();
}

void CTestorProxyDlg::UpdateGrid_SendNum(int nSeqNum, int nNum)
{
	int nRow = -1;
	if((nRow=FindRow(nSeqNum))<1)
		return;

	CString strText;
	
	strText.Format("%d", nNum);
	m_gridStatus.SetItemText(nRow, conColSendNum, strText);
	m_gridStatus.Invalidate();
}
void CTestorProxyDlg::UpdateGrid_MsgRecvNum(int nSeqNum, int nNum)
{
	int nRow = -1;
	if((nRow=FindRow(nSeqNum))<1)
		return;

	CString strText;
	
	strText.Format("%d", nNum);
	m_gridStatus.SetItemText(nRow, conColMsgRecvNum, strText);
	m_gridStatus.Invalidate();
}

void CTestorProxyDlg::UpdateGrid_InsertNum(int nSeqNum, int nNum)
{
	int nRow = -1;
	if((nRow=FindRow(nSeqNum))<1)
		return;

	CString strText;
	
	strText.Format("%d", nNum);
	m_gridStatus.SetItemText(nRow, conColInsertNum, strText);
	m_gridStatus.Invalidate();
}

void CTestorProxyDlg::UpdateGrid_RejectNum(int nSeqNum, int nNum)
{
	int nRow = -1;
	if((nRow=FindRow(nSeqNum))<1)
		return;

	CString strText;
	
	strText.Format("%d", nNum);
	m_gridStatus.SetItemText(nRow, conColRejectNum, strText);
	m_gridStatus.Invalidate();
}
