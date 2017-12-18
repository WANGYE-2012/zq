// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PLUSIN_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PLUSIN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once

#include <map>
#include <set>
#include <queue>
using std::string;
using std::map;

#include "../inc/PlusinCommon/CommonDef.h"
#include "../Module-Misc2/BusinessData.hpp"

typedef wxWindow* (*CreatePlugPane)(int nPos,wxString caption,wxString AppID);
typedef bool (*DeletePlugPane)(int npos, wxString AppID);

struct TargetMsg
{
	HWND hWnd;
	COPYDATASTRUCT msg;
};

struct PLUGWIN2
{
    //���������ʵ����Ϣ
    struct Stru_Instance
    {
        Stru_Instance()
        {
            memset(this,0,sizeof(*this));
        };

	    HWND PluginMsgHWnd;	            //���������Ϣ���ھ��
	    HWND PlusinTopHWnd;	            //������㴰�ھ��
	    HWND ContainerHWnd;	            //�������ھ��
	    DWORD dwLastUpdateTime;         //���һ������ʱ�䣬��λΪ΢��
    };

	PLUGWIN2()
	{
		ShowPos = NEED_NO_POS_NO_BIND;

		strPath = "";

		unMenuPos = 0;
		strMenuName = "";
		strSummary = "";
		strDesc = "";
	};
    PLUGWIN2(const PLUGWIN2& r)
    {
        *this=r;
    };
    PLUGWIN2& operator= (const PLUGWIN2& r)
    {
        if (this == &r)
            return *this;

        ShowPos=r.ShowPos;
        strPath=r.strPath;
        bMultiInstance=r.bMultiInstance;
        bNeedCurrentInstrumentID=r.bNeedCurrentInstrumentID;
        unMenuPos=r.unMenuPos;
        strMenuName=r.strMenuName;
        strSummary=r.strSummary;
        strDesc=r.strDesc;
        m_Instance=r.m_Instance;

        return *this;
    };

	UINT ShowPos;

	string strPath;

    bool bMultiInstance;                //�Ƿ���Դ򿪶��ʵ��
    bool bNeedCurrentInstrumentID;      //����ʱ�Ƿ���Ҫ����ǰ��ԼID��Ϊ����

	UINT unMenuPos;			            //�˵���ʾλ��
	string strMenuName;                 //�˵���ʾ����
	string strSummary;	                //��Ҫ˵��
	string strDesc;	                    //��ϸ˵��

    map<HWND,Stru_Instance> m_Instance; //��ǰʵ��map, PluginMsgHWnd -> Instance

};

//����˵�λ�ö��壬��λ����
#define MENU_POS_NONE		0	//�������Ҫ�˵�
#define MENU_POS_MAIN_MENU	1	//���˵�λ��
#define MENU_POS_QUOTE		2	//���۱��Ҽ��˵�λ��

class PlusInMgrDlg;
// This class is exported from the Plusin.dll
class CPlusin 
{
public:
	//Ĭ�Ϲ��캯��
	CPlusin(void);
	//Ĭ����������
	~CPlusin(void);

public:
	//�������ʼ�������ϵͳ���
	void LoadPlusin();

	//��Ӧ������˵������������ʾ�Ի���
	void ShowPlusinDlg(wxWindow* pParent);
	//��ʼ�����ģ��
	bool Init( wxWindow* pParent, CreatePlugPane createFunc, DeletePlugPane deleteFunc );

	void SubscribeBusinessData();

	//ж�ز��ģ��
	void UnInit();

	void UnSubscribeBusinessData();

	//������������ȡ��WM_COPYDATA��Ϣ
	BOOL ProcessMessage( WPARAM wParam, COPYDATASTRUCT* pCopyDataStruct );
	//ѭ����鲢�ͷ��Ѳ����ڵĲ��
	void ReleasePlusInRC();
	//�ر��ɴ��ھ��ָ���Ĳ��
	void ClosePlusInProc( HWND hWnd );

	bool RegisterPlusin( const wxString& strPlusinPath, bool bSendMenuChangeMsg );
	//�ر���APPIDָ���Ĳ��
	bool UnRegisterPlusin( const wxString& strAppID );

	//GUI��ģ����������ָ����Ϣ��hPlusinTopWndΪNULLʱ����ʾ�����в��Ⱥ����Ϣ����ΪNULLʱ����ʾ��ָ�����������Ϣ
	bool Send(COPYDATASTRUCT msg, HWND hPlusinTopWnd = NULL);

	void SetOrderModuleCurInstrumentID( const wxString& strInstrumentID);

	bool GetPlusinByAppID( const wxString& strAppID, PLUGWIN2& plusin);

	//��ȡ����˵���Ϣ
    void GetPlusinMenuInfo( UINT nMenuPos,std::vector<wxString>& AppIDs );
	//�������
	void ShowPlusin( const wxString& strAppID );

	bool GetOrderVerifyStatus();
	bool SendNeedVerifyOrder(PlatformStru_InputOrder& Order);

protected:
	//���鵽��ʱ�Ļص�����
	static int CallBackFunc_BID_RtnDepthMarketData(const GUIModuleID GID,const AbstractBusinessData& data);
	//ί�е�״̬�����Ļص�����
	static int CallBackFunc_BID_RtnOrder(const GUIModuleID GID,const AbstractBusinessData& data);
	//�ɽ��ر������Ļص�����
	static int CallBackFunc_BID_RtnTrade(const GUIModuleID GID,const AbstractBusinessData& data);
	//��ѯ�ֲ���Ӧ�Ļص�����
	static int CallBackFunc_BID_RspQryInvestorPosition(const GUIModuleID GID,const AbstractBusinessData& data);
	//��ѯ��ϳֲ���Ӧ�Ļص�����
	static int CallBackFunc_BID_RspQryInvestorComboPositionDetail(const GUIModuleID GID,const AbstractBusinessData& data);
	//������ѯ�ر�
	static int CallBackFunc_BID_RspQryOrder(const GUIModuleID GID,const AbstractBusinessData& data);
	//�ʽ��˻���ѯ�ر�
	static int CallBackFunc_BID_RspQryTradingAccount(const GUIModuleID GID,const AbstractBusinessData& data);
	//������ִ1
	static int RspOrderAction1CallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	 
	//������ִ2
	static int RspOrderAction2CallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	 
	//�µ������ִ1
	static int RspOrderInsert1CallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	 
	//�µ������ִ2
	static int RspOrderInsert2CallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	 
    //�����߳�
	static DWORD SendThreadFunc(void* pData);

	bool PushIntoMsgQueue(const TargetMsg& targetMsg, int nRequestId);
	void SaveUserPlusIn();
	void CloseAllPlusin();
	// TODO: add your methods here.
	void ReleasePlusInRC(int nPos, const wxString& strAppID );

	BOOL ProcessConsoleMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessConsoleLogon( HWND hWnd, void* pData, int nDataLen, int nRequestId );

	BOOL ProcessAccountMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessInstrumentMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessAllOrderMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessPositionMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessExchangeMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessQuoteMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessOrderMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessConfigMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessInsertOrderVerifyMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );
	BOOL ProcessOtherMsg( HWND hWnd, int nCMDID, void* pData, int nDataLen, int nRequestId );

private:
    //��һ�����̣�����ProcessID��ʧ��ʱ����0
    //�����в���Ϊ��FastTrader�������ھ�� Param
    DWORD OpenPlusInProc( const wxString& strPath, const string& Param );

private:
	CreatePlugPane m_pFnCreatePlusPanel;
	DeletePlugPane m_pFnDeletePlusPanel;

	std::map<wxString, PLUGWIN2> m_mapAppID2Plusin;
	PlusInMgrDlg* m_pPlusInDlg;
	std::set<std::string> m_setSysPlusIn;
	HWND m_pParentHWnd;
	bool m_bInit;
	HANDLE m_hSendThreadHandle;
	bool m_bSendThreadExit;

	std::queue<TargetMsg> m_msgQueue;
	CRITICAL_SECTION m_cs;
	//���ĺ�Լ���ݼ�
	typedef std::map<std::string, std::set<HWND>> QUOTE_SUBSCRIBE_MAP;
	QUOTE_SUBSCRIBE_MAP m_quoteSubscribe;

	wxString m_strOrderModuleCurInstrumentID;
	std::set<DWORD> m_setProcessIDWaitForShow;

	bool m_bOrderVerify;	//�µ��Ƿ���Ҫ���
	HWND m_hMsgWnd_OrderVerify;	//��˲������Ϣ���ھ��
};

