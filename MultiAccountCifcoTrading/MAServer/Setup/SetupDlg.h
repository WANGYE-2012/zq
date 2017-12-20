// SetupDlg.h : ͷ�ļ�
//

#pragma once
#include <set>

// CSetupDlg �Ի���
class CSetupDlg : public CDialog
{
// ����
public:
	CSetupDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SETUP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonStep2();
	afx_msg void OnBnClickedButtonStep3();

	CString m_strSID;
	CString m_strSysAccount;
	CString m_strSysPwd;
	CString m_strMAAccount;
	CString m_strMAPwd;
	CString m_strSysPath;
	CString m_strLogFilePath;
	CString m_strImpLogPath;
	CString m_strSqlScriptPath;
	CString m_strDumpFilePath;
	CString m_strIniFilePath;
	CString m_strServerExeFilePath;

	BOOL m_bStep2ControlStatus;
	BOOL m_bStep3ControlStatus;

private:
	bool WriteBatchFile( const CString& csFile);
	bool ExcuteSqlScript( CString csBatPath );
	void EnableStep1Controls(BOOL bFlag);
	void EnableStep2Controls(BOOL bFlag);
	void EnableStep3Controls(BOOL bFlag);
	bool IsErrorExist(const CString& csFile, const CString& csFilter, 
		std::set<CString> setIgnore, CString& csErrorCode);
	CString GetErrorInfo(const CString& csErrorCode);

	DWORD GetLocalMatchineIP();
	bool WriteIniFile();

public:
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedButton1();
};
