// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����
#include <Afxcview.h>



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <vector>
#include <map>
#include <set>
#include "MyEdit.h"
#include "sizecbar.h"

#define MAX_PWD_LENGTH	16
#define MIN_PWD_LENGTH	6
#define MAX_ACCOUNT_LENGTH	16
#define MAX_TRUST_ACCOUNT_NAME_LENGTH	200
#define MIN_ACCOUNT_LENGTH	1
#define MAX_USER_NAME_LENGTH	20
#define MAX_CONTACTINFO_LENGTH	100
#define MAX_IDCARD_NO_LENGTH	20
#define MAX_PERMISSION_NAME_LENGTH	20
#define MAX_PHONE_NO_LENGTH	20
#define MAX_MSG_TITLE_LENGTH	20
#define MAX_MSG_CONTENT_LENGTH	1000

#define INVALID_ACOUNTID	-1
#define OUT_TIME_SEC	60
#define TIMER_LOGON_CHECK	1000
#define TIMER_MODIFY_PWD_CHECK	1001
#define TIMER_BROKER_INFO_CHECK	1002
#define TIMER_NET_AND_LOGON_CHECK	1003

#define MSG_SEARCH					WM_USER+501
#define MSG_LIST_SEL_CHANGE			WM_USER+502
#define MSG_MAIN_TAB_SEL_CHANGE		WM_USER+503
#define MSG_MODIFY_ACCOUNT			WM_USER+504
#define MSG_DEL_ACCOUNT				WM_USER+505
#define MSG_ADD_ACCOUNT				WM_USER+506
#define MSG_TARGETACCOUNT_CHANGE	WM_USER+507
#define MSG_SAVE_WARNING_SET		WM_USER+508

enum OperatorType
{
	OPERATOR_ADD,
	OPERATOR_MODIFY
};

enum AccountType
{
	ACCOUNT_TRUST_TRADE,
	ACCOUNT_MANAGE,
	ACCOUNT_TRADER
};

const char VALID_FOREVER_DATE[] = "9999-12-31";

const LPCTSTR lpszAccoutActive = _T("����");
const LPCTSTR lpszAccountStop = _T("δ����");
const LPCTSTR lpszStatusLogon = _T("�ѵ�¼");
const LPCTSTR lpszStatusLogout = _T("δ��¼");

const LPCTSTR lpszAccountError = _T("�˺ű���Ϊ1~16���ַ�");
const LPCTSTR lpszPasswordError = _T("�������Ϊ6~16���ַ�");
const LPCTSTR lpszOriPasswordError = _T("ԭ�����������");
const LPCTSTR lpszPasswordNotSame = _T("������������벻һ�£�");
const LPCTSTR lpszSamePassword = _T("�¾����벻��һ����");
const LPCTSTR lpszPasswordModified = _T("�������޸�!");
const LPCTSTR lpszEmptyPermission = _T("Ȩ�޲���Ϊ�գ�");
const LPCTSTR lpszEmptyOrg = _T("�����ʹܲ���Ϊ�գ�");
const LPCTSTR lpszEmptyAccountStatus = _T("�˺�״̬����Ϊ�գ�");
const LPCTSTR lpszEmptyBrokerCode = _T("���͹�˾���벻��Ϊ�գ�");

const LPCTSTR lpszQryBrokerError = _T("���������쳣�����󾭼͹�˾�б�ʧ�ܣ�");
const LPCTSTR lpszQryPermissionModuleError = _T("���������쳣����ȡȨ������ʧ�ܣ�");
const LPCTSTR lpszQryServerGroupError = _T("���������쳣��������������б�ʧ�ܣ�");
const LPCTSTR lpszQryError = _T("���������쳣������ʧ�ܣ�");
const LPCTSTR lpszLogonError = _T("���������쳣����¼ʧ�ܣ�");
const LPCTSTR lpszLogonOutTime = _T("��¼��ʱ��");
const LPCTSTR lpszOperatorOutTime = _T("������ʱ��");
const LPCTSTR lpszSelectOneItem = _T("��ѡ��һ�����ݣ�");
const LPCTSTR lpszQuit = _T("�Ƿ��˳�ϵͳ��");
const LPCTSTR lpszCanModifySelDataOnly = _T("ֻ���޸��Լ����������ݣ�");
const LPCTSTR lpszCanDelSelfDataOnly = _T("ֻ��ɾ���Լ����������ݣ�");
const LPCTSTR lpszChangePermissionTypeWarning = _T("����/���Ȩ��ֻ��ѡ��һ����[����]��\n����ȫ����֮ǰ���ݣ���ȷ�ϣ�");
const LPCTSTR lpszAddAccountSuccess = _T("�����û��ɹ�����ˢ���б�");
const LPCTSTR lpszModifyAccountSuccess = _T("�޸��û��ɹ�����ˢ���б�");
const LPCTSTR lpszDelAccountSuccess = _T("ɾ���û��ɹ�����ˢ���б�");
const LPCTSTR lpszModifyAccountStatusWarning = _T("�����û������ͣ�ã����ᱻ�����رմ��ڣ��Ƿ�ȷ�ϸò�����");
const LPCTSTR lpszDelDataComfirm = _T("ȷ��Ҫɾ�������ݣ�");
const LPCTSTR lpszDelDataCaseCadeComfirm = _T("�ò�����ɾ���˺ż���������������˺ţ����Ҳ��ɻָ����Ƿ������");
const LPCTSTR lpszDelBrokerComfirm = _T("ɾ�����͹�˾����ͬ��ɾ�����µķ�������͵�ַ����ȷ�ϣ�");

const LPCTSTR lpszPrompt = _T("��ʾ");
const LPCTSTR lpszAllAccount = _T("ȫ��");

const LPCTSTR lpszPromtAction[] = 
{
	_T("����"),
	_T("������"),
	_T("���ѡ�������"),
};

CString GetAccountFormatString(double dAmount, int nPrecision);

#pragma warning(disable:4996) 

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
