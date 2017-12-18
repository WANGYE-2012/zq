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
//              6. DEFAULT_SVR()->GetPositionKeySet()-��ȡ�ֲּ�¼��ֵ�б�,���سֲּ�¼������
//              7. DEFAULT_SVR()->GetPositionData()-��ȡָ����Լ������Ͷ���ĳֲ�����
//              8. DEFAULT_SVR()->GetCurPrice()-��ȡָ����Լ���ּ�
//          ����ģ��CfgMgr
//              1. CfgMgr::GetInstance()->GetListCfg(GID_POSITION)
//              2. CfgMgr::GetInstance()->GetListColCount(GID_POSITION)
//              3. CfgMgr::GetInstance()->GetListColCfg(GID_POSITION, i);
//          ���ó���ConfigApp
//              1. GETTOPWINDOW()->ProcessEvent(wxEVT_LOAD_CONFIG_APP) - ���������
//          ���۱�ģ��
//              1. GETTOPWINDOW()->ProcessEvent(wxEVT_GRID_INSTRUMENTID_CHANGED) - ��ǰ��Լ����
//          �µ���ģ��
//              1. GETTOPWINDOW()->ProcessEvent(wxEVT_ORDERINSERT_ORDERCHANGE) - �м۷���
//              2. GETTOPWINDOW()->ProcessEvent(wxEVT_ORDERINSERT_MOUSECLOSE) - ������ƽ��
//          ���㴰��ģ��
//              1. ((MainFrame*)GETTOPWINDOW())->GetStyle() - ȡ��������
//************************************************************************************

#pragma once

#include "..\inc\Module-Misc\ExtListCtrl.h"
#include "..\\cfgMgr\\CfgMgr.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../Module-Misc/DataListCtrl.hpp"


class zqWriteLog;


typedef std::multimap<std::string, PlatformStru_Position > PositionMAP;
class CMovePositionDlg;
class CSwapPositionDlg;

class CPositionInfoList : public CDataListCtrl<PositionKey,PlatformStru_Position>
{

public:
	CPositionInfoList(wxWindow *parent,
					   wxWindowID id = wxID_ANY,
					   const wxPoint& pos = wxDefaultPosition,
					   const wxSize& size = wxDefaultSize,
					   long style = wxLCEXT_REPORT|wxLCEXT_VRULES|wxLCEXT_TOGGLE_COLOUR,
					   const wxValidator& validator = wxDefaultValidator,
					   const wxString& name = wxEmptyString);


	virtual ~CPositionInfoList(void);

    void ShowAll();

    //����listctrl�Ĳ���
	void Init();

	std::string GetItemText(int row, int col);
	//�Ʋ�
    void OnMovePosition();
	//��λ����
    void OnSwapPosition();
	void OnUpdatePosMoveSwapSwap(wxCommandEvent& evt);
	void OnContext(wxContextMenuEvent& evt);
    void OnCfgChanged();
	void OnLanguageChanged(int langid);
	void OnActivated(wxExtListEvent& event);
	void OnSelected(wxExtListEvent& event);
	void OnColumnResize(wxExtListEvent& event);
	DECLARE_EVENT_TABLE();

	void SaveColWidth();
	wxString GetListCtrlRowLog(wxExtListCtrl* pListCtrl, int nRow);

private:
	CMovePositionDlg*   m_pMovePositionDlg;
	CSwapPositionDlg*   m_pSwapPositionDlg;

public:
    void OnRspQryPositionLast(wxCommandEvent& evt);
    void OnGetMarginRate(wxCommandEvent& evt);
    void OnRtnTrade(wxCommandEvent& evt);
    void OnRtnOrder(wxCommandEvent& evt);

	void OnRtnDepthMarketData(wxCommandEvent& evt);
	void OnMAPlatformAdd(wxCommandEvent& event);
	void OnMAPlatformDelete(wxCommandEvent& evt);

    static bool UpdateListItemCallBack(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem);
	
	void CmdAddNewPosiPL(PlatformStru_Position& posiData);

    zqWriteLog*           m_pWriteLog;

private:
    //���º�һ����ԼID��ص���
    void UpdateByInstrument_Internal(const string& strAccount,const string& strInstrumentID);

private:
	static CPositionInfoList *m_PostionList;

    unsigned long   m_TextColor;  //������ɫ
	std::map<int, long> m_FieldID2ColumnID; //��¼�ֶ�����һ����ʾ 

    //PlatformID -> map<InstrumentID,UpdateSeq>
    //ÿ����Լ��Ӧ����һ�γֲ����ݵĸ������
    //ÿ����Ҫ���³ֲ�����ʱ�������ǰ��UpdateSeq�������ֵ����ʾ����Ҫ����
    //��Ϊ�ײ㷢��UI�����ݸ���ָ����ܻ������ʹ�����Ҫ��������Ʊ�����Ч����
    map<int,map<string,long>> m_PositionDataLastUpdateSeq;

};