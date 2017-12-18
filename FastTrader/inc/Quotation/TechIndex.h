#pragma once
//���ĳ��ID���������ݶ��������������
#include <string>
#include "../inc/Quotation/CritSec.h"
#include "../inc/Quotation/AutoLock.h"
struct STechIndexValue 
{ 
	struct tm tmTime;
	string strName;
	double fValue; 
};	
typedef vector<STechIndexValue>  VecTechIndexValue; 
class CTechIndex
{
public:
	CTechIndex(void);
	~CTechIndex(void);	
	
	wxString				m_strTechName;		//BOLL
	EnumPhrase				m_enumPhrase;
	VecTechIndexValue*		m_vecData;         //MID UPPER LOWER ��������
	int						m_nVecDataCount;	
	
	int						m_nDrawBody;//������� macd ֻҪ���� diff��dea������Ҫ����macd���������ֵ��2


	VecTechIndexValue*		m_vecMiddle;        //���ּ���ָ����м�ֵ������ͼû��ʵ������
	int						m_nVecMiddleCount;

	vector<int>				m_vecConfig;		// BOLL(26,2);�е�26,2������
	vector<string>			m_vecConfigText;	// MID UPPER LOWER	


public:
	void ClearMemory();
	void Calc_MA(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_VOL(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_KDJ(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_MACD(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_BOLL(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_UOS(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_BIAS(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_DMI(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_PSY(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_ROC(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_BBI(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_EXPMA(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_DMA(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_SAR(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_TRIX(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_MTM(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_CRTECH(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_VR(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_OBV(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_ASI(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_EMV(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_CCI(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_RSI(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_CDP(vector<SKLine*>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText);
};

