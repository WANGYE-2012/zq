#include "StdAfx.h"
#include "MainGrid.h"
#include "../inc/Module-Misc2/tools_util.h"
#include "Module-Misc2/GlobalFunc.h"
#include "Module-Misc2/globalDefines.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include "../Module-Misc/orderCommonFunc.h"
#include "Module-Misc2/GlobalConfigMgr.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

const int COL_LEFT_MARGIN_WIDTH = 10;
const int COL_RIGHT_MARGIN_WIDTH = 10;
//���ACCOUNTS-159
#define FILL_ITEM(vec,id)\
	if(id<vec.size())\
		vec[id]=string("-");\
	else {\
    for(int i=vec.size();i<=id;i++)\
		vec.push_back(string("-"));\
	}
#define FILL_ITEM_ONCE(itemname, priceTick)  \
	FILL_ITEM(vec,QUOT_FIELD_ID_##itemname);\
	if(!util::isInvalidValue(rawQuot.itemname)&&vec[QUOT_FIELD_ID_##itemname]!=Price2String(rawQuot.itemname, priceTick))\
	vec[QUOT_FIELD_ID_##itemname]=Price2String(rawQuot.itemname, priceTick)
IMPLEMENT_DYNAMIC_CLASS(MainGrid,wxGrid)
MainGrid::MainGrid(wxWindow *parent,
				   wxWindowID id,
				   const wxPoint& pos,
				   const wxSize& size,
				   long style ):
wxGrid(parent,id,pos,size,style)
{
	SetColLabelSize(23);
	EnableDragGridSize(false);
	SetRowLabelSize( 0 );
	EnableDragRowSize(false);
	EnableEditing(false);
	EnableGridLines(false);
	SetScrollLineY(GetDefaultRowSize());
	SetCellHighlightPenWidth(0);
	SetCellHighlightROPenWidth(0);
	GetGridWindow()->Connect(wxEVT_RIGHT_UP,wxMouseEventHandler(MainGrid::OnMainContextMenu));
}

MainGrid::~MainGrid(void)
{
	GetGridWindow()->Disconnect(wxEVT_RIGHT_UP,wxMouseEventHandler(MainGrid::OnMainContextMenu));
}
BEGIN_EVENT_TABLE(MainGrid, wxGrid)
    EVT_KEY_DOWN( MainGrid::OnKeyDown )
END_EVENT_TABLE()
void MainGrid::OnMainContextMenu( wxMouseEvent& event )
{
	wxGridEvent gridEvt( GetId(),
		wxEVT_GRID_LABEL_RIGHT_CLICK,
		this,
		-1,-1,
		event.GetX(),
		event.GetY(),
		false,
		event.ControlDown(),
		event.ShiftDown(),
		event.AltDown(),
		event.MetaDown() );
	GetEventHandler()->ProcessEvent(gridEvt);
}
void MainGrid::SelectCurrentRow()
{
	int nCursorRow = GetGridCursorRow();
	SelectBlock(nCursorRow,0,nCursorRow,GetNumberCols()-1);
	MakeCellVisible(nCursorRow,GetGridCursorCol());
}
void MainGrid::OnKeyDown( wxKeyEvent& event )
{
	wxGrid::OnKeyDown(event);

	int nKeyCode = event.GetKeyCode();
	if ( nKeyCode == WXK_UP || nKeyCode == WXK_DOWN )
	{
		SelectCurrentRow();
	}
	else if(nKeyCode ==WXK_RETURN||nKeyCode ==WXK_NUMPAD_ENTER)
	{
		int nCursorRow = GetGridCursorRow();
		if(nCursorRow>0)
		{
			wxCommandEvent parentevt(wxEVT_QUOT_VIEW_CHANGE);
			GetParent()->ProcessEvent(parentevt);
		}
	}

}
bool MainGrid::IsRowVisible( int row,bool wholeCellVisible)
{
    int rowtop,rowbottom;
	rowtop=GetRowTop(row);
	CalcScrolledPosition(0,rowtop,NULL,&rowtop);
    rowbottom=rowtop+GetRowHeight(row);
    if (m_gridLinesEnabled)
    {
        rowtop -= 1;
    }
    int cw, ch;
	GetGridWindow()->GetClientSize( &cw, &ch );
	return ((rowtop>=0&&rowtop<ch)||(rowbottom>0&&rowbottom<=ch));
}
void MainGrid::RefreshRow(int row)
{
    int cw, ch;
	GetGridWindow()->GetClientSize( &cw, &ch );
    int rowtop;
	rowtop=GetRowTop(row);
	CalcScrolledPosition(0,rowtop,NULL,&rowtop);
	wxRect rt(0,rowtop,cw,GetRowHeight(row));
	GetGridWindow()->RefreshRect(rt);
}
bool MainGrid::SetTable( wxGridTableBase *table, bool takeOwnership,
                       wxGrid::wxGridSelectionModes selmode )
{
	bool bRet=wxGrid::SetTable(table,takeOwnership,selmode);
	CfgMgr *pCfgMgr=CFG_MGR_DEFAULT();
    if(pCfgMgr)
	{
		int colcount=pCfgMgr->GetListVisibleColCount(GID_QUOT_TABLE);
		for(int i=0;i<colcount;i++)
		{
			LPLIST_COLUMN_CFG pColCfg=pCfgMgr->GetListVisibleColCfg(GID_QUOT_TABLE,i);
			SetColSize(i,pColCfg->Width);
		}
	}
	return bRet;

}

bool MainGrid::GetSelections( vector<int>& vRows )
{
	if(GetTable()==NULL) return false;
	wxGridCellCoordsArray tls=GetSelectionBlockTopLeft();
	wxGridCellCoordsArray brs=GetSelectionBlockBottomRight();
	for(int i=0;i<(int)tls.size();i++)
	{
		for(int row=tls[i].GetRow();row<=brs[i].GetRow();++row)
		{
			vRows.push_back(row);
		}
	}
	return true;

}

void MainGrid::AutoAdjustColumnWidth()
{
	int nColumnCount = GetNumberCols();
	for ( int i = 0; i < nColumnCount; i++ )
	{
		wxString strMaxText = GetColumnLongestText(i);
		int nHeiht = 0;
		int nTextWidth = 0;
		GetTextExtent(strMaxText, &nTextWidth, &nHeiht);
		nTextWidth = nTextWidth + COL_LEFT_MARGIN_WIDTH + COL_RIGHT_MARGIN_WIDTH;
		SetColumnWidth(i, nTextWidth);
	}

	Refresh();
}

wxString MainGrid::GetColumnLongestText( long nCol )
{
	wxString strMaxText;
	int nItemCount = GetNumberRows();
	for ( int i = 0; i < nItemCount; i++ )
	{
		wxString text = GetCellValue(i, nCol);
		if ( text.Length() > strMaxText.Length() )
		{
			strMaxText = text;
		}
	}

	wxString strHeader = GetColLabelValue(nCol);
	if ( strHeader.Length() > strMaxText.Length())
	{
		strMaxText = strHeader;
	}

	return strMaxText;
}

QuotGridTable::QuotGridTable(int id)
{
	m_pCfgMgr=CFG_MGR_DEFAULT();
	m_id=id;
	m_rowcount=0;
	m_pTextData=NULL;
};
int QuotGridTable::GetNumberCols() 
{ 
	if(m_pCfgMgr)
		return m_pCfgMgr->GetListVisibleColCount(m_id);
	else
		return 0;
}
wxString QuotGridTable::GetValue( int row, int col )
{

	if(m_pTextData)
	{
		map<int,vector<string>>::iterator iter=m_pTextData->find(row);
		if(iter!=m_pTextData->end())
		{
			vector<string>& vec=iter->second;
			int realcol=GetIdFromColumn(col);
			if(realcol>=0&&realcol<(int)vec.size())
				return vec[realcol];
		}
	}
	return wxEmptyString;
}
wxGridCellAttr *QuotGridTable::GetAttr( int row, int col,wxGridCellAttr::wxAttrKind  kind )
{
	wxGridCellAttr *pCellAttr = new wxGridCellAttr(); 
	LPLIST_TABLE_CFG pListCfg = m_pCfgMgr->GetListCfg(GID_QUOT_TABLE);
	pCellAttr->SetBackgroundColour((row%2)>0?
		wxColor(pListCfg->EvenLineBackgroundColor):
		wxColor(pListCfg->OddLineBackgroundColor));
	

	LPLIST_COLUMN_CFG pColCfg = m_pCfgMgr->GetListVisibleColCfg(m_id, col);//��������ɫ
	if(pColCfg) 
	{
		pCellAttr->SetTextColour(wxColor(pColCfg->TextColor));
	}
	SetPriceChangeColor(pCellAttr,row,col);//�����ǵ���ɫ�ı�
    //���ö���
	if(GetIdFromColumn(col)!= QUOT_FIELD_ID_InstrumentID)
	{
		pCellAttr->SetAlignment(wxALIGN_RIGHT,-1);
	}
	return pCellAttr;
}
void QuotGridTable::SetPriceChangeColor(wxGridCellAttr * pCellAttr,int row,int col)
{
	int colid=GetIdFromColumn(col);
	if(colid!=QUOT_FIELD_ID_LastPrice&&
		colid!=QUOT_FIELD_ID_ChangeValue&&
		colid!=QUOT_FIELD_ID_ChangePercentage&&
		colid!=QUOT_FIELD_ID_BidPrice1&&
		colid!=QUOT_FIELD_ID_AskPrice1) 
		return;
    string InstrumentID=GetInstrumentIDFromRow(row);
	if(InstrumentID.empty())
		return;
	PlatformStru_DepthMarketData rawData;
	memset(&rawData,0,sizeof(rawData));
	if(CURRENT_SVR()&&CURRENT_SVR()->GetQuotInfo(InstrumentID,rawData)==0)
	{

		if(util::isInvalidValue(rawData.LastPrice)||
			util::isInvalidValue(rawData.PreSettlementPrice)||
			util::equals(rawData.LastPrice, 0) ||
			util::equals(rawData.PreSettlementPrice, 0))	
		{
			pCellAttr->SetTextColour(RGB(0,255,0));//��ɫ		
		}
		else
		{
			double theChange= rawData.LastPrice-rawData.PreSettlementPrice;
			if(util::less(theChange, 0))	
				pCellAttr->SetTextColour(RGB(0,255,0));//��ɫ		
			else
				pCellAttr->SetTextColour(RGB(255,0,0));//��ɫ
		}
	}
}
wxString QuotGridTable::GetColLabelValue( int col )
{
	if(m_pCfgMgr)
	{
		LPLIST_COLUMN_CFG pColCfg = m_pCfgMgr->GetListVisibleColCfg(m_id, col);
		if(pColCfg) 
		{
			return m_pCfgMgr->GetColCfgCaption(m_id,pColCfg->id);
		}
	}
	return wxEmptyString;
}
int QuotGridTable::GetIdFromColumn(int col)
{
	LPLIST_COLUMN_CFG pColCfg = m_pCfgMgr->GetListVisibleColCfg(m_id, col);
	if(pColCfg) 
	{
		return pColCfg->id;
	}
	return -1;
}
string QuotGridTable::GetInstrumentIDFromRow(int row)
{
	string str;
	map<int,vector<string>>::iterator iter=m_pTextData->find(row);
	if(iter!=m_pTextData->end())
	{
		vector<string>& vec=iter->second;
		if(QUOT_FIELD_ID_InstrumentID<vec.size())
			str=vec[QUOT_FIELD_ID_InstrumentID];
	}
	return str;
}
char QuotGridTable::GetDirectionFromColumn(int col)
{
	char Direction=0;
	int id=GetIdFromColumn(col);
	if(id==QUOT_FIELD_ID_BidPrice1||id==QUOT_FIELD_ID_BidVolume1)
		Direction=THOST_FTDC_D_Buy;
	else if(id==QUOT_FIELD_ID_AskPrice1||id==QUOT_FIELD_ID_AskVolume1)
		Direction=THOST_FTDC_D_Sell;
	return Direction;
}
bool QuotGridTable::IsInstrumentIDColumn(int Col)
{
	int id=GetIdFromColumn(Col);
	return id==QUOT_FIELD_ID_InstrumentID;
}

int QuotGridTable::GetRowFromInstrumentID(string InstrumentID)
{
	if(m_pTextData)
	{
		map<int,vector<string>>::iterator iter;
		for(iter=m_pTextData->begin();iter!=m_pTextData->end();++iter)
		{
			vector<string>& vec=iter->second;
			if(vec[QUOT_FIELD_ID_InstrumentID]==InstrumentID)
				return iter->first;
		}
	}
	return -1;

}
void QuotGridTable::LoadNewQuotData(PlatformStru_DepthMarketData& rawQuot,bool bRefresh)
{
	int row=GetRowFromInstrumentID(rawQuot.InstrumentID);
	if(row<0)return;
	bool bCellVisible=true;
	PlatformStru_DepthMarketData OldQuot;
    bool bHaveOldQuot= CURRENT_SVR()?(CURRENT_SVR()->GetOldQuotInfo(rawQuot.InstrumentID,OldQuot)==0):FALSE;

	PlatformStru_InstrumentInfo insInfo;
    memset(&insInfo,0,sizeof(insInfo));
	if(CURRENT_SVR()) CURRENT_SVR()->GetInstrumentInfo(rawQuot.InstrumentID,insInfo);


	map<int,vector<string>>::iterator iter=m_pTextData->find(row);
	if(iter!=m_pTextData->end())
	{
		vector<string>& vec=iter->second;

		//�������¼�
		FILL_ITEM(vec,QUOT_FIELD_ID_LastPrice);
		if(!util::isInvalidValue(rawQuot.LastPrice))	
		{
			vec[QUOT_FIELD_ID_LastPrice]=Price2String(rawQuot.LastPrice, insInfo.PriceTick);
		}
		//�������һ
		FILL_ITEM(vec,QUOT_FIELD_ID_BidPrice1);
		if(!util::isInvalidValue(rawQuot.BidPrice1))	
		{
			vec[QUOT_FIELD_ID_BidPrice1]=Price2String(rawQuot.BidPrice1, insInfo.PriceTick);
		}
		//��������һ
		FILL_ITEM(vec,QUOT_FIELD_ID_BidVolume1);
		if(!util::isInvalidValue(rawQuot.BidVolume1))	
		{
			vec[QUOT_FIELD_ID_BidVolume1]=GlobalFunc::ConvertToString(rawQuot.BidVolume1);
		}
		//��������һ
		FILL_ITEM(vec,QUOT_FIELD_ID_AskPrice1);
		if(!util::isInvalidValue(rawQuot.AskPrice1))	
		{
			vec[QUOT_FIELD_ID_AskPrice1]=Price2String(rawQuot.AskPrice1, insInfo.PriceTick);
		}
		//��������һ
		FILL_ITEM(vec,QUOT_FIELD_ID_AskVolume1);
		if(!util::isInvalidValue(rawQuot.AskVolume1))	
		{
			vec[QUOT_FIELD_ID_AskVolume1]=GlobalFunc::ConvertToString(rawQuot.AskVolume1);
		}
		//�����ǵ�
		FILL_ITEM(vec,QUOT_FIELD_ID_ChangeValue);
		if(!(util::isInvalidValue(rawQuot.LastPrice)||
			util::isInvalidValue(rawQuot.PreSettlementPrice)||
			util::equals(rawQuot.LastPrice, 0) ||
			util::equals(rawQuot.PreSettlementPrice, 0)))
		{
			double theChange= rawQuot.LastPrice-rawQuot.PreSettlementPrice;
			vec[QUOT_FIELD_ID_ChangeValue]=Price2String(theChange, insInfo.PriceTick);
		}
		//�����ǵ�����
		FILL_ITEM(vec,QUOT_FIELD_ID_ChangePercentage);
		if(!(util::isInvalidValue(rawQuot.LastPrice)||
			util::isInvalidValue(rawQuot.PreSettlementPrice)||
			util::equals(rawQuot.LastPrice, 0) ||
			util::equals(rawQuot.PreSettlementPrice, 0)))
		{
			char newText[256];
			double theChange= rawQuot.LastPrice-rawQuot.PreSettlementPrice;
			sprintf(newText,"%.02f%%", theChange/rawQuot.PreSettlementPrice*100.0);
			vec[QUOT_FIELD_ID_ChangePercentage]=GlobalFunc::ConvertToString(newText);
		}
		//���óɽ���
		FILL_ITEM(vec,QUOT_FIELD_ID_Volume);
		if(!util::isInvalidValue(rawQuot.Volume))	
		{
			vec[QUOT_FIELD_ID_Volume]=GlobalFunc::ConvertToString(rawQuot.Volume);
		}
		//�������������һ�ʳɽ�����˫�߼�����
		FILL_ITEM(vec,QUOT_FIELD_ID_LastVolume);
		if(bHaveOldQuot)
		{
			int LastVolume=rawQuot.Volume-OldQuot.Volume;
			if(LastVolume>=0)
			{
				vec[QUOT_FIELD_ID_LastVolume]=GlobalFunc::ConvertToString(LastVolume);
			}
		}
		else
		{
			vec[QUOT_FIELD_ID_LastVolume]=GlobalFunc::ConvertToString(0);
		}

		PlatformStru_InstrumentInfo outData;
        memset(&outData,0,sizeof(outData));
		if(CURRENT_SVR()) CURRENT_SVR()->GetInstrumentInfo(rawQuot.InstrumentID, outData);
		//���� ��Լ����
		FILL_ITEM(vec,QUOT_FIELD_ID_InstrumentName);
		if(vec[QUOT_FIELD_ID_InstrumentName]==string("-"))//ֻ������һ��
		{
			if(strlen(outData.InstrumentName)>0)
				vec[QUOT_FIELD_ID_InstrumentName]=GlobalFunc::ConvertToString(outData.InstrumentName);
		}

		//���ñ���
		FILL_ITEM(vec, QUOT_FIELD_ID_CURRENCY);
		if (strlen(outData.Currency) > 0 &&
			vec[QUOT_FIELD_ID_CURRENCY] != GlobalFunc::ConvertToString(outData.Currency))
		{
			vec[QUOT_FIELD_ID_CURRENCY] = GlobalFunc::ConvertToString(outData.Currency);
		}
		//����������
		FILL_ITEM(vec, QUOT_FIELD_ID_NewVolume);
		vec[QUOT_FIELD_ID_NewVolume]=GlobalFunc::ConvertToString(rawQuot.NewVolume);
		//���� ������ID
		FILL_ITEM(vec,QUOT_FIELD_ID_ExchangeID);
		if(vec[QUOT_FIELD_ID_ExchangeID]==string("-"))//ֻ������һ��
		{
			if(strlen(outData.ExchangeID)>0)
                vec[QUOT_FIELD_ID_ExchangeID]=CURRENT_SVR()?CURRENT_SVR()->ExchangeID2String(outData.ExchangeID):"";
		}
		//���ÿ��̼�
		FILL_ITEM(vec,QUOT_FIELD_ID_OpenPrice);
		if(!util::isInvalidValue(rawQuot.OpenPrice))
		{
			vec[QUOT_FIELD_ID_OpenPrice]=Price2String(rawQuot.OpenPrice, insInfo.PriceTick);
		}
		//���óֲ���
		FILL_ITEM(vec,QUOT_FIELD_ID_OpenInterest);
		if(!util::isInvalidValue(rawQuot.OpenInterest))
		{
			vec[QUOT_FIELD_ID_OpenInterest]=GlobalFunc::ConvertToString(rawQuot.OpenInterest);
		}
		//������ͣ���
		FILL_ITEM_ONCE(UpperLimitPrice, insInfo.PriceTick);
		//���õ�ͣ���
		FILL_ITEM_ONCE(LowerLimitPrice, insInfo.PriceTick);
		//���� �ϴν����
		FILL_ITEM_ONCE(PreSettlementPrice, insInfo.PriceTick);
		//���� ��߼�
		FILL_ITEM(vec,QUOT_FIELD_ID_HighestPrice);
		if(!util::isInvalidValue(rawQuot.HighestPrice))
		{
			vec[QUOT_FIELD_ID_HighestPrice]=Price2String(rawQuot.HighestPrice, insInfo.PriceTick);
		}
		//���� ��ͼ�
		FILL_ITEM(vec,QUOT_FIELD_ID_LowestPrice);
		if(!util::isInvalidValue(rawQuot.LowestPrice))
		{
			vec[QUOT_FIELD_ID_LowestPrice]=Price2String(rawQuot.LowestPrice, insInfo.PriceTick);
		}
		//����޸�ʱ��
		FILL_ITEM(vec,QUOT_FIELD_ID_UpdateTime);
		vec[QUOT_FIELD_ID_UpdateTime]=GlobalFunc::ConvertToString(rawQuot.UpdateTime);
		//���� ������
		FILL_ITEM_ONCE(PreClosePrice, insInfo.PriceTick);
		//���� ��ֲ���
		FILL_ITEM(vec,QUOT_FIELD_ID_PreOpenInterest);
		if(/*vec[FIELD_ID_PreOpenInterest]==string("-")&&*/
			!util::isInvalidValue(rawQuot.PreOpenInterest))
		{
			vec[QUOT_FIELD_ID_PreOpenInterest]=GlobalFunc::ConvertToString(rawQuot.PreOpenInterest);
		}
		//���� ������
		FILL_ITEM(vec,QUOT_FIELD_ID_ClosePrice);
		if(!util::isInvalidValue(rawQuot.ClosePrice))
		{
			vec[QUOT_FIELD_ID_ClosePrice]=Price2String(rawQuot.ClosePrice, insInfo.PriceTick);
		}
		//���� ������
		FILL_ITEM(vec,QUOT_FIELD_ID_SettlementPrice);
		if(!util::isInvalidValue(rawQuot.SettlementPrice))
		{
			vec[QUOT_FIELD_ID_SettlementPrice]=Price2String(rawQuot.SettlementPrice, insInfo.PriceTick);
		}
		//���� �ɽ����
		FILL_ITEM(vec,QUOT_FIELD_ID_Turnover);
		if(!util::isInvalidValue(rawQuot.Turnover))
		{
			char newText[256];
			sprintf(newText,"%.2lf",rawQuot.Turnover);
			vec[QUOT_FIELD_ID_Turnover]=GlobalFunc::ConvertToString(newText);
		}
		
		//ʮ������
		//������۶�
		FILL_ITEM(vec,QUOT_FIELD_ID_BidPrice2);
		if(!util::isInvalidValue(rawQuot.BidPrice2))	
		{
			vec[QUOT_FIELD_ID_BidPrice2]=Price2String(rawQuot.BidPrice2, insInfo.PriceTick);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_BidVolume2);
		if(!util::isInvalidValue(rawQuot.BidVolume2))	
		{
			vec[QUOT_FIELD_ID_BidVolume2]=GlobalFunc::ConvertToString(rawQuot.BidVolume2);
		}
		//�������۶�
		FILL_ITEM(vec,QUOT_FIELD_ID_AskPrice2);
		if(!util::isInvalidValue(rawQuot.AskPrice2))	
		{
			vec[QUOT_FIELD_ID_AskPrice2]=Price2String(rawQuot.AskPrice2, insInfo.PriceTick);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_AskVolume2);
		if(!util::isInvalidValue(rawQuot.AskVolume2))	
		{
			vec[QUOT_FIELD_ID_AskVolume2]=GlobalFunc::ConvertToString(rawQuot.AskVolume2);
		}

		//���������
		FILL_ITEM(vec,QUOT_FIELD_ID_BidPrice3);
		if(!util::isInvalidValue(rawQuot.BidPrice3))	
		{
			vec[QUOT_FIELD_ID_BidPrice3]=Price2String(rawQuot.BidPrice3, insInfo.PriceTick);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_BidVolume3);
		if(!util::isInvalidValue(rawQuot.BidVolume3))	
		{
			vec[QUOT_FIELD_ID_BidVolume3]=GlobalFunc::ConvertToString(rawQuot.BidVolume3);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_AskPrice3);
		if(!util::isInvalidValue(rawQuot.AskPrice3))	
		{
			vec[QUOT_FIELD_ID_AskPrice3]=Price2String(rawQuot.AskPrice3, insInfo.PriceTick);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_AskVolume3);
		if(!util::isInvalidValue(rawQuot.AskVolume3))	
		{
			vec[QUOT_FIELD_ID_AskVolume3]=GlobalFunc::ConvertToString(rawQuot.AskVolume3);
		}

		//���������
		FILL_ITEM(vec,QUOT_FIELD_ID_BidPrice4);
		if(!util::isInvalidValue(rawQuot.BidPrice4))	
		{
			vec[QUOT_FIELD_ID_BidPrice4]=Price2String(rawQuot.BidPrice4, insInfo.PriceTick);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_BidVolume4);
		if(!util::isInvalidValue(rawQuot.BidVolume4))	
		{
			vec[QUOT_FIELD_ID_BidVolume4]=GlobalFunc::ConvertToString(rawQuot.BidVolume4);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_AskPrice4);
		if(!util::isInvalidValue(rawQuot.AskPrice4))	
		{
			vec[QUOT_FIELD_ID_AskPrice4]=Price2String(rawQuot.AskPrice4, insInfo.PriceTick);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_AskVolume4);
		if(!util::isInvalidValue(rawQuot.AskVolume4))	
		{
			vec[QUOT_FIELD_ID_AskVolume4]=GlobalFunc::ConvertToString(rawQuot.AskVolume4);
		}

		//���������
		FILL_ITEM(vec,QUOT_FIELD_ID_BidPrice5);
		if(!util::isInvalidValue(rawQuot.BidPrice5))	
		{
			vec[QUOT_FIELD_ID_BidPrice5]=Price2String(rawQuot.BidPrice5, insInfo.PriceTick);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_BidVolume5);
		if(!util::isInvalidValue(rawQuot.BidVolume5))	
		{
			vec[QUOT_FIELD_ID_BidVolume5]=GlobalFunc::ConvertToString(rawQuot.BidVolume5);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_AskPrice5);
		if(!util::isInvalidValue(rawQuot.AskPrice5))	
		{
			vec[QUOT_FIELD_ID_AskPrice5]=Price2String(rawQuot.AskPrice5, insInfo.PriceTick);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_AskVolume5);
		if(!util::isInvalidValue(rawQuot.AskVolume5))	
		{
			vec[QUOT_FIELD_ID_AskVolume5]=GlobalFunc::ConvertToString(rawQuot.AskVolume5);
		}

		//���������
		FILL_ITEM(vec,QUOT_FIELD_ID_BidPrice6);
		if(!util::isInvalidValue(rawQuot.BidPrice6))	
		{
			vec[QUOT_FIELD_ID_BidPrice6]=Price2String(rawQuot.BidPrice6, insInfo.PriceTick);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_BidVolume6);
		if(!util::isInvalidValue(rawQuot.BidVolume6))	
		{
			vec[QUOT_FIELD_ID_BidVolume6]=GlobalFunc::ConvertToString(rawQuot.BidVolume6);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_AskPrice6);
		if(!util::isInvalidValue(rawQuot.AskPrice6))	
		{
			vec[QUOT_FIELD_ID_AskPrice6]=Price2String(rawQuot.AskPrice6, insInfo.PriceTick);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_AskVolume6);
		if(!util::isInvalidValue(rawQuot.AskVolume6))	
		{
			vec[QUOT_FIELD_ID_AskVolume6]=GlobalFunc::ConvertToString(rawQuot.AskVolume6);
		}

		//���������
		FILL_ITEM(vec,QUOT_FIELD_ID_BidPrice7);
		if(!util::isInvalidValue(rawQuot.BidPrice7))	
		{
			vec[QUOT_FIELD_ID_BidPrice7]=Price2String(rawQuot.BidPrice7, insInfo.PriceTick);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_BidVolume7);
		if(!util::isInvalidValue(rawQuot.BidVolume7))	
		{
			vec[QUOT_FIELD_ID_BidVolume7]=GlobalFunc::ConvertToString(rawQuot.BidVolume7);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_AskPrice7);
		if(!util::isInvalidValue(rawQuot.AskPrice7))	
		{
			vec[QUOT_FIELD_ID_AskPrice7]=Price2String(rawQuot.AskPrice7, insInfo.PriceTick);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_AskVolume7);
		if(!util::isInvalidValue(rawQuot.AskVolume7))	
		{
			vec[QUOT_FIELD_ID_AskVolume7]=GlobalFunc::ConvertToString(rawQuot.AskVolume7);
		}

		//������۰�
		FILL_ITEM(vec,QUOT_FIELD_ID_BidPrice8);
		if(!util::isInvalidValue(rawQuot.BidPrice8))	
		{
			vec[QUOT_FIELD_ID_BidPrice8]=Price2String(rawQuot.BidPrice8, insInfo.PriceTick);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_BidVolume8);
		if(!util::isInvalidValue(rawQuot.BidVolume8))	
		{
			vec[QUOT_FIELD_ID_BidVolume8]=GlobalFunc::ConvertToString(rawQuot.BidVolume8);
		}
		//�������۰�
		FILL_ITEM(vec,QUOT_FIELD_ID_AskPrice8);
		if(!util::isInvalidValue(rawQuot.AskPrice8))	
		{
			vec[QUOT_FIELD_ID_AskPrice8]=Price2String(rawQuot.AskPrice8, insInfo.PriceTick);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_AskVolume8);
		if(!util::isInvalidValue(rawQuot.AskVolume8))	
		{
			vec[QUOT_FIELD_ID_AskVolume8]=GlobalFunc::ConvertToString(rawQuot.AskVolume8);
		}

		//������۾�
		FILL_ITEM(vec,QUOT_FIELD_ID_BidPrice9);
		if(!util::isInvalidValue(rawQuot.BidPrice9))	
		{
			vec[QUOT_FIELD_ID_BidPrice9]=Price2String(rawQuot.BidPrice9, insInfo.PriceTick);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_BidVolume9);
		if(!util::isInvalidValue(rawQuot.BidVolume9))	
		{
			vec[QUOT_FIELD_ID_BidVolume9]=GlobalFunc::ConvertToString(rawQuot.BidVolume9);
		}
		//�������۾�
		FILL_ITEM(vec,QUOT_FIELD_ID_AskPrice9);
		if(!util::isInvalidValue(rawQuot.AskPrice9))	
		{
			vec[QUOT_FIELD_ID_AskPrice9]=Price2String(rawQuot.AskPrice9, insInfo.PriceTick);
		}
		//����������
		FILL_ITEM(vec,QUOT_FIELD_ID_AskVolume9);
		if(!util::isInvalidValue(rawQuot.AskVolume9))	
		{
			vec[QUOT_FIELD_ID_AskVolume9]=GlobalFunc::ConvertToString(rawQuot.AskVolume9);
		}

		//�������ʮ
		FILL_ITEM(vec,QUOT_FIELD_ID_BidPrice10);
		if(!util::isInvalidValue(rawQuot.BidPrice10))	
		{
			vec[QUOT_FIELD_ID_BidPrice10]=Price2String(rawQuot.BidPrice10, insInfo.PriceTick);
		}
		//��������ʮ
		FILL_ITEM(vec,QUOT_FIELD_ID_BidVolume10);
		if(!util::isInvalidValue(rawQuot.BidVolume10))	
		{
			vec[QUOT_FIELD_ID_BidVolume10]=GlobalFunc::ConvertToString(rawQuot.BidVolume10);
		}
		//��������ʮ
		FILL_ITEM(vec,QUOT_FIELD_ID_AskPrice10);
		if(!util::isInvalidValue(rawQuot.AskPrice10))	
		{
			vec[QUOT_FIELD_ID_AskPrice10]=Price2String(rawQuot.AskPrice10, insInfo.PriceTick);
		}
		//��������ʮ
		FILL_ITEM(vec,QUOT_FIELD_ID_AskVolume10);
		if(!util::isInvalidValue(rawQuot.AskVolume10))	
		{
			vec[QUOT_FIELD_ID_AskVolume10]=GlobalFunc::ConvertToString(rawQuot.AskVolume10);
		}		
	}
	if(bRefresh&&bCellVisible)
	{
		MainGrid* pGrid=(MainGrid*)GetView();
		if(pGrid&&pGrid->IsRowVisible(row,false)) 
		{
			pGrid->RefreshRow(row);
		}
	}
}

void QuotGridTable::DeleteRows( vector<int> vRows )
{
	if ( NULL == m_pTextData )
	{
		return;
	}

	for ( int i = 0; i < (int)vRows.size(); i++ )
	{
		m_pTextData->erase( vRows[i] );
	}

	map<int,vector<string>> mapTempData;
	mapTempData.clear();
	map<int,vector<string>>::iterator it = m_pTextData->begin();
	for ( int i = 0; it != m_pTextData->end(); ++it, i++ )
	{
		mapTempData[i] = it->second;
	}
	
	*m_pTextData = mapTempData;
	m_rowcount = m_pTextData->size();
}
