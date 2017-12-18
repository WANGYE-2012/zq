// TimeLineWin.cpp: implementation of the CTimeLineWin class.
//
//////////////////////////////////////////////////////////////////////

#include "TimeLineWin.h"


CPreTimeLineWin CTimeLineWin::m_PreOperation;


Stru_TimeLineWin_Cfg::Stru_TimeLineWin_Cfg(int markPos,
                                            COLORREF bkCol,
                                            COLORREF lineCol,
                                            COLORREF fontCol,
                                            int fontSize,
                                            const char* fontName,
                                            int longScaleHei,
                                            int shortScaleHei,
                                            int timespanstartScaleHei,
											int wndHei)
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

    hBrBk=CreateSolidBrush(BkCol);
    hPen=CreatePen(PS_SOLID,1,LineCol);
    hFont=CreateFont(-1*FontSize,0,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,0,FontName);
}

Stru_TimeLineWin_Cfg::Stru_TimeLineWin_Cfg(const Stru_TimeLineWin_Cfg& r)
:hBrBk(NULL),hPen(NULL),hFont(NULL)
{
    *this=r;
}

Stru_TimeLineWin_Cfg::~Stru_TimeLineWin_Cfg()
{
    if(hBrBk)   { DeleteObject(hBrBk);  hBrBk=NULL; }
    if(hPen)    { DeleteObject(hPen);   hPen=NULL; }
    if(hFont)   { DeleteObject(hFont);  hFont=NULL; }
}

Stru_TimeLineWin_Cfg& Stru_TimeLineWin_Cfg::operator=(const Stru_TimeLineWin_Cfg& r)
{
    if(BkCol!=r.BkCol)
    {
        BkCol=r.BkCol;
        if(hBrBk) { DeleteObject(hBrBk);  hBrBk=NULL; }
    }
    if(!hBrBk) hBrBk=CreateSolidBrush(BkCol);

    if(LineCol!=r.LineCol)
    {
        LineCol=r.LineCol;
        if(hPen) { DeleteObject(hPen);  hPen=NULL; }
    }
    if(!hPen) hPen=CreatePen(PS_SOLID,1,LineCol);

    if(FontSize!=r.FontSize||memcmp(FontName,r.FontName,sizeof(FontName))!=0)
    {
        FontSize=r.FontSize;
        memcpy(FontName,r.FontName,sizeof(FontName));
        if(hFont) { DeleteObject(hFont);  hFont=NULL; }
    }
    if(!hFont) hFont=CreateFont(-1*FontSize,0,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,0,FontName);

	FontCol=r.FontCol;
    LongScaleHei=r.LongScaleHei;
    ShortScaleHei=r.ShortScaleHei;
    WndHei=r.WndHei;
    MarkPos=r.MarkPos;
    return *this;
};


Stru_TimeLineWin_Data::Stru_TimeLineWin_Data(int wndWide,
                                             const CEasyTimespan<time_t>& WholePeriod,
                                             const CEasyTradingTimespan& TradingTimespan)
{
    m_WndWide=wndWide;
    m_WholePeriod=WholePeriod;                  //ʱ��̶���ֹʱ��
    m_TradingTimespan=TradingTimespan;          //����ʱ�ζ���
    m_MarkUnit=0;
	m_WeekendDaysInclued=0;
    m_totalsecs=0;

	refresh();
}

//utc�Ƿ��Ѿ���ʱ
bool Stru_TimeLineWin_Data::isovertime(time_t utc) const
{
    time_t lastend=m_TimeSpans.getLastTimespan().end;
    return lastend==(time_t)0 || utc>=lastend ? true : false;
}
//����x��λ��ȡ��Ӧʱ��
time_t Stru_TimeLineWin_Data::getUTC(int xPos) const
{
    if(xPos<=0||m_WndWide==0) return m_TimeSpans.getFirstTimespan().start;
    if(xPos>=m_WndWide) return m_TimeSpans.getLastTimespan().end;

    int offset=xPos*m_totalsecs/m_WndWide;
    return m_TimeSpans.getTimeByOffset(offset);
}
//����ʱ��ȡx��λ��
int Stru_TimeLineWin_Data::getxPos(time_t utc) const
{
    if(m_totalsecs==0) return -1;

    int offset=m_TimeSpans.getOffsetByTime(utc);
    if(offset<0||offset>=m_totalsecs) return -1;

    return offset*m_WndWide/m_totalsecs;
}

//��ȡ�̶ȵ�ʱ�伯
void Stru_TimeLineWin_Data::getScale(set<time_t>& longScale,set<time_t>& shortScale) const
{
    longScale=LongScaleUTC;
    shortScale=ShortScaleUTC;
}
//������ʼʱ��ʹ��ڿ�ȣ�����̶ȵ�ʱ��
void Stru_TimeLineWin_Data::calcuPeriods_internal(void)
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
void Stru_TimeLineWin_Data::calcuScalePos_internal(void)
{
    m_MarkUnit=0;
	BreakScaleUTC.clear();
	BreakScalePos.clear();
    LongScaleUTC.clear();
    LongScalePos.clear();
    ShortScaleUTC.clear();
    ShortScalePos.clear();

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
		BreakScaleUTC.insert(it_TimeSpans->first);
    }

    //����̶�ֵ
    if(m_MarkUnit==1)		//SpanDays>3
    {
        if(m_WndWide/SpanDays>=40)
        {
            //���ճ��̶ȣ�û�ж̶̿�
			if(m_TradingTimespan.isLocalTzDST()) tmputc=CEasyTime(syear,smon,sday).gettimeb().time;
			else tmputc=CEasyTime2(syear,smon,sday,0,0,0,m_TradingTimespan.getTimezoneDst()).gettimeb().time;
            while(tmputc<=endtime)
            {
                if(m_TimeSpans.isInTimespans(tmputc))
					LongScaleUTC.insert(tmputc);
                else
                {
                    it_TimeSpans=mapTimeSpans.lower_bound(tmputc);
                    if(it_TimeSpans!=mapTimeSpans.end())
					    LongScaleUTC.insert(it_TimeSpans->first);
                }

                tmputc+=24*3600;
            }
        }
        else if(SpanDays>=30&&m_WndWide/(SpanDays/30)>=40)
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
					LongScaleUTC.insert(tmputc);
                else
                {
                    it_TimeSpans=mapTimeSpans.lower_bound(tmputc);
                    if(it_TimeSpans!=mapTimeSpans.end())
					    LongScaleUTC.insert(it_TimeSpans->first);
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
					LongScaleUTC.insert(tmputc);
                else
                {
                    it_TimeSpans=mapTimeSpans.lower_bound(tmputc);
                    if(it_TimeSpans!=mapTimeSpans.end())
					    LongScaleUTC.insert(it_TimeSpans->first);
                }

                year++;
            }
        }
    }
    else if(m_MarkUnit==0&&SpanHours>0)
    {
        int longhours;  //���̶Ȱ���Сʱһ��
        int shourmins;  //�̶̿Ȱ�������һ��
        if(m_WndWide/SpanHours>=80)
        {
            //��СʱΪ���̶ȣ���15/30/45����Ϊ�̶̿�
            longhours=1;
            shourmins=15;
        }
        else if(m_WndWide/SpanHours*4>=100)
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
				LongScaleUTC.insert(tmputc);
            tmputc+=longhours*3600;
        }
        //����̶̿�
		if(m_TradingTimespan.isLocalTzDST()) tmputc=CEasyTime(syear,smon,sday).gettimeb().time;
		else tmputc=CEasyTime2(syear,smon,sday,0,0,0,m_TradingTimespan.getTimezoneDst()).gettimeb().time;
        while(tmputc<=endtime)
        {
            if(m_TimeSpans.isInTimespans(tmputc))
				ShortScaleUTC.insert(tmputc);
            tmputc+=shourmins*60;
        }
    }

	//����̶�λ��
    set<time_t>::const_iterator it;
    for(it=BreakScaleUTC.begin();it!=BreakScaleUTC.end();it++)
        BreakScalePos.insert(getxPos(*it));
    for(it=LongScaleUTC.begin();it!=LongScaleUTC.end();it++)
        LongScalePos.insert(getxPos(*it));
    for(it=ShortScaleUTC.begin();it!=ShortScaleUTC.end();it++)
        ShortScalePos.insert(getxPos(*it));
}


//ȡÿ��Ϸ�������
int Stru_TimeLineWin_Data::getSeconds_EachDay(void)
{
    return m_TradingTimespan.getSeconds_EachDay();
}

void Stru_TimeLineWin_Data::setRange(time_t start,time_t end) 
{ 
    m_WholePeriod.clear();
    m_WholePeriod.insert(start,end);
}

//��������ʱ��̶ȵķ�Χ��ָ����ʼʱ�䣬����Ч����
//Ŀǰ����һ�ֱȽϵ�Ч���㷨���ȼ���һ�죬���Ƿ񹻣�����������һ�죬����
void Stru_TimeLineWin_Data::setRange2(time_t StartUTC,int SpanSeconds)
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



CPreTimeLineWin::CPreTimeLineWin()
{
    InitializeCriticalSection(&m_CS);

    WNDCLASSEX wc;
    ZeroMemory(&wc,sizeof(wc));
    wc.cbSize				=sizeof(wc);
    wc.style				=CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    wc.lpfnWndProc	        =CTimeLineWin::TimeLineProc;
    wc.cbClsExtra		    =0;
    wc.cbWndExtra		    =0;
    wc.hInstance		    =GetModuleHandle(NULL); 
    wc.hCursor			    =LoadCursor(NULL,IDC_ARROW);
    wc.hIcon				=NULL;
    wc.hbrBackground        =NULL;
    wc.lpszMenuName         =NULL;
    wc.lpszClassName        =ClsName_TimeLineWin; 
    wc.hIconSm			    =NULL;
    RegisterClassEx(&wc);
}
CPreTimeLineWin::~CPreTimeLineWin()
{
    UnregisterClass(ClsName_TimeLineWin,GetModuleHandle(NULL));

    DeleteCriticalSection(&m_CS);
}
void* CPreTimeLineWin::getObj_withoutLocking(HWND hWnd)
{
    void* prlt=NULL;
    map<HWND,void*>::iterator it=m_TimeLineWins.find(hWnd);
    if(it!=m_TimeLineWins.end()) prlt=it->second;
    return prlt;
}
void CPreTimeLineWin::AddHWND(HWND hWnd,void* pobj)
{
    EnterCriticalSection(&m_CS);
	m_TimeLineWins.insert(pair<HWND,void*>(hWnd,pobj));
    LeaveCriticalSection(&m_CS);
}
void CPreTimeLineWin::RemoveHWND(HWND hWnd)
{
    EnterCriticalSection(&m_CS);
    map<HWND,void*>::iterator it=m_TimeLineWins.find(hWnd);
    if(it!=m_TimeLineWins.end())
        m_TimeLineWins.erase(it);
    LeaveCriticalSection(&m_CS);
}


CTimeLineWin::CTimeLineWin(const Stru_TimeLineWin_Cfg& cfg,bool bCreate,HWND hParent)
:m_cfg(cfg)
{
	m_TimeTipPos=-1;
    m_hWnd=NULL;

    if(bCreate)
		CreateWnd(hParent);
}


CTimeLineWin::~CTimeLineWin()
{
    m_PreOperation.RemoveHWND(m_hWnd);

    if(m_hWnd)
    {
        if(IsWindow(m_hWnd))
            DestroyWindow(m_hWnd);
        m_hWnd=NULL;
    }
}

HWND CTimeLineWin::CreateWnd(HWND hParent)
{
	if(m_hWnd!=NULL) return m_hWnd;

    DWORD dwStyle=WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE;
    if(hParent) dwStyle|=WS_CHILD;

    m_hWnd=CreateWindow(ClsName_TimeLineWin,"",dwStyle,0,0,0,m_cfg.WndHei,hParent,NULL,GetModuleHandle(NULL),NULL);

	if(m_hWnd)
		m_PreOperation.AddHWND(m_hWnd,this);
	return m_hWnd;
}

void CTimeLineWin::OnSize(HWND hWnd) 
{
    RECT		Rect;
    GetClientRect(hWnd,&Rect);

    m_data.setWndWide(Rect.right-Rect.left);
    m_data.refresh();

    InvalidateRect(hWnd,NULL,FALSE);
    SendMessage(hWnd,WM_PAINT,0,0L);
}

void CTimeLineWin::OnPaint(HWND hWnd) 
{
    RECT Rect;
    int Wid,Hei;

	GetClientRect(hWnd,&Rect);
	Wid=Rect.right-Rect.left;
	Hei=Rect.bottom-Rect.top;

	PAINTSTRUCT			Ps;
	HDC					hDC,MemDC;
	HBITMAP				hBmp;
	hDC=BeginPaint(hWnd,&Ps);
	hBmp=CreateCompatibleBitmap(hDC,Wid,Hei);
	MemDC=CreateCompatibleDC(hDC);
	SelectObject(MemDC,hBmp);

    FillRect(MemDC,&Rect,m_cfg.hBrBk);
    SelectObject(MemDC,m_cfg.hPen);
    SelectObject(MemDC,m_cfg.hFont);
    SetTextColor(MemDC,m_cfg.FontCol);
    SetBkMode(MemDC,TRANSPARENT);

    //��������
    MoveToEx(MemDC,0,0,NULL);
    LineTo(MemDC,Wid,0);

    set<int>::const_iterator it_pos;

    //ʱ��εĿ�ʼ���̶̿���
    for(it_pos=m_data.BreakScalePos.begin();it_pos!=m_data.BreakScalePos.end();it_pos++)
    {
        MoveToEx(MemDC,*it_pos,1,NULL);
        LineTo(MemDC,*it_pos,m_cfg.TimespanStartScaleHei);
//		SetPixel(MemDC,(*it_pos)-1,1,m_cfg.LineCol);
//		SetPixel(MemDC,(*it_pos)+1,1,m_cfg.LineCol);
    }

    //�����̶���
    for(it_pos=m_data.LongScalePos.begin();it_pos!=m_data.LongScalePos.end();it_pos++)
    {
        MoveToEx(MemDC,*it_pos,1,NULL);
        LineTo(MemDC,*it_pos,m_cfg.LongScaleHei);
    }

    //���̶̿���
    for(it_pos=m_data.ShortScalePos.begin();it_pos!=m_data.ShortScalePos.end();it_pos++)
    {
        MoveToEx(MemDC,*it_pos,1,NULL);
        LineTo(MemDC,*it_pos,m_cfg.ShortScaleHei);
    }

    //��ע���̶�
    it_pos=m_data.LongScalePos.begin();
    set<time_t>::const_iterator it_utc=m_data.LongScaleUTC.begin();
    int lastlabelpos=-1;
    for(;it_pos!=m_data.LongScalePos.end()&&it_utc!=m_data.LongScaleUTC.end();it_pos++,it_utc++)
    {
        char buf[256];
        short year,month,day,hour,min,sec,wday;
		if(m_data.m_TradingTimespan.isLocalTzDST()) CEasyTime(*it_utc).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
		else CEasyTime2(*it_utc,0,m_data.m_TradingTimespan.getTimezoneDst()).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
        if(m_data.m_MarkUnit==1)
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
        if(m_cfg.MarkPos==1)
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
		time_t TimeTipUTC=m_data.getUTC(m_TimeTipPos);
		if(m_data.m_TradingTimespan.isLocalTzDST()) CEasyTime(TimeTipUTC).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
		else CEasyTime2(TimeTipUTC,0,m_data.m_TradingTimespan.getTimezoneDst()).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
		buf[0]=0;
		if(m_data.m_MarkUnit==1)
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
	EndPaint(hWnd,&Ps);
}

LRESULT WINAPI CTimeLineWin::TimeLineProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam) 
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
            m_PreOperation.Lock();
            CTimeLineWin* pObj=(CTimeLineWin*)m_PreOperation.getObj_withoutLocking(hWnd);
            if(pObj!=NULL)
                pObj->OnSize(hWnd);
            m_PreOperation.Unlock();
            break;
        }
        case WM_PAINT:
        {
            m_PreOperation.Lock();
            CTimeLineWin* pObj=(CTimeLineWin*)m_PreOperation.getObj_withoutLocking(hWnd);
            if(pObj!=NULL)
                pObj->OnPaint(hWnd);
            m_PreOperation.Unlock();
            break;
        }
    }
    return(DefWindowProc(hWnd,Msg,wParam,lParam));
}

void CTimeLineWin::SetTimeTip(int pox)
{
	m_TimeTipPos=pox;
	if(m_hWnd!=NULL)
	{
		::InvalidateRect(m_hWnd,NULL,FALSE);
		::SendMessage(m_hWnd,WM_PAINT,0,0);
	}
}

void CTimeLineWin::ClearTimeTip(void)
{
	m_TimeTipPos=-1;
	if(m_hWnd!=NULL)
	{
		::InvalidateRect(m_hWnd,NULL,FALSE);
		::SendMessage(m_hWnd,WM_PAINT,0,0);
	}
}
