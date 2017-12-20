// MainFrm.h : CMainFrame ��Ľӿ�
//


#pragma once

#include <map>
#include <vector>
#include "dockpagebar.h"
#include "MultiAccountStruct.h"
#include "ToolBarDlg.h"

class CInfoDlg;

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
	//struct SeachParam 
	//{
	//	AccountType eType;
	//	int nOwnerAccountID;
	//	CString csAccount;
	//	CString csDate;

	//	SeachParam()
	//	{
	//		eType == ACCOUNT_TYPE_LOGON;
	//		nOwnerAccountID == 0;
	//		csAccount = _T("");
	//		csDate = _T("");
	//	}
	//};

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

public:
	//virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnModifyPwd();
	afx_msg void OnBrokerManage();
	afx_msg void OnQuit();
	afx_msg void OnServerManage();
	afx_msg void OnPermissionSet();
	afx_msg void OnAddAccount();
	afx_msg void OnModifyAccount();
	afx_msg void OnDelAccount();
	afx_msg void OnRefresh();
	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSearchMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnListSelChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMainTabSelChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnModifyAccountMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDelAccountMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddAccountMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateManageList(CCmdUI *pCmdUI);
	afx_msg void OnManageList();
	afx_msg void OnUpdateToolAdd(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnTreeCtrlSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeCtrlRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolStop();

	afx_msg void OnUpdateToolModify(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolDel(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolStop(CCmdUI *pCmdUI);
	afx_msg void OnUserSet();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRClickModifyAccount();
	afx_msg void OnRClickDelAccount();
	afx_msg void OnRClickStopAccount();
	afx_msg void OnToolBatchAdd();
	afx_msg void OnUpdateToolBatchAdd(CCmdUI *pCmdUI);

private:
	void ShowTreeItems();
	void AddChildItem(const AccountInfo& accountInfo );
	void DelChildItem( int nAccountID );
	void DelChildItem(HTREEITEM hParent, int nAccountID);
	void AddAccountItems( int nOwnerID, HTREEITEM hParent );
	void CalcShowData(int nOwnerID, std::map<int, AccountInfoEx>& mapShowData);
	void GetAccountInfoEx( const AccountInfo& accountInfo, AccountInfoEx& accountInfoEx );
	void UpdateMenuStatus();
	void UpdateToolBarStatus();
	void UpdateClientAreaStatus();
	void SetModulePermissionData( const Permission& permissionInfo);
	CString ConventErrorMsg( const CString& csMsg );

	void RefreshAccountPermissionName();
	void UseNewPermissions( const std::map<int, Permission>& newPermission );
	void ModifyAccount( const AccountInfo& accountInfo);
	void StopAccount( int nAccountID );

private:
	CDockPageBar m_wndDockPageBar;
	CTreeCtrl m_wndTree;
	CImageList m_imgState;
	CImageList m_imageList;
	CToolBarDlg m_toolBarDlg;

	std::map<int, AccountInfoEx> m_mapAccount;		//keyΪAccountID
	std::map<int, AccountInfoEx> m_tempMapAccount;		//���ڻ���ְ����յ��˻���Ϣ
	std::map<int, std::map<int, AccountInfoEx>> m_mapAccountTree;	//���keyΪOwnerID�����keyΪAccountID
	int m_nSelAccountID;
	AccountInfo m_curAccountInfo;

	std::map<int, Permission> m_mapAllPermission;
	std::vector<Permission> m_vPermission;
	PermissionData m_foreAccountPermission;
	PermissionData m_bkgAccountPermission;
	PermissionData m_brokerPermission;
	PermissionData m_serverPermission;
	PermissionData m_permissionSetPermission;

	std::vector<IDCardInfo> m_vIDCardInfo;
	
	bool m_bSearchResult;
	//SeachParam m_seachParam;
	bool m_bAccountMenuStatus;

	//Ȩ�޿��Ʊ�־λ
	bool m_bAddPermission;
	bool m_bModifyPermission;
	bool m_bDelPermission;
	bool m_bStatusChangePermission;
	bool m_bShowTreeData;
	CInfoDlg* m_pInfoDlg;
public:
	afx_msg void OnToolMessage();
	afx_msg void OnUpdateToolMessage(CCmdUI *pCmdUI);
};


