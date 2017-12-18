// Global.cpp : ֻ������׼�����ļ���Դ�ļ�
// Module-BookOrder.pch ����ΪԤ����ͷ
// Global.obj ������Ԥ����������Ϣ

#include "stdafx.h"
#include "Global.h"
#include "tinyxml\tinyxml.h"
#include <string>

using namespace std;

HANDLE g_hMutex = CreateMutex(NULL, FALSE, NULL);

BOOL LockGlobalObject()
{
	::WaitForSingleObject(g_hMutex, INFINITE);
	return TRUE;
}

BOOL UnlockGlobalObject()
{
	::ReleaseMutex(g_hMutex);
	return TRUE;
}

BOOL g_bInitStatus = FALSE;

BOOL GetInitStatus()
{
	return g_bInitStatus;
}

void SetInitStatus(BOOL bIsSuccess)
{
	g_bInitStatus = bIsSuccess;
}

HWND g_hTargetWnd = NULL;

void SetTargetHWnd(HWND hTargetWnd)
{
	g_hTargetWnd = hTargetWnd;
}

HWND GetTargetHWnd()
{
	return g_hTargetWnd;
}

PlusinAPI* g_poPlusinApi = NULL;

void SetPlusinAPI(PlusinAPI* poPlusinApi)
{
	g_poPlusinApi = poPlusinApi;
}

PlusinAPI* GetPlusinAPI()
{
	return g_poPlusinApi;
}

UINT g_nTTableCloseMSGID = ::RegisterWindowMessage("OptionPriceListT TTable Dialog Close");

UINT GetTTableCloseMSGID()
{
	return g_nTTableCloseMSGID;
}

UINT g_nColSetChangedMSGID = ::RegisterWindowMessage("OptionPriceListT Cols Set Changed");

UINT GetColSetChangedMSGID()
{
	return g_nColSetChangedMSGID;
}

BOOL g_bIsConfirmRaskInfo = FALSE;

void SetIsConfirmRaskInfo(BOOL bConfirm)
{
	g_bIsConfirmRaskInfo = bConfirm;
}

BOOL GetIsConfirmRaskInfo()
{
	return g_bIsConfirmRaskInfo;
}

CString g_strProductShortNames;

void SetProductShortNames(CString& strProductShortNames)
{
	g_strProductShortNames = strProductShortNames;
}

CString& GetProductShortNames()
{
	return g_strProductShortNames;
}

std::set<std::string> g_setExchangeID;

void SetExchangeIDList(std::set<std::string>& setExchangeID)
{
	g_setExchangeID = setExchangeID;
}

std::set<std::string>& GetExchangeIDList()
{
	return g_setExchangeID;
}

std::set<std::string> g_setInstrumentID;

void SetInstrumentIDList(std::set<std::string>& setInstrumentID)
{
	g_setInstrumentID = setInstrumentID;
}

std::set<std::string>& GetInstrumentIDList()
{
	return g_setInstrumentID;
}

std::map<std::string, PlatformStru_InstrumentInfo> g_mapInstrumentInfo;

std::map<std::string, PlatformStru_InstrumentInfo>& GetInstrumentInfos()
{
	return g_mapInstrumentInfo;
}

std::map<std::string, std::map<std::string, std::set<std::string>>> g_mapOptionTargetRelate;
std::map<std::string, std::string> g_mapOption2Target;

void ParseOptionInstrument(std::map<std::string, PlatformStru_InstrumentInfo>& mapInstrument)
{
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it;
	std::map<std::string, std::set<std::string>>::iterator itPrice;
	std::map<std::string, std::map<std::string, std::set<std::string>>>::iterator itTarg;
	CString strExchangeID, strTargetID, strPrice;
	BOOL bDirect = FALSE;
	for(it = mapInstrument.begin(); it != mapInstrument.end(); it++) {
		if(it->second.ProductClass==THOST_FTDC_PC_Options) {
			strExchangeID = it->second.ExchangeID;
			if(ParseOptionFormat(CString(it->second.InstrumentID), strExchangeID, strTargetID, bDirect, strPrice)) {
				if(stricmp(it->second.ExchangeID, "CFFEX")==0) {
					strTargetID.Replace("IO", "IF");
					strTargetID.Replace("HO", "IH");
				}
				g_mapOption2Target.insert(std::make_pair(it->second.InstrumentID, std::string(strTargetID)));
				itTarg = g_mapOptionTargetRelate.find(strTargetID.GetBuffer(0));
				if(itTarg != g_mapOptionTargetRelate.end()) {
					itPrice = itTarg->second.find(strPrice.GetBuffer(0));
					if(itPrice != itTarg->second.end()) {
						itPrice->second.insert(it->second.InstrumentID);
					}
					else {
						std::set<std::string> setOption;
						setOption.insert(it->second.InstrumentID);
						itTarg->second.insert(std::make_pair(strPrice, setOption));
					}
				}
				else {
					std::set<std::string> setOption;
					std::map<std::string, std::set<std::string>> mapPrice;
					setOption.insert(it->second.InstrumentID);
					mapPrice.insert(std::make_pair(strPrice, setOption));
					g_mapOptionTargetRelate.insert(std::make_pair(strTargetID, mapPrice));
				}
			}
		}
	}
}

std::map<std::string, std::map<std::string, std::set<std::string>>>& GetOptionTargetRelate()
{
	return g_mapOptionTargetRelate;
}

BOOL FindTargetID(const CString& strOptionID, CString& strTargetID) 
{
	std::map<std::string, std::string>::iterator it = g_mapOption2Target.find(std::string(strOptionID));
	if(it != g_mapOption2Target.end()) {
		strTargetID = it->second.c_str();
		return TRUE;
	}
	else {
		return FALSE;
	}
}

std::set<CWnd*> g_setTTableDlgSet;

std::set<CWnd*>& GetTTableDlgSet()
{
	return g_setTTableDlgSet;
}

// һ���������
void ReleaseGlobalObject()
{
#if 0
	std::set<CWnd*>& setDlg = GetBookOrderDlgSet();
	std::set<CWnd*>::iterator it = setDlg.begin();
	for(; it!=setDlg.end(); it++)
	{
		if((*it)!=NULL && IsWindow((*it)->GetSafeHwnd()))
			(*it)->ShowWindow(SW_HIDE);
	}
	Sleep(20);
	CWnd* pWnd = NULL;
	it=setDlg.begin();
	while(it!=setDlg.end()) 
	{
		pWnd = *it;
		it=setDlg.erase(it);
		if(pWnd!=NULL)
			delete pWnd;
	}
#endif
}

CString GetIniFile()
{
	char strPath[1024];
	ZeroMemory(strPath, sizeof(strPath));
	DWORD dwRet = 0;
	dwRet = ::GetModuleFileName(NULL, strPath, 1023);
	if(dwRet<4)
		return CString("");
	strPath[dwRet-4] = '\000';
	CString strIniFile(strPath);
	strIniFile += ".INI";
	return strIniFile;
}

CString GetXmlFile()
{
	int nPathLen = 0;
	char strPath[1024];
	ZeroMemory(strPath, sizeof(strPath));
	nPathLen = GetModuleFileName(NULL, strPath, 1023);
	string filename=strPath;
	size_t splitpos=filename.find_last_of('\\');
	string strXmlFile(filename.substr(0, splitpos+1).c_str());
	strXmlFile += "grid.xml";
	return strXmlFile.c_str();
}

std::map<int, std::string> g_mapTargetCols;
std::map<int, std::string> g_mapOptionCols;

const std::map<int, std::string>& GetTargetColMap()
{
	return g_mapTargetCols;
}

const std::map<int, std::string>& GetOptionColMap()
{
	return g_mapOptionCols;
}

void InitGridMap()
{
	g_mapTargetCols.clear();
	g_mapTargetCols[QUOT_InstrumentID] = "��Լ";
	g_mapTargetCols[QUOT_LastPrice] = "���¼�";
	g_mapTargetCols[QUOT_ChangeValue] = "�ǵ�";
	g_mapTargetCols[QUOT_ChangePercentage] = "�ǵ���%";
	g_mapTargetCols[QUOT_BidPrice1] = "���";
	g_mapTargetCols[QUOT_BidVolume1] = "����";
	g_mapTargetCols[QUOT_AskPrice1] = "����";
	g_mapTargetCols[QUOT_AskVolume1] = "����";
	g_mapTargetCols[QUOT_OpenInterest] = "�ֲ���";
	g_mapTargetCols[QUOT_OpenPrice] = "���̼�";
	g_mapTargetCols[QUOT_PreSettlementPrice] = "������";
	g_mapTargetCols[QUOT_LastVolume] = "����";
	g_mapTargetCols[QUOT_HighestPrice] = "��߼�";
	g_mapTargetCols[QUOT_LowestPrice] = "��ͼ�";
	g_mapTargetCols[QUOT_ClosePrice] = "���̼�";
	g_mapTargetCols[QUOT_Volume] = "�ɽ���";
	g_mapTargetCols[QUOT_Turnover] = "�ɽ����";
	g_mapTargetCols[QUOT_UpperLimitPrice] = "��ͣ��";
	g_mapTargetCols[QUOT_LowerLimitPrice] = "��ͣ��";
	g_mapTargetCols[QUOT_SettlementPrice] = "�����";
	g_mapTargetCols[QUOT_PreClosePrice] = "���ռ�";
	g_mapTargetCols[QUOT_PreOpenInterest] = "��ֲ���";
	g_mapTargetCols[QUOT_UpdateTime] = "����ʱ��";

	g_mapOptionCols.clear();
	g_mapOptionCols[QUOT_InstrumentID] = "��Լ";
	g_mapOptionCols[QUOT_LastPrice] = "���¼�";
	g_mapOptionCols[QUOT_ChangeValue] = "�ǵ�";
	g_mapOptionCols[QUOT_ChangePercentage] = "�ǵ���%";
	g_mapOptionCols[QUOT_BidPrice1] = "���";
	g_mapOptionCols[QUOT_BidVolume1] = "����";
	g_mapOptionCols[QUOT_AskPrice1] = "����";
	g_mapOptionCols[QUOT_AskVolume1] = "����";
	g_mapOptionCols[QUOT_OpenInterest] = "�ֲ���";
	g_mapOptionCols[QUOT_OpenPrice] = "���̼�";
	g_mapOptionCols[QUOT_PreSettlementPrice] = "������";
	g_mapOptionCols[QUOT_LastVolume] = "����";
	g_mapOptionCols[QUOT_HighestPrice] = "��߼�";
	g_mapOptionCols[QUOT_LowestPrice] = "��ͼ�";
	g_mapOptionCols[QUOT_ClosePrice] = "���̼�";
	g_mapOptionCols[QUOT_Volume] = "�ɽ���";
	g_mapOptionCols[QUOT_Turnover] = "�ɽ����";
	g_mapOptionCols[QUOT_UpperLimitPrice] = "��ͣ��";
	g_mapOptionCols[QUOT_LowerLimitPrice] = "��ͣ��";
	g_mapOptionCols[QUOT_SettlementPrice] = "�����";
	g_mapOptionCols[QUOT_PreClosePrice] = "���ռ�";
	g_mapOptionCols[QUOT_PreOpenInterest] = "��ֲ���";
	g_mapOptionCols[QUOT_UpdateTime] = "����ʱ��";

	g_mapOptionCols[QUOT_OptionPrice] = "���ǿ�����";

	int nNewHead = QUOTNEWHEAD;

	g_mapOptionCols[nNewHead + QUOT_InstrumentID] = "��Լ";
	g_mapOptionCols[nNewHead + QUOT_LastPrice] = "���¼�";
	g_mapOptionCols[nNewHead + QUOT_ChangeValue] = "�ǵ�";
	g_mapOptionCols[nNewHead + QUOT_ChangePercentage] = "�ǵ���%";
	g_mapOptionCols[nNewHead + QUOT_BidPrice1] = "���";
	g_mapOptionCols[nNewHead + QUOT_BidVolume1] = "����";
	g_mapOptionCols[nNewHead + QUOT_AskPrice1] = "����";
	g_mapOptionCols[nNewHead + QUOT_AskVolume1] = "����";
	g_mapOptionCols[nNewHead + QUOT_OpenInterest] = "�ֲ���";
	g_mapOptionCols[nNewHead + QUOT_OpenPrice] = "���̼�";
	g_mapOptionCols[nNewHead + QUOT_PreSettlementPrice] = "������";
	g_mapOptionCols[nNewHead + QUOT_LastVolume] = "����";
	g_mapOptionCols[nNewHead + QUOT_HighestPrice] = "��߼�";
	g_mapOptionCols[nNewHead + QUOT_LowestPrice] = "��ͼ�";
	g_mapOptionCols[nNewHead + QUOT_ClosePrice] = "���̼�";
	g_mapOptionCols[nNewHead + QUOT_Volume] = "�ɽ���";
	g_mapOptionCols[nNewHead + QUOT_Turnover] = "�ɽ����";
	g_mapOptionCols[nNewHead + QUOT_UpperLimitPrice] = "��ͣ��";
	g_mapOptionCols[nNewHead + QUOT_LowerLimitPrice] = "��ͣ��";
	g_mapOptionCols[nNewHead + QUOT_SettlementPrice] = "�����";
	g_mapOptionCols[nNewHead + QUOT_PreClosePrice] = "���ռ�";
	g_mapOptionCols[nNewHead + QUOT_PreOpenInterest] = "��ֲ���";
	g_mapOptionCols[nNewHead + QUOT_UpdateTime] = "����ʱ��";
}

// ����ȷʵ����ʾ��
void ResetDefaultShowCol()
{
	std::vector<int> vecTargetCol;
	vecTargetCol.push_back(QUOT_InstrumentID);
	vecTargetCol.push_back(QUOT_LastPrice);
	vecTargetCol.push_back(QUOT_ChangeValue);
	vecTargetCol.push_back(QUOT_ChangePercentage);
	vecTargetCol.push_back(QUOT_BidPrice1);
	vecTargetCol.push_back(QUOT_BidVolume1);
	vecTargetCol.push_back(QUOT_AskPrice1);
	vecTargetCol.push_back(QUOT_AskVolume1);

	LockGlobalObject();
	SetTargetShowCol(vecTargetCol);
	UnlockGlobalObject();

	std::vector<int> vecOptionCol;
	int nNewHead = QUOTNEWHEAD;

	//vecOptionCol.push_back(QUOT_InstrumentID);
	vecOptionCol.push_back(QUOT_AskVolume1);
	vecOptionCol.push_back(QUOT_AskPrice1);
	vecOptionCol.push_back(QUOT_BidVolume1);
	vecOptionCol.push_back(QUOT_BidPrice1);
	vecOptionCol.push_back(QUOT_ChangePercentage);
	vecOptionCol.push_back(QUOT_ChangeValue);
	vecOptionCol.push_back(QUOT_LastPrice);

	vecOptionCol.push_back(QUOT_OptionPrice);

	//vecOptionCol.push_back(nNewHead + QUOT_InstrumentID);
	vecOptionCol.push_back(nNewHead + QUOT_LastPrice);
	vecOptionCol.push_back(nNewHead + QUOT_ChangeValue);
	vecOptionCol.push_back(nNewHead + QUOT_ChangePercentage);
	vecOptionCol.push_back(nNewHead + QUOT_BidPrice1);
	vecOptionCol.push_back(nNewHead + QUOT_BidVolume1);
	vecOptionCol.push_back(nNewHead + QUOT_AskPrice1);
	vecOptionCol.push_back(nNewHead + QUOT_AskVolume1);

	LockGlobalObject();
	SetOptionShowCol(vecOptionCol);
	UnlockGlobalObject();
}

std::vector<int> g_vecTargetShowCol;

const std::vector<int>& GetTargetShowCol()
{
	return g_vecTargetShowCol;
}

void SetTargetShowCol(const std::vector<int>& vecCol)
{
	g_vecTargetShowCol = vecCol;
}

std::vector<int> g_vecOptionShowCol;

const std::vector<int>& GetOptionShowCol()
{
	return g_vecOptionShowCol;
}

void SetOptionShowCol(const std::vector<int>& vecCol)
{
	g_vecOptionShowCol = vecCol;
}

void SaveConfig()
{
	string strPath(GetXmlFile());
	CString strText;

	// ����һ��XML���ĵ�����
	TiXmlDocument *pXmlDocment = new TiXmlDocument(); 
	std::auto_ptr<TiXmlDocument> ptr(pXmlDocment);
	// ����һ����Ԫ�ز����ӡ�
	TiXmlElement *pRootElement = new TiXmlElement("root"); 
	pXmlDocment->LinkEndChild(pRootElement); 
	
	//����һ��NormalGridԪ�ز����ӡ�
	TiXmlElement *pNormalGridElement = new TiXmlElement("NormalGrid"); 
	pRootElement->LinkEndChild(pNormalGridElement); 

	for(int i=0; i<(int)g_vecTargetShowCol.size(); i++) {

		//����һ��NormalGridԪ�ز����ӡ�
		TiXmlElement *pColumnElement = new TiXmlElement("Column"); 
		pNormalGridElement->LinkEndChild(pColumnElement); 

		strText.Format("%d", g_vecTargetShowCol[i]);

		//����ColumnԪ�ص����ԡ�
		pColumnElement->SetAttribute("ID", strText); 
	}
    //���浽�ļ�
	pXmlDocment->SaveFile(strPath.c_str()); 

}

void LoadConfig()
{
	string strPath(GetXmlFile());

	TiXmlDocument* pXmlDocment = new TiXmlDocument(strPath.c_str());
	if(pXmlDocment == NULL) 
		return;

	std::auto_ptr<TiXmlDocument> ptr(pXmlDocment);
	if(!pXmlDocment->LoadFile()) 
		return;

	TiXmlElement *root = pXmlDocment->RootElement();
	if ( root == NULL ) 
		return;

	if( std::string(root->Value()) != "root")
		return;

	TiXmlElement *tList = root->FirstChildElement("NormalGrid");
	if(tList == NULL)
		return;

	std::vector<int> vecCol;

	for(TiXmlElement *ColumnItem = tList->FirstChildElement("Column");
			ColumnItem; 
			ColumnItem = ColumnItem->NextSiblingElement("Column")) {

		const char *pColumnid = ColumnItem->Attribute("ID");
		if(!pColumnid) 
			continue;

		if(atoi(pColumnid) != QUOT_InstrumentID)
			vecCol.push_back(atoi(pColumnid));

		//string strID = pColumnid;
		//const char *pColumnName = ColumnItem->Attribute("name");
		//if(!pColumnName)
		//	continue;
	}
	//delete pXmlDocment;

	if(vecCol.size() > 0) {
		std::vector<int> vecTargetCol;

		vecTargetCol.push_back(QUOT_InstrumentID);
		for(int i=0; i<(int)vecCol.size(); i++) {
			vecTargetCol.push_back(vecCol[i]);
		}

		std::vector<int> vecOptionCol;
		int nNewHead = QUOTNEWHEAD;

		for(int i=(int)vecCol.size()-1; i>=0; i--) {
			vecOptionCol.push_back(vecCol[i]);
		}
		vecOptionCol.push_back(QUOT_OptionPrice);
		for(int i=0; i<(int)vecCol.size(); i++) {
			vecOptionCol.push_back(nNewHead + vecCol[i]);
		}

		LockGlobalObject();
		SetTargetShowCol(vecTargetCol);
		SetOptionShowCol(vecOptionCol);
		UnlockGlobalObject();
	}

}