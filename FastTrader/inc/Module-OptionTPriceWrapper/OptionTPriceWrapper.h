/***************************************************************************
*   @file         OptionTPriceWrapper.h
*   @brief        ��ȨT�ͱ��۰�װ��
*
********************************************************************************/

#ifndef _OPTIONTPRICEWRAPPER_H_
#define _OPTIONTPRICEWRAPPER_H_

#include <windows.h>
#include "wx/wxprec.h"
#ifndef WX_PRECOMP    
    #include <wx/wx.h>
#endif

#include <vector>
#include <string>
using std::vector;
using std::pair;
using std::string;

#include "../inc/ISvr/PlatformSvrMgr.h"


//**********************����TPriceList�еĽӿڷ���*************************
//�����µ�T�ͱ��۴��ڣ����ش��ھ��
typedef HWND (*CreateWnd_TPriceListFunc)(HWND hParent);
//����(��Ȩ��Լ-������)
typedef void (*SetOptionInstruments_TPriceListFunc)(const vector<PlatformStru_InstrumentInfo>& vecOptions);
//�����鵽��
typedef void (*NewMarketData_TPriceListFunc)(const PlatformStru_DepthMarketData& NewMD);
//ͨ��EventParamID��ȡ�������������ȡ��ɾ���� ClickMode: 0 ����/ 1 ˫��; szInstrument: �����Ȩ��Լ������; bDirection: ��/��
typedef bool (*Get_Del_ClickParam_TPriceListFunc)(int EventParamID, int& nClickMode, string& strInstrumentID, BOOL& bDirection);
//������ɫģʽ��0Ϊǳ��ɫ��ɫ��1Ϊ���ɫ��ɫ
typedef void (*SetColorMode_TPriceListFunc)(int colmode);


//**********************����OptionQuation�еĽӿڷ���*************************
//�������鴰��
typedef bool (*CreateQuationWnd_OptionQuationFunc)(HWND hParent);
//���ý�����
typedef void (*SetTradingDay_OptionQuationFunc)(std::string strTradingDay);
//������ʾ�ĺ�Լ
typedef void (*ShowInstrument_OptionQuationFunc)(std::string strInstrument);
//���ú�Լ����ʱ��
typedef bool (*SetTradingScales_OptionQuationFunc)(std::string strInstrument, const std::vector<std::pair<std::string,std::string>>& TradingTime);
//��ɳ�ʼ����Լ
typedef void (*InitFinish_OptionQuationFunc)(std::string strInstrument);


class COptionTPriceWrapper : public wxPanel
{
public:
	COptionTPriceWrapper(wxWindow *parent,
		wxWindowID id = GID_OptionTPrice,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxEmptyString);

    virtual ~COptionTPriceWrapper();

    DECLARE_EVENT_TABLE()

public:
	void OnSize(wxSizeEvent& event);
    //���۱��Լ�ı���Ϣ
    void OnInstrumentIDChangeInQuotPanel(wxCommandEvent& event);
    //����ص�
    static int CallBackFunc_BID_RtnDepthMarketData(const GUIModuleID GID,const AbstractBusinessData& data);
    //���鴦��
    void OnRtnDepthMarketData(wxCommandEvent& evt);
    //������ɫ����
    void OnDarkBackground(wxCommandEvent& evt);
    //������ɫ����
    void OnLightBackground(wxCommandEvent& evt);

    //�����ƵĴ�����Ϣ
	WXLRESULT MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam);

protected:
	void ShowKLine(std::string& strInstrument);

public:
    static COptionTPriceWrapper* m_pOptionTPriceWrapper;
    static bool m_bValid;
    static bool m_bValid_OptionQuation;

private:
    HINSTANCE m_hLib_TPriceList;
    HINSTANCE m_hLib_OptionQuation;

    CreateWnd_TPriceListFunc m_Method_CreateWnd;
    SetOptionInstruments_TPriceListFunc m_Method_SetOptionInstruments;
    NewMarketData_TPriceListFunc m_Method_NewMarketData;
    Get_Del_ClickParam_TPriceListFunc m_Method_Get_Del_ClickParam;
    SetColorMode_TPriceListFunc m_Method_SetColorMode;

	CreateQuationWnd_OptionQuationFunc	m_CreateQuationWnd_OptionQuationFunc;
	SetTradingDay_OptionQuationFunc		m_SetTradingDay_OptionQuationFunc;
	ShowInstrument_OptionQuationFunc	m_ShowInstrument_OptionQuationFunc;
	SetTradingScales_OptionQuationFunc	m_SetTradingScales_OptionQuationFunc;
	InitFinish_OptionQuationFunc		m_InitFinish_OptionQuationFunc;

    HWND m_hWnd_TPriceList;

    string  m_UnderlyingInstrumentID;

    int m_BackgroundMode;       //����ɫ������0:ǳɫ������1:��ɫ����


};


#endif