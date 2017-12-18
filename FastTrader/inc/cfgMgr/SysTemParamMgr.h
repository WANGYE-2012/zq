#pragma once 

#include "CfgGlobal.h"

class ShortCutKeysMgr;
class SystemParamMgr: public BaseCfgMgr
{
public:
	SystemParamMgr(void);
	virtual ~SystemParamMgr(void);

	bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false );
	bool SaveCfg(TiXmlElement *root);
	void ClearCfg();

	void CalcUsableShortCutKeys(ShortCutKeysMgr* pShortCutKeyMgr);
	void ProcessDulplicateShortCutKeys(ShortCutKeysMgr* pShortCutKeyMgr, bool bModify);

	SystemParamInfo m_SystemParamInfo;
	//���ڻָ�ϵͳĬ��
    SystemParamInfo m_DefaultInfo;
};