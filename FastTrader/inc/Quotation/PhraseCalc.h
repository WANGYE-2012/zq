#pragma once
#include "stdafx.h"
//����ת���ĺ������Ժ���ܻ��õ�����ʱ��������


class   CPhraseCalc
{
public:	
	bool CPhraseCalc::ChangePhrase(vector<SKLine *> &vecKLineSrc, EnumPhrase enumPhraseSource, vector<SKLine *> &vecKLineDest, EnumPhrase enumPhraseDest);
protected:
	bool ChangePhrase_Day2Week(vector<SKLine *>& vecKLineSrc, vector<SKLine *>& vecKLineDest);
	
};
