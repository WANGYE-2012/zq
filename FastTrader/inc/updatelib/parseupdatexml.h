#pragma once

//����update.xml
//�ļ�����ʾ�����£�
//<update>
//   <version>1,5,0,7</version>
//   <version_min>1,3,2,84</version_min>
//   <version_svrpath>update_std_01050007</version_svrpath>
//   <ReplaceFiles>
//		<FileName>FastTrader.exe</FileName>
//   	<FileName>layout\1.layout</FileName>
//   </ReplaceFiles>  
//   <note>����˵��</note>
//</update>  

#include "stdafx.h"

#include <string>
#include <map>
using std::string;
using std::map;




class CParseUpdateXml
{
public:
    CParseUpdateXml(const string& strUpdateXmlFileName);
    ~CParseUpdateXml();


    void Parse(const string& strUpdateXmlFileName);

    //������update.xml������
    string          m_version;                  //�������汾�ţ�1.5.0.7
    string          m_version_min;              //�ͻ��˿���������Ͱ汾�š����ڴ˰汾�ŵ�ֻ�����°�װ
    string          m_version_svrpath;          //�����ļ��ڷ������ϵ����·��
    map<string,bool>m_replacefiles;             //��Ҫ�����滻���ļ����ϡ�first��filename, second��ʾ�Ƿ��Ѿ�����
    string          m_note;                     //����˵��
    bool            m_bvalid;                   //�Ƿ���Ч

    //�Ƚ������汾������1.5.7.1��1,5,7,1������1/0,-1
    static int CompareVersion(const string& ver1,const string& ver2);
    //���汾����ʽ��Ϊ1.5.7.1
    static string FormatVersion(const string& ver);

    //����������������
	CParseUpdateXml(const CParseUpdateXml&);
	CParseUpdateXml& operator= (const CParseUpdateXml&);
};
