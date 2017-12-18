#pragma once

#include <string>
#include <map>

class CListCfg;
class TiXmlElement;

class ListCfgMgr: public BaseCfgMgr
{
public:
	ListCfgMgr(void);
	virtual ~ListCfgMgr(void);

	void ClearCfg();

	bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false );
	bool SaveCfg(TiXmlElement *root);

	//�õ��б�������Ϣ
	LPLIST_TABLE_CFG GetListCfg(int ModuleId);

	int	GetListColCount(int ModuleId);

	//�õ��б��е�������Ϣ
	LPLIST_COLUMN_CFG GetListColCfg(int ModuleId, int index);

	int	GetListVisibleColCount(int ModuleId);

	//�õ��б��е�������Ϣ
	LPLIST_COLUMN_CFG GetListVisibleColCfg(int ModuleId, int index);

	std::vector<LPLIST_COLUMN_CFG>* GetListColCfg(int ModuleId);

	void AddListCfgInfo();

	void SetListColCfg(int ModuleId, LPLIST_TABLE_CFG pList,
		std::map<std::string ,LPLIST_COLUMN_CFG> NewColCfgMap,int ColCount);

	void SetListColCfg(int ModuleId, LPLIST_TABLE_CFG pList,
		std::vector<LPLIST_COLUMN_CFG> NewColCfgVec);

	std::map<int, CListCfg*>* GetListCfgMap();

private:
	std::map<int, CListCfg*> m_ListCfgMap;

};
