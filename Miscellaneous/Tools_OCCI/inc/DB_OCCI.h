#pragma once

#include <comutil.h>
#include <string>
#include <vector>
using std::string;
using std::vector;
#include "easymutex.h"
#include "EasyTime.h"


//������дʱ�����һ����Ļص������õ��ó����л���������һ������/��ȡ�������ݣ���ִ����һ������
//BatchNo: ��һ�����Σ����㿪ʼ��һ������n+1�Ρ�׼����дǰ��BatchNo==0�ȵ���һ�Σ�Ȼ��ÿ���һ������һ��
//ThisTimeRWCount: ������ɶ�д�ļ�¼����
//NextTimeRWCount: ��һ��Ҫ��д������
//UserData: �û������֣��ص�ʱ���ء����ڵ���������������
//�����Ƿ������һ����д��true: ִ����һ����д; false: ֹͣ��д
typedef bool (*BatchDBRWProcessFunc_OCCI)(int BatchNo,int ThisTimeRWCount,int& NextTimeRWCount,unsigned int UserData);

//������дʱ���ֶ����͡���oracle��Typeֵ��Ӧ������ֱ����setDataBuffer��ʹ��
enum FieldType_DB_OCCI
{
	eOCCIINT=3,
	eOCCI_SQLT_STR=5,
	eOCCIFLOAT=4,
};

class CDB_OCCI
{
public:
	CDB_OCCI(const string& username,const string& pw,const string& connstr);
	virtual ~CDB_OCCI(void);

	virtual void Init(void);
	virtual void Release(void);

	virtual void Connect(void);
	virtual void Disconn(void);

	/**
	* @��������: ִ�в�ѯSQL��䣬�����ؽ����
	* @�����б�: pSql :��ѯSQL���
	* @�����б�: rtnvec :���ؽ����
	* @�����б�: ErrCode/ErrMsg :���ش�����/������Ϣ
	* @�� �� ֵ: true,ִ�гɹ� false,ִ��ʧ��
	**/
	virtual bool QueryData( const char* pSql, std::vector<std::vector<_variant_t>>& rtnvec, int& ErrCode, string& ErrMsg );

	/**
	* @��������: ִ�и��¡�ɾ��SQL��䣬����Ӱ��ļ�¼����
	* @�����б�: pSql :SQL���
	* @�����б�: AffectedCount :��Ӱ��ļ�¼����
	* @�����б�: ErrCode/ErrMsg :������/������Ϣ
	* @�� �� ֵ: true,ִ�гɹ� false,ִ��ʧ��
	**/
	virtual bool ExcuteUpdate( const char* pSql, int& AffectedCount, int& ErrCode, string& ErrMsg );

	/**
	* @��������: ִ������SQL���
	* @�����б�: pSql :SQL���
	* @�����б�: ErrCode/ErrMsg :������/������Ϣ
	* @�� �� ֵ: true,ִ�гɹ� false,ִ��ʧ��
	**/
	virtual bool Excute( const char* pSql, int& ErrCode, string& ErrMsg );

	/**
	* @��������: ִ�в�ѯSQL��䣬���ز�ѯ������׼�¼�ĵ�һ���ֶε�����ֵ��һ���ѯselect count(*) from...
	* @�����б�: pSql :SQL���
	* @�����б�: FirstRtnValue: ��ѯ������׼�¼�ĵ�һ���ֶε�����ֵ
	* @�����б�: ErrCode/ErrMsg :������/������Ϣ
	* @�� �� ֵ: true,ִ�гɹ� false,ִ��ʧ��
	**/
	virtual bool ExcuteSelectCount( const char* pSql, int& FirstRtnValue, int& ErrCode, string& ErrMsg );

	/**
	* @��������: ����һ�����ݣ������ز����¼��ID
	* @�����б�: pSql :��������SQL���
	* @�����б�: pGetIDSql :��ѯ����sequnce id��SQL���
	* @�����б�: rtnID : ���ز����¼��ID
	* @�����б�: ErrCode/ErrMsg :������/������Ϣ
	* @�� �� ֵ: true,ִ�гɹ� false,ִ��ʧ��
	**/
	virtual bool InsertAndReturnID( const char* pSql, const char* pGetIDSql, int& rtnID, int& ErrCode, string& ErrMsg );

	/**
	* @��������: ִ��������ȡ����
	* @�����б�: bQuery : �Ƿ��Ƕ�ȡ��true��ʾ������ȡ��ִ��rset->getNumArrayRows��false��ʾ�������£�ִ��pSmt->executeArrayUpdate
	* @�����б�: pSql : ����SQL���
	* @�����б�: fieldnum : �ֶ�/����������
	* @�����б�: buffers : ����ָ�����飬Ӧ��������void* [fieldnum]
	* @�����б�: types : �ֶ��������顣Ӧ��������FieldType_DB_OCCI[fieldnum]
	* @�����б�: sizes : �ֶλ������ߴ����飬Ӧ����int[fieldnum]
	* @�����б�: lens :  �ֶ����ݳ������飬Ӧ����unsigned short*[fieldnum]��int����Ӧ����sizeof(int); �ַ�������Ӧ����strlen()����ֵ������NULL
	* @�����б�: cbfunc : ��д�����еĻص���������ʼ��дǰ��ÿ�ζ�д�󶼻�ִ�д˻ص�����
	* @�����б�: UserData: �û������֣��ص�ʱ���ء����ڵ���������������
	* @�����б�: ErrCode/ErrMsg :������/������Ϣ
	* @�� �� ֵ: true,ִ�гɹ� false,ִ��ʧ��
	**/
	virtual bool BatchQuery( const char* pSql, int fieldnum,void**buffers, FieldType_DB_OCCI* types,int* sizes, unsigned short** lens, BatchDBRWProcessFunc_OCCI cbfunc,unsigned int UserData,int& ErrCode, string& ErrMsg);

	/**
	* @��������: ִ��������������
	* @�����б�: pSql : д��SQL��䡣Ӧ�������ڣ�insert into dbname values(:v1,:v2,:v3,:v4,:v5,:v6,:v7,:v8,:v9,:v10)
	* @�����б�: fieldnum : ����������
	* @�����б�: buffers : ����ָ�����飬Ӧ��������void* [fieldnum]
	* @�����б�: types : �ֶ��������顣Ӧ��������FieldType_DB_OCCI[fieldnum]
	* @�����б�: sizes : �ֶλ������ߴ����飬Ӧ����int[fieldnum]
	* @�����б�: lens :  �ֶ����ݳ������飬Ӧ����unsigned short*[fieldnum]��int����Ӧ����sizeof(int); �ַ�������Ӧ����strlen()
	* @�����б�: cbfunc : ��д�����еĻص���������ʼ��дǰ��ÿ�ζ�д�󶼻�ִ�д˻ص�����
	* @�����б�: UserData: �û������֣��ص�ʱ���ء����ڵ���������������
	* @�����б�: ErrCode/ErrMsg :������/������Ϣ
	* @�� �� ֵ: true,ִ�гɹ� false,ִ��ʧ��
	**/
	virtual bool BatchUpdate( const char* pSql, int fieldnum,void**buffers, FieldType_DB_OCCI* types,int* sizes, unsigned short** lens, BatchDBRWProcessFunc_OCCI cbfunc,unsigned int UserData,int& ErrCode, string& ErrMsg);
	/**
	* @��������: ִ��������������
	* @�����б�: UpdateCount :  ��Ҫ���µļ�¼����
	**/
	virtual bool BatchUpdate( const char* pSql, int fieldnum,void**buffers, FieldType_DB_OCCI* types,int* sizes, unsigned short** lens, int UpdateCount,int& ErrCode, string& ErrMsg);

private:
	Ceasymutex m_mutex;

	void 			*m_pEnv;
	void 			*m_pConn;

	string 			m_username;
	string			m_pw;
	string			m_connstr;

	CEasyTime		m_TryInitTime;
	CEasyTime		m_ReleaseTime;
	CEasyTime		m_TryConnTime;
	CEasyTime		m_DisconnTime;



	void Connect_Internal(void);
	void Disconn_Internal(void);
	void CheckConn_Internal(void);
	
	bool CheckParam_Init_Internal(const char* pSql, int& ErrCode, string& ErrMsg );
	bool ReleaseStmt_Rset_Internal(void* pStmt,void* pRes);
	void Rollback_Internal();



	//������ֹ����
private:
	CDB_OCCI(const CDB_OCCI&);
	CDB_OCCI& operator= (const CDB_OCCI&);

};
