#pragma once

#include <Windows.h>
#include "wx\msw\dialog.h"
#include <vector>
#include <wx/listctrl.h>
#include "../inc/Module-Misc/ExtListCtrl.h"
#include "../inc/PlusinCommon/CommonDef.h"
#include "Plusin.h"

class CPlusin;

class PlusInMgrDlg :
	public wxDialog
{
	enum
	{
		ID_LIST,
		ID_OPEN,
		ID_DEL,
		ID_START,
	};
public:
	PlusInMgrDlg(wxWindow *parent, std::map<wxString, PLUGWIN2>& mapPlusIn, CPlusin& rPlusin);
	~PlusInMgrDlg(void);
	//��Ӧ��Ӱ�ť
	void OnAdd( wxCommandEvent& event );
	//��Ӧɾ����ť
	void OnDel( wxCommandEvent& event );
	//��Ӧ�رհ�ť
	void OnClose( wxCommandEvent& event );
	//�б����
	void OnItemSelected(wxExtListEvent& event);
public:
	//���б����һ���������ܳ��ֶ��̲߳���
	void UpdatePlusIn( const wxString& strAppID, const PLUGWIN2& plusin );
	//���б�ɾ��һ���������ܳ��ֶ��̲߳���
	void DelPlusIn( const wxString& strAppID );

	DECLARE_EVENT_TABLE()

private:
	wxExtListCtrl* m_pListCtrl;
	CPlusin& m_rPlusin;
	wxStaticText* m_pStaticText;
};
