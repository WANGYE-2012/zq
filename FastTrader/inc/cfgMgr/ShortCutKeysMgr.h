#pragma once

#include <map>
#include <set>
#include <string>

#include "CfgGlobal.h"

class ShortCutKeysMgr: public BaseCfgMgr
{
public:
	ShortCutKeysMgr(void);
	virtual ~ShortCutKeysMgr(void);

	bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false );				
	bool SaveCfg(TiXmlElement *root);
	void ClearCfg();

public:
	std::map<int, std::string> GetShortCutKeyNameMap(){return m_KeyNameMap;}
	std::set<std::string> GetUsableShortCutKeys(){return m_usableShortCutKeySet;}

	void ClearUsableShortCutKeys();
	void AddUsableShortCutKey( const std::string& strKey );
	void DelUsableShortCutKey( const std::string& strKey );
	void InitUsableShortCutKey();
	bool IsShortKeyUsable( const std::string& strKey );

	void ClearUsedShortCutKeys();
	void CalcDulplicateShortCutKey( const std::string& strKey );
	bool IsShortKeyDulplicate( const std::string& strKey );
	std::string GetDulplicateKeyString();

private:
	void InitKeyNameMap();

private:
	std::map<int, std::string>				m_KeyNameMap;								//��ݼ� ��ֵ������ӳ���
	std::set<std::string>					m_usableShortCutKeySet;						//ʣ����õĿ�ݼ�
	std::set<std::string>					m_usedShortCutKeySet;						//�Ѿ�ʹ�õĿ�ݼ�
	std::set<std::string>					m_dulplicateKeys;							//�ظ��Ŀ�ݼ�
};
