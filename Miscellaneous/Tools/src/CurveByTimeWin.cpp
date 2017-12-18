// CurveByTimeWin.h: interface for the CCurveByTimeWin class.
//
//	��ʾ����ʱ��仯�����ߵĴ�����
//
//////////////////////////////////////////////////////////////////////


#include "CurveByTimeWin.h"
#include <windows.h>
#include <Tools_Win32.h>
#include <assert.h>


#ifdef WIN32
#pragma unmanaged
#endif 




//*************************************************************************************
//		CCurveByTimeWin::Stru_PartWndParam��ʵ��
//*************************************************************************************

CCurveByTimeWin::Stru_PartWndParam::Stru_PartWndParam()
{
	hWnd=0;
	bDrawTopBorder=false;
	TopBorderCol=0x7f7f7f;
	TickMin=0;
	hMemBmp=0;
	pValByPox=NULL;
	pPoyByPox=NULL;
	CntOfPox=0;
}
CCurveByTimeWin::Stru_PartWndParam::Stru_PartWndParam(const CCurveByTimeWin::Stru_PartWndParam& r)
{
	new(this) Stru_PartWndParam();
	*this=r;
}
CCurveByTimeWin::Stru_PartWndParam::Stru_PartWndParam(int hwnd,bool bdrawTopBorder,unsigned topBorderCol)
{
	new(this) Stru_PartWndParam();
	hWnd=hwnd;
	bDrawTopBorder=bdrawTopBorder;
	TopBorderCol=topBorderCol;
}
CCurveByTimeWin::Stru_PartWndParam::~Stru_PartWndParam()
{
	hWnd=0;
	TickMin=0;
	ReleaseResource();
	CurveDispRanges.clear();
}
CCurveByTimeWin::Stru_PartWndParam& CCurveByTimeWin::Stru_PartWndParam::operator= (const CCurveByTimeWin::Stru_PartWndParam& r)
{
	ReleaseResource();
	CurveDispRanges.clear();
	hWnd=r.hWnd;
	bDrawTopBorder=r.bDrawTopBorder;
	TopBorderCol=r.TopBorderCol;
	TickMin=r.TickMin;
	return *this;
}
void CCurveByTimeWin::Stru_PartWndParam::ReleaseResource(void)
{
	if(hMemBmp) { DeleteObject((HBITMAP)hMemBmp);  hMemBmp=0; }
	if(pValByPox) { free(pValByPox);  pValByPox=NULL; }
	if(pPoyByPox) { free(pPoyByPox);  pPoyByPox=NULL; }
	CntOfPox=0;
}


//*************************************************************************************
//		CCurveByTimeWin::Stru_CurveData��ʵ��
//*************************************************************************************

//ע�⣬data.x�����֮ǰ����x����
void CCurveByTimeWin::Stru_CurveData::add(const CCurveByTimeWin::Stru_CurveData::Stru_Data& data,bool bForceNewBlock,bool bFromDB)
{
	if(!bForceNewBlock)
	{
		if(mapDataBlocks.size()==0) bForceNewBlock=true;
		else 
		{
			//�ж����һ�����ǲ�������
			map<int,Stru_DataBlock>::const_iterator it=mapDataBlocks.end();
			it--;
			if(it->second.DataCnt>=it->second.MaxData) bForceNewBlock=true;
		}
	}

	if(bForceNewBlock)
		mapDataBlocks[data.x]=CCurveByTimeWin::Stru_CurveData::Stru_DataBlock(blocksz,bFromDB);

	map<int,CCurveByTimeWin::Stru_CurveData::Stru_DataBlock>::iterator it=mapDataBlocks.end();
	assert(it!=mapDataBlocks.begin());
	it--;
	it->second.add(data);
}




//*************************************************************************************
//		CCurveByTimeWin��ʵ��
//*************************************************************************************

CCurveByTimeWin::CCurveByTimeWin()
:	m_pTimeLineWin(NULL)
{
}


CCurveByTimeWin::~CCurveByTimeWin()
{
	m_pTimeLineWin=NULL;
	m_Curves.clear();
	m_WndParams.clear();
}

//���һ�����ߴ��ھ��
void CCurveByTimeWin::AddWnd(int hWnd,bool bdrawTopBorder,unsigned topBorderCol,bool brefresh)
{
	if(!hWnd) return;
	m_WndParams.push_back(Stru_PartWndParam(hWnd,bdrawTopBorder,topBorderCol));
	if(brefresh) refresh(true,true,true,true);
}
//�Ӵ��ڼ���ɾ��һ������
void CCurveByTimeWin::RemoveWnd(int hWnd,bool brefresh)
{
	if(!hWnd) return;
	int wndord=getWndOrd(hWnd);
	if(wndord==-1) return;
	m_WndParams.erase(m_WndParams.begin()+wndord);
	if(brefresh) refresh(true,true,true,true);
}
//�������ߴ��ں�ʱ���ᴰ�ڵ�z�򣬱�֤pTimeLineWin��ˢ��
void CCurveByTimeWin::AdjustZOrder(void)
{
	if(!m_pTimeLineWin) return;

	for(int i=0;i<(int)m_WndParams.size()&&i<(int)m_pTimeLineWin->m_hWnds.size();i++)
		SetWindowPos((HWND)m_pTimeLineWin->m_hWnds[i],(HWND)m_WndParams[i].hWnd,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
}
//ˢ�´�����ʾ
//hWndΪNULLˢ��ȫ���ִ�����ʾ������ˢ��ָ���ִ�����ʾ
void CCurveByTimeWin::repaint(int hWnd)
{
	if(!hWnd)
	{
		for(int i=0;i<(int)m_WndParams.size();i++)
		{
			::InvalidateRect((HWND)m_WndParams[i].hWnd,NULL,FALSE);
			::SendMessage((HWND)m_WndParams[i].hWnd,WM_PAINT,0,0);
		}
	}
	else
	{
		::InvalidateRect((HWND)hWnd,NULL,FALSE);
		::SendMessage((HWND)hWnd,WM_PAINT,0,0);
	}
}

//ʱ��Ƚϵĺ��������ڶԽ����ݵĶ��ַ�����
static int COMPARE_Time_CurveByTimeWin(const void* pdata1,const void* pdata2)
{
	int t1=*((int*)pdata1);
	int t2=*((int*)pdata2);
	if(t1==t2) return 0;
	if(t1>t2) return 1;
	return -1;
}

//��ȡ��һ�����ڵ���ָ��X������λ��
void CCurveByTimeWin::GetFirstDataPositionByX(int iX,int curveord,int& blockord,int& dataord)
{
	blockord=dataord=-1;
	if(curveord<0||curveord>=(int)m_Curves.size()) return;

	//����������ݿ���ź����������
	map<int,Stru_CurveData::Stru_DataBlock>::iterator itlower=m_Curves[curveord].mapDataBlocks.lower_bound(iX);
	map<int,Stru_CurveData::Stru_DataBlock>::iterator itupper=m_Curves[curveord].mapDataBlocks.upper_bound(iX);

	if(itlower==m_Curves[curveord].mapDataBlocks.end())
	{
		if(itlower==m_Curves[curveord].mapDataBlocks.begin())
		{
			//�Ҳ�����ʼ�ĵ�
			return;
		}
		itlower--;
	}
	else
	{
		if(itlower->first==iX)
		{
			//itlower�ĵ�һ����͵���iX
			blockord=itlower->first;
			dataord=0;
			return;
		}
		if(itlower==m_Curves[curveord].mapDataBlocks.begin())
		{
			//itlower�ǵ�һ�����ݿ�
			blockord=itlower->first;
			dataord=0;
			return;
		}

		//��ǰһ����
		itlower--;
	}

	//����ö��ַ���һ�������ʱ�仺��������ҿ�ʼʱ��
	//�����Ǵ�С����
	//�������RltBegin��RltEnd�У���ʾ��һ������������λ�ú͵�һ��������������λ�á�(������ȣ���ʾû�ҵ�)
	int rltBegin,rltEnd;
	CTools_Ansi::LookupInSequenceBuffer(rltBegin,rltEnd,
		&(itlower->second.pData[0].x),itlower->second.DataCnt,sizeof(Stru_CurveData::Stru_Data),
		&iX,
		COMPARE_Time_CurveByTimeWin);

	if(rltBegin<itlower->second.DataCnt)
	{
		//������û���ҵ���������
		blockord=itlower->first;
		dataord=rltBegin;
	}
	else
	{
		itlower++;
		if(itlower!=m_Curves[curveord].mapDataBlocks.end())
		{
			blockord=itlower->first;
			dataord=0;
		}
	}
}
//��ȡ��һ�����ڵ���ָ��X������
bool CCurveByTimeWin::GetFirstDataByX(int iX,int curveord,Stru_CurveData::Stru_Data& data)
{
	int blockord,dataord;
	GetFirstDataPositionByX(iX,curveord,blockord,dataord);
	if(blockord==-1||dataord==-1) return false;
	map<int,Stru_CurveData::Stru_DataBlock>::iterator it=m_Curves[curveord].mapDataBlocks.find(blockord);
	if(it==m_Curves[curveord].mapDataBlocks.end()) return false;
	if(it->second.DataCnt<=dataord) return false;
	data=it->second.pData[dataord];
	return true;
}

int CCurveByTimeWin::GetFirstVisibleCurveOrd(void)
{
	for(int CurveOrd=0;CurveOrd<(int)m_Curves.size();CurveOrd++)
	{
		if((int)m_Curves[CurveOrd].mapDataBlocks.size()>0&&m_Curves[CurveOrd].bView)
			return CurveOrd;
	}
	return -1;
}


//��ȡÿ�����ߵ���ʾ��Χ,m_WndParams[].CurveDispRanges
void CCurveByTimeWin::GetCurveDispRange(void)
{
	if(!m_pTimeLineWin) return;

	for(int wndord=0;wndord<(int)m_WndParams.size();wndord++)
	{
		int startX=(int)m_pTimeLineWin->m_Data.getXbyEntireUnitOff(m_pTimeLineWin->getEntireUnitOff(wndord));
		int endX=(int)m_pTimeLineWin->m_Data.getXbyEntireUnitOff(m_pTimeLineWin->getEntireUnitOff(wndord)+m_pTimeLineWin->getUnitCnt(wndord));

		m_WndParams[wndord].StartXDisp=startX;
		m_WndParams[wndord].EndXDisp=endX;

		m_WndParams[wndord].CurveDispRanges.clear();
		for(int curveord=0;curveord<(int)m_Curves.size();curveord++)
		{
			m_WndParams[wndord].CurveDispRanges.push_back(Stru_DispRange());

			//�ж������Ƿ�����ʾ��Χ֮��
			if((int)m_Curves[curveord].mapDataBlocks.size()==0) continue;
			if(m_Curves[curveord].mapDataBlocks.begin()->first>=endX) continue;
			if(m_Curves[curveord].mapDataBlocks.rbegin()->second.DataCnt>0&&
				m_Curves[curveord].mapDataBlocks.rbegin()->second.pData[m_Curves[curveord].mapDataBlocks.rbegin()->second.DataCnt-1].x<startX) continue;

			GetFirstDataPositionByX(startX , curveord , m_WndParams[wndord].CurveDispRanges[curveord].StartBlock , m_WndParams[wndord].CurveDispRanges[curveord].StartDataOrd);
			GetFirstDataPositionByX(endX , curveord , m_WndParams[wndord].CurveDispRanges[curveord].EndBlock , m_WndParams[wndord].CurveDispRanges[curveord].EndDataOrd);

			if(m_WndParams[wndord].CurveDispRanges[curveord].StartBlock==-1)
				m_WndParams[wndord].CurveDispRanges[curveord].clear();
		}
	}
}

//ˢ�´�����ʾ����
//breset: ��ȡ��ʾ��Χ�����ű�����ԭ��0�����¼���������ڵ�TickMin(ʹ�ִ��ڵ�һ�����ݵ��ڴ����м�)
//brepaint: ˢ��ȫ���ִ���
void CCurveByTimeWin::refresh(bool bresetRatio,bool bresetTickMin,bool brefreshmembmp,bool brepaint)
{
	if(!m_pTimeLineWin) return;

	if(bresetRatio||bresetTickMin||brefreshmembmp)
	{
		//��ȡ��ʾ��Χ
		GetCurveDispRange();
	}

	if(bresetRatio)
	{
		//���ű�����ԭ��0
		m_Cfg.TickRatio2PowerN=0;
	}

	if(bresetRatio||bresetTickMin)
	{
		//���¼���������ڵ�TickMin(ʹ�ִ��ڵ�һ�����ݵ��ڴ����м�)
		ResetTickMins();
	}

	if(brefreshmembmp)
	{
		for(int i=0;i<(int)m_WndParams.size();i++)
			RefreshMemBmp(m_WndParams[i].hWnd);
	}

	if(brepaint)
	{
		//ˢ�¸����ִ��ڵ���ʾ
		repaint(0);
	}
}


//����ʼ����������ִ��ڵ�TickMin(ʹ�ִ��ڵ�һ�����ݵ��ڴ����м�)
void CCurveByTimeWin::ResetTickMins(void)
{
	if(!m_pTimeLineWin) return;
	if(!m_Cfg.TickSize) return;
	if(m_Curves.size()==0) return;

	for(int wndord=0;wndord<(int)m_WndParams.size()&&wndord<(int)m_pTimeLineWin->m_hWnds.size();wndord++)
	{
		m_WndParams[wndord].TickMin=0;

		int FirstVisibleCurveOrd=GetFirstVisibleCurveOrd();
		if(FirstVisibleCurveOrd<0) continue;

		//ֱ��ͼ������TickMin
		if(m_Curves[FirstVisibleCurveOrd].DrawMode==Stru_CurveData::DrawMode_histogram)
			continue;

		int WndHei=CTools_Win32::GetClientHei((HWND)m_WndParams[wndord].hWnd);

		//�˴��ڵ�һ�����Ӧ��X
		int FirstX=(int)m_pTimeLineWin->m_Data.getXbyEntireUnitOff(m_pTimeLineWin->getEntireUnitOff(wndord));
		//��һ�����X��Ӧ������
		Stru_CurveData::Stru_Data FirstData;
		if(GetFirstDataByX(FirstX,FirstVisibleCurveOrd,FirstData))
		{
			int WndTickCnt=m_Cfg.getTickCntByPixelCnt(WndHei);
			m_WndParams[wndord].TickMin=FirstData.y/m_Cfg.TickSize-WndTickCnt/2;
		}
	}
}

//��ָ���ִ��ڵ�MemBmp�ϻ������ᡢ������
void CCurveByTimeWin::DrawFrameInMemBmp_Internal(int MemDC,int WndOrd,int WndWid,int WndHei,int DispOff,int SecOff,int SecCnt,int LeftSpaceWid)
{
	if(!m_pTimeLineWin) return;
	if(!MemDC) return;
	if(WndOrd>=(int)m_WndParams.size()) return;

	//����ˢ�����ʺ�����
	HPEN		hPen=CreatePen(PS_SOLID,1,m_Cfg.LineCol);
    HFONT       hFont=CreateFont(-1*m_Cfg.FontSize,0,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,0,m_Cfg.FontName);
	HPEN		hGridPen=CreatePen(PS_SOLID,1,m_Cfg.GridCol);		//���ڻ�ˮƽ������
	HPEN		hGridPen2=CreatePen(PS_SOLID,1,m_Cfg.GridCol/2);	//���ڻ����㴹ֱ������
	HPEN		hBorderPen=CreatePen(PS_SOLID,1,m_WndParams[WndOrd].TopBorderCol);

    HPEN		OldPen=(HPEN)SelectObject((HDC)MemDC,hPen);
    HFONT		OldFont=(HFONT)SelectObject((HDC)MemDC,hFont);
    SetTextColor((HDC)MemDC,m_Cfg.FontCol);
    SetBkMode((HDC)MemDC,TRANSPARENT);

	int pox;
	vector<int> vecHoriGridPos;

	//�������ע
	if((int)m_Curves.size()>0&&m_Cfg.TickSize>0)
	{
		SelectObject((HDC)MemDC,hPen);
		int SpacingTicks=m_Cfg.getTickCntByPixelCnt(m_Cfg.MarkSpacing);		//�����̶�֮��tick��
		if(SpacingTicks==0) SpacingTicks=1;
		for(int scaletick=m_WndParams[WndOrd].TickMin+SpacingTicks;;scaletick+=SpacingTicks)
		{
			//scaletick�ǿ̶�λ�õ�tick����(���λ��tick��)
			int Pixels=m_Cfg.getPixelCntByTickCnt(scaletick-m_WndParams[WndOrd].TickMin);
			int poy=WndHei-1-Pixels;
			if(poy<=0) break;

			if(m_Cfg.bGrid)
			{
				vecHoriGridPos.push_back(poy);
			}
			else
			{
				//���̶���
				MoveToEx((HDC)MemDC,LeftSpaceWid,poy,NULL);
				LineTo((HDC)MemDC,LeftSpaceWid-m_Cfg.ScaleHei,poy);
			}


			//��ע�̶�
			char buf[256];
			itoa(scaletick*m_Cfg.TickSize,buf,10);
			SIZE Sz=CTools_Win32::GetTextoutSize3((HDC)MemDC,buf,strlen(buf));
			if(poy-Sz.cy/2<0) continue;
			pox=LeftSpaceWid-2-Sz.cx;
			if(!m_Cfg.bGrid) pox-=m_Cfg.ScaleHei;
			TextOut((HDC)MemDC,pox,poy-Sz.cy/2,buf,strlen(buf));
		}
	}

	//��������
	if((int)m_Curves.size()>0&&m_Cfg.TickSize>0&&
		m_Cfg.bGrid)
	{
		//�����㴹ֱ������
		SelectObject((HDC)MemDC,hGridPen2);
		for(int i=0;i<(int)m_pTimeLineWin->m_IntegralPointPos.size();i++)
		{
			pox=m_pTimeLineWin->m_IntegralPointPos[i];
			//����Խ�����
			if(pox<=DispOff) continue;
			if(pox>=DispOff+m_pTimeLineWin->m_DispWidPerWnd) break;

			pox=pox-DispOff+LeftSpaceWid;

			MoveToEx((HDC)MemDC,pox,0,NULL);
			LineTo((HDC)MemDC,pox,WndHei);
		}


		//��ˮƽ������
		SelectObject((HDC)MemDC,hGridPen);
		for(int i=0;i<(int)vecHoriGridPos.size();i++)
		{
			MoveToEx((HDC)MemDC,LeftSpaceWid,vecHoriGridPos[i],NULL);
			LineTo((HDC)MemDC,WndWid,vecHoriGridPos[i]);
		}
		
		//����ֱ�����ߣ�����m_pTimeLineWin��ʱ��ο̶Ⱥͳ��̶ȵ�λ��
		vector<int> vecVertGridPos;
		int xleft=SecOff;
		int xright=xleft+SecCnt;
		set<int>::const_iterator itlower=m_pTimeLineWin->m_Data.m_BreakScalePos.lower_bound(xleft);
		set<int>::const_iterator itupper=m_pTimeLineWin->m_Data.m_BreakScalePos.lower_bound(xright);
		for(;itlower!=m_pTimeLineWin->m_Data.m_BreakScalePos.end();itlower++)
		{
			vecVertGridPos.push_back(m_pTimeLineWin->getDispWidByUnitCnt((*itlower)-m_pTimeLineWin->m_EntireUnitOff4Disp));
			if(itlower==itupper) break;
		}

		itlower=m_pTimeLineWin->m_Data.m_LongScalePos.lower_bound(xleft);
		itupper=m_pTimeLineWin->m_Data.m_LongScalePos.lower_bound(xright);
		for(;itlower!=itupper;itlower++)
			vecVertGridPos.push_back(m_pTimeLineWin->getDispWidByUnitCnt((*itlower)-m_pTimeLineWin->m_EntireUnitOff4Disp));

		SelectObject((HDC)MemDC,hGridPen);
		for(int i=0;i<(int)vecVertGridPos.size();i++)
		{
			pox=vecVertGridPos[i];
			//����Խ�����
			if(pox<=DispOff) continue;
			if(pox>DispOff+m_pTimeLineWin->m_DispWidPerWnd) break;

			pox=pox-DispOff+LeftSpaceWid;

			MoveToEx((HDC)MemDC,pox,0,NULL);
			LineTo((HDC)MemDC,pox,WndHei);
		}
	}


    //��������
	SelectObject((HDC)MemDC,hPen);
	pox=LeftSpaceWid>0?LeftSpaceWid-1:0;
    MoveToEx((HDC)MemDC,pox,0,NULL);
    LineTo((HDC)MemDC,pox,WndHei);

	//������߽���
	if(m_WndParams[WndOrd].bDrawTopBorder)
	{
		SelectObject((HDC)MemDC,hBorderPen);
		pox=LeftSpaceWid>0?LeftSpaceWid-1:0;
		MoveToEx((HDC)MemDC,pox,0,NULL);
		LineTo((HDC)MemDC,WndWid,0);
	}


	SelectObject((HDC)MemDC,OldPen);
	SelectObject((HDC)MemDC,OldFont);
	if(hBorderPen) DeleteObject(hBorderPen);
	if(hPen) DeleteObject(hPen);
	if(hGridPen) DeleteObject(hGridPen);
	if(hGridPen2) DeleteObject(hGridPen2);
	if(hFont) DeleteObject(hFont);
}

//��ָ���ִ��ڵ�MemBmp�ϻ�����
void CCurveByTimeWin::DrawCurveInMemBmp_Internal(int hDC,int WndOrd,int WndWid,int WndHei,int DispOff,int LeftSpaceWid,int StartX,int EndX,int*pMinPox,int* pMaxPox)
{
	int MinPox=-1,MaxPox=-1;
	if(pMinPox) *pMinPox=-1;
	if(pMaxPox) *pMaxPox=-1;

	if(!m_pTimeLineWin) return;
	if(!hDC) return;
	if(WndOrd>=(int)m_WndParams.size()) return;
	if(m_WndParams[WndOrd].CurveDispRanges.size()!=m_Curves.size()) return;
	if(m_WndParams.size()!=m_pTimeLineWin->m_hWnds.size()) return;
	if(m_Curves.size()==0) return;
	if(m_Cfg.TickSize==0) return;

	HDC MemDC=CreateCompatibleDC((HDC)hDC);
	HBITMAP MemBmp=CreateCompatibleBitmap((HDC)hDC,WndWid,WndHei);
	SelectObject(MemDC,MemBmp);
	SetTextColor(MemDC,m_Cfg.NameTextCol);
	SetBkColor(MemDC,m_Cfg.BkCol);
    SetBkMode(MemDC,OPAQUE);


	//��࣬��������utc�ļ�࣬�ڻ������ͬʱ��ĵ�ʱʹ��
	int PtSpace=m_pTimeLineWin->getDispWidByUnitCnt(1);				

	//��һ����Ч��CurveOrd
	int FirstValidCurveOrd=0;
	for(FirstValidCurveOrd=0;FirstValidCurveOrd<(int)m_Curves.size();FirstValidCurveOrd++)
	{
		if((int)m_Curves[FirstValidCurveOrd].mapDataBlocks.size()>0&&m_Curves[FirstValidCurveOrd].bView)
			break;
	}

	//�����ߣ��Ӻ���ǰ��
	for(int curveord=(int)m_Curves.size()-1;curveord>=0;curveord--)
	{
		if(!m_Curves[curveord].bView) continue;
		Stru_DispRange& DispRange=m_WndParams[WndOrd].CurveDispRanges[curveord];
		if(DispRange.StartBlock==-1||DispRange.StartDataOrd==-1) continue;
		if(StartX!=-1&&m_WndParams[WndOrd].EndXDisp!=-1&&StartX>=m_WndParams[WndOrd].EndXDisp) continue;
		if(EndX!=-1&&m_WndParams[WndOrd].StartXDisp!=-1&&EndX<=m_WndParams[WndOrd].StartXDisp) continue;

		//��ʾ�������Ƶ����ֿ�ߴ�
		SIZE CurveNameTextSz;
		int CurveNameTextPox=-1,CurveNameTextPoy=-1;
		if(m_Cfg.bViewName)
		{
			CurveNameTextSz=CTools_Win32::GetTextoutSize3(MemDC,m_Curves[curveord].Name.c_str(),m_Curves[curveord].Name.size());
		}

		//��ɫ�����С����к�ƽ��
		HPEN hCurvePen=CreatePen(PS_SOLID,m_Curves[curveord].CurvePenWid,m_Curves[curveord].CurveCol);
		HPEN hCurvePen2=CreatePen(PS_SOLID,m_Curves[curveord].CurvePenWid,m_Curves[curveord].CurveCol2);
		HPEN hCurvePen0=CreatePen(PS_SOLID,m_Curves[curveord].CurvePenWid,m_Curves[curveord].CurveCol0);
		HPEN hOldPen=(HPEN)SelectObject((HDC)MemDC,hCurvePen);
		HBRUSH hPointBr=CreateSolidBrush(m_Curves[curveord].CurvePtCol);
		HBRUSH hPointBr2=CreateSolidBrush(m_Curves[curveord].CurvePtCol2);
		HBRUSH hPointBr0=CreateSolidBrush(m_Curves[curveord].CurvePtCol0);
		HBRUSH hOldBr=(HBRUSH)SelectObject((HDC)MemDC,hPointBr);

		map<int,Stru_CurveData::Stru_DataBlock>::const_iterator itlower = m_Curves[curveord].mapDataBlocks.find(DispRange.StartBlock);
		map<int,Stru_CurveData::Stru_DataBlock>::const_iterator itupper = DispRange.EndBlock==-1 ? m_Curves[curveord].mapDataBlocks.end() : m_Curves[curveord].mapDataBlocks.find(DispRange.EndBlock);
		if(itlower==m_Curves[curveord].mapDataBlocks.end()) continue;

		int startdataord=DispRange.StartDataOrd;
		assert(startdataord>=0&&startdataord<itlower->second.DataCnt);
		if(startdataord>0) startdataord--;

		int lastpox=-1,lastpoy=-1;	//����Ϊ-1��ʾ�����ϵ��ˣ���ҪMoveToEx
		int lastzerotimeutc=-1;		//���յ�ʱ����Ҫ�ж�����
		int lastval=-1;				//�����ж����л�����
		unsigned lastptcol=m_Curves[curveord].CurvePtCol0;
		while(itlower!=m_Curves[curveord].mapDataBlocks.end())
		{
			Stru_CurveData::Stru_Data *pData=itlower->second.pData;
			if(pData!=NULL)
			{
				assert(itlower!= m_Curves[curveord].mapDataBlocks.end());
				int enddataord = itlower==itupper ? DispRange.EndDataOrd : itlower->second.DataCnt;
				assert(enddataord>=0&&enddataord<=itlower->second.DataCnt);
				if(enddataord<itlower->second.DataCnt) enddataord++;
				
				//SameTimePointCnt: ����X��ͬ�ĵ��������(Ҳ����ͬһ���ڵ�����utc��ͬ�ĵ�ƽ���ֲ���һ��utc�����)
				int SameTimePtCnt;
				for(int i=startdataord;i<enddataord;i+=SameTimePtCnt)
				{
					SameTimePtCnt=1;
					
					int iX=pData[i].x;
					if(StartX!=-1&&iX<StartX) continue;
					if(EndX!=-1&&iX>=EndX) break;

					for(;i+SameTimePtCnt<enddataord&&pData[i+SameTimePtCnt].x==iX;SameTimePtCnt++);

					int unitoff=m_pTimeLineWin->m_Data.getEntireUnitOffbyX(iX);
					if(unitoff==-1) continue;

					//�ж��Ƿ����
					if(m_pTimeLineWin->m_Data.m_bTimeBased&&(lastzerotimeutc==-1||pData[i].x>=lastzerotimeutc+24*3600))
					{
						//����һ�������ĵ�
						if(m_Curves[curveord].DrawMode==Stru_CurveData::DrawMode_connectbyline&&
							lastpox!=-1&&lastpoy!=-1)
							SetPixel((HDC)MemDC,lastpox,lastpoy,m_Curves[curveord].CurvePtCol0);
						lastpox=lastpoy=lastval=-1;
						lastzerotimeutc=(int)CEasyTime(iX).getutc_timezero();
					}

					int DispPox=m_pTimeLineWin->getDispWidByUnitCnt(unitoff-m_pTimeLineWin->m_EntireUnitOff4Disp);

					int pox=DispPox-DispOff+LeftSpaceWid;

					for(int j=0;j<SameTimePtCnt;j++)
					{
						int jval=pData[j+i].y;
						int poy=WndHei-1-m_Cfg.getPixelCntByTickCnt(jval/m_Cfg.TickSize-m_WndParams[WndOrd].TickMin);
						int jpox=pox+PtSpace*j/SameTimePtCnt;

						unsigned iptcol;
						if(lastval!=-1&&jval>lastval) 
						{ 
							//����
							iptcol=m_Curves[curveord].CurvePtCol; 
							SelectObject((HDC)MemDC,hCurvePen);
							SelectObject((HDC)MemDC,hPointBr);
						}		
						else if(lastval!=-1&&jval<lastval) 
						{ 
							//����
							iptcol=m_Curves[curveord].CurvePtCol2; 
							SelectObject((HDC)MemDC,hCurvePen2);
							SelectObject((HDC)MemDC,hPointBr2);
						}	
						else 
						{ 
							//ƽ��
							iptcol=m_Curves[curveord].CurvePtCol0; 
							SelectObject((HDC)MemDC,hCurvePen0);
							SelectObject((HDC)MemDC,hPointBr0);
						}								

						if(MinPox==-1||MinPox>jpox) MinPox=jpox;
						if(MaxPox==-1||MaxPox<jpox) MaxPox=jpox;

						if(/*curveord==FirstValidCurveOrd&&*/jpox>=0&&jpox<m_WndParams[WndOrd].CntOfPox)
						{
							m_WndParams[WndOrd].pValByPox[jpox]=jval;
							m_WndParams[WndOrd].pPoyByPox[jpox]=poy;
						}

						if(m_Cfg.bViewName&&CurveNameTextPox==-1&&CurveNameTextPoy==-1)
						{
							//��������������ʾλ��
							CurveNameTextPox=jpox;
							CurveNameTextPoy=poy;
						}

						if(m_Curves[curveord].DrawMode==Stru_CurveData::DrawMode_connectbyline)
						{
							//����
							if(lastpox==-1||lastpoy==-1) 
							{
								SetPixel((HDC)MemDC,jpox,poy,lastptcol);
								MoveToEx((HDC)MemDC,jpox,poy,NULL);
							}
							else 
							{
								LineTo((HDC)MemDC,jpox,poy);
								if(lastpox>=0&&lastpoy>=0)
									SetPixel((HDC)MemDC,lastpox,lastpoy,lastptcol);
							}
						}
						else if(m_Curves[curveord].DrawMode==Stru_CurveData::DrawMode_crosspoint)
						{
							//�������
							CTools_Win32::MyLineTo2((HDC)MemDC,jpox-3,poy-3,jpox+4,poy+4);
							CTools_Win32::MyLineTo2((HDC)MemDC,jpox-3,poy-2,jpox+3,poy+4);
							CTools_Win32::MyLineTo2((HDC)MemDC,jpox-2,poy-3,jpox+4,poy+3);

							CTools_Win32::MyLineTo2((HDC)MemDC,jpox-3,poy+3,jpox+4,poy-4);
							CTools_Win32::MyLineTo2((HDC)MemDC,jpox-3,poy+2,jpox+3,poy-4);
							CTools_Win32::MyLineTo2((HDC)MemDC,jpox-2,poy+3,jpox+4,poy-3);
						}
						else if(m_Curves[curveord].DrawMode==Stru_CurveData::DrawMode_histogram)
						{
							//��ֱ��ͼ
							MoveToEx((HDC)MemDC,jpox,poy,NULL);
							LineTo((HDC)MemDC,jpox,WndHei);
						}
						else if(m_Curves[curveord].DrawMode==Stru_CurveData::DrawMode_pointonly)
						{
							//����
							if(m_Curves[curveord].CurvePenWid<=1)
								SetPixel((HDC)MemDC,jpox,poy,iptcol);
							else
							{
								int r=m_Curves[curveord].CurvePenWid/2;
								Ellipse((HDC)MemDC,jpox-r,poy-r,jpox+r,poy+r);
							}
						}

						lastpox=jpox;
						lastpoy=poy;
						lastptcol=iptcol;
						lastval=jval;
					}
				}
			}

			//��������һ�����ݿ飬��������
			if(itlower==itupper) break;

			//׼��������һ�����ݿ�
			itlower++;
			startdataord=0;
		}


		//��ʾ��������
		if(m_Cfg.bViewName&&(CurveNameTextPox!=-1||CurveNameTextPoy!=-1)&&!m_Curves[curveord].Name.empty())
		{
			TextOut(MemDC,CurveNameTextPox,CurveNameTextPoy-CurveNameTextSz.cy/2,m_Curves[curveord].Name.c_str(),m_Curves[curveord].Name.size());
		}

		SelectObject((HDC)MemDC,hOldPen);
		SelectObject((HDC)MemDC,hOldBr);
		DeleteObject(hCurvePen);
		DeleteObject(hCurvePen2);
		DeleteObject(hCurvePen0);
		DeleteObject(hPointBr);
		DeleteObject(hPointBr2);
		DeleteObject(hPointBr0);
	}

	BitBlt((HDC)hDC,LeftSpaceWid,1,WndWid-LeftSpaceWid,WndHei-1,MemDC,LeftSpaceWid,1,SRCCOPY);
	DeleteDC(MemDC);
	DeleteObject(MemBmp);

	if(pMinPox) *pMinPox=MinPox;
	if(pMaxPox) *pMaxPox=MaxPox+1;

}

//ˢ��ָ���ִ��ڵ�MemBmp
void CCurveByTimeWin::RefreshMemBmp(int hWnd)
{
	if(!m_pTimeLineWin) return;

	//WndOrd�Ǵ��ڵ����
	int WndOrd=getWndOrd(hWnd);
	if(WndOrd==-1) return;

	m_WndParams[WndOrd].ReleaseResource();

	if(m_WndParams[WndOrd].CurveDispRanges.size()!=m_Curves.size()) return;
	if(m_WndParams.size()!=m_pTimeLineWin->m_hWnds.size()) return;

	//��ȡWndHei,WndWid,DispOff,SecOff,SecCnt
	int WndHei=CTools_Win32::GetClientHei((HWND)hWnd);
	int WndWid=m_pTimeLineWin->m_WidPerWnd;
	int DispOff=m_pTimeLineWin->getDispOff2(WndOrd);
	int SecOff=m_pTimeLineWin->getEntireUnitOff(WndOrd);
	int SecCnt=m_pTimeLineWin->getUnitCnt(WndOrd);
	int LeftSpaceWid=m_pTimeLineWin->m_Cfg.LeftSpaceWid;

	HDC hDC=GetDC((HWND)hWnd);
	if(!hDC) return ;

	HDC MemDC=CreateCompatibleDC(hDC);
	if(!MemDC) { ReleaseDC((HWND)hWnd,hDC);  return; }

	m_WndParams[WndOrd].hMemBmp=(int)CreateCompatibleBitmap(hDC,WndWid,WndHei);
	if(!m_WndParams[WndOrd].hMemBmp) {  DeleteDC(MemDC);  ReleaseDC((HWND)hWnd,hDC);  return; }
	SelectObject(MemDC,(HBITMAP)m_WndParams[WndOrd].hMemBmp);

	m_WndParams[WndOrd].CntOfPox=WndWid;
	m_WndParams[WndOrd].pPoyByPox=(int*)malloc(WndWid*sizeof(int));
	m_WndParams[WndOrd].pValByPox=(int*)malloc(WndWid*sizeof(int));
	if(!m_WndParams[WndOrd].pPoyByPox||!m_WndParams[WndOrd].pValByPox) { DeleteDC(MemDC);  ReleaseDC((HWND)hWnd,hDC);  m_WndParams[WndOrd].ReleaseResource();  return; }
	memset(m_WndParams[WndOrd].pPoyByPox,0,WndWid*sizeof(int));
	memset(m_WndParams[WndOrd].pValByPox,0,WndWid*sizeof(int));


	//��䴰�ڱ���
    HBRUSH		hBrBk=CreateSolidBrush(m_Cfg.BkCol);
	RECT		Rect={0,0,WndWid,WndHei};
    FillRect(MemDC,&Rect,hBrBk);
	if(hBrBk) DeleteObject(hBrBk);

	//�������ᡢ�����ߵ�
	DrawFrameInMemBmp_Internal((int)MemDC,WndOrd,WndWid,WndHei,DispOff,SecOff,SecCnt,LeftSpaceWid);

	//������
	DrawCurveInMemBmp_Internal((int)MemDC,WndOrd,WndWid,WndHei,DispOff,LeftSpaceWid);

	DeleteDC(MemDC);
	ReleaseDC((HWND)hWnd,hDC);
}


//��ָ���ִ��ڵ�MemBmp��ˢ��ָ��ʱ�䷶Χ�ڵ�����
void CCurveByTimeWin::RefreshMemBmp_Part(int startX,int endX)
{
	if(!m_pTimeLineWin) return;
	if(m_WndParams.size()!=m_pTimeLineWin->m_hWnds.size()) return;
	if(startX>=endX) return;
	if(m_Curves.size()==0) return;

	for(int WndOrd=0;WndOrd<(int)m_WndParams.size();WndOrd++)
	{
		if(m_WndParams[WndOrd].CurveDispRanges.size()!=m_Curves.size()) continue;
		if(m_WndParams[WndOrd].EndXDisp<=startX) continue;
		if(m_WndParams[WndOrd].StartXDisp>=endX) break;
		if(!m_WndParams[WndOrd].hMemBmp||!m_WndParams[WndOrd].pPoyByPox||!m_WndParams[WndOrd].pValByPox) continue;

		HWND hWnd=(HWND)m_WndParams[WndOrd].hWnd;
		int WndHei=CTools_Win32::GetClientHei(hWnd);
		int WndWid=m_pTimeLineWin->m_WidPerWnd;
		int DispOff=m_pTimeLineWin->getDispOff2(WndOrd);
		int SecOff=m_pTimeLineWin->getEntireUnitOff(WndOrd);
		int SecCnt=m_pTimeLineWin->getUnitCnt(WndOrd);
		int LeftSpaceWid=m_pTimeLineWin->m_Cfg.LeftSpaceWid;

		HDC hDC=GetDC((HWND)hWnd);
		if(!hDC) continue;

		HDC MemDC=CreateCompatibleDC(hDC);
		if(!MemDC) { ReleaseDC((HWND)hWnd,hDC);  continue; }

		HBITMAP hMemBmp=CreateCompatibleBitmap(hDC,WndWid,WndHei);
		if(!hMemBmp) {  DeleteDC(MemDC);  ReleaseDC(hWnd,hDC);  continue; }
		SelectObject(MemDC,hMemBmp);

		//��䴰�ڱ���
		HBRUSH		hBrBk=CreateSolidBrush(m_Cfg.BkCol);
		RECT		Rect={0,0,WndWid,WndHei};
		FillRect(MemDC,&Rect,hBrBk);
		if(hBrBk) DeleteObject(hBrBk);

		//����utc��Χ
		int istartX=startX;
		if(istartX!=-1&&istartX<m_WndParams[WndOrd].StartXDisp) istartX=m_WndParams[WndOrd].StartXDisp;
		int iendX=endX;
		if(iendX!=-1&&iendX>m_WndParams[WndOrd].EndXDisp) iendX=m_WndParams[WndOrd].EndXDisp;

		//�������ᡢ�����ߵ�
		DrawFrameInMemBmp_Internal((int)MemDC,WndOrd,WndWid,WndHei,DispOff,SecOff,SecCnt,LeftSpaceWid);

		int MinPox,MaxPox;
		//������
		DrawCurveInMemBmp_Internal((int)MemDC,WndOrd,WndWid,WndHei,DispOff,LeftSpaceWid,istartX,iendX,&MinPox,&MaxPox);

		//��Ӱ�췶Χ������m_WndParams[WndOrd].hMemBmp��ȥ
		HDC MemDC2=CreateCompatibleDC(hDC);
		SelectObject(MemDC2,(HBITMAP)m_WndParams[WndOrd].hMemBmp);
		BitBlt(MemDC2,MinPox,0,MaxPox-MinPox,WndHei,MemDC,MinPox,0,SRCCOPY);
		DeleteDC(MemDC2);

		DeleteDC(MemDC);
		ReleaseDC((HWND)hWnd,hDC);
		DeleteObject(hMemBmp);

		//ˢ�´���
		SetRect(&Rect,MinPox,0,MaxPox,WndHei);
		InvalidateRect(hWnd,&Rect,false);
		SendMessage(hWnd,WM_PAINT,0,0);
	}
}


void CCurveByTimeWin::OnPaint(int hWnd,int hDC) 
{
	if(!m_pTimeLineWin) return;

	//WndOrd�Ǵ��ڵ����
	int WndOrd=getWndOrd(hWnd);
	if(WndOrd==-1) return;

	if(m_WndParams[WndOrd].CurveDispRanges.size()!=m_Curves.size()) return;
	if(m_WndParams.size()!=m_pTimeLineWin->m_hWnds.size()) return;

	//��ȡWndHei
	int WndHei=CTools_Win32::GetClientHei((HWND)hWnd);
	//��ȡWndWid
	int WndWid=m_pTimeLineWin->m_WidPerWnd;
	//��ȡLeftSpaceWid
	int LeftSpaceWid=m_pTimeLineWin->m_Cfg.LeftSpaceWid;

	HDC MemDC=CreateCompatibleDC((HDC)hDC);
	if(!MemDC) return;

	HBITMAP hMemBmp=CreateCompatibleBitmap((HDC)hDC,WndWid,WndHei);
	if(!hMemBmp) {  DeleteDC(MemDC);  return; }
	SelectObject(MemDC,hMemBmp);

    HBRUSH		hBrBk=CreateSolidBrush(m_Cfg.BkCol);
	RECT Rect={0,0,WndWid,WndHei};
    FillRect(MemDC,&Rect,hBrBk);
	DeleteObject(hBrBk);

	//�����ߺ��������´����MemDC��
	if(m_WndParams[WndOrd].hMemBmp)
	{
		HDC MemDC2=CreateCompatibleDC((HDC)hDC);
		SelectObject(MemDC2,(HBITMAP)m_WndParams[WndOrd].hMemBmp);
		BitBlt(MemDC,0,0,WndWid,WndHei,MemDC2,0,0,SRCCOPY);
		DeleteDC(MemDC2);
	}


	//��tip��
	int tipwnd=m_pTimeLineWin->m_TimeTipWnd;
	int tippos=m_pTimeLineWin->m_TimeTipPos;
	if(tipwnd==m_pTimeLineWin->m_hWnds[WndOrd]&&
		tippos>=LeftSpaceWid&&
		tippos<m_pTimeLineWin->m_DispWidPerWnd+LeftSpaceWid&&
		m_pTimeLineWin->getEntireUnitOff(WndOrd)!=-1&&
		m_pTimeLineWin->getUnitCntByDispWid(tippos-LeftSpaceWid)<m_pTimeLineWin->getUnitCnt(WndOrd))
	{
		//�ж�����Ƿ��ڴ�����
		RECT Rect;
		GetWindowRect((HWND)hWnd,&Rect);
		POINT CursorPt;
		GetCursorPos(&CursorPt);

		if(!PtInRect(&Rect,CursorPt))
		{
			//�����겻�ڴ��ڷ�Χ�ڡ����ж�����ߴ��ڣ����������������ߴ����ڡ��������ڴ����ߴ��ڵ����棬��һ���ᴩ�Ĵ�ֱ��
			if(CursorPt.y<Rect.top)
			{
				HPEN	hPen=CreatePen(PS_SOLID,1,m_Cfg.TipLineCol);
				HPEN	hOldPen=(HPEN)SelectObject(MemDC,hPen);
				int oldRop=SetROP2(MemDC,R2_XORPEN);
				MoveToEx(MemDC,tippos,0,NULL);
				LineTo(MemDC,tippos,WndHei);
				SetROP2(MemDC,oldRop);
				SelectObject(MemDC,hOldPen);
				DeleteObject(hPen);
			}
		}
		else
		{
			if(m_WndParams[WndOrd].CntOfPox==WndWid&&
				m_WndParams[WndOrd].pPoyByPox&&
				m_WndParams[WndOrd].pValByPox)
			{

				bool bdraw=false;
				int poy,val;
				if(m_WndParams[WndOrd].pValByPox[tippos]!=0||m_WndParams[WndOrd].pPoyByPox[tippos]!=0)
				{
					poy=m_WndParams[WndOrd].pPoyByPox[tippos];
					val=m_WndParams[WndOrd].pValByPox[tippos];
					bdraw=true;
				}
				else
				{
					//��ǰ�����ڶ˵��λ��
					for(int i=0;i<tippos-LeftSpaceWid;i++)
					{
						if(m_WndParams[WndOrd].pValByPox[tippos-i-1]!=0||m_WndParams[WndOrd].pPoyByPox[tippos-i-1]!=0)
						{
							poy=m_WndParams[WndOrd].pPoyByPox[tippos-i-1];
							val=m_WndParams[WndOrd].pValByPox[tippos-i-1];
							bdraw=true;
							break;
						}
					}
				}

				if(bdraw)
				{
					HBRUSH		hBrBk=CreateSolidBrush(m_Cfg.BkCol);
					HPEN		hPen=CreatePen(PS_SOLID,1,m_Cfg.TipLineCol);
					HFONT       hFont=CreateFont(-1*m_Cfg.FontSize,0,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,0,m_Cfg.FontName);

					HPEN hOldPen=(HPEN)SelectObject(MemDC,hPen);
					HFONT hOldFont=(HFONT)SelectObject(MemDC,hFont);
					HBRUSH hOldBr=(HBRUSH)SelectObject(MemDC,hBrBk);
					SetTextColor(MemDC,RGB(255,255,255));
					SetBkColor(MemDC,RGB(0,0,255));
					SetBkMode(MemDC,OPAQUE);

					int oldRop=SetROP2(MemDC,R2_XORPEN);
					//��tip��
					MoveToEx(MemDC,LeftSpaceWid+1,poy,NULL);
					LineTo(MemDC,tippos,poy);
					LineTo(MemDC,tippos,WndHei);
					SetROP2(MemDC,oldRop);

					//��עTimeTip��Ӧ������ֵ
					char buf[256];
					itoa(val,buf,10);
					SIZE Sz=CTools_Win32::GetTextoutSize3(MemDC,buf,strlen(buf));
					SetTextColor(MemDC,RGB(255,255,255));
					SetBkColor(MemDC,RGB(0,0,255));
					SetBkMode(MemDC,OPAQUE);
					TextOut(MemDC,LeftSpaceWid-4-Sz.cx,poy-Sz.cy/2,buf,strlen(buf));

					SelectObject(MemDC,hOldPen);
					SelectObject(MemDC,hOldFont);
					SelectObject(MemDC,hOldBr);
					DeleteObject(hPen);
					DeleteObject(hBrBk);
					DeleteObject(hFont);
				}
			}
		}
	}

	//��MemDC�л�����ɣ�������hDC��
	BitBlt((HDC)hDC,0,0,WndWid,WndHei,MemDC,0,0,SRCCOPY);

	DeleteDC(MemDC);
	DeleteObject(hMemBmp);
}


//��������Ŵ�һ��
void CCurveByTimeWin::zoomin(void) 
{ 
	if(m_Cfg.TickRatio2PowerN<8&&(int)m_WndParams.size()>0)
	{
		int WndHei=CTools_Win32::GetClientHei((HWND)m_WndParams[0].hWnd);
		int MaxTickCnt=m_Cfg.getTickCntByPixelCnt(WndHei);
		int FirstVisibleCurveOrd=GetFirstVisibleCurveOrd();
		if((int)m_Curves.size()>0&&FirstVisibleCurveOrd>=0&&m_Curves[FirstVisibleCurveOrd].DrawMode!=Stru_CurveData::DrawMode_histogram)
		{
			//��ֱ��ͼ������TickMin
			for(int i=0;i<(int)m_WndParams.size();i++)
				m_WndParams[i].TickMin+=MaxTickCnt/4;
		}

		m_Cfg.TickRatio2PowerN++;

		for(int i=0;i<(int)m_WndParams.size();i++)
		{
			RefreshMemBmp(m_WndParams[i].hWnd);
			repaint(m_WndParams[i].hWnd);
		}
	}
}
//����������Сһ��
void CCurveByTimeWin::zoomout(void) 
{
	if(m_Cfg.TickRatio2PowerN>-8&&(int)m_WndParams.size()>0)
	{
		int WndHei=CTools_Win32::GetClientHei((HWND)m_WndParams[0].hWnd);
		int MaxTickCnt=m_Cfg.getTickCntByPixelCnt(WndHei);
		int FirstVisibleCurveOrd=GetFirstVisibleCurveOrd();
		if((int)m_Curves.size()>0&&FirstVisibleCurveOrd>=0&&m_Curves[FirstVisibleCurveOrd].DrawMode!=Stru_CurveData::DrawMode_histogram)
		{
			//��ֱ��ͼ������TickMin
			for(int i=0;i<(int)m_WndParams.size();i++)
				m_WndParams[i].TickMin-=MaxTickCnt/2;
		}

		m_Cfg.TickRatio2PowerN--;

		for(int i=0;i<(int)m_WndParams.size();i++)
		{
			RefreshMemBmp(m_WndParams[i].hWnd);
			repaint(m_WndParams[i].hWnd);
		}
	}
}
//��������һ��ƫ����
void CCurveByTimeWin::moveup(int hWnd,int offset)
{
	//ֱ��ͼ�����ܵ���TickMin
	int FirstVisibleCurveOrd=GetFirstVisibleCurveOrd();
	if((int)m_Curves.size()==0||FirstVisibleCurveOrd<0||m_Curves[FirstVisibleCurveOrd].DrawMode==Stru_CurveData::DrawMode_histogram)
		return;

	int tickoffset=m_Cfg.getTickCntByPixelCnt(offset);
	if(tickoffset<=0) tickoffset=1;
	if(hWnd==NULL)
	{
		for(int i=0;i<(int)m_WndParams.size();i++)
		{
			m_WndParams[i].TickMin-=tickoffset;
			RefreshMemBmp(m_WndParams[i].hWnd);
			repaint(m_WndParams[i].hWnd);
		}
	}
	else
	{
		int WndOrd=getWndOrd(hWnd);
		if(WndOrd!=-1)
		{
			m_WndParams[WndOrd].TickMin-=tickoffset;
			RefreshMemBmp(m_WndParams[WndOrd].hWnd);
			repaint(m_WndParams[WndOrd].hWnd);
		}
	}
}
//��������һ��ƫ����
void CCurveByTimeWin::movedown(int hWnd,int offset)
{
	//ֱ��ͼ�����ܵ���TickMin
	int FirstVisibleCurveOrd=GetFirstVisibleCurveOrd();
	if((int)m_Curves.size()==0||FirstVisibleCurveOrd<0||m_Curves[FirstVisibleCurveOrd].DrawMode==Stru_CurveData::DrawMode_histogram)
		return;

	int tickoffset=m_Cfg.getTickCntByPixelCnt(offset);
	if(tickoffset<=0) tickoffset=1;

	if(hWnd==NULL)
	{
		for(int i=0;i<(int)m_WndParams.size();i++)
		{
			m_WndParams[i].TickMin+=tickoffset;
			RefreshMemBmp(m_WndParams[i].hWnd);
			repaint(m_WndParams[i].hWnd);
		}
	}
	else
	{
		int WndOrd=getWndOrd(hWnd);
		if(WndOrd!=-1)
		{
			m_WndParams[WndOrd].TickMin+=tickoffset;
			RefreshMemBmp(m_WndParams[WndOrd].hWnd);
			repaint(m_WndParams[WndOrd].hWnd);
		}
	}
}



