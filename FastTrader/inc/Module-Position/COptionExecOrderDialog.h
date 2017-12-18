#pragma once
#include "stdafx.h"
#include "../inc/Module-Misc2/KernelStruct.h"

// ִ������
///TFtdcActionTypeType��һ��ִ����������
//ActionType    	ִ�л��Ƿ���

//Volume		����

//OffsetFlag	��ƽ

/////��
//#define THOST_FTDC_PD_Net '1'
/////��ͷ
//#define THOST_FTDC_PD_Long '2'
/////��ͷ
//#define THOST_FTDC_PD_Short '3'
///����ͷ������ĳֲַ���
//PosiDirection

/////TFtdcExecOrderPositionFlagType��һ����Ȩ��Ȩ���Ƿ����ڻ�ͷ��ı������
//�����򲻱���
//ReservePositionFlag

/////��Ȩ��Ȩ�����ɵ�ͷ���Ƿ��Զ�ƽ��
///TFtdcExecOrderCloseFlagType��һ����Ȩ��Ȩ�����ɵ�ͷ���Ƿ��Զ�ƽ������
///�Զ�ƽ��
///�����Զ�ƽ��
//CloseFlag



class COptionExecOrderDialog : public wxDialog
{
public:

	enum
	{
		ID_ACTIONTYPE_COMB=2001,
		ID_OFFSETFLAG_COMB,
		ID_POSIDIRECTION_COMB,
		ID_RESERVEPOSITIONFLAG_COMB,
		ID_CLOSEFLAG_COMB,
		ID_VOLUME_TEXT,
		ID_OK,
		ID_CANCEL,
		ID_INSTRUMENTID_STATIC,
		ID_STATIC,
		SUMNUM_ID	// ��־ID�ŵ�����
	};


public:
	COptionExecOrderDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr);
	~COptionExecOrderDialog();
    
	static COptionExecOrderDialog* CreateDailog(wxWindow* parent);
	void SetOptionPosition(PlatformStru_Position& position) {
		m_Position = position;
		wxStaticText* pStc = (wxStaticText*)FindWindowById(ID_INSTRUMENTID_STATIC, this);
		if(pStc != NULL) {
			pStc->SetLabel(wxString("��Ȩ��Լ: ")+position.InstrumentID);
		}
		wxTextCtrl* pTxt = (wxTextCtrl*)FindWindowById(ID_VOLUME_TEXT, this);
		if(pTxt != NULL) {
			wxString strText;
			strText.Printf("%d", position.CanCloseVolume);
			pTxt->SetValue(strText);
		}
	};

	void OnBtnOrder(wxCommandEvent& evt);
	void OnBtnClose(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE()

public:
	void ResetLanguage();

private:
	PlatformStru_Position m_Position;
};