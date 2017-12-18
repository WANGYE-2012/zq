// TimeLineWin.cpp: implementation of the CTimeLineWin class.
//
//////////////////////////////////////////////////////////////////////


#include "TimeLineWin.h"
#include <windows.h>
#include "Tools_Win32.h"


#ifdef WIN32
#pragma unmanaged
#endif 



CTimeLineWin::Stru_Data::Stru_Data(bool bTimeBased)
:	m_bTimeBased(bTimeBased),
	m_WholePeriod(CEasyTimespan<int>()),
	m_WorkingTimespan(CEasyTradingTimespan()),
	m_CacheTimespanOffset(-1),
	m_CacheTimespanStart(-1),
	m_CacheTimespanEnd(-1)
{
	recalcu();
}

void CTimeLineWin::Stru_Data::clearRange(void)
{
    m_WholePeriod.clear();
}

void CTimeLineWin::Stru_Data::setRange(int start,int end) 
{ 
    m_WholePeriod.clear();
    m_WholePeriod.insert(start,end);
}

//��������ʱ��̶ȵķ�Χ��ָ����ʼʱ�䣬����Ч����
//Ŀǰ����һ�ֱȽϵ�Ч���㷨���ȼ���һ�죬���Ƿ񹻣�����������һ�죬����
void CTimeLineWin::Stru_Data::setRange2(int start,int SpanUnits)
{
	if(SpanUnits<=0)
	{
        m_WholePeriod.clear();
		return;
	}

	if(m_bTimeBased)
	{
		//����ʱ��̶ȣ�SpanUnits������
		int ValidSecondsEachDay=getSeconds_EachDay();
		if(ValidSecondsEachDay==0)
		{
			m_WholePeriod.clear();
			return;
		}

		int days=(SpanUnits/ValidSecondsEachDay)+1;
		if(m_WorkingTimespan.isExcludeWeekend())
			days+=days/5*2;

		while(true)
		{
			setRange(start,start+days*24*3600);
			calcuPeriods_internal();
			if(m_totalunits>=SpanUnits)
				break;
			days+=2;
		}

		setRange(start,m_Spans.getTimeByOffset(SpanUnits));
	}
	else
	{
		setRange(start,start+SpanUnits);
	}
}

//������ʼʱ��ʹ��ڿ�ȣ�����ʱ��κ�������/��Ԫ��
void CTimeLineWin::Stru_Data::calcuPeriods_internal(void)
{
    m_Spans.clear();
    m_WeekendDaysInclued=0;
    m_totalunits=0;

	m_CacheTimespanOffset=-1;
	m_CacheTimespanStart=-1;
	m_CacheTimespanEnd=-1;

	if(m_bTimeBased)
	{
		if(!m_WholePeriod.isempty())
		{
			m_Spans=m_WorkingTimespan.getTradingTimespan_InPeriod(m_WholePeriod.getFirstTimespan().start,m_WholePeriod.getFirstTimespan().end,m_WeekendDaysInclued);
			m_totalunits=(int)m_Spans.getTimeDuration();
		}
	}
	else
	{
		m_Spans=m_WholePeriod;
		m_totalunits=(int)m_Spans.getTimeDuration();
	}
}

//����̶���λ��
void CTimeLineWin::Stru_Data::calcuScalePos_internal(void)
{
    m_MarkUnit=0;
	m_BreakScaleUTC.clear();
	m_BreakScalePos.clear();
    m_LongScaleUTC.clear();
    m_LongScalePos.clear();
    m_ShortScaleUTC.clear();
    m_ShortScalePos.clear();

	if(!m_bTimeBased) 
	{
		//��ʱ��̶ȴ��ڣ�����ʱ��̶�
		return;
	}

	if(m_Spans.isempty()) 
        return;

    short syear,smon,sday,shour,smin,ssec;
	int starttime,endtime,tmputc;
	int SecondsIntraday,SecondsIntraday2;						//��������������ʱ����ʱ�������
	if(m_WorkingTimespan.isLocalTzDST())
	{
		CEasyTime tt(m_Spans.getFirstTimespan().start,0);
		starttime=(int)tt.getutc();
		SecondsIntraday=tt.getSecondsIntraday();		
		tt.convert2fileds(&syear,&smon,&sday,&shour,&smin,&ssec,NULL);
		CEasyTime tt2(m_Spans.getLastTimespan().end,0);
		endtime=(int)tt2.getutc();
		SecondsIntraday2=tt2.getSecondsIntraday();
	}
	else
	{
		CEasyTime2 tt(m_Spans.getFirstTimespan().start,0,m_WorkingTimespan.getTimezoneDst());
		starttime=(int)tt.getutc();
		SecondsIntraday=tt.getSecondsIntraday();
		tt.convert2fileds(&syear,&smon,&sday,&shour,&smin,&ssec,NULL);
		CEasyTime2 tt2(m_Spans.getLastTimespan().end,0,m_WorkingTimespan.getTimezoneDst());
		endtime=(int)tt2.getutc();
		SecondsIntraday2=tt2.getSecondsIntraday();
	}

    //����ʱ����(��)
	int SpanDays=(int)(endtime-SecondsIntraday2-(starttime-SecondsIntraday))/(24*3600) + (SecondsIntraday2>0 ? 1 : 0) - m_WeekendDaysInclued;
    if(SpanDays<0) SpanDays=0;
    
	//����ʱ����(Сʱ)
	int SpanHours=m_totalunits/3600+(m_totalunits%3600!=0?1:0);

    const map<int,Stru_TimespanInfo<int>>& mapTimeSpans=m_Spans.getTimespans();
    map<int,Stru_TimespanInfo<int>>::const_iterator it_TimeSpans;

	//����ʱ��εĿ�ʼ�������̶�
    for(it_TimeSpans=mapTimeSpans.begin();it_TimeSpans!=mapTimeSpans.end();it_TimeSpans++)
		m_BreakScaleUTC.insert(it_TimeSpans->first);
	//����ʱ��Ҳ�����̶�
	if((int)mapTimeSpans.size()>0)
		m_BreakScaleUTC.insert(mapTimeSpans.rbegin()->second.end);

    //����̶�ֵ
    if(SpanDays>=365)
    {
        //���곤�̶ȣ����¶̶̿�
		m_MarkUnit=0;
		for(int year=syear;;year++)
        {
			if(m_WorkingTimespan.isLocalTzDST()) tmputc=(int)CEasyTime(year,1,1).gettimeb().time;
			else tmputc=(int)CEasyTime2(year,1,1,0,0,0,m_WorkingTimespan.getTimezoneDst()).gettimeb().time;
			if(tmputc>=endtime) break;

			if(m_Spans.isInTimespans(tmputc)) m_LongScaleUTC.insert(tmputc);
            else
            {
                it_TimeSpans=mapTimeSpans.lower_bound(tmputc);
                if(it_TimeSpans!=mapTimeSpans.end()) m_LongScaleUTC.insert(it_TimeSpans->first);
            }

			for(int mon=1;mon<=12;mon++)
            {
				if(m_WorkingTimespan.isLocalTzDST()) tmputc=(int)CEasyTime(year,mon,1).gettimeb().time;
				else tmputc=(int)CEasyTime2(year,mon,1,0,0,0,m_WorkingTimespan.getTimezoneDst()).gettimeb().time;
				if(tmputc>=endtime) break;
                if(m_Spans.isInTimespans(tmputc)) m_ShortScaleUTC.insert(tmputc);
            }
        }
    }
	else  if(SpanDays>=30)
	{
		//���³��̶ȣ����ն̶̿�
		m_MarkUnit=1;
        int year=syear;
        int mon=smon;
		for(int year=syear,mon=smon;;year=mon==12?year+1:year,mon=mon==12?1:mon+1)
        {
			if(m_WorkingTimespan.isLocalTzDST()) tmputc=(int)CEasyTime(year,mon,1).gettimeb().time;
			else tmputc=(int)CEasyTime2(year,mon,1,0,0,0,m_WorkingTimespan.getTimezoneDst()).gettimeb().time;
			if(tmputc>=endtime) break;

            if(m_Spans.isInTimespans(tmputc)) m_LongScaleUTC.insert(tmputc);
            else
            {
                it_TimeSpans=mapTimeSpans.lower_bound(tmputc);
                if(it_TimeSpans!=mapTimeSpans.end()) m_LongScaleUTC.insert(it_TimeSpans->first);
            }

			int day=(year==syear&&mon==smon?sday:1);
			if(m_WorkingTimespan.isLocalTzDST()) tmputc=(int)CEasyTime(year,mon,day).gettimeb().time;
			else tmputc=(int)CEasyTime2(year,mon,1,0,0,0,m_WorkingTimespan.getTimezoneDst()).gettimeb().time;
			for(;day<=31&&tmputc<endtime;day++,tmputc+=24*3600)
                if(m_Spans.isInTimespans(tmputc)) m_ShortScaleUTC.insert(tmputc);
        }
	}
	else if(SpanDays>3)
	{
		//���ճ��̶ȣ���Сʱ�̶̿�
		m_MarkUnit=2;
		for(int dayutc=(int)(starttime-SecondsIntraday);dayutc<endtime;dayutc+=24*3600)
        {
            if(m_Spans.isInTimespans(dayutc)) m_LongScaleUTC.insert(dayutc);
            else
            {
                it_TimeSpans=mapTimeSpans.lower_bound(dayutc);
                if(it_TimeSpans!=mapTimeSpans.end()) m_LongScaleUTC.insert(it_TimeSpans->first);
            }

			for(int hhutc=dayutc;hhutc<dayutc+24*3600;hhutc+=3600)
				if(m_Spans.isInTimespans(hhutc)) m_ShortScaleUTC.insert(hhutc);
        }
	}
    else if(m_totalunits>=3600)
    {
		//��Сʱ���̶ȣ���5���Ӷ̶̿�
		m_MarkUnit=3;
		for(int hhutc=(int)(starttime-SecondsIntraday+shour*3600);hhutc<endtime;hhutc+=3600)
		{
            if(m_Spans.isInTimespans(hhutc)) m_LongScaleUTC.insert(hhutc);
            else
            {
                it_TimeSpans=mapTimeSpans.lower_bound(hhutc);
                if(it_TimeSpans!=mapTimeSpans.end()) m_LongScaleUTC.insert(it_TimeSpans->first);
            }

			for(int mmutc=hhutc;mmutc<hhutc+3600;mmutc+=300)
				if(m_Spans.isInTimespans(mmutc)) m_ShortScaleUTC.insert(mmutc);
		}
    }
	else
	{
		//��5���ӳ��̶ȣ���1���Ӷ̶̿�
		m_MarkUnit=4;
		for(int mm5utc=(int)(starttime-SecondsIntraday+shour*3600+(smin-smin%5)*60);mm5utc<endtime;mm5utc+=300)
		{
            if(m_Spans.isInTimespans(mm5utc)) m_LongScaleUTC.insert(mm5utc);
            else
            {
                it_TimeSpans=mapTimeSpans.lower_bound(mm5utc);
                if(it_TimeSpans!=mapTimeSpans.end()) m_LongScaleUTC.insert(it_TimeSpans->first);
            }

			for(int mm1utc=mm5utc;mm1utc<mm5utc+300;mm1utc+=60)
				if(m_Spans.isInTimespans(mm1utc)) m_ShortScaleUTC.insert(mm1utc);
		}
	}

	//����̶�λ��
    set<time_t>::const_iterator it;
    for(it=m_BreakScaleUTC.begin();it!=m_BreakScaleUTC.end();it++)
        m_BreakScalePos.insert(getEntireUnitOffbyX((int)(*it)));
    for(it=m_LongScaleUTC.begin();it!=m_LongScaleUTC.end();it++)
        m_LongScalePos.insert(getEntireUnitOffbyX((int)(*it)));
    for(it=m_ShortScaleUTC.begin();it!=m_ShortScaleUTC.end();it++)
        m_ShortScalePos.insert(getEntireUnitOffbyX((int)(*it)));
}

//X�Ƿ��Ѿ���ʱ/����
bool CTimeLineWin::Stru_Data::isovertime(int iX) const
{
    return iX<m_Spans.getFirstTimespan().start||iX>=m_Spans.getLastTimespan().end ? true : false;
}
//��������/��Ԫ��ƫ����ȡX
int CTimeLineWin::Stru_Data::getXbyEntireUnitOff(int UnitOff) const
{
    if(UnitOff<0) return m_Spans.getFirstTimespan().start;
    if(UnitOff>=m_totalunits) return m_Spans.getLastTimespan().end;
    return m_Spans.getTimeByOffset(UnitOff);
}
//����Xȡ����/��Ԫ��ƫ����, �Ƿ��򷵻�-1
int CTimeLineWin::Stru_Data::getEntireUnitOffbyX(int iX) const
{
	if(m_totalunits==0) return 0;

	//Ϊ���ٲ��ң�ʹ�û���
	if(m_CacheTimespanOffset!=-1&&m_CacheTimespanStart!=-1&&m_CacheTimespanEnd!=-1&&
		iX>=m_CacheTimespanStart&&iX<m_CacheTimespanEnd)
		return (int)(iX-m_CacheTimespanStart+m_CacheTimespanOffset);

    return m_Spans.getOffsetByTime2(iX,
		*(const_cast<int*>(&m_CacheTimespanOffset)),
		*(const_cast<int*>(&m_CacheTimespanStart)),
		*(const_cast<int*>(&m_CacheTimespanEnd)));
}

//ȡÿ��Ϸ�������
int CTimeLineWin::Stru_Data::getSeconds_EachDay(void) const
{
	return m_totalunits&&m_bTimeBased ? m_WorkingTimespan.getSeconds_EachDay() : 0;
}









CTimeLineWin::CTimeLineWin(bool bTimeBased)
:	m_MaxUnitPerWnd(0),
	m_DispWidPerWnd(0),	
	m_ZoomFactor(0),
	m_EntireUnitOff4Disp(0),
	m_EntireUnitCnt4Disp(0),
	m_TimeTipWnd(0),
	m_TimeTipPos(-1),
	m_IntegralPointUnitSpace(-1),
	m_MemBmp(0)
{
	m_Data.m_bTimeBased=bTimeBased;
}


CTimeLineWin::~CTimeLineWin()
{
	if(m_MemBmp)
	{
		DeleteObject((HBITMAP)m_MemBmp);
		m_MemBmp=0;
	}
}

//����ʱ���ᴰ�ھ���������winform����ʱ���ᴰ�ڣ��������������������hWnd���
void CTimeLineWin::AddPartWnd(int hWnd,bool brefresh)
{
	if(!hWnd) return;
	m_hWnds.push_back(hWnd);
	if(brefresh) refresh(false,true,true,true);
}
//�Ӵ��ڼ���ɾ��һ������
void CTimeLineWin::RemovePartWnd(int hWnd,bool brefresh)
{
	if(!hWnd) return;
	int wndord=getWndOrd(hWnd);
	if(wndord==-1) return;
	m_hWnds.erase(m_hWnds.begin()+wndord);
	if(brefresh) refresh(false,true,true,true);
}

//ˢ�������Լ���ʾ������
//brecalcu:�Ƿ���Ҫ���¼��㡣����ʱ�����򡢽���ʱ�κ���Ҫ���¼��㡣ʱ�����ݱ仯ʱ��Ӧ��Ϊtrue
//bsetfullwnd:�Ƿ���Ҫ�����޸���������ʹ֮ȫ����ʾ��ʱ�����ݱ仯ʱ�����������仯ʱ��Ӧ��Ϊtrue
//bgetwndwid:�Ƿ���Ҫ���»�ȡ�ִ��ڿ�ȡ����������仯ʱ���Լ����ڳߴ�仯ʱӦ��Ϊtrue
//brepaint:�Ƿ�ˢ��ȫ���ִ���
void CTimeLineWin::refresh(bool brecalcu,bool bsetfullwnd,bool bgetwndwid,bool brepaint) 
{ 
	//���¼�������
	if(brecalcu)
		m_Data.recalcu();

	//��������ִ��ڿ�Ⱥ��ܿ��
	if(bgetwndwid)
	{
		m_WidPerWnd = m_hWnds.size()>0 ? CTools_Win32::GetClientWid((HWND)m_hWnds[0]) : 0;
	}

	//��Ҫ�����������ӣ�ʹ����ʱ������ȫ����ʾ
	if(bsetfullwnd&&(int)m_hWnds.size()>0)
	{
		m_ZoomFactor=0;
		if((m_WidPerWnd-m_Cfg.LeftSpaceWid)*(int)m_hWnds.size()<=m_Data.m_totalunits)
		{
			for( ; m_ZoomFactor>=-16&&getUnitCntByDispWid(m_WidPerWnd-m_Cfg.LeftSpaceWid)*(int)m_hWnds.size()<m_Data.m_totalunits ; m_ZoomFactor--);
		}
		else
		{
			for( ; m_ZoomFactor<16&&getUnitCntByDispWid(m_WidPerWnd-m_Cfg.LeftSpaceWid)*(int)m_hWnds.size()>m_Data.m_totalunits ; m_ZoomFactor++);
			if(m_ZoomFactor<16&&m_ZoomFactor>0) 
			{
				//��ͷ�ˣ���Сһ������
				m_ZoomFactor--;
			}
		}

		m_EntireUnitOff4Disp=0;
		m_EntireUnitCnt4Disp=m_Data.m_totalunits;
	}

	m_MaxUnitPerWnd=(int)m_hWnds.size()>0?getUnitCntByDispWid(m_WidPerWnd-m_Cfg.LeftSpaceWid):0;
	m_DispWidPerWnd=(int)m_hWnds.size()>0?getDispWidByUnitCnt(m_MaxUnitPerWnd):0;

	//����ʱ�������ʾ��Χ
	RegularOff4Disp_Internal(); 

	//ˢ��ȫ���ִ��ڵ���ʾ
	if(brepaint)
		repaint(NULL);
}

//�淶��m_Off4Disp��m_Wid4Disp
void CTimeLineWin::RegularOff4Disp_Internal(void)
{
	int DispWid=m_DispWidPerWnd*m_hWnds.size();
	int MaxDispUnits = getUnitCntByDispWid(DispWid);

	//�ȵ���m_EntireUnitCnt4Disp�������ĳ�������
	m_EntireUnitCnt4Disp=MaxDispUnits;
	if(m_EntireUnitCnt4Disp>m_Data.m_totalunits) m_EntireUnitCnt4Disp=m_Data.m_totalunits;

	//��m_EntireUnitOff4Disp�Ƿ�Խ��
	if(m_EntireUnitOff4Disp>m_Data.m_totalunits) m_EntireUnitOff4Disp=m_Data.m_totalunits;
	if(m_EntireUnitOff4Disp<0) m_EntireUnitOff4Disp=0;

	//�ж�m_EntireUnitOff4Disp+m_EntireUnitCnt4Disp�Ƿ�Խ��
	if(m_EntireUnitOff4Disp+m_EntireUnitCnt4Disp>m_Data.m_totalunits) m_EntireUnitOff4Disp=m_Data.m_totalunits-m_EntireUnitCnt4Disp;


	//��ʾ�Ŀ�ʼλ�úͽ���λ�õ�UTC
	int StartX4Disp=m_Data.getXbyEntireUnitOff(m_EntireUnitOff4Disp);					
	int EndX4Disp=m_Data.getXbyEntireUnitOff(m_EntireUnitOff4Disp+m_EntireUnitCnt4Disp);			


	//������ʾ��Χ�ڣ������ߵ�λ�á��������32������ʱ��Ч�������߿�����Сʱ��30�֡�15�֡�5�֡�1�֡�30�롢15�롢5�롢1����
	m_IntegralPointPos.clear();
	m_IntegralPointX.clear();
	m_IntegralPointUnitSpace=-1;
	const int pixels=32;														//��������С���

	if(m_Data.m_bTimeBased)
	{
		int HourSpace=getDispWidByUnitCnt(3600);								//3600�����ʾ���(���)
		if(HourSpace/3600>=pixels) m_IntegralPointUnitSpace=1;					//ÿ�뻭һ��������
		else if(HourSpace/(3600/   5)>=pixels) m_IntegralPointUnitSpace=5;		//ÿ5�뻭һ��������
		else if(HourSpace/(3600/  15)>=pixels) m_IntegralPointUnitSpace=15;		//ÿ15�뻭һ��������
		else if(HourSpace/(3600/  30)>=pixels) m_IntegralPointUnitSpace=30;		//ÿ30�뻭һ��������
		else if(HourSpace/(3600/  60)>=pixels) m_IntegralPointUnitSpace=60;		//ÿ���ӻ�һ��������
		else if(HourSpace/(3600/ 300)>=pixels) m_IntegralPointUnitSpace=300;	//ÿ5���ӻ�һ��������
		else if(HourSpace/(3600/ 900)>=pixels) m_IntegralPointUnitSpace=900;	//ÿ15���ӻ�һ��������
		else if(HourSpace/(3600/1800)>=pixels) m_IntegralPointUnitSpace=1800;	//ÿ30���ӻ�һ��������
		else if(HourSpace/(3600/3600)>=pixels) m_IntegralPointUnitSpace=3600;	//ÿСʱ��һ��������
		else m_IntegralPointUnitSpace=-1;
	}
	else
	{
		for(m_IntegralPointUnitSpace=1;getDispWidByUnitCnt(m_IntegralPointUnitSpace)<pixels&&m_IntegralPointUnitSpace<100000000;m_IntegralPointUnitSpace*=5);
		if(m_IntegralPointUnitSpace>=100000000) m_IntegralPointUnitSpace=-1;
	}

	if(m_IntegralPointUnitSpace!=-1)
	{
		int iX = StartX4Disp%m_IntegralPointUnitSpace==0 ? StartX4Disp : StartX4Disp-StartX4Disp%m_IntegralPointUnitSpace+m_IntegralPointUnitSpace;
		for(;iX<EndX4Disp;iX+=m_IntegralPointUnitSpace)
		{
			int unitoff=m_Data.getEntireUnitOffbyX(iX);  
			if(unitoff>=m_EntireUnitOff4Disp)
			{
				m_IntegralPointPos.push_back(getDispWidByUnitCnt(unitoff-m_EntireUnitOff4Disp));
				m_IntegralPointX.push_back(iX);
			}
		}
	}
}

//ˢ�´�����ʾ
//hWndΪNULLˢ��ȫ���ִ�����ʾ������ˢ��ָ���ִ�����ʾ
void CTimeLineWin::repaint(int hWnd)
{
	if(!hWnd)
	{
		for(int i=0;i<(int)m_hWnds.size();i++)
		{
			::InvalidateRect((HWND)m_hWnds[i],NULL,FALSE);
			::SendMessage((HWND)m_hWnds[i],WM_PAINT,0,0);
		}
	}
	else
	{
		::InvalidateRect((HWND)hWnd,NULL,FALSE);
		::SendMessage((HWND)hWnd,WM_PAINT,0,0);
	}
}

void CTimeLineWin::SetTimeTip(int hWnd,int pox)
{
	int oldtipwnd=m_TimeTipWnd;

	m_TimeTipWnd=hWnd;
	m_TimeTipPos=pox;

	if(oldtipwnd!=NULL)
		repaint(oldtipwnd);
	if(hWnd!=NULL&&hWnd!=oldtipwnd)
		repaint(hWnd);
}

void CTimeLineWin::ClearTimeTip(void)
{
	int oldtipwnd=m_TimeTipWnd;
	m_TimeTipWnd=NULL;
	m_TimeTipPos=-1;
	if(oldtipwnd!=NULL)
		repaint(oldtipwnd);
}
//��ȡ�������ʱ��tip�ĵ�ǰλ��
void CTimeLineWin::getTimeTipPos(int& hWnd,int& tippos)
{
	hWnd=m_TimeTipWnd;
	tippos=m_TimeTipPos;
}

//ʱ����Ŵ�һ����(��������m_ZoomFactor��һ����ʹ��ʾ���ݱ��)��ע�⣺�ڲ��Ѿ�refresh��
bool CTimeLineWin::zoomin(void) 
{ 
	if(m_ZoomFactor>=16) return false;
	if(m_MaxUnitPerWnd<4) return false;
	m_ZoomFactor++;
	m_EntireUnitOff4Disp+=m_EntireUnitCnt4Disp/4;
	m_EntireUnitCnt4Disp/=2;
	refresh(false,false,false,true); 
	return true;
}
//ʱ���ᴰ����Сһ����(��������m_ZoomFactor��һ����ʹ��ʾ���ݱ�С)��ע�⣺�ڲ��Ѿ�refresh��
bool CTimeLineWin::zoomout(void) 
{ 
	if(m_ZoomFactor<=-16) return false;
	if(getUnitCntByDispWid(m_DispWidPerWnd*m_hWnds.size())>m_Data.m_totalunits) return false;

	m_ZoomFactor--;
	m_EntireUnitOff4Disp-=m_EntireUnitCnt4Disp/2;
	m_EntireUnitCnt4Disp*=2;
	refresh(false,false,false,true); 
	return true;
}
//ʱ��������һ��ƫ������(�������ǰ����ͷ����m_EntireUnitOff4Disp���ƣ���ʹ��ʾ��ߵ�����)��ע�⣺�ڲ��Ѿ�refresh��
bool CTimeLineWin::movebyleftkey(int offset)
{
	if(m_EntireUnitOff4Disp>0)
	{
		int unitcnt=getUnitCntByDispWid(offset);
		if(unitcnt<=0) unitcnt=1;
		m_EntireUnitOff4Disp-=unitcnt;
		refresh(false,false,false,true); 
		return true;
	}
	else return false;
}
//ʱ��������һ��ƫ������(�������ǰ��Ҽ�ͷ����m_EntireUnitOff4Disp���ƣ���ʹ��ʾ�ұߵ�����)��ע�⣺�ڲ��Ѿ�refresh��
bool CTimeLineWin::movebyrightkey(int offset)
{
	if(m_EntireUnitOff4Disp+m_EntireUnitCnt4Disp<m_Data.m_totalunits)
	{
		int unitcnt=getUnitCntByDispWid(offset);
		if(unitcnt<=0) unitcnt=1;
		m_EntireUnitOff4Disp+=unitcnt;
		refresh(false,false,false,true); 
		return true;
	}
	else return false;
}



//����x��λ��ȡ��Ӧʱ�䡣xPos����LeftSpaceWid
bool CTimeLineWin::getX(int hWnd,int xPos,int& rltX) const 
{ 
	if(xPos<m_Cfg.LeftSpaceWid||xPos>m_Cfg.LeftSpaceWid+m_DispWidPerWnd) return false;
	int WndOrd=getWndOrd(hWnd);
	if(WndOrd==-1) return false;

	int unitoff=getUnitCntByDispWid(WndOrd*m_DispWidPerWnd+xPos-m_Cfg.LeftSpaceWid);
	if(unitoff>=m_EntireUnitCnt4Disp) return false;
	rltX=(int)m_Data.getXbyEntireUnitOff(unitoff+m_EntireUnitOff4Disp); 
	return true;
}
//����ʱ��ȡx��λ�á����ص�rltPos����LeftSpaceWid�����utc������Чʱ����ڣ�����false
bool CTimeLineWin::getxPos(int iX,int& hWnd,int& rltPos) const 
{ 
	int unitoff=m_Data.getEntireUnitOffbyX(iX);  
	if(unitoff<0) return false; 
	int totalpos=getDispWidByUnitCnt(unitoff-m_EntireUnitOff4Disp);
	int wndord=totalpos/m_DispWidPerWnd;
	if(wndord>=(int)m_hWnds.size()) return false;
	hWnd=m_hWnds[wndord];
	rltPos=totalpos%m_DispWidPerWnd+m_Cfg.LeftSpaceWid;
	return true;
}
//����ʱ����ָ��������ȡx��λ�á����ص�rltPos����LeftSpaceWid�����utc���ڴ�����Чʱ����ڣ�����false
bool CTimeLineWin::getxPosInWnd(int iX,int WndOrd,int& rltPos) const
{
	if(WndOrd<0||WndOrd>=(int)m_hWnds.size()) return false;
	int unitoff=m_Data.getEntireUnitOffbyX(iX);  
	if(unitoff<0) return false; 
	int totalpos=getDispWidByUnitCnt(unitoff-m_EntireUnitOff4Disp);
	if(WndOrd!=totalpos/m_DispWidPerWnd) return false;
	rltPos=totalpos%m_DispWidPerWnd+m_Cfg.LeftSpaceWid;
	return true;
}


//��ȡָ���ִ��ڵĲ�����SecOff�Ǵ�����m_DispWids�ϵ�����ƫ��,-1Ϊ��Ч��SecCnt�Ǳ�����Ҫ��ʾ������
bool CTimeLineWin::GetDispOff_SecOff_SecCnt(int wndord,int& WndWid,int& DispOff,int& UnitOff,int& UnitCnt)
{
	if(wndord<0||wndord>=(int)m_hWnds.size()) return false;
	DispOff=wndord*m_DispWidPerWnd;
	WndWid=m_WidPerWnd;
	UnitOff=wndord*m_MaxUnitPerWnd;
	if(UnitOff>=m_EntireUnitCnt4Disp) { UnitOff=-1;  UnitCnt=0; }
	else { UnitCnt=m_EntireUnitCnt4Disp-UnitOff;  if(UnitCnt>m_MaxUnitPerWnd) UnitCnt=m_MaxUnitPerWnd; }
	return true;
}


//���ָ���ִ��ڣ���MemDC��Paint
void CTimeLineWin::PaintOnMemDC_Internal(int hWnd,int MemDC)
{
	//WndOrd�Ǵ��ڵ����
	int WndOrd=getWndOrd(hWnd);
	if(WndOrd==-1) return;

	//WndWidΪ�˴��ڵĿ��
	int DispOff=getDispOff2(WndOrd);
	int WndWid=m_WidPerWnd;

	//ʱ���ᴰ�ڵı���ˢ�����ʺ�����
    HBRUSH	hBrBk=CreateSolidBrush(m_Cfg.BkCol);
	RECT Rect={0,0,WndWid,m_Cfg.WndHei};
    FillRect((HDC)MemDC,&Rect,hBrBk);
	DeleteObject(hBrBk);

    HPEN	hPen=CreatePen(PS_SOLID,1,m_Cfg.LineCol);
    HPEN	hOldPen=(HPEN)SelectObject((HDC)MemDC,hPen);

	HFONT   hFont=CreateFont(-1*m_Cfg.FontSize,0,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,0,m_Cfg.FontName);
    HFONT	hOldFont=(HFONT)SelectObject((HDC)MemDC,hFont);

    SetTextColor((HDC)MemDC,m_Cfg.FontCol);
    SetBkMode((HDC)MemDC,TRANSPARENT);

    //��������
	int pox=m_Cfg.LeftSpaceWid>0?m_Cfg.LeftSpaceWid-1:0;
	MoveToEx((HDC)MemDC,pox,0,NULL);
    LineTo((HDC)MemDC,WndWid,0);

    set<int>::const_iterator it_pos,it_pos2,it_postmp;

    //����ÿ��ʱ��εĿ�ʼ�����̶̿���
	it_pos=m_Data.m_BreakScalePos.lower_bound(getEntireUnitOff(WndOrd));
	it_pos2=m_Data.m_BreakScalePos.lower_bound(getEntireUnitOff(WndOrd)+getUnitCnt(WndOrd));
    for(;it_pos!=m_Data.m_BreakScalePos.end();it_pos++)
    {
		pox=getDispWidByUnitCnt((*it_pos)-m_EntireUnitOff4Disp);
		if(pox>=DispOff&&pox<=DispOff+m_DispWidPerWnd)
		{
			pox=pox-DispOff+m_Cfg.LeftSpaceWid;
			if(pox!=m_Cfg.LeftSpaceWid)
			{
				MoveToEx((HDC)MemDC,pox,m_Cfg.TimespanStartScaleHei,NULL);
				LineTo((HDC)MemDC,pox,0);
			}
		}
		if(it_pos==it_pos2) break;
    }

    //�����̶���
	it_pos=m_Data.m_LongScalePos.lower_bound(getEntireUnitOff(WndOrd));
	it_pos2=m_Data.m_LongScalePos.lower_bound(getEntireUnitOff(WndOrd)+getUnitCnt(WndOrd));
    for(;it_pos!=it_pos2;it_pos++)
    {
		int pox=getDispWidByUnitCnt((*it_pos)-m_EntireUnitOff4Disp);
		if(pox>=DispOff&&pox<=DispOff+m_DispWidPerWnd)
		{
			pox=pox-DispOff+m_Cfg.LeftSpaceWid;
			if(pox!=m_Cfg.LeftSpaceWid)
			{
				MoveToEx((HDC)MemDC,pox,m_Cfg.LongScaleHei,NULL);
				LineTo((HDC)MemDC,pox,0);
			}
		}
    }

    //���̶̿���
	int lastpox=-1;		//��һ�εĿ̶��ߣ��̶̿���֮��Ҫ���8���ϣ�������������
	it_pos=m_Data.m_ShortScalePos.lower_bound(getEntireUnitOff(WndOrd));
	it_pos2=m_Data.m_ShortScalePos.lower_bound(getEntireUnitOff(WndOrd)+getUnitCnt(WndOrd));
    for(;it_pos!=it_pos2;it_pos++)
    {
		int pox=getDispWidByUnitCnt((*it_pos)-m_EntireUnitOff4Disp);
		if(pox>=DispOff&&pox<=DispOff+m_DispWidPerWnd)
		{
			pox=pox-DispOff+m_Cfg.LeftSpaceWid;
			if(pox!=m_Cfg.LeftSpaceWid&&(lastpox==-1||pox-lastpox>=8))
			{
				MoveToEx((HDC)MemDC,pox,m_Cfg.ShortScaleHei,NULL);
				LineTo((HDC)MemDC,pox,0);
				lastpox=pox;
			}
		}
    }

    //��ע���̶�
	vector<int> scaletxtpox;
	vector<int> scaletxtwid;
    lastpox=-1;
	it_pos=m_Data.m_LongScalePos.lower_bound(getEntireUnitOff(WndOrd));
	it_pos2=m_Data.m_LongScalePos.lower_bound(getEntireUnitOff(WndOrd)+getUnitCnt(WndOrd));
	set<time_t>::const_iterator it_utc=m_Data.m_LongScaleUTC.begin();
	for(it_postmp=m_Data.m_LongScalePos.begin();it_postmp!=it_pos&&it_utc!=m_Data.m_LongScaleUTC.end();it_postmp++,it_utc++);
	//��ʱit_pos��it_utc�Ƕ�Ӧ�ĵ�����
	for(;it_pos!=it_pos2&&it_utc!=m_Data.m_LongScaleUTC.end();it_pos++,it_utc++)
    {
		int pox=getDispWidByUnitCnt((*it_pos)-m_EntireUnitOff4Disp);

		//����Խ�����
		if(pox<DispOff||pox>=DispOff+m_DispWidPerWnd) continue;

		pox=pox-DispOff+m_Cfg.LeftSpaceWid;

		//�������һ���̶ȱ�ע�غ�
		if(lastpox!=-1&&pox<lastpox+8) continue;

		char buf[256]={0,};
		memset(buf,0,sizeof(buf));
        short year,month,day,hour,min,sec,wday;
		if(m_Data.m_WorkingTimespan.isLocalTzDST()) CEasyTime(*it_utc).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
		else CEasyTime2(*it_utc,0,m_Data.m_WorkingTimespan.getTimezoneDst()).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
		switch(m_Data.m_MarkUnit)
		{
			case 0:						//���곤�̶ȣ����¶̶̿�
			case 1:						//���³��̶ȣ����ն̶̿�
			case 2:						//���ճ��̶ȣ���Сʱ�̶̿�
			{
				if(m_Data.m_MarkUnit==0) sprintf(buf,"%04d/",year);
				sprintf(buf+strlen(buf),"%02d/%02d/",month,day);
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
				break;
			}
			case 3:						//��Сʱ���̶ȣ���5���Ӷ̶̿�
			{
				sprintf(buf,"%02d:%02d",hour,min);
				break;
			}
			case 4:						//��5���ӳ��̶ȣ���1���Ӷ̶̿�
			{
				sprintf(buf,"%02d",min);
				break;
			}
		}

        SIZE Sz=CTools_Win32::GetTextoutSize3((HDC)MemDC,buf,strlen(buf));
        int ipox;
        if(m_Cfg.MarkPos==1) ipox=pox+2;		//�̶�д���ұ�
        else ipox=pox-Sz.cx/2;					//�̶�д���м�
		if(ipox<0&&ipox+Sz.cx>0) ipox=0;

		//�ٴα������һ���̶ȱ�ע�غ�
		if(lastpox!=-1&&ipox<lastpox+8) continue;

		TextOut((HDC)MemDC,ipox,m_Cfg.WndHei-Sz.cy,buf,strlen(buf));
        lastpox=ipox+Sz.cx;
		scaletxtpox.push_back(ipox);
		scaletxtwid.push_back(Sz.cx);
    }

	//��ע�����ߵ�ʱ�䣬Ҫ�ܿ����̶ȱ�ע�ķ�Χ
    SetTextColor((HDC)MemDC,m_Cfg.FontCol/2);
	lastpox=-1;
	for(int i=0,ilongscale=0;i<(int)m_IntegralPointPos.size();i++)
	{
		int pox=m_IntegralPointPos[i];

		//����Խ�����
		if(pox<DispOff) continue;
		if(pox>=DispOff+m_DispWidPerWnd) break;

		pox=pox-DispOff+m_Cfg.LeftSpaceWid;

		char buf[256];
		short hour,min,sec;
		if(!m_Data.m_bTimeBased)
		{
			sprintf(buf,"%d",m_IntegralPointX[i]);
		}
		else
		{
			CEasyTime(m_IntegralPointX[i]).convert2fileds(NULL,NULL,NULL,&hour,&min,&sec);
			if(m_IntegralPointUnitSpace>=3600) 
			{
				sprintf(buf,"%d",hour);
			}
			else if(m_IntegralPointUnitSpace>=60) 
			{
				if(min==0) sprintf(buf,"%02d:%02d",hour,min);
				else sprintf(buf,"%d'",min);
			}
			else 
			{
				if(sec==0) sprintf(buf,"%02d:%02d",hour,min);
				else sprintf(buf,"%d\"",sec);
			}
		}

		SIZE Sz=CTools_Win32::GetTextoutSize3((HDC)MemDC,buf,strlen(buf));
		int ipox=pox-Sz.cx/2;

		if(lastpox!=-1&&ipox<=lastpox+4) 
			continue;

		int boverlap=false;		//�Ƿ��ص�
		while(ilongscale<(int)scaletxtpox.size())
		{
			//��Ҫ�ж��Ƿ�ͳ��̶ȱ�ע�ص�
			int scaletxtleft=scaletxtpox[ilongscale];
			int scaletxtright=scaletxtpox[ilongscale]+scaletxtwid[ilongscale];
			if(scaletxtleft<=ipox)
			{
				if(scaletxtright+8<=ipox)
				{
					//�����һ�����̶ȱ�ע
					ilongscale++;
					continue;
				}
				else
				{
					//���ص�
					boverlap=true;
					break;
				}
			}
			else
			{
				if(scaletxtleft>=ipox+Sz.cx+8)
				{
					//û���ص��������˳���
					break;
				}
				else
				{
					//���ص�
					boverlap=true;
					break;
				}
			}
		}

		if(!boverlap)
		{
			TextOut((HDC)MemDC,ipox,m_Cfg.WndHei-Sz.cy,buf,strlen(buf));
			lastpox=ipox+Sz.cx;
		}
	}
    SetTextColor((HDC)MemDC,m_Cfg.FontCol);

	//��עTimeTip
	if(m_TimeTipWnd==hWnd&&
		m_TimeTipPos>=m_Cfg.LeftSpaceWid&&m_TimeTipPos<m_DispWidPerWnd+m_Cfg.LeftSpaceWid)
	{
        char buf[256];
		int TimeTipX;
		if(getX(hWnd,m_TimeTipPos,TimeTipX))
		{
			HPEN TipLinePen=CreatePen(PS_SOLID,1,m_Cfg.TipLineCol);
			HPEN hOldPen=(HPEN)SelectObject((HDC)MemDC,TipLinePen);
			MoveToEx((HDC)MemDC,m_TimeTipPos,1,NULL);
			LineTo((HDC)MemDC,m_TimeTipPos,m_Cfg.WndHei);
			SelectObject((HDC)MemDC,hOldPen);
			DeleteObject(TipLinePen);

			memset(buf,0,sizeof(buf));
			if(m_Data.m_bTimeBased)
			{
				short year,month,day,hour,min,sec,wday;

				if(m_Data.m_WorkingTimespan.isLocalTzDST()) CEasyTime((time_t)TimeTipX).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
				else CEasyTime2((time_t)TimeTipX,0,m_Data.m_WorkingTimespan.getTimezoneDst()).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);

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
				sprintf(buf+strlen(buf),"%d:%d:%d",hour,min,sec);
			}
			else sprintf(buf,"%d",TimeTipX);

			SIZE Sz=CTools_Win32::GetTextoutSize3((HDC)MemDC,buf,strlen(buf));
			int txtpox=m_TimeTipPos-Sz.cx/2;
			if(txtpox+Sz.cx>WndWid) txtpox=WndWid-Sz.cx;
			if(txtpox<0) txtpox=0;
			SetTextColor((HDC)MemDC,RGB(255,255,255));
			SetBkColor((HDC)MemDC,RGB(0,0,255));
			SetBkMode((HDC)MemDC,OPAQUE);
			TextOut((HDC)MemDC,txtpox,m_Cfg.WndHei-1-Sz.cy,buf,strlen(buf));
		}
	}


	if(hOldPen) SelectObject((HDC)MemDC,hOldPen);
	if(hOldFont) SelectObject((HDC)MemDC,hOldFont);

	if(hPen) DeleteObject(hPen);
	if(hFont) DeleteObject(hFont);
}

void CTimeLineWin::OnPaint(int hWnd,int iDC) 
{
	int WndOrd=getWndOrd(hWnd);
	if(WndOrd==-1) return;
	int WndWid=m_WidPerWnd;

	HBITMAP hMemBmp=CreateCompatibleBitmap((HDC)iDC,WndWid,m_Cfg.WndHei);
	if(!hMemBmp) return;

	HDC		MemDC=CreateCompatibleDC((HDC)iDC);
	HBITMAP hOldBmp=(HBITMAP)SelectObject((HDC)MemDC,hMemBmp);

	PaintOnMemDC_Internal(hWnd,(int)MemDC);
	BitBlt((HDC)iDC,0,0,WndWid,m_Cfg.WndHei,MemDC,0,0,SRCCOPY);

	if(hOldBmp) SelectObject((HDC)MemDC,hOldBmp);
	if(hMemBmp) DeleteObject(hMemBmp);
	DeleteDC(MemDC);
}

