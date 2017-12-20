
#include "stdafx.h"
#include "DataManage.h"
#include "..\CommonClasses\ConvEnum.h"

CDataManage* CDataManage::m_pInstance = NULL;
int g_Count = 0;


CDataManage* CDataManage::NewInstance()
{
	g_Count ++;
	if( m_pInstance == NULL ) 
	{
		m_pInstance = new CDataManage();
	}

	return m_pInstance;
}

void CDataManage::DestroyInstance()
{
	g_Count --;
	if(g_Count == 0 && m_pInstance != NULL) 
	{		
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

void CDataManage::Clear()
{
	
}

CDataManage::CDataManage()
{
	Clear();
}

CDataManage::~CDataManage()
{
}


bool CDataManage::CheckIndicatorName(const std::string& name)
{
	std::map<std::string, CIndicatorDataEx>::iterator it = m_mapDownloadIndicators.find(name);
	if (it == m_mapDownloadIndicators.end())
	{
		return true;//û������
	}
	else
		return false;
}
//bool CDataManage::CheckStrategyParaName(const std::string& name)
//{
//	std::map<std::string,StrategyPara>::iterator it = m_mapStrategyPara.find(name);
//	if (it == m_mapStrategyPara.end())
//	{
//		return true;//û������
//	}
//	else
//		return false;
//}
//const char* CDataManage::IndiPara2String(IndicatorParaType eType)
//{
//
//	const static char* szIntegerType="int";
//	const static char* szDoubleType="double";
//	const static char* szBooleanType="bool";
//	switch(eType)
//	{
//	case IndicatorParaType::IntegerType:return szIntegerType;
//	case IndicatorParaType::DoubleType:return szDoubleType;
//	case IndicatorParaType::BooleanType:return szBooleanType;
//	default: return szIntegerType;
//	}
//}
//IndicatorParaType CDataManage::IndiPara2Enum( const string& eType)
//{
//	const static std::string strInteger="int";
//	const static std::string strDouble="double";
//	const static std::string strBoolean="bool";
//
//
//	if(eType==strInteger)
//		return IndicatorParaType::IntegerType;
//	if(eType==strDouble)
//		return IndicatorParaType::DoubleType;
//	if(eType==strBoolean)
//		return IndicatorParaType::BooleanType;
//	else
//	return IndicatorParaType::IntegerType;
//}

/**����һ��ָ��*/
void CDataManage::AddIndcator(CIndicatorDataEx& data)
{
	m_mapIndicatorInfo[data.GetIndicatorName()] = data;
}
void CDataManage::AddDownloadIndicator(CIndicatorDataEx& data)
{
	m_mapDownloadIndicators[data.GetIndicatorName()] = data;
}
/*�õ�һ��ָ������в���*/
bool CDataManage::GetIndiPara(const string& IndiName,std::map<std::string, IndicatorPara> &  lMapFiled)
{
	std::map<std::string, CIndicatorDataEx>::iterator it = m_mapIndicatorInfo.find(IndiName);
	if (it!=m_mapIndicatorInfo.end())
	{
	    it->second.GetIndiPara(lMapFiled);
		return true;
	}
	return false;
}
/*ɾ��һ��ָ��*/
void CDataManage::DeleteIndcator( const string& name)
{
	std::map<std::string, CIndicatorDataEx>::iterator it =m_mapIndicatorInfo.find(name);
	if (it!=m_mapIndicatorInfo.end())
	{
		m_mapIndicatorInfo.erase(it);
	}
	
	std::map<std::string, CIndicatorDataEx>::iterator it2 = m_mapDownloadIndicators.find(name);
	if (it2 !=m_mapDownloadIndicators.end())
	{
		m_mapDownloadIndicators.erase(it2);
	}
}
//IndicatorType CDataManage::IndiType2Enum( const string& eType)
//{
//	const static std::string strBar="K��ָ��";
//	const static std::string strMD="��������ָ��";
//
//
//
//	if(eType==strBar)
//		return IndicatorType::BarIndicator;
//	if(eType==strMD)
//		return IndicatorType::MDIndicator;
//	else
//
//	return IndicatorType::BarIndicator;
//}
//IndicatorFile CDataManage::IndiFile2Enum( const string& eType)
//{
//	const static std::string strPriceIndicatorFile="�۸�ָ��";
//	const static std::string strTradingVolumeFile="������ָ��";
//	const static std::string strIndicatorSampleFile="ָ������";
//
//
//
//	if(eType==strPriceIndicatorFile)
//		return IndicatorFile::PriceIndicatorFile;
//	if(eType==strTradingVolumeFile)
//		return IndicatorFile::TradingVolumeFile;
//	if(eType==strIndicatorSampleFile)
//		return IndicatorFile::IndicatorSampleFile;
//	else
//	    return IndicatorFile::IndicatorSampleFile;
//}
//
//DisplayType CDataManage::DisplayType2Enum(const string& eType)
//{
//
//	const static std::string strEditBox="�ı���";
//	const static std::string strRadioBox="��ѡ��";
//	const static std::string strDropDownBox="������";
//	const static std::string strBOOLBox="����";
//	const static std::string strCYCLEBox="����";
//	const static std::string strDirectionBox="��������";
//	const static std::string strOffsetFlagBox="��ƽ����";
//	const static std::string strTimeBox="����ʱ�������";
//	const static std::string strDateBox="���������";
//
//
//
//	if(eType==strEditBox)
//		return DisplayType::EditBox;
//	if(eType==strRadioBox)
//		return DisplayType::RadioBox;
//	if(eType==strDropDownBox)
//		return DisplayType::DropDownBox;
//	if(eType==strBOOLBox)
//		return DisplayType::BOOLBox;
//	if(eType==strCYCLEBox)
//		return DisplayType::CYCLEBox;
//	if(eType==strDirectionBox)
//		return DisplayType::DirectionBox;
//	if(eType==strOffsetFlagBox)
//		return DisplayType::OffsetFlagBox;
//	if(eType==strTimeBox)
//		return DisplayType::TimeBox;
//	/*if(eType==strDateBox)
//		return DisplayType::DateBox;*/
//	else
//
//	    return DisplayType::EditBox;
//}
//const char* CDataManage::DisplayType2String(DisplayType eType)
//{
//	const static char* strEditBox="�ı���";
//	const static char* strRadioBox="��ѡ��";
//	const static char* strDropDownBox="������";
//	const static char* strBOOLBox="����";
//	const static char* strCYCLEBox="����";
//	const static char* strDirectionBox="��������";
//	const static char* strOffsetFlagBox="��ƽ����";
//	const static char* strTimeBox="ʱ�������";
//	//const static char* strDateBox="���������";
//
//	switch(eType)
//	{
//	case 0:return strEditBox;
//	case 1:return strRadioBox;
//	case 2:return strDropDownBox;
//	case 3:return strBOOLBox;
//	case 4:return strCYCLEBox;
//	case 5:return strDirectionBox;
//	case 6:return strOffsetFlagBox;
//	case 7:return strTimeBox;
//	//case 8:return strDateBox;
//
//	default: return strEditBox;
//	}
//}
//const char* CDataManage::DirectionType2String(StrategyDirection eType)
//{
//	const static char* strDirectionIN ="IN";
//	const static char* strDirectionOUT ="OUT";
//	const static char* strDirectionINOUT="INOUT";
//
//	switch(eType)
//	{
//	case 0:return strDirectionIN;
//	case 1:return strDirectionOUT;
//	case 2:return strDirectionINOUT;
//
//	default: return strDirectionIN;
//	}
//}
//StrategyParaType CDataManage::StrategyType2Enum(const string& eType)
//{
//
//	const static std::string strStrategyIntegerType ="int";
//	const static std::string strStrategyDoubleType ="double";
//	const static std::string strStrategyBooleanType="bool";
//	const static std::string strStrategyStringType="String";
//	const static std::string strStrategyDateTimeType="DateTime";
//	const static std::string strStrategyCycleType="Cycle";
//	const static std::string strSubStrategyType="Strategy";
//
//
//
//
//	if(eType==strStrategyIntegerType )
//		return StrategyParaType::StrategyIntegerType ;
//	if(eType==strStrategyDoubleType )
//		return StrategyParaType::StrategyDoubleType ;
//	if(eType==strStrategyBooleanType)
//		return StrategyParaType::StrategyBooleanType;
//	if(eType==strStrategyStringType)
//		return StrategyParaType::StrategyStringType;
//	if(eType==strStrategyDateTimeType)
//		return StrategyParaType::StrategyDateTimeType;
//	if(eType==strStrategyCycleType)
//		return StrategyParaType::StrategyCycleType;
//	if(eType==strSubStrategyType)
//		return StrategyParaType::SubStrategyType;
//
//	else
//	return StrategyParaType::StrategyIntegerType ;
//}
//const char* CDataManage::Bool2String( bool bData)
//{
//
//	const static char* strTrue="true";
//	const static char* strFalse="false";
//	if ( bData == true)
//		return strTrue;
//	else
//		return strFalse;
//}
//const char* CDataManage::StrategyType2String(StrategyParaType eType)
//{
//
//	const static char* szIntegerType="int";
//	const static char* szDoubleType="double";
//	const static char* szBooleanType="bool";
//	const static char* szStringType="String";
//	const static char* szDateTimeType="DateTime";
//	const static char* szCycleType="Cycle";
//	const static char* szStrategyType="Strategy";
//
//	switch(eType)
//	{
//	case StrategyParaType::StrategyIntegerType:return szIntegerType;
//	case StrategyParaType::StrategyDoubleType:return szDoubleType;
//	case StrategyParaType::StrategyBooleanType:return szBooleanType;
//	case StrategyParaType::StrategyStringType:return szStringType;
//	case StrategyParaType::StrategyDateTimeType:return szDateTimeType;
//	case StrategyParaType::StrategyCycleType:return szCycleType;
//	case StrategyParaType::SubStrategyType:return szStrategyType;
//	
//	default: return szIntegerType;
//	}
//}
//StrategyDirection CDataManage::StrategyDirection2Enum( const string& eType)
//{
//	const static std::string strDirectionIN="IN";
//	const static std::string strDirectionOUT="OUT";
//	const static std::string strDirectionINOUT="INOUT";
//
//
//
//	if(eType==strDirectionIN)
//		return StrategyDirection::DirectionIN;
//	if(eType==strDirectionOUT)
//		return StrategyDirection::DirectionOUT;
//	if(eType==strDirectionINOUT)
//		return StrategyDirection::DirectionINOUT;
//	else
//	return StrategyDirection::DirectionIN;
//}
//�ж�������ַ����Ƿ���INT
bool CDataManage::IsInteger(string& str)
{
	bool isInt;
	char strChar=0;
	for( int i=0;i <str.length();i++)
	{
		isInt=false;
		for(int j=0;j <10;j++)
		{
			itoa(j,&strChar,10);
			if(str.substr(i,1).compare(&strChar) == 0 )
			{
				isInt=true;
				break;
			}
		}
		if(!isInt)
			break;
	}
	return isInt;
}

///*����һ�����Բ���*/
//void CDataManage::AddStrategyPara(StrategyPara& data)
//{
//	m_mapStrategyPara[data.szTitle] = data;
//}
///*�õ����в��Բ���map*/
//void CDataManage::GetStrategyPara(const string& name,std::map<std::string,StrategyPara>& data)
//{
//    data =m_mapStrategyPara;
//}
///*ɾ��һ�����Բ���*/
//void CDataManage::DeleteStrategyPara( const string& name)
//{
//	std::map<std::string, StrategyPara>::iterator it =m_mapStrategyPara.find(name);
//	if (it != m_mapStrategyPara.end())
//	{
//		m_mapStrategyPara.erase(it);
//	}
//	map<string,list<StrategyPrarSubItem>> ::iterator it2 =m_mapPrarSubItem.find(name);
//	if (it2 != m_mapPrarSubItem.end())
//	{
//		m_mapPrarSubItem.erase(it2);
//	}
//	int nCnt = m_multimapstrategyParaType2Name.count(name);
//	multimap<string,string>::iterator it3 = m_multimapstrategyParaType2Name.find(name);
//	for ( int i = 0; i< nCnt; i++)
//	{					 
//
//		string title(name);
//		if ( it3->second  == title)
//		{
//			it3 = m_multimapstrategyParaType2Name.erase(it3);
//			break;
//		}
//		else
//		{
//			++it3;
//		}
//
//	}
//	
//}
///*����һ������ָ��*/
//void CDataManage::AddStrategyIndicator(const string& Strategyname,StrategyIndicator& data)
//{
//	m_mapStrategyIndicator[data.szName] = data;
//	m_multimapStrategy2Indicator.insert(make_pair(Strategyname,data.szName));
//}
///*ɾ��һ������ָ��*/
//void CDataManage::DeleteStrategyIndicator(const string& Strategyname,const string& Indicatorname)
//{
//	std::map<std::string, StrategyIndicator>::iterator it =m_mapStrategyIndicator.find(Indicatorname);
//	if (it != m_mapStrategyIndicator.end())
//	{
//		m_mapStrategyIndicator.erase(it);
//	}
//	int nCnt = m_multimapStrategy2Indicator.count(Strategyname);
//	multimap<string,string>::iterator it2 = m_multimapStrategy2Indicator.find(Strategyname);
//	for ( int i = 0; i< nCnt; i++)
//	{					 
//
//		string indi(Indicatorname);
//		if ( it2->second  == indi)
//		{
//			it2 = m_multimapStrategy2Indicator.erase(it2);
//			break;
//		}
//		else
//		{
//            ++it2;
//		}
//		
//	}
//}
/*�õ����в���*/
void CDataManage::GetAllStrategy(std::map<std::string, CStrategyDataEx> & lMapFiled)
{
	
	lMapFiled = m_mapStrategyInfo;


}
void CDataManage::GetAllStrategy_Download(std::map<std::string, CStrategyDataEx> & lMapFiled, bool bOnlyMyself)
{
	if(bOnlyMyself) {
		std::map<std::string, CStrategyDataEx>::iterator it;
		for(it = m_mapDownloadStrategies.begin(); 
				it != m_mapDownloadStrategies.end(); it++) {
			if(!it->second.IsOwner(_GetCurrUserID())) 
				continue;
			lMapFiled[it->first] = it->second;
		}
	}
	else {
		lMapFiled = m_mapDownloadStrategies;
	}
}

/*�õ�����ָ��*/
void CDataManage::GetAllIndicator( std::map<std::string, CIndicatorDataEx> & lMapFiled)
{

	lMapFiled = m_mapIndicatorInfo;


}
void CDataManage::GetAllIndicator_Download( std::map<std::string, CIndicatorDataEx> & lMapFiled, bool bOnlyMyself)
{
	if(bOnlyMyself) {
		std::map<std::string, CIndicatorDataEx>::iterator it;
		for(it = m_mapDownloadIndicators.begin(); 
				it != m_mapDownloadIndicators.end(); it++) {
			if(!it->second.IsOwner(_GetCurrUserID())) 
				continue;
			lMapFiled[it->first] = it->second;
		}
	}
	else {
		lMapFiled = m_mapDownloadIndicators;
	}
}
/*ͨ��ָ�����õ�ָ����Ϣ*/
bool CDataManage::GetIndicatorInfobyName(const string& IndiName,CIndicatorDataEx& data)
{
	std::map<std::string, CIndicatorDataEx>::iterator it = m_mapIndicatorInfo.find(IndiName);
	if (it != m_mapIndicatorInfo.end())
	{
		data = it->second;
		return true;
	}
	else
		return false;

}

//void CDataManage::SetParaType2NameMap(multimap<string,string>& data)
//{
//	m_multimapstrategyParaType2Name.clear();
//	m_multimapstrategyParaType2Name = data;
//}
//void CDataManage::GetParaType2NameMap(multimap<string,string>& data)
//{
//	data = m_multimapstrategyParaType2Name;
//}
///*�õ��½����Ե�����ָ������*/
//void CDataManage::GetAllStrategyIndicator(std::map<std::string,StrategyIndicator>& lMapFiled)
//{
//	lMapFiled = m_mapStrategyIndicator;
//}
///*���ò��Բ�����ѡ��������ʱ��ѡ��*/
//void CDataManage::SetParaSubItem(map<string,list<StrategyPrarSubItem>>& PrarSubItem)
//{
//	m_mapPrarSubItem.clear();
//    m_mapPrarSubItem = PrarSubItem;
//}
///*�õ����Բ�����ѡ��������ʱ��ѡ��*/
//void CDataManage::GetParaSubItem(map<string,list<StrategyPrarSubItem>>&  PrarSubItem)
//{
//	PrarSubItem = m_mapPrarSubItem;
//	m_mapPrarSubItem.clear();
//}
//void CDataManage::GetSubItem(list<StrategyPrarSubItem>& listSubItem)
//{
//	
//	listSubItem =it->second;
//	
//}
//void CDataManage::ClearIndicatiorMap()
//{
//    m_mapIndcatorPara.clear();
//}
//void CDataManage::ClearStrategyMap()
//{
//
//	m_mapStrategyPara.clear();
//	m_multimapstrategyParaType2Name.clear();
//}
void CDataManage::AddStrategy(CStrategyDataEx& data)
{
    m_mapStrategyInfo[data.GetStrategyName()] = data;
}
void CDataManage::AddDownloadStrategy(CStrategyDataEx& data)
{
	m_mapDownloadStrategies[data.GetStrategyName()] = data;
}
/*ɾ��һ������*/
void CDataManage::DeleteStrategy( const string& name)
{
	std::map<std::string, CStrategyDataEx>::iterator it =m_mapStrategyInfo.find(name);
	if (it!=m_mapStrategyInfo.end())
	{
		m_mapStrategyInfo.erase(it);
	}

	std::map<std::string, CStrategyDataEx>::iterator it2 = m_mapDownloadStrategies.find(name);
	if (it2 !=m_mapDownloadStrategies.end())
	{
		m_mapDownloadStrategies.erase(it2);
	}
}
void CDataManage::GetIndicator(const string& name,list<string>& data)
{
	std::map<std::string, CStrategyDataEx>::iterator it = m_mapStrategyInfo.find(name);
	if (it != m_mapStrategyInfo.end())
	{
		it->second.GetIndicator(data);
	}

}
bool CDataManage::CheckStrategyName(const std::string& name)
{
	std::map<std::string, CStrategyDataEx>::iterator it = m_mapDownloadStrategies.find(name);
	if (it == m_mapDownloadStrategies.end())
	{
		return true;//û������
	}
	else
		return false;
}
void CDataManage::SetIndicatorDiaplay(list<string>& data,bool state)
{

	list<string>::iterator it = data.begin();
	for ( ;it!=data.end(); it++)
	{
		std::map<string,int>::iterator it2 = m_mapIndicatorDiaplay.find(*it);
		if (state)
		{
			if (it2 != m_mapIndicatorDiaplay.end())
			{	
				(it2->second)++;	
			}
			else
			{
				m_mapIndicatorDiaplay[*it]=1;
			}
		}
		else
		{
			if (it2 != m_mapIndicatorDiaplay.end() && it2->second > 1)
			{

				(it2->second)--;

			}
			else if (it2 != m_mapIndicatorDiaplay.end() && it2->second == 1)
			{
				m_mapIndicatorDiaplay.erase(it2);
			}
		}
	}
	
}
bool CDataManage::IsRelativeIndi(const string& name)
{

	std::map<string,int>::iterator it = m_mapIndicatorDiaplay.find(name);
	if (it != m_mapIndicatorDiaplay.end())
	{	
		return true;
	}
	else
	{
		return false;
	}
}
void CDataManage::indicatorDisplyMapClear()
{

	m_mapIndicatorDiaplay.clear();
}
/*�ж�һ��ָ��˽�л��ǹ���*/
int CDataManage::IsPrivate_Indicator(const std::string& name)
{
	std::map<std::string, CIndicatorDataEx>::iterator it =m_mapIndicatorInfo.find(name);
	if (it!=m_mapIndicatorInfo.end())
	{
		return it->second.GetIndicatorPrivate();
	}
	else
		return 0;//�޴�ָ��
}
/*�ж�һ������˽�л��ǹ���*/
int CDataManage::IsPrivate_Strategy(const std::string& name)
{
	std::map<std::string, CStrategyDataEx>::iterator it =m_mapStrategyInfo.find(name);
	if (it!=m_mapStrategyInfo.end())
	{
		return it->second.GetStrategyPrivate();
	}
	else
		return 0;//�޴˲���
}

/*�ж�һ��ָ���Ƿ�Ϊָ���û�����*/
bool CDataManage::IsOwner_Indicator(const std::string& name, const std::string strUserID)
{
	bool bIsOwner = false;
	std::map<std::string, CIndicatorDataEx>::iterator it =m_mapDownloadIndicators.find(name);
	if (it!=m_mapDownloadIndicators.end())
	{
		bIsOwner = it->second.IsOwner(strUserID);
	}
	return bIsOwner;
}

/*�ж�һ�������Ƿ�Ϊָ���û�����*/
bool CDataManage::IsOwner_Strategy(const std::string& name, const std::string strUserID)
{
	bool bIsOwner = false;
	std::map<std::string, CStrategyDataEx>::iterator it = m_mapDownloadStrategies.find(name);
	if (it!=m_mapDownloadStrategies.end())
	{
		bIsOwner = it->second.IsOwner(strUserID);
	}
	return bIsOwner;
}

/*�õ�һ������*/
bool CDataManage:: GetStrategy( const string& name,CStrategyDataEx& data)
{
	std::map<std::string, CStrategyDataEx>::iterator it =m_mapStrategyInfo.find(name);
	if (it!=m_mapStrategyInfo.end())
	{
		data =it->second;
		return true;
	}
	else
		return false;
}
/*��m_mapDownloadStrategies�õ�һ������*/
bool CDataManage:: GetDownloadStrategy( const string& name,CStrategyDataEx& data)
{
	std::map<std::string, CStrategyDataEx>::iterator it =m_mapDownloadStrategies.find(name);
	if (it!=m_mapDownloadStrategies.end())
	{
		data =it->second;
		return true;
	}
	else
		return false;
}
/*�õ�һ��ָ��*/
bool CDataManage:: GetIndicator( const string& name,CIndicatorDataEx& data)
{
	std::map<std::string, CIndicatorDataEx>::iterator it =m_mapIndicatorInfo.find(name);
	if (it!=m_mapIndicatorInfo.end())
	{
		data =it->second;
		return true;
	}
	else
		return false;
}
bool CDataManage:: GetDownloadIndicator( const string& name,CIndicatorDataEx& data)
{
	std::map<std::string, CIndicatorDataEx>::iterator it =m_mapDownloadIndicators.find(name);
	if (it!=m_mapDownloadIndicators.end())
	{
		data =it->second;
		return true;
	}
	else
		return false;
}
bool CDataManage:: IsIndicatorExist(const string& name)
{

	std::map<std::string, CIndicatorDataEx>::iterator it =m_mapIndicatorInfo.find(name);
	if (it!=m_mapIndicatorInfo.end())
	{
		return true;
	}
	else
		return false;
}
bool CDataManage:: IsInStrategyExist(const string& name)
{
	std::map<std::string, CStrategyDataEx>::iterator it =m_mapStrategyInfo.find(name);
	if (it!=m_mapStrategyInfo.end())
	{
		return true;
	}
	else
		return false;
}
