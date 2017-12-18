#pragma once

#include <map>
#include "RiskManageStruct.h"

class CRiskMsgNotifyTip2
{
public:
    CRiskMsgNotifyTip2(void);
    ~CRiskMsgNotifyTip2(void);

    static CRiskMsgNotifyTip2* NewInstance() {return new CRiskMsgNotifyTip2();}
    static void DestroyInstance(CRiskMsgNotifyTip2* instance) {if(instance!=NULL) delete instance;}

    //�˶�ȫ���¼�
    static void UnsubscribeAllEvt(unsigned int hWnd);
    //������Ϣ������Ӧ
    static void SubscribeAddMessagePush(unsigned int hWnd);

    void SetMessageIndex(int nCount);
    void SetMessageInfo(MessageInfo& oMessageInfo);

    MessageInfo* GetPreMessageInfo();
    MessageInfo* GetNextmessageInfo();
    int          GetMessageInfoCount();

private:
    int m_MessageIndex;
    int m_CurMessageIndex;
    std::map<int,MessageInfo> m_mapMessageInfo;
};
