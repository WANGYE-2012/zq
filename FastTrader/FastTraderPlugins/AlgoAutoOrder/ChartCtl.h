#pragma once

#include "TimeLineWinEx.h"
#include "Tools_AlgoTrading.h"
#include "AlgoAutoData.h"

// CChartCtl

#define ClassName_ChartCtl "ChartCtrlClass"

#define BkCol_ChartCtl RGB(204,232,207)
#define njCol_ChartCtl RGB(0,0,255)				//������������״ͼ��ɫ
#define njTradedCol_ChartCtl RGB(0,0,192)		//�������ѳɽ�������״ͼ��ɫ
#define nj2Col_ChartCtl RGB(128,64,0)			//������������״ͼ��ɫ
#define nj2TradedCol_ChartCtl RGB(96,48,0)		//�������ѳɽ�������״ͼ��ɫ
#define VCol_ChartCtl RGB(255,0,0)			    //�������߶�

class CChartCtl : public CWnd
{
	DECLARE_DYNAMIC(CChartCtl)

public:
	CChartCtl();
	virtual ~CChartCtl();
	BOOL RegisterWndClass();

	//�����Ӵ��ڡ���Ϊ�Զ���ؼ��޷���ӦOnCreate�¼�������ֻ����ʽ�Ĵ���
	bool CreateChildren(int nFlag = -1);

    //ʱ�����Ӵ���
 	CTimeLineWinEx m_TimeLineWin;

protected:
	DECLARE_MESSAGE_MAP()

public:
	void setcontent(CAlgoAutoData *pAlgo,time_t StartTradingUTC);
	//ˢ���ѳɽ�����ֻˢ��ָ���ڡ�
	void RefreshTradedVolume(int Type);
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

	CAlgoAutoData *m_pAlgoData;

public:
    // 0���ɽ�����ʱ���ϵ 1���ɽ������µ�����ϵ
    int m_nType;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:
	bool m_bMouseInClient;
	POINT m_LastMousePt;

    bool m_bPaint;


};


