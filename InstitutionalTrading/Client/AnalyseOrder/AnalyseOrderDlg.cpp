// AnalyseOrderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AnalyseOrder.h"
#include "AnalyseOrderDlg.h"
#include <map>
#include <string>

using namespace std;

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


// CAnalyseOrderDlg �Ի���




CAnalyseOrderDlg::CAnalyseOrderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnalyseOrderDlg::IDD, pParent)
	, m_strTradingDay(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAnalyseOrderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_TRADINGDAY, m_strTradingDay);
	DDV_MaxChars(pDX, m_strTradingDay, 8);
}

BEGIN_MESSAGE_MAP(CAnalyseOrderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CAnalyseOrderDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAnalyseOrderDlg ��Ϣ�������

BOOL CAnalyseOrderDlg::OnInitDialog()
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

	CString strDate;
	SYSTEMTIME sysTime;

	::GetLocalTime(&sysTime);
	strDate.Format("%04d%02d%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	m_strTradingDay = strDate;
	UpdateData(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CAnalyseOrderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAnalyseOrderDlg::OnPaint()
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
HCURSOR CAnalyseOrderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAnalyseOrderDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(!UpdateData())
		return;

	map<string, ORDERANALYSE> mapData;
	map<string, ORDERANALYSE>::iterator it;
	CString strInputText("\r\n"), strFirstText("\r\n");
	CString strFileInput("userdata\\"), strFileFirst("userdata\\"), strFileCtp("userdata\\"), strFileResult("userdata\\");

	strFileInput += m_strTradingDay;
	strFileFirst += m_strTradingDay;
	strFileCtp += m_strTradingDay;
	strFileResult += m_strTradingDay;

	strFileInput += "-input.dat";
	strFileFirst += "-first.dat";
	strFileCtp += "-ctp.dat";
	strFileResult += "-result.txt";

	CFile file;
	int nInputCount = 0, nFirstCount = 0, nCtpCount = 0;
	CString strText;
	ORDERTIME item = {0};
	ORDERANALYSE oana = {0};

	if(file.Open(strFileInput, CFile::modeRead | CFile::shareDenyNone)) {
		nInputCount = file.GetLength()/sizeof(ORDERTIME);
		while(file.Read(&item, sizeof(item))) {
			ZeroMemory(&oana, sizeof(oana));
			oana.FrontID = item.FrontID;
			oana.SessionID = item.SessionID;
			strncpy(oana.OrderRef, item.OrderRef, sizeof(oana.OrderRef));
			strncpy(oana.BrokerID, item.BrokerID, sizeof(oana.BrokerID));
			strncpy(oana.InvestorID, item.InvestorID, sizeof(oana.InvestorID));
			oana.timeinput = item.DateTime;
			strText.Format("%d,%d,%s", item.FrontID, item.SessionID, item.OrderRef);
			mapData.insert(make_pair(string(strText), oana));
			strText.Format("Input����:  %s,%d,%d  \t%02d:%02d:%02d.%03d\r\n", 
					item.OrderRef, item.FrontID, item.SessionID, 
					item.DateTime.wHour, item.DateTime.wMinute, item.DateTime.wSecond, 
					item.DateTime.wMilliseconds);
			strInputText += strText;
		}
		file.Close();
	}
	else {
		AfxMessageBox("�޷��򿪽�����["+m_strTradingDay+"]�������ļ�");
		return;
	}

	if(file.Open(strFileFirst, CFile::modeRead | CFile::shareDenyNone)) {
		nFirstCount = file.GetLength()/sizeof(ORDERTIME);
		while(file.Read(&item, sizeof(item))) {
			strText.Format("%d,%d,%s", item.FrontID, item.SessionID, item.OrderRef);
			it = mapData.find(string(strText));
			if(it != mapData.end()) {
				it->second.timefirst = item.DateTime;
			}
			strText.Format("Server����:  %s,%d,%d  \t%02d:%02d:%02d.%03d\r\n", 
					item.OrderRef, item.FrontID, item.SessionID, 
					item.DateTime.wHour, item.DateTime.wMinute, item.DateTime.wSecond, 
					item.DateTime.wMilliseconds);
			strFirstText += strText;
		}
		file.Close();
	}

	if(file.Open(strFileCtp, CFile::modeRead | CFile::shareDenyNone)) {
		nCtpCount = file.GetLength()/sizeof(ORDERTIME);
		while(file.Read(&item, sizeof(item))) {
			strText.Format("%d,%d,%s", item.FrontID, item.SessionID, item.OrderRef);
			it = mapData.find(string(strText));
			if(it != mapData.end()) {
				it->second.timectp = item.DateTime;
			}
		}
		file.Close();
	}

	if(file.Open(strFileResult, CFile::modeCreate | CFile::modeWrite)) {
		
		int nSumFirst = 0, nSumCtp = 0;
		int nSumFirstError = 0, nSumCtpError = 0;
		int nMaxFirst = 0, nMaxCtp = 0;
		int nSumFirst_Over1S = 0, nSumCtp_Over1S = 0;
		int nFirstCount_Over1S = 0, nCtpCount_Over1S = 0;

		strText.Format("��������ʱ���������\r\n����Key(FrontID,SessionID,OrderRef), ʱ�䵥λ������\r\n\r\n");
		file.Write(strText, strText.GetLength());
		for(it = mapData.begin(); it != mapData.end(); it++) {
			int nTime1 = -1, nTime2 = -1;
			int nTimeFist = -1, nTimeCtp = -1;

			oana = it->second;
			TRACE(	"FrontID = %d, SessionID = %d, OrderRef = %s\n"
					"Input  Time: %04d-%02d-%02d %02d:%02d:%02d.%03d\n"
					"Server Time: %04d-%02d-%02d %02d:%02d:%02d.%03d\n"
					"Ctp    Time: %04d-%02d-%02d %02d:%02d:%02d.%03d\n\n", 
					oana.FrontID, oana.SessionID, oana.OrderRef, 
					oana.timeinput.wYear, oana.timeinput.wMonth, oana.timeinput.wDay, 
					oana.timeinput.wHour, oana.timeinput.wMinute, oana.timeinput.wSecond, 
					oana.timeinput.wMilliseconds, 
					oana.timefirst.wYear, oana.timefirst.wMonth, oana.timefirst.wDay, 
					oana.timefirst.wHour, oana.timefirst.wMinute, oana.timefirst.wSecond, 
					oana.timefirst.wMilliseconds, 
					oana.timectp.wYear, oana.timectp.wMonth, oana.timectp.wDay, 
					oana.timectp.wHour, oana.timectp.wMinute, oana.timectp.wSecond, 
					oana.timectp.wMilliseconds);

			if(oana.timefirst.wYear>0) {
				nTime2 = (oana.timefirst.wHour*3600 + oana.timefirst.wMinute*60
							+oana.timefirst.wSecond)*1000 + oana.timefirst.wMilliseconds;
				nTime1 = (oana.timeinput.wHour*3600 + oana.timeinput.wMinute*60
							+oana.timeinput.wSecond)*1000 + oana.timeinput.wMilliseconds;
				nSumFirst += nTime2-nTime1;
				nTimeFist = nTime2-nTime1;
				if(nMaxFirst < nTimeFist)
					nMaxFirst = nTimeFist;
				if(nTimeFist > 1000) {
					nSumFirst_Over1S += nTimeFist;
					nFirstCount_Over1S++;
				}
			}
			else
				nSumFirstError++;
			if(oana.timectp.wYear>0) {
				nTime2 = (oana.timectp.wHour*3600 + oana.timectp.wMinute*60
							+oana.timectp.wSecond)*1000 + oana.timectp.wMilliseconds;
				nTime1 = (oana.timeinput.wHour*3600 + oana.timeinput.wMinute*60
							+oana.timeinput.wSecond)*1000 + oana.timeinput.wMilliseconds;
				nSumCtp += nTime2-nTime1;
				nTimeCtp = nTime2-nTime1;
				if(nMaxCtp < nTimeCtp)
					nMaxCtp = nTimeCtp;
				if(nTimeCtp > 1000) {
					nSumCtp_Over1S += nTimeCtp;
					nCtpCount_Over1S++;
				}
			}
			else
				nSumCtpError++;
			strText.Format("����(%d,%d,%s) �������״η���ʱ��: %d, CTP�״η���ʱ��: %d\r\n", 
					oana.FrontID, oana.SessionID, oana.OrderRef, 
					nTimeFist, nTimeCtp);
			file.Write(strText, strText.GetLength());
		}
		strText.Format("\r\n�µ�����: %d, �������ر�����: %d, CTP�ر�����: %d\r\n", 
				nInputCount, nFirstCount, nCtpCount);
		file.Write(strText, strText.GetLength());
		strText.Format("�������޷�����: %d, CTP�޷�����: %d\r\n", 
				nInputCount-nFirstCount, nInputCount-nCtpCount);
		file.Write(strText, strText.GetLength());
		strText.Format("������ƽ������ʱ��: %d, ��󷵻�ʱ��: %d; CTPƽ������ʱ��: %d, CTP��󷵻�ʱ��: %d\r\n", 
				nFirstCount>0?nSumFirst/nFirstCount:0, nMaxFirst, nCtpCount>0?nSumCtp/nCtpCount:0, nMaxCtp);
		file.Write(strText, strText.GetLength());
		strText.Format("����1��ķ������ر�����: %d, �ۼ�ʱ��: %d, ƽ������ʱ��: %d\r\n", 
				nFirstCount_Over1S, nSumFirst_Over1S, nFirstCount_Over1S>0?nSumFirst_Over1S/nFirstCount_Over1S:0);
		file.Write(strText, strText.GetLength());
		strText.Format("����1���CTP�ر�����: %d, �ۼ�ʱ��: %d, ƽ������ʱ��: %d\r\n", 
				nCtpCount_Over1S, nSumCtp_Over1S, nCtpCount_Over1S>0?nSumCtp_Over1S/nCtpCount_Over1S:0);
		file.Write(strText, strText.GetLength());

		strInputText += "\r\n";
		strFirstText += "\r\n";
		file.Write(strInputText, strInputText.GetLength());
		file.Write(strFirstText, strFirstText.GetLength());

		file.Close();
	}
	

	//OnOK();
}
