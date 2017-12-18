#pragma once
#include "timelinewin2.h"

class CTimeLineWinEx :
    public CTimeLineWin2
{
public:
//     CTimeLineWinEx(void);
    CTimeLineWinEx(const Stru_TimeLineWin2_Cfg& cfg);
    ~CTimeLineWinEx(void);

    void SetPaintFlag(int nFlag) { m_nPaintFlag = nFlag; }
    void SetPaint(int bEnable) { m_bPaint = bEnable; }

private:
    void OnPaint(HWND hWnd);

    int m_nPaintFlag;                           //0:���ɽ�����ʱ����״ͼ 1:���ɽ����������״ͼ -1:Ĭ��
    BOOL m_bPaint;                              //�Ƿ�ʼ��
};
