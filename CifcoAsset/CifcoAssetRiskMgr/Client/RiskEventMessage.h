#pragma once
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public ref class RiskEventEventArgs : EventArgs // �¼�������
{
public:
	RiskEventEventArgs()
	{

	}
	RiskEventEventArgs( int nID)
	{
		this->nID = nID;		
	}
	void SetStr(String^ strPara)
	{
		str = strPara;
	}

	int nID;
	String^ str;	
};

