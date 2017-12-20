#pragma once

#include "CfgGlobal.h"

#include <string>
#include <vector>

class CInstrumentGroup
{
public:
	CInstrumentGroup(void);
	virtual ~CInstrumentGroup(void);

	bool IsExitGroup(const char *pInstrumentId);

	//��Ӻ�Լ
	void AddInstrument(const char *pInstrumentId);

	//ɾ����Լ
	void DelInstrument(const char *pInstrumentId);

	void ClearInstrument();

	//��ȡ��Ա����
	int  GetGroupMemberCount();

	std::string GetInstrumentId(int nIndex);  

	//��ȡ��Լ�б�
	std::vector<std::string>* GetGroupMember();

	LPINSTRUMENT_GROUP_CFG GetGroupInfo();

	std::string GetGroupName();
	int GetGroupId();
	int IsVisible();

	void SetGroupName(const char *name);
	void SetGroupId(DWORD id);
	void SetVisible( bool bVisible);

private:
	INSTRUMENT_GROUP_CFG	  m_InstrumentGroupCfg;
	std::vector<std::string>  m_InstrumentVector;
};
