#pragma once
#include "CDataInfo.h"
//#include "CFinancialProductIDListDlg.h"
//#include "CAssetMgmtOrganIDTreeListDlg.h"
//#include "COrganAndAccountIDTreeListDlg.h"
# define MsgStatus_All    10

struct RiskEventKey
{
	RiskEventIDType			nRiskEventID;
	RiskEventSubIDType		nRiskEventSubID;
	bool operator<(const RiskEventKey item) const
	{
		if(nRiskEventID > item.nRiskEventID)
			return false;
		else if(nRiskEventID < item.nRiskEventID)
			return true;

		if(nRiskEventSubID > item.nRiskEventSubID)
			return false;
		else if(nRiskEventSubID < item.nRiskEventSubID)
			return true;

		
		return false;
	}
};

class QueryRiskEvt2
{
public:
	QueryRiskEvt2(void);
	~QueryRiskEvt2(void);
	//�����¼�
	static void SubscribeEvt(unsigned int hWnd);
	//�˶��¼�
	static void UnsubscribeEvt(unsigned int hWnd);

	//������������ѯ�����¼�
    void ReqQueryRiskEvt(std::set<int> setAccntID, std::string strDateStart, std::string strDateEnd );
	//��������¼���Ϣ
    bool ProcessQueryRiskEvtPkg(const char* pPkg,std::map<int, RiskEvent>& mapRiskEvent,int fieldcnt);


public:
	//��������
	CDataInfo* m_pdataInfo;
	/*std::map<RiskEventKey, RiskEvent> mapRiskEvent;*/
	//CAssetMgmtOrganIDTreeListDlg^     dlgMgmtOrganIDTreeList;
	//CFinancialProductIDListDlg^       dlgFinancialProductIDList;
	//COrganAndAccountIDTreeListDlg^    dlgOrganAndAccountIDTreeList;

};
