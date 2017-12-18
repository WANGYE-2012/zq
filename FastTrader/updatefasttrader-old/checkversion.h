//#ifdef CHECKVERSION_EXPORTS
//#define CHECKVERSION_API __declspec(dllexport)
//#else
//#define CHECKVERSION_API __declspec(dllimport)
//#endif

#pragma once 
//extern "C" DLL_EXPORT
#include "stdafx.h"
#include <string>
#include "VersionInfor.h"
/*----------------------------------------------------------------------------------------------------------------
FastTrader_Update.xml �ļ���ʽ���£�
<update>   
	<version>2,5,0,6</version> 
	<version_min>1,2,0,1</version_min> 
	<versionInfo>1.o&#x0D;&#x0A;2.oo&#x0D;&#x0A;3.ooo</versionInfo> 
	<url>http://www.cifco.net/special/updatefile</url> 
	<AppendFiles>
	</AppendFiles>
	<ReplaceFiles>	 
		<FileName name ="1.layout" Path="layout"          MD5 =""/> 
		<FileName name ="2.layout" Path="layout"          MD5 =""/>
		<FileName name ="3.layout" Path="layout"          MD5 =""/>
		<FileName name ="4.layout" Path="layout"          MD5 =""/>
		<FileName name ="cfg.xml" Path="layout"          MD5 =""/>
		<FileName name ="AutoUpdate.xml" Path=""          MD5 =""/> 	
		<FileName name ="FastTrader.exe" Path=""          MD5 =""/>
		<FileName name ="help.chm" Path=""          			 MD5 =""/>
		<FileName name ="HypertextNotice.exe" Path=""     MD5 =""/>			
		<FileName name ="language.csv" Path=""            MD5 =""/>
		<FileName name ="layout.txt" Path=""              MD5 =""/>				
		<FileName name ="platform.xml" Path=""              MD5 =""/>				 
		<FileName name ="ServerParam.xml" Path=""              MD5 =""/>	
		<FileName name ="thostmduserapi.dll" Path=""              MD5 =""/>
		<FileName name ="thosttraderapi.dll" Path=""              MD5 =""/>			
		<FileName name ="updatefasttrader.exe" Path=""              MD5 =""/>					 
		</ReplaceFiles >  
	<ExeFiles>
		<FileName name ="FastTraderSetup.exe" Path=""         MD5 =""/>
	</ExeFiles>
	<datetime>2012-06-10</datetime>          
</update>  
-----------------------------------------------------------------------------------------------------------------*/
struct SFileInfo
{
	string strFileName;
	string strFilePath;
	string strFileVersion;
	string strFileMD5;

	bool	 bSameMD5;         //���MD5һ����������
	wxString strFullPathExe;   //��װ��������·��
	wxString strFullPathUpdate;//���������������ļ�����·��
	wxString strFullPathRemote;//Զ�������ļ�����·��
	void init(string strName, string strPath, string strVersion, string strMD5)
	{
		strFileName = strName;
		strFilePath = strPath;
		strFileVersion = strVersion;
		strFileMD5     = strMD5;

		bSameMD5	  = false;
	}
};
typedef vector<SFileInfo *> VecFileInfo;

struct UpdateInfo
{//FastTrader_Update.xml �����ļ���������Ϣ����ȡ������ṹ������
	string      strVersion;
	string      strRemotePath;
	VecFileInfo vecAppendFiles;
	VecFileInfo vecReplaceFiles;
	VecFileInfo vecExeFiles;//����֮ǰ�汾������version_min�İ汾ʱ�򣬸��ǰ�װ
	string      strDateTime;
};

DECLARE_EVENT_TYPE(wxEVT_UPDATE_PROGRESS, 10008)
class CheckVersion 
{
public:
	CheckVersion(){};
 CheckVersion(std::string localVersion,std::string remoteVersion);
~CheckVersion();
bool NeedUpdate();

bool GetClientVersion(std::string& UserVersion);
bool GetSourceTextFile(std::string theUrl,std::string Filename);
bool GetServerVersion(std::string &remoteVersion);
bool HasBigVersion(string strLocal, string strRemote);
private:
    std::string m_localVersion;
    std::string m_remoteVersion;


	wxString    m_strVersionInfo;
	std::string m_remoteVersion_Min;//��С�汾�ţ�С������汾�ţ�����Ҫ���ǰ�װ
protected:
	UpdateInfo  m_UpdateInfo;

   bool  ReadAllFile(string strUpdateFile, UpdateInfo& updateInfo);

   bool DirExist(const char *pszDirName);
   bool CreateDir(const char *pszDirName);
   bool DownLoadOneFile(SFileInfo *&p, string strExePath, string strLoaclPath, string strRemotePath);

   void ProcessIniFile(LPCTSTR pFilePath, LPCTSTR pFileDest);//ini�ļ���׷�ӹ���
   void GetFileName(std::string &filename,bool bWithExt);
public:
   bool DownLoadAllFile();
   bool DoUpdate();
   static bool DownLoadUpdateXML_ForMainExe();//Ϊ�����������ж�������XML�ļ�

   wxWindow* m_window;
   void SetMsgWindow(wxWindow* window);
   void ProcessMsg(wxString str, double dbPrecnt);
   bool GetFileName(wxString& strPath, wxString& strFileName);
   bool IsProcExist(wxString strProc);

   bool m_bCancelUpdate;
   void SetCancelUpdate(bool bCancelUpdate)
   { 
	   m_bCancelUpdate = bCancelUpdate;
   }
   BOOL mkdirEx(wxString lpPath);

   std::map<int, std::string > m_mapSeq2File;
   int m_nSeq;
   CReadWriteLock m_Seq2File_Lock;

   void GetmapSeq2File(std::map<int, std::string >& mapSeq2File);
   
   SConfigPara m_sPara;
   bool ReadConfig();
   bool QueryUpdateMainFile();
   bool m_bMainFileOut;//�������ļ��յ����˳�

   bool QueryUpdateFile(std::string strUpdateFile);
   std::string  m_strLastFile;



  
 };