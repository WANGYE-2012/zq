#pragma once
#include <list>
#include <string>
#include <map>
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "CommonStruct.h"

using namespace std;

#define  NULL_VALUE "��"
class CStrategyData
{
public:
    CStrategyData();
	virtual ~CStrategyData();

	/*����һ�����Բ���*/
	void AddStrategyPara(StrategyPara& data);
	/*ɾ��һ�����Բ���*/
	void DeleteStrategyPara( const string& name);
	/*����һ������ָ��*/
	void AddStrategyIndicator(const string& Strategyname,StrategyIndicator& data);
    /*ͨ������ָ�������һ������ָ��*/
	bool GetStrategyIndicator(const string& szName,StrategyIndicator& data);
	/*ɾ��һ������ָ��*/
	void DeleteStrategyIndicator(const string& Strategyname,const string& Indicatorname);
	/*���ò��Բ�����ѡ��������ʱ��ѡ��*/
	void SetParaSubItem(const string&szTitle,list<StrategyPrarSubItem>& ListPrarSubItem);
	/*�õ����Բ�����ѡ��������ʱ��ѡ��*/
	void GetParaSubItem( map<string,list<StrategyPrarSubItem>>& PrarSubItem);
	void GetSubItem(const string name,list<StrategyPrarSubItem>& listSubItem);
	bool CheckStrategyParaName(const std::string& name);
	void SetParaType2NameMap(multimap<string,string>& data);
	void GetParaType2NameMap(multimap<string,string>& data);
	/*�õ����в��Բ���map*/
	void GetStrategyPara(std::map<std::string,StrategyPara>& data);
	std::map<std::string,StrategyPara>& GetStrategyParams();
	/*�õ��½����Ե�����ָ������*/
	void GetAllStrategyIndicator(std::map<std::string,StrategyIndicator>& lMapFiled);
	const string GetStrategyName();
	void SetStrategyName(const string name);
	const string GetStrategyNickName();
	void SetStrategyNickName(const string name);
	const string GetStrategyVersion();
	void SetStrategyVersion(const string Version);
	void  SetStrategyPrivate(const int data);
	int  GetStrategyPrivate();
	void GetIndicator(list<string>& data);

	bool IsOwner(const string& strCurrUserID);
	void SetOwner(const string& strName);
	string GetOwner();

	bool HasUpload();
	void ResetUpdoad(bool bHas);

	int GetServerUseStatus();
	void SetServerUseStatus(int nStatus);

	void SetDescribe(const string& strDescribe);
	string GetDescribe();

	int GetInstrumentCount();
	void SetInstrumentCount(int nInstrumentCount);


protected:

	string		m_strategyName;		//������	
	string		m_strategyNickName;	//����������
	string		m_Version;			//���԰汾��
	string		m_strDescribe;	   //��������
	int			m_nPrivate;			//1:˽�У�2����
	string			m_strOwner;			//���Ե�ӵ����
	bool			m_bHasUpload;		//�Ƿ����ϴ���
	int				m_nServerUseStatus;	//������ָ���������״̬ 1 ���ã�2 ����
	int				m_nInstrumentCount;//���Եĺ�Լ����


	std::map<std::string,StrategyPara>                                   m_mapStrategyPara;//���Բ���map
	multimap<string,string>                                              m_multimapstrategyParaType2Name;//���Բ������ͺͲ��Բ�������ӳ��
	std::map<std::string,StrategyIndicator>                              m_mapStrategyIndicator;//���Ե�ָ������MAP
	map<string,string>                                                   m_mapIndicator;//����ָ�����Ƶ�ָ��ӳ�� һ�������õ����ļ���ָ��
	map<string,list<StrategyPrarSubItem>>                                m_mapPrarSubItem; //�½����Բ������õ�ѡ��������ʱ��ѡ��



};
