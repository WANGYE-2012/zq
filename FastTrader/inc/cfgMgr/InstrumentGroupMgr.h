#pragma once

class CInstrumentGroup;

class InstrumentGroupMgr
{
public:
	InstrumentGroupMgr(void);
	virtual ~InstrumentGroupMgr(void);

	//��ȡ�����ļ�������LIST ID���Map
	bool InitCfg();
	bool SaveCfg();
	void ClearCfg();

	//��ȡ��Լ�����
	int	 GetGroupCount();

	LPINSTRUMENT_GROUP_CFG GetGroupInfo(int GroupIndex);

	std::string GetGroupName(int GroupIndex);
	int  GetGroupMemberCount(int GroupIndex);

	//��ȡ��Լ���Ա����
	std::string GetGroupInstrumentId(int GroupIndex, int InstrumentIndex);

	//��ȡ��Լ���к�Լ��ID����˳���ȡ��
	std::vector<std::string>* GetGroupMember(int GroupIndex);
	std::vector<std::string>* GetCurrentGroupMember();

	bool IsExitGroup(const std::string strGroupName);
	bool IsExitGroup( int nGroupId );

	int GetGroupIndexByGroupId( int nGroupId );

	//��Ӻ�Լ�飬���غ�Լ��index
	int AddInstrumentGroup(std::string strName, bool bVisible, DWORD id);

	int ReplaceInstrumentGroup( int nGroupIndex, const char * szName,
		bool bVisible, DWORD id = -1 );

	//ͬʱɾ�����Ա
	bool DelInstrumentGroup(const char *name); 
	bool DelInstrumentGroup( int nGroupId );

	//��Ӻ�Լ
	void AddInstrument(int GroupIndex, const char *pInstrumentId);					

	//ɾ����Լ�����к�Լ
	void DelInstrument(int GroupIndex);

	//ɾ����Լ
	void DelInstrument(int GroupIndex, const char *pInstrumentId);

	int GetCurrentId( );
	std::string GetCurrentGroupName();
	void SetCurrentId( int id );

	void ClearInstrumentGroupVector();

	int GetNextGroupId();

private:
	DWORD m_dwCurrentGroupId;
	DWORD m_nMaxGroupId;
	std::vector<CInstrumentGroup*> m_InstrumentGroupVector;
};
