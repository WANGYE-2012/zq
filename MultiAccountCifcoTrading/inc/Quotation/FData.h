#pragma once
//���ĳ��ID���������ݶ��������������
#include <string>
#include "../inc/Quotation/CritSec.h"
#include "../inc/Quotation/AutoLock.h"
#include "..\\inc\\ISvr\\BusinessData.hpp"
#include "../inc/Quotation/TechIndex.h"
#define  KONGKAI    "�տ�"
#define  DUOKAI     "�࿪"
#define  DUOPING	"��ƽ"
#define  KONGPING   "��ƽ"
#define  DUOHUAN    "�໻"
#define  KONGHUAN   "�ջ�"
#define  SHUANGPING "˫ƽ"
#define  SHUANGKAI  "˫��"
class CFData
{
public:
	CFData();
	~CFData();
public:
	string  m_ExchangeName; //����������
	string  m_ContractName; //��Լ����
	string  m_ContractDate; //��Լ����
	
	string	m_strTradingDay;//������
	int		m_VolumeMultiple;//��Լ����
	double	m_PreSettlementPrice;//������
	double	m_UpperLimitPrice;///��ͣ���
	double	m_LowerLimitPrice;	///��ͣ���
	double  m_PriceTick;//��С�䶯��λ
	int     m_Simulate;//�Ƿ�ģ�⣬1-ģ�⣬0-��ʵ
	double			 m_fMaxValue;//������߼�
	double			 m_fMinValue;//������ͼ�
//	double			  m_dbLastdbPrice;      //ǰ�����
	vector<STLine *>  m_vecTLine;
	vector<struct tm> m_vecTimeScales;				//С��������
	
protected:	
	vector<DWORD>     m_vecTMWidthSpan;			//��������ʱ��������ܳ�
	DWORD			  m_TMWidthCount;			//����ʱ��̶��ܹ���С
	int				  m_nOneDayScalesCount;		//һ�콻�׵�ʱ���ܳ�����λ���룩	
	CCritSec		  m_CritSecVector_TLine;//����ͬ����	
	struct tm		  m_tmLastTrans_TLine;  //���һ�ε������ʱ��    
	int				  m_nNumDigits;         //������Ч����λ������С�䶯λ��Ӧ
	STLine			  m_sTLineLast;			//���һ���ֵ�ʵʱ����
	bool			  m_bInitKLine;
	bool			  m_bInitTLine;
	
	
public:
	void SetPriceTick(double  PriceTick);//������С�䶯λ
	int  GetDotDigits();//�õ�������Чλ��
	bool SetFuture();
	void SetDataInTime(PlatformStru_DepthMarketData* pData, int nTrace);//����ʵʱ���飬ʵʱ���鶼���������������д���
	bool SaveFile_TLine();
	bool ReadFile_TLine();
	void SetLastTransTime_TLine(struct tm tmTime);
	void SetScales(vector<struct tm>& vecScales);
	void SetTodayScales(vector<struct tm>& vecTimeScales);
	void ClearMemory_TLine();
	void SetLastClosePrice(double dbPrice);
	BOOL SetInTimeData_TLine(STLine* pTLine);
	BOOL SetInTimeData_DiffSpan_TLine(STLine* pTLine);
	void CalcMaxMin();//���������ʷ���ݵ���� ��Сֵ
protected:
	void			  Init();
	struct tm		  GetTimeInSpan_TLine(struct tm tmPara, int nSpan);	
	bool			  DirExist(const char *pszDirName) ;
	bool			  CreateDir(const char *pszDirName);//����Ŀ¼��������Ŀ¼�����Դ����༶��Ŀ¼�� �Ժ�洢�ļ�����·�������Ż�Ϊ����������
	bool			  GetFileSavePath_TLine(wxString &filepath);
	bool			  SaveOneFile_TLine(wxString filepath, vector<STLine *>& vecTLine);

//����Ϊtline��أ�����Ϊkline���
public:
	bool SetPhrase(EnumPhrase enumPhrase);	
	bool ReadFile();
	bool SaveFile();
	void SetLastTransTime(struct tm tmTime);
	void ClearMemory();
	void ClearMemory_Mark();
	void *m_pWnd;
	void SetWnd(void*  pWnd){ m_pWnd = pWnd; };
	vector<SKLine *>* GetVectorCommon(EnumPhrase enumPhrase);
	bool				AddMarks(KTYPE enumType, struct tm tmTime, double dbPrice);
	MAPsmark			m_mapMarks;
	EnumMark			m_enumMark;

	SPoint				*m_pDelMark;//k�ߣ�����ģ����Ҫɾ����ĳ����ע

protected:	
	bool GetFileSavePath(wxString &filepath);
	bool GetSaveFileName(string  ExchangeName, string  ContractName, string  ContractDate, EnumPhrase enumPhrase, wxString &filepath);
	bool SaveOneFile(wxString filepath, vector<SKLine *>& vecKLine);

	BOOL SetInTimeData_DiffSpan(SKLine* pKLine);
	BOOL SetInTimeData_Day(vector<SKLine *>& vecKLinePara, SKLine* pKLine);
	BOOL SetInTimeData_Week(vector<SKLine *>& vecKLinePara, SKLine* pKLine);
	BOOL SetInTimeData_Month(vector<SKLine *>& vecKLinePara, SKLine* pKLine);
	BOOL SetInTimeData_MINS(vector<SKLine *>& vecKLinePara, SKLine* pKLine, int nTimeSpan);

	struct tm GetTimeInSpan(struct tm tmPara, int nSpan);
	EnumPhrase		 m_enumPhrase;   //ʱ������;�Ժ��������Ҫȥ����������ʱ�洢marksʱ��Ҫ�õ����Ժ�marksҲҪ�����ڴ洢
	//////////////////////////////////////////////////////////////////////////
	/*
	1�����ṩ���15������������
	5��-60���Ӷ����ṩ30�������յ�����
	*/
	//////////////////////////////////////////////////////////////////////////
	vector<SKLine *>* m_vecKLineCommon;
	

	vector<SKLine *> m_vecKLineDay; 
	bool  m_bSelDay;

	vector<SKLine *> m_vecKLineWeek; 
	bool  m_bSelWeek;

	vector<SKLine *> m_vecKLineMonth; 
	bool  m_bSelMonth;

	vector<SKLine *> m_vecKLineMin1;
	bool  m_bSelMin1;

	vector<SKLine *> m_vecKLineMin5; 
	bool  m_bSelMin5;

	vector<SKLine *> m_vecKLineMin15; 
	bool  m_bSelMin15;

	vector<SKLine *> m_vecKLineMin30; 
	bool  m_bSelMin30;

	vector<SKLine *> m_vecKLineMin60;  
	bool  m_bSelMin60;
	
	SKLine				m_sKLineLast;
	CCritSec			m_CritSecVector;
	void				SetSelMinsValue(EnumPhrase enumPhrase);
	bool				GetSelMinsValue(EnumPhrase enumPhrase);

	bool				IsOneWeek(struct tm tm1, struct tm tm2);	
	bool				IsOneMonth(struct tm tm1, struct tm tm2);

	bool				ReadFileMarks();
	bool				SaveFileMarks();
	bool				SaveOneFile(wxString filepath, MAPsmark&	mapMarks);


	int					GetPhraseSpan(EnumPhrase enumPhrase);
	struct tm			m_tmLastTrans;
	void				SetLastTransTime_DiffSpan(struct tm tmTime, vector<SKLine *>& m_vecKLineCommon, EnumPhrase enmuPhrase);
	BOOL				SetInTimeData(SKLine* pKLine);

public:	//ʵʱ���ݣ��Ҳ����
	vector<SQIntime *>				m_vecQIntime;//ʵʱ

	bool				SetFuture_QIntime();
	bool				SetInTimeData_QIntime(PlatformStru_DepthMarketData& MarketDataInfo);
	bool				SaveFile_QIntime();	
	void				ClearMemory_QIntime();
	void                GetNeiWaiPan(int& nNeiPan, int& nWaiPan);
	
protected:
	CCritSec						m_CritSecVector_QIntime;
	bool							m_bInitQIntime;
	bool							m_bInitQIntimeFirst;//��һ������
	PlatformStru_DepthMarketData	m_MarketDataInfoLast;
	int								m_nNeiPan;//����
	int								m_nWaiPan;//����	
	bool				IsSameAsPrice(double dbPrice, double dbOrgPrice, bool bBid);//bBidΪtrueʱ�����ж���һ����ͬ������ֵtrueΪͬ����һ�ۣ����ߵ��������
	bool				ReadFile_QIntime();
public://����ָ�����	
	bool CalcKTechIndex(vector<SKLine *>& m_vecKLine, string strTechIndexNameOrg, string strTechIndexName, EnumPhrase enumPhrase, CTechIndex*& pTechIndex, vector<int>* vecConfigDefault = NULL);
	void GetTechIndexConfig(string strTechIndex, EnumPhrase enumPhrase, vector<int>& vecConfig);
	void SetTechIndexConfig(string strTechIndex, EnumPhrase enumPhrase, vector<int>& vecConfig);

private:
	void	ClearMemory_TechIndex();
	CCritSec					m_CritSecVector_TechIndex;
	map<string, CTechIndex*>	m_mapName2TechIndex; //	
	map<string, vector<int>>	m_mapName2Config;   
	vector<string>				m_vecChangeName;
	bool GetConfigFrmFile(string strTechIndexName, EnumPhrase enumPhrase, vector<int>& vecCofig);
	void split(const string& s, char c, vector<string>& v);

	void GetDefaultConfigPara(string strTechIndexName, vector<int>& vecCofig);
	void Calc(string strTechIndexName, vector<SKLine *>& m_vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, CTechIndex*& pTechIndex);
};
