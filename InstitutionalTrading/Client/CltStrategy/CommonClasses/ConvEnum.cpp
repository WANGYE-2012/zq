
#include "stdafx.h"
#include "..\CommonClasses\ConvEnum.h"

const char* IndiPara2String(IndicatorParaType eType)
{
	const static char* szIntegerType="int";
	const static char* szDoubleType="double";
	const static char* szBooleanType="bool";

	switch(eType)
	{
	case IndicatorParaType::IntegerType:return szIntegerType;
	case IndicatorParaType::DoubleType:return szDoubleType;
	case IndicatorParaType::BooleanType:return szBooleanType;
	default: return szIntegerType;
	}
}
IndicatorParaType IndiPara2Enum(const string& eType)
{
	const static std::string strInteger="int";
	const static std::string strDouble="double";
	const static std::string strBoolean="bool";

	if(eType==strInteger)
		return IndicatorParaType::IntegerType;
	if(eType==strDouble)
		return IndicatorParaType::DoubleType;
	if(eType==strBoolean)
		return IndicatorParaType::BooleanType;
	else
		return IndicatorParaType::IntegerType;
}
IndicatorType IndiType2Enum(const string& eType)
{
	const static std::string strBar="K��ָ��";
	const static std::string strMD="��������ָ��";

	if(eType==strBar)
		return IndicatorType::BarIndicator;
	if(eType==strMD)
		return IndicatorType::MDIndicator;
	else
		return IndicatorType::BarIndicator;
}
IndicatorFile IndiFile2Enum(const string& eType)
{
	const static std::string strPriceIndicatorFile="�۸�ָ��";
	const static std::string strTradingVolumeFile="������ָ��";
	const static std::string strIndicatorSampleFile="ָ������";

	if(eType==strPriceIndicatorFile)
		return IndicatorFile::PriceIndicatorFile;
	if(eType==strTradingVolumeFile)
		return IndicatorFile::TradingVolumeFile;
	if(eType==strIndicatorSampleFile)
		return IndicatorFile::IndicatorSampleFile;
	else
	    return IndicatorFile::IndicatorSampleFile;
}
DisplayType DisplayType2Enum(const string& eType)
{
	const static std::string strEditBox="�ı���";
	const static std::string strRadioBox="��ѡ��";
	const static std::string strDropDownBox="������";
	const static std::string strBOOLBox="����";
	const static std::string strCYCLEBox="����";
	const static std::string strDirectionBox="��������";
	const static std::string strOffsetFlagBox="��ƽ����";
	const static std::string strTimeBox="����ʱ�������";
	const static std::string strDateBox="���������";

	if(eType==strEditBox)
		return DisplayType::EditBox;
	if(eType==strRadioBox)
		return DisplayType::RadioBox;
	if(eType==strDropDownBox)
		return DisplayType::DropDownBox;
	if(eType==strBOOLBox)
		return DisplayType::BOOLBox;
	if(eType==strCYCLEBox)
		return DisplayType::CYCLEBox;
	if(eType==strDirectionBox)
		return DisplayType::DirectionBox;
	if(eType==strOffsetFlagBox)
		return DisplayType::OffsetFlagBox;
	if(eType==strTimeBox)
		return DisplayType::TimeBox;
	/*if(eType==strDateBox)
		return DisplayType::DateBox;*/
	else
	    return DisplayType::EditBox;
}
const char* DisplayType2String(DisplayType eType)
{
	const static char* strEditBox="�ı���";
	const static char* strRadioBox="��ѡ��";
	const static char* strDropDownBox="������";
	const static char* strBOOLBox="����";
	const static char* strCYCLEBox="����";
	const static char* strDirectionBox="��������";
	const static char* strOffsetFlagBox="��ƽ����";
	const static char* strTimeBox="ʱ�������";
	//const static char* strDateBox="���������";

	switch(eType)
	{
	case 0:return strEditBox;
	case 1:return strRadioBox;
	case 2:return strDropDownBox;
	case 3:return strBOOLBox;
	case 4:return strCYCLEBox;
	case 5:return strDirectionBox;
	case 6:return strOffsetFlagBox;
	case 7:return strTimeBox;
	//case 8:return strDateBox;
	default: return strEditBox;
	}
}
const char* DirectionType2String(StrategyDirection eType)
{
	const static char* strDirectionIN ="IN";
	const static char* strDirectionOUT ="OUT";
	const static char* strDirectionINOUT="INOUT";

	switch(eType)
	{
	case 0:return strDirectionIN;
	case 1:return strDirectionOUT;
	case 2:return strDirectionINOUT;
	default: return strDirectionIN;
	}
}
StrategyParaType StrategyType2Enum(const string& eType)
{
	const static std::string strStrategyIntegerType ="int";
	const static std::string strStrategyDoubleType ="double";
	const static std::string strStrategyBooleanType="bool";
	const static std::string strStrategyStringType="String";
	const static std::string strStrategyDateTimeType="DateTime";
	const static std::string strStrategyCycleType="Cycle";
	const static std::string strSubStrategyType="Strategy";

	if(eType==strStrategyIntegerType )
		return StrategyParaType::StrategyIntegerType ;
	if(eType==strStrategyDoubleType )
		return StrategyParaType::StrategyDoubleType ;
	if(eType==strStrategyBooleanType)
		return StrategyParaType::StrategyBooleanType;
	if(eType==strStrategyStringType)
		return StrategyParaType::StrategyStringType;
	if(eType==strStrategyDateTimeType)
		return StrategyParaType::StrategyDateTimeType;
	if(eType==strStrategyCycleType)
		return StrategyParaType::StrategyCycleType;
	if(eType==strSubStrategyType)
		return StrategyParaType::SubStrategyType;
	else
		return StrategyParaType::StrategyIntegerType ;
}
const char* Bool2String( bool bData)
{
	const static char* strTrue="true";
	const static char* strFalse="false";
	if ( bData == true)
		return strTrue;
	else
		return strFalse;
}
const char* StrategyType2String(StrategyParaType eType)
{
	const static char* szIntegerType="int";
	const static char* szDoubleType="double";
	const static char* szBooleanType="bool";
	const static char* szStringType="String";
	const static char* szDateTimeType="DateTime";
	const static char* szCycleType="Cycle";
	const static char* szStrategyType="Strategy";

	switch(eType)
	{
	case StrategyParaType::StrategyIntegerType:return szIntegerType;
	case StrategyParaType::StrategyDoubleType:return szDoubleType;
	case StrategyParaType::StrategyBooleanType:return szBooleanType;
	case StrategyParaType::StrategyStringType:return szStringType;
	case StrategyParaType::StrategyDateTimeType:return szDateTimeType;
	case StrategyParaType::StrategyCycleType:return szCycleType;
	case StrategyParaType::SubStrategyType:return szStrategyType;
	default: return szIntegerType;
	}
}
StrategyDirection StrategyDirection2Enum( const string& eType)
{
	const static std::string strDirectionIN="IN";
	const static std::string strDirectionOUT="OUT";
	const static std::string strDirectionINOUT="INOUT";

	if(eType==strDirectionIN)
		return StrategyDirection::DirectionIN;
	if(eType==strDirectionOUT)
		return StrategyDirection::DirectionOUT;
	if(eType==strDirectionINOUT)
		return StrategyDirection::DirectionINOUT;
	else
		return StrategyDirection::DirectionIN;
}
const string ExchangeName2ID( const string& name)
{
	const static std::string strCZCE="֣����";
	const static std::string strDCE="������";
	const static std::string strSHFE="������";
	const static std::string strCFFEX="�н���";

	if(name==strCZCE)
		return std::string("CZCE");
	if(name==strDCE)
		return std::string("DCE");
	if(name==strSHFE)
		return std::string("SHFE");
	if(name==strCFFEX)
		return std::string("CFFEX");
	return "";
}
const string ExchangeID2String(const string& ExchangeID)
{
	const string SHFE="������";
	const string CZCE="֣����";
	const string DCE="������";
	const string CFFEX="�н���";

	if(ExchangeID=="SHFE")
		return SHFE;
	else if(ExchangeID=="CZCE")
		return CZCE;
	else if(ExchangeID=="DCE")
		return DCE;
	else if(ExchangeID=="CFFEX")
		return CFFEX;
	else
		return "";
}
std::string Int2String(int field)
{

	char strVolume[128];
	memset(strVolume,0,sizeof(strVolume));
	sprintf(strVolume, "%d",field);
	return strVolume;

}
std::string Double2String(double field)
{
	double InvalideValue=util::GetDoubleInvalidValue();
	if (field != InvalideValue)
	{
		char strVolume[128];
		memset(strVolume,0,sizeof(strVolume));
		if (field > pow(10.0, 16))
		{
			sprintf(strVolume, "%e",field);
		}
		else
		{
			sprintf(strVolume, "%0.2f",field);
		}
		return strVolume;
	}
	else
		return "";
}
std::string long2String(long field)
{
	char strVolume[128];
	memset(strVolume,0,sizeof(strVolume));
	sprintf(strVolume, "%ld",field);
	return strVolume;

}