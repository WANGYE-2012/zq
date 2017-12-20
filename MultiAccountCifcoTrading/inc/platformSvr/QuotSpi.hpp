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
#define DELAY_QUOT
#include "ctp/ThostFtdcMdApi.h"
#include "ISvr/PlatformStruct.h"
#include <set>
class CPackageCache;
class CPlatFormParam;
class CPlatFormService;
class zqWriteLog;

class CQuotSpi
    :public CThostFtdcMdSpi
{
public:
    CQuotSpi(CPlatFormService& RefCPlatFormService);
    ~CQuotSpi();
private:
    CQuotSpi(const CQuotSpi&);
    CQuotSpi& operator= (const CQuotSpi&);
    
    CThostFtdcMdApi&     m_RefMdApi;
    CPackageCache&       m_RefPackageCache;
    CPlatFormParam&      m_PlatformParam;
    CPlatFormService&    m_RefCPlatFormService;
    zqWriteLog*           m_pWriteLog;

private:
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
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
#ifdef DELAY_QUOT	
	void OnTimer();
	CRITICAL_SECTION m_CS;
	std::map<std::string,CThostFtdcDepthMarketDataField> m_mapQuot;
	static std::set<CQuotSpi*> m_pQuotSpiSet;
	static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
#endif
private:

	void CopyDepthMarketData(PlatformStru_DepthMarketData& data, const CThostFtdcDepthMarketDataField& depthMarketData);
};


#endif

