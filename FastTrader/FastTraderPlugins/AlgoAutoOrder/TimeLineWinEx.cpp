#include "StdAfx.h"
#include "TimeLineWinEx.h"
using std::set;

CTimeLineWinEx::CTimeLineWinEx(const Stru_TimeLineWin2_Cfg& cfg)
: CTimeLineWin2(cfg)
, m_nPaintFlag(-1)
, m_bPaint(FALSE)
{
}

CTimeLineWinEx::~CTimeLineWinEx(void)
{
}

void CTimeLineWinEx::OnPaint(HWND hWnd)
{
    RECT Rect;
    int Wid,Hei;

    const Stru_TimeLineWin2_Cfg&  m_cfg=getCfg();              //ʱ��̶ȴ��ڵ����ò���
    const Stru_TimeLineWin2_Data&  m_data=getData();
    int& m_TimeTipPos = getTimeTipPos();


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

    HBRUSH		hBrBk=CreateSolidBrush(m_cfg.BkCol);
    HPEN		hPen=CreatePen(PS_SOLID,1,m_cfg.LineCol);
    HFONT       hFont=CreateFont(-1*m_cfg.FontSize,0,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,0,m_cfg.FontName);

    FillRect(MemDC,&Rect,hBrBk);
    SelectObject(MemDC,hPen);
    SelectObject(MemDC,hFont);
    SetTextColor(MemDC,FontCol);
    SetBkMode(MemDC,TRANSPARENT);

    if( m_nPaintFlag == -1)//Ĭ�ϵ�
    {
        //��������
        MoveToEx(MemDC,0,0,NULL);
        LineTo(MemDC,Wid,0);

        set<int>::const_iterator it_pos;

        //ʱ��εĿ�ʼ���̶̿���
        for(it_pos=m_data.m_BreakScalePos.begin();it_pos!=m_data.m_BreakScalePos.end();it_pos++)
        {
            MoveToEx(MemDC,*it_pos,1,NULL);
            LineTo(MemDC,*it_pos,m_cfg.TimespanStartScaleHei);
            //		SetPixel(MemDC,(*it_pos)-1,1,m_cfg.LineCol);
            //		SetPixel(MemDC,(*it_pos)+1,1,m_cfg.LineCol);
        }

        //�����̶���
        for(it_pos=m_data.m_LongScalePos.begin();it_pos!=m_data.m_LongScalePos.end();it_pos++)
        {
            MoveToEx(MemDC,*it_pos,1,NULL);
            LineTo(MemDC,*it_pos,m_cfg.LongScaleHei);
        }

        //���̶̿���
        for(it_pos=m_data.m_ShortScalePos.begin();it_pos!=m_data.m_ShortScalePos.end();it_pos++)
        {
            MoveToEx(MemDC,*it_pos,1,NULL);
            LineTo(MemDC,*it_pos,m_cfg.ShortScaleHei);
        }

        //��ע���̶�
        it_pos=m_data.m_LongScalePos.begin();
        set<time_t>::const_iterator it_utc=m_data.m_LongScaleUTC.begin();
        int lastlabelpos=-1;
        for(;it_pos!=m_data.m_LongScalePos.end()&&it_utc!=m_data.m_LongScaleUTC.end();it_pos++,it_utc++)
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

    }else if(0 == m_nPaintFlag && m_bPaint)//���ɽ�����ʱ����״ͼ
        {
            //��������
            MoveToEx(MemDC,0,0,NULL);
            LineTo(MemDC,Wid,0);

            set<int>::const_iterator it_pos;

            //ʱ��εĿ�ʼ���̶̿���
            for(it_pos=m_data.m_BreakScalePos.begin();it_pos!=m_data.m_BreakScalePos.end();it_pos++)
            {
                MoveToEx(MemDC,*it_pos,1,NULL);
                LineTo(MemDC,*it_pos,m_cfg.TimespanStartScaleHei);
                //		SetPixel(MemDC,(*it_pos)-1,1,m_cfg.LineCol);
                //		SetPixel(MemDC,(*it_pos)+1,1,m_cfg.LineCol);
            }

            //�����̶���
            for(it_pos=m_data.m_LongScalePos.begin();it_pos!=m_data.m_LongScalePos.end();it_pos++)
            {
                MoveToEx(MemDC,*it_pos,1,NULL);
                LineTo(MemDC,*it_pos,m_cfg.LongScaleHei);
            }

            //���̶̿���
            for(it_pos=m_data.m_ShortScalePos.begin();it_pos!=m_data.m_ShortScalePos.end();it_pos++)
            {
                MoveToEx(MemDC,*it_pos,1,NULL);
                LineTo(MemDC,*it_pos,m_cfg.ShortScaleHei);
            }

            //��ע���̶�
            it_pos=m_data.m_LongScalePos.begin();
            set<time_t>::const_iterator it_utc=m_data.m_LongScaleUTC.begin();
            int lastlabelpos=-1;
            for(;it_pos!=m_data.m_LongScalePos.end()&&it_utc!=m_data.m_LongScaleUTC.end();it_pos++,it_utc++)
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
        }else if (1 == m_nPaintFlag && m_bPaint)//���ɽ����������״ͼ
        {
            //��������
            MoveToEx(MemDC,0,0,NULL);
            LineTo(MemDC,Wid,0);
        }

    if (m_bPaint)
	{
		if(1 == m_nPaintFlag)
		   ::ShowWindow((HWND)this->m_hWnd,FALSE);
		else
		   ::ShowWindow((HWND)this->m_hWnd,TRUE);
	}
    else
        ::ShowWindow((HWND)this->m_hWnd,FALSE);
    
	BitBlt(hDC,0,0,Wid,Hei,MemDC,0,0,SRCCOPY);

	DeleteDC(MemDC);
	DeleteObject(hBmp);
	EndPaint(hWnd,&Ps);

	if(hBrBk) DeleteObject(hBrBk);
	if(hPen) DeleteObject(hPen);
	if(hFont) DeleteObject(hFont);

}