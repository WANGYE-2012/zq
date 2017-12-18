//*******************************************************************************************************
//			����������ʱ�����
//			(1) ����ÿ���������Ľ���ʱ�β�����ͨ����Ӧ�ò��xml�ж���
//			(2) �����ÿ������������ʱ�εķ�������
//				�������ڽ���ʱ��Ϊ21:00-2:30 + 9:00-10:15 + 10:30-11:30 + 13:30-15:00
//				��Ӧ�����1260-1440 + 0-150 + 540-615 + 630-690 + 810-900
//			(3) �ṩ����������һ������ʱ��ֵ�����ڵĽ��׷�����
//				��9:01�����ڽ��׷�����Ϊ331��ͨ����ֵ�����Լ����9:01��K�������������е�λ��
//			(4) ��ͨ�����ڻ���������֤ȯ������
//*******************************************************************************************************

#pragma once
#include <vector>
#include <string>
using std::vector;
using std::string;
using std::make_pair;
using std::pair;

class Ceasytradingtime2
{
public:
	Ceasytradingtime2(const vector<pair<int,int>>& tradingtime);
	Ceasytradingtime2(const Ceasytradingtime2& r);
	virtual ~Ceasytradingtime2(void);
	Ceasytradingtime2& operator= (const Ceasytradingtime2& r);

public:

	//ȡutcʱ������ڵĽ��׷��������罻��ʱ��Ϊ21:00-24:00;  0:00-2:30����0:01�����ڽ��׷�����Ϊ181
	bool GetTradingMinutesIntraday(time_t utc,int& TradingMinutes) const;

	//�жϵ�ǰ�Ƿ��ڽ���ʱ����
	bool IsNowTradingTime(void);

private:
	void calculate_tradingsecs(void);


public:
	//utcʱ������ֵ��utc+m_tzseconds=1970.1.1����������*24*3600+hour*3600+min*60+sec
	int m_tzseconds;

	//1970.1.1������ֵ��������Ϊ0���˺�����һ�������ֵ=(utcday+m_basetmweek)%7������һ��������=(utcday+m_basetmweek)/7
	int m_basetmweek;

	//���ڽ���ʱ��, ������������ʾ,ÿ��ʱ�β��ܿ���,��ͬʱ�ο��Կ���
	// ��21:00-24:00; 0:00-2:30; 9:00-10:15; 10:30-11:30; 13:30-15:00, ��ʾΪ��(75600,86400),(0,9000),(32400,36900),(37800,41400),(48600,54000)
	const vector<pair<int,int>> m_basetradingtime;

	//���ڽ���������/�ܷ�����
	int m_tradingsecs_intraday;
	int m_tradingmins_intraday;

	//���տ��̺����̵�utcʱ��
	time_t m_OpenTime_Today;
	time_t m_CloseTime_Today;
};
