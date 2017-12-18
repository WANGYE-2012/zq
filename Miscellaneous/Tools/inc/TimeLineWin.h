// TimeLineWin.h: interface for the CTimeLineWin class.
//	ʱ���ᴦ����
//	1. һ������£�ʱ������һ����������ʾ��
//	2. ��������£���Ҫ��ʾ�ܳ���ʱ���ᣬ�����ڶ����������ʾ���ڸ�����������β������
//		��������£�һ������ڿ����ͬ�����°ڷţ���ʵ����һ����Ļ���ö��������ʾ��ʱ����
//		��AddPartWnd()��RemovePartWnd()�������Ӻ�ɾ���ִ���
//	3. ����ಢ���̰߳�ȫ
//	4. ���Ի�ʱ��̶ȣ�Ҳ���Ի���ʱ��̶ȡ���m_Cfg.bTimeBased
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMELINEWIN_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_)
#define AFX_TIMELINEWIN_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#include <time.h>
#include <map>
#include <set>
#include <vector>
using std::map;
using std::set;
using std::vector;


#include "Tools_Ansi.h"
#include "easytime.h"
#include "easytimespan.hpp"
#include "easytradingtimespan.hpp"
#include "easymutex.h"


#ifdef WIN32
#pragma managed(push,off)
#endif 





//ʱ��̶ȴ�����
class CTimeLineWin
{
public:
	//����ʱ��̶ȴ��ڵ����ò���
	struct Stru_Cfg
	{
		Stru_Cfg(unsigned bkCol=0,
			unsigned lineCol=0xff,
			unsigned fontCol=0xff,
			int fontSize=12,
			const char* fontName="������",
			int longScaleHei=6,
			int shortScaleHei=3,
			int timespanstartScaleHei=1,
			int wndHei=16,
			int leftSpaceWid=40,
			unsigned tipLineCol=0x7f0000,
			int markPos=0)
		{
			BkCol=bkCol;
			LineCol=lineCol;
			FontCol=fontCol;
			FontSize=fontSize;
			memset(FontName,0,sizeof(FontName));
			if(!fontName) strncpy(FontName,"����",sizeof(FontName)-1);
			else strncpy(FontName,fontName,sizeof(FontName)-1);
			LongScaleHei=longScaleHei;
			ShortScaleHei=shortScaleHei;
			TimespanStartScaleHei=timespanstartScaleHei;
			WndHei=wndHei;
			LeftSpaceWid=leftSpaceWid;
			MarkPos=markPos;
			TipLineCol=tipLineCol;
		}

		unsigned	BkCol;						//������ɫ
		unsigned	LineCol;					//������ɫ
		unsigned	FontCol;					//������ɫ
		int         FontSize;					//�����С������
		char        FontName[256];				//��������
		int         LongScaleHei;				//���̶��ߵĸ߶�
		int         ShortScaleHei;				//�̶̿��ߵĸ߶�
		int         TimespanStartScaleHei;		//ʱ�ο�ʼʱ��̶ȵĸ߶�
		int			WndHei;						//ʱ���ᴰ�ڸ߶�
		int			LeftSpaceWid;				//ʱ������߿հ����Ŀ�ȣ���λΪ���ء�������򻭱���������ʱ����
		int         MarkPos;					//�̶ȱ�עλ�ã�0-���ֱ�ע�ڿ̶��м䣻1-��ע���ұ�
		unsigned	TipLineCol;					//Tip�ߵ���ɫ
	};

	//����ʱ��̶ȵķ�Χ������
	struct Stru_Data
	{
	public:
		//bTimeBased: �Ƿ����ʱ�䡣���Ի�����ͨ������
		Stru_Data(bool bTimeBased=true);

		//���ʱ��̶ȵķ�Χ��m_WholePeriod��ע�⣺���������Ҫ����refresh����Ч
		void clearRange(void);
		//��������ʱ��̶ȵķ�Χ��ע�⣺���������Ҫ����refresh����Ч
		void setRange(int start,int end);
		//��������ʱ��̶ȵķ�Χ��ָ����ʼʱ�䣬����Ч�������ǽ���ʱ�ε�ʱ�䲻����Ч������ע�⣺���������Ҫ����refresh����Ч
		void setRange2(int start,int SpanUnits);
		//���¼��㣬�������ú���Ҫ������������������¼���
		void recalcu(void) { calcuPeriods_internal(); calcuScalePos_internal(); }


		//Xֵ�Ƿ��Ѿ���ʱ
		bool isovertime(int iX) const;
		//��������/��Ԫ��ƫ����ȡX���������ʼʱ��
		int getXbyEntireUnitOff(int UnitOff) const;
		//����Xȡ����/��Ԫ��ƫ�������������ʼʱ�䣬�Ƿ��򷵻�-1
		int getEntireUnitOffbyX(int iX) const;
		//ȡÿ��Ϸ�������������ڽ���ʱ�Σ�����ʱ��̶���Ч
		int getSeconds_EachDay(void) const;

	private:
		//������ʼʱ��ʹ��ڿ�ȣ�����̶ȵ�ʱ��
		void calcuPeriods_internal(void);
		//����̶���λ��
		void calcuScalePos_internal(void);


	public:
		//�������������ı����Ҫ����recalcu���¼���
		bool							m_bTimeBased;			//�Ƿ����ʱ�䡣�������ʱ�䣬Ҫ���ǽ���ʱ�εȺ�ʱ����ص�����
		CEasyTimespan<int>				m_WholePeriod;          //(ʱ��)�̶���ֹʱ��
		CEasyTradingTimespan            m_WorkingTimespan;      //����ʱ�ζ��壬��ʱ��̶���Ч

		//����������������recalcu�����õ��Ľ�����ⲿ����ʹ�ã���Ҫ�޸�
		CEasyTimespan<int>				m_Spans;				//��Ч��ֹ(ʱ��)�ļ���
		int								m_WeekendDaysInclued;	//ʱ���߰�������ĩ����������ʱ��̶���Ч
		int								m_totalunits;			//ʱ������������ָ��Ч�������ų��˷ǽ���ʱ�Ρ�����ʱ����

		//���������ʱ��̶���Ч
		int                             m_MarkUnit;             //��ע��λ����ʾ���̶ȺͶ̶̿ȵı�ע��λ��0:���곤�̶ȣ����¶̶̿�; 1:���³��̶ȣ����ն̶̿�; 2:���ճ��̶ȣ���Сʱ�̶̿�; 3:��Сʱ���̶ȣ���5���Ӷ̶̿�; 4:��5���ӳ��̶ȣ���1���Ӷ̶̿�
		set<time_t>						m_BreakScaleUTC;        //ʱ���жϵ��ʱ�伯
		set<int>						m_BreakScalePos;        //ʱ���жϵ��λ�ü��� 
		set<time_t>						m_LongScaleUTC;         //���̶ȵ�ʱ�伯   
		set<int>						m_LongScalePos;         //���̶ȵ�λ�ü��� 
		set<time_t>						m_ShortScaleUTC;        //�̶̿ȵ�ʱ�伯
		set<int>						m_ShortScalePos;        //�̶̿ȵ�λ�ü���

	private:
		//���漸���������ڱ��浱ǰ��ѯ��ʱ��Σ���getEntireUnitOffbyX()��ʹ�ã�Ŀ���ǿ��ٲ��ҡ����m_Spans
		int								m_CacheTimespanOffset;	//�ϴβ��ҵ�ʱ��ε�offset��-1��ʾ��Ч
		int								m_CacheTimespanStart;	//�ϴβ��ҵ�ʱ��ε�start��-1��ʾ��Ч
		int								m_CacheTimespanEnd;		//�ϴβ��ҵ�ʱ��ε�end��-1��ʾ��Ч

	private:
		//������������ֹʹ��
		Stru_Data(const Stru_Data& r);
		Stru_Data& operator=(const Stru_Data& r);
	};


public:
	CTimeLineWin(bool bTimeBased=true);
	virtual ~CTimeLineWin();

	//�������������winform�������Ҫʹ��
	void AddPartWnd(int hWnd,bool brefresh=true);				//����ʱ���ᴰ�ھ���������winform����ʱ���ᴰ�ڣ�������������������hWnd���
	void RemovePartWnd(int hWnd,bool brefresh=true);
	
    //���溯������������MFC/Winform�����Ĵ����е���
	//���ָ���ִ���
    void OnPaint(int hWnd,int hDC); 

	//ˢ�������Լ���ʾ������
	//brecalcu:�Ƿ���Ҫ���¼��㡣����ʱ�����򡢽���ʱ�κ���Ҫ���¼��㡣ʱ�����ݱ仯ʱ��Ӧ��Ϊtrue
	//bsetfullwnd:�Ƿ���Ҫ�����޸���������ʹ֮ȫ����ʾ��ʱ�����ݱ仯ʱ�����������仯ʱ��Ӧ��Ϊtrue
	//bgetwndwid:�Ƿ���Ҫ���»�ȡ�ִ��ڿ�ȡ����������仯ʱ���Լ����ڳߴ�仯ʱӦ��Ϊtrue
	//brepaint:�Ƿ�ˢ��ȫ���ִ���
	void refresh(bool brecalcu,bool bsetfullwnd,bool bgetwndwid,bool brepaint);


	//ˢ�´�����ʾ
	//hWndΪNULLˢ��ȫ���ִ�����ʾ������ˢ��ָ���ִ�����ʾ
	void repaint(int hWnd);


	void SetTimeTip(int hWnd,int pox);          //��ʾ�������λ�ö�Ӧʱ��tip������ڸ����ڿͻ������ƶ�ʱ����
	void ClearTimeTip(void);                    //����������λ�ö�Ӧʱ��tip������뿪������ʱ����
    //��ȡ�������ʱ��tip�ĵ�ǰλ��
    void getTimeTipPos(int& hWnd,int& tippos);

	
	bool zoomin(void);							//ʱ���ᴰ�ڷŴ�һ��
	bool zoomout(void);							//ʱ���ᴰ����Сһ��
	bool movebyleftkey(int offset);				//�������ʱ��������һ��ƫ����
	bool movebyrightkey(int offset);			//���Ҽ���ʱ��������һ��ƫ����


	//��������������Stru_TimeLineWin_Data�ĺ���������Ҫ����LeftSpaceWid��Ӱ�죬�������һ��
    //����x��λ��ȡ��Ӧʱ�䡣hWnd�Ƿִ��ھ����xPos�Ƿִ����ϵ�λ��
    bool getX(int hWnd,int xPos,int& rltX) const;
    //����ʱ��ȡx��λ�ã����utc������Чʱ����ڣ�����false
    bool getxPos(int iX,int& hWnd,int& rltPos) const;
	//����ʱ����ָ��������ȡx��λ�á����ص�rltPos����LeftSpaceWid�����utc���ڴ�����Чʱ����ڣ�����false
    bool getxPosInWnd(int iX,int WndOrd,int& rltPos) const;
	//��Unit����������ʾ���
	inline int getDispWidByUnitCnt(int UnitCnt) const
	{
		return m_ZoomFactor>=0? (UnitCnt<<m_ZoomFactor) : (UnitCnt>>(-m_ZoomFactor));
	}
	//����ʾ��ȼ���Unit����
	inline int getUnitCntByDispWid(int DispWid) const 
	{
		return m_ZoomFactor>=0? (DispWid>>m_ZoomFactor) : (DispWid<<(-m_ZoomFactor));
	}
	//ȡĳ���ִ��ڵ���ţ�0��ʼ��ʧ�ܷ���-1
	inline int getWndOrd(int hWnd) const
	{
		for(int WndOrd=0;WndOrd<(int)m_hWnds.size();WndOrd++) if(m_hWnds[WndOrd]==hWnd) return WndOrd;
		return -1;
	}
	//ȡĳ���ִ��ڵ�DispOff��������LeftSpaceWid��ʧ�ܷ���-1
	inline int getDispOff(int hWnd) const
	{
		int WndOrd=getWndOrd(hWnd);
		if(WndOrd==-1) return -1;
		return WndOrd*m_DispWidPerWnd;
	}
	//ȡĳ���ִ��ڵ�DispOff��������LeftSpaceWid��ʧ�ܷ���-1
	inline int getDispOff2(int wndord) const
	{
		if(wndord<0||wndord>=(int)m_hWnds.size()) return -1;
		return wndord*m_DispWidPerWnd;
	}
	//ȡĳ���ִ��������������UnitOff
	inline int getEntireUnitOff(int wndord) const
	{
		if(wndord<0||wndord>=(int)m_hWnds.size()) return -1;
		return wndord*m_MaxUnitPerWnd>=m_EntireUnitCnt4Disp ? -1 : m_EntireUnitOff4Disp+wndord*m_MaxUnitPerWnd;
	}
	//ȡĳ���ִ��ڵ�UnitCnt���������һ���ִ��ڣ���ʵ��UnitCnt; ����֮ǰ�Ĵ��ڣ���m_MaxUnitPerWnd
	inline int getUnitCnt(int wndord) const
	{
		if(wndord<0||wndord>=(int)m_hWnds.size()) return 0;
		int unitoff=wndord*m_MaxUnitPerWnd;
		if(unitoff>=m_EntireUnitCnt4Disp) return 0;
		return  m_EntireUnitCnt4Disp-unitoff>=m_MaxUnitPerWnd ? m_MaxUnitPerWnd : m_EntireUnitCnt4Disp-unitoff;
	}

private:
	//���ָ���ִ��ڣ���MemDC��Paint
	void PaintOnMemDC_Internal(int hWnd,int MemDC);

	//�淶��m_Off4Disp��m_Wid4Disp������ȡ��֮�����
	void RegularOff4Disp_Internal(void);					
	//��ȡָ���ִ��ڵĲ�����WndWid:�ִ��ڿ�ȣ�DispOff:��ʾ��ƫ�ƣ�UnitOff:����������/��Ԫ��ƫ�ƣ�UnitCnt�Ǳ�����Ҫ��ʾ������/��Ԫ��
	bool GetDispOff_SecOff_SecCnt(int wndord,int& WndWid,int& DispOff,int& UnitOff,int& UnitCnt);


public:

	Stru_Cfg						m_Cfg;
	Stru_Data						m_Data;
	
	//���漸�������������������ۺ���������
	int								m_WidPerWnd;			//ÿ�����ڵĿ�ȡ����д��ڿ��һ��
	int								m_MaxUnitPerWnd;		//ÿ����������ʾ�������/��Ԫ��
	int								m_DispWidPerWnd;		//ÿ����������ʾʱ��Ŀ�ȣ�=WndWid-LeftSpaceWid-���������Ĳ���=(m_MaxUnitPerWnd>>m_ZoomFactor)
	int								m_EntireUnitOff4Disp;	//��ʾ������ʼλ����ʱ�����ϵ�λ�ã���Ϊ��λ��
	int								m_EntireUnitCnt4Disp;	//��ǰ��ʾ��ʱ����Ŀ�ȣ���Ϊ��λ

	vector<int>						m_hWnds;				//ʱ��̶ȴ��ھ������ÿ�����ڵĳߴ�һ����ͬ


public:
	int								m_ZoomFactor;			//�������ӣ�2��N�η��������Ŵ󣬸�����С

	int								m_TimeTipWnd;			//���tip���ڵķִ��ھ��
	int								m_TimeTipPos;			//���tip���ڴ��ڵ�λ�á��ڴ�λ����ʾ�������ʱ�䡣-1Ϊ����ʾ������ڷִ���

	vector<int>						m_IntegralPointPos;		//��ʾ��Χ�ڣ������ߵ�λ�á��������32������ʱ��Ч�������m_MemBmp��������LeftSpace
	vector<time_t>					m_IntegralPointX;		//��ʾ��Χ�ڣ������ߵ�X��
	int								m_IntegralPointUnitSpace;//�����ߵļ��������������1��5��15��30��60��300��900��1800��3600

private:
	int								m_MemBmp;				//��ϴ��ڵ�membmp��ע�⣬������LeftSpace

};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_TIMELINE_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_)
