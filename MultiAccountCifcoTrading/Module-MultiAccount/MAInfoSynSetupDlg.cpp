#include "StdAfx.h"
#include "MAInfoSynSetupDlg.h"
#include "../inc/MultiAccountCore/MultiAccountCore.h"
#include "../inc/MultiAccountCore/MAConnSocket.h"
#include "../inc/Module-Misc/CryptoAPI.h"
char txtPrompt[]=
"1.�û����ϱ�����´δӸ�λ�õļ��ؽ��Զ���д���б�������ݣ������ظ���д�ķ�����������һ�����գ��������\n"
"2.ÿ�α��������������ԭ�����Ҳ��ɻָ�\n"
"3.�˳��������ý���ÿ���˳�ϵͳʱ���Զ�������˺�����û����ϵ�ָ��λ�á������������ý���ÿ�οͻ��˵�¼ʱ�Զ���ָ��λ�ü����û����ϡ�\n";
BEGIN_EVENT_TABLE(CMAInfoSynSetupDlg,wxDialog)
	EVT_BUTTON(BTN_SAVE,OnBtnSave)
	EVT_BUTTON(BTN_LOAD,OnBtnLoad)
	EVT_BUTTON(BTN_DEL_LOCAL,OnDelLocal)
	EVT_BUTTON(BTN_DEL_SERVER,OnDelServer)
	EVT_COMMAND(wxID_ANY,wxEVT_MA_SOCKET,OnMASocket)
END_EVENT_TABLE()
CMAInfoSynSetupDlg::CMAInfoSynSetupDlg(wxWindow *parent):
wxDialog(parent,wxID_ANY,"���˺��û����ϴ洢����",wxDefaultPosition,wxSize(500,300))
{
	wxChoice* pChoice=NULL;
	wxArrayString savestring,loadstring;
	savestring.Add("���ϲ�����");
	savestring.Add("����(������)���浽����");
	savestring.Add("����(������)���浽������");
	savestring.Add("����(��������)���浽����");
	savestring.Add("����(��������)���浽������");
	loadstring.Add("������");
	loadstring.Add("�ӱ��ؼ���");
	loadstring.Add("�ӷ���������");
	new wxStaticText(this,wxID_STATIC,txtPrompt,wxPoint(5,5),wxSize(490,80), 0 );
//////////////////////////////////////////////////////////////////////////////////
	int box_x=5,box_y=90;
	new wxStaticBox( this, wxID_STATIC,"��������", wxPoint(box_x,box_y), wxSize(240,100), 0 );
	new wxStaticText(this,wxID_STATIC,"�˳���������:",wxPoint(box_x+5,110),wxSize(80,20),0);
	pChoice=new wxChoice( this, CMB_AUTO_SAVE,wxPoint(box_x+85,110), wxSize(150,20),savestring, 0 );
    pChoice->SetSelection(MA_CORE()->GetSaveTypeAuto());
	new wxStaticText(this,wxID_STATIC,"�ֶ���������:",wxPoint(box_x+5,140),wxSize(80,20),0);
	pChoice=new wxChoice( this, CMB_MAN_SAVE,wxPoint(box_x+85,140), wxSize(150,20),savestring, 0 );
	pChoice->SetSelection(MA_CORE()->GetSaveTypeManual());
	new wxButton( this, BTN_SAVE,"�ֶ�����",wxPoint(box_x+110,165), wxSize(60,24), 0 );
//////////////////////////////////////////////////////////////////////////////////
	box_x=250,box_y=90;
	new wxStaticBox( this, wxID_STATIC,"��������", wxPoint(250,box_y), wxSize(240,100), 0 );
	new wxStaticText(this,wxID_STATIC,"������������:",wxPoint(box_x+5,110),wxSize(80,20),0);
	pChoice=new wxChoice( this, CMB_AUTO_LOAD,wxPoint(box_x+85,110), wxSize(150,20),loadstring, 0 );
    pChoice->SetSelection(MA_CORE()->GetLoadTypeAuto());
	new wxStaticText(this,wxID_STATIC,"�ֶ���������:",wxPoint(box_x+5,140),wxSize(80,20),0);
	pChoice=new wxChoice( this, CMB_MAN_LOAD,wxPoint(box_x+85,140), wxSize(150,20), loadstring, 0 );
	pChoice->SetSelection(MA_CORE()->GetLoadTypeManual());
	new wxButton( this, BTN_LOAD,"�ֶ�����",wxPoint(box_x+110,165), wxSize(60,24), 0 );
////////////////////////////////////////////////////////////////////////////////////////
	box_x=5,box_y=200;
	new wxStaticBox( this, wxID_STATIC,"�ֶ�ɾ������", wxPoint(box_x,box_y), wxSize(480,50), 0 );
	new wxButton( this, BTN_DEL_LOCAL,"ɾ����������",wxPoint(box_x+5,220), wxSize(100,24),  0 );
	new wxButton( this, BTN_DEL_SERVER,"ɾ������������",wxPoint(box_x+120,220), wxSize(100,24), 0 );
}
CMAInfoSynSetupDlg::~CMAInfoSynSetupDlg(void)
{
	wxChoice* pChoice=(wxChoice*)FindWindowById(CMB_AUTO_SAVE,this);
	if(pChoice) MA_CORE()->SetSaveTypeAuto(pChoice->GetSelection());
	pChoice=(wxChoice*)FindWindowById(CMB_AUTO_LOAD,this);
	if(pChoice) MA_CORE()->SetLoadTypeAuto(pChoice->GetSelection());
	pChoice=(wxChoice*)FindWindowById(CMB_MAN_SAVE,this);
	if(pChoice) MA_CORE()->SetSaveTypeManual(pChoice->GetSelection());
	pChoice=(wxChoice*)FindWindowById(CMB_MAN_LOAD,this);
	if(pChoice) MA_CORE()->SetLoadTypeManual(pChoice->GetSelection());
}
void CMAInfoSynSetupDlg::OnBtnSave(wxCommandEvent& event)
{
	wxChoice* pChoice=(wxChoice*)FindWindowById(CMB_MAN_SAVE,this);
	if(pChoice==NULL) return;
	switch(pChoice->GetSelection())
	{
	case 1:
		MA_CORE()->SaveDataToLocal(true);
		break;
	case 2:
		MA_CORE()->SaveDataToSvr(true,this);
		break;
	case 3:
		MA_CORE()->SaveDataToLocal(false);
		break;
	case 4:
		MA_CORE()->SaveDataToSvr(false,this);
		break;
	}
}
void CMAInfoSynSetupDlg::OnBtnLoad(wxCommandEvent& event)
{
	wxChoice* pChoice=(wxChoice*)FindWindowById(CMB_MAN_LOAD,this);
	if(pChoice==NULL) return;
	switch(pChoice->GetSelection())
	{
	case 1:
		{
			MA_CORE()->LoadFromLocal();
			wxCommandEvent evt(wxEVT_MA_LIST_UPDATE);
			GetParent()->AddPendingEvent(evt);
		}
		break;
	case 2:
		MA_CORE()->LoadFromSvr(this);
		break;
	}

}
void CMAInfoSynSetupDlg::OnDelLocal(wxCommandEvent& event)
{
		MA_CORE()->DelLocal();
}
void CMAInfoSynSetupDlg::OnDelServer(wxCommandEvent& event)
{
		MA_CORE()->DelServer();
}
void CMAInfoSynSetupDlg::OnMASocket(wxCommandEvent& event)
{
	RecvData* pRecvData=(RecvData*)event.GetClientData();
	if(pRecvData==NULL)return;
	if(pRecvData->head.cmdid==Cmd_MA_QryTradeAccount_Rsp)
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			MA_CORE()->ClearAllTradeAccount();
			//�����˺���Ϣ
			int count=pRecvData->head.len/sizeof(TradeAccount);
			for (int i=0;i<count;++i)
			{
				CTPTradeAccount info;
				ZeroMemory(&info,sizeof(info));
				info.t=*(TradeAccount*)((char*)pRecvData->pData+sizeof(TradeAccount)*i);
				CCryptoAPI::Decode3DES_ToString(info.t.szPassword);
				MA_CORE()->AddTradeAccount(info);
			}
			wxCommandEvent evt(wxEVT_MA_LIST_UPDATE);
			GetParent()->AddPendingEvent(evt);
			wxMessageBox("��ȡ��������¼�˺���Ϣ�ɹ�",LOADSTRING(USERLOGINDLG_INFORMATION));
		}
		else
		{
			wxMessageBox(wxString::Format("��ȡ��������¼�˺���Ϣʧ��\n%s",(char*)(pRecvData->pData)),LOADSTRING(USERLOGINDLG_INFORMATION));
		}
	}
	else if(pRecvData->head.cmdid==Cmd_MA_SaveTradeAccount_Rsp)
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			wxMessageBox("�����˺���Ϣ�ɹ�",LOADSTRING(USERLOGINDLG_INFORMATION));
		}
		else
		{
			wxMessageBox(wxString::Format("�����˺���Ϣʧ��\n%s",(char*)(pRecvData->pData)),LOADSTRING(USERLOGINDLG_INFORMATION));
		}
	}
	CMAConnSocket::GetInstance()->UnRegisteWindow(this);
}