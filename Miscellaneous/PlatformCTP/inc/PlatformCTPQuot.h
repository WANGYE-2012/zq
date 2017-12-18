/***************************************************************************//**
*   @file          PlatformCTPQuot.h
*   @brief        �ۺϽ���ƽ̨(CTP)���׽ӿ�
*
********************************************************************************/
#ifndef _PLATFORMCTPQUOT_H_
#define _PLATFORMCTPQUOT_H_

#include <string>
#include <set>
#include <map>
#include <vector>
using std::vector;
using std::string;
using std::map;
using std::multimap;
using std::set;




#include "PlatformCTP.h"


#pragma managed(push,off)



//����ӿ�
class CPlatformCTPQuot : public CThostFtdcMdSpi, public CPlatformCTP
{
public:
    //******************************************************************************************
    //ConnParam:                ���Ӳ���
    //bPushCache:               �Ƿ���Ҫ��ctp���ص����ݷŵ�m_Cache�У��ȴ������̴߳���(������FastTrader����ʽ)
    //bUserComb:                �Ƿ�ʹ����ϵ�����
    //bCalcuCombQuotOnLegQuot:  �����ϵ����Ƿ���ݵ��Ⱥ�Լ�����Զ�������Ϻ�Լ����
    //******************************************************************************************
    CPlatformCTPQuot(const Stru_CTPConnectionParam& ConnParam=Stru_CTPConnectionParam(),
        bool bPushCache=true,
        bool bUseComb=false,
        bool bCalcuCombQuotOnLegQuot=false);

    virtual ~CPlatformCTPQuot();

    //******************************************************************************************
    //  pSpi:                   spi�ص���������ָ�롣һ��ΪNULL��ʹ�ñ��������Ĭ�ϴ����ɣ�
    //                          ���ʵ��������CPlatformCTPTrade�����࣬pMdSpiӦ�õ�����������ָ�룬
    //                          ʹ���������Խ��лص�����
    //******************************************************************************************
    virtual bool Login(const CThostFtdcMdSpi* pSpi=NULL);
    virtual void Logout(void);

    ///��������
    int SubscribeMarketData(const string& InstrumentID);
    ///�˶�����
    int UnSubscribeMarketData(const string& InstrumentID);
    ///�˶�ȫ������
    int UnSubscribeAllMarketData(void);
    ///��ȡ���鶩���б�
    vector<string> GetSubscribedInstrumentIDs(void) const;

	void SetInstrumentInfo(const map<string,CThostFtdcInstrumentField>& InstrumentID2Info);


///****************************************************************************
/// ������ctp�ص�����
///****************************************************************************

protected:
    ///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();
	
	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);
		
	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);
	

	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);


private:
    //����ָ����Լ���飬���õ�������£��򷵻�true�����򷵻�false����ԭ����������ϣ�
    //������OnRtnDepthMarketData()��
    bool SetQuotInfo_Internal(const CThostFtdcDepthMarketDataField& inData);
    //��ȡָ����Լ���顣������OnRtnDepthMarketData()��
    bool GetQuotInfo_Internal(const std::string& InstrumentID, CThostFtdcDepthMarketDataField& outData);
	///��ȡ�����˵��Ⱥ�Լ����Ϻ�Լ�б�
	bool GetCombInstrumentIDs_IncludeLeg_Subscribed_Internal(const string& LegInstrument,vector<string>& vecCombInstruments);


    //�ж�һ����Լ�Ƿ�����ϵ���Լ������ǣ����ص��Ⱥ�Լ���ơ�
    //Ŀǰ�����жϴ��̺�֣�̵���Ϻ�Լ
    //�ж�������ǰ�пո����&����SP c1305&c1309
    static bool IsCombInstrument(const string& strInstrument,string& LegInstrument1,string& LegInstrument2);

    //�ж�һ����Լ�Ƿ�����ϵ���Լ��
    //Ŀǰ�����жϴ��̺�֣�̵���Ϻ�Լ
    //�ж�������ǰ�пո����&����SP c1305&c1309
    static bool IsCombInstrument2(const string& strInstrument);


///****************************************************************************
/// �����ǳ�Ա����
///****************************************************************************

private:

    map<string,int>         m_SubscribedInstrumentID;       //��ǰ��������ĺ�Լ -> ���Ĵ���
    multimap<string,string>	m_LegInstrument2CombSubscribed; //���Ⱥ�Լ����Ϻ�Լ�Ķ�Ӧ��
	CThostFtdcMdApi*        m_pQuotApi;						//����ӿڶ������ۺϽ���ƽ̨�Ľӿ��ṩ
    map<string,CThostFtdcDepthMarketDataField> m_lastQuot;	//���ﱣ��ÿ����Լ����������

	//��Լ��Ϣ�����ⲿ�ӽ��׽ӿ�ȡ�������ù���
	map<string,CThostFtdcInstrumentField> m_InstrumentID2Info;


private:
    CPlatformCTPQuot(const CPlatformCTPQuot&);
	CPlatformCTPQuot& operator= (const CPlatformCTPQuot&);
};


#pragma managed(pop)


#endif

