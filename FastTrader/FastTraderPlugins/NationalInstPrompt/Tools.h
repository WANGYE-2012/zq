// common-AlgoTradingNo1Dlg.h : ���屾��Ŀ��ͨ�ýṹ��
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include <string>
#include <sstream>
#include <vector>
using namespace std;



class CTools
{
public:

    static bool IsHaveDot(std::string& str)
    {
        return (std::string::npos == str.find('.'))?false:true;
    }
	static const string StringFromDouble(double dval)
	{
		char buf[256];
		memset(buf,0,sizeof(buf));
		sprintf_s(buf,sizeof(buf)-1,"%.10f",dval);
		if(strchr(buf,'.')!=NULL)
		{
			int len=strlen(buf);
			while(len>1&&buf[len-1]=='0'&&buf[len-2]!='.')
			{
				buf[len-1]=0;
				len--;
			}
		}
		return string(buf);
	};
	static const string StringFromInt(int ival)
	{
		char buf[256];
		memset(buf,0,sizeof(buf));
		sprintf_s(buf,sizeof(buf)-1,"%d",ival);
		return string(buf);
	}
	//��CEdit��ȡ��doubleֵ
	static double GetDoubleFromCEdit(CEdit& edit)
	{
		char buf[256];
		memset(buf,0,sizeof(buf));
		edit.GetWindowTextA(buf,sizeof(buf)-1);
		return atof(buf);
	}
	//��doubleֵ���õ�CEdit��
	static void SetDoubleToCEdit(CEdit& edit,double dval)
	{
		edit.SetWindowTextA(CTools::StringFromDouble(dval).c_str());
	}
	//��intֵ���õ�CEdit��
	static void SetIntToCEdit(CEdit& edit,int ival)
	{
		edit.SetWindowTextA(CTools::StringFromInt(ival).c_str());
	}
	static void SetStringToCEdit(CEdit& edit,const char* pval)
	{
		edit.SetWindowTextA(pval);
	}
	//��CComboBox��ȡ��string
	static std::string GetStringFromCComboBox(CComboBox& comboBox)
	{
		char buf[256];
		memset(buf,0,sizeof(buf));
		comboBox.GetWindowTextA(buf,sizeof(buf)-1);
		return string(buf);
	}
	//��CEdit��ȡ��string
	static std::string GetStringFromCEdit(CEdit& edit)
	{
		char buf[256];
		memset(buf,0,sizeof(buf));
		edit.GetWindowTextA(buf,sizeof(buf)-1);
		return string(buf);
	}
    //��CDateTimeCtrl��ȡ������ֵ
    static CTime GetCTimeFromCDateTimeCtrl(CDateTimeCtrl& edit)
    {
        CTime l_time;
        edit.GetTime(l_time);
        return l_time;
    }
    //������ֵ���õ�CDateTimeCtrl��
    static void SetCTimeToCDateTimeCtrl(CDateTimeCtrl& edit,const CTime* pVal)
    {
        edit.SetTime(pVal);
    }
};