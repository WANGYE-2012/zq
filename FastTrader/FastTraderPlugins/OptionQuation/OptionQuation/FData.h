#pragma once
#include "stdafx.h"
#include "QuoteCommon.h"

class  CFData
{
public:
	CFData(void);
	~CFData(void);

public:
	//��¼ԭʼʵʱ���ݣ���Ҫ���ڸ���ʷ�����������в���ʱ����
	bool		    m_bRecordIntime;
	//��������ԭʼ����
	std::vector<PlatformStru_DepthMarketData>  m_vecDepthMarketData;	
	CRITICAL_SECTION	m_csDepthMarketData;


public:
	string		     m_ExchangeName;			//����������
	string		     m_strInstrument;			//��Լ����

	string		     m_strTradingDay;			//������
	int			     m_VolumeMultiple;			//��Լ����
	double		     m_PreSettlementPrice;		//������
	double			 m_UpperLimitPrice;			//��ͣ���
	double			 m_LowerLimitPrice;			//��ͣ���
	double			 m_PriceTick;				//��С�䶯��λ
//	int				 m_Simulate;				//�Ƿ�ģ�⣬1-ģ�⣬0-��ʵ
	
	double			 m_fMaxValue;				//������߼�
	double			 m_fMinValue;				//������ͼ�

	vector<struct tm> m_vecTimeScales;			//С��������
	CRITICAL_SECTION  m_csTimeScales;

private:
	vector<STLine>   m_vecTLine;
	CRITICAL_SECTION	m_csTLine;
	bool m_bTLineShow;

	EnumPhrase		 m_enumPhrase;   //ʱ������;�Ժ��������Ҫȥ����������ʱ�洢marksʱ��Ҫ�õ����Ժ�marksҲҪ�����ڴ洢
	//////////////////////////////////////////////////////////////////////////
	/*
	1�����ṩ���15������������
	5��-60���Ӷ����ṩ30�������յ�����
	*/
	//////////////////////////////////////////////////////////////////////////

	vector<SKLine>		m_vecKLineDay; 
	CRITICAL_SECTION	m_csKLineDay;
	bool m_bDayShow;

	vector<SKLine>		m_vecKLineWeek; 
	CRITICAL_SECTION	m_csKLineWeek;
	bool m_bWeekShow;

	vector<SKLine>		m_vecKLineMonth; 
	CRITICAL_SECTION	m_csKLineMonth;
	bool m_bMonthShow;

	vector<SKLine>		m_vecKLineMin1;
	CRITICAL_SECTION	m_csKLineMin1;
	bool m_bMin1Show;
//С��500���ķ�������Ϊ���ؽ���

	vector<SKLine>		m_vecKLineMin5; 
	CRITICAL_SECTION	m_csKLineMin5;
	bool m_bMin5Show;

	vector<SKLine>		m_vecKLineMin15; 
	CRITICAL_SECTION	m_csKLineMin15;
	bool m_bMin15Show;

	vector<SKLine>		m_vecKLineMin30; 
	CRITICAL_SECTION	m_csKLineMin30;
	bool m_bMin30Show;

	vector<SKLine>		m_vecKLineMin60;  
	CRITICAL_SECTION	m_csKLineMin60;
	bool m_bMin60Show;


protected:	
	vector<DWORD>     m_vecTMWidthSpan;			//��������ʱ��������ܳ�
	DWORD			  m_TMWidthCount;			//����ʱ��̶��ܹ���С
	int				  m_nOneDayScalesCount;		//һ�콻�׵�ʱ���ܳ�����λ���룩		
	struct tm		  m_tmLastTrans_TLine;		//���һ�ε������ʱ��    
	int				  m_nNumDigits;				//������Ч����λ������С�䶯λ��Ӧ
	STLine			  m_sTLineLast;				//���һ���ֵ�ʵʱ����
	
	SKLine			  m_sKLineLast;



public:
	void SetDataInTime(PlatformStru_DepthMarketData* pData);//����ʵʱ���飬ʵʱ���鶼���������������д���
	BOOL SetInTimeData_TLine(PlatformStru_DepthMarketData* pData);
	BOOL SetInTimeData_DiffSpan_TLine(STLine& pTLine);
	struct tm GetTimeInSpan_TLine(struct tm tmPara, int nSpan);
	struct tm GetTimeInSpan(struct tm tmPara, int nSpan);
	struct tm GetTimeInSpan_History(struct tm tmPara, int nSpan);

	BOOL SetInTimeData_DiffSpan(SKLine& pKLine, EnumPhrase enumPhrase, bool bShow);
	BOOL SetInTimeData_Day(vector<SKLine>& vecKLinePara, SKLine& pKLine);
	BOOL SetInTimeData_Week(vector<SKLine>& vecKLinePara, SKLine& pKLine);
	BOOL SetInTimeData_Month(vector<SKLine>& vecKLinePara, SKLine& pKLine);
	BOOL SetInTimeData_MINS(vector<SKLine>& vecKLinePara, SKLine& pKLine, int nTimeSpan);
	void SetLastClosePrice(double dbPrice);
	BOOL SetInTimeData_KLine(PlatformStru_DepthMarketData* pData, 
														bool bDayShow,
														bool bWeekShow,
														bool bMonthShow,
														bool bMin1Show,
														bool bMin5Show,
														bool bMin15Show,
														bool bMin30Show,
														bool bMin60Show);
	bool IsOneWeek(struct tm tm1, struct tm tm2);
	bool IsOneMonth(struct tm tm1, struct tm tm2);
	
	void TestVector(vector<SKLine> vecKLinePara);
	bool GetVecKLine(EnumPhrase enumPhrase, vector<SKLine>& vec);
	bool GetVecTLine(vector<STLine>& vec);

	bool IsKLineQueryFinish(EnumPhrase enumPhrase);
private:
	bool GetVecKLineDay(vector<SKLine>& vec);
	bool GetVecKLineWeek(vector<SKLine>& vec);
	bool GetVecKLineMonth(vector<SKLine>& vec);
	bool GetVecKLineMin1(vector<SKLine>& vec);
	bool GetVecKLineMin5(vector<SKLine>& vec);
	bool GetVecKLineMin15(vector<SKLine>& vec);
	bool GetVecKLineMin30(vector<SKLine>& vec);
	bool GetVecKLineMin60(vector<SKLine>& vec);

public:
	void SetScales(vector<struct tm>& vecScales);

	bool SaveFile_TLine();
	bool ReadFile_TLine();
	bool ReadFile(EnumPhrase enumPhrase);
	bool SaveFile();
	bool GetFileSavePath(CString &filepath);
	bool SaveOneFile(CString filepath, vector<SKLine>& vecKLine);
	bool GetSelMinsValue(EnumPhrase enumPhrase);
	bool GetFileSavePath_TLine(CString &filepath);
	bool GetSaveFileName(string  ContractDate, EnumPhrase enumPhrase, CString &filepath);
	bool SaveOneFile_TLine(CString filepath, vector<STLine>& vecTLine);
	
	bool AppendHistoryTLineData(std::vector<TimeLineData>& vecTimeLineData);
	bool AppendRealTimeTLineData();
	bool AppendHistoryKLineData_Min1(std::vector<KLineData>& vecData);
	bool AppendRealTimeKLineData_Min1();
	bool AppendHistoryKLineData_Min5(std::vector<KLineData>& vecData);
	bool AppendRealTimeKLineData_Min5();
	bool AppendHistoryKLineData_Min15(std::vector<KLineData>& vecData);
	bool AppendRealTimeKLineData_Min15();
	bool AppendHistoryKLineData_Min30(std::vector<KLineData>& vecData);
	bool AppendRealTimeKLineData_Min30();
	bool AppendHistoryKLineData_Min60(std::vector<KLineData>& vecData);
	bool AppendRealTimeKLineData_Min60();
	bool AppendHistoryKLineData_Day(std::vector<KLineData>& vecData);
	bool AppendRealTimeKLineData_Day();
	bool AppendHistoryKLineData_Week(std::vector<KLineData>& vecData);
	bool AppendRealTimeKLineData_Week();
	bool AppendHistoryKLineData_Month(std::vector<KLineData>& vecData);
	bool AppendRealTimeKLineData_Month();






	public://����ָ�����	
		bool CalcKTechIndex(vector<SKLine>& vecKLine, string strTechIndexNameOrg, string strTechIndexName, STechCalcPara& sTCPara, CTechIndex& pTechIndex, vector<int>* vecConfigDefault = NULL);
		void GetTechIndexConfig(string strTechIndex, EnumPhrase enumPhrase, vector<int>& vecConfig);
		void SetTechIndexConfig(string strTechIndex, EnumPhrase enumPhrase, vector<int>& vecConfig);

private:
	void	ClearMemory_TechIndex();
//	CCritSec					m_CritSecVector_TechIndex;
	map<string, CTechIndex>	m_mapName2TechIndex; 	
	map<string, vector<int>>	m_mapName2Config;   
	vector<string>				m_vecChangeName;
	bool GetConfigFrmFile(string strTechIndexName, EnumPhrase enumPhrase, vector<int>& vecCofig);
	void split(const string& s, char c, vector<string>& v);

	void GetDefaultConfigPara(string strTechIndexName, vector<int>& vecCofig);
	void Calc(string strTechIndexName, vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, CTechIndex& pTechIndex);

};
