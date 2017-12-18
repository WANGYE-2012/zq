#include "StdAfx.h"
#include "RiskMsgNotify2.h"
#include "RiskManageCmd.h"
#include "TcpLayer.h"

CRiskMsgNotify2::CRiskMsgNotify2(void)
{
}

CRiskMsgNotify2::~CRiskMsgNotify2(void)
{
}

//�˶�ȫ���¼�
void CRiskMsgNotify2::UnsubscribeAllEvt(unsigned int hWnd)
{
    CTcpLayer::UnsubscribeAllPkg((int)hWnd);
}

//����������Ϣ��Ӧ
void CRiskMsgNotify2::SubscribeAddMessageRsp(unsigned int hWnd)
{
    CTcpLayer::SubscribePkg(Cmd_RM_AddMessage_Rsp,(int)hWnd);
}

//�����������Ϣ����
void CRiskMsgNotify2::ReqAddMessage(MessageInfo& msgInfo,std::vector<TargetAccount>& targetAccount)
{   
    int nCount = (int)targetAccount.size();
    char* senfBuf = new char[sizeof(MessageInfo)+nCount*sizeof(TargetAccount)+1];
    memset(senfBuf,0,sizeof(MessageInfo)+nCount*sizeof(TargetAccount)+1);
    memcpy(senfBuf,(void*)&msgInfo,sizeof(MessageInfo));

    int nIndex = 0;
    std::vector<TargetAccount>::iterator it;    
    for (it=targetAccount.begin(); it!=targetAccount.end(); it++) 
    {       
        memcpy(senfBuf+sizeof(MessageInfo)+nIndex*sizeof(TargetAccount),(void*)&(*it),sizeof(TargetAccount));
        nIndex++;
    }

    CTcpLayer::SendData(Cmd_RM_AddMessage_Req, (void*)senfBuf, sizeof(MessageInfo)+nCount*sizeof(TargetAccount), 0);

    if(senfBuf != NULL)
    {
        delete senfBuf;
        senfBuf = NULL;
    }
}