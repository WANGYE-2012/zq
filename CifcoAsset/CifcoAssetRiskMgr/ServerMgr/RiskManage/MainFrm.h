// MainFrm.h : CMainFrame ��Ľӿ�
//


#pragma once

#include <map>
#include <vector>
#include "dockpagebar.h"
#include "RiskManageStruct.h"
#include "ToolBarDlg.h"

class CInfoDlg;

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

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
	afx_msg void OnQuit();
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRClickModifyAccount();
	afx_msg void OnRClickDelAccount();
	afx_msg void OnRClickStopAccount();
	afx_msg void OnToolBatchAdd();
	afx_msg void OnUpdateToolBatchAdd(CCmdUI *pCmdUI);
	afx_msg void OnOrgManage();
	afx_msg void OnProductManage();
	afx_msg void OnWarningSet();
	afx_msg void OnMsgSet();

private:
	void ShowTreeItems();
	void AddOrgItems( int nUpperLevelOrgID, HTREEITEM hParent );
	void CalcShowData(int nOrgID, std::map<int, RiskMgmtUserInfoEx>& mapShowData);
	void CalcShowData(int nOrgID, std::map<int, TrustTradeAccountEx>& mapShowData);
	void CalcShowData(int nOrgID, std::map<int, TraderInfoEx>& mapShowData);
	void GetUserInfoEx( const RiskMgmtUserInfo& userInfo, RiskMgmtUserInfoEx& userInfoEx );
	void GetTrustTradeAccountEx( const TrustTradeAccount& trustTradeAccount, 
		TrustTradeAccountEx& trustTradeAccountEx );
	void GetTraderInfoEx( const TraderInfo& traderInfo, TraderInfoEx& traderInfoEx );

	void UpdateClientAreaStatus();
	void SetModulePermissionData( std::map<int, int>& mapActionValue);
	CString ConventErrorMsg( const CString& csMsg );

	void RefreshAccountPermissionName();
	void UseNewPermissions( const std::map<int, PermissionName>& newPermission );

	void ModifyAccount(int nAccountID);
	void ModifyRiskUser( const RiskMgmtUserInfo& userInfo);
	void ModifyTrustTradeAccount( const TrustTradeAccount& trustTradeAccount);
	void ModifyTraderInfo( const TraderInfo& traderInfo);

	void StopAccount( int nAccountID );

	void RefreshListData( bool bOrg = true, bool bFinProduct = true, 
		bool bPermission = true, bool bIndModule = true);

	bool IsIndicatorModuleChanged( const std::map<int, RiskIndicatorModule> &mapIndModule );
	bool NeedRefreshOrg( std::map<int, AssetMgmtOrganization> &mapAssetOrg );

private:
	CDockPageBar m_wndDockPageBar;
	CTreeCtrl m_wndTree;
	CImageList m_imageList;
	CToolBarDlg m_toolBarDlg;
	CInfoDlg* m_pInfoDlg;

	std::map<int, RiskMgmtUserInfoEx> m_mapUserInfoEx;		//keyΪnRiskMgmtUserID
	std::map<int, RiskMgmtUserInfo> m_tempMapUserInfo;		//���ڻ���ְ����յ��˻���Ϣ

	std::map<int, TrustTradeAccountEx> m_mapTrustTradeAccountEx;	//keyΪnTradeAccountID
	std::map<int, TrustTradeAccount> m_tempMapTrustTradeAccount;	//���ڻ���ְ����յ�ί�н����˻�

	std::map<int, TraderInfoEx> m_mapTraderInfoEx;	//keyΪnTraderID
	std::map<int, TraderInfo> m_tempMapTraderInfo;	//���ڻ���ְ����յĽ���Ա�˻�

	std::map<int, AssetMgmtOrganization> m_mapAssetMgmtOrg;			//keyΪ�ʹ�����ID
	std::map<int, std::map<int, AssetMgmtOrganization>> m_mapAssetMgmtOrgTree;	//���keyΪ��һ���ʹ�����ID�����keyΪ�ʹ�����ID

	std::map<int, FinancialProduct> m_mapFinancialProduct;	//�洢��Ʋ�Ʒ��Ϣ��keyΪ��Ʋ�ƷID
	std::map<int, std::map<int, FinancialProduct>> m_mapFinancialProductTree;	//�洢��Ʋ�Ʒ��Ϣ�����keyΪ�ʹ�ID�����keyΪ��Ʋ�ƷID

	std::map<int, RiskIndicatorModule> m_mapIndModule;	//�洢����ģ��
	std::vector<RiskIndicator> m_vIndicator;		//�洢����ָ��

	int m_nSelOrgID;
	RiskMgmtUserInfo m_curUserInfo;

	//����Ա
	TraderInfo m_curTraderInfo;
	//ί�н����˻�
	TrustTradeAccount m_curTrustTradeAccount;

	std::map<int, PermissionName> m_mapAllPermission;
	
	bool m_bSearchResult;

	//Ȩ�޿��Ʊ�־λ
	bool m_bAddPermission;
	bool m_bModifyPermission;
	bool m_bDelPermission;
	bool m_bStatusChangePermission;
	bool m_bBatchAddAccountPermission;
	
	bool m_bOrgManagePermission;
	bool m_bFinProductManagePermission;
	bool m_bPermissionSetPermission;
	bool m_bRiskManagePermission;
	bool m_bRiskEvtMsgManagePermission;

	bool m_bShowTreeData;
public:
	afx_msg void OnUpdateOrgManage(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProductManage(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePermissionSet(CCmdUI *pCmdUI);
	afx_msg void OnUpdateWarningSet(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMsgSet(CCmdUI *pCmdUI);
	afx_msg void OnSpecialInd();
	afx_msg void OnUpdateSpecialInd(CCmdUI *pCmdUI);
	afx_msg void OnFundnetParam();
	afx_msg void OnUpdateFundnetParam(CCmdUI *pCmdUI);
	afx_msg void OnUpdateForceCloseParam(CCmdUI *pCmdUI);
	afx_msg void OnForceCloseParam();
};


