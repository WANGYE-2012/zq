#pragma once

//��ɫ
enum BK_COLOR_MODE
{
    Color_Light,    //ǳɫ
    Color_Thick     //��ɫ
};


class CBKColor
{
public:
    CBKColor(void);
    virtual ~CBKColor(void);

private:
    //������
    COLORREF    m_OptionStrategyTitleColorRef;       //����
    COLORREF    m_OptionStrategyClientColorRef;      //�ͻ���

    //T�ͱ��۴���
    COLORREF    m_TPriceTitleColorRef;       //����
    COLORREF    m_TPriceClientColorRef;      //�ͻ���

    //�����µ�����
    COLORREF    m_QuickOrderColorRef;                   //�ͻ���
    COLORREF    m_QuickOrderThumbnailColorRef;          //��������ͼ����
    COLORREF    m_QuickOrderListHeaderCtrlColorRef;     //�б�ͷ
    COLORREF    m_QuickOrderListCtrlColorRef;           //�б���
    COLORREF    m_QuickOrderListItemSelectedColorRef;   //�б�ѡ�б���

    //��������
    COLORREF    m_AnalyseModuleColorRef;    //��������

    COLORREF    m_CustomTabPageSheetNormalColorRef;    //tabCtrl����pageSheet��������
    COLORREF    m_CustomTabPageSheetSelectedColorRef;  //tabCtrl����pageSheet��ѡ����

    COLORREF    m_TabListBoxCtrlIconColorRef;               //TabTradeAnalyse������ListBoxCtrl itemǰ��ͼ�걳��
    COLORREF    m_TabListBoxCtrlIconSelectedColorRef;       //TabTradeAnalyse������ListBoxCtrl item��ѡ��ǰ��ͼ�걳��
    COLORREF    m_TabListBoxCtrlBKColorRef;                 //TabTradeAnalyse������ListBoxCtrl����
    COLORREF    m_TabListBoxCtrlItemTextBKColorRef;         //TabTradeAnalyse������ListBoxCtrl item text����
    COLORREF    m_TabListBoxCtrlItemSelectedColorRef;       //TabTradeAnalyse������ListBoxCtrl item ��ѡ�б���
    COLORREF    m_TabListBoxCtrlItemTextColorRef;           //TabTradeAnalyse������ListBoxCtrl item text��ɫ
    COLORREF    m_TabListBoxCtrlItemTextSelectedBKColorRef; //TabTradeAnalyse������ListBoxCtrl item text��ѡ�б�ɫ
    

    COLORREF    m_TabLineChartCtrlBottomColorRef;  //TabProbaCtrl������LineChartCtrl�ײ㱳��
    COLORREF    m_TabLineChartCtrlTopColorRef;     //TabProbaCtrl������LineChartCtrl���㱳��
    COLORREF    m_TabLineChartCtrlBorderColorRef;  //TabProbaCtrl������LineChartCtrl�߿򱳾�
    COLORREF    m_TabLineChartCtrlLineColorRef;    //TabProbaCtrl������LineChartCtrl��������    
    COLORREF    m_TabLineChartCtrlLostColorRef;    //TabProbaCtrl������LineChartCtrl���𱳾�
    COLORREF    m_TabLineChartCtrlWinColorRef;     //TabProbaCtrl������LineChartCtrlӯ������
    COLORREF    m_TabLineChartCtrlAxisTextColorRef;    //TabProbaCtrl������LineChartCtrl����̶�����
public:
    // ������ɫѡ��ģʽ
    void SetBkColorMode(BK_COLOR_MODE mode);
};
