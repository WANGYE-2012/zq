#include "StdAfx.h"
#include "BKColor.h"

CBKColor::CBKColor(void)
{
    //������
    m_OptionStrategyTitleColorRef = RGB(12,70,144);         //����
    m_OptionStrategyClientColorRef = RGB(226,226,226);      //�ͻ���

    //T�ͱ��۴���
    m_TPriceTitleColorRef = RGB(58,117,164);        //����
    m_TPriceClientColorRef = RGB(246,245,245);      //�ͻ���

    //�����µ�����
    m_QuickOrderColorRef = RGB(226,226,226);                    //�ͻ���
    m_QuickOrderThumbnailColorRef = RGB(156,187,212);           //��������ͼ����
    m_QuickOrderListHeaderCtrlColorRef = RGB(48,107,156);       //�б�ͷ
    m_QuickOrderListCtrlColorRef = RGB(246,245,245);            //�б���
    m_QuickOrderListItemSelectedColorRef = RGB(49,106,197);     //�б�ѡ�б���

    //��������
    m_AnalyseModuleColorRef = RGB(226,226,226);     //��������

    m_CustomTabPageSheetNormalColorRef = RGB(31,119,217);    //tabCtrl����pageSheet��������
    m_CustomTabPageSheetSelectedColorRef = RGB(13,49,89);    //tabCtrl����pageSheet��ѡ����

    m_TabLineChartCtrlBottomColorRef = RGB(249,249,249);    //TabProbaCtrl������LineChartCtrl�ײ㱳��
    m_TabLineChartCtrlTopColorRef = RGB(234,234,234);       //TabProbaCtrl������LineChartCtrl���㱳��
    m_TabLineChartCtrlBorderColorRef = RGB(160,160,160);    //TabProbaCtrl������LineChartCtrl�߿򱳾�
    m_TabLineChartCtrlLostColorRef = RGB(150,199,97);       //TabProbaCtrl������LineChartCtrl���𱳾�
    m_TabLineChartCtrlWinColorRef = RGB(229,181,169);       //TabProbaCtrl������LineChartCtrlӯ������
    m_TabLineChartCtrlAxisTextColorRef = RGB(0,0,0);        //TabProbaCtrl������LineChartCtrl����̶�����
}

CBKColor::~CBKColor(void)
{
}

// ������ɫѡ��ģʽ
void CBKColor::SetBkColorMode(BK_COLOR_MODE mode)
{
    if(Color_Light == mode)
    {
        //������
        m_OptionStrategyTitleColorRef = RGB(12,70,144);         //����
        m_OptionStrategyClientColorRef = RGB(226,226,226);      //�ͻ���

        //T�ͱ��۴���
        m_TPriceTitleColorRef = RGB(58,117,164);        //����
        m_TPriceClientColorRef = RGB(246,245,245);      //�ͻ���

        //�����µ�����
        m_QuickOrderColorRef = RGB(226,226,226);                    //�ͻ���
        m_QuickOrderThumbnailColorRef = RGB(156,187,212);           //��������ͼ����
        m_QuickOrderListHeaderCtrlColorRef = RGB(48,107,156);       //�б�ͷ
        m_QuickOrderListCtrlColorRef = RGB(246,245,245);            //�б���
        m_QuickOrderListItemSelectedColorRef = RGB(49,106,197);     //�б�ѡ�б���

        //��������
        m_AnalyseModuleColorRef = RGB(226,226,226);     //��������

        m_CustomTabPageSheetNormalColorRef = RGB(31,119,217);    //tabCtrl����pageSheet��������
        m_CustomTabPageSheetSelectedColorRef = RGB(13,49,89);    //tabCtrl����pageSheet��ѡ����

        m_TabListBoxCtrlIconColorRef = RGB(243,148,0);                  //TabTradeAnalyse������ListBoxCtrl itemǰ��ͼ�걳��
        m_TabListBoxCtrlIconSelectedColorRef = RGB(243,148,0);          //TabTradeAnalyse������ListBoxCtrl item��ѡ��ǰ��ͼ�걳��
        m_TabListBoxCtrlBKColorRef = RGB(255,255,255);                  //TabTradeAnalyse������ListBoxCtrl����
        m_TabListBoxCtrlItemTextBKColorRef = RGB(255,255,255);          //TabTradeAnalyse������ListBoxCtrl item text���� 
        m_TabListBoxCtrlItemSelectedColorRef = RGB(10,67,137);          //TabTradeAnalyse������ListBoxCtrl item ��ѡ�б���
        m_TabListBoxCtrlItemTextColorRef = RGB(0,0,0);                  //TabTradeAnalyse������ListBoxCtrl item text��ɫ
        m_TabListBoxCtrlItemTextSelectedBKColorRef= RGB(255,255,255);   //TabTradeAnalyse������ListBoxCtrl item text��ѡ�б�ɫ

        m_TabLineChartCtrlBottomColorRef = RGB(249,249,249);    //TabProbaChart������LineChartCtrl�ײ㱳��
        m_TabLineChartCtrlTopColorRef = RGB(234,234,234);       //TabProbaChart������LineChartCtrl���㱳��
        m_TabLineChartCtrlBorderColorRef = RGB(160,160,160);    //TabProbaChart������LineChartCtrl�߿򱳾�
        m_TabLineChartCtrlLineColorRef = RGB(160,160,160);      //TabProbaChart������LineChartCtrl��������
        m_TabLineChartCtrlLostColorRef = RGB(150,199,97);       //TabProbaChart������LineChartCtrl���𱳾�
        m_TabLineChartCtrlWinColorRef = RGB(229,181,169);       //TabProbaChart������LineChartCtrlӯ������
        m_TabLineChartCtrlAxisTextColorRef = RGB(0,0,0);        //TabProbaChart������LineChartCtrl����̶�����
    }
    else if(Color_Thick == mode)
    {
        //������
        m_OptionStrategyTitleColorRef = RGB(9,58,117);          //����
        m_OptionStrategyClientColorRef = RGB(226,226,226);      //�ͻ���

        //T�ͱ��۴���
        m_TPriceTitleColorRef = RGB(1,0,128);           //����
        m_TPriceClientColorRef = RGB(5,34,69);          //�ͻ���

        //�����µ�����
        m_QuickOrderColorRef = RGB(226,226,226);                    //�ͻ���
        m_QuickOrderThumbnailColorRef = RGB(239,239,239);           //��������ͼ����
        m_QuickOrderListHeaderCtrlColorRef = RGB(38,73,103);        //�б�ͷ
        m_QuickOrderListCtrlColorRef = RGB(9,58,117);               //�б���
        m_QuickOrderListItemSelectedColorRef = RGB(120,0,1);        //�б�ѡ�б���

        //��������
        m_AnalyseModuleColorRef = RGB(226,226,226);     //��������

        m_CustomTabPageSheetNormalColorRef = RGB(31,119,217);    //tabCtrl����pageSheet��������
        m_CustomTabPageSheetSelectedColorRef = RGB(13,49,89);    //tabCtrl����pageSheet��ѡ����

        m_TabListBoxCtrlIconColorRef = RGB(120,0,1);                    //TabTradeAnalyse������ListBoxCtrl itemǰ��ͼ�걳��
        m_TabListBoxCtrlIconSelectedColorRef = RGB(0,132,0);            //TabTradeAnalyse������ListBoxCtrl item��ѡ��ǰ��ͼ�걳��
        m_TabListBoxCtrlBKColorRef = RGB(9,58,117);                     //TabTradeAnalyse������ListBoxCtrl����
        m_TabListBoxCtrlItemTextBKColorRef = RGB(9,58,117);             //TabTradeAnalyse������ListBoxCtrl item text����
        m_TabListBoxCtrlItemSelectedColorRef = RGB(120,0,1);            //TabTradeAnalyse������ListBoxCtrl item ��ѡ�б���
        m_TabListBoxCtrlItemTextColorRef = RGB(255,255,255);                  //TabTradeAnalyse������ListBoxCtrl item text��ɫ
        m_TabListBoxCtrlItemTextSelectedBKColorRef= RGB(255,255,255);         //TabTradeAnalyse������ListBoxCtrl item text��ѡ�б�ɫ
        

        m_TabLineChartCtrlBottomColorRef = RGB(9,58,117);           //TabProbaCtrl������LineChartCtrl�ײ㱳��
        m_TabLineChartCtrlTopColorRef = RGB(0,0,0);                 //TabProbaCtrl������LineChartCtrl���㱳��
        m_TabLineChartCtrlBorderColorRef = RGB(154,156,158);        //TabProbaCtrl������LineChartCtrl�߿򱳾�
        m_TabLineChartCtrlLineColorRef = RGB(160,160,160);          //TabProbaCtrl������LineChartCtrl��������
        m_TabLineChartCtrlLostColorRef = RGB(0,132,0);              //TabProbaCtrl������LineChartCtrl���𱳾�
        m_TabLineChartCtrlWinColorRef = RGB(255,4,0);               //TabProbaCtrl������LineChartCtrlӯ������
        m_TabLineChartCtrlAxisTextColorRef = RGB(255,255,255);      //TabProbaCtrl������LineChartCtrl����̶�����
    }
}
