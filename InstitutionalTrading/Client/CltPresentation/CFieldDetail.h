
#ifndef _FIELDDETAIL_H_
#define _FIELDDETAIL_H_

namespace CltPresentation {
class CFieldDetail
{
public:
	///��������
	static const char* Direction2String(char param);	
	static const char String2Direction(char* Dir);	
	///�ֲֶ�շ���
	static const char* PosiDirection2String(char param);
	static const char String2PosiDirection(char* Type);
	///Ͷ���ױ���־
	static const char* HedgeFlag2String(char param);
	static const char String2HedgeFlag(char* Dir);
	///��ƽ��־
	static const char* OffsetFlag2String(char param);
	static const char String2OffsetFlag(char* flag);
	///����״̬
	static const char* OrderStatus2String(char param);
	static const char String2OrderStatus(char* Type);
	///�����ύ״̬
	static const char* OrderSubmitStatus2String(char param);
	static const char String2OrderSubmitStatus(char* Type);
	///�����۸�����
	static const char* OrderPriceType2String(char param);
	static const char String2OrderPriceType(char* Type);
	///�û�ǿƽ��־
	static const char* UserForceClose2String(int UserForceClose);
	static const int String2UserForceClose(char* Type);
	///ǿƽԭ��
	static const char* ForceCloseReason2String(char param);
	static const char String2ForceCloseReason(char* Type);
	///��������
	static const char* OrderType2String(char param);
	static const char String2OrderType(char* Type);
	///��Ч������
	static const char* TimeCondition2String(char param);
	static const char String2TimeCondition(char* Type);
	///�ɽ�������
	static const char* VolumeCondition2String(char param);
	static const char String2VolumeCondition(char* Type);
	///ҵ�����������ؽ��
	static const char* ReturnCode2String(int param);
	static const int String2ReturnCode(char* Type);
    ///Ʒ������
    static const char* ProductType2String(char param);
	static const char String2ProductType(char* Type);
	///�ɽ�����
	static const char* TradeType2String(char TradeType);
	static const char String2TradeType(char* Type);
	//
	static const char* TodayPosition2String(int param);
	static const int String2TodayPosition(char* Type);
	//
	static const char* OrderStatusMsg2String(int param);
	static const int String2OrderStatusMsg(char* Type);

	static const char* ConvertStatusMsg(char* strStatusMsg);

	static const char* ExchangeID2Name(char* ExchangeID);

	//���׽�ɫ
	static const char* TraderRole2String(int param);


	//�ɽ���Դ
	static const char* TradeSourceString(int param);


	//�ɽ�����Դ
	static const char* PriceType2String(int param);

private:
	CFieldDetail();
	~CFieldDetail();
};
}
#endif
