/***************************************************************************//**
*   @file         QuotSpi.hpp
*   @brief        �ۺϽ���ƽ̨(CTP)������صĻص�����
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/09/14
********************************************************************************/
#ifndef _QUOTSPI_HPP_
#define _QUOTSPI_HPP_

//#include "ctp/ThostFtdcMdApi.h"
#include "InstitudeMDApi.h"
#include "Module-Misc2/KernelStruct.h"
#include <string>
#include <map>

class CInstitudeMdApi;
class CPackageCache;
class CPlatFormParam;
class CPlatFormService;
class zqWriteLog;

//class CQuotSpi
//    :public CThostFtdcMdSpi
class CQuotSpi
{
public:
    CQuotSpi(CPlatFormService& RefCPlatFormService);
    ~CQuotSpi();

    void SetRtnDepthMarketData(PlatformStru_DepthMarketData *pDepthMarketData){ OnRtnDepthMarketData(pDepthMarketData); };

private:
    CQuotSpi(const CQuotSpi&);
    CQuotSpi& operator= (const CQuotSpi&);
    
    //CThostFtdcMdApi&     m_RefMdApi;
    CInstitudeMdApi&     m_RefMdApi;
    CPackageCache&       m_RefPackageCache;
    CPlatFormParam&      m_PlatformParam;
    CPlatFormService&    m_RefCPlatFormService;
    zqWriteLog*           m_pWriteLog;



public:
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

	///�ǳ�������Ӧ
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(PlatformStru_DepthMarketData *pDepthMarketData);

private:
	void CopyDepthMarketData(PlatformStru_DepthMarketData& data, const CThostFtdcDepthMarketDataField& depthMarketData);

    //���ﱣ��ÿ����Լ���������飬������ϵ������鴦��
    std::map<std::string,PlatformStru_DepthMarketData> m_lastQuot;    
    //����ָ����Լ���飬���õ�������£��򷵻�true�����򷵻�false����ԭ����������ϣ�
    //������OnRtnDepthMarketData()��
    bool SetQuotInfo_Internal(const PlatformStru_DepthMarketData& inData);
    //��ȡָ����Լ���顣������OnRtnDepthMarketData()��
    bool GetQuotInfo_Internal(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);

};


#endif

