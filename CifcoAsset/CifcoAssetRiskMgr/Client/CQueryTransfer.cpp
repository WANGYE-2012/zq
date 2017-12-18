#include "StdAfx.h"
#include "CQueryTransfer.h"


#include <windows.h>
#include "RiskManageCmd.h"
#include "RiskManageStruct.h"
#include "CommonPkg.h"
#include "TcpLayer.h"

//�����¼�
void CQueryTransfer::SubscribeEvt(unsigned int hWnd)
{
	CTcpLayer::SubscribePkg(Cmd_RM_SubscribeDeposit_Req,(HWND)hWnd);
}
//�˶��¼�
void CQueryTransfer::UnsubscribeEvt(unsigned int hWnd)
{
	CTcpLayer::UnsubscribeAllPkg((HWND)hWnd);
}


//�����������Ͷ���߳����
void CQueryTransfer::ReqQueryTransfer(std::string strAccountID)
{
	char buf[sizeof(Stru_UniPkgHead)+4];
	Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)buf;
	PkgHead.Set(0,Cmd_RM_Deposit_Push,0,0);
	*(unsigned int*)(buf+sizeof(PkgHead))=TailID_UniPkg;

	CTcpLayer::Senddata(buf,sizeof(buf));
}

