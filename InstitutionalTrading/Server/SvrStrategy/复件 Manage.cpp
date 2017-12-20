#include "stdafx.h"

#include "Manage.h"
#include "shellapi.h"
CManage::CManage(void)
{
	
}

CManage::~CManage(void)
{
}
void CManage::Init()
{
	m_DataCenter.Init();
	StartAllInstance();
}
bool CManage::BeginSStrategyInstanceProcess(SStrategyInstance& instance)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si,0,sizeof(si));
	si.cb=sizeof(si);
	si.wShowWindow=SW_SHOW;
	si.dwFlags=STARTF_USESHOWWINDOW;

	std::string strXML;
	loadXMLString(instance, strXML);
	std::string strFileFolder = "Strategy";
	std::string strFileName   = "RunSingleStrategy.exe";
	GetFolderFileName(strFileName, strFileFolder);

	std::string strCmd = strFileName + " \"" + strXML + "\"";	
	
	bool bRet = CreateProcess(NULL,(LPSTR)strCmd.c_str(),NULL, NULL, FALSE, 0, NULL, (LPSTR)strFileFolder.c_str(), &si,&pi);
	STraderInstance key(instance.strategyName, instance.strTraderName);
	m_lockInstance2Handle.write_lock();
	m_mapInstance2Handle[key] = pi.hProcess;
	m_lockInstance2Handle.write_unlock();
	return bRet;
}
void CManage::StartAllInstance()
{
	std::map<STraderInstance, SStrategyInstance> mapStrategyInstance;
	m_DataCenter.GetAllInstance(mapStrategyInstance);
	std::map<STraderInstance, SStrategyInstance>::iterator it = mapStrategyInstance.begin();
	while(it != mapStrategyInstance.end())
	{
		SStrategyInstance& instance = it->second;
	//	if(instance.nStart == 1)
		{//����
			BeginSStrategyInstanceProcess(instance);
		}
		it++;
	}
	
}
void CManage::StopAllInstance()
{
	std::map<STraderInstance, SStrategyInstance> mapStrategyInstance;
	m_DataCenter.GetAllInstance(mapStrategyInstance);
	std::map<STraderInstance, SStrategyInstance>::iterator it = mapStrategyInstance.begin();
	while(it != mapStrategyInstance.end())
	{
		SStrategyInstance& instance = it->second;
// 	//	if(instance.nStart == 1)
 		{//��ͣ��
 			CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(instance.strTraderName, instance.strategyName, 1);			
 		}
		it++;
	}
	Sleep(2000);
	it = mapStrategyInstance.begin();
	while(it != mapStrategyInstance.end())
	{
		SStrategyInstance& instance = it->second;
		//	if(instance.nStart == 1)
		{//����
			TerminatInstanceThread(instance.strategyName, instance.strTraderName);			
		}
		it++;
	}
}
void CManage::HandleAllInstanceTimer(bool bStop)
{
    std::map<STraderInstance, SStrategyInstance> mapStrategyInstance;
    m_DataCenter.GetAllInstance(mapStrategyInstance);
    std::map<STraderInstance, SStrategyInstance>::iterator it = mapStrategyInstance.begin();
    while(it != mapStrategyInstance.end())
    {
        SStrategyInstance& instance = it->second;
        if(instance.bStartTimer)
        {           
            //����ͣ�ã���Ϊ���ӶϿ����û����������ȥ�µ��ˣ���ֻ�ǹر������Ķ�ʱ��
            CInterface_SvrTradeData::getObj().PushStrategyTimer(instance.strTraderName, instance.strategyName, 1);
        }
        it++;
    }
}
bool CManage::TerminatInstanceThread(std::string strName, std::string strUploader)
{
	m_lockInstance2Handle.write_lock();
	STraderInstance key(strName.c_str(), strUploader.c_str());
	std::map<STraderInstance, HANDLE>::iterator it =  m_mapInstance2Handle.find(key);
	if(it != m_mapInstance2Handle.end())
	{
		HANDLE hd = it->second;
		::TerminateProcess(hd, 4);
	}
	m_lockInstance2Handle.write_unlock();
	return true;
}
void CManage::string_replace( std::string &strBig, const std::string &strsrc, const std::string &strdst )
{	
	std::string::size_type pos = 0;
	std::string::size_type srclen = strsrc.size();
	std::string::size_type dstlen = strdst.size();

	while( (pos=strBig.find(strsrc, pos)) != std::string::npos )
	{
		strBig.replace( pos, srclen, strdst );
		pos += dstlen;
	}
} 
bool CManage::loadXMLString(SStrategyInstance& instance, std::string& strXML)
{

	TiXmlDocument *myDocument = new TiXmlDocument();
//	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0","gb2312","no");  
//	myDocument->LinkEndChild(decl);  


	//����һ����Ԫ�ز����ӡ�
	TiXmlElement *RootElement = new TiXmlElement("strategy");

	myDocument->LinkEndChild(RootElement);

	//����һ��PersonԪ�ز����ӡ�
	TiXmlElement *strategyName = new TiXmlElement("strategyName");
	RootElement->LinkEndChild(strategyName);
	TiXmlText *strategyNameContent = new TiXmlText(instance.strategyName);
	strategyName->LinkEndChild(strategyNameContent);

	TiXmlElement *traderName = new TiXmlElement("traderName");
	RootElement->LinkEndChild(traderName);
	TiXmlText *traderNameContent = new TiXmlText(instance.strTraderName);
	traderName->LinkEndChild(traderNameContent);

	TiXmlElement *para = new TiXmlElement("parameter");
	RootElement->LinkEndChild(para);

	char szPara[2555];
	sprintf(szPara,"%s", instance.strPara);
//	sprintf(szPara,"%s","<para\>test</para>");
	TiXmlText *paraContent = new TiXmlText(szPara);	
	para->LinkEndChild(paraContent);
	
	
	TiXmlElement *instrument = new TiXmlElement("instrument");
	RootElement->LinkEndChild(instrument);
	TiXmlText *instrumentContent = new TiXmlText(instance.Instruments);
	instrument->LinkEndChild(instrumentContent);

	int nFrontID =0, nSessionID =0;
	CInterface_SvrLogin::getObj().GenerateUniqueFrontAndSessionID(nFrontID, nSessionID);
	char szFrontID[55];
	sprintf(szFrontID,"%d",nFrontID);
	char szSessionID[55];
	sprintf(szSessionID,"%d",nSessionID);

	TiXmlElement *sessionID = new TiXmlElement("sessionID");
	RootElement->LinkEndChild(sessionID);
	TiXmlText *sessionIDContent = new TiXmlText(szSessionID);
	sessionID->LinkEndChild(sessionIDContent);

	TiXmlElement *FrontID = new TiXmlElement("frontID");
	RootElement->LinkEndChild(FrontID);
	TiXmlText *szFrontIDContent = new TiXmlText(szFrontID);
	FrontID->LinkEndChild(szFrontIDContent);

	TiXmlElement *orderRefID = new TiXmlElement("orderRef");
	RootElement->LinkEndChild(orderRefID);
	TiXmlText *orderRefIDContent = new TiXmlText("1");
	orderRefID->LinkEndChild(orderRefIDContent);
//////////////////////////////////////////////////////////////////////////
	char szStartTimer[55];
	sprintf(szStartTimer,"%d",instance.bStartTimer);

	TiXmlElement *StartTimer = new TiXmlElement("startTimer");
	RootElement->LinkEndChild(StartTimer);
	TiXmlText *sessionStartTimer = new TiXmlText(szStartTimer);
	StartTimer->LinkEndChild(sessionStartTimer);

	char szTimerSpan[55];
	sprintf(szTimerSpan,"%d",instance.nTimerSpan);

	TiXmlElement *TimerSpan = new TiXmlElement("timerSpan");
	RootElement->LinkEndChild(TimerSpan);
	TiXmlText *sessionTimerSpan = new TiXmlText(szTimerSpan);
	TimerSpan->LinkEndChild(sessionTimerSpan);

	
	int nRunAfterOffline =0;
	char szRunAfterOffline[5];
	memset(&szRunAfterOffline,0,sizeof(szRunAfterOffline));
	if(instance.nStart == 1)
	{
		sprintf(szRunAfterOffline,"%d",1);
		if(instance.nRunAfterOffline  == 0)
		{//���ߺ�����
			UserInfo lUserInfo;
			bool bGet = CInterface_SvrUserOrg::getObj().GetUserByName(instance.strTraderName,lUserInfo);			
			if(bGet)
			{
				sUserOnlineInfo lsInfo;
				CInterface_SvrLogin::getObj().GetOnlineUserStatus(lUserInfo.nUserID,lsInfo);
				if(lsInfo.mUserOnlineCount == 0)
				{
					sprintf(szRunAfterOffline,"%d",0);
				}
			}
		}		
	}
	else 
		sprintf(szRunAfterOffline,"%d",0);

	TiXmlElement *RunAfterOffline = new TiXmlElement("RunAfterOffline");//0Ϊ��ת��1Ϊ���У���RunAfterOffline���ǽ���Ա���Ⱥ���ͣ���е���˼����������ˣ�
	RootElement->LinkEndChild(RunAfterOffline);
	TiXmlText *ttszRunAfterOffline = new TiXmlText(szRunAfterOffline);
	RunAfterOffline->LinkEndChild(ttszRunAfterOffline);

//////////////////////////////////////////////////////////////////////////
	TiXmlPrinter printer;
	myDocument->Accept(&printer);
	strXML =	printer.CStr();
	string_replace(strXML, "&lt;", "<");
	string_replace(strXML, "&gt;", ">");
	string_replace(strXML, "&apos;", "'");
	string_replace(strXML, "&#x0D;&#x0A;", "");
	string_replace(strXML, "&#x0A;", "");//ֻ�л��з���ֱ��ȥ��
	string_replace(strXML, "&#x0D;", "");//c#���벻֧�ֻس����з�
	//string_replace(strXML, ">\n;", ">");//c#���벻֧�ֻس����з�
	//myDocument->SaveFile("c:\\1.xml");
	return true;
}
bool CManage::UploadBaseStrategy(SStrategy& strategy, int& nError)
{
	if(strategy.strTraderName =="")
	{//����ϴ���Ϊ�գ��������ϴ�
		nError = CF_ERROR_DATA_STRATEGY_EXISTANDUSE;
		return false;
	}
	SStrategy strategyExist;
	bool bGet = m_DataCenter.GetStrategy(strategy.strategyName, strategyExist);
	if(bGet) {
		if(strcmp(strategyExist.strTraderName, strategy.strTraderName)!=0) {
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER;
			return false;
		}
		if(strategyExist.nUse ==1)
		{//�����Ѿ����ڣ����Ҵ�������״̬
			nError = CF_ERROR_DATA_STRATEGY_EXISTANDUSE;
			return false;
		}
	}
	//strategy.nUse = 2;//�ϴ���ʱ���ȴ��ڽ��ã���Ȼ�Ļ��ڶ������������ϴ�û����ɵĻ����´ξ�û�������ٴ��ϴ���������ˣ���Ϊ���Դ�������״̬����
	CInterface_SvrDBOpr::getObj().DB_AddStrategy(strategy, nError);
	if(nError != 0)//
		return false;
	m_DataCenter.AddStrategy(strategy);

	std::vector<SStrategy2Index> vec;
	ParsePara(strategy.strPara, vec);
	for(int i =0; i<(int)vec.size(); i++)
	{
		SStrategy2Index& ssI = vec[i];
		bool bSuccess = AddStrategyIndexRelation(ssI.strategyName, ssI.IndexName, nError);	
		if(!bSuccess)
		{
			return false;
		}
		m_DataCenter.AddStrategy2IndexRelation(ssI.strategyName, ssI.IndexName);		
	}

	//log�����Ϣ
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nLogErrorCode; 
	LogStrategy logData;
	memset(&logData,0,sizeof(LogStrategy));

	strcpy(logData.logOperatorID, strategy.strTraderName);
	strcpy(logData.logTargetorID, strategy.strategyName);

	strcpy(logData.logTable,"LOG_STRATEGY");
	sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	logData.logAction     = Oper_Add;
	sprintf(logData.logContent,"�ϴ�����(������:[%s]  ����Ա:[%s] ",strategy.strategyName, strategy.strTraderName);
	logData.logResult = (CF_ERROR_SUCCESS == nError)?true:false;

	CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);

	return true;
}
bool CManage::ParsePara(std::string strPara, std::vector<SStrategy2Index>& vec)
{
	TiXmlDocument *doc = new TiXmlDocument();    
	doc->Parse(strPara.c_str());   
	if (&doc == NULL)   
		return false;

	TiXmlElement* RootElement=doc->RootElement();
	if(RootElement)
	{
		TiXmlAttribute* attributeOfstrategy = RootElement->FirstAttribute();  //���strategy��name����
		std::string strstrategy = attributeOfstrategy->Value();


		const char* pRootElement = RootElement->Value();
		if(strcmp(pRootElement, "strategy") ==0)
		{
			TiXmlElement* pElement=RootElement->FirstChildElement("indicators");
			if(pElement)
			{
				TiXmlElement *pElementindicator = pElement->FirstChildElement("indicator");
				while(pElementindicator)
				{
					TiXmlAttribute* attributeOFindicator = pElementindicator->FirstAttribute();  //���student��name����
					std::string strindicator = attributeOFindicator->Value();
					SStrategy2Index sI;
					strcpy(sI.strategyName, strstrategy.c_str());
					strcpy(sI.IndexName, strindicator.c_str());
					vec.push_back(sI);
					pElementindicator = pElementindicator->NextSiblingElement();	
				}
			}

		}			
	}

	return true;
}
bool CManage::AddStrategyIndexRelation(std::string strName, std::string strIndex, int& nError)
{	
	CInterface_SvrDBOpr::getObj().DB_AddStrategy2IndexRelation(strName, strIndex, nError);
	if(nError != 0)//
		return false;
	m_DataCenter.AddStrategy2IndexRelation(strName, strIndex);

	return true;
}
bool CManage::UploadDllFile(std::string strName, std::string strUploader, void* pData, int nLength, int& nError)
{
	SStrategy strategyExist;
	if(m_DataCenter.GetStrategy(strName, strategyExist)) {
		if(strcmp(strUploader.c_str(), strategyExist.strTraderName)!=0) {
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER;
			return false;
		}
	}
	std::string strFileFolder = "Strategy";
	std::string strFileName   = strName + ".dll";
	GetFolderFileName(strFileName, strFileFolder);
	FILE* pFile = fopen( strFileName.c_str(), "wb" );
	if(pFile == NULL)
	{
		nError = CF_ERROR_DATA_STRATEGY_SAVEDLLFAIL;
		return false;// ������ļ��洢ʧ��
	}
	fwrite(pData, nLength, 1, pFile);	
	fclose(pFile);
	CInterface_SvrDBOpr::getObj().DB_WriteStragegyFile(strName, UPLOAD_STRATEGY_DLL, (char*)pData, nLength, nError);
	if(nError != 0)
		return false;
	return true;
}
bool CManage::UploadZIPFile(std::string strName, std::string strUploader, void* pData, int nLength, int& nError)
{
	SStrategy strategyExist;
	if(m_DataCenter.GetStrategy(strName, strategyExist)) {
		if(strcmp(strUploader.c_str(), strategyExist.strTraderName)!=0) {
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER;
			return false;
		}
	}
	std::string strFileFolder = "Strategy";
	std::string strFileName   = strName + ".zip";
	GetFolderFileName(strFileName, strFileFolder);
	FILE* pFile = fopen( strFileName.c_str(), "wb" );
	if(pFile == NULL)
	{
		nError = CF_ERROR_DATA_STRATEGY_SAVEZIPFAIL;
		return false;// ������ļ��洢ʧ��
	}
	fwrite(pData, nLength, 1, pFile);	
	fclose(pFile);
	CInterface_SvrDBOpr::getObj().DB_WriteStragegyFile(strName, UPLOAD_STRATEGY_ZIP, (char*)pData, nLength, nError);
	if(nError != 0)
		return false;

	m_DataCenter.calcUploadAllInfo(strUploader, strName, 0);
	return true;
}
bool CManage::DownloadAllBaseStategy(std::string strUploader, std::vector<SStrategy>& vecStrategy, int& nError)
{
	bool bGet = m_DataCenter.GetAllUserPriveledgeStrategy(strUploader, vecStrategy);
	if(!bGet)
	{
		nError = CF_ERROR_DATA_STRATEGY_NOTEXIST;
		return false;		
	}

	return true;
}
bool CManage::DownloadBaseStrategy(std::string strName/*, std::string strUploader*/, SStrategy& strategy, int& nError)
{
	bool bGet = m_DataCenter.GetStrategy(strName, strategy);
	if(!bGet)
	{
		nError = CF_ERROR_DATA_STRATEGY_NOTEXIST;
		return false;		
	}
/*	bool bIsUpLoader = m_DataCenter.IsStrategyBelongToTrader(strName, strUploader);
	if(!bIsUpLoader)
	{
		if(strategy.nPrivate == 1)
		{//�ò��Բ������ڸý���Ա�����Ҹò���Ϊ˽��
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_PRIVATE;
			return false;	
		}		
	}	*/
	return true;
}
bool CManage::DownloadDllFile(std::string strName, std::string strUploader, void*& pData, int& nLength, int& nError)
{
	SStrategy strategy;
	bool bGet = m_DataCenter.GetStrategy(strName, strategy);
	if(!bGet)
	{
		nError = CF_ERROR_DATA_STRATEGY_NOTEXIST;
		return false;		
	}
	bool bIsUpLoader = m_DataCenter.IsStrategyBelongToTrader(strName, strUploader);
	if(!bIsUpLoader)
	{
		if(strategy.nPrivate == 1)
		{//�ò��Բ������ڸý���Ա�����Ҹò���Ϊ˽��
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_PRIVATE;
			return false;	
		}		
	}	
	
	std::string strFileFolder = "Strategy";
	std::string strFileName   = strName + ".dll";
	GetFolderFileName(strFileName, strFileFolder);
	FILE* pFile = fopen( strFileName.c_str(), "rb" );
	if(pFile == NULL)
	{
		nError = CF_ERROR_DATA_READDLLFAIL;
		return false;// ������ļ��洢ʧ��
	}

	fseek(pFile, 0, SEEK_END);
	int lnSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	if(lnSize > 0)
	{
		pData = new char[lnSize+54];
		int n = fread((char*)pData+54,lnSize,1,pFile);
		if(n>0)
		 nLength = lnSize+54;
	}
	fclose(pFile);

	return lnSize>0;
}
bool CManage::DownloadZIPFile(std::string strName, std::string strUploader, void*& pData, int& nLength, int& nError)
{
	SStrategy strategy;
	bool bGet = m_DataCenter.GetStrategy(strName, strategy);
	if(!bGet)
	{
		nError = CF_ERROR_DATA_STRATEGY_NOTEXIST;
		return false;		
	}
	bool bIsUpLoader = m_DataCenter.IsStrategyBelongToTrader(strName, strUploader);
	if(!bIsUpLoader)
	{
		//if(strategy.nPrivate == 1)
		{//�ò��Բ������ڸý���Ա��Դ������������
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_PUBLIC;
			return false;	
		}		
	}	

	std::string strFileFolder = "Strategy";
	std::string strFileName   = strName + ".zip";
	GetFolderFileName(strFileName, strFileFolder);
	FILE* pFile = fopen( strFileName.c_str(), "rb" );
	if(pFile == NULL)
	{
		nError = CF_ERROR_DATA_READZIPFAIL;
		return false;// ������ļ��洢ʧ��
	}

	fseek(pFile, 0, SEEK_END);
	int lnSize = ftell(pFile);
	if(lnSize > 0)
	{
		pData = new char[lnSize+54];
		fseek(pFile, 0, SEEK_SET);
		int n = fread((char*)pData+54,lnSize,1,pFile);
		if(n>0)
			nLength = lnSize+54;
	}
	fclose(pFile);

	return lnSize>0;
}

bool CManage::UploadBaseIndex(SIndex& Index, int& nError)
{
	if(Index.strTraderName =="")
	{//����ϴ���Ϊ�գ��������ϴ�
		nError = CF_ERROR_DATA_STRATEGY_EXISTANDUSE;
		return false;
	}
	SIndex oldIndex;
	bool bGet = m_DataCenter.GetIndex(Index.IndexName, oldIndex);
	if(bGet)
	{	
		if(strcmp(oldIndex.strTraderName, Index.strTraderName)!=0) {
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER;
			return false;
		}
		std::vector<std::string> vecStrategyName;
		m_DataCenter.GetStrategyofIndex(Index.IndexName, vecStrategyName);		
		for(int i =0; i< (int)vecStrategyName.size(); i++)
		{
			std::string strStrategyName = vecStrategyName[i];
			SStrategy strategyExist;
			bool bGet = m_DataCenter.GetStrategy(strStrategyName, strategyExist);
			if(bGet)
			{
				if(strategyExist.nUse ==1)
				{//�����Ѿ����ڣ����Ҵ�������״̬
					nError = CF_ERROR_DATA_IndexIsUsing;
					return false;
				}
			}
		}				
	}
	CInterface_SvrDBOpr::getObj().DB_AddIndex(Index, nError);
	if(nError != 0)//
		return false;
	m_DataCenter.AddIndex(Index);

	//log�����Ϣ
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nLogErrorCode; 
	LogStrategy logData;
	memset(&logData,0,sizeof(LogStrategy));

	strcpy(logData.logOperatorID, Index.strTraderName);
	strcpy(logData.logTargetorID, Index.IndexName);

	strcpy(logData.logTable,"LOG_STRATEGY");
	sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	logData.logAction     = Oper_Modify;
	sprintf(logData.logContent,"�ϴ�ָ��(ָ����:[%s]  ����Ա:[%s] ",Index.IndexName, Index.strTraderName);
	logData.logResult = (CF_ERROR_SUCCESS == nError)?true:false;

	CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);
	return true;
}
bool CManage::UploadIndexFile(std::string strName, std::string strUploader, int nType, void* pData, int nLength, int& nError)
{
	SIndex existIndex;
	if(m_DataCenter.GetIndex(strName, existIndex)) {	
		if(strcmp(strUploader.c_str(), existIndex.strTraderName)!=0) {
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER;
			return false;
		}
	}
	std::string strFileFolder = "Strategy";
	std::string strFileName;
	if(nType == 2)
	   strFileName   = strName + ".dll";
	else if(nType == 3)
		strFileName   = strName + ".zip";
	GetFolderFileName(strFileName, strFileFolder);
	FILE* pFile = fopen( strFileName.c_str(), "wb" );
	if(pFile == NULL)
	{
		nError = CF_ERROR_DATA_INDEX_SAVEDLLFAIL;
		return false;// ������ļ��洢ʧ��
	}
	fwrite(pData, nLength, 1, pFile);	
	fclose(pFile);
	CInterface_SvrDBOpr::getObj().DB_WriteIndexFile(strName, nType, (char*)pData, nLength, nError);
	if(nError != 0)
		return false;

	if(nType == 3)
		m_DataCenter.calcUploadAllInfo(strUploader, strName, 1);
	return true;

}
bool CManage::DownloadAllBaseIndex(std::string strUploader, std::vector<SIndex>& vecIndex, int& nError)
{
	return	m_DataCenter.GetAllUserPriveledgeIndex(strUploader, vecIndex);
}
bool CManage::DownloadBaseIndex(std::string strName/*, std::string strUploader*/, SIndex& index, int& nError)
{
	bool bGet = m_DataCenter.GetIndex(strName, index);
	if(!bGet)
	{
		nError = CF_ERROR_DATA_INDEX_NOTEXIST;
		return false;		
	}/*
	bool bIsUpLoader = m_DataCenter.IsStrategyBelongToTrader(strName, strUploader);
	if(!bIsUpLoader)
	{
		if(index.nPrivate == 1)
		{//�ò��Բ������ڸý���Ա�����Ҹò���Ϊ˽��
			nError = CF_ERROR_DATA_INDEX_NOTBELONGTOUPLOADER_PRIVATE;
			return false;	
		}		
	}	*/
	return true;
}
bool CManage::DownloadIndexFile(std::string strName, std::string strUploader, int nType, void*& pData, int& nLength, int& nError)
{
	SIndex Index;
	bool bGet = m_DataCenter.GetIndex(strName, Index);
	if(!bGet)
	{
		nError = CF_ERROR_DATA_INDEX_NOTEXIST;
		return false;		
	}
	bool bIsUpLoader = m_DataCenter.IsIndexBelongToTrader(strName, strUploader);
	if(!bIsUpLoader)
	{
		if(Index.nPrivate == 1)
		{//�ò��Բ������ڸý���Ա�����Ҹò���Ϊ˽��
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_PRIVATE;
			return false;	
		}
		else if(nType == 3) {
			// ����������Դ����
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_PUBLIC;
			return false;
		}
	}	

	std::string strFileFolder = "Strategy";
	std::string strFileName;
	if(nType == 2)
		strFileName   = strName + ".dll";
	else if(nType == 3)
		strFileName   = strName + ".zip";
	GetFolderFileName(strFileName, strFileFolder);
	FILE* pFile = fopen( strFileName.c_str(), "rb" );
	if(pFile == NULL)
	{
		nError = CF_ERROR_DATA_READDLLFAIL;
		return false;// ������ļ��洢ʧ��
	}

	fseek(pFile, 0, SEEK_END);
	int lnSize = ftell(pFile);
	if(lnSize > 0)
	{
		pData = new char[lnSize + 54];//���54���ֽ�Ϊ��ͷ�ĵĲ��֣�����Ժ�������ĵĻ������Ҳ�øģ������ƵĲ���
		fseek(pFile, 0, SEEK_SET);
		int n = fread((char*)pData+54,lnSize,1,pFile);//���54���ֽ�Ϊ��ͷ�ĵĲ��֣�����Ժ�������ĵĻ������Ҳ�øģ������ƵĲ���
		if(n>0)
			nLength = lnSize+54;
	}
	fclose(pFile);

	return lnSize>0;

}
bool CManage::UseStrategy(SUseStrategy& useStrategy, int& nError)
{
	if(useStrategy.nUserType == 2)
	{//�����ն�
		SStrategy strategy;
		bool bGet = m_DataCenter.GetStrategy(useStrategy.strategyName, strategy);
		if(!bGet)
		{
			nError = CF_ERROR_DATA_STRATEGY_NOTEXIST;
			return false;		
		}
		bool bIsUpLoader = m_DataCenter.IsStrategyBelongToTrader(useStrategy.strategyName, useStrategy.strTraderName);
		if(!bIsUpLoader)
		{
			if(strategy.nPrivate == 1)
			{//�ò��Բ������ڸý���Ա�����Ҹò���Ϊ˽��
				nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_USE;
				return false;	
			}		
		}	
	}

	CInterface_SvrDBOpr::getObj().DB_SetStragegyStatus(useStrategy.strategyName, useStrategy.nStatus, nError);
	if(nError != CF_ERROR_SUCCESS)
		return false;
	
	m_DataCenter.SetStragegyStatus(useStrategy.strategyName, useStrategy.nStatus);	

	if(useStrategy.nStatus == 1)
	{
		//���ò���
		std::map<STraderInstance, SStrategyInstance> mapStrategyInstance;
		m_DataCenter.GetAllInstance(mapStrategyInstance);
		std::map<STraderInstance, SStrategyInstance>::iterator it = mapStrategyInstance.begin();
		while(it != mapStrategyInstance.end())
		{
			if(m_mapInstance2Handle.find(it->first) != m_mapInstance2Handle.end()) {
				it++;
				continue;
			}
			SStrategyInstance& instance = it->second;
			if(useStrategy.strategyName == useStrategy.strategyName  && instance.nStart == 1)
			{//����
				BeginSStrategyInstanceProcess(instance);
			}
			it++;
		}
	}
	else {
		//��ֹ����
		std::map<STraderInstance, SStrategyInstance> mapStrategyInstance;
		m_DataCenter.GetAllInstance(mapStrategyInstance);
		std::map<STraderInstance, SStrategyInstance>::iterator it = mapStrategyInstance.begin();
		while(it != mapStrategyInstance.end())
		{
			CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(it->first.strTrader, it->first.strStrategy, 1);			
			it++;
		}
		Sleep(1000);
		it = mapStrategyInstance.begin();
		while(it != mapStrategyInstance.end())
		{			
			TerminatInstanceThread(it->first.strStrategy, it->first.strTrader);//ɱ������	
			m_mapInstance2Handle.erase(it->first);
			it++;
		}
	}
	if(useStrategy.nStatus == 1)
		CInterface_SvrTradeData::getObj().PushStrategyStopOrRun(useStrategy.strategyName, 0);
	else if(useStrategy.nStatus == 0)
		CInterface_SvrTradeData::getObj().PushStrategyStopOrRun(useStrategy.strategyName, 1);
		
	//log�����Ϣ
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nLogErrorCode; 
	LogStrategy logData;
	memset(&logData,0,sizeof(LogStrategy));

	strcpy(logData.logOperatorID, useStrategy.strTraderName);
	strcpy(logData.logTargetorID, useStrategy.strategyName);

	strcpy(logData.logTable,"LOG_STRATEGY");
	sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	logData.logAction     = Oper_Modify;
	sprintf(logData.logContent,"����״̬�ı�(������:[%s]  ����Ա:[%s]�� ״̬��[%d] ",useStrategy.strategyName, useStrategy.strTraderName, useStrategy.nStatus );
	logData.logResult = (CF_ERROR_SUCCESS == nError)?true:false;

	CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);
	return true;
}
bool CManage::DelStrategy(SDelStrategy& sDel, int& nError)
{
	if(sDel.nUserType == 2)
	{//�����ն�
		SStrategy strategyExist;
		bool bGet = m_DataCenter.GetStrategy(sDel.strategyName, strategyExist);
		if(bGet)
		{
			if(strategyExist.nUse ==1)
			{//�����Ѿ����ڣ����Ҵ�������״̬
				nError = CF_ERROR_DATA_STRATEGY_EXISTANDUSE;
				return false;
			}
		}
		bool bIsUpLoader = m_DataCenter.IsStrategyBelongToTrader(sDel.strategyName, sDel.strTraderName);
		if(!bIsUpLoader)
		{
			if(strategyExist.nPrivate == 1)
			{//�ò��Բ������ڸý���Ա�����Ҹò���Ϊ˽��
				nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_USE;
				return false;	
			}		
		}	
	}

	CInterface_SvrDBOpr::getObj().DB_DelStrategy(sDel.strategyName, nError);
	if(nError != CF_ERROR_SUCCESS)
		return false;
	
	CInterface_SvrDBOpr::getObj().DB_DelInstance(sDel.strategyName, nError);
	if(nError != CF_ERROR_SUCCESS)
		return false;
	
	CInterface_SvrDBOpr::getObj().DB_DelStrategy2IndexRelation(sDel.strategyName, nError);
	if(nError != CF_ERROR_SUCCESS)
		return false;

	m_DataCenter.DelStrategy(sDel.strategyName);
	m_DataCenter.DelInstance(sDel.strategyName, sDel.strTraderName);
	m_DataCenter.DelStrategy2IndexRelation(sDel.strategyName);

	CInterface_SvrTradeData::getObj().PushStrategyStopOrRun(sDel.strategyName, 1);//֪ͨ���в��Է���ֹͣ
	Sleep(500);
	TerminatInstanceThread(sDel.strategyName, sDel.strTraderName);	

	//log�����Ϣ
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nLogErrorCode; 
	LogStrategy logData;
	memset(&logData,0,sizeof(LogStrategy));

	strcpy(logData.logOperatorID, sDel.strTraderName);
	strcpy(logData.logTargetorID, sDel.strategyName);

	strcpy(logData.logTable,"LOG_STRATEGY");
	sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	logData.logAction     = Oper_Delete;
	sprintf(logData.logContent,"ɾ������(������:[%s]  ����Ա:[%s]",sDel.strategyName, sDel.strTraderName );
	logData.logResult = (CF_ERROR_SUCCESS == nError)?true:false;

	CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);
	
	return true;
}
bool CManage::AddInstance(SStrategyInstance& strategy, int& nErrorCode)
{
	CInterface_SvrDBOpr::getObj().DB_AddInstance(strategy, nErrorCode);
	if(nErrorCode != CF_ERROR_SUCCESS)
		return false;

	m_DataCenter.AddInstance(strategy);

	CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(strategy.strTraderName, strategy.strategyName, 1);
	Sleep(500);

	TerminatInstanceThread(strategy.strategyName, strategy.strTraderName);	
	BeginSStrategyInstanceProcess(strategy);

	//log�����Ϣ
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nLogErrorCode; 
	LogStrategy logData;
	memset(&logData,0,sizeof(LogStrategy));

	strcpy(logData.logOperatorID, strategy.strTraderName);
	strcpy(logData.logTargetorID, strategy.strategyName);
	
	strcpy(logData.logTable,"LOG_STRATEGY");
	sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	
	logData.logAction     = Oper_Add;
	sprintf(logData.logContent,"���ӽ��ײ��Է���(������:[%s]  ����Ա:[%s]  ��Լ��[%s]) ",strategy.strategyName, strategy.strTraderName, strategy.Instruments);
	logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

	CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);

	MessageInfo MsgInfo;
	memset(&MsgInfo,0,sizeof(MessageInfo));
	safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),"���Ӳ��Է���");
	sprintf(MsgInfo.szContent,"���ӽ��ײ��Է���(������:[%s]  ����Ա:[%s]  ��Լ��[%s]) ",strategy.strategyName, strategy.strTraderName, strategy.Instruments);
	SendMessage2Trader(strategy.strTraderName, MsgInfo);
	return true;
}
bool CManage::ModifyInstance(SStrategyInstance& strategy, int& nErrorCode)
{
	CInterface_SvrDBOpr::getObj().DB_AddInstance(strategy, nErrorCode);
	if(nErrorCode != CF_ERROR_SUCCESS)
		return false;

	m_DataCenter.AddInstance(strategy);

	CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(strategy.strTraderName, strategy.strategyName, 1);
	Sleep(500);

	TerminatInstanceThread(strategy.strategyName, strategy.strTraderName);		
	BeginSStrategyInstanceProcess(strategy);

	//log�����Ϣ
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nLogErrorCode; 
	LogStrategy logData;
	memset(&logData,0,sizeof(LogStrategy));

	strcpy(logData.logOperatorID, strategy.strTraderName);
	strcpy(logData.logTargetorID, strategy.strategyName);

	strcpy(logData.logTable,"LOG_STRATEGY");
	sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	logData.logAction     = Oper_Modify;
	sprintf(logData.logContent,"�޸Ľ��ײ��Է���(������:[%s]  ����Ա:[%s]  ��Լ��[%s]) ",strategy.strategyName, strategy.strTraderName, strategy.Instruments);
	logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

	CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);

	MessageInfo MsgInfo;
	memset(&MsgInfo,0,sizeof(MessageInfo));
	safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),"���Է����޸�");
	sprintf(MsgInfo.szContent,"�޸Ľ��ײ��Է���(������:[%s]  ����Ա:[%s]  ��Լ��[%s]) ",strategy.strategyName, strategy.strTraderName, strategy.Instruments);
	SendMessage2Trader(strategy.strTraderName, MsgInfo);
	return true;
}
bool CManage::DelInstance_ByTrader(SDelStrategy& sDel, int& nErrorCode)
{
	//bool bIsUpLoader = m_DataCenter.IsStrategyBelongToTrader(sDel.strategyName, sDel.strTraderName);
	//if(!bIsUpLoader)
	//{		
	//	//�ò��Է����������ڸý���Ա
	//	nErrorCode = CF_ERROR_DATA_INSTANCE_NOTBELONGTOUPLOADER;
	//	return false;			
	//}	
	CInterface_SvrDBOpr::getObj().DB_DelInstance_ByTrader(sDel.strategyName, sDel.strTraderName, nErrorCode);
	if(nErrorCode != CF_ERROR_SUCCESS)
		return false;

	m_DataCenter.DelInstance(sDel.strategyName, sDel.strTraderName);
	
	CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(sDel.strTraderName, sDel.strategyName, 1);//1��ʱ���ǿ�ת
	Sleep(500);
	TerminatInstanceThread(sDel.strategyName, sDel.strTraderName);	
	//log�����Ϣ
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nLogErrorCode; 
	LogStrategy logData;
	memset(&logData,0,sizeof(LogStrategy));

	strcpy(logData.logOperatorID, sDel.strTraderName);
	strcpy(logData.logTargetorID, sDel.strategyName);

	strcpy(logData.logTable,"LOG_STRATEGY");
	sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	logData.logAction     = Oper_Delete;
	sprintf(logData.logContent,"ɾ�����ײ��Է���(������:[%s]  ����Ա:[%s]  ",sDel.strategyName, sDel.strTraderName);
	logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

	CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);

	MessageInfo MsgInfo;
	memset(&MsgInfo,0,sizeof(MessageInfo));
	safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),"ɾ�����Է���");
	sprintf(MsgInfo.szContent,"ɾ�����ײ��Է���(������:[%s]  ����Ա:[%s]  ",sDel.strategyName, sDel.strTraderName);
	SendMessage2Trader(sDel.strTraderName, MsgInfo);
	return true;
}
bool CManage::UseInstance(SUseStrategy& useStrategy, int& nError)
{
	if(useStrategy.nUserType == 2)
	{//�����ն�
		SStrategyInstance instance;
		bool bGet = m_DataCenter.GetInstance(useStrategy.strategyName, useStrategy.strTraderName, instance);
		if(!bGet)
		{
			nError = CF_ERROR_DATA_INDEX_NOTEXIST;
			return false;		
		}
		bool bIsUpLoader = m_DataCenter.IsInstanceBelongToTrader(useStrategy.strategyName, useStrategy.strTraderName);
		if(!bIsUpLoader)
		{
			//�ò��Է����������ڸý���Ա
			nError = CF_ERROR_DATA_INSTANCE_NOTBELONGTOUPLOADER;
			return false;	

		}	
	}
	int nStatus = 0;//SStrategyInstance��1,0��ʾ��ͣ
	if(useStrategy.nStatus ==1)
		nStatus = 1;
	CInterface_SvrDBOpr::getObj().DB_SetInstanceStatus(useStrategy.strategyName, useStrategy.strTraderName, nStatus, nError);
	if(nError != CF_ERROR_SUCCESS)
		return false;

	m_DataCenter.SetInstanceStatus(useStrategy.strategyName, useStrategy.strTraderName, nStatus);
	//���ò���
	if(useStrategy.nStatus == 1)
	{
		STraderInstance instance(useStrategy.strategyName, useStrategy.strTraderName);
		if(m_mapInstance2Handle.find(instance) == m_mapInstance2Handle.end())
		{
			std::map<STraderInstance, SStrategyInstance> mapStrategyInstance;
			m_DataCenter.GetAllInstance(mapStrategyInstance);
			std::map<STraderInstance, SStrategyInstance>::iterator it = mapStrategyInstance.find(instance);
			if(it != mapStrategyInstance.end()) 
			{
				SStrategyInstance& instance = it->second;
				//����
				BeginSStrategyInstanceProcess(instance);
			}
		}
	}
	if(useStrategy.nStatus == 0)//ͣ��
		CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(useStrategy.strTraderName, 
			useStrategy.strategyName,  1);
	else if(useStrategy.nStatus == 1)//����
		CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(useStrategy.strTraderName, 
			useStrategy.strategyName, 0);
	//log�����Ϣ
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nLogErrorCode; 
	LogStrategy logData;
	memset(&logData,0,sizeof(LogStrategy));

	strcpy(logData.logOperatorID, useStrategy.strTraderName);
	strcpy(logData.logTargetorID, useStrategy.strategyName);

	strcpy(logData.logTable,"LOG_STRATEGY");
	sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	logData.logAction     = Oper_Modify;
	std::string str;
	str = useStrategy.nStatus ==1?"����":"ͣ��";
	sprintf(logData.logContent,"%s���ײ��Է���(������:[%s]  ����Ա:[%s] ) ",str.c_str(), useStrategy.strategyName, useStrategy.strTraderName);
	logData.logResult = (CF_ERROR_SUCCESS == nError)?true:false;

	CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);


	MessageInfo MsgInfo;
	memset(&MsgInfo,0,sizeof(MessageInfo));
	safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),"���Է���״̬�ı�");
	sprintf(MsgInfo.szContent,"%s���ײ��Է���(������:[%s]  ����Ա:[%s] ) ",str.c_str(), useStrategy.strategyName, useStrategy.strTraderName);
	SendMessage2Trader(useStrategy.strTraderName, MsgInfo);
	return true;
}
bool CManage::OnOffLineProcessInstance(std::string strTrader, bool bOnline)
{
	std::map<STraderInstance, SStrategyInstance> mapStrategyInstance;
	m_DataCenter.GetAllInstance(mapStrategyInstance);
	std::map<STraderInstance, SStrategyInstance>::iterator it = mapStrategyInstance.begin();
	while(it != mapStrategyInstance.end())
	{
		SStrategyInstance& instance = it->second;
		if(instance.nRunAfterOffline == false )
		{//����
			if(bOnline)
			{
				if(instance.nStart == 1)
				{//PushStrategyInstanceStopOrRun �������1����ʱ���ǿ�ת��0��ʱ��ʼ����
					CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(strTrader, instance.strategyName, 0);
					Strategy_LOGINFO("����Ա���ߣ�PushStrategyInstanceStopOrRun  [����]\n");
					//BeginSStrategyInstanceProcess(instance);
					//log�����Ϣ
					SYSTEMTIME sysTime;
					::GetLocalTime(&sysTime);
					int nLogErrorCode; 
					LogStrategy logData;
					memset(&logData,0,sizeof(LogStrategy));

					strcpy(logData.logOperatorID, instance.strTraderName);
					strcpy(logData.logTargetorID, instance.strategyName);

					strcpy(logData.logTable,"LOG_STRATEGY");
					sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
						sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

					logData.logAction     = Oper_Modify;
					sprintf(logData.logContent,"����Ա��������,���Է���(������:[%s]  ����Ա:[%s]  ��Լ��[%s]) ",instance.strategyName, instance.strTraderName, instance.Instruments);
					logData.logResult = true;

					CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);

					MessageInfo MsgInfo;
					memset(&MsgInfo,0,sizeof(MessageInfo));
					safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),"���Է�������");
					sprintf(MsgInfo.szContent,"����Ա[%s]����,���Է���:[%s] ��ʼ����;\n ", instance.strTraderName, instance.strategyName);
					SendMessage2Trader(instance.strTraderName, MsgInfo);
				}
			}
			else
			{
				if(strcmp(strTrader.c_str(),instance.strTraderName) ==0)
				{
					CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(strTrader, instance.strategyName, 1);
					Strategy_LOGINFO("����Ա���ߣ�PushStrategyInstanceStopOrRun  [ֹͣ]\n");
					//log�����Ϣ
					SYSTEMTIME sysTime;
					::GetLocalTime(&sysTime);
					int nLogErrorCode; 
					LogStrategy logData;
					memset(&logData,0,sizeof(LogStrategy));

					strcpy(logData.logOperatorID, instance.strTraderName);
					strcpy(logData.logTargetorID, instance.strategyName);

					strcpy(logData.logTable,"LOG_STRATEGY");
					sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
						sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

					logData.logAction     = Oper_Modify;
					sprintf(logData.logContent,"����Ա����ֹͣ���Է���(������:[%s]  ����Ա:[%s]  ��Լ��[%s]) ",instance.strategyName, instance.strTraderName, instance.Instruments);
					logData.logResult = true;

					CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);

					MessageInfo MsgInfo;
					memset(&MsgInfo,0,sizeof(MessageInfo));
					safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),"���Է���ֹͣ");
					sprintf(MsgInfo.szContent,"����Ա[%s]����,���Է���:[%s] ֹͣ����;\n ", instance.strTraderName, instance.strategyName);
					SendMessage2Trader(instance.strTraderName, MsgInfo);
				}
			}			
		}
		it++;
	}
	return true;
}
bool CManage::DownloadAllInstance(std::string strUploader, std::vector<SStrategyInstance>& vecInstance, int& nError)
{
	std::map<STraderInstance, SStrategyInstance> mapStrategyInstance;
	m_DataCenter.GetAllInstance(mapStrategyInstance);
	std::map<STraderInstance, SStrategyInstance>::iterator it = mapStrategyInstance.begin();
	while(it != mapStrategyInstance.end())
	{
		SStrategyInstance& instance = it->second;
		if(strUploader =="" || strUploader == instance.strTraderName)//�õ����в��Է��� ���� ĳ����Ա���еĲ��Է��� 
			vecInstance.push_back(instance);
		it++;
	}
	return true;
}
void CManage::SendMessage2Trader(std::string strUserName, MessageInfo& MsgInfo)
{
	UserInfo lUserInfo;
	bool bGet = CInterface_SvrUserOrg::getObj().GetUserByName(strUserName,lUserInfo);
	if(!bGet)
		return;
	set<int> setRealUsers;
	setRealUsers.insert(lUserInfo.nUserID);

	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);		
	int nExpiredDate = sysTime.wYear*10000 + sysTime.wMonth*100 + sysTime.wDay;

	CInterface_SvrMsg::getObj().SendMsg(0, setRealUsers, conAutoMsg, nExpiredDate, 
		MsgInfo.szTitle, MsgInfo.szContent);

}

bool CManage::UploadStartAll(UploadStart& start, int& nError, std::string& strError)
{
	TiXmlDocument *doc = new TiXmlDocument();    
	doc->Parse(start.uploadstring);   
	if (&doc == NULL)   
		return false;	

	SUploadAllInfo uploadAllInfo;
	TiXmlHandle docHandle(doc);   
	TiXmlNode * uploadid = docHandle.FirstChild("uploadid").ToElement();   
	TiXmlElement * elemElem = uploadid->ToElement();   
	const char* pData  =  elemElem->GetText();
	std::string strID="";
	if(pData)
		strID = pData;
	uploadAllInfo.strID = strID;
	
	TiXmlNode *traderid = docHandle.FirstChild("traderid").ToElement();   
	TiXmlElement * elemElem1 = traderid->ToElement();   
	const char* charelemElem1  =  elemElem1->GetText();
	std::string strtraderID ="";
	if(charelemElem1)
		strtraderID = charelemElem1;

	uploadAllInfo.strTraderID = strtraderID;

	TiXmlNode * stratregyname = docHandle.FirstChild("stratregyname").ToElement();   
	TiXmlElement * elemElem2 = stratregyname->ToElement();   
	
	const char* charelemElem2  =  elemElem2->GetText();
	std::string strstratregyname = "";
	if(charelemElem2)
		strstratregyname = charelemElem2;

	uploadAllInfo.vecStrategy = CTools_Ansi::SplitString(strstratregyname.c_str(), ';');

	TiXmlNode * indexname = docHandle.FirstChild("indexname").ToElement();   
	TiXmlElement * elemElem3 = indexname->ToElement();   
	const char* charelemElem3  =  elemElem3->GetText();
	std::string strindexname = "";
	if(charelemElem3)
		strindexname = charelemElem3;
	uploadAllInfo.vecIndex = CTools_Ansi::SplitString(strindexname.c_str(), ';');

	bool bUse = HasStrategyRun(uploadAllInfo.vecStrategy, uploadAllInfo.vecIndex, strtraderID, nError, strError);
	if(!bUse)//ָ��Ͳ�������ʹ��
		return false;

	bool bBack = m_DataCenter.AddUploadAllInfo(uploadAllInfo);	
	if(!bBack)
	{		
		nError = CF_ERROR_DATA_UPLOAD_BACKFAIL;
		return false;
	}	
	return true;
}
bool CManage::HasStrategyRun(std::vector<std::string>& vecStrategy, std::vector<std::string>& vecIndex, std::string strTrader,int& nError, std::string& strError)
{
	for(int i =0; i<(int)vecStrategy.size(); i++)
	{
		std::string str = vecStrategy[i];
		SStrategy strategyExist;
		bool bGet = m_DataCenter.GetStrategy(str, strategyExist);
		if(bGet) 
		{
			if(strcmp(strategyExist.strTraderName, strTrader.c_str())!=0)
			{
				nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER;
				return false;
			}
			if(strategyExist.nUse ==1)
			{//�����Ѿ����ڣ����Ҵ�������״̬
				nError = CF_ERROR_DATA_STRATEGY_EXISTANDUSE;

				const char * lErrorString = FormatErrorCode(nError);					
				char Buf[1024];
				memset(Buf,0,sizeof(Buf));
				sprintf(Buf, lErrorString, str.c_str());
				strError = Buf;
				return false;
			}
		}
	}
	for(int i =0; i<(int)vecIndex.size(); i++)
	{
		std::string str = vecIndex[i];
		std::vector<std::string> vecStrategyName;
		m_DataCenter.GetStrategyofIndex(str, vecStrategyName);		
		for(int i =0; i< (int)vecStrategyName.size(); i++)
		{
			std::string strStrategyName = vecStrategyName[i];
			SStrategy strategyExist;
			bool bGet = m_DataCenter.GetStrategy(strStrategyName, strategyExist);
			if(bGet)
			{
				if(strategyExist.nUse ==1)
				{//�����Ѿ����ڣ����Ҵ�������״̬
					nError = CF_ERROR_DATA_IndexIsUsing;
					const char * lErrorString = FormatErrorCode(nError);					
					char Buf[1024];
					memset(Buf,0,sizeof(Buf));
					sprintf(Buf, lErrorString, str.c_str(), strStrategyName.c_str());
					strError = Buf;
					return false;
				}
			}
		}				
	}
	return true;
}
bool CManage::UploadEndAll(UploadEnd& end, int& nError)
{
	SUploadAllInfo uploadAllInfo;
	m_DataCenter.GetUploadAllInfo(end.strTraderName, uploadAllInfo);

	if(m_DataCenter.IsUploadAllInfoExist(uploadAllInfo))
	{
		bool bBack = m_DataCenter.RollBackUploadInfo(uploadAllInfo);
		if(!bBack)
		{	
			Strategy_LOGINFO("�ϴ�ʧ�ܣ��ָ���ԭ����ʧ��\n");
			nError = CF_ERROR_DATA_UPLOAD_RollBACKFAIL;
			return false;
		}	
		else
		{
			Strategy_LOGINFO("�ϴ�ʧ�ܣ��ָ���ԭ���ĳɹ�\n");
			nError = CF_ERROR_DATA_UPLOAD_FAIL;
			return false;			
		}
	}
	else
		m_DataCenter.DelUploadAllInfo(uploadAllInfo);	
	return true;
}
