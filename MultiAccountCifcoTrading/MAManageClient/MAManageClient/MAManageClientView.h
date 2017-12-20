// MAManageClientView.h : CMAManageClientView ��Ľӿ�
//


#pragma once
#include "AccountManageDlg.h"
#include "MultiAccountStruct.h"
#include <map>
#include <vector>


class CMAManageClientView : public CFormView
{
protected: // �������л�����
	CMAManageClientView();
	DECLARE_DYNCREATE(CMAManageClientView)

public:
	enum{ IDD = IDD_MACLIENT_FORM };

// ����
public:
	CMAManageClientDoc* GetDocument() const;

// ����
public:
	int GetCurPageIndex();
	AccountType GetAccountType();
	void GetSelectAccountID(std::vector<int>& vec);
	void ClearAllData();
	void ClearData( AccountType eType );
	void SetData( std::map<int, AccountInfoEx>& accountMap );
	void SetData( AccountType eType, std::map<int, AccountInfoEx>& accountMap );
	void AddAccount( const AccountInfoEx& accountInfoEx);
	void AddAccount( AccountType eType, const AccountInfoEx& accountInfoEx);
	void ModifyAccount( const AccountInfoEx& accountInfoEx);
	void DelAccount( int nAccountID, AccountType eType );
	void ShowForeListData(bool bShow);
	void ShowBkgListData(bool bShow);
	void SetPermissionData(const PermissionData& foreData, const PermissionData& bkgData);

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// ʵ��
public:
	virtual ~CMAManageClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

public:
	CTabCtrl m_mainTab;
	CAccountManageDlg m_bkgAccountDlg;
	CAccountManageDlg m_foreAccountDlg;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnTcnSelchangeTabView(NMHDR *pNMHDR, LRESULT *pResult);

private:
	bool m_bInited;
	bool m_bShowForeListData;
	bool m_bShowBkgListData;
};

#ifndef _DEBUG  // MAManageClientView.cpp �еĵ��԰汾
inline CMAManageClientDoc* CMAManageClientView::GetDocument() const
   { return reinterpret_cast<CMAManageClientDoc*>(m_pDocument); }
#endif

