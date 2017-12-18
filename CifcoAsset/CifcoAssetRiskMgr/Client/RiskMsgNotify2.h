#pragma once

#include <vector>
#include "RiskManageStruct.h"

class CRiskMsgNotify2
{
public:
    CRiskMsgNotify2(void);
    ~CRiskMsgNotify2(void);

    static CRiskMsgNotify2* NewInstance() {return new CRiskMsgNotify2();}
    static void DestroyInstance(CRiskMsgNotify2* instance) {if(instance!=NULL) delete instance;}

    //�˶�ȫ���¼�
    static void UnsubscribeAllEvt(unsigned int hWnd);
    //����������Ϣ��Ӧ
    static void SubscribeAddMessageRsp(unsigned int hWnd);
    //�����������Ϣ����
    static void ReqAddMessage(MessageInfo& msgInfo,std::vector<TargetAccount>& targetAccount);  

    void ClearRiskMgmtUserInfo(){m_vecRiskMgmtUserInfo.clear();}
    void ClearTargetAccount(){m_vecTargetAccount.clear();}

    void SetRiskMgmtUserInfo(RiskMgmtUserInfo& riskMgmtUserInfo){m_vecRiskMgmtUserInfo.push_back(riskMgmtUserInfo);}
    void SetTargetAccount(TargetAccount& targetAccount){m_vecTargetAccount.push_back(targetAccount);}

    std::vector<RiskMgmtUserInfo> GetRiskMgmtUserInfo(){return m_vecRiskMgmtUserInfo;}
    std::vector<TargetAccount> GetTargetAccount(){return m_vecTargetAccount;}

private:
    std::vector<RiskMgmtUserInfo> m_vecRiskMgmtUserInfo;//���еķ��Ա
    std::vector<TargetAccount> m_vecTargetAccount;//Ҫ���͵�Ŀ����Ա
};
