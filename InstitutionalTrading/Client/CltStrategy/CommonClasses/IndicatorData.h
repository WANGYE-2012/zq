#pragma once

#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "CommonStruct.h"
#include <string>
#include <map>

class CIndicatorData
{
public:
    CIndicatorData();
	virtual ~CIndicatorData();

public:
	static CIndicatorData* NewInstance();
	static void DestroyInstance();
	static CIndicatorData* m_pInstance;

	/*�½�ָ������������ʱ���������Ƿ��ظ�*/
	bool CheckName(const std::string& name);

	void AddIndcatorPara(IndicatorPara& data);
	void DeleteIndcatorPara( const std::string& name);
	
	/*�½�ָ��ʱ���ָ�����MAP*/
	void ClearIndiPara();
	/*�õ�һ��ָ������в���*/
	void GetIndiPara(std::map<std::string, IndicatorPara> &  lMapFiled);
	const std::string GetIndicatorName();
	void SetIndicatorName(const std::string name);
	std::string GetIndicatorDescribe();
	void SetIndicatorDescribe(const std::string data);
	void SetIndicatorType( IndicatorType data);
	IndicatorType GetIndicatorType();
	void SetIndicatorFile(const std::string data);
	std::string GetIndicatorFile();
	void SetIndicatorPrivate(const int data);
	int GetIndicatorPrivate();

	bool IsOwner(const std::string& strCurrUserID);
	void SetOwner(const std::string& strName);
	std::string GetOwner();

	bool HasUpload();
	void ResetUpdoad(bool bHas);

	int GetServerUseStatus();
	void SetServerUseStatus(int nStatus);
    
protected:

	std::string				m_szName;			//ָ������
	IndicatorType			m_eIndiType;		// ָ������
	std::string				m_szIndiFile;		//ָ���ļ���
	std::string				m_szDescribe;		//ָ������
	int						m_nPrivate;			//1:˽�У�2����
	std::string				m_strOwner;			//ָ���ӵ����
	bool					m_bHasUpload;		//�Ƿ����ϴ���
	int						m_nServerUseStatus;	//������ָ���������״̬

	std::map<std::string, IndicatorPara>	m_mapIndcatorPara;//ָ���������ʹ�õĲ���MAP,<����������������>

};
