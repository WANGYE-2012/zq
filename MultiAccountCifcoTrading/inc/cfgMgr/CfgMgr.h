// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CFGMGR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CFGMGR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include "CfgGlobal.h"

#include "cfgmgr/ListCfgMgr.h"
#include "cfgmgr/PanelSetMgr.h"
#include "cfgMgr/AccountGroup.h"
#include "cfgMgr/OrderStrategyRowCfg.h"

// This class is exported from the CfgMgr.dll


class ShortCutKeysMgr;
class InstrumentGroupMgr;
class ListCfgMgr;
class RiskCfgMgr;
class OrderCfgMgr;
class CancelOrderCfgMgr;
class MainWindowCfgMgr;
class FundAccountCfgMgr;
class PanelSetMgr;
class VarietyMgr;
class NumberShortKeyMgr;
class SystemParamMgr;
class MovePositionMgr;
class ReplacePositionMgr;
class TiXmlDocument;
class LanguageMgr;
class GUIShowSetMgr;
class AccountGroupMgr;
class MultiAccountMgr;
enum CFG_STYLE;

class CfgMgr 
{
public:
	//��ȡ��ʵ��
	static CfgMgr * GetInstance();

	//���ٵ�ʵ��
	static void DestroyInstance();

	//���������ļ�
	bool Save();
	//��ȡ�����ļ�
	//1��ʾ�ɹ�
	//0��ʾ��ȡsyscfg��usercfgʧ��
	//-1��ʾ��ȡlanguage.csvʧ��
	int Init();

	//���¶�ȡ�����ļ�
	int ReInit();

public:
	//��ȡ��ҳ��������Ϣ
	LPMAIN_WINDOW_CFG GetMainWindowCfg();

	//��ȡָ���б�������������Ϣ
	LPLIST_TABLE_CFG GetListCfg( int nModuleId );

	//��ȡָ���б�������
	int	GetListColCount( int nModuleId );

	//��ȡ�б��е�������Ϣ
	LPLIST_COLUMN_CFG GetListColCfg( int nModuleId, int nIndex );

	//��ȡָ���б�ɼ�������
	int	GetListVisibleColCount( int nModuleId );

	//��ȡָ���б�ɼ��е�������Ϣ
	LPLIST_COLUMN_CFG GetListVisibleColCfg( int nModuleId, int nIndex );
	//��ȡָ���б���ı�
	std::string GetColCfgCaption( int nModuleId, int ColumnId);
	//��ȡָ���б�������������Ϣ
	std::vector<LPLIST_COLUMN_CFG>* GetListColCfg( int nModuleId );

	//�����б��е�������Ϣ
	void SetListColCfg(int nModuleId, LPLIST_TABLE_CFG pList,
		std::map<std::string ,LPLIST_COLUMN_CFG> newColCfgMap,int nColCount);

	//�����б��е�������Ϣ
	void SetListColCfg(int nModuleId, LPLIST_TABLE_CFG pList,
		std::vector<LPLIST_COLUMN_CFG> newColCfgVec);

	//��ȡ��Լ�����
	int	GetInstrumentGroupCount();
	int GetNextGroupId();

	//��ȡ��Լ����Ϣ
	LPINSTRUMENT_GROUP_CFG GetInstrumentGroupInfo(int GroupIndex);
	//��ȡ��Լ���Ա����
	int	GetInstrumentGroupMemberCount(int GroupIndex);
	bool IsExitGroup(const char *pGroupName);
	bool DelInstrumentGroup(const char *name);
	void DelInstrument( int nGroupId, const char *pInstrumentId );
	//��ȡ��Լ���к�Լ��ID����˳���ȡ��
	std::string	GetInstrumentId(int GroupIndex, int InstrumentIndex);
	void ClearInstrumentGroupVector();

	void AddInstrumentGroup(std::string name, bool bVisible, DWORD id);
	void AddInstrument(int GroupIndex, const char *pInstrumentId);

	int GetCurrentGruopId( );
	std::string GetCurrentGroupName();
	void SetCurrentGruopId( int id );

	std::vector<std::string> *  GetGroupMember(int GroupIndex);
	std::vector<std::string> *  GetCurrentGroupMember();
	std::string GetGroupName(int GroupIndex);

	//�µ�������Ϣ
	LPORDER_CFG	GetOrderCfg();

	//����������Ϣ
	CANCEL_ORDER_CFG  GetCancelOrderCfg();
	void SetCancelOrderCfg( const CANCEL_ORDER_CFG& cancelOrderCfg );

	//�ʽ�������Ϣ
	FUND_ACCOUNT_CFG GetFundAccountCfg();
	void SetFundAccountCfg( const FUND_ACCOUNT_CFG& fundAccountCfg );

	//���չ���������Ϣ
	LPRISK_MGR_CFG		GetRiskMgrCfg();
	
	//��ȡ��ݼ���Ϣ
	std::map<int, std::string> GetShortCutKeyNameMap();
	std::set<std::string> GetUsableShortCutKeys();
	void AddUsableShortCutKey( const std::string& strKey );
	void DelUsableShortCutKey( const std::string& strKey );
	bool IsShortKeyUsable( const std::string& strKey );
	//������õĿ�ݼ�
	void CalcUsableShortCutKeys();
	void ClearUsableShortCutKeys();
	std::string GetDulplicateKeyString();

	std::map<int, CListCfg*>* GetListCfgMap();
	std::vector< PanelInfo>* GetPanelSetVector();
	std::vector< VarietyInfo>* GetVarietyInfoVector();
	std::vector< NumberShortKey>* GetNumberShortKeyVector();
	SystemParamInfo* GetSystemParamInfo(bool bDefault=false);

	MOVEPOSITIONCFG GetMovePositionCfg();
	void SetMovePositionCfg( const MOVEPOSITIONCFG& movePosition );

	REPLACEPOSITION GetReplacePositionCfg();
	void SetReplacePositionCfg( const REPLACEPOSITION& replacePosition );

	const char* GetGroupText();
	void ApplyGroupText( const char* szBuffer );
	std::string LoadString(std::string id,int langid=-1);
	std::string GetFmtString(std::string pStr);
	std::string LoadFormatString(std::string id,std::string fmt,int langid=-1);
	std::string ReplaceFmtString(std::string str);

	int GetPosX();
	int GetPosY();
	void SetPos(int nPosX, int nPosY);
	int GetWidth();
	int GetHeight();
	void SetSize(int nWidth, int nHeight);
	int GetStatus();
	void SetStatus(int nStatus);

	//�û�������
	void ClearAccoutGroup();
	bool IsAccountGroupExist( int nGroupID );
	int GetNextAccountGroupID();
	bool AddAccountGroup( int nGroupID, std::string strGroupName );
	bool AddAccount( int nGroupID, int nAccountID, std::string strAccountName );
	bool DelAccount( int nAccountID );
	void GetAccountGroup(std::vector<AccountGroup>& accountGroup);
	void SetAccountGroup( const std::vector<AccountGroup>& accountGroup);
	void SetCurAccountGroupID( int nGroupID );
	int GetCurAccountGroupID();

	MULTIACCOUNT* GetMultiAccountCfg();
	void GetContracts( OrderStrategy eStrategy, std::vector<std::string>& vContracts);
	bool GetOrderStrategyData(OrderStrategy eStrategy, const std::string strAccount, OrderStrategyRowCfg& rowCfg );
	void SetOrderStrategyData(OrderStrategy eStrategy, const std::vector<std::string>& vContracts,
		const std::vector<OrderStrategyRowCfg>& vStrategyData);
	int GetOrderStrategyContractVolume(OrderStrategy eStrategy, const std::string strAccount,
		const std::string strContract, const std::string strContractType);

#ifdef _USE_MULTI_LANGUAGE
public:
	void SetLanguageId(int id){m_languageId=id;}
private:
	int m_languageId;
#endif
private:
	std::string GetEN(std::string id);
	std::string GetCN(std::string id);
	std::string GetHK(std::string id);

	CfgMgr(void);
	~CfgMgr(void);
	void CreateCfgManagers();
	void ReleaseCfgManegers();
	bool InitFromCFG( std::string strFilePath, bool bFromUserCfg = false );
	void ClearCfg();

private:
	static CfgMgr*		m_pInstance;
	ShortCutKeysMgr*	m_pShortCutKeysMgr;
	InstrumentGroupMgr*	m_pInstrumentGroupMgr;
	ListCfgMgr*			m_pListCfgMgr;
	RiskCfgMgr*			m_pRiskCfgMgr;
	OrderCfgMgr*		m_pOrderCfgMgr;
	CancelOrderCfgMgr*	m_pCancelOrderCfgMgr;
	MainWindowCfgMgr*	m_pMainWindowCfgMgr;
	FundAccountCfgMgr*	m_pFundAccountCfgMgr;
	PanelSetMgr*		m_pPanelSetMgr;
	VarietyMgr*			m_pVarietyMgr;
	NumberShortKeyMgr*	m_pNumberShortKeyMgr;
	SystemParamMgr*		m_pSystemParamMgr;
	MovePositionMgr*	m_pMovePositionMgr;
	ReplacePositionMgr* m_pReplacePositionMgr;
	LanguageMgr*		m_pLanguageMgr;
	GUIShowSetMgr*		m_pGUIShowSetMgr;
	AccountGroupMgr*	m_pAccountGroupMgr;
	MultiAccountMgr*	m_pMultiAccountMgr;

	bool				m_bInit;
};

#define CFG_MGR_DEFAULT() (CfgMgr::GetInstance())
#define CFG_MGR_DEFAULT_REINITCFG()  (CfgMgr::GetInstance()->ReInit())
