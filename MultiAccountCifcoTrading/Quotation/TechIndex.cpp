#include "StdAfx.h"
#include "../inc/Quotation/TechIndex.h"

CTechIndex::CTechIndex(void)
{	
	m_strTechName.empty();
	m_vecData		 = NULL;	
	m_nVecDataCount  = 0;	
	m_nDrawBody      = 0;
	m_vecConfig.clear();		
	m_vecConfigText.clear();

	m_vecMiddle = NULL;
	m_nVecMiddleCount = 0;

}
CTechIndex::~CTechIndex(void)
{	
	ClearMemory();
}
void CTechIndex::ClearMemory()
{
	m_vecConfig.clear();		
	m_vecConfigText.clear();
	for(int n = 0; n< m_nVecDataCount; n++)
	{
		if(m_vecData)
			m_vecData[n].clear();		
	}
	if(m_vecData)
	{
		delete []m_vecData;
		m_vecData = NULL;
	}

	for(int n = 0; n< m_nVecMiddleCount; n++)
	{
		if(m_vecMiddle)
			m_vecMiddle[n].clear();		
	}
	if(m_vecMiddle)
	{
		delete []m_vecMiddle;
		m_vecMiddle = NULL;
	}
}
void CTechIndex::Calc_MA(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>& vecConfigText)
{//vecKLine 日期由近到远，今天的在0序号
	if(vecConfig.size() == 0)
		return;
	wxASSERT(vecConfig.size() == vecConfigText.size());

	ClearMemory();	
	m_enumPhrase  = enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;
	m_strTechName = wxT(MA);

	m_vecData = new VecTechIndexValue[vecConfig.size()];
	m_nVecDataCount = vecConfig.size();
	m_nDrawBody = vecConfig.size();
	
	for(int i = 0; i< (int)vecConfig.size(); i++)
	{
		int nDay = vecConfig[i];
		if(nDay == 0)
			continue;

		VecTechIndexValue& pTech = m_vecData[i];
		for(int nItem = 0; nItem < (int)vecKLine.size(); nItem++)
		{
			SKLine *pKline = vecKLine[nItem];
			STechIndexValue sValue;
			sValue.tmTime  = pKline->dwTime;
			sValue.strName = vecConfigText[i];//ma1
			sValue.fValue  = MIN_VALUE_DEFAULT;
			if(nItem + nDay > (int)vecKLine.size())
			{//超出范围，没有形成足够天数，比如只有四天数据，自然没有5日线
				
			}
			else
			{
				double dbValue = 0.0;
				for(int nBegin = nItem; nBegin < nItem + nDay; nBegin++)
				{
					SKLine *pKBegin = vecKLine[nBegin];
					dbValue = dbValue + pKBegin->fClosePrice;	
				}
				sValue.fValue = dbValue/nDay;
			}
			pTech.push_back(sValue);
		}	
			
	}
}
void CTechIndex::Calc_VOL(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>& vecConfigText)
{//vecKLine 日期由近到远，今天的在0序号
	if(vecConfig.size() == 0)
		return;
	wxASSERT(vecConfig.size() == vecConfigText.size());

	ClearMemory();	
	m_enumPhrase  = enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;
	m_strTechName = wxT(VOL);

	m_vecData = new VecTechIndexValue[vecConfig.size()];
	m_nVecDataCount = vecConfig.size();
	m_nDrawBody = vecConfig.size();
	
	for(int i = 0; i < (int)vecConfig.size(); i++)
	{
		int nDay = vecConfig[i];
		if(nDay == 0)
			continue;

		VecTechIndexValue& pTech = m_vecData[i];
		for(int nItem = 0; nItem < (int)vecKLine.size(); nItem++)
		{
			SKLine *pKline = vecKLine[nItem];
			STechIndexValue sValue;
			sValue.tmTime  = pKline->dwTime;
			sValue.strName = vecConfigText[i];//ma1
			sValue.fValue  = MIN_VALUE_DEFAULT;
			if(nItem + nDay > (int)vecKLine.size())
			{//超出范围，没有形成足够天数，比如只有四天数据，自然没有5日线

			}
			else
			{
				double dbValue = 0.0;
				for(int nBegin = nItem; nBegin < nItem + nDay; nBegin++)
				{
					SKLine *pKBegin = vecKLine[nBegin];
					dbValue = dbValue + pKBegin->dwVolume;
				}
				sValue.fValue = dbValue/nDay;
			}
			pTech.push_back(sValue);
		}	

	}
}
void CTechIndex::Calc_KDJ(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 3)
		return;

	ClearMemory();	
	m_enumPhrase  = enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = wxT(KDJ);
	if(vecKLine.size() == 0)
		return;

	m_nVecDataCount = 3;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];//最后一个数组用于保存RSV数据
	m_nDrawBody = 3;
	

	m_vecMiddle = new VecTechIndexValue[1];
	m_nVecMiddleCount = 1;
	int nSize = vecKLine.size();
	for(int i = nSize-2; i>=0; i--)
	{
		SKLine* pStkInfo = vecKLine[i];//第2天

		float fMax =MIN_VALUE_DEFAULT , fMin = MAX_VALUE_DEFAULT;		
		for(int j=0; j< vecConfig[0] && (i+j)<nSize; j++)
		{
			SKLine* pStkInfo = vecKLine[i+j];
			if(fMax < pStkInfo->fHighPrice)
				fMax = pStkInfo->fHighPrice;
			if(fMin > pStkInfo->fLowPrice)
				fMin = pStkInfo->fLowPrice;				
		}

		SKLine* pStkInfo0 = vecKLine[i+1];//SHANG一天

		if(i== nSize -2)
		{
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo0->dwTime;
			sValue.strName = vecConfigText[0];//ma1
			sValue.fValue  = MIN_VALUE_DEFAULT;
			if(IsDoubleSame(fMax, fMin))
				sValue.fValue = 100;
			else
				sValue.fValue = (pStkInfo->fClosePrice - fMin)/(fMax - fMin) *100;
			
			m_vecData[0].push_back(sValue);	
			sValue.strName = vecConfigText[1];
			m_vecData[1].push_back(sValue);	
			sValue.strName = vecConfigText[2];
			m_vecData[2].push_back(sValue);	
			m_vecMiddle[0].push_back(sValue);			
			continue;
		}
		STechIndexValue sValue;
		sValue.tmTime  = pStkInfo0->dwTime;
		sValue.strName = vecConfigText[2];
		sValue.fValue  = MIN_VALUE_DEFAULT;

		if(IsDoubleSame(fMax, fMin))
			sValue.fValue = 100;
		else
			sValue.fValue= (pStkInfo->fClosePrice - fMin)/(fMax - fMin) *100;
		m_vecMiddle[0].insert(m_vecMiddle[0].begin(), sValue);			

		sValue.strName = vecConfigText[0];
		sValue.fValue  = (float)( (vecConfig[1]-1.0)/vecConfig[1] * m_vecData[0][0].fValue+ 1.0/vecConfig[1] * m_vecMiddle[0][0].fValue );
		m_vecData[0].insert(m_vecData[0].begin(), sValue);
		
		sValue.strName = vecConfigText[1];
		sValue.fValue  = (float)( (vecConfig[1]-1.0)/vecConfig[1] * m_vecData[1][0].fValue + 1.0/vecConfig[1] * m_vecData[0][0].fValue    );
		m_vecData[1].insert(m_vecData[1].begin(), sValue);
	
		sValue.strName = vecConfigText[2];
		sValue.fValue  =  vecConfig[2] * m_vecData[0][0].fValue -  (vecConfig[2]-1)*m_vecData[1][0].fValue;
		m_vecData[2].insert(m_vecData[2].begin(), sValue);
	}
	
}
void CTechIndex::Calc_MACD(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText)
{//vecConfigText文字的次序：DIFF,DEA,MACD; vecConfig 12;26;9
	if(vecConfig.size() != 3)
		return;

	ClearMemory();	
	m_enumPhrase  = enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = wxT(MACD);
	if(vecKLine.size() == 0)
		return;

	m_nVecDataCount   = 3;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 2;
	

	m_nVecMiddleCount = 2;
	m_vecMiddle = new VecTechIndexValue[2];
	int nSize = vecKLine.size();
	for(int i= nSize -1; i>=0; i--)
	{
		SKLine* pStkInfo = vecKLine[i];//第2天
		if(i== nSize-1)
		{
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo->dwTime;
			sValue.strName = "EMA12";
			sValue.fValue  = pStkInfo->fClosePrice;
			m_vecMiddle[0].push_back(sValue);
				
			sValue.strName = "EMA26";
			m_vecMiddle[1].push_back(sValue);
			
			sValue.strName = "DIFF";
			sValue.fValue  = 0.0;
			m_vecData[0].push_back(sValue);

			sValue.strName = "DEA";
			sValue.fValue  = 0.0;
			m_vecData[1].push_back(sValue);

			sValue.strName = MACD;
			m_vecData[2].push_back(sValue); 
			continue;
		}
		SKLine* pStkInfo0 = vecKLine[i+1];
/*/////////////////////////////////////////////////////////////////////////
nday12 = vecConfig[0];
/////////////////////////////////////////////////////////////////////////*/
		STechIndexValue sValue;
		sValue.tmTime  = pStkInfo->dwTime;
		sValue.strName = "EMA12";
		sValue.fValue  = (float)( (vecConfig[1] -1.0)/(vecConfig[1] +1)*m_vecMiddle[0][0].fValue + 2.0/(vecConfig[1] + 1)*pStkInfo->fClosePrice);		
		m_vecMiddle[0].insert(m_vecMiddle[0].begin(), sValue);

		sValue.strName = "EMA26";
		sValue.fValue  = (float)((vecConfig[0] -1.0)/(vecConfig[0] +1)*m_vecMiddle[1][0].fValue + 2.0/(vecConfig[0] + 1)*pStkInfo->fClosePrice);
		m_vecMiddle[1].insert(m_vecMiddle[1].begin(), sValue);

		sValue.strName = "DIFF";
		sValue.fValue  = m_vecMiddle[0][0].fValue - m_vecMiddle[1][0].fValue;
		m_vecData[0].insert(m_vecData[0].begin(), sValue);
		
		sValue.strName = "DEA";
		sValue.fValue  = (float)((vecConfig[2] -1.0)/(vecConfig[2] + 1.0)*m_vecData[1][0].fValue + 2.0/(vecConfig[2] + 1.0)*m_vecData[0][0].fValue);;
		m_vecData[1].insert(m_vecData[1].begin(), sValue);

		sValue.strName = MACD;
		sValue.fValue  = 2*(m_vecData[0][0].fValue - m_vecData[1][0].fValue);
		m_vecData[2].insert(m_vecData[2].begin(), sValue);
	}
}
void CTechIndex::Calc_BOLL(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 2)
		return;

	ClearMemory();	
	m_enumPhrase  = enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = wxT(BOLL);
	if(vecKLine.size() == 0)
		return;

	m_nVecDataCount   = 3;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 3;

	int nSize = vecKLine.size();
	float fSum = 0.0;
	for(int i = nSize -1; i>=0; i--)
	{
		SKLine *pStkInfo = vecKLine[i];
		fSum += pStkInfo->fClosePrice;
		if(i + vecConfig[0] <= nSize)
		{
			if(i + vecConfig[0] < nSize)
			{
				int nItem = i+vecConfig[0];
				SKLine *pStkInfoT = vecKLine[nItem];
				fSum -= pStkInfoT->fClosePrice;
			}
			float fMID = fSum/vecConfig[0];

			float fSumDev = 0.0;
			for(int j =0; j<vecConfig[0]; j++)
			{
				SKLine *pStkInfoT1 = vecKLine[i+j];
				fSumDev += (pStkInfoT1->fClosePrice - fMID)*(pStkInfoT1->fClosePrice - fMID);
			}
			
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo->dwTime;
			sValue.strName = "MID";
			sValue.fValue  = fMID;
			m_vecData[0].insert(m_vecData[0].begin(), sValue);

			sValue.strName = "UPPER";
			sValue.fValue  = float(fMID + vecConfig[1] * sqrt(fSumDev/vecConfig[0]));;
			m_vecData[1].insert(m_vecData[1].begin(), sValue);	

			sValue.strName = "LOWER";
			sValue.fValue  = float(fMID - vecConfig[1] * sqrt(fSumDev/vecConfig[0]));;
			m_vecData[2].insert(m_vecData[2].begin(), sValue);
		}
	}

}
void CTechIndex::Calc_UOS(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 4)
		return;

	ClearMemory();	
	m_enumPhrase  = enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = wxT(UOS);
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 2;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 2;

	int nSize = vecKLine.size();
	float fSum71  = 0.0,  fSum72  = 0.0;
	float fSum141 = 0.0,  fSum142 = 0.0;
	float fSum281 = 0.0,  fSum282 = 0.0;

	for(int i = nSize-2; i>=0; i-- )
	{
		SKLine *pStkInfo = vecKLine[i];
		SKLine *pStkInfoBefore = vecKLine[i+1];
		float fTH = max(pStkInfo->fHighPrice , pStkInfoBefore->fClosePrice);
		float fTL = min(pStkInfo->fLowPrice , pStkInfoBefore->fClosePrice);
		fSum71 += pStkInfo->fClosePrice - fTL;
		fSum72 += fTH - fTL;
		if(i + vecConfig[0] < nSize-1)
		{
			SKLine *pStkInfoT = vecKLine[i + vecConfig[0]];
			SKLine *pStkInfoTBefore = vecKLine[i + vecConfig[0]+1];
			float fTH = max(pStkInfoT->fHighPrice, pStkInfoTBefore->fClosePrice);
			float fTL = min(pStkInfoT->fLowPrice, pStkInfoTBefore->fClosePrice);
			fSum71 -= pStkInfoT->fClosePrice - fTL;
			fSum72 -= fTH - fTL;
		}

		fSum141 += pStkInfo->fClosePrice - fTL;
		fSum142 += fTH - fTL;
		if(i + vecConfig[1] < nSize-1)
		{
			SKLine *pStkInfoT14 = vecKLine[i + vecConfig[1]];
			SKLine *pStkInfoT14Before = vecKLine[i + vecConfig[1] + 1];
			float fTH = max(pStkInfoT14->fHighPrice, pStkInfoT14Before->fClosePrice);
			float fTL = min(pStkInfoT14->fLowPrice, pStkInfoT14Before->fClosePrice);
			fSum141 -= pStkInfoT14->fClosePrice - fTL;
			fSum142 -= fTH - fTL;
		}

		fSum281 += pStkInfo->fClosePrice - fTL;
		fSum282 += fTH - fTL;
		if(i + vecConfig[2] <= nSize)
		{
			if(i + vecConfig[2] < nSize-1)
			{
				SKLine *pStkInfoT28 = vecKLine[i + vecConfig[2]];
				SKLine *pStkInfoT28Before = vecKLine[i + vecConfig[2] + 1];
				float fTH = max(pStkInfoT28->fHighPrice, pStkInfoT28Before->fClosePrice);
				float fTL = min(pStkInfoT28->fLowPrice, pStkInfoT28Before->fClosePrice);
				fSum281 -= pStkInfoT28->fClosePrice - fTL;
				fSum282 -= fTH - fTL;
			} 
			//输出终极指标:(ACC1*N2*N3+ACC2*N1*N3+ACC3*N1*N2)*100/(N1*N2+N1*N3+N2*N3)
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo->dwTime;
			sValue.strName = "UOS";
			sValue.fValue  = (fSum71/fSum72*vecConfig[1]*vecConfig[2] + fSum141/fSum142*vecConfig[0]*vecConfig[2] + fSum281/fSum282*vecConfig[0]*vecConfig[1])*100/(vecConfig[1]*vecConfig[2] + vecConfig[0]*vecConfig[2] + vecConfig[0]*vecConfig[1]);
			m_vecData[0].insert(m_vecData[0].begin(), sValue);

			sValue.strName = "MUOS";
			if(i + vecConfig[2] == nSize)
				m_vecData[1].insert(m_vecData[1].begin(), sValue);
			else
			{
				sValue.fValue  = (vecConfig[3] -1.0)/(vecConfig[3] +1)* m_vecData[1][0].fValue + 2.0/(vecConfig[3] + 1)* m_vecData[0][0].fValue;
				m_vecData[1].insert(m_vecData[1].begin(), sValue);
			}		
		}
	}


}
void CTechIndex::Calc_BIAS(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 3)
		return;

	ClearMemory();	
	m_enumPhrase  = enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = wxT(BIAS);
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 3;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 3;

	int nSize = vecKLine.size();
	float f6days =0.0, f12days =0.0, f24days =0.0;
	for(int k = nSize-1; k>=0; k--)
	{
		SKLine* pStkInfo = vecKLine[k];
		f6days  = f6days + pStkInfo->fClosePrice;
		f12days = f12days + pStkInfo->fClosePrice;
		f24days = f24days + pStkInfo->fClosePrice;

		if(k <= nSize-vecConfig[0])
		{
			float fQuChuShouPanJia =0.0;
			if(k +vecConfig[0] <nSize)
			{
				fQuChuShouPanJia = vecKLine[k + vecConfig[0]]->fClosePrice;
				f6days = f6days - fQuChuShouPanJia;
			}
			float fMA = f6days /vecConfig[0];	

			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo->dwTime;
			sValue.strName = "BIAS1";
			sValue.fValue  = (pStkInfo->fClosePrice - fMA)/fMA *100;
			m_vecData[0].insert(m_vecData[0].begin(), sValue);			
		} 

		if(k <= nSize-vecConfig[1])
		{
			float fQuChuShouPanJia =0.0;
			if(k +vecConfig[1] <nSize)
			{
				fQuChuShouPanJia = vecKLine[k + vecConfig[1]]->fClosePrice;
				f12days = f12days - fQuChuShouPanJia;
			}
			float fMA = f12days /vecConfig[1];	

			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo->dwTime;
			sValue.strName = "BIAS2";
			sValue.fValue  = (pStkInfo->fClosePrice - fMA)/fMA *100;
			m_vecData[1].insert(m_vecData[1].begin(), sValue);			
		} 

		if(k <= nSize-vecConfig[2])
		{
			float fQuChuShouPanJia =0.0;
			if(k +vecConfig[2] <nSize)
			{
				fQuChuShouPanJia = vecKLine[k + vecConfig[2]]->fClosePrice;
				f24days = f24days - fQuChuShouPanJia;
			}
			float fMA = f24days /vecConfig[2];	

			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo->dwTime;
			sValue.strName = "BIAS3";
			sValue.fValue  = (pStkInfo->fClosePrice - fMA)/fMA *100;
			m_vecData[2].insert(m_vecData[2].begin(), sValue);				
		} 
	}


}
void CTechIndex::Calc_DMI(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 2)
		return;

	ClearMemory();	
	m_enumPhrase  = enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = wxT(DMI);
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 4;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 4;

	int nSize = vecKLine.size();
	float fDMPSum =0.0, fDMMSum =0.0, fTRSum =0.0;
	float fADXSum = 0.0;
	for(int i = nSize -2; i >= 0; i--)
	{
		SKLine* pStkInfo = vecKLine[i];
		SKLine* pStkInfoBefore = vecKLine[i+1];
		float f1 = pStkInfo->fHighPrice - pStkInfo->fLowPrice;
		float f2 = pStkInfo->fHighPrice - pStkInfoBefore->fClosePrice;
		if(f2 <0)
			f2 = -f2;
		float f3 = pStkInfoBefore->fClosePrice - pStkInfo->fLowPrice;
		if(f3 < 0)
			f3 = -f3;
		float fTR = f1;
		if(f2 > fTR)
			fTR = f2;
		if(f3 > fTR)
			fTR = f3;
		fTRSum  = fTRSum + fTR;
		SKLine* pStkInfo0 = vecKLine[i+1];
		float fHD = pStkInfo->fHighPrice - pStkInfo0->fHighPrice;
		float fLD = pStkInfo0->fLowPrice - pStkInfo->fLowPrice;

		float fDMP = 0;
		if(fHD > 0 &&  fHD > fLD)
			fDMP = fHD;
		float fDMM = 0;
		if(fLD >0 && fLD > fHD)
			fDMM = fLD;
		fDMPSum = fDMPSum + fDMP;
		fDMMSum = fDMMSum + fDMM;

		if(i + vecConfig[0] <= nSize)
		{
			if(i+vecConfig[0] < nSize-1)//第一天不是有效数据，不需要处理
			{
				SKLine* pStkInfoT = vecKLine[i+vecConfig[0]];
				SKLine* pStkInfoTBefore = vecKLine[i+vecConfig[0] +1];
				float f1 = pStkInfoT->fHighPrice - pStkInfoT->fLowPrice;
				float f2 = (float)fabs(pStkInfoT->fHighPrice - pStkInfoTBefore->fClosePrice);
				float f3 = (float)fabs(pStkInfoTBefore->fClosePrice - pStkInfoT->fLowPrice);
				float fTR = f1;
				if(f2 > fTR)
					fTR = f2;
				if(f3 > fTR)
					fTR = f3;
				fTRSum  = fTRSum - fTR;

				SKLine* pStkInfoT0 = vecKLine[i+vecConfig[0]+1];

				float fHD = pStkInfoT->fHighPrice - pStkInfoT0->fHighPrice;
				float fLD = pStkInfoT0->fLowPrice - pStkInfoT->fLowPrice;

				float fDMP = 0;
				if(fHD > 0 &&  fHD > fLD)
					fDMP = fHD;
				float fDMM = 0;
				if(fLD >0 && fLD > fHD)
					fDMM = fLD;
				fDMPSum = fDMPSum - fDMP;
				fDMMSum = fDMMSum - fDMM;
			}

			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo->dwTime;
			sValue.strName = "PDI";
			sValue.fValue  = fDMPSum *100/vecConfig[0]/(fTRSum/vecConfig[0]);
			m_vecData[0].insert(m_vecData[0].begin(), sValue);	

			sValue.strName = "MDI";
			sValue.fValue  = fDMMSum *100/vecConfig[0]/(fTRSum/vecConfig[0]);
			m_vecData[1].insert(m_vecData[1].begin(), sValue);	
			
			fADXSum =( fADXSum + (float)fabs(m_vecData[1][0].fValue - m_vecData[0][0].fValue)/(m_vecData[1][0].fValue + m_vecData[0][0].fValue)*100);
		}

		if(i + vecConfig[0] + vecConfig[1] -1 <= nSize)
		{	
			if(i + vecConfig[0] + vecConfig[1] -1 < nSize)
			{
				if((int)m_vecData[0].size() > vecConfig[1] && (int)m_vecData[1].size() > vecConfig[1])
				{					
					fADXSum = fADXSum - (float)fabs(m_vecData[1][vecConfig[1]].fValue - m_vecData[0][vecConfig[1]].fValue)/(m_vecData[1][vecConfig[1]].fValue + m_vecData[0][vecConfig[1]].fValue)*100;
				}
			}
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo->dwTime;
			sValue.strName = "ADX";
			sValue.fValue  = fADXSum/vecConfig[1]; 
			m_vecData[2].insert(m_vecData[2].begin(), sValue);	
		}

		if(i + vecConfig[0] + vecConfig[1] + vecConfig[1] -1 <= nSize)
		{	
			if((int)m_vecData[2].size() > vecConfig[1])
			{
				STechIndexValue sValue;
				sValue.tmTime  = pStkInfo->dwTime;
				sValue.strName = "ADXR";
				sValue.fValue  = (m_vecData[2][vecConfig[1]].fValue + m_vecData[2][0].fValue)/2;
				m_vecData[3].insert(m_vecData[3].begin(), sValue);	
			}
		}
	}
}
void CTechIndex::Calc_PSY(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 1)
		return;

	ClearMemory();	
	m_enumPhrase  = enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = wxT(PSY);
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 1;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 1;

	int nSize = vecKLine.size();
	if(nSize < vecConfig[0])
		return;
	int nSum12 =0; 
	for(int i = nSize -2; i>=0; i--)
	{
		SKLine* pStkInfo = vecKLine[i];
		SKLine* pStkInfoBefore = vecKLine[i+1];
		if(pStkInfo->fClosePrice > pStkInfoBefore->fClosePrice)
		{
			nSum12 = nSum12 +1;			
		}
		if(i + vecConfig[0] <= nSize)
		{
			if(i + vecConfig[0]< nSize-1)//第一天不算，所以nSize -1
			{
				SKLine* pStkInfoT0 = vecKLine[i+vecConfig[0]];
				SKLine* pStkInfoT0Before = vecKLine[i+vecConfig[0]+1];
				if(pStkInfoT0->fClosePrice > pStkInfoT0Before->fClosePrice)
					nSum12 = nSum12 - 1;					
			}

			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo->dwTime;
			sValue.strName = " ";
			sValue.fValue  = (float)100.0*nSum12/vecConfig[0];
			m_vecData[0].insert(m_vecData[0].begin(), sValue);			
		}		
	}	
}
void CTechIndex::Calc_ROC(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 2)
		return;

	ClearMemory();	
	m_enumPhrase  = enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = wxT(ROC);
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 2;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 2;

	int nSize = vecKLine.size();
	float fSum = 0.0;
	for(int i = nSize - vecConfig[0] -1; i>=0; i--)
	{
		SKLine* pStkInfo = vecKLine[i];
		SKLine* pStkInfo0 = vecKLine[i+ vecConfig[0]];
	
		STechIndexValue sValue;
		sValue.tmTime  = pStkInfo->dwTime;
		sValue.strName = "ROC";
		sValue.fValue  = 100 * (pStkInfo->fClosePrice - pStkInfo0->fClosePrice)/pStkInfo0->fClosePrice;
		m_vecData[0].insert(m_vecData[0].begin(), sValue);	

		fSum = fSum + m_vecData[0][0].fValue;
		if(i + vecConfig[0] + vecConfig[1]  <= nSize)
		{	
			if(i + vecConfig[0] + vecConfig[1]  < nSize)
			{
				fSum = fSum - m_vecData[0][vecConfig[1]].fValue;
			}
			sValue.strName = "ROCMA";
			sValue.fValue  = fSum/vecConfig[1]; 
			m_vecData[1].insert(m_vecData[1].begin(), sValue);	
		}
	}	
}