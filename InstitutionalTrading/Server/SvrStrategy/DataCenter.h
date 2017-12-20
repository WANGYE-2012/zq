#pragma once
#include "stdafx.h"

struct STraderInstance
{
	STraderInstance()
	{
		memset(this,0,sizeof(*this));		
	};

	STraderInstance(const char* npsStrategy,const char* npsTraderID)
	{
		memset(this,0,sizeof(*this));

		if(npsStrategy != NULL && npsTraderID != NULL)
		{			
			//���ȳ���������
			strncpy_s(strTrader, sizeof(strTrader),npsTraderID, sizeof(strTrader)-1);
			strncpy_s(strStrategy, sizeof(strStrategy),npsStrategy, sizeof(strStrategy)-1);
		}
	};

	STraderInstance& operator= (const STraderInstance& other)
	{
		if (this == &other)
			return *this;

		memcpy((char*)this,(char*)&other,sizeof(*this));
		return *this;
	};

	bool operator< (const STraderInstance& r) const
	{
		int irlt;

		irlt=strncmp(strTrader,r.strTrader,sizeof(strTrader));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(strStrategy,r.strStrategy,sizeof(strStrategy));
		if(irlt<0) return true;
		if(irlt>0) return false;

		return false;
	}

	bool operator== (const STraderInstance& r) const
	{
		int irlt;

		irlt=strncmp(strTrader,r.strTrader,sizeof(strTrader));
		if(irlt!=0) return false;

		irlt=strncmp(strStrategy,r.strStrategy,sizeof(strStrategy));
		if(irlt!=0) return false;		

		return true;
	}

	bool operator> (const STraderInstance& r) const
	{
		return !((*this) < r || (*this) == r);
	}		
	
	char	strTrader[25];	//����ԱId
	char	strStrategy[25];	//���ײ�����
};
struct SUploadAllInfo
{
	std::string strID; //�ϴ���Ψһ��ID
	std::string strTraderID;//����ԱID
	std::vector<std::string> vecStrategy;//������������
	int  nStrategySuccess;				 //�����ϴ��ɹ���Ŀ
	std::vector<std::string> vecIndex;   //ָ����������
	int  nIndexSuccess;				 //ָ���ϴ��ɹ���Ŀ
	SUploadAllInfo()
	{
		nStrategySuccess = 0;
		nIndexSuccess	 = 0;
	}
};
class CDataCenter
{
public:
	CDataCenter(void);
	~CDataCenter(void);
private:
	std::map<std::string, SStrategy> m_mapStrategy;//���ݲ������ֲ�����Ӧ����
	CReadWriteLock	m_lockStrategy;

	std::map<STraderInstance, SStrategyInstance> m_mapStrategyInstance;//���ݲ������ֲ������в���ʵ��
	CReadWriteLock	m_lockInstance;

	std::map<std::string, SIndex> m_mapSIndex;//���ݲ������ֲ�����Ӧ����
	CReadWriteLock	m_lockSIndex;


	std::multimap<std::string, std::string> m_mapStrategy2Index;
	std::multimap<std::string, std::string> m_mapIndex2Strategy;
	CReadWriteLock	m_lockStrategyIndexRelation;
//////////////////////////////////////////////////////////////////////////
public:
	//���ݵ�����
	std::map<std::string, SStrategy> m_mapStrategyBack;
	CReadWriteLock	m_lockStrategyBack;	

	std::map<std::string, SIndex> m_mapSIndexBack;
	CReadWriteLock	m_lockSIndexBack;


	std::map<std::string, SUploadAllInfo>  m_mapTrader2uploadAllInfo;		//����Ҫ�ϴ��Ĳ��Ժ�ָ��;����Ա��Ӧ������
	CReadWriteLock	m_lockTrader2UploadAllInfo;		

	//�õ��ϴ�����
	bool GetUploadAllInfo(std::string strTraderID, SUploadAllInfo& sUploadAllInfo);

	//�ж��Ƿ�����ϴ�����
	bool IsUploadAllInfoExist(SUploadAllInfo& uploadAllInfo);

	//����һ���ϴ�����
	bool AddUploadAllInfo(SUploadAllInfo& uploadAllInfo);
	//ɾ��һ���ϴ����Ի���Ŀ��
	bool DelUploadAllInfo(SUploadAllInfo& uploadAllInfo);

	//��λԭ�е��ϴ�������Ϣ
	bool RollBackUploadInfo(SUploadAllInfo& uploadAllInfo);

	bool copyUploadFile(std::string& strsrx, std::string& strDest, std::string strDestSubPath, std::string filename, bool bCopyFromSrc);

	//nType ���Ի���ָ��0������ ��1��Ϊָ��
	//nSubType 0Ϊdll��1Ϊzip
	bool WriteFile2DB(std::string strFilename, std::string strName, int nType, int nSubType);

	//���ϴ�������м��� nType0Ϊdll��1Ϊzip
	bool calcUploadAllInfo(std::string strTraderID, std::string strName, int nType);


public:
	void Init();
	//����	
	bool AddStrategy(SStrategy& strategy);//����һ�����ԣ��Ѿ��е��򸲸�
	bool DelStrategy(std::string strName);//ɾ��һ�����ԣ�����״̬�²ſ���ɾ�����ԡ�
	bool SetStragegyStatus(std::string strName, int bStart);// bstart 0:ͣ�� 1:����	
	bool IsStrategyBelongToTrader(std::string strName, std::string strTrader);//�жϸò����ǲ��Ǹý���Ա��
	bool GetStrategy(std::string strName, SStrategy& strategy);

	bool GetAllUserPriveledgeStrategy(std::string strTrader, vector<SStrategy>& vecStrategy);

	//���Է���
	bool AddInstance(SStrategyInstance& strategy);//����һ�����Է������Ѿ��е��򸲸�
	bool DelInstance(std::string strName, std::string strTrader);//ɾ��һ�����Է�����
	bool SetInstanceStatus(std::string strName, std::string strTrader, bool bUse);// bUse 0:��ͣ 1:����
	bool IsInstanceBelongToTrader(std::string strName, std::string strTrader);//�жϸò��Է����ǲ��Ǹý���Ա��
	bool GetInstance(std::string strName, std::string strTrader, SStrategyInstance& Instance);
	bool GetAllInstance(std::map<STraderInstance, SStrategyInstance>& mapStrategyInstance);
	
	//ָ��
	bool AddIndex(SIndex& index);//����һ��ָ�ꣻ�Ѿ��е��򸲸�
	bool DelIndex(std::string strName);//ɾ��һ��ָ�ꣻ
	bool SetIndexStatus(std::string strName, int nPrivate);// bstart 0:ͣ�� 1:����	
	bool IsIndexBelongToTrader(std::string strName, std::string strTrader);//�жϸ�ָ���ǲ��Ǹý���Ա��
	bool GetIndex(std::string strName, SIndex& index);
	bool GetStrategyofIndex(std::string strName, std::vector<std::string>& vecStrategyName);//�õ��õ���ָ������в�����

	bool GetAllUserPriveledgeIndex(std::string strName, vector<SIndex>& vecIndex);
	
	//����ָ��Ĺ�ϵ
	bool DelStrategy2IndexRelation(std::string strategyName);//ɾ��������ָ��Ĺ�ϵ

	bool AddStrategy2IndexRelation(std::string strategyName, std::string strIndexName);//���Ӳ�������ָ��Ĺ�ϵ


	
	
};
