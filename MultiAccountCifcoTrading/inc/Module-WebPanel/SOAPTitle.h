// SOAPTitle.h: interface for the SOAPTitle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOAPTitle_H__B1272FDC_06B4_4C5C_8B33_9D4A35AF336C__INCLUDED_)
#define AFX_SOAPTitle_H__B1272FDC_06B4_4C5C_8B33_9D4A35AF336C__INCLUDED_
#ifdef _USE_TOP_SPLITTER
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//SOAPʹ��XML��Ϊ�����ݸ�ʽ��������ǻ���Ҫ΢���XML Parser������XML���ݣ���������msxml3.dll�С�
#import "msxml4.dll" 
	//exclude( "IXMLDOMNode", "IXMLDOMDocument", "IXMLDOMElement", "IXMLDOMNodeList" ) 
//using namespace MSXML2;//ʹ��MSXML2�������ռ�

#import "C:\Program Files\Common Files\MSSoap\Binaries\MSSOAP30.dll" \
	exclude("IStream", "IErrorInfo", "ISequentialStream", "_LARGE_INTEGER", \
"_ULARGE_INTEGER", "tagSTATSTG", "_FILETIME")//����mssoap30.dll

using namespace MSSOAPLib30;  //ʹ��MSSOAPLib30�������ռ䣬��Ҫ��װSOAP Toolkit3.0
class SoapGuideThread : public wxThread
{
public:
	SoapGuideThread(wxWindow* parent,wxString url);
	~SoapGuideThread();
	void SetParam(wxString s_maxCount);
	void* Entry();
private:
	wxWindow* m_parent;
	wxString m_url;
	wxString m_maxCount;
};


class CResearchReportThread : public wxThread
{
public:
	CResearchReportThread(wxWindow* parent,wxString url);
	~CResearchReportThread();
	void SetParam(wxString s_type,wxString s_categoryId,wxString s_fieldId,wxString s_maxCount);
	void* Entry();
private:
	wxWindow* m_parent;
	wxString m_url;
	wxString m_type;
	wxString m_categoryId;
	wxString m_fieldId;
	wxString m_maxCount;
};
class SoapGuideArticleThread : public wxThread
{
public:
	SoapGuideArticleThread(wxWindow* parent,wxString url,wxString articleId);
	~SoapGuideArticleThread();
	void* Entry();
private:
	wxWindow* m_parent;
	wxString m_url;
	wxString m_articleId;
};

#endif //_USE_TOP_SPLITTER
#endif // !defined(AFX_SOAPTitle_H__B1272FDC_06B4_4C5C_8B33_9D4A35AF336C__INCLUDED_)