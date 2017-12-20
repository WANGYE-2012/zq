#pragma once
#include "constants.h"
class CImportExport
{
public:
	CImportExport(int sysbaseid,int userbaseid);
	virtual ~CImportExport(void);
	//��ȡ�ⲿ�������ɲ˵���
    wxMenu* CreateLayoutMenu(wxMenu* layoutSubMenu=NULL);
	//�����ļ��ı���
    void DoConfigSave(wxString caption);
	//�����ļ�������,�����Ƿ���Ҫ���ô��ڴ�СΪ1024*768
    bool DoChangeStyle(int cmdid);
	//ɾ��һ���û�����
	void DoDeleteConfig(wxString caption);
	//�жϵ�ǰ�����Ƿ��ϵͳ�����ļ�һ��
	//bool CfgXmlIsChanged();
public:
	//��ȡ��ִ�г���·��
	wxString GetAppPath(){return m_strAppPath.c_str();}
    //��ȡ��ǰ�����ļ�ȫ·��
	wxString GetCfgFileName(){return m_strAppPath+USERCFG_FILENAME;}
    //��ȡ��ǰ�����ļ�ȫ·��
	wxString GetLayoutFileName(){return m_strAppPath+LAYOUT_FILENAME;}
    //��ȡϵͳ����·��
	wxString GetSystemLayoutPath(){return m_strAppPath+"layout\\";}
    //��ȡϵͳ�����ļ�ȫ·��
	wxString GetSystemCfgFileName(){return m_strAppPath+"syscfg.xml";}
    //��ȡϵͳ�����ļ�ȫ·��
	wxString GetSystemLayoutFileName(wxString caption){return GetSystemLayoutPath()+caption+".layout";}
    //��ȡ�û�����·��
	wxString GetUserLayoutPath(){return m_strAppPath+"userlayout\\";}
    //��ȡ�û������ļ�ȫ·��
	wxString GetUserCfgFileName(wxString caption){return GetUserLayoutPath()+caption+".xml";}
    //��ȡ�û������ļ�ȫ·��
	wxString GetUserLayoutFileName(wxString caption){return GetUserLayoutPath()+caption+".layout";}
	//����û������ļ��б�
	void GetLayoutArray(wxArrayString& arr,bool renew=true);
	//��ȡϵͳ�����ļ��б�
	void LoadSystemLayoutArray(wxArrayString& arr);
private:
	//ϵͳ���ֲ˵�����СID
	int m_SystemBaseId;
	//�û����ֲ˵�����СID
	int m_UserBaseId;
	std::string m_strAppPath;
    //
    wxArrayString m_UserLayoutCaption;

};
