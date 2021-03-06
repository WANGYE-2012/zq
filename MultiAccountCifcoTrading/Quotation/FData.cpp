#include "StdAfx.h"

#include "../inc/Quotation/FData.h"
#include "../inc/Quotation/KLineView.h"

CFData::CFData(void)
{
	Init();
	m_pWnd = NULL;
	m_bInitKLine = false;
	m_bInitTLine = true;
	m_bInitQIntime =true;
	m_bInitQIntimeFirst = false;
	m_vecKLineCommon = NULL;
	m_Simulate  = 0;
	m_fMaxValue  = MIN_VALUE_DEFAULT;
	m_fMinValue  = MAX_VALUE_DEFAULT;
}
void CFData::Init()
{
	m_PreSettlementPrice = 1;
	m_tmLastTrans_TLine.tm_year = 0;
	m_sKLineLast.dwTime.tm_year = 0;
	m_bSelDay	= false;
	m_bSelWeek	= false;
	m_bSelMonth	= false;
	m_bSelMin1	= false;
	m_bSelMin5	= false;
	m_bSelMin15	= false; 
	m_bSelMin30	= false;
	m_bSelMin60	= false;
	m_enumMark = MARK_UNDEFINE;
	m_tmLastTrans.tm_year = 0;
	m_pDelMark = NULL;

	m_nNeiPan = 0;
	m_nWaiPan = 0;
	m_nNumDigits = 2;	

	
}
CFData::~CFData(void)
{
	ClearMemory_TLine();
	ClearMemory();
	ClearMemory_Mark();
	ClearMemory_TechIndex();

}
void CFData::ClearMemory_TLine()
{
	CAutoLock l(&m_CritSecVector_TLine);
	for(int i = 0; i< (int)m_vecTLine.size() ; i++)
	{
		STLine *pTLine = m_vecTLine[i];
		if(pTLine)
		{
			delete pTLine;
			pTLine = NULL;
		}
	}
	m_vecTLine.clear();
}
void CFData::SetPriceTick(double  PriceTick)
{
	m_PriceTick = PriceTick;
	m_nNumDigits = GetNumDigits(m_PriceTick);
}


int CFData::GetDotDigits()
{//小数点后多少位 
	return m_nNumDigits;
}
bool CFData::SetFuture()
{
	if(!m_bInitTLine)
		return false;

	m_bInitTLine = false;
	m_sTLineLast.dwTime.tm_year = 0;
	ReadFile_TLine();
	return true;
}
bool CFData::ReadFile_TLine()
{
	CAutoLock l(&m_CritSecVector_TLine);
	if(m_vecTimeScales.size() ==0)
		return false;

	struct tm tmDay;
	wxString TradingDay;
	TradingDay = m_strTradingDay;
	tmDay.tm_year	= atoi(TradingDay.Left(4))-1900;//year since 1900		
	tmDay.tm_mon	= atoi(TradingDay.Mid(4,2))-1;//[0, 11]
	tmDay.tm_mday	= atoi(TradingDay.Right(2));

	wxString strPath;
	GetFileSavePath_TLine(strPath);	
	wxString strDir;
	strDir.Printf(_T("%shqData\\"), strPath);
	wxString strSubDir;
	if(m_Simulate == 1)
		strSubDir.Printf(_T("%shqData_simi\\"), strDir);
	else
		strSubDir.Printf(_T("%shqData_Trans\\"), strDir);	
	CreateDirectory(strSubDir, NULL);

	strPath.Printf(_T("%s%s\\"), strSubDir, m_ExchangeName.c_str());
	strPath.Append(wxT("TL\\"));
	
	wxString filepath;
	wxString strFileName;
	wxString strFileName2;
	strFileName2.Printf(_T("%s_%s_%s"), m_ExchangeName.c_str(), m_ContractName.c_str(), m_ContractDate.c_str());
	strFileName.Printf(_T("%s\\%s%s"), strPath.c_str(), strFileName2.c_str(), m_strTradingDay.c_str());

	FILE* fp = fopen( strFileName, "rb" );
	if(fp  == NULL )
		return false;
	int nCount;
	fread( &nCount, sizeof(int) , 1 , fp );
	if(nCount <= 0)
	{
		fclose(fp);
		return true;
	}
	vector<STLine_IO> vecTLine_IO(nCount);
	fread( &vecTLine_IO[0], sizeof(STLine_IO) , nCount , fp );
	fclose(fp);

	for(int i=0; i < (int)vecTLine_IO.size(); i++)
	{
		STLine_IO kIO = vecTLine_IO[i];
		STLine *pTLine = new STLine;;
		time_t t = time_t(kIO.dwTime);
		pTLine->dwTime = *localtime(&t);
		pTLine->dwHoldVolume = kIO.dwHoldVolume;
		pTLine->dwVolume     = kIO.dwVolume;
		pTLine->dwVolumeMoney = (double)kIO.dwVolumeMoney;
		pTLine->fLastPrice    = (double)kIO.fLastPrice;
		m_vecTLine.push_back(pTLine);
	}
	vecTLine_IO.clear();
	return true;
}
void CFData::CalcMaxMin()
{
	if(m_vecTLine.size() == 0)
		return;

	m_fMaxValue  = MIN_VALUE_DEFAULT;
	m_fMinValue = MAX_VALUE_DEFAULT;
	for(int i = 0; i< (int)m_vecTLine.size(); i--)
	{		
		STLine *pTLine = m_vecTLine[i];
		if(pTLine->fLastPrice > m_fMaxValue)
		{
			m_fMaxValue = pTLine->fLastPrice;
		}
		if(pTLine->fLastPrice < m_fMinValue)
		{
			m_fMinValue = pTLine->fLastPrice;
		}
	}
	if(abs(m_fMaxValue - MIN_VALUE_DEFAULT)<0.00001 && abs(m_fMinValue-MAX_VALUE_DEFAULT)<0.00001)
	{
		m_fMaxValue = m_PreSettlementPrice *1.001;
		m_fMinValue = m_PreSettlementPrice*0.999;

		if(abs(m_fMaxValue)<0.00001 && abs(m_fMaxValue)>-0.00001)
			m_fMaxValue = 1;//没有昨结算价很麻烦
	}
	else
	{
		double dbMaxSpan = abs(m_fMaxValue - m_PreSettlementPrice);
		double dbMax2	 = abs(m_fMinValue - m_PreSettlementPrice);
		if(dbMax2 > dbMaxSpan)
			dbMaxSpan = dbMax2;

		m_fMaxValue = m_PreSettlementPrice + dbMaxSpan;
		m_fMinValue = m_PreSettlementPrice - dbMaxSpan;

		if(m_fMaxValue - m_fMinValue > -0.00001 && m_fMaxValue - m_fMinValue <0.00001)
		{
			m_fMaxValue = m_fMinValue *1.01;
			m_fMinValue = m_fMinValue*0.99;
		}
	}

}
struct tm CFData::GetTimeInSpan_TLine(struct tm tmPara, int nSpan)
{
	if(m_vecTimeScales.size() == 0)
	{
		struct tm tm1;
		tm1.tm_year = 0;
		return tm1;
	}
	time_t tmMax =   mktime(&m_vecTimeScales[m_vecTimeScales.size()-1]);
	time_t tReturn;
	//用这个值判断成功否
	time_t tm11 =   mktime(&tmPara);
	int nIndex = 0;	
	for(int i= 0; i< (int)m_vecTimeScales.size(); i++)
	{
		if(difftime(mktime(&m_vecTimeScales[i]), mktime(&tmPara)) >0) 
		{
			nIndex = i;
			break;
		}
		if(i == m_vecTimeScales.size() -1)
			return m_vecTimeScales[i];//15:00:07 算成 15:00:00
	}
	if(nIndex == 0)
	{		
		
		nIndex = nIndex +1;
		//	return m_vecTimeScales[0];	
	}
	else if( nIndex%2==0)
	{
		//	nIndex = nIndex +1;
		return m_vecTimeScales[nIndex-1];//11:30:10 算成11:30分
	}
	DWORD	dwCount = 0;
	for(int i = 0; i<nIndex-1; i++)
	{		
		time_t t1 = mktime( &m_vecTimeScales[i] );
		time_t t2 = mktime( &m_vecTimeScales[i+1]);
		DWORD dw = abs(difftime(t1, t2));
		dwCount += dw;
		i++;
	}
	if(nIndex != 0)
	{
		DWORD dwn = dwCount%nSpan;
		time_t t1 = mktime( &m_vecTimeScales[nIndex -1]);
		time_t t3 = t1+dwn;
		while(t3 <= tm11)
		{
			t3 += nSpan;
		}
		tReturn = t3;	
	}
	if(tReturn > tmMax)
		tReturn = tmMax;

	struct tm tmReturn;
	tmReturn.tm_year = 0;
	tmReturn = *localtime(&tReturn);

	return tmReturn;
}
bool CFData::GetFileSavePath_TLine(wxString &filepath)
{
	char localPath[256];
	memset(localPath, 0, 256);
	GetModuleFileName( NULL, localPath, 256);
	string filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	filepath = filename.substr(0, splitpos+1);
	return true;
}
bool CFData::SaveFile_TLine()
{	
	CAutoLock l(&m_CritSecVector_TLine);
	if(m_ExchangeName == "")
		return false;
	if(m_vecTLine.size() ==0)
		return false;

	wxString strPath;
	GetFileSavePath_TLine(strPath);	
	wxString strDir;
	strDir.Printf(_T("%shqData\\"), strPath);
	CreateDirectory(strDir, NULL);
	wxString strSubDir;
	if(m_Simulate == 1)
		strSubDir.Printf(_T("%shqData_simi\\"), strDir);
	else
		strSubDir.Printf(_T("%shqData_Trans\\"), strDir);	
	CreateDirectory(strSubDir, NULL);

	strPath.Printf(_T("%s%s\\"), strSubDir, m_ExchangeName.c_str());
	CreateDirectory(strPath.c_str(), NULL);
	strPath.Append(wxT("TL\\"));
	CreateDirectory(strPath.c_str(), NULL);

	wxString filepath;
	wxString strFileName;
	wxString strFileName2;
	strFileName2.Printf(_T("%s_%s_%s"), m_ExchangeName.c_str(), m_ContractName.c_str(), m_ContractDate.c_str());
	strFileName.Printf(_T("%s\\%s%s"), strPath.c_str(), strFileName2.c_str(), m_strTradingDay.c_str());
	//	filepath.Printf(_T("%s\\%s"), strPath.c_str(), strFileName.c_str());
	SaveOneFile_TLine(strFileName, m_vecTLine);


	return true;
}
bool CFData::SaveOneFile_TLine(wxString filepath, vector<STLine *>& vecTLine)
{
	if(m_vecTLine.size() ==0)
		return false;

	vector<STLine_IO> vecTLine_IO;
	for(int i=0; i< (int)vecTLine.size(); i++)
	{
		STLine p = *(vecTLine[i]);
		STLine_IO temp;
		temp.dwTime = mktime(& p.dwTime);
		temp.dwVolumeMoney = p.dwVolumeMoney;
		temp.dwHoldVolume = p.dwHoldVolume;
		temp.dwVolume     = p.dwVolume;	
		temp.fLastPrice   = p.fLastPrice ;
		vecTLine_IO.push_back(temp);
	}
	FILE* fp = fopen( filepath, "wb" );
	if(fp  == NULL )
		return false;
	int nCount = vecTLine_IO.size();
	if(nCount != 0)
	{
		fwrite(&nCount, sizeof(int), 1, fp);
		fwrite( &vecTLine_IO[0], sizeof(STLine_IO) , vecTLine_IO.size(), fp );
	}
	fclose(fp);
	return true;
}
void CFData::SetLastTransTime_TLine(struct tm tmTime)
{
	CAutoLock l(&m_CritSecVector_TLine);
	if(m_ExchangeName == ""
		&& m_ContractName == ""
		&& m_ContractDate == "")
		return;
	m_tmLastTrans_TLine = tmTime;

	if(m_vecTimeScales.size() == 0)
		return;
	if(difftime(mktime(&m_vecTimeScales[0]), mktime(&tmTime)) > 0) 
		return;
	//if(difftime(mktime(&m_vecTimeScales[m_vecTimeScales.size()-1]), mktime(&tmTime)) < 0) 
	//	return;//有时候会有超出交易时间的数据，所以这个代码不能要

	tmTime.tm_min -= 1;
	mktime( &tmTime);
	tmTime = GetTimeInSpan_TLine(tmTime, 60);
	if(m_vecTLine.size() == 0)
	{	
		return;
		STLine *pTLine = new STLine;
		pTLine->fLastPrice = m_PreSettlementPrice;
		pTLine->dwVolume   = 0;
		pTLine->dwHoldVolume = 0;
		pTLine->dwVolumeMoney = 0;
		pTLine->dwTime = GetTimeInSpan_TLine(tmTime, 60);
		m_vecTLine.push_back(pTLine);	
		if(WRITELOG)
		{
			AppendLog(pTLine, wxT("add7"));
		}		
	}
	else
	{
		STLine* pLast = m_vecTLine[0];

		struct tm tTime = GetTimeInSpan_TLine(tmTime, 60);

		if(tTime.tm_year ==pLast->dwTime.tm_year
			&& tTime.tm_mon == pLast->dwTime.tm_mon 
			&& tTime.tm_mday == pLast->dwTime.tm_mday
			&& tTime.tm_hour == pLast->dwTime.tm_hour
			&&tTime.tm_min == pLast->dwTime.tm_min)
		{
			return;
		}	
		int nTimeSpanTemp = difftime(mktime(&pLast->dwTime), mktime(&tTime));
		if(nTimeSpanTemp < 0)
		{
			STLine *pTLine = new STLine;
			pTLine->fLastPrice = pLast->fLastPrice;
			pTLine->dwVolume   = 0;
			pTLine->dwHoldVolume = 0;
			pTLine->dwVolumeMoney = 0;
			pTLine->dwTime = tTime;
			m_vecTLine.insert(m_vecTLine.begin(), pTLine);	
			if(WRITELOG)
			{
				AppendLog(pTLine, wxT("add8"));
			}		
		}
		else
			return;
	}
}
void CFData::SetScales(vector<struct tm>& vecScales)
{
	if(vecScales.size() < 2)
		return;

	m_vecTimeScales.clear();
	m_vecTimeScales = vecScales;
	m_nOneDayScalesCount = m_vecTimeScales.size();

	m_vecTMWidthSpan.clear();
	m_TMWidthCount = 0;
	for(int i = 0; i< (int)m_vecTimeScales.size(); i++)
	{
		struct tm tm1 = m_vecTimeScales[i];
		struct tm tm2 = m_vecTimeScales[i+1];
		DWORD dw = abs(difftime(mktime(&tm1), mktime(&tm2)));
		m_vecTMWidthSpan.push_back(dw);
		m_TMWidthCount = m_TMWidthCount + dw;
		i++;
	}
}
void CFData::SetTodayScales(vector<struct tm>& vecTimeScales)
{
	CAutoLock l(&m_CritSecVector_TLine);
	SetScales(vecTimeScales);	
}
void CFData::SetLastClosePrice(double dbPrice)
{
	CAutoLock l(&m_CritSecVector_TLine);

	m_PreSettlementPrice = dbPrice;
}
BOOL CFData::SetInTimeData_TLine(STLine* pTLine)
{
	CAutoLock l(&m_CritSecVector_TLine);

	if(m_vecTimeScales.size() == 0)
	{
		if(pTLine)
		{	
			delete pTLine;
			pTLine = NULL;
		}

		return false;
	}
	//	if(m_sTLineLast.dwTime.tm_year !=0 && difftime(mktime(&m_sTLineLast.dwTime), mktime(&pTLine->dwTime)) ==0)
	//		return false;//重复行情


	BOOL bNotDelMemory = SetInTimeData_DiffSpan_TLine(pTLine);
	if(!bNotDelMemory)
	{	
		delete pTLine;
		pTLine = NULL;
	}

	return TRUE;
}
BOOL CFData::SetInTimeData_DiffSpan_TLine(STLine* pTLine)
{
	bool bSameMin = false;
	STLine sTemp = *pTLine;
	struct tm TM = GetTimeInSpan_TLine(pTLine->dwTime, 60);//60分钟，3600秒
	pTLine->dwTime  = TM;//这样处理就需要先过滤重复行情
	pTLine->dwTime.tm_sec   = 0;

	if(m_vecTLine.size() == 0)
	{
		bSameMin = true;
		m_vecTLine.push_back(pTLine);	
		struct tm tm1 = m_vecTimeScales[0];
		struct tm tm2 = pTLine->dwTime;
		if(difftime(mktime(&tm1), mktime(&tm2)) !=0)
		{
			pTLine->dwVolume		= 0;
			pTLine->dwVolumeMoney   = 0;
		}
		pTLine->dwTime  = TM;
		if(WRITELOG)
		{
			AppendLog(pTLine, wxT("add"));
		}		
	}
	else
	{
		STLine* pLast = m_vecTLine[0];

		if(pLast->dwTime.tm_year == TM.tm_year
			&& pLast->dwTime.tm_mon == TM.tm_mon 
			&& pLast->dwTime.tm_mday == TM.tm_mday
			&& pLast->dwTime.tm_hour == TM.tm_hour)
		{
			if(WRITELOG)
			{
				AppendLog(pLast, wxT("orgin"));
			}
			/*	if(pLast->dwTime.tm_min == pTLine->dwTime.tm_min && pLast->dwTime.tm_sec == pTLine->dwTime.tm_sec)
			{
			bSameMin = false;//重复行情
			m_sTLineLast = sTemp;
			return bSameMin;
			}*/
			if(pLast->dwTime.tm_min == TM.tm_min )
			{
				bSameMin = false;
				pLast->dwTime     = pTLine->dwTime;
				pLast->fLastPrice = pTLine->fLastPrice;
				if(m_sTLineLast.dwTime.tm_year != 0)
				{
					pLast->dwVolume  += pTLine->dwVolume - m_sTLineLast.dwVolume;
					pLast->dwVolumeMoney += pTLine->dwVolumeMoney - m_sTLineLast.dwVolumeMoney;
				}					
				if(WRITELOG)
				{
					AppendLog(pTLine, wxT("modify"));
				}
			}
		}	
		int nTimeSpanTemp = difftime(mktime(&pLast->dwTime), mktime(&pTLine->dwTime));//刚加的，要测试一下
		if(nTimeSpanTemp >0)
		{
			STLine* pLastFirst = m_vecTLine[m_vecTLine.size() - 1];
			if(pLastFirst && difftime(mktime(&pLastFirst->dwTime), mktime(&TM)) > 0)
			{//比第一个数据还要早，主要是正对模拟行情处理
				bSameMin = true;
				if(m_sTLineLast.dwTime.tm_year != 0)
				{
					pTLine->dwVolume  = pTLine->dwVolume - m_sTLineLast.dwVolume;
					pTLine->dwVolumeMoney = pTLine->dwVolumeMoney - m_sTLineLast.dwVolumeMoney;
				}
				else
				{
					pTLine->dwVolume		= 0;
					pTLine->dwVolumeMoney   = 0;
				}
				m_vecTLine.push_back(pTLine);					
			}
			else
			{			
				vector<STLine *>::iterator itLast = m_vecTLine.end();
				for(vector<STLine *>::iterator it = m_vecTLine.begin(); it!= m_vecTLine.end(); it++)
				{
					STLine* pLastTemp = *it;
					if(pLastTemp->dwTime.tm_year == TM.tm_year
						&& pLastTemp->dwTime.tm_mon == TM.tm_mon 
						&& pLastTemp->dwTime.tm_mday == TM.tm_mday
						&& pLastTemp->dwTime.tm_hour == TM.tm_hour
						&& pLastTemp->dwTime.tm_min == TM.tm_min)
					{//如果是按时间先加进去的则有可能出现这种情况
						bSameMin = false;

						pLastTemp->fLastPrice = pTLine->fLastPrice;
						if(m_sTLineLast.dwTime.tm_year != 0)
						{
							pLastTemp->dwVolume  += pTLine->dwVolume - m_sTLineLast.dwVolume;
							pLastTemp->dwVolumeMoney += pTLine->dwVolumeMoney - m_sTLineLast.dwVolumeMoney;
						}						
						if(WRITELOG)
						{
							AppendLog(pTLine, wxT("modify2"));
						}
						break;
					}
					else if(difftime(mktime(&pLastTemp->dwTime), mktime(&TM)) < 0)
					{//处于中间的某个数据，兼容模拟行情	
						bSameMin = true;
					//	if(itLast == m_vecTLine.end())
					//		m_vecTLine.insert(m_vecTLine.begin(), pTLine);	
					//	else
						if(m_sTLineLast.dwTime.tm_year != 0)
						{
							pTLine->dwVolume  = pTLine->dwVolume - m_sTLineLast.dwVolume;
							pTLine->dwVolumeMoney = pTLine->dwVolumeMoney - m_sTLineLast.dwVolumeMoney;
						}
						else
						{
							pTLine->dwVolume		= 0;
							pTLine->dwVolumeMoney   = 0;
						}
						m_vecTLine.insert(it, pTLine);
						if(WRITELOG)
						{
							AppendLog(pTLine, wxT("insert by2"));
						}
						break;
					}				
					itLast = it;
				}	
			}			
		}
		else if(!bSameMin && nTimeSpanTemp < 0)
		{
			if(WRITELOG)
			{
				AppendLog(pTLine, wxT("append1"));
			}
			if(m_sTLineLast.dwTime.tm_year != 0)
			{
				pTLine->dwVolume  = pTLine->dwVolume - m_sTLineLast.dwVolume;
				pTLine->dwVolumeMoney = pTLine->dwVolumeMoney - m_sTLineLast.dwVolumeMoney;
			}
			else
			{
				pTLine->dwVolume		= 0;
				pTLine->dwVolumeMoney   = 0;
			}
			m_vecTLine.insert(m_vecTLine.begin(), pTLine);				
			if(WRITELOG)
			{
				AppendLog(pTLine, wxT("append2"));
			}

			bSameMin = true;			
		}
	}

	m_sTLineLast = sTemp;
	return bSameMin;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

bool CFData::SetPhrase(EnumPhrase enumPhrase)
{
	CAutoLock l(&m_CritSecVector);
	if(m_bInitKLine && m_enumPhrase == enumPhrase)
		return false;
	
	m_bInitKLine = true;
	SaveFileMarks();//标准需要保存
	m_enumPhrase = enumPhrase;
	GetVectorCommon(m_enumPhrase);
	ReadFile();
	ClearMemory_Mark();
	ReadFileMarks();
	return true;

}
bool CFData::IsOneWeek(struct tm tm1, struct tm tm2)
{
	wxDateTime date1(tm1), date2(tm2);
	if(date1.GetYear() == date2.GetYear() && date1.GetWeekOfYear() == date2.GetWeekOfYear())
		return true;

	return false;
}

bool CFData::IsOneMonth(struct tm tm1, struct tm tm2)
{
	wxDateTime date1(tm1), date2(tm2);
	if(date1.GetYear() == date2.GetYear() && date1.GetMonth() == date2.GetMonth())
		return true;

	return false;
}
bool CFData::ReadFileMarks()
{
	wxString strPath;
	GetFileSavePath(strPath);	
	wxString strDir;
	strDir.Printf(_T("%shqData\\"), strPath);
	wxString strSubDir;
	if(m_Simulate == 1)
		strSubDir.Printf(_T("%shqData_simi\\"), strDir);
	else
		strSubDir.Printf(_T("%shqData_Trans\\"), strDir);	
	CreateDirectory(strSubDir, NULL);

	strPath.Printf(_T("%s%s\\"), strSubDir, m_ExchangeName.c_str());
	strPath.Append(wxT("Marks\\"));
	
	wxString filepath;
	wxString strFileName2;
	GetSaveFileName(m_ExchangeName, m_ContractName, m_ContractDate, m_enumPhrase,strFileName2);
	filepath.Printf(_T("%s\\%s_Mark"), strPath.c_str(), strFileName2.c_str());

	ifstream infile(filepath,ios::in|ios::binary);  	
	while(!infile.eof())
	{
		if (infile.peek() == EOF)
			break;
		int i =0;		
		SMark *pTemp = new SMark;
		infile >> i;
		EnumMark enumMark = (EnumMark)i;
		infile >> pTemp;
		m_mapMarks.insert(MAPsmark::value_type(enumMark,  pTemp));					

	}
	infile.close();	
	return true;
}
bool CFData::SaveFileMarks()
{
	if(m_mapMarks.size() ==0)
		return true;
	wxString strPath;
	GetFileSavePath(strPath);	
	wxString strDir;
	strDir.Printf(_T("%shqData\\"), strPath);
	CreateDirectory(strDir, NULL);
	
	wxString strSubDir;
	if(m_Simulate == 1)
		strSubDir.Printf(_T("%shqData_simi\\"), strDir);
	else
		strSubDir.Printf(_T("%shqData_Trans\\"), strDir);	
	CreateDirectory(strSubDir, NULL);

	strPath.Printf(_T("%s%s\\"), strSubDir, m_ExchangeName.c_str());
	CreateDirectory(strPath.c_str(), NULL);
	strPath.Append(wxT("Marks\\"));
	CreateDirectory(strPath.c_str(), NULL);

	wxString filepath;
	wxString strFileName;
	GetSaveFileName(m_ExchangeName, m_ContractName, m_ContractDate, m_enumPhrase,strFileName);
	filepath.Printf(_T("%s\\%s_Mark"), strPath.c_str(), strFileName.c_str());


	SaveOneFile(filepath, m_mapMarks);
	return true;
}
bool CFData::SaveOneFile(wxString filepath, MAPsmark& mapMarks)
{
	if(mapMarks.size() ==0)
		return true;
	ofstream outfile(filepath ,ios::out|ios::binary);
	MAPsmark::iterator it = mapMarks.begin();
	while(it != mapMarks.end())
	{
		MAPsmark::iterator itTemp = it;
		it++;
		SMark *pTemp = itTemp->second;
		outfile << itTemp->first;
		outfile << pTemp;
	}
	return true;
}
bool CFData::AddMarks(KTYPE enumType,struct tm tmTime, double dbPrice)
{
	map<int, SMark *>::iterator it = m_mapMarks.find(m_enumMark);
	if(it != m_mapMarks.end())
	{
		SMark *pTemp	= it->second;
		SPoint *sp		= new SPoint;
		sp->enumType    = enumType;
		sp->dwPrice		= dbPrice;
		sp->dwTime		= tmTime;
		pTemp->vecPt.push_back(sp);
	}
	else
	{
		SMark *pTemp	 = new SMark;
		SPoint *sp		 = new SPoint;
		sp->enumType	 = enumType;
		sp->dwPrice		 = dbPrice;
		sp->dwTime		 = tmTime;
		pTemp->vecPt.push_back(sp);
		m_mapMarks.insert(MAPsmark::value_type(m_enumMark,  pTemp));						
	}	

/*	if(m_pKLineCtrl)
		m_pKLineCtrl->SetMarks(m_mapMarks);
	if(m_pKVolumeCtrl)
		m_pKVolumeCtrl->SetMarks(m_mapMarks);
*/
	return true;
}
int CFData::GetPhraseSpan(EnumPhrase enumPhrase)
{
	int nday = 1;
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		nday = 60;
		break;
	case PHRASE_5MIN:
		nday = 300;
		break;
	case PHRASE_15MIN:
		nday = 900;
		break;
	case PHRASE_30MIN:
		nday = 1800;
		break;
	case PHRASE_60MIN:
		nday = 3600;
		break;
	}
	return nday;
}

bool CFData::GetFileSavePath(wxString &filepath)
{
	char localPath[256];
	memset(localPath, 0, 256);
	GetModuleFileName( NULL, localPath, 256);
	string filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	filepath = filename.substr(0, splitpos+1);

	return true;
}
bool CFData::GetSaveFileName(string  ExchangeName, string  ContractName, string  ContractDate, EnumPhrase enumPhrase, wxString &filepath)
{
	string strPhrase;
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		strPhrase = "min1";
		break;
	case PHRASE_5MIN:
		strPhrase = "min5";
		break;
	case PHRASE_15MIN:
		strPhrase = "min15";
		break;
	case PHRASE_30MIN:
		strPhrase = "min30";
		break;
	case PHRASE_60MIN:
		strPhrase = "min60";
		break;
	case PHRASE_DAY:
		strPhrase = "day";
		break;
	case PHRASE_WEEK:
		strPhrase = "week";
		break;
	case PHRASE_MONTH:
		strPhrase = "month";
		break;

	}
	filepath.Printf(_T("%s_%s_%s_%s"), ExchangeName.c_str(), ContractName.c_str(), ContractDate.c_str(), strPhrase.c_str());
	return true;
}
bool CFData::SaveOneFile(wxString filepath, vector<SKLine *>& vecKLine)
{
	vector<SKLine_IO> m_vecKLine_IO;
	for(int i=0; i< (int)vecKLine.size(); i++)
	{
		SKLine p = *(vecKLine[i]);
		SKLine_IO temp;
		temp.dwTime = mktime(& p.dwTime);
		temp.fOpenPrice = p.fOpenPrice;
		temp.fClosePrice= p.fClosePrice;
		temp.fHighPrice   = p.fHighPrice;
		temp.fLowPrice    = p.fLowPrice;
		temp.dwHoldVolume = p.dwHoldVolume;
		temp.dwVolume     = p.dwVolume;	
		temp.fSumTradeVolume = p.fSumTradeVolume;
		temp.dJieSuan		 = p.dJieSuan;
		m_vecKLine_IO.push_back(temp);
	}
	FILE* fp = fopen( filepath, "wb" );
	if(fp  == NULL )
		return false;
	int nCount = m_vecKLine_IO.size();
	if(nCount != 0)
	{
		fwrite(&nCount, sizeof(int), 1, fp);
		fwrite( &m_vecKLine_IO[0], sizeof(SKLine_IO) , m_vecKLine_IO.size() , fp );
	}
	fclose(fp);
/* 这个函数优化后效率没有相差多少
	ofstream outfile(filepath ,ios::out|ios::binary);
	for(int i=0; i<vecKLine.size(); i++)
	{
		outfile << vecKLine[i];
	}
	outfile.close();	
	*/
	return true;
}
bool CFData::SaveFile()
{	
	if(m_ExchangeName == "")
		return false;
	wxString strPath;
	GetFileSavePath(strPath);	
	wxString strDir;
	strDir.Printf(_T("%shqData\\"), strPath);
	CreateDirectory(strDir, NULL);
	wxString strSubDir;
	if(m_Simulate == 1)
		strSubDir.Printf(_T("%shqData_simi\\"), strDir);
	else
		strSubDir.Printf(_T("%shqData_Trans\\"), strDir);	
	CreateDirectory(strSubDir, NULL);
	strPath.Printf(_T("%s%s\\"), strSubDir, m_ExchangeName.c_str());
	CreateDirectory(strPath.c_str(), NULL);

	wxString filepath;
	wxString strFileName;
	/*
	GetSaveFileName(m_ExchangeName, m_ContractName, m_ContractDate, m_enumPhrase,strFileName);
	filepath.Printf(_T("%s\\%s"), strPath.c_str(), strFileName.c_str());
	SaveOneFile(filepath, m_vecKLine);
	*/
	if(m_vecKLineDay.size()!= 0 && m_bSelDay)//只有在读取本地数据以后，才会保存相应的周期数据，要不然本地数据会被覆盖掉
	{
		wxString strPathTemp = strPath;
		strPathTemp.Append(wxT("Day\\"));
		CreateDirectory(strPathTemp.c_str(), NULL);
		GetSaveFileName(m_ExchangeName, m_ContractName, m_ContractDate, PHRASE_DAY,strFileName);
		filepath.Printf(_T("%s\\%s"), strPathTemp.c_str(), strFileName.c_str());
		SaveOneFile(filepath, m_vecKLineDay);
	}

	if(m_vecKLineWeek.size()!= 0 && m_bSelWeek)
	{
		wxString strPathTemp = strPath;
		strPathTemp.Append(wxT("Week\\"));
		CreateDirectory(strPathTemp.c_str(), NULL);
		GetSaveFileName(m_ExchangeName, m_ContractName, m_ContractDate, PHRASE_WEEK,strFileName);
		filepath.Printf(_T("%s\\%s"), strPathTemp.c_str(), strFileName.c_str());
		SaveOneFile(filepath, m_vecKLineWeek);
	}

	if(m_vecKLineMonth.size()!= 0 && m_bSelMonth)
	{
		wxString strPathTemp = strPath;
		strPathTemp.Append(wxT("Month\\"));
		CreateDirectory(strPathTemp.c_str(), NULL);
		GetSaveFileName(m_ExchangeName, m_ContractName, m_ContractDate, PHRASE_MONTH,strFileName);
		filepath.Printf(_T("%s\\%s"), strPathTemp.c_str(), strFileName.c_str());
		SaveOneFile(filepath, m_vecKLineMonth);
	}

	if(m_vecKLineMin1.size()!= 0&& m_bSelMin1)
	{
		wxString strPathTemp = strPath;
		strPathTemp.Append(wxT("Min1\\"));
		CreateDirectory(strPathTemp.c_str(), NULL);
		GetSaveFileName(m_ExchangeName, m_ContractName, m_ContractDate, PHRASE_1MIN,strFileName);
		filepath.Printf(_T("%s\\%s"), strPathTemp.c_str(), strFileName.c_str());
		SaveOneFile(filepath, m_vecKLineMin1);
	}	

	if(m_vecKLineMin5.size()!= 0 && m_bSelMin5)
	{
		wxString strPathTemp = strPath;
		strPathTemp.Append(wxT("Min5\\"));
		CreateDirectory(strPathTemp.c_str(), NULL);
		GetSaveFileName(m_ExchangeName, m_ContractName, m_ContractDate, PHRASE_5MIN,strFileName);
		filepath.Printf(_T("%s\\%s"), strPathTemp.c_str(), strFileName.c_str());
		SaveOneFile(filepath, m_vecKLineMin5);
	}	
	//////////////////////////////////////////////////////////////////////////
	//以后只保留1分钟，5分钟基础数据，15,30,60分钟不再保存，目前因为没有5分到其他的转换函数，暂时先简单化处理
	if(m_vecKLineMin15.size()!= 0 && m_bSelMin15)
	{
		wxString strPathTemp = strPath;
		strPathTemp.Append(wxT("Min15\\"));
		CreateDirectory(strPathTemp.c_str(), NULL);
		GetSaveFileName(m_ExchangeName, m_ContractName, m_ContractDate, PHRASE_15MIN,strFileName);
		filepath.Printf(_T("%s\\%s"), strPathTemp.c_str(), strFileName.c_str());
		SaveOneFile(filepath, m_vecKLineMin15);
	}	

	if(m_vecKLineMin30.size()!= 0 && m_bSelMin30)
	{
		wxString strPathTemp = strPath;
		strPathTemp.Append(wxT("Min30\\"));
		CreateDirectory(strPathTemp.c_str(), NULL);
		GetSaveFileName(m_ExchangeName, m_ContractName, m_ContractDate, PHRASE_30MIN,strFileName);
		filepath.Printf(_T("%s\\%s"), strPathTemp.c_str(), strFileName.c_str());
		SaveOneFile(filepath, m_vecKLineMin30);
	}	

	if(m_vecKLineMin60.size()!= 0 && m_bSelMin60)
	{
		wxString strPathTemp = strPath;
		strPathTemp.Append(wxT("Min60\\"));
		CreateDirectory(strPathTemp.c_str(), NULL);
		GetSaveFileName(m_ExchangeName, m_ContractName, m_ContractDate, PHRASE_60MIN,strFileName);
		filepath.Printf(_T("%s\\%s"), strPathTemp.c_str(), strFileName.c_str());
		SaveOneFile(filepath, m_vecKLineMin60);
	}

	SaveFileMarks();
	return true;
}
bool CFData::ReadFile()
{
	GetVectorCommon(m_enumPhrase);

	if(GetSelMinsValue(m_enumPhrase))
		return true;//判断是不是已经读取过，读取过则不再重复读取
	SetSelMinsValue(m_enumPhrase);

	wxString strPath;
	GetFileSavePath(strPath);	
	wxString strDir;
	strDir.Printf(_T("%shqData\\"), strPath);
	wxString strSubDir;
	if(m_Simulate == 1)
		strSubDir.Printf(_T("%shqData_simi\\"), strDir);
	else
		strSubDir.Printf(_T("%shqData_Trans\\"), strDir);	
	CreateDirectory(strSubDir, NULL);
	strPath.Printf(_T("%s%s\\"), strSubDir, m_ExchangeName.c_str());
	switch(m_enumPhrase)
	{
	case PHRASE_1MIN:
		{
			strPath.Append(wxT("Min1\\"));
		}
		break;
	case PHRASE_5MIN:
		{
			strPath.Append(wxT("Min5\\"));
		}
		break;
	case PHRASE_15MIN:
		{
			strPath.Append(wxT("Min15\\"));
		}
		break;
	case PHRASE_30MIN:
		{
			strPath.Append(wxT("Min30\\"));
		}
		break;
	case PHRASE_60MIN:
		{
			strPath.Append(wxT("Min60\\"));
		}
		break;
	case PHRASE_DAY:
		{
			strPath.Append(wxT("Day\\"));
		}
		break;
	case PHRASE_WEEK:
		{
			strPath.Append(wxT("Week\\"));
		}
		break;
	case PHRASE_MONTH:
		{
			strPath.Append(wxT("Month\\"));
		}
		break;
	}
	wxString filepath;
	wxString strFileName2;
	GetSaveFileName(m_ExchangeName, m_ContractName, m_ContractDate, m_enumPhrase,strFileName2);
	filepath.Printf(_T("%s%s"), strPath.c_str(), strFileName2.c_str());

	FILE* fp = fopen( filepath, "rb" );
	if(fp  == NULL )
		return false;
	int nCount;
	fread( &nCount, sizeof(int) , 1 , fp );	
	if(nCount <= 0)
	{
		fclose(fp);
		return true;
	}
	vector<SKLine_IO> vecKLine_IO(nCount);
	fread( &vecKLine_IO[0], sizeof(SKLine_IO) , nCount , fp );
	fclose(fp);
	vector<SKLine *> vecKLineCommon;
	for(int i=0; i < (int)vecKLine_IO.size(); i++)
	{
		SKLine_IO kIO = vecKLine_IO[i];
		SKLine *pKLine = new SKLine;;
		time_t t = time_t(kIO.dwTime);
		pKLine->dwTime = *localtime(&t);
		pKLine->fOpenPrice = (double)kIO.fOpenPrice;
		pKLine->fHighPrice = (double)kIO.fHighPrice;
		pKLine->fLowPrice  = (double)kIO.fLowPrice;
		pKLine->fClosePrice = (double)kIO.fClosePrice;
		pKLine->dwVolume    = kIO.dwVolume;
		pKLine->fSumTradeVolume = (double)kIO.fSumTradeVolume;
		pKLine->dwHoldVolume    = kIO.dwHoldVolume;
		pKLine->dJieSuan        = (double)kIO.dJieSuan;
		vecKLineCommon.push_back(pKLine);
	}
	vecKLine_IO.clear();

	int nSize = m_vecKLineCommon->size();
	SKLine *pFirst = NULL;
	vector<SKLine *>::iterator itBegin;
	if(nSize != 0)
	{
		pFirst = (*m_vecKLineCommon)[nSize-1];
		itBegin = m_vecKLineCommon->end() - nSize;
	}
	for(int i=0; i< (int)vecKLineCommon.size(); i++)
	{
		SKLine *p = vecKLineCommon[i];
		
		if(pFirst == NULL)
			m_vecKLineCommon->push_back(p);
		else
		{
			if(m_enumPhrase == PHRASE_DAY) 
			{
				if(p->dwTime.tm_year == pFirst->dwTime.tm_year 
					&& p->dwTime.tm_mon == pFirst->dwTime.tm_mon
					&& p->dwTime.tm_mday == pFirst->dwTime.tm_mday)
				{
					if(p->fHighPrice > pFirst->fHighPrice  )
						pFirst->fHighPrice	= p->fHighPrice;
					if(p->fLowPrice < pFirst->fLowPrice)
						pFirst->fLowPrice	= p->fLowPrice;
				}
				else
					m_vecKLineCommon->push_back(p);//insert 还是push__back？
			}
			else if(m_enumPhrase ==PHRASE_WEEK )
			{
				if(IsOneWeek(p->dwTime, pFirst->dwTime)) 					
				{
					if(p->fHighPrice > pFirst->fHighPrice  )
						pFirst->fHighPrice	= p->fHighPrice;
					if(p->fLowPrice < pFirst->fLowPrice)
						pFirst->fLowPrice	= p->fLowPrice;
				}
				else
					m_vecKLineCommon->push_back(p);//insert 还是push__back？
			}
			else if(m_enumPhrase ==PHRASE_MONTH )
			{
				if(IsOneMonth(p->dwTime, pFirst->dwTime)) 		
				{
					if(p->fHighPrice > pFirst->fHighPrice  )
						pFirst->fHighPrice	= p->fHighPrice;
					if(p->fLowPrice < pFirst->fLowPrice)
						pFirst->fLowPrice	= p->fLowPrice;
				}
				else
					m_vecKLineCommon->push_back(p);//insert 还是push__back？
			}
			else
			{
				int nSpan = GetPhraseSpan(m_enumPhrase);
				struct tm tm1 = p->dwTime;//
				struct tm tm2 = GetTimeInSpan(pFirst->dwTime, nSpan);
				int nvalue = difftime(mktime(&tm1), mktime(&tm2));
				if(nvalue< 0)
				{	
					struct tm tm1 = GetTimeInSpan(p->dwTime, nSpan);
					struct tm tm2 = GetTimeInSpan(pFirst->dwTime, nSpan);
					int nvalueTtoday = difftime(mktime(&tm1), mktime(&tm2));
					if(nvalueTtoday == 0 
						&& p->dwTime.tm_mday ==  pFirst->dwTime.tm_mday
						&& p->dwTime.tm_mon  == pFirst->dwTime.tm_mon
						&& p->dwTime.tm_year  == pFirst->dwTime.tm_year)
					{//同一天，同一个时间周期内
						SKLine *pKline = *itBegin;
						if(p->fHighPrice > pKline->fHighPrice  )
							pKline->fHighPrice	= p->fHighPrice;
						if(p->fLowPrice < pKline->fLowPrice)
							pKline->fLowPrice	= p->fLowPrice;
						pKline->dwVolume = pKline->dwVolume + p->dwVolume;
						pKline->fSumTradeVolume = pKline->fSumTradeVolume + p->fSumTradeVolume;
					}
					else
					{
						m_vecKLineCommon->push_back(p);
						itBegin = m_vecKLineCommon->end() - nSize;
					}					
				}
			}			

		}
	}
	vecKLineCommon.clear();

/*	int nSize = m_vecKLineCommon->size();
	SKLine *pFirst = NULL;
	vector<SKLine *>::iterator itBegin;
	if(nSize != 0)
	{
		pFirst = (*m_vecKLineCommon)[nSize-1];
		itBegin = m_vecKLineCommon->end() - nSize;
	}
	ifstream infile(filepath,ios::in|ios::binary);  	
	while(!infile.eof())
	{
		if (infile.peek() == EOF)
			break;
		SKLine *p = new SKLine;
		infile >> p;
		if(pFirst == NULL)
			m_vecKLineCommon->push_back(p);
		else
		{
			if(m_enumPhrase == PHRASE_DAY) 
			{
				if(p->dwTime.tm_year == pFirst->dwTime.tm_year 
					&& p->dwTime.tm_mon == pFirst->dwTime.tm_mon
					&& p->dwTime.tm_mday == pFirst->dwTime.tm_mday)
				{
					if(p->fHighPrice > pFirst->fHighPrice  )
						pFirst->fHighPrice	= p->fHighPrice;
					if(p->fLowPrice < pFirst->fLowPrice)
						pFirst->fLowPrice	= p->fLowPrice;
				}
				else
					m_vecKLineCommon->push_back(p);//insert 还是push__back？
			}
			else if(m_enumPhrase ==PHRASE_WEEK )
			{
				if(IsOneWeek(p->dwTime, pFirst->dwTime)) 					
				{
					if(p->fHighPrice > pFirst->fHighPrice  )
						pFirst->fHighPrice	= p->fHighPrice;
					if(p->fLowPrice < pFirst->fLowPrice)
						pFirst->fLowPrice	= p->fLowPrice;
				}
				else
					m_vecKLineCommon->push_back(p);//insert 还是push__back？
			}
			else if(m_enumPhrase ==PHRASE_MONTH )
			{
				if(IsOneMonth(p->dwTime, pFirst->dwTime)) 		
				{
					if(p->fHighPrice > pFirst->fHighPrice  )
						pFirst->fHighPrice	= p->fHighPrice;
					if(p->fLowPrice < pFirst->fLowPrice)
						pFirst->fLowPrice	= p->fLowPrice;
				}
				else
					m_vecKLineCommon->push_back(p);//insert 还是push__back？
			}
			else
			{
				int nSpan = GetPhraseSpan(m_enumPhrase);
				struct tm tm1 = p->dwTime;
				struct tm tm2 = GetTimeInSpan(pFirst->dwTime, nSpan);
				int nvalue = difftime(mktime(&tm1), mktime(&tm2));
				if(nvalue< 0)
				{					
					m_vecKLineCommon->push_back(p);
					itBegin = m_vecKLineCommon->end() - nSize;
				}
				else if(nvalue == 0)
				{
					SKLine *pKline = *itBegin;
					if(p->fHighPrice > pKline->fHighPrice  )
						pKline->fHighPrice	= p->fHighPrice;
					if(p->fLowPrice < pKline->fLowPrice)
						pKline->fLowPrice	= p->fLowPrice;
					pKline->dwVolume = pKline->dwVolume + p->dwVolume;
					pKline->fSumTradeVolume = pKline->fSumTradeVolume + p->fSumTradeVolume;
				}
			}			

		}
	}
	infile.close();	

*/
	return true;
}
void CFData::ClearMemory_Mark()
{
	MAPsmark::iterator it = m_mapMarks.begin();
	while(it != m_mapMarks.end())
	{
		MAPsmark::iterator itTemp = it;
		it++;
		SMark *pTemp = itTemp->second;

		for(vector<SPoint *>::iterator it = pTemp->vecPt.begin(); it != pTemp->vecPt.end(); it++)
		{
			SPoint *p = *it;
			delete p;
			p= NULL;
		}
		pTemp->vecPt.erase(pTemp->vecPt.begin(), pTemp->vecPt.end());	
		delete pTemp;
		pTemp = NULL;
		m_mapMarks.erase(itTemp);
	}
	if(m_pWnd)
		((CKLineView*)m_pWnd)->ClearMemory_Mark();
/*	if(m_pKLineCtrl)
		m_pKLineCtrl->SetMarks(m_mapMarks);
	if(m_pKVolumeCtrl)
		m_pKVolumeCtrl->SetMarks(m_mapMarks);*/
}
struct tm CFData::GetTimeInSpan(struct tm tmPara, int nSpan)
{
	time_t tmMax =   mktime(&m_vecTimeScales[m_vecTimeScales.size()-1]);
	time_t tReturn;

	//用这个值判断成功否
	time_t tm11 =   mktime(&tmPara);
	int nIndex = 0;	
	for(int i= 0; i< (int)m_vecTimeScales.size(); i++)
	{
		if(difftime(mktime(&m_vecTimeScales[i]), mktime(&tmPara)) >0) 
		{
			nIndex = i;
			break;
		}

		if(i == m_vecTimeScales.size() -1)
			return m_vecTimeScales[i];//15:00:07 算成 15:00:00
	}

	if(nIndex == 0)
	{		
		time_t t1 = mktime( &m_vecTimeScales[0]);
		t1 += nSpan;
		struct tm tmReturn = *localtime(&t1);
		return tmReturn; 
		//	return m_vecTimeScales[0];	
	}
	else if( nIndex%2==0)
	{
		//	nIndex = nIndex +1;
		return m_vecTimeScales[nIndex-1];//11:30:10 算成11:30分
	}
	DWORD	dwCount = 0;
	for(int i = 0; i<nIndex-1; i++)
	{		
		time_t t1 = mktime( &m_vecTimeScales[i] );
		time_t t2 = mktime( &m_vecTimeScales[i+1]);
		DWORD dw = abs(difftime(t1, t2));
		dwCount += dw;
		i++;
	}
	if(nIndex != 0)
	{
		DWORD dwn = dwCount%nSpan;
		time_t t1 = mktime( &m_vecTimeScales[nIndex -1]);
		time_t t3 = t1-dwn;
		while(t3 <= tm11)
		{
			t3 += nSpan;
		}
		if(nIndex + 2 < (int)m_vecTimeScales.size())
		{
			time_t tNext = mktime( &m_vecTimeScales[nIndex]);
			if(t3 > tNext)
			{
				t3 = t3- tNext + mktime(&m_vecTimeScales[nIndex+1]);//11:29:00在30分钟是应该是13:15（9:15开盘）
			}			
		}
		tReturn = t3;	
	}
	if(tReturn > tmMax)
		tReturn = tmMax;

	struct tm tmReturn;
	tmReturn.tm_year = 0;
	tmReturn = *localtime(&tReturn);

	return tmReturn;
}
void CFData::SetLastTransTime(struct tm tmTime)
{
	CAutoLock l(&m_CritSecVector);
	if(m_ExchangeName == ""
		&& m_ContractName == ""
		&& m_ContractDate == "")
		return;
	m_tmLastTrans = tmTime;

	if(m_vecTimeScales.size() == 0)
		return;
	if(difftime(mktime(&m_vecTimeScales[0]), mktime(&tmTime)) > 0) 
		return;
	tmTime.tm_min -= 1;
	mktime( &tmTime);

	SetLastTransTime_DiffSpan(tmTime, m_vecKLineMin1, PHRASE_1MIN);		
	SetLastTransTime_DiffSpan(tmTime, m_vecKLineMin5, PHRASE_5MIN);		
	SetLastTransTime_DiffSpan(tmTime, m_vecKLineMin15, PHRASE_15MIN);				
	SetLastTransTime_DiffSpan(tmTime, m_vecKLineMin30, PHRASE_30MIN);		
	SetLastTransTime_DiffSpan(tmTime, m_vecKLineMin60, PHRASE_60MIN);				


	GetVectorCommon(m_enumPhrase);
}
void CFData::SetLastTransTime_DiffSpan(struct tm tmTime, vector<SKLine *>& vecKLineCommon, EnumPhrase enmuPhrase)
{
	if(vecKLineCommon.size() == 0)
	{	
		return;
		//	STLine *pTLine = new STLine;
		//	pTLine->fLastPrice = m_dbLastdbPrice;
		//	pTLine->dwVolume   = 0;
		//	pTLine->dwHoldVolume = 0;
		//	pTLine->dwVolumeMoney = 0;
		//	pTLine->dwTime = GetTimeInSpan(tmTime, 60);
		//	m_vecTLine.push_back(pTLine);	
		//	if(WRITELOG)
		//	{
		//		AppendLog(pTLine, wxT("add7"));
		//	}		
	}
	else
	{		
		SKLine* pLast = vecKLineCommon[0];
		int nTimeSpant = GetPhraseSpan(enmuPhrase);
		struct tm tTime = GetTimeInSpan(tmTime, nTimeSpant);

		if(tTime.tm_year ==pLast->dwTime.tm_year
			&& tTime.tm_mon == pLast->dwTime.tm_mon 
			&& tTime.tm_mday == pLast->dwTime.tm_mday
			&& tTime.tm_hour == pLast->dwTime.tm_hour
			&&tTime.tm_min == pLast->dwTime.tm_min)
		{
			return;
		}	
		int nTimeSpanTemp = difftime(mktime(&pLast->dwTime), mktime(&tTime));
		if(nTimeSpanTemp < 0)
		{
			SKLine *pKLine = new SKLine;			
			pKLine->fOpenPrice = pLast->fClosePrice;
			pKLine->fHighPrice = pLast->fClosePrice;
			pKLine->fLowPrice  = pLast->fClosePrice;
			pKLine->fClosePrice = pLast->fClosePrice;
			pKLine->dwVolume		 = 0;
			pKLine->dwHoldVolume	 = 0;
			pKLine->fSumTradeVolume  = 0;
			pKLine->dJieSuan		 = pLast->dJieSuan;
			pKLine->dwTime = tTime;
			vecKLineCommon.insert(vecKLineCommon.begin(), pKLine);	
			if(WRITELOG)
			{
				AppendLog(pKLine, wxT("add8"));
			}		
		}
		else
			return;
	}


}void CFData::ClearMemory()
{
	for(int i = 0; i< (int)m_vecKLineDay.size() ; i++)
	{
		SKLine *pKLine = m_vecKLineDay[i];
		if(pKLine)
		{
			delete pKLine;
			pKLine = NULL;
		}
	}
	m_vecKLineDay.clear();

	for(int i = 0; i< (int)m_vecKLineWeek.size() ; i++)
	{
		SKLine *pKLine = m_vecKLineWeek[i];
		if(pKLine)
		{
			delete pKLine;
			pKLine = NULL;
		}
	}
	m_vecKLineWeek.clear();

	for(int i = 0; i< (int)m_vecKLineMonth.size() ; i++)
	{
		SKLine *pKLine = m_vecKLineMonth[i];
		if(pKLine)
		{
			delete pKLine;
			pKLine = NULL;
		}
	}
	m_vecKLineMonth.clear();

	for(int i = 0; i< (int)m_vecKLineMin1.size() ; i++)
	{
		SKLine *pKLine = m_vecKLineMin1[i];
		if(pKLine)
		{
			delete pKLine;
			pKLine = NULL;
		}
	}
	m_vecKLineMin1.clear();

	for(int i = 0; i< (int)m_vecKLineMin5.size() ; i++)
	{
		SKLine *pKLine = m_vecKLineMin5[i];
		if(pKLine)
		{
			delete pKLine;
			pKLine = NULL;
		}
	}
	m_vecKLineMin5.clear();

	for(int i = 0; i< (int)m_vecKLineMin15.size() ; i++)
	{
		SKLine *pKLine = m_vecKLineMin15[i];
		if(pKLine)
		{
			delete pKLine;
			pKLine = NULL;
		}
	}
	m_vecKLineMin15.clear();

	for(int i = 0; i< (int)m_vecKLineMin30.size() ; i++)
	{
		SKLine *pKLine = m_vecKLineMin30[i];
		if(pKLine)
		{
			delete pKLine;
			pKLine = NULL;
		}
	}
	m_vecKLineMin30.clear();

	for(int i = 0; i< (int)m_vecKLineMin60.size() ; i++)
	{
		SKLine *pKLine = m_vecKLineMin60[i];
		if(pKLine)
		{
			delete pKLine;
			pKLine = NULL;
		}
	}
	m_vecKLineMin60.clear();


}
void CFData::SetSelMinsValue(EnumPhrase enumPhrase)
{
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{
			m_bSelMin1 = true;
		}
		break;
	case PHRASE_5MIN:
		{
			m_bSelMin5 = true;
		}
		break;
	case PHRASE_15MIN:
		{
			m_bSelMin15 = true;
		}
		break;
	case PHRASE_30MIN:
		{
			m_bSelMin30 = true;
		}
		break;
	case PHRASE_60MIN:
		{
			m_bSelMin60 = true;
		}
		break;
	case PHRASE_DAY:
		{
			m_bSelDay = true;
		}
		break;
	case PHRASE_WEEK:
		{
			m_bSelWeek = true;
		}
		break;
	case PHRASE_MONTH:
		{
			m_bSelMonth = true;
		}
		break;
	}

}
bool CFData::GetSelMinsValue(EnumPhrase enumPhrase)
{
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{
			return m_bSelMin1;
		}
		break;
	case PHRASE_5MIN:
		{
			return  m_bSelMin5;
		}
		break;
	case PHRASE_15MIN:
		{
			return  m_bSelMin15;
		}
		break;
	case PHRASE_30MIN:
		{
			return  m_bSelMin30;
		}
		break;
	case PHRASE_60MIN:
		{
			return m_bSelMin60;
		}
		break;
	case PHRASE_DAY:
		{
			return m_bSelDay;
		}
		break;
	case PHRASE_WEEK:
		{
			return m_bSelWeek;
		}
		break;
	case PHRASE_MONTH:
		{
			return m_bSelMonth;
		}
		break;
	}
	return false;
}
vector<SKLine *>* CFData::GetVectorCommon(EnumPhrase enumPhrase)
{
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{
			m_vecKLineCommon  =  &m_vecKLineMin1;
		}
		break;
	case PHRASE_5MIN:
		{
			m_vecKLineCommon  =  &m_vecKLineMin5;
		}
		break;
	case PHRASE_15MIN:
		{
			m_vecKLineCommon  =  &m_vecKLineMin15;
		}
		break;
	case PHRASE_30MIN:
		{
			m_vecKLineCommon  =  &m_vecKLineMin30;
		}
		break;
	case PHRASE_60MIN:
		{
			m_vecKLineCommon  =  &m_vecKLineMin60;
		}
		break;
	case PHRASE_DAY:
		{
			m_vecKLineCommon  =  &m_vecKLineDay;
		}
		break;
	case PHRASE_WEEK:
		{
			m_vecKLineCommon  = &m_vecKLineWeek;
		}
		break;
	case PHRASE_MONTH:
		{
			m_vecKLineCommon  = &m_vecKLineMonth;
		}
		break;
	}
	return m_vecKLineCommon;
}/*
bool CFData::SetFuture(string ExchangeName,string ContractName,string ContractDate, EnumPhrase	enumPhrase)
{	
	CAutoLock l(&m_CritSecVector);
	if(!m_bInitKLine && m_ExchangeName == ExchangeName
		&& m_ContractName == ContractName
		&& m_ContractDate == ContractDate)
		return false;

	m_bInitKLine = false;
	//这里还欠缺同步
	SaveFile();	
	SaveFileMarks();
	ClearMemory();

	if(m_ExchangeName != ExchangeName
		|| m_ContractName != ContractName
		|| m_ContractDate != ContractDate)
	{
		m_sKLineLast.dwTime.tm_year = 0;
		m_bSelDay	= false;
		m_bSelWeek	= false;
		m_bSelMonth	= false;
		m_bSelMin1	= false;
		m_bSelMin5	= false;
		m_bSelMin15	= false; 
		m_bSelMin30	= false;
		m_bSelMin60	= false;
	}

	m_ExchangeName = ExchangeName;
	m_ContractName = ContractName;
	m_ContractDate = ContractDate;
	m_enumPhrase   = enumPhrase;

	ReadFile();
	ClearMemory_Mark();
	ReadFileMarks();
	return true;
}*/

BOOL CFData::SetInTimeData(SKLine* pKLine)
{
	CAutoLock l(&m_CritSecVector);
	if(m_vecTimeScales.size() == 0)
	{
		if(pKLine)
		{	
			delete pKLine;
			pKLine = NULL;
		}

		return false;
	}

	BOOL bClear = FALSE;
	SetInTimeData_DiffSpan(pKLine);
	if(pKLine)
	{
		delete pKLine;
		pKLine = NULL;
	}
	return true;
}
BOOL CFData::SetInTimeData_DiffSpan(SKLine* pKLine)
{	
	BOOL bClear = FALSE;
	SKLine *pTemp = NULL;


	pTemp = new SKLine;
	*pTemp = *pKLine;

	//bClear = SetInTimeData_Min(m_vecKLineMin1, pTemp);
	//	if(m_bSelMin1)
	{
		bClear = SetInTimeData_MINS(m_vecKLineMin1, pTemp, 60);//5
		if(!bClear)
		{//用到这块内存，则需要新的内存，因为要表示不同的周期线
			pTemp = new SKLine;
			*pTemp = *pKLine;
		}
	}

	//	if(m_bSelMin5)
	{
		bClear = SetInTimeData_MINS(m_vecKLineMin5, pTemp, 300);//5
		if(!bClear)
		{
			pTemp = new SKLine;
			*pTemp = *pKLine;
		}
	}

	//if(m_bSelMin15)
	{
		bClear = SetInTimeData_MINS(m_vecKLineMin15, pTemp, 900);//15
		if(!bClear)
		{
			pTemp = new SKLine;
			*pTemp = *pKLine;
		}
	}


	//if(m_bSelMin30)
	{
		bClear = SetInTimeData_MINS(m_vecKLineMin30, pTemp, 1800);//30
		if(!bClear)
		{
			pTemp = new SKLine;
			*pTemp = *pKLine;
		}
	}

	//if(m_bSelMin60)
	{
		bClear = SetInTimeData_MINS(m_vecKLineMin60, pTemp, 3600);//60
		if(!bClear)
		{
			pTemp = new SKLine;
			*pTemp = *pKLine;
		}
	}

	//////////////////////////////////////////////////////////////////////////1149
	//日，周，月线不需要时,分,秒;统一处理
	pKLine->dwTime.tm_hour = 0;
	pKLine->dwTime.tm_min  = 0;
	pKLine->dwTime.tm_sec  = 0;

	pTemp->dwTime.tm_hour = 0;
	pTemp->dwTime.tm_min  = 0;
	pTemp->dwTime.tm_sec  = 0;
	//if(m_bSelDay)
	{
		bClear = SetInTimeData_Day(m_vecKLineDay, pTemp);
		if(!bClear)
		{
			pTemp = new SKLine;
			*pTemp = *pKLine;
		}
	}

	//if(m_bSelWeek)
	{
		bClear = SetInTimeData_Week(m_vecKLineWeek, pTemp);
		if(!bClear)
		{
			pTemp = new SKLine;
			*pTemp = *pKLine;
		}
	}

	//if(m_bSelMonth)
	{
		bClear = SetInTimeData_Month(m_vecKLineMonth, pTemp);
		if(bClear)
		{//最后一个没用到，可以删除
			delete pTemp;
			pTemp = NULL;
		}
	}

	m_sKLineLast = *pKLine;
	return bClear;
}
BOOL CFData::SetInTimeData_Day(vector<SKLine *>& vecKLinePara, SKLine* pKLine)
{
	bool bSameMin = false;
	if(vecKLinePara.size() == 0)
	{
		vecKLinePara.push_back(pKLine);
		if(WRITELOG)
		{
			wxString str;
			str= wxT("day add");
			AppendLog(pKLine, str);
		}
	}
	else
	{
		SKLine* pLast = vecKLinePara[0];		
		if(pLast->dwTime.tm_year == pKLine->dwTime.tm_year
			&& pLast->dwTime.tm_mon == pKLine->dwTime.tm_mon 
			&& pLast->dwTime.tm_mday == pKLine->dwTime.tm_mday)
		{
			if(WRITELOG)
			{
				wxString str;
				str= wxT("day orgin");
				AppendLog(pLast, str);
			}
			pLast->fClosePrice  = pKLine->fClosePrice;
			pLast->dwHoldVolume = pKLine->dwHoldVolume;
			pLast->dJieSuan     = pKLine->dJieSuan;
			pLast->dwVolume     = pKLine->dwVolume;
			if(pLast->fHighPrice < pKLine->fClosePrice  )
				pLast->fHighPrice	= pKLine->fClosePrice;
			if(pLast->fLowPrice > pKLine->fClosePrice)
				pLast->fLowPrice	= pKLine->fClosePrice;
			pLast->fOpenPrice	= pLast->fOpenPrice;
			pLast->fSumTradeVolume	= pLast->fSumTradeVolume;
			bSameMin = true;
			if(WRITELOG)
			{
				wxString str;
				str= wxT("day modify");
				AppendLog(pKLine, str);
			}

		}
		if(!bSameMin)
		{
			//	wxASSERT(0);
			vecKLinePara.insert(vecKLinePara.begin(), pKLine);	
			if(WRITELOG)
			{
				wxString str;
				str= wxT("day append");
				AppendLog(pKLine, str);
			}
		}
	}
	//	m_sKLineLast = *pKLine;
	return bSameMin;
}
BOOL CFData::SetInTimeData_Week(vector<SKLine *>& vecKLinePara, SKLine* pKLine)
{	
	bool bSameMin = false;
	if(vecKLinePara.size() == 0)
	{
		vecKLinePara.push_back(pKLine);
		if(WRITELOG)
		{
			wxString str;
			str= wxT("week add");
			AppendLog(pKLine, str);
		}
	}
	else
	{
		SKLine* pLast = vecKLinePara[0];	
		if(IsOneWeek(pLast->dwTime, pKLine->dwTime))
		{
			if(WRITELOG)
			{
				wxString str;
				str= wxT("week orgin");
				AppendLog(pLast, str);
			}
			pLast->dwTime = pKLine->dwTime;
			pLast->fClosePrice  = pKLine->fClosePrice;
			pLast->dwHoldVolume = pKLine->dwHoldVolume;
			pLast->dJieSuan     = pKLine->dJieSuan;
			if(pLast->fHighPrice < pKLine->fClosePrice  )
				pLast->fHighPrice	= pKLine->fClosePrice;
			if(pLast->fLowPrice > pKLine->fClosePrice)
				pLast->fLowPrice	= pKLine->fClosePrice;

			if(m_sKLineLast.dwTime.tm_year != 0)
			{
				pLast->dwVolume     += pKLine->dwVolume - m_sKLineLast.dwVolume;				
				pLast->fSumTradeVolume	+= pKLine->fSumTradeVolume - m_sKLineLast.fSumTradeVolume;
			}
			bSameMin = true;
			if(WRITELOG)
			{
				wxString str;
				str= wxT("week modify");
				AppendLog(pKLine, str);
			}
		}
		if(!bSameMin)
		{
			vecKLinePara.insert(vecKLinePara.begin(), pKLine);	
			if(WRITELOG)
			{
				wxString str;
				str= wxT("week append");
				AppendLog(pKLine, str);
			}
		}
	}
	return bSameMin;
}
BOOL CFData::SetInTimeData_Month(vector<SKLine *>& vecKLinePara, SKLine* pKLine)
{
	bool bSameMin = false;
	if(vecKLinePara.size() == 0)
	{		
		vecKLinePara.push_back(pKLine);
		if(WRITELOG)
		{
			wxString str;
			str= wxT("month add");
			AppendLog(pKLine, str);
		}
	}
	else
	{
		SKLine* pLast = vecKLinePara[0];	
		if(IsOneMonth(pLast->dwTime, pKLine->dwTime))
		{
			if(WRITELOG)
			{
				wxString str;
				str= wxT("month orgin");
				AppendLog(pLast, str);
			}
			pLast->dwTime = pKLine->dwTime;
			pLast->fClosePrice  = pKLine->fClosePrice;
			pLast->dwHoldVolume = pKLine->dwHoldVolume;
			pLast->dJieSuan     = pKLine->dJieSuan;
			if(pLast->fHighPrice < pKLine->fClosePrice  )
				pLast->fHighPrice	= pKLine->fClosePrice;
			if(pLast->fLowPrice > pKLine->fClosePrice)
				pLast->fLowPrice	= pKLine->fClosePrice;
			if(m_sKLineLast.dwTime.tm_year != 0)
			{
				pLast->dwVolume     += pKLine->dwVolume - m_sKLineLast.dwVolume;				
				pLast->fSumTradeVolume	+= pKLine->fSumTradeVolume - m_sKLineLast.fSumTradeVolume;
			}
			bSameMin = true;

			if(WRITELOG)
			{
				wxString str;
				str= wxT("month modify");
				AppendLog(pKLine, str);
			}
		}
		if(!bSameMin)
		{
			vecKLinePara.insert(vecKLinePara.begin(), pKLine);	
			if(WRITELOG)
			{
				wxString str;
				str= wxT("month append");
				AppendLog(pKLine, str);
			}
		}
	}

	return bSameMin;
}

BOOL CFData::SetInTimeData_MINS(vector<SKLine *>& vecKLinePara, SKLine* pKLine, int nTimeSpan)
{
	if(m_vecTimeScales.size() == 0)
		return false;
	SKLine ptemp = *pKLine;

	bool bSameMin = false;
	if(vecKLinePara.size() == 0)
	{
		vecKLinePara.push_back(pKLine);
		if(WRITELOG)
		{
			wxString str;
			str.Printf(wxT("%dadd "), nTimeSpan/60);
			AppendLog(pKLine, str);
		}
		struct tm tm1 = m_vecTimeScales[0];
		struct tm tm2 = pKLine->dwTime;
		if(difftime(mktime(&tm1), mktime(&tm2)) != 0)
		{
			pKLine->dwVolume		= 0;
			pKLine->fSumTradeVolume   = 0;
			pKLine->fOpenPrice      = pKLine->fClosePrice;
			pKLine->fHighPrice		= pKLine->fClosePrice;
			pKLine->fLowPrice       = pKLine->fClosePrice;
		}

		struct tm TM = GetTimeInSpan(pKLine->dwTime, nTimeSpan);//60分钟，3600秒
		pKLine->dwTime = TM;
		pKLine->dwTime.tm_sec   = 0;
		//wxASSERT(TM.tm_year != 0);
		if(WRITELOG)
		{
			wxString str;
			str.Printf(wxT("%dadd2 "), nTimeSpan/60);
			AppendLog(pKLine, str);
		}
	}
	else
	{

		SKLine* pLast = vecKLinePara[0];
		struct tm TM = GetTimeInSpan(pKLine->dwTime, nTimeSpan);//15:15:09 就可以转换为15:15
		if(pLast->dwTime.tm_year == TM.tm_year
			&& pLast->dwTime.tm_mon == TM.tm_mon 
			&& pLast->dwTime.tm_mday == TM.tm_mday)
		{//
			struct tm tm1 = pLast->dwTime;
			struct tm tm2 = TM;
			int dwTimeSpan = difftime(mktime(&tm1), mktime(&tm2));
			if(dwTimeSpan==0)
			{//同一分钟	
				bSameMin = true;
				if(WRITELOG)
				{
					wxString str;
					str.Printf(wxT("%d orgin"), nTimeSpan/60);
					AppendLog(pKLine, str);
				}
				pLast->fClosePrice  = pKLine->fClosePrice;
				pLast->dwHoldVolume = pKLine->dwHoldVolume;
				pLast->dJieSuan     = pKLine->dJieSuan;
				if(pLast->fHighPrice < pKLine->fClosePrice  )
					pLast->fHighPrice	= pKLine->fClosePrice;
				if(pLast->fLowPrice > pKLine->fClosePrice)
					pLast->fLowPrice	= pKLine->fClosePrice;
			
				if(m_sKLineLast.dwTime.tm_year != 0)
				{
					pLast->dwVolume     += pKLine->dwVolume - m_sKLineLast.dwVolume;				
					pLast->fSumTradeVolume	+= pKLine->fSumTradeVolume - m_sKLineLast.fSumTradeVolume;
				}
				
				if(WRITELOG)
				{
					wxString str;
					str.Printf(wxT("%dmodify"), nTimeSpan/60);
					AppendLog(pKLine, str);
				}
				return bSameMin;

			}
			else if(dwTimeSpan >0)
			{		
				SKLine* pLastFirst = vecKLinePara[vecKLinePara.size() - 1];
				if(pLastFirst && difftime(mktime(&pLastFirst->dwTime), mktime(&TM)) > 0)
				{//比第一个数据还要早，主要是正对模拟行情处理
					if(m_sTLineLast.dwTime.tm_year != 0)
					{
						pKLine->dwVolume  = pKLine->dwVolume - m_sKLineLast.dwVolume;
						pKLine->fSumTradeVolume = pKLine->fSumTradeVolume - m_sKLineLast.fSumTradeVolume;
					}
					else
					{
						pKLine->dwVolume		= 0;
						pKLine->fSumTradeVolume   = 0;
					}
					vecKLinePara.push_back(pKLine);					
				}
				else 
					{//实时行情加到目前数组的中间去
						vector<SKLine *>::iterator itLast = vecKLinePara.end();
						for(vector<SKLine *>::iterator it = vecKLinePara.begin(); it!= vecKLinePara.end(); it++)
						{
							SKLine* pLastTemp = *it;
							if(pLastTemp->dwTime.tm_year == TM.tm_year
								&& pLastTemp->dwTime.tm_mon == TM.tm_mon 
								&& pLastTemp->dwTime.tm_mday == TM.tm_mday
								&& pLastTemp->dwTime.tm_hour == TM.tm_hour
								&& pLastTemp->dwTime.tm_min == TM.tm_min)
							{//如果是按时间先加进去的则有可能出现这种情况
								bSameMin = true;

								pLastTemp->fClosePrice = pKLine->fClosePrice;
								if(pLastTemp->fHighPrice < pKLine->fClosePrice  )
									pLastTemp->fHighPrice	= pKLine->fClosePrice;
								if(pLastTemp->fLowPrice > pKLine->fClosePrice)
									pLastTemp->fLowPrice	= pKLine->fClosePrice;
								if(m_sKLineLast.dwTime.tm_year != 0)
								{
									pLastTemp->dwVolume  += pKLine->dwVolume - m_sKLineLast.dwVolume;
									pLastTemp->fSumTradeVolume += pKLine->fSumTradeVolume - m_sKLineLast.fSumTradeVolume;
								}						
								if(WRITELOG)
								{
									AppendLog(pKLine, wxT("modify2"));
								}
								break;
							}
							if(difftime(mktime(&pLastTemp->dwTime), mktime(&TM)) < 0)
							{		
								pKLine->dwTime = TM;
								pKLine->dwTime.tm_sec   = 0;
								if(m_sKLineLast.dwTime.tm_year != 0)
								{
									pKLine->dwVolume  = pKLine->dwVolume - m_sKLineLast.dwVolume;
									pKLine->fSumTradeVolume = pKLine->fSumTradeVolume - m_sKLineLast.fSumTradeVolume;
								}			
								vecKLinePara.insert(it, pKLine);
								if(WRITELOG)
								{
									AppendLog(pKLine, wxT("insert by2"));
								}
								break;
							}
							itLast = it;
						}	
					}
					return bSameMin;
				}
		}
		if(!bSameMin)
		{
			if(WRITELOG)
			{
				wxString str;
				str.Printf(wxT("%dAppend1"), nTimeSpan/60);
				AppendLog(pKLine, str);
			}
			if(m_sKLineLast.dwTime.tm_year != 0)
			{//
				pKLine->dwVolume     = pKLine->dwVolume - m_sKLineLast.dwVolume;				

				pKLine->fSumTradeVolume	= pKLine->fSumTradeVolume - m_sKLineLast.fSumTradeVolume;
				/* 廖说开盘价不用上一时间周期的收盘价修改之前;以后如果换成上一分钟的收盘价则应该把注解的打开，把后面的注掉就可以了
				pKLine->fOpenPrice      = m_sKLineLast.fClosePrice;
				pKLine->fHighPrice		= m_sKLineLast.fClosePrice;
				pKLine->fLowPrice       = m_sKLineLast.fClosePrice;
				*/
				pKLine->fOpenPrice      = pKLine->fClosePrice;
				pKLine->fHighPrice		= pKLine->fClosePrice;
				pKLine->fLowPrice       = pKLine->fClosePrice;
			}
			else
			{//刚开盘的第一笔
				struct tm tm1 = m_vecTimeScales[0];
				struct tm tm2 = pKLine->dwTime;
				if(difftime(mktime(&tm1), mktime(&tm2)) != 0)
				{
					pKLine->dwVolume		= 0;
					pKLine->fSumTradeVolume   = 0;
					pKLine->fOpenPrice      = pKLine->fClosePrice;
					pKLine->fHighPrice		= pKLine->fClosePrice;
					pKLine->fLowPrice       = pKLine->fClosePrice;					
				}
			}				

			pKLine->dwTime = TM;
			pKLine->dwTime.tm_sec   = 0;
			vecKLinePara.insert(vecKLinePara.begin(), pKLine);	
			if(WRITELOG)
			{
				wxString str;
				str.Printf(wxT("%dAppend2"), nTimeSpan/60);
				AppendLog(pKLine, str);
			}

			//wxASSERT(TM.tm_year != 0);

		}		
	}

	return bSameMin;
}
//////////////////////////////////////////////////////////////////////////
//SQIntime 相关
/*
性质的区分：
	开仓数大于平仓数，则属于开仓。  成交价格属于买一价或者比最新价低，则为空开，成交价格为卖一价或者比最新价高，则为多开。
	开仓数小于平仓数，则属于平仓。	成交价格属于买一价或者比最新价低，则为多平，成交价格为卖一价或者比最新价高，则为空平。
	开仓数等于平仓数，则属于换仓。	成交价格属于买一价或者比最新价低，则为空换，成交价格为卖一价或者比最新价高，则为多换。
	开仓数等于0，则为双平。
	平仓数等于0，则为双开。

仓差：开仓量减去平仓量    即对持仓量的即时性增减。

颜色说明：成交价为买一价时，显示为绿色  代表价格下降。
		  成交价为卖一价时，显示为红色，代表价格上升。//这两点简单处理就是针对上一笔价格的上涨还是下跌判定颜色，上涨为红色，下跌为绿色
	 成交价为最新价时:
		  颜色首先根据价格判断，影响量和性质。如果价格不变，则根据性质判断。
		  多换（红色）       空换（绿色）
		  双开（红色）       双平（绿色）
		  空平（红色）       多平（绿色）
		  多开（红色）       空开（绿色）

*/
bool CFData::SetInTimeData_QIntime(PlatformStru_DepthMarketData& MarketDataInfo)
{
	CAutoLock l(&m_CritSecVector_QIntime);
	if(!m_bInitQIntimeFirst)
	{
		m_bInitQIntimeFirst = true;
		m_MarketDataInfoLast = MarketDataInfo;
		return true;
	}
	if(MarketDataInfo.Volume == m_MarketDataInfoLast.Volume)
	{//量没有增加，视为没有新的成交，此行情信息抛弃
		m_MarketDataInfoLast = MarketDataInfo;
		return true;
	}
	SQIntime *pQIntime = new SQIntime;

	wxString strDate = m_strTradingDay;
	wxString strTime = MarketDataInfo.UpdateTime;
	pQIntime->dwTime.tm_year	= atoi(strDate.Left(4))-1900;
	pQIntime->dwTime.tm_mon	= atoi(strDate.Mid(4,2))-1;
	pQIntime->dwTime.tm_mday	= atoi(strDate.Right(2));
	
	pQIntime->dwTime.tm_hour	= atoi(strTime.Left(2));
	pQIntime->dwTime.tm_min	= atoi(strTime.Mid(3, 2));
	pQIntime->dwTime.tm_sec	= atoi(strTime.Mid(6, 2));;
	pQIntime->dwTime.tm_wday	= 0;
	pQIntime->dwTime.tm_yday	= 0;

	pQIntime->fLastPrice = MarketDataInfo.LastPrice;
	pQIntime->dwVolume = MarketDataInfo.Volume - m_MarketDataInfoLast.Volume;
	
	pQIntime->dwOpenVolume  = (pQIntime->dwVolume + MarketDataInfo.OpenInterest - m_MarketDataInfoLast.OpenInterest)/2   + 0.5;
	pQIntime->dwCloseVolume =  (pQIntime->dwVolume -(MarketDataInfo.OpenInterest - m_MarketDataInfoLast.OpenInterest))/2 + 0.5;

	if(pQIntime->dwOpenVolume == 0)
		pQIntime->nDesc   = 6;//双平
	else if(pQIntime->dwCloseVolume == 0)
		pQIntime->nDesc   = 7;//双开
	else if(pQIntime->dwOpenVolume > pQIntime->dwCloseVolume)
	{//开仓数大于平仓数，则属于开仓。  成交价格<买一价 或者小于上一笔最新价，则为空开，成交价格>卖一价或者>上一笔最新价，则为多开。
		if(IsSameAsPrice(MarketDataInfo.LastPrice, m_MarketDataInfoLast.BidPrice1, true))		
		    pQIntime->nDesc   = 0;//	pQIntime->strDesc = "空开";
		else if(IsSameAsPrice(MarketDataInfo.LastPrice,m_MarketDataInfoLast.AskPrice1, false))			
			pQIntime->nDesc   = 1;//pQIntime->strDesc = "多开";
		else if(IsSameAsPrice(MarketDataInfo.LastPrice, m_MarketDataInfoLast.LastPrice, true))
			 pQIntime->nDesc   = 0;
		else if(IsSameAsPrice(MarketDataInfo.LastPrice,m_MarketDataInfoLast.LastPrice, false))
			 pQIntime->nDesc   = 1;
	}
	else if(pQIntime->dwOpenVolume < pQIntime->dwCloseVolume)
	{//开仓数小于平仓数，则属于平仓。	成交价格<买一价或者小于上一笔最新价，则为多平，成交价格>卖一价或者>上一笔最新价，则为空平。
		if(IsSameAsPrice(MarketDataInfo.LastPrice, m_MarketDataInfoLast.BidPrice1, true))
			pQIntime->nDesc   = 2;//pQIntime->strDesc = "多平";			
		else if(IsSameAsPrice(MarketDataInfo.LastPrice,m_MarketDataInfoLast.AskPrice1, false) )
			pQIntime->nDesc   = 3;//pQIntime->strDesc = "空平";
		else if(IsSameAsPrice(MarketDataInfo.LastPrice, m_MarketDataInfoLast.LastPrice, true))
			pQIntime->nDesc   = 2;//pQIntime->strDesc = "多平";			
		else if(IsSameAsPrice(MarketDataInfo.LastPrice,m_MarketDataInfoLast.LastPrice, false))
			pQIntime->nDesc   = 3;//pQIntime->strDesc = "空平";
			
	}
	else if(pQIntime->dwOpenVolume == pQIntime->dwCloseVolume)
	{//开仓数等于平仓数，则属于换仓。	成交价格<买一价或者小于上一笔最新价，则为空换，成交价格>卖一价或者>上一笔最新价，则为多换。
		if(IsSameAsPrice(MarketDataInfo.LastPrice, m_MarketDataInfoLast.BidPrice1, true))
			pQIntime->nDesc   = 5;//pQIntime->strDesc = "空换";			
		else if(IsSameAsPrice(MarketDataInfo.LastPrice, m_MarketDataInfoLast.AskPrice1, false))
			pQIntime->nDesc   = 4;//pQIntime->strDesc = "多换";
		else if(IsSameAsPrice(MarketDataInfo.LastPrice, m_MarketDataInfoLast.LastPrice, true) )
			pQIntime->nDesc   = 5;//pQIntime->strDesc = "空换";		
		else if(IsSameAsPrice(MarketDataInfo.LastPrice, m_MarketDataInfoLast.LastPrice, false) )
			pQIntime->nDesc   = 4;//pQIntime->strDesc = "多换";
			
	}

	wxString strDate2 = m_strTradingDay;
	wxString strTime2 = MarketDataInfo.UpdateTime;//差了一秒的问题
	struct tm tmTime2;
	tmTime2.tm_year	= atoi(strDate2.Left(4))-1900;
	tmTime2.tm_mon	= atoi(strDate2.Mid(4,2))-1;
	tmTime2.tm_mday	= atoi(strDate2.Right(2));
	tmTime2.tm_hour	= atoi(strTime2.Left(2));
	tmTime2.tm_min	= atoi(strTime2.Mid(3, 2));
	tmTime2.tm_sec	= atoi(strTime2.Mid(6, 2));;
	tmTime2.tm_wday	= 0;
	tmTime2.tm_yday	= 0;
/*	if(difftime(mktime(&pQIntime->dwTime), mktime(&tmTime2))==0  && m_vecQIntime.size()>0)
	{
		SQIntime *pQIntime2 = m_vecQIntime[m_vecQIntime.size()-1];
		pQIntime2->fLastPrice = MarketDataInfo.LastPrice;
		pQIntime2->dwVolume = pQIntime2->dwVolume + pQIntime->dwVolume;
		pQIntime2->dwOpenVolume  = pQIntime2->dwOpenVolume + pQIntime->dwOpenVolume;
		pQIntime2->dwCloseVolume = pQIntime2->dwCloseVolume + pQIntime->dwCloseVolume; 
		delete pQIntime;
	}
	else*/
		m_vecQIntime.push_back(pQIntime);
		
	if(IsSameAsPrice(MarketDataInfo.LastPrice, m_MarketDataInfoLast.AskPrice1, false))
		m_nWaiPan += MarketDataInfo.Volume - m_MarketDataInfoLast.Volume;
	
	if(IsSameAsPrice(MarketDataInfo.LastPrice, m_MarketDataInfoLast.BidPrice1, true))
		m_nNeiPan += MarketDataInfo.Volume - m_MarketDataInfoLast.Volume;

	m_MarketDataInfoLast = MarketDataInfo;
	return true;
}
bool CFData::IsSameAsPrice(double dbPrice, double dbOrgPrice, bool bBid)
{
	if(dbPrice - dbOrgPrice > -0.00001 && dbPrice - dbOrgPrice < 0.00001)
		return true;

	if(bBid)
	{//成交价格低于买一价；
		if(dbPrice <= dbOrgPrice)
		return true;
	}
	if(!bBid)
	{//成交价格高于卖一价；
		if(dbPrice >= dbOrgPrice)
			return true;
	}
	return false;
}
bool CFData::SaveFile_QIntime()
{	
	CAutoLock l(&m_CritSecVector_QIntime);
	if(m_ExchangeName == "")
		return false;
	if(m_vecQIntime.size() ==0)
		return false;

	wxString strPath;
	GetFileSavePath_TLine(strPath);	
	wxString strDir;
	strDir.Printf(_T("%shqData\\"), strPath);
	CreateDirectory(strDir, NULL);
	wxString strSubDir;
	if(m_Simulate == 1)
		strSubDir.Printf(_T("%shqData_simi\\"), strDir);
	else
		strSubDir.Printf(_T("%shqData_Trans\\"), strDir);	
	CreateDirectory(strSubDir, NULL);
	strPath.Printf(_T("%s%s\\"), strSubDir, m_ExchangeName.c_str());
	CreateDirectory(strPath.c_str(), NULL);
	strPath.Append(wxT("QInTime\\"));
	CreateDirectory(strPath.c_str(), NULL);

	wxString strFileName;
	wxString strFileName2;
	strFileName2.Printf(_T("%s_%s_%s"), m_ExchangeName.c_str(), m_ContractName.c_str(), m_ContractDate.c_str());
	strFileName.Printf(_T("%s\\%s_QIntime%s"), strPath.c_str(), strFileName2.c_str(), m_strTradingDay.c_str());

	if(m_vecQIntime.size() ==0)
		return false;

	vector<SQIntime_IO> vecQIntime_IO;
	for(int i=0; i< (int)m_vecQIntime.size(); i++)
	{
		SQIntime p = *(m_vecQIntime[i]);
		if(p.dwTime.tm_hour == 13 && p.dwTime.tm_min == 10)
		{
			int i =0;
		}
		SQIntime_IO temp;
		temp.dwTime = mktime(& p.dwTime);
		temp.dwCloseVolume = p.dwCloseVolume;
		temp.dwOpenVolume  = p.dwOpenVolume;
		temp.dwVolume      = p.dwVolume;
		temp.fLastPrice    = p.fLastPrice;
		temp.nDesc         = p.nDesc;
		vecQIntime_IO.push_back(temp);
	}
	FILE* fp = fopen( strFileName, "wb" );
	if(fp  == NULL )
		return false;
	int nCount = vecQIntime_IO.size();
	if(nCount != 0)
	{
		fwrite(&nCount, sizeof(int), 1, fp);
		fwrite( &vecQIntime_IO[0], sizeof(SQIntime_IO) , vecQIntime_IO.size(), fp );
	}
	fclose(fp);
	
//////////////////////////////////////////////////////////////////////////
//以下代码为解决bug用的
	fp = fopen( strFileName, "rb" );
	if(fp  == NULL )
		return false;
	int nCount1 =0;
	fread( &nCount1, sizeof(int) , 1 , fp );
	if(nCount1 <= 0)
	{
		fclose(fp);
		return true;
	}
	vector<SQIntime_IO> vecQIntime_IO1(nCount1);
	fread( &vecQIntime_IO1[0], sizeof(SQIntime_IO) , nCount1, fp );
	fclose(fp);

	vector<SQIntime *> vecQIntime;
	for(int i=0; i < (int)vecQIntime_IO1.size(); i++)
	{
		SQIntime_IO kIO = vecQIntime_IO1[i];

		SQIntime *pQIntime = new SQIntime;;
		time_t t = time_t(kIO.dwTime);
		pQIntime->dwTime = *localtime(&t);
		pQIntime->dwCloseVolume = kIO.dwCloseVolume;
		pQIntime->dwOpenVolume  = kIO.dwOpenVolume;
		pQIntime->dwVolume      = kIO.dwVolume;
		pQIntime->fLastPrice    = kIO.fLastPrice;
		pQIntime->nDesc       = kIO.nDesc;
		
		vecQIntime.push_back(pQIntime);
	}



/*

	int nDayCount = 0;
	int nLastDay  = 0;
	ofstream outfile("c:\\1eee.txt" ,ios::out|ios::binary);
	
	struct tm tmLastDay;
	if(m_vecQIntime.size()>0)
	{
		SQIntime *p = m_vecQIntime[m_vecQIntime.size()-1];
		tmLastDay = p->dwTime;//只保存最后一天的数据
	}
	for(int i=0; i<m_vecQIntime.size(); i++)
	{  
		SQIntime *p = m_vecQIntime[i];
		if((tmLastDay.tm_year == p->dwTime.tm_year)
			&& (tmLastDay.tm_mon == p->dwTime.tm_mon)
			&& (tmLastDay.tm_mday == p->dwTime.tm_mday))
		{			
			outfile << m_vecQIntime[i];
		}		
	}
	outfile.close();	*/
	return true;
	
}
bool CFData::ReadFile_QIntime()
{
	CAutoLock l(&m_CritSecVector_QIntime);
	if(m_vecTimeScales.size() ==0)
		return false;
	wxString strPath;
	GetFileSavePath_TLine(strPath);	
	wxString strDir;
	strDir.Printf(_T("%shqData\\"), strPath);
	wxString strSubDir;
	if(m_Simulate == 1)
		strSubDir.Printf(_T("%shqData_simi\\"), strDir);
	else
		strSubDir.Printf(_T("%shqData_Trans\\"), strDir);	
	CreateDirectory(strSubDir, NULL);
	strPath.Printf(_T("%s%s\\"), strSubDir, m_ExchangeName.c_str());
	strPath.Append(wxT("QInTime\\"));
	
	wxString strFileName;
	wxString strFileName2;
	strFileName2.Printf(_T("%s_%s_%s"), m_ExchangeName.c_str(), m_ContractName.c_str(), m_ContractDate.c_str());
	strFileName.Printf(_T("%s\\%s_QIntime%s"), strPath.c_str(), strFileName2.c_str(), m_strTradingDay.c_str());
	
	FILE* fp = fopen( strFileName, "rb" );
	if(fp  == NULL )
		return false;
	int nCount =0;
	fread( &nCount, sizeof(int) , 1 , fp );
	if(nCount <= 0)
	{
		fclose(fp);
		return true;
	}
	vector<SQIntime_IO> vecQIntime_IO(nCount);
	fread( &vecQIntime_IO[0], sizeof(SQIntime_IO) , nCount, fp );
	fclose(fp);
	
	vector<SQIntime *> vecQIntime;
	for(int i=0; i < (int)vecQIntime_IO.size(); i++)
	{
		SQIntime_IO kIO = vecQIntime_IO[i];
		
		SQIntime *pQIntime = new SQIntime;;
		time_t t = time_t(kIO.dwTime);
		pQIntime->dwTime = *localtime(&t);
		pQIntime->dwCloseVolume = kIO.dwCloseVolume;
		pQIntime->dwOpenVolume  = kIO.dwOpenVolume;
		pQIntime->dwVolume      = kIO.dwVolume;
		pQIntime->fLastPrice    = kIO.fLastPrice;
		pQIntime->nDesc       = kIO.nDesc;
		
		vecQIntime.push_back(pQIntime);
	}
	vecQIntime_IO.clear();
//读取文件完毕
//////////////////////////////////////////////////////////////////////////
//结合到实时行情里面去
	int nSize = m_vecQIntime.size();
	SQIntime *pFirst = NULL;
	vector<SQIntime *>::iterator itBegin;
	if(nSize != 0)
	{
		pFirst = m_vecQIntime[nSize-1];
		itBegin = m_vecQIntime.end() - nSize;
	}

	for(int i=0; i< (int)vecQIntime.size(); i++)
	{
		SQIntime *p = vecQIntime[i];
		if(pFirst == NULL)
			m_vecQIntime.push_back(p);
		else
		{
			struct tm tm1 = p->dwTime;
			struct tm tm2 = pFirst->dwTime;
			if(difftime(mktime(&tm1), mktime(&tm2))< 0)
			{
				m_vecQIntime.insert(itBegin, p);
				itBegin = m_vecQIntime.end() - nSize;
			}
		}	
	}
	vecQIntime.clear();
/*	int nSize = m_vecQIntime.size();
	SQIntime *pFirst = NULL;
	vector<SQIntime *>::iterator itBegin;
	if(nSize != 0)
	{
		pFirst = m_vecQIntime[nSize-1];
		itBegin = m_vecQIntime.end() - nSize;
	}
	ifstream infile(strFileName,ios::in|ios::binary);  	
	if (infile.peek() == EOF)
		return false;

	while(!infile.eof())
	{
		if (infile.peek() == EOF)
			break;
		SQIntime *p = new SQIntime;
		infile >> p;
		if(pFirst == NULL)
			m_vecQIntime.push_back(p);
		else
		{
			struct tm tm1 = p->dwTime;
			struct tm tm2 = pFirst->dwTime;
			if(difftime(mktime(&tm1), mktime(&tm2))< 0)
			{
				m_vecQIntime.insert(itBegin, p);
				itBegin = m_vecQIntime.end() - nSize;
			}
		}
	}
	infile.close();	
	*/
	return true;
}
void CFData::ClearMemory_QIntime()
{
	CAutoLock l(&m_CritSecVector_QIntime);
	for(int i = 0; i< (int)m_vecQIntime.size() ; i++)
	{
		SQIntime *pQIntime= m_vecQIntime[i];
		if(pQIntime)
		{
			delete pQIntime;
			pQIntime = NULL;
		}
	}
	m_vecQIntime.clear();
}
bool CFData::SetFuture_QIntime()
{
	CAutoLock l(&m_CritSecVector_QIntime);
	if(!m_bInitQIntime)
		return false;

	m_bInitQIntime = false;
	ReadFile_QIntime();
	return true;	
}
void  CFData::GetNeiWaiPan(int& nNeiPan, int& nWaiPan)
{
	CAutoLock l(&m_CritSecVector_QIntime);
	nNeiPan = m_nNeiPan;
	nWaiPan = m_nWaiPan;
}
bool CFData::DirExist(const char *pszDirName) 
{ 
	WIN32_FIND_DATA fileinfo; 
	char _szDir[_MAX_PATH]; 
	strcpy(_szDir, pszDirName); 
	int nLen = strlen(_szDir); 
	if( (_szDir[nLen-1] == '\\') || (_szDir[nLen-1] == '/') ) 
	{ 
		_szDir[nLen-1] = '\0'; 
	} 
	HANDLE hFind = ::FindFirstFile(_szDir, &fileinfo); 

	if (hFind == INVALID_HANDLE_VALUE) 
	{ 
		return false; 
	} 
	if( fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY ) 
	{ 
		::FindClose(hFind); 
		return true; 
	} 
	::FindClose(hFind); 
	return false; 
} 
bool CFData::CreateDir(const char *pszDirName) 
{ 
	bool bRet = false; 
	char _szDir[_MAX_PATH]; 
	char _szTmp[_MAX_DIR]; 
	int nLen = 0; 
	int idx ; 
	if( (DirExist(pszDirName)) == true ) 
		return true; 
	strcpy(_szDir, pszDirName); 
	nLen = strlen(_szDir); 
	if( _szDir[nLen-1] == '\\' || _szDir[nLen-1] == '/' ) 
	{ 
		_szDir[nLen-1] = '\0'; 
	} 
	nLen = strlen(_szDir); 
	memset(_szTmp, 0, _MAX_DIR); 
	char _str[2]; 
	for(idx = 0; idx < nLen; idx++) 
	{ 
		if(_szDir[idx] != '\\' ) 
		{ 
			_str[0] = _szDir[idx]; 
			_str[1] = 0; 
			strcat(_szTmp, _str); 
		} 
		else 
		{ 
			bRet = !!::CreateDirectory(_szTmp, NULL); 
			if( bRet ) 
			{ 
				::SetFileAttributes(_szTmp, FILE_ATTRIBUTE_NORMAL); 
			} 
			_str[0] = _szDir[idx]; 
			_str[1] = 0; 
			strcat(_szTmp, _str); 
		} 
		if( idx == nLen-1 ) 
		{ 
			bRet = !!::CreateDirectory(_szTmp, NULL); //没有WARING 加两个感叹号
			if( bRet ) 
			{ 
				::SetFileAttributes(_szTmp, FILE_ATTRIBUTE_NORMAL); 
			} 
		} 
	} 
	if( DirExist(_szTmp) ) 
		return true; 
	return false; 
}  

void CFData::SetDataInTime(PlatformStru_DepthMarketData* pData, int nTrace)
{//实时行情受到后，在这个函数里面处理
	if(pData == NULL)
		return;	
	
	wxString strDate = m_strTradingDay;//如果行情里面没有日期，照样不能解决跨日期的问题
	wxString strTime		= pData->UpdateTime;
	wxString strFutureID = pData->InstrumentID;
	if(isInvalidValue(pData->LastPrice) 
		||isInvalidValue(pData->OpenPrice)
		||isInvalidValue(pData->HighestPrice)
		||isInvalidValue(pData->LowestPrice)
		||isInvalidValue(pData->Volume))
	{
	//	if(nTrace)
	//		AppendLog_DepthMarketData(pData);
		return;//无效值
	}	

	if(isInvalidValue(pData->AskPrice1))
		pData->AskPrice1 = 0;
	if(isInvalidValue(pData->BidPrice1))
		pData->BidPrice1 = 0;
//	if(nTrace)
//		AppendLog_DepthMarketData(pData);

	if(strDate.length() != 8)
	{
		wxString str;
		return;
	}
	m_fMaxValue = pData->HighestPrice;
	m_fMinValue = pData->LowestPrice;
	STLine *pLine = new STLine;
	pLine->dwTime.tm_year	= atoi(strDate.Left(4))-1900;
	pLine->dwTime.tm_mon	= atoi(strDate.Mid(4,2))-1;
	pLine->dwTime.tm_mday	= atoi(strDate.Right(2));
	
	pLine->dwTime.tm_hour	= atoi(strTime.Left(2));
	pLine->dwTime.tm_min	= atoi(strTime.Mid(3, 2));
	pLine->dwTime.tm_sec	= atoi(strTime.Mid(6, 2));;
	pLine->dwTime.tm_wday	= 0;
	pLine->dwTime.tm_yday	= 0;
	pLine->fLastPrice = pData->LastPrice;
	pLine->dwVolume	  = pData->Volume;	
	pLine->dwVolumeMoney	  = pData->Turnover;
	pLine->dwHoldVolume	      = pData->OpenInterest;

	double dcClose = pData->PreSettlementPrice;
	if(dcClose <0.00001)
		dcClose = pData->OpenPrice;//如果没有昨结算，则用今日开盘价
	pLine->dwTime.tm_wday	= 0;
	pLine->dwTime.tm_yday	= 0;
	
	SetLastClosePrice(dcClose);
	SetInTimeData_TLine(pLine);	

	SKLine *pKLine = new SKLine;
	pKLine->dwTime.tm_year	= atoi(strDate.Left(4))-1900;
	pKLine->dwTime.tm_mon	= atoi(strDate.Mid(4,2))-1;
	pKLine->dwTime.tm_mday	= atoi(strDate.Right(2));
	pKLine->dwTime.tm_hour	= atoi(strTime.Left(2));
	pKLine->dwTime.tm_min	= atoi(strTime.Mid(3, 2));
	pKLine->dwTime.tm_sec	= atoi(strTime.Mid(6, 2));;
	pKLine->dwTime.tm_wday	= 0;
	pKLine->dwTime.tm_yday	= 0;
	pKLine->dwHoldVolume    = pData->OpenInterest;
	pKLine->dwVolume        = pData->Volume;
	pKLine->fClosePrice     = pData->LastPrice;
	pKLine->fHighPrice      = pData->HighestPrice;
	pKLine->fLowPrice		= pData->LowestPrice;
	pKLine->fOpenPrice      = pData->OpenPrice;
	pKLine->fSumTradeVolume	= pData->Turnover;
	
	SetInTimeData(pKLine);		
	SetInTimeData_QIntime(*pData);

}
