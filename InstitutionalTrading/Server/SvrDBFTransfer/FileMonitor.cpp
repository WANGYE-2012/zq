#include "StdAfx.h"
#include "FileMonitor.h"
#include "digittools.h"
#include "FileOpr.h"
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","FileMonitor", fmt, __VA_ARGS__);


CThostFtdcMdSpi *CFileMonitor::m_pThostFtdcMdSpi=NULL;
void CFileMonitor::CopyDepthMarketData( const PlatformStru_DepthMarketData& data, CThostFtdcDepthMarketDataField& depthMarketData )
{
	memset(&depthMarketData,0,sizeof(CThostFtdcDepthMarketDataField));

	strcpy(depthMarketData.TradingDay, data.TradingDay);
	strcpy(depthMarketData.InstrumentID, data.ExchangeID);
	strcat(depthMarketData.InstrumentID, data.InstrumentID);
	strcpy(depthMarketData.ExchangeID, data.ExchangeID);
	strcpy(depthMarketData.ExchangeInstID, data.ExchangeInstID);
	depthMarketData.LastPrice = data.LastPrice;
	depthMarketData.PreSettlementPrice = data.PreSettlementPrice;
	depthMarketData.PreClosePrice = data.PreClosePrice;
	depthMarketData.PreOpenInterest = data.PreOpenInterest;
	depthMarketData.OpenPrice = data.OpenPrice;
	depthMarketData.HighestPrice = data.HighestPrice;
	depthMarketData.LowestPrice = data.LowestPrice;
	depthMarketData.Volume = data.Volume;
	depthMarketData.Turnover = data.Turnover;
	depthMarketData.OpenInterest = data.OpenInterest;
	depthMarketData.ClosePrice = data.ClosePrice;
	depthMarketData.SettlementPrice = data.SettlementPrice;
	depthMarketData.UpperLimitPrice = data.UpperLimitPrice;
	depthMarketData.LowerLimitPrice = data.LowerLimitPrice;
	depthMarketData.PreDelta = data.PreDelta;
	depthMarketData.CurrDelta = data.CurrDelta;
	strcpy(depthMarketData.UpdateTime, data.UpdateTime);
	depthMarketData.UpdateMillisec = data.UpdateMillisec;
	depthMarketData.BidPrice1 = data.BidPrice1;
	depthMarketData.BidVolume1 = data.BidVolume1;
	depthMarketData.AskPrice1 = data.AskPrice1;
	depthMarketData.AskVolume1 = data.AskVolume1;
	depthMarketData.BidPrice2 = data.BidPrice2;
	depthMarketData.BidVolume2 = data.BidVolume2;
	depthMarketData.AskPrice2 = data.AskPrice2;
	depthMarketData.AskVolume2 = data.AskVolume2;
	depthMarketData.BidPrice3 = data.BidPrice3;
	depthMarketData.BidVolume3 = data.BidVolume3;
	depthMarketData.AskPrice3 = data.AskPrice3;
	depthMarketData.AskVolume3 = data.AskVolume3;
	depthMarketData.BidPrice4 = data.BidPrice4;
	depthMarketData.BidVolume4 = data.BidVolume4;
	depthMarketData.AskPrice4 = data.AskPrice4;
	depthMarketData.AskVolume4 = data.AskVolume4;
	depthMarketData.BidPrice5 = data.BidPrice5;
	depthMarketData.BidVolume5 = data.BidVolume5;
	depthMarketData.AskPrice5 = data.AskPrice5;
	depthMarketData.AskVolume5 = data.AskVolume5;
	depthMarketData.AveragePrice = data.AveragePrice;
}


//////////////////////////////////////////////////////////////////////////
DWORD ThreadSendQuotProcess(void* lpParam)
{
	CFileMonitor* pInst = (CFileMonitor*)lpParam;

	CThostFtdcDepthMarketDataField depthMarketData;
	vector<PlatformStru_DepthMarketData> sVec;
	while (!pInst->m_bQuitSendQuot)
	{
		if(CFileMonitor::m_pThostFtdcMdSpi==NULL)
		{
			Sleep(30);
			continue;
		}

		pInst->GetQueueQuot(sVec);
		for (vector<PlatformStru_DepthMarketData>::iterator it = sVec.begin();it!=sVec.end();it++)
		{            
			if(WaitForSingleObject(pInst->m_hQuitSendQuotEvent,0)==WAIT_OBJECT_0)
			{
				printf("�˳�����DBF�����߳�");
				return 1;
			}

			PlatformStru_DepthMarketData data = (*it);

			printf("����DBFʵʱ���� ExchangeID=%s InstrumentID=%s\n",data.ExchangeID,data.InstrumentID);
			
			if(CFileMonitor::m_pThostFtdcMdSpi)
			{
				CFileMonitor::CopyDepthMarketData(data,depthMarketData);
				CFileMonitor::m_pThostFtdcMdSpi->OnRtnDepthMarketData(&depthMarketData);
			}
		}

		sVec.clear();

		Sleep(200);
	}

	return 1;
}


CFileMonitor::CFileMonitor()
{
    m_hDir        =  NULL;
    m_bContinue   =  FALSE;
    m_hThread     =  NULL;
}

CFileMonitor::~CFileMonitor()
{

}

void CFileMonitor::Init()
{
	m_strTimeOffShow2003  =  m_strTimeOffSJSHQ = " ";

	m_bQuitSendQuot = false;
	m_hQuitSendQuotEvent = NULL;
	m_hThreadSendQuot = NULL;
	m_idThreadSendQuot = 0;
	//m_hThreadSendQuot=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadSendQuotProcess,this,0,&m_idThreadSendQuot);	
}

void CFileMonitor::Release()
{
	SetEvent(m_hQuitSendQuotEvent);
	Sleep(1000);

	DWORD ExitCode;
	WaitForSingleObject((HANDLE)m_hThreadSendQuot,8000);
	if(GetExitCodeThread((HANDLE)m_hThreadSendQuot,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
		TerminateThread((HANDLE)m_hThreadSendQuot,0);
	CloseHandle(m_hThreadSendQuot);

	m_hThreadSendQuot=NULL;
	m_idThreadSendQuot=0;

	CloseHandle(m_hQuitSendQuotEvent);
	m_hQuitSendQuotEvent = NULL;
}

/*
* ���ûص�����
*/
void  CFileMonitor::SetDealFilePtr( lpFunDealFile pFunDeal )
{
    ASSERT( pFunDeal != NULL );
    m_pFunDeal  =  pFunDeal;
}

BOOL  CFileMonitor::StartMonitor( LPCTSTR lpszDir )
{
    ASSERT( m_hThread == NULL );

    HANDLE  hDir =  ::CreateFile( lpszDir, GENERIC_READ|FILE_LIST_DIRECTORY,
        FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL, 
        OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS,NULL);
    if( INVALID_HANDLE_VALUE == hDir ) 
        return FALSE;
    this->m_hDir     =  hDir;

    m_bContinue  =  TRUE;
    m_hThread     =  ::CreateThread( NULL, 0, MonitorProc, this, 0, NULL );
    return ( NULL == m_hThread )?FALSE:TRUE;
}

void   CFileMonitor::EndMonitor()
{
    ASSERT( m_hThread != NULL );
    m_bContinue   =  FALSE;
    DWORD  dwRet = ::WaitForSingleObject( m_hThread, 1000 );
    if( WAIT_TIMEOUT == dwRet )
    {
        ASSERT( m_hThread != NULL );
        ::TerminateThread( m_hThread, -1 );
    }

    ::CloseHandle( m_hDir );
    m_hDir   =  NULL;
    m_hThread  =  NULL;

}

BOOL  CFileMonitor::IsMoniting()
{
    return m_bContinue;
}

void CFileMonitor::GetQueueQuot(std::vector<PlatformStru_DepthMarketData>& vecData)
{
	CeasymutexGuard guard(m_mutex);

	while (!m_queueQuot.empty())
	{                
		PlatformStru_DepthMarketData targetData = m_queueQuot.front();
		m_queueQuot.pop();

		vecData.push_back(targetData);
	} 
}

string CFileMonitor::Parase(int nCol, const COleVariant& varValue, bool bShow2003/*=true*/)
{
    char tempFormat[64],tempDest[128];
    string strText(" ");
    switch(varValue.vt)
    {
    case VT_BSTR:
        {
            strText = (char*)(_bstr_t)varValue.bstrVal ; 
            break;
        }
    case VT_R8:
        {
            memset(tempDest,0,sizeof(tempDest));
            memset(tempFormat,0,sizeof(tempFormat));
            sprintf(tempFormat,"%s.%uf","%",bShow2003?(m_SetShow2003.m_DBFFields[nCol].m_nDec):(m_SetSJSHQ.m_DBFFields[nCol].m_nDec));
            sprintf(tempDest,tempFormat,varValue.dblVal); 
            strText = tempDest;
            break;
        }
    case VT_BOOL:
        {
            if(varValue.boolVal)
                strText = "TRUE";
            else
                strText = "FALSE";

            break;
        }
    case VT_R4:
        {
            memset(tempDest,0,sizeof(tempDest));
            sprintf(tempDest,"%d",varValue.lVal); 
            strText = tempDest;
            break;
        }
    case VT_CY:
        {
            COleCurrency curValue;
            curValue.m_cur = varValue.cyVal;
            strText = curValue.Format().GetBuffer(0);
            break;
        }
    case VT_DATE:
        {
            if(varValue.date > MIN_DATE && varValue.date < MAX_DATE)
            {
                COleDateTime dteValue(varValue);
                strText = dteValue.Format("%Y.%m.%d").GetBuffer(0);
            }
            break;
        }
    case VT_BYREF|VT_UI1:
        {
            long lLen = 40;
            if(lLen > 0 && varValue.pbVal != NULL)
            {
                CString str,strTemp;
                for(int i = 0; i < min(lLen,100); i++)
                {
                    str.Format("0%X",varValue.pbVal[i]);
                    strTemp = strTemp + " " + str.Right(2);
                    strText = strTemp.GetBuffer(0);
                }

            }
            break;
        }
    }

    return strText;
}
//�Ͻ���
bool CFileMonitor::Analyse1( LPCTSTR lpszDir, vector<PlatformStru_DepthMarketData>& vShow2003, string& stTime )
{
    try
    {
        if(m_SetShow2003.IsOpen())
            m_SetShow2003.Close();

        m_SetShow2003.Open(lpszDir);
        if(!m_SetShow2003.IsOpen()) return false;

        char bufFormat[120],bufDest[120];
        COleVariant varValue;  
        string strText;
        PlatformStru_DepthMarketData sStruct;

        //���� 
        //������ʱ��
		memset(bufDest,0,sizeof(bufDest));
        m_SetShow2003.GetFieldValue(1,varValue);
		strText = Parase(1,varValue);
		if(strlen(strText.c_str())<6)
			sprintf(bufDest,"%0*d%s",6-strlen(strText.c_str()),0,strText.c_str());
		else
			strncpy(bufDest,strText.c_str(),6);

		strText = bufDest;
        if(m_strTimeOffShow2003.compare(strText) == 0)
            return false;
        else
            m_strTimeOffShow2003 = strText;

		
		sprintf(sStruct.UpdateTime,"%2s:%2s:%2s",
									m_strTimeOffShow2003.substr(0,2).c_str(),
									m_strTimeOffShow2003.substr(2,2).c_str(),
									m_strTimeOffShow2003.substr(4,2).c_str());

        //������
        m_SetShow2003.GetFieldValue(5,varValue);
        memset(bufFormat,0,sizeof(bufFormat));
        memset(bufDest,0,sizeof(bufDest));
        sprintf(bufFormat,"%s.%uf","%",m_SetShow2003.m_DBFFields[5].m_nDec);
        sprintf(bufDest,bufFormat,varValue.dblVal); 

        memcpy(sStruct.TradingDay,bufDest,sizeof(sStruct.TradingDay)-1);
        

        stTime = (string)sStruct.TradingDay + " " + (string)sStruct.UpdateTime;


        //��һ��
        m_SetShow2003.MoveNext();

        int nCol;
        long nCount = m_SetShow2003.GetRecordCount();
        long nColumns = m_SetShow2003.m_DBFFields.GetFieldCount();
        for (long nRow=0; nRow<nCount; nRow++)
        {
			
			strText = "SSE";//sse
			memcpy(sStruct.ExchangeID,strText.c_str(),sizeof(sStruct.ExchangeID)-1);//֤ȯ������
			//sStruct.NewVolume = 83;//NewVolume ��ʱ���� ������ID

			sStruct.PreSettlementPrice = DigitTools::GetInvalidValue_CTPDouble();
			sStruct.SettlementPrice = DigitTools::GetInvalidValue_CTPDouble();

            nCol = 0;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S1 ֤ȯ����
            strText = Parase(nCol,varValue);
            //memcpy(sStruct.ExchangeInstID,strText.c_str(),sizeof(sStruct.ExchangeInstID)-1);   
			memcpy(sStruct.InstrumentID,strText.c_str(),sizeof(sStruct.InstrumentID)-1);

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S2 ֤ȯ����
            strText = Parase(nCol,varValue);
            //memcpy(sStruct.InstrumentID,strText.c_str(),sizeof(sStruct.InstrumentID)-1);
			memcpy(sStruct.ExchangeInstID,strText.c_str(),sizeof(sStruct.ExchangeInstID)-1);   

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S3 ǰ���̼۸�
            strText = Parase(nCol,varValue);
            sStruct.PreClosePrice = atof(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S4 ���̼۸�
            strText = Parase(nCol,varValue);
            sStruct.OpenPrice = atof(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S5 ��ɽ����
            strText = Parase(nCol,varValue);
            sStruct.Turnover = atof(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S6 ��߼۸�
            strText = Parase(nCol,varValue);
            sStruct.HighestPrice = atof(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S7 ��ͼ۸�
            strText = Parase(nCol,varValue);
            sStruct.LowestPrice = atof(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S8 ����ָ��=���̼�
            strText = Parase(nCol,varValue);
            sStruct.LastPrice = atof(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S9 ��ǰ����۸� = �����һ
            strText = Parase(nCol,varValue);
            sStruct.BidPrice1 = atof(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S10 ��ǰ�����۸� = ������һ
            strText = Parase(nCol,varValue);
            sStruct.AskPrice1 = atof(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S11 �ɽ�����
            strText = Parase(nCol,varValue);
            sStruct.Volume = atoi(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S13 ��ӯ��
            strText = Parase(nCol,varValue);

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S15 ������һ
            strText = Parase(nCol,varValue);
            sStruct.BidVolume1 = atoi(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S16 ����۶�
            strText = Parase(nCol,varValue);
            sStruct.BidPrice2 = atof(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S17 ��������
            strText = Parase(nCol,varValue);
            sStruct.BidVolume2 = atoi(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S18 �������
            strText = Parase(nCol,varValue);
            sStruct.BidPrice3 = atof(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S19 ��������
            strText = Parase(nCol,varValue);
            sStruct.BidVolume3 = atoi(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S21 ������һ
            strText = Parase(nCol,varValue);
            sStruct.AskVolume1 = atoi(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S22 �����۶�
            strText = Parase(nCol,varValue);
            sStruct.AskPrice2 = atof(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S23 ��������
            strText = Parase(nCol,varValue);
            sStruct.AskVolume2 = atoi(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S24 ��������
            strText = Parase(nCol,varValue);
            sStruct.AskPrice3 = atof(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S25 ��������
            strText = Parase(nCol,varValue);
            sStruct.AskVolume3 = atoi(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S26 �������
            strText = Parase(nCol,varValue);
            sStruct.BidPrice4 = atof(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S27 ��������
            strText = Parase(nCol,varValue);
            sStruct.BidVolume4 = atoi(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S28 �������
            strText = Parase(nCol,varValue);
            sStruct.BidPrice5 = atof(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S29 ��������
            strText = Parase(nCol,varValue);
            sStruct.BidVolume5 = atoi(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S30 ��������
            strText = Parase(nCol,varValue);
            sStruct.AskPrice4 = atof(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S31 ��������
            strText = Parase(nCol,varValue);
            sStruct.AskVolume4 = atoi(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S32 ��������
            strText = Parase(nCol,varValue);
            sStruct.AskPrice5 = atof(strText.c_str());

            nCol++;
            m_SetShow2003.GetFieldValue(nCol,varValue);//S33 ��������
            strText = Parase(nCol,varValue);
            sStruct.AskVolume5 = atoi(strText.c_str());


			//ֻ��¼�ı��˵�����
			map<string, PlatformStru_DepthMarketData>::iterator it = m_mapPreShow2003.find(sStruct.InstrumentID);
			if(it!=m_mapPreShow2003.end())//�ҵ�				
			{
				if(memcmp(&sStruct,&(it->second),sizeof(PlatformStru_DepthMarketData))!=0)//����ͬ
				{
					LOG_INFO("%s  %s  %s %.2f %d",sStruct.UpdateTime,sStruct.ExchangeID,sStruct.InstrumentID,sStruct.LastPrice,sStruct.Volume);
					memcpy(&(it->second),&sStruct,sizeof(PlatformStru_DepthMarketData));//����
				}
			}
			else//û�ҵ�
				m_mapPreShow2003.insert(make_pair(sStruct.InstrumentID, sStruct));//����

			//m_mutex.lock();
			//m_queueQuot.push(sStruct);
			//m_mutex.unlock();

            if(nRow<nCount-1)
                m_SetShow2003.MoveNext();
        }
			
		//�������������������vShow2003
		//map<string,PlatformStru_DepthMarketData>::const_iterator it = m_mapPreShow2003.begin();
		//for (;it!=m_mapPreShow2003.end();it++)
		//	vShow2003.push_back(it->second);

        return true;

    }
    catch(CDBFException* e)
    {
        e->ReportError();
        e->Delete();
    }

    return false;
}

//���
bool CFileMonitor::Analyse2( LPCTSTR lpszDir, vector<PlatformStru_DepthMarketData>& vSJSHQ, string& stTime )
{
    try
    {
        if(m_SetSJSHQ.IsOpen())
            m_SetSJSHQ.Close();

        m_SetSJSHQ.Open(lpszDir);
        if(!m_SetSJSHQ.IsOpen()) return false;

        char bufFormat[120],bufDest[120],bufTemp[120];
        COleVariant varValue;  
        string strText;
        PlatformStru_DepthMarketData sStruct;

        //���� 
        //ʱ��
        m_SetSJSHQ.GetFieldValue(7,varValue);
        memset(bufFormat,0,sizeof(bufFormat));
		memset(bufDest,0,sizeof(bufDest));
        memset(bufTemp,0,sizeof(bufTemp));
        sprintf(bufFormat,"%s.%uf","%",m_SetSJSHQ.m_DBFFields[1].m_nDec);
        sprintf(bufTemp,bufFormat,varValue.dblVal);	
		if(strlen(bufTemp)<6)
			sprintf(bufDest,"%0*d%s",6-strlen(bufTemp),0,bufTemp);
		else
			strcpy(bufDest,bufTemp);
		
		strText = bufDest;
        if(m_strTimeOffSJSHQ.compare(strText) == 0)
            return false;
        else
            m_strTimeOffSJSHQ = strText; 

		//memcpy(sStruct.UpdateTime,bufDest,sizeof(sStruct.UpdateTime)-1);
		sprintf(sStruct.UpdateTime,"%2s:%2s:%2s",
									m_strTimeOffSJSHQ.substr(0,2).c_str(),
									m_strTimeOffSJSHQ.substr(2,2).c_str(),
									m_strTimeOffSJSHQ.substr(4,2).c_str());


        //����   
        m_SetSJSHQ.GetFieldValue(1,varValue);
        strText = (char*)(_bstr_t)varValue.bstrVal;
        memcpy(sStruct.TradingDay,strText.c_str(),sizeof(sStruct.TradingDay)-1);

        stTime = (string)sStruct.TradingDay + " " + (string)sStruct.UpdateTime;

        //��һ��
        m_SetSJSHQ.MoveNext();

        int nCol;
        long nCount = m_SetSJSHQ.GetRecordCount();
        long nColumns = m_SetSJSHQ.m_DBFFields.GetFieldCount();
        for (long nRow=0;nRow<nCount;nRow++)
        {
			strText = "SZSE";
			memcpy(sStruct.ExchangeID,strText.c_str(),sizeof(sStruct.ExchangeID)-1);//֤ȯ������
			//sStruct.NewVolume = 90;//NewVolume ��ʱ���� ������ID

			sStruct.PreSettlementPrice = DigitTools::GetInvalidValue_CTPDouble();
			sStruct.SettlementPrice = DigitTools::GetInvalidValue_CTPDouble();

            nCol = 0;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQZQDM ֤ȯ����
            strText = Parase(nCol,varValue,false);
            //memcpy(sStruct.ExchangeInstID,strText.c_str(),sizeof(sStruct.ExchangeInstID)-1);
			memcpy(sStruct.InstrumentID,strText.c_str(),sizeof(sStruct.InstrumentID)-1);

			nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQZQJC ֤ȯ���
            strText = Parase(nCol,varValue,false);
            //memcpy(sStruct.InstrumentID,strText.c_str(),sizeof(sStruct.InstrumentID)-1);
			memcpy(sStruct.ExchangeInstID,strText.c_str(),sizeof(sStruct.ExchangeInstID)-1);

			nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQZRSP �������̼�
            strText = Parase(nCol,varValue,false);
            sStruct.PreClosePrice = atof(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQJRKP ���տ��̼�
            strText = Parase(nCol,varValue,false);
            sStruct.OpenPrice = atof(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQZJCJ ����ɽ��� = ���¼�
            strText = Parase(nCol,varValue,false);
            sStruct.LastPrice = atof(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQCJSL �ɽ�����
            strText = Parase(nCol,varValue,false);
            sStruct.Volume = atoi(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQCJJE �ɽ����
            strText = Parase(nCol,varValue,false);
            sStruct.Turnover = atof(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQCJBS �ɽ�����
            strText = Parase(nCol,varValue,false);

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQZGCJ ��߳ɽ���
            strText = Parase(nCol,varValue,false);
            sStruct.HighestPrice = atof(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQZDCJ ��ͳɽ���
            strText = Parase(nCol,varValue,false);
            sStruct.LowestPrice = atof(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQSYL1 ��ӯ�� 1
            strText = Parase(nCol,varValue,false);

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQSYL2 ��ӯ�� 2
            strText = Parase(nCol,varValue,false);

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQJSD1 �۸����� 1  
            strText = Parase(nCol,varValue,false);

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQJSD2 �۸����� 2  
            strText = Parase(nCol,varValue,false);

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQHYCC ��Լ�ֲ���
            strText = Parase(nCol,varValue,false);
            sStruct.OpenInterest = atoi(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQSJW5  ����λ��
            strText = Parase(nCol,varValue,false);
            sStruct.AskPrice5 = atof(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQSSL5  ��������
            strText = Parase(nCol,varValue,false);
            sStruct.AskVolume5 = atoi(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQSJW4  ����λ��
            strText = Parase(nCol,varValue,false);
            sStruct.AskPrice4 = atof(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQSSL4  ��������
            strText = Parase(nCol,varValue,false);
            sStruct.AskVolume4 = atoi(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQSJW3  ����λ��
            strText = Parase(nCol,varValue,false);
            sStruct.AskPrice3 = atof(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQSSL3  ��������
            strText = Parase(nCol,varValue,false);
            sStruct.AskVolume3 = atoi(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQSJW2  ����λ��
            strText = Parase(nCol,varValue,false);
            sStruct.AskPrice2 = atof(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQSSL2  ��������
            strText = Parase(nCol,varValue,false);
            sStruct.AskVolume2 = atoi(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQSJW1  ����λһ/������ʾ��
            strText = Parase(nCol,varValue,false);
            sStruct.AskPrice1 = atof(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQSSL1  ������һ
            strText = Parase(nCol,varValue,false);
            sStruct.AskVolume1 = atoi(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQBJW1  ���λһ/�����ʾ��
            strText = Parase(nCol,varValue,false);
            sStruct.BidPrice1 = atof(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQBSL1  ������һ
            strText = Parase(nCol,varValue,false);
            sStruct.BidVolume1 = atoi(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQBJW2  ���λ��
            strText = Parase(nCol,varValue,false);
            sStruct.BidPrice2 = atof(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQBSL2  ��������
            strText = Parase(nCol,varValue,false);
            sStruct.BidVolume2 = atoi(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQBJW3  ���λ��
            strText = Parase(nCol,varValue,false);
            sStruct.BidPrice3 = atof(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQBSL3  ��������
            strText = Parase(nCol,varValue,false);
            sStruct.BidVolume3 = atoi(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQBJW4  ���λ��
            strText = Parase(nCol,varValue,false);
            sStruct.BidPrice4 = atof(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQBSL4  ��������
            strText = Parase(nCol,varValue,false);
            sStruct.BidVolume4 = atoi(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQBJW5  ���λ��
            strText = Parase(nCol,varValue,false);
            sStruct.BidPrice5 = atof(strText.c_str());

            nCol++;
            m_SetSJSHQ.GetFieldValue(nCol,varValue);//HQBSL5  ��������
            strText = Parase(nCol,varValue,false);
            sStruct.BidVolume5 = atoi(strText.c_str());

			//ֻ��¼�ı��˵�����
			map<string, PlatformStru_DepthMarketData>::iterator it = m_mapPreSJSHQ.find(sStruct.InstrumentID);
			if(it!=m_mapPreSJSHQ.end())//�ҵ�				
			{
				if(memcmp(&sStruct,&(it->second),sizeof(PlatformStru_DepthMarketData))!=0)//����ͬ
				{
					LOG_INFO("%s  %s  %s %.2f %d",sStruct.UpdateTime,sStruct.ExchangeID,sStruct.InstrumentID,sStruct.LastPrice,sStruct.Volume);
					memcpy(&(it->second),&sStruct,sizeof(PlatformStru_DepthMarketData));//����
				}
			}
			else//û�ҵ�
				m_mapPreSJSHQ.insert(make_pair(sStruct.InstrumentID, sStruct));//����

			//m_mutex.lock();
			//m_queueQuot.push(sStruct);
			//m_mutex.unlock();

            if(nRow<nCount-1)
                m_SetSJSHQ.MoveNext();
        }
					
		//�������������������vSJSHQ
		//map<string,PlatformStru_DepthMarketData>::const_iterator it = m_mapPreSJSHQ.begin();
		//for (;it!=m_mapPreSJSHQ.end();it++)
		//	vSJSHQ.push_back(it->second);

        return true;
    }
    catch(CDBFException* e)
    {
        e->ReportError();
        e->Delete();
    }
       
    return false;
}
DWORD WINAPI  CFileMonitor::MonitorProc ( LPVOID lParam )
{
    CFileMonitor* pThis = ( CFileMonitor* )lParam;
    ASSERT( pThis != NULL );

    char szBuf[ 2 * ( sizeof ( FILE_NOTIFY_INFORMATION ) + MAX_PATH ) ];
    FILE_NOTIFY_INFORMATION* pNotify  =  ( FILE_NOTIFY_INFORMATION * )szBuf;
    DWORD dwBytesReturned( 0 );

    while( pThis->m_bContinue )
    {
        if( !::ReadDirectoryChangesW( pThis->m_hDir, pNotify, sizeof( szBuf ), TRUE, FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME|
            FILE_NOTIFY_CHANGE_ATTRIBUTES|    FILE_NOTIFY_CHANGE_SIZE|FILE_NOTIFY_CHANGE_LAST_WRITE|
            FILE_NOTIFY_CHANGE_LAST_ACCESS|FILE_NOTIFY_CHANGE_CREATION|FILE_NOTIFY_CHANGE_SECURITY,
            &dwBytesReturned, NULL,    NULL ) )
        {
            break;
        }
        else
        {
            WCHAR *pszFileDst  =  NULL;
            WCHAR *pszFileSrc  =  pNotify->FileName;
            pszFileSrc[ pNotify->FileNameLength/2 ] = L'\0';

            if( 0 != pNotify->NextEntryOffset )
            {
                PFILE_NOTIFY_INFORMATION pNext = (PFILE_NOTIFY_INFORMATION)( ( char* )pNotify + pNotify->NextEntryOffset);
                pszFileDst  = pNext->FileName;
                pszFileDst[ pNext->FileNameLength/2 ] = L'\0';
            }
            if( NULL != pThis->m_pFunDeal )
                pThis->m_pFunDeal( (tagACTION)pNotify->Action, CW2T(pszFileSrc) , CW2T(pszFileDst) );
        }
    }

    return 0;
}

//char* WcharToChar(const wchar_t* wp)  
//{  
//    int len= WideCharToMultiByte(CP_ACP,0,wp,wcslen(wp),NULL,0,NULL,NULL);  
//    char m_char[256];  
//    WideCharToMultiByte(CP_ACP,0,wp,wcslen(wp),m_char,len,NULL,NULL);  
//    m_char[len]='\0';  
//    return m_char;  
//} 