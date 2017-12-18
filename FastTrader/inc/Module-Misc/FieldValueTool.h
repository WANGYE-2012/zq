/***************************************************************************//**
*   @file         FieldValueTool.h
*   @brief        ������ҵ����Ϣ���ֶ�תΪ����GUI��ʾ���ַ���
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/10/11
********************************************************************************/
#ifndef _FIELDVALUETOOL_H_
#define _FIELDVALUETOOL_H_

//#include "../inc/ISvr/PlatformSvrMgr.h"
#include "Module-Misc2/globalDefines.h"
#include <string>
using std::string;

class CFieldValueTool
{
public:
	///��������
	static const wxString Direction2String(char param,int language=LANGUAGE_CHINESE);	
	static const char String2Direction(wxString Dir,int* language=NULL);	
	///�ֲֶ�շ���
	static const wxString PosiDirection2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2PosiDirection(wxString Type,int* language=NULL);
	///Ͷ���ױ���־
	static const wxString HedgeFlag2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2HedgeFlag(wxString Dir,int* language=NULL);
	///��ƽ��־
	static const wxString OffsetFlag2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2OffsetFlag(wxString flag,int* language=NULL);
	///����״̬
	static const wxString OrderStatus2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2OrderStatus(wxString Type,int* language=NULL);
	///�����ύ״̬
	static const wxString OrderSubmitStatus2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2OrderSubmitStatus(wxString Type,int* language=NULL);
	///�����۸�����
	static const wxString OrderPriceType2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2OrderPriceType(wxString Type,int* language=NULL);
	///�û�ǿƽ��־
	static const wxString UserForceClose2String(int UserForceClose,int language=LANGUAGE_CHINESE);
	static const int String2UserForceClose(wxString Type,int* language=NULL);
	///ǿƽԭ��
	static const wxString ForceCloseReason2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2ForceCloseReason(wxString Type,int* language=NULL);
	///��������
	static const wxString OrderType2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2OrderType(wxString Type,int* language=NULL);
	///��Ч������
	static const wxString TimeCondition2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2TimeCondition(wxString Type,int* language=NULL);
	///�ɽ�������
	static const wxString VolumeCondition2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2VolumeCondition(wxString Type,int* language=NULL);
	///ҵ�����������ؽ��
	static const wxString ReturnCode2String(int param,int language=LANGUAGE_CHINESE);
	static const int String2ReturnCode(wxString Type,int* language=NULL);
    ///Ʒ������
    static const wxString ProductType2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2ProductType(wxString Type,int* language=NULL);
	///�ɽ�����
	static const wxString TradeType2String(char TradeType,int language=LANGUAGE_CHINESE);
	static const wxString TradeType2CombState(char TradeType,int language);
	static const char String2TradeType(wxString Type,int* language=NULL);
	//
	static const wxString TodayPosition2String(int param,int language=LANGUAGE_CHINESE);
	static const int String2TodayPosition(wxString Type,int* language=NULL);
	//
	static const wxString OrderStatusMsg2String(int param,int language=LANGUAGE_CHINESE);
	static const int String2OrderStatusMsg(wxString Type,int* language=NULL);

	static const wxString ConvertStatusMsg(wxString strStatusMsg);

	static const wxString ExchangeID2Name(char* ExchangeID,int language=LANGUAGE_CHINESE);

	///ִ������
	static const wxString ActionType2String(char ActionType,int language=LANGUAGE_CHINESE);
	static const char String2ActionType(wxString Type,int* language=NULL);
	///��Ȩ��Ȩ���Ƿ����ڻ�ͷ��ı������
	static const wxString ReservePositionFlag2String(char ReservePositionFlag,int language=LANGUAGE_CHINESE);
	static const char String2ReservePositionFlag(wxString Type,int* language=NULL);
	///��Ȩ��Ȩ�����ɵ�ͷ���Ƿ��Զ�ƽ������
	static const wxString CloseFlag2String(char CloseFlag,int language=LANGUAGE_CHINESE);
	static const char String2CloseFlag(wxString Type,int* language=NULL);
	///ִ�н������
	static const wxString ExecResult2String(char ExecResult,int language=LANGUAGE_CHINESE);
	static const char String2ExecResult(wxString Type,int* language=NULL);

	//ȡС�����λ��
	static int GetNumDigits(double dbTick);
	static const string Price2String(double dbData, double dbTick);
	static const string Int2String(int nData);

private:
	CFieldValueTool();
};

#endif
