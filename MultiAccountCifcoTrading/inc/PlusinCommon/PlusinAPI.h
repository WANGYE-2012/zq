#ifndef PLUSINAPI_H
#define PLUSINAPI_H

#include "CommonDef.h"

typedef int (*MsgCallBackFunc)(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId);

class __declspec(dllexport) PlusinAPI
{
public:
	virtual ~PlusinAPI(){}

	/**
	 * @��������: ��ȡAPI�����ָ��
	 * @�� �� ֵ: API����ָ��
	**/
	static PlusinAPI* GetInstance();


	/**
	 * @��������: ����API����
	 * @�� �� ֵ: ��
	**/
	static void DestroyInstance();

	/**
	 * @��������: ��ʼ��API�����ñ�Ҫ����
	 * @�����б�: szAppID :			���Ψһ��ʶ
	 * @�����б�: szLicense :		���License
	 * @�����б�: msgCallBackFn :	���ڽ�����Ϣ�Ļص�����ָ��
	 * @�����б�: hTargetWnd :		FastTrader���㴰�ھ����������FastTrader���̷�����Ϣ
	 * @�����б�: nPos :				���������ʾ��λ��
	 * @�����б�: hAPPHWnd :			������㴰�ھ����û�ж��㴰�ھ��ʱ��ΪNULL
	 * @�� �� ֵ: ��
	**/
	virtual void Init(const char* szAppID, 
		const char* szLicense, 
		MsgCallBackFunc msgCallBackFn, 
		HWND hTargetWnd,
		int nPos = NEED_NO_POS_NO_BIND, 
		HWND hAPPHWnd = NULL) = 0;

	/**
	 * @��������: ��FastTrader���̷����¼����
	 * @�� �� ֵ: true����������ɹ� false����������ʧ��
	**/
	virtual bool Logon() = 0;

	/**
	 * @��������: ��FastTrader���̷�����Ϣ
	 * @�����б�: nCmdId :		��Ϣ������
	 * @�����б�: pData :		��Ϣ����ָ��
	 * @�����б�: nDataLen :		��Ϣ���ݳ���
	 * @�����б�: nRequestId :	�������
	 * @�� �� ֵ: true����Ϣ���ͳɹ� false����Ϣ����ʧ��
	**/
	virtual bool Send(int nCmdId, void* pData, int nDataLen, int nRequestId = 0) = 0;

	/**
	 * @��������: �˳���¼���ͷ���Դ
	 * @�� �� ֵ: ��
	**/
	virtual void Logout() = 0;
};

#endif