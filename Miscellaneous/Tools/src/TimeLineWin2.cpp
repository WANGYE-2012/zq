// TimeLineWin.cpp: implementation of the CTimeLineWin2 class.
//
//////////////////////////////////////////////////////////////////////


#include "TimeLineWin2.h"
#include <windows.h>
#include <map>
#include <set>
using namespace std;


#ifdef WIN32
#pragma unmanaged
#endif 

Ceasymutex				CTimeLineWin2::m_mutex;
map<int,CTimeLineWin2*>	CTimeLineWin2::m_mapTimeLineWins;

//���ڴ�����������win32 api�����Ĵ��ڡ���Ҫ����WM_SIZE��WM_PAINT
LRESULT WINAPI TimeLineProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);


Stru_TimeLineWin2_Data::Stru_TimeLineWin2_Data(int TimeLineWid,
                                             const CEasyTimespan<time_t>& WholePeriod,
                                             const CEasyTradingTimespan& TradingTimespan)
{
    m_TimeLineWid=TimeLineWid;
    m_WholePeriod=WholePeriod;                  //ʱ��̶���ֹʱ��
    m_TradingTimespan=TradingTimespan;          //����ʱ�ζ���
    m_MarkUnit=0;
	m_WeekendDaysInclued=0;
    m_totalsecs=0;

	refresh();
}

//utc�Ƿ��Ѿ���ʱ
bool Stru_TimeLineWin2_Data::isovertime(time_t utc) const
{
    time_t lastend=m_TimeSpans.getLastTimespan().end;
    return lastend==(time_t)0 || utc>=lastend ? true : false;
}
//����x��λ��ȡ��Ӧʱ��
time_t Stru_TimeLineWin2_Data::getUTC(int xPos) const
{
    if(xPos<=0||m_TimeLineWid==0) return m_TimeSpans.getFirstTimespan().start;
    if(xPos>=m_TimeLineWid) return m_TimeSpans.getLastTimespan().end;

    int offset=xPos*m_totalsecs/m_TimeLineWid;
    return m_TimeSpans.getTimeByOffset(offset);
}
//����ʱ��ȡx��λ��
int Stru_TimeLineWin2_Data::getxPos(time_t utc) const
{
    if(m_totalsecs==0) return -1;

    int offset=m_TimeSpans.getOffsetByTime(utc);
    if(offset<0||offset>=m_totalsecs) return -1;

    return offset*m_TimeLineWid/m_totalsecs;
}

//��ȡ�̶ȵ�ʱ�伯
void Stru_TimeLineWin2_Data::getScale(set<time_t>& longScale,set<time_t>& shortScale) const
{
    longScale=m_LongScaleUTC;
    shortScale=m_ShortScaleUTC;
}
//������ʼʱ��ʹ��ڿ�ȣ�����̶ȵ�ʱ��
void Stru_TimeLineWin2_Data::calcuPeriods_internal(void)
{
    m_TimeSpans.clear();
    m_WeekendDaysInclued=0;
    m_totalsecs=0;

    const map<time_t,Stru_TimespanInfo<time_t>>& wholeperiod=m_WholePeriod.getTimespans();

    if(wholeperiod.size()==1)
    {
        m_TimeSpans=m_TradingTimespan.getTradingTimespan_InPeriod(wholeperiod.begin()->first,
                                                                    wholeperiod.begin()->second.end,
                                                                    m_WeekendDaysInclued);
        m_totalsecs=(int)m_TimeSpans.getTimeDuration();
    }
}


//����̶���λ��
void Stru_TimeLineWin2_Data::calcuScalePos_internal(void)
{
    m_MarkUnit=0;
	m_BreakScaleUTC.clear();
	m_BreakScalePos.clear();
    m_LongScaleUTC.clear();
    m_LongScalePos.clear();
    m_ShortScaleUTC.clear();
    m_ShortScalePos.clear();

    if((int)m_TimeSpans.getTimespans().size()==0) 
        return;

    const map<time_t,Stru_TimespanInfo<time_t>>& mapTimeSpans=m_TimeSpans.getTimespans();
    map<time_t,Stru_TimespanInfo<time_t>>::const_iterator it_TimeSpans;

    short syear,smon,sday,shour,smin,ssec;
	time_t starttime,endtime,tmputc;
	int SecondsIntraday;
	if(m_TradingTimespan.isLocalTzDST())
	{
		CEasyTime tt(m_TimeSpans.getFirstTimespan().start,0);
		starttime=tt.getutc();
		SecondsIntraday=tt.getSecondsIntraday();
		tt.convert2fileds(&syear,&smon,&sday,&shour,&smin,&ssec,NULL);
		endtime=CEasyTime(m_TimeSpans.getLastTimespan().end,0).getutc();
	}
	else
	{
		CEasyTime2 tt(m_TimeSpans.getFirstTimespan().start,0,m_TradingTimespan.getTimezoneDst());
		starttime=tt.getutc();
		SecondsIntraday=tt.getSecondsIntraday();
		tt.convert2fileds(&syear,&smon,&sday,&shour,&smin,&ssec,NULL);
		endtime=CEasyTime2(m_TimeSpans.getLastTimespan().end,0,m_TradingTimespan.getTimezoneDst()).getutc();
	}

    //����ʱ����(��)
    int i=(int)(endtime-starttime+SecondsIntraday);
    int SpanDays=i/(24*3600)+(i%(24*3600)==0?0:1);
    SpanDays-=m_WeekendDaysInclued;
    if(SpanDays<0) SpanDays=0;
    
	//����ʱ����(Сʱ)
	int SpanHours=m_totalsecs/3600+(m_totalsecs%3600!=0?1:0);

    //ȷ����ע��λ������3�찴�ձ�ע������Сʱ��ע
    m_MarkUnit = SpanDays>3 ? 1 : 0;

	//����ʱ��εĿ�ʼ�������̶�
    for(it_TimeSpans=mapTimeSpans.begin();it_TimeSpans!=mapTimeSpans.end();it_TimeSpans++)
    {
		m_BreakScaleUTC.insert(it_TimeSpans->first);
    }

    //����̶�ֵ
    if(m_MarkUnit==1)		//SpanDays>3
    {
        if(m_TimeLineWid/SpanDays>=40)
        {
            //���ճ��̶ȣ�û�ж̶̿�
			if(m_TradingTimespan.isLocalTzDST()) tmputc=CEasyTime(syear,smon,sday).gettimeb().time;
			else tmputc=CEasyTime2(syear,smon,sday,0,0,0,m_TradingTimespan.getTimezoneDst()).gettimeb().time;
            while(tmputc<=endtime)
            {
                if(m_TimeSpans.isInTimespans(tmputc))
					m_LongScaleUTC.insert(tmputc);
                else
                {
                    it_TimeSpans=mapTimeSpans.lower_bound(tmputc);
                    if(it_TimeSpans!=mapTimeSpans.end())
					    m_LongScaleUTC.insert(it_TimeSpans->first);
                }

                tmputc+=24*3600;
            }
        }
        else if(SpanDays>=30&&m_TimeLineWid/(SpanDays/30)>=40)
        {
            //���³��̶ȣ�û�ж̶̿�
            int year=syear;
            int mon=smon;
            while(true)
            {
				if(m_TradingTimespan.isLocalTzDST()) tmputc=CEasyTime(year,mon,1).gettimeb().time;
				else tmputc=CEasyTime2(year,mon,1,0,0,0,m_TradingTimespan.getTimezoneDst()).gettimeb().time;
				if(tmputc>endtime) break;

                if(m_TimeSpans.isInTimespans(tmputc))
					m_LongScaleUTC.insert(tmputc);
                else
                {
                    it_TimeSpans=mapTimeSpans.lower_bound(tmputc);
                    if(it_TimeSpans!=mapTimeSpans.end())
					    m_LongScaleUTC.insert(it_TimeSpans->first);
                }

                mon++;
                if(mon>12) { year++;  mon=1; }
            }
        }
        else
        {
            //���곤�̶ȣ�û�ж̶̿�
            int year=syear;
            while(true)
            {
				if(m_TradingTimespan.isLocalTzDST()) tmputc=CEasyTime(year,1,1).gettimeb().time;
				else tmputc=CEasyTime2(year,1,1,0,0,0,m_TradingTimespan.getTimezoneDst()).gettimeb().time;
				if(tmputc>endtime) break;

				if(m_TimeSpans.isInTimespans(tmputc))
					m_LongScaleUTC.insert(tmputc);
                else
                {
                    it_TimeSpans=mapTimeSpans.lower_bound(tmputc);
                    if(it_TimeSpans!=mapTimeSpans.end())
					    m_LongScaleUTC.insert(it_TimeSpans->first);
                }

                year++;
            }
        }
    }
    else if(m_MarkUnit==0&&SpanHours>0)
    {
        int longhours;  //���̶Ȱ���Сʱһ��
        int shourmins;  //�̶̿Ȱ�������һ��
        if(m_TimeLineWid/SpanHours>=80)
        {
            //��СʱΪ���̶ȣ���15/30/45����Ϊ�̶̿�
            longhours=1;
            shourmins=15;
        }
        else if(m_TimeLineWid/SpanHours*4>=100)
        {
            //��4СʱΪ���̶ȣ���1СʱΪ�̶̿�
            longhours=4;
            shourmins=60;
        }
        else 
        {
            //��12СʱΪ���̶ȣ���1СʱΪ�̶̿�
            longhours=12;
            shourmins=60;
        }

        //���㳤�̶�
		if(m_TradingTimespan.isLocalTzDST()) tmputc=CEasyTime(syear,smon,sday).gettimeb().time;
		else tmputc=CEasyTime2(syear,smon,sday,0,0,0,m_TradingTimespan.getTimezoneDst()).gettimeb().time;
        while(tmputc<=endtime)
        {
            if(m_TimeSpans.isInTimespans(tmputc))
				m_LongScaleUTC.insert(tmputc);
            tmputc+=longhours*3600;
        }
        //����̶̿�
		if(m_TradingTimespan.isLocalTzDST()) tmputc=CEasyTime(syear,smon,sday).gettimeb().time;
		else tmputc=CEasyTime2(syear,smon,sday,0,0,0,m_TradingTimespan.getTimezoneDst()).gettimeb().time;
        while(tmputc<=endtime)
        {
            if(m_TimeSpans.isInTimespans(tmputc))
				m_ShortScaleUTC.insert(tmputc);
            tmputc+=shourmins*60;
        }
    }

	//����̶�λ��
    set<time_t>::const_iterator it;
    for(it=m_BreakScaleUTC.begin();it!=m_BreakScaleUTC.end();it++)
        m_BreakScalePos.insert(getxPos(*it));
    for(it=m_LongScaleUTC.begin();it!=m_LongScaleUTC.end();it++)
        m_LongScalePos.insert(getxPos(*it));
    for(it=m_ShortScaleUTC.begin();it!=m_ShortScaleUTC.end();it++)
        m_ShortScalePos.insert(getxPos(*it));
}


//ȡÿ��Ϸ�������
int Stru_TimeLineWin2_Data::getSeconds_EachDay(void) const
{
    return m_TradingTimespan.getSeconds_EachDay();
}

void Stru_TimeLineWin2_Data::setRange(time_t start,time_t end) 
{ 
    m_WholePeriod.clear();
    m_WholePeriod.insert(start,end);
}

//��������ʱ��̶ȵķ�Χ��ָ����ʼʱ�䣬����Ч����
//Ŀǰ����һ�ֱȽϵ�Ч���㷨���ȼ���һ�죬���Ƿ񹻣�����������һ�죬����
void Stru_TimeLineWin2_Data::setRange2(time_t StartUTC,int SpanSeconds)
{
	if(SpanSeconds<=0)
	{
        m_WholePeriod.clear();
		return;
	}

	int ValidSecondsEachDay=getSeconds_EachDay();
    if(ValidSecondsEachDay==0)
    {
        m_WholePeriod.clear();
		return;
    }

	int days=(SpanSeconds/ValidSecondsEachDay)+1;
    if(m_TradingTimespan.isExcludeWeekend())
		days+=days/5*2;

	while(days<366)
	{
		setRange(StartUTC,StartUTC+days*24*3600);
		calcuPeriods_internal();
		if(m_totalsecs>=SpanSeconds)
			break;
		days+=2;
	}

	setRange(StartUTC,m_TimeSpans.getTimeByOffset(SpanSeconds));
}







CTimeLineWin2::CTimeLineWin2(const Stru_TimeLineWin2_Cfg& cfg)
:Stru_TimeLineWin2_Cfg(cfg),
 m_hWnd(NULL),
 m_TimeTipPos(-1)
{
}


CTimeLineWin2::~CTimeLineWin2()
{
	if(m_hWnd)
		RemoveWnd(m_hWnd);
}

//ע�ᴰ����
void CTimeLineWin2::RegistWndClass(void)
{
    WNDCLASSEX wc;
    ZeroMemory(&wc,sizeof(wc));
    wc.cbSize				=sizeof(wc);
    wc.style				=CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    wc.lpfnWndProc	        =TimeLineProc;
    wc.cbClsExtra		    =0;
    wc.cbWndExtra		    =0;
    wc.hInstance		    =GetModuleHandle(NULL); 
    wc.hCursor			    =LoadCursor(NULL,IDC_ARROW);
    wc.hIcon				=NULL;
    wc.hbrBackground        =NULL;
    wc.lpszMenuName         =NULL;
    wc.lpszClassName        =ClsName_TimeLineWin2; 
    wc.hIconSm			    =NULL;
    RegisterClassEx(&wc);
}
//ע��������
void CTimeLineWin2::UnregistWndClass(void)
{
    UnregisterClass(ClsName_TimeLineWin2,GetModuleHandle(NULL));
}

int CTimeLineWin2::CreateWnd(int hParent)
{
	if(m_hWnd!=NULL) return m_hWnd;

    DWORD dwStyle=WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE;
    if(hParent) dwStyle|=WS_CHILD;

    m_hWnd=(int)CreateWindow(ClsName_TimeLineWin2,"",dwStyle,0,0,0,WndHei,(HWND)hParent,NULL,GetModuleHandle(NULL),NULL);

	if(m_hWnd)
		AddWnd(m_hWnd);
	return m_hWnd;
}
//����ʱ���ᴰ�ھ���������winform����ʱ���ᴰ�ڣ��������������������hWnd���
void CTimeLineWin2::SetWnd(int hWnd)
{
	m_hWnd=hWnd;
	if(m_hWnd)
		AddWnd(m_hWnd);
}


void CTimeLineWin2::OnSize(int iWnd) 
{
    RECT		Rect;
	HWND		hWnd=(HWND)iWnd;
    GetClientRect(hWnd,&Rect);

    setTimeLineWid(Rect.right-Rect.left);
    refresh();

    InvalidateRect(hWnd,NULL,FALSE);
    SendMessage(hWnd,WM_PAINT,0,0L);
}

void CTimeLineWin2::OnPaint(int iWnd)
{
	PAINTSTRUCT			Ps;
	HWND				hWnd=(HWND)iWnd;
	HDC					hDC=BeginPaint((HWND)hWnd,&Ps);
	OnPaint2(iWnd,(int)hDC);
	EndPaint(hWnd,&Ps);
}
void CTimeLineWin2::OnPaint2(int iWnd,int iDC) 
{
    RECT Rect;
    int Wid,Hei;
	HWND	hWnd=(HWND)iWnd;

	GetClientRect(hWnd,&Rect);
	Wid=Rect.right-Rect.left;
	Hei=Rect.bottom-Rect.top;

	HDC		hDC=(HDC)iDC;
	HDC		MemDC;
	HBITMAP	hBmp;
	hBmp=CreateCompatibleBitmap(hDC,Wid,Hei);
	MemDC=CreateCompatibleDC(hDC);
	SelectObject(MemDC,hBmp);

	//ʱ���ᴰ�ڵı���ˢ�����ʺ�����
    HBRUSH		hBrBk=CreateSolidBrush(BkCol);
    HPEN		hPen=CreatePen(PS_SOLID,1,LineCol);
    HFONT       hFont=CreateFont(-1*FontSize,0,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,0,FontName);

    FillRect(MemDC,&Rect,hBrBk);
    SelectObject(MemDC,hPen);
    SelectObject(MemDC,hFont);
    SetTextColor(MemDC,FontCol);
    SetBkMode(MemDC,TRANSPARENT);

    //��������
    MoveToEx(MemDC,LeftSpaceWid,0,NULL);
    LineTo(MemDC,Wid,0);

    set<int>::const_iterator it_pos;

    //ʱ��εĿ�ʼ���̶̿���
    for(it_pos=m_BreakScalePos.begin();it_pos!=m_BreakScalePos.end();it_pos++)
    {
        MoveToEx(MemDC,*it_pos,1,NULL);
        LineTo(MemDC,*it_pos,TimespanStartScaleHei);
//		SetPixel(MemDC,(*it_pos)-1,1,LineCol);
//		SetPixel(MemDC,(*it_pos)+1,1,LineCol);
    }

    //�����̶���
    for(it_pos=m_LongScalePos.begin();it_pos!=m_LongScalePos.end();it_pos++)
    {
        MoveToEx(MemDC,*it_pos,1,NULL);
        LineTo(MemDC,*it_pos,LongScaleHei);
    }

    //���̶̿���
    for(it_pos=m_ShortScalePos.begin();it_pos!=m_ShortScalePos.end();it_pos++)
    {
        MoveToEx(MemDC,*it_pos,1,NULL);
        LineTo(MemDC,*it_pos,ShortScaleHei);
    }

    //��ע���̶�
    it_pos=m_LongScalePos.begin();
    set<time_t>::const_iterator it_utc=m_LongScaleUTC.begin();
    int lastlabelpos=-1;
    for(;it_pos!=m_LongScalePos.end()&&it_utc!=m_LongScaleUTC.end();it_pos++,it_utc++)
    {
        char buf[256];
        short year,month,day,hour,min,sec,wday;
		if(m_TradingTimespan.isLocalTzDST()) CEasyTime(*it_utc).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
		else CEasyTime2(*it_utc,0,m_TradingTimespan.getTimezoneDst()).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
        if(m_MarkUnit==1)
        {
            sprintf(buf,"%d/%d/%d/",year,month,day);
            switch(wday)
            {
                case 0: strcat(buf,"��");  break;
                case 1: strcat(buf,"һ");  break;
                case 2: strcat(buf,"��");  break;
                case 3: strcat(buf,"��");  break;
                case 4: strcat(buf,"��");  break;
                case 5: strcat(buf,"��");  break;
                case 6: strcat(buf,"��");  break;
            }
        }
        else
        {
			if(min==0) sprintf(buf,"%d",hour);
			else sprintf(buf,"%d:%d",hour,min);
        }

        SIZE Sz;
        GetTextExtentPoint32(MemDC,buf,strlen(buf),&Sz);
        int pox,poy;
        if(MarkPos==1)
        {
            //�̶�д���ұ�
            pox=(*it_pos)+2;
            poy=2;
        }
        else
        {
            //�̶�д���м�
            pox=(*it_pos)-Sz.cx/2;
            poy=Hei-Sz.cy;
        }
        if(pox<0) pox=0;
        if(lastlabelpos==-1||pox>=lastlabelpos+4)
        {
            //�������һ���̶ȱ�ע�غ�
            TextOut(MemDC,pox,poy,buf,strlen(buf));
            lastlabelpos=pox+Sz.cx;
        }
    }

	//��עTimeTip
	if(m_TimeTipPos>=0&&m_TimeTipPos<Wid)
	{
		MoveToEx(MemDC,m_TimeTipPos,1,NULL);
		LineTo(MemDC,m_TimeTipPos,Hei);

        char buf[256];
        short year,month,day,hour,min,sec,wday;
		time_t TimeTipUTC=getUTC(m_TimeTipPos);
		if(m_TradingTimespan.isLocalTzDST()) CEasyTime(TimeTipUTC).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
		else CEasyTime2(TimeTipUTC,0,m_TradingTimespan.getTimezoneDst()).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
		buf[0]=0;
		if(m_MarkUnit==1)
		{
			sprintf(buf,"%d/%d/%d/",year,month,day);
			switch(wday)
			{
				case 0: strcat(buf,"��");  break;
				case 1: strcat(buf,"һ");  break;
				case 2: strcat(buf,"��");  break;
				case 3: strcat(buf,"��");  break;
				case 4: strcat(buf,"��");  break;
				case 5: strcat(buf,"��");  break;
				case 6: strcat(buf,"��");  break;
			}
			strcat(buf," ");
		}
		sprintf(buf+strlen(buf),"%d:%d:%d",hour,min,sec);

        SIZE Sz;
        GetTextExtentPoint32(MemDC,buf,strlen(buf),&Sz);
		int txtpox=m_TimeTipPos-Sz.cx/2;
		if(txtpox+Sz.cx>Wid) txtpox=Wid-Sz.cx;
		if(txtpox<0) txtpox=0;
		SetTextColor(MemDC,RGB(255,255,255));
		SetBkColor(MemDC,RGB(0,0,255));
		SetBkMode(MemDC,OPAQUE);
        TextOut(MemDC,txtpox,Hei-Sz.cy,buf,strlen(buf));
	}



	BitBlt(hDC,0,0,Wid,Hei,MemDC,0,0,SRCCOPY);

	DeleteDC(MemDC);
	DeleteObject(hBmp);

	if(hBrBk) DeleteObject(hBrBk);
	if(hPen) DeleteObject(hPen);
	if(hFont) DeleteObject(hFont);
}

LRESULT WINAPI TimeLineProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam) 
{
    switch(Msg)
    {
        case WM_CLOSE:		
        {
			DestroyWindow(hWnd);
            return 0;
        }
        case WM_DESTROY:		
		{
			return 0;
		}
        case WM_SIZE:
        {
			CTimeLineWin2::m_mutex.lock();
			CTimeLineWin2* pObj=CTimeLineWin2::getObj_withoutLocking((int)hWnd);
            if(pObj!=NULL)
                pObj->OnSize((int)hWnd);
			CTimeLineWin2::m_mutex.unlock();
            break;
        }
        case WM_PAINT:
        {
			CTimeLineWin2::m_mutex.lock();
			CTimeLineWin2* pObj=CTimeLineWin2::getObj_withoutLocking((int)hWnd);
            if(pObj!=NULL)
                pObj->OnPaint((int)hWnd);
			CTimeLineWin2::m_mutex.unlock();
            break;
        }
    }
    return(DefWindowProc(hWnd,Msg,wParam,lParam));
}

void CTimeLineWin2::SetTimeTip(int pox)
{
	m_TimeTipPos=pox;
	if(m_hWnd!=NULL)
	{
		::InvalidateRect((HWND)m_hWnd,NULL,FALSE);
		::SendMessage((HWND)m_hWnd,WM_PAINT,0,0);
	}
}

void CTimeLineWin2::ClearTimeTip(void)
{
	m_TimeTipPos=-1;
	if(m_hWnd!=NULL)
	{
		::InvalidateRect((HWND)m_hWnd,NULL,FALSE);
		::SendMessage((HWND)m_hWnd,WM_PAINT,0,0);
	}
}
CTimeLineWin2* CTimeLineWin2::getObj_withoutLocking(int hWnd)
{
    CTimeLineWin2* prlt=NULL;
    map<int,CTimeLineWin2*>::iterator it=m_mapTimeLineWins.find(hWnd);
    if(it!=m_mapTimeLineWins.end()) prlt=it->second;
    return prlt;
}
void CTimeLineWin2::AddWnd(int hWnd)
{
	m_mutex.lock();
	m_mapTimeLineWins.insert(pair<int,CTimeLineWin2*>(hWnd,this));
	m_mutex.unlock();
}
void CTimeLineWin2::RemoveWnd(int hWnd)
{
	m_mutex.lock();
    map<int,CTimeLineWin2*>::iterator it=m_mapTimeLineWins.find(hWnd);
    if(it!=m_mapTimeLineWins.end())
        m_mapTimeLineWins.erase(it);
	m_mutex.unlock();
}


