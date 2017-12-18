// TimeLineWin.h: interface for the CTimeLineWin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMELINEWIN_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_)
#define AFX_TIMELINEWIN_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include <time.h>
#include <map>
#include <set>
#include <vector>
using namespace std;

#include "Tools_Ansi.h"
#include "easytime.hpp"
#include "easytimespan.hpp"
#include "easytradingtimespan.hpp"


struct Stru_TimeLineWin_Cfg;
class CPreTimeLineWin;


#define ClsName_TimeLineWin "cls_TimeLineWin"


//����ʱ��̶ȴ��ڵ����ò���
struct Stru_TimeLineWin_Cfg
{
    Stru_TimeLineWin_Cfg(int markPos=0,
                            COLORREF bkCol=RGB(0,0,0),
                            COLORREF lineCol=RGB(255,0,0),
                            COLORREF fontCol=RGB(255,0,0),
                            int fontSize=12,
                            const char* fontName="������",
                            int longScaleHei=6,
                            int shortScaleHei=4,
                            int timespanstartScaleHei=2,
							int wndHei=16);
    Stru_TimeLineWin_Cfg(const Stru_TimeLineWin_Cfg& r);
    ~Stru_TimeLineWin_Cfg();
    Stru_TimeLineWin_Cfg& operator=(const Stru_TimeLineWin_Cfg& r);

    void setMarkPos(int markPos) { MarkPos=markPos; }


    COLORREF    BkCol;              //������ɫ
    COLORREF    LineCol;            //������ɫ
    COLORREF    FontCol;            //������ɫ
    int         FontSize;           //�����С������
    char        FontName[256];      //��������
    int         LongScaleHei;       //���̶��ߵĸ߶�
    int         ShortScaleHei;      //�̶̿��ߵĸ߶�
    int         TimespanStartScaleHei;//ʱ�ο�ʼʱ��̶ȵĸ߶�
	int			WndHei;			    //ʱ���ᴰ�ڸ߶�

    int         MarkPos;            //�̶ȱ�עλ�ã�0-���ֱ�ע�ڿ̶��м䣻1-��ע���ұ�


    HBRUSH      hBrBk;              //ʱ���ᴰ�ڵı���ˢ
    HPEN        hPen;               //ʱ���ᴰ�ڵĻ���
    HFONT       hFont;              //ʱ���ᴰ�ڵ�������
};
//����ʱ��̶ȵķ�Χ
struct Stru_TimeLineWin_Data
{
    Stru_TimeLineWin_Data(int wndWide=0,
                            const CEasyTimespan<time_t>& WholePeriod=CEasyTimespan<time_t>(),
                            const CEasyTradingTimespan& TradingTimespan=CEasyTradingTimespan()
                            );

    //��������ʱ��̶ȵķ�Χ
    void setRange(time_t start,time_t end);
    //��������ʱ��̶ȵķ�Χ��ָ����ʼʱ�䣬����Ч����
    void setRange2(time_t StartUTC,int SpanSeconds);
    //�������ô��ڿ��
    void setWndWide(int wndWide) { m_WndWide=wndWide; }
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
    void getScale(set<time_t>& longScale,set<time_t>& shortScale) const;


    //������ʼʱ��ʹ��ڿ�ȣ�����̶ȵ�ʱ��
    void calcuPeriods_internal(void);
    //����̶���λ��
    void calcuScalePos_internal(void);


	//ȡÿ��Ϸ�������
	int getSeconds_EachDay(void);

    int                             m_WndWide;              //���ڿ��

    CEasyTradingTimespan            m_TradingTimespan;      //����ʱ�ζ���

    CEasyTimespan<time_t>           m_WholePeriod;          //ʱ��̶���ֹʱ��
    CEasyTimespan<time_t>           m_TimeSpans;            //��Ч��ֹʱ��ļ���
	
	int								m_WeekendDaysInclued;	//��������ĩ������

	int								m_totalsecs;			//��������ָ��Ч����������ʱ����

    int                             m_MarkUnit;             //��ע��λ��0:��hh��ע��1:��yyyy-mm-dd��ע

    set<time_t>                     BreakScaleUTC;          //ʱ���жϵ��ʱ�伯   
	set<int>                        BreakScalePos;          //ʱ���жϵ��λ�ü��� 

    set<time_t>                     LongScaleUTC;           //���̶ȵ�ʱ�伯   
    set<int>                        LongScalePos;           //���̶ȵ�λ�ü��� 

    set<time_t>                     ShortScaleUTC;          //�̶̿ȵ�ʱ�伯
    set<int>                        ShortScalePos;          //�̶̿ȵ�λ�ü���

private:
    //������������ֹʹ��
    Stru_TimeLineWin_Data(const Stru_TimeLineWin_Data& r);
    Stru_TimeLineWin_Data& operator=(const Stru_TimeLineWin_Data& r);
};


//����ΪCTimeLineWin��̬ע�ᴰ����
class CPreTimeLineWin
{
public:
    CPreTimeLineWin();
    ~CPreTimeLineWin();
    void Lock(void) { EnterCriticalSection(&m_CS); };
    void Unlock(void) { LeaveCriticalSection(&m_CS); };

    void AddHWND(HWND hWnd,void* pobj);
    void RemoveHWND(HWND hWnd);
    void* getObj_withoutLocking(HWND hWnd);

private:

    map<HWND,void*> m_TimeLineWins;
    CRITICAL_SECTION m_CS;
};

//ʱ��̶ȴ�����
class CTimeLineWin
{
public:
	CTimeLineWin(const Stru_TimeLineWin_Cfg& cfg,bool bCreate=false,HWND hParent=NULL);
	virtual ~CTimeLineWin();

	HWND CreateWnd(HWND hParent);               //�ڸ������д���ʱ��̶ȴ���

    //��������������������MFC�����Ĵ����е���
    virtual void OnSize(HWND hWnd);
    virtual void OnPaint(HWND hWnd); 

    //���ڴ�����������win32 api�����Ĵ��ڡ���Ҫ����WM_SIZE��WM_PAINT
    static LRESULT WINAPI TimeLineProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);


	void SetTimeTip(int pox);                   //��ʾ�������λ�ö�Ӧʱ��tip������ڸ����ڿͻ������ƶ�ʱ����
	void ClearTimeTip(void);                    //����������λ�ö�Ӧʱ��tip������뿪������ʱ����

    //��������ʱ��̶ȵķ�Χ
    void setRange(time_t start,time_t end) { m_data.setRange(start,end); }
    //��������ʱ��̶ȵķ�Χ��ָ����ʼʱ�䣬����Ч����
    void setRange2(time_t StartUTC,int SpanSeconds) { m_data.setRange2(StartUTC,SpanSeconds); }
    //�������ô��ڿ��
    void setWndWide(int wndWide) { m_data.setWndWide(wndWide); }
    //���ý���ʱ�����
    void setTradingTimespan(const CEasyTradingTimespan& TradingTimespan) { m_data.setTradingTimespan(TradingTimespan); }
    //���¼��㣬�������ú���Ҫ������������������¼���
    void refresh(void) { m_data.refresh(); }

	//utc�Ƿ��Ѿ���ʱ
    bool isovertime(time_t utc) { return m_data.isovertime(utc); }
    //����x��λ��ȡ��Ӧʱ��
    time_t getUTC(int xPos) { return m_data.getUTC(xPos); }
    //����ʱ��ȡx��λ��
    int getxPos(time_t utc) { return m_data.getxPos(utc); }
    //��ȡ�̶ȵ�ʱ�伯
    void getScale(set<time_t>& longScale,set<time_t>& shortScale) { return m_data.getScale(longScale,shortScale); }
    //��ȡʱ���ᴰ�ڿ��
    int getWndWide(void) { return m_data.m_WndWide; }
	//ȡÿ��Ϸ�������
    int getSeconds_EachDay(void) { return m_data.getSeconds_EachDay(); }


    //��ȡ���ò���
    const Stru_TimeLineWin_Cfg& getCfg(void) { return m_cfg; }
    //��ȡ���ò���
    const Stru_TimeLineWin_Data& getData(void) { return m_data; }
    //��ȡ�������ʱ��tip�ĵ�ǰλ��
    int& getTimeTipPos(void) { return m_TimeTipPos; }

    HWND                    m_hWnd;             //ʱ��̶ȴ��ھ��

private:
    Stru_TimeLineWin_Cfg    m_cfg;              //ʱ��̶ȴ��ڵ����ò���
    Stru_TimeLineWin_Data   m_data;

	int m_TimeTipPos;                           //�������ʱ��tip�ĵ�ǰλ�á��ڴ�λ����ʾ�������ʱ��

public:
    static CPreTimeLineWin  m_PreOperation;     //��¼���ھ���ʹ��ڶ���Ķ�Ӧ��ϵ���ڴ��ڻص�������ʹ��
};


#endif // !defined(AFX_TIMELINE_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_)
