#pragma once


#include "CfgGlobal.h"

class ShortCutKeysMgr;

class OrderCfgMgr: public BaseCfgMgr
{
public:
	OrderCfgMgr(void);
	virtual ~OrderCfgMgr(void);

	bool InitCfg(TiXmlElement *root, bool bFromUserCfg = false );
	bool SaveCfg(TiXmlElement *root);
	void ClearCfg();

	LPORDER_CFG GetOrderCfg(){return &m_OrderCfg;}

	void CalcUsableShortCutKeys(ShortCutKeysMgr* pShortCutKeyMgr);
	void ProcessDulplicateShortCutKeys(ShortCutKeysMgr* pShortCutKeyMgr, bool bModify);

private:
	ORDER_CFG   m_OrderCfg;
};
