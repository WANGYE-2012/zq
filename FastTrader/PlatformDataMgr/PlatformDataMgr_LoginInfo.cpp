//���ڱ����ά�����׵�¼���������

#pragma warning(disable:4996)


#include "PlatformDataMgr_LoginInfo.h"
#include "PlatformDataMgrMacro.h"
#include "FileOpr.h"

#include <string>
using namespace std;


#ifdef WIN32
#pragma unmanaged
#endif 


//���죺
CPlatformDataMgr_LoginInfo::CPlatformDataMgr_LoginInfo(void)
//: m_TradingFrontID(0),
//  m_TradingSessionID(0)
{
    memset(&m_LoginInfo,0,sizeof(m_LoginInfo));
    //memset(m_UserID,0,sizeof(m_UserID));
}

CPlatformDataMgr_LoginInfo::~CPlatformDataMgr_LoginInfo(void)
{
    LOG_INFO("CPlatformDataMgr_LoginInfo::~CPlatformDataMgr_LoginInfo(����CPlatformDataMgr_LoginInfo����)");
}

//���ý������ӵ�FrontID��SessionID
void CPlatformDataMgr_LoginInfo::SetTradingLoginInfo(const PlatformStru_LoginInfo& LoginInfo)
{
    m_LoginInfo=LoginInfo;
    //m_TradingFrontID=FrontID;
    //m_TradingSessionID=SessionID;

    //memset(m_UserID,0,sizeof(m_UserID));
    //if(UserID)
    //    strncpy(m_UserID,UserID,sizeof(m_UserID)-1);
}

//��ȡ�������ӵ�FrontID��SessionID
const PlatformStru_LoginInfo& CPlatformDataMgr_LoginInfo::GetTradingLoginInfo(void)
{
    return m_LoginInfo;
    //FrontID=m_TradingFrontID;
    //SessionID=m_TradingSessionID;
    //UserID.assign(m_UserID);
}

void CPlatformDataMgr_LoginInfo::clear(void)
{
    m_LoginInfo.clear();
    //m_TradingFrontID=0;
    //m_TradingSessionID=0;
    //memset(m_UserID,0,sizeof(m_UserID));
}
