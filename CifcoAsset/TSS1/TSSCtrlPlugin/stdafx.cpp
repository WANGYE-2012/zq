// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// TSSCtrlPlugin.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"


UINT nDataPkgArriveMSGID = ::RegisterWindowMessage("TSSCtrlPlugin Data Package Arrive Message ID");
UINT nDataPkgArriveNextMSGID = ::RegisterWindowMessage("TSSCtrlPlugin Data Package Arrive Next Message ID");


CString _GetIniFile()
{
	DWORD dwRetLen = 0;
	char strTemp[1024];

	ZeroMemory(strTemp, sizeof(strTemp));
	dwRetLen = GetCurrentDirectory(sizeof(strTemp), strTemp);
	if(strTemp[dwRetLen-1]!='\\')
		strcat_s(strTemp, sizeof(strTemp)-1, "\\");
	strcat_s(strTemp, sizeof(strTemp)-1, INIFILE);
	return strTemp;
}

UINT _GetDataPkgArriveMSGID()
{
	return nDataPkgArriveMSGID;
}
