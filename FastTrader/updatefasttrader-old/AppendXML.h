#ifndef APPENDXML__H__
#define APPENDXML__H__


#include "tinyxml.h"
#include "tinystr.h"
#include <string>
using namespace std;
#include <iostream>
#include <vector>
#include <map>
typedef map<string, string> MapUnique;
struct SAttribute 
{
	string strItem; //ĳһ���ڵ�
	string strAttri;//���Ե�Ψһ�Ա�ʶ
	string strValue;//���Ե�ֵ
	SAttribute(string str, string str2, string str3)
	{
		strItem  = str;
		strAttri = str2;
		strValue = str3;
	}
};

typedef vector< SAttribute*> vecAttri;


class AppendXML
{
private:
	string m_strUnique;
	string m_strFile;
	string m_strUpdateFile;
private:
	bool GetUniqueMap(string strFileName, MapUnique& mapUnique);
	bool IsItemExist(TiXmlNode* pSrcChild,TiXmlNode* pParentDest, vecAttri& VecStr, vecAttri& vecTemp, MapUnique& mapUnique);
	bool InsertItem(TiXmlNode* pParentDest, vecAttri& VecStr,  vecAttri& vecTemp, string strValue = "");//nType��ʾ��Ԫ�ػ���ֵ����
	void ParaseUpdateXml(TiXmlNode* pParent,TiXmlNode* pParentDest, vecAttri& VecStr,  MapUnique& mapUnique);

	bool compareVersion(const char* localVersion,const char* remoteVersion);

public:
   AppendXML(string strUnique, string strFile, string strUpdateFile)
   {
	   m_strUnique		= strUnique;
	   m_strFile		= strFile;
	   m_strUpdateFile = strUpdateFile;
   }
   void	SetFiles(string strUnique, string strFile, string strUpdateFile);
   bool ProcessFile();
};

#endif // APPENDXML__H__