// SetupDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Setup.h"
#include "SetupDlg.h"
#include <stdio.h>
#include "ConfigManager.h"

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


// CSetupDlg �Ի���




CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDlg::IDD, pParent)
	, m_strSID(_T(""))
	, m_strSysAccount(_T(""))
	, m_strSysPwd(_T(""))
	, m_strMAAccount(_T(""))
	, m_strMAPwd(_T(""))
	, m_bStep2ControlStatus(FALSE)
	, m_bStep3ControlStatus(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SID, m_strSID);
	DDX_Text(pDX, IDC_EDIT_SYS_ACCOUNT, m_strSysAccount);
	DDX_Text(pDX, IDC_EDIT_SYS_PWD, m_strSysPwd);
	DDX_Text(pDX, IDC_EDIT_MA_ACCOUNT, m_strMAAccount);
	DDX_Text(pDX, IDC_EDIT_MA_PWD, m_strMAPwd);
}

BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CSetupDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_STEP2, &CSetupDlg::OnBnClickedButtonStep2)
	ON_BN_CLICKED(IDC_BUTTON_STEP3, &CSetupDlg::OnBnClickedButtonStep3)
	ON_BN_CLICKED(IDC_CHECK1, &CSetupDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CSetupDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetupDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSetupDlg ��Ϣ�������

BOOL CSetupDlg::OnInitDialog()
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_strSID = _T("orcl");
	m_strMAAccount = _T("maaccount");
	m_strMAPwd = _T("maaccount");
	UpdateData(FALSE);

	char szBuff[MAX_PATH];
	memset(szBuff, 0, sizeof(szBuff));
	::GetModuleFileName(NULL, szBuff, sizeof(szBuff));
	m_strSysPath = szBuff;
	int nPos = m_strSysPath.ReverseFind('\\');
	if ( nPos != -1 )
	{
		m_strSysPath = m_strSysPath.Left(nPos);
	}

	m_strLogFilePath = m_strSysPath + _T("\\sqlplus.log");
	m_strSqlScriptPath = m_strSysPath + _T("\\MAAccount.sql");
	m_strDumpFilePath = m_strSysPath + _T("\\maaccount.dump");
	m_strImpLogPath = m_strSysPath + _T("\\imp.log");
	m_strIniFilePath = m_strSysPath + _T("\\function.ini");
	m_strServerExeFilePath = m_strSysPath + _T("\\uniServer.exe");

	//EnableStep2Controls(FALSE);
	//EnableStep3Controls(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSetupDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSetupDlg::OnPaint()
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
HCURSOR CSetupDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSetupDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CSetupDlg::OnBnClickedButtonStep2()
{
	// TODO: Add your control notification handler code here
}

void CSetupDlg::OnBnClickedButtonStep3()
{
	// TODO: Add your control notification handler code here
}

bool CSetupDlg::WriteBatchFile(const CString& csFile)
{
	CString csCmdLineCreateUser;
	csCmdLineCreateUser.Format(_T("sqlplus.exe -L %s/%s@%s >\"%s\" @\"%s\""),
		m_strSysAccount.GetBuffer(0), m_strSysPwd.GetBuffer(0),
		m_strSID.GetBuffer(0), m_strLogFilePath.GetBuffer(0),
		m_strSqlScriptPath.GetBuffer(0));
	m_strSysAccount.ReleaseBuffer();
	m_strSysPwd.ReleaseBuffer();
	m_strSID.ReleaseBuffer();
	m_strLogFilePath.ReleaseBuffer();
	m_strSqlScriptPath.ReleaseBuffer();

	CString csCmdLineImp;
	csCmdLineImp.Format(_T("imp %s/%s@%s file=\"%s\" fromuser=maaccount touser=%s log=\"%s\""),
		m_strMAAccount.GetBuffer(0), m_strMAPwd.GetBuffer(0), m_strSID.GetBuffer(0),
		m_strDumpFilePath.GetBuffer(0), m_strMAAccount.GetBuffer(0),
		m_strImpLogPath.GetBuffer(0));
	m_strSID.ReleaseBuffer();
	m_strMAAccount.ReleaseBuffer();
	m_strMAPwd.ReleaseBuffer();
	m_strDumpFilePath.ReleaseBuffer();
	m_strImpLogPath.ReleaseBuffer();

	//������д��bat�ļ�
	CFile file;
	if ( !file.Open(csFile, CFile::modeCreate|CFile::modeWrite))
	{
		return false;
	}
	else
	{
		file.Write(csCmdLineCreateUser.GetBuffer(0), csCmdLineCreateUser.GetLength());
		csCmdLineCreateUser.ReleaseBuffer();
		file.Write(_T("\n"), 1);
		file.Write(csCmdLineImp.GetBuffer(0), csCmdLineImp.GetLength());
		csCmdLineImp.ReleaseBuffer();
		file.Write(_T("\n"), 1);
		file.Close();
		return true;
	}
}

bool CSetupDlg::ExcuteSqlScript( CString csBatPath )
{
	STARTUPINFO sInfo;
	PROCESS_INFORMATION pInfo;
	ZeroMemory(&sInfo,sizeof(sInfo));
	sInfo.cb=sizeof(sInfo);
	sInfo.dwFlags=STARTF_USESHOWWINDOW;
	sInfo.wShowWindow=SW_SHOWNORMAL;

	ZeroMemory(&pInfo,sizeof(pInfo));

	//����һ������
	if(!::CreateProcess(csBatPath, NULL,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&sInfo,
		&pInfo))
	{
		::GetLastError();
		return false;
	}

	//�Ƚ��̹ر�
	WaitForSingleObject(pInfo.hProcess,INFINITE);

	//�رս��̺��̵߳ľ��
	CloseHandle(pInfo.hProcess);
	CloseHandle(pInfo.hThread);

	return true;
}

void CSetupDlg::EnableStep1Controls( BOOL bFlag )
{
	(GetDlgItem(IDC_CHECK1))->EnableWindow(bFlag);
}

void CSetupDlg::EnableStep2Controls( BOOL bFlag )
{
	m_bStep2ControlStatus = bFlag;
	(GetDlgItem(IDC_EDIT_SID))->EnableWindow(bFlag);
	(GetDlgItem(IDC_EDIT_SYS_ACCOUNT))->EnableWindow(bFlag);
	(GetDlgItem(IDC_EDIT_SYS_PWD))->EnableWindow(bFlag);
	(GetDlgItem(IDC_EDIT_MA_ACCOUNT))->EnableWindow(bFlag);
	(GetDlgItem(IDC_EDIT_MA_PWD))->EnableWindow(bFlag);
	(GetDlgItem(IDC_BUTTON_STEP2))->EnableWindow(bFlag);
	(GetDlgItem(IDC_CHECK2))->EnableWindow(bFlag);
}

void CSetupDlg::EnableStep3Controls( BOOL bFlag )
{
	m_bStep3ControlStatus = bFlag;
	(GetDlgItem(IDC_BUTTON_STEP3))->EnableWindow(bFlag);
	(GetDlgItem(IDC_CHECK3))->EnableWindow(bFlag);
}

void CSetupDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	//EnableStep2Controls(!m_bStep2ControlStatus);
}

void CSetupDlg::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	//EnableStep3Controls(!m_bStep3ControlStatus);
}

bool CSetupDlg::IsErrorExist( const CString& csFile, const CString& csFilter, 
							 std::set<CString> setIgnore, CString& csErrorCode )
{
	CStdioFile file;
	if (!file.Open(csFile, CFile::modeRead))
	{
		return false;
	}
	else
	{
		bool bHasError = false;
		CString csLine;
		while(file.ReadString(csLine))
		{
			CString csError = csLine.Left(4);
			if ( csError == csFilter )
			{
				csError = csLine.Left(9);
				std::set<CString>::iterator it = setIgnore.find(csError);
				if ( it == setIgnore.end())
				{
					csErrorCode = csError;
					bHasError = true;
					break;
				}
			}
		}

		file.Close();
		return bHasError;
	}
}

CString CSetupDlg::GetErrorInfo( const CString& csErrorCode )
{
	CString csErrorInfo;
	if ( csErrorCode == "ORA-01017")
	{
		csErrorInfo = "ϵͳ�û�����������󣬵�¼ʧ��";
	}
	else if ( csErrorCode == "ORA-12154" )
	{
		csErrorInfo = "�޷����ӵ�ָ��SID";
	}
	else
	{

	}

	return csErrorInfo;
}

void CSetupDlg::OnBnClickedButton1()
{
	CString csText;
	((CButton*)GetDlgItem(IDC_BUTTON1))->GetWindowText(csText);
	if ( csText == "���")
	{
		OnOK();
		return;
	}

	// TODO: Add your control notification handler code here
	if (!((CButton*)(GetDlgItem(IDC_CHECK1)))->GetCheck())
	{
		MessageBox("��ȷ��oracle���ݿ��Ѱ�װ��ɣ����Ѱ�װ��ɣ��빴ѡ����һ�е�[�����]��ť");
		return;
	}

	UpdateData(TRUE);

	//ִ�в���2
	if ( m_strSysAccount.IsEmpty())
	{
		MessageBox(_T("������oracleϵͳ�û���"));
		return;
	}

	if ( m_strSysPwd.IsEmpty())
	{
		MessageBox(_T("������oracleϵͳ�û�����"));
		return;
	}

	if ( m_strMAAccount.IsEmpty())
	{
		MessageBox(_T("��������˺��û���"));
		return;
	}

	if ( m_strMAPwd.IsEmpty())
	{
		MessageBox(_T("��������˺��û�����"));
		return;
	}

	CFile file;
	if( !file.Open(m_strSqlScriptPath, CFile::modeCreate|CFile::modeWrite))
	{
		MessageBox(_T("�����ű��ļ�ʧ�ܣ������������Ŀ¼Ȩ�ޣ�"));
		return;
	}
	else
	{
		char szSql[1024];
		memset(szSql, 0, sizeof(szSql));
		sprintf(szSql, _T("create tablespace ts_zqdb datafile 'ts_zqdb.dbf' size 100M autoextend on next 10M maxsize 1000M;\n \
						  create temporary tablespace ts_tempdb tempfile 'ts_tmpdb.dbf' size 32M autoextend on next 32M maxsize 2048M extent management local;\n \
						  create user %s identified by %s default tablespace ts_zqdb temporary tablespace ts_tempdb;\n \
						  grant connect, resource to %s;\n \
						  exit;"), 
						  m_strMAAccount.GetBuffer(0), m_strMAPwd.GetBuffer(0), m_strMAAccount.GetBuffer(0));
		file.Write(szSql, strlen(szSql));
		file.Close();
	}

	CString csBatPath = m_strSysPath + _T("\\sqlplus.bat");
	if ( !WriteBatchFile(csBatPath))
	{
		MessageBox(_T("д���������ļ�ʧ�ܣ�"));
		return;
	}

	//�������̣�ִ��sql�ű�
	if(!ExcuteSqlScript(csBatPath))
	{

	}
	else
	{
		//�����־�ļ������Ƿ��д���
		std::set<CString> oraIgnore;
		oraIgnore.insert("ORA-01543");
		oraIgnore.insert("ORA-01920");

		std::set<CString> impIgnore;
		impIgnore.insert("IMP-00015");
		impIgnore.insert("IMP-00058");

		CString csErrorCode;
		if ( IsErrorExist(m_strLogFilePath, "ORA-", oraIgnore, csErrorCode))
		{
			MessageBox(GetErrorInfo(csErrorCode));
			return;
		}

		if ( IsErrorExist(m_strImpLogPath, "IMP-", impIgnore, csErrorCode))
		{
			MessageBox(GetErrorInfo(csErrorCode));
			return;
		}

		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(TRUE);
		//EnableStep2Controls(FALSE);
	}

	//ִ�в���3
	ConfigManager::GetInstance()->SetDBSID(m_strSID.GetBuffer(0));
	ConfigManager::GetInstance()->SetDBUser(m_strMAAccount.GetBuffer(0));
	ConfigManager::GetInstance()->SetDBPwd(m_strMAPwd.GetBuffer(0));
	m_strSID.ReleaseBuffer();
	m_strMAAccount.ReleaseBuffer();
	m_strMAPwd.ReleaseBuffer();
	if( !ConfigManager::GetInstance()->SaveConfig())
	{
		MessageBox("д��system.xmlʧ�ܣ�");
		return;
	}

	//��IP��ַд�뵽�����ļ�
	if ( !WriteIniFile())
	{
		MessageBox("д��function.ini�ļ�ʧ��");
		return;
	}

	//��װ����
	if( NULL == ShellExecute(NULL, "open",m_strServerExeFilePath, "install",NULL,SW_SHOWNORMAL))
	{
		MessageBox("��װuniServer����ʧ��");
		return;
	}

	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_BUTTON1))->SetWindowText("���");
}

DWORD CSetupDlg::GetLocalMatchineIP()
{
	WSADATA wsaData;
	if ( WSAStartup( MAKEWORD(2,0), &wsaData ) != 0 )
	{
		return -1;
	}

	char szLocalHostName[64];
	memset(szLocalHostName, 0, sizeof(szLocalHostName));
	gethostname(szLocalHostName,sizeof(szLocalHostName)-1);
	if(!szLocalHostName[0])
	{
		return -1;
	}

	HOSTENT* pHost = NULL;
	pHost = gethostbyname(szLocalHostName);
	if(!pHost) 
	{
		return -1;
	}

	if( pHost->h_length != 4 || pHost->h_addrtype != 2 ) 
	{
		return -1;
	}

	DWORD** ppAddress = NULL;
	ppAddress = (DWORD**)(pHost->h_addr_list);
	if(!ppAddress)
	{
		return -1;
	}

	DWORD dwLocalIP;
	for(int i = 0; i < 5; i++)
	{
		if( ppAddress[i] == NULL )
			break;

		if( (*(ppAddress[i])) != 0x0100007f)
		{
			dwLocalIP = *(ppAddress[i]);
			break;
		}
	}

	if( ppAddress[0] != NULL && dwLocalIP == 0)
	{
		dwLocalIP = *(ppAddress[0]);
	}

	WSACleanup();
	return dwLocalIP;
}

bool CSetupDlg::WriteIniFile()
{
	CFile file;
	if ( !file.Open(m_strIniFilePath, CFile::modeCreate|CFile::modeWrite))
	{
		return false;
	}
	else
	{
		DWORD dwLocalIP = GetLocalMatchineIP();
		CString strIP;
		strIP.Format(_T("%d.%d.%d.%d"), 
			dwLocalIP&0xff, (dwLocalIP>>8)&0xff, 
			(dwLocalIP>>16)&0xff, (dwLocalIP>>24)&0xff);

		std::string strTemp = "[Function]";
		file.Write(strTemp.c_str(), strTemp.length());
		file.Write("\r\n", strlen("\r\n"));
		strTemp = "DllName=MultiAccountServer.dll";
		file.Write(strTemp.c_str(), strTemp.length());
		file.Write("\r\n", strlen("\r\n"));

		CString csUdpIPAddr;
		csUdpIPAddr.Format("ip_UdpSvr=%s", strIP.GetBuffer(0));
		strIP.ReleaseBuffer();
		file.Write(csUdpIPAddr.GetBuffer(0), csUdpIPAddr.GetLength());
		file.Write("\r\n", strlen("\r\n"));
		csUdpIPAddr.ReleaseBuffer();
		CString csUdpPort;
		csUdpPort.Format("port_UdpSvr=%d", ConfigManager::GetInstance()->GetPort()-1);
		file.Write(csUdpPort.GetBuffer(0), csUdpPort.GetLength());
		file.Write("\r\n", strlen("\r\n"));
		csUdpPort.ReleaseBuffer();

		CString csTcpIPAddr;
		csTcpIPAddr.Format("ip_TcpSvr=%s", strIP.GetBuffer(0));
		strIP.ReleaseBuffer();
		file.Write(csTcpIPAddr.GetBuffer(0), csTcpIPAddr.GetLength());
		file.Write("\r\n", strlen("\r\n"));
		csTcpIPAddr.ReleaseBuffer();
		CString csTcpPort;
		csTcpPort.Format("port_TcpSvr=%d", ConfigManager::GetInstance()->GetPort());
		file.Write(csTcpPort.GetBuffer(0), csTcpPort.GetLength());
		file.Write("\r\n", strlen("\r\n"));
		csTcpPort.ReleaseBuffer();

		strTemp = "TcpSvrMaxListen=16";
		file.Write(strTemp.c_str(), strTemp.length());
		file.Write("\r\n", strlen("\r\n"));

		file.Close();
		return true;
	}
}
