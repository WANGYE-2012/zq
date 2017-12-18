#pragma once
#include "IPlatformSingleSvr.h"
#include "platformID.h"
#include "easymutex.h"
#include <map>
#include <vector>
#include <string>
#include <set>
using std::map;
using std::string;
using std::set;
using std::vector;

class PlatformSvrMgr
{
public:
	static void CreateInstance();
	static void DestroyInstance();

    void CreateAccount(ePLATFORMTYPE PlatformType=PTYPE_NULL,const string& strAccount = "");
	void ReleaseAccount( const string strAccount);
	void ReleaseAccount( IPlatformSingleSvr* pSvr);
    void ReleaseAllAccount();
	IPlatformSingleSvr* GetPlatformSvr( const string strAccount = "");
	IPlatformSingleSvr* GetPlatformSvr( int PlatformID );
    string GetAccount(int PlatformID);
	IPlatformSingleSvr* GetDefaultPlatformSvr();
	IPlatformSingleSvr* GetCurrentPlatformSvr();
	IPlatformSingleSvr* GetFirstPlatformSvr();
	void SetCurrentPlatformSvr(IPlatformSingleSvr* pSvr);
	void GetAllPlatformSvr(set<IPlatformSingleSvr*>& vec);
	vector<IPlatformSingleSvr*> GetLogonPlatformSvr();
    vector<string> GetLogonAccounts();
    //ȡPlatformID��ʧ�ܷ���-1
    int GetPlatformID(IPlatformSingleSvr* pSvr);
    int GetPlatformID(const string& strAccount);

    void SetAccount(IPlatformSingleSvr* pSvr,const string& strAccount);
    //֪ͨĳ��Svr��ʼ����ѯ���
    void NotifyInitQrySucc(IPlatformSingleSvr* pSvr);

private:
	PlatformSvrMgr();
	~PlatformSvrMgr();
    
    //����ȱʡSvr���������ѵ�¼��PlatformID��С�ģ�������PlatformID��С��
    void PlatformSvrMgr::ResetDefSvr_Internal(void);

private:
	map<string, IPlatformSingleSvr*> m_allPlatform;
    IPlatformSingleSvr* m_pDefSvr;
    IPlatformSingleSvr* m_pCurSvr;
    IPlatformSingleSvr* m_pFirstSvr;        //�ڶ��˺��У������Ӧ��¼���˺ŵ�Svr�������ж��˺ŵ�¼�û�����������Ϣ

    map<IPlatformSingleSvr*, int> m_mapPlatformID;
    map<int, IPlatformSingleSvr*> m_mapPlatformID2;
    //int m_platformID;

    Ceasymutex m_mutex;

};