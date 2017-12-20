#pragma once
#include "DataCenter.h"
class CManage
{
public:
	CManage(void);
	~CManage(void);
	void Init();
	void StartAllInstance();
	bool loadXMLString(SStrategyInstance& instance, std::string& strXML);
	void StopAllInstance();//ֹͣ���е�ʵ���������ʱ����Ҫ
    void HandleAllInstanceTimer(bool bStop);//ֹͣ���е�ʵ���ļ�ʱ����������Ļ��������ӶϿ�ʱ����Ҫ

	std::map<STraderInstance, HANDLE>  m_mapInstance2Handle;//���Է����ͽ���֮��Ĺ�ϵ
	CReadWriteLock	m_lockInstance2Handle;

	void string_replace( std::string &strBig, const std::string &strsrc, const std::string &strdst );
private:
	CDataCenter m_DataCenter;
public:
	bool UploadStartAll(UploadStart& start, int& nError, std::string& strError);
	bool UploadEndAll(UploadEnd& end, int& nError);

public:
	bool UploadBaseStrategy(SStrategy& strategy, int& nError);
	bool ParsePara(std::string strPara, std::vector<SStrategy2Index>& vec);//�����ϴ���������xml������Ժ�ָ��Ĺ�ϵ
	bool UploadDllFile(std::string strName, std::string strUploader, void* pData, int nLength, int& nError);
	bool UploadZIPFile(std::string strName, std::string strUploader, void* pData, int nLength, int& nError);
	bool AddStrategyIndexRelation(std::string strName, std::string strIndex, int& nError);

	//�������в��Ի�����Ϣ
	bool DownloadAllBaseStategy(std::string strUploader, std::vector<SStrategy>& vecIndex, int& nError);
	bool DownloadBaseStrategy(std::string strName/*, std::string strUploader*/, SStrategy& strategy, int& nError);
	bool DownloadDllFile(std::string strName, std::string strUploader, void*& pData, int& nLength, int& nError);
	bool DownloadZIPFile(std::string strName, std::string strUploader, void*& pData, int& nLength, int& nError);

	
	//�ϴ�ָ�������Ϣ
	bool UploadBaseIndex(SIndex& Index, int& nError);
	//�ϴ�ָ���ļ�dll
	bool UploadIndexFile(std::string strName, std::string strUploader, int nType, void* pData, int nLength, int& nError);

	//����ָ�������Ϣ
	bool DownloadBaseIndex(std::string strName/*, std::string strUploader*/, SIndex& index, int& nError);
	//����ָ���ļ�dll
	bool DownloadIndexFile(std::string strName, std::string strUploader, int nType, void*& pData, int& nLength, int& nError);

	//��������ָ�������Ϣ
	bool DownloadAllBaseIndex(std::string strUploader, std::vector<SIndex>& vecIndex, int& nError);



	//����ͣ�ò���
	bool UseStrategy(SUseStrategy& useStrategy, int& nError);
	//ɾ������
	bool DelStrategy(SDelStrategy& sDel, int& nError);


	//�������в��Է�����Ϣ
	bool DownloadAllInstance(std::string strUploader,std::vector<SStrategyInstance>& vecInstance, int& nError);

	//���Ӳ��Է���
	bool AddInstance(SStrategyInstance& strategy, int& nErrorCode);

	//�޸Ĳ��Է���
	bool ModifyInstance(SStrategyInstance& strategy, int& nErrorCode);

	//ɾ��ĳ����Ա��صĲ��Է���
	bool DelInstance_ByTrader(SDelStrategy& sDel, int& nErrorCode);
	//����ͣ�ò��Է���
	bool UseInstance(SUseStrategy& useStrategy, int& nError);

	//����Ա�������߿��Ʒ����Ƿ�����
	bool OnOffLineProcessInstance(std::string strTrader, bool bOnline);//bOnline true:���ߣ� false ����

	bool BeginSStrategyInstanceProcess(SStrategyInstance& instance);

	void SendMessage2Trader(std::string strUserName, MessageInfo& MsgInfo);

	//�˳�ĳ����Ա��ĳ�����Է���
	bool TerminatInstanceThread(std::string strName, std::string strUploader);

	//ָ����߲������������е�
	bool HasStrategyRun(std::vector<std::string>& vecStrategy, std::vector<std::string>& vecIndex, std::string strTrader,int& nError, std::string& strError);
};
