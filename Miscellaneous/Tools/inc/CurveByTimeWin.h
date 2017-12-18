// CurveByTimeWin.h: interface for the CCurveByTimeWin class.
//
//	��ʾ����ʱ��仯�����ߵĴ�����
//		1. �������Ҫ������TimeLineWin��
//		2. Ŀǰֻ֧��int�͵��������ͣ�����С�䶯��λΪ������Ʒ�֡�au��IF����С�䶯��λΪС������֧��
//		3. ��ͼʱ��ʹ��m_pTimeLineWin��m_WidPerWnd����������ÿ�����ڵĿ�Ⱥ��ܿ��
//		4. ���ڳߴ�仯ʱ���ϲ㴰��Ӧ�ñ�֤��ִ��TimeLineWin��OnSize�����ٵ���CurveByTimeWin��RefreshMemBmp()��RePaint()
//		5. m_hWnds�����ߴ��ڼ�
//		6. �ϲ㴰�ڴ������ߵ�˳��Ϊ
//			a. ����TimeLineWin
//			b. ����CurveByTimeWin
//			c. ����pCurveByTimeWin->SetTimeLineWin(pCurveByTimeWin)
//			d. pTimeLineWin->AddWnd��pCurveByTimeWin->AddWnd������ִ�С�һ���������TimeLineWin���ڣ������CurveByTimeWin����
//			e. �������ߴ��ں�ʱ���ᴰ�ڵ�Z��AdjustZOrder()
//			f. �������
//			g. ˢ����ʾrefresh(true,true)
//
//////////////////////////////////////////////////////////////////////

#ifndef CURVEBYTIMEWIN_H_
#define CURVEBYTIMEWIN_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#include <time.h>
#include <map>
#include <set>
#include <vector>
#include <string>
using std::map;
using std::set;
using std::vector;
using std::string;


#include "Tools_Ansi.h"
#include "easytime.h"
#include "easytimespan.hpp"
#include "easytradingtimespan.hpp"
#include "easymutex.h"
#include "TimeLineWin.h"


#ifdef WIN32
#pragma managed(push,off)
#endif 


//����ʱ������ߴ�����
class CCurveByTimeWin
{
public:
	//�������ߴ��ڵ����ò���
	struct Stru_Cfg
	{
		Stru_Cfg(unsigned bkCol=0,
			unsigned lineCol=0xff,
			unsigned fontCol=0xff,
			int fontSize=12,
			const char* fontName="������",
			int scaleHei=6,
			int tickSize=1,
			int markSpacing=50,
			bool bgrid=true,
			unsigned gridCol=0x1f,
			unsigned tipLineCol=0x7f0000,
			unsigned nameTextCol=0xffffff
			)
		: TickRatio2PowerN(0)
		{
			BkCol=bkCol;
			LineCol=lineCol;
			FontCol=fontCol;
			FontSize=fontSize;
			memset(FontName,0,sizeof(FontName));
			if(!fontName) strncpy(FontName,"����",sizeof(FontName)-1);
			else strncpy(FontName,fontName,sizeof(FontName)-1);
			ScaleHei=scaleHei;
			TickSize=tickSize;
			MarkSpacing=markSpacing;
			bGrid=bgrid;
			GridCol=gridCol;
			TipLineCol=tipLineCol;
			NameTextCol=nameTextCol;
			bViewName=false;
		}
		Stru_Cfg(const Stru_Cfg& r) { *this=r; }
		inline int getTickCntByPixelCnt(int PixelCnt)
		{
			if(TickRatio2PowerN>=0) return (PixelCnt>>TickRatio2PowerN);
			else return (PixelCnt<<(-TickRatio2PowerN));
		}
		inline int getPixelCntByTickCnt(int TickCnt)
		{
			if(TickRatio2PowerN>=0) return (TickCnt<<TickRatio2PowerN);
			else return (TickCnt>>(-TickRatio2PowerN));
		}
		inline int getPixelCntByValue(int Val)
		{
			if(!TickSize) return 0;
			if(Val<0) return 0;
			int ticks=Val/TickSize;
			int remainder=Val%TickSize;
			int Pixels=getPixelCntByTickCnt(ticks);
			if(remainder!=0)
			{
				//�Ŵ�100��ȡ��������
				Pixels+=getPixelCntByTickCnt(remainder*100/TickSize)/100;
			}
			return Pixels;
		}

		unsigned	BkCol;									//������ɫ
		unsigned	LineCol;								//������ɫ
		unsigned	FontCol;								//������ɫ
		int         FontSize;								//�����С������
		char        FontName[256];							//��������
		int         ScaleHei;								//������̶��ߵĸ߶�
		int			TickSize;								//���ݵ���С�䶯��λ
		int			TickRatio2PowerN;						//ÿ��tick����ʾ���ʣ���ʾ�ö��ٸ����ر�ʾһ��tick����2��N�η���ʾ(-8 - 8)��2^0=1; 2^1=2; 2^2=4;2^=8;...2^-1=0.5; 2^-2=0.25
		int			MarkSpacing;							//��ע���
		bool		bGrid;									//�Ƿ���Ҫ��������
		unsigned	GridCol;								//��������ɫ
		unsigned	TipLineCol;								//Tip�ߵ���ɫ
		unsigned	NameTextCol;							//���������ı���ɫ
		bool		bViewName;								//�Ƿ���ʾ�������ơ���ʾ��ÿ���ִ��ڵ����߿�ʼ��
	};

	//����һ�����ߵ�����
	struct Stru_CurveData
	{
	public:
		//��������ö��
		enum eDrawMode
		{
			DrawMode_connectbyline=0,		//��ֱ�����Ӹ�������
			DrawMode_pointonly,				//ֻ������
			DrawMode_crosspoint,			//�������
			DrawMode_histogram				//��ֱ��ͼ
		};
		//��������
		enum eCurveDataType
		{
			CurveDataType_Other=0,			//������������
			CurveDataType_LastPrice,		//���¼���������
			CurveDataType_BidPrice,			//�����������
			CurveDataType_AskPrice,			//������������
			CurveDataType_BuyPoint,			//�����������
			CurveDataType_SellPoint,		//������������
			CurveDataType_Average1,			//����1
			CurveDataType_Average2,			//����2
			CurveDataType_Average3,			//����3
			CurveDataType_Average4,			//����4
			CurveDataType_WeightedAverage1,	//��Ȩ����1
			CurveDataType_WeightedAverage2,	//��Ȩ����2
			CurveDataType_WeightedAverage3,	//��Ȩ����3
			CurveDataType_WeightedAverage4,	//��Ȩ����4
			CurveDataType_Performance,		//��Ч��������
			CurveDataType_Volume			//�ɽ�����������
		};

		//�����ݽṹ��xһ����ʱ�䣬yΪ��Ӧ�����ݣ���۸�
		struct Stru_Data
		{
			Stru_Data() : x(0),y(0) {}
			Stru_Data(int ix,int iy) : x(ix),y(iy) {}

			int x,y;
		};

		//�����������ݿ顣һ�����ߵ������ɶ�����ݿ���ɡ����ݿ�֮�䰴��ʼʱ�������໥������
		//ÿ�����ݿ��ɰ�ʱ������ĵ��������
		struct Stru_DataBlock
		{
			Stru_DataBlock(): pData(NULL),MaxData(0),DataCnt(0),bFromDB(true) {}
			Stru_DataBlock(int maxData,bool bfromDB)
			{
				new(this) Stru_DataBlock(NULL,maxData,0,bfromDB);
			}
			Stru_DataBlock(const Stru_DataBlock& r)
			{
				new(this) Stru_DataBlock(r.pData,r.MaxData,r.DataCnt,r.bFromDB);
			}
			~Stru_DataBlock()
			{
				if(pData) { free(pData);  pData=NULL; }
			}
			Stru_DataBlock& operator= (const Stru_DataBlock& r)
			{
				if(pData) { free(pData);  pData=NULL; }

				new(this) Stru_DataBlock(r.pData,r.MaxData,r.DataCnt,r.bFromDB);
				return *this;
			}
			bool add(const Stru_Data& idata)
			{
				if(DataCnt>=MaxData) return false;
				pData[DataCnt++]=idata;
				return true;
			}
			void clear(void) { DataCnt=0; }

			Stru_Data*	pData;
			int			MaxData;
			int			DataCnt;
			bool		bFromDB;		//true: data from DB;  false: data from realtime quotation

		private:
			Stru_DataBlock(const Stru_Data* pdata,int maxData,int dataCnt,bool bfromDB)
			{
				new(this) Stru_DataBlock();

				MaxData=maxData;
				DataCnt=dataCnt;
				if(MaxData<DataCnt) MaxData=DataCnt;
				pData=(Stru_Data*)malloc(MaxData*sizeof(Stru_Data));
				if(pData&&pdata&&DataCnt>0) memcpy(pData,pdata,DataCnt*sizeof(Stru_Data));
				else DataCnt=0;
				bFromDB=bfromDB;
			}

		};


		Stru_CurveData(eCurveDataType dataType,eDrawMode drawMode=DrawMode_connectbyline,
			int curvePenWid=1,
			unsigned curveCol=0x7f,unsigned curvePtCol=0xff,
			unsigned curveCol2=0x7f,unsigned curvePtCol2=0xff,
			unsigned curveCol0=0x7f,unsigned curvePtCol0=0xff)
		:	bView(true),
			DataType(dataType),
			DrawMode(drawMode),
			CurvePenWid(curvePenWid),
			CurveCol(curveCol),
			CurvePtCol(curvePtCol),
			CurveCol2(curveCol2),
			CurvePtCol2(curvePtCol2),
			CurveCol0(curveCol0),
			CurvePtCol0(curvePtCol0)
		{
		}
		~Stru_CurveData()
		{
		}
		Stru_CurveData(const Stru_CurveData& r)
		{
			*this=r;
		}
		Stru_CurveData& operator= (const Stru_CurveData& r)
		{
			Name=r.Name;
			mapDataBlocks=r.mapDataBlocks;
			bView=r.bView;
			DataType=r.DataType;
			DrawMode=r.DrawMode;
			CurvePenWid=r.CurvePenWid;
			CurveCol=r.CurveCol;
			CurvePtCol=r.CurvePtCol;
			CurveCol2=r.CurveCol2;
			CurvePtCol2=r.CurvePtCol2;
			CurveCol0=r.CurveCol0;
			CurvePtCol0=r.CurvePtCol0;
			return *this;
		}
		int GetPointCnt(void)
		{
			int rlt=0;
			for(map<int,Stru_DataBlock>::const_iterator it=mapDataBlocks.begin();it!=mapDataBlocks.end();it++)
				rlt+=it->second.DataCnt;
			return rlt;
		}
		bool GetLastData(Stru_Data& rlt)
		{
			map<int,Stru_DataBlock>::const_iterator it=mapDataBlocks.end();
			while(it!=mapDataBlocks.begin())
			{
				it--;
				if(it->second.DataCnt>0) { rlt=it->second.pData[it->second.DataCnt-1];  return true; }
			}
			return false;
		}
		bool GetMinX(int& MinX)
		{
			if((int)mapDataBlocks.size()==0) return false;
			MinX=mapDataBlocks.begin()->first;
			return true;
		}
		bool GetMaxX(int& MaxX)
		{
			map<int,Stru_DataBlock>::const_iterator it=mapDataBlocks.end();
			while(it!=mapDataBlocks.begin())
			{
				it--;
				if(it->second.DataCnt>0) { MaxX=it->second.pData[it->second.DataCnt-1].x;  return true; }
			}
			return false;
		}
		void add(const Stru_Data& data,bool bForceNewBlock,bool bFromDB);
		void setName(const string& name) { Name=name; }
		string getName(void) { return Name; }


		//���ݿ�ļ��ϣ�firstTime -> DataBlock
		//1. ���ݿ��ڲ����ݰ�ʱ������
		//2. ���ݿ鰴��������ʱ������
		//3. ��ͬ���ݿ��ʱ�䲻����
		string						Name;
		map<int,Stru_DataBlock>		mapDataBlocks;			//�������ݿ�, map startx to datablock
		bool						bView;					//�Ƿ���ʾ
		eCurveDataType				DataType;				//������������
		eDrawMode					DrawMode;				//���߻���ģʽ
		int							CurvePenWid;			//���߻��ʿ�ȣ�����ģʽ��Ч
		unsigned					CurveCol;				//����������ɫ
		unsigned					CurvePtCol;				//�������߶˵���ɫ
		unsigned					CurveCol2;				//����������ɫ
		unsigned					CurvePtCol2;			//�������߶˵���ɫ
		unsigned					CurveCol0;				//ƽ��������ɫ
		unsigned					CurvePtCol0;			//ƽ�����߶˵���ɫ

		static const int			blocksz=65536;
	};


	//����ÿһ��������һ���ִ����е���ʾ��Χ����Ӧ��m_pTimeLineWin->m_StartUTC4Disp��m_pTimeLineWin->m_EndUTC4Disp
	struct Stru_DispRange
	{
		Stru_DispRange() { clear(); };
		void clear(void) { StartBlock=StartDataOrd=EndBlock=EndDataOrd=-1; };

		int StartBlock;										//m_StartUTC4Disp��Ӧ�����ݿ����ʼx
		int StartDataOrd;									//m_StartUTC4Disp��Ӧ�����ݿ�������������
		int EndBlock;										//m_EndUTC4Disp��Ӧ�����ݿ����ʼx��������-1
		int EndDataOrd;										//m_EndUTC4Disp��Ӧ�����ݿ������������ţ�������-1
	};

	//ÿ���ִ��ڵ���ʾ������������ߴ��ڷ�N�㣬��ÿ�㴰�ڶ���M���ִ���
	struct Stru_PartWndParam
	{
		Stru_PartWndParam();
		Stru_PartWndParam(int hwnd,bool bdrawTopBorder,unsigned topBorderCol);
		Stru_PartWndParam(const CCurveByTimeWin::Stru_PartWndParam& r);
		~Stru_PartWndParam();
		Stru_PartWndParam& operator= (const Stru_PartWndParam& r);
		void ReleaseResource(void);

		int							hWnd;					//���ߴ��ھ��
		bool						bDrawTopBorder;			//�Ƿ�Ҫ�����ϵı߽硣���ڵڶ������ߴ��ڣ�Ӧ�û�һ�����ϵı߽���
		unsigned					TopBorderCol;			//���ϱ߽��ߵ���ɫ
		int							TickMin;				//���ڵ�tickminֵ
		int							hMemBmp;				//���ڵ�MemBmp������LeftSpaceWid
		int*						pValByPox;				//Pox��Ӧ��Yֵ����pValByPox=(int*)malloc(CntOfPox*sizeof(int))��ʽ���롣pValByPox!=0&&pPoyByPox!=0Ϊ��Ч
		int*						pPoyByPox;				//Pox��Ӧ��Poyֵ����pPoyByPox=(int*)malloc(CntOfPox*sizeof(int))��ʽ����
		int							CntOfPox;				//Pox��������Ҳ���Ǵ��ڿ�ȣ�����LeftSpaceWid
		int							StartXDisp;				//��ʾ��Χ��ʼ��Ӧ��utc���������Ƿ������ݣ�ֻ����m_pTimeLineWin�ķ�Χ
		int							EndXDisp;				//��ʾ��Χ������Ӧ��utc���������Ƿ������ݣ�ֻ����m_pTimeLineWin�ķ�Χ
		vector<Stru_DispRange>		CurveDispRanges;		//����ÿ�����ߵ���ʾ��Χ����Ӧ��m_pTimeLineWin�ִ��ڵ�EntireUnitOff()��UnitCnt
	};

public:
	CCurveByTimeWin();
	virtual ~CCurveByTimeWin();

	//�������������winform�������Ҫʹ��
	void SetTimeLineWin(CTimeLineWin* pTimeLineWin) { m_pTimeLineWin=pTimeLineWin; }
	void AddWnd(int hWnd,bool bdrawTopBorder,unsigned topBorderCol,bool brefresh=true);									//�������ߴ��ھ���������winform�������ߴ��ڣ��������������������hWnd���
	void RemoveWnd(int hWnd,bool brefresh=true);
	//�������ߴ��ں�ʱ���ᴰ�ڵ�z�򣬱�֤pTimeLineWin��ˢ��
	void AdjustZOrder(void);

	//ˢ�´�����ʾ
	//hWndΪNULLˢ��ȫ���ִ�����ʾ������ˢ��ָ���ִ�����ʾ
	void repaint(int hWnd);
	
	//ˢ�´�����ʾ����
	//breset: ��ȡ��ʾ��Χ�����ű�����ԭ��0�����¼���������ڵ�TickMin(ʹ�ִ��ڵ�һ�����ݵ��ڴ����м�)
	//brepaint: ˢ��ȫ���ִ���
	void refresh(bool bresetRatio,bool bresetTickMin,bool brefreshmembmp,bool brepaint);

private:
	//��ָ���ִ��ڵ�MemBmp�ϻ������������ᡢ������
	void DrawFrameInMemBmp_Internal(int MemDC,int WndOrd,int WndWid,int WndHei,int DispOff,int SecOff,int SecCnt,int LeftSpaceWid);
	//��ָ���ִ��ڵ�MemBmp�ϻ�����
	void DrawCurveInMemBmp_Internal(int MemDC,int WndOrd,int WndWid,int WndHei,int DispOff,int LeftSpaceWid,int StartX=-1,int EndX=-1,int*pMinPox=NULL,int* pMaxPox=NULL);

public:
	//ˢ��ָ���ִ��ڵ�MemBmp
	void RefreshMemBmp(int hWnd);
	//��ָ���ִ��ڵ�MemBmp��ˢ��ָ��ʱ�䷶Χ�ڵ�����
	void RefreshMemBmp_Part(int startX,int endX);
    void OnPaint(int hWnd,int hDC); 

	void zoomin(void);										//��������Ŵ�һ��
	void zoomout(void);										//����������Сһ��
	void moveup(int hWnd,int offset);						//��������һ��ƫ���������hWnd==NULL�����зִ��ڶ�����
	void movedown(int hWnd,int offset);						//��������һ��ƫ���������hWnd==NULL�����зִ��ڶ�����

	//ȡĳ���ִ��ڵ���ţ�0��ʼ��ʧ�ܷ���-1
	inline int getWndOrd(int hWnd) const
	{
		int WndOrd;
		for(WndOrd=0;WndOrd<(int)m_WndParams.size()&&m_WndParams[WndOrd].hWnd!=hWnd;WndOrd++);
		if(WndOrd<(int)m_WndParams.size()) return WndOrd;
		else return -1;
	}
	//ȡX���ڷִ��ڵ���ţ�0��ʼ��ʧ�ܷ���-1
	inline int getWndOrdByX(int X) const
	{
		int WndOrd;
		if(m_WndParams.size()==0||m_WndParams[0].CurveDispRanges.size()==0) return -1;
		for(WndOrd=0;WndOrd<(int)m_WndParams.size();WndOrd++)
			if(m_WndParams[WndOrd].StartXDisp<=X&&m_WndParams[WndOrd].EndXDisp>X) return WndOrd;
		return -1;
	}

	//���������ߵ����һ��������Ƚϣ�ȡ��С��x��ʧ�ܷ���-1
	int GetMinLastX_AllCurve(void)
	{
		int minx=-1;
		Stru_CurveData::Stru_Data LastData;
		for(int i=0;i<(int)m_Curves.size();i++)
		{
			if(m_Curves[i].GetLastData(LastData)&&(minx==-1||minx>LastData.x))
				minx=LastData.x;
		}
		return minx;
	}

	int GetFirstVisibleCurveOrd(void);

private:

	//��ȡÿ�����ߵ���ʾ��Χ,m_CurveDispRanges
	void GetCurveDispRange(void);
	//����ʼ����������ִ��ڵ�TickMin(ʹ�ִ��ڵ�һ�����ݵ��ڴ����м�)
	void ResetTickMins(void);
	//��ȡ��һ�����ڵ���ָ��UTC������λ�ã�ʧ��ʱ���߶�����-1
	void GetFirstDataPositionByX(int iX,int curveord,int& blockord,int& dataord);
	//��ȡ��һ�����ڵ���ָ��UTC������
	bool GetFirstDataByX(int iX,int curveord,Stru_CurveData::Stru_Data& data);

public:
	Stru_Cfg						m_Cfg;					//���ߵ����ò���

	CTimeLineWin*					m_pTimeLineWin;			//��Ӧ��ʱ���ᴰ�ڵ�ָ�롣Ҫ��֤�ڶ����������ڣ�m_pTimeLineWin����Ч��

	vector<Stru_CurveData>			m_Curves;				//��������ָ�롣�п����ж������ߡ���ÿ�����ߣ���push_backһ���յ����ߣ������ø���������

	vector<Stru_PartWndParam>		m_WndParams;

	Ceasymutex				        m_mutex;

private:
    //������������ֹʹ��
    CCurveByTimeWin(const CCurveByTimeWin& r);
    CCurveByTimeWin& operator=(const CCurveByTimeWin& r);
};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(CURVEBYTIMEWIN_H_)
