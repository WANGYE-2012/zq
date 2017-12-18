#pragma once
#include <string>
#include "CDataInfo.h"
#include "UserApiStruct.h"


using namespace std;

class QueryInvestorInfo2
{
public:
	QueryInvestorInfo2(void);
	~QueryInvestorInfo2(void);
    //��ѯͶ������Ϣ,����Ͷ����map
   /* void ReqInvestorInfo(std::string strOrganization,std::string strFinancial, std::map<int, TrustTradeAccount>& mapAccount);*/
	void ReqInvestorInfo(std::set<int>&  OrganizationId,std::set<int> & FinancialId, std::map<int, TrustTradeAccount>& mapAccount);
	void ReqInvestorInfo(std::set<int> setAccntID,std::map<int, TrustTradeAccount>& mapAccount);

  //  bool ProcessTransferEvtPkg(const char* pPkg,std::map<int, TrustTradeAccount>& mapAccount,int fieldcnt,string& Direction);
	// ����ַ���
	std::vector<std::string> split(const std::string &str);
	//doubleתstring
	string Double2String(double dbData);
	public:
	//��������
	CDataInfo* m_pdataInfo;
	MAPTrustTradeAccount	m_mapTrustTradeAccount;
};
