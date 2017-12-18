#pragma once

#include "TimeLineWin2.h"
#include "common-AlgoTradingNo1Dlg.h"
#include "Algo_No1.hpp"

// CChartCtl

#define ClassName_ChartCtl "ChartCtrlClass"

#define BkCol_ChartCtl RGB(204,232,207)
#define njCol_ChartCtl RGB(0,0,255)				//������������״ͼ��ɫ
#define njTradedCol_ChartCtl RGB(0,0,192)		//�������ѳɽ�������״ͼ��ɫ
#define nj2Col_ChartCtl RGB(128,64,0)			//������������״ͼ��ɫ
#define nj2TradedCol_ChartCtl RGB(96,48,0)		//�������ѳɽ�������״ͼ��ɫ

class CChartCtl : public CWnd
{
	DECLARE_DYNAMIC(CChartCtl)

public:
	CChartCtl();
	virtual ~CChartCtl();
	BOOL RegisterWndClass();

	//�����Ӵ��ڡ���Ϊ�Զ���ؼ��޷���ӦOnCreate�¼�������ֻ����ʽ�Ĵ���
	bool CreateChildren(void);

    //ʱ�����Ӵ���
	CTimeLineWin2 m_TimeLineWin;

protected:
	DECLARE_MESSAGE_MAP()

public:
	void setcontent(const CAlgo_No1& algo,time_t StartTradingUTC);
	//ˢ���ѳɽ�����ֻˢ��ָ���ڡ�
	void RefreshTradedVolume(int TradingSession,int TradedVolume);
	//utc�Ƿ��Ѿ���ʱ
	bool isovertime(time_t utc);
    //����ʱ��ȡx��λ��
    int getxPos(time_t utc);
    //ȡʱ���ᴰ�ڿ��
    int getTimeWndWid(void);
    //���ý���ʱ�����
    void setTradingTimespan(const CEasyTradingTimespan& TradingTimespan);
    //ˢ��ʱ��̶ȴ��ڵĲ���
    void refreshTimeLineWin(void);

	CAlgo_No1 m_algo;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:
	bool m_bMouseInClient;
	POINT m_LastMousePt;
};


