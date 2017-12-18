// checkversion.cpp : Defines the exported functions for the DLL application.
//
//#define CHECKVERSION_EXPORTS
#include "stdafx.h"
#include "checkversion.h"
//using namespace std
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <limits>
#include <windows.h>
#include "InternetGetFile.h"
#pragma comment(lib, "ws2_32.lib")	
#pragma comment(lib, "version.lib")
#pragma warning( disable : 4996 )
 #include <Tlhelp32.h>
#include "updatefasttrader.h"
char g_Url[1024];
//#define CLINETNAME "E:\\��GUI\\FastTrader\\bin\\zq.exe"


DEFINE_EVENT_TYPE(wxEVT_UPDATE_PROGRESS)
CheckVersion::CheckVersion(std::string localVersion,std::string remoteVersion):
m_localVersion(localVersion),
m_remoteVersion(remoteVersion)

{
    m_strVersionInfo = "";
	m_window = NULL;

	m_bCancelUpdate = false;

	m_nSeq = 1;
	m_bMainFileOut =false;

	m_strLastFile = "";
}
CheckVersion::~CheckVersion()
{
	for(vector<SFileInfo*>::iterator iter = m_UpdateInfo.vecAppendFiles.begin(); iter!= m_UpdateInfo.vecAppendFiles.end(); iter++)
	{
		SFileInfo *p = *iter;
		if(p)
		{
			delete p;
			p= NULL;
		}
	}
	for(vector<SFileInfo*>::iterator iter = m_UpdateInfo.vecReplaceFiles.begin(); iter!= m_UpdateInfo.vecReplaceFiles.end(); iter++)
	{
		SFileInfo *p = *iter;
		if(p)
		{
			delete p;
			p= NULL;
		}
	}
	for(vector<SFileInfo*>::iterator iter = m_UpdateInfo.vecExeFiles.begin(); iter!= m_UpdateInfo.vecExeFiles.end(); iter++)
	{
		SFileInfo *p = *iter;
		if(p)
		{
			delete p;
			p= NULL;
		}
	}
}
bool CheckVersion::HasBigVersion(string strLocal, string strRemote)
{
	int i1,i2,i3,i4;
	int j1,j2,j3,j4;
	sscanf(strLocal.c_str(),"%d,%d,%d,%d",&i1,&i2,&i3,&i4);
	sscanf(strRemote.c_str(),"%d,%d,%d,%d",&j1,&j2,&j3,&j4);

	if(i1>j1||
		i1==j1&&i2>j2||
		i1==j1&&i2==j2&&i3>j3||
		i1==j1&&i2==j2&&i3==j3&&i4>j4)
	{
		//localversion>remoteversion
		return false;
	}
	else if(i1==j1&&i2==j2&&i3==j3&&i4==j4)
	{
		//local==remote
		return false;
	}
	else
	{
		//local<remote
		return true;
	}
	return false;
}
bool CheckVersion::NeedUpdate()
{
	bool bNeed = false;
	if(!GetClientVersion(m_localVersion))
	{
		LOG_INFO("�õ����ذ汾ʧ��%s;", m_localVersion.c_str());
		return bNeed;
	}	
    if(!GetServerVersion(m_remoteVersion))
	{
		LOG_INFO("�õ�Զ�̷������汾ʧ��%s;", m_remoteVersion.c_str());
		return bNeed;
	}

   	bNeed = HasBigVersion(m_localVersion, m_remoteVersion);
   
	return bNeed;
	//return false;
}




//��ȡ�ͻ��˰汾   
bool CheckVersion::GetClientVersion(std::string& localVersion)   
{ /*
#ifdef _DEBUG
    char* strClientName="zq.exe";
#else
    char* strClientName="FastTrader.exe";
#endif
*/
	string strClientName = "";
	std::string strPath;
	GetPath(strPath);
	strPath+=SERVERURL_FILENAME;
	TiXmlDocument myDocument(strPath.c_str()); 
	if(myDocument.LoadFile())
	{
		TiXmlElement *root = myDocument.RootElement();
		std::string rootVal = root->Value();
		if(rootVal != "update")
		{
			return false;
		}
		TiXmlNode* pchild =root->FirstChildElement("version");
		if(pchild)
		{
			const char *p =root->FirstChildElement("version")->GetText();
			if(p != NULL)
				strClientName = p;
		}
		else
		{
			OutputDebugString(_T("�����޷��������У�AutoUpdate.xml�ļ�����û�а汾��Ϣ"));
			return false;
		}
		
	}
	else
	{
		//strClientName="FastTrader.exe";
	}	

    CVersionInfo info;
	if(strClientName!= "" &&  info.Load(strClientName.c_str()))
	{
		wxString wxVersion=info.GetFileVersionAsString();
		localVersion=wxVersion;   
		return TRUE;
	}
	return FALSE;
}

//��ȡ�������İ汾  
bool CheckVersion::GetServerVersion(std::string &remoteVersion) 
{	
    std::string strPath;
    GetPath(strPath);
    wxString strUpdatePath=strPath+"Update";
    CreateDirectory(strUpdatePath.c_str(), NULL);
    wxString ConfigFilename=strPath+"Update\\userVersion.xml";
  
    TiXmlDocument doc(ConfigFilename.c_str());
    bool bRet = doc.LoadFile();

    if(!bRet)
        return false;
    TiXmlElement *root = doc.RootElement();

    std::string rootValue = root->Value();

    if(rootValue != "update")
    {
        return false;
    }
	
	TiXmlElement* pVersion = root->FirstChildElement("version");
	if(pVersion)
	  remoteVersion = pVersion->GetText();

	TiXmlElement* pVersionInfo = root->FirstChildElement("versionInfo");
	if(pVersionInfo)
		m_strVersionInfo = pVersionInfo->GetText();

	TiXmlElement* pVersionInfo_min = root->FirstChildElement("version_min");
	if(pVersionInfo_min)
		m_remoteVersion_Min = pVersionInfo_min->GetText();

	if(!ReadAllFile(ConfigFilename.c_str(), m_UpdateInfo))
	{
		wxString strInfo;
		strInfo.Printf(wxT("��ȡ�ļ�%sʧ�ܣ���رս����նˣ����ذ�װ�����ֶ���װ��"), ConfigFilename.c_str());
		ProcessMsg(strInfo, 0);
		wxString str = wxString::Format(_T("Read File %s error!\n"), ConfigFilename.c_str());
		 OutputDebugString(str.c_str());
		return false;
	}
    std::string strUrl;

    memset(g_Url,0,sizeof(g_Url));
    strUrl=root->FirstChildElement("url")->GetText();
    strncpy(g_Url,strUrl.c_str(),sizeof(g_Url)-1);

    if(remoteVersion.empty())
        return false;
    else
        return true;

   
    return false;

}
void CheckVersion::ProcessIniFile(LPCTSTR pFilePath, LPCTSTR pFileDest)   
{   
	// TODO: Add your control notification handler code here   
	TCHAR strAppNameTemp[1024];//����AppName�ķ���ֵ   
	TCHAR strKeyNameTemp[1024];//��Ӧÿ��AppName������KeyName�ķ���ֵ   
	TCHAR strReturnTemp[1024];//����ֵ   
	DWORD dwKeyNameSize;//��Ӧÿ��AppName������KeyName���ܳ���   
	//����AppName���ܳ���   
	DWORD dwAppNameSize = GetPrivateProfileString(NULL,NULL,NULL,strAppNameTemp,1024,pFilePath);   
	if(dwAppNameSize>0)   
	{   
		TCHAR *pAppName = new TCHAR[dwAppNameSize];   
		int nAppNameLen=0;  //ÿ��AppName�ĳ���   
		for(int i = 0;i<(int)dwAppNameSize;i++)   
		{   
			pAppName[nAppNameLen++]=strAppNameTemp[i];   
			if(strAppNameTemp[i]=='\0')   
			{   
				OutputDebugString(pAppName);   
				OutputDebugString(_T("\r\n"));   
				dwKeyNameSize = GetPrivateProfileString(pAppName,NULL,NULL,strKeyNameTemp,1024,pFilePath);   
				if(dwAppNameSize>0)   
				{   
					TCHAR *pKeyName = new TCHAR[dwKeyNameSize];   
					int nKeyNameLen=0;    //ÿ��KeyName�ĳ���   
					for(int j = 0;j<(int)dwKeyNameSize;j++)   
					{   
						pKeyName[nKeyNameLen++]=strKeyNameTemp[j];   
						if(strKeyNameTemp[j]=='\0')   
						{   
							OutputDebugString(pKeyName);   
							OutputDebugString(_T("="));   
							if(GetPrivateProfileString(pAppName,pKeyName,NULL,strReturnTemp,1024,pFilePath))   
							{
								TCHAR strTemp[1024];
								if(!GetPrivateProfileString(pAppName,pKeyName,NULL,strTemp,1024,pFileDest))
									WritePrivateProfileString(pAppName,pKeyName,strReturnTemp,pFileDest);
								OutputDebugString(strReturnTemp); 
							}
							else
							{
								TCHAR strTemp[1024];
								if(!GetPrivateProfileString(pAppName,pKeyName,NULL,strTemp,1024,pFileDest))
									WritePrivateProfileString(pAppName,pKeyName,_T(""),pFileDest);
								OutputDebugString(strReturnTemp); 
							}
							memset(pKeyName,0,dwKeyNameSize);   
							nKeyNameLen=0;   
							OutputDebugString(_T("\r\n"));   
						}   
					}   
					delete[]pKeyName;   

				}   
				memset(pAppName,0,dwAppNameSize);   
				nAppNameLen=0;   
			}   
		}   
		delete[]pAppName;   
	}   
} 
bool  CheckVersion::ReadAllFile(string strUpdateFile, UpdateInfo& updateInfo)
{
	TiXmlDocument doc(strUpdateFile.c_str());
	bool bRet = doc.LoadFile();
	if(!bRet)
		return false;
	TiXmlElement *root = doc.RootElement();

	TiXmlNode* pchild = root->FirstChild(_T("version"));
	if(!pchild)
		return false;
	updateInfo.strVersion = pchild->ToElement()->GetText();
	pchild = root->FirstChild(_T("url"));
	if(!pchild)
		return false;
	updateInfo.strRemotePath = pchild->ToElement()->GetText();

	TiXmlNode* pAppendFile = root->FirstChild(_T("AppendFiles"));
	if(!pAppendFile)
		return false;

	TiXmlNode* pFiles = pAppendFile->FirstChild(_T("FileName"));
	while(pFiles)
	{
		vector<string> vecAttriName;
		vector<string> vecAttriValue;
		TiXmlAttribute* attr = pFiles->ToElement()->FirstAttribute();
		while(attr)
		{
		//	cout << ""  << attr->Name() << "=" << attr->Value();				
			vecAttriName.push_back(attr->Name());
			vecAttriValue.push_back(attr->Value());
			attr = attr->Next();	
		}
		if(vecAttriName.size() != 4)
		{
			OutputDebugString(_T("file do not has four attributes.(name; path;version;MD5)"));
			return false;
		}
		SFileInfo *pInfo = new SFileInfo;
		pInfo->init(vecAttriValue[0], vecAttriValue[1], vecAttriValue[2], vecAttriValue[3]);

		updateInfo.vecAppendFiles.push_back(pInfo);
		pFiles =  pFiles->NextSibling(_T("FileName"));			
	}
	
	TiXmlNode* pReplaceFile = root->FirstChild(_T("ReplaceFiles"));
	if(!pReplaceFile)
		return false;
	 pFiles = pReplaceFile->FirstChild(_T("FileName"));
	while(pFiles)
	{
		vector<string> vecAttriName;
		vector<string> vecAttriValue;
		TiXmlAttribute* attr = pFiles->ToElement()->FirstAttribute();
		while(attr)
		{
		//	cout << ""  << attr->Name() << "=" << attr->Value();				
			vecAttriName.push_back(attr->Name());
			vecAttriValue.push_back(attr->Value());
			attr = attr->Next();	
		}
		if(vecAttriName.size() != 3)
		{
			OutputDebugString(_T("file do not has four attributes.(name; path;version;MD5)"));
			return false;
		}
		SFileInfo *pInfo = new SFileInfo;
		pInfo->init(vecAttriValue[0], vecAttriValue[1], "", vecAttriValue[2]);

		updateInfo.vecReplaceFiles.push_back(pInfo);
		pFiles =  pFiles->NextSibling(_T("FileName"));			
	}

	TiXmlNode* pExeFile = root->FirstChild(_T("ExeFiles"));
	if(!pExeFile)
		return false;
	pFiles = pExeFile->FirstChild(_T("FileName"));
	while(pFiles)
	{
		vector<string> vecAttriName;
		vector<string> vecAttriValue;
		TiXmlAttribute* attr = pFiles->ToElement()->FirstAttribute();
		while(attr)
		{
			//	cout << ""  << attr->Name() << "=" << attr->Value();				
			vecAttriName.push_back(attr->Name());
			vecAttriValue.push_back(attr->Value());
			attr = attr->Next();	
		}
		if(vecAttriName.size() != 3)
		{
			OutputDebugString(_T("file do not has four attributes.(name; path;version;MD5)"));
			return false;
		}
		SFileInfo *pInfo = new SFileInfo;
		pInfo->init(vecAttriValue[0], vecAttriValue[1], "", vecAttriValue[2]);

		updateInfo.vecExeFiles.push_back(pInfo);
		pFiles =  pFiles->NextSibling(_T("FileName"));			
	}


	pchild = root->FirstChild(_T("datetime"));
	if(!pchild)
		return false;
	updateInfo.strDateTime = pchild->ToElement()->GetText();

	return true;
}
bool CheckVersion::DownLoadOneFile(SFileInfo *&p, string strExePath, string strLoaclPath, string strRemotePath)
{
	if(p==NULL)
	{
		OutputDebugString(_T("it will not occur,but i process it"));
		return false;
	}
	
	wxString strAppendPath = strRemotePath;
	if(p->strFilePath != "")
		strAppendPath.Printf(_T("%s%s/"), strRemotePath.c_str(), p->strFilePath.c_str());
	p->strFullPathRemote = wxString::Format(_T("%s%s"), strAppendPath, p->strFileName.c_str());	//Զ���ļ���ȫ·��


	wxString strExe = strExePath;//�Ѱ�װ�汾��·��
	wxString strAppendPathSrc = strLoaclPath;
	if(p->strFilePath != "")
	{
		strExe.Printf(_T("%s%s\\"), strExePath.c_str(), p->strFilePath.c_str());
		strAppendPathSrc.Printf(_T("%s%s\\"), strLoaclPath.c_str(), strAppendPath.c_str());//�������������ļ�����ʱ·��
	}
	p->strFullPathExe = wxString::Format(_T("%s%s"), strExe, p->strFileName.c_str()); //�Ѱ�װ�汾�����ļ���ȫ·��
	p->strFullPathUpdate = wxString::Format(_T("%s%s"), strLoaclPath.c_str(),  p->strFullPathRemote.c_str());//�������������ļ���ȫ·��

	return QueryUpdateFile(p->strFullPathRemote.c_str());
}
bool CheckVersion::DownLoadAllFile()
{		
	string strExePath;//��װ��������·��
	string strLoaclPath;//�����ļ�������ʱ·��
	GetPath(strLoaclPath);
	strExePath = strLoaclPath;
	strLoaclPath += "Update\\";
	string strRemotePath = m_UpdateInfo.strRemotePath;
	if(strRemotePath.length() !=0 && strRemotePath[strRemotePath.length()-1] != _T('/'))
		strRemotePath.append("/");

	int nCount = m_UpdateInfo.vecAppendFiles.size() + m_UpdateInfo.vecReplaceFiles.size() + m_UpdateInfo.vecExeFiles.size();
	if(nCount == 0)
		return true;
	
	int nDownLoadCount = 0;
	for(vector<SFileInfo*>::iterator iter = m_UpdateInfo.vecAppendFiles.begin(); iter!= m_UpdateInfo.vecAppendFiles.end(); iter++)
	{
		if(m_bCancelUpdate)
			return false;

		SFileInfo *p = *iter;
		if(!DownLoadOneFile(p,strExePath, strLoaclPath, strRemotePath))
			return false;
	//	nDownLoadCount++;
	//	wxString strInfo;
	//	strInfo.Printf(wxT("����:%s..."), p->strFileName.c_str() );
	//	ProcessMsg(strInfo, 0.8*nDownLoadCount/nCount);//������ʱ��ֻռ20%���������Եúÿ�����Ϊ���������ؿ����
	}
	for(vector<SFileInfo*>::iterator iter = m_UpdateInfo.vecReplaceFiles.begin(); iter!= m_UpdateInfo.vecReplaceFiles.end(); iter++)
	{
		if(m_bCancelUpdate)
			return false;
		SFileInfo *p = *iter;
		if(!DownLoadOneFile(p,strExePath, strLoaclPath, strRemotePath))
			return false;
	//	nDownLoadCount++;
	//	wxString strInfo;
	//	strInfo.Printf(wxT("����:%s..."), p->strFileName.c_str() );
	//	ProcessMsg(strInfo, 0.8*nDownLoadCount/nCount);//������ʱ��ֻռ20%���������Եúÿ�����Ϊ���������ؿ����
	}
	for(vector<SFileInfo*>::iterator iter = m_UpdateInfo.vecExeFiles.begin(); iter!= m_UpdateInfo.vecExeFiles.end(); iter++)
	{
		if(m_bCancelUpdate)
			return false;
		SFileInfo *p = *iter;
		if(!DownLoadOneFile(p,strExePath, strLoaclPath, strRemotePath))
			return false;
	//	nDownLoadCount++;
	//	wxString strInfo;
	//	strInfo.Printf(wxT("����:%s..."), p->strFileName.c_str() );
	//	ProcessMsg(strInfo, 0.8*nDownLoadCount/nCount);//������ʱ��ֻռ20%���������Եúÿ�����Ϊ���������ؿ����
	}

	return true;
}

bool CheckVersion::DirExist(const char *pszDirName) 
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



// ����Ŀ¼��������Ŀ¼�����Դ����༶��Ŀ¼ 
bool CheckVersion::CreateDir(const char *pszDirName) 
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
			bRet = !!::CreateDirectory(_szTmp, NULL); //û��WARING ��������̾��
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

bool CheckVersion::DoUpdate()
{
	bool bNeed = HasBigVersion(m_localVersion, m_remoteVersion_Min);
	int errcode=0;    
    if(wxGetApp().argc>1)
	{
		while(1)
		{
			wxString strPath = wxGetApp().argv[1];
			wxString strFileName;
			GetFileName(strPath, strFileName);
			if(IsProcExist(strFileName))
			{
				int iret = wxMessageBox("��ر��������еĽ����նˣ���Ȼ����������ʧ�ܣ��رպ󣬵��\"��\"������","��ʾ",wxYES_NO);
				if( iret == wxNO)
				{
					wxString strInfo;
					strInfo = wxT("����ʧ��,�����ڻ������ն�û�йرգ������س����ֶ�����");
					ProcessMsg(strInfo, 0.8);
					return false;
				}
				Sleep(100);
			}
			else
				break;
		}
	}
	
	if(bNeed)
	{//���ذ汾����С�汾С������������
		for(vector<SFileInfo*>::iterator iter = m_UpdateInfo.vecExeFiles.begin(); iter!= m_UpdateInfo.vecExeFiles.end(); iter++)
		{
			SFileInfo *p = *iter;
			if(p->bSameMD5)
			{	//md5ֵһ�����ļ���û�����ع�	
				continue;
			}
			 ShellExecute(NULL, _T("open"),p->strFullPathUpdate, _T(""), _T(""), SW_NORMAL);

			 wxString strInfo = wxT("������ɣ�");
			 ProcessMsg(strInfo, 1);
		}
		return true;
	}
	int nCount = m_UpdateInfo.vecAppendFiles.size() + m_UpdateInfo.vecReplaceFiles.size() + m_UpdateInfo.vecExeFiles.size();
	if(nCount == 0)
		return true;

	int nDownLoadCount = 1;
	SFileInfo* pUnique = NULL;
	for(vector<SFileInfo*>::iterator iter = m_UpdateInfo.vecAppendFiles.begin(); iter!= m_UpdateInfo.vecAppendFiles.end(); iter++)
	{//���ѭ����Ҫ�ҳ�unique.xml
		if(m_bCancelUpdate)
			return false;
		SFileInfo *p = *iter;
		if(p->strFileName == "unique.xml")
		{
			pUnique = p;
			nDownLoadCount++;
			wxString strInfo;
			strInfo.Printf(wxT("����:%s..."), p->strFileName.c_str());
			ProcessMsg(strInfo, 0.8+0.2*nDownLoadCount/nCount);
			break;
		}
	}

	for(vector<SFileInfo*>::iterator iter = m_UpdateInfo.vecAppendFiles.begin(); iter!= m_UpdateInfo.vecAppendFiles.end(); iter++)
	{
		if(m_bCancelUpdate)
			return false;
		SFileInfo *p = *iter;
		mkdirEx(p->strFullPathExe);
		if(p->bSameMD5)
		{	//md5ֵһ�����ļ���û�����ع�	
			continue;
		}
		if(p->strFileName != "unique.xml")
		{
			wxString strFile = p->strFullPathUpdate.c_str();
			if(strFile.Length()>4)
			{
				if(strFile.Right(4).MakeLower() == _T(".xml")||strFile.Right(4).MakeLower() == _T(".txt"))
				{//׷��xml�ļ�
					AppendXML appendFile(pUnique->strFullPathUpdate.c_str(), p->strFullPathExe.c_str(), p->strFullPathUpdate.c_str());
					if(!appendFile.ProcessFile())
					{
						nDownLoadCount++;
						wxString strInfo;
						strInfo.Printf(wxT("����%sʧ�ܣ��Զ����������п����ļ�ʧ�ܣ���رս����նˣ����ذ�װ�����ֶ���װ��"), p->strFileName);
						ProcessMsg(strInfo, 0.8+0.2*nDownLoadCount/nCount);
						wxString str = wxString::Format(_T("update file from [%s] to [%s] error, the unique file is [%s]!\n"), p->strFullPathExe.c_str(), p->strFullPathUpdate.c_str(), pUnique->strFileName.c_str());
						OutputDebugString(str.c_str());			
						return false;
					}
					nDownLoadCount++;
					wxString strInfo;
					strInfo.Printf(wxT("����:%s..."), p->strFileName.c_str());
					ProcessMsg(strInfo, 0.8+0.2*nDownLoadCount/nCount);
				}
				else if(strFile.Right(4).MakeLower() == _T(".ini"))
				{//׷��ini�ļ�
					ProcessIniFile(p->strFullPathUpdate.c_str(), p->strFullPathExe.c_str());
					nDownLoadCount++;
					wxString strInfo;
					strInfo.Printf(wxT("����:%s..."), p->strFileName.c_str());
					ProcessMsg(strInfo, 0.8+0.2*nDownLoadCount/nCount);
				}
			}
			
		}
	}
	string strFileName;
	GetFileName(strFileName, true);
	for(vector<SFileInfo*>::iterator iter = m_UpdateInfo.vecReplaceFiles.begin(); iter!= m_UpdateInfo.vecReplaceFiles.end(); iter++)
	{
		if(m_bCancelUpdate)
			return false;
		SFileInfo *p = *iter;
		if(p->bSameMD5)
		{	//md5ֵһ�����ļ���û�����ع�	
			continue;
		}
		if(p->strFileName == strFileName)
			continue;//����������������������ʧ�ܣ�������Ե�����������ȥ����

		mkdirEx(p->strFullPathExe);
		if(!CopyFile( p->strFullPathUpdate,p->strFullPathExe, false))
		{
			::MessageBox(NULL,  _T("�Զ����������п����ļ�ʧ�ܣ���رս����նˣ����ذ�װ�����ֶ���װ��"), _T("�Զ�����ʧ��"),0);
			return false;
		}
		nDownLoadCount++;
		wxString strInfo;
		strInfo.Printf(wxT("����:%s..."), p->strFileName.c_str());
		ProcessMsg(strInfo, 0.8+0.2*nDownLoadCount/nCount);
	}
	wxString strInfo;
	strInfo = wxT("�����ɹ����!");
	ProcessMsg(strInfo, 1);
	return true;
}
BOOL CheckVersion::mkdirEx(wxString lpPath)
{
	wxString pathname = lpPath;

	int end = pathname.Find('\\', true);

	int pt = pathname.find('\\');

	if(pathname[pt-1] == ':')
		pt = pathname.find('\\', pt+1);

	wxString path;
	while(pt != -1 && pt<=end)
	{
		path = pathname.Left(pt+1);
		if(_access(path, 0) == -1)
			_mkdir(path);
		pt = pathname.find('\\', pt+1);
	}
	return true;

}
void CheckVersion::GetFileName(std::string &filename,bool bWithExt)
{
	char localPath[256];
	memset(localPath, 0, 256);
	GetModuleFileName( NULL, localPath, 256);
	filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	filename=filename.substr(splitpos+1);
	if(!bWithExt)
	{
		splitpos=filename.find_last_of('.');
		filename=filename.substr(0,splitpos);
	}
}
bool CheckVersion::DownLoadUpdateXML_ForMainExe()
{
	std::string strPath;
	GetPath(strPath);
	wxString strUpdatePath=strPath+"Update";
	CreateDirectory(strUpdatePath.c_str(), NULL);
	wxString ConfigFilename=strPath+"Update\\userVersion.xml";
	std::string strServerUrl;

	//�ӿͻ��������ļ���ȡ������URL
	strPath+=SERVERURL_FILENAME;
	TiXmlDocument myDocument(strPath.c_str()); 
	if(myDocument.LoadFile())
	{
		TiXmlElement *root = myDocument.RootElement();
		std::string rootVal = root->Value();
		if(rootVal != "update")
		{
			return false;
		}
		TiXmlElement* pURL = root->FirstChildElement("url");
		if(pURL)
			strServerUrl = pURL->GetText();        
	}
	else
	{
		//  strServerUrl="http://www.cifco.net/special/updatefile/FastTrader_Update.xml";
	}
	/*�ӷ��������������ļ� */
	if (Internet::InternetGetURL(strServerUrl.c_str(),ConfigFilename.c_str(), NULL, NULL)
		== Internet::INTERNET_SUCCESS)
	{
		return true;

	}
	return false;	
}
void CheckVersion::SetMsgWindow(wxWindow* window)
{
	m_window = window;
}
void CheckVersion::ProcessMsg(wxString str, double dbPrecnt)
{
	wxCommandEvent myEvent(wxEVT_UPDATE_PROGRESS);
	myEvent.SetString(str);
	myEvent.SetClientData(&dbPrecnt);
	m_window->ProcessEvent(myEvent);
}
bool CheckVersion::GetFileName(wxString& strPath, wxString& strFileName)
{
	int nPos  = strPath.Find('\\', true);
	if(nPos != -1)
		strFileName = strPath.Right(strPath.Length() - nPos-1);
	return true;
}
bool CheckVersion::IsProcExist(wxString strProc)
{
	HANDLE   hProcessSnap   =   NULL;   
	PROCESSENTRY32   pe32   =   {0};  
	// ��þ��   
	hProcessSnap   =   CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,   0);   
	if(hProcessSnap   ==   (HANDLE)-1)   
	{   
		return false;   
	}   
	pe32.dwSize   =   sizeof(PROCESSENTRY32);   

	// �о����н�������   
	if   (Process32First(hProcessSnap,   &pe32))   
	{   
		do   
		{   
			wxString str;
			str.Printf(wxT("%s"),pe32.szExeFile);
			if(str == strProc)
			{
				CloseHandle   (hProcessSnap);
				return true;				
			}
		}   
		while   (Process32Next(hProcessSnap,   &pe32)); 
	}   
	// �رվ��   
	CloseHandle   (hProcessSnap);
	return false;
}

bool CheckVersion::ReadConfig()
{
	string strClientName = "";
	std::string strPath;
	GetPath(strPath);
	strPath+=SERVERURL_FILENAME;
	TiXmlDocument myDocument(strPath.c_str()); 
	if(myDocument.LoadFile())
	{
		TiXmlElement *root = myDocument.RootElement();
		std::string rootVal = root->Value();
		if(rootVal != "update")
		{
			return false;
		}
		TiXmlNode* pchild =root->FirstChildElement("address");
		if(pchild)
		{
			const char *p =root->FirstChildElement("address")->GetText();
			if(p != NULL)
			{
				std::string str = p;
				int nPos = str.find(':');
				if ( nPos != -1 )
				{
					m_sPara.strIP = str.substr(0, nPos);
					m_sPara.nPort = atoi(str.substr(nPos+1, str.length()).c_str());					
				}
			}
		}
		TiXmlNode* pchildVersion  =root->FirstChildElement("version");
		if(pchildVersion)
		{
			const char *p =root->FirstChildElement("version")->GetText();
			if(p != NULL)
			{			
				m_sPara.strVersion = p;					
			}
		}
		TiXmlNode* pMainFile  =root->FirstChildElement("MainFile");
		if(pMainFile)
		{
			const char *p =root->FirstChildElement("MainFile")->GetText();
			if(p != NULL)
			{			
				m_sPara.strMainFile = p;					
			}
		}		
	}
	return true;
}
bool CheckVersion::QueryUpdateMainFile()
{	
	return QueryUpdateFile(m_sPara.strMainFile);
}
bool CheckVersion::QueryUpdateFile(std::string strUpdateFile)
{
	sGet sGetFile;
	strcpy(sGetFile.mPath, strUpdateFile.c_str());
	if( !CTCPDataService::GetInstance()->SendData(CMDID_SvrGet, &sGetFile, sizeof(sGetFile), m_nSeq, false))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();		
		return false;;
	}
	else
	{
		m_Seq2File_Lock.write_lock();
		m_mapSeq2File.insert(make_pair(m_nSeq, strUpdateFile));
		++m_nSeq;
		m_Seq2File_Lock.write_unlock();
	}

	return true;
}
void CheckVersion::GetmapSeq2File(std::map<int, std::string >& mapSeq2File)
{
	m_Seq2File_Lock.write_lock();
	mapSeq2File = m_mapSeq2File;
	m_Seq2File_Lock.write_unlock();
}