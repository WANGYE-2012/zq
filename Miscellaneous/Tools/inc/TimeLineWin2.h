// TimeLineWin.h: interface for the CTimeLineWin2 class.
//	Ϊ����ԭ��������ݡ��Ժ󽫷���
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


#include "Tools_Ansi.h"
#include "easytime.h"
#include "easytimespan.hpp"
#include "easytradingtimespan.hpp"
#include "easymutex.h"


#ifdef WIN32
#pragma managed(push,off)
#endif 


#define ClsName_TimeLineWin2 "cls_TimeLineWin2"


//����ʱ��̶ȴ��ڵ����ò���
struct Stru_TimeLineWin2_Cfg
{
    Stru_TimeLineWin2_Cfg(int markPos=0,
                            unsigned bkCol=0,
                            unsigned lineCol=0xff,
                            unsigned fontCol=0xff,
                            int fontSize=12,
                            const char* fontName="������",
                            int longScaleHei=6,
                            int shortScaleHei=4,
                            int timespanstartScaleHei=2,
							int wndHei=16,
							int leftSpaceWid=0)
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
	}
    Stru_TimeLineWin2_Cfg(const Stru_TimeLineWin2_Cfg& r)
	{
		setCfg(r);
	}
    ~Stru_TimeLineWin2_Cfg()
	{
	}

	void setCfg(const Stru_TimeLineWin2_Cfg& r)
	{
		MarkPos=r.MarkPos;
		BkCol=r.BkCol;
		LineCol=r.LineCol;
		FontSize=r.FontSize;
		memcpy(FontName,r.FontName,sizeof(FontName));
		FontCol=r.FontCol;
		LongScaleHei=r.LongScaleHei;
		ShortScaleHei=r.ShortScaleHei;
		TimespanStartScaleHei=r.TimespanStartScaleHei;
		WndHei=r.WndHei;
        LeftSpaceWid=r.LeftSpaceWid;
	}
    const Stru_TimeLineWin2_Cfg& getCfg(void) { return *this; }


    unsigned	BkCol;					//������ɫ
    unsigned	LineCol;				//������ɫ
    unsigned	FontCol;				//������ɫ
    int         FontSize;				//�����С������
    char        FontName[256];			//��������
    int         LongScaleHei;			//���̶��ߵĸ߶�
    int         ShortScaleHei;			//�̶̿��ߵĸ߶�
    int         TimespanStartScaleHei;	//ʱ�ο�ʼʱ��̶ȵĸ߶�
	int			WndHei;					//ʱ���ᴰ�ڸ߶�
	int			LeftSpaceWid;			//ʱ������߿հ����Ŀ�ȣ���λΪ���ء�������򻭱���������ʱ����

    int         MarkPos;				//�̶ȱ�עλ�ã�0-���ֱ�ע�ڿ̶��м䣻1-��ע���ұ�
};


//����ʱ��̶ȵķ�Χ
struct Stru_TimeLineWin2_Data
{
public:
    Stru_TimeLineWin2_Data(int TimeLineWid=0,
                            const CEasyTimespan<time_t>& WholePeriod=CEasyTimespan<time_t>(),
                            const CEasyTradingTimespan& TradingTimespan=CEasyTradingTimespan()
                            );

    //��������ʱ��̶ȵķ�Χ
    void setRange(time_t start,time_t end);
    //��������ʱ��̶ȵķ�Χ��ָ����ʼʱ�䣬����Ч�������ǽ���ʱ�ε�ʱ�䲻����Ч����
    void setRange2(time_t StartUTC,int SpanSeconds);
    //�������ô��ڿ��
    void setTimeLineWid(int TimeLineWid) { m_TimeLineWid=TimeLineWid; }
    //���ý���ʱ�����
    void setTradingTimespan(const CEasyTradingTimespan& TradingTimespan) { m_TradingTimespan=TradingTimespan; }
    //���¼��㣬�������ú���Ҫ������������������¼���
    void refresh(void) { calcuPeriods_internal(); calcuScalePos_internal(); }


	//utc�Ƿ��Ѿ���ʱ
	bool isovertime(time_t utc) const;
    //����x��λ��ȡ��Ӧʱ��
    time_t getUTC(int xPos) const;
    //����ʱ��ȡx��λ��
    int getxPos(time_t utc) const;
    //��ȡ�̶ȵ�ʱ�伯
	void getScale(std::set<time_t>& longScale,std::set<time_t>& shortScale) const;
    //��ȡ���ڿ��
    int getTimeLineWid(void ) { return m_TimeLineWid; }

	//ȡÿ��Ϸ�������
	int getSeconds_EachDay(void) const;

    //��ȡ���ò���
    const Stru_TimeLineWin2_Data& getData(void) { return *this; }

private:
    //������ʼʱ��ʹ��ڿ�ȣ�����̶ȵ�ʱ��
    void calcuPeriods_internal(void);
    //����̶���λ��
    void calcuScalePos_internal(void);


public:

    int                             m_TimeLineWid;          //���ڿ��

    CEasyTradingTimespan            m_TradingTimespan;      //����ʱ�ζ���

    CEasyTimespan<time_t>           m_WholePeriod;          //ʱ��̶���ֹʱ��
    CEasyTimespan<time_t>           m_TimeSpans;            //��Ч��ֹʱ��ļ���
	
	int								m_WeekendDaysInclued;	//��������ĩ������

	int								m_totalsecs;			//��������ָ��Ч����������ʱ����

    int                             m_MarkUnit;             //��ע��λ��0:��hh��ע��1:��yyyy-mm-dd��ע

	std::set<time_t>                m_BreakScaleUTC;        //ʱ���жϵ��ʱ�伯   
	std::set<int>                   m_BreakScalePos;        //ʱ���жϵ��λ�ü��� 

    std::set<time_t>                m_LongScaleUTC;         //���̶ȵ�ʱ�伯   
    std::set<int>                   m_LongScalePos;         //���̶ȵ�λ�ü��� 

    std::set<time_t>                m_ShortScaleUTC;        //�̶̿ȵ�ʱ�伯
    std::set<int>                   m_ShortScalePos;        //�̶̿ȵ�λ�ü���

private:
    //������������ֹʹ��
    Stru_TimeLineWin2_Data(const Stru_TimeLineWin2_Data& r);
    Stru_TimeLineWin2_Data& operator=(const Stru_TimeLineWin2_Data& r);
};


//ʱ��̶ȴ�����
class CTimeLineWin2 : public Stru_TimeLineWin2_Cfg,public Stru_TimeLineWin2_Data
{
public:
	CTimeLineWin2(const Stru_TimeLineWin2_Cfg& cfg=Stru_TimeLineWin2_Cfg());
	virtual ~CTimeLineWin2();

	//��������������win32�������Ҫʹ��
	static void RegistWndClass(void);			//ע�ᴰ���ࡣ��������ǰ�����ȵ��ô˺���ע�ᴰ����
	static void UnregistWndClass(void);			//ע��������
	int CreateWnd(int hParent);					//�ڸ������д���ʱ��̶ȴ���

	//�������������winform�������Ҫʹ��
	void SetWnd(int hWnd);						//����ʱ���ᴰ�ھ���������winform����ʱ���ᴰ�ڣ��������������������hWnd���

    //��������������������MFC/Winform�����Ĵ����е���
    virtual void OnSize(int iWnd);
    virtual void OnPaint(int iWnd); 
    virtual void OnPaint2(int iWnd,int hDC); 

    //��������������Stru_TimeLineWin2_Data�ĺ���������Ҫ����LeftSpaceWid��Ӱ�죬�������һ��
    //����x��λ��ȡ��Ӧʱ��
    time_t getUTC(int xPos) const { return Stru_TimeLineWin2_Data::getUTC(xPos-LeftSpaceWid); }
    //����ʱ��ȡx��λ��
    int getxPos(time_t utc) const { return Stru_TimeLineWin2_Data::getxPos(utc)+LeftSpaceWid; }


	void SetTimeTip(int pox);                   //��ʾ�������λ�ö�Ӧʱ��tip������ڸ����ڿͻ������ƶ�ʱ����
	void ClearTimeTip(void);                    //����������λ�ö�Ӧʱ��tip������뿪������ʱ����


    //��ȡ�������ʱ��tip�ĵ�ǰλ��
    int& getTimeTipPos(void) { return m_TimeTipPos; }

    int								m_hWnd;					//ʱ��̶ȴ��ھ��
	static Ceasymutex				m_mutex;				//ͬ����
    static CTimeLineWin2* getObj_withoutLocking(int hWnd);	//���ݴ��ھ������ȡ��Ӧʱ��������ָ��

private:

	int								m_TimeTipPos;			//�������ʱ��tip�ĵ�ǰλ�á��ڴ�λ����ʾ�������ʱ��

private:

	static std::map<int,CTimeLineWin2*>	m_mapTimeLineWins;	//���ǵ������ж��ʱ���ᴰ�ڴ��ڣ����ﱣ��ÿ��ʱ���ᴰ�ھ���Ͷ���ָ���map����OnSize/OnPaintʱҪ֪��������ĸ�����
	void AddWnd(int hWnd);									//��һ��ʱ���ᴰ�ھ����ӵ�map��ȥ
    void RemoveWnd(int hWnd);								//��һ��ʱ��������map��ɾ��

};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_TIMELINE_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_)
