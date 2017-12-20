//  Name: �ֲ�ģ��
//  Desc:
//          1. ����ֲ�ģ�����ʾ
//          2. ���Ҽ��˵�������ͨ�����㴰�ڴ��ݸ��µ�ģ��
//  Interface:
//          �ײ�ģ��platformSvr
//              1. ����BID_RspQryInvestorPositionDetail
//              2. ����BID_RspQryInstrumentMarginRate
//              3. ����BID_RtnTrade
//              4. ����BID_RtnOrder
//              5. ����BID_RtnDepthMarketData
//              6. DEFAULT_SVR()->GetPositionDetails
//              7. DEFAULT_SVR()->GetCurPrice
//              8. DEFAULT_SVR()->GetProductClassType
//              9. DEFAULT_SVR()->GetTradingDay
//              10.DEFAULT_SVR()->GetInstrumentInfo
//              11.DEFAULT_SVR()->GetQuotInfo
//              12.DEFAULT_SVR()->GetMarginRate
//          ����ģ��CfgMgr
//              1. CfgMgr::GetInstance()->GetListCfg(GID_POSITIONDETAIL)
//              2. CfgMgr::GetInstance()->GetListColCount(GID_POSITIONDETAIL)
//              3. CfgMgr::GetInstance()->GetListColCfg(GID_POSITIONDETAIL, i);
//          ���ó���ConfigApp
//              1. GETTOPWINDOW()->ProcessEvent(wxEVT_LOAD_CONFIG_APP) - ���������
//************************************************************************************

#pragma once

#include "cfgMgr/CfgMgr.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../Module-Misc/DataListCtrl.hpp"

using namespace std;


class CPositionDetailInfoList : public CDataListCtrl<PositionDetailKey,PlatformStru_PositionDetail>
{
public:
	CPositionDetailInfoList(wxWindow *parent,
           wxWindowID id = wxID_ANY,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = wxLCEXT_REPORT|wxLCEXT_VRULES|wxLCEXT_TOGGLE_COLOUR,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxEmptyString);

public:
	virtual ~CPositionDetailInfoList(void);

    void ShowAll();

    //����listctrl�Ĳ���
    void Init(void);

    //�Ҽ��˵�
	void OnContext(wxContextMenuEvent& evt);

	static int TradeCallBackFunc(const AbstractBusinessData&);	  //�ɽ��ر�
	static int QuotCallBackFunc(const AbstractBusinessData&);	  //����
	void OnPositionDetailUpdate(wxCommandEvent& evt);                 
	void OnPositionDetailTradeUpdate(wxCommandEvent& evt);
    void OnCfgChanged();
	void OnLanguageChanged(int langid);
	void OnColumnResize(wxExtListEvent& event);

    void OnRspQryPositionDetailLast(wxCommandEvent& evt);
    void OnGetMarginRate(wxCommandEvent& evt);
    void OnRtnTrade(wxCommandEvent& evt);
//    void OnRtnOrder(wxCommandEvent& evt);
    void OnRtnDepthMarketData(wxCommandEvent& evt);
	void OnMAPlatformAdd(wxCommandEvent& event);
	void OnMAPlatformDelete(wxCommandEvent& evt);
    static bool UpdateListItemCallBack(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem);
	void SaveColWidth();
	set<IPlatformSingleSvr*>* m_ppSvrSet;

	DECLARE_EVENT_TABLE();


private:	
	static CPositionDetailInfoList *m_PostionDetailList;
	
    unsigned long   m_TextColor;  //������ɫ
	std::map<int, long> m_FieldID2ColumnID; //��¼�ֶ�����һ����ʾ 

    //ÿ����Լ��Ӧ����һ�γֲ���ϸ���ݵĸ������
    //ÿ����Ҫ��������ʱ�������ǰ��UpdateSeq�������ֵ����ʾ����Ҫ����
    //��Ϊ�ײ㷢��UI�����ݸ���ָ����ܻ������ʹ�����Ҫ��������Ʊ�����Ч����
    std::map<std::string,long> m_PositionDetailDataLastUpdateSeq;

    zqWriteLog*           m_pWriteLog;


};