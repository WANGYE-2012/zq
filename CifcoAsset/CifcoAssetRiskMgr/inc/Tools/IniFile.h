// IniFile.h: interface for the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIFILE_H__FC455F2F_486A_4A75_861C_E25A1262AC67__INCLUDED_)
#define AFX_INIFILE_H__FC455F2F_486A_4A75_861C_E25A1262AC67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Tools_Ansi.h"
#include "Tools_Win32.h"
#include <sys/types.h>										//must include TYPES.H before STAT.H
#include <sys/stat.h>
#include <vector>

class CIniFile  
{
public:
	CIniFile(const char *pIniFileName=NULL,int iUseImgID=0);
	virtual ~CIniFile();


	//ȡ��·����ini�ļ������������RltIniFileName��ߴ���빻������ʱ�����ڵ�ǰĿ¼��Ȼ�����¼���configĿ¼��������ϼ���configĿ¼������ini�ļ����ڵ�·��������Ҳ�����RltIniFileNameΪ��
	void GetIniFileName(char *RltIniFileName,char *RltIniFilePath,const char *pMainPath,const char *pIniFileName);
	//Tell you whether a ini file is exist
	bool IsIniFileExist(const char *filename);


	//ȡָ����key_val�Ե�����
	int GetKeyValCount(const char* title,const char* key,int TitleOrd=0);
	//get configure data in string value from ini file
	void GetIniKeyStr(const char *title,const char *key,const char *defstr,char *RltBuf,int RltSize,int SearchOrd=0,int TitleOrd=0);
	//get configure data in integer value from ini file 
	int GetIniKeyVal(const char *title,const char *key,const char *defstr,int SearchOrd=0,int TitleOrd=0); 
	int GetIniKeyVal(const char *title,const char *key,int defval,int SearchOrd=0,int TitleOrd=0);
	double GetIniKeyVal2(const char *title,const char *key,double defval,int SearchOrd=0,int TitleOrd=0);
	std::vector<double> GetIniKeyVal3(const char *title,const char *key,int SearchOrd=0,int TitleOrd=0);
	std::vector<int> GetIniKeyVal4(const char *title,const char *key,int SearchOrd=0,int TitleOrd=0);
	char GetIniKeyChar(const char *title,const char *key,char defchar,int SearchOrd=0,int TitleOrd=0);

	//get configurre data in IP value from ini file
	DWORD GetIniKeyIP(const char *title,const char *key,const char *defstr=NULL,int SearchOrd=0,int TitleOrd=0);
	int GetIniKey_time_t(const char *title,const char *key,char *deftm,int SearchOrd=0,int TitleOrd=0);
	unsigned int GetIniKeyColor(const char *title,const char *key,unsigned int defcolor,int SearchOrd=0,int TitleOrd=0);

	bool GetKey_Val(const char *title,char *RltKey,int RltKeySize,char *RltBuf,int RltBufSize,int SearchOrd,int TitleOrd=0);//��ָ��title����һ��key

	//Tell you whether a rootnode is in the inifile
	bool HasIniRootNode(const char *title,int TitleOrd=0);
	//Tell you whether a rootnode/key is in the inifile, rtn 1:rootnode/key is in the inifile; 0:isn't in
	bool HasIniKey(const char *title,const char *key,int SearchOrd=0,int TitleOrd=0);
	//tell you whether the inifile was update 
	bool IsIniFileUpdate(void);
	//Load inifile content to img
	void LoadIniFileImg(void);


	//ȡһ�ڵ���βλ�á�title:������pNodeStartPos��pNodeEndPos�����սڵ���βλ�õ�ָ��
	bool GetNodePos(const char *title,int *pNodeStartPos,int *pNodeEndPos,int TitleOrd=0);		
	//ȡָ��key�е���βλ�ã�title:������key:������pKeyStartPos��pKeyEndPos�����ռ�����βλ�õ�ָ��
	bool GetKeyPos(const char *title,const char *key,int *pKeyStartPos,int *pKeyEndPos,int SearchOrd=0,int TitleOrd=0);


	//ɾ��ָ�����ƵĽ�
	bool DeleteNode(const char *title,int TitleOrd=0);											
	//ɾ��ָ�����Ƶ�key
	bool DeleteKey(const char *title,const char *key,int SearchOrd=0,int TitleOrd=0);							
	bool AddKeyStr(const char *title,const char *key,char *keystr,int TitleOrd=0);
	bool AddKeydwordVal(const char *title,const char *key,unsigned intkeyval,int TitleOrd=0);
	bool AddKeyintVal(const char *title,const char *key,int keyval,int TitleOrd=0);
	//�ı�һ����ֵ������
	bool UpdateKeyStr(const char *title,const char *key,const char *keystr,int SearchOrd=0,int TitleOrd=0);			
	//�ı�һ����ֵ������
	bool UpdateKeyVal(const char *title,const char *key,int val,int SearchOrd=0,int TitleOrd=0);				
	bool UpdateKeyVal2(const char *title,const char *key,double val,int SearchOrd=0,int TitleOrd=0);				
	bool UpdateKeyVal3(const char *title,const char *key,std::vector<double>& vals,int SearchOrd=0,int TitleOrd=0);				
	bool UpdateKeyVal4(const char *title,const char *key,std::vector<int>& vals,int SearchOrd=0,int TitleOrd=0);	
    bool UpdateKeyVal5(const char *title,const char *key,std::vector<std::string>& vals,int SearchOrd=0,int TitleOrd=0);	
	bool UpdatedwordKeyVal(const char *title,const char *key,unsigned int val,int SearchOrd=0,int TitleOrd=0);				
	bool AddOneNode(const char *pNodeBuf);


	//ȡ����ָ��Keyֵ��Title����ţ������ж��ͬ��Title��ini,return TitleOrd,-1Ϊ�Ҳ���
	int GetTitleOrdByKey_Str(const char *title,const char *key,const char *pKeystr);
	int GetTitleOrdByVal(const char *title,const char *key,const int KeyVal);

	char			m_IniFileName[256];
	int				m_UseImgID;

private:
	struct _stat	m_IniFileStat;

	char			*m_pIniFileImg;
	int				m_IniFileLen;

	//ͬ������
	char			*m_pSem;												


	//���ļ���ȡ��һ�����ݣ����m_UseImgID,����ļ�ӳ����ȡ
	bool GetLineContent_Internal(char* pRltBuf,int RltSize,char* fp,int&FileOff);
	//����ini���ݣ���Ҫ��ע�����ݹ��˵��������˫�����У����˫������ȡ���ַ�����������ͷβ�Ŀո�
	void FilterContent_Internal(const char* pBuf,int BufLen,int& StartPos,int& EndPos);

	bool AddKeyStr_Internal(const char *title,const char *key,const char *keystr,int TitleOrd);
	void LoadIniFileImg_Internal(void);
	//function		:GetTitleOrdByKey
	//description	:1. ȡ����ָ��Keyֵ��Title����ţ������ж��ͬ��Title��ini
	//				 2. pKeystr����Ϊ��
	//return		:TitleOrd,-1Ϊ�Ҳ���
	int GetTitleOrdByKey_Str_Internal(const char *title,const char *key,const char *pKeystr);
	void GetIniKeyStr_Internal(const char *title,const char *key,const char *defstr,char *RltBuf,int RltSize,int SearchOrd,int TitleOrd);
	bool GetNodePos_Internal(const char *title,int *pNodeStartPos,int *pNodeEndPos,int TitleOrd);
	bool GetKeyPos_Internal(const char *title,const char *key,int *pKeyStartPos,int *pKeyEndPos,int SearchOrd,int TitleOrd);
	bool DeleteKey_Internal(const char *title,const char *key,int SearchOrd,int TitleOrd);
	bool UpdateKeyStr_Internal(const char *title,const char *key,const char *keystr,int SearchOrd,int TitleOrd);

};

#endif // !defined(AFX_INIFILE_H__FC455F2F_486A_4A75_861C_E25A1262AC67__INCLUDED_)
